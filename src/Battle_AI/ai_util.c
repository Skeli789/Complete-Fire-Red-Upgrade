#include "../defines.h"
#include "../defines_battle.h"
#include "../../include/random.h"
#include "../../include/constants/items.h"

#include "../../include/new/accuracy_calc.h"
#include "../../include/new/ai_advanced.h"
#include "../../include/new/ai_util.h"
#include "../../include/new/ai_master.h"
#include "../../include/new/ai_scripts.h"
#include "../../include/new/battle_start_turn_start.h"
#include "../../include/new/battle_util.h"
#include "../../include/new/damage_calc.h"
#include "../../include/new/dynamax.h"
#include "../../include/new/end_turn.h"
#include "../../include/new/general_bs_commands.h"
#include "../../include/new/util.h"
#include "../../include/new/item.h"
#include "../../include/new/mega.h"
#include "../../include/new/move_tables.h"
#include "../../include/new/multi.h"
#include "../../include/new/set_z_effect.h"
#include "../../include/new/switching.h"
#include "../../include/new/z_move_effects.h"

/*
ai_util.c
	Commonly used functions in AI logic.
*/

//This file's functions:
static u32 CalcPredictedDamageForCounterMoves(u16 move, u8 bankAtk, u8 bankDef);
static bool8 CalculateMoveKnocksOutXHits(u16 move, u8 bankAtk, u8 bankDef, u8 numHits);

bool8 CanKillAFoe(u8 bank)
{
	u8 foe = FOE(bank);
	if (!(gAbsentBattlerFlags & gBitTable[foe]) && CanKnockOut(bank, foe))
		return TRUE;

	if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE && !(gAbsentBattlerFlags & gBitTable[PARTNER(foe)]))
		return CanKnockOut(bank, PARTNER(foe));

	return FALSE;
}

bool8 CanKnockOut(u8 bankAtk, u8 bankDef)
{
	if (!BATTLER_ALIVE(bankAtk) || !BATTLER_ALIVE(bankDef))
		return FALSE; //Can't KO if you're dead or target is dead

	if (gNewBS->ai.canKnockOut[bankAtk][bankDef] == 0xFF) //Hasn't been calculated yet
	{
		struct BattlePokemon backupMonAtk, backupMonDef;
		u8 backupAbilityAtk = ABILITY_NONE; u8 backupAbilityDef = ABILITY_NONE;
		u16 backupSpeciesAtk = SPECIES_NONE; u16 backupSpeciesDef = SPECIES_NONE;

		TryTempMegaEvolveBank(bankAtk, &backupMonAtk, &backupSpeciesAtk, &backupAbilityAtk);
		TryTempMegaEvolveBank(bankDef, &backupMonDef, &backupSpeciesDef, &backupAbilityDef);

		if (gNewBS->ai.strongestMove[bankAtk][bankDef] == 0xFFFF)
			gNewBS->ai.strongestMove[bankAtk][bankDef] = CalcStrongestMove(bankAtk, bankDef, FALSE);

		gNewBS->ai.canKnockOut[bankAtk][bankDef] = MoveKnocksOutXHits(gNewBS->ai.strongestMove[bankAtk][bankDef], bankAtk, bankDef, 1);

		if (gNewBS->ai.canKnockOut[bankAtk][bankDef])
			gNewBS->ai.can2HKO[bankAtk][bankDef] = TRUE; //If you can KO in 1 hit you can KO in 2

		TryRevertTempMegaEvolveBank(bankDef, &backupMonDef, &backupSpeciesDef, &backupAbilityDef);
		TryRevertTempMegaEvolveBank(bankAtk, &backupMonAtk, &backupSpeciesAtk, &backupAbilityAtk);
	}

	return gNewBS->ai.canKnockOut[bankAtk][bankDef];
}

bool8 GetCanKnockOut(u8 bankAtk, u8 bankDef)
{
	int i;
	u16 move;
	bool8 isAsleep = IsBankAsleep(bankAtk);

	if (IsBankIncapacitated(bankAtk)
	|| gAbsentBattlerFlags & (gBitTable[bankAtk] | gBitTable[bankDef]))
		return FALSE;

	u8 moveLimitations = CheckMoveLimitations(bankAtk, 0, AdjustMoveLimitationFlagsForAI(bankAtk, bankDef));

	for (i = 0; i < MAX_MON_MOVES; ++i)
	{
		move = GetBattleMonMove(bankAtk, i);

		if (move == MOVE_NONE)
			break;

		if (isAsleep && move != MOVE_SNORE)
			continue;

		if (!(gBitTable[i] & moveLimitations))
		{
			if (MoveKnocksOutXHits(move, bankAtk, bankDef, 1))
				return TRUE;
		}
	}

	return FALSE;
}

bool8 Can2HKO(u8 bankAtk, u8 bankDef)
{
	if (gNewBS->ai.can2HKO[bankAtk][bankDef] == 0xFF) //Hasn't been calculated yet
	{
		struct BattlePokemon backupMonAtk, backupMonDef;
		u8 backupAbilityAtk = ABILITY_NONE; u8 backupAbilityDef = ABILITY_NONE;
		u16 backupSpeciesAtk = SPECIES_NONE; u16 backupSpeciesDef = SPECIES_NONE;

		TryTempMegaEvolveBank(bankAtk, &backupMonAtk, &backupSpeciesAtk, &backupAbilityAtk);
		TryTempMegaEvolveBank(bankDef, &backupMonDef, &backupSpeciesDef, &backupAbilityDef);

		if (gNewBS->ai.strongestMove[bankAtk][bankDef] == 0xFFFF)
			gNewBS->ai.strongestMove[bankAtk][bankDef] = CalcStrongestMove(bankAtk, bankDef, FALSE);

		gNewBS->ai.can2HKO[bankAtk][bankDef] = MoveKnocksOutXHits(gNewBS->ai.strongestMove[bankAtk][bankDef], bankAtk, bankDef, 2);

		TryRevertTempMegaEvolveBank(bankDef, &backupMonDef, &backupSpeciesDef, &backupAbilityDef);
		TryRevertTempMegaEvolveBank(bankAtk, &backupMonAtk, &backupSpeciesAtk, &backupAbilityAtk);
	}

	return gNewBS->ai.can2HKO[bankAtk][bankDef];
}

bool8 GetCan2HKO(u8 bankAtk, u8 bankDef)
{
	int i;
	u16 move;
	bool8 isAsleep = IsBankAsleep(bankAtk);

	if (IsBankIncapacitated(bankAtk)
	|| gAbsentBattlerFlags & (gBitTable[bankAtk] | gBitTable[bankDef]))
		return FALSE;

	u8 moveLimitations = CheckMoveLimitations(bankAtk, 0, AdjustMoveLimitationFlagsForAI(bankAtk, bankDef));

	for (i = 0; i < MAX_MON_MOVES; ++i)
	{
		move = GetBattleMonMove(bankAtk, i);

		if (move == MOVE_NONE)
			break;

		if (isAsleep && move != MOVE_SNORE)
			continue;

		if (!(gBitTable[i] & moveLimitations))
		{
			if (MoveKnocksOutXHits(move, bankAtk, bankDef, 2))
				return TRUE;
		}
	}

	return FALSE;
}

bool8 CanKnockOutAfterHealing(u8 bankAtk, u8 bankDef, u16 healAmount, u8 numHits)
{
	int i;
	u16 move;
	bool8 isAsleep = IsBankAsleep(bankAtk);

	if (IsBankIncapacitated(bankAtk)
	|| gAbsentBattlerFlags & (gBitTable[bankAtk] | gBitTable[bankDef]))
		return FALSE;

	//Temporarily increase target's hp for calcs
	u16 backupHp = gBattleMons[bankDef].hp;
	gBattleMons[bankDef].hp = MathMin(backupHp + healAmount, gBattleMons[bankDef].maxHP);

	u8 moveLimitations = CheckMoveLimitations(bankAtk, 0, AdjustMoveLimitationFlagsForAI(bankAtk, bankDef));

	for (i = 0; i < MAX_MON_MOVES; ++i)
	{
		move = GetBattleMonMove(bankAtk, i);

		if (move == MOVE_NONE)
			break;

		if (isAsleep && move != MOVE_SNORE)
			continue;

		if (!(gBitTable[i] & moveLimitations))
		{
			if (CalculateMoveKnocksOutXHits(move, bankAtk, bankDef, numHits)) //Need fresh calculation since data is locked earlier
			{
				gBattleMons[bankDef].hp = backupHp;
				return TRUE;
			}
		}
	}

	gBattleMons[bankDef].hp = backupHp;
	return FALSE;
}

bool8 CanKnockOutWithoutMove(const u16 ignoredMove, const u8 bankAtk, const u8 bankDef, const bool8 ignoreFutureAttacks)
{
	u16 move;
	int i;
	bool8 isAsleep = IsBankAsleep(bankAtk);

	if (IsBankIncapacitated(bankAtk)
	|| gAbsentBattlerFlags & (gBitTable[bankAtk] | gBitTable[bankDef]))
		return FALSE;

	u8 moveLimitations = CheckMoveLimitations(bankAtk, 0, AdjustMoveLimitationFlagsForAI(bankAtk, bankDef));

	for (i = 0; i < MAX_MON_MOVES; ++i)
	{
		move = GetBattleMonMove(bankAtk, i);

		if (move == ignoredMove || (ignoreFutureAttacks && gBattleMoves[move].effect == EFFECT_FUTURE_SIGHT)) //Can't actually knock out right now
			continue;

		if (move == MOVE_NONE)
			break;

		if (isAsleep && move != MOVE_SNORE)
			continue;

		if (!(gBitTable[i] & moveLimitations))
		{
			if (MoveKnocksOutXHits(move, bankAtk, bankDef, 1))
				return TRUE;
		}
	}

	return FALSE;
}

bool8 MoveKnocksOutPossiblyGoesFirstWithBestAccuracy(u16 move, u8 bankAtk, u8 bankDef, bool8 checkGoingFirst)
{
	u16 currMove, currAcc;

	u8 bestMoveIndex = 0xFF;
	u16 bestAcc = 0;
	u8 moveLimitations = CheckMoveLimitations(bankAtk, 0, AdjustMoveLimitationFlagsForAI(bankAtk, bankDef));

	for (int i = 0; i < MAX_MON_MOVES; ++i)
	{
		currMove = GetBattleMonMove(bankAtk, i);
		if (currMove == MOVE_NONE)
			break;

		currMove = TryReplaceMoveWithZMove(bankAtk, bankDef, currMove);

		if (!(gBitTable[i] & moveLimitations))
		{
			currAcc = CalcAIAccuracy(currMove, bankAtk, bankDef);

			if ((!checkGoingFirst || MoveWouldHitFirst(currMove, bankAtk, bankDef))
			&& MoveKnocksOutXHits(currMove, bankAtk, bankDef, 1))
			{
				if (MoveWillHit(currMove, bankAtk, bankDef) || currAcc > bestAcc)
				{
					bestAcc = currAcc;
					bestMoveIndex = i;
				}
				else if (currAcc == bestAcc)
				{
					if (PriorityCalc(bankAtk, ACTION_USE_MOVE, currMove) > PriorityCalc(bankAtk, ACTION_USE_MOVE, gBattleMons[bankAtk].moves[bestMoveIndex])) //The better move is still the one with more priority
						bestMoveIndex = i;
				}
			}
		}
	}

	if (bestMoveIndex == 0xFF) //No moves knock out and go first
		return FALSE;

	if (gBattleMons[bankAtk].moves[bestMoveIndex] == move)
		return TRUE;

	return FALSE;
}

bool8 IsWeakestContactMoveWithBestAccuracy(u16 move, u8 bankAtk, u8 bankDef)
{
	u16 currMove, currAcc, moveEffect;

	u8 bestMoveIndex = 0xFF;
	u16 bestAcc = 0;
	u32 bestDmg = 0xFFFFFFFF;
	u8 moveLimitations = CheckMoveLimitations(bankAtk, 0, AdjustMoveLimitationFlagsForAI(bankAtk, bankDef));

	struct DamageCalc damageData = {0};
	damageData.bankAtk = bankAtk;
	damageData.bankDef = bankDef;
	PopulateDamageCalcStructWithBaseAttackerData(&damageData);
	PopulateDamageCalcStructWithBaseDefenderData(&damageData);

	for (int i = 0; i < MAX_MON_MOVES; ++i)
	{
		currMove = GetBattleMonMove(bankAtk, i);

		if (currMove == MOVE_NONE)
			break;

		moveEffect = gBattleMoves[currMove].effect;

		if (!(gBitTable[i] & moveLimitations)
		&& CheckContact(currMove, bankAtk)
		&& moveEffect != EFFECT_RECHARGE
		&& moveEffect != EFFECT_COUNTER
		&& moveEffect != EFFECT_MIRROR_COAT
		&& moveEffect != EFFECT_BURN_UP
		&& moveEffect != EFFECT_FAKE_OUT
		&& moveEffect != EFFECT_0HKO) //Don't use these move effects on partner
		{
			currAcc = AccuracyCalc(currMove, bankAtk, bankDef);
			u32 dmg = GetFinalAIMoveDamage(currMove, bankAtk, bankDef, 1, &damageData);

			if (dmg < bestDmg && currAcc > bestAcc)
			{
				bestAcc = currAcc;
				bestDmg = dmg;
				bestMoveIndex = i;
			}
		}
	}

	if (bestMoveIndex == 0xFF) //No moves knock out and go first
		return FALSE;

	if (gBattleMons[bankAtk].moves[bestMoveIndex] == move)
		return TRUE;

	return FALSE;
}

bool8 StrongestMoveGoesFirst(u16 move, u8 bankAtk, u8 bankDef)
{
	u16 currMove;
	u32 currDmg;

	u8 bestMoveIndex = 0xFF;
	u32 bestDmg = 0;
	u8 moveLimitations = CheckMoveLimitations(bankAtk, 0, AdjustMoveLimitationFlagsForAI(bankAtk, bankDef));

	struct DamageCalc damageData = {0};
	damageData.bankAtk = bankAtk;
	damageData.bankDef = bankDef;
	PopulateDamageCalcStructWithBaseAttackerData(&damageData);
	PopulateDamageCalcStructWithBaseDefenderData(&damageData);

	for (int i = 0; i < MAX_MON_MOVES; ++i)
	{
		currMove = GetBattleMonMove(bankAtk, i);

		if (currMove == MOVE_NONE)
			break;

		currMove = TryReplaceMoveWithZMove(bankAtk, bankDef, currMove);

		if (!(gBitTable[i] & moveLimitations))
		{
			currDmg = GetFinalAIMoveDamage(currMove, bankAtk, bankDef, 1, &damageData);

			if (MoveWouldHitFirst(currMove, bankAtk, bankDef)
			&& currDmg > bestDmg)
			{
				bestDmg = currDmg;
				bestMoveIndex = i;
			}
		}
	}

	if (bestMoveIndex == 0xFF) //No moves knock out and go first
		return FALSE;

	if (gBattleMons[bankAtk].moves[bestMoveIndex] == move)
		return TRUE;

	return FALSE;
}

bool8 CanKnockOutFromParty(struct Pokemon* monAtk, u8 bankDef, struct DamageCalc* damageData)
{
	int i;
	u16 move;
	bool8 isAsleep = GetMonData(monAtk, MON_DATA_STATUS, NULL) & STATUS_SLEEP;

	if (gAbsentBattlerFlags & (gBitTable[bankDef]))
		return FALSE;

	u8 moveLimitations = CheckMoveLimitationsFromParty(monAtk, 0, 0xFF);

	for (i = 0; i < MAX_MON_MOVES; ++i)
	{
		move = GetMonData(monAtk, MON_DATA_MOVE1 + i, NULL);

		if (move == MOVE_NONE)
			break;

		if (isAsleep && move != MOVE_SNORE)
			continue;

		if (!(gBitTable[i] & moveLimitations))
		{
			if (MoveKnocksOutXHitsFromParty(move, monAtk, bankDef, 1, damageData))
				return TRUE;
		}
	}

	return FALSE;
}

/*
	One Target/Two Targets							Three Targets
12: Hit 2 Foes, KO 2 Foes
11: Hit 2 Foes, KO 1 Foe, Strongest Move 1 Foe
10: Hit 2 Foes, KO 1 Foe/Strongest Move 2 Foes
9: Hit 2 Foes, Strongest Move 1 Foe					Hit 2 Foes, KO 2 Foes, Hurt Partner
8: Hit 2 Foes, Decent Damage 2 Foes					Hit 2 Foes, KO 1 Foe, Strongest Move 1 Foe, Hurt Partner
6: Hit 1 Foe, KO 1 Foe								Hit 2 Foes, KO 1 Foe/Strongest Move 2 Foes, Hurt Partner
5: Hit 1 Foe, Strongest Move 1 Foe					Hit 2 Foes, Strongest Move 1 Foe, Hurt Partner
4: Hit 1 Foe										Hit 2 Foes, Hurt Partner
*/

