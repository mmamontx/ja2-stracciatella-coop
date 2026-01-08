#ifndef __SOLDER_CONTROL_H
#define __SOLDER_CONTROL_H


//Kris:  November 10, 1997
//Please don't change this value from 10.  It will invalidate all of the maps and soldiers.
#define MAXPATROLGRIDS  10  // *** THIS IS A DUPLICATION - MUST BE MOVED !

#include "Animation_Cache.h"
#include "Animation_Control.h"
#include "Animation_Data.h"
#include "Coop.h"
#include "Faces.h"
#include "GameSettings.h"
#include "JA2Types.h"
#include "Keys.h"
#include "Overhead_Types.h"
#include "Player.h"
#include "Random.h"
#include "Shading.h"
#include "Item_Types.h"
#include "Timer_Control.h"

#include <string_theory/string>
#include <optional>


// ANDREW: these are defines for OKDestanation usage - please move to approprite file
#define IGNOREPEOPLE					0
#define PEOPLETOO					1
#define ALLPEOPLE					2
#define FALLINGTEST					3

#define LOCKED_NO_NEWGRIDNO				2

constexpr ProfileID NO_PROFILE = 200;

constexpr bool BATTLE_SND_LOWER_VOLUME = true;

#define TAKE_DAMAGE_GUNFIRE				1
#define TAKE_DAMAGE_BLADE				2
#define TAKE_DAMAGE_HANDTOHAND				3
#define TAKE_DAMAGE_FALLROOF				4
#define TAKE_DAMAGE_BLOODLOSS				5
#define TAKE_DAMAGE_EXPLOSION				6
#define TAKE_DAMAGE_ELECTRICITY			7
#define TAKE_DAMAGE_GAS				8
#define TAKE_DAMAGE_TENTACLES				9
#define TAKE_DAMAGE_STRUCTURE_EXPLOSION		10
#define TAKE_DAMAGE_OBJECT				11


#define SOLDIER_MULTI_SELECTED				0x00000004
#define SOLDIER_PC					0x00000008
#define SOLDIER_ATTACK_NOTICED				0x00000010
#define SOLDIER_PCUNDERAICONTROL			0x00000020
#define SOLDIER_UNDERAICONTROL				0x00000040
#define SOLDIER_DEAD					0x00000080
#define SOLDIER_LOOKFOR_ITEMS				0x00000200
#define SOLDIER_ENEMY					0x00000400
#define SOLDIER_ENGAGEDINACTION			0x00000800
#define SOLDIER_ROBOT					0x00001000
#define SOLDIER_MONSTER				0x00002000
#define SOLDIER_ANIMAL					0x00004000
#define SOLDIER_VEHICLE				0x00008000
#define SOLDIER_MULTITILE				0x00020000
#define SOLDIER_TURNINGFROMHIT				0x00080000
#define SOLDIER_BOXER					0x00100000
#define SOLDIER_LOCKPENDINGACTIONCOUNTER		0x00200000
#define SOLDIER_COWERING				0x00400000
#define SOLDIER_MUTE					0x00800000
#define SOLDIER_GASSED					0x01000000
#define SOLDIER_OFF_MAP				0x02000000
#define SOLDIER_PAUSEANIMOVE				0x04000000
#define SOLDIER_DRIVER					0x08000000
#define SOLDIER_PASSENGER				0x10000000
#define SOLDIER_NPC_DOING_PUNCH			0x20000000
#define SOLDIER_NPC_SHOOTING				0x40000000
#define SOLDIER_LOOK_NEXT_TURNSOLDIER			0x80000000


/*
#define SOLDIER_TRAIT_LOCKPICKING			0x0001
#define SOLDIER_TRAIT_HANDTOHAND			0x0002
#define SOLDIER_TRAIT_ELECTRONICS			0x0004
#define SOLDIER_TRAIT_NIGHTOPS				0x0008
#define SOLDIER_TRAIT_THROWING				0x0010
#define SOLDIER_TRAIT_TEACHING				0x0020
#define SOLDIER_TRAIT_HEAVY_WEAPS			0x0040
#define SOLDIER_TRAIT_AUTO_WEAPS			0x0080
#define SOLDIER_TRAIT_STEALTHY				0x0100
#define SOLDIER_TRAIT_AMBIDEXT				0x0200
#define SOLDIER_TRAIT_THIEF				0x0400
#define SOLDIER_TRAIT_MARTIALARTS			0x0800
#define SOLDIER_TRAIT_KNIFING				0x1000
*/
#define HAS_SKILL_TRAIT(s, t)				((s)->ubSkillTrait1 == (t) || (s)->ubSkillTrait2 == (t))
#define NUM_SKILL_TRAITS(s, t)				(((s)->ubSkillTrait1 == (t)) + ((s)->ubSkillTrait2 == (t)))

#define SOLDIER_QUOTE_SAID_IN_SHIT			0x0001
#define SOLDIER_QUOTE_SAID_LOW_BREATH			0x0002
#define SOLDIER_QUOTE_SAID_BEING_PUMMELED		0x0004
#define SOLDIER_QUOTE_SAID_NEED_SLEEP			0x0008
#define SOLDIER_QUOTE_SAID_LOW_MORAL			0x0010
#define SOLDIER_QUOTE_SAID_MULTIPLE_CREATURES		0x0020
#define SOLDIER_QUOTE_SAID_ANNOYING_MERC		0x0040
#define SOLDIER_QUOTE_SAID_LIKESGUN			0x0080
#define SOLDIER_QUOTE_SAID_DROWNING			0x0100
#define SOLDIER_QUOTE_SAID_SPOTTING_CREATURE_ATTACK	0x0400
#define SOLDIER_QUOTE_SAID_SMELLED_CREATURE		0x0800
#define SOLDIER_QUOTE_SAID_ANTICIPATING_DANGER		0x1000
#define SOLDIER_QUOTE_SAID_WORRIED_ABOUT_CREATURES	0x2000
#define SOLDIER_QUOTE_SAID_PERSONALITY			0x4000
#define SOLDIER_QUOTE_SAID_FOUND_SOMETHING_NICE	0x8000

#define SOLDIER_QUOTE_SAID_EXT_HEARD_SOMETHING	0x0001
#define SOLDIER_QUOTE_SAID_EXT_SEEN_CREATURE_ATTACK	0x0002
#define SOLDIER_QUOTE_SAID_EXT_USED_BATTLESOUND_HIT	0x0004
#define SOLDIER_QUOTE_SAID_EXT_CLOSE_CALL		0x0008
#define SOLDIER_QUOTE_SAID_EXT_MIKE			0x0010
#define SOLDIER_QUOTE_SAID_DONE_ASSIGNMENT		0x0020
#define SOLDIER_QUOTE_SAID_BUDDY_1_WITNESSED		0x0040
#define SOLDIER_QUOTE_SAID_BUDDY_2_WITNESSED		0x0080
#define SOLDIER_QUOTE_SAID_BUDDY_3_WITNESSED		0x0100


#define SOLDIER_CONTRACT_RENEW_QUOTE_NOT_USED		0
#define SOLDIER_CONTRACT_RENEW_QUOTE_89_USED		1
#define SOLDIER_CONTRACT_RENEW_QUOTE_115_USED		2


#define SOLDIER_MISC_HEARD_GUNSHOT			0x01
// make sure soldiers (esp tanks) are not hurt multiple times by explosions
#define SOLDIER_MISC_HURT_BY_EXPLOSION			0x02
// should be revealed due to xrays
#define SOLDIER_MISC_XRAYED				0x04

#define NOBLOOD					40
#define MIN_BLEEDING_THRESHOLD				12 // you're OK while <4 Yellow life bars

#define BANDAGED( s )					(s->bLifeMax - s->bLife - s->bBleeding)

// amount of time a stats is to be displayed differently, due to change
#define CHANGE_STAT_RECENTLY_DURATION			60000


#define NO_PENDING_ACTION				255
#define NO_PENDING_ANIMATION				32001
#define NO_PENDING_DIRECTION				253
#define NO_PENDING_STANCE				254
#define NO_DESIRED_HEIGHT				255


//ENUMERATIONS FOR ACTIONS
enum
{
	MERC_OPENDOOR,
	MERC_OPENSTRUCT,
	MERC_PICKUPITEM,
	MERC_PUNCH,
	MERC_KNIFEATTACK,
	MERC_GIVEAID,
	MERC_GIVEITEM,
	MERC_WAITFOROTHERSTOTRIGGER,
	MERC_CUTFFENCE,
	MERC_DROPBOMB,
	MERC_STEAL,
	MERC_TALK,
	MERC_ENTER_VEHICLE,
	MERC_REPAIR,
	MERC_RELOADROBOT,
	MERC_TAKEBLOOD,
	MERC_ATTACH_CAN,
	MERC_FUEL_VEHICLE,
};

// ENUMERATIONS FOR THROW ACTIONS
enum
{
	NO_THROW_ACTION,
	THROW_ARM_ITEM,
	THROW_TARGET_MERC_CATCH,
};

// An enumeration for playing battle sounds
enum BattleSound : INT8
{
	BATTLE_SOUND_OK1,
	BATTLE_SOUND_OK2,
	BATTLE_SOUND_COOL1,
	BATTLE_SOUND_CURSE1,
	BATTLE_SOUND_HIT1,
	BATTLE_SOUND_HIT2,
	BATTLE_SOUND_LAUGH1,
	BATTLE_SOUND_ATTN1,
	BATTLE_SOUND_DIE1,
	BATTLE_SOUND_HUMM,
	BATTLE_SOUND_NOTHING,
	BATTLE_SOUND_GOTIT,
	BATTLE_SOUND_LOWMORALE_OK1,
	BATTLE_SOUND_LOWMORALE_OK2,
	BATTLE_SOUND_LOWMORALE_ATTN1,
	BATTLE_SOUND_LOCKED,
	BATTLE_SOUND_ENEMY,
	NUM_MERC_BATTLE_SOUNDS
};


//different kinds of merc
enum
{
	MERC_TYPE__PLAYER_CHARACTER,
	MERC_TYPE__AIM_MERC,
	MERC_TYPE__MERC,
	MERC_TYPE__NPC,
	MERC_TYPE__EPC,
	MERC_TYPE__NPC_WITH_UNEXTENDABLE_CONTRACT,
	MERC_TYPE__VEHICLE,
};

// I don't care if this isn't intuitive!  The hand positions go right
// before the big pockets so we can loop through them that way. --CJC
#define NO_SLOT -1

// vehicle/human path structure
struct PathSt
{
	UINT32 uiSectorId;
	PathSt* pNext;
	PathSt* pPrev;
};


enum InvSlotPos
{
	HELMETPOS = 0,
	VESTPOS,
	LEGPOS,
	HEAD1POS,
	HEAD2POS,
	HANDPOS,
	SECONDHANDPOS,
	BIGPOCK1POS,
	BIGPOCK2POS,
	BIGPOCK3POS,
	BIGPOCK4POS,
	SMALLPOCK1POS,
	SMALLPOCK2POS,
	SMALLPOCK3POS,
	SMALLPOCK4POS,
	SMALLPOCK5POS,
	SMALLPOCK6POS,
	SMALLPOCK7POS,
	SMALLPOCK8POS, // = 18, so 19 pockets needed

	NUM_INV_SLOTS,
};

extern UINT8 gubItemDroppableFlag[NUM_INV_SLOTS];

//used for color codes, but also shows the enemy type for debugging purposes
enum SoldierClass
{
	SOLDIER_CLASS_NONE,
	SOLDIER_CLASS_ADMINISTRATOR,
	SOLDIER_CLASS_ELITE,
	SOLDIER_CLASS_ARMY,
	SOLDIER_CLASS_GREEN_MILITIA,
	SOLDIER_CLASS_REG_MILITIA,
	SOLDIER_CLASS_ELITE_MILITIA,
	SOLDIER_CLASS_CREATURE,
	SOLDIER_CLASS_MINER,
};

#define SOLDIER_CLASS_ENEMY( bSoldierClass )		( ( bSoldierClass >= SOLDIER_CLASS_ADMINISTRATOR ) && ( bSoldierClass <= SOLDIER_CLASS_ARMY ) )
#define SOLDIER_CLASS_MILITIA( bSoldierClass )		( ( bSoldierClass >= SOLDIER_CLASS_GREEN_MILITIA ) && ( bSoldierClass <= SOLDIER_CLASS_ELITE_MILITIA ) )

// This macro should be used whenever we want to see if someone is neutral
// IF WE ARE CONSIDERING ATTACKING THEM.  Creatures & bloodcats will attack neutrals
// but they can't attack empty vehicles!!
#define CONSIDERED_NEUTRAL(me, them)			((them)->bNeutral && ((me)->bTeam != CREATURE_TEAM || (them)->uiStatusFlags & SOLDIER_VEHICLE))

struct KEY_ON_RING
{
	UINT8 ubKeyID{INVALID_KEY_NUMBER};
	UINT8 ubNumber{0};

	bool isValid() const { return ubKeyID != INVALID_KEY_NUMBER && ubNumber != 0; }
};


struct THROW_PARAMS
{
	float dX;
	float dY;
	float dZ;
	float dForceX;
	float dForceY;
	float dForceZ;
	float dLifeSpan;
	UINT8 ubActionCode;
	SOLDIERTYPE *target;
};

#define DELAYED_MOVEMENT_FLAG_PATH_THROUGH_PEOPLE	0x01

// reasons for being unable to continue movement
enum
{
	REASON_STOPPED_NO_APS,
	REASON_STOPPED_SIGHT,
};


enum
{
	HIT_BY_TEARGAS = 0x01,
	HIT_BY_MUSTARDGAS = 0x02,
	HIT_BY_CREATUREGAS = 0x04,
};


enum WeaponModes : INT8
{
	WM_NORMAL = 0,
	WM_BURST,
	WM_ATTACHED,
	NUM_WEAPON_MODES
};

using SoldierID = UINT8;

#define SOLDIERTYPE_NAME_LENGTH 10


// The following declarations are required so that these functions can be used inside the class methods
extern SoldierID Soldier2ID(const SOLDIERTYPE* const s);
extern SOLDIERTYPE* ID2Soldier(const SoldierID id);

extern void EVENT_InitNewSoldierAnim(SOLDIERTYPE*, UINT16 new_state, UINT16 starting_ani_code, BOOLEAN force);
extern void CreateSoldierPalettes(SOLDIERTYPE&);

