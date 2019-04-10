#pragma once

#include "global.h"

#define TRAINER_BATTLE_SINGLE                           0
#define TRAINER_BATTLE_CONTINUE_SCRIPT_NO_MUSIC         1
#define TRAINER_BATTLE_CONTINUE_SCRIPT                  2
#define TRAINER_BATTLE_SINGLE_NO_INTRO_TEXT             3
#define TRAINER_BATTLE_DOUBLE                           4
#define TRAINER_BATTLE_REMATCH                          5
#define TRAINER_BATTLE_CONTINUE_SCRIPT_DOUBLE           6
#define TRAINER_BATTLE_REMATCH_DOUBLE                   7
#define TRAINER_BATTLE_CONTINUE_SCRIPT_DOUBLE_NO_MUSIC  8
#define TRAINER_BATTLE_OAK_TUTORIAL                     9
#define TRAINER_BATTLE_TAG								10
#define TRAINER_BATTLE_TWO_OPPONENTS					11


void __attribute__((long_call)) BattleSetup_StartScriptedWildBattle(void);
u8 __attribute__((long_call)) BattleSetup_GetTerrainId(void);
u8 __attribute__((long_call)) *BattleSetup_ConfigureTrainerBattle(const u8 *data);
void __attribute__((long_call)) BattleSetup_StartBattlePikeWildBattle(void);
void __attribute__((long_call)) BattleSetup_StartWildBattle(void);
void __attribute__((long_call)) BattleSetup_StartRoamerBattle(void);

u8 __attribute__((long_call)) HasTrainerAlreadyBeenFought(u16);
void __attribute__((long_call)) trainer_flag_set(u16);
void __attribute__((long_call)) trainer_flag_clear(u16);
void __attribute__((long_call)) BattleSetup_StartTrainerBattle(void);
u8 __attribute__((long_call)) *BattleSetup_GetScriptAddrAfterBattle(void);
u8 __attribute__((long_call)) *BattleSetup_GetTrainerPostBattleScript(void);
void __attribute__((long_call)) sub_80803FC(void);

