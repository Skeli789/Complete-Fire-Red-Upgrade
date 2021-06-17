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
extern const u8 gInterfaceGfx_emptyTiles[];
extern const u8 gInterfaceGfx_emptyPal[];
extern const u8 gInterfaceGfx_CapturedAllPokemonTiles[];
extern const u8 gInterfaceGfx_CapturedAllPokemonPal[];
extern const u8 gInterfaceGfx_DexNavNoDataSymbolTiles[];
extern const u8 DexNavBarTiles[];
extern const u8 DexNavSightTiles[];
extern const u8 DexNavBButtonTiles[];
extern const u8 DexNavHUDChainCanvasTiles[];

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
extern const u8 gText_DexNav_PokemonSelected[];
extern const u8 gText_DexNav_Locked[];
extern const u8 gText_DexNav_TooDark[];
extern const u8 gText_DexNav_UpArrow[];
extern const u8 gText_DexNav_DownArrow[];
extern const u8 gText_DexNav_LeftArrow[];
extern const u8 gText_DexNav_RightArrow[];
extern const u8 gText_DexNav_ListMenuRegister[];
extern const u8 gText_DexNav_ListMenuScan[];
extern const u8 gText_DexNav_ListMenuCancel[];
extern const u8 gText_DexNavHUDChainNumber[];
	
// DEFINES
#define TILE_SIZE 32
#define SPRITE_RAM 0x6010000

#define TOOL_COUNT 2
#define priv0 gTasks[taskId].data[0]

#define CPUFSCPY 0
#define CPUFSSET 1
#define CPUModeFS(size, mode) ((size >> 2) | (mode << 24))

#define SCANSTART_X 7
#define SCANSTART_Y 5
#define SCANSIZE_X 12
#define SCANSIZE_Y 12

#define ICON_PAL_TAG 0xDAC0
#define ICON_GFX_TAG 0xD75A
#define SELECTION_CURSOR_TAG 0x200
#define CAPTURED_ALL_TAG 0xFDF2 //Tag is from Mega Evo and not in use

#define ICONX 0x10
#define ICONY 0x92

#define ICONX 0x10
#define ICONY 0x92

#define NUM_LAND_MONS 12
#define NUM_WATER_MONS 5
#define NUM_OLD_ROD_MONS 2
#define NUM_GOOD_ROD_MONS 3
#define NUM_SUPER_ROD_MONS 5
#define NUM_FISHING_MONS (NUM_OLD_ROD_MONS + NUM_GOOD_ROD_MONS + NUM_SUPER_ROD_MONS)
#define NUM_TOTAL_WATER_MONS (NUM_WATER_MONS + NUM_FISHING_MONS)

#define LAND_ROW_LENGTH (6 * 2)
#define LAND_FIRST_ROW_LAST_INDEX (5 * 2)
#define LAND_SECOND_ROW_LAST_INDEX (LAND_FIRST_ROW_LAST_INDEX + LAND_ROW_LENGTH)
#define LAND_SECOND_ROW_FIRST_INDEX (6 * 2)
#define WATER_ROW_LAST_INDEX (4 * 2)
#define ROW_MON_LENGTH 2
#define WATER_MONS_PER_PAGE 5

extern u8 gMoveNames[][MOVE_NAME_LENGTH + 1];

//GUI Data
struct DexNavGuiData
{
    u16 grassSpecies[NUM_LAND_MONS];
    u16 waterSpecies[NUM_TOTAL_WATER_MONS];
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
	s8 waterPage;
	u16 waterScrollArrowDummy;
	u8* tilemapPtr;
};

enum DexNavMessages
{
	MESSAGE_INVALID,
	MESSAGE_CHOOSE_MON,
	MESSAGE_POKEMON_SELECTED,
	MESSAGE_REGISTERED,
	MESSAGE_NO_DATA,
	MESSAGE_TOO_DARK,
};

enum BGs
{
	BG_CONTEXT_MENU,
	BG_UNUSED,
	BG_TEXT,
	BG_BACKGROUND,
};

enum
{
	WIN_SPECIES,
	WIN_SEARCH_LEVEL,
	WIN_CHAIN_LENGTH,
	WIN_HIDDEN_ABILITY,
	WIN_MESSAGE,
	WIN_WATER,
	WIN_LAND,
	WIN_MAP_NAME,
	WIN_INSTRUCTIONS,
	WIN_CONTEXT_MENU,
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
	[WIN_CHAIN_LENGTH] =
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
	[WIN_CONTEXT_MENU] =
	{
        .bg = BG_CONTEXT_MENU,
        .tilemapLeft = 22,
        .tilemapTop = 13,
        .width = 7,
        .height = 6,
        .paletteNum = 14,
        .baseBlock = 384,
	},
	//Base block 500 is used for frame tiles
    DUMMY_WIN_TEMPLATE
};

