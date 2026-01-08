#include "Animation_Control.h"
#include "CoopReplicaManager.h"
#include "Finances.h"
#include "Game_Clock.h"
#include "Game_Event_Hook.h"
#include "GameLoop.h"
#include "GameScreen.h"
#include "History.h"
#include "Interface_Items.h"
#include "Interface_Panels.h"
#include "MapScreen.h"
#include "Merc_Hiring.h"
#include "Message.h"
#include "Overhead.h"
#include "RakNet/MessageIdentifiers.h"
#include "RakNet/RPC4Plugin.h"
#include "Soldier_Profile.h"
#include "Squads.h"
#include "Strategic.h"
#include "Weapons.h"


// Networking

BOOLEAN gConnected;
NetworkIDManager gNetworkIdManager;
NETWORK_OPTIONS gNetworkOptions;
RakPeerInterface* gPeerInterface;

// Replication

/*
 * The following array is maintained by the server only. The clients use its
 * replicated copy inside gReplicaList.
 */
struct PLAYER gPlayers[MAX_NUM_PLAYERS];
// The following list holds all the objects shared by the server
DataStructures::List<Replica3*> gReplicaList;
CoopReplicaManager gReplicaManager;

// RPCs

RPC4 gRPC; // Single RPC plugin handle
/*
 * In HandleTacticalUI() the server uses this variable to turn on remote events
 * when there is no local events for processing.
 */
BOOLEAN gRPC_Enable = TRUE;
/*
 * In AddCharacterToSquad() the server uses this variable to check if it is
 * being executed as an RPC.
 */
BOOLEAN gRPC_Squad = FALSE;
// Events processed by the server in HandleTacticalUI()
std::list<RPC_DATA_EVENT> gRPC_Events;
/*
 * Item RPCs:
 *
 * 1. gRPC_InvClick is used when clients click on an item in the inventory.
 *
 * 2. gRPC_ItemPointerClick is used when clients click somewhere with an item
 *    held in their cursor.
 *
 * gpItemPointerRPC and gpItemPointerSoldierRPC keep pointers to the picked
 * item and the merc, who picks it (in whose inventory it has been located).
 *
 * gRPC_ClientIndex indicates the client, who is executing an RPC.
 */
INT8 gRPC_ClientIndex = -1;
RPC_DATA_INV_CLICK* gRPC_InvClick = NULL;
RPC_DATA_ITEM_PTR_CLICK* gRPC_ItemPointerClick = NULL;
OBJECTTYPE* gpItemPointerRPC[MAX_NUM_PLAYERS] = { NULL };
SOLDIERTYPE* gpItemPointerSoldierRPC[MAX_NUM_PLAYERS] = { NULL };

// Etc.

/*
 * The following variable is used when the host lets the clients enter the
 * tactical map.
 */
BOOLEAN gEnableTimeCompression = FALSE;
BOOLEAN gGameOptionsReceived = FALSE;
BOOLEAN gStrategicReadyButtonValue = FALSE;
// Handles for suspending and terminating the threads
HANDLE gMainThread;
HANDLE gPacketThread;


unsigned char PacketId(Packet* p)
{
	if (p == 0) return 255; // -1

	if (p->data[0] == ID_TIMESTAMP)	return p->data[sizeof(unsigned long) + 1];

	return p->data[0];
}

