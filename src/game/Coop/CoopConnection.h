#ifndef __COOP_CONNECTION_H
#define __COOP_CONNECTION_H

#include "Coop.h"
#include "Player.h"
#include "Soldier_Control.h"
#include "Soldier_Profile_Type.h"


using namespace RakNet;

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

#endif // __COOP_CONNECTION_H
