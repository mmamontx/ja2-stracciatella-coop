#ifndef __COOP_H
#define __COOP_H

#include "Game_Event_Hook.h"
#include "GameSettings.h"
#include "Handle_UI.h"
#define GROUP GROUP_JA2
#include "NetworkIDManager.h"
#include "RakNet/RPC4Plugin.h"
#include "RakPeerInterface.h"
#include "ReplicaManager3.h"
#undef GROUP
#include "Merc_Hiring.h"
#include "MessageIdentifiers.h"


using namespace RakNet;

//#define COOP_DEBUG // Comment out for releases
#define COOP_DEBUG_NUM_MERCS_TOTAL      4
#define COOP_DEBUG_NUM_MERCS_PER_PLAYER 2

#define ENEMY_ENABLED

#define KEY_RETURN 13

#define MAX_NUM_PLAYERS 4

#define TIMEOUT_MS 2000

#define IS_SERVER       (gGameOptions.fNetworkClient == FALSE)
#define IS_CLIENT       (gGameOptions.fNetworkClient == TRUE)
#define IS_VALID_CLIENT (IS_CLIENT && gConnected && (gReplicaList.Size() != 0))

#define RPC_EVENT_READY (gRPC_Enable && !(gRPC_Events.empty()))

#define FOR_EACH_PLAYER(i) for (int i = 0; i < MAX_NUM_PLAYERS; i++)
#define FOR_EACH_CLIENT(i) for (int i = 1; i < MAX_NUM_PLAYERS; i++)

#define REPLICA_PROFILE_INDEX TOTAL_SOLDIERS
#define REPLICA_PLAYER_INDEX  (TOTAL_SOLDIERS + NUM_PROFILES)

#define ITEM_PTR ((gRPC_ClientIndex != -1) ? \
	gpItemPointerRPC[gRPC_ClientIndex] : gpItemPointer)
#define ITEM_PTR_SOLDIER ((gRPC_ClientIndex != -1) ? \
	gpItemPointerSoldierRPC[gRPC_ClientIndex] : gpItemPointerSoldier)

#define ITEM_PTR_CLICK_CURRENT_AP (gRPC_ItemPointerClick ? \
	gRPC_ItemPointerClick->sCurrentActionPoints : gsCurrentActionPoints)
#define ITEM_PTR_CLICK_FULL_TARGET (gRPC_ItemPointerClick ? \
	ID2Soldier(gRPC_ItemPointerClick->tgt_id) : gUIFullTarget)
#define ITEM_PTR_CLICK_SRC_SLOT (gRPC_ItemPointerClick ? \
	gRPC_ItemPointerClick->ubHandPos : gbItemPointerSrcSlot)

#define INV_CLICK_CURRENT_MERC (gRPC_InvClick ? \
	ID2Soldier(gRPC_InvClick->id) : gpSMCurrentMerc)
#define INV_CLICK_HAND_POS (gRPC_InvClick ? \
	gRPC_InvClick->ubHandPos : uiHandPos)
#define INV_CLICK_SM_CURRENT_MERC (gRPC_InvClick ? \
	pSMCurrentMercRPC : gpSMCurrentMerc)

#define EXECUTE_WHILE_MAIN_SUSPENDED(func) do { \
		SuspendThread(gMainThread); \
		func; \
		ResumeThread(gMainThread); \
	} while (0);

#define WAIT_WHILE_COND_ELSE_NET_SHUTDOWN(cond, timeout, failure_msg) do { \
	UINT16 elapsed_time = 0; \
	do { \
		Sleep(1); \
		elapsed_time += 1; \
	} while ((cond) && (elapsed_time < timeout)); \
	if (cond) \
	{ \
		SLOGW(failure_msg); \
		NetworkShutdown(); \
		break; \
	} \
} while (0);

/*
 * The following macroses maintain access to the shared objects:
 *
 * 1. For the host return local values from the original objects that are
 *    replicated to the clients.
 * 2. For the valid clients return replicated values.
 * 3. For the invalid clients return local values (which are just garbage).
 */
#define PLAYER_READY(i) (IS_VALID_CLIENT ? \
    ((PLAYER*)(gReplicaList[REPLICA_PLAYER_INDEX + i]))->ready : \
    gPlayers[i].ready)
#define PLAYER_NAME(i) (IS_VALID_CLIENT ? \
    ((PLAYER*)(gReplicaList[REPLICA_PLAYER_INDEX + i]))->name.c_str() : \
    gPlayers[i].name.c_str())
#define PLAYER_GUID(i) (IS_VALID_CLIENT ? \
    ((PLAYER*)(gReplicaList[REPLICA_PLAYER_INDEX + i]))->guid : \
    gPlayers[i].guid)

