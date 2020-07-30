#include "defines.h"
#include "defines_battle.h"
#include "../include/random.h"
#include "../include/constants/items.h"

#include "../include/new/ability_battle_scripts.h"
#include "../include/new/accuracy_calc.h"
#include "../include/new/attackcanceler.h"
#include "../include/new/battle_indicators.h"
#include "../include/new/battle_start_turn_start.h"
#include "../include/new/battle_util.h"
#include "../include/new/cmd49.h"
#include "../include/new/cmd49_battle_scripts.h"
#include "../include/new/damage_calc.h"
#include "../include/new/dynamax.h"
#include "../include/new/form_change.h"
#include "../include/new/item.h"
#include "../include/new/item_battle_scripts.h"
#include "../include/new/move_battle_scripts.h"
#include "../include/new/move_tables.h"
#include "../include/new/set_effect.h"
#include "../include/new/util.h"

/*
cmd49.c
	handles a ton of battle logic at the end of each turn
*/

//TODO:
//Make sure there's no choice lock glitch
//Add check to see if AI move prediction was successful. If not, then if the same move is predicted, don't predict that same move again.
//Remove the lines at the bottom?

/*Fix references to:
BattleScript_SpikesOnAttackerFainted
BattleScript_SpikesOnTargetFainted
*/

enum
{
	ATK49_SET_UP,
	ATK49_ATTACKER_ABILITIES,
	ATK49_ADVERSE_PROTECTION,
	ATK49_RAGE,
	ATK49_SYNCHRONIZE_TARGET,
	ATK49_BEAK_BLAST_BURN,
	ATK49_SYNCHRONIZE_ATTACKER,
	ATK49_MOVE_END_ABILITIES,
	ATK49_SYNCHRONIZE_ATTACKER_2,
	ATK49_ITEM_EFFECTS_CONTACT_TARGET,

//Above are all done in prefaintmoveendeffects
//They are done here if they didn't activate before.

	ATK49_UNDO_SKY_DROP,
	ATK49_UNDO_SKY_DROP_2,
	ATK49_ATTACKER_INVISIBLE,
	ATK49_TARGET_INVISIBLE,
	ATK49_ATTACKER_VISIBLE,
	ATK49_TARGET_VISIBLE,
	ATK49_CHOICE_MOVE,
	ATK49_PLUCK,
	ATK49_STICKYHOLD,
	ATK49_ITEM_EFFECTS_END_TURN_TARGET,
	ATK49_ITEM_EFFECTS_KINGS_ROCK,
	ATK49_ITEM_EFFECTS_END_TURN_ATTACKER,
	ATK49_STATUS_IMMUNITY_ABILITIES,
	ATK49_UPDATE_LAST_MOVES,
	ATK49_MIRROR_MOVE,
	ATK49_MULTI_HIT_MOVES,
	ATK49_RAID_SHIELD_BREAK,
	ATK49_DEFROST,
	ATK49_SECOND_MOVE_EFFECT,
	ATK49_MAGICIAN_MOXIE_BATTLEBOND,
	ATK49_FATIGUE,
	ATK49_ITEM_EFFECTS_END_TURN_ATTACKER_2,
	ATK49_NEXT_TARGET,
	ATK49_MOVE_RECOIL,
	ATK49_ITEM_EFFECTS_END_TURN_ATTACKER_3,
	ATK49_EJECT_BUTTON,
	ATK49_RED_CARD,
	ATK49_EJECT_PACK,
	ATK49_SHELL_BELL_LIFE_ORB_RECOIL,
	ATK49_SWITCH_OUT_ABILITIES,
	ATK49_RESTORE_ABILITIES,
	ATK49_PICKPOCKET,
	ATK49_RAID_MON_PREP_MORE_ATTACKS,
	ATK49_SUBSTITUTE,
	ATK49_END_ZMOVES,
	ATK49_DANCER,
	ATK49_RAID_SHIELD,
	ATK49_COUNT,
};

static bool8 CanDoMoveEndSwitchout(u8 arg);

