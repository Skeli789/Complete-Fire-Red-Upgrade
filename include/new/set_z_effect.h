#pragma once

#include "../global.h"

/**
 * \file set_z_effect.h
 * \brief Contains the function that applies the special effects for status Z-Moves. Also
 *		  contains the sprite data for the Z-Move trigger that appears when a move can be
 *		  turned into a Z-Move. Additionally contains general Z-Move utility functions.
 */
 
//Exported Functions
void SetZEffect(void);
void TryLoadZTrigger(void);
void DestroyZTrigger(void);
u16 GetTypeBasedZMove(u16 move, u8 bank);
u16 GetSpecialZMove(u16 move, u16 species, u16 item);
bool8 IsSpecialZCrystal(u16 item);
const u8* GetZMoveName(u16 move);