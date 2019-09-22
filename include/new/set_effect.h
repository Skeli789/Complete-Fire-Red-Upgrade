#pragma once

#include "../global.h"

/**
 * \file set_effect.h
 * \brief Contains functions relating to applying secondary effects such as burn and
 *		  flinching in battle.
 */

//Exported Functions
void SetMoveEffect(bool8 primary, u8 certain);
bool8 SetMoveEffect2(void);

//Functions Hooked In
void atk15_seteffectwithchance(void);
