#pragma once
#include "../global.h"
#include "../battle.h"

#include "ai_scripts.h"
#include "damage_calc.h"

//Exported Functions
u16 AIRandom(void);
bool8 CanKillAFoe(u8 bank);
bool8 CanKnockOut(u8 bankAtk, u8 bankDef);
bool8 CanKnockOutWithFasterMove(u8 bankAtk, u8 bankDef, u16 defMove);
bool8 Can2HKO(u8 bankAtk, u8 bankDef);
bool8 GetCan2HKO(u8 bankAtk, u8 bankDef);
bool8 CanKnockOutAfterHealing(u8 bankAtk, u8 bankDef, u16 healAmount, u8 numHits, bool8 dealOneHitFirst);
bool8 CanKnockOutWithoutMove(const u16 ignoredMove, const u8 bankAtk, const u8 bankDef, const bool8 ignoreFutureAttacks);
bool8 CanKnockOutWithChipDamage(u8 bankAtk, u8 bankDef);
bool8 Can2HKOWithChipDamage(u8 bankAtk, u8 bankDef);
bool8 MoveKnocksOutPossiblyGoesFirstWithBestAccuracy(u16 move, u8 bankAtk, u8 bankDef, bool8 checkGoingFirst);
bool8 HasMoveThatGoesFirstAndKnocksOut(u8 bankAtk, u8 bankDef);
bool8 IsWeakestContactMoveWithBestAccuracy(u16 move, u8 bankAtk, u8 bankDef);
bool8 StrongestMoveGoesFirst(u16 move, u8 bankAtk, u8 bankDef);
bool8 CanKnockOutFromParty(struct Pokemon* monAtk, u8 bankDef, struct DamageCalc* damageData);
void UpdateBestDoubleKillingMoveScore(u8 bankAtk, u8 bankDef, u8 bankAtkPartner, u8 bankDefPartner, s8 bestMoveScores[MAX_BATTLERS_COUNT], u16* bestMove);
u8 GetDoubleKillingScore(u16 move, u8 bankAtk, u8 bankDef);
bool8 TryRemovePartnerDoublesKillingScore(u8 bankAtk, u8 bankDef, u16 chosenMove, bool8 doSpeedCalc);
void TryRemovePartnerDoublesKillingScoreComplete(u8 bankAtk, u8 bankDef, u16 chosenMove, u32 moveTarget, bool8 doSpeedCalc);
bool8 RangeMoveCanHurtPartner(u16 move, u8 bankAtk, u8 bankAtkPartner);
bool8 MoveKnocksOutXHits(u16 move, u8 bankAtk, u8 bankDef, u8 numHits);
bool8 MoveKnocksOutFromParty(u16 move, struct Pokemon* monAtk, u8 bankDef, struct DamageCalc* damageData);
u16 CalcFinalAIMoveDamage(u16 move, u8 bankAtk, u8 bankDef, u8 numHits, struct DamageCalc* damageData);
u32 GetFinalAIMoveDamage(u16 move, u8 bankAtk, u8 bankDef, u8 numHits, struct DamageCalc* damageData);
u16 GetFinalAIMoveDamageFromParty(u16 move, struct Pokemon* monAtk, u8 bankDef, struct DamageCalc* damageData);
move_t CalcStrongestMove(const u8 bankAtk, const u8 bankDef, const bool8 onlySpreadMoves);
void RecalcStrongestMoveIgnoringMove(const u8 bankAtk, const u8 bankDef, const u16 ignoreMove);
bool8 IsStrongestMove(const u16 currentMove, const u8 bankAtk, const u8 bankDef);
u16 GetStrongestMove(const u8 bankAtk, const u8 bankDef);
bool8 ResistsAllMoves(const u8 bankAtk, const u8 bankDef);
bool8 ResistsAllMovesAtFullHealth(const u8 bankAtk, const u8 bankDef);
void ForceSpecificDamageRecalculation(const u8 bankAtk, const u8 bankDef, const u8 movePos);
void ForceCompleteDamageRecalculation(const u8 bankAtk);
bool8 MoveWillHit(u16 move, u8 bankAtk, u8 bankDef);
bool8 MonMoveWillHit(u16 move, struct Pokemon* monAtk, u8 bankDef);
bool8 MoveWouldHitFirst(u16 move, u16 bankAtk, u16 bankDef);
bool8 MoveWouldHitBeforeOtherMove(u16 moveAtk, u8 bankAtk, u16 moveDef, u8 bankDef);
bool8 IsUsefulToFlinchTarget(u8 bankDef);
bool8 IsBankAsleep(u8 bank);
bool8 IsBankIncapacitated(u8 bank);
bool8 EnduresAHitFromFullHealth(u8 bankDef, u8 defAbility, u8 atkAbility);
bool8 WillFaintFromWeatherSoon(u8 bank);
bool8 WillTakeSignificantDamageFromEntryHazards(u8 bank, u8 healthFraction);
bool8 CanBeFlinched(u8 bankDef, u8 bankAtk, u8 defAbility, u16 move);
bool8 IsCurrentWeatherPartnersWeather(u8 partner, u8 partnerAbility);
u8 CountBanksPositiveStatStages(u8 bank);
u8 CountBanksNegativeStatStages(u8 bank);
u16 GetPokemonOnSideSpeedAverage(u8 bank);
u16 GetTeamMaxSpeed(u8 bank);
bool8 FasterThanEntireTeam(u16 atkSpeed, u8 opposingBank);
bool8 WillBeFasterAfterSpeedDrop(u8 bankAtk, u8 bankDef, u8 reduceBy);
bool8 WillBeFasterAfterMoveSpeedBuff(u8 bankAtk, u8 bankDef, u16 move);