#define BS_ARRAY_READ(array, size) \
    for (int i = 0; i < size; i++) \
		deserializeParameters->serializationBitstream[0].Read(array[i])
#define BS_ARRAY_WRITE(array, size) \
    for (int i = 0; i < size; i++) \
		serializeParameters->outputBitstream[0].Write(array[i])

// The following fields are initialized in the game init options screen
struct NETWORK_OPTIONS
{
	ST::string name;
	ST::string ip;
	UINT16 port;
};

#define ID_USER_PACKET_CONNECT          ID_USER_PACKET_ENUM
#define ID_USER_PACKET_MESSAGE          (ID_USER_PACKET_ENUM + 1)
#define ID_USER_PACKET_READY            (ID_USER_PACKET_ENUM + 2)
#define ID_USER_PACKET_START            (ID_USER_PACKET_ENUM + 3)
#define ID_USER_PACKET_TEAM_PANEL_DIRTY (ID_USER_PACKET_ENUM + 4)
#define ID_USER_PACKET_TOP_MESSAGE      (ID_USER_PACKET_ENUM + 5)
#define ID_USER_PACKET_END_COMBAT       (ID_USER_PACKET_ENUM + 6)
#define ID_USER_PACKET_END_TURN         (ID_USER_PACKET_ENUM + 7)
#define ID_USER_PACKET_GAME_OPTIONS     (ID_USER_PACKET_ENUM + 8)

#define DEFINE_EMPTY_PACKET_STRUCT(name) \
	struct name \
	{ \
		unsigned char id; \
	}

DEFINE_EMPTY_PACKET_STRUCT(USER_PACKET_START);
DEFINE_EMPTY_PACKET_STRUCT(USER_PACKET_TEAM_PANEL_DIRTY);
DEFINE_EMPTY_PACKET_STRUCT(USER_PACKET_END_COMBAT);
DEFINE_EMPTY_PACKET_STRUCT(USER_PACKET_END_TURN);

#define MAX_NAME_LEN 16

struct USER_PACKET_CONNECT
{
	unsigned char id;
	char name[MAX_NAME_LEN];
	BOOLEAN ready;
};

#define MAX_MESSAGE_LEN 128

struct USER_PACKET_MESSAGE
{
	unsigned char id;
	char message[MAX_MESSAGE_LEN];
};

struct USER_PACKET_READY
{
	unsigned char id;
	BOOLEAN ready;
};

struct USER_PACKET_TOP_MESSAGE
{
	unsigned char id;
	UINT8 ubCurrentTeam;
	UINT8 ubTopMessageType;
	UINT16 usTactialTurnLimitCounter;
	UINT16 usTactialTurnLimitMax;
};

struct USER_PACKET_GAME_OPTIONS
{
	unsigned char id;
	BOOLEAN fGunNut;
	BOOLEAN	fSciFi;
	UINT8	ubDifficultyLevel;
	BOOLEAN	fTurnTimeLimit;
	UINT8	ubGameSaveMode;
};

using SoldierID = UINT8;

// AddCharacterToSquadRPC
struct RPC_DATA_ADD_TO_SQUAD
{
	SoldierID id;
	INT8 bSquadValue;
};

// AddHistoryToPlayersLogRPC
struct RPC_DATA_ADD_HISTORY
{
	UINT8 ubCode;
	UINT8 ubSecondCode;
	UINT32 uiDate;
};

// AddStrategicEventRPC
struct RPC_DATA_ADD_STRATEGIC_EVENT
{
	StrategicEventKind Kind;
	UINT32 uiMinStamp;
	UINT32 uiParam;
};

// AddTransactionToPlayersBookRPC
struct RPC_DATA_ADD_TRANSACTION
{
	UINT8 ubCode;
	UINT8 ubSecondCode;
	UINT32 uiDate;
	INT32 iAmount;
};

// BeginSoldierClimbDownRoofRPC
struct RPC_DATA_CLIMB_DOWN
{
	SoldierID id;
};

// BeginSoldierClimbFenceRPC
struct RPC_DATA_CLIMB_FENCE
{
	SoldierID id;
};

// BeginSoldierClimbUpRoofRPC
struct RPC_DATA_CLIMB_UP
{
	SoldierID id;
};

// BtnStealthModeCallbackRPC
struct RPC_DATA_STEALTH_MODE
{
	SoldierID id;
};

// ChangeWeaponModeRPC
struct RPC_DATA_CHANGE_WEAPON_MODE
{
	SoldierID id;
};

