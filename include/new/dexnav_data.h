#pragma once

#include "../global.h"

/**
 * \file dns_data.h
 * \brief A file to be included only by "src/dexnav.c". It contains declarations,
 *		  constants, and data structures used by the DexNav feature.
 */

//IMAGES
extern const u8 gInterfaceGfx_dexnavGuiTiles[];
extern const u8 gInterfaceGfx_dexnavGuiMap[];
extern const u8 gInterfaceGfx_dexnavGuiPal[];
extern const u8 gInterfaceGfx_DexNavGuiVolcanoPal[];
extern const u8 gInterfaceGfx_DexNavGuiFlowerParadiseAPal[];
extern const u8 gInterfaceGfx_DexNavGuiFlowerParadiseBPal[];
extern const u8 gInterfaceGfx_DexNavGuiFlowerParadiseCPal[];
extern const u8 gInterfaceGfx_DexNavGuiAutumnPal[];
extern const u8 gInterfaceGfx_DexNavGuiWinterPal[];
extern const u8 gInterfaceGfx_DexNavGuiDesertPal[];
extern const u8 gInterfaceGfx_DexNavGuiSwampPal[];
extern const u8 gInterfaceGfx_DexNavGuiCavePal[];
extern const u8 gInterfaceGfx_DexNavGuiDarkerCavePal[];
extern const u8 gInterfaceGfx_DexNavGuiIndoorPal[];
extern const u8 gInterfaceGfx_dexnavStarsTiles[];
extern const u8 gInterfaceGfx_dexnavStarsPal[];
extern const u8 DexNavCursorTiles[];
extern const u8 DexNavCursorPal[];
extern const u8 gInterfaceGfx_emptyTiles[];
extern const u8 gInterfaceGfx_emptyPal[];
extern const u8 gInterfaceGfx_CapturedAllPokemonTiles[];
extern const u8 gInterfaceGfx_CapturedAllPokemonPal[];
extern const u8 gInterfaceGfx_DexNavNoDataSymbolTiles[];
extern const u8 DexNavBarTiles[];
extern const u8 DexNavSightTiles[];
extern const u8 DexNavBButtonTiles[];

//STRINGS
extern const u8 gText_DexNavInstructions[];
extern const u8 gText_DexNavWater[];
extern const u8 gText_DexNavLand[];
extern const u8 gText_PinkFlowers[];
extern const u8 gText_YellowFlowers[];
extern const u8 gText_RedFlowers[];
extern const u8 gText_BlueFlowers[];
extern const u8 gText_PinkAndPurpleFlowers[];
extern const u8 gText_BlueAndYellowFlowers[];
extern const u8 gText_Magma[];
extern const u8 gText_Sand[];
extern const u8 gText_PokeTools[];
extern const u8 gText_GotAway[];
extern const u8 gText_LostSignal[];
extern const u8 gText_GotAwayShouldSneak[];
extern const u8 gText_CannotBeFound[];
extern const u8 gText_NotFoundNearby[];
extern const u8 gText_TooDarkForDexNav[];
extern const u8 gText_DexNavBack[];
extern const u8 gText_DexNav_NoInfo[];
extern const u8 gText_DexNav_CaptureToSee[];
extern const u8 gText_DexNav_ChooseMon[];
extern const u8 gText_DexNav_Invalid[];
extern const u8 gText_DexNav_NoDataForSlot[];
extern const u8 gText_DexNav_Locked[];
extern const u8 gText_DexNav_TooDark[];

// DEFINES
#define sSearchLevels ((u8*) 0x203C75C)

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

#define ICON_PAL_TAG 0xDAC0
#define ICON_GFX_TAG 0xD75A
#define SELECTION_CURSOR_TAG 0x200

#define ICONX 0x10
#define ICONY 0x92

#define ICONX 0x10
#define ICONY 0x92

#define NUM_LAND_MONS 12
#define NUM_WATER_MONS 5

