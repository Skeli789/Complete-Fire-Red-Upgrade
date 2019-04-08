#pragma once

#include "../../src/defines.h"


// DEFINES
#define OEI_GRASS 0x13
#define OEI_WATER 0x16
#define OEI_CAVE 0x1A

#define EGGMOVE_MAX 8

#define TOOL_COUNT 2
#define priv0 tasks[taskId].data[0]

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

// TYPEDEFS
typedef void (*SuperCallback)(void);


/*

// STRUCTS
static struct DexnavHudData** DNavState = (struct DexnavHudData**)(DEXNAV_SAVERAM + SPECIES_MAX);
static u8* SearchLevels = (u8*)DEXNAV_SAVERAM;
struct TextColor MenuTextBlack = {0, 2, 3};
struct Textbox tbox = {0, 2, 2, 10, TOOL_COUNT << 1, 0xF, 0x130};

struct TextColor DexNav_BlackText = {0, 3, 4};
struct TextColor DexNav_WhiteText = {0, 1, 2};
struct TextColor DexNav_RedText = {0, 7, 8};
struct TextColor DexNav_GreenText = {0, 5, 6};

//struct SpritePalette {
//	const void* data;
//    u16 tag;
//};

struct OieState2
{
    const struct SpritePalette* p;
    SuperCallback s;
};

const struct OieState2 caveState = { .p = (const struct SpritePalette*)0x083A5348,
                                        .s = (SuperCallback)oei_1A_cave,
                                        };
const struct BgTemplate BgConfigDexNavGUI[4] = {
    {
        .padding = 0,
        .b_padding = 0,
        .priority = 2,
        .paletteMode = 0,
        .screenSize = 0,
        .mapBaseIndex = 29,
        .charBaseIndex = 1,
        .bg = 0,
    },
    {
        .padding = 0,
        .b_padding = 0,
        .priority = 3,
        .paletteMode = 0,
        .screenSize = 0,
        .mapBaseIndex = 28,
        .charBaseIndex = 0,
        .bg = 1,
    },
    {
        .padding = 0,
        .b_padding = 0,
        .priority = 3,
        .paletteMode = 0,
        .screenSize = 0,
        .mapBaseIndex = 30,
        .charBaseIndex = 2,
        .bg = 2,
    },
    {
        .padding = 0,
        .b_padding = 0,
        .priority = 3,
        .paletteMode = 0,
        .screenSize = 1,
        .mapBaseIndex = 31,
        .charBaseIndex = 3,
        .bg = 3,
    },
};

const struct OamData iconOAM = {
    .y = 0,
    .affine_mode = 1,
    .obj_mode = 0,
    .mosaic = 0,
    .bpp = 0,
    .shape = 0,
    .x = 0,
    .matrix_num = 0,
    .size = 2, // 32x32 square
    .tile_num = 0,
    .priority = 2, //above the rest
    .palette_num = 0,
    .affine_param = 0,
};


const struct OamData cursor_oam = {
    .y = 0,
    .affine_mode = 0,
    .obj_mode = 0,
    .mosaic = 0,
    .bpp = 0,
    .shape = 0,
    .x = 0,
    .matrix_num = 0,
    .size = 2, //32x32
    .tile_num = 0,
    .priority = 0, // above BG layers
    .palette_num = 0,
    .affine_param = 0
};


// cursor positions for water
const u16 cursor_positions2[] = {
    30 + 24 * 0, 48,
    30 + 24 * 1, 48,
    30 + 24 * 2, 48,
    30 + 24 * 3, 48,
    30 + 24 * 4, 48,
};
// positions for grass
const u16 cursor_positions1[] = {
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

const struct SpriteTiles cave_gfx[4] = {
	{.data = (const u8*)&cave_smokeTiles[128 * 0],
		.size = 0x80,
		.tag = 0xFFFF
	},
	{.data = (const u8*)&cave_smokeTiles[128 * 1],
		.size = 0x80,
		.tag = 0xFFFF
	},
	{.data = (const u8*)&cave_smokeTiles[128 * 2],
		.size = 0x80,
		.tag = 0xFFFF
	},
	{.data = (const u8*)&cave_smokeTiles[128 * 3],
		.size = 0x80,
		.tag = 0xFFFF
	},
};
//struct SpriteTiles* ptr_cave_gfx =
struct SpriteTemplate objt_cave = {
	.tiles_tag = 0xFFFF,
	.pal_tag = 0x1005,
	.oam = (struct OamData*)0x83A36F0,
	.animation = (const struct Frame (**)[])0x83A5B70,
	.graphics = &cave_gfx[0],
	.rotscale = (const struct RotscaleFrame (**)[])0x8231CFC,
	.callback = (ObjectCallback)0x80DCD1D
};

// GUI Rboxes
#define rgb5(r, g, b) (u16)((r >> 3) | ((g >> 3) << 5) | ((b >> 3) << 10))

const u16 dnav_text_pal[] = {rgb5(255, 0, 255),   rgb5(248, 248, 248), rgb5(112, 112, 112), rgb5(96, 96, 96),
                                rgb5(208, 208, 208), rgb5(76, 154, 38),   rgb5(102, 194, 66),  rgb5(168, 75, 76),
                                rgb5(224, 114, 75),  rgb5(180, 124, 41),  rgb5(241, 188, 60),  rgb5(255, 0, 255),
                                rgb5(255, 0, 255),   rgb5(255, 0, 255),   rgb5(255, 133, 200),   rgb5(64, 200, 248)};

struct TextboxTemplate dexnav_boxes[] = {
    {
        //Species
        .bg_id = 0,
        .x = 21,
        .y = 6,
        .width = 9,
        .height = 2,
        .pal_id = 15,
        .charbase = 1,
    },
    {
        //Search level
        .bg_id = 0,
        .x = 21,
        .y = 9,
        .width = 9,
        .height = 2,
        .pal_id = 15,
        .charbase = 31,
    },
    {
        //Level bonus
        .bg_id = 0,
        .x = 21,
        .y = 12,
        .width = 3,
        .height = 3,
        .pal_id = 15,
        .charbase = 61,
    },
    {
        //Hidden Ability
        .bg_id = 0,
        .x = 21,
        .y = 15,
        .width = 12,
        .height = 3,
        .pal_id = 15,
        .charbase = 76,
    },
    {
        // Reply text
        .bg_id = 0,
        .x = 1,
        .y = 17,
        .width = 22,
        .height = 3,
        .pal_id = 15,
        .charbase = 136,
    },
    {
        .bg_id = 0xFF, // marks the end of the tb array
    },

};

const struct Frame (**nullframe)[] = (const struct Frame (**)[])0x8231CF0;
const struct RotscaleFrame (**nullrsf)[] = (const struct RotscaleFrame (**)[])0x8231CFC;

// 32x64 oam with highest priority
const struct OamData fontOAM = {    .y = ICONY,
                                    .affine_mode = 0,
                                    .obj_mode = 0,
                                    .mosaic = 0,
                                    .bpp = 0,
                                    .shape = 1,
                                    .x = ICONX,
                                    .matrix_num = 0,
                                    .size = 3,
                                    .tile_num = 0,
                                    .priority = 0,
                                    .palette_num = 0,
                                    .affine_param = 0
};


// 8x8 oam with highest priority
const struct OamData held_oam = {   .y = ICONY,
                                    .affine_mode = 0,
                                    .obj_mode = 0,
                                    .mosaic = 0,
                                    .bpp = 0,
                                    .shape = 0,
                                    .x = ICONX,
                                    .matrix_num = 0,
                                    .size = 0,
                                    .tile_num = 0,
                                    .priority = 0,
                                    .palette_num = 0,
                                    .affine_param = 0
};

struct SpritePalette heldPal = {(const u8*)&pal_heldicons, 0x8472};

// 32x32 object with priority 1, one less than held item which overlaps it
const struct OamData picon_oam = {  .y = ICONY,
                                    .affine_mode = 0,
                                    .obj_mode = 0,
                                    .mosaic = 0,
                                    .bpp = 0,
                                    .shape = 0,
                                    .x = ICONX,
                                    .matrix_num = 0,
                                    .size = 2,
                                    .tile_num = 0,
                                    .priority = 1,
                                    .palette_num = 0,
                                    .affine_param = 0
};

const struct SpriteTemplate bulbtemp = {0x3139, 0x3139, &picon_oam, (const struct Frame (**)[])0x8231CF0, 0x0,
                            (const struct RotscaleFrame (**)[])0x8231CFC, (ObjectCallback)0x800760D};

// EXTERNAL DEFINES
extern u8 gText_DexNavText[];
extern u8 gText_PokeTools[];
extern u8 gText_GotAway[];
extern u8 gText_LostSignal[];
extern u8 gText_CannotBeFound[];
extern u8 gText_NotFoundNearby[];
extern u8 gText_DexNavBack[];
extern u8 gText_DexNav_NoInfo[];
extern u8 gText_DexNav_ChooseMon[];
extern u8 gText_DexNav_Invalid[];
extern u8 gText_DexNav_Locked[];

extern u16 rand_range(u16 min, u16 max);
extern const struct OamData fontOAM;
extern const struct OamData held_oam;
extern struct SpritePalette heldPal;
extern const struct OamData picon_oam;
extern const struct SpriteTemplate bulbtemp;
extern const struct Frame (**nullframe)[];
extern const struct RotscaleFrame (**nullrsf)[];

extern void vblank_cb_spq(void);
extern void c2_dexnav_gui(void);
extern void setup(void);
extern u16 rand_range(u16 min, u16 max);



extern void dprintf(const char * str, ...);

#define CPUFSCPY 0
#define CPUFSSET 1
#define CPUModeFS(size, mode) ((size >> 2) | (mode << 24))
extern void CpuFastSet(void* src, void* dst, u32 mode);
#define OBJID_HIDE(objid) objects[objid].final_oam.affine_mode = 2
#define OBJID_SHOW(objid) objects[objid].final_oam.affine_mode = 0

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

struct DexnavHudData {
    u16 species;
    enum Move move_id[4]; // u16
    enum Item held_item;
    enum PokemonAbility ability; // u8
    u8 potential;
    u8 search_level;
    u8 pokemon_level;
    u8 move_name_length;
    u8 proximity;
    u8 environment;
    s16 tile_x; // position of shaking grass
    s16 tile_y;
    u8 obj_id_species;
    u8 obj_id_sight;
    u8 obj_id_ability;
    u8 obj_id_move;
    u8 obj_id_item;
    u8 obj_id_shaking_grass;
    u8 obj_id_potential[3];
    u8 movement_times;

    // GUI data
    u16 grass_species[12];
    u16 water_species[5];
    u8 cursor_id;
    u8 objids[17];
    u8 selected_index;
    u8 selected_arr;
    void* backbuffer;

};

//
//extern void dexnav_generate_move(u16, u8, u8, u16*);
//extern enum PokemonAbility dexnav_generate_hiddenability(u16, u8);
//extern u8 dexnav_generate_potential(u8);
//extern enum Item dexnav_generate_helditem(u16, u8);
//extern u8 get_sightlvl(u8);
//extern u8 dexnav_generate_pokemonlvl(u16, u8, u8);
//extern void dexnav_proximity_update(void);
//extern void dexnav_draw_icons(void);
//extern void dexnav_icons_vision_update(u8, u8);
//extern void dexnav_hud_manage(u8);
//extern void dexhud_hblank(void);
//extern void MsgNormal(pchar *);


//extern const u16 dnav_text_pal[];
//extern struct TextColor dnav_text_black;
//extern struct TextColor dnav_text_white;
//extern struct TextColor dnav_text_red;
//extern struct TextColor dnav_text_green;
//extern struct TextboxTemplate dexnav_boxes[];


// GUI BG template
extern const struct BgConfig bg_config_dexnav_gui[4];

// cursor and pokeicon oam
extern const struct OamData icon_oam;
extern const struct OamData cursor_oam;
// cursor positions for water
extern const u16 cursor_positions2[];
// positions for grass
extern const u16 cursor_positions1[];




// FOR NOW -> need to replace
struct wild_pokemon {
    u8 min_level;
    u8 max_level;
    u16 species;
};

union wild_pokemon_densities {
    struct wild_pokemon wild_grass[12];
    struct wild_pokemon wild_water[5];
    struct wild_pokemon wild_tree[5];
    struct wild_pokemon wild_fishing[10];
};

struct pokemon_by_encounter_rate {
    u8 encounter_chance;
    u8 padding[3];
    union wild_pokemon_densities *wild_encounters;
};

struct map_wild_pokemon_data {
    u8 map_bank;
    u8 map_num;
    u16 padding;
    struct pokemon_by_encounter_rate *grass_encounter; // 12 slots
    struct pokemon_by_encounter_rate *water_encounter; // 5 slots 
    struct pokemon_by_encounter_rate *tree_encounter; // 5 slots
    struct pokemon_by_encounter_rate *fishing_encounter; // 10 slots

};



*/

