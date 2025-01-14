#include "defines.h"
#include "../include/daycare.h"
#include "../include/pokemon.h"
#include "../include/pokemon_storage_system.h"
#include "../include/random.h"
#include "../include/constants/abilities.h"
#include "../include/constants/hold_effects.h"
#include "../include/constants/items.h"
#include "../include/constants/moves.h"
#include "../include/constants/region_map_sections.h"
#include "../include/constants/species.h"
#include "../include/constants/pokemon.h"
#include "../include/constants/pokedex.h"

#include "../include/new/build_pokemon.h"
#include "../include/new/catching.h"
#include "../include/new/daycare.h"
#include "../include/new/exp.h"
#include "../include/new/form_change.h"
#include "../include/new/learn_move.h"
#include "../include/new/item.h"
#include "../include/new/pokemon_storage_system.h"
#include "../include/new/util.h"
/*
daycare.c
	functions that handle all daycare functions, including attribute inheritance and step counts
*/

#define EGG_LVL_UP_MOVES_ARRAY_COUNT 50

/*Priority:
1. Volt Tackle
2. Mother's Egg Moves
3. Father's Egg Moves
4. Father TM and HM Moves
5. Inherited Level-Up Moves
6. Baby's Default Moveset.
*/

enum Parents
{
	DAYCARE_MOTHER,
	DAYCARE_FATHER,
};

//This file's functions:
static s32 GetSlotToInheritNature(struct DayCare* daycare);
static void DetermineEggParentSlots(struct DayCare* daycare, u8* parentSlots);
static u16 DetermineEggSpeciesAndParentSlots(struct DayCare* daycare, u8* parentSlots, u32 personality);
static u16 GetOverrideEggSpecies(u16 eggSpecies);
static u8 CheckPowerItem(u16 item);
static void InheritIVs(struct Pokemon* egg, struct DayCare* daycare);
static void AlterEggSpeciesWithIncenseItem(u16* species, struct DayCare* daycare);
static void AlterSpeciesWithIncenseItems(u16* species, u16 motherItem, u16 fatherItem);
static void InheritPokeBall(struct Pokemon* egg, struct BoxPokemon* father, struct BoxPokemon* mother);
static u32 DetermineEggPersonality(struct DayCare* daycare, struct BoxPokemon* mother);
static bool8 DetermineEggHiddenAbility(struct BoxPokemon* father, struct BoxPokemon* mother);
static void SetInitialEggData(struct Pokemon* mon, u16 species, u32 personality);
static u8 GetEggStepsToSubtract(void);

//Called from GiveEggFromDaycare
void BuildEggMoveset(struct Pokemon* egg, struct BoxPokemon* father, struct BoxPokemon* mother)
{
	u32 i, j;
	u16 eggSpecies = egg->species;
	u32 numLevelUpMoves, numEggMoves, numSharedParentMoves;
	
	u16 levelUpMoves[MAX_LEARNABLE_MOVES] = {0};
	u16 eggMoves[EGG_MOVES_ARRAY_COUNT] = {0};
	u16 finalMoves[MAX_MON_MOVES] = {0};
	u16 fatherMoves[MAX_MON_MOVES] = {0};
	u16 motherMoves[MAX_MON_MOVES] = {0};

	numSharedParentMoves = 0;
	numLevelUpMoves = GetLevelUpMovesBySpecies(eggSpecies, levelUpMoves);

	for (i = 0; i < MAX_MON_MOVES; ++i)
	{
		fatherMoves[i] = GetBoxMonData(father, MON_DATA_MOVE1 + i, NULL);
		motherMoves[i] = GetBoxMonData(mother, MON_DATA_MOVE1 + i, NULL);
	}

	numEggMoves = GetEggMoves(egg, eggMoves);

	//Shared Moves Between Parents
	for (i = 0; i < MAX_MON_MOVES; ++i)
	{
		for (j = 0; j < MAX_MON_MOVES && fatherMoves[i] != MOVE_NONE; j++)
		{
			if (fatherMoves[i] == motherMoves[j])
				finalMoves[numSharedParentMoves++] = fatherMoves[i];
		}
	}

	//Try Assign Shared Moves to Baby
	for (i = 0; i < MAX_MON_MOVES; ++i)
	{
		for (j = 0; j < numLevelUpMoves && finalMoves[i] != MOVE_NONE; ++j)
		{
			if (finalMoves[i] == levelUpMoves[j])
			{
				if (GiveMoveToMon(egg, finalMoves[i]) == 0xFFFF)
					DeleteFirstMoveAndGiveMoveToMon(egg, finalMoves[i]);
				break;
			}
		}
	}


#ifdef FATHER_PASSES_TMS
	for (i = 0; i < MAX_MON_MOVES; ++i)
	{
		if (fatherMoves[i] != MOVE_NONE)
		{
			for (j = 0; j < NUM_TMSHMS; ++j)	//loop through tm indices
			{
				if (fatherMoves[i] == gTMHMMoves[j] && CanMonLearnTMHM(egg, j))
				{
					if (GiveMoveToMon(egg, fatherMoves[i]) == 0xFFFF)
						DeleteFirstMoveAndGiveMoveToMon(egg, fatherMoves[i]);
				}
			}
		}
	}
#endif

	//Father Egg Moves
	for (i = 0; i < MAX_MON_MOVES; ++i)
	{
		if (fatherMoves[i] != MOVE_NONE)
		{
			for (j = 0; j < numEggMoves; ++j)
			{
				if (fatherMoves[i] == eggMoves[j])
				{
					if (GiveMoveToMon(egg, fatherMoves[i]) == 0xFFFF)
						DeleteFirstMoveAndGiveMoveToMon(egg, fatherMoves[i]);
					break;
				}
			}
		}
	}

	//Mother Egg Moves
	for (i = 0; i < MAX_MON_MOVES; ++i)
	{
		if (motherMoves[i] != MOVE_NONE)
		{
			for (j = 0; j < numEggMoves; ++j)
			{
				if (motherMoves[i] == eggMoves[j])
				{
					if (GiveMoveToMon(egg, motherMoves[i]) == 0xFFFF)
						DeleteFirstMoveAndGiveMoveToMon(egg, motherMoves[i]);
					break;
				}
			}
		}
	}

	//Volt Tackle
	#if (defined SPECIES_PICHU && defined ITEM_LIGHT_BALL)
	if (eggSpecies == SPECIES_PICHU)
	{
		if (GetBoxMonData(mother, MON_DATA_HELD_ITEM, NULL) == ITEM_LIGHT_BALL
		||  GetBoxMonData(father, MON_DATA_HELD_ITEM, NULL) == ITEM_LIGHT_BALL)
		{
			if (GiveMoveToMon(egg, MOVE_VOLTTACKLE) == 0xFFFF)
				DeleteFirstMoveAndGiveMoveToMon(egg, MOVE_VOLTTACKLE);
		}
	}
	#endif
}


