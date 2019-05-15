#pragma once

#include "../../src/defines.h"


// IMAGES
extern const u8 gInterfaceGfx_dexnavGuiTiles[];
extern const u8 gInterfaceGfx_dexnavGuiMap[];
extern const u8 gInterfaceGfx_dexnavGuiPal[];
extern const u8 gInterfaceGfx_dexnavStarsTiles[];
extern const u8 gInterfaceGfx_dexnavStarsPal[];
extern const u8 gInterfaceGfx_selectionCursorTiles[];
extern const u8 gInterfaceGfx_selectionCursorPal[];
extern const u8 gInterfaceGfx_emptyTiles[];
extern const u8 gInterfaceGfx_emptyPal[];
extern const u8 gInterfaceGfx_caveSmokeTiles[];
extern const u8 gInterfaceGfx_caveSmokePal[];

// TYPEDEFS
typedef void (*SuperCallback)(void);
typedef void (*SpriteCallback)(struct Sprite* s);

// DEFINES
#define TILE_SIZE 32
#define SPRITE_RAM 0x6010000

#define OEI_GRASS 0x13
#define OEI_WATER 0x16
#define OEI_CAVE 0x1A

#define EGGMOVE_MAX 8

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

// 60 frames per second. 30 seconds is the time out. Max of 1092 seconds allowed
#define DEXNAV_TIMEOUT 60 * 500

// chance of encountering egg move at search levels
#define SEARCHLEVEL0_MOVECHANCE 21
#define SEARCHLEVEL5_MOVECHANCE 46
#define SEARCHLEVEL10_MOVECHANCE 58
#define SEARCHLEVEL25_MOVECHANCE 63
#define SEARCHLEVEL50_MOVECHANCE 65
#define SEARCHLEVEL100_MOVECHANCE 83

// chance of encountering hidden abilities at search levels
#define SEARCHLEVEL0_ABILITYCHANCE 0
#define SEARCHLEVEL5_ABILITYCHANCE 0
#define SEARCHLEVEL10_ABILITYCHANCE 5
#define SEARCHLEVEL25_ABILITYCHANCE 15
#define SEARCHLEVEL50_ABILITYCHANCE 20
#define SEARCHLEVEL100_ABILITYCHANCE 23

// chance of encountering one star potential
#define SEARCHLEVEL0_ONESTAR 0
#define SEARCHLEVEL5_ONESTAR 14
#define SEARCHLEVEL10_ONESTAR 17
#define SEARCHLEVEL25_ONESTAR 17
#define SEARCHLEVEL50_ONESTAR 15
#define SEARCHLEVEL100_ONESTAR 8

// chance of encountering two star potential
#define SEARCHLEVEL0_TWOSTAR 0
#define SEARCHLEVEL5_TWOSTAR 1
#define SEARCHLEVEL10_TWOSTAR 9
#define SEARCHLEVEL25_TWOSTAR 16
#define SEARCHLEVEL50_TWOSTAR 17
#define SEARCHLEVEL100_TWOSTAR 24

// chance of encountering three star potential
#define SEARCHLEVEL0_THREESTAR 0
#define SEARCHLEVEL5_THREESTAR 0
#define SEARCHLEVEL10_THREESTAR 1
#define SEARCHLEVEL25_THREESTAR 7
#define SEARCHLEVEL50_THREESTAR 6
#define SEARCHLEVEL100_THREESTAR 12

// chance of encountering held item
#define SEARCHLEVEL0_ITEM 0
#define SEARCHLEVEL5_ITEM 0
#define SEARCHLEVEL10_ITEM 1
#define SEARCHLEVEL25_ITEM 7
#define SEARCHLEVEL50_ITEM 6
#define SEARCHLEVEL100_ITEM 12

struct FieldEffectArguments {
    struct Coords32 effectPos;
    u32 priority;
};

#define gFieldEffectArguments ((struct FieldEffectArguments*) 0x20386E0)


extern const u16 DexNavTextPal[];
extern const struct OamData IconOAM;
extern const struct OamData CursorOAM;
extern const struct BgTemplate BgConfigDexNavGUI[4];
extern const u16 CursorPositions2[];
extern const u16 CursorPositions1[];


// STRUCTS
static u8* SearchLevels = (u8*)DEXNAV_SAVERAM;
struct TextColor MenuTextBlack = {0, 2, 3};
struct WindowTemplate Tbox = {0, 2, 2, 10, TOOL_COUNT << 1, 0xF, 0x130};

struct TextColor DexNav_BlackText = {0, 3, 4};
struct TextColor DexNav_WhiteText = {0, 1, 2};
struct TextColor DexNav_RedText = {0, 7, 8};
struct TextColor DexNav_GreenText = {0, 5, 6};


const u8* gIconPals[] = {
	(void*) 0x83d3740, 
	(void*) 0x83d3760,
	(void*) 0x83d3780,
}; 

