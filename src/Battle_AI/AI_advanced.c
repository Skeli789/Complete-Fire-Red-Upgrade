#include "../defines.h"
#include "../../include/random.h"
#include "../../include/constants/items.h"

#include "../../include/new/AI_advanced.h"
#include "../../include/new/AI_Helper_Functions.h"
#include "../../include/new/battle_start_turn_start.h"
#include "../../include/new/damage_calc.h"
#include "../../include/new/Helper_Functions.h"

enum FightingClasses
{
	FIGHT_CLASS_NONE,
	FIGHT_CLASS_SWEEPER_KILL,
	FIGHT_CLASS_SWEEPER_SETUP_STATS,
	FIGHT_CLASS_SWEEPER_SETUP_STATUS,
	FIGHT_CLASS_STALL,
	FIGHT_CLASS_TEAM_SUPPORT_BATON_PASS,
	FIGHT_CLASS_TEAM_SUPPORT_CLERIC,
	FIGHT_CLASS_TEAM_SUPPORT_SCREENS,
	FIGHT_CLASS_TEAM_SUPPORT_PHAZING,
	FIGHT_CLASS_ENTRY_HAZARDS,
};

//This file's functions:
static bool8 BankHoldingUsefulItemToProtectFor(u8 bank);
static bool8 BankHasAbilityUsefulToProtectFor(u8 bankAtk, u8 bankDef);
static bool8 MoveSplitOnTeam(u8 bank, u8 split);
static bool8 ShouldTryToSetUpStat(u8 bankAtk, u8 bankDef, u16 move, u8 stat, u8 statLimit);
static bool8 ShouldUseSubstitute(u8 bankAtk, u8 bankDef);

bool8 IsClassSweeper(u8 class)
{
	return class == FIGHT_CLASS_SWEEPER_KILL
		|| class == FIGHT_CLASS_SWEEPER_SETUP_STATS
		|| class == FIGHT_CLASS_SWEEPER_SETUP_STATUS;
}

bool8 IsClassKillerSweeper(u8 class)
{
	return class == FIGHT_CLASS_SWEEPER_KILL;
}

bool8 IsClassSetupSweeper(u8 class)
{
	return class == FIGHT_CLASS_SWEEPER_SETUP_STATS;
}

bool8 IsClassStatusSweeper(u8 class)
{
	return class == FIGHT_CLASS_SWEEPER_SETUP_STATUS;
}

bool8 IsClassStall(u8 class)
{
	return class == FIGHT_CLASS_STALL;
}

bool8 IsClassTeamSupport(u8 class)
{
	return class == FIGHT_CLASS_TEAM_SUPPORT_BATON_PASS
		|| class == FIGHT_CLASS_TEAM_SUPPORT_CLERIC
		|| class == FIGHT_CLASS_TEAM_SUPPORT_SCREENS
		|| class == FIGHT_CLASS_TEAM_SUPPORT_PHAZING;
}

bool8 IsClassBatonPass(u8 class)
{
	return class == FIGHT_CLASS_TEAM_SUPPORT_BATON_PASS;
}

bool8 IsClassScreener(u8 class)
{
	return class == FIGHT_CLASS_TEAM_SUPPORT_SCREENS;
}

bool8 IsClassCleric(u8 class)
{
	return class == FIGHT_CLASS_TEAM_SUPPORT_CLERIC;
}

bool8 IsClassPhazer(u8 class)
{
	return class == FIGHT_CLASS_TEAM_SUPPORT_PHAZING;
}

bool8 IsClassEntryHazards(u8 class)
{
	return class == FIGHT_CLASS_ENTRY_HAZARDS;
}

u8 PredictBankFightingStyle(u8 bank)
{
	return PredictFightingStyle(gBattleMons[bank].moves, ITEM_EFFECT(bank), bank);
}

