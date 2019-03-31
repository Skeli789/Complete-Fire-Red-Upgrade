#pragma once

#include "defines.h"


//#define gStatStageRatios ((u8**) 0x825DEAD)
#define gBitTable ((u32*) 0x825E45C)

extern struct BattleMove gBattleMoves[];
//#define gBattleMoves ((struct BattleMove*) 0x8900000)
//#defin gMoveNames ((

#define gItems ((struct Item*) *((u32*) 0x80001C8))

typedef u8 SpeciesNames_t[POKEMON_NAME_LENGTH + 1];
#define gSpeciesNames ((SpeciesNames_t*) *((u32*) 0x8000144))
#define gBaseStats ((struct BaseStats*) *((u32*) 0x80001BC))

extern const u8 gTypeEffectiveness[124 * 3];

extern const struct Evolution gEvolutionTable[][EVOS_PER_MON];

//typedef u32 ExperienceTable_t[101];
//#define gExperienceTables ((ExperienceTable_t*) 0x8253AE4)
extern const u32 gExperienceTables[][255 + 1];

extern const u8 gAbilityNames[][ABILITY_NAME_LENGTH + 1];

#define BattleScript_MoveEnd (u8*) 0x81D694E
#define BattleScript_Atk49 (u8*) 0x81D6954
extern u8 BattleScript_ButItFailed[];

#define gGameVersion (*((u8*) 0x81E9F10))

#define gBattleTerrainTable ((struct BattleBackground*) *((u32*) 0x800F320))

typedef u8 TrainerClassNames_t[13];
#define gTrainerClassNames ((TrainerClassNames_t*) *((u32*) 0x811B4B4)) //0x823E558
#define gTrainers ((struct Trainer*) *((u32*) 0x800FC00)) //0x823EAC8
#define gTrainerFrontPicCoords ((struct TrainerPicCoords*) 0x823932C)
#define gTrainerFrontPicTable ((struct CompressedSpriteSheet*) 0x823957C)
#define gTrainerFrontPicPaletteTable ((struct CompressedSpritePalette*) 0x8239A1C)
extern struct TrainerPicCoords gTrainerBackPicCoords[];
#define gTrainerMoneyTable ((struct TrainerMoney*) 0x824F220)

#define sATypeMove_Table ((u8**) 0x83FEA28) //Table of things like "A Normal Move!"
#define gBattleStringsTable ((u8**) 0x83FDF3C)

#define gStatusConditionString_Poison ((u8*) 0x8250094)
#define gStatusConditionString_Sleep ((u8*) 0x825009C)
#define gStatusConditionString_Paralysis ((u8*) 0x82500A4)
#define gStatusConditionString_Burn ((u8*) 0x82500AC)
#define gStatusConditionString_Ice ((u8*) 0x82500B4)
#define gStatusConditionString_Confusion ((u8*) 0x82500BC)
#define gStatusConditionString_Love ((u8*) 0x82500C4)

#define gDummySpriteAffineAnimTable (const union AffineAnimCmd* const*) 0x8231CFC

#define gWildMonHeaders ((struct WildPokemonHeader*) *((u32*) 0x8082990))
extern u8 gUnownDistributionByChamber[7][12]; //[NUM_ROOMS][NUM_WILD_INDEXES]

#define SafariZoneEndScript ((void*) 0x81bfbc5)
