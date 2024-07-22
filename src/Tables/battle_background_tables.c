#include "../../src/defines.h"
#include "../../src/defines_battle.h"
#include "../../include/new/battle_terrain.h"

#ifdef NEW_BATTLE_BACKGROUNDS
#define gBattleTerrainAnimTiles_TallGrass (void*) 0x8248c68
#define gBattleTerrainAnimTilemap_TallGrass (void*) 0x8248f58
#define gBattleTerrainAnimTiles_LongGrass (void*) 0x82498dc
#define gBattleTerrainAnimTilemap_LongGrass (void*) 0x8249e10
#define gBattleTerrainAnimTiles_Sand (void*) 0x824a618
#define gBattleTerrainAnimTilemap_Sand (void*) 0x824a844
#define gBattleTerrainAnimTiles_Underwater (void*) 0x824af70
#define gBattleTerrainAnimTilemap_Underwater (void*) 0x824b0dc
#define gBattleTerrainAnimTiles_Water (void*) 0x824b8a8
#define gBattleTerrainAnimTilemap_Water (void*) 0x824bbe0
#define gBattleTerrainAnimTiles_PondWater (void*) 0x824c314
#define gBattleTerrainAnimTilemap_PondWater (void*) 0x824c520
#define gBattleTerrainAnimTiles_Rock (void*) 0x824cbf8
#define gBattleTerrainAnimTilemap_Rock (void*) 0x824cec8
#define gBattleTerrainAnimTiles_Cave (void*) 0x824d6b8
#define gBattleTerrainAnimTilemap_Cave (void*) 0x824dc98
#define gBattleTerrainAnimTiles_Building (void*) 0x824e410
#define gBattleTerrainAnimTilemap_Building (void*) 0x824e490

// Note: entry tileset & map will use the palette of the background. It has 48 entries, 16*2 for the background and 16*1 for the entry 
extern const u8 BG_GrassTiles[];
extern const u8 BG_GrassMap[];
extern const u8 BG_GrassPal[];
extern const u8 BG_SnowTiles[];
extern const u8 BG_SnowMap[];
extern const u8 BG_SnowPal[];

const struct BattleBackground gBattleTerrainTable[] =
{
	[BATTLE_TERRAIN_GRASS] =
	{
		.tileset = BG_GrassTiles,
		.tilemap = BG_GrassMap,
		.entryTileset = gBattleTerrainAnimTiles_TallGrass,
		.entryTilemap = gBattleTerrainAnimTilemap_TallGrass,
		.palette = BG_GrassPal,
	},
	    [BATTLE_TERRAIN_SNOWY] =
	{
		.tileset = BG_SnowTiles,
		.tilemap = BG_SnowMap,
		.entryTileset = gBattleTerrainAnimTiles_TallGrass,
		.entryTilemap = gBattleTerrainAnimTilemap_TallGrass,
		.palette = BG_SnowPal,
	},
};
#endif