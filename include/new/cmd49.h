#pragma once

#include "../global.h"

/**
 * \file cmd49.h
 * \brief Contains functions relating to the end of an attack. This occurs after
 *		  the faint check of the target Pokemon. The main function refers to the
 *		  "cmd49" or "moveend" battle scripting command.
 */

//Exported Functions
void atk49_moveend(void);
bank_t GetNextMultiTarget(void);

//Exported Constants
enum
{
	FAINT_SET_UP,
	FAINT_ATTACKER_ABILITIES,
	FAINT_ADVERSE_PROTECTION,
    FAINT_RAGE,
    FAINT_SYNCHRONIZE_TARGET,
	FAINT_BEAK_BLAST_BURN,
	FAINT_SYNCHRONIZE_ATTACKER,
    FAINT_MOVE_END_ABILITIES,
    FAINT_SYNCHRONIZE_ATTACKER_2,
	FAINT_ITEM_EFFECTS_CONTACT_TARGET,
	FAINT_COUNT,
};

enum
{
	Force_Switch_Regular,
	Force_Switch_Dragon_Tail,
	Force_Switch_Red_Card
};

#define ARG_IN_FUTURE_ATTACK 3
#define ARG_IN_PURSUIT 4
#define ARG_ONLY_EMERGENCY_EXIT 5
#define ARG_DRAGON_TAIL 6
#define ARG_PARTING_SHOT 7

