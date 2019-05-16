#pragma once

#include "../global.h"

/**
 * \file ai_master.h
 * \brief Contains functions relating to general doings of the battle AI.
 */
 
//Exported Functions
void BattleAI_SetupAIData(u8 defaultScoreMoves);
u32 GetAIFlags(void);
u8 GetMostSuitableMonToSwitchInto(void);
u32 WildMonIsSmart(u8 bank);

//Functions Hooked In
void BattleAI_HandleItemUseBeforeAISetup(u8 defaultScoreMoves);
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