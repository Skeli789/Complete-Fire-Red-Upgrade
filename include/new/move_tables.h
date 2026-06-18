#pragma once

#include "../global.h"

/**
 * \file move_tables.h
 * \brief Contains declarations for many lists of moves.
 */

struct SpecialMoveFlags
{
	bool8 gGravityBannedMoves : 1;
	bool8 gMeFirstBannedMoves : 1;
	bool8 gCopycatBannedMoves : 1;
	bool8 gInstructBannedMoves : 1;
	bool8 gMetronomeBannedMoves : 1;
	bool8 gAssistBannedMoves : 1;
	bool8 gMimicBannedMoves : 1;
	bool8 gSleepTalkBannedMoves : 1;
	bool8 gParentalBondBannedMoves : 1;
	bool8 gSkyBattleBannedMoves : 1;
	bool8 gRaidBattleBannedMoves : 1;
	bool8 gRaidBattleBannedRaidMonMoves : 1;
	bool8 gDynamaxBannedMoves : 1;
	bool8 gSheerForceBoostedMoves : 1;
	bool8 gRecklessBoostedMoves : 1;
	bool8 gPunchingMoves : 1;
	bool8 gPulseAuraMoves : 1;
	bool8 gBitingMoves : 1;
	bool8 gBallBombMoves : 1;
	bool8 gDanceMoves : 1;
	bool8 gPowderMoves : 1;
	bool8 gSoundMoves : 1;
	bool8 gTailMoves : 1;
	bool8 gDrillMoves : 1;
	bool8 gSubstituteBypassMoves : 1;
	bool8 gTypeChangeExceptionMoves : 1;
	bool8 gIgnoreStatChangesMoves : 1;
	bool8 gHighCriticalChanceMoves : 1;
	bool8 gAlwaysCriticalMoves : 1;
	bool8 gMovesThatCallOtherMoves : 1;
	bool8 gAlwaysHitWhenMinimizedMoves : 1;
	bool8 gMoldBreakerMoves : 1;
	bool8 gFlinchChanceMoves : 1;
	bool8 gMovesCanUnfreezeAttacker : 1;
	bool8 gMovesCanUnfreezeTarget : 1;
	bool8 gMovesThatChangePhysicality : 1;
	bool8 gDoubleDamageOnStatus : 1;
	bool8 gTwoToFiveStrikesMoves : 1;
	bool8 gTwoStrikesMoves : 1;
	bool8 gPercent25RecoilMoves : 1;
	bool8 gPercent33RecoilMoves : 1;
	bool8 gPercent50RecoilMoves : 1;
	bool8 gHalfMaxHealthRecoilMoves : 1;
	bool8 gIgnoreInAirMoves : 1;
	bool8 gIgnoreUndergoundMoves : 1;
	bool8 gIgnoreUnderwaterMoves : 1;
	bool8 gAlwaysHitInRainMoves : 1;
	bool8 gSpecialAttackPhysicalDamageMoves : 1;
	bool8 gSpecialWholeFieldMoves : 1;
	bool8 gAromaVeilProtectedMoves : 1;
	bool8 gMovesThatLiftProtectTable : 1;
	bool8 gRandomizerBanTable : 1;
	bool8 gBuffMoves : 1;
	bool8 gSlicingMoves : 1;
	bool8 gWindMoves : 1;
	bool8 gTenStrikesMoves : 1;
};

extern const struct SpecialMoveFlags gSpecialMoveFlags[];
extern const u8 gMoveEffectsThatIgnoreWeaknessResistance[];
