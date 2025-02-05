#define MAX_OBJECT_HEIGHT 4
#define MAX_OBJECT_WIDTH 4

#include "../global.h"
#include "../../include/battle_anim.h"

extern const struct OamData gOamData_AffineOff_ObjNormal_32x32;
extern const struct OamData gOamData_AffineOff_ObjNormal_64x64;
extern const struct OamData gOamData_AffineOff_ObjNormal_64x32;
extern const struct OamData gOamData_AffineOff_ObjNormal_32x64;

void __attribute__((long_call)) Free(void *pointer);
void TransferPlttBuffer(void);

enum Prizes
{
	PRIZE_HEART_SCALE,
	PRIZE_RED_SHARD,
	PRIZE_YELLOW_SHARD,
	PRIZE_GREEN_SHARD,
	PRIZE_BLUE_SHARD,
	PRIZE_HARD_STONE,
	PRIZE_FIRE_STONE,
	PRIZE_WATER_STONE,
	PRIZE_THUNDER_STONE,
	PRIZE_LEAF_STONE,
	PRIZE_MOON_STONE,
	PRIZE_SUN_STONE,
	PRIZE_SHINY_STONE,
	PRIZE_DAWN_STONE,
	PRIZE_DUSK_STONE,
	PRIZE_EVERSTONE,
	PRIZE_REVIVE,
	PRIZE_MAX_REVIVE,
	PRIZE_STAR_PIECE,
        PRIZE_OLD_AMBER,
	PRIZE_DOME_FOSSIL,
	PRIZE_HELIX_FOSSIL,
	PRIZE_CLAW_FOSSIL,
	PRIZE_ROOT_FOSSIL,
	PRIZE_SKULL_FOSSIL,
	PRIZE_ARMOR_FOSSIL,
	PRIZE_PLUME_FOSSIL,
	PRIZE_COVER_FOSSIL,
	PRIZE_JAW_FOSSIL,
	PRIZE_SAIL_FOSSIL,
	PRIZE_COUNT
};

#define PRIZE_FIRST_SHARD PRIZE_RED_SHARD
#define PRIZE_LAST_SHARD PRIZE_BLUE_SHARD
#define PRIZE_FIRST_EVO_STONE PRIZE_FIRE_STONE
#define PRIZE_LAST_EVO_STONE PRIZE_DUSK_STONE
#define PRIZE_FIRST_FOSSIL PRIZE_OLD_AMBER
#define PRIZE_LAST_FOSSIL PRIZE_SAIL_FOSSIL

typedef u8 PrizeShape_T[MAX_OBJECT_WIDTH];

struct MiningPrize
{
	u16 itemId;
	const u8* spriteTiles;
	const u16* spritePal;
	const PrizeShape_T* shape;
	const struct OamData* oam;
	u16 spriteSize;
};

extern const u8 MiningHeartScaleTiles[];
extern const u16 MiningHeartScalePal[];
extern const u8 MiningRedShardTiles[];
extern const u16 MiningRedShardPal[];
extern const u8 MiningYellowShardTiles[];
extern const u16 MiningYellowShardPal[];
extern const u8 MiningGreenShardTiles[];
extern const u16 MiningGreenShardPal[];
extern const u8 MiningBlueShardTiles[];
extern const u16 MiningBlueShardPal[];
extern const u8 MiningHardStoneTiles[];
extern const u16 MiningHardStonePal[];
extern const u8 MiningFireStoneTiles[];
extern const u16 MiningFireStonePal[];
extern const u8 MiningWaterStoneTiles[];
extern const u16 MiningWaterStonePal[];
extern const u8 MiningThunderStoneTiles[];
extern const u16 MiningThunderStonePal[];
extern const u8 MiningLeafStoneTiles[];
extern const u16 MiningLeafStonePal[];
extern const u8 MiningMoonStoneTiles[];
extern const u16 MiningMoonStonePal[];
extern const u8 MiningSunStoneTiles[];
extern const u16 MiningSunStonePal[];
extern const u8 MiningShinyStoneTiles[];
extern const u16 MiningShinyStonePal[];
extern const u8 MiningDawnStoneTiles[];
extern const u16 MiningDawnStonePal[];
extern const u8 MiningDuskStoneTiles[];
extern const u16 MiningDuskStonePal[];
extern const u8 MiningEverstoneTiles[];
extern const u16 MiningEverstonePal[];
extern const u8 MiningReviveTiles[];
extern const u16 MiningRevivePal[];
extern const u8 MiningMaxReviveTiles[];
extern const u16 MiningMaxRevivePal[];
extern const u8 MiningStarPieceTiles[];
extern const u16 MiningStarPiecePal[];
extern const u8 MiningOldAmberTiles[];
extern const u16 MiningOldAmberPal[];
extern const u8 MiningHelixFossilTiles[];
extern const u16 MiningHelixFossilPal[];
extern const u8 MiningDomeFossilTiles[];
extern const u8 MiningClawFossilTiles[];
extern const u8 MiningRootFossilTiles[];
extern const u8 MiningSkullFossilTiles[];
extern const u8 MiningArmorFossilTiles[];
extern const u8 MiningPlumeFossilTiles[];
extern const u16 MiningPlumeFossilPal[];
extern const u8 MiningCoverFossilTiles[];
extern const u8 MiningJawFossilTiles[];
extern const u16 MiningJawFossilPal[];
extern const u8 MiningSailFossilTiles[];

