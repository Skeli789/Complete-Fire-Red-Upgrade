#include "defines.h"
#include "../include/random.h"
#include "../include/constants/abilities.h"

#include "../include/new/ability_tables.h"
#include "../include/new/damage_calc.h"
#include "../include/new/evolution.h"
#include "../include/new/frontier.h"
#include "../include/new/mega.h"
#include "../include/new/util.h"

/*
util.c
	general utility functions
*/

//This file's functions:
static u8 TryRandomizeAbility(u8 ability, unusedArg u16 species);

u32 MathMax(u32 num1, u32 num2)
{
	if (num1 > num2)
		return num1;

	return num2;
}

u32 MathMin(u32 num1, u32 num2)
{
	if (num1 < num2)
		return num1;

	return num2;
}

u16 RandRange(u16 min, u16 max)
{
	if (min == max)
		return min;

	return (Random() % (max - min)) + min;
}

bool8 CheckTableForMove(u16 move, const u16 table[])
{
	for (u32 i = 0; table[i] != MOVE_TABLES_TERMIN; ++i)
	{
		if (move == table[i])
			return TRUE;
	}

	return FALSE;
}

bool8 CheckTableForMovesEffect(u16 move, const u8 table[])
{
	for (u32 i = 0; table[i] != MOVE_EFFECT_TABLES_TERMIN; ++i)
	{
		if (gBattleMoves[move].effect == table[i])
			return TRUE;
	}

	return FALSE;
}

bool8 CheckTableForAbility(u8 ability, const u8 table[])
{
	for (u32 i = 0; table[i] != ABILITY_TABLES_TERMIN; ++i)
	{
		if (ability == table[i])
			return TRUE;
	}

	return FALSE;
}

bool8 CheckTableForSpecialMoveEffect(u8 effect, const u8 table[])
{
	for (u32 i = 0; table[i] != ABILITY_TABLES_TERMIN; ++i)
	{
		if (effect == table[i])
			return TRUE;
	}

	return FALSE;
}

bool8 CheckTableForSpecies(u16 species, const u16 table[])
{
	for (u32 i = 0; table[i] != SPECIES_TABLES_TERMIN; ++i)
	{
		if (species == table[i])
			return TRUE;
	}

	return FALSE;
}

bool8 CheckTableForItem(u16 item, const u16 table[])
{
	for (u32 i = 0; table[i] != ITEM_TABLES_TERMIN; ++i)
	{
		if (item == table[i])
			return TRUE;
	}

	return FALSE;
}

u8 ViableMonCount(struct Pokemon* party)
{
	u8 count = 0;

	for (u32 i = 0; i < PARTY_SIZE; ++i)
	{
		//Don't use GetMonData because time saving is important
		if (party[i].species != SPECIES_NONE
		&&  party[i].hp > 0
		&& !party[i].isEgg)
			++count;
	}

	return count;
}

void HealMon(struct Pokemon* mon)
{
	u32 none = 0;
	u16 maxHP = GetMonData(mon, MON_DATA_MAX_HP, NULL);

	//Restore HP.
	SetMonData(mon, MON_DATA_HP, &maxHP);

	//Restore PP.
	MonRestorePP(mon);

	//Restore Status.
	SetMonData(mon, MON_DATA_STATUS, &none);
}

void SetMonPokedexFlags(struct Pokemon* mon)
{
	if (!GetMonData(mon, MON_DATA_IS_EGG, NULL))
	{
		u16 dexNum = SpeciesToNationalPokedexNum(GetMonData(mon, MON_DATA_SPECIES, NULL));
		u32 personality = GetMonData(mon, MON_DATA_PERSONALITY, NULL);
		GetSetPokedexFlag(dexNum, FLAG_SET_SEEN);
		HandleSetPokedexFlag(dexNum, FLAG_SET_CAUGHT, personality);
	}
}

