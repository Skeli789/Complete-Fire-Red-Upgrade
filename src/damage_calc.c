#include "defines.h"
#include "defines_battle.h"
#include "../include/event_data.h"
#include "../include/pokedex.h"
#include "../include/random.h"
#include "../include/constants/items.h"
#include "../include/constants/pokedex.h"

#include "../include/new/accuracy_calc.h"
#include "../include/new/ai_util.h"
#include "../include/new/battle_start_turn_start.h"
#include "../include/new/battle_util.h"
#include "../include/new/damage_calc.h"
#include "../include/new/frontier.h"
#include "../include/new/general_bs_commands.h"
#include "../include/new/util.h"
#include "../include/new/item.h"
#include "../include/new/move_tables.h"

//TODO: Update Type Calc to have an option to be fooled by Illusion

extern const struct NaturalGiftStruct gNaturalGiftTable[];
extern const struct FlingStruct gFlingTable[];
extern struct AlternateSize gAlternateSpeciesSizeTable[];

#ifdef CRIT_CHANCE_GEN_6
	static const u16 Gen6CriticalHitChance[] = {16, 8, 2, 1, 1};
#elifdef CRIT_CHANCE_GEN_2_TO_5
	static const u16 Gen2_5CriticalHitChance[] = {16, 8, 4, 3, 2};
#else
	static const u16 Gen7CriticalHitChance[] = {24, 8, 2, 1, 1};
#endif

#define BASE_CRIT_MULTIPLIER 100
#ifdef OLD_CRIT_DAMAGE
	#define CRIT_MULTIPLIER 200
#else //Gen 6+ crit damage
	#define CRIT_MULTIPLIER 150
#endif

//This file's functions:
static u8 CalcPossibleCritChance(u8 bankAtk, u8 bankDef, u16 move, struct Pokemon* atkMon, bool8 CheckParty);
static void TypeDamageModificationByDefTypes(u8 atkAbility, u8 bankDef, u16 move, u8 moveType, u8* flags, u8 defType1, u8 defType2, u8 defType3);
static void ModulateDmgByType(u8 multiplier, const u16 move, const u8 moveType, const u8 defType, const u8 defBank, u8* flags, struct Pokemon* monDef, bool8 CheckPartyDef);
static bool8 AbilityCanChangeTypeAndBoost(u8 bankAtk, u16 move);
static u16 GetZMovePower(u16 zMove);
static u32 AdjustWeight(u32 weight, ability_t, item_effect_t, bank_t, bool8 check_nimble);
static u8 GetFlingPower(u16 item, u16 species, u8 ability, u8 bank, bool8 partyCheck);
static void AdjustDamage(bool8 CheckFalseSwipe);
static void ApplyRandomDmgMultiplier(void);

void atk04_critcalc(void) {
	u8 atkEffect = ITEM_EFFECT(gBankAttacker);
	u16 critChance = 0;
	bool8 confirmedCrit = FALSE;
	gStringBank = gBankAttacker;

	u8 atkAbility = ABILITY(gBankAttacker);
	u8 defAbility = ABILITY(gBankTarget);

	if (defAbility == ABILITY_BATTLEARMOR
	||  defAbility == ABILITY_SHELLARMOR
	||  CantScoreACrit(gBankAttacker, NULL)
	||  gBattleTypeFlags & (BATTLE_TYPE_OLD_MAN | BATTLE_TYPE_OAK_TUTORIAL | BATTLE_TYPE_POKE_DUDE)
	||  gNewBS->LuckyChantTimers[SIDE(gBankTarget)])
			confirmedCrit = FALSE;

	else if ((atkAbility == ABILITY_MERCILESS && (gBattleMons[gBankTarget].status1 & STATUS_PSN_ANY))
	|| IsLaserFocused(gBankAttacker)
	|| CheckTableForMove(gCurrentMove, gAlwaysCriticalMoves))
		confirmedCrit = TRUE;

	else {
		critChance  = 2 * ((gBattleMons[gBankAttacker].status2 & STATUS2_FOCUS_ENERGY) != 0)
					+ (CheckTableForMove(gCurrentMove, gHighCriticalChanceMoves))
					+ (atkEffect == ITEM_EFFECT_SCOPE_LENS)
					+ (atkAbility == ABILITY_SUPERLUCK)
					+ 2 * (atkEffect == ITEM_EFFECT_LUCKY_PUNCH && gBattleMons[gBankAttacker].species == SPECIES_CHANSEY)
					+ 2 * (atkEffect == ITEM_EFFECT_STICK && gBattleMons[gBankAttacker].species == SPECIES_FARFETCHD)
					+ 2 * (gCurrentMove == MOVE_10000000_VOLT_THUNDERBOLT);

		if (critChance > 4)
			critChance = 4;

		#ifdef CRIT_CHANCE_GEN_6
			if (!(Random() % Gen6CriticalHitChance[critChance]))
				confirmedCrit = TRUE;
		#elifdef CRIT_CHANCE_GEN_2_TO_5
			if (!(Random() % Gen2_5CriticalHitChance[critChance]))
				confirmedCrit = TRUE;
		#else
			if (!(Random() % Gen7CriticalHitChance[critChance]))
				confirmedCrit = TRUE;
		#endif
	}

	gCritMultiplier = BASE_CRIT_MULTIPLIER;

	//These damages will be divded by 100 so really 2x and 1.5x
	if (confirmedCrit)
		gCritMultiplier = CRIT_MULTIPLIER;

	++gBattlescriptCurrInstr;
}

static u8 CalcPossibleCritChance(u8 bankAtk, u8 bankDef, u16 move, struct Pokemon* atkMon, bool8 CheckParty) {
	u8 atkAbility;
	u8 defAbility = ABILITY(bankDef);
	u8 atkEffect = 0;
	u16 atkSpecies;
	u32 atkStatus2;
	u16 critChance = 0;

	switch (CheckParty) {
		case FALSE:
			atkAbility = ABILITY(bankAtk);
			atkEffect = ITEM_EFFECT(bankAtk);
			atkSpecies = gBattleMons[bankAtk].species;
			atkStatus2 = gBattleMons[bankAtk].status2;
			break;

		default:
			atkAbility = GetPartyAbility(atkMon);
			atkSpecies = atkMon->species;
			if (atkAbility != ABILITY_KLUTZ && !IsMagicRoomActive())
				atkEffect = ItemId_GetHoldEffect(atkMon->item);
			atkStatus2 = 0;
	}

	if (defAbility == ABILITY_BATTLEARMOR
	||  defAbility == ABILITY_SHELLARMOR
	||  CantScoreACrit(bankAtk, atkMon)
	||  gBattleTypeFlags & (BATTLE_TYPE_OLD_MAN | BATTLE_TYPE_OAK_TUTORIAL)
	||  gNewBS->LuckyChantTimers[SIDE(bankDef)])
		return FALSE;

	else if ((atkAbility == ABILITY_MERCILESS && (gBattleMons[bankDef].status1 & STATUS_PSN_ANY))
	|| (IsLaserFocused(bankAtk) && !CheckParty)
	|| CheckTableForMove(move, gAlwaysCriticalMoves))
		return TRUE;

	else {
		critChance  = 2 * ((atkStatus2 & STATUS2_FOCUS_ENERGY) != 0)
					+ (CheckTableForMove(move, gHighCriticalChanceMoves))
					+ (atkEffect == ITEM_EFFECT_SCOPE_LENS)
					+ (atkAbility == ABILITY_SUPERLUCK)
					+ 2 * (atkEffect == ITEM_EFFECT_LUCKY_PUNCH && atkSpecies == SPECIES_CHANSEY)
					+ 2 * (atkEffect == ITEM_EFFECT_STICK && atkSpecies == SPECIES_FARFETCHD)
					+ 2 * (move == MOVE_10000000_VOLT_THUNDERBOLT);

		#ifdef CRIT_CHANCE_GEN_6
			if (critChance >= 3)
				return TRUE;
			if (critChance >= 2)
				return 2; //50 % Chance
		#elifdef CRIT_CHANCE_GEN_2_TO_5
			if (critChance >= 4)
				return 2; //50 % Chance
		#else
			if (critChance >= 3)
				return TRUE;
			if (critChance >= 2)
				return 2; //50 % Chance
		#endif
	}
	return FALSE;
}

void atk05_damagecalc(void) {
	gBattleStruct->dynamicMoveType = GetMoveTypeSpecial(gBankAttacker, gCurrentMove);
	u16 side_hword = gSideAffecting[SIDE(gBankTarget)];

	if (gNewBS->DamageTaken[gBankTarget] && gMultiHitCounter == 0)
		gBattleMoveDamage = gNewBS->DamageTaken[gBankTarget];

	else if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE
	&& gBattleMoves[gCurrentMove].target & (MOVE_TARGET_BOTH | MOVE_TARGET_ALL)) //All multi target foes are calculated now
	{																			 //b/c stats can change after first kill (eg. Moxie)
		for (int i = 0; i < gBattlersCount; ++i)
		{
			if (!(gNewBS->ResultFlags[i] & MOVE_RESULT_NO_EFFECT)) //The attacker will have had this loaded for itself earlier
			{
				gNewBS->DamageTaken[i] = CalculateBaseDamage(&gBattleMons[gBankAttacker], &gBattleMons[i], gCurrentMove,
															 side_hword, gDynamicBasePower,
															 gNewBS->ResultFlags[i],
															 gBattleStruct->dynamicMoveType, gBankAttacker, i,
															 GetBankPartyData(gBankAttacker), FALSE, FALSE, FALSE);
			}
		}
		gBattleMoveDamage = gNewBS->DamageTaken[gBankTarget];
	}
	else //Single Battle or single target move
	{
		gBattleMoveDamage = CalculateBaseDamage(&gBattleMons[gBankAttacker], &gBattleMons[gBankTarget], gCurrentMove,
												side_hword, gDynamicBasePower,
												TypeCalc(gCurrentMove, gBankAttacker, gBankTarget, 0, FALSE),
												gBattleStruct->dynamicMoveType, gBankAttacker, gBankTarget,
												GetBankPartyData(gBankAttacker), FALSE, FALSE, FALSE);
	}

	gBattleMoveDamage = gBattleMoveDamage * (gCritMultiplier / BASE_CRIT_MULTIPLIER);
	++gBattlescriptCurrInstr;
}

s32 FutureSightDamageCalc(void)
{
	gBattleStruct->dynamicMoveType = GetMoveTypeSpecial(gBankAttacker, gCurrentMove);
	u16 side_hword = gSideAffecting[SIDE(gBankTarget)];
	gBattleMoveDamage = CalculateBaseDamage(&gBattleMons[gBankAttacker], &gBattleMons[gBankTarget], gCurrentMove,
											side_hword, gDynamicBasePower,
											TypeCalc(gCurrentMove, gBankAttacker, gBankTarget, 0, FALSE),
											gBattleStruct->dynamicMoveType, gBankAttacker, gBankTarget,
											GetBankPartyData(gBankAttacker), FALSE, TRUE, FALSE);
	gBattleMoveDamage = gBattleMoveDamage * (gCritMultiplier / BASE_CRIT_MULTIPLIER);
	return gBattleMoveDamage;
}

u32 AI_CalcDmg(const u8 bankAtk, const u8 bankDef, const u16 move) {
	u32 damage = 0;
	u8 resultFlags = TypeCalc(move, bankAtk, bankDef, 0, FALSE);

	if (SPLIT(move) == SPLIT_STATUS || resultFlags & MOVE_RESULT_NO_EFFECT)
		return 0;

	switch (gBattleMoves[move].effect) {
		case EFFECT_SUPER_FANG:
			return gBattleMons[bankDef].hp / 2;
		case EFFECT_DRAGON_RAGE:
			return 40;
		case EFFECT_SONICBOOM:
			return 20;
		case EFFECT_LEVEL_DAMAGE:
			return gBattleMons[bankAtk].level;
		case EFFECT_PSYWAVE:
			return GetPsywaveDamage(50); //On average, 50 will be selected as the random number
		case EFFECT_MEMENTO: //Final Gambit
			return gBattleMons[bankAtk].hp;
		case EFFECT_ENDEAVOR:
			if (gBattleMons[bankDef].hp <= gBattleMons[bankAtk].hp)
				return 0;
			return gBattleMons[bankDef].hp - gBattleMons[bankAtk].hp;
		case EFFECT_PAIN_SPLIT: ;
			u16 finalHp = MathMax(1, (gBattleMons[bankAtk].hp + gBattleMons[bankDef].hp) / 2);

			if (finalHp >= gBattleMons[bankDef].hp)
				return 0;
			return gBattleMons[bankDef].hp - finalHp;
	}

	gDynamicBasePower = (IsZMove(move)) ? GetZMovePower(move) : 0;

	gBattleScripting->dmgMultiplier = 1;
	gBattleStruct->dynamicMoveType = GetMoveTypeSpecial(bankAtk, move);

	gCritMultiplier = CalcPossibleCritChance(bankAtk, bankDef, move, 0, FALSE); //Return 0 if none, 1 if always, 2 if 50%
	if (gCritMultiplier != 0 && Random() % gCritMultiplier == 0)
		gCritMultiplier = CRIT_MULTIPLIER;
	else
		gCritMultiplier = BASE_CRIT_MULTIPLIER;

	u16 side_hword = gSideAffecting[SIDE(bankDef)];
	damage = CalculateBaseDamage(&gBattleMons[bankAtk], &gBattleMons[bankDef], move,
											side_hword, gDynamicBasePower,
											resultFlags,
											gBattleStruct->dynamicMoveType, bankAtk, bankDef,
											GetBankPartyData(bankAtk), FALSE, FALSE, FALSE);

	gBattleMoveDamage = MathMin(0x7FFFFFFF, damage);
	AI_SpecialTypeCalc(move, bankAtk, bankDef);
	damage = gBattleMoveDamage;
	damage *= (gCritMultiplier / BASE_CRIT_MULTIPLIER);
	gCritMultiplier = BASE_CRIT_MULTIPLIER; //Reset

	damage = (damage * 93) / 100; //Roll 93% damage - about halfway between min & max damage

	if (CheckTableForMove(move, gTwoToFiveStrikesMoves) && ABILITY(bankAtk) == ABILITY_SKILLLINK)
	{
		damage *= 5;
		return damage;
	}
	else if (CheckTableForMove(move, gTwoStrikesMoves))
	{
		damage *= 2;
		return damage;
	}
	else if (CheckTableForMove(move, gThreeStrikesMoves) || CheckTableForMove(move, gTwoToFiveStrikesMoves)) //Three hits on average
	{
		damage *= 3;
		return damage;
	}

	//Multi hit moves skip these checks
	if (gBattleMoves[move].effect == EFFECT_FALSE_SWIPE
	|| (BATTLER_MAX_HP(bankDef) && ABILITY(bankDef) == ABILITY_STURDY && NO_MOLD_BREAKERS(ABILITY(bankAtk), move))
	|| (BATTLER_MAX_HP(bankDef) && IsBankHoldingFocusSash(bankDef)))
		damage = MathMin(damage, gBattleMons[bankDef].hp - 1);

	return damage;
}

