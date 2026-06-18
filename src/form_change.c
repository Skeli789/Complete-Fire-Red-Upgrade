#include "defines.h"
#include "defines_battle.h"
#include "../include/constants/items.h"
#include "../include/constants/region_map_sections.h"

#include "../include/new/battle_terrain.h"
#include "../include/new/battle_util.h"
#include "../include/new/dns.h"
#include "../include/new/evolution.h"
#include "../include/new/form_change.h"
#include "../include/new/frontier.h"
#include "../include/new/learn_move.h"
#include "../include/new/set_z_effect.h"
#include "../include/new/util.h"

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
	SPECIES_MORPEKO_HANGRY,
	SPECIES_EISCUE_NOICE,
	SPECIES_CRAMORANT_GULPING,
	SPECIES_CRAMORANT_GORGING,
	SPECIES_ZACIAN_CROWNED,
	SPECIES_ZAMAZENTA_CROWNED,
	SPECIES_DARMANITAN_G_ZEN,
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
	u8 ability = GetMonAbility(mon);

	if (IS_TRANSFORMED(bank))
		return;

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

		#if (defined SPECIES_MORPEKO && defined SPECIES_MORPEKO_HANGRY)
		case SPECIES_MORPEKO_HANGRY:
			if (backupSpecies != SPECIES_NONE)
				DoFormChange(bank, backupSpecies, FALSE, TRUE, FALSE);
			else
				DoFormChange(bank, SPECIES_MORPEKO, FALSE, TRUE, FALSE);
			break;
		#endif

		#if (defined SPECIES_CRAMORANT && defined SPECIES_CRAMORANT_GULPING && defined SPECIES_CRAMORANT_GORGING)
		case SPECIES_CRAMORANT_GULPING:
		case SPECIES_CRAMORANT_GORGING:
			if (backupSpecies != SPECIES_NONE)
				DoFormChange(bank, backupSpecies, FALSE, TRUE, FALSE);
			else
				DoFormChange(bank, SPECIES_CRAMORANT, FALSE, TRUE, FALSE);
			break;
		#endif

		#if (defined SPECIES_PALAFIN && defined SPECIES_PALAFIN_HERO)
		case SPECIES_PALAFIN:
			if (backupSpecies != SPECIES_NONE && ability == ABILITY_TORRENT)
				DoFormChange(bank, backupSpecies, FALSE, TRUE, FALSE);
			else
				DoFormChange(bank, SPECIES_PALAFIN_HERO, FALSE, TRUE, FALSE);
			break;
		#endif

	}
}

void FormsRevert(struct Pokemon* party)
{
	int i;

	for (i = 0; i < PARTY_SIZE; ++i)
		TryFormRevert(&party[i]);
}

