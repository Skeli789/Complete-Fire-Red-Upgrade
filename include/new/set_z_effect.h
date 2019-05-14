#pragma once

#include "../global.h"

/**
 * \file set_z_effect.h
 * \brief Contains the function that applies the special effects for status Z-Moves. Also
 *		  contains the sprite data for the Z-Move trigger that appears when a move can be
 *		  turned into a Z-Move.
 */
 
//Exported Functions
void SetZEffect(void);
void TryLoadZTrigger(void);
void DestroyZTrigger(void);