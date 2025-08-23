#ifndef COOP_H
#define COOP_H

#include "Handle_UI.h"
#define GROUP GROUP_JA2
#include "NetworkIDManager.h"
#include "RakNet/RPC4Plugin.h"
#include "RakPeerInterface.h"
#include "ReplicaManager3.h"
#undef GROUP
#include "MessageIdentifiers.h"
#include "MouseSystem.h"
#include "Soldier_Control.h"

using namespace RakNet;

#define ID_USER_PACKET_CONNECT          ID_USER_PACKET_ENUM
#define ID_USER_PACKET_MESSAGE          (ID_USER_PACKET_ENUM + 1)
#define ID_USER_PACKET_READY            (ID_USER_PACKET_ENUM + 2)
#define ID_USER_PACKET_START            (ID_USER_PACKET_ENUM + 3)
#define ID_USER_PACKET_TEAM_PANEL_DIRTY (ID_USER_PACKET_ENUM + 4)
#define ID_USER_PACKET_TOP_MESSAGE      (ID_USER_PACKET_ENUM + 5)

#define MAX_NAME_LEN    16
#define MAX_MESSAGE_LEN 128

#define MAX_NUM_PLAYERS 4

#define KEY_RETURN 13

#define IS_CLIENT       (gGameOptions.fNetwork)
#define IS_VALID_CLIENT ((gGameOptions.fNetwork) && (gConnected) && (gReplicaList.Size() != 0))

#define RPC_READY ((gRPC_Events.empty() == FALSE) && gRPC_Enable)

// TODO: Modify the macro below to use an iterator that points to the local (for the server) or the replicated (for the client) variables
#define FOR_EACH_PLAYER(i) for (int i = 0; i < MAX_NUM_PLAYERS; i++)
#define FOR_EACH_CLIENT(i) for (int i = 1; i < MAX_NUM_PLAYERS; i++)

#define REPLICA_PLAYER_INDEX TOTAL_SOLDIERS

struct NETWORK_OPTIONS {
	ST::string name;
	ST::string ip;
	UINT16 port;
	RakPeerInterface *peer;
};

struct USER_PACKET_CONNECT {
	unsigned char id;
	char name[MAX_NAME_LEN];
	BOOLEAN ready;
};

struct USER_PACKET_MESSAGE {
	unsigned char id;
	char message[MAX_MESSAGE_LEN];
};

struct USER_PACKET_READY {
	unsigned char id;
	BOOLEAN ready;
};

struct USER_PACKET_START {
	unsigned char id;
};

struct USER_PACKET_TEAM_PANEL_DIRTY {
	unsigned char id;
};

struct USER_PACKET_TOP_MESSAGE {
	unsigned char id;
	UINT8 ubTopMessageType;
	UINT16 usTactialTurnLimitCounter;
	UINT16 usTactialTurnLimitMax;
};

struct PLAYER : public Replica3 {
	RakNetGUID guid;
	RakString name;
	BOOLEAN ready;

	bool operator == (const PLAYER& s) const { return guid == s.guid; }

	virtual RakNet::RakString GetName(void) const { return RakNet::RakString("PLAYER"); }

	virtual void WriteAllocationID(RakNet::Connection_RM3* destinationConnection, RakNet::BitStream* allocationIdBitstream) const {
		allocationIdBitstream->Write(GetName());
	}

	void PrintStringInBitstream(RakNet::BitStream* bs)
	{
		if (bs->GetNumberOfBitsUsed() == 0)
			return;
		RakNet::RakString rakString;
		bs->Read(rakString);
		//printf("Receive: %s\n", rakString.C_String());
	}

	virtual void SerializeConstruction(RakNet::BitStream* constructionBitstream, RakNet::Connection_RM3* destinationConnection) {

		constructionBitstream->Write(GetName() + RakNet::RakString(" SerializeConstruction"));
	}

	virtual bool DeserializeConstruction(RakNet::BitStream* constructionBitstream, RakNet::Connection_RM3* sourceConnection) {
		PrintStringInBitstream(constructionBitstream);
		return true;
	}

	virtual void SerializeDestruction(RakNet::BitStream* destructionBitstream, RakNet::Connection_RM3* destinationConnection) {

		destructionBitstream->Write(GetName() + RakNet::RakString(" SerializeDestruction"));

	}

	virtual bool DeserializeDestruction(RakNet::BitStream* destructionBitstream, RakNet::Connection_RM3* sourceConnection) {
		PrintStringInBitstream(destructionBitstream);
		return true;
	}

	virtual void DeallocReplica(RakNet::Connection_RM3* sourceConnection) {
		delete this;
	}

	/// Overloaded Replica3 function
	virtual void OnUserReplicaPreSerializeTick(void)
	{
	}

	virtual RM3SerializationResult Serialize(RakNet::SerializeParameters* serializeParameters) {
		serializeParameters->outputBitstream[0].Write(guid);

		//rname = RakString(name);
		//serializeParameters->outputBitstream[0].Write(rname);
		serializeParameters->outputBitstream[0].Write(name);

		serializeParameters->outputBitstream[0].Write(ready);

		return RM3SR_BROADCAST_IDENTICALLY;
	}

	virtual void Deserialize(RakNet::DeserializeParameters* deserializeParameters) {
		deserializeParameters->serializationBitstream[0].Read(guid);

		//deserializeParameters->serializationBitstream[0].Read(rname);
		//strcpy(name, rname.C_String());
		deserializeParameters->serializationBitstream[0].Read(name);

		deserializeParameters->serializationBitstream[0].Read(ready);
	}

	virtual void SerializeConstructionRequestAccepted(RakNet::BitStream* serializationBitstream, RakNet::Connection_RM3* requestingConnection) {
		serializationBitstream->Write(GetName() + RakNet::RakString(" SerializeConstructionRequestAccepted"));
	}