bool8 TryFormRevert(struct Pokemon* mon)
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
		#ifdef SPECIES_ZACIAN
		if (mon->species == SPECIES_ZACIAN)
		{
			u8 moveIndex = FindMovePositionInMonMoveset(MOVE_BEHEMOTHBLADE, mon);
			if (moveIndex < MAX_MON_MOVES)
			{
				u16 newMove = MOVE_IRONHEAD; //Zacian's Behemoth Blade changes to Iron Head in its base forme
				SetMonData(mon, MON_DATA_MOVE1 + moveIndex, &newMove);
			}
		}
		#endif
		#ifdef SPECIES_ZAMAZENTA
		if (mon->species == SPECIES_ZAMAZENTA)
		{
			u8 moveIndex = FindMovePositionInMonMoveset(MOVE_BEHEMOTHBASH, mon);
			if (moveIndex < MAX_MON_MOVES)
			{
				u16 newMove = MOVE_IRONHEAD; //Zamazenta's Behemoth Bash changes to Iron Head in its base forme
				SetMonData(mon, MON_DATA_MOVE1 + moveIndex, &newMove);
			}
		}
		#endif

		return TRUE;
	}
	#if (defined SPECIES_SHAYMIN && defined SPECIES_SHAYMIN_SKY)
	else if (mon->species == SPECIES_SHAYMIN_SKY)
	{
		if (IsNightTime() && !(gBattleTypeFlags & BATTLE_TYPE_FRONTIER))
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
	#ifdef SPECIES_GIRATINA_ORIGIN
	else if (mon->species == SPECIES_GIRATINA_ORIGIN)
	{
		TryRevertOriginFormes(mon, FALSE);
	}
	#endif
	#if (defined SPECIES_DIALGA_ORIGIN && defined SPECIES_PALKIA_ORIGIN)
	else if (mon->species == SPECIES_DIALGA_ORIGIN || mon->species == SPECIES_PALKIA_ORIGIN)
	{
		TryRevertOriginFormes(mon, FALSE);
	}
	#endif
	#if (defined SPECIES_ETERNATUS && defined SPECIES_ETERNATUS_ETERNAMAX)
	else if (mon->species == SPECIES_ETERNATUS_ETERNAMAX) //If it was hacked in
	{
		mon->species = SPECIES_ETERNATUS;
		CalculateMonStats(mon);
		return TRUE;
	}
	#endif
	#if (defined SPECIES_ZACIAN && defined SPECIES_ZACIAN_CROWNED)
	else if (mon->species == SPECIES_ZACIAN_CROWNED) //If it was hacked in
	{
		mon->species = SPECIES_ZACIAN;
		u8 moveIndex = FindMovePositionInMonMoveset(MOVE_BEHEMOTHBLADE, mon);
		if (moveIndex < MAX_MON_MOVES)
		{
			u16 newMove = MOVE_IRONHEAD; //Zacian's Behemoth Blade changes to Iron Head in its base forme
			SetMonData(mon, MON_DATA_MOVE1 + moveIndex, &newMove);
		}
	}
	#endif
	#if (defined SPECIES_ZAMAZENTA && defined SPECIES_ZAMAZENTA_CROWNED)
	else if (mon->species == SPECIES_ZAMAZENTA_CROWNED) //If it was hacked in
	{
		mon->species = SPECIES_ZAMAZENTA;
		u8 moveIndex = FindMovePositionInMonMoveset(MOVE_BEHEMOTHBASH, mon);
		if (moveIndex < MAX_MON_MOVES)
		{
			u16 newMove = MOVE_IRONHEAD; //Zamazenta's Behemoth Bash changes to Iron Head in its base forme
			SetMonData(mon, MON_DATA_MOVE1 + moveIndex, &newMove);
		}
	}
	#endif
	#ifdef SPECIES_SHADOW_WARRIOR
	else if (mon->species == SPECIES_SHADOW_WARRIOR) //If it was hacked in
	{
		ZeroMonData(mon);
	}
	#endif

	return FALSE;
}

