#include "defines.h"
#include "defines_battle.h"
#include "../include/battle_string_ids.h"
#include "../include/event_data.h"
#include "../include/random.h"
#include "../include/util.h"
#include "../include/constants/flags.h"
#include "../include/constants/items.h"

#include "../include/new/ability_tables.h"
#include "../include/new/ability_util.h"
#include "../include/new/accuracy_calc.h"
#include "../include/new/ai_switching.h"
#include "../include/new/attackcanceler.h"
#include "../include/new/attackcanceler_battle_scripts.h"
#include "../include/new/battle_start_turn_start.h"
#include "../include/new/battle_util.h"
#include "../include/new/damage_calc.h"
#include "../include/new/dynamax.h"
#include "../include/new/form_change.h"
#include "../include/new/frontier.h"
#include "../include/new/general_bs_commands.h"
#include "../include/new/item.h"
#include "../include/new/move_tables.h"
#include "../include/new/util.h"
/*
attackcanceler.c
	Handles any logic for discerning if a move can be used or is effective, eg. mold breaker, flinch status, truant, etc.
*/

//TODO: Make sure Powder stops Inferno Overdrive and not Pledge moves

#ifdef UNBOUND
typedef bool8 (*IsIngameTradeMon_T) (struct Pokemon* mon);
#define IsIngameTradeMon ((IsIngameTradeMon_T) (0x801D86C |1)) //From Battle Tower Scripts
#endif

//This file's functions:
static u8 AtkCanceller_UnableToUseMove(void);
static bool8 TryActivateMagicCoat(u8 bankDef, u8 bankAtk, u16 currentMove);
static bool8 TryActivateMagicBounce(u8 bankDef);
static u8 IsMonDisobedient(void);
static bool8 IsTargetUnaffectedByPrankster(u8 bankAtk, u8 bankDef, u16 currentMove, u16 chosenMove);

void atk00_attackcanceler(void)
{
	u32 i, moveTarget;

	if (gBattleOutcome != 0)
	{
		gCurrentActionFuncId = ACTION_FINISHED;
		return;
	}

	ResetBestMonToSwitchInto(gBankAttacker); //So the AI recalculates the Pokemon to switch into for moves like U-Turn

	if ((gBattleMons[gBankAttacker].hp == 0
	|| (gStatuses3[gBankAttacker] & STATUS3_SKY_DROP_TARGET)
	|| (gNewBS->NoMoreMovingThisTurn & gBitTable[gBankAttacker])) //From Smack Down/Gravity
		&& !(gHitMarker & HITMARKER_NO_ATTACKSTRING))
	{
		gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
		gBattlescriptCurrInstr = BattleScript_MoveEnd;
		return;
	}

	if (IS_MOLD_BREAKER(ABILITY(gBankAttacker), gCurrentMove) || gNewBS->dynamaxData.nullifiedStats) //There is a Mold Breaker
	{
		if (!gNewBS->dontActivateMoldBreakersAnymoreThisTurn) //Like after Neutralizing Gas disappears during a spread move
		{
			for (i = 0; i < gBattlersCount; ++i)
			{
				if (i != gBankAttacker
				&& ((gSpecialAbilityFlags[ABILITY(i)].gMoldBreakerIgnoredAbilities && !SpeciesHasMyceliumMight(SPECIES(gBankAttacker)))
				|| (gSpecialAbilityFlags[ABILITY(i)].gMyceliumMighIgnoredAbilities && SpeciesHasMyceliumMight(SPECIES(gBankAttacker)) && SPLIT(gCurrentMove) == SPLIT_STATUS)))
				{
					gNewBS->DisabledMoldBreakerAbilities[i] = gBattleMons[i].ability; //Temporarily disable all relevant abilities on the field
					gBattleMons[i].ability = ABILITY_NONE;
				}
			}
		}
	}

	gNewBS->originalAttackerBackup = gBankAttacker;
	gNewBS->originalTargetBackup = gBankTarget;

	if (AtkCanceller_UnableToUseMove())
		return;

	moveTarget = GetBaseMoveTarget(gCurrentMove, gBankAttacker);
	if (!BATTLER_ALIVE(gBankTarget)
	&& AttacksThisTurn(gBankAttacker, gCurrentMove) == 2 //Not charging move
	&& gBattleMoves[gCurrentMove].effect != EFFECT_EXPLOSION //Exploding moves still KO the attacker
	&& !(moveTarget & MOVE_TARGET_OPPONENTS_FIELD) //Moves like Stealth Rock can still be used
	&& !(SPLIT(gCurrentMove) == SPLIT_STATUS && moveTarget & MOVE_TARGET_DEPENDS)) //Status moves like Metronome can still be used
	{
		CancelMultiTurnMoves(gBankAttacker);
		gBattlescriptCurrInstr = BattleScript_ButItFailed - 2;
		return;
	}
	else if (!ProtectAffects(gCurrentMove, gBankAttacker, gBankTarget, FALSE)
	&& !MissesDueToSemiInvulnerability(gBankAttacker, gBankTarget, gCurrentMove)
	&& (AbilityBattleEffects(ABILITYEFFECT_MOVES_BLOCK, gBankTarget, 0, 0, 0)
	|| (IS_DOUBLE_BATTLE && AbilityBattleEffects(ABILITYEFFECT_MOVES_BLOCK_PARTNER, PARTNER(gBankTarget), 0, 0, 0))))
		return;

	else if (!gNewBS->ParentalBondOn
	&& gNewBS->OriginalAttackerTargetCount == 0
	&& ABILITY(gBankAttacker) == ABILITY_PARENTALBOND
	&& IsMoveAffectedByParentalBond(gCurrentMove, gBankAttacker)
	&& !(gAbsentBattlerFlags & gBitTable[gBankTarget]))
	{
		gNewBS->ParentalBondOn = 2;
		gMultiHitCounter = 2;
		PREPARE_BYTE_NUMBER_BUFFER(gBattleScripting.multihitString, 1, 0);
	}

	if (gBattleMons[gBankAttacker].pp[gCurrMovePos] == 0
	&& gCurrentMove != MOVE_STRUGGLE
	&& !(gHitMarker & (HITMARKER_x800000 | HITMARKER_NO_ATTACKSTRING))
	&& !(gBattleMons[gBankAttacker].status2 & STATUS2_MULTIPLETURNS)
	&& !gNewBS->DancerInProgress)
	{
		gBattlescriptCurrInstr = BattleScript_NoPPForMove;
		gMoveResultFlags |= MOVE_RESULT_MISSED;
		return;
	}

	gHitMarker &= ~(HITMARKER_x800000);

	if (MoveIgnoresSubstitutes(gCurrentMove, ABILITY(gBankAttacker)))
		gNewBS->bypassSubstitute = TRUE;

	if (gNewBS->MoveBounceInProgress == 2) //Bounce just ended
		gNewBS->MoveBounceInProgress = 0;

	if (!gNewBS->MoveBounceInProgress
	&& gBattleMoves[gCurrentMove].flags & FLAG_MAGIC_COAT_AFFECTED
	&& !(moveTarget & MOVE_TARGET_ALL)) //Safety measure; no default moves allow this
	{
		if (moveTarget == MOVE_TARGET_OPPONENTS_FIELD)
		{
			u8 foe1 = SIDE(gBankAttacker) ^ BIT_SIDE;
			u8 foe2 = PARTNER(foe1);
		
			if (TryActivateMagicCoat(foe1, gBankAttacker, gCurrentMove)
			|| TryActivateMagicCoat(foe2, gBankAttacker, gCurrentMove)
			|| TryActivateMagicBounce(foe1)
			|| TryActivateMagicBounce(foe2))
				return;
		}
		else
		{
			if (TryActivateMagicCoat(gBankTarget, gBankAttacker, gCurrentMove)
			|| TryActivateMagicBounce(gBankTarget))
				return;
		}
	}

	for (i = 0; i < gBattlersCount; i++)
	{
		if ((gProtectStructs[gBanksByTurnOrder[i]].stealMove) && gBattleMoves[gCurrentMove].flags & FLAG_SNATCH_AFFECTED)
		{
			PressurePPLose(gBankAttacker, gBanksByTurnOrder[i], MOVE_SNATCH);
			gProtectStructs[gBanksByTurnOrder[i]].stealMove = 0;
			gBattleScripting.bank = gBanksByTurnOrder[i];
			BattleScriptPushCursor();
			gBattlescriptCurrInstr = BattleScript_SnatchedMove;
			return;
		}
	}

	if (gSpecialStatuses[gBankTarget].lightningRodRedirected)
	{
		gSpecialStatuses[gBankTarget].lightningRodRedirected = 0;
		gBattleScripting.bank = gBankTarget;
		BattleScriptPushCursor();

		if (BankHasEvaporate(gBankTarget))
			gBattlescriptCurrInstr = BattleScript_EvaporatedAttack;
		else
			gBattlescriptCurrInstr = BattleScript_TookAttack;
	}
	else if (ProtectAffects(gCurrentMove, gBankAttacker, gBankTarget, FALSE)
	 && (gCurrentMove != MOVE_CURSE || IsOfType(gBankAttacker, TYPE_GHOST))
	 && ((!IsTwoTurnsMove(gCurrentMove) || (gBattleMons[gBankAttacker].status2 & STATUS2_MULTIPLETURNS))))
	{
		if (gBattleMoves[gCurrentMove].effect != EFFECT_RAMPAGE) //These are handled in CMD49
			CancelMultiTurnMoves(gBankAttacker);

		gMoveResultFlags |= MOVE_RESULT_MISSED;
		gLastLandedMoves[gBankTarget] = 0;
		gLastHitByType[gBankTarget] = 0;
		gBattleCommunication[6] = 1;
		gBattlescriptCurrInstr++;
	}
	else
	{
		gBattlescriptCurrInstr++;
	}
}

