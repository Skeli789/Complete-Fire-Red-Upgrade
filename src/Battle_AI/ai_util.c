#include "../defines.h"
#include "../defines_battle.h"
#include "../../include/random.h"
#include "../../include/constants/items.h"

#include "../../include/new/ability_tables.h"
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
#include "../../include/new/frontier.h"
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
static bool8 CalculateMoveKnocksOutXHitsFresh(u16 move, u8 bankAtk, u8 bankDef, u8 numHits);
static bool8 CalcShouldAIUseZMove(u8 bankAtk, u8 bankDef, u16 move);

u16 AIRandom()
{
    gNewBS->ai.randSeed = 1103515245 * gNewBS->ai.randSeed + 24691; //Seeded every frame no matter what
    return gNewBS->ai.randSeed >> 16;
}

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
	if (!BATTLER_ALIVE(bankAtk) || !BATTLER_ALIVE(bankDef) || IsBankIncapacitated(bankAtk))
		return FALSE; //Can't KO if you're dead or target is dead

	if (gNewBS->ai.canKnockOut[bankAtk][bankDef] == 0xFF) //Hasn't been calculated yet
	{
		if (gNewBS->ai.strongestMove[bankAtk][bankDef] == 0xFFFF)
			gNewBS->ai.strongestMove[bankAtk][bankDef] = CalcStrongestMove(bankAtk, bankDef, FALSE);

		gNewBS->ai.canKnockOut[bankAtk][bankDef] = MoveKnocksOutXHits(gNewBS->ai.strongestMove[bankAtk][bankDef], bankAtk, bankDef, 1);

		if (gNewBS->ai.canKnockOut[bankAtk][bankDef])
			gNewBS->ai.can2HKO[bankAtk][bankDef] = TRUE; //If you can KO in 1 hit you can KO in 2
	}

	return gNewBS->ai.canKnockOut[bankAtk][bankDef];
}

bool8 CanKnockOutWithFasterMove(u8 bankAtk, u8 bankDef, u16 defMove)
{
	int i;
	u16 move;
	bool8 isAsleep = IsBankAsleep(bankAtk);

	if (!BATTLER_ALIVE(bankAtk) || !BATTLER_ALIVE(bankDef) || IsBankIncapacitated(bankAtk))
		return FALSE; //Can't KO if you're dead or target is dead

	u8 moveLimitations = CheckMoveLimitations(bankAtk, 0, AdjustMoveLimitationFlagsForAI(bankAtk, bankDef));

	for (i = 0; i < MAX_MON_MOVES; ++i)
	{
		move = GetBattleMonMove(bankAtk, i);

		if (move == MOVE_NONE)
			break; //End of usable moves

		if (isAsleep && move != MOVE_SNORE)
			continue;

		if (!(gBitTable[i] & moveLimitations))
		{
			if (MoveKnocksOutXHits(move, bankAtk, bankDef, 1)
			&& MoveWouldHitBeforeOtherMove(move, bankAtk, defMove, bankDef))
				return TRUE;
		}
	}

	return FALSE;
}

