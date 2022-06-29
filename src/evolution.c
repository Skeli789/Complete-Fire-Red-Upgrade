#include "defines.h"
#include "defines_battle.h"
#include "../include/battle.h"
#include "../include/field_weather.h"
#include "../include/field_player_avatar.h"
#include "../include/fieldmap.h"
#include "../include/overworld.h"
#include "../include/constants/hold_effects.h"
#include "../include/constants/items.h"
#include "../include/constants/pokedex.h"
#include "../include/constants/pokemon.h"
#include "../include/constants/species.h"

#include "../include/new/catching.h"
#include "../include/new/dns.h"
#include "../include/new/evolution.h"
#include "../include/new/form_change.h"
#include "../include/new/util.h"

/*
evolution.c
	handles old and new evolution methods
*/

u16 GetEvolutionTargetSpecies(struct Pokemon* mon, u8 type, u16 evolutionItem)
{
	u32 i, j;
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
	case EVO_MODE_NORMAL:
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
					if (gEvolutionTable[species][i].unknown && friendship < 220) //Needs High Friendship & Move Type
						break;

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
				
				case EVO_MOVE_MALE:
					if (GetMonGender(mon) != MON_MALE)
						break;
					goto REGULAR_EVO_MOVE;
				
				case EVO_MOVE_FEMALE:
					if (GetMonGender(mon) != MON_FEMALE)
						break;
					//Fallthrough

				case EVO_MOVE:
				REGULAR_EVO_MOVE:
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
					break;

				case EVO_CRITICAL_HIT:
					if (gMain.callback2 == BattleMainCB2) //Only after battles
					{
						for (j = 0; j < PARTY_SIZE; ++j)
						{
							if (mon == &gPlayerParty[j] || mon == &gEnemyParty[j]) //Get the correct mon id
							{
								if (gScored3CritsInBattle & gBitTable[j])
									targetSpecies = gEvolutionTable[species][i].targetSpecies;
								break;
							}
						}
					}
					break;

				case EVO_NATURE_HIGH:
					if (level >= gEvolutionTable[species][i].param
					&& HasHighNature(mon))
						targetSpecies = gEvolutionTable[species][i].targetSpecies;
					break;

				case EVO_NATURE_LOW:
					if (level >= gEvolutionTable[species][i].param
					&& !HasHighNature(mon))
						targetSpecies = gEvolutionTable[species][i].targetSpecies;
					break;

				//case EVO_DAMAGE_LOCATION:

				case EVO_LEVEL_HOLD_ITEM: //Level up to level while holding item
					if (level >= gEvolutionTable[species][i].param && heldItem == gEvolutionTable[species][i].unknown)
					{
						targetSpecies = gEvolutionTable[species][i].targetSpecies;
						#ifdef EVO_HOLD_ITEM_REMOVAL
							FlagSet(FLAG_REMOVE_EVO_ITEM);
						#endif
					}
					break;
			}
		}

		#ifdef SPECIES_SILVALLY
		if (targetSpecies == SPECIES_SILVALLY
		&& ItemId_GetHoldEffect(GetMonData(mon, MON_DATA_HELD_ITEM, NULL)) == ITEM_EFFECT_MEMORY
		&& (!mon->hiddenAbility || gBaseStats[SPECIES_SILVALLY].hiddenAbility == ABILITY_NONE)) //Ensure the new mon will have RKS System
		{
			//Evolve into the Silvally of the correct type
			u8 type = ItemId_GetHoldEffectParam(GetMonData(mon, MON_DATA_HELD_ITEM, NULL));
			if (gTypeToSilvallyForm[type] != SPECIES_NONE)
				targetSpecies = gTypeToSilvallyForm[type];
		}
		#endif

		break;

	case EVO_MODE_TRADE:
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

	case EVO_MODE_ITEM_USE:
	case EVO_MODE_ITEM_CHECK:	// using items
		for (i = 0; i < EVOS_PER_MON; ++i)
		{
			if ((gEvolutionTable[species][i].method == EVO_ITEM
			 || gEvolutionTable[species][i].method == EVO_ITEM_LOCATION
			 || gEvolutionTable[species][i].method == EVO_ITEM_HOLD_ITEM
			 || gEvolutionTable[species][i].method == EVO_ITEM_NIGHT)
			 && gEvolutionTable[species][i].param == evolutionItem)
			{
				if (evolutionItem == ITEM_DAWN_STONE && GetMonGender(mon) != gEvolutionTable[species][i].unknown)
					continue;

				if (gEvolutionTable[species][i].method == EVO_ITEM_LOCATION)
				{
					s16 x, y, behaviour;
					PlayerGetDestCoords(&x, &y);
					behaviour = MapGridGetMetatileBehaviorAt(x, y);

					if (behaviour != gEvolutionTable[species][i].unknown) //Not standing on the correct kind of tile
						continue;				
				}
				else if (gEvolutionTable[species][i].method == EVO_ITEM_HOLD_ITEM)
				{
					if (heldItem != gEvolutionTable[species][i].unknown) //Not holding the correct item
						continue;

					#ifdef EVO_HOLD_ITEM_REMOVAL
					FlagSet(FLAG_REMOVE_EVO_ITEM);
					#endif
				}
				else if (gEvolutionTable[species][i].method == EVO_ITEM_NIGHT)
				{
					if (!IsNightTime())
						continue;
				}

				targetSpecies = gEvolutionTable[species][i].targetSpecies;
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
		case EVO_NATURE_HIGH:
		case EVO_NATURE_LOW:
		case EVO_LEVEL_HOLD_ITEM:
			return TRUE;
		default:
			return FALSE;
	}
}

