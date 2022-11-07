#include "defines.h"
#include "defines_battle.h"
#include "../include/random.h"
#include "../include/string_util.h"
#include "../include/constants/items.h"
#include "../include/constants/pokedex.h"

#include "../include/new/ai_master.h"
#include "../include/new/ability_tables.h"
#include "../include/new/battle_start_turn_start.h"
#include "../include/new/battle_util.h"
#include "../include/new/build_pokemon.h"
#include "../include/new/damage_calc.h"
#include "../include/new/dynamax.h"
#include "../include/new/frontier.h"
#include "../include/new/item.h"
#include "../include/new/mega.h"
#include "../include/new/move_tables.h"
#include "../include/new/multi.h"
#include "../include/new/util.h"

/*
battle_util.c
	general functions for aiding in battle logic for everything
*/

#define IS_BATTLE_CIRCUS (gBattleTypeFlags & BATTLE_TYPE_BATTLE_CIRCUS)

static void TryRemoveUnburdenBoost(u8 bank);
static bool8 CanBeGeneralStatused(u8 bankDef, u8 defAbility, u8 atkAbility, bool8 checkFlowerVeil);

u8 GetBankForBattleScript(u8 caseId)
{
	u8 ret = 0;

	switch (caseId) {
		case BS_GET_TARGET:
			ret = gBankTarget;
			break;
		case BS_GET_ATTACKER:
			ret = gBankAttacker;
			break;
		case BS_GET_EFFECT_BANK:
			ret = gEffectBank;
			break;
		case BS_GET_BANK_0:
			ret = 0;
			break;
		case BS_GET_SCRIPTING_BANK:
			ret = gBattleScripting.bank;
			break;
		case BS_GET_FAINTED_BANK:
			ret = gBankFainted;
			break;
		case 5:
			ret = gBankFainted;
			break;
		case BS_GET_PLAYER1:
			ret = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
			break;
		case BS_GET_OPPONENT1:
			ret = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
			break;
		case BS_GET_PLAYER2:
			ret = GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT);
			break;
		case BS_GET_OPPONENT2:
			ret = GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT);
			break;
		case BS_GET_SWITCHING_BANK:
			ret = gBankSwitching;
			break;
		case 4:
		case 6:
		case 8:
		case 9:
			break;
	}

	return ret;
}

u8 GetFirstAliveActiveBattler(void)
{
	u32 i;

	for (i = 0; i < gBattlersCount; ++i)
	{
		if (BATTLER_ALIVE(i))
			return i;
	}

	return 1; //If no choice, use opponent because if SIDE is called on it, the position returned will be 0xFF, and that defaults to opponent side
}

ability_t GetBankAbility(u8 bank) //Not actually used anymore
{
	if (IsAbilitySuppressed(bank))
		return ABILITY_NONE;

	return gBattleMons[bank].ability;
}

ability_t GetRecordedAbility(u8 bank)
{
	if (IsAbilitySuppressed(bank))
		return ABILITY_NONE;

	if (BATTLE_HISTORY->abilities[bank] != ABILITY_NONE)
		return BATTLE_HISTORY->abilities[bank];

	u16 species = SPECIES(bank);
	u8 ability1 = GetAbility1(species);
	u8 ability2 = GetAbility2(species);
	u8 hiddenAbility = GetHiddenAbility(species);

	if (ability1 == ability2 && hiddenAbility == ABILITY_NONE)
		return ability1;

	if (ability1 == ability2 && ability1 == hiddenAbility)
		return ability1;

	if (ability2 == ABILITY_NONE && hiddenAbility == ABILITY_NONE)
		return ability1;

	return ABILITY_NONE; //We don't know which ability the target has
}

ability_t CopyAbility(u8 bank)
{
	if (IsAbilitySuppressed(bank))
		return gNewBS->SuppressedAbilities[bank];
	else if (gNewBS->neutralizingGasBlockedAbilities[bank])
		return gNewBS->neutralizingGasBlockedAbilities[bank];
	else if (gNewBS->DisabledMoldBreakerAbilities[bank])
		return gNewBS->DisabledMoldBreakerAbilities[bank];
	else
		return gBattleMons[bank].ability;
}

ability_t* GetAbilityLocation(u8 bank)
{
	if (IsAbilitySuppressed(bank))
		return &gNewBS->SuppressedAbilities[bank];
	else if (gNewBS->neutralizingGasBlockedAbilities[bank])
		return &gNewBS->neutralizingGasBlockedAbilities[bank];
	else if (gNewBS->DisabledMoldBreakerAbilities[bank])
		return &gNewBS->DisabledMoldBreakerAbilities[bank];
	else
		return &gBattleMons[bank].ability;
}

ability_t* GetAbilityLocationIgnoreNeutralizingGas(u8 bank)
{
	if (IsAbilitySuppressed(bank))
		return &gNewBS->SuppressedAbilities[bank];
	else if (gNewBS->DisabledMoldBreakerAbilities[bank])
		return &gNewBS->DisabledMoldBreakerAbilities[bank];
	else
		return &gBattleMons[bank].ability;
}

void RecordAbilityBattle(u8 bank, u8 ability)
{
	BATTLE_HISTORY->abilities[bank] = ability;
}

void ClearBattlerAbilityHistory(u8 bank)
{
	BATTLE_HISTORY->abilities[bank] = ABILITY_NONE;
}

item_effect_t GetBankItemEffect(u8 bank)
{
	if (ABILITY(bank) != ABILITY_KLUTZ && !gNewBS->EmbargoTimers[bank] && !IsMagicRoomActive())
		return ItemId_GetHoldEffect(ITEM(bank));

	return 0;
}

item_effect_t GetMonItemEffect(const struct Pokemon* mon)
{
	if (GetMonAbility(mon) != ABILITY_KLUTZ && !IsMagicRoomActive())
		return ItemId_GetHoldEffect(GetMonData(mon, MON_DATA_HELD_ITEM, NULL));

	return 0;
}

item_effect_t GetRecordedItemEffect(u8 bank)
{
	if (ITEM_EFFECT(bank) == gNewBS->ai.itemEffects[bank]) //Allows factoring in Klutz and Magic Room
		return gNewBS->ai.itemEffects[bank];

	return 0;
}

void RecordItemEffectBattle(u8 bank, u8 itemEffect)
{
	gNewBS->ai.itemEffects[bank] = itemEffect;
}

void ClearBattlerItemEffectHistory(u8 bank)
{
	gNewBS->ai.itemEffects[bank] = 0;
}

void RecordLastUsedMoveByAttacker(u16 move)
{
	u32 i;

	for (i = 0; i < MAX_MON_MOVES; i++)
	{
		if (BATTLE_HISTORY->usedMoves[gBankAttacker][i] == move) //Move already recorded
			break; //Don't record again

		if (BATTLE_HISTORY->usedMoves[gBankAttacker][i] == MOVE_NONE) //Move hasn't been used yet
		{
			BATTLE_HISTORY->usedMoves[gBankAttacker][i] = move; //Record move
			return;
		}
	}
}

void ClearBattlerMoveHistory(u8 bank)
{
	u32 i;

	for (i = 0; i < MAX_MON_MOVES; i++)
		BATTLE_HISTORY->usedMoves[bank][i] = MOVE_NONE;
}

struct Pokemon* GetBankPartyData(u8 bank)
{
	u8 index = gBattlerPartyIndexes[bank];
	return (SIDE(bank) == B_SIDE_OPPONENT) ? &gEnemyParty[index] : &gPlayerParty[index];
}

u16 GetBaseCurrentHP(u8 bank)
{
	if (IsDynamaxed(bank))
	{
		//Ceiling
		return MathMax(1, gBattleMons[bank].hp / GetDynamaxHPBoost(bank) + (gBattleMons[bank].hp & 1));
	}

	return gBattleMons[bank].hp;
}

u16 GetBaseMaxHP(u8 bank)
{
	if (IsDynamaxed(bank))
	{
		//Ceiling
		return MathMax(1, gBattleMons[bank].maxHP / GetDynamaxHPBoost(bank) + (gBattleMons[bank].maxHP & 1));
	}

	return gBattleMons[bank].maxHP;
}

u8 GetBankFromPartyData(struct Pokemon* mon)
{
	for (int i = 0; i < gBattlersCount; ++i)
	{
		if (SIDE(i) == B_SIDE_OPPONENT)
		{
			if ((u32) (&gEnemyParty[gBattlerPartyIndexes[i]]) == (u32) mon)
				return i;
		}

		if (SIDE(i) == B_SIDE_PLAYER)
		{
			if ((u32) (&gPlayerParty[gBattlerPartyIndexes[i]]) == (u32) mon)
				return i;
		}
	}

	return PARTY_SIZE;
}

bool8 CanHitSemiInvulnerableTarget(u8 bankAtk, u8 bankDef, u16 move)
{
	if (ABILITY(bankAtk) == ABILITY_NOGUARD || ABILITY(bankDef) == ABILITY_NOGUARD)
		return TRUE;

	if (move == MOVE_TOXIC && IsOfType(bankAtk, TYPE_POISON))
		return TRUE;

	return gStatuses3[bankDef] & STATUS3_ALWAYS_HITS && gDisableStructs[bankDef].bankWithSureHit == bankAtk;
}

bool8 CheckGrounding(u8 bank)
{
	if (BATTLER_SEMI_INVULNERABLE(bank)) //Apparently a thing
		return IN_AIR;

	if (IsGravityActive()
	|| GetBankItemEffect(bank) == ITEM_EFFECT_IRON_BALL
	|| (gStatuses3[bank] & (STATUS3_SMACKED_DOWN | STATUS3_ROOTED)))
		return GROUNDED;

	else if ((gStatuses3[bank] & (STATUS3_LEVITATING | STATUS3_TELEKINESIS | STATUS3_IN_AIR))
	|| ITEM_EFFECT(bank) == ITEM_EFFECT_AIR_BALLOON
	|| ABILITY(bank) == ABILITY_LEVITATE
	|| IsFloatingWithMagnetism(bank)
	|| gBattleMons[bank].type3 == TYPE_FLYING
	|| gBattleMons[bank].type1 == TYPE_FLYING
	|| gBattleMons[bank].type2 == TYPE_FLYING)
		return IN_AIR;

	return GROUNDED;
}

bool8 NonInvasiveCheckGrounding(u8 bank, u8 defAbility, u8 defType1, u8 defType2, u8 defType3)
{
	if (BATTLER_SEMI_INVULNERABLE(bank)) //Apparently a thing
		return IN_AIR;

	if (IsGravityActive()
	|| GetRecordedItemEffect(bank) == ITEM_EFFECT_IRON_BALL
	|| (gStatuses3[bank] & (STATUS3_SMACKED_DOWN | STATUS3_ROOTED)))
		return GROUNDED;

	else if ((gStatuses3[bank] & (STATUS3_LEVITATING | STATUS3_TELEKINESIS | STATUS3_IN_AIR))
   || GetRecordedItemEffect(bank) == ITEM_EFFECT_AIR_BALLOON
   || IsFloatingWithMagnetism(bank)
   || defAbility == ABILITY_LEVITATE
   || defType1 == TYPE_FLYING
   || defType2 == TYPE_FLYING
   || defType3 == TYPE_FLYING)
		return IN_AIR;

	return GROUNDED;
}

bool8 CheckMonGrounding(struct Pokemon* mon)
{
	u16 species = GetMonData(mon, MON_DATA_SPECIES, NULL);
	u16 item = GetMonData(mon, MON_DATA_HELD_ITEM, NULL);

	if (IsGravityActive()
	|| (ItemId_GetHoldEffect(item) == ITEM_EFFECT_IRON_BALL && GetMonAbility(mon) != ABILITY_KLUTZ))
		return GROUNDED;

	else if (GetMonAbility(mon) == ABILITY_LEVITATE
	|| IsMonFloatingWithMagnetism(mon)
	|| gBaseStats[species].type1 == TYPE_FLYING
	|| gBaseStats[species].type2 == TYPE_FLYING)
		return IN_AIR;

	return GROUNDED;
}

bool8 CheckGroundingByDetails(u16 species, u16 item, u8 ability)
{
	if (ability != ABILITY_KLUTZ && ItemId_GetHoldEffect(item) == ITEM_EFFECT_IRON_BALL)
		return GROUNDED;
	else if (ability == ABILITY_LEVITATE
	|| gBaseStats[species].type1 == TYPE_FLYING
	|| gBaseStats[species].type2 == TYPE_FLYING)
		return IN_AIR;

	return GROUNDED;
}

