#pragma once
#include "../global.h"
#include "../battle.h"

//Exported Functions
u8 PredictBankFightingStyle(u8 bank);
bool8 IsClassSweeper(u8 class);
bool8 IsClassKillerSweeper(u8 class);
bool8 IsClassSetupSweeper(u8 class);
bool8 IsClassStatusSweeper(u8 class);
bool8 IsClassStall(u8 class);
bool8 IsClassTeamSupport(u8 class);
bool8 IsClassBatonPass(u8 class);
bool8 IsClassScreener(u8 class);
bool8 IsClassCleric(u8 class);
bool8 IsClassPhazer(u8 class);
u8 PredictBankFightingStyle(u8 bank);

bool8 ShouldTrap(u8 bankAtk, u8 bankDef, u16 move, u8 class);
bool8 ShouldRecover(u8 bankAtk, u8 bankDef, u16 move, u8 class);
u8 ShouldProtect(u8 bankAtk, u8 bankDef, u16 move, u8 class);
bool8 ShouldPhaze(u8 bankAtk, u8 bankDef, u16 move, u8 class);
bool8 ShouldUseWishAromatherapy(u8 bankAtk, u8 bankDef, u16 move, u8 class);
bool8 ShouldSetUpScreens(u8 bankAtk, u8 bankDef, u16 move, u8 class);
u8 BankLikelyToUseMoveSplit(u8 bank, u8 class);
u16 GetAmountToRecoverBy(u8 bankAtk, u8 bankDef, u16 move);

void IncreaseStatusViability(s16* originalViability, u8 class, u8 boost, u8 bankAtk, u8 bankDef);
void IncreaseStatViability(s16* originalViability, u8 class, u8 boost, u8 bankAtk, u8 bankDef, u16 move, u8 stat, u8 statLimit);
void IncreaseSubstituteViability(s16* viability, u8 class, u8 bankAtk, u8 bankDef);
void IncreaseEntryHazardsViability(s16* viability, u8 class, u8 bankAtk, u8 bankDef, u16 move);

//Exported Constants
enum ProtectQueries
{
	DONT_PROTECT,
	USE_PROTECT,
	USE_STATUS_THEN_PROTECT,
};

#define INCREASE_STATUS_VIABILITY(x) (IncreaseStatusViability(&viability, class, x, bankAtk, bankDef))
#define INCREASE_STAT_VIABILITY(stat, statLimit, viabilityBoost) (IncreaseStatViability(&viability, class, viabilityBoost, bankAtk, bankDef, move, stat, statLimit))
