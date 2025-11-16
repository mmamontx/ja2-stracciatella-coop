#ifndef __SOLDER_PROFILE_TYPE_H
#define __SOLDER_PROFILE_TYPE_H

#include "GameSettings.h"
#include "Types.h"
#define GROUP GROUP_JA2
#include "ReplicaManager3.h"
#undef GROUP

using namespace RakNet;

#include <string_theory/string>


#define NUM_PROFILES						170
#define NUM_RECRUITABLE						75

#define NAME_LENGTH						30
#define NICKNAME_LENGTH					10


//ONLY HAVE 8 MISC FLAGS.. SHOULD BE ENOUGH
#define PROFILE_MISC_FLAG_RECRUITED				0x01
#define PROFILE_MISC_FLAG_HAVESEENCREATURE			0x02
#define PROFILE_MISC_FLAG_FORCENPCQUOTE			0x04
#define PROFILE_MISC_FLAG_WOUNDEDBYPLAYER			0x08
#define PROFILE_MISC_FLAG_TEMP_NPC_QUOTE_DATA_EXISTS		0x10
#define PROFILE_MISC_FLAG_SAID_HOSTILE_QUOTE			0x20
#define PROFILE_MISC_FLAG_EPCACTIVE				0x40
#define PROFILE_MISC_FLAG_ALREADY_USED_ITEMS			0x80 //The player has already purchased the mercs items.

#define PROFILE_MISC_FLAG2_DONT_ADD_TO_SECTOR			0x01
#define PROFILE_MISC_FLAG2_LEFT_COUNTRY			0x02
#define PROFILE_MISC_FLAG2_BANDAGED_TODAY			0x04
#define PROFILE_MISC_FLAG2_SAID_FIRSTSEEN_QUOTE		0x08
#define PROFILE_MISC_FLAG2_NEEDS_TO_SAY_HOSTILE_QUOTE		0x10
#define PROFILE_MISC_FLAG2_MARRIED_TO_HICKS			0x20
#define PROFILE_MISC_FLAG2_ASKED_BY_HICKS			0x40

#define PROFILE_MISC_FLAG3_PLAYER_LEFT_MSG_FOR_MERC_AT_AIM	0x01 // In the aimscreen, the merc was away and the player left a message
#define PROFILE_MISC_FLAG3_PERMANENT_INSERTION_CODE		0x02
#define PROFILE_MISC_FLAG3_PLAYER_HAD_CHANCE_TO_HIRE		0x04 // player's had a chance to hire this merc
#define PROFILE_MISC_FLAG3_HANDLE_DONE_TRAVERSAL		0x08

#define PROFILE_MISC_FLAG3_NPC_PISSED_OFF			0x10
#define PROFILE_MISC_FLAG3_MERC_MERC_IS_DEAD_AND_QUOTE_SAID	0x20 // In the merc site, the merc has died and Speck quote for the dead merc has been said

#define PROFILE_MISC_FLAG3_TOWN_DOESNT_CARE_ABOUT_DEATH	0x40
#define PROFILE_MISC_FLAG3_GOODGUY				0x80
//
// The following variables are used with the 'bMercStatus' variable
//
//

//Merc is ready
#define MERC_OK						0

//if the merc doesnt have a EDT file
#define MERC_HAS_NO_TEXT_FILE					-1

//used in the aim video conferencing screen
#define MERC_ANNOYED_BUT_CAN_STILL_CONTACT			-2
#define MERC_ANNOYED_WONT_CONTACT				-3
#define MERC_HIRED_BUT_NOT_ARRIVED_YET				-4

//self explanatory
#define MERC_IS_DEAD						-5

//set when the merc is returning home.  A delay for 1,2 or 3 days
#define MERC_RETURNING_HOME					-6

// used when merc starts game on assignment, goes on assignment later, or leaves to go on another contract
#define MERC_WORKING_ELSEWHERE					-7

//When the merc was fired, they were a POW, make sure they dont show up in AIM, or MERC as available
#define MERC_FIRED_AS_A_POW					-8


// the values for categories of stats
#define SUPER_STAT_VALUE					80
#define NEEDS_TRAINING_STAT_VALUE				50
#define NO_CHANCE_IN_HELL_STAT_VALUE				40

#define SUPER_SKILL_VALUE					80
#define NEEDS_TRAINING_SKILL_VALUE				50
#define NO_CHANCE_IN_HELL_SKILL_VALUE				0


enum SkillTrait
{
	NO_SKILLTRAIT = 0,
	LOCKPICKING,
	HANDTOHAND,
	ELECTRONICS,
	NIGHTOPS,
	THROWING,
	TEACHING,
	HEAVY_WEAPS,
	AUTO_WEAPS,
	STEALTHY,
	AMBIDEXT,
	THIEF,
	MARTIALARTS,
	KNIFING,
	ONROOF,
	CAMOUFLAGED,
	NUM_SKILLTRAITS
};

enum PersonalityTrait
{
	NO_PERSONALITYTRAIT = 0,
	HEAT_INTOLERANT,
	NERVOUS,
	CLAUSTROPHOBIC,
	NONSWIMMER,
	FEAR_OF_INSECTS,
	FORGETFUL,
	PSYCHO
};

#define NERVOUS_RADIUS 10

enum Attitudes
{
	ATT_NORMAL = 0,
	ATT_FRIENDLY,
	ATT_LONER,
	ATT_OPTIMIST,
	ATT_PESSIMIST,
	ATT_AGGRESSIVE,
	ATT_ARROGANT,
	ATT_BIG_SHOT,
	ATT_ASSHOLE,
	ATT_COWARD,
	NUM_ATTITUDES
};

enum Sexes
{
	MALE = 0,
	FEMALE
};

enum SexistLevels
{
	NOT_SEXIST = 0,
	SOMEWHAT_SEXIST,
	VERY_SEXIST,
	GENTLEMAN
};



// training defines for evolution, no stat increase, stat decrease( de-evolve )
enum CharacterEvolution
{
	NORMAL_EVOLUTION = 0,
	NO_EVOLUTION,
	DEVOLVE,
};

#define BUDDY_MERC(prof, bud) ((prof).bBuddy[0] == (bud) || (prof).bBuddy[1] == (bud) || (prof).bBuddy[2] == (bud))
#define HATED_MERC(prof, hat) ((prof).bHated[0] == (hat) || (prof).bHated[1] == (hat) || (prof).bHated[2] == (hat))

#define BUDDY_OPINION						+25
#define HATED_OPINION						-25

enum BuddySlot
{
	BUDDY_NOT_FOUND = -1,
	BUDDY_SLOT1,
	BUDDY_SLOT2,
	LEARNED_TO_LIKE_SLOT,
	NUM_BUDDY_SLOTS
};

enum HatedSlot
{
	HATED_NOT_FOUND = -1,
	HATED_SLOT1,
	HATED_SLOT2,
	LEARNED_TO_HATE_SLOT,
	NUM_HATED_SLOTS
};