u16 GetBattleMonMove(u8 bank, u8 index);
u8 GetAIAbility(u8 bankAtk, u8 bankDef, u16 move);
u8 GetPredictedAIAbility(u8 bankAtk, u8 bankDef);
u8 GetMonAbilityAfterTrace(struct Pokemon* mon, u8 foe);
u8 TryReplaceImposterAbility(u8 ability, u8 monBank);
bool8 IsDamagingMoveUnusable(u16 move, u8 bankAtk, u8 bankDef);
bool8 IsDamagingMoveUnusableByMon(u16 move, struct Pokemon* monAtk, u8 bankDef);
bool8 IsStatRecoilMove(u16 move);
bool8 IsSuckerPunchOkayToUseThisRound(u16 move, u8 bankAtk, u8 bankDef);
bool8 CanHealFirstToPreventKnockOut(u8 bankAtk, u8 foe);
s32 GetMonPassiveRecovery(struct Pokemon* mon, u8 bank);
u16 GetAIChosenMove(u8 bankAtk, u8 bankDef);
bool8 IsTrapped(u8 bank, bool8 switching);
u32 GetSecondaryEffectDamage(u8 bank);
bool8 IsTakingSecondaryDamage(u8 bank, bool8 checkConfusion);
bool8 WillFaintFromSecondaryDamage(u8 bank);
u32 GetContactDamage(u16 move, u16 bankAtk, u16 bankDef);
u32 GetContactDamageMonAtk(struct Pokemon* monAtk, u16 bankDef);
u32 GetContactDamageMonDef(u16 bankAtk, struct Pokemon* monDef);
bool8 WillFaintFromContactDamage(u8 bankAtk, u8 bankDef, u16 move);
bool8 HighChanceOfBeingImmobilized(u8 bank);
u16 CalcSecondaryEffectChance(u8 bank, u16 move, u8 ability);
u16 CalcAIAccuracy(u16 move, u8 bankAtk, u8 bankDef);
bool8 ShouldAIDelayMegaEvolution(u8 bankAtk, u8 bankDef, u16 move, bool8 optimizeAndLookAtMegaPotential, bool8 runDamageCalcs);
bool8 ShouldPredictBankToMegaEvolve(u8 bank);
void ClearMovePredictionsOnBank(u8 bank);