void UpdateBestDoubleKillingMoveScore(u8 bankAtk, u8 bankDef, u8 bankAtkPartner, u8 bankDefPartner, s8 bestMoveScores[MAX_BATTLERS_COUNT], u16* bestMove)
{
	//mgba_printf(MGBA_LOG_ERROR, "");
	int i, j;
	u8 currTarget;
	u16 move;
	s8 moveScores[MAX_MON_MOVES][MAX_BATTLERS_COUNT] = {0};
	u16 turnedToZMove[MAX_MON_MOVES] = {0};

	u16 partnerMove = MOVE_NONE;
	u16 partnerTarget = gBattleStruct->moveTarget[bankAtkPartner];
	if (gChosenMovesByBanks[bankAtkPartner] != MOVE_NONE)
		partnerMove = GetAIChosenMove(bankAtkPartner, partnerTarget);

	bool8 partnerIncapacitated = IsBankIncapacitated(bankAtkPartner);
	bool8 partnerKnocksOut = partnerTarget != bankAtkPartner && MoveKnocksOutXHits(partnerMove, bankAtkPartner, partnerTarget, 1);

	u8 moveLimitations = CheckMoveLimitations(bankAtk, 0, 0xFF);
	
	u8 foes[2] = {bankDef, bankDefPartner};
	bool8 foeAlive[2] = {BATTLER_ALIVE(bankDef), BATTLER_ALIVE(bankDefPartner)};
	bool8 partnerHandling[2] = {FALSE, FALSE};
	bool8 foesAlive = CountAliveMonsInBattle(BATTLE_ALIVE_DEF_SIDE, bankAtk, bankDef);
	
	if (partnerMove != MOVE_NONE && !partnerIncapacitated && partnerKnocksOut && foesAlive >= 2) //More than 1 target left
	{
		for (j = 0; j < gBattlersCount / 2; ++j)
		{
			if (partnerTarget == foes[j])
				partnerHandling[j] = TRUE;
		}
	}

	struct DamageCalc damageDatas[2];
	for (j = 0; j < gBattlersCount / 2; ++j)
	{
		if (foeAlive[j] && !partnerHandling[j]) //Damage will actually get calculated
		{
			//Populate the damage calc structs
			struct DamageCalc* damageData = &damageDatas[j];
			Memset(damageData, 0, sizeof(damageDatas[j]));
			damageData->bankAtk = bankAtk;
			damageData->bankDef = foes[j];
			PopulateDamageCalcStructWithBaseAttackerData(damageData);
			PopulateDamageCalcStructWithBaseDefenderData(damageData);
		}
	}

	for (i = 0; i < MAX_MON_MOVES; ++i)
	{
		//mgba_printf(MGBA_LOG_INFO, "");
		move = GetBattleMonMove(bankAtk, i);

		if (move == MOVE_NONE)
			break;

		if (SPLIT(move) == SPLIT_STATUS)
			continue;

		move = TryReplaceMoveWithZMove(bankAtk, bankDef, move);
		if (IsZMove(move) || IsAnyMaxMove(move))
			turnedToZMove[i] = move;

		if (!(gBitTable[i] & moveLimitations))
		{
			for (j = 0; j < gBattlersCount / 2; ++j)
			{
				//mgba_printf(MGBA_LOG_WARN, "");
				currTarget = foes[j];

				if (foeAlive[j] && (j == 0 || gBattleMoves[move].target & (MOVE_TARGET_BOTH | MOVE_TARGET_ALL)) //Only can hit second foe with spread move
				&& !partnerHandling[j]) //Don't count the target if the partner is already taking care of it
				{
					if (!(AI_SpecialTypeCalc(move, bankAtk, currTarget) & MOVE_RESULT_NO_EFFECT)) //Move has effect on current target
					{
						moveScores[i][currTarget] += DOUBLES_INCREASE_HIT_FOE; //Hit one enemy

						if (MoveKnocksOutXHits(move, bankAtk, currTarget, 1))
						{
							moveScores[i][currTarget] += DOUBLES_INCREASE_KO_FOE; //KO enemy

							if (gBattleMons[currTarget].status2 & STATUS2_DESTINY_BOND)
								moveScores[i][currTarget] -= DOUBLES_DECREASE_DESTINY_BOND;
						}
						else if (IsStrongestMove(move, bankAtk, currTarget))
							moveScores[i][currTarget] += DOUBLES_INCREASE_STRONGEST_MOVE;
						else
						{
							#define CALC (CalcSecondaryEffectChance(bankAtk, move) >= 50)
							//These move effects are good even if they do minimal damage
							switch (gBattleMoves[move].effect) {
								case EFFECT_FLINCH_HIT:
									if (MoveWouldHitFirst(move, bankAtk, currTarget))
										break;
									goto DEFAULT_CHECK;
								case EFFECT_PARALYZE_HIT:
									if (CALC && !BadIdeaToParalyze(currTarget, bankAtk))
										break;
									goto DEFAULT_CHECK;
								case EFFECT_BURN_HIT:
									if (CALC && !BadIdeaToBurn(currTarget, bankAtk))
										break;
									goto DEFAULT_CHECK;
								case EFFECT_FREEZE_HIT:
									if (CALC && !BadIdeaToFreeze(currTarget, bankAtk))
										break;
									goto DEFAULT_CHECK;
								case EFFECT_POISON_HIT:
								case EFFECT_BAD_POISON_HIT:
									if (CALC && !BadIdeaToPoison(currTarget, bankAtk))
										break;
									goto DEFAULT_CHECK;
								case EFFECT_ATTACK_DOWN_HIT:
									if (CALC && GoodIdeaToLowerAttack(currTarget, bankAtk, move))
										break;
									goto DEFAULT_CHECK;
								case EFFECT_DEFENSE_DOWN_HIT:
									if (CALC && GoodIdeaToLowerDefense(currTarget, bankAtk, move))
										break;
									goto DEFAULT_CHECK;
								case EFFECT_SPECIAL_ATTACK_DOWN_HIT:
									if (CALC && GoodIdeaToLowerSpAtk(currTarget, bankAtk, move))
										break;
									goto DEFAULT_CHECK;
								case EFFECT_SPECIAL_DEFENSE_DOWN_HIT:
									if (CALC && GoodIdeaToLowerSpDef(currTarget, bankAtk, move))
										break;
									goto DEFAULT_CHECK;
								case EFFECT_SPEED_DOWN_HIT:
									if (CALC && GoodIdeaToLowerSpeed(currTarget, bankAtk, move))
										break;
									goto DEFAULT_CHECK;
								case EFFECT_ACCURACY_DOWN_HIT:
									if (CALC && GoodIdeaToLowerAccuracy(currTarget, bankAtk, move))
										break;
									goto DEFAULT_CHECK;
								case EFFECT_EVASION_DOWN_HIT:
									if (CALC && GoodIdeaToLowerEvasion(currTarget, bankAtk, move))
										break;
									goto DEFAULT_CHECK;
								case EFFECT_CONFUSE_HIT:
									if (CALC && CanBeConfused(bankDef, TRUE))
										break;
									goto DEFAULT_CHECK;
								case EFFECT_SPEED_UP_1_HIT:
								case EFFECT_ATTACK_UP_HIT:
									if (CALC)
										break;
									goto DEFAULT_CHECK;

								default:
								DEFAULT_CHECK:
									if (foesAlive >= 2) //If there's only 1 target we rely on the strongest move check
									{
										u32 dmg = GetFinalAIMoveDamage(move, bankAtk, currTarget, 1, &damageDatas[j]);
										if (dmg < gBattleMons[currTarget].maxHP / 4)
											moveScores[i][currTarget] -= (DOUBLES_INCREASE_HIT_FOE - 1); //Count it as if you basically don't do damage to the enemy
										else if (dmg < gBattleMons[currTarget].maxHP / 3)
											moveScores[i][currTarget] -= (DOUBLES_INCREASE_HIT_FOE - 2); //Count less than basically not hitting the enemy
									}
							}
							#undef CALC
						}
					}
				}
			}

			if (gBattleMoves[move].target & MOVE_TARGET_ALL
			&&  RangeMoveCanHurtPartner(move, bankAtk, bankAtkPartner))
			{
				u8 status1 = gBattleMons[bankAtkPartner].status1;
				if ((status1 & STATUS1_SLEEP) <= 1 //Partner will be awake to use move
				&& !(status1 & STATUS1_FREEZE)
				&& !gDisableStructs[bankAtkPartner].truantCounter)
				{
					if (partnerMove != MOVE_NONE) //Partner has chosen a move
					{
						u8 moveEffect = gBattleMoves[partnerMove].effect;

						if (moveEffect == EFFECT_PROTECT
						&&  partnerMove != MOVE_QUICKGUARD
						&&  partnerMove != MOVE_CRAFTYSHIELD
						&& (partnerMove != MOVE_MATBLOCK || gDisableStructs[bankAtkPartner].isFirstTurn))
						{
							//Partner is protecting so use the spread move
						}
						else if (moveEffect == EFFECT_SEMI_INVULNERABLE && !MoveWouldHitFirst(move, bankAtk, bankAtkPartner))
						{
						}
						else
							moveScores[i][bankAtkPartner] -= DOUBLES_DECREASE_HIT_PARTNER; //Hitting partner is bad
					}
					else if (!HasProtectionMoveInMoveset(bankAtkPartner, CHECK_WIDE_GUARD | CHECK_MAT_BLOCK) //Ally can't protect against attack
					|| (gBattleMoves[gLastResultingMoves[bankAtkPartner]].effect == EFFECT_PROTECT && gDisableStructs[bankAtkPartner].protectUses >= 1)) //Don't rely on double protect
					{
						moveScores[i][bankAtkPartner] -= DOUBLES_DECREASE_HIT_PARTNER; //Hitting partner is bad
					}
				}
				else
					moveScores[i][bankAtkPartner] -= DOUBLES_DECREASE_HIT_PARTNER; //Hitting partner is bad
			}
		}
	}

	u8 bestIndex = 0;
	for (int i = 1; i < MAX_MON_MOVES; ++i)
	{
		u8 currScore = moveScores[i][bankDef]
					 + moveScores[i][bankDefPartner]
					 + moveScores[i][bankAtkPartner];

		u8 bestScore = moveScores[bestIndex][bankDef]
					 + moveScores[bestIndex][bankDefPartner]
					 + moveScores[bestIndex][bankAtkPartner];

		if (currScore > bestScore)
			bestIndex = i;
		else if (currScore == bestScore)
		{
			u8 currentBestRange = gBattleMoves[gBattleMons[bankAtk].moves[bestIndex]].target;
			u8 checkBestRange = gBattleMoves[gBattleMons[bankAtk].moves[i]].target;

			if (currentBestRange & MOVE_TARGET_ALL
			&& !(checkBestRange & MOVE_TARGET_ALL)
			&& checkBestRange & MOVE_TARGET_BOTH
			&& RangeMoveCanHurtPartner(gBattleMons[bankAtk].moves[bestIndex], bankAtk, bankAtkPartner))
				bestIndex = i; //If an all-hitting move and a both foes move have the same score, use the one that only hits both foes
		}
	}

	bestMoveScores[bankDef] = moveScores[bestIndex][bankDef];
	bestMoveScores[bankDefPartner] = moveScores[bestIndex][bankDefPartner];
	bestMoveScores[bankAtkPartner] = moveScores[bestIndex][bankAtkPartner];

	*bestMove = gBattleMons[bankAtk].moves[bestIndex];
	if (turnedToZMove[bestIndex] != MOVE_NONE)
		*bestMove = turnedToZMove[bestIndex];
}

u8 GetDoubleKillingScore(u16 move, u8 bankAtk, u8 bankDef)
{
	if (gBattleMoves[move].target & MOVE_TARGET_ALL
	&&  gBattleMoves[gChosenMovesByBanks[PARTNER(bankAtk)]].effect == EFFECT_PROTECT
	&& !gNewBS->recalculatedBestDoublesKillingScores[bankAtk])
	{
		//Recalculate as partner has chosen to protect from spread move
		gNewBS->recalculatedBestDoublesKillingScores[bankAtk] = TRUE;
		UpdateBestDoubleKillingMoveScore(bankAtk, bankDef, PARTNER(bankAtk), PARTNER(bankDef), gNewBS->ai.bestDoublesKillingScores[bankAtk][bankDef], &gNewBS->ai.bestDoublesKillingMoves[bankAtk][bankDef]);
	}

	if (move == gNewBS->ai.bestDoublesKillingMoves[bankAtk][bankDef])
	{
		//When a Pokemon attacks a target, it can gain or lose points for the damage it does
		//to the surrounding field
		return gNewBS->ai.bestDoublesKillingScores[bankAtk][bankDef][bankDef]
		     + gNewBS->ai.bestDoublesKillingScores[bankAtk][bankDef][PARTNER(bankDef)]
			 + gNewBS->ai.bestDoublesKillingScores[bankAtk][bankDef][PARTNER(bankAtk)];
	}

	return 0;
}

static void RemoveDoublesKillingScore(u8 bankAtk, u8 bankDef)
{
	//Reset the points received by attacking bankDef for both foes
	UpdateBestDoubleKillingMoveScore(bankAtk, bankDef, PARTNER(bankAtk), PARTNER(bankDef), gNewBS->ai.bestDoublesKillingScores[bankAtk][bankDef], &gNewBS->ai.bestDoublesKillingMoves[bankAtk][bankDef]);
}

void TryRemoveDoublesKillingScore(u8 bankAtk, u8 bankDef, u16 chosenMove)
{
	u8 partner = PARTNER(bankAtk);

	if (IS_DOUBLE_BATTLE && BATTLER_ALIVE(partner))
	{
		u16 partnerKillingMove = gNewBS->ai.bestDoublesKillingMoves[partner][bankDef];

		if (!(bankAtk & BIT_FLANK) //Don't bother with reseting scores if no Pokemon are going to choose moves after this
		&& !IsBankIncapacitated(bankAtk) //Not actually going to hit a target this turn
		&& SIDE(bankAtk) != SIDE(bankDef) //No scores are calculated for hitting partners
		&& (CountAliveMonsInBattle(BATTLE_ALIVE_DEF_SIDE, bankAtk, bankDef) >= 2 //Two targets that can be hit on enemy side
		 || MoveWouldHitFirst(chosenMove, bankAtk, bankDef) //This mon is going to hit the enemy before it can attack
		 || !MoveWouldHitFirst(partnerKillingMove, partner, bankDef)) //The partner won't hit the enemy before it can attack
		&& MoveKnocksOutXHits(chosenMove, bankAtk, bankDef, 1))
			RemoveDoublesKillingScore(partner, bankDef); //This mon's got it covered
	}
}

bool8 RangeMoveCanHurtPartner(u16 move, u8 bankAtk, u8 bankAtkPartner)
{
	return BATTLER_ALIVE(bankAtkPartner)
		&& ABILITY(bankAtkPartner) != ABILITY_TELEPATHY
		&& !(BATTLER_SEMI_INVULNERABLE(bankAtkPartner) && MoveWouldHitFirst(move, bankAtk, bankAtkPartner)) //Target will still be semi-invulnerable when attack hits
		&& !(AI_SpecialTypeCalc(move, bankAtk, bankAtkPartner) & MOVE_RESULT_NO_EFFECT); //Move has effect
}

static bool8 CalculateMoveKnocksOutXHits(u16 move, u8 bankAtk, u8 bankDef, u8 numHits)
{
	u8 ability = ABILITY(bankDef);
	u16 species = SPECIES(bankDef);
	bool8 noMoldBreakers = NO_MOLD_BREAKERS(ABILITY(bankAtk), move);

	if (MoveBlockedBySubstitute(move, bankAtk, bankDef)
	#ifdef SPECIES_MIMIKYU
	|| (ability == ABILITY_DISGUISE && species == SPECIES_MIMIKYU && noMoldBreakers)
	#endif
	#ifdef SPECIES_EISCUE
	|| (ability == ABILITY_ICEFACE && species == SPECIES_EISCUE && SPLIT(move) == SPLIT_PHYSICAL && noMoldBreakers)
	#endif
	)
	{
		if (numHits > 0)
			numHits -= 1; //Takes at least a hit to break Disguise/Ice Face or sub
	}

	if (GetFinalAIMoveDamage(move, bankAtk, bankDef, numHits, NULL) >= gBattleMons[bankDef].hp)
		return TRUE;

	return FALSE;
}

bool8 MoveKnocksOutXHits(u16 move, u8 bankAtk, u8 bankDef, u8 numHits)
{
	u8 movePos;

	switch (numHits) {
		case 1:
			if (gBattleMoves[move].effect == EFFECT_FUTURE_SIGHT)
				return FALSE; //Really always 3 hits

			movePos = FindMovePositionInMoveset(move, bankAtk);
			if (movePos >= MAX_MON_MOVES)
				break; //Move not in moveset

			if (gNewBS->ai.moveKnocksOut1Hit[bankAtk][bankDef][movePos] != 0xFF)
				return gNewBS->ai.moveKnocksOut1Hit[bankAtk][bankDef][movePos];
			return gNewBS->ai.moveKnocksOut1Hit[bankAtk][bankDef][movePos] = CalculateMoveKnocksOutXHits(move, bankAtk, bankDef, 1);

		case 2:
			if (gBattleMoves[move].effect == EFFECT_FUTURE_SIGHT)
				return FALSE; //Really always 3 hits

			movePos = FindMovePositionInMoveset(move, bankAtk);
			if (movePos >= MAX_MON_MOVES)
				break; //Move not in moveset

			if (gNewBS->ai.moveKnocksOut2Hits[bankAtk][bankDef][movePos] != 0xFF)
				return gNewBS->ai.moveKnocksOut2Hits[bankAtk][bankDef][movePos];
			return gNewBS->ai.moveKnocksOut2Hits[bankAtk][bankDef][movePos] = CalculateMoveKnocksOutXHits(move, bankAtk, bankDef, 2);
	}

	return CalculateMoveKnocksOutXHits(move, bankAtk, bankDef, numHits);
}