static u8 AtkCanceller_UnableToUseMove(void)
{
	int i, j;
	u8 effect = 0;

	do
	{
		switch (gBattleStruct->atkCancellerTracker) {
		case CANCELLER_FLAGS: // flags clear
			gBattleMons[gBankAttacker].status2 &= ~(STATUS2_DESTINY_BOND);
			gStatuses3[gBankAttacker] &= ~(STATUS3_GRUDGE);
			gStatuses3[gBankAttacker] &= ~(STATUS3_GLAIVERUSH);
			gBattleScripting.tripleKickPower = 0;
			gNewBS->ai.zMoveHelper = 0;
			gBattleStruct->atkCancellerTracker++;
			break;

		case CANCELLER_ASLEEP: // check being asleep
			if (gBattleMons[gBankAttacker].status1 & STATUS1_SLEEP)
			{
				u8 toSub;
				if (ABILITY(gBankAttacker) == ABILITY_EARLYBIRD)
					toSub = 2;
				else
					toSub = 1;

				if ((gBattleMons[gBankAttacker].status1 & STATUS1_SLEEP) < toSub)
					gBattleMons[gBankAttacker].status1 &= ~(STATUS1_SLEEP);
				else
					gBattleMons[gBankAttacker].status1 -= toSub;

				if (gBattleMons[gBankAttacker].status1 & STATUS1_SLEEP)
				{
					if (gCurrentMove != MOVE_SNORE && gCurrentMove != MOVE_SLEEPTALK)
					{
						if (BATTLER_SEMI_INVULNERABLE(gBankAttacker)) //Caused by Yawn
							CancelMultiTurnMoves(gActiveBattler = gBankAttacker);

						gBattlescriptCurrInstr = BattleScript_MoveUsedIsAsleep;
						gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
						effect = 2;
					}
					else
						effect = 2;
				}
				else
				{
					gBattleMons[gBankAttacker].status2 &= ~(STATUS2_NIGHTMARE);
					BattleScriptPushCursor();
					gBattleCommunication[MULTISTRING_CHOOSER] = 0;
					gBattlescriptCurrInstr = BattleScript_MoveUsedWokeUp;
					effect = 2;
				}
			}
			gBattleStruct->atkCancellerTracker++;
			break;

		case CANCELLER_FROZEN:
			#ifndef FROSTBITE
			if (gBattleMons[gBankAttacker].status1 & STATUS1_FREEZE)
			{
				if (umodsi(Random(), 5))
				{
					if (!gSpecialMoveFlags[gCurrentMove].gMovesCanUnfreezeAttacker || gMoveResultFlags & MOVE_RESULT_FAILED) // unfreezing via a move effect happens in case 13
					{
						gBattlescriptCurrInstr = BattleScript_MoveUsedIsFrozen;
						gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
					}
					else
					{
						gBattleStruct->atkCancellerTracker++;
						break;
					}
				}
				else // unfreeze
				{
					gBattleMons[gBankAttacker].status1 &= ~(STATUS1_FREEZE);
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_MoveUsedUnfroze;
					gBattleCommunication[MULTISTRING_CHOOSER] = 0;
				}
				effect = 2;
			}
			#endif
			gBattleStruct->atkCancellerTracker++;
			break;

		case CANCELLER_TRUANT: // truant
			if (ABILITY(gBankAttacker) == ABILITY_TRUANT && gDisableStructs[gBankAttacker].truantCounter)
			{
				CancelMultiTurnMoves(gBankAttacker);
				gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
				gBattleCommunication[MULTISTRING_CHOOSER] = 4;
				gBattleScripting.bank = gBankAttacker;
				gBattlescriptCurrInstr = BattleScript_MoveUsedLoafingAround;
				gMoveResultFlags |= MOVE_RESULT_MISSED;
				effect = 1;
			}
			gBattleStruct->atkCancellerTracker++;
			break;

		case CANCELLER_RECHARGE: // recharge
			if (gBattleMons[gBankAttacker].status2 & STATUS2_RECHARGE)
			{
				gNewBS->MetronomeCounter[gBankAttacker] = MathMin(100, gNewBS->MetronomeCounter[gBankAttacker] + 20);
				gBattleMons[gBankAttacker].status2 &= ~(STATUS2_RECHARGE);
				gDisableStructs[gBankAttacker].rechargeTimer = 0;
				CancelMultiTurnMoves(gBankAttacker);
				gBattlescriptCurrInstr = BattleScript_MoveUsedMustRecharge;
				gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
				effect = 1;
			}
			gBattleStruct->atkCancellerTracker++;
			break;

		case CANCELLER_FLINCH: // flinch
			if (gBattleMons[gBankAttacker].status2 & STATUS2_FLINCHED && !IsDynamaxed(gBankAttacker))
			{
				gProtectStructs[gBankAttacker].flinchImmobility = 1;
				CancelMultiTurnMoves(gBankAttacker);
				gBattlescriptCurrInstr = BattleScript_MoveUsedFlinched;
				gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
				effect = 1;
			}
			gBattleStruct->atkCancellerTracker++;
			break;

		case CANCELLER_RAID_BATTLE_NULLIFICATION:
			if (IsRaidBattle()
			&& gBankAttacker == BANK_RAID_BOSS
			&& Random() % 100 < GetRaidBattleStatNullificationChance(gBankAttacker))
			{
				for (i = 0; i < gBattlersCount; ++i)
				{
					if (i == gBankAttacker || !BATTLER_ALIVE(i))
						continue;

					for (j = STAT_STAGE_ATK; j < BATTLE_STATS_NO; ++j)
						STAT_STAGE(i, j) = 6;
				}

				gNewBS->dynamaxData.nullifiedStats = TRUE;
				gSpecialStatuses[gBankTarget].lightningRodRedirected = 0; //The target will still be changed, but this is the best I can do

				gActiveBattler = gBankAttacker;
				gBattleMons[gActiveBattler].status1 = 0; //Clear non-volatile status condition
				EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[gActiveBattler].status1);
				MarkBufferBankForExecution(gActiveBattler);

				BattleScriptPushCursor();
				gBattleScripting.bank = gBankAttacker;
				gBattlescriptCurrInstr = BattleScript_RaidBattleStatNullification;
				effect = 1;
			}
			gBattleStruct->atkCancellerTracker++;
			break;

		case CANCELLER_DEVOLVE:
			if (gNewBS->devolveForgotMove & gBitTable[gBankAttacker])
			{
				gNewBS->devolveForgotMove &= ~(gBitTable[gBankAttacker]);
				CancelMultiTurnMoves(gBankAttacker);
				gBattlescriptCurrInstr = BattleScript_MoveUsedDevolvedForgot;
				gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
				effect = 1;
			}
			gBattleStruct->atkCancellerTracker++;
			break;

		case CANCELLER_RAID_BATTLES_FAILED_MOVES:
		case CANCELLER_RAID_BATTLES_FAILED_MOVES_2:
			if ((IsRaidBattle() 
			&& gSpecialMoveFlags[gCurrentMove].gRaidBattleBannedMoves
			&& !gNewBS->zMoveData.active)) //Raid Battles stop status Z-Moves, so there will be a second check later on
			{
				gBattlescriptCurrInstr = BattleScript_MoveUsedRaidBattlePrevents;
				effect = 1;
			}
			else if (gCurrentMove == MOVE_TRANSFORM && HasRaidShields(gBankTarget)) //Shields can be used outside Raid Battles now
			{
				gBattlescriptCurrInstr = BattleScript_MoveUsedRaidShieldPrevents;
				effect = 1;
			}

			if (effect)
			{
				gBattleScripting.bank = gBankAttacker;
				CancelMultiTurnMoves(gBankAttacker);
				gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
			}

			gBattleStruct->atkCancellerTracker++;
			break;

		case CANCELLER_DYNAMAX_FAILED_MOVES:
			if ((IsDynamaxed(gBankTarget)
			 && gSpecialMoveFlags[gCurrentMove].gDynamaxBannedMoves
			 && !gNewBS->zMoveData.active) //Dynamax Pokemon stop status Z-Moves, so there will be a second check later on
			|| (IsRaidBattle() && GetBattlerPosition(gBankAttacker) == B_POSITION_OPPONENT_LEFT && gSpecialMoveFlags[gCurrentMove].gRaidBattleBannedRaidMonMoves))
			{
				gBattleScripting.bank = gBankAttacker;
				CancelMultiTurnMoves(gBankAttacker);
				gBattlescriptCurrInstr = BattleScript_MoveUsedDynamaxPrevents;
				gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
				effect = 1;
			}

			gBattleStruct->atkCancellerTracker++;
			break;

		case CANCELLER_BANNED_SIGNATURE_MOVE:
		case CANCELLER_BANNED_SIGNATURE_MOVE_2: ;
			u16 species = SPECIES(gBankAttacker);

			switch (gCurrentMove) {
				case MOVE_DARKVOID:
					if (!gNewBS->zMoveData.active || gNewBS->zMoveData.effectApplied) //Allows for Z-Dark Void to still have Z-Effect
					{
						#ifdef SPECIES_DARKRAI
						if (species != SPECIES_DARKRAI && !gNewBS->MoveBounceInProgress)
							effect = 1;
						#endif
					}
					break;
				case MOVE_HYPERSPACEFURY:
					#ifdef SPECIES_HOOPA_UNBOUND
					if (species != SPECIES_HOOPA_UNBOUND)
					{
						#ifdef SPECIES_HOOPA
						if (species == SPECIES_HOOPA)
							effect = 2;
						else
						#endif
							effect = 1;
					}
					#endif
					break;
			}

			if (effect)
			{
				gBattleScripting.bank = gBankAttacker;
				CancelMultiTurnMoves(gBankAttacker);

				gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;

				if (effect == 2)
					gBattlescriptCurrInstr = BattleScript_HoopaCantUseHyperspaceFury;
				else
					gBattlescriptCurrInstr = BattleScript_CantUseSignatureMove;
			}
			gBattleStruct->atkCancellerTracker++;
			break;

		case CANCELLER_DISABLED: // disabled move
			if (gDisableStructs[gBankAttacker].disabledMove == gCurrentMove
			&& gDisableStructs[gBankAttacker].disabledMove != 0
			&& !gNewBS->zMoveData.active
			&& !IsAnyMaxMove(gCurrentMove))
			{
				gProtectStructs[gBankAttacker].usedDisabledMove = 1;
				gBattleScripting.bank = gBankAttacker;
				CancelMultiTurnMoves(gBankAttacker);
				gBattlescriptCurrInstr = BattleScript_MoveUsedIsDisabled;
				gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
				effect = 1;
			}
			gBattleStruct->atkCancellerTracker++;
			break;

		case CANCELLER_HEAL_BLOCKED:
		case CANCELLER_HEAL_BLOCKED_2:
			if (IsHealBlocked(gBankAttacker)
			&& CheckHealingMove(gCurrentMove)
			&& !gNewBS->zMoveData.active
			&& !IsAnyMaxMove(gCurrentMove))
			{
				gBattleScripting.bank = gBankAttacker;
				CancelMultiTurnMoves(gBankAttacker);
				gBattlescriptCurrInstr = BattleScript_MoveUsedHealBlockPrevents;
				gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
				effect = 1;
			}
			gBattleStruct->atkCancellerTracker++;
			break;

		case CANCELLER_THROAT_CHOP:
		case CANCELLER_THROAT_CHOP_2:
			if (CantUseSoundMoves(gBankAttacker)
			&& CheckSoundMove(gCurrentMove)
			&& !gNewBS->zMoveData.active
			&& !IsAnyMaxMove(gCurrentMove))
			{
				gBattleScripting.bank = gBankAttacker;
				CancelMultiTurnMoves(gBankAttacker);
				gBattlescriptCurrInstr = BattleScript_MoveUsedThroatChopPrevents;
				gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
				effect = 1;
			}
			gBattleStruct->atkCancellerTracker++;
			break;

		case CANCELLER_GRAVITY:
		case CANCELLER_GRAVITY_2:
			if (IsGravityActive()
			&& gSpecialMoveFlags[gCurrentMove].gGravityBannedMoves
			&& !gNewBS->zMoveData.active //Gravity stops Z-Moves, so there will be a second check later on
			&& !IsAnyMaxMove(gCurrentMove))
			{
				gBattleScripting.bank = gBankAttacker;
				CancelMultiTurnMoves(gBankAttacker);
				gBattlescriptCurrInstr = BattleScript_MoveUsedGravityPrevents;
				gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
				effect = 1;
			}

			gBattleStruct->atkCancellerTracker++;
			break;

		case CANCELLER_TAUNTED: // taunt
			if (IsTaunted(gBankAttacker)
			&& SPLIT(gCurrentMove) == SPLIT_STATUS
			&& !gNewBS->zMoveData.active
			&& !IsAnyMaxMove(gCurrentMove))
			{
				gProtectStructs[gBankAttacker].usedTauntedMove = 1;
				CancelMultiTurnMoves(gBankAttacker);
				gBattlescriptCurrInstr = BattleScript_MoveUsedIsTaunted;
				gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
				effect = 1;
			}
			gBattleStruct->atkCancellerTracker++;
			break;

		case CANCELLER_IMPRISONED: // imprisoned
			if (IsImprisoned(gBankAttacker, gCurrentMove)
			&& !gNewBS->zMoveData.active
			&& !IsAnyMaxMove(gCurrentMove))
			{
				gProtectStructs[gBankAttacker].usedImprisonedMove = 1;
				CancelMultiTurnMoves(gBankAttacker);
				gBattlescriptCurrInstr = BattleScript_MoveUsedIsImprisoned;
				gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
				effect = 1;
			}
			gBattleStruct->atkCancellerTracker++;
			break;

		case CANCELLER_CONFUSED: // confusion
			if (IsConfused(gBankAttacker))
			{
				if (gBattleMons[gBankAttacker].status2 & STATUS2_CONFUSION)
					gBattleMons[gBankAttacker].status2--;

				if (IsConfused(gBankAttacker))
				{
					#ifdef OLD_CONFUSION_CHANCE
					if (Random() % 100 < 50) //50 %
					{
						gBattleCommunication[MULTISTRING_CHOOSER] = 0;
						BattleScriptPushCursor();
					}
					#else
					if (Random() % 100 > 33) //33 %
					{
						gBattleCommunication[MULTISTRING_CHOOSER] = 0;
						BattleScriptPushCursor();
					}
					#endif
					else // confusion dmg
					{
						gBattleCommunication[MULTISTRING_CHOOSER] = 1;
						gBankTarget = gBankAttacker;
						gBattleMoveDamage = ConfusionDamageCalc();
						gProtectStructs[gBankAttacker].confusionSelfDmg = 1;
						gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
						gNewBS->breakDisguiseSpecialDmg = TRUE;
					}
					gBattlescriptCurrInstr = BattleScript_MoveUsedIsConfused;
				}
				else // snapped out of confusion
				{
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_MoveUsedIsConfusedNoMore;
				}
				effect = 1;
			}
			gBattleStruct->atkCancellerTracker++;
			break;

		case CANCELLER_PARALYSED: // paralysis
			if ((gBattleMons[gBankAttacker].status1 & STATUS1_PARALYSIS) && Random() % 4 == 0)
			{
				gProtectStructs[gBankAttacker].prlzImmobility = 1;
				CancelMultiTurnMoves(gBankAttacker);
				gBattlescriptCurrInstr = BattleScript_MoveUsedIsParalyzed;
				gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
				effect = 1;
			}
			gBattleStruct->atkCancellerTracker++;
			break;

		case CANCELLER_GHOST: //Lavender Town Ghost battle
		#ifndef NO_GHOST_BATTLES
			if ((gBattleTypeFlags & (BATTLE_TYPE_SCRIPTED_WILD_1 | BATTLE_TYPE_GHOST)) == BATTLE_TYPE_GHOST)
			{
				if (SIDE(gBankAttacker) == B_SIDE_PLAYER)
					gBattlescriptCurrInstr = BattleScript_TooScaredToMove;
				else
					gBattlescriptCurrInstr = BattleScript_GhostGetOut;
				gBattleCommunication[MULTISTRING_CHOOSER] = 0;
				gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
				effect = 1;
			}
		#endif
			gBattleStruct->atkCancellerTracker++;
			break;

		case CANCELLER_IN_LOVE: // infatuation
			if (gBattleMons[gBankAttacker].status2 & STATUS2_INFATUATION)
			{
				gBattleScripting.bank = CountTrailingZeroBits((gBattleMons[gBankAttacker].status2 & STATUS2_INFATUATION) >> 0x10);
				if (Random() % 100 < 50) //Use % to help increase randomness
				{
					BattleScriptPushCursor();
				}
				else
				{
					BattleScriptPush(BattleScript_ImmobilizedByLove);
					gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
					gProtectStructs[gBankAttacker].loveImmobility = 1;
					CancelMultiTurnMoves(gBankAttacker);
				}
				gBattlescriptCurrInstr = BattleScript_MoveUsedIsInLove;
				effect = 1;
			}
			gBattleStruct->atkCancellerTracker++;
			break;

		case CANCELLER_OBEDIENCE:
			if (gBattleMons[gBankAttacker].status2 & STATUS2_MULTIPLETURNS)
				gHitMarker |= HITMARKER_OBEYS; //Second turn always obeys
			else if (!(gHitMarker & HITMARKER_OBEYS))
			{
				switch (IsMonDisobedient()) {
					case 0:
						gHitMarker |= HITMARKER_OBEYS;
						break;
					case 2:
						gHitMarker |= HITMARKER_OBEYS;
						effect = 1;
						break;
					default:
						gMoveResultFlags |= MOVE_RESULT_MISSED;
						effect = 1;
				}
			}

			gBattleStruct->atkCancellerTracker++;
			break;

		case CANCELLER_STANCE_CHANGE:
		case CANCELLER_STANCE_CHANGE_2:
			#if (defined SPECIES_AEGISLASH && defined SPECIES_AEGISLASH_BLADE)
			if (ABILITY(gBankAttacker) == ABILITY_STANCECHANGE && !(gBattleMons[gBankAttacker].status2 & STATUS2_TRANSFORMED))
			{
				switch (gBattleMons[gBankAttacker].species)
				{
					case SPECIES_AEGISLASH:
						if (SPLIT(gCurrentMove) != SPLIT_STATUS)
						{
							DoFormChange(gBankAttacker, SPECIES_AEGISLASH_BLADE, FALSE, TRUE, FALSE);
							BattleScriptPushCursor();
							gBattlescriptCurrInstr = BattleScript_StanceChangeToBlade;
							gBattleScripting.bank = gBankAttacker;
							effect = 1;
						}
						break;

					case SPECIES_AEGISLASH_BLADE:
						if (gCurrentMove == MOVE_KINGSSHIELD)
						{
							DoFormChange(gBankAttacker, SPECIES_AEGISLASH, FALSE, TRUE, FALSE);
							BattleScriptPushCursor();
							gBattlescriptCurrInstr = BattleScript_StanceChangeToShield;
							gBattleScripting.bank = gBankAttacker;
							effect = 1;
						}
						break;
				}
			}
			#endif
			gBattleStruct->atkCancellerTracker++;
			break;

		case CANCELLER_BIDE: // bide
			if (gBattleMons[gBankAttacker].status2 & STATUS2_BIDE)
			{
				gBattleMons[gBankAttacker].status2 -= 0x100;
				if (gBattleMons[gBankAttacker].status2 & STATUS2_BIDE)
				{
					gBattlescriptCurrInstr = BattleScript_BideStoringEnergy;
				}
				else
				{
					gBattleMons[gBankAttacker].status2 &= ~(STATUS2_MULTIPLETURNS);
					if (gTakenDmg[gBankAttacker])
					{
						gCurrentMove = MOVE_BIDE;
						gBattleScripting.bideDmg = gTakenDmg[gBankAttacker] * 2;
						gBankTarget = gTakenDmgBanks[gBankAttacker];
						if (gAbsentBattlerFlags & gBitTable[gBankTarget])
							gBankTarget = GetMoveTarget(MOVE_BIDE, 1);
						gBattlescriptCurrInstr = BattleScript_BideAttack;
					}
					else
					{
						gBattlescriptCurrInstr = BattleScript_BideNoEnergyToAttack;
					}
				}
				effect = 1;
			}
			gBattleStruct->atkCancellerTracker++;
			break;

		case CANCELLER_THAW: // move thawing
			if (gBattleMons[gBankAttacker].status1 & STATUS1_FREEZE)
			{
				if (gSpecialMoveFlags[gCurrentMove].gMovesCanUnfreezeAttacker
				&& !(gMoveResultFlags & MOVE_RESULT_FAILED)) //When Burn Up fails, it can't unfreeze
				{
					gBattleMons[gBankAttacker].status1 &= ~(STATUS1_FREEZE);
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_MoveUsedUnfroze;
					gBattleCommunication[MULTISTRING_CHOOSER] = 1;
				}
				effect = 2;
			}
			gBattleStruct->atkCancellerTracker++;
			break;

		case CANCELLER_Z_MOVES:
			if (gNewBS->zMoveData.active)
			{
				RecordItemEffectBattle(gBankAttacker, ITEM_EFFECT_Z_CRYSTAL);
				gNewBS->zMoveData.used[gBankAttacker] = TRUE;
				gNewBS->zMoveData.toBeUsed[gBankAttacker] = 0;
				gNewBS->zMoveData.partyIndex[gBankAttacker] |= gBitTable[gBattlerPartyIndexes[gBankAttacker]]; //Stops Rayquaza from Mega Evolving

				if (IsDoubleBattle())
				{
					if (SIDE(gBankAttacker) == B_SIDE_PLAYER && !(gBattleTypeFlags & (BATTLE_TYPE_INGAME_PARTNER | BATTLE_TYPE_MULTI))) //In team Battles, both players can use Z-moves
					{
						gNewBS->zMoveData.used[PARTNER(gBankAttacker)] = TRUE;
						gNewBS->zMoveData.toBeUsed[PARTNER(gBankAttacker)] = 0;
					}
					else if (SIDE(gBankAttacker) == B_SIDE_OPPONENT && !(gBattleTypeFlags & (BATTLE_TYPE_TWO_OPPONENTS | BATTLE_TYPE_MULTI)))
					{
						gNewBS->zMoveData.used[PARTNER(gBankAttacker)] = TRUE;
						gNewBS->zMoveData.toBeUsed[PARTNER(gBankAttacker)] = 0;
					}
				}

				gBattleScripting.bank = gBankAttacker;
				if (SPLIT(gCurrentMove) == SPLIT_STATUS)
				{
					if (!gNewBS->zMoveData.effectApplied)
					{
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_ZMoveActivateStatus;
						gNewBS->zMoveData.effect = gBattleMoves[gCurrentMove].z_move_effect;
						gNewBS->zMoveData.effectApplied = TRUE;
					}
					else
					{
						gBattleStruct->atkCancellerTracker++;
						break;
					}
				}
				else
				{
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_ZMoveActivateDamaging;
				}
				effect = 1;
			}
			gBattleStruct->atkCancellerTracker++;
			break;

		case CANCELLER_GRAVITY_Z_MOVES:
			if (IsGravityActive() && gSpecialMoveFlags[gCurrentMove].gGravityBannedMoves && gNewBS->zMoveData.active) //Gravity stops Z-Moves after they apply their effect
			{
				gBattleScripting.bank = gBankAttacker;
				CancelMultiTurnMoves(gBankAttacker);
				gBattlescriptCurrInstr = BattleScript_MoveUsedGravityPrevents;
				gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
				effect = 1;
			}
			gBattleStruct->atkCancellerTracker++;
			break;

		case CANCELLER_SKY_BATTLE:
		#ifdef FLAG_SKY_BATTLE
			if (FlagGet(FLAG_SKY_BATTLE) && gSpecialMoveFlags[gCurrentMove].gSkyBattleBannedMoves)
			{
				gBattleScripting.bank = gBankAttacker;
				CancelMultiTurnMoves(gBankAttacker);
				gBattlescriptCurrInstr = BattleScript_MoveUsedSkyBattlePrevents;
				gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
				effect = 1;
			}
			else
		#endif
			if (gBattleTypeFlags & BATTLE_TYPE_RING_CHALLENGE && IsMoveBannedInRingChallenge(gCurrentMove, gBankAttacker))
			{
				gBattleScripting.bank = gBankAttacker;
				CancelMultiTurnMoves(gBankAttacker);
				gBattlescriptCurrInstr = BattleScript_MoveUsedRingChallengePrevents;
				gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
				effect = 1;
			}
			gBattleStruct->atkCancellerTracker++;
			break;

		//Natural Gift fails before it can take Powder damage
		case CANCELLER_NATURAL_GIFT:
			if (gCurrentMove == MOVE_NATURALGIFT &&
			  (ABILITY(gBankAttacker) == ABILITY_KLUTZ
			 || !IsBerry(ITEM(gBankAttacker))
			 || IsMagicRoomActive()
			 || gNewBS->EmbargoTimers[gBankAttacker]))
			{
				gBattlescriptCurrInstr = BattleScript_ButItFailed - 2;
				gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
				effect = 1;
			}
			gBattleStruct->atkCancellerTracker++;
			break;

		case CANCELLER_DANCER:
			if (gNewBS->DancerInProgress)
			{
				if ((gBattleMons[gBankAttacker].status2 & STATUS2_MULTIPLETURNS)
				&&  gCurrentMove != gLockedMoves[gBankAttacker])
				{
					gBattlescriptCurrInstr = BattleScript_ButItFailed - 2;
					gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
					effect = 1;
				}
				else if (gDisableStructs[gBankAttacker].encoredMove
				&& gCurrentMove != gDisableStructs[gBankAttacker].encoredMove)
				{
					gBattlescriptCurrInstr = BattleScript_ButItFailed - 2;
					gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
					effect = 1;
				}
				else if (gBattleStruct->choicedMove[gBankAttacker]
				&& gCurrentMove != gBattleStruct->choicedMove[gBankAttacker])
				{
					gBattlescriptCurrInstr = BattleScript_ButItFailed - 2;
					gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
					effect = 1;
				}
			}
			gBattleStruct->atkCancellerTracker++;
			break;

		case CANCELLER_POWDER:
			if ((gNewBS->PowderByte & gBitTable[gBankAttacker]) && (gBattleStruct->dynamicMoveType == TYPE_FIRE))
			{
				gBattleMoveDamage = gBattleMons[gBankAttacker].maxHP / 4;
				gBattlescriptCurrInstr = BattleScript_MoveUsedPowderPrevents;
				gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
				effect = 1;
			}
			gBattleStruct->atkCancellerTracker++;
			break;

		case CANCELLER_PRIMAL_WEATHER:
			if (WEATHER_HAS_EFFECT && SPLIT(gCurrentMove) != SPLIT_STATUS) //Damaging moves only
			{
				if ((gBattleStruct->dynamicMoveType == TYPE_FIRE && gBattleWeather & WEATHER_RAIN_PRIMAL)
				||  (gBattleStruct->dynamicMoveType == TYPE_WATER && gBattleWeather & WEATHER_SUN_PRIMAL))
				{
					CancelMultiTurnMoves(gBankAttacker);
					gBattlescriptCurrInstr = BattleScript_MoveUsedFailedPrimalWeather;
					gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
					effect = 1;
				}
			}
			gBattleStruct->atkCancellerTracker++;
			break;

		case CANCELLER_PSYCHIC_TERRAIN:
			if (gTerrainType == PSYCHIC_TERRAIN
			&& CheckGrounding(gBankTarget)
			&& gBankAttacker != gBankTarget
			&& (IS_SINGLE_BATTLE || gBankTarget != PARTNER(gBankAttacker)) //Can still hit partner
			&& PriorityCalc(gBankAttacker, ACTION_USE_MOVE, gCurrentMove) > 0
			&& !gSpecialMoveFlags[gCurrentMove].gSpecialWholeFieldMoves
			&& !(gBattleMoves[gCurrentMove].target & MOVE_TARGET_OPPONENTS_FIELD) //Spikes are never affected
			&& !ProtectAffects(gCurrentMove, gBankAttacker, gBankTarget, FALSE)
			&& !MissesDueToSemiInvulnerability(gBankAttacker, gBankTarget, gCurrentMove))
			{
				if (IS_SINGLE_BATTLE || !(GetBaseMoveTarget(gCurrentMove, gBankAttacker) & (MOVE_TARGET_BOTH | MOVE_TARGET_ALL))) //Don't cancel moves that can hit two targets b/c one target might not be protected
					CancelMultiTurnMoves(gBankAttacker);
				gBattlescriptCurrInstr = BattleScript_MoveUsedPsychicTerrainPrevents;
				effect = 1;
			}
			gBattleStruct->atkCancellerTracker++;
			break;

		case CANCELLER_PRANKSTER: ;
			#ifndef OLD_PRANKSTER
			if (IsTargetUnaffectedByPrankster(gBankAttacker, gBankTarget, gCurrentMove, gChosenMove))
			{
				if (IS_SINGLE_BATTLE || !(GetBaseMoveTarget(gCurrentMove, gBankAttacker) & (MOVE_TARGET_BOTH | MOVE_TARGET_ALL))) //Don't cancel moves that can hit two targets b/c one target might not be protected
					CancelMultiTurnMoves(gBankAttacker);
	
				gBattleScripting.bank = gBankTarget;
				gBattlescriptCurrInstr = BattleScript_DarkTypePreventsPrankster;
				effect = 1;
			}
			#endif
			gBattleStruct->atkCancellerTracker++;
			break;

		case CANCELLER_EXPLODING_DAMP:
			if (gBattleMoves[gCurrentMove].effect == EFFECT_EXPLOSION || gCurrentMove == MOVE_MINDBLOWN)
			{
				u8 bank;

				for (bank = 0; bank < gBattlersCount; ++bank)
				{
					if (ABILITY(bank) == ABILITY_DAMP)
						break;
				}

				if (bank != gBattlersCount)
				{
					gBattleScripting.bank = bank;
					gBattlescriptCurrInstr = BattleScript_DampStopsExplosion;
					gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
					effect = 1;
				}
			}
			gBattleStruct->atkCancellerTracker++;
			break;

		case CANCELLER_MULTIHIT_MOVES:
			if (gSpecialMoveFlags[gCurrentMove].gTwoToFiveStrikesMoves)
			{
				u8 ability = ABILITY(gBankAttacker);

				if (gCurrentMove == MOVE_SURGINGSTRIKES)
				{
					gMultiHitCounter = 3;
				}
				else if (ability == ABILITY_SKILLLINK)
				{
					gMultiHitCounter = 5;
				}
				else if (ITEM_EFFECT(gBankAttacker) == ITEM_EFFECT_LOADED_DICE && gCurrentMove != MOVE_POPULATIONBOMB &&  gCurrentMove != MOVE_TRIPLEKICK &&  gCurrentMove != MOVE_TRIPLEAXEL)
				{
					gMultiHitCounter = 5;
				}
				#ifdef SPECIES_ASHGRENINJA
				else if (ability == ABILITY_BATTLEBOND
				&& gCurrentMove == MOVE_WATERSHURIKEN
				&& SPECIES(gBankAttacker) == SPECIES_ASHGRENINJA)
				{
					gMultiHitCounter = 3;
				}
				#endif
				#ifdef SPECIES_MAUSHOLD
				else if (gCurrentMove == MOVE_POPULATIONBOMB
				&& SPECIES(gBankAttacker) == SPECIES_MAUSHOLD)
				{
					gMultiHitCounter = Random() % 3; //Split into groups of 3
					switch (gMultiHitCounter)
					{
						case 0: //33 %
							gMultiHitCounter = 4;
							break;
						case 1: //33 %
							gMultiHitCounter = 6;
							break;
						case 2: //33 %
							if ((Random() & 1) == 0) //16.7 %
								gMultiHitCounter = 8;
							else //16.7 %
								gMultiHitCounter = 10;
							break;
					}
				}
				else
				#endif
				{
					gMultiHitCounter = Random() % 3; //Split into groups of 3
					switch (gMultiHitCounter)
					{
						case 0: //33 %
							gMultiHitCounter = 2;
							break;
						case 1: //33 %
							gMultiHitCounter = 3;
							break;
						case 2: //33 %
							if ((Random() & 1) == 0) //16.7 %
								gMultiHitCounter = 4;
							else //16.7 %
								gMultiHitCounter = 5;
							break;
					}
				}

				PREPARE_BYTE_NUMBER_BUFFER(gBattleScripting.multihitString, 1, 0)
			}
			else if (gSpecialMoveFlags[gCurrentMove].gTwoStrikesMoves)
			{
				gMultiHitCounter = 2;
				PREPARE_BYTE_NUMBER_BUFFER(gBattleScripting.multihitString, 1, 0)

				if (gCurrentMove == MOVE_DRAGONDARTS
				&& CanTargetPartner(gBankTarget)
				&& TargetFullyImmuneToCurrMove(gBankTarget))
				{
					//Smart target to partner
					gBankTarget = PARTNER(gBankTarget);
				}
			}
			else if (gBattleMoves[gCurrentMove].effect == EFFECT_TRIPLE_KICK)
			{
				gMultiHitCounter = 3;
				PREPARE_BYTE_NUMBER_BUFFER(gBattleScripting.multihitString, 1, 0)
			}
			else if (gBattleMoves[gCurrentMove].effect == EFFECT_BEAT_UP)
			{
				struct Pokemon* party;

				if (SIDE(gBankAttacker) == B_SIDE_PLAYER)
					party = gPlayerParty;
				else
					party = gEnemyParty;

				for (int i = 0; i < PARTY_SIZE; ++i)
				{
					u16 species = GetMonData(&party[i], MON_DATA_SPECIES2, 0);
					if (species != SPECIES_NONE
					&& species != SPECIES_EGG
					&& GetMonData(&party[i], MON_DATA_HP, 0)
					&& !GetMonData(&party[i], MON_DATA_STATUS, 0))
						++gMultiHitCounter;
				}

				gBattleCommunication[0] = 0; //For later
				PREPARE_BYTE_NUMBER_BUFFER(gBattleScripting.multihitString, 1, 0)
			}

			gBattleStruct->atkCancellerTracker++;
			break;

		case CANCELLER_MULTI_TARGET_MOVES:
			if (IS_DOUBLE_BATTLE)
			{
				const u8* backupScript = gBattlescriptCurrInstr; //Script can get overwritten by ability blocking
				u8 moveTarget = GetBaseMoveTarget(gCurrentMove, gBankAttacker);

				if (moveTarget & (MOVE_TARGET_BOTH | MOVE_TARGET_ALL)
				&& !gSpecialMoveFlags[gCurrentMove].gSpecialWholeFieldMoves)
				{
					u8 priority = PriorityCalc(gBankAttacker, ACTION_USE_MOVE, gCurrentMove);

					for (i = 0; i < gBattlersCount; ++i)
					{
						if (i != gBankAttacker && BATTLER_ALIVE(i)
						&& ((moveTarget & MOVE_TARGET_ALL) || i != PARTNER(gBankAttacker)) //Skip partner when not all-hitting move
						&& !ProtectAffects(gCurrentMove, gBankAttacker, i, FALSE)
						&& !MissesDueToSemiInvulnerability(gBankAttacker, i, gCurrentMove))
						{
							if (AbilityBattleEffects(ABILITYEFFECT_MOVES_BLOCK, i, 0, 0, 0)
							||  AbilityBattleEffects(ABILITYEFFECT_MOVES_BLOCK_PARTNER, PARTNER(i), 0, 0, 0)
							|| (gTerrainType == PSYCHIC_TERRAIN && CheckGrounding(i) && priority > 0))
							{
								gNewBS->ResultFlags[i] = 0;
								gNewBS->noResultString[i] = TRUE;
							}
							else if (AbilityBattleEffects(ABILITYEFFECT_ABSORBING, i, 0, 0, gCurrentMove))
							{
								gNewBS->ResultFlags[i] = 0;
								gNewBS->noResultString[i] = 2; //Indicator to factor in Accuracy checks
							}
							else
								gNewBS->ResultFlags[i] = TypeCalc(gCurrentMove, gBankAttacker, i, NULL);
						}
						else
						{
							gNewBS->ResultFlags[i] = MOVE_RESULT_NO_EFFECT; //You can't strike these targets
							gNewBS->noResultString[i] = TRUE;
						}
					}
				}

				gBattlescriptCurrInstr = backupScript; //Restore original script
				gActiveBattler = gBankAttacker;
				gNewBS->foeSpreadTargets = CountAliveMonsInBattle(BATTLE_ALIVE_DEF_SIDE, gBankAttacker, FOE(gBankAttacker));
				gNewBS->allSpreadTargets = CountAliveMonsInBattle(BATTLE_ALIVE_EXCEPT_ACTIVE, gBankAttacker, FOE(gBankAttacker));
			}
			gBattleStruct->atkCancellerTracker++;
			break;

		case CANCELLER_END:
			break;
		}

	} while (gBattleStruct->atkCancellerTracker != CANCELLER_END && effect == 0);

	if (effect == 2)
	{
		gActiveBattler = gBankAttacker;
		EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[gActiveBattler].status1);
		MarkBufferBankForExecution(gActiveBattler);
	}
	return effect;
}

