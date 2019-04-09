#pragma once
#include "../../src/defines.h"
#include "../../src/defines_battle.h"

enum
{
	CANCELLER_FLAGS,
	CANCELLER_ASLEEP,
	CANCELLER_FROZEN,
	CANCELLER_TRUANT,
	CANCELLER_RECHARGE,
	CANCELLER_FLINCH,
	CANCELLER_GRAVITY,
	CANCELLER_DISABLED,
	CANCELLER_HEAL_BLOCKED,
	CANCELLER_THROAT_CHOP,
	CANCELLER_TAUNTED,
	CANCELLER_IMPRISONED,
	CANCELLER_CONFUSED,
	CANCELLER_PARALYSED,
	CANCELLER_GHOST,
	CANCELLER_IN_LOVE,
	CANCELLER_STANCE_CHANGE,
	CANCELLER_BIDE,
	CANCELLER_THAW,
	CANCELLER_Z_MOVES,
	CANCELLER_GRAVITY_Z_MOVES,
	
//Called Attacks Start Here
	CANCELLER_GRAVITY_2,
	CANCELLER_SKY_BATTLE,
	CANCELLER_HEAL_BLOCKED_2,
	CANCELLER_THROAT_CHOP_2,
	CANCELLER_STANCE_CHANGE_2,
	CANCELLER_NATURAL_GIFT,
	CANCELLER_DANCER,
	CANCELLER_POWDER,
	CANCELLER_PRIMAL_WEATHER,
	CANCELLER_PSYCHIC_TERRAIN,
	CANCELLER_PRANKSTER,
	CANCELLER_MULTIHIT_MOVES,
	CANCELLER_MULTI_TARGET_MOVES,
	CANCELLER_END,
};

#define BattleScript_NoPPForMove (u8*) 0x81D8EA8
#define BattleScript_MagicCoatBounce (u8*) 0x81D8FAA
#define BattleScript_SnatchedMove (u8*) 0x81D8FC2
#define BattleScript_TookAttack (u8*) 0x81D938D

#define BattleScript_MoveUsedIsAsleep (u8*) 0x81D9029
#define BattleScript_MoveUsedWokeUp (u8*) 0x81D9036
#define BattleScript_MoveUsedIsFrozen (u8*) 0x81D9080
#define BattleScript_MoveUsedUnfroze (u8*) 0x81D908D
#define BattleScript_MoveUsedLoafingAround (u8*) 0x81D94F2
#define BattleScript_MoveUsedMustRecharge (u8*) 0x81D7342
#define BattleScript_MoveUsedIsDisabled (u8*) 0x81D8C4F
#define BattleScript_MoveUsedIsTaunted (u8*) 0x81D8ECA
#define BattleScript_MoveUsedIsImprisoned (u8*) 0x81D8F94
#define BattleScript_MoveUsedIsConfused (u8*) 0x81D90D3
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