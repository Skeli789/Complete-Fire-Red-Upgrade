#include "defines.h"
#include "../include/daycare.h"
#include "../include/pokemon.h"
#include "../include/pokemon_storage_system.h"
#include "../include/random.h"
#include "../include/constants/abilities.h"
#include "../include/constants/items.h"
#include "../include/constants/moves.h"
#include "../include/constants/species.h"
#include "../include/constants/pokedex.h"
#include "../include/constants/hold_effects.h"	

#include "../include/new/catching.h"
#include "../include/new/daycare.h"
#include "../include/new/item.h"

#define sHatchedEggFatherMoves ((u16*) 0x202455C)
#define sHatchedEggMotherMoves ((u16*)0x2024580)
#define sHatchedEggFinalMoves ((u16*) 0x2024564)
#define sHatchedEggLevelUpMoves ((u16*) 0x20244F8)

#define EGG_LVL_UP_MOVES_ARRAY_COUNT 50
#define gTMHMMoves ((const u16*) *((u32*) 0x8125A8C))

extern u8 GetLevelUpMovesBySpecies(u16 species, u16* moves);
extern u16 GetItemIdFromTmId(u8 tmId);

void BuildEggMoveset(struct Pokemon* egg, struct BoxPokemon* father, struct BoxPokemon* mother);
static u8 GetEggStepsToSubtract(void);

/*Priority: 
1. Volt Tackle
2. Mother's Egg Moves
3. Father's Egg Moves 
4. Father TM and HM Moves
5. Inherited Level-Up Moves 
6. Baby's Default Moveset.
*/

// called from GiveEggFromDaycare
void BuildEggMoveset(struct Pokemon* egg, struct BoxPokemon* father, struct BoxPokemon* mother)
{
	
	u16 eggSpecies = egg->species;
    u32 numLevelUpMoves, numEggMoves, numSharedParentMoves;
    u32 i, j;
	
	u16 sHatchedEggEggMoves[EGG_MOVES_ARRAY_COUNT] = {0};

    numSharedParentMoves = 0;
    for (i = 0; i < MAX_MON_MOVES; ++i)
    {
        sHatchedEggMotherMoves[i] = 0;
        sHatchedEggFatherMoves[i] = 0;
        sHatchedEggFinalMoves[i] = 0;
    }
    //for (i = 0; i < EGG_MOVES_ARRAY_COUNT; ++i)
    //    sHatchedEggEggMoves[i] = 0;
    for (i = 0; i < EGG_LVL_UP_MOVES_ARRAY_COUNT; ++i)
        sHatchedEggLevelUpMoves[i] = 0;

    numLevelUpMoves = GetLevelUpMovesBySpecies(eggSpecies, sHatchedEggLevelUpMoves);
    for (i = 0; i < MAX_MON_MOVES; ++i)
    {
        sHatchedEggFatherMoves[i] = GetBoxMonData(father, MON_DATA_MOVE1 + i, NULL);
        sHatchedEggMotherMoves[i] = GetBoxMonData(mother, MON_DATA_MOVE1 + i, NULL);
    }

    numEggMoves = GetEggMoves(egg, sHatchedEggEggMoves);
	
	//Shared Moves Between Parents
    for (i = 0; i < MAX_MON_MOVES; ++i)
    {
        for (j = 0; j < MAX_MON_MOVES && sHatchedEggFatherMoves[i] != MOVE_NONE; j++)
        {
            if (sHatchedEggFatherMoves[i] == sHatchedEggMotherMoves[j])
                sHatchedEggFinalMoves[numSharedParentMoves++] = sHatchedEggFatherMoves[i];
        }
    }
	
	//Try Assign Shared Moves to Baby
    for (i = 0; i < MAX_MON_MOVES; ++i)
    {
        for (j = 0; j < numLevelUpMoves && sHatchedEggFinalMoves[i] != MOVE_NONE; ++j)
        {
            if (sHatchedEggFinalMoves[i] == sHatchedEggLevelUpMoves[j])
            {
                if (GiveMoveToMon(egg, sHatchedEggFinalMoves[i]) == 0xFFFF)
                    DeleteFirstMoveAndGiveMoveToMon(egg, sHatchedEggFinalMoves[i]);
                break;
            }
        }
    }


#ifdef FATHER_PASSES_TMS
    for (i = 0; i < MAX_MON_MOVES; ++i)
    {	
		if (sHatchedEggFatherMoves[i] != MOVE_NONE)
		{
			for (j = 0; j < NUM_TMSHMS; ++j)	//loop through tm indices
			{
				if (sHatchedEggFatherMoves[i] == gTMHMMoves[j] && CanMonLearnTMHM(egg, j))
				{
					if (GiveMoveToMon(egg, sHatchedEggFatherMoves[i]) == 0xFFFF)
						DeleteFirstMoveAndGiveMoveToMon(egg, sHatchedEggFatherMoves[i]);								
				}			
			}
		}
	}
#endif


	//Father Egg Moves
    for (i = 0; i < MAX_MON_MOVES; ++i)
    {
        if (sHatchedEggFatherMoves[i] != MOVE_NONE)
        {
            for (j = 0; j < numEggMoves; ++j)
            {
                if (sHatchedEggFatherMoves[i] == sHatchedEggEggMoves[j])
                {
                    if (GiveMoveToMon(egg, sHatchedEggFatherMoves[i]) == 0xFFFF)
                        DeleteFirstMoveAndGiveMoveToMon(egg, sHatchedEggFatherMoves[i]);
                    break;
                }
            }
        }
    }

	//Mother Egg Moves
    for (i = 0; i < MAX_MON_MOVES; ++i)
    {
        if (sHatchedEggMotherMoves[i] != MOVE_NONE)
        {
            for (j = 0; j < numEggMoves; ++j)
            {
                if (sHatchedEggMotherMoves[i] == sHatchedEggEggMoves[j])
                {
                    if (GiveMoveToMon(egg, sHatchedEggMotherMoves[i]) == 0xFFFF)
                        DeleteFirstMoveAndGiveMoveToMon(egg, sHatchedEggMotherMoves[i]);
                    break;
                }
            }
        }
    }

	//Volt Tackle
	if (eggSpecies == SPECIES_PICHU)
	{
		if (GetBoxMonData(mother, MON_DATA_HELD_ITEM, NULL) == ITEM_LIGHT_BALL 
		||  GetBoxMonData(father, MON_DATA_HELD_ITEM, NULL) == ITEM_LIGHT_BALL)
		{
			if (GiveMoveToMon(egg, MOVE_VOLTTACKLE) == 0xFFFF)
				DeleteFirstMoveAndGiveMoveToMon(egg, MOVE_VOLTTACKLE);
		}
	}
}


