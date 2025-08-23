#include "Animation_Control.h"
#include "Faces.h"
#include "Game_Clock.h"
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
#include "Strategic.h"
#include "Weapons.h"

#include <Game_Event_Hook.h>

BOOLEAN gConnected = FALSE;
BOOLEAN gEnemyEnabled = TRUE;
BOOLEAN gNetworkCreated = FALSE;
BOOLEAN gReady = FALSE;
BOOLEAN gRPC_Enable = TRUE; // Enables remote events so that they don't overlap with local events
BOOLEAN gStarted = FALSE;
DataStructures::List<Replica3*> gReplicaList;
NETWORK_OPTIONS gNetworkOptions;
NetworkIDManager gNetworkIdManager;
OBJECTTYPE* gpItemPointerRPC = NULL; // If RPCs can be executed in parallel (to be checked) a single shared variable would cause conflicts
SOLDIERTYPE* gpItemPointerSoldierRPC = NULL;
ReplicaManager3Sample gReplicaManager;
RPC_DATA* gRPC_Inv = NULL; // Currently executed inventory RPC
RPC4 gRPC;
std::list<RPC_DATA> gRPC_Events;
struct PLAYER gPlayers[MAX_NUM_PLAYERS];

DWORD WINAPI replicamgr(LPVOID lpParam)
{
	while (1)
		Sleep(1000); // NOTE: It doesn't affect object replication sync delay, but it may have some other effect - keep this in mind
	return 0;
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
			case ID_DISCONNECTION_NOTIFICATION:
				ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"ID_DISCONNECTION_NOTIFICATION");
				break;
			case ID_ALREADY_CONNECTED:
				ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"ID_ALREADY_CONNECTED");
				break;
			case ID_REMOTE_DISCONNECTION_NOTIFICATION: // Server telling the clients of another client disconnecting gracefully.  You can manually broadcast this in a peer to peer enviroment if you want.
				ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"ID_REMOTE_DISCONNECTION_NOTIFICATION");
				break;
			case ID_REMOTE_CONNECTION_LOST: // Server telling the clients of another client disconnecting forcefully.  You can manually broadcast this in a peer to peer enviroment if you want.
				ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"ID_REMOTE_CONNECTION_LOST");
				break;
			case ID_REMOTE_NEW_INCOMING_CONNECTION: // Server telling the clients of another client connecting.  You can manually broadcast this in a peer to peer enviroment if you want.
				ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"ID_REMOT/MING_CONNECTION");
				break;
			case ID_CONNECTION_ATTEMPT_FAILED:
				ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"ID_CONNECTION_ATTEMPT_FAILED");
				break;
			case ID_NO_FREE_INCOMING_CONNECTIONS:
				// Sorry, the server is full
				ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"ID_NO_FREE_INCOMING_CONNECTIONS");
				break;
			case ID_CONNECTION_LOST:
				// Couldn't deliver a reliable packet - i.e. the other system was abnormally
				// terminated
				ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"ID_CONNECTION_LOST");

				FOR_EACH_CLIENT(i)
					if (gPlayers[i].guid == p->guid) {
						ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, (ST::string)(gPlayers[i].name) + " got disconnected (connection lost).");
						gPlayers[i].guid = UNASSIGNED_RAKNET_GUID;
						break;
					}

				UpdateTeamPanel(); // Without the disconnected player

				// Tell others
				struct USER_PACKET_MESSAGE up_broadcast;
				up_broadcast.id = ID_USER_PACKET_TEAM_PANEL_DIRTY;
				gNetworkOptions.peer->Send((char*)&up_broadcast, sizeof(up_broadcast), MEDIUM_PRIORITY, RELIABLE, 0, UNASSIGNED_RAKNET_GUID, true);

				break;
			case ID_CONNECTION_REQUEST_ACCEPTED:
				// This tells the client that it has connected
				ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"ID_CONNECTION_REQUEST_ACCEPTED");
				break;
			case ID_NEW_INCOMING_CONNECTION:
				// This tells the server that a client has connected
				//ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"ID_NEW_INCOMING_CONNECTION");
				break;
			case ID_USER_PACKET_CONNECT: // This message and below are custom messages of JA2S Coop
			{
				//ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"ID_USER_PACKET_CONNECT");
				struct USER_PACKET_CONNECT* up;
				up = (struct USER_PACKET_CONNECT*)p->data;

				// Registering the new player in the global struct
				bool present = false;
				INT8 first_free = -1;
				FOR_EACH_CLIENT(i)
					if (gPlayers[i].guid == p->guid) {
						present = true;
						break;
					} else if (first_free == -1) {
						first_free = i;
					}

				if ((!present) && (first_free != -1)) {
					gPlayers[first_free].guid = p->guid;
					gPlayers[first_free].name = up->name;
					gPlayers[first_free].ready = up->ready;

					ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, (ST::string)up->name + " has connected.");

					UpdateTeamPanel(); // With the connected player

					// Tell others
					struct USER_PACKET_MESSAGE up_broadcast;
					up_broadcast.id = ID_USER_PACKET_TEAM_PANEL_DIRTY;
					gNetworkOptions.peer->Send((char*)&up_broadcast, sizeof(up_broadcast), MEDIUM_PRIORITY, RELIABLE, 0, UNASSIGNED_RAKNET_GUID, true);
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
			case ID_REPLICA_MANAGER_SCOPE_CHANGE:
				// Changed scope of an object
				//ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"ID_REPLICA_MANAGER_SCOPE_CHANGE");
				break;
			case ID_RPC_PLUGIN:
				ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"ID_RPC_PLUGIN");
				break;
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
			case ID_DISCONNECTION_NOTIFICATION:
				ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"ID_DISCONNECTION_NOTIFICATION");
				break;
			case ID_ALREADY_CONNECTED:
				ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"ID_ALREADY_CONNECTED");
				break;
			case ID_REMOTE_DISCONNECTION_NOTIFICATION: // Server telling the clients of another client disconnecting gracefully.  You can manually broadcast this in a peer to peer enviroment if you want.
				ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"ID_REMOTE_DISCONNECTION_NOTIFICATION");
				break;
			case ID_REMOTE_CONNECTION_LOST: // Server telling the clients of another client disconnecting forcefully.  You can manually broadcast this in a peer to peer enviroment if you want.
				ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"ID_REMOTE_CONNECTION_LOST");
				break;
			case ID_REMOTE_NEW_INCOMING_CONNECTION: // Server telling the clients of another client connecting.  You can manually broadcast this in a peer to peer enviroment if you want.
				ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"ID_REMOT/MING_CONNECTION");
				break;
			case ID_CONNECTION_ATTEMPT_FAILED:
				ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"ID_CONNECTION_ATTEMPT_FAILED");
				break;
			case ID_NO_FREE_INCOMING_CONNECTIONS:
				// Sorry, the server is full
				ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"ID_NO_FREE_INCOMING_CONNECTIONS");
				break;
			case ID_CONNECTION_LOST:
				// Couldn't deliver a reliable packet - i.e. the other system was abnormally
				// terminated
				ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"ID_CONNECTION_LOST");
				break;
			case ID_CONNECTION_REQUEST_ACCEPTED:
				// This tells the client that it has connected
				//ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"ID_CONNECTION_REQUEST_ACCEPTED");

				ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Connected to the server.");

				gConnected = TRUE;
				break;
			case ID_NEW_INCOMING_CONNECTION:
				// This tells the server that a client has connected
				ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"ID_NEW_INCOMING_CONNECTION");
				break;
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

				ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Replication is completed.");

				gReplicaManager.GetReferencedReplicaList(gReplicaList);

				FOR_EACH_IN_TEAM(s, OUR_TEAM) { // FIXME: Below are actions that were supposed to be done when hiring mercs through AIM - this should only go in pair with HireRandomMercs()
					gfAtLeastOneMercWasHired = true; // Ugly, but should handle the case when the server doesn't have any characters yet on client connection
					AddStrategicEvent(EVENT_DELAYED_HIRING_OF_MERC, (STARTING_TIME + FIRST_ARRIVAL_DELAY) / NUM_SEC_IN_MIN, s->ubID); // Place to the helicopter
				}

				// Update merc list in the left panel to show replicated characters
				UpdateTeamPanel();

				break;
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
				ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"ID_USER_PACKET_START");

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

				gTacticalStatus.ubTopMessageType = (MESSAGE_TYPES)(up->ubTopMessageType);
				gTacticalStatus.usTactialTurnLimitCounter = up->usTactialTurnLimitCounter;
				gTacticalStatus.usTactialTurnLimitMax = up->usTactialTurnLimitMax;
				AddTopMessage((MESSAGE_TYPES)(up->ubTopMessageType));

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

void HandleEventRPC(RakNet::BitStream* bitStream, RakNet::Packet* packet)
{
	RPC_DATA data;
	int offset = bitStream->GetReadOffset();
	bool read = bitStream->ReadCompressed(data);
	RakAssert(read);

	gRPC_Events.push_back(data);
}
