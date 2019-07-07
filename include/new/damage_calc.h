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
u8 TypeCalc(move_t, u8 bankAtk, u8 bankDef, struct Pokemon* party_data_atk, bool8 CheckParty);
u8 AI_TypeCalc(move_t, u8 bankAtk, struct Pokemon* party_data_def);
void TypeDamageModification(u8 atkAbility, u8 bankDef, move_t, u8 moveType, u8* flags);
u8 GetMoveTypeSpecial(u8 bankAtk, move_t);
u8 GetMoveTypeSpecialFromParty(struct Pokemon* party_data, u16 move);
u8 GetExceptionMoveType(u8 bankAtk, move_t);
u8 GetExceptionMoveTypeFromParty(struct Pokemon*, move_t);

void atk05_damagecalc(void);
s32 FutureSightDamageCalc(void);
u32 AI_CalcDmg(const u8 bankAtk, const u8 bankDef, const u16 move);
u32 AI_CalcPartyDmg(u8 bankAtk, u8 bankDef, u16 move, struct Pokemon* mon);
s32 CalculateBaseDamage(struct BattlePokemon* attacker, struct BattlePokemon* defender, const u16 move, u16 sideStatus, u16 powerOverride, u8 effectivenessFlags, u8 typeOverride, u8 bankAtk, u8 bankDef, pokemon_t* party_data_atk, bool8 PartyCheck, bool8 IgnoreAttacker, bool8 CheckingConfusion);
u16 GetBasePower(u8 bankAtk, u8 bankDef, u16 move, u16 item, u8 item_effect, u8 ability, u32 atkStatus1, u16 atk_hp, u16 atk_maxHP, u16 species, struct Pokemon* party_data_atk, bool8 PartyCheck, bool8 menuCheck, bool8 ignoreDef);
u16 CalcVisualBasePower(u8 bankAtk, u8 bankDef, u16 move, u16 power, u8 moveType, bool8 ignoreDef);
u16 TryGetAlternateSpeciesSize(u16 species, u8 type);

u32 GetActualSpeciesWeight(u16 species, u8 ability, u8 itemEffect, u8 bank, bool8 checkNimble);

//Functions Hooked In
u8 GetSummaryScreenMoveType(u16 move, struct Pokemon* mon);