struct SOLDIERTYPE : public Replica3
{
	SOLDIERTYPE()
	{
		ubID = 0;

		ubBodyType = 0;
		bActionPoints = 0;
		bInitialActionPoints = 0;

		uiStatusFlags = 0;

		//memset(inv, 0, sizeof(inv)); // Class object has its own constructor
		pTempObject = NULL;
		pKeyRing = NULL;

		bOldLife = 0;

		bInSector = 0;
		bFlashPortraitFrame = 0;
		sFractLife = 0;
		bBleeding = 0;
		bBreath = 0;
		bBreathMax = 0;
		bStealthMode = 0;

		sBreathRed = 0;
		fDelayedMovement = 0;

		ubWaitActionToDo = 0;
		ubInsertionDirection = 0;

		opponent = NULL;
		bLastRenderVisibleValue = 0;
		ubAttackingHand = 0;

		sWeightCarriedAtTurnStart = 0;

		bVisible = 0;

		bActive = 0;

		bTeam = 0;

		ubGroupID = 0;
		fBetweenSectors = 0;

		ubMovementNoiseHeard = 0;

		dXPos = 0;
		dYPos = 0;
		sInitialGridNo = 0;
		sGridNo = 0;
		bDirection = 0;
		sHeightAdjustment = 0;
		sDesiredHeight = 0;
		sTempNewGridNo = 0;
		bOverTerrainType = 0;

		bCollapsed = 0;
		bBreathCollapsed = 0;

		ubDesiredHeight = 0;
		usPendingAnimation = 0;
		ubPendingStanceChange = 0;
		usAnimState = 0;
		fNoAPToFinishMove = 0;
		fPausedMove = 0;
		fUIdeadMerc = 0;
		fUICloseMerc = 0;

		ubSkillTrait1 = 0;
		ubSkillTrait2 = 0;

		bDexterity = 0;
		bWisdom = 0;
		attacker = NULL;
		previous_attacker = NULL;
		next_to_previous_attacker = NULL;
		fTurnInProgress = 0;

		fIntendedTarget = 0;
		fPauseAllAnimation = 0;

		bExpLevel = 0;
		sInsertionGridNo = 0;

		fContinueMoveAfterStanceChange = 0;

		AnimCache_initialized = FALSE;

		bLife = 0;
		bSide = (Side)0;
		bNewOppCnt = 0;

		usAniCode = 0;
		usAniFrame = 0;
		sAniDelay = 0;

		bAgility = 0;
		sDelayedMovementCauseGridNo = 0;
		sReservedMovementGridNo = 0;

		bStrength = 0;

		sTargetGridNo = 0;
		bTargetLevel = 0;
		bTargetCubeLevel = 0;
		sLastTarget = 0;
		bTilesMoved = 0;
		bLeadership = 0;
		dNextBleed = 0;
		fWarnedAboutBleeding = 0;
		fDyingComment = 0;

		ubTilesMovedPerRTBreathUpdate = 0;
		usLastMovementAnimPerRTBreathUpdate = 0;

		fTurningToShoot = 0;
		fTurningUntilDone = 0;
		fGettingHit = 0;
		fInNonintAnim = 0;
		fFlashLocator = 0;
		sLocatorFrame = 0;
		fShowLocator = 0;
		fFlashPortrait = 0;
		bMechanical = 0;
		bLifeMax = 0;

		face = NULL;
		face_initialized = FALSE;

		// Nullifying uint pointers
		memset(pShades, 0, sizeof(pShades));
		memset(pGlowShades, 0, sizeof(pGlowShades));
		bMedical = 0;
		fBeginFade = 0;
		ubFadeLevel = 0;
		ubServiceCount = 0;
		service_partner = NULL;
		bMarksmanship = 0;
		bExplosive = 0;
		pThrowParams = NULL;
		fTurningFromPronePosition = 0;
		bReverse = 0;
		pLevelNode = NULL;

		bDesiredDirection = 0;
		sDestXPos = 0;
		sDestYPos = 0;
		sDestination = 0;
		sFinalDestination = 0;
		bLevel = 0;

		memset(ubPathingData, 0, sizeof(ubPathingData)); // Nullifying uints
		ubPathDataSize = 0;
		ubPathIndex = 0;
		sBlackList = 0;
		bAimTime = 0;
		bShownAimTime = 0;
		bPathStored = 0;
		bHasKeys = 0;

		ubStrategicInsertionCode = 0;
		usStrategicInsertionData = 0;

		light = NULL;
		muzzle_flash = NULL;
		bMuzFlashCount = 0;

		sX = 0;
		sY = 0;

		usOldAniState = 0;
		sOldAniCode = 0;

		bBulletsLeft = 0;
		ubSuppressionPoints = 0;

		uiTimeOfLastRandomAction = 0;

		memset(bOppList, 0, sizeof(bOppList)); // Nullifying ints
		bLastAction = 0;
		bAction = 0;
		usActionData = 0;
		bNextAction = 0;
		usNextActionData = 0;
		bActionInProgress = 0;
		bAlertStatus = 0;
		bOppCnt = 0;
		bNeutral = 0;
		bNewSituation = 0;
		bNextTargetLevel = 0;
		bOrders = 0;
		bAttitude = 0;
		bUnderFire = 0;
		bShock = 0;
		bBypassToGreen = 0;
		bDominantDir = 0;
		bPatrolCnt = 0;
		bNextPatrolPnt = 0;
		memset(usPatrolGrid, 0, sizeof(usPatrolGrid)); // Nullifying ints
		sNoiseGridno = 0;
		ubNoiseVolume = 0;
		bLastAttackHit = 0;
		xrayed_by = NULL;
		dHeightAdjustment = 0;
		bMorale = 0;
		bTeamMoraleMod = 0;
		bTacticalMoraleMod = 0;
		bStrategicMoraleMod = 0;
		bAIMorale = 0;
		ubPendingAction = 0;
		ubPendingActionAnimCount = 0;
		uiPendingActionData1 = 0;
		sPendingActionData2 = 0;
		bPendingActionData3 = 0;
		ubDoorHandleCode = 0;
		uiPendingActionData4 = 0;
		bInterruptDuelPts = 0;
		bPassedLastInterrupt = 0;
		bIntStartAPs = 0;
		bMoved = 0;
		bHunting = 0;
		ubCaller = 0;
		sCallerGridNo = 0;
		bCallPriority = 0;
		bCallActedUpon = 0;
		bFrenzied = 0;
		bNormalSmell = 0;
		bMonsterSmell = 0;
		bMobility = 0;
		fAIFlags = 0;

		fDontChargeReadyAPs = 0;
		usAnimSurface = 0;
		sZLevel = 0;
		fPrevInWater = 0;
		fGoBackToAimAfterHit = 0;

		sWalkToAttackGridNo = 0;
		sWalkToAttackWalkToCost = 0;

		fForceShade = 0;
		pForcedShade = NULL;

		bDisplayDamageCount = 0;
		fDisplayDamage = 0;
		sDamage = 0;
		sDamageX = 0;
		sDamageY = 0;
		bDoBurst = 0;
		usUIMovementMode = 0;
		fUIMovementFast = 0;

		fDeadSoundPlayed = 0;
		ubProfile = 0;
		ubQuoteRecord = 0;
		ubQuoteActionID = 0;
		ubBattleSoundID = 0;

		fClosePanel = 0;
		fClosePanelToDie = 0;
		ubClosePanelFrame = 0;
		fDeadPanel = 0;
		ubDeadPanelFrame = 0;

		sPanelFaceX = 0;
		sPanelFaceY = 0;

		bNumHitsThisTurn = 0;
		usQuoteSaidFlags = 0;
		fCloseCall = 0;
		bLastSkillCheck = 0;
		ubSkillCheckAttempts = 0;

		bStartFallDir = 0;
		fTryingToFall = 0;

		ubPendingDirection = 0;
		uiAnimSubFlags = 0;

		bAimShotLocation = 0;
		ubHitLocation = 0;

		effect_shade = NULL;

		memset(sSpreadLocations, 0, sizeof(sSpreadLocations)); // Nullifying ints
		fDoSpread = 0;
		sStartGridNo = 0;
		sEndGridNo = 0;
		sForcastGridno = 0;
		sZLevelOverride = 0;
		bMovedPriorToInterrupt = 0;
		iEndofContractTime = 0;
		iStartContractTime = 0;
		iTotalContractLength = 0;
		iNextActionSpecialData = 0;
		ubWhatKindOfMercAmI = 0;
		bAssignment = 0;
		fForcedToStayAwake = 0;
		bTrainStat = 0;
		sSector = 0;
		iVehicleId = 0;
		pMercPath = NULL;
		fHitByGasFlags = 0;
		usMedicalDeposit = 0;
		usLifeInsurance = 0;

		iStartOfInsuranceContract = 0;
		uiLastAssignmentChangeMin = 0;
		iTotalLengthOfInsuranceContract = 0;

		ubSoldierClass = 0;
		ubAPsLostToSuppression = 0;
		fChangingStanceDueToSuppression = 0;
		suppressor = NULL;

		ubCivilianGroup = 0;

		uiChangeLevelTime = 0;
		uiChangeHealthTime = 0;
		uiChangeStrengthTime = 0;
		uiChangeDexterityTime = 0;
		uiChangeAgilityTime = 0;
		uiChangeWisdomTime = 0;
		uiChangeLeadershipTime = 0;
		uiChangeMarksmanshipTime = 0;
		uiChangeExplosivesTime = 0;
		uiChangeMedicalTime = 0;
		uiChangeMechanicalTime = 0;

		uiUniqueSoldierIdValue = 0;
		bBeingAttackedCount = 0;

		// Nullifying ints
		memset(bNewItemCount, 0, sizeof(bNewItemCount));
		memset(bNewItemCycleCount, 0, sizeof(bNewItemCycleCount));
		fCheckForNewlyAddedItems = 0;
		bEndDoorOpenCode = 0;

		ubScheduleID = 0;
		sEndDoorOpenCodeData = 0;
		fBlockedByAnotherMerc = 0;
		bBlockedByAnotherMercDirection = 0;
		usAttackingWeapon = 0;
		target = NULL;
		bWeaponMode = WM_NORMAL;
		bAIScheduleProgress = 0;
		sOffWorldGridNo = 0;
		pAniTile = NULL;
		bCamo = 0;
		sAbsoluteFinalDestination = 0;
		ubHiResDirection = 0;
		ubLastFootPrintSound = 0;
		bVehicleID = 0;
		fPastXDest = 0;
		fPastYDest = 0;
		bMovementDirection = 0;
		sOldGridNo = 0;
		usDontUpdateNewGridNoOnMoveAnimChange = 0;
		sBoundingBoxWidth = 0;
		sBoundingBoxHeight = 0;
		sBoundingBoxOffsetX = 0;
		sBoundingBoxOffsetY = 0;
		uiTimeSameBattleSndDone = 0;
		bOldBattleSnd = BATTLE_SOUND_OK1; // Random valid value: it is expected to be modified before use
		fContractPriceHasIncreased = 0;
		uiBurstSoundID = 0;
		fFixingSAMSite = 0;
		fFixingRobot = 0;
		bSlotItemTakenFrom = 0;
		fSignedAnotherContract = 0;
		fDontChargeTurningAPs = 0;
		auto_bandaging_medic = NULL;
		robot_remote_holder = NULL;
		uiTimeOfLastContractUpdate = 0;
		bTypeOfLastContract = 0;
		bTurnsCollapsed = 0;
		bSleepDrugCounter = 0;
		ubMilitiaKills = 0;

		// Nullifying ints
		memset(bFutureDrugEffect, 0, sizeof(bFutureDrugEffect));
		memset(bDrugEffectRate, 0, sizeof(bDrugEffectRate));
		memset(bDrugEffect, 0, sizeof(bDrugEffect));
		memset(bDrugSideEffectRate, 0, sizeof(bDrugSideEffectRate));
		memset(bDrugSideEffect, 0, sizeof(bDrugSideEffect));

		bBlindedCounter = 0;
		fMercCollapsedFlag = 0;
		fDoneAssignmentAndNothingToDoFlag = 0;
		fMercAsleep = 0;
		fDontChargeAPsForStanceChange = 0;

		ubTurnsUntilCanSayHeardNoise = 0;
		usQuoteSaidExtFlags = 0;

		sContPathLocation = 0;
		bGoodContPath = 0;
		bNoiseLevel = 0;
		bRegenerationCounter = 0;
		bRegenBoostersUsedToday = 0;
		bNumPelletsHitBy = 0;
		sSkillCheckGridNo = 0;
		ubLastEnemyCycledID = 0;

		ubPrevSectorID = 0;
		ubNumTilesMovesSinceLastForget = 0;
		bTurningIncrement = 0;
		uiBattleSoundID = 0;

		fSoldierWasMoving = 0;
		fSayAmmoQuotePending = 0;
		usValueGoneUp = 0;

		ubNumLocateCycles = 0;
		ubDelayedMovementFlags = 0;
		fMuzzleFlash = 0;
		CTGTTarget = NULL;

		bCurrentCivQuote = 0;
		bCurrentCivQuoteDelta = 0;
		ubMiscSoldierFlags = 0;
		ubReasonCantFinishMove = 0;

		sLocationOfFadeStart = 0;
		bUseExitGridForReentryDirection = 0;

		uiTimeSinceLastSpoke = 0;
		ubContractRenewalQuoteCode = 0;
		sPreTraversalGridNo = 0;
		uiXRayActivatedTime = 0;
		bTurningFromUI = 0;
		bPendingActionData5 = 0;

		bDelayedStrategicMoraleMod = 0;
		ubDoorOpeningNoise = 0;

		ubLeaveHistoryCode = 0;
		fDontUnsetLastTargetFromTurn = 0;
		bOverrideMoveSpeed = 0;
		fUseMoverrideMoveSpeed = 0;

		uiTimeSoldierWillArrive = 0;
		fUseLandingZoneForArrival = 0;
		fFallClockwise = 0;
		bVehicleUnderRepairID = 0;
		iTimeCanSignElsewhere = 0;
		bHospitalPriceModifier = 0;
		uiStartTimeOfInsuranceContract = 0;
		fRTInNonintAnim = 0;
		fDoingExternalDeath = 0;
		bCorpseQuoteTolerance = 0;
		iPositionSndID = 0;
		uiTuringSoundID = 0;
		ubLastDamageReason = 0;
		fComplainedThatTired = 0;
		memset(sLastTwoLocations, 0, sizeof(sLastTwoLocations)); // Nullifying ints
		uiTimeSinceLastBleedGrunt = 0;

		ubPlayer = 0;
	}