static s32 GetSlotToInheritNature(struct DayCare* daycare)
{
	int i;
	u8 numWithEverstone = 0;
	s32 slot = -1;

	for (i = 0; i < DAYCARE_MON_COUNT; ++i)
	{
		if (ItemId_GetHoldEffect(GetBoxMonData(&daycare->mons[i].mon, MON_DATA_HELD_ITEM, NULL)) == ITEM_EFFECT_PREVENT_EVOLVE)
		{
			slot = i;
			++numWithEverstone;
		}
	}

	if (numWithEverstone < 2)
		return slot;

	return Random() & 1; //Either nature b/c both hold everstone
}

static void DetermineEggParentSlots(struct DayCare* daycare, u8* parentSlots)
{
	u16 i;
	u16 species[DAYCARE_MON_COUNT];

	// Determine which of the daycare mons is the mother and father of the egg.
	// The 0th index of the parentSlots array is considered the mother slot, and the
	// 1st index is the father slot.
	for (i = 0; i < DAYCARE_MON_COUNT; ++i)
	{
		species[i] = GetBoxMonData(&daycare->mons[i].mon, MON_DATA_SPECIES, NULL);
		#ifdef SPECIES_DITTO
		if (species[i] == SPECIES_DITTO)
		{
			parentSlots[0] = i ^ 1;
			parentSlots[1] = i;
		}
		else
		#endif
		if (GetBoxMonGender(&daycare->mons[i].mon) == MON_FEMALE)
		{
			parentSlots[0] = i;
			parentSlots[1] = i ^ 1;
		}
	}

	#ifdef SPECIES_DITTO
	if (species[parentSlots[1]] == SPECIES_DITTO && GetBoxMonGender(&daycare->mons[parentSlots[0]].mon) != MON_FEMALE)
	{
		u8 temp = parentSlots[1];
		parentSlots[1] = parentSlots[0];
		parentSlots[0] = temp;
	}
	#endif
}

