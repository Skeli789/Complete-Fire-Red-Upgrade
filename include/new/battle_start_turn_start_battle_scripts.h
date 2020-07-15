#pragma once

#include "../global.h"

/**
 * \file battle_start_turn_start_battle_scripts.h
 * \brief Contains declarations for battle scripts usually called at the beginning of
 *		  battle of before Pokemon have executed their actions for the turn.
 */

extern const u8 BattleScript_AirBalloonFloat[];
extern const u8 BattleScript_Totem[];
extern const u8 BattleScript_TotemRet[];
extern const u8 BattleScript_TotemOmniboost[];
extern const u8 BattleScript_TotemOmniboostRet[];
extern const u8 BattleScript_ElectricTerrainBattleBegin[];
extern const u8 BattleScript_GrassyTerrainBattleBegin[];
extern const u8 BattleScript_MistyTerrainBattleBegin[];
extern const u8 BattleScript_PsychicTerrainBattleBegin[];
extern const u8 BattleScript_BeakBlastSetUp[];
extern const u8 BattleScript_ShellTrapSetUp[];
extern const u8 BattleScript_FocusPunchSetUp[];
extern const u8 BattleScript_QuickClaw[];
extern const u8 BattleScript_NoTargetMoveFailed[];
extern const u8 BattleScript_CamomonsTypeRevealEnd3[];
extern const u8 BattleScript_CamomonsTypeRevealRet[];
extern const u8 BattleScript_RaidBattleStart[];
extern const u8 BattleScript_RaidBattleStorm[];
extern const u8 BattleScript_DynamaxEnergySwirl[];

extern const u8 gText_CamomonsTypeReveal[];
extern const u8 gText_CamomonsTypeRevealDualType[];
extern const u8 gText_RaidBattleReveal[];
extern const u8 gText_DynamaxUsable[];