#pragma once

#include "global.h"

#define MOVE_LIMITATION_ZEROMOVE                (1 << 0)
#define MOVE_LIMITATION_PP                      (1 << 1)
#define MOVE_LIMITATION_DISABLED                (1 << 2)
#define MOVE_LIMITATION_TORMENTED               (1 << 3)
#define MOVE_LIMITATION_TAUNT                   (1 << 4)
#define MOVE_LIMITATION_IMPRISION               (1 << 5)

#define ABILITYEFFECT_ON_SWITCHIN               0x0
#define ABILITYEFFECT_ENDTURN                   0x1
#define ABILITYEFFECT_MOVES_BLOCK               0x2
#define ABILITYEFFECT_ABSORBING                 0x3
#define ABILITYEFFECT_CONTACT                   0x4
#define ABILITYEFFECT_IMMUNITY                  0x5
#define ABILITYEFFECT_FORECAST                  0x6
#define ABILITYEFFECT_SYNCHRONIZE               0x7
#define ABILITYEFFECT_ATK_SYNCHRONIZE           0x8
#define ABILITYEFFECT_INTIMIDATE1               0x9
#define ABILITYEFFECT_INTIMIDATE2               0xA
#define ABILITYEFFECT_TRACE                     0xB
#define ABILITYEFFECT_CHECK_OTHER_SIDE          0xC
#define ABILITYEFFECT_CHECK_BANK_SIDE           0xD
#define ABILITYEFFECT_FIELD_SPORT               0xE
#define ABILITYEFFECT_CHECK_FIELD_EXCEPT_BANK   0xF
#define ABILITYEFFECT_COUNT_OTHER_SIDE          0x10
#define ABILITYEFFECT_COUNT_BANK_SIDE           0x11
#define ABILITYEFFECT_COUNT_ON_FIELD            0x12
#define ABILITYEFFECT_CHECK_ON_FIELD            0x13
#define ABILITYEFFECT_MOVES_BLOCK_PARTNER		0x14
#define ABILITYEFFECT_SWITCH_IN_WEATHER         0xFF

#define ABILITY_ON_OPPOSING_FIELD(battlerId, abilityId) (AbilityBattleEffects(ABILITYEFFECT_CHECK_OTHER_SIDE, battlerId, abilityId, 0, 0))
#define ABILITY_ON_FIELD(abilityId) (AbilityBattleEffects(ABILITYEFFECT_CHECK_ON_FIELD, 0, abilityId, 0, 0))
#define ABILITY_ON_FIELD2(abilityId) (AbilityBattleEffects(ABILITYEFFECT_FIELD_SPORT, 0, abilityId, 0, 0))

#define ITEMEFFECT_ON_SWITCH_IN                 0x0

extern bool8 WeatherHasEffect(void);
#define WEATHER_HAS_EFFECT WeatherHasEffect()

#define MON_CAN_BATTLE(mon) (((GetMonData(mon, MON_DATA_SPECIES, NULL) != SPECIES_NONE && !GetMonData(mon, MON_DATA_IS_EGG, NULL) && GetMonData(mon, MON_DATA_HP, NULL) != 0)))
#define SPECIES_CANT_BATTLE(species) (species == SPECIES_NONE || species == SPECIES_EGG)

#define BS_GET_TARGET                   0
#define BS_GET_ATTACKER                 1
#define BS_GET_EFFECT_BANK              2
#define BS_GET_SCRIPTING_BANK           10
#define BS_GET_PLAYER1                  11
#define BS_GET_OPPONENT1                12
#define BS_GET_PLAYER2                  13
#define BS_GET_OPPONENT2                14

void __attribute__((long_call)) PressurePPLose(u8 bankDef, u8 bankAtk, u16 move);
void __attribute__((long_call)) PressurePPLoseOnUsingPerishSong(u8 bankAtk);
void __attribute__((long_call)) PressurePPLoseOnUsingImprision(u8 bankAtk);

/*
void __attribute__((long_call)) MarkAllBufferBanksForExecution(void); // unused
void __attribute__((long_call)) MarkBufferBankForExecution(u8 bank);
void __attribute__((long_call)) sub_803F850(u8 arg0);
void __attribute__((long_call)) CancelMultiTurnMoves(u8 bank);
bool8 __attribute__((long_call)) WasUnableToUseMove(u8 bank);
void __attribute__((long_call)) PrepareStringBattle(u16 stringId, u8 bank);
void __attribute__((long_call)) ResetSentPokesToOpponentValue(void);
void __attribute__((long_call)) sub_803F9EC(u8 bank);
void __attribute__((long_call)) sub_803FA70(u8 bank);
void __attribute__((long_call)) BattleScriptPush(const u8* bsPtr);
void __attribute__((long_call)) BattleScriptPushCursor(void);
void __attribute__((long_call)) BattleScriptPop(void);
u8 __attribute__((long_call)) TrySetCantSelectMoveBattleScript(void);
u8 __attribute__((long_call)) CheckMoveLimitations(u8 bank, u8 unusableMoves, u8 check);
bool8 __attribute__((long_call)) AreAllMovesUnusable(void);
u8 __attribute__((long_call)) GetImprisonedMovesCount(u8 bank, u16 move);
u8 __attribute__((long_call)) UpdateTurnCounters(void);
u8 __attribute__((long_call)) TurnBasedEffects(void);
bool8 __attribute__((long_call)) HandleWishPerishSongOnTurnEnd(void);
bool8 __attribute__((long_call)) HandleFaintedMonActions(void);
void __attribute__((long_call)) TryClearRageStatuses(void);
u8 __attribute__((long_call)) AtkCanceller_UnableToUseMove(void);
bool8 __attribute__((long_call)) sub_80423F4(u8 bank, u8 r1, u8 r2);
u8 __attribute__((long_call)) CastformDataTypeChange(u8 bank);
u8 __attribute__((long_call)) AbilityBattleEffects(u8 caseID, u8 bank, u8 ability, u8 special, u16 moveArg);
void __attribute__((long_call)) BattleScriptExecute(const u8* BS_ptr);
void __attribute__((long_call)) BattleScriptPushCursorAndCallback(const u8* BS_ptr);
//u8 __attribute__((long_call)) ItemBattleEffects(u8 caseID, u8 bank, bool8 moveTurn);
void __attribute__((long_call)) ClearFuryCutterDestinyBondGrudge(u8 bank);
void __attribute__((long_call)) HandleAction_RunBattleScript(void);
u8 __attribute__((long_call)) GetMoveTarget(u16 move, u8 useMoveTarget);
u8 __attribute__((long_call)) IsMonDisobedient(void);
void __attribute__((long_call)) MarkBufferBankForExecution(u8 battlerId);
*/
