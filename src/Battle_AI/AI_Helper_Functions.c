#include "../defines.h"
#include "../../include/random.h"
#include "../../include/constants/items.h"

#include "../../include/new/battle_start_turn_start.h"
#include "../../include/new/AI_Helper_Functions.h"
#include "../../include/new/damage_calc.h"
#include "../../include/new/general_bs_commands.h"
#include "../../include/new/helper_functions.h"
#include "../../include/new/item.h"
#include "../../include/new/mega.h"
#include "../../include/new/move_tables.h"

//This file's functions:
static u32 CalcPredictedDamageForCounterMoves(u16 move, u8 bankAtk, u8 bankDef);
static u16 GetBattleMonMove(u8 bank, u8 i);

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
	u16 move;
	int i;

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
			if (MoveKnocksOut(move, bankAtk, bankDef))
				return TRUE;
		}
	}

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
			if (MoveKnocksOut(move, bankAtk, bankDef))
				return TRUE;
		}
	}

	return FALSE;
}

bool8 MoveKnocksOut(u16 move, u8 bankAtk, u8 bankDef)
{
	u16 defHP = gBattleMons[bankDef].hp;

	if (SPLIT(move) == SPLIT_STATUS)
		return FALSE;

	if (gBattleMoves[move].effect == EFFECT_0HKO)
	{
		if (gBattleMons[bankAtk].level <= gBattleMons[bankDef].level)
			return FALSE;
		if (move == MOVE_SHEERCOLD && IsOfType(bankDef, TYPE_ICE))
			return FALSE;
		return TRUE;
	}

	if (gBattleMoves[move].effect == EFFECT_COUNTER || gBattleMoves[move].effect == EFFECT_MIRROR_COAT) //Includes Metal Burst
	{
		if (CalcPredictedDamageForCounterMoves(move, bankAtk, bankDef) >= defHP)
			return TRUE;

		return FALSE;
	}

	if (gBattleMoves[move].power == 0)
		return FALSE;

	if (AI_CalcDmg(bankAtk, bankDef, move) >= defHP)
		return TRUE;

	return FALSE;
}

