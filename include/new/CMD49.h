#pragma once

#include "../../src/defines.h"
#include "../../src/defines_battle.h"

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

#define ARG_IN_FUTURE_ATTACK 3
#define ARG_IN_PURSUIT 4
#define ARG_ONLY_EMERGENCY_EXIT 5

#define BattleScript_ThrashConfuses (u8*) 0x81D90C5
#define BattleScript_DefrostedViaFireMove (u8*) 0x81D9098
#define BattleScript_FlushMessageBox (u8*) 0x81D96A8
#define BattleScript_Recoil (u8*) 0x81D9243
