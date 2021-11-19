#include "../config.h"
#include "../../include/global.h"
#include "../../include/battle.h"
#include "../../include/constants/moves.h"
#include "../../include/constants/species.h"

#include "../../include/new/battle_terrain.h"
/*
terrain_tables.c
	define tables for each battle terrain type, as well as camouflage type based on the terrain

tables to edit:
	gTerrainTable
	gAttackTerrainTable
	gCamouflageColours

*/

#define gBattleTerrainAnimTiles_Building (void*) 0x824E410
#define gBattleTerrainAnimTilemap_Building (void*) 0x824E490

extern const u8 BG_Electric_TerrainTiles[];
extern const u8 BG_Electric_TerrainMap[];
extern const u8 BG_Electric_TerrainPal[];
extern const u8 BG_Grassy_TerrainTiles[];
extern const u8 BG_Grassy_TerrainMap[];
extern const u8 BG_Grassy_TerrainPal[];
extern const u8 BG_Misty_TerrainTiles[];
extern const u8 BG_Misty_TerrainMap[];
extern const u8 BG_Misty_TerrainPal[];
extern const u8 BG_Psychic_TerrainTiles[];
extern const u8 BG_Psychic_TerrainMap[];
extern const u8 BG_Psychic_TerrainPal[];

const struct TerrainTableStruct gTerrainTable[] =
{
	[ELECTRIC_TERRAIN - 1] =
		{
			.camouflageType = TYPE_ELECTRIC,
			.secretPowerEffect = MOVE_EFFECT_PARALYSIS,
			.secretPowerAnim = MOVE_THUNDERSHOCK,
			.naturePowerMove = MOVE_THUNDERBOLT,
		},

	[GRASSY_TERRAIN - 1] =
		{
			.camouflageType = TYPE_GRASS,
			.secretPowerEffect = MOVE_EFFECT_SLEEP,
			.secretPowerAnim = MOVE_VINEWHIP,
			.naturePowerMove = MOVE_ENERGYBALL,
		},

	[MISTY_TERRAIN - 1] =
		{
			.camouflageType = TYPE_FAIRY,
			.secretPowerEffect = MOVE_EFFECT_SP_ATK_MINUS_1,
			.secretPowerAnim = MOVE_FAIRYWIND,
			.naturePowerMove = MOVE_MOONBLAST,
		},

	[PSYCHIC_TERRAIN - 1] =
		{
			.camouflageType = TYPE_PSYCHIC,
			.secretPowerEffect = MOVE_EFFECT_SPD_MINUS_1,
			.secretPowerAnim = MOVE_CONFUSION,
			.naturePowerMove = MOVE_PSYCHIC,
		},

	[BATTLE_TERRAIN_GRASS + 4] =
		{
			.camouflageType = TYPE_GRASS,
			.secretPowerEffect = MOVE_EFFECT_SLEEP,
			.secretPowerAnim = MOVE_VINEWHIP,
			.naturePowerMove = MOVE_ENERGYBALL,
			.burmyForm = SPECIES_BURMY,
		},

	[BATTLE_TERRAIN_LONG_GRASS + 4] =
		{
			.camouflageType = TYPE_GRASS,
			.secretPowerEffect = MOVE_EFFECT_SLEEP,
			.secretPowerAnim = MOVE_VINEWHIP,
			.naturePowerMove = MOVE_ENERGYBALL,
			.burmyForm = SPECIES_BURMY,
		},

	[BATTLE_TERRAIN_SAND + 4] =
		{
			.camouflageType = TYPE_GROUND,
			.secretPowerEffect = MOVE_EFFECT_ACC_MINUS_1,
			.secretPowerAnim = MOVE_MUDSLAP,
			.naturePowerMove = MOVE_EARTHPOWER,
			.burmyForm = SPECIES_BURMY_SANDY,
		},

	[BATTLE_TERRAIN_UNDERWATER + 4] =
		{
			.camouflageType = TYPE_WATER,
			.secretPowerEffect = MOVE_EFFECT_ATK_MINUS_1,
			.secretPowerAnim = MOVE_WATERPULSE,
			.naturePowerMove = MOVE_HYDROPUMP,
			.burmyForm = SPECIES_NONE,
		},

	[BATTLE_TERRAIN_WATER + 4] =
		{
			.camouflageType = TYPE_WATER,
			.secretPowerEffect = MOVE_EFFECT_ATK_MINUS_1,
			.secretPowerAnim = MOVE_WATERPULSE,
			.naturePowerMove = MOVE_HYDROPUMP,
			.burmyForm = SPECIES_NONE,
		},

	[BATTLE_TERRAIN_POND + 4] =
		{
			.camouflageType = TYPE_WATER,
			.secretPowerEffect = MOVE_EFFECT_ATK_MINUS_1,
			.secretPowerAnim = MOVE_WATERPULSE,
			.naturePowerMove = MOVE_HYDROPUMP,
			.burmyForm = SPECIES_NONE,
		},

	[BATTLE_TERRAIN_MOUNTAIN + 4] =
		{
			.camouflageType = TYPE_GROUND,
			.secretPowerEffect = MOVE_EFFECT_ACC_MINUS_1,
			.secretPowerAnim = MOVE_MUDSLAP,
			.naturePowerMove = MOVE_EARTHPOWER,
			.burmyForm = SPECIES_BURMY_SANDY,
		},

	[BATTLE_TERRAIN_CAVE + 4] =
		{
			.camouflageType = TYPE_ROCK,
			.secretPowerEffect = MOVE_EFFECT_FLINCH,
			.secretPowerAnim = MOVE_ROCKTHROW,
			.naturePowerMove = MOVE_POWERGEM,
			.burmyForm = SPECIES_BURMY_SANDY,
		},

	[BATTLE_TERRAIN_INSIDE + 4] =
		{
			.camouflageType = TYPE_NORMAL,
			.secretPowerEffect = MOVE_EFFECT_PARALYSIS,
			.secretPowerAnim = MOVE_BODYSLAM,
			.naturePowerMove = MOVE_TRIATTACK,
			.burmyForm = SPECIES_BURMY_TRASH,
		},

	[BATTLE_TERRAIN_PLAIN + 4] =
		{
			.camouflageType = TYPE_NORMAL,
			.secretPowerEffect = MOVE_EFFECT_PARALYSIS,
			.secretPowerAnim = MOVE_SLAM,
			.naturePowerMove = MOVE_TRIATTACK,
			.burmyForm = SPECIES_BURMY,
		},

	[BATTLE_TERRAIN_INSIDE_2 + 4] =
		{
			.camouflageType = TYPE_NORMAL,
			.secretPowerEffect = MOVE_EFFECT_PARALYSIS,
			.secretPowerAnim = MOVE_BODYSLAM,
			.naturePowerMove = MOVE_TRIATTACK,
			.burmyForm = SPECIES_BURMY_TRASH,
		},

	[BATTLE_TERRAIN_INSIDE_3 + 4] =
		{
			.camouflageType = TYPE_NORMAL,
			.secretPowerEffect = MOVE_EFFECT_PARALYSIS,
			.secretPowerAnim = MOVE_BODYSLAM,
			.naturePowerMove = MOVE_TRIATTACK,
			.burmyForm = SPECIES_BURMY_TRASH,
		},

	[BATTLE_TERRAIN_INSIDE_4 + 4] =
		{
			.camouflageType = TYPE_NORMAL,
			.secretPowerEffect = MOVE_EFFECT_PARALYSIS,
			.secretPowerAnim = MOVE_BODYSLAM,
			.naturePowerMove = MOVE_TRIATTACK,
			.burmyForm = SPECIES_BURMY_TRASH,
		},

	[BATTLE_TERRAIN_INSIDE_5 + 4] =
		{
			.camouflageType = TYPE_NORMAL,
			.secretPowerEffect = MOVE_EFFECT_PARALYSIS,
			.secretPowerAnim = MOVE_BODYSLAM,
			.naturePowerMove = MOVE_TRIATTACK,
			.burmyForm = SPECIES_BURMY_TRASH,
		},

	[BATTLE_TERRAIN_INSIDE_6 + 4] =
		{
			.camouflageType = TYPE_NORMAL,
			.secretPowerEffect = MOVE_EFFECT_PARALYSIS,
			.secretPowerAnim = MOVE_BODYSLAM,
			.naturePowerMove = MOVE_TRIATTACK,
			.burmyForm = SPECIES_BURMY_TRASH,
		},

#ifndef UNBOUND
	[BATTLE_TERRAIN_LORLEI + 4] =
		{
			.camouflageType = TYPE_ICE,
			.secretPowerEffect = MOVE_EFFECT_FREEZE,
			.secretPowerAnim = MOVE_ICESHARD,
			.naturePowerMove = MOVE_ICEBEAM,
			.burmyForm = SPECIES_BURMY_TRASH,
		},

	[BATTLE_TERRAIN_BRUNO + 4] =
		{
			.camouflageType = TYPE_FIGHTING,
			.secretPowerEffect = MOVE_EFFECT_FLINCH,
			.secretPowerAnim = MOVE_KARATECHOP,
			.naturePowerMove = MOVE_FOCUSBLAST,
			.burmyForm = SPECIES_BURMY_TRASH,
		},

	[BATTLE_TERRAIN_AGATHA + 4] =
		{
			.camouflageType = TYPE_GHOST,
			.secretPowerEffect = MOVE_EFFECT_EVS_MINUS_1,
			.secretPowerAnim = MOVE_SHADOWSNEAK,
			.naturePowerMove = MOVE_SHADOWBALL,
			.burmyForm = SPECIES_BURMY_TRASH,
		},

	[BATTLE_TERRAIN_LANCE + 4] =
		{
			.camouflageType = TYPE_DRAGON,
			.secretPowerEffect = MOVE_EFFECT_ATK_MINUS_1,
			.secretPowerAnim = MOVE_DRAGONRAGE,
			.naturePowerMove = MOVE_DRAGONPULSE,
			.burmyForm = SPECIES_BURMY_TRASH,
		},

	[BATTLE_TERRAIN_CHAMPION + 4] =
		{
			.camouflageType = TYPE_NORMAL,
			.secretPowerEffect = MOVE_EFFECT_PARALYSIS,
			.secretPowerAnim = MOVE_BODYSLAM,
			.naturePowerMove = MOVE_TRIATTACK,
			.burmyForm = SPECIES_BURMY_TRASH,
		},
#endif
	//ADD NEW ENTRIES HERE

#ifdef UNBOUND //The ones below are for Pokemon Unbound. Feel free to remove.
	[BATTLE_TERRAIN_MOLEMAN + 4] =
		{
			.camouflageType = TYPE_GROUND,
			.secretPowerEffect = MOVE_EFFECT_ACC_MINUS_1,
			.secretPowerAnim = MOVE_MUDSLAP,
			.naturePowerMove = MOVE_EARTHPOWER,
			.burmyForm = SPECIES_BURMY_TRASH,
		},

	[BATTLE_TERRAIN_ELIAS + 4] =
		{
			.camouflageType = TYPE_GHOST,
			.secretPowerEffect = MOVE_EFFECT_EVS_MINUS_1,
			.secretPowerAnim = MOVE_SHADOWSNEAK,
			.naturePowerMove = MOVE_SHADOWBALL,
			.burmyForm = SPECIES_BURMY_TRASH,
		},

	[BATTLE_TERRAIN_ANABELLE + 4] =
		{
			.camouflageType = TYPE_FAIRY,
			.secretPowerEffect = MOVE_EFFECT_SP_ATK_MINUS_1,
			.secretPowerAnim = MOVE_FAIRYWIND,
			.naturePowerMove = MOVE_MOONBLAST,
			.burmyForm = SPECIES_BURMY_TRASH,
		},

	[BATTLE_TERRAIN_PENNY + 4] =
		{
			.camouflageType = TYPE_DRAGON,
			.secretPowerEffect = MOVE_EFFECT_ALL_STATS_UP | MOVE_EFFECT_AFFECTS_USER,
			.secretPowerAnim = MOVE_DRAGONRAGE,
			.naturePowerMove = MOVE_DRAGONPULSE,
			.burmyForm = SPECIES_BURMY_TRASH,
		},

	[BATTLE_TERRAIN_CHAMPION + 4] =
		{
			.camouflageType = TYPE_NORMAL,
			.secretPowerEffect = MOVE_EFFECT_PARALYSIS,
			.secretPowerAnim = MOVE_BODYSLAM,
			.naturePowerMove = MOVE_HYPERBEAM,
			.burmyForm = SPECIES_BURMY_TRASH,
		},

	[BATTLE_TERRAIN_SNOW_FIELD + 4] =
		{
			.camouflageType = TYPE_ICE,
			.secretPowerEffect = MOVE_EFFECT_FREEZE,
			.secretPowerAnim = MOVE_AVALANCHE,
			.naturePowerMove = MOVE_ICEBEAM,
			.burmyForm = SPECIES_NONE,
		},

	[BATTLE_TERRAIN_VOLCANO + 4] =
		{
			.camouflageType = TYPE_FIRE,
			.secretPowerEffect = MOVE_EFFECT_BURN,
			.secretPowerAnim = MOVE_INCINERATE,
			.naturePowerMove = MOVE_LAVAPLUME,
			.burmyForm = SPECIES_BURMY_SANDY,
		},

	[BATTLE_TERRAIN_DARK_CAVE + 4] =
		{
			.camouflageType = TYPE_ROCK,
			.secretPowerEffect = MOVE_EFFECT_FLINCH,
			.secretPowerAnim = MOVE_ROCKTHROW,
			.naturePowerMove = MOVE_POWERGEM,
			.burmyForm = SPECIES_BURMY_SANDY,
		},

	[BATTLE_TERRAIN_DARK_CAVE_WATER + 4] =
		{
			.camouflageType = TYPE_WATER,
			.secretPowerEffect = MOVE_EFFECT_ATK_MINUS_1,
			.secretPowerAnim = MOVE_WATERPULSE,
			.naturePowerMove = MOVE_HYDROPUMP,
			.burmyForm = SPECIES_NONE,
		},

	[BATTLE_TERRAIN_CRYSTAL_PEAK + 4] =
		{
			.camouflageType = TYPE_ROCK,
			.secretPowerEffect = MOVE_EFFECT_DEF_PLUS_1 | MOVE_EFFECT_AFFECTS_USER,
			.secretPowerAnim = MOVE_DIAMONDSTORM,
			.naturePowerMove = MOVE_POWERGEM,
			.burmyForm = SPECIES_BURMY_SANDY,
		},

	[BATTLE_TERRAIN_CRYSTAL_PEAK_WATER + 4] =
		{
			.camouflageType = TYPE_WATER,
			.secretPowerEffect = MOVE_EFFECT_ATK_MINUS_1,
			.secretPowerAnim = MOVE_WATERPULSE,
			.naturePowerMove = MOVE_HYDROPUMP,
			.burmyForm = SPECIES_NONE,
		},

	[BATTLE_TERRAIN_SNOW_CAVE + 4] =
		{
			.camouflageType = TYPE_ICE,
			.secretPowerEffect = MOVE_EFFECT_FREEZE,
			.secretPowerAnim = MOVE_AVALANCHE,
			.naturePowerMove = MOVE_FROSTBREATH,
			.burmyForm = SPECIES_BURMY_SANDY,
		},

	[BATTLE_TERRAIN_FOREST + 4] =
		{
			.camouflageType = TYPE_BUG,
			.secretPowerEffect = MOVE_EFFECT_POISON,
			.secretPowerAnim = MOVE_TWINEEDLE,
			.naturePowerMove = MOVE_BUGBUZZ,
			.burmyForm = SPECIES_BURMY,
		},

	[BATTLE_TERRAIN_ICE_IN_CAVE + 4] =
		{
			.camouflageType = TYPE_ICE,
			.secretPowerEffect = MOVE_EFFECT_FREEZE,
			.secretPowerAnim = MOVE_ICESHARD,
			.naturePowerMove = MOVE_ICEBEAM,
			.burmyForm = SPECIES_NONE,
		},

	[BATTLE_TERRAIN_RUINS_OF_VOID + 4] =
		{
			.camouflageType = TYPE_ROCK,
			.secretPowerEffect = MOVE_EFFECT_FLINCH,
			.secretPowerAnim = MOVE_ROCKTHROW,
			.naturePowerMove = MOVE_POWERGEM,
			.burmyForm = SPECIES_BURMY_SANDY,
		},

	[BATTLE_TERRAIN_DISTORTION_WORLD + 4] =
		{
			.camouflageType = TYPE_GHOST,
			.secretPowerEffect = MOVE_EFFECT_DEF_MINUS_1,
			.secretPowerAnim = MOVE_SHADOWSNEAK,
			.naturePowerMove = MOVE_SHADOWBALL,
			.burmyForm = SPECIES_BURMY_SANDY,
		},

	[BATTLE_TERRAIN_DISTORTION_WORLD_FULL + 4] =
		{
			.camouflageType = TYPE_GHOST,
			.secretPowerEffect = MOVE_EFFECT_DEF_MINUS_1,
			.secretPowerAnim = MOVE_SHADOWSNEAK,
			.naturePowerMove = MOVE_SHADOWBALL,
			.burmyForm = SPECIES_BURMY_SANDY,
		},

	[BATTLE_TERRAIN_GRASS_GYM + 4] =
		{
			.camouflageType = TYPE_GRASS,
			.secretPowerEffect = MOVE_EFFECT_SLEEP,
			.secretPowerAnim = MOVE_VINEWHIP,
			.naturePowerMove = MOVE_ENERGYBALL,
			.burmyForm = SPECIES_BURMY,
		},

	[BATTLE_TERRAIN_DESERT + 4] =
		{
			.camouflageType = TYPE_GROUND,
			.secretPowerEffect = MOVE_EFFECT_ACC_MINUS_1,
			.secretPowerAnim = MOVE_MUDSLAP,
			.naturePowerMove = MOVE_EARTHPOWER,
			.burmyForm = SPECIES_BURMY_SANDY,
		},

	[BATTLE_TERRAIN_WATER_IN_CAVE + 4] =
		{
			.camouflageType = TYPE_WATER,
			.secretPowerEffect = MOVE_EFFECT_ATK_MINUS_1,
			.secretPowerAnim = MOVE_WATERPULSE,
			.naturePowerMove = MOVE_HYDROPUMP,
			.burmyForm = SPECIES_NONE,
		},

	[BATTLE_TERRAIN_WATER_IN_SNOW_CAVE + 4] =
		{
			.camouflageType = TYPE_WATER,
			.secretPowerEffect = MOVE_EFFECT_ATK_MINUS_1,
			.secretPowerAnim = MOVE_WATERPULSE,
			.naturePowerMove = MOVE_HYDROPUMP,
			.burmyForm = SPECIES_NONE,
		},

	[BATTLE_TERRAIN_LAVA_IN_VOLCANO + 4] =
		{
			.camouflageType = TYPE_FIRE,
			.secretPowerEffect = MOVE_EFFECT_BURN,
			.secretPowerAnim = MOVE_INCINERATE,
			.naturePowerMove = MOVE_LAVAPLUME,
			.burmyForm = SPECIES_NONE,
		},

	[BATTLE_TERRAIN_WATER_IN_FOREST + 4] =
		{
			.camouflageType = TYPE_WATER,
			.secretPowerEffect = MOVE_EFFECT_ATK_MINUS_1,
			.secretPowerAnim = MOVE_WATERPULSE,
			.naturePowerMove = MOVE_HYDROPUMP,
			.burmyForm = SPECIES_NONE,
		},

	[BATTLE_TERRAIN_AUTUMN_GRASS + 4] =
		{
			.camouflageType = TYPE_GRASS,
			.secretPowerEffect = MOVE_EFFECT_SLEEP,
			.secretPowerAnim = MOVE_VINEWHIP,
			.naturePowerMove = MOVE_ENERGYBALL,
			.burmyForm = SPECIES_BURMY,
		},

	[BATTLE_TERRAIN_AUTUMN_PLAIN + 4] =
		{
			.camouflageType = TYPE_NORMAL,
			.secretPowerEffect = MOVE_EFFECT_PARALYSIS,
			.secretPowerAnim = MOVE_BODYSLAM,
			.naturePowerMove = MOVE_TRIATTACK,
			.burmyForm = SPECIES_BURMY,
		},

	[BATTLE_TERRAIN_SNOW_GRASS + 4] =
		{
			.camouflageType = TYPE_ICE,
			.secretPowerEffect = MOVE_EFFECT_FREEZE,
			.secretPowerAnim = MOVE_AVALANCHE,
			.naturePowerMove = MOVE_ICEBEAM,
			.burmyForm = SPECIES_BURMY,
		},

	[BATTLE_TERRAIN_NIGHTMARE + 4] =
		{
			.camouflageType = TYPE_DARK,
			.secretPowerEffect = MOVE_EFFECT_SLEEP,
			.secretPowerAnim = MOVE_FEINTATTACK,
			.naturePowerMove = MOVE_NIGHTDAZE,
			.burmyForm = SPECIES_NONE,
		},

	[BATTLE_TERRAIN_ANTISIS_CITY + 4] =
		{
			.camouflageType = TYPE_STEEL,
			.secretPowerEffect = MOVE_EFFECT_DEF_MINUS_1,
			.secretPowerAnim = MOVE_METALCLAW,
			.naturePowerMove = MOVE_FLASHCANNON,
			.burmyForm = SPECIES_BURMY_TRASH,
		},

	[BATTLE_TERRAIN_ANTISIS_SEWERS + 4] =
		{
			.camouflageType = TYPE_POISON,
			.secretPowerEffect = MOVE_EFFECT_TOXIC,
			.secretPowerAnim = MOVE_POISONTAIL,
			.naturePowerMove = MOVE_SLUDGEBOMB,
			.burmyForm = SPECIES_BURMY_TRASH,
		},

	[BATTLE_TERRAIN_ANTISIS_SEWERS_WATER + 4] =
		{
			.camouflageType = TYPE_POISON,
			.secretPowerEffect = MOVE_EFFECT_TOXIC,
			.secretPowerAnim = MOVE_POISONTAIL,
			.naturePowerMove = MOVE_SLUDGEWAVE,
			.burmyForm = SPECIES_NONE,
		},

	[BATTLE_TERRAIN_SKY_BATTLE + 4] =
		{
			.camouflageType = TYPE_FLYING,
			.secretPowerEffect = MOVE_EFFECT_FLINCH,
			.secretPowerAnim = MOVE_WINGATTACK,
			.naturePowerMove = MOVE_AIRSLASH,
			.burmyForm = SPECIES_NONE,
		},

	[BATTLE_TERRAIN_BOG + 4] =
		{
			.camouflageType = TYPE_GROUND,
			.secretPowerEffect = MOVE_EFFECT_SPD_MINUS_1,
			.secretPowerAnim = MOVE_MUDSHOT,
			.naturePowerMove = MOVE_MUDBOMB,
			.burmyForm = SPECIES_BURMY_SANDY,
		},

	[BATTLE_TERRAIN_BOG_WATER + 4] =
		{
			.camouflageType = TYPE_GROUND,
			.secretPowerEffect = MOVE_EFFECT_ACC_MINUS_1,
			.secretPowerAnim = MOVE_MUDSLAP,
			.naturePowerMove = MOVE_MUDDYWATER,
			.burmyForm = SPECIES_NONE,
		},

	[BATTLE_TERRAIN_CUBE_SPACE + 4] =
		{
			.camouflageType = TYPE_STEEL,
			.secretPowerEffect = MOVE_EFFECT_FREEZE,
			.secretPowerAnim = MOVE_GEARGRIND,
			.naturePowerMove = MOVE_SHIFTGEAR,
			.burmyForm = SPECIES_BURMY_TRASH,
		},
#endif
};

