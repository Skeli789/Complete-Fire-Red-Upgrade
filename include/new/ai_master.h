#pragma once

#include "../global.h"
#include "ai_scripts.h"

/**
 * \file ai_master.h
 * \brief Contains functions relating to general doings of the battle AI.
 */

//Exported Functions
void BattleAI_SetupAIData(u8 defaultScoreMoves);
void WipeOldDeperateAttemptRecord(u8 bankAtk);
u32 GetAIFlags(void);
u8 BattleAI_ChooseMoveOrAction(void);
u32 WildMonIsSmart(u8 bank);
void PopulateAIScriptStructWithBaseAttackerData(struct AIScript* data, u8 bankAtk);
void PopulateAIScriptStructWithBaseDefenderData(struct AIScript* data, u8 bankDef);
void LoadBattlersAndFoes(u8* battlerIn1, u8* battlerIn2, u8* foe1, u8* foe2);
void CalculateAIPredictions(void);
u8 GetAI_ItemType(u16 itemId, const u8 *itemEffect); //Fixed from vanilla
void ClearCachedAIData(void);
void ClearMonCachedDamageCalcs(void);
void RechooseAIMoveAfterSwitchIfNecessary(void);
void TryChangeMoveTargetToCounterPlayerProtectCheese(void);
void PickRaidBossRepeatedMove(u8 moveLimitations);

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
