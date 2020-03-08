#pragma once

#include "../battle.h"
#include "../global.h"
#include "../pokemon.h"

/**
 * \file damage_calc.h
 * \brief Contains functions relating to damage calculation, base power calculation,
 *		  move types, and type effectiveness.
 */

//Exported Functions
void atk04_critcalc(void);

void atk06_typecalc(void);
void atk4A_typecalc2(void);
u8 TypeCalc(move_t, u8 bankAtk, u8 bankDef, struct Pokemon* monAtk, bool8 CheckParty);
u8 AI_TypeCalc(move_t, u8 bankAtk, struct Pokemon* monDef);
u8 AI_SpecialTypeCalc(u16 move, u8 bankAtk, u8 bankDef);
u8 VisualTypeCalc(u16 move, u8 bankAtk, u8 bankDef);
void TypeDamageModification(u8 atkAbility, u8 bankDef, move_t, u8 moveType, u8* flags);
void TypeDamageModificationPartyMon(u8 atkAbility, struct Pokemon* monDef, u16 move, u8 moveType, u8* flags);
u8 GetMoveTypeSpecial(u8 bankAtk, move_t);
u8 GetMoveTypeSpecialPreAbility(u16 move, u8 bankAtk, struct Pokemon* monAtk);
u8 GetMoveTypeSpecialPostAbility(u16 move, u8 atkAbility, bool8 zMoveActive);
u8 GetMonMoveTypeSpecial(struct Pokemon* monAtk, u16 move);
u8 GetExceptionMoveType(u8 bankAtk, move_t);
u8 GetMonExceptionMoveType(struct Pokemon*, move_t);
u8 CalcMonHiddenPowerType(struct Pokemon* mon);

void atk05_damagecalc(void);
s32 FutureSightDamageCalc(void);
s32 ConfusionDamageCalc(void);
u32 AI_CalcDmg(const u8 bankAtk, const u8 bankDef, const u16 move);
u32 AI_CalcPartyDmg(u8 bankAtk, u8 bankDef, u16 move, struct Pokemon* mon);
u32 AI_CalcMonDefDmg(u8 bankAtk, u8 bankDef, u16 move, struct Pokemon* monDef);
u16 CalcVisualBasePower(u8 bankAtk, u8 bankDef, u16 move, bool8 ignoreDef);
u16 TryGetAlternateSpeciesSize(u16 species, u8 type);
u8 CountAliveMonsInBattle(u8 caseId, u8 bankAtk, u8 bankDef);

u32 GetActualSpeciesWeight(u16 species, u8 ability, u8 itemEffect, u8 bank, bool8 checkNimble);
bool8 IsBankHoldingFocusSash(u8 bank);

//Functions Hooked In
u8 GetSummaryScreenMoveType(u16 move, struct Pokemon* mon);

//Exported Constants
/*enum
{
	BATTLE_ALIVE_EXCEPT_ACTIVE,
	BATTLE_ALIVE_ATK_SIDE,
	BATTLE_ALIVE_DEF_SIDE,
};*/

#define BASE_CRIT_MULTIPLIER 100