bool8 IsFloatingWithMagnetism(u8 bank)
{
	return IsMagnetRiseBattle()
		&& (IsOfType(bank, TYPE_ELECTRIC) || IsOfType(bank, TYPE_STEEL));
}

bool8 IsMonFloatingWithMagnetism(unusedArg struct Pokemon* mon)
{
	return IsMagnetRiseBattle()
		&& (IsMonOfType(mon, TYPE_ELECTRIC) || IsMonOfType(mon, TYPE_STEEL));
}

bool8 IsAffectedByPixies(u8 bank)
{
	return IsPixieBattle() && IsOfType(bank, TYPE_FAIRY);
}

bool8 IsMonAffectedByPixies(struct Pokemon* mon)
{
	return IsPixieBattle() && IsMonOfType(mon, TYPE_FAIRY);
}

bool8 IsAffectedByShadowShieldBattle(u8 bank)
{
	return IsShadowShieldBattle() && IsOfType(bank, TYPE_GHOST);
}

bool8 IsMonAffectedByShadowShieldBattle(struct Pokemon* mon)
{
	return IsShadowShieldBattle() && IsMonOfType(mon, TYPE_GHOST);
}

bool8 DoesShadowShieldPreventHazardDamage(u8 bank)
{
	return IsAffectedByShadowShieldBattle(bank) && BATTLER_MAX_HP(bank);
}

bool8 DoesShadowShieldPreventMonHazardDamage(struct Pokemon* mon)
{
	return IsMonAffectedByShadowShieldBattle(mon) && mon->hp == mon->maxHP;
}

bool8 IsDamageHalvedDueToFullHP(u8 bank, u8 defAbility, u16 move, u8 atkAbility)
{
	if (BATTLER_MAX_HP(bank))
	{
		if (IsMultiscaleAbility(defAbility))
			return NO_MOLD_BREAKERS(atkAbility, move) || !gSpecialAbilityFlags[defAbility].gMoldBreakerIgnoredAbilities;

		return IsAffectedByShadowShieldBattle(bank);
	}

	return FALSE;
}

bool8 IsMonDamageHalvedDueToFullHP(struct Pokemon* mon, u8 defAbility, u16 move, u8 atkAbility)
{
	if (mon->hp == mon->maxHP)
	{
		if (IsMultiscaleAbility(defAbility))
			return NO_MOLD_BREAKERS(atkAbility, move) || !gSpecialAbilityFlags[defAbility].gMoldBreakerIgnoredAbilities;

		return IsMonAffectedByShadowShieldBattle(mon);
	}

	return FALSE;
}

bool8 ItemEffectPreventsHazards(u8 itemEffect)
{
	return itemEffect == ITEM_EFFECT_HEAVY_DUTY_BOOTS;
}

bool8 IsAffectedByHazards(u8 bank)
{
	return !ItemEffectPreventsHazards(ITEM_EFFECT(bank))
		&& !DoesShadowShieldPreventHazardDamage(bank);
}

bool8 IsMonAffectedByHazards(struct Pokemon* mon)
{
	return IsMonAffectedByHazardsByItemEffect(mon, GetMonItemEffect(mon));
}

bool8 IsMonAffectedByHazardsByItemEffect(struct Pokemon* mon, u8 itemEffect)
{
	return !ItemEffectPreventsHazards(itemEffect)
		&& !DoesShadowShieldPreventMonHazardDamage(mon);
}

u8 ViableMonCountFromBank(u8 bank)
{
	return (SIDE(bank) == B_SIDE_PLAYER) ? ViableMonCount(gPlayerParty) : ViableMonCount(gEnemyParty);
}

u8 ViableMonCountFromBankLoadPartyRange(u8 bank)
{
	u8 count = 0;
	u8 firstMonId, lastMonId;
	struct Pokemon* party = LoadPartyRange(bank, &firstMonId, &lastMonId);

	for (int i = firstMonId; i < lastMonId; ++i)
	{
		if (party[i].species != SPECIES_NONE
		&& !GetMonData(&party[i], MON_DATA_IS_EGG, NULL)
		&& party[i].hp != 0)
			++count;
	}

	return count;
}

bool8 HasMonToSwitchTo(u8 bank)
{
	u8 i, firstMonId, lastMonId;
	u8 battlerIn1, battlerIn2;
	u8 foe1, foe2;
	struct Pokemon* party = LoadPartyRange(bank, &firstMonId, &lastMonId);

	u8 backupActiveBattler = gActiveBattler;
	gActiveBattler = bank; //Needed for LoadBattlersAndFoes
	LoadBattlersAndFoes(&battlerIn1, &battlerIn2, &foe1, &foe2);
	gActiveBattler = backupActiveBattler;

	for (i = firstMonId; i < lastMonId; ++i)
	{
		u16 species = GetMonData(&party[i], MON_DATA_SPECIES2, 0);

		if (species != SPECIES_NONE
		&& species != SPECIES_EGG
		&& GetMonData(&party[i], MON_DATA_HP, 0) > 0
		&& i != gBattlerPartyIndexes[battlerIn1]
		&& i != gBattlerPartyIndexes[battlerIn2])
			break;
	}

	return i != lastMonId;
}

bool8 CheckContact(u16 move, u8 bankAtk, u8 bankDef)
{
	return IsContactMove(move, bankAtk, bankDef)
		&& !CanNeverMakeContact(bankAtk);
}

bool8 CheckContactByMon(u16 move, struct Pokemon* mon)
{
	return gBattleMoves[move].flags & FLAG_MAKES_CONTACT
		&& !CanMonNeverMakeContact(mon);
}

bool8 IsContactMove(u16 move, u8 bankAtk, u8 bankDef)
{
	if (move == MOVE_SHELLSIDEARM && bankAtk != bankDef)
		return gNewBS->shellSideArmSplit[bankAtk][bankDef] == SPLIT_PHYSICAL; //Calculated in advance
	else
		return gBattleMoves[move].flags & FLAG_MAKES_CONTACT;
}

bool8 CanNeverMakeContact(u8 bank)
{
	return CanNeverMakeContactByAbilityItemEffect(ABILITY(bank), ITEM_EFFECT(bank));
}

bool8 CanMonNeverMakeContact(struct Pokemon* mon)
{
	return CanNeverMakeContactByAbilityItemEffect(GetMonAbility(mon), GetMonItemEffect(mon));
}

bool8 CanNeverMakeContactByAbilityItemEffect(u8 ability, u8 itemEffect)
{
	return ability == ABILITY_LONGREACH
		|| CanNeverMakeContactByItemEffect(itemEffect);
}

bool8 CanNeverMakeContactByItemEffect(u8 itemEffect)
{
	return itemEffect == ITEM_EFFECT_PROTECTIVE_PADS;
}

bool8 CheckHealingMove(move_t move)
{
	return gBattleMoves[move].flags & FLAG_TRIAGE_AFFECTED;
}

bool8 CheckSoundMove(move_t move)
{
	return gSpecialMoveFlags[move].gSoundMoves;
}

bool8 CheckRecoil(u16 move)
{
	return move == MOVE_STRUGGLE
		|| gSpecialMoveFlags[move].gPercent25RecoilMoves
		|| gSpecialMoveFlags[move].gPercent33RecoilMoves
		|| gSpecialMoveFlags[move].gPercent50RecoilMoves
		|| gSpecialMoveFlags[move].gHalfMaxHealthRecoilMoves; //Eg. Mind Blown, Steel Beam
}

bool8 SheerForceCheck(void)
{
	return ABILITY(gBankAttacker) == ABILITY_SHEERFORCE && gSpecialMoveFlags[gCurrentMove].gSheerForceBoostedMoves;
}

bool8 IsOfType(u8 bank, u8 type)
{
	u8 type1 = gBattleMons[bank].type1;
	u8 type2 = gBattleMons[bank].type2;
	u8 type3 = gBattleMons[bank].type3;

	if (!IS_BLANK_TYPE(type1) && type1 == type)
		return TRUE;

	if (!IS_BLANK_TYPE(type2) && type2 == type)
		return TRUE;

	if (!IS_BLANK_TYPE(type3) && type3 == type)
		return TRUE;

	return FALSE;
}

bool8 LiftProtect(u8 bank)
{
	if ((gProtectStructs[bank].protected && !IsDynamaxed(bank))
	|| gProtectStructs[bank].KingsShield
	|| gProtectStructs[bank].SpikyShield
	|| gProtectStructs[bank].BanefulBunker
	|| gProtectStructs[bank].obstruct
	|| gSideStatuses[SIDE(bank)] & (SIDE_STATUS_CRAFTY_SHIELD | SIDE_STATUS_MAT_BLOCK | SIDE_STATUS_QUICK_GUARD | SIDE_STATUS_WIDE_GUARD))
	{
		if (!IsDynamaxed(bank))
			gProtectStructs[bank].protected = 0; //Max Guard is not affected by Feint

		gProtectStructs[bank].KingsShield = 0;
		gProtectStructs[bank].SpikyShield = 0;
		gProtectStructs[bank].BanefulBunker = 0;
		gProtectStructs[bank].obstruct = 0;
		gSideStatuses[SIDE(bank)] &= ~(SIDE_STATUS_CRAFTY_SHIELD | SIDE_STATUS_MAT_BLOCK | SIDE_STATUS_QUICK_GUARD | SIDE_STATUS_WIDE_GUARD);
		return TRUE;
	}

	return FALSE;
}

bool8 ProtectsAgainstZMoves(u16 move, u8 bankAtk, u8 bankDef)
{
	if (gProtectStructs[bankDef].protected
	|| gProtectStructs[bankDef].SpikyShield
	|| gProtectStructs[bankDef].BanefulBunker
	|| gProtectStructs[bankDef].obstruct)
	{
		return TRUE;
	}
	else if ((gProtectStructs[bankDef].KingsShield || (gSideStatuses[SIDE(bankDef)] & SIDE_STATUS_MAT_BLOCK))
		 && SPLIT(move) != SPLIT_STATUS)
	{
		return TRUE;
	}
	else if (gSideStatuses[SIDE(bankDef)] & SIDE_STATUS_CRAFTY_SHIELD && bankAtk != bankDef && SPLIT(move) == SPLIT_STATUS)
	{
		return TRUE;
	}
	else if (gSideStatuses[SIDE(bankDef)] & (SIDE_STATUS_QUICK_GUARD) && PriorityCalc(bankAtk, ACTION_USE_MOVE, move) > 0)
	{
		return TRUE;
	}
	else if (gSideStatuses[SIDE(bankDef)] & SIDE_STATUS_WIDE_GUARD
		&& (GetBaseMoveTarget(move, bankAtk) & (MOVE_TARGET_BOTH | MOVE_TARGET_FOES_AND_ALLY)))
	{
		return TRUE;
	}

	return FALSE;
}

bool8 StatsMaxed(u8 bank)
{
	for (u8 i = STAT_STAGE_ATK; i < BATTLE_STATS_NO; ++i)
	{
		if (STAT_STAGE(bank, i) < STAT_STAGE_MAX)
			return FALSE;
	}

	return TRUE;
}

bool8 MainStatsMaxed(u8 bank)
{
	for (u8 i = STAT_STAGE_ATK; i <= STAT_SPDEF; ++i)
	{
		if (STAT_STAGE(bank, i) < STAT_STAGE_MAX)
			return FALSE;
	}

	return TRUE;
}

bool8 StatsMinned(u8 bank)
{
	for (u8 i = STAT_STAGE_ATK; i < BATTLE_STATS_NO; ++i)
	{
		if (STAT_STAGE(bank, i) > 0)
			return FALSE;
	}
	return TRUE;
}

bool8 MainStatsMinned(u8 bank)
{
	for (u8 i = STAT_STAGE_ATK; i <= STAT_SPDEF; ++i)
	{
		if (STAT_STAGE(bank, i) > 0)
			return FALSE;
	}
	return TRUE;
}

bool8 AnyStatGreaterThan(u8 bank, u8 amount)
{
	for (u8 i = STAT_STAGE_ATK; i < BATTLE_STATS_NO; ++i)
	{
		if (STAT_STAGE(bank, i) > amount)
			return TRUE;
	}

	return FALSE;
}

