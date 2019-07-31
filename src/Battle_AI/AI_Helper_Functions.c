#include "../defines.h"
#include "../../include/random.h"
#include "../../include/constants/items.h"

#include "../../include/new/accuracy_calc.h"
#include "../../include/new/AI_advanced.h"
#include "../../include/new/AI_Helper_Functions.h"
#include "../../include/new/AI_scripts.h"
#include "../../include/new/battle_start_turn_start.h"
#include "../../include/new/damage_calc.h"
#include "../../include/new/end_turn.h"
#include "../../include/new/general_bs_commands.h"
#include "../../include/new/Helper_Functions.h"
#include "../../include/new/item.h"
#include "../../include/new/mega.h"
#include "../../include/new/move_tables.h"
#include "../../include/new/set_z_effect.h"

//This file's functions:
static u32 CalcPredictedDamageForCounterMoves(u16 move, u8 bankAtk, u8 bankDef);

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
	int i;
	u16 move;

	if (gAbsentBattlerFlags & (gBitTable[bankAtk] | gBitTable[bankDef]))
		return FALSE;

	u8 moveLimitations = CheckMoveLimitations(bankAtk, 0, 0xFF);

	for (i = 0; i < MAX_MON_MOVES; ++i)
	{
		move = GetBattleMonMove(bankAtk, i);

		if (move == MOVE_NONE)
			break;

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
	int i;
	u16 move;

	if (gAbsentBattlerFlags & (gBitTable[bankAtk] | gBitTable[bankDef]))
		return FALSE;

	u8 moveLimitations = CheckMoveLimitations(bankAtk, 0, 0xFF);

	for (i = 0; i < MAX_MON_MOVES; ++i)
	{
		move = GetBattleMonMove(bankAtk, i);

		if (move == MOVE_NONE)
			break;

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
	
	//Temporarily increase target's hp for calcs
	u16 backupHp = gBattleMons[bankDef].hp;
	gBattleMons[bankDef].hp = MathMin(backupHp + healAmount, gBattleMons[bankDef].maxHP);

	if (gAbsentBattlerFlags & (gBitTable[bankAtk] | gBitTable[bankDef]))
		return FALSE;

	u8 moveLimitations = CheckMoveLimitations(bankAtk, 0, 0xFF);

	for (i = 0; i < MAX_MON_MOVES; ++i)
	{
		move = GetBattleMonMove(bankAtk, i);

		if (move == MOVE_NONE)
			break;

		if (!(gBitTable[i] & moveLimitations))
		{
			if (MoveKnocksOutXHits(move, bankAtk, bankDef, numHits))
			{
				gBattleMons[bankDef].hp = backupHp;
				return TRUE;
			}
		}
	}

	gBattleMons[bankDef].hp = backupHp;
	return FALSE;
}

bool8 CanKnockOutWithoutMove(const u16 ignoredMove, const u8 bankAtk, const u8 bankDef)
{
	u16 move;
	int i;

	if (gAbsentBattlerFlags & (gBitTable[bankAtk] | gBitTable[bankDef]))
		return FALSE;

	u8 moveLimitations = CheckMoveLimitations(bankAtk, 0, 0xFF);

	for (i = 0; i < MAX_MON_MOVES; ++i)
	{
		move = GetBattleMonMove(bankAtk, i);
		if (move == ignoredMove)
			continue;

		if (move == MOVE_NONE)
			break;

		if (!(gBitTable[i] & moveLimitations))
		{
			if (MoveKnocksOutXHits(move, bankAtk, bankDef, 1))
				return TRUE;
		}
	}

	return FALSE;
}

bool8 MoveKnocksOutGoesFirstWithBestAccuracy(u16 move, u8 bankAtk, u8 bankDef)
{
	u16 currMove, currAcc;
	
	u8 bestMoveIndex = 0xFF;
	u16 bestAcc = 0;
	u8 moveLimitations = CheckMoveLimitations(bankAtk, 0, 0xFF);

	for (int i = 0; i < MAX_MON_MOVES; ++i)
	{
		currMove = GetBattleMonMove(bankAtk, i);
		if (currMove == MOVE_NONE)
			break;
		
		currMove = TryReplaceMoveWithZMove(bankAtk, currMove);

		if (!(gBitTable[i] & moveLimitations))
		{
			currAcc = AccuracyCalc(currMove, bankAtk, bankDef);

			if (MoveWouldHitFirst(currMove, bankAtk, bankDef)
			&& MoveKnocksOutXHits(currMove, bankAtk, bankDef, 1)
			&& (MoveWillHit(currMove, bankAtk, bankDef) || currAcc > bestAcc))
			{
				bestAcc = currAcc;
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
	u8 moveLimitations = CheckMoveLimitations(bankAtk, 0, 0xFF);

	for (int i = 0; i < MAX_MON_MOVES; ++i)
	{
		currMove = GetBattleMonMove(bankAtk, i);

		if (currMove == MOVE_NONE)
			break;
		
		currMove = TryReplaceMoveWithZMove(bankAtk, currMove);

		if (!(gBitTable[i] & moveLimitations))
		{
			currDmg = CalcFinalAIMoveDamage(move, bankAtk, bankDef, 1);

			if (MoveWouldHitFirst(currMove, bankAtk, bankDef)
			&& (currDmg > bestDmg))
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

/*
	One Target/Two Targets							Three Targets
12: Hit 2 Foes, KO 2 Foes
11: Hit 2 Foes, KO 1 Foe, Strongest Move 1 Foe
10: Hit 2 Foes, KO 1 Foe/Strongest Move 2 Foes
9: Hit 2 Foes, Strongest Move 1 Foe					Hit 2 Foes, KO 2 Foes, Hurt Partner
8: Hit 2 Foes										Hit 2 Foes, KO 1 Foe, Strongest Move 1 Foe, Hurt Partner
6: Hit 1 Foe, KO 1 Foe								Hit 2 Foes, KO 1 Foe/Strongest Move 2 Foes, Hurt Partner
5: Hit 1 Foe, Strongest Move 1 Foe					Hit 2 Foes, Strongest Move 1 Foe, Hurt Partner
4: Hit 1 Foe										Hit 2 Foes, Hurt Partner						
*/

#define DOUBLES_INCREASE_HIT_FOE 4
#define DOUBLES_INCREASE_KO_FOE 2
#define DOUBLES_INCREASE_STRONGEST_MOVE 1

#define DOUBLES_DECREASE_HIT_PARTNER 3

u8 GetBestDoubleKillingMoveScore(u8 bankAtk, u8 bankDef, u8 bankAtkPartner, u8 bankDefPartner, u16* bestMove)
{
	u16 move;
	s8 moveScores[MAX_MON_MOVES] = {0};
	
	u8 moveLimitations = CheckMoveLimitations(bankAtk, 0, 0xFF);
	for (int i = 0; i < MAX_MON_MOVES; ++i)
	{
		move = GetBattleMonMove(bankAtk, i);
	
		if (move == MOVE_NONE)
			break;
			
		if (SPLIT(move) == SPLIT_STATUS)
			continue;
			
		move = TryReplaceMoveWithZMove(bankAtk, move);
		if (!(gBitTable[i] & moveLimitations))
		{
			if (BATTLER_ALIVE(bankDef))
			{
				if (!(AI_SpecialTypeCalc(move, bankAtk, bankDef) & MOVE_RESULT_NO_EFFECT))
				{
					moveScores[i] += DOUBLES_INCREASE_HIT_FOE; //Hit one enemy
			
					if (MoveKnocksOutXHits(move, bankAtk, bankDef, 1))
						moveScores[i] += DOUBLES_INCREASE_KO_FOE; //KO enemy
					else if (IsStrongestMove(move, bankAtk, bankDef, FALSE))
						moveScores[i] += DOUBLES_INCREASE_STRONGEST_MOVE;
				}
			}

			if (BATTLER_ALIVE(bankDefPartner))
			{
				if (gBattleMoves[move].target & (MOVE_TARGET_BOTH | MOVE_TARGET_ALL)
				&& !(AI_SpecialTypeCalc(move, bankAtk, bankDefPartner) & MOVE_RESULT_NO_EFFECT))
				{
					moveScores[i] += DOUBLES_INCREASE_HIT_FOE; //Hit second enemy
					
					if (MoveKnocksOutXHits(move, bankAtk, bankDefPartner, 1))
						moveScores[i] += DOUBLES_INCREASE_KO_FOE; //KO second enemy
					else if (IsStrongestMove(move, bankAtk, bankDefPartner, TRUE))
						moveScores[i] += DOUBLES_INCREASE_STRONGEST_MOVE;
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
					u16 partnerMove = gChosenMovesByBanks[bankAtkPartner];
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
							moveScores[i] -= DOUBLES_DECREASE_HIT_PARTNER; //Hitting partner is bad
					}
					else if (!HasProtectionMoveInMoveset(bankAtkPartner, CHECK_WIDE_GUARD | CHECK_MAT_BLOCK) //Ally can't protect against attack
					|| (gBattleMoves[gLastResultingMoves[bankAtkPartner]].effect == EFFECT_PROTECT && gDisableStructs[bankAtkPartner].protectUses >= 1)) //Don't rely on double protect
					{
						moveScores[i] -= DOUBLES_DECREASE_HIT_PARTNER; //Hitting partner is bad
					}
				}
				else
					moveScores[i] -= DOUBLES_DECREASE_HIT_PARTNER; //Hitting partner is bad
			}
		}
	}

	u8 bestIndex = 0;
	for (int i = 1; i < MAX_MON_MOVES; ++i)
	{
		if (moveScores[i] > moveScores[bestIndex])
			bestIndex = i;
		else if (moveScores[i] == moveScores[bestIndex])
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

	*bestMove = gBattleMons[bankAtk].moves[bestIndex];

	if (moveScores[bestIndex] >= 12)
		return 9;
	else if (moveScores[bestIndex] >= 9)
		return 2;
	else if (moveScores[bestIndex] >= 5)
		return 1;

	return 0;
}

u8 GetDoubleKillingScore(u16 move, u8 bankAtk, u8 bankDef)
{
	if (gBattleMoves[move].target & MOVE_TARGET_ALL
	&&  gChosenMovesByBanks[PARTNER(bankAtk)] != MOVE_NONE)
	{
		//Recalculate as partner has chosen a move
		gNewBS->bestDoublesKillingScores[bankAtk][bankDef] = GetBestDoubleKillingMoveScore(bankAtk, bankDef, PARTNER(bankAtk), PARTNER(bankDef), &gNewBS->bestDoublesKillingMoves[bankAtk][bankDef]);
	}
	
	if (move == gNewBS->bestDoublesKillingMoves[bankAtk][bankDef])
		return gNewBS->bestDoublesKillingScores[bankAtk][bankDef];
	
	return 0;
}

bool8 RangeMoveCanHurtPartner(u16 move, u8 bankAtk, u8 bankAtkPartner)
{
	return BATTLER_ALIVE(bankAtkPartner)
		&& ABILITY(bankAtkPartner) != ABILITY_TELEPATHY
		&& !(BATTLER_SEMI_INVULNERABLE(bankAtkPartner) && MoveWouldHitFirst(move, bankAtk, bankAtkPartner)) //Target will still be semi-invulnerable when attack hits
		&& !(AI_SpecialTypeCalc(move, bankAtk, bankAtkPartner) & MOVE_RESULT_NO_EFFECT); //Move has effect
}

bool8 MoveKnocksOutXHits(u16 move, u8 bankAtk, u8 bankDef, u8 numHits)
{
	if (CalcFinalAIMoveDamage(move, bankAtk, bankDef, numHits) >= gBattleMons[bankDef].hp)
		return TRUE;

	return FALSE;
}

u16 CalcFinalAIMoveDamage(u16 move, u8 bankAtk, u8 bankDef, u8 numHits)
{
	if (move == MOVE_NONE || SPLIT(move) == SPLIT_STATUS || gBattleMoves[move].power == 0)
		return 0;

	if (gBattleMoves[move].effect == EFFECT_FAKE_OUT && !gDisableStructs[bankAtk].isFirstTurn)
		return 0;

	if (gBattleMoves[move].effect == EFFECT_0HKO)
	{
		if (gBattleMons[bankAtk].level <= gBattleMons[bankDef].level)
			return 0;
		if (move == MOVE_SHEERCOLD && IsOfType(bankDef, TYPE_ICE))
			return 0;

		return gBattleMons[bankDef].hp;
	}

	if (gBattleMoves[move].effect == EFFECT_COUNTER || gBattleMoves[move].effect == EFFECT_MIRROR_COAT) //Includes Metal Burst
	{
		return CalcPredictedDamageForCounterMoves(move, bankAtk, bankDef);
	}

	return MathMin(AI_CalcDmg(bankAtk, bankDef, move) * numHits, gBattleMons[bankDef].maxHP);
}

static u32 CalcPredictedDamageForCounterMoves(u16 move, u8 bankAtk, u8 bankDef)
{
	u16 predictedMove = IsValidMovePrediction(bankDef, bankAtk);
	u32 predictedDamage = 0;

	if (predictedMove != MOVE_NONE)
	{
		predictedDamage = AI_CalcDmg(bankDef, bankAtk, predictedMove); //The damage the enemy will do to the AI

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

move_t GetStrongestMove(const u8 bankAtk, const u8 bankDef, const bool8 onlySpreadMoves)
{
	u16 move;
	u16 strongestMove = gBattleMons[bankAtk].moves[0];
	u32 highestDamage = 0;
	u32 predictedDamage;
	int i;

	u8 moveLimitations = CheckMoveLimitations(bankAtk, 0, 0xFF);

	for (i = 0; i < MAX_MON_MOVES; ++i)
	{
		move = gBattleMons[bankAtk].moves[i];
		if (move == MOVE_NONE)
			break;
		move = TryReplaceMoveWithZMove(bankAtk, move);	

		if (!(gBitTable[i] & moveLimitations))
		{
			if (SPLIT(move) == SPLIT_STATUS
			||  gBattleMoves[move].power == 0
			||  (onlySpreadMoves && !(gBattleMoves[move].target & (MOVE_TARGET_BOTH | MOVE_TARGET_ALL))))
				continue;

			if (gBattleMoves[move].effect == EFFECT_COUNTER || gBattleMoves[move].effect == EFFECT_MIRROR_COAT) //Includes Metal Burst
			{
				predictedDamage = CalcPredictedDamageForCounterMoves(move, bankAtk, bankDef);
				if (predictedDamage > (u32) highestDamage)
				{
					highestDamage = predictedDamage;
					strongestMove = move;
				}
			}
			else if (gBattleMoves[move].effect == EFFECT_0HKO)
			{
				if (gBattleMons[bankAtk].level <= gBattleMons[bankDef].level)
					continue;
				if (move == MOVE_SHEERCOLD && IsOfType(bankDef, TYPE_ICE))
					continue;
				if (ABILITY(bankDef) == ABILITY_STURDY)
					continue;
				if (MoveWillHit(move, bankAtk, bankDef))
				{
					return move; //No stronger move that OHKO move that can kill
				}
			}
			else
			{
				predictedDamage = AI_CalcDmg(bankAtk, bankDef, move);
				if (predictedDamage > highestDamage)
				{
					strongestMove = move;
					highestDamage = predictedDamage;
				}
			}
		}
	}
	
	return strongestMove;
}

bool8 IsStrongestMove(const u16 currentMove, const u8 bankAtk, const u8 bankDef, const bool8 onlySpreadMoves)
{
	if (onlySpreadMoves)
		return gNewBS->strongestSpreadMove[bankAtk][bankDef] == currentMove;
		
	return gNewBS->strongestMove[bankAtk][bankDef] == currentMove;
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

	if (((gStatuses3[bankDef] & (STATUS3_IN_AIR | STATUS3_SKY_DROP_ATTACKER | STATUS3_SKY_DROP_TARGET)) && !CheckTableForMove(move, IgnoreAirTable))
	||  ((gStatuses3[bankDef] & STATUS3_UNDERGROUND) && !CheckTableForMove(move, IgnoreUndergoundTable))
	||  ((gStatuses3[bankDef] & STATUS3_UNDERWATER) && !CheckTableForMove(move, IgnoreUnderwaterTable))
	||   (gStatuses3[bankDef] & STATUS3_DISAPPEARED))
		return FALSE;

	if ((move == MOVE_TOXIC && IsOfType(bankAtk, TYPE_POISON))
	||  (CheckTableForMove(move, MinimizeHitTable) && gStatuses3[bankDef] & STATUS3_MINIMIZED)
	|| ((gStatuses3[bankDef] & STATUS3_TELEKINESIS) && gBattleMoves[move].effect != EFFECT_0HKO)
	||  gBattleMoves[move].accuracy == 0
	|| (WEATHER_HAS_EFFECT && (gBattleWeather & WEATHER_RAIN_ANY) && CheckTableForMove(move, AlwaysHitRainTable))
	||  IsZMove(move))
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
	if (gNewBS->TrickRoomTimer)
	{
		temp = bankDefSpeed;
		bankDefSpeed = bankAtkSpeed;
		bankAtkSpeed = temp;
	}

	if (bankAtkSpeed > bankDefSpeed)
		return TRUE;

	return FALSE;
}

bool8 WillFaintFromWeatherSoon(u8 bank)
{
	if (TakesDamageFromSandstorm(bank) || TakesDamageFromHail(bank))
	{
		return gBattleMons[bank].hp <= gBattleMons[bank].maxHP / 16;
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

u16 GetBattleMonMove(u8 bank, u8 i)
{
	u16 move;

	if (i >= MAX_MON_MOVES)
		return MOVE_NONE;

	#ifdef REALLY_SMART_AI
		move = gBattleMons[bank].moves[i];
	#else
		if (SIDE(bank) == B_SIDE_PLAYER && !(gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER))
			move = BATTLE_HISTORY->usedMoves[bank].moves[i];
		else if (SIDE(bank) == B_SIDE_PLAYER && gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER && GetBattlerPosition(bank) == B_POSITION_PLAYER_LEFT)
			move = BATTLE_HISTORY->usedMoves[bank].moves[i];
		else
			move = gBattleMons[bank].moves[i];
	#endif

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
		|| gNewBS->FairyLockTimer != 0)
			return TRUE;
	}
	
	return FALSE;
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
		||  gWishFutureKnock->futureSightCounter[bank] == 1
		||  gStatuses3[bank] & STATUS3_LEECHSEED
		||  (gBattleMons[bank].status1 & STATUS1_PSN_ANY && ability != ABILITY_POISONHEAL)
		||  gBattleMons[bank].status1 & STATUS1_BURN
		||  ((gBattleMons[bank].status1 & STATUS1_SLEEP) > 1 && gBattleMons[bank].status2 & STATUS2_NIGHTMARE)
		||  gBattleMons[bank].status2 & (STATUS2_CURSED | STATUS2_WRAPPED))
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
		if (GetSandstormDamage(bank) >= hp
		||  GetHailDamage(bank) >= hp
		||  GetLeechSeedDamage(bank) >= hp
		||  GetPoisonDamage(bank) >= hp
		||  GetBurnDamage(bank) >= hp
		||  GetNightmareDamage(bank) >= hp
		||  GetCurseDamage(bank) >= hp
 		||  GetTrapDamage(bank) >= hp)
			return TRUE;
	}
	
	return FALSE;
}

u16 CalcSecondaryEffectChance(u8 bank, u16 move)
{
	u16 chance = gBattleMoves[move].secondaryEffectChance;

	if (ABILITY(bank) == ABILITY_SERENEGRACE || gNewBS->RainbowTimers[SIDE(bank)])
		chance *= 2;

	return chance;
}

bool8 ShouldAIDelayMegaEvolution(u8 bankAtk, u8 bankDef, u16 move)
{
	u8 atkAbility = ABILITY(bankAtk);
	u8 megaAbility = GetBankMegaFormAbility(bankAtk);

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

		case ABILITY_MOXIE:
		case ABILITY_BEASTBOOST:
		case ABILITY_SOULHEART: ;
			if (MoveWouldHitFirst(move, bankAtk, bankDef)
			&&  MoveKnocksOutXHits(move, bankAtk, bankDef, 1))
				return TRUE; //Get that Moxie boost
			break;
	}

	return FALSE;
}

move_t IsValidMovePrediction(u8 bankAtk, u8 bankDef)
{
	if (gNewBS->movePredictions[bankAtk][bankDef] == MOVE_PREDICTION_SWITCH)
		return MOVE_NONE;
	else
		return gNewBS->movePredictions[bankAtk][bankDef];
}

bool8 IsPredictedToSwitch(u8 bankAtk, u8 bankDef)
{
	return gNewBS->movePredictions[bankAtk][bankDef] == MOVE_PREDICTION_SWITCH;
}

void StoreMovePrediction(u8 bankAtk, u8 bankDef, u16 move)
{
	gNewBS->movePredictions[bankAtk][bankDef] = move;
}

void StoreSwitchPrediction(u8 bankAtk, u8 bankDef)
{
	gNewBS->movePredictions[bankAtk][bankDef] = MOVE_PREDICTION_SWITCH;
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

bool8 OnlyBadMovesLeftInMoveset(u8 bankAtk, u8 bankDef)
{
	u8 viability;
	u16 move;
	u32 dmg;
	u8 numDamageMoves = 0;

	u8 moveLimitations = CheckMoveLimitations(gActiveBattler, 0, 0xFF);

	for (int i = 0; i < MAX_MON_MOVES; ++i)
	{
		move = GetBattleMonMove(bankAtk, i);

		if (move == MOVE_NONE)
			break;

		if (!(gBitTable[i] & moveLimitations))
		{
			viability = AI_Script_Negatives(bankAtk, bankDef, move, 100);
			if (viability >= 100)
			{
				if (SPLIT(move) == SPLIT_STATUS)
					return FALSE; //Has viable status move

				++numDamageMoves; //By default can't be a status move here
			}
		}
	}
	
	if (IsDoubleBattle() || !MoveEffectInMoveset(EFFECT_PROTECT, bankAtk)) //Single battle protector usually protect to stall
	{
		if (numDamageMoves == 0)
		{
			return TRUE; //Legit no moves left
		}
		else if (!MoveEffectInMoveset(EFFECT_BATON_PASS, bankAtk) //U-Turn/Volt Switch switch on their own
		&& numDamageMoves == 1) //Only 1 viable damaging move left
		{
			//This is meant to help the Pokemon with a single attacking move
			//and a bunch of setup moves to know they should switch out instead
			//of trying to attack the enemy with their weak move.

			move = gNewBS->strongestMove[bankAtk][bankDef]; //Assume the usuable damage move is the strongest move
			dmg = CalcFinalAIMoveDamage(move, bankAtk, bankDef, 1);

			if (dmg < gBattleMons[bankDef].hp) //Move doesn't KO
			{
				if (HealingMoveInMoveset(bankDef) //Target can heal itself with move
				||  GetAmountToRecoverBy(bankDef, bankAtk, MOVE_PROTECT)) //Target can heal itself with ability/item
				{
					if (dmg >= gBattleMons[bankDef].maxHP / 2) //Damage deals half or more of max health
						return FALSE;
				}
				else
				{
					if (dmg >= gBattleMons[bankDef].maxHP / 3) //Damage deals a third or more of max health
						return FALSE;
				}
			}
			else
				return FALSE;

			return TRUE;
		}
	}
	
	return FALSE; //Has some moves to use
}

u16 TryReplaceMoveWithZMove(u8 bankAtk, u16 move)
{
	if (!gNewBS->ZMoveData->used[bankAtk] && SPLIT(move) != SPLIT_STATUS)
	{
		u8 moveIndex = FindMovePositionInMoveset(move, bankAtk);
		if (moveIndex < MAX_MON_MOVES)
		{
			u16 zMove = ShouldAIUseZMove(bankAtk, moveIndex, move);
			if (zMove != MOVE_NONE)
				move = zMove;
		}
	}

	return move;
}

bool8 GetHealthPercentage(u8 bank)
{
	return (gBattleMons[bank].hp * 100) / gBattleMons[bank].maxHP;
}

bool8 TeamFullyHealedMinusBank(u8 bank)
{
	u8 firstId, lastId;

	pokemon_t* party = LoadPartyRange(bank, &firstId, &lastId);

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

bool8 PartyMemberStatused(u8 bank, bool8 checkSoundproof)
{
	struct Pokemon* party;
	if (SIDE(bank) == B_SIDE_PLAYER)
		party = gPlayerParty;
	else
		party = gEnemyParty;

	for (int i = 0; i < PARTY_SIZE; ++i)
	{
		if (checkSoundproof && GetPartyAbility(&party[i]) == ABILITY_SOUNDPROOF)
			continue;

		if (party[i].condition != STATUS1_NONE)
			return TRUE;
	}

	return FALSE;
}

u16 ShouldAIUseZMove(u8 bank, u8 moveIndex, u16 move)
{
	struct Pokemon* mon = GetBankPartyData(bank);
	u16 item = mon->item;

	if (move == MOVE_NONE)
		move = GetBattleMonMove(bank, moveIndex);

	if (IsMega(bank)
	|| IsRedPrimal(bank)
	|| IsBluePrimal(bank))
		return MOVE_NONE;

	if (IsZCrystal(item) || item == ITEM_ULTRA_NECROZIUM_Z) //The only "Mega Stone" that let's you use a Z-Move
	{
		u16 zmove = GetSpecialZMove(move, SPECIES(bank), item);
		if (zmove != MOVE_NONE && zmove != 0xFFFF) //There's a special Z-Move
			return zmove;

		if (move != MOVE_NONE && zmove != 0xFFFF //Special Z-Crystal
		&& gBattleMoves[move].type == ItemId_GetHoldEffectParam(item))
		{
			if (SPLIT(move) == SPLIT_STATUS)
				return 0xFFFF;
			else
				return GetTypeBasedZMove(move, bank);
		}
	}

	return MOVE_NONE;
}

void IncreaseViability(s16* viability, u8 amount)
{
	*viability = MathMin(*viability + amount, 255);
}

void DecreaseViability(s16* viability, u8 amount)
{
	*viability -= amount;
	
	if (*viability < 0)
		*viability = 0;
}