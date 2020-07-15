#pragma once

#include "../global.h"

/**
 * \file end_battle.h
 * \brief Contains functions relating to things that end battle and what happens
 *		  when a battle actually ends.
 */

//Exported Functions
bool8 IsConsumable(u16 item);
bool8 AreAllKindsOfRunningPrevented(void);

//Functions Hooked In
void HandleEndTurn_BattleWon(void);
void HandleEndTurn_BattleLost(void);
void HandleEndTurn_RanFromBattle(void);
bool8 HandleRunActionFrontier(void);
u8 IsRunningFromBattleImpossible(void);
bool8 TryRunFromBattle(u8 bank);
void EndOfBattleThings(void);

//Exported Data Structures
extern u8 gConsumableItemEffects[];