s32 GetSlotToInheritNature(struct DayCare* daycare)
{
	int i;
	u8 numWithEverstone = 0;
    s32 slot = -1;

	for (i = 0; i < DAYCARE_MON_COUNT; ++i)
	{
		if (GetBoxMonData(&daycare->mons[i].mon, MON_DATA_HELD_ITEM, NULL) == ITEM_EVERSTONE)
		{
			slot = i;
			++numWithEverstone;
		}
	}
	
	if (numWithEverstone < 2)
		return slot;

    return Random() % 2; //Either nature b/c both hold everstone
}


u16 DetermineEggSpeciesAndParentSlots(struct DayCare* daycare, u8* parentSlots)
{
    u16 i;
    u16 species[DAYCARE_MON_COUNT];
    u16 eggSpecies;

    // Determine which of the daycare mons is the mother and father of the egg.
    // The 0th index of the parentSlots array is considered the mother slot, and the
    // 1st index is the father slot.
    for (i = 0; i < DAYCARE_MON_COUNT; ++i)
    {
        species[i] = GetBoxMonData(&daycare->mons[i].mon, MON_DATA_SPECIES, NULL);
        if (species[i] == SPECIES_DITTO)
        {
            parentSlots[0] = i ^ 1;
            parentSlots[1] = i;
        }
        else if (GetBoxMonGender(&daycare->mons[i].mon) == MON_FEMALE)
        {
            parentSlots[0] = i;
            parentSlots[1] = i ^ 1;
        }
    }

    eggSpecies = GetEggSpecies(species[parentSlots[0]]);
	switch(SpeciesToNationalPokedexNum(eggSpecies))
	{
		case NATIONAL_DEX_NIDORAN_F:
			if (daycare->offspringPersonality & 0x8000)
				eggSpecies = SPECIES_NIDORAN_M;
			break;
			
		case NATIONAL_DEX_ILLUMISE:
			if (daycare->offspringPersonality & 0x8000)
				eggSpecies = SPECIES_VOLBEAT;
			break;
			
		case NATIONAL_DEX_MANAPHY:
			eggSpecies = SPECIES_PHIONE;
			break;
			
		case NATIONAL_DEX_ROTOM:
			eggSpecies = SPECIES_ROTOM;
			break;
			
		case NATIONAL_DEX_FURFROU:
			eggSpecies = SPECIES_FURFROU;
	}

    // Make Ditto the "mother" slot if the other daycare mon is male/genderless).
    if (species[parentSlots[1]] == SPECIES_DITTO && GetBoxMonGender(&daycare->mons[parentSlots[0]].mon) != MON_FEMALE)
    {
        u8 temp = parentSlots[1];
        parentSlots[1] = parentSlots[0];
        parentSlots[0] = temp;
    }

    return eggSpecies;
}



