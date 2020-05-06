#pragma once

#include "../global.h"

/**
 * \file dns_data.h
 * \brief A file to be included only by "src/dexnav.c". It contains declarations,
 *		  constants, and data structures used by the DexNav feature.
 */

// IMAGES
extern const u8 gInterfaceGfx_dexnavGuiTiles[];
extern const u8 gInterfaceGfx_dexnavGuiMap[];
extern const u8 gInterfaceGfx_dexnavGuiPal[];
extern const u8 gInterfaceGfx_DexNavGuiVolcanoPal[];
extern const u8 gInterfaceGfx_DexNavGuiFlowerParadiseAPal[];
extern const u8 gInterfaceGfx_DexNavGuiFlowerParadiseBPal[];
extern const u8 gInterfaceGfx_DexNavGuiFlowerParadiseCPal[];
extern const u8 gInterfaceGfx_DexNavGuiAutumnPal[];
extern const u8 gInterfaceGfx_DexNavGuiWinterPal[];
extern const u8 gInterfaceGfx_DexNavGuiSwampPal[];
extern const u8 gInterfaceGfx_DexNavGuiCavePal[];
extern const u8 gInterfaceGfx_DexNavGuiDarkerCavePal[];
extern const u8 gInterfaceGfx_DexNavGuiIndoorPal[];
extern const u8 gInterfaceGfx_dexnavStarsTiles[];
extern const u8 gInterfaceGfx_dexnavStarsPal[];
extern const u8 gInterfaceGfx_selectionCursorTiles[];
extern const u8 gInterfaceGfx_selectionCursorPal[];
extern const u8 gInterfaceGfx_emptyTiles[];
extern const u8 gInterfaceGfx_emptyPal[];
extern const u8 gInterfaceGfx_CapturedAllPokemonTiles[];
extern const u8 gInterfaceGfx_CapturedAllPokemonPal[];
extern const u8 gInterfaceGfx_caveSmokeTiles[];
extern const u16 gInterfaceGfx_caveSmokePal[];
extern const u8 gInterfaceGfx_SparklesTiles[];
extern const u16 gInterfaceGfx_SparklesPal[];
extern const u8 gInterfaceGfx_LavaBubblesTiles[];
extern const u16 gInterfaceGfx_LavaBubblesPal[];
extern const u8 gInterfaceGfx_DexNavNoDataSymbolTiles[];

// STRINGS
extern const u8 gText_DexNavWater[];
extern const u8 gText_DexNavLand[];
extern const u8 gText_PinkFlowers[];
extern const u8 gText_YellowFlowers[];
extern const u8 gText_RedFlowers[];
extern const u8 gText_BlueFlowers[];
extern const u8 gText_PinkAndPurpleFlowers[];
extern const u8 gText_BlueAndYellowFlowers[];
extern const u8 gText_Magma[];
extern const u8 gText_PokeTools[];
extern const u8 gText_GotAway[];
extern const u8 gText_LostSignal[];
extern const u8 gText_GotAwayShouldSneak[];
extern const u8 gText_CannotBeFound[];
extern const u8 gText_NotFoundNearby[];
extern const u8 gText_DexNavBack[];
extern const u8 gText_DexNav_NoInfo[];
extern const u8 gText_DexNav_CaptureToSee[];
extern const u8 gText_DexNav_ChooseMon[];
extern const u8 gText_DexNav_Invalid[];
extern const u8 gText_DexNav_NoDataForSlot[];
extern const u8 gText_DexNav_Locked[];

// TYPEDEFS
typedef void (*SuperCallback)(void);
typedef void (*SpriteCallback)(struct Sprite* s);

// DEFINES
#define DEXNAV_SAVERAM 0x203C75C

#define TILE_SIZE 32
#define SPRITE_RAM 0x6010000

#define TOOL_COUNT 2
#define priv0 gTasks[taskId].data[0]

