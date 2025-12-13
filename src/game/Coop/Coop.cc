#include "Animation_Control.h"
#include "Assignments.h"
#include "Faces.h"
#include "Finances.h"
#include "Game_Clock.h"
#include "GameScreen.h"
#include "History.h"
#include "Interface_Items.h"
#include "Interface_Panels.h"
#include "Items.h"
#include "Map_Screen_Interface.h"
#include "MapScreen.h"
#include "Merc_Hiring.h"
#include "Message.h"
#include "Overhead.h"
#include "RakNet/MessageIdentifiers.h"
#define GROUP GROUP_JA2
#include "RakNet/RPC4Plugin.h"
#undef GROUP
#include "Soldier_Profile.h"
#include "Squads.h"
#include "Strategic.h"
#include "Weapons.h"

#include <Game_Event_Hook.h>


// Networking
NETWORK_OPTIONS gNetworkOptions;
NetworkIDManager gNetworkIdManager;

// Replication
DataStructures::List<Replica3*> gReplicaList;
ReplicaManager3Sample gReplicaManager;
// gPlayers is located in gReplicaList starting from REPLICA_PLAYER_INDEX
struct PLAYER gPlayers[MAX_NUM_PLAYERS];

// RPCs
// FIXME: Use a common entity for all the sorts of RPCs below
BOOLEAN gRPC_Enable = TRUE; // Enables remote events so that they don't overlap with local events
BOOLEAN gRPC_Squad = FALSE;
OBJECTTYPE* gpItemPointerRPC = NULL; // If RPCs can be executed in parallel (to be checked) a single shared variable would cause conflicts
RPC_DATA* gRPC_Inv = NULL; // Currently executed inventory RPC
RPC4 gRPC;
SOLDIERTYPE* gpItemPointerSoldierRPC = NULL;
std::list<RPC_DATA> gRPC_Events;

// Etc.
HANDLE gMainThread;
// For clients: whether the server hit the time compression button
BOOLEAN gStarted = FALSE;
BOOLEAN MPReadyButtonValue = FALSE;
BOOLEAN gEnemyEnabled;
BOOLEAN gGameOptionsReceived = FALSE;


DWORD WINAPI replicamgr(LPVOID lpParam)
{
	while (1)
		Sleep(33); // NOTE: ~30 FPS, can be improved if needed
	return 0;
}

INT8 PlayerIndex(RakNetGUID guid)
{
	printf("IS_VALID_CLIENT = %s\n", IS_VALID_CLIENT ? "TRUE" : "FALSE");

	FOR_EACH_PLAYER(i) {
		SLOGI("PLAYER_GUID({}) = {}", i, (PLAYER_GUID(i)).ToUint32(PLAYER_GUID(i)));
		if (PLAYER_GUID(i) == guid) {
			return i;
		}
	}

	return -1;
}

UINT8 NumberOfPlayers()
{
	UINT8 n = 0;

	FOR_EACH_PLAYER(i) {
		if (PLAYER_GUID(i) != UNASSIGNED_RAKNET_GUID) {
			n++;
		}
	}

	return n;
}

void UpdateTeamPanel()
{
	fDrawCharacterList = true;
	fTeamPanelDirty = true;
	ReBuildCharactersList();
}

// For debugging purposes (so we don't have to hire mercs manually everytime)
void HireRandomMercs(unsigned int n)
{
	struct MERC_HIRE_STRUCT h;
	SOLDIERTYPE* s;
	int id_random;
	srand((unsigned)time(NULL));

	for (int i = 0; i < n; i++) {
		// Prevent repetitive mercs
		do {
			id_random = rand() % 40; // There should be at least 40 mercs (considering AIM only)
		} while (gMercProfiles[id_random].bMercStatus < 0);

		h = MERC_HIRE_STRUCT{};

		h.ubProfileID = id_random;
		h.sSector = g_merc_arrive_sector;
		h.fUseLandingZoneForArrival = TRUE;
		h.ubInsertionCode = INSERTION_CODE_ARRIVING_GAME;
		h.iTotalContractLength = 1;
		h.fCopyProfileItemsOver = true;
		h.uiTimeTillMercArrives = GetMercArrivalTimeOfDay();
		h.bWhatKindOfMerc = MERC_TYPE__AIM_MERC;

		gMercProfiles[id_random].ubMiscFlags |= PROFILE_MISC_FLAG_ALREADY_USED_ITEMS;

		HireMerc(h);

		s = FindSoldierByProfileID(id_random);
		s->ubPlayer = i % 2; // NB: Considering that 2 players would be used for debugging
	}

	fDrawCharacterList = true;
	fTeamPanelDirty = true;
	ReBuildCharactersList();
}