static bool8 TryActivateMagicCoat(u8 bankDef, u8 bankAtk, u16 currentMove)
{
	if (gProtectStructs[bankDef].bounceMove)
	{
		PressurePPLose(bankAtk, bankDef, MOVE_MAGICCOAT);
		gProtectStructs[bankDef].bounceMove = 0;
		gNewBS->MoveBounceInProgress = TRUE;
		gNewBS->moveWasBouncedThisTurn = TRUE;
		
		if (IsTargetUnaffectedByPrankster(bankDef, bankAtk, currentMove, currentMove))
			gBattlescriptCurrInstr = BattleScript_DarkTypePreventsPrankster;
		
		BattleScriptPushCursor();
		gBattlescriptCurrInstr = BattleScript_MagicCoatBounce;
		return TRUE;
	}

	return FALSE;
}

static bool8 TryActivateMagicBounce(u8 bankDef)
{
	if (ABILITY(bankDef) == ABILITY_MAGICBOUNCE && !BATTLER_SEMI_INVULNERABLE(bankDef))
	{
		gNewBS->MoveBounceInProgress = TRUE;
		gNewBS->moveWasBouncedThisTurn = TRUE;
		gBattleScripting.bank = bankDef;
		BattleScriptPushCursor();
		gBattlescriptCurrInstr = BattleScript_MagicBounce;
		return TRUE;
	}

	return FALSE;
}