struct DexnavHudData {
    u16 species;
    u16 moveId[4];
    u16 heldItem;
    u8 ability;
    u8 potential;
    u8 searchLevel;
    u8 pokemonLevel;
    u8 moveNameLength;
    u8 proximity;
    u8 environment;
    s16 tileX; // position of shaking grass
    s16 tileY;
    u8 objIdSpecies;
    u8 objIdSight;
    u8 objIdAbility;
    u8 objIdMove;
    u8 objIdItem;
    u8 objIdShakingGrass;
    u8 objIdPotential[3];
    u8 movementTimes;

    // GUI data
    u16 grassSpecies[12];
    u16 waterSpecies[5];
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

extern const struct BgTemplate BgConfigDexNavGUI[4];
const struct BgTemplate BgConfigDexNavGUI[4] = {
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


const struct OamData IconOAM = {
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

const struct OamData CursorOAM = {
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
const u16 CursorPositions2[] = {
    30 + 24 * 0, 48,
    30 + 24 * 1, 48,
    30 + 24 * 2, 48,
    30 + 24 * 3, 48,
    30 + 24 * 4, 48,
};

// positions for grass
const u16 CursorPositions1[] = {
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
const u16 DexNavTextPal[] = {
	rgb5(255, 0, 255), rgb5(248, 248, 248), rgb5(112, 112, 112), rgb5(96, 96, 96),
	rgb5(208, 208, 208), rgb5(76, 154, 38), rgb5(102, 194, 66), rgb5(168, 75, 76),
	rgb5(224, 114, 75), rgb5(180, 124, 41), rgb5(241, 188, 60), rgb5(255, 0, 255),
	rgb5(255, 0, 255), rgb5(255, 0, 255), rgb5(255, 133, 200), rgb5(64, 200, 248)
};

//extern const struct WindowTemplate sDexNavWindows[];
const struct WindowTemplate sDexNavWindows[] = {
	{
        .bg = 0,		//Species 
        .tilemapLeft = 21,
        .tilemapTop = 6,
        .width = 9,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = 1,
	},
    {
        .bg = 0,		//Search level
        .tilemapLeft = 21,
        .tilemapTop = 9,
        .width = 9,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = 31,
    },
    {
        .bg = 0,		//Level bonus
        .tilemapLeft = 21,
        .tilemapTop = 12,
        .width = 3,
        .height = 3,
        .paletteNum = 15,
        .baseBlock = 61,
    },
    {
        .bg = 0,		//Hidden Ability
        .tilemapLeft = 21,
        .tilemapTop = 15,
        .width = 12,
        .height = 3,
        .paletteNum = 15,
        .baseBlock = 76,
    },
    {
        .bg = 0,		// Reply text
        .tilemapLeft = 1,
        .tilemapTop = 17,
        .width = 22,
        .height = 3,
        .paletteNum = 15,
        .baseBlock = 136,
    },
    {
        .bg = 0xFF, // marks the end of the tb array
    },

};


//const struct Frame (**nullframe)[] = (const struct Frame (**)[])0x8231CF0;
//const struct AffineAnimCmd (**nullrsf)[] = (const struct AffineAnimCmd (**)[])0x8231CFC;

// 32x64 oam with highest priority
const struct OamData FontOAM = {    .y = ICONY,
                                    .affineMode = 0,
                                    .objMode = 0,
                                    .mosaic = 0,
                                    .bpp = 0,
                                    .shape = 1,
                                    .x = ICONX,
                                    .matrixNum = 0,
                                    .size = 3,
                                    .tileNum = 0,
                                    .priority = 0,
                                    .paletteNum = 0,
                                    .affineParam = 0
};


// 8x8 oam with highest priority
const struct OamData HeldOAM = {   .y = ICONY,
                                    .affineMode = 0,
                                    .objMode = 0,
                                    .mosaic = 0,
                                    .bpp = 0,
                                    .shape = 0,
                                    .x = ICONX,
                                    .matrixNum = 0,
                                    .size = 0,
                                    .tileNum = 0,
                                    .priority = 0,
                                    .paletteNum = 0,
                                    .affineParam = 0
};

//#define gPalHeldItemIcon ((u32*) 0x0845A3EC) //Pal for held items. The yellow box and mail icon
const struct SpritePalette HeldPal = {(void*) 0x0845A3EC, 0x8472};

// 32x32 object with priority 1, one less than held item which overlaps it
const struct OamData PIconOAM = {  .y = ICONY,
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
                                    .affineParam = 0
};

const struct SpriteTemplate BulbTemp = {
	.tileTag = 0x3139, 
	.paletteTag = 0x3139, 
	.oam = (struct OamData*) &PIconOAM, 
	.anims = (const union AnimCmd* const*)0x8231CF0, 
	.images = NULL,
    .affineAnims = (const union AffineAnimCmd* const*) 0x8231CFC, 
	.callback = (SpriteCallback) 0x800760D,
};

