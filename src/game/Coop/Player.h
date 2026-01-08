#ifndef __PLAYER_H
#define __PLAYER_H

#include "Coop.h"


using namespace RakNet;

struct PLAYER : public Replica3
{
	RakNetGUID guid;
	ST::string name;
	RakString rname;
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

	void PreSerialize()
	{
		rname = name.c_str();
	}

	virtual RM3SerializationResult
		Serialize(SerializeParameters* serializeParameters)
	{
		// If we are a client we don't serialize the objects back to the server
		if (IS_CLIENT) return RM3SR_DO_NOT_SERIALIZE;

		PreSerialize();

		serializeParameters->outputBitstream[0].Write(guid);
		serializeParameters->outputBitstream[0].Write(rname);
		serializeParameters->outputBitstream[0].Write(ready);
		serializeParameters->outputBitstream[0].Write(endturn);

		return RM3SR_BROADCAST_IDENTICALLY;
	}

	virtual void Deserialize(DeserializeParameters* deserializeParameters)
	{
		deserializeParameters->serializationBitstream[0].Read(guid);
		deserializeParameters->serializationBitstream[0].Read(rname);
		deserializeParameters->serializationBitstream[0].Read(ready);
		deserializeParameters->serializationBitstream[0].Read(endturn);

		PostDeserialize();
	}

	void PostDeserialize()
	{
		name = rname;
	}

	virtual void
		SerializeConstructionRequestAccepted(BitStream* serializationBitstream,
			Connection_RM3* requestingConnection)
	{
		serializationBitstream->Write(GetName() +
			RakString(" SerializeConstructionRequestAccepted"));
	}

	virtual void
		DeserializeConstructionRequestAccepted(
			BitStream* serializationBitstream,
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
		DeserializeConstructionRequestRejected(
			BitStream* serializationBitstream,
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

#endif // __PLAYER_H
