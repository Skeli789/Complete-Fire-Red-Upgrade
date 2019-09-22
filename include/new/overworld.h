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
void FollowerPositionFix(void);
bool8 IsAutoRunEnabled(void);
bool8 IsRunningDisallowed(u8 tile);
bool8 IsCurrentAreaVolcano(void);
bool8 IsCurrentAreaAutumn(void);
bool8 IsCurrentAreaWinter(void);
bool8 IsCurrentAreaDarkerCave(void);
bool8 InTanobyRuins(void);
void PlayGrassFootstepNoise(void);
void PlaySandFootstepNoise(void);
const u8* LoadProperWhiteoutString(const u8* string);
bool8 IsPlayerFacingRockClimbableWall(void);
void ShouldRockClimbContinue(void);
void ShouldRockClimbContinueDiagonally(void);

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
s32 DoPoisonFieldEffect(void);
void TaskRepel(u8 taskId);
bool8 UpdateRepelCounter(void);

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
