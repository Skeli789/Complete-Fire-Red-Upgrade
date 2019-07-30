#pragma once
#include "../global.h"
#include "../battle.h"

//Exported Functions
bool8 CanKillAFoe(u8 bank);
bool8 CanKnockOut(u8 bankAtk, u8 bankDef);
bool8 Can2HKO(u8 bankAtk, u8 bankDef);
bool8 CanKnockOutAfterHealing(u8 bankAtk, u8 bankDef, u16 healAmount, u8 numHits);
bool8 CanKnockOutWithoutMove(const u16 ignoredMove, const u8 bankAtk, const u8 bankDef);
bool8 MoveKnocksOutGoesFirstWithBestAccuracy(u16 move, u8 bankAtk, u8 bankDef);
bool8 StrongestMoveGoesFirst(u16 move, u8 bankAtk, u8 bankDef);
u8 GetBestDoubleKillingMoveScore(u8 bankAtk, u8 bankDef, u8 bankAtkPartner, u8 bankDefPartner, u16* bestMove);
u8 GetDoubleKillingScore(u16 move, u8 bankAtk, u8 bankDef);
bool8 RangeMoveCanHurtPartner(u16 move, u8 bankAtk, u8 bankAtkPartner);
bool8 MoveKnocksOutXHits(u16 move, u8 bankAtk, u8 bankDef, u8 numHits);
u16 CalcFinalAIMoveDamage(u16 move, u8 bankAtk, u8 bankDef, u8 numHits);
move_t GetStrongestMove(const u8 bankAtk, const u8 bankDef, const bool8 onlySpreadMoves);
bool8 IsStrongestMove(const u16 currentMove, const u8 bankAtk, const u8 bankDef, const bool8 onlySpreadMoves);
bool8 MoveWillHit(u16 move, u8 bankAtk, u8 bankDef);
bool8 MoveWouldHitFirst(u16 move, u16 bankAtk, u16 bankDef);
bool8 WillFaintFromWeatherSoon(u8 bank);
u8 CountBanksPositiveStatStages(u8 bank);
u8 CountBanksNegativeStatStages(u8 bank);
u16 GetTeamSpeedAverage(u8 bank);
u16 GetBattleMonMove(u8 bank, u8 index);
u8 GetAIAbility(u8 bankAtk, u8 bankDef, u16 move);
u8 GetPredictedAIAbility(u8 bankAtk, u8 bankDef);
bool8 IsTrapped(u8 bank, bool8 switching);
bool8 IsTakingSecondaryDamage(u8 bank);
bool8 WillFaintFromSecondaryDamage(u8 bank);
u16 CalcSecondaryEffectChance(u8 bank, u16 move);
bool8 ShouldAIDelayMegaEvolution(u8 bankAtk, u8 bankDef, u16 move);

move_t IsValidMovePrediction(u8 bankAtk, u8 bankDef);
bool8 IsPredictedToSwitch(u8 bankAtk, u8 bankDef);
void StoreMovePrediction(u8 bankAtk, u8 bankDef, u16 move);
void StoreSwitchPrediction(u8 bankAtk, u8 bankDef);
bool8 IsMovePredictionSemiInvulnerable(u8 bankAtk, u8 bankDef);
bool8 IsMovePredictionHealingMove(u8 bankAtk, u8 bankDef);
bool8 IsPredictedToUsePursuitableMove(u8 bankAtk, u8 bankDef);

bool8 DamagingMoveInMoveset(u8 bank);
bool8 PhysicalMoveInMoveset(u8 bank);
bool8 SpecialMoveInMoveset(u8 bank);
bool8 MoveSplitInMoveset(u8 bank, u8 moveSplit);
bool8 MagicCoatableMovesInMoveset(u8 bank);
bool8 HasProtectionMoveInMoveset(u8 bank, u8 checkType);
bool8 MoveTypeInMoveset(u8 bank, u8 moveType);
bool8 DamagingMoveTypeInMoveset(u8 bank, u8 moveType);
bool8 DamagingAllHitMoveTypeInMoveset(u8 bank, u8 moveType);
bool8 HasSnatchableMove(u8 bank);
bool8 MoveEffectInMoveset(u8 moveEffect, u8 bank);
bool8 StatusMoveInMoveset(u8 bank);
bool8 MoveInMovesetWithAccuracyLessThan(u8 bankAtk, u8 bankDef, u8 acc, bool8 ignoreStatusMoves);
bool8 FlinchingMoveInMoveset(u8 bank);
bool8 HealingMoveInMoveset(u8 bank);
bool8 SoundMoveInMoveset(u8 bank);
bool8 MoveThatCanHelpAttacksHitInMoveset(u8 bank);
bool8 OnlyBadMovesLeftInMoveset(u8 bankAtk, u8 bankDef);
u16 TryReplaceMoveWithZMove(u8 bankAtk, u16 move);

bool8 GetHealthPercentage(u8 bank);
bool8 TeamFullyHealedMinusBank(u8 bank);
bool8 AnyStatIsRaised(u8 bank);
bool8 PartyMemberStatused(u8 bank, bool8 checkSoundproof);
move_t ShouldAIUseZMove(u8 bank, u8 moveIndex, u16 move);

void IncreaseViability(s16* viability, u8 amount);
void DecreaseViability(s16* viability, u8 amount);

//Exported Constants
#define AI_THINKING_STRUCT ((struct AI_ThinkingStruct*) (gBattleResources->ai))
#define BATTLE_HISTORY ((struct BattleHistory*) (gBattleResources->battleHistory))
#define INCREASE_VIABILITY(x) (IncreaseViability(&viability, x))
#define DECREASE_VIABILITY(x) (DecreaseViability(&viability, x))
#define MOVE_PREDICTION_SWITCH 0xFFFF

#define CHECK_NO_SPECIAL_PROTECTION 0x0
#define CHECK_QUICK_GUARD 0x1
#define CHECK_WIDE_GUARD 0x2
#define CHECK_CRAFTY_SHIELD 0x4
#define CHECK_MAT_BLOCK 0x8