enum {
	HpIv = 0,
	AtkIv,
	DefIv,
	SpdIv,
	SpAtkIv,
	SpDefIv,
};


static u8 CheckPowerItem(u16 item) {
	u8 itemEffect = ItemId_GetHoldEffect(item);
	u8 itemQuality = ItemId_GetHoldEffectParam(item);
	
	if (itemEffect != ITEM_EFFECT_MACHO_BRACE)
		return 0xFF;
	
	switch (itemQuality)
	{
		case QUALITY_POWER_BRACER:
			return HpIv;
			
		case QUALITY_POWER_BELT:
			return AtkIv;
			
		case QUALITY_POWER_LENS:
			return DefIv;
			
		case QUALITY_POWER_BAND:
			return SpdIv;
			
		case QUALITY_POWER_ANKLET:
			return SpAtkIv;
			
		case QUALITY_POWER_WEIGHT:
			return SpDefIv;
			
		default:
			return 0xFF;
	}
};




static void InheritIVs(struct Pokemon *egg, struct DayCare *daycare)
{
	u8 i, numIVs, iv;
	u16 items[2];

	items[0] = GetBoxMonData(&daycare->mons[0].mon, MON_DATA_HELD_ITEM, NULL);
	items[1] = GetBoxMonData(&daycare->mons[1].mon, MON_DATA_HELD_ITEM, NULL);
		
	// get number of IVs to inherit from either parent
	if (items[0] == ITEM_DESTINY_KNOT || items[1] == ITEM_DESTINY_KNOT)
		numIVs = 5;
	else
		numIVs = 3;

	u8 selectedIvs[numIVs];
	//u8 availableIVs[NUM_STATS];
	u8 whichParent[numIVs];
		
	// initiate first 1 or 2 IV slots with power items from either or both parents
	u8 initVal = 0;
	u8 powerResult = 0xFF;
	
	for (i = 0; i < DAYCARE_MON_COUNT; ++i)
	{
		powerResult = CheckPowerItem(items[i]);	// check if parent i has a power item
		if (powerResult == 0xFF)
			continue;	// this parent does not have a power item, check next
		
		// parent has a power item, save index to IV slot
		if (items[0] == items[1])
		{
			// both parents have same power item, choose parent at random
			whichParent[i] = Random() % 2;	// 0 or 1
			selectedIvs[i] = powerResult;
			initVal++;
			break;	//no need to do second loop since both have same item
		}
		else
		{
			whichParent[i] = i;
			selectedIvs[i] = powerResult;
			initVal++;
		}
	}

	// randomize the remaining IV indices with unique values
	bool8 unique;
	for (i = initVal; i < ARRAY_COUNT(selectedIvs); ++i)
	{
		iv = Random() % NUM_STATS;
		
		// check if index is unique
		unique = TRUE;
		u8 j = 0;
		while (unique == TRUE)
		{
			if (iv == selectedIvs[j])
				unique = FALSE;
			
			if (j == NUM_STATS)
			{
				selectedIvs[i] = iv;
				break;
			}
			j++;
		}
		whichParent[i] = Random() % 2;
	}
	
	
	
/*			original routine
    // Initialize a list of IV indices.
    for (i = 0; i < NUM_STATS; ++i)
    {
        availableIVs[i] = i;
    }

    // Select the num IVs that will be inherited.
    for (i = 0; i < ARRAY_COUNT(selectedIvs); ++i)
    {
        // Randomly pick an IV from the available list.
        selectedIvs[i] = availableIVs[Random() % (NUM_STATS - i)];

        // Remove the selected IV index from the available IV indices.
        RemoveIVIndexFromList(availableIVs, i);
    }

    // Determine which parent each of the selected IVs should inherit from.
    for (i = 0; i < ARRAY_COUNT(selectedIvs); ++i)
    {
        whichParent[i] = Random() % 2;
    }
	
*/

    // Set each of inherited IVs on the egg mon.
    for (i = 0; i < ARRAY_COUNT(selectedIvs); ++i)
    {
        switch (selectedIvs[i])
        {
            case HpIv:
                iv = GetBoxMonData(&daycare->mons[whichParent[i]].mon, MON_DATA_HP_IV, NULL);
                SetMonData(egg, MON_DATA_HP_IV, &iv);
                break;
				
            case AtkIv:
                iv = GetBoxMonData(&daycare->mons[whichParent[i]].mon, MON_DATA_ATK_IV, NULL);
                SetMonData(egg, MON_DATA_ATK_IV, &iv);
                break;
				
            case DefIv:
                iv = GetBoxMonData(&daycare->mons[whichParent[i]].mon, MON_DATA_DEF_IV, NULL);
                SetMonData(egg, MON_DATA_DEF_IV, &iv);
                break;
				
            case SpdIv:
                iv = GetBoxMonData(&daycare->mons[whichParent[i]].mon, MON_DATA_SPEED_IV, NULL);
                SetMonData(egg, MON_DATA_SPEED_IV, &iv);
                break;
				
            case SpAtkIv:
                iv = GetBoxMonData(&daycare->mons[whichParent[i]].mon, MON_DATA_SPATK_IV, NULL);
                SetMonData(egg, MON_DATA_SPATK_IV, &iv);
                break;
				
            case SpDefIv:
                iv = GetBoxMonData(&daycare->mons[whichParent[i]].mon, MON_DATA_SPDEF_IV, NULL);
                SetMonData(egg, MON_DATA_SPDEF_IV, &iv);
                break;
        }
    }
};


