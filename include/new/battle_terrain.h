#pragma once

#include "../global.h"

/**
 * \file battle_terrain.h
 * \brief Contains functions relating to the battle background.
 */

//Exported Functions
bool8 MetatileBehavior_IsSand(u8 metatileBehavior);
bool8 MetatileBehavior_IsIce(u8 metatileBehavior);
bool8 MetatileBehavior_IsDeepOrOceanWater(u8 metatileBehavior);
bool8 IsTerrainMoveIndoors(void);
u8 GetBattleTerrainOverride(void);

//Hooked In Functions
u8 BattleSetup_GetTerrainId(void);
u8 LoadBattleBG_TerrainID(void);
void LoadBattleBG_Background(u8 terrainId);
void DrawBattleEntryBackground(void);

//Exported Structs
struct BattleBackground
{
    const void* tileset;
    const void* tilemap;
    const void* entryTileset;
    const void* entryTilemap;
    const void* palette;
};

struct TerrainTableStruct
{
	u8 camouflageType;
	u8 secretPowerEffect;
	u16 secretPowerAnim;
	u16 naturePowerMove;
	u16 burmyForm;
};

extern const struct TerrainTableStruct gTerrainTable[];
extern const u16 gCamouflageColours[];