const struct BattleBackground gAttackTerrainTable[] =
{
    {
        .tileset = BG_Electric_TerrainTiles,
        .tilemap = BG_Electric_TerrainMap,
        .entryTileset = gBattleTerrainAnimTiles_Building,
        .entryTilemap = gBattleTerrainAnimTilemap_Building,
        .palette = BG_Electric_TerrainPal,
    },

    {
        .tileset = BG_Grassy_TerrainTiles,
        .tilemap = BG_Grassy_TerrainMap,
        .entryTileset = gBattleTerrainAnimTiles_Building,
        .entryTilemap = gBattleTerrainAnimTilemap_Building,
        .palette = BG_Grassy_TerrainPal,
    },

    {
        .tileset = BG_Misty_TerrainTiles,
        .tilemap = BG_Misty_TerrainMap,
        .entryTileset = gBattleTerrainAnimTiles_Building,
        .entryTilemap = gBattleTerrainAnimTilemap_Building,
        .palette = BG_Misty_TerrainPal,
    },

    {
        .tileset = BG_Psychic_TerrainTiles,
        .tilemap = BG_Psychic_TerrainMap,
        .entryTileset = gBattleTerrainAnimTiles_Building,
        .entryTilemap = gBattleTerrainAnimTilemap_Building,
        .palette = BG_Psychic_TerrainPal,
    },
};

