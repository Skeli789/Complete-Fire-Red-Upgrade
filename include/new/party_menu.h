#pragma once

#include "../global.h"

/**
 * \file party_menu.h
 * \brief Contains functions relating to viewing Pokemon from the party menu. These include
 *		  the viewing the Pokemon menu screen in multi battles, as well as a feature that
 *		  causes the summary screen to wrap around when viewing the first or last Pokemon
 *		  in the party.
 */
 
//Functions Hooked In
void SetNewPartySelectTarget2(s8* highlightedMon, s8 movementDir);
u8 ChangeSummaryScreenMon(u8 delta);