static const struct BgTemplate sDexNavBgTemplates[] =
{
    [BG_CONTEXT_MENU] =
	{
        .bg = 0,
        .charBaseIndex = 0,
        .mapBaseIndex = 31,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0,
    },
	[BG_UNUSED] =
    {
        .bg = 1,
        .charBaseIndex = 1,
        .mapBaseIndex = 30,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 1,
        .baseTile = 0,
    },
	[BG_TEXT] =
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

static const union AnimCmd sAnimCmdHandCursor[] =
{
	ANIMCMD_FRAME(0, 30),
	ANIMCMD_FRAME(16, 30),
	ANIMCMD_JUMP(0)
};

static const union AnimCmd *const sAnimCmdTable_HandCursor[] =
{
	sAnimCmdHandCursor,
};

static void SpriteCB_GUICursor(struct Sprite* sprite);
static const struct SpriteTemplate sGUICursorTemplate =
{
	.tileTag = SELECTION_CURSOR_TAG,
	.paletteTag = SELECTION_CURSOR_TAG,
	.oam = &sCursorOam,
	.anims = sAnimCmdTable_HandCursor,
	.images = NULL,
	.affineAnims = gDummySpriteAffineAnimTable,
	.callback = SpriteCB_GUICursor,
};

static const struct SpriteTemplate sCapturedAllPokemonSymbolTemplate =
{
	.tileTag = CAPTURED_ALL_TAG,
	.paletteTag = CAPTURED_ALL_TAG,
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

static const struct SpriteSheet sCursorSpriteSheet = {(void*) 0x83D2BEC, (32 * 32 * 4) / 2, SELECTION_CURSOR_TAG};
static const struct SpritePalette sCursorSpritePalette = {(void*) 0x83CE7F0, SELECTION_CURSOR_TAG};
static const struct CompressedSpriteSheet sCapturedAllPokemonSpriteSheet = {gInterfaceGfx_CapturedAllPokemonTiles, (8 * 8) / 2, CAPTURED_ALL_TAG};
static const struct CompressedSpritePalette sCapturedAllPokemonSpritePalette = {gInterfaceGfx_CapturedAllPokemonPal, CAPTURED_ALL_TAG};
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
    u8 xProximity;
    u8 yProximity;
    u8 totalProximity;
    u8 environment;
	u8 unownLetter;
    s16 tileX; // position of shaking grass
    s16 tileY;
    u8 spriteIdSpecies;
    u8 spriteIdSight;
    u8 spriteIdBButton;
	u8 spriteIdChainNumber;
    u8 spriteIdAbility;
    u8 spriteIdMove;
    u8 spriteIdItem;
    u8 spriteIdShakingGrass;
    u8 spriteIdPotential[3];
    u8 blackBarWindowId;
	u8 arrowWindowId;
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

//32x16 oam with highest priority
static const struct OamData sSmallerFontOAM =
{
	.affineMode = ST_OAM_AFFINE_OFF,
	.objMode = ST_OAM_OBJ_NORMAL,
	.shape = SPRITE_SHAPE(32x16),
	.size = SPRITE_SIZE(32x16),
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

static const struct SpriteTemplate sSightTemplate =
{
	.tileTag = 0x5424,
	.paletteTag = GFX_TAG_HELD_ITEM,
	.oam = &sSightOam,
	.anims = sAnimCmdTable_Sight,
	.images = NULL,
	.affineAnims = gDummySpriteAffineAnimTable,
	.callback = SpriteCallbackDummy,
};

static const struct SpriteTemplate sBButtonTemplate =
{
	.tileTag = 0x5425,
	.paletteTag = GFX_TAG_HELD_ITEM,
	.oam = &sBButtonOam,
	.anims = gDummySpriteAnimTable,
	.images = NULL,
	.affineAnims = gDummySpriteAffineAnimTable,
	.callback = SpriteCallbackDummy,
};

static const struct SpriteTemplate sMoveCanvasTemplate =
{
	.tileTag = 0x4736,
	.paletteTag = GFX_TAG_HELD_ITEM,
	.oam = &sFontOAM,
	.anims = gDummySpriteAnimTable,
	.images = NULL,
	.affineAnims = gDummySpriteAffineAnimTable,
	.callback = SpriteCallbackDummy,
};

static const struct SpriteTemplate sAbilityCanvasTemplate =
{
	.tileTag = 0x1EE7,
	.paletteTag = GFX_TAG_HELD_ITEM,
	.oam = &sFontOAM,
	.anims = gDummySpriteAnimTable,
	.images = NULL,
	.affineAnims = gDummySpriteAffineAnimTable,
	.callback = SpriteCallbackDummy,
};

static const struct SpriteTemplate sChainNumberCanvasTemplate =
{
	.tileTag = 0x1EE8,
	.paletteTag = GFX_TAG_HELD_ITEM,
	.oam = &sSmallerFontOAM,
	.anims = gDummySpriteAnimTable,
	.images = NULL,
	.affineAnims = gDummySpriteAffineAnimTable,
	.callback = SpriteCallbackDummy,
};

static const struct SpriteTemplate sStarLitTemplate =
{
	.tileTag = 0x61,
	.paletteTag = GFX_TAG_HELD_ITEM,
	.oam = &sHeldItemOam,
	.anims = gDummySpriteAnimTable,
	.images = NULL,
	.affineAnims = gDummySpriteAffineAnimTable,
	.callback = SpriteCallbackDummy,
};

static const struct SpriteTemplate sStarDullTemplate =
{
	.tileTag = 0x2613,
	.paletteTag = GFX_TAG_HELD_ITEM,
	.oam = &sHeldItemOam,
	.anims = gDummySpriteAnimTable,
	.images = NULL,
	.affineAnims = gDummySpriteAffineAnimTable,
	.callback = SpriteCallbackDummy,
};

const struct SpriteTemplate gHeldItemTemplate =
{
	.tileTag = GFX_TAG_HELD_ITEM,
	.paletteTag = GFX_TAG_HELD_ITEM,
	.oam = &sHeldItemOam,
	.anims = gDummySpriteAnimTable,
	.images = NULL,
	.affineAnims = gDummySpriteAffineAnimTable,
	.callback = SpriteCallbackDummy,
};

static const struct CompressedSpriteSheet sSightSpriteSheet = {DexNavSightTiles, (16 * 8 * 3) / 2, 0x5424};
static const struct CompressedSpriteSheet sBButtonSpriteSheet = {DexNavBButtonTiles, (32 * 8) / 2, 0x5425};
static const struct CompressedSpriteSheet sMoveCanvasSpriteSheet = {(u8*) gInterfaceGfx_emptyTiles, (64 * 32) / 2, 0x4736};
static const struct CompressedSpriteSheet sAbilityCanvasSpriteSheet = {gInterfaceGfx_emptyTiles, (64 * 32) / 2, 0x1EE7};
static const struct CompressedSpriteSheet sChainNumberCanvasSpriteSheet = {DexNavHUDChainCanvasTiles, (32 * 16) / 2, 0x1EE8};

#ifdef UNBOUND
extern const u8 DexNavStarTiles[];
static const struct SpriteSheet sStarLitSpriteSheet = {DexNavStarTiles, (8 * 8) / 2, 0x61}; //1st Tile
static const struct SpriteSheet sStarDullSpriteSheet = {&DexNavStarTiles[1 * 32], (8 * 8) / 2, 0x2613}; //Second tile
#else
static const struct SpriteSheet sStarLitSpriteSheet = {&gInterfaceGfx_dexnavStarsTiles[19 * 4 * 32], (8 * 8) / 2, 0x61}; //19 tiles per row, stars are on the 4th row. 1 tile is 32 bytes. Hence 19 * 4 *32
static const struct SpriteSheet sStarDullSpriteSheet = {&gInterfaceGfx_dexnavStarsTiles[((19 * 4) + 1)*32], (8 * 8) / 2, 0x2613};
#endif
const struct SpriteSheet gHeldItemSpriteSheet = {(const u8*) 0x845A3AC, (8 * 16) / 2, GFX_TAG_HELD_ITEM};
const struct SpritePalette gHeldItemSpritePalette = {(const u16*) 0x0845A3EC, GFX_TAG_HELD_ITEM};

static const struct ListMenuItem sContextMenuListItems[] =
{
	{gText_DexNav_ListMenuRegister, 0},
	{gText_DexNav_ListMenuScan, 1},
	{gText_DexNav_ListMenuCancel, 2},
};