static void AlterEggSpeciesWithIncenseItem(u16 *species, struct DayCare *daycare)
{
	u16 motherItem = GetBoxMonData(&daycare->mons[0].mon, MON_DATA_HELD_ITEM, NULL);
	u16 fatherItem = GetBoxMonData(&daycare->mons[1].mon, MON_DATA_HELD_ITEM, NULL);   
	AlterSpeciesWithIncenseItems(species, motherItem, fatherItem);
}

void AlterSpeciesWithIncenseItems(u16* species, u16 motherItem, u16 fatherItem)
{
	// if neither parent holding incense, force 2nd evo species
	switch (*species)
	{
		case SPECIES_WYNAUT:
			if (motherItem != ITEM_LAX_INCENSE && fatherItem != ITEM_LAX_INCENSE)
				*species = SPECIES_WOBBUFFET;
			break;
			
		case SPECIES_AZURILL:
			if (motherItem != ITEM_SEA_INCENSE && fatherItem != ITEM_SEA_INCENSE)
				*species = SPECIES_MARILL;
			break;
			
		case SPECIES_MUNCHLAX:
			if (motherItem != ITEM_FULL_INCENSE && fatherItem != ITEM_FULL_INCENSE)
				*species = SPECIES_SNORLAX;
			break;
			
		case SPECIES_MIME_JR:
			if (motherItem != ITEM_ODD_INCENSE && fatherItem != ITEM_ODD_INCENSE)
				*species = SPECIES_MR_MIME;
			break;
			
		case SPECIES_CHINGLING:
			if (motherItem != ITEM_PURE_INCENSE && fatherItem != ITEM_PURE_INCENSE)
				*species = SPECIES_CHIMECHO;
			break;
			
		case SPECIES_BONSLY:
			if (motherItem != ITEM_ROCK_INCENSE && fatherItem != ITEM_ROCK_INCENSE)
				*species = SPECIES_SUDOWOODO;
			break;
			
		case SPECIES_BUDEW:
			if (motherItem != ITEM_ROSE_INCENSE && fatherItem != ITEM_ROSE_INCENSE)
				*species = SPECIES_ROSELIA;
			break;
			
		case SPECIES_MANTYKE:
			if (motherItem != ITEM_WAVE_INCENSE && fatherItem != ITEM_WAVE_INCENSE)
				*species = SPECIES_MANTINE;
			break;
			
		case SPECIES_HAPPINY:
			if (motherItem != ITEM_LUCK_INCENSE && fatherItem != ITEM_LUCK_INCENSE)
				*species = SPECIES_CHANSEY;
			break;
			
		default:
			break;
	}
};