unsigned char SGetPacketIdentifier(Packet* p)
{
	if (p == 0)
		return 255;

	if ((unsigned char)p->data[0] == ID_TIMESTAMP) {
		assert(p->length > sizeof(unsigned char) + sizeof(unsigned long));
		return (unsigned char)p->data[sizeof(unsigned char) + sizeof(unsigned long)];
	} else {
		return (unsigned char)p->data[0];
	}
}

// We are the server and process packets from clients here
DWORD WINAPI server_packet(LPVOID lpParam)
{
	RakNet::Packet* p;
	unsigned char SpacketIdentifier;

	while (TRUE) {
		p = gNetworkOptions.peer->Receive();

		while (p) {
			// We got a packet, get the identifier with our handy function
			SpacketIdentifier = SGetPacketIdentifier(p);

			// Check if this is a network message packet
			switch (SpacketIdentifier) {
			case ID_NEW_INCOMING_CONNECTION:
				// This tells the server that a client has connected
				// NB: The player is not yet registered in gPlayers structure at this point
				//ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"ID_NEW_INCOMING_CONNECTION");
				break;
			case ID_DISCONNECTION_NOTIFICATION:
			case ID_CONNECTION_LOST:
				// Couldn't deliver a reliable packet - i.e. the other system was abnormally
				// terminated
				ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"ID_DISCONNECTION_NOTIFICATION/ID_CONNECTION_LOST");

				SLOGI("A player disconnected, guid = {}", p->guid.ToUint32(p->guid));

				FOR_EACH_CLIENT(i)
					if (gPlayers[i].guid == p->guid) {
						ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, (ST::string)(gPlayers[i].name) + " disconnected.");
						gPlayers[i].guid = UNASSIGNED_RAKNET_GUID;
						break;
					}

				UpdateTeamPanel(); // Without the disconnected player

				// Tell others
				struct USER_PACKET_MESSAGE up_broadcast;
				up_broadcast.id = ID_USER_PACKET_TEAM_PANEL_DIRTY;
				gNetworkOptions.peer->Send((char*)&up_broadcast, sizeof(up_broadcast), MEDIUM_PRIORITY, RELIABLE, 0, UNASSIGNED_RAKNET_GUID, true);

				break;
			// Replication
			case ID_REPLICA_MANAGER_SCOPE_CHANGE:
				// Changed scope of an object
				//ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"ID_REPLICA_MANAGER_SCOPE_CHANGE");
				break;
			case ID_RPC_PLUGIN:
				//ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"ID_RPC_PLUGIN");
				break;
			// Custom
			case ID_USER_PACKET_CONNECT:
			{
				//ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"ID_USER_PACKET_CONNECT");
				struct USER_PACKET_CONNECT* up;
				up = (struct USER_PACKET_CONNECT*)p->data;

				// Registering the new player in the global struct
				bool present = false;
				INT8 first_free = -1;
				FOR_EACH_CLIENT(i) {
					if (gPlayers[i].guid == p->guid) {
						present = true;
						break;
					} else if ((gPlayers[i].guid == UNASSIGNED_RAKNET_GUID) &&
						       (first_free == -1)) {
						first_free = i;
					}
				}

				if ((!present) && (first_free != -1)) {
					gPlayers[first_free].guid = p->guid;
					gPlayers[first_free].name = up->name;
					gPlayers[first_free].ready = up->ready;

					SLOGI("A new player connected, guid = {}", p->guid.ToUint32(p->guid));

					ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, (ST::string)up->name + " connected.");

					// FIXME: Remove?
					UpdateTeamPanel(); // With the connected player

					// Tell others
					struct USER_PACKET_MESSAGE up_broadcast;
					up_broadcast.id = ID_USER_PACKET_TEAM_PANEL_DIRTY;
					gNetworkOptions.peer->Send((char*)&up_broadcast, sizeof(up_broadcast),
						MEDIUM_PRIORITY, RELIABLE, 0, UNASSIGNED_RAKNET_GUID, true);

					// Share the game options with the connected player
					struct USER_PACKET_GAME_OPTIONS up_gops;
					up_gops.id = ID_USER_PACKET_GAME_OPTIONS;

					up_gops.fGunNut = gGameOptions.fGunNut;
					up_gops.fSciFi = gGameOptions.fSciFi;
					up_gops.ubDifficultyLevel = gGameOptions.ubDifficultyLevel;
					up_gops.fTurnTimeLimit = gGameOptions.fTurnTimeLimit;
					up_gops.ubGameSaveMode = gGameOptions.ubGameSaveMode;

					gNetworkOptions.peer->Send((char*)&up_gops, sizeof(up_gops),
						MEDIUM_PRIORITY, RELIABLE, 0, p->guid, false);
				} // Otherwise ignore the duplicated connection request

				break;
			}
			case ID_USER_PACKET_MESSAGE:
			{
				//ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"ID_USER_PACKET_MESSAGE");
				struct USER_PACKET_MESSAGE* up;
				struct USER_PACKET_MESSAGE up_broadcast;
				up = (struct USER_PACKET_MESSAGE*)p->data;

				FOR_EACH_CLIENT(i)
					if (gPlayers[i].guid == p->guid) {
						ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, (ST::string)(gPlayers[i].name) + "> " + (ST::string)up->message);

						// Broadcasting the message to the clients including the one that has sent this message
						up_broadcast.id = ID_USER_PACKET_MESSAGE;
						strcpy(up_broadcast.message, ((ST::string)(gPlayers[i].name) + "> " + (ST::string)up->message).c_str());
						gNetworkOptions.peer->Send((char*)&up_broadcast, sizeof(up_broadcast), MEDIUM_PRIORITY, RELIABLE, 0, UNASSIGNED_RAKNET_GUID, true);

						break;
					}

				break;
			}
			case ID_USER_PACKET_READY:
			{
				//ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"ID_USER_PACKET_READY");
				struct USER_PACKET_READY* up;
				struct USER_PACKET_MESSAGE up_broadcast;
				char str[256];
				up = (struct USER_PACKET_READY*)p->data;

				FOR_EACH_PLAYER(i)
					if (gPlayers[i].guid == p->guid) {
						gPlayers[i].ready = up->ready;

						// Broadcasting the name of the person that is ready
						sprintf(str, "%s is %s.", gPlayers[i].name.C_String(), gPlayers[i].ready ? "ready" : "not ready");
						up_broadcast.id = ID_USER_PACKET_MESSAGE;
						strcpy(up_broadcast.message, str);
						gNetworkOptions.peer->Send((char*)&up_broadcast, sizeof(up_broadcast), MEDIUM_PRIORITY, RELIABLE, 0, UNASSIGNED_RAKNET_GUID, true);

						ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, str); // Duplicating to the server chat

						break;
					}

				break;
			}
			case ID_USER_PACKET_END_TURN:
			{
				//ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"ID_USER_PACKET_END_TURN");
				struct USER_PACKET_END_TURN* up;
				struct USER_PACKET_MESSAGE up_broadcast;
				char str[256];
				up = (struct USER_PACKET_END_TURN*)p->data;

				UINT8 finished = 0;
				FOR_EACH_PLAYER(i)
					if (gPlayers[i].endturn)
						finished++;

				UINT8 n = NumberOfPlayers();

				FOR_EACH_PLAYER(i)
					if (gPlayers[i].guid == p->guid) {
						if (!(gPlayers[i].endturn)) {
							gPlayers[i].endturn = true;
							finished++;

							// Broadcasting the name of the person that has finished its turn
							sprintf(str, "%s has finished his/her turn. %d/%d total.", gPlayers[i].name.C_String(), finished, n);
							up_broadcast.id = ID_USER_PACKET_MESSAGE;
							strcpy(up_broadcast.message, str);
							gNetworkOptions.peer->Send((char*)&up_broadcast, sizeof(up_broadcast), MEDIUM_PRIORITY, RELIABLE, 0, UNASSIGNED_RAKNET_GUID, true);

							ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, str); // Duplicating to the server chat
						}

						break;
					}

				// TODO: In the interrupt mode check only the players, whose mercs have received the interrupt
				if (finished == n)
					gfBeginEndTurn = TRUE;

				break;
			}
			default:
				char unknown_id[3];
				itoa(SpacketIdentifier, unknown_id, 10);
				ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, "SpacketIdentifier = " + (ST::string)unknown_id);
				break;
			}

			// We're done with the packet, get more :)
			gNetworkOptions.peer->DeallocatePacket(p);
			p = gNetworkOptions.peer->Receive();
		}

		Sleep(33); // NOTE: ~30 FPS, can be improved if needed
	}

	return 0;
}