static u16 DetermineEggSpeciesAndParentSlots(struct DayCare* daycare, u8* parentSlots, u32 personality)
{
	u16 i;
	u16 species[DAYCARE_MON_COUNT];
	u16 motherSpecies, eggSpecies;

	for (i = 0; i < DAYCARE_MON_COUNT; ++i)
		species[i] = GetBoxMonData(&daycare->mons[i].mon, MON_DATA_SPECIES, NULL);

	// Determine which of the daycare mons is the mother and father of the egg.
	// The 0th index of the parentSlots array is considered the mother slot, and the
	// 1st index is the father slot.
	for (i = 0, motherSpecies = SPECIES_NONE; i < DAYCARE_MON_COUNT; ++i)
	{
		#ifdef SPECIES_DITTO
		if (species[i] == SPECIES_DITTO)
		{
			motherSpecies = species[i ^ 1]; //Other species
			parentSlots[0] = i ^ 1;
			parentSlots[1] = i;
		}
		else
		#endif
		if (GetBoxMonGender(&daycare->mons[i].mon) == MON_FEMALE)
		{
			motherSpecies = species[i];

			//Handle Regional Form Breeding
			if (species[i] != species[i ^ 1]) //Different species
			{
				u16 currDexNum = SpeciesToNationalPokedexNum(species[i]);
				u16 otherDexNum = SpeciesToNationalPokedexNum(species[i ^ 1]);
				if (currDexNum == otherDexNum) //Two different species of the same dexNum
				{
					u16 currHeldItem = GetBoxMonData(&daycare->mons[i].mon, MON_DATA_HELD_ITEM, NULL);
					u16 otherHeldItem = GetBoxMonData(&daycare->mons[i ^ 1].mon, MON_DATA_HELD_ITEM, NULL);
					if (currHeldItem != ITEM_EVERSTONE && otherHeldItem == ITEM_EVERSTONE) //Go by the species of the Everstone holder
					{
						motherSpecies = species[i ^ 1]; //Mon with Everstone
					}
				}
			}

			parentSlots[0] = i;
			parentSlots[1] = i ^ 1;
		}
	}

	eggSpecies = GetEggSpecies(motherSpecies);
	eggSpecies = GetOverrideEggSpecies(eggSpecies);

	switch(SpeciesToNationalPokedexNum(eggSpecies))
	{
		#if (defined NATIONAL_DEX_NIDORAN_F && defined SPECIES_NIDORAN_M)
		case NATIONAL_DEX_NIDORAN_F:
			if (personality & 0x8000)
				eggSpecies = SPECIES_NIDORAN_M;
			break;
		#endif

		#if (defined NATIONAL_DEX_ILLUMISE && defined SPECIES_VOLBEAT)
		case NATIONAL_DEX_ILLUMISE:
			if (personality & 0x8000)
				eggSpecies = SPECIES_VOLBEAT;
			break;

		case NATIONAL_DEX_VOLBEAT:
			if (!(personality & 0x8000))
				eggSpecies = SPECIES_ILLUMISE;
			break;
		#endif

		#if (defined NATIONAL_DEX_MANAPHY && defined SPECIES_PHIONE)
		case NATIONAL_DEX_MANAPHY:
			eggSpecies = SPECIES_PHIONE;
			break;
		#endif

		#ifdef NATIONAL_DEX_MINIOR
		case NATIONAL_DEX_MINIOR:
			eggSpecies = GetMiniorCoreFromPersonality(personality);
			break;
		#endif

		#if (defined NATIONAL_DEX_INDEEDEE && defined SPECIES_INDEEDEE && defined SPECIES_INDEEDEE_FEMALE)
		case NATIONAL_DEX_INDEEDEE:
			eggSpecies = (personality & 0x8000) ? SPECIES_INDEEDEE : SPECIES_INDEEDEE_FEMALE;
			break;
		#endif
	}

	// Make Ditto the "mother" slot if the other daycare mon is male/genderless).
	#ifdef SPECIES_DITTO
	if (species[parentSlots[1]] == SPECIES_DITTO && GetBoxMonGender(&daycare->mons[parentSlots[0]].mon) != MON_FEMALE)
	{
		u8 temp = parentSlots[1];
		parentSlots[1] = parentSlots[0];
		parentSlots[0] = temp;
	}
	#endif

	return eggSpecies;
}

static u16 GetOverrideEggSpecies(u16 eggSpecies)
{
	//Gets the proper species for Pokemon that may not have an Egg species

	switch(SpeciesToNationalPokedexNum(eggSpecies))
	{
		#if (defined NATIONAL_DEX_PIKACHU && defined SPECIES_PICHU)
		case NATIONAL_DEX_PIKACHU: //Get's all the special forms
			eggSpecies = SPECIES_PICHU;
			break;
		#endif

		#if (defined NATIONAL_DEX_ROTOM && defined SPECIES_ROTOM)
		case NATIONAL_DEX_ROTOM:
			eggSpecies = SPECIES_ROTOM;
			break;
		#endif

		#if (defined NATIONAL_DEX_VIVILLON && defined SPECIES_SCATTERBUG)
		case NATIONAL_DEX_VIVILLON:
			eggSpecies = SPECIES_SCATTERBUG;
			break;
		#endif

		#if (defined NATIONAL_DEX_FURFROU && defined SPECIES_FURFROU)
		case NATIONAL_DEX_FURFROU:
			eggSpecies = SPECIES_FURFROU;
			break;
		#endif
	}

	return eggSpecies;
}

static u8 CheckPowerItem(u16 item)
{
	u8 itemEffect = ItemId_GetHoldEffect(item);
	u8 itemQuality = ItemId_GetHoldEffectParam(item);

	if (itemEffect != ITEM_EFFECT_MACHO_BRACE)
		return 0xFF;

	switch (itemQuality)
	{
		case QUALITY_POWER_BRACER:
			return STAT_ATK;

		case QUALITY_POWER_BELT:
			return STAT_DEF;

		case QUALITY_POWER_LENS:
			return STAT_SPATK;

		case QUALITY_POWER_BAND:
			return STAT_SPDEF;

		case QUALITY_POWER_ANKLET:
			return STAT_SPD;

		case QUALITY_POWER_WEIGHT:
			return STAT_HP;

		default:
			return 0xFF;
	}
};