u8 PredictFightingStyle(const u16* const moves, const u8 itemEffect, const u8 bank)
{
	int i;
	u8 class = FIGHT_CLASS_NONE;
	u8 attackMoveNum = 0;
	u8 entryHazardNum = 0;
	u8 statusMoveNum = 0;
	bool8 boostingMove = FALSE;

	for (i = 0; i < MAX_MON_MOVES; ++i)
	{
		u16 move = moves[i];
		u8 moveEffect = gBattleMoves[move].effect;

		if (move == MOVE_BATONPASS)
		{
			if (bank != 0xFF)
			{
				if ((!IsDoubleBattle() && ViableMonCountFromBank(bank) > 1)
				  || (IsDoubleBattle() && ViableMonCountFromBank(bank) > 2))
					class = FIGHT_CLASS_TEAM_SUPPORT_BATON_PASS;
				else
					goto MOVE_EFFECT_SWITCH;
			}
			else
				class = FIGHT_CLASS_TEAM_SUPPORT_BATON_PASS;
		}
		else
		{
		MOVE_EFFECT_SWITCH:
			switch (moveEffect) {
				case EFFECT_ROAR:
				case EFFECT_HAZE:
				case EFFECT_REMOVE_TARGET_STAT_CHANGES:
					class = FIGHT_CLASS_TEAM_SUPPORT_PHAZING;
					break;
					
				case EFFECT_WISH:
				case EFFECT_HEAL_BELL:
					class = FIGHT_CLASS_TEAM_SUPPORT_CLERIC;
					break;
					
				case EFFECT_HEAL_TARGET:
					if (IsDoubleBattle())
						class = FIGHT_CLASS_TEAM_SUPPORT_CLERIC;
					break;
					
				case EFFECT_REFLECT:
				case EFFECT_LIGHT_SCREEN:
					class = FIGHT_CLASS_TEAM_SUPPORT_SCREENS;
					break;
					
				case EFFECT_TRAP:
				case EFFECT_MEAN_LOOK:
					class = FIGHT_CLASS_STALL;
					break;
					
				default:
					if (itemEffect == ITEM_EFFECT_CHOICE_BAND || itemEffect == ITEM_EFFECT_ASSAULT_VEST)
						class = FIGHT_CLASS_SWEEPER_KILL;
					else if (moveEffect == EFFECT_RESTORE_HP //Not placed above because checked in this order
						  || moveEffect == EFFECT_MORNING_SUN
						  || moveEffect == EFFECT_SWALLOW)
					{
						class = FIGHT_CLASS_STALL;
					}
			}
		}

		if (class != FIGHT_CLASS_NONE) break; //Leave loop because we found a class

		if (SPLIT(move) != SPLIT_STATUS)
			++attackMoveNum;

		switch (moveEffect) {
			case EFFECT_ATTACK_UP:
			case EFFECT_DEFENSE_UP:
			case EFFECT_SPEED_UP:
			case EFFECT_SPECIAL_ATTACK_UP:
			case EFFECT_SPECIAL_DEFENSE_UP:
			case EFFECT_ACCURACY_UP:
			case EFFECT_EVASION_UP:
			case EFFECT_ATTACK_UP_2:
			case EFFECT_DEFENSE_UP_2:
			case EFFECT_SPEED_UP_2:
			case EFFECT_SPECIAL_ATTACK_UP_2:
			case EFFECT_SPECIAL_DEFENSE_UP_2:
			case EFFECT_ACCURACY_UP_2:
			case EFFECT_EVASION_UP_2:
			case EFFECT_COSMIC_POWER:
			case EFFECT_DRAGON_DANCE:
			case EFFECT_CALM_MIND:
			case EFFECT_BULK_UP:
				boostingMove = TRUE;
				break;
				
			case EFFECT_SPIKES:
				++entryHazardNum;
				break;
				
			default:
				if (SPLIT(move) == SPLIT_STATUS)
					++statusMoveNum;
		}
	}

	if (class == FIGHT_CLASS_NONE)
	{
		if (entryHazardNum >= 2)
			class = FIGHT_CLASS_ENTRY_HAZARDS;
		else if (attackMoveNum >= 3)
			class = FIGHT_CLASS_SWEEPER_KILL;
		else if (attackMoveNum >= 2)
		{
			if (boostingMove)
				class = FIGHT_CLASS_SWEEPER_SETUP_STATS;
			else if (statusMoveNum)
				class = FIGHT_CLASS_SWEEPER_SETUP_STATUS;
		}

		class = FIGHT_CLASS_STALL; //Returns STALL by default
	}

	return class; 
}

bool8 ShouldTrap(u8 bankAtk, u8 bankDef, u16 move, u8 class)
{
	if (IsClassStall(class))
	{
		if (MoveWouldHitFirst(move, bankAtk, bankDef)) //Attacker goes first
		{
			if (!CanKnockOut(bankDef, bankAtk)) //Enemy can't kill attacker
				return TRUE;
		}
		else //Opponent Goes First
		{
			if (!CanKnockOut(bankDef, bankAtk)
			&&  !Can2HKO(bankDef, bankAtk))
				return TRUE;
		}
	}
	
	return FALSE;
}