extern const u8 MiningBedrock0Tiles[];
extern const u16 MiningBedrock0Pal[];
extern const u8 MiningBedrock1Tiles[];
extern const u8 MiningBedrock2Tiles[];
extern const u8 MiningBedrock3Tiles[];
extern const u8 MiningBedrock4Tiles[];
extern const u8 MiningBedrock5Tiles[];
extern const u8 MiningBedrock6Tiles[];
extern const u8 MiningBedrock7Tiles[];
extern const u8 MiningBedrock8Tiles[];
extern const u8 MiningBedrock9Tiles[];

static const u8 sHeartScaleShape[MAX_OBJECT_HEIGHT][MAX_OBJECT_WIDTH] =
{
	{1, 1},
	{1, 1},
};

static const u8 sRedShardShape[MAX_OBJECT_HEIGHT][MAX_OBJECT_WIDTH] =
{
	{1, 1, 1},
	{1, 1, 0},
	{1, 1, 1},
};

static const u8 sYellowShardShape[MAX_OBJECT_HEIGHT][MAX_OBJECT_WIDTH] =
{
	{1, 0, 1, 0},
	{1, 1, 1, 0},
	{1, 1, 1, 1},
};

static const u8 sGreenShardShape[MAX_OBJECT_HEIGHT][MAX_OBJECT_WIDTH] =
{
	{1, 1, 1, 1},
	{1, 1, 1, 1},
	{1, 1, 0, 1},
};

static const u8 sBlueShardShape[MAX_OBJECT_HEIGHT][MAX_OBJECT_WIDTH] =
{
	{1, 1, 1},
	{1, 1, 1},
	{1, 1, 0},
};

static const u8 sHardStoneShape[MAX_OBJECT_HEIGHT][MAX_OBJECT_WIDTH] =
{
	{1, 1},
	{1, 1},
};

static const u8 sFireStoneShape[MAX_OBJECT_HEIGHT][MAX_OBJECT_WIDTH] =
{
	{1, 1, 1},
	{1, 1, 1},
	{1, 1, 1},
};

static const u8 sWaterStoneShape[MAX_OBJECT_HEIGHT][MAX_OBJECT_WIDTH] =
{
	{1, 1, 1},
	{1, 1, 1},
	{1, 1, 0},
};

static const u8 sThunderStoneShape[MAX_OBJECT_HEIGHT][MAX_OBJECT_WIDTH] =
{
	{0, 1, 1},
	{1, 1, 1},
	{1, 1, 0},
};

static const u8 sLeafStoneShape[MAX_OBJECT_HEIGHT][MAX_OBJECT_WIDTH] =
{
	{0, 1, 0},
	{1, 1, 1},
	{1, 1, 1},
	{0, 1, 0},
};

static const u8 sMoonStoneShape[MAX_OBJECT_HEIGHT][MAX_OBJECT_WIDTH] =
{
	{1, 0},
	{1, 1},
	{1, 1},
	{0, 1},
};

static const u8 sSunStoneShape[MAX_OBJECT_HEIGHT][MAX_OBJECT_WIDTH] =
{
	{0, 1, 0},
	{1, 1, 1},
	{1, 1, 1},
};

static const u8 sShinyStoneShape[MAX_OBJECT_HEIGHT][MAX_OBJECT_WIDTH] =
{
	{1, 1, 1},
	{1, 1, 1},
	{1, 1, 1},
};

static const u8 sDawnStoneShape[MAX_OBJECT_HEIGHT][MAX_OBJECT_WIDTH] =
{
	{1, 1, 1},
	{1, 1, 1},
	{1, 1, 1},
};

static const u8 sDuskStoneShape[MAX_OBJECT_HEIGHT][MAX_OBJECT_WIDTH] =
{
	{1, 1, 1},
	{1, 1, 1},
	{1, 1, 1},
};

static const u8 sEverstoneShape[MAX_OBJECT_HEIGHT][MAX_OBJECT_WIDTH] =
{
	{1, 1, 1, 1},
	{1, 1, 1, 1},
};

static const u8 sReviveShape[MAX_OBJECT_HEIGHT][MAX_OBJECT_WIDTH] =
{
	{0, 1, 0},
	{1, 1, 1},
	{0, 1, 0},
};

static const u8 sMaxReviveShape[MAX_OBJECT_HEIGHT][MAX_OBJECT_WIDTH] =
{
	{1, 1, 1},
	{1, 1, 1},
	{1, 1, 1},
};

static const u8 sStarPieceShape[MAX_OBJECT_HEIGHT][MAX_OBJECT_WIDTH] =
{
	{0, 1, 0},
	{1, 1, 1},
	{0, 1, 0},
};

static const u8 sOldAmberShape[MAX_OBJECT_HEIGHT][MAX_OBJECT_WIDTH] =
{
	{0, 1, 1, 1},
	{1, 1, 1, 1},
	{1, 1, 1, 1},
	{1, 1, 1, 0},
};