static void InheritIVs(struct Pokemon* egg, struct DayCare* daycare)
{
	u8 i, j, numIVs, stat, powerResult;
	u16 items[2];

	items[0] = GetBoxMonData(&daycare->mons[0].mon, MON_DATA_HELD_ITEM, NULL);
	items[1] = GetBoxMonData(&daycare->mons[1].mon, MON_DATA_HELD_ITEM, NULL);

	// get number of IVs to inherit from either parent
	if (ItemId_GetHoldEffect(items[0]) == ITEM_EFFECT_DESTINY_KNOT
	||  ItemId_GetHoldEffect(items[1]) == ITEM_EFFECT_DESTINY_KNOT)
		numIVs = 5;
	else
		numIVs = 3;

	u8 selectedIvs[numIVs];
	u8 whichParent[numIVs];
	bool8 statChosen[NUM_STATS] = {FALSE};

	//Initiate first 1 or 2 IV slots with power items from either or both parents
	for (i = 0, j = 0, powerResult = 0xFF; i < DAYCARE_MON_COUNT; ++i)
	{
		powerResult = CheckPowerItem(items[i]);	// check if parent i has a power item
		if (powerResult == 0xFF)
			continue;	// this parent does not have a power item, check next

		// parent has a power item, save index to IV slot
		if (CheckPowerItem(items[0]) == CheckPowerItem(items[1]))
		{
			// both parents have same power item, choose parent at random
			whichParent[j] = Random() & 1;	// 0 or 1
			selectedIvs[j++] = powerResult;
			statChosen[powerResult] = TRUE;
			break;	//no need to do second loop since both have same item
		}
		else
		{
			whichParent[j] = i;
			selectedIvs[j++] = powerResult;
			statChosen[powerResult] = TRUE;
		}
	}

	//Randomize the remaining IV indices with unique values
	for (i = j; i < numIVs; ++i)
	{
		do
		{
			stat = Random() % NUM_STATS;
		} while (statChosen[stat]);

		statChosen[stat] = TRUE;
		selectedIvs[i] = stat;
		whichParent[i] = Random() & 1;
	}

	// Set each of inherited IVs on the egg mon.
	for (i = 0; i < numIVs; ++i)
	{
		switch (selectedIvs[i])
		{
			case STAT_HP:
			case STAT_ATK:
			case STAT_DEF:
			case STAT_SPD:
			case STAT_SPATK:
			case STAT_SPDEF: ;
				u8 diff = selectedIvs[i] - STAT_HP;
				u8 iv = GetBoxMonData(&daycare->mons[whichParent[i]].mon, MON_DATA_HP_IV + diff, NULL);
				SetMonData(egg, MON_DATA_HP_IV + diff, &iv);
				break;
		}
	}
}


static void AlterEggSpeciesWithIncenseItem(u16* species, struct DayCare* daycare)
{
	u16 motherItem = GetBoxMonData(&daycare->mons[0].mon, MON_DATA_HELD_ITEM, NULL);
	u16 fatherItem = GetBoxMonData(&daycare->mons[1].mon, MON_DATA_HELD_ITEM, NULL);
	AlterSpeciesWithIncenseItems(species, motherItem, fatherItem);
}

static void AlterSpeciesWithIncenseItems(u16* species, u16 motherItem, u16 fatherItem)
{
	// if neither parent holding incense, force 2nd evo species
	switch (*species)
	{
		#if (defined SPECIES_WYNAUT && defined SPECIES_WOBBUFFET && defined ITEM_LAX_INCENSE)
		case SPECIES_WYNAUT:
			if (motherItem != ITEM_LAX_INCENSE && fatherItem != ITEM_LAX_INCENSE)
				*species = SPECIES_WOBBUFFET;
			break;
		#endif

		#if (defined SPECIES_AZURILL && defined SPECIES_MARILL && defined ITEM_SEA_INCENSE)
		case SPECIES_AZURILL:
			if (motherItem != ITEM_SEA_INCENSE && fatherItem != ITEM_SEA_INCENSE)
				*species = SPECIES_MARILL;
			break;
		#endif

		#if (defined SPECIES_MUNCHLAX && defined SPECIES_SNORLAX && defined ITEM_FULL_INCENSE)
		case SPECIES_MUNCHLAX:
			if (motherItem != ITEM_FULL_INCENSE && fatherItem != ITEM_FULL_INCENSE)
				*species = SPECIES_SNORLAX;
			break;
		#endif

		#if (defined SPECIES_MIME_JR && defined SPECIES_MR_MIME && defined ITEM_ODD_INCENSE)
		case SPECIES_MIME_JR:
			if (motherItem != ITEM_ODD_INCENSE && fatherItem != ITEM_ODD_INCENSE)
				*species = SPECIES_MR_MIME;
			break;
		#endif

		#if (defined SPECIES_CHINGLING && defined SPECIES_CHIMECHO && defined ITEM_PURE_INCENSE)
		case SPECIES_CHINGLING:
			if (motherItem != ITEM_PURE_INCENSE && fatherItem != ITEM_PURE_INCENSE)
				*species = SPECIES_CHIMECHO;
			break;
		#endif

		#if (defined SPECIES_BONSLY && defined SPECIES_SUDOWOODO && defined ITEM_ROCK_INCENSE)
		case SPECIES_BONSLY:
			if (motherItem != ITEM_ROCK_INCENSE && fatherItem != ITEM_ROCK_INCENSE)
				*species = SPECIES_SUDOWOODO;
			break;
		#endif

		#if (defined SPECIES_BUDEW && defined SPECIES_ROSELIA && defined ITEM_ROSE_INCENSE)
		case SPECIES_BUDEW:
			if (motherItem != ITEM_ROSE_INCENSE && fatherItem != ITEM_ROSE_INCENSE)
				*species = SPECIES_ROSELIA;
			break;
		#endif

		#if (defined SPECIES_MANTYKE && defined SPECIES_MANTINE && defined ITEM_WAVE_INCENSE)
		case SPECIES_MANTYKE:
			if (motherItem != ITEM_WAVE_INCENSE && fatherItem != ITEM_WAVE_INCENSE)
				*species = SPECIES_MANTINE;
			break;
		#endif

		#if (defined SPECIES_HAPPINY && defined SPECIES_CHANSEY && defined ITEM_LUCK_INCENSE)
		case SPECIES_HAPPINY:
			if (motherItem != ITEM_LUCK_INCENSE && fatherItem != ITEM_LUCK_INCENSE)
				*species = SPECIES_CHANSEY;
			break;
		#endif

		default:
			break;
	}
}

