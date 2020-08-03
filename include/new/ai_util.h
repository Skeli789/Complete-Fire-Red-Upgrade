#pragma once
#include "../global.h"
#include "../battle.h"

#include "damage_calc.h"

//Exported Functions
bool8 CanKillAFoe(u8 bank);
bool8 CanKnockOut(u8 bankAtk, u8 bankDef);
bool8 GetCanKnockOut(u8 bankAtk, u8 bankDef);
bool8 Can2HKO(u8 bankAtk, u8 bankDef);
bool8 GetCan2HKO(u8 bankAtk, u8 bankDef);
bool8 CanKnockOutAfterHealing(u8 bankAtk, u8 bankDef, u16 healAmount, u8 numHits);
bool8 CanKnockOutWithoutMove(const u16 ignoredMove, const u8 bankAtk, const u8 bankDef, const bool8 ignoreFutureAttacks);
bool8 MoveKnocksOutPossiblyGoesFirstWithBestAccuracy(u16 move, u8 bankAtk, u8 bankDef, bool8 checkGoingFirst);
bool8 IsWeakestContactMoveWithBestAccuracy(u16 move, u8 bankAtk, u8 bankDef);
bool8 StrongestMoveGoesFirst(u16 move, u8 bankAtk, u8 bankDef);
bool8 CanKnockOutFromParty(struct Pokemon* monAtk, u8 bankDef, struct DamageCalc* damageData);
void UpdateBestDoubleKillingMoveScore(u8 bankAtk, u8 bankDef, u8 bankAtkPartner, u8 bankDefPartner, s8 bestMoveScores[MAX_BATTLERS_COUNT], u16* bestMove);
u8 GetDoubleKillingScore(u16 move, u8 bankAtk, u8 bankDef);
void TryRemoveDoublesKillingScore(u8 bankAtk, u8 bankDef, u16 chosenMove);
bool8 RangeMoveCanHurtPartner(u16 move, u8 bankAtk, u8 bankAtkPartner);
bool8 MoveKnocksOutXHits(u16 move, u8 bankAtk, u8 bankDef, u8 numHits);
bool8 MoveKnocksOutXHitsFromParty(u16 move, struct Pokemon* monAtk, u8 bankDef, u8 numHits, struct DamageCalc* damageData);
u16 CalcFinalAIMoveDamage(u16 move, u8 bankAtk, u8 bankDef, u8 numHits, struct DamageCalc* damageData);
u32 GetFinalAIMoveDamage(u16 move, u8 bankAtk, u8 bankDef, u8 numHits, struct DamageCalc* damageData);
u16 CalcFinalAIMoveDamageFromParty(u16 move, struct Pokemon* monAtk, u8 bankDef, u8 numHits, struct DamageCalc* damageData);
move_t CalcStrongestMove(const u8 bankAtk, const u8 bankDef, const bool8 onlySpreadMoves);
bool8 IsStrongestMove(const u16 currentMove, const u8 bankAtk, const u8 bankDef);
u16 GetStrongestMove(const u8 bankAtk, const u8 bankDef);
bool8 MoveWillHit(u16 move, u8 bankAtk, u8 bankDef);
bool8 MoveWouldHitFirst(u16 move, u16 bankAtk, u16 bankDef);
bool8 MoveWouldHitBeforeOtherMove(u16 moveAtk, u8 bankAtk, u16 moveDef, u8 bankDef);
bool8 IsUsefulToFlinchTarget(u8 bankDef);
bool8 IsBankAsleep(u8 bank);
bool8 IsBankIncapacitated(u8 bank);
bool8 WillFaintFromWeatherSoon(u8 bank);
bool8 WillTakeSignificantDamageFromEntryHazards(u8 bank, u8 healthFraction);
u8 CountBanksPositiveStatStages(u8 bank);
u8 CountBanksNegativeStatStages(u8 bank);
u16 GetTeamSpeedAverage(u8 bank);
u16 GetPokemonOnSideSpeedAverage(u8 bank);
u16 GetBattleMonMove(u8 bank, u8 index);
u8 GetAIAbility(u8 bankAtk, u8 bankDef, u16 move);
u8 GetPredictedAIAbility(u8 bankAtk, u8 bankDef);
u16 GetAIChosenMove(u8 bankAtk, u8 bankDef);
bool8 IsTrapped(u8 bank, bool8 switching);
bool8 BankHasMonToSwitchTo(u8 bank);
bool8 IsTakingSecondaryDamage(u8 bank);
bool8 WillFaintFromSecondaryDamage(u8 bank);
u16 CalcSecondaryEffectChance(u8 bank, u16 move);
u16 CalcAIAccuracy(u16 move, u8 bankAtk, u8 bankDef);
bool8 ShouldAIDelayMegaEvolution(u8 bankAtk, u8 bankDef, u16 move);