static const u8 sDomeFossilShape[MAX_OBJECT_HEIGHT][MAX_OBJECT_WIDTH] =
{
	{1, 1, 1, 1},
	{1, 1, 1, 1},
	{1, 1, 1, 1},
	{0, 1, 1, 1},
};

static const u8 sHelixFossilShape[MAX_OBJECT_HEIGHT][MAX_OBJECT_WIDTH] =
{
	{0, 1, 1, 1},
	{1, 1, 1, 1},
	{1, 1, 1, 1},
	{1, 1, 1, 0},
};

static const u8 sClawFossilShape[MAX_OBJECT_HEIGHT][MAX_OBJECT_WIDTH] =
{
	{0, 1, 1, 1},
	{0, 1, 1, 1},
	{1, 1, 1, 1},
	{1, 1, 1, 0},
};

static const u8 sRootFossilShape[MAX_OBJECT_HEIGHT][MAX_OBJECT_WIDTH] =
{
	{0, 1, 1, 0},
	{1, 1, 1, 0},
	{1, 1, 1, 1},
	{1, 1, 1, 1},
};

static const u8 sSkullFossilShape[MAX_OBJECT_HEIGHT][MAX_OBJECT_WIDTH] =
{
	{1, 1, 1, 1},
	{1, 1, 1, 1},
	{1, 1, 1, 1},
	{0, 1, 1, 0},
};

static const u8 sArmorFossilShape[MAX_OBJECT_HEIGHT][MAX_OBJECT_WIDTH] =
{
	{1, 1, 1, 1},
	{1, 1, 1, 1},
	{1, 1, 1, 1},
	{1, 1, 1, 1},
};

static const u8 sPlumeFossilShape[MAX_OBJECT_HEIGHT][MAX_OBJECT_WIDTH] =
{
	{0, 1, 1, 1},
	{1, 1, 1, 1},
	{1, 1, 1, 1},
	{1, 1, 1, 1},
};

static const u8 sCoverFossilShape[MAX_OBJECT_HEIGHT][MAX_OBJECT_WIDTH] =
{
	{0, 1, 1, 1},
	{1, 1, 1, 1},
	{1, 1, 1, 1},
	{1, 1, 1, 1},
};

static const u8 sJawFossilShape[MAX_OBJECT_HEIGHT][MAX_OBJECT_WIDTH] =
{
	{1, 1, 1, 1},
	{1, 1, 1, 1},
	{1, 1, 1, 1},
	{1, 1, 1, 1},
};

static const u8 sSailFossilShape[MAX_OBJECT_HEIGHT][MAX_OBJECT_WIDTH] =
{
	{0, 1, 1, 1},
	{1, 1, 1, 1},
	{1, 1, 1, 1},
	{1, 1, 1, 1},
};