u16 GetNationalPokedexCount(u8 caseID)
{
	u32 i;
	u16 count = 0;
	u8 byte;
	u8* flags;

	switch (caseID) {
		case FLAG_GET_SEEN:
			flags = (u8*) SEEN_DEX_FLAGS;
			break;

		default: //case FLAG_GET_CAUGHT:
			flags = (u8*) CAUGHT_DEX_FLAGS;
			break;
	}

	for (i = 0; i <= (NATIONAL_DEX_COUNT - 1) / 8; ++i) //8 Pokemon per byte
	{
		byte = flags[i];

		while (byte != 0)
		{
			if (byte & 1)
				++count;

			byte >>= 1;
		}
	}

	return count;
}

bool8 SpeciesWithDexNumOnTeam(u16 dexNum)
{
	u32 i;

	for (i = 0; i < PARTY_SIZE; ++i)
	{
		u16 species = GetMonData(&gPlayerParty[i], MON_DATA_SPECIES2, NULL);

		if (species != SPECIES_NONE
		&& species != SPECIES_EGG
		&& SpeciesToNationalPokedexNum(species) == dexNum)
			return TRUE;
	}

	return FALSE;
}

bool8 CanSpeciesEvolve(u16 species)
{
	const struct Evolution* evolutions = gEvolutionTable[species];

	for (u32 i = 0; i < EVOS_PER_MON; ++i)
	{
		if (evolutions[i].method == EVO_NONE) //Most likely end of entries
			break; //Break now to save time
		else if (evolutions[i].method != MEGA_EVOLUTION && evolutions[i].method != EVO_GIGANTAMAX)
			return TRUE;
	}

	return FALSE;
}

bool8 CanEvolve(struct Pokemon* mon)
{
	return CanSpeciesEvolve(GetMonData(mon, MON_DATA_SPECIES, NULL));
}

bool8 CouldHaveEvolvedViaLevelUp(struct Pokemon* mon)
{
	u16 species = GetMonData(mon, MON_DATA_SPECIES, NULL);
	const struct Evolution* evolutions = gEvolutionTable[species];

	for (u32 i = 0; i < EVOS_PER_MON; ++i)
	{
		if (evolutions[i].method == EVO_NONE) //Most likely end of entries
			break; //Break now to save time
		else if (IsLevelUpEvolutionMethod(evolutions[i].method) && mon->level >= evolutions[i].param)
			return TRUE;
	}

	return FALSE;
}

bool8 EvolveSpeciesByLevel(u16* species, u8 level)
{
	const struct Evolution* evolutions;
	bool8 evolved = FALSE;

	START:
	evolutions = gEvolutionTable[*species];

	for (u32 i = 0; i < EVOS_PER_MON; ++i)
	{
		if (evolutions[i].method == EVO_NONE) //Most likely end of entries
			break; //Break now to save time
		else if ((IsLevelUpEvolutionMethod(evolutions[i].method) && level >= evolutions[i].param)
		||  (IsOtherEvolutionMethod(evolutions[i].method) && level >= 40)
		||  (IsItemEvolutionMethod(evolutions[i].method) && level >= 45)
		||  (IsFriendshipEvolutionMethod(evolutions[i].method) && level >= 55))
		{
			*species = evolutions[i].targetSpecies;
			evolved = TRUE;
			goto START; //Evolve until it can't evolve any more
		}
	}

	return evolved;
}

u32 GetBaseStatsTotal(const u16 species)
{
	u32 sum = 0;
	u8* ptr = &gBaseStats[species].baseHP;

	for (int i = 0; i < NUM_STATS; ++i)
		sum += ptr[i];

	return sum;
}

