#include "defines.h"
#include "defines_battle.h"

#include "../include/constants/pokemon.h"	
#include "../include/constants/items.h"
#include "../include/constants/hold_effects.h"
#include "../include/constants/species.h"
#include "../include/field_weather.h"
#include "../include/battle.h"

extern struct Evolution gEvolutionTable[MAX_NUM_POKEMON][EVOS_PER_MON];

u16 GetEvolutionTargetSpecies(struct Pokemon *mon, u8 type, u16 evolutionItem) {
    int i, j;
    u16 targetSpecies = 0;
    u16 species = GetMonData(mon, MON_DATA_SPECIES, NULL);
    u16 heldItem = GetMonData(mon, MON_DATA_HELD_ITEM, NULL);
    u32 personality = GetMonData(mon, MON_DATA_PERSONALITY, NULL);
    u8 level;
    u16 friendship;
    u8 beauty = GetMonData(mon, MON_DATA_BEAUTY, 0);
    u16 upperPersonality = personality >> 16;
    u8 holdEffect;
	
    if (heldItem == ITEM_ENIGMA_BERRY)
        holdEffect = gSaveBlock1->enigmaBerry.holdEffect;
    else
        holdEffect = ItemId_GetHoldEffect(heldItem);

    if (holdEffect == ITEM_EFFECT_PREVENT_EVOLVE && type != 3)
        return SPECIES_NONE;

    switch (type)
    {
    case 0:
        level = GetMonData(mon, MON_DATA_LEVEL, 0);
        friendship = GetMonData(mon, MON_DATA_FRIENDSHIP, 0);
		
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
					if (Clock->hour >= TIME_DAY_START && Clock->hour < TIME_NIGHT_START && friendship >= 220)
						targetSpecies = gEvolutionTable[species][i].targetSpecies;
				#endif
					break;
					
				case EVO_FRIENDSHIP_NIGHT:
				#ifdef TIME_ENABLED
					if (Clock->hour >= TIME_NIGHT_START && Clock->hour < TIME_DAY_START && friendship >= 220)
						targetSpecies = gEvolutionTable[species][i].targetSpecies;
				#endif
					break;
					
				case EVO_LEVEL:					
					if (gEvolutionTable[species][i].param <= level)
						targetSpecies = gEvolutionTable[species][i].targetSpecies;
					break;
					
				case EVO_LEVEL_ATK_GT_DEF:
					if (gEvolutionTable[species][i].param <= level)
						if (GetMonData(mon, MON_DATA_ATK, 0) > GetMonData(mon, MON_DATA_DEF, 0))
							targetSpecies = gEvolutionTable[species][i].targetSpecies;
					break;
					
				case EVO_LEVEL_ATK_EQ_DEF:
					if (gEvolutionTable[species][i].param <= level)
						if (GetMonData(mon, MON_DATA_ATK, 0) == GetMonData(mon, MON_DATA_DEF, 0))
							targetSpecies = gEvolutionTable[species][i].targetSpecies;
					break;
					
				case EVO_LEVEL_ATK_LT_DEF:
					if (gEvolutionTable[species][i].param <= level)
						if (GetMonData(mon, MON_DATA_ATK, 0) < GetMonData(mon, MON_DATA_DEF, 0))
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
					
				case EVO_RAINY_OW:
					if (gEvolutionTable[species][i].param <= level
						&& (GetCurrentWeather() == WEATHER_RAIN_LIGHT
						|| GetCurrentWeather() == WEATHER_RAIN_MED
						|| GetCurrentWeather() == WEATHER_RAIN_HEAVY))
						targetSpecies = gEvolutionTable[species][i].targetSpecies;
					break;
					
				case EVO_HOLD_ITEM_DAY:
					// hold item in param
					#ifdef TIME_ENABLED
					if ((Clock->hour >= TIME_DAY_START && Clock->hour < TIME_NIGHT_START)
					&& heldItem == gEvolutionTable[species][i].param)
					{
						targetSpecies = gEvolutionTable[species][i].targetSpecies;
						#ifdef EVO_HOLD_ITEM_REMOVAL
							FlagSet(FLAG_REMOVE_EVO_ITEM);
						#endif
					}
					#endif
					break;
					
				case EVO_HOLD_ITEM_NIGHT:
					#ifdef TIME_ENABLED
					if (heldItem == gEvolutionTable[species][i].param)
					{
						targetSpecies = gEvolutionTable[species][i].targetSpecies;
						#ifdef EVO_HOLD_ITEM_REMOVAL
							FlagSet(FLAG_REMOVE_EVO_ITEM);
						#endif
					}
					#endif
					break;
					
				case EVO_MAP:
					if (gSaveBlock1->location.mapGroup == gEvolutionTable[species][i].param
					&& gSaveBlock1->location.mapNum == gEvolutionTable[species][i].unknown)
						targetSpecies = gEvolutionTable[species][i].targetSpecies;
					break;
					
				case EVO_MOVE:
					for (j = 0; j < MAX_MON_MOVES; ++j)
					{
						if (gEvolutionTable[species][i].param == GetMonData(mon, MON_DATA_MOVE1 + j, NULL))
						{
							targetSpecies = gEvolutionTable[species][i].targetSpecies;
							break;
						}
					}
					break;
					
				case EVO_MOVE_TYPE:	// expanded for custom evolutions of any move type
					// move type to know in param
					for (j = 0; j < MAX_MON_MOVES; ++j)
					{
						if (gBattleMoves[GetMonData(mon, MON_DATA_MOVE1 + j, NULL)].type == gEvolutionTable[species][i].param)
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
						if (GetMonData(&gPlayerParty[j],MON_DATA_SPECIES,NULL) == gEvolutionTable[species][i].param)
						{
							targetSpecies = gEvolutionTable[species][i].targetSpecies;
							break;
						}
					}
					break;
					
				case EVO_TYPE_IN_PARTY:
					// type in param
					// level in unknown
					// eg. Pancham table would be:
					//		[SPECIES_PANCHAM] = {{EVO_TYPE_IN_PARTY, TYPE_DARK, SPECIES_PANGORO, 32}},
					if (level >= gEvolutionTable[species][i].unknown)
					{
						u8 typeCheck = gEvolutionTable[species][i].param;
						if (typeCheck >= NUMBER_OF_MON_TYPES)
							break;
						for (j = 0; j < gPlayerPartyCount; ++j)
						{
							if (gBaseStats[GetMonData(&gPlayerParty[j],MON_DATA_SPECIES,NULL)].type1 == typeCheck
							|| gBaseStats[GetMonData(&gPlayerParty[j],MON_DATA_SPECIES,NULL)].type2 == typeCheck)
							{
								targetSpecies = gEvolutionTable[species][i].targetSpecies;
								break;
							}
						}
					}
					break;
					
				case EVO_MALE_LEVEL:
					if (gEvolutionTable[species][i].param <= level && GetMonGender(mon) == MON_MALE)
						targetSpecies = gEvolutionTable[species][i].targetSpecies;
					break;
				
				case EVO_FEMALE_LEVEL:
					if (gEvolutionTable[species][i].param <= level && GetMonGender(mon) == MON_FEMALE)
						targetSpecies = gEvolutionTable[species][i].targetSpecies;
					break;
					
				case EVO_LEVEL_DAY:
					#ifdef TIME_ENABLED
						if (gEvolutionTable[species][i].param <= level &&
						(Clock->hour >= TIME_DAY_START && Clock->hour < TIME_NIGHT_START))
							targetSpecies = gEvolutionTable[species][i].targetSpecies;
					#else  // regular level up check
						if (gEvolutionTable[species][i].param <= level)
							targetSpecies = gEvolutionTable[species][i].targetSpecies;
					#endif
					break;
				
				case EVO_LEVEL_NIGHT:
					#ifdef TIME_ENABLED
						if (gEvolutionTable[species][i].param <= level &&
						(Clock->hour >= TIME_NIGHT_START && Clock->hour < TIME_DAY_START))
							targetSpecies = gEvolutionTable[species][i].targetSpecies;
					#else  // regular level up check
						if (gEvolutionTable[species][i].param <= level)
							targetSpecies = gEvolutionTable[species][i].targetSpecies;
					#endif
					break;
					
				case EVO_LEVEL_SPECIFIC_TIME_RANGE:
					{
					#ifdef TIME_ENABLED
						u8 startTime = (gEvolutionTable[species][i].unknown >> 8) & 0xFF;	//upper byte
						u8 endTime = gEvolutionTable[species][i].unknown & 0xFF;	// lower byte
						if (gEvolutionTable[species][i].param <= level &&
						(Clock->hour >= startTime && Clock->hour < endTime))
							targetSpecies = gEvolutionTable[species][i].targetSpecies;
					#else  // regular level up check
						if (gEvolutionTable[species][i].param <= level)
							targetSpecies = gEvolutionTable[species][i].targetSpecies;
					#endif
					}
					break;
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
            case EVO_TRADE_ITEM:
                if (gEvolutionTable[species][i].param == heldItem)
                {
                    heldItem = 0;
                    SetMonData(mon, MON_DATA_HELD_ITEM, &heldItem);
                    targetSpecies = gEvolutionTable[species][i].targetSpecies;
                }
                break;
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
};


void ItemEvolutionRemoval(pokemon_t* mon) {
	#ifdef EVO_HOLD_ITEM_REMOVAL
		if (FlagGet(FLAG_REMOVE_EVO_ITEM))
		{
			u32 heldItem = 0;
			SetMonData(mon, MON_DATA_HELD_ITEM, &heldItem);
			FlagClear(FLAG_REMOVE_EVO_ITEM);
		}
	#endif
	
};