	SOLDIERTYPE& SOLDIERTYPE::operator =(const SOLDIERTYPE& other)
	{
		ubID = other.ubID;

		ubBodyType = other.ubBodyType;
		bActionPoints = other.bActionPoints;
		bInitialActionPoints = other.bInitialActionPoints;

		uiStatusFlags = other.uiStatusFlags;

		for (int i = 0; i < NUM_INV_SLOTS; i++)
			inv[i] = other.inv[i];
		pTempObject = other.pTempObject;
		pKeyRing = other.pKeyRing;

		bOldLife = other.bOldLife;

		bInSector = other.bInSector;
		bFlashPortraitFrame = other.bFlashPortraitFrame;
		sFractLife = other.sFractLife;
		bBleeding = other.bBleeding;
		bBreath = other.bBreath;
		bBreathMax = other.bBreathMax;
		bStealthMode = other.bStealthMode;

		sBreathRed = other.sBreathRed;
		fDelayedMovement = other.fDelayedMovement;

		ubWaitActionToDo = other.ubWaitActionToDo;
		ubInsertionDirection = other.ubInsertionDirection;

		opponent = other.opponent;
		bLastRenderVisibleValue = other.bLastRenderVisibleValue;
		ubAttackingHand = other.ubAttackingHand;

		sWeightCarriedAtTurnStart = other.sWeightCarriedAtTurnStart;
		name = other.name.c_str(); // Instead of direct assignment to avoid exception

		bVisible = other.bVisible;

		bActive = other.bActive;

		bTeam = other.bTeam;

		ubGroupID = other.ubGroupID;
		fBetweenSectors = other.fBetweenSectors;

		ubMovementNoiseHeard = other.ubMovementNoiseHeard;

		dXPos = other.dXPos;
		dYPos = other.dYPos;
		sInitialGridNo = other.sInitialGridNo;
		sGridNo = other.sGridNo;
		bDirection = other.bDirection;
		sHeightAdjustment = other.sHeightAdjustment;
		sDesiredHeight = other.sDesiredHeight;
		sTempNewGridNo = other.sTempNewGridNo;
		bOverTerrainType = other.bOverTerrainType;

		bCollapsed = other.bCollapsed;
		bBreathCollapsed = other.bBreathCollapsed;

		ubDesiredHeight = other.ubDesiredHeight;
		usPendingAnimation = other.usPendingAnimation;
		ubPendingStanceChange = other.ubPendingStanceChange;
		usAnimState = other.usAnimState;
		fNoAPToFinishMove = other.fNoAPToFinishMove;
		fPausedMove = other.fPausedMove;
		fUIdeadMerc = other.fUIdeadMerc;
		fUICloseMerc = other.fUICloseMerc;

		UpdateCounter = other.UpdateCounter;
		DamageCounter = other.DamageCounter;
		AICounter = other.AICounter;
		FadeCounter = other.FadeCounter;

		ubSkillTrait1 = other.ubSkillTrait1;
		ubSkillTrait2 = other.ubSkillTrait2;

		bDexterity = other.bDexterity;
		bWisdom = other.bWisdom;
		attacker = other.attacker;
		previous_attacker = other.previous_attacker;
		next_to_previous_attacker = other.next_to_previous_attacker;
		fTurnInProgress = other.fTurnInProgress;

		fIntendedTarget = other.fIntendedTarget;
		fPauseAllAnimation = other.fPauseAllAnimation;

		bExpLevel = other.bExpLevel;
		sInsertionGridNo = other.sInsertionGridNo;

		fContinueMoveAfterStanceChange = other.fContinueMoveAfterStanceChange;

		AnimCache = other.AnimCache;

		bLife = other.bLife;
		bSide = other.bSide;
		bNewOppCnt = other.bNewOppCnt;

		usAniCode = other.usAniCode;
		usAniFrame = other.usAniFrame;
		sAniDelay = other.sAniDelay;

		bAgility = other.bAgility;
		sDelayedMovementCauseGridNo = other.sDelayedMovementCauseGridNo;
		sReservedMovementGridNo = other.sReservedMovementGridNo;

		bStrength = other.bStrength;

		sTargetGridNo = other.sTargetGridNo;
		bTargetLevel = other.bTargetLevel;
		bTargetCubeLevel = other.bTargetCubeLevel;
		sLastTarget = other.sLastTarget;
		bTilesMoved = other.bTilesMoved;
		bLeadership = other.bLeadership;
		dNextBleed = other.dNextBleed;
		fWarnedAboutBleeding = other.fWarnedAboutBleeding;
		fDyingComment = other.fDyingComment;

		ubTilesMovedPerRTBreathUpdate = other.ubTilesMovedPerRTBreathUpdate;
		usLastMovementAnimPerRTBreathUpdate = other.usLastMovementAnimPerRTBreathUpdate;

		fTurningToShoot = other.fTurningToShoot;
		fTurningUntilDone = other.fTurningUntilDone;
		fGettingHit = other.fGettingHit;
		fInNonintAnim = other.fInNonintAnim;
		fFlashLocator = other.fFlashLocator;
		sLocatorFrame = other.sLocatorFrame;
		fShowLocator = other.fShowLocator;
		fFlashPortrait = other.fFlashPortrait;
		bMechanical = other.bMechanical;
		bLifeMax = other.bLifeMax;

		face = other.face;

		// Instead of direct assignment to avoid exception
		HeadPal = other.HeadPal.c_str();
		PantsPal = other.PantsPal.c_str();
		VestPal = other.VestPal.c_str();
		SkinPal = other.SkinPal.c_str();

		for (int i = 0; i < NUM_SOLDIER_SHADES; i++)
			pShades[i] = other.pShades[i];
		for (int i = 0; i < 20; i++)
			pGlowShades[i] = other.pGlowShades[i];
		bMedical = other.bMedical;
		fBeginFade = other.fBeginFade;
		ubFadeLevel = other.ubFadeLevel;
		ubServiceCount = other.ubServiceCount;
		service_partner = other.service_partner;
		bMarksmanship = other.bMarksmanship;
		bExplosive = other.bExplosive;
		pThrowParams = other.pThrowParams;
		fTurningFromPronePosition = other.fTurningFromPronePosition;
		bReverse = other.bReverse;
		pLevelNode = other.pLevelNode;

		bDesiredDirection = other.bDesiredDirection;
		sDestXPos = other.sDestXPos;
		sDestYPos = other.sDestYPos;
		sDestination = other.sDestination;
		sFinalDestination = other.sFinalDestination;
		bLevel = other.bLevel;

		for (int i = 0; i < MAX_PATH_LIST_SIZE; i++)
			ubPathingData[i] = other.ubPathingData[i];
		ubPathDataSize = other.ubPathDataSize;
		ubPathIndex = other.ubPathIndex;
		sBlackList = other.sBlackList;
		bAimTime = other.bAimTime;
		bShownAimTime = other.bShownAimTime;
		bPathStored = other.bPathStored;
		bHasKeys = other.bHasKeys;

		ubStrategicInsertionCode = other.ubStrategicInsertionCode;
		usStrategicInsertionData = other.usStrategicInsertionData;

		light = other.light;
		muzzle_flash = other.muzzle_flash;
		bMuzFlashCount = other.bMuzFlashCount;

		sX = other.sX;
		sY = other.sY;

		usOldAniState = other.usOldAniState;
		sOldAniCode = other.sOldAniCode;

		bBulletsLeft = other.bBulletsLeft;
		ubSuppressionPoints = other.ubSuppressionPoints;

		uiTimeOfLastRandomAction = other.uiTimeOfLastRandomAction;

		for (int i = 0; i < MAX_NUM_SOLDIERS; i++)
			bOppList[i] = other.bOppList[i];
		bLastAction = other.bLastAction;
		bAction = other.bAction;
		usActionData = other.usActionData;
		bNextAction = other.bNextAction;
		usNextActionData = other.usNextActionData;
		bActionInProgress = other.bActionInProgress;
		bAlertStatus = other.bAlertStatus;
		bOppCnt = other.bOppCnt;
		bNeutral = other.bNeutral;
		bNewSituation = other.bNewSituation;
		bNextTargetLevel = other.bNextTargetLevel;
		bOrders = other.bOrders;
		bAttitude = other.bAttitude;
		bUnderFire = other.bUnderFire;
		bShock = other.bShock;
		bBypassToGreen = other.bBypassToGreen;
		bDominantDir = other.bDominantDir;
		bPatrolCnt = other.bPatrolCnt;
		bNextPatrolPnt = other.bNextPatrolPnt;
		for (int i = 0; i < MAXPATROLGRIDS; i++)
			usPatrolGrid[i] = other.usPatrolGrid[i];
		sNoiseGridno = other.sNoiseGridno;
		ubNoiseVolume = other.ubNoiseVolume;
		bLastAttackHit = other.bLastAttackHit;
		xrayed_by = other.xrayed_by;
		dHeightAdjustment = other.dHeightAdjustment;
		bMorale = other.bMorale;
		bTeamMoraleMod = other.bTeamMoraleMod;
		bTacticalMoraleMod = other.bTacticalMoraleMod;
		bStrategicMoraleMod = other.bStrategicMoraleMod;
		bAIMorale = other.bAIMorale;
		ubPendingAction = other.ubPendingAction;
		ubPendingActionAnimCount = other.ubPendingActionAnimCount;
		uiPendingActionData1 = other.uiPendingActionData1;
		sPendingActionData2 = other.sPendingActionData2;
		bPendingActionData3 = other.bPendingActionData3;
		ubDoorHandleCode = other.ubDoorHandleCode;
		uiPendingActionData4 = other.uiPendingActionData4;
		bInterruptDuelPts = other.bInterruptDuelPts;
		bPassedLastInterrupt = other.bPassedLastInterrupt;
		bIntStartAPs = other.bIntStartAPs;
		bMoved = other.bMoved;
		bHunting = other.bHunting;
		ubCaller = other.ubCaller;
		sCallerGridNo = other.sCallerGridNo;
		bCallPriority = other.bCallPriority;
		bCallActedUpon = other.bCallActedUpon;
		bFrenzied = other.bFrenzied;
		bNormalSmell = other.bNormalSmell;
		bMonsterSmell = other.bMonsterSmell;
		bMobility = other.bMobility;
		fAIFlags = other.fAIFlags;

		fDontChargeReadyAPs = other.fDontChargeReadyAPs;
		usAnimSurface = other.usAnimSurface;
		sZLevel = other.sZLevel;
		fPrevInWater = other.fPrevInWater;
		fGoBackToAimAfterHit = other.fGoBackToAimAfterHit;

		sWalkToAttackGridNo = other.sWalkToAttackGridNo;
		sWalkToAttackWalkToCost = other.sWalkToAttackWalkToCost;

		fForceShade = other.fForceShade;
		pForcedShade = other.pForcedShade;

		bDisplayDamageCount = other.bDisplayDamageCount;
		fDisplayDamage = other.fDisplayDamage;
		sDamage = other.sDamage;
		sDamageX = other.sDamageX;
		sDamageY = other.sDamageY;
		bDoBurst = other.bDoBurst;
		usUIMovementMode = other.usUIMovementMode;
		fUIMovementFast = other.fUIMovementFast;

		BlinkSelCounter = other.BlinkSelCounter;
		PortraitFlashCounter = other.PortraitFlashCounter;
		fDeadSoundPlayed = other.fDeadSoundPlayed;
		ubProfile = other.ubProfile;
		ubQuoteRecord = other.ubQuoteRecord;
		ubQuoteActionID = other.ubQuoteActionID;
		ubBattleSoundID = other.ubBattleSoundID;

		fClosePanel = other.fClosePanel;
		fClosePanelToDie = other.fClosePanelToDie;
		ubClosePanelFrame = other.ubClosePanelFrame;
		fDeadPanel = other.fDeadPanel;
		ubDeadPanelFrame = other.ubDeadPanelFrame;

		sPanelFaceX = other.sPanelFaceX;
		sPanelFaceY = other.sPanelFaceY;

		bNumHitsThisTurn = other.bNumHitsThisTurn;
		usQuoteSaidFlags = other.usQuoteSaidFlags;
		fCloseCall = other.fCloseCall;
		bLastSkillCheck = other.bLastSkillCheck;
		ubSkillCheckAttempts = other.ubSkillCheckAttempts;

		bStartFallDir = other.bStartFallDir;
		fTryingToFall = other.fTryingToFall;

		ubPendingDirection = other.ubPendingDirection;
		uiAnimSubFlags = other.uiAnimSubFlags;

		bAimShotLocation = other.bAimShotLocation;
		ubHitLocation = other.ubHitLocation;

		effect_shade = other.effect_shade;

		for (int i = 0; i < 6; i++)
			sSpreadLocations[i] = other.sSpreadLocations[i];
		fDoSpread = other.fDoSpread;
		sStartGridNo = other.sStartGridNo;
		sEndGridNo = other.sEndGridNo;
		sForcastGridno = other.sForcastGridno;
		sZLevelOverride = other.sZLevelOverride;
		bMovedPriorToInterrupt = other.bMovedPriorToInterrupt;
		iEndofContractTime = other.iEndofContractTime;
		iStartContractTime = other.iStartContractTime;
		iTotalContractLength = other.iTotalContractLength;
		iNextActionSpecialData = other.iNextActionSpecialData;
		ubWhatKindOfMercAmI = other.ubWhatKindOfMercAmI;
		bAssignment = other.bAssignment;
		fForcedToStayAwake = other.fForcedToStayAwake;
		bTrainStat = other.bTrainStat;
		sSector = other.sSector;
		iVehicleId = other.iVehicleId;
		pMercPath = other.pMercPath;
		fHitByGasFlags = other.fHitByGasFlags;
		usMedicalDeposit = other.usMedicalDeposit;
		usLifeInsurance = other.usLifeInsurance;

		iStartOfInsuranceContract = other.iStartOfInsuranceContract;
		uiLastAssignmentChangeMin = other.uiLastAssignmentChangeMin;
		iTotalLengthOfInsuranceContract = other.iTotalLengthOfInsuranceContract;

		ubSoldierClass = other.ubSoldierClass;
		ubAPsLostToSuppression = other.ubAPsLostToSuppression;
		fChangingStanceDueToSuppression = other.fChangingStanceDueToSuppression;
		suppressor = other.suppressor;

		ubCivilianGroup = other.ubCivilianGroup;

		uiChangeLevelTime = other.uiChangeLevelTime;
		uiChangeHealthTime = other.uiChangeHealthTime;
		uiChangeStrengthTime = other.uiChangeStrengthTime;
		uiChangeDexterityTime = other.uiChangeDexterityTime;
		uiChangeAgilityTime = other.uiChangeAgilityTime;
		uiChangeWisdomTime = other.uiChangeWisdomTime;
		uiChangeLeadershipTime = other.uiChangeLeadershipTime;
		uiChangeMarksmanshipTime = other.uiChangeMarksmanshipTime;
		uiChangeExplosivesTime = other.uiChangeExplosivesTime;
		uiChangeMedicalTime = other.uiChangeMedicalTime;
		uiChangeMechanicalTime = other.uiChangeMechanicalTime;

		uiUniqueSoldierIdValue = other.uiUniqueSoldierIdValue;
		bBeingAttackedCount = other.bBeingAttackedCount;

		for (int i = 0; i < NUM_INV_SLOTS; i++) {
			bNewItemCount[i] = other.bNewItemCount[i];
			bNewItemCycleCount[i] = other.bNewItemCycleCount[i];
		}
		fCheckForNewlyAddedItems = other.fCheckForNewlyAddedItems;
		bEndDoorOpenCode = other.bEndDoorOpenCode;

		ubScheduleID = other.ubScheduleID;
		sEndDoorOpenCodeData = other.sEndDoorOpenCodeData;
		NextTileCounter = other.NextTileCounter;
		fBlockedByAnotherMerc = other.fBlockedByAnotherMerc;
		bBlockedByAnotherMercDirection = other.bBlockedByAnotherMercDirection;
		usAttackingWeapon = other.usAttackingWeapon;
		target = other.target;
		bWeaponMode = other.bWeaponMode;
		bAIScheduleProgress = other.bAIScheduleProgress;
		sOffWorldGridNo = other.sOffWorldGridNo;
		pAniTile = other.pAniTile;
		bCamo = other.bCamo;
		sAbsoluteFinalDestination = other.sAbsoluteFinalDestination;
		ubHiResDirection = other.ubHiResDirection;
		ubLastFootPrintSound = other.ubLastFootPrintSound;
		bVehicleID = other.bVehicleID;
		fPastXDest = other.fPastXDest;
		fPastYDest = other.fPastYDest;
		bMovementDirection = other.bMovementDirection;
		sOldGridNo = other.sOldGridNo;
		usDontUpdateNewGridNoOnMoveAnimChange = other.usDontUpdateNewGridNoOnMoveAnimChange;
		sBoundingBoxWidth = other.sBoundingBoxWidth;
		sBoundingBoxHeight = other.sBoundingBoxHeight;
		sBoundingBoxOffsetX = other.sBoundingBoxOffsetX;
		sBoundingBoxOffsetY = other.sBoundingBoxOffsetY;
		uiTimeSameBattleSndDone = other.uiTimeSameBattleSndDone;
		bOldBattleSnd = other.bOldBattleSnd;
		fContractPriceHasIncreased = other.fContractPriceHasIncreased;
		uiBurstSoundID = other.uiBurstSoundID;
		fFixingSAMSite = other.fFixingSAMSite;
		fFixingRobot = other.fFixingRobot;
		bSlotItemTakenFrom = other.bSlotItemTakenFrom;
		fSignedAnotherContract = other.fSignedAnotherContract;
		fDontChargeTurningAPs = other.fDontChargeTurningAPs;
		auto_bandaging_medic = other.auto_bandaging_medic;
		robot_remote_holder = other.robot_remote_holder;
		uiTimeOfLastContractUpdate = other.uiTimeOfLastContractUpdate;
		bTypeOfLastContract = other.bTypeOfLastContract;
		bTurnsCollapsed = other.bTurnsCollapsed;
		bSleepDrugCounter = other.bSleepDrugCounter;
		ubMilitiaKills = other.ubMilitiaKills;

		for (int i = 0; i < 2; i++) {
			bFutureDrugEffect[i] = other.bFutureDrugEffect[i];
			bDrugEffectRate[i] = other.bDrugEffectRate[i];
			bDrugEffect[i] = other.bDrugEffect[i];
			bDrugSideEffectRate[i] = other.bDrugSideEffectRate[i];
			bDrugSideEffect[i] = other.bDrugSideEffect[i];
		}

		bBlindedCounter = other.bBlindedCounter;
		fMercCollapsedFlag = other.fMercCollapsedFlag;
		fDoneAssignmentAndNothingToDoFlag = other.fDoneAssignmentAndNothingToDoFlag;
		fMercAsleep = other.fMercAsleep;
		fDontChargeAPsForStanceChange = other.fDontChargeAPsForStanceChange;

		ubTurnsUntilCanSayHeardNoise = other.ubTurnsUntilCanSayHeardNoise;
		usQuoteSaidExtFlags = other.usQuoteSaidExtFlags;

		sContPathLocation = other.sContPathLocation;
		bGoodContPath = other.bGoodContPath;
		bNoiseLevel = other.bNoiseLevel;
		bRegenerationCounter = other.bRegenerationCounter;
		bRegenBoostersUsedToday = other.bRegenBoostersUsedToday;
		bNumPelletsHitBy = other.bNumPelletsHitBy;
		sSkillCheckGridNo = other.sSkillCheckGridNo;
		ubLastEnemyCycledID = other.ubLastEnemyCycledID;

		ubPrevSectorID = other.ubPrevSectorID;
		ubNumTilesMovesSinceLastForget = other.ubNumTilesMovesSinceLastForget;
		bTurningIncrement = other.bTurningIncrement;
		uiBattleSoundID = other.uiBattleSoundID;

		fSoldierWasMoving = other.fSoldierWasMoving;
		fSayAmmoQuotePending = other.fSayAmmoQuotePending;
		usValueGoneUp = other.usValueGoneUp;

		ubNumLocateCycles = other.ubNumLocateCycles;
		ubDelayedMovementFlags = other.ubDelayedMovementFlags;
		fMuzzleFlash = other.fMuzzleFlash;
		CTGTTarget = other.CTGTTarget;

		PanelAnimateCounter = other.PanelAnimateCounter;

		bCurrentCivQuote = other.bCurrentCivQuote;
		bCurrentCivQuoteDelta = other.bCurrentCivQuoteDelta;
		ubMiscSoldierFlags = other.ubMiscSoldierFlags;
		ubReasonCantFinishMove = other.ubReasonCantFinishMove;

		sLocationOfFadeStart = other.sLocationOfFadeStart;
		bUseExitGridForReentryDirection = other.bUseExitGridForReentryDirection;

		uiTimeSinceLastSpoke = other.uiTimeSinceLastSpoke;
		ubContractRenewalQuoteCode = other.ubContractRenewalQuoteCode;
		sPreTraversalGridNo = other.sPreTraversalGridNo;
		uiXRayActivatedTime = other.uiXRayActivatedTime;
		bTurningFromUI = other.bTurningFromUI;
		bPendingActionData5 = other.bPendingActionData5;

		bDelayedStrategicMoraleMod = other.bDelayedStrategicMoraleMod;
		ubDoorOpeningNoise = other.ubDoorOpeningNoise;

		ubLeaveHistoryCode = other.ubLeaveHistoryCode;
		fDontUnsetLastTargetFromTurn = other.fDontUnsetLastTargetFromTurn;
		bOverrideMoveSpeed = other.bOverrideMoveSpeed;
		fUseMoverrideMoveSpeed = other.fUseMoverrideMoveSpeed;

		uiTimeSoldierWillArrive = other.uiTimeSoldierWillArrive;
		fUseLandingZoneForArrival = other.fUseLandingZoneForArrival;
		fFallClockwise = other.fFallClockwise;
		bVehicleUnderRepairID = other.bVehicleUnderRepairID;
		iTimeCanSignElsewhere = other.iTimeCanSignElsewhere;
		bHospitalPriceModifier = other.bHospitalPriceModifier;
		uiStartTimeOfInsuranceContract = other.uiStartTimeOfInsuranceContract;
		fRTInNonintAnim = other.fRTInNonintAnim;
		fDoingExternalDeath = other.fDoingExternalDeath;
		bCorpseQuoteTolerance = other.bCorpseQuoteTolerance;
		iPositionSndID = other.iPositionSndID;
		uiTuringSoundID = other.uiTuringSoundID;
		ubLastDamageReason = other.ubLastDamageReason;
		fComplainedThatTired = other.fComplainedThatTired;
		for (int i = 0; i < 2; i++)
			sLastTwoLocations[i] = other.sLastTwoLocations[i];
		uiTimeSinceLastBleedGrunt = other.uiTimeSinceLastBleedGrunt;

		ubPlayer = other.ubPlayer;

		return *this;
	}

	virtual RakString GetName(void) const { return RakString("SOLDIERTYPE"); }

	virtual void WriteAllocationID(Connection_RM3* destinationConnection, BitStream* allocationIdBitstream) const {
		allocationIdBitstream->Write(GetName());
	}

	void PrintStringInBitstream(BitStream* bs)
	{
		if (bs->GetNumberOfBitsUsed() == 0)
			return;
		RakString rakString;
		bs->Read(rakString);
		//printf("Receive: %s\n", rakString.C_String());
	}

	virtual void SerializeConstruction(BitStream* constructionBitstream, Connection_RM3* destinationConnection) {

		constructionBitstream->Write(GetName() + RakString(" SerializeConstruction"));
	}

	virtual bool DeserializeConstruction(BitStream* constructionBitstream, Connection_RM3* sourceConnection) {
		PrintStringInBitstream(constructionBitstream);
		return true;
	}

	virtual void SerializeDestruction(BitStream* destructionBitstream, Connection_RM3* destinationConnection) {

		destructionBitstream->Write(GetName() + RakString(" SerializeDestruction"));

	}

	virtual bool DeserializeDestruction(BitStream* destructionBitstream, Connection_RM3* sourceConnection) {
		PrintStringInBitstream(destructionBitstream);
		return true;
	}

	virtual void DeallocReplica(Connection_RM3* sourceConnection) {
		delete this;
	}

	/// Overloaded Replica3 function
	virtual void OnUserReplicaPreSerializeTick(void)
	{
	}

