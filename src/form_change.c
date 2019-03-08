#include "defines.h"
#include "helper_functions.h"

extern const struct TerrainTableStruct TerrainTable[];

void DoFormChange(u8 bank, u16 species, bool8 ReloadType, bool8 ReloadStats) ;
void SwitchOutFormsRevert(u8 bank);
void FormsRevert(pokemon_t* party);
void TryFormRevert(pokemon_t* mon);
void UpdateBurmy(void);

void DoFormChange(u8 bank, u16 species, bool8 ReloadType, bool8 ReloadStats) {
	gActiveBattler = bank;
	
	pokemon_t* partydata = GetBankPartyData(bank);
	partydata->backupSpecies = partydata->species; //Figure out if Emit is needed for this
	
	gBattleMons[bank].species = species;
	partydata->species = species; //Needed so the right stats, types, and abilities can be loaded

	CalculateMonStats(partydata);
	RELOAD_BATTLE_STATS(bank, partydata);
	
	if (ReloadStats)
		EmitSetMonData(0, REQUEST_FORM_CHANGE_BATTLE, 0, sizeof(struct BattlePokemon), &gBattleMons[bank]);
	else
		EmitSetMonData(0, REQUEST_SPECIES_BATTLE, 0, 2, &gBattleMons[bank].species);
	
	MarkBufferBankForExecution(gActiveBattler);
	
	if (ReloadType) {
		gBattleMons[bank].type1 = gBaseStats[species].type1;
		gBattleMons[bank].type2 = gBaseStats[species].type2;
		gBattleMons[bank].type3 = TYPE_BLANK;
	}
	
	gStatuses3[bank] &= ~(STATUS3_SWITCH_IN_ABILITY_DONE | STATUS3_ILLUSION); //A Pokemon undergoing form change can't be hidden under Illusion
}

void SwitchOutFormsRevert(u8 bank) {
	switch (gBattleMons[bank].species) {
		case PKMN_CHERRIMSUN:
		case PKMN_DARMANITANZEN:
		case PKMN_MELOETTA_PIROUETTE:
		case PKMN_AEGISLASH_BLADE:
		case PKMN_WISHIWASHI_S:
		case PKMN_MINIORSHIELD:
			DoFormChange(bank, GetBankPartyData(bank)->backupSpecies, FALSE, TRUE);
	}
}

void FormsRevert(pokemon_t* party) {
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
			
		if (mon->species == PKMN_ZYGARDE || mon->species == PKMN_ZYGARDE_10)
			mon->hp = MathMin(mon->maxHP, oldHP);
	}
	else if (mon->species == PKMN_MINIORSHIELD) //Minior that has never had a colour yet (Eg. Wild)
	{
		mon->species = umodsi(mon->personality, 7); //Get Minior Colour
		CalculateMonStats(mon);
	}
}

void UpdateBurmy(void) {
	int i;
	u16 form = TerrainTable[gBattleTerrain].burmyForm;
	
	if (form != PKMN_NONE)
	{	
		for (i = 0; i < PARTY_SIZE; ++i) 
		{
			u16 species = gPlayerParty[i].species;
			if (species == PKMN_BURMY
			||  species == PKMN_BURMY_SANDY
			||  species == PKMN_BURMY_TRASH)
			{
				gPlayerParty[i].species = form;
				CalculateMonStats(&gPlayerParty[i]);
			}
		}
	}
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