#define CPUFSCPY 0
#define CPUFSSET 1
#define CPUModeFS(size, mode) ((size >> 2) | (mode << 24))

#define SCANSTART_X 0
#define SCANSTART_Y 0
#define SCANSIZE_X 12
#define SCANSIZE_Y 12
//extern void CpuFastSet(void* src, void* dst, u32 mode);

#define ICON_PAL_TAG 0xDAC0
#define ICON_GFX_TAG 0xD75A
#define SELECTION_CURSOR_TAG 0x200

#define SMOKE_TAG 0x2710

#define ICONX 0x10
#define ICONY 0x92

//extern void dprintf(const char * str, ...);

//#define OBJID_HIDE(objid) objects[objid].final_oam.affineMode = 2
//#define OBJID_SHOW(objid) objects[objid].final_oam.affineMode = 0

#define ICONX 0x10
#define ICONY 0x92

#define NUM_LAND_MONS 12
#define NUM_WATER_MONS 5

struct FieldEffectArguments
{
    struct Coords32 effectPos;
    u32 priority;
};

extern u8 gMoveNames[][MOVE_NAME_LENGTH + 1];

// STRUCTS
static u8* sSearchLevels = (u8*) (DEXNAV_SAVERAM);
static const struct TextColor MenuTextBlack = {0, 2, 3};
static const struct WindowTemplate Tbox = {0, 2, 2, 10, TOOL_COUNT << 1, 0xF, 0x130};

static const struct TextColor DexNav_BlackText = {0, 3, 4};
static const struct TextColor DexNav_WhiteText = {0, 1, 2};
static const struct TextColor DexNav_RedText = {0, 7, 8};
static const struct TextColor DexNav_GreenText = {0, 5, 6};

struct DexnavHudData
{
    u16 species;
    u16 moveId[MAX_MON_MOVES];
    u16 heldItem;
    u8 ability;
    u8 potential;
    u8 searchLevel;
    u8 pokemonLevel;
    u8 moveNameLength;
    u8 proximity;
    u8 environment;
	u8 unownLetter;
    s16 tileX; // position of shaking grass
    s16 tileY;
    u8 objIdSpecies;
    u8 objIdBlackBar[4];
    u8 objIdSight;
    u8 objIdAbility;
    u8 objIdMove;
    u8 objIdItem;
    u8 objIdShakingGrass;
    u8 objIdPotential[3];
    u8 movementTimes;

    // GUI data
    u16 grassSpecies[NUM_LAND_MONS];
    u16 waterSpecies[NUM_WATER_MONS];
	u16 hiddenSpecies[NUM_LAND_MONS + 1];
	u8 unownForms[NUM_LAND_MONS];
	u8 unownFormsByDNavIndices[NUM_LAND_MONS];
	u8 numGrassMons;
	u8 numWaterMons;
	u8 numHiddenLandMons;
	u8 numHiddenWaterMons;
    u8 cursorId;
    u8 objids[17];
    u8 selectedIndex;
    u8 selectedArr;
    void* backBuffer;
};

/*
struct OieState2
{
    const struct SpritePalette* p;
    SuperCallback s;
};
*/

static const struct BgTemplate BgConfigDexNavGUI[4] =
{
    {
        .baseTile = 0,
        .priority = 2,
        .paletteMode = 0,
        .screenSize = 0,
        .mapBaseIndex = 29,
        .charBaseIndex = 1,
        .bg = 0,
    },
    {
        .baseTile = 0,
        .priority = 3,
        .paletteMode = 0,
        .screenSize = 0,
        .mapBaseIndex = 28,
        .charBaseIndex = 0,
        .bg = 1,
    },
    {
        .baseTile = 0,
        .priority = 3,
        .paletteMode = 0,
        .screenSize = 0,
        .mapBaseIndex = 30,
        .charBaseIndex = 2,
        .bg = 2,
    },
    {
        .baseTile = 0,
        .priority = 3,
        .paletteMode = 0,
        .screenSize = 1,
        .mapBaseIndex = 31,
        .charBaseIndex = 3,
        .bg = 3,
    },
};