DWORD WINAPI ServerProcessesPacketsHere(LPVOID lpParam)
{
	Packet* p;
	unsigned char id;

	while (TRUE)
	{
		while (TRUE)
		{
			p = gPeerInterface->Receive();
			if (p == NULL) break;

			id = PacketId(p);

			switch (id)
			{
			case ID_DISCONNECTION_NOTIFICATION:
			case ID_CONNECTION_LOST:
				FOR_EACH_CLIENT(i)
				{
					if (gPlayers[i].guid == p->guid)
					{
						gPlayers[i].guid = UNASSIGNED_RAKNET_GUID;

						SendToChat(gPlayers[i].name + " has disconnected.",
							TRUE);

						break;
					}
				}

				UpdateTeamPanel(); // Without the disconnected player

				break;
			case ID_USER_PACKET_CONNECT:
			{
				struct USER_PACKET_CONNECT* up;
				up = (struct USER_PACKET_CONNECT*)p->data;

				// Registering the new player in the global struct
				bool present = false;
				INT8 first_free = -1;
				FOR_EACH_CLIENT(i)
				{
					if (gPlayers[i].guid == p->guid)
					{
						present = true;
						break;
					}
					else
					{
						if ((gPlayers[i].guid == UNASSIGNED_RAKNET_GUID) &&
							(first_free == -1))
						{
							first_free = i;
						}
					}
				}

				// Not connected already and there is space available
				if ((!present) && (first_free != -1))
				{
					gPlayers[first_free].guid = p->guid;
					gPlayers[first_free].name = up->name;
					gPlayers[first_free].ready = up->ready;

					SendToChat((ST::string)(up->name) + " has connected.",
						TRUE);

					UpdateTeamPanel(); // With the connected player

					// Share the game options with the connected player
					struct USER_PACKET_GAME_OPTIONS up_gops;
					up_gops.id = ID_USER_PACKET_GAME_OPTIONS;

					up_gops.fGunNut = gGameOptions.fGunNut;
					up_gops.fSciFi = gGameOptions.fSciFi;
					up_gops.ubDifficultyLevel = gGameOptions.ubDifficultyLevel;
					up_gops.fTurnTimeLimit = gGameOptions.fTurnTimeLimit;
					up_gops.ubGameSaveMode = gGameOptions.ubGameSaveMode;

					gPeerInterface->Send((char*)&up_gops,
						sizeof(up_gops), MEDIUM_PRIORITY, RELIABLE, 0, p->guid,
						false);
				} // Otherwise - ignore

				break;
			}
			case ID_USER_PACKET_MESSAGE:
			{
				struct USER_PACKET_MESSAGE* up;
				up = (struct USER_PACKET_MESSAGE*)p->data;

				FOR_EACH_CLIENT(i)
				{
					if (gPlayers[i].guid == p->guid)
					{
						SendToChat(gPlayers[i].name + "> " +
							(ST::string)up->message, TRUE);

						break;
					}
				}

				break;
			}
			case ID_USER_PACKET_READY:
			{
				struct USER_PACKET_READY* up;
				struct USER_PACKET_MESSAGE up_broadcast;
				char str[256];

				up = (struct USER_PACKET_READY*)p->data;

				FOR_EACH_PLAYER(i)
				{
					if (gPlayers[i].guid == p->guid)
					{
						gPlayers[i].ready = up->ready;

						SendToChat(gPlayers[i].name +
							(gPlayers[i].ready ? " is ready." :
							" is not ready anymore."), TRUE);

						break;
					}
				}

				break;
			}
			case ID_USER_PACKET_END_TURN:
			{
				struct USER_PACKET_END_TURN* up;
				struct USER_PACKET_MESSAGE up_broadcast;
				char str[256];

				up = (struct USER_PACKET_END_TURN*)p->data;

				UINT8 finished = 0;
				FOR_EACH_PLAYER(i)
				{
					if (gPlayers[i].endturn) finished++;
				}

				UINT8 n = NumberOfPlayers();

				FOR_EACH_PLAYER(i)
				{
					if (gPlayers[i].guid == p->guid)
					{
						if (!(gPlayers[i].endturn))
						{
							gPlayers[i].endturn = true;
							finished++;

							SendToChat(gPlayers[i].name +
								" has finished his/her turn. " +
								std::to_string(finished) + "/" +
								std::to_string(n) + " total.", TRUE);
						}

						break;
					}
				}

				if (finished == n) gfBeginEndTurn = TRUE;

				break;
			}
			case ID_NEW_INCOMING_CONNECTION:
			case ID_REPLICA_MANAGER_SCOPE_CHANGE:
			case ID_RPC_PLUGIN:
				break;
			default:
				SLOGW("Received unknown packet id = {}", id);
				break;
			}

			gPeerInterface->DeallocatePacket(p);
		}

		Sleep(1); // 1 ms
	}

	return 0;
}

