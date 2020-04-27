#include "defines.h"
#include "../include/random.h"
#include "../include/constants/abilities.h"

#include "../include/new/damage_calc.h"
#include "../include/new/evolution.h"
#include "../include/new/util.h"
#include "../include/new/frontier.h"
#include "../include/new/mega.h"
/*
util.c
	general utility functions
*/

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

bool8 CheckTableForMove(move_t move, const u16 table[])
{
	for (u32 i = 0; table[i] != MOVE_TABLES_TERMIN; ++i)
	{
		if (move == table[i])
			return TRUE;
	}

	return FALSE;
}

bool8 CheckTableForMoveEffect(move_t move, const u8 table[])
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
		if (GetMonData(&party[i], MON_DATA_SPECIES, NULL) != SPECIES_NONE
		&& !GetMonData(&party[i], MON_DATA_IS_EGG, NULL)
		&&  GetMonData(&party[i], MON_DATA_HP, NULL) > 0)
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

bool8 CanEvolve(struct Pokemon* mon)
{
	u16 species = GetMonData(mon, MON_DATA_SPECIES, NULL);
	const struct Evolution* evolutions = gEvolutionTable[species];

	for (u32 i = 0; i < EVOS_PER_MON; ++i)
	{
		if (evolutions[i].method != MEGA_EVOLUTION && evolutions[i].method != EVO_GIGANTAMAX && evolutions[i].method != 0)
			return TRUE;
	}

	return FALSE;
}

bool8 CouldHaveEvolvedViaLevelUp(struct Pokemon* mon)
{
	u16 species = GetMonData(mon, MON_DATA_SPECIES, NULL);
	const struct Evolution* evolutions = gEvolutionTable[species];

	for (u32 i = 0; i < EVOS_PER_MON; ++i)
	{
		if (IsLevelUpEvolutionMethod(evolutions[i].method) && mon->level >= evolutions[i].param)
			return TRUE;
	}

	return FALSE;
}

void EvolveSpeciesByLevel(u16* species, u8 level)
{
	const struct Evolution* evolutions;

	START:
	evolutions = gEvolutionTable[*species];

	for (u32 i = 0; i < EVOS_PER_MON; ++i)
	{
		if ((IsLevelUpEvolutionMethod(evolutions[i].method) && level >= evolutions[i].param)
		||  (IsOtherEvolutionMethod(evolutions[i].method) && level >= 40)
		||  (IsItemEvolutionMethod(evolutions[i].method) && level >= 50)
		||  (IsFriendshipEvolutionMethod(evolutions[i].method) && level >= 60))
		{
			*species = evolutions[i].targetSpecies;
			goto START; //Evolve until it can't evolve any more
		}
	}
}

u32 GetBaseStatsTotal(const u16 species)
{
	u32 sum = 0;
	u8* ptr = &gBaseStats[species].baseHP;

	for (int i = 0; i < NUM_STATS; ++i)
		sum += ptr[i];

	return sum;
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

#define TILE_SIZE 32
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
}

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
		case ABILITY_VITALSPIRIT:
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
