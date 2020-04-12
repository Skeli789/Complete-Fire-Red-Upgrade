#pragma once

#include "../global.h"

/**
 * \file stat_buffs.h
 * \brief Functions related to the raising and lowering of stats in-battle.
 */

//Exported Functions
u8 ChangeStatBuffs(s8 statValue, u8 statId, u8 flags, const u8* BS_ptr);
bool8 DefiantActivation(void);

//Functions Hooked In
void atk13_printfromtable(void);
void atk20_jumpifstat(void);
void atk48_playstatchangeanimation(void);
void atk89_statbuffchange(void);