bool8 BadIdeaToPutToSleep(u8 bankDef, u8 bankAtk);
bool8 BadIdeaToPoison(u8 bankDef, u8 bankAtk);
bool8 GoodIdeaToPoisonSelf(u8 bankAtk);
bool8 BadIdeaToParalyze(u8 bankDef, u8 bankAtk);
bool8 GoodIdeaToParalyzeSelf(u8 bankAtk);
bool8 BadIdeaToBurn(u8 bankDef, u8 bankAtk);
bool8 GoodIdeaToBurnSelf(u8 bankAtk);
bool8 GoodIdeaToFrostbiteSelf(u8 bankAtk);
bool8 BadIdeaToFreeze(u8 bankDef, u8 bankAtk);
bool8 BadIdeaToMakeContactWith(u8 bankAtk, u8 bankDef);

bool8 BadIdeaToRaiseStatAgainst(u8 bankAtk, u8 bankDef, bool8 checkDefAbility);
bool8 BadIdeaToRaiseAttackAgainst(u8 bankAtk, u8 bankDef, u8 amount, bool8 checkPartner);
bool8 BadIdeaToRaiseDefenseAgainst(u8 bankAtk, u8 bankDef, u8 amount, bool8 checkPartner);
bool8 BadIdeaToRaiseSpAttackAgainst(u8 bankAtk, u8 bankDef, u8 amount, bool8 checkPartner);
bool8 BadIdeaToRaiseSpDefenseAgainst(u8 bankAtk, u8 bankDef, u8 amount, bool8 checkPartner);
bool8 BadIdeaToRaiseSpeedAgainst(u8 bankAtk, u8 bankDef, u8 amount, bool8 checkPartner);
bool8 BadIdeaToRaiseEvasionAgainst(u8 bankAtk, u8 bankDef, u8 amount, bool8 checkPartner);
bool8 BadIdeaToRaiseAccuracyAgainst(u8 bankAtk, u8 bankDef, u8 amount, bool8 checkPartner);

bool8 GoodIdeaToRaiseAttackAgainst(u8 bankAtk, u8 bankDef, u8 amount);
bool8 GoodIdeaToRaiseDefenseAgainst(u8 bankAtk, u8 bankDef, u8 amount);
bool8 GoodIdeaToRaiseSpAttackAgainst(u8 bankAtk, u8 bankDef, u8 amount);
bool8 GoodIdeaToRaiseSpDefenseAgainst(u8 bankAtk, u8 bankDef, u8 amount);
bool8 GoodIdeaToRaiseSpeedAgainst(u8 bankAtk, u8 bankDef, u8 amount, u16 atkSpeed, u16 defSpeed);
bool8 GoodIdeaToRaiseEvasionAgainst(u8 bankAtk, u8 bankDef, u8 amount);
bool8 GoodIdeaToRaiseAccuracyAgainst(u8 bankAtk, u8 bankDef, u8 amount);

bool8 GoodIdeaToLowerAttack(u8 bankDef, u8 bankAtk, u16 move);
bool8 GoodIdeaToLowerDefense(u8 bankDef, u8 bankAtk, u16 move);
bool8 GoodIdeaToLowerSpAtk(u8 bankDef, u8 bankAtk, u16 move);
bool8 GoodIdeaToLowerSpDef(u8 bankDef, u8 bankAtk, u16 move);
bool8 GoodIdeaToLowerSpeed(u8 bankDef, u8 bankAtk, u16 move, u8 reduceBy);
bool8 GoodIdeaToLowerAccuracy(u8 bankDef, u8 bankAtk, u16 move);
bool8 GoodIdeaToLowerEvasion(u8 bankDef, u8 bankAtk, u16 move);

bool8 GoodIdeaToSwapStatStages(u8 bankAtk, u8 bankDef);

move_t IsValidMovePrediction(u8 bankAtk, u8 bankDef);
bool8 IsPredictedToSwitch(u8 bankAtk, u8 bankDef);
void StoreMovePrediction(u8 bankAtk, u8 bankDef, u16 move);
void StoreSwitchPrediction(u8 bankAtk, u8 bankDef);
bool8 IsMovePredictionSemiInvulnerable(u8 bankAtk, u8 bankDef);
bool8 IsMovePredictionHealingMove(u8 bankAtk, u8 bankDef);
bool8 IsPredictedToUsePursuitableMove(u8 bankAtk, u8 bankDef);
bool8 IsMovePredictionPhazingMove(u8 bankAtk, u8 bankDef);
bool8 IsMovePredictionHPDrainingMove(u8 bankAtk, u8 bankDef);
bool8 IsMovePredictionHighAccSleepingMove(u8 bankAtk, u8 bankDef);
bool8 CanMovePredictionProtectAgainstMove(u8 bankAtk, u8 bankDef, u16 move);
bool8 IsStrongestMoveHPDrainingMove(u8 bankAtk, u8 bankDef);