bool8 MoveKnocksOutXHitsFromParty(u16 move, struct Pokemon* monAtk, u8 bankDef, u8 numHits, struct DamageCalc* damageData)
{
	u8 ability = ABILITY(bankDef);
	u16 species = SPECIES(bankDef);
	bool8 noMoldBreakers = NO_MOLD_BREAKERS(GetMonAbility(monAtk), move);

	if (MonMoveBlockedBySubstitute(move, monAtk, bankDef)
	#ifdef SPECIES_MIMIKYU
	|| (ability == ABILITY_DISGUISE && species == SPECIES_MIMIKYU && noMoldBreakers)
	#endif
	#ifdef SPECIES_EISCUE
	|| (ability == ABILITY_ICEFACE && species == SPECIES_EISCUE && SPLIT(move) == SPLIT_PHYSICAL && noMoldBreakers)
	#endif
	)
	{
		if (numHits > 0)
			numHits -= 1; //Takes at least a hit to break Disguise/Ice Face or sub
	}

	if (CalcFinalAIMoveDamageFromParty(move, monAtk, bankDef, numHits, damageData) >= gBattleMons[bankDef].hp)
		return TRUE;

	return FALSE;
}

static bool8 MoveKnocksOutAfterDynamax(u16 move, u8 bankAtk, u8 bankDef)
{
	bool8 ret;
	u8 movePos = FindMovePositionInMoveset(move, bankAtk);
	if (movePos < MAX_MON_MOVES && !gNewBS->ai.moveKnocksOut1Hit[bankAtk][bankDef][movePos])
		return FALSE; //Move doesn't normally KO, certainly won't KO after Dynamaxing

	//Temporarily Dynamax target
	u16 backupHP = gBattleMons[bankDef].hp;
	u16 backupMaxHP = gBattleMons[bankDef].maxHP;
	gBattleMons[bankDef].hp *= GetDynamaxHPBoost(bankDef);
	gBattleMons[bankDef].maxHP *= GetDynamaxHPBoost(bankDef);
	gNewBS->dynamaxData.timer[bankDef] += 1;

	ret = CalculateMoveKnocksOutXHits(move, bankAtk, bankDef, 1);

	//Revert Dynamax
	gNewBS->dynamaxData.timer[bankDef] -= 1;
	gBattleMons[bankDef].maxHP = backupMaxHP;
	gBattleMons[bankDef].hp = backupHP;

	return ret;
}

u16 CalcFinalAIMoveDamage(u16 move, u8 bankAtk, u8 bankDef, u8 numHits, struct DamageCalc* damageData)
{
	if (move == MOVE_NONE || numHits == 0 || gBattleMoves[move].power == 0)
		return 0;

	switch (gBattleMoves[move].effect) {
		case EFFECT_FAKE_OUT:
			if (!gDisableStructs[bankAtk].isFirstTurn)
				return 0;
			break;

		case EFFECT_BURN_UP:
			if (!IsOfType(bankAtk, TYPE_FIRE))
				return 0;
			break;

		case EFFECT_0HKO:
			if (gBattleMons[bankAtk].level <= gBattleMons[bankDef].level)
				return 0;
			if (move == MOVE_SHEERCOLD && IsOfType(bankDef, TYPE_ICE))
				return 0;

			return gBattleMons[bankDef].hp;

		case EFFECT_COUNTER: //Includes Metal Burst
		case EFFECT_MIRROR_COAT:
			return CalcPredictedDamageForCounterMoves(move, bankAtk, bankDef);
	}

	u32 dmg = AI_CalcDmg(bankAtk, bankDef, move, damageData);
	if (dmg >= gBattleMons[bankDef].hp)
		return gBattleMons[bankDef].hp;

	u8 defAbility = ABILITY(bankDef);
	if (numHits >= 2 && BATTLER_MAX_HP(bankDef) && (defAbility == ABILITY_MULTISCALE || defAbility == ABILITY_SHADOWSHIELD))
		return MathMin(dmg + (dmg * 2) * (numHits - 1), gBattleMons[bankDef].maxHP); //Adjust damage on subsequent hits

	return MathMin(dmg * numHits, gBattleMons[bankDef].maxHP);
}

u32 GetFinalAIMoveDamage(u16 move, u8 bankAtk, u8 bankDef, u8 numHits, struct DamageCalc* damageData)
{
	u8 movePos = FindMovePositionInMoveset(move, bankAtk);
	if (movePos < MAX_MON_MOVES) //Move in moveset
	{
		if (gNewBS->ai.damageByMove[bankAtk][bankDef][movePos] != 0xFFFFFFFF)
			return gNewBS->ai.damageByMove[bankAtk][bankDef][movePos] * numHits;
		gNewBS->ai.damageByMove[bankAtk][bankDef][movePos] = CalcFinalAIMoveDamage(move, bankAtk, bankDef, 1, damageData);
		return gNewBS->ai.damageByMove[bankAtk][bankDef][movePos] * numHits;
	}

	return CalcFinalAIMoveDamage(move, bankAtk, bankDef, 1, damageData) * numHits;
}

u16 CalcFinalAIMoveDamageFromParty(u16 move, struct Pokemon* monAtk, u8 bankDef, u8 numHits, struct DamageCalc* damageData)
{
	if (move == MOVE_NONE || SPLIT(move) == SPLIT_STATUS || gBattleMoves[move].power == 0)
		return 0;

	switch (gBattleMoves[move].effect) {
		case EFFECT_0HKO:
			if (GetMonData(monAtk, MON_DATA_LEVEL, NULL) <= gBattleMons[bankDef].level)
				return 0;
			if (move == MOVE_SHEERCOLD && IsOfType(bankDef, TYPE_ICE))
				return 0;

			return gBattleMons[bankDef].hp;

		case EFFECT_COUNTER: //Includes Metal Burst
		case EFFECT_MIRROR_COAT:
			return 0;
	}

	return MathMin(AI_CalcPartyDmg(FOE(bankDef), bankDef, move, monAtk, damageData) * numHits, gBattleMons[bankDef].maxHP);
}

static u32 CalcPredictedDamageForCounterMoves(u16 move, u8 bankAtk, u8 bankDef)
{
	u16 predictedMove;
	u32 predictedDamage = 0;
	
	if (AI_SpecialTypeCalc(move, bankAtk, bankDef) & MOVE_RESULT_NO_EFFECT)
		return 0; //These moves are subject to immunities

	if (gNewBS->ai.strongestMove[bankDef][bankAtk] != 0xFFFF) //Don't force calculation here - can cause infinite loop if both Pokemon have a counter move
		predictedMove = GetStrongestMove(bankDef, bankAtk); //Get the strongest move as the predicted move
	else
		predictedMove = IsValidMovePrediction(bankDef, bankAtk);

	if (predictedMove != MOVE_NONE && SPLIT(predictedMove) != SPLIT_STATUS && !MoveBlockedBySubstitute(predictedMove, bankDef, bankAtk))
	{
		predictedDamage = GetFinalAIMoveDamage(predictedMove, bankDef, bankAtk, 1, NULL); //The damage the enemy will do to the AI

		switch (move) {
			case MOVE_COUNTER:
				if (CalcMoveSplit(bankDef, predictedMove) == SPLIT_PHYSICAL)
					predictedDamage *= 2;
				else
					predictedDamage = 0;
				break;
			case MOVE_MIRRORCOAT:
				if (CalcMoveSplit(bankDef, predictedMove) == SPLIT_SPECIAL)
					predictedDamage *= 2;
				else
					predictedDamage = 0;
				break;
			default: //Metal Burst
				predictedDamage = (predictedDamage * 3) / 2;
		}
	}

	return predictedDamage;
}

move_t CalcStrongestMove(const u8 bankAtk, const u8 bankDef, const bool8 onlySpreadMoves)
{
	u16 move;
	u32 predictedDamage;
	u16 strongestMove = gBattleMons[bankAtk].moves[0];
	u32 highestDamage = 0;
	u16 defHP = gBattleMons[bankDef].hp;
	struct DamageCalc damageData = {0};

	if (defHP == 0) //Foe dead
		return MOVE_NONE;

	u8 moveLimitations = CheckMoveLimitations(bankAtk, 0, AdjustMoveLimitationFlagsForAI(bankAtk, bankDef));
	predictedDamage = 0;

	//Save time and do this now instead of before each move
	damageData.bankAtk = bankAtk;
	damageData.bankDef = bankDef;
	PopulateDamageCalcStructWithBaseAttackerData(&damageData);
	PopulateDamageCalcStructWithBaseDefenderData(&damageData);

	for (u32 i = 0; i < MAX_MON_MOVES; ++i)
	{
		move = gBattleMons[bankAtk].moves[i];
		if (move == MOVE_NONE)
			break;

		move = TryReplaceMoveWithZMove(bankAtk, bankDef, move);

		if (!(gBitTable[i] & moveLimitations))
		{
			if (gBattleMoves[move].power == 0
			|| (onlySpreadMoves && !(gBattleMoves[move].target & (MOVE_TARGET_BOTH | MOVE_TARGET_ALL))))
				continue;

			if (gBattleMoves[move].effect == EFFECT_COUNTER || gBattleMoves[move].effect == EFFECT_MIRROR_COAT) //Includes Metal Burst
			{
				predictedDamage = CalcPredictedDamageForCounterMoves(move, bankAtk, bankDef);
				if (predictedDamage > (u32) highestDamage)
				{
					strongestMove = move;
					highestDamage = predictedDamage;
				}
			}
			else if (gBattleMoves[move].effect == EFFECT_0HKO)
			{
				gNewBS->ai.moveKnocksOut1Hit[bankAtk][bankDef][i] = FALSE;
				gNewBS->ai.damageByMove[bankAtk][bankDef][i] = 0;
				if (gBattleMons[bankAtk].level <= gBattleMons[bankDef].level)
					continue;
				if (move == MOVE_SHEERCOLD && IsOfType(bankDef, TYPE_ICE))
					continue;
				if (ABILITY(bankDef) == ABILITY_STURDY)
					continue;
				if (MoveWillHit(move, bankAtk, bankDef))
				{
					gNewBS->ai.moveKnocksOut1Hit[bankAtk][bankDef][i] = TRUE;
					gNewBS->ai.damageByMove[bankAtk][bankDef][i] = defHP;
					return move; //No stronger move that OHKO move that can kill
				}
			}
			else
			{
				predictedDamage = CalcFinalAIMoveDamage(move, bankAtk, bankDef, 1, &damageData);

				if (predictedDamage > (u32) highestDamage)
				{
					strongestMove = move;
					highestDamage = predictedDamage;
				}
				else if (predictedDamage == highestDamage
				&& PriorityCalc(bankAtk, ACTION_USE_MOVE, move) > PriorityCalc(bankAtk, ACTION_USE_MOVE, strongestMove)) //Use faster of two strongest moves
				{
					strongestMove = move;
				}
				else if (predictedDamage == highestDamage) //Find which move has better Acc
				{
					u16 currAcc = CalcAIAccuracy(move, bankAtk, bankDef);
					u16 bestMoveAcc = CalcAIAccuracy(strongestMove, bankAtk, bankDef);

					if (currAcc > bestMoveAcc)
					{
						strongestMove = move;
						highestDamage = predictedDamage;
					}
				}
			}

			gNewBS->ai.damageByMove[bankAtk][bankDef][i] = predictedDamage;
			if (predictedDamage >= defHP)
				gNewBS->ai.moveKnocksOut1Hit[bankAtk][bankDef][i] = TRUE;
			else
				gNewBS->ai.moveKnocksOut1Hit[bankAtk][bankDef][i] = FALSE;
		}
	}

	return strongestMove;
}

bool8 IsStrongestMove(const u16 currentMove, const u8 bankAtk, const u8 bankDef)
{
	if (gNewBS->ai.strongestMove[bankAtk][bankDef] == 0xFFFF)
		gNewBS->ai.strongestMove[bankAtk][bankDef] = CalcStrongestMove(bankAtk, bankDef, FALSE);

	return gNewBS->ai.strongestMove[bankAtk][bankDef] == currentMove;
}

u16 GetStrongestMove(const u8 bankAtk, const u8 bankDef)
{
	if (gNewBS->ai.strongestMove[bankAtk][bankDef] == 0xFFFF)
		gNewBS->ai.strongestMove[bankAtk][bankDef] = CalcStrongestMove(bankAtk, bankDef, FALSE);

	return gNewBS->ai.strongestMove[bankAtk][bankDef];
}

static void ClearStrongestMoveAndCanKnockOut(const u8 bankAtk, const u8 bankDef)
{
	gNewBS->ai.strongestMove[bankAtk][bankDef] = 0xFFFF;
	gNewBS->ai.canKnockOut[bankAtk][bankDef] = 0xFF;
	gNewBS->ai.can2HKO[bankAtk][bankDef] = 0xFF;
}

bool8 MoveWillHit(u16 move, u8 bankAtk, u8 bankDef)
{
	#ifdef REALLY_SMART_AI
		u8 defAbility = BATTLE_HISTORY->abilities[bankDef];
	#else
		u8 defAbility = ABILITY(bankDef);
	#endif

	if (ABILITY(bankAtk) == ABILITY_NOGUARD
	||  defAbility == ABILITY_NOGUARD
	||  (gStatuses3[bankDef] & STATUS3_ALWAYS_HITS && gDisableStructs[bankDef].bankWithSureHit == bankAtk))
		return TRUE;

	if (((gStatuses3[bankDef] & (STATUS3_IN_AIR | STATUS3_SKY_DROP_ATTACKER | STATUS3_SKY_DROP_TARGET)) && !CheckTableForMove(move, gIgnoreInAirMoves))
	||  ((gStatuses3[bankDef] & STATUS3_UNDERGROUND) && !CheckTableForMove(move, gIgnoreUndergoundMoves))
	||  ((gStatuses3[bankDef] & STATUS3_UNDERWATER) && !CheckTableForMove(move, gIgnoreUnderwaterMoves))
	||   (gStatuses3[bankDef] & STATUS3_DISAPPEARED))
		return FALSE;

	if ((move == MOVE_TOXIC && IsOfType(bankAtk, TYPE_POISON))
	||  (CheckTableForMove(move, gAlwaysHitWhenMinimizedMoves) && gStatuses3[bankDef] & STATUS3_MINIMIZED)
	|| ((gStatuses3[bankDef] & STATUS3_TELEKINESIS) && gBattleMoves[move].effect != EFFECT_0HKO)
	||  gBattleMoves[move].accuracy == 0
	|| (WEATHER_HAS_EFFECT && (gBattleWeather & WEATHER_RAIN_ANY) && CheckTableForMove(move, gAlwaysHitInRainMoves))
	||  IsZMove(move)
	||  IsAnyMaxMove(move))
		return TRUE;

	return FALSE;
}

bool8 MoveWouldHitFirst(u16 move, u16 bankAtk, u16 bankDef)
{
	//Use move prediction
	u16 defMovePrediction = IsValidMovePrediction(bankDef, bankAtk);
	return MoveWouldHitBeforeOtherMove(move, bankAtk, defMovePrediction, bankDef);
}

bool8 MoveWouldHitBeforeOtherMove(u16 moveAtk, u8 bankAtk, u16 moveDef, u8 bankDef)
{
	u32 temp;
	u32 bankAtkSpeed, bankDefSpeed;

//Priority Calc
	if (moveDef == MOVE_NONE)
	{
		if (PriorityCalc(bankAtk, ACTION_USE_MOVE, moveAtk) > 0)
			return TRUE;
	}
	else
	{
		if (PriorityCalc(bankAtk, ACTION_USE_MOVE, moveAtk) > PriorityCalc(bankDef, ACTION_USE_MOVE, moveDef))
			return TRUE;
	}

//BracketCalc
	if (BracketCalc(bankAtk) > BracketCalc(bankDef)) //Hehehe...AI knows when its Quick Claw activates
		return TRUE;

//SpeedCalc
	bankAtkSpeed = SpeedCalc(bankAtk);
	bankDefSpeed = SpeedCalc(bankDef);
	if (IsTrickRoomActive())
	{
		temp = bankDefSpeed;
		bankDefSpeed = bankAtkSpeed;
		bankAtkSpeed = temp;
	}

	if (bankAtkSpeed > bankDefSpeed)
		return TRUE;

	return FALSE;
}

bool8 IsUsefulToFlinchTarget(u8 bankDef)
{
	if (IsDynamaxed(bankDef))
		return FALSE;

	if (IsBankAsleep(bankDef)
	&&  !MoveInMoveset(MOVE_SLEEPTALK, bankDef)
	&&  !MoveInMoveset(MOVE_SNORE, bankDef))
		return FALSE;

	return TRUE;
}

bool8 IsBankAsleep(u8 bank)
{
	return gBattleMons[bank].status1 & STATUS1_SLEEP
	#ifdef REALLY_SMART_AI
	&& gBattleMons[bank].status1 > 1 //Won't wake up this turn
	#endif
	;
}

bool8 IsBankIncapacitated(u8 bank)
{
	if (IsBankAsleep(bank))
		return TRUE;

	if (gBattleMons[bank].status1 & STATUS1_FREEZE)
		return TRUE;

	if (gBattleMons[bank].status2 & STATUS2_RECHARGE
	||  (ABILITY(bank) == ABILITY_TRUANT && gDisableStructs[bank].truantCounter != 0))
		return TRUE;

	return FALSE;
}