u32 AI_CalcPartyDmg(u8 bankAtk, u8 bankDef, u16 move, struct Pokemon* mon) {
	u32 damage = 0;
	u8 resultFlags = TypeCalc(move, bankAtk, bankDef, mon, TRUE);

	if (SPLIT(move) == SPLIT_STATUS || resultFlags & MOVE_RESULT_NO_EFFECT)
		return 0;

	switch (gBattleMoves[move].effect) {
		case EFFECT_SUPER_FANG:
			return gBattleMons[bankDef].hp / 2;
		case EFFECT_DRAGON_RAGE:
			return 40;
		case EFFECT_SONICBOOM:
			return 20;
		case EFFECT_LEVEL_DAMAGE:
			return mon->level;
		case EFFECT_PSYWAVE:
			return GetPsywaveDamage(50); //On average, 50 will be selected as the random number
		case EFFECT_MEMENTO: //Final Gambit
			return mon->hp;
		case EFFECT_ENDEAVOR:
			if (gBattleMons[bankDef].hp <= mon->hp)
				return 0;
			return gBattleMons[bankDef].hp - mon->hp;
		case EFFECT_PAIN_SPLIT: ;
			u16 finalHp = MathMax(1, (mon->hp + gBattleMons[bankDef].hp) / 2);

			if (finalHp >= gBattleMons[bankDef].hp)
				return 0;
			return gBattleMons[bankDef].hp - finalHp;
	}

	gDynamicBasePower = (IsZMove(move)) ? GetZMovePower(move) : 0;

	gBattleScripting->dmgMultiplier = 1;
	gBattleStruct->dynamicMoveType = GetMoveTypeSpecialFromParty(mon, move);
	u16 side_hword = gSideAffecting[SIDE(bankDef)];
	gCritMultiplier = CalcPossibleCritChance(bankAtk, bankDef, move, mon, TRUE); //Return 0 if none, 1 if always, 2 if 50%

	if (gCritMultiplier != 0 && Random() % gCritMultiplier == 0)
		gCritMultiplier = CRIT_MULTIPLIER;
	else
		gCritMultiplier = BASE_CRIT_MULTIPLIER;

	damage = CalculateBaseDamage(&gBattleMons[0], &gBattleMons[bankDef], move,
											side_hword, resultFlags,
											TypeCalc(move, bankAtk, bankDef, mon, TRUE),
											gBattleStruct->dynamicMoveType, bankAtk, bankDef,
											mon, TRUE, FALSE, FALSE);

	gBattleMoveDamage = damage;
	TypeCalc(move, bankAtk, bankDef, mon, TRUE);
	damage = gBattleMoveDamage;
	damage *= (gCritMultiplier / BASE_CRIT_MULTIPLIER);
	gCritMultiplier = BASE_CRIT_MULTIPLIER; //Reset

	damage = (damage * 96) / 100; //Roll 96% damage with party mons - be more idealistic

	if (CheckTableForMove(move, gTwoToFiveStrikesMoves) && GetPartyAbility(mon) == ABILITY_SKILLLINK)
	{
		damage *= 5;
		return damage;
	}
	else if (CheckTableForMove(move, gTwoStrikesMoves))
	{
		damage *= 2;
		return damage;
	}
	else if (CheckTableForMove(move, gThreeStrikesMoves) || CheckTableForMove(move, gTwoToFiveStrikesMoves)) //Three hits on average
	{
		damage *= 3;
		return damage;
	}

	//Multi hit moves skip these checks
	if (gBattleMoves[move].effect == EFFECT_FALSE_SWIPE
	|| (BATTLER_MAX_HP(bankDef) && ABILITY(bankDef) == ABILITY_STURDY && NO_MOLD_BREAKERS(GetPartyAbility(mon), move))
	|| (BATTLER_MAX_HP(bankDef) && IsBankHoldingFocusSash(bankDef)))
		damage = MathMin(damage, gBattleMons[bankDef].hp - 1);

	return damage;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//This is type calc BS Command
void atk06_typecalc(void) {
	u8 moveType = gBattleStruct->dynamicMoveType & 0x3F;
	u8 atkAbility = ABILITY(gBankAttacker);
	u8 defAbility = ABILITY(gBankTarget);
	u8 defEffect = ITEM_EFFECT(gBankTarget);
	u8 atkType1 = gBattleMons[gBankAttacker].type1;
	u8 atkType2 = gBattleMons[gBankAttacker].type2;
	u8 atkType3 = gBattleMons[gBankAttacker].type3;

	if (gCurrentMove != MOVE_STRUGGLE) {
		//Check Stab
		if (atkType1 == moveType || atkType2 == moveType || atkType3 == moveType) {
			if (atkAbility == ABILITY_ADAPTABILITY)
				gBattleMoveDamage *= 2;
			else
				gBattleMoveDamage = udivsi(gBattleMoveDamage * 150, 100);
		}

		//Check Special Ground Immunities
		if (moveType == TYPE_GROUND && !CheckGrounding(gBankTarget) && gCurrentMove != MOVE_THOUSANDARROWS) {
			if (defAbility == ABILITY_LEVITATE) {
				gLastUsedAbility = defAbility;
				gMoveResultFlags |= (MOVE_RESULT_MISSED | MOVE_RESULT_DOESNT_AFFECT_FOE);
				gLastLandedMoves[gBankTarget] = 0;
				gLastHitByType[gBankTarget] = 0;
				gBattleCommunication[6] = moveType;
				RecordAbilityBattle(gBankTarget, gLastUsedAbility);
			}
			else if (defEffect == ITEM_EFFECT_AIR_BALLOON) {
				gMoveResultFlags |= (MOVE_RESULT_DOESNT_AFFECT_FOE);
				gLastLandedMoves[gBankTarget] = 0;
				gLastHitByType[gBankTarget] = 0;
				RecordItemEffectBattle(gBankTarget, defEffect);
			}
			else if (gStatuses3[gBankTarget] & (STATUS3_LEVITATING | STATUS3_TELEKINESIS)) {
				gMoveResultFlags |= (MOVE_RESULT_DOESNT_AFFECT_FOE);
				gLastLandedMoves[gBankTarget] = 0;
				gLastHitByType[gBankTarget] = 0;
			}
			else
				goto RE_ENTER_TYPE_CHECK;	//You're a flying type
		}

		//Check Powder Moves
		else if (CheckTableForMove(gCurrentMove, gPowderMoves)) {
			if (defAbility == ABILITY_OVERCOAT) {
				gLastUsedAbility = defAbility;
				gMoveResultFlags |= (MOVE_RESULT_MISSED | MOVE_RESULT_DOESNT_AFFECT_FOE);
				gLastLandedMoves[gBankTarget] = 0;
				gLastHitByType[gBankTarget] = 0xFF;
				gBattleCommunication[6] = 3;
				RecordAbilityBattle(gBankTarget, gLastUsedAbility);
			}
			else if (defEffect == ITEM_EFFECT_SAFETY_GOGGLES) {
				gMoveResultFlags |= (MOVE_RESULT_DOESNT_AFFECT_FOE);
				gLastLandedMoves[gBankTarget] = 0;
				gLastHitByType[gBankTarget] = 0xFF;
				RecordItemEffectBattle(gBankTarget, defEffect);
			}
			else if (IsOfType(gBankTarget, TYPE_GRASS)) {
				gMoveResultFlags |= (MOVE_RESULT_DOESNT_AFFECT_FOE);
				gLastLandedMoves[gBankTarget] = 0;
				gLastHitByType[gBankTarget] = 0xFF;
			}
			else
				goto RE_ENTER_TYPE_CHECK;
		}

		else if (gCurrentMove == MOVE_SKYDROP && IsOfType(gBankTarget, TYPE_FLYING)) {
			gMoveResultFlags |= (MOVE_RESULT_DOESNT_AFFECT_FOE);
			gLastLandedMoves[gBankTarget] = 0;
			gLastHitByType[gBankTarget] = 0xFF;
		}

		else {
		RE_ENTER_TYPE_CHECK:
			TypeDamageModification(atkAbility, gBankTarget, gCurrentMove, moveType, &gMoveResultFlags);
		}

		if (defAbility == ABILITY_WONDERGUARD
		 //&& AttacksThisTurn(gBankAttacker, gCurrentMove) == 2
		 && (!(gMoveResultFlags & MOVE_RESULT_SUPER_EFFECTIVE) || ((gMoveResultFlags & (MOVE_RESULT_SUPER_EFFECTIVE | MOVE_RESULT_NOT_VERY_EFFECTIVE)) == (MOVE_RESULT_SUPER_EFFECTIVE | MOVE_RESULT_NOT_VERY_EFFECTIVE)))
		 && SPLIT(gCurrentMove) != SPLIT_STATUS) {
			gLastUsedAbility = defAbility;
			gMoveResultFlags |= MOVE_RESULT_MISSED;
			gLastLandedMoves[gBankTarget] = 0;
			gLastHitByType[gBankTarget] = 0;
			gBattleCommunication[6] = 3;
			RecordAbilityBattle(gBankTarget, gLastUsedAbility);
		}

		if (gMoveResultFlags & MOVE_RESULT_DOESNT_AFFECT_FOE)
			gProtectStructs[gBankAttacker].targetNotAffected = 1;
	}
	++gBattlescriptCurrInstr;
}

//This is the type calc that doesn't modify the damage
void atk4A_typecalc2(void) {
	u8 moveType = gBattleStruct->dynamicMoveType & 0x3F;
	u8 atkAbility = ABILITY(gBankAttacker);
	u8 defAbility = ABILITY(gBankTarget);
	u8 defEffect = ITEM_EFFECT(gBankTarget);

	//Check Special Ground Immunities
	if (moveType == TYPE_GROUND && !CheckGrounding(gBankTarget) && gCurrentMove != MOVE_THOUSANDARROWS) {
		if (defAbility == ABILITY_LEVITATE) {
			gLastUsedAbility = atkAbility;
			gMoveResultFlags |= (MOVE_RESULT_MISSED | MOVE_RESULT_DOESNT_AFFECT_FOE);
			gLastLandedMoves[gBankTarget] = 0;
			gBattleCommunication[6] = moveType;
			RecordAbilityBattle(gBankTarget, gLastUsedAbility);
		}
		else if (defEffect == ITEM_EFFECT_AIR_BALLOON) {
			gMoveResultFlags |= (MOVE_RESULT_DOESNT_AFFECT_FOE);
			gLastLandedMoves[gBankTarget] = 0;
			RecordItemEffectBattle(gBankTarget, defEffect);
		}
		else if (gStatuses3[gBankTarget] & (STATUS3_LEVITATING | STATUS3_TELEKINESIS)) {
			gMoveResultFlags |= (MOVE_RESULT_DOESNT_AFFECT_FOE);
			gLastLandedMoves[gBankTarget] = 0;
		}
		else
			goto RE_ENTER_TYPE_CHECK_2;	//You're a flying type
	}

	else if (CheckTableForMove(gCurrentMove, gPowderMoves)) {
		if (defAbility == ABILITY_OVERCOAT) {
			gLastUsedAbility = defAbility;
			gMoveResultFlags |= (MOVE_RESULT_MISSED | MOVE_RESULT_DOESNT_AFFECT_FOE);
			gLastLandedMoves[gBankTarget] = 0;
			gBattleCommunication[6] = moveType;
			RecordAbilityBattle(gBankTarget, gLastUsedAbility);
		}
		else if (defEffect == ITEM_EFFECT_SAFETY_GOGGLES) {
			gMoveResultFlags |= (MOVE_RESULT_DOESNT_AFFECT_FOE);
			gLastLandedMoves[gBankTarget] = 0;
			RecordItemEffectBattle(gBankTarget, defEffect);
		}
		else if (IsOfType(gBankTarget, TYPE_GRASS)) {
			gMoveResultFlags |= (MOVE_RESULT_DOESNT_AFFECT_FOE);
			gLastLandedMoves[gBankTarget] = 0;
		}
		else
			goto RE_ENTER_TYPE_CHECK_2;
	}

	else if (gCurrentMove == MOVE_SKYDROP && IsOfType(gBankTarget, TYPE_FLYING)) {
		gMoveResultFlags |= (MOVE_RESULT_DOESNT_AFFECT_FOE);
		gLastLandedMoves[gBankTarget] = 0;
	}

	else {
	RE_ENTER_TYPE_CHECK_2:	;
		s32 backupDamage = gBattleMoveDamage;
		TypeDamageModification(atkAbility, gBankTarget, gCurrentMove, moveType, &gMoveResultFlags);
		gBattleMoveDamage = backupDamage;
	}

	if (defAbility == ABILITY_WONDERGUARD
	&& AttacksThisTurn(gBankAttacker, gCurrentMove) == 2
	&& (!(gMoveResultFlags & MOVE_RESULT_SUPER_EFFECTIVE) || ((gMoveResultFlags & (MOVE_RESULT_SUPER_EFFECTIVE | MOVE_RESULT_NOT_VERY_EFFECTIVE)) == (MOVE_RESULT_SUPER_EFFECTIVE | MOVE_RESULT_NOT_VERY_EFFECTIVE)))
	&& gBattleMoves[gCurrentMove].power) {
		gLastUsedAbility = defAbility;
		gMoveResultFlags |= MOVE_RESULT_MISSED;
		gLastLandedMoves[gBankTarget] = 0;
		gBattleCommunication[6] = 3;
		RecordAbilityBattle(gBankTarget, gLastUsedAbility);
	}

	if (gMoveResultFlags & MOVE_RESULT_DOESNT_AFFECT_FOE)
		gProtectStructs[gBankAttacker].targetNotAffected = 1;

	++gBattlescriptCurrInstr;
}

//This is the type calc that is callable as a function
//This calc now also contains the calc the AI uses with its party pokemon to help with switching
u8 TypeCalc(u16 move, u8 bankAtk, u8 bankDef, struct Pokemon* monAtk, bool8 CheckParty) {
	u8 moveType;
	u8 defAbility = ABILITY(bankDef);
	u8 defEffect = ITEM_EFFECT(bankDef);
	u8 atkAbility, atkType1, atkType2, atkType3;
	u8 flags = 0;

	if (move == MOVE_STRUGGLE)
		return 0;

	if (CheckParty) {
		atkAbility = GetPartyAbility(monAtk);
		atkType1 = (gBattleTypeFlags & BATTLE_TYPE_CAMOMONS) ? GetCamomonsTypeByMon(monAtk, 0) : gBaseStats[monAtk->species].type1;
		atkType2 = (gBattleTypeFlags & BATTLE_TYPE_CAMOMONS) ? GetCamomonsTypeByMon(monAtk, 1) : gBaseStats[monAtk->species].type2;
		atkType3 = TYPE_BLANK;
		moveType = GetMoveTypeSpecialFromParty(monAtk, move);
	}
	else {
		atkAbility = ABILITY(bankAtk);
		atkType1 = gBattleMons[bankAtk].type1;
		atkType2 = gBattleMons[bankAtk].type2;
		atkType3 = gBattleMons[bankAtk].type3;
		moveType = GetMoveTypeSpecial(bankAtk, move);
	}

	//Check stab
	if (atkType1 == moveType || atkType2 == moveType || atkType3 == moveType) {
		if (atkAbility == ABILITY_ADAPTABILITY)
			gBattleMoveDamage *= 2;
		else
			gBattleMoveDamage = udivsi(gBattleMoveDamage * 150, 100);
	}

	//Check Special Ground Immunities
	if (moveType == TYPE_GROUND
	&& !CheckGrounding(bankDef)
	&& ((defAbility == ABILITY_LEVITATE && NO_MOLD_BREAKERS(atkAbility, move)) || defEffect == ITEM_EFFECT_AIR_BALLOON || (gStatuses3[bankDef] & (STATUS3_LEVITATING | STATUS3_TELEKINESIS)))
	&& move != MOVE_THOUSANDARROWS)
		flags |= (MOVE_RESULT_MISSED | MOVE_RESULT_DOESNT_AFFECT_FOE);

	else if (CheckTableForMove(move, gPowderMoves) && !IsAffectedByPowder(bankDef))
		flags |= (MOVE_RESULT_MISSED | MOVE_RESULT_DOESNT_AFFECT_FOE);

	else if (move == MOVE_SKYDROP && IsOfType(bankDef, TYPE_FLYING))
		flags |= (MOVE_RESULT_DOESNT_AFFECT_FOE);

	//Regular Type Calc
	else
		TypeDamageModification(atkAbility, bankDef, move, moveType, &flags);

	//Wonder Guard Check
	if (defAbility == ABILITY_WONDERGUARD
	&& NO_MOLD_BREAKERS(atkAbility, move)
	&& !(flags & MOVE_RESULT_MISSED)
	&& AttacksThisTurn(bankAtk, move) == 2
	&& (!(flags & MOVE_RESULT_SUPER_EFFECTIVE) || ((flags & (MOVE_RESULT_SUPER_EFFECTIVE | MOVE_RESULT_NOT_VERY_EFFECTIVE)) == (MOVE_RESULT_SUPER_EFFECTIVE | MOVE_RESULT_NOT_VERY_EFFECTIVE)))
	&& gBattleMoves[move].power
	&& SPLIT(move) != SPLIT_STATUS)
		flags |= MOVE_RESULT_MISSED;

	return flags;
}

//The function allows the AI to do type calculations from a move onto one of their partied mons
u8 AI_TypeCalc(u16 move, u8 bankAtk, struct Pokemon* monDef) {
	u8 flags = 0;

	u8 defAbility = GetPartyAbility(monDef);
	u8 defEffect = ItemId_GetHoldEffectParam(monDef->item);
	u8 defType1 = (gBattleTypeFlags & BATTLE_TYPE_CAMOMONS) ? GetCamomonsTypeByMon(monDef, 0) : gBaseStats[monDef->species].type1;
	u8 defType2 = (gBattleTypeFlags & BATTLE_TYPE_CAMOMONS) ? GetCamomonsTypeByMon(monDef, 1) : gBaseStats[monDef->species].type2;

	u8 atkAbility = ABILITY(bankAtk);
	u8 atkType1 = gBattleMons[bankAtk].type1;
	u8 atkType2 = gBattleMons[bankAtk].type2;
	u8 atkType3 = gBattleMons[bankAtk].type3;
	u8 moveType;

	if (move == MOVE_STRUGGLE)
		return 0;

	moveType = GetMoveTypeSpecial(bankAtk, move);

	//Check stab
	if (atkType1 == moveType || atkType2 == moveType || atkType3 == moveType) {
		if (atkAbility == ABILITY_ADAPTABILITY)
			gBattleMoveDamage *= 2;
		else
			gBattleMoveDamage = udivsi(gBattleMoveDamage * 150, 100);
	}

	//Check Special Ground Immunities
	if (moveType == TYPE_GROUND
	&& !CheckGroundingFromPartyData(monDef)
	&& ((defAbility == ABILITY_LEVITATE && NO_MOLD_BREAKERS(atkAbility, move)) || (defEffect == ITEM_EFFECT_AIR_BALLOON && defAbility != ABILITY_KLUTZ))
	&& move != MOVE_THOUSANDARROWS)
		flags = MOVE_RESULT_MISSED | MOVE_RESULT_DOESNT_AFFECT_FOE;

	else if (CheckTableForMove(move, gPowderMoves)
	&& (defAbility == ABILITY_OVERCOAT || defEffect == ITEM_EFFECT_SAFETY_GOGGLES || defType1 == TYPE_GRASS || defType2 == TYPE_GRASS))
		flags |= (MOVE_RESULT_MISSED | MOVE_RESULT_DOESNT_AFFECT_FOE);

	else if (move == MOVE_SKYDROP && (defType1 == TYPE_FLYING || defType2 == TYPE_FLYING))
		flags |= (MOVE_RESULT_DOESNT_AFFECT_FOE);

	else
		TypeDamageModificationPartyMon(atkAbility, monDef, move, moveType, &flags);

	if (defAbility == ABILITY_WONDERGUARD
	 && (!(flags & MOVE_RESULT_SUPER_EFFECTIVE) || ((flags & (MOVE_RESULT_SUPER_EFFECTIVE | MOVE_RESULT_NOT_VERY_EFFECTIVE)) == (MOVE_RESULT_SUPER_EFFECTIVE | MOVE_RESULT_NOT_VERY_EFFECTIVE)))
	 && gBattleMoves[move].power)
		flags |= MOVE_RESULT_DOESNT_AFFECT_FOE;
	return flags;
}

//This calc takes into account things like Pokemon Mega Evolving, Protean, & Illusion
u8 AI_SpecialTypeCalc(u16 move, u8 bankAtk, u8 bankDef)
{
	u8 moveType;
	u8 atkAbility = GetAIAbility(bankAtk, bankDef, move);
	u8 defAbility = GetAIAbility(bankDef, bankAtk, IsValidMovePrediction(bankDef, bankAtk));
	u8 defEffect = ITEM_EFFECT(bankDef);
	u8 atkType1, atkType2, atkType3, defType1, defType2, defType3;
	u8 flags = 0;

	if (move == MOVE_STRUGGLE)
		return 0;

	atkType1 = gBattleMons[bankAtk].type1;
	atkType2 = gBattleMons[bankAtk].type2;
	atkType3 = gBattleMons[bankAtk].type3;
	moveType = GetMoveTypeSpecial(bankAtk, move);

	if (atkAbility == ABILITY_PROTEAN)
		atkType1 = atkType2 = atkType3 = moveType;

	if (gStatuses3[bankDef] & STATUS3_ILLUSION && gDisableStructs[bankDef].isFirstTurn) //Under illusion and haven't figured it out yet
	{
		struct Pokemon* illusionMon = GetIllusionPartyData(bankDef);
		u16 fakeSpecies = GetMonData(illusionMon, MON_DATA_SPECIES, NULL);
		defAbility = GetPartyAbility(illusionMon);
		defType1 = gBaseStats[fakeSpecies].type1;
		defType2 = gBaseStats[fakeSpecies].type2;
	}
	else
	{
		defType1 = gBattleMons[bankDef].type1;
		defType2 = gBattleMons[bankDef].type2;
	}
	defType3 = gBattleMons[bankDef].type3; //Same type 3 - eg switched in on Forest's curse

	//Check STAB
	if (atkType1 == moveType || atkType2 == moveType || atkType3 == moveType)
	{
		if (atkAbility == ABILITY_ADAPTABILITY)
			gBattleMoveDamage *= 2;
		else
			gBattleMoveDamage = (gBattleMoveDamage * 150) / 100;
	}

	//Check Special Ground Immunities
	if (moveType == TYPE_GROUND
	&& !CheckGrounding(bankDef)
	&& ((defAbility == ABILITY_LEVITATE && NO_MOLD_BREAKERS(atkAbility, move)) || defEffect == ITEM_EFFECT_AIR_BALLOON || (gStatuses3[bankDef] & (STATUS3_LEVITATING | STATUS3_TELEKINESIS)))
	&& move != MOVE_THOUSANDARROWS)
		flags |= (MOVE_RESULT_MISSED | MOVE_RESULT_DOESNT_AFFECT_FOE);

	else if (CheckTableForMove(move, gPowderMoves) && !IsAffectedByPowderByDetails(defType1, defType2, defType3, defAbility, defEffect))
		flags |= (MOVE_RESULT_MISSED | MOVE_RESULT_DOESNT_AFFECT_FOE);

	else if (move == MOVE_SKYDROP && (defType1 == TYPE_FLYING || defType2 == TYPE_FLYING || defType3 == TYPE_FLYING))
		flags |= (MOVE_RESULT_DOESNT_AFFECT_FOE);

	//Regular Type Calc
	else
		TypeDamageModificationByDefTypes(atkAbility, bankDef, move, moveType, &flags, defType1, defType2, defType3);

	//Wonder Guard Check
	if (defAbility == ABILITY_WONDERGUARD
	&& NO_MOLD_BREAKERS(atkAbility, move)
	&& !(flags & MOVE_RESULT_MISSED)
	&& AttacksThisTurn(bankAtk, move) == 2
	&& (!(flags & MOVE_RESULT_SUPER_EFFECTIVE) || ((flags & (MOVE_RESULT_SUPER_EFFECTIVE | MOVE_RESULT_NOT_VERY_EFFECTIVE)) == (MOVE_RESULT_SUPER_EFFECTIVE | MOVE_RESULT_NOT_VERY_EFFECTIVE)))
	&& gBattleMoves[move].power
	&& SPLIT(move) != SPLIT_STATUS)
		flags |= MOVE_RESULT_MISSED;

	if (flags & MOVE_RESULT_NO_EFFECT)
		gBattleMoveDamage = 0;

	return flags;
}

//The TypeCalc for showing move effectiveness on the move menu
u8 VisualTypeCalc(u16 move, u8 bankAtk, u8 bankDef)
{
	u8 moveType;
	u8 defAbility = GetRecordedAbility(bankDef);
	u8 defEffect = GetRecordedItemEffect(bankDef);
	u8 atkAbility, defType1, defType2, defType3;
	u8 flags = 0;

	if (move == MOVE_STRUGGLE)
		return 0;

	atkAbility = ABILITY(bankAtk);
	moveType = GetMoveTypeSpecial(bankAtk, move);

	struct Pokemon* monIllusion = GetIllusionPartyData(bankDef);
	if (monIllusion != GetBankPartyData(bankDef)) //Under illusion
	{
		u16 species = GetMonData(monIllusion, MON_DATA_SPECIES, NULL);
		defType1 = (gBattleTypeFlags & BATTLE_TYPE_CAMOMONS) ? GetCamomonsTypeByMon(monIllusion, 0) : gBaseStats[species].type1;
		defType2 = (gBattleTypeFlags & BATTLE_TYPE_CAMOMONS) ? GetCamomonsTypeByMon(monIllusion, 0) : gBaseStats[species].type2;
		defType3 = TYPE_BLANK;
	}
	else
	{
		defType1 = gBattleMons[bankDef].type1;
		defType2 = gBattleMons[bankDef].type2;
		defType3 = gBattleMons[bankDef].type3;
	}

	//Check Special Ground Immunities
	if (moveType == TYPE_GROUND
	&& !NonInvasiveCheckGrounding(bankDef)
	&& ((defAbility == ABILITY_LEVITATE && NO_MOLD_BREAKERS(atkAbility, move)) || defEffect == ITEM_EFFECT_AIR_BALLOON || (gStatuses3[bankDef] & (STATUS3_LEVITATING | STATUS3_TELEKINESIS)))
	&& move != MOVE_THOUSANDARROWS)
	{
		flags |= (MOVE_RESULT_MISSED | MOVE_RESULT_DOESNT_AFFECT_FOE);
	}
	else if (CheckTableForMove(move, gPowderMoves) && !IsAffectedByPowderByDetails(defType1, defType2, defType3, defAbility, defEffect))
	{
		flags |= (MOVE_RESULT_MISSED | MOVE_RESULT_DOESNT_AFFECT_FOE);
	}
	else if (move == MOVE_SKYDROP && (defType1 == TYPE_FLYING || defType2 == TYPE_FLYING || defType3 == TYPE_FLYING))
	{
		flags |= (MOVE_RESULT_DOESNT_AFFECT_FOE);
	}
	else //Regular Type Calc
		TypeDamageModificationByDefTypes(atkAbility, bankDef, move, moveType, &flags, defType1, defType2, defType3);

	//Wonder Guard Check
	if (defAbility == ABILITY_WONDERGUARD
	&& NO_MOLD_BREAKERS(atkAbility, move)
	&& !(flags & MOVE_RESULT_MISSED)
	&& (!(flags & MOVE_RESULT_SUPER_EFFECTIVE) || ((flags & (MOVE_RESULT_SUPER_EFFECTIVE | MOVE_RESULT_NOT_VERY_EFFECTIVE)) == (MOVE_RESULT_SUPER_EFFECTIVE | MOVE_RESULT_NOT_VERY_EFFECTIVE)))
	&& gBattleMoves[move].power
	&& SPLIT(move) != SPLIT_STATUS)
		flags |= MOVE_RESULT_MISSED;

	return flags;
}

void TypeDamageModification(u8 atkAbility, u8 bankDef, u16 move, u8 moveType, u8* flags)
{
	return TypeDamageModificationByDefTypes(atkAbility, bankDef, move, moveType, flags, gBattleMons[bankDef].type1, gBattleMons[bankDef].type2, gBattleMons[bankDef].type3);
}

static void TypeDamageModificationByDefTypes(u8 atkAbility, u8 bankDef, u16 move, u8 moveType, u8* flags, u8 defType1, u8 defType2, u8 defType3)
{
	int i = 0;
	u8 tableAtk;
	u8 tableDef;
	u8 tableMult;

	TYPE_LOOP:
		while (gTypeEffectiveness[i] != TYPE_ENDTABLE) {
			tableAtk = gTypeEffectiveness[i];
			tableDef = gTypeEffectiveness[i + 1];
			tableMult = gTypeEffectiveness[i + 2];

			if (tableAtk == TYPE_FORESIGHT) {
				if (gBattleMons[bankDef].status2 & STATUS2_FORESIGHT || atkAbility == ABILITY_SCRAPPY)
					break;
				i += 3;
				continue;
			}

			else if (tableAtk == moveType) {
				if (tableAtk == TYPE_PSYCHIC && tableDef == TYPE_DARK && (gStatuses3[bankDef] & STATUS3_MIRACLE_EYED)) {
						i += 3;
						continue;
				}

				//check type1
				if (tableDef == defType1)
					ModulateDmgByType(tableMult, move, tableAtk, tableDef, bankDef, flags, 0, FALSE);

				//check type2
				if (tableDef == defType2 && defType1 != defType2)
					ModulateDmgByType(tableMult, move, tableAtk, tableDef, bankDef, flags, 0, FALSE);

				//check type3
				if (tableDef == defType3 && defType3 != defType1 && defType3 != defType2)
					ModulateDmgByType(tableMult, move, tableAtk, tableDef, bankDef, flags, 0, FALSE);

			}
			i += 3;
		}
		if (move == MOVE_FLYINGPRESS && moveType != TYPE_FLYING) {
			moveType = TYPE_FLYING;
			i = 0;
			goto TYPE_LOOP;
		}
}


void TypeDamageModificationPartyMon(u8 atkAbility, struct Pokemon* monDef, u16 move, u8 moveType, u8* flags)
{
	int i = 0;
	u8 tableAtk;
	u8 tableDef;
	u8 tableMult;
	u8 defType1 = (gBattleTypeFlags & BATTLE_TYPE_CAMOMONS) ? GetCamomonsTypeByMon(monDef, 0) : gBaseStats[monDef->species].type1;
	u8 defType2 = (gBattleTypeFlags & BATTLE_TYPE_CAMOMONS) ? GetCamomonsTypeByMon(monDef, 1) : gBaseStats[monDef->species].type2;

TYPE_LOOP_AI:
	while (gTypeEffectiveness[i] != TYPE_ENDTABLE)
	{
		tableAtk = gTypeEffectiveness[i];
		tableDef = gTypeEffectiveness[i + 1];
		tableMult = gTypeEffectiveness[i + 2];

		if (gTypeEffectiveness[i] == TYPE_FORESIGHT)
		{
			if (atkAbility == ABILITY_SCRAPPY)
				break;

			i += 3;
			continue;
		}

		if (tableAtk == moveType)
		{
			//check type1
			if (tableDef == defType1)
				ModulateDmgByType(tableMult, move, tableAtk, tableDef, 0, flags, monDef, TRUE);

			//check type2
			if (tableDef == defType2 && defType1 != defType2)
				ModulateDmgByType(tableMult, move, tableAtk, tableDef, 0, flags, monDef, TRUE);
		}

		i += 3;
	}

	if (move == MOVE_FLYINGPRESS && moveType != TYPE_FLYING)
	{
		moveType = TYPE_FLYING;
		i = 0;
		goto TYPE_LOOP_AI;
	}
}

static void ModulateDmgByType(u8 multiplier, const u16 move, const u8 moveType, const u8 defType, const u8 defBank, u8* flags, struct Pokemon* monDef, bool8 CheckPartyDef) {

	#ifdef INVERSE_BATTLES
		if (IsInverseBattle()) {
			switch (multiplier) {
				case TYPE_MUL_NO_EFFECT:
					multiplier = TYPE_MUL_SUPER_EFFECTIVE;
					break;

				case TYPE_MUL_NOT_EFFECTIVE:
					multiplier = TYPE_MUL_SUPER_EFFECTIVE;
					break;

				case TYPE_MUL_SUPER_EFFECTIVE:
					multiplier = TYPE_MUL_NOT_EFFECTIVE;
					break;
				}
		}
	#endif

	if (move == MOVE_FREEZEDRY && defType == TYPE_WATER) //Always Super-Effective, even in Inverse Battles
		multiplier = TYPE_MUL_SUPER_EFFECTIVE;

	if (CheckPartyDef) {
		if (multiplier == TYPE_MUL_NO_EFFECT && ItemId_GetHoldEffectParam(monDef->item) == ITEM_EFFECT_RING_TARGET
		&& GetPartyAbility(monDef) != ABILITY_KLUTZ)
			multiplier = TYPE_MUL_NORMAL;
		else if (multiplier == TYPE_MUL_NO_EFFECT && moveType == TYPE_GROUND
		&& (CheckGroundingFromPartyData(monDef) || move == MOVE_THOUSANDARROWS))
			multiplier = TYPE_MUL_NORMAL;
	}
	else {
		if (multiplier == TYPE_MUL_NO_EFFECT && ITEM_EFFECT(defBank) == ITEM_EFFECT_RING_TARGET)
			multiplier = TYPE_MUL_NORMAL;
		else if (multiplier == TYPE_MUL_NO_EFFECT && moveType == TYPE_GROUND
		&& (CheckGrounding(defBank) || move == MOVE_THOUSANDARROWS))
			multiplier = TYPE_MUL_NORMAL;
	}

	switch (multiplier) {
		case TYPE_MUL_NO_EFFECT:
			*flags |= MOVE_RESULT_DOESNT_AFFECT_FOE;
			*flags &= ~(MOVE_RESULT_NOT_VERY_EFFECTIVE | MOVE_RESULT_SUPER_EFFECTIVE);
			break;

		case TYPE_MUL_NOT_EFFECTIVE:
			if (gBattleMoves[move].power && !(*flags & MOVE_RESULT_NO_EFFECT)) {
				if (*flags & MOVE_RESULT_SUPER_EFFECTIVE)
					*flags &= ~MOVE_RESULT_SUPER_EFFECTIVE;
				else
					*flags |= MOVE_RESULT_NOT_VERY_EFFECTIVE;
			}
			break;

		case TYPE_MUL_SUPER_EFFECTIVE:
			if (gBattleMoves[move].power && !(*flags & MOVE_RESULT_NO_EFFECT)) {
				if (*flags & MOVE_RESULT_NOT_VERY_EFFECTIVE)
					*flags &= ~MOVE_RESULT_NOT_VERY_EFFECTIVE;
				else
					*flags |= MOVE_RESULT_SUPER_EFFECTIVE;
			}
			break;
	}

	if (defType == TYPE_FLYING && multiplier == TYPE_MUL_SUPER_EFFECTIVE && gBattleWeather & WEATHER_AIR_CURRENT_PRIMAL && move != MOVE_STEALTHROCK)
		multiplier = TYPE_MUL_NORMAL;

	gBattleMoveDamage = MathMax(1, udivsi(gBattleMoveDamage * multiplier, 10));
}


u8 GetMoveTypeSpecial(u8 bankAtk, u16 move) {
	u8 atkAbility = ABILITY(bankAtk);
	u8 moveType = gBattleMoves[move].type;

	if (gNewBS->ElectrifyTimers[bankAtk])
		return TYPE_ELECTRIC;

	if (CheckTableForMove(move, gTypeChangeExceptionMoves))
		return GetExceptionMoveType(bankAtk, move);

//Change Normal-type Moves
	if (moveType == TYPE_NORMAL) {
		if (IsIonDelugeActive())
			return TYPE_ELECTRIC;

		if ((!gNewBS->ZMoveData->active && !gNewBS->ZMoveData->viewing) || SPLIT(move) == SPLIT_STATUS) {
			switch (atkAbility) {
				case ABILITY_REFRIGERATE:
					return TYPE_ICE;

				case ABILITY_PIXILATE:
					return TYPE_FAIRY;

				case ABILITY_AERILATE:
					return TYPE_FLYING;

				case ABILITY_GALVANIZE:
					return TYPE_ELECTRIC;
			}
		}
	}

//Change non-Normal-type moves
	else if (atkAbility == ABILITY_NORMALIZE && ((!gNewBS->ZMoveData->active && !gNewBS->ZMoveData->viewing) || SPLIT(move) == SPLIT_STATUS))
		return TYPE_NORMAL;

//Change Sound Moves
	if (CheckSoundMove(move) && atkAbility == ABILITY_LIQUIDVOICE && ((!gNewBS->ZMoveData->active && !gNewBS->ZMoveData->viewing) || SPLIT(move) == SPLIT_STATUS))
		return TYPE_WATER;

	return moveType;
}

u8 GetMoveTypeSpecialFromParty(struct Pokemon* mon, u16 move) {
	u8 atkAbility = GetPartyAbility(mon);
	u8 moveType = gBattleMoves[move].type;

	if (CheckTableForMove(move, gTypeChangeExceptionMoves))
		return GetExceptionMoveTypeFromParty(mon, move);

//Change Normal-type Moves
	if (moveType == TYPE_NORMAL) {
		switch (atkAbility) {
			case ABILITY_REFRIGERATE:
				return TYPE_ICE;

			case ABILITY_PIXILATE:
				return TYPE_FAIRY;

			case ABILITY_AERILATE:
				return TYPE_FLYING;

			case ABILITY_GALVANIZE:
				return TYPE_ELECTRIC;
		}
	}

//Change non-Normal-type moves
	else if (atkAbility == ABILITY_NORMALIZE)
		return TYPE_NORMAL;

//Change Sound Moves
	if (CheckSoundMove(move) && atkAbility == ABILITY_LIQUIDVOICE)
		return TYPE_WATER;

	return moveType;
}

static bool8 AbilityCanChangeTypeAndBoost(u8 bankAtk, u16 move) {
	u8 atkAbility = ABILITY(bankAtk);
	u8 moveType = gBattleMoves[move].type;

	if (gNewBS->ElectrifyTimers[bankAtk]
	|| CheckTableForMove(move, gTypeChangeExceptionMoves))
		return FALSE;

//Check Normal-type Moves
	if (moveType == TYPE_NORMAL) {
		if (IsIonDelugeActive())
			return FALSE;

		if ((!gNewBS->ZMoveData->active && !gNewBS->ZMoveData->viewing) || SPLIT(move) == SPLIT_STATUS) {
			switch (atkAbility) {
				case ABILITY_REFRIGERATE:
				case ABILITY_PIXILATE:
				case ABILITY_AERILATE:
				case ABILITY_GALVANIZE:
					return TRUE;
			}
		}
	}

//Check non-Normal-type moves
	else if (atkAbility == ABILITY_NORMALIZE && ((!gNewBS->ZMoveData->active && !gNewBS->ZMoveData->viewing) || SPLIT(move) == SPLIT_STATUS))
		return TRUE;

	return FALSE;
}

u8 GetExceptionMoveType(u8 bankAtk, u16 move) {
	int i;
	u8 moveType = gBattleMoves[move].type;
	u16 item = ITEM(bankAtk);
	u8 effect = ITEM_EFFECT(bankAtk);
	u8 quality = ITEM_QUALITY(bankAtk);

	switch (move) {
		case MOVE_HIDDENPOWER:
			moveType = ((gBattleMons[bankAtk].hpIV & 1)) |
						((gBattleMons[bankAtk].attackIV & 1) << 1) |
						((gBattleMons[bankAtk].defenseIV & 1) << 2) |
						((gBattleMons[bankAtk].speedIV & 1) << 3) |
						((gBattleMons[bankAtk].spAttackIV & 1) << 4) |
						((gBattleMons[bankAtk].spDefenseIV & 1) << 5);

			moveType = udivsi((15 * moveType), 63) + 1;
			if (moveType >= TYPE_MYSTERY)
				moveType++;
			break;

		case MOVE_WEATHERBALL:
			if (WEATHER_HAS_EFFECT) {
				if (gBattleWeather & WEATHER_RAIN_ANY)
					moveType = TYPE_WATER;
				else if (gBattleWeather & WEATHER_SANDSTORM_ANY)
					moveType = TYPE_ROCK;
				else if (gBattleWeather & WEATHER_SUN_ANY)
					moveType = TYPE_FIRE;
				else if (gBattleWeather & WEATHER_HAIL_ANY)
					moveType = TYPE_ICE;
				else
					moveType = TYPE_NORMAL;
			}
			break;

		case MOVE_NATURALGIFT:
			if (IsBerry(item))
			{
				for (i = 0; gNaturalGiftTable[i].berry != ITEM_TABLES_TERMIN; ++i)
				{
					if (gNaturalGiftTable[i].berry == item)
					{
						moveType = gNaturalGiftTable[i].type;
						goto BREAK_NATURAL_GIFT;
					}
				}
				moveType = TYPE_MYSTERY; //If the berry isn't in the table, it has no type
			}
			else
				moveType = TYPE_MYSTERY; //If Natural Gift fails, it has no type
		BREAK_NATURAL_GIFT:
			break;

		case MOVE_JUDGMENT:
			if (effect == ITEM_EFFECT_PLATE)
				moveType = quality;
			else
				moveType = TYPE_NORMAL;
			break;

		case MOVE_TECHNOBLAST:
			if (effect == ITEM_EFFECT_DRIVE)
				moveType = quality;
			else
				moveType = TYPE_NORMAL;
			break;

		//Based on https://bulbapedia.bulbagarden.net/wiki/Revelation_Dance_(move)
		case MOVE_REVELATIONDANCE:	;
			u8 atkType1 = gBattleMons[bankAtk].type1;
			u8 atkType2 = gBattleMons[bankAtk].type2;
			u8 atkType3 = gBattleMons[bankAtk].type3;
			if (gNewBS->DancerInProgress) {
				if (atkType1 != TYPE_MYSTERY && atkType1 != TYPE_ROOSTLESS)
					moveType = atkType1;
				else if (atkType2 != TYPE_MYSTERY && atkType2 != TYPE_ROOSTLESS)
					moveType = atkType2;
				else
					moveType = TYPE_NORMAL;
			}
			else {
				if (atkType1 != TYPE_MYSTERY && atkType1 != TYPE_ROOSTLESS)
					moveType = atkType1;
				else if (atkType2 != TYPE_MYSTERY && atkType2 != TYPE_ROOSTLESS)
					moveType = atkType2;
				else if (atkType3 != TYPE_MYSTERY && atkType3 != TYPE_ROOSTLESS && atkType3 != TYPE_BLANK)
					moveType = atkType3;
				else
					moveType = TYPE_MYSTERY;
			}
	}

	if (moveType == TYPE_NORMAL && IsIonDelugeActive())
		moveType = TYPE_ELECTRIC;

	return moveType;
}

u8 GetExceptionMoveTypeFromParty(struct Pokemon* mon, u16 move) {
	int i;
	u8 moveType = gBattleMoves[move].type;
	u8 ability = GetPartyAbility(mon);
	u16 item = mon->item;
	u8 effect = ItemId_GetHoldEffect(item);
	u8 quality = ItemId_GetHoldEffectParam(item);

	switch (move) {
		case MOVE_HIDDENPOWER:
			moveType = CalcMonHiddenPowerType(mon);
			break;

		case MOVE_WEATHERBALL:
			if (WEATHER_HAS_EFFECT) {
				if (gBattleWeather & WEATHER_RAIN_ANY)
					moveType = TYPE_WATER;
				else if (gBattleWeather & WEATHER_SANDSTORM_ANY)
					moveType = TYPE_ROCK;
				else if (gBattleWeather & WEATHER_SUN_ANY)
					moveType = TYPE_FIRE;
				else if (gBattleWeather & WEATHER_HAIL_ANY)
					moveType = TYPE_ICE;
				else
					moveType = TYPE_NORMAL;
			}
			break;

		case MOVE_NATURALGIFT:
			if (IsBerry(item))
			{
				for (i = 0; gNaturalGiftTable[i].berry != ITEM_TABLES_TERMIN; ++i)
				{
					if (gNaturalGiftTable[i].berry == item)
					{
						moveType = gNaturalGiftTable[i].type;
						goto NATURAL_GIFT_BREAK;
					}
				}
				moveType = TYPE_MYSTERY; //If the berry isn't in the table, it has no type
			}
			else
				moveType = TYPE_MYSTERY; //If Natural Gift fails, it has no type
		NATURAL_GIFT_BREAK:
			break;

		case MOVE_JUDGMENT:
			if (effect == ITEM_EFFECT_PLATE && ability != ABILITY_KLUTZ)
				moveType = quality;
			else
				moveType = TYPE_NORMAL;
			break;

		case MOVE_TECHNOBLAST:
			if (effect == ITEM_EFFECT_DRIVE && ability != ABILITY_KLUTZ)
				moveType = quality;
			else
				moveType = TYPE_NORMAL;
			break;

		case MOVE_REVELATIONDANCE:
			moveType = (gBattleTypeFlags & BATTLE_TYPE_CAMOMONS) ? GetCamomonsTypeByMon(mon, 0) : gBaseStats[mon->species].type1;
	}

	return moveType;
}

u8 CalcMonHiddenPowerType(struct Pokemon* mon)
{
	u8 moveType;

	moveType = ((mon->hpIV & 1)) |
			   ((mon->attackIV & 1) << 1) |
			   ((mon->defenseIV & 1) << 2) |
			   ((mon->speedIV & 1) << 3) |
			   ((mon->spAttackIV & 1) << 4) |
			   ((mon->spDefenseIV & 1) << 5);

	moveType = ((15 * moveType) / 63) + 1;
	if (moveType >= TYPE_MYSTERY)
		++moveType;

	return moveType;
}

// Unhidden Power function
u8 GetSummaryScreenMoveType(u16 move, struct Pokemon* mon) {
#ifdef DISPLAY_REAL_MOVE_TYPE_ON_MENU
	return GetMoveTypeSpecialFromParty(mon, move);
#else
	++mon; //So no compiler errors
	return gBattleMoves[move].type;
#endif
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void atk07_adjustnormaldamage(void) {
	ApplyRandomDmgMultiplier();
	AdjustDamage(TRUE);
}

 void atk08_adjustnormaldamage2(void) {
	ApplyRandomDmgMultiplier();
	AdjustDamage(FALSE);
}

void atk69_adjustsetdamage(void) {
	AdjustDamage(TRUE);
}

void AdjustDamage(bool8 CheckFalseSwipe) {
	u16 item = gBattleMons[gBankTarget].item;
	u8 hold_effect = ITEM_EFFECT(gBankTarget);
	u8 quality = ItemId_GetHoldEffectParam(item);
	u8 ability = ABILITY(gBankTarget);
	gStringBank = gBankTarget;

	if (gNewBS->ZMoveData->active && ProtectsAgainstZMoves(gCurrentMove, gBankAttacker, gBankTarget))
		gBattleMoveDamage = udivsi(gBattleMoveDamage  * 25, 100);

	if (gBattleMons[gBankTarget].status2 & STATUS2_SUBSTITUTE
	&& !CheckSoundMove(gCurrentMove)
	&& ABILITY(gBankAttacker) != ABILITY_INFILTRATOR)
		goto END;

	if (ability == ABILITY_STURDY && gBattleMons[gBankTarget].hp == gBattleMons[gBankTarget].maxHP)
	{
		RecordAbilityBattle(gBankTarget, ability);
		gProtectStructs[gBankTarget].enduredSturdy = 1;
		gNewBS->EnduranceHelper = ENDURE_STURDY;
	}
	else if (hold_effect == ITEM_EFFECT_FOCUS_BAND && umodsi(Random(), 100) < quality && !IsBankHoldingFocusSash(gBankTarget))
	{
		RecordItemEffectBattle(gBankTarget, hold_effect);
		gSpecialStatuses[gBankTarget].focusBanded = 1;
	}
	else if (IsBankHoldingFocusSash(gBankTarget) && BATTLER_MAX_HP(gBankTarget))
	{
		RecordItemEffectBattle(gBankTarget, hold_effect);
		gSpecialStatuses[gBankTarget].focusBanded = 1;
		gNewBS->EnduranceHelper = ENDURE_FOCUS_SASH;
	}

	if (CheckFalseSwipe) {
		if (gBattleMoves[gCurrentMove].effect != EFFECT_FALSE_SWIPE
		&& !gProtectStructs[gBankTarget].endured
		&& !gProtectStructs[gBankTarget].enduredSturdy
		&& !gSpecialStatuses[gBankTarget].focusBanded)
			goto END;
	}
	else {
		if (!gProtectStructs[gBankTarget].endured
		&& !gProtectStructs[gBankTarget].enduredSturdy
		&& !gSpecialStatuses[gBankTarget].focusBanded)
		goto END;
	}

	if (gBattleMons[gBankTarget].hp > gBattleMoveDamage)
		goto END;

	gBattleMoveDamage = gBattleMons[gBankTarget].hp - 1;

	if (gProtectStructs[gBankTarget].endured || gProtectStructs[gBankTarget].enduredSturdy) {
		gMoveResultFlags |= MOVE_RESULT_FOE_ENDURED;
		goto END;
	}

	if (gSpecialStatuses[gBankTarget].focusBanded) {
		gMoveResultFlags |= MOVE_RESULT_FOE_HUNG_ON;
		gLastUsedItem = item;
	}

	END:
		++gBattlescriptCurrInstr;
}

s32 CalculateBaseDamage(struct BattlePokemon* attacker, struct BattlePokemon* defender, const u16 move, u16 sideStatus, u16 powerOverride, u8 effectivenessFlags, u8 typeOverride, u8 bankAtk, u8 bankDef, struct Pokemon* monAtk, bool8 partyCheck, bool8 IgnoreAttacker, bool8 CheckingConfusion) {
	u16 mon;
	u32 damage = 0;  //Damage is usually signed, but is set as unsigned here to help prevent overflow just in case it happens (although it's unlikely)
	u8 type;
	u32 attack, defense;
	u32 spAttack, spDefense;
	ability_t atkAbility, atkPartner_ability, defAbility, defPartner_ability;
	item_effect_t atkEffect, target_effect;
	u8 atkQuality, target_quality;
	u16 attacker_hp, attacker_maxHP;
	u32 attackerStatus1;
	u16 atkSpecies, defSpecies;
	u16 attacker_item;
	u8 moveSplit;

	defAbility = ABILITY(bankDef);
	defPartner_ability = ABILITY(PARTNER(bankDef));
	target_effect = ITEM_EFFECT(bankDef);
	target_quality = ITEM_QUALITY(bankDef);
	defSpecies = defender->species;

	if (partyCheck) { //Load data from elsewhere if we're calculating damage for a partied mon
		atkAbility = GetPartyAbility(monAtk);
		atkPartner_ability = 0;
		attacker_hp = monAtk->hp;
		attacker_maxHP = monAtk->maxHP;
		attackerStatus1 = monAtk->condition;
		attacker_item = monAtk->item;
		if (atkAbility != ABILITY_KLUTZ)
			atkEffect = ItemId_GetHoldEffect(monAtk->item);
		else
			atkEffect = 0;
		atkQuality = ItemId_GetHoldEffectParam(monAtk->item);
		atkSpecies = monAtk->species;
		moveSplit = CalcMoveSplitFromParty(monAtk, move);
	}
	else {
		atkAbility = ABILITY(bankAtk);
		atkPartner_ability = ABILITY(PARTNER(bankAtk));
		attacker_hp = attacker->hp;
		attacker_maxHP = attacker->maxHP;
		attackerStatus1 = attacker->status1;
		attacker_item = attacker->item;
		atkEffect = ITEM_EFFECT(bankAtk);
		atkQuality = ITEM_QUALITY(bankAtk);
		atkSpecies = attacker->species;
		moveSplit = CalcMoveSplit(bankAtk, move);
	}

//Load Correct Power and Type
	if (gNewBS->ZMoveData->active)
	{
		if (gNewBS->aiZMoveHelper == MOVE_NONE) //Would be set by moves like Mirror Move & Me First
			gNewBS->aiZMoveHelper = gBattleMons[bankAtk].moves[gBattleStruct->chosenMovePositions[bankAtk]];

		gBattleMovePower = GetZMovePower(move);
	}
	else if (!powerOverride)
		gBattleMovePower = GetBasePower(bankAtk, bankDef, move, attacker_item, atkEffect, atkAbility, attackerStatus1, attacker_hp, attacker_maxHP, atkSpecies, monAtk, partyCheck, FALSE, FALSE);
	else
		gBattleMovePower = powerOverride;

	//if (!typeOverride)
	//	type = gBattleMoves[move].type;
	//else
		type = typeOverride & 0x3F;

//Load the Correct Stats
	if (move == MOVE_FOULPLAY) {
		attack = defender->attack;
		spAttack = defender->spAttack;
	}
	else {
		if (partyCheck) {
			attack = monAtk->attack;
			spAttack = monAtk->spAttack;
		}
		else {
			attack = attacker->attack;
			spAttack = attacker->spAttack;
		}
	}

	if (IsWonderRoomActive())
	{
		defense = defender->spDefense;
		spDefense = defender->defense;
	}
	else
	{
		defense = defender->defense;
		spDefense = defender->spDefense;
	}

//Attacker Ability Checks
	if (!IgnoreAttacker) { //Find out if this is necessary for Future Sight
		switch(atkAbility) {
			case ABILITY_HUGEPOWER:
			case ABILITY_PUREPOWER:
				attack *= 2;
				break;

			case ABILITY_FLOWERGIFT:
				if (WEATHER_HAS_EFFECT && (gBattleWeather & WEATHER_SUN_ANY))
					attack = udivsi((attack * 150), 100);
				break;

			case ABILITY_PLUS:
			case ABILITY_MINUS:
				if ((gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
				&& (atkPartner_ability == ABILITY_PLUS || atkPartner_ability == ABILITY_MINUS))
					spAttack = udivsi((spAttack * 150), 100);
				break;

			case ABILITY_HUSTLE:
				attack = udivsi((attack * 150), 100);
				break;

			case ABILITY_GUTS:
				if (attackerStatus1 & STATUS_ANY)
					attack = udivsi((attack * 150), 100);
				break;

			case ABILITY_SOLARPOWER:
				if (WEATHER_HAS_EFFECT && (gBattleWeather & WEATHER_SUN_ANY))
					spAttack = udivsi((spAttack * 150), 100);
				break;

			case ABILITY_SLOWSTART:
				if (partyCheck)
					attack /= 2; //Pokemon in the party would start with half atk
				else if (gNewBS->SlowStartTimers[bankAtk])
					attack /= 2;
				break;

			case ABILITY_DEFEATIST:
				if (attacker_hp <= (attacker_maxHP / 2)) {
					attack /= 2;
					spAttack /= 2;
				}
				break;

			case ABILITY_TECHNICIAN:
				if (!CheckingConfusion && gBattleMovePower <= 60) {
					attack = udivsi((attack * 150), 100);
					spAttack = udivsi((spAttack * 150), 100);
				}
				break;

			case ABILITY_FLASHFIRE:
				if (!partyCheck && (gBattleResources->flags->flags[bankAtk] & 1) && type == TYPE_FIRE)
					spAttack = udivsi((spAttack * 150), 100);
				break;

			case ABILITY_OVERGROW:
				if (type == TYPE_GRASS && attacker_hp <= udivsi(attacker_maxHP, 3)) {
					attack = udivsi((attack * 150), 100);
					spAttack = udivsi((spAttack * 150), 100);
				}
				break;
			case ABILITY_BLAZE:
				if (type == TYPE_FIRE && attacker_hp <= udivsi(attacker_maxHP, 3)) {
					attack = udivsi((attack * 150), 100);
					spAttack = udivsi((spAttack * 150), 100);
				}
				break;
			case ABILITY_TORRENT:
				if (type == TYPE_WATER && attacker_hp <= udivsi(attacker_maxHP, 3)) {
					attack = udivsi((attack * 150), 100);
					spAttack = udivsi((spAttack * 150), 100);
				}
				break;
			case ABILITY_SWARM:
				if (type == TYPE_BUG && attacker_hp <= udivsi(attacker_maxHP, 3)) {
					attack = udivsi((attack * 150), 100);
					spAttack = udivsi((spAttack * 150), 100);
				}
				break;

			case ABILITY_RIVALRY:	;
				u8 attackerGender;
				u8 targetGender = GetGenderFromSpeciesAndPersonality(defSpecies, defender->personality);

				if (partyCheck)
					attackerGender = GetGenderFromSpeciesAndPersonality(atkSpecies, monAtk->personality);
				else
					attackerGender = GetGenderFromSpeciesAndPersonality(atkSpecies, attacker->personality);

				if (!CheckingConfusion && attackerGender != 0xFF && targetGender != 0xFF) {
					if (attackerGender == targetGender) {
						attack = udivsi((attack * 125), 100);
						spAttack = udivsi((spAttack * 125), 100);
					}
					else {
						attack = udivsi((attack * 75), 100);
						spAttack = udivsi((spAttack * 75), 100);
					}
				}
				break;

			case ABILITY_RECKLESS:
				if (CheckTableForMove(move, gRecklessBoostedMoves)) {
					attack = udivsi((attack * 120), 100);
					spAttack = udivsi((spAttack * 120), 100);
				}
				break;

			case ABILITY_IRONFIST:
				if (CheckTableForMove(move, gPunchingMoves)) {
					attack = udivsi((attack * 120), 100);
					spAttack = udivsi((spAttack * 120), 100);
				}
				break;

			case ABILITY_TOXICBOOST:
				if (attackerStatus1 & STATUS_PSN_ANY)
					attack = udivsi((attack * 150), 100);
				break;

			case ABILITY_FLAREBOOST:
				if (attackerStatus1 & STATUS_BURN)
					spAttack = udivsi((attack * 150), 100);
				break;

			case ABILITY_SANDFORCE:
				if (gBattleWeather & WEATHER_SANDSTORM_ANY &&
					(type == TYPE_ROCK || type == TYPE_GROUND || type == TYPE_STEEL)) {
						attack = udivsi((attack * 130), 100);
						spAttack = udivsi((spAttack * 130), 100);
				}
				break;

			case ABILITY_SHEERFORCE:
				if (CheckTableForMove(move, gSheerForceBoostedMoves)) {
					attack = udivsi((attack * 130), 100);
					spAttack = udivsi((spAttack * 130), 100);
				}
				break;

			case ABILITY_AERILATE:
			case ABILITY_PIXILATE:
			case ABILITY_REFRIGERATE:
			case ABILITY_GALVANIZE:
			case ABILITY_NORMALIZE:
				if (AbilityCanChangeTypeAndBoost(bankAtk, move)) {
					#ifdef OLD_ATE_BOOST
						attack = udivsi((attack * 130), 100);
						spAttack = udivsi((spAttack * 130), 100);
					#else
						attack = udivsi((attack * 120), 100);
						spAttack = udivsi((spAttack * 120), 100);
					#endif
				}
				break;

			case ABILITY_MEGALAUNCHER:
				if (CheckTableForMove(move, gPulseAuraMoves)) {
					attack = udivsi((attack * 150), 100);
					spAttack = udivsi((spAttack * 150), 100);
				}
				break;

			case ABILITY_STRONGJAW:
				if (CheckTableForMove(move, gBitingMoves)) {
					attack = udivsi((attack * 150), 100);
					spAttack = udivsi((spAttack * 150), 100);
				}
				break;

			case ABILITY_TOUGHCLAWS:
				if (gBattleMoves[move].flags & FLAG_MAKES_CONTACT && atkEffect != ITEM_EFFECT_PROTECTIVE_PADS) {
					attack = udivsi((attack * 130), 100);
					spAttack = udivsi((spAttack * 130), 100);
				}
				break;

			case ABILITY_NEUROFORCE:
				if (effectivenessFlags & MOVE_RESULT_SUPER_EFFECTIVE) {
					attack = udivsi((attack * 125), 100);
					spAttack = udivsi((spAttack * 125), 100);
				}
				break;

			case ABILITY_STAKEOUT:
				if (!partyCheck && gNewBS->StakeoutCounters[bankDef]) {
					attack *= 2;
					spAttack *= 2;
				}
				break;

			case ABILITY_STEELWORKER:
				if (type == TYPE_STEEL) {
					attack = udivsi((attack * 150), 100);
					spAttack = udivsi((spAttack * 150), 100);
				}
				break;

			case ABILITY_WATERBUBBLE:
				if (type == TYPE_WATER) {
					attack *= 2;
					spAttack *= 2;
				}
				break;

			case ABILITY_ANALYTIC:
				if (!CheckingConfusion && BankMovedBefore(bankDef, bankAtk)) {
					attack = udivsi((attack * 130), 100);
					spAttack = udivsi((spAttack * 130), 100);
				}
		}


		switch (atkPartner_ability) {
			case ABILITY_FLOWERGIFT:
				if (WEATHER_HAS_EFFECT && (gBattleWeather & WEATHER_SUN_ANY))
					attack = udivsi((attack * 150), 100);
				break;

			case ABILITY_BATTERY:
				spAttack = udivsi((spAttack * 130), 100);
		}
	}


//Target Ability Checks
	switch (defAbility) {
		case ABILITY_MARVELSCALE:
			if (defender->status1 & STATUS_ANY)
				defense = udivsi((defense * 150), 100);
			break;

		case ABILITY_GRASSPELT:
			if (gTerrainType == GRASSY_TERRAIN)
				defense = udivsi((defense * 150), 100);
			break;

		case ABILITY_THICKFAT:
			if (type == TYPE_FIRE || type == TYPE_ICE) {
				attack /= 2;
				spAttack /= 2;
			}
			break;

		case ABILITY_FURCOAT:
			defense *= 2;
			break;

		case ABILITY_PORTALPOWER:
		#ifdef PORTAL_POWER
			if (!CheckContact(move, bankAtk)) {
				attack = udivsi(attack * 75, 100);
				spAttack = udivsi(spAttack * 75, 100);
			}
		#endif
			break;
	}

//Attacker Item Checks
	if (!IgnoreAttacker) {
		mon = atkSpecies;
		switch (atkEffect) {
			case ITEM_EFFECT_CHOICE_BAND:
				switch (atkQuality) {
					case QUALITY_CHOICE_BAND:
						attack = udivsi((attack * 150), 100);
						break;

					case QUALITY_CHOICE_SPECS:
						spAttack = udivsi((spAttack * 150), 100);
				}
				break;

			case ITEM_EFFECT_THICK_CLUB:
				if (mon == SPECIES_CUBONE || mon == SPECIES_MAROWAK || mon == SPECIES_MAROWAK_A)
					attack *= 2;
				break;

			case ITEM_EFFECT_LIGHT_BALL:
				if (SpeciesToNationalPokedexNum(mon) == NATIONAL_DEX_PIKACHU) {
					attack *= 2;
					spAttack *= 2;
				}
				break;

			case ITEM_EFFECT_DEEP_SEA_TOOTH:
				if (mon == SPECIES_CLAMPERL)
					spAttack *= 2;
				break;

			case ITEM_EFFECT_MUSCLE_BAND:
				attack = udivsi((attack * 110), 100);
				break;

			case ITEM_EFFECT_WISE_GLASSES:
				spAttack = udivsi((spAttack * 110), 100);
				break;

			case ITEM_EFFECT_PLATE:
			case ITEM_EFFECT_TYPE_BOOSTERS:
				if (type == atkQuality) {
					attack = udivsi((attack * 120), 100);
					spAttack = udivsi((spAttack * 120), 100);
				}
				break;

			case ITEM_EFFECT_ADAMANT_ORB:
				if (mon == SPECIES_DIALGA && (type == TYPE_STEEL || type == TYPE_DRAGON)) {
					attack = udivsi((attack * 120), 100);
					spAttack = udivsi((spAttack * 120), 100);
				}
				break;

			case ITEM_EFFECT_LUSTROUS_ORB:
				if (mon == SPECIES_PALKIA && (type == TYPE_WATER || type == TYPE_DRAGON)) {
					attack = udivsi((attack * 120), 100);
					spAttack = udivsi((spAttack * 120), 100);
				}
				break;

			case ITEM_EFFECT_GRISEOUS_ORB:
				if ((mon == SPECIES_GIRATINA || mon == SPECIES_GIRATINA_ORIGIN) && (type == TYPE_GHOST || type == TYPE_DRAGON)) {
					attack = udivsi((attack * 120), 100);
					spAttack = udivsi((spAttack * 120), 100);
				}
				break;

			case ITEM_EFFECT_SOUL_DEW:
				if (mon == SPECIES_LATIOS || mon == SPECIES_LATIAS) {
					#ifdef OLD_SOUL_DEW_EFFECT
						spAttack *= 2;
					#else
						if (type == TYPE_PSYCHIC || type == TYPE_DRAGON) {
							attack = udivsi((attack * 120), 100);
							spAttack = udivsi((spAttack * 120), 100);
						}
					#endif
				}
				break;

			case ITEM_EFFECT_GEM: //This check is specifically meant for the AI, as the Gem would usually be consumed by now
								  //If the gNewBS->GemHelper is active and the user still has a gem, that means it received another through Symbiosis which is ignored
				if (type == atkQuality && !gNewBS->GemHelper) {
					#ifdef OLD_GEM_BOOST
						attack = udivsi((attack * 150), 100);
						spAttack = udivsi((spAttack * 150), 100);
					#else
						attack = udivsi((attack * 130), 100);
						spAttack = udivsi((spAttack * 130), 100);
					#endif
				}
				break;

			case ITEM_EFFECT_METRONOME: ;
				if (!partyCheck) {
					u16 boost = MathMin(200, 100 + gNewBS->MetronomeCounter[bankAtk]);
					attack = udivsi((attack * boost), 100);
					spAttack = udivsi((spAttack * boost), 100);
				}
				break;

			case ITEM_EFFECT_EXPERT_BELT:
				if (effectivenessFlags & MOVE_RESULT_SUPER_EFFECTIVE) {
					attack = udivsi((attack * 120), 100);
					spAttack = udivsi((spAttack * 120), 100);
				}
				break;

			case ITEM_EFFECT_LIFE_ORB:
				if (!CheckingConfusion) {
					attack = udivsi((attack * 130), 100);
					spAttack = udivsi((spAttack * 130), 100);
				}
		}

		if (gNewBS->GemHelper) { //If the gem activated
			#ifdef OLD_GEM_BOOST
				attack = udivsi((attack * 150), 100);
				spAttack = udivsi((spAttack * 150), 100);
			#else
				attack = udivsi((attack * 130), 100);
				spAttack = udivsi((spAttack * 130), 100);
			#endif
		}
	}

//Target Item Checks
	mon = defSpecies;
	switch (target_effect) {
		case ITEM_EFFECT_SOUL_DEW:
			#ifdef OLD_SOUL_DEW_EFFECT
				spDefense *= 2;
			#endif
			break;

		case ITEM_EFFECT_METAL_POWDER:
			if (mon == SPECIES_DITTO && !(defender->status2 & STATUS2_TRANSFORMED))
				defense *= 2;
			break;

		case ITEM_EFFECT_DEEP_SEA_SCALE:
			if (mon == SPECIES_CLAMPERL)
				spDefense *= 2;
			break;

		case ITEM_EFFECT_EVIOLITE:
			if (CanEvolve(GetBankPartyData(bankDef))) {
				defense = udivsi((defense * 150), 100);
				spDefense = udivsi((spDefense * 150), 100);
			}
			break;

		case ITEM_EFFECT_ASSAULT_VEST:
			spDefense = udivsi((spDefense * 150), 100);
			break;
	}

//Terrain Checks
	switch (gTerrainType) {
		case ELECTRIC_TERRAIN:
			if (partyCheck) {
				if (CheckGroundingFromPartyData(monAtk) && type == TYPE_ELECTRIC) {
					attack = udivsi(attack * 150, 100);
					spAttack = udivsi(spAttack * 150, 100);
				}
			}
			else {
				if (CheckGrounding(bankAtk) && type == TYPE_ELECTRIC) {
					attack = udivsi(attack * 150, 100);
					spAttack = udivsi(spAttack * 150, 100);
				}
			}
			break;

		case GRASSY_TERRAIN:
			if (partyCheck) {
				if (CheckGroundingFromPartyData(monAtk) && type == TYPE_GRASS) {
					attack = udivsi(attack * 150, 100);
					spAttack = udivsi(spAttack * 150, 100);
				}
			}
			else {
				if (CheckGrounding(bankAtk) && type == TYPE_GRASS) {
					attack = udivsi(attack * 150, 100);
					spAttack = udivsi(spAttack * 150, 100);
				}
			}
			if ((move == MOVE_MAGNITUDE || move == MOVE_EARTHQUAKE || move == MOVE_BULLDOZE)
			&& !(gStatuses3[bankDef] & STATUS3_SEMI_INVULNERABLE)) {
					attack /= 2;
					spAttack /= 2;
			}
			break;

		case MISTY_TERRAIN:
			if (CheckGrounding(bankDef) && type == TYPE_DRAGON) {
				attack /= 2;
				spAttack /= 2;
			}
			break;

		case PSYCHIC_TERRAIN:
			if (partyCheck) {
				if (CheckGroundingFromPartyData(monAtk) && type == TYPE_PSYCHIC) {
					attack = udivsi(attack * 150, 100);
					spAttack = udivsi(spAttack * 150, 100);
				}
			}
			else {
				if (CheckGrounding(bankAtk) && type == TYPE_PSYCHIC) {
					attack = udivsi(attack * 150, 100);
					spAttack = udivsi(spAttack * 150, 100);
				}
			}
	}

//Gym Badge Checks
	#ifdef BADGE_BOOSTS
		if (!partyCheck
		&& !(gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_TRAINER_TOWER | BATTLE_TYPE_FRONTIER | BATTLE_TYPE_EREADER_TRAINER))
		&& gBattleTypeFlags & BATTLE_TYPE_TRAINER
		&& SIDE(bankAtk) == B_SIDE_PLAYER
		&& gTrainerBattleOpponent != 0x400) {
			if (FlagGet(FLAG_BADGE01_GET) && SIDE(bankAtk) == B_SIDE_PLAYER)
				attack = udivsi((110 * attack), 100);

			if (FlagGet(FLAG_BADGE05_GET) && SIDE(bankDef) == B_SIDE_PLAYER)
				defense = udivsi((110 * defense), 100);

			if (FlagGet(FLAG_BADGE07_GET) && SIDE(bankAtk) == B_SIDE_PLAYER)
				spAttack = udivsi((110 * spAttack), 100);

			if (FlagGet(FLAG_BADGE07_GET) && SIDE(bankDef) == B_SIDE_PLAYER)
				spDefense = udivsi((110 * spDefense), 100);
		}
	#endif

//Sandstorm Sp. Def Increase
	if (WEATHER_HAS_EFFECT && (gBattleWeather & WEATHER_SANDSTORM_ANY) && IsOfType(bankDef, TYPE_ROCK))
		spDefense = udivsi((150 * spDefense), 100);

//Charge Boost
	if (!partyCheck && (gStatuses3[bankAtk] & STATUS3_CHARGED_UP) && type == TYPE_ELECTRIC)
	{
		attack *= 2;
		spAttack *= 2;
	}

//Sport Checks
	switch (type) {
		case TYPE_FIRE:
			if (IsWaterSportActive()) {
				attack = udivsi(attack, 3);
				spAttack = udivsi(spAttack, 3);
			}
			break;
		case TYPE_ELECTRIC:
			if (IsMudSportActive()) {
				attack = udivsi(attack, 3);
				spAttack = udivsi(spAttack, 3);
			}
	}

//Old Exploding Check
	#ifdef OLD_EXPLOSION_BOOST
		if (move == MOVE_SELFDESTRUCT || move == MOVE_EXPLOSION)
			defense /= 2;
	#endif

//Stat Buffs
	u32 buffedAttack = attack;
	u32 buffedSpAttack = spAttack;
	u32 buffedDefense = defense;
	u32 buffedSpDefense = spDefense;

	if (defAbility == ABILITY_UNAWARE || partyCheck) {
		buffedAttack = attack;
		buffedSpAttack = spAttack;
	}
	else if (gCritMultiplier > BASE_CRIT_MULTIPLIER) {
		if (attacker->statStages[STAT_STAGE_ATK-1] > 6) {
			APPLY_STAT_MOD(buffedAttack, attacker, attack, STAT_STAGE_ATK);
		}
		else
			buffedAttack = attack;

		if (attacker->statStages[STAT_STAGE_SPATK-1] > 6) {
			APPLY_STAT_MOD(buffedSpAttack, attacker, spAttack, STAT_STAGE_SPATK);
		}
		else
			buffedSpAttack = spAttack;
	}
	else {
		APPLY_STAT_MOD(buffedAttack, attacker, attack, STAT_STAGE_ATK);
		APPLY_STAT_MOD(buffedSpAttack, attacker, spAttack, STAT_STAGE_SPATK);
	}

	if (atkAbility == ABILITY_UNAWARE
	|| CheckTableForMove(move, gIgnoreStatChangesMoves)) {
		buffedDefense = defense;
		buffedSpDefense = spDefense;
	}
	else if (gCritMultiplier > BASE_CRIT_MULTIPLIER) {
		if (defender->statStages[STAT_STAGE_DEF-1] < 6) {
			APPLY_STAT_MOD(buffedDefense, defender, defense, STAT_STAGE_DEF);
		}
		else
			buffedDefense = defense;

		if (defender->statStages[STAT_STAGE_SPDEF-1] < 6) {
			APPLY_STAT_MOD(buffedSpDefense, defender, spDefense, STAT_STAGE_SPDEF);
		}
		else
			buffedSpDefense = spDefense;
	}
	else {
		APPLY_STAT_MOD(buffedDefense, defender, defense, STAT_STAGE_DEF);
		APPLY_STAT_MOD(buffedSpDefense, defender, spDefense, STAT_STAGE_SPDEF);
	}

//Actual Calculation
	if (partyCheck)
		damage = (2 * GetMonData(monAtk, MON_DATA_LEVEL, NULL)) / 5 + 2;
	else
		damage = (2 * attacker->level) / 5 + 2;

	damage *= gBattleMovePower;

	if (CheckTableForMove(move, gSpecialAttackPhysicalDamageMoves)) {
		damage *= buffedSpAttack;
		damage = udivsi(damage, MathMax(1, buffedDefense)); //MathMax prevents underflow
	}

	else {
		switch (moveSplit) {
			case SPLIT_PHYSICAL:
				PHYS_CALC:
					damage *= buffedAttack;
					damage = udivsi(damage, MathMax(1, buffedDefense));
					break;
			case SPLIT_SPECIAL:
					damage *= buffedSpAttack;
					damage = udivsi(damage, MathMax(1, buffedSpDefense));
					break;
			default:
				goto PHYS_CALC;
		}
	}

	damage = udivsi(damage, 50);
	damage += 2;

//Final Damage mods
	if (gBattleMoves[move].effect != EFFECT_BRICK_BREAK) //Shatters the screens
	{
		switch (moveSplit) {
			case SPLIT_PHYSICAL:
				if ((sideStatus & SIDE_STATUS_REFLECT || gNewBS->AuroraVeilTimers[SIDE(bankDef)])
				&& gCritMultiplier <= BASE_CRIT_MULTIPLIER
				&& !(atkAbility == ABILITY_INFILTRATOR
				&& !IgnoreAttacker)) {
					if ((gBattleTypeFlags & BATTLE_TYPE_DOUBLE) && CountAliveMonsInBattle(BATTLE_ALIVE_DEF_SIDE, bankAtk, bankDef) >= 2)
						damage = udivsi((damage * 2), 3);
					else
						damage /= 2;
				}

				if ((attackerStatus1 & STATUS_BURN) && atkAbility != ABILITY_GUTS && move != MOVE_FACADE)
					damage /= 2;
				break;

			case SPLIT_SPECIAL:
				if ((sideStatus & SIDE_STATUS_LIGHTSCREEN || gNewBS->AuroraVeilTimers[SIDE(bankDef)])
				&& gCritMultiplier <= BASE_CRIT_MULTIPLIER
				&& !(atkAbility == ABILITY_INFILTRATOR
				&& !IgnoreAttacker)) {
					if ((gBattleTypeFlags & BATTLE_TYPE_DOUBLE) && CountAliveMonsInBattle(BATTLE_ALIVE_DEF_SIDE, bankAtk, bankDef) >= 2)
						damage = udivsi((damage * 2), 3);
					else
						damage /= 2;
				}
		}
	}

	if (WEATHER_HAS_EFFECT) {
		if (gBattleWeather & WEATHER_RAIN_ANY) {
			switch (type) {
				case TYPE_FIRE:
					damage /= 2;
				break;

				case TYPE_WATER:
					damage = udivsi((damage * 150), 100);
			}
		}
		else if (gBattleWeather & WEATHER_SUN_ANY) {
			switch (type) {
				case TYPE_FIRE:
					damage = udivsi((damage * 150), 100);
				break;

				case TYPE_WATER:
					damage /= 2;
			}
		}
	}

	if ((ABILITY_PRESENT(ABILITY_DARKAURA) && type == TYPE_DARK) || (ABILITY_PRESENT(ABILITY_FAIRYAURA) && type == TYPE_FAIRY)) {
		if (ABILITY_PRESENT(ABILITY_AURABREAK))
			damage = udivsi((damage * 75), 100);
		else
			damage = udivsi((damage * 4), 3);
	}

//Second Attacker Ability Checks
	if (!IgnoreAttacker) {
		switch (atkAbility) {
			case ABILITY_TINTEDLENS:
				if (effectivenessFlags & MOVE_RESULT_NOT_VERY_EFFECTIVE)
					damage *= 2;
				break;

			case ABILITY_SNIPER:
				if (gCritMultiplier > BASE_CRIT_MULTIPLIER)
					damage = udivsi((damage * 150), 100);
				break;
		}
	}

//Second Target Ability Checks
	switch (defAbility) {
		case ABILITY_DRYSKIN:
			if (type == TYPE_FIRE)
				damage = udivsi((damage * 125), 100);
			break;

		case ABILITY_SOLIDROCK:
		case ABILITY_FILTER:
		case ABILITY_PRISMARMOR:
			if (effectivenessFlags & MOVE_RESULT_SUPER_EFFECTIVE)
				damage = udivsi((damage * 75), 100);
			break;

		case ABILITY_HEATPROOF:
		case ABILITY_WATERBUBBLE:
			if (type == TYPE_FIRE)
				damage /= 2;
			break;

		case ABILITY_MULTISCALE:
		case ABILITY_SHADOWSHIELD:
			if (defender->hp == defender->maxHP)
				damage /= 2;
			break;

		case ABILITY_FLUFFY:
			if (type == TYPE_FIRE)
				damage *= 2;
			if (gBattleMoves[move].flags & FLAG_MAKES_CONTACT
			&& atkEffect != ITEM_EFFECT_PROTECTIVE_PADS
			&& atkAbility != ABILITY_LONGREACH)
				damage /= 2;
	}

	if (defPartner_ability == ABILITY_FRIENDGUARD)
		damage = udivsi((damage * 75), 100);

//Damage Reduction Berry Check
	if (target_effect == ITEM_EFFECT_WEAKNESS_BERRY && !AbilityBattleEffects(ABILITYEFFECT_CHECK_OTHER_SIDE, bankDef, ABILITY_UNNERVE, 0, 0)) {
		if (effectivenessFlags & MOVE_RESULT_SUPER_EFFECTIVE && target_quality == type)
			damage /= 2;
		else if (target_quality == TYPE_NORMAL && type == TYPE_NORMAL) //Chilan Berry
			damage /= 2;
	}

//Specific Move Modifiers
	switch (move) {
		case MOVE_ASSURANCE:
			if (TOOK_DAMAGE(bankDef))
				damage *= 2;
			break;

		case MOVE_MAGNITUDE:
		case MOVE_EARTHQUAKE:
			if (gStatuses3[bankDef] & STATUS3_UNDERGROUND)
				damage *= 2;
			break;

		case MOVE_SURF:
			if (gStatuses3[bankDef] & STATUS3_UNDERWATER)
				damage *= 2;
			break;

		case MOVE_KNOCKOFF:
			if (CanKnockOffItem(bankDef))
				damage = udivsi((damage * 150), 100);
			break;

		case MOVE_SOLARBEAM:
		case MOVE_SOLARBLADE:
			if (WEATHER_HAS_EFFECT && gBattleWeather & (WEATHER_RAIN_ANY | WEATHER_SANDSTORM_ANY | WEATHER_HAIL_ANY | WEATHER_FOG_ANY))
				damage /= 2; //Any weather except sun weakens Solar Beam
	}

	if (CheckTableForMove(move, gAlwaysHitWhenMinimizedMoves) && gStatuses3[bankDef] & STATUS3_MINIMIZED)
		damage *= 2;

	if (gNewBS->MeFirstByte)
		damage = udivsi((damage * 150), 100);

	if (gNewBS->ParentalBondOn == 1)
	{
	#ifdef OLD_PARENTAL_BOND_DAMAGE
		damage /= 2;
	#else
		damage /= 4;
	#endif
	}

	if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
	{
		if (gBattleMoves[move].target & MOVE_TARGET_BOTH && CountAliveMonsInBattle(BATTLE_ALIVE_DEF_SIDE, bankAtk, bankDef) >= 2)
			damage = (damage * 75) / 100;

		else if (gBattleMoves[move].target & MOVE_TARGET_FOES_AND_ALLY && CountAliveMonsInBattle(BATTLE_ALIVE_EXCEPT_ACTIVE, bankAtk, bankDef) >= 2)
			damage = (damage * 75) / 100;
	}

	if (!IgnoreAttacker && gProtectStructs[bankAtk].helpingHand)
		damage = (damage * 150) / 100;

	if (damage == 0)
		damage = 1;
	else if (damage > 0x7FFFFFFF) //Prevent overflow
		damage = 0x7FFFFFFF;

	return damage;
}

//Banks need to be accurate, even if just checking party data (for side checking)
u16 GetBasePower(u8 bankAtk, u8 bankDef, u16 move, u16 item, u8 item_effect, u8 ability, u32 atkStatus1, u16 atkHP, u16 atkMaxHP, u16 species, struct Pokemon* monAtk, bool8 partyCheck, bool8 menuCheck, bool8 ignoreDef) {
	u16 power = gBattleMoves[move].power;
	int i;

	switch (move) {
		case MOVE_ACROBATICS:
			if (item == 0)
				power *= 2;
			break;

		case MOVE_AVALANCHE:
		case MOVE_REVENGE:
			if (!menuCheck
			&& !partyCheck
			&& ((gProtectStructs[bankAtk].physicalDmg && gProtectStructs[bankAtk].physicalBank == bankDef)
			 || (gProtectStructs[bankAtk].specialDmg && gProtectStructs[bankAtk].specialBank == bankDef)))
				power *= 2;
			break;

		case MOVE_BRINE:
			if (!ignoreDef
			&& gBattleMons[bankDef].hp < gBattleMons[bankDef].maxHP / 2)
				power *= 2;
			break;

		case MOVE_ECHOEDVOICE:
			power = MathMin(200, power + (40 * gNewBS->EchoedVoiceDamageScale));
			break;

		case MOVE_FACADE:
			if (atkStatus1 & STATUS_ANY)
				power *= 2;
			break;

		case MOVE_FURYCUTTER:
			if (!partyCheck) {
				for (i = 0; i < gDisableStructs[bankAtk].furyCutterCounter; i++)
					power *= 2;
			}
			break;

		case MOVE_FUSIONBOLT:
			if (!menuCheck
			&& !partyCheck
			&& gNewBS->fusionFlareUsedPrior
			&& gBanksByTurnOrder[0] != bankAtk)
				power *= 2;
			break;

		case MOVE_FUSIONFLARE:
			if (!menuCheck
			&& !partyCheck
			&& gNewBS->fusionBoltUsedPrior
			&& gBanksByTurnOrder[0] != bankAtk)
				power *= 2;
			break;

		case MOVE_HEX:
			if (!ignoreDef
			&& gBattleMons[bankDef].status1 & STATUS_ANY)
				power *= 2;
			break;

		case MOVE_PAYBACK:
			if (!menuCheck
			&& !partyCheck
			&& BankMovedBefore(bankDef, bankAtk))
				power *= 2;
			break;

		case MOVE_RETALIATE:
			if (gNewBS->RetaliateCounters[SIDE(bankAtk)]) //Bank should be accurate for party too
				power *= 2;
			break;

		case MOVE_ROUND:
			if (!menuCheck
			&& !partyCheck
			&& gNewBS->roundUsed)
				power *= 2;
			break;

		case MOVE_SMELLINGSALTS:
			if (!ignoreDef
			&& gBattleMons[bankDef].status1 & STATUS_PARALYSIS)
				power *= 2;
			break;

		case MOVE_STOMPINGTANTRUM:
			if (!partyCheck && gNewBS->StompingTantrumTimers[bankAtk])
				power *= 2;
			break;

		case MOVE_VENOSHOCK:
			if (!ignoreDef
			&& gBattleMons[bankDef].status1 & STATUS_PSN_ANY)
				power *= 2;
			break;

		case MOVE_WAKEUPSLAP:
			if (!ignoreDef
			&& gBattleMons[bankDef].status1 & STATUS_SLEEP)
				power *= 2;
			break;

		case MOVE_WEATHERBALL:
			if (gBattleWeather & WEATHER_ANY && !(gBattleWeather & WEATHER_AIR_CURRENT_PRIMAL))
				power *= 2;
			break;

		case MOVE_WHIRLPOOL:
			if (!ignoreDef
			&& gStatuses3[bankDef] & STATUS3_UNDERWATER)
				power *= 2;
			break;

		case MOVE_GUST:
		case MOVE_TWISTER:
			if (!ignoreDef
			&& gStatuses3[bankDef] & (STATUS3_IN_AIR | STATUS3_SKY_DROP_ATTACKER | STATUS3_SKY_DROP_TARGET))
				power *= 2;
			break;

		case MOVE_ROLLOUT:
		case MOVE_ICEBALL:
			if (!menuCheck && !partyCheck)
			{
				if (gBattleMons[bankAtk].status2 & STATUS2_DEFENSE_CURL)
					power *= 2;

				if (gBattleMons[bankAtk].status2 & STATUS2_MULTIPLETURNS) //Rollout has started
				{
					for (i = 1; i < (5 - gDisableStructs[bankAtk].rolloutTimer); ++i)
						power *= 2;
				}
			}
			break;

		case MOVE_GRASSPLEDGE:
		case MOVE_FIREPLEDGE:
		case MOVE_WATERPLEDGE:
			if (!menuCheck && !partyCheck && gNewBS->PledgeHelper)
				power = 150;
			break;

		case MOVE_ELECTROBALL:	;
			u32 ratio;
			if (!ignoreDef)
			{
				if (partyCheck)
					ratio = udivsi(SpeedCalcForParty(SIDE(bankAtk), monAtk), SpeedCalc(bankDef));
				else
					ratio = udivsi(SpeedCalc(bankAtk), SpeedCalc(bankDef));

				if (ratio >= 4)
					power = 150;
				else if (ratio >= 3)
					power = 120;
				else if (ratio >= 2)
					power = 80;
				else
					power = 60;
			}
			break;

		case MOVE_GYROBALL:	;
			if (!ignoreDef)
			{
				u32 def_spd = 25 * SpeedCalc(bankDef);
				if (partyCheck)
					power = udivsi(def_spd, SpeedCalcForParty(SIDE(bankAtk), monAtk));
				else
					power = udivsi(def_spd, SpeedCalc(bankAtk));

				power = MathMin(150, power);
			}
			break;

		case MOVE_PUNISHMENT:
			power = MathMin(200, 60 + 20 * CountBoosts(bankDef));
			break;

		case MOVE_LOWKICK:
		case MOVE_GRASSKNOT:	;
			if (!ignoreDef)
			{
				u32 weight = GetActualSpeciesWeight(SPECIES(bankDef), ABILITY(bankDef), ITEM_EFFECT(bankDef), bankDef, TRUE) / 10;

				if (weight >= 200)
					power = 120;
				else if (weight >= 100)
					power = 100;
				else if (weight >= 50)
					power = 80;
				else if (weight >= 25)
					power = 60;
				else if (weight >= 10)
					power = 40;
				else
					power = 20;
			}
			break;

		case MOVE_HEAVYSLAM:
		case MOVE_HEATCRASH:	;
			if (!ignoreDef)
			{
				u32 atkWeight, defWeight, weightRatio;

				defWeight = GetActualSpeciesWeight(SPECIES(bankDef), ABILITY(bankDef), ITEM_EFFECT(bankDef), bankDef, TRUE);

				if (partyCheck)
					atkWeight = GetActualSpeciesWeight(monAtk->species, ability, item_effect, bankAtk, FALSE);
				else
					atkWeight = GetActualSpeciesWeight(SPECIES(bankAtk), ability, item_effect, bankAtk, TRUE);

				weightRatio = udivsi(atkWeight, defWeight);

				switch (weightRatio) {
					case 0:
					case 1:
						power = 40;
						break;
					case 2:
						power = 60;
						break;
					case 3:
						power = 80;
						break;
					case 4:
						power = 100;
						break;
					default:
						power = 120;
				}
			}
			break;

		case MOVE_POWERTRIP:
		case MOVE_STOREDPOWER:
			if (!partyCheck)
				power = 20 * (1 + CountBoosts(bankAtk));
			break;

		case MOVE_FLING:	;
			power = GetFlingPower(item, species, ability, bankAtk, partyCheck);
			break;

		case MOVE_ERUPTION:
		case MOVE_WATERSPOUT:	;
			power = MathMax(1, udivsi(150 * atkHP, atkMaxHP));
			break;

		case MOVE_REVERSAL:
		case MOVE_FLAIL:	;
			u16 percentage = udivsi(48 * atkHP, atkMaxHP);

			if (percentage <= 1)
				power = 200;
			else if (percentage <= 4)
				power = 150;
			else if (percentage <= 9)
				power = 100;
			else if (percentage <= 16)
				power = 80;
			else if (percentage <= 32)
				power = 40;
			else
				power = 20;
			break;

		case MOVE_WATERSHURIKEN:
			if (species == SPECIES_ASHGRENINJA && ability == ABILITY_BATTLEBOND)
				power = 20;
			break;

		case MOVE_CRUSHGRIP:
		case MOVE_WRINGOUT:
			if (!ignoreDef)
				power = MathMax(1, udivsi(gBattleMons[bankDef].hp * 120, gBattleMons[bankDef].maxHP));
			break;

		case MOVE_TRUMPCARD:	;
			u8 index, pp;
			for (index = 0; i < 5; ++i) {
				if (index == 5) {
					index = 0xFF;
					break;
				}
				if (monAtk->moves[index] == MOVE_TRUMPCARD)
					break;
			}
			if (index < 4) {
				pp = monAtk->pp[index];
				if (pp > 0) {
					switch (pp-1) {
						case 0:
							power = 200;
							break;
						case 1:
							power = 80;
							break;
						case 2:
							power = 60;
							break;
						case 3:
							power = 50;
							break;
						default:
							power = 40;
					}
				}
			}
			break;

		case MOVE_SPITUP:
			if (!partyCheck)
				power = 100 * gDisableStructs[bankAtk].stockpileCounter; //Will be 0 if no stockpile
			break;

		case MOVE_NATURALGIFT:	;
			power = 0;
			if (IsBerry(item))
			{
				for (i = 0; gNaturalGiftTable[i].berry != ITEM_TABLES_TERMIN; ++i)
				{
					if (gNaturalGiftTable[i].berry == item)
					{
						power = gNaturalGiftTable[i].power;
						break;
					}
				}
			}
			break;

		case MOVE_PURSUIT:
			if (!menuCheck)
				power *= gBattleScripting->dmgMultiplier;
			break;

		case MOVE_RETURN:
			if (gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_TRAINER_TOWER | BATTLE_TYPE_FRONTIER | BATTLE_TYPE_EREADER_TRAINER))
				power = udivsi(10 * 255, 25);
			else if (partyCheck)
				power = udivsi(10 * (monAtk->friendship), 25);
			else
				power = udivsi(10 * (gBattleMons[bankAtk].friendship), 25);
			break;

		case MOVE_FRUSTRATION:
			if (gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_TRAINER_TOWER | BATTLE_TYPE_FRONTIER | BATTLE_TYPE_EREADER_TRAINER))
				power = udivsi(10 * 255, 25);
			else if (partyCheck)
				power = udivsi(10 * (255 - monAtk->friendship), 25);
			else
				power = udivsi(10 * (255 - gBattleMons[bankAtk].friendship), 25);
			break;

		case MOVE_BEATUP:
			if (partyCheck || menuCheck)
				power = udivsi(gBaseStats[species].baseAttack, 10) + 5;
			else
			{
				struct Pokemon* party;
				if (SIDE(bankAtk) == B_SIDE_PLAYER)
					party = gPlayerParty;
				else
					party = gEnemyParty;

				power = udivsi(gBaseStats[party[gBattleCommunication[0] - 1].species].baseAttack, 10) + 5;
			}
			break;

		case MOVE_HIDDENPOWER:
		#ifdef OLD_HIDDEN_POWER_BP
			if (partyCheck) {
				power = ((monAtk->hpIV & 2) >> 1) |
						((monAtk->attackIV & 2)) |
						((monAtk->defenseIV & 2) << 1) |
						((monAtk->speedIV & 2) << 2) |
						((monAtk->spAttackIV & 2) << 3) |
						((monAtk->spDefenseIV & 2) << 4);
			}
			else {
				power = ((gBattleMons[bankAtk].hpIV & 2) >> 1) |
						((gBattleMons[bankAtk].attackIV & 2)) |
						((gBattleMons[bankAtk].defenseIV & 2) << 1) |
						((gBattleMons[bankAtk].speedIV & 2) << 2) |
						((gBattleMons[bankAtk].spAttackIV & 2) << 3) |
						((gBattleMons[bankAtk].spDefenseIV & 2) << 4);
			}
			power = udivsi((40 * power), 63) + 30;
		#endif
			break;

		case MOVE_TRIPLEKICK:
			if (!menuCheck && !partyCheck)
				power = gBattleScripting->tripleKickPower;
			break;
	}

	return power;
}