u8 CountBoosts(u8 bank)
{
	u8 sum = 0;

	for (u8 i = STAT_STAGE_ATK; i < BATTLE_STATS_NO; ++i)
	{
		if (STAT_STAGE(bank, i) > 6)
			sum += STAT_STAGE(bank, i) - 6;
	}

	return sum;
}

bool8 AreDefensesHigherThanOffenses(u8 bank)
{
	u16 atk = gBattleMons[bank].attack;
	u16 spAtk = gBattleMons[bank].spAttack;
	u16 def = gBattleMons[bank].defense;
	u16 spDef = gBattleMons[bank].spDefense;

	return def + spDef > atk + spAtk;
}

u8 CheckMoveLimitations(u8 bank, u8 unusableMoves, u8 check)
{
	u8 ability = ABILITY(bank);
	u8 holdEffect = ITEM_EFFECT(bank);
	u16 choicedMove = CHOICED_MOVE(bank);
	int i;

	gStringBank = bank;
	for (i = 0; i < MAX_MON_MOVES; ++i)
	{
		u16 move = gBattleMons[bank].moves[i];
		if (IsDynamaxed(bank))
			move = GetMaxMoveByMove(bank, move);

		if (IsUnusableMove(move, bank, check, gBattleMons[bank].pp[i], ability, holdEffect, choicedMove))
			unusableMoves |= gBitTable[i];
	}

	return unusableMoves;
}

//MOVE_LIMITATION_ENCORE is checked only when every other limitation is checked
//MOVE_LIMITATION_DISABLED is checked specifically for Sleep Talk
//MOVE_LIMITATION_TAUNT is checked for status Dynamax moves
//Otherwise, limitations are checked all together
bool8 IsUnusableMove(u16 move, u8 bank, u8 check, u8 pp, u8 ability, u8 holdEffect, u16 choicedMove)
{
	bool8 isMaxMove = IsAnyMaxMove(move);

	if (move == MOVE_NONE && check & MOVE_LIMITATION_ZEROMOVE)
		return TRUE;
	else if (pp == 0 && check & MOVE_LIMITATION_PP)
		return TRUE;
	else if (!isMaxMove && move == gDisableStructs[bank].disabledMove && check & MOVE_LIMITATION_DISABLED)
		return TRUE;
	else if (!isMaxMove && move == gLastUsedMoves[bank] && check & MOVE_LIMITATION_TORMENTED && IsTormented(bank))
		return TRUE;
	else if (IsTaunted(bank) && check & MOVE_LIMITATION_TAUNT && SPLIT(move) == SPLIT_STATUS)
		return TRUE;
	else if (!isMaxMove && IsImprisoned(bank, move) && check & MOVE_LIMITATION_IMPRISION)
		return TRUE;
	else if (gDisableStructs[bank].encoreTimer && gDisableStructs[bank].encoredMove != move && check & MOVE_LIMITATION_ENCORE)
		return TRUE;
	else if (!isMaxMove
		 && IsChoiceItemEffectOrAbility(holdEffect, ability)
		 && choicedMove != 0 && choicedMove != 0xFFFF && choicedMove != move
		 && check & MOVE_LIMITATION_CHOICE)
	{
		return TRUE;
	}
	else if (holdEffect == ITEM_EFFECT_ASSAULT_VEST && IsMoveBannedByAssaultVest(move) && check & MOVE_LIMITATION_TAUNT)
		return TRUE;
	#ifdef FLAG_SKY_BATTLE
	else if (gSpecialMoveFlags[move].gSkyBattleBannedMoves && FlagGet(FLAG_SKY_BATTLE) && check & MOVE_LIMITATION_ENCORE)
		return TRUE;
	#endif
	else if (gBattleTypeFlags & BATTLE_TYPE_RING_CHALLENGE && IsMoveBannedInRingChallenge(move, bank) && check & MOVE_LIMITATION_ENCORE)
		return TRUE;
	else if (gSpecialMoveFlags[move].gGravityBannedMoves && IsGravityActive() && check & MOVE_LIMITATION_DISABLED)
		return TRUE;
	else if (CheckSoundMove(move) && CantUseSoundMoves(bank) && check & MOVE_LIMITATION_DISABLED)
		return TRUE;
	else if (CheckHealingMove(move) && IsHealBlocked(bank) && check & MOVE_LIMITATION_DISABLED)
		return TRUE;
	else if (IsRaidBattle() && bank != BANK_RAID_BOSS && gSpecialMoveFlags[move].gRaidBattleBannedMoves && check & MOVE_LIMITATION_ENCORE)
		return TRUE;

	return FALSE;
}

u8 CheckMoveLimitationsFromParty(struct Pokemon* mon, u8 unusableMoves, u8 check)
{
	u8 holdEffect = GetMonItemEffect(mon);

	for (int i = 0; i < MAX_MON_MOVES; ++i)
	{
		u16 move = GetMonData(mon, MON_DATA_MOVE1 + i, NULL);

		if (move == MOVE_NONE && check & MOVE_LIMITATION_ZEROMOVE)
			unusableMoves |= gBitTable[i];
		else if (GetMonData(mon, MON_DATA_PP1 + i, NULL) == 0 && check & MOVE_LIMITATION_PP)
			unusableMoves |= gBitTable[i];
		else if (holdEffect == ITEM_EFFECT_ASSAULT_VEST && IsMoveBannedByAssaultVest(move))
			unusableMoves |= gBitTable[i];
		#ifdef FLAG_SKY_BATTLE
		else if (check & MOVE_LIMITATION_ENCORE && FlagGet(FLAG_SKY_BATTLE) && gSpecialMoveFlags[move].gSkyBattleBannedMoves)
			unusableMoves |= gBitTable[i];
		#endif
		else if (check & MOVE_LIMITATION_ENCORE && gBattleTypeFlags & BATTLE_TYPE_RING_CHALLENGE && IsMoveBannedInRingChallengeByMon(move, mon))
			unusableMoves |= gBitTable[i];
		else if (check & MOVE_LIMITATION_DISABLED && IsGravityActive() && gSpecialMoveFlags[move].gGravityBannedMoves)
			unusableMoves |= gBitTable[i];
		else if (check & MOVE_LIMITATION_ENCORE && IsRaidBattle() && gSpecialMoveFlags[move].gRaidBattleBannedMoves)
			unusableMoves |= gBitTable[i];
	}

	return unusableMoves;
}

void CancelMultiTurnMoves(u8 battler)
{
	gBattleMons[battler].status2 &= ~(STATUS2_MULTIPLETURNS);
	gBattleMons[battler].status2 &= ~(STATUS2_LOCK_CONFUSE);
	gBattleMons[battler].status2 &= ~(STATUS2_UPROAR);
	gBattleMons[battler].status2 &= ~(STATUS2_BIDE);

	gStatuses3[battler] &= (~(STATUS3_SEMI_INVULNERABLE) | STATUS3_SKY_DROP_ATTACKER | STATUS3_SKY_DROP_TARGET); //Sky Drop is removed seperately

	gDisableStructs[battler].rolloutTimer = 0;
	gDisableStructs[battler].furyCutterCounter = 0;
}

bool8 IsMoveBannedByAssaultVest(u16 move)
{
	return SPLIT(move) == SPLIT_STATUS && move != MOVE_MEFIRST;
}

bool8 IsMoveRedirectedByFollowMe(u16 move, u8 bankAtk, u8 defSide)
{	
	if (SIDE(bankAtk) == SIDE(gSideTimers[defSide].followmeTarget)
	|| IsMoveRedirectionPrevented(move, ABILITY(bankAtk))
	|| gSideTimers[defSide].followmeTimer == 0
	|| !BATTLER_ALIVE(gSideTimers[defSide].followmeTarget)
	|| (gNewBS->ragePowdered & gBitTable[defSide] && !IsAffectedByPowder(bankAtk)))
		return FALSE;

	return TRUE;
}

bool8 IsMoveRedirectionPrevented(u16 move, u8 atkAbility)
{
	return move == MOVE_SNIPESHOT
		|| (move != MOVE_NONE && gBattleMoves[move].effect == EFFECT_SKY_DROP)
		#ifdef ABILITY_PROPELLERTAIL
		|| atkAbility == ABILITY_PROPELLERTAIL
		#endif
		|| atkAbility == ABILITY_STALWART;
}

u8 GetMoveTarget(u16 move, u8 useMoveTarget)
{
	u8 moveTarget;
	u8 bankAtk = gBankAttacker;
	u8 bankDef = 0;
	u8 atkSide, defSide;
	u8 chosen = FALSE;
	u8 atkAbility = ABILITY(bankAtk);

	if (useMoveTarget)
		moveTarget = useMoveTarget - 1;
	else
		moveTarget = GetBaseMoveTarget(move, bankAtk);

	switch (moveTarget) {
	case MOVE_TARGET_SELECTED:
		defSide = SIDE(bankAtk) ^ BIT_SIDE;
		if (IsMoveRedirectedByFollowMe(move, bankAtk, defSide))
		{
			bankDef = gSideTimers[defSide].followmeTarget;
		}
		else
		{
			if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
			{
				if (gBattleMons[defSide].hp == 0 && gBattleMons[defSide ^ BIT_FLANK].hp == 0)
				{
					bankDef = defSide; //Both targets are dead so either target on opposing side works
					chosen = TRUE;
				}
			}
			else if (gBattleMons[defSide].hp == 0)
			{
				bankDef = defSide; //Target is dead so no choice but to hit it
				chosen = TRUE;
			}

			if (!chosen) {
				atkSide = SIDE(bankAtk);
				do {
					bankDef = umodsi(Random(), gBattlersCount);
				} while (bankDef == bankAtk || atkSide == SIDE(bankDef) || gAbsentBattlerFlags & gBitTable[bankDef]);
			}

			if (NO_MOLD_BREAKERS(atkAbility, move) && !IsMoveRedirectionPrevented(move, atkAbility))
			{
				u8 moveType = GetMoveTypeSpecial(bankAtk, move);
				switch (moveType) {
					case TYPE_WATER:
						if (ABILITY(bankDef) != ABILITY_STORMDRAIN)
						{
							if (ABILITY(SIDE(bankAtk) ^ BIT_SIDE) == ABILITY_STORMDRAIN)
							{
								bankDef = SIDE(bankAtk) ^ BIT_SIDE;
								RecordAbilityBattle(bankDef, ABILITY_STORMDRAIN);
								gSpecialStatuses[bankDef].lightningRodRedirected = 1;
							}
							else if (ABILITY(PARTNER(SIDE(bankAtk) ^ BIT_SIDE)) == ABILITY_STORMDRAIN)
							{
								bankDef = PARTNER(SIDE(bankAtk) ^ BIT_SIDE);
								RecordAbilityBattle(bankDef, ABILITY_STORMDRAIN);
								gSpecialStatuses[bankDef].lightningRodRedirected = 1;
							}
							else if (ABILITY(PARTNER(bankAtk)) == ABILITY_STORMDRAIN)
							{
								bankDef = PARTNER(bankAtk);
								RecordAbilityBattle(bankDef, ABILITY_STORMDRAIN);
								gSpecialStatuses[bankDef].lightningRodRedirected = 1;
							}
						}
						break;

					case TYPE_ELECTRIC:
						if (ABILITY(bankDef) != ABILITY_LIGHTNINGROD)
						{
							if (ABILITY(SIDE(bankAtk) ^ BIT_SIDE) == ABILITY_LIGHTNINGROD)
							{
								bankDef = SIDE(bankAtk) ^ BIT_SIDE;
								RecordAbilityBattle(bankDef, ABILITY_LIGHTNINGROD);
								gSpecialStatuses[bankDef].lightningRodRedirected = 1;
							}
							else if (ABILITY(PARTNER(SIDE(bankAtk) ^ BIT_SIDE)) == ABILITY_LIGHTNINGROD)
							{
								bankDef = PARTNER(SIDE(bankAtk) ^ BIT_SIDE);
								RecordAbilityBattle(bankDef, ABILITY_LIGHTNINGROD);
								gSpecialStatuses[bankDef].lightningRodRedirected = 1;
							}
							else if (ABILITY(PARTNER(bankAtk)) == ABILITY_LIGHTNINGROD)
							{
								bankDef = PARTNER(bankAtk);
								RecordAbilityBattle(bankDef, ABILITY_LIGHTNINGROD);
								gSpecialStatuses[bankDef].lightningRodRedirected = 1;
							}
						}
				}
			}
		}
		break;

	case MOVE_TARGET_DEPENDS:
	case MOVE_TARGET_BOTH:
	case MOVE_TARGET_FOES_AND_ALLY:
	case MOVE_TARGET_OPPONENTS_FIELD:
		bankDef = GetBattlerAtPosition((GetBattlerPosition(bankAtk) & BIT_SIDE) ^ BIT_SIDE);
		if (gAbsentBattlerFlags & gBitTable[bankDef])
			bankDef ^= BIT_FLANK;
		break;

	case MOVE_TARGET_RANDOM:
		defSide = SIDE(bankAtk) ^ BIT_SIDE;
		if (IsMoveRedirectedByFollowMe(move, bankAtk, defSide))
		{
			bankDef = gSideTimers[defSide].followmeTarget;
		}
		else if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE && moveTarget & 4)
		{
			if (SIDE(bankAtk) == B_SIDE_PLAYER)
			{
				if (IsRaidBattle() || Random() & 1)
					bankDef = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
				else
					bankDef = GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT);
			}
			else
			{
				if (Random() & 1)
					bankDef = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
				else
					bankDef = GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT);
			}

			if (gAbsentBattlerFlags & gBitTable[bankDef])
				bankDef ^= 2;
		}
		else
			bankDef = GetBattlerAtPosition((GetBattlerPosition(bankAtk) & BIT_SIDE) ^ BIT_SIDE);
		break;

	case MOVE_TARGET_USER_OR_PARTNER:
	case MOVE_TARGET_USER:
		bankDef = bankAtk;
		break;
	}

	if (!gNewBS->DancerInProgress && !gNewBS->InstructInProgress)
		gBattleStruct->moveTarget[bankAtk] = bankDef;

	return bankDef;
}

