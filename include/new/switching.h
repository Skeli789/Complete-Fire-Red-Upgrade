#pragma once

#include "../global.h"

/**
 * \file switching.h
 * \brief Contains functions relating to Pokemon switching in battle. These include effects
 *		  when the Pokemon switches out, and effects when a new Pokemon switches in. It also
 *		  includes code for Baton Pass.
 */
 
//Exported Functions
bool8 HasNoMonsToSwitch(u8 battler);
bool8 TryRemovePrimalWeather(u8 bank, u8 ability);
void ClearSwitchBytes(u8 bank);
void ClearSwitchBits(u8 bank);

//Functions Hooked In
void atkE2_switchoutabilities(void);
void atk61_drawpartystatussummary(void);
void atk4D_switchindataupdate(void);
void atk4F_jumpifcantswitch(void);
void atk8F_forcerandomswitch(void);
void PartyMenuSwitchingUpdate(void);