// We are a client and process packets from the server here
DWORD WINAPI client_packet(LPVOID lpParam)
{
	RakNet::Packet* p;
	unsigned char SpacketIdentifier;

	while (TRUE) {
		p = gNetworkOptions.peer->Receive();

		while (p) {
			// We got a packet, get the identifier with our handy function
			SpacketIdentifier = SGetPacketIdentifier(p);

			// Check if this is a network message packet
			switch (SpacketIdentifier) {
			case ID_CONNECTION_REQUEST_ACCEPTED: // This message and below are custom messages of JA2S Coop
				// This tells the client that it has connected
				//ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"ID_CONNECTION_REQUEST_ACCEPTED");

				ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Connected.");

				gNetworkOptions.connected = TRUE;
				break;
			case ID_DISCONNECTION_NOTIFICATION:
			case ID_CONNECTION_LOST:
				ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"ID_DISCONNECTION_NOTIFICATION/ID_CONNECTION_LOST");
				break;
			// Can't connect for various reasons:
			case ID_CONNECTION_ATTEMPT_FAILED:
				ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"ID_CONNECTION_ATTEMPT_FAILED");
				break;
			case ID_NO_FREE_INCOMING_CONNECTIONS:
				// Sorry, the server is full
				ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"ID_NO_FREE_INCOMING_CONNECTIONS");
				break;
			case ID_ALREADY_CONNECTED:
				ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"ID_ALREADY_CONNECTED");
				break;
			// Notifications about other clients:
			case ID_REMOTE_DISCONNECTION_NOTIFICATION: // Server telling the clients of another client disconnecting gracefully.  You can manually broadcast this in a peer to peer enviroment if you want.
			case ID_REMOTE_CONNECTION_LOST: // Server telling the clients of another client disconnecting forcefully.  You can manually broadcast this in a peer to peer enviroment if you want.
				ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"ID_REMOTE_DISCONNECTION_NOTIFICATION/ID_REMOTE_CONNECTION_LOST");
				break;
			case ID_REMOTE_NEW_INCOMING_CONNECTION: // Server telling the clients of another client connecting.  You can manually broadcast this in a peer to peer enviroment if you want.
				ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"ID_REMOT/MING_CONNECTION");
				break;
			// Replication
			case ID_REPLICA_MANAGER_CONSTRUCTION:
				// Create an object
				//ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"ID_REPLICA_MANAGER_CONSTRUCTION");
				break;
			case ID_REPLICA_MANAGER_SCOPE_CHANGE: // Not sure what does this one mean
				// Changed scope of an object
				//ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"ID_REPLICA_MANAGER_SCOPE_CHANGE");
				break;
			case ID_REPLICA_MANAGER_SERIALIZE:
				// Serialized data of an object
				//ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"ID_REPLICA_MANAGER_SERIALIZE");
				break;
			case ID_REPLICA_MANAGER_DOWNLOAD_STARTED:
				// New connection, about to send all world objects
				//ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"ID_REPLICA_MANAGER_DOWNLOAD_STARTED");
				break;
			case ID_REPLICA_MANAGER_DOWNLOAD_COMPLETE:
				// Finished downloading all serialized objects
				//ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"ID_REPLICA_MANAGER_DOWNLOAD_COMPLETE");

				//ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Replication is completed.");
				SLOGI("ID_REPLICA_MANAGER_DOWNLOAD_COMPLETE");

				gReplicaManager.GetReferencedReplicaList(gReplicaList);

				// Update merc list in the left panel to show replicated characters
				UpdateTeamPanel();

				break;
			// Custom
			case ID_USER_PACKET_MESSAGE:
			{
				//ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"ID_USER_PACKET_MESSAGE");
				struct USER_PACKET_MESSAGE* up;
				up = (struct USER_PACKET_MESSAGE*)p->data;

				ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, (ST::string)up->message);

				break;
			}
			case ID_USER_PACKET_START:
			{
				//ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"ID_USER_PACKET_START");

				gStarted = TRUE;

				break;
			}
			case ID_USER_PACKET_TEAM_PANEL_DIRTY:
			{
				//ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"ID_USER_PACKET_TEAM_PANEL_DIRTY");

				UpdateTeamPanel();

				break;
			}
			case ID_USER_PACKET_TOP_MESSAGE:
			{
				struct USER_PACKET_TOP_MESSAGE* up;
				up = (struct USER_PACKET_TOP_MESSAGE*)p->data;

				gTacticalStatus.ubCurrentTeam = up->ubCurrentTeam;
				gTacticalStatus.ubTopMessageType = (MESSAGE_TYPES)(up->ubTopMessageType);
				gTacticalStatus.usTactialTurnLimitCounter = up->usTactialTurnLimitCounter;
				gTacticalStatus.usTactialTurnLimitMax = up->usTactialTurnLimitMax;

				SuspendThread(gMainThread);
				AddTopMessage((MESSAGE_TYPES)(up->ubTopMessageType));
				ResumeThread(gMainThread);

				break;
			}
			case ID_USER_PACKET_END_COMBAT:
			{
				//ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"ID_USER_PACKET_END_COMBAT");

				ExitCombatMode();

				break;
			}
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
			default:
				char unknown_id[3];
				itoa(SpacketIdentifier, unknown_id, 10);
				ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, "SpacketIdentifier = " + (ST::string)unknown_id);
				break;
			}

			// We're done with the packet, get more :)
			gNetworkOptions.peer->DeallocatePacket(p);
			p = gNetworkOptions.peer->Receive();
		}

		Sleep(33); // NOTE: ~30 FPS, can be improved if needed
	}

	return 0;
}

