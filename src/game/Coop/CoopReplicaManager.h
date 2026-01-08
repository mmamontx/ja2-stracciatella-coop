#ifndef __COOP_REPLICA_MANAGER_H
#define __COOP_REPLICA_MANAGER_H

#include "Coop.h"
#include "CoopConnection.h"


using namespace RakNet;

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

#endif // __COOP_REPLICA_MANAGER_H