bool8 BadIdeaToPutToSleep(u8 bankDef, u8 bankAtk);
bool8 BadIdeaToPoison(u8 bankDef, u8 bankAtk);
bool8 GoodIdeaToPoisonSelf(u8 bankAtk);
bool8 BadIdeaToParalyze(u8 bankDef, u8 bankAtk);
bool8 GoodIdeaToParalyzeSelf(u8 bankAtk);
bool8 BadIdeaToBurn(u8 bankDef, u8 bankAtk);
bool8 GoodIdeaToBurnSelf(u8 bankAtk);
bool8 BadIdeaToFreeze(u8 bankDef, u8 bankAtk);

bool8 GoodIdeaToLowerAttack(u8 bankDef, u8 bankAtk, u16 move);
bool8 GoodIdeaToLowerDefense(u8 bankDef, u8 bankAtk, u16 move);
bool8 GoodIdeaToLowerSpAtk(u8 bankDef, u8 bankAtk, u16 move);
bool8 GoodIdeaToLowerSpDef(u8 bankDef, u8 bankAtk, u16 move);
bool8 GoodIdeaToLowerSpeed(u8 bankDef, u8 bankAtk, u16 move);
bool8 GoodIdeaToLowerAccuracy(u8 bankDef, u8 bankAtk, u16 move);
bool8 GoodIdeaToLowerEvasion(u8 bankDef, u8 bankAtk, u16 move);

move_t IsValidMovePrediction(u8 bankAtk, u8 bankDef);
bool8 IsPredictedToSwitch(u8 bankAtk, u8 bankDef);
void StoreMovePrediction(u8 bankAtk, u8 bankDef, u16 move);
void StoreSwitchPrediction(u8 bankAtk, u8 bankDef);
bool8 IsMovePredictionSemiInvulnerable(u8 bankAtk, u8 bankDef);
bool8 IsMovePredictionHealingMove(u8 bankAtk, u8 bankDef);
bool8 IsPredictedToUsePursuitableMove(u8 bankAtk, u8 bankDef);
bool8 IsMovePredictionPhazingMove(u8 bankAtk, u8 bankDef);
bool8 CanMovePredictionProtectAgainstMove(u8 bankAtk, u8 bankDef, u16 move);

