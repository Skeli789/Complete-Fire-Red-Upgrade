#pragma once

#include "../global.h"

/**
 * \file attackcanceler_battle_scripts.h
 * \brief Contains declarations for battle scripts usually at the beginning of
*		  a move's execution. Many times used to stop the execution.
 */

extern u8 BattleScript_MagicBounce[];
extern u8 BattleScript_MoveUsedFlinched[]; //0x81D90B1
extern u8 BattleScript_MoveUsedDevolvedForgot[];
extern u8 BattleScript_MoveUsedIsConfused[]; //0x81D90D3
extern u8 BattleScript_MoveUsedLoafingAround[]; //0x81D94F2
extern u8 BattleScript_MoveUsedGravityPrevents[];
extern u8 BattleScript_MoveUsedHealBlockPrevents[];
extern u8 BattleScript_MoveUsedThroatChopPrevents[];
extern u8 BattleScript_MoveUsedFailedPrimalWeather[];
extern u8 BattleScript_MoveUsedPsychicTerrainPrevents[];
extern u8 BattleScript_MoveUsedPowderPrevents[];
extern u8 BattleScript_StanceChangeToBlade[];
extern u8 BattleScript_StanceChangeToShield[];
extern u8 BattleScript_ZMoveActivateStatus[];
extern u8 BattleScript_ZMoveActivateDamaging[];
extern u8 BattleScript_DarkTypePreventsPrankster[];
extern u8 BattleScript_MoveUsedSkyBattlePrevents[];
extern u8 BattleScript_DampStopsExplosion[];
extern u8 BattleScript_TookAttack[];

#define BattleScript_NoPPForMove (u8*) 0x81D8EA8
#define BattleScript_MagicCoatBounce (u8*) 0x81D8FAA
#define BattleScript_SnatchedMove (u8*) 0x81D8FC2

#define BattleScript_MoveUsedIsAsleep (u8*) 0x81D9029
#define BattleScript_MoveUsedWokeUp (u8*) 0x81D9036
#define BattleScript_MoveUsedIsFrozen (u8*) 0x81D9080
#define BattleScript_MoveUsedUnfroze (u8*) 0x81D908D
#define BattleScript_MoveUsedMustRecharge (u8*) 0x81D7342
#define BattleScript_MoveUsedIsDisabled (u8*) 0x81D8C4F
#define BattleScript_MoveUsedIsTaunted (u8*) 0x81D8ECA
#define BattleScript_MoveUsedIsImprisoned (u8*) 0x81D8F94
#define BattleScript_MoveUsedIsConfusedNoMore (u8*) 0x81D9116
#define BattleScript_MoveUsedIsParalyzed (u8*) 0x81D90A1
#define BattleScript_TooScaredToMove (u8*) 0x81D9180
#define BattleScript_GhostGetOut (u8*) 0x81D9192
#define BattleScript_ImmobilizedByLove (u8*) 0x81D914A
#define BattleScript_MoveUsedIsInLove (u8*) 0x81D913D
#define BattleScript_BideStoringEnergy (u8*) 0x81D8BA9
#define BattleScript_BideAttack (u8*) 0x81D8BB4
#define BattleScript_BideNoEnergyToAttack (u8*) 0x81D8BFC
#define BattleScript_IgnoresWhileAsleep (u8*) 0x81D94DA
#define BattleScript_IgnoresAndUsesRandomMove (u8*) 0x81D94EA
#define BattleScript_IgnoresAndFallsAsleep (u8*) 0x81D9504
#define BattleScript_IgnoresAndHitsItself (u8*) 0x81D951B
