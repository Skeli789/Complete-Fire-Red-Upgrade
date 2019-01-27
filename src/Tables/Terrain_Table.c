#include "..\\defines.h"
#include "..\\battle_terrain.h"

const struct TerrainTableStruct TerrainTable[] =
{
	{
		.terrainNum = -1,
		.camouflageType = TYPE_ELECTRIC,
		.secretPowerEffect = MOVE_EFFECT_PARALYSIS,
		.secretPowerAnim = MOVE_THUNDERSHOCK,
		.naturePowerMove = MOVE_THUNDERBOLT,
	},

	{
		.terrainNum = -2,
		.camouflageType = TYPE_GRASS,
		.secretPowerEffect = MOVE_EFFECT_SLEEP,
		.secretPowerAnim = MOVE_VINEWHIP,
		.naturePowerMove = MOVE_ENERGYBALL,
	},
	
	{
		.terrainNum = -3,
		.camouflageType = TYPE_FAIRY,
		.secretPowerEffect = MOVE_EFFECT_SP_ATK_MINUS_1,
		.secretPowerAnim = MOVE_FAIRYWIND,
		.naturePowerMove = MOVE_MOONBLAST,
	},
	
	{
		.terrainNum = -4,
		.camouflageType = TYPE_PSYCHIC,
		.secretPowerEffect = MOVE_EFFECT_SPD_MINUS_1,
		.secretPowerAnim = MOVE_CONFUSION,
		.naturePowerMove = MOVE_PSYCHIC,
	},
	
	{
		.terrainNum = BATTLE_TERRAIN_GRASS,
		.camouflageType = TYPE_GRASS,
		.secretPowerEffect = MOVE_EFFECT_SLEEP,
		.secretPowerAnim = MOVE_VINEWHIP,
		.naturePowerMove = MOVE_ENERGYBALL,
	},
	
	{
		.terrainNum = BATTLE_TERRAIN_LONG_GRASS,
		.camouflageType = TYPE_GRASS,
		.secretPowerEffect = MOVE_EFFECT_SLEEP,
		.secretPowerAnim = MOVE_VINEWHIP,
		.naturePowerMove = MOVE_ENERGYBALL,
	},
	
	{
		.terrainNum = BATTLE_TERRAIN_SAND,
		.camouflageType = TYPE_GROUND,
		.secretPowerEffect = MOVE_EFFECT_ACC_MINUS_1,
		.secretPowerAnim = MOVE_MUDSLAP,
		.naturePowerMove = MOVE_EARTHPOWER,
	},
	
	{
		.terrainNum = BATTLE_TERRAIN_UNDERWATER,
		.camouflageType = TYPE_WATER,
		.secretPowerEffect = MOVE_EFFECT_ATK_MINUS_1,
		.secretPowerAnim = MOVE_WATERPULSE,
		.naturePowerMove = MOVE_HYDROPUMP,
	},
	
	{
		.terrainNum = BATTLE_TERRAIN_WATER,
		.camouflageType = TYPE_WATER,
		.secretPowerEffect = MOVE_EFFECT_ATK_MINUS_1,
		.secretPowerAnim = MOVE_WATERPULSE,
		.naturePowerMove = MOVE_HYDROPUMP,
	},
	
	{
		.terrainNum = BATTLE_TERRAIN_POND,
		.camouflageType = TYPE_WATER,
		.secretPowerEffect = MOVE_EFFECT_ATK_MINUS_1,
		.secretPowerAnim = MOVE_WATERPULSE,
		.naturePowerMove = MOVE_HYDROPUMP,
	},
	
	{
		.terrainNum = BATTLE_TERRAIN_MOUNTAIN,
		.camouflageType = TYPE_GROUND,
		.secretPowerEffect = MOVE_EFFECT_ACC_MINUS_1,
		.secretPowerAnim = MOVE_MUDSLAP,
		.naturePowerMove = MOVE_EARTHPOWER,
	},
	
	{
		.terrainNum = BATTLE_TERRAIN_CAVE,
		.camouflageType = TYPE_ROCK,
		.secretPowerEffect = MOVE_EFFECT_FLINCH,
		.secretPowerAnim = MOVE_ROCKTHROW,
		.naturePowerMove = MOVE_POWERGEM,
	},
	
	{
		.terrainNum = BATTLE_TERRAIN_INSIDE,
		.camouflageType = TYPE_NORMAL,
		.secretPowerEffect = MOVE_EFFECT_PARALYSIS,
		.secretPowerAnim = MOVE_BODYSLAM,
		.naturePowerMove = MOVE_TRIATTACK,
	},
	
	{
		.terrainNum = BATTLE_TERRAIN_PLAIN,
		.camouflageType = TYPE_NORMAL,
		.secretPowerEffect = MOVE_EFFECT_PARALYSIS,
		.secretPowerAnim = MOVE_SLAM,
		.naturePowerMove = MOVE_TRIATTACK,
	},
	
	{
		.terrainNum = BATTLE_TERRAIN_INSIDE_2,
		.camouflageType = TYPE_NORMAL,
		.secretPowerEffect = MOVE_EFFECT_PARALYSIS,
		.secretPowerAnim = MOVE_BODYSLAM,
		.naturePowerMove = MOVE_TRIATTACK,
	},
	
	{
		.terrainNum = BATTLE_TERRAIN_INSIDE_3,
		.camouflageType = TYPE_NORMAL,
		.secretPowerEffect = MOVE_EFFECT_PARALYSIS,
		.secretPowerAnim = MOVE_BODYSLAM,
		.naturePowerMove = MOVE_TRIATTACK,
	},
	
	{
		.terrainNum = BATTLE_TERRAIN_INSIDE_4,
		.camouflageType = TYPE_NORMAL,
		.secretPowerEffect = MOVE_EFFECT_PARALYSIS,
		.secretPowerAnim = MOVE_BODYSLAM,
		.naturePowerMove = MOVE_TRIATTACK,
	},
	
	{
		.terrainNum = BATTLE_TERRAIN_INSIDE_5,
		.camouflageType = TYPE_NORMAL,
		.secretPowerEffect = MOVE_EFFECT_PARALYSIS,
		.secretPowerAnim = MOVE_BODYSLAM,
		.naturePowerMove = MOVE_TRIATTACK,
	},
	
	{
		.terrainNum = BATTLE_TERRAIN_INSIDE_6,
		.camouflageType = TYPE_NORMAL,
		.secretPowerEffect = MOVE_EFFECT_PARALYSIS,
		.secretPowerAnim = MOVE_BODYSLAM,
		.naturePowerMove = MOVE_TRIATTACK,
	},
	
	{
		.terrainNum = BATTLE_TERRAIN_LORLEI,
		.camouflageType = TYPE_ICE,
		.secretPowerEffect = MOVE_EFFECT_FREEZE,
		.secretPowerAnim = MOVE_ICESHARD,
		.naturePowerMove = MOVE_ICEBEAM,
	},
	
	{
		.terrainNum = BATTLE_TERRAIN_BRUNO,
		.camouflageType = TYPE_FIGHTING,
		.secretPowerEffect = MOVE_EFFECT_FLINCH,
		.secretPowerAnim = MOVE_KARATECHOP,
		.naturePowerMove = MOVE_FOCUSBLAST,
	},
	
	{
		.terrainNum = BATTLE_TERRAIN_AGATHA,
		.camouflageType = TYPE_GHOST,
		.secretPowerEffect = MOVE_EFFECT_EVS_MINUS_1,
		.secretPowerAnim = MOVE_SHADOWSNEAK,
		.naturePowerMove = MOVE_SHADOWBALL,
	},
	
	{
		.terrainNum = BATTLE_TERRAIN_LANCE,
		.camouflageType = TYPE_DRAGON,
		.secretPowerEffect = MOVE_EFFECT_ATK_MINUS_1,
		.secretPowerAnim = MOVE_DRAGONRAGE,
		.naturePowerMove = MOVE_DRAGONPULSE,
	},
	
	{
		.terrainNum = BATTLE_TERRAIN_CHAMPION,
		.camouflageType = TYPE_NORMAL,
		.secretPowerEffect = MOVE_EFFECT_PARALYSIS,
		.secretPowerAnim = MOVE_BODYSLAM,
		.naturePowerMove = MOVE_TRIATTACK,
	}
	
	#ifdef UNBOUND
	,
	{
		.terrainNum = BATTLE_TERRAIN_SNOW_FIELD,
		.camouflageType = TYPE_ICE,
		.secretPowerEffect = MOVE_EFFECT_FREEZE,
		.secretPowerAnim = MOVE_AVALANCHE,
		.naturePowerMove = MOVE_FROSTBREATH,
	},

	{
		.terrainNum = BATTLE_TERRAIN_VOLCANO,
		.camouflageType = TYPE_FIRE,
		.secretPowerEffect = MOVE_EFFECT_BURN,
		.secretPowerAnim = MOVE_INCINERATE,
		.naturePowerMove = MOVE_LAVAPLUME,
	},
	
	{
		.terrainNum = BATTLE_TERRAIN_DARK_CAVE_WATER,
		.camouflageType = TYPE_WATER,
		.secretPowerEffect = MOVE_EFFECT_ATK_MINUS_1,
		.secretPowerAnim = MOVE_WATERPULSE,
		.naturePowerMove = MOVE_HYDROPUMP,
	},
	
	{
		.terrainNum = BATTLE_TERRAIN_DARK_CAVE,
		.camouflageType = TYPE_ROCK,
		.secretPowerEffect = MOVE_EFFECT_FLINCH,
		.secretPowerAnim = MOVE_ROCKTHROW,
		.naturePowerMove = MOVE_POWERGEM,
	},
	
	{
		.terrainNum = BATTLE_TERRAIN_SNOW_CAVE,
		.camouflageType = TYPE_ICE,
		.secretPowerEffect = MOVE_EFFECT_FREEZE,
		.secretPowerAnim = MOVE_AVALANCHE,
		.naturePowerMove = MOVE_FROSTBREATH,
	},
	
	{
		.terrainNum = BATTLE_TERRAIN_FOREST,
		.camouflageType = TYPE_BUG,
		.secretPowerEffect = MOVE_EFFECT_POISON,
		.secretPowerAnim = MOVE_TWINEEDLE,
		.naturePowerMove = MOVE_BUGBUZZ,
	},
	
	{
		.terrainNum = BATTLE_TERRAIN_ICE_IN_CAVE,
		.camouflageType = TYPE_ICE,
		.secretPowerEffect = MOVE_EFFECT_FREEZE,
		.secretPowerAnim = MOVE_ICESHARD,
		.naturePowerMove = MOVE_ICEBEAM,
	},
	
	{
		.terrainNum = BATTLE_TERRAIN_RUINS_OF_VOID,
		.camouflageType = TYPE_ROCK,
		.secretPowerEffect = MOVE_EFFECT_FLINCH,
		.secretPowerAnim = MOVE_ROCKTHROW,
		.naturePowerMove = MOVE_POWERGEM,
	},
	
	{
		.terrainNum = BATTLE_TERRAIN_DISTORTION_WORLD,
		.camouflageType = TYPE_GHOST,
		.secretPowerEffect = MOVE_EFFECT_DEF_MINUS_1,
		.secretPowerAnim = MOVE_SHADOWSNEAK,
		.naturePowerMove = MOVE_SHADOWBALL,
	},
	
	{
		.terrainNum = BATTLE_TERRAIN_GRASS_GYM,
		.camouflageType = TYPE_GRASS,
		.secretPowerEffect = MOVE_EFFECT_SLEEP,
		.secretPowerAnim = MOVE_VINEWHIP,
		.naturePowerMove = MOVE_ENERGYBALL,
	},
	
	{
		.terrainNum = BATTLE_TERRAIN_DESERT,
		.camouflageType = TYPE_GROUND,
		.secretPowerEffect = MOVE_EFFECT_ACC_MINUS_1,
		.secretPowerAnim = MOVE_MUDSLAP,
		.naturePowerMove = MOVE_EARTHPOWER,
	},
	
	{
		.terrainNum = BATTLE_TERRAIN_WATER_IN_CAVE,
		.camouflageType = TYPE_WATER,
		.secretPowerEffect = MOVE_EFFECT_ATK_MINUS_1,
		.secretPowerAnim = MOVE_WATERPULSE,
		.naturePowerMove = MOVE_HYDROPUMP,
	},
	
	{
		.terrainNum = BATTLE_TERRAIN_WATER_IN_SNOW_CAVE,
		.camouflageType = TYPE_WATER,
		.secretPowerEffect = MOVE_EFFECT_ATK_MINUS_1,
		.secretPowerAnim = MOVE_WATERPULSE,
		.naturePowerMove = MOVE_HYDROPUMP,
	},
	
	{
		.terrainNum = BATTLE_TERRAIN_LAVA_IN_VOLCANO,
		.camouflageType = TYPE_FIRE,
		.secretPowerEffect = MOVE_EFFECT_BURN,
		.secretPowerAnim = MOVE_INCINERATE,
		.naturePowerMove = MOVE_LAVAPLUME,
	},
	
	{
		.terrainNum = BATTLE_TERRAIN_WATER_IN_FOREST,
		.camouflageType = TYPE_WATER,
		.secretPowerEffect = MOVE_EFFECT_ATK_MINUS_1,
		.secretPowerAnim = MOVE_WATERPULSE,
		.naturePowerMove = MOVE_HYDROPUMP,
	},
	
	{
		.terrainNum = BATTLE_TERRAIN_AUTUMN_GRASS,
		.camouflageType = TYPE_GRASS,
		.secretPowerEffect = MOVE_EFFECT_SLEEP,
		.secretPowerAnim = MOVE_VINEWHIP,
		.naturePowerMove = MOVE_ENERGYBALL,
	},
	
	{
		.terrainNum = BATTLE_TERRAIN_AUTUMN_FIELD,
		.camouflageType = TYPE_NORMAL,
		.secretPowerEffect = MOVE_EFFECT_PARALYSIS,
		.secretPowerAnim = MOVE_BODYSLAM,
		.naturePowerMove = MOVE_TRIATTACK,
	},
	
	{
		.terrainNum = BATTLE_TERRAIN_SNOW_GRASS,
		.camouflageType = TYPE_ICE,
		.secretPowerEffect = MOVE_EFFECT_FREEZE,
		.secretPowerAnim = MOVE_AVALANCHE,
		.naturePowerMove = MOVE_FROSTBREATH,
	}
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
    }
};