bool8 DamagingMoveInMoveset(u8 bank);
bool8 PhysicalMoveInMoveset(u8 bank);
bool8 SpecialMoveInMoveset(u8 bank);
bool8 MoveSplitInMoveset(u8 bank, u8 moveSplit);
bool8 PhysicalMoveInMonMoveset(struct Pokemon* mon, u8 moveLimitations);
bool8 SpecialMoveInMonMoveset(struct Pokemon* mon, u8 moveLimitations);
bool8 MagicCoatableMovesInMoveset(u8 bank);
bool8 HasProtectionMoveInMoveset(u8 bank, u8 checkType);
bool8 MoveTypeInMoveset(u8 bank, u8 moveType);
bool8 DamagingMoveTypeInMoveset(u8 bank, u8 moveType);
bool8 DamagingAllHitMoveTypeInMoveset(u8 bank, u8 moveType);
bool8 DamagingSpreadMoveInMoveset(u8 bank);
bool8 HasSnatchableMove(u8 bank);
bool8 MoveEffectInMoveset(u8 moveEffect, u8 bank);
bool8 StatusMoveInMoveset(u8 bank);
bool8 MoveInMovesetWithAccuracyLessThan(u8 bankAtk, u8 bankDef, u8 acc, bool8 ignoreStatusMoves);
bool8 FlinchingMoveInMoveset(u8 bank);
bool8 HealingMoveInMoveset(u8 bank);
bool8 HealingMoveInMonMoveset(struct Pokemon* mon);
bool8 SoundMoveInMoveset(u8 bank);
bool8 MoveThatCanHelpAttacksHitInMoveset(u8 bank);
bool8 DamagingMoveThaCanBreakThroughSubstituteInMoveset(u8 bankAtk, u8 bankDef);
bool8 ContactMovesThatAffectTargetInMoveset(u8 bankAtk, u8 bankDef);
bool8 UnfreezingMoveInMoveset(u8 bank);
bool8 SleepMoveInMovesetWithLowAccuracy(u8 bankAtk, u8 bankDef);
bool8 OnlyBadMovesLeftInMoveset(u8 bankAtk, u8 bankDef);
u16 TryReplaceMoveWithZMove(u8 bankAtk, u8 bankDef, u16 move);
u8 GetAIMoveEffectForMaxMove(u16 move, u8 bankAtk, u8 bankDef);

bool8 GetHealthPercentage(u8 bank);
bool8 TeamFullyHealedMinusBank(u8 bank);
bool8 AnyStatIsRaised(u8 bank);
bool8 AnyUsefulStatIsRaised(u8 bank);
bool8 AnyUsefulOffseniveStatIsRaised(u8 bank);
bool8 PartyMemberStatused(u8 bank, bool8 checkSoundproof);
bool8 ShouldAIUseZMove(u8 bank, u8 moveIndex, u16 move);
void CalcAIDynamaxMon(u8 bank);
void CalcShouldAIDynamax(u8 bankAtk, u8 bankDef);
bool8 ShouldAIDynamax(u8 bankAtk, u8 bankDef);
u8 AdjustMoveLimitationFlagsForAI(u8 bankAtk, u8 bankDef);

void IncreaseViability(s16* viability, u8 amount);
void DecreaseViability(s16* viability, u16 amount);

#define CAN_SWITCH_OUT(bank) (!IsTrapped(bank, TRUE) && ((IS_SINGLE_BATTLE && ViableMonCountFromBankLoadPartyRange(bank) >= 2) \
													  || (IS_DOUBLE_BATTLE && ViableMonCountFromBankLoadPartyRange(bank) >= 3)))

//Exported Constants
#define AI_THINKING_STRUCT ((struct AI_ThinkingStruct*) (gBattleResources->ai))
#define INCREASE_VIABILITY(x) (IncreaseViability(&viability, x))
#define DECREASE_VIABILITY(x) (DecreaseViability(&viability, x))
#define MOVE_PREDICTION_SWITCH 0xFFFF

#define CHECK_NO_SPECIAL_PROTECTION 0x0
#define CHECK_QUICK_GUARD 0x1
#define CHECK_WIDE_GUARD 0x2
#define CHECK_CRAFTY_SHIELD 0x4
#define CHECK_MAT_BLOCK 0x8


#define DOUBLES_INCREASE_HIT_FOE 4
#define DOUBLES_INCREASE_KO_FOE 2
#define DOUBLES_INCREASE_STRONGEST_MOVE 1

#define DOUBLES_DECREASE_HIT_PARTNER 3
#define DOUBLES_DECREASE_DESTINY_BOND 2

#define BEST_DOUBLES_KO_SCORE (DOUBLES_INCREASE_HIT_FOE + DOUBLES_INCREASE_HIT_FOE + DOUBLES_INCREASE_KO_FOE + DOUBLES_INCREASE_KO_FOE)