//Requires that the base move be loaded into gNewBS->aiZMoveHelper
static u16 GetZMovePower(u16 zMove) {
	u16 power = 1;

	switch (zMove) {
		case MOVE_CATASTROPIKA:
		case MOVE_10000000_VOLT_THUNDERBOLT:
		case MOVE_STOKED_SPARKSURFER:
		case MOVE_EXTREME_EVOBOOST:
		case MOVE_PULVERIZING_PANCAKE:
		case MOVE_GENESIS_SUPERNOVA:
		case MOVE_SINISTER_ARROW_RAID:
		case MOVE_MALICIOUS_MOONSAULT:
		case MOVE_OCEANIC_OPERETTA:
		case MOVE_SPLINTERED_STORMSHARDS:
		case MOVE_LETS_SNUGGLE_FOREVER:
		case MOVE_CLANGOROUS_SOULBLAZE:
		case MOVE_GUARDIAN_OF_ALOLA:
		case MOVE_SEARING_SUNRAZE_SMASH:
		case MOVE_MENACING_MOONRAZE_MAELSTROM:
		case MOVE_LIGHT_THAT_BURNS_THE_SKY:
		case MOVE_SOUL_STEALING_7_STAR_STRIKE:
			power = gBattleMoves[zMove].power;
			break;
		default:
			//An issue arose in the case of the AI calculating Z-Move power. To fix this,
			//gNewBS->aiZMoveHelper stores the original move to helper turn it into a Z-Move.
			power = gBattleMoves[gNewBS->aiZMoveHelper].z_move_power;
	}

	return power;
}

