#pragma once

#include "../global.h"

/**
 * \file ai_advanced.h
 * \brief Contains functions relating to advanced AI analysis of Pokemon in battle.
 */

//Exported Functions
u8 GetBankFightingStyle(u8 bank);
u8 PredictBankFightingStyle(u8 bank);
bool8 IsClassSweeper(u8 class);
bool8 IsClassKillerSweeper(u8 class);
bool8 IsClassSetupSweeper(u8 class);
bool8 IsClassStatusSweeper(u8 class);
bool8 IsClassStall(u8 class);
bool8 IsClassTeamSupport(u8 class);
bool8 IsClassBatonPass(u8 class);
bool8 IsClassScreener(u8 class);
bool8 IsClassSupportScreener(u8 class);
bool8 IsClassCleric(u8 class);
bool8 IsClassPhazer(u8 class);
bool8 IsClassDoublesPhazer(u8 class);
bool8 IsClassEntryHazards(u8 class);
bool8 IsClassDoublesAllOutAttacker(u8 class);
bool8 IsClassDoublesSetupAttacker(u8 class);
bool8 IsClassDoublesTrickRoomSetup(u8 class);
bool8 IsClassDoublesTrickRoomer(u8 class);
bool8 IsClassDoublesUtility(u8 class);
bool8 IsClassDoublesTeamSupport(u8 class);
bool8 IsClassDoublesTotalTeamSupport(u8 class);
bool8 IsClassDoublesSpecific(u8 class);
bool8 IsClassDoublesAttacker(u8 class);
bool8 IsClassDamager(u8 class);
bool8 IsClassGoodToTaunt(u8 class);
u8 PredictBankFightingStyle(u8 bank);
u8 PredictFightingStyle(const u16* const moves, const u8 ability, const u8 itemEffect, const u8 bank);

bool8 ShouldPrioritizeKOingFoesDoubles(u8 bank);
bool8 ShouldPrioritizeMostDamageDoubles(u8 bank);
bool8 ShouldPrioritizeDangerousTarget(u8 bank);
bool8 ShouldTrap(u8 bankAtk, u8 bankDef, u16 move, u8 class);
bool8 ShouldRecover(u8 bankAtk, u8 bankDef, u16 move);
u8 ShouldProtect(u8 bankAtk, u8 bankDef, u16 move);
bool8 ShouldPhaze(u8 bankAtk, u8 bankDef, u16 move, u8 class);
bool8 ShouldUseWishAromatherapy(u8 bankAtk, u8 bankDef, u16 move, u8 class);
bool8 ShouldSetUpScreens(u8 bankAtk, u8 bankDef, u16 move);
bool8 ShouldUseFakeOut(u8 bankAtk, u8 bankDef, u8 defAbility);
bool8 ShouldPivot(u8 bankAtk, u8 bankDef, u16 move, u8 class);
bool8 ShouldCourtChange(u8 bankAtk, u8 bankDef);
u8 BankLikelyToUseMoveSplit(u8 bank, u8 class);
u16 GetAmountToRecoverBy(u8 bankAtk, u8 bankDef, u16 move);
bool8 MoveSplitOnTeam(u8 bank, u8 split);

void IncreaseStatusViability(s16* viability, u8 class, u8 boost, u8 bankAtk, u8 bankDef);
void IncreaseStatViability(s16* viability, u8 class, u8 boost, u8 bankAtk, u8 bankDef, u16 move, u8 stat, u8 statLimit);
void IncreaseSleepViability(s16* viability, u8 class, u8 bankAtk, u8 bankDef, u16 move);
void IncreaseFreezeViability(s16* originalViability, u8 class, u8 bankAtk, u8 bankDef);
void IncreaseSubstituteViability(s16* viability, u8 class, u8 bankAtk, u8 bankDef);
void IncreaseEntryHazardsViability(s16* viability, u8 class, u8 bankAtk, u8 bankDef, u16 move);
void IncreaseFakeOutViability(s16* viability, u8 class, u8 bankAtk, u8 bankDef, u16 move);
void IncreasePivotViability(s16* viability, u8 class, u8 bankAtk, u8 bankDef, u8 pivotType);
void IncreaseViabilityForSlowKOMove(s16* originalViability, u8 class, u8 bankAtk, u8 bankDef);
void IncreaseFoeProtectionViability(s16* viability, u8 class, u8 bankAtk, u8 bankDef);
void IncreaseAllyProtectionViability(s16* viability, u8 class);
void IncreaseTeamProtectionViability(s16* viability, u8 class);
void IncreaseTailwindViability(s16* viability, u8 class, u8 bankAtk, u8 bankDef);
void IncreaseHelpingHandViability(s16* originalViability, u8 class);
void IncreaseHealPartnerViability(s16* originalViability, u8 class, u8 partner);
void IncreasePsychUpViability(s16* originalViability, u8 class, u8 bankAtk, u8 bankDef);
bool8 IncreaseViabilityForSpeedControl(s16* originalViability, u8 class, u8 bankAtk, u8 bankDef);
void IncreaseDoublesDamageViability(s16* originalViability, u8 class, u8 bankAtk, u8 bankDef, u16 move);
void IncreaseDoublesDamageViabilityToScore(s16* originalViability, u8 class, u8 score, u8 bankAtk, u8 bankDef);

//Exported Constants
enum ProtectQueries
{
	DONT_PROTECT,
	USE_PROTECT,
	USE_STATUS_THEN_PROTECT,
	PROTECT_FROM_ALLIES,
	PROTECT_FROM_FOES,
};

enum PivotQueries
{
	DONT_PIVOT,
	CAN_TRY_PIVOT,
	PIVOT,
	PIVOT_IMMEDIATELY,
};

#define INCREASE_STATUS_VIABILITY(x) (IncreaseStatusViability(&viability, class, x, bankAtk, bankDef))
#define INCREASE_STAT_VIABILITY(stat, statLimit, viabilityBoost) (IncreaseStatViability(&viability, class, viabilityBoost, bankAtk, bankDef, move, stat, statLimit))