void atk49_moveend(void) //All the effects that happen after a move is used
{
	int i;
	u32 effect = FALSE;
	u8 moveType = 0;
	u8 holdEffectAtk = 0;
	u16* choicedMoveAtk = NULL;
	u8 arg1, arg2;
	u16 originallyUsedMove;

	u8 bankDef = gBankTarget;

	if (gChosenMove == 0xFFFF)
		originallyUsedMove = 0;
	else
		originallyUsedMove = gChosenMove;

	arg1 = gBattlescriptCurrInstr[1];
	arg2 = gBattlescriptCurrInstr[2];

	holdEffectAtk = ITEM_EFFECT(gBankAttacker);

	if (gNewBS->MoveBounceInProgress)
		choicedMoveAtk = &gBattleStruct->choicedMove[gBankTarget];
	else
		choicedMoveAtk = &gBattleStruct->choicedMove[gBankAttacker];

	moveType = gBattleStruct->dynamicMoveType;

	if (gHitMarker & HITMARKER_UNABLE_TO_USE_MOVE)
	{ 	//Cancel the Z-Moves
		gNewBS->zMoveData.active = FALSE;
		gNewBS->zMoveData.toBeUsed[gBankAttacker] = 0;
		gNewBS->dynamaxData.active = FALSE;
	}

	if (arg1 == ARG_ONLY_EMERGENCY_EXIT)
		gBattleScripting.atk49_state = ATK49_SWITCH_OUT_ABILITIES;

	if (gBattleScripting.atk49_state == ATK49_SET_UP //If these have already
	&&  gNewBS->preFaintEffectsState == FAINT_COUNT) //been done
	{
		gNewBS->originalAttackerBackup = gBankAttacker;
		gBattleScripting.atk49_state = ATK49_UNDO_SKY_DROP;
	}

	do
	{
		switch (gBattleScripting.atk49_state)
		{
		case ATK49_SET_UP: //For Emergency Exit to use later
			gNewBS->originalAttackerBackup = gBankAttacker;

			if (gNewBS->MultiHitOn)
				gNewBS->turnDamageTaken[gBankTarget] += gHpDealt; //Total up damage taken
			else
				gNewBS->turnDamageTaken[gBankTarget] = gHpDealt;

			gNewBS->totalDamageGiven += gHpDealt;
			gNewBS->ResultFlags[gBankTarget] = gMoveResultFlags;
			gBattleScripting.atk49_state++;
			break;

		case ATK49_ATTACKER_ABILITIES:
			if (arg1 != ARG_IN_FUTURE_ATTACK
			&& TOOK_DAMAGE(gBankTarget)
			&& MOVE_HAD_EFFECT
			&& BATTLER_ALIVE(gBankTarget)
			&& !MoveBlockedBySubstitute(gCurrentMove, gBankAttacker, gBankTarget))
			{
				switch (ABILITY(gBankAttacker)) {
					case ABILITY_STENCH: //Check for Stench is taken care of in King's Rock check
						if (umodsi(Random(), 100) < 10
						&& gCurrentTurnActionNumber < GetBattlerTurnOrderNum(gBankTarget)) //Attacker moved before target
						{
							gBattleMons[gBankTarget].status2 |= STATUS2_FLINCHED;
						}
						break;

					case ABILITY_POISONTOUCH:
						if (CheckContact(gCurrentMove, gBankAttacker)
						&& ABILITY(gBankTarget) != ABILITY_SHIELDDUST
						&& CanBePoisoned(gBankTarget, gBankAttacker, TRUE)
						&& umodsi(Random(), 100) < 30)
						{
							BattleScriptPushCursor();
							gBattlescriptCurrInstr = BattleScript_PoisonTouch;
							effect = TRUE;
						}
				}
			}
			gBattleScripting.atk49_state++;
			break;

		case ATK49_ADVERSE_PROTECTION:
			if (gProtectStructs[gBankTarget].kingsshield_damage)
			{
				gProtectStructs[gBankTarget].kingsshield_damage = 0;

				if (BATTLER_ALIVE(gBankAttacker) && STAT_CAN_FALL(gBankAttacker, STAT_ATK))
				{
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_KingsShield;
					effect = TRUE;
					break;
				}
			}

			if (gProtectStructs[gBankTarget].spikyshield_damage)
			{
				gProtectStructs[gBankTarget].spikyshield_damage = 0;

				if (BATTLER_ALIVE(gBankAttacker) && ABILITY(gBankAttacker) != ABILITY_MAGICGUARD)
				{
					gBattleMoveDamage = MathMax(1, GetBaseMaxHP(gBankAttacker) / 8);
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_SpikyShield;
					effect = TRUE;
					break;
				}
			}

			if (gProtectStructs[gBankTarget].banefulbunker_damage)
			{
				gProtectStructs[gBankTarget].banefulbunker_damage = 0;

				if (BATTLER_ALIVE(gBankAttacker) && CanBePoisoned(gBankAttacker, gBankTarget, TRUE)) //Target poisons Attacker
				{
					gBattleMons[gBankAttacker].status1 = STATUS_POISON;
					gEffectBank = gActiveBattler = gBankAttacker;
					EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[gBankAttacker].status1);
					MarkBufferBankForExecution(gActiveBattler);

					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_BanefulBunker;
					effect = TRUE;
					break;
				}
			}

			if (gProtectStructs[gBankTarget].obstructDamage)
			{
				gProtectStructs[gBankTarget].obstructDamage = FALSE;

				if (BATTLER_ALIVE(gBankAttacker) && STAT_CAN_FALL(gBankAttacker, STAT_DEF))
				{
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_ObstructStatDecrement;
					effect = TRUE;
					break;
				}
			}

			gBattleScripting.atk49_state++;
			break;

		case ATK49_RAGE: // rage check
			if (gBattleMons[gBankTarget].status2 & STATUS2_RAGE
			&& BATTLER_ALIVE(gBankTarget)
			&& gBankAttacker != gBankTarget
			&& SIDE(gBankAttacker) != SIDE(gBankTarget)
			&& MOVE_HAD_EFFECT
			&& TOOK_DAMAGE(gBankTarget)
			&& SPLIT(gCurrentMove) != SPLIT_STATUS
			&& STAT_CAN_RISE(gBankTarget, STAT_ATK))
			{
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_RageIsBuilding;
				effect = TRUE;
			}
			gBattleScripting.atk49_state++;
			break;

		case ATK49_SYNCHRONIZE_TARGET: // target synchronize
			if (gCurrentMove != MOVE_PSYCHOSHIFT || !MOVE_HAD_EFFECT) //The lazy way of taking care of Psycho Shift Status Transfer->Synchronize->Heal Status
			{
				if (AbilityBattleEffects(ABILITYEFFECT_SYNCHRONIZE, gBankTarget, 0, 0, 0))
					effect = TRUE;
			}
			gBattleScripting.atk49_state++;
			break;

		case ATK49_BEAK_BLAST_BURN:
			if (CheckContact(gCurrentMove, gBankAttacker)
			&& MOVE_HAD_EFFECT
			&& TOOK_DAMAGE(gBankTarget)
			&& gNewBS->BeakBlastByte & gBitTable[gBankTarget]
			&& CanBeBurned(gBankAttacker, TRUE))
			{
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_BeakBlastBurn;
				effect = TRUE;
			}

			gBattleScripting.atk49_state++;
			break;

		case ATK49_SYNCHRONIZE_ATTACKER: // attacker synchronize
		case ATK49_SYNCHRONIZE_ATTACKER_2:
			if (AbilityBattleEffects(ABILITYEFFECT_SYNCHRONIZE, gBankAttacker, 0, 0, 0))
				effect = TRUE;
			gBattleScripting.atk49_state++;
			break;

		case ATK49_MOVE_END_ABILITIES: // Such as abilities activating on contact(Poison Spore, Rough Skin, etc.).
			if (AbilityBattleEffects(ABILITYEFFECT_CONTACT, gBankTarget, 0, 0, 0))
				effect = TRUE;
			gBattleScripting.atk49_state++;
			break;

		case ATK49_ITEM_EFFECTS_CONTACT_TARGET:
			if (gBattleMoves[gCurrentMove].effect != EFFECT_EAT_BERRY //Skip Berries for Pluck to eat
			|| gCurrentMove == MOVE_INCINERATE
			|| ITEM_POCKET(gBankTarget) != POCKET_BERRY_POUCH
			|| ITEM_EFFECT(gBankTarget) == ITEM_EFFECT_JABOCA_ROWAP_BERRY) //Only berries that activate before pluck
			{
				if (gBattleMoves[gCurrentMove].effect != EFFECT_KNOCK_OFF
				|| (ITEM_EFFECT(gBankTarget) != ITEM_EFFECT_KEE_BERRY && ITEM_EFFECT(gBankTarget) != ITEM_EFFECT_MARANGA_BERRY)) //Only contact items that don't activate first
				{
					if (ItemBattleEffects(ItemEffects_ContactTarget, gBankTarget, TRUE, FALSE))
						effect = TRUE;
				}
			}
			gBattleScripting.atk49_state++;
			break;


//From here on are always done in CMD49

		case ATK49_UNDO_SKY_DROP: ; //One of the mons in the air died
			u8 bankToFree;

			for (i = 0; i < gBattlersCount; ++i)
			{
				if (gBattleMons[i].hp == 0)
				{
					if (gStatuses3[gNewBS->skyDropAttackersTarget[gBankTarget]] & STATUS3_SKY_DROP_TARGET
					&&  gNewBS->skyDropTargetsAttacker[gNewBS->skyDropAttackersTarget[gBankTarget]] == gBankTarget)
					{
						bankToFree = gNewBS->skyDropAttackersTarget[i];
						gNewBS->skyDropAttackersTarget[i] = 0;
						gNewBS->skyDropTargetsAttacker[bankToFree] = 0;

						//A message is only printed when the target is freed.
						gBattleScripting.bank = bankToFree;
						gBattleStringLoader = FreedFromSkyDropString;
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_PrintCustomString;
						effect = TRUE;

						gActiveBattler = bankToFree;
						EmitSpriteInvisibility(0, FALSE);
						MarkBufferBankForExecution(gActiveBattler);
					}
					else
						continue;

					gStatuses3[i] &= ~(STATUS3_SKY_DROP_ATTACKER | STATUS3_SKY_DROP_TARGET | STATUS3_IN_AIR);
					gStatuses3[bankToFree] &= ~(STATUS3_SKY_DROP_ATTACKER | STATUS3_SKY_DROP_TARGET | STATUS3_IN_AIR);
					break;
				}
			}

			gBattleScripting.atk49_state++;
			break;

		case ATK49_UNDO_SKY_DROP_2: //The attacker can no longer attack while in the air due to paralysis etc.
			if (gCurrentMove == MOVE_SKYDROP
			&& gHitMarker & HITMARKER_UNABLE_TO_USE_MOVE
			&& gStatuses3[gBankAttacker] & STATUS3_SKY_DROP_ATTACKER)
			{
				gStatuses3[gBankAttacker] &= ~STATUS3_SKY_DROP_ATTACKER;
				gStatuses3[gBankTarget] &= ~STATUS3_SKY_DROP_TARGET;
				gBattleScripting.bank = gBankTarget;
				gBattleStringLoader = FreedFromSkyDropString;
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_PrintCustomString;
				effect = TRUE;
			}
			gBattleScripting.atk49_state++;
			break;

		case ATK49_ATTACKER_INVISIBLE: // make attacker sprite invisible
			if (gStatuses3[gBankAttacker] & (STATUS3_SEMI_INVULNERABLE)
			&&  gHitMarker & HITMARKER_NO_ANIMATIONS)
			{
				gActiveBattler = gBankAttacker;
				EmitSpriteInvisibility(0, TRUE);
				MarkBufferBankForExecution(gActiveBattler);
				gBattleScripting.atk49_state++;
				return;
			}
			gBattleScripting.atk49_state++;
			break;

		case ATK49_TARGET_INVISIBLE: // make target sprite invisible
			if (gStatuses3[gBankTarget] & (STATUS3_SEMI_INVULNERABLE)
			&&  gHitMarker & HITMARKER_NO_ANIMATIONS)
			{
				gActiveBattler = gBankTarget;
				EmitSpriteInvisibility(0, TRUE);
				MarkBufferBankForExecution(gActiveBattler);
				gBattleScripting.atk49_state++;
				return;
			}
			gBattleScripting.atk49_state++;
			break;

		case ATK49_ATTACKER_VISIBLE: // make attacker sprite visible
			if (gMoveResultFlags & MOVE_RESULT_NO_EFFECT
			|| !(gStatuses3[gBankAttacker] & (STATUS3_SEMI_INVULNERABLE))
			|| WasUnableToUseMove(gBankAttacker))
			{
				gActiveBattler = gBankAttacker;
				EmitSpriteInvisibility(0, FALSE);
				MarkBufferBankForExecution(gActiveBattler);
				gStatuses3[gBankAttacker] &= ~(STATUS3_SEMI_INVULNERABLE);
				gSpecialStatuses[gBankAttacker].restoredBankSprite = 1;
				gBattleScripting.atk49_state++;
				return;
			}
			gBattleScripting.atk49_state++;
			break;

		case ATK49_TARGET_VISIBLE: // make target sprite visible
			if (!gSpecialStatuses[gBankTarget].restoredBankSprite
			&& gBankTarget < gBattlersCount
			&& !(gStatuses3[gBankTarget] & STATUS3_SEMI_INVULNERABLE))
			{
				gActiveBattler = gBankTarget;
				EmitSpriteInvisibility(0, FALSE);
				MarkBufferBankForExecution(gActiveBattler);
				gStatuses3[gBankTarget] &= ~(STATUS3_SEMI_INVULNERABLE);
				gBattleScripting.atk49_state++;
				return;
			}
			gBattleScripting.atk49_state++;
			break;

		case ATK49_CHOICE_MOVE: // update choice band move
			if (arg1 != ARG_IN_FUTURE_ATTACK && !IsDynamaxed(gBankAttacker))
			{
				if (gChosenMove != MOVE_STRUGGLE)
				{
					if (gHitMarker & HITMARKER_OBEYS)
					{
						if (holdEffectAtk == ITEM_EFFECT_CHOICE_BAND || ABILITY(gBankAttacker) == ABILITY_GORILLATACTICS)
						{
							if (*choicedMoveAtk == 0 || *choicedMoveAtk == 0xFFFF)
							{
								if (gChosenMove == MOVE_BATONPASS && !(gMoveResultFlags & MOVE_RESULT_FAILED))
								{
									gBattleScripting.atk49_state++;
									break;
								}

								*choicedMoveAtk = gChosenMove;
							}
						}
						else //This should remove the choice lock glitch
							*choicedMoveAtk = 0;
					}

					if (!MoveInMoveset(*choicedMoveAtk, gBankAttacker))
						*choicedMoveAtk = 0;
				}
			}
			gBattleScripting.atk49_state++;
			break;

		extern const u16 gBannedBattleEatBerries[];
		case ATK49_PLUCK:
			if (MOVE_HAD_EFFECT
			&& TOOK_DAMAGE(gBankTarget)
			&& gBattleMoves[gCurrentMove].effect == EFFECT_EAT_BERRY
			&& gCurrentMove != MOVE_INCINERATE
			&& IsBerry(ITEM(gBankTarget))
			&& !CheckTableForItem(ITEM(gBankTarget), gBannedBattleEatBerries)
			&& ABILITY(gBankTarget) != ABILITY_STICKYHOLD
			&& (!BATTLER_ALIVE(gBankTarget) || !(ITEM_EFFECT(gBankTarget) == ITEM_EFFECT_JABOCA_ROWAP_BERRY && ITEM_QUALITY(gBankTarget) == CalcMoveSplit(gBankAttacker, gCurrentMove))))
			{
				gNewBS->BelchCounters |= gBitTable[gBattlerPartyIndexes[gBankAttacker]];

				gLastUsedItem = gBattleMons[gBankTarget].item;
				gBattleMons[gBankTarget].item = 0;
				HandleUnburdenBoost(gBankTarget); //Give target Unburden boost

				gActiveBattler = gBankTarget;
				EmitSetMonData(0, REQUEST_HELDITEM_BATTLE, 0, 2, &gBattleMons[gActiveBattler].item);
				MarkBufferBankForExecution(gActiveBattler);

				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_PluckEat;

				effect = TRUE;
			}
			gBattleScripting.atk49_state++;
			break;

		case ATK49_STICKYHOLD:
			if (MOVE_HAD_EFFECT
			&& TOOK_DAMAGE(gBankTarget)
			&& (gBattleMoves[gCurrentMove].effect == EFFECT_KNOCK_OFF || gBattleMoves[gCurrentMove].effect == EFFECT_THIEF)
			&& CanKnockOffItem(gBankTarget)
			&& ABILITY(gBankTarget) == ABILITY_STICKYHOLD
			&& BATTLER_ALIVE(gBankTarget))
			{
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_StickyHoldActivatesRet;
				effect = TRUE;
			}
			gBattleScripting.atk49_state++;
			break;

		case ATK49_ITEM_EFFECTS_END_TURN_TARGET:
			if ((ABILITY(gBankTarget) == ABILITY_STICKYHOLD && BATTLER_ALIVE(gBankTarget))
			||  !BATTLER_ALIVE(gBankAttacker)
			||  !CanKnockOffItem(gBankTarget)
			||  gBattleMoves[gCurrentMove].effect != EFFECT_KNOCK_OFF) //Don't activate items that should be knocked off
			{
				if (ItemBattleEffects(ItemEffects_EndTurn, gBankTarget, TRUE, FALSE))
					effect = TRUE;
			}
			gBattleScripting.atk49_state++;
			break;

		case ATK49_ITEM_EFFECTS_KINGS_ROCK:
			if (arg1 != ARG_IN_FUTURE_ATTACK && ITEM_EFFECT(gBankAttacker) == ITEM_EFFECT_FLINCH) //King's Rock Only
			{
				if (ItemBattleEffects(ItemEffects_ContactAttacker, gBankAttacker, TRUE, FALSE))
					effect = TRUE;
			}
			gBattleScripting.atk49_state++;
			break;

		case ATK49_ITEM_EFFECTS_END_TURN_ATTACKER:
		case ATK49_ITEM_EFFECTS_END_TURN_ATTACKER_2:
		case ATK49_ITEM_EFFECTS_END_TURN_ATTACKER_3:
			if (ItemBattleEffects(ItemEffects_EndTurn, gBankAttacker, TRUE, FALSE))
				effect = TRUE;
			gBattleScripting.atk49_state++;
			break;

		case ATK49_STATUS_IMMUNITY_ABILITIES: // status immunities
			if (AbilityBattleEffects(ABILITYEFFECT_IMMUNITY, 0, 0, 0, 0))
				effect = TRUE; // it loops through all battlers, so we increment after its done with all battlers
			else
				gBattleScripting.atk49_state++;
			break;

		case ATK49_UPDATE_LAST_MOVES:
			if (gNewBS->DancerInProgress
			|| arg1 == ARG_IN_FUTURE_ATTACK)
			{
				gBattleScripting.atk49_state++;
				break;
			}

			if (gHitMarker & HITMARKER_SWAP_ATTACKER_TARGET && arg1 != ARG_IN_PURSUIT)
			{
				bankDef = gBankAttacker;
				gBankAttacker = gBankTarget;
				gBankTarget = bankDef;
				gHitMarker &= ~(HITMARKER_SWAP_ATTACKER_TARGET);
			}

			if (gHitMarker & HITMARKER_ATTACKSTRING_PRINTED)
			{
				gNewBS->lastTargeted[gBankAttacker] = gBankTarget;
				if (IsZMove(gCurrentMove) || IsAnyMaxMove(gCurrentMove))
					gLastPrintedMoves[gBankAttacker] = gCurrentMove;
				else
					gLastPrintedMoves[gBankAttacker] = gChosenMove;

				if (!IsZMove(gCurrentMove) && !IsDynamaxed(gBankAttacker))
					gNewBS->usedMoveIndices[gBankAttacker] |= gBitTable[gCurrMovePos];
			}

			if (!(gAbsentBattlerFlags & gBitTable[gBankAttacker])
			&& !(gBattleStruct->field_91 & gBitTable[gBankAttacker])
			&& gBattleMoves[originallyUsedMove].effect != EFFECT_BATON_PASS)
			{
				if (gHitMarker & HITMARKER_OBEYS)
				{
					gLastUsedMoves[gBankAttacker] = gChosenMove;
					gLastResultingMoves[gBankAttacker] = gCurrentMove;
				}
				else
				{
					gLastUsedMoves[gBankAttacker] = 0xFFFF;
					gLastResultingMoves[gBankAttacker] = 0xFFFF;
				}

				if (!(gHitMarker & HITMARKER_FAINTED(bankDef)))
					gLastHitBy[bankDef] = gBankAttacker;

				if (gHitMarker & HITMARKER_OBEYS && MOVE_HAD_EFFECT)
				{
					if (gChosenMove == 0xFFFF)
					{
						gLastLandedMoves[bankDef] = gChosenMove;
					}
					else
					{
						if (gNewBS->zMoveData.active)
							gLastLandedMoves[bankDef] = gChosenMove;

						else
							gLastLandedMoves[bankDef] = gCurrentMove;

						gLastHitByType[bankDef] = moveType;
					}
				}
				else
				{
					gLastLandedMoves[bankDef] = 0xFFFF;
				}
			}
			gBattleScripting.atk49_state++;
			break;

		case ATK49_MIRROR_MOVE:
			if (arg1 != ARG_IN_FUTURE_ATTACK
			&& !(gAbsentBattlerFlags & gBitTable[gBankAttacker])
			&& !(gBattleStruct->field_91 & gBitTable[gBankAttacker])
			&& gBattleMoves[originallyUsedMove].flags & FLAG_MIRROR_MOVE_AFFECTED
			&& gHitMarker & HITMARKER_OBEYS
			&& gBankAttacker != bankDef
			&& !(gHitMarker & HITMARKER_FAINTED(bankDef))
			&& MOVE_HAD_EFFECT)
			{
				gBattleStruct->lastTakenMove[bankDef] = gChosenMove;
				gBattleStruct->lastTakenMoveFrom[bankDef][gBankAttacker] = gChosenMove;
			}

			if (gNewBS->zMoveData.active)
			{
				gBattleStruct->lastTakenMove[bankDef] = MOVE_NONE;
				gBattleStruct->lastTakenMoveFrom[bankDef][gBankAttacker] = MOVE_NONE;
			}

			gBattleScripting.atk49_state++;
			gSeedHelper[0] = 0; //Reset Seed Helper for Soul Heart
			break;

		case ATK49_MULTI_HIT_MOVES:
			if (arg1 != ARG_IN_FUTURE_ATTACK
			&&  arg1 != ARG_IN_PURSUIT
			&& !(gMoveResultFlags & MOVE_RESULT_NO_EFFECT)
			&& !(gHitMarker & HITMARKER_UNABLE_TO_USE_MOVE)
			&& gMultiHitCounter)
			{
				++gBattleScripting.multihitString[4];
				if (--gMultiHitCounter == 0)
				{
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_MultiHitPrintStrings;
					effect = 1;
				}
				else
				{
					if (gCurrentMove == MOVE_DRAGONDARTS
					&&  gBattleStruct->moveTarget[gBankAttacker] == gBankTarget //Haven't already changed targets
					&&  CanTargetPartner(gBankTarget)
					&&  !TargetFullyImmuneToCurrMove(PARTNER(gBankTarget)))
					{
						gBankTarget = PARTNER(gBankTarget); //Target the partner is doubles for second hit.
						PressurePPLose(gBankTarget, gBankAttacker, gChosenMove);
					}

					if (BATTLER_ALIVE(gBankAttacker)
					&& BATTLER_ALIVE(gBankTarget)
					&& (gChosenMove == MOVE_SLEEPTALK || !(gBattleMons[gBankAttacker].status1 & STATUS1_SLEEP))
					&& !(gBattleMons[gBankAttacker].status1 & STATUS1_FREEZE))
					{
						if (gNewBS->ParentalBondOn)
							--gNewBS->ParentalBondOn;

						gHitMarker |= (HITMARKER_NO_PPDEDUCT | HITMARKER_NO_ATTACKSTRING);
						gBattleScripting.animTargetsHit = 0;
						gNewBS->preFaintEffectsState = 0;
						gBattleScripting.atk49_state = 0;
						gNewBS->MultiHitOn = TRUE; //Used to help second accuracy calcs
						MoveValuesCleanUp();
						BattleScriptPush(gBattleScriptsForMoveEffects[gBattleMoves[gCurrentMove].effect]);
						gBattlescriptCurrInstr = BattleScript_FlushMessageBox;
						return;
					}
					else
					{
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_MultiHitPrintStrings;
						effect = 1;
					}
				}
			}

			gBattleScripting.atk49_state++;
			gMultiHitCounter = 0;
			gNewBS->ParentalBondOn = 0;
			gNewBS->MultiHitOn = FALSE;
			break;

		case ATK49_RAID_SHIELD_BREAK: //Here b/c multi-hit moves only break one shield
		#ifdef FLAG_RAID_BATTLE
			if (IsRaidBattle()
			&& GetBattlerPosition(gBankTarget) == B_POSITION_OPPONENT_LEFT
			&& MOVE_HAD_EFFECT
			&& TOOK_DAMAGE(gBankTarget)
			&& gNewBS->dynamaxData.raidShieldsUp)
			{
				DestroyRaidShieldSprite();
				if (IsAnyMaxMove(gCurrentMove)
				|| IsZMove(gCurrentMove)
				|| gBattleMoves[gCurrentMove].effect == EFFECT_0HKO
				|| gBattleMoves[gCurrentMove].effect == EFFECT_BRICK_BREAK) //Unofficial addition
					DestroyRaidShieldSprite();
				if (IsZMove(gCurrentMove))
					DestroyRaidShieldSprite(); //Z-Moves destroy 3 shields

				if (gNewBS->dynamaxData.shieldsDestroyed >= gNewBS->dynamaxData.shieldCount)
				{
					gNewBS->dynamaxData.raidShieldsUp = FALSE;
					gBattleScripting.bank = gBankTarget;
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_BrokenRaidBarrier;

					//Do some residual damage after the shattering
					gBattleMoveDamage = gBattleMons[gBankTarget].maxHP / 6;
					if (gNewBS->dynamaxData.shieldsDestroyed > gNewBS->dynamaxData.shieldCount)
						gBattleMoveDamage = (gBattleMoveDamage * 15) / 10;
					gNewBS->dynamaxData.turnStartHP = gBattleMons[gBankTarget].hp - gBattleMoveDamage; //No reactivating barrier yet
					effect = TRUE;
				}
			}
		#endif
			gBattleScripting.atk49_state++;
			break;

		case ATK49_DEFROST: // defrosting check
			if (gBattleMons[bankDef].status1 & STATUS1_FREEZE
			&&  gBattleMons[bankDef].hp
			&&  gBankAttacker != bankDef
			&&  MOVE_HAD_EFFECT
			&&  TOOK_DAMAGE(bankDef)
			&& (moveType == TYPE_FIRE || CheckTableForMove(gCurrentMove, gMovesCanUnfreezeTarget)))
			{
				gBattleMons[bankDef].status1 &= ~(STATUS1_FREEZE);
				gActiveBattler = bankDef;
				EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[bankDef].status1);
				MarkBufferBankForExecution(gActiveBattler);
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_DefrostedViaFireMove;
				effect = TRUE;
			}
			gBattleScripting.atk49_state++;
			break;

		case ATK49_SECOND_MOVE_EFFECT:
			gBattleCommunication[MOVE_EFFECT_BYTE] = gNewBS->backupMoveEffect;
			gNewBS->backupMoveEffect = 0;
			effect = SetMoveEffect2();
			gBattleScripting.atk49_state++;
			break;

		case ATK49_MAGICIAN_MOXIE_BATTLEBOND:
			switch (ABILITY(gBankAttacker)) {
				case ABILITY_MAGICIAN:
					if (ITEM(gBankAttacker) == ITEM_NONE
					&& ITEM(bankDef) != ITEM_NONE
					&& BATTLER_ALIVE(gBankAttacker)
					&& !MoveBlockedBySubstitute(gCurrentMove, gBankAttacker, bankDef)
					&& TOOK_DAMAGE(bankDef)
					&& MOVE_HAD_EFFECT
					&& CanTransferItem(SPECIES(bankDef), ITEM(bankDef))
					&& CanTransferItem(SPECIES(gBankAttacker), ITEM(bankDef))
					&& (ABILITY(bankDef) != ABILITY_STICKYHOLD || !BATTLER_ALIVE(bankDef)))
					{
						gBattleScripting.bank = gBankAttacker;
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_Magician;
						effect = 1;
					}
					break;

				case ABILITY_MOXIE:
					if ((arg1 != ARG_IN_FUTURE_ATTACK || gWishFutureKnock.futureSightPartyIndex[bankDef] == gBattlerPartyIndexes[gBankAttacker])
					&& gBattleMons[bankDef].hp == 0
					&& BATTLER_ALIVE(gBankAttacker)
					&& TOOK_DAMAGE(bankDef)
					&& MOVE_HAD_EFFECT
					&& STAT_CAN_RISE(gBankAttacker, STAT_STAGE_ATK)
					&& ViableMonCountFromBank(FOE(gBankAttacker)) > 0) //Use FOE so as to not get boost when KOing partner last after enemy has no mons left
					{
						PREPARE_STAT_BUFFER(gBattleTextBuff1, STAT_STAGE_ATK);

						gEffectBank = gBankAttacker;
						gBattleScripting.bank = gBankAttacker;
						gBattleScripting.statChanger = INCREASE_1 | STAT_STAGE_ATK;
						gBattleScripting.animArg1 = 0xE + STAT_STAGE_ATK;
						gBattleScripting.animArg2 = 0;

						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_Moxie;
						effect = 1;
					}
					break;

				case ABILITY_BEASTBOOST: ;
					if ((arg1 != ARG_IN_FUTURE_ATTACK || gWishFutureKnock.futureSightPartyIndex[bankDef] == gBattlerPartyIndexes[gBankAttacker])
					&& gBattleMons[bankDef].hp == 0
					&& BATTLER_ALIVE(gBankAttacker)
					&& TOOK_DAMAGE(bankDef)
					&& MOVE_HAD_EFFECT
					&& ViableMonCountFromBank(FOE(gBankAttacker)) > 0) //Use FOE so as to not get boost when KOing partner last after enemy has no mons left
					{
						u16 temp;
						u16 max;
						u16 stats[STAT_STAGE_SPDEF]; //Create new array to avoid modifying original stats

						stats[STAT_STAGE_ATK-1] = gBattleMons[gBankAttacker].attack;
						stats[STAT_STAGE_DEF-1] = gBattleMons[gBankAttacker].defense;
						stats[STAT_STAGE_SPATK-2] = gBattleMons[gBankAttacker].spAttack;
						stats[STAT_STAGE_SPDEF-2] = gBattleMons[gBankAttacker].spDefense;
						stats[STAT_STAGE_SPEED+1] = gBattleMons[gBankAttacker].speed;

						if (IsWonderRoomActive()) {
							temp = stats[STAT_STAGE_DEF-1];
							stats[STAT_STAGE_DEF-1] = stats[STAT_STAGE_SPDEF-2]; //-2 b/c shifted left due to speed
							stats[STAT_STAGE_SPDEF-2] = temp;
						}

						max = 0;
						for (int i = 1; i < STAT_STAGE_SPDEF; ++i) {
							if (stats[i] > stats[max])
								max = i;
						}

						//Get the proper stat stage value
						switch(max) {
							case 0: //Attack
							case 1: //Defense
								max += 1;
								break;
							case 2: //Special Attack
							case 3: //Special Defense
								max += 2;
								break;
							case 4:
								max = STAT_STAGE_SPEED;
						}

						if (STAT_CAN_RISE(gBankAttacker, max))
						{
							PREPARE_STAT_BUFFER(gBattleTextBuff1, max);

							gEffectBank = gBankAttacker;
							gBattleScripting.bank = gBankAttacker;
							gBattleScripting.statChanger = INCREASE_1 | max;
							gBattleScripting.animArg1 = 0xE + max;
							gBattleScripting.animArg2 = 0;
							gLastUsedAbility = ABILITY_BEASTBOOST;
							RecordAbilityBattle(gBankAttacker, gLastUsedAbility);

							BattleScriptPushCursor();
							gBattlescriptCurrInstr = BattleScript_Moxie;
							effect = 1;
						}
					}
					break;

				#if (defined SPECIES_GRENINJA && defined SPECIES_ASHGRENINJA)
				case ABILITY_BATTLEBOND:
					if ((arg1 != ARG_IN_FUTURE_ATTACK || gWishFutureKnock.futureSightPartyIndex[bankDef] == gBattlerPartyIndexes[gBankAttacker])
					&& SPECIES(gBankAttacker) == SPECIES_GRENINJA
					&& gBattleMons[bankDef].hp == 0
					&& BATTLER_ALIVE(gBankAttacker)
					&& TOOK_DAMAGE(bankDef)
					&& MOVE_HAD_EFFECT
					&& ViableMonCountFromBank(FOE(gBankAttacker)) > 0 //Use FOE so as to not get boost when KOing partner last after enemy has no mons left
					&& !IS_TRANSFORMED(gBankAttacker))
					{
						DoFormChange(gBankAttacker, SPECIES_ASHGRENINJA, TRUE, TRUE, FALSE);

						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_AbilityTransformed;
						effect = 1;
					}
				#endif
			}
			*gSeedHelper = 0; //For Soul-Heart Loop
			gBattleScripting.atk49_state++;
			break;

		case ATK49_FATIGUE:
			if (gBattleMons[gBankAttacker].status2 & STATUS2_LOCK_CONFUSE
			&& BATTLER_ALIVE(gBankAttacker)
			&& !gNewBS->DancerInProgress)
			{
				gBattleMons[gBankAttacker].status2 -= 0x400;

				if (!(gBattleMons[gBankAttacker].status2 & STATUS2_LOCK_CONFUSE)
				&&   (gBattleMons[gBankAttacker].status2 & STATUS2_MULTIPLETURNS))
				{
					gBattleMons[gBankAttacker].status2 &= ~(STATUS2_MULTIPLETURNS);

					if (!IsConfused(gBankAttacker)
					&& ABILITY(gBankAttacker) != ABILITY_OWNTEMPO
					&& !(CheckGrounding(gBankAttacker) && gTerrainType == MISTY_TERRAIN))
					{
						gBattleMons[gBankAttacker].status2 |= (umodsi(Random(), 4)) + 2;
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_ThrashConfuses;
						effect = 1;
					}
				}
				else if (!MOVE_HAD_EFFECT || (gHitMarker & HITMARKER_UNABLE_TO_USE_MOVE))
					CancelMultiTurnMoves(gBankAttacker);
			}
			gBattleScripting.atk49_state++;
			break;

		case ATK49_NEXT_TARGET: // For moves hitting two opposing Pokemon or whole field.
			gHitMarker |= HITMARKER_NO_ATTACKSTRING;
			if (gNewBS->MoveBounceInProgress)
				++gNewBS->MoveBounceTargetCount;
			else
				++gNewBS->OriginalAttackerTargetCount;

			if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
			{
				if (!(gHitMarker & HITMARKER_UNABLE_TO_USE_MOVE)
				&& !gProtectStructs[gBankAttacker].chargingTurn)
				{
					if (gBattleMoves[gCurrentMove].target == MOVE_TARGET_BOTH)
					{
						if ((gNewBS->OriginalAttackerTargetCount < 2 && !gNewBS->MoveBounceInProgress)
						|| (gNewBS->MoveBounceTargetCount < 2 && gNewBS->MoveBounceInProgress))
						{ //Get Next Target
							u8 battlerId = PARTNER(bankDef);
							if (gBattleMons[battlerId].hp && battlerId != gBankAttacker)
							{
								gBankTarget = battlerId;
								gNewBS->preFaintEffectsState = 0;
								gBattleScripting.atk49_state = 0;
								MoveValuesCleanUp();

								if (gBattleMoves[gCurrentMove].effect == EFFECT_SUPERPOWER //Only lower user stats once for Clanging Scales (and similar custom moves)
								&&  gNewBS->AttackerDidDamageAtLeastOnce)
									gBattlescriptCurrInstr = gBattleScriptsForMoveEffects[0];
								else
									gBattlescriptCurrInstr = gBattleScriptsForMoveEffects[gBattleMoves[gCurrentMove].effect];

								if (SPLIT(gCurrentMove) == SPLIT_STATUS)
								{
									BattleScriptPushCursor();
									gBattlescriptCurrInstr = BattleScript_FlushMessageBox;
								}
								return;
							}
							else if (gNewBS->MoveBounceInProgress)
								goto RESTORE_BOUNCE_ATTACKER;
						}
						else if (gNewBS->MoveBounceInProgress)
						{
						RESTORE_BOUNCE_ATTACKER:
							++gNewBS->OriginalAttackerTargetCount;
							gNewBS->MoveBounceInProgress = 2; //Bounce just finished
							gNewBS->MoveBounceTargetCount = 0;
							u8 battlerId = PARTNER(gBankAttacker);
							if (BATTLER_ALIVE(battlerId) && gNewBS->OriginalAttackerTargetCount < 2)
							{
								gBankAttacker = gBanksByTurnOrder[gCurrentTurnActionNumber]; //Restore original attacker
								gBankTarget = battlerId; //Attack Bouncer's partner
								gBattleScripting.animTargetsHit = 0;
								gNewBS->preFaintEffectsState = 0;
								gBattleScripting.atk49_state = 0;
								MoveValuesCleanUp();
								BattleScriptPush(gBattleScriptsForMoveEffects[gBattleMoves[gCurrentMove].effect]);
								gBattlescriptCurrInstr = BattleScript_FlushMessageBox;
								return;
							}
						}
					}
					else if (gBattleMoves[gCurrentMove].target == MOVE_TARGET_ALL && !CheckTableForMove(gCurrentMove, gSpecialWholeFieldMoves))
					{
						if (gNewBS->OriginalAttackerTargetCount < 3)
						{ //Get Next Target
							u8 battlerId = GetNextMultiTarget();
							while ((battlerId = GetNextMultiTarget()) != 0xFF)
							{
								gBankTarget = battlerId;
								if (BATTLER_ALIVE(battlerId) && battlerId != gBankAttacker)
								{
									gNewBS->preFaintEffectsState = 0;
									gBattleScripting.atk49_state = 0;
									MoveValuesCleanUp();
									if (gBattleMoves[gCurrentMove].effect == EFFECT_EXPLOSION)
										BattleScriptPush(gBattleScriptsForMoveEffects[0]); //Fixes issues with Explosion not changing targets
									else
										BattleScriptPush(gBattleScriptsForMoveEffects[gBattleMoves[gCurrentMove].effect]);
									gBattlescriptCurrInstr = BattleScript_FlushMessageBox;
									return;
								}
								++gNewBS->OriginalAttackerTargetCount;
							}
						}
					}
				}
			}

			if (gNewBS->MoveBounceInProgress)
				gNewBS->MoveBounceInProgress = 2; //Bounce just finished

			gNewBS->MoveBounceTargetCount = 0;
			gNewBS->OriginalAttackerTargetCount = 0;
			gBattleScripting.atk49_state++;
			break;

		case ATK49_MOVE_RECOIL:
			if (arg1 != ARG_IN_FUTURE_ATTACK
			&&  !(gHitMarker & HITMARKER_UNABLE_TO_USE_MOVE))
			{
				if ((gCurrentMove == MOVE_MINDBLOWN || gCurrentMove == MOVE_STEELBEAM)
				&& ABILITY(gBankAttacker) != ABILITY_MAGICGUARD
				&& BATTLER_ALIVE(gBankAttacker))
				{
					gBattleMoveDamage = MathMax(1, gBattleMons[gBankAttacker].maxHP / 2);

					if (IS_ODD(gBattleMons[gBankAttacker].maxHP)
					&& gBattleMons[gBankAttacker].maxHP >= 3)
						++gBattleMoveDamage; //Round up Damage

					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_MindBlownDamage;

					effect = 1;
				}
				else if (gCurrentMove == MOVE_STRUGGLE
				&& BATTLER_ALIVE(gBankAttacker))
				{
					gBattleMoveDamage = MathMax(1, gBattleMons[gBankAttacker].maxHP / 4);
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_Recoil;
					effect = 1;
				}
				else if (gBattleMoves[gCurrentMove].effect == EFFECT_EXPLOSION
				&& !ABILITY_PRESENT(ABILITY_DAMP))
				{
					gBattleMoveDamage = 0;
					BattleScriptPushCursor();

					if (gNewBS->AttackerDidDamageAtLeastOnce)
						gBattlescriptCurrInstr = BattleScript_FaintAttackerForExplosion;
					else
						gBattlescriptCurrInstr = BattleScript_ExplosionAnim; //The attacker still needs to explode

					effect = 1;
				}
				else if (ABILITY(gBankAttacker) != ABILITY_MAGICGUARD
				&& ABILITY(gBankAttacker) != ABILITY_ROCKHEAD
				&& gNewBS->AttackerDidDamageAtLeastOnce
				&& BATTLER_ALIVE(gBankAttacker))
				{
					if (CheckTableForMove(gCurrentMove, gPercent25RecoilMoves))
					{
						gBattleMoveDamage = MathMax(1, gNewBS->totalDamageGiven / 4);
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_Recoil;
						effect = 1;
					}
					else if (CheckTableForMove(gCurrentMove, gPercent33RecoilMoves))
					{
						gBattleMoveDamage = MathMax(1, gNewBS->totalDamageGiven / 3);
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_Recoil;
						effect = 1;
					}
					else if (CheckTableForMove(gCurrentMove, gPercent50RecoilMoves))
					{
						gBattleMoveDamage = MathMax(1, gNewBS->totalDamageGiven / 2);
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_Recoil;
						effect = 1;
					}
					else if (CheckTableForMove(gCurrentMove, gPercent66RecoilMoves))
					{
						gBattleMoveDamage = MathMax(1, (gNewBS->totalDamageGiven * 2) / 3);
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_Recoil;
						effect = 1;
					}
					else if (CheckTableForMove(gCurrentMove, gPercent75RecoilMoves))
					{
						gBattleMoveDamage = MathMax(1, (gNewBS->totalDamageGiven * 3) / 4);
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_Recoil;
						effect = 1;
					}
					else if (CheckTableForMove(gCurrentMove, gPercent100RecoilMoves))
					{
						gBattleMoveDamage = MathMax(1, gNewBS->totalDamageGiven);
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_Recoil;
						effect = 1;
					}
				}
				
				if (effect)
					gNewBS->selfInflictedDamage += gBattleMoveDamage; //For Emergency Exit
			}
			gBattleScripting.atk49_state++;
			break;

		case ATK49_EJECT_BUTTON:
			gBankAttacker = gNewBS->originalAttackerBackup;
			if (CanDoMoveEndSwitchout(arg1))
			{
				u8 banks[4] = {0, 1, 2, 3};
				SortBanksBySpeed(banks, FALSE);

				for (i = 0; i < gBattlersCount; ++i)
				{
					if (banks[i] != gBankAttacker
					&&  gBattleMons[banks[i]].hp
					&&  !SheerForceCheck()
					&&  ITEM_EFFECT(banks[i]) == ITEM_EFFECT_EJECT_BUTTON
					&&  !(gNewBS->ResultFlags[banks[i]] & MOVE_RESULT_NO_EFFECT)
					&&  gNewBS->turnDamageTaken[banks[i]] != 0
					&&  !MoveBlockedBySubstitute(gCurrentMove, gBankAttacker, gBankTarget)
					&&  ((gBattleTypeFlags & BATTLE_TYPE_TRAINER) || SIDE(i) == B_SIDE_PLAYER)) //Wilds can't activate
					{
						if (gBattleMoves[gCurrentMove].effect == EFFECT_BATON_PASS)
							gBattlescriptCurrInstr = BattleScript_MoveEnd; //Cancel switchout for U-Turn & Volt Switch

						gNewBS->NoSymbiosisByte = TRUE;
						gActiveBattler = gBankSwitching = gBattleScripting.bank = banks[i];
						gLastUsedItem = ITEM(banks[i]);
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_EjectButton;
						effect = 1;
						break; //Only the fastest Eject Button activates so end loop
					}
				}
			}
			gBattleScripting.atk49_state++;
			break;

		case ATK49_RED_CARD:
			gBankAttacker = gNewBS->originalAttackerBackup;
			if (CanDoMoveEndSwitchout(arg1)
			&& BATTLER_ALIVE(gBankAttacker))
			{
				u8 banks[4] = {0, 1, 2, 3};
				SortBanksBySpeed(banks, FALSE);

				for (i = 0; i < gBattlersCount; ++i)
				{
					if (banks[i] != gBankAttacker
					&&  BATTLER_ALIVE(banks[i])
					&&  !SheerForceCheck()
					//&&  !IsDynamaxed(banks[i]) //Can't force out a Dynamaxed mon - handled in BS
					&&  ITEM_EFFECT(banks[i]) == ITEM_EFFECT_RED_CARD
					&&  !(gNewBS->ResultFlags[banks[i]] & MOVE_RESULT_NO_EFFECT)
					&&  gNewBS->turnDamageTaken[banks[i]] != 0
					&&  !MoveBlockedBySubstitute(gCurrentMove, gBankAttacker, banks[i])
					&&  ((gBattleTypeFlags & BATTLE_TYPE_TRAINER) || SIDE(gBankAttacker) == B_SIDE_PLAYER)) //Wild attackers can't activate
					{
						gNewBS->NoSymbiosisByte = TRUE;
						gForceSwitchHelper = Force_Switch_Red_Card;
						gBattlescriptCurrInstr = BattleScript_Atk49; //Cancels U-Turn, Volt Switch, and Natural Gift
						gActiveBattler = gBankSwitching = gBattleScripting.bank = gNewBS->originalTargetBackup = banks[i];
						gLastUsedItem = ITEM(banks[i]);
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_RedCard;
						effect = 1;
						break; //Only the fastest Red Card activates so end loop
					}
				}
			}
			gBattleScripting.atk49_state++;
			break;

		case ATK49_EJECT_PACK:
			gBankAttacker = gNewBS->originalAttackerBackup;
			if (CanDoMoveEndSwitchout(arg1))
			{
				for (i = 0; i < gBattlersCount; ++i)
				{
					if (BATTLER_ALIVE(i)
					&&  ITEM_EFFECT(i) == ITEM_EFFECT_EJECT_PACK
					&&  gNewBS->statFellThisTurn[i])
					{
						gNewBS->statFellThisTurn[i] = FALSE;
						gNewBS->NoSymbiosisByte = TRUE;
						gActiveBattler = gBankSwitching = gBattleScripting.bank = i;
						gLastUsedItem = ITEM(i);
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_EjectPackCMD49;
						effect = 1;
						return;
					}
				}
			}
			gBattleScripting.atk49_state++;
			gNewBS->switchOutBankLooper = 0;
			break;

		case ATK49_SWITCH_OUT_ABILITIES:
			gBankAttacker = gNewBS->originalAttackerBackup;
			if (CanDoMoveEndSwitchout(arg1) && !SheerForceCheck())
			{
				for (; gNewBS->switchOutBankLooper < gBattlersCount; ++gNewBS->switchOutBankLooper)
				{
					u8 bank = gBanksByTurnOrder[gNewBS->switchOutBankLooper];

					if (bank != gBankAttacker)
					{
						if ((/*ABILITY(bank) == ABILITY_WIMPOUT ||*/ ABILITY(bank) == ABILITY_EMERGENCYEXIT)
						&&  !(gNewBS->ResultFlags[bank] & MOVE_RESULT_NO_EFFECT)
						&&  !MoveBlockedBySubstitute(gCurrentMove, gBankAttacker, bank)
						&&  !(gStatuses3[bank] & (STATUS3_SKY_DROP_ANY))
						&&  BATTLER_ALIVE(bank)
						&&  gBattleMons[bank].hp <= gBattleMons[bank].maxHP / 2
						&&  gBattleMons[bank].hp + gNewBS->turnDamageTaken[bank] > gBattleMons[bank].maxHP / 2) //Fell this turn
						{
							if (gBattleMoves[gCurrentMove].effect == EFFECT_BATON_PASS)
								gBattlescriptCurrInstr = BattleScript_Atk49; //Cancel switchout for U-Turn & Volt Switch

							gActiveBattler = gBattleScripting.bank = gBankSwitching = bank;
							BattleScriptPushCursor();
							gBattlescriptCurrInstr = BattleScript_EmergencyExit;
							effect = 1;
							break;
						}
					}
					else //Self inflicted damage
					{
						if ((/*ABILITY(bank) == ABILITY_WIMPOUT ||*/ ABILITY(bank) == ABILITY_EMERGENCYEXIT)
						&&  !(gStatuses3[bank] & (STATUS3_SKY_DROP_ANY))
						&&  BATTLER_ALIVE(bank)
						&&  (gBattleMons[bank].hp <= gBattleMons[bank].maxHP / 2 || gNewBS->lessThanHalfHPBeforeShellBell) //Ignore Shell Bell Recovery
						&&  gBattleMons[bank].hp + gNewBS->selfInflictedDamage > gBattleMons[bank].maxHP / 2) //Fell this turn
						{
							if (gBattleMoves[gCurrentMove].effect == EFFECT_BATON_PASS)
								gBattlescriptCurrInstr = BattleScript_Atk49; //Cancel switchout for U-Turn & Volt Switch

							gActiveBattler = gBattleScripting.bank = gBankSwitching = bank;
							BattleScriptPushCursor();
							gBattlescriptCurrInstr = BattleScript_EmergencyExit;
							effect = 1;
							break;
						}
					}
				}
			}
			gBattleScripting.atk49_state++;
			break;

		case ATK49_SHELL_BELL_LIFE_ORB_RECOIL:
			gBankAttacker = gNewBS->originalAttackerBackup; //Restore Original Attacker
			u8 itemEffect = ITEM_EFFECT(gBankAttacker);

			if (arg1 != ARG_IN_FUTURE_ATTACK
			&& !(gHitMarker & HITMARKER_UNABLE_TO_USE_MOVE)
			&& (itemEffect == ITEM_EFFECT_LIFE_ORB
			 || itemEffect == ITEM_EFFECT_SHELL_BELL
			 || itemEffect == ITEM_EFFECT_THROAT_SPRAY
			 || itemEffect == ITEM_EFFECT_BLUNDER_POLICY))
			{
				if (ItemBattleEffects(ItemEffects_ContactAttacker, gBankAttacker, TRUE, FALSE))
					effect = TRUE;
			}
			gBattleScripting.atk49_state++;
			break;

		case ATK49_RESTORE_ABILITIES:
			for (i = 0; i < gBattlersCount; ++i)
			{
				if (gNewBS->DisabledMoldBreakerAbilities[i])
				{
					gBattleMons[i].ability = gNewBS->DisabledMoldBreakerAbilities[i];
					gNewBS->DisabledMoldBreakerAbilities[i] = 0;
				}
			}
			gBattleScripting.atk49_state++;
			break;

		case ATK49_PICKPOCKET: ;
			u8 banks[4] = {0, 1, 2, 3};
			SortBanksBySpeed(banks, FALSE);

			for (i = 0; i < gBattlersCount; ++i)
			{
				u8 bank = banks[i];

				if (bank != gBankAttacker
				&&  !SheerForceCheck()
				&&  ABILITY(bank) == ABILITY_PICKPOCKET
				&&  !(gNewBS->ResultFlags[bank] & MOVE_RESULT_NO_EFFECT)
				&&  TOOK_DAMAGE(bank)
				&&  !MoveBlockedBySubstitute(gCurrentMove, gBankAttacker, bank)
				&&  gBattleMons[bank].hp != 0
				&&  CheckContact(gCurrentMove, gBankAttacker)
				&&  ITEM(gBankAttacker) != ITEM_NONE
				&&  ITEM(bank) == ITEM_NONE
				&& (ABILITY(gBankAttacker) != ABILITY_STICKYHOLD || !BATTLER_ALIVE(gBankAttacker)))
				{
					gNewBS->NoSymbiosisByte = TRUE;
					gLastUsedItem = ITEM(gBankAttacker);
					gBankTarget = gActiveBattler = gBattleScripting.bank = bank;
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_Pickpocket;
					effect = 1;
					break; //Only fastest Pickpocket activates so exit loop.
				}
			}
			gBattleScripting.atk49_state++;
			break;

		case ATK49_RAID_MON_PREP_MORE_ATTACKS:
			if (IsRaidBattle()
			&& BATTLER_ALIVE(gBankAttacker)
			&& gBankAttacker == BANK_RAID_BOSS
			&& (SPLIT(gCurrentMove) == SPLIT_STATUS || !IsAnyMaxMove(gCurrentMove)) //Status move or Raid Boss didn't use Max Move
			&& Random() % 100 < GetRaidBattleRepeatedAttackChance(gBankAttacker))
				gNewBS->dynamaxData.attackAgain = TRUE;
			gBattleScripting.atk49_state++;
			break;

		case ATK49_SUBSTITUTE: // update substitute
			for (i = 0; i < gBattlersCount; i++)
			{
				if (gDisableStructs[i].substituteHP == 0)
					gBattleMons[i].status2 &= ~(STATUS2_SUBSTITUTE);
			}
			gBattleScripting.atk49_state++;
			break;

		case ATK49_END_ZMOVES:
			if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE
			&&  gBattleMoves[gCurrentMove].target & (MOVE_TARGET_BOTH | MOVE_TARGET_ALL))
			{
				int i;

				for (i = 0; i < gBattlersCount; ++i)
				{
					if (!(gNewBS->ResultFlags[i] & (MOVE_RESULT_DOESNT_AFFECT_FOE | MOVE_RESULT_FAILED))) //If at least one of the targets was affected
						break;
				}
				if (i == gBattlersCount) //No targets were affected
					gNewBS->StompingTantrumTimers[gBankAttacker] = 2;
			}
			else if (gMoveResultFlags & (MOVE_RESULT_DOESNT_AFFECT_FOE | MOVE_RESULT_FAILED))
				gNewBS->StompingTantrumTimers[gBankAttacker] = 2;

			for (int i = 0; i < MAX_BATTLERS_COUNT; ++i)
			{
				gNewBS->turnDamageTaken[i] = 0;
				gNewBS->DamageTaken[i] = 0;
				gNewBS->criticalMultiplier[i] = 0;
				gNewBS->ResultFlags[i] = 0;
				gNewBS->missStringId[i] = 0;
				gNewBS->noResultString[i] = 0;
				gNewBS->statFellThisTurn[i] = 0;
			}

			gNewBS->totalDamageGiven = 0;
			gNewBS->selfInflictedDamage = 0;
			gNewBS->lessThanHalfHPBeforeShellBell = FALSE;
			//Clear spread move things
			gNewBS->doneDoublesSpreadHit = FALSE;
			gNewBS->calculatedSpreadMoveData = FALSE;
			gNewBS->calculatedSpreadMoveAccuracy = FALSE;

			switch (gCurrentMove) {
				case MOVE_FUSIONFLARE:
					gNewBS->fusionFlareUsedPrior = TRUE;
					gNewBS->fusionBoltUsedPrior = FALSE;
					break;
				case MOVE_FUSIONBOLT:
					gNewBS->fusionFlareUsedPrior = FALSE;
					gNewBS->fusionBoltUsedPrior = TRUE;
					break;
				default:
					gNewBS->fusionFlareUsedPrior = FALSE;
					gNewBS->fusionBoltUsedPrior = FALSE;
					break;
			}

			gNewBS->zMoveData.active = FALSE;
			gNewBS->zMoveData.effectApplied = FALSE;
			gNewBS->zMoveData.toBeUsed[gBankAttacker] = 0;
			gNewBS->dynamaxData.active = FALSE;
			gNewBS->secondaryEffectApplied = FALSE;
			gNewBS->InstructInProgress = FALSE;
			gNewBS->bypassSubstitute = FALSE;
			gNewBS->preFaintEffectsState = 0;
			gNewBS->MeFirstByte = FALSE;
			gNewBS->breakDisguiseSpecialDmg = FALSE;
			gBattleScripting.atk49_state++;
			break;

		case ATK49_DANCER:
			if (gNewBS->DancerBankCount == gBattlersCount)
			{
				gNewBS->DancerInProgress = FALSE;
				gNewBS->DancerByte = 0;
				gNewBS->DancerBankCount = 0;
				gNewBS->DancerTurnOrder[0] = 0; //Set the whole array to 0
				gNewBS->DancerTurnOrder[1] = 0;
				gNewBS->DancerTurnOrder[2] = 0;
				gNewBS->DancerTurnOrder[3] = 0;
				gBattleScripting.atk49_state++;
				gNewBS->AttackerDidDamageAtLeastOnce = FALSE;
				gNewBS->attackAnimationPlayed = FALSE;
				gNewBS->moveWasBouncedThisTurn = FALSE;
				break;
			}

			if (!gNewBS->DancerInProgress
			&& arg1 != ARG_IN_PURSUIT
			&& ABILITY_PRESENT(ABILITY_DANCER)
			&& gNewBS->attackAnimationPlayed
			&& !gNewBS->moveWasBouncedThisTurn
			&& CheckTableForMove(gCurrentMove, gDanceMoves))
			{
				gNewBS->DancerInProgress = TRUE;
				gNewBS->CurrentTurnAttacker = gBankAttacker;
				gNewBS->CurrentTurnTarget = gBankTarget;
				gNewBS->DancerBankCount = 0;

				for (i = 0; i < gBattlersCount; ++i)
					gNewBS->DancerTurnOrder[i] = i;

				SortBanksBySpeed(gNewBS->DancerTurnOrder, TRUE);
			}
			else if (!gNewBS->DancerInProgress)
			{
				gBattleScripting.atk49_state++;
				gNewBS->AttackerDidDamageAtLeastOnce = FALSE;
				gNewBS->attackAnimationPlayed = FALSE;
				gNewBS->moveWasBouncedThisTurn = FALSE;
				break;
			}

			u8 bank = gNewBS->DancerTurnOrder[gNewBS->DancerBankCount];

			if (ABILITY(bank) == ABILITY_DANCER
			&& !(gAbsentBattlerFlags & gBitTable[bank])
			&& bank != gNewBS->CurrentTurnAttacker)
			{
				gBankAttacker = bank;

				if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE
				&&  gNewBS->CurrentTurnAttacker == PARTNER(bank)
				&&  gBattleMoves[gCurrentMove].target == MOVE_TARGET_SELECTED
				&&  gBattleMons[gNewBS->CurrentTurnTarget].hp)
					gBankTarget = gNewBS->CurrentTurnTarget; //Target the same as partner's target
				else
					gBankTarget = GetMoveTarget(gCurrentMove, FALSE);

				gNewBS->preFaintEffectsState = 0;
				gBattleScripting.atk49_state = 0;
				gBattleStruct->atkCancellerTracker = 0;
				gNewBS->AttackerDidDamageAtLeastOnce = FALSE;
				gCalledMove = gCurrentMove;
				gBattleScripting.bank = gBankAttacker;
				gBattlescriptCurrInstr = BattleScript_DancerActivated;
				gHitMarker &= ~(HITMARKER_NO_ATTACKSTRING | HITMARKER_ATTACKSTRING_PRINTED);
				++gNewBS->DancerBankCount;
				return;
			}

			++gNewBS->DancerBankCount;
			break;

		case ATK49_RAID_SHIELD:
			if (IsRaidBattle() && ShouldCreateRaidShields(BANK_RAID_BOSS))
			{
				gNewBS->dynamaxData.raidShieldsUp = TRUE;
				gNewBS->dynamaxData.shieldsDestroyed = 0;
				gBattleScripting.bank = BANK_RAID_BOSS;
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_RaidShields;
				effect = 1;
			}

			gBattleScripting.atk49_state++;
			break;

		case ATK49_COUNT:
			break;
		}

		//Should I remove these lines?
		if (arg1 == 1 && effect == FALSE)
			gBattleScripting.atk49_state = ATK49_COUNT;
		else if (arg1 == 2 && arg2 == gBattleScripting.atk49_state)
			gBattleScripting.atk49_state = ATK49_COUNT;
		else if (arg1 == ARG_ONLY_EMERGENCY_EXIT)
			gBattleScripting.atk49_state = ATK49_COUNT;

	} while (gBattleScripting.atk49_state != ATK49_COUNT && effect == FALSE);

	if (gBattleScripting.atk49_state == ATK49_COUNT && effect == FALSE)
		gBattlescriptCurrInstr += 3;
}