void UpdateBurmy(void)
{
	#ifdef SPECIES_BURMY
	int i;
	u16 form = gTerrainTable[GetBattleTerrainOverride() + 4].burmyForm;

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

void TryRevertOriginFormes(unusedArg struct Pokemon* mon, unusedArg bool8 ignoreDistortionWorld)
{
	u16 targetSpecies = SPECIES_NONE;
	u16 item = GetMonData(mon, MON_DATA_HELD_ITEM, NULL);
	u8 itemEffect = ItemId_GetHoldEffect(item);

	switch (GetMonData(mon, MON_DATA_SPECIES, NULL))
	{
		#if (defined SPECIES_GIRATINA && defined SPECIES_GIRATINA_ORIGIN)
		case SPECIES_GIRATINA_ORIGIN:
			if (itemEffect != ITEM_EFFECT_GRISEOUS_ORB
			#ifdef MAPSEC_DISTORTION_WORLD
			&& (ignoreDistortionWorld || GetCurrentRegionMapSectionId() != MAPSEC_DISTORTION_WORLD)
			#endif
			)
				targetSpecies = SPECIES_GIRATINA;
			break;
		#endif

		#ifdef PLA_HELD_ORIGIN_ORBS
		#if (defined SPECIES_DIALGA && defined SPECIES_DIALGA_ORIGIN)
		case SPECIES_DIALGA_ORIGIN:
			if (itemEffect != ITEM_EFFECT_ADAMANT_ORB)
				targetSpecies = SPECIES_DIALGA;
			break;
		#endif

		#if (defined SPECIES_PALKIA && defined SPECIES_PALKIA_ORIGIN)
		case SPECIES_PALKIA_ORIGIN:
			if (itemEffect != ITEM_EFFECT_LUSTROUS_ORB)
				targetSpecies = SPECIES_PALKIA;
			break;
		#endif
		#endif
	}

	if (targetSpecies != SPECIES_NONE)
		SetMonData(mon, MON_DATA_SPECIES, &targetSpecies);
}

species_t GetMiniorCoreFromPersonality(u32 personality)
{
	return gMiniorCores[personality % (NELEMS(gMiniorCores) - 1)];
}

species_t GetMiniorCoreSpecies(struct Pokemon* mon)
{
	return GetMiniorCoreFromPersonality(mon->personality);
}

bool8 IsMiniorCore(u16 species)
{
	return CheckTableForSpecies(species, gMiniorCores);
}

bool8 IsMinior(u16 species)
{
	return
		#ifdef SPECIES_MINIOR_SHIELD
		species == SPECIES_MINIOR_SHIELD ||
		#endif
		IsMiniorCore(species);
}

void HandleFormChange(void)
{
	struct Pokemon* mon = GetBankPartyData(gActiveBattler);
	struct BattlePokemon* battleMon = (struct BattlePokemon*) &gBattleBufferA[gActiveBattler][3];
	#ifdef UNBOUND
	u8 oldGender = GetMonGender(mon);
	#endif

	mon->backupSpecies = GetMonData(mon, MON_DATA_SPECIES, NULL);
	SetMonData(mon, MON_DATA_SPECIES, &battleMon->species);

	#ifdef UNBOUND
	//Try fix changed genders
	typedef void (*ChangeMonGender_T) (struct Pokemon*);
	#define ChangeMonGender ((ChangeMonGender_T) (0x0801D834 |1))
	if (gBaseStats[mon->backupSpecies].genderRatio != gBaseStats[battleMon->species].genderRatio
	&& oldGender != GetMonGender(mon))
		ChangeMonGender(mon);
	#endif

	SetMonData(mon, MON_DATA_ATK, &battleMon->attack);
	SetMonData(mon, MON_DATA_DEF, &battleMon->defense);
	SetMonData(mon, MON_DATA_SPEED, &battleMon->speed);
	SetMonData(mon, MON_DATA_SPATK, &battleMon->spAttack);
	SetMonData(mon, MON_DATA_SPDEF, &battleMon->spDefense);

	SetMonData(mon, MON_DATA_HP, &battleMon->hp);
	SetMonData(mon, MON_DATA_MAX_HP, &battleMon->maxHP);
}

void TryCrownZacianZamazenta(struct Pokemon* party)
{
	//Zacian and Zamazenta only transform at the beginning of battle
	#if (defined SPECIES_ZACIAN && defined SPECIES_ZAMAZENTA)
	u32 i;
	for (i = 0; i < PARTY_SIZE; ++i)
	{
		u16 newSpecies, newMove;
		struct Pokemon* mon = &party[i];
		u16 species = GetMonData(mon, MON_DATA_SPECIES2, NULL);
		u16 itemEffect = ItemId_GetHoldEffect(GetMonData(mon, MON_DATA_HELD_ITEM, NULL));
		bool8 transform = FALSE;

		if (species == SPECIES_ZACIAN && itemEffect == ITEM_EFFECT_RUSTED_SWORD)
		{
			newSpecies = SPECIES_ZACIAN_CROWNED;
			newMove = MOVE_BEHEMOTHBLADE;
			transform = TRUE;
		}
		else if (species == SPECIES_ZAMAZENTA && itemEffect == ITEM_EFFECT_RUSTED_SHIELD)
		{
			newSpecies = SPECIES_ZAMAZENTA_CROWNED;
			newMove = MOVE_BEHEMOTHBASH;
			transform = TRUE;
		}

		if (transform)
		{
			party[i].backupSpecies = species;
			SetMonData(mon, MON_DATA_SPECIES, &newSpecies);
			CalculateMonStats(mon);
			
			u8 moveIndex = FindMovePositionInMonMoveset(newMove, mon);
			if (moveIndex < MAX_MON_MOVES) //Uh, oh... already knows the Behemoth move
			{
				moveIndex = FindMovePositionInMonMoveset(MOVE_IRONHEAD, mon);
				if (moveIndex < MAX_MON_MOVES) //And also knows Iron Head
				{
					SetMonMoveSlot(mon, MOVE_NONE, moveIndex);
					RemoveMonPPBonus(mon, moveIndex);
					for (i = moveIndex; i < MAX_MON_MOVES - 1; ++i)
						ShiftMoveSlot(mon, i, i + 1);
				}
			}
			else
			{
				moveIndex = FindMovePositionInMonMoveset(MOVE_IRONHEAD, mon);
				if (moveIndex < MAX_MON_MOVES)
				{
					u8 newPP = CalculatePPWithBonus(newMove, GetMonData(mon, MON_DATA_PP_BONUSES, NULL), moveIndex);
					SetMonData(mon, MON_DATA_MOVE1 + moveIndex, &newMove); //Iron Head changes when changing forms
					if (GetMonData(mon, MON_DATA_PP1 + moveIndex, NULL) > newPP)
						SetMonData(mon, MON_DATA_PP1 + moveIndex, &newPP); //Adjust PP
				}
			}
		}
	}
	#endif
}

void TrySetCorrectToxtricityForm(struct BoxPokemon* mon)
{
	u16 species = GetBoxMonData(mon, MON_DATA_SPECIES2, NULL);

	if (species == SPECIES_TOXTRICITY || species == SPECIES_TOXTRICITY_LOW_KEY)
	{
		if (HasHighNature((struct Pokemon*) mon))
			species = SPECIES_TOXTRICITY;
		else
			species = SPECIES_TOXTRICITY_LOW_KEY;
	}
	else
		species = SPECIES_NONE;

	if (species != SPECIES_NONE)
		SetBoxMonData(mon, MON_DATA_SPECIES, &species); //Set the correct form
}

//Overworld Form Change Functions////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const u16 sTypeToArceusForm[NUMBER_OF_MON_TYPES] =
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

	[TYPE_FAIRY] =		SPECIES_ARCEUS_FAIRY,
	[TYPE_STELLAR] =    0
};