	virtual void DeserializeConstructionRequestAccepted(RakNet::BitStream* serializationBitstream, RakNet::Connection_RM3* acceptingConnection) {
		PrintStringInBitstream(serializationBitstream);
	}

	virtual void SerializeConstructionRequestRejected(RakNet::BitStream* serializationBitstream, RakNet::Connection_RM3* requestingConnection) {
		serializationBitstream->Write(GetName() + RakNet::RakString(" SerializeConstructionRequestRejected"));
	}

	virtual void DeserializeConstructionRequestRejected(RakNet::BitStream* serializationBitstream, RakNet::Connection_RM3* rejectingConnection) {
		PrintStringInBitstream(serializationBitstream);
	}

	virtual void OnPoppedConnection(RakNet::Connection_RM3* droppedConnection)
	{
	}

	void NotifyReplicaOfMessageDeliveryStatus(RakNetGUID guid, uint32_t receiptId, bool messageArrived)
	{
	}

	virtual RM3ConstructionState QueryConstruction(RakNet::Connection_RM3* destinationConnection, ReplicaManager3* replicaManager3) {
		return QueryConstruction_ServerConstruction(destinationConnection, gGameOptions.fNetwork != TRUE);
	}

	virtual bool QueryRemoteConstruction(RakNet::Connection_RM3* sourceConnection) {
		return QueryRemoteConstruction_ServerConstruction(sourceConnection, gGameOptions.fNetwork != TRUE);
	}

	virtual RM3QuerySerializationResult QuerySerialization(RakNet::Connection_RM3* destinationConnection) {
		return QuerySerialization_ServerSerializable(destinationConnection, gGameOptions.fNetwork != TRUE);
	}

	virtual RM3ActionOnPopConnection QueryActionOnPopConnection(RakNet::Connection_RM3* droppedConnection) const {
		return QueryActionOnPopConnection_Server(droppedConnection);
	}
};

struct RPC_DATA {
	UIEventKind puiNewEvent;
	SoldierID id;
	SoldierID tgt_id;
	GridNo usMapPos;
	BOOLEAN fUIMovementFast; // For moving
	INT8 bNewStance; // For changing the stance
	// For operating with items
	UINT8 ubHandPos;
	UINT8 ubCtrl;
	UINT8 ubShift;
	INT16 sCurrentActionPoints;
};

class SampleConnection : public Connection_RM3
{
public:
	SampleConnection(const SystemAddress& _systemAddress, RakNetGUID _guid) : Connection_RM3(_systemAddress, _guid) {}
	virtual ~SampleConnection() {}

	bool QueryGroupDownloadMessages(void) const { return true; }

	virtual Replica3* AllocReplica(BitStream* allocationId, ReplicaManager3* replicaManager3)
	{
		RakString typeName;
		allocationId->Read(typeName);
		if (typeName == "SOLDIERTYPE") return new SOLDIERTYPE;
		if (typeName == "PLAYER") return new PLAYER;
		return 0;
	}
protected:
};

class ReplicaManager3Sample : public ReplicaManager3
{
	virtual Connection_RM3* AllocConnection(const SystemAddress& systemAddress, RakNetGUID rakNetGUID) const
	{
		return new SampleConnection(systemAddress, rakNetGUID);
	}
	virtual void DeallocConnection(Connection_RM3* connection) const
	{
		delete connection;
	}
};

extern BOOLEAN gConnected;
extern BOOLEAN gEnemyEnabled;
extern BOOLEAN gNetworkCreated;
extern BOOLEAN gReady;
extern BOOLEAN gRPC_Enable;
extern BOOLEAN gStarted;
extern DataStructures::List<Replica3*> gReplicaList;
extern NETWORK_OPTIONS gNetworkOptions;
extern NetworkIDManager gNetworkIdManager;
extern OBJECTTYPE* gpItemPointerRPC;
extern SOLDIERTYPE* gpItemPointerSoldierRPC;
extern ReplicaManager3Sample gReplicaManager;
extern RPC_DATA* gRPC_Inv;
extern RPC4 gRPC;
extern std::list<RPC_DATA> gRPC_Events;
extern struct PLAYER gPlayers[MAX_NUM_PLAYERS];

extern DWORD WINAPI client_packet(LPVOID lpParam);
extern DWORD WINAPI replicamgr(LPVOID lpParam);
extern DWORD WINAPI server_packet(LPVOID lpParam);
extern unsigned char SGetPacketIdentifier(Packet* p);
extern void BeginSoldierClimbDownRoofRPC(RakNet::BitStream* bitStream, RakNet::Packet* packet);
extern void BeginSoldierClimbFenceRPC(RakNet::BitStream* bitStream, RakNet::Packet* packet);
extern void BeginSoldierClimbUpRoofRPC(RakNet::BitStream* bitStream, RakNet::Packet* packet);
extern void BtnStealthModeCallbackRPC(RakNet::BitStream* bitStream, RakNet::Packet* packet);
extern void ChangeWeaponModeRPC(RakNet::BitStream* bitStream, RakNet::Packet* packet);
extern void HandleEventRPC(RakNet::BitStream* bitStream, RakNet::Packet* packet);
extern void HandleItemPointerClickRPC(RakNet::BitStream* bitStream, RakNet::Packet* packet);
extern void HireRandomMercs(unsigned int n);
extern void SMInvClickCallbackPrimaryRPC(RakNet::BitStream* bitStream, RakNet::Packet* packet);
extern void UIHandleSoldierStanceChangeRPC(RakNet::BitStream* bitStream, RakNet::Packet* packet);
extern void UpdateTeamPanel();

#endif