bool8 WillFaintFromWeatherSoon(u8 bank)
{
	if (TakesDamageFromSandstorm(bank) || TakesDamageFromHail(bank))
	{
		return gBattleMons[bank].hp <= GetBaseMaxHP(bank) / 16;
	}

	return FALSE;
}

bool8 WillTakeSignificantDamageFromEntryHazards(u8 bank, u8 healthFraction)
{
	u32 dmg = 0;

	if (gSideStatuses[SIDE(bank)] & SIDE_STATUS_SPIKES
	&& GetMonAbility(GetBankPartyData(bank)) != ABILITY_MAGICGUARD
	&& ITEM_EFFECT(bank) != ITEM_EFFECT_HEAVY_DUTY_BOOTS)
	{
		struct Pokemon* mon = GetBankPartyData(bank);

		if (gSideTimers[SIDE(bank)].srAmount > 0)
			dmg += CalcStealthRockDamagePartyMon(mon);

		if (gSideTimers[SIDE(bank)].steelsurge > 0)
			dmg += CalcSteelsurgeDamagePartyMon(mon);

		if (gSideTimers[SIDE(bank)].spikesAmount > 0)
			dmg += CalcSpikesDamagePartyMon(mon, SIDE(bank));

		if (dmg >= GetBaseCurrentHP(bank))
			return TRUE;

		if (dmg >= GetBaseMaxHP(bank) / healthFraction) //More or equal than a quarter of max health
			return TRUE;
	}

	return FALSE;
}

u8 CountBanksPositiveStatStages(u8 bank)
{
	u8 positiveStages = 0;
	for (int i = STAT_STAGE_ATK; i < BATTLE_STATS_NO; ++i)
	{
		if (STAT_STAGE(bank, i) > 6)
			++positiveStages;
	}

	return positiveStages;
}

u8 CountBanksNegativeStatStages(u8 bank)
{
	u8 negativeStages = 0;
	for (int i = STAT_STAGE_ATK; i < BATTLE_STATS_NO; ++i)
	{
		if (STAT_STAGE(bank, i) < 6)
			++negativeStages;
	}

	return negativeStages;
}

u16 GetTeamSpeedAverage(u8 bank)
{
	u8 firstId, lastId;
	struct Pokemon* party = LoadPartyRange(bank, &firstId, &lastId);

	u8 totalNum = 0;
	u16 sum = 0;

	for (int i = 0; i < PARTY_SIZE; ++i)
	{
		if (party[i].species == SPECIES_NONE
		|| GetMonData(&party[i], MON_DATA_IS_EGG, NULL))
			continue;

		++totalNum;
		sum += GetMonData(&party[i], MON_DATA_SPEED, NULL);
	}

	return sum / totalNum;
}

u16 GetPokemonOnSideSpeedAverage(u8 bank)
{
	u16 speed1 = 0;
	u16 speed2 = 0;
	u8 numBattlersAlive = 0;

	if (BATTLER_ALIVE(bank))
	{
		speed1 = SpeedCalc(bank);
		++numBattlersAlive;
	}

	if (IS_DOUBLE_BATTLE
	&&  BATTLER_ALIVE(PARTNER(bank)))
	{
		speed2 = SpeedCalc(PARTNER(bank));
		++numBattlersAlive;
	}

	return (speed1 + speed2) / numBattlersAlive;
}

u16 GetBattleMonMove(u8 bank, u8 i)
{
	u16 move;

	if (i >= MAX_MON_MOVES)
		return MOVE_NONE;

	#ifdef REALLY_SMART_AI
		move = gBattleMons[bank].moves[i];
	#else
		break_func(BATTLE_HISTORY->usedMoves) //Should throw error. We never want this to exist
		if (SIDE(bank) == B_SIDE_PLAYER && !(gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER))
			move = BATTLE_HISTORY->usedMoves[bank][i];
		else if (SIDE(bank) == B_SIDE_PLAYER && gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER && GetBattlerPosition(bank) == B_POSITION_PLAYER_LEFT)
			move = BATTLE_HISTORY->usedMoves[bank][i];
		else
			move = gBattleMons[bank].moves[i];
	#endif

	if (IsDynamaxed(bank))
	{
		gNewBS->ai.zMoveHelper = move; //Store the original move in memory for damage calcs later
		move = GetMaxMove(bank, i);
	}

	return move;
}

u8 GetAIAbility(u8 bankAtk, u8 bankDef, u16 move)
{
	u8 ability = ABILITY_NONE;

	if (!ShouldAIDelayMegaEvolution(bankAtk, bankDef, move))
		ability = GetBankMegaFormAbility(bankAtk);

	if (ability == ABILITY_NONE)
		return ABILITY(bankAtk);

	return ability;
}

u8 GetPredictedAIAbility(u8 bankAtk, u8 bankDef)
{
	u16 predictedUserMove = IsValidMovePrediction(bankAtk, bankDef);
	if (predictedUserMove != MOVE_NONE)
		return GetAIAbility(bankAtk, bankDef, predictedUserMove);
	else
		return ABILITY(bankAtk);
}

u16 GetAIChosenMove(u8 bankAtk, u8 bankDef)
{
	u16 move = gChosenMovesByBanks[bankAtk];
	if (move == MOVE_NONE)
		move = IsValidMovePrediction(bankAtk, bankDef);
	else if (gNewBS->zMoveData.toBeUsed[bankAtk]) //Pokemon chose a Z-Move
		move = ReplaceWithZMoveRuntime(bankAtk, move);
	else if (IsDynamaxed(bankAtk) || gNewBS->dynamaxData.toBeUsed[bankAtk])
	{
		u16 maxMove = GetMaxMoveByMove(bankAtk, move);
		if (maxMove != MOVE_NONE)
			move = maxMove;
	}

	return move;
}

bool8 IsTrapped(u8 bank, bool8 switching)
{
	if (IsOfType(bank, TYPE_GHOST)
	|| (switching && ITEM_EFFECT(bank) == ITEM_EFFECT_SHED_SHELL)
	|| (!switching && ABILITY(bank) == ABILITY_RUNAWAY)
	|| (!switching && ITEM_EFFECT(bank) == ITEM_EFFECT_CAN_ALWAYS_RUN))
		return FALSE;
	else
	{
		if (gBattleMons[bank].status2 & (STATUS2_ESCAPE_PREVENTION | STATUS2_WRAPPED)
		|| (ABILITY_ON_OPPOSING_FIELD(bank, ABILITY_SHADOWTAG) && ABILITY(bank) != ABILITY_SHADOWTAG)
		|| (ABILITY_ON_OPPOSING_FIELD(bank, ABILITY_ARENATRAP) && CheckGrounding(bank) == GROUNDED)
		|| (ABILITY_ON_OPPOSING_FIELD(bank, ABILITY_MAGNETPULL) && IsOfType(bank, TYPE_STEEL))
		|| gStatuses3[bank] & (STATUS3_ROOTED | STATUS3_SKY_DROP_TARGET)
		|| IsFairyLockActive())
			return TRUE;
	}

	return FALSE;
}

bool8 BankHasMonToSwitchTo(u8 bank)
{
	if (IS_SINGLE_BATTLE)
	{
		if (ViableMonCountFromBank(bank) <= 1)
			return FALSE;
	}
	else //Double
	{
		if (SIDE(bank) == B_SIDE_PLAYER)
		{
			if (IsTagBattle())
			{
				if (ViableMonCountFromBankLoadPartyRange(bank) <= 1)
					return FALSE;
			}
			else //Regular Double Battle
			{
				if (ViableMonCountFromBankLoadPartyRange(bank) <= 2)
					return FALSE;
			}
		}
		else //Opponent
		{
			if (IsTwoOpponentBattle())
			{
				if (ViableMonCountFromBankLoadPartyRange(bank) <= 1)
					return FALSE;
			}
			else //Regular Double Battle
			{
				if (ViableMonCountFromBankLoadPartyRange(bank) <= 2)
					return FALSE;
			}
		}
	}

	return TRUE;
}

bool8 IsTakingSecondaryDamage(u8 bank)
{
	u8 ability = ABILITY(bank);

	if (gStatuses3[bank] & STATUS3_PERISH_SONG)
		return TRUE;

	if (ability != ABILITY_MAGICGUARD)
	{
		if (TakesDamageFromSandstorm(bank)
		||  TakesDamageFromHail(bank)
		||  gWishFutureKnock.futureSightCounter[bank] == 1
		||  gStatuses3[bank] & STATUS3_LEECHSEED
		||  (gBattleMons[bank].status1 & STATUS1_PSN_ANY && ability != ABILITY_POISONHEAL)
		||  gBattleMons[bank].status1 & STATUS1_BURN
		||  ((gBattleMons[bank].status1 & STATUS1_SLEEP) > 1 && gBattleMons[bank].status2 & STATUS2_NIGHTMARE)
		||  gBattleMons[bank].status2 & (STATUS2_CURSED | STATUS2_WRAPPED)
		||	(BankSideHasSeaOfFire(bank) && !IsOfType(bank, TYPE_FIRE))
		||  (BankSideHasGMaxVineLash(bank) && !IsOfType(bank, TYPE_GRASS))
		||  (BankSideHasGMaxWildfire(bank) && !IsOfType(bank, TYPE_FIRE))
		||  (BankSideHasGMaxCannonade(bank) && !IsOfType(bank, TYPE_WATER))
		||  BankSideHasGMaxVolcalith(bank))
			return TRUE;
	}

	return FALSE;
}

bool8 WillFaintFromSecondaryDamage(u8 bank)
{
	u8 hp = gBattleMons[bank].hp + GetAmountToRecoverBy(bank, 0, MOVE_PROTECT); //Assume leftover etc. healing first
	u8 ability = ABILITY(bank);

	if (gStatuses3[bank] & STATUS3_PERISH_SONG
	&&  gDisableStructs[bank].perishSongTimer == 0)
		return TRUE;

	if (ability != ABILITY_MAGICGUARD)
	{
		u32 damage = 0;
		if (gWishFutureKnock.weatherDuration != 1)
			damage += GetSandstormDamage(bank)
				    + GetHailDamage(bank); //Weather's not about to end

		if ((gBattleMons[bank].status1 & STATUS1_SLEEP) != 1)
			damage += GetNightmareDamage(bank); //Sleep's not about to end

		if (!(gNewBS->brokeFreeMessage & gBitTable[bank]))
			damage += GetTrapDamage(bank); //Trapping isn't about to end

		if (damage
		+  GetLeechSeedDamage(bank)
		+  GetPoisonDamage(bank)
		+  GetBurnDamage(bank)
		+  GetCurseDamage(bank)
		+  GetSeaOfFireDamage(bank) //Sea of Fire runs on last turn
		+  GetGMaxVineLashDamage(bank)
		+  GetGMaxWildfireDamage(bank)
		+  GetGMaxCannonadeDamage(bank)
		+  GetGMaxVolcalithDamage(bank) >= hp)
			return TRUE;
	}

	return FALSE;
}

u16 CalcSecondaryEffectChance(u8 bank, u16 move)
{
	u16 chance = gBattleMoves[move].secondaryEffectChance;

	if (ABILITY(bank) == ABILITY_SERENEGRACE || BankSideHasRainbow(bank))
		chance *= 2;

	return chance;
}

u16 CalcAIAccuracy(u16 move, u8 bankAtk, u8 bankDef)
{
	u16 acc = AccuracyCalc(move, bankAtk, bankDef);

	if (gBattleMoves[move].effect == EFFECT_RECHARGE)
		acc = 49; //Set recharge moves to a pretty low number only higher than OHKO moves
	else if (gBattleMoves[move].effect == EFFECT_RAMPAGE || gBattleMoves[move].effect == EFFECT_ROLLOUT)
		acc -= 1; //Don't rampage if you don't have to
	else if (move == MOVE_FINALGAMBIT)
		acc -= 5; //Don't kill yourself if you don't have to

	return acc;
}

bool8 ShouldAIDelayMegaEvolution(u8 bankAtk, unusedArg u8 bankDef, u16 move)
{
	u8 atkAbility = ABILITY(bankAtk);
	u8 megaAbility = GetBankMegaFormAbility(bankAtk);

	if (BATTLER_SEMI_INVULNERABLE(bankAtk))
		return TRUE; //Can't Mega Evolve this turn

	if (atkAbility == megaAbility //Ability isn't changing
	||  megaAbility == ABILITY_NONE) //Can't Mega evolve or ability suppressed
		return FALSE;

	switch (atkAbility) {
		case ABILITY_SPEEDBOOST:
		case ABILITY_MOODY:
			switch (move) {
				case MOVE_PROTECT:
				case MOVE_DETECT:
				case MOVE_SPIKYSHIELD:
				case MOVE_KINGSSHIELD:
					return TRUE; //Delay Mega Evolution if using Protect for Speed Boost benefits
			}
			break;
/* With these uncommented, the game winds up in an endless loop due to AI_SpecialTypeCalc calling GetAIAbility in the damage calc
		case ABILITY_MOXIE:
		case ABILITY_BEASTBOOST:
		case ABILITY_SOULHEART: ;
			if (MoveWouldHitFirst(move, bankAtk, bankDef)
			&&  MoveKnocksOutXHits(move, bankAtk, bankDef, 1))
				return TRUE; //Get that Moxie boost
			break;
*/
	}

	return FALSE;
}

bool8 BadIdeaToPutToSleep(u8 bankDef, u8 bankAtk)
{
	u8 defItemEffect = ITEM_EFFECT(bankDef);
	u8 defAbility = ABILITY(bankDef);

	return !CanBePutToSleep(bankDef, TRUE)
		|| gStatuses3[bankDef] & STATUS3_YAWN
		|| defItemEffect == ITEM_EFFECT_CURE_SLP
		|| defItemEffect == ITEM_EFFECT_CURE_STATUS
		|| defAbility == ABILITY_EARLYBIRD
		|| defAbility == ABILITY_SHEDSKIN
		|| (defAbility == ABILITY_SYNCHRONIZE && CanBePutToSleep(bankAtk, TRUE))
		|| (defAbility == ABILITY_HYDRATION && gBattleWeather & WEATHER_RAIN_ANY && gWishFutureKnock.weatherDuration != 1)
		|| (IS_DOUBLE_BATTLE && BATTLER_ALIVE(PARTNER(bankDef)) && ABILITY(PARTNER(bankDef)) == ABILITY_HEALER);
}

bool8 BadIdeaToPoison(u8 bankDef, u8 bankAtk)
{
	u8 defItemEffect = ITEM_EFFECT(bankDef);
	u8 defAbility = ABILITY(bankDef);
	u8 atkAbility = ABILITY(bankAtk);

	return !CanBePoisoned(bankDef, bankAtk, TRUE)
		||  defItemEffect == ITEM_EFFECT_CURE_PSN
		||  defItemEffect == ITEM_EFFECT_CURE_STATUS
		||  defAbility == ABILITY_SHEDSKIN
		||  defAbility == ABILITY_POISONHEAL
		||  defAbility == ABILITY_MAGICGUARD
		||  defAbility == ABILITY_QUICKFEET
		|| (defAbility == ABILITY_SYNCHRONIZE && CanBePoisoned(bankAtk, bankDef, TRUE) && !GoodIdeaToPoisonSelf(bankAtk))
		|| (defAbility == ABILITY_MARVELSCALE && PhysicalMoveInMoveset(bankAtk))
		|| (defAbility == ABILITY_NATURALCURE && CAN_SWITCH_OUT(bankDef))
		|| (defAbility == ABILITY_TOXICBOOST && PhysicalMoveInMoveset(bankDef))
		|| (defAbility == ABILITY_GUTS && PhysicalMoveInMoveset(bankDef))
		|| (atkAbility == ABILITY_POISONTOUCH && ContactMovesThatAffectTargetInMoveset(bankAtk, bankDef)) //Just poison it using attacker's ability
		|| (defAbility == ABILITY_HYDRATION && gBattleWeather & WEATHER_RAIN_ANY && gWishFutureKnock.weatherDuration != 1)
		|| (IS_DOUBLE_BATTLE && BATTLER_ALIVE(PARTNER(bankDef)) && ABILITY(PARTNER(bankDef)) == ABILITY_HEALER)
		||  MoveInMoveset(MOVE_FACADE, bankDef)
		||  MoveInMoveset(MOVE_PSYCHOSHIFT, bankDef);
}

bool8 GoodIdeaToPoisonSelf(u8 bankAtk)
{
	u8 atkAbility = ABILITY(bankAtk);

	return CanBePoisoned(bankAtk, bankAtk, FALSE)
		&&  (atkAbility == ABILITY_MARVELSCALE
		 ||  atkAbility == ABILITY_POISONHEAL
		 ||  atkAbility == ABILITY_QUICKFEET
		 ||  atkAbility == ABILITY_MAGICGUARD
		 || (atkAbility == ABILITY_TOXICBOOST && PhysicalMoveInMoveset(bankAtk))
		 || (atkAbility == ABILITY_GUTS && PhysicalMoveInMoveset(bankAtk))
		 ||  MoveInMoveset(MOVE_FACADE, bankAtk)
		 ||  MoveInMoveset(MOVE_PSYCHOSHIFT, bankAtk));
}