u8 GetBaseMoveTarget(u16 move, u8 bankAtk)
{
	if (move == MOVE_EXPANDINGFORCE
	&& IS_DOUBLE_BATTLE
	&& gTerrainType == PSYCHIC_TERRAIN
	&& CheckGrounding(bankAtk))
		return MOVE_TARGET_BOTH; //Special property of Expanding Force in Doubles
	
	return gBattleMoves[move].target;
}

u8 GetBaseMoveTargetByGrounding(u16 move, bool8 atkIsGrounded)
{
	if (move == MOVE_EXPANDINGFORCE
	&& IS_DOUBLE_BATTLE
	&& gTerrainType == PSYCHIC_TERRAIN
	&& atkIsGrounded)
		return MOVE_TARGET_BOTH; //Special property of Expanding Force in Doubles

	return gBattleMoves[move].target;
}

bool8 IsBattlerAlive(u8 bank)
{
	if (!BATTLER_ALIVE(bank)
	||	bank >= gBattlersCount
	||	gAbsentBattlerFlags & gBitTable[bank])
	{
		return FALSE;
	}

	return TRUE;
}

struct Pokemon* LoadPartyRange(u8 bank, u8* firstMonId, u8* lastMonId)
{
	struct Pokemon* party;

	party = (SIDE(bank) == B_SIDE_OPPONENT) ? gEnemyParty : gPlayerParty;

	if (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER && SIDE(bank) == B_SIDE_PLAYER)
	{
		*firstMonId = 0;
		if (GetBattlerPosition(bank) == B_POSITION_PLAYER_RIGHT)
			*firstMonId = 3;

		*lastMonId = *firstMonId + 3;
	}
	else if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
	{
		//Two Human Trainers vs Two AI Trainers
		if (gBattleTypeFlags & BATTLE_TYPE_LINK && gBattleTypeFlags & BATTLE_TYPE_FRONTIER)
		{
			if (SIDE(bank) == B_SIDE_PLAYER)
			{
				*firstMonId = 0;
				if (GetLinkTrainerFlankId(GetBattlerMultiplayerId(bank)) == TRUE)
					*firstMonId = 3;
			}
			else //B_SIDE_OPPONENT
			{
				if (bank == B_POSITION_OPPONENT_LEFT)
					*firstMonId = 0;
				else
					*firstMonId = 3;
			}
		}
		//Two Human Trainers vs Two Human Trainers
		else
		{
			*firstMonId = 0;
			if (GetLinkTrainerFlankId(GetBattlerMultiplayerId(bank)) == TRUE)
				*firstMonId = 3;
		}

		*lastMonId = *firstMonId + 3;
	}
	else if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS && SIDE(bank) == B_SIDE_OPPONENT)
	{
		*firstMonId = 0;
		if (GetBattlerPosition(bank) == B_POSITION_OPPONENT_RIGHT)
			*firstMonId = 3;

		*lastMonId = *firstMonId + 3;
	}
	else //Single Battle // Regular Double Battle // Regular Double Against Two Trainers + PlayerSide // Wild Double Battle
	{
		*firstMonId = 0;
		*lastMonId = PARTY_SIZE;
	}

	return party;
}

bool8 UproarWakeUpCheck(unusedArg u8 bank)
{
	u32 i;

	for (i = 0; i < gBattlersCount; ++i)
	{
		if (gBattleMons[i].status2 & STATUS2_UPROAR)
		{
			gBattleScripting.bank = i;
			if (gBankTarget == 0xFF)
				gBankTarget = i;
			else if (gBankTarget == i)
				gBattleCommunication[MULTISTRING_CHOOSER] = 0;
			else
				gBattleCommunication[MULTISTRING_CHOOSER] = 1;

			break;
		}
	}

	return i < gBattlersCount;
}

bool8 IsUproarBeingMade(void)
{
	for (u32 i = 0; i < gBattlersCount; ++i)
	{
		if (gBattleMons[i].status2 & STATUS2_UPROAR)
			return TRUE;
	}

	return FALSE;
}

//Change to loop through battle modified party indexes
u8 GetIllusionPartyNumber(u8 bank)
{
	u8 firstMonId, lastMonId, side, illusionMonId;
	bool8 useBattleParty;

	if (gStatuses3[bank] & STATUS3_ILLUSION)
	{
		side = SIDE(bank);
	
		//Wild Pokemon can't diguise themselves
		if (!(gBattleTypeFlags & BATTLE_TYPE_TRAINER) && side == B_SIDE_OPPONENT)
			return gBattlerPartyIndexes[bank];
		
		//Check for a saved party number first.
		//This is necessary for when in doubles the last alive Pokemon is KOd.
		//If the party was checked each time, the Pokemon could change who it was
		//disguised as mid-Illusion.
		if (gNewBS->disguisedAs[bank] > 0)
			return gNewBS->disguisedAs[bank] - 1;

		struct Pokemon* party = LoadPartyRange(bank, &firstMonId, &lastMonId);
		useBattleParty = (side == B_SIDE_PLAYER && !(gBattleTypeFlags & BATTLE_TYPE_LINK));
		illusionMonId = (useBattleParty) ? GetBattlePartyIdFromPartyId(gBattlerPartyIndexes[bank]) : gBattlerPartyIndexes[bank];
		bool8 isTagBattle = IsTagBattle();
	
		if (useBattleParty && isTagBattle)
			lastMonId += 1; //Because team is slots 0, 2, 3

		for (u8 i = lastMonId - 1; i >= firstMonId; --i) //Loop through party in reverse order
		{
			u8 monId = i; //Mon id may get overwritten later

			if (i == 1 && useBattleParty && isTagBattle)
				--i;

			if (monId == illusionMonId) //Finished checking mons after
				return gBattlerPartyIndexes[bank];

			if (useBattleParty)
				monId = GetPartyIdFromBattleSlot(monId); //Use the adjusted party order

			if (party[monId].species == SPECIES_NONE
			|| party[monId].hp == 0
			|| GetMonData(&party[monId], MON_DATA_IS_EGG, NULL)
			|| monId < firstMonId
			|| monId >= lastMonId)
				continue;

			gNewBS->disguisedAs[bank] = monId + 1; //Use original mon id
			return monId;
		}
	}

	return gBattlerPartyIndexes[bank];
}

u8 GetIllusionPartyNumberForShiftSwitch(struct Pokemon* party, u8 monId, u8 firstMonId, u8 lastMonId)
{
	for (u32 i = lastMonId - 1; i >= firstMonId; --i) //Loop through party in reverse order
	{
		if (i == monId) //Finished checking mons after
			return monId;

		if (party[i].species == SPECIES_NONE
		|| party[i].hp == 0
		|| GetMonData(&party[i], MON_DATA_IS_EGG, NULL))
			continue;

		return i;
	}
	
	return monId;
}

struct Pokemon* GetIllusionPartyData(u8 bank)
{
	u8 firstMonId, lastMonId;
	struct Pokemon* party = LoadPartyRange(bank, &firstMonId, &lastMonId);

	return &party[GetIllusionPartyNumber(bank)];
}

void GetMonIdAndSideByMon(struct Pokemon* mon, u32* monId, u32* side)
{
	u32 tempMonId;

	if ((u32) mon >= (u32) gPlayerParty && (u32) mon <= (u32) &gPlayerParty[PARTY_SIZE - 1])
	{
		tempMonId = (u32) mon - (u32) gPlayerParty;
		*monId = tempMonId / sizeof(struct Pokemon);
		*side = B_SIDE_PLAYER;
	}
	else if ((u32) mon >= (u32) gEnemyParty && (u32) mon <= (u32) &gEnemyParty[PARTY_SIZE - 1])
	{
		tempMonId = (u32) mon - (u32) gEnemyParty;
		*monId = tempMonId / sizeof(struct Pokemon);
		*side = B_SIDE_OPPONENT;
	}
	else
	{
		//This mon is probably on the stack
		*monId = 0xFF;
		*side = 0xFF;
	}
}

u16 GetWishHPRecovery(u8 bank, bool8 ignoreWishTimer)
{
	u16 recovery = 0;

	if (gWishFutureKnock.wishCounter[bank] > 0 || ignoreWishTimer)
	{
		u8 firstId, lastId;
		struct Pokemon* party = LoadPartyRange(bank, &firstId, &lastId);
		recovery = MathMax(1, GetMonData(&party[gWishFutureKnock.wishMonId[bank]], MON_DATA_MAX_HP, NULL) / 2);
	}

	return recovery;
}

bool8 BankMovedBefore(u8 bank1, u8 bank2)
{
	for (u32 i = 0; i < gBattlersCount; ++i)
	{
		if (gBanksByTurnOrder[i] == bank1)
			return TRUE;
		else if (gBanksByTurnOrder[i] == bank2)
			return FALSE;
	}

	return FALSE; //Should never be reached
}

bool8 BankMovedBeforeIgnoreSwitch(u8 bank1, u8 bank2)
{
	for (u32 i = 0; i < gBattlersCount; ++i)
	{
		if (gChosenActionByBank[gBanksByTurnOrder[i]] == ACTION_SWITCH)
			continue;

		if (gBanksByTurnOrder[i] == bank1)
			return TRUE;
		else if (gBanksByTurnOrder[i] == bank2)
			return FALSE;
	}

	return FALSE; //Should never be reached
}

bool8 IsFirstAttacker(u8 bank)
{
	for (u8 i = 0; i < gBattlersCount; ++i)
	{
		if (gActionsByTurnOrder[i] == ACTION_USE_ITEM
		||  gActionsByTurnOrder[i] == ACTION_SWITCH)
			continue;

		return gBanksByTurnOrder[i] == bank;
	}

	return FALSE; //Should never be reached
}

