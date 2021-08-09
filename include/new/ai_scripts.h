#pragma once

#include "../global.h"

/**
 * \file ai_scripts.h
 * \brief Contains declarations of the various "ai scripts" that can be run to help
 *		  the battle AI determine which moves to use.
 */

//Exported Structures

struct AIScript
{
	u16 atkSpecies;
	u16 defSpecies;
	u8 atkAbility;
	u8 defAbility;

	u8 atkItemEffect;
	u8 defItemEffect;
	u16 defItem;
	u16 atkItem;

	u8 atkItemQuality;
	u8 defItemQuality;
	u32 atkStatus1;
	u32 defStatus1;
	u32 atkStatus2;
	u32 defStatus2;
	u32 atkStatus3;
	u32 defStatus3;
	u8 atkGender;
	u8 defGender;

	u16 atkAttack;
	u16 atkDefense;
	u16 atkSpeed;
	u16 atkSpAtk;
	u16 atkSpDef;

	u16 defAttack;
	u16 defDefense;
	u16 defSpeed;
	u16 defSpAtk;
	u16 defSpDef;

	u8 bankAtkPartner;
	u8 bankDefPartner;
	u8 atkPartnerAbility;
	u8 defPartnerAbility;

	u8 foe1;
	u8 foe2;
	u16 partnerMove;
};

//Exported Functions
u8 AIScript_Negatives(const u8 bankAtk, const u8 bankDef, const u16 move, const u8 originalViability, struct AIScript*);
u8 AIScript_SemiSmart(const u8 bankAtk, const u8 bankDef, const u16 move, const u8 originalViability, struct AIScript*);
u8 AIScript_Positives(const u8 bankAtk, const u8 bankDef, const u16 move, const u8 originalViability, struct AIScript*);
u8 AIScript_Partner(const u8 bankAtk, const u8 bankAtkPartner, const u16 originalMove, const u8 originalViability, struct AIScript*);
u8 AIScript_Roaming(const u8 bankAtk, const u8 bankDef, const u16 move, const u8 originalViability, struct AIScript*);
u8 AIScript_Safari(const u8 bankAtk, const u8 bankDef, const u16 move, const u8 originalViability, struct AIScript*);
u8 AIScript_FirstBattle(const u8 bankAtk, const u8 bankDef, const u16 move, const u8 originalViability, struct AIScript*);