	// Pass everything except for pointers and strings (they are handlded in PreSerialize() and PostDeserialize() calls)
	virtual RM3SerializationResult Serialize(SerializeParameters* serializeParameters) {
		if (IS_CLIENT) // If we are client we don't serialize objects back to server
			return RM3SR_DO_NOT_SERIALIZE;
		else
			PreSerialize();

		serializeParameters->outputBitstream[0].Write(ubID);

		serializeParameters->outputBitstream[0].Write(ubBodyType);
		serializeParameters->outputBitstream[0].Write(bActionPoints);
		serializeParameters->outputBitstream[0].Write(bInitialActionPoints);

		serializeParameters->outputBitstream[0].Write(uiStatusFlags);

		serializeParameters->outputBitstream[0].Write(inv); // FIXME
		//serializeParameters->outputBitstream[0].Write(pTempObject);
		//serializeParameters->outputBitstream[0].Write(pKeyRing);

		serializeParameters->outputBitstream[0].Write(bOldLife);

		serializeParameters->outputBitstream[0].Write(bInSector);
		serializeParameters->outputBitstream[0].Write(bFlashPortraitFrame);
		serializeParameters->outputBitstream[0].Write(sFractLife);
		serializeParameters->outputBitstream[0].Write(bBleeding);
		serializeParameters->outputBitstream[0].Write(bBreath);
		serializeParameters->outputBitstream[0].Write(bBreathMax);
		serializeParameters->outputBitstream[0].Write(bStealthMode);

		serializeParameters->outputBitstream[0].Write(sBreathRed);
		serializeParameters->outputBitstream[0].Write(fDelayedMovement);

		serializeParameters->outputBitstream[0].Write(ubWaitActionToDo);
		serializeParameters->outputBitstream[0].Write(ubInsertionDirection);

		//serializeParameters->outputBitstream[0].Write(opponent);
		serializeParameters->outputBitstream[0].Write(opponent_id);
		serializeParameters->outputBitstream[0].Write(bLastRenderVisibleValue);
		serializeParameters->outputBitstream[0].Write(ubAttackingHand);

		serializeParameters->outputBitstream[0].Write(sWeightCarriedAtTurnStart);
		//serializeParameters->outputBitstream[0].Write(name);
		serializeParameters->outputBitstream[0].Write(rname);

		serializeParameters->outputBitstream[0].Write(bVisible);

		serializeParameters->outputBitstream[0].Write(bActive);

		serializeParameters->outputBitstream[0].Write(bTeam);

		serializeParameters->outputBitstream[0].Write(ubGroupID);
		serializeParameters->outputBitstream[0].Write(fBetweenSectors);

		serializeParameters->outputBitstream[0].Write(ubMovementNoiseHeard);

		serializeParameters->outputBitstream[0].Write(dXPos);
		serializeParameters->outputBitstream[0].Write(dYPos);
		serializeParameters->outputBitstream[0].Write(sInitialGridNo);
		serializeParameters->outputBitstream[0].Write(sGridNo);
		serializeParameters->outputBitstream[0].Write(bDirection);
		serializeParameters->outputBitstream[0].Write(sHeightAdjustment);
		serializeParameters->outputBitstream[0].Write(sDesiredHeight);
		serializeParameters->outputBitstream[0].Write(sTempNewGridNo);
		serializeParameters->outputBitstream[0].Write(bOverTerrainType);

		serializeParameters->outputBitstream[0].Write(bCollapsed);
		serializeParameters->outputBitstream[0].Write(bBreathCollapsed);

		serializeParameters->outputBitstream[0].Write(ubDesiredHeight);
		serializeParameters->outputBitstream[0].Write(usPendingAnimation);
		serializeParameters->outputBitstream[0].Write(ubPendingStanceChange);
		serializeParameters->outputBitstream[0].Write(usAnimState);
		serializeParameters->outputBitstream[0].Write(fNoAPToFinishMove);
		serializeParameters->outputBitstream[0].Write(fPausedMove);
		serializeParameters->outputBitstream[0].Write(fUIdeadMerc);
		serializeParameters->outputBitstream[0].Write(fUICloseMerc);

		serializeParameters->outputBitstream[0].Write(UpdateCounter);
		serializeParameters->outputBitstream[0].Write(DamageCounter);
		serializeParameters->outputBitstream[0].Write(AICounter);
		serializeParameters->outputBitstream[0].Write(FadeCounter);

		serializeParameters->outputBitstream[0].Write(ubSkillTrait1);
		serializeParameters->outputBitstream[0].Write(ubSkillTrait2);

		serializeParameters->outputBitstream[0].Write(bDexterity);
		serializeParameters->outputBitstream[0].Write(bWisdom);
		//serializeParameters->outputBitstream[0].Write(attacker);
		//serializeParameters->outputBitstream[0].Write(previous_attacker);
		//serializeParameters->outputBitstream[0].Write(next_to_previous_attacker);
		serializeParameters->outputBitstream[0].Write(attacker_id);
		serializeParameters->outputBitstream[0].Write(previous_attacker_id);
		serializeParameters->outputBitstream[0].Write(next_to_previous_attacker_id);
		serializeParameters->outputBitstream[0].Write(fTurnInProgress);

		serializeParameters->outputBitstream[0].Write(fIntendedTarget);
		serializeParameters->outputBitstream[0].Write(fPauseAllAnimation);

		serializeParameters->outputBitstream[0].Write(bExpLevel);
		serializeParameters->outputBitstream[0].Write(sInsertionGridNo);

		serializeParameters->outputBitstream[0].Write(fContinueMoveAfterStanceChange);

		//serializeParameters->outputBitstream[0].Write(AnimCache);

		serializeParameters->outputBitstream[0].Write(bLife);
		serializeParameters->outputBitstream[0].Write(bSide);
		serializeParameters->outputBitstream[0].Write(bNewOppCnt);

		serializeParameters->outputBitstream[0].Write(usAniCode);
		serializeParameters->outputBitstream[0].Write(usAniFrame);
		serializeParameters->outputBitstream[0].Write(sAniDelay);

		serializeParameters->outputBitstream[0].Write(bAgility);
		serializeParameters->outputBitstream[0].Write(sDelayedMovementCauseGridNo);
		serializeParameters->outputBitstream[0].Write(sReservedMovementGridNo);

		serializeParameters->outputBitstream[0].Write(bStrength);

		serializeParameters->outputBitstream[0].Write(sTargetGridNo);
		serializeParameters->outputBitstream[0].Write(bTargetLevel);
		serializeParameters->outputBitstream[0].Write(bTargetCubeLevel);
		serializeParameters->outputBitstream[0].Write(sLastTarget);
		serializeParameters->outputBitstream[0].Write(bTilesMoved);
		serializeParameters->outputBitstream[0].Write(bLeadership);
		serializeParameters->outputBitstream[0].Write(dNextBleed);
		serializeParameters->outputBitstream[0].Write(fWarnedAboutBleeding);
		serializeParameters->outputBitstream[0].Write(fDyingComment);

		serializeParameters->outputBitstream[0].Write(ubTilesMovedPerRTBreathUpdate);
		serializeParameters->outputBitstream[0].Write(usLastMovementAnimPerRTBreathUpdate);

		serializeParameters->outputBitstream[0].Write(fTurningToShoot);
		serializeParameters->outputBitstream[0].Write(fTurningUntilDone);
		serializeParameters->outputBitstream[0].Write(fGettingHit);
		serializeParameters->outputBitstream[0].Write(fInNonintAnim);
		serializeParameters->outputBitstream[0].Write(fFlashLocator);
		serializeParameters->outputBitstream[0].Write(sLocatorFrame);
		serializeParameters->outputBitstream[0].Write(fShowLocator);
		serializeParameters->outputBitstream[0].Write(fFlashPortrait);
		serializeParameters->outputBitstream[0].Write(bMechanical);
		serializeParameters->outputBitstream[0].Write(bLifeMax);

		//serializeParameters->outputBitstream[0].Write(face);

		/*serializeParameters->outputBitstream[0].Write(HeadPal);
		serializeParameters->outputBitstream[0].Write(PantsPal);
		serializeParameters->outputBitstream[0].Write(VestPal);
		serializeParameters->outputBitstream[0].Write(SkinPal);*/
		serializeParameters->outputBitstream[0].Write(rHeadPal);
		serializeParameters->outputBitstream[0].Write(rPantsPal);
		serializeParameters->outputBitstream[0].Write(rVestPal);
		serializeParameters->outputBitstream[0].Write(rSkinPal);

		//serializeParameters->outputBitstream[0].Write(pShades);
		//serializeParameters->outputBitstream[0].Write(pGlowShades);
		serializeParameters->outputBitstream[0].Write(bMedical);
		serializeParameters->outputBitstream[0].Write(fBeginFade);
		serializeParameters->outputBitstream[0].Write(ubFadeLevel);
		serializeParameters->outputBitstream[0].Write(ubServiceCount);
		//serializeParameters->outputBitstream[0].Write(service_partner);
		serializeParameters->outputBitstream[0].Write(service_partner_id);
		serializeParameters->outputBitstream[0].Write(bMarksmanship);
		serializeParameters->outputBitstream[0].Write(bExplosive);
		//serializeParameters->outputBitstream[0].Write(pThrowParams);
		serializeParameters->outputBitstream[0].Write(fTurningFromPronePosition);
		serializeParameters->outputBitstream[0].Write(bReverse);
		//serializeParameters->outputBitstream[0].Write(pLevelNode);

		serializeParameters->outputBitstream[0].Write(bDesiredDirection);
		serializeParameters->outputBitstream[0].Write(sDestXPos);
		serializeParameters->outputBitstream[0].Write(sDestYPos);
		serializeParameters->outputBitstream[0].Write(sDestination);
		serializeParameters->outputBitstream[0].Write(sFinalDestination);
		serializeParameters->outputBitstream[0].Write(bLevel);

		BS_ARRAY_WRITE(ubPathingData, MAX_PATH_LIST_SIZE);
		serializeParameters->outputBitstream[0].Write(ubPathDataSize);
		serializeParameters->outputBitstream[0].Write(ubPathIndex);
		serializeParameters->outputBitstream[0].Write(sBlackList);
		serializeParameters->outputBitstream[0].Write(bAimTime);
		// Don't copy the aiming value (circle radius) - let clients keep it local
		//serializeParameters->outputBitstream[0].Write(bShownAimTime);
		serializeParameters->outputBitstream[0].Write(bPathStored);
		serializeParameters->outputBitstream[0].Write(bHasKeys);

		serializeParameters->outputBitstream[0].Write(ubStrategicInsertionCode);
		serializeParameters->outputBitstream[0].Write(usStrategicInsertionData);

		//serializeParameters->outputBitstream[0].Write(light);
		//serializeParameters->outputBitstream[0].Write(muzzle_flash);
		serializeParameters->outputBitstream[0].Write(bMuzFlashCount);

		serializeParameters->outputBitstream[0].Write(sX);
		serializeParameters->outputBitstream[0].Write(sY);

		serializeParameters->outputBitstream[0].Write(usOldAniState);
		serializeParameters->outputBitstream[0].Write(sOldAniCode);

		serializeParameters->outputBitstream[0].Write(bBulletsLeft);
		serializeParameters->outputBitstream[0].Write(ubSuppressionPoints);

		serializeParameters->outputBitstream[0].Write(uiTimeOfLastRandomAction);

		BS_ARRAY_WRITE(bOppList, MAX_NUM_SOLDIERS);
		serializeParameters->outputBitstream[0].Write(bLastAction);
		serializeParameters->outputBitstream[0].Write(bAction);
		serializeParameters->outputBitstream[0].Write(usActionData);
		serializeParameters->outputBitstream[0].Write(bNextAction);
		serializeParameters->outputBitstream[0].Write(usNextActionData);
		serializeParameters->outputBitstream[0].Write(bActionInProgress);
		serializeParameters->outputBitstream[0].Write(bAlertStatus);
		serializeParameters->outputBitstream[0].Write(bOppCnt);
		serializeParameters->outputBitstream[0].Write(bNeutral);
		serializeParameters->outputBitstream[0].Write(bNewSituation);
		serializeParameters->outputBitstream[0].Write(bNextTargetLevel);
		serializeParameters->outputBitstream[0].Write(bOrders);
		serializeParameters->outputBitstream[0].Write(bAttitude);
		serializeParameters->outputBitstream[0].Write(bUnderFire);
		serializeParameters->outputBitstream[0].Write(bShock);
		serializeParameters->outputBitstream[0].Write(bBypassToGreen);
		serializeParameters->outputBitstream[0].Write(bDominantDir);
		serializeParameters->outputBitstream[0].Write(bPatrolCnt);
		serializeParameters->outputBitstream[0].Write(bNextPatrolPnt);
		BS_ARRAY_WRITE(usPatrolGrid, MAXPATROLGRIDS);
		serializeParameters->outputBitstream[0].Write(sNoiseGridno);
		serializeParameters->outputBitstream[0].Write(ubNoiseVolume);
		serializeParameters->outputBitstream[0].Write(bLastAttackHit);
		//serializeParameters->outputBitstream[0].Write(xrayed_by);
		serializeParameters->outputBitstream[0].Write(xrayed_by_id);
		serializeParameters->outputBitstream[0].Write(dHeightAdjustment);
		serializeParameters->outputBitstream[0].Write(bMorale);
		serializeParameters->outputBitstream[0].Write(bTeamMoraleMod);
		serializeParameters->outputBitstream[0].Write(bTacticalMoraleMod);
		serializeParameters->outputBitstream[0].Write(bStrategicMoraleMod);
		serializeParameters->outputBitstream[0].Write(bAIMorale);
		serializeParameters->outputBitstream[0].Write(ubPendingAction);
		serializeParameters->outputBitstream[0].Write(ubPendingActionAnimCount);
		serializeParameters->outputBitstream[0].Write(uiPendingActionData1);
		serializeParameters->outputBitstream[0].Write(sPendingActionData2);
		serializeParameters->outputBitstream[0].Write(bPendingActionData3);
		serializeParameters->outputBitstream[0].Write(ubDoorHandleCode);
		serializeParameters->outputBitstream[0].Write(uiPendingActionData4);
		serializeParameters->outputBitstream[0].Write(bInterruptDuelPts);
		serializeParameters->outputBitstream[0].Write(bPassedLastInterrupt);
		serializeParameters->outputBitstream[0].Write(bIntStartAPs);
		serializeParameters->outputBitstream[0].Write(bMoved);
		serializeParameters->outputBitstream[0].Write(bHunting);
		serializeParameters->outputBitstream[0].Write(ubCaller);
		serializeParameters->outputBitstream[0].Write(sCallerGridNo);
		serializeParameters->outputBitstream[0].Write(bCallPriority);
		serializeParameters->outputBitstream[0].Write(bCallActedUpon);
		serializeParameters->outputBitstream[0].Write(bFrenzied);
		serializeParameters->outputBitstream[0].Write(bNormalSmell);
		serializeParameters->outputBitstream[0].Write(bMonsterSmell);
		serializeParameters->outputBitstream[0].Write(bMobility);
		serializeParameters->outputBitstream[0].Write(fAIFlags);

		serializeParameters->outputBitstream[0].Write(fDontChargeReadyAPs);
		serializeParameters->outputBitstream[0].Write(usAnimSurface);
		serializeParameters->outputBitstream[0].Write(sZLevel);
		serializeParameters->outputBitstream[0].Write(fPrevInWater);
		serializeParameters->outputBitstream[0].Write(fGoBackToAimAfterHit);

		serializeParameters->outputBitstream[0].Write(sWalkToAttackGridNo);
		serializeParameters->outputBitstream[0].Write(sWalkToAttackWalkToCost);

		serializeParameters->outputBitstream[0].Write(fForceShade);
		//serializeParameters->outputBitstream[0].Write(pForcedShade);

		serializeParameters->outputBitstream[0].Write(bDisplayDamageCount);
		serializeParameters->outputBitstream[0].Write(fDisplayDamage);
		serializeParameters->outputBitstream[0].Write(sDamage);
		serializeParameters->outputBitstream[0].Write(sDamageX);
		serializeParameters->outputBitstream[0].Write(sDamageY);
		serializeParameters->outputBitstream[0].Write(bDoBurst);
		serializeParameters->outputBitstream[0].Write(usUIMovementMode);
		serializeParameters->outputBitstream[0].Write(fUIMovementFast);

		serializeParameters->outputBitstream[0].Write(BlinkSelCounter);
		serializeParameters->outputBitstream[0].Write(PortraitFlashCounter);
		serializeParameters->outputBitstream[0].Write(fDeadSoundPlayed);
		serializeParameters->outputBitstream[0].Write(ubProfile);
		serializeParameters->outputBitstream[0].Write(ubQuoteRecord);
		serializeParameters->outputBitstream[0].Write(ubQuoteActionID);
		serializeParameters->outputBitstream[0].Write(ubBattleSoundID);

		serializeParameters->outputBitstream[0].Write(fClosePanel);
		serializeParameters->outputBitstream[0].Write(fClosePanelToDie);
		serializeParameters->outputBitstream[0].Write(ubClosePanelFrame);
		serializeParameters->outputBitstream[0].Write(fDeadPanel);
		serializeParameters->outputBitstream[0].Write(ubDeadPanelFrame);

		serializeParameters->outputBitstream[0].Write(sPanelFaceX);
		serializeParameters->outputBitstream[0].Write(sPanelFaceY);

		serializeParameters->outputBitstream[0].Write(bNumHitsThisTurn);
		serializeParameters->outputBitstream[0].Write(usQuoteSaidFlags);
		serializeParameters->outputBitstream[0].Write(fCloseCall);
		serializeParameters->outputBitstream[0].Write(bLastSkillCheck);
		serializeParameters->outputBitstream[0].Write(ubSkillCheckAttempts);

		serializeParameters->outputBitstream[0].Write(bStartFallDir);
		serializeParameters->outputBitstream[0].Write(fTryingToFall);

		serializeParameters->outputBitstream[0].Write(ubPendingDirection);
		serializeParameters->outputBitstream[0].Write(uiAnimSubFlags);

		serializeParameters->outputBitstream[0].Write(bAimShotLocation);
		serializeParameters->outputBitstream[0].Write(ubHitLocation);

		//serializeParameters->outputBitstream[0].Write(effect_shade);

		BS_ARRAY_WRITE(sSpreadLocations, 6);
		serializeParameters->outputBitstream[0].Write(fDoSpread);
		serializeParameters->outputBitstream[0].Write(sStartGridNo);
		serializeParameters->outputBitstream[0].Write(sEndGridNo);
		serializeParameters->outputBitstream[0].Write(sForcastGridno);
		serializeParameters->outputBitstream[0].Write(sZLevelOverride);
		serializeParameters->outputBitstream[0].Write(bMovedPriorToInterrupt);
		serializeParameters->outputBitstream[0].Write(iEndofContractTime);
		serializeParameters->outputBitstream[0].Write(iStartContractTime);
		serializeParameters->outputBitstream[0].Write(iTotalContractLength);
		serializeParameters->outputBitstream[0].Write(iNextActionSpecialData);
		serializeParameters->outputBitstream[0].Write(ubWhatKindOfMercAmI);
		serializeParameters->outputBitstream[0].Write(bAssignment);
		serializeParameters->outputBitstream[0].Write(fForcedToStayAwake);
		serializeParameters->outputBitstream[0].Write(bTrainStat);
		serializeParameters->outputBitstream[0].Write(sSector);
		serializeParameters->outputBitstream[0].Write(iVehicleId);
		//serializeParameters->outputBitstream[0].Write(pMercPath);
		serializeParameters->outputBitstream[0].Write(fHitByGasFlags);
		serializeParameters->outputBitstream[0].Write(usMedicalDeposit);
		serializeParameters->outputBitstream[0].Write(usLifeInsurance);

		serializeParameters->outputBitstream[0].Write(iStartOfInsuranceContract);
		serializeParameters->outputBitstream[0].Write(uiLastAssignmentChangeMin);
		serializeParameters->outputBitstream[0].Write(iTotalLengthOfInsuranceContract);

		serializeParameters->outputBitstream[0].Write(ubSoldierClass);
		serializeParameters->outputBitstream[0].Write(ubAPsLostToSuppression);
		serializeParameters->outputBitstream[0].Write(fChangingStanceDueToSuppression);
		//serializeParameters->outputBitstream[0].Write(suppressor);
		serializeParameters->outputBitstream[0].Write(suppressor_id);

		serializeParameters->outputBitstream[0].Write(ubCivilianGroup);

		serializeParameters->outputBitstream[0].Write(uiChangeLevelTime);
		serializeParameters->outputBitstream[0].Write(uiChangeHealthTime);
		serializeParameters->outputBitstream[0].Write(uiChangeStrengthTime);
		serializeParameters->outputBitstream[0].Write(uiChangeDexterityTime);
		serializeParameters->outputBitstream[0].Write(uiChangeAgilityTime);
		serializeParameters->outputBitstream[0].Write(uiChangeWisdomTime);
		serializeParameters->outputBitstream[0].Write(uiChangeLeadershipTime);
		serializeParameters->outputBitstream[0].Write(uiChangeMarksmanshipTime);
		serializeParameters->outputBitstream[0].Write(uiChangeExplosivesTime);
		serializeParameters->outputBitstream[0].Write(uiChangeMedicalTime);
		serializeParameters->outputBitstream[0].Write(uiChangeMechanicalTime);

		serializeParameters->outputBitstream[0].Write(uiUniqueSoldierIdValue);
		serializeParameters->outputBitstream[0].Write(bBeingAttackedCount);

		BS_ARRAY_WRITE(bNewItemCount, NUM_INV_SLOTS);
		BS_ARRAY_WRITE(bNewItemCycleCount, NUM_INV_SLOTS);
		serializeParameters->outputBitstream[0].Write(fCheckForNewlyAddedItems);
		serializeParameters->outputBitstream[0].Write(bEndDoorOpenCode);

		serializeParameters->outputBitstream[0].Write(ubScheduleID);
		serializeParameters->outputBitstream[0].Write(sEndDoorOpenCodeData);
		serializeParameters->outputBitstream[0].Write(NextTileCounter);
		serializeParameters->outputBitstream[0].Write(fBlockedByAnotherMerc);
		serializeParameters->outputBitstream[0].Write(bBlockedByAnotherMercDirection);
		serializeParameters->outputBitstream[0].Write(usAttackingWeapon);
		//serializeParameters->outputBitstream[0].Write(target);
		serializeParameters->outputBitstream[0].Write(target_id);
		serializeParameters->outputBitstream[0].Write(bWeaponMode);
		serializeParameters->outputBitstream[0].Write(bAIScheduleProgress);
		serializeParameters->outputBitstream[0].Write(sOffWorldGridNo);
		//serializeParameters->outputBitstream[0].Write(pAniTile);
		serializeParameters->outputBitstream[0].Write(bCamo);
		serializeParameters->outputBitstream[0].Write(sAbsoluteFinalDestination);
		serializeParameters->outputBitstream[0].Write(ubHiResDirection);
		serializeParameters->outputBitstream[0].Write(ubLastFootPrintSound);
		serializeParameters->outputBitstream[0].Write(bVehicleID);
		serializeParameters->outputBitstream[0].Write(fPastXDest);
		serializeParameters->outputBitstream[0].Write(fPastYDest);
		serializeParameters->outputBitstream[0].Write(bMovementDirection);
		serializeParameters->outputBitstream[0].Write(sOldGridNo);
		serializeParameters->outputBitstream[0].Write(usDontUpdateNewGridNoOnMoveAnimChange);
		serializeParameters->outputBitstream[0].Write(sBoundingBoxWidth);
		serializeParameters->outputBitstream[0].Write(sBoundingBoxHeight);
		serializeParameters->outputBitstream[0].Write(sBoundingBoxOffsetX);
		serializeParameters->outputBitstream[0].Write(sBoundingBoxOffsetY);
		serializeParameters->outputBitstream[0].Write(uiTimeSameBattleSndDone);
		serializeParameters->outputBitstream[0].Write(bOldBattleSnd);
		serializeParameters->outputBitstream[0].Write(fContractPriceHasIncreased);
		serializeParameters->outputBitstream[0].Write(uiBurstSoundID);
		serializeParameters->outputBitstream[0].Write(fFixingSAMSite);
		serializeParameters->outputBitstream[0].Write(fFixingRobot);
		serializeParameters->outputBitstream[0].Write(bSlotItemTakenFrom);
		serializeParameters->outputBitstream[0].Write(fSignedAnotherContract);
		serializeParameters->outputBitstream[0].Write(fDontChargeTurningAPs);
		//serializeParameters->outputBitstream[0].Write(auto_bandaging_medic);
		//serializeParameters->outputBitstream[0].Write(robot_remote_holder);
		serializeParameters->outputBitstream[0].Write(auto_bandaging_medic_id);
		serializeParameters->outputBitstream[0].Write(robot_remote_holder_id);
		serializeParameters->outputBitstream[0].Write(uiTimeOfLastContractUpdate);
		serializeParameters->outputBitstream[0].Write(bTypeOfLastContract);
		serializeParameters->outputBitstream[0].Write(bTurnsCollapsed);
		serializeParameters->outputBitstream[0].Write(bSleepDrugCounter);
		serializeParameters->outputBitstream[0].Write(ubMilitiaKills);

		BS_ARRAY_WRITE(bFutureDrugEffect, 2);
		BS_ARRAY_WRITE(bDrugEffectRate, 2);
		BS_ARRAY_WRITE(bDrugEffect, 2);
		BS_ARRAY_WRITE(bDrugSideEffectRate, 2);
		BS_ARRAY_WRITE(bDrugSideEffect, 2);

		serializeParameters->outputBitstream[0].Write(bBlindedCounter);
		serializeParameters->outputBitstream[0].Write(fMercCollapsedFlag);
		serializeParameters->outputBitstream[0].Write(fDoneAssignmentAndNothingToDoFlag);
		serializeParameters->outputBitstream[0].Write(fMercAsleep);
		serializeParameters->outputBitstream[0].Write(fDontChargeAPsForStanceChange);

		serializeParameters->outputBitstream[0].Write(ubTurnsUntilCanSayHeardNoise);
		serializeParameters->outputBitstream[0].Write(usQuoteSaidExtFlags);

		serializeParameters->outputBitstream[0].Write(sContPathLocation);
		serializeParameters->outputBitstream[0].Write(bGoodContPath);
		serializeParameters->outputBitstream[0].Write(bNoiseLevel);
		serializeParameters->outputBitstream[0].Write(bRegenerationCounter);
		serializeParameters->outputBitstream[0].Write(bRegenBoostersUsedToday);
		serializeParameters->outputBitstream[0].Write(bNumPelletsHitBy);
		serializeParameters->outputBitstream[0].Write(sSkillCheckGridNo);
		serializeParameters->outputBitstream[0].Write(ubLastEnemyCycledID);

		serializeParameters->outputBitstream[0].Write(ubPrevSectorID);
		serializeParameters->outputBitstream[0].Write(ubNumTilesMovesSinceLastForget);
		serializeParameters->outputBitstream[0].Write(bTurningIncrement);
		serializeParameters->outputBitstream[0].Write(uiBattleSoundID);

		serializeParameters->outputBitstream[0].Write(fSoldierWasMoving);
		serializeParameters->outputBitstream[0].Write(fSayAmmoQuotePending);
		serializeParameters->outputBitstream[0].Write(usValueGoneUp);

		serializeParameters->outputBitstream[0].Write(ubNumLocateCycles);
		serializeParameters->outputBitstream[0].Write(ubDelayedMovementFlags);
		serializeParameters->outputBitstream[0].Write(fMuzzleFlash);
		//serializeParameters->outputBitstream[0].Write(CTGTTarget);
		serializeParameters->outputBitstream[0].Write(CTGTTarget_id);

		serializeParameters->outputBitstream[0].Write(PanelAnimateCounter);

		serializeParameters->outputBitstream[0].Write(bCurrentCivQuote);
		serializeParameters->outputBitstream[0].Write(bCurrentCivQuoteDelta);
		serializeParameters->outputBitstream[0].Write(ubMiscSoldierFlags);
		serializeParameters->outputBitstream[0].Write(ubReasonCantFinishMove);

		serializeParameters->outputBitstream[0].Write(sLocationOfFadeStart);
		serializeParameters->outputBitstream[0].Write(bUseExitGridForReentryDirection);

		serializeParameters->outputBitstream[0].Write(uiTimeSinceLastSpoke);
		serializeParameters->outputBitstream[0].Write(ubContractRenewalQuoteCode);
		serializeParameters->outputBitstream[0].Write(sPreTraversalGridNo);
		serializeParameters->outputBitstream[0].Write(uiXRayActivatedTime);
		serializeParameters->outputBitstream[0].Write(bTurningFromUI);
		serializeParameters->outputBitstream[0].Write(bPendingActionData5);

		serializeParameters->outputBitstream[0].Write(bDelayedStrategicMoraleMod);
		serializeParameters->outputBitstream[0].Write(ubDoorOpeningNoise);

		serializeParameters->outputBitstream[0].Write(ubLeaveHistoryCode);
		serializeParameters->outputBitstream[0].Write(fDontUnsetLastTargetFromTurn);
		serializeParameters->outputBitstream[0].Write(bOverrideMoveSpeed);
		serializeParameters->outputBitstream[0].Write(fUseMoverrideMoveSpeed);

		serializeParameters->outputBitstream[0].Write(uiTimeSoldierWillArrive);
		serializeParameters->outputBitstream[0].Write(fUseLandingZoneForArrival);
		serializeParameters->outputBitstream[0].Write(fFallClockwise);
		serializeParameters->outputBitstream[0].Write(bVehicleUnderRepairID);
		serializeParameters->outputBitstream[0].Write(iTimeCanSignElsewhere);
		serializeParameters->outputBitstream[0].Write(bHospitalPriceModifier);
		serializeParameters->outputBitstream[0].Write(uiStartTimeOfInsuranceContract);
		serializeParameters->outputBitstream[0].Write(fRTInNonintAnim);
		serializeParameters->outputBitstream[0].Write(fDoingExternalDeath);
		serializeParameters->outputBitstream[0].Write(bCorpseQuoteTolerance);
		serializeParameters->outputBitstream[0].Write(iPositionSndID);
		serializeParameters->outputBitstream[0].Write(uiTuringSoundID);
		serializeParameters->outputBitstream[0].Write(ubLastDamageReason);
		serializeParameters->outputBitstream[0].Write(fComplainedThatTired);
		BS_ARRAY_WRITE(sLastTwoLocations, 2);
		serializeParameters->outputBitstream[0].Write(uiTimeSinceLastBleedGrunt);

		serializeParameters->outputBitstream[0].Write(ubPlayer);

		return RM3SR_BROADCAST_IDENTICALLY;
	}

