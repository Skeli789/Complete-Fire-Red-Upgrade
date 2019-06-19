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
bool8 MoveKnocksOutXHits(u16 move, u8 bankAtk, u8 bankDef, u8 numHits);
u16 CalcFinalAIMoveDamage(u16 move, u8 bankAtk, u8 bankDef, u8 numHits);
bool8 IsStrongestMove(const u16 currentMove, const u8 bankAtk, const u8 bankDef);
bool8 MoveWillHit(u16 move, u8 bankAtk, u8 bankDef);
bool8 MoveWouldHitFirst(u16 move, u16 bankAtk, u16 bankDef);
bool8 WillFaintFromWeatherSoon(u8 bank);
u8 CountBanksPositiveStatStages(u8 bank);
u8 CountBanksNegativeStatStages(u8 bank);
u16 GetTeamSpeedAverage(u8 bank);
u16 GetBattleMonMove(u8 bank, u8 index);
bool8 IsTrapped(u8 bank, bool8 switching);
bool8 IsTakingSecondaryDamage(u8 bank);
u16 CalcSecondaryEffectChance(u8 bank, u16 move);

move_t IsValidMovePrediction(u8 bankAtk, u8 bankDef);
bool8 IsPredictedToSwitch(u8 bankAtk, u8 bankDef);
void StoreMovePrediction(u8 bankAtk, u8 bankDef, u16 move);
bool8 IsMovePredictionSemiInvulnerable(u8 bankAtk, u8 bankDef);
bool8 IsMovePredictionHealingMove(u8 bankAtk, u8 bankDef);

bool8 DamagingMoveInMoveset(u8 bank);
bool8 PhysicalMoveInMoveset(u8 bank);
bool8 SpecialMoveInMoveset(u8 bank);
bool8 MoveSplitInMoveset(u8 bank, u8 moveSplit);
bool8 MagicCoatableMovesInMoveset(u8 bank);
bool8 HasProtectionMoveInMoveset(u8 bank, bool8 AllKinds);
bool8 MoveTypeInMoveset(u8 bank, u8 moveType);
bool8 HasSnatchableMove(u8 bank);
bool8 MoveEffectInMoveset(u8 moveEffect, u8 bank);
bool8 StatusMoveInMoveset(u8 bank);
bool8 MoveInMovesetWithAccuracyLessThan(u8 bankAtk, u8 bankDef, u8 acc, bool8 ignoreStatusMoves);
bool8 FlinchingMoveInMoveset(u8 bank);
bool8 HealingMoveInMoveset(u8 bank);
bool8 SoundMoveInMoveset(u8 bank);
bool8 MoveThatCanHelpAttacksHitInMoveset(u8 bank);

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
