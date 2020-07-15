#include "defines.h"
#include "defines_battle.h"
#include "../include/battle.h"
#include "../include/field_weather.h"
#include "../include/overworld.h"
#include "../include/constants/hold_effects.h"
#include "../include/constants/items.h"
#include "../include/constants/pokedex.h"
#include "../include/constants/pokemon.h"
#include "../include/constants/species.h"

#include "../include/new/dns.h"
#include "../include/new/evolution.h"
/*
evolution.c
	handles old and new evolution methods
*/

u16 GetEvolutionTargetSpecies(struct Pokemon* mon, u8 type, u16 evolutionItem)
{
	int i, j;
	u16 targetSpecies = 0;
	u32 personality = mon->personality;
	u16 species = mon->species;
	u16 heldItem = mon->item;
	u8 level;
	u16 friendship;
	u8 beauty = mon->beauty;
	u16 upperPersonality = personality >> 16;
	u8 holdEffect = ItemId_GetHoldEffect(heldItem);

	if (holdEffect == ITEM_EFFECT_PREVENT_EVOLVE && type != 3)
		return SPECIES_NONE;

	switch (type)
	{
	case 0:
		level = mon->level;
		friendship = mon->friendship;

		for (i = 0; i < EVOS_PER_MON; ++i)
		{
			switch (gEvolutionTable[species][i].method)
			{
				case EVO_FRIENDSHIP:
					if (friendship >= 220)
						targetSpecies = gEvolutionTable[species][i].targetSpecies;
					break;

				case EVO_FRIENDSHIP_DAY:
				#ifdef TIME_ENABLED
					if (IsDayTime() && friendship >= 220)
						targetSpecies = gEvolutionTable[species][i].targetSpecies;
				#endif
					break;

				case EVO_FRIENDSHIP_NIGHT:
				#ifdef TIME_ENABLED
					if (IsNightTime() && friendship >= 220)
						targetSpecies = gEvolutionTable[species][i].targetSpecies;
				#endif
					break;

				case EVO_LEVEL:
					if (gEvolutionTable[species][i].param <= level)
						targetSpecies = gEvolutionTable[species][i].targetSpecies;
					break;

				case EVO_LEVEL_ATK_GT_DEF:
					if (gEvolutionTable[species][i].param <= level)
						if (mon->attack > mon->defense)
							targetSpecies = gEvolutionTable[species][i].targetSpecies;
					break;

				case EVO_LEVEL_ATK_EQ_DEF:
					if (gEvolutionTable[species][i].param <= level)
						if (mon->attack == mon->defense)
							targetSpecies = gEvolutionTable[species][i].targetSpecies;
					break;

				case EVO_LEVEL_ATK_LT_DEF:
					if (gEvolutionTable[species][i].param <= level)
						if (mon->attack < mon->defense)
							targetSpecies = gEvolutionTable[species][i].targetSpecies;
					break;

				case EVO_LEVEL_SILCOON:
					if (gEvolutionTable[species][i].param <= level && (upperPersonality % 10) <= 4)
						targetSpecies = gEvolutionTable[species][i].targetSpecies;
					break;

				case EVO_LEVEL_CASCOON:
					if (gEvolutionTable[species][i].param <= level && (upperPersonality % 10) > 4)
						targetSpecies = gEvolutionTable[species][i].targetSpecies;
					break;

				case EVO_LEVEL_NINJASK:
					if (gEvolutionTable[species][i].param <= level)
						targetSpecies = gEvolutionTable[species][i].targetSpecies;
					break;

				case EVO_BEAUTY:
					if (gEvolutionTable[species][i].param <= beauty)
						targetSpecies = gEvolutionTable[species][i].targetSpecies;
					break;

				case EVO_RAINY_FOGGY_OW:
					if (gEvolutionTable[species][i].param <= level
					&& (GetCurrentWeather() == WEATHER_RAIN_LIGHT
					 || GetCurrentWeather() == WEATHER_RAIN_MED
					 || GetCurrentWeather() == WEATHER_RAIN_HEAVY
					 || GetCurrentWeather() == WEATHER_FOG_1
					 || GetCurrentWeather() == WEATHER_FOG_2))
					{
						targetSpecies = gEvolutionTable[species][i].targetSpecies;
					}
					break;

				case EVO_MOVE_TYPE:	// expanded for custom evolutions of any move type
					// move type to know in param
					for (j = 0; j < MAX_MON_MOVES; ++j)
					{
						if (gBattleMoves[mon->moves[j]].type == gEvolutionTable[species][i].param)
						{
							targetSpecies = gEvolutionTable[species][i].targetSpecies;
							break;
						}
					}
					break;

				case EVO_TYPE_IN_PARTY:
					// level in param
					// type in unknown
					// eg. Pancham table would be:
					//		[SPECIES_PANCHAM] = {{EVO_TYPE_IN_PARTY, 32, SPECIES_PANGORO, TYPE_DARK}},
					if (level >= gEvolutionTable[species][i].param)
					{
						u8 typeCheck = gEvolutionTable[species][i].unknown;
						if (typeCheck >= NUMBER_OF_MON_TYPES)
							break;
						for (j = 0; j < gPlayerPartyCount; ++j)
						{
							if (gBaseStats[gPlayerParty[j].species].type1 == typeCheck
							||  gBaseStats[gPlayerParty[j].species].type2 == typeCheck)
							{
								targetSpecies = gEvolutionTable[species][i].targetSpecies;
								break;
							}
						}
					}
					break;

				case EVO_MAP:
					if (GetCurrentRegionMapSectionId() == gEvolutionTable[species][i].param) //Based on map name
						targetSpecies = gEvolutionTable[species][i].targetSpecies;
					break;

				case EVO_MALE_LEVEL:
					if (gEvolutionTable[species][i].param <= level && GetMonGender(mon) == MON_MALE)
						targetSpecies = gEvolutionTable[species][i].targetSpecies;
					break;

				case EVO_FEMALE_LEVEL:
					if (gEvolutionTable[species][i].param <= level && GetMonGender(mon) == MON_FEMALE)
						targetSpecies = gEvolutionTable[species][i].targetSpecies;
					break;

				case EVO_LEVEL_NIGHT:
					#ifdef TIME_ENABLED
						if (gEvolutionTable[species][i].param <= level && IsNightTime())
							targetSpecies = gEvolutionTable[species][i].targetSpecies;
					#else  // regular level up check
						if (gEvolutionTable[species][i].param <= level)
							targetSpecies = gEvolutionTable[species][i].targetSpecies;
					#endif
					break;

				case EVO_LEVEL_DAY:
					#ifdef TIME_ENABLED
						if (gEvolutionTable[species][i].param <= level && IsDayTime())
							targetSpecies = gEvolutionTable[species][i].targetSpecies;
					#else  // regular level up check
						if (gEvolutionTable[species][i].param <= level)
							targetSpecies = gEvolutionTable[species][i].targetSpecies;
					#endif
					break;

				case EVO_HOLD_ITEM_NIGHT:
					#ifdef TIME_ENABLED
					if (heldItem == gEvolutionTable[species][i].param && IsNightTime())
					{
						targetSpecies = gEvolutionTable[species][i].targetSpecies;
						#ifdef EVO_HOLD_ITEM_REMOVAL
							FlagSet(FLAG_REMOVE_EVO_ITEM);
						#endif
					}
					#endif
					break;

				case EVO_HOLD_ITEM_DAY:
					// hold item in param
					#ifdef TIME_ENABLED
					if (heldItem == gEvolutionTable[species][i].param && IsDayTime())
					{
						targetSpecies = gEvolutionTable[species][i].targetSpecies;
						#ifdef EVO_HOLD_ITEM_REMOVAL
							FlagSet(FLAG_REMOVE_EVO_ITEM);
						#endif
					}
					#endif
					break;

				case EVO_MOVE:
					for (j = 0; j < MAX_MON_MOVES; ++j)
					{
						if (gEvolutionTable[species][i].param == mon->moves[j])
						{
							targetSpecies = gEvolutionTable[species][i].targetSpecies;
							break;
						}
					}
					break;

				case EVO_OTHER_PARTY_MON:
					// species to check for in param
					for (j = 0; j < gPlayerPartyCount; ++j)
					{
						if (gPlayerParty[j].species == gEvolutionTable[species][i].param)
						{
							targetSpecies = gEvolutionTable[species][i].targetSpecies;
							break;
						}
					}
					break;

				case EVO_LEVEL_SPECIFIC_TIME_RANGE: ;
					#ifdef TIME_ENABLED
						u8 startTime = (gEvolutionTable[species][i].unknown >> 8) & 0xFF;	//upper byte
						u8 endTime = gEvolutionTable[species][i].unknown & 0xFF;	// lower byte
						if (gEvolutionTable[species][i].param <= level
						&& (gClock.hour >= startTime && gClock.hour < endTime))
							targetSpecies = gEvolutionTable[species][i].targetSpecies;
					#else  // regular level up check
						if (gEvolutionTable[species][i].param <= level)
							targetSpecies = gEvolutionTable[species][i].targetSpecies;
					#endif
					break;

				case EVO_FLAG_SET:
					if (FlagGet(gEvolutionTable[species][i].param))
						targetSpecies = gEvolutionTable[species][i].targetSpecies;
			}
		}
		break;

	case 1:
		for (i = 0; i < EVOS_PER_MON; ++i)
		{
			switch (gEvolutionTable[species][i].method)
			{
			case EVO_TRADE:
				targetSpecies = gEvolutionTable[species][i].targetSpecies;
				break;
			#ifdef EVO_HOLD_ITEM_REMOVAL
			case EVO_TRADE_ITEM:
				if (gEvolutionTable[species][i].param == heldItem)
				{
					heldItem = 0;
					SetMonData(mon, MON_DATA_HELD_ITEM, &heldItem);
					targetSpecies = gEvolutionTable[species][i].targetSpecies;
				}
				break;
			#endif
			}
		}
		break;

	case 2:
	case 3:	// using items
		for (i = 0; i < EVOS_PER_MON; ++i)
		{
			if (gEvolutionTable[species][i].method == EVO_ITEM
			 && gEvolutionTable[species][i].param == evolutionItem)
			{
				if (evolutionItem == ITEM_DAWN_STONE && GetMonGender(mon) != gEvolutionTable[species][i].unknown)
					break;
				targetSpecies = gEvolutionTable[species][i].targetSpecies;
				break;
			}
		}
		break;
	}

	return targetSpecies;
}