void BeginSoldierClimbUpRoofRPC(RakNet::BitStream* bitStream, RakNet::Packet* packet)
{
	RPC_DATA data;
	int offset = bitStream->GetReadOffset();
	bool read = bitStream->ReadCompressed(data);
	RakAssert(read);

	BeginSoldierClimbUpRoof(ID2Soldier(data.id));
}

void BeginSoldierClimbDownRoofRPC(RakNet::BitStream* bitStream, RakNet::Packet* packet)
{
	RPC_DATA data;
	int offset = bitStream->GetReadOffset();
	bool read = bitStream->ReadCompressed(data);
	RakAssert(read);

	BeginSoldierClimbDownRoof(ID2Soldier(data.id));
}

void BeginSoldierClimbFenceRPC(RakNet::BitStream* bitStream, RakNet::Packet* packet)
{
	RPC_DATA data;
	int offset = bitStream->GetReadOffset();
	bool read = bitStream->ReadCompressed(data);
	RakAssert(read);

	BeginSoldierClimbFence(ID2Soldier(data.id));
}

void BtnStealthModeCallbackRPC(RakNet::BitStream* bitStream, RakNet::Packet* packet)
{
	RPC_DATA data;
	int offset = bitStream->GetReadOffset();
	bool read = bitStream->ReadCompressed(data);
	RakAssert(read);

	SOLDIERTYPE* const s = ID2Soldier(data.id);

	gpSMCurrentMerc->bStealthMode = !(s->bStealthMode);
	gfUIStanceDifferent = TRUE;
	gfPlotNewMovement = TRUE; // Not sure if it is needed, but let it be
	fInterfacePanelDirty = DIRTYLEVEL2;
}

