#pragma once

#include "global.h"

enum
{
    FLAG_GET_SEEN,
    FLAG_GET_CAUGHT,
    FLAG_SET_SEEN,
    FLAG_SET_CAUGHT
};

enum
{
	PKDX_GET_HEIGHT,
	PKDX_GET_WEIGHT
};

struct PokedexEntry
{
    /*0x00*/ u8 categoryName[12];
    /*0x0C*/ u16 height; //in decimeters
    /*0x0E*/ u16 weight; //in hectograms
    /*0x10*/ const u8* description;
    /*0x14*/ u16 unused;
    /*0x16*/ u16 pokemonScale;
    /*0x18*/ u16 pokemonOffset;
    /*0x1A*/ u16 trainerScale;
    /*0x1C*/ u16 trainerOffset;
	/*0x1E*/ u16 unknown1;
	/*0x20*/ u32 unknown2;
};  /*size = 0x24*/

struct AlternateSize
{
	u16 species;
	u16 height;
	u16 weight;
};

#define SEEN_DEX_FLAGS gSaveBlock1->dexSeenFlags //0x202583C
#define CAUGHT_DEX_FLAGS gSaveBlock1->dexCaughtFlags //0x20258B9

bool16 __attribute__((long_call)) HasAllMons(void);

/*
void ResetPokedex(void);
void CopyMonCategoryText(u16 species, u8 *dst);
u16 GetPokedexHeightWeight(u16 dexNum, u8 data);
u16 GetNationalPokedexCount(u8);
u16 GetHoennPokedexCount(u8);
u8 CreateDexDisplayMonDataTask(u16 dexNum, u32 trainerId, u32 personality);
*/