static u8 TryRandomizeAbility(u8 originalAbility, unusedArg u16 species)
{
	u32 newAbility = originalAbility;

	#ifdef FLAG_ABILITY_RANDOMIZER
	if (FlagGet(FLAG_ABILITY_RANDOMIZER) && !FlagGet(FLAG_BATTLE_FACILITY)
	&& !gSpecialAbilityFlags[originalAbility].gRandomizerBannedOriginalAbilities) //This Ability can be changed
	{
		u32 id = T1_READ_32(gSaveBlock2->playerTrainerId);
		u16 startAt = (id & 0xFFFF) % (u32) ABILITIES_COUNT + species;
		u16 xorVal = (id >> 16) % (u32) 0xFF; //Only set the bits likely to be in the ability
		u32 numAttempts = 0;

		newAbility = originalAbility + startAt;
		if (newAbility >= ABILITIES_COUNT)
		{
			u16 overflow = newAbility - (ABILITIES_COUNT - 2);
			newAbility = overflow;
		}

		newAbility ^= xorVal;
		newAbility %= (u32) ABILITIES_COUNT; //Prevent overflow

		while (gSpecialAbilityFlags[newAbility].gRandomizerBannedNewAbilities && numAttempts < 100)
		{
			newAbility *= xorVal; //Multiply this time
			newAbility %= (u32) ABILITIES_COUNT;
			++numAttempts;
		}

		if (numAttempts >= 100 && gSpecialAbilityFlags[newAbility].gRandomizerBannedNewAbilities) //If the Ability is still banned
			newAbility = originalAbility; //Just use the original ability
		else if (newAbility == ABILITY_NONE) //Somehow wound up with no Ability
			newAbility = originalAbility; //Just use the original ability
	}
	#endif

	return newAbility;
}

u8 GetAbility1(const u16 species)
{
	return TryRandomizeAbility(gBaseStats[species].ability1, species);
}

u8 GetAbility2(const u16 species)
{
	return TryRandomizeAbility(gBaseStats[species].ability2, species);
}

u8 GetHiddenAbility(const u16 species)
{
	return TryRandomizeAbility(gBaseStats[species].hiddenAbility, species);
}

u8 FindMovePositionInMonMoveset(u16 move, struct Pokemon* mon)
{
	u8 i;

	for (i = 0; i < MAX_MON_MOVES; ++i)
	{
		if (GetMonData(mon, MON_DATA_MOVE1 + i, NULL) == move)
			break;
	}

	return i;
}

bool8 MoveInMonMoveset(u16 move, struct Pokemon* mon)
{
	return FindMovePositionInMonMoveset(move, mon) < MAX_MON_MOVES;
}

bool8 AllHittingMoveWithTypeInMonMoveset(struct Pokemon* mon, u8 moveType)
{
	for (u32 i = 0; i < MAX_MON_MOVES; ++i)
	{
		u16 move = GetMonData(mon, MON_DATA_MOVE1 + i, NULL);

		if (gBattleMoves[move].target & MOVE_TARGET_ALL
		&& GetMonMoveTypeSpecial(mon, move) == moveType)
			return TRUE;
	}

	return FALSE;
}

u8 GetMonType(struct Pokemon* mon, u8 typeId)
{
	u16 species = GetMonData(mon, MON_DATA_SPECIES, NULL);

	if (typeId == 0)
		return (ShouldReplaceTypesWithCamomons()) ? GetCamomonsTypeByMon(mon, 0) : gBaseStats[species].type1;
	else
		return (ShouldReplaceTypesWithCamomons()) ? GetCamomonsTypeByMon(mon, 1) : gBaseStats[species].type2;
}

bool8 IsMonOfType(struct Pokemon* mon, u8 type)
{
	u8 type1 = GetMonType(mon, 0);
	u8 type2 = GetMonType(mon, 1);

	return type1 == type || type2 == type;
}

bool8 IsSpeciesOfType(u16 species, u8 type)
{
	return gBaseStats[species].type1 == type || gBaseStats[species].type2 == type;
}

bool8 IsSpeciesAffectedByScalemons(u16 species)
{
	if (species == SPECIES_SHEDINJA) //Shedinja would get OP stats because of its low HP and BST
		return FALSE;

	if (IsOnlyScalemonsGame() && CanSpeciesEvolve(species))
		return FALSE; //Only Pokemon that are fully evolved are affected by the scaling outside of the Frontier

	return TRUE;
}

u8 GetVisualBaseStat(u8 statId, u16 species) //For the Pokedex screen
{
	u16 base = ((u8*) (&gBaseStats[species].baseHP))[statId];

	if (statId != STAT_HP && IsScaleMonsBattle() && IsSpeciesAffectedByScalemons(species))
	{
		u8 baseHP = gBaseStats[species].baseHP;
		base = MathMin((base * (600 - baseHP)) / (GetBaseStatsTotal(species) - baseHP), 255); //Max 255
	}

	return base;
}