u16 GetAmountToRecoverBy(u8 bankAtk, u8 bankDef, u16 move)
{
	if (gNewBS->HealBlockTimers[bankAtk] > 0)
		return 0;

	u16 amountToRecover = 0;
	u16 curHp = gBattleMons[bankAtk].hp;
	u16 maxHp = gBattleMons[bankAtk].maxHP;

	switch (gBattleMoves[move].effect) {
		case EFFECT_RESTORE_HP:
			amountToRecover = MathMax(1, maxHp / 2);
			break;
		
		case EFFECT_MORNING_SUN:
			switch (move) {
				case MOVE_SHOREUP:
					if (gBattleWeather & WEATHER_SANDSTORM_ANY && WEATHER_HAS_EFFECT)
						amountToRecover = maxHp;
					else
						amountToRecover = MathMax(1, maxHp / 2);
					break;
					
				default:
					if (gBattleWeather & WEATHER_SUN_ANY && WEATHER_HAS_EFFECT)
						amountToRecover = maxHp;
					else if (gBattleWeather && WEATHER_HAS_EFFECT) //Not sunny
						amountToRecover = MathMax(1, maxHp / 4);
					else
						amountToRecover = MathMax(1, maxHp / 2);
					break;
			}
			break;
		
		case EFFECT_SWALLOW:
			switch (gDisableStructs[bankAtk].stockpileCounter) {
				case 1:
					amountToRecover = MathMax(1, maxHp / 4);
					break;
				case 2:
					amountToRecover = MathMax(1, maxHp / 2);
					break;
				case 3:
					amountToRecover = maxHp;
					break;
			}
			break;
		
		case EFFECT_REST:
			amountToRecover = maxHp;
			break;
		
		case EFFECT_ABSORB:
		case EFFECT_DREAM_EATER: ;
			u16 predictedDmg = CalcFinalAIMoveDamage(move, bankAtk, bankDef, 1);

			if (move == MOVE_OBLIVIONWING)
				amountToRecover = MathMax(1, (75 * predictedDmg) / 100);
			else
				amountToRecover = MathMax(1, predictedDmg / 2);

			if (ITEM_EFFECT(bankAtk) == ITEM_EFFECT_BIG_ROOT)
				amountToRecover = (130 * predictedDmg) / 100;
			break;
		
		case EFFECT_PAIN_SPLIT: ;
			u16 finalHp = MathMax(1, (curHp + gBattleMons[bankDef].hp) / 2);
			
			if (finalHp > curHp)
				amountToRecover = finalHp - curHp;
			break;

		case EFFECT_PROTECT: ;
			u8 ability = ABILITY(bankAtk);
			u8 itemEffect = ITEM_EFFECT(bankAtk);

			if (gStatuses3[bankAtk] & (STATUS3_ROOTED | STATUS3_AQUA_RING))
			{
				u16 hp = MathMax(1, maxHp / 16);
				if (itemEffect == ITEM_EFFECT_BIG_ROOT)
					amountToRecover += (130 * hp) / 100;
			}

			for (int i = 0; i < gBattlersCount; ++i)
			{
				if (gStatuses3[i] & STATUS3_LEECHSEED
				&&  ABILITY(i) != ABILITY_MAGICGUARD
				&& (gStatuses3[i] & STATUS3_LEECHSEED_BATTLER) == bankAtk)
				{
					amountToRecover += MathMax(1, gBattleMons[i].maxHP / 8);
				}
			}
			
			if (itemEffect == ITEM_EFFECT_LEFTOVERS
			|| (itemEffect == ITEM_EFFECT_BLACK_SLUDGE && IsOfType(bankAtk, TYPE_POISON)))
			{
				amountToRecover += MathMax(1, maxHp / 16);
			}
			
			if (gWishFutureKnock->wishCounter[bankAtk] > 0)
			{
				amountToRecover += MathMax(1, maxHp / 2);
			}
			
			if (gBattleWeather & WEATHER_RAIN_ANY && WEATHER_HAS_EFFECT)
			{
				if (ability == ABILITY_RAINDISH)
					amountToRecover += MathMax(1, maxHp / 16);
				else if (ability == ABILITY_DRYSKIN)
					amountToRecover += MathMax(1, maxHp / 8);
			}
			
			if (gBattleWeather & WEATHER_HAIL_ANY && WEATHER_HAS_EFFECT)
			{
				if (ability == ABILITY_ICEBODY)
					amountToRecover += MathMax(1, maxHp / 16);
			}
			
			if (gBattleMons[bankAtk].status1 & STATUS1_PSN_ANY)
			{
				if (ability == ABILITY_POISONHEAL)
					amountToRecover += MathMax(1, maxHp / 8);
			}
			
			if (TerrainType == GRASSY_TERRAIN && CheckGrounding(bankAtk) == GROUNDED)
			{
				amountToRecover += MathMax(1, maxHp / 16);
			}
	}
	
	return MathMin(amountToRecover, maxHp - curHp);
}