static u32 CalcPredictedDamageForCounterMoves(u16 move, u8 bankAtk, u8 bankDef)
{
	u16 predictedMove = IsValidMovePrediction(bankDef, bankAtk);
	u32 predictedDamage = 0;

	if (predictedMove != MOVE_NONE)
	{
		predictedDamage = AI_CalcDmg(bankDef, bankAtk, predictedMove); //The damage the enemy will do to the AI

		gBattleMoveDamage = predictedDamage;
		TypeCalc(move, bankAtk, bankDef, 0, FALSE);
		predictedDamage = gBattleMoveDamage;

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

bool8 IsStrongestMove(const u16 currentMove, const u8 bankAtk, const u8 bankDef)
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

		if (!(gBitTable[i] & moveLimitations))
		{
			if (SPLIT(move) == SPLIT_STATUS
			||  gBattleMoves[move].power == 0)
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
				if (MoveWillHit(move, bankAtk, bankDef))
				{
					strongestMove = move;
					highestDamage = 0xFFFFFFFF;

					if (currentMove == strongestMove)
						return TRUE;
					//Keep going because user may have another 0HKO move to be checked that could be strongest
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

	if (strongestMove == currentMove)
		return TRUE;

	return FALSE;
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
	|| (WEATHER_HAS_EFFECT && (gBattleWeather & WEATHER_RAIN_ANY) && CheckTableForMove(move, AlwaysHitRainTable)))
		return TRUE;

	return FALSE;
}

//Add section for move prediction
bool8 MoveWouldHitFirst(u16 move, u16 bankAtk, u16 bankDef)
{
	u32 temp;
	u32 bankAtkSpeed, bankDefSpeed;

//Priority Calc
	if (PriorityCalc(bankAtk, ACTION_USE_MOVE, move) > 0)
		return TRUE;

//BracketCalc
	if (BracketCalc(bankAtk) > BracketCalc(bankDef))
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
	u8 ability = ABILITY(bank);
	u8 effect = ITEM_EFFECT(bank);

	if (WEATHER_HAS_EFFECT
	&& 	ability != ABILITY_MAGICGUARD
	&&	ability != ABILITY_OVERCOAT
	&&	effect  != ITEM_EFFECT_SAFETY_GOGGLES
	&& !(gStatuses3[bank] & (STATUS3_UNDERGROUND | STATUS3_UNDERWATER | STATUS3_DISAPPEARED)))
	{
        if (gBattleWeather & WEATHER_SANDSTORM_ANY)
		{
            if (!IsOfType(bank, TYPE_ROCK) && !IsOfType(bank, TYPE_GROUND) && !IsOfType(bank, TYPE_STEEL)
            && ability != ABILITY_SANDVEIL && ability != ABILITY_SANDRUSH && ability != ABILITY_SANDFORCE
			&& gBattleMons[bank].hp <= gBattleMons[bank].maxHP / 16)
                return TRUE;
        }
        else if (gBattleWeather & WEATHER_HAIL_ANY)
		{
            if (!IsOfType(bank, TYPE_ICE) && ability != ABILITY_ICEBODY && ability != ABILITY_SNOWCLOAK && ability != ABILITY_SLUSHRUSH
			&& gBattleMons[bank].hp <= gBattleMons[bank].maxHP / 16)
				return TRUE;
        }
    }

    return FALSE;
}

u8 CountBanksPositiveStatStages(u8 bank)
{
	u8 positiveStages = 0;
	for (int i = 0; i < BATTLE_STATS_NO - 1; ++i)
	{
		if (STAT_STAGE(bank, i) > 6)
			++positiveStages;
	}

	return positiveStages;
}

u8 CountBanksNegativeStatStages(u8 bank)
{
	u8 negativeStages = 0;
	for (int i = 0; i < BATTLE_STATS_NO - 1; ++i)
	{
		if (STAT_STAGE(bank, i) < 6)
			++negativeStages;
	}

	return negativeStages;
}

static u16 GetBattleMonMove(u8 bank, u8 i)
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

move_t IsValidMovePrediction(u8 bankAtk, u8 bankDef)
{
	if (gNewBS->movePredictions[bankAtk][bankDef] == 0xFFFF)
		return MOVE_NONE;
	else
		return gNewBS->movePredictions[bankAtk][bankDef];
}

void StoreMovePrediction(u8 bankAtk, u8 bankDef, u16 move)
{
	gNewBS->movePredictions[bankAtk][bankDef] = move;
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

bool8 HasProtectionMoveInMoveset(u8 bank, bool8 AllKinds)
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
					case MOVE_WIDEGUARD:
					case MOVE_CRAFTYSHIELD:
					case MOVE_MATBLOCK:
						if (AllKinds)
							return TRUE;
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
			if (gBattleMoves[move].type == moveType)
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

// AI function to check if bank has a status move in their moveset
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
	for (u8 statId = 0; statId < BATTLE_STATS_NO - 1; ++statId)
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
	int i;
	bool8 isSpecialZCrystal = FALSE;
	pokemon_t* mon = GetBankPartyData(bank);

	if (move == MOVE_NONE)
		move = GetBattleMonMove(bank, moveIndex);

	if (IsMega(bank)
	|| IsRedPrimal(bank)
	|| IsBluePrimal(bank))
		return FALSE;

	if (IsZCrystal(mon->item) || mon->item == ITEM_ULTRA_NECROZIUM_Z) //The only "Mega Stone" that let's you use a Z-Move
	{
		for (i = 0; gSpecialZMoveTable[i].species != 0xFFFF; ++i)
		{
			if (gSpecialZMoveTable[i].item == mon->item)
			{
				isSpecialZCrystal = TRUE;
				if (gSpecialZMoveTable[i].species == SPECIES(bank)
				&&  gSpecialZMoveTable[i].move == move)
				{
					return gSpecialZMoveTable[i].zmove;
				}
			}
		}

		if (move != MOVE_NONE && !isSpecialZCrystal
		&& gBattleMoves[move].type == ItemId_GetHoldEffectParam(mon->item))
		{
			if (SPLIT(move) == SPLIT_STATUS)
				return 0xFFFF;
			else if (gBattleMoves[move].type < TYPE_FIRE)
				return MOVE_BREAKNECK_BLITZ_P + (gBattleMoves[move].type * 2) + CalcMoveSplit(bank, move);
			else
				return MOVE_BREAKNECK_BLITZ_P + ((gBattleMoves[move].type - 1) * 2) + CalcMoveSplit(bank, move);
		}
	}

	return FALSE;
}

enum PrefixClasses
{
	PREFIX_SPEEDY,
	PREFIX_BULKY,
};

enum BaseClasses
{
	BASE_SWEEPER,
	BASE_WALL
};

enum ItemClasses
{
	ITM_NONE,
	ITM_TRICK,
	ITM_CHOICE,
};

enum BoostingClasses
{
	NO_BOOST,
	SUB_OFF_BOOST,
	SUB_DEF_BOOST,
	OFF_BOOST,
	DEF_BOOST,
	SET_UP_SUBSTITUTE,
};

struct FightingStyle
{
	enum PrefixClasses prefixClass;
	enum BaseClasses baseClass;
	enum ItemClasses itemClass;
	enum BoostingClasses boostingClass;
	bool8 hazardClass;
	bool8 statusClass;
	bool8 clericClass;
	bool8 recoveryClass;
};

/*
void PredictBankFightingStyle(u8 bank, struct FightingStyle* style)
{
	struct Pokemon* mon = GetBankPartyData(bank);
	u16 offensiveEvs = mon->atkEv + mon->spAtkEv;
	u16 defensiveEvs = mon->defEv + mon->spDefEv;

	//Determine prefix class
	if (mon->spdEv > mon->hpEv)
		style->prefixClass = PREFIX_SPEEDY;
	else
		style->prefixClass = PREFIX_BULKY;

	//Determine base class
	if (offensiveEvs > defensiveEvs)
		style->baseClass = BASE_SWEEPER;
	else
		style->baseClass = BASE_WALL;

	//Determine item class
	u8 itemEffect = ItemId_GetHoldEffect(ITEM(bank)); //Don't use ITEM_EFFECT b/c may not have an effect now, but will when tricked away

	if (MoveEffectInMoveset(EFFECT_TRICK, bank)
	&&  ITEM(bank) != ITEM_NONE
	&&  CanTransferItem(SPECIES(bank), ITEM(bank), mon)
	&&  (itemEffect == ITEM_EFFECT_CHOICE_BAND
	 ||  itemEffect == ITEM_EFFECT_TOXIC_ORB
	 ||  itemEffect == ITEM_EFFECT_FLAME_ORB
	 ||  itemEffect == ITEM_EFFECT_BLACK_SLUDGE
	 ||  itemEffect == ITEM_EFFECT_LAGGING_TAIL
	 ||  itemEffect == ITEM_EFFECT_IRON_BALL
	 ||  itemEffect == ITEM_EFFECT_STICKY_BARB))
	{
		style->itemClass = ITM_TRICK;
	}
	else if (itemEffect == ITEM_EFFECT_CHOICE_BAND)
		style->itemClass = ITM_CHOICE;
	else
		style->itemClass = ITM_NONE;

	//Determine boosting class
	style->boostingClass = NO_BOOST;

	if (IS_BEHIND_SUBSTITUTE(bank))
	{
		if (OffenseBoostingMoveEffectInMoveset(bank)
			style->boostingClass = SUB_OFF_BOOST;
		else if (DefensiveBoostingMoveEffect(bank))
			style->boostingClass = SUB_DEF_BOOST;
	}
	else
	{
		if (MoveEffectInMoveset(EFFECT_SUBSTITUTE, bank)
		&&  GetHealthPercentage(bank) > 49)
			style->boostingClass = SET_UP_SUBSTITUTE;
		else if (OffenseBoostingMoveEffectInMoveset(bank))
			style->boostingClass = OFF_BOOST;
		else if (DefensiveBoostingMoveEffectInMoveset(bank))
			style->boostingClass = DEF_BOOST;
	}

	//Determine hazard class
	if (CanSetUpHazard(bank))
		style->hazardClass = TRUE;
	else
		style->hazardClass = FALSE;

	//Determing status class
	if (StatusSettingMoveInMoveset(bank))
		style->statusClass = TRUE;
	else
		style->statusClass = FALSE

	//Determine cleric class
	if (MoveEffectInMoveset(EFFECT_HEAL_BELL, bank)
	&& PartyMemberStatused(bank))
		style->clericClass = TRUE;
	else
		style->clericClass = FALSE

	//Determine recovery class
	if (GetHealthPercentage(bank) < 50)
		style->recoveryClass = TRUE;
	else
		style->recoveryClass = FALSE;

	//Determine Clear Smog class
	//if (!
}
*/