	// Here the number and sequence of Read() calls on de-serialized objects must correspond 1:1 to respective Write() calls in Serialize() above
	virtual void Deserialize(DeserializeParameters* deserializeParameters) {
		deserializeParameters->serializationBitstream[0].Read(ubID);

		deserializeParameters->serializationBitstream[0].Read(ubBodyType);
		deserializeParameters->serializationBitstream[0].Read(bActionPoints);
		deserializeParameters->serializationBitstream[0].Read(bInitialActionPoints);

		deserializeParameters->serializationBitstream[0].Read(uiStatusFlags);

		deserializeParameters->serializationBitstream[0].Read(inv); // FIXME
		//deserializeParameters->serializationBitstream[0].Read(pTempObject);
		//deserializeParameters->serializationBitstream[0].Read(pKeyRing);

		deserializeParameters->serializationBitstream[0].Read(bOldLife);

		deserializeParameters->serializationBitstream[0].Read(bInSector);
		deserializeParameters->serializationBitstream[0].Read(bFlashPortraitFrame);
		deserializeParameters->serializationBitstream[0].Read(sFractLife);
		deserializeParameters->serializationBitstream[0].Read(bBleeding);
		deserializeParameters->serializationBitstream[0].Read(bBreath);
		deserializeParameters->serializationBitstream[0].Read(bBreathMax);
		deserializeParameters->serializationBitstream[0].Read(bStealthMode);

		deserializeParameters->serializationBitstream[0].Read(sBreathRed);
		deserializeParameters->serializationBitstream[0].Read(fDelayedMovement);

		deserializeParameters->serializationBitstream[0].Read(ubWaitActionToDo);
		deserializeParameters->serializationBitstream[0].Read(ubInsertionDirection);

		//deserializeParameters->serializationBitstream[0].Read(opponent);
		deserializeParameters->serializationBitstream[0].Read(opponent_id);
		deserializeParameters->serializationBitstream[0].Read(bLastRenderVisibleValue);
		deserializeParameters->serializationBitstream[0].Read(ubAttackingHand);

		deserializeParameters->serializationBitstream[0].Read(sWeightCarriedAtTurnStart);
		//deserializeParameters->serializationBitstream[0].Read(name);
		deserializeParameters->serializationBitstream[0].Read(rname);

		deserializeParameters->serializationBitstream[0].Read(bVisible);


		deserializeParameters->serializationBitstream[0].Read(bActive);

		deserializeParameters->serializationBitstream[0].Read(bTeam);

		deserializeParameters->serializationBitstream[0].Read(ubGroupID);
		deserializeParameters->serializationBitstream[0].Read(fBetweenSectors);

		deserializeParameters->serializationBitstream[0].Read(ubMovementNoiseHeard);

		deserializeParameters->serializationBitstream[0].Read(dXPos);
		deserializeParameters->serializationBitstream[0].Read(dYPos);
		deserializeParameters->serializationBitstream[0].Read(sInitialGridNo);
		deserializeParameters->serializationBitstream[0].Read(sGridNo);
		deserializeParameters->serializationBitstream[0].Read(bDirection);
		deserializeParameters->serializationBitstream[0].Read(sHeightAdjustment);
		deserializeParameters->serializationBitstream[0].Read(sDesiredHeight);
		deserializeParameters->serializationBitstream[0].Read(sTempNewGridNo);
		deserializeParameters->serializationBitstream[0].Read(bOverTerrainType);

		deserializeParameters->serializationBitstream[0].Read(bCollapsed);
		deserializeParameters->serializationBitstream[0].Read(bBreathCollapsed);

		deserializeParameters->serializationBitstream[0].Read(ubDesiredHeight);
		deserializeParameters->serializationBitstream[0].Read(usPendingAnimation);
		deserializeParameters->serializationBitstream[0].Read(ubPendingStanceChange);
		deserializeParameters->serializationBitstream[0].Read(usAnimState);
		deserializeParameters->serializationBitstream[0].Read(fNoAPToFinishMove);
		deserializeParameters->serializationBitstream[0].Read(fPausedMove);
		deserializeParameters->serializationBitstream[0].Read(fUIdeadMerc);
		deserializeParameters->serializationBitstream[0].Read(fUICloseMerc);

		deserializeParameters->serializationBitstream[0].Read(UpdateCounter);
		deserializeParameters->serializationBitstream[0].Read(DamageCounter);
		deserializeParameters->serializationBitstream[0].Read(AICounter);
		deserializeParameters->serializationBitstream[0].Read(FadeCounter);

		deserializeParameters->serializationBitstream[0].Read(ubSkillTrait1);
		deserializeParameters->serializationBitstream[0].Read(ubSkillTrait2);

		deserializeParameters->serializationBitstream[0].Read(bDexterity);
		deserializeParameters->serializationBitstream[0].Read(bWisdom);
		//deserializeParameters->serializationBitstream[0].Read(attacker);
		//deserializeParameters->serializationBitstream[0].Read(previous_attacker);
		//deserializeParameters->serializationBitstream[0].Read(next_to_previous_attacker);
		deserializeParameters->serializationBitstream[0].Read(attacker_id);
		deserializeParameters->serializationBitstream[0].Read(previous_attacker_id);
		deserializeParameters->serializationBitstream[0].Read(next_to_previous_attacker_id);
		deserializeParameters->serializationBitstream[0].Read(fTurnInProgress);

		deserializeParameters->serializationBitstream[0].Read(fIntendedTarget);
		deserializeParameters->serializationBitstream[0].Read(fPauseAllAnimation);

		deserializeParameters->serializationBitstream[0].Read(bExpLevel);
		deserializeParameters->serializationBitstream[0].Read(sInsertionGridNo);

		deserializeParameters->serializationBitstream[0].Read(fContinueMoveAfterStanceChange);

		//deserializeParameters->serializationBitstream[0].Read(AnimCache);

		deserializeParameters->serializationBitstream[0].Read(bLife);
		deserializeParameters->serializationBitstream[0].Read(bSide);
		deserializeParameters->serializationBitstream[0].Read(bNewOppCnt);

		deserializeParameters->serializationBitstream[0].Read(usAniCode);
		deserializeParameters->serializationBitstream[0].Read(usAniFrame);
		deserializeParameters->serializationBitstream[0].Read(sAniDelay);

		deserializeParameters->serializationBitstream[0].Read(bAgility);
		deserializeParameters->serializationBitstream[0].Read(sDelayedMovementCauseGridNo);
		deserializeParameters->serializationBitstream[0].Read(sReservedMovementGridNo);

		deserializeParameters->serializationBitstream[0].Read(bStrength);

		deserializeParameters->serializationBitstream[0].Read(sTargetGridNo);
		deserializeParameters->serializationBitstream[0].Read(bTargetLevel);
		deserializeParameters->serializationBitstream[0].Read(bTargetCubeLevel);
		deserializeParameters->serializationBitstream[0].Read(sLastTarget);
		deserializeParameters->serializationBitstream[0].Read(bTilesMoved);
		deserializeParameters->serializationBitstream[0].Read(bLeadership);
		deserializeParameters->serializationBitstream[0].Read(dNextBleed);
		deserializeParameters->serializationBitstream[0].Read(fWarnedAboutBleeding);
		deserializeParameters->serializationBitstream[0].Read(fDyingComment);

		deserializeParameters->serializationBitstream[0].Read(ubTilesMovedPerRTBreathUpdate);
		deserializeParameters->serializationBitstream[0].Read(usLastMovementAnimPerRTBreathUpdate);

		deserializeParameters->serializationBitstream[0].Read(fTurningToShoot);
		deserializeParameters->serializationBitstream[0].Read(fTurningUntilDone);
		deserializeParameters->serializationBitstream[0].Read(fGettingHit);
		deserializeParameters->serializationBitstream[0].Read(fInNonintAnim);
		deserializeParameters->serializationBitstream[0].Read(fFlashLocator);
		deserializeParameters->serializationBitstream[0].Read(sLocatorFrame);
		deserializeParameters->serializationBitstream[0].Read(fShowLocator);
		deserializeParameters->serializationBitstream[0].Read(fFlashPortrait);
		deserializeParameters->serializationBitstream[0].Read(bMechanical);
		deserializeParameters->serializationBitstream[0].Read(bLifeMax);

		//deserializeParameters->serializationBitstream[0].Read(face);

		/*deserializeParameters->serializationBitstream[0].Read(HeadPal);
		deserializeParameters->serializationBitstream[0].Read(PantsPal);
		deserializeParameters->serializationBitstream[0].Read(VestPal);
		deserializeParameters->serializationBitstream[0].Read(SkinPal);*/
		deserializeParameters->serializationBitstream[0].Read(rHeadPal);
		deserializeParameters->serializationBitstream[0].Read(rPantsPal);
		deserializeParameters->serializationBitstream[0].Read(rVestPal);
		deserializeParameters->serializationBitstream[0].Read(rSkinPal);

		//deserializeParameters->serializationBitstream[0].Read(pShades);
		//deserializeParameters->serializationBitstream[0].Read(pGlowShades);
		deserializeParameters->serializationBitstream[0].Read(bMedical);
		deserializeParameters->serializationBitstream[0].Read(fBeginFade);
		deserializeParameters->serializationBitstream[0].Read(ubFadeLevel);
		deserializeParameters->serializationBitstream[0].Read(ubServiceCount);
		//deserializeParameters->serializationBitstream[0].Read(service_partner);
		deserializeParameters->serializationBitstream[0].Read(service_partner_id);
		deserializeParameters->serializationBitstream[0].Read(bMarksmanship);
		deserializeParameters->serializationBitstream[0].Read(bExplosive);
		//deserializeParameters->serializationBitstream[0].Read(pThrowParams);
		deserializeParameters->serializationBitstream[0].Read(fTurningFromPronePosition);
		deserializeParameters->serializationBitstream[0].Read(bReverse);
		//deserializeParameters->serializationBitstream[0].Read(pLevelNode);

		deserializeParameters->serializationBitstream[0].Read(bDesiredDirection);
		deserializeParameters->serializationBitstream[0].Read(sDestXPos);
		deserializeParameters->serializationBitstream[0].Read(sDestYPos);
		deserializeParameters->serializationBitstream[0].Read(sDestination);
		deserializeParameters->serializationBitstream[0].Read(sFinalDestination);
		deserializeParameters->serializationBitstream[0].Read(bLevel);

		BS_ARRAY_READ(ubPathingData, MAX_PATH_LIST_SIZE);
		deserializeParameters->serializationBitstream[0].Read(ubPathDataSize);
		deserializeParameters->serializationBitstream[0].Read(ubPathIndex);
		deserializeParameters->serializationBitstream[0].Read(sBlackList);
		deserializeParameters->serializationBitstream[0].Read(bAimTime);
		//deserializeParameters->serializationBitstream[0].Read(bShownAimTime);
		deserializeParameters->serializationBitstream[0].Read(bPathStored);
		deserializeParameters->serializationBitstream[0].Read(bHasKeys);

		deserializeParameters->serializationBitstream[0].Read(ubStrategicInsertionCode);
		deserializeParameters->serializationBitstream[0].Read(usStrategicInsertionData);

		//deserializeParameters->serializationBitstream[0].Read(light);
		//deserializeParameters->serializationBitstream[0].Read(muzzle_flash);
		deserializeParameters->serializationBitstream[0].Read(bMuzFlashCount);

		deserializeParameters->serializationBitstream[0].Read(sX);
		deserializeParameters->serializationBitstream[0].Read(sY);

		deserializeParameters->serializationBitstream[0].Read(usOldAniState);
		deserializeParameters->serializationBitstream[0].Read(sOldAniCode);

		deserializeParameters->serializationBitstream[0].Read(bBulletsLeft);
		deserializeParameters->serializationBitstream[0].Read(ubSuppressionPoints);

		deserializeParameters->serializationBitstream[0].Read(uiTimeOfLastRandomAction);

		BS_ARRAY_READ(bOppList, MAX_NUM_SOLDIERS);
		deserializeParameters->serializationBitstream[0].Read(bLastAction);
		deserializeParameters->serializationBitstream[0].Read(bAction);
		deserializeParameters->serializationBitstream[0].Read(usActionData);
		deserializeParameters->serializationBitstream[0].Read(bNextAction);
		deserializeParameters->serializationBitstream[0].Read(usNextActionData);
		deserializeParameters->serializationBitstream[0].Read(bActionInProgress);
		deserializeParameters->serializationBitstream[0].Read(bAlertStatus);
		deserializeParameters->serializationBitstream[0].Read(bOppCnt);
		deserializeParameters->serializationBitstream[0].Read(bNeutral);
		deserializeParameters->serializationBitstream[0].Read(bNewSituation);
		deserializeParameters->serializationBitstream[0].Read(bNextTargetLevel);
		deserializeParameters->serializationBitstream[0].Read(bOrders);
		deserializeParameters->serializationBitstream[0].Read(bAttitude);
		deserializeParameters->serializationBitstream[0].Read(bUnderFire);
		deserializeParameters->serializationBitstream[0].Read(bShock);
		deserializeParameters->serializationBitstream[0].Read(bBypassToGreen);
		deserializeParameters->serializationBitstream[0].Read(bDominantDir);
		deserializeParameters->serializationBitstream[0].Read(bPatrolCnt);
		deserializeParameters->serializationBitstream[0].Read(bNextPatrolPnt);
		BS_ARRAY_READ(usPatrolGrid, MAXPATROLGRIDS);
		deserializeParameters->serializationBitstream[0].Read(sNoiseGridno);
		deserializeParameters->serializationBitstream[0].Read(ubNoiseVolume);
		deserializeParameters->serializationBitstream[0].Read(bLastAttackHit);
		//deserializeParameters->serializationBitstream[0].Read(xrayed_by);
		deserializeParameters->serializationBitstream[0].Read(xrayed_by_id);
		deserializeParameters->serializationBitstream[0].Read(dHeightAdjustment);
		deserializeParameters->serializationBitstream[0].Read(bMorale);
		deserializeParameters->serializationBitstream[0].Read(bTeamMoraleMod);
		deserializeParameters->serializationBitstream[0].Read(bTacticalMoraleMod);
		deserializeParameters->serializationBitstream[0].Read(bStrategicMoraleMod);
		deserializeParameters->serializationBitstream[0].Read(bAIMorale);
		deserializeParameters->serializationBitstream[0].Read(ubPendingAction);
		deserializeParameters->serializationBitstream[0].Read(ubPendingActionAnimCount);
		deserializeParameters->serializationBitstream[0].Read(uiPendingActionData1);
		deserializeParameters->serializationBitstream[0].Read(sPendingActionData2);
		deserializeParameters->serializationBitstream[0].Read(bPendingActionData3);
		deserializeParameters->serializationBitstream[0].Read(ubDoorHandleCode);
		deserializeParameters->serializationBitstream[0].Read(uiPendingActionData4);
		deserializeParameters->serializationBitstream[0].Read(bInterruptDuelPts);
		deserializeParameters->serializationBitstream[0].Read(bPassedLastInterrupt);
		deserializeParameters->serializationBitstream[0].Read(bIntStartAPs);
		deserializeParameters->serializationBitstream[0].Read(bMoved);
		deserializeParameters->serializationBitstream[0].Read(bHunting);
		deserializeParameters->serializationBitstream[0].Read(ubCaller);
		deserializeParameters->serializationBitstream[0].Read(sCallerGridNo);
		deserializeParameters->serializationBitstream[0].Read(bCallPriority);
		deserializeParameters->serializationBitstream[0].Read(bCallActedUpon);
		deserializeParameters->serializationBitstream[0].Read(bFrenzied);
		deserializeParameters->serializationBitstream[0].Read(bNormalSmell);
		deserializeParameters->serializationBitstream[0].Read(bMonsterSmell);
		deserializeParameters->serializationBitstream[0].Read(bMobility);
		deserializeParameters->serializationBitstream[0].Read(fAIFlags);

		deserializeParameters->serializationBitstream[0].Read(fDontChargeReadyAPs);
		deserializeParameters->serializationBitstream[0].Read(usAnimSurface);
		deserializeParameters->serializationBitstream[0].Read(sZLevel);
		deserializeParameters->serializationBitstream[0].Read(fPrevInWater);
		deserializeParameters->serializationBitstream[0].Read(fGoBackToAimAfterHit);

		deserializeParameters->serializationBitstream[0].Read(sWalkToAttackGridNo);
		deserializeParameters->serializationBitstream[0].Read(sWalkToAttackWalkToCost);

		deserializeParameters->serializationBitstream[0].Read(fForceShade);
		//deserializeParameters->serializationBitstream[0].Read(pForcedShade);

		deserializeParameters->serializationBitstream[0].Read(bDisplayDamageCount);
		deserializeParameters->serializationBitstream[0].Read(fDisplayDamage);
		deserializeParameters->serializationBitstream[0].Read(sDamage);
		deserializeParameters->serializationBitstream[0].Read(sDamageX);
		deserializeParameters->serializationBitstream[0].Read(sDamageY);
		deserializeParameters->serializationBitstream[0].Read(bDoBurst);
		deserializeParameters->serializationBitstream[0].Read(usUIMovementMode);
		deserializeParameters->serializationBitstream[0].Read(fUIMovementFast);

		deserializeParameters->serializationBitstream[0].Read(BlinkSelCounter);
		deserializeParameters->serializationBitstream[0].Read(PortraitFlashCounter);
		deserializeParameters->serializationBitstream[0].Read(fDeadSoundPlayed);
		deserializeParameters->serializationBitstream[0].Read(ubProfile);
		deserializeParameters->serializationBitstream[0].Read(ubQuoteRecord);
		deserializeParameters->serializationBitstream[0].Read(ubQuoteActionID);
		deserializeParameters->serializationBitstream[0].Read(ubBattleSoundID);

		deserializeParameters->serializationBitstream[0].Read(fClosePanel);
		deserializeParameters->serializationBitstream[0].Read(fClosePanelToDie);
		deserializeParameters->serializationBitstream[0].Read(ubClosePanelFrame);
		deserializeParameters->serializationBitstream[0].Read(fDeadPanel);
		deserializeParameters->serializationBitstream[0].Read(ubDeadPanelFrame);

		deserializeParameters->serializationBitstream[0].Read(sPanelFaceX);
		deserializeParameters->serializationBitstream[0].Read(sPanelFaceY);

		deserializeParameters->serializationBitstream[0].Read(bNumHitsThisTurn);
		deserializeParameters->serializationBitstream[0].Read(usQuoteSaidFlags);
		deserializeParameters->serializationBitstream[0].Read(fCloseCall);
		deserializeParameters->serializationBitstream[0].Read(bLastSkillCheck);
		deserializeParameters->serializationBitstream[0].Read(ubSkillCheckAttempts);

		deserializeParameters->serializationBitstream[0].Read(bStartFallDir);
		deserializeParameters->serializationBitstream[0].Read(fTryingToFall);

		deserializeParameters->serializationBitstream[0].Read(ubPendingDirection);
		deserializeParameters->serializationBitstream[0].Read(uiAnimSubFlags);

		deserializeParameters->serializationBitstream[0].Read(bAimShotLocation);
		deserializeParameters->serializationBitstream[0].Read(ubHitLocation);

		//deserializeParameters->serializationBitstream[0].Read(effect_shade);

		BS_ARRAY_READ(sSpreadLocations, 6);
		deserializeParameters->serializationBitstream[0].Read(fDoSpread);
		deserializeParameters->serializationBitstream[0].Read(sStartGridNo);
		deserializeParameters->serializationBitstream[0].Read(sEndGridNo);
		deserializeParameters->serializationBitstream[0].Read(sForcastGridno);
		deserializeParameters->serializationBitstream[0].Read(sZLevelOverride);
		deserializeParameters->serializationBitstream[0].Read(bMovedPriorToInterrupt);
		deserializeParameters->serializationBitstream[0].Read(iEndofContractTime);
		deserializeParameters->serializationBitstream[0].Read(iStartContractTime);
		deserializeParameters->serializationBitstream[0].Read(iTotalContractLength);
		deserializeParameters->serializationBitstream[0].Read(iNextActionSpecialData);
		deserializeParameters->serializationBitstream[0].Read(ubWhatKindOfMercAmI);
		deserializeParameters->serializationBitstream[0].Read(bAssignment);
		deserializeParameters->serializationBitstream[0].Read(fForcedToStayAwake);
		deserializeParameters->serializationBitstream[0].Read(bTrainStat);
		deserializeParameters->serializationBitstream[0].Read(sSector);
		deserializeParameters->serializationBitstream[0].Read(iVehicleId);
		//deserializeParameters->serializationBitstream[0].Read(pMercPath);
		deserializeParameters->serializationBitstream[0].Read(fHitByGasFlags);
		deserializeParameters->serializationBitstream[0].Read(usMedicalDeposit);
		deserializeParameters->serializationBitstream[0].Read(usLifeInsurance);

		deserializeParameters->serializationBitstream[0].Read(iStartOfInsuranceContract);
		deserializeParameters->serializationBitstream[0].Read(uiLastAssignmentChangeMin);
		deserializeParameters->serializationBitstream[0].Read(iTotalLengthOfInsuranceContract);

		deserializeParameters->serializationBitstream[0].Read(ubSoldierClass);
		deserializeParameters->serializationBitstream[0].Read(ubAPsLostToSuppression);
		deserializeParameters->serializationBitstream[0].Read(fChangingStanceDueToSuppression);
		//deserializeParameters->serializationBitstream[0].Read(suppressor);
		deserializeParameters->serializationBitstream[0].Read(suppressor_id);

		deserializeParameters->serializationBitstream[0].Read(ubCivilianGroup);

		deserializeParameters->serializationBitstream[0].Read(uiChangeLevelTime);
		deserializeParameters->serializationBitstream[0].Read(uiChangeHealthTime);
		deserializeParameters->serializationBitstream[0].Read(uiChangeStrengthTime);
		deserializeParameters->serializationBitstream[0].Read(uiChangeDexterityTime);
		deserializeParameters->serializationBitstream[0].Read(uiChangeAgilityTime);
		deserializeParameters->serializationBitstream[0].Read(uiChangeWisdomTime);
		deserializeParameters->serializationBitstream[0].Read(uiChangeLeadershipTime);
		deserializeParameters->serializationBitstream[0].Read(uiChangeMarksmanshipTime);
		deserializeParameters->serializationBitstream[0].Read(uiChangeExplosivesTime);
		deserializeParameters->serializationBitstream[0].Read(uiChangeMedicalTime);
		deserializeParameters->serializationBitstream[0].Read(uiChangeMechanicalTime);

		deserializeParameters->serializationBitstream[0].Read(uiUniqueSoldierIdValue);
		deserializeParameters->serializationBitstream[0].Read(bBeingAttackedCount);

		BS_ARRAY_READ(bNewItemCount, NUM_INV_SLOTS);
		BS_ARRAY_READ(bNewItemCycleCount, NUM_INV_SLOTS);
		deserializeParameters->serializationBitstream[0].Read(fCheckForNewlyAddedItems);
		deserializeParameters->serializationBitstream[0].Read(bEndDoorOpenCode);

		deserializeParameters->serializationBitstream[0].Read(ubScheduleID);
		deserializeParameters->serializationBitstream[0].Read(sEndDoorOpenCodeData);
		deserializeParameters->serializationBitstream[0].Read(NextTileCounter);
		deserializeParameters->serializationBitstream[0].Read(fBlockedByAnotherMerc);
		deserializeParameters->serializationBitstream[0].Read(bBlockedByAnotherMercDirection);
		deserializeParameters->serializationBitstream[0].Read(usAttackingWeapon);
		//deserializeParameters->serializationBitstream[0].Read(target);
		deserializeParameters->serializationBitstream[0].Read(target_id);
		deserializeParameters->serializationBitstream[0].Read(bWeaponMode);
		deserializeParameters->serializationBitstream[0].Read(bAIScheduleProgress);
		deserializeParameters->serializationBitstream[0].Read(sOffWorldGridNo);
		//deserializeParameters->serializationBitstream[0].Read(pAniTile);
		deserializeParameters->serializationBitstream[0].Read(bCamo);
		deserializeParameters->serializationBitstream[0].Read(sAbsoluteFinalDestination);
		deserializeParameters->serializationBitstream[0].Read(ubHiResDirection);
		deserializeParameters->serializationBitstream[0].Read(ubLastFootPrintSound);
		deserializeParameters->serializationBitstream[0].Read(bVehicleID);
		deserializeParameters->serializationBitstream[0].Read(fPastXDest);
		deserializeParameters->serializationBitstream[0].Read(fPastYDest);
		deserializeParameters->serializationBitstream[0].Read(bMovementDirection);
		deserializeParameters->serializationBitstream[0].Read(sOldGridNo);
		deserializeParameters->serializationBitstream[0].Read(usDontUpdateNewGridNoOnMoveAnimChange);
		deserializeParameters->serializationBitstream[0].Read(sBoundingBoxWidth);
		deserializeParameters->serializationBitstream[0].Read(sBoundingBoxHeight);
		deserializeParameters->serializationBitstream[0].Read(sBoundingBoxOffsetX);
		deserializeParameters->serializationBitstream[0].Read(sBoundingBoxOffsetY);
		deserializeParameters->serializationBitstream[0].Read(uiTimeSameBattleSndDone);
		deserializeParameters->serializationBitstream[0].Read(bOldBattleSnd);
		deserializeParameters->serializationBitstream[0].Read(fContractPriceHasIncreased);
		deserializeParameters->serializationBitstream[0].Read(uiBurstSoundID);
		deserializeParameters->serializationBitstream[0].Read(fFixingSAMSite);
		deserializeParameters->serializationBitstream[0].Read(fFixingRobot);
		deserializeParameters->serializationBitstream[0].Read(bSlotItemTakenFrom);
		deserializeParameters->serializationBitstream[0].Read(fSignedAnotherContract);
		deserializeParameters->serializationBitstream[0].Read(fDontChargeTurningAPs);
		//deserializeParameters->serializationBitstream[0].Read(auto_bandaging_medic);
		//deserializeParameters->serializationBitstream[0].Read(robot_remote_holder);
		deserializeParameters->serializationBitstream[0].Read(auto_bandaging_medic_id);
		deserializeParameters->serializationBitstream[0].Read(robot_remote_holder_id);
		deserializeParameters->serializationBitstream[0].Read(uiTimeOfLastContractUpdate);
		deserializeParameters->serializationBitstream[0].Read(bTypeOfLastContract);
		deserializeParameters->serializationBitstream[0].Read(bTurnsCollapsed);
		deserializeParameters->serializationBitstream[0].Read(bSleepDrugCounter);
		deserializeParameters->serializationBitstream[0].Read(ubMilitiaKills);

		BS_ARRAY_READ(bFutureDrugEffect, 2);
		BS_ARRAY_READ(bDrugEffectRate, 2);
		BS_ARRAY_READ(bDrugEffect, 2);
		BS_ARRAY_READ(bDrugSideEffectRate, 2);
		BS_ARRAY_READ(bDrugSideEffect, 2);

		deserializeParameters->serializationBitstream[0].Read(bBlindedCounter);
		deserializeParameters->serializationBitstream[0].Read(fMercCollapsedFlag);
		deserializeParameters->serializationBitstream[0].Read(fDoneAssignmentAndNothingToDoFlag);
		deserializeParameters->serializationBitstream[0].Read(fMercAsleep);
		deserializeParameters->serializationBitstream[0].Read(fDontChargeAPsForStanceChange);

		deserializeParameters->serializationBitstream[0].Read(ubTurnsUntilCanSayHeardNoise);
		deserializeParameters->serializationBitstream[0].Read(usQuoteSaidExtFlags);

		deserializeParameters->serializationBitstream[0].Read(sContPathLocation);
		deserializeParameters->serializationBitstream[0].Read(bGoodContPath);
		deserializeParameters->serializationBitstream[0].Read(bNoiseLevel);
		deserializeParameters->serializationBitstream[0].Read(bRegenerationCounter);
		deserializeParameters->serializationBitstream[0].Read(bRegenBoostersUsedToday);
		deserializeParameters->serializationBitstream[0].Read(bNumPelletsHitBy);
		deserializeParameters->serializationBitstream[0].Read(sSkillCheckGridNo);
		deserializeParameters->serializationBitstream[0].Read(ubLastEnemyCycledID);

		deserializeParameters->serializationBitstream[0].Read(ubPrevSectorID);
		deserializeParameters->serializationBitstream[0].Read(ubNumTilesMovesSinceLastForget);
		deserializeParameters->serializationBitstream[0].Read(bTurningIncrement);
		deserializeParameters->serializationBitstream[0].Read(uiBattleSoundID);

		deserializeParameters->serializationBitstream[0].Read(fSoldierWasMoving);
		deserializeParameters->serializationBitstream[0].Read(fSayAmmoQuotePending);
		deserializeParameters->serializationBitstream[0].Read(usValueGoneUp);

		deserializeParameters->serializationBitstream[0].Read(ubNumLocateCycles);
		deserializeParameters->serializationBitstream[0].Read(ubDelayedMovementFlags);
		deserializeParameters->serializationBitstream[0].Read(fMuzzleFlash);
		//deserializeParameters->serializationBitstream[0].Read(CTGTTarget);
		deserializeParameters->serializationBitstream[0].Read(CTGTTarget_id);

		deserializeParameters->serializationBitstream[0].Read(PanelAnimateCounter);

		deserializeParameters->serializationBitstream[0].Read(bCurrentCivQuote);
		deserializeParameters->serializationBitstream[0].Read(bCurrentCivQuoteDelta);
		deserializeParameters->serializationBitstream[0].Read(ubMiscSoldierFlags);
		deserializeParameters->serializationBitstream[0].Read(ubReasonCantFinishMove);

		deserializeParameters->serializationBitstream[0].Read(sLocationOfFadeStart);
		deserializeParameters->serializationBitstream[0].Read(bUseExitGridForReentryDirection);

		deserializeParameters->serializationBitstream[0].Read(uiTimeSinceLastSpoke);
		deserializeParameters->serializationBitstream[0].Read(ubContractRenewalQuoteCode);
		deserializeParameters->serializationBitstream[0].Read(sPreTraversalGridNo);
		deserializeParameters->serializationBitstream[0].Read(uiXRayActivatedTime);
		deserializeParameters->serializationBitstream[0].Read(bTurningFromUI);
		deserializeParameters->serializationBitstream[0].Read(bPendingActionData5);

		deserializeParameters->serializationBitstream[0].Read(bDelayedStrategicMoraleMod);
		deserializeParameters->serializationBitstream[0].Read(ubDoorOpeningNoise);

		deserializeParameters->serializationBitstream[0].Read(ubLeaveHistoryCode);
		deserializeParameters->serializationBitstream[0].Read(fDontUnsetLastTargetFromTurn);
		deserializeParameters->serializationBitstream[0].Read(bOverrideMoveSpeed);
		deserializeParameters->serializationBitstream[0].Read(fUseMoverrideMoveSpeed);

		deserializeParameters->serializationBitstream[0].Read(uiTimeSoldierWillArrive);
		deserializeParameters->serializationBitstream[0].Read(fUseLandingZoneForArrival);
		deserializeParameters->serializationBitstream[0].Read(fFallClockwise);
		deserializeParameters->serializationBitstream[0].Read(bVehicleUnderRepairID);
		deserializeParameters->serializationBitstream[0].Read(iTimeCanSignElsewhere);
		deserializeParameters->serializationBitstream[0].Read(bHospitalPriceModifier);
		deserializeParameters->serializationBitstream[0].Read(uiStartTimeOfInsuranceContract);
		deserializeParameters->serializationBitstream[0].Read(fRTInNonintAnim);
		deserializeParameters->serializationBitstream[0].Read(fDoingExternalDeath);
		deserializeParameters->serializationBitstream[0].Read(bCorpseQuoteTolerance);
		deserializeParameters->serializationBitstream[0].Read(iPositionSndID);
		deserializeParameters->serializationBitstream[0].Read(uiTuringSoundID);
		deserializeParameters->serializationBitstream[0].Read(ubLastDamageReason);
		deserializeParameters->serializationBitstream[0].Read(fComplainedThatTired);
		BS_ARRAY_READ(sLastTwoLocations, 2);
		deserializeParameters->serializationBitstream[0].Read(uiTimeSinceLastBleedGrunt);

		deserializeParameters->serializationBitstream[0].Read(ubPlayer);

		PostDeserialize();
	}

