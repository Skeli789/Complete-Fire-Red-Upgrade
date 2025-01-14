#pragma once

#include "../battle.h"
#include "../global.h"
#include "../pokemon.h"

/**
 * \file damage_calc.h
 * \brief Contains functions relating to damage calculation, base power calculation,
 *		  move types, and type effectiveness.
 */

//Exported Structures
struct DamageCalc
{
	u8 bankAtk;
	u8 bankDef;
	struct Pokemon* monAtk;
	struct Pokemon* monDef;

	u16 atkSpecies;
	u16 defSpecies;

	u8 atkAbility;
	u8 defAbility;
	u8 atkPartnerAbility;
	u8 defPartnerAbility;

	u8 atkItemEffect;
	u8 atkItemQuality;
	u8 defItemEffect;
	u8 defItemQuality;

	u16 atkItem;

	u16 atkHP;
	u16 defHP;
	u16 atkMaxHP;
	u16 defMaxHP;
//	u32 attack;
//	u32 spAttack;
	u32 defense;
	u32 spDefense;
	u32 atkSpeed;
	u32 defSpeed;

	u8 atkBuff;
	u8 spAtkBuff;
	u8 defBuff;
	u8 spDefBuff;

	u32 atkStatus1;
	u32 defStatus1;
	u32 atkStatus3;
	u32 defStatus3;
	u16 defSideStatus;
	bool8 atkIsGrounded;
	bool8 defIsGrounded;

	u16 move;
	u8 moveType;
	u8 moveSplit;
	u8 resultFlags;
	u8 basePower;

	u8 atkImposterBank;
	u8 defImposterBank;
	bool8 atkImposter;
	bool8 defImposter;

	u8 specialFlags;
	bool8 attackerLoaded;
	bool8 defenderLoaded;
};

//Exported Functions
void atk04_critcalc(void);

void atk06_typecalc(void);
void atk4A_typecalc2(void);
u8 TypeCalc(move_t, u8 bankAtk, u8 bankDef, struct Pokemon* monAtk);
u8 AI_TypeCalc(move_t, u8 bankAtk, u8 bankDef, struct Pokemon* monDef);
u8 AI_SpecialTypeCalc(u16 move, u8 bankAtk, u8 bankDef);
u8 VisualTypeCalc(u16 move, u8 bankAtk, u8 bankDef);
void TypeDamageModification(u8 atkAbility, u8 bankDef, move_t, u8 moveType, u8* flags);
void TypeDamageModificationPartyMon(u8 atkAbility, struct Pokemon* monDef, u16 move, u8 moveType, u8* flags);
void ModulateByTypeEffectiveness(u8 atkType, u8 defType1, u8 defType2, u8* var);
u8 GetMoveTypeSpecial(u8 bankAtk, move_t);
u8 GetMoveTypeSpecialPreAbility(u16 move, u8 bankAtk, struct Pokemon* monAtk);
u8 GetMoveTypeSpecialPostAbility(u16 move, u8 atkAbility, bool8 zMoveActive);
u8 GetMonMoveTypeSpecial(struct Pokemon* monAtk, u16 move);
u8 GetExceptionMoveType(u8 bankAtk, move_t);
u8 GetMonExceptionMoveType(struct Pokemon*, move_t);
u8 CalcMonHiddenPowerType(struct Pokemon* mon);

void atk05_damagecalc(void);
void FutureSightDamageCalc(void);
u32 SplintersDamageCalc(u8 bankAtk, u8 bankDef, u16 move);
s32 ConfusionDamageCalc(void);
u32 AI_CalcDmg(const u8 bankAtk, const u8 bankDef, const u16 move, struct DamageCalc* damageData);
u32 AI_CalcPartyDmg(u8 bankAtk, u8 bankDef, u16 move, struct Pokemon* mon, struct DamageCalc* damageData);
u32 AI_CalcMonDefDmg(u8 bankAtk, u8 bankDef, u16 move, struct Pokemon* monDef, struct DamageCalc* damageData);
u32 TryAdjustDamageForRaidBoss(u8 bankDef, u32 damage);
void PopulateDamageCalcStructWithBaseAttackerData(struct DamageCalc* data);
void PopulateDamageCalcStructWithBaseDefenderData(struct DamageCalc* data);
u16 CalcVisualBasePower(u8 bankAtk, u8 bankDef, u16 move, bool8 ignoreDef);
u16 TryGetAlternateSpeciesSize(u16 species, u8 type);
u8 CountAliveMonsInBattle(u8 caseId, u8 bankAtk, u8 bankDef);
void CalculateShellSideArmSplits(void);

u8 GetNaturalGiftMoveType(u16 item);
u8 GetNaturalGiftMovePower(u16 item);
u32 GetActualSpeciesWeight(u16 species, u8 ability, u8 itemEffect, u8 bank, bool8 checkNimble);

//Functions Hooked In
u8 GetSummaryScreenMoveType(u16 move, struct Pokemon* mon);

//Exported Constants
/*enum
{
	BATTLE_ALIVE_EXCEPT_ACTIVE,
	BATTLE_ALIVE_ATK_SIDE,
	BATTLE_ALIVE_DEF_SIDE,
};*/

#define BASE_CRIT_MULTIPLIER 10