static const struct MiningPrize sMiningPrizes[PRIZE_COUNT] =
{
	[PRIZE_HEART_SCALE] = {ITEM_HEART_SCALE, MiningHeartScaleTiles, MiningHeartScalePal, sHeartScaleShape, &gOamData_AffineOff_ObjNormal_32x32, (32 * 32) / 2},
	[PRIZE_RED_SHARD] = {ITEM_RED_SHARD, MiningRedShardTiles, MiningRedShardPal, sRedShardShape, &gOamData_AffineOff_ObjNormal_64x64, (64 * 64) / 2},
	[PRIZE_YELLOW_SHARD] = {ITEM_YELLOW_SHARD, MiningYellowShardTiles, MiningYellowShardPal, sYellowShardShape, &gOamData_AffineOff_ObjNormal_64x64, (64 * 64) / 2},
	[PRIZE_GREEN_SHARD] = {ITEM_GREEN_SHARD, MiningGreenShardTiles, MiningGreenShardPal, sGreenShardShape, &gOamData_AffineOff_ObjNormal_64x64, (64 * 64) / 2},
	[PRIZE_BLUE_SHARD] = {ITEM_BLUE_SHARD, MiningBlueShardTiles, MiningBlueShardPal, sBlueShardShape, &gOamData_AffineOff_ObjNormal_64x64, (64 * 64) / 2},
	[PRIZE_HARD_STONE] = {ITEM_HARD_STONE, MiningHardStoneTiles, MiningHardStonePal, sHardStoneShape, &gOamData_AffineOff_ObjNormal_32x32, (32 * 32) / 2},
	[PRIZE_FIRE_STONE] = {ITEM_FIRE_STONE, MiningFireStoneTiles, MiningFireStonePal, sFireStoneShape, &gOamData_AffineOff_ObjNormal_64x64, (64 * 64) / 2},
	[PRIZE_WATER_STONE] = {ITEM_WATER_STONE, MiningWaterStoneTiles, MiningWaterStonePal, sWaterStoneShape, &gOamData_AffineOff_ObjNormal_64x64, (64 * 64) / 2},
	[PRIZE_THUNDER_STONE] = {ITEM_THUNDER_STONE, MiningThunderStoneTiles, MiningThunderStonePal, sThunderStoneShape, &gOamData_AffineOff_ObjNormal_64x64, (64 * 64) / 2},
	[PRIZE_LEAF_STONE] = {ITEM_LEAF_STONE, MiningLeafStoneTiles, MiningLeafStonePal, sLeafStoneShape, &gOamData_AffineOff_ObjNormal_64x64, (64 * 64) / 2},
	[PRIZE_MOON_STONE] = {ITEM_MOON_STONE, MiningMoonStoneTiles, MiningMoonStonePal, sMoonStoneShape, &gOamData_AffineOff_ObjNormal_64x64, (64 * 64) / 2},
	[PRIZE_SUN_STONE] = {ITEM_SUN_STONE, MiningSunStoneTiles, MiningSunStonePal, sSunStoneShape, &gOamData_AffineOff_ObjNormal_64x64, (64 * 64) / 2},
	[PRIZE_SHINY_STONE] = {ITEM_SHINY_STONE, MiningShinyStoneTiles, MiningShinyStonePal, sShinyStoneShape, &gOamData_AffineOff_ObjNormal_64x64, (64 * 64) / 2},
	[PRIZE_DAWN_STONE] = {ITEM_DAWN_STONE, MiningDawnStoneTiles, MiningDawnStonePal, sDawnStoneShape, &gOamData_AffineOff_ObjNormal_64x64, (64 * 64) / 2},
	[PRIZE_DUSK_STONE] = {ITEM_DUSK_STONE, MiningDuskStoneTiles, MiningDuskStonePal, sDuskStoneShape, &gOamData_AffineOff_ObjNormal_64x64, (64 * 64) / 2},
	[PRIZE_EVERSTONE] = {ITEM_EVERSTONE, MiningEverstoneTiles, MiningEverstonePal, sEverstoneShape, &gOamData_AffineOff_ObjNormal_64x32, (64 * 32) / 2},
	[PRIZE_REVIVE] = {ITEM_REVIVE, MiningReviveTiles, MiningRevivePal, sReviveShape, &gOamData_AffineOff_ObjNormal_64x64, (64 * 64) / 2},
	[PRIZE_MAX_REVIVE] = {ITEM_MAX_REVIVE, MiningMaxReviveTiles, MiningMaxRevivePal, sMaxReviveShape, &gOamData_AffineOff_ObjNormal_64x64, (64 * 64) / 2},
	[PRIZE_STAR_PIECE] = {ITEM_STAR_PIECE, MiningStarPieceTiles, MiningStarPiecePal, sStarPieceShape, &gOamData_AffineOff_ObjNormal_64x64, (64 * 64) / 2},
        [PRIZE_OLD_AMBER] = {ITEM_OLD_AMBER, MiningOldAmberTiles, MiningOldAmberPal, sOldAmberShape, &gOamData_AffineOff_ObjNormal_64x64, (64 * 64) / 2},
	[PRIZE_DOME_FOSSIL] = {ITEM_DOME_FOSSIL, MiningDomeFossilTiles, MiningHelixFossilPal, sDomeFossilShape, &gOamData_AffineOff_ObjNormal_64x64, (64 * 64) / 2},
	[PRIZE_HELIX_FOSSIL] = {ITEM_HELIX_FOSSIL, MiningHelixFossilTiles, MiningHelixFossilPal, sHelixFossilShape, &gOamData_AffineOff_ObjNormal_64x64, (64 * 64) / 2},
	[PRIZE_CLAW_FOSSIL] = {ITEM_CLAW_FOSSIL, MiningClawFossilTiles, MiningHelixFossilPal, sClawFossilShape, &gOamData_AffineOff_ObjNormal_64x64, (64 * 64) / 2},
	[PRIZE_ROOT_FOSSIL] = {ITEM_ROOT_FOSSIL, MiningRootFossilTiles, MiningHelixFossilPal, sRootFossilShape, &gOamData_AffineOff_ObjNormal_64x64, (64 * 64) / 2},
	[PRIZE_SKULL_FOSSIL] = {ITEM_SKULL_FOSSIL, MiningSkullFossilTiles, MiningHelixFossilPal, sSkullFossilShape, &gOamData_AffineOff_ObjNormal_64x64, (64 * 64) / 2},
	[PRIZE_ARMOR_FOSSIL] = {ITEM_ARMOR_FOSSIL, MiningArmorFossilTiles, MiningHelixFossilPal, sArmorFossilShape, &gOamData_AffineOff_ObjNormal_64x64, (64 * 64) / 2},
	[PRIZE_PLUME_FOSSIL] = {ITEM_PLUME_FOSSIL, MiningPlumeFossilTiles, MiningPlumeFossilPal, sPlumeFossilShape, &gOamData_AffineOff_ObjNormal_64x64, (64 * 64) / 2},
	[PRIZE_COVER_FOSSIL] = {ITEM_COVER_FOSSIL, MiningCoverFossilTiles,MiningPlumeFossilPal, sCoverFossilShape, &gOamData_AffineOff_ObjNormal_64x64, (64 * 64) / 2},
	[PRIZE_JAW_FOSSIL] = {ITEM_JAW_FOSSIL, MiningJawFossilTiles, MiningJawFossilPal, sJawFossilShape, &gOamData_AffineOff_ObjNormal_64x64, (64 * 64) / 2},
	[PRIZE_SAIL_FOSSIL] = {ITEM_SAIL_FOSSIL, MiningSailFossilTiles, MiningJawFossilPal, sSailFossilShape, &gOamData_AffineOff_ObjNormal_64x64, (64 * 64) / 2},
};