	void PreSerialize() {
		// Using RakString as intermediate entity
		rname = name.c_str();

		rHeadPal = HeadPal.c_str();
		rPantsPal = PantsPal.c_str();
		rVestPal = VestPal.c_str();
		rSkinPal = SkinPal.c_str();

		// Passing soldier pointers through IDs
		opponent_id = Soldier2ID(opponent);
		attacker_id = Soldier2ID(attacker);
		previous_attacker_id = Soldier2ID(previous_attacker);
		next_to_previous_attacker_id = Soldier2ID(next_to_previous_attacker);
		service_partner_id = Soldier2ID(service_partner);
		xrayed_by_id = Soldier2ID(xrayed_by);
		suppressor_id = Soldier2ID(suppressor);
		target_id = Soldier2ID(target);
		auto_bandaging_medic_id = Soldier2ID(auto_bandaging_medic);
		robot_remote_holder_id = Soldier2ID(robot_remote_holder);
		CTGTTarget_id = Soldier2ID(CTGTTarget);
	}

	/*
	 * This method sets local versions/copies of complicated remote variables:
	 *
	 * Pointers:
	 *
	 * OBJECTTYPE* pTempObject - skip (created locally when interacting with inventory items);
	 * KEY_ON_RING* pKeyRing - done;
	 * SOLDIERTYPE* opponent - done;
	 * SOLDIERTYPE* attacker - done;
	 * SOLDIERTYPE* previous_attacker - done;
	 * SOLDIERTYPE* next_to_previous_attacker - done;
	 * FACETYPE* face - done;
	 * UINT16* pShades[NUM_SOLDIER_SHADES] - skip (created locally in CreateSoldierPalettes() function);
	 * UINT16* pGlowShades[20] - skip (created locally in CreateSoldierPalettes() function);
	 * SOLDIERTYPE* service_partner - done;
	 * THROW_PARAMS* pThrowParams - skip (created locally);
	 * LEVELNODE* pLevelNode - skip (created locally in AddMercToHead() function), but might require replication;
	 * LIGHT_SPRITE* light - skip (created locally by LightSpriteCreate() in CreateSoldierLight() function);
	 * LIGHT_SPRITE* muzzle_flash - skip (created locally by LightSpriteCreate() in AdjustToNextAnimationFrame() function);
	 * SOLDIERTYPE* xrayed_by - done;
	 * UINT16* pForcedShade - done;
	 * UINT16* effect_shade - skip (created locally in CreateSoldierPalettes() function);
	 * PathSt* pMercPath - skip (seems to be created locally by BuildAStrategicPath() in PlotPathForCharacter() function - TODO: Verify in the strategic screen movement);
	 * SOLDIERTYPE* suppressor - done;
	 * SOLDIERTYPE* target - done;
	 * ANITILE* pAniTile - skip (seems to be created locally by CreateAnimationTile() in HandleCrowShadowNewGridNo() function - TODO: Verify after the battle by getting back to the sector with dead bodies);
	 * SOLDIERTYPE* auto_bandaging_medic - done;
	 * SOLDIERTYPE* robot_remote_holder - done;
	 * const SOLDIERTYPE* CTGTTarget - done;
	 *
	 * Strings:
	 *
	 * ST::string name - done;
	 * ST::string HeadPal - done;
	 * ST::string PantsPal - done;
	 * ST::string VestPal - done;
	 * ST::string SkinPal - done;
	 *
	 * Etc:
	 *
	 * AnimationSurfaceCacheType AnimCache;
	 *
	 * + Loading the correspoding animation surfaces for the pair of usAnimSurface and usAnimState.
	 */
	void PostDeserialize() {
		// Using RakString as intermediate entity - in the order from Serialize() and Deserialize() calls
		name = rname;

		HeadPal = rHeadPal;
		PantsPal = rPantsPal;
		VestPal = rVestPal;
		SkinPal = rSkinPal;

		// Passing soldier pointers through IDs - in the order from Serialize() and Deserialize() calls
		opponent = ID2Soldier(opponent_id);
		attacker = ID2Soldier(attacker_id);
		previous_attacker = ID2Soldier(previous_attacker_id);
		next_to_previous_attacker = ID2Soldier(next_to_previous_attacker_id);
		service_partner = ID2Soldier(service_partner_id);
		xrayed_by = ID2Soldier(xrayed_by_id);
		suppressor = ID2Soldier(suppressor_id);
		target = ID2Soldier(target_id);
		auto_bandaging_medic = ID2Soldier(auto_bandaging_medic_id);
		robot_remote_holder = ID2Soldier(robot_remote_holder_id);
		CTGTTarget = ID2Soldier(CTGTTarget_id);

		// Other pointers - in the order from Serialize() and Deserialize() calls
		if ((pKeyRing == NULL) && bActive && (bTeam == OUR_TEAM))
			pKeyRing = new KEY_ON_RING[NUM_KEYS]{};

		if (bActive && !AnimCache_initialized) {
			AnimCache.init(ubID);

			// These two calls go together with AnimCache.init() when soldiers get created
			EVENT_InitNewSoldierAnim(this, usAnimState, usAnimState == STANDING ? Random(10) : 0, TRUE); // Initialize animation locally
			CreateSoldierPalettes(*this); // Initialize palette locally

			AnimCache_initialized = TRUE;
		}

		if ((face == NULL) && bActive && (bTeam == OUR_TEAM) && !face_initialized) {
			InitSoldierFace(*this);
			face_initialized = TRUE;
		}

		// The corresponding hVideoObject for the pair of usAnimState and usAnimSurface must be initialized locally, otherwise it would trigger an error in GetSoldierAnimationSurface()
		if (bActive && (gAnimSurfaceDatabase[usAnimSurface].hVideoObject == NULL))
			LoadAnimationSurface(ubID, usAnimSurface, usAnimState);

		pForcedShade = White16BPPPalette; // Hardcoding to the local resource copy (it seems that the pointer always points there anyway)
	}

