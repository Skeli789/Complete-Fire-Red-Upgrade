#pragma once

#include "../global.h"

/**
 * \file dns_data.h
 * \brief A file to be included only by "src/dexnav.c". It contains declarations,
 *		  constants, and data structures used by the DexNav feature.
 */

//IMAGES
extern const u8 DexNavBGTiles[];
extern const u8 DexNavBGMap[];
extern const u16 DexNavBGPal[];
extern const u8 DexNavBGUnboundTiles[];
extern const u8 DexNavBGUnboundMap[];
extern const u16 DexNavBGUnboundPal[];
extern const u16 DexNavBG_VolcanoPal[];
extern const u16 DexNavBG_FlowerParadiseAPal[];
extern const u16 DexNavBG_FlowerParadiseBPal[];
extern const u16 DexNavBG_FlowerParadiseCPal[];
extern const u16 DexNavBG_AutumnPal[];
extern const u16 DexNavBG_WinterPal[];
extern const u16 DexNavBG_DesertPal[];
extern const u16 DexNavBG_SwampPal[];
extern const u16 DexNavBG_CavePal[];
extern const u16 DexNavBG_DarkerCavePal[];
extern const u16 DexNavBG_IndoorPal[];
extern const u8 gInterfaceGfx_dexnavStarsTiles[];
extern const u8 gInterfaceGfx_dexnavStarsPal[];
extern const u8 gInterfaceGfx_emptyTiles[];
extern const u8 gInterfaceGfx_emptyPal[];
extern const u8 gInterfaceGfx_DexNavRegisteredIconTiles[];
extern const u8 gInterfaceGfx_CapturedAllPokemonTiles[];
extern const u8 gInterfaceGfx_CapturedAllPokemonPal[];
extern const u8 gInterfaceGfx_DexNavNoDataSymbolTiles[];
extern const u8 DexNavBarTiles[];
extern const u8 DexNavSightTiles[];
extern const u8 DexNavBButtonTiles[];
extern const u8 DexNavHUDChainCanvasTiles[];

//STRINGS
extern const u8 gText_EmptyString[];
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
extern const u8 gText_DexNavReeds[];
extern const u8 gText_DexNavDeepWater[];
extern const u8 gText_PokeTools[];
extern const u8 gText_GotAway[];
extern const u8 gText_PokemonGotAway[];
extern const u8 gText_LostSignal[];
extern const u8 gText_GotAwayShouldSneak[];
extern const u8 gText_PokemonGotAwayShouldSneak[];
extern const u8 gText_CannotBeFound[];
extern const u8 gText_FormCannotBeFound[];
extern const u8 gText_NotFoundNearby[];
extern const u8 gText_TooDarkForDexNav[];
extern const u8 gText_DexNav_Chain[];
extern const u8 gText_DexNav_NoInfo[];
extern const u8 gText_DexNav_CaptureToSee[];
extern const u8 gText_DexNav_Unavailable[];
extern const u8 gText_DexNav_Walk[];
extern const u8 gText_DexNav_Surf[];
extern const u8 gText_DexNav_LavaSurf[];
extern const u8 gText_DexNav_Fish[];
extern const u8 gText_DexNav_OldRod[];
extern const u8 gText_DexNav_GoodRod[];
extern const u8 gText_DexNav_SuperRod[];
extern const u8 gText_DexNav_SurfOldRod[];
extern const u8 gText_DexNav_SurfGoodRod[];
extern const u8 gText_DexNav_SurfSuperRod[];
extern const u8 gText_DexNav_Swarm[];
extern const u8 gText_DexNav_ChooseMon[];
extern const u8 gText_DexNav_Invalid[];
extern const u8 gText_DexNav_NoDataForSlot[];
extern const u8 gText_DexNav_PokemonSelected[];
extern const u8 gText_DexNav_Locked[];
extern const u8 gText_DexNav_NeedRod[];
extern const u8 gText_DexNav_TooDark[];
extern const u8 gText_DexNav_NoPokemonHere[];
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