static const u8 sPrizeEncounterChart[100] =
{
	/*20 %*/ PRIZE_HARD_STONE, PRIZE_HARD_STONE, PRIZE_HARD_STONE, PRIZE_HARD_STONE, PRIZE_HARD_STONE, PRIZE_HARD_STONE, PRIZE_HARD_STONE, PRIZE_HARD_STONE, PRIZE_HARD_STONE, PRIZE_HARD_STONE,
	/*"" %*/ PRIZE_HARD_STONE, PRIZE_HARD_STONE, PRIZE_HARD_STONE, PRIZE_HARD_STONE, PRIZE_HARD_STONE, PRIZE_HARD_STONE, PRIZE_HARD_STONE, PRIZE_HARD_STONE, PRIZE_HARD_STONE, PRIZE_HARD_STONE,
	/*20 %*/ PRIZE_EVERSTONE, PRIZE_EVERSTONE, PRIZE_EVERSTONE, PRIZE_EVERSTONE, PRIZE_EVERSTONE, PRIZE_EVERSTONE, PRIZE_EVERSTONE, PRIZE_EVERSTONE, PRIZE_EVERSTONE, PRIZE_EVERSTONE,
	/*"" %*/ PRIZE_EVERSTONE, PRIZE_EVERSTONE, PRIZE_EVERSTONE, PRIZE_EVERSTONE, PRIZE_EVERSTONE, PRIZE_EVERSTONE, PRIZE_EVERSTONE, PRIZE_EVERSTONE, PRIZE_EVERSTONE, PRIZE_EVERSTONE,
	/*10 %*/ PRIZE_FIRST_SHARD, PRIZE_FIRST_SHARD, PRIZE_FIRST_SHARD, PRIZE_FIRST_SHARD, PRIZE_FIRST_SHARD, PRIZE_FIRST_SHARD, PRIZE_FIRST_SHARD, PRIZE_FIRST_SHARD, PRIZE_FIRST_SHARD, PRIZE_FIRST_SHARD,
	/* 9 %*/ PRIZE_FIRST_EVO_STONE, PRIZE_FIRST_EVO_STONE, PRIZE_FIRST_EVO_STONE, PRIZE_FIRST_EVO_STONE, PRIZE_FIRST_EVO_STONE, PRIZE_FIRST_EVO_STONE, PRIZE_FIRST_EVO_STONE, PRIZE_FIRST_EVO_STONE, PRIZE_FIRST_EVO_STONE,
	/* 6 %*/ PRIZE_HEART_SCALE, PRIZE_HEART_SCALE, PRIZE_HEART_SCALE, PRIZE_HEART_SCALE, PRIZE_HEART_SCALE, PRIZE_HEART_SCALE,
	/* 5 %*/ PRIZE_REVIVE, PRIZE_REVIVE, PRIZE_REVIVE, PRIZE_REVIVE, PRIZE_REVIVE,
	/* 5 %*/ PRIZE_MAX_REVIVE, PRIZE_MAX_REVIVE, PRIZE_MAX_REVIVE, PRIZE_MAX_REVIVE, PRIZE_MAX_REVIVE
	/* 15 %*/ PRIZE_STAR_PIECE, PRIZE_STAR_PIECE, PRIZE_STAR_PIECE, PRIZE_STAR_PIECE, PRIZE_STAR_PIECE, PRIZE_STAR_PIECE, PRIZE_STAR_PIECE, PRIZE_STAR_PIECE, PRIZE_STAR_PIECE, PRIZE_STAR_PIECE,
	/*"" %*/ PRIZE_STAR_PIECE, PRIZE_STAR_PIECE, PRIZE_STAR_PIECE, PRIZE_STAR_PIECE, PRIZE_STAR_PIECE, 
	/*10 %*/ PRIZE_FIRST_FOSSIL, PRIZE_FIRST_FOSSIL, PRIZE_FIRST_FOSSIL, PRIZE_FIRST_FOSSIL, PRIZE_FIRST_FOSSIL, PRIZE_FIRST_FOSSIL, PRIZE_FIRST_FOSSIL, PRIZE_FIRST_FOSSIL, PRIZE_FIRST_FOSSIL, PRIZE_FIRST_FOSSIL,
};

//Bedrock
struct Bedrock
{
	const u8* spriteTiles;
	const PrizeShape_T** shapes;
	const struct OamData* oam;
	const union AnimCmd* const* anims;
	u8 numShapes;
	u16 spriteSize;
};

static const u8 sBedrock0Shape0[MAX_OBJECT_HEIGHT][MAX_OBJECT_WIDTH] =
{
	{1},
	{1},
	{1},
	{1},
};

static const u8 sBedrock1Shape0[MAX_OBJECT_HEIGHT][MAX_OBJECT_WIDTH] =
{
	{1, 1, 1, 1},
};

static const u8 sBedrock2Shape0[MAX_OBJECT_HEIGHT][MAX_OBJECT_WIDTH] =
{
	{1, 1},
	{1, 1},
	{1, 1},
	{1, 1},
};