void ChangeWeaponModeRPC(RakNet::BitStream* bitStream, RakNet::Packet* packet)
{
	RPC_DATA data;
	int offset = bitStream->GetReadOffset();
	bool read = bitStream->ReadCompressed(data);
	RakAssert(read);

	ChangeWeaponMode(ID2Soldier(data.id));
}

void UIHandleSoldierStanceChangeRPC(RakNet::BitStream* bitStream, RakNet::Packet* packet)
{
	RPC_DATA data;
	int offset = bitStream->GetReadOffset();
	bool read = bitStream->ReadCompressed(data);
	RakAssert(read);

	UIHandleSoldierStanceChange(ID2Soldier(data.id), data.bNewStance);
}

void SMInvClickCallbackPrimaryRPC(RakNet::BitStream* bitStream, RakNet::Packet* packet)
{
	RPC_DATA data;
	int offset = bitStream->GetReadOffset();
	bool read = bitStream->ReadCompressed(data);
	RakAssert(read);

	gRPC_Inv = &data;

	SMInvClickCallbackPrimary(NULL, 0);

	gRPC_Inv = NULL;
}

void HandleItemPointerClickRPC(RakNet::BitStream* bitStream, RakNet::Packet* packet)
{
	RPC_DATA data;
	int offset = bitStream->GetReadOffset();
	bool read = bitStream->ReadCompressed(data);
	RakAssert(read);

	gRPC_Inv = &data;

	HandleItemPointerClick(data.usMapPos);

	gRPC_Inv = NULL;
}