bool8 BadIdeaToParalyze(u8 bankDef, u8 bankAtk)
{
	u8 defItemEffect = ITEM_EFFECT(bankDef);
	u8 defAbility = ABILITY(bankDef);

	return !CanBeParalyzed(bankDef, TRUE)
	   ||  defItemEffect == ITEM_EFFECT_CURE_PAR
	   ||  defItemEffect == ITEM_EFFECT_CURE_STATUS
	   ||  defAbility == ABILITY_SHEDSKIN
	   ||  defAbility == ABILITY_QUICKFEET
	   || (defAbility == ABILITY_SYNCHRONIZE && CanBeParalyzed(bankAtk, TRUE) && !GoodIdeaToParalyzeSelf(bankAtk))
	   || (defAbility == ABILITY_MARVELSCALE && PhysicalMoveInMoveset(bankAtk))
	   || (defAbility == ABILITY_NATURALCURE && CAN_SWITCH_OUT(bankDef))
	   || (defAbility == ABILITY_GUTS && PhysicalMoveInMoveset(bankDef))
	   || (defAbility == ABILITY_HYDRATION && gBattleWeather & WEATHER_RAIN_ANY && gWishFutureKnock.weatherDuration != 1)
	   || (IS_DOUBLE_BATTLE && BATTLER_ALIVE(PARTNER(bankDef)) && ABILITY(PARTNER(bankDef)) == ABILITY_HEALER)
	   ||  MoveInMoveset(MOVE_FACADE, bankDef)
	   ||  MoveInMoveset(MOVE_PSYCHOSHIFT, bankDef);
}

bool8 GoodIdeaToParalyzeSelf(u8 bankAtk)
{
	u8 atkAbility = ABILITY(bankAtk);

	return CanBeParalyzed(bankAtk, FALSE)
		&&  (atkAbility == ABILITY_MARVELSCALE
		 ||  atkAbility == ABILITY_QUICKFEET
		 || (atkAbility == ABILITY_GUTS && PhysicalMoveInMoveset(bankAtk))
		 || MoveInMoveset(MOVE_FACADE, bankAtk)
		 || MoveInMoveset(MOVE_PSYCHOSHIFT, bankAtk));
}

bool8 BadIdeaToBurn(u8 bankDef, u8 bankAtk)
{
	u8 defItemEffect = ITEM_EFFECT(bankDef);
	u8 defAbility = ABILITY(bankDef);

	return !CanBeBurned(bankDef, TRUE)
		||  defItemEffect == ITEM_EFFECT_CURE_BRN
		||  defItemEffect == ITEM_EFFECT_CURE_STATUS
		||  defAbility == ABILITY_SHEDSKIN
		||  defAbility == ABILITY_MAGICGUARD
		||  defAbility == ABILITY_QUICKFEET
		|| (defAbility == ABILITY_SYNCHRONIZE && CanBeBurned(bankAtk, TRUE) && !GoodIdeaToBurnSelf(bankAtk))
		|| (defAbility == ABILITY_MARVELSCALE && PhysicalMoveInMoveset(bankAtk))
		|| (defAbility == ABILITY_NATURALCURE && CAN_SWITCH_OUT(bankDef))
		|| (defAbility == ABILITY_FLAREBOOST && SpecialMoveInMoveset(bankDef))
		|| (defAbility == ABILITY_GUTS && PhysicalMoveInMoveset(bankDef))
		|| (defAbility == ABILITY_HYDRATION && gBattleWeather & WEATHER_RAIN_ANY && gWishFutureKnock.weatherDuration != 1)
		|| (IS_DOUBLE_BATTLE && BATTLER_ALIVE(PARTNER(bankDef)) && ABILITY(PARTNER(bankDef)) == ABILITY_HEALER)
		||  MoveInMoveset(MOVE_FACADE, bankDef)
		||  MoveInMoveset(MOVE_PSYCHOSHIFT, bankDef);
}

bool8 GoodIdeaToBurnSelf(u8 bankAtk)
{
	u8 atkAbility = ABILITY(bankAtk);

	return CanBeBurned(bankAtk, FALSE)
		&&  (atkAbility == ABILITY_QUICKFEET
		 ||  atkAbility == ABILITY_HEATPROOF
		 ||  atkAbility == ABILITY_MAGICGUARD
		 || (atkAbility == ABILITY_FLAREBOOST && SpecialMoveInMoveset(bankAtk))
		 || (atkAbility == ABILITY_GUTS && PhysicalMoveInMoveset(bankAtk))
		 || MoveInMoveset(MOVE_FACADE, bankAtk)
		 || MoveInMoveset(MOVE_PSYCHOSHIFT, bankAtk));
}

bool8 BadIdeaToFreeze(u8 bankDef, u8 bankAtk)
{
	u8 defAbility = ABILITY(bankDef);
	u8 defItemEffect = ITEM_EFFECT(bankDef);

	return !CanBeFrozen(bankDef, TRUE)
		|| defItemEffect != ITEM_EFFECT_CURE_FRZ
		|| defItemEffect != ITEM_EFFECT_CURE_STATUS
		|| (defAbility == ABILITY_SYNCHRONIZE && CanBeFrozen(bankAtk, TRUE))
		|| (defAbility == ABILITY_NATURALCURE && CAN_SWITCH_OUT(bankDef)) //Don't waste a one-time freeze
		|| UnfreezingMoveInMoveset(bankDef);
}

bool8 GoodIdeaToLowerAttack(u8 bankDef, u8 bankAtk, u16 move)
{
	if (!MoveWouldHitFirst(move, bankAtk, bankDef) && CanKnockOut(bankAtk, bankDef))
		return FALSE; //Don't bother lowering stats if can kill enemy.

	u8 defAbility = ABILITY(bankDef);

	return STAT_STAGE(bankDef, STAT_STAGE_ATK) > 4 && PhysicalMoveInMoveset(bankDef)
		&& defAbility != ABILITY_CONTRARY
		&& defAbility != ABILITY_CLEARBODY
		&& defAbility != ABILITY_WHITESMOKE
		//&& defAbility != ABILITY_FULLMETALBODY
		&& defAbility != ABILITY_HYPERCUTTER;
}

bool8 GoodIdeaToLowerDefense(u8 bankDef, u8 bankAtk, u16 move)
{
	if (!MoveWouldHitFirst(move, bankAtk, bankDef) && CanKnockOut(bankAtk, bankDef))
		return FALSE; //Don't bother lowering stats if can kill enemy.

	u8 defAbility = ABILITY(bankDef);

	return STAT_STAGE(bankDef, STAT_STAGE_DEF) > 4
		&& PhysicalMoveInMoveset(bankAtk)
		&& defAbility != ABILITY_CONTRARY
		&& defAbility != ABILITY_CLEARBODY
		&& defAbility != ABILITY_WHITESMOKE
		//&& defAbility != ABILITY_FULLMETALBODY
		&& defAbility != ABILITY_BIGPECKS;
}

bool8 GoodIdeaToLowerSpAtk(u8 bankDef, u8 bankAtk, u16 move)
{
	if (!MoveWouldHitFirst(move, bankAtk, bankDef) && CanKnockOut(bankAtk, bankDef))
		return FALSE; //Don't bother lowering stats if can kill enemy.

	u8 defAbility = ABILITY(bankDef);

	return STAT_STAGE(bankDef, STAT_STAGE_SPATK) > 4 && SpecialMoveInMoveset(bankDef)
		&& defAbility != ABILITY_CONTRARY
		&& defAbility != ABILITY_CLEARBODY
		//&& defAbility != ABILITY_FULLMETALBODY
		&& defAbility != ABILITY_WHITESMOKE;
}

bool8 GoodIdeaToLowerSpDef(u8 bankDef, u8 bankAtk, u16 move)
{
	if (!MoveWouldHitFirst(move, bankAtk, bankDef) && CanKnockOut(bankAtk, bankDef))
		return FALSE; //Don't bother lowering stats if can kill enemy.

	u8 defAbility = ABILITY(bankDef);

	return STAT_STAGE(bankDef, STAT_STAGE_SPDEF) > 4 && SpecialMoveInMoveset(bankAtk)
		&& defAbility != ABILITY_CONTRARY
		&& defAbility != ABILITY_CLEARBODY
		//&& defAbility != ABILITY_FULLMETALBODY
		&& defAbility != ABILITY_WHITESMOKE;
}

bool8 GoodIdeaToLowerSpeed(u8 bankDef, u8 bankAtk, u16 move)
{
	if (!MoveWouldHitFirst(move, bankAtk, bankDef) && CanKnockOut(bankAtk, bankDef))
		return FALSE; //Don't bother lowering stats if can kill enemy.

	u8 defAbility = ABILITY(bankDef);

	return SpeedCalc(bankAtk) <= SpeedCalc(bankDef)
		&& defAbility != ABILITY_CONTRARY
		&& defAbility != ABILITY_CLEARBODY
		//&& defAbility != ABILITY_FULLMETALBODY
		&& defAbility != ABILITY_WHITESMOKE;
}

bool8 GoodIdeaToLowerAccuracy(u8 bankDef, u8 bankAtk, u16 move)
{
	if (!MoveWouldHitFirst(move, bankAtk, bankDef) && CanKnockOut(bankAtk, bankDef))
		return FALSE; //Don't bother lowering stats if can kill enemy.

	u8 defAbility = ABILITY(bankDef);

	return defAbility != ABILITY_CONTRARY
		&& defAbility != ABILITY_CLEARBODY
		&& defAbility != ABILITY_WHITESMOKE
		//&& defAbility != ABILITY_FULLMETALBODY
		&& defAbility != ABILITY_KEENEYE;
}

bool8 GoodIdeaToLowerEvasion(u8 bankDef, u8 bankAtk, unusedArg u16 move)
{
	u8 defAbility = ABILITY(bankDef);

	return (STAT_STAGE(bankDef, STAT_STAGE_EVASION) > 6 || MoveInMovesetWithAccuracyLessThan(bankAtk, bankDef, 90, TRUE))
		&& defAbility != ABILITY_CONTRARY
		&& defAbility != ABILITY_CLEARBODY
		//&& defAbility != ABILITY_FULLMETALBODY
		&& defAbility != ABILITY_WHITESMOKE;
}

//Move Prediction Code

move_t IsValidMovePrediction(u8 bankAtk, u8 bankDef)
{
	if (gNewBS->ai.movePredictions[bankAtk][bankDef] == MOVE_PREDICTION_SWITCH)
		return MOVE_NONE;
	else
		return gNewBS->ai.movePredictions[bankAtk][bankDef];
}

bool8 IsPredictedToSwitch(u8 bankAtk, u8 bankDef)
{
	return gNewBS->ai.movePredictions[bankAtk][bankDef] == MOVE_PREDICTION_SWITCH;
}

void StoreMovePrediction(u8 bankAtk, u8 bankDef, u16 move)
{
	gNewBS->ai.movePredictions[bankAtk][bankDef] = move;
}

void StoreSwitchPrediction(u8 bankAtk, u8 bankDef)
{
	gNewBS->ai.movePredictions[bankAtk][bankDef] = MOVE_PREDICTION_SWITCH;
}

bool8 IsMovePredictionSemiInvulnerable(u8 bankAtk, u8 bankDef)
{
	u16 move = IsValidMovePrediction(bankAtk, bankDef);

	if (move != MOVE_NONE)
	{
		u8 effect = gBattleMoves[move].effect;
		return effect == EFFECT_SEMI_INVULNERABLE;

	}

	return FALSE;
}

bool8 IsMovePredictionHealingMove(u8 bankAtk, u8 bankDef)
{
	u16 move = IsValidMovePrediction(bankAtk, bankDef);

	if (move != MOVE_NONE)
	{
		u8 effect = gBattleMoves[move].effect;
		return effect == EFFECT_RESTORE_HP
			|| effect == EFFECT_MORNING_SUN
			|| effect == EFFECT_SWALLOW
			|| effect == EFFECT_WISH;
	}

	return FALSE;
}

bool8 IsPredictedToUsePursuitableMove(u8 bankAtk, u8 bankDef)
{
	u16 move = IsValidMovePrediction(bankAtk, bankDef);

	if (move != MOVE_NONE)
	{
		u8 effect = gBattleMoves[move].effect;
		return effect == EFFECT_BATON_PASS && move != MOVE_BATONPASS;
	}

	return FALSE;
}

bool8 IsMovePredictionPhazingMove(u8 bankAtk, u8 bankDef)
{
	u16 move = IsValidMovePrediction(bankAtk, bankDef);

	if (IsDynamaxed(bankDef))
		return FALSE; //Dynamax Pokemon can't be phazed out

	if (move != MOVE_NONE)
	{
		u8 effect = gBattleMoves[move].effect;
		return effect == EFFECT_ROAR || effect == EFFECT_HAZE || effect == EFFECT_REMOVE_TARGET_STAT_CHANGES;
	}

	return FALSE;
}

//bankAtk is the protector
bool8 CanMovePredictionProtectAgainstMove(u8 bankAtk, u8 bankDef, u16 move)
{
	u16 protectMove = IsValidMovePrediction(bankAtk, bankDef);

	if (move != MOVE_NONE && gBattleMoves[protectMove].effect == EFFECT_PROTECT)
	{
		return DoesProtectionMoveBlockMove(bankDef, bankAtk, move, protectMove); //The "target" is the one not using Protection move and attacking
	}

	return FALSE;
}

bool8 DamagingMoveInMoveset(u8 bank)
{
	u16 move;
	u8 moveLimitations = CheckMoveLimitations(bank, 0, 0xFF);

	for (int i = 0; i < MAX_MON_MOVES; ++i)
	{
		move = GetBattleMonMove(bank, i);
		if (move == MOVE_NONE)
			break;

		if (!(gBitTable[i] & moveLimitations))
		{
			if (SPLIT(move) != SPLIT_STATUS
			&& gBattleMoves[move].power != 0
			&& gBattleMoves[move].effect != EFFECT_COUNTER
			&& gBattleMoves[move].effect != EFFECT_MIRROR_COAT)
				return TRUE;
		}
	}

	return FALSE;
}

bool8 PhysicalMoveInMoveset(u8 bank)
{
	u16 move;
	u8 moveLimitations = CheckMoveLimitations(bank, 0, 0xFF);

	for (int i = 0; i < MAX_MON_MOVES; ++i)
	{
		move = GetBattleMonMove(bank, i);
		if (move == MOVE_NONE)
			break;

		if (!(gBitTable[i] & moveLimitations))
		{
			if (CalcMoveSplit(bank, move) == SPLIT_PHYSICAL
			&& gBattleMoves[move].power != 0
			&& gBattleMoves[move].effect != EFFECT_COUNTER)
				return TRUE;
		}
	}

	return FALSE;
}

bool8 SpecialMoveInMoveset(u8 bank)
{
	u16 move;
	u8 moveLimitations = CheckMoveLimitations(bank, 0, 0xFF);

	for (int i = 0; i < MAX_MON_MOVES; ++i)
	{
		move = GetBattleMonMove(bank, i);
		if (move == MOVE_NONE)
			break;

		if (!(gBitTable[i] & moveLimitations))
		{
			if (CalcMoveSplit(bank, move) == SPLIT_SPECIAL
			&& gBattleMoves[move].power != 0
			&& gBattleMoves[move].effect != EFFECT_MIRROR_COAT)
				return TRUE;
		}
	}

	return FALSE;
}

bool8 MoveSplitInMoveset(u8 bank, u8 moveSplit)
{
	switch (moveSplit) {
		case SPLIT_PHYSICAL:
			return PhysicalMoveInMoveset(bank);
		case SPLIT_SPECIAL:
			return SpecialMoveInMoveset(bank);
		default:
			return !PhysicalMoveInMoveset(bank) && !SpecialMoveInMoveset(bank);
	}
}

bool8 PhysicalMoveInMonMoveset(struct Pokemon* mon, u8 moveLimitations)
{
	u16 move;

	moveLimitations = CheckMoveLimitationsFromParty(mon, 0, moveLimitations);
	for (int i = 0; i < MAX_MON_MOVES; ++i)
	{
		move = GetMonData(mon, MON_DATA_MOVE1 + i, NULL);
		if (move == MOVE_NONE)
			break;

		if (!(gBitTable[i] & moveLimitations))
		{
			if (CalcMoveSplitFromParty(mon, move) == SPLIT_PHYSICAL
			&& gBattleMoves[move].power != 0
			&& gBattleMoves[move].effect != EFFECT_COUNTER)
				return TRUE;
		}
	}

	return FALSE;
}

bool8 SpecialMoveInMonMoveset(struct Pokemon* mon, u8 moveLimitations)
{
	u16 move;

	moveLimitations = CheckMoveLimitationsFromParty(mon, 0, moveLimitations);
	for (int i = 0; i < MAX_MON_MOVES; ++i)
	{
		move = GetMonData(mon, MON_DATA_MOVE1 + i, NULL);
		if (move == MOVE_NONE)
			break;

		if (!(gBitTable[i] & moveLimitations))
		{
			if (CalcMoveSplitFromParty(mon, move) == SPLIT_SPECIAL
			&& gBattleMoves[move].power != 0
			&& gBattleMoves[move].effect != EFFECT_MIRROR_COAT)
				return TRUE;
		}
	}

	return FALSE;
}

bool8 MagicCoatableMovesInMoveset(u8 bank)
{
	u16 move;
	u8 moveLimitations = CheckMoveLimitations(bank, 0, 0xFF);

	for (int i = 0; i < MAX_MON_MOVES; ++i)
	{
		move = GetBattleMonMove(bank, i);
		if (move == MOVE_NONE)
			break;

		if (!(gBitTable[i] & moveLimitations))
		{
			if (gBattleMoves[move].flags & FLAG_MAGIC_COAT_AFFECTED)
				return TRUE;
		}
	}
	return FALSE;
}

