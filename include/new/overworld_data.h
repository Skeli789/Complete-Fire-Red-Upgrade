#pragma once

#define SCRCMD_TRAINERBATTLE 0x5C

#define sOrdinaryBattleParams (struct TrainerBattleParameter*) 0x83C6900 //Battle Type 0 & 5
#define sContinueScriptBattleParams (struct TrainerBattleParameter*) 0x83C6948 //Battle Type 1 & 2
#define sOrdinaryNoIntroBattleParams (struct TrainerBattleParameter*) 0x83C69D8 //Battle Type 3
#define sDoubleBattleParams (struct TrainerBattleParameter*) 0x83C6990 //Battle Type 4 & 7
#define sContinueScriptDoubleBattleParams (struct TrainerBattleParameter*) 0x83C6A68 //Batttle Type 6 & 8
#define sOakTutorialParams (struct TrainerBattleParameter*) 0x83C6A20 //Batttle Type 9

#define EventScript_TryDoNormalTrainerBattle (u8*) 0x81A4EC1
#define EventScript_DoTrainerBattle (u8*) 0x81A4F21
#define EventScript_TryDoDoubleTrainerBattle (u8*) 0x81A4EE9
#define EventScript_TryDoRematchBattle (u8*) 0x81A4F3E
#define EventScript_TryDoDoubleRematchBattle (u8*) 0x81A4F73

#define EventScript_VSSeeker (const u8*) 0x81A8CED
#define EventScript_Poison (const u8*) 0x81A8DFD
#define EventScript_EggHatch (const u8*) 0x81BF546

extern u8 EventScript_SetUpNPCSpotting[];
extern u8 Script_TrainerSpottedMulti[];
extern u8 EventScript_DoTwoOpponentBattle[];
extern u8 EventScript_TryDoTwoOpponentBattle[];
extern u8 SystemScript_PoisonSurvial[];
extern u8 EventScript_RepelWoreOff[];
extern u8 EventScript_BwRepelWoreOff[];