bool8 ShouldRecover(u8 bankAtk, u8 bankDef, u16 move)
{
	u32 healAmount = GetAmountToRecoverBy(bankAtk, bankDef, move);

	if (MoveWouldHitFirst(move, bankAtk, bankDef)) //Attacker goes first
	{
		if (CanKnockOut(bankDef, bankAtk)
		&& !CanKnockOutAfterHealing(bankDef, bankAtk, healAmount, 1))
			return TRUE;
	}
	else //Opponent Goes First
	{
		if (!CanKnockOut(bankDef, bankAtk)) //Enemy can't kill attacker
		{
			if (Can2HKO(bankDef, bankAtk)
			&& !CanKnockOutAfterHealing(bankDef, bankAtk, healAmount, 2))
				return TRUE;
		}
	}

	return FALSE;
}

static bool8 BankHoldingUsefulItemToProtectFor(u8 bank)
{
	u8 ability = ABILITY(bank);
	u8 itemEffect = ITEM_EFFECT(bank);

	if (itemEffect == ITEM_EFFECT_TOXIC_ORB
	&& CanBePoisoned(bank, bank, FALSE)
	&& (ability == ABILITY_POISONHEAL
	 || ability == ABILITY_TOXICBOOST
	 || ability == ABILITY_QUICKFEET
	 || ability == ABILITY_MAGICGUARD
	 || MoveInMoveset(MOVE_FACADE, bank)))
		return TRUE;

	if (itemEffect == ITEM_EFFECT_FLAME_ORB
	&& CanBeBurned(bank, FALSE)
	&& (ability == ABILITY_GUTS
	 || ability == ABILITY_FLAREBOOST
	 || ability == ABILITY_MAGICGUARD
	 || MoveInMoveset(MOVE_FACADE, bank)))
		return TRUE;
		
	return FALSE;
}

static bool8 BankHasAbilityUsefulToProtectFor(u8 bankAtk, u8 bankDef)
{
	u8 ability = ABILITY(bankAtk);

	switch (ability) {
		case ABILITY_MOODY:
			if (!StatsMaxed(bankAtk))
				return TRUE;
			break;

		case ABILITY_SPEEDBOOST:
			if (SpeedCalc(bankAtk) <= SpeedCalc(bankDef)
			&&  STAT_STAGE(bankAtk, STAT_STAGE_SPEED) < STAT_STAGE_MAX)
				return TRUE;
			break;
	}

	return FALSE;
}

enum ProtectQueries ShouldProtect(u8 bankAtk, u8 bankDef, u16 move)
{
	u8 predictedMoveEffect = gBattleMoves[IsValidMovePrediction(bankDef, bankAtk)].effect;

	if (BankHoldingUsefulItemToProtectFor(bankAtk)
	||  BankHasAbilityUsefulToProtectFor(bankAtk, bankDef)
	||  predictedMoveEffect == EFFECT_EXPLOSION
	|| (predictedMoveEffect == EFFECT_SEMI_INVULNERABLE && BATTLER_SEMI_INVULNERABLE(bankDef))) //Foe coming down
		return USE_PROTECT;

