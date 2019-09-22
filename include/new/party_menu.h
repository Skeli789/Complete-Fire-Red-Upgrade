#pragma once

#include "../global.h"

/**
 * \file party_menu.h
 * \brief Contains functions relating to viewing Pokemon from the party menu. These include
 *		  the viewing the Pokemon menu screen in multi battles, as well as a feature that
 *		  causes the summary screen to wrap around when viewing the first or last Pokemon
 *		  in the party. It also updates the Pokemon selection for the Battle Tower.
 */


//Functions Hooked In
void SetNewPartySelectTarget2(s8* highlightedMon, s8 movementDir);
void CursorCb_Summary(u8 taskId);
u8 ChangeSummaryScreenMon(u8 delta);
u8 ChoosePokemon_LoadMaxPKMNStr(const u8** strPtr, bool8 loadString);
void CursorCb_Enter(u8 taskId);
void CursorCb_NoEntry(u8 taskId);
void DisplayPartyPokemonSelectForBattle(u8 slot);