#define SCAN_SIZE_X_START 7
#define SCAN_SIZE_Y_START 5
#define SCAN_SIZE_X_REPEAT 4 //For moving in caves and water
#define SCAN_SIZE_Y_REPEAT 4 //For moving in caves and water

#define ICON_PAL_TAG 0xDAC0
#define ICON_GFX_TAG 0xD75A
#define SELECTION_CURSOR_TAG 0x200
#define CAPTURED_ALL_TAG 0xFDF2 //Tag is from Mega Evo and not in use
#define REGISTERED_ICON_TAG 0xFDF3

#define ICONX 0x10
#define ICONY 0x92

#define ICONX 0x10
#define ICONY 0x92

#define NUM_WATER_MONS 5
#define NUM_OLD_ROD_MONS 2
#define NUM_GOOD_ROD_MONS 3
#define NUM_SUPER_ROD_MONS 5
#define NUM_FISHING_MONS (NUM_OLD_ROD_MONS + NUM_GOOD_ROD_MONS + NUM_SUPER_ROD_MONS)

#define ROW_MON_LENGTH 1
#define WATER_ROW_LENGTH 6
#define LAND_ROW_LENGTH 6
#define LAND_ROW_COUNT 2
#define WATER_ROW_COUNT 3
#define MIN_WATER_ROW_COUNT 2
#define VISIBLE_LAND_ROW_COUNT 2
#define VISIBLE_WATER_ROW_COUNT 2
#define LAND_ROW_LAST_INDEX (LAND_ROW_LENGTH - 1)
#define WATER_ROW_LAST_INDEX (WATER_ROW_LENGTH - 1)
#define MAX_TOTAL_LAND_MONS (LAND_ROW_COUNT * LAND_ROW_LENGTH)
#define MAX_TOTAL_WATER_MONS (WATER_ROW_LENGTH * WATER_ROW_COUNT)

#define HIDDEN_SPEECIES_TERMIN 0xFEFE

extern u8 gMoveNames[][MOVE_NAME_LENGTH + 1];

//GUI Data
struct DexNavGUIData
{
	u16 grassSpecies[MAX_TOTAL_LAND_MONS];
	u16 waterSpecies[MAX_TOTAL_WATER_MONS];
	u16 hiddenSpecies[max(MAX_TOTAL_LAND_MONS, MAX_TOTAL_WATER_MONS) + 1];
	u8 hiddenLandEncounterMethod[MAX_TOTAL_LAND_MONS];
	u8 hiddenWaterEncounterMethod[MAX_TOTAL_LAND_MONS];
	u8 unownForms[MAX_TOTAL_LAND_MONS];
	u8 unownFormsByDNavIndices[MAX_TOTAL_LAND_MONS];
	u16 waterItemRequired[MAX_TOTAL_WATER_MONS];
	u8 landEncounterMethod[MAX_TOTAL_LAND_MONS];
	u8 waterEncounterMethod[MAX_TOTAL_WATER_MONS];
	u8 numLandMons;
	u8 numWaterMons;
	u8 numHiddenLandMons;
	u8 numHiddenWaterMons;
	u8 cursorSpriteId;
	u8 selectedIndex;
	u8 selectedArea;
	u8 registeredIndex;
	u8 registeredArea;
	bool8 registeredIconVisible;
	u16 landRowScroll;
	u16 waterRowScroll;
	u16 waterRowsAbove;
	u8* tilemapPtr;
};

enum DexNavMessages
{
	MESSAGE_POKEMON_SELECTED,
	MESSAGE_REGISTERED,
	MESSAGE_NEED_ROD,
	MESSAGE_INVALID,
	MESSAGE_NO_DATA,
	MESSAGE_TOO_DARK,
	MESSAGE_NO_POKEMON_HERE,
};

