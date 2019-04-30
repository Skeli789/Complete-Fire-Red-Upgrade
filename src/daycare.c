#include "defines.h"
#include "../include/daycare.h"
#include "../include/constants/moves.h"
#include "../include/constants/species.h"
#include "../include/constants/items.h"
#include "../include/pokemon.h"
#include "../include/pokemon_storage_system.h"

#define sHatchedEggFatherMoves ((u16*) 0x202455C)
#define sHatchedEggMotherMoves ((u16*)0x2024580)
#define sHatchedEggFinalMoves ((u16*) 0x2024564)
#define sHatchedEggLevelUpMoves ((u16*) 0x20244F8)

#define EGG_LVL_UP_MOVES_ARRAY_COUNT 50
#define EGG_MOVES_ARRAY_COUNT 50

extern u8 GetLevelUpMovesBySpecies(u16 species, u16* moves);

void BuildEggMoveset(struct Pokemon* egg, struct BoxPokemon* father, struct BoxPokemon* mother);

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
	//Father TMs
    for (i = 0; i < MAX_MON_MOVES; ++i)
    {
        if (sHatchedEggFatherMoves[i] != MOVE_NONE)
        {
            for (j = 0; j < NUM_TECHNICAL_MACHINES + NUM_HIDDEN_MACHINES; ++j)
            {
                if (sHatchedEggFatherMoves[i] == ItemIdToBattleMoveId(ITEM_TM01_FOCUS_PUNCH + j) && CanMonLearnTMHM(egg, j))
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
	switch(eggSpecies) {
		case SPECIES_NIDORAN_F:
			if (daycare->offspringPersonality & 0x8000)
				eggSpecies = SPECIES_NIDORAN_M;
			break;
		case SPECIES_ILLUMISE:
			if (daycare->offspringPersonality & 0x8000)
				eggSpecies = SPECIES_VOLBEAT;
			break;
		case SPECIES_MANAPHY:
			eggSpecies = SPECIES_PHIONE;
			break;
		case SPECIES_ROTOM_HEAT:
		case SPECIES_ROTOM_WASH:
		case SPECIES_ROTOM_FROST:
		case SPECIES_ROTOM_FAN:
		case SPECIES_ROTOM_MOW:
			eggSpecies = SPECIES_ROTOM;
			break;
		case SPECIES_FURFROU_HEART:
		case SPECIES_FURFROU_DIAMOND:
		case SPECIES_FURFROU_STAR:
		case SPECIES_FURFROU_PHAROAH:
		case SPECIES_FURFROU_KABUKI:
		case SPECIES_FURFROU_LA_REINE:
		case SPECIES_FURFROU_MATRON:
		case SPECIES_FURFROU_DANDY:
		case SPECIES_FURFROU_DEBUTANTE:
			eggSpecies = SPECIES_FURFROU;
	}

    // Make Ditto the "mother" slot if the other daycare mon is male.
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


u8 CheckPowerItem(u16 item) {
	//u16 item = GetBoxMonData(&daycare->mons[parent].mon, MON_DATA_HELD_ITEM, NULL);
	
	// use hold effect / hold parameter instead?
	switch (item)
	{
		case ITEM_POWER_BRACER:
			return HpIv;
		case ITEM_POWER_BELT:
			return AtkIv;
		case ITEM_POWER_LENS:
			return DefIv;
		case ITEM_POWER_BAND:
			return SpdIv;
		case ITEM_POWER_ANKLET:
			return SpAtkIv;
		case ITEM_POWER_WEIGHT:
			return SpDefIv;
		default:
			return 0xFF;
	}
};




void InheritIVs(struct Pokemon *egg, struct DayCare *daycare) {
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
			continue;	// parent does not have a power item
		
		// parent has a power item, save index to IV slot
		if (items[0] == items[1])
		{
			// both parents have same power item, choose parent at random
			whichParent[i] = Random() % 2;
			selectedIvs[i] = powerResult;
			initVal++;
			break;
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
    for (i = 0; i < NUM_STATS; i++)
    {
        availableIVs[i] = i;
    }

    // Select the num IVs that will be inherited.
    for (i = 0; i < ARRAY_COUNT(selectedIvs); i++)
    {
        // Randomly pick an IV from the available list.
        selectedIvs[i] = availableIVs[Random() % (NUM_STATS - i)];

        // Remove the selected IV index from the available IV indices.
        RemoveIVIndexFromList(availableIVs, i);
    }

    // Determine which parent each of the selected IVs should inherit from.
    for (i = 0; i < ARRAY_COUNT(selectedIvs); i++)
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


void AlterEggSpeciesWithIncenseItem(u16 *species, struct DayCare *daycare) {
	u16 motherItem = GetBoxMonData(&daycare->mons[0].mon, MON_DATA_HELD_ITEM, NULL);
	u16 fatherItem = GetBoxMonData(&daycare->mons[1].mon, MON_DATA_HELD_ITEM, NULL);   
	
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

// Decide features to inherit
void GiveEggFromDaycare(struct DayCare *daycare) {
    struct Pokemon egg;
    u16 species;
    u8 parentSlots[2]; // 0th index is "mother" daycare slot, 1st is "father"
    bool8 isEgg;

    species = DetermineEggSpeciesAndParentSlots(daycare, parentSlots);
    AlterEggSpeciesWithIncenseItem(&species, daycare);
    SetInitialEggData(&egg, species, daycare);
    InheritIVs(&egg, daycare);	// destiny knot check
    BuildEggMoveset(&egg, &daycare->mons[parentSlots[1]].mon, &daycare->mons[parentSlots[0]].mon);

	// handled in BuildEggMoveset
    //if (species == SPECIES_PICHU)
    //    GiveVoltTackleIfLightBall(&egg, daycare);

    isEgg = TRUE;
    SetMonData(&egg, MON_DATA_IS_EGG, &isEgg);
    gPlayerParty[PARTY_SIZE - 1] = egg;
    CompactPartySlots();
    CalculatePlayerPartyCount();
    RemoveEggFromDayCare(daycare);
};