DWORD WINAPI ClientProcessesPacketsHere(LPVOID lpParam)
{
	Packet* p;
	unsigned char id;

	while (TRUE)
	{
		while (TRUE)
		{
			p = gPeerInterface->Receive();
			if (p == NULL) break;

			id = PacketId(p);

			switch (id)
			{
			case ID_CONNECTION_REQUEST_ACCEPTED:
				gConnected = TRUE;

				SendToChat((ST::string)"Connected", FALSE);

				break;
			case ID_DISCONNECTION_NOTIFICATION:
			case ID_CONNECTION_LOST:
				guiPendingScreen = MAINMENU_SCREEN;
				return 0; // Stop processing the packets and exit the thread
			case ID_REPLICA_MANAGER_DOWNLOAD_COMPLETE:
				gReplicaManager.GetReferencedReplicaList(gReplicaList);

				// With the replicated mercs
				EXECUTE_WHILE_MAIN_SUSPENDED(UpdateTeamPanel());

				break;
			case ID_USER_PACKET_MESSAGE:
			{
				struct USER_PACKET_MESSAGE* up;
				up = (struct USER_PACKET_MESSAGE*)p->data;

				SendToChat((ST::string)(up->message), FALSE);

				break;
			}
			case ID_USER_PACKET_START:
				gEnableTimeCompression = TRUE;
				break;
			case ID_USER_PACKET_TEAM_PANEL_DIRTY:
				EXECUTE_WHILE_MAIN_SUSPENDED(UpdateTeamPanel());
				break;
			case ID_USER_PACKET_TOP_MESSAGE:
			{
				struct USER_PACKET_TOP_MESSAGE* up;
				up = (struct USER_PACKET_TOP_MESSAGE*)p->data;

				gTacticalStatus.ubCurrentTeam = up->ubCurrentTeam;
				gTacticalStatus.ubTopMessageType =
					(MESSAGE_TYPES)(up->ubTopMessageType);
				gTacticalStatus.usTactialTurnLimitCounter =
					up->usTactialTurnLimitCounter;
				gTacticalStatus.usTactialTurnLimitMax =
					up->usTactialTurnLimitMax;

				EXECUTE_WHILE_MAIN_SUSPENDED(
					AddTopMessage((MESSAGE_TYPES)(up->ubTopMessageType)));

				break;
			}
			case ID_USER_PACKET_END_COMBAT:
				EXECUTE_WHILE_MAIN_SUSPENDED(ExitCombatMode());
				break;
			case ID_USER_PACKET_GAME_OPTIONS:
			{
				struct USER_PACKET_GAME_OPTIONS* up;
				up = (struct USER_PACKET_GAME_OPTIONS*)p->data;

				gGameOptions.fGunNut = up->fGunNut;
				gGameOptions.fSciFi = up->fSciFi;
				gGameOptions.ubDifficultyLevel = up->ubDifficultyLevel;
				gGameOptions.fTurnTimeLimit = up->fTurnTimeLimit;
				gGameOptions.ubGameSaveMode = up->ubGameSaveMode;

				gGameOptionsReceived = TRUE;

				break;
			}
			case ID_ALREADY_CONNECTED:
			case ID_CONNECTION_ATTEMPT_FAILED:
			case ID_NO_FREE_INCOMING_CONNECTIONS:
			case ID_REMOTE_CONNECTION_LOST:
			case ID_REMOTE_DISCONNECTION_NOTIFICATION:
			case ID_REMOTE_NEW_INCOMING_CONNECTION:
			case ID_REPLICA_MANAGER_CONSTRUCTION:
			case ID_REPLICA_MANAGER_DOWNLOAD_STARTED:
			case ID_REPLICA_MANAGER_SCOPE_CHANGE:
			case ID_REPLICA_MANAGER_SERIALIZE:
				break;
			default:
				SLOGW("Received unknown packet id = {}", id);
				break;
			}

			gPeerInterface->DeallocatePacket(p);
		};

		Sleep(1); // 1 ms
	}

	return 0;
}