enum EncounterTypes
{
	ENCOUNTER_METHOD_GRASS,
	ENCOUNTER_METHOD_WATER,
	ENCOUNTER_METHOD_OLD_ROD,
	ENCOUNTER_METHOD_GOOD_ROD,
	ENCOUNTER_METHOD_SUPER_ROD,
	ENCOUNTER_METHOD_SURF_OLD_ROD,
	ENCOUNTER_METHOD_SURF_GOOD_ROD,
	ENCOUNTER_METHOD_SURF_SUPER_ROD,
	ENCOUNTER_METHOD_SWARM,
	ENCOUNTER_METHOD_COUNT,
};

enum BGs
{
	BG_TEXTBOX,
	BG_TEXT_2,
	BG_TEXT,
	BG_BACKGROUND,
};

enum
{
	WIN_TEXTBOX,
	WIN_CONTEXT_MENU,
	WIN_SPECIES,
	WIN_SEARCH_LEVEL,
	WIN_METHOD,
	WIN_HIDDEN_ABILITY,
	WIN_HELD_ITEMS,
	WIN_MON_TYPE_1,
	WIN_MON_TYPE_2,
	WIN_WATER,
	WIN_LAND,
	WIN_MAP_NAME,
	WIN_CHAIN_LENGTH,
	WINDOW_COUNT,
};

static const struct WindowTemplate sDexNavWinTemplates[WINDOW_COUNT + 1] =
{
	[WIN_TEXTBOX] =
	{
		.bg = BG_TEXTBOX,
		.tilemapLeft = 1,
		.tilemapTop = 15,
		.width = 28,
		.height = 4,
		.paletteNum = 14,
		.baseBlock = 1,
	},
	[WIN_CONTEXT_MENU] =
	{
		.bg = BG_TEXTBOX,
		.tilemapLeft = 21,
		.tilemapTop = 7,
		.width = 7,
		.height = 6,
		.paletteNum = 14,
		.baseBlock = 113,
	},
	[WIN_SPECIES] =
	{
		.bg = BG_TEXT_2, //Above so it doesn't conflict with the type icons
		.tilemapLeft = 20,
		.tilemapTop = 2,
		.width = 9,
		.height = 3,
		.paletteNum = 15,
		.baseBlock = 155,
	},
	[WIN_SEARCH_LEVEL] =
	{
		.bg = BG_TEXT,
		.tilemapLeft = 20,
		.tilemapTop = 7,
		.width = 9,
		.height = 2,
		.paletteNum = 15,
		.baseBlock = 182,
	},
	[WIN_METHOD] =
	{
		.bg = BG_TEXT,
		.tilemapLeft = 20,
		.tilemapTop = 10,
		.width = 10,
		.height = 2,
		.paletteNum = 15,
		.baseBlock = 200,
	},
	[WIN_HIDDEN_ABILITY] =
	{
		.bg = BG_TEXT,
		.tilemapLeft = 20,
		.tilemapTop = 13,
		.width = 10,
		.height = 2,
		.paletteNum = 15,
		.baseBlock = 220,
	},
	[WIN_HELD_ITEMS] =
	{
		.bg = BG_TEXT,
		.tilemapLeft = 20,
		.tilemapTop = 16,
		.width = 10,
		.height = 4,
		.paletteNum = 15,
		.baseBlock = 240,
	},
	[WIN_MON_TYPE_1] =
	{
		.bg = BG_TEXT,
		.tilemapLeft = 20,
		.tilemapTop = 4,
		.width = 5,
		.height = 2,
		.paletteNum = 12,
		.baseBlock = 280,
	},
	[WIN_MON_TYPE_2] =
	{
		.bg = BG_TEXT,
		.tilemapLeft = 25,
		.tilemapTop = 4,
		.width = 5,
		.height = 2,
		.paletteNum = 12,
		.baseBlock = 290,
	},
	[WIN_WATER] =
	{
		.bg = BG_TEXT,
		.tilemapLeft = 0,
		.tilemapTop = 2,
		.width = 19,
		.height = 3,
		.paletteNum = 15,
		.baseBlock = 300,
	},
	[WIN_LAND] =
	{
		.bg = BG_TEXT,
		.tilemapLeft = 0,
	#ifdef UNBOUND
		.tilemapTop = 10,
	#else
		.tilemapTop = 11,
	#endif
		.width = 19,
		.height = 3,
		.paletteNum = 15,
		.baseBlock = 357,
	},
	[WIN_MAP_NAME] =
	{
		.bg = BG_TEXT,
		.tilemapLeft = 0,
		.tilemapTop = 0,
		.width = 12,
		.height = 3,
		.paletteNum = 15,
		.baseBlock = 414,
	},
	[WIN_CHAIN_LENGTH] =
	{
		.bg = BG_TEXT,
		.tilemapLeft = 22,
		.tilemapTop = 0,
		.width = 8,
		.height = 2,
		.paletteNum = 15,
		.baseBlock = 450,
	},
	//Base block 500 is used for frame tiles
	DUMMY_WIN_TEMPLATE
};