static const u8 sBedrock3Shape0[MAX_OBJECT_HEIGHT][MAX_OBJECT_WIDTH] =
{
	{1, 1, 1, 1},
	{1, 1, 1, 1},
};

static const u8 sBedrock4Shape0[MAX_OBJECT_HEIGHT][MAX_OBJECT_WIDTH] =
{
	{1, 1, 1},
	{1, 1, 1},
	{1, 1, 1},
};

static const u8 sBedrock5Shape0[MAX_OBJECT_HEIGHT][MAX_OBJECT_WIDTH] =
{
	{1, 1},
	{1, 1},
};

static const u8 sBedrock6Shape0[MAX_OBJECT_HEIGHT][MAX_OBJECT_WIDTH] =
{
	{1, 0},
	{1, 1},
	{0, 1},
};

static const u8 sBedrock6Shape1[MAX_OBJECT_HEIGHT][MAX_OBJECT_WIDTH] =
{
	{0, 1},
	{1, 1},
	{1, 0},
};

static const u8 sBedrock7Shape0[MAX_OBJECT_HEIGHT][MAX_OBJECT_WIDTH] =
{
	{0, 1, 1},
	{1, 1, 0},
};

static const u8 sBedrock7Shape1[MAX_OBJECT_HEIGHT][MAX_OBJECT_WIDTH] =
{
	{1, 1, 0},
	{0, 1, 1},
};

static const u8 sBedrock8Shape0[MAX_OBJECT_HEIGHT][MAX_OBJECT_WIDTH] =
{
	{1, 0},
	{1, 1},
	{1, 0},
};

static const u8 sBedrock8Shape1[MAX_OBJECT_HEIGHT][MAX_OBJECT_WIDTH] =
{
	{0, 1},
	{1, 1},
	{0, 1},
};

static const u8 sBedrock9Shape0[MAX_OBJECT_HEIGHT][MAX_OBJECT_WIDTH] =
{
	{1, 1, 1},
	{0, 1, 0},
};

static const u8 sBedrock9Shape1[MAX_OBJECT_HEIGHT][MAX_OBJECT_WIDTH] =
{
	{0, 1, 0},
	{1, 1, 1},
};

static const PrizeShape_T* sBedrock0Shapes[] =
{
	sBedrock0Shape0,
};

static const PrizeShape_T* sBedrock1Shapes[] =
{
	sBedrock1Shape0,
};

static const PrizeShape_T* sBedrock2Shapes[] =
{
	sBedrock2Shape0,
};

static const PrizeShape_T* sBedrock3Shapes[] =
{
	sBedrock3Shape0,
};

static const PrizeShape_T* sBedrock4Shapes[] =
{
	sBedrock4Shape0,
};

static const PrizeShape_T* sBedrock5Shapes[] =
{
	sBedrock5Shape0,
};

static const PrizeShape_T* sBedrock6Shapes[] =
{
	sBedrock6Shape0,
	sBedrock6Shape1,
};

static const PrizeShape_T* sBedrock7Shapes[] =
{
	sBedrock7Shape0,
	sBedrock7Shape1,
};

static const PrizeShape_T* sBedrock8Shapes[] =
{
	sBedrock8Shape0,
	sBedrock8Shape1,
};

static const PrizeShape_T* sBedrock9Shapes[] =
{
	sBedrock9Shape0,
	sBedrock9Shape1,
};

static const union AnimCmd sAnimCmdFlipBedrockNormal[] =
{
	ANIMCMD_FRAME(0, 1),
	ANIMCMD_END,
};

static const union AnimCmd sAnimCmdFlipBedrockHorizontal[] =
{
	ANIMCMD_FRAME(0, 1, .hFlip = TRUE),
	ANIMCMD_END,
};

static const union AnimCmd sAnimCmdFlipBedrockVertical[] =
{
	ANIMCMD_FRAME(0, 1, .vFlip = TRUE),
	ANIMCMD_END,
};

const union AnimCmd *const sAnimCmdBedrock68[] =
{
	sAnimCmdFlipBedrockNormal,
	sAnimCmdFlipBedrockHorizontal,
};

const union AnimCmd *const sAnimCmdBedrock79[] =
{
	sAnimCmdFlipBedrockNormal,
	sAnimCmdFlipBedrockVertical,
};

