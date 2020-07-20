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
#include "../include/constants/pokedex.h"

#include "../include/new/build_pokemon.h"
#include "../include/new/catching.h"
#include "../include/new/daycare.h"
#include "../include/new/learn_move.h"
#include "../include/new/item.h"
#include "../include/new/util.h"
/*
daycare.c
	functions that handle all daycare functions, including attribute inheritance and step counts
*/

#define sHatchedEggFatherMoves ((u16*) 0x202455C)
#define sHatchedEggMotherMoves ((u16*)0x2024580)
#define sHatchedEggFinalMoves ((u16*) 0x2024564)
#define sHatchedEggLevelUpMoves ((u16*) 0x20244F8)

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
static u8 CheckPowerItem(u16 item);
static void InheritIVs(struct Pokemon* egg, struct DayCare* daycare);
static void AlterEggSpeciesWithIncenseItem(u16* species, struct DayCare* daycare);
static void AlterSpeciesWithIncenseItems(u16* species, u16 motherItem, u16 fatherItem);
static void InheritPokeBall(struct Pokemon* egg, struct DayCare* daycare);
static u32 DetermineEggPersonality(struct DayCare* daycare, struct BoxPokemon* mother);
static bool8 DetermineEggHiddenAbility(struct BoxPokemon* father, struct BoxPokemon* mother);
static void SetInitialEggData(struct Pokemon* mon, u16 species, u32 personality);
static u8 GetEggStepsToSubtract(void);

//Called from GiveEggFromDaycare
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
	//	sHatchedEggEggMoves[i] = 0;
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
	switch(SpeciesToNationalPokedexNum(eggSpecies))
	{
		#if (defined NATIONAL_DEX_NIDORAN_F && defined SPECIES_NIDORAN_M)
		case NATIONAL_DEX_NIDORAN_F:
			if (personality & 0x8000)
				eggSpecies = SPECIES_NIDORAN_M;
			break;
		#endif

		#if (defined NATIONAL_DEX_PIKACHU && defined SPECIES_PICHU)
		case NATIONAL_DEX_PIKACHU: //Get's all the special forms
			eggSpecies = SPECIES_PICHU;
			break;
		#endif

		#if (defined NATIONAL_DEX_ILLUMISE && defined SPECIES_VOLBEAT)
		case NATIONAL_DEX_ILLUMISE:
			if (personality & 0x8000)
				eggSpecies = SPECIES_VOLBEAT;
			break;
		#endif

		#if (defined NATIONAL_DEX_MANAPHY && defined SPECIES_PHIONE)
		case NATIONAL_DEX_MANAPHY:
			eggSpecies = SPECIES_PHIONE;
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

static void InheritPokeBall(struct Pokemon* egg, struct DayCare* daycare)
{
	u8 parent = DAYCARE_MOTHER;	// mother by default
	u8 parentBall;

	u16 motherSpecies = GetBoxMonData(&daycare->mons[0].mon, MON_DATA_SPECIES, NULL);
	u16 fatherSpecies = GetBoxMonData(&daycare->mons[1].mon, MON_DATA_SPECIES, NULL);

	// gen 7 same species checks
	#ifdef SPECIES_DITTO
	if (motherSpecies == SPECIES_DITTO)
		parent = DAYCARE_FATHER;	// gen 7 ditto check -> male or non-gendered mon with ditto (mother) makes pokemon inherit from father
	else
	#endif
	if (motherSpecies == fatherSpecies)
		parent = Random() % 2;	// same parent species -> pokeball inherited randomly

	// get poke ball ID
	parentBall = GetBoxMonData(&daycare->mons[parent].mon, MON_DATA_POKEBALL, NULL);

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
	s32 natureSlot = GetSlotToInheritNature(daycare);	// updated nature slot check
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
		return personality | abilityBit;
	}
	else
	{
		u8 wantedNature = GetNatureFromPersonality(GetBoxMonData(&daycare->mons[natureSlot].mon, MON_DATA_PERSONALITY, NULL));
		u32 personality;

		do
		{
			personality = Random32() & ~(1);
			personality |= abilityBit;
			if (wantedNature == GetNatureFromPersonality(personality))
				break; // we found a personality with the same nature
		} while (TRUE);

		return personality;
	}
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
}

static u8 GetEggStepsToSubtract(void)
{
	u8 i;
	u8 count = CalculatePlayerPartyCount();
	//If any party mons have Magma Armor or Flame Body, subtract 2 steps from hatch counter instead of 1
	for (i = 0; i < count; ++i)
	{
		if (!GetMonData(&gPlayerParty[i], MON_DATA_IS_EGG, NULL))
		{
			u8 ability = GetMonAbility(&gPlayerParty[i]);
			if (ability == ABILITY_MAGMAARMOR || ability == ABILITY_FLAMEBODY || ability == ABILITY_STEAMENGINE)
			{
				return 2;
			}
		}
	}
	return 1;
}

u32 SubtractEggSteps(u32 steps, struct Pokemon* mon)
{
	u8 toSub = GetEggStepsToSubtract();

	if (steps >= toSub)
		steps -= toSub;
	else
		steps -= 1;

	SetMonData(mon, MON_DATA_FRIENDSHIP, &steps);
	return steps;
}

void TryDecrementingDaycareStepCounterIfMoreEggsToHatch(struct DayCare* daycare, u8 ignoreId)
{
	u32 i, steps;

	for (i = 0; i < gPlayerPartyCount; ++i)
	{
		if (i == ignoreId)
			continue;
		if (!GetMonData(&gPlayerParty[i], MON_DATA_IS_EGG, NULL))
			continue;
		if (GetMonData(&gPlayerParty[i], MON_DATA_SANITY_IS_BAD_EGG, NULL))
			continue;

		steps = GetMonData(&gPlayerParty[i], MON_DATA_FRIENDSHIP, NULL);
		if (steps == 0 || steps == 1)
		{
			daycare->stepCounter--;
			return; //So the next Egg can hatch on the next step
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
			if (!moveInList[eggMovesBuffer[i]] && !MoveInMonMoveset(eggMovesBuffer[i], mon))
				moves[j++] = eggMovesBuffer[i];
		}
	}

	return j;
}