static const struct BgTemplate sDexNavBgTemplates[] =
{
	[BG_TEXTBOX] =
	{
		.bg = BG_TEXTBOX,
		.charBaseIndex = 0,
		.mapBaseIndex = 31,
		.screenSize = 0,
		.paletteMode = 0,
		.priority = 0,
		.baseTile = 0,
	},
	[BG_TEXT_2] =
	{
		.bg = BG_TEXT_2,
		.charBaseIndex = 1,
		.mapBaseIndex = 30,
		.screenSize = 0,
		.paletteMode = 0,
		.priority = 1,
		.baseTile = 0,
	},
	[BG_TEXT] =
	{
		.bg = BG_TEXT,
		.charBaseIndex = 2,
		.mapBaseIndex = 29,
		.screenSize = 0,
		.paletteMode = 0,
		.priority = 2,
		.baseTile = 0,
	},
	[BG_BACKGROUND] =
	{
		.bg = BG_BACKGROUND,
		.charBaseIndex = 3,
		.mapBaseIndex = 28,
		.screenSize = 0,
		.paletteMode = 0,
		.priority = 3,
		.baseTile = 0,
	},
};

static const struct ListMenuItem sContextMenuListItems[] =
{
	{gText_DexNav_ListMenuRegister, 0},
	{gText_DexNav_ListMenuScan, 1},
	{gText_DexNav_ListMenuCancel, 2},
};

static const struct ListMenuTemplate sContextMenuTemplate =
{
	.items = sContextMenuListItems,
	.totalItems = 3, //Register, Scan, Close
	.moveCursorFunc = ContextMenuMoveCursorFunc,
	.itemPrintFunc = ContextMenuItemPrintFunc,

	.windowId = WIN_CONTEXT_MENU,
	.header_X = 0,
	.item_X = 8, //Text is 8 px from left side of tile
	.cursor_X = 0, //Cursor is 0 px from left side of tile
	.lettersSpacing = 0,
	.itemVerticalPadding = 2, //2 px between items
	.upText_Y = 3, //3 px from top of tile
	.maxShowed = 3, //3 at a time
	.fontId = 2, //Normal size
	.cursorPal = 2, //Grey
	.fillValue = 1,
	.cursorShadowPal = 3, //Light Grey
	.cursorKind = 0,
	.scrollMultiple = LIST_NO_MULTIPLE_SCROLL,
};

static const struct OamData sCursorOam =
{
	.affineMode = ST_OAM_AFFINE_OFF,
	.objMode = ST_OAM_OBJ_NORMAL,
	.shape = SPRITE_SHAPE(32x32),
	.size = SPRITE_SIZE(32x32),
	.priority = 1, //Above other sprites
};