/* ORIGINAL	
void AlterEggSpeciesWithIncenseItem(u16 *species, struct DayCare *daycare) {
	u16 motherItem, fatherItem;
    if (*species == SPECIES_WYNAUT || *species == SPECIES_AZURILL)
    {
        motherItem = GetBoxMonData(&daycare->mons[0].mon, MON_DATA_HELD_ITEM);
        fatherItem = GetBoxMonData(&daycare->mons[1].mon, MON_DATA_HELD_ITEM);
        if (*species == SPECIES_WYNAUT && motherItem != ITEM_LAX_INCENSE && fatherItem != ITEM_LAX_INCENSE)
        {
            *species = SPECIES_WOBBUFFET;
        }

        if (*species == SPECIES_AZURILL && motherItem != ITEM_SEA_INCENSE && fatherItem != ITEM_SEA_INCENSE)
        {
            *species = SPECIES_MARILL;
        }
    }
};
*/


static void InheritPokeBall(struct Pokemon *egg, struct DayCare *daycare)
{
	u8 parent = 0;	// mother by default
	u8 parentBall;
	
	u16 motherSpecies = GetBoxMonData(&daycare->mons[0].mon, MON_DATA_SPECIES, NULL);
	u16 fatherSpecies = GetBoxMonData(&daycare->mons[1].mon, MON_DATA_SPECIES, NULL);
	
	// gen 7 same species checks
	if (motherSpecies == SPECIES_DITTO)
		parent = 1;	// gen 7 ditto check -> male or non-gendered mon with ditto (mother) makes pokemon inherit from father
	else if (motherSpecies == fatherSpecies)
		parent = Random() % 2;	// same parent species -> pokeball inherited randomly
	
	// get poke ball ID
	parentBall = GetBoxMonData(&daycare->mons[parent].mon, MON_DATA_POKEBALL, NULL);
	
	// master ball and cherish ball become poke ball
	#ifndef INHERIT_MASTER_CHERISH_BALL
	if (parentBall == BALL_TYPE_MASTER_BALL || parentBall == BALL_TYPE_CHERISH_BALL)
		parentBall = BALL_TYPE_POKE_BALL;
	#endif
	
	SetMonData(egg, MON_DATA_POKEBALL, &parentBall);
};



static void SetInitialEggData(struct Pokemon *mon, u16 species, struct DayCare *daycare)
{
    u32 personality;
    u8 ball;
    u8 metLevel;
    u8 language;

    personality = daycare->offspringPersonality;
    CreateMon(mon, species, EGG_HATCH_LEVEL, 0x20, TRUE, personality, FALSE, 0);
    metLevel = 0;
	
    //ball = ITEM_POKE_BALL;
	ball = BALL_TYPE_POKE_BALL;
	
    language = LANGUAGE_JAPANESE;
    SetMonData(mon, MON_DATA_POKEBALL, &ball);
    SetMonData(mon, MON_DATA_NICKNAME, (const void*) 0x825F83E);
    SetMonData(mon, MON_DATA_FRIENDSHIP, &gBaseStats[species].eggCycles);
    SetMonData(mon, MON_DATA_MET_LEVEL, &metLevel);
    SetMonData(mon, MON_DATA_LANGUAGE, &language);
}



// Decide features to inherit
void GiveEggFromDaycare(struct DayCare *daycare)
{
    struct Pokemon egg;
    u16 species;
    u8 parentSlots[2]; // 0th index is "mother" daycare slot, 1st is "father"
    bool8 isEgg;

    species = DetermineEggSpeciesAndParentSlots(daycare, parentSlots);
    AlterEggSpeciesWithIncenseItem(&species, daycare);
    SetInitialEggData(&egg, species, daycare);	// sets base data (ball, met level, lang, etc)
    InheritIVs(&egg, daycare);	// destiny knot check
	InheritPokeBall(&egg, daycare);
    BuildEggMoveset(&egg, &daycare->mons[parentSlots[1]].mon, &daycare->mons[parentSlots[0]].mon);

    isEgg = TRUE;
    SetMonData(&egg, MON_DATA_IS_EGG, &isEgg);
    
	//gPlayerParty[PARTY_SIZE - 1] = egg;
    Memcpy(&gPlayerParty[5], &egg, 100);
	
	CompactPartySlots();
    CalculatePlayerPartyCount();
    RemoveEggFromDayCare(daycare);
};



