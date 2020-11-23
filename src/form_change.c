#include "defines.h"
#include "defines_battle.h"

#include "../include/new/battle_terrain.h"
#include "../include/new/battle_util.h"
#include "../include/new/dns.h"
#include "../include/new/form_change.h"
#include "../include/new/frontier.h"
#include "../include/new/util.h"
#include "../include/new/set_z_effect.h"
/*
form_change.c
	functions/structures/arrays that handle species that change form

tables to edit:
	gMiniorCores
	sBannedBackupSpecies

*/

const species_t gMiniorCores[] =
{
	SPECIES_MINIOR_RED,
	SPECIES_MINIOR_BLUE,
	SPECIES_MINIOR_ORANGE,
	SPECIES_MINIOR_YELLOW,
	SPECIES_MINIOR_INDIGO,
	SPECIES_MINIOR_GREEN,
	SPECIES_MINIOR_VIOLET,
	SPECIES_TABLES_TERMIN
};

static const species_t sBannedBackupSpecies[] =
{
	SPECIES_CHERRIM_SUN,
	SPECIES_SHAYMIN_SKY,
	SPECIES_DARMANITANZEN,
	SPECIES_KELDEO_RESOLUTE,
	SPECIES_MELOETTA_PIROUETTE,
	SPECIES_AEGISLASH_BLADE,
	SPECIES_WISHIWASHI_S,
	SPECIES_MIMIKYU_BUSTED,
	SPECIES_MINIOR_SHIELD,
	SPECIES_TABLES_TERMIN
};

//This file's functions:
void DoFormChange(u8 bank, u16 species, bool8 ReloadType, bool8 ReloadStats, bool8 reloadAbility)
{
	u16 backup;
	gActiveBattler = bank;

	struct Pokemon* mon = GetBankPartyData(bank);
	backup = mon->species;

	gBattleMons[bank].species = species;
	SetMonData(mon, MON_DATA_SPECIES, &species); //Needed so the right stats, types, and abilities can be loaded

	if (ReloadStats)
	{
		if (IsDynamaxed(bank))
			mon->maxHP = MathMax(mon->maxHP / GetDynamaxHPBoost(bank) + (mon->maxHP & 1), 1);

		CalculateMonStats(mon);

		if (IsDynamaxed(bank))
			mon->maxHP *= GetDynamaxHPBoost(bank);

		RELOAD_BATTLE_STATS(bank, mon);
	}

	if (reloadAbility)
		*GetAbilityLocation(bank) = GetMonAbility(mon);

	EmitSetMonData(0, REQUEST_FORM_CHANGE_BATTLE, 0, sizeof(struct BattlePokemon), &gBattleMons[bank]);
	MarkBufferBankForExecution(gActiveBattler);

	if (ReloadType)
	{
		if (gBattleTypeFlags & BATTLE_TYPE_CAMOMONS) //The Pokemon takes on the types of its first two moves
			UpdateTypesForCamomons(bank);
		else
		{
			gBattleMons[bank].type1 = gBaseStats[species].type1;
			gBattleMons[bank].type2 = gBaseStats[species].type2;
		}

		gBattleMons[bank].type3 = TYPE_BLANK;
	}

	gStatuses3[bank] &= ~(STATUS3_SWITCH_IN_ABILITY_DONE | STATUS3_ILLUSION); //A Pokemon undergoing form change can't be hidden under Illusion

	SetMonData(mon, MON_DATA_SPECIES, &backup); //Backup species is written to by the form change handler
}

