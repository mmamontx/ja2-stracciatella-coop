#ifndef __COOP_H
#define __COOP_H

#include "Game_Event_Hook.h"
#include "Handle_UI.h"
#define GROUP GROUP_JA2
#include "NetworkIDManager.h"
#include "RakNet/RPC4Plugin.h"
#include "RakPeerInterface.h"
#include "ReplicaManager3.h"
#undef GROUP
#include "Merc_Hiring.h"
#include "MessageIdentifiers.h"
#include "Soldier_Control.h"
#include "Soldier_Profile_Type.h"


#define COOP_DEBUG // Comment out for releases
#define ENEMY_ENABLED

#define MAX_NUM_PLAYERS  4
#define TEST_NUM_PLAYERS 2

#define KEY_RETURN 13

#define IS_SERVER       (gGameOptions.fNetwork == 0)
#define IS_CLIENT       (gGameOptions.fNetwork != 0)
#define IS_VALID_CLIENT (IS_CLIENT && connected && \
	(gReplicaList.Size() != 0))

#define CONNECT_TIMEOUT_MS 1000

#define RPC_READY (gRPC_Enable && !(gRPC_Events.empty()))

#define FOR_EACH_PLAYER(i) for (int i = 0; i < MAX_NUM_PLAYERS; i++)
#define FOR_EACH_CLIENT(i) for (int i = 1; i < MAX_NUM_PLAYERS; i++)

#define REPLICA_PROFILE_INDEX TOTAL_SOLDIERS
#define REPLICA_PLAYER_INDEX  (TOTAL_SOLDIERS + NUM_PROFILES)

#define EXECUTE_WHILE_MAIN_SUSPENDED(func) { do { \
		SuspendThread(gMainThread); \
		func; \
		ResumeThread(gMainThread); \
	} while (0); }

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
    ((PLAYER*)(gReplicaList[REPLICA_PLAYER_INDEX + i]))->name.C_String() : \
    gPlayers[i].name.C_String())
#define PLAYER_GUID(i) (IS_VALID_CLIENT ? \
    ((PLAYER*)(gReplicaList[REPLICA_PLAYER_INDEX + i]))->guid : \
    gPlayers[i].guid)

// Packet structures

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

// Classes

struct PLAYER : public Replica3
{
	RakNetGUID guid;
	RakString name;
	BOOLEAN ready; // Ready button (on the strategic map in the very beginning)
	BOOLEAN endturn;

	bool operator == (const PLAYER& s) const { return guid == s.guid; }

	virtual RakString GetName(void) const
	{
		return RakString("PLAYER");
	}

	virtual void WriteAllocationID(Connection_RM3* destinationConnection,
		BitStream* allocationIdBitstream) const
	{
		allocationIdBitstream->Write(GetName());
	}

	void PrintStringInBitstream(BitStream* bs)
	{
		if (bs->GetNumberOfBitsUsed() == 0) return;
		RakString rakString;
		bs->Read(rakString);
		//SLOGI("Receive: {}", rakString.C_String());
	}

	virtual void SerializeConstruction(BitStream* constructionBitstream,
		Connection_RM3* destinationConnection)
	{
		constructionBitstream->Write(GetName() +
			RakString(" SerializeConstruction"));
	}

	virtual bool DeserializeConstruction(BitStream* constructionBitstream,
		Connection_RM3* sourceConnection)
	{
		PrintStringInBitstream(constructionBitstream);
		return true;
	}

	virtual void SerializeDestruction(BitStream* destructionBitstream,
		Connection_RM3* destinationConnection)
	{
		destructionBitstream->Write(GetName() +
			RakString(" SerializeDestruction"));
	}

	virtual bool DeserializeDestruction(BitStream* destructionBitstream,
		Connection_RM3* sourceConnection)
	{
		PrintStringInBitstream(destructionBitstream);
		return true;
	}

	virtual void DeallocReplica(Connection_RM3* sourceConnection)
	{
		delete this;
	}

	virtual void OnUserReplicaPreSerializeTick(void)
	{
	}

	virtual RM3SerializationResult
	Serialize(SerializeParameters* serializeParameters)
	{
		serializeParameters->outputBitstream[0].Write(guid);
		serializeParameters->outputBitstream[0].Write(name);
		serializeParameters->outputBitstream[0].Write(ready);
		serializeParameters->outputBitstream[0].Write(endturn);

		/*
		 * For some reason, sometimes objects of this class aren't serialized,
		 * which causes a failed connection attempt because guid is not
		 * replicated. So here the serialization is always forced.
		 */
		//return RM3SR_BROADCAST_IDENTICALLY;
		return RM3SR_BROADCAST_IDENTICALLY_FORCE_SERIALIZATION;
	}

	virtual void Deserialize(DeserializeParameters* deserializeParameters)
	{
		deserializeParameters->serializationBitstream[0].Read(guid);
		deserializeParameters->serializationBitstream[0].Read(name);
		deserializeParameters->serializationBitstream[0].Read(ready);
		deserializeParameters->serializationBitstream[0].Read(endturn);
	}

