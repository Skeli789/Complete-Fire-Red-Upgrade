#pragma once

#include "../global.h"

/**
 * \file stat_buffs.h
 * \brief Functions related to the raising and lowering of stats in-battle.
 */

//Exported Constants
enum
{
	STAT_CAN_BE_LOWERED,
	STAT_AT_MIN,
	STAT_PROTECTED_BY_MIST,
	STAT_PROTECTED_BY_GENERAL_ABILITY,
	STAT_PROTECTED_BY_SPECIFIC_ABILITY,
	STAT_AT_MAX,
	STAT_FAINTED,
	STAT_PROTECTED_BY_PARTNER_ABILITY,
};

//Exported Functions
u8 ChangeStatBuffs(s8 statValue, u8 statId, u8 flags, const u8* BS_ptr);
bool8 DefiantActivation(void);
u8 CanStatNotBeLowered(u8 statId, u8 bankDef, u8 bankAtk, u8 defAbility);
bool8 CanStatBeLowered(u8 statId, u8 bankDef, u8 bankAtk, u8 defAbility);

//Functions Hooked In
void atk13_printfromtable(void);
void atk20_jumpifstat(void);
void atk48_playstatchangeanimation(void);
void atk89_statbuffchange(void);