bool8 IsItemEvolutionMethod(u8 method)
{
	switch (method) {
		case EVO_ITEM:
		case EVO_ITEM_LOCATION:
		case EVO_ITEM_HOLD_ITEM:
		case EVO_TRADE_ITEM:
		case EVO_HOLD_ITEM_NIGHT:
		case EVO_HOLD_ITEM_DAY:
		case EVO_ITEM_NIGHT:
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
		case EVO_MOVE_TYPE:
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
		case EVO_MAP:
		case EVO_MOVE:
		case EVO_MOVE_MALE:
		case EVO_MOVE_FEMALE:
		case EVO_OTHER_PARTY_MON:
		case EVO_FLAG_SET:
		case EVO_CRITICAL_HIT:
		case EVO_DAMAGE_LOCATION:
			return TRUE;
		default:
			return FALSE;
	}
}

bool8 EvolutionMethodRequiresLevelUp(u8 method)
{
	return IsLevelUpEvolutionMethod(method)
		|| IsFriendshipEvolutionMethod(method)
		|| method == EVO_BEAUTY
		|| method == EVO_MOVE_TYPE
		|| method == EVO_MAP
		|| method == EVO_MOVE
		|| method == EVO_MOVE_MALE
		|| method == EVO_MOVE_FEMALE
		|| method == EVO_OTHER_PARTY_MON
		|| method == EVO_FLAG_SET;
}

bool8 HasHighNature(struct Pokemon* mon)
{
	switch (GetNature(mon))
	{
		case NATURE_HARDY:
		case NATURE_BRAVE:
		case NATURE_ADAMANT:
		case NATURE_NAUGHTY:
		case NATURE_DOCILE:
		case NATURE_IMPISH:
		case NATURE_LAX:
		case NATURE_HASTY:
		case NATURE_JOLLY:
		case NATURE_NAIVE:
		case NATURE_RASH:
		case NATURE_SASSY:
		case NATURE_QUIRKY:
			return TRUE;
		default:
			return FALSE;
	}
}

bool8 EvolvesViaScoring3Crits(struct Pokemon* mon)
{
	u16 species = GetMonData(mon, MON_DATA_SPECIES, NULL);
	const struct Evolution* evolutions = gEvolutionTable[species];

	for (u32 i = 0; i < EVOS_PER_MON; ++i)
	{
		if (evolutions[i].method == EVO_NONE) //Most likely end of entries
			break; //Break now to save time
		else if (evolutions[i].method == EVO_CRITICAL_HIT)
			return TRUE;
	}

	return FALSE;
}

static u16 GetDevolution(u16 originalSpecies, u16 backupSpecies)
{
	int j, k;
	bool8 found;
	bool8 checkingBackupSpecies = FALSE;
	u16 dexNum = SpeciesToNationalPokedexNum(originalSpecies);

	switch(dexNum) {
		#if (defined NATIONAL_DEX_PIKACHU && defined SPECIES_PICHU)
		case NATIONAL_DEX_PIKACHU: //Get's all the special forms
			return SPECIES_PICHU;
		#endif
		#if (defined NATIONAL_DEX_VIVILLON && defined SPECIES_SPEWPA)
		case NATIONAL_DEX_VIVILLON:
			return SPECIES_SPEWPA;
		#endif
	}

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

			if (gEvolutionTable[j][k].method == EVO_NONE)
				break; //Likely no more entries, so break here to save time
		}

		if (found)
			break;
	}

	if (species != originalSpecies)
		return species;

	if (backupSpecies != SPECIES_NONE && !checkingBackupSpecies) //Only check once
	{
		checkingBackupSpecies = TRUE;
		originalSpecies = backupSpecies;
		goto SEARCH_START;
	}

	return SPECIES_NONE;
}