#define LAND_ROW_LENGTH (6 * 2)
#define LAND_FIRST_ROW_LAST_INDEX (5 * 2)
#define LAND_SECOND_ROW_LAST_INDEX (LAND_FIRST_ROW_LAST_INDEX + LAND_ROW_LENGTH)
#define LAND_SECOND_ROW_FIRST_INDEX (6 * 2)
#define WATER_ROW_LAST_INDEX (4 * 2)
#define ROW_MON_LENGTH 2

extern u8 gMoveNames[][MOVE_NAME_LENGTH + 1];

//GUI Data
struct DexNavGuiData
{
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
    u8 spriteIds[17];
    u8 selectedIndex;
    u8 selectedArr;
	u8* tilemapPtr;
};

enum DexNavMessages
{
	MESSAGE_INVALID,
	MESSAGE_CHOOSE_MON,
	MESSAGE_REGISTERED,
	MESSAGE_NO_DATA,
	MESSAGE_TOO_DARK,
};

enum BGs
{
	BG_TEXT,
	BG_UNUSED_1,
	BG_UNUSED_2,
	BG_BACKGROUND,
};

enum
{
	WIN_SPECIES,
	WIN_SEARCH_LEVEL,
	WIN_LEVEL_BONUS,
	WIN_HIDDEN_ABILITY,
	WIN_MESSAGE,
	WIN_WATER,
	WIN_LAND,
	WIN_MAP_NAME,
	WIN_INSTRUCTIONS,
	WINDOW_COUNT,
};

static const struct WindowTemplate sDexNavWinTemplates[WINDOW_COUNT + 1] =
{
	[WIN_SPECIES] =
	{
        .bg = BG_TEXT,
        .tilemapLeft = 21,
        .tilemapTop = 6,
        .width = 9,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = 1,
	},
	[WIN_SEARCH_LEVEL] =
    {
        .bg = BG_TEXT,
        .tilemapLeft = 21,
        .tilemapTop = 9,
        .width = 9,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = 19,
    },
	[WIN_LEVEL_BONUS] =
    {
        .bg = BG_TEXT,
        .tilemapLeft = 21,
        .tilemapTop = 12,
        .width = 9,
        .height = 3,
        .paletteNum = 15,
        .baseBlock = 64,
    },
	[WIN_HIDDEN_ABILITY] =
    {
        .bg = BG_TEXT,
        .tilemapLeft = 21,
        .tilemapTop = 15,
        .width = 12,
        .height = 3,
        .paletteNum = 15,
        .baseBlock = 91,
    },
	[WIN_MESSAGE] =
    {
        .bg = BG_TEXT,
        .tilemapLeft = 0,
        .tilemapTop = 17,
        .width = 27,
        .height = 3,
        .paletteNum = 15,
        .baseBlock = 127,
    },
	[WIN_WATER] =
	{
        .bg = BG_TEXT,
        .tilemapLeft = 1,
        .tilemapTop = 2,
        .width = 19,
        .height = 3,
        .paletteNum = 15,
        .baseBlock = 208,
	},
	[WIN_LAND] =
	{
        .bg = BG_TEXT,
        .tilemapLeft = 0,
        .tilemapTop = 8,
        .width = 19,
        .height = 3,
        .paletteNum = 15,
        .baseBlock = 265,
	},
	[WIN_MAP_NAME] =
	{
        .bg = BG_TEXT,
        .tilemapLeft = 0,
        .tilemapTop = 0,
        .width = 12,
        .height = 3,
        .paletteNum = 15,
        .baseBlock = 322,
	},
	[WIN_INSTRUCTIONS] =
	{
        .bg = BG_TEXT,
        .tilemapLeft = 17,
        .tilemapTop = 0,
        .width = 13,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = 358,
	},
    DUMMY_WIN_TEMPLATE
};

static const struct BgTemplate sDexNavBgTemplates[] =
{
    [BG_TEXT] =
	{
        .bg = 0,
        .charBaseIndex = 0,
        .mapBaseIndex = 31,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0,
    },
	[BG_UNUSED_1] =
    {
        .bg = 1,
        .charBaseIndex = 1,
        .mapBaseIndex = 30,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 1,
        .baseTile = 0,
    },
	[BG_UNUSED_2] =
    {
        .bg = 2,
        .charBaseIndex = 2,
        .mapBaseIndex = 29,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 2,
        .baseTile = 0,
    },
	[BG_BACKGROUND] =
    {
        .bg = 3,
        .charBaseIndex = 3,
        .mapBaseIndex = 28,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 3,
        .baseTile = 0,
    },
};