void TriggerPendingDaycareEgg(struct DayCare *daycare)
{
    s32 natureSlot;
    s32 natureTries = 0;

    SeedRng2(gMain.vblankCounter2);
	
    natureSlot = GetSlotToInheritNature(daycare);	// updated nature slot check

    if (natureSlot < 0)
    {
        daycare->offspringPersonality = (Random2() << 0x10) | ((Random() % 0xfffe) + 1);
    }
    else
    {
        u8 wantedNature = GetNatureFromPersonality(GetBoxMonData(&daycare->mons[natureSlot].mon, MON_DATA_PERSONALITY, NULL));
        u32 personality;

        do
        {
            personality = (Random2() << 0x10) | (Random());
            if (wantedNature == GetNatureFromPersonality(personality) && personality != 0)
                break; // we found a personality with the same nature

            natureTries++;
        } while (natureTries <= 2400);

        daycare->offspringPersonality = personality;
    }

    FlagSet(FLAG_PENDING_DAYCARE_EGG);
};



// ability inheritance
void CreatedHatchedMon(struct Pokemon *egg, struct Pokemon *temp)
{
    u16 species;
    u32 personality, pokerus;
    u8 i, friendship, language, gameMet, markings, hidden;
    u16 moves[4];
    u32 ivs[NUM_STATS];


    species = GetMonData(egg, MON_DATA_SPECIES, NULL);

    for (i = 0; i < 4; ++i)
    {
        moves[i] = GetMonData(egg, MON_DATA_MOVE1 + i, NULL);
    }

    personality = GetMonData(egg, MON_DATA_PERSONALITY, NULL);

    for (i = 0; i < NUM_STATS; ++i)
    {
        ivs[i] = GetMonData(egg, MON_DATA_HP_IV + i, NULL);
    }

    language = GetMonData(egg, MON_DATA_LANGUAGE, NULL);
    gameMet = GetMonData(egg, MON_DATA_MET_GAME, NULL);
    markings = GetMonData(egg, MON_DATA_MARKINGS, NULL);
    pokerus = GetMonData(egg, MON_DATA_POKERUS, NULL);
	
    //obedience = GetMonData(egg, MON_DATA_OBEDIENCE, NULL);
	hidden = GetMonData(egg, MON_DATA_ALT_ABILITY, NULL);

    CreateMon(temp, species, EGG_HATCH_LEVEL, 32, TRUE, personality, 0, 0);

    for (i = 0; i < 4; ++i)
    {
        SetMonData(temp, MON_DATA_MOVE1 + i,  &moves[i]);
    }

    for (i = 0; i < NUM_STATS; ++i)
    {
        SetMonData(temp, MON_DATA_HP_IV + i,  &ivs[i]);
    }

    language = GAME_LANGUAGE;
    SetMonData(temp, MON_DATA_LANGUAGE, &language);
    SetMonData(temp, MON_DATA_MET_GAME, &gameMet);
    SetMonData(temp, MON_DATA_MARKINGS, &markings);

    friendship = 120;
    SetMonData(temp, MON_DATA_FRIENDSHIP, &friendship);
    SetMonData(temp, MON_DATA_POKERUS, &pokerus);
    
	//SetMonData(temp, MON_DATA_OBEDIENCE, &obedience);
	SetMonData(temp, MON_DATA_ALT_ABILITY, &hidden);

    *egg = *temp;
};


void SubtractEggSteps(u32 steps, struct Pokemon* mon)
{
	u8 toSub = GetEggStepsToSubtract();
	
	if (steps >= toSub)
		steps -= toSub;
	else
		steps -= 1;

	mon->friendship = steps;
}


static u8 GetEggStepsToSubtract(void)
{
    u8 count, i;
    for (count = CalculatePlayerPartyCount(), i = 0; i < count; ++i)
    {
        if (!GetMonData(&gPlayerParty[i], MON_DATA_SANITY_IS_EGG, NULL))
        {
            u8 ability = GetPartyAbility(&gPlayerParty[i]);
            if (ability == ABILITY_MAGMAARMOR || ability == ABILITY_FLAMEBODY)
                return 2;
        }
    }
    return 1;
}