void AddCharacterToSquadRPC(BitStream* bitStream, Packet* packet)
{
	RPC_DATA_ADD_TO_SQUAD data;
	int offset = bitStream->GetReadOffset();
	bool read = bitStream->ReadCompressed(data);
	RakAssert(read);

	gRPC_Squad = TRUE;

	EXECUTE_WHILE_MAIN_SUSPENDED(AddCharacterToSquad(ID2Soldier(data.id),
		data.bSquadValue));

	gRPC_Squad = FALSE;
}

void AddHistoryToPlayersLogRPC(BitStream* bitStream, Packet* packet)
{
	RPC_DATA_ADD_HISTORY data;
	int offset = bitStream->GetReadOffset();
	bool read = bitStream->ReadCompressed(data);
	RakAssert(read);

	EXECUTE_WHILE_MAIN_SUSPENDED(
		AddHistoryToPlayersLog(data.ubCode, data.ubSecondCode, data.uiDate,
			SGPSector(-1, -1)));
}

void AddStrategicEventRPC(BitStream* bitStream, Packet* packet)
{
	RPC_DATA_ADD_STRATEGIC_EVENT data;
	int offset = bitStream->GetReadOffset();
	bool read = bitStream->ReadCompressed(data);
	RakAssert(read);

	EXECUTE_WHILE_MAIN_SUSPENDED(
		AddStrategicEvent(data.Kind, data.uiMinStamp, data.uiParam));
}

void AddTransactionToPlayersBookRPC(BitStream* bitStream, Packet* packet)
{
	RPC_DATA_ADD_TRANSACTION data;
	int offset = bitStream->GetReadOffset();
	bool read = bitStream->ReadCompressed(data);
	RakAssert(read);

	EXECUTE_WHILE_MAIN_SUSPENDED(
		AddTransactionToPlayersBook(data.ubCode, data.ubSecondCode,
			data.uiDate, data.iAmount));
}

void BeginSoldierClimbDownRoofRPC(BitStream* bitStream, Packet* packet)
{
	RPC_DATA_CLIMB_DOWN data;
	int offset = bitStream->GetReadOffset();
	bool read = bitStream->ReadCompressed(data);
	RakAssert(read);

	EXECUTE_WHILE_MAIN_SUSPENDED(
		BeginSoldierClimbDownRoof(ID2Soldier(data.id)));
}

void BeginSoldierClimbFenceRPC(BitStream* bitStream, Packet* packet)
{
	RPC_DATA_CLIMB_FENCE data;
	int offset = bitStream->GetReadOffset();
	bool read = bitStream->ReadCompressed(data);
	RakAssert(read);

	EXECUTE_WHILE_MAIN_SUSPENDED(BeginSoldierClimbFence(ID2Soldier(data.id)));
}

void BeginSoldierClimbUpRoofRPC(BitStream* bitStream, Packet* packet)
{
	RPC_DATA_CLIMB_UP data;
	int offset = bitStream->GetReadOffset();
	bool read = bitStream->ReadCompressed(data);
	RakAssert(read);

	EXECUTE_WHILE_MAIN_SUSPENDED(BeginSoldierClimbUpRoof(ID2Soldier(data.id)));
}

void BtnStealthModeCallbackRPC(BitStream* bitStream, Packet* packet)
{
	RPC_DATA_STEALTH_MODE data;
	int offset = bitStream->GetReadOffset();
	bool read = bitStream->ReadCompressed(data);
	RakAssert(read);

	SOLDIERTYPE* const s = ID2Soldier(data.id);

	gpSMCurrentMerc->bStealthMode = !(s->bStealthMode);
	gfUIStanceDifferent = TRUE;
	gfPlotNewMovement = TRUE;
	fInterfacePanelDirty = DIRTYLEVEL2;
}