static u8 IsMonDisobedient(void)
{
	s32 rnd;
	s32 calc;
	u8 obedienceLevel = 0;

	if (gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_POKE_DUDE | BATTLE_TYPE_FRONTIER))
		return 0;
	else if (IsFrontierRaidBattle())
		return 0;
	else if (gBattleTypeFlags & (BATTLE_TYPE_INGAME_PARTNER) && GetBattlerPosition(gBankAttacker) == B_POSITION_PLAYER_RIGHT)
		return 0;
	else if (gBattleTypeFlags & (BATTLE_TYPE_MOCK_BATTLE) && GetBattlerPosition(gBankAttacker) == B_POSITION_PLAYER_LEFT)
		return 0;
	else if (SIDE(gBankAttacker) == B_SIDE_OPPONENT)
		return 0;
	else if (gNewBS->InstructInProgress || gNewBS->DancerInProgress || gNewBS->zMoveData.active || IsAnyMaxMove(gCurrentMove))
		return 0;
	#ifdef FLAG_USE_HALF_PARTNER_TEAM
	else if (FlagGet(FLAG_USE_HALF_PARTNER_TEAM))
		return 0; //Otherwise the partner's Pokemon might disobey
	#endif

	#ifdef DEBUG_OBEDIENCE
	return 0;
	#endif

	#ifndef OBEDIENCE_CHECK_FOR_PLAYER_ORIGINAL_POKEMON
	if (!IsOtherTrainer(gBattleMons[gBankAttacker].otId, gBattleMons[gBankAttacker].otName))
		return 0;
	#endif
	
	#ifdef INGAME_TRADE_MONS_ALWAYS_OBEY
	if (IsIngameTradeMon(GetBankPartyData(gBankAttacker)))
		return 0;
	#endif

	#ifdef OBEDIENCE_BY_BADGE_AMOUNT
		u8 badgeCount = 0;

		if (FlagGet(FLAG_BADGE08_GET))
			++badgeCount;
		if (FlagGet(FLAG_BADGE07_GET))
			++badgeCount;
		if (FlagGet(FLAG_BADGE06_GET))
			++badgeCount;
		if (FlagGet(FLAG_BADGE05_GET))
			++badgeCount;
		if (FlagGet(FLAG_BADGE04_GET))
			++badgeCount;
		if (FlagGet(FLAG_BADGE03_GET))
			++badgeCount;
		if (FlagGet(FLAG_BADGE02_GET))
			++badgeCount;
		if (FlagGet(FLAG_BADGE01_GET))
			++badgeCount;

		switch(badgeCount) {
			case 0:
				obedienceLevel = BASE_OBEDIENCE_LEVEL;
				break;
			case 1:
				obedienceLevel = BADGE_1_OBEDIENCE_LEVEL;
				break;
			case 2:
				obedienceLevel = BADGE_2_OBEDIENCE_LEVEL;
				break;
			case 3:
				obedienceLevel = BADGE_3_OBEDIENCE_LEVEL;
				break;
			case 4:
				obedienceLevel = BADGE_4_OBEDIENCE_LEVEL;
				break;
			case 5:
				obedienceLevel = BADGE_5_OBEDIENCE_LEVEL;
				break;
			case 6:
				obedienceLevel = BADGE_6_OBEDIENCE_LEVEL;
				break;
			case 7:
				obedienceLevel = BADGE_7_OBEDIENCE_LEVEL;
				break;
			default:
				return 0;
		}
	#else
		if (FlagGet(FLAG_BADGE08_GET))
			return 0;
		else if (FlagGet(FLAG_BADGE07_GET))
			obedienceLevel = BADGE_7_OBEDIENCE_LEVEL;
		else if (FlagGet(FLAG_BADGE06_GET))
			obedienceLevel = BADGE_6_OBEDIENCE_LEVEL;
		else if (FlagGet(FLAG_BADGE05_GET))
			obedienceLevel = BADGE_5_OBEDIENCE_LEVEL;
		else if (FlagGet(FLAG_BADGE04_GET))
			obedienceLevel = BADGE_4_OBEDIENCE_LEVEL;
		else if (FlagGet(FLAG_BADGE03_GET))
			obedienceLevel = BADGE_3_OBEDIENCE_LEVEL;
		else if (FlagGet(FLAG_BADGE02_GET))
			obedienceLevel = BADGE_2_OBEDIENCE_LEVEL;
		else if (FlagGet(FLAG_BADGE01_GET))
			obedienceLevel = BADGE_1_OBEDIENCE_LEVEL;
		else
			obedienceLevel = BASE_OBEDIENCE_LEVEL;
	#endif

	if (gBattleMons[gBankAttacker].level <= obedienceLevel)
		return 0;
	rnd = (Random() & 255);
	calc = (gBattleMons[gBankAttacker].level + obedienceLevel) * rnd >> 8;
	if (calc < obedienceLevel)
		return 0;

	// is not obedient
	if (gCurrentMove == MOVE_RAGE)
		gBattleMons[gBankAttacker].status2 &= ~(STATUS2_RAGE);
	if (gBattleMons[gBankAttacker].status1 & STATUS1_SLEEP && (gCurrentMove == MOVE_SNORE || gCurrentMove == MOVE_SLEEPTALK))
	{
		gBattlescriptCurrInstr = BattleScript_IgnoresWhileAsleep;
		return 1;
	}

	rnd = (Random() & 255);
	calc = (gBattleMons[gBankAttacker].level + obedienceLevel) * rnd >> 8;
	if (calc < obedienceLevel && gCurrentMove != MOVE_FOCUSPUNCH && gCurrentMove != MOVE_BEAKBLAST && gCurrentMove != MOVE_SHELLTRAP)
	{
		calc = CheckMoveLimitations(gBankAttacker, gBitTable[gCurrMovePos], 0xFF);
		if (calc == 0xF) // all moves cannot be used
		{
			gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
			gBattleCommunication[MULTISTRING_CHOOSER] = Random() & 3;
			gBattlescriptCurrInstr = BattleScript_MoveUsedLoafingAround;
			return 1;
		}
		else // use a random move
		{
			do
			{
				gCurrMovePos = gChosenMovePos = Random() & 3;
			} while (gBitTable[gCurrMovePos] & calc);

			if (!gNewBS->ParentalBondOn)
				gMultiHitCounter = 0; //So multi-strike moves don't cause the new move to strike more than once
			gCalledMove = gBattleMons[gBankAttacker].moves[gCurrMovePos];
			gBattlescriptCurrInstr = BattleScript_IgnoresAndUsesRandomMove;
			gBankTarget = GetMoveTarget(gCalledMove, 0);
			gHitMarker |= HITMARKER_x200000;
			return 2;
		}
	}
	else
	{
		obedienceLevel = gBattleMons[gBankAttacker].level - obedienceLevel;

		calc = (Random() & 255);
		if (calc < obedienceLevel && CanBePutToSleep(gBankAttacker, gBankAttacker, FALSE))
		{
			// try putting asleep
			int i;
			for (i = 0; i < gBattlersCount; i++)
			{
				if (gBattleMons[i].status2 & STATUS2_UPROAR)
					break;
			}
			if (i == gBattlersCount)
			{
				gBattlescriptCurrInstr = BattleScript_IgnoresAndFallsAsleep;
				return 1;
			}
		}
		calc -= obedienceLevel;
		if (calc < obedienceLevel)
		{
			gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
			gBattleMoveDamage = ConfusionDamageCalc();
			gBankTarget = gBankAttacker;
			gBattlescriptCurrInstr = BattleScript_IgnoresAndHitsItself;
			return 2;
		}
		else
		{
			gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
			gBattleCommunication[MULTISTRING_CHOOSER] = Random() & 3;
			gBattlescriptCurrInstr = BattleScript_MoveUsedLoafingAround;
			return 1;
		}
	}
}

