#pragma once

#include "../global.h"

/**
 * \file ai_scripts.h
 * \brief Contains declarations of the various "ai scripts" that can be run to help
 *		  the battle AI determine which moves to use.
 */

//Exported Functions
u8 AI_Script_Negatives(const u8 bankAtk, const u8 bankDef, const u16 move, const u8 originalViability);
u8 AI_Script_Positives(const u8 bankAtk, const u8 bankDef, const u16 move, const u8 originalViability);
u8 AI_Script_Partner(const u8 bankAtk, const u8 bankAtkPartner, const u16 originalMove, const u8 originalViability);
u8 AI_Script_Roaming(const u8 bankAtk, const u8 bankDef, const u16 move, const u8 originalViability);
u8 AI_Script_Safari(const u8 bankAtk, const u8 bankDef, const u16 move, const u8 originalViability);
u8 AI_Script_FirstBattle(const u8 bankAtk, const u8 bankDef, const u16 move, const u8 originalViability);
