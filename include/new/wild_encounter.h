#pragma once

#include "../global.h"
#include "../script.h"

/**
 * \file wild_encounter.h
 * \brief Contains functions relating to encountering Pokemon in the wild
 *		  and beginning battles with wild Pokemon.
 */

//Exported Functions
void CreateWildMon(u16 species, u8 level, u8 monHeaderIndex, bool8 purgeParty);
u8 PickUnownLetter(u16 species, u8 headerIndex);
void TryUpdateSwarm(void);
bool8 IsValidSwarmIndex(u8 index);
u8 GetCurrentSwarmIndex(void);
void FishingWildEncounter(u8 rod);
bool8 DoesCurrentMapHaveFishingMons(void);
bool8 StandardWildEncounter(u32 currMetaTileBehavior, u16 previousMetaTileBehavior);
bool8 TryStandardWildEncounter(u32 metatileBehavior);
void RockSmashWildEncounter(void);
bool8 SweetScentWildEncounter(void);
bool8 StartRandomWildEncounter(bool8 waterMon); //Used in script specials
void DoStandardWildBattle(void);
void sp138_StartLegendaryBattle(void);
bool8 ScrCmd_setwildbattle(struct ScriptContext* ctx);
void TrySetWildDoubleBattleTypeScripted();
species_t GetLocalWildMon(bool8* isWaterMon);
u16 GetLocalWaterMon(void);
const struct WildPokemonInfo* LoadProperMonsData(u8 type);

//Exported COnstants
enum
{
	LAND_MONS_HEADER,
	WATER_MONS_HEADER,
	FISHING_MONS_HEADER,
	ROCK_SMASH_MONS_HEADER,
};

#define TILE_FLAG_ENCOUNTER_TILE 1
#define TILE_FLAG_SURFABLE 2
#define TILE_FLAG_WILD_DOUBLE 4
#define TILE_FLAG_SHAKING 8

#define ENCOUNTER_TYPE_LAND 0
#define ENCOUNTER_TYPE_WATER 1