void HireMercRPC(RakNet::BitStream* bitStream, RakNet::Packet* packet)
{
	RPC_DATA data;
	int offset = bitStream->GetReadOffset();
	bool read = bitStream->ReadCompressed(data);
	RakAssert(read);

	INT8 const ret = HireMerc(data.h);
	if (ret == MERC_HIRE_OK)
	{
		// Set the type of contract the merc is on
		SOLDIERTYPE* const s = FindSoldierByProfileIDOnPlayerTeam(data.h.ubProfileID);
		if (!s) return;
		s->bTypeOfLastContract = data.contract_type;

		MERCPROFILESTRUCT& p = GetProfile(data.h.ubProfileID);
		if (data.fBuyEquipment) p.ubMiscFlags |= PROFILE_MISC_FLAG_ALREADY_USED_ITEMS;

		// Add an entry in the finacial page for the hiring of the merc
		AddTransactionToPlayersBook(HIRED_MERC, data.h.ubProfileID,
			GetWorldTotalMin(), -(data.iContractAmount) +
			p.sMedicalDepositAmount);

		if (p.bMedicalDeposit)
		{ // Add an entry in the finacial page for the medical deposit
			AddTransactionToPlayersBook(MEDICAL_DEPOSIT, data.h.ubProfileID,
				GetWorldTotalMin(), -p.sMedicalDepositAmount);
		}

		// Add an entry in the history page for the hiring of the merc
		AddHistoryToPlayersLog(HISTORY_HIRED_MERC_FROM_AIM, data.h.ubProfileID,
			GetWorldTotalMin(), SGPSector(-1, -1));

		INT8 i = PlayerIndex(packet->guid);
		s->ubPlayer = i;

		// Broadcast
		struct USER_PACKET_MESSAGE up_broadcast;
		char str[256];
		sprintf(str, "%s hired %s.", gPlayers[i].name.C_String(), s->name.c_str());

		up_broadcast.id = ID_USER_PACKET_MESSAGE;
		strcpy(up_broadcast.message, str);
		gNetworkOptions.peer->Send((char*)&up_broadcast, sizeof(up_broadcast),
			MEDIUM_PRIORITY, RELIABLE, 0, UNASSIGNED_RAKNET_GUID, true);

		// FIXME: The line below doesn't (always?) show the message in the host chat
		ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, str);
	}
	else if (ret == MERC_HIRE_OVER_20_MERCS_HIRED)
	{
		// Display a warning saying you can't hire more then 20 mercs
		// TODO: Display to the client
		//DoLapTopMessageBox(MSG_BOX_LAPTOP_DEFAULT,
		// AimPopUpText[AIM_MEMBER_ALREADY_HAVE_20_MERCS], LAPTOP_SCREEN,
		// MSG_BOX_FLAG_OK, NULL);
	}

	UpdateTeamPanel();
}

void AddStrategicEventRPC(RakNet::BitStream* bitStream, RakNet::Packet* packet)
{
	RPC_DATA data;
	int offset = bitStream->GetReadOffset();
	bool read = bitStream->ReadCompressed(data);
	RakAssert(read);

	AddStrategicEvent(data.Kind, data.uiMinStamp, data.uiParam);
}

void AddCharacterToSquadRPC(RakNet::BitStream* bitStream, RakNet::Packet* packet)
{
	RPC_DATA data;
	int offset = bitStream->GetReadOffset();
	bool read = bitStream->ReadCompressed(data);
	RakAssert(read);

	gRPC_Squad = TRUE;

	AddCharacterToSquad(ID2Soldier(data.id), data.bSquadValue);

	gRPC_Squad = FALSE;
}

void AddHistoryToPlayersLogRPC(RakNet::BitStream* bitStream, RakNet::Packet* packet)
{
	RPC_DATA data;
	int offset = bitStream->GetReadOffset();
	bool read = bitStream->ReadCompressed(data);
	RakAssert(read);

	AddHistoryToPlayersLog(data.ubCode, data.ubSecondCode, data.uiDate, SGPSector(-1, -1));
}

void AddTransactionToPlayersBookRPC(RakNet::BitStream* bitStream, RakNet::Packet* packet)
{
	RPC_DATA data;
	int offset = bitStream->GetReadOffset();
	bool read = bitStream->ReadCompressed(data);
	RakAssert(read);

	AddTransactionToPlayersBook(data.ubCode, data.ubSecondCode, data.uiDate, data.iAmount);
}

void HandleEventRPC(RakNet::BitStream* bitStream, RakNet::Packet* packet)
{
	RPC_DATA data;
	int offset = bitStream->GetReadOffset();
	bool read = bitStream->ReadCompressed(data);
	RakAssert(read);

	gRPC_Events.push_back(data);
}