void ChangeWeaponModeRPC(BitStream* bitStream, Packet* packet)
{
	RPC_DATA_CHANGE_WEAPON_MODE data;
	int offset = bitStream->GetReadOffset();
	bool read = bitStream->ReadCompressed(data);
	RakAssert(read);

	EXECUTE_WHILE_MAIN_SUSPENDED(ChangeWeaponMode(ID2Soldier(data.id)));
}

/*
 * The following call processes the events that are generated by the clients in
 * HandleTacticalUI(). At the same location the host executes these events on
 * behalf of the clients.
 */
void HandleEventRPC(BitStream* bitStream, Packet* packet)
{
	RPC_DATA_EVENT data;
	int offset = bitStream->GetReadOffset();
	bool read = bitStream->ReadCompressed(data);
	RakAssert(read);

	gRPC_Events.push_back(data);
}

void HandleItemPointerClickRPC(BitStream* bitStream, Packet* packet)
{
	RPC_DATA_ITEM_PTR_CLICK data;
	int offset = bitStream->GetReadOffset();
	bool read = bitStream->ReadCompressed(data);
	RakAssert(read);

	gRPC_ClientIndex = ClientIndex(packet->guid);

	gRPC_ItemPointerClick = &data;

	EXECUTE_WHILE_MAIN_SUSPENDED(HandleItemPointerClick(data.usMapPos));

	gRPC_ItemPointerClick = NULL;

	gRPC_ClientIndex = -1;
}

void HireMercRPC(BitStream* bitStream, Packet* packet)
{
	RPC_DATA_HIRE_MERC data;
	int offset = bitStream->GetReadOffset();
	bool read = bitStream->ReadCompressed(data);
	RakAssert(read);

	SuspendThread(gMainThread);

	INT8 const ret = HireMerc(data.h);
	if (ret == MERC_HIRE_OK)
	{
		SOLDIERTYPE* const s =
			FindSoldierByProfileIDOnPlayerTeam(data.h.ubProfileID);
		if (s == NULL)
		{
			ResumeThread(gMainThread);
			return;
		}
		s->bTypeOfLastContract = data.contract_type;

		MERCPROFILESTRUCT& p = GetProfile(data.h.ubProfileID);
		if (data.fBuyEquipment)
		{
			p.ubMiscFlags |= PROFILE_MISC_FLAG_ALREADY_USED_ITEMS;
		}

		AddTransactionToPlayersBook(HIRED_MERC, data.h.ubProfileID,
			GetWorldTotalMin(), -(data.iContractAmount) +
			p.sMedicalDepositAmount);

		if (p.bMedicalDeposit)
		{
			AddTransactionToPlayersBook(MEDICAL_DEPOSIT, data.h.ubProfileID,
				GetWorldTotalMin(), -p.sMedicalDepositAmount);
		}

		AddHistoryToPlayersLog(HISTORY_HIRED_MERC_FROM_AIM, data.h.ubProfileID,
			GetWorldTotalMin(), SGPSector(-1, -1));

		INT8 i = ClientIndex(packet->guid);
		s->ubPlayer = i;

		SendToChat(gPlayers[i].name + " has hired " + s->name + ".", TRUE);
	}

	UpdateTeamPanel(); // With the new merc

	ResumeThread(gMainThread);
}

void SMInvClickCallbackPrimaryRPC(BitStream* bitStream, Packet* packet)
{
	RPC_DATA_INV_CLICK data;
	int offset = bitStream->GetReadOffset();
	bool read = bitStream->ReadCompressed(data);
	RakAssert(read);

	gRPC_ClientIndex = ClientIndex(packet->guid);

	gRPC_InvClick = &data;

	EXECUTE_WHILE_MAIN_SUSPENDED(SMInvClickCallbackPrimary(NULL, 0));

	gRPC_InvClick = NULL;

	gRPC_ClientIndex = -1;
}