	if (!IsDoubleBattle())
	{
		u32 healAmount = GetAmountToRecoverBy(bankAtk, bankDef, move);
		
		if (CanKnockOut(bankDef, bankAtk)) //Foe can kill
		{
			if (!CanKnockOutAfterHealing(bankDef, bankAtk, healAmount, 1))
				return USE_PROTECT;
			else if (IsTakingSecondaryDamage(bankDef))
				return USE_PROTECT;
			else
				return USE_STATUS_THEN_PROTECT;
		}
		else if (Can2HKO(bankDef, bankAtk))
		{
			if (!CanKnockOutAfterHealing(bankDef, bankAtk, healAmount, 2))
				return USE_PROTECT;
			else if (IsTakingSecondaryDamage(bankDef))
				return USE_PROTECT;
			else
				return USE_STATUS_THEN_PROTECT;
		}
	}
	else //Double Battle
	{
		u8 partner = PARTNER(bankAtk);

		if (BATTLER_ALIVE(partner)
		&& ABILITY(bankAtk) != ABILITY_TELEPATHY)
		{
			u16 partnerMove = gChosenMovesByBanks[partner];
			if (partnerMove == MOVE_NONE)
				partnerMove = IsValidMovePrediction(partner, bankDef);

			if (partnerMove != MOVE_NONE
			&&  gBattleMoves[partnerMove].target & MOVE_TARGET_ALL
			&& !(AI_SpecialTypeCalc(partnerMove, partner, bankAtk) & MOVE_RESULT_NO_EFFECT))
			{
				return USE_PROTECT; //Protect if partner is going to use a move that damages the whole field
			}
		}
		
		// if (predictedMoveEffect == EFFECT_FAKE_OUT)
			// return USE_PROTECT;
	}

	return DONT_PROTECT;
}

bool8 ShouldPhaze(u8 bankAtk, u8 bankDef, u16 move, u8 class)
{
	if (IsClassPhazer(class))
	{
		if (MoveWouldHitFirst(move, bankAtk, bankDef)) //Attacker goes first
		{
			if (CanKnockOut(bankDef, bankAtk)) //Foe can kill
			{
				if (AnyStatIsRaised(bankDef))
					return TRUE;
			}
			else
				goto ROAR_CHECK;
		}
		else //Opponent Goes First
		{
			if (Can2HKO(bankDef, bankAtk))
			{
				if (AnyStatIsRaised(bankDef))
					return TRUE;
			}
			else
			{
			ROAR_CHECK:
				switch (gBattleMoves[move].effect)
				{
					case EFFECT_ROAR:
						if (gSideAffecting[SIDE(bankDef)] & SIDE_STATUS_SPIKES
						||  AnyStatIsRaised(bankDef))
							return TRUE;
						break;
					default:
						if (AnyStatIsRaised(bankDef))
							return TRUE;
				}
			}
		}
	}
	
	return FALSE;
}

bool8 ShouldUseWishAromatherapy(u8 bankAtk, unusedArg u8 bankDef, u16 move, u8 class)
{
	int i;
	u8 firstId, lastId;
	struct Pokemon* party;
	bool8 hasStatus = FALSE;
	bool8 needHealing = FALSE;
	
	party = LoadPartyRange(bankAtk, &firstId, &lastId);

	if (IsClassCleric(class))
	{
		for (i = 0; i < PARTY_SIZE; ++i)
		{
			u16 curHp = GetMonData(&party[i], MON_DATA_HP, NULL);
			u16 maxHp = GetMonData(&party[i], MON_DATA_MAX_HP, NULL);

			if (!GetMonData(&party[i], MON_DATA_IS_EGG, NULL) && curHp > 0)
			{
				if ((curHp * 100) / maxHp < 65) //Less than 65% health remaining
					needHealing = TRUE;
					
				if (GetMonData(&party[i], MON_DATA_STATUS, NULL) != STATUS1_NONE)
				{
					if (move != MOVE_HEALBELL || GetPartyAbility(&party[i]) != ABILITY_SOUNDPROOF) 
						hasStatus = TRUE;
				}
			}
		}
		
		switch (gBattleMoves[move].effect) {
			case EFFECT_WISH:
				if (needHealing)
					return TRUE;
				break;
				
			case EFFECT_HEAL_BELL:
				if (hasStatus)
					return TRUE;
		}
	}
	
	return FALSE;
}

static bool8 MoveSplitOnTeam(u8 bank, u8 split)
{
	u8 firstId, lastId;
	struct Pokemon* party = LoadPartyRange(bank, &firstId, &lastId);

	for (int i = 0; i < PARTY_SIZE; ++i)
	{
		for (int j = 0; j < MAX_MON_MOVES; ++j)
		{
			bool8 alive = GetMonData(&party[i], MON_DATA_HP, NULL);
			u16 move = GetMonData(&party[i], MON_DATA_MOVE1 + j, NULL);
			u16 pp = GetMonData(&party[i], MON_DATA_PP1 + j, NULL);

			if (alive && pp > 0 && move != MOVE_NONE)
			{
				if (CalcMoveSplitFromParty(&party[i], move) == split)
					return TRUE;
			}
		}
	}
	
	return FALSE;
}