static const bank_t gTargetsByBank[4][4] =
{
	{B_POSITION_OPPONENT_LEFT, B_POSITION_OPPONENT_RIGHT, B_POSITION_PLAYER_RIGHT, 0xFF},	//Bank 0 - Player Left
	{B_POSITION_PLAYER_LEFT, B_POSITION_PLAYER_RIGHT, B_POSITION_OPPONENT_RIGHT, 0xFF}, 	//Bank 1 - Opponent Left
	{B_POSITION_OPPONENT_LEFT, B_POSITION_OPPONENT_RIGHT, B_POSITION_PLAYER_LEFT, 0xFF}, 	//Bank 2 - Player Right
	{B_POSITION_PLAYER_LEFT, B_POSITION_PLAYER_RIGHT, B_POSITION_OPPONENT_LEFT, 0xFF}  		//Bank 3 - Opponent Right
};

bank_t GetNextMultiTarget(void)
{
	u8 pos = gTargetsByBank[gBankAttacker][gNewBS->OriginalAttackerTargetCount];
	if (pos != 0xFF)
		return GetBattlerAtPosition(pos);

	return pos;
}

static bool8 CanDoMoveEndSwitchout(u8 arg)
{
	return arg != ARG_IN_PURSUIT
		&& arg != ARG_DRAGON_TAIL
		&& arg != ARG_PARTING_SHOT
		&& (SPLIT(gCurrentMove) != SPLIT_STATUS || gBattleMoves[gCurrentMove].effect != EFFECT_ROAR); //Dragon Tail & Circle Throw are handled in line above
}

void SortBanksBySpeed(u8 banks[], bool8 slowToFast)
{
	int i, j, key, keyBank;
	u16 speeds[4] = {0};
	for (i = 0; i < gBattlersCount; ++i)
		speeds[i] = SpeedCalc(banks[i]);

	for (i = 1; i < gBattlersCount; ++i)
	{
		keyBank = banks[i];
		key = speeds[i];
		j = i - 1;

		if (slowToFast)
		{
			while (j >= 0 && speeds[j] > key)
			{
				banks[j + 1] = banks[j];
				speeds[j + 1] = speeds[j];
				j = j - 1;
			}
		}
		else
		{
			while (j >= 0 && speeds[j] < key)
			{
				banks[j + 1] = banks[j];
				speeds[j + 1] = speeds[j];
				j = j - 1;
			}
		}

		banks[j + 1] = keyBank;
		speeds[j + 1] = key;
	}
}