void SwitchOutFormsRevert(u8 bank)
{
	struct Pokemon* mon = GetBankPartyData(bank);
	u16 backupSpecies = mon->backupSpecies;

	switch (gBattleMons[bank].species) {
		#if (defined SPECIES_CHERRIM && defined SPECIES_CHERRIM_SUN)
		case SPECIES_CHERRIM_SUN:
			if (backupSpecies != SPECIES_NONE)
				DoFormChange(bank, backupSpecies, FALSE, TRUE, FALSE);
			else
				DoFormChange(bank, SPECIES_CHERRIM, FALSE, TRUE, FALSE);
			break;
		#endif

		#if (defined SPECIES_DARMANITAN && defined SPECIES_DARMANITANZEN)
		case SPECIES_DARMANITANZEN:
			if (backupSpecies != SPECIES_NONE)
				DoFormChange(bank, backupSpecies, FALSE, TRUE, FALSE);
			else
				DoFormChange(bank, SPECIES_CHERRIM, FALSE, TRUE, FALSE);
			break;
		#endif

		#if (defined SPECIES_KELDEO && defined SPECIES_KELDEO_RESOLUTE)
		case SPECIES_KELDEO_RESOLUTE:
			if (FindMovePositionInMoveset(MOVE_SECRETSWORD, bank) == MAX_MON_MOVES) //Doesn't know Secret Sword
				DoFormChange(bank, SPECIES_KELDEO, FALSE, TRUE, FALSE);
			break;
		#endif

		#if (defined SPECIES_MELOETTA && defined SPECIES_MELOETTA_PIROUETTE)
		case SPECIES_MELOETTA_PIROUETTE:
			if (backupSpecies != SPECIES_NONE)
				DoFormChange(bank, backupSpecies, FALSE, TRUE, FALSE);
			else
				DoFormChange(bank, SPECIES_MELOETTA, FALSE, TRUE, FALSE);
			break;
		#endif

		#if (defined SPECIES_AEGISLASH && defined SPECIES_AEGISLASH_BLADE)
		case SPECIES_AEGISLASH_BLADE:
			if (backupSpecies != SPECIES_NONE)
				DoFormChange(bank, backupSpecies, FALSE, TRUE, FALSE);
			else
				DoFormChange(bank, SPECIES_AEGISLASH, FALSE, TRUE, FALSE);
			break;
		#endif

		#if (defined SPECIES_WISHIWASHI && defined SPECIES_WISHIWASHI_S)
		case SPECIES_WISHIWASHI_S:
			if (backupSpecies != SPECIES_NONE)
				DoFormChange(bank, backupSpecies, FALSE, TRUE, FALSE);
			else
				DoFormChange(bank, SPECIES_WISHIWASHI, FALSE, TRUE, FALSE);
			break;
		#endif

		#ifdef SPECIES_MINIOR_SHIELD
		case SPECIES_MINIOR_SHIELD:
			if (backupSpecies != SPECIES_NONE)
				DoFormChange(bank, backupSpecies, FALSE, TRUE, FALSE);
			else
				DoFormChange(bank, GetMiniorCoreSpecies(mon), FALSE, TRUE, FALSE);
			break;
		#endif
	}
}

void FormsRevert(pokemon_t* party)
{
	int i;

	for (i = 0; i < PARTY_SIZE; ++i)
		TryFormRevert(&party[i]);
}

bool8 TryFormRevert(pokemon_t* mon)
{
	int i;
	u16 species = mon->species;
	u16 oldHP;

	if (IsMinior(species))
	{
		mon->species = GetMiniorCoreSpecies(mon); //Get Minior Colour
		CalculateMonStats(mon);
		return TRUE;
	}
	else if (IsGigantamaxSpecies(mon->backupSpecies) || CheckTableForSpecies(mon->backupSpecies, sBannedBackupSpecies)) //Forms the mon shouldn't revert to
	{
		mon->backupSpecies = SPECIES_NONE;
	}
	else if (mon->backupSpecies != SPECIES_NONE && mon->backupSpecies < NUM_SPECIES)
	{
		mon->species = mon->backupSpecies;
		mon->backupSpecies = SPECIES_NONE;
		oldHP = mon->hp;
		CalculateMonStats(mon);

		#if (defined SPECIES_ZYGARDE && defined SPECIES_ZYGARDE_10)
		if (mon->species == SPECIES_ZYGARDE || mon->species == SPECIES_ZYGARDE_10)
			mon->hp = MathMin(mon->maxHP, oldHP);
		#endif

		return TRUE;
	}
	#if (defined SPECIES_SHAYMIN && defined SPECIES_SHAYMIN_SKY)
	else if (mon->species == SPECIES_SHAYMIN_SKY)
	{
		if (IsNightTime())
		{
			mon->species = SPECIES_SHAYMIN; //Shaymin reverts to normal form at night
			CalculateMonStats(mon);
			return TRUE;
		}
	}
	#endif
	#if (defined SPECIES_KELDEO && SPECIES_KELDEO_RESOLUTE)
	else if (mon->species == SPECIES_KELDEO)
	{
		for (i = 0; i < MAX_MON_MOVES; ++i)
		{
			if (mon->moves[i] == MOVE_SECRETSWORD)
				break;
		}

		if (i != MAX_MON_MOVES) //Keldeo knows Secret Sword
		{
			mon->species = SPECIES_KELDEO_RESOLUTE;
			CalculateMonStats(mon);
			return TRUE;
		}
	}
	else if (mon->species == SPECIES_KELDEO_RESOLUTE)
	{
		for (i = 0; i < MAX_MON_MOVES; ++i)
		{
			if (mon->moves[i] == MOVE_SECRETSWORD)
				break;
		}

		if (i == MAX_MON_MOVES) //Keldeo doesn't know Secret Sword
		{
			mon->species = SPECIES_KELDEO;
			CalculateMonStats(mon);
			return TRUE;
		}
	}
	#endif

	return FALSE;
}