struct Bedrock sBedrocks[] =
{
	{MiningBedrock0Tiles, sBedrock0Shapes, &gOamData_AffineOff_ObjNormal_32x64, gDummySpriteAnimTable, NELEMS(sBedrock0Shapes), (32 * 64) / 2},
	{MiningBedrock1Tiles, sBedrock1Shapes, &gOamData_AffineOff_ObjNormal_64x32, gDummySpriteAnimTable, NELEMS(sBedrock1Shapes), (64 * 32) / 2},
	{MiningBedrock2Tiles, sBedrock2Shapes, &gOamData_AffineOff_ObjNormal_32x64, gDummySpriteAnimTable, NELEMS(sBedrock2Shapes), (32 * 64) / 2},
	{MiningBedrock3Tiles, sBedrock3Shapes, &gOamData_AffineOff_ObjNormal_64x32, gDummySpriteAnimTable, NELEMS(sBedrock3Shapes), (64 * 32) / 2},
	{MiningBedrock4Tiles, sBedrock4Shapes, &gOamData_AffineOff_ObjNormal_64x64, gDummySpriteAnimTable, NELEMS(sBedrock4Shapes), (64 * 64) / 2},
	{MiningBedrock5Tiles, sBedrock5Shapes, &gOamData_AffineOff_ObjNormal_32x32, gDummySpriteAnimTable, NELEMS(sBedrock5Shapes), (32 * 32) / 2},
	{MiningBedrock6Tiles, sBedrock6Shapes, &gOamData_AffineOff_ObjNormal_32x64, sAnimCmdBedrock68,     NELEMS(sBedrock6Shapes), (32 * 64) / 2},
	{MiningBedrock7Tiles, sBedrock7Shapes, &gOamData_AffineOff_ObjNormal_64x32, sAnimCmdBedrock79,     NELEMS(sBedrock7Shapes), (64 * 32) / 2},
	{MiningBedrock8Tiles, sBedrock8Shapes, &gOamData_AffineOff_ObjNormal_32x64, sAnimCmdBedrock68,     NELEMS(sBedrock8Shapes), (32 * 64) / 2},
	{MiningBedrock9Tiles, sBedrock9Shapes, &gOamData_AffineOff_ObjNormal_64x32, sAnimCmdBedrock79,     NELEMS(sBedrock9Shapes), (64 * 32) / 2},
};

//Dirt & Stress
enum DirtType
{
	L1_NO_ROCK,
	L2_NO_ROCK,
	L2_SMALL_ROCK,
	L3_NO_ROCK,
	L3_SMALL_ROCK,
	L3_MED_ROCK,
	L3_BIG_ROCK,
};

//DirtTile[0] = (DirtType * 4) + 1

enum DirtTiles
{
	//First Layer
	L1_NO_ROCK_TOP_LEFT,
	L1_NO_ROCK_TOP_RIGHT,
	L1_NO_ROCK_BOTTOM_LEFT,
	L1_NO_ROCK_BOTTOM_RIGHT,

	//Second Layer
	L2_NO_ROCK_TOP_LEFT,
	L2_NO_ROCK_TOP_RIGHT,
	L2_NO_ROCK_BOTTOM_LEFT,
	L2_NO_ROCK_BOTTOM_RIGHT,

	L2_SMALL_ROCK_TOP_LEFT,
	L2_SMALL_ROCK_TOP_RIGHT,
	L2_SMALL_ROCK_BOTTOM_LEFT,
	L2_SMALL_ROCK_BOTTOM_RIGHT,

	//Third Layer
	L3_NO_ROCK_TOP_LEFT,
	L3_NO_ROCK_TOP_RIGHT,
	L3_NO_ROCK_BOTTOM_LEFT,
	L3_NO_ROCK_BOTTOM_RIGHT,

	L3_SMALL_ROCK_TOP_LEFT,
	L3_SMALL_ROCK_TOP_RIGHT,
	L3_SMALL_ROCK_BOTTOM_LEFT,
	L3_SMALL_ROCK_BOTTOM_RIGHT,

	L3_MED_ROCK_TOP_LEFT,
	L3_MED_ROCK_TOP_RIGHT,
	L3_MED_ROCK_BOTTOM_LEFT,
	L3_MED_ROCK_BOTTOM_RIGHT,

	L3_BIG_ROCK_TOP_LEFT,
	L3_BIG_ROCK_TOP_RIGHT,
	L3_BIG_ROCK_BOTTOM_LEFT,
	L3_BIG_ROCK_BOTTOM_RIGHT,
	
	//Stress State 1
	STRESS_1_1_2,
	STRESS_1_1_3,
	STRESS_1_2_2,
	STRESS_1_2_3,
	STRESS_1_1_4,
	STRESS_1_1_UNUSED,
	STRESS_1_2_4,
	STRESS_1_3_4,
	
	//Stress State 2
	STRESS_2_0_2,
	STRESS_2_0_3,
	STRESS_2_1_2,
	STRESS_2_1_3,
	STRESS_2_2_3,
	STRESS_2_2_4,
	STRESS_2_1_4,
	STRESS_2_3_4,
	
	//Stress State 3
	STRESS_3_0_1,
	STRESS_3_0_2,
	STRESS_3_1_1,
	STRESS_3_1_2,
	STRESS_3_1_3,
	STRESS_3_1_4,
	STRESS_3_2_3,
	STRESS_3_2_4,
	STRESS_3_0_3,
	STRESS_3_2_2,
	
	//Stress State 4
	STRESS_4_0_3,
	STRESS_5_0_3,
	STRESS_4_1_1,
	STRESS_4_1_2,
	STRESS_4_2_1,
	STRESS_4_2_2,
	STRESS_4_1_3,
	STRESS_4_1_4,
	STRESS_4_2_3,
	STRESS_4_2_4,

	//Stress State 5
	STRESS_5_1_1,
	STRESS_5_1_2,
	STRESS_5_2_1,
	STRESS_5_2_2,
	STRESS_5_1_3,
	STRESS_5_1_4,
	STRESS_5_2_3,
	STRESS_5_2_4,
	STRESS_5_2_0,
	STRESS_5_3_2,
	STRESS_5_3_0,
	STRESS_5_3_1,

