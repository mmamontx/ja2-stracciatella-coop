#ifndef __STRATPATH_H
#define __STRATPATH_H

#include "Strategic_Movement.h"


// directions of movement for badsector determination ( blocking off of a sector exit from foot or vehicle travel)

// Shortest Path Defines
#define NORTH_MOVE -18
#define EAST_MOVE    1
#define WEST_MOVE    -1
#define SOUTH_MOVE  18


INT32 FindStratPath(INT16 sStart, INT16 sDestination, GROUP_JA2 const&, BOOLEAN fTacticalTraversal);

// build a stategic path
PathSt* BuildAStrategicPath(INT16 iStartSectorNum, INT16 iEndSectorNum, GROUP_JA2 const&, BOOLEAN fTacticalTraversal);


// append onto path list
PathSt* AppendStrategicPath(PathSt* pNewSection, PathSt* pHeadOfPathList);


// clear out strategic path list
PathSt* ClearStrategicPathList(PathSt* pHeadOfPath, INT16 sMvtGroup);

// remove head of list
PathSt* RemoveHeadFromStrategicPath(PathSt* pList);

// clear out path list after/including this sector sX, sY..will start at end of path and work it's way back till sector is found...removes most recent sectors first
PathSt* ClearStrategicPathListAfterThisSector(PathSt* pHeadOfPath, const SGPSector& sMap, INT16 sMvtGroup);

// get id of last sector in mercs path list
INT16 GetLastSectorIdInCharactersPath(const SOLDIERTYPE* pCharacter);

// copy paths
PathSt* CopyPaths(PathSt* src);

// rebuild way points for strategic mapscreen path changes
void RebuildWayPointsForGroupPath(PathSt* pHeadOfPath, GROUP_JA2&);

// clear strategic movement (mercpaths and waypoints) for this soldier, and his group (including its vehicles)
void ClearMvtForThisSoldierAndGang( SOLDIERTYPE *pSoldier );

// start movement of this group to this sector...not to be used by the player merc groups.
BOOLEAN MoveGroupFromSectorToSector(GROUP_JA2&, const SGPSector& sStart, const SGPSector& sDest);

BOOLEAN MoveGroupFromSectorToSectorButAvoidPlayerInfluencedSectors(GROUP_JA2&, const SGPSector& sStart, const SGPSector& sDest);
BOOLEAN MoveGroupFromSectorToSectorButAvoidPlayerInfluencedSectorsAndStopOneSectorBeforeEnd(GROUP_JA2&, const SGPSector& sStart, const SGPSector& sDest);


// get length of path
INT32 GetLengthOfPath(PathSt* pHeadPath);
INT32 GetLengthOfMercPath(const SOLDIERTYPE* pSoldier);

PathSt* GetSoldierMercPathPtr(SOLDIERTYPE const*);
PathSt* GetGroupMercPathPtr(GROUP_JA2 const&);

GROUP_JA2* GetSoldierGroup(SOLDIERTYPE const&);

// clears this groups strategic movement (mercpaths and waypoints), include those in the vehicle structs(!)
void ClearMercPathsAndWaypointsForAllInGroup(GROUP_JA2&);

void AddSectorToFrontOfMercPathForAllSoldiersInGroup(GROUP_JA2 *pGroup, const SGPSector& sMap);

#endif
