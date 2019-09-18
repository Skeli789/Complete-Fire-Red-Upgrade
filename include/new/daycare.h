#pragma once

#include "../global.h"

/**
 * \file daycare.h
 * \brief Contains functions relating to the Pokemon daycare (mainly focusing on
 *		  breeding Pokemon).
 */

//Exported Constants
#define EGG_MOVES_ARRAY_COUNT 50
 
//Exported Functions
void AlterSpeciesWithIncenseItems(u16* species, u16 motherItem, u16 fatherItem);
u8 GetAllEggMoves(struct Pokemon* mon, u16* moves, bool8 ignoreAlreadyKnownMoves);

//Functions Hooked In