static void InheritPokeBall(struct Pokemon* egg, struct BoxPokemon* father, struct BoxPokemon* mother)
{
	struct BoxPokemon* parent = mother;	// mother by default
	u8 parentBall;

	u16 motherSpecies = GetBoxMonData(mother, MON_DATA_SPECIES, NULL);
	u16 fatherSpecies = GetBoxMonData(father, MON_DATA_SPECIES, NULL);

	// gen 7 same species checks
	#ifdef SPECIES_DITTO
	if (motherSpecies == SPECIES_DITTO)
		parent = father;	// gen 7 ditto check -> male or non-gendered mon with ditto (mother) makes pokemon inherit from father
	else
	#endif
	if (motherSpecies == fatherSpecies && Random() & 1)
		parent = father;	// same parent species -> pokeball inherited randomly

	// get poke ball ID
	parentBall = GetBoxMonData(parent, MON_DATA_POKEBALL, NULL);

	// master ball and cherish ball become poke ball
	#ifndef INHERIT_MASTER_CHERISH_BALL
	if (parentBall == BALL_TYPE_MASTER_BALL || parentBall == BALL_TYPE_CHERISH_BALL)
		parentBall = BALL_TYPE_POKE_BALL;
	#endif

	SetMonData(egg, MON_DATA_POKEBALL, &parentBall);
}

static u32 DetermineEggPersonality(struct DayCare* daycare, struct BoxPokemon* mother)
{
	u32 personality;
	s32 natureSlot = GetSlotToInheritNature(daycare);	//Updated nature slot check
	u8 abilityBit = Random() & 1;

	if (!((struct Pokemon*) mother)->hiddenAbility
	#ifdef SPECIES_DITTO
	&& GetBoxMonData(mother, MON_DATA_SPECIES, NULL) != SPECIES_DITTO
	#endif
	&& Random() % 100 < 80) //80 % chance to pass down slotted ability
		abilityBit = GetBoxMonData(mother, MON_DATA_PERSONALITY, NULL) & 1;

	if (natureSlot < 0)
	{
		personality = Random32() & ~(1); //Clear ability bit
		personality |= abilityBit;
	}
	else
	{
		u8 wantedNature = GetNatureFromPersonality(GetBoxMonData(&daycare->mons[natureSlot].mon, MON_DATA_PERSONALITY, NULL));

		do
		{
			personality = Random32() & ~(1);
			personality |= abilityBit;
		} while (wantedNature != GetNatureFromPersonality(personality));

		//We found a personality with the same nature
	}

	return personality;
}

static bool8 DetermineEggHiddenAbility(unusedArg struct BoxPokemon* father, struct BoxPokemon* mother)
{
	#ifdef SPECIES_DITTO
	if (GetBoxMonData(mother, MON_DATA_SPECIES, NULL) == SPECIES_DITTO)
		mother = father; //Get Hidden Ability from father instead
	#endif

	if (((struct Pokemon*) mother)->hiddenAbility)
		return Random() % 100 < 60; //60 % chance to pass down Hidden Ability

	return FALSE;
}

static void TryDoMasudaMethod(struct Pokemon* mon, struct BoxPokemon* parent1, struct BoxPokemon* parent2)
{
	//Since regional mons are basically non-existant in ROM Hacks,
	//activate Masuda method for mons with differing OTIds
	if (!IsMonShiny(mon)
	&& GetBoxMonData(parent1, MON_DATA_OT_ID, NULL) != GetBoxMonData(parent2, MON_DATA_OT_ID, NULL)) //Parents from differing Trainers
	{
		u32 i;
		bool8 forceShiny = FALSE;
		u32 otId = GetMonData(mon, MON_DATA_OT_ID, NULL);

		for (i = 0; i < 5; ++i) //Try 5 attempts to force shiny
		{
			if (IsShinyOtIdPersonality(otId, Random32()))
			{
				forceShiny = TRUE;
				break;
			}
		}

		if (forceShiny)
			ForceMonShiny(mon);
	}
}

static void SetInitialEggData(struct Pokemon* mon, u16 species, u32 personality)
{
	//u8 ball;
	u8 metLevel;
	u8 language;
	bool8 hiddenAbility = mon->hiddenAbility;

	CreateMon(mon, species, EGG_HATCH_LEVEL, 0x20, TRUE, personality, FALSE, 0);
	metLevel = 0;

	//Not used due to ball inheritance
	//ball = BALL_TYPE_POKE_BALL;
	//SetMonData(mon, MON_DATA_POKEBALL, &ball);

	language = GAME_LANGUAGE;
	SetMonData(mon, MON_DATA_NICKNAME, (const void*) 0x825F83E);
	SetMonData(mon, MON_DATA_FRIENDSHIP, &gBaseStats[species].eggCycles);	//required steps to hatch
	SetMonData(mon, MON_DATA_MET_LEVEL, &metLevel);
	SetMonData(mon, MON_DATA_LANGUAGE, &language);
	mon->hiddenAbility = hiddenAbility;
}