u16 CalcVisualBasePower(u8 bankAtk, u8 bankDef, u16 move, u16 power, u8 moveType, bool8 ignoreDef) {
	if (power == 1)
		return power; //Don't modify a variable base power move

	u16 attack = power;
	u16 spAttack = power;

	struct BattlePokemon* attacker = &gBattleMons[bankAtk];
	struct BattlePokemon* defender = &gBattleMons[bankDef];

	u32 effectivenessFlags = 0;

	if (!ignoreDef)
		effectivenessFlags = TypeCalc(move, bankAtk, bankDef, 0, FALSE);

	u16 atkSpecies = attacker->species;
	u16 defSpecies = defender->species;
	u8 atkEffect = ITEM_EFFECT(bankAtk);
	u8 atkQuality = ITEM_QUALITY(bankAtk);
	u32 attackerStatus1 = attacker->status1;

	switch(ABILITY(bankAtk)) {
		case ABILITY_TECHNICIAN:
			if (power <= 60) {
				attack = udivsi((attack * 150), 100);
				spAttack = udivsi((spAttack * 150), 100);
			}
			break;

		case ABILITY_RIVALRY:	;
			if (!ignoreDef) {
				u8 attackerGender = GetGenderFromSpeciesAndPersonality(atkSpecies, attacker->personality);
				u8 targetGender = GetGenderFromSpeciesAndPersonality(defSpecies, defender->personality);

				if (attackerGender != 0xFF && targetGender != 0xFF) {
					if (attackerGender == targetGender) {
						attack = udivsi((attack * 125), 100);
						spAttack = udivsi((spAttack * 125), 100);
					}
					else {
						attack = udivsi((attack * 75), 100);
						spAttack = udivsi((spAttack * 75), 100);
					}
				}
			}
			break;

		case ABILITY_RECKLESS:
			if (CheckTableForMove(move, gRecklessBoostedMoves)) {
				attack = udivsi((attack * 120), 100);
				spAttack = udivsi((spAttack * 120), 100);
			}
			break;

		case ABILITY_IRONFIST:
			if (CheckTableForMove(move, gPunchingMoves)) {
				attack = udivsi((attack * 120), 100);
				spAttack = udivsi((spAttack * 120), 100);
			}
			break;

		case ABILITY_TOXICBOOST:
			if (attackerStatus1 & STATUS_PSN_ANY)
				attack = udivsi((attack * 150), 100);
			break;

		case ABILITY_FLAREBOOST:
			if (attackerStatus1 & STATUS_BURN)
				spAttack = udivsi((attack * 150), 100);
			break;

		case ABILITY_SANDFORCE:
			if (gBattleWeather & WEATHER_SANDSTORM_ANY
			&& (moveType == TYPE_ROCK || moveType == TYPE_GROUND || moveType == TYPE_STEEL)) {
				attack = udivsi((attack * 130), 100);
				spAttack = udivsi((spAttack * 130), 100);
			}
			break;

		case ABILITY_SHEERFORCE:
			if (CheckTableForMove(move, gSheerForceBoostedMoves)) {
				attack = udivsi((attack * 130), 100);
				spAttack = udivsi((spAttack * 130), 100);
			}
			break;

		case ABILITY_AERILATE:
		case ABILITY_PIXILATE:
		case ABILITY_REFRIGERATE:
		case ABILITY_GALVANIZE:
		case ABILITY_NORMALIZE:
			if (AbilityCanChangeTypeAndBoost(bankAtk, move)) {
				#ifdef OLD_ATE_BOOST
					attack = udivsi((attack * 130), 100);
					spAttack = udivsi((spAttack * 130), 100);
				#else
					attack = udivsi((attack * 120), 100);
					spAttack = udivsi((spAttack * 120), 100);
				#endif
			}
			break;

		case ABILITY_MEGALAUNCHER:
			if (CheckTableForMove(move, gPulseAuraMoves)) {
				attack = udivsi((attack * 150), 100);
				spAttack = udivsi((spAttack * 150), 100);
			}
			break;

		case ABILITY_STRONGJAW:
			if (CheckTableForMove(move, gBitingMoves)) {
				attack = udivsi((attack * 150), 100);
				spAttack = udivsi((spAttack * 150), 100);
			}
			break;

		case ABILITY_TOUGHCLAWS:
			if (gBattleMoves[move].flags & FLAG_MAKES_CONTACT && atkEffect != ITEM_EFFECT_PROTECTIVE_PADS) {
				attack = udivsi((attack * 130), 100);
				spAttack = udivsi((spAttack * 130), 100);
			}
			break;

		case ABILITY_NEUROFORCE:
			if (effectivenessFlags & MOVE_RESULT_SUPER_EFFECTIVE) {
				attack = udivsi((attack * 125), 100);
				spAttack = udivsi((spAttack * 125), 100);
			}
			break;

		case ABILITY_STEELWORKER:
			if (moveType == TYPE_STEEL) {
				attack = udivsi((attack * 150), 100);
				spAttack = udivsi((spAttack * 150), 100);
			}
			break;

		case ABILITY_WATERBUBBLE:
			if (moveType == TYPE_WATER) {
				attack *= 2;
				spAttack *= 2;
			}
			break;
	}

	if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE
	&& ABILITY(PARTNER(bankAtk)) == ABILITY_BATTERY)
		spAttack = udivsi((spAttack * 130), 100);

	if (gStatuses3[bankAtk] & STATUS3_CHARGED_UP && moveType == TYPE_ELECTRIC) {
		attack *= 2;
		spAttack *= 2;
	}