static const struct OamData sCursorOam =
{
	.affineMode = ST_OAM_AFFINE_OFF,
	.objMode = ST_OAM_OBJ_NORMAL,
	.shape = SPRITE_SHAPE(32x32),
	.size = SPRITE_SIZE(32x32),
	.priority = 0, //Above other sprites
};

static const struct OamData sCapturedAllPokemonSymbolOam =
{
	.affineMode = ST_OAM_AFFINE_OFF,
	.objMode = ST_OAM_OBJ_NORMAL,
	.shape = SPRITE_SHAPE(8x8),
	.size = SPRITE_SIZE(8x8),
	.priority = 2,
};

static const struct OamData sNoDataIconOam =
{
	.affineMode = ST_OAM_AFFINE_OFF,
	.objMode = ST_OAM_OBJ_NORMAL,
	.shape = SPRITE_SHAPE(32x32),
	.size = SPRITE_SIZE(32x32),
	.priority = 2,
};

static void SpriteCB_GUICursor(struct Sprite* sprite);
static const struct SpriteTemplate sGUICursorTemplate =
{
	.tileTag = SELECTION_CURSOR_TAG,
	.paletteTag = SELECTION_CURSOR_TAG,
	.oam = &sCursorOam,
	.anims = gDummySpriteAnimTable,
	.images = NULL,
	.affineAnims = gDummySpriteAffineAnimTable,
	.callback = SpriteCB_GUICursor,
};

static const struct SpriteTemplate sCapturedAllPokemonSymbolTemplate =
{
	.tileTag = 0xFDF2,
	.paletteTag = SELECTION_CURSOR_TAG,
	.oam = &sCapturedAllPokemonSymbolOam,
	.anims = gDummySpriteAnimTable,
	.images = NULL,
	.affineAnims = gDummySpriteAffineAnimTable,
	.callback = SpriteCallbackDummy,
};

static const struct SpriteTemplate sNoDataIconTemplate =
{
	.tileTag = ICON_GFX_TAG,
	.paletteTag = ICON_PAL_TAG,
	.oam = &sNoDataIconOam,
	.anims = gDummySpriteAnimTable,
	.images = NULL,
	.affineAnims = gDummySpriteAffineAnimTable,
	.callback = SpriteCallbackDummy,
};

static const struct CompressedSpriteSheet sCursorSpriteSheet = {DexNavCursorTiles, (32 * 32) / 2, SELECTION_CURSOR_TAG};
static const struct CompressedSpritePalette sCursorSpritePalette = {DexNavCursorPal, SELECTION_CURSOR_TAG};
static const struct CompressedSpriteSheet sCapturedAllPokemonSpriteSheet = {gInterfaceGfx_CapturedAllPokemonTiles, (8 * 8) / 2, 0xFDF2}; //Tag is from Mega Evo and not in use
static const struct CompressedSpriteSheet sNoDataIconSpriteSheet = {gInterfaceGfx_DexNavNoDataSymbolTiles, (32 * 32) / 2, ICON_GFX_TAG};

static const struct TextColor sDexNav_BlackText = {0, 3, 4};
static const struct TextColor sDexNav_WhiteText = {0, 1, 2};

#define rgb5(r, g, b) (u16)((r >> 3) | ((g >> 3) << 5) | ((b >> 3) << 10))
static const u16 sDexNavGuiTextPal[] =
{
	rgb5(255, 0, 255), rgb5(248, 248, 248), rgb5(112, 112, 112), rgb5(96, 96, 96),
	rgb5(208, 208, 208), rgb5(76, 154, 38), rgb5(102, 194, 66), rgb5(168, 75, 76),
	rgb5(224, 114, 75), rgb5(180, 124, 41), rgb5(241, 188, 60), rgb5(255, 0, 255),
	rgb5(255, 0, 255), rgb5(255, 0, 255), rgb5(255, 133, 200), rgb5(64, 200, 248)
};

