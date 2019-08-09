#ifndef GUARD_BATTLE_AI_SCRIPT_COMMANDS_H
#define GUARD_BATTLE_AI_SCRIPT_COMMANDS_H

#include "global.h"

void __attribute__((long_call)) BattleAI_HandleItemUseBeforeAISetup(u8 defaultScoreMoves);
void __attribute__((long_call)) BattleAI_SetupAIData(u8 defaultScoreMoves);
//u8 __attribute__((long_call)) BattleAI_ChooseMoveOrAction(void);
void __attribute__((long_call)) ClearBankMoveHistory(u8 bank);
void __attribute__((long_call)) RecordAbilityBattle(u8 bank, u8 abilityId);
void __attribute__((long_call)) ClearBankAbilityHistory(u8 bank);
void __attribute__((long_call)) RecordItemEffectBattle(u8 bank, u8 itemEffect);
void __attribute__((long_call)) ClearBankItemEffectHistory(u8 bank);

#endif // GUARD_BATTLE_AI_SCRIPT_COMMANDS_H