//Attacker Item Checks
	switch (atkEffect) {
		case ITEM_EFFECT_MUSCLE_BAND:
			attack = udivsi((attack * 110), 100);
			break;

		case ITEM_EFFECT_WISE_GLASSES:
			spAttack = udivsi((spAttack * 110), 100);
			break;

		case ITEM_EFFECT_PLATE:
		case ITEM_EFFECT_TYPE_BOOSTERS:
			if (moveType == atkQuality) {
				attack = udivsi((attack * 120), 100);
				spAttack = udivsi((spAttack * 120), 100);
			}
			break;

		case ITEM_EFFECT_ADAMANT_ORB:
			if (atkSpecies == SPECIES_DIALGA
			&& (moveType == TYPE_STEEL || moveType == TYPE_DRAGON)) {
				attack = udivsi((attack * 120), 100);
				spAttack = udivsi((spAttack * 120), 100);
			}
			break;

		case ITEM_EFFECT_LUSTROUS_ORB:
			if (atkSpecies == SPECIES_PALKIA
			&& (moveType == TYPE_WATER || moveType == TYPE_DRAGON)) {
				attack = udivsi((attack * 120), 100);
				spAttack = udivsi((spAttack * 120), 100);
			}
			break;

		case ITEM_EFFECT_GRISEOUS_ORB:
			if ((atkSpecies == SPECIES_GIRATINA || atkSpecies == SPECIES_GIRATINA_ORIGIN)
			&& (moveType == TYPE_GHOST || moveType == TYPE_DRAGON)) {
				attack = udivsi((attack * 120), 100);
				spAttack = udivsi((spAttack * 120), 100);
			}
			break;

		case ITEM_EFFECT_SOUL_DEW:
			if (atkSpecies == SPECIES_LATIOS || atkSpecies == SPECIES_LATIAS) {
				#ifndef OLD_SOUL_DEW_EFFECT
					if (moveType == TYPE_PSYCHIC || moveType == TYPE_DRAGON) {
						attack = udivsi((attack * 120), 100);
						spAttack = udivsi((spAttack * 120), 100);
					}
				#endif
			}
			break;

		case ITEM_EFFECT_GEM:
			if (moveType == atkQuality) {
				#ifdef OLD_GEM_BOOST
					attack = udivsi((attack * 150), 100);
					spAttack = udivsi((spAttack * 150), 100);
				#else
					attack = udivsi((attack * 130), 100);
					spAttack = udivsi((spAttack * 130), 100);
				#endif
			}
			break;

		case ITEM_EFFECT_METRONOME: ;
			u16 boost = MathMin(200, 100 + gNewBS->MetronomeCounter[bankAtk]);
			attack = udivsi((attack * boost), 100);
			spAttack = udivsi((spAttack * boost), 100);
			break;

		case ITEM_EFFECT_EXPERT_BELT:
			if (effectivenessFlags & MOVE_RESULT_SUPER_EFFECTIVE) {
				attack = udivsi((attack * 120), 100);
				spAttack = udivsi((spAttack * 120), 100);
			}
			break;

		case ITEM_EFFECT_LIFE_ORB:
			attack = udivsi((attack * 130), 100);
			spAttack = udivsi((spAttack * 130), 100);
	}