void CreateEgg(struct Pokemon *mon, u16 species) //The function used by the giveegg scripting command
{
	u8 metLevel = 0;
	u8 metLocation = METLOC_SPECIAL_EGG;
	u8 language = GAME_LANGUAGE;
	u16 ball = BALL_TYPE_POKE_BALL;
	bool8 isEgg = TRUE;

	CreateMon(mon, species, EGG_HATCH_LEVEL, 0x20, FALSE, 0, FALSE, 0);
	SetMonData(mon, MON_DATA_POKEBALL, &ball);
	SetMonData(mon, MON_DATA_NICKNAME, (void*) 0x825F83E);
	SetMonData(mon, MON_DATA_FRIENDSHIP, &gBaseStats[species].eggCycles);
	SetMonData(mon, MON_DATA_MET_LEVEL, &metLevel);
	SetMonData(mon, MON_DATA_MET_LOCATION, &metLocation);
	SetMonData(mon, MON_DATA_LANGUAGE, &language);
	SetMonData(mon, MON_DATA_IS_EGG, &isEgg);
	HealMon(mon);
}

//Decide features to inherit
void GiveEggFromDaycare(struct DayCare* daycare)
{
	struct Pokemon egg = {0};
	u16 species;
	u8 parentSlots[2] = {0}; // 0th index is "mother" daycare slot, 1st is "father"
	bool8 isEgg;

	DetermineEggParentSlots(daycare, parentSlots);
	egg.hiddenAbility = DetermineEggHiddenAbility(&daycare->mons[parentSlots[1]].mon, &daycare->mons[parentSlots[0]].mon);
	u32 personality = DetermineEggPersonality(daycare, &daycare->mons[parentSlots[0]].mon);
	species = DetermineEggSpeciesAndParentSlots(daycare, parentSlots, personality);
	AlterEggSpeciesWithIncenseItem(&species, daycare);
	SetInitialEggData(&egg, species, personality);	// sets base data (ball, met level, lang, etc)
	TryDoMasudaMethod(&egg, &daycare->mons[parentSlots[0]].mon, &daycare->mons[parentSlots[1]].mon);
	InheritIVs(&egg, daycare);	// destiny knot check
	InheritPokeBall(&egg, &daycare->mons[parentSlots[1]].mon, &daycare->mons[parentSlots[0]].mon);
	BuildEggMoveset(&egg, &daycare->mons[parentSlots[1]].mon, &daycare->mons[parentSlots[0]].mon);

	isEgg = TRUE;
	SetMonData(&egg, MON_DATA_IS_EGG, &isEgg);
	CalculateMonStats(&egg);

	#ifdef SEND_EGGS_TO_PC_IF_MAX_PARTY
	if (gPlayerPartyCount >= PARTY_SIZE) //Calculated in script
	{
		SendMonToPC(&egg);
	}
	else
	#endif
	{
		Memcpy(&gPlayerParty[5], &egg, sizeof(egg));
		CompactPartySlots();
		CalculatePlayerPartyCount();
	}

	//RemoveEggFromDayCare(daycare); //Normally resets the personality and step counter
}

void TriggerPendingDaycareEgg(unusedArg struct DayCare *daycare)
{
	#ifdef VAR_DAYCARE_NUM_EGGS
		u16 numEggs = VarGet(VAR_DAYCARE_NUM_EGGS);
		if (numEggs < 100)
			++numEggs;
		VarSet(VAR_DAYCARE_NUM_EGGS, numEggs);
	#else
		FlagSet(FLAG_PENDING_DAYCARE_EGG);
	#endif
}

bool8 IsEggPending(unusedArg struct DayCare *daycare)
{
	#ifdef VAR_DAYCARE_NUM_EGGS
		return VarGet(VAR_DAYCARE_NUM_EGGS) != 0;
	#else
		return FlagGet(FLAG_PENDING_DAYCARE_EGG);
	#endif
}

void CreateHatchedMon(struct Pokemon *egg, struct Pokemon *temp)
{
	u16 species;
	u32 personality, pokerus;
	u8 i, friendship, language, gameMet, markings, hidden;
	u16 moves[4];
	u32 ivs[NUM_STATS];
	u8 ballType;

	species = GetMonData(egg, MON_DATA_SPECIES, NULL);

	for (i = 0; i < MAX_MON_MOVES; ++i)
	{
		moves[i] = GetMonData(egg, MON_DATA_MOVE1 + i, NULL);
	}

	personality = GetMonData(egg, MON_DATA_PERSONALITY, NULL);

	for (i = 0; i < NUM_STATS; ++i)
	{
		ivs[i] = GetMonData(egg, MON_DATA_HP_IV + i, NULL);
	}

	ballType = GetMonData(egg, MON_DATA_POKEBALL, NULL);
	language = GetMonData(egg, MON_DATA_LANGUAGE, NULL);
	gameMet = GetMonData(egg, MON_DATA_MET_GAME, NULL);
	markings = GetMonData(egg, MON_DATA_MARKINGS, NULL);
	pokerus = GetMonData(egg, MON_DATA_POKERUS, NULL);

	//obedience = GetMonData(egg, MON_DATA_OBEDIENCE, NULL);
	hidden = egg->hiddenAbility;

	CreateMon(temp, species, EGG_HATCH_LEVEL, 0, TRUE, personality, OT_ID_PLAYER_ID, 0); //Set all IVs to 0 to start (set actually later on)

	for (i = 0; i < 4; ++i)
		SetMonData(temp, MON_DATA_MOVE1 + i,  &moves[i]);

	for (i = 0; i < NUM_STATS; ++i)
		SetMonData(temp, MON_DATA_HP_IV + i,  &ivs[i]);

	language = GAME_LANGUAGE;
	SetMonData(temp, MON_DATA_POKEBALL, &ballType);
	SetMonData(temp, MON_DATA_LANGUAGE, &language);
	SetMonData(temp, MON_DATA_MET_GAME, &gameMet);
	SetMonData(temp, MON_DATA_MARKINGS, &markings);

	friendship = 120;
	SetMonData(temp, MON_DATA_FRIENDSHIP, &friendship);
	SetMonData(temp, MON_DATA_POKERUS, &pokerus);

	//SetMonData(temp, MON_DATA_OBEDIENCE, &obedience);
	temp->hiddenAbility = hidden;

	*egg = *temp;
	CalculateMonStats(egg);
	HealMon(egg); //Fixes a bug where new Pokemon could hatch with more HP
}