bool8 ShouldSetUpScreens(u8 bankAtk, u8 bankDef, u16 move)
{
	if (!gNewBS->AuroraVeilTimers[SIDE(bankAtk)])
	{
		if (move == MOVE_AURORAVEIL
		&& gBattleWeather & WEATHER_HAIL_ANY
		&& !((gSideAffecting[bankAtk] & (SIDE_STATUS_REFLECT | SIDE_STATUS_LIGHTSCREEN)) == (SIDE_STATUS_REFLECT | SIDE_STATUS_LIGHTSCREEN)))
			return TRUE;
		else
		{
			switch (gBattleMoves[move].effect) {
				case EFFECT_REFLECT:
					if (MoveSplitOnTeam(bankDef, SPLIT_PHYSICAL))
						return TRUE;
					break;

				case EFFECT_LIGHT_SCREEN:
					if (MoveSplitOnTeam(bankDef, SPLIT_SPECIAL))
						return TRUE;
			}
		}
	}
	
	return FALSE;
}

u8 BankLikelyToUseMoveSplit(u8 bank, u8 class)
{
	if (PhysicalMoveInMoveset(bank))
	{
		if (IsClassSweeper(class))
		{
			if (gBattleMons[bank].attack > gBattleMons[bank].spAttack)
				return SPLIT_PHYSICAL;
		}
	}
	else if (SpecialMoveInMoveset(bank))
	{
		if (IsClassSweeper(class))
		{
			if (gBattleMons[bank].spAttack > gBattleMons[bank].attack)
				return SPLIT_SPECIAL;
		}
	}

	return SPLIT_STATUS;
}

void IncreaseStatusViability(s16* originalViability, u8 class, u8 boost, u8 bankAtk, u8 bankDef)
{
	s16 viability = *originalViability;

	switch (class) {
		case FIGHT_CLASS_SWEEPER_KILL:
			break;

		case FIGHT_CLASS_SWEEPER_SETUP_STATS:
			if (!Can2HKO(bankDef, bankAtk))
				INCREASE_VIABILITY(3);
			break;	
			
		case FIGHT_CLASS_SWEEPER_SETUP_STATUS:
			INCREASE_VIABILITY(4 + boost);
			break;
		
		case FIGHT_CLASS_STALL:
			INCREASE_VIABILITY(4 + boost);
			break;
		
		case FIGHT_CLASS_TEAM_SUPPORT_BATON_PASS:
			if (boost >= 3)
				INCREASE_VIABILITY(1);
			break;
		
		case FIGHT_CLASS_TEAM_SUPPORT_CLERIC:
			INCREASE_VIABILITY(3 + boost);
			break;

		case FIGHT_CLASS_TEAM_SUPPORT_SCREENS:
			INCREASE_VIABILITY(2 + boost);
			break;
		
		case FIGHT_CLASS_TEAM_SUPPORT_PHAZING:
			INCREASE_VIABILITY(4 + boost);
			break;
			
		case FIGHT_CLASS_ENTRY_HAZARDS:
			INCREASE_VIABILITY(3);
			break;
	}
	
	
	*originalViability = MathMin(viability, 255);
}

static bool8 ShouldTryToSetUpStat(u8 bankAtk, u8 bankDef, u16 move, u8 stat, u8 statLimit)
{
	if (ABILITY(bankDef) == ABILITY_UNAWARE
	&& !MoveInMoveset(MOVE_STOREDPOWER, bankAtk)
	&& !MoveInMoveset(MOVE_POWERTRIP, bankAtk))
		return FALSE;

	if (MoveWouldHitFirst(move, bankAtk, bankDef)) //Attacker goes first
	{
		if (CanKnockOut(bankDef, bankAtk))
		{
			return FALSE;
		}
		else
		{
			if (BATTLER_SEMI_INVULNERABLE(bankDef))
				return TRUE; //Can't hit target anyways

			if (STAT_STAGE(bankAtk, stat) >= statLimit)
				return FALSE;
		}
	
		return TRUE;
	}
	else //Opponent Goes First
	{
		if (IsMovePredictionSemiInvulnerable(bankDef, bankAtk))
			return TRUE;

		if (!Can2HKO(bankDef, bankAtk) && STAT_STAGE(bankAtk, stat) < statLimit)
			return TRUE;
			
		return FALSE;
	}
}