static const struct OamData sCapturedAllPokemonSymbolOAM =
{
    .y = 0,
    .affineMode = 1,
    .objMode = 0,
    .mosaic = 0,
    .bpp = 0,
    .shape = SPRITE_SHAPE(8x8),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(8x8),
    .tileNum = 0,
    .priority = 0, //Highest
    .paletteNum = 0,
    .affineParam = 0,
};

static const struct OamData IconOAM =
{
    .y = 0,
    .affineMode = 1,
    .objMode = 0,
    .mosaic = 0,
    .bpp = 0,
    .shape = 0,
    .x = 0,
    .matrixNum = 0,
    .size = 2, // 32x32 square
    .tileNum = 0,
    .priority = 2, //above the rest
    .paletteNum = 0,
    .affineParam = 0,
};

static const struct OamData CursorOAM =
{
    .y = 0,
    .affineMode = 0,
    .objMode = 0,
    .mosaic = 0,
    .bpp = 0,
    .shape = 0,
    .x = 0,
    .matrixNum = 0,
    .size = 2, //32x32
    .tileNum = 0,
    .priority = 0, // above BG layers
    .paletteNum = 0,
    .affineParam = 0
};


// cursor positions for water
static const u16 CursorPositions2[] =
{
    30 + 24 * 0, 48,
    30 + 24 * 1, 48,
    30 + 24 * 2, 48,
    30 + 24 * 3, 48,
    30 + 24 * 4, 48,
};

// positions for grass
static const u16 CursorPositions1[] =
{
    20 + 24 * 0, 92,
    20 + 24 * 1, 92,
    20 + 24 * 2, 92,
    20 + 24 * 3, 92,
    20 + 24 * 4, 92,
    20 + 24 * 5, 92,
    20 + 24 * 0, 92 + 28,
    20 + 24 * 1, 92 + 28,
    20 + 24 * 2, 92 + 28,
    20 + 24 * 3, 92 + 28,
    20 + 24 * 4, 92 + 28,
    20 + 24 * 5, 92 + 28,
};


// GUI Windows
#define rgb5(r, g, b) (u16)((r >> 3) | ((g >> 3) << 5) | ((b >> 3) << 10))
static const u16 DexNavTextPal[] =
{
	rgb5(255, 0, 255), rgb5(248, 248, 248), rgb5(112, 112, 112), rgb5(96, 96, 96),
	rgb5(208, 208, 208), rgb5(76, 154, 38), rgb5(102, 194, 66), rgb5(168, 75, 76),
	rgb5(224, 114, 75), rgb5(180, 124, 41), rgb5(241, 188, 60), rgb5(255, 0, 255),
	rgb5(255, 0, 255), rgb5(255, 0, 255), rgb5(255, 133, 200), rgb5(64, 200, 248)
};

enum
{
	WINDOW_SPECIES,
	WINDOW_SEARCH_LEVEL,
	WINDOW_LEVEL_BONUS,
	WINDOW_HIDDEN_ABILITY,
	WINDOW_REPLY_TEXT,
	WINDOW_WATER,
	WINDOW_LAND,
	WINDOW_MAP_NAME,
	WINDOW_COUNT,
};