bool8 CanTransferItem(u16 species, u16 item)
{
	u16 dexNum = SpeciesToNationalPokedexNum(species);
	u8 effect = ItemId_GetHoldEffect(item);
	const struct Evolution* evolutions = gEvolutionTable[species];
	int i;

	if (item == ITEM_NONE || IsMail(item))
		return FALSE;

	switch (effect) {
		case ITEM_EFFECT_Z_CRYSTAL:
			return FALSE;

		#ifdef NATIONAL_DEX_GIRATINA
		case ITEM_EFFECT_GRISEOUS_ORB:
			if (dexNum == NATIONAL_DEX_GIRATINA)
				return FALSE;
			break;
		#endif

	#ifdef PLA_HELD_ORIGIN_ORBS
		#ifdef NATIONAL_DEX_DIALGA
		case ITEM_EFFECT_ADAMANT_ORB:
			if (dexNum == NATIONAL_DEX_DIALGA)
				return FALSE;
			break;
		#endif

		#ifdef NATIONAL_DEX_PALKIA
		case ITEM_EFFECT_LUSTROUS_ORB:
			if (dexNum == NATIONAL_DEX_PALKIA)
				return FALSE;
			break;
		#endif
	#endif

		#ifdef NATIONAL_DEX_ARCEUS
		case ITEM_EFFECT_PLATE:
			if (dexNum == NATIONAL_DEX_ARCEUS)
				return FALSE;
			break;
		#endif

		#ifdef NATIONAL_DEX_SILVALLY
		case ITEM_EFFECT_MEMORY:
			if (dexNum == NATIONAL_DEX_SILVALLY)
				return FALSE;
			break;
		#endif

		#ifdef NATIONAL_DEX_GENESECT
		case ITEM_EFFECT_DRIVE:
			if (dexNum == NATIONAL_DEX_GENESECT)
				return FALSE;
			break;
		#endif

		#ifdef NATIONAL_DEX_ZACIAN
		case ITEM_EFFECT_RUSTED_SWORD:
			if (dexNum == NATIONAL_DEX_ZACIAN)
				return FALSE;
			break;
		#endif

		#ifdef NATIONAL_DEX_ZAMAZENTA
		case ITEM_EFFECT_RUSTED_SHIELD:
			if (dexNum == NATIONAL_DEX_ZAMAZENTA)
				return FALSE;
			break;
		#endif

		case ITEM_EFFECT_MEGA_STONE:
			for (i = 0; i < EVOS_PER_MON; ++i)
			{
				if (evolutions[i].method == EVO_NONE) //Most likely end of entries
					break; //Break now to save time
				else if ((evolutions[i].method == MEGA_EVOLUTION && evolutions[i].param == item) //Can Mega Evolve
				||  (evolutions[i].method == MEGA_EVOLUTION && evolutions[i].param == 0)) //Is Mega
					return FALSE;
			}
			break;

		case ITEM_EFFECT_PRIMAL_ORB:
			return !IsPrimalSpecies(species)
				&& GetPrimalSpecies(species, item) == SPECIES_NONE; //Can't primal evolve
	}

	return TRUE;
}

//Make sure the input bank is any bank on the specific mon's side
bool8 CanFling(u16 item, u16 species, u8 ability, u8 bankOnSide, u8 embargoTimer)
{
	u8 itemEffect = ItemId_GetHoldEffect(item);

	if (item == ITEM_NONE
	|| ability == ABILITY_KLUTZ
	|| IsMagicRoomActive()
	|| embargoTimer != 0
	|| !CanTransferItem(species, item)
	|| itemEffect == ITEM_EFFECT_PRIMAL_ORB
	|| itemEffect == ITEM_EFFECT_GEM
	|| itemEffect == ITEM_EFFECT_ABILITY_CAPSULE
	|| itemEffect == ITEM_EFFECT_RUSTED_SWORD
	|| itemEffect == ITEM_EFFECT_RUSTED_SHIELD
	|| (IsBerry(item) && UnnerveOnOpposingField(bankOnSide))
	|| GetPocketByItemId(item) == POCKET_POKE_BALLS)
		return FALSE;

	return TRUE;
}

bool8 SymbiosisCanActivate(u8 giverBank, u8 receiverBank)
{
	u16 item = ITEM(giverBank);

	if (!(gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
	||  ABILITY(giverBank) != ABILITY_SYMBIOSIS
	||  ITEM(receiverBank) != ITEM_NONE
	||  ITEM(giverBank) == ITEM_NONE
	|| !CanTransferItem(gBattleMons[giverBank].species, item)
	|| !CanTransferItem(gBattleMons[receiverBank].species, item)
	|| (gNewBS->corrodedItems[SIDE(receiverBank)] & gBitTable[gBattlerPartyIndexes[receiverBank]]))
		return FALSE;

	return TRUE;
}

//Sticky Hold also, but the boost ignores it
bool8 CanKnockOffItem(u8 bank)
{
	u16 item = ITEM(bank);

	if (item == ITEM_NONE)
		return FALSE;

	if (!(gBattleTypeFlags & BATTLE_TYPE_TRAINER) && SIDE(bank) == B_SIDE_PLAYER) //Wild mons can't knock off items
		return FALSE;

	if (!CanTransferItem(SPECIES(bank), item))
		return FALSE;

	return TRUE;
}

bool8 CanKnockOffMonItem(struct Pokemon* mon, u8 side)
{
	u16 item = GetMonData(mon, MON_DATA_HELD_ITEM, NULL);

	if (item == ITEM_NONE)
		return FALSE;

	if (!(gBattleTypeFlags & BATTLE_TYPE_TRAINER) && side == B_SIDE_PLAYER) //Wild mons can't knock off items
		return FALSE;

	if (!CanTransferItem(GetMonData(mon, MON_DATA_SPECIES, NULL), item))
		return FALSE;

	return TRUE;
}

bool8 IsBankHoldingFocusSash(u8 bank)
{
	return ITEM_EFFECT(bank) == ITEM_EFFECT_FOCUS_BAND && ItemId_GetMystery2(ITEM(bank));
}

bool8 IsMonHoldingFocusSash(struct Pokemon* mon)
{
	return GetMonItemEffect(mon) == ITEM_EFFECT_FOCUS_BAND && ItemId_GetMystery2(mon->item);
}

bool8 IsAffectedByFocusSash(u8 bank)
{
	return BATTLER_MAX_HP(bank) && IsBankHoldingFocusSash(bank);
}

bool8 IsMonAffectedByFocusSash(struct Pokemon* mon)
{
	return mon->hp == mon->maxHP && IsMonHoldingFocusSash(mon);
}

bool8 CanBeTrapped(u8 bank)
{
	return !IsOfType(bank, TYPE_GHOST) && ITEM_EFFECT(bank) != ITEM_EFFECT_SHED_SHELL;
}

bool8 IsAffectedByPowder(u8 bank)
{
	return IsAffectedByPowderByDetails(gBattleMons[bank].type1, gBattleMons[bank].type2, gBattleMons[bank].type3, ABILITY(bank), ITEM_EFFECT(bank));
}

bool8 IsAffectedByPowderByDetails(u8 type1, u8 type2, u8 type3, u8 ability, u8 itemEffect)
{
	return ability != ABILITY_OVERCOAT
		&& itemEffect != ITEM_EFFECT_SAFETY_GOGGLES
		&& type1 != TYPE_GRASS
		&& type2 != TYPE_GRASS
		&& type3 != TYPE_GRASS;
}

bool8 MoveIgnoresSubstitutes(u16 move, u8 atkAbility)
{
	return CheckSoundMove(move)
		|| (BypassesScreens(atkAbility) && move != MOVE_TRANSFORM && gBattleMoves[move].effect != EFFECT_SKY_DROP)
		|| gSpecialMoveFlags[move].gSubstituteBypassMoves;
}

bool8 MoveBlockedBySubstitute(u16 move, u8 bankAtk, u8 bankDef)
{
	return IS_BEHIND_SUBSTITUTE(bankDef) && !MoveIgnoresSubstitutes(move, ABILITY(bankAtk));
}

bool8 MonMoveBlockedBySubstitute(u16 move, struct Pokemon* monAtk, u8 bankDef)
{
	return IS_BEHIND_SUBSTITUTE(bankDef) && !MoveIgnoresSubstitutes(move, GetMonAbility(monAtk));
}

bool8 BypassesScreens(u8 ability)
{
	return ability == ABILITY_INFILTRATOR;
}

bool8 BypassesFog(unusedArg u8 ability, unusedArg u8 itemEffect)
{
	#ifdef UNBOUND
	return BypassesScreens(ability) || ability == ABILITY_KEENEYE || ItemEffectIgnoresSunAndRain(itemEffect);
	#else
	return FALSE;
	#endif
}

bool8 IsAuraBoss(u8 bank)
{
	return !(gBattleTypeFlags & BATTLE_TYPE_TRAINER) && VarGet(VAR_TOTEM + bank) != 0;
}

bool8 IsMockBattle(void)
{
	return (gBattleTypeFlags & BATTLE_TYPE_MOCK_BATTLE) != 0;
}

bool8 IsPlayerInControl(u8 bank)
{
	if (SIDE(bank) == B_SIDE_OPPONENT || IsMockBattle()) //AI side or AI controls everyone
		return FALSE;

	if (IsTagBattle() && GetBattlerPosition(bank) == B_POSITION_PLAYER_RIGHT)
		return FALSE;

	return TRUE;
}

bool8 IsMoveAffectedByParentalBond(u16 move, u8 bankAtk)
{
	if (SPLIT(move) != SPLIT_STATUS
	&& !IsAnyMaxMove(move)
	&& !gSpecialMoveFlags[move].gParentalBondBannedMoves
	&& !IsTwoTurnsMove(move)
	&& gBattleMoves[move].effect != EFFECT_0HKO
	&& gBattleMoves[move].effect != EFFECT_MULTI_HIT
	&& gBattleMoves[move].effect != EFFECT_TRIPLE_KICK
	&& gBattleMoves[move].effect != EFFECT_DOUBLE_HIT)
	{
		if (IS_DOUBLE_BATTLE)
		{
			switch (GetBaseMoveTarget(move, bankAtk)) {
				case MOVE_TARGET_BOTH:
					if (CountAliveMonsInBattle(BATTLE_ALIVE_DEF_SIDE, 0, FOE(bankAtk)) >= 2) //Check for single target
						return FALSE;
					break;
				case MOVE_TARGET_FOES_AND_ALLY:
					if (CountAliveMonsInBattle(BATTLE_ALIVE_EXCEPT_ACTIVE, bankAtk, 0) >= 2) //Count mons on both sides; ignore attacker
						return FALSE;
					break;
			}
		}

		return TRUE;
	}
	
	return FALSE;
}

u8 CalcMoveSplit(u16 move, u8 bankAtk, u8 bankDef)
{
	if (gSpecialMoveFlags[move].gMovesThatChangePhysicality
	&&  SPLIT(move) != SPLIT_STATUS)
	{
		u32 attack = gBattleMons[bankAtk].attack;
		u32 spAttack = gBattleMons[bankAtk].spAttack;
		
		APPLY_QUICK_STAT_MOD(attack, STAT_STAGE(bankAtk, STAT_STAGE_ATK));
		APPLY_QUICK_STAT_MOD(spAttack, STAT_STAGE(bankAtk, STAT_STAGE_SPATK));

		if (spAttack >= attack)
			return SPLIT_SPECIAL;
		else
			return SPLIT_PHYSICAL;
	}
	else if (move == MOVE_SHELLSIDEARM
	&& bankAtk != bankDef) //Indicator to just use the base physicality
	{
		return gNewBS->shellSideArmSplit[bankAtk][bankDef];
	}

	#ifdef OLD_MOVE_SPLIT
		if (gBattleMoves[move].type < TYPE_FIRE)
			return SPLIT_PHYSICAL;
		else
			return SPLIT_SPECIAL;
	#else
		return SPLIT(move);
	#endif
}

u8 CalcMoveSplitFromParty(u16 move, struct Pokemon* mon)
{
	if (gSpecialMoveFlags[move].gMovesThatChangePhysicality)
	{
		if (mon->spAttack >= mon->attack)
			return SPLIT_SPECIAL;
		else
			return SPLIT_PHYSICAL;
	}

	return SPLIT(move);
}

u8 FindMovePositionInMoveset(u16 move, u8 bank)
{
	int i;

	for (i = 0; i < MAX_MON_MOVES; ++i)
	{
		if (gBattleMons[bank].moves[i] == move)
			break;
	}

	return i;
}

bool8 MoveInMoveset(u16 move, u8 bank)
{
	return FindMovePositionInMoveset(move, bank) < MAX_MON_MOVES;
}

u8 AttacksThisTurn(u8 bank, u16 move) // Note: returns 1 if it's a charging turn, otherwise 2
{
	u8 moveEffect = gBattleMoves[move].effect;

	if (ITEM_EFFECT(bank) == ITEM_EFFECT_POWER_HERB)
		return 2;

	if (moveEffect == EFFECT_SOLARBEAM && (gBattleWeather & WEATHER_SUN_ANY) && WEATHER_HAS_EFFECT)
		return 2;

	if (moveEffect == EFFECT_SKULL_BASH
	||  moveEffect == EFFECT_RAZOR_WIND
	||  moveEffect == EFFECT_SKY_ATTACK
	||  moveEffect == EFFECT_SOLARBEAM
	||  moveEffect == EFFECT_SEMI_INVULNERABLE
	||  moveEffect == EFFECT_SKY_DROP
	||  moveEffect == EFFECT_BIDE
	||	move == MOVE_GEOMANCY)
	{
		if (gBattleMons[bank].status2 & STATUS2_MULTIPLETURNS)
			return 2;
		else
			return 1; //About to initiate attack.
	}

	return 2;
}

bool8 IsZMove(const u16 move)
{
	return move >= FIRST_Z_MOVE && move <= LAST_Z_MOVE;
}

void ResetVarsForAbilityChange(u8 bank)
{
	gNewBS->SlowStartTimers[bank] = 0;
	gDisableStructs[gBankTarget].truantCounter = 0;
	gStatuses3[bank] &= ~(STATUS3_SWITCH_IN_ABILITY_DONE);
	TryRemoveUnburdenBoost(bank);
}

void HandleUnburdenBoost(u8 bank)
{
	//1. Boost bit is only set if the Pokemon has Unburden.
	//2. Losing Unburden clears the boost, even if the Pokemon gets Unburden back.
	//3. If Unburden is suppressed, the boost isn't lost if Unburden returns,
	//   but a new one can't be gained during the suppression.
	if (ABILITY(bank) == ABILITY_UNBURDEN && ITEM(bank) == ITEM_NONE)
		gNewBS->UnburdenBoosts |= gBitTable[bank];
	else if (*GetAbilityLocation(bank) != ABILITY_UNBURDEN || ITEM(bank) != ITEM_NONE)
		gNewBS->UnburdenBoosts &= ~gBitTable[bank];
}

static void TryRemoveUnburdenBoost(u8 bank)
{
	if (*GetAbilityLocation(bank) != ABILITY_UNBURDEN || ITEM(bank) != ITEM_NONE)
		gNewBS->UnburdenBoosts &= ~gBitTable[bank];
}

void AddBankToPickupStack(const u8 bank)
{
	u32 i, j;
	u8 newStack[MAX_BATTLERS_COUNT];

	for (i = 0, j = 0; i < gBattlersCount; ++i)
	{
		if (gNewBS->pickupStack[i] != bank && gNewBS->pickupStack[i] != 0xFF)
			newStack[j++] = gNewBS->pickupStack[i];
	}

	newStack[j++] = bank;

	while (j < gBattlersCount)
		newStack[j++] = 0xFF;

	for (i = 0; i < gBattlersCount; ++i)
		gNewBS->pickupStack[i] = newStack[i];
}

void RemoveBankFromPickupStack(const u8 bank)
{
	u32 i, j;
	u8 newStack[MAX_BATTLERS_COUNT];

	for (i = 0, j = 0; i < gBattlersCount; ++i)
	{
		if (gNewBS->pickupStack[i] != bank && gNewBS->pickupStack[i] != 0xFF)
			newStack[j++] = gNewBS->pickupStack[i];
	}

	while (j < gBattlersCount)
		newStack[j++] = 0xFF;

	for (i = 0; i < gBattlersCount; ++i)
		gNewBS->pickupStack[i] = newStack[i];
}

u8 GetTopOfPickupStackNotIncludingBank(const u8 bank)
{
	u32 i;

	for (i = 0; i < gBattlersCount; ++i)
	{
		if (gNewBS->pickupStack[i] == 0xFF)
			break;
	}

	if (i == 0 //Stack is empty
	|| (i == 1 && gNewBS->pickupStack[0] == bank)) //Stack only contains ignored bank
		return 0xFF;

	if (gNewBS->pickupStack[i - 1] == bank)
		return gNewBS->pickupStack[i - 2];

	return gNewBS->pickupStack[i - 1];
}

void RemoveScreensFromSide(const u8 side)
{
	gSideStatuses[side] &= ~(SIDE_STATUS_REFLECT);
	gSideStatuses[side] &= ~(SIDE_STATUS_LIGHTSCREEN);
	gSideTimers[side].reflectTimer = 0;
	gSideTimers[side].lightscreenTimer = 0;
	gNewBS->AuroraVeilTimers[side] = 0;
}

void UpdateQuickClawRandomNumber(u8 bank)
{
	gNewBS->quickClawRandomNumber[bank] = ((u32) Random32()) % 100;
	gNewBS->quickDrawRandomNumber[bank] = ((u32) Random32()) % 100;
}

void GiveOmniboost(u8 bank)
{
	u32 i;

	for (i = STAT_STAGE_ATK; i <= STAT_STAGE_SPDEF; ++i)
	{
		if (STAT_STAGE(bank, i) < STAT_STAGE_MAX)
			++STAT_STAGE(bank, i);
	}
}

bool8 WillPoltergeistFail(u16 item, u8 ability)
{
	return item == ITEM_NONE
		|| ability == ABILITY_KLUTZ
		|| IsMagicRoomActive();
}

bool8 WillSyncronoiseFail(u8 bankAtk, u8 bankDef)
{
	u8 atkType1 = gBattleMons[bankAtk].type1;
	u8 atkType2 = gBattleMons[bankAtk].type2;
	u8 atkType3 = gBattleMons[bankAtk].type3;

	u8 defType1 = gBattleMons[bankDef].type1;
	u8 defType2 = gBattleMons[bankDef].type2;
	u8 defType3 = gBattleMons[bankDef].type3;

	u8 defItemEffect = ITEM_EFFECT(bankDef);

	return WillSyncronoiseFailByAttackerTypesAnd3DefTypesAndItemEffect(atkType1, atkType2, atkType3, defType1, defType2, defType3, defItemEffect);
}

bool8 WillSyncronoiseFailByAttackerTypesAndBank(u8 atkType1, u8 atkType2, u8 atkType3, u8 bankDef)
{
	u8 defType1 = gBattleMons[bankDef].type1;
	u8 defType2 = gBattleMons[bankDef].type2;
	u8 defType3 = gBattleMons[bankDef].type3;

	u8 defItemEffect = ITEM_EFFECT(bankDef);

	return WillSyncronoiseFailByAttackerTypesAnd3DefTypesAndItemEffect(atkType1, atkType2, atkType3, defType1, defType2, defType3, defItemEffect);
}

bool8 WillSyncronoiseFailByAttackerAnd3DefTypesAndItemEffect(u8 bankAtk, u8 defType1, u8 defType2, u8 defType3, u8 defItemEffect)
{
	u8 atkType1 = gBattleMons[bankAtk].type1;
	u8 atkType2 = gBattleMons[bankAtk].type2;
	u8 atkType3 = gBattleMons[bankAtk].type3;

	return WillSyncronoiseFailByAttackerTypesAnd3DefTypesAndItemEffect(atkType1, atkType2, atkType3, defType1, defType2, defType3, defItemEffect);
}

bool8 WillSyncronoiseFailByAttackerTypesAnd2DefTypesAndItemEffect(u8 atkType1, u8 atkType2, u8 atkType3, u8 defType1, u8 defType2, u8 defItemEffect)
{
	return WillSyncronoiseFailByAttackerTypesAnd3DefTypesAndItemEffect(atkType1, atkType2, atkType3, defType1, defType2, TYPE_BLANK, defItemEffect);
}

bool8 WillSyncronoiseFailByAttackerTypesAnd3DefTypesAndItemEffect(u8 atkType1, u8 atkType2, u8 atkType3, u8 defType1, u8 defType2, u8 defType3, u8 defItemEffect)
{
	if (defItemEffect == ITEM_EFFECT_RING_TARGET)
		return FALSE;

	if (!IS_BLANK_TYPE(atkType1))
	{
		if (atkType1 == defType1
		||  atkType1 == defType2
		||  atkType1 == defType3)
			return FALSE; //No fail
	}

	if (!IS_BLANK_TYPE(atkType2))
	{
		if (atkType2 == defType1
		||  atkType2 == defType2
		||  atkType2 == defType3)
			return FALSE; //No fail
	}

	if (!IS_BLANK_TYPE(atkType3))
	{
		if (atkType3 == defType1
		||  atkType3 == defType2
		||  atkType3 == defType3)
			return FALSE; //No fail
	}

	return TRUE; //No type in common so fail
}

bool8 IsAffectedByElectricTerrain(u8 bank)
{
	return CheckGrounding(bank) || IsFloatingWithMagnetism(bank); //Draws Electric Terrain up to it when floating with electromagnetism
}

bool8 IsMonAffectedByElectricTerrain(struct Pokemon* mon)
{
	return CheckMonGrounding(mon) || IsMonFloatingWithMagnetism(mon);
}

bool8 WeatherHasEffect(void)
{
	u32 i;

	for (i = 0; i < gBattlersCount; ++i)
	{
		u8 ability = ABILITY(i);

		if ((ability == ABILITY_CLOUDNINE
		#ifdef ABILITY_AIRLOCK
		|| ability == ABILITY_AIRLOCK
		#endif
		)
		&& BATTLER_ALIVE(i))
			return FALSE;
	}

	return TRUE;
}

bool8 RainCanBeEvaporated(void)
{
	return gBattleWeather & WEATHER_RAIN_ANY
		&& !(gBattleWeather & (WEATHER_PRIMAL_ANY | WEATHER_CIRCUS));
}

bool8 ItemEffectIgnoresSunAndRain(u8 itemEffect)
{
	return itemEffect == ITEM_EFFECT_UTILITY_UMBRELLA;
}

bool8 IgnoresSunAndRain(u8 bank)
{
	return ItemEffectIgnoresSunAndRain(ITEM_EFFECT(bank));
}

bool8 AffectedBySun(u8 bank)
{
	return !IgnoresSunAndRain(bank);
}

bool8 AffectedByRain(u8 bank)
{
	return !IgnoresSunAndRain(bank);
}

bool8 IsChoiceItemEffectOrAbility(u8 itemEffect, u8 ability)
{
	return itemEffect == ITEM_EFFECT_CHOICE_BAND || IsChoiceAbility(ability);
}

u8 GetImposterBank(u8 bank)
{
	u8 transformBank;

	if (IS_SINGLE_BATTLE)
		transformBank = FOE(bank);
	else if (IsRaidBattle())
	{
		if (SIDE(bank) == B_SIDE_PLAYER)
			transformBank = BANK_RAID_BOSS;
		else //Raid boss always transforms into player's Pokemon
			transformBank = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
	}
	else //Standard double battle
		transformBank = GetBattlerAtPosition(PARTNER(BATTLE_OPPOSITE(GetBattlerPosition(bank)))); //Directly in front of in doubles

	return transformBank;
}

bool8 ImposterWorks(u8 bankAtk, bool8 checkingMonAtk) //bankAtk here is mainly used for the battle slot
{
	u8 targetBank = GetImposterBank(bankAtk);

	return BATTLER_ALIVE(targetBank)
		&& !(gBattleMons[targetBank].status2 & (STATUS2_TRANSFORMED | STATUS2_SUBSTITUTE))
		&& !(gStatuses3[targetBank] & (STATUS3_SEMI_INVULNERABLE | STATUS3_ILLUSION))
		&& (checkingMonAtk || !IS_TRANSFORMED(bankAtk)) //Obviously a party mon can't be transformed
		#ifdef UNBOUND
		&& SPECIES(targetBank) != SPECIES_SHADOW_WARRIOR
		&& (SPECIES(targetBank) < SPECIES_CELEBI || SPECIES(targetBank) > SPECIES_TREECKO) //Those Pokemon in between are used for Fakemon bosses
		#endif
		&& !HasRaidShields(targetBank)
		&& !ABILITY_ON_FIELD(ABILITY_NEUTRALIZINGGAS);
}

void ClearBankStatus(u8 bank)
{
	if (gBattleMons[bank].status1 & (STATUS_POISON | STATUS_TOXIC_POISON))
		StringCopy(gBattleTextBuff1, gStatusConditionString_Poison);
	else if (gBattleMons[bank].status1 & STATUS_SLEEP)
		StringCopy(gBattleTextBuff1, gStatusConditionString_Sleep);
	else if (gBattleMons[bank].status1 & STATUS_PARALYSIS)
		StringCopy(gBattleTextBuff1, gStatusConditionString_Paralysis);
	else if (gBattleMons[bank].status1 & STATUS_BURN)
		StringCopy(gBattleTextBuff1, gStatusConditionString_Burn);
	else if (gBattleMons[bank].status1 & STATUS_FREEZE)
		StringCopy(gBattleTextBuff1, gStatusConditionString_Ice);

	gBattleMons[bank].status1 = 0;
	gBattleMons[bank].status2 &= ~(STATUS2_NIGHTMARE);
	gActiveBattler = bank;
	EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[bank].status1);
	MarkBufferBankForExecution(gActiveBattler);
}

bool8 IsSleepClauseInEffect(unusedArg u8 bankToPutToSleep)
{
	if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER)
	{
		switch (VarGet(VAR_BATTLE_FACILITY_TIER))
		{
			case BATTLE_FACILITY_OU:
			case BATTLE_FACILITY_UBER:
			case BATTLE_FACILITY_LITTLE_CUP:
			case BATTLE_FACILITY_MONOTYPE:
			case BATTLE_FACILITY_CAMOMONS:
			case BATTLE_FACILITY_UBER_CAMOMONS:
			case BATTLE_FACILITY_LC_CAMOMONS:
			case BATTLE_FACILITY_SCALEMONS:
			case BATTLE_FACILITY_350_CUP:
			case BATTLE_FACILITY_AVERAGE_MONS:
			case BATTLE_FACILITY_BENJAMIN_BUTTERFREE:
			case BATTLE_FACILITY_NATIONAL_DEX_OU:
			case BATTLE_FACILITY_UU:
			case BATTLE_FACILITY_RU:
			case BATTLE_FACILITY_NU:
				return TRUE;
		}
	}
	#ifdef VAR_GAME_DIFFICULTY
	else if (VarGet(VAR_GAME_DIFFICULTY) >= OPTIONS_EXPERT_DIFFICULTY) //Insane
	{
		if (SIDE(bankToPutToSleep) == B_SIDE_OPPONENT) //Bank to be put to sleep
			return TRUE; //Only player is affected by the sleep clause
	}
	#endif

	return FALSE;
}

bool8 DoesSleepClausePrevent(u8 bankToPutToSleep)
{
	if (IsSleepClauseInEffect(bankToPutToSleep))
	{
		u8 i, firstId, lastId;
		struct Pokemon* party = LoadPartyRange(bankToPutToSleep, &firstId, &lastId);

		for (i = 0; i < PARTY_SIZE; ++i)
		{
			u16 species = GetMonData(&party[i], MON_DATA_SPECIES2, NULL);

			if (species != SPECIES_NONE && species != SPECIES_EGG
			&& GetMonData(&party[i], MON_DATA_HP, NULL) > 0
			&& GetMonData(&party[i], MON_DATA_STATUS, NULL) & STATUS_SLEEP) //Someone on team is already asleep
				return TRUE;
		}
	}

	return FALSE;
}

static bool8 CanBeGeneralStatused(u8 bankDef, u8 defAbility, u8 atkAbility, bool8 checkFlowerVeil)
{
	if (!IsTargetAbilityIgnoredNoMove(defAbility, atkAbility)) //Target's Ability is not ignored
	{
		switch (defAbility) {
			case ABILITY_COMATOSE:
				return FALSE;

			case ABILITY_LEAFGUARD:
				if (gBattleWeather & WEATHER_SUN_ANY && WEATHER_HAS_EFFECT && AffectedBySun(bankDef))
					return FALSE;
				break;

			case ABILITY_FLOWERVEIL:
				if (checkFlowerVeil && IsOfType(bankDef, TYPE_GRASS))
					return FALSE;
				break;

			#ifdef SPECIES_MINIOR_SHIELD
			case ABILITY_SHIELDSDOWN:
				if (GetBankPartyData(bankDef)->species == SPECIES_MINIOR_SHIELD) //Prevents Ditto from getting this benefit
					return FALSE;
				break;
			#endif
		}
	}

	if (checkFlowerVeil
	&& IS_DOUBLE_BATTLE
	&& ABILITY(PARTNER(bankDef)) == ABILITY_FLOWERVEIL //Check target partner Flower Veil
	&& IsOfType(bankDef, TYPE_GRASS)
	&& !IsTargetAbilityIgnoredNoMove(ABILITY_FLOWERVEIL, atkAbility)
	&& !(gHitMarker & HITMARKER_IGNORE_SAFEGUARD))
		return FALSE;

	if (gTerrainType == MISTY_TERRAIN && CheckGrounding(bankDef))
		return FALSE;

	if (gBattleMons[bankDef].status1 != STATUS1_NONE)
		return FALSE;

	if (checkFlowerVeil && BankSideHasSafeguard(bankDef) && !BypassesScreens(atkAbility) && !(gHitMarker & HITMARKER_IGNORE_SAFEGUARD))
		return FALSE;

	return TRUE;
}

bool8 CanBePutToSleep(u8 bankDef, u8 bankAtk, bool8 checkFlowerVeil)
{
	u8 atkAbility = ABILITY(bankAtk);
	u8 defAbility = ABILITY(bankDef);

	if (!CanBeGeneralStatused(bankDef, defAbility, atkAbility, checkFlowerVeil))
		return FALSE;

	if (!IsTargetAbilityIgnoredNoMove(defAbility, atkAbility)) //Target's Ability is not ignored
	{
		switch (defAbility) {
			case ABILITY_INSOMNIA:
			#ifdef ABILITY_VITALSPIRIT
			case ABILITY_VITALSPIRIT:
			#endif
			case ABILITY_SWEETVEIL:
				return FALSE;
		}
	}

	if (gTerrainType == ELECTRIC_TERRAIN && IsAffectedByElectricTerrain(bankDef))
		return FALSE;

	if (IS_DOUBLE_BATTLE && ABILITY(PARTNER(bankDef)) == ABILITY_SWEETVEIL && !IsTargetAbilityIgnoredNoMove(ABILITY_SWEETVEIL, atkAbility))
		return FALSE;

	if (DoesSleepClausePrevent(bankDef))
		return FALSE;

	if (IsUproarBeingMade())
		return FALSE;

	return TRUE;
}

bool8 CanBeYawned(u8 bankDef, u8 bankAtk)
{
	if (gTerrainType == ELECTRIC_TERRAIN && IsAffectedByElectricTerrain(bankDef))
		return FALSE;

	if (gBattleMons[bankDef].status1 != STATUS1_NONE)
		return FALSE;

	if (!(gStatuses3[bankDef] & STATUS3_YAWN))
		return FALSE;

	u8 atkAbility = ABILITY(bankAtk);
	if (BankSideHasSafeguard(bankDef) && !BypassesScreens(atkAbility) && !(gHitMarker & HITMARKER_IGNORE_SAFEGUARD))
		return FALSE;

	u8 defAbility = ABILITY(bankDef);
	if (!IsTargetAbilityIgnoredNoMove(defAbility, atkAbility)) //Target's Ability is not ignored
	{
		switch (defAbility) {
			case ABILITY_INSOMNIA:
			#ifdef ABILITY_VITALSPIRIT
			case ABILITY_VITALSPIRIT:
			#endif
			case ABILITY_SWEETVEIL:
			case ABILITY_COMATOSE:
				return FALSE;
			case ABILITY_LEAFGUARD:
				if (gBattleWeather & WEATHER_SUN_ANY && WEATHER_HAS_EFFECT && AffectedBySun(bankDef))
					return FALSE;
				break;
			case ABILITY_FLOWERVEIL:
				if (IsOfType(bankDef, TYPE_GRASS))
					return FALSE;
				break;
			#ifdef SPECIES_MINIOR_SHIELD
			case ABILITY_SHIELDSDOWN:
				if (GetBankPartyData(bankDef)->species == SPECIES_MINIOR_SHIELD) //Prevents Ditto from getting this benefit
					return FALSE;
				break;
			#endif
		}
	}

	if (IS_DOUBLE_BATTLE)
	{
		u8 defPartnerAbility = ABILITY(PARTNER(bankDef));

		if (!IsTargetAbilityIgnoredNoMove(defPartnerAbility, atkAbility)) //Target partner's Ability is not ignored
		{
			switch (defPartnerAbility) {
				case ABILITY_SWEETVEIL:
					return TRUE;
				case ABILITY_FLOWERVEIL:
					if (IsOfType(bankDef, TYPE_GRASS) && !(gHitMarker & HITMARKER_IGNORE_SAFEGUARD))
						return TRUE;
					break;
			}
		}
	}

	return TRUE;
}

bool8 CanRest(u8 bank)
{
	if (BATTLER_MAX_HP(bank))
		return FALSE;

	if (gTerrainType == ELECTRIC_TERRAIN && IsAffectedByElectricTerrain(bank))
		return FALSE;

	if (gTerrainType == MISTY_TERRAIN && CheckGrounding(bank))
		return FALSE;

	if (gBattleMons[bank].status1 & STATUS1_SLEEP)
		return FALSE;

	if (DoesSleepClausePrevent(bank))
		return FALSE;

	if (IsUproarBeingMade())
		return FALSE;

	if (IS_DOUBLE_BATTLE)
	{
		switch (ABILITY(PARTNER(bank))) {
			case ABILITY_SWEETVEIL:
				return TRUE;
			case ABILITY_FLOWERVEIL:
				if (IsOfType(bank, TYPE_GRASS))
					return TRUE;
				break;
		}
	}

	switch (ABILITY(bank)) {
		case ABILITY_INSOMNIA:
		#ifdef ABILITY_VITALSPIRIT
		case ABILITY_VITALSPIRIT:
		#endif
		case ABILITY_SWEETVEIL:
		case ABILITY_COMATOSE:
			return FALSE;
		case ABILITY_LEAFGUARD:
			if (gBattleWeather & WEATHER_SUN_ANY && WEATHER_HAS_EFFECT && AffectedBySun(bank))
				return FALSE;
			break;
		case ABILITY_FLOWERVEIL:
			if (IsOfType(bank, TYPE_GRASS))
				return FALSE;
			break;
		#ifdef SPECIES_MINIOR_SHIELD
		case ABILITY_SHIELDSDOWN:
			if (GetBankPartyData(bank)->species == SPECIES_MINIOR_SHIELD) //Prevents Ditto from getting this benefit
				return FALSE;
			break;
		#endif
	}

	return TRUE;
}

bool8 CanBePoisoned(u8 bankDef, u8 bankAtk, bool8 checkFlowerVeil)
{
	u8 atkAbility = (bankAtk > gBattlersCount) ? 0 : ABILITY(bankAtk); //bankAtk == 0xFF means no attacker - eg. Toxic Spikes
	u8 defAbility = ABILITY(bankDef);

	if (!CanBeGeneralStatused(bankDef, defAbility, atkAbility, checkFlowerVeil))
		return FALSE;

	if (!IsTargetAbilityIgnoredNoMove(defAbility, atkAbility)) //Target's Ability is not ignored
	{
		switch (defAbility) {
			case ABILITY_IMMUNITY:
			case ABILITY_PASTELVEIL:
				return FALSE;
		}
	}

	if (IS_DOUBLE_BATTLE && ABILITY(PARTNER(bankDef)) == ABILITY_PASTELVEIL && !IsTargetAbilityIgnoredNoMove(ABILITY_PASTELVEIL, atkAbility))
		return FALSE;

	if (atkAbility != ABILITY_CORROSION)
	{
		if (IsOfType(bankDef, TYPE_POISON) || IsOfType(bankDef, TYPE_STEEL))
			return FALSE;
	}

	return TRUE;
}

bool8 CanBeParalyzed(u8 bankDef, u8 bankAtk, bool8 checkFlowerVeil)
{
	u8 atkAbility = ABILITY(bankAtk);
	u8 defAbility = ABILITY(bankDef);

	if (!CanBeGeneralStatused(bankDef, defAbility, atkAbility, checkFlowerVeil))
		return FALSE;

	if (IsOfType(bankDef, TYPE_ELECTRIC))
		return FALSE;

	if (!IsTargetAbilityIgnoredNoMove(defAbility, atkAbility)) //Target's Ability is not ignored
	{
		switch (defAbility) {
			case ABILITY_LIMBER:
				return FALSE;
		}
	}

	return TRUE;
}

bool8 CanBeBurned(u8 bankDef, u8 bankAtk, bool8 checkFlowerVeil)
{
	u8 atkAbility = ABILITY(bankAtk);
	u8 defAbility = ABILITY(bankDef);

	if (!CanBeGeneralStatused(bankDef, defAbility, atkAbility, checkFlowerVeil))
		return FALSE;

	if (IsOfType(bankDef, TYPE_FIRE))
		return FALSE;

	#ifdef UNBOUND
	if (SPECIES(bankDef) == SPECIES_SHADOW_WARRIOR)
		return FALSE;
	#endif

	if (!IsTargetAbilityIgnoredNoMove(defAbility, atkAbility)) //Target's Ability is not ignored
	{
		switch (defAbility) {
			case ABILITY_WATERVEIL:
			case ABILITY_WATERBUBBLE:
				return FALSE;
		}
	}

	return TRUE;
}

bool8 CanBeFrozen(u8 bankDef, u8 bankAtk, bool8 checkFlowerVeil)
{
	u8 atkAbility = ABILITY(bankAtk);
	u8 defAbility = ABILITY(bankDef);

	if (!CanBeGeneralStatused(bankDef, defAbility, atkAbility, checkFlowerVeil))
		return FALSE;

	if (IsOfType(bankDef, TYPE_ICE))
		return FALSE;

	#ifdef UNBOUND
	if (SPECIES(bankDef) == SPECIES_SHADOW_WARRIOR)
		return FALSE;
	#endif

	if (!IsTargetAbilityIgnoredNoMove(defAbility, atkAbility)) //Target's Ability is not ignored
	{
		switch (defAbility) {
			case ABILITY_MAGMAARMOR:
				return FALSE;
		}
	}

	if (gBattleWeather & WEATHER_SUN_ANY && WEATHER_HAS_EFFECT && AffectedBySun(bankDef))
		return FALSE;

	return TRUE;
}

bool8 CanBeConfused(u8 bankDef, u8 bankAtk, u8 checkSafeguard)
{
	if (IsConfused(bankDef))
		return FALSE;

	if (gTerrainType == MISTY_TERRAIN && CheckGrounding(bankDef))
		return FALSE;

	u8 atkAbility = ABILITY(bankAtk);
	u8 defAbility = ABILITY(bankDef);
	if (!IsTargetAbilityIgnoredNoMove(defAbility, atkAbility)) //Target's Ability is not ignored
	{
		switch (defAbility) {
			case ABILITY_OWNTEMPO:
				return FALSE;
		}
	}

	if (checkSafeguard && BankSideHasSafeguard(bankDef) && !BypassesScreens(atkAbility) && !(gHitMarker & HITMARKER_IGNORE_SAFEGUARD))
		return FALSE;

	return TRUE;
}

bool8 CanBeTormented(u8 bank)
{
	return !(gBattleMons[bank].status2 & STATUS2_TORMENT) && !IsDynamaxed(bank);
}

bool8 CanBeInfatuated(u8 bankDef, u8 bankAtk)
{
	struct Pokemon *monAttacker, *monTarget;
	u16 speciesAttacker, speciesTarget;
	u32 personalityAttacker, personalityTarget;

	monAttacker = GetBankPartyData(bankAtk);
	monTarget = GetBankPartyData(bankDef);

	speciesAttacker = monAttacker->species;
	personalityAttacker = monAttacker->personality;

	speciesTarget = monTarget->species;
	personalityTarget = monTarget->personality;

	return BATTLER_ALIVE(bankDef)
		&& !(gBattleMons[bankDef].status2 & STATUS2_INFATUATION)
		&& (ABILITY(bankDef) != ABILITY_OBLIVIOUS || IsTargetAbilityIgnoredNoMove(ABILITY_OBLIVIOUS, ABILITY(bankAtk)))
		&& GetGenderFromSpeciesAndPersonality(speciesAttacker, personalityAttacker) != GetGenderFromSpeciesAndPersonality(speciesTarget, personalityTarget)
		&& GetGenderFromSpeciesAndPersonality(speciesAttacker, personalityAttacker) != MON_GENDERLESS
		&& GetGenderFromSpeciesAndPersonality(speciesTarget, personalityTarget) != MON_GENDERLESS
		&& !AbilityBattleEffects(ABILITYEFFECT_CHECK_BANK_SIDE, bankDef, ABILITY_AROMAVEIL, 0, 0);
}

bool8 CanFlinch(u8 bank, u8 ability)
{
	return ability != ABILITY_INNERFOCUS && !IsDynamaxed(bank);
}

bool8 CanBeChoiceLocked(u8 bank)
{
	return IsChoiceItemEffectOrAbility(ITEM_EFFECT(bank), ABILITY(bank));
}

bool8 IsTrickRoomActive(void)
{
	return gNewBS->TrickRoomTimer > 0
		#ifdef FLAG_TRICK_ROOM_BATTLE
		|| FlagGet(FLAG_TRICK_ROOM_BATTLE)
		#endif
		|| (IS_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_TRICK_ROOM);
}

bool8 IsTrickRoomOnLastTurn(void)
{
	if ((IS_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_TRICK_ROOM)
	#ifdef FLAG_TRICK_ROOM_BATTLE
	|| FlagGet(FLAG_TRICK_ROOM_BATTLE)
	#endif
	)
		return FALSE;

	return gNewBS->TrickRoomTimer == 1;
}

bool8 IsMagicRoomActive(void)
{
	return gNewBS->MagicRoomTimer > 0
		|| (IS_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_MAGIC_ROOM);
}

bool8 IsWonderRoomActive(void)
{
	return gNewBS->WonderRoomTimer > 0
		|| (IS_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_WONDER_ROOM);
}

bool8 IsGravityActive(void)
{
	return gNewBS->GravityTimer > 0
		|| (IS_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_GRAVITY);
}

bool8 IsIonDelugeActive(void)
{
	return gNewBS->IonDelugeTimer > 0;
}

bool8 IsFairyLockActive(void)
{
	return gNewBS->FairyLockTimer > 0
		|| (IS_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_FAIRY_LOCK);
}

bool8 IsMudSportActive(void)
{
	return gNewBS->MudSportTimer > 0;
}

bool8 IsWaterSportActive(void)
{
	return gNewBS->WaterSportTimer > 0;
}

bool8 IsDeltaStreamBattle(void)
{
	return 
		#ifdef FLAG_DELTA_STREAM_BATTLE
		FlagGet(FLAG_DELTA_STREAM_BATTLE) ||
		#endif
		(IS_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_DELTA_STREAM);
}

bool8 IsMagnetRiseBattle(void)
{
	return 
		#ifdef FLAG_MAGNET_RISE_BATTLE
		FlagGet(FLAG_MAGNET_RISE_BATTLE) ||
		#endif
		(IS_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_MAGNET_RISE);
}

bool8 IsBadThoughtsBattle(void)
{
	return
		#ifdef FLAG_BAD_THOUGHTS_BATTLE
		FlagGet(FLAG_BAD_THOUGHTS_BATTLE) ||
		#endif
		(IS_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_BAD_THOUGHTS);
}

bool8 IsShadowShieldBattle(void)
{
	#ifdef FLAG_SHADOW_SHIELD_BATTLE
	return FlagGet(FLAG_SHADOW_SHIELD_BATTLE);
	#else
	return FALSE;
	#endif
}

bool8 IsPixieBattle(void)
{
	return
		#ifdef FLAG_PIXIE_BATTLE
		FlagGet(FLAG_PIXIE_BATTLE) ||
		#endif
		(IS_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_PIXIES);
}

bool8 IsInverseBattle(void)
{
	return
		#ifdef FLAG_INVERSE
		FlagGet(FLAG_INVERSE) ||
		#endif
		(IS_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_INVERSE);
}

bool8 BankSideHasSafeguard(u8 bank)
{
	return gSideStatuses[SIDE(bank)] & SIDE_STATUS_SAFEGUARD
		|| (IS_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_SAFEGUARD);
}

bool8 BankSideHasMist(u8 bank)
{
	u8 side = SIDE(bank);

	return gSideStatuses[side] & SIDE_STATUS_MIST
		|| gSideTimers[side].mistTimer > 0 //Guard Spec
		|| (IS_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_MIST);
}

bool8 BankHasTailwind(u8 bank)
{
	return BankSideHasTailwind(bank)
		#ifdef FLAG_TAILWIND_BATTLE
		|| (FlagGet(FLAG_TAILWIND_BATTLE) && IsOfType(bank, TYPE_FLYING))
		#endif
		;
}

bool8 BankSideHasTailwind(u8 bank)
{
	return gNewBS->TailwindTimers[SIDE(bank)];
}

bool8 MonHasTailwind(unusedArg struct Pokemon* mon, u8 side)
{
	return gNewBS->TailwindTimers[side]
		#ifdef FLAG_TAILWIND_BATTLE
		|| (FlagGet(FLAG_TAILWIND_BATTLE) && IsMonOfType(mon, TYPE_FLYING))
		#endif
		;
}

bool8 BankSideHasSeaOfFire(u8 bank)
{
	return gNewBS->SeaOfFireTimers[SIDE(bank)]
		|| (IS_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_SEA_OF_FIRE);
}

bool8 BankHasRainbow(u8 bank)
{
	return BankSideHasRainbow(bank)
		#ifdef FLAG_RAINBOW_BATTLE
		|| (FlagGet(FLAG_RAINBOW_BATTLE) && IsOfType(bank, TYPE_DRAGON))
		#endif
		;
}

bool8 BankSideHasRainbow(u8 bank)
{
	return gNewBS->RainbowTimers[SIDE(bank)]
		|| (IS_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_RAINBOW);
}

bool8 BankSideHasSwamp(u8 bank)
{
	return SideHasSwamp(SIDE(bank));
}

bool8 SideHasSwamp(u8 side)
{
	return gNewBS->SwampTimers[side];
}

bool8 BankSideHasGMaxVineLash(u8 bank)
{
	return gNewBS->maxVineLashTimers[SIDE(bank)] > 0;
}

bool8 BankSideHasGMaxWildfire(u8 bank)
{
	return gNewBS->maxWildfireTimers[SIDE(bank)] > 0;
}

bool8 BankSideHasGMaxCannonade(u8 bank)
{
	return gNewBS->maxCannonadeTimers[SIDE(bank)] > 0;
}

bool8 BankSideHasGMaxVolcalith(u8 bank)
{
	return gNewBS->maxVolcalithTimers[SIDE(bank)] > 0;
}

bool8 IsConfused(u8 bank)
{
	return (gBattleMons[bank].status2 & STATUS2_CONFUSION) != 0
		|| (IS_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_CONFUSED && ABILITY(bank) != ABILITY_OWNTEMPO);
}

bool8 IsTaunted(u8 bank)
{
	return gDisableStructs[bank].tauntTimer > 0
		|| (IS_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_TAUNT && ABILITY(bank) != ABILITY_OBLIVIOUS);
}

bool8 IsTormented(u8 bank)
{
	return (gBattleMons[bank].status2 & STATUS2_TORMENT) != 0
		|| (IS_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_TORMENT);
}

bool8 IsHealBlocked(u8 bank)
{
	return gNewBS->HealBlockTimers[bank] > 0
		|| (IS_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_HEAL_BLOCK);
}

bool8 CantUseSoundMoves(u8 bank)
{
	return gNewBS->ThroatChopTimers[bank] > 0
		|| (IS_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_THROAT_CHOP);
}

bool8 IsLaserFocused(u8 bank)
{
	return gNewBS->LaserFocusTimers[bank] > 0
		|| (IS_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_ALWAYS_CRIT);
}

bool8 IsAbilitySuppressed(u8 bank)
{
	return (gStatuses3[bank] & STATUS3_ABILITY_SUPPRESS) != 0
		|| AreAbilitiesSuppressed();
}

bool8 AreAbilitiesSuppressed(void)
{
	return IS_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_ABILITY_SUPPRESSION;
}

bool8 CantScoreACrit(u8 bank, struct Pokemon* mon)
{
	if (mon != NULL)
		return FALSE;

	return (gStatuses3[bank] & STATUS3_CANT_SCORE_A_CRIT) != 0
		|| (IS_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_NO_CRITS);
}

void ClearTemporarySpeciesSpriteData(u8 bank, bool8 dontClearSubstitute)
{
	if (!gDontRemoveTransformSpecies) //For Dynamaxing a transformed Pokemon
	{
		gBattleSpritesDataPtr->bankData[bank].transformSpecies = SPECIES_NONE;
		gBattleMonForms[bank] = 0;
	}

	if (!dontClearSubstitute)
		ClearBehindSubstituteBit(bank);
}

u16 TryFixDynamaxTransformSpecies(u8 bank, u16 species)
{
	if (gDontRemoveTransformSpecies && gBattleSpritesDataPtr->bankData[bank].transformSpecies != SPECIES_NONE)
		species = gBattleSpritesDataPtr->bankData[bank].transformSpecies;

	return species;
}
