#pragma once

#include "global.h"

#define LAND_WILD_COUNT     12
#define WATER_WILD_COUNT    5
#define ROCK_WILD_COUNT     5
#define FISH_WILD_COUNT     10

struct WildPokemon
{
    u8 minLevel;
    u8 maxLevel;
    u16 species;
};

struct WildPokemonInfo
{
   u8 encounterRate;
   const struct WildPokemon *wildPokemon;
};

struct WildPokemonHeader
{
    u8 mapGroup;
    u8 mapNum;
	u16 padding;
    const struct WildPokemonInfo *landMonsInfo;
    const struct WildPokemonInfo *waterMonsInfo;
    const struct WildPokemonInfo *rockSmashMonsInfo;
    const struct WildPokemonInfo *fishingMonsInfo;
};

#define gWildMonHeaders ((struct WildPokemonHeader*) *((u32*) 0x8082990))

struct SwarmData
{
	u8 mapName;
	u16 species;
};

u8 __attribute__((long_call)) ChooseWildMonIndex_Land(void);
u8 __attribute__((long_call)) ChooseWildMonIndex_WaterRock(void);
u8 __attribute__((long_call)) ChooseWildMonIndex_Fishing(u8 rod);
bool8 __attribute__((long_call)) CanEncounterUnownInTanobyRuins(void);
u32 __attribute__((long_call)) GenerateUnownPersonalityByLetter(u8 letter);
//bool8 __attribute__((long_call)) DoWildEncounterRateDiceRoll(u16 encounterRate);
bool8 __attribute__((long_call)) DoGlobalWildEncounterDiceRoll(void);
bool8 __attribute__((long_call)) IsWildLevelAllowedByRepel(u8 level);
void __attribute__((long_call)) ApplyFluteEncounterRateMod(u32 *encRate);
void __attribute__((long_call)) ApplyCleanseTagEncounterRateMod(u32 *encRate);
void __attribute__((long_call)) AddToWildEncounterRateBuff(u8 encounterRate);
u8  __attribute__((long_call)) GetUnownLetterFromPersonality(u32 personality);
bool8 __attribute__((long_call)) HandleWildEncounterCooldown(u32 currMetatileBehavior);
u8 __attribute__((long_call)) GetFluteEncounterRateModType(void);