//extern const struct WindowTemplate sDexNavWindows[];
static const struct WindowTemplate sDexNavWindows[] =
{
	[WINDOW_SPECIES] =
	{
        .bg = 0,
        .tilemapLeft = 21,
        .tilemapTop = 6,
        .width = 9,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = 1,
	},

	[WINDOW_SEARCH_LEVEL] =
    {
        .bg = 0,
        .tilemapLeft = 21,
        .tilemapTop = 9,
        .width = 9,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = 19,
    },

	[WINDOW_LEVEL_BONUS] =
    {
        .bg = 0,
        .tilemapLeft = 21,
        .tilemapTop = 12,
        .width = 9,
        .height = 3,
        .paletteNum = 15,
        .baseBlock = 64,
    },

	[WINDOW_HIDDEN_ABILITY] =
    {
        .bg = 0,
        .tilemapLeft = 21,
        .tilemapTop = 15,
        .width = 12,
        .height = 3,
        .paletteNum = 15,
        .baseBlock = 91,
    },

	[WINDOW_REPLY_TEXT] =
    {
        .bg = 0,
        .tilemapLeft = 0,
        .tilemapTop = 17,
        .width = 26,
        .height = 3,
        .paletteNum = 15,
        .baseBlock = 127,
    },

	[WINDOW_WATER] =
	{
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 2,
        .width = 19,
        .height = 3,
        .paletteNum = 15,
        .baseBlock = 205,
	},

	[WINDOW_LAND] =
	{
        .bg = 0,
        .tilemapLeft = 0,
        .tilemapTop = 8,
        .width = 19,
        .height = 3,
        .paletteNum = 15,
        .baseBlock = 262,
	},

	[WINDOW_MAP_NAME] =
	{
        .bg = 0,
        .tilemapLeft = 0,
        .tilemapTop = 0,
        .width = 12,
        .height = 3,
        .paletteNum = 15,
        .baseBlock = 319,
	},

	[WINDOW_COUNT] =
    {
        .bg = 0xFF, // marks the end of the tb array
    },

};


//const struct Frame (**nullframe)[] = (const struct Frame (**)[])0x8231CF0;
//const struct AffineAnimCmd (**nullrsf)[] = (const struct AffineAnimCmd (**)[])0x8231CFC;

//64x32 oam with second highest priority
static const struct OamData sBlackBarOAM =
{
	.affineMode = ST_OAM_AFFINE_OFF,
	.objMode = ST_OAM_OBJ_NORMAL,
	.shape = SPRITE_SHAPE(64x32),
	.size = SPRITE_SIZE(64x32),
	.priority = 1, //Above everything
};


//64x32 oam with highest priority
static const struct OamData FontOAM =
{
	.affineMode = ST_OAM_AFFINE_OFF,
	.objMode = ST_OAM_OBJ_NORMAL,
	.shape = SPRITE_SHAPE(64x32),
	.size = SPRITE_SIZE(64x32),
	.priority = 0, //Above everything
};


// 8x8 oam with highest priority
static const struct OamData HeldOAM =
{
	.affineMode = ST_OAM_AFFINE_OFF,
	.objMode = ST_OAM_OBJ_NORMAL,
	.shape = SPRITE_SHAPE(8x8),
	.size = SPRITE_SIZE(8x8),
	.priority = 0, //Above everything
};

//#define gPalHeldItemIcon ((u32*) 0x0845A3EC) //Pal for held items. The yellow box and mail icon
static const struct SpritePalette HeldPal = {(void*) 0x0845A3EC, 0x8472};

// 32x32 object with priority 1, one less than held item which overlaps it
static const struct OamData PIconOAM =
{
	.y = ICONY,
	.affineMode = 0,
    .objMode = 0,
    .mosaic = 0,
    .bpp = 0,
    .shape = 0,
    .x = ICONX,
    .matrixNum = 0,
    .size = 2,
    .tileNum = 0,
    .priority = 1,
    .paletteNum = 0,
    .affineParam = 0,
};

static const struct SpriteTemplate BulbTemp =
{
	.tileTag = 0x3139,
	.paletteTag = 0x3139,
	.oam = (struct OamData*) &PIconOAM,
	.anims = (const union AnimCmd* const*)0x8231CF0,
	.images = NULL,
    .affineAnims = (const union AffineAnimCmd* const*) 0x8231CFC,
	.callback = (SpriteCallback) 0x800760D,
};