// HandleItemPointerClickRPC
struct RPC_DATA_ITEM_PTR_CLICK
{
	SoldierID id;
	UINT8 ubHandPos;
	SoldierID tgt_id;
	GridNo usMapPos;
	INT16 sCurrentActionPoints;
};

// HireMercRPC
struct RPC_DATA_HIRE_MERC
{
	MERC_HIRE_STRUCT h;
	INT8 contract_type;
	BOOLEAN fBuyEquipment;
	INT32 iContractAmount;
};

// SMInvClickCallbackPrimaryRPC
struct RPC_DATA_INV_CLICK
{
	SoldierID id;
	UINT8 ubHandPos;
	UINT8 ubCtrl;
	UINT8 ubShift;
};

// UIHandleSoldierStanceChangeRPC
struct RPC_DATA_STANCE_CHANGE
{
	SoldierID id;
	INT8 bNewStance;
};

/*
 * The following events are generated by the clients in HandleTacticalUI() and
 * processed by HandleEventRPC().
 */
struct RPC_DATA_EVENT
{
	UIEventKind puiNewEvent;
	SoldierID id;

	union
	{
		// C_MOVE_MERC
		struct
		{
			GridNo usMapPos;
			BOOLEAN fUIMovementFast;
		} c_move_merc;

		// CA_MERC_SHOOT
		struct
		{
			SoldierID tgt_id;
			GridNo usMapPos;
			INT8 bShownAimTime;
		} ca_merc_shoot;

		// LC_LOOK
		struct
		{
			GridNo usMapPos;
		} lc_look;
	};
};


// Networking

extern BOOLEAN gConnected;
extern NetworkIDManager gNetworkIdManager;
extern NETWORK_OPTIONS gNetworkOptions;
extern RakPeerInterface* gPeerInterface;

// Replication

extern struct PLAYER gPlayers[MAX_NUM_PLAYERS];
extern DataStructures::List<Replica3*> gReplicaList;
extern CoopReplicaManager gReplicaManager;

// RPCs

extern RPC4 gRPC;
extern BOOLEAN gRPC_Enable;
extern BOOLEAN gRPC_Squad;
extern std::list<RPC_DATA_EVENT> gRPC_Events;
// Item RPCs
extern INT8 gRPC_ClientIndex;
extern RPC_DATA_INV_CLICK* gRPC_InvClick;
extern RPC_DATA_ITEM_PTR_CLICK* gRPC_ItemPointerClick;
extern OBJECTTYPE* gpItemPointerRPC[MAX_NUM_PLAYERS];
extern SOLDIERTYPE* gpItemPointerSoldierRPC[MAX_NUM_PLAYERS];

// Etc.

extern BOOLEAN gEnableTimeCompression;
extern BOOLEAN gGameOptionsReceived;
extern BOOLEAN gStrategicReadyButtonValue;
extern HANDLE gMainThread;
extern HANDLE gPacketThread;


// Networking

unsigned char PacketId(Packet* p);
DWORD WINAPI ServerProcessesPacketsHere(LPVOID lpParam);
DWORD WINAPI ClientProcessesPacketsHere(LPVOID lpParam);

// RPCs

void AddCharacterToSquadRPC(BitStream* bitStream, Packet* packet);
void AddHistoryToPlayersLogRPC(BitStream* bitStream, Packet* packet);
void AddStrategicEventRPC(BitStream* bitStream, Packet* packet);
void AddTransactionToPlayersBookRPC(BitStream* bitStream, Packet* packet);
void BeginSoldierClimbDownRoofRPC(BitStream* bitStream, Packet* packet);
void BeginSoldierClimbFenceRPC(BitStream* bitStream, Packet* packet);
void BeginSoldierClimbUpRoofRPC(BitStream* bitStream, Packet* packet);
void BtnStealthModeCallbackRPC(BitStream* bitStream, Packet* packet);
void ChangeWeaponModeRPC(BitStream* bitStream, Packet* packet);
void HandleEventRPC(BitStream* bitStream, Packet* packet);
void HandleItemPointerClickRPC(BitStream* bitStream, Packet* packet);
void HireMercRPC(BitStream* bitStream, Packet* packet);
void SMInvClickCallbackPrimaryRPC(BitStream* bitStream, Packet* packet);
void UIHandleSoldierStanceChangeRPC(BitStream* bitStream, Packet* packet);

// Etc.

INT8 ClientIndex(RakNetGUID guid);
INT8 PlayerIndex(RakNetGUID guid);
void HireRandomMercs(unsigned int n);
UINT8 NumberOfPlayers();
void SendToChat(ST::string message, BOOLEAN broadcast);
void UpdateTeamPanel();

#endif // __COOP_H