static u8 GetEggStepsToSubtract(void)
{
	u32 i;
	u8 steps = 1;

	//If any party mons have Magma Armor or Flame Body, subtract 2 steps from hatch counter instead of 1
	for (i = 0; i < PARTY_SIZE; ++i)
	{
		u16 species = GetMonData(&gPlayerParty[i], MON_DATA_SPECIES2, NULL);
	
		if (species != SPECIES_NONE && species != SPECIES_EGG)
		{
			u8 ability = GetMonAbility(&gPlayerParty[i]);

			if (ability == ABILITY_MAGMAARMOR || ability == ABILITY_FLAMEBODY || ability == ABILITY_STEAMENGINE)
			{
				steps = 2;
				break;
			}
		}
	}

	#ifdef UNBOUND
	if (FlagGet(FLAG_SANDBOX_MODE))
		steps = 100; //Basically insta-hatch
	else if (CheckBagHasItem(ITEM_MAGMA_STONE, 1))
		steps += 1;
	#endif

	return steps;
}

u32 SubtractEggSteps(u32 steps, struct Pokemon* mon)
{
	u8 toSub = GetEggStepsToSubtract();

	if (steps >= toSub)
		steps -= toSub;
	else
		steps = 0;

	SetMonData(mon, MON_DATA_FRIENDSHIP, &steps);
	return steps;
}

void TryDecrementingDaycareStepCounterIfMoreEggsToHatch(struct DayCare* daycare, u8 ignoreId)
{
	u32 i, steps, toSubtract;
	toSubtract = GetEggStepsToSubtract();

	for (i = 0; i < gPlayerPartyCount; ++i)
	{
		if (i == ignoreId)
			continue;
		if (!GetMonData(&gPlayerParty[i], MON_DATA_IS_EGG, NULL))
			continue;
		if (GetMonData(&gPlayerParty[i], MON_DATA_SANITY_IS_BAD_EGG, NULL))
			continue;

		steps = GetMonData(&gPlayerParty[i], MON_DATA_FRIENDSHIP, NULL);
		if (steps <= toSubtract) //Egg should hatch next step
		{
			daycare->stepCounter--;
			return; //So the next Egg can hatch on the next step
		}
	}
}

u8 GetDaycareCompatibilityScore(struct DayCare *daycare)
{
	u32 i;
	u16 eggGroups[DAYCARE_MON_COUNT][EGG_GROUPS_PER_MON];
	u16 species[DAYCARE_MON_COUNT];
	u32 trainerIds[DAYCARE_MON_COUNT];
	u32 genders[DAYCARE_MON_COUNT];

	for (i = 0; i < DAYCARE_MON_COUNT; i++)
	{
		u32 personality;

		species[i] = GetBoxMonData(&daycare->mons[i].mon, MON_DATA_SPECIES, NULL);
		trainerIds[i] = GetBoxMonData(&daycare->mons[i].mon, MON_DATA_OT_ID, NULL);
		personality = GetBoxMonData(&daycare->mons[i].mon, MON_DATA_PERSONALITY, NULL);
		genders[i] = GetGenderFromSpeciesAndPersonality(species[i], personality);
		eggGroups[i][0] = gBaseStats[species[i]].eggGroup1;
		eggGroups[i][1] = gBaseStats[species[i]].eggGroup2;
	}

	//Check unbreedable egg group
	if (eggGroups[0][0] == EGG_GROUP_UNDISCOVERED || eggGroups[1][0] == EGG_GROUP_UNDISCOVERED)
		return PARENTS_INCOMPATIBLE;

	//Two Ditto can't breed
	if (eggGroups[0][0] == EGG_GROUP_DITTO && eggGroups[1][0] == EGG_GROUP_DITTO)
		return PARENTS_INCOMPATIBLE;

	//One parent is Ditto
	if (eggGroups[0][0] == EGG_GROUP_DITTO || eggGroups[1][0] == EGG_GROUP_DITTO)
	{
		if (trainerIds[0] == trainerIds[1])
			return PARENTS_LOW_COMPATIBILITY;

		return PARENTS_MED_COMPATIBILITY;
	}
	//Neither parent is Ditto
	else
	{
		if (genders[0] == genders[1])
			return PARENTS_INCOMPATIBLE;
		if (genders[0] == MON_GENDERLESS || genders[1] == MON_GENDERLESS)
			return PARENTS_INCOMPATIBLE;
		if (!EggGroupsOverlap(eggGroups[0], eggGroups[1]))
			return PARENTS_INCOMPATIBLE;

		if (SpeciesToNationalPokedexNum(species[0]) == SpeciesToNationalPokedexNum(species[1]))
		{
			if (trainerIds[0] == trainerIds[1])
				return PARENTS_MED_COMPATIBILITY; //Same species, same Trainer

			return PARENTS_MAX_COMPATIBILITY; //Same species, different Trainers
		}
		else
		{
			if (trainerIds[0] != trainerIds[1])
				return PARENTS_MED_COMPATIBILITY; //Different species, different trainers

			return PARENTS_LOW_COMPATIBILITY; //Different species, same Trainer
		}
	}
}