//Terrain Checks
	switch (gTerrainType) {
		case ELECTRIC_TERRAIN:
			if (CheckGrounding(bankAtk) && moveType == TYPE_ELECTRIC) {
				attack = udivsi(attack * 150, 100);
				spAttack = udivsi(spAttack * 150, 100);
			}
			break;

			case GRASSY_TERRAIN:
				if (CheckGrounding(bankAtk) && moveType == TYPE_GRASS) {
					attack = udivsi(attack * 150, 100);
					spAttack = udivsi(spAttack * 150, 100);
				}

				if (!ignoreDef
				&& (move == MOVE_MAGNITUDE || move == MOVE_EARTHQUAKE || move == MOVE_BULLDOZE)
				&& !(gStatuses3[bankDef] & STATUS3_SEMI_INVULNERABLE)) {
					attack /= 2;
					spAttack /= 2;
				}
				break;

			case MISTY_TERRAIN:
				if (!ignoreDef
				&& CheckGrounding(bankDef)
				&& moveType == TYPE_DRAGON) {
					attack /= 2;
					spAttack /= 2;
				}
				break;

			case PSYCHIC_TERRAIN:
				if (CheckGrounding(bankAtk) && moveType == TYPE_PSYCHIC) {
					attack = udivsi(attack * 150, 100);
					spAttack = udivsi(spAttack * 150, 100);
				}
	}