static const s16 sCursorPositionsLand[] =
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

static const s16 sCursorPositionsWater[] =
{
    30 + 24 * 0, 48,
    30 + 24 * 1, 48,
    30 + 24 * 2, 48,
    30 + 24 * 3, 48,
    30 + 24 * 4, 48,
};

//HUD Data

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
    u8 spriteIdSpecies;
    u8 spriteIdBlackBar[4];
    u8 spriteIdSight;
    u8 spriteIdBButton;
    u8 spriteIdAbility;
    u8 spriteIdMove;
    u8 spriteIdItem;
    u8 spriteIdShakingGrass;
    u8 spriteIdPotential[3];
    u8 movementTimes;
};

enum FieldMessages
{
	FIELD_MSG_NOT_IN_AREA, 
	FIELD_MSG_LOOK_IN_OTHER_SPOT,
	FIELD_MSG_TOO_DARK,
	FIELD_MSG_GOT_AWAY,
	FIELD_MSG_LOST_SIGNAL,
	FIELD_MSG_SNEAK_NEXT_TIME,
};

//64x32 oam with second highest priority
static const struct OamData sBlackBarOAM =
{
	.affineMode = ST_OAM_AFFINE_DOUBLE,
	.objMode = ST_OAM_OBJ_NORMAL,
	.shape = SPRITE_SHAPE(64x32),
	.size = SPRITE_SIZE(64x32),
	.priority = 1, //Above almost everything
};

static const struct OamData sSightOam =
{
	.affineMode = ST_OAM_AFFINE_OFF,
	.objMode = ST_OAM_OBJ_NORMAL,
	.shape = SPRITE_SHAPE(16x8),
	.size = SPRITE_SIZE(16x8),
	.priority = 0, //Above everything
};

static const struct OamData sBButtonOam =
{
	.affineMode = ST_OAM_AFFINE_OFF,
	.objMode = ST_OAM_OBJ_NORMAL,
	.shape = SPRITE_SHAPE(32x8),
	.size = SPRITE_SIZE(32x8),
	.priority = 0, //Above everything
};

//64x32 oam with highest priority
static const struct OamData sFontOAM =
{
	.affineMode = ST_OAM_AFFINE_OFF,
	.objMode = ST_OAM_OBJ_NORMAL,
	.shape = SPRITE_SHAPE(64x32),
	.size = SPRITE_SIZE(64x32),
	.priority = 0, //Above everything
};

//8x8 oam with highest priority
static const struct OamData sHeldItemOam =
{
	.affineMode = ST_OAM_AFFINE_OFF,
	.objMode = ST_OAM_OBJ_NORMAL,
	.shape = SPRITE_SHAPE(8x8),
	.size = SPRITE_SIZE(8x8),
	.priority = 0, //Above everything
};

static const union AnimCmd sAnimCmdSight0[] =
{
	ANIMCMD_FRAME(0, 1),
	ANIMCMD_END
};

static const union AnimCmd sAnimCmdSight1[] =
{
	ANIMCMD_FRAME(2, 1),
	ANIMCMD_END
};

static const union AnimCmd sAnimCmdSight2[] =
{
	ANIMCMD_FRAME(4, 1),
	ANIMCMD_END
};

static const union AnimCmd *const sAnimCmdTable_Sight[] =
{
	sAnimCmdSight0,
	sAnimCmdSight1,
	sAnimCmdSight2,
};

static const union AffineAnimCmd sSpriteAffineAnim_DexNavBar[] =
{
	AFFINEANIMCMD_FRAME(256, 0, 0, 1), //Double sprite width
	AFFINEANIMCMD_END,
};

static const union AffineAnimCmd* const sSpriteAffineAnimTable_DexNavBar[] =
{
	sSpriteAffineAnim_DexNavBar,
};

static const struct SpriteTemplate sBlackBarTemplate =
{
	.tileTag = 0xFDF1,
	.paletteTag = 0x8472,
	.oam = &sBlackBarOAM,
	.anims = gDummySpriteAnimTable,
	.images = NULL,
	.affineAnims = sSpriteAffineAnimTable_DexNavBar,
	.callback = SpriteCallbackDummy,
};