struct MERCPROFILESTRUCT : public Replica3
{
	MERCPROFILESTRUCT()
	{
		/*zName = 0;
		zNickname = 0;*/
		bSex = 0;
		ubCivilianGroup = 0;

		ubMiscFlags = 0;
		ubMiscFlags2 = 0;
		ubMiscFlags3 = 0;

		ubBodyType = 0;
		uiBodyTypeSubFlags = 0; // BODY TYPE SUBSITUTIONS
		/* Portrait */
		ubFaceIndex = 0; // overwritten with the char's ID on profile load
		usEyesX = 0;
		usEyesY = 0;
		usMouthX = 0;
		usMouthY = 0;
		uiBlinkFrequency = 0;
		uiExpressionFrequency = 0;

		/*PANTS = 0;
		VEST = 0;
		SKIN = 0;
		HAIR = 0;*/
		/* stats */
		bEvolution = 0;

		bLifeMax = 0;
		bLife = 0;
		bAgility = 0; // agility (speed) value
		bDexterity = 0; // dexterity (hand coord) value
		bStrength = 0;
		bLeadership = 0;
		bWisdom = 0;
		bExpLevel = 0; // general experience level
		bMarksmanship = 0;
		bExplosive = 0;
		bMechanical = 0;
		bMedical = 0;
		ubNeedForSleep = 0;

		sLifeGain = 0;
		bLifeDelta = 0;
		sAgilityGain = 0;
		bAgilityDelta = 0;
		sDexterityGain = 0;
		bDexterityDelta = 0;
		sStrengthGain = 0;
		bStrengthDelta = 0;
		sLeadershipGain = 0;
		bLeadershipDelta = 0;
		sWisdomGain = 0;
		bWisdomDelta = 0;
		sExpLevelGain = 0;
		bExpLevelDelta = 0;
		sMarksmanshipGain = 0;
		bMarksmanshipDelta = 0;
		sExplosivesGain = 0;
		bExplosivesDelta = 0;
		sMechanicGain = 0;
		bMechanicDelta = 0;
		sMedicalGain = 0;
		bMedicalDelta = 0;

		for (int i = 0; i < 12; i++) {
			usStatChangeChances[i] = 0; // used strictly for balancing, never shown!
			usStatChangeSuccesses[i] = 0; // used strictly for balancing, never shown!
		}

		bPersonalityTrait = 0;
		bSkillTrait = 0;
		bSkillTrait2 = 0;
		bAttitude = 0;
		bSexist = 0;

		/* Contract */
		bMercStatus = 0; //The status of the merc. If negative, see flags at the top of this file. Positive: The number of days the merc is away for. 0: Not hired but ready to be.
		bReputationTolerance = 0;
		bDeathRate = 0;
		uiDayBecomesAvailable = 0; //day the merc will be available. used with the bMercStatus
		sSalary = 0;
		uiWeeklySalary = 0;
		uiBiWeeklySalary = 0;
		bMedicalDeposit = 0; // Is medical deposit required?
		sMedicalDepositAmount = 0;
		iMercMercContractLength = 0; //Used for MERC mercs, specifies how many days the merc has gone since last page
		usOptionalGearCost = 0;
		ubSuspiciousDeath = 0;
		ubDaysOfMoraleHangover = 0; // used only when merc leaves team while having poor morale
		/* Locations */
		sSector = 0;
		sGridNo = 0; // The Gridno the NPC was in before leaving the sector
		sPreCombatGridNo = 0;
		ubStrategicInsertionCode = 0;
		usStrategicInsertionData = 0;
		fUseProfileInsertionInfo = 0; // Set to various flags, ( contained in TacticalSave.h )
		bTown = 0;
		bTownAttachment = 0;
		for (int i = 0; i < 2; i++) {
			ubRoomRangeStart[i] = 0;
			ubRoomRangeEnd[i] = 0;
		}

		for (int i = 0; i < 5; i++) {
			bBuddy[i] = 0; // Only indices 0, 1, 2 are used. Contain id's for friend1, friend2 and eventual friend respectively
			bHated[i] = 0; // Only indices 0, 1, 2 are used. Contain id's for enemy1, enemy2 and eventual enemy respectively
			bHatedCount[i] = 0; // Only indices 0, 1, 2 are used. Contain remaining decrements till contract termination due to an enemy present on the team
			bHatedTime[i] = 0; // Only indices 0, 1, 2 are used. Contain decrements till contract termination due to an enemy present on the team
		}
		bLearnToLike = 0; // eventual friend's id
		bLearnToLikeCount = 0; // remaining decrements till the eventual friend becomes an actual friend
		bLearnToLikeTime = 0; // how many decrements till the eventual friend becomes an actual friend
		bLearnToHate = 0; // eventual enemy's id
		bLearnToHateCount = 0; // remaining decrements till the eventual enemy becomes an actual enemy
		bLearnToHateTime = 0; // how many decrements till the eventual enemy becomes an actual enemy
		// Flags used for the precedent to repeating oneself in Contract negotiations. Used for quote 80 - ~107. Gets reset every day
		ubTimeTillNextHatedComplaint = 0;

		for (int i = 0; i < 75; i++) {
			bMercOpinion[i] = 0;
		}

		for (int i = 0; i < 19; i++) {
			inv[i] = 0;
			bInvNumber[i] = 0;
			bInvStatus[i] = 0;
		}
		ubInvUndroppable = 0;
		uiMoney = 0;
		bArmourAttractiveness = 0;
		bMainGunAttractiveness = 0;

		iBalance = 0; // if negative the player owes money to this NPC (e.g. for Skyrider's services)
		ubNumTimesDrugUseInLifetime = 0; // The # times a drug has been used in the player's lifetime...
		/* Specific quest or script related */
		bNPCData = 0; // NPC specific
		bNPCData2 = 0; // NPC specific
		/* Dialogue and script records */
		ubQuoteRecord = 0;
		ubLastQuoteSaid = 0;
		uiPrecedentQuoteSaid = 0;
		bLastQuoteSaidWasSpecial = 0;
		ubLastDateSpokenTo = 0;
		ubQuoteActionID = 0;
		bFriendlyOrDirectDefaultResponseUsedRecently = 0;
		bRecruitDefaultResponseUsedRecently = 0;
		bThreatenDefaultResponseUsedRecently = 0;

		bApproached = 0;
		for (int i = 0; i < 4; i++) {
			usApproachFactor[i] = 0;
			ubApproachVal[i] = 0;
		}
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 4; j++) {
				ubApproachMod[i][j] = 0;
			}
		}
		/* Statistics */
		usKills = 0;
		usAssists = 0;
		usShotsFired = 0;
		usShotsHit = 0;
		usBattlesFought = 0;
		usTimesWounded = 0;
		usTotalDaysServed = 0;
		uiTotalCostToDate = 0; // The total amount of money that has been paid to the merc for their salary

		bSectorZ = 0; // unused
		bRace = 0; // unused
		bRacist = 0; // unused
		bNationality = 0; // unused
		bAppearance = 0; // unused
		bAppearanceCareLevel = 0; // unused
		bRefinement = 0; // unused
		bRefinementCareLevel = 0; // unused
		bHatedNationality = 0; // unused
		bHatedNationalityCareLevel = 0; // unused
	}

	MERCPROFILESTRUCT& MERCPROFILESTRUCT::operator =(const MERCPROFILESTRUCT& other)
	{
		zName = other.zName.c_str();
		zNickname = other.zNickname.c_str();
		bSex = other.bSex;
		ubCivilianGroup = other.ubCivilianGroup;

		ubMiscFlags = other.ubMiscFlags;
		ubMiscFlags2 = other.ubMiscFlags2;
		ubMiscFlags3 = other.ubMiscFlags3;

		ubBodyType = other.ubBodyType;
		uiBodyTypeSubFlags = other.uiBodyTypeSubFlags; // BODY TYPE SUBSITUTIONS
		/* Portrait */
		ubFaceIndex = other.ubFaceIndex; // overwritten with the char's ID on profile load
		usEyesX = other.usEyesX;
		usEyesY = other.usEyesY;
		usMouthX = other.usMouthX;
		usMouthY = other.usMouthY;
		uiBlinkFrequency = other.uiBlinkFrequency;
		uiExpressionFrequency = other.uiExpressionFrequency;

		PANTS = other.PANTS.c_str();
		VEST = other.VEST.c_str();
		SKIN = other.SKIN.c_str();
		HAIR = other.HAIR.c_str();
		/* stats */
		bEvolution = other.bEvolution;

		bLifeMax = other.bLifeMax;
		bLife = other.bLife;
		bAgility = other.bAgility; // agility (speed) value
		bDexterity = other.bDexterity; // dexterity (hand coord) value
		bStrength = other.bStrength;
		bLeadership = other.bLeadership;
		bWisdom = other.bWisdom;
		bExpLevel = other.bExpLevel;  // general experience level
		bMarksmanship = other.bMarksmanship;
		bExplosive = other.bExplosive;
		bMechanical = other.bMechanical;
		bMedical = other.bMedical;
		ubNeedForSleep = other.ubNeedForSleep;

		sLifeGain = other.sLifeGain;
		bLifeDelta = other.bLifeDelta;
		sAgilityGain = other.sAgilityGain;
		bAgilityDelta = other.bAgilityDelta;
		sDexterityGain = other.sDexterityGain;
		bDexterityDelta = other.bDexterityDelta;
		sStrengthGain = other.sStrengthGain;
		bStrengthDelta = other.bStrengthDelta;
		sLeadershipGain = other.sLeadershipGain;
		bLeadershipDelta = other.bLeadershipDelta;
		sWisdomGain = other.sWisdomGain;
		bWisdomDelta = other.bWisdomDelta;
		sExpLevelGain = other.sExpLevelGain;
		bExpLevelDelta = other.bExpLevelDelta;
		sMarksmanshipGain = other.sMarksmanshipGain;
		bMarksmanshipDelta = other.bMarksmanshipDelta;
		sExplosivesGain = other.sExplosivesGain;
		bExplosivesDelta = other.bExplosivesDelta;
		sMechanicGain = other.sMechanicGain;
		bMechanicDelta = other.bMechanicDelta;
		sMedicalGain = other.sMedicalGain;
		bMedicalDelta = other.bMedicalDelta;

		for (int i = 0; i < 12; i++) {
			usStatChangeChances[i] = other.usStatChangeChances[i]; // used strictly for balancing, never shown!
			usStatChangeSuccesses[i] = other.usStatChangeSuccesses[i]; // used strictly for balancing, never shown!
		}

		bPersonalityTrait = other.bPersonalityTrait;
		bSkillTrait = other.bSkillTrait;
		bSkillTrait2 = other.bSkillTrait2;
		bAttitude = other.bAttitude;
		bSexist = other.bSexist;

		/* Contract */
		bMercStatus = other.bMercStatus; //The status of the merc. If negative, see flags at the top of this file. Positive: The number of days the merc is away for. 0: Not hired but ready to be.
		bReputationTolerance = other.bReputationTolerance;
		bDeathRate = other.bDeathRate;
		uiDayBecomesAvailable = other.uiDayBecomesAvailable; //day the merc will be available. used with the bMercStatus
		sSalary = other.sSalary;
		uiWeeklySalary = other.uiWeeklySalary;
		uiBiWeeklySalary = other.uiBiWeeklySalary;
		bMedicalDeposit = other.bMedicalDeposit; // Is medical deposit required?
		sMedicalDepositAmount = other.sMedicalDepositAmount;
		iMercMercContractLength = other.iMercMercContractLength; //Used for MERC mercs, specifies how many days the merc has gone since last page
		usOptionalGearCost = other.usOptionalGearCost;
		ubSuspiciousDeath = other.ubSuspiciousDeath;
		ubDaysOfMoraleHangover = other.ubDaysOfMoraleHangover; // used only when merc leaves team while having poor morale
		/* Locations */
		sSector = other.sSector;
		sGridNo = other.sGridNo; // The Gridno the NPC was in before leaving the sector
		sPreCombatGridNo = other.sPreCombatGridNo;
		ubStrategicInsertionCode = other.ubStrategicInsertionCode;
		usStrategicInsertionData = other.usStrategicInsertionData;
		fUseProfileInsertionInfo = other.fUseProfileInsertionInfo; // Set to various flags, ( contained in TacticalSave.h )
		bTown = other.bTown;
		bTownAttachment = other.bTownAttachment;
		for (int i = 0; i < 2; i++) {
			ubRoomRangeStart[i] = other.ubRoomRangeStart[i];
			ubRoomRangeEnd[i] = other.ubRoomRangeEnd[i];
		}

		for (int i = 0; i < 5; i++) {
			bBuddy[i] = other.bBuddy[i]; // Only indices 0, 1, 2 are used. Contain id's for friend1, friend2 and eventual friend respectively
			bHated[i] = other.bHated[i]; // Only indices 0, 1, 2 are used. Contain id's for enemy1, enemy2 and eventual enemy respectively
			bHatedCount[i] = other.bHatedCount[i]; // Only indices 0, 1, 2 are used. Contain remaining decrements till contract termination due to an enemy present on the team
			bHatedTime[i] = other.bHatedTime[i]; // Only indices 0, 1, 2 are used. Contain decrements till contract termination due to an enemy present on the team
		}
		bLearnToLike = other.bLearnToLike; // eventual friend's id
		bLearnToLikeCount = other.bLearnToLikeCount; // remaining decrements till the eventual friend becomes an actual friend
		bLearnToLikeTime = other.bLearnToLikeTime; // how many decrements till the eventual friend becomes an actual friend
		bLearnToHate = other.bLearnToHate; // eventual enemy's id
		bLearnToHateCount = other.bLearnToHateCount; // remaining decrements till the eventual enemy becomes an actual enemy
		bLearnToHateTime = other.bLearnToHateTime; // how many decrements till the eventual enemy becomes an actual enemy
		// Flags used for the precedent to repeating oneself in Contract negotiations. Used for quote 80 - ~107. Gets reset every day
		ubTimeTillNextHatedComplaint = other.ubTimeTillNextHatedComplaint;

		for (int i = 0; i < 75; i++) {
			bMercOpinion[i] = other.bMercOpinion[i];
		}

		for (int i = 0; i < 19; i++) {
			inv[i] = other.inv[i];
			bInvNumber[i] = other.bInvNumber[i];
			bInvStatus[i] = other.bInvStatus[i];
		}
		ubInvUndroppable = other.ubInvUndroppable;
		uiMoney = other.uiMoney;
		bArmourAttractiveness = other.bArmourAttractiveness;
		bMainGunAttractiveness = other.bMainGunAttractiveness;

		iBalance = other.iBalance; // if negative the player owes money to this NPC (e.g. for Skyrider's services)
		ubNumTimesDrugUseInLifetime = other.ubNumTimesDrugUseInLifetime; // The # times a drug has been used in the player's lifetime...
		/* Specific quest or script related */
		bNPCData = other.bNPCData; // NPC specific
		bNPCData2 = other.bNPCData2; // NPC specific
		/* Dialogue and script records */
		ubQuoteRecord = other.ubQuoteRecord;
		ubLastQuoteSaid = other.ubLastQuoteSaid;
		uiPrecedentQuoteSaid = other.uiPrecedentQuoteSaid;
		bLastQuoteSaidWasSpecial = other.bLastQuoteSaidWasSpecial;
		ubLastDateSpokenTo = other.ubLastDateSpokenTo;
		ubQuoteActionID = other.ubQuoteActionID;
		bFriendlyOrDirectDefaultResponseUsedRecently = other.bFriendlyOrDirectDefaultResponseUsedRecently;
		bRecruitDefaultResponseUsedRecently = other.bRecruitDefaultResponseUsedRecently;
		bThreatenDefaultResponseUsedRecently = other.bThreatenDefaultResponseUsedRecently;

		bApproached = other.bApproached;
		for (int i = 0; i < 4; i++) {
			usApproachFactor[i] = other.usApproachFactor[i];
			ubApproachVal[i] = other.ubApproachVal[i];
		}
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 4; j++) {
				ubApproachMod[i][j] = other.ubApproachMod[i][j];
			}
		}
		/* Statistics */
		usKills = other.usKills;
		usAssists = other.usAssists;
		usShotsFired = other.usShotsFired;
		usShotsHit = other.usShotsHit;
		usBattlesFought = other.usBattlesFought;
		usTimesWounded = other.usTimesWounded;
		usTotalDaysServed = other.usTotalDaysServed;
		uiTotalCostToDate = other.uiTotalCostToDate; // The total amount of money that has been paid to the merc for their salary

		bSectorZ = other.bSectorZ; // unused
		bRace = other.bRace; // unused
		bRacist = other.bRacist; // unused
		bNationality = other.bNationality; // unused
		bAppearance = other.bAppearance; // unused
		bAppearanceCareLevel = other.bAppearanceCareLevel; // unused
		bRefinement = other.bRefinement; // unused
		bRefinementCareLevel = other.bRefinementCareLevel; // unused
		bHatedNationality = other.bHatedNationality; // unused
		bHatedNationalityCareLevel = other.bHatedNationalityCareLevel; // unused

		return *this;
	}

	MERCPROFILESTRUCT(const MERCPROFILESTRUCT& other) : MERCPROFILESTRUCT()
	{
		*this = other;
	}

	virtual RakNet::RakString GetName(void) const { return RakNet::RakString("MERCPROFILESTRUCT"); }

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
	virtual void OnUserReplicaPreSerializeTick(void) {
	}

	virtual RM3SerializationResult Serialize(RakNet::SerializeParameters* serializeParameters) {
		if (gGameOptions.fNetwork) // If we are client we don't serialize objects back to server
			return RM3SR_DO_NOT_SERIALIZE;
		else
			PreSerialize();

		/*serializeParameters->outputBitstream[0].Write(zName);
		serializeParameters->outputBitstream[0].Write(zNickname);*/
		serializeParameters->outputBitstream[0].Write(rzName);
		serializeParameters->outputBitstream[0].Write(rzNickname);
		serializeParameters->outputBitstream[0].Write(bSex);
		serializeParameters->outputBitstream[0].Write(ubCivilianGroup);

		serializeParameters->outputBitstream[0].Write(ubMiscFlags);
		serializeParameters->outputBitstream[0].Write(ubMiscFlags2);
		serializeParameters->outputBitstream[0].Write(ubMiscFlags3);

		serializeParameters->outputBitstream[0].Write(ubBodyType);
		serializeParameters->outputBitstream[0].Write(uiBodyTypeSubFlags); // BODY TYPE SUBSITUTIONS
		/* Portrait */
		serializeParameters->outputBitstream[0].Write(ubFaceIndex); // overwritten with the char's ID on profile load
		serializeParameters->outputBitstream[0].Write(usEyesX);
		serializeParameters->outputBitstream[0].Write(usEyesY);
		serializeParameters->outputBitstream[0].Write(usMouthX);
		serializeParameters->outputBitstream[0].Write(usMouthY);
		serializeParameters->outputBitstream[0].Write(uiBlinkFrequency);
		serializeParameters->outputBitstream[0].Write(uiExpressionFrequency);

		/*serializeParameters->outputBitstream[0].Write(PANTS);
		serializeParameters->outputBitstream[0].Write(VEST);
		serializeParameters->outputBitstream[0].Write(SKIN);
		serializeParameters->outputBitstream[0].Write(HAIR);*/
		serializeParameters->outputBitstream[0].Write(rPANTS);
		serializeParameters->outputBitstream[0].Write(rVEST);
		serializeParameters->outputBitstream[0].Write(rSKIN);
		serializeParameters->outputBitstream[0].Write(rHAIR);
		/* stats */
		serializeParameters->outputBitstream[0].Write(bEvolution);

		serializeParameters->outputBitstream[0].Write(bLifeMax);
		serializeParameters->outputBitstream[0].Write(bLife);
		serializeParameters->outputBitstream[0].Write(bAgility); // agility (speed) value
		serializeParameters->outputBitstream[0].Write(bDexterity); // dexterity (hand coord) value
		serializeParameters->outputBitstream[0].Write(bStrength);
		serializeParameters->outputBitstream[0].Write(bLeadership);
		serializeParameters->outputBitstream[0].Write(bWisdom);
		serializeParameters->outputBitstream[0].Write(bExpLevel); // general experience level
		serializeParameters->outputBitstream[0].Write(bMarksmanship);
		serializeParameters->outputBitstream[0].Write(bExplosive);
		serializeParameters->outputBitstream[0].Write(bMechanical);
		serializeParameters->outputBitstream[0].Write(bMedical);
		serializeParameters->outputBitstream[0].Write(ubNeedForSleep);

		serializeParameters->outputBitstream[0].Write(sLifeGain);
		serializeParameters->outputBitstream[0].Write(bLifeDelta);
		serializeParameters->outputBitstream[0].Write(sAgilityGain);
		serializeParameters->outputBitstream[0].Write(bAgilityDelta);
		serializeParameters->outputBitstream[0].Write(sDexterityGain);
		serializeParameters->outputBitstream[0].Write(bDexterityDelta);
		serializeParameters->outputBitstream[0].Write(sStrengthGain);
		serializeParameters->outputBitstream[0].Write(bStrengthDelta);
		serializeParameters->outputBitstream[0].Write(sLeadershipGain);
		serializeParameters->outputBitstream[0].Write(bLeadershipDelta);
		serializeParameters->outputBitstream[0].Write(sWisdomGain);
		serializeParameters->outputBitstream[0].Write(bWisdomDelta);
		serializeParameters->outputBitstream[0].Write(sExpLevelGain);
		serializeParameters->outputBitstream[0].Write(bExpLevelDelta);
		serializeParameters->outputBitstream[0].Write(sMarksmanshipGain);
		serializeParameters->outputBitstream[0].Write(bMarksmanshipDelta);
		serializeParameters->outputBitstream[0].Write(sExplosivesGain);
		serializeParameters->outputBitstream[0].Write(bExplosivesDelta);
		serializeParameters->outputBitstream[0].Write(sMechanicGain);
		serializeParameters->outputBitstream[0].Write(bMechanicDelta);
		serializeParameters->outputBitstream[0].Write(sMedicalGain);
		serializeParameters->outputBitstream[0].Write(bMedicalDelta);

		serializeParameters->outputBitstream[0].Write(usStatChangeChances); // used strictly for balancing, never shown!
		serializeParameters->outputBitstream[0].Write(usStatChangeSuccesses); // used strictly for balancing, never shown!

		serializeParameters->outputBitstream[0].Write(bPersonalityTrait);
		serializeParameters->outputBitstream[0].Write(bSkillTrait);
		serializeParameters->outputBitstream[0].Write(bSkillTrait2);
		serializeParameters->outputBitstream[0].Write(bAttitude);
		serializeParameters->outputBitstream[0].Write(bSexist);

		/* Contract */
		serializeParameters->outputBitstream[0].Write(bMercStatus); //The status of the merc. If negative, see flags at the top of this file. Positive: The number of days the merc is away for. 0: Not hired but ready to be.
		serializeParameters->outputBitstream[0].Write(bReputationTolerance);
		serializeParameters->outputBitstream[0].Write(bDeathRate);
		serializeParameters->outputBitstream[0].Write(uiDayBecomesAvailable); //day the merc will be available. used with the bMercStatus
		serializeParameters->outputBitstream[0].Write(sSalary);
		serializeParameters->outputBitstream[0].Write(uiWeeklySalary);
		serializeParameters->outputBitstream[0].Write(uiBiWeeklySalary);
		serializeParameters->outputBitstream[0].Write(bMedicalDeposit); // Is medical deposit required?
		serializeParameters->outputBitstream[0].Write(sMedicalDepositAmount);
		serializeParameters->outputBitstream[0].Write(iMercMercContractLength); //Used for MERC mercs, specifies how many days the merc has gone since last page
		serializeParameters->outputBitstream[0].Write(usOptionalGearCost);
		serializeParameters->outputBitstream[0].Write(ubSuspiciousDeath);
		serializeParameters->outputBitstream[0].Write(ubDaysOfMoraleHangover); // used only when merc leaves team while having poor morale
		/* Locations */
		serializeParameters->outputBitstream[0].Write(sSector);
		serializeParameters->outputBitstream[0].Write(sGridNo); // The Gridno the NPC was in before leaving the sector
		serializeParameters->outputBitstream[0].Write(sPreCombatGridNo);
		serializeParameters->outputBitstream[0].Write(ubStrategicInsertionCode);
		serializeParameters->outputBitstream[0].Write(usStrategicInsertionData);
		serializeParameters->outputBitstream[0].Write(fUseProfileInsertionInfo); // Set to various flags, ( contained in TacticalSave.h )
		serializeParameters->outputBitstream[0].Write(bTown);
		serializeParameters->outputBitstream[0].Write(bTownAttachment);
		serializeParameters->outputBitstream[0].Write(ubRoomRangeStart);
		serializeParameters->outputBitstream[0].Write(ubRoomRangeEnd);

		serializeParameters->outputBitstream[0].Write(bBuddy); // Only indices 0, 1, 2 are used. Contain id's for friend1, friend2 and eventual friend respectively
		serializeParameters->outputBitstream[0].Write(bHated); // Only indices 0, 1, 2 are used. Contain id's for enemy1, enemy2 and eventual enemy respectively
		serializeParameters->outputBitstream[0].Write(bHatedCount); // Only indices 0, 1, 2 are used. Contain remaining decrements till contract termination due to an enemy present on the team
		serializeParameters->outputBitstream[0].Write(bHatedTime); // Only indices 0, 1, 2 are used. Contain decrements till contract termination due to an enemy present on the team
		serializeParameters->outputBitstream[0].Write(bLearnToLike); // eventual friend's id
		serializeParameters->outputBitstream[0].Write(bLearnToLikeCount); // remaining decrements till the eventual friend becomes an actual friend
		serializeParameters->outputBitstream[0].Write(bLearnToLikeTime); // how many decrements till the eventual friend becomes an actual friend
		serializeParameters->outputBitstream[0].Write(bLearnToHate); // eventual enemy's id
		serializeParameters->outputBitstream[0].Write(bLearnToHateCount); // remaining decrements till the eventual enemy becomes an actual enemy
		serializeParameters->outputBitstream[0].Write(bLearnToHateTime); // how many decrements till the eventual enemy becomes an actual enemy
		// Flags used for the precedent to repeating oneself in Contract negotiations. Used for quote 80 - ~107. Gets reset every day
		serializeParameters->outputBitstream[0].Write(ubTimeTillNextHatedComplaint);

		serializeParameters->outputBitstream[0].Write(bMercOpinion);

		serializeParameters->outputBitstream[0].Write(inv);
		serializeParameters->outputBitstream[0].Write(bInvNumber);
		serializeParameters->outputBitstream[0].Write(bInvStatus);
		serializeParameters->outputBitstream[0].Write(ubInvUndroppable);
		serializeParameters->outputBitstream[0].Write(uiMoney);
		serializeParameters->outputBitstream[0].Write(bArmourAttractiveness);
		serializeParameters->outputBitstream[0].Write(bMainGunAttractiveness);

		serializeParameters->outputBitstream[0].Write(iBalance); // if negative the player owes money to this NPC (e.g. for Skyrider's services)
		serializeParameters->outputBitstream[0].Write(ubNumTimesDrugUseInLifetime); // The # times a drug has been used in the player's lifetime...
		/* Specific quest or script related */
		serializeParameters->outputBitstream[0].Write(bNPCData); // NPC specific
		serializeParameters->outputBitstream[0].Write(bNPCData2); // NPC specific
		/* Dialogue and script records */
		serializeParameters->outputBitstream[0].Write(ubQuoteRecord);
		serializeParameters->outputBitstream[0].Write(ubLastQuoteSaid);
		serializeParameters->outputBitstream[0].Write(uiPrecedentQuoteSaid);
		serializeParameters->outputBitstream[0].Write(bLastQuoteSaidWasSpecial);
		serializeParameters->outputBitstream[0].Write(ubLastDateSpokenTo);
		serializeParameters->outputBitstream[0].Write(ubQuoteActionID);
		serializeParameters->outputBitstream[0].Write(bFriendlyOrDirectDefaultResponseUsedRecently);
		serializeParameters->outputBitstream[0].Write(bRecruitDefaultResponseUsedRecently);
		serializeParameters->outputBitstream[0].Write(bThreatenDefaultResponseUsedRecently);

		serializeParameters->outputBitstream[0].Write(bApproached);
		serializeParameters->outputBitstream[0].Write(usApproachFactor);
		serializeParameters->outputBitstream[0].Write(ubApproachVal);
		serializeParameters->outputBitstream[0].Write(ubApproachMod);
		/* Statistics */
		serializeParameters->outputBitstream[0].Write(usKills);
		serializeParameters->outputBitstream[0].Write(usAssists);
		serializeParameters->outputBitstream[0].Write(usShotsFired);
		serializeParameters->outputBitstream[0].Write(usShotsHit);
		serializeParameters->outputBitstream[0].Write(usBattlesFought);
		serializeParameters->outputBitstream[0].Write(usTimesWounded);
		serializeParameters->outputBitstream[0].Write(usTotalDaysServed);
		serializeParameters->outputBitstream[0].Write(uiTotalCostToDate); // The total amount of money that has been paid to the merc for their salary

		serializeParameters->outputBitstream[0].Write(bSectorZ); // unused
		serializeParameters->outputBitstream[0].Write(bRace); // unused
		serializeParameters->outputBitstream[0].Write(bRacist); // unused
		serializeParameters->outputBitstream[0].Write(bNationality); // unused
		serializeParameters->outputBitstream[0].Write(bAppearance); // unused
		serializeParameters->outputBitstream[0].Write(bAppearanceCareLevel); // unused
		serializeParameters->outputBitstream[0].Write(bRefinement); // unused
		serializeParameters->outputBitstream[0].Write(bRefinementCareLevel); // unused
		serializeParameters->outputBitstream[0].Write(bHatedNationality); // unused
		serializeParameters->outputBitstream[0].Write(bHatedNationalityCareLevel); // unused

		return RM3SR_BROADCAST_IDENTICALLY;
	}

	virtual void Deserialize(RakNet::DeserializeParameters* deserializeParameters) {
		/*deserializeParameters->serializationBitstream[0].Read(zName);
		deserializeParameters->serializationBitstream[0].Read(zNickname);*/
		deserializeParameters->serializationBitstream[0].Read(rzName);
		deserializeParameters->serializationBitstream[0].Read(rzNickname);
		deserializeParameters->serializationBitstream[0].Read(bSex);
		deserializeParameters->serializationBitstream[0].Read(ubCivilianGroup);

		deserializeParameters->serializationBitstream[0].Read(ubMiscFlags);
		deserializeParameters->serializationBitstream[0].Read(ubMiscFlags2);
		deserializeParameters->serializationBitstream[0].Read(ubMiscFlags3);

		deserializeParameters->serializationBitstream[0].Read(ubBodyType);
		deserializeParameters->serializationBitstream[0].Read(uiBodyTypeSubFlags); // BODY TYPE SUBSITUTIONS
		/* Portrait */
		deserializeParameters->serializationBitstream[0].Read(ubFaceIndex); // overwritten with the char's ID on profile load
		deserializeParameters->serializationBitstream[0].Read(usEyesX);
		deserializeParameters->serializationBitstream[0].Read(usEyesY);
		deserializeParameters->serializationBitstream[0].Read(usMouthX);
		deserializeParameters->serializationBitstream[0].Read(usMouthY);
		deserializeParameters->serializationBitstream[0].Read(uiBlinkFrequency);
		deserializeParameters->serializationBitstream[0].Read(uiExpressionFrequency);

		/*deserializeParameters->serializationBitstream[0].Read(PANTS);
		deserializeParameters->serializationBitstream[0].Read(VEST);
		deserializeParameters->serializationBitstream[0].Read(SKIN);
		deserializeParameters->serializationBitstream[0].Read(HAIR);*/
		deserializeParameters->serializationBitstream[0].Read(rPANTS);
		deserializeParameters->serializationBitstream[0].Read(rVEST);
		deserializeParameters->serializationBitstream[0].Read(rSKIN);
		deserializeParameters->serializationBitstream[0].Read(rHAIR);
		/* stats */
		deserializeParameters->serializationBitstream[0].Read(bEvolution);

		deserializeParameters->serializationBitstream[0].Read(bLifeMax);
		deserializeParameters->serializationBitstream[0].Read(bLife);
		deserializeParameters->serializationBitstream[0].Read(bAgility); // agility (speed) value
		deserializeParameters->serializationBitstream[0].Read(bDexterity); // dexterity (hand coord) value
		deserializeParameters->serializationBitstream[0].Read(bStrength);
		deserializeParameters->serializationBitstream[0].Read(bLeadership);
		deserializeParameters->serializationBitstream[0].Read(bWisdom);
		deserializeParameters->serializationBitstream[0].Read(bExpLevel); // general experience level
		deserializeParameters->serializationBitstream[0].Read(bMarksmanship);
		deserializeParameters->serializationBitstream[0].Read(bExplosive);
		deserializeParameters->serializationBitstream[0].Read(bMechanical);
		deserializeParameters->serializationBitstream[0].Read(bMedical);
		deserializeParameters->serializationBitstream[0].Read(ubNeedForSleep);

		deserializeParameters->serializationBitstream[0].Read(sLifeGain);
		deserializeParameters->serializationBitstream[0].Read(bLifeDelta);
		deserializeParameters->serializationBitstream[0].Read(sAgilityGain);
		deserializeParameters->serializationBitstream[0].Read(bAgilityDelta);
		deserializeParameters->serializationBitstream[0].Read(sDexterityGain);
		deserializeParameters->serializationBitstream[0].Read(bDexterityDelta);
		deserializeParameters->serializationBitstream[0].Read(sStrengthGain);
		deserializeParameters->serializationBitstream[0].Read(bStrengthDelta);
		deserializeParameters->serializationBitstream[0].Read(sLeadershipGain);
		deserializeParameters->serializationBitstream[0].Read(bLeadershipDelta);
		deserializeParameters->serializationBitstream[0].Read(sWisdomGain);
		deserializeParameters->serializationBitstream[0].Read(bWisdomDelta);
		deserializeParameters->serializationBitstream[0].Read(sExpLevelGain);
		deserializeParameters->serializationBitstream[0].Read(bExpLevelDelta);
		deserializeParameters->serializationBitstream[0].Read(sMarksmanshipGain);
		deserializeParameters->serializationBitstream[0].Read(bMarksmanshipDelta);
		deserializeParameters->serializationBitstream[0].Read(sExplosivesGain);
		deserializeParameters->serializationBitstream[0].Read(bExplosivesDelta);
		deserializeParameters->serializationBitstream[0].Read(sMechanicGain);
		deserializeParameters->serializationBitstream[0].Read(bMechanicDelta);
		deserializeParameters->serializationBitstream[0].Read(sMedicalGain);
		deserializeParameters->serializationBitstream[0].Read(bMedicalDelta);

		deserializeParameters->serializationBitstream[0].Read(usStatChangeChances); // used strictly for balancing, never shown!
		deserializeParameters->serializationBitstream[0].Read(usStatChangeSuccesses); // used strictly for balancing, never shown!

		deserializeParameters->serializationBitstream[0].Read(bPersonalityTrait);
		deserializeParameters->serializationBitstream[0].Read(bSkillTrait);
		deserializeParameters->serializationBitstream[0].Read(bSkillTrait2);
		deserializeParameters->serializationBitstream[0].Read(bAttitude);
		deserializeParameters->serializationBitstream[0].Read(bSexist);

		/* Contract */
		deserializeParameters->serializationBitstream[0].Read(bMercStatus); //The status of the merc. If negative, see flags at the top of this file. Positive: The number of days the merc is away for. 0: Not hired but ready to be.
		deserializeParameters->serializationBitstream[0].Read(bReputationTolerance);
		deserializeParameters->serializationBitstream[0].Read(bDeathRate);
		deserializeParameters->serializationBitstream[0].Read(uiDayBecomesAvailable); //day the merc will be available. used with the bMercStatus
		deserializeParameters->serializationBitstream[0].Read(sSalary);
		deserializeParameters->serializationBitstream[0].Read(uiWeeklySalary);
		deserializeParameters->serializationBitstream[0].Read(uiBiWeeklySalary);
		deserializeParameters->serializationBitstream[0].Read(bMedicalDeposit); // Is medical deposit required?
		deserializeParameters->serializationBitstream[0].Read(sMedicalDepositAmount);
		deserializeParameters->serializationBitstream[0].Read(iMercMercContractLength); //Used for MERC mercs, specifies how many days the merc has gone since last page
		deserializeParameters->serializationBitstream[0].Read(usOptionalGearCost);
		deserializeParameters->serializationBitstream[0].Read(ubSuspiciousDeath);
		deserializeParameters->serializationBitstream[0].Read(ubDaysOfMoraleHangover); // used only when merc leaves team while having poor morale
		/* Locations */
		deserializeParameters->serializationBitstream[0].Read(sSector);
		deserializeParameters->serializationBitstream[0].Read(sGridNo); // The Gridno the NPC was in before leaving the sector
		deserializeParameters->serializationBitstream[0].Read(sPreCombatGridNo);
		deserializeParameters->serializationBitstream[0].Read(ubStrategicInsertionCode);
		deserializeParameters->serializationBitstream[0].Read(usStrategicInsertionData);
		deserializeParameters->serializationBitstream[0].Read(fUseProfileInsertionInfo); // Set to various flags, ( contained in TacticalSave.h )
		deserializeParameters->serializationBitstream[0].Read(bTown);
		deserializeParameters->serializationBitstream[0].Read(bTownAttachment);
		deserializeParameters->serializationBitstream[0].Read(ubRoomRangeStart[2]);
		deserializeParameters->serializationBitstream[0].Read(ubRoomRangeEnd[2]);

		deserializeParameters->serializationBitstream[0].Read(bBuddy); // Only indices 0, 1, 2 are used. Contain id's for friend1, friend2 and eventual friend respectively
		deserializeParameters->serializationBitstream[0].Read(bHated); // Only indices 0, 1, 2 are used. Contain id's for enemy1, enemy2 and eventual enemy respectively
		deserializeParameters->serializationBitstream[0].Read(bHatedCount); // Only indices 0, 1, 2 are used. Contain remaining decrements till contract termination due to an enemy present on the team
		deserializeParameters->serializationBitstream[0].Read(bHatedTime); // Only indices 0, 1, 2 are used. Contain decrements till contract termination due to an enemy present on the team
		deserializeParameters->serializationBitstream[0].Read(bLearnToLike); // eventual friend's id
		deserializeParameters->serializationBitstream[0].Read(bLearnToLikeCount); // remaining decrements till the eventual friend becomes an actual friend
		deserializeParameters->serializationBitstream[0].Read(bLearnToLikeTime); // how many decrements till the eventual friend becomes an actual friend
		deserializeParameters->serializationBitstream[0].Read(bLearnToHate); // eventual enemy's id
		deserializeParameters->serializationBitstream[0].Read(bLearnToHateCount); // remaining decrements till the eventual enemy becomes an actual enemy
		deserializeParameters->serializationBitstream[0].Read(bLearnToHateTime); // how many decrements till the eventual enemy becomes an actual enemy
		// Flags used for the precedent to repeating oneself in Contract negotiations. Used for quote 80 - ~107. Gets reset every day
		deserializeParameters->serializationBitstream[0].Read(ubTimeTillNextHatedComplaint);

		deserializeParameters->serializationBitstream[0].Read(bMercOpinion);

		deserializeParameters->serializationBitstream[0].Read(inv);
		deserializeParameters->serializationBitstream[0].Read(bInvNumber);
		deserializeParameters->serializationBitstream[0].Read(bInvStatus);
		deserializeParameters->serializationBitstream[0].Read(ubInvUndroppable);
		deserializeParameters->serializationBitstream[0].Read(uiMoney);
		deserializeParameters->serializationBitstream[0].Read(bArmourAttractiveness);
		deserializeParameters->serializationBitstream[0].Read(bMainGunAttractiveness);

		deserializeParameters->serializationBitstream[0].Read(iBalance); // if negative the player owes money to this NPC (e.g. for Skyrider's services)
		deserializeParameters->serializationBitstream[0].Read(ubNumTimesDrugUseInLifetime); // The # times a drug has been used in the player's lifetime...
		/* Specific quest or script related */
		deserializeParameters->serializationBitstream[0].Read(bNPCData); // NPC specific
		deserializeParameters->serializationBitstream[0].Read(bNPCData2); // NPC specific
		/* Dialogue and script records */
		deserializeParameters->serializationBitstream[0].Read(ubQuoteRecord);
		deserializeParameters->serializationBitstream[0].Read(ubLastQuoteSaid);
		deserializeParameters->serializationBitstream[0].Read(uiPrecedentQuoteSaid);
		deserializeParameters->serializationBitstream[0].Read(bLastQuoteSaidWasSpecial);
		deserializeParameters->serializationBitstream[0].Read(ubLastDateSpokenTo);
		deserializeParameters->serializationBitstream[0].Read(ubQuoteActionID);
		deserializeParameters->serializationBitstream[0].Read(bFriendlyOrDirectDefaultResponseUsedRecently);
		deserializeParameters->serializationBitstream[0].Read(bRecruitDefaultResponseUsedRecently);
		deserializeParameters->serializationBitstream[0].Read(bThreatenDefaultResponseUsedRecently);

		deserializeParameters->serializationBitstream[0].Read(bApproached);
		deserializeParameters->serializationBitstream[0].Read(usApproachFactor);
		deserializeParameters->serializationBitstream[0].Read(ubApproachVal);
		deserializeParameters->serializationBitstream[0].Read(ubApproachMod);
		/* Statistics */
		deserializeParameters->serializationBitstream[0].Read(usKills);
		deserializeParameters->serializationBitstream[0].Read(usAssists);
		deserializeParameters->serializationBitstream[0].Read(usShotsFired);
		deserializeParameters->serializationBitstream[0].Read(usShotsHit);
		deserializeParameters->serializationBitstream[0].Read(usBattlesFought);
		deserializeParameters->serializationBitstream[0].Read(usTimesWounded);
		deserializeParameters->serializationBitstream[0].Read(usTotalDaysServed);
		deserializeParameters->serializationBitstream[0].Read(uiTotalCostToDate); // The total amount of money that has been paid to the merc for their salary

		deserializeParameters->serializationBitstream[0].Read(bSectorZ); // unused
		deserializeParameters->serializationBitstream[0].Read(bRace); // unused
		deserializeParameters->serializationBitstream[0].Read(bRacist); // unused
		deserializeParameters->serializationBitstream[0].Read(bNationality); // unused
		deserializeParameters->serializationBitstream[0].Read(bAppearance); // unused
		deserializeParameters->serializationBitstream[0].Read(bAppearanceCareLevel); // unused
		deserializeParameters->serializationBitstream[0].Read(bRefinement); // unused
		deserializeParameters->serializationBitstream[0].Read(bRefinementCareLevel); // unused
		deserializeParameters->serializationBitstream[0].Read(bHatedNationality); // unused
		deserializeParameters->serializationBitstream[0].Read(bHatedNationalityCareLevel); // unused

		PostDeserialize();
	}

	void PreSerialize() {
		rzName = zName.c_str();
		rzNickname = zNickname.c_str();
		rPANTS = PANTS.c_str();
		rVEST = VEST.c_str();
		rSKIN = SKIN.c_str();
		rHAIR = HAIR.c_str();
	}

	void PostDeserialize() {
		zName = rzName;
		zNickname = rzNickname;
		PANTS = rPANTS;
		VEST = rVEST;
		SKIN = rSKIN;
		HAIR = rHAIR;
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

	virtual void OnPoppedConnection(RakNet::Connection_RM3* droppedConnection) {
	}

	void NotifyReplicaOfMessageDeliveryStatus(RakNetGUID guid, uint32_t receiptId, bool messageArrived) {
	}

	virtual RM3ConstructionState QueryConstruction(RakNet::Connection_RM3* destinationConnection, ReplicaManager3* replicaManager3) {
		return QueryConstruction_ServerConstruction(destinationConnection, gGameOptions.fNetwork == 0);
	}

	virtual bool QueryRemoteConstruction(RakNet::Connection_RM3* sourceConnection) {
		return QueryRemoteConstruction_ServerConstruction(sourceConnection, gGameOptions.fNetwork == 0);
	}

	virtual RM3QuerySerializationResult QuerySerialization(RakNet::Connection_RM3* destinationConnection) {
		return QuerySerialization_ServerSerializable(destinationConnection, gGameOptions.fNetwork == 0);
	}

	virtual RM3ActionOnPopConnection QueryActionOnPopConnection(RakNet::Connection_RM3* droppedConnection) const {
		return QueryActionOnPopConnection_Server(droppedConnection);
	}

	ST::string zName;
	ST::string zNickname;
	RakString rzName;
	RakString rzNickname;
	INT8 bSex;
	UINT8 ubCivilianGroup;

	UINT8 ubMiscFlags;
	UINT8 ubMiscFlags2;
	UINT8 ubMiscFlags3;

	UINT8 ubBodyType;
	UINT32 uiBodyTypeSubFlags; // BODY TYPE SUBSITUTIONS
	/* Portrait */
	UINT8 ubFaceIndex; // overwritten with the char's ID on profile load
	UINT16 usEyesX;
	UINT16 usEyesY;
	UINT16 usMouthX;
	UINT16 usMouthY;
	UINT32 uiBlinkFrequency{ 3000 };
	UINT32 uiExpressionFrequency{ 2000 };

	ST::string PANTS;
	ST::string VEST;
	ST::string SKIN;
	ST::string HAIR;
	RakString rPANTS;
	RakString rVEST;
	RakString rSKIN;
	RakString rHAIR;
	/* stats */
	INT8 bEvolution;

	INT8 bLifeMax{ 15 };
	INT8 bLife{ 15 };
	INT8 bAgility{ 1 }; // agility (speed) value
	INT8 bDexterity{ 1 }; // dexterity (hand coord) value
	INT8 bStrength{ 1 };
	INT8 bLeadership{ 1 };
	INT8 bWisdom{ 1 };
	INT8 bExpLevel{ 1 }; // general experience level
	INT8 bMarksmanship{ 0 };
	INT8 bExplosive{ 0 };
	INT8 bMechanical{ 0 };
	INT8 bMedical{ 0 };
	UINT8 ubNeedForSleep{ 7 };

	INT16 sLifeGain;
	INT8 bLifeDelta;
	INT16 sAgilityGain;
	INT8 bAgilityDelta;
	INT16 sDexterityGain;
	INT8 bDexterityDelta;
	INT16 sStrengthGain;
	INT8 bStrengthDelta;
	INT16 sLeadershipGain;
	INT8 bLeadershipDelta;
	INT16 sWisdomGain;
	INT8 bWisdomDelta;
	INT16 sExpLevelGain;
	INT8 bExpLevelDelta;
	INT16 sMarksmanshipGain;
	INT8 bMarksmanshipDelta;
	INT16 sExplosivesGain;
	INT8 bExplosivesDelta;
	INT16 sMechanicGain;
	INT8 bMechanicDelta;
	INT16 sMedicalGain;
	INT8 bMedicalDelta;

	UINT16 usStatChangeChances[12]; // used strictly for balancing, never shown!
	UINT16 usStatChangeSuccesses[12]; // used strictly for balancing, never shown!

	INT8 bPersonalityTrait;
	INT8 bSkillTrait;
	INT8 bSkillTrait2;
	INT8 bAttitude;
	UINT8 bSexist;

	/* Contract */
	INT8 bMercStatus; //The status of the merc. If negative, see flags at the top of this file. Positive: The number of days the merc is away for. 0: Not hired but ready to be.
	INT8 bReputationTolerance;
	INT8 bDeathRate;
	UINT32 uiDayBecomesAvailable; //day the merc will be available. used with the bMercStatus
	INT16 sSalary;
	UINT32 uiWeeklySalary;
	UINT32 uiBiWeeklySalary;
	INT8 bMedicalDeposit; // Is medical deposit required?
	UINT16 sMedicalDepositAmount;
	INT32 iMercMercContractLength; //Used for MERC mercs, specifies how many days the merc has gone since last page
	UINT16 usOptionalGearCost;
	UINT8 ubSuspiciousDeath;
	UINT8 ubDaysOfMoraleHangover; // used only when merc leaves team while having poor morale
	/* Locations */
	SGPSector sSector;
	INT16 sGridNo; // The Gridno the NPC was in before leaving the sector
	INT16 sPreCombatGridNo;
	UINT8 ubStrategicInsertionCode;
	UINT16 usStrategicInsertionData;
	BOOLEAN fUseProfileInsertionInfo; // Set to various flags, ( contained in TacticalSave.h )
	INT8 bTown;
	INT8 bTownAttachment;
	UINT8 ubRoomRangeStart[2];
	UINT8 ubRoomRangeEnd[2];

	INT8 bBuddy[5]{ -1, -1, -1, -1, -1 }; // Only indices 0, 1, 2 are used. Contain id's for friend1, friend2 and eventual friend respectively
	INT8 bHated[5]{ -1, -1, -1, -1, -1 }; // Only indices 0, 1, 2 are used. Contain id's for enemy1, enemy2 and eventual enemy respectively
	INT8 bHatedCount[5]; // Only indices 0, 1, 2 are used. Contain remaining decrements till contract termination due to an enemy present on the team
	INT8 bHatedTime[5]; // Only indices 0, 1, 2 are used. Contain decrements till contract termination due to an enemy present on the team
	INT8 bLearnToLike{ -1 }; // eventual friend's id
	INT8 bLearnToLikeCount; // remaining decrements till the eventual friend becomes an actual friend
	INT8 bLearnToLikeTime; // how many decrements till the eventual friend becomes an actual friend
	INT8 bLearnToHate{ -1 }; // eventual enemy's id
	INT8 bLearnToHateCount; // remaining decrements till the eventual enemy becomes an actual enemy
	INT8 bLearnToHateTime; // how many decrements till the eventual enemy becomes an actual enemy
	// Flags used for the precedent to repeating oneself in Contract negotiations. Used for quote 80 - ~107. Gets reset every day
	UINT8 ubTimeTillNextHatedComplaint;

	INT8 bMercOpinion[75];

	UINT16 inv[19];
	UINT8 bInvNumber[19];
	UINT8 bInvStatus[19];
	UINT8 ubInvUndroppable;
	UINT32 uiMoney;
	INT8 bArmourAttractiveness;
	INT8 bMainGunAttractiveness;

	INT32 iBalance; // if negative the player owes money to this NPC (e.g. for Skyrider's services)
	UINT8 ubNumTimesDrugUseInLifetime; // The # times a drug has been used in the player's lifetime...
	/* Specific quest or script related */
	INT8 bNPCData; // NPC specific
	INT8 bNPCData2; // NPC specific
	/* Dialogue and script records */
	UINT8 ubQuoteRecord;
	UINT8 ubLastQuoteSaid;
	UINT32 uiPrecedentQuoteSaid;
	UINT8 bLastQuoteSaidWasSpecial;
	UINT8 ubLastDateSpokenTo;
	UINT8 ubQuoteActionID;
	INT8 bFriendlyOrDirectDefaultResponseUsedRecently;
	INT8 bRecruitDefaultResponseUsedRecently;
	INT8 bThreatenDefaultResponseUsedRecently;

	INT8 bApproached;
	UINT16 usApproachFactor[4];
	UINT8 ubApproachVal[4];
	UINT8 ubApproachMod[3][4];
	/* Statistics */
	UINT16 usKills;
	UINT16 usAssists;
	UINT16 usShotsFired;
	UINT16 usShotsHit;
	UINT16 usBattlesFought;
	UINT16 usTimesWounded;
	UINT16 usTotalDaysServed;
	UINT32 uiTotalCostToDate; // The total amount of money that has been paid to the merc for their salary

	INT8 bSectorZ; // unused
	INT8 bRace; // unused
	INT8 bRacist; // unused
	INT8 bNationality; // unused
	INT8 bAppearance; // unused
	INT8 bAppearanceCareLevel; // unused
	INT8 bRefinement; // unused
	INT8 bRefinementCareLevel; // unused
	INT8 bHatedNationality; // unused
	INT8 bHatedNationalityCareLevel; // unused
};


static inline bool HasSkillTrait(MERCPROFILESTRUCT const& p, SkillTrait const skill)
{
	return p.bSkillTrait == skill || p.bSkillTrait2 == skill;
}


#define TIME_BETWEEN_HATED_COMPLAINTS				24

#define SUSPICIOUS_DEATH					1
#define VERY_SUSPICIOUS_DEATH					2

#endif