//Sport Checks
	switch (moveType) {
		case TYPE_FIRE:
			if (IsWaterSportActive()) {
				attack = udivsi(attack, 3);
				spAttack = udivsi(spAttack, 3);
			}
			break;
		case TYPE_ELECTRIC:
			if (IsMudSportActive()) {
				attack = udivsi(attack, 3);
				spAttack = udivsi(spAttack, 3);
			}
	}

	switch (move) {
		case MOVE_SOLARBEAM:
		case MOVE_SOLARBLADE:
			if (WEATHER_HAS_EFFECT
			&& gBattleWeather & (WEATHER_RAIN_ANY | WEATHER_SANDSTORM_ANY | WEATHER_HAIL_ANY | WEATHER_FOG_ANY))
			{
				attack /= 2; //Any weather except sun weakens Solar Beam
				spAttack /= 2;
			}
	}

	switch (CalcMoveSplit(bankAtk, move)) {
		case SPLIT_SPECIAL:
			return spAttack;
		default:
			return attack;
	}
}

static u32 AdjustWeight(u32 weight, u8 ability, u8 item_effect, u8 bank, bool8 check_nimble)
{
	int i;

	switch (ability) {
		case ABILITY_LIGHTMETAL:
			weight /= 2;
			break;
		case ABILITY_HEAVYMETAL:
			weight *= 2;
	}

	if (item_effect == ITEM_EFFECT_FLOAT_STONE)
		weight /= 2;

	if (check_nimble) {
		for (i = 0; i < gNewBS->NimbleCounters[bank]; ++i) {
			if (weight > 2200)
				weight -= 2200;
			else {
				weight = 1;
				break;
			}
		}
	}
	return weight;
}