bool8 Can2HKO(u8 bankAtk, u8 bankDef)
{
	if (gNewBS->ai.can2HKO[bankAtk][bankDef] == 0xFF) //Hasn't been calculated yet
	{
		if (gNewBS->ai.strongestMove[bankAtk][bankDef] == 0xFFFF)
			gNewBS->ai.strongestMove[bankAtk][bankDef] = CalcStrongestMove(bankAtk, bankDef, FALSE);

		gNewBS->ai.can2HKO[bankAtk][bankDef] = MoveKnocksOutXHits(gNewBS->ai.strongestMove[bankAtk][bankDef], bankAtk, bankDef, 2);
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

bool8 CanKnockOutAfterHealing(u8 bankAtk, u8 bankDef, u16 healAmount, u8 numHits, bool8 dealOneHitFirst)
{
	int i;
	u16 move;
	bool8 isAsleep = IsBankAsleep(bankAtk);

	if (IsBankIncapacitated(bankAtk)
	|| gAbsentBattlerFlags & (gBitTable[bankAtk] | gBitTable[bankDef]))
		return FALSE;

	u8 moveLimitations = CheckMoveLimitations(bankAtk, 0, AdjustMoveLimitationFlagsForAI(bankAtk, bankDef));

	//Temporarily increase target's hp for calcs
	u16 backupHp = gBattleMons[bankDef].hp;
	
	if (dealOneHitFirst)
	{
		for (i = 0; i < MAX_MON_MOVES; ++i)
		{
			move = GetBattleMonMove(bankAtk, i);

			if (move == MOVE_NONE)
				break;

			if (isAsleep && move != MOVE_SNORE)
				continue;

			if (!(gBitTable[i] & moveLimitations))
			{
				u32 dmg = CalcFinalAIMoveDamage(move, bankAtk, bankDef, 1, NULL);
				if (dmg >= backupHp)
				{
					gBattleMons[bankDef].hp = backupHp;
					return TRUE; //Can't even survive a single attack, let alone 2 or more
				}

				gBattleMons[bankDef].hp = (backupHp - dmg) + healAmount; //Deal the damage and then heal it off
				gBattleMons[bankDef].hp += GetAmountToRecoverBy(bankDef, 0, MOVE_PROTECT); //Then get more HP at the end of the turn
				gBattleMons[bankDef].hp = MathMin(gBattleMons[bankDef].hp, gBattleMons[bankDef].maxHP);

				if (CalculateMoveKnocksOutXHitsFresh(move, bankAtk, bankDef, numHits)) //Then see if it can KO with the rest of the hits
				{
					gBattleMons[bankDef].hp = backupHp;
					return TRUE;
				}
			}
		}
	}
	else
	{
		gBattleMons[bankDef].hp = MathMin(backupHp + healAmount, gBattleMons[bankDef].maxHP);

		for (i = 0; i < MAX_MON_MOVES; ++i)
		{
			move = GetBattleMonMove(bankAtk, i);

			if (move == MOVE_NONE)
				break;

			if (isAsleep && move != MOVE_SNORE)
				continue;

			if (!(gBitTable[i] & moveLimitations))
			{
				if (CalculateMoveKnocksOutXHitsFresh(move, bankAtk, bankDef, numHits)) //Need fresh calculation since data is locked earlier
				{
					gBattleMons[bankDef].hp = backupHp;
					return TRUE;
				}
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

bool8 CanKnockOutWithChipDamage(u8 bankAtk, u8 bankDef)
{
	if (CanKnockOut(bankAtk, bankDef))
		return TRUE;

	u16 strongestMove = GetStrongestMove(bankAtk, bankDef);

	return GetFinalAIMoveDamage(strongestMove, bankAtk, bankDef, 1, NULL)
			+ GetSecondaryEffectDamage(bankDef) >= gBattleMons[bankDef].hp;
}

bool8 Can2HKOWithChipDamage(u8 bankAtk, u8 bankDef)
{
	if (CanKnockOut(bankAtk, bankDef))
		return TRUE;

	u16 strongestMove = GetStrongestMove(bankAtk, bankDef);

	return GetFinalAIMoveDamage(strongestMove, bankAtk, bankDef, 2, NULL)
			+ GetSecondaryEffectDamage(bankDef) >= gBattleMons[bankDef].hp;
}

bool8 MoveKnocksOutPossiblyGoesFirstWithBestAccuracy(u16 checkMove, u8 bankAtk, u8 bankDef, bool8 checkGoingFirst)
{
	u32 i;
	u16 bestAcc = 0;
	s8 bestPriority = 0;
	bool8 perfectMoves = 0;
	bool8 goodMoves = 0;
	u8 moveLimitations = CheckMoveLimitations(bankAtk, 0, AdjustMoveLimitationFlagsForAI(bankAtk, bankDef));
	bool8 badIdeaToMakeContact = BadIdeaToMakeContactWith(bankAtk, bankDef);
	bool8 goodMoveThatDoesntMakeContact = FALSE;
	bool8 hasNonMultiHitMove = FALSE;
	bool8 perfectMoveThatDoesntMakeContact = FALSE;

	for (i = 0; i < MAX_MON_MOVES; ++i)
	{
		u16 currMove = GetBattleMonMove(bankAtk, i);
		if (currMove == MOVE_NONE)
			break;

		currMove = TryReplaceMoveWithZMove(bankAtk, bankDef, currMove);

		if (!(gBitTable[i] & moveLimitations)) //This move can be used
		{
			if (MoveKnocksOutXHits(currMove, bankAtk, bankDef, 1) //This move knocks out
			&& (!checkGoingFirst || MoveWouldHitFirst(currMove, bankAtk, bankDef))) //And this move goes first or it doesn't matter if it goes first
			{
				if (MoveWillHit(currMove, bankAtk, bankDef)) //This is a sure-hit move like with No Guard
				{
					perfectMoves |= gBitTable[i]; //This is one of the best moves
					perfectMoveThatDoesntMakeContact |= !CheckContact(currMove, bankAtk); //Make sure at least one perfect move doesn't make contact
				}
				else if (perfectMoves == 0) //Only waste time with the other moves if there isn't already a perfect move
				{
					u16 currAcc = CalcAIAccuracy(currMove, bankAtk, bankDef);
					s8 currPriority = PriorityCalc(bankAtk, ACTION_USE_MOVE, currMove);

					if (goodMoves == 0 //No good moves yet
					|| (currAcc > bestAcc && bestAcc < 100)) //This move has perfect accuracy and the best accuracy so far was worse
					{
						bestAcc = currAcc;
						bestPriority = currPriority;
						goodMoves = gBitTable[i]; //Wipe all previously good moves
						goodMoveThatDoesntMakeContact = !CheckContact(currMove, bankAtk); //Update whether the "best" move makes contact
						hasNonMultiHitMove = gBattleMoves[currMove].effect != EFFECT_MULTI_HIT;
					}
					else if (currAcc == bestAcc || currAcc >= 100) //This move has the same as the best accuracy or this move has perfect accuracy
					{
						//The better move is still the one with more priority
						if (PriorityCalc(bankAtk, ACTION_USE_MOVE, currMove) > bestPriority) //This move has the highest priority of all moves so far
						{
							bestAcc = currAcc;
							bestPriority = currPriority;
							goodMoves = gBitTable[i]; //Wipe all previously good moves
							goodMoveThatDoesntMakeContact = !CheckContact(currMove, bankAtk); //Update whether the "best" move makes contact
							hasNonMultiHitMove = gBattleMoves[currMove].effect != EFFECT_MULTI_HIT;
						}
						else
						{
							//This move has the same priority as pre-existing moves
							goodMoves |= gBitTable[i];
							goodMoveThatDoesntMakeContact |= !CheckContact(currMove, bankAtk); //Update whether at least one good move doesn't make contact
							hasNonMultiHitMove |= gBattleMoves[currMove].effect != EFFECT_MULTI_HIT;
						}
					}
				}
			}
		}
	}

	bool8 moveIsContact = CheckContact(checkMove, bankAtk);
	bool8 movesToCheck = (perfectMoves == 0) ? goodMoves : perfectMoves;
	bool8 hasMoveThatDoesntMakeContact = (perfectMoves == 0) ? goodMoveThatDoesntMakeContact : perfectMoveThatDoesntMakeContact;

	for (i = 0; i < MAX_MON_MOVES; ++i)
	{
		if (movesToCheck & gBitTable[i] //Considered a best move
		&& gBattleMons[bankAtk].moves[i] == checkMove) //The original move in question is this best move
		{
			if (badIdeaToMakeContact //It's better not to use a contact move
			&& moveIsContact) //And the move being checked is a contact move
			{
				if (hasMoveThatDoesntMakeContact) //There is a good move that doesn't make contact
					continue; //There's a better move that could be used

				if (gBattleMoves[checkMove].effect == EFFECT_MULTI_HIT && hasNonMultiHitMove) //A different contect move only hits once
					continue; //Prefer that move
			}

			return TRUE;
		}
	}

	return FALSE;
}

bool8 HasMoveThatGoesFirstAndKnocksOut(u8 bankAtk, u8 bankDef)
{
	u16 currMove;
	u8 moveLimitations = CheckMoveLimitations(bankAtk, 0, AdjustMoveLimitationFlagsForAI(bankAtk, bankDef));

	for (u32 i = 0; i < MAX_MON_MOVES; ++i)
	{
		currMove = GetBattleMonMove(bankAtk, i);
		if (currMove == MOVE_NONE)
			break;

		//currMove = TryReplaceMoveWithZMove(bankAtk, bankDef, currMove); //Z-Moves don't usually go first

		if (!(gBitTable[i] & moveLimitations))
		{
			if (MoveKnocksOutXHits(currMove, bankAtk, bankDef, 1)
			&& MoveWouldHitFirst(currMove, bankAtk, bankDef))
				return TRUE;
		}
	}

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

	for (u32 i = 0; i < MAX_MON_MOVES; ++i)
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
		&& moveEffect != EFFECT_SOLARBEAM
		&& moveEffect != EFFECT_SEMI_INVULNERABLE
		&& moveEffect != EFFECT_SKY_ATTACK
		&& moveEffect != EFFECT_RAZOR_WIND
		&& moveEffect != EFFECT_SKULL_BASH
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

static u16 CalcStrongestMoveGoesFirst(u8 bankAtk, u8 bankDef)
{
	u32 i;
	u16 currMove, bestMove;
	u32 currDmg;
	u32 bestDmg = 0;
	u8 moveLimitations = CheckMoveLimitations(bankAtk, 0, AdjustMoveLimitationFlagsForAI(bankAtk, bankDef));
	bool8 badIdeaToMakeContact = BadIdeaToMakeContactWith(bankAtk, bankDef);
	bool8 takesRecoilDamage = ABILITY(bankAtk) != ABILITY_MAGICGUARD;

	struct DamageCalc damageData = {0};
	damageData.bankAtk = bankAtk;
	damageData.bankDef = bankDef;
	PopulateDamageCalcStructWithBaseAttackerData(&damageData);
	PopulateDamageCalcStructWithBaseDefenderData(&damageData);

	for (i = 0, bestMove = MOVE_NONE; i < MAX_MON_MOVES; ++i)
	{
		currMove = GetBattleMonMove(bankAtk, i);

		if (currMove == MOVE_NONE)
			break;

		currMove = TryReplaceMoveWithZMove(bankAtk, bankDef, currMove);

		if (AttacksThisTurn(bankAtk, currMove) == 1) //Has to charge first
			continue;

		if (!(gBitTable[i] & moveLimitations))
		{
			currDmg = GetFinalAIMoveDamage(currMove, bankAtk, bankDef, 1, &damageData);

			if (MoveWouldHitFirst(currMove, bankAtk, bankDef))
			{
				if (currDmg > bestDmg)
				{
					bestDmg = currDmg;
					bestMove = currMove;
				}
				else if (currDmg == bestDmg) //Two moves do the same (probably both KO)
				{
					//Try choosing the one with higher priority
					u8 currPriority = PriorityCalc(bankAtk, ACTION_USE_MOVE, currMove);
					u8 bestPriority = PriorityCalc(bankAtk, ACTION_USE_MOVE, bestMove); //Priority of the best move so far

					if (currPriority > bestPriority) //This new move has a higher priority
						bestMove = currMove; //So pick it since it does the most damage and has a higher chance of going first
					else if (currPriority == bestPriority) //If the two moves have the same priority
					{
						if (!CanBeChoiceLocked(bankAtk)) //Only care about contact and recoil if not going to be locked into a single move
						{
							//Pick a non-contact move if contact is a bad idea
							if (badIdeaToMakeContact)
							{
								bool8 bestMoveContact = CheckContact(bestMove, bankAtk);
								bool8 currMoveContact = CheckContact(currMove, bankAtk);
							
								if (bestMoveContact && !currMoveContact)
								{
									//The new move isn't contact unlike the old best move
									bestMove = currMove;
									continue; //Proceed to next move
								}
								else if (!bestMoveContact && currMoveContact)
									continue; //Check the next move - this move is out
								else if (bestMoveContact && currMoveContact)
								{
									//They're both contact moves, so pick the one less likely to proc the contact
									bool8 bestMoveMultiHit = gBattleMoves[bestMove].effect == EFFECT_MULTI_HIT;
									bool8 currMoveMultiHit = gBattleMoves[currMove].effect == EFFECT_MULTI_HIT;

									if (bestMoveMultiHit && !currMoveMultiHit)
									{
										//The new move isn't a multi-hit unlike the old best move
										bestMove = currMove;
										continue; //Proceed to next move
									}
									else if (!bestMoveMultiHit && currMoveMultiHit)
										continue; //Check the next move - this move is out
								}
							}

							//Pick a non-recoil move preferably
							if (takesRecoilDamage)
							{
								bool8 bestMoveRecoil = CheckRecoil(bestMove);
								bool8 currMoveRecoil = CheckRecoil(currMove);

								if (bestMoveRecoil && !currMoveRecoil)
								{
									//Prefer the move that doesn't do recoil damage
									bestMove = currMove;
									continue; //Proceed to next move
								}
								else if (!bestMoveRecoil && currMoveRecoil)
									continue; //Check the next move - this move is out
							}

							//Pick a non-stat recoil move preferably
							{
								bool8 bestMoveStatRecoil = IsStatRecoilMove(bestMove);
								bool8 currMoveStatRecoil = IsStatRecoilMove(currMove);

								if (bestMoveStatRecoil && !currMoveStatRecoil)
								{
									//Prefer the move that doesn't lower stats
									bestMove = currMove;
									continue; //Proceed to next move
								}
								else if (!bestMoveStatRecoil && currMoveStatRecoil)
									continue; //Check the next move - this move is out
							}
						}

						//Pick a move at random.
						if (Random() & 1)
							bestMove = currMove;
					}
				}
			}
		}
	}

	return bestMove;
}

bool8 StrongestMoveGoesFirst(u16 currentMove, u8 bankAtk, u8 bankDef)
{
	if (gNewBS->ai.strongestMoveGoesFirst[bankAtk][bankDef] == 0xFFFF)
		gNewBS->ai.strongestMoveGoesFirst[bankAtk][bankDef] = CalcStrongestMoveGoesFirst(bankAtk, bankDef);

	return gNewBS->ai.strongestMoveGoesFirst[bankAtk][bankDef] == currentMove;
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
	s32 i, j;
	u8 currTarget;
	u16 move;
	s8 moveScores[MAX_MON_MOVES][MAX_BATTLERS_COUNT] = {0};
	u16 turnedToZMove[MAX_MON_MOVES] = {0};

	u16 partnerMove = MOVE_NONE;
	u16 partnerTarget = gBattleStruct->moveTarget[bankAtkPartner];
	if (gChosenMovesByBanks[bankAtkPartner] != MOVE_NONE)
		partnerMove = GetAIChosenMove(bankAtkPartner, partnerTarget);

	u8 moveLimitations = CheckMoveLimitations(bankAtk, 0, 0xFF);
	
	u8 foes[2] = {bankDef, bankDefPartner};
	bool8 foeAlive[2] = {BATTLER_ALIVE(bankDef), BATTLER_ALIVE(bankDefPartner)};
	bool8 partnerHandling[2] = {FALSE, FALSE};
	bool8 foesAlive = CountAliveMonsInBattle(BATTLE_ALIVE_DEF_SIDE, bankAtk, bankDef);

	//Check if any foe has Wide Guard
	bool8 foeHasWideGuard = FALSE;
	if (Random() % 100 < 75) //75 % chance the AI will care about wide guard this round
	{
		for (j = 0; j < (s32) NELEMS(foes); ++j)
		{
			if (foeAlive[j] && HasProtectionMoveInMoveset(foes[j], CHECK_WIDE_GUARD) && !IsBankIncapacitated(foes[j]))
			{
				foeHasWideGuard = TRUE;
				break;
			}
		}
	}

	//Check targets partner is already prepared to deal with
	bool8 partnerHitsBothFoes = FALSE;
	bool8 partnerWillAttack = !IsBankIncapacitated(bankAtkPartner) && partnerMove != MOVE_NONE; //Partner is going to attack
	bool8 partnerWillUseDamagingMove = partnerWillAttack && SPLIT(partnerMove) != SPLIT_STATUS;

	if (partnerWillAttack && foesAlive >= 2) //More than 1 target left
	{
		partnerHitsBothFoes = partnerMove != MOVE_NONE && GetBaseMoveTarget(partnerMove, bankAtkPartner) & MOVE_TARGET_SPREAD;

		if (!partnerHitsBothFoes) //Partner isn't using spread move
		{
			if (partnerTarget != bankAtkPartner && MoveKnocksOutXHits(partnerMove, bankAtkPartner, partnerTarget, 1)) //Partner will KO its target
			{
				//Find the target it will KO and assume it unimportant
				for (j = 0; j < gBattlersCount / 2; ++j)
				{
					if (partnerTarget == foes[j])
						partnerHandling[j] = TRUE;
				}
			}
		}
		else //Partner is using spread move
		{
			//Find the targets it will KO and assume them unimportant
			for (j = 0; j < gBattlersCount / 2; ++j)
			{
				if (MoveKnocksOutXHits(partnerMove, bankAtkPartner, foes[j], 1))
					partnerHandling[j] = TRUE;
			}

			if (partnerHandling[0] && partnerHandling[1]) //Partner is planning to KO both foes
			{
				if (PriorityCalc(bankAtkPartner, ACTION_USE_MOVE, partnerMove) <= 0) //With a non-priority move
				{
					u32 partnerSpeed = SpeedCalc(bankAtkPartner);

					if (partnerSpeed < SpeedCalc(foes[0]) || partnerSpeed < SpeedCalc(foes[1])) //And partner is slower then one of the foes
						partnerHandling[0] = FALSE; //Don't get complacent and at least consider bankDef to be a viable single target (still prevents moves like EQ from being considered viable)
				}
			}
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
			u8 moveTarget = GetBaseMoveTarget(move, bankAtk);
			if (foeHasWideGuard //Enemy side has mon who can use Wide Guard
			&& moveTarget & MOVE_TARGET_SPREAD) //This move is a spread move
				goto MOVE_LOOP_END;//Pretend this move sucks

			for (j = 0; j < gBattlersCount / 2; ++j)
			{
				//mgba_printf(MGBA_LOG_WARN, "");
				currTarget = foes[j];

				if (foeAlive[j] && (j == 0 || moveTarget & MOVE_TARGET_SPREAD) //Only can hit second foe with spread move
				&& !partnerHandling[j]) //Don't count the target if the partner is already taking care of it
				{
					u32 dmg = GetFinalAIMoveDamage(move, bankAtk, currTarget, 1, &damageDatas[j]);

					if (dmg > 0) //Move will do damage/hit enemy
					{
						moveScores[i][currTarget] += DOUBLES_INCREASE_HIT_FOE; //Hit one enemy

						//Don't prioritze one dependent on how much HP the foe has left if the partner is going to attack the target first
						if (gBattleMoves[move].effect == EFFECT_SUPER_FANG //Considering Super Fang
						&& partnerWillUseDamagingMove //And the partner's planning to use a damaging move
						&& (partnerTarget == currTarget || partnerHitsBothFoes) //On this same target, but won't KO (otherwise wouldn't get here)
						&& MoveWouldHitBeforeOtherMove(partnerMove, bankAtkPartner, move, bankAtk)) //And will go before Super Fang gets a chance
							moveScores[i][currTarget] -= 2; //Remove part of the added bonus

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
							#define CALC (CalcSecondaryEffectChance(bankAtk, move, ABILITY(bankAtk)) >= 50)
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
									if (CALC && GoodIdeaToLowerSpeed(currTarget, bankAtk, move, 1))
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

			if (moveTarget & MOVE_TARGET_ALL
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

		MOVE_LOOP_END:;
	}

	u8 bestIndex = 0;
	for (u32 i = 1; i < MAX_MON_MOVES; ++i)
	{
		s8 currScore = moveScores[i][bankDef]
					 + moveScores[i][bankDefPartner]
					 + moveScores[i][bankAtkPartner];

		s8 bestScore = moveScores[bestIndex][bankDef]
					 + moveScores[bestIndex][bankDefPartner]
					 + moveScores[bestIndex][bankAtkPartner];

		if (currScore > bestScore)
			bestIndex = i;
		else if (currScore == bestScore)
		{
			//Try use the one with the best range
			u16 thisMove = gBattleMons[bankAtk].moves[i];
			u8 currentBestRange = GetBaseMoveTarget(gBattleMons[bankAtk].moves[bestIndex], bankAtk);
			u8 checkBestRange = GetBaseMoveTarget(thisMove, bankAtk);

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
	if (gBattleMoves[gChosenMovesByBanks[PARTNER(bankAtk)]].effect == EFFECT_PROTECT
	&& GetBaseMoveTarget(move, bankAtk) & MOVE_TARGET_ALL
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

bool8 TryRemovePartnerDoublesKillingScore(u8 bankAtk, u8 bankDef, u16 chosenMove, bool8 doSpeedCalc)
{
	u8 partner = PARTNER(bankAtk);

	if (IS_DOUBLE_BATTLE && BATTLER_ALIVE(bankDef) && BATTLER_ALIVE(partner))
	{
		u16 partnerKillingMove = gNewBS->ai.bestDoublesKillingMoves[partner][bankDef];

		if (((!doSpeedCalc && !(bankAtk & BIT_FLANK)) || (doSpeedCalc && SpeedCalc(bankAtk) >= SpeedCalc(partner))) //Don't bother with reseting scores if no Pokemon are going to choose moves after this
		&& !IsBankIncapacitated(bankAtk) //Actually going to hit a target this turn
		&& SIDE(bankAtk) != SIDE(bankDef) //No scores are calculated for hitting partners
		&& (CountAliveMonsInBattle(BATTLE_ALIVE_DEF_SIDE, bankAtk, bankDef) >= 2 //Two targets that can be hit on enemy side
		 || MoveWouldHitFirst(chosenMove, bankAtk, bankDef) //This mon is going to hit the enemy before it can attack
		 || !MoveWouldHitFirst(partnerKillingMove, partner, bankDef)) //The partner won't hit the enemy before it can attack
		&& MoveKnocksOutXHits(chosenMove, bankAtk, bankDef, 1))
		{
			//This mon's got it covered, so recalculate killing score for the partner factoring this info in
			RemoveDoublesKillingScore(partner, bankDef);
			RemoveDoublesKillingScore(partner, PARTNER(bankDef));
		}
	}

	return FALSE;
}

void TryRemovePartnerDoublesKillingScoreComplete(u8 bankAtk, u8 bankDef, u16 chosenMove, u32 moveTarget, bool8 doSpeedCalc)
{
	//Also handles the special case where the Pokemon is using a spread move
	if (!TryRemovePartnerDoublesKillingScore(bankAtk, bankDef, chosenMove, doSpeedCalc))
	{
		if (moveTarget & MOVE_TARGET_SPREAD)
			TryRemovePartnerDoublesKillingScore(bankAtk, PARTNER(bankDef), chosenMove, doSpeedCalc); //Important in case that foe is KOd
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
	if (GetFinalAIMoveDamage(move, bankAtk, bankDef, numHits, NULL) >= gBattleMons[bankDef].hp)
		return TRUE;

	return FALSE;
}

static bool8 CalculateMoveKnocksOutXHitsFresh(u16 move, u8 bankAtk, u8 bankDef, u8 numHits)
{
	if (CalcFinalAIMoveDamage(move, bankAtk, bankDef, numHits, NULL) >= gBattleMons[bankDef].hp)
		return TRUE;

	return FALSE;
}

bool8 MoveKnocksOutXHits(u16 move, u8 bankAtk, u8 bankDef, u8 numHits)
{
	u8 movePos;
	u16 species = SPECIES(bankDef);
	u8 ability = ABILITY(bankDef);

	if (MoveBlockedBySubstitute(move, bankAtk, bankDef)
	#ifdef SPECIES_MIMIKYU
	|| (ability == ABILITY_DISGUISE && species == SPECIES_MIMIKYU && NO_MOLD_BREAKERS(ABILITY(bankAtk), move))
	#endif
	#ifdef SPECIES_EISCUE
	|| (ability == ABILITY_ICEFACE && species == SPECIES_EISCUE && SPLIT(move) == SPLIT_PHYSICAL && NO_MOLD_BREAKERS(ABILITY(bankAtk), move))
	#endif
	)
	{
		if (numHits > 0)
			numHits -= 1; //Takes at least a hit to break Disguise/Ice Face or sub
	}

	if (AttacksThisTurn(bankAtk, move) == 1) //Has to charge first
	{
		if (numHits > 0)
			numHits -= 1; //Takes at least a turn to charge
	}

	if (gBattleMoves[move].effect == EFFECT_EXPLOSION)
	{
		if (numHits > 0)
			numHits = 1; //Only can be used once before fainting
	}

	if (IsRaidBattle() && bankDef == BANK_RAID_BOSS)
	{
		u8 shieldsUp = GetNumRaidShieldsUp();
		if (shieldsUp >= numHits)
			numHits = 0;
		else
			numHits -= shieldsUp;
	}

	switch (numHits) {
		case 0:
			return FALSE;

		//Most common two cases are 1 hit and 2 hits
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

	//Unlikely this will ever even be reached
	return CalculateMoveKnocksOutXHits(move, bankAtk, bankDef, numHits);
}

bool8 MoveKnocksOutXHitsFromParty(u16 move, struct Pokemon* monAtk, u8 bankDef, u8 numHits, struct DamageCalc* damageData)
{
	u8 ability = ABILITY(bankDef);
	u16 species = SPECIES(bankDef);
	bool8 noMoldBreakers = NO_MOLD_BREAKERS(GetMonAbilityAfterTrace(monAtk, bankDef), move);

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

	if (SPLIT(move) != SPLIT_STATUS && IsDamagingMoveUnusable(move, bankAtk, bankDef))
		return 0;

	switch (gBattleMoves[move].effect)
	{
		case EFFECT_0HKO:
			return gBattleMons[bankDef].hp;

		case EFFECT_COUNTER: //Includes Metal Burst
		case EFFECT_MIRROR_COAT:
			return MathMin(CalcPredictedDamageForCounterMoves(move, bankAtk, bankDef), gBattleMons[bankDef].hp);
	}

	u32 dmg = AI_CalcDmg(bankAtk, bankDef, move, damageData);
	dmg = TryAdjustDamageForRaidBoss(bankDef, dmg);
	if (dmg >= gBattleMons[bankDef].hp)
		return gBattleMons[bankDef].hp;

	if (numHits >= 2)
	{
		dmg += GetSecondaryEffectDamage(bankDef) * (numHits - 1); //Eg Hit, Secondary Damage, Hit

		u8 defAbility = (damageData != NULL) ? damageData->defAbility : ABILITY(bankDef); //Would be loaded in by the damage calc prior

		if (BATTLER_MAX_HP(bankDef) && (defAbility == ABILITY_MULTISCALE
									 || defAbility == ABILITY_SHADOWSHIELD
									 || (IsShadowShieldBattle() && IsOfType(bankDef, TYPE_GHOST))))
		{
			return MathMin(dmg + (dmg * 2) * (numHits - 1), gBattleMons[bankDef].maxHP); //Adjust damage on subsequent hits
		}
	}

	return MathMin(dmg * numHits, gBattleMons[bankDef].hp);
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

	if (IsDamagingMoveUnusableByMon(move, monAtk, bankDef))
		return 0;

	switch (gBattleMoves[move].effect) 
	{
		case EFFECT_0HKO:
			return gBattleMons[bankDef].hp;
	}

	u32 dmg = AI_CalcPartyDmg(FOE(bankDef), bankDef, move, monAtk, damageData) * numHits;
	dmg = TryAdjustDamageForRaidBoss(bankDef, dmg);
	return MathMin(dmg, gBattleMons[bankDef].maxHP);
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

static void ClearDamageByMove(u8 bankAtk, u8 bankDef, u8 movePos)
{
	gNewBS->ai.damageByMove[bankAtk][bankDef][movePos] = 0xFFFFFFFF;
}

static move_t CalcStrongestMoveIgnoringMove(const u8 bankAtk, const u8 bankDef, const bool8 onlySpreadMoves, const u16 ignoreMove)
{
	u32 predictedDamage;
	u16 strongestMove = gBattleMons[bankAtk].moves[0];
	u32 highestDamage = 0;
	u16 bestMoveAcc = 0;
	u16 defHP = gBattleMons[bankDef].hp;
	bool8 badIdeaToMakeContact = BadIdeaToMakeContactWith(bankAtk, bankDef);
	bool8 takesRecoilDamage = ABILITY(bankAtk) != ABILITY_MAGICGUARD;
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
		u16 move = gBattleMons[bankAtk].moves[i];
		if (move == MOVE_NONE)
			break;

		if (move == ignoreMove)
			continue;

		move = TryReplaceMoveWithZMove(bankAtk, bankDef, move);

		if (!(gBitTable[i] & moveLimitations))
		{
			if (gBattleMoves[move].power == 0
			|| (onlySpreadMoves && !(GetBaseMoveTarget(move, bankAtk) & MOVE_TARGET_SPREAD)))
				continue;

			u8 moveEffect = gBattleMoves[move].effect;

			if (moveEffect == EFFECT_0HKO)
			{
				if (gBattleMons[bankAtk].level < gBattleMons[bankDef].level
				|| (move == MOVE_SHEERCOLD && IsOfType(bankDef, TYPE_ICE))
				|| (ABILITY(bankDef) == ABILITY_STURDY)
				|| (IsDynamaxed(bankDef) && !(gNewBS->dynamaxData.raidShieldsUp && bankDef == BANK_RAID_BOSS)) //Not hitting Raid shield
				|| !(MoveWillHit(move, bankAtk, bankDef))) //Never worth the risk unless it's a sure hit
					continue;
			}
	
			predictedDamage = CalcFinalAIMoveDamage(move, bankAtk, bankDef, 1, &damageData);

			if (predictedDamage > (u32) highestDamage)
			{
				strongestMove = move;
				highestDamage = predictedDamage;
			}
			else if (predictedDamage == highestDamage //This move does the same as the strongest move so far (they probably just both KO)
			&& moveEffect != EFFECT_COUNTER
			&& moveEffect != EFFECT_MIRROR_COAT) //Never try to make counter moves a priority unless they do the most damage
			{
				if (PriorityCalc(bankAtk, ACTION_USE_MOVE, move) > PriorityCalc(bankAtk, ACTION_USE_MOVE, strongestMove)) //Use faster of two strongest moves
				{
					strongestMove = move;
				}
				else
				{
					//Find which move has better Acc
					u16 currAcc = (MoveWillHit(move, bankAtk, bankDef)) ? 100 : CalcAIAccuracy(move, bankAtk, bankDef);

					if (currAcc > bestMoveAcc && bestMoveAcc < 100)
					{
						//This move has a better chance of hitting
						strongestMove = move;
						bestMoveAcc = currAcc;
					}
					else if (currAcc == bestMoveAcc || currAcc >= 100)
					{
						if (!CanBeChoiceLocked(bankAtk)) //Only care about contact and recoil if not going to be locked into a single move
						{
							//Pick a non-contact move if contact is a bad idea
							if (badIdeaToMakeContact)
							{
								bool8 strongestMoveContact = CheckContact(strongestMove, bankAtk);
								bool8 currMoveContact = CheckContact(move, bankAtk);

								if (strongestMoveContact && !currMoveContact)
								{
									//The new move isn't contact unlike the old best move
									strongestMove = move;
									continue; //Proceed to next move
								}
								else if (!strongestMoveContact && currMoveContact)
									continue; //Check the next move - this move is out
								else if (strongestMoveContact && currMoveContact)
								{
									//They're both contact moves, so pick the one less likely to proc the contact
									bool8 strongestMoveMultiHit = gBattleMoves[strongestMove].effect == EFFECT_MULTI_HIT;
									bool8 currMoveMultiHit = gBattleMoves[move].effect == EFFECT_MULTI_HIT;

									if (strongestMoveMultiHit && !currMoveMultiHit)
									{
										//The new move isn't a multi-hit unlike the old best move
										strongestMove = move;
										continue; //Proceed to next move
									}
									else if (!strongestMoveMultiHit && currMoveMultiHit)
										continue; //Check the next move - this move is out
								}
							}

							//Pick a non-recoil move preferably
							if (takesRecoilDamage)
							{
								bool8 strongestMoveRecoil = CheckRecoil(strongestMove);
								bool8 currMoveRecoil = CheckRecoil(move);

								if (strongestMoveRecoil && !currMoveRecoil)
								{
									//Prefer the move that doesn't do recoil damage
									strongestMove = move;
									continue; //Proceed to next move
								}
								else if (!strongestMoveRecoil && currMoveRecoil)
									continue; //Check the next move - this move is out
							}

							//Pick a non-stat recoil move preferably
							{
								bool8 strongestMoveStatRecoil = IsStatRecoilMove(strongestMove);
								bool8 currMoveStatRecoil = IsStatRecoilMove(move);

								if (strongestMoveStatRecoil && !currMoveStatRecoil)
								{
									//Prefer the move that doesn't lower stats
									strongestMove = move;
									continue; //Proceed to next move
								}
								else if (!strongestMoveStatRecoil && currMoveStatRecoil)
									continue; //Check the next move - this move is out
							}
						}

						//Pick a move at random.
						if (Random() & 1)
							strongestMove = move;
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

move_t CalcStrongestMove(const u8 bankAtk, const u8 bankDef, const bool8 onlySpreadMoves)
{
	return CalcStrongestMoveIgnoringMove(bankAtk, bankDef, onlySpreadMoves, 0);
}

void RecalcStrongestMoveIgnoringMove(const u8 bankAtk, const u8 bankDef, const u16 ignoreMove)
{
	gNewBS->ai.strongestMove[bankAtk][bankDef] = CalcStrongestMoveIgnoringMove(bankAtk, bankDef, FALSE, ignoreMove);
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

bool8 ResistsAllMoves(const u8 bankAtk, const u8 bankDef)
{
	u8 moveLimitations = CheckMoveLimitations(bankAtk, 0, 0xFF);
	u16 hp = gBattleMons[bankDef].hp;

	for (u32 i = 0; i < MAX_MON_MOVES; ++i)
	{
		u16 move = GetBattleMonMove(bankAtk, i);
		if (move == MOVE_NONE)
			break;

		if (!(gBitTable[i] & moveLimitations))
		{
			u32 dmg = GetFinalAIMoveDamage(move, bankAtk, bankDef, 1, NULL); //Assume damage has already been calculated before so don't pass in damage calc struct
			if (dmg >= hp / 3) //Move does a third or more of remaining HP (same criteria as used in switching)
				return FALSE;
		}
	}

	return TRUE;
}

bool8 ResistsAllMovesAtFullHealth(const u8 bankAtk, const u8 bankDef)
{
	u16 backupHealth = gBattleMons[bankDef].hp;
	gBattleMons[bankDef].hp = gBattleMons[bankDef].maxHP; //Simulate full HP
	
	bool8 ret = ResistsAllMoves(bankAtk, bankDef);
	
	gBattleMons[bankDef].hp = backupHealth; //Restore from backup
	
	return ret;
}

static bool8 WallsFoe(const u8 bankAtk, const u8 bankDef)
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


void ForceSpecificDamageRecalculation(const u8 bankAtk, const u8 bankDef, const u8 movePos)
{
	ClearStrongestMoveAndCanKnockOut(bankAtk, bankDef);
	ClearDamageByMove(bankAtk, bankDef, movePos);

	if (IS_DOUBLE_BATTLE)
	{
		u8 bankDefPartner = PARTNER(bankDef);
		ClearStrongestMoveAndCanKnockOut(bankAtk, bankDefPartner);
		ClearDamageByMove(bankAtk, bankDefPartner, movePos);
		RemoveDoublesKillingScore(bankAtk, bankDef);
		RemoveDoublesKillingScore(bankAtk, bankDefPartner);
	}
}

void ForceCompleteDamageRecalculation(const u8 bankAtk)
{
	u8 foe1 = FOE(bankAtk);
	u8 foe2 = PARTNER(foe1);
	bool8 isDoubles = IS_DOUBLE_BATTLE != 0;

	ClearStrongestMoveAndCanKnockOut(bankAtk, foe1);
	if (isDoubles)
		ClearStrongestMoveAndCanKnockOut(bankAtk, foe2);

	for (u32 i = 0; i < MAX_MON_MOVES; ++i)
	{
		ClearDamageByMove(bankAtk, foe1, i);
		if (isDoubles)
			ClearDamageByMove(bankAtk, foe2, i);
	}

	if (isDoubles)
	{
		RemoveDoublesKillingScore(bankAtk, foe1);
		RemoveDoublesKillingScore(bankAtk, foe2);
	}
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
	|| (gStatuses3[bankDef] & STATUS3_ALWAYS_HITS && gDisableStructs[bankDef].bankWithSureHit == bankAtk))
		return TRUE;

	if (((gStatuses3[bankDef] & (STATUS3_IN_AIR | STATUS3_SKY_DROP_ATTACKER | STATUS3_SKY_DROP_TARGET)) && !gSpecialMoveFlags[move].gIgnoreInAirMoves)
	||  ((gStatuses3[bankDef] & STATUS3_UNDERGROUND) && !gSpecialMoveFlags[move].gIgnoreUndergoundMoves)
	||  ((gStatuses3[bankDef] & STATUS3_UNDERWATER) && !gSpecialMoveFlags[move].gIgnoreUnderwaterMoves)
	||   (gStatuses3[bankDef] & STATUS3_DISAPPEARED))
		return FALSE;

	if ((move == MOVE_TOXIC && IsOfType(bankAtk, TYPE_POISON))
	||  (gSpecialMoveFlags[move].gAlwaysHitWhenMinimizedMoves && gStatuses3[bankDef] & STATUS3_MINIMIZED)
	|| ((gStatuses3[bankDef] & STATUS3_TELEKINESIS) && gBattleMoves[move].effect != EFFECT_0HKO)
	||  gBattleMoves[move].accuracy == 0
	|| ((gBattleWeather & WEATHER_RAIN_ANY) && gSpecialMoveFlags[move].gAlwaysHitInRainMoves && WEATHER_HAS_EFFECT)
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
		s8 atkPriority = PriorityCalc(bankAtk, ACTION_USE_MOVE, moveAtk);
		s8 defPriority = PriorityCalc(bankDef, ACTION_USE_MOVE, moveDef);

		if (atkPriority > defPriority)
			return TRUE;
		else if (defPriority > atkPriority)
			return FALSE;
	}

//BracketCalc
	if (BracketCalc(bankAtk, ACTION_USE_MOVE, moveAtk) > BracketCalc(bankDef, ACTION_USE_MOVE, moveDef)) //Hehehe...AI knows when its Quick Claw activates
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

	if (gChosenActionByBank[bankDef] == ACTION_SWITCH
	|| gChosenActionByBank[bankDef] == ACTION_USE_ITEM)
		return FALSE; //Player tried to cheese the Fake Out - only gets used on cheesing recalc

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
	for (u32 i = STAT_STAGE_ATK; i < BATTLE_STATS_NO; ++i)
	{
		if (STAT_STAGE(bank, i) > 6)
			++positiveStages;
	}

	return positiveStages;
}

u8 CountBanksNegativeStatStages(u8 bank)
{
	u8 negativeStages = 0;
	for (u32 i = STAT_STAGE_ATK; i < BATTLE_STATS_NO; ++i)
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

	for (u32 i = 0; i < PARTY_SIZE; ++i)
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

bool8 WillBeFasterAfterSpeedDrop(u8 bankAtk, u8 bankDef, u8 reduceBy)
{
	u8 oldSpeedStage = STAT_STAGE(bankDef, STAT_STAGE_SPEED); //Backup current stat stage before modification
	
	//Emulate speed drop
	if (reduceBy > STAT_STAGE(bankDef, STAT_STAGE_SPEED))
		STAT_STAGE(bankDef, STAT_STAGE_SPEED) = 0;
	else
		STAT_STAGE(bankDef, STAT_STAGE_SPEED) -= reduceBy;

	bool8 faster = SpeedCalc(bankAtk) <= SpeedCalc(bankDef); //Check speeds now
	STAT_STAGE(bankDef, STAT_STAGE_SPEED) = oldSpeedStage; //Restore speed from backup
	return faster;
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
	u8 ability = ABILITY(bankAtk);

	if (ability != ABILITY_NONE //Ability has effect
	&& gNewBS->ai.backupAbilities[bankAtk] != ABILITY_NONE //Is temp Mega Evolved
	&& ShouldAIDelayMegaEvolution(bankAtk, bankDef, move, TRUE))
		ability = gNewBS->ai.backupAbilities[bankAtk]; //Base form Ability

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

u8 GetMonAbilityAfterTrace(struct Pokemon* mon, u8 foe)
{
	u8 ability = GetMonAbility(mon);
	
	if (IS_SINGLE_BATTLE && ability == ABILITY_TRACE)
	{
		u8 foeAbility = *GetAbilityLocation(foe);
		if (!gSpecialAbilityFlags[foeAbility].gTraceBannedAbilities)
			ability = foeAbility; //What the Ability will become
	}

	return ability;
}

//Basically a bunch of checks handled in the Negatives but still needed for damage calcs to work properly
//These by default are not handled in IsUnusableMove
bool8 IsDamagingMoveUnusable(u16 move, u8 bankAtk, u8 bankDef)
{
	if (NO_MOLD_BREAKERS(ABILITY(bankAtk), move))
	{
		switch (ABILITY(bankDef))
		{
			//Electric
			case ABILITY_VOLTABSORB:
			case ABILITY_MOTORDRIVE:
			case ABILITY_LIGHTNINGROD:
				if (GetMoveTypeSpecial(bankAtk, move) == TYPE_ELECTRIC)
					return TRUE;
				break;

			//Water
			case ABILITY_WATERABSORB:
			case ABILITY_DRYSKIN:
			case ABILITY_STORMDRAIN:
				if (GetMoveTypeSpecial(bankAtk, move) == TYPE_WATER)
					return TRUE;
				break;

			//Fire
			case ABILITY_FLASHFIRE:
				if (GetMoveTypeSpecial(bankAtk, move) == TYPE_FIRE)
					return TRUE;
				break;

			//Grass
			case ABILITY_SAPSIPPER:
				if (GetMoveTypeSpecial(bankAtk, move) == TYPE_GRASS)
					return TRUE;
				break;

			//Move category checks
			case ABILITY_SOUNDPROOF:
				if (CheckSoundMove(move))
					return TRUE;
				break;

			case ABILITY_BULLETPROOF:
				if (gSpecialMoveFlags[move].gBallBombMoves)
					return TRUE;
				break;

			case ABILITY_DAZZLING:
			#ifdef ABILITY_QUEENLYMAJESTY
			case ABILITY_QUEENLYMAJESTY:
			#endif
				if (PriorityCalc(bankAtk, ACTION_USE_MOVE, move) > 0) //Check if right num
					return TRUE;
				break;
		}
	}

	switch (gBattleMoves[move].effect) {
		case EFFECT_FAKE_OUT:
			if (!gDisableStructs[bankAtk].isFirstTurn)
				return TRUE;
			break;
		case EFFECT_BURN_UP:
			if (!IsOfType(bankAtk, TYPE_FIRE))
				return TRUE;
			break;
		case EFFECT_POLTERGEIST:
			if (WillPoltergeistFail(ITEM(bankDef), ABILITY(bankDef)))
				return TRUE;
			break;
		case EFFECT_0HKO:
			if (gBattleMons[bankAtk].level < gBattleMons[bankDef].level)
				return TRUE;
			if (move == MOVE_SHEERCOLD && IsOfType(bankDef, TYPE_ICE))
				return TRUE;
			if (IsDynamaxed(bankDef) && !(gNewBS->dynamaxData.raidShieldsUp && bankDef == BANK_RAID_BOSS))
				return TRUE;
			if (NO_MOLD_BREAKERS(ABILITY(bankAtk), move) && ABILITY(bankDef) == ABILITY_STURDY)
				return TRUE;
			break;
		case EFFECT_SUCKER_PUNCH:
			if (!IsSuckerPunchOkayToUseThisRound(move, bankAtk, bankDef))
				return TRUE;
			break;
	}

	if (IsDynamaxed(bankDef) && gSpecialMoveFlags[move].gDynamaxBannedMoves)
		return TRUE;

	//Raid Battle Check
	if (IsRaidBattle())
	{
		if (bankAtk == BANK_RAID_BOSS && gSpecialMoveFlags[move].gRaidBattleBannedRaidMonMoves)
			return TRUE;
	}

	//Primal Weather Check
	if ((gBattleWeather & WEATHER_SUN_PRIMAL && GetMoveTypeSpecial(bankAtk, move) == TYPE_WATER)
	|| (gBattleWeather & WEATHER_RAIN_PRIMAL && GetMoveTypeSpecial(bankAtk, move) == TYPE_FIRE))
		return TRUE;

	//Terrain Check
	switch (gTerrainType) {
		case PSYCHIC_TERRAIN:
			if (CheckGrounding(bankDef) == GROUNDED
			&& PriorityCalc(bankAtk, ACTION_USE_MOVE, move) > 0)
				return TRUE;
			break;
	}

	return FALSE;
}

bool8 IsDamagingMoveUnusableByMon(u16 move, struct Pokemon* monAtk, u8 bankDef)
{
	if (NO_MOLD_BREAKERS(GetMonAbilityAfterTrace(monAtk, bankDef), move))
	{
		switch (ABILITY(bankDef))
		{
			//Electric
			case ABILITY_VOLTABSORB:
			case ABILITY_MOTORDRIVE:
			case ABILITY_LIGHTNINGROD:
				if (GetMonMoveTypeSpecial(monAtk, move) == TYPE_ELECTRIC)
					return TRUE;
				break;

			//Water
			case ABILITY_WATERABSORB:
			case ABILITY_DRYSKIN:
			case ABILITY_STORMDRAIN:
				if (GetMonMoveTypeSpecial(monAtk, move) == TYPE_WATER)
					return TRUE;
				break;

			//Fire
			case ABILITY_FLASHFIRE:
				if (GetMonMoveTypeSpecial(monAtk, move) == TYPE_FIRE)
					return TRUE;
				break;

			//Grass
			case ABILITY_SAPSIPPER:
				if (GetMonMoveTypeSpecial(monAtk, move) == TYPE_GRASS)
					return TRUE;
				break;

			//Move category checks
			case ABILITY_SOUNDPROOF:
				if (CheckSoundMove(move))
					return TRUE;
				break;

			case ABILITY_BULLETPROOF:
				if (gSpecialMoveFlags[move].gBallBombMoves)
					return TRUE;
				break;

			case ABILITY_DAZZLING:
			#ifdef ABILITY_QUEENLYMAJESTY
			case ABILITY_QUEENLYMAJESTY:
			#endif
				if (PriorityCalcMon(monAtk, move) > 0) //Check if right num
					return TRUE;
				break;
		}
	}

	switch (gBattleMoves[move].effect)
	{
		case EFFECT_BURN_UP:
			if (!IsMonOfType(monAtk, TYPE_FIRE))
				return TRUE;
			break;
		case EFFECT_POLTERGEIST:
			if (WillPoltergeistFail(ITEM(bankDef), ABILITY(bankDef)))
				return TRUE;
			break;
		case EFFECT_0HKO:
			if (GetMonData(monAtk, MON_DATA_LEVEL, NULL) < gBattleMons[bankDef].level)
				return TRUE;
			if (move == MOVE_SHEERCOLD && IsOfType(bankDef, TYPE_ICE))
				return TRUE;
			break;
		case EFFECT_COUNTER: //Includes Metal Burst
		case EFFECT_MIRROR_COAT:
			return TRUE;
	}

	if (IsDynamaxed(bankDef) && gSpecialMoveFlags[move].gDynamaxBannedMoves)
		return TRUE;

	//Primal Weather Check
	if ((gBattleWeather & WEATHER_SUN_PRIMAL && GetMonMoveTypeSpecial(monAtk, move) == TYPE_WATER)
	|| (gBattleWeather & WEATHER_RAIN_PRIMAL && GetMonMoveTypeSpecial(monAtk, move) == TYPE_FIRE))
		return TRUE;

	//Terrain Check
	switch (gTerrainType) {
		case PSYCHIC_TERRAIN:
			if (CheckGrounding(bankDef) == GROUNDED
			&& PriorityCalcMon(monAtk, move) > 0)
				return TRUE;
			break;
	}

	return FALSE;
}

bool8 IsStatRecoilMove(u16 move)
{
	switch (gBattleMoves[move].effect)
	{
		case EFFECT_SUPERPOWER:
		case EFFECT_OVERHEAT:
			return TRUE;
		default:
			return FALSE;
	}
}

bool8 IsSuckerPunchOkayToUseThisRound(u16 move, u8 bankAtk, u8 bankDef)
{
	u8 movePos = FindMovePositionInMoveset(move, bankAtk);

	if (AI_THINKING_STRUCT->aiFlags > AI_SCRIPT_SEMI_SMART //Only smart AI
	&& movePos < MAX_MON_MOVES //Mon actually knows Sucker Punch (and isn't just copying it from somewhere)
	&& SIDE(bankAtk) == B_SIDE_OPPONENT //AI side is attacker
	&& gBattleMons[bankAtk].pp[movePos] < CalculatePPWithBonus(move, gBattleMons[bankAtk].ppBonuses, movePos) //Mon has revealed Sucker Punch
	&& (StatusMoveInMoveset(bankDef) //Player can cheese AI with status move spam
	 || (DamagingPriorityMoveInMovesetThatAffects(bankDef, bankAtk) && SpeedCalc(bankDef) > SpeedCalc(bankAtk)))) //Player can cheese AI with priority spam
	{
		if (!gNewBS->ai.suckerPunchOkay[bankAtk]) //This turn wasn't randomly chosen to be okay for Sucker Punch
			return FALSE;
	}
	else //Regular Sucker Punch logic
	{
		u16 predictedMove = IsValidMovePrediction(bankDef, bankAtk);
		if (predictedMove != MOVE_NONE)
		{
			if (SPLIT(predictedMove) == SPLIT_STATUS
			|| !MoveWouldHitFirst(move, bankAtk, bankDef))
				return FALSE;
		}
	}

	return TRUE;
}

bool8 CanHealFirstToPreventKnockOut(u8 bankAtk, u8 foe)
{
	u16 move;
	u8 moveLimitations = CheckMoveLimitations(bankAtk, 0, 0xFF);
	u16 foePrediction = IsValidMovePrediction(foe, bankAtk);
	bool8 isAtkDynamaxed = IsDynamaxed(bankAtk);

	for (u32 i = 0; i < MAX_MON_MOVES; ++i)
	{
		move = GetBattleMonMove(bankAtk, i);
		if (move == MOVE_NONE)
			break;

		if (!(gBitTable[i] & moveLimitations))
		{
			u8 effect = gBattleMoves[move].effect;
			if ((effect == EFFECT_RESTORE_HP && !isAtkDynamaxed)
			 || (effect == EFFECT_MORNING_SUN && !isAtkDynamaxed)
			 || (effect == EFFECT_PROTECT && DoesProtectionMoveBlockMove(foe, bankAtk, foePrediction, move)))
			{
				u32 healAmount = GetAmountToRecoverBy(bankAtk, foe, move);
				if (MoveWouldHitFirst(move, bankAtk, foe)
				&& !CanKnockOutAfterHealing(foe, bankAtk, healAmount, 1, FALSE))
					return TRUE;
			}
		}
	}

	return FALSE;
}

s32 GetMonPassiveRecovery(struct Pokemon* mon, unusedArg u8 bank)
{
	s32 amountToRecover = 0;
	u8 ability = GetMonAbility(mon);
	u8 itemEffect = GetMonItemEffect(mon);
	u16 maxHp = GetMonData(mon, MON_DATA_MAX_HP, NULL);

	if (itemEffect == ITEM_EFFECT_LEFTOVERS)
		amountToRecover += MathMax(1, maxHp / 16);
	else if (itemEffect == ITEM_EFFECT_BLACK_SLUDGE)
	{
		if (IsMonOfType(mon, TYPE_POISON))
			amountToRecover += MathMax(1, maxHp / 16);
		else
			amountToRecover -= MathMax(1, maxHp / 8); //Damage
	}

	if (gBattleWeather & WEATHER_RAIN_ANY && WEATHER_HAS_EFFECT && itemEffect != ITEM_EFFECT_UTILITY_UMBRELLA)
	{
		if (ability == ABILITY_RAINDISH)
			amountToRecover += MathMax(1, maxHp / 16);
		else if (ability == ABILITY_DRYSKIN)
			amountToRecover += MathMax(1, maxHp / 8);
	}

	if (gBattleWeather & WEATHER_SUN_ANY && WEATHER_HAS_EFFECT)
	{
		if (ability == ABILITY_SOLARPOWER || ability == ABILITY_DRYSKIN)
			amountToRecover -= MathMax(1, maxHp / 8); //Damage
	}

	if (gBattleWeather & WEATHER_HAIL_ANY && WEATHER_HAS_EFFECT)
	{
		if (ability == ABILITY_ICEBODY)
			amountToRecover += MathMax(1, maxHp / 16);
	}

	if (ability == ABILITY_POISONHEAL && GetMonData(mon, MON_DATA_STATUS, NULL) & STATUS1_PSN_ANY)
		amountToRecover += MathMax(1, maxHp / 8);

	if (gTerrainType == GRASSY_TERRAIN && CheckMonGrounding(mon) == GROUNDED)
		amountToRecover += MathMax(1, maxHp / 16);

	return amountToRecover;
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

static u32 CalcSecondaryEffectDamage(u8 bank)
{
	u32 damage = 0;
	u8 ability = ABILITY(bank);

	if (ability != ABILITY_MAGICGUARD)
	{
		if (gWishFutureKnock.weatherDuration != 1)
			damage += GetSandstormDamage(bank)
				    + GetHailDamage(bank); //Weather's not about to end

		if ((gBattleMons[bank].status1 & STATUS1_SLEEP) != 1)
			damage += GetNightmareDamage(bank); //Sleep's not about to end

		damage += GetTrapDamage(bank)
			+ GetLeechSeedDamage(bank)
			+ GetPoisonDamage(bank, TRUE)
			+ GetBurnDamage(bank)
			+ GetCurseDamage(bank)
			+ GetSeaOfFireDamage(bank) //Sea of Fire runs on last turn
			+ GetBadDreamsDamage(bank)
			+ GetBadThoughtsDamage(bank)
			+ GetGMaxVineLashDamage(bank)
			+ GetGMaxWildfireDamage(bank)
			+ GetGMaxCannonadeDamage(bank)
			+ GetGMaxVolcalithDamage(bank);
	}

	return damage;
}

u32 GetSecondaryEffectDamage(u8 bank)
{
	if (gNewBS->ai.secondaryEffectDamage[bank] != 0xFFFFFFFF)
		return gNewBS->ai.secondaryEffectDamage[bank];

	gNewBS->ai.secondaryEffectDamage[bank] = CalcSecondaryEffectDamage(bank);
	return gNewBS->ai.secondaryEffectDamage[bank];
}

bool8 IsTakingSecondaryDamage(u8 bank)
{
	return GetSecondaryEffectDamage(bank) > 0;
}

bool8 WillFaintFromSecondaryDamage(u8 bank)
{
	if (gStatuses3[bank] & STATUS3_PERISH_SONG
	&&  gDisableStructs[bank].perishSongTimer == 0)
		return TRUE;

	u8 hp = MathMin(gBattleMons[bank].hp + GetAmountToRecoverBy(bank, 0, MOVE_PROTECT), gBattleMons[bank].maxHP); //Assume leftover etc. healing first

	return GetSecondaryEffectDamage(bank) >= hp;
}

static u32 GetContactDamageByDefAbilityItemEffect(u8 defAbility, u8 defItemEffect, u16 baseMaxHP)
{
	u32 dmg = 0;

	if (defAbility == ABILITY_ROUGHSKIN
	#ifdef ABILITY_IRONBARBS
	|| defAbility == ABILITY_IRONBARBS
	#endif
	)
		dmg += baseMaxHP / 8;

	if (defItemEffect == ITEM_EFFECT_ROCKY_HELMET)
		dmg += baseMaxHP / 6;

	return dmg;
}

u32 GetContactDamage(u16 move, u16 bankAtk, u16 bankDef)
{
	if (MoveBlockedBySubstitute(move, bankAtk, bankDef))
		return 0;

	if (CanNeverMakeContact(bankAtk) || ABILITY(bankAtk) == ABILITY_MAGICGUARD)
		return 0;

	return GetContactDamageByDefAbilityItemEffect(ABILITY(bankDef), ITEM_EFFECT(bankDef), GetBaseMaxHP(bankAtk));
}

u32 GetContactDamageMonAtk(struct Pokemon* monAtk, u16 bankDef)
{
	if (CanMonNeverMakeContact(monAtk) || GetMonAbilityAfterTrace(monAtk, bankDef) == ABILITY_MAGICGUARD)
		return 0;

	return GetContactDamageByDefAbilityItemEffect(ABILITY(bankDef), ITEM_EFFECT(bankDef), monAtk->maxHP);
}

u32 GetContactDamageMonDef(u16 bankAtk, struct Pokemon* monDef)
{
	if (CanNeverMakeContact(bankAtk) || ABILITY(bankAtk) == ABILITY_MAGICGUARD)
		return 0;

	return GetContactDamageByDefAbilityItemEffect(GetMonAbilityAfterTrace(monDef, bankAtk), GetMonItemEffect(monDef), GetBaseMaxHP(bankAtk));
}

bool8 WillFaintFromContactDamage(u8 bankAtk, u8 bankDef, u16 move)
{
	return GetContactDamage(move, bankAtk, bankDef) >= gBattleMons[bankAtk].hp;
}

bool8 HighChanceOfBeingImmobilized(u8 bank)
{
	u32 odds = 100; //The odds of being able to land an attack

	if (gBattleMons[bank].status1 & STATUS1_PARALYSIS)
		odds = (odds * 75) / 100; //75% chance of attacking
	else if (gBattleMons[bank].status1 & STATUS1_FREEZE)
		odds = (odds * 20) / 100; //20% chance of attacking

	if (gBattleMons[bank].status2 & STATUS2_INFATUATION)
		odds = (odds * 50) / 100; //50% chance of attacking

	if ((gBattleMons[bank].status2 & STATUS2_CONFUSION) > 1)
	{
		#ifdef OLD_CONFUSION_CHANCE
		odds = (odds * 50) / 100; //50% chance of attacking
		#else
		odds = (odds * 67) / 100; //67% chance of attacking
		#endif
	}

	return odds <= 50;
}

u16 CalcSecondaryEffectChance(u8 bank, u16 move, u8 ability)
{
	u16 chance = gBattleMoves[move].secondaryEffectChance;

	if (ability == ABILITY_SERENEGRACE || BankHasRainbow(bank))
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

bool8 ShouldAIDelayMegaEvolution(u8 bankAtk, u8 bankDef, u16 move, bool8 optimizeAndLookAtMegaPotential)
{
	u8 atkAbility, megaAbility;
	
	if (optimizeAndLookAtMegaPotential && gNewBS->ai.megaPotential[bankAtk] == NULL)
		return TRUE; //This bank can't Mega Evolve

	atkAbility = ABILITY(bankAtk);
	megaAbility = GetBankMegaFormAbility(bankAtk, bankDef);

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
				case MOVE_OBSTRUCT:
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

bool8 ShouldPredictBankToMegaEvolve(u8 bank)
{
	if (!IsPlayerInControl(bank))
		return TRUE; //Always predict the AI to Mega Evolve

	if (SPECIES(bank) == SPECIES_NECROZMA_DUSK_MANE
	||  SPECIES(bank) == SPECIES_NECROZMA_DAWN_WINGS)
		return FALSE; //Don't predict player to Ultra Burst

	if (!(AI_THINKING_STRUCT->aiFlags & AI_SCRIPT_CHECK_GOOD_MOVE))
		return FALSE; //Dumb and Semi-Smart AI should never predict a Mega Evo

	if (!gDisableStructs[bank].isFirstTurn)
		return FALSE; //If the player didn't Mega Evolve on the first turn, they're probably not going to (yet)

	u16 megaSpecies = GetMegaSpecies(SPECIES(bank), ITEM(bank), gBattleMons[bank].moves);
	if (megaSpecies != SPECIES_NONE && !ShouldReplaceTypesWithCamomons()) //Types are always the same in Camomons so this logic won't apply
	{
		if (gBaseStats[megaSpecies].type1 != gBattleMons[bank].type1
		||  gBaseStats[megaSpecies].type2 != gBattleMons[bank].type2)
			return FALSE; //If the Pokemon is changing type, don't predict because the player might catch on and purposely not Mega Evolve to mess with the AI
	}

	return TRUE;
}

void ClearMovePredictionsOnBank(u8 bank)
{
	u32 i;
	
	for (i = 0; i < gBattlersCount; ++i)
		gNewBS->ai.movePredictions[i][bank] = MOVE_NONE;
}

bool8 BadIdeaToPutToSleep(u8 bankDef, u8 bankAtk)
{
	u8 defItemEffect = ITEM_EFFECT(bankDef);
	u8 defAbility = ABILITY(bankDef);

	return !CanBePutToSleep(bankDef, TRUE)
		|| gStatuses3[bankDef] & STATUS3_YAWN
		|| (gBattleTypeFlags & BATTLE_TYPE_FRONTIER && defItemEffect == ITEM_EFFECT_CURE_SLP) //Don't use this logic in general battles
		|| (gBattleTypeFlags & BATTLE_TYPE_FRONTIER && defItemEffect == ITEM_EFFECT_CURE_STATUS) //Don't use this logic in general battles
		|| defAbility == ABILITY_EARLYBIRD
		|| defAbility == ABILITY_SHEDSKIN
		|| (defAbility == ABILITY_SYNCHRONIZE && CanBePutToSleep(bankAtk, TRUE))
		|| (defAbility == ABILITY_HYDRATION && gBattleWeather & WEATHER_RAIN_ANY && gWishFutureKnock.weatherDuration != 1 && WEATHER_HAS_EFFECT)
		|| (IS_DOUBLE_BATTLE && BATTLER_ALIVE(PARTNER(bankDef)) && ABILITY(PARTNER(bankDef)) == ABILITY_HEALER);
}

bool8 BadIdeaToPoison(u8 bankDef, u8 bankAtk)
{
	u8 defItemEffect = ITEM_EFFECT(bankDef);
	u8 defAbility = ABILITY(bankDef);
	u8 atkAbility = ABILITY(bankAtk);

	return !CanBePoisoned(bankDef, bankAtk, TRUE)
		||  (gBattleTypeFlags & BATTLE_TYPE_FRONTIER && defItemEffect == ITEM_EFFECT_CURE_PSN) //Don't use this logic in general battles
		||  (gBattleTypeFlags & BATTLE_TYPE_FRONTIER && defItemEffect == ITEM_EFFECT_CURE_STATUS) //Don't use this logic in general battles
		||  defAbility == ABILITY_SHEDSKIN
		||  defAbility == ABILITY_POISONHEAL
		|| (defAbility == ABILITY_MAGICGUARD && !MoveInMoveset(MOVE_HEX, bankAtk) && !MoveInMoveset(MOVE_VENOSHOCK, bankAtk))
		||  defAbility == ABILITY_QUICKFEET
		|| (defAbility == ABILITY_SYNCHRONIZE && CanBePoisoned(bankAtk, bankDef, TRUE) && !GoodIdeaToPoisonSelf(bankAtk))
		|| (defAbility == ABILITY_MARVELSCALE && PhysicalMoveInMoveset(bankAtk))
		|| (defAbility == ABILITY_NATURALCURE && CAN_SWITCH_OUT(bankDef))
		|| (defAbility == ABILITY_TOXICBOOST && RealPhysicalMoveInMoveset(bankDef))
		|| (defAbility == ABILITY_GUTS && RealPhysicalMoveInMoveset(bankDef))
		|| (atkAbility == ABILITY_POISONTOUCH && ContactMovesThatAffectTargetInMoveset(bankAtk, bankDef)) //Just poison it using attacker's ability
		|| (defAbility == ABILITY_HYDRATION && gBattleWeather & WEATHER_RAIN_ANY && gWishFutureKnock.weatherDuration != 1 && WEATHER_HAS_EFFECT)
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
		 || (atkAbility == ABILITY_TOXICBOOST && RealPhysicalMoveInMoveset(bankAtk))
		 || (atkAbility == ABILITY_GUTS && RealPhysicalMoveInMoveset(bankAtk))
		 ||  MoveInMoveset(MOVE_FACADE, bankAtk)
		 ||  MoveInMoveset(MOVE_PSYCHOSHIFT, bankAtk));
}

bool8 BadIdeaToParalyze(u8 bankDef, u8 bankAtk)
{
	u8 defItemEffect = ITEM_EFFECT(bankDef);
	u8 defAbility = ABILITY(bankDef);

	return !CanBeParalyzed(bankDef, TRUE)
	   ||  (gBattleTypeFlags & BATTLE_TYPE_FRONTIER && defItemEffect == ITEM_EFFECT_CURE_PAR) //Don't use this logic in general battles
	   ||  (gBattleTypeFlags & BATTLE_TYPE_FRONTIER && defItemEffect == ITEM_EFFECT_CURE_STATUS) //Don't use this logic in general battles
	   ||  defAbility == ABILITY_SHEDSKIN
	   ||  defAbility == ABILITY_QUICKFEET
	   || (defAbility == ABILITY_SYNCHRONIZE && CanBeParalyzed(bankAtk, TRUE) && !GoodIdeaToParalyzeSelf(bankAtk))
	   || (defAbility == ABILITY_MARVELSCALE && PhysicalMoveInMoveset(bankAtk))
	   || (defAbility == ABILITY_NATURALCURE && CAN_SWITCH_OUT(bankDef))
	   || (defAbility == ABILITY_GUTS && RealPhysicalMoveInMoveset(bankDef))
	   || (defAbility == ABILITY_HYDRATION && gBattleWeather & WEATHER_RAIN_ANY && gWishFutureKnock.weatherDuration != 1 && WEATHER_HAS_EFFECT)
	   || (IS_DOUBLE_BATTLE && BATTLER_ALIVE(PARTNER(bankDef)) && ABILITY(PARTNER(bankDef)) == ABILITY_HEALER)
	   ||  MoveInMoveset(MOVE_FACADE, bankDef)
	   ||  MoveInMoveset(MOVE_PSYCHOSHIFT, bankDef)
	   ||  MoveInMoveset(MOVE_REST, bankDef);
}

bool8 GoodIdeaToParalyzeSelf(u8 bankAtk)
{
	u8 atkAbility = ABILITY(bankAtk);

	return CanBeParalyzed(bankAtk, FALSE)
		&&  (atkAbility == ABILITY_MARVELSCALE
		 ||  atkAbility == ABILITY_QUICKFEET
		 || (atkAbility == ABILITY_GUTS && RealPhysicalMoveInMoveset(bankAtk))
		 || MoveInMoveset(MOVE_FACADE, bankAtk)
		 || MoveInMoveset(MOVE_PSYCHOSHIFT, bankAtk));
}

bool8 BadIdeaToBurn(u8 bankDef, u8 bankAtk)
{
	u8 defItemEffect = ITEM_EFFECT(bankDef);
	u8 defAbility = ABILITY(bankDef);

	return !CanBeBurned(bankDef, TRUE)
		||  (gBattleTypeFlags & BATTLE_TYPE_FRONTIER && defItemEffect == ITEM_EFFECT_CURE_BRN) //Don't use this logic in general battles
		||  (gBattleTypeFlags & BATTLE_TYPE_FRONTIER && defItemEffect == ITEM_EFFECT_CURE_STATUS) //Don't use this logic in general battles
		||  defAbility == ABILITY_SHEDSKIN
		|| (defAbility == ABILITY_MAGICGUARD && !MoveInMoveset(MOVE_HEX, bankAtk))
		||  defAbility == ABILITY_QUICKFEET
		|| (defAbility == ABILITY_SYNCHRONIZE && CanBeBurned(bankAtk, TRUE) && !GoodIdeaToBurnSelf(bankAtk))
		|| (defAbility == ABILITY_MARVELSCALE && PhysicalMoveInMoveset(bankAtk))
		|| (defAbility == ABILITY_NATURALCURE && CAN_SWITCH_OUT(bankDef))
		|| (defAbility == ABILITY_FLAREBOOST && SpecialMoveInMoveset(bankDef))
		|| (defAbility == ABILITY_GUTS && RealPhysicalMoveInMoveset(bankDef))
		|| (defAbility == ABILITY_HYDRATION && gBattleWeather & WEATHER_RAIN_ANY && gWishFutureKnock.weatherDuration != 1 && WEATHER_HAS_EFFECT)
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
		 || (atkAbility == ABILITY_GUTS && RealPhysicalMoveInMoveset(bankAtk))
		 || MoveInMoveset(MOVE_FACADE, bankAtk)
		 || MoveInMoveset(MOVE_PSYCHOSHIFT, bankAtk));
}

bool8 BadIdeaToFreeze(u8 bankDef, u8 bankAtk)
{
	u8 defAbility = ABILITY(bankDef);
	u8 defItemEffect = ITEM_EFFECT(bankDef);

	return !CanBeFrozen(bankDef, TRUE)
		|| defItemEffect == ITEM_EFFECT_CURE_FRZ //Use this logic in general battles because Freezing is caused only by Flinging a one-time use item, so don't waste it
		|| defItemEffect == ITEM_EFFECT_CURE_STATUS
		|| (defAbility == ABILITY_SYNCHRONIZE && CanBeFrozen(bankAtk, TRUE))
		|| (defAbility == ABILITY_NATURALCURE && CAN_SWITCH_OUT(bankDef)) //Don't waste a one-time freeze
		|| UnfreezingMoveInMoveset(bankDef);
}

bool8 BadIdeaToMakeContactWith(u8 bankAtk, u8 bankDef)
{
	u8 atkAbility = ABILITY(bankAtk);
	bool8 badIdea = FALSE;

	switch (ABILITY(bankDef))
	{
		case ABILITY_EFFECTSPORE:
			badIdea = CanBePoisoned(bankAtk, bankDef, TRUE) || CanBeParalyzed(bankAtk, TRUE) || CanBePutToSleep(bankAtk, TRUE);
			break;
		case ABILITY_POISONPOINT:
			badIdea = CanBePoisoned(bankAtk, bankDef, TRUE);
			break;
		case ABILITY_STATIC:
			badIdea = CanBeParalyzed(bankAtk, TRUE);
			break;
		case ABILITY_FLAMEBODY:
			badIdea = CanBeBurned(bankAtk, TRUE);
			break;
		case ABILITY_CUTECHARM:
			badIdea = CanBeInfatuated(bankAtk, bankDef);
			break;
		case ABILITY_AFTERMATH:
			badIdea = !ABILITY_ON_FIELD(ABILITY_DAMP) && atkAbility != ABILITY_MAGICGUARD;
			break;
		#ifdef ABILITY_TANGLINGHAIR
		case ABILITY_TANGLINGHAIR:
		#endif
		case ABILITY_GOOEY:
			badIdea = STAT_CAN_FALL(gBankAttacker, STAT_SPD) && atkAbility != ABILITY_MIRRORARMOR;
			break;
		#ifdef ABILITY_IRONBARBS
		case ABILITY_IRONBARBS:
		#endif
		case ABILITY_ROUGHSKIN:
			badIdea = atkAbility != ABILITY_MAGICGUARD;
			break;
		case ABILITY_MUMMY:
		case ABILITY_WANDERINGSPIRIT:
		case ABILITY_COTTONDOWN:
		case ABILITY_PERISH_BODY:
			return TRUE;
	}

	if (ITEM_EFFECT(bankDef) == ITEM_EFFECT_ROCKY_HELMET && atkAbility != ABILITY_MAGICGUARD)
		badIdea = TRUE;

	if (!badIdea && HasContactProtectionMoveInMoveset(bankDef))
		badIdea = TRUE;

	return badIdea;
}

bool8 GoodIdeaToLowerAttack(u8 bankDef, u8 bankAtk, u16 move)
{
	if (!MoveWouldHitFirst(move, bankAtk, bankDef) && CanKnockOut(bankAtk, bankDef))
		return FALSE; //Don't bother lowering stats if can kill enemy.

	u8 defAbility = ABILITY(bankDef);

	return STAT_STAGE(bankDef, STAT_STAGE_ATK) > 4 && RealPhysicalMoveInMoveset(bankDef)
		&& defAbility != ABILITY_CONTRARY
		&& !IsClearBodyAbility(defAbility)
		&& defAbility != ABILITY_HYPERCUTTER
		&& defAbility != ABILITY_DEFIANT
		&& defAbility != ABILITY_COMPETITIVE;
}

bool8 GoodIdeaToLowerDefense(u8 bankDef, u8 bankAtk, u16 move)
{
	if (!MoveWouldHitFirst(move, bankAtk, bankDef) && CanKnockOut(bankAtk, bankDef))
		return FALSE; //Don't bother lowering stats if can kill enemy.

	u8 defAbility = ABILITY(bankDef);

	return STAT_STAGE(bankDef, STAT_STAGE_DEF) > 4
		&& PhysicalMoveInMoveset(bankAtk)
		&& defAbility != ABILITY_CONTRARY
		&& !IsClearBodyAbility(defAbility)
		&& defAbility != ABILITY_BIGPECKS
		&& defAbility != ABILITY_DEFIANT
		&& defAbility != ABILITY_COMPETITIVE;
}

bool8 GoodIdeaToLowerSpAtk(u8 bankDef, u8 bankAtk, u16 move)
{
	if (!MoveWouldHitFirst(move, bankAtk, bankDef) && CanKnockOut(bankAtk, bankDef))
		return FALSE; //Don't bother lowering stats if can kill enemy.

	u8 defAbility = ABILITY(bankDef);

	return STAT_STAGE(bankDef, STAT_STAGE_SPATK) > 4 && SpecialMoveInMoveset(bankDef)
		&& defAbility != ABILITY_CONTRARY
		&& !IsClearBodyAbility(defAbility)
		&& defAbility != ABILITY_DEFIANT
		&& defAbility != ABILITY_COMPETITIVE;
}

bool8 GoodIdeaToLowerSpDef(u8 bankDef, u8 bankAtk, u16 move)
{
	if (!MoveWouldHitFirst(move, bankAtk, bankDef) && CanKnockOut(bankAtk, bankDef))
		return FALSE; //Don't bother lowering stats if can kill enemy.

	u8 defAbility = ABILITY(bankDef);

	return STAT_STAGE(bankDef, STAT_STAGE_SPDEF) > 4 && SpecialMoveInMoveset(bankAtk)
		&& defAbility != ABILITY_CONTRARY
		&& !IsClearBodyAbility(defAbility)
		&& defAbility != ABILITY_DEFIANT
		&& defAbility != ABILITY_COMPETITIVE;
}

bool8 GoodIdeaToLowerSpeed(u8 bankDef, u8 bankAtk, u16 move, u8 reduceBy)
{
	if (!MoveWouldHitFirst(move, bankAtk, bankDef) && CanKnockOut(bankAtk, bankDef))
		return FALSE; //Don't bother lowering stats if can kill enemy.

	u8 defAbility = ABILITY(bankDef);

	return SpeedCalc(bankAtk) <= SpeedCalc(bankDef)
		&& defAbility != ABILITY_CONTRARY
		&& !IsClearBodyAbility(defAbility)
		&& (!IS_DOUBLE_BATTLE || WillBeFasterAfterSpeedDrop(bankAtk, bankDef, reduceBy));
}

bool8 GoodIdeaToLowerAccuracy(u8 bankDef, u8 bankAtk, u16 move)
{
	if (!MoveWouldHitFirst(move, bankAtk, bankDef) && CanKnockOut(bankAtk, bankDef))
		return FALSE; //Don't bother lowering stats if can kill enemy.

	u8 defAbility = ABILITY(bankDef);

	return defAbility != ABILITY_CONTRARY
		&& !IsClearBodyAbility(defAbility)
		&& defAbility != ABILITY_KEENEYE;
}

bool8 GoodIdeaToLowerEvasion(u8 bankDef, u8 bankAtk, unusedArg u16 move)
{
	u8 defAbility = ABILITY(bankDef);

	return (STAT_STAGE(bankDef, STAT_STAGE_EVASION) > 6 || MoveInMovesetWithAccuracyLessThan(bankAtk, bankDef, 90, TRUE))
		&& !IsClearBodyAbility(defAbility)
		&& defAbility != ABILITY_CONTRARY;
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
	if (IsPlayerInControl(bankAtk)
	&& !(gBattleTypeFlags & BATTLE_TYPE_FRONTIER) //Outside of the Frontier facing a known enemy
	&& HazingMoveInMoveset(bankAtk))
		return TRUE; //Assume the player knows the foe has a setup move and will try to cheese them with Hazing

	u16 move = IsValidMovePrediction(bankAtk, bankDef);

	if (move != MOVE_NONE)
	{
		u8 effect = gBattleMoves[move].effect;

		if (effect == EFFECT_ROAR)
			return !IsDynamaxed(bankDef); //Dynamax Pokemon can't be phazed out

		return effect == EFFECT_HAZE || effect == EFFECT_REMOVE_TARGET_STAT_CHANGES; //Check hazing move
	}

	return FALSE;
}

bool8 IsMovePredictionHPDrainingMove(u8 bankAtk, u8 bankDef)
{
	u16 move = IsValidMovePrediction(bankAtk, bankDef);

	if (move != MOVE_NONE)
	{
		u8 effect = gBattleMoves[move].effect;
		return effect == EFFECT_ABSORB || effect == EFFECT_DREAM_EATER;
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

bool8 MoveInMovesetAndUsable(u16 move, u8 bank)
{
	u8 movePos = FindMovePositionInMoveset(move, bank);
	if (movePos >= MAX_MON_MOVES)
		return FALSE;

	return !(gBitTable[movePos] & CheckMoveLimitations(bank, 0, 0xFF));
}

bool8 DamagingMoveInMoveset(u8 bank)
{
	u16 move;
	u8 moveLimitations = CheckMoveLimitations(bank, 0, 0xFF);

	for (u32 i = 0; i < MAX_MON_MOVES; ++i)
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

	for (u32 i = 0; i < MAX_MON_MOVES; ++i)
	{
		move = GetBattleMonMove(bank, i);
		if (move == MOVE_NONE)
			break;

		if (!(gBitTable[i] & moveLimitations))
		{
			if (CalcMoveSplit(bank, move) == SPLIT_PHYSICAL
			&& gBattleMoves[move].power != 0
			&& gBattleMoves[move].effect != EFFECT_COUNTER
			&& move != MOVE_FAKEOUT) //While physical, it can only be used on the first turn
				return TRUE;
		}
	}

	return FALSE;
}

//Doesn't factor in Foul Play, or Body Press
bool8 RealPhysicalMoveInMoveset(u8 bank)
{
	u16 move;
	u8 moveLimitations = CheckMoveLimitations(bank, 0, 0xFF);

	for (u32 i = 0; i < MAX_MON_MOVES; ++i)
	{
		move = GetBattleMonMove(bank, i);
		if (move == MOVE_NONE)
			break;

		if (!(gBitTable[i] & moveLimitations))
		{
			if (CalcMoveSplit(bank, move) == SPLIT_PHYSICAL
			&& gBattleMoves[move].power != 0
			&& gBattleMoves[move].effect != EFFECT_COUNTER
			&& move != MOVE_FAKEOUT
			&& move != MOVE_FOULPLAY
			&& move != MOVE_BODYPRESS)
				return TRUE;
		}
	}

	return FALSE;
}


bool8 SpecialMoveInMoveset(u8 bank)
{
	u16 move;
	u8 moveLimitations = CheckMoveLimitations(bank, 0, 0xFF);

	for (u32 i = 0; i < MAX_MON_MOVES; ++i)
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
	for (u32 i = 0; i < MAX_MON_MOVES; ++i)
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
	for (u32 i = 0; i < MAX_MON_MOVES; ++i)
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

bool8 PriorityMoveInMoveset(u8 bank)
{
	u16 move;
	u8 moveLimitations = CheckMoveLimitations(bank, 0, 0xFF);

	for (u32 i = 0; i < MAX_MON_MOVES; ++i)
	{
		move = GetBattleMonMove(bank, i);
		if (move == MOVE_NONE)
			break;

		if (!(gBitTable[i] & moveLimitations))
		{
			if (PriorityCalc(bank, ACTION_USE_MOVE, move) > 0)
				return TRUE;
		}
	}

	return FALSE;	
}

bool8 DamagingPriorityMoveInMovesetThatAffects(u8 bankAtk, u8 bankDef)
{
	u16 move;
	u8 moveLimitations = CheckMoveLimitations(bankAtk, 0, 0xFF);

	for (u32 i = 0; i < MAX_MON_MOVES; ++i)
	{
		move = GetBattleMonMove(bankAtk, i);
		if (move == MOVE_NONE)
			break;

		if (!(gBitTable[i] & moveLimitations))
		{
			if (SPLIT(move) != SPLIT_STATUS
			&& PriorityCalc(bankAtk, ACTION_USE_MOVE, move) > 0
			&& !(AI_SpecialTypeCalc(move, bankAtk, bankDef) & MOVE_RESULT_NO_EFFECT) //Move affects
			&& !IsDamagingMoveUnusable(move, bankAtk, bankDef)) //Move is usable
				return TRUE;
		}
	}

	return FALSE;	
}

bool8 MagicCoatableMovesInMoveset(u8 bank)
{
	u16 move;
	u8 moveLimitations = CheckMoveLimitations(bank, 0, 0xFF);

	for (u32 i = 0; i < MAX_MON_MOVES; ++i)
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

	for (u32 i = 0; i < MAX_MON_MOVES; ++i)
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
					case MOVE_BANEFULBUNKER:
					case MOVE_OBSTRUCT:
						if (checkType & CHECK_REGULAR_PROTECTION)
							return TRUE;
						break;

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

bool8 HasContactProtectionMoveInMoveset(u8 bank)
{
	u8 moveLimitations = CheckMoveLimitations(bank, 0, 0xFF);

	for (u32 i = 0; i < MAX_MON_MOVES; ++i)
	{
		u16 move = GetBattleMonMove(bank, i);
		if (move == MOVE_NONE)
			break;

		if (!(gBitTable[i] & moveLimitations))
		{
			switch (move)
			{
				case MOVE_SPIKYSHIELD:
				case MOVE_KINGSSHIELD:
				case MOVE_BANEFULBUNKER:
				case MOVE_OBSTRUCT:
					return TRUE;
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

	for (u32 i = 0; i < MAX_MON_MOVES; ++i)
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

	for (u32 i = 0; i < MAX_MON_MOVES; ++i)
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

	for (u32 i = 0; i < MAX_MON_MOVES; ++i)
	{
		move = GetBattleMonMove(bank, i);
		if (move == MOVE_NONE)
			break;

		if (!(gBitTable[i] & moveLimitations))
		{
			if (GetMoveTypeSpecial(bank, move) == moveType
			&&  SPLIT(move) != SPLIT_STATUS
			&&  GetBaseMoveTarget(move, bank) & MOVE_TARGET_ALL)
				return TRUE;
		}
	}

	return FALSE;
}

bool8 DamagingSpreadMoveInMoveset(u8 bank)
{
	u16 move;
	u8 moveLimitations = CheckMoveLimitations(bank, 0, 0xFF);

	for (u32 i = 0; i < MAX_MON_MOVES; ++i)
	{
		move = GetBattleMonMove(bank, i);
		if (move == MOVE_NONE)
			break;

		if (!(gBitTable[i] & moveLimitations))
		{
			if (SPLIT(move) != SPLIT_STATUS
			&&  GetBaseMoveTarget(move, bank) & MOVE_TARGET_SPREAD)
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

	for (u32 i = 0; i < MAX_MON_MOVES; ++i)
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

	for (u32 i = 0; i < MAX_MON_MOVES; ++i)
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

	for (u32 i = 0; i < MAX_MON_MOVES; ++i)
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

	for (u32 i = 0; i < MAX_MON_MOVES; ++i)
	{
		move = GetBattleMonMove(bankAtk, i);
		if (move == MOVE_NONE)
			break;

		if (!(gBitTable[i] & moveLimitations))
		{
			if (ignoreStatusMoves && SPLIT(move) == SPLIT_STATUS)
				continue;

			if (gBattleMoves[move].accuracy == 0 //Always hits
			||  GetBaseMoveTarget(move, bankAtk) & (MOVE_TARGET_USER | MOVE_TARGET_OPPONENTS_FIELD))
				continue;

			if (AccuracyCalc(move, bankAtk, bankDef) < acc)
				return TRUE;
		}
	}

	return FALSE;
}

bool8 AllMovesInMovesetWithAccuracyLessThan(u8 bankAtk, u8 bankDef, u8 acc, bool8 ignoreStatusMoves)
{
	u16 move;
	u8 moveLimitations = CheckMoveLimitations(bankAtk, 0, 0xFF);

	for (u32 i = 0; i < MAX_MON_MOVES; ++i)
	{
		move = GetBattleMonMove(bankAtk, i);
		if (move == MOVE_NONE)
			break;

		if (!(gBitTable[i] & moveLimitations))
		{
			if (ignoreStatusMoves && SPLIT(move) == SPLIT_STATUS)
				continue;

			if (gBattleMoves[move].accuracy == 0 //Always hits
			||  GetBaseMoveTarget(move, bankAtk) & (MOVE_TARGET_USER | MOVE_TARGET_OPPONENTS_FIELD))
				return FALSE; //At least one move hits

			if (AccuracyCalc(move, bankAtk, bankDef) >= acc)
				return FALSE; //At least one move hits
		}
	}

	return TRUE; //No moves hit
}

bool8 FlinchingMoveInMoveset(u8 bank)
{
	u16 move;
	u8 moveLimitations = CheckMoveLimitations(bank, 0, 0xFF);

	for (u32 i = 0; i < MAX_MON_MOVES; ++i)
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

	for (u32 i = 0; i < MAX_MON_MOVES; ++i)
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

	for (u32 i = 0; i < MAX_MON_MOVES; ++i)
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

	for (u32 i = 0; i < MAX_MON_MOVES; ++i)
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

	for (u32 i = 0; i < MAX_MON_MOVES; ++i)
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

	for (u32 i = 0; i < MAX_MON_MOVES; ++i)
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

	for (u32 i = 0; i < MAX_MON_MOVES; ++i)
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

	for (u32 i = 0; i < MAX_MON_MOVES; ++i)
	{
		u16 move = GetBattleMonMove(bank, i);

		if (move == MOVE_NONE)
			break;

		if (!(gBitTable[i] & moveLimitations))
		{
			if (gSpecialMoveFlags[move].gMovesCanUnfreezeAttacker)
				return TRUE;
		}
	}

	return FALSE;
}

bool8 SleepMoveInMovesetWithLowAccuracy(u8 bankAtk, u8 bankDef)
{
	u8 moveLimitations = CheckMoveLimitations(bankAtk, 0, 0xFF);

	for (u32 i = 0; i < MAX_MON_MOVES; ++i)
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

bool8 PivotingMoveInMovesetThatAffects(u8 bankAtk, u8 bankDef)
{
	u8 moveLimitations = CheckMoveLimitations(bankAtk, 0, 0xFF);

	for (u32 i = 0; i < MAX_MON_MOVES; ++i)
	{
		u16 move = GetBattleMonMove(bankAtk, i);

		if (move == MOVE_NONE)
			break;

		if (!(gBitTable[i] & moveLimitations))
		{
			if (move != MOVE_BATONPASS //Passing stats isn't considering pivoting
			&& (gBattleMoves[move].effect == EFFECT_BATON_PASS
			 || gBattleMoves[move].effect == EFFECT_TELEPORT))
			{
				if (SPLIT(move) != SPLIT_STATUS
				&& (AI_SpecialTypeCalc(move, bankAtk, bankDef) & MOVE_RESULT_NO_EFFECT
				 || IsDamagingMoveUnusable(move, bankAtk, bankDef)))
					continue; //A move like Volt Switch doesn't affect

				return TRUE;
			}
		}
	}

	return FALSE;
}

bool8 FastPivotingMoveInMovesetThatAffects(u8 bankAtk, u8 bankDef)
{
	u8 moveLimitations = CheckMoveLimitations(bankAtk, 0, 0xFF);

	for (u32 i = 0; i < MAX_MON_MOVES; ++i)
	{
		u16 move = GetBattleMonMove(bankAtk, i);

		if (move == MOVE_NONE)
			break;

		if (!(gBitTable[i] & moveLimitations))
		{
			if (move != MOVE_BATONPASS //Passing stats isn't considering pivoting
			&& gBattleMoves[move].effect == EFFECT_BATON_PASS)
			{
				if (SPLIT(move) != SPLIT_STATUS
				&& (AI_SpecialTypeCalc(move, bankAtk, bankDef) & MOVE_RESULT_NO_EFFECT
				 || IsDamagingMoveUnusable(move, bankAtk, bankDef)))
					continue; //A move like Volt Switch doesn't affect

				if (MoveWouldHitFirst(move, bankAtk, bankDef))
					return TRUE;
			}
		}
	}

	return FALSE;
}

bool8 OHKOMoveInMovesetThatAffects(u8 bankAtk, u8 bankDef, struct AIScript* aiScriptData)
{
	if (IsDynamaxed(bankDef))
		return FALSE;

	u8 moveLimitations = CheckMoveLimitations(bankAtk, 0, 0xFF);

	for (u32 i = 0; i < MAX_MON_MOVES; ++i)
	{
		u16 move = GetBattleMonMove(bankAtk, i);

		if (move == MOVE_NONE)
			break;

		if (!(gBitTable[i] & moveLimitations)
		&& gBattleMoves[move].effect == EFFECT_0HKO)
		{
			if (AIScript_Negatives(bankAtk, bankDef, move, 100, aiScriptData) >= 100) //Not bad idea to use
				return TRUE;
		}
	}

	return FALSE;
}

bool8 OffensiveSetupMoveInMoveset(u8 bankAtk, u8 bankDef)
{
	u8 moveLimitations = CheckMoveLimitations(bankAtk, 0, 0xFF);

	for (u32 i = 0; i < MAX_MON_MOVES; ++i)
	{
		u16 move = GetBattleMonMove(bankAtk, i);

		if (move == MOVE_NONE)
			break;

		if (!(gBitTable[i] & moveLimitations))
		{
			switch (gBattleMoves[move].effect)
			{
				case EFFECT_ATTACK_UP:
				case EFFECT_SPECIAL_ATTACK_UP:
				case EFFECT_EVASION_UP:
				case EFFECT_ATTACK_UP_2:
				case EFFECT_SPEED_UP_2:
				case EFFECT_SPECIAL_ATTACK_UP_2:
				case EFFECT_EVASION_UP_2:
				case EFFECT_ATK_SPATK_UP:
				case EFFECT_BULK_UP:
				case EFFECT_CALM_MIND:
				case EFFECT_DRAGON_DANCE:
				case EFFECT_BELLY_DRUM:
					return TRUE;

				case EFFECT_ATTACK_UP_HIT:
				case EFFECT_SPECIAL_ATTACK_UP_HIT:
				case EFFECT_SPECIAL_DEFENSE_DOWN_HIT:
				case EFFECT_DEFENSE_DOWN_HIT:
					if (CalcSecondaryEffectChance(bankAtk, move, ABILITY(bankAtk)) >= 50)
						return TRUE;

				CHECK_AFFECTS:
					if (!(AI_SpecialTypeCalc(move, bankAtk, bankDef) & MOVE_RESULT_NO_EFFECT) //Move affects
					&& IsDamagingMoveUnusable(move, bankAtk, bankDef)) //And can be used
						return TRUE;
					break;

				case EFFECT_SUPERPOWER:
				case EFFECT_OVERHEAT:
					if (ABILITY(bankAtk) == ABILITY_CONTRARY)
						goto CHECK_AFFECTS;
					break;
			}
		}
	}

	return FALSE;
}

bool8 HazingMoveInMoveset(u8 bank)
{
	u8 moveLimitations = CheckMoveLimitations(bank, 0, 0xFF);

	for (u32 i = 0; i < MAX_MON_MOVES; ++i)
	{
		u16 move = GetBattleMonMove(bank, i);

		if (move == MOVE_NONE)
			break;

		if (!(gBitTable[i] & moveLimitations))
		{
			if (gBattleMoves[move].effect == EFFECT_HAZE
			|| gBattleMoves[move].effect == EFFECT_REMOVE_TARGET_STAT_CHANGES)
				return TRUE;
		}
	}

	return FALSE;
}

bool8 HasUsedMove(u8 bank, u16 move)
{
    u32 i;

    for (i = 0; i < MAX_MON_MOVES; ++i)
	{
		if (BATTLE_HISTORY->usedMoves[bank][i] == MOVE_NONE)
			break; //Speed optimization since no blank move slots are left

		if (BATTLE_HISTORY->usedMoves[bank][i] == move)
			return TRUE;
	}

	return FALSE;
}

bool8 HasUsedMoveWithEffect(u8 bank, u8 effect)
{
    u32 i;

    for (i = 0; i < MAX_MON_MOVES; ++i)
	{
		if (BATTLE_HISTORY->usedMoves[bank][i] == MOVE_NONE)
			break; //Speed optimization since no blank move slots are left

		if (gBattleMoves[BATTLE_HISTORY->usedMoves[bank][i]].effect == effect)
			return TRUE;
	}

	return FALSE;
}

bool8 HasUsedMoveWithEffectHigherThanChance(u8 bank, u8 effect, u8 chance)
{
    u32 i;

    for (i = 0; i < MAX_MON_MOVES; ++i)
	{
		if (BATTLE_HISTORY->usedMoves[bank][i] == MOVE_NONE)
			break; //Speed optimization since no blank move slots are left

		if (gBattleMoves[BATTLE_HISTORY->usedMoves[bank][i]].effect == effect
		&& CalcSecondaryEffectChance(bank, BATTLE_HISTORY->usedMoves[bank][i], ABILITY(bank)) >= chance)
			return TRUE;
	}

	return FALSE;
}

bool8 HasUsedPhazingMoveThatAffects(u8 bankAtk, u8 bankDef)
{
    u32 i;

    for (i = 0; i < MAX_MON_MOVES; ++i)
	{
		u16 move = BATTLE_HISTORY->usedMoves[bankAtk][i];

		if (move == MOVE_NONE)
			break; //No more moves after this

		if (!MoveInMovesetAndUsable(move, bankAtk))
			continue;

		u8 effect = gBattleMoves[move].effect;

		if (effect == EFFECT_ROAR
		&& !IsDynamaxed(bankDef)
		&& !IsDamagingMoveUnusable(move, bankAtk, bankDef)) //Contains just Soundproof check for Roar
			return TRUE;

		if (effect == EFFECT_HAZE || move == MOVE_TOPSYTURVY)
			return TRUE;

		if (effect == EFFECT_REMOVE_TARGET_STAT_CHANGES
		&& !(AI_SpecialTypeCalc(move, bankAtk, bankDef) & MOVE_RESULT_NO_EFFECT) //Move affects
		&& !IsDamagingMoveUnusable(move, bankAtk, bankDef)) //Move is usable
			return TRUE;
	}

	return FALSE;
}

bool8 NoUsableHazardsInMoveset(u8 bankAtk, u8 bankDef, struct AIScript* aiScriptData)
{
    u32 i;

    for (i = 0; i < MAX_MON_MOVES; ++i)
	{
		u16 move = BATTLE_HISTORY->usedMoves[bankAtk][i];

		if (move == MOVE_NONE)
			break; //No more moves after this

		if (!MoveInMovesetAndUsable(move, bankAtk))
			continue;

		u8 effect = gBattleMoves[move].effect;

		if (effect == EFFECT_SPIKES
		&& AIScript_Negatives(bankAtk, bankDef, move, 100, aiScriptData) >= 100)
			return FALSE; //Some hazard move can still be used
	}

	return FALSE;
}

static bool8 ShouldAIFreeChoiceLockWithDynamax(u8 bankAtk, u8 bankDef)
{
	if (CanDynamax(bankAtk)
	&& CanBeChoiceLocked(bankAtk)
	&& CHOICED_MOVE(bankAtk) != MOVE_NONE && CHOICED_MOVE(bankAtk) != 0xFFFF) //AI is locked into some move
	{
		u8 moveLimitations = CheckMoveLimitations(bankAtk, 0, MOVE_LIMITATION_ZEROMOVE | MOVE_LIMITATION_PP);
		struct AIScript aiScriptData = {0};
		PopulateAIScriptStructWithBaseAttackerData(&aiScriptData, bankAtk);
		PopulateAIScriptStructWithBaseDefenderData(&aiScriptData, bankDef);
		for (u32 i = 0; i < MAX_MON_MOVES; ++i)
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

				if (AI_SpecialTypeCalc(move, bankAtk, bankDef) & MOVE_RESULT_SUPER_EFFECTIVE)
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
		else if (!PivotingMoveInMovesetThatAffects(bankAtk, bankDef) //U-Turn/Volt Switch switch on their own
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
				u8 switchFlags = GetMostSuitableMonToSwitchIntoFlags();
				if (switchFlags & SWITCHING_FLAG_HAS_SUPER_EFFECTIVE_MOVE)
				{
					gActiveBattler = backupActiveBattler;
					if (hasSuperEffectiveMove)
						return FALSE; //Don't switch to another Pokemon just because they have a Super-Effective move if you have one too.

					if ((switchFlags & SWITCHING_FLAG_HAS_SUPER_EFFECTIVE_MOVE) == SWITCHING_FLAG_HAS_SUPER_EFFECTIVE_MOVE //Only a single flag set
					&& WallsFoe(bankDef, bankAtk))
						return FALSE; //Tough it out
				}
				else if (switchFlags == 0)
				{
					gActiveBattler = backupActiveBattler;
					return FALSE;
				}
				else if (switchFlags & SWITCHING_FLAG_WALLS_FOE)
				{
					gActiveBattler = backupActiveBattler;
					//Check if current mon can't wall
					bool8 canWall = WallsFoe(bankDef, bankAtk);

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

	for (u32 i = firstId; i < lastId; ++i)
	{
		if (party[i].species == SPECIES_NONE
		|| GetMonData(&party[i], MON_DATA_IS_EGG, NULL)
		|| i == gBattlerPartyIndexes[bank])
			continue;

		if (party[i].hp > 0 //Alive
		&& party[i].hp < party[i].maxHP) //Not max HP
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

static u8 CountUsefulStatChanges(u8 bank, bool8 debuff)
{
	u8 buffs = 0;
	bool8 storedPowerInMoveset = !debuff && (MoveInMovesetAndUsable(MOVE_STOREDPOWER, bank) || MoveInMovesetAndUsable(MOVE_POWERTRIP, bank));

	for (u8 statId = STAT_STAGE_ATK; statId < BATTLE_STATS_NO; ++statId)
	{
		s8 statVal = STAT_STAGE(bank, statId) - 6;
		bool8 cond = (debuff) ? statVal < 0 : statVal > 0;

		if (cond)
		{
			bool8 add = FALSE;

			if (storedPowerInMoveset)
				add = TRUE;
			else
			{
				switch (statId)
				{
					case STAT_STAGE_ATK:
						if (RealPhysicalMoveInMoveset(bank))
							add = TRUE;
						break;
					case STAT_STAGE_DEF:
						if (MoveSplitOnTeam(FOE(bank), SPLIT_PHYSICAL)
						|| MoveInMoveset(MOVE_BODYPRESS, bank))
							add = TRUE;
						break;
					case STAT_STAGE_SPATK:
						if (SpecialMoveInMoveset(bank))
							add = TRUE;
						break;
					case STAT_STAGE_SPDEF:
						if (MoveSplitOnTeam(FOE(bank), SPLIT_SPECIAL))
							add = TRUE;
						break;
					case STAT_STAGE_SPEED:
					case STAT_STAGE_ACC:
					case STAT_STAGE_EVASION:
						add = TRUE;
				}
			}

			if (add)
			{
				if (debuff)
					buffs += (statVal * -1);
				else
					buffs += statVal;
			}
		}
	}

	return buffs;
}

u8 CountUsefulBoosts(u8 bank)
{
	return CountUsefulStatChanges(bank, FALSE);
}

u8 CountUsefulDebuffs(u8 bank)
{
	return CountUsefulStatChanges(bank, TRUE);
}

bool8 AnyUsefulStatIsRaised(u8 bank)
{
	if (CountUsefulBoosts(bank) > 0)
		return TRUE;

	if (gBattleResources->flags->flags[bank] & 1 //Flash Fire activated
	&&  DamagingMoveTypeInMoveset(bank, TYPE_FIRE))
		return TRUE;

	if (gNewBS->UnburdenBoosts & gBitTable[bank] && ITEM(bank) == ITEM_NONE)
		return TRUE;

	return FALSE;
}

bool8 AnyUsefulOffseniveStatIsRaised(u8 bank)
{
	if (STAT_STAGE(bank, STAT_STAGE_ATK) > 6
	&& RealPhysicalMoveInMoveset(bank))
		return TRUE;

	if (STAT_STAGE(bank, STAT_STAGE_DEF) > 6
	&& MoveInMoveset(MOVE_BODYPRESS, bank))
		return TRUE;

	if (STAT_STAGE(bank, STAT_STAGE_SPATK) > 6
	&& SpecialMoveInMoveset(bank))
		return TRUE;

	if (STAT_STAGE(bank, STAT_STAGE_SPEED) > 6)
		return TRUE;

	if (gBattleResources->flags->flags[bank] & 1 //Flash Fire activated
	&&  DamagingMoveTypeInMoveset(bank, TYPE_FIRE))
		return TRUE;

	return FALSE;
}

bool8 PartyMemberStatused(u8 bank, bool8 checkSoundproof)
{
	struct Pokemon* party;
	if (SIDE(bank) == B_SIDE_PLAYER)
		party = gPlayerParty;
	else
		party = gEnemyParty;

	for (u32 i = 0; i < PARTY_SIZE; ++i)
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
	u8 movePos = FindMovePositionInMoveset(move, bankAtk);
	if (movePos >= MAX_MON_MOVES)
		return CalcShouldAIUseZMove(bankAtk, bankDef, move); //Move not in moveset requires calc

	return ShouldAIUseZMoveByMoveAndMovePos(bankAtk, bankDef, move, movePos);
}

bool8 ShouldAIUseZMoveByMoveAndMovePos(u8 bankAtk, u8 bankDef, u16 move, u8 movePos)
{
	if (gNewBS->ai.shouldUseZMove[bankAtk][bankDef][movePos] != 0xFF)
		return gNewBS->ai.shouldUseZMove[bankAtk][bankDef][movePos];

	return gNewBS->ai.shouldUseZMove[bankAtk][bankDef][movePos] = CalcShouldAIUseZMove(bankAtk, bankDef, move);
}

void ClearShouldAIUseZMoveByMoveAndMovePos(u8 bankAtk, u8 bankDef, u8 movePos)
{
	gNewBS->ai.shouldUseZMove[bankAtk][bankDef][movePos] = 0xFF;
}

static bool8 CalcShouldAIUseZMove(u8 bankAtk, u8 bankDef, u16 move)
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
			u8 atkAbility = ABILITY(bankAtk);
			u8 defAbility = ABILITY(bankDef);
			u16 defSpecies = SPECIES(bankDef);
			bool8 noMoldBreakers = NO_MOLD_BREAKERS(atkAbility, zMove);

			if (move == MOVE_FAKEOUT && ShouldUseFakeOut(bankAtk, bankDef))
				return FALSE; //Prefer actual Fake Out over Breakneck Blitz

			if (MoveBlockedBySubstitute(zMove, bankAtk, bankDef)
			|| (defMovePrediction == MOVE_SUBSTITUTE
			 && !MoveWouldHitFirst(zMove, bankAtk, bankDef)
			 && !MoveIgnoresSubstitutes(zMove, atkAbility)))
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
				if (GetNumRaidShieldsUp() <= 2 //Less than 3 shields left
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

			if (gBattleMoves[move].effect != EFFECT_OVERHEAT //Base move won't lower user stats
			&& gBattleMoves[move].effect != EFFECT_SUPERPOWER
			&& (atkAbility == ABILITY_MAGICGUARD || !CheckRecoil(move)) //Base move won't do recoil
			&& MoveKnocksOutXHits(move, bankAtk, bankDef, 1) //Base move can KO
			&& AccuracyCalc(move, bankAtk, bankDef) >= 90 //And the move is likely to hit
			&& ViableMonCountFromBank(bankDef) >= 2) //And the foe has another Pokemon left
				return FALSE; //If the base move can KO, don't turn it into a Z-Move

			if (IsTypeZCrystal(ITEM(bankAtk), TYPE_NORMAL) //Normalium Z
			&& MoveEffectInMoveset(EFFECT_FAIRY_LOCK_HAPPY_HOUR, bankAtk)) //Happy Hour/Celebrate/Hold Hands
				return FALSE; //Don't waste the Z-Move on a damaging move and instead save it for +1 in all stats

			if (move == MOVE_SPECTRALTHIEF
			&& AnyUsefulStatIsRaised(bankDef))
				return FALSE; //Steal stats instead

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
						if (i == STAT_STAGE_ATK && !RealPhysicalMoveInMoveset(bankAtk)) //Only reset lowered Attack if useful
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
					if (AI_STAT_CAN_RISE(bankAtk, STAT_STAGE_ATK)
					&& RealPhysicalMoveInMoveset(bankAtk))
						return TRUE;
					break;
				case Z_EFFECT_DEF_UP_1:
				case Z_EFFECT_DEF_UP_2:
				case Z_EFFECT_DEF_UP_3:
					if (AI_STAT_CAN_RISE(bankAtk, STAT_STAGE_DEF))
						return TRUE;
					break;
				case Z_EFFECT_SPATK_UP_1:
				case Z_EFFECT_SPATK_UP_2:
				case Z_EFFECT_SPATK_UP_3:
					if (AI_STAT_CAN_RISE(bankAtk, STAT_STAGE_SPATK)
					&& SpecialMoveInMoveset(bankAtk))
						return TRUE;
					break;
				case Z_EFFECT_SPDEF_UP_1:
				case Z_EFFECT_SPDEF_UP_2:
				case Z_EFFECT_SPDEF_UP_3:
					if (AI_STAT_CAN_RISE(bankAtk, STAT_STAGE_SPDEF))
						return TRUE;
					break;
				case Z_EFFECT_SPD_UP_1:
				case Z_EFFECT_SPD_UP_2:
				case Z_EFFECT_SPD_UP_3:
					if (AI_STAT_CAN_RISE(bankAtk, STAT_STAGE_SPEED))
						return TRUE;
					break;
				case Z_EFFECT_ACC_UP_1:
				case Z_EFFECT_ACC_UP_2:
				case Z_EFFECT_ACC_UP_3:
					if (AI_STAT_CAN_RISE(bankAtk, STAT_STAGE_ACC)
					&&  MoveInMovesetWithAccuracyLessThan(bankAtk, bankDef, 90, FALSE))
						return TRUE;
					break;
				case Z_EFFECT_EVSN_UP_1:
				case Z_EFFECT_EVSN_UP_2:
				case Z_EFFECT_EVSN_UP_3:
					if (AI_STAT_CAN_RISE(bankAtk, STAT_STAGE_EVASION))
						return TRUE;
					break;
				case Z_EFFECT_RECOVER_HP:
					if (!BATTLER_MAX_HP(bankAtk) || IS_DOUBLE_BATTLE)
						return TRUE;
					else //At full HP currently
					{
						if (gBattleMoves[move].effect == EFFECT_BELLY_DRUM)
							return TRUE; //Always try to heal before using Belly Drum

						if (defMovePrediction != MOVE_NONE)
						{
							if (!MoveWouldHitFirst(move, bankAtk, bankDef)) //Enemy could strike and lower HP
								return TRUE;
						}

						//Otherwise assume will move first so don't waste the Z-Effect
					}
					break;
				default: //Recover Replacement HP
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
			else if (IsChoiceItemEffectOrAbility(itemEffect, ability))
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
				else if (IsRaidBattle()
				&& gBattleResults.battleTurnCounter <= 3 //First three turns
				&& !PlayerHasNoMonsLeftThatCanDynamax()) //Player still can Dynamax one of their mons
					return; //Give the Player a chance to Dynamax first in a Raid battle
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

				if (CanBeChoiceLocked(bankAtk)
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
