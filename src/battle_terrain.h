#pragma once
#include "defines.h"

#define gBattleTerrainAnimTiles_Building (void*) 0x824E410
#define gBattleTerrainAnimTilemap_Building (void*) 0x824E490

struct BattleBackground
{
    const void* tileset;
    const void* tilemap;
    const void* entryTileset;
    const void* entryTilemap;
    const void* palette;
};

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