void IncreaseStatViability(s16* originalViability, u8 class, u8 boost, u8 bankAtk, u8 bankDef, u16 move, u8 stat, u8 statLimit)
{
	s16 viability = *originalViability;

	switch (class) {
		case FIGHT_CLASS_SWEEPER_KILL:
			break;

		case FIGHT_CLASS_SWEEPER_SETUP_STATS:
			if (ShouldTryToSetUpStat(bankAtk, bankDef, move, stat, statLimit))
				INCREASE_VIABILITY(3 + boost);
			break;
			
		case FIGHT_CLASS_SWEEPER_SETUP_STATUS:
			if (ShouldTryToSetUpStat(bankAtk, bankDef, move, stat, statLimit))
				INCREASE_STATUS_VIABILITY(1); //Treat like a low-priority status move
			break;
		
		case FIGHT_CLASS_STALL:
			if (ShouldTryToSetUpStat(bankAtk, bankDef, move, stat, statLimit))
				INCREASE_STATUS_VIABILITY(1); //Treat like a low-priority status move
			break;
		
		case FIGHT_CLASS_TEAM_SUPPORT_BATON_PASS:
			switch (stat) {
				//Offsensive Stat Booster
				case STAT_STAGE_ATK:
				case STAT_STAGE_SPATK:
				case STAT_STAGE_SPEED:
				case STAT_STAGE_ACC:
					if (STAT_STAGE(bankAtk, stat) < 6 + 4)
						INCREASE_VIABILITY(5);
					break;

				case STAT_STAGE_DEF:
				case STAT_STAGE_SPDEF:
					if (STAT_STAGE(bankAtk, stat) < 6 + 4)
						INCREASE_VIABILITY(4);
					break;

				//Evasiveness Stat Booster
				case STAT_STAGE_EVASION:
					if (STAT_STAGE(bankAtk, stat) < STAT_STAGE_MAX)
						INCREASE_VIABILITY(6);
					break;
			}
			break;

		case FIGHT_CLASS_TEAM_SUPPORT_CLERIC:
			if (ShouldTryToSetUpStat(bankAtk, bankDef, move, stat, statLimit))
				INCREASE_STATUS_VIABILITY(1); //Treat like a low-priority status move
			break;

		case FIGHT_CLASS_TEAM_SUPPORT_SCREENS:
			if (ShouldTryToSetUpStat(bankAtk, bankDef, move, stat, statLimit))
				INCREASE_STATUS_VIABILITY(1); //Treat like a low-priority status move
			break;
		
		case FIGHT_CLASS_TEAM_SUPPORT_PHAZING:
			if (!MoveEffectInMoveset(EFFECT_HAZE, bankAtk) //Don't boost stats if can haze them away
			&&  ShouldTryToSetUpStat(bankAtk, bankDef, move, stat, statLimit))
				INCREASE_STATUS_VIABILITY(1); //Treat like a low-priority status move
			break;
		
		case FIGHT_CLASS_ENTRY_HAZARDS:
			if (ShouldTryToSetUpStat(bankAtk, bankDef, move, stat, statLimit))
				INCREASE_STATUS_VIABILITY(1); //Treat like a low-priority status move
			break;
	}
	
	*originalViability = MathMin(viability, 255);
}

static bool8 ShouldUseSubstitute(u8 bankAtk, u8 bankDef)
{
	if (MoveWouldHitFirst(MOVE_SUBSTITUTE, bankAtk, bankDef)) //Attacker goes first
	{
		if (CalcFinalAIMoveDamage(IsValidMovePrediction(bankDef, bankAtk), bankDef, bankAtk, 1) < MathMax(1, gBattleMons[bankAtk].maxHP / 4))
			return TRUE;
	}
	else
	{
		if (IsPredictedToSwitch(bankDef, bankAtk))
			return TRUE;
	}
	
	return FALSE;
}