	//Stress State 6
	STRESS_6_1_1,
	STRESS_6_1_2,
	STRESS_6_2_1,
	STRESS_6_2_2,
	STRESS_6_2_0,
	STRESS_6_3_2,
	STRESS_6_3_0,
	STRESS_6_3_1,

	//Stress State 7
	STRESS_7_1_4,
	STRESS_7_3_4,
	STRESS_7_2_4,
};

static const u8 sTilesByStressState[][4][5] =
{
	[1] =
	{
		{0, 0,            0,            0,            0},
		{0, 0, STRESS_1_1_2, STRESS_1_1_3, STRESS_1_1_4},
		{0, 0,            0, STRESS_1_2_3, STRESS_1_2_4},
		{0, 0,            0,            0, STRESS_1_3_4},
	},
	[2] =
	{
		{0, 0, STRESS_2_0_2, STRESS_2_0_3,            0},
		{0, 0, STRESS_2_1_2, STRESS_2_1_3, STRESS_2_1_4},
		{0, 0,            0, STRESS_2_2_3, STRESS_2_2_4},
		{0, 0,            0,            0, STRESS_2_3_4},
	},
	[3] =
	{
		{0, STRESS_3_0_1, STRESS_3_0_2, STRESS_3_0_3,            0},
		{0, STRESS_3_1_1, STRESS_3_1_2, STRESS_3_1_3, STRESS_3_1_4},
		{0,            0, STRESS_3_2_2, STRESS_3_2_3, STRESS_3_2_4},
		{0,            0,            0,            0, STRESS_2_3_4},
	},
	[4] =
	{
		{0,            0,            0, STRESS_4_0_3,            0},
		{0, STRESS_4_1_1, STRESS_4_1_2, STRESS_4_1_3, STRESS_4_1_4},
		{0, STRESS_4_2_1, STRESS_4_2_2, STRESS_4_2_3, STRESS_4_2_4},
		{0,            0,            0,            0, STRESS_2_3_4},
	},
	[5] =
	{
		{0,            0,            0,            STRESS_5_0_3,            0},
		{0,            STRESS_5_1_1, STRESS_5_1_2, STRESS_5_1_3, STRESS_5_1_4},
		{STRESS_5_2_0, STRESS_5_2_1, STRESS_5_2_2, STRESS_5_2_3, STRESS_5_2_4},
		{STRESS_5_3_0, STRESS_5_3_1, STRESS_5_3_2,            0, STRESS_2_3_4},
	},
	[6] =
	{
		{0,                       0,            0, STRESS_5_0_3,            0},
		{0,            STRESS_6_1_1, STRESS_6_1_2, STRESS_5_1_3, STRESS_5_1_4},
		{STRESS_6_2_0, STRESS_6_2_1, STRESS_6_2_2, STRESS_5_2_3, STRESS_5_2_4},
		{STRESS_6_3_0, STRESS_6_3_1, STRESS_6_3_2,            0, STRESS_2_3_4},
	},
	[7] = //Start shifting over by 2
	{
		{0,            0,            0,            0,            0},
		{0, STRESS_1_1_2, STRESS_1_1_3, STRESS_1_1_4, STRESS_7_1_4},
		{0,            0, STRESS_1_2_3, STRESS_1_2_4, STRESS_7_2_4},
		{0,            0,            0, STRESS_1_3_4, STRESS_7_3_4},
	},
/*	[8] = //Shifting over by 1 - Same as 2
	{
		{0, 0, 1, 1, 0},
		{0, 0, 1, 1, 1},
		{0, 0, 0, 1, 1},
		{0, 0, 0, 0, 1},
	},*/
};

extern const u8 UndergroundMiningBaseTiles[];
extern const u16 UndergroundMiningBaseMap[];
extern const u16 UndergroundMiningBasePal[];
extern const u8 UndergroundMiningDirtTiles[];
extern const u16 UndergroundMiningDirtPal[];
extern const u8 UndergroundMiningTextboxTiles[];
extern const u16 UndergroundMiningTextboxPal[];
extern const u16 UndergroundMiningTextboxMap[];
extern const u8 UndergroundMiningCollapseTiles[];
extern const u16 UndergroundMiningCollapsePal[];
extern const u16 UndergroundMiningCollapseMap[];
extern const u8 MiningHammerTiles[];
extern const u16 MiningHammerPal[];
extern const u8 MiningPickAxeTiles[];
extern const u16 MiningPickAxePal[];
extern const u8 MiningCursorTiles[];
extern const u8 MiningMiniToolsTiles[];
extern const u16 MiningMiniToolsPal[];
extern const u8 MiningImpactTiles[];
extern const u16 MiningImpactPal[];

extern const u8 gText_UndergroundMining_DisplayPossiblePrizes[];
extern const u8 gText_UndergroundMining_Instructions[];
extern const u8 gText_UndergroundMining_WallCollapsed[];
extern const u8 gText_UndergroundMining_GotAllItems[];
extern const u8 gText_UndergroundMining_FoundItem[];
extern const u8 gText_UndergroundMining_NoSpace[];
extern const u8 gText_UndergroundMining_ToolSwapInstructions[];
