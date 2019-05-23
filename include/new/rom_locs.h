#pragma once

#include "../../src/defines.h"
#include "../constants/species.h"

//#define gStatStageRatios ((u8**) 0x825DEAD)
#define gBitTable ((u32*) 0x825E45C)

#define gItems ((struct Item*) *((u32*) 0x80001C8))

typedef u8 SpeciesNames_t[POKEMON_NAME_LENGTH + 1];
#define gSpeciesNames ((SpeciesNames_t*) *((u32*) 0x8000144))
#define gBaseStats ((struct BaseStats*) *((u32*) 0x80001BC))

typedef struct Evolution EvolutionTableT[EVOS_PER_MON];
#define gEvolutionTable ((EvolutionTableT*) *((u32*) 0x8042F6C))

//extern const struct Evolution gEvolutionTable[][EVOS_PER_MON];

//typedef u32 ExperienceTable_t[101];
//#define gExperienceTables ((ExperienceTable_t*) 0x8253AE4)
extern const u32 gExperienceTables[][255 + 1];

#define gMonIconPaletteIndices ((u8*) (*((u32*) 0x0800013C)))

#define gGameVersion (*((u8*) 0x81E9F10))
#define gGameLanguage (*((u8*) 0x81E9F11))

#define gDummySpriteAffineAnimTable (const union AffineAnimCmd* const*) 0x8231CFC

//#define gWildMonHeaders ((struct WildPokemonHeader*) *((u32*) 0x8082990))

#define SafariZoneEndScript ((void*) 0x81bfbc5)

#define sBasePaletteGammaTypes ((u8*) 0x83C2CC0)

//typedef int TutorLearnsets[NUM_SPECIES];
//#define sTutorLearnsets ((TutorLearnsets*) (*((u32*) 0x08120BE4)))


