#pragma once

#include "../global.h"

/**
 * \file set_z_effect.h
 * \brief Contains the function that applies the special effects for status Z-Moves. Also
 *		  Additionally contains general Z-Move utility functions.
 */

//Exported Functions
void SetZEffect(void);
u16 GetTypeBasedZMove(u16 move, u8 bank);
u16 GetSpecialZMove(u16 move, u16 species, u16 item);
bool8 IsSpecialZCrystal(u16 item);
const u8* GetZMoveName(u16 move);
bool8 DoesZMoveUsageStopMegaEvolution(u8 bank);
bool8 DoesZMoveUsageStopDynamaxing(u8 bank);
move_t CanUseZMove(u8 bank, u8 moveIndex, u16 move);
move_t ReplaceWithZMoveRuntime(u8 bankAtk, u16 move);