u8 ModifyBreedingScoreForOvalCharm(u8 score)
{
	#ifdef ITEM_OVAL_CHARM
	if (CheckBagHasItem(ITEM_OVAL_CHARM, 1) > 0)
	{
		switch (score) { //This is the easy way of doing this
			case 20:
				score = 40;
				break;
			case 50:
				score = 80;
				break;
			case 70:
				score = 88;
				break;
		}
	}
	#endif

	return score;
}

u8 GetAllEggMoves(struct Pokemon* mon, u16* moves, bool8 ignoreAlreadyKnownMoves)
{
	u8 numEggMoves;
	u32 i, j;
	struct Pokemon dummyMon = {0};
	u16 eggMovesBuffer[EGG_MOVES_ARRAY_COUNT];
	bool8 moveInList[MOVES_COUNT] = {FALSE};
	u16 species = GetMonData(mon, MON_DATA_SPECIES, NULL);
	u16 eggSpecies = GetEggSpecies(species);
	eggSpecies = GetOverrideEggSpecies(eggSpecies);

	SetMonData(&dummyMon, MON_DATA_SPECIES, &eggSpecies);
	numEggMoves = GetEggMoves(&dummyMon, eggMovesBuffer);

	//Filter out any egg moves the Pokemon already knows
	for (i = 0, j = 0; i < numEggMoves; ++i)
	{
		if (!ignoreAlreadyKnownMoves || !MoveInMonMoveset(eggMovesBuffer[i], mon))
		{
			moves[j++] = eggMovesBuffer[i];
			moveInList[eggMovesBuffer[i]] = TRUE;
		}
	}

	u16 eggSpecies2 = eggSpecies;
	AlterSpeciesWithIncenseItems(&eggSpecies2, 0, 0);
	if (eggSpecies2 != eggSpecies) //Different baby; eg. Marill + Azurill
	{
		SetMonData(&dummyMon, MON_DATA_SPECIES, &eggSpecies2);
		numEggMoves = GetEggMoves(&dummyMon, eggMovesBuffer);

		//Filter out any egg moves the Pokemon already knows
		for (i = 0; i < numEggMoves && j < EGG_MOVES_ARRAY_COUNT; ++i)
		{
			if (!moveInList[eggMovesBuffer[i]]) //Move wasn't already added above
			{
				if (!ignoreAlreadyKnownMoves || !MoveInMonMoveset(eggMovesBuffer[i], mon))
					moves[j++] = eggMovesBuffer[i];
			}
		}
	}

	return j;
}

u32 GetExperienceAfterDaycareSteps(struct BoxPokemon* mon, u32 steps)
{
	u32 originalExp = GetBoxMonData(mon, MON_DATA_EXP, NULL);
	u32 experience = originalExp + steps;

	#ifdef FLAG_HARD_LEVEL_CAP
	struct BoxPokemon tempMon = *mon;
	SetBoxMonData(&tempMon, MON_DATA_EXP, &experience);
	u8 level = GetLevelFromBoxMonExp(&tempMon);
	extern u8 GetCurrentLevelCap(void); //Must be implemented yourself
	if (FlagGet(FLAG_HARD_LEVEL_CAP) && level >= GetCurrentLevelCap())
	{
		SetBoxMonData(&tempMon, MON_DATA_EXP, &originalExp); //Prepare to get the original level
		if (GetLevelFromBoxMonExp(&tempMon) >= GetCurrentLevelCap())
			experience = originalExp; //Original level is already past level cap so don't add anymore
		else
			experience = GetSpeciesExpToLevel(GetBoxMonData(mon, MON_DATA_SPECIES, NULL), GetCurrentLevelCap()); //Exp can't go past level cap
	}
	#endif

	return experience;
}

u8 GetLevelAfterDaycareSteps(struct BoxPokemon* mon, u32 steps)
{
	struct BoxPokemon tempMon = *mon;
	u32 experience = GetExperienceAfterDaycareSteps(mon, steps);
	SetBoxMonData(&tempMon, MON_DATA_EXP, &experience);
	return GetLevelFromBoxMonExp(&tempMon);
}

bool8 ShouldSkipOfferEggHatchNickname(void)
{
	#ifdef FLAG_DONT_OFFER_NICKNAMES_BATTLE
	return FlagGet(FLAG_DONT_OFFER_NICKNAMES_BATTLE);
	#else
	return FALSE;
	#endif
}