void ItemEvolutionRemoval(pokemon_t* mon)
{
	#ifdef EVO_HOLD_ITEM_REMOVAL
		if (FlagGet(FLAG_REMOVE_EVO_ITEM))
		{
			u16 none = 0;
			SetMonData(mon, MON_DATA_HELD_ITEM, &none);
			FlagClear(FLAG_REMOVE_EVO_ITEM);
		}
	#else
		++mon; //So compiler doesn't complain
	#endif
}

bool8 IsLevelUpEvolutionMethod(u8 method)
{
	switch (method) {
		case EVO_LEVEL:
		case EVO_LEVEL_ATK_GT_DEF:
		case EVO_LEVEL_ATK_EQ_DEF:
		case EVO_LEVEL_ATK_LT_DEF:
		case EVO_LEVEL_SILCOON:
		case EVO_LEVEL_CASCOON:
		case EVO_LEVEL_NINJASK:
		case EVO_LEVEL_SHEDINJA:
		case EVO_RAINY_FOGGY_OW:
		case EVO_TYPE_IN_PARTY:
		case EVO_MALE_LEVEL:
		case EVO_FEMALE_LEVEL:
		case EVO_LEVEL_NIGHT:
		case EVO_LEVEL_DAY:
		case EVO_LEVEL_SPECIFIC_TIME_RANGE:
			return TRUE;
		default:
			return FALSE;
	}
}

