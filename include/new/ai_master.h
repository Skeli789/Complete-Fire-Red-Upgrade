#pragma once

#include "../global.h"
#include "ai_scripts.h"

/**
 * \file ai_master.h
 * \brief Contains functions relating to general doings of the battle AI.
 */

//Exported Functions
void BattleAI_SetupAIData(u8 defaultScoreMoves);
u32 GetAIFlags(void);
u8 BattleAI_ChooseMoveOrAction(void);
u8 GetMostSuitableMonToSwitchInto(void);
s8 GetMostSuitableMonToSwitchIntoScore(void);
u8 CalcMostSuitableMonToSwitchInto(void);
void ResetBestMonToSwitchInto(u8 bank);
void RemoveBestMonToSwitchInto(u8 bank);
u32 WildMonIsSmart(u8 bank);
void PopulateAIScriptStructWithBaseAttackerData(struct AIScript* data, u8 bankAtk);
void PopulateAIScriptStructWithBaseDefenderData(struct AIScript* data, u8 bankDef);
void LoadBattlersAndFoes(u8* battlerIn1, u8* battlerIn2, u8* foe1, u8* foe2);
void TryTempMegaEvolveBank(u8 bank, struct BattlePokemon* backupMon, u16* backupSpecies, u8* backupAbility);
void TryRevertTempMegaEvolveBank(u8 bank, struct BattlePokemon* backupMon, u16* backupSpecies, u8* backupAbility);
void ShouldDoAIShiftSwitch(void);

//Functions Hooked In
void BattleAI_HandleItemUseBeforeAISetup(void);
void AI_TrySwitchOrUseItem(void);
bool8 HasSuperEffectiveMoveAgainstOpponents(bool8 noRng);
bool8 FindMonWithFlagsAndSuperEffective(u8 flags, u8 moduloPercent);

//Exported Constants
#define AI_ACTION_DONE          0x0001
#define AI_ACTION_FLEE          0x0002
#define AI_ACTION_WATCH         0x0004
#define AI_ACTION_DO_NOT_ATTACK 0x0008
#define AI_ACTION_UNK5          0x0010
#define AI_ACTION_UNK6          0x0020
#define AI_ACTION_UNK7          0x0040
#define AI_ACTION_UNK8          0x0080

#define SWITCHING_INCREASE_KO_FOE 4
#define SWITCHING_INCREASE_RESIST_ALL_MOVES 4
#define SWITCHING_INCREASE_REVENGE_KILL 2 //Can only happen if can KO in the first place
#define SWITCHING_INCREASE_WALLS_FOE 2 //Can only wall if no Super-Effective moves against foe
#define SWITCHING_INCREASE_HAS_SUPER_EFFECTIVE_MOVE 1
#define SWITCHING_INCREASE_CAN_REMOVE_HAZARDS 10

#define SWITCHING_DECREASE_WEAK_TO_MOVE 1

#define SWITCHING_SCORE_MAX (SWITCHING_INCREASE_KO_FOE + SWITCHING_INCREASE_RESIST_ALL_MOVES + SWITCHING_INCREASE_REVENGE_KILL)
