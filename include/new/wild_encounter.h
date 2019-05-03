#pragma once
#include "../../src/defines.h"
#include "../script.h"
#include "../wild_encounter.h"

enum
{
	LAND_MONS_HEADER,
	WATER_MONS_HEADER,
	FISHING_MONS_HEADER,
	ROCK_SMASH_MONS_HEADER,
};

//Exported Functions
void CreateWildMon(u16 species, u8 level, u8 monHeaderIndex, bool8 purgeParty);
void FishingWildEncounter(u8 rod);
bool8 DoesCurrentMapHaveFishingMons(void);
bool8 StandardWildEncounter(u16 currMetaTileBehavior, u16 previousMetaTileBehavior);
bool8 CheckStandardWildEncounter(u16 metatileBehavior);
void RockSmashWildEncounter(void);
bool8 SweetScentWildEncounter(void);
void DoStandardWildBattle(void);
void sp138_StartLegendaryBattle(void);
bool8 ScrCmd_setwildbattle(struct ScriptContext* ctx);
void TrySetWildDoubleBattleTypeScripted();
species_t GetLocalWildMon(bool8* isWaterMon);
u16 GetLocalWaterMon(void);
const struct WildPokemonInfo* LoadProperMonsData(u8 type);