bool8 MoveInMovesetAndUsable(u16 move, u8 bank);
bool8 MoveEffectInMovesetAndUsable(u16 move, u8 bank);
bool8 DamagingMoveInMoveset(u8 bank);
bool8 PhysicalMoveInMoveset(u8 bank);
bool8 RealPhysicalMoveInMoveset(u8 bank);
bool8 SpecialMoveInMoveset(u8 bank);
bool8 MoveSplitInMoveset(u8 bank, u8 moveSplit);
bool8 PhysicalMoveInMonMoveset(struct Pokemon* mon, u8 moveLimitations);
bool8 SpecialMoveInMonMoveset(struct Pokemon* mon, u8 moveLimitations);
bool8 PriorityMoveInMoveset(u8 bank);
bool8 PriorityMoveInMovesetRange(u8 bank, u8 lowValue, u8 highValue);
bool8 DamagingPriorityMoveInMovesetThatAffects(u8 bankAtk, u8 bankDef);
bool8 MagicCoatableMovesInMoveset(u8 bank);
bool8 HasProtectionMoveInMoveset(u8 bank, u8 checkType);
bool8 HasContactProtectionMoveInMoveset(u8 bank);
bool8 MoveTypeInMoveset(u8 bank, u8 moveType);
bool8 DamagingMoveTypeInMoveset(u8 bank, u8 moveType);
bool8 DamagingMoveTypeInMovesetThatAffects(u8 bankAtk, u8 bankDef, u8 moveType);
bool8 DamagingAllHitMoveTypeInMoveset(u8 bank, u8 moveType);
bool8 DamagingSpreadMoveInMoveset(u8 bank);
bool8 HasSnatchableMove(u8 bank);
bool8 MoveEffectInMoveset(u8 moveEffect, u8 bank);
bool8 StatusMoveInMoveset(u8 bank);
bool8 MoveInMovesetWithAccuracyLessThan(u8 bankAtk, u8 bankDef, u8 acc, bool8 ignoreStatusMoves);
bool8 AllMovesInMovesetWithAccuracyLessThan(u8 bankAtk, u8 bankDef, u8 acc, bool8 ignoreStatusMoves);
bool8 FlinchingMoveInMoveset(u8 bank);
bool8 HealingMoveInMoveset(u8 bank);
bool8 HealingMoveInMonMoveset(struct Pokemon* mon);
bool8 SoundMoveInMoveset(u8 bank);
bool8 MoveThatCanHelpAttacksHitInMoveset(u8 bank);
bool8 DamagingMoveThaCanBreakThroughSubstituteInMoveset(u8 bankAtk, u8 bankDef);
bool8 ContactMovesThatAffectTargetInMoveset(u8 bankAtk, u8 bankDef);
bool8 UnfreezingMoveInMoveset(u8 bank);
bool8 SleepMoveInMovesetWithLowAccuracy(u8 bankAtk, u8 bankDef);
bool8 PivotingMoveInMovesetThatAffects(u8 bankAtk, u8 bankDef);
bool8 FastPivotingMoveInMovesetThatAffects(u8 bankAtk, u8 bankDef);
bool8 MultiHitMoveWithSplitInMovesetThatAffects(u8 bankAtk, u8 bankDef, u8 split);
bool8 OHKOMoveInMovesetThatAffects(u8 bankAtk, u8 bankDef, struct AIScript* aiScriptData);
bool8 OffensiveSetupMoveInMoveset(u8 bankAtk, u8 bankDef);
bool8 HazingMoveInMoveset(u8 bank);
bool8 PhazingMoveInMovesetThatAffects(u8 bankAtk, u8 bankDef);
bool8 HazardClearingMoveInMovesetThatAffects(u8 bankAtk, u8 bankDef);
bool8 DoubleDamageWithStatusMoveInMovesetThatAffects(u8 bankAtk, u8 bankDef);
bool8 HasUsedMove(u8 bankAtk, u16 move);
bool8 HasUsedMoveWithEffect(u8 bankAtk, u8 effect);
bool8 HasUsedMoveWithEffectHigherThanChance(u8 bank, u8 effect, u8 chance);
bool8 HasUsedPhazingMoveThatAffects(u8 bankAtk, u8 bankDef);
bool8 NoUsableHazardsInMoveset(u8 bankAtk, u8 bankDef, struct AIScript* aiScriptData);
bool8 OnlyBadMovesLeftInMoveset(u8 bankAtk, u8 bankDef);
u16 TryReplaceMoveWithZMove(u8 bankAtk, u8 bankDef, u16 move);
u8 GetAIMoveEffectForMaxMove(u16 move, u8 bankAtk, u8 bankDef);

