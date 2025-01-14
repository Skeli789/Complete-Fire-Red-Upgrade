#pragma once

#include "../global.h"

/**
 * \file dexnav.h
 * \brief Contains a significant improvement of the DexNav system created by FBI on Pokecommunity:
 *		  https://www.pokecommunity.com/showthread.php?p=9998647
 */

//Expert Constants
#define GFX_TAG_HELD_ITEM 0x8472

//Exported Functions
bool8 InitDexNavHUD(u16 species, u8 environment, bool8 detectorMode);
bool8 IsDexNavHudActive(void);
void TryFreeDexnavState(void);
u8 GetPlayerDistance(s16 x, s16 y);

//Functions Hooked In
u8 SelectHookRestore(void);
u8 StartMenuDexNavCallback(void);