u16 GetMonDevolution(struct Pokemon* mon)
{
	return GetDevolution(GetMonData(mon, MON_DATA_SPECIES, NULL), mon->backupSpecies);
}

u8 GetMinimumLevel(u16 species)
{
	u32 i;
	u32 increase = 0;

	while (TRUE)
	{
		u16 devolutionSpecies = GetDevolution(species, SPECIES_NONE);
		if (devolutionSpecies == SPECIES_NONE)
			return 1 + increase; //Can't go any lower

		for (i = 0; i < EVOS_PER_MON; ++i)
		{
			if (gEvolutionTable[devolutionSpecies][i].targetSpecies == species)
			{
				u8 method = gEvolutionTable[devolutionSpecies][i].method;

				if (IsLevelUpEvolutionMethod(method))
					return gEvolutionTable[devolutionSpecies][i].param + increase; //The minimum level is the one that it was when it evolved
				else if (EvolutionMethodRequiresLevelUp(method))
					increase = 1; //Must have leveled up at least once to evolve
			}

			if (gEvolutionTable[devolutionSpecies][i].method == EVO_NONE)
				break; //Likely no more entries, so break here to save time
		}

		species = devolutionSpecies; //Go another round down
	}
}

void CreateShedinja(u16 preEvoSpecies, struct Pokemon* mon)
{
	u32 data = 0;
	if (gEvolutionTable[preEvoSpecies][0].method == EVO_LEVEL_NINJASK
	&& gPlayerPartyCount < PARTY_SIZE
	&& CheckBagHasItem(ITEM_POKE_BALL, 1)) //Must have a standard Poke Ball in the Bag
	{
		s32 i;
		const struct Evolution *evos;
		const struct Evolution *evos2;
		u8 ball = BALL_TYPE_POKE_BALL;

		CopyMon(&gPlayerParty[gPlayerPartyCount], mon, sizeof(struct Pokemon));
		SetMonData(&gPlayerParty[gPlayerPartyCount], MON_DATA_SPECIES, (&gEvolutionTable[preEvoSpecies][1].targetSpecies));
		SetMonData(&gPlayerParty[gPlayerPartyCount], MON_DATA_NICKNAME, (gSpeciesNames[gEvolutionTable[preEvoSpecies][1].targetSpecies]));
		SetMonData(&gPlayerParty[gPlayerPartyCount], MON_DATA_HELD_ITEM, (&data));
		SetMonData(&gPlayerParty[gPlayerPartyCount], MON_DATA_MARKINGS, (&data));
		SetMonData(&gPlayerParty[gPlayerPartyCount], MON_DATA_ENCRYPT_SEPARATOR, (&data));
		SetMonData(&gPlayerParty[gPlayerPartyCount], MON_DATA_POKEBALL, &ball);

		for (i = MON_DATA_COOL_RIBBON; i < MON_DATA_COOL_RIBBON + 5; i++)
			SetMonData(&gPlayerParty[gPlayerPartyCount], i, (&data));
		for (i = MON_DATA_CHAMPION_RIBBON; i <= MON_DATA_FATEFUL_ENCOUNTER; i++)
			SetMonData(&gPlayerParty[gPlayerPartyCount], i, (&data));

		SetMonData(&gPlayerParty[gPlayerPartyCount], MON_DATA_STATUS, (&data));
		data = 0xFF;
		SetMonData(&gPlayerParty[gPlayerPartyCount], MON_DATA_MAIL, (&data));

		CalculateMonStats(&gPlayerParty[gPlayerPartyCount]);
		CalculatePlayerPartyCount();

		// can't match it otherwise, ehh
		evos2 = gEvolutionTable[0];
		evos = evos2 + EVOS_PER_MON * preEvoSpecies;

		GetSetPokedexFlag(SpeciesToNationalPokedexNum(evos[1].targetSpecies), FLAG_SET_SEEN);
		GetSetPokedexFlag(SpeciesToNationalPokedexNum(evos[1].targetSpecies), FLAG_SET_CAUGHT);
		RemoveBagItem(ITEM_POKE_BALL, 1);
	}
}