/*#define TILE_SIZE 32
#define SPRITE_RAM 0x6010000
#define sSpriteTileAllocBitmap ((u8*) 0x2021B48)
#define FREE_SPRITE_TILE(n) (sSpriteTileAllocBitmap[(n) / 8] &= ~(1 << ((n) % 8)))

void DestroyMonIconSprite(struct Sprite* sprite)
{
	u16 tile = sprite->oam.tileNum;
	u8* tiles = (u8*)((tile * TILE_SIZE) + SPRITE_RAM);
	Memset(tiles, 0, 0x200);

	for (int i = tile; i < tile + 16; ++i)
		FREE_SPRITE_TILE(i);

	ResetSprite(sprite);
}*/

bool8 CanPartyMonBeGeneralStatused(struct Pokemon* mon)
{
	u16 species = GetMonData(mon, MON_DATA_SPECIES, NULL);
	u8 type1 = GetMonType(mon, 0);
	u8 type2 = GetMonType(mon, 1);

	#ifdef SPECIES_MINIOR_SHIELD
	if (species == SPECIES_MINIOR_SHIELD)
		return FALSE;
	#endif

	switch (GetMonAbility(mon)) {
		case ABILITY_COMATOSE:
			return FALSE;

		case ABILITY_FLOWERVEIL:
			if (type1 == TYPE_GRASS
			||  type2 == TYPE_GRASS)
				return FALSE;
	}

	return TRUE;
}

bool8 CanPartyMonBePutToSleep(struct Pokemon* mon)
{
	if (!CanPartyMonBeGeneralStatused(mon))
		return FALSE;

	switch (GetMonAbility(mon)) {
		case ABILITY_INSOMNIA:
		#ifdef ABILITY_VITALSPIRIT
		case ABILITY_VITALSPIRIT:
		#endif
		case ABILITY_SWEETVEIL:
			return FALSE;
	}

	return TRUE;
}

bool8 CanPartyMonBePoisoned(struct Pokemon* mon)
{
	u8 type1 = GetMonType(mon, 0);
	u8 type2 = GetMonType(mon, 1);

	if (!CanPartyMonBeGeneralStatused(mon))
		return FALSE;

	switch (GetMonAbility(mon)) {
		case ABILITY_IMMUNITY:
		case ABILITY_PASTELVEIL:
			return FALSE;
	}

	if (type1 == TYPE_POISON
	||  type2 == TYPE_POISON
	||  type1 == TYPE_STEEL
	||  type2 == TYPE_STEEL)
		return FALSE;

	return TRUE;
}

bool8 CanPartyMonBeParalyzed(struct Pokemon* mon)
{
	u8 type1 = GetMonType(mon, 0);
	u8 type2 = GetMonType(mon, 1);

	if (!CanPartyMonBeGeneralStatused(mon))
		return FALSE;

	switch (GetMonAbility(mon)) {
		case ABILITY_LIMBER:
			return FALSE;
	}

	if (type1 == TYPE_ELECTRIC
	||  type2 == TYPE_ELECTRIC)
		return FALSE;

	return TRUE;
}

bool8 CanPartyMonBeBurned(struct Pokemon* mon)
{
	u8 type1 = GetMonType(mon, 0);
	u8 type2 = GetMonType(mon, 1);

	if (!CanPartyMonBeGeneralStatused(mon))
		return FALSE;

	switch (GetMonAbility(mon)) {
		case ABILITY_WATERVEIL:
		case ABILITY_WATERBUBBLE:
			return FALSE;
	}

	if (type1 == TYPE_FIRE
	||  type2 == TYPE_FIRE)
		return FALSE;

	return TRUE;
}

bool8 CanPartyMonBeFrozen(struct Pokemon* mon)
{
	u8 type1 = GetMonType(mon, 0);
	u8 type2 = GetMonType(mon, 1);

	if (!CanPartyMonBeGeneralStatused(mon))
		return FALSE;

	switch (GetMonAbility(mon)) {
		case ABILITY_MAGMAARMOR:
			return FALSE;
	}

	if (type1 == TYPE_ICE
	||  type2 == TYPE_ICE)
		return FALSE;

	return TRUE;
}
