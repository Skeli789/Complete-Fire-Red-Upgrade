#include "defines.h"
#include "helper_functions.h"

extern const struct TerrainTableStruct TerrainTable[];

void DoFormChange(u8 bank, u16 species, u8 ReloadType);
void SwitchOutFormsRevert(u8 bank);
void FormsRevert(pokemon_t* party);
void UpdateBurmy(void);

//Make sure to update all functions that call DoFormChange to account for the fact the EmitSetMonData takes time!
//On second thought, just make sure the form change works

void DoFormChange(u8 bank, u16 species, u8 ReloadType) {
	gActiveBattler = bank;
	
	pokemon_t* partydata = GetBankPartyData(bank);
	partydata->backupSpecies = partydata->species; //Figure out if Emit is needed for this
	
	gBattleMons[bank].species = species;
	partydata->species = species; //Needed so the right types and abilities can be loaded
	EmitSetMonData(0, REQUEST_SPECIES_BATTLE, 0, 2, &gBattleMons[bank].species);
	MarkBufferBankForExecution(gActiveBattler);
	
	CalculateMonStats(partydata);
	RELOAD_BATTLE_STATS(bank, partydata);
	
	if (ReloadType) {
		gBattleMons[bank].type1 = gBaseStats[species].type1;
		gBattleMons[bank].type2 = gBaseStats[species].type2;
		gBattleMons[bank].type3 = TYPE_BLANK;
	}
	
	gStatuses3[bank] &= ~STATUS3_ILLUSION; //A Pokemon undergoing form change can't be hidden under Illusion
}

void SwitchOutFormsRevert(u8 bank) {
	switch (gBattleMons[bank].species) {
		case PKMN_CHERRIMSUN:
		case PKMN_DARMANITANZEN:
		case PKMN_MELOETTA_PIROUETTE:
		case PKMN_AEGISLASH_BLADE:
		case PKMN_WISHIWASHI_S:
		case PKMN_MINIORSHIELD:
			DoFormChange(bank, GetBankPartyData(bank)->backupSpecies, FALSE);
	}
}

void FormsRevert(pokemon_t* party) {
	int i;
	u16 oldHP;
	
	for (i = 0; i < PARTY_SIZE; ++i) {
		if (party[i].backupSpecies) 
		{
			party[i].species = party[i].backupSpecies;
			party[i].backupSpecies = 0;
			oldHP = party[i].hp;
			CalculateMonStats(&party[i]);
			
			if (party[i].species == PKMN_ZYGARDE || party[i].species == PKMN_ZYGARDE_10)
				party[i].hp = MathMin(party[i].maxHP, oldHP);
		}
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