	virtual void SerializeConstructionRequestAccepted(BitStream* serializationBitstream, Connection_RM3* requestingConnection) {
		serializationBitstream->Write(GetName() + RakString(" SerializeConstructionRequestAccepted"));
	}

	virtual void DeserializeConstructionRequestAccepted(BitStream* serializationBitstream, Connection_RM3* acceptingConnection) {
		PrintStringInBitstream(serializationBitstream);
	}

	virtual void SerializeConstructionRequestRejected(BitStream* serializationBitstream, Connection_RM3* requestingConnection) {
		serializationBitstream->Write(GetName() + RakString(" SerializeConstructionRequestRejected"));
	}

	virtual void DeserializeConstructionRequestRejected(BitStream* serializationBitstream, Connection_RM3* rejectingConnection) {
		PrintStringInBitstream(serializationBitstream);
	}

	virtual void OnPoppedConnection(Connection_RM3* droppedConnection)
	{
	}

	void NotifyReplicaOfMessageDeliveryStatus(RakNetGUID guid, uint32_t receiptId, bool messageArrived)
	{
	}

	virtual RM3ConstructionState QueryConstruction(Connection_RM3* destinationConnection, ReplicaManager3* replicaManager3) {
		return QueryConstruction_ServerConstruction(destinationConnection, IS_SERVER);
	}

	virtual bool QueryRemoteConstruction(Connection_RM3* sourceConnection) {
		return QueryRemoteConstruction_ServerConstruction(sourceConnection, IS_SERVER);
	}

	virtual RM3QuerySerializationResult QuerySerialization(Connection_RM3* destinationConnection) {
		return QuerySerialization_ServerSerializable(destinationConnection, IS_SERVER);
	}

	virtual RM3ActionOnPopConnection QueryActionOnPopConnection(Connection_RM3* droppedConnection) const {
		return QueryActionOnPopConnection_Server(droppedConnection);
	}

	// ID
	SoldierID ubID;

	// DESCRIPTION / STATS, ETC
	UINT8 ubBodyType;
	INT8 bActionPoints;
	INT8 bInitialActionPoints;

	UINT32 uiStatusFlags;

	OBJECTTYPE inv[ NUM_INV_SLOTS ];
	OBJECTTYPE *pTempObject;
	KEY_ON_RING *pKeyRing;

	INT8 bOldLife; // life at end of last turn, recorded for monster AI
	// attributes
	UINT8 bInSector;
	INT8 bFlashPortraitFrame;
	INT16 sFractLife; // fraction of life pts (in hundreths)
	INT8 bBleeding; // blood loss control variable
	INT8 bBreath; // current breath value
	INT8 bBreathMax; // max breath, affected by fatigue/sleep
	INT8 bStealthMode;

	INT16 sBreathRed; // current breath value
	BOOLEAN fDelayedMovement;

	UINT8 ubWaitActionToDo;
	INT8 ubInsertionDirection;
	// skills
	SOLDIERTYPE* opponent;
	UINT8 opponent_id;
	INT8 bLastRenderVisibleValue;
	UINT8 ubAttackingHand;
	// traits
	INT16 sWeightCarriedAtTurnStart;
	ST::string name;
	RakString rname; // Intermediate entity

	INT8 bVisible; // to render or not to render...


	INT8 bActive;

	INT8 bTeam; // Team identifier

	//NEW MOVEMENT INFORMATION for Strategic Movement
	UINT8 ubGroupID; //the movement group the merc is currently part of.
	BOOLEAN fBetweenSectors; //set when the group isn't actually in a sector.
	//sSectorX and sSectorY will reflect the sector the
	//merc was at last.

	UINT8 ubMovementNoiseHeard;// 8 flags by direction

	// WORLD POSITION STUFF
	FLOAT dXPos;
	FLOAT dYPos;
	INT16 sInitialGridNo;
	INT16 sGridNo;
	UINT8 bDirection;
	INT16 sHeightAdjustment;
	INT16 sDesiredHeight;
	INT16 sTempNewGridNo; // New grid no for advanced animations
	INT8 bOverTerrainType;

	INT8 bCollapsed; // collapsed due to being out of APs
	INT8 bBreathCollapsed; // collapsed due to being out of APs

	UINT8 ubDesiredHeight;
	UINT16 usPendingAnimation;
	UINT8 ubPendingStanceChange;
	UINT16 usAnimState;
	BOOLEAN fNoAPToFinishMove;
	BOOLEAN fPausedMove;
	BOOLEAN fUIdeadMerc; // UI Flags for removing a newly dead merc
	BOOLEAN fUICloseMerc; // UI Flags for closing panels

	TIMECOUNTER UpdateCounter;
	TIMECOUNTER DamageCounter;
	TIMECOUNTER AICounter;
	TIMECOUNTER FadeCounter;

	UINT8 ubSkillTrait1;
	UINT8 ubSkillTrait2;

	INT8 bDexterity; // dexterity (hand coord) value
	INT8 bWisdom;
	SOLDIERTYPE* attacker;
	SOLDIERTYPE* previous_attacker;
	SOLDIERTYPE* next_to_previous_attacker;
	UINT8 attacker_id;
	UINT8 previous_attacker_id;
	UINT8 next_to_previous_attacker_id;
	BOOLEAN fTurnInProgress;

	BOOLEAN fIntendedTarget; // intentionally shot?
	BOOLEAN fPauseAllAnimation;

	INT8 bExpLevel; // general experience level
	INT16 sInsertionGridNo;

	BOOLEAN fContinueMoveAfterStanceChange;

	AnimationSurfaceCacheType AnimCache;
	BOOLEAN AnimCache_initialized;

	INT8 bLife; // current life (hit points or health)
	Side bSide;
	INT8 bNewOppCnt;

	UINT16 usAniCode;
	UINT16 usAniFrame;
	INT16 sAniDelay;

	// MOVEMENT TO NEXT TILE HANDLING STUFF
	INT8 bAgility; // agility (speed) value
	INT16 sDelayedMovementCauseGridNo;
	INT16 sReservedMovementGridNo;

	INT8 bStrength;

	// Weapon Stuff
	INT16 sTargetGridNo;
	INT8 bTargetLevel;
	INT8 bTargetCubeLevel;
	INT16 sLastTarget;
	INT8 bTilesMoved;
	INT8 bLeadership;
	FLOAT dNextBleed;
	BOOLEAN fWarnedAboutBleeding;
	BOOLEAN fDyingComment;

	UINT8 ubTilesMovedPerRTBreathUpdate;
	UINT16 usLastMovementAnimPerRTBreathUpdate;

	BOOLEAN fTurningToShoot;
	BOOLEAN fTurningUntilDone;
	BOOLEAN fGettingHit;
	BOOLEAN fInNonintAnim;
	BOOLEAN fFlashLocator;
	INT16 sLocatorFrame;
	BOOLEAN fShowLocator;
	BOOLEAN fFlashPortrait;
	INT8 bMechanical;
	INT8 bLifeMax; // maximum life for this merc

	FACETYPE* face;
	BOOLEAN face_initialized;


	// PALETTE MANAGEMENT STUFF
	ST::string HeadPal;
	ST::string PantsPal;
	ST::string VestPal;
	ST::string SkinPal;
	// Intermediate entities
	RakString rHeadPal;
	RakString rPantsPal;
	RakString rVestPal;
	RakString rSkinPal;

	UINT16 *pShades[ NUM_SOLDIER_SHADES ]; // Shading tables
	UINT16 *pGlowShades[20];
	INT8 bMedical;
	BOOLEAN fBeginFade;
	UINT8 ubFadeLevel;
	UINT8 ubServiceCount;
	SOLDIERTYPE* service_partner;
	UINT8 service_partner_id;
	INT8 bMarksmanship;
	INT8 bExplosive;
	THROW_PARAMS *pThrowParams;
	BOOLEAN fTurningFromPronePosition;
	INT8 bReverse;
	LEVELNODE* pLevelNode;

	// WALKING STUFF
	INT8 bDesiredDirection;
	INT16 sDestXPos;
	INT16 sDestYPos;
	INT16 sDestination;
	INT16 sFinalDestination;
	INT8 bLevel;

	// PATH STUFF
	UINT8 ubPathingData[ MAX_PATH_LIST_SIZE ];
	UINT8 ubPathDataSize;
	UINT8 ubPathIndex;
	INT16 sBlackList;
	INT8 bAimTime;
	INT8 bShownAimTime;
	INT8 bPathStored; // good for AI to reduct redundancy
	INT8 bHasKeys; // allows AI controlled dudes to open locked doors

	UINT8 ubStrategicInsertionCode;
	UINT16 usStrategicInsertionData;

	LIGHT_SPRITE* light;
	LIGHT_SPRITE* muzzle_flash;
	INT8 bMuzFlashCount;

	INT16 sX;
	INT16 sY;

	UINT16 usOldAniState;
	INT16 sOldAniCode;

	INT8 bBulletsLeft;
	UINT8 ubSuppressionPoints;

	// STUFF FOR RANDOM ANIMATIONS
	UINT32 uiTimeOfLastRandomAction;

	// AI STUFF
	INT8 bOppList[MAX_NUM_SOLDIERS]; // AI knowledge database
	INT8 bLastAction;
	INT8 bAction;
	UINT16 usActionData;
	INT8 bNextAction;
	UINT16 usNextActionData;
	INT8 bActionInProgress;
	INT8 bAlertStatus;
	INT8 bOppCnt;
	INT8 bNeutral;
	INT8 bNewSituation;
	INT8 bNextTargetLevel;
	INT8 bOrders;
	INT8 bAttitude;
	INT8 bUnderFire;
	INT8 bShock;
	INT8 bBypassToGreen;
	INT8 bDominantDir; // AI main direction to face...
	INT8 bPatrolCnt; // number of patrol gridnos
	INT8 bNextPatrolPnt; // index to next patrol gridno
	INT16 usPatrolGrid[MAXPATROLGRIDS];// AI list for ptr->orders==PATROL
	INT16 sNoiseGridno;
	UINT8 ubNoiseVolume;
	INT8 bLastAttackHit;
	SOLDIERTYPE* xrayed_by;
	UINT8 xrayed_by_id;
	FLOAT dHeightAdjustment;
	INT8 bMorale;
	INT8 bTeamMoraleMod;
	INT8 bTacticalMoraleMod;
	INT8 bStrategicMoraleMod;
	INT8 bAIMorale;
	UINT8 ubPendingAction;
	UINT8 ubPendingActionAnimCount;
	UINT32 uiPendingActionData1;
	INT16 sPendingActionData2;
	INT8 bPendingActionData3;
	INT8 ubDoorHandleCode;
	UINT32 uiPendingActionData4;
	INT8 bInterruptDuelPts;
	INT8 bPassedLastInterrupt;
	INT8 bIntStartAPs;
	INT8 bMoved;
	INT8 bHunting;
	UINT8 ubCaller;
	INT16 sCallerGridNo;
	UINT8 bCallPriority;
	INT8 bCallActedUpon;
	INT8 bFrenzied;
	INT8 bNormalSmell;
	INT8 bMonsterSmell;
	INT8 bMobility;
	INT8 fAIFlags;

	BOOLEAN fDontChargeReadyAPs;
	UINT16 usAnimSurface;
	UINT16 sZLevel;
	BOOLEAN fPrevInWater;
	BOOLEAN fGoBackToAimAfterHit;

	INT16 sWalkToAttackGridNo;
	INT16 sWalkToAttackWalkToCost;

	BOOLEAN fForceShade;
	UINT16 *pForcedShade;

	INT8 bDisplayDamageCount;
	INT8 fDisplayDamage;
	INT16 sDamage;
	INT16 sDamageX;
	INT16 sDamageY;
	INT8 bDoBurst;
	INT16 usUIMovementMode;
	BOOLEAN fUIMovementFast;

	TIMECOUNTER BlinkSelCounter;
	TIMECOUNTER PortraitFlashCounter;
	BOOLEAN fDeadSoundPlayed;
	ProfileID ubProfile;
	UINT8 ubQuoteRecord;
	UINT8 ubQuoteActionID;
	UINT8 ubBattleSoundID;

	BOOLEAN fClosePanel;
	BOOLEAN fClosePanelToDie;
	UINT8 ubClosePanelFrame;
	BOOLEAN fDeadPanel;
	UINT8 ubDeadPanelFrame;

	INT16 sPanelFaceX;
	INT16 sPanelFaceY;

	// QUOTE STUFF
	INT8 bNumHitsThisTurn;
	UINT16 usQuoteSaidFlags;
	INT8 fCloseCall;
	INT8 bLastSkillCheck;
	INT8 ubSkillCheckAttempts;

	INT8 bStartFallDir;
	INT8 fTryingToFall;

	UINT8 ubPendingDirection;
	UINT32 uiAnimSubFlags;

	UINT8 bAimShotLocation;
	UINT8 ubHitLocation;

	UINT16* effect_shade; // Shading table for effects

	INT16 sSpreadLocations[ 6 ];
	BOOLEAN fDoSpread;
	INT16 sStartGridNo;
	INT16 sEndGridNo;
	INT16 sForcastGridno;
	INT16 sZLevelOverride;
	INT8 bMovedPriorToInterrupt;
	INT32 iEndofContractTime; // time, in global time(resolution, minutes) that merc will leave, or if its a M.E.R.C. merc it will be set to -1. -2 for NPC and player generated
	INT32 iStartContractTime;
	INT32 iTotalContractLength; // total time of AIM mercs contract or the time since last paid for a M.E.R.C. merc
	INT32 iNextActionSpecialData; // AI special action data record for the next action
	UINT8 ubWhatKindOfMercAmI; //Set to the type of character it is
	INT8 bAssignment; // soldiers current assignment
	BOOLEAN fForcedToStayAwake; // forced by player to stay awake, reset to false, the moment they are set to rest or sleep
	INT8 bTrainStat; // current stat soldier is training
	SGPSector sSector; // position on the Stategic Map
	INT32 iVehicleId; // the id of the vehicle the char is in
	PathSt* pMercPath; // Path Structure
	UINT8 fHitByGasFlags; // flags
	UINT16 usMedicalDeposit; // is there a medical deposit on merc
	UINT16 usLifeInsurance; // is there life insurance taken out on merc

	INT32 iStartOfInsuranceContract;
	UINT32 uiLastAssignmentChangeMin; // timestamp of last assignment change in minutes
	INT32 iTotalLengthOfInsuranceContract;

	UINT8 ubSoldierClass; //admin, elite, troop (creature types?)
	UINT8 ubAPsLostToSuppression;
	BOOLEAN fChangingStanceDueToSuppression;
	SOLDIERTYPE* suppressor;
	UINT8 suppressor_id;

	UINT8 ubCivilianGroup;

	// time changes...when a stat was changed according to GetJA2Clock();
	UINT32 uiChangeLevelTime;
	UINT32 uiChangeHealthTime;
	UINT32 uiChangeStrengthTime;
	UINT32 uiChangeDexterityTime;
	UINT32 uiChangeAgilityTime;
	UINT32 uiChangeWisdomTime;
	UINT32 uiChangeLeadershipTime;
	UINT32 uiChangeMarksmanshipTime;
	UINT32 uiChangeExplosivesTime;
	UINT32 uiChangeMedicalTime;
	UINT32 uiChangeMechanicalTime;

	UINT32 uiUniqueSoldierIdValue; // the unique value every instance of a soldier gets - 1 is the first valid value
	INT8 bBeingAttackedCount; // Being attacked counter

	INT8 bNewItemCount[ NUM_INV_SLOTS ];
	INT8 bNewItemCycleCount[ NUM_INV_SLOTS ];
	BOOLEAN fCheckForNewlyAddedItems;
	INT8 bEndDoorOpenCode;