void UpdateBurmy(void)
{
	#ifdef SPECIES_BURMY
	int i;
	u16 form = gTerrainTable[gBattleTerrain].burmyForm;

	if (form != SPECIES_NONE)
	{
		for (i = 0; i < PARTY_SIZE; ++i)
		{
			u16 species = gPlayerParty[i].species;
			if (species == SPECIES_BURMY
			||  species == SPECIES_BURMY_SANDY
			||  species == SPECIES_BURMY_TRASH)
			{
				gPlayerParty[i].species = form;
				CalculateMonStats(&gPlayerParty[i]);
			}
		}
	}
	#endif
}

species_t GetMiniorCoreFromPersonality(u32 personality)
{
	return gMiniorCores[personality % (NELEMS(gMiniorCores) - 1)];
}

species_t GetMiniorCoreSpecies(struct Pokemon* mon)
{
	return GetMiniorCoreFromPersonality(mon->personality);
}

bool8 IsMinior(u16 species)
{
	return
	#ifdef SPECIES_MINIOR_SHIELD
	species == SPECIES_MINIOR_SHIELD ||
	#endif
	CheckTableForSpecies(species, gMiniorCores);
}

void HandleFormChange(void)
{
	struct Pokemon* mon = GetBankPartyData(gActiveBattler);
	struct BattlePokemon* battleMon = (struct BattlePokemon*) &gBattleBufferA[gActiveBattler][3];

	mon->backupSpecies = GetMonData(mon, MON_DATA_SPECIES, NULL);
	SetMonData(mon, MON_DATA_SPECIES, &battleMon->species);

	SetMonData(mon, MON_DATA_ATK, &battleMon->attack);
	SetMonData(mon, MON_DATA_DEF, &battleMon->defense);
	SetMonData(mon, MON_DATA_SPEED, &battleMon->speed);
	SetMonData(mon, MON_DATA_SPATK, &battleMon->spAttack);
	SetMonData(mon, MON_DATA_SPDEF, &battleMon->spDefense);

	SetMonData(mon, MON_DATA_HP, &battleMon->hp);
	SetMonData(mon, MON_DATA_MAX_HP, &battleMon->maxHP);
}

//Overworld Form Change Functions////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static u16 sTypeToArceusForm[NUMBER_OF_MON_TYPES] =
{
	[TYPE_NORMAL] =		0,
	[TYPE_FIGHTING] =	SPECIES_ARCEUS_FIGHT,
	[TYPE_FLYING] = 	SPECIES_ARCEUS_FLYING,
	[TYPE_POISON] = 	SPECIES_ARCEUS_POISON,
	[TYPE_GROUND] = 	SPECIES_ARCEUS_GROUND,
	[TYPE_ROCK] =		SPECIES_ARCEUS_ROCK,
	[TYPE_BUG] =		SPECIES_ARCEUS_BUG,
	[TYPE_GHOST] =		SPECIES_ARCEUS_GHOST,
	[TYPE_STEEL] =		SPECIES_ARCEUS_STEEL,
	[TYPE_MYSTERY] =	0,
	[TYPE_FIRE] =		SPECIES_ARCEUS_FIRE,
	[TYPE_WATER] =		SPECIES_ARCEUS_WATER,
	[TYPE_GRASS] =		SPECIES_ARCEUS_GRASS,
	[TYPE_ELECTRIC] =	SPECIES_ARCEUS_ELECTRIC,
	[TYPE_PSYCHIC] =	SPECIES_ARCEUS_PSYCHIC,
	[TYPE_ICE] =		SPECIES_ARCEUS_ICE,
	[TYPE_DRAGON] = 	SPECIES_ARCEUS_DRAGON,
	[TYPE_DARK] =		SPECIES_ARCEUS_DARK,
	[TYPE_ROOSTLESS] =	SPECIES_ARCEUS_FLYING, //This Arceus should stay in the proper form

	[TYPE_FAIRY] =		SPECIES_ARCEUS_FAIRY
};

