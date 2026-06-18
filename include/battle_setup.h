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
#define TRAINER_BATTLE_MULTI                            10
#define TRAINER_BATTLE_TWO_OPPONENTS                    11
#define TRAINER_BATTLE_TAG                              12
#define TRAINER_BATTLE_SINGLE_SCALED                    13
#define TRAINER_BATTLE_DOUBLE_SCALED                    14
#define TRAINER_BATTLE_SINGLE_NO_INTRO_TEXT_SCALED      15
#define TRAINER_BATTLE_REMATCH_TWO_OPPONENTS            16

struct TrainerBattleParameter
{
    void* varPtr;
    u8 ptrType;
};

//void __attribute__((long_call)) InitTrainerBattleVariables(void);
void __attribute__((long_call)) TrainerBattleLoadArgs(const struct TrainerBattleParameter* specs, const u8 *data);
u16 __attribute__((long_call)) TrainerBattleLoadArg16(const u8 *ptr);
u8 __attribute__((long_call)) TrainerBattleLoadArg8(const u8 *ptr);
void __attribute__((long_call)) SetMapVarsToTrainer(void);
//u8* __attribute__((long_call)) BattleSetup_ConfigureTrainerBattle(u8 *data);
void __attribute__((long_call)) TrainerWantsBattle(u8 trainerEventObjId, const u8 *trainerScript);
//u32 __attribute__((long_call)) GetTrainerFlagFromScriptPointer(const u8 *data);
void CB2_EndTrainerBattle(void);
bool8 __attribute__((long_call)) GetTrainerApproachDistance(struct EventObject* trainerObj);
void __attribute__((long_call)) TrainerApproachPlayer(struct EventObject* trainerObj, u8 range);
u16 __attribute__((long_call)) GetRematchTrainerId(u16 opponentId);
u8 __attribute__((long_call)) GetBattleTransitionTypeByMap(void);
u16 __attribute__((long_call)) GetSumOfEnemyPartyLevel(u16 opponentId, u8 numMons);
u16 __attribute__((long_call)) GetSumOfPlayerPartyLevel(u8 numMons);
bool8 __attribute__((long_call)) HasTrainerBeenFought(u16 trainerId);
bool32 __attribute__((long_call)) IsThisTrainerRematchable(u32);

/*
void __attribute__((long_call)) BattleSetup_StartScriptedWildBattle(void);
u8 __attribute__((long_call)) BattleSetup_GetTerrainId(void);
u8* __attribute__((long_call)) BattleSetup_ConfigureTrainerBattle(const u8 *data);
void __attribute__((long_call)) BattleSetup_StartBattlePikeWildBattle(void);
void __attribute__((long_call)) BattleSetup_StartWildBattle(void);
void __attribute__((long_call)) BattleSetup_StartRoamerBattle(void);

u8 __attribute__((long_call)) HasTrainerAlreadyBeenFought(u16);
void __attribute__((long_call)) trainer_flag_set(u16);
void __attribute__((long_call)) trainer_flag_clear(u16);
void __attribute__((long_call)) BattleSetup_StartTrainerBattle(void);
u8* __attribute__((long_call)) BattleSetup_GetScriptAddrAfterBattle(void);
u8* __attribute__((long_call)) BattleSetup_GetTrainerPostBattleScript(void);
void __attribute__((long_call)) sub_80803FC(void);
*/