bool8 GetHealthPercentage(u8 bank);
bool8 TeamFullyHealedMinusBank(u8 bank);
u8 CountUsefulBoosts(u8 bank);
u8 CountUsefulDebuffs(u8 bank);
u8 CountUsefulBoostsOfBankForBank(u8 bankWithBuffs, u8 bankToGetBuffs);
u8 CountUsefulDebuffsOfBankForBank(u8 bankWithDefuffs, u8 bankToGetDebuffs);
bool8 AnyStatIsRaised(u8 bank);
bool8 AnyUsefulStatIsRaised(u8 bank);
bool8 AnyUsefulOffseniveStatIsRaised(u8 bank);
bool8 PartyMemberStatused(u8 bank, bool8 checkSoundproof);
bool8 ShouldAIUseZMove(u8 bank, u8 moveIndex, u16 move);
bool8 ShouldAIUseZMoveByMoveAndMovePos(u8 bankAtk, u8 bankDef, u16 move, u8 movePos);
void ClearShouldAIUseZMoveByMoveAndMovePos(u8 bankAtk, u8 bankDef, u8 movePos);
void CalcAIDynamaxMon(u8 bank);
void CalcShouldAIDynamax(u8 bankAtk, u8 bankDef);
bool8 ShouldAIDynamax(u8 bankAtk);
u8 AdjustMoveLimitationFlagsForAI(u8 bankAtk);

void IncreaseViability(s16* viability, u8 amount);
void DecreaseViability(s16* viability, u16 amount);

#define CAN_SWITCH_OUT(bank) (!IsTrapped(bank, TRUE) && ((IS_SINGLE_BATTLE && ViableMonCountFromBankLoadPartyRange(bank) >= 2) \
													  || (IS_DOUBLE_BATTLE && ViableMonCountFromBankLoadPartyRange(bank) >= 3)))

//Exported Constants
#define AI_THINKING_STRUCT ((struct AI_ThinkingStruct*) (gBattleResources->ai))
#define INCREASE_VIABILITY(x) (IncreaseViability(&viability, x))
#define DECREASE_VIABILITY(x) (DecreaseViability(&viability, x))
#define MOVE_PREDICTION_SWITCH 0xFFFF

#define CHECK_REGULAR_PROTECTION 0x1
#define CHECK_QUICK_GUARD 0x2
#define CHECK_WIDE_GUARD 0x4
#define CHECK_CRAFTY_SHIELD 0x8
#define CHECK_MAT_BLOCK 0x10

#define DOUBLES_INCREASE_HIT_FOE 4
#define DOUBLES_INCREASE_KO_FOE 2
#define DOUBLES_INCREASE_STRONGEST_MOVE 1

#define DOUBLES_DECREASE_HIT_PARTNER 3
#define DOUBLES_DECREASE_DESTINY_BOND 2

#define BEST_DOUBLES_KO_SCORE (DOUBLES_INCREASE_HIT_FOE + DOUBLES_INCREASE_HIT_FOE + DOUBLES_INCREASE_KO_FOE + DOUBLES_INCREASE_KO_FOE)