u32 GetActualSpeciesWeight(u16 species, u8 ability, u8 itemEffect, u8 bank, bool8 checkNimble)
{
	u32 weight = TryGetAlternateSpeciesSize(species, PKDX_GET_WEIGHT); //Eg. Mega Form
	if (weight == 0)
		weight = GetPokedexHeightWeight(SpeciesToNationalPokedexNum(species), 1);

	return AdjustWeight(weight, ability, itemEffect, bank, checkNimble);
}

static u8 GetFlingPower(u16 item, u16 species, u8 ability, u8 bank, bool8 partyCheck)
{
	u8 power = 0;
	u8 embargoTimer = (partyCheck) ? 0 : gNewBS->EmbargoTimers[bank];

	if (CanFling(item, species, ability, bank, embargoTimer))
	{
		power = gFlingTable[item].power;
		if (power == 0)
			power = 30; //Default power is 30
	}

	return power;
}

static void ApplyRandomDmgMultiplier(void)
{
	u16 rando = 100 - (Random() & 15);
	if (gBattleMoveDamage)
		gBattleMoveDamage = MathMax(1, (gBattleMoveDamage * rando) / 100);
}

//Some species have alternate forms with different sizes (like Megas)
u16 TryGetAlternateSpeciesSize(u16 species, u8 type)
{
	for (int i = 0; gAlternateSpeciesSizeTable[i].species != SPECIES_TABLES_TERMIN; ++i)
	{
		if (gAlternateSpeciesSizeTable[i].species == species)
		{
			switch (type) {
				case PKDX_GET_HEIGHT:
					return gAlternateSpeciesSizeTable[i].height;

				case PKDX_GET_WEIGHT:
					return gAlternateSpeciesSizeTable[i].weight;
			}
		}
	}

	return 0;
}

bool8 IsBankHoldingFocusSash(u8 bank)
{
	if (ITEM_EFFECT(bank) == ITEM_EFFECT_FOCUS_BAND
	&& ItemId_GetMystery2(ITEM(bank)))
		return TRUE;

	return FALSE;
}

u8 CountAliveMonsInBattle(u8 caseId, u8 bankAtk, u8 bankDef)
{
	s32 i;
	u8 retVal = 0;

	switch (caseId)
	{
	case BATTLE_ALIVE_EXCEPT_ACTIVE:
		for (i = 0; i < gBattlersCount; ++i)
		{
			if (i != bankAtk && !(gAbsentBattlerFlags & gBitTable[i]) && BATTLER_ALIVE(i))
				retVal++;
		}
		break;
	case BATTLE_ALIVE_ATK_SIDE:
		for (i = 0; i < gBattlersCount; ++i)
		{
			if (SIDE(i) == SIDE(bankAtk) && !(gAbsentBattlerFlags & gBitTable[i]) && BATTLER_ALIVE(i))
				retVal++;
		}
		break;
	case BATTLE_ALIVE_DEF_SIDE:
		for (i = 0; i < gBattlersCount; ++i)
		{
			if (SIDE(i) == SIDE(bankDef) && !(gAbsentBattlerFlags & gBitTable[i]) && BATTLER_ALIVE(i))
				retVal++;
		}
		break;
	}

	return retVal;
}