bool8 HasProtectionMoveInMoveset(u8 bank, u8 checkType)
{
	u16 move;
	u8 moveLimitations = CheckMoveLimitations(bank, 0, 0xFF);

	for (int i = 0; i < MAX_MON_MOVES; ++i)
	{
		move = GetBattleMonMove(bank, i);
		if (move == MOVE_NONE)
			break;

		if (!(gBitTable[i] & moveLimitations))
		{
			if (gBattleMoves[gBattleMons[bank].moves[i]].effect == EFFECT_PROTECT)
			{
				switch (move) {
					case MOVE_PROTECT:
					case MOVE_SPIKYSHIELD:
					case MOVE_KINGSSHIELD:
						return TRUE;

					case MOVE_QUICKGUARD:
						if (checkType & CHECK_QUICK_GUARD)
							return TRUE;
						break;

					case MOVE_WIDEGUARD:
						if (checkType & CHECK_WIDE_GUARD)
							return TRUE;
						break;

					case MOVE_CRAFTYSHIELD:
						if (checkType & CHECK_CRAFTY_SHIELD)
							return TRUE;
						break;

					case MOVE_MATBLOCK:
						if (checkType & CHECK_MAT_BLOCK
						&&  gDisableStructs[bank].isFirstTurn)
							return TRUE;
						break;
				}
			}
		}
	}

	return FALSE;
}

// AI function to check if any move is of given type
bool8 MoveTypeInMoveset(u8 bank, u8 moveType)
{
	u16 move;
	u8 moveLimitations = CheckMoveLimitations(bank, 0, 0xFF);

	for (int i = 0; i < MAX_MON_MOVES; ++i)
	{
		move = GetBattleMonMove(bank, i);
		if (move == MOVE_NONE)
			break;

		if (!(gBitTable[i] & moveLimitations))
		{
			if (GetMoveTypeSpecial(bank, move) == moveType)
				return TRUE;
		}
	}

	return FALSE;
}

bool8 DamagingMoveTypeInMoveset(u8 bank, u8 moveType)
{
	u16 move;
	u8 moveLimitations = CheckMoveLimitations(bank, 0, 0xFF);

	for (int i = 0; i < MAX_MON_MOVES; ++i)
	{
		move = GetBattleMonMove(bank, i);
		if (move == MOVE_NONE)
			break;

		if (!(gBitTable[i] & moveLimitations))
		{
			if (GetMoveTypeSpecial(bank, move) == moveType
			&&  SPLIT(move) != SPLIT_STATUS)
				return TRUE;
		}
	}

	return FALSE;
}

bool8 DamagingAllHitMoveTypeInMoveset(u8 bank, u8 moveType)
{
	u16 move;
	u8 moveLimitations = CheckMoveLimitations(bank, 0, 0xFF);

	for (int i = 0; i < MAX_MON_MOVES; ++i)
	{
		move = GetBattleMonMove(bank, i);
		if (move == MOVE_NONE)
			break;

		if (!(gBitTable[i] & moveLimitations))
		{
			if (GetMoveTypeSpecial(bank, move) == moveType
			&&  SPLIT(move) != SPLIT_STATUS
			&&  gBattleMoves[move].target & MOVE_TARGET_ALL)
				return TRUE;
		}
	}

	return FALSE;
}

bool8 DamagingSpreadMoveInMoveset(u8 bank)
{
	u16 move;
	u8 moveLimitations = CheckMoveLimitations(bank, 0, 0xFF);

	for (int i = 0; i < MAX_MON_MOVES; ++i)
	{
		move = GetBattleMonMove(bank, i);
		if (move == MOVE_NONE)
			break;

		if (!(gBitTable[i] & moveLimitations))
		{
			if (SPLIT(move) != SPLIT_STATUS
			&&  gBattleMoves[move].target & (MOVE_TARGET_BOTH | MOVE_TARGET_ALL))
				return TRUE;
		}
	}

	return FALSE;
}

// AI Function to check if bank has a snatchable move in moveset
bool8 HasSnatchableMove(u8 bank)
{
	u16 move;
	u8 moveLimitations = CheckMoveLimitations(bank, 0, 0xFF);

	for (int i = 0; i < MAX_MON_MOVES; ++i)
	{
		move = GetBattleMonMove(bank, i);
		if (move == MOVE_NONE)
			break;

		if (!(gBitTable[i] & moveLimitations))
		{
			if (gBattleMoves[move].flags & FLAG_SNATCH_AFFECTED)
				return TRUE;
		}
	}

	return FALSE;
}

// AI function to see if bank has a move with specific move effect
bool8 MoveEffectInMoveset(u8 moveEffect, u8 bank)
{
	u16 move;
	u8 moveLimitations = CheckMoveLimitations(bank, 0, 0xFF);

	for (int i = 0; i < MAX_MON_MOVES; ++i)
	{
		move = GetBattleMonMove(bank, i);
		if (move == MOVE_NONE)
			break;

		if (!(gBitTable[i] & moveLimitations))
		{
			if (gBattleMoves[move].effect == moveEffect)
				return TRUE;
		}
	}

	return FALSE;
}

//AI function to check if bank has a status move in their moveset
bool8 StatusMoveInMoveset(u8 bank)
{
	u16 move;
	u8 moveLimitations = CheckMoveLimitations(bank, 0, 0xFF);

	for (int i = 0; i < MAX_MON_MOVES; ++i)
	{
		move = GetBattleMonMove(bank, i);
		if (move == MOVE_NONE)
			break;

		if (!(gBitTable[i] & moveLimitations))
		{
			if (CalcMoveSplit(bank, move) == SPLIT_STATUS)
				return TRUE;
		}
	}

	return FALSE;
}

bool8 MoveInMovesetWithAccuracyLessThan(u8 bankAtk, u8 bankDef, u8 acc, bool8 ignoreStatusMoves)
{
	u16 move;
	u8 moveLimitations = CheckMoveLimitations(bankAtk, 0, 0xFF);

	for (int i = 0; i < MAX_MON_MOVES; ++i)
	{
		move = GetBattleMonMove(bankAtk, i);
		if (move == MOVE_NONE)
			break;

		if (!(gBitTable[i] & moveLimitations))
		{
			if (ignoreStatusMoves && SPLIT(move) == SPLIT_STATUS)
				continue;

			if (gBattleMoves[move].accuracy == 0 //Always hits
			||  gBattleMoves[move].target & (MOVE_TARGET_USER | MOVE_TARGET_OPPONENTS_FIELD))
				continue;

			if (AccuracyCalc(move, bankAtk, bankDef) < acc)
				return TRUE;
		}
	}

	return FALSE;
}

bool8 FlinchingMoveInMoveset(u8 bank)
{
	u16 move;
	u8 moveLimitations = CheckMoveLimitations(bank, 0, 0xFF);

	for (int i = 0; i < MAX_MON_MOVES; ++i)
	{
		move = GetBattleMonMove(bank, i);
		if (move == MOVE_NONE)
			break;

		if (!(gBitTable[i] & moveLimitations))
		{
			if (gBattleMoves[move].effect == EFFECT_FLINCH_HIT)
				return TRUE;
		}
	}

	return FALSE;
}

bool8 HealingMoveInMoveset(u8 bank)
{
	u16 move;
	u8 moveLimitations = CheckMoveLimitations(bank, 0, 0xFF);

	for (int i = 0; i < MAX_MON_MOVES; ++i)
	{
		move = GetBattleMonMove(bank, i);
		if (move == MOVE_NONE)
			break;

		if (!(gBitTable[i] & moveLimitations))
		{
			u8 effect = gBattleMoves[move].effect;
			if (effect == EFFECT_RESTORE_HP
			||  effect == EFFECT_MORNING_SUN
			||  effect == EFFECT_SWALLOW
			||  effect == EFFECT_WISH)
				return TRUE;
		}
	}

	return FALSE;
}

bool8 HealingMoveInMonMoveset(struct Pokemon* mon)
{
	u16 move;
	u8 moveLimitations = CheckMoveLimitationsFromParty(mon, 0, 0xFF);

	for (int i = 0; i < MAX_MON_MOVES; ++i)
	{
		move = GetMonData(mon, MON_DATA_MOVE1 + i, NULL);
		if (move == MOVE_NONE)
			break;

		if (!(gBitTable[i] & moveLimitations))
		{
			u8 effect = gBattleMoves[move].effect;
			if (effect == EFFECT_RESTORE_HP
			||  effect == EFFECT_MORNING_SUN
			||  effect == EFFECT_SWALLOW
			||  effect == EFFECT_WISH)
				return TRUE;
		}
	}

	return FALSE;
}

bool8 SoundMoveInMoveset(u8 bank)
{
	u16 move;
	u8 moveLimitations = CheckMoveLimitations(bank, 0, 0xFF);

	for (int i = 0; i < MAX_MON_MOVES; ++i)
	{
		move = GetBattleMonMove(bank, i);
		if (move == MOVE_NONE)
			break;

		if (!(gBitTable[i] & moveLimitations))
		{
			if (CheckSoundMove(move))
				return TRUE;
		}
	}

	return FALSE;
}

bool8 MoveThatCanHelpAttacksHitInMoveset(u8 bank)
{
	u16 move;
	u8 moveLimitations = CheckMoveLimitations(bank, 0, 0xFF);

	for (int i = 0; i < MAX_MON_MOVES; ++i)
	{
		move = GetBattleMonMove(bank, i);
		if (move == MOVE_NONE)
			break;

		if (!(gBitTable[i] & moveLimitations))
		{
			switch (gBattleMoves[move].effect) {
				case EFFECT_ACCURACY_UP:
				case EFFECT_ACCURACY_UP_2:
				case EFFECT_EVASION_DOWN:
				case EFFECT_EVASION_DOWN_2:
					return TRUE;

				case EFFECT_LOCK_ON:
					if (move != MOVE_LASERFOCUS)
						return TRUE;
					break;
			}

			switch (move) {
				case MOVE_HONECLAWS:
				case MOVE_COIL:
				case MOVE_DEFOG:
					return TRUE;
			}
		}
	}

	return FALSE;
}

bool8 DamagingMoveThaCanBreakThroughSubstituteInMoveset(u8 bankAtk, u8 bankDef)
{
	u8 moveLimitations = CheckMoveLimitations(bankAtk, 0, 0xFF);

	for (int i = 0; i < MAX_MON_MOVES; ++i)
	{
		u16 move = GetBattleMonMove(bankAtk, i);

		if (move == MOVE_NONE)
			break;

		if (!(gBitTable[i] & moveLimitations))
		{
			if (SPLIT(move) != SPLIT_STATUS //Damaging move
			&& !MoveBlockedBySubstitute(move, bankAtk, bankDef))
				return TRUE;
		}
	}

	return FALSE;
}

bool8 ContactMovesThatAffectTargetInMoveset(u8 bankAtk, u8 bankDef)
{
	u8 moveLimitations = CheckMoveLimitations(bankAtk, 0, 0xFF);

	for (int i = 0; i < MAX_MON_MOVES; ++i)
	{
		u16 move = GetBattleMonMove(bankAtk, i);

		if (move == MOVE_NONE)
			break;

		if (!(gBitTable[i] & moveLimitations))
		{
			if (CheckContact(move, bankAtk)
			&& !(AI_SpecialTypeCalc(move, bankDef, bankAtk) & MOVE_RESULT_NO_EFFECT))
				return TRUE;
		}
	}

	return FALSE;
}

bool8 UnfreezingMoveInMoveset(u8 bank)
{
	u8 moveLimitations = CheckMoveLimitations(bank, 0, 0xFF);

	for (int i = 0; i < MAX_MON_MOVES; ++i)
	{
		u16 move = GetBattleMonMove(bank, i);

		if (move == MOVE_NONE)
			break;

		if (!(gBitTable[i] & moveLimitations))
		{
			if (CheckTableForMove(move, gMovesCanUnfreezeAttacker))
				return TRUE;
		}
	}

	return FALSE;
}

bool8 SleepMoveInMovesetWithLowAccuracy(u8 bankAtk, u8 bankDef)
{
	u8 moveLimitations = CheckMoveLimitations(bankAtk, 0, 0xFF);

	for (int i = 0; i < MAX_MON_MOVES; ++i)
	{
		u16 move = GetBattleMonMove(bankAtk, i);

		if (move == MOVE_NONE)
			break;

		if (!(gBitTable[i] & moveLimitations))
		{
			if (gBattleMoves[move].effect == EFFECT_SLEEP && AccuracyCalc(move, bankAtk, bankDef) < 85)
				return TRUE;
		}
	}

	return FALSE;
}

static bool8 WallsFoe(u8 bankAtk, u8 bankDef)
{
	u32 attack, spAttack;
	bool8 cantWall = FALSE;

	APPLY_STAT_MOD(attack, &gBattleMons[bankAtk], gBattleMons[bankAtk].attack, STAT_STAGE_ATK);
	APPLY_STAT_MOD(spAttack, &gBattleMons[bankAtk], gBattleMons[bankAtk].spAttack, STAT_STAGE_SPATK);

	if (PhysicalMoveInMoveset(bankAtk) && gBattleMons[bankDef].defense <= attack)
		cantWall = TRUE;
	else if (SpecialMoveInMoveset(bankAtk) && gBattleMons[bankDef].spDefense <= spAttack)
		cantWall = TRUE;

	return !cantWall;
}

static bool8 ShouldAIFreeChoiceLockWithDynamax(u8 bankAtk, u8 bankDef)
{
	if (CanDynamax(bankAtk)
	&& (ITEM_EFFECT(bankAtk) == ITEM_EFFECT_CHOICE_BAND || ABILITY(bankAtk) == ABILITY_GORILLATACTICS)
	&& CHOICED_MOVE(bankAtk) != MOVE_NONE && CHOICED_MOVE(bankAtk) != 0xFFFF) //AI is locked into some move
	{
		u8 moveLimitations = CheckMoveLimitations(bankAtk, 0, MOVE_LIMITATION_ZEROMOVE | MOVE_LIMITATION_PP);
		struct AIScript aiScriptData = {0};
		PopulateAIScriptStructWithBaseAttackerData(&aiScriptData, bankAtk);
		PopulateAIScriptStructWithBaseDefenderData(&aiScriptData, bankDef);
		for (int i = 0; i < MAX_MON_MOVES; ++i)
		{
			u16 move = GetMaxMove(bankAtk, i);
			if (move == MOVE_NONE)
				break;

			if (!(gBitTable[i] & moveLimitations))
			{
				if (AIScript_Negatives(bankAtk, bankDef, move, 100, &aiScriptData) >= 100)
					return TRUE;
			}
		}
	}

	return FALSE; //AI still has no usable moves after Dynamaxing
}