static u16 sTypeToSilvallyForm[NUMBER_OF_MON_TYPES] =
{
	[TYPE_NORMAL] = 	0,
	[TYPE_FIGHTING] = 	SPECIES_SILVALLY_FIGHT,
	[TYPE_FLYING] = 	SPECIES_SILVALLY_FLYING,
	[TYPE_POISON] = 	SPECIES_SILVALLY_POISON,
	[TYPE_GROUND] = 	SPECIES_SILVALLY_GROUND,
	[TYPE_ROCK] = 		SPECIES_SILVALLY_ROCK,
	[TYPE_BUG] = 		SPECIES_SILVALLY_BUG,
	[TYPE_GHOST] =		SPECIES_SILVALLY_GHOST,
	[TYPE_STEEL] =		SPECIES_SILVALLY_STEEL,
	[TYPE_MYSTERY] = 	0,
	[TYPE_FIRE] = 		SPECIES_SILVALLY_FIRE,
	[TYPE_WATER] =		SPECIES_SILVALLY_WATER,
	[TYPE_GRASS] =		SPECIES_SILVALLY_GRASS,
	[TYPE_ELECTRIC] = 	SPECIES_SILVALLY_ELECTRIC,
	[TYPE_PSYCHIC] = 	SPECIES_SILVALLY_PSYCHIC,
	[TYPE_ICE] = 		SPECIES_SILVALLY_ICE,
	[TYPE_DRAGON] = 	SPECIES_SILVALLY_DRAGON,
	[TYPE_DARK] =		SPECIES_SILVALLY_DARK,
	[TYPE_ROOSTLESS] =	SPECIES_SILVALLY_FLYING, //This Silvally should stay in the proper form

	[TYPE_FAIRY] =		SPECIES_SILVALLY_FAIRY
};