	UINT8 ubScheduleID;
	INT16 sEndDoorOpenCodeData;
	TIMECOUNTER NextTileCounter;
	BOOLEAN fBlockedByAnotherMerc;
	INT8 bBlockedByAnotherMercDirection;
	UINT16 usAttackingWeapon;
	SOLDIERTYPE* target;
	UINT8 target_id;
	WeaponModes bWeaponMode;
	INT8 bAIScheduleProgress;
	INT16 sOffWorldGridNo;
	ANITILE* pAniTile;
	INT8 bCamo;
	INT16 sAbsoluteFinalDestination;
	UINT8 ubHiResDirection;
	UINT8 ubLastFootPrintSound;
	INT8 bVehicleID;
	INT8 fPastXDest;
	INT8 fPastYDest;
	INT8 bMovementDirection;
	INT16 sOldGridNo;
	UINT16 usDontUpdateNewGridNoOnMoveAnimChange;
	INT16 sBoundingBoxWidth;
	INT16 sBoundingBoxHeight;
	INT16 sBoundingBoxOffsetX;
	INT16 sBoundingBoxOffsetY;
	UINT32 uiTimeSameBattleSndDone;
	BattleSound bOldBattleSnd;
	BOOLEAN fContractPriceHasIncreased;
	UINT32 uiBurstSoundID;
	BOOLEAN fFixingSAMSite;
	BOOLEAN fFixingRobot;
	INT8 bSlotItemTakenFrom;
	BOOLEAN fSignedAnotherContract;
	BOOLEAN fDontChargeTurningAPs;
	SOLDIERTYPE* auto_bandaging_medic;
	SOLDIERTYPE* robot_remote_holder;
	UINT8 auto_bandaging_medic_id;
	UINT8 robot_remote_holder_id;
	UINT32 uiTimeOfLastContractUpdate;
	INT8 bTypeOfLastContract;
	INT8 bTurnsCollapsed;
	INT8 bSleepDrugCounter;
	UINT8 ubMilitiaKills;

	INT8 bFutureDrugEffect[2]; // value to represent effect of a needle
	INT8 bDrugEffectRate[2]; // represents rate of increase and decrease of effect
	INT8 bDrugEffect[2]; // value that affects AP & morale calc ( -ve is poorly )
	INT8 bDrugSideEffectRate[2]; // duration of negative AP and morale effect
	INT8 bDrugSideEffect[2]; // duration of negative AP and morale effect

	INT8 bBlindedCounter;
	BOOLEAN fMercCollapsedFlag;
	BOOLEAN fDoneAssignmentAndNothingToDoFlag;
	BOOLEAN fMercAsleep;
	BOOLEAN fDontChargeAPsForStanceChange;

	UINT8 ubTurnsUntilCanSayHeardNoise;
	UINT16 usQuoteSaidExtFlags;

	UINT16 sContPathLocation;
	INT8 bGoodContPath;
	INT8 bNoiseLevel;
	INT8 bRegenerationCounter;
	INT8 bRegenBoostersUsedToday;
	INT8 bNumPelletsHitBy;
	INT16 sSkillCheckGridNo;
	UINT8 ubLastEnemyCycledID;

	UINT8 ubPrevSectorID;
	UINT8 ubNumTilesMovesSinceLastForget;
	INT8 bTurningIncrement;
	UINT32 uiBattleSoundID;

	BOOLEAN fSoldierWasMoving;
	BOOLEAN fSayAmmoQuotePending;
	UINT16 usValueGoneUp;

	UINT8 ubNumLocateCycles;
	UINT8 ubDelayedMovementFlags;
	BOOLEAN fMuzzleFlash;
	const SOLDIERTYPE* CTGTTarget;
	UINT8 CTGTTarget_id;

	TIMECOUNTER PanelAnimateCounter;

	INT8 bCurrentCivQuote;
	INT8 bCurrentCivQuoteDelta;
	UINT8 ubMiscSoldierFlags;
	UINT8 ubReasonCantFinishMove;

	INT16 sLocationOfFadeStart;
	UINT8 bUseExitGridForReentryDirection;

	UINT32 uiTimeSinceLastSpoke;
	UINT8 ubContractRenewalQuoteCode;
	INT16 sPreTraversalGridNo;
	UINT32 uiXRayActivatedTime;
	INT8 bTurningFromUI;
	INT8 bPendingActionData5;

	INT8 bDelayedStrategicMoraleMod;
	UINT8 ubDoorOpeningNoise;

	UINT8 ubLeaveHistoryCode;
	BOOLEAN fDontUnsetLastTargetFromTurn;
	INT8 bOverrideMoveSpeed;
	BOOLEAN fUseMoverrideMoveSpeed;

	UINT32 uiTimeSoldierWillArrive;
	BOOLEAN fUseLandingZoneForArrival;
	BOOLEAN fFallClockwise;
	INT8 bVehicleUnderRepairID;
	INT32 iTimeCanSignElsewhere;
	INT8 bHospitalPriceModifier;
	UINT32 uiStartTimeOfInsuranceContract;
	BOOLEAN fRTInNonintAnim;
	BOOLEAN fDoingExternalDeath;
	INT8 bCorpseQuoteTolerance;
	INT32 iPositionSndID;
	UINT32 uiTuringSoundID;
	UINT8 ubLastDamageReason;
	BOOLEAN fComplainedThatTired;
	INT16 sLastTwoLocations[2];
	INT32 uiTimeSinceLastBleedGrunt;

	INT8 ubPlayer;
};

#define BASE_FOR_EACH_SOLDIER_INV_SLOT(type, iter, soldier) \
	for (type* iter = (soldier).inv, * const iter##__end = endof((soldier).inv); iter != iter##__end; ++iter)
#define FOR_EACH_SOLDIER_INV_SLOT(iter, soldier) \
	BASE_FOR_EACH_SOLDIER_INV_SLOT(OBJECTTYPE,       iter, soldier)
#define CFOR_EACH_SOLDIER_INV_SLOT(iter, soldier) \
	BASE_FOR_EACH_SOLDIER_INV_SLOT(OBJECTTYPE const, iter, soldier)

#define HEALTH_INCREASE					0x0001
#define STRENGTH_INCREASE				0x0002
#define DEX_INCREASE					0x0004
#define AGIL_INCREASE					0x0008
#define WIS_INCREASE					0x0010
#define LDR_INCREASE					0x0020

#define MRK_INCREASE					0x0040
#define MED_INCREASE					0x0080
#define EXP_INCREASE					0x0100
#define MECH_INCREASE					0x0200

#define LVL_INCREASE					0x0400


// TYPEDEFS FOR ANIMATION PROFILES
struct ANIM_PROF_TILE
{
	UINT16 usTileFlags;
	INT8   bTileX;
	INT8   bTileY;
};

struct ANIM_PROF_DIR
{
	UINT8 ubNumTiles;
	ANIM_PROF_TILE *pTiles;
};

struct ANIM_PROF
{
	ANIM_PROF_DIR Dirs[8];
};


struct PaletteReplacementType
{
	UINT8            ubType;
	ST::string       ID;
	UINT8            ubPaletteSize;
	SGPPaletteEntry* rgb;
};


// VARIABLES FOR PALETTE REPLACEMENTS FOR HAIR, ETC
extern UINT8*                  gubpNumReplacementsPerRange;
extern PaletteReplacementType* gpPalRep;

extern UINT8 bHealthStrRanges[];


void DeleteSoldier(SOLDIERTYPE&);
void DeleteSoldierLight(SOLDIERTYPE*);

void CreateSoldierCommon(SOLDIERTYPE&);


// Soldier Management functions, called by Event Pump.c
void EVENT_InitNewSoldierAnim(SOLDIERTYPE*, UINT16 new_state, UINT16 starting_ani_code, BOOLEAN force);

void ChangeSoldierState(SOLDIERTYPE* pSoldier, UINT16 usNewState, UINT16 usStartingAniCode, BOOLEAN fForce);

enum SetSoldierPosFlags
{
	SSP_NONE          = 0,
	SSP_NO_DEST       = 1U << 0,
	SSP_NO_FINAL_DEST = 1U << 1,
	SSP_FORCE_DELETE  = 1U << 2
};
ENUM_BITSET(SetSoldierPosFlags)

void EVENT_SetSoldierPosition(SOLDIERTYPE* s, GridNo gridno, SetSoldierPosFlags flags);
void EVENT_SetSoldierPositionNoCenter(SOLDIERTYPE* s, GridNo gridno, SetSoldierPosFlags flags);
void EVENT_SetSoldierPositionXY(SOLDIERTYPE* s, FLOAT dNewXPos, FLOAT dNewYPos, SetSoldierPosFlags flags);

void EVENT_GetNewSoldierPath( SOLDIERTYPE *pSoldier, UINT16 sDestGridNo, UINT16 usMovementAnim );
BOOLEAN EVENT_InternalGetNewSoldierPath( SOLDIERTYPE *pSoldier, UINT16 sDestGridNo, UINT16 usMovementAnim, BOOLEAN fFromUI, BOOLEAN fForceRestart );

void EVENT_SetSoldierDirection( SOLDIERTYPE *pSoldier, UINT16	usNewDirection );
void EVENT_SetSoldierDesiredDirection( SOLDIERTYPE *pSoldier, UINT16	usNewDirection );
void EVENT_SetSoldierDesiredDirectionForward(SOLDIERTYPE* s, UINT16 new_direction);
void EVENT_FireSoldierWeapon( SOLDIERTYPE *pSoldier, INT16 sTargetGridNo );
void EVENT_SoldierGotHit(SOLDIERTYPE* pSoldier, UINT16 usWeaponIndex, INT16 sDamage, INT16 sBreathLoss, UINT16 bDirection, UINT16 sRange, SOLDIERTYPE* att, UINT8 ubSpecial, UINT8 ubHitLocation, INT16 sLocationGrid);
void EVENT_SoldierBeginBladeAttack( SOLDIERTYPE *pSoldier, INT16 sGridNo, UINT8 ubDirection );
void EVENT_SoldierBeginPunchAttack( SOLDIERTYPE *pSoldier, INT16 sGridNo, UINT8 ubDirection );
void EVENT_SoldierBeginFirstAid( SOLDIERTYPE *pSoldier, INT16 sGridNo, UINT8 ubDirection );
void EVENT_StopMerc(SOLDIERTYPE*);
void EVENT_StopMerc(SOLDIERTYPE*, GridNo, INT8 direction);
void EVENT_SoldierBeginCutFence( SOLDIERTYPE *pSoldier, INT16 sGridNo, UINT8 ubDirection );
void EVENT_SoldierBeginRepair(SOLDIERTYPE&, GridNo, UINT8 direction);
void EVENT_SoldierBeginRefuel( SOLDIERTYPE *pSoldier, INT16 sGridNo, UINT8 ubDirection );


BOOLEAN SoldierReadyWeapon(SOLDIERTYPE* pSoldier, GridNo tgt_pos, BOOLEAN fEndReady);
void SetSoldierHeight(SOLDIERTYPE*, FLOAT new_height);
void BeginSoldierClimbUpRoof( SOLDIERTYPE *pSoldier );
void BeginSoldierClimbDownRoof(SOLDIERTYPE*);
void BeginSoldierClimbFence(SOLDIERTYPE*);
void BeginSoldierClimbWindow(SOLDIERTYPE*);

BOOLEAN CheckSoldierHitRoof( SOLDIERTYPE *pSoldier );
void BeginSoldierGetup( SOLDIERTYPE *pSoldier );

// Soldier Management functions called by Overhead.c
BOOLEAN ConvertAniCodeToAniFrame( SOLDIERTYPE *pSoldier, UINT16 usAniFrame );
void TurnSoldier( SOLDIERTYPE *pSold);
void EVENT_BeginMercTurn(SOLDIERTYPE&);
void ChangeSoldierStance( SOLDIERTYPE *pSoldier, UINT8 ubDesiredStance );
void ModifySoldierAniSpeed( SOLDIERTYPE *pSoldier );
void StopSoldier( SOLDIERTYPE *pSoldier );
UINT8 SoldierTakeDamage(SOLDIERTYPE* pSoldier, INT16 sLifeDeduct, INT16 sBreathLoss, UINT8 ubReason, SOLDIERTYPE* attacker);
void ReviveSoldier( SOLDIERTYPE *pSoldier );


// Palette functions for soldiers
void  CreateSoldierPalettes(SOLDIERTYPE&);
std::optional<UINT8> GetPaletteRepIndexFromID(const ST::string& pal_rep);
void SetPaletteReplacement(SGPPaletteEntry* p8BPPPalette, const ST::string& aPalRep);
void  LoadPaletteData(void);
void  DeletePaletteData(void);

// UTILITY FUNCTUIONS
void MoveMerc( SOLDIERTYPE *pSoldier, FLOAT dMovementChange, FLOAT dAngle, BOOLEAN fCheckRange );
void MoveMercFacingDirection( SOLDIERTYPE *pSoldier, BOOLEAN fReverse, FLOAT dMovementDist );
INT16 GetDirectionFromGridNo(INT16 sGridNo, const SOLDIERTYPE* pSoldier);
UINT8 atan8( INT16 sXPos, INT16 sYPos, INT16 sXPos2, INT16 sYPos2 );
INT8 CalcActionPoints(const SOLDIERTYPE*);
INT16 GetDirectionToGridNoFromGridNo( INT16 sGridNoDest, INT16 sGridNoSrc );
void ReleaseSoldiersAttacker( SOLDIERTYPE *pSoldier );
BOOLEAN MercInWater(const SOLDIERTYPE* pSoldier);
UINT16 GetMoveStateBasedOnStance(const SOLDIERTYPE*, UINT8 ubStanceHeight);
void SoldierGotoStationaryStance( SOLDIERTYPE *pSoldier );
void ReCreateSoldierLight(SOLDIERTYPE*);

// Returns the soldier's structure ID if he has a levelnode, INVALID_STRUCTURE_ID otherwise
UINT16 GetStructureID(SOLDIERTYPE const *);

void    MakeCharacterDialogueEventDoBattleSound(SOLDIERTYPE& s, BattleSound, UINT32 delay);
BOOLEAN DoMercBattleSound(SOLDIERTYPE*, BattleSound);
BOOLEAN InternalDoMercBattleSound(SOLDIERTYPE*, BattleSound, bool lowerVolume);


UINT32 SoldierDressWound( SOLDIERTYPE *pSoldier, SOLDIERTYPE *pVictim, INT16 sKitPts, INT16 sStatus );
void ReceivingSoldierCancelServices( SOLDIERTYPE *pSoldier );
void GivingSoldierCancelServices( SOLDIERTYPE *pSoldier );
void InternalGivingSoldierCancelServices( SOLDIERTYPE *pSoldier, BOOLEAN fPlayEndAnim );


// WRAPPERS FOR SOLDIER EVENTS
void SendGetNewSoldierPathEvent(SOLDIERTYPE*, UINT16 sDestGridNo);
void SendSoldierSetDesiredDirectionEvent(const SOLDIERTYPE* pSoldier, UINT16 usDesiredDirection);
void SendBeginFireWeaponEvent( SOLDIERTYPE *pSoldier, INT16 sTargetGridNo );

void HaultSoldierFromSighting( SOLDIERTYPE *pSoldier, BOOLEAN fFromSightingEnemy );
void ReLoadSoldierAnimationDueToHandItemChange( SOLDIERTYPE *pSoldier, UINT16 usOldItem, UINT16 usNewItem );

bool CheckForBreathCollapse(SOLDIERTYPE&);

static inline BOOLEAN IsOnCivTeam(const SOLDIERTYPE* const s)
{
	return s->bTeam == CIV_TEAM;
}

#define PTR_CROUCHED					(gAnimControl[ pSoldier->usAnimState ].ubHeight == ANIM_CROUCH)
#define PTR_STANDING					(gAnimControl[ pSoldier->usAnimState ].ubHeight == ANIM_STAND)
#define PTR_PRONE					(gAnimControl[ pSoldier->usAnimState ].ubHeight == ANIM_PRONE)


void EVENT_SoldierBeginGiveItem( SOLDIERTYPE *pSoldier );

void DoNinjaAttack( SOLDIERTYPE *pSoldier );

BOOLEAN InternalSoldierReadyWeapon( SOLDIERTYPE *pSoldier, UINT8 sFacingDir, BOOLEAN fEndReady );

void RemoveSoldierFromGridNo(SOLDIERTYPE&);

void PositionSoldierLight( SOLDIERTYPE *pSoldier );

void EVENT_InternalSetSoldierDestination( SOLDIERTYPE *pSoldier, UINT16	usNewDirection, BOOLEAN fFromMove, UINT16 usAnimState );

void ChangeToFallbackAnimation( SOLDIERTYPE *pSoldier, INT8 bDirection );

void EVENT_SoldierBeginKnifeThrowAttack( SOLDIERTYPE *pSoldier, INT16 sGridNo, UINT8 ubDirection );
void EVENT_SoldierBeginUseDetonator( SOLDIERTYPE *pSoldier );
void EVENT_SoldierBeginDropBomb( SOLDIERTYPE *pSoldier );
void EVENT_SoldierEnterVehicle(SOLDIERTYPE&, GridNo);


void SetSoldierCowerState( SOLDIERTYPE *pSoldier, BOOLEAN fOn );

BOOLEAN PlayerSoldierStartTalking( SOLDIERTYPE *pSoldier, UINT8 ubTargetID, BOOLEAN fValidate );

void CalcNewActionPoints( SOLDIERTYPE *pSoldier );

BOOLEAN InternalIsValidStance(const SOLDIERTYPE* pSoldier, INT8 bDirection, INT8 bNewStance);

void AdjustNoAPToFinishMove( SOLDIERTYPE *pSoldier, BOOLEAN fSet );


void UpdateRobotControllerGivenController( SOLDIERTYPE *pSoldier );
void UpdateRobotControllerGivenRobot( SOLDIERTYPE *pSoldier );
const SOLDIERTYPE *GetRobotController( const SOLDIERTYPE *pSoldier );
BOOLEAN CanRobotBeControlled(const SOLDIERTYPE* pSoldier);
BOOLEAN ControllingRobot(const SOLDIERTYPE* s);

void EVENT_SoldierBeginReloadRobot( SOLDIERTYPE *pSoldier, INT16 sGridNo, UINT8 ubDirection, UINT8 ubMercSlot );

void EVENT_SoldierBeginTakeBlood( SOLDIERTYPE *pSoldier, INT16 sGridNo, UINT8 ubDirection );

void EVENT_SoldierBeginAttachCan( SOLDIERTYPE *pSoldier, INT16 sGridNo, UINT8 ubDirection );

void PickDropItemAnimation( SOLDIERTYPE *pSoldier );

bool IsValidSecondHandShot(SOLDIERTYPE const*);
bool IsValidSecondHandShotForReloadingPurposes(SOLDIERTYPE const*);

void CrowsFlyAway( UINT8 ubTeam );

void DebugValidateSoldierData(void);

void BeginTyingToFall( SOLDIERTYPE *pSoldier );

void SetSoldierAsUnderAiControl( SOLDIERTYPE *pSoldier );
void HandlePlayerTogglingLightEffects( BOOLEAN fToggleValue );

void HandleSystemNewAISituation(SOLDIERTYPE*);
void SetSoldierAniSpeed(SOLDIERTYPE* pSoldier);
void PlaySoldierFootstepSound(SOLDIERTYPE* pSoldier);
void PlayStealthySoldierFootstepSound(SOLDIERTYPE* pSoldier);

// DO NOT CALL UNLESS THROUGH EVENT_SetSoldierPosition
UINT16 PickSoldierReadyAnimation(SOLDIERTYPE* pSoldier, BOOLEAN fEndReady);

extern BOOLEAN gfGetNewPathThroughPeople;

void FlashSoldierPortrait(SOLDIERTYPE*);

static inline bool IsWearingHeadGear(SOLDIERTYPE const& s, UINT16 const item)
{
	return s.inv[HEAD1POS].usItem == item || s.inv[HEAD2POS].usItem == item;
}

#endif
