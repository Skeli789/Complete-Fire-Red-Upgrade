#pragma once

#include "global.h"
#include "pokemon.h"

#define ORIGINAL_TOTAL_BOXES_COUNT 	14
#define TOTAL_BOXES_COUNT       	25
#define IN_BOX_ROWS             	5
#define IN_BOX_COLUMNS          	6
#define IN_BOX_COUNT            	(IN_BOX_ROWS * IN_BOX_COLUMNS)

//So bytereplacement can call a constant
#define TOTAL_BOXES_COUNT_1_LESS	24

/*
            ROWS
COLUMNS     0   1   2   3   4   5
            6   7   8   9   10  11
            12  13  14  15  16  17
            18  19  20  21  22  23
            24  25  26  27  28  29
*/

struct PokemonStorage
{
    /*0x0000*/ u8 currentBox;
    /*0x0001*/ struct BoxPokemon boxes[ORIGINAL_TOTAL_BOXES_COUNT][IN_BOX_COUNT];
    /*0x8344*/ u8 boxNames[ORIGINAL_TOTAL_BOXES_COUNT][9];
    /*0x83C2*/ u8 boxWallpapers[ORIGINAL_TOTAL_BOXES_COUNT];
};

struct WallpaperTable
{
    const u8* tiles;
    const u8* tileMap;
    const u16* palettes;
};

//extern struct PokemonStorage* gPokemonStoragePtr;

u8 __attribute__((long_call)) StorageGetCurrentBox(void);
void __attribute__((long_call)) CompactPartySlots(void);
u16 __attribute__((long_call)) GetPCBoxToSendMon(void);
void __attribute__((long_call)) SetPCBoxToSendMon(u8 id);