static const struct SpriteTemplate sSightTemplate =
{
	.tileTag = 0x5424,
	.paletteTag = 0x8472,
	.oam = &sSightOam,
	.anims = sAnimCmdTable_Sight,
	.images = NULL,
	.affineAnims = gDummySpriteAffineAnimTable,
	.callback = SpriteCallbackDummy,
};

static const struct SpriteTemplate sBButtonTemplate =
{
	.tileTag = 0x5425,
	.paletteTag = 0x8472,
	.oam = &sBButtonOam,
	.anims = gDummySpriteAnimTable,
	.images = NULL,
	.affineAnims = gDummySpriteAffineAnimTable,
	.callback = SpriteCallbackDummy,
};

static const struct SpriteTemplate sMoveCanvasTemplate =
{
	.tileTag = 0x4736,
	.paletteTag = 0x8472,
	.oam = &sFontOAM,
	.anims = gDummySpriteAnimTable,
	.images = NULL,
	.affineAnims = gDummySpriteAffineAnimTable,
	.callback = SpriteCallbackDummy,
};

static const struct SpriteTemplate sAbilityCanvasTemplate =
{
	.tileTag = 0x1EE7,
	.paletteTag = 0x8472,
	.oam = &sFontOAM,
	.anims = gDummySpriteAnimTable,
	.images = NULL,
	.affineAnims = gDummySpriteAffineAnimTable,
	.callback = SpriteCallbackDummy,
};

static const struct SpriteTemplate sStarLitTemplate =
{
	.tileTag = 0x61,
	.paletteTag = 0x8472,
	.oam = &sHeldItemOam,
	.anims = gDummySpriteAnimTable,
	.images = NULL,
	.affineAnims = gDummySpriteAffineAnimTable,
	.callback = SpriteCallbackDummy,
};

static const struct SpriteTemplate sStarDullTemplate =
{
	.tileTag = 0x2613,
	.paletteTag = 0x8472,
	.oam = &sHeldItemOam,
	.anims = gDummySpriteAnimTable,
	.images = NULL,
	.affineAnims = gDummySpriteAffineAnimTable,
	.callback = SpriteCallbackDummy,
};

static const struct SpriteTemplate sHeldItemTemplate =
{
	.tileTag = 0x8472,
	.paletteTag = 0x8472,
	.oam = &sHeldItemOam,
	.anims = gDummySpriteAnimTable,
	.images = NULL,
	.affineAnims = gDummySpriteAffineAnimTable,
	.callback = SpriteCallbackDummy,
};

static const struct CompressedSpriteSheet sBlackBarTiles = {DexNavBarTiles, (64 * 32) / 2, 0xFDF1};
static const struct CompressedSpriteSheet sSightSpriteSheet = {DexNavSightTiles, (16 * 8 * 3) / 2, 0x5424};
static const struct CompressedSpriteSheet sBButtonSpriteSheet = {DexNavBButtonTiles, (32 * 8) / 2, 0x5425};
static const struct CompressedSpriteSheet sMoveCanvasSpriteSheet = {(u8*) gInterfaceGfx_emptyTiles, (64 * 32) / 2, 0x4736};
static const struct CompressedSpriteSheet sAbilityCanvasSpriteSheet = {gInterfaceGfx_emptyTiles, (64 * 32) / 2, 0x1EE7};
static const struct SpriteSheet sStarLitSpriteSheet = {&gInterfaceGfx_dexnavStarsTiles[19 * 4 * 32], (8 * 8) / 2, 0x61}; //19 tiles per row, stars are on the 4th row. 1 tile is 32 bytes. Hence 19 * 4 *32
static const struct SpriteSheet sStarDullSpriteSheet = {&gInterfaceGfx_dexnavStarsTiles[((19 * 4) + 1)*32], (8 * 8) / 2, 0x2613};
static const struct SpriteSheet sHeldItemSpriteSheet = {(const u8*) 0x845A3AC, (8 * 16) / 2, 0x8472};
static const struct SpritePalette sHeldItemSpritePalette = {(const u16*) 0x0845A3EC, 0x8472};