static bool8 CalcOnlyBadMovesLeftInMoveset(u8 bankAtk, u8 bankDef)
{
	int i;
	u8 viability;
	u16 move;
	u32 dmg;
	u8 numDamageMoves = 0;
	bool8 hasSuperEffectiveMove = FALSE;

	if (AI_THINKING_STRUCT->aiFlags == AI_SCRIPT_CHECK_BAD_MOVE) //Only basic AI
		return FALSE; //The dumb AI doesn't get to switch like this

	u8 moveLimitations = CheckMoveLimitations(bankAtk, 0, 0xFF);
	struct AIScript aiScriptData = {0};
	PopulateAIScriptStructWithBaseAttackerData(&aiScriptData, bankAtk);
	PopulateAIScriptStructWithBaseDefenderData(&aiScriptData, bankDef);
	for (i = 0; i < MAX_MON_MOVES; ++i)
	{
		move = GetBattleMonMove(bankAtk, i);

		if (move == MOVE_NONE)
			break;

		if (!(gBitTable[i] & moveLimitations))
		{
			viability = AIScript_Negatives(bankAtk, bankDef, move, 100, &aiScriptData);
			if (viability >= 100)
			{
				if (SPLIT(move) == SPLIT_STATUS)
					return FALSE; //Has viable status move

				if (gBattleMoves[move].effect == EFFECT_KNOCK_OFF
				&&  CanKnockOffItem(bankDef))
					return FALSE; //Using Knock-Off is good even if it does minimal damage

				if (TypeCalc(move, bankAtk, bankDef, NULL, FALSE) & MOVE_RESULT_SUPER_EFFECTIVE)
					hasSuperEffectiveMove = TRUE;

				++numDamageMoves; //By default can't be a status move here
			}
		}
	}

	if (IsDoubleBattle() || !MoveEffectInMoveset(EFFECT_PROTECT, bankAtk)) //Single battle protector usually protect to stall
	{
		if (numDamageMoves == 0)
		{
			if (IS_BEHIND_SUBSTITUTE(bankAtk)
			&& !DamagingMoveThaCanBreakThroughSubstituteInMoveset(bankDef, bankAtk)
			&& IsTakingSecondaryDamage(bankDef))
				return FALSE; //Don't switch out on a foe who's taking damage even if you literally have no good moves

			if (ShouldAIFreeChoiceLockWithDynamax(bankAtk, bankDef)) //AI is locked into some move
			{
				gNewBS->ai.dynamaxMonId[SIDE(bankAtk)] = gBattlerPartyIndexes[bankAtk]; //Dynamax this mon and free the choice lock
				gNewBS->ai.shouldFreeChoiceLockWithDynamax[bankAtk][bankDef] = TRUE;
				return FALSE;
			}

			return TRUE; //Legit no moves left
		}
		else if (IS_BEHIND_SUBSTITUTE(bankAtk)
		&& !DamagingMoveThaCanBreakThroughSubstituteInMoveset(bankDef, bankAtk))
		{
			return FALSE; //Don't switch if hiding legitimately behind substitute
		}
		else if (!MoveEffectInMoveset(EFFECT_BATON_PASS, bankAtk) //U-Turn/Volt Switch switch on their own
		&& (ABILITY(bankDef) == ABILITY_MAGICGUARD || !(gBattleMons[bankDef].status2 & STATUS2_WRAPPED)) //Only switch if the enemy isn't taking trap damage
		&& (!IsTrapped(bankDef, TRUE) || !(gStatuses3[bankDef] & STATUS3_PERISH_SONG) || gDisableStructs[bankDef].perishSongTimer == 0) //Don't let a trapped foe escape from Perish Song
		/*&& numDamageMoves == 1*/) //Only 1 viable damaging move left
		{
			/*This is meant to help the Pokemon with attacking moves
			  and a bunch of setup moves to know they should switch out instead
			  of trying to attack the enemy with their weak moves.*/

			move = GetStrongestMove(bankAtk, bankDef); //Assume the usuable damage move is the strongest move
			dmg = GetFinalAIMoveDamage(move, bankAtk, bankDef, 1, NULL);

			if (dmg < gBattleMons[bankDef].hp) //Move doesn't KO
			{
				u16 leftoversRecovery = GetAmountToRecoverBy(bankDef, bankAtk, MOVE_PROTECT);
				if (leftoversRecovery != 0  //Target can heal itself with ability/item
				||  HealingMoveInMoveset(bankDef)) //Target can heal itself with move
				{
					if (dmg * 3 >= gBattleMons[bankDef].hp + ((u32) leftoversRecovery * 2)) //Damage deals a third or more of remaining health after leftovers recovery
						return FALSE; //Don't switch if can 2-3HKO
				}
				else
				{
					if (dmg * 3 >= gBattleMons[bankDef].hp) //Damage deals a third or more of remaining health
						return FALSE; //Don't switch if can 3HKO
				}

				if (ShouldAIFreeChoiceLockWithDynamax(bankAtk, bankDef)) //AI is locked into some move
				{
					gNewBS->ai.dynamaxMonId[SIDE(bankAtk)] = gBattlerPartyIndexes[bankAtk]; //Dynamax this mon and free the choice lock
					gNewBS->ai.shouldFreeChoiceLockWithDynamax[bankAtk][bankDef] = TRUE;
					return FALSE;
				}

				u8 backupActiveBattler = gActiveBattler;
				gActiveBattler = bankAtk;
				if (GetMostSuitableMonToSwitchIntoScore() & SWITCHING_INCREASE_HAS_SUPER_EFFECTIVE_MOVE)
				{
					gActiveBattler = backupActiveBattler;
					if (hasSuperEffectiveMove)
						return FALSE; //Don't switch to another Pokemon just because they have a Super-Effective move if you have one too.

					if (GetMostSuitableMonToSwitchIntoScore() == SWITCHING_INCREASE_HAS_SUPER_EFFECTIVE_MOVE
					&& WallsFoe(bankAtk, bankDef))
						return FALSE; //Tough it out
				}
				else if (GetMostSuitableMonToSwitchIntoScore() < SWITCHING_INCREASE_HAS_SUPER_EFFECTIVE_MOVE)
				{
					gActiveBattler = backupActiveBattler;
					return FALSE;
				}
				else if (GetMostSuitableMonToSwitchIntoScore() == SWITCHING_INCREASE_WALLS_FOE)
				{
					gActiveBattler = backupActiveBattler;
					//Check if current mon can't wall
					bool8 canWall = WallsFoe(bankAtk, bankDef);

					if (canWall) //Current mon can wall
						return FALSE; //Tough it out
				}
				else
					gActiveBattler = backupActiveBattler;
			}
			else
				return FALSE; //Don't switch if you can KO the opponent

			return TRUE;
		}
	}

	return FALSE; //Has some moves to use
}

bool8 OnlyBadMovesLeftInMoveset(u8 bankAtk, u8 bankDef)
{
	if (gNewBS->ai.onlyBadMovesLeft[bankAtk][bankDef] == 0xFF)
		gNewBS->ai.onlyBadMovesLeft[bankAtk][bankDef] = CalcOnlyBadMovesLeftInMoveset(bankAtk, bankDef);

	return gNewBS->ai.onlyBadMovesLeft[bankAtk][bankDef];
}

u16 TryReplaceMoveWithZMove(u8 bankAtk, u8 bankDef, u16 move)
{
	if (IsAnyMaxMove(move))
		return move;

	if (!gNewBS->zMoveData.used[bankAtk] && SPLIT(move) != SPLIT_STATUS
	&& ShouldAIUseZMove(bankAtk, bankDef, move))
	{
		u8 moveIndex = FindMovePositionInMoveset(move, bankAtk);
		if (moveIndex < MAX_MON_MOVES)
		{
			u16 zMove = CanUseZMove(bankAtk, moveIndex, move);
			if (zMove != MOVE_NONE)
			{
				gNewBS->ai.zMoveHelper = move; //Store the original move in memory for damage calcs later
				move = zMove;
			}
		}
	}
	else if (IsDynamaxed(bankAtk) || (!gNewBS->dynamaxData.used[bankAtk] && ShouldAIDynamax(bankAtk, bankDef)))
	{
		if (IsRaidBattle() && bankAtk == BANK_RAID_BOSS && IsRaidBossUsingRegularMove(bankAtk, move))
			return move; //This turn the raid boss isn't using a Max Move

		u16 maxMove = GetMaxMoveByMove(bankAtk, move);
		if (maxMove != MOVE_NONE)
		{
			gNewBS->ai.zMoveHelper = move; //Store the original move in memory for damage calcs later
			move = maxMove;
		}
	}

	return move;
}

u8 GetAIMoveEffectForMaxMove(u16 move, u8 bankAtk, u8 bankDef)
{
	u8 maxEffect = gBattleMoves[move].z_move_effect;
	u8 moveEffect = 0;

	if (move == MOVE_MAX_GUARD)
		return EFFECT_PROTECT;

	switch (maxEffect) {
		case MAX_EFFECT_RAISE_TEAM_ATTACK:
		case MAX_EFFECT_RAISE_TEAM_DEFENSE:
		case MAX_EFFECT_RAISE_TEAM_SPEED:
		case MAX_EFFECT_RAISE_TEAM_SP_ATK:
		case MAX_EFFECT_RAISE_TEAM_SP_DEF:
			if (STAT_STAGE(bankAtk, (maxEffect - MAX_EFFECT_RAISE_TEAM_ATTACK) + 1) < STAT_STAGE_MAX
			|| (IS_DOUBLE_BATTLE && BATTLER_ALIVE(PARTNER(bankAtk)) && STAT_STAGE(PARTNER(bankAtk), (maxEffect - MAX_EFFECT_RAISE_TEAM_ATTACK) + 1) < STAT_STAGE_MAX))
				moveEffect = (maxEffect - MAX_EFFECT_RAISE_TEAM_ATTACK) + EFFECT_ATTACK_UP;
			break;

		case MAX_EFFECT_LOWER_ATTACK:
		case MAX_EFFECT_LOWER_DEFENSE:
		case MAX_EFFECT_LOWER_SPEED:
		case MAX_EFFECT_LOWER_SP_ATK:
		case MAX_EFFECT_LOWER_SP_DEF:
			if (STAT_STAGE(bankDef, (maxEffect - MAX_EFFECT_LOWER_ATTACK) + 1) > STAT_STAGE_MIN)
				moveEffect = (maxEffect - MAX_EFFECT_LOWER_ATTACK) + EFFECT_ATTACK_DOWN;
			break;

		case MAX_EFFECT_SUN:
			if (!(gBattleWeather & (WEATHER_SUN_ANY | WEATHER_PRIMAL_ANY | WEATHER_CIRCUS)))
				moveEffect = EFFECT_SUNNY_DAY;
			break;
		case MAX_EFFECT_RAIN:
			if (!(gBattleWeather & (WEATHER_RAIN_ANY | WEATHER_PRIMAL_ANY | WEATHER_CIRCUS)))
				moveEffect = EFFECT_RAIN_DANCE;
			break;
		case MAX_EFFECT_SANDSTORM:
			if (!(gBattleWeather & (WEATHER_SANDSTORM_ANY | WEATHER_PRIMAL_ANY | WEATHER_CIRCUS)))
				moveEffect = EFFECT_SANDSTORM;
			break;
		case MAX_EFFECT_HAIL:
			if (!(gBattleWeather & (WEATHER_HAIL_ANY | WEATHER_PRIMAL_ANY | WEATHER_CIRCUS)))
				moveEffect = EFFECT_HAIL;
			break;

		case MAX_EFFECT_ELECTRIC_TERRAIN:
		case MAX_EFFECT_GRASSY_TERRAIN:
		case MAX_EFFECT_MISTY_TERRAIN:
		case MAX_EFFECT_PSYCHIC_TERRAIN:
			if (!(gBattleTypeFlags & BATTLE_TYPE_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_TERRAIN)
			&& gTerrainType != (maxEffect - MAX_EFFECT_ELECTRIC_TERRAIN) + 1)
				moveEffect = EFFECT_SET_TERRAIN;
			break;

		case MAX_EFFECT_VINE_LASH:
			//TODO AI
			break;

		case MAX_EFFECT_WILDFIRE:
			//TODO AI
			break;

		case MAX_EFFECT_CANNONADE:
			//TODO AI
			break;

		case MAX_EFFECT_EFFECT_SPORE_FOES:
			//TODO AI
			break;
		case MAX_EFFECT_POISON_PARALYZE_FOES:
			//TODO AI
			break;
		case MAX_EFFECT_PARALYZE_FOES:
			if (CanBeParalyzed(bankDef, TRUE))
				moveEffect = EFFECT_PARALYZE;
			break;
		case MAX_EFFECT_POISON_FOES:
			if (CanBePoisoned(bankDef, bankAtk, TRUE))
				moveEffect = EFFECT_POISON;
			break;
		case MAX_EFFECT_CONFUSE_FOES_PAY_DAY:
		case MAX_EFFECT_CONFUSE_FOES:
			if (CanBeConfused(bankDef, TRUE))
				moveEffect = EFFECT_CONFUSE;
			break;

		case MAX_EFFECT_CRIT_PLUS:
			if (!(gBattleMons[bankAtk].status2 & STATUS2_FOCUS_ENERGY))
				moveEffect = EFFECT_FOCUS_ENERGY;
			break;

		case MAX_EFFECT_MEAN_LOOK:
			if (!IsTrapped(bankDef, TRUE))
				moveEffect = EFFECT_MEAN_LOOK;
			break;

		case MAX_EFFECT_AURORA_VEIL:
			if (gNewBS->AuroraVeilTimers[SIDE(bankAtk)] == 0)
				moveEffect = EFFECT_REFLECT;
			break;

		case MAX_EFFECT_INFATUATE_FOES:
			if (CanBeInfatuated(bankDef, bankAtk))
				moveEffect = EFFECT_ATTRACT;
			break;

		case MAX_EFFECT_RECYCLE_BERRIES:
			if (SAVED_CONSUMED_ITEMS(bankAtk) != ITEM_NONE
			&& IsBerry(SAVED_CONSUMED_ITEMS(bankAtk))
			&& ITEM(bankAtk) == ITEM_NONE)
				moveEffect = EFFECT_RECYCLE;
			break;

		case MAX_EFFECT_STEALTH_ROCK:
			if (gSideTimers[SIDE(bankDef)].srAmount == 0)
				moveEffect = EFFECT_SPIKES;
			break;
		case MAX_EFFECT_STEELSURGE:
			if (gSideTimers[SIDE(bankDef)].steelsurge == 0)
				moveEffect = EFFECT_SPIKES;
			break;

		case MAX_EFFECT_DEFOG:
			if (gSideStatuses[SIDE(bankDef)] & (SIDE_STATUS_REFLECT | SIDE_STATUS_LIGHTSCREEN | SIDE_STATUS_SAFEGUARD | SIDE_STATUS_MIST)
			|| gNewBS->AuroraVeilTimers[SIDE(bankDef)] != 0
			|| gSideStatuses[SIDE(bankAtk)] & SIDE_STATUS_SPIKES
			|| !(gSideStatuses[SIDE(bankDef)] & SIDE_STATUS_SPIKES))
				moveEffect = EFFECT_RAPID_SPIN;
			break;

		case MAX_EFFECT_HEAL_TEAM:
			moveEffect = EFFECT_RESTORE_HP;
			break;

		case MAX_EFFECT_SPITE:
			moveEffect = EFFECT_SPITE;
			break;

		case MAX_EFFECT_GRAVITY:
			if (!IsGravityActive())
				moveEffect = EFFECT_FIELD_EFFECTS;
			break;

		case MAX_EFFECT_VOLCAITH_FOES:
			break;

		case MAX_EFFECT_SANDBLAST_FOES:
		case MAX_EFFECT_FIRE_SPIN_FOES:
			moveEffect = EFFECT_TRAP;
			break;

		case MAX_EFFECT_YAWN_FOE:
			if (CanBePutToSleep(bankDef, TRUE) && !(gStatuses3[bankDef] & STATUS3_YAWN))
				moveEffect = EFFECT_YAWN;
			break;

		case MAX_EFFECT_LOWER_EVASIVENESS_FOES:
			if (STAT_STAGE(bankDef, STAT_STAGE_EVASION) > STAT_STAGE_MIN)
				moveEffect = EFFECT_EVASION_DOWN;
			break;

		case MAX_EFFECT_AROMATHERAPY:
			if (PartyMemberStatused(bankAtk, FALSE))
				moveEffect = EFFECT_HEAL_BELL;
			break;

		case MAX_EFFECT_TORMENT_FOES:
			if (CanBeTormented(bankDef))
				moveEffect = EFFECT_TORMENT;
			break;

		case MAX_EFFECT_LOWER_SPEED_2_FOES:
			if (STAT_STAGE(bankDef, STAT_STAGE_SPEED) > STAT_STAGE_MIN)
				moveEffect = EFFECT_SPEED_DOWN_2;
			break;
	}

	return moveEffect;
}

bool8 GetHealthPercentage(u8 bank)
{
	return (gBattleMons[bank].hp * 100) / gBattleMons[bank].maxHP;
}

bool8 TeamFullyHealedMinusBank(u8 bank)
{
	u8 firstId, lastId;

	struct Pokemon* party = LoadPartyRange(bank, &firstId, &lastId);

	for (int i = firstId; i < lastId; ++i)
	{
		if (party[i].species == SPECIES_NONE
		|| GetMonData(&party[i], REQ_EGG, NULL)
		|| i == gBattlerPartyIndexes[bank])
			continue;

		if (party[i].hp < party[i].maxHP)
			return FALSE;
	}
	return TRUE;
}

bool8 AnyStatIsRaised(u8 bank)
{
	for (u8 statId = STAT_STAGE_ATK; statId < BATTLE_STATS_NO; ++statId)
	{
		if (STAT_STAGE(bank, statId) > 6)
			return TRUE;
	}

	return FALSE;
}

bool8 AnyUsefulStatIsRaised(u8 bank)
{
	bool8 storedPowerInMoveset = MoveInMoveset(MOVE_STOREDPOWER, bank) || MoveInMoveset(MOVE_STOREDPOWER, bank);

	for (u8 statId = STAT_STAGE_ATK; statId < BATTLE_STATS_NO; ++statId)
	{
		if (STAT_STAGE(bank, statId) > 6)
		{
			if (storedPowerInMoveset)
				return TRUE;

			switch (statId) {
				case STAT_STAGE_ATK:
					if (PhysicalMoveInMoveset(bank))
						return TRUE;
					break;
				case STAT_STAGE_DEF:
					if (MoveSplitOnTeam(FOE(bank), SPLIT_PHYSICAL))
						return TRUE;
					break;
				case STAT_STAGE_SPATK:
					if (SpecialMoveInMoveset(bank))
						return TRUE;
					break;
				case STAT_STAGE_SPDEF:
					if (MoveSplitOnTeam(FOE(bank), SPLIT_SPECIAL))
						return TRUE;
					break;
				case STAT_STAGE_SPEED:
				case STAT_STAGE_ACC:
				case STAT_STAGE_EVASION:
					return TRUE;
			}
		}
	}

	if (gBattleResources->flags->flags[bank] & 1
	&&  DamagingMoveTypeInMoveset(bank, TYPE_FIRE))
		return TRUE;

	return FALSE;
}

bool8 AnyUsefulOffseniveStatIsRaised(u8 bank)
{
	for (u8 statId = STAT_STAGE_ATK; statId < BATTLE_STATS_NO; ++statId)
	{
		if (STAT_STAGE(bank, statId) > 6)
		{
			switch (statId) {
				case STAT_STAGE_ATK:
					if (PhysicalMoveInMoveset(bank))
						return TRUE;
					break;
				case STAT_STAGE_SPATK:
					if (SpecialMoveInMoveset(bank))
						return TRUE;
					break;
				case STAT_STAGE_SPEED:
					return TRUE;
			}
		}
	}

	return FALSE;
}

bool8 PartyMemberStatused(u8 bank, bool8 checkSoundproof)
{
	struct Pokemon* party;
	if (SIDE(bank) == B_SIDE_PLAYER)
		party = gPlayerParty;
	else
		party = gEnemyParty;

	for (int i = 0; i < PARTY_SIZE; ++i)
	{
		if (checkSoundproof && GetMonAbility(&party[i]) == ABILITY_SOUNDPROOF)
			continue;

		if (party[i].condition != STATUS1_NONE)
			return TRUE;
	}

	return FALSE;
}