const u16 gCamouflageColours[] =
{
	[TYPE_NORMAL] = 	RGB(31, 31, 31), //White
	[TYPE_FIGHTING] = 	RGB(29, 6, 0), //Red
	[TYPE_FLYING] = 	RGB(22, 27, 27), //Grayish Sky Blue
	[TYPE_POISON] = 	RGB(27, 0, 27), //Purple
	[TYPE_GROUND] = 	RGB(14, 9, 3), //Dark Bown
	[TYPE_ROCK] = 		RGB(23, 20, 7), //Light Brown
	[TYPE_BUG] = 		RGB(12, 24, 2), //Light Green
	[TYPE_GHOST] = 		RGB(20, 8, 20), //Darker Purple
	[TYPE_STEEL] = 		RGB(25, 25, 25), //Gray
	[TYPE_MYSTERY] = 	RGB(31, 31, 31), //White
	[TYPE_FIRE] = 		RGB(30, 16, 6), //Orangey Red
	[TYPE_GRASS] = 		RGB(0, 15, 2), //Dark Green
	[TYPE_WATER] = 		RGB(11, 22, 31), //Blue
	[TYPE_ELECTRIC] = 	RGB(31, 30, 11), //Yellow
	[TYPE_PSYCHIC] = 	RGB(26, 7, 17), //Purplish Pink
	[TYPE_ICE] = 		RGB(19, 27, 27), //Ice Blue
	[TYPE_DARK] = 		RGB(5, 5, 5), //Near Black
	[TYPE_DRAGON] = 	RGB(7, 4, 31), //Royal Blue
	[TYPE_FAIRY] = 		RGB(31, 20, 31), //Light Pink
};