void HoldItemFormChange(struct Pokemon* mon, u16 item)
{
	u16 targetSpecies = SPECIES_NONE;

	u16 species = GetMonData(mon, MON_DATA_SPECIES, NULL);
	u8 ability = GetMonAbility(mon);
	u8 itemEffect = ItemId_GetHoldEffect(item);
	u8 type = ItemId_GetHoldEffectParam(item);

	switch(species) {
		#if (defined SPECIES_GIRATINA && defined SPECIES_GIRATINA_ORIGIN)
		case SPECIES_GIRATINA:
			if (itemEffect == ITEM_EFFECT_GRISEOUS_ORB)
				targetSpecies = SPECIES_GIRATINA_ORIGIN;
			break;

		case SPECIES_GIRATINA_ORIGIN:
			if (itemEffect != ITEM_EFFECT_GRISEOUS_ORB)
				targetSpecies = SPECIES_GIRATINA;
			break;
		#endif

		#ifdef SPECIES_GENESECT
		case SPECIES_GENESECT:
		case SPECIES_GENESECT_BURN:
		case SPECIES_GENESECT_CHILL:
		case SPECIES_GENESECT_DOUSE:
		case SPECIES_GENESECT_SHOCK:
			if (itemEffect == ITEM_EFFECT_DRIVE)
			{
				switch (type) {
					case TYPE_WATER:
						targetSpecies = SPECIES_GENESECT_DOUSE;
						break;
					case TYPE_FIRE:
						targetSpecies = SPECIES_GENESECT_BURN;
						break;
					case TYPE_ICE:
						targetSpecies = SPECIES_GENESECT_CHILL;
						break;
					case TYPE_ELECTRIC:
						targetSpecies = SPECIES_GENESECT_SHOCK;
						break;
				}
			}

			if (targetSpecies == SPECIES_NONE)
				targetSpecies = SPECIES_GENESECT;
			break;
		#endif

		#ifdef SPECIES_ARCEUS
		case SPECIES_ARCEUS:
		case SPECIES_ARCEUS_FIGHT:
		case SPECIES_ARCEUS_FLYING:
		case SPECIES_ARCEUS_POISON:
		case SPECIES_ARCEUS_GROUND:
		case SPECIES_ARCEUS_ROCK:
		case SPECIES_ARCEUS_BUG:
		case SPECIES_ARCEUS_GHOST:
		case SPECIES_ARCEUS_STEEL:
		case SPECIES_ARCEUS_FIRE:
		case SPECIES_ARCEUS_WATER:
		case SPECIES_ARCEUS_GRASS:
		case SPECIES_ARCEUS_ELECTRIC:
		case SPECIES_ARCEUS_PSYCHIC:
		case SPECIES_ARCEUS_ICE:
		case SPECIES_ARCEUS_DRAGON:
		case SPECIES_ARCEUS_DARK:
		case SPECIES_ARCEUS_FAIRY:
			if (ability == ABILITY_MULTITYPE) //Only transform if set with proper ability
			{
				if (itemEffect == ITEM_EFFECT_PLATE
				|| (itemEffect == ITEM_EFFECT_Z_CRYSTAL && !IsSpecialZCrystal(item)))
					targetSpecies = sTypeToArceusForm[type];

				if (targetSpecies == SPECIES_NONE)
					targetSpecies = SPECIES_ARCEUS;
			}
			break;
		#endif

		#ifdef SPECIES_SILVALLY
		case SPECIES_SILVALLY:
		case SPECIES_SILVALLY_FIGHT:
		case SPECIES_SILVALLY_FLYING:
		case SPECIES_SILVALLY_POISON:
		case SPECIES_SILVALLY_GROUND:
		case SPECIES_SILVALLY_ROCK:
		case SPECIES_SILVALLY_BUG:
		case SPECIES_SILVALLY_GHOST:
		case SPECIES_SILVALLY_STEEL:
		case SPECIES_SILVALLY_FIRE:
		case SPECIES_SILVALLY_WATER:
		case SPECIES_SILVALLY_GRASS:
		case SPECIES_SILVALLY_ELECTRIC:
		case SPECIES_SILVALLY_PSYCHIC:
		case SPECIES_SILVALLY_ICE:
		case SPECIES_SILVALLY_DRAGON:
		case SPECIES_SILVALLY_DARK:
		case SPECIES_SILVALLY_FAIRY:
			if (ability == ABILITY_RKS_SYSTEM) //Only transform if set with proper ability
			{
				if (itemEffect == ITEM_EFFECT_MEMORY)
					targetSpecies = sTypeToSilvallyForm[type];

				if (targetSpecies == SPECIES_NONE)
					targetSpecies = SPECIES_SILVALLY;
			}
			break;
		#endif
	}

	if (targetSpecies != SPECIES_NONE && targetSpecies != species)
	{
		SetMonData(mon, MON_DATA_SPECIES, &targetSpecies);
		CalculateMonStats(mon);
	}
}

void HoopaShayminPCRevertCheck(struct Pokemon* mon, bool8 recalcStats)
{
	u16 species = GetMonData(mon, MON_DATA_SPECIES, NULL);
	u16 targetSpecies = SPECIES_NONE;

	switch (species) {
		#if (defined HOOPA_CHANGE_IN_PC && defined SPECIES_HOOPA_UNBOUND && defined SPECIES_HOOPA)
		case SPECIES_HOOPA_UNBOUND:
			targetSpecies = SPECIES_HOOPA;
			break;
		#endif

		#if (defined SHAYMIN_CHANGE_IN_PC && defined SPECIES_SHAYMIN_SKY && defined SPECIES_SHAYMIN)
		case SPECIES_SHAYMIN_SKY:
			targetSpecies = SPECIES_SHAYMIN;
			break;
		#endif
	}

	if (targetSpecies != SPECIES_NONE)
	{
		SetMonData(mon, MON_DATA_SPECIES, &targetSpecies);
		if (recalcStats)
			CalculateMonStats(mon);
	}
}
