#include "defines.h"
#include "defines_battle.h"
#include "../include/event_data.h"
#include "../include/pokedex.h"
#include "../include/random.h"
#include "../include/constants/items.h"
#include "../include/constants/pokedex.h"
#include "../include/constants/trainers.h"

#include "../include/new/accuracy_calc.h"
#include "../include/new/ai_util.h"
#include "../include/new/battle_start_turn_start.h"
#include "../include/new/battle_util.h"
#include "../include/new/damage_calc.h"
#include "../include/new/dynamax.h"
#include "../include/new/frontier.h"
#include "../include/new/general_bs_commands.h"
#include "../include/new/item.h"
#include "../include/new/move_tables.h"
#include "../include/new/switching.h"
#include "../include/new/util.h"

#include "Tables/type_tables.h"

/*
damage_calc.c
	functions responsible for calculating damage, including modifications from abilities, effects, etc...
*/

extern const struct NaturalGiftStruct gNaturalGiftTable[];
extern const struct FlingStruct gFlingTable[];
extern struct AlternateSize gAlternateSpeciesSizeTable[];
extern const u8 gDynamaxMovePowers[];

static const u16 sCriticalHitChances[] =
#ifdef CRIT_CHANCE_GEN_6
	{16, 8, 2, 1, 1};
#elif (defined CRIT_CHANCE_GEN_2_TO_5)
	{16, 8, 4, 3, 2};
#else
	{24, 8, 2, 1, 1};
#endif

#define BASE_CRIT_MULTIPLIER 10 //1x
#ifdef OLD_CRIT_DAMAGE
	#define CRIT_MULTIPLIER 20 //2x
#else //Gen 6+ crit damage
	#define CRIT_MULTIPLIER 15 //1.5x
#endif

#define FLAG_IGNORE_TARGET 0x1
#define FLAG_CONFUSION_DAMAGE 0x2
#define FLAG_CHECKING_FROM_MENU 0x4
#define FLAG_AI_CALC 0x8
#define FLAG_FUTURE_SIGHT_DAMAGE 0x10
#define FLAG_SPLINTER_DAMAGE 0x20

//This file's functions:
static u8 CalcPossibleCritChance(u8 bankAtk, u8 bankDef, u16 move, struct Pokemon* monAtk, struct Pokemon* monDef);
static void TypeDamageModificationByDefTypes(u8 atkAbility, u8 bankDef, u16 move, u8 moveType, u8* flags, u8 defType1, u8 defType2, u8 defType3, struct Pokemon* monDef);
static void ModulateDmgByType(u8 multiplier, const u16 move, const u8 moveType, const u8 defType, const u8 bankDef, u8 atkAbility, u8* flags, struct Pokemon* monDef);
static bool8 AbilityCanChangeTypeAndBoost(u16 move, u8 atkAbility, u8 electrifyTimer, bool8 zMoveActive);
static s32 CalculateBaseDamage(struct DamageCalc* data);
static u16 GetBasePower(struct DamageCalc* data);
static u16 AdjustBasePower(struct DamageCalc* data, u16 power);
static u16 GetZMovePower(u16 zMove);
static u16 GetMaxMovePower(u16 maxMove);
static u32 AdjustWeight(u32 weight, ability_t, item_effect_t, bank_t, bool8 check_nimble);
static u8 GetFlingPower(u16 item, u16 species, u8 ability, u8 bank, bool8 partyCheck);
static u32 ScreensWeakenDamage(u32 damage, bool8 screensUp, u8 atkAbility, u8 bankDef);
static void AdjustDamage(bool8 CheckFalseSwipe);
static void ApplyRandomDmgMultiplier(void);
static void TryBoostMonOffensesForTotemBoost(struct DamageCalc* data, u8 bankAtk, bool8 bodyPress);
static void TryBoostMonDefensesForTotemBoost(struct DamageCalc* data, u8 bankDef);
static void BoostMonOffensesForTotemBoost(struct DamageCalc* data, u8 bankAtk, bool8 multiBoost, bool8 bodyPress);
static void BoostMonDefensesForTotemBoost(struct DamageCalc* data, u8 bankDef, bool8 multiBoost);

void atk04_critcalc(void)
{
	u16 critChance;
	bool8 confirmedCrit;
	u8 atkAbility = ABILITY(gBankAttacker);
	u8 atkEffect = ITEM_EFFECT(gBankAttacker);
	u8 moveTarget = GetBaseMoveTarget(gCurrentMove, gBankAttacker);
	bool8 calcSpreadMove = IS_DOUBLE_BATTLE && moveTarget & (MOVE_TARGET_BOTH | MOVE_TARGET_ALL);

	gStringBank = gBankAttacker;

	for (u32 bankDef = 0; bankDef < gBattlersCount; ++bankDef)
	{
		confirmedCrit = FALSE;
		critChance = 0;

		if (!calcSpreadMove) //Single target
			bankDef = gBankTarget;
		else if (gNewBS->calculatedSpreadMoveData)
			break; //Already calculated crit chance
		else if (!BATTLER_ALIVE(bankDef) || bankDef == gBankAttacker
		|| (bankDef == PARTNER(gBankAttacker) && !(moveTarget & MOVE_TARGET_ALL))
		|| gNewBS->ResultFlags[bankDef] & MOVE_RESULT_NO_EFFECT
		|| gNewBS->noResultString[bankDef])
			continue; //Don't bother with this target

		u8 defAbility = ABILITY(bankDef);

		if (defAbility == ABILITY_BATTLEARMOR
		||  defAbility == ABILITY_SHELLARMOR
		||  CantScoreACrit(gBankAttacker, NULL)
		||  gBattleTypeFlags & (BATTLE_TYPE_OLD_MAN | BATTLE_TYPE_OAK_TUTORIAL | BATTLE_TYPE_POKE_DUDE)
		||  gNewBS->LuckyChantTimers[SIDE(bankDef)])
		{
			confirmedCrit = FALSE;
		}
		else if (IsLaserFocused(gBankAttacker)
		|| (atkAbility == ABILITY_MERCILESS && !SpeciesHasDrillBeak(GetProperAbilityPopUpSpecies(gBankAttacker)) && (gBattleMons[bankDef].status1 & STATUS_PSN_ANY))
		|| (atkAbility == ABILITY_DRILLBEAK && SpeciesHasDrillBeak(GetProperAbilityPopUpSpecies(gBankAttacker)) && gSpecialMoveFlags[gCurrentMove].gDrillMoves) //Drill moves always crit
		|| gSpecialMoveFlags[gCurrentMove].gAlwaysCriticalMoves)
		{
			confirmedCrit = TRUE;
		}
		else 
		{
			critChance  = 2 * ((gBattleMons[gBankAttacker].status2 & STATUS2_FOCUS_ENERGY) != 0)
						+ gNewBS->chiStrikeCritBoosts[gBankAttacker]
						+ (gSpecialMoveFlags[gCurrentMove].gHighCriticalChanceMoves)
						+ (atkEffect == ITEM_EFFECT_SCOPE_LENS)
						+ (atkAbility == ABILITY_SUPERLUCK)
						#ifdef NATIONAL_DEX_CHANSEY
						+ 2 * (atkEffect == ITEM_EFFECT_LUCKY_PUNCH && SpeciesToNationalPokedexNum(SPECIES(gBankAttacker)) == NATIONAL_DEX_CHANSEY)
						#endif
						#if (defined NATIONAL_DEX_FARFETCHD && defined NATIONAL_DEX_SIRFETCHD)
						+ 2 * (atkEffect == ITEM_EFFECT_STICK && (SpeciesToNationalPokedexNum(SPECIES(gBankAttacker)) == NATIONAL_DEX_FARFETCHD
						                                       || SpeciesToNationalPokedexNum(SPECIES(gBankAttacker)) == NATIONAL_DEX_SIRFETCHD))
						#endif
						#ifdef SPECIES_PALKIA_ORIGIN
						+ 2 * (gCurrentMove == MOVE_SPACIALREND && SPECIES(gBankAttacker) == SPECIES_PALKIA_ORIGIN)
						#endif
						+ 2 * (gCurrentMove == MOVE_10000000_VOLT_THUNDERBOLT);

			if (critChance > 4)
				critChance = 4;

			if (!(Random() % sCriticalHitChances[critChance]))
				confirmedCrit = TRUE;
		}

		gCritMultiplier = BASE_CRIT_MULTIPLIER;

		//These damages will be divded by 100 so really 2x and 1.5x
		if (confirmedCrit)
			gCritMultiplier = CRIT_MULTIPLIER;

		gNewBS->criticalMultiplier[bankDef] = gCritMultiplier;

		if (!calcSpreadMove)
			break; //Only 1 target
	}

	gCritMultiplier = gNewBS->criticalMultiplier[gBankTarget];
	++gBattlescriptCurrInstr;
}