	virtual void
	SerializeConstructionRequestAccepted(BitStream* serializationBitstream,
		Connection_RM3* requestingConnection)
	{
		serializationBitstream->Write(GetName() +
			RakString(" SerializeConstructionRequestAccepted"));
	}

	virtual void
	DeserializeConstructionRequestAccepted(BitStream* serializationBitstream,
		Connection_RM3* acceptingConnection)
	{
		PrintStringInBitstream(serializationBitstream);
	}

	virtual void
	SerializeConstructionRequestRejected(BitStream* serializationBitstream,
		Connection_RM3* requestingConnection)
	{
		serializationBitstream->Write(GetName() +
			RakString(" SerializeConstructionRequestRejected"));
	}

	virtual void
	DeserializeConstructionRequestRejected(BitStream* serializationBitstream,
		Connection_RM3* rejectingConnection)
	{
		PrintStringInBitstream(serializationBitstream);
	}

	virtual void OnPoppedConnection(Connection_RM3* droppedConnection)
	{
	}

	void NotifyReplicaOfMessageDeliveryStatus(RakNetGUID guid,
		uint32_t receiptId, bool messageArrived)
	{
	}

	virtual RM3ConstructionState
	QueryConstruction(Connection_RM3* destinationConnection,
		ReplicaManager3* replicaManager3)
	{
		return QueryConstruction_ServerConstruction(destinationConnection,
			IS_SERVER);
	}

	virtual bool QueryRemoteConstruction(Connection_RM3* sourceConnection)
	{
		return QueryRemoteConstruction_ServerConstruction(sourceConnection,
			IS_SERVER);
	}

	virtual RM3QuerySerializationResult
	QuerySerialization(Connection_RM3* destinationConnection)
	{
		return QuerySerialization_ServerSerializable(destinationConnection,
			IS_SERVER);
	}

	virtual RM3ActionOnPopConnection
	QueryActionOnPopConnection(Connection_RM3* droppedConnection) const
	{
		return QueryActionOnPopConnection_Server(droppedConnection);
	}
};

class CoopConnection : public Connection_RM3
{
public:

	CoopConnection(const SystemAddress& _systemAddress, RakNetGUID _guid) :
		Connection_RM3(_systemAddress, _guid) {}

	virtual ~CoopConnection() {}

	bool QueryGroupDownloadMessages(void) const { return true; }

	virtual Replica3*
	AllocReplica(BitStream* allocationId, ReplicaManager3* replicaManager3)
	{
		RakString typeName;
		allocationId->Read(typeName);
		if (typeName == "SOLDIERTYPE") return new SOLDIERTYPE;
		if (typeName == "MERCPROFILESTRUCT") return new MERCPROFILESTRUCT;
		if (typeName == "PLAYER") return new PLAYER;
		return 0;
	}

protected:
};

class CoopReplicaManager : public ReplicaManager3
{
	virtual Connection_RM3* AllocConnection(const SystemAddress& systemAddress,
		RakNetGUID rakNetGUID) const
	{
		return new CoopConnection(systemAddress, rakNetGUID);
	}

	virtual void DeallocConnection(Connection_RM3* connection) const
	{
		delete connection;
	}
};

// RPC structures

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


// Externs

// Networking
extern NETWORK_OPTIONS gNetworkOptions;
extern NetworkIDManager gNetworkIdManager;
extern RakPeerInterface* peer;
extern BOOLEAN connected;

// Replication
extern DataStructures::List<Replica3*> gReplicaList;
extern CoopReplicaManager gReplicaManager;
extern struct PLAYER gPlayers[MAX_NUM_PLAYERS];

// RPCs
extern RPC4 gRPC;

extern BOOLEAN gRPC_Enable;
extern BOOLEAN gRPC_Squad;

// Item RPCs
extern RPC_DATA_INV_CLICK* gRPC_InvClick;
extern OBJECTTYPE* gpItemPointerRPC[MAX_NUM_PLAYERS];
extern SOLDIERTYPE* gpItemPointerSoldierRPC[MAX_NUM_PLAYERS];
extern INT8 gRPC_ClientIndex;

extern RPC_DATA_ITEM_PTR_CLICK* gRPC_ItemPointerClick;

extern std::list<RPC_DATA_EVENT> gRPC_Events;

// Etc.
extern HANDLE gMainThread;
extern HANDLE gPacketThread;

extern BOOLEAN gEnableTimeCompression;
extern BOOLEAN MPReadyButtonValue;
extern BOOLEAN gGameOptionsReceived;


// Prototypes

// Networking
DWORD WINAPI ClientProcessesPacketsHere(LPVOID lpParam);
unsigned char PacketId(Packet* p);
DWORD WINAPI ServerProcessesPacketsHere(LPVOID lpParam);

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
void HireRandomMercs(unsigned int n);
UINT8 NumberOfPlayers();
INT8 PlayerIndex(RakNetGUID guid);
void SendToChat(ST::string message, BOOLEAN broadcast);
void UpdateTeamPanel();

#endif // __COOP_H