bool8 IsItemEvolutionMethod(u8 method)
{
	switch (method) {
		case EVO_ITEM:
		case EVO_TRADE_ITEM:
		case EVO_HOLD_ITEM_NIGHT:
		case EVO_HOLD_ITEM_DAY:
			return TRUE;
		default:
			return FALSE;
	}
}

bool8 IsFriendshipEvolutionMethod(u8 method)
{
	switch (method) {
		case EVO_FRIENDSHIP:
		case EVO_FRIENDSHIP_DAY:
		case EVO_FRIENDSHIP_NIGHT:
			return TRUE;
		default:
			return FALSE;
	}
}

bool8 IsOtherEvolutionMethod(u8 method)
{
	switch (method) {
		case EVO_BEAUTY:
		case EVO_TRADE:
		case EVO_MOVE_TYPE:
		case EVO_MAP:
		case EVO_MOVE:
		case EVO_OTHER_PARTY_MON:
		case EVO_FLAG_SET:
			return TRUE;
		default:
			return FALSE;
	}
}

u16 GetMonDevolution(struct Pokemon* mon)
{
	int j, k;
	bool8 found;
	bool8 checkingBackupSpecies = FALSE;
	u16 originalSpecies = GetMonData(mon, MON_DATA_SPECIES, NULL);

	SEARCH_START: ;
	u16 species = originalSpecies;
	found = FALSE;
	for (j = 1; j < NUM_SPECIES; ++j)
	{
		for (k = 0; k < EVOS_PER_MON; ++k)
		{
			if (gEvolutionTable[j][k].method == EVO_MEGA)
			{
				if (gEvolutionTable[j][k].targetSpecies == species && gEvolutionTable[j][k].param != ITEM_NONE)
				{
					originalSpecies = j;
					goto SEARCH_START; //Find base form for Mega and then actually look
				}
			}
			else if (gEvolutionTable[j][k].method == EVO_GIGANTAMAX)
			{
				if (gEvolutionTable[j][k].targetSpecies == species && gEvolutionTable[j][k].param != FALSE)
				{
					originalSpecies = j;
					goto SEARCH_START; //Find base form for Gigantamax and then actually look
				}
			}
			else if (gEvolutionTable[j][k].targetSpecies == species)
			{
				species = j;
				found = TRUE;
				break;
			}
		}

		if (found)
			break;
	}

	u16 dexNum = SpeciesToNationalPokedexNum(species);
	switch(dexNum) {
		#if (defined NATIONAL_DEX_PIKACHU && defined SPECIES_PICHU)
		case NATIONAL_DEX_PIKACHU: //Get's all the special forms
			species = SPECIES_PICHU;
			break;
		#endif
		#if (defined NATIONAL_DEX_VIVILLON && defined SPECIES_SCATTERBUG)
		case NATIONAL_DEX_VIVILLON:
			species = SPECIES_SCATTERBUG;
			break;
		#endif
	}

	if (species != originalSpecies)
		return species;

	if (mon->backupSpecies != SPECIES_NONE && !checkingBackupSpecies) //Only check once
	{
		checkingBackupSpecies = TRUE;
		originalSpecies = mon->backupSpecies;
		goto SEARCH_START;
	}

	return SPECIES_NONE;
}
