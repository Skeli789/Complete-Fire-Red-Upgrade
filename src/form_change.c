#include "defines.h"
#include "defines_battle.h"

#include "../include/new/battle_terrain.h"
#include "../include/new/form_change.h"
#include "../include/new/helper_functions.h"

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

void DoFormChange(u8 bank, u16 species, bool8 ReloadType, bool8 ReloadStats) 
{
	gActiveBattler = bank;
	
	pokemon_t* partydata = GetBankPartyData(bank);
	partydata->backupSpecies = partydata->species;
	
	gBattleMons[bank].species = species;
	partydata->species = species; //Needed so the right stats, types, and abilities can be loaded

	CalculateMonStats(partydata);
	RELOAD_BATTLE_STATS(bank, partydata);
	
	if (ReloadStats)
		EmitSetMonData(0, REQUEST_FORM_CHANGE_BATTLE, 0, sizeof(struct BattlePokemon), &gBattleMons[bank]);
	else
		EmitSetMonData(0, REQUEST_SPECIES_BATTLE, 0, 2, &gBattleMons[bank].species);
	
	MarkBufferBankForExecution(gActiveBattler);
	
	if (ReloadType) 
	{
		gBattleMons[bank].type1 = gBaseStats[species].type1;
		gBattleMons[bank].type2 = gBaseStats[species].type2;
		gBattleMons[bank].type3 = TYPE_BLANK;
	}
	
	gStatuses3[bank] &= ~(STATUS3_SWITCH_IN_ABILITY_DONE | STATUS3_ILLUSION); //A Pokemon undergoing form change can't be hidden under Illusion
}

//This function could have been much simpler if I didn't care about stupid people who
//would give people the below mentioned species before battle.
void SwitchOutFormsRevert(u8 bank) 
{
	struct Pokemon* mon = GetBankPartyData(bank);
	u16 backupSpecies = mon->backupSpecies;
	
	switch (gBattleMons[bank].species) {
		case SPECIES_CHERRIM_SUN:
			if (backupSpecies != SPECIES_NONE)
				DoFormChange(bank, backupSpecies, FALSE, TRUE);
			else
				DoFormChange(bank, SPECIES_CHERRIM, FALSE, TRUE);
			break;

		case SPECIES_DARMANITANZEN:
			if (backupSpecies != SPECIES_NONE)
				DoFormChange(bank, backupSpecies, FALSE, TRUE);
			else
				DoFormChange(bank, SPECIES_CHERRIM, FALSE, TRUE);
			break;

		case SPECIES_KELDEO_RESOLUTE:
			if (FindMovePositionInMoveset(MOVE_SECRETSWORD, bank) == MAX_MON_MOVES) //Doesn't know Secret Sword
				DoFormChange(bank, SPECIES_KELDEO, FALSE, TRUE);
			break;

		case SPECIES_MELOETTA_PIROUETTE:
			if (backupSpecies != SPECIES_NONE)
				DoFormChange(bank, backupSpecies, FALSE, TRUE);
			else
				DoFormChange(bank, SPECIES_MELOETTA, FALSE, TRUE);
			break;

		case SPECIES_AEGISLASH_BLADE:
			if (backupSpecies != SPECIES_NONE)
				DoFormChange(bank, backupSpecies, FALSE, TRUE);
			else
				DoFormChange(bank, SPECIES_AEGISLASH, FALSE, TRUE);
			break;

		case SPECIES_WISHIWASHI_S:
			if (backupSpecies != SPECIES_NONE)
				DoFormChange(bank, backupSpecies, FALSE, TRUE);
			else
				DoFormChange(bank, SPECIES_WISHIWASHI, FALSE, TRUE);
			break;

		case SPECIES_MINIOR_SHIELD:
			if (backupSpecies != SPECIES_NONE)
				DoFormChange(bank, backupSpecies, FALSE, TRUE);
			else
				DoFormChange(bank, GetMiniorCoreSpecies(mon), FALSE, TRUE);
			break;
	}
}

void FormsRevert(pokemon_t* party) 
{
	int i;
	
	for (i = 0; i < PARTY_SIZE; ++i)
		TryFormRevert(&party[i]);
}

void TryFormRevert(pokemon_t* mon)
{
	u16 oldHP;
	
	if (mon->backupSpecies) 
	{
		mon->species = mon->backupSpecies;
		mon->backupSpecies = 0;
		oldHP = mon->hp;
		CalculateMonStats(mon);
			
		if (mon->species == SPECIES_ZYGARDE || mon->species == SPECIES_ZYGARDE_10)
			mon->hp = MathMin(mon->maxHP, oldHP);
	}
	else if (mon->species == SPECIES_MINIOR_SHIELD) //Minior that has never had a colour yet (Eg. Wild)
	{
		mon->species = GetMiniorCoreSpecies(mon); //Get Minior Colour
		CalculateMonStats(mon);
	}
	else if (mon->species == SPECIES_KELDEO_RESOLUTE)
	{
		int i;
		for (i = 0; i < MAX_MON_MOVES; ++i)
		{
			if (mon->moves[i] == MOVE_SECRETSWORD)
				break;
		}
		
		if (i == MAX_MON_MOVES) //Keldeo doesn't know Secret Sword
		{
			mon->species = SPECIES_KELDEO;
			CalculateMonStats(mon);
		}
	}
}

void UpdateBurmy(void)
{
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
}

species_t GetMiniorCoreSpecies(struct Pokemon* mon)
{
	return gMiniorCores[mon->personality % (ARRAY_COUNT(gMiniorCores) - 1)];
}

void HandleFormChange(void)
{
	pokemon_t* mon = GetBankPartyData(gActiveBattler);
	struct BattlePokemon* battleMon = (struct BattlePokemon*) &gBattleBufferA[gActiveBattler][3];
	
	mon->species = battleMon->species;
	
	mon->attack = battleMon->attack;
	mon->defense = battleMon->defense;
	mon->speed = battleMon->speed;
	mon->spAttack = battleMon->spAttack;
	mon->spDefense = battleMon->spDefense;
	
	mon->hp = battleMon->hp;
	mon->maxHP = battleMon->maxHP;
}
