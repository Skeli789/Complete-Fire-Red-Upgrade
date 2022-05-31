/*This file ONLY exists because util.c is being weird.*/

#include "defines.h"
#include "../include/random.h"
#include "../include/constants/abilities.h"

#include "../include/new/build_pokemon.h"
#include "../include/new/damage_calc.h"
#include "../include/new/evolution.h"
#include "../include/new/util2.h"
#include "../include/new/frontier.h"
#include "../include/new/mega.h"
#include "../include/new/dynamax.h"
#include "../include/constants/items.h"
#include "../include/constants/moves.h"
#include "../include/base_stats.h"
/*
util2.c
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
			&& GetMonData(&party[i], MON_DATA_HP, NULL) > 0)
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
		flags = (u8*)SEEN_DEX_FLAGS;
		break;

	default: //case FLAG_GET_CAUGHT:
		flags = (u8*)CAUGHT_DEX_FLAGS;
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
			|| (IsOtherEvolutionMethod(evolutions[i].method) && level >= 40)
			|| (IsItemEvolutionMethod(evolutions[i].method) && level >= 50)
			|| (IsFriendshipEvolutionMethod(evolutions[i].method) && level >= 60))
		{
			*species = evolutions[i].targetSpecies;
			goto START; //Evolve until it can't evolve any more
		}
	}
}

u32 GetBaseStatsTotal(const u16 species)
{
	u32 sum = 0;
	const u8* ptr = &gBaseStats2[species].baseHP;

	for (u32 i = 0; i < NUM_STATS; ++i)
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
		return (ShouldReplaceTypesWithCamomons()) ? GetCamomonsTypeByMon(mon, 0) : gBaseStats2[species].type1;
	else
		return (ShouldReplaceTypesWithCamomons()) ? GetCamomonsTypeByMon(mon, 1) : gBaseStats2[species].type2;
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
			|| type2 == TYPE_GRASS)
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
		|| type2 == TYPE_POISON
		|| type1 == TYPE_STEEL
		|| type2 == TYPE_STEEL)
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
		|| type2 == TYPE_ELECTRIC)
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
		|| type2 == TYPE_FIRE)
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
		|| type2 == TYPE_ICE)
		return FALSE;

	return TRUE;
}

void GetRandomStarter(void) {
	u8 num = Random() % 26;
	if (num == 0) {
		Var8008 = SPECIES_BULBASAUR;
	}
	if (num == 1) {
		Var8008 = SPECIES_SQUIRTLE;
	}
	if (num == 2) {
		Var8008 = SPECIES_CHARMANDER;
	}
	if (num == 3) {
		Var8008 = SPECIES_CHIKORITA;
	}
	if (num == 4) {
		Var8008 = SPECIES_TOTODILE;
	}
	if (num == 5) {
		Var8008 = SPECIES_CYNDAQUIL;
	}
	if (num == 6) {
		Var8008 = SPECIES_TREECKO;
	}
	if (num == 7) {
		Var8008 = SPECIES_MUDKIP;
	}
	if (num == 8) {
		Var8008 = SPECIES_TORCHIC;
	}
	if (num == 9) {
		Var8008 = SPECIES_TURTWIG;
	}
	if (num == 10) {
		Var8008 = SPECIES_PIPLUP;
	}
	if (num == 11) {
		Var8008 = SPECIES_CHIMCHAR;
	}
	if (num == 12) {
		Var8008 = SPECIES_SNIVY;
	}
	if (num == 13) {
		Var8008 = SPECIES_OSHAWOTT;
	}
	if (num == 14) {
		Var8008 = SPECIES_TEPIG;
	}
	if (num == 15) {
		Var8008 = SPECIES_CHESPIN;
	}
	if (num == 16) {
		Var8008 = SPECIES_FROAKIE;
	}
	if (num == 17) {
		Var8008 = SPECIES_FENNEKIN;
	}
	if (num == 18) {
		Var8008 = SPECIES_ROWLET;
	}
	if (num == 19) {
		Var8008 = SPECIES_POPPLIO;
	}
	if (num == 20) {
		Var8008 = SPECIES_LITTEN;
	}
	if (num == 21) {
		Var8008 = SPECIES_GROOKEY;
	}
	if (num == 22) {
		Var8008 = SPECIES_SOBBLE;
	}
	if (num == 23) {
		Var8008 = SPECIES_SCORBUNNY;
	}

}

void SetStarterSpeciesToBuy(void) {
	u8 pokmon = Var8005;
	switch (pokmon) {
	case 0:
		Var8007 = SPECIES_BULBASAUR;
		break;
	case 1:
		Var8007 = SPECIES_CHIKORITA;
		break;
	case 2:
		Var8007 = SPECIES_TREECKO;
		break;
	case 3:
		Var8007 = SPECIES_TURTWIG;
		break;
	case 4:
		Var8007 = SPECIES_SNIVY;
		break;
	case 5:
		Var8007 = SPECIES_CHESPIN;
		break;
	case 6:
		Var8007 = SPECIES_ROWLET;
		break;
	case 7:
		Var8007 = SPECIES_GROOKEY;
		break;
	case 8:
		Var8007 = SPECIES_CHARMANDER;
		break;
	case 9:
		Var8007 = SPECIES_CYNDAQUIL;
		break;
	case 10:
		Var8007 = SPECIES_TORCHIC;
		break;
	case 11:
		Var8007 = SPECIES_CHIMCHAR;
		break;
	case 12:
		Var8007 = SPECIES_TEPIG;
		break;
	case 13:
		Var8007 = SPECIES_FENNEKIN;
		break;
	case 14:
		Var8007 = SPECIES_LITTEN;
		break;
	case 15:
		Var8007 = SPECIES_SCORBUNNY;
		break;
	case 16:
		Var8007 = SPECIES_SQUIRTLE;
		break;
	case 17:
		Var8007 = SPECIES_TOTODILE;
		break;
	case 18:
		Var8007 = SPECIES_MUDKIP;
		break;
	case 19:
		Var8007 = SPECIES_PIPLUP;
		break;
	case 20:
		Var8007 = SPECIES_OSHAWOTT;
		break;
	case 21:
		Var8007 = SPECIES_FROAKIE;
		break;
	case 22:
		Var8007 = SPECIES_POPPLIO;
		break;
	case 23:
		Var8007 = SPECIES_SOBBLE;
		break;
	}
}

void SetPseudoOtherSpeciesToBuy(void) {
	u8 pokmon = Var8005;
	switch (pokmon) {
	case 0:
		Var8007 = SPECIES_DRATINI;
		Var8008 = 0xFA0;
		break;
	case 1:
		Var8007 = SPECIES_LARVITAR;
		Var8008 = 0xFA0;
		break;
	case 2:
		Var8007 = SPECIES_BAGON;
		Var8008 = 0xFA0;
		break;
	case 3:
		Var8007 = SPECIES_BELDUM;
		Var8008 = 0xFA0;
		break;
	case 4:
		Var8007 = SPECIES_GIBLE;
		Var8008 = 0xBB8;
		break;
	case 5:
		Var8007 = SPECIES_DEINO;
		Var8008 = 0xFA0;
		break;
	case 6:
		Var8007 = SPECIES_GOOMY;
		Var8008 = 0xFA0;
		break;
	case 7:
		Var8007 = SPECIES_JANGMO_O;
		Var8008 = 0xFA0;
		break;
	case 8:
		Var8007 = SPECIES_DREEPY;
		Var8008 = 0xFA0;
		break;
	case 9:
		Var8007 = SPECIES_ARON;
		Var8008 = 0xBB8;
		break;
	case 10:
		Var8007 = SPECIES_TRAPINCH;
		Var8008 = 0xBB8;
		break;
	case 11:
		Var8007 = SPECIES_RIOLU;
		Var8008 = 0xBB8;
		break;
	case 12:
		Var8007 = SPECIES_AXEW;
		Var8008 = 0xBB8;
		break;
	case 13:
		Var8007 = SPECIES_LARVESTA;
		Var8008 = 0xBB8;
		break;
	case 14:
		Var8007 = SPECIES_ZORUA;
		Var8008 = 0xBB8;
		break;
	case 15:
		Var8007 = SPECIES_NOIBAT;
		Var8008 = 0xBB8;
		break;
	case 16:
		Var8007 = SPECIES_HONEDGE;
		Var8008 = 0xBB8;
		break;
	case 17:
		Var8007 = SPECIES_WIMPOD;
		Var8008 = 0xBB8;
		break;
	case 18:
		Var8007 = SPECIES_MIMIKYU;
		Var8008 = 0xBB8;
		break;
	case 19:
		Var8007 = SPECIES_MELTAN;
		Var8008 = 0xBB8;
		break;
	case 20:
		Var8007 = SPECIES_PORYGON;
		Var8008 = 0xBB8;
		break;
	case 21:
		Var8007 = SPECIES_RATTATA_A;
		Var8008 = 0x9C4;
		break;
	case 22:
		Var8007 = SPECIES_RAICHU_A;
		Var8008 = 0x9C4;
		break;
	case 23:
		Var8007 = SPECIES_SANDSHREW_A;
		Var8008 = 0x9C4;
		break;
	case 24:
		Var8007 = SPECIES_VULPIX_A;
		Var8008 = 0x9C4;
		break;
	case 25:
		Var8007 = SPECIES_DIGLETT_A;
		Var8008 = 0x9C4;
		break;
	case 26:
		Var8007 = SPECIES_MEOWTH_A;
		Var8008 = 0x9C4;
		break;
	case 27:
		Var8007 = SPECIES_GEODUDE_A;
		Var8008 = 0x9C4;
		break;
	case 28:
		Var8007 = SPECIES_GRIMER_A;
		Var8008 = 0x9C4;
		break;
	case 29:
		Var8007 = SPECIES_EXEGGUTOR_A;
		Var8008 = 0x9C4;
		break;
	case 30:
		Var8007 = SPECIES_MAROWAK_A;
		Var8008 = 0x9C4;
		break;
	case 31:
		Var8007 = SPECIES_MEOWTH_G;
		Var8008 = 0x9C4;
		break;
	case 32:
		Var8007 = SPECIES_PONYTA_G;
		Var8008 = 0x9C4;
		break;
	case 33:
		Var8007 = SPECIES_FARFETCHD_G;
		Var8008 = 0x9C4;
		break;
	case 34:
		Var8007 = SPECIES_WEEZING_G;
		Var8008 = 0x9C4;
		break;
	case 35:
		Var8007 = SPECIES_MR_MIME_G;
		Var8008 = 0x9C4;
		break;
	case 36:
		Var8007 = SPECIES_CORSOLA_G;
		Var8008 = 0x9C4;
		break;
	case 37:
		Var8007 = SPECIES_ZIGZAGOON_G;
		Var8008 = 0x9C4;
		break;
	case 38:
		Var8007 = SPECIES_DARUMAKA_G;
		Var8008 = 0x9C4;
		break;
	case 39:
		Var8007 = SPECIES_YAMASK_G;
		Var8008 = 0x9C4;
		break;
	case 40:
		Var8007 = SPECIES_STUNFISK_G;
		Var8008 = 0x9C4;
		break;
	case 41:
		Var8007 = SPECIES_SLOWPOKE_G;
		Var8008 = 0x9C4;
		break;
	}
}


void MaxMonIV(void) {
	struct Pokemon* mon = &gPlayerParty[Var8004];
	u16 perfect = 31;
	switch (Var8005) {
	case 0:
		SetMonData(mon, MON_DATA_HP_IV, &perfect);
		break;
	case 1:
		SetMonData(mon, MON_DATA_ATK_IV, &perfect);
		break;
	case 2:
		SetMonData(mon, MON_DATA_DEF_IV, &perfect);
		break;
	case 3:
		SetMonData(mon, MON_DATA_SPATK_IV, &perfect);
		break;
	case 4:
		SetMonData(mon, MON_DATA_SPDEF_IV, &perfect);
		break;
	case 5:
		SetMonData(mon, MON_DATA_SPEED_IV, &perfect);
		break;
	}
	CalculateMonStats(mon);
}

void SetChosenMonNature(void)
{
	struct Pokemon* mon = &gPlayerParty[Var8004];
	u8 nature = Var8005; //Place nature in Var8005

	GiveMonNatureAndAbility(mon, nature, GetMonData(mon, MON_DATA_PERSONALITY, NULL) & 1, IsMonShiny(mon), TRUE, FALSE);
	CalculateMonStats(mon);
}

void GetSpeciesNumFromParty(void) {
	struct Pokemon* mon = &gPlayerParty[Var8004];
	Var8008 = GetMonData(mon, MON_DATA_SPECIES, 0);
}

void SetBallVar(void) {
	switch (Var8005) {
	case 0:
		Var8006 = ITEM_MASTER_BALL;
		break;
	case 1:
		Var8006 = ITEM_ULTRA_BALL;
		break;
	case 2:
		Var8006 = ITEM_GREAT_BALL;
		break;
	case 3:
		Var8006 = ITEM_POKE_BALL;
		break;
	case 4:
		Var8006 = ITEM_SAFARI_BALL;
		break;
	case 5:
		Var8006 = ITEM_NET_BALL;
		break;
	case 6:
		Var8006 = ITEM_DIVE_BALL;
		break;
	case 7:
		Var8006 = ITEM_NEST_BALL;
		break;
	case 8:
		Var8006 = ITEM_REPEAT_BALL;
		break;
	case 9:
		Var8006 = ITEM_TIMER_BALL;
		break;
	case 10:
		Var8006 = ITEM_LUXURY_BALL;
		break;
	case 11:
		Var8006 = ITEM_PREMIER_BALL;
		break;
	case 12:
		Var8006 = ITEM_DUSK_BALL;
		break;
	case 13:
		Var8006 = ITEM_HEAL_BALL;
		break;
	case 14:
		Var8006 = ITEM_QUICK_BALL;
		break;
	case 15:
		Var8006 = ITEM_CHERISH_BALL;
		break;
	case 16:
		Var8006 = ITEM_PARK_BALL;
		break;
	case 17:
		Var8006 = ITEM_FAST_BALL;
		break;
	case 18:
		Var8006 = ITEM_LEVEL_BALL;
		break;
	case 19:
		Var8006 = ITEM_LURE_BALL;
		break;
	case 20:
		Var8006 = ITEM_HEAVY_BALL;
		break;
	case 21:
		Var8006 = ITEM_LOVE_BALL;
		break;
	case 22:
		Var8006 = ITEM_FRIEND_BALL;
		break;
	case 23:
		Var8006 = ITEM_MOON_BALL;
		break;
	case 24:
		Var8006 = ITEM_SPORT_BALL;
		break;
	case 25:
		Var8006 = ITEM_BEAST_BALL;
		break;
	case 26:
		Var8006 = ITEM_DREAM_BALL;
		break;
	}
}

void GetBallFromMon(void) {
	struct Pokemon* mon = &gPlayerParty[Var8004];
	Var8009 = mon->pokeball + 1;
}
void PartyNumToSpecies(void) {
	struct Pokemon* mon = &gPlayerParty[Var8004];
	Var800A = mon->species;
}



void SetGreninjaAbility(void) {
	struct Pokemon* mon = &gPlayerParty[Var8004];
	u8 ability = GetMonAbility(mon);
	if (ability == ABILITY_PROTEAN || ability == ABILITY_TORRENT) {
		mon->hiddenAbility = FALSE;
		GiveMonNatureAndAbility(mon, GetNature(mon), 2, IsMonShiny(mon), FALSE, FALSE);
		Var8008 = 0x0;
		return;
	}
	else if (ability == ABILITY_BATTLEBOND) {
		mon->hiddenAbility = TRUE;
		Var8008 = 0x1;
		return;
	}
}

void GiveMonHiddenAbility(void) {
	struct Pokemon* mon = &gPlayerParty[Var8004]; //put mon party num in var8004
	if (mon->hiddenAbility == TRUE) {
		mon->hiddenAbility = FALSE;
	}
	else {
		mon->hiddenAbility = TRUE;
	}
}