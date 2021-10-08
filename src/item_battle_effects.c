#include "defines.h"
#include "defines_battle.h"
#include "../include/battle_anim.h"
#include "../include/battle_string_ids.h"
#include "../include/random.h"
#include "../include/string_util.h"
#include "../include/constants/items.h"

#include "../include/new/battle_util.h"
#include "../include/new/util.h"
#include "../include/new/item.h"
#include "../include/new/item_battle_effects.h"
#include "../include/new/item_battle_scripts.h"
#include "../include/new/move_tables.h"
#include "../include/new/stat_buffs.h"
/*
item_battle_effects.c
	handles functions that deal with battle effects of specific items
*/

#define BattleScript_ItemHealHP_Ret (u8*) 0x81D9622
#define BattleScript_ItemHealHP_End2 (u8*) 0x81D961C
#define BattleScript_WhiteHerbRet (u8*) 0x81D95DF
#define BattleScript_WhiteHerbEnd2 (u8*) 0x81D95D9

extern u8 gStatusConditionString_DisableProblem[];
extern u8 gStatusConditionString_EncoreProblem[];
extern u8 gStatusConditionString_MentalState[];
extern u8 gStatusConditionString_TauntProblem[];

enum
{
	ITEM_NO_EFFECT, // 0
	ITEM_STATUS_CHANGE, // 1
	ITEM_EFFECT_OTHER, // 2
	ITEM_PP_CHANGE, // 3
	ITEM_HP_CHANGE, // 4
	ITEM_STATS_CHANGE, // 5
};

enum
{
	FLAVOR_SPICY, // 0
	FLAVOR_DRY, // 1
	FLAVOR_SWEET, // 2
	FLAVOR_BITTER, // 3
	FLAVOR_SOUR, // 4
};

//This file's functions:
static u8 ConfusionBerries(u8 bank, u8 flavour, bool8 moveTurn, bool8 doPluck);
static u8 StatRaiseBerries(u8 bank, u8 stat, bool8 moveTurn, bool8 doPluck);
static u8 RaiseStatsContactItem(u8 bank, u8 stat, bool8 doPluck);
static u8 KeeMaranagaBerryFunc(u8 bank, u8 stat, u8 split, bool8 doPluck);

u8 ItemBattleEffects(u8 caseID, u8 bank, bool8 moveTurn, bool8 doPluck)
{
	int i = 0;
	u8 effect = ITEM_NO_EFFECT;
	u8 changedPP = 0;
	u8 bankHoldEffect, atkHoldEffect;
	u8 bankQuality, atkQuality;
	u16 atkItem;

	#ifndef NO_GHOST_BATTLES
	if (IS_GHOST_BATTLE && SIDE(bank) == B_SIDE_OPPONENT)
		return 0; //Ghost's items don't activate
	#endif

	if (doPluck)
	{
		bankHoldEffect = ItemId_GetHoldEffect(gLastUsedItem);
		bankQuality = ItemId_GetHoldEffectParam(gLastUsedItem);
	}
	else
	{
		gLastUsedItem = ITEM(bank);

		if (IsBerry(gLastUsedItem) && UnnerveOnOpposingField(bank))
		{
			bankHoldEffect = 0;
			bankQuality = 0;
		}
		else
		{
			bankHoldEffect = ITEM_EFFECT(bank);
			bankQuality = ITEM_QUALITY(bank);
		}
	}

	atkItem = ITEM(gBankAttacker);
	atkHoldEffect = ItemId_GetHoldEffect(atkItem);
	atkQuality = ItemId_GetHoldEffectParam(atkItem);

/*	// def variables are unused
	defItem = gBattleMons[gBankTarget].item;
	defHoldEffect = ItemId_GetHoldEffect(defItem);
	defQuality = ItemId_GetHoldEffectParam(defItem);
*/

	switch (caseID) {
	case ItemEffects_SwitchIn:
		switch (bankHoldEffect) {
			case ITEM_EFFECT_DOUBLE_PRIZE:
				if (!gNewBS->usedAmuletCoin && SIDE(bank) == B_SIDE_PLAYER)
				{
					gNewBS->usedAmuletCoin = TRUE;
	
					#ifdef VAR_AMULET_COIN_LEVEL
					gBattleStruct->moneyMultiplier *= max(2, VarGet(VAR_AMULET_COIN_LEVEL));
					#else
					gBattleStruct->moneyMultiplier *= 2;
					#endif
				}
				break;

			case ITEM_EFFECT_RESTORE_STATS:
				for (i = 0; i < BATTLE_STATS_NO-1; i++) {
					if (gBattleMons[bank].statStages[i] < 6) {
						gBattleMons[bank].statStages[i] = 6;
						effect = ITEM_STATS_CHANGE;
					}
				}

				if (effect) {
					gBattleScripting.bank = bank;
					gStringBank = bank;
					gActiveBattler = gBankAttacker = bank;

					if (moveTurn)
					{
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_WhiteHerbRet;
					}
					else
						BattleScriptExecute(BattleScript_WhiteHerbEnd2);
				}
				break;

			case ITEM_EFFECT_SEEDS: ;
				u8 stat = 0;

				if (bankQuality && bankQuality == gTerrainType)
				{
					switch (gTerrainType) {
						case ELECTRIC_TERRAIN:
						case GRASSY_TERRAIN:
							if (STAT_CAN_RISE(bank, STAT_STAGE_DEF))
							{
								stat = STAT_STAGE_DEF;
								effect = ITEM_STATS_CHANGE;
							}
							break;
						case MISTY_TERRAIN:
						case PSYCHIC_TERRAIN:
							if (STAT_CAN_RISE(bank, STAT_STAGE_SPDEF))
							{
								stat = STAT_STAGE_SPDEF;
								effect = ITEM_STATS_CHANGE;
							}
							break;
					}

					if (effect)
					{
						PREPARE_STAT_BUFFER(gBattleTextBuff1, stat);
						gBattleScripting.statChanger = INCREASE_1 | stat;
						gBattleScripting.animArg1 = 0xE + stat;
						gBattleScripting.animArg2 = 0;
						gBattleScripting.bank = gBankTarget = bank;

						if (moveTurn)
						{
							BattleScriptPushCursor();
							gBattlescriptCurrInstr = BattleScript_ItemStatChangeRet;
						}
						else
							BattleScriptExecute(BattleScript_ItemStatChangeEnd2);
					}
				}
				break;

			case ITEM_EFFECT_EJECT_PACK:
				if (gNewBS->statFellThisTurn[bank])
				{
					gNewBS->statFellThisTurn[bank] = FALSE;
					gNewBS->NoSymbiosisByte = TRUE;
					gActiveBattler = gBattleScripting.bank = gBankSwitching = bank;
					effect = ITEM_STATS_CHANGE;

					if (moveTurn)
					{
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_EjectPackRet;
					}
					else
						BattleScriptExecute(BattleScript_EjectPackEnd2);
				}
				break;

			case ITEM_EFFECT_ROOM_SERVICE:
				if (IsTrickRoomActive() && STAT_CAN_FALL(bank, STAT_STAGE_SPEED))
				{
					PREPARE_STAT_BUFFER(gBattleTextBuff1, STAT_STAGE_SPEED);
					gBattleScripting.statChanger = DECREASE_1 | STAT_STAGE_SPEED;
					gBattleScripting.animArg1 = STAT_ANIM_MINUS1 + STAT_STAGE_SPEED - 1;
					gBattleScripting.animArg2 = 0;
					gBattleScripting.bank = gBankTarget = bank;
					effect = ITEM_STATS_CHANGE;

					if (moveTurn)
					{
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_ItemStatChangeRet;
					}
					else
						BattleScriptExecute(BattleScript_ItemStatChangeEnd2);
				}
				break;
		}
		break;

	case ItemEffects_EndTurn:
		gBattleScripting.bank = bank;

		if (BATTLER_ALIVE(bank))
		{
			switch (bankHoldEffect) {
			case ITEM_EFFECT_RESTORE_HP:
			
				if (!IsHealBlocked(bank)
				&& ((gBattleMons[bank].hp <= gBattleMons[bank].maxHP / 2) || (doPluck && !BATTLER_MAX_HP(bank))))
				{
					if (gLastUsedItem == ITEM_SITRUS_BERRY)
						gBattleMoveDamage = GetBaseMaxHP(bank) / 4;
					else
						gBattleMoveDamage = bankQuality;

					if (ABILITY(bank) == ABILITY_RIPEN && IsBerry(gLastUsedItem))
						gBattleMoveDamage *= 2;

					if (gBattleMons[bank].hp + gBattleMoveDamage > gBattleMons[bank].maxHP)
						gBattleMoveDamage = gBattleMons[bank].maxHP - gBattleMons[bank].hp;

					gBattleMoveDamage *= -1;
					if (moveTurn || doPluck)
					{
						BattleScriptPushCursor();
						if (IsBerry(gLastUsedItem))
							gBattlescriptCurrInstr = BattleScript_BerryHealHP_RemoveBerryRet;
						else
							gBattlescriptCurrInstr = BattleScript_ItemHealHP_RemoveItemRet; //Berry Juice
					}
					else
					{
						if (IsBerry(gLastUsedItem))
							BattleScriptExecute(BattleScript_BerryHealHP_RemoveBerryEnd2);
						else
							BattleScriptExecute(BattleScript_ItemHealHP_RemoveItemEnd2);
					}
					effect = ITEM_HP_CHANGE;
				}
				break;

			case ITEM_EFFECT_RESTORE_PP:
				if (!moveTurn || doPluck)
				{
					u8 ppBonuses, maxPP;
					u16 move;
					pokemon_t* poke = GetBankPartyData(bank);
					u8 restoreAmount = bankQuality;

					if (ABILITY(bank) == ABILITY_RIPEN)
						restoreAmount *= 2;

					for (i = 0; i < MAX_MON_MOVES; ++i)
					{
						move = GetMonData(poke, MON_DATA_MOVE1 + i, 0);
						changedPP = GetMonData(poke, MON_DATA_PP1 + i, 0);
						ppBonuses = GetMonData(poke, MON_DATA_PP_BONUSES, 0);

						if (doPluck)
						{
							maxPP = CalculatePPWithBonus(move, ppBonuses, i);
							if (IS_TRANSFORMED(bank) && maxPP > 5)
								maxPP = 5; //Can't restore past 5 PP if transformed

							if (changedPP < maxPP)
								break;
						}
						else if (move && changedPP == 0)
							break;
					}

					if (i < MAX_MON_MOVES)
					{
						maxPP = CalculatePPWithBonus(move, ppBonuses, i);
						if (IS_TRANSFORMED(bank) && maxPP > 5)
							maxPP = 5; //Can't restore past 5 PP if transformed

						if (changedPP + restoreAmount > maxPP)
							changedPP = maxPP;
						else
							changedPP = changedPP + restoreAmount;

						PREPARE_MOVE_BUFFER(gBattleTextBuff1, move);

						if (doPluck)
						{
							BattleScriptPushCursor();
							gBattlescriptCurrInstr = BattleScript_BerryPPHealRet;
						}
						else
							BattleScriptExecute(BattleScript_BerryPPHealEnd2);


						gActiveBattler = bank;
						EmitSetMonData(0, i + REQUEST_PPMOVE1_BATTLE, 0, 1, &changedPP);
						MarkBufferBankForExecution(gActiveBattler);
						effect = ITEM_PP_CHANGE;
					}
				}
				break;

			case ITEM_EFFECT_RESTORE_STATS:
				for (i = 0; i < BATTLE_STATS_NO-1; ++i)
				{
					if (gBattleMons[bank].statStages[i] < 6)
					{
						gBattleMons[bank].statStages[i] = 6;
						effect = ITEM_STATS_CHANGE;
					}
				}

				if (effect)
				{
					gBattleScripting.bank = gActiveBattler = gStringBank = bank;
					if (moveTurn)
					{
						BattleScriptPushCursor();
						if (doPluck) //Fling
							gBattlescriptCurrInstr = BattleScript_WhiteHerbFling;
						else
							gBattlescriptCurrInstr = BattleScript_WhiteHerbRet;
					}
					else
						BattleScriptExecute(BattleScript_WhiteHerbEnd2);
				}
				break;

			case ITEM_EFFECT_LEFTOVERS:
			LEFTOVERS_HEAL:
				if (!moveTurn && !BATTLER_MAX_HP(bank)
				&&  !IsHealBlocked(bank)
				&&  !gNewBS->leftoverHealingDone[bank])
				{
					gBattleMoveDamage = MathMax(1, GetBaseMaxHP(bank) / 16);
					if (gBattleMons[bank].hp + gBattleMoveDamage > gBattleMons[bank].maxHP)
						gBattleMoveDamage = gBattleMons[bank].maxHP - gBattleMons[bank].hp;
					gBattleMoveDamage *= -1;
					BattleScriptExecute(BattleScript_ItemHealHP_End2);
					effect = ITEM_HP_CHANGE;
					RecordItemEffectBattle(bank, bankHoldEffect);
					gNewBS->leftoverHealingDone[bank] = TRUE;
				}
				break;

			case ITEM_EFFECT_CONFUSE_SPICY:
				effect = ConfusionBerries(bank, FLAVOR_SPICY, moveTurn, doPluck);
				break;

			case ITEM_EFFECT_CONFUSE_DRY:
				effect = ConfusionBerries(bank, FLAVOR_DRY, moveTurn, doPluck);
				break;

			case ITEM_EFFECT_CONFUSE_SWEET:
				effect = ConfusionBerries(bank, FLAVOR_SWEET, moveTurn, doPluck);
				break;

			case ITEM_EFFECT_CONFUSE_BITTER:
				effect = ConfusionBerries(bank, FLAVOR_BITTER, moveTurn, doPluck);
				break;

			case ITEM_EFFECT_CONFUSE_SOUR:
				effect = ConfusionBerries(bank, FLAVOR_SOUR, moveTurn, doPluck);
				break;

			case ITEM_EFFECT_ATTACK_UP:
				effect = StatRaiseBerries(bank, STAT_STAGE_ATK, moveTurn, doPluck);
				break;

			case ITEM_EFFECT_DEFENSE_UP:
				effect = StatRaiseBerries(bank, STAT_STAGE_DEF, moveTurn, doPluck);
				break;

			case ITEM_EFFECT_SPEED_UP:
				effect = StatRaiseBerries(bank, STAT_STAGE_SPEED, moveTurn, doPluck);
				break;

			case ITEM_EFFECT_SP_ATTACK_UP:
				effect = StatRaiseBerries(bank, STAT_STAGE_SPATK, moveTurn, doPluck);
				break;

			case ITEM_EFFECT_SP_DEFENSE_UP:
				effect = StatRaiseBerries(bank, STAT_STAGE_SPDEF, moveTurn, doPluck);
				break;

			case ITEM_EFFECT_CRITICAL_UP:
				if ((PINCH_BERRY_CHECK(bank) || doPluck)
				&& !(gBattleMons[bank].status2 & STATUS2_FOCUS_ENERGY))
				{
					gBattleMons[bank].status2 |= STATUS2_FOCUS_ENERGY;
					if (moveTurn || doPluck)
					{
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_BerryFocusEnergyRet;
					}
					else
						BattleScriptExecute(BattleScript_BerryFocusEnergyEnd2);
					effect = ITEM_EFFECT_OTHER;
				}
				break;

			case ITEM_EFFECT_RANDOM_STAT_UP:
				if ((PINCH_BERRY_CHECK(bank) || doPluck)
				&& !((MainStatsMaxed(bank) && ABILITY(bank) != ABILITY_CONTRARY)
				  || (MainStatsMinned(bank) && ABILITY(bank) == ABILITY_CONTRARY)))
				{
					u8 buff = (ABILITY(bank) == ABILITY_RIPEN) ? 4 : 2;

					do
					{
						i = RandRange(STAT_STAGE_ATK, NUM_STATS);
					} while (STAT_STAGE(bank, i) >= STAT_STAGE_MAX);

					if (!ChangeStatBuffs(SET_STAT_BUFF_VALUE(buff), i, MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_CERTAIN, 0))
					{
						PREPARE_STAT_BUFFER(gBattleTextBuff1, i);

						gBattleTextBuff2[0] = B_BUFF_PLACEHOLDER_BEGIN;
						gBattleTextBuff2[1] = B_BUFF_STRING;
						gBattleTextBuff2[2] = STRINGID_STATSHARPLY;
						gBattleTextBuff2[3] = STRINGID_STATSHARPLY >> 8;
						gBattleTextBuff2[4] = B_BUFF_STRING;
						gBattleTextBuff2[5] = STRINGID_STATROSE;
						gBattleTextBuff2[6] = STRINGID_STATROSE >> 8;
						gBattleTextBuff2[7] = EOS;

						gEffectBank = bank;
						SET_STATCHANGER(i, buff, FALSE);
						gBattleScripting.animArg1 = STAT_ANIM_PLUS2 + (i - 1);
						gBattleScripting.animArg2 = 0;

						if (moveTurn || doPluck)
						{
							BattleScriptPushCursor();
							gBattlescriptCurrInstr = BattleScript_BerryStatRaiseRet;
						}
						else
							BattleScriptExecute(BattleScript_BerryStatRaiseEnd2);

						effect = ITEM_STATS_CHANGE;
					}
				}
				break;

			case ITEM_EFFECT_CURE_PAR:
				if (gBattleMons[bank].status1 & STATUS_PARALYSIS)
				{
					gBattleMons[bank].status1 &= ~(STATUS_PARALYSIS);
					if (moveTurn || doPluck) {
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_BerryCureParRet;
					}
					else
						BattleScriptExecute(BattleScript_BerryCurePrlzEnd2);
					effect = ITEM_STATUS_CHANGE;
				}
				break;

			case ITEM_EFFECT_CURE_PSN:
				if (gBattleMons[bank].status1 & STATUS_PSN_ANY)
				{
					gBattleMons[bank].status1 &= ~(STATUS_PSN_ANY | STATUS_TOXIC_COUNTER);
					if (moveTurn || doPluck) {
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_BerryCurePsnRet;
					}
					else
						BattleScriptExecute(BattleScript_BerryCurePsnEnd2);
					effect = ITEM_STATUS_CHANGE;
				}
				break;

			case ITEM_EFFECT_CURE_BRN:
				if (gBattleMons[bank].status1 & STATUS_BURN)
				{
					gBattleMons[bank].status1 &= ~(STATUS_BURN);
					if (moveTurn || doPluck) {
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_BerryCureBrnRet;
					}
					else
						BattleScriptExecute(BattleScript_BerryCureBrnEnd2);
					effect = ITEM_STATUS_CHANGE;
				}
				break;

			case ITEM_EFFECT_CURE_FRZ:
				if (gBattleMons[bank].status1 & STATUS_FREEZE)
				{
					gBattleMons[bank].status1 &= ~(STATUS_FREEZE);
					if (moveTurn || doPluck) {
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_BerryCureFrzRet;
					}
					else
						BattleScriptExecute(BattleScript_BerryCureFrzEnd2);
					effect = ITEM_STATUS_CHANGE;
				}
				break;

			case ITEM_EFFECT_CURE_SLP:
				if (gBattleMons[bank].status1 & STATUS_SLEEP)
				{
					gBattleMons[bank].status1 &= ~(STATUS_SLEEP);
					gBattleMons[bank].status2 &= ~(STATUS2_NIGHTMARE);
					if (moveTurn || doPluck) {
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_BerryCureSlpRet;
					}
					else
						BattleScriptExecute(BattleScript_BerryCureSlpEnd2);
					effect = ITEM_STATUS_CHANGE;
				}
				break;

			case ITEM_EFFECT_CURE_CONFUSION:
				if (gBattleMons[bank].status2 & STATUS2_CONFUSION)
				{
					gBattleMons[bank].status2 &= ~(STATUS2_CONFUSION);
					if (moveTurn || doPluck) {
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_BerryCureConfusionRet;
					}
					else
						BattleScriptExecute(BattleScript_BerryCureConfusionEnd2);
					effect = ITEM_EFFECT_OTHER;
				}
				break;

			case ITEM_EFFECT_CURE_STATUS:
				if (gBattleMons[bank].status1 & STATUS_ANY || gBattleMons[bank].status2 & STATUS2_CONFUSION) {
					i = 0;
					if (gBattleMons[bank].status1 & STATUS_PSN_ANY) {
						StringCopy(gBattleTextBuff1, gStatusConditionString_Poison);
						i++;
					}

					if (gBattleMons[bank].status1 & STATUS_SLEEP) {
						gBattleMons[bank].status2 &= ~(STATUS2_NIGHTMARE);
						StringCopy(gBattleTextBuff1, gStatusConditionString_Sleep);
						i++;
					}

					if (gBattleMons[bank].status1 & STATUS_PARALYSIS) {
						StringCopy(gBattleTextBuff1, gStatusConditionString_Paralysis);
						i++;
					}

					if (gBattleMons[bank].status1 & STATUS_BURN) {
						StringCopy(gBattleTextBuff1, gStatusConditionString_Burn);
						i++;
					}

					if (gBattleMons[bank].status1 & STATUS_FREEZE) {
						StringCopy(gBattleTextBuff1, gStatusConditionString_Ice);
						i++;
					}

					if (gBattleMons[bank].status2 & STATUS2_CONFUSION) {
						StringCopy(gBattleTextBuff1, gStatusConditionString_Confusion);
						i++;
					}

					if (i < 2)
						gBattleCommunication[MULTISTRING_CHOOSER] = 0;
					else
						gBattleCommunication[MULTISTRING_CHOOSER] = 1;
					gBattleMons[bank].status1 = 0;
					gBattleMons[bank].status2 &= ~(STATUS2_CONFUSION);
					if (moveTurn || doPluck) {
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_BerryCureChosenStatusRet;
					}
					else
						BattleScriptExecute(BattleScript_BerryCureChosenStatusEnd2);
					effect = ITEM_STATUS_CHANGE;
				}
				break;

			//Find out what strings displays in S/M
			case ITEM_EFFECT_CURE_ATTRACT:
				i = 0;
				if (gBattleMons[bank].status2 & STATUS2_INFATUATION)
				{
					gBattleMons[bank].status2 &= ~(STATUS2_INFATUATION);
					StringCopy(gBattleTextBuff1, gStatusConditionString_Love);
					++i;
				}
				if (gDisableStructs[bank].disabledMove)
				{
					gDisableStructs[bank].disabledMove = 0;
					gDisableStructs[bank].disableTimer1 = 0;
					StringCopy(gBattleTextBuff1, gStatusConditionString_DisableProblem);
					++i;
				}
				if (gDisableStructs[bank].encoredMove)
				{
					gDisableStructs[bank].encoredMove = 0; //Same as end turn clear
					gDisableStructs[bank].encoreTimer = 0;
					StringCopy(gBattleTextBuff1, gStatusConditionString_EncoreProblem);
					++i;
				}
				if (gDisableStructs[bank].tauntTimer > 0)
				{
					gDisableStructs[bank].tauntTimer = 0;
					StringCopy(gBattleTextBuff1, gStatusConditionString_TauntProblem);
					++i;
				}
				if (i == 0)
					break;

				else if (i >= 2)
					StringCopy(gBattleTextBuff1, gStatusConditionString_MentalState);

				if (moveTurn)
				{
					BattleScriptPushCursor();

					if (doPluck) //Fling
						gBattlescriptCurrInstr = BattleScript_HerbCureChosenStatusFling;
					else
						gBattlescriptCurrInstr = BattleScript_HerbCureChosenStatusRet;
				}
				else
					BattleScriptExecute(BattleScript_HerbCureChosenStatusEnd2);

				gBattleCommunication[MULTISTRING_CHOOSER] = 0;
				effect = ITEM_EFFECT_OTHER;
				break;


			case ITEM_EFFECT_MICLE_BERRY:
				if (PINCH_BERRY_CHECK(bank) || doPluck)
				{
					gNewBS->MicleBerryBits |= (1 << bank);

					if (moveTurn || doPluck) {
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_MicleBerryRet;
					}
					else
						BattleScriptExecute(BattleScript_MicleBerryEnd2);
					effect = ITEM_EFFECT_OTHER;
				}
				break;

			case ITEM_EFFECT_BLACK_SLUDGE:
				if (!moveTurn)
				{
					if (IsOfType(bank, TYPE_POISON))
						goto LEFTOVERS_HEAL;
					else if (!moveTurn && ABILITY(bank) != ABILITY_MAGICGUARD
					&& !gNewBS->leftoverHealingDone[bank])
					{
						gBattleMoveDamage = MathMax(1, GetBaseMaxHP(bank) / 8);
						BattleScriptExecute(BattleScript_BlackSludgeHurt);
						effect = ITEM_EFFECT_OTHER;
						gNewBS->leftoverHealingDone[bank] = TRUE;
					}
				}
			}

			if (effect)
			{
				if (GetPocketByItemId(gLastUsedItem) == POCKET_BERRY_POUCH)
					gNewBS->canBelch[SIDE(bank)] |= gBitTable[gBattlerPartyIndexes[bank]];

				gBattleScripting.bank = bank;
				gStringBank = bank;
				gActiveBattler = bank;
				switch (effect) {
					case ITEM_STATUS_CHANGE:
						EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[bank].status1);
						MarkBufferBankForExecution(gActiveBattler);
						break;
					case ITEM_PP_CHANGE:
						if (!(gBattleMons[bank].status2 & STATUS2_TRANSFORMED) && !(gDisableStructs[bank].mimickedMoves & gBitTable[i]))
							gBattleMons[bank].pp[i] = changedPP;
						break;
					case ITEM_HP_CHANGE:
						EmitSetMonData(0, REQUEST_HP_BATTLE, 0, 2, &gBattleMons[bank].hp);
						MarkBufferBankForExecution(gActiveBattler);
						break;
				}
			}
		}
		break;

	case ItemEffects_ContactTarget:
		if (doPluck)
			gBattleScripting.bank = bank;
		else
			gBattleScripting.bank = bank = gBankTarget;

		switch (bankHoldEffect) {
			case ITEM_EFFECT_ROCKY_HELMET:
				if (TOOK_DAMAGE(bank)
				&& MOVE_HAD_EFFECT
				&& ABILITY(gBankAttacker) != ABILITY_MAGICGUARD
				&& CheckContact(gCurrentMove, gBankAttacker, bank)
				&& !MoveBlockedBySubstitute(gCurrentMove, gBankAttacker, bank)
				&& gBattleMons[gBankAttacker].hp)
				{
					gBattleMoveDamage = MathMax(1, GetBaseMaxHP(gBankAttacker) / 6);
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_RockyHelmetDamage;
					RecordItemEffectBattle(bank, bankHoldEffect);
					gActiveBattler = gBankAttacker;
					effect = ITEM_HP_CHANGE;
				}
				break;

			case ITEM_EFFECT_WEAKNESS_POLICY:
				if (TOOK_DAMAGE(bank)
				&& gMoveResultFlags == MOVE_RESULT_SUPER_EFFECTIVE
				&& gBattleMons[bank].hp
				&& (STAT_CAN_RISE(bank, STAT_STAGE_ATK) || STAT_CAN_RISE(bank, STAT_STAGE_SPATK))
				&& !MoveBlockedBySubstitute(gCurrentMove, gBankAttacker, bank))
				{
					gEffectBank = bank;
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_WeaknessPolicy;
					gActiveBattler = gBankTarget;
					effect = ITEM_STATS_CHANGE;
				}
				break;

			case ITEM_EFFECT_ABSORB_BULB:
				effect = RaiseStatsContactItem(bank, STAT_STAGE_SPATK, TYPE_WATER);
				break;

			case ITEM_EFFECT_AIR_BALLOON:
				if (TOOK_DAMAGE(bank) && MOVE_HAD_EFFECT)
				{
					gEffectBank = bank;
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_AirBallooonPop;
					gActiveBattler = bank;
					effect = ITEM_EFFECT_OTHER;
				}
				break;

			case ITEM_EFFECT_CELL_BATTERY:
				effect = RaiseStatsContactItem(bank, STAT_STAGE_ATK, TYPE_ELECTRIC);
				break;

			case ITEM_EFFECT_LUMINOUS_MOSS:
				effect = RaiseStatsContactItem(bank, STAT_STAGE_SPDEF, TYPE_WATER);
				break;

			case ITEM_EFFECT_SNOWBALL:
				effect = RaiseStatsContactItem(bank, STAT_STAGE_ATK, TYPE_ICE);
				break;

			case ITEM_EFFECT_JABOCA_ROWAP_BERRY:	;
				if (!doPluck)
				{
					if (TOOK_DAMAGE(bank)
					&& MOVE_HAD_EFFECT
					&& gBattleMons[gBankAttacker].hp
					&& gBattleMons[bank].hp
					&& !MoveBlockedBySubstitute(gCurrentMove, gBankAttacker, bank)
					&& CalcMoveSplit(gCurrentMove, gBankAttacker, bank) == bankQuality)
					{
						gBattleMoveDamage = MathMax(1, GetBaseMaxHP(gBankAttacker) / 8);

						if (ABILITY(bank) == ABILITY_RIPEN)
							gBattleMoveDamage *= 2;

						gEffectBank = gBankAttacker;
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_JabocaRowapBerry;
						gActiveBattler = gBankAttacker;
						effect = ITEM_HP_CHANGE;
					}
				}
				break;

			case ITEM_EFFECT_KEE_BERRY:
				effect = KeeMaranagaBerryFunc(bank, STAT_STAGE_DEF, SPLIT_PHYSICAL, doPluck);
				break;

			case ITEM_EFFECT_MARANGA_BERRY:
				effect = KeeMaranagaBerryFunc(bank, STAT_STAGE_SPDEF, SPLIT_SPECIAL, doPluck);
				break;

			case ITEM_EFFECT_ENIGMA_BERRY:
				if (!doPluck
				&& !IsHealBlocked(bank)
				&& TOOK_DAMAGE(bank)
				&& gMoveResultFlags == MOVE_RESULT_SUPER_EFFECTIVE
				&& BATTLER_ALIVE(bank)
				&& !MoveBlockedBySubstitute(gCurrentMove, gBankAttacker, bank))
				{
					gBattleMoveDamage = MathMax(1, GetBaseMaxHP(bank) / 4);

					if (ABILITY(bank) == ABILITY_RIPEN)
						gBattleMoveDamage *= 2;

					if (gBattleMons[bank].hp + gBattleMoveDamage > gBattleMons[bank].maxHP)
						gBattleMoveDamage = gBattleMons[bank].maxHP - gBattleMons[bank].hp;
					gBattleMoveDamage *= -1;
					BattleScriptPushCursor();
					BattleScriptExecute(BattleScript_BerryHealHP_RemoveBerryRet);
					effect = ITEM_HP_CHANGE;
				}
				break;

			case ITEM_EFFECT_STICKY_BARB:
				if (TOOK_DAMAGE(bank)
				&& MOVE_HAD_EFFECT
				&& CheckContact(gCurrentMove, gBankAttacker, bank)
				&& !MoveBlockedBySubstitute(gCurrentMove, gBankAttacker, bank)
				&& gBattleMons[gBankAttacker].hp != 0
				&& gBattleMons[gBankAttacker].item == ITEM_NONE)
				{
					gEffectBank = bank;

					gBattleMons[bank].item = 0;
					gBattleMons[gBankAttacker].item = gLastUsedItem;
					HandleUnburdenBoost(bank); //Give old holder's Unburden boost
					HandleUnburdenBoost(gBankAttacker); //Remove new holder's Unburden boost

					gActiveBattler = gBankAttacker;
					EmitSetMonData(0, REQUEST_HELDITEM_BATTLE, 0, 2, &gLastUsedItem);
					MarkBufferBankForExecution(gActiveBattler);

					gActiveBattler = bank;
					EmitSetMonData(0, REQUEST_HELDITEM_BATTLE, 0, 2, &gBattleMons[gActiveBattler].item);
					MarkBufferBankForExecution(gActiveBattler);

					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_StickyBarbTransfer;
					gActiveBattler = gBankAttacker;
					effect = ITEM_EFFECT_OTHER;
				}
				break;
		}

		if (effect && GetPocketByItemId(gLastUsedItem)== POCKET_BERRY_POUCH)
			gNewBS->canBelch[SIDE(bank)] |= gBitTable[gBattlerPartyIndexes[bank]];

		if (effect == REQUEST_HP_BATTLE)
		{
			gBattleScripting.bank = bank;
			gStringBank = bank;
			gActiveBattler = bank;
			EmitSetMonData(0, REQUEST_HP_BATTLE, 0, 2, &gBattleMons[gActiveBattler].status1);
			MarkBufferBankForExecution(gActiveBattler);
			break;
		}

		break;

	case ItemEffects_ContactAttacker:
		//These item effects must explicitly be listed in CMD49.c in order to work!
		switch (atkHoldEffect)
		{
			case ITEM_EFFECT_FLINCH:
				if (ABILITY(gBankAttacker) == ABILITY_SERENEGRACE || BankHasRainbow(gBankAttacker)) //NO STACKING!
					bankQuality *= 2;
				if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT)
				&& ABILITY(gBankTarget) != ABILITY_STENCH
				&& TOOK_DAMAGE(gBankTarget)
				&& MOVE_HAD_EFFECT
				&& (umodsi(Random(), 100)) < bankQuality
				&& !gSpecialMoveFlags[gCurrentMove].gFlinchChanceMoves
				&& BATTLER_ALIVE(gBankTarget)
				&& !SheerForceCheck())
				{
					gBattleMons[gBankTarget].status2 |= STATUS2_FLINCHED;
				}
				break;
			case ITEM_EFFECT_SHELL_BELL:
				if (gNewBS->totalDamageGiven > 0
				&& bank != gBankTarget
				&& BATTLER_ALIVE(bank)
				&& !BATTLER_MAX_HP(bank)
				&& !IsHealBlocked(bank)
				&& !SheerForceCheck())
				{
					if (gBattleMons[bank].hp <= gBattleMons[bank].maxHP / 2)
						gNewBS->lessThanHalfHPBeforeShellBell = TRUE; //For Emergency Exit

					gStringBank = bank;
					gBattleScripting.bank = bank;
					gBattleMoveDamage = MathMax(1, udivsi(gNewBS->totalDamageGiven, atkQuality)) * - 1;
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_ItemHealHP_Ret;
					effect++;
				}
				break;
			case ITEM_EFFECT_LIFE_ORB: ;
				u8 moveEffect = gBattleMoves[gCurrentMove].effect;

				if (gNewBS->AttackerDidDamageAtLeastOnce
				&& moveEffect != EFFECT_BIDE //Moves that deal direct damage aren't included
				&& moveEffect != EFFECT_COUNTER
				&& moveEffect != EFFECT_MIRROR_COAT
				&& moveEffect != EFFECT_ENDEAVOR
				&& moveEffect != EFFECT_SUPER_FANG
				&& moveEffect != EFFECT_LEVEL_DAMAGE
				&& moveEffect != EFFECT_PSYWAVE
				&& gCurrentMove != MOVE_FINALGAMBIT
				&& ABILITY(bank) != ABILITY_MAGICGUARD
				&& BATTLER_ALIVE(bank)
				&& !SheerForceCheck())
				{
					gBattleMoveDamage = MathMax(1, GetBaseMaxHP(bank) / 10);
					gNewBS->selfInflictedDamage += gBattleMoveDamage; //For Emergency Exit
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_LifeOrbDamage;
					effect++;
				}
				break;
			case ITEM_EFFECT_THROAT_SPRAY: ;
				bool8 throatSprayWork = FALSE;

				if (SPLIT(gCurrentMove) == SPLIT_STATUS)
				{
					if (IS_DOUBLE_BATTLE)
					{
						if (gBattleMoves[gCurrentMove].target & MOVE_TARGET_SPREAD)
						{
							//Check affected at least one target
							//Assumes the status move doesn't do damage (which it shouldn't)
							throatSprayWork = (BATTLER_ALIVE(gBankTarget) && !(gNewBS->ResultFlags[gBankTarget] & MOVE_RESULT_NO_EFFECT))
											|| (BATTLER_ALIVE(PARTNER(gBankTarget)) && !(gNewBS->ResultFlags[PARTNER(gBankTarget)] & MOVE_RESULT_NO_EFFECT));

							if (gBattleMoves[gCurrentMove].target & MOVE_TARGET_ALL)
								throatSprayWork |= (BATTLER_ALIVE(PARTNER(gBankAttacker)) && !(gNewBS->ResultFlags[PARTNER(gBankAttacker)] & MOVE_RESULT_NO_EFFECT));
						}
					}
					else if (MOVE_HAD_EFFECT)
						throatSprayWork = TRUE;
				}
				else //Damaging move
				{
					if (gNewBS->AttackerDidDamageAtLeastOnce)
						throatSprayWork = TRUE;
				}

				if (throatSprayWork
				&& BATTLER_ALIVE(gBankAttacker)
				&& !(gHitMarker & HITMARKER_UNABLE_TO_USE_MOVE)
				&& CheckSoundMove(gCurrentMove)
				&& !ChangeStatBuffs(SET_STAT_BUFF_VALUE(1), STAT_STAGE_SPATK, MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_CERTAIN, 0))
				{
					PREPARE_STAT_BUFFER(gBattleTextBuff1, STAT_STAGE_SPATK);

					gEffectBank = bank;
					gBattleScripting.animArg1 = STAT_ANIM_PLUS1 + STAT_STAGE_SPATK - 1;
					gBattleScripting.animArg2 = 0;

					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_RaiseStatsItem;
					effect = ITEM_STATS_CHANGE;
				}
				break;
			case ITEM_EFFECT_BLUNDER_POLICY:
				if (gNewBS->activateBlunderPolicy
				&& BATTLER_ALIVE(gBankAttacker)
				&& !ChangeStatBuffs(SET_STAT_BUFF_VALUE(2), STAT_STAGE_SPEED, MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_CERTAIN, 0))
				{
					PREPARE_STAT_BUFFER(gBattleTextBuff1, STAT_STAGE_SPEED);

					gEffectBank = gBankAttacker;
					gBattleScripting.animArg1 = STAT_ANIM_PLUS2 + STAT_STAGE_SPEED - 1;
					gBattleScripting.animArg2 = 0;

					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_RaiseStatsItem;
					effect = ITEM_STATS_CHANGE;
				}

				gNewBS->activateBlunderPolicy = FALSE;
				break;
		}
		break;
	}

	return effect;
}

static u8 ConfusionBerries(u8 bank, u8 flavour, bool8 moveTurn, bool8 doPluck) {
	u8 effect = 0;

	if (IsHealBlocked(bank))
		return effect;

	#ifdef OLD_CONFUSION_HEAL_BERRIES
	if ((gBattleMons[bank].hp <= gBattleMons[bank].maxHP / 2 && !moveTurn)
	|| (doPluck && !BATTLER_MAX_HP(bank)))
	{
		gBattleTextBuff1[0] = 0xFD;
		gBattleTextBuff1[1] = 8;
		gBattleTextBuff1[2] = flavour;
		gBattleTextBuff1[3] = EOS;

		gBattleMoveDamage = GetBaseMaxHP(bank) / 8;

		if (ABILITY(bank) == ABILITY_RIPEN)
			gBattleMoveDamage *= 2;

		if (gBattleMoveDamage == 0)
			gBattleMoveDamage = 1;
		if (gBattleMons[bank].hp + gBattleMoveDamage > gBattleMons[bank].maxHP)
			gBattleMoveDamage = gBattleMons[bank].maxHP - gBattleMons[bank].hp;
		gBattleMoveDamage *= -1;

		if (moveTurn || doPluck)
		{
			BattleScriptPushCursor();
			if (GetPokeFlavourRelation(gBattleMons[bank].personality, flavour) < 0)
				gBattlescriptCurrInstr = BattleScript_BerryConfuseHealRet;
			else
				gBattlescriptCurrInstr = BattleScript_BerryHealHP_RemoveBerryRet;
		}
		else
		{
			if (GetPokeFlavourRelation(gBattleMons[bank].personality, flavour) < 0)
				BattleScriptExecute(BattleScript_BerryConfuseHealEnd2);
			else
				BattleScriptExecute(BattleScript_BerryHealHP_RemoveBerryEnd2);
		}
		effect = ITEM_HP_CHANGE;
	}
	#else
	if (PINCH_BERRY_CHECK(bank) || (doPluck && !BATTLER_MAX_HP(bank)))
	{
		gBattleTextBuff1[0] = 0xFD;
		gBattleTextBuff1[1] = 8;
		gBattleTextBuff1[2] = flavour;
		gBattleTextBuff1[3] = EOS;

		#ifdef GEN_7_CONFUSION_HEAL_BERRIES
		gBattleMoveDamage = GetBaseMaxHP(bank) / 2;
		if (ABILITY(bank) == ABILITY_RIPEN)
			gBattleMoveDamage *= 2;
		#else
		gBattleMoveDamage = GetBaseMaxHP(bank) / 3;
		if (ABILITY(bank) == ABILITY_RIPEN)
			gBattleMoveDamage = (gBattleMoveDamage * 4) / 3; //So total winds up being 1.666
		#endif

		if (ABILITY(bank) == ABILITY_RIPEN)
			gBattleMoveDamage *= 2;

		if (gBattleMoveDamage == 0)
			gBattleMoveDamage = 1;
		if (gBattleMons[bank].hp + gBattleMoveDamage > gBattleMons[bank].maxHP)
			gBattleMoveDamage = gBattleMons[bank].maxHP - gBattleMons[bank].hp;
		gBattleMoveDamage *= -1;

		if (moveTurn || doPluck)
		{
			BattleScriptPushCursor();
			if (GetPokeFlavourRelation(gBattleMons[bank].personality, flavour) < 0)
				gBattlescriptCurrInstr = BattleScript_BerryConfuseHealRet;
			else
				gBattlescriptCurrInstr = BattleScript_BerryHealHP_RemoveBerryRet;
		}
		else
		{
			if (GetPokeFlavourRelation(gBattleMons[bank].personality, flavour) < 0)
				BattleScriptExecute(BattleScript_BerryConfuseHealEnd2);
			else
				BattleScriptExecute(BattleScript_BerryHealHP_RemoveBerryEnd2);
		}
		effect = ITEM_HP_CHANGE;
	}
	#endif
	return effect;
}

static u8 StatRaiseBerries(u8 bank, u8 stat, bool8 moveTurn, bool8 doPluck) {
	u8 effect = 0;
	u8 backupUser = gBankAttacker;
	u8 buff = 1;
	u8 animId = STAT_ANIM_PLUS1;
	gBankAttacker = bank;

	if (ABILITY(bank) == ABILITY_RIPEN)
	{
		buff *= 2;
		animId = STAT_ANIM_PLUS2;
	}

	if ((PINCH_BERRY_CHECK(bank) || doPluck) && STAT_CAN_RISE(bank, stat)
	&& BATTLER_ALIVE(bank)
	&& !ChangeStatBuffs(SET_STAT_BUFF_VALUE(buff), stat, MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_CERTAIN, 0))
	{
		PREPARE_STAT_BUFFER(gBattleTextBuff1, stat);
		PREPARE_STRING_BUFFER(gBattleTextBuff2, STRINGID_STATROSE);

		gEffectBank = bank;
		gBattleScripting.statChanger = INCREASE_1 | stat;
		gBattleScripting.animArg1 = animId + stat - 1;
		gBattleScripting.animArg2 = 0;
		if (moveTurn || doPluck)
		{
			BattleScriptPushCursor();
			gBattlescriptCurrInstr = BattleScript_BerryStatRaiseRet;
		}
		else
			BattleScriptExecute(BattleScript_BerryStatRaiseEnd2);
		effect = ITEM_STATS_CHANGE;
	}

	gBankAttacker = backupUser;
	return effect;
}

static u8 RaiseStatsContactItem(u8 bank, u8 stat, u8 moveType) {
	u8 effect = 0;
	u8 backupUser = gBankAttacker;
	gBankAttacker = bank;

	if (TOOK_DAMAGE(bank)
	&& gBattleStruct->dynamicMoveType == moveType
	&& BATTLER_ALIVE(bank)
	&& !MoveBlockedBySubstitute(gCurrentMove, gBankAttacker, bank)
	&& !ChangeStatBuffs(SET_STAT_BUFF_VALUE(1), stat, MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_CERTAIN, 0))
	{
		PREPARE_STAT_BUFFER(gBattleTextBuff1, stat);

		gEffectBank = bank;
		gBattleScripting.animArg1 = STAT_ANIM_PLUS1 + stat - 1;
		gBattleScripting.animArg2 = 0;

		BattleScriptPushCursor();
		gBattlescriptCurrInstr = BattleScript_RaiseStatsItem;
		effect = ITEM_STATS_CHANGE;
	}

	gBankAttacker = backupUser;
	return effect;
}

static u8 KeeMaranagaBerryFunc(u8 bank, u8 stat, u8 split, bool8 doPluck) {
	u8 effect = 0;
	u8 backupUser = gBankAttacker;
	u8 buff = 1;
	u8 animId = STAT_ANIM_PLUS1;
	gBankAttacker = bank;

	if (ABILITY(bank) == ABILITY_RIPEN)
	{
		buff *= 2;
		animId = STAT_ANIM_PLUS2;
	}

	if (((TOOK_DAMAGE(bank) && CalcMoveSplit(gCurrentMove, gBankAttacker, bank) == split && !MoveBlockedBySubstitute(gCurrentMove, gBankAttacker, bank)) || doPluck)
	&& BATTLER_ALIVE(bank)
	&& !SheerForceCheck()
	&& !ChangeStatBuffs(SET_STAT_BUFF_VALUE(buff), stat, MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_CERTAIN, 0))
	{
		gEffectBank = gBankAttacker;
		gBattleScripting.animArg1 = animId + stat - 1;
		gBattleScripting.animArg2 = 0;

		BattleScriptPushCursor();
		gBattlescriptCurrInstr = BattleScript_BerryStatRaiseRet;
		effect = ITEM_STATS_CHANGE;
	}

	gBankAttacker = backupUser;
	return effect;
}