static u8 CalcPossibleCritChance(u8 bankAtk, u8 bankDef, u16 move, struct Pokemon* monAtk, struct Pokemon* monDef)
{
	u8 atkAbility;
	u8 defAbility;
	u16 atkSpecies, atkAbilitySpecies;
	u32 atkStatus2;
	u32 defStatus1;

	u8 atkEffect = 0;
	u16 critChance = 0;

	if (monAtk != NULL)
	{
		atkAbility = GetMonAbilityAfterTrace(monAtk, bankDef);
		atkSpecies = atkAbilitySpecies = monAtk->species;
		atkEffect = GetMonItemEffect(monAtk);
		atkStatus2 = 0;
	}
	else
	{
		atkAbility = ABILITY(bankAtk);
		atkEffect = ITEM_EFFECT(bankAtk);
		atkSpecies = gBattleMons[bankAtk].species;
		atkAbilitySpecies = GetProperAbilityPopUpSpecies(bankAtk);
		atkStatus2 = gBattleMons[bankAtk].status2;
	}

	if (monDef != NULL)
	{
		defAbility = GetMonAbilityAfterTrace(monDef, bankAtk);
		defStatus1 = monDef->condition;
	}
	else
	{
		defAbility = ABILITY(bankDef);
		defStatus1 = gBattleMons[bankDef].status1;
	}

	if (IsTargetAbilityIgnored(defAbility, atkAbility, move))
		defAbility = ABILITY_NONE; //Ignore Ability

	if (defAbility == ABILITY_BATTLEARMOR
	||  defAbility == ABILITY_SHELLARMOR
	||  CantScoreACrit(bankAtk, monAtk)
	||  gBattleTypeFlags & (BATTLE_TYPE_OLD_MAN | BATTLE_TYPE_OAK_TUTORIAL)
	||  gNewBS->LuckyChantTimers[SIDE(bankDef)])
	{
		return FALSE;
	}
	else if ((IsLaserFocused(bankAtk) && monAtk == NULL)
	|| (atkAbility == ABILITY_MERCILESS && !SpeciesHasDrillBeak(atkAbilitySpecies) && (defStatus1 & STATUS_PSN_ANY))
	|| (atkAbility == ABILITY_DRILLBEAK && SpeciesHasDrillBeak(atkAbilitySpecies) && gSpecialMoveFlags[move].gDrillMoves) //Drill moves always crit
	|| gSpecialMoveFlags[move].gAlwaysCriticalMoves)
	{
		return TRUE;
	}
	else
	{
		critChance  = 2 * ((atkStatus2 & STATUS2_FOCUS_ENERGY) != 0)
					+ gNewBS->chiStrikeCritBoosts[bankAtk]
					+ (gSpecialMoveFlags[move].gHighCriticalChanceMoves)
					+ (atkEffect == ITEM_EFFECT_SCOPE_LENS)
					+ (atkAbility == ABILITY_SUPERLUCK)
					#ifdef NATIONAL_DEX_CHANSEY
					+ 2 * (atkEffect == ITEM_EFFECT_LUCKY_PUNCH && SpeciesToNationalPokedexNum(atkSpecies) == NATIONAL_DEX_CHANSEY)
					#endif
					#ifdef NATIONAL_DEX_FARFETCHD
					+ 2 * (atkEffect == ITEM_EFFECT_STICK && SpeciesToNationalPokedexNum(atkSpecies) == NATIONAL_DEX_FARFETCHD)
					#endif
					+ 2 * (move == MOVE_10000000_VOLT_THUNDERBOLT);

		#ifdef CRIT_CHANCE_GEN_6
			if (critChance >= 3)
				return TRUE;
			if (critChance >= 2)
				return 2; //50 % Chance
		#elif (defined CRIT_CHANCE_GEN_2_TO_5)
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

void atk05_damagecalc(void)
{
	struct DamageCalc data = {0};
	u8 moveTarget = GetBaseMoveTarget(gCurrentMove, gBankAttacker);
	gBattleStruct->dynamicMoveType = GetMoveTypeSpecial(gBankAttacker, gCurrentMove);

	if (gNewBS->calculatedSpreadMoveData && gMultiHitCounter == 0)
	{
		//Just use the calculated values below
	}
	else if (IS_DOUBLE_BATTLE && moveTarget & (MOVE_TARGET_BOTH | MOVE_TARGET_ALL))
	{
		data.bankAtk = gBankAttacker;
		data.move = gCurrentMove;
		PopulateDamageCalcStructWithBaseAttackerData(&data);

		//All multi target foes are calculated now b/c stats can change after first kill (eg. Moxie) - also for synchronized HP bar reduction
		for (u32 bankDef = 0; bankDef < gBattlersCount; ++bankDef)
		{
			if (!BATTLER_ALIVE(bankDef) || bankDef == gBankAttacker
			|| (bankDef == PARTNER(gBankAttacker) && !(moveTarget & MOVE_TARGET_ALL))
			|| gNewBS->ResultFlags[bankDef] & MOVE_RESULT_NO_EFFECT
			|| gNewBS->noResultString[bankDef])
					continue; //Don't bother with this target

			data.bankDef = bankDef;
			PopulateDamageCalcStructWithBaseDefenderData(&data);
			gCritMultiplier = gNewBS->criticalMultiplier[bankDef];
			gNewBS->DamageTaken[bankDef] = (CalculateBaseDamage(&data) * gCritMultiplier) / BASE_CRIT_MULTIPLIER;
		}
	}
	else //Single Battle or single target move
	{
		data.bankAtk = gBankAttacker;
		data.bankDef = gBankTarget;
		data.move = gCurrentMove;
		gCritMultiplier = gNewBS->criticalMultiplier[gBankTarget];
		gBattleMoveDamage = (CalculateBaseDamage(&data) * gCritMultiplier) / BASE_CRIT_MULTIPLIER;
		gNewBS->DamageTaken[gBankTarget] = gBattleMoveDamage;
	}

	gBattleMoveDamage = gNewBS->DamageTaken[gBankTarget];
	gCritMultiplier = gNewBS->criticalMultiplier[gBankTarget];
	++gBattlescriptCurrInstr;
}

static bool8 IsFutureSightAttackerOnField(u8 bankDef, u8 possibleUser)
{
	u8 monIndexAttacker = gWishFutureKnock.futureSightPartyIndex[bankDef];

	if (BATTLER_ALIVE(possibleUser) && gBattlerPartyIndexes[possibleUser] == monIndexAttacker)
		return TRUE;

	if (IS_DOUBLE_BATTLE && BATTLER_ALIVE(PARTNER(possibleUser)) && gBattlerPartyIndexes[PARTNER(possibleUser)] == monIndexAttacker)
		return TRUE;

	return FALSE;
}

static struct Pokemon* GetFutureSightMon(u8 bankDef, u8 bankAtk)
{
	u8 monId = gWishFutureKnock.futureSightPartyIndex[bankDef];
	return SIDE(bankAtk) == B_SIDE_PLAYER ? &gPlayerParty[monId] : &gEnemyParty[monId];
}

void FutureSightDamageCalc(void)
{
	struct DamageCalc data = {0};
	data.bankAtk = gBankAttacker;
	data.bankDef = gBankTarget;
	data.move = gCurrentMove;
	data.specialFlags |= FLAG_FUTURE_SIGHT_DAMAGE;

	if (!IsFutureSightAttackerOnField(gBankTarget, gBankAttacker))
	{
		//Uses the data from the party if the user of Future Sight switched out
		data.monAtk = GetFutureSightMon(gBankTarget, gBankAttacker);
	}

	gBattleMoveDamage = (CalculateBaseDamage(&data) * gCritMultiplier) / BASE_CRIT_MULTIPLIER;
	gNewBS->DamageTaken[gBankTarget] = gBattleMoveDamage;
}

s32 ConfusionDamageCalc(void)
{
	struct DamageCalc data = {0};
	data.bankAtk = data.bankDef = gBankAttacker;
	data.move = MOVE_POUND;
	data.specialFlags |= FLAG_CONFUSION_DAMAGE;
	gBattleMoveDamage = CalculateBaseDamage(&data);
	return gBattleMoveDamage;
}

static bool8 IsSplinterAttackerOnField(u8 bankDef, u8 possibleUser)
{
	u8 monIdAttacker = gNewBS->splinterAttackerMonId[bankDef];

	if (BATTLER_ALIVE(possibleUser) && gBattlerPartyIndexes[possibleUser] == monIdAttacker)
		return TRUE;

	if (IS_DOUBLE_BATTLE && BATTLER_ALIVE(PARTNER(possibleUser)) && gBattlerPartyIndexes[PARTNER(possibleUser)] == monIdAttacker)
		return TRUE;

	return FALSE;
}

static u8 GetSplinterBankAttacker(u8 bankDef, u8 possibleUser) //Assumes IsSplinterAttackerOnField returns TRUE
{
	u8 monIdAttacker = gNewBS->splinterAttackerMonId[bankDef];

	if (BATTLER_ALIVE(possibleUser) && gBattlerPartyIndexes[possibleUser] == monIdAttacker)
		return possibleUser;

	return PARTNER(possibleUser);
}

static struct Pokemon* GetSplinterMon(u8 bankDef, u8 bankAtk)
{
	u8 monId = gNewBS->splinterAttackerMonId[bankDef];
	return SIDE(bankAtk) == B_SIDE_PLAYER ? &gPlayerParty[monId] : &gEnemyParty[monId];
}

u32 SplintersDamageCalc(u8 bankAtk, u8 bankDef, u16 move)
{
	struct DamageCalc data = {0};
	data.bankAtk = bankAtk;
	data.bankDef = bankDef;
	data.move = move;
	data.specialFlags |= FLAG_FUTURE_SIGHT_DAMAGE | FLAG_SPLINTER_DAMAGE;

	if (!IsSplinterAttackerOnField(bankDef, bankAtk))
	{
		//Uses the data from the party if the user of the splinters switched out
		data.monAtk = GetSplinterMon(bankDef, bankAtk);
	}
	else
		data.bankAtk = GetSplinterBankAttacker(bankDef, bankAtk);

	gBattleMoveDamage = CalculateBaseDamage(&data);
	TypeCalc(move, data.bankAtk, bankDef, data.monAtk);
	return gBattleMoveDamage;
}

static u8 GetNumHitsBasedOnMove(u16 move, u8 atkAbility, unusedArg u16 atkSpecies)
{
	u8 numHits = 1;

	if (move == MOVE_SURGINGSTRIKES
	#ifdef SPECIES_ASHGRENINJA
	|| (move == MOVE_WATERSHURIKEN && atkSpecies == SPECIES_ASHGRENINJA)
	#endif
	)
		numHits = 3;
	else if (gSpecialMoveFlags[move].gTwoToFiveStrikesMoves)
	{
		if (atkAbility == ABILITY_SKILLLINK)
			numHits = 5;
		else
			numHits = 3; //Three hits on average
	}
	//else if (gBattleMoves[move].effect == EFFECT_TRIPLE_KICK) //Factored into the base calc - not the best solution since doesn't account for breaking sash/mulitscale
	//	numHits = 3;
	else if (gSpecialMoveFlags[move].gTwoStrikesMoves)
		numHits = 2;

	return numHits;
}

static u16 GetAIParentalBondMultiplierForMove(u16 move, u8 bankAtk, u8 numHits, u8 ability)
{
	u16 multiplier = 0;

	if (numHits <= 1
	&& ability == ABILITY_PARENTALBOND
	&& IsMoveAffectedByParentalBond(move, bankAtk)) //Parental Bond can have effect on move
	{
		#ifdef OLD_PARENTAL_BOND_DAMAGE
			multiplier = 150; //1.5x overall boost
			if (move == MOVE_ASSURANCE)
				multiplier = 200; //Basically winds up being 2x
		#else
			multiplier = 125; //1.25x overall boost
			if (move == MOVE_ASSURANCE)
				multiplier = 150; //Basically winds up being 1.5x
		#endif
	}

	return multiplier;
}

static u8 AdjustNumHitsForContactDamage(u8 numHits, s32 currHP, u32 contactDamage)
{
	if (contactDamage > 0)
	{
		u16 possibleHits = 0;

		do
		{
			possibleHits += 1;
			currHP -= contactDamage;
		} while (currHP > 0 //Once you take more damage than HP you have left, you faint and can't attack anymore
			&& possibleHits < numHits);  //Unless you're not going to be hitting that much

		if (possibleHits < numHits) //The attacker will faint before it could do all of the hits
			numHits = possibleHits;
	}

	return numHits;
}

u32 AI_CalcDmg(const u8 bankAtk, const u8 bankDef, const u16 move, struct DamageCalc* damageData)
{
	u32 damage = 0;
	u8 resultFlags = AI_SpecialTypeCalc(move, bankAtk, bankDef);

	if (gBattleMoves[move].effect != EFFECT_PAIN_SPLIT && resultFlags & MOVE_RESULT_NO_EFFECT)
		return 0;

	struct DamageCalc data = {0};

	if (damageData == NULL)
		damageData = &data;

	damageData->bankAtk = bankAtk;
	damageData->bankDef = bankDef;
	damageData->move = move;
	damageData->specialFlags |= FLAG_AI_CALC;

	bool8 parentalBond = (damageData->atkAbility == ABILITY_PARENTALBOND);

	switch (gBattleMoves[move].effect) {
		case EFFECT_SUPER_FANG:
			damage = GetBaseCurrentHP(bankDef) / 2; //50 % of base HP
			if (parentalBond)
				damage += GetBaseCurrentHP(bankDef) / 4; //75 % of base HP
			return damage;
		case EFFECT_DRAGON_RAGE:
			damage = 40 * (parentalBond ? 2 : 1);
			return damage;
		case EFFECT_SONICBOOM:
			damage = 20 * (parentalBond ? 2 : 1);
			return damage;
		case EFFECT_LEVEL_DAMAGE:
			damage = gBattleMons[bankAtk].level * (parentalBond ? 2 : 1);
			return damage;
		case EFFECT_PSYWAVE:
			damage = GetPsywaveDamage(gBattleMons[bankAtk].level, 50) * (parentalBond ? 2 : 1); //On average, 50 will be selected as the random number
			return damage;
		case EFFECT_MEMENTO: //Final Gambit
			if (move == MOVE_FINALGAMBIT)
				return gBattleMons[bankAtk].hp;
			return 0;
		case EFFECT_ENDEAVOR:
			damage = GetBaseCurrentHP(bankDef) - GetBaseCurrentHP(bankAtk);
			if (damage <= 0)
				return 0;
			return damage;
		case EFFECT_PAIN_SPLIT: ;
			u16 finalHp = (GetBaseCurrentHP(bankAtk) + GetBaseCurrentHP(bankDef)) / 2;

			if (finalHp >= GetBaseCurrentHP(bankDef))
				return 0;
			return GetBaseCurrentHP(bankDef) - finalHp;
	}

	if (SPLIT(move) == SPLIT_STATUS) //At this point we don't care about Status moves anymore
		return 0;

	gBattleScripting.dmgMultiplier = 1;

	gCritMultiplier = CalcPossibleCritChance(bankAtk, bankDef, move, NULL, NULL); //Return 0 if none, 1 if always, 2 if 50%
	if (gCritMultiplier != 0 && AIRandom() % gCritMultiplier == 0)
		gCritMultiplier = CRIT_MULTIPLIER;
	else
		gCritMultiplier = BASE_CRIT_MULTIPLIER;

	damage = CalculateBaseDamage(damageData);

	gBattleMoveDamage = MathMin(0x7FFFFFFF, damage);
	AI_SpecialTypeCalc(move, bankAtk, bankDef);
	damage = (gBattleMoveDamage * gCritMultiplier) / BASE_CRIT_MULTIPLIER;
	gCritMultiplier = BASE_CRIT_MULTIPLIER; //Reset

	damage = (damage * 93) / 100; //Roll 93% damage - about halfway between min & max damage

	u8 numHits = GetNumHitsBasedOnMove(move, damageData->atkAbility, damageData->atkSpecies);
	u16 multiplier = GetAIParentalBondMultiplierForMove(move, bankAtk, numHits, damageData->atkAbility);
	if (multiplier != 0) //Move affected by Parental Bond
		return (damage * multiplier) / 100;

	//Try to reduce the number of hits for a multi-hit move if the attacker won't be able to finish because it will be KOd by the contact recoil first
	if (numHits >= 2)
		numHits = AdjustNumHitsForContactDamage(numHits, gBattleMons[bankAtk].hp, GetContactDamage(move, bankAtk, bankDef));

	if (numHits <= 1)
	{
		//Multi hit moves skip these checks
		if (gBattleMoves[move].effect == EFFECT_FALSE_SWIPE
		|| (IsAffectedBySturdy(damageData->defAbility, bankDef) && NO_MOLD_BREAKERS(damageData->atkAbility, move))
		|| IsAffectedByFocusSash(bankDef))
			damage = MathMin(damage, gBattleMons[bankDef].hp - 1);
	}
	else
	{
		if (IsDamageHalvedDueToFullHP(bankDef, damageData->defAbility, move, damageData->atkAbility))
			damage = damage + (damage * 2 * (numHits - 1)); //Adjust damage on subsequent hits
		else
			damage *= numHits;
	}

	return damage;
}

u32 AI_CalcPartyDmg(u8 bankAtk, u8 bankDef, u16 move, struct Pokemon* monAtk, struct DamageCalc* damageData)
{
	u32 damage;
	u8 resultFlags = TypeCalc(move, bankAtk, bankDef, monAtk);

	if (gBattleMoves[move].effect != EFFECT_PAIN_SPLIT
	&& (SPLIT(move) == SPLIT_STATUS || resultFlags & MOVE_RESULT_NO_EFFECT))
		return 0;

	switch (gBattleMoves[move].effect) {
		case EFFECT_SUPER_FANG:
			damage = GetBaseCurrentHP(bankDef) / 2; //50 % of base HP
			if (damageData->atkAbility == ABILITY_PARENTALBOND)
				damage += GetBaseCurrentHP(bankDef) / 4; //75 % of base HP
			return damage;
		case EFFECT_DRAGON_RAGE:
			damage = 40;
			if (damageData->atkAbility == ABILITY_PARENTALBOND)
				damage *= 2;
			return damage;
		case EFFECT_SONICBOOM:
			damage = 20;
			if (damageData->atkAbility == ABILITY_PARENTALBOND)
				damage *= 2;
			return damage;
		case EFFECT_LEVEL_DAMAGE:
			damage = monAtk->level;
			if (damageData->atkAbility == ABILITY_PARENTALBOND)
				damage *= 2;
			return damage;
		case EFFECT_PSYWAVE:
			damage = GetPsywaveDamage(monAtk->level, 50); //On average, 50 will be selected as the random number
			if (damageData->atkAbility == ABILITY_PARENTALBOND)
				damage *= 2;
			return damage;
		case EFFECT_MEMENTO: //Final Gambit
			if (move == MOVE_FINALGAMBIT)
				return monAtk->hp;
			return 0;
		case EFFECT_ENDEAVOR:
			if (gBattleMons[bankDef].hp <= monAtk->hp)
				return 0;
			return gBattleMons[bankDef].hp - monAtk->hp;
		case EFFECT_PAIN_SPLIT: ;
			u16 finalHp = MathMax(1, (monAtk->hp + gBattleMons[bankDef].hp) / 2);

			if (finalHp >= gBattleMons[bankDef].hp)
				return 0;
			return gBattleMons[bankDef].hp - finalHp;
	}

	if (SPLIT(move) == SPLIT_STATUS) //At this point we don't care about Status moves anymore
		return 0;

	damage = 0;
	struct DamageCalc data = {0};
	gBattleScripting.dmgMultiplier = 1;

	gCritMultiplier = CalcPossibleCritChance(bankAtk, bankDef, move, monAtk, NULL); //Return 0 if none, 1 if always, 2 if 50%
	if (gCritMultiplier != 0 && AIRandom() % gCritMultiplier == 0)
		gCritMultiplier = CRIT_MULTIPLIER;
	else
		gCritMultiplier = BASE_CRIT_MULTIPLIER;

	if (damageData == NULL)
		damageData = &data;

	damageData->bankAtk = bankAtk;
	damageData->bankDef = bankDef;
	damageData->move = move;
	damageData->monAtk = monAtk;
	damageData->specialFlags |= FLAG_AI_CALC;
	damage = CalculateBaseDamage(damageData);

	gBattleMoveDamage = damage;
	TypeCalc(move, bankAtk, bankDef, monAtk);
	damage = (gBattleMoveDamage * gCritMultiplier) / BASE_CRIT_MULTIPLIER;
	gCritMultiplier = BASE_CRIT_MULTIPLIER; //Reset

	damage = (damage * 96) / 100; //Roll 96% damage with party mons - be more idealistic

	u8 numHits = GetNumHitsBasedOnMove(move, damageData->atkAbility, damageData->atkSpecies);
	u16 multiplier = GetAIParentalBondMultiplierForMove(move, bankAtk, numHits, damageData->atkAbility);
	if (multiplier != 0) //Move affected by Parental Bond
		return (damage * multiplier) / 100;

	//Try to reduce the number of hits for a multi-hit move if the attacker won't be able to finish because it will be KOd by the contact recoil first
	if (numHits > 1)
		numHits = AdjustNumHitsForContactDamage(numHits, monAtk->hp, GetContactDamageMonAtk(monAtk, bankDef));

	if (numHits <= 1)
	{
		//Multi hit moves skip these checks
		if (gBattleMoves[move].effect == EFFECT_FALSE_SWIPE
		|| (IsAffectedBySturdy(damageData->defAbility, bankDef) && NO_MOLD_BREAKERS(damageData->atkAbility, move))
		|| IsAffectedByFocusSash(bankDef))
			damage = MathMin(damage, gBattleMons[bankDef].hp - 1);
	}
	else
	{
		if (IsDamageHalvedDueToFullHP(bankDef, damageData->defAbility, move, damageData->atkAbility))
			damage = damage + (damage * 2 * (numHits - 1)); //Adjust damage on subsequent hits
		else
			damage *= numHits;
	}

	return damage;
}

u32 AI_CalcMonDefDmg(u8 bankAtk, u8 bankDef, u16 move, struct Pokemon* monDef, struct DamageCalc* damageData)
{
	u32 damage;

	if (gBattleMoves[move].effect != EFFECT_PAIN_SPLIT
	&& (SPLIT(move) == SPLIT_STATUS || (AI_TypeCalc(move, bankAtk, bankDef, monDef) & MOVE_RESULT_NO_EFFECT)))
		return 0;

	switch (gBattleMoves[move].effect) {
		case EFFECT_SUPER_FANG:
			damage = monDef->hp / 2; //50 % of base HP
			if (damageData->atkAbility == ABILITY_PARENTALBOND)
				damage += monDef->hp / 4; //75 % of base HP
			return damage;
		case EFFECT_DRAGON_RAGE:
			damage = 40;
			if (damageData->atkAbility == ABILITY_PARENTALBOND)
				damage *= 2;
			return damage;
		case EFFECT_SONICBOOM:
			damage = 20;
			if (damageData->atkAbility == ABILITY_PARENTALBOND)
				damage *= 2;
			return damage;
		case EFFECT_LEVEL_DAMAGE:
			damage = gBattleMons[bankAtk].level;
			if (damageData->atkAbility == ABILITY_PARENTALBOND)
				damage *= 2;
			return damage;
		case EFFECT_PSYWAVE:
			damage = GetPsywaveDamage(gBattleMons[bankAtk].level, 50); //On average, 50 will be selected as the random number
			if (damageData->atkAbility == ABILITY_PARENTALBOND)
				damage *= 2;
			return damage;
		case EFFECT_MEMENTO: //Final Gambit
			if (move == MOVE_FINALGAMBIT)
				return gBattleMons[bankAtk].hp;
			return 0;
		case EFFECT_ENDEAVOR:
			if (monDef->hp <= gBattleMons[bankAtk].hp)
				return 0;
			return monDef->hp - gBattleMons[bankAtk].hp;
		case EFFECT_PAIN_SPLIT: ;
			u16 finalHp = MathMax(1, (gBattleMons[bankAtk].hp + monDef->hp) / 2);

			if (finalHp >= monDef->hp)
				return 0;
			return monDef->hp - finalHp;
		case EFFECT_BURN_UP:
			if (!IsOfType(bankAtk, TYPE_FIRE))
				return 0;
			break;
		case EFFECT_POLTERGEIST:
			if (WillPoltergeistFail(monDef->item, GetMonAbilityAfterTrace(monDef, bankAtk)))
				return 0;
			break;
	}

	damage = 0;
	struct DamageCalc data = {0};
	gBattleScripting.dmgMultiplier = 1;

	gCritMultiplier = CalcPossibleCritChance(bankAtk, bankDef, move, NULL, monDef); //Return 0 if none, 1 if always, 2 if 50%
	if (gCritMultiplier != 0 && AIRandom() % gCritMultiplier == 0)
		gCritMultiplier = CRIT_MULTIPLIER;
	else
		gCritMultiplier = BASE_CRIT_MULTIPLIER;

	if (damageData == NULL)
		damageData = &data;

	damageData->bankAtk = bankAtk;
	damageData->bankDef = bankDef;
	damageData->move = move;
	damageData->monDef = monDef;
	damageData->specialFlags |= FLAG_AI_CALC;
	damage = CalculateBaseDamage(damageData);

	gBattleMoveDamage = damage;
	AI_TypeCalc(move, bankAtk, bankDef, monDef);
	damage = (gBattleMoveDamage * gCritMultiplier) / BASE_CRIT_MULTIPLIER;
	gCritMultiplier = BASE_CRIT_MULTIPLIER; //Reset

	damage = (damage * 96) / 100; //Roll 96% damage with party mons - be more idealistic

	u8 numHits = GetNumHitsBasedOnMove(move, damageData->atkAbility, damageData->atkSpecies);
	u16 multiplier = GetAIParentalBondMultiplierForMove(move, bankAtk, numHits, damageData->atkAbility);
	if (multiplier != 0) //Move affected by Parental Bond
		return (damage * multiplier) / 100;

	//Try to reduce the number of hits for a multi-hit move if the attacker won't be able to finish because it will be KOd by the contact recoil first
	if (numHits > 1)
		numHits = AdjustNumHitsForContactDamage(numHits, gBattleMons[bankAtk].hp, GetContactDamageMonDef(bankAtk, monDef));

	if (numHits <= 1)
	{
		//Multi hit moves skip these checks
		if (gBattleMoves[move].effect == EFFECT_FALSE_SWIPE)
			damage = MathMin(damage, monDef->hp - 1);
		else if (GetMonEntryHazardDamage(monDef, SIDE(bankDef)) == 0) //Focus Sash and Sturdy would work
		{
			if ((monDef->hp == monDef->maxHP && damageData->defAbility == ABILITY_STURDY && NO_MOLD_BREAKERS(damageData->atkAbility, move))
			 || IsMonAffectedByFocusSash(monDef))
				damage = MathMin(damage, monDef->hp - 1);
		}
	}
	else
	{
		if (IsMonDamageHalvedDueToFullHP(monDef, damageData->defAbility, move, damageData->atkAbility))
			damage = damage + (damage * 2 * (numHits - 1)); //Adjust damage on subsequent hits
		else
			damage *= numHits;
	}

	return damage;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//This is type calc BS Command
void atk06_typecalc(void)
{
	u8 moveType = gBattleStruct->dynamicMoveType & 0x3F;
	u8 atkAbility = ABILITY(gBankAttacker);
	u8 atkType1 = gBattleMons[gBankAttacker].type1;
	u8 atkType2 = gBattleMons[gBankAttacker].type2;
	u8 atkType3 = gBattleMons[gBankAttacker].type3;
	u8 moveTarget = GetBaseMoveTarget(gCurrentMove, gBankAttacker);
	bool8 calcSpreadMove = IS_DOUBLE_BATTLE && moveTarget & (MOVE_TARGET_BOTH | MOVE_TARGET_ALL);

	if (gCurrentMove != MOVE_STRUGGLE)
	{
		for (u32 bankDef = 0; bankDef < gBattlersCount; ++bankDef)
		{
			if (!calcSpreadMove) //Single target move
				bankDef = gBankTarget;
			else if (gNewBS->calculatedSpreadMoveData)
				break; //Already calculated type adjustment
			else if (!BATTLER_ALIVE(bankDef) || bankDef == gBankAttacker
			|| (bankDef == PARTNER(gBankAttacker) && !(moveTarget & MOVE_TARGET_ALL))
			|| gNewBS->noResultString[bankDef])
				continue;

			u8 defAbility = ABILITY(bankDef);
			u8 defEffect = ITEM_EFFECT(bankDef);
			gBattleMoveDamage = gNewBS->DamageTaken[bankDef];
			gNewBS->ResultFlags[bankDef] &= ~(MOVE_RESULT_SUPER_EFFECTIVE | MOVE_RESULT_NOT_VERY_EFFECTIVE | MOVE_RESULT_DOESNT_AFFECT_FOE); //Reset for now so damage can be modulated properly

			//Check Stab
			if (atkType1 == moveType || atkType2 == moveType || atkType3 == moveType)
			{
				if (atkAbility == ABILITY_ADAPTABILITY)
					gBattleMoveDamage *= 2;
				else
					gBattleMoveDamage = (gBattleMoveDamage * 15) / 10;
			}

			//Check Special Ground Immunities
			if (moveType == TYPE_GROUND && !CheckGrounding(bankDef) && gCurrentMove != MOVE_THOUSANDARROWS)
			{
				if (defAbility == ABILITY_LEVITATE)
				{
					gLastUsedAbility = defAbility;
					gNewBS->ResultFlags[bankDef] |= (MOVE_RESULT_MISSED | MOVE_RESULT_DOESNT_AFFECT_FOE);
					gLastLandedMoves[bankDef] = 0;
					gLastHitByType[bankDef] = 0;
					gNewBS->missStringId[bankDef] = 3;
					RecordAbilityBattle(bankDef, gLastUsedAbility);
				}
				else if (defEffect == ITEM_EFFECT_AIR_BALLOON)
				{
					gNewBS->ResultFlags[bankDef] |= (MOVE_RESULT_DOESNT_AFFECT_FOE);
					gLastLandedMoves[bankDef] = 0;
					gLastHitByType[bankDef] = 0;
					RecordItemEffectBattle(bankDef, defEffect);
				}
				else if ((gStatuses3[bankDef] & (STATUS3_LEVITATING | STATUS3_TELEKINESIS))
				|| IsFloatingWithMagnetism(bankDef))
				{
					gNewBS->ResultFlags[bankDef] |= (MOVE_RESULT_DOESNT_AFFECT_FOE);
					gLastLandedMoves[bankDef] = 0;
					gLastHitByType[bankDef] = 0;
				}
				else
					goto RE_ENTER_TYPE_CHECK;	//You're a flying type
			}

			//Check Powder Moves
			else if (gSpecialMoveFlags[gCurrentMove].gPowderMoves)
			{
				if (defAbility == ABILITY_OVERCOAT)
				{
					gLastUsedAbility = defAbility;
					gNewBS->ResultFlags[bankDef] |= (MOVE_RESULT_MISSED | MOVE_RESULT_DOESNT_AFFECT_FOE);
					gLastLandedMoves[bankDef] = 0;
					gLastHitByType[bankDef] = 0xFF;
					gNewBS->missStringId[bankDef] = 3;
					RecordAbilityBattle(bankDef, gLastUsedAbility);
				}
				else if (defEffect == ITEM_EFFECT_SAFETY_GOGGLES)
				{
					gNewBS->ResultFlags[bankDef] |= (MOVE_RESULT_MISSED | MOVE_RESULT_DOESNT_AFFECT_FOE);
					gLastLandedMoves[bankDef] = 0;
					gLastHitByType[bankDef] = 0xFF;
					TrySetMissStringForSafetyGoggles(bankDef);
					RecordItemEffectBattle(bankDef, defEffect);
				}
				else if (IsOfType(bankDef, TYPE_GRASS))
				{
					gNewBS->ResultFlags[bankDef] |= (MOVE_RESULT_DOESNT_AFFECT_FOE);
					gLastLandedMoves[bankDef] = 0;
					gLastHitByType[bankDef] = 0xFF;
				}
				else
					goto RE_ENTER_TYPE_CHECK;
			}
			else if ((gBattleMoves[gCurrentMove].effect == EFFECT_SKY_DROP && IsOfType(bankDef, TYPE_FLYING))
			|| (gCurrentMove == MOVE_SYNCHRONOISE && WillSyncronoiseFail(gBankAttacker, bankDef)))
			{
				gNewBS->ResultFlags[bankDef] |= (MOVE_RESULT_DOESNT_AFFECT_FOE);
				gLastLandedMoves[bankDef] = 0;
				gLastHitByType[bankDef] = 0xFF;
			}
			else
			{
			RE_ENTER_TYPE_CHECK:
				TypeDamageModification(atkAbility, bankDef, gCurrentMove, moveType, &gNewBS->ResultFlags[bankDef]);
			}

			if (defAbility == ABILITY_WONDERGUARD
			 //&& AttacksThisTurn(gBankAttacker, gCurrentMove) == 2
			 && (!(gNewBS->ResultFlags[bankDef] & MOVE_RESULT_SUPER_EFFECTIVE)
				|| ((gNewBS->ResultFlags[bankDef] & (MOVE_RESULT_SUPER_EFFECTIVE | MOVE_RESULT_NOT_VERY_EFFECTIVE)) == (MOVE_RESULT_SUPER_EFFECTIVE | MOVE_RESULT_NOT_VERY_EFFECTIVE)))
			 && SPLIT(gCurrentMove) != SPLIT_STATUS)
			 {
				gLastUsedAbility = defAbility;
				gNewBS->ResultFlags[bankDef] |= MOVE_RESULT_MISSED;
				gLastLandedMoves[bankDef] = 0;
				gLastHitByType[bankDef] = 0;
				gNewBS->missStringId[bankDef] = 3;
				RecordAbilityBattle(bankDef, gLastUsedAbility);
			}

			if (gNewBS->ResultFlags[bankDef] & MOVE_RESULT_DOESNT_AFFECT_FOE)
				gProtectStructs[gBankAttacker].targetNotAffected = 1;

			gNewBS->DamageTaken[bankDef] = gBattleMoveDamage;

			if (!calcSpreadMove)
				break; //Only 1 target
		}

		gBattleMoveDamage = gNewBS->DamageTaken[gBankTarget];
		gMoveResultFlags = gNewBS->ResultFlags[gBankTarget];
	}

	++gBattlescriptCurrInstr;
}

//This is the type calc that doesn't modify the damage
void atk4A_typecalc2(void)
{
	u8 moveType = gBattleStruct->dynamicMoveType & 0x3F;
	u8 atkAbility = ABILITY(gBankAttacker);
	u8 defAbility = ABILITY(gBankTarget);
	u8 defEffect = ITEM_EFFECT(gBankTarget);

	//Check Special Ground Immunities
	if (moveType == TYPE_GROUND && !CheckGrounding(gBankTarget) && gCurrentMove != MOVE_THOUSANDARROWS)
	{
		if (defAbility == ABILITY_LEVITATE)
		{
			gLastUsedAbility = atkAbility;
			gMoveResultFlags |= (MOVE_RESULT_MISSED | MOVE_RESULT_DOESNT_AFFECT_FOE);
			gLastLandedMoves[gBankTarget] = 0;
			gNewBS->missStringId[gBankTarget] = 3;
			RecordAbilityBattle(gBankTarget, gLastUsedAbility);
		}
		else if (defEffect == ITEM_EFFECT_AIR_BALLOON)
		{
			gMoveResultFlags |= (MOVE_RESULT_DOESNT_AFFECT_FOE);
			gLastLandedMoves[gBankTarget] = 0;
			RecordItemEffectBattle(gBankTarget, defEffect);
		}
		else if ((gStatuses3[gBankTarget] & (STATUS3_LEVITATING | STATUS3_TELEKINESIS))
		|| IsFloatingWithMagnetism(gBankTarget))
		{
			gMoveResultFlags |= (MOVE_RESULT_DOESNT_AFFECT_FOE);
			gLastLandedMoves[gBankTarget] = 0;
		}
		else
			goto RE_ENTER_TYPE_CHECK_2;	//You're a flying type
	}
	else if (gSpecialMoveFlags[gCurrentMove].gPowderMoves)
	{
		if (defAbility == ABILITY_OVERCOAT)
		{
			gLastUsedAbility = defAbility;
			gMoveResultFlags |= (MOVE_RESULT_MISSED | MOVE_RESULT_DOESNT_AFFECT_FOE);
			gLastLandedMoves[gBankTarget] = 0;
			gNewBS->missStringId[gBankTarget] = 3;
			RecordAbilityBattle(gBankTarget, gLastUsedAbility);
		}
		else if (defEffect == ITEM_EFFECT_SAFETY_GOGGLES)
		{
			gMoveResultFlags |= (MOVE_RESULT_MISSED | MOVE_RESULT_DOESNT_AFFECT_FOE);
			gLastLandedMoves[gBankTarget] = 0;
			TrySetMissStringForSafetyGoggles(gBankTarget);
			RecordItemEffectBattle(gBankTarget, defEffect);
		}
		else if (IsOfType(gBankTarget, TYPE_GRASS))
		{
			gMoveResultFlags |= (MOVE_RESULT_DOESNT_AFFECT_FOE);
			gLastLandedMoves[gBankTarget] = 0;
		}
		else
			goto RE_ENTER_TYPE_CHECK_2;
	}
	else if ((gBattleMoves[gCurrentMove].effect == EFFECT_SKY_DROP && IsOfType(gBankTarget, TYPE_FLYING))
	|| (gCurrentMove == MOVE_SYNCHRONOISE && WillSyncronoiseFail(gBankAttacker, gBankTarget)))
	{
		gMoveResultFlags |= (MOVE_RESULT_DOESNT_AFFECT_FOE);
		gLastLandedMoves[gBankTarget] = 0;
	}
	else
	{
	RE_ENTER_TYPE_CHECK_2:	;
		s32 backupDamage = gBattleMoveDamage;
		TypeDamageModification(atkAbility, gBankTarget, gCurrentMove, moveType, &gMoveResultFlags);
		gBattleMoveDamage = backupDamage;
	}

	if (defAbility == ABILITY_WONDERGUARD
	//&& AttacksThisTurn(gBankAttacker, gCurrentMove) == 2
	&& (!(gMoveResultFlags & MOVE_RESULT_SUPER_EFFECTIVE)
		|| ((gMoveResultFlags & (MOVE_RESULT_SUPER_EFFECTIVE | MOVE_RESULT_NOT_VERY_EFFECTIVE)) == (MOVE_RESULT_SUPER_EFFECTIVE | MOVE_RESULT_NOT_VERY_EFFECTIVE)))
	&& gBattleMoves[gCurrentMove].power)
	{
		gLastUsedAbility = defAbility;
		gMoveResultFlags |= MOVE_RESULT_MISSED;
		gLastLandedMoves[gBankTarget] = 0;
		gNewBS->missStringId[gBankTarget] = 3;
		RecordAbilityBattle(gBankTarget, gLastUsedAbility);
	}

	if (gMoveResultFlags & MOVE_RESULT_DOESNT_AFFECT_FOE)
		gProtectStructs[gBankAttacker].targetNotAffected = 1;

	++gBattlescriptCurrInstr;
}

//This is the type calc that is callable as a function
//This calc now also contains the calc the AI uses
u8 TypeCalc(u16 move, u8 bankAtk, u8 bankDef, struct Pokemon* monAtk)
{
	u8 moveType;
	u8 defAbility = ABILITY(bankDef);
	u8 defEffect = ITEM_EFFECT(bankDef);
	u8 atkAbility, atkType1, atkType2, atkType3;
	u8 flags = 0;

	if (move == MOVE_STRUGGLE)
		return 0;

	if (monAtk != NULL) //Use party mon as attacker
	{
		atkAbility = GetMonAbilityAfterTrace(monAtk, bankDef);

		if (atkAbility == ABILITY_IMPOSTER && ImposterWorks(bankAtk, TRUE))
		{
			u8 imposterBank = GetImposterBank(bankAtk);
			atkAbility = ABILITY(imposterBank);
			atkType1 = gBattleMons[imposterBank].type1;
			atkType2 = gBattleMons[imposterBank].type2;
			atkType3 = gBattleMons[imposterBank].type3;
			moveType = GetMoveTypeSpecial(imposterBank, move);
		}
		else
		{
			atkType1 = GetMonType(monAtk, 0);
			atkType2 = GetMonType(monAtk, 1);
			atkType3 = TYPE_BLANK;
			moveType = GetMonMoveTypeSpecial(monAtk, move);
		}
	}
	else
	{
		atkAbility = ABILITY(bankAtk);
		atkType1 = gBattleMons[bankAtk].type1;
		atkType2 = gBattleMons[bankAtk].type2;
		atkType3 = gBattleMons[bankAtk].type3;
		moveType = GetMoveTypeSpecial(bankAtk, move);
	}

	if (IsTargetAbilityIgnored(defAbility, atkAbility, move))
		defAbility = ABILITY_NONE; //Ignore Ability

	//Check stab
	if (atkType1 == moveType || atkType2 == moveType || atkType3 == moveType
	|| atkAbility == ABILITY_PROTEAN) //Will change type to get STAB
	{
		if (atkAbility == ABILITY_ADAPTABILITY)
			gBattleMoveDamage *= 2;
		else
			gBattleMoveDamage = udivsi(gBattleMoveDamage * 15, 10);
	}

	//Check Special Ground Immunities
	if (moveType == TYPE_GROUND
	&& !CheckGrounding(bankDef)
	&& ((defAbility == ABILITY_LEVITATE && NO_MOLD_BREAKERS(atkAbility, move))
	 || defEffect == ITEM_EFFECT_AIR_BALLOON
	 || (gStatuses3[bankDef] & (STATUS3_LEVITATING | STATUS3_TELEKINESIS))
	 || IsFloatingWithMagnetism(bankDef))
	&& move != MOVE_THOUSANDARROWS)
	{
		flags |= (MOVE_RESULT_MISSED | MOVE_RESULT_DOESNT_AFFECT_FOE);
	}
	else if (gSpecialMoveFlags[move].gPowderMoves && !IsAffectedByPowder(bankDef))
	{
		flags |= (MOVE_RESULT_MISSED | MOVE_RESULT_DOESNT_AFFECT_FOE);
	}
	else if ((gBattleMoves[move].effect == EFFECT_SKY_DROP && IsOfType(bankDef, TYPE_FLYING))
	|| (move == MOVE_SYNCHRONOISE && WillSyncronoiseFailByAttackerTypesAndBank(atkType1, atkType2, atkType3, bankDef)))
	{
		flags |= (MOVE_RESULT_DOESNT_AFFECT_FOE);
	}
	//Regular Type Calc
	else
	{
		TypeDamageModification(atkAbility, bankDef, move, moveType, &flags);
	}

	//Wonder Guard Check
	if (defAbility == ABILITY_WONDERGUARD
	&& NO_MOLD_BREAKERS(atkAbility, move)
	&& !(flags & MOVE_RESULT_MISSED)
	&& AttacksThisTurn(bankAtk, move) == 2
	&& (!(flags & MOVE_RESULT_SUPER_EFFECTIVE) || ((flags & (MOVE_RESULT_SUPER_EFFECTIVE | MOVE_RESULT_NOT_VERY_EFFECTIVE)) == (MOVE_RESULT_SUPER_EFFECTIVE | MOVE_RESULT_NOT_VERY_EFFECTIVE)))
	&& gBattleMoves[move].power
	&& SPLIT(move) != SPLIT_STATUS)
	{
		flags |= MOVE_RESULT_MISSED;
	}

	return flags;
}

//The function allows the AI to do type calculations from a move onto one of their partied mons
u8 AI_TypeCalc(u16 move, u8 bankAtk, u8 bankDef, struct Pokemon* monDef) //bankDef is used here as the slot monDef will go into
{
	if (move == MOVE_STRUGGLE)
		return 0;

	u8 flags = 0;

	u8 defAbility = GetMonAbilityAfterTrace(monDef, bankAtk);
	u8 defEffect = ItemId_GetHoldEffectParam(monDef->item);
	u8 defType1 = GetMonType(monDef, 0);
	u8 defType2 = GetMonType(monDef, 1);
	u8 defType3 = TYPE_BLANK; //Used for Imposter

	u8 atkAbility = ABILITY(bankAtk);
	u8 atkType1 = gBattleMons[bankAtk].type1;
	u8 atkType2 = gBattleMons[bankAtk].type2;
	u8 atkType3 = gBattleMons[bankAtk].type3;
	u8 moveType = GetMoveTypeSpecial(bankAtk, move);

	if (defAbility == ABILITY_IMPOSTER && ImposterWorks(bankDef, TRUE))
	{
		u8 imposterBank = GetImposterBank(bankDef);
		defAbility = ABILITY(imposterBank);
		defType1 = gBattleMons[imposterBank].type1;
		defType2 = gBattleMons[imposterBank].type2;
		defType3 = gBattleMons[imposterBank].type3;
	}

	if (IsTargetAbilityIgnored(defAbility, atkAbility, move))
		defAbility = ABILITY_NONE; //Ignore Ability

	//Check STAB
	if (atkType1 == moveType || atkType2 == moveType || atkType3 == moveType
	|| atkAbility == ABILITY_PROTEAN) //Will change type to get STAB
	{
		if (atkAbility == ABILITY_ADAPTABILITY)
			gBattleMoveDamage *= 2;
		else
			gBattleMoveDamage = udivsi(gBattleMoveDamage * 15, 10);
	}

	//Check Special Ground Immunities
	if (moveType == TYPE_GROUND
	&& !CheckMonGrounding(monDef)
	&& ((defAbility == ABILITY_LEVITATE && NO_MOLD_BREAKERS(atkAbility, move))
	 || (defEffect == ITEM_EFFECT_AIR_BALLOON && defAbility != ABILITY_KLUTZ)
	 || IsMonFloatingWithMagnetism(monDef))
	&& move != MOVE_THOUSANDARROWS)
	{
		flags = MOVE_RESULT_MISSED | MOVE_RESULT_DOESNT_AFFECT_FOE;
	}
	else if (gSpecialMoveFlags[move].gPowderMoves && !IsAffectedByPowderByDetails(defType1, defType2, 0xFF, defAbility, defEffect))
	{
		flags |= (MOVE_RESULT_MISSED | MOVE_RESULT_DOESNT_AFFECT_FOE);
	}
	else if ((gBattleMoves[move].effect == EFFECT_SKY_DROP && (defType1 == TYPE_FLYING || defType2 == TYPE_FLYING))
	|| (move == MOVE_SYNCHRONOISE && WillSyncronoiseFailByAttackerTypesAnd2DefTypesAndItemEffect(atkType1, atkType2, atkType3, defType1, defType2, defEffect)))
	{
		flags |= (MOVE_RESULT_DOESNT_AFFECT_FOE);
	}
	else
	{
		TypeDamageModificationByDefTypes(atkAbility, bankDef, move, moveType, &flags, defType1, defType2, defType3, monDef);
	}

	if (defAbility == ABILITY_WONDERGUARD
	&& (!(flags & MOVE_RESULT_SUPER_EFFECTIVE) || ((flags & (MOVE_RESULT_SUPER_EFFECTIVE | MOVE_RESULT_NOT_VERY_EFFECTIVE)) == (MOVE_RESULT_SUPER_EFFECTIVE | MOVE_RESULT_NOT_VERY_EFFECTIVE)))
	&& gBattleMoves[move].power)
	{
		flags |= MOVE_RESULT_DOESNT_AFFECT_FOE;
	}

	return flags;
}

//This calc takes into account things like Pokemon Mega Evolving, Protean, & Illusion
u8 AI_SpecialTypeCalc(u16 move, u8 bankAtk, u8 bankDef)
{
	if (move == MOVE_STRUGGLE)
		return 0;

	u8 moveType;
	u8 atkAbility = GetAIAbility(bankAtk, bankDef, move);
	u8 defAbility = GetAIAbility(bankDef, bankAtk, IsValidMovePrediction(bankDef, bankAtk));
	u8 defEffect = ITEM_EFFECT(bankDef);
	u8 atkType1, atkType2, atkType3, defType1, defType2, defType3;
	u8 flags = 0;

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
		defAbility = GetMonAbility(illusionMon);
		defType1 = gBaseStats[fakeSpecies].type1;
		defType2 = gBaseStats[fakeSpecies].type2;
	}
	else
	{
		defType1 = gBattleMons[bankDef].type1;
		defType2 = gBattleMons[bankDef].type2;
	}
	defType3 = gBattleMons[bankDef].type3; //Same type 3 - eg switched in on Forest's curse

	if (IsTargetAbilityIgnored(defAbility, atkAbility, move))
		defAbility = ABILITY_NONE; //Ignore Ability

	//Check STAB
	if (atkType1 == moveType || atkType2 == moveType || atkType3 == moveType
	|| atkAbility == ABILITY_PROTEAN) //Will change type to get STAB
	{
		if (atkAbility == ABILITY_ADAPTABILITY)
			gBattleMoveDamage *= 2;
		else
			gBattleMoveDamage = (gBattleMoveDamage * 15) / 10;
	}

	//Check Special Ground Immunities
	if (moveType == TYPE_GROUND
	&& !CheckGrounding(bankDef)
	&& ((defAbility == ABILITY_LEVITATE && NO_MOLD_BREAKERS(atkAbility, move))
	 || defEffect == ITEM_EFFECT_AIR_BALLOON
	 || (gStatuses3[bankDef] & (STATUS3_LEVITATING | STATUS3_TELEKINESIS))
	 || IsFloatingWithMagnetism(bankDef))
	&& move != MOVE_THOUSANDARROWS)
	{
		flags |= (MOVE_RESULT_MISSED | MOVE_RESULT_DOESNT_AFFECT_FOE);
	}
	else if (gSpecialMoveFlags[move].gPowderMoves && !IsAffectedByPowderByDetails(defType1, defType2, defType3, defAbility, defEffect))
	{
		flags |= (MOVE_RESULT_MISSED | MOVE_RESULT_DOESNT_AFFECT_FOE);
	}
	else if ((gBattleMoves[move].effect == EFFECT_SKY_DROP && (defType1 == TYPE_FLYING || defType2 == TYPE_FLYING || defType3 == TYPE_FLYING))
	|| (move == MOVE_SYNCHRONOISE && WillSyncronoiseFailByAttackerTypesAnd3DefTypesAndItemEffect(atkType1, atkType2, atkType3, defType1, defType2, defType3, defEffect)))
	{
		flags |= (MOVE_RESULT_DOESNT_AFFECT_FOE);
	}
	else //Regular Type Calc
		TypeDamageModificationByDefTypes(atkAbility, bankDef, move, moveType, &flags, defType1, defType2, defType3, NULL);

	//Wonder Guard Check
	if (defAbility == ABILITY_WONDERGUARD
	&& NO_MOLD_BREAKERS(atkAbility, move)
	&& !(flags & MOVE_RESULT_MISSED)
	&& AttacksThisTurn(bankAtk, move) == 2
	&& (!(flags & MOVE_RESULT_SUPER_EFFECTIVE) || ((flags & (MOVE_RESULT_SUPER_EFFECTIVE | MOVE_RESULT_NOT_VERY_EFFECTIVE)) == (MOVE_RESULT_SUPER_EFFECTIVE | MOVE_RESULT_NOT_VERY_EFFECTIVE)))
	&& gBattleMoves[move].power
	&& SPLIT(move) != SPLIT_STATUS)
	{
		flags |= MOVE_RESULT_MISSED;
	}

	if (flags & MOVE_RESULT_NO_EFFECT)
		gBattleMoveDamage = 0;

	return flags;
}

//The TypeCalc for showing move effectiveness on the move menu
u8 VisualTypeCalc(u16 move, u8 bankAtk, u8 bankDef)
{
	if (move == MOVE_STRUGGLE)
		return 0;

	u8 moveType, moveEffect;
	u8 defAbility;
	u8 defEffect = GetRecordedItemEffect(bankDef);
	u8 atkAbility, defType1, defType2, defType3;
	u8 flags = 0;

	atkAbility = ABILITY(bankAtk);
	moveType = GetMoveTypeSpecial(bankAtk, move);
	moveEffect = gBattleMoves[move].effect;

	struct Pokemon* monIllusion = GetIllusionPartyData(bankDef);
	bool8 underIllusion = monIllusion != GetBankPartyData(bankDef); 
	if (underIllusion)
	{
		u16 defSpecies = GetMonData(monIllusion, MON_DATA_SPECIES, NULL);
		defAbility = GetMonAbility(monIllusion);
		if ((gBaseStats[defSpecies].ability1 != ABILITY_NONE && gBaseStats[defSpecies].ability1 != defAbility)
		|| (gBaseStats[defSpecies].ability2 != ABILITY_NONE && gBaseStats[defSpecies].ability2 != defAbility)
		|| (gBaseStats[defSpecies].hiddenAbility != ABILITY_NONE && gBaseStats[defSpecies].hiddenAbility != defAbility))
			defAbility = ABILITY_NONE; //Mon could have multiple Abilities so don't reveal the correct one to the player

		defType1 = GetMonType(monIllusion, 0);
		defType2 = GetMonType(monIllusion, 1);
	}
	else
	{
		defAbility = GetRecordedAbility(bankDef);
		defType1 = gBattleMons[bankDef].type1;
		defType2 = gBattleMons[bankDef].type2;
	}

	defType3 = gBattleMons[bankDef].type3; //Not affected by Illusion

	if (IsTargetAbilityIgnored(defAbility, atkAbility, move))
		defAbility = ABILITY_NONE; //Ignore Ability

	if (IsDynamaxed(bankDef) && gSpecialMoveFlags[move].gDynamaxBannedMoves) //These moves aren't related to type matchups, but they can still cause the move to fail and should be known to the player
	{
		flags |= MOVE_RESULT_DOESNT_AFFECT_FOE;
	}
	else if (moveType == TYPE_GROUND //Check Special Ground Immunities
	&& SPLIT(move) != SPLIT_STATUS
	&& !NonInvasiveCheckGrounding(bankDef, defAbility, defType1, defType2, defType3)
	&& ((defAbility == ABILITY_LEVITATE && NO_MOLD_BREAKERS(atkAbility, move))
	 || defEffect == ITEM_EFFECT_AIR_BALLOON
	 || (gStatuses3[bankDef] & (STATUS3_LEVITATING | STATUS3_TELEKINESIS))
	 || IsFloatingWithMagnetism(bankDef)
	 || underIllusion) //Needed because ModulateDamageByType doesn't take Illusion into account - will still cause a mistake if Flying-type mon has Illusion, but that case is so rare it doesn't matter
	&& move != MOVE_THOUSANDARROWS)
	{
		flags |= MOVE_RESULT_DOESNT_AFFECT_FOE;
	}
	else if (gSpecialMoveFlags[move].gPowderMoves && !IsAffectedByPowderByDetails(defType1, defType2, defType3, defAbility, defEffect))
	{
		flags |= MOVE_RESULT_DOESNT_AFFECT_FOE;
	}
	else if (moveEffect == EFFECT_PARALYZE && (defType1 == TYPE_ELECTRIC || defType2 == TYPE_ELECTRIC || defType3 == TYPE_ELECTRIC))
	{
		flags |= MOVE_RESULT_DOESNT_AFFECT_FOE;
	}
	else if (moveEffect == EFFECT_WILL_O_WISP && (defType1 == TYPE_FIRE || defType2 == TYPE_FIRE || defType3 == TYPE_FIRE))
	{
		flags |= MOVE_RESULT_DOESNT_AFFECT_FOE;
	}
	else if ((moveEffect == EFFECT_POISON || moveEffect == EFFECT_TOXIC)
	&& atkAbility != ABILITY_CORROSION
	&& (defType1 == TYPE_POISON || defType2 == TYPE_POISON || defType3 == TYPE_POISON
	 || defType1 == TYPE_STEEL || defType2 == TYPE_STEEL || defType3 == TYPE_STEEL))
	{
		flags |= MOVE_RESULT_DOESNT_AFFECT_FOE;
	}
	else if (moveEffect == EFFECT_LEECH_SEED && (defType1 == TYPE_GRASS || defType2 == TYPE_GRASS || defType3 == TYPE_GRASS))
	{
		flags |= MOVE_RESULT_DOESNT_AFFECT_FOE;
	}
	else if (moveEffect == EFFECT_SKY_DROP && (defType1 == TYPE_FLYING || defType2 == TYPE_FLYING || defType3 == TYPE_FLYING))
	{
		flags |= MOVE_RESULT_DOESNT_AFFECT_FOE;
	}
	else if (move == MOVE_SYNCHRONOISE && WillSyncronoiseFailByAttackerAnd3DefTypesAndItemEffect(bankAtk, defType1, defType2, defType3, defEffect))
	{
		flags |= MOVE_RESULT_DOESNT_AFFECT_FOE;
	}
	else //Regular Type Calc
	{
		if (SPLIT(move) != SPLIT_STATUS || move == MOVE_THUNDERWAVE) //Thunder Wave is the only status move that doesn't affect based on type (Ground)
			TypeDamageModificationByDefTypes(atkAbility, bankDef, move, moveType, &flags, defType1, defType2, defType3, NULL);
	}

	if (SPLIT(move) == SPLIT_STATUS)
	{
		flags &= ~(MOVE_RESULT_SUPER_EFFECTIVE | MOVE_RESULT_NOT_VERY_EFFECTIVE); //Status moves can't be super/not very effective
		return flags; //Status moves ignore Wonder Guard and Primal weather
	}

	if (CheckTableForMovesEffect(move, gMoveEffectsThatIgnoreWeaknessResistance)
	|| moveEffect == EFFECT_0HKO)
		flags &= ~(MOVE_RESULT_SUPER_EFFECTIVE | MOVE_RESULT_NOT_VERY_EFFECTIVE); //These moves can't be super/not very effective

	//Wonder Guard Check
	if (defAbility == ABILITY_WONDERGUARD
	&& NO_MOLD_BREAKERS(atkAbility, move)
	&& (!(flags & MOVE_RESULT_SUPER_EFFECTIVE) || ((flags & (MOVE_RESULT_SUPER_EFFECTIVE | MOVE_RESULT_NOT_VERY_EFFECTIVE)) == (MOVE_RESULT_SUPER_EFFECTIVE | MOVE_RESULT_NOT_VERY_EFFECTIVE)))
	&& gBattleMoves[move].power
	&& SPLIT(move) != SPLIT_STATUS)
		flags |= MOVE_RESULT_DOESNT_AFFECT_FOE;

	//Primal Weather Check
	if (WEATHER_HAS_EFFECT
	&& ((gBattleWeather & WEATHER_SUN_PRIMAL && moveType == TYPE_WATER)
	 || (gBattleWeather & WEATHER_RAIN_PRIMAL && moveType == TYPE_FIRE)))
		flags |= MOVE_RESULT_DOESNT_AFFECT_FOE;

	return flags;
}

void FutureSightTypeCalc(void)
{
	struct Pokemon* futureSightMon;
	u8 backupType1, backupType2, backupType3;
	bool8 futureSightMonInParty = !IsFutureSightAttackerOnField(gBankTarget, gBankAttacker);

	if (futureSightMonInParty)
	{
		//Backup attacker types
		backupType1 = gBattleMons[gBankAttacker].type1;
		backupType2 = gBattleMons[gBankAttacker].type2;
		backupType3 = gBattleMons[gBankAttacker].type3;

		//Replace with party mon types
		futureSightMon = GetFutureSightMon(gBankTarget, gBankAttacker);
		gBattleMons[gBankAttacker].type1 = GetMonType(futureSightMon, 0); //Helps get STAB if necessary
		gBattleMons[gBankAttacker].type2 = GetMonType(futureSightMon, 1);
		gBattleMons[gBankAttacker].type3 = TYPE_BLANK;
	}

	atk06_typecalc();
	--gBattlescriptCurrInstr; //Offset the addition in the type calc function

	if (futureSightMonInParty)
	{
		//Restore attacker types
		gBattleMons[gBankAttacker].type1 = backupType1;
		gBattleMons[gBankAttacker].type2 = backupType2;
		gBattleMons[gBankAttacker].type3 = backupType3;
	}
}

void TypeDamageModification(u8 atkAbility, u8 bankDef, u16 move, u8 moveType, u8* flags)
{
	return TypeDamageModificationByDefTypes(atkAbility, bankDef, move, moveType, flags, gBattleMons[bankDef].type1, gBattleMons[bankDef].type2, gBattleMons[bankDef].type3, NULL);
}

static void TypeDamageModificationByDefTypes(u8 atkAbility, u8 bankDef, u16 move, u8 moveType, u8* flags, u8 defType1, u8 defType2, u8 defType3, struct Pokemon* monDef)
{
	u8 multiplier1, multiplier2, multiplier3;

TYPE_LOOP:
	multiplier1 = gTypeEffectiveness[moveType][defType1];
	multiplier2 = gTypeEffectiveness[moveType][defType2];
	multiplier3 = gTypeEffectiveness[moveType][defType3];
	
	//If the multiplier is 0, that means normal damage. No effect is 1 (it is modified to 0 later).
	ModulateDmgByType(multiplier1, move, moveType, defType1, bankDef, atkAbility, flags, monDef);

	if (defType1 != defType2)
		ModulateDmgByType(multiplier2, move, moveType, defType2, bankDef, atkAbility, flags, monDef);

	if (defType3 != defType1 && defType3 != defType2)
		ModulateDmgByType(multiplier3, move, moveType, defType3, bankDef, atkAbility, flags, monDef);

	if (move == MOVE_FLYINGPRESS && moveType != TYPE_FLYING)
	{
		moveType = TYPE_FLYING;
		goto TYPE_LOOP;
	}
}

void TypeDamageModificationPartyMon(u8 atkAbility, struct Pokemon* monDef, u16 move, u8 moveType, u8* flags)
{
	u8 defType1, defType2, multiplier1, multiplier2;

	defType1 = GetMonType(monDef, 0);
	defType2 = GetMonType(monDef, 1);

TYPE_LOOP_AI:
	multiplier1 = gTypeEffectiveness[moveType][defType1];
	multiplier2 = gTypeEffectiveness[moveType][defType2];

	ModulateDmgByType(multiplier1, move, moveType, defType1, 0, atkAbility, flags, monDef);

	if (defType1 != defType2)
		ModulateDmgByType(multiplier2, move, moveType, defType2, 0, atkAbility, flags, monDef);

	if (move == MOVE_FLYINGPRESS && moveType != TYPE_FLYING)
	{
		moveType = TYPE_FLYING;
		goto TYPE_LOOP_AI;
	}
}

static void ModulateDmgByType(u8 multiplier, const u16 move, const u8 moveType, const u8 defType, const u8 bankDef, u8 atkAbility, u8* flags, struct Pokemon* monDef)
{
	bool8 checkMonDef = monDef != NULL;

	if (IsInverseBattle())
	{
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

	if (!checkMonDef && multiplier == TYPE_MUL_NO_EFFECT)
	{
		if ((defType == TYPE_GHOST && (moveType == TYPE_NORMAL || moveType == TYPE_FIGHTING))
		&& (gBattleMons[bankDef].status2 & STATUS2_FORESIGHT || atkAbility == ABILITY_SCRAPPY))
			return; //Foresight & Scrappy break Ghost immunity

		if (moveType == TYPE_PSYCHIC && defType == TYPE_DARK && (gStatuses3[bankDef] & STATUS3_MIRACLE_EYED))
			return; //Miracle Eye causes normal damage hits
	}
	else if (checkMonDef)
	{
		if (atkAbility == ABILITY_SCRAPPY
		&& (defType == TYPE_GHOST && (moveType == TYPE_NORMAL || moveType == TYPE_FIGHTING)))
			return; //Scrappy breaks Ghost immunity
	}

	if (move == MOVE_FREEZEDRY && defType == TYPE_WATER) //Always Super-Effective, even in Inverse Battles
		multiplier = TYPE_MUL_SUPER_EFFECTIVE;

	if (moveType == TYPE_FIRE && gNewBS->tarShotBits & gBitTable[bankDef]) //Fire always Super-Effective if covered in tar
		multiplier = TYPE_MUL_SUPER_EFFECTIVE;

	if (defType == TYPE_FLYING && multiplier == TYPE_MUL_SUPER_EFFECTIVE && gBattleWeather & WEATHER_AIR_CURRENT_PRIMAL && move != MOVE_STEALTHROCK && WEATHER_HAS_EFFECT)
		multiplier = TYPE_MUL_NORMAL; //Actually changes the modifier including the "it's super effective" string

	if (checkMonDef)
	{
		if (multiplier == TYPE_MUL_NO_EFFECT && GetMonItemEffect(monDef) == ITEM_EFFECT_RING_TARGET)
			multiplier = TYPE_MUL_NORMAL;
		else if (multiplier == TYPE_MUL_NO_EFFECT && moveType == TYPE_GROUND)
		{
			if (move == MOVE_THOUSANDARROWS)
				multiplier = TYPE_MUL_NORMAL;
			else if (!gMain.inBattle) //Eg. when called from the Frontier overworld
			{
				if (CheckGroundingByDetails(GetMonData(monDef, MON_DATA_SPECIES, NULL), GetMonData(monDef, MON_DATA_HELD_ITEM, NULL), GetMonAbility(monDef)))
					multiplier = TYPE_MUL_NORMAL;
			}
			else
			{
				if (CheckMonGrounding(monDef))
					multiplier = TYPE_MUL_NORMAL;
			}
		}
	}
	else
	{
		if (multiplier == TYPE_MUL_NO_EFFECT && ITEM_EFFECT(bankDef) == ITEM_EFFECT_RING_TARGET)
			multiplier = TYPE_MUL_NORMAL;
		else if (multiplier == TYPE_MUL_NO_EFFECT && moveType == TYPE_GROUND)
		{
			if (CheckGrounding(bankDef))
				multiplier = TYPE_MUL_NORMAL;
			else if (move == MOVE_THOUSANDARROWS)
			{
				multiplier = TYPE_MUL_NORMAL;

				if (defType == TYPE_FLYING)
				{
					//Does neutral damage regardless of secondary type

					if (*flags & MOVE_RESULT_SUPER_EFFECTIVE) //Super Effective on the first type
					{
						*flags &= ~MOVE_RESULT_SUPER_EFFECTIVE;
						multiplier = TYPE_MUL_NOT_EFFECTIVE; //Counteract the boost given earlier
						goto SKIP_FLAG_CHANGE;
					}
					else if (*flags & MOVE_RESULT_NOT_VERY_EFFECTIVE)
					{
						*flags &= ~MOVE_RESULT_NOT_VERY_EFFECTIVE;
						multiplier = TYPE_MUL_SUPER_EFFECTIVE; //Counteract the boost given earlier
						goto SKIP_FLAG_CHANGE;
					}
				}
			}
		}
	}

	switch (multiplier) {
		case TYPE_MUL_NO_EFFECT:
			*flags |= MOVE_RESULT_DOESNT_AFFECT_FOE;
			*flags &= ~(MOVE_RESULT_NOT_VERY_EFFECTIVE | MOVE_RESULT_SUPER_EFFECTIVE);
			break;

		case TYPE_MUL_NOT_EFFECTIVE:
			if (SPLIT(move) != SPLIT_STATUS && !(*flags & MOVE_RESULT_NO_EFFECT))
			{
				if (*flags & MOVE_RESULT_SUPER_EFFECTIVE)
					*flags &= ~MOVE_RESULT_SUPER_EFFECTIVE;
				else
					*flags |= MOVE_RESULT_NOT_VERY_EFFECTIVE;
			}
			break;

		case TYPE_MUL_SUPER_EFFECTIVE:
			if (SPLIT(move) != SPLIT_STATUS && !(*flags & MOVE_RESULT_NO_EFFECT))
			{
				if (*flags & MOVE_RESULT_NOT_VERY_EFFECTIVE)
					*flags &= ~MOVE_RESULT_NOT_VERY_EFFECTIVE;
				else
					*flags |= MOVE_RESULT_SUPER_EFFECTIVE;
			}
			break;
	}

	SKIP_FLAG_CHANGE:
	if (multiplier != TYPE_MUL_NO_DATA && multiplier != TYPE_MUL_NORMAL)
	{
		if (multiplier == TYPE_MUL_NO_EFFECT)
			multiplier = 0; //Set to 0 properly

		gBattleMoveDamage = MathMax(1, (gBattleMoveDamage * multiplier) / 10);
	}
}

void ModulateByTypeEffectiveness(u8 moveType, u8 defType1, u8 defType2, u8* var)
{
	u8 multiplier1 = gTypeEffectiveness[moveType][defType1];
	u8 multiplier2 = gTypeEffectiveness[moveType][defType2];

	//Fix multiplier 1 calculation
	if (multiplier1 == TYPE_MUL_NO_DATA)
		multiplier1 = TYPE_MUL_NORMAL;
	else if (multiplier1 == TYPE_MUL_NO_EFFECT)
		multiplier1 = 0;

	//Fix multiplier 2 calculation
	if (multiplier2 == TYPE_MUL_NO_DATA)
		multiplier2 = TYPE_MUL_NORMAL;
	else if (multiplier2 == TYPE_MUL_NO_EFFECT)
		multiplier2 = 0;

	*var = (*var * multiplier1) / 10;
	*var = (*var * multiplier2) / 10;
}

u8 GetMoveTypeSpecial(u8 bankAtk, u16 move)
{
	u8 atkAbility = ABILITY(bankAtk);
	u8 moveType = GetMoveTypeSpecialPreAbility(move, bankAtk, NULL);
	if (moveType != 0xFF)
		return moveType;

	return GetMoveTypeSpecialPostAbility(move, atkAbility, gNewBS->zMoveData.active || gNewBS->zMoveData.viewing);
}

u8 GetMoveTypeSpecialPreAbility(u16 move, u8 bankAtk, struct Pokemon* monAtk)
{
	u8 moveType = gBattleMoves[move].type;

	if (monAtk == NULL && gNewBS->ElectrifyTimers[bankAtk] > 0)
		return TYPE_ELECTRIC;

	if (gSpecialMoveFlags[move].gTypeChangeExceptionMoves)
	{
		if (monAtk != NULL)
			return GetMonExceptionMoveType(monAtk, move);
		else
			return GetExceptionMoveType(bankAtk, move);
	}

	if (moveType == TYPE_NORMAL && monAtk == NULL && IsIonDelugeActive()
	&& !AbilityCanChangeTypeAndBoost(move, ABILITY(bankAtk), gNewBS->ElectrifyTimers[bankAtk], FALSE)) //Type-change abilities override Ion Deluge
		return TYPE_ELECTRIC;

	return 0xFF;
}

u8 GetMoveTypeSpecialPostAbility(u16 move, u8 atkAbility, bool8 zMoveActive)
{
	u8 moveType = gBattleMoves[move].type;
	bool8 moveTypeCanBeChanged = !zMoveActive || SPLIT(move) == SPLIT_STATUS;

	if (moveTypeCanBeChanged)
	{
		//Change Normal-type Moves
		if (moveType == TYPE_NORMAL)
		{
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
		switch (atkAbility) {
			case ABILITY_NORMALIZE:
				return TYPE_NORMAL;
			case ABILITY_LIQUIDVOICE:
				if (CheckSoundMove(move)) //Change Sound Moves
					return TYPE_WATER;
				break;
		}
	}

	return moveType;
}

u8 GetMonMoveTypeSpecial(struct Pokemon* mon, u16 move)
{
	u8 atkAbility = GetMonAbility(mon);
	u8 moveType = GetMoveTypeSpecialPreAbility(move, 0, mon);
	if (moveType != 0xFF)
		return moveType;

	return GetMoveTypeSpecialPostAbility(move, atkAbility, FALSE);
}

static bool8 AbilityCanChangeTypeAndBoost(u16 move, u8 atkAbility, u8 electrifyTimer, bool8 zMoveActive)
{
	u8 moveType = gBattleMoves[move].type;
	bool8 moveTypeCanBeChanged = !zMoveActive || SPLIT(move) == SPLIT_STATUS;

	if (electrifyTimer > 0
	|| IsAnyMaxMove(move)
	|| gSpecialMoveFlags[move].gTypeChangeExceptionMoves)
		return FALSE;

	//Check Normal-type Moves
	if (moveType == TYPE_NORMAL)
	{
		if (moveTypeCanBeChanged)
		{
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
	return atkAbility == ABILITY_NORMALIZE && moveTypeCanBeChanged;
}

u8 GetExceptionMoveType(u8 bankAtk, u16 move)
{
	u8 moveType = gBattleMoves[move].type;
	u16 item = ITEM(bankAtk);
	u8 effect = ITEM_EFFECT(bankAtk);
	u8 quality = ITEM_QUALITY(bankAtk);

	switch (move) {
		case MOVE_HIDDENPOWER:
			moveType = ((gBattleMons[bankAtk].hpIV & 1))
					 | ((gBattleMons[bankAtk].attackIV & 1) << 1)
					 | ((gBattleMons[bankAtk].defenseIV & 1) << 2)
					 | ((gBattleMons[bankAtk].speedIV & 1) << 3)
					 | ((gBattleMons[bankAtk].spAttackIV & 1) << 4)
					 | ((gBattleMons[bankAtk].spDefenseIV & 1) << 5);

			moveType = (15 * moveType) / 63 + 1;
			if (moveType >= TYPE_MYSTERY)
				moveType++;
			break;

		case MOVE_WEATHERBALL:
			if (gBattleWeather & WEATHER_RAIN_ANY && !ItemEffectIgnoresSunAndRain(effect) && WEATHER_HAS_EFFECT)
				moveType = TYPE_WATER;
			else if (gBattleWeather & WEATHER_SANDSTORM_ANY && WEATHER_HAS_EFFECT)
				moveType = TYPE_ROCK;
			else if (gBattleWeather & WEATHER_SUN_ANY && !ItemEffectIgnoresSunAndRain(effect) && WEATHER_HAS_EFFECT)
				moveType = TYPE_FIRE;
			else if (gBattleWeather & WEATHER_HAIL_ANY && WEATHER_HAS_EFFECT)
				moveType = TYPE_ICE;
			break;

		case MOVE_NATURALGIFT:
			moveType = GetNaturalGiftMoveType(item);
			break;

		case MOVE_JUDGMENT:
			if (effect == ITEM_EFFECT_PLATE)
				moveType = quality;
			else
				moveType = TYPE_NORMAL;
			break;

		case MOVE_MULTIATTACK:
			#ifdef NATIONAL_DEX_SILVALLY
			if (effect == ITEM_EFFECT_MEMORY
			&& SpeciesToNationalPokedexNum(SPECIES(bankAtk)) == NATIONAL_DEX_SILVALLY)
				moveType = quality;
			else
			#endif
				moveType = TYPE_NORMAL;
			break;

		case MOVE_TECHNOBLAST:
			if (effect == ITEM_EFFECT_DRIVE)
				moveType = quality;
			else
				moveType = TYPE_NORMAL;
			break;

		//Based on https://bulbapedia.bulbagarden.net/wiki/Revelation_Dance_(move)
		case MOVE_RAGINGBULL:
		case MOVE_REVELATIONDANCE: ;
			u8 atkType1 = gBattleMons[bankAtk].type1;
			u8 atkType2 = gBattleMons[bankAtk].type2;
			u8 atkType3 = gBattleMons[bankAtk].type3;
			if (gNewBS->DancerInProgress)
			{
				if (atkType1 != TYPE_MYSTERY && atkType1 != TYPE_ROOSTLESS)
					moveType = atkType1;
				else if (atkType2 != TYPE_MYSTERY && atkType2 != TYPE_ROOSTLESS)
					moveType = atkType2;
				else
					moveType = TYPE_NORMAL;
			}
			else
			{
				if (atkType1 != TYPE_MYSTERY && atkType1 != TYPE_ROOSTLESS)
					moveType = atkType1;
				else if (atkType2 != TYPE_MYSTERY && atkType2 != TYPE_ROOSTLESS)
					moveType = atkType2;
				else if (atkType3 != TYPE_MYSTERY && atkType3 != TYPE_ROOSTLESS && atkType3 != TYPE_BLANK)
					moveType = atkType3;
				else
					moveType = TYPE_MYSTERY;
			}
			break;

		case MOVE_AURAWHEEL:
			#ifdef SPECIES_MORPEKO_HANGRY
			if (SPECIES(bankAtk) == SPECIES_MORPEKO_HANGRY)
				moveType = TYPE_DARK;
			#endif
			break;
		
		case MOVE_IVYCUDGEL:
			#if (defined SPECIES_OGERPON && SPECIES_OGERPON_WELLSPRING_MASK && SPECIES_OGERPON_HEARTHFLAME_MASK && SPECIES_OGERPON_CORNERSTONE_MASK)
			if (SPECIES(bankAtk) == SPECIES_OGERPON_CORNERSTONE_MASK)
				moveType = TYPE_ROCK;
			else if (SPECIES(bankAtk) == SPECIES_OGERPON_WELLSPRING_MASK)
				moveType = TYPE_WATER;
			else if (SPECIES(bankAtk) == SPECIES_OGERPON_HEARTHFLAME_MASK)
				moveType = TYPE_FIRE;
			else
				moveType = TYPE_GRASS;
			#endif
			break;
		
		case MOVE_TERRAINPULSE:
			if (IsAffectedByElectricTerrain(bankAtk))
			{
				switch (gTerrainType) {
					case ELECTRIC_TERRAIN:
						moveType = TYPE_ELECTRIC;
						break;
					case GRASSY_TERRAIN:
						moveType = TYPE_GRASS;
						break;
					case MISTY_TERRAIN:
						moveType = TYPE_FAIRY;
						break;
					case PSYCHIC_TERRAIN:
						moveType = TYPE_PSYCHIC;
						break;
					default:
						moveType = TYPE_NORMAL;
						break;
				}
			}
			break;
	}

	if (moveType == TYPE_NORMAL && IsIonDelugeActive())
		moveType = TYPE_ELECTRIC;

	return moveType;
}

u8 GetMonExceptionMoveType(struct Pokemon* mon, u16 move)
{
	u8 moveType = gBattleMoves[move].type;
	u8 ability = GetMonAbility(mon);
	u16 item = mon->item;
	u8 effect = ItemId_GetHoldEffect(item);
	u8 quality = ItemId_GetHoldEffectParam(item);

	if (ability == ABILITY_KLUTZ)
		effect = 0;

	switch (move) {
		case MOVE_HIDDENPOWER:
			moveType = CalcMonHiddenPowerType(mon);
			break;

		case MOVE_WEATHERBALL:
			if (gMain.inBattle)
			{
				if (gBattleWeather & WEATHER_RAIN_ANY && !ItemEffectIgnoresSunAndRain(effect) && WEATHER_HAS_EFFECT)
					moveType = TYPE_WATER;
				else if (gBattleWeather & WEATHER_SANDSTORM_ANY && WEATHER_HAS_EFFECT)
					moveType = TYPE_ROCK;
				else if (gBattleWeather & WEATHER_SUN_ANY && !ItemEffectIgnoresSunAndRain(effect) && WEATHER_HAS_EFFECT)
					moveType = TYPE_FIRE;
				else if (gBattleWeather & WEATHER_HAIL_ANY && WEATHER_HAS_EFFECT)
					moveType = TYPE_ICE;
			}
			break;

		case MOVE_NATURALGIFT:
			moveType = GetNaturalGiftMoveType(item);
			break;

		case MOVE_JUDGMENT:
			if (effect == ITEM_EFFECT_PLATE && ability != ABILITY_KLUTZ)
				moveType = quality;
			else
				moveType = TYPE_NORMAL;
			break;

		case MOVE_MULTIATTACK:
			#ifdef NATIONAL_DEX_SILVALLY
			if (effect == ITEM_EFFECT_MEMORY && ability != ABILITY_KLUTZ
			&& SpeciesToNationalPokedexNum(mon->species) == NATIONAL_DEX_SILVALLY)
				moveType = quality;
			else
			#endif
				moveType = TYPE_NORMAL;
			break;

		case MOVE_TECHNOBLAST:
			if (effect == ITEM_EFFECT_DRIVE && ability != ABILITY_KLUTZ)
				moveType = quality;
			else
				moveType = TYPE_NORMAL;
			break;

		case MOVE_RAGINGBULL:
		case MOVE_REVELATIONDANCE:
			moveType = GetMonType(mon, 0);
			break;

		case MOVE_AURAWHEEL:
			#ifdef SPECIES_MORPEKO_HANGRY
			if (mon->species == SPECIES_MORPEKO_HANGRY)
				moveType = TYPE_DARK;
			else
			#endif
				moveType = TYPE_ELECTRIC;
			break;
		
		case MOVE_IVYCUDGEL:
			#if (defined SPECIES_OGERPON && SPECIES_OGERPON_WELLSPRING_MASK && SPECIES_OGERPON_HEARTHFLAME_MASK && SPECIES_OGERPON_CORNERSTONE_MASK)
			if (mon->species == SPECIES_OGERPON_CORNERSTONE_MASK)
				moveType = TYPE_ROCK;
			else if (mon->species == SPECIES_OGERPON_WELLSPRING_MASK)
				moveType = TYPE_WATER;
			else if (mon->species == SPECIES_OGERPON_HEARTHFLAME_MASK)
				moveType = TYPE_FIRE;
			else
				moveType = TYPE_GRASS;
			#endif
			break;

		case MOVE_TERRAINPULSE:
			if (gMain.inBattle && IsMonAffectedByElectricTerrain(mon))
			{
				switch (gTerrainType) {
					case ELECTRIC_TERRAIN:
						moveType = TYPE_ELECTRIC;
						break;
					case GRASSY_TERRAIN:
						moveType = TYPE_GRASS;
						break;
					case MISTY_TERRAIN:
						moveType = TYPE_FAIRY;
						break;
					case PSYCHIC_TERRAIN:
						moveType = TYPE_PSYCHIC;
						break;
					default:
						moveType = TYPE_NORMAL;
						break;
				}
			}
			break;
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
u8 GetSummaryScreenMoveType(u16 move, unusedArg struct Pokemon* mon) {
#ifdef DISPLAY_REAL_MOVE_TYPE_ON_MENU
	return GetMonMoveTypeSpecial(mon, move);
#else
	return gBattleMoves[move].type;
#endif
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void atk07_adjustnormaldamage(void)
{
	ApplyRandomDmgMultiplier();
	AdjustDamage(TRUE);
}

 void atk08_adjustnormaldamage2(void)
 {
	ApplyRandomDmgMultiplier();
	AdjustDamage(FALSE);
}

void atk69_adjustsetdamage(void)
{
	AdjustDamage(TRUE);
}

void AdjustDamage(bool8 checkFalseSwipe)
{
	s32 damage = gBattleMoveDamage;
	u8 resultFlags = gMoveResultFlags;
	u8 moveTarget = GetBaseMoveTarget(gCurrentMove, gBankAttacker);
	bool8 calcSpreadMove = checkFalseSwipe && IS_DOUBLE_BATTLE && moveTarget & (MOVE_TARGET_BOTH | MOVE_TARGET_ALL);
	gStringBank = gBankTarget;

	for (u32 bankDef = 0; bankDef < gBattlersCount; ++bankDef)
	{
		if (!calcSpreadMove) //Single target move
			bankDef = gBankTarget;
		else if (gNewBS->calculatedSpreadMoveData)
			break; //Already calculated adjusted damage
		else if (!BATTLER_ALIVE(bankDef) || bankDef == gBankAttacker
		|| (bankDef == PARTNER(gBankAttacker) && !(moveTarget & MOVE_TARGET_ALL))
		|| gNewBS->noResultString[bankDef])
			continue;

		if (calcSpreadMove)
		{
			damage = gNewBS->DamageTaken[bankDef];
			resultFlags = gNewBS->ResultFlags[bankDef];
		}

		u16 item = ITEM(bankDef);
		u8 itemEffect = ITEM_EFFECT(bankDef);
		u8 itemQuality = ITEM_QUALITY(bankDef);
		u8 defAbility = ABILITY(bankDef);

		if ((gNewBS->zMoveData.active || IsAnyMaxMove(gCurrentMove))
		&& !IsDynamaxed(bankDef)
		&& ProtectsAgainstZMoves(gCurrentMove, gBankAttacker, bankDef))
			damage = max(1, (damage  * 25) / 100);

		if (MoveBlockedBySubstitute(gCurrentMove, gBankAttacker, bankDef))
			goto END;

		if (bankDef == BANK_RAID_BOSS)
		{
			if (gNewBS->dynamaxData.raidShieldsUp) //Shields heavily reduce damage
			{
				u16 divisor = gBattleMons[bankDef].hp / 8;
				damage /= divisor; //1 hp of damage for every 8th of hp that would have been done

				if (damage >= gBattleMons[bankDef].hp)
					damage = gBattleMons[bankDef].hp - 1; //Can't KO while shields are up
				goto END;
			}
			else if (IsRaidBattle()) //Shields can start in a non-Dynamax battle, but they can only respawn in a Raid Battle
			{
				u16 cutOff = GetNextRaidShieldHP(bankDef);
				if (cutOff > 0 && gBattleMons[bankDef].hp - damage < cutOff)
				{
					damage = gBattleMons[bankDef].hp - cutOff; //Limit damage before Raid shields go up
					goto END;
				}
			}
		}

		if (IsAffectedBySturdy(defAbility, bankDef))
		{
			RecordAbilityBattle(bankDef, defAbility);
			gProtectStructs[bankDef].enduredSturdy = TRUE;
			gNewBS->EnduranceHelper[bankDef] = ENDURE_STURDY;
		}
		else if (IsAffectedByFocusSash(bankDef))
		{
			RecordItemEffectBattle(bankDef, itemEffect);
			gSpecialStatuses[bankDef].focusBanded = 1;
			gNewBS->EnduranceHelper[bankDef] = ENDURE_FOCUS_SASH;
		}
		else if (itemEffect == ITEM_EFFECT_FOCUS_BAND && Random() % 100 < itemQuality && !IsBankHoldingFocusSash(bankDef))
		{
			RecordItemEffectBattle(bankDef, itemEffect);
			gSpecialStatuses[bankDef].focusBanded = TRUE;
		}

		if (checkFalseSwipe)
		{
			if (gBattleMoves[gCurrentMove].effect != EFFECT_FALSE_SWIPE
			&& !gProtectStructs[bankDef].endured
			&& !gProtectStructs[bankDef].enduredSturdy
			&& !gSpecialStatuses[bankDef].focusBanded)
				goto END;
		}
		else
		{
			if (!gProtectStructs[bankDef].endured
			&& !gProtectStructs[bankDef].enduredSturdy
			&& !gSpecialStatuses[bankDef].focusBanded)
				goto END;
		}

		if (gBattleMons[bankDef].hp > damage)
			goto END;

		damage = gBattleMons[bankDef].hp - 1;

		if (gProtectStructs[bankDef].endured || gProtectStructs[bankDef].enduredSturdy)
		{
			gNewBS->enduredDamage |= gBitTable[bankDef]; //Helps contact Abilities still work even if no damage was done (fixes a bug from vanilla FR)
			resultFlags |= MOVE_RESULT_FOE_ENDURED;
			goto END;
		}

		if (gSpecialStatuses[bankDef].focusBanded)
		{
			gNewBS->enduredDamage |= gBitTable[bankDef];
			resultFlags |= MOVE_RESULT_FOE_HUNG_ON;
			gLastUsedItem = item;
		}

		END:		
		if (!calcSpreadMove)
			break; //Only 1 target
		else
		{
			gNewBS->DamageTaken[bankDef] = damage;
			gNewBS->ResultFlags[bankDef] = resultFlags;
		}
	}

	if (calcSpreadMove)
	{
		gBattleMoveDamage = gNewBS->DamageTaken[gBankTarget];
		gMoveResultFlags = gNewBS->ResultFlags[gBankTarget];
	}
	else
	{
		gBattleMoveDamage = damage;
		gMoveResultFlags = resultFlags;
	}

	++gBattlescriptCurrInstr;
}

//Only for the AI
u32 TryAdjustDamageForRaidBoss(u8 bankDef, u32 damage)
{
	if (bankDef == BANK_RAID_BOSS)
	{
		if (gNewBS->dynamaxData.raidShieldsUp) //Shields heavily reduce damage
		{
			//Pretend all moves do max damage in case partner breaks shield
			if (damage >= gBattleMons[bankDef].hp)
				damage = gBattleMons[bankDef].hp - 1; //Can't KO while shields are up
		}
		else if (IsRaidBattle()) //Shields can start in a non-Dynamax battle, but they can only respawn in a Raid Battle
		{
			u16 cutOff = GetNextRaidShieldHP(bankDef);
			if (cutOff > 0 && gBattleMons[bankDef].hp - damage < cutOff)
				damage = gBattleMons[bankDef].hp - cutOff; //Limit damage before Raid shields go up
		}
	}

	return damage;
}

void PopulateDamageCalcStructWithBaseAttackerData(struct DamageCalc* data)
{
	u8 bankAtk = data->bankAtk;
	bool8 useMonAtk = data->monAtk != NULL;

	if (useMonAtk)
	{
		u8 side = SIDE(bankAtk);
		struct Pokemon* monAtk = data->monAtk;

		if (!(data->specialFlags & FLAG_FUTURE_SIGHT_DAMAGE)) //Ignores Abilities and held items if mon who used Future Sight isn't on the field
		{
			data->atkAbility = GetMonAbilityAfterTrace(monAtk, FOE(side));
			data->atkItemEffect = GetMonItemEffect(monAtk);
			data->atkItem = monAtk->item;
			data->atkItemQuality = ItemId_GetHoldEffectParam(monAtk->item);
			
			if (data->atkAbility == ABILITY_IMPOSTER && ImposterWorks(bankAtk, TRUE))
			{
				data->atkImposter = TRUE;
				data->atkImposterBank = GetImposterBank(bankAtk);
				data->atkIsGrounded = CheckGrounding(data->atkImposterBank);
			}
			else
				data->atkIsGrounded = CheckMonGrounding(monAtk); //Set later on for an imposter mon
		}

		data->atkPartnerAbility = ABILITY_NONE;
		data->atkHP = monAtk->hp;
		data->atkMaxHP = monAtk->maxHP;
		data->atkStatus3 = 0;

		if (data->atkImposter) //Would only be set in a non-Future Sight damage calc
		{
			u8 imposterBank = data->atkImposterBank;
			data->atkAbility = ABILITY(imposterBank);
			data->atkSpecies = SPECIES(imposterBank);
			data->atkSpeed = SpeedCalc(imposterBank);
		}
		else
		{
			data->atkSpecies = monAtk->species;
			data->atkSpeed = SpeedCalcMon(side, monAtk);
		}

		data->atkStatus1 = monAtk->condition;
		if (data->atkStatus1 == 0 && !(data->specialFlags & FLAG_FUTURE_SIGHT_DAMAGE))
		{
			if (gSideTimers[side].tspikesAmount > 0
			&& data->atkIsGrounded
			&& !IsMonOfType(monAtk, TYPE_POISON)
			&& IsMonAffectedByHazardsByItemEffect(monAtk, data->atkItemEffect) //Affected by hazards
			&& !BankSideHasSafeguard(bankAtk)
			&& CanPartyMonBePoisoned(monAtk))
				data->atkStatus1 = STATUS1_POISON; //Will be poisoned - relevant for Facade
			//else //TO-DO Flame Orb when switching in
			//	data->atkStatus1 = GetMonPotentialStatus1(monAtk, data->atkItemEffect);
		}
	}
	else //Load from bank
	{
		data->atkSpecies = SPECIES(bankAtk);
		data->atkAbility = ABILITY(bankAtk);
		if (IS_DOUBLE_BATTLE && BATTLER_ALIVE(PARTNER(bankAtk)))
			data->atkPartnerAbility = ABILITY(PARTNER(bankAtk));
		else
			data->atkPartnerAbility = 0;
		data->atkItem = ITEM(bankAtk);
		data->atkItemEffect = ITEM_EFFECT(bankAtk);
		data->atkItemQuality = ITEM_QUALITY(bankAtk);

		data->atkHP = gBattleMons[bankAtk].hp;
		data->atkMaxHP = gBattleMons[bankAtk].maxHP;
		data->atkSpeed = SpeedCalc(bankAtk);
		data->atkStatus1 = gBattleMons[bankAtk].status1;
		data->atkStatus3 = gStatuses3[bankAtk];
		data->atkIsGrounded = CheckGrounding(bankAtk);
	}
	
	data->attackerLoaded = TRUE;
}

void PopulateDamageCalcStructWithBaseDefenderData(struct DamageCalc* data)
{
	bool8 useMonDef = data->monDef != NULL;
	u8 bankDef = data->bankDef;

	if (useMonDef)
	{
		u8 side = SIDE(bankDef);
		struct Pokemon* monDef = data->monDef;
		data->defAbility = GetMonAbilityAfterTrace(monDef, FOE(side));

		if (data->defAbility == ABILITY_IMPOSTER && ImposterWorks(bankDef, TRUE))
		{
			//Copy imposter bank's details
			u8 imposterBank;
			data->defImposter = TRUE;
			data->defImposterBank = imposterBank = GetImposterBank(bankDef);
			data->defAbility = ABILITY(imposterBank);

			data->defSpecies = SPECIES(imposterBank);
			data->defSpeed = SpeedCalc(imposterBank);
			data->defIsGrounded = CheckGrounding(imposterBank);
			data->defBuff = STAT_STAGE(imposterBank, STAT_STAGE_DEF);
			data->spDefBuff = STAT_STAGE(imposterBank, STAT_STAGE_SPDEF);
			
			if (IsWonderRoomActive())
			{
				data->defense = gBattleMons[imposterBank].spDefense;
				data->spDefense = gBattleMons[imposterBank].defense;
			}
			else
			{
				data->defense = gBattleMons[imposterBank].defense;
				data->spDefense = gBattleMons[imposterBank].spDefense;
			}
			
			if (IsAffectedByPixies(imposterBank))
			{
				if (data->defense <= data->spDefense)
					data->defBuff = min(data->defBuff + 1, STAT_STAGE_MAX);
				else
					data->spDefBuff = min(data->spDefBuff + 1, STAT_STAGE_MAX);
			}
		}
		else
		{
			data->defSpecies = monDef->species;
			data->defSpeed = SpeedCalcMon(side, monDef);
			data->defIsGrounded = CheckMonGrounding(monDef);
			data->defBuff = 6;
			data->spDefBuff = 6;
			
			if (IsWonderRoomActive())
			{
				data->defense = monDef->spDefense;
				data->spDefense = monDef->defense;
			}
			else
			{
				data->defense = monDef->defense;
				data->spDefense = monDef->spDefense;
			}

			if (IsMonAffectedByPixies(monDef))
			{
				if (data->defense <= data->spDefense)
					data->defBuff += 1;
				else
					data->spDefBuff += 1;
			}
		}

		data->defPartnerAbility = ABILITY_NONE;
		data->defItemEffect = GetMonItemEffect(monDef);
		data->defItemQuality = ItemId_GetHoldEffectParam(monDef->item);
		data->defHP = monDef->hp;
		data->defMaxHP = monDef->maxHP;
		data->defStatus3 = 0;
		data->defSideStatus = gSideStatuses[side];
		
		if (data->defAbility == ABILITY_DAUNTLESSSHIELD)
			data->defBuff = min(data->defBuff + 1, STAT_STAGE_MAX);

		TryBoostMonDefensesForTotemBoost(data, bankDef);

		if (monDef->condition == 0
		&& gSideTimers[side].tspikesAmount > 0
		&& data->defIsGrounded
		&& !IsMonOfType(monDef, TYPE_POISON) //Hazards damage before Imposter activates
		&& IsMonAffectedByHazardsByItemEffect(monDef, data->defItemEffect) //Affected by hazards
		&& !BankSideHasSafeguard(bankDef)
		&& CanPartyMonBePoisoned(monDef))
			data->defStatus1 = STATUS1_POISON; //Will be poisoned - relevant for things like Marvel Scale
		else
			data->defStatus1 = monDef->condition;
	}
	else //Load from bank
	{
		data->defSpecies = SPECIES(bankDef);
		data->defAbility = ABILITY(bankDef);
		if (IS_DOUBLE_BATTLE && BATTLER_ALIVE(PARTNER(bankDef)))
			data->defPartnerAbility = ABILITY(PARTNER(bankDef));
		else
			data->defPartnerAbility = 0;
		data->defItemEffect = ITEM_EFFECT(bankDef);
		data->defItemQuality = ITEM_QUALITY(bankDef);
		data->defHP = gBattleMons[bankDef].hp;
		data->defMaxHP = gBattleMons[bankDef].maxHP;
		data->defSpeed = SpeedCalc(bankDef);
		data->defStatus1 = gBattleMons[bankDef].status1;
		data->defStatus3 = gStatuses3[bankDef];
		data->defSideStatus = gSideStatuses[SIDE(bankDef)];
		data->defIsGrounded = CheckGrounding(bankDef);

		data->defBuff = STAT_STAGE(bankDef, STAT_STAGE_DEF);
		data->spDefBuff = STAT_STAGE(bankDef, STAT_STAGE_SPDEF);

		if (IsWonderRoomActive())
		{
			data->defense = gBattleMons[bankDef].spDefense;
			data->spDefense = gBattleMons[bankDef].defense;
		}
		else
		{
			data->defense = gBattleMons[bankDef].defense;
			data->spDefense = gBattleMons[bankDef].spDefense;
		}

		//Try to "hide" knowledge of Type-Resist Berries so they actually have a use being equipped
		if (data->defItemEffect == ITEM_EFFECT_WEAKNESS_BERRY
		&& data->specialFlags & FLAG_AI_CALC
		&& IsPlayerInControl(bankDef)
		&& (gBattleTypeFlags & BATTLE_TYPE_FRONTIER //Never allow knowledge in the Frontier
		#ifdef VAR_GAME_DIFFICULTY
		 || VarGet(VAR_GAME_DIFFICULTY) < OPTIONS_EXPERT_DIFFICULTY
		#endif
		))
		{
			data->defItemEffect = GetRecordedItemEffect(bankDef);
		}
	}

	data->defenderLoaded = TRUE;
}

static s32 CalculateBaseDamage(struct DamageCalc* data)
{
	u32 attack, spAttack, defense, spDefense;

	//Take variables off struct for easier access
	u8 bankAtk = data->bankAtk;
	u8 bankDef = data->bankDef;
	u16 move = data->move;

	u16 power = 0;
	u32 damage = 0;
	bool8 useMonAtk = data->monAtk != NULL;
	bool8 useMonDef = data->monDef != NULL;

	if (!data->attackerLoaded)
		PopulateDamageCalcStructWithBaseAttackerData(data);
	if (!data->defenderLoaded)
		PopulateDamageCalcStructWithBaseDefenderData(data);

	if (IsTargetAbilityIgnored(data->defAbility, data->atkAbility, move))
		data->defAbility = ABILITY_NONE; //Ignore Ability - modify original value intentionally

	//Create new variables so original values stay constant
	defense = data->defense;
	spDefense = data->spDefense;

//Load attacker Data
	if (useMonAtk)
	{
		u8 imposterBank = 0; //0 is needed otherwise compiler throws an unitialized variable warning (it wouldn't be, but whatever)
		u16 atkAbility = data->atkAbility;
		struct Pokemon* monAtk = data->monAtk;

		if (data->atkImposter) //Copy target's stats
		{
			imposterBank = data->atkImposterBank;
			data->atkBuff = STAT_STAGE(imposterBank, STAT_STAGE_ATK);
			data->spAtkBuff = STAT_STAGE(imposterBank, STAT_STAGE_SPATK);
		}
		else
		{
			data->atkBuff = 6;
			data->spAtkBuff = 6;
		}

		switch (move) {
			case MOVE_BODYPRESS:
				if (data->atkImposter)
				{
					attack = gBattleMons[imposterBank].defense;
					spAttack = gBattleMons[imposterBank].spDefense;
				}
				else
				{
					attack = monAtk->defense;
					spAttack = monAtk->spDefense;
				}

				//Factor in Abilities that will activate on switch-in
				if (atkAbility == ABILITY_DAUNTLESSSHIELD)
					data->atkBuff = min(data->atkBuff + 1, STAT_STAGE_MAX);

				TryBoostMonOffensesForTotemBoost(data, bankAtk, TRUE); //Check the Defense Totem Buff
				break;
			default:
				if (data->atkImposter)
				{
					attack = gBattleMons[imposterBank].attack;
					spAttack = gBattleMons[imposterBank].spAttack;
				}
				else
				{
					attack = monAtk->attack;
					spAttack = monAtk->spAttack;
				}

				//Factor in Abilities that will activate on switch-in
				if (atkAbility == ABILITY_INTREPIDSWORD)
					data->atkBuff = min(data->atkBuff + 1, STAT_STAGE_MAX);
				else if (atkAbility == ABILITY_DOWNLOAD)
				{
					u32 downloadDefense = defense;
					APPLY_QUICK_STAT_MOD(downloadDefense, data->defBuff);
					u32 downloadSpDefense = spDefense;
					APPLY_QUICK_STAT_MOD(downloadDefense, data->spDefBuff);

					if (downloadDefense < downloadSpDefense)
						data->atkBuff = min(data->atkBuff + 1, STAT_STAGE_MAX);
					else
						data->spAtkBuff = min(data->spAtkBuff + 1, STAT_STAGE_MAX);
				}
				else if (atkAbility == ABILITY_EVAPORATE
				&& RainCanBeEvaporated()
				&& SpeciesHasEvaporate(data->atkSpecies)
				&& !ItemEffectIgnoresSunAndRain(data->atkItemEffect))
					data->spAtkBuff = min(data->spAtkBuff + 1, STAT_STAGE_MAX);

				TryBoostMonOffensesForTotemBoost(data, bankAtk, FALSE);
				break;
		}

		data->moveSplit = CalcMoveSplitFromParty(move, monAtk);
		data->moveType = GetMonMoveTypeSpecial(monAtk, move);

		/*if (useMonDef) //CAN'T AND SHOULD NOT HAPPEN
			data->resultFlags = AI_TypeCalc(move, monAtk, bankDef, monDef);
		else*/
			data->resultFlags = TypeCalc(move, bankAtk, bankDef, monAtk);
	}
	else //Load from bank
	{
		switch (move) {
			case MOVE_BODYPRESS:
				attack = gBattleMons[bankAtk].defense;
				spAttack = gBattleMons[bankAtk].spDefense;
				if (IsWonderRoomActive())
				{
					//For some strange reason in SwSh, Wonder Room swaps the defense boosts
					//when using Body Press, as opposed to the actual stat.
					data->atkBuff = STAT_STAGE(bankAtk, STAT_STAGE_SPDEF);
					data->spAtkBuff = STAT_STAGE(bankAtk, STAT_STAGE_DEF);
				}
				else
				{
					data->atkBuff = STAT_STAGE(bankAtk, STAT_STAGE_DEF);
					data->spAtkBuff = STAT_STAGE(bankAtk, STAT_STAGE_SPDEF);
				}
				break;
			default:
				attack = gBattleMons[bankAtk].attack;
				spAttack = gBattleMons[bankAtk].spAttack;
				data->atkBuff = STAT_STAGE(bankAtk, STAT_STAGE_ATK);
				data->spAtkBuff = STAT_STAGE(bankAtk, STAT_STAGE_SPATK);
		}

		data->moveSplit = CalcMoveSplit(move, bankAtk, bankDef);
		data->moveType = GetMoveTypeSpecial(bankAtk, move);

		if (useMonDef)
			data->resultFlags = AI_TypeCalc(move, bankAtk, bankDef, data->monDef);
		else if (data->specialFlags & FLAG_AI_CALC)
			data->resultFlags = AI_SpecialTypeCalc(move, bankAtk, bankDef); //Takes into account things like Illusion
		else
			data->resultFlags = TypeCalc(move, bankAtk, bankDef, NULL);
	}

//Load target data
	if (useMonDef)
	{
		u16 defAbility = data->defAbility;
		struct Pokemon* monDef = data->monDef;

		if (defAbility == ABILITY_INTIMIDATE) //Goes before Foul Play
		{
			if (!AbilityPreventsLoweringAtk(data->atkAbility)
			&& !AbilityBlocksIntimidate(data->atkAbility)
			&& !BankSideHasMist(bankAtk))
			{
				//Factor in Intimidate
				if (data->atkAbility == ABILITY_CONTRARY && data->atkBuff < STAT_STAGE_MAX)
					data->atkBuff = min(data->atkBuff + 1, STAT_STAGE_MAX);
				else if (data->atkBuff > STAT_STAGE_MIN)
					--data->atkBuff;
			}

			if (data->atkAbility == ABILITY_DEFIANT && data->atkBuff < STAT_STAGE_MAX)
				data->atkBuff = min(data->atkBuff + 2, STAT_STAGE_MAX);
			else if (data->atkAbility == ABILITY_COMPETITIVE && data->spAtkBuff < STAT_STAGE_MAX)
				data->spAtkBuff = min(data->spAtkBuff + 2, STAT_STAGE_MAX);
		}

		switch (move) {
			case MOVE_FOULPLAY:
				if (data->defImposter) //Target has Imposter
				{
					//Copy attacker's data
					u8 imposterBank = data->defImposterBank; //Essentially becomes bankAtk here (in singles, 50/50 in doubles)
					attack = gBattleMons[imposterBank].attack;
					spAttack = gBattleMons[imposterBank].spAttack;
					data->atkBuff = STAT_STAGE(imposterBank, STAT_STAGE_ATK);
					data->spAtkBuff = STAT_STAGE(imposterBank, STAT_STAGE_SPATK);
					
					if (defAbility == ABILITY_INTREPIDSWORD)
						data->atkBuff = min(data->atkBuff + 1, STAT_STAGE_MAX);
				}
				else
				{
					attack = monDef->attack;
					spAttack = monDef->spAttack;
					data->atkBuff = (defAbility == ABILITY_INTREPIDSWORD) ? 7 : 6; //Party mon has no buffs usually
					data->spAtkBuff = 6;
				}

				TryBoostMonOffensesForTotemBoost(data, bankDef, FALSE); //Use target totem boost against it
				break;
		}
	}
	else //Load from bank
	{
		switch (move) {
			case MOVE_FOULPLAY:
				attack = gBattleMons[bankDef].attack;
				spAttack = gBattleMons[bankDef].spAttack;
				data->atkBuff = STAT_STAGE(bankDef, STAT_STAGE_ATK);
				data->spAtkBuff = STAT_STAGE(bankDef, STAT_STAGE_SPATK);
				break;
		}
	}

	data->basePower = gBattleMoves[move].power; //Save real base power for later

//Load correct move power
	if (data->specialFlags & FLAG_SPLINTER_DAMAGE)
	{
		power = data->basePower = 25;
		data->moveType = gBattleMoves[move].type;
	}
	if (!(data->specialFlags & FLAG_CONFUSION_DAMAGE))
	{
		power = GetBasePower(data);
		power = AdjustBasePower(data, power);
	}
	else //Self-confusion damage
	{
		gBattleMovePower = power = data->basePower;
		data->moveType = TYPE_MYSTERY;
		data->resultFlags = 0;
	}

//Attacker Ability Checks
	switch(data->atkAbility) {
		case ABILITY_HUGEPOWER:
		#ifdef ABILITY_PUREPOWER
		case ABILITY_PUREPOWER:
		#endif
		//2x Boost
			if (!IsScaleMonsBattle() //Too OP
			|| !IsSpeciesAffectedByScalemons(data->atkSpecies)) //Doesn't get the Scalemons boost
				attack *= 2;
			break;

		case ABILITY_FLOWERGIFT:
		//1.5x Boost
			if (WEATHER_HAS_EFFECT && (gBattleWeather & WEATHER_SUN_ANY)
			&& !ItemEffectIgnoresSunAndRain(data->atkItemEffect))
				attack = (attack * 15) / 10;
			break;

		case ABILITY_PLUS:
		case ABILITY_MINUS:
		//1.5x Boost
			if (IsPlusMinusAbility(data->atkPartnerAbility)) //Double battle check prior
				spAttack = (spAttack * 15) / 10;
			break;

		case ABILITY_HUSTLE:
		//1.5x Boost
			attack = (attack * 15) / 10;
			break;

		case ABILITY_GUTS:
		//1.5x Boost
			if (data->atkStatus1 & STATUS_ANY)
				attack = (attack * 15) / 10;
			break;

		case ABILITY_SOLARPOWER:
		//1.5x Boost
			if (WEATHER_HAS_EFFECT && (gBattleWeather & WEATHER_SUN_ANY)
			&& !ItemEffectIgnoresSunAndRain(data->atkItemEffect))
				spAttack = (spAttack * 15) / 10;
			break;

		case ABILITY_SLOWSTART:
		//0.5x Boost
			if (useMonAtk)
				attack /= 2; //Pokemon in the party would start with half atk
			else if (gNewBS->SlowStartTimers[bankAtk])
				attack /= 2;
			break;

		case ABILITY_DEFEATIST:
		//0.5x Boost
			if (data->atkHP <= (data->atkMaxHP / 2))
			{
				attack /= 2;
				spAttack /= 2;
			}
			break;

		case ABILITY_HADRONENGINE:
			//1.33x Boost
			if (gTerrainType == ELECTRIC_TERRAIN && SpeciesHasHadronEngine(SPECIES(bankAtk)) && data->atkIsGrounded)
				spAttack = (spAttack * 133) / 100;
			break;

		case ABILITY_FLASHFIRE:
		//1.5x Boost
			if (!useMonAtk && data->moveType == TYPE_FIRE
			&& (gBattleResources->flags->flags[bankAtk] & RESOURCE_FLAG_FLASH_FIRE))
				spAttack = (spAttack * 15) / 10;
			break;

		case ABILITY_OVERGROW:
		//1.5x Boost
			if (data->moveType == TYPE_GRASS && data->atkHP <= data->atkMaxHP / 3)
			{
				attack = (attack * 15) / 10;
				spAttack = (spAttack * 15) / 10;
			}
			break;
		case ABILITY_BLAZE:
		//1.5x Boost
			if (data->moveType == TYPE_FIRE && data->atkHP <= data->atkMaxHP / 3)
			{
				attack = (attack * 15) / 10;
				spAttack = (spAttack * 15) / 10;
			}
			break;
		case ABILITY_TORRENT:
		//1.5x Boost
			if (data->moveType == TYPE_WATER && data->atkHP <= data->atkMaxHP / 3)
			{
				attack = (attack * 15) / 10;
				spAttack = (spAttack * 15) / 10;
			}
			break;
		case ABILITY_SWARM:
		//1.5x Boost
			if (data->moveType == TYPE_BUG && data->atkHP <= data->atkMaxHP / 3)
			{
				attack = (attack * 15) / 10;
				spAttack = (spAttack * 15) / 10;
			}
			break;

		case ABILITY_GORILLATACTICS:
		//1.5x Boost
			if (!IsDynamaxed(bankAtk))
				attack = (attack * 15) / 10;
			break;
	}

	switch (data->atkPartnerAbility) {
		case ABILITY_FLOWERGIFT:
			if (WEATHER_HAS_EFFECT && (gBattleWeather & WEATHER_SUN_ANY)
			&& !ItemEffectIgnoresSunAndRain(ITEM_EFFECT(PARTNER(bankAtk))))
				attack = (attack * 15) / 10;
			break;
	}

//Target Ability Checks
	switch (data->defAbility) {
		case ABILITY_MARVELSCALE:
		//1.5x Boost
			if (data->defStatus1 & STATUS_ANY)
				defense = (defense * 15) / 10;
			break;

		case ABILITY_GRASSPELT:
		//1.5x Boost
			if (gTerrainType == GRASSY_TERRAIN)
				defense = (defense * 15) / 10;
			break;

		case ABILITY_THICKFAT:
		//0.5x Decrement
			if (data->moveType == TYPE_FIRE || data->moveType == TYPE_ICE)
			{
				attack /= 2;
				spAttack /= 2;
			}
			break;

		case ABILITY_FURCOAT:
		//2x Boost
			defense *= 2;
			break;

		case ABILITY_PORTALPOWER:
		//0.75x Decrement
		#ifdef PORTAL_POWER
			if ((useMonAtk && !CheckContactByMon(move, data->monAtk))
			|| (!useMonAtk && !CheckContact(move, bankAtk, bankDef)))
			{
				attack = (attack * 75) / 100;
				spAttack = (spAttack * 75) / 100;
			}
		#endif
			break;
	}

//Attacker Item Checks
	switch (data->atkItemEffect) {
		case ITEM_EFFECT_CHOICE_BAND:
		//1.5x Boost
			if (!IsDynamaxed(bankAtk))
			{
				switch (data->atkItemQuality) {
					case QUALITY_CHOICE_BAND:
						attack = (attack * 15) / 10;
						break;

					case QUALITY_CHOICE_SPECS:
						spAttack = (spAttack * 15) / 10;
						break;
				}
			}
			break;

		#ifdef SPECIES_CUBONE
		case ITEM_EFFECT_THICK_CLUB:
		//2x Boost
			if (data->atkSpecies == SPECIES_CUBONE
			#ifdef SPECIES_MAROWAK
			||  data->atkSpecies == SPECIES_MAROWAK
			#endif
			#ifdef SPECIES_MAROWAK_A
			||  data->atkSpecies == SPECIES_MAROWAK_A
			#endif
			)
			{
				if (!IsScaleMonsBattle() //Too OP
				|| !IsSpeciesAffectedByScalemons(data->atkSpecies)) //Doesn't get the Scalemons boost
					attack *= 2;
			}
			break;
		#endif

		#ifdef NATIONAL_DEX_PIKACHU
		case ITEM_EFFECT_LIGHT_BALL:
		//2x Boost
			if (SpeciesToNationalPokedexNum(data->atkSpecies) == NATIONAL_DEX_PIKACHU)
			{
				attack *= 2;
				spAttack *= 2;
			}
			break;
		#endif

		#ifdef SPECIES_CLAMPERL
		case ITEM_EFFECT_DEEP_SEA_TOOTH:
		//2x Boost
			if (data->atkSpecies == SPECIES_CLAMPERL)
				spAttack *= 2;
			break;
		#endif

		#if (defined OLD_SOUL_DEW_EFFECT && defined SPECIES_LATIOS && defined SPECIES_LATIAS)
		case ITEM_EFFECT_SOUL_DEW:
		//1.5x Boost
			if (data->atkSpecies == SPECIES_LATIOS || data->atkSpecies == SPECIES_LATIAS)
				spAttack = (spAttack * 3) / 2;
			break;
		#endif
	}

//Target Item Checks
	switch (data->defItemEffect)
	{
		#if (defined OLD_SOUL_DEW_EFFECT && defined SPECIES_LATIOS && defined SPECIES_LATIAS)
		case ITEM_EFFECT_SOUL_DEW:
			if (data->defSpecies == SPECIES_LATIOS || data->defSpecies == SPECIES_LATIAS)
				spDefense = (spDefense * 3) / 2; //1.5
			break;
		#endif

		#ifdef SPECIES_DITTO
		case ITEM_EFFECT_METAL_POWDER:
			if (data->defSpecies == SPECIES_DITTO && (useMonDef || !IS_TRANSFORMED(bankDef)))
				defense *= 2;
			break;
		#endif

		#ifdef SPECIES_CLAMPERL
		case ITEM_EFFECT_DEEP_SEA_SCALE:
			if (data->defSpecies == SPECIES_CLAMPERL)
				spDefense *= 2;
			break;
		#endif

		case ITEM_EFFECT_EVIOLITE:
			if (CanSpeciesEvolve(data->defSpecies))
			{
				defense = (defense * 15) / 10;
				spDefense = (spDefense * 15) / 10;
			}
			break;

		case ITEM_EFFECT_ASSAULT_VEST:
			spDefense = (spDefense * 15) / 10;
			break;
	}

//Gym Badge Checks
	#ifdef BADGE_BOOSTS
	//1.1x Boosts
		if (!(gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_TRAINER_TOWER | BATTLE_TYPE_FRONTIER | BATTLE_TYPE_EREADER_TRAINER))
		&& gBattleTypeFlags & BATTLE_TYPE_TRAINER
		&& SIDE(bankAtk) == B_SIDE_PLAYER
		&& gTrainerBattleOpponent_A != TRAINER_SECRET_BASE)
		{
			if (FlagGet(FLAG_BADGE01_GET) && SIDE(bankAtk) == B_SIDE_PLAYER)
				attack = (11 * attack) / 10;

			if (FlagGet(FLAG_BADGE05_GET) && SIDE(bankDef) == B_SIDE_PLAYER)
				defense = (11 * defense) / 10;

			if (FlagGet(FLAG_BADGE07_GET) && SIDE(bankAtk) == B_SIDE_PLAYER)
				spAttack = (11 * spAttack) / 10;

			if (FlagGet(FLAG_BADGE07_GET) && SIDE(bankDef) == B_SIDE_PLAYER)
				spDefense = (11 * spDefense) / 10;
		}
	#endif

//Sandstorm Sp. Def Increase
	if (gBattleWeather & WEATHER_SANDSTORM_ANY && WEATHER_HAS_EFFECT)
	{
		if ((!useMonDef && IsOfType(bankDef, TYPE_ROCK)) || (useMonDef && IsMonOfType(data->monDef, TYPE_ROCK)))
			spDefense = (15 * spDefense) / 10;
		else if (gBattleWeather & WEATHER_SANDSTORM_PRIMAL
		&& ((!useMonDef && IsOfType(bankDef, TYPE_GROUND)) || (useMonDef && IsMonOfType(data->monDef, TYPE_GROUND))))
			spDefense = (15 * spDefense) / 10; //Ground types get a Sp. Def boost in a "Vicious Sandstorm"
	}

//Hail Def Increase
    if (WEATHER_HAS_EFFECT && (gBattleWeather & WEATHER_HAIL_ANY)
        && ((!useMonDef && IsOfType(bankDef, TYPE_ICE)) || (useMonDef && IsMonOfType(data->monDef, TYPE_ICE)))){
        defense = (defense * 15) / 10;
	}

	bool8 stallInField = FALSE;
	bool8 beadsOfRuinInField = FALSE;
	bool8 TabletsOfRuinInField = FALSE;
	bool8 VesselOfRuinInField = FALSE;
	bool8 SwordOfRuinInField = FALSE;
	u8 stallSide = 0xFF;  // Stores the side of the field where ABILITY_STALL was found

	// Check if any Pokémon on the field has ABILITY_STALL
	for (u8 bank = 0; bank < gBattlersCount; ++bank) {
		if (GetBankAbility(bank) == ABILITY_STALL) {
			stallInField = TRUE;
			stallSide = GetBattlerSide(bank); // Store the side of the field of the Pokémon with Stall
		}
		
		if (SpeciesHasBeadsofRuin(SPECIES(bank)) && GetBattlerSide(bank) == stallSide) {
			beadsOfRuinInField = TRUE;
		}
		if (SpeciesHasTabletsofRuin(SPECIES(bank)) && GetBattlerSide(bank) == stallSide) {
			TabletsOfRuinInField = TRUE;
		}
		if (SpeciesHasVesselofRuin(SPECIES(bank)) && GetBattlerSide(bank) == stallSide) {
			VesselOfRuinInField = TRUE;
		}
		if (SpeciesHasSwordofRuin(SPECIES(bank)) && GetBattlerSide(bank) == stallSide) {
			SwordOfRuinInField = TRUE;
		}

		// If both are true and on the same side, no need to continue the loop
		if (stallInField && (beadsOfRuinInField || TabletsOfRuinInField || VesselOfRuinInField || SwordOfRuinInField)) {
			break;
		}
	}

	if (stallInField && beadsOfRuinInField && !SpeciesHasBeadsofRuin(SPECIES(bankAtk))) {
		spDefense = (spDefense * 75) / 100;  // Reduce Sp defense by 25%
	}

	if (stallInField && TabletsOfRuinInField && !SpeciesHasTabletsofRuin(SPECIES(bankAtk))) {
		attack = (attack * 75) / 100;  // Reduce attack by 25%
	}

	if (stallInField && VesselOfRuinInField && !SpeciesHasVesselofRuin(SPECIES(bankAtk))) {
		spAttack = (spAttack * 75) / 100;  // Reduce Sp attack by 25%
	}

	if (stallInField && SwordOfRuinInField && !SpeciesHasSwordofRuin(SPECIES(bankAtk))) {
		defense = (defense * 75) / 100;  // Reduce defense by 25%
	}

//Old Exploding Check
	#ifdef OLD_EXPLOSION_BOOST
		if (move == MOVE_SELFDESTRUCT || move == MOVE_EXPLOSION)
			defense /= 2;
		else if (move == MOVE_MISTYEXPLOSION)
			spDefense /= 2;
	#endif

//Stat Buffs - Attacker
	if (data->defAbility != ABILITY_UNAWARE
	&& (data->atkBuff != 6 || data->spAtkBuff != 6)) //No point in wasting time with these calcs if mon has regular stats
	{
		if (gCritMultiplier > BASE_CRIT_MULTIPLIER)
		{
			if (data->atkBuff > 6)
				APPLY_QUICK_STAT_MOD(attack, data->atkBuff);

			if (data->spAtkBuff > 6)
				APPLY_QUICK_STAT_MOD(spAttack, data->spAtkBuff);
		}
		else
		{
			APPLY_QUICK_STAT_MOD(attack, data->atkBuff);
			APPLY_QUICK_STAT_MOD(spAttack, data->spAtkBuff);
		}
	}

//Stat Buffs - Target
	if (data->atkAbility != ABILITY_UNAWARE
	&& !gSpecialMoveFlags[move].gIgnoreStatChangesMoves
	&& (data->defBuff != 6 || data->spDefBuff != 6)) //No point in wasting time with these calcs if mon has regular stats
	{
		if (gCritMultiplier > BASE_CRIT_MULTIPLIER)
		{
			if (data->defBuff < 6)
				APPLY_QUICK_STAT_MOD(defense, data->defBuff);

			if (data->spDefBuff < 6)
				APPLY_QUICK_STAT_MOD(spDefense, data->spDefBuff);
		}
		else
		{
			APPLY_QUICK_STAT_MOD(defense, data->defBuff);
			APPLY_QUICK_STAT_MOD(spDefense, data->spDefBuff);
		}
	}

//Actual Calculation
	if (useMonAtk)
		damage = (2 * data->monAtk->level) / 5 + 2;
	else
		damage = (2 * gBattleMons[bankAtk].level) / 5 + 2;

	damage *= power;

	if (gSpecialMoveFlags[move].gSpecialAttackPhysicalDamageMoves)
	{
		damage *= spAttack;
		damage /= MathMax(1, defense); //MathMax prevents underflow
	}
	else
	{
		switch (data->moveSplit) {
			default:
			case SPLIT_PHYSICAL:
					damage *= attack;
					damage /= MathMax(1, defense);
					break;
			case SPLIT_SPECIAL:
					damage *= spAttack;
					damage /= MathMax(1, spDefense);
					break;
		}
	}

	damage /= 50;
	damage += 2;

//Final Damage mods - Likely boosts placed first, followed by damage cuts
	//Specific Move Modifiers
	switch (move) {
		case MOVE_ASSURANCE:
			if (!(data->specialFlags & (FLAG_CHECKING_FROM_MENU | FLAG_AI_CALC))
			&& (gProtectStructs[bankDef].physicalDmg || gProtectStructs[bankDef].specialDmg))
				damage *= 2;
			break;

		case MOVE_MAGNITUDE:
		case MOVE_EARTHQUAKE:
			if (data->defStatus3 & STATUS3_UNDERGROUND)
				damage *= 2;
			break;

		case MOVE_SURF:
			if (data->defStatus3 & STATUS3_UNDERWATER)
				damage *= 2;
			break;

		case MOVE_KNOCKOFF:
			if ((!useMonDef && CanKnockOffItem(bankDef))
			|| (useMonDef && CanKnockOffMonItem(data->monDef, SIDE(bankDef))))
				damage = (damage * 15) / 10;
			break;

		case MOVE_SOLARBEAM:
		case MOVE_SOLARBLADE:
			if (WEATHER_HAS_EFFECT
			&& !ItemEffectIgnoresSunAndRain(data->atkItemEffect)
			&& gBattleWeather & (WEATHER_RAIN_ANY | WEATHER_SANDSTORM_ANY | WEATHER_HAIL_ANY | WEATHER_FOG_ANY | WEATHER_AIR_CURRENT_PRIMAL))
				damage /= 2; //Any weather except sun weakens Solar Beam
			break;

		case MOVE_DYNAMAXCANNON:
		case MOVE_BEHEMOTHBLADE:
		case MOVE_BEHEMOTHBASH:
			if (IsDynamaxed(bankDef))
				damage *= 2;
			break;
	}

	//Stomp Minimize Boost
	if (data->defStatus3 & STATUS3_MINIMIZED && gSpecialMoveFlags[move].gAlwaysHitWhenMinimizedMoves)
		damage *= 2;

	//Me First Boost
	if (!(data->specialFlags & (FLAG_CHECKING_FROM_MENU | FLAG_AI_CALC)) && gNewBS->MeFirstByte)
		damage = (damage * 15) / 10;

	//Helping Hand Boost
	if (!(data->specialFlags & FLAG_AI_CALC) && gProtectStructs[bankAtk].helpingHand)
		damage = (damage * 15) / 10;

	//Glaive Rush Panalty
	if (data->defStatus3 & STATUS3_GLAIVERUSH)
		damage *= 2;

	//Punching Glove Boost
	if (ITEM_EFFECT(gBankAttacker) == ITEM_EFFECT_PUNCHING_GLOVE && gSpecialMoveFlags[move].gPunchingMoves)
		damage = (damage * 11) / 10;

	//Weather Boost
	if (WEATHER_HAS_EFFECT && !ItemEffectIgnoresSunAndRain(data->defItemEffect))
	{
		if (gBattleWeather & WEATHER_RAIN_ANY)
		{
			switch (data->moveType) {
				case TYPE_FIRE:
					damage /= 2;
					break;
				case TYPE_WATER:
					damage = (damage * 15) / 10;
					break;
			}
		}
		else if (gBattleWeather & WEATHER_SUN_ANY)
		{
			switch (data->moveType) {
				case TYPE_FIRE:
					damage = (damage * 15) / 10;
					break;
				case TYPE_WATER:
					if (gCurrentMove == MOVE_HYDROSTEAM)
						damage = (damage * 15) / 10;
					else
					damage /= 2;
					break;
			}
		}
	}

	//Aura Abilities
	if ((data->moveType == TYPE_DARK
		&& (ABILITY_ON_FIELD(ABILITY_DARKAURA) || data->atkAbility == ABILITY_DARKAURA || data->defAbility == ABILITY_DARKAURA)) //Check all because may be party mon
	||  (data->moveType == TYPE_FAIRY
		&& (ABILITY_ON_FIELD(ABILITY_FAIRYAURA) || data->atkAbility == ABILITY_FAIRYAURA || data->defAbility == ABILITY_FAIRYAURA)))
	{
		if (ABILITY_ON_FIELD(ABILITY_AURABREAK) || data->atkAbility == ABILITY_AURABREAK || data->defAbility == ABILITY_AURABREAK)
			damage = (damage * 75) / 100;
		else
			damage = (damage * 4) / 3;
	}

	//Second Attacker Ability Checks
	switch (data->atkAbility) {
		case ABILITY_TINTEDLENS:
			if (data->resultFlags & MOVE_RESULT_NOT_VERY_EFFECTIVE)
				damage *= 2;
			break;

		case ABILITY_SNIPER:
			if (gCritMultiplier > BASE_CRIT_MULTIPLIER)
				damage = (damage * 15) / 10;
			break;
	}

	//Second Attacker Item Checks
	switch (data->atkItemEffect) {
		case ITEM_EFFECT_EXPERT_BELT:
		//1.2x Boost
			if (data->resultFlags & MOVE_RESULT_SUPER_EFFECTIVE)
				damage = (damage * 12) / 10;
			break;
	}

	//Second Target Ability Checks
	switch (data->defAbility) {
		case ABILITY_DRYSKIN:
		//1.5x Boost
			if (data->moveType == TYPE_FIRE)
				damage = (damage * 125) / 100;
			break;

		#ifdef ABILITY_SOLIDROCK
		case ABILITY_SOLIDROCK:
		#endif
		case ABILITY_FILTER:
		case ABILITY_PRISMARMOR:
		//0.75x Decrement
			if (data->resultFlags & MOVE_RESULT_SUPER_EFFECTIVE)
				damage = (damage * 75) / 100;
			break;

		case ABILITY_HEATPROOF:
		case ABILITY_WATERBUBBLE:
		//0.5x Decrement
			if (data->moveType == TYPE_FIRE)
				damage /= 2;
			break;

		case ABILITY_MULTISCALE:
		case ABILITY_SHADOWSHIELD:
		//0.5x Decrement
			if (data->defHP >= data->defMaxHP)
				damage /= 2;
			break;

		case ABILITY_FLUFFY:
		//2x Boost / 0.5x Decrement
			if (data->moveType == TYPE_FIRE)
				damage *= 2;

			if ((useMonAtk && CheckContactByMon(move, data->monAtk))
			|| (!useMonAtk && CheckContact(move, bankAtk, bankDef)))
				damage /= 2;
			break;

		case ABILITY_PUNKROCK:
		//0.5x Decrement
			if (CheckSoundMove(move))
				damage /= 2; //50 % reduction
			break;

		case ABILITY_ICESCALES:
		//0.5x Decrement
			if (data->moveSplit == SPLIT_SPECIAL)
				damage /= 2;
			break;
	}

	//Second Target Partner Ability Checks
	if (data->defPartnerAbility == ABILITY_FRIENDGUARD)
		damage = (damage * 75) / 100;

	//Second Target Item Checks
	switch (data->defItemEffect) {
		case ITEM_EFFECT_WEAKNESS_BERRY:
			if (!UnnerveOnOpposingField(bankDef)
			&& !IsUnnerveAbility(data->atkAbility))
			{
				if ((data->resultFlags & MOVE_RESULT_SUPER_EFFECTIVE && data->defItemQuality == data->moveType)
				|| (data->defItemQuality == TYPE_NORMAL && data->moveType == TYPE_NORMAL)) //Chilan Berry
				{
					if (data->defAbility == ABILITY_RIPEN)
						damage /= 3;
					else
						damage /= 2;
				}
			}
			break;
	}

	//Screen Resistance
	if (gBattleMoves[move].effect != EFFECT_BRICK_BREAK) //Shatters the screens
	{
		switch (data->moveSplit) {
			case SPLIT_PHYSICAL:
				damage = ScreensWeakenDamage(damage, (data->defSideStatus & SIDE_STATUS_REFLECT) != 0, data->atkAbility, bankDef);
				break;
			case SPLIT_SPECIAL:
				damage = ScreensWeakenDamage(damage, (data->defSideStatus & SIDE_STATUS_LIGHTSCREEN) != 0, data->atkAbility, bankDef);
				break;
		}
	}

	//Burn Cut
	if (data->moveSplit == SPLIT_PHYSICAL
	&& (data->atkStatus1 & STATUS_BURN)
	&& data->atkAbility != ABILITY_GUTS
	&& move != MOVE_FACADE)
		damage /= 2;
	#ifdef FROSTBITE
	else if (data->moveSplit == SPLIT_SPECIAL
	&& data->atkStatus1 & STATUS_FREEZE)
		damage /= 2;
	#endif

	//Parental Bond Second Strike
	if (gNewBS->ParentalBondOn == 1)
	{
		#ifdef OLD_PARENTAL_BOND_DAMAGE
			damage /= 2;
		#else
			damage /= 4;
		#endif
	}

	//Spread Move Cut
	if (IS_DOUBLE_BATTLE)
	{
		u8 moveTarget = GetBaseMoveTargetByGrounding(move, data->atkIsGrounded);
		if (moveTarget & MOVE_TARGET_BOTH && CountAliveMonsInBattle(BATTLE_ALIVE_DEF_SIDE, bankAtk, bankDef) >= 2)
			damage = (damage * 75) / 100;

		else if (moveTarget & MOVE_TARGET_FOES_AND_ALLY && CountAliveMonsInBattle(BATTLE_ALIVE_EXCEPT_ACTIVE, bankAtk, bankDef) >= 2)
			damage = (damage * 75) / 100;
	}

	//Shadow Shield Battle
	//Ghosts get Shadow Shields (halve damage when at max health)
	if (data->defHP >= data->defMaxHP
	&& ((!useMonDef && IsAffectedByShadowShieldBattle(bankDef))
	 || (useMonDef && IsMonAffectedByShadowShieldBattle(data->monDef))))
		damage /= 2;

	if (damage == 0)
		damage = 1;
	else if (damage > 0x7FFFFFFF) //Prevent overflow
		damage = 0x7FFFFFFF;

	return damage;
}

static u16 GetBasePower(struct DamageCalc* data)
{
	int i;
	u16 power = data->basePower;

	//Take variables off struct for easier access
	u8 bankAtk = data->bankAtk;
	u8 bankDef = data->bankDef;
	u16 move = data->move;

	bool8 useMonAtk = data->monAtk != NULL;
	bool8 useMonDef = data->monDef != NULL;

	if (gNewBS->zMoveData.active) //Only active at runtime
	{
		if (gNewBS->ai.zMoveHelper == MOVE_NONE) //Would be set by moves like Mirror Move & Me First
			gNewBS->ai.zMoveHelper = gBattleMons[bankAtk].moves[gBattleStruct->chosenMovePositions[bankAtk]];

		return data->basePower = GetZMovePower(move);
	}
	else if (gNewBS->dynamaxData.active) //Only active at runtime
	{
		gNewBS->ai.zMoveHelper = gBattleMons[bankAtk].moves[gBattleStruct->chosenMovePositions[bankAtk]];
		return data->basePower = GetMaxMovePower(move);
	}
	else if (IsZMove(move)) //Only used in AI calcs
	{
		return data->basePower = GetZMovePower(move);
	}
	else if (IsAnyMaxMove(move)) //Only used in AI calcs
	{
		return data->basePower = GetMaxMovePower(move);
	}

	switch (move) {
		case MOVE_ACROBATICS:
			if (data->atkItem == ITEM_NONE)
				power *= 2;
			else if (data->specialFlags & (FLAG_CHECKING_FROM_MENU | FLAG_AI_CALC))
			{
				if (ITEM_EFFECT(bankAtk) == ITEM_EFFECT_GEM
				&& ITEM_QUALITY(bankAtk) == data->moveType)
					power *= 2; //The Gem will be used up before the attack
			}
			break;

		case MOVE_AVALANCHE:
		case MOVE_REVENGE:
			if (!(data->specialFlags & (FLAG_CHECKING_FROM_MENU | FLAG_AI_CALC))
			&& !useMonAtk
			&& ((gProtectStructs[bankAtk].physicalDmg && gProtectStructs[bankAtk].physicalBank == bankDef)
			 || (gProtectStructs[bankAtk].specialDmg && gProtectStructs[bankAtk].specialBank == bankDef)))
				power *= 2;
			break;

		case MOVE_FICKLEBEAM:
			if (Random() % 100 < 30)
				power *=2;
			break;

		case MOVE_BRINE:
			if (!(data->specialFlags & FLAG_IGNORE_TARGET)
			&& data->defHP < data->defMaxHP / 2)
				power *= 2;
			break;

		case MOVE_ECHOEDVOICE:
			if (data->specialFlags & (FLAG_CHECKING_FROM_MENU | FLAG_AI_CALC))
			{
				if (gNewBS->EchoedVoiceCounter != 0) //Will increase if used
					power = MathMin(200, power + (40 * (gNewBS->EchoedVoiceDamageScale + 1)));
			}
			else
				power = MathMin(200, power + (40 * gNewBS->EchoedVoiceDamageScale));
			break;

		case MOVE_FACADE:
			if (data->atkStatus1 & STATUS_ANY)
				power *= 2;
			break;

		case MOVE_FURYCUTTER:
			if (!useMonAtk)
			{
				for (i = 0; i < gDisableStructs[bankAtk].furyCutterCounter; ++i)
					power *= 2;

				if (power > 160)
					power = 160; //Max base power
			}
			break;

		case MOVE_FUSIONBOLT:
			if (data->specialFlags & FLAG_AI_CALC)
			{
				u8 partner = PARTNER(bankAtk);
				if (IS_DOUBLE_BATTLE
				&& BATTLER_ALIVE(partner)
				&& gChosenMovesByBanks[partner] == MOVE_FUSIONFLARE
				&& GetWhoStrikesFirst(partner, bankAtk, FALSE) == 0)
					power *= 2; //Fusion Flare will be used first
			}
			else if (!(data->specialFlags & (FLAG_CHECKING_FROM_MENU | FLAG_AI_CALC))
			&& !useMonAtk
			&& gNewBS->fusionFlareUsedPrior
			&& !IsFirstAttacker(bankAtk))
				power *= 2;
			break;

		case MOVE_FUSIONFLARE:
			if (data->specialFlags & FLAG_AI_CALC)
			{
				u8 partner = PARTNER(bankAtk);
				if (IS_DOUBLE_BATTLE
				&& BATTLER_ALIVE(partner)
				&& gChosenMovesByBanks[partner] == MOVE_FUSIONBOLT
				&& GetWhoStrikesFirst(partner, bankAtk, FALSE) == 0)
					power *= 2; //Fusion Bolt will be used first
			}
			else if (!(data->specialFlags & (FLAG_CHECKING_FROM_MENU | FLAG_AI_CALC))
			&& !useMonAtk
			&& gNewBS->fusionBoltUsedPrior
			&& !IsFirstAttacker(bankAtk))
				power *= 2;
			break;

		case MOVE_HEX:
			if (!(data->specialFlags & FLAG_IGNORE_TARGET)
			&& data->defStatus1 & STATUS_ANY)
				power *= 2;
			break;

		case MOVE_COLLISIONCOURSE:
		case MOVE_ELECTRODRIFT:
			if (!(data->specialFlags & FLAG_IGNORE_TARGET)
			&& data->resultFlags & MOVE_RESULT_SUPER_EFFECTIVE)
				power *= 15 / 10;
			break;

		case MOVE_PAYBACK:
			if (!(data->specialFlags & (FLAG_IGNORE_TARGET | FLAG_CHECKING_FROM_MENU))
			&& !useMonAtk
			&& !useMonDef
			&& BankMovedBefore(bankDef, bankAtk))
				power *= 2;
			break;

		case MOVE_RETALIATE:
			if (gNewBS->RetaliateCounters[SIDE(bankAtk)]) //Bank should be accurate for party too
				power *= 2;
			break;

		case MOVE_LASTRESPECTS:
		if (!(data->specialFlags & FLAG_IGNORE_TARGET))
		{
			int boost = 50;
			for(int i = 0; i < gPlayerPartyCount; i++)
				{
					struct Pokemon mon = gPlayerParty[i];
					if(mon.hp == 0)
					{
						boost++;
					}
				}
				power = boost;
		}
			break;

		case MOVE_PSYBLADE:
			if (gTerrainType == ELECTRIC_TERRAIN)
				power = (power * 15) / 10; //Boosts 1.5x in Electric Terrain
			break;

		case MOVE_TEMPERFLARE:
			if (gNewBS->activateTemperFlare) //Bank should be accurate for party too
				power *= 2;
			break;

		case MOVE_RAGEFIST:
			if (gNewBS->rageFistCounter[SIDE(bankAtk)])
				power = 50 * (gNewBS->rageFistCounter[SIDE(bankAtk)]);
			break;

		case MOVE_ROUND:
			if (!(data->specialFlags & (FLAG_CHECKING_FROM_MENU | FLAG_AI_CALC))
			&& !useMonAtk
			&& gNewBS->roundUsed)
				power *= 2;
			break;

		case MOVE_SMELLINGSALTS:
			if (!(data->specialFlags & FLAG_IGNORE_TARGET)
			&& data->defStatus1 & STATUS_PARALYSIS)
				power *= 2;
			break;

		case MOVE_STOMPINGTANTRUM:
			if (!useMonAtk && gNewBS->StompingTantrumTimers[bankAtk])
				power *= 2;
			break;

		case MOVE_VENOSHOCK:
		case MOVE_BARBBARRAGE:
			if (!(data->specialFlags & FLAG_IGNORE_TARGET)
			&& data->defStatus1 & STATUS_PSN_ANY)
				power *= 2;
			break;

		case MOVE_WAKEUPSLAP:
			if (!(data->specialFlags & FLAG_IGNORE_TARGET)
			&& data->defStatus1 & STATUS_SLEEP)
				power *= 2;
			break;

		case MOVE_INFERNALPARADE:
			if (!(data->specialFlags & FLAG_IGNORE_TARGET)
			&& data->defStatus1 & STATUS_BURN)
				power *= 2;
			break;

		case MOVE_BITTERMALICE:
			if (!(data->specialFlags & FLAG_IGNORE_TARGET)
			&& data->defStatus1 & STATUS_FREEZE)
				power *= 2;
			break;

		case MOVE_WEATHERBALL:
			if (gBattleWeather & WEATHER_ANY && WEATHER_HAS_EFFECT && !(gBattleWeather & WEATHER_AIR_CURRENT_PRIMAL))
				power *= 2;
			break;

		case MOVE_WHIRLPOOL:
			if (!(data->specialFlags & FLAG_IGNORE_TARGET)
			&& data->defStatus3 & STATUS3_UNDERWATER)
				power *= 2;
			break;

		case MOVE_GUST:
		case MOVE_TWISTER:
			if (!(data->specialFlags & FLAG_IGNORE_TARGET)
			&& data->defStatus3 & (STATUS3_IN_AIR | STATUS3_SKY_DROP_ATTACKER | STATUS3_SKY_DROP_TARGET))
				power *= 2;
			break;

		case MOVE_ROLLOUT:
		case MOVE_ICEBALL:
			if (!(data->specialFlags & (FLAG_CHECKING_FROM_MENU | FLAG_AI_CALC)) && !useMonAtk)
			{
				if (gBattleMons[bankAtk].status2 & STATUS2_DEFENSE_CURL)
					power *= 2;

				if (gBattleMons[bankAtk].status2 & STATUS2_MULTIPLETURNS //Rollout has started
				|| gNewBS->rolloutFinalHit) //It's the final hit of rollout so the status isn't set anymore
				{
					for (i = 1; i < (5 - gDisableStructs[bankAtk].rolloutTimer); ++i)
						power *= 2;
				}
			}
			break;

		case MOVE_GRASSPLEDGE:
		case MOVE_FIREPLEDGE:
		case MOVE_WATERPLEDGE:
			if (!(data->specialFlags & (FLAG_CHECKING_FROM_MENU | FLAG_AI_CALC)) && !useMonAtk && gNewBS->PledgeHelper)
				power = 150;
			break;

		case MOVE_ELECTROBALL:	;
			u32 ratio;
			if (!(data->specialFlags & FLAG_IGNORE_TARGET))
			{
				ratio = data->atkSpeed / data->defSpeed;

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
			if (!(data->specialFlags & FLAG_IGNORE_TARGET))
			{
				if (data->atkSpeed == 0)
					power = 1;
				else
					power = MathMin(150, (data->defSpeed * 25) / data->atkSpeed + 1);
			}
			break;

		case MOVE_PUNISHMENT:
			if (!(data->specialFlags & FLAG_IGNORE_TARGET) && !useMonDef)
				power = MathMin(200, 60 + 20 * CountBoosts(bankDef));
			break;

		case MOVE_LOWKICK:
		case MOVE_GRASSKNOT:
			if (!(data->specialFlags & FLAG_IGNORE_TARGET))
			{
				u32 weight = GetActualSpeciesWeight(data->defSpecies, data->defAbility, data->defItemEffect, bankDef, !useMonDef) / 10;

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
			if (!(data->specialFlags & FLAG_IGNORE_TARGET))
			{
				u32 atkWeight, defWeight, weightRatio;

				defWeight = GetActualSpeciesWeight(data->defSpecies, data->defAbility, data->defItemEffect, bankDef, !useMonDef);
				atkWeight = GetActualSpeciesWeight(data->atkSpecies, data->atkAbility, data->atkItemEffect, bankAtk, !useMonAtk);
				weightRatio = atkWeight / defWeight;

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
			if (!useMonAtk)
				power = 20 * (1 + CountBoosts(bankAtk));
			break;

		case MOVE_FLING: ;
			power = GetFlingPower(data->atkItem, data->atkSpecies, data->atkAbility, bankAtk, useMonAtk);
			break;

		case MOVE_ERUPTION:
		case MOVE_WATERSPOUT:
		case MOVE_DRAGONENERGY:
			power = MathMax(1, (150 * data->atkHP) / data->atkMaxHP);
			break;

		case MOVE_REVERSAL:
		case MOVE_FLAIL: ;
			u16 percentage = (48 * data->atkHP) / data->atkMaxHP;

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

		#ifdef SPECIES_ASHGRENINJA
		case MOVE_WATERSHURIKEN:
			if (data->atkSpecies == SPECIES_ASHGRENINJA && data->atkAbility == ABILITY_BATTLEBOND)
				power = 20;
			break;
		#endif

		case MOVE_CRUSHGRIP:
		case MOVE_WRINGOUT:
			if (!(data->specialFlags & FLAG_IGNORE_TARGET))
				power = MathMax(1, (data->defHP * 120) / data->defMaxHP);
			break;

		case MOVE_HARDPRESS:
			if (!(data->specialFlags & FLAG_IGNORE_TARGET))
				power = MathMax(1, (data->defHP * 100) / data->defMaxHP);
			break;

		case MOVE_TRUMPCARD: ;
			u8 index, pp;
			bool8 decrement = FALSE;

			//Load proper index of move
			if (useMonAtk)
			{
				index = FindMovePositionInMonMoveset(move, data->monAtk);
				pp = data->monAtk->pp[index];
				decrement = TRUE;
			}
			else if (data->specialFlags & (FLAG_CHECKING_FROM_MENU | FLAG_AI_CALC))
			{
				index = FindMovePositionInMoveset(move, bankAtk);
				pp = gBattleMons[bankAtk].pp[index];
				decrement = TRUE;
			}
			else
			{
				index = gBattleStruct->chosenMovePositions[bankAtk];
				pp = gBattleMons[bankAtk].pp[index];
			}

			if (decrement && pp > 0) //PP will be decremented at runtime so imitate this
			{
				pp -= 1; //One PP will be used on execution

				if (pp > 0 && data->defAbility == ABILITY_PRESSURE
				&& !(data->specialFlags & FLAG_IGNORE_TARGET))
					pp -= 1; //Decrement one more PP for Pressure usage
			}

			if (index < MAX_MON_MOVES)
			{
				switch (pp) {
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
			break;

		case MOVE_SPITUP:
			if (!useMonAtk)
				power = 100 * gDisableStructs[bankAtk].stockpileCounter; //Will be 0 if no stockpile
			break;

		case MOVE_NATURALGIFT:
			power = GetNaturalGiftMovePower(data->atkItem);
			break;

		case MOVE_PURSUIT:
			if (data->specialFlags & FLAG_AI_CALC)
			{
				if (!useMonAtk
				&& !useMonDef
				&& IsPredictedToSwitch(bankDef, bankAtk))
					power *= 2;
			}
			else if (!(data->specialFlags & FLAG_CHECKING_FROM_MENU))
				power *= gBattleScripting.dmgMultiplier;
			break;

		case MOVE_RETURN:
			if ((gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_TRAINER_TOWER | BATTLE_TYPE_FRONTIER | BATTLE_TYPE_EREADER_TRAINER))
			|| IsFrontierRaidBattle()
			#ifdef FLAG_SANDBOX_MODE
			|| FlagGet(FLAG_SANDBOX_MODE)
			#endif
			)
				power = (10 * 255) / 25;
			else if (useMonAtk)
				power = (10 * (data->monAtk->friendship)) / 25;
			else
				power = (10 * (gBattleMons[bankAtk].friendship)) / 25;
			break;

		case MOVE_FRUSTRATION:
			if ((gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_TRAINER_TOWER | BATTLE_TYPE_FRONTIER | BATTLE_TYPE_EREADER_TRAINER))
			|| IsFrontierRaidBattle()
			#ifdef FLAG_SANDBOX_MODE
			|| FlagGet(FLAG_SANDBOX_MODE)
			#endif
			)
				power = (10 * 255) / 25; //Always max damage
			else if (useMonAtk)
				power = (10 * (255 - data->monAtk->friendship)) / 25;
			else
				power = (10 * (255 - gBattleMons[bankAtk].friendship)) / 25;
			break;

		case MOVE_BEATUP:
			if (useMonAtk || (data->specialFlags & (FLAG_CHECKING_FROM_MENU | FLAG_AI_CALC)))
				power = (gBaseStats[data->atkSpecies].baseAttack / 10) + 5;
			else
			{
				struct Pokemon* party;
				if (SIDE(bankAtk) == B_SIDE_PLAYER)
					party = gPlayerParty;
				else
					party = gEnemyParty;

				power = (gBaseStats[party[gBattleCommunication[0] - 1].species].baseAttack / 10) + 5;
			}
			break;

		case MOVE_HIDDENPOWER:
		#ifdef OLD_HIDDEN_POWER_BP
			if (useMonAtk)
			{
				power = ((data->monAtk->hpIV & 2) >> 1) |
						((data->monAtk->attackIV & 2)) |
						((data->monAtk->defenseIV & 2) << 1) |
						((data->monAtk->speedIV & 2) << 2) |
						((data->monAtk->spAttackIV & 2) << 3) |
						((data->monAtk->spDefenseIV & 2) << 4);
			}
			else
			{
				power = ((gBattleMons[bankAtk].hpIV & 2) >> 1) |
						((gBattleMons[bankAtk].attackIV & 2)) |
						((gBattleMons[bankAtk].defenseIV & 2) << 1) |
						((gBattleMons[bankAtk].speedIV & 2) << 2) |
						((gBattleMons[bankAtk].spAttackIV & 2) << 3) |
						((gBattleMons[bankAtk].spDefenseIV & 2) << 4);
			}

			power = ((40 * power) / 63) + 30;
		#endif
			break;

		case MOVE_MAGNITUDE:
			if (!(data->specialFlags & (FLAG_CHECKING_FROM_MENU | FLAG_AI_CALC)) && !useMonAtk)
				power = gDynamicBasePower;
			else if (data->specialFlags & FLAG_AI_CALC)
				power = 71; //Average Power
			break;

		case MOVE_PRESENT:
			if (!(data->specialFlags & (FLAG_CHECKING_FROM_MENU | FLAG_AI_CALC)) && !useMonAtk)
				power = gDynamicBasePower;
			else if (data->specialFlags & FLAG_AI_CALC)
				power = 40; //Average Power
			break;

		case MOVE_BOLTBEAK:
		case MOVE_FISHIOUSREND:
			if (!(data->specialFlags & (FLAG_IGNORE_TARGET | FLAG_CHECKING_FROM_MENU))
			&& BankMovedBeforeIgnoreSwitch(bankAtk, bankDef))
				power *= 2;
			break;

		case MOVE_GRAVAPPLE:
			if (IsGravityActive())
				power = (power * 15) / 10; //1.5x boost in Gravity
			break;

		case MOVE_TERRAINPULSE:
			if (gTerrainType && data->atkIsGrounded)
				power *= 2;
			break;

		case MOVE_RISINGVOLTAGE:
			if (!(data->specialFlags & FLAG_IGNORE_TARGET)
			&& gTerrainType == ELECTRIC_TERRAIN && (data->defIsGrounded || IsFloatingWithMagnetism(bankDef)))
				power *= 2;
			break;

		case MOVE_EXPANDINGFORCE:
			if (gTerrainType == PSYCHIC_TERRAIN && data->atkIsGrounded)
				power = (power * 15) / 10;
			break;

		case MOVE_MISTYEXPLOSION:
			if (gTerrainType == MISTY_TERRAIN && data->atkIsGrounded)
				power = (power * 15) / 10;
			break;

		case MOVE_LASHOUT:
			if (gNewBS->statFellThisRound[bankAtk])
				power *= 2;
			break;

		case MOVE_ROAROFTIME:
			#ifdef SPECIES_DIALGA_ORIGIN
			if (data->atkSpecies == SPECIES_DIALGA_ORIGIN)
				power = 170;
			#endif
			break;

		case MOVE_SPACIALREND:
			#ifdef SPECIES_PALKIA_ORIGIN
			if (data->atkSpecies == SPECIES_PALKIA_ORIGIN)
				power = 90;
			#endif
			break;

		default:
			if (gBattleMoves[move].effect == EFFECT_TRIPLE_KICK)
			{
				if (data->specialFlags & FLAG_AI_CALC) //Pretend as if it'll hit three times
				{
					//Generalized base power considering accuracy and missing
					if (move == MOVE_TRIPLEAXEL)
						power = 100;
					else
						power = 50;
				}
				else if (!(data->specialFlags & (FLAG_CHECKING_FROM_MENU)) && !useMonAtk)
					power = gBattleScripting.tripleKickPower;
			}
			break;
	}

	gBattleMovePower = power;
	return power;
}

static u16 AdjustBasePower(struct DamageCalc* data, u16 power)
{
	//Take variables off struct for easier access
	u8 bankAtk = data->bankAtk;
	u8 bankDef = data->bankDef;
	u16 move = data->move;

	bool8 useMonAtk = data->monAtk != NULL;
	bool8 useMonDef = data->monDef != NULL;

	if (data->specialFlags & (FLAG_CONFUSION_DAMAGE))
		return power;

	//Check attacker ability boost
	switch(data->atkAbility) {
		case ABILITY_TECHNICIAN:
		//1.5x Boost
			if (data->basePower <= 60)
				power = (power * 15) / 10;
			break;

		case ABILITY_RIVALRY:
		//1.25x / 0.75x Boost
			if (!(data->specialFlags & FLAG_IGNORE_TARGET))
			{
				u8 attackerGender, targetGender;
				if (useMonAtk)
					attackerGender = GetGenderFromSpeciesAndPersonality(data->atkSpecies, data->monAtk->personality);
				else
					attackerGender = GetGenderFromSpeciesAndPersonality(data->atkSpecies, gBattleMons[bankAtk].personality);

				if (useMonDef)
					targetGender = GetGenderFromSpeciesAndPersonality(data->defSpecies, data->monDef->personality);
				else
					targetGender = GetGenderFromSpeciesAndPersonality(data->defSpecies, gBattleMons[bankDef].personality);

				if (attackerGender != 0xFF && targetGender != 0xFF)
				{
					if (attackerGender == targetGender)
						power = (power * 125) / 100;
					else
						power = (power * 75) / 100;
				}
			}
			break;

		case ABILITY_RECKLESS:
		//1.2x Boost
			if (gBattleMoves[move].effect == EFFECT_RECOIL_IF_MISS
			|| gSpecialMoveFlags[move].gPercent25RecoilMoves
			|| gSpecialMoveFlags[move].gPercent33RecoilMoves
			|| gSpecialMoveFlags[move].gPercent50RecoilMoves)
				power = (power * 12) / 10;
			break;

		case ABILITY_IRONFIST:
		//1.2x Boost
			if (gSpecialMoveFlags[move].gPunchingMoves)
				power = (power * 12) / 10;
			break;

		case ABILITY_TOXICBOOST:
		//1.5x Boost
			if (data->atkStatus1 & STATUS_PSN_ANY && data->moveSplit == SPLIT_PHYSICAL)
				power = (power * 15) / 10;
			break;

		case ABILITY_FLAREBOOST:
		//1.5x Boost
			if (data->atkStatus1 & STATUS_BURN && data->moveSplit == SPLIT_SPECIAL)
				power = (power * 15) / 10;
			break;

		case ABILITY_SANDFORCE:
		//1.3x Boost
			if (gBattleWeather & WEATHER_SANDSTORM_ANY
			&& (data->moveType == TYPE_ROCK || data->moveType == TYPE_GROUND || data->moveType == TYPE_STEEL))
				power = (power * 13) / 10;
			break;

		case ABILITY_SHEERFORCE:
		//1.3x Boost
			if (gSpecialMoveFlags[move].gSheerForceBoostedMoves)
				power = (power * 13) / 10;
			break;

		case ABILITY_AERILATE:
		case ABILITY_PIXILATE:
		case ABILITY_REFRIGERATE:
		case ABILITY_GALVANIZE:
		case ABILITY_NORMALIZE:
		//1.2x / 1.3x Boost
			if ((!useMonAtk && AbilityCanChangeTypeAndBoost(move, data->atkAbility, gNewBS->ElectrifyTimers[bankAtk], (gNewBS->zMoveData.active || gNewBS->zMoveData.viewing)))
			||   (useMonAtk && AbilityCanChangeTypeAndBoost(move, data->atkAbility, 0, FALSE)))
			{
				#ifdef OLD_ATE_BOOST
					power = (power * 13) / 10;
				#else
					power = (power * 12) / 10;
				#endif
			}
			break;

		case ABILITY_MEGALAUNCHER:
		//1.5x Boost
			if (gSpecialMoveFlags[move].gPulseAuraMoves)
				power = (power * 15) / 10;
			break;

		case ABILITY_STRONGJAW:
		//1.5x Boost
			if (gSpecialMoveFlags[move].gBitingMoves)
				power = (power * 15) / 10;
			break;

		case ABILITY_TOUGHCLAWS:
		//1.3x Boost
			if (((!useMonAtk && IsContactMove(move, bankAtk, bankDef))
			   || (useMonAtk && gBattleMoves[move].flags & FLAG_MAKES_CONTACT)) //Party mons can't use any fancy calculations for contact moves
			&& !CanNeverMakeContactByItemEffect(data->atkItemEffect)) //Don't check Ability since it's known to be Tough Claws
				power = (power * 13) / 10;
			break;

		case ABILITY_NEUROFORCE:
		//1.25x Boost
			if (data->resultFlags & MOVE_RESULT_SUPER_EFFECTIVE)
				power = (power * 125) / 100;
			break;

		case ABILITY_STAKEOUT:
		//2x Boost
			if (!useMonAtk && gNewBS->StakeoutCounters[bankDef])
				power *= 2;
			break;

		case ABILITY_STEELWORKER:
		case ABILITY_STEELYSPIRIT:
		//1.5x Boost
			if (data->moveType == TYPE_STEEL)
				power = (power * 15) / 10;
			break;

		case ABILITY_WATERBUBBLE:
		//2x Boost
			if (data->moveType == TYPE_WATER)
				power *= 2; //2x Boost
			break;

		case ABILITY_ANALYTIC:
		//1.3x Boost
			if (BankMovedBefore(bankDef, bankAtk))
				power = (power * 13) / 10;
			break;

		case ABILITY_PUNKROCK:
		//1.3x Boost
			if (CheckSoundMove(move))
				power = (power * 13) / 10;
			break;

		#ifdef ABILITY_TRANSISTOR
		case ABILITY_TRANSISTOR:
		//1.5x Boost
			if (data->moveType == TYPE_ELECTRIC)
				power = (power * 15) / 10;
			break;
		#endif

		#ifdef ABILITY_DRAGONSMAW
		case ABILITY_DRAGONSMAW:
		//1.5x Boost
			if (data->moveType == TYPE_DRAGON)
				power = (power * 15) / 10;
			break;
		#endif
	}

	//Check attacker partner ability boost
	switch (data->atkPartnerAbility) {
		case ABILITY_BATTERY:
		//1.3x Boost
			if (data->moveSplit == SPLIT_SPECIAL)
				power = (power * 13) / 10;
			break;

		case ABILITY_STEELYSPIRIT:
		//1.5x Boost
			if (data->moveType == TYPE_STEEL)
				power = (power * 15) / 10;
			break;

		case ABILITY_POWERSPOT:
		//1.3x Boost
			power = (power * 13) / 10;
			break;
	}

	//Check attacker item effect boost
	switch (data->atkItemEffect) {
		case ITEM_EFFECT_MUSCLE_BAND:
			if (data->moveSplit == SPLIT_PHYSICAL)
				power = (power * 11) / 10;
			break;

		case ITEM_EFFECT_WISE_GLASSES:
			if (data->moveSplit == SPLIT_SPECIAL)
				power = (power * 11) / 10;
			break;

		case ITEM_EFFECT_PLATE:
		case ITEM_EFFECT_TYPE_BOOSTERS:
		//1.2x Boost
			if (data->moveType == data->atkItemQuality)
				power = (power * 12) / 10;
			break;

		#ifdef NATIONAL_DEX_DIALGA
		case ITEM_EFFECT_ADAMANT_ORB:
		//1.2x Boost
			if (SpeciesToNationalPokedexNum(data->atkSpecies) == NATIONAL_DEX_DIALGA
			&& (data->moveType == TYPE_STEEL || data->moveType == TYPE_DRAGON))
				power = (power * 12) / 10;
			break;
		#endif

		#ifdef NATIONAL_DEX_PALKIA
		case ITEM_EFFECT_LUSTROUS_ORB:
		//1.2x Boost
			if (SpeciesToNationalPokedexNum(data->atkSpecies) == NATIONAL_DEX_PALKIA
			&& (data->moveType == TYPE_WATER || data->moveType == TYPE_DRAGON))
				power = (power * 12) / 10;
			break;
		#endif

		#ifdef NATIONAL_DEX_GIRATINA
		case ITEM_EFFECT_GRISEOUS_ORB:
		//1.2x Boost
			if (SpeciesToNationalPokedexNum(data->atkSpecies) == NATIONAL_DEX_GIRATINA
			&& (data->moveType == TYPE_GHOST || data->moveType == TYPE_DRAGON))
				power = (power * 12) / 10;
			break;
		#endif

		#if (!(defined OLD_SOUL_DEW_EFFECT) && defined NATIONAL_DEX_LATIOS && defined NATIONAL_DEX_LATIAS)
		case ITEM_EFFECT_SOUL_DEW:
		//1.2x Boost
			if ((SpeciesToNationalPokedexNum(data->atkSpecies) == NATIONAL_DEX_LATIOS
			  || SpeciesToNationalPokedexNum(data->atkSpecies) == NATIONAL_DEX_LATIAS)
			&& (data->moveType == TYPE_PSYCHIC
			 || data->moveType == TYPE_DRAGON))
				power = (power * 12) / 10;
			break;
		#endif

		case ITEM_EFFECT_GEM: //This check is specifically meant for the AI, as the Gem would usually be consumed by now
							  //If the gNewBS->consumedGem is active and the user still has a gem, that means it received another through Symbiosis which is ignored
		//1.3x / 1.5x Boost
			if (data->moveType == data->atkItemQuality && !gNewBS->consumedGem)
			{
				#ifdef OLD_GEM_BOOST
					power = (power * 15) / 10;
				#else
					power = (power * 13) / 10;
				#endif
			}
			break;

		case ITEM_EFFECT_METRONOME:
			//1.2x - 2.0x Boost
			if (!useMonAtk)
			{
				u16 boost;

				if (data->specialFlags & FLAG_CHECKING_FROM_MENU)
				{
					if (move == gLastPrintedMoves[bankAtk]) //Viewing the same move as used last turn
						boost = MathMin(200, 100 + gNewBS->MetronomeCounter[bankAtk] + 20);
					else
						break;
				}
				else
					boost = MathMin(200, 100 + gNewBS->MetronomeCounter[bankAtk]);

				power = (power * boost) / 100;
			}
			break;

		case ITEM_EFFECT_LIFE_ORB:
			//1.3x Boost
			power = (power * 13) / 10;
			break;
	}

	//Gem activated at runtime
	if (gNewBS->consumedGem)
	{
		//1.3x / 1.5x Boost
		#ifdef OLD_GEM_BOOST
			power = (power * 15) / 10;
		#else
			power = (power * 13) / 10;
		#endif
	}

	//Charge check - 2x Boost
	if (data->atkStatus3 & STATUS3_CHARGED_UP && data->moveType == TYPE_ELECTRIC)
		power *= 2;

	#ifdef OLD_TERRAIN_BOOST
		#define TERRAIN_BOOST 15 //1.5x in Gen 6 & 7
	#else
		#define TERRAIN_BOOST 13 //1.3x in Gen 8
	#endif

	//Terrain Checks
	if (!useMonAtk || !(data->specialFlags & FLAG_FUTURE_SIGHT_DAMAGE)) //Future Sight mon in back of party isn't affected by Terrain
	{
		switch (gTerrainType) {
			case ELECTRIC_TERRAIN:
			//1.5x Boost
				if ((data->atkIsGrounded || IsFloatingWithMagnetism(bankAtk)) && data->moveType == TYPE_ELECTRIC)
					power = (power * TERRAIN_BOOST) / 10;
				break;

			case GRASSY_TERRAIN:
			//1.5x / 0.5 Boost
				if (data->atkIsGrounded && data->moveType == TYPE_GRASS)
					power = (power * TERRAIN_BOOST) / 10;

				if ((move == MOVE_MAGNITUDE || move == MOVE_EARTHQUAKE || move == MOVE_BULLDOZE)
				&& !(data->defStatus3 & STATUS3_SEMI_INVULNERABLE))
					power /= 2;
				break;

			case MISTY_TERRAIN:
			//0.5x Boost
				if (data->defIsGrounded && data->moveType == TYPE_DRAGON)
					power /= 2;
				break;

			case PSYCHIC_TERRAIN:
			//1.5x Boost
				if (data->atkIsGrounded && data->moveType == TYPE_PSYCHIC)
					power = (power * TERRAIN_BOOST) / 10;
				break;
		}
	}

	//Sport Checks
	switch (data->moveType) {
		case TYPE_FIRE:
			if (IsWaterSportActive())
				power /= 3;
			break;
		case TYPE_ELECTRIC:
			if (IsMudSportActive())
				power /= 3;
			break;
	}

	return power;
}

//Requires that the base move be loaded into gNewBS->ai.zMoveHelper
static u16 GetZMovePower(u16 zMove)
{
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
			//gNewBS->ai.zMoveHelper stores the original move to helper turn it into a Z-Move.
			power = gBattleMoves[gNewBS->ai.zMoveHelper].z_move_power;
	}

	return power;
}

//Requires that the base move be loaded into gNewBS->ai.zMoveHelper
static u16 GetMaxMovePower(u16 maxMove)
{
	switch (maxMove)
	{
		case MOVE_G_MAX_DRUM_SOLO_P:
		case MOVE_G_MAX_DRUM_SOLO_S:
		case MOVE_G_MAX_FIREBALL_P:
		case MOVE_G_MAX_FIREBALL_S:
		case MOVE_G_MAX_HYDROSNIPE_P:
		case MOVE_G_MAX_HYDROSNIPE_S:
			return 160; //Always the same regardless of base move
	}

	#ifdef DYNAMAX_FEATURE
	return gDynamaxMovePowers[gNewBS->ai.zMoveHelper];
	#else
	return 0;
	#endif
}

u16 CalcVisualBasePower(u8 bankAtk, u8 bankDef, u16 move, bool8 ignoreDef)
{
	struct DamageCalc data = {0};

	data.bankAtk = bankAtk;
	data.bankDef = bankDef;
	data.move = move;
	data.specialFlags = FLAG_CHECKING_FROM_MENU;
	u16 power = 0;

//Load attacker Data
	PopulateDamageCalcStructWithBaseAttackerData(&data);
	data.moveSplit = CalcMoveSplit(move, bankAtk, bankDef);
	data.moveType = GetMoveTypeSpecial(bankAtk, move);

//Load target data
	if (ignoreDef)
		data.specialFlags |= FLAG_IGNORE_TARGET;
	else
		PopulateDamageCalcStructWithBaseDefenderData(&data);

	data.resultFlags = VisualTypeCalc(move, bankAtk, bankDef);

//Load correct move power
	data.basePower = gBattleMoves[move].power;
	power = GetBasePower(&data);
	power = AdjustBasePower(&data, power);

	//Not really a base power thing, but stick them in anayways
	switch (move) {
		case MOVE_SOLARBEAM:
		case MOVE_SOLARBLADE:
			if (gBattleWeather & (WEATHER_RAIN_ANY | WEATHER_SANDSTORM_ANY | WEATHER_HAIL_ANY | WEATHER_FOG_ANY | WEATHER_AIR_CURRENT_PRIMAL)
			&& WEATHER_HAS_EFFECT
			&& !ItemEffectIgnoresSunAndRain(data.atkItemEffect))
				power /= 2; //Any weather except sun weakens Solar Beam
			break;
		case MOVE_DYNAMAXCANNON:
		case MOVE_BEHEMOTHBLADE:
		case MOVE_BEHEMOTHBASH:
			if (IsDynamaxed(bankDef))
				power *= 2;
			break;
	}

	return power;
}

u8 GetNaturalGiftMoveType(u16 item)
{
	u32 i;
	u8 moveType = TYPE_NORMAL; //If the berry isn't in the table, it's just shown as a normal type move (can't be Mystery because of Max Strike)

	if (IsBerry(item))
	{
		for (i = 0; gNaturalGiftTable[i].berry != ITEM_TABLES_TERMIN; ++i)
		{
			if (gNaturalGiftTable[i].berry == item)
			{
				moveType = gNaturalGiftTable[i].type;
				break;
			}
		}
	}

	return moveType;
}

u8 GetNaturalGiftMovePower(u16 item)
{
	u32 i;
	u8 power = 0; //Would be at 1 before

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

	return power;
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

static u32 ScreensWeakenDamage(u32 damage, bool8 screensUp, u8 atkAbility, u8 bankDef)
{
	if ((screensUp || gNewBS->AuroraVeilTimers[SIDE(bankDef)])
	&& gCritMultiplier <= BASE_CRIT_MULTIPLIER && !BypassesScreens(atkAbility))
	{
		if (IS_DOUBLE_BATTLE && CountAliveMonsInBattle(BATTLE_ALIVE_DEF_SIDE, bankDef, bankDef) >= 2)
			damage = (damage * 2) / 3;
		else
			damage /= 2;
	}

	return damage;
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

void CalculateShellSideArmSplits(void)
{
	u8 bankAtk, bankDef;
	u8 power = gBattleMoves[MOVE_SHELLSIDEARM].power;
	u8 defMod = !IsWonderRoomActive() ? STAT_STAGE_DEF : STAT_STAGE_SPDEF; //Wonder Room in official games erronously only swaps the stat stages for the calculation
	u8 spDefMod = !IsWonderRoomActive() ? STAT_STAGE_SPDEF : STAT_STAGE_DEF;

	for (bankAtk = 0; bankAtk < gBattlersCount; ++bankAtk)
	{
		if (!BATTLER_ALIVE(bankAtk))
			continue;

		u8 atkAbility = ABILITY(bankAtk);
		u8 level = gBattleMons[bankAtk].level;
		u32 attack = gBattleMons[bankAtk].attack;
		u32 spAttack = gBattleMons[bankAtk].spAttack;
		APPLY_QUICK_STAT_MOD(attack, STAT_STAGE(bankAtk, STAT_STAGE_ATK));
		APPLY_QUICK_STAT_MOD(spAttack, STAT_STAGE(bankAtk, STAT_STAGE_SPATK));

		for (bankDef = 0; bankDef < gBattlersCount; ++bankDef)
		{
			if (bankAtk == bankDef || !BATTLER_ALIVE(bankDef))
				continue;

			//Factors in raw stats, stat stages, and screens
			u8 split;
			u32 defense = gBattleMons[bankDef].defense;
			u32 spDefense = gBattleMons[bankDef].spDefense;
			APPLY_QUICK_STAT_MOD(defense, STAT_STAGE(bankDef, defMod));
			APPLY_QUICK_STAT_MOD(spDefense, STAT_STAGE(bankDef, spDefMod));

			u32 physicalDamage = (((((2 * level) / 5 + 2) * power * attack) / defense) / 50);
			u32 specialDamage = (((((2 * level) / 5 + 2) * power * spAttack) / spDefense) / 50);
			physicalDamage = ScreensWeakenDamage(physicalDamage, (gSideStatuses[bankDef] & SIDE_STATUS_REFLECT) != 0, atkAbility, bankDef);
			specialDamage = ScreensWeakenDamage(specialDamage, (gSideStatuses[bankDef] & SIDE_STATUS_LIGHTSCREEN) != 0, atkAbility, bankDef);

			if (physicalDamage > specialDamage)
				split = SPLIT_PHYSICAL;
			else if (specialDamage > physicalDamage)
				split = SPLIT_SPECIAL;
			else
				split = (Random() & 1) ? SPLIT_PHYSICAL : SPLIT_SPECIAL;

			gNewBS->shellSideArmSplit[bankAtk][bankDef] = split; //Calculate for all because any Pokemon could call Shell Side Arm from another move (eg. Metronome)
		}
	}
}

static void TryBoostMonOffensesForTotemBoost(struct DamageCalc* data, u8 bankAtk, bool8 bodyPress)
{
	switch (CanActivateTotemBoost(bankAtk)) //Bank is used mainly for position on field
	{
		case TOTEM_MULTI_BOOST: //Boost 2 Stats
			BoostMonOffensesForTotemBoost(data, bankAtk, TRUE, bodyPress);
			//Fallthrough
		case TOTEM_SINGLE_BOOST:
			BoostMonOffensesForTotemBoost(data, bankAtk, FALSE, bodyPress);
			break;
		case TOTEM_OMNIBOOST:
			data->atkBuff += 1;
			data->spAtkBuff += 1;
			break;
	}
}

static void TryBoostMonDefensesForTotemBoost(struct DamageCalc* data, u8 bankDef)
{
	switch (CanActivateTotemBoost(bankDef))
	{
		case TOTEM_MULTI_BOOST: //Boost 2 Stats
			BoostMonDefensesForTotemBoost(data, bankDef, TRUE);
			//Fallthrough
		case TOTEM_SINGLE_BOOST:
			BoostMonDefensesForTotemBoost(data, bankDef, FALSE);
			break;
		case TOTEM_OMNIBOOST:
			data->defBuff += 1;
			data->spDefBuff += 1;
			break;
	}
}

static void BoostMonOffensesForTotemBoost(struct DamageCalc* data, u8 bankAtk, bool8 multiBoost, bool8 bodyPress)
{
	u8 checkStat1, checkStat2;
	u8 totemStat, raiseAmount;
	totemStat = GetTotemStat(bankAtk, multiBoost);
	raiseAmount = GetTotemRaiseAmount(bankAtk, multiBoost);

	if (!bodyPress)
	{
		checkStat1 = STAT_STAGE_ATK;
		checkStat2 = STAT_STAGE_SPATK;
	}
	else //Body Press
	{
		checkStat1 = STAT_STAGE_DEF;
		checkStat2 = STAT_STAGE_SPDEF;
	}

	if (totemStat == checkStat1)
		data->atkBuff = min(data->atkBuff + TotemRaiseAmountToStatMod(raiseAmount), STAT_STAGE_MAX);
	else if (totemStat == checkStat2)
		data->spAtkBuff = min(data->spAtkBuff + TotemRaiseAmountToStatMod(raiseAmount), STAT_STAGE_MAX);
}

static void BoostMonDefensesForTotemBoost(struct DamageCalc* data, u8 bankDef, bool8 multiBoost)
{
	u8 totemStat, raiseAmount;
	totemStat = GetTotemStat(bankDef, multiBoost);
	raiseAmount = GetTotemRaiseAmount(bankDef, multiBoost);

	if (totemStat == STAT_STAGE_DEF)
		data->defBuff = min(data->defBuff + TotemRaiseAmountToStatMod(raiseAmount), STAT_STAGE_MAX);
	else if (totemStat == STAT_STAGE_SPDEF)
		data->spDefBuff = min(data->spDefBuff + TotemRaiseAmountToStatMod(raiseAmount), STAT_STAGE_MAX);
}