void IncreaseSubstituteViability(s16* originalViability, u8 class, u8 bankAtk, u8 bankDef)
{
	s16 viability = *originalViability;

	if (!IS_BEHIND_SUBSTITUTE(bankAtk))
	{
		switch (class) {
			case FIGHT_CLASS_SWEEPER_KILL:
				break;

			case FIGHT_CLASS_SWEEPER_SETUP_STATS:
				if (ShouldUseSubstitute(bankAtk, bankDef))
					INCREASE_VIABILITY(8);
				break;
				
			case FIGHT_CLASS_SWEEPER_SETUP_STATUS:
				if (ShouldUseSubstitute(bankAtk, bankDef))
					INCREASE_STATUS_VIABILITY(2);
				break;
			
			case FIGHT_CLASS_STALL:
				if (ShouldUseSubstitute(bankAtk, bankDef))
					INCREASE_STATUS_VIABILITY(2);
				break;
			
			case FIGHT_CLASS_TEAM_SUPPORT_BATON_PASS:
				if (ShouldUseSubstitute(bankAtk, bankDef))
					IncreaseViability(&viability, 4);
				break;

			case FIGHT_CLASS_TEAM_SUPPORT_CLERIC:
				if (ShouldUseSubstitute(bankAtk, bankDef))
					INCREASE_STATUS_VIABILITY(1);
				break;

			case FIGHT_CLASS_TEAM_SUPPORT_SCREENS:
				if (ShouldUseSubstitute(bankAtk, bankDef))
					INCREASE_STATUS_VIABILITY(1);
				break;
			
			case FIGHT_CLASS_TEAM_SUPPORT_PHAZING:
				if (ShouldUseSubstitute(bankAtk, bankDef))
					INCREASE_STATUS_VIABILITY(1);
				break;

			case FIGHT_CLASS_ENTRY_HAZARDS:
				if (ShouldUseSubstitute(bankAtk, bankDef))
					INCREASE_STATUS_VIABILITY(1);
				break;
		}
	}
	
	*originalViability = MathMin(viability, 255);
}

void IncreaseEntryHazardsViability(s16* originalViability, u8 class, u8 bankAtk, u8 bankDef, u16 move)
{
	s16 viability = *originalViability;

	switch (class) {
		case FIGHT_CLASS_SWEEPER_KILL:
			break;

		case FIGHT_CLASS_SWEEPER_SETUP_STATS:
			INCREASE_STATUS_VIABILITY(2); //Treat like a middle-priority status move
			break;
			
		case FIGHT_CLASS_SWEEPER_SETUP_STATUS:
			INCREASE_STATUS_VIABILITY(2); //Treat like a middle-priority status move
			break;
		
		case FIGHT_CLASS_STALL:
			INCREASE_STATUS_VIABILITY(2); //Treat like a middle-priority status move
			break;
		
		case FIGHT_CLASS_TEAM_SUPPORT_BATON_PASS:
			if (gBattleMoves[move].effect == EFFECT_SPIKES) //Entry Hazards
			{
				if (MoveWouldHitFirst(move, bankAtk, bankDef))
				{
					if (!CanKnockOut(bankDef, bankAtk)) //Opponent can't kill before user can Baton Pass
						INCREASE_VIABILITY(4); //Right more important than Baton Pass
				}
				else
				{
					if (!Can2HKO(bankDef, bankAtk)) //Opponent can't kill before user can Baton Pass
						INCREASE_VIABILITY(4); //Right more important than Baton Pass
				}
			}
			else
				INCREASE_STATUS_VIABILITY(2); //Treat like a middle-priority status move
			break;

		case FIGHT_CLASS_TEAM_SUPPORT_CLERIC:
			INCREASE_STATUS_VIABILITY(2); //Treat like a middle-priority status move
			break;

		case FIGHT_CLASS_TEAM_SUPPORT_SCREENS:
			INCREASE_STATUS_VIABILITY(2); //Treat like a middle-priority status move
			break;
		
		case FIGHT_CLASS_TEAM_SUPPORT_PHAZING:
			INCREASE_STATUS_VIABILITY(3);
			break;
			
		case FIGHT_CLASS_ENTRY_HAZARDS:
			switch (move) {
				case MOVE_STICKYWEB:
					INCREASE_VIABILITY(7);
					break;
				case MOVE_STEALTHROCK:
					INCREASE_VIABILITY(6);
					break;
				case MOVE_TOXICSPIKES:
					INCREASE_VIABILITY(5);
					break;
				default:
					INCREASE_VIABILITY(4);
					break;
			}
			break;
	}
	
	*originalViability = MathMin(viability, 255);
}