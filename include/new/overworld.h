#pragma once

#include "../global.h"
/**
 * \file overworld.h
 * \brief Contains functions relating to the things that happen in the overworld. This
 *	      includes trainers spotting the player for battle (including two at a time),
 *		  the player facing trainers before battle, scripts that run on every step,
 *		  scripts that run on every frame (like Swarm checks), updated whiteout
 *		  logic, Player running logic, and OW poison features.
 */

//Exported Functions
const u8* BattleSetup_ConfigureTrainerBattle(const u8* data);
bool8 IsTrainerBattleModeAgainstTwoOpponents(void);
bool8 IsTrainerBattleModeWithPartner(void);
void AllowTrainerIncrementation(void);
void MoveSecondNPCForTwoOpponentSighting(void);
void LoadProperIntroSpeechForTwoOpponentSighting(void);
void PrepTrainerB(void);
void HasOneTrainerBeenDefeated(void);
void MoveCameraToTrainerB(void);
void TrainerFaceFix(void);
void FollowerPositionFix(u8 offset);
bool8 IsAutoRunEnabled(void);
bool8 IsRunningDisallowed(u8 tile);
bool8 IsCurrentAreaHotCave(void);
bool8 IsCurrentAreaVolcano(void);
bool8 IsCurrentAreaAutumn(void);
bool8 IsCurrentAreaWinter(void);
bool8 IsCurrentAreaDesert(void);
bool8 IsCurrentAreaSwamp(void);
bool8 IsCurrentAreaDarkerCave(void);
bool8 InTanobyRuins(void);
void PlayGrassFootstepNoise(void);
void PlaySandFootstepNoise(void);
const u8* LoadProperWhiteoutString(const u8* string);
bool8 IsPlayerSurfingNorthOrSouth(void);
bool8 IsPlayerFacingRockClimbableWall(void);
bool8 MetatileBehavior_IsRockClimbableWall(u8 behaviour);
u8 PartyHasMonWithFieldMovePotential(u16 move, u16 item, u8 surfingType);
void UseRegisteredItem(u16 registeredItem);
void TryLoadMiningSpots(void);
void ChooseMiningSpotToShow(void);
void IsBestMiningSpotOutOfView(void);
void ResetMiningSpots(void);
bool8 IsValidMiningSpot(s16 x, s16 y);
bool8 MetatileBehavior_IsStairs(u8 behaviour);
bool8 CanUseEscapeRopeOnCurrMap(void);
void FieldCB_RushInjuredPokemonToCenter(void);

//Functions Hooked In
u8 CheckForTrainersWantingBattle(void);
void BattleSetup_StartTrainerBattle(void);
const u8* GetIntroSpeechOfApproachingTrainer(void);
const u8* GetTrainerCantBattleSpeech(void);
void SetUpTrainerEncounterMusic(void);
void SetTrainerFlags(void);
bool8 TryStartStepCountScript(u16 metatileBehavior);
bool8 TryRunOnFrameMapScript(void);
bool8 WhiteoutLogic(void);
bool8 IsRunningDisallowedByMetatile(u8 tile);
bool8 Overworld_IsBikingAllowed(void);
void MoveOnBike(u8 direction);
s32 DoPoisonFieldEffect(void);
void TaskRepel(u8 taskId);
bool8 UpdateRepelCounter(void);
const u8* GetInteractedWaterScript(u32 unused1, u8 metatileBehavior, u8 direction);

//Exported Constants
enum
{
	SHOULDNT_BE_SURFING = 1,
	SHOULD_BE_SURFING,
};

//Exported Data Structures
struct Pedometers
{
	u32 alwaysActive;
	u32 large;
	u16 medium;
	u8 smallOne;
	u8 smallTwo;
};

#define gPedometers ((struct Pedometers*) 0x203B7BC)