static const struct OamData sRegisteredIconOam =
{
	.affineMode = ST_OAM_AFFINE_OFF,
	.objMode = ST_OAM_OBJ_NORMAL,
	.shape = SPRITE_SHAPE(16x16),
	.size = SPRITE_SIZE(16x16),
	.priority = 1, //Above other sprites
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

static const union AnimCmd sAnimCmdHandCursorPointed[] =
{
	ANIMCMD_FRAME(16, 30),
	ANIMCMD_END
};

static const union AnimCmd *const sAnimCmdTable_HandCursor[] =
{
	sAnimCmdHandCursor,
	sAnimCmdHandCursorPointed,
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

static void SpriteCB_GUIRegisteredIcon(struct Sprite* sprite);
static const struct SpriteTemplate sRegisteredIconTemplate =
{
	.tileTag = REGISTERED_ICON_TAG,
	.paletteTag = SELECTION_CURSOR_TAG,
	.oam = &sRegisteredIconOam,
	.anims = gDummySpriteAnimTable,
	.images = NULL,
	.affineAnims = gDummySpriteAffineAnimTable,
	.callback = SpriteCB_GUIRegisteredIcon,
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
static const struct CompressedSpriteSheet sRegisteredIconSpriteSheet = {gInterfaceGfx_DexNavRegisteredIconTiles, (16 * 16) / 2, REGISTERED_ICON_TAG};
static const struct CompressedSpriteSheet sCapturedAllPokemonSpriteSheet = {gInterfaceGfx_CapturedAllPokemonTiles, (8 * 8) / 2, CAPTURED_ALL_TAG};
static const struct CompressedSpritePalette sCapturedAllPokemonSpritePalette = {gInterfaceGfx_CapturedAllPokemonPal, CAPTURED_ALL_TAG};
static const struct CompressedSpriteSheet sNoDataIconSpriteSheet = {gInterfaceGfx_DexNavNoDataSymbolTiles, (32 * 32) / 2, ICON_GFX_TAG};

static const struct TextColor sWhiteText =
{
	.bgColor = TEXT_COLOR_TRANSPARENT,
	.fgColor = TEXT_COLOR_WHITE,
	.shadowColor = TEXT_COLOR_DARK_GREY,
};

static const struct TextColor sBlackText =
{
	.bgColor = TEXT_COLOR_TRANSPARENT,
	.fgColor = TEXT_COLOR_DARK_GREY,
	.shadowColor = TEXT_COLOR_LIGHT_GREY,
};

static const struct TextColor sLightRedText =
{
	.bgColor = TEXT_COLOR_TRANSPARENT,
	.fgColor = TEXT_COLOR_LIGHT_RED,
	.shadowColor = TEXT_COLOR_RED,
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
	u8 elevation;
	u8 unownLetter;
	s16 tileX; // position of shaking grass
	s16 tileY;
	u8 spriteIdSpecies;
	u8 spriteIdSight;
	u8 spriteIdBButton;
	u8 spriteIdChainNumber;
	u8 spriteIdChainStar;
	u8 spriteIdAbility;
	u8 spriteIdMove;
	u8 spriteIdItem;
	u8 spriteIdShakingGrass;
	u8 spriteIdPotential[3];
	u8 blackBarWindowId;
	u8 arrowWindowId;
	u8 movementTimes;
	
	s16 scanLeftX;
	s16 scanTopY;
	bool8 reachableTiles[16][16];
	bool8 impassibleTiles[16][16];
	s16 scanRightX;
	s16 scanBottomY;
};

enum FieldMessages
{
	FIELD_MSG_NOT_IN_AREA, 
	FIELD_MSG_LOOK_IN_OTHER_SPOT,
	FIELD_MSG_TOO_DARK,
	FIELD_MSG_GOT_AWAY,
	FIELD_MSG_GOT_AWAY_DETECTOR,
	FIELD_MSG_LOST_SIGNAL,
	FIELD_MSG_SNEAK_NEXT_TIME,
	FIELD_MSG_SNEAK_NEXT_TIME_DETECTOR,
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
	.oam = &sBButtonOam,
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
static const struct CompressedSpriteSheet sChainNumberCanvasSpriteSheet = {DexNavHUDChainCanvasTiles, (32 * 8) / 2, 0x1EE8};

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

static bool8 SpeciesHasMultipleSearchableForms(u16 species)
{
	u16 dexNum = SpeciesToNationalPokedexNum(species);
	
	switch (dexNum)
	{
		case NATIONAL_DEX_RATTATA:
		case NATIONAL_DEX_RATICATE:
		case NATIONAL_DEX_PIKACHU:
		case NATIONAL_DEX_RAICHU:
		case NATIONAL_DEX_SANDSHREW:
		case NATIONAL_DEX_SANDSLASH:
		case NATIONAL_DEX_VULPIX:
		case NATIONAL_DEX_NINETALES:
		case NATIONAL_DEX_DIGLETT:
		case NATIONAL_DEX_DUGTRIO:
		case NATIONAL_DEX_MEOWTH:
		case NATIONAL_DEX_PERSIAN:
		case NATIONAL_DEX_GROWLITHE:
		case NATIONAL_DEX_ARCANINE:
		case NATIONAL_DEX_GEODUDE:
		case NATIONAL_DEX_GRAVELER:
		case NATIONAL_DEX_GOLEM:
		case NATIONAL_DEX_PONYTA:
		case NATIONAL_DEX_RAPIDASH:
		case NATIONAL_DEX_SLOWPOKE:
		case NATIONAL_DEX_SLOWBRO:
		case NATIONAL_DEX_FARFETCHD:
		case NATIONAL_DEX_GRIMER:
		case NATIONAL_DEX_MUK:
		case NATIONAL_DEX_VOLTORB:
		case NATIONAL_DEX_ELECTRODE:
		case NATIONAL_DEX_EXEGGUTOR:
		case NATIONAL_DEX_MAROWAK:
		case NATIONAL_DEX_WEEZING:
		case NATIONAL_DEX_MR_MIME:
		case NATIONAL_DEX_ARTICUNO:
		case NATIONAL_DEX_ZAPDOS:
		case NATIONAL_DEX_MOLTRES:
		case NATIONAL_DEX_TYPHLOSION:
		case NATIONAL_DEX_CORSOLA:
		case NATIONAL_DEX_SLOWKING:
		case NATIONAL_DEX_UNOWN:
		case NATIONAL_DEX_QWILFISH:
		case NATIONAL_DEX_SNEASEL:
		case NATIONAL_DEX_ZIGZAGOON:
		case NATIONAL_DEX_LINOONE:
		case NATIONAL_DEX_DEOXYS:
		case NATIONAL_DEX_BURMY:
		case NATIONAL_DEX_WORMADAM:
		case NATIONAL_DEX_SHELLOS:
		case NATIONAL_DEX_GASTRODON:
		case NATIONAL_DEX_SAMUROTT:
		case NATIONAL_DEX_DARUMAKA:
		case NATIONAL_DEX_DARMANITAN:
		case NATIONAL_DEX_LILLIGANT:
		case NATIONAL_DEX_YAMASK:
		case NATIONAL_DEX_BASCULIN:
		case NATIONAL_DEX_ZORUA:
		case NATIONAL_DEX_ZOROARK:
		case NATIONAL_DEX_DEERLING:
		case NATIONAL_DEX_SAWSBUCK:
		case NATIONAL_DEX_STUNFISK:
		case NATIONAL_DEX_BRAVIARY:
		case NATIONAL_DEX_VIVILLON:
		case NATIONAL_DEX_FLABEBE:
		case NATIONAL_DEX_FLOETTE:
		case NATIONAL_DEX_SLIGGOO:
		case NATIONAL_DEX_GOODRA:
		case NATIONAL_DEX_AVALUGG:
		case NATIONAL_DEX_PUMPKABOO:
		case NATIONAL_DEX_GOURGEIST:
		case NATIONAL_DEX_DECIDUEYE:
		case NATIONAL_DEX_ORICORIO:
		case NATIONAL_DEX_LYCANROC:
		case NATIONAL_DEX_TOXTRICITY:
		case NATIONAL_DEX_SINISTEA:
		case NATIONAL_DEX_POLTEAGEIST:
		case NATIONAL_DEX_ALCREMIE:
		case NATIONAL_DEX_URSHIFU:
		case NATIONAL_DEX_BASCULEGION:
			return TRUE;
		default:
			return FALSE;
	}
}