void UIHandleSoldierStanceChangeRPC(BitStream* bitStream, Packet* packet)
{
	RPC_DATA_STANCE_CHANGE data;
	int offset = bitStream->GetReadOffset();
	bool read = bitStream->ReadCompressed(data);
	RakAssert(read);

	EXECUTE_WHILE_MAIN_SUSPENDED(
		UIHandleSoldierStanceChange(ID2Soldier(data.id), data.bNewStance));
}

INT8 ClientIndex(RakNetGUID guid)
{
	FOR_EACH_CLIENT(i)
	{
		if (PLAYER_GUID(i) == guid) return i;
	}

	return -1;
}

INT8 PlayerIndex(RakNetGUID guid)
{
	FOR_EACH_PLAYER(i)
	{
		if (PLAYER_GUID(i) == guid) return i;
	}

	return -1;
}

// For debugging purposes (so we don't have to hire mercs manually everytime)
void HireRandomMercs(unsigned int n)
{
	struct MERC_HIRE_STRUCT h;
	SOLDIERTYPE* s;
	int id_random;

	srand((unsigned)time(NULL));

	for (int i = 0; i < n; i++)
	{
		do
		{
			id_random = rand() % 40; // 40 AIM mercs
		}
		while (gMercProfiles[id_random].bMercStatus < 0); // Prevent repeats

		h = MERC_HIRE_STRUCT{};

		h.ubProfileID = id_random;
		h.sSector = g_merc_arrive_sector;
		h.fUseLandingZoneForArrival = TRUE;
		h.ubInsertionCode = INSERTION_CODE_ARRIVING_GAME;
		h.iTotalContractLength = 1;
		h.fCopyProfileItemsOver = TRUE;
		h.uiTimeTillMercArrives = GetMercArrivalTimeOfDay();
		h.bWhatKindOfMerc = MERC_TYPE__AIM_MERC;

		gMercProfiles[id_random].ubMiscFlags
			|= PROFILE_MISC_FLAG_ALREADY_USED_ITEMS;

		HireMerc(h);

		s = FindSoldierByProfileID(id_random);
		s->ubPlayer = (i / COOP_DEBUG_NUM_MERCS_PER_PLAYER);
	}

	UpdateTeamPanel(); // With the new merc(s)
}

UINT8 NumberOfPlayers()
{
	UINT8 n = 1;

	FOR_EACH_CLIENT(i)
	{
		if (PLAYER_GUID(i) != UNASSIGNED_RAKNET_GUID) n++;
	}

	return n;
}

void SendToChat(ST::string message, BOOLEAN broadcast)
{
	struct USER_PACKET_MESSAGE up_broadcast;

	ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, message);

	if (broadcast && IS_SERVER && (NumberOfPlayers() > 1))
	{
		// Broadcasting the message to the clients
		up_broadcast.id = ID_USER_PACKET_MESSAGE;
		strcpy(up_broadcast.message, message.c_str());
		gPeerInterface->Send((char*)&up_broadcast, sizeof(up_broadcast),
			MEDIUM_PRIORITY, RELIABLE, 0, UNASSIGNED_RAKNET_GUID, true);
	}
}

void UpdateTeamPanel()
{
	fDrawCharacterList = TRUE;
	fTeamPanelDirty = TRUE;
	ReBuildCharactersList();

	if (IS_SERVER && (NumberOfPlayers() > 1)) // Tell others
	{
		struct USER_PACKET_MESSAGE up_broadcast;
		up_broadcast.id = ID_USER_PACKET_TEAM_PANEL_DIRTY;
		gPeerInterface->Send((char*)&up_broadcast, sizeof(up_broadcast),
			MEDIUM_PRIORITY, RELIABLE, 0, UNASSIGNED_RAKNET_GUID, true);
	}
}