bool8 ShouldAIUseZMove(u8 bankAtk, u8 bankDef, u16 move)
{
	int i;

	if (IS_DOUBLE_BATTLE && bankDef == PARTNER(bankAtk))
		return FALSE; //No need to waste a Z-Move on your partner

	u16 zMove = CanUseZMove(bankAtk, 0xFF, move);
	u16 defMovePrediction = IsValidMovePrediction(bankDef, bankAtk);

	if (zMove != MOVE_NONE)
	{
		if (zMove != 0xFFFF) //Damaging Z-Move
		{
			u8 defAbility = ABILITY(bankDef);
			u16 defSpecies = SPECIES(bankDef);
			bool8 noMoldBreakers = NO_MOLD_BREAKERS(ABILITY(bankAtk), zMove);

			if (move == MOVE_FAKEOUT && ShouldUseFakeOut(bankAtk, bankDef))
				return FALSE; //Prefer actual Fake Out over Breakneck Blitz

			if (MoveBlockedBySubstitute(zMove, bankAtk, bankDef)
			|| (defMovePrediction == MOVE_SUBSTITUTE
			 && !MoveWouldHitFirst(zMove, bankAtk, bankDef)
			 && !MoveIgnoresSubstitutes(zMove, ABILITY(bankAtk))))
				return FALSE; //Don't use a Z-Move on a Substitute or if the enemy is going to go first and use Substitute

			#ifdef SPECIES_MIMIKYU
			if (noMoldBreakers && defAbility == ABILITY_DISGUISE && defSpecies == SPECIES_MIMIKYU)
				return FALSE; //Don't waste a Z-Move busting Mimikyu's disguise
			#endif
			#ifdef SPECIES_EISCUE
			if (noMoldBreakers && defAbility == ABILITY_ICEFACE && defSpecies == SPECIES_EISCUE && SPLIT(zMove) == SPLIT_PHYSICAL)
				return FALSE; //Don't waste a Z-Move busting Eiscue's Ice Face
			#endif

			if (defMovePrediction == MOVE_PROTECT || defMovePrediction == MOVE_KINGSSHIELD || defMovePrediction == MOVE_SPIKYSHIELD || defMovePrediction == MOVE_OBSTRUCT
			|| (IsDynamaxed(bankDef) && SPLIT(defMovePrediction) == SPLIT_STATUS))
				return FALSE; //Don't waste a Z-Move on a Protect

			if (IsRaidBattle() && gNewBS->dynamaxData.raidShieldsUp && SIDE(bankAtk) == B_SIDE_PLAYER && SIDE(bankDef) == B_SIDE_OPPONENT) //Partner AI on Raid Pokemon with shields up
			{
				if (gNewBS->dynamaxData.shieldCount - gNewBS->dynamaxData.shieldsDestroyed <= 2 //Less than 3 shields left
				&& gNewBS->dynamaxData.stormLevel < 3) //The Raid boss hasn't almost won
					return FALSE; //Don't waste a Z-Move breaking a shield

				u16 bankAtkPartner = PARTNER(bankAtk);
				u16 partnerMove = GetAIChosenMove(bankAtkPartner, bankDef);

				if (SPLIT(partnerMove) == SPLIT_STATUS
				|| MoveWouldHitFirst(partnerMove, bankAtkPartner, bankAtk)
				|| (gChosenMovesByBanks[bankAtkPartner] != MOVE_NONE && gBattleStruct->moveTarget[bankAtkPartner] != bankDef)) //Not targeting raid Pokemon
					return FALSE; //Don't waste a Z-Move if partner can't destroy shield first
			}

			//These moves should always be turned into Z-Moves, regardless if they KO or not
			switch (gBattleMoves[move].effect) {
				case EFFECT_RECHARGE:
				case EFFECT_SEMI_INVULNERABLE:
				case EFFECT_SKY_ATTACK:
				case EFFECT_SKULL_BASH:
				case EFFECT_SOLARBEAM:
				case EFFECT_LASTRESORT_SKYDROP:
					return TRUE;
			}

			if (MoveKnocksOutXHits(move, bankAtk, bankDef, 1))
				return FALSE; //If the base move can KO, don't turn it into a Z-Move

			return TRUE;
		}
		else //Status Move
		{
			u8 zEffect = gBattleMoves[move].z_move_effect;

			switch (zEffect) {
				case Z_EFFECT_NONE:
					return FALSE;
				case Z_EFFECT_RESET_STATS:
					for (i = STAT_STAGE_ATK; i < BATTLE_STATS_NO; ++i)
					{
						if (i == STAT_STAGE_ATK && !PhysicalMoveInMoveset(bankAtk)) //Only reset lowered Attack if useful
							continue;
						else if (i == STAT_STAGE_ATK && !SpecialMoveInMoveset(bankAtk)) //Only reset lowered Special Attack if useful
							continue;

						if (STAT_STAGE(bankAtk, i) < 6)
							return TRUE; //Reset any negative stats
					}
					break;
				case Z_EFFECT_ALL_STATS_UP_1:
					if (!StatsMaxed(bankAtk))
						return TRUE;
					break;
				case Z_EFFECT_BOOST_CRITS:
					if (!(gBattleMons[bankAtk].status2 & STATUS2_FOCUS_ENERGY))
						return TRUE;
					break;
				case Z_EFFECT_FOLLOW_ME:
					if (IS_DOUBLE_BATTLE)
						return TRUE;
					break;
				case Z_EFFECT_ATK_UP_1:
				case Z_EFFECT_ATK_UP_2:
				case Z_EFFECT_ATK_UP_3:
					if (STAT_CAN_RISE(bankAtk, STAT_STAGE_ATK)
					&& MoveSplitInMoveset(bankAtk, SPLIT_PHYSICAL))
						return TRUE;
					break;
				case Z_EFFECT_DEF_UP_1:
				case Z_EFFECT_DEF_UP_2:
				case Z_EFFECT_DEF_UP_3:
					if (STAT_CAN_RISE(bankAtk, STAT_STAGE_DEF))
						return TRUE;
					break;
				case Z_EFFECT_SPATK_UP_1:
				case Z_EFFECT_SPATK_UP_2:
				case Z_EFFECT_SPATK_UP_3:
					if (STAT_CAN_RISE(bankAtk, STAT_STAGE_SPATK)
					&& MoveSplitInMoveset(bankAtk, SPLIT_SPECIAL))
						return TRUE;
					break;
				case Z_EFFECT_SPDEF_UP_1:
				case Z_EFFECT_SPDEF_UP_2:
				case Z_EFFECT_SPDEF_UP_3:
					if (STAT_CAN_RISE(bankAtk, STAT_STAGE_SPDEF))
						return TRUE;
					break;
				case Z_EFFECT_SPD_UP_1:
				case Z_EFFECT_SPD_UP_2:
				case Z_EFFECT_SPD_UP_3:
					if (STAT_CAN_RISE(bankAtk, STAT_STAGE_SPEED))
						return TRUE;
					break;
				case Z_EFFECT_ACC_UP_1:
				case Z_EFFECT_ACC_UP_2:
				case Z_EFFECT_ACC_UP_3:
					if (STAT_CAN_RISE(bankAtk, STAT_STAGE_ACC)
					&&  MoveInMovesetWithAccuracyLessThan(bankAtk, bankDef, 90, FALSE))
						return TRUE;
					break;
				case Z_EFFECT_EVSN_UP_1:
				case Z_EFFECT_EVSN_UP_2:
				case Z_EFFECT_EVSN_UP_3:
					if (STAT_CAN_RISE(bankAtk, STAT_STAGE_EVASION))
						return TRUE;
					break;
				default: //Recover HP
					return TRUE;
			}
		}
	}

	return FALSE;
}

/*
	1. Highest Attacking Stat
	2. Weather Boosting Ability
	3. Choice Item
	4. Weakness Policy
	
	Gigantamax gives +1 to all of the above
*/
static bool8 MonCanTriggerWeatherAbilityWithMaxMove(struct Pokemon* mon)
{
	if (WEATHER_HAS_EFFECT)
	{
		switch (GetMonAbility(mon)) {
			case ABILITY_SWIFTSWIM:
			case ABILITY_RAINDISH:
			case ABILITY_DRYSKIN:
				return MonCanUseMaxMoveWithEffect(mon, MAX_EFFECT_RAIN);
			case ABILITY_CHLOROPHYLL:
			case ABILITY_SOLARPOWER:
			case ABILITY_FLOWERGIFT:
				return MonCanUseMaxMoveWithEffect(mon, MAX_EFFECT_SUN);
			case ABILITY_SANDRUSH:
			case ABILITY_SANDFORCE:
				return MonCanUseMaxMoveWithEffect(mon, MAX_EFFECT_SANDSTORM);
			case ABILITY_SLUSHRUSH:
			case ABILITY_ICEBODY:
			case ABILITY_ICEFACE:
				return MonCanUseMaxMoveWithEffect(mon, MAX_EFFECT_HAIL);
			case ABILITY_SURGESURFER:
				return MonCanUseMaxMoveWithEffect(mon, MAX_EFFECT_ELECTRIC_TERRAIN);
		}
	}

	return FALSE;
}

void CalcAIDynamaxMon(u8 bank)
{
	u8 bestMonId = 0xFF;

	if (gBattleTypeFlags & BATTLE_TYPE_DYNAMAX && !gNewBS->dynamaxData.used[bank])
	{
		struct Pokemon* party;
		u8 bestMonScore, bestStatAmount, firstId, lastId, battlerIn1, battlerIn2, i;

		party = LoadPartyRange(bank, &firstId, &lastId);
		if (IS_SINGLE_BATTLE)
			battlerIn1 = battlerIn2 = bank;
		else
		{
			if (gAbsentBattlerFlags & gBitTable[bank])
				battlerIn1 = battlerIn2 = PARTNER(bank);
			else if (gAbsentBattlerFlags & gBitTable[PARTNER(bank)])
				battlerIn1 = battlerIn2 = bank;
			else
			{
				battlerIn1 = bank;
				battlerIn2 = PARTNER(bank);
			}
		}

		for (i = 0, bestMonScore = 0, bestStatAmount = 0; i < PARTY_SIZE; ++i) //Do entire party at once, even for Multi Battles
		{
			struct Pokemon* mon = &party[i];
			u8 updateScore, itemEffect, ability;
			u32 bestMonStat, attack, spAttack;

			if (mon->species == SPECIES_NONE
			|| GetMonData(mon, MON_DATA_HP, NULL) == 0
			|| GetMonData(mon, MON_DATA_IS_EGG, NULL)
			|| !MonCanDynamax(mon))
				continue;

			updateScore = 0;
			bestMonStat = 0;
			if (gBattlerPartyIndexes[battlerIn1] == i || gBattlerPartyIndexes[battlerIn2] == i)
			{
				u8 checkBank;
				if (gBattlerPartyIndexes[battlerIn1] == i)
					checkBank = battlerIn1;
				else
					checkBank = battlerIn2;

				itemEffect = ITEM_EFFECT(checkBank);
				ability = ABILITY(checkBank);
				attack = gBattleMons[checkBank].attack;
				spAttack = gBattleMons[checkBank].spAttack;
				APPLY_QUICK_STAT_MOD(attack, STAT_STAGE(checkBank, STAT_STAGE_ATK));
				APPLY_QUICK_STAT_MOD(spAttack, STAT_STAGE(checkBank, STAT_STAGE_SPATK));
			}
			else
			{
				itemEffect = GetMonItemEffect(mon);
				ability = GetMonAbility(mon);
				attack = mon->attack;
				spAttack = mon->spAttack;
			}

			if (PhysicalMoveInMonMoveset(mon, MOVE_LIMITATION_ZEROMOVE | MOVE_LIMITATION_PP))
			{
				if (SpecialMoveInMonMoveset(mon, MOVE_LIMITATION_ZEROMOVE | MOVE_LIMITATION_PP) && spAttack > attack)
					bestMonStat = spAttack;
				else
					bestMonStat = attack;
			}
			else if (SpecialMoveInMonMoveset(mon, MOVE_LIMITATION_ZEROMOVE | MOVE_LIMITATION_PP)) //Only set if mon has Special move
				bestMonStat = spAttack;

			if (itemEffect == ITEM_EFFECT_WEAKNESS_POLICY)
				updateScore = 4;
			else if (itemEffect == ITEM_EFFECT_CHOICE_BAND || ability == ABILITY_GORILLATACTICS)
				updateScore = 3;
			else if (MonCanTriggerWeatherAbilityWithMaxMove(mon))
				updateScore = 2;
			else if (bestMonStat > 0 //Has an actual attacking move
			&& itemEffect != ITEM_EFFECT_EJECT_BUTTON && itemEffect != ITEM_EFFECT_EJECT_PACK) //And probably won't be forced out by its item
				updateScore = 1;

			if (MoveInMonMovesetThatCanChangeByGigantamaxing(mon))
				++updateScore; //This is even better

			if (updateScore >= bestMonScore)
			{
				if (updateScore == bestMonScore
				&& bestMonStat <= bestStatAmount)
					continue; //Stats aren't better so check next mon

				if (bestMonStat > 0) //Mon has attacking moves
				{
					bestMonId = i;
					bestMonScore = updateScore;
					bestStatAmount = bestMonStat;
				}
			}
		}
	}

	gNewBS->ai.dynamaxMonId[SIDE(bank)] = bestMonId;
}

void CalcShouldAIDynamax(u8 bankAtk, u8 bankDef)
{
	gNewBS->ai.dynamaxPotential[bankAtk][bankDef] = FALSE;

	if (bankAtk != bankDef && CanDynamax(bankAtk))
	{
		u8 partnerBank = PARTNER(bankAtk);

		if (BATTLER_SEMI_INVULNERABLE(bankAtk))
			return; //Can't Dynamax out of view

		if (IS_DOUBLE_BATTLE && BATTLER_ALIVE(partnerBank) && gNewBS->dynamaxData.toBeUsed[partnerBank])
			return; //Don't Dynamax if the other mon is going to

		if (IS_DOUBLE_BATTLE && bankDef == PARTNER(bankAtk))
			return; //No need to Dynamax against your partner

		if (IsRaidBattle() && bankAtk == BANK_RAID_BOSS)
			return; //Raid Boss is always Dynamaxed

		if (!IsMockBattle() && SIDE(bankAtk) == B_SIDE_PLAYER)
		{
			if (IsTagBattle())
			{
				if (GetBattlerPosition(bankAtk) == B_POSITION_PLAYER_LEFT)
					return; //Never predict that the player will Dynamax
			}
			else //Player is in control
				return; //Never predict that the player will Dynamax
		}

		if (gNewBS->ai.dynamaxMonId[SIDE(bankAtk)] == gBattlerPartyIndexes[bankAtk])
		{
			if (AI_THINKING_STRUCT->aiFlags > AI_SCRIPT_CHECK_BAD_MOVE || SIDE(bankAtk) == B_SIDE_PLAYER) //Smart AI or partners only
			{
				u16 predictedMove = IsValidMovePrediction(bankAtk, bankDef);
				if (predictedMove != MOVE_NONE
				&&  MoveWouldHitFirst(predictedMove, bankAtk, bankDef)
				&&  MoveKnocksOutXHits(predictedMove, bankAtk, bankDef, 1))
					return; //Just KO the opponent normally

				if ((IS_SINGLE_BATTLE && ViableMonCountFromBank(bankAtk) > 1)
				||  (IS_DOUBLE_BATTLE && ViableMonCountFromBank(bankAtk) > 2)) //Could use Dynamax on another Pokemon
				{
					predictedMove = IsValidMovePrediction(bankDef, bankAtk);
					if (predictedMove != MOVE_NONE
					&&  MoveWouldHitFirst(predictedMove, bankDef, bankAtk)
					&&  MoveKnocksOutAfterDynamax(predictedMove, bankDef, bankAtk))
						return; //Don't Dynamax is foe will just KO you
				}

				OnlyBadMovesLeftInMoveset(bankAtk, bankDef); //Force calculation

				if ((ITEM_EFFECT(bankAtk) == ITEM_EFFECT_CHOICE_BAND || ABILITY(bankAtk) == ABILITY_GORILLATACTICS)
				&& !gNewBS->ai.shouldFreeChoiceLockWithDynamax[bankAtk][bankDef]) //There are good moves left
					return; //Save Dynamax for when you really need it
			}

			gNewBS->ai.dynamaxPotential[bankAtk][bankDef] = TRUE;
			ClearStrongestMoveAndCanKnockOut(bankAtk, bankDef); //All moves now are treated like Max Moves so wipe old data
		}
	}
}

bool8 ShouldAIDynamax(u8 bankAtk, u8 bankDef)
{
	return gNewBS->ai.dynamaxPotential[bankAtk][bankDef];
}

u8 AdjustMoveLimitationFlagsForAI(u8 bankAtk, u8 bankDef)
{
	if (ShouldAIDynamax(bankAtk, bankDef)) //AI will Dynamax this turn
		return MOVE_LIMITATION_ZEROMOVE | MOVE_LIMITATION_PP | MOVE_LIMITATION_TAUNT;

	return 0xFF; //All flags by default
}

void IncreaseViability(s16* viability, u8 amount)
{
	*viability = MathMin(*viability + amount, 255);
}

void DecreaseViability(s16* viability, u16 amount)
{
	if (IS_DOUBLE_BATTLE) //Double decreases in doubles
		amount *= 2; //Assumes amount won't be too big to cause overflow

	if (amount > *viability)
		*viability = 0;
	else
		*viability -= amount;
}