const u16 gTypeToSilvallyForm[NUMBER_OF_MON_TYPES] =
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

	[TYPE_FAIRY] =		SPECIES_SILVALLY_FAIRY,
	[TYPE_STELLAR] =    0
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
			if (itemEffect != ITEM_EFFECT_GRISEOUS_ORB
			#ifdef MAPSEC_DISTORTION_WORLD
			&& GetCurrentRegionMapSectionId() != MAPSEC_DISTORTION_WORLD
			#endif
			)
				targetSpecies = SPECIES_GIRATINA;
			break;
		#endif

		#ifdef PLA_HELD_ORIGIN_ORBS
		#if (defined SPECIES_DIALGA && defined SPECIES_DIALGA_ORIGIN)
		case SPECIES_DIALGA:
			if (itemEffect == ITEM_EFFECT_ADAMANT_ORB)
				targetSpecies = SPECIES_DIALGA_ORIGIN;
			break;

		case SPECIES_DIALGA_ORIGIN:
			if (itemEffect != ITEM_EFFECT_ADAMANT_ORB)
				targetSpecies = SPECIES_DIALGA;
			break;
		#endif

		#if (defined SPECIES_PALKIA && defined SPECIES_PALKIA_ORIGIN)
		case SPECIES_PALKIA:
			if (itemEffect == ITEM_EFFECT_LUSTROUS_ORB)
				targetSpecies = SPECIES_PALKIA_ORIGIN;
			break;

		case SPECIES_PALKIA_ORIGIN:
			if (itemEffect != ITEM_EFFECT_LUSTROUS_ORB)
				targetSpecies = SPECIES_PALKIA;
			break;
		#endif
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
					targetSpecies = gTypeToSilvallyForm[type];

				if (targetSpecies == SPECIES_NONE)
					targetSpecies = SPECIES_SILVALLY;
			}
			else
				targetSpecies = SPECIES_SILVALLY;
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