bool8 CanTargetPartner(u8 bankDef)
{
	return IS_DOUBLE_BATTLE
		&& BATTLER_ALIVE(PARTNER(bankDef))
		&& bankDef != PARTNER(gBankAttacker);
}

bool8 TargetFullyImmuneToCurrMove(u8 bankDef)
{
	return TypeCalc(gCurrentMove, gBankAttacker, bankDef, NULL) & MOVE_RESULT_NO_EFFECT
		 || ProtectAffects(gCurrentMove, gBankAttacker, bankDef, FALSE)
		 || BATTLER_SEMI_INVULNERABLE(bankDef)
		 || DoesTargetHaveAbilityImmunity();
}

static bool8 IsTargetUnaffectedByPrankster(u8 bankAtk, u8 bankDef, u16 currentMove, u16 chosenMove)
{
	u8 moveTarget = GetBaseMoveTarget(gCurrentMove, gBankAttacker);

	return ABILITY(bankAtk) == ABILITY_PRANKSTER
		&& IsOfType(bankDef, TYPE_DARK)
		&& (SPLIT(currentMove) == SPLIT_STATUS || (currentMove != chosenMove && SPLIT(chosenMove) == SPLIT_STATUS)) //This move was called by a status move (eg. Assist)
		&& !(moveTarget & (MOVE_TARGET_OPPONENTS_FIELD | MOVE_TARGET_DEPENDS))
		&& SIDE(bankAtk) != SIDE(bankDef) //Affects allies but not foes
		&& (ABILITY(bankDef) != ABILITY_MAGICBOUNCE || !(gBattleMoves[currentMove].flags & FLAG_MAGIC_COAT_AFFECTED)) //This move would be bounced back and not affected
		&& (!gProtectStructs[bankDef].bounceMove || !(gBattleMoves[currentMove].flags & FLAG_MAGIC_COAT_AFFECTED)) //Bounce back with Magic Coat if can
		&& AttacksThisTurn(bankAtk, currentMove) == 2
		&& !gSpecialMoveFlags[currentMove].gSpecialWholeFieldMoves
		&& !ProtectAffects(currentMove, bankAtk, bankDef, FALSE)
		&& !MissesDueToSemiInvulnerability(bankAtk, bankDef, currentMove);
}

//For Dragon Darts
bool8 DoesTargetHaveAbilityImmunity(void)
{
	BattleScriptPushCursor(); //Backup the current script

	if (AbilityBattleEffects(ABILITYEFFECT_MOVES_BLOCK, gBankTarget, 0, 0, 0)
	||  AbilityBattleEffects(ABILITYEFFECT_ABSORBING, gBankTarget, 0, 0, 0))
	{
		BattleScriptPop(); //Restore the original script
		return TRUE;
	}

	BattleScriptPop(); //Restore the original script
	return FALSE;
}
