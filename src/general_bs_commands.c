#include "defines.h"
#include "defines_battle.h"
#include "../include/battle_anim.h"
#include "../include/battle_string_ids.h"
#include "../include/money.h"
#include "../include/random.h"
#include "../include/string_util.h"
#include "../include/wild_encounter.h"
#include "../include/constants/items.h"
#include "../include/constants/songs.h"
#include "../include/constants/trainer_classes.h"

#include "../include/new/ability_battle_effects.h"
#include "../include/new/ability_battle_scripts.h"
#include "../include/new/ability_tables.h"
#include "../include/new/accuracy_calc.h"
#include "../include/new/attackcanceler.h"
#include "../include/new/battle_start_turn_start.h"
#include "../include/new/battle_start_turn_start_battle_scripts.h"
#include "../include/new/battle_strings.h"
#include "../include/new/battle_terrain.h"
#include "../include/new/battle_util.h"
#include "../include/new/cmd49_battle_scripts.h"
#include "../include/new/damage_calc.h"
#include "../include/new/dynamax.h"
#include "../include/new/end_turn_battle_scripts.h"
#include "../include/new/evolution.h"
#include "../include/new/form_change.h"
#include "../include/new/frontier.h"
#include "../include/new/general_bs_commands.h"
#include "../include/new/item_battle_scripts.h"
#include "../include/new/move_battle_scripts.h"
#include "../include/new/move_tables.h"
#include "../include/new/multi.h"
#include "../include/new/pickup_items.h"
#include "../include/new/stat_buffs.h"
#include "../include/new/switching.h"
#include "../include/new/set_z_effect.h"
#include "../include/new/util.h"

/*
general_bs_commands.c
	functions that support the battle scripting in assembly/battle_scripts

tables:
	sEntryHazardsStrings

*/

#define TEXT_BUFFER_SIDE_STATUS(move, status, side)				\
{																\
	gSideStatuses[side] &= ~status;							\
	gBattleTextBuff1[0] = 0xFD;									\
	gBattleTextBuff1[1] = 0x2;									\
	gBattleTextBuff1[2] = (move & 0xFF);						\
	gBattleTextBuff1[3] = move >> 8;							\
	gBattleTextBuff1[4] = EOS;									\
	BattleScriptPushCursor();									\
	gBattlescriptCurrInstr = BattleScript_SideStatusWoreOffRet;	\
}

//This file's functions:
static bool8 ProcessPreAttackAnimationFuncs(void);
static bool8 TryStrongWindsWeakenAttack(u8 bank);
static bool8 TryActivateWeakenessBerry(u8 bank, u8 resultFlags);
static bool8 IsSingleTargetOfDoublesSpreadMove(void);
static bool8 IsDoubleSpreadMove(void);
static bool8 DoesBankNegateDamage(u8 bank, u16 move);
static void TryContraryChangeStatAnim(u8 bank, u16* argumentPtr);
static item_t ChoosePickupItem(u8 level);

const u16 gMissStringIds[] =
{
	STRINGID_PKMNAVOIDEDATTACK,
	STRINGID_PKMNPROTECTEDITSELF,
	STRINGID_PKMNAVOIDEDATTACK,
	STRINGID_AVOIDEDDAMAGE,
	STRINGID_PKMNMAKESGROUNDMISS,
	0x184, //Crafty Shield
	0x184, //Mat Block
	0x184, //Quick Guard
	0x184, //Wide Guard
	0x184, //Raid Shield
	0x184, //Safety Goggles
};

static const u8* const sEntryHazardsStrings[] =
{
	SpikesLayString,
	StealthRockLayString,
	ToxicSpikesLayString,
	StickyWebLayString,
	gText_SteelsurgeLay,
};


void TrySetMissStringForSafetyGoggles(u8 bankDef)
{
	if (ITEM_EFFECT(bankDef) == ITEM_EFFECT_SAFETY_GOGGLES)
	{
		gLastUsedItem = ITEM(bankDef);
		gBattleStringLoader = gText_NotAffectedBecauseOfItem;
		gNewBS->missStringId[ bankDef] = 10;
	}
}

bool8 TryActivateGemBattlescript(void)
{
	//Gems only work when the target is affected

	if (ITEM_EFFECT(gBankAttacker) == ITEM_EFFECT_GEM
	&&  ITEM_QUALITY(gBankAttacker) == gBattleStruct->dynamicMoveType
	&&  SPLIT(gCurrentMove) != SPLIT_STATUS
	&& !(gMoveResultFlags & (MOVE_RESULT_MISSED | MOVE_RESULT_DOESNT_AFFECT_FOE | MOVE_RESULT_FAILED))
	&& !ProtectAffects(gCurrentMove, gBankAttacker, gBankTarget, FALSE)
	&& !DoesTargetHaveAbilityImmunity()
	&& gBattleMoves[gCurrentMove].effect != EFFECT_PLEDGE
	&& AttacksThisTurn(gBankAttacker, gCurrentMove) == 2)
	{
		gLastUsedItem = ITEM(gBankAttacker);
		gNewBS->consumedGem = TRUE;
		BattleScriptPushCursor();
		gBattlescriptCurrInstr = BattleScript_Gems;
		return TRUE;
	}

	return FALSE;
}

void TryUseGemFutureSight(void)
{
	if (gBattlerPartyIndexes[gBankAttacker] == gWishFutureKnock.futureSightPartyIndex[gBankTarget]) //Original user of Future Sight
	{
		if (TryActivateGemBattlescript())
			gBattlescriptCurrInstr -= 5;
	}
}

void atk02_attackstring(void)
{
	u8 moveType = gBattleStruct->dynamicMoveType;

	if (gBattleExecBuffer) return;

	if (gBattlescriptCurrInstr == BattleScript_ButItFailedAttackstring)
		gMoveResultFlags |= MOVE_RESULT_FAILED;

	if (!(gHitMarker & (HITMARKER_NO_ATTACKSTRING | HITMARKER_ATTACKSTRING_PRINTED)))
	{
		PrepareStringBattle(STRINGID_USEDMOVE, gBankAttacker);

		gBattlescriptCurrInstr++;
		gBattleCommunication[MSG_DISPLAY] = 0;

		if (!gNewBS->DancerInProgress && gCurrentMove != MOVE_COPYCAT)
		{
			gNewBS->LastUsedMove = gCurrentMove;
			gNewBS->LastUsedTypes[gBankAttacker] = moveType;

			if (IsAnyMaxMove(gCurrentMove))
				gNewBS->LastUsedMove = gChosenMove;

			if (!gSpecialMoveFlags[gCurrentMove].gMovesThatCallOtherMoves)
			{
				u8 chargingBonus = 20 * gNewBS->metronomeItemBonus[gBankAttacker];
				if (gLastPrintedMoves[gBankAttacker] == gCurrentMove)
					gNewBS->MetronomeCounter[gBankAttacker] = MathMin(100, gNewBS->MetronomeCounter[gBankAttacker] + 20 + chargingBonus);
				else
					gNewBS->MetronomeCounter[gBankAttacker] = 0 + chargingBonus;
			}
		}

		if (gCurrentMove != MOVE_STRUGGLE && !(gHitMarker & HITMARKER_UNABLE_TO_USE_MOVE))
		{
			if (ABILITY(gBankAttacker) == ABILITY_PROTEAN
			&& !(gMoveResultFlags & MOVE_RESULT_FAILED)
			&& !gSpecialMoveFlags[gCurrentMove].gMovesThatCallOtherMoves)
			{
				if (gBattleMons[gBankAttacker].type1 != moveType
				||  gBattleMons[gBankAttacker].type2 != moveType
				|| (gBattleMons[gBankAttacker].type3 != moveType && gBattleMons[gBankAttacker].type3 != TYPE_BLANK))
				{
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_AbilityChangedType;
				}
			}
		}

		gHitMarker |= HITMARKER_ATTACKSTRING_PRINTED;
	}
	else
	{
		gBattlescriptCurrInstr++;
		gBattleCommunication[MSG_DISPLAY] = 0;
	}

	if (gCurrentMove != MOVE_STRUGGLE && !(gHitMarker & HITMARKER_UNABLE_TO_USE_MOVE))
		TryActivateGemBattlescript();
}

void BufferAttackerItem(void)
{
	gLastUsedItem = ITEM(gBankAttacker);
}

void atk03_ppreduce(void) {
	s32 ppToDeduct = 1;

	if (gBattleExecBuffer) return;

	if (!gSpecialStatuses[gBankAttacker].ppNotAffectedByPressure)
	{
		switch (GetBaseMoveTarget(gCurrentMove, gBankAttacker)) {
			case MOVE_TARGET_FOES_AND_ALLY:
				ppToDeduct += AbilityBattleEffects(ABILITYEFFECT_COUNT_ON_FIELD, gBankAttacker, ABILITY_PRESSURE, 0, 0);
				break;

			case MOVE_TARGET_BOTH:
			case MOVE_TARGET_OPPONENTS_FIELD:
				ppToDeduct += AbilityBattleEffects(ABILITYEFFECT_COUNT_OTHER_SIDE, gBankAttacker, ABILITY_PRESSURE, 0, 0);
				break;

			default:
				if (gCurrentMove == MOVE_IMPRISON
				|| gCurrentMove == MOVE_SNATCH
				|| (gBattleMoves[gCurrentMove].effect == EFFECT_SPIKES && gCurrentMove != MOVE_STICKYWEB)) //No Sticky Web is assumed GF bug
					ppToDeduct += AbilityBattleEffects(ABILITYEFFECT_COUNT_OTHER_SIDE, gBankAttacker, ABILITY_PRESSURE, 0, 0);
				else if (SIDE(gBankAttacker) != SIDE(gBankTarget) && ABILITY(gBankTarget) == ABILITY_PRESSURE)
					ppToDeduct++;
				break;
		}
	}

	if (!(gHitMarker & (HITMARKER_NO_PPDEDUCT | HITMARKER_NO_ATTACKSTRING))
	&& gNewBS->ParentalBondOn != 1
	&& !gNewBS->DancerInProgress
	&& gBattleMons[gBankAttacker].pp[gCurrMovePos]) {
		gProtectStructs[gBankAttacker].notFirstStrike = 1;

		if (gBattleMons[gBankAttacker].pp[gCurrMovePos] > ppToDeduct)
			gBattleMons[gBankAttacker].pp[gCurrMovePos] -= ppToDeduct;
		else
			gBattleMons[gBankAttacker].pp[gCurrMovePos] = 0;

		if (!(gBattleMons[gBankAttacker].status2 & STATUS2_TRANSFORMED)
			&& !((gDisableStructs[gBankAttacker].mimickedMoves) & gBitTable[gCurrMovePos]))
		{
			gActiveBattler = gBankAttacker;
			EmitSetMonData(0, REQUEST_PPMOVE1_BATTLE + gCurrMovePos, 0, 1, &gBattleMons[gBankAttacker].pp[gCurrMovePos]);
			MarkBufferBankForExecution(gBankAttacker);
		}
	}

	gHitMarker &= ~(HITMARKER_NO_PPDEDUCT);
	gBattlescriptCurrInstr++;
}

static bool8 ProcessPreAttackAnimationFuncs(void)
{
	if (IsDoubleSpreadMove())
	{
		if (!gNewBS->printedStrongWindsWeakenedAttack)
		{
			for (u8 bankDef = 0; bankDef < gBattlersCount; ++bankDef)
			{
				if (!BATTLER_ALIVE(bankDef) || bankDef == gBankAttacker
				|| (bankDef == PARTNER(gBankAttacker) && !(GetBaseMoveTarget(gCurrentMove, gBankAttacker) & MOVE_TARGET_ALL))
				|| (gNewBS->noResultString[bankDef] && gNewBS->noResultString[bankDef] != 2))
					continue; //Don't bother with this target

				if (TryStrongWindsWeakenAttack(bankDef))
					return TRUE;
			}
		}

		for (u8 bankDef = 0; bankDef < gBattlersCount; ++bankDef)
		{
			if (!BATTLER_ALIVE(bankDef) || bankDef == gBankAttacker
			|| (bankDef == PARTNER(gBankAttacker) && !(GetBaseMoveTarget(gCurrentMove, gBankAttacker) & MOVE_TARGET_ALL))
			|| (gNewBS->noResultString[bankDef] && gNewBS->noResultString[bankDef] != 2))
				continue; //Don't bother with this target

			if (TryActivateWeakenessBerry(bankDef, gNewBS->ResultFlags[bankDef]))
			{
				gBattlescriptCurrInstr += 5; //Counteract the callasm decrement
				return TRUE;
			}
		}
	}
	else //Single Target Move
	{
		if (TryStrongWindsWeakenAttack(gBankTarget))
			return TRUE;

		if (TryActivateWeakenessBerry(gBankTarget, gMoveResultFlags))
		{
			gBattlescriptCurrInstr += 5; //Counteract the callasm decrement
			return TRUE;
		}
	}

	return FALSE;
}

static bool8 TryStrongWindsWeakenAttack(u8 bankDef)
{
	//Delta Stream indicates that it weakens all attacks that would normally be super-effective against
	//Flying-type Pokemon, even if the attack wouldn't normally be super effective
	if (gBattleWeather & WEATHER_AIR_CURRENT_PRIMAL
	&& SPLIT(gCurrentMove) != SPLIT_STATUS
	&& IsOfType(bankDef, TYPE_FLYING)
	&& gTypeEffectiveness[gBattleStruct->dynamicMoveType][TYPE_FLYING] == TYPE_MUL_SUPER_EFFECTIVE
	&& WEATHER_HAS_EFFECT
	&& !gNewBS->printedStrongWindsWeakenedAttack) //Already checked before in doubles but not in singles
	{
		BattleScriptPushCursor();
		gBattlescriptCurrInstr = BattleScript_StrongWindsWeakenedttack;
		gNewBS->printedStrongWindsWeakenedAttack = TRUE;
		return TRUE;
	}

	return FALSE;
}

static bool8 TryActivateWeakenessBerry(u8 bank, u8 resultFlags)
{
	if (ITEM_EFFECT(bank) == ITEM_EFFECT_WEAKNESS_BERRY
	&& !UnnerveOnOpposingField(bank))
	{
		if ((resultFlags & MOVE_RESULT_SUPER_EFFECTIVE && ITEM_QUALITY(bank) == gBattleStruct->dynamicMoveType && !DoesBankNegateDamage(bank, gCurrentMove))
		||  (SPLIT(gCurrentMove) != SPLIT_STATUS && ITEM_QUALITY(bank) == TYPE_NORMAL && gBattleStruct->dynamicMoveType == TYPE_NORMAL)) //Chilan Berry
		{
			gLastUsedItem = ITEM(bank);
			gBattleScripting.bank = bank;
			BattleScriptPushCursor();
			gNewBS->canBelch[SIDE(bank)] |= gBitTable[gBattlerPartyIndexes[bank]];
			gBattlescriptCurrInstr = BattleScript_WeaknessBerryActivate - 5;
			return TRUE;
		}
	}

	return FALSE;
}

void TryActivateWeakenessBerryFutureSight(void)
{
	TryActivateWeakenessBerry(gBankTarget, gMoveResultFlags);
}

static bool8 IsSingleTargetOfDoublesSpreadMove(void)
{
	u8 moveTarget = GetBaseMoveTarget(gCurrentMove, gBankAttacker);

	if (moveTarget & MOVE_TARGET_ALL)
		return gNewBS->allSpreadTargets <= 1;
	else if (moveTarget & MOVE_TARGET_BOTH)
		return gNewBS->foeSpreadTargets <= 1;

	return TRUE;
}

static bool8 IsDoubleSpreadMove(void)
{
	return IS_DOUBLE_BATTLE
		&& !(gHitMarker & (HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_NON_ATTACK_DMG | HITMARKER_UNABLE_TO_USE_MOVE))
		&& GetBaseMoveTarget(gCurrentMove, gBankAttacker) & (MOVE_TARGET_ALL | MOVE_TARGET_BOTH)
		&& !IsSingleTargetOfDoublesSpreadMove();
}

static bool8 DoesBankNegateDamage(u8 bank, unusedArg u16 move)
{
	unusedArg u16 species = SPECIES(bank);
	unusedArg u8 ability = ABILITY(bank);

	return !IS_TRANSFORMED(bank) && IsAffectedByDisguse(ability, species, SPLIT(move));
}

static u8 UpdateEffectivenessResultFlagsForDoubleSpreadMoves(u8 resultFlags)
{
	u32 i, j;

	//Only play the "best" sound
	for (i = 0; i < 3; ++i)
	{
		for (j = 0; j < gBattlersCount; ++j)
		{
			if (!(gNewBS->ResultFlags[j] & (MOVE_RESULT_MISSED | MOVE_RESULT_NO_EFFECT)) && !gNewBS->noResultString[j])
			{
				switch (i) {
					case 0:
						if (gNewBS->ResultFlags[j] & MOVE_RESULT_SUPER_EFFECTIVE && !DoesBankNegateDamage(j, gCurrentMove))
							return gNewBS->ResultFlags[j];
						break;
					case 1:
						if (gNewBS->ResultFlags[j] & MOVE_RESULT_NOT_VERY_EFFECTIVE && !DoesBankNegateDamage(j, gCurrentMove))
							return gNewBS->ResultFlags[j];
						break;
					case 2:
						if (DoesBankNegateDamage(j, gCurrentMove))
							return 0; //Normal effectiveness
						return gNewBS->ResultFlags[j];
				}
			}
		}
	}

	return resultFlags;
}

void atk09_attackanimation(void)
{
	bool8 cramorantTransform = FALSE;
	u16 move = gCurrentMove;

	if (gBattleExecBuffer
	|| ProcessPreAttackAnimationFuncs())
		return;

	#if (defined SPECIES_CRAMORANT && defined SPECIES_CRAMORANT_GORGING && defined SPECIES_CRAMORANT_GULPING)
	if ((move == MOVE_SURF || move == MOVE_DIVE)
	&& ABILITY(gBankAttacker) == ABILITY_GULPMISSILE
	&& !IsDynamaxed(gBankAttacker))
	{
		u16 species = GetMonData(GetBankPartyData(gBankAttacker), MON_DATA_SPECIES2, NULL);

		if (species == SPECIES_CRAMORANT)
		{
			u16 newSpecies = (gBattleMons[gBankAttacker].hp <= gBattleMons[gBankAttacker].maxHP / 2) ? SPECIES_CRAMORANT_GORGING : SPECIES_CRAMORANT_GULPING;
			DoFormChange(gBankAttacker, newSpecies, TRUE, FALSE, FALSE);
			gNewBS->cramorantTransformed = TRUE; //Indicator so it doesn't transform again in gorging and gulping forms
			return;
		}
		else if (gBattleScripting.animTurn == 0
		&& gNewBS->cramorantTransformed
		&& (species == SPECIES_CRAMORANT_GORGING || species == SPECIES_CRAMORANT_GULPING))
		{
			gBattleScripting.bank = gBankAttacker;
			cramorantTransform = TRUE;
			gNewBS->cramorantTransformed = FALSE;
		}
	}
	#endif

	u8 resultFlags = gMoveResultFlags;
	if (IsDoubleSpreadMove())
		resultFlags = UpdateEffectivenessResultFlagsForDoubleSpreadMoves(resultFlags);

	if (((gHitMarker & HITMARKER_NO_ANIMATIONS)
	 && (move != MOVE_TRANSFORM && move != MOVE_SUBSTITUTE
	  && move != MOVE_ELECTRICTERRAIN && move != MOVE_PSYCHICTERRAIN
	  && move != MOVE_MISTYTERRAIN && move != MOVE_GRASSYTERRAIN)) //Terrain animations always need to play and reload BG
	|| gNewBS->tempIgnoreAnimations)
	{
		if (!(resultFlags & MOVE_RESULT_NO_EFFECT))
			gNewBS->attackAnimationPlayed = TRUE;

		if (gNewBS->tempIgnoreAnimations)
		{
			gBattlescriptCurrInstr += 1;
			gNewBS->tempIgnoreAnimations = FALSE;
		}
		else if (cramorantTransform)
		{
			BattleScriptPush(gBattlescriptCurrInstr + 1);
			gBattlescriptCurrInstr = BattleScript_CramorantCatchPrey;
		}
		else
		{
			#ifdef FLAG_FAST_BATTLE_MESSAGES
			if (FlagGet(FLAG_FAST_BATTLE_MESSAGES))
				gBattlescriptCurrInstr++;
			else
			#endif
			{
				BattleScriptPush(gBattlescriptCurrInstr + 1);
				gBattlescriptCurrInstr = BattleScript_Pausex20;
			}
		}

		gBattleScripting.animTurn++;
		gBattleScripting.animTargetsHit++;
	}
	else
	{
		u8 moveTarget = GetBaseMoveTarget(move, gBankAttacker);
	
		if (gNewBS->ParentalBondOn == 1)
		{
			gBattlescriptCurrInstr++;
			return;
		}
		else if (gBattleScripting.animTargetsHit > 0
			&& (moveTarget & (MOVE_TARGET_BOTH | MOVE_TARGET_FOES_AND_ALLY | MOVE_TARGET_DEPENDS)
			  || move == MOVE_DEFOG
			  || move == MOVE_SHELLSMASH
			  || move == MOVE_HOWL))
		{
			gBattlescriptCurrInstr++;
			return;
		}

		if (!(resultFlags & MOVE_RESULT_NO_EFFECT))
		{
			u8 multihit;

			gActiveBattler = gBankAttacker;

			if (gBattleMons[gBankTarget].status2 & STATUS2_SUBSTITUTE)
				multihit = gMultiHitCounter;
			else if (gMultiHitCounter != 0 && gMultiHitCounter != 1)
			{
				if (gBattleMons[gBankTarget].hp <= gBattleMoveDamage)
					multihit = 1;
				else
					multihit = gMultiHitCounter;
			}
			else
				multihit = gMultiHitCounter;

			u8 animTurn = gBattleScripting.animTurn;
			if (move == MOVE_EXPANDINGFORCE && moveTarget & MOVE_TARGET_BOTH)
				animTurn = 1; //Play doubles animation

			gNewBS->attackAnimationPlayed = TRUE;
			EmitMoveAnimation(0, move, animTurn, gBattleMovePower, gBattleMoveDamage, gBattleMons[gBankAttacker].friendship, &gDisableStructs[gBankAttacker], multihit);
			gBattleScripting.animTurn += 1;
			gBattleScripting.animTargetsHit += 1;
			MarkBufferBankForExecution(gBankAttacker);
			gBattlescriptCurrInstr++;

			if (cramorantTransform)
			{
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_CramorantCatchPrey;
			}
		}
		else if (!IsDoubleSpreadMove() || !gNewBS->calculatedSpreadMoveData)
		{
			#ifdef FLAG_FAST_BATTLE_MESSAGES
			if (FlagGet(FLAG_FAST_BATTLE_MESSAGES))
				gBattlescriptCurrInstr++;
			else
			#endif
			{
				BattleScriptPush(gBattlescriptCurrInstr + 1);
				gBattlescriptCurrInstr = BattleScript_Pausex20;
			}
		}
		else
			gBattlescriptCurrInstr++;
	}
	
	gNewBS->calculatedSpreadMoveData = TRUE;
}

static void DoublesHPBarReduction(void)
{
	if (!gNewBS->doneDoublesSpreadHit //Spread moves in doubles are only done once
	&& !(gHitMarker & (HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_NON_ATTACK_DMG)))
	{
		s16 healthValue;
		s32 currDmg, maxPossibleDmgValue;
		maxPossibleDmgValue = 0xFFFF; //Ensures that huge damage values don't change sign

		for (u32 i = 0; i < gBattlersCount; ++i)
		{
			if (!(gNewBS->ResultFlags[i] & MOVE_RESULT_NO_EFFECT) && gNewBS->DamageTaken[i] != 0 && !gNewBS->noResultString[i]
			&& !MoveBlockedBySubstitute(gCurrentMove, gBankAttacker, i)
			&& !DoesBankNegateDamage(i, gCurrentMove))
			{
				gActiveBattler = i;
				currDmg = gNewBS->DamageTaken[i];
				
				if (currDmg <= maxPossibleDmgValue)
					healthValue = currDmg;
				else
					healthValue = maxPossibleDmgValue;

				EmitHealthBarUpdate(0, healthValue);
				MarkBufferBankForExecution(gActiveBattler);

				if (SIDE(gActiveBattler) == B_SIDE_PLAYER && currDmg > 0)
					gBattleResults.playerMonWasDamaged = TRUE;
			}
		}

		gNewBS->doneDoublesSpreadHit = TRUE;
	}
}

void atk0B_healthbarupdate(void)
{
	u8 ability;

	if (gBattleExecBuffer) return;

	if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) || (gHitMarker & HITMARKER_NON_ATTACK_DMG))
	{
		gActiveBattler = GetBankForBattleScript(gBattlescriptCurrInstr[1]);
		ability = ABILITY(gActiveBattler);

		if (IS_BEHIND_SUBSTITUTE(gActiveBattler)
		&& gDisableStructs[gActiveBattler].substituteHP != 0
		&& !(gHitMarker & (HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_NON_ATTACK_DMG))
		&& !gNewBS->bypassSubstitute)
		{
			PrepareStringBattle(STRINGID_SUBSTITUTEDAMAGED, gActiveBattler);
			if (IsDoubleSpreadMove())
				DoublesHPBarReduction();
		}
		#ifdef SPECIES_MIMIKYU
		else if (ability == ABILITY_DISGUISE
		&& (!(gHitMarker & (HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_NON_ATTACK_DMG)) || gNewBS->breakDisguiseSpecialDmg)
		&& SPECIES(gActiveBattler) == SPECIES_MIMIKYU
		&& !IS_TRANSFORMED(gActiveBattler))
		{
			gBattleScripting.bank = gBankTarget;
			BattleScriptPush(gBattlescriptCurrInstr + 2);
			gBattlescriptCurrInstr = BattleScript_DisguiseTookDamage;
			if (IsDoubleSpreadMove())
				DoublesHPBarReduction();
			return;
		}
		#endif
		#ifdef SPECIES_EISCUE
		else if (ability == ABILITY_ICEFACE
		&& SPECIES(gActiveBattler) == SPECIES_EISCUE
		&& (!(gHitMarker & (HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_NON_ATTACK_DMG)) || gNewBS->breakDisguiseSpecialDmg)
		&& SPLIT(gCurrentMove) == SPLIT_PHYSICAL //Only physical moves are stopped by the ice face
		&& !IS_TRANSFORMED(gActiveBattler))
		{
			gBattleScripting.bank = gBankTarget;
			BattleScriptPush(gBattlescriptCurrInstr + 2);
			gBattlescriptCurrInstr = BattleScript_IceFaceTookDamage;
			if (IsDoubleSpreadMove())
				DoublesHPBarReduction();
			return;
		}
		#endif
		else
		{
			if (!IsDoubleSpreadMove())
			{
				s32 healthValue, maxPossibleDmgValue;
				maxPossibleDmgValue = 30000; //Ensures that huge damage values don't change sign
			
				if (gBattleMoveDamage <= maxPossibleDmgValue)
					healthValue = gBattleMoveDamage;
				else
					healthValue = maxPossibleDmgValue;

				EmitHealthBarUpdate(0, healthValue);
				MarkBufferBankForExecution(gActiveBattler);

				if (SIDE(gActiveBattler) == B_SIDE_PLAYER && gBattleMoveDamage > 0)
					gBattleResults.playerMonWasDamaged = TRUE;
			}
			else
			{
				DOUBLES_HEALTH_REDUCE:
				DoublesHPBarReduction();
			}
		}
	}
	else if (IsDoubleSpreadMove())
		goto DOUBLES_HEALTH_REDUCE;

	gBattlescriptCurrInstr += 2;
}

void atk0C_datahpupdate(void)
{
	if (gBattleExecBuffer) return;

	if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) || (gHitMarker & HITMARKER_NON_ATTACK_DMG))
	{
		gActiveBattler = GetBankForBattleScript(gBattlescriptCurrInstr[1]);

		//If Substitute Up
		if (gBattleMons[gActiveBattler].status2 & STATUS2_SUBSTITUTE
		&& gDisableStructs[gActiveBattler].substituteHP
		&& !(gHitMarker & HITMARKER_IGNORE_SUBSTITUTE)
		&& !gNewBS->bypassSubstitute)
		{
			if (gDisableStructs[gActiveBattler].substituteHP >= gBattleMoveDamage)
			{
				if (gSpecialStatuses[gActiveBattler].moveturnLostHP == 0)
					gSpecialStatuses[gActiveBattler].moveturnLostHP = gBattleMoveDamage;
				gDisableStructs[gActiveBattler].substituteHP -= gBattleMoveDamage;
				gHpDealt = gBattleMoveDamage;
			}
			else
			{
				if (gSpecialStatuses[gActiveBattler].moveturnLostHP == 0)
					gSpecialStatuses[gActiveBattler].moveturnLostHP = gDisableStructs[gActiveBattler].substituteHP;
				gHpDealt = gDisableStructs[gActiveBattler].substituteHP;
				gDisableStructs[gActiveBattler].substituteHP = 0;
			}

			gNewBS->AttackerDidDamageAtLeastOnce = TRUE;
			//Check Substitute Fading
			if (gDisableStructs[gActiveBattler].substituteHP == 0)
			{
				gBattlescriptCurrInstr += 2;
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_SubstituteFade;
				return;
			}
		}
		#ifdef SPECIES_MIMIKYU
		else if (ABILITY(gActiveBattler) == ABILITY_DISGUISE //Disguise Protected
		&& SPECIES(gActiveBattler) == SPECIES_MIMIKYU
		&& (!(gHitMarker & (HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_NON_ATTACK_DMG)) || gNewBS->breakDisguiseSpecialDmg)
		&& !IS_TRANSFORMED(gActiveBattler))
		{
			gBattleMoveDamage = GetBaseCurrentHP(gActiveBattler) / 8;

			if (gSpecialStatuses[gActiveBattler].moveturnLostHP == 0)
				gSpecialStatuses[gActiveBattler].moveturnLostHP = 1;
			gHpDealt = gBattleMoveDamage;
			gNewBS->AttackerDidDamageAtLeastOnce = TRUE;
			gMoveResultFlags = 0;

			if (CalcMoveSplit(gCurrentMove, gBankAttacker, gActiveBattler) == SPLIT_PHYSICAL)
			{
				gProtectStructs[gActiveBattler].physicalDmg = gHpDealt;
				gSpecialStatuses[gActiveBattler].moveturnLostHP_physical = gHpDealt;
				gProtectStructs[gActiveBattler].physicalBank = gBankAttacker;
				gSpecialStatuses[gActiveBattler].moveturnPhysicalBank = gBankAttacker;
			}
			else if (CalcMoveSplit(gCurrentMove, gBankAttacker, gActiveBattler) == SPLIT_SPECIAL)
			{
				gProtectStructs[gActiveBattler].specialDmg = gHpDealt;
				gSpecialStatuses[gActiveBattler].moveturnLostHP_special = gHpDealt;
				gProtectStructs[gActiveBattler].specialBank = gBankAttacker;
				gSpecialStatuses[gActiveBattler].moveturnSpecialBank = gBankAttacker;
			}

			gBattleScripting.bank = gActiveBattler;
			DoFormChange(gActiveBattler, SPECIES_MIMIKYU_BUSTED, TRUE, FALSE, FALSE);
			gBattlescriptCurrInstr += 2;
			BattleScriptPushCursor();
			gBattlescriptCurrInstr = BattleScript_DisguiseTransform;
			return;
		}
		#endif
		#ifdef SPECIES_EISCUE
		else if (ABILITY(gActiveBattler) == ABILITY_ICEFACE //Disguise Protected
		&& SPECIES(gActiveBattler) == SPECIES_EISCUE
		&& SPLIT(gCurrentMove) == SPLIT_PHYSICAL //Only physical attacks break the ice
		&& (!(gHitMarker & (HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_NON_ATTACK_DMG)) || gNewBS->breakDisguiseSpecialDmg)
		&& !IS_TRANSFORMED(gActiveBattler))
		{
			if (gSpecialStatuses[gActiveBattler].moveturnLostHP == 0)
				gSpecialStatuses[gActiveBattler].moveturnLostHP = 1;
			gHpDealt = 1;
			gNewBS->AttackerDidDamageAtLeastOnce = TRUE;
			gMoveResultFlags = 0;

			gProtectStructs[gActiveBattler].physicalDmg = gHpDealt;
			gSpecialStatuses[gActiveBattler].moveturnLostHP_physical = gHpDealt;
			gProtectStructs[gActiveBattler].physicalBank = gBankAttacker;
			gSpecialStatuses[gActiveBattler].moveturnPhysicalBank = gBankAttacker;

			gBattleScripting.bank = gActiveBattler;
			DoFormChange(gActiveBattler, SPECIES_EISCUE_NOICE, TRUE, TRUE, FALSE);
			gBattlescriptCurrInstr += 2;
			BattleScriptPushCursor();
			gBattlescriptCurrInstr = BattleScript_IceFaceTransform;
			return;
		}
		#endif
		else //No Substitute
		{
			if (gBattleMoveDamage < 0) //HP goes up
			{
				gBattleMons[gActiveBattler].hp -= gBattleMoveDamage;
				if (gBattleMons[gActiveBattler].hp > gBattleMons[gActiveBattler].maxHP)
					gBattleMons[gActiveBattler].hp = gBattleMons[gActiveBattler].maxHP;

				gHitMarker &= ~(HITMARKER_IGNORE_SUBSTITUTE);
			}
			else //HP goes down
			{
				if (gHitMarker & HITMARKER_x20)
					gHitMarker &= ~(HITMARKER_x20);

				else
				{
					gTakenDmg[gActiveBattler] += gBattleMoveDamage;
					if (gBattlescriptCurrInstr[1] == BS_GET_TARGET)
						gTakenDmgBanks[gActiveBattler] = gBankAttacker;
					else
						gTakenDmgBanks[gActiveBattler] = gBankTarget;
				}


				if (gBattleMons[gActiveBattler].hp > gBattleMoveDamage)
				{
					gBattleMons[gActiveBattler].hp -= gBattleMoveDamage;
					gHpDealt = gBattleMoveDamage;
				}
				else
				{
					gHpDealt = gBattleMons[gActiveBattler].hp;
					gBattleMons[gActiveBattler].hp = 0;
				}

				u32 hpDealt = gHpDealt;
				if (HasRaidShields(gActiveBattler)) //Raid Shields can be used outside of Raid Battles now
					hpDealt = MathMax(1, hpDealt); //Because damage can get heavily reduced to 0
				if (!gSpecialStatuses[gActiveBattler].moveturnLostHP && !(gHitMarker & HITMARKER_NON_ATTACK_DMG))
					gSpecialStatuses[gActiveBattler].moveturnLostHP = hpDealt;

				if (CalcMoveSplit(gCurrentMove, gBankAttacker, gActiveBattler) == SPLIT_PHYSICAL
				&& !(gHitMarker & HITMARKER_NON_ATTACK_DMG))
				{
					gProtectStructs[gActiveBattler].physicalDmg = hpDealt;
					gSpecialStatuses[gActiveBattler].moveturnLostHP_physical = hpDealt;

					if (gBattlescriptCurrInstr[1] == BS_GET_TARGET)
					{
						gProtectStructs[gActiveBattler].physicalBank = gBankAttacker;
						gSpecialStatuses[gActiveBattler].moveturnPhysicalBank = gBankAttacker;
						gNewBS->AttackerDidDamageAtLeastOnce = TRUE;
					}
					else
					{
						gProtectStructs[gActiveBattler].physicalBank = gBankTarget;
						gSpecialStatuses[gActiveBattler].moveturnPhysicalBank = gBankTarget;
					}
				}

				else if (CalcMoveSplit(gCurrentMove, gBankAttacker, gActiveBattler) == SPLIT_SPECIAL
				&& !(gHitMarker & HITMARKER_NON_ATTACK_DMG))
				{
					gProtectStructs[gActiveBattler].specialDmg = hpDealt;
					gSpecialStatuses[gActiveBattler].moveturnLostHP_special = hpDealt;

					if (gBattlescriptCurrInstr[1] == BS_GET_TARGET)
					{
						gProtectStructs[gActiveBattler].specialBank = gBankAttacker;
						gSpecialStatuses[gActiveBattler].moveturnSpecialBank = gBankAttacker;
						gNewBS->AttackerDidDamageAtLeastOnce = TRUE;
					}
					else
					{
						gProtectStructs[gActiveBattler].specialBank = gBankTarget;
						gSpecialStatuses[gActiveBattler].moveturnSpecialBank = gBankTarget;
					}
				}

				if (gStatuses3[gActiveBattler] & STATUS3_ILLUSION
				&& !(gHitMarker & HITMARKER_IGNORE_SUBSTITUTE)
				&& !BATTLER_ALIVE(gActiveBattler))
				{
					gBattleScripting.bank = gActiveBattler;
					gStatuses3[gActiveBattler] &= ~(STATUS3_ILLUSION);
					BattleScriptPush(gBattlescriptCurrInstr + 2);
					gBattlescriptCurrInstr = BattleScript_IllusionBrokenFaint - 2;
				}

				gHitMarker &= ~(HITMARKER_IGNORE_SUBSTITUTE);
			}

			gHitMarker &= ~(HITMARKER_NON_ATTACK_DMG);
			EmitSetMonData(0, REQUEST_HP_BATTLE, 0, 2, &gBattleMons[gActiveBattler].hp);
			MarkBufferBankForExecution(gActiveBattler);
		}
	}
	else
	{
		gActiveBattler = GetBankForBattleScript(gBattlescriptCurrInstr[1]);
		if (gSpecialStatuses[gActiveBattler].moveturnLostHP == 0)
			gSpecialStatuses[gActiveBattler].moveturnLostHP = 0xFFFF;
	}

	gBattlescriptCurrInstr += 2;
}

void atk0D_critmessage(void)
{
	if (gBattleExecBuffer)
		return;

	u16 stringId = 0;

	gBattleCommunication[MSG_DISPLAY] = 0;
	if (gCritMultiplier > BASE_CRIT_MULTIPLIER)
	{
		if (IsDoubleSpreadMove())
		{
			if (!(gNewBS->ResultFlags[gBankTarget] & MOVE_RESULT_NO_EFFECT) && !gNewBS->noResultString[gBankTarget])
			{
				stringId = 0x184;
				if (gBankTarget == FOE(gBankAttacker)
				&& !(gNewBS->ResultFlags[PARTNER(gBankTarget)] & MOVE_RESULT_NO_EFFECT)
				&& !gNewBS->noResultString[PARTNER(gBankTarget)]
				&& gNewBS->criticalMultiplier[PARTNER(gBankTarget)] > BASE_CRIT_MULTIPLIER)
					gBattleStringLoader = gText_CriticalHitTwoFoes;
				else if (gBankTarget == PARTNER(FOE(gBankAttacker))
				&& !(gNewBS->ResultFlags[FOE(gBankAttacker)] & MOVE_RESULT_NO_EFFECT)
				&& !gNewBS->noResultString[FOE(gBankAttacker)]
				&& gNewBS->criticalMultiplier[FOE(gBankAttacker)] > BASE_CRIT_MULTIPLIER)
				{
					//Was handled or will be handled as a double string
					stringId = 0;
				}
				else
					gBattleStringLoader = gText_CriticalHitTarget;
			}
		}
		else if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT))
			stringId = STRINGID_CRITICALHIT;
	
		if (stringId != 0)
		{
			++gNewBS->criticalHitsThisBattle[gBattlerPartyIndexes[gBankAttacker]];
			PrepareStringBattle(stringId, gBankAttacker);
			gBattleCommunication[MSG_DISPLAY] = 1;
		}
	}

	gBattlescriptCurrInstr++;
}

void atk0E_effectivenesssound(void)
{
	if (gBattleExecBuffer)
		return;

	u8 resultFlags = gMoveResultFlags;

	if (IsDoubleSpreadMove())
	{
		if (gNewBS->doneDoublesSpreadHit
		|| !gNewBS->calculatedSpreadMoveData //The attack animation didn't play yet - only play sound after animation
		|| SPLIT(gCurrentMove) == SPLIT_STATUS) //To handle Dark Void missing basically
			goto END;
		resultFlags = UpdateEffectivenessResultFlagsForDoubleSpreadMoves(resultFlags);
	}
	else if (!(resultFlags & MOVE_RESULT_NO_EFFECT) && DoesBankNegateDamage(gBankTarget, gCurrentMove)) //Mimikyu disguised
		resultFlags = 0;

	gActiveBattler = gBankTarget;
	if (!(resultFlags & MOVE_RESULT_MISSED))
	{
		switch (resultFlags & (u8)(~(MOVE_RESULT_MISSED))) {
			case MOVE_RESULT_SUPER_EFFECTIVE:
				EmitPlaySE(0, SE_SUPER_EFFECTIVE);
				MarkBufferBankForExecution(gActiveBattler);
				break;
			case MOVE_RESULT_NOT_VERY_EFFECTIVE:
				EmitPlaySE(0, SE_NOT_VERY_EFFECTIVE);
				MarkBufferBankForExecution(gActiveBattler);
				break;
			case MOVE_RESULT_DOESNT_AFFECT_FOE:
			case MOVE_RESULT_FAILED:
				//No sound
				break;
			case MOVE_RESULT_FOE_ENDURED:
			case MOVE_RESULT_ONE_HIT_KO:
			case MOVE_RESULT_FOE_HUNG_ON:
			default:
				if (resultFlags & MOVE_RESULT_SUPER_EFFECTIVE)
				{
					EmitPlaySE(0, SE_SUPER_EFFECTIVE);
					MarkBufferBankForExecution(gActiveBattler);
				}
				else if (resultFlags & MOVE_RESULT_NOT_VERY_EFFECTIVE)
				{
					EmitPlaySE(0, SE_NOT_VERY_EFFECTIVE);
					MarkBufferBankForExecution(gActiveBattler);
				}
				else if (!(resultFlags & (MOVE_RESULT_DOESNT_AFFECT_FOE | MOVE_RESULT_FAILED)))
				{
					EmitPlaySE(0, SE_EFFECTIVE);
					MarkBufferBankForExecution(gActiveBattler);
				}
				break;
			}
	}

END:
	gBattlescriptCurrInstr++;
}

static bool8 ShouldPrintTwoFoesMessage(u8 moveResult)
{
	return gBankTarget == FOE(gBankAttacker)
		&& gNewBS->ResultFlags[PARTNER(gBankTarget)] & moveResult
		&& !gNewBS->noResultString[PARTNER(gBankTarget)];
}

static bool8 ShouldRelyOnTwoFoesMessage(u8 moveResult)
{
	return gBankTarget == PARTNER(FOE(gBankAttacker))
		&& gNewBS->ResultFlags[FOE(gBankAttacker)] & moveResult
		&& !(gNewBS->ResultFlags[FOE(gBankAttacker)] & MOVE_RESULT_MISSED && gNewBS->missStringId[FOE(gBankAttacker)] > 2) //Partner was missed
		&& !gNewBS->noResultString[FOE(gBankAttacker)];
}

void atk0F_resultmessage(void)
{
	u32 stringId = 0;

	if (gBattleExecBuffer) return;

	if (gMoveResultFlags & MOVE_RESULT_MISSED && (!(gMoveResultFlags & MOVE_RESULT_DOESNT_AFFECT_FOE) || gNewBS->missStringId[gBankTarget] > 2))
	{
		gNewBS->MetronomeCounter[gBankAttacker] = 0;

		if (gNewBS->missStringId[gBankTarget] == 3 || gNewBS->missStringId[gBankTarget] == 4) //Levitate + Wonder Guard
		{
			BattleScriptPush(gBattlescriptCurrInstr + 1);
			gBattleScripting.bank = gBankTarget;
			gBattlescriptCurrInstr = BattleScript_AvoidedMoveWithAbility;
			gBattleCommunication[MSG_DISPLAY] = 0;
			return;
		}
		else
		{
			stringId = gMissStringIds[gNewBS->missStringId[gBankTarget]];
			gBattleCommunication[MSG_DISPLAY] = 1;
		}
	}
	else
	{
		gBattleCommunication[MSG_DISPLAY] = 1;
		switch (gMoveResultFlags & (u8)(~(MOVE_RESULT_MISSED))) {
		case MOVE_RESULT_SUPER_EFFECTIVE:
			if (IsDoubleSpreadMove())
			{
				stringId = 0x184;
				if (ShouldPrintTwoFoesMessage(MOVE_RESULT_SUPER_EFFECTIVE))
					gBattleStringLoader = gText_SuperEffectiveTwoFoes;
				else if (ShouldRelyOnTwoFoesMessage(MOVE_RESULT_SUPER_EFFECTIVE))
				{
					//Was handled or will be handled as a double string
					stringId = 0;
				}
				else
					gBattleStringLoader = gText_SuperEffectiveTarget;
			}
			else if (gMultiHitCounter <= 1 || !BATTLER_ALIVE(gBankTarget)) //Only print once after multi-hit moves - always when KOing
				stringId = STRINGID_SUPEREFFECTIVE;
			break;

		case MOVE_RESULT_NOT_VERY_EFFECTIVE:
			if (IsDoubleSpreadMove())
			{
				stringId = 0x184;
				if (ShouldPrintTwoFoesMessage(MOVE_RESULT_NOT_VERY_EFFECTIVE))
					gBattleStringLoader = gText_NotVeryEffectiveTwoFoes;
				else if (ShouldRelyOnTwoFoesMessage(MOVE_RESULT_NOT_VERY_EFFECTIVE))
				{
					//Was handled or will be handled as a double string
					stringId = 0;
				}
				else
					gBattleStringLoader = gText_NotVeryEffectiveTarget;
			}
			else if (gMultiHitCounter <= 1 || !BATTLER_ALIVE(gBankTarget)) //Only print once after multi-hit moves - always when KOing
				stringId = STRINGID_NOTVERYEFFECTIVE;
			break;

		case MOVE_RESULT_ONE_HIT_KO:
			stringId = STRINGID_ONEHITKO;
			break;

		case MOVE_RESULT_FOE_ENDURED:
			if (gNewBS->EnduranceHelper[gBankTarget] == ENDURE_STURDY)
			{
				gMoveResultFlags &= ~(MOVE_RESULT_FOE_ENDURED);
				gNewBS->EnduranceHelper[gBankTarget] = 0;
				gProtectStructs[gBankTarget].enduredSturdy = 0;
				gBattleScripting.bank = gBankTarget;
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_EnduredSturdy;
				return;
			}
			else
				stringId = STRINGID_PKMNENDUREDHIT;
			break;

		case MOVE_RESULT_FAILED:
			gNewBS->MetronomeCounter[gBankAttacker] = 0;
			stringId = STRINGID_BUTITFAILED;
			break;

		case MOVE_RESULT_DOESNT_AFFECT_FOE:
			if (IsDoubleSpreadMove())
			{
				stringId = 0x184;
				if (ShouldPrintTwoFoesMessage(MOVE_RESULT_DOESNT_AFFECT_FOE))
				{
					gNewBS->MetronomeCounter[gBankAttacker] = 0;
					gBattleStringLoader = gText_DoesntAffectTwoFoes;
				}
				else if (ShouldRelyOnTwoFoesMessage(MOVE_RESULT_DOESNT_AFFECT_FOE))
				{
					//Was handled or will be handled as a double string
					stringId = 0;
				}
				else
					stringId = STRINGID_ITDOESNTAFFECT;
			}
			else
			{
				gNewBS->MetronomeCounter[gBankAttacker] = 0;
				stringId = STRINGID_ITDOESNTAFFECT;
			}
			break;

		case MOVE_RESULT_FOE_HUNG_ON:
			gLastUsedItem = ITEM(gBankTarget);
			gStringBank = gBankTarget;
			gMoveResultFlags &= ~(MOVE_RESULT_FOE_ENDURED | MOVE_RESULT_FOE_HUNG_ON);
			BattleScriptPushCursor();
			if (gNewBS->EnduranceHelper[gBankTarget] == ENDURE_FOCUS_SASH)
			{
				gNewBS->EnduranceHelper[gBankTarget] = 0;
				gBattlescriptCurrInstr = BattleScript_HangedOnFocusSash;
			}
			else
				gBattlescriptCurrInstr = BattleScript_HangedOnMsg;
			return;

		default:
			if (gMoveResultFlags & MOVE_RESULT_DOESNT_AFFECT_FOE)
			{
				if (IsDoubleSpreadMove())
				{
					stringId = 0x184;
					if (ShouldPrintTwoFoesMessage(MOVE_RESULT_DOESNT_AFFECT_FOE))
					{
						gNewBS->MetronomeCounter[gBankAttacker] = 0;
						gBattleStringLoader = gText_DoesntAffectTwoFoes;
					}
					else if (ShouldRelyOnTwoFoesMessage(MOVE_RESULT_DOESNT_AFFECT_FOE))
					{
						//Was handled or will be handled as a double string
						stringId = 0;
					}
					else
						stringId = STRINGID_ITDOESNTAFFECT;
				}
				else
				{
					gNewBS->MetronomeCounter[gBankAttacker] = 0;
					stringId = STRINGID_ITDOESNTAFFECT;
				}
			}
			else if (gMoveResultFlags & MOVE_RESULT_ONE_HIT_KO)
			{
				gMoveResultFlags &= ~(MOVE_RESULT_ONE_HIT_KO);
				gMoveResultFlags &= ~(MOVE_RESULT_SUPER_EFFECTIVE);
				gMoveResultFlags &= ~(MOVE_RESULT_NOT_VERY_EFFECTIVE);
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_OneHitKOMsg;
				return;
			}
			else if (gMoveResultFlags & MOVE_RESULT_FOE_ENDURED)
			{
				gMoveResultFlags &= ~(MOVE_RESULT_FOE_ENDURED | MOVE_RESULT_FOE_HUNG_ON);
				BattleScriptPushCursor();
				if (gNewBS->EnduranceHelper[gBankTarget] == ENDURE_STURDY)
				{
					gNewBS->EnduranceHelper[gBankTarget] = 0;
					gProtectStructs[gBankTarget].enduredSturdy = 0;
					gBattleScripting.bank = gBankTarget;
					gBattlescriptCurrInstr = BattleScript_EnduredSturdy;
				}
				else
					gBattlescriptCurrInstr = BattleScript_EnduredMsg;
				return;
			}
			else if (gMoveResultFlags & MOVE_RESULT_FOE_HUNG_ON)
			{
				gLastUsedItem = gBattleMons[gBankTarget].item;
				gStringBank = gBankTarget;
				gMoveResultFlags &= ~(MOVE_RESULT_FOE_ENDURED | MOVE_RESULT_FOE_HUNG_ON);
				BattleScriptPushCursor();
				if (gNewBS->EnduranceHelper[gBankTarget] == ENDURE_FOCUS_SASH)
				{
					gNewBS->EnduranceHelper[gBankTarget] = 0;
					gBattlescriptCurrInstr = BattleScript_HangedOnFocusSash;
				}
				else
					gBattlescriptCurrInstr = BattleScript_HangedOnMsg;

				gSpecialStatuses[gBankTarget].focusBanded = FALSE;
				return;
			}
			else if (gMoveResultFlags & MOVE_RESULT_FAILED)
			{
				gNewBS->MetronomeCounter[gBankAttacker] = 0;
				stringId = STRINGID_BUTITFAILED;
			}
			else
			{
				gBattleCommunication[MSG_DISPLAY] = 0;
			}
		}

		gNewBS->EnduranceHelper[gBankTarget] = 0; //Clear these here for multi-hit moves that didn't KO target
		gSpecialStatuses[gBankTarget].focusBanded = FALSE;
		gProtectStructs[gBankTarget].enduredSturdy = FALSE;
	}

	if (stringId != 0)
		PrepareStringBattle(stringId, gBankAttacker);
	else
		gBattleCommunication[MSG_DISPLAY] = 0;

	gBattlescriptCurrInstr++;

	if ((gNewBS->zMoveData.active || IsAnyMaxMove(gCurrentMove))
	&& !IsDynamaxed(gBankTarget) && ProtectsAgainstZMoves(gCurrentMove, gBankAttacker, gBankTarget)
	&& !(gMoveResultFlags & MOVE_RESULT_DOESNT_AFFECT_FOE))
	{
		BattleScriptPushCursor();
		gBattlescriptCurrInstr = BattleScript_CouldntFullyProtect;
	}
}

void atk12_waitmessage(void)
{
	if (gBattleExecBuffer == 0)
	{
		if (!gBattleCommunication[MSG_DISPLAY])
		{
			gBattlescriptCurrInstr += 3;
		}
		else
		{
			u16 toWait = T2_READ_16(gBattlescriptCurrInstr + 1);
			if (++gPauseCounterBattle >= toWait
			#ifdef FLAG_FAST_BATTLE_MESSAGES
			|| FlagGet(FLAG_FAST_BATTLE_MESSAGES)
			#endif
			|| gMain.newKeys & A_BUTTON)
			{
				gPauseCounterBattle = 0;
				gBattlescriptCurrInstr += 3;
				gBattleCommunication[MSG_DISPLAY] = 0;
			}
		}
	}
}

void atk19_tryfaintmon(void)
{
	const u8* BS_ptr;

	if (gBattlescriptCurrInstr[2] != 0)
	{
		gActiveBattler = GetBankForBattleScript(gBattlescriptCurrInstr[1]);
		if (gHitMarker & HITMARKER_FAINTED(gActiveBattler) && gNewBS->endTurnDone) //To prevent things like Whirlwind from activating this
		{
			BS_ptr = T1_READ_PTR(gBattlescriptCurrInstr + 3);

			BattleScriptPop();
			gBattlescriptCurrInstr = BS_ptr;
			gSideStatuses[SIDE(gActiveBattler)] &= ~(SIDE_STATUS_SPIKES_DAMAGED);
		}
		else
		{
			gBattlescriptCurrInstr += 7;
		}
	}
	else
	{
		u8 bank;

		if (gBattlescriptCurrInstr[1] == BS_GET_ATTACKER)
		{
			gActiveBattler = gBankAttacker;
			bank = gBankAttacker;

			if (IsCatchableRaidBattle() && bank == BANK_RAID_BOSS)
				BS_ptr = BattleScript_FaintRaidAttacker;
			else
				BS_ptr = BattleScript_FaintAttacker;
		}
		else if (gBattlescriptCurrInstr[1] == BS_GET_SCRIPTING_BANK)
		{
			gActiveBattler = gBattleScripting.bank;
			bank = gBattleScripting.bank;

			if (IsCatchableRaidBattle() && bank == BANK_RAID_BOSS)
				BS_ptr = BattleScript_FaintRaidScriptingBank;
			else
				BS_ptr = BattleScript_FaintScriptingBank;
		}
		else
		{
			gActiveBattler = gBankTarget;
			bank = gBankTarget;

			if (IsCatchableRaidBattle() && bank == BANK_RAID_BOSS)
				BS_ptr = BattleScript_FaintRaidTarget;
			else
				BS_ptr = BattleScript_FaintTarget;
		}

		if (TryDoBenjaminButterfree(7))
			return;

		if (!(gAbsentBattlerFlags & gBitTable[gActiveBattler])
		&& gBattleMons[gActiveBattler].hp == 0)
		{
			gNewBS->lastFainted = gActiveBattler;
			gHitMarker |= HITMARKER_FAINTED(gActiveBattler);
			BattleScriptPush(gBattlescriptCurrInstr + 7);
			gBattlescriptCurrInstr = BS_ptr;
			if (SIDE(gActiveBattler) == B_SIDE_PLAYER
			&& (!IsTagBattle() || GetBattlerPosition(gActiveBattler) == B_POSITION_OPPONENT_LEFT)) //Is player's mon
			{
				gHitMarker |= HITMARKER_PLAYER_FAINTED;
				if (gBattleResults.playerFaintCounter < 0xFF)
					gBattleResults.playerFaintCounter++;
				AdjustFriendshipOnBattleFaint(gActiveBattler);
			}
			else
			{
				if (gBattleResults.opponentFaintCounter < 0xFF)
					gBattleResults.opponentFaintCounter++;
				gBattleResults.lastOpponentSpecies = GetMonData(&gEnemyParty[gBattlerPartyIndexes[gActiveBattler]], MON_DATA_SPECIES, NULL);
			}

			gNewBS->RetaliateCounters[SIDE(gActiveBattler)] = 2;

			if ((gHitMarker & HITMARKER_DESTINYBOND)
			&& gBattleMons[gBankAttacker].hp != 0
			&& !IsDynamaxed(gBankAttacker)
			&& gBattlescriptCurrInstr[1] != BS_GET_SCRIPTING_BANK)
			{
				gHitMarker &= ~(HITMARKER_DESTINYBOND);
				const u8* backupScript = gBattlescriptCurrInstr;
				gBattlescriptCurrInstr = BattleScript_DestinyBondTakesLife;
				BattleScriptPushCursor();
				gBattleMoveDamage = gBattleMons[bank].hp;
				gBattlescriptCurrInstr = backupScript;
			}

			if ((gStatuses3[gBankTarget] & STATUS3_GRUDGE)
			 && !(gHitMarker & HITMARKER_GRUDGE)
			 && SIDE(gBankAttacker) != SIDE(gBankTarget)
			 && gBattleMons[gBankAttacker].hp != 0
			 && gCurrentMove != MOVE_STRUGGLE
			 && gBattlescriptCurrInstr[1] != BS_GET_SCRIPTING_BANK)
			{
				u8 moveIndex = gBattleStruct->chosenMovePositions[gBankAttacker];

				gBattleMons[gBankAttacker].pp[moveIndex] = 0;
				BattleScriptPush(gBattlescriptCurrInstr);
				gBattlescriptCurrInstr = BattleScript_GrudgeTakesPp;
				gActiveBattler = gBankAttacker;
				EmitSetMonData(0, moveIndex + REQUEST_PPMOVE1_BATTLE, 0, 1, &gBattleMons[gActiveBattler].pp[moveIndex]);
				MarkBufferBankForExecution(gActiveBattler);

				PREPARE_MOVE_BUFFER(gBattleTextBuff1, gBattleMons[gBankAttacker].moves[moveIndex])
			}
		}
		else
		{
			gBattlescriptCurrInstr += 7;
		}
	}
}

bool8 TryDoBenjaminButterfree(u8 scriptOffset)
{
	if (IsBenjaminButterfreeBattle() && !IS_TRANSFORMED(gActiveBattler) && !BATTLER_ALIVE(gActiveBattler))
	{
		u16 devolutionSpecies = GetMonDevolution(GetBankPartyData(gActiveBattler));
		if (devolutionSpecies != SPECIES_NONE)
		{
			PREPARE_SPECIES_BUFFER(gBattleTextBuff1, devolutionSpecies)
			DoFormChange(gActiveBattler, devolutionSpecies, TRUE, TRUE, TRUE);
			gEffectBank = gActiveBattler;
			BattleScriptPush(gBattlescriptCurrInstr + scriptOffset);
			gBattlescriptCurrInstr = BattleScript_BenjaminButterfreeDevolution;
			return TRUE;
		}
	}

	return FALSE;
}

void atk1B_cleareffectsonfaint(void) {
	gActiveBattler = GetBankForBattleScript(gBattlescriptCurrInstr[1]);
	u8 partner = PARTNER(gActiveBattler);
	struct Pokemon* mon = GetBankPartyData(gActiveBattler);

	if (!gBattleExecBuffer) {
		switch (gNewBS->faintEffectsState) {
			case Faint_DynamaxHP: //Update max HP if was dynamaxed
				if (IsDynamaxed(gActiveBattler))
				{
					gBattleMons[gActiveBattler].maxHP = GetBaseMaxHP(gActiveBattler);
					EmitSetMonData(0, REQUEST_MAX_HP_BATTLE, 0, 2, &gBattleMons[gActiveBattler].maxHP);
					MarkBufferBankForExecution(gActiveBattler);
					gNewBS->dynamaxData.timer[gActiveBattler] = 0;
					return;
				}
				++gNewBS->faintEffectsState;
				__attribute__ ((fallthrough));

			case Faint_ClearEffects:
				gBattleMons[gActiveBattler].status1 = 0;
				EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 0x4, &gBattleMons[gActiveBattler].status1);
				MarkBufferBankForExecution(gActiveBattler);

				{
					u32 backupStatus2[gBattlersCount];
					for (int i = 0; i < gBattlersCount; ++i)
						backupStatus2[i] = gBattleMons[i].status2;
					UndoEffectsAfterFainting();
					TryReactivateCentifernoSandblast(backupStatus2);
				}

				ClearSwitchBytes(gActiveBattler);
				ClearSwitchBits(gActiveBattler);
				gNewBS->ai.switchesInARow[gActiveBattler] = 0; //Will become 1 when player sends in new mon, proccing cheese counter if they try switching out again twice
				gNewBS->ai.previousMonIn[gActiveBattler] = 0xFF; //Reset since mon fainted
				gNewBS->ai.secondPreviousMonIn[gActiveBattler] = 0xFF; //Reset since mon fainted

				gBattleMons[gActiveBattler].type3 = TYPE_BLANK;
				*gSeedHelper = 0;
				++gNewBS->faintEffectsState;
				return;

			case Faint_SoulHeart:
				for (; *gSeedHelper < gBattlersCount; ++*gSeedHelper)
				{
					u8 bank = *gSeedHelper;
					if (ABILITY(bank) == ABILITY_SOULHEART
					&&  bank != gActiveBattler
					&&  gBattleMons[bank].hp != 0
					&&  STAT_CAN_RISE(bank, STAT_SPATK)
					&&  ViableMonCountFromBank(FOE(bank)) > 0) //Foe still has mons
					{
						++gBattleMons[bank].statStages[STAT_SPATK - 1];
						gBattleScripting.bank = gEffectBank = bank;

						PREPARE_STAT_BUFFER(gBattleTextBuff1, STAT_STAGE_SPATK);
						PREPARE_STAT_ROSE(gBattleTextBuff2);
						gBattleScripting.animArg1 = 0xE + STAT_STAGE_SPATK;
						gBattleScripting.animArg2 = 0;

						gLastUsedAbility = ABILITY_SOULHEART;

						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_SoulHeart;
						++*gSeedHelper;
						return;
					}
				}
				++gNewBS->faintEffectsState;
			__attribute__ ((fallthrough));

			case Faint_ReceiverActivate:
				gNewBS->ReceiverActivated = FALSE;
				u8 partnerAbility = ABILITY(partner);

				if (IS_DOUBLE_BATTLE
				&& (partnerAbility == ABILITY_RECEIVER
				#ifdef ABILITY_POWEROFALCHEMY
				|| partnerAbility == ABILITY_POWEROFALCHEMY
				#endif
				)
				&& !gSpecialAbilityFlags[CopyAbility(gActiveBattler)].gReceiverBannedAbilities)
				{
					gLastUsedAbility = partnerAbility;
					SetProperAbilityPopUpSpecies(partner); //Must be set up here before the Ability changes
					*GetAbilityLocation(partner) = CopyAbility(gActiveBattler);
					SetTookAbilityFrom(partner, gActiveBattler);
					gEffectBank = gActiveBattler;
					gBattleScripting.bank = partner;
					gNewBS->ReceiverActivated = TRUE;
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_Receiver;

					gAbilityPopUpHelper = gLastUsedAbility;
					EmitDataTransfer(0, &gAbilityPopUpHelper, 3, &gAbilityPopUpHelper); //Copy Ability and Species
					MarkBufferBankForExecution(gActiveBattler);

					++gNewBS->faintEffectsState;
					return;
				}
				++gNewBS->faintEffectsState;
			__attribute__ ((fallthrough));

			case Faint_SwitchInAbilities: //Now null b/c handled in BS
				++gNewBS->faintEffectsState;
			__attribute__ ((fallthrough));

			case Faint_PrimalWeather:
				if (HandleSpecialSwitchOutAbilities(gActiveBattler, ABILITY(gActiveBattler), TRUE))
					return;

				mon = GetBankPartyData(gActiveBattler); //Mon gets overwritten by Neutralizing Gas for some reason
				++gNewBS->faintEffectsState;
			__attribute__ ((fallthrough));

			case Faint_SkyDrop:
				++gNewBS->faintEffectsState;
				u32 status3;
				u8 bankToFree;

				//Determine if this mon was active in a Sky Drop
				for (bankToFree = 0, status3 = 0; bankToFree < gBattlersCount; ++bankToFree)
				{
					if (gStatuses3[bankToFree] & STATUS3_SKY_DROP_ATTACKER)
					{
						if (gNewBS->skyDropAttackersTarget[bankToFree] == gActiveBattler)
						{
							status3 = STATUS3_SKY_DROP_TARGET; //The fainted bank was a target of Sky Drop
							break;
						}
					}
					else if (gStatuses3[bankToFree] & STATUS3_SKY_DROP_TARGET)
					{
						if (gNewBS->skyDropTargetsAttacker[bankToFree] == gActiveBattler)
						{
							status3 = STATUS3_SKY_DROP_ATTACKER; //The fainted bank was a user of Sky Drop
							break;
						}
					}
				}

				if (status3 & STATUS3_SKY_DROP_ANY) //The fainted bank was active in a Sky Drop
				{
					if (status3 & STATUS3_SKY_DROP_ATTACKER) //The fainted bank was the user of a Sky Drop
					{
						//So free the Pokemon it had held up in the air
						gNewBS->skyDropAttackersTarget[gActiveBattler] = 0;
						gNewBS->skyDropTargetsAttacker[bankToFree] = 0;

						//A message is only printed when the target is freed.
						gBattleScripting.bank = bankToFree;
						gBattleStringLoader = FreedFromSkyDropString;
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_PrintCustomString;

						gActiveBattler = bankToFree;
						EmitSpriteInvisibility(0, FALSE);
						MarkBufferBankForExecution(gActiveBattler);
					}
					else if (status3 & STATUS3_SKY_DROP_TARGET) //The fainted bank was the target of a Sky Drop
					{
						//So free the Pokemon that was holding it in the air
						CancelMultiTurnMoves(bankToFree);
						gNewBS->skyDropAttackersTarget[bankToFree] = 0;
						gNewBS->skyDropTargetsAttacker[gActiveBattler] = 0;
						gNewBS->NoMoreMovingThisTurn |= gBitTable[bankToFree]; //So it doesn't try using Sky Drop again
						gActiveBattler = bankToFree;
						EmitSpriteInvisibility(0, FALSE);
						MarkBufferBankForExecution(gActiveBattler);
					}
					else
						goto END_SKY_DROP_CHECK;

					gStatuses3[gActiveBattler] &= ~(STATUS3_SKY_DROP_ANY | STATUS3_IN_AIR);
					gStatuses3[bankToFree] &= ~(STATUS3_SKY_DROP_ANY | STATUS3_IN_AIR);
					return;
				}
				END_SKY_DROP_CHECK: ;
			__attribute__ ((fallthrough));

			case Faint_RaidBattle:
				++gNewBS->faintEffectsState;
			#ifdef FLAG_RAID_BATTLE
				if (IsRaidBattle() && SIDE(gActiveBattler) == B_SIDE_PLAYER && BATTLER_ALIVE(BANK_RAID_BOSS))
				{
					u8 raidBank = BANK_RAID_BOSS;
					const u8* stormString = NULL;
					++gNewBS->dynamaxData.stormLevel;

					if (gCurrentMove != MOVE_STRUGGLE && Random() % 100 < GetRaidBattleRepeatedAttackChance(raidBank))
						gNewBS->dynamaxData.attackAgain = TRUE; //Set indicator to attack again after CMD49 finishes

					if ((Random() & 1)) //50 % Chance
					{
						u8 statId, increase;
						increase = GetRaidBattleKOStatIncreaseAmount(raidBank);

						if (increase > 0)
						{
							if ((Random() & 1) == 0)
								statId = STAT_STAGE_ATK;
							else
								statId = STAT_STAGE_SPATK;

							if (ABILITY(raidBank) == ABILITY_CONTRARY)
								increase *= -1; //So it still gets the boost
							gBattleScripting.statChanger = statId | SET_STAT_BUFF_VALUE(increase);

							u8 backup = gBankAttacker;
							gBankAttacker = raidBank;
							if (!ChangeStatBuffs(SET_STAT_BUFF_VALUE(increase), statId, MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_CERTAIN, NULL))
							{
								gBattleScripting.bank = raidBank;
								BattleScriptPushCursor();
								gBattlescriptCurrInstr = BattleScript_RaidBattleStatIncrease;
							}
							gBankAttacker = backup;
						}
					}

					#ifdef FLAG_RAID_BATTLE_NO_FORCE_END
					if (FlagGet(FLAG_RAID_BATTLE_NO_FORCE_END))
						return; //Don't force end this battle
					#endif

					BattleScriptPushCursor();

					switch (gNewBS->dynamaxData.stormLevel) {
						case 1:
							stormString = gText_RaidBattleKO1;
							gBattlescriptCurrInstr = BattleScript_RaidBattleStorm;
							break;
						case 2:
							stormString = gText_RaidBattleKO2;
							gBattlescriptCurrInstr = BattleScript_RaidBattleStorm;
							break;
						case 3:
							stormString = gText_RaidBattleKO3;
							gBattlescriptCurrInstr = BattleScript_RaidBattleStorm;
							break;
						case 4:
							stormString = gText_RaidBattleKO4;
							gBattleScripting.bank = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
							gBattlescriptCurrInstr = BattleScript_LoseRaidBattle;
							break;
					}

					gBattleStringLoader = stormString;
					return;
				}
			#endif
				//Fallthrough
			case Faint_LastPokemonMusic:
				#ifdef BGM_BATTLE_GYM_LEADER_LAST_POKEMON
				if ((gBattleTypeFlags & (BATTLE_TYPE_TRAINER | BATTLE_TYPE_DOUBLE)) == (BATTLE_TYPE_TRAINER | BATTLE_TYPE_DOUBLE) //Double Gym battle
				&& !(gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_FRONTIER | BATTLE_TYPE_TRAINER_TOWER))
				&& gTrainers[gTrainerBattleOpponent_A].trainerClass == CLASS_LEADER
				&& SIDE(gActiveBattler) == B_SIDE_OPPONENT //Enemy mon fainted
				&& ViableMonCount(gEnemyParty) == 1) //1 left exactly
				{
					PlayBGM(BGM_BATTLE_GYM_LEADER_LAST_POKEMON);
				}
				#endif
				++gNewBS->faintEffectsState;
				//Fallthrough

			case Faint_FormsRevert:
				if (!IsRaidBattle() || gActiveBattler != BANK_RAID_BOSS) //Raid Boss doesn't revert until later
				{
					if (TryFormRevert(mon))
					{
						EmitSetMonData(0, REQUEST_SPECIES_BATTLE, 0, 2, &mon->species);
						MarkBufferBankForExecution(gActiveBattler);
						mon->backupSpecies = SPECIES_NONE;
						++gNewBS->faintEffectsState;
						return;
					}
				}
				break; //No form change means skip the next two states

			case Faint_FormsStats:
				CalculateMonStats(mon);
				EmitSetRawMonData(0, offsetof(struct Pokemon, attack), 2 /*Atk*/ + 2 /*Def*/ + 2 /*Spd*/ + 2 */*Sp Atk*/ + 2 /*Sp Def*/, &mon->attack); //Reload all stats
				MarkBufferBankForExecution(gActiveBattler);
				++gNewBS->faintEffectsState;
				return;

			case Faint_FormsHP: ;
				u16 oldHP, newHP;
				oldHP = mon->hp;

				#if (defined SPECIES_ZYGARDE && defined SPECIES_ZYGARDE_10)
				if (mon->species == SPECIES_ZYGARDE || mon->species == SPECIES_ZYGARDE_10)
				{
					newHP = MathMin(mon->maxHP, oldHP);
					EmitSetMonData(0, REQUEST_HP_BATTLE, 0, 2, &newHP);
					MarkBufferBankForExecution(gActiveBattler);
					++gNewBS->faintEffectsState;
					return;
				}
				#endif
		}

		gNewBS->faintEffectsState = 0;
		gBattlescriptCurrInstr += 2;
	}
}

void atk1D_jumpifstatus2(void) {
	u8 bank = GetBankForBattleScript(gBattlescriptCurrInstr[1]);
	u32 flags = T2_READ_32(gBattlescriptCurrInstr + 2);
	void* jump_loc = T2_READ_PTR(gBattlescriptCurrInstr + 6);

	if (gBattlescriptCurrInstr[1] == BS_GET_TARGET && !MoveBlockedBySubstitute(gCurrentMove, gBankAttacker, bank))
		flags &= ~(STATUS2_SUBSTITUTE);

	if (gBattleMons[bank].status2 & flags && BATTLER_ALIVE(bank))
		gBattlescriptCurrInstr = jump_loc;
	else
		gBattlescriptCurrInstr += 10;
}

void atk1F_jumpifsideaffecting(void)
{
	u8 side;
	u16 flags = T2_READ_16(gBattlescriptCurrInstr + 2);
	void* jumpLoc = T2_READ_PTR(gBattlescriptCurrInstr + 4);

	if (gBattlescriptCurrInstr[1] == BS_GET_ATTACKER)
	{
		side = SIDE(gBankAttacker);
		if (BankSideHasSafeguard(gBankAttacker))
			flags |= SIDE_STATUS_SAFEGUARD;
		if (BankSideHasMist(gBankAttacker))
			flags |= SIDE_STATUS_MIST;
	}
	else
	{
		side = SIDE(gBankTarget);
		if (BankSideHasSafeguard(gBankTarget))
			flags |= SIDE_STATUS_SAFEGUARD;
		if (BankSideHasMist(gBankTarget))
			flags |= SIDE_STATUS_MIST;
		if (BypassesScreens(ABILITY(gBankTarget)))
			flags &= ~(SIDE_STATUS_SAFEGUARD | SIDE_STATUS_MIST);
	}

	if (gSideStatuses[side] & flags)
		gBattlescriptCurrInstr = jumpLoc;
	else
		gBattlescriptCurrInstr += 8;
}

void atk1E_jumpifability(void)
{
	u8 battlerId;
	u8 ability = gBattlescriptCurrInstr[2];
	const u8* jumpPtr = T2_READ_PTR(gBattlescriptCurrInstr + 3);

	if (gBattlescriptCurrInstr[1] == BS_GET_ATTACKER_SIDE)
	{
		battlerId = AbilityBattleEffects(ABILITYEFFECT_CHECK_BANK_SIDE, gBankAttacker, ability, 0, 0);
		if (battlerId)
		{
			gLastUsedAbility = ability;
			gBattlescriptCurrInstr = jumpPtr;
			RecordAbilityBattle(battlerId - 1, gLastUsedAbility);
			gBattleScripting.bankWithAbility = battlerId - 1;
		}
		else
			gBattlescriptCurrInstr += 7;
	}
	else if (gBattlescriptCurrInstr[1] == BS_GET_NOT_ATTACKER_SIDE)
	{
		battlerId = ABILITY_ON_OPPOSING_FIELD(gBankAttacker, ability);
		if (battlerId)
		{
			gLastUsedAbility = ability;
			gBattlescriptCurrInstr = jumpPtr;
			RecordAbilityBattle(battlerId - 1, gLastUsedAbility);
			gBattleScripting.bankWithAbility = battlerId - 1;
		}
		else
			gBattlescriptCurrInstr += 7;
	}
	else
	{
		battlerId = GetBankForBattleScript(gBattlescriptCurrInstr[1]);
		if (ABILITY(battlerId) == ability)
		{
			gLastUsedAbility = ability;
			gBattlescriptCurrInstr = jumpPtr;
			RecordAbilityBattle(battlerId, gLastUsedAbility);
			gBattleScripting.bankWithAbility = battlerId;
		}
		else
			gBattlescriptCurrInstr += 7;
	}
}

void atk22_jumpiftype(void) //u8 bank, u8 type, *ptr
{
	u8 bank = GetBankForBattleScript(gBattlescriptCurrInstr[1]);
	u8 type = gBattlescriptCurrInstr[2];
	void* jump_loc = T2_READ_PTR(gBattlescriptCurrInstr + 3);

	if (IsOfType(bank, type))
		gBattlescriptCurrInstr = jump_loc;
	else
		gBattlescriptCurrInstr += 7;
}

void MoveValuesCleanUp(void)
{
	gMoveResultFlags = 0;
	gBattleScripting.dmgMultiplier = 1;
	gCritMultiplier = BASE_CRIT_MULTIPLIER;
	gBattleCommunication[MOVE_EFFECT_BYTE] = 0;
	gNewBS->backupMoveEffect = 0;
	gBattleCommunication[6] = 0;
	gHitMarker &= ~(HITMARKER_DESTINYBOND);
	gHitMarker &= ~(HITMARKER_SYNCHRONISE_EFFECT);
}

void atk27_decrementmultihit(void)
{
	if (gMultiHitCounter == 0 || --gMultiHitCounter == 0)
		gBattlescriptCurrInstr += 5;
	else
		gBattlescriptCurrInstr = T2_READ_PTR(gBattlescriptCurrInstr + 1);
}

void atk40_jumpifaffectedbyprotect(void)
{
	if (ProtectAffects(gCurrentMove, gBankAttacker, gBankTarget, FALSE))
	{
		gMoveResultFlags |= MOVE_RESULT_MISSED;
		JumpIfMoveFailed(5, 0);
		gBattleCommunication[6] = 1;
	}
	else
		gBattlescriptCurrInstr += 5;
}

void atk42_jumpiftype2(void) //u8 bank, u8 type, *ptr
{
	u8 bank = GetBankForBattleScript(gBattlescriptCurrInstr[1]);

	if (IsOfType(bank, gBattlescriptCurrInstr[2]))
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 3);
	else
		gBattlescriptCurrInstr += 7;
}

#define CASE_PLUS(by, stat) case (STAT_ANIM_PLUS##by + stat - 1): value = STAT_ANIM_MINUS##by + stat - 1; break;
#define CASE_MINUS(by, stat) case (STAT_ANIM_MINUS##by + stat - 1): value = STAT_ANIM_PLUS##by + stat - 1; break;
#define CASE_MULTIPLE_PLUS(by) case (STAT_ANIM_MULTIPLE_PLUS##by): value = STAT_ANIM_MINUS##by; break;
#define CASE_MULTIPLE_MINUS(by) case (STAT_ANIM_MULTIPLE_MINUS##by): value = STAT_ANIM_PLUS##by; break;
static void TryContraryChangeStatAnim(u8 bank, u16* argumentPtr)
{
	if (ABILITY(bank) == ABILITY_CONTRARY)
	{
		u8 value = 0;
		switch (*argumentPtr)
		{
			CASE_PLUS(1,  STAT_ATK)
			CASE_PLUS(1,  STAT_DEF)
			CASE_PLUS(1,  STAT_SPEED)
			CASE_PLUS(1,  STAT_SPATK)
			CASE_PLUS(1,  STAT_SPDEF)
			CASE_PLUS(1,  STAT_ACC)
			CASE_PLUS(1,  STAT_EVASION)

			CASE_MINUS(1, STAT_ATK)
			CASE_MINUS(1, STAT_DEF)
			CASE_MINUS(1, STAT_SPEED)
			CASE_MINUS(1, STAT_SPATK)
			CASE_MINUS(1, STAT_SPDEF)
			CASE_MINUS(1, STAT_ACC)
			CASE_MINUS(1, STAT_EVASION)

			CASE_PLUS(2,  STAT_ATK)
			CASE_PLUS(2,  STAT_DEF)
			CASE_PLUS(2,  STAT_SPEED)
			CASE_PLUS(2,  STAT_SPATK)
			CASE_PLUS(2,  STAT_SPDEF)
			CASE_PLUS(2,  STAT_ACC)
			CASE_PLUS(2,  STAT_EVASION)

			CASE_MINUS(2, STAT_ATK)
			CASE_MINUS(2, STAT_DEF)
			CASE_MINUS(2, STAT_SPEED)
			CASE_MINUS(2, STAT_SPATK)
			CASE_MINUS(2, STAT_SPDEF)
			CASE_MINUS(2, STAT_ACC)
			CASE_MINUS(2, STAT_EVASION)

			CASE_MULTIPLE_PLUS(1)
			CASE_MULTIPLE_PLUS(2)
			CASE_MULTIPLE_MINUS(1)
			CASE_MULTIPLE_MINUS(2)
		}

		*argumentPtr = value;
	}
}
#undef CASE_PLUS
#undef CASE_MINUS
#undef CASE_MULTIPLE_PLUS
#undef CASE_MULTIPLE_MINUS

static bool8 IsFancyTransformationAnimId(u8 animId)
{
	return animId == B_ANIM_MEGA_EVOLUTION
		|| animId == B_ANIM_BLUE_PRIMAL_REVERSION
		|| animId == B_ANIM_RED_PRIMAL_REVERSION
		|| animId == B_ANIM_ULTRA_BURST
		|| animId == B_ANIM_WISHIWASHI_FISH
		|| animId == B_ANIM_ZYGARDE_CELL_SWIRL;
}

static bool8 ShouldAnimBeDoneEvenWithBattleAnimsOff(u8 animId)
{
	return animId == B_ANIM_CASTFORM_CHANGE
		|| animId == B_ANIM_STATS_CHANGE
		|| animId == B_ANIM_CALL_BACK_POKEMON
		|| animId == B_ANIM_CALL_BACK_POKEMON_2
		|| animId == B_ANIM_SNATCH_MOVE
		|| animId == B_ANIM_SUBSTITUTE_FADE
		|| animId == B_ANIM_TRANSFORM
		|| animId == B_ANIM_WISHIWASHI_FISH
		|| animId == B_ANIM_ZYGARDE_CELL_SWIRL
		|| animId == B_ANIM_BLUE_PRIMAL_REVERSION
		|| animId == B_ANIM_RED_PRIMAL_REVERSION
		|| animId == B_ANIM_MEGA_EVOLUTION
		|| animId == B_ANIM_ULTRA_BURST
		|| animId == B_ANIM_ELECTRIC_SURGE
		|| animId == B_ANIM_GRASSY_SURGE
		|| animId == B_ANIM_MISTY_SURGE
		|| animId == B_ANIM_PSYCHIC_SURGE
		|| animId == B_ANIM_ELECTRIC_TERRAIN_ACTIVE
		|| animId == B_ANIM_GRASSY_TERRAIN_ACTIVE
		|| animId == B_ANIM_MISTY_TERRAIN_ACTIVE
		|| animId == B_ANIM_PSYCHIC_TERRAIN_ACTIVE
		|| animId == B_ANIM_LOAD_DEFAULT_BG
		|| animId == B_ANIM_LOAD_ABILITY_POP_UP
		|| animId == B_ANIM_DESTROY_ABILITY_POP_UP
		|| animId == B_ANIM_DYNAMAX_START
		|| animId == B_ANIM_POWDER_EXPLOSION
		|| animId == B_ANIM_BATON_PASS
		|| animId == B_ANIM_DRAGON_TAIL_BLOW_AWAY
		|| animId == B_ANIM_RAID_BATTLE_BLOW_AWAY;
}

void atk45_playanimation(void)
{
	u16* argumentPtr, animId;

	gActiveBattler = GetBankForBattleScript(gBattlescriptCurrInstr[1]);
	animId = gBattlescriptCurrInstr[2];
	argumentPtr = T2_READ_PTR(gBattlescriptCurrInstr + 3);

	if (animId == B_ANIM_STATS_CHANGE)
		TryContraryChangeStatAnim(gActiveBattler, argumentPtr);

	if (gHitMarker & HITMARKER_NO_ANIMATIONS && IsFancyTransformationAnimId(animId))
		animId = B_ANIM_TRANSFORM; //Play simplified animation because player wants faster battles

	if (animId == B_ANIM_STATS_CHANGE
	&&  gActiveBattler == gNewBS->skipBankStatAnim) //The Pokemon has no sprite on the screen, so don't play the stat anim
	{
		gNewBS->skipBankStatAnim = 0xFF;
		gBattlescriptCurrInstr += 7;
	}
	else if (ShouldAnimBeDoneEvenWithBattleAnimsOff(animId))
	{
		EmitBattleAnimation(0, animId, *argumentPtr);
		MarkBufferBankForExecution(gActiveBattler);
		gBattlescriptCurrInstr += 7;
	}
	else if (animId == B_ANIM_TRANSFORM_MOVE)
	{
		EmitMoveAnimation(0, MOVE_TRANSFORM, 0, 1, 1, 0xFF, &gDisableStructs[gActiveBattler], 0);
		MarkBufferBankForExecution(gActiveBattler);
		gBattlescriptCurrInstr += 7;
	}
	else if (gHitMarker & HITMARKER_NO_ANIMATIONS)
	{
		#ifdef FLAG_FAST_BATTLE_MESSAGES
		if (FlagGet(FLAG_FAST_BATTLE_MESSAGES))
			gBattlescriptCurrInstr += 7;
		else
		#endif
		{
			BattleScriptPush(gBattlescriptCurrInstr + 7);
			gBattlescriptCurrInstr = BattleScript_Pausex20;
		}
	}
	else if (gNewBS->tempIgnoreAnimations)
	{
		gBattlescriptCurrInstr += 7;
		gNewBS->tempIgnoreAnimations = FALSE;
	}
	else if (animId == B_ANIM_RAIN_CONTINUES
		  || animId == B_ANIM_SUN_CONTINUES
		  || animId == B_ANIM_SANDSTORM_CONTINUES
		  || animId == B_ANIM_HAIL_CONTINUES
		  || animId == B_ANIM_STRONG_WINDS_CONTINUE
		  || animId == B_ANIM_FOG_CONTINUES
		  || animId == B_ANIM_RAID_BATTLE_STORM)
	{
		EmitBattleAnimation(0, animId, *argumentPtr);
		MarkBufferBankForExecution(gActiveBattler);
		gBattlescriptCurrInstr += 7;
	}
	else if (gStatuses3[gActiveBattler] & STATUS3_SEMI_INVULNERABLE)
	{
		gBattlescriptCurrInstr += 7;
	}
	else
	{
		EmitBattleAnimation(0, animId, *argumentPtr);
		MarkBufferBankForExecution(gActiveBattler);
		gBattlescriptCurrInstr += 7;
	}
}

void atk46_playanimation2(void) // animation Id is stored in the first pointer
{
	u16* argumentPtr;
	u8 animId;

	gActiveBattler = GetBankForBattleScript(gBattlescriptCurrInstr[1]);
	animId = *((const u8*) T2_READ_PTR(gBattlescriptCurrInstr + 2));
	argumentPtr = T2_READ_PTR(gBattlescriptCurrInstr + 6);

	if (animId == B_ANIM_STATS_CHANGE)
		TryContraryChangeStatAnim(gActiveBattler, argumentPtr);

	if (gHitMarker & HITMARKER_NO_ANIMATIONS && IsFancyTransformationAnimId(animId))
		animId = B_ANIM_TRANSFORM; //Play simplified animation because player wants faster battles

	if (ShouldAnimBeDoneEvenWithBattleAnimsOff(animId))
	{
		EmitBattleAnimation(0, animId, *argumentPtr);
		MarkBufferBankForExecution(gActiveBattler);
		gBattlescriptCurrInstr += 10;
	}
	else if (animId == B_ANIM_TRANSFORM_MOVE)
	{
		EmitMoveAnimation(0, MOVE_TRANSFORM, 0, 1, 1, 0xFF, &gDisableStructs[gActiveBattler], 0);
		MarkBufferBankForExecution(gActiveBattler);
		gBattlescriptCurrInstr += 10;
	}
	else if (gHitMarker & HITMARKER_NO_ANIMATIONS
	|| gNewBS->tempIgnoreAnimations)
	{
		gNewBS->tempIgnoreAnimations = FALSE;
		gBattlescriptCurrInstr += 10;
	}
	else if (animId == B_ANIM_RAIN_CONTINUES
	      || animId == B_ANIM_SUN_CONTINUES
	      || animId == B_ANIM_SANDSTORM_CONTINUES
	      || animId == B_ANIM_HAIL_CONTINUES
	      || animId == B_ANIM_STRONG_WINDS_CONTINUE
	      || animId == B_ANIM_FOG_CONTINUES)
	{
		EmitBattleAnimation(0, animId, *argumentPtr);
		MarkBufferBankForExecution(gActiveBattler);
		gBattlescriptCurrInstr += 10;
	}
	else if (gStatuses3[gActiveBattler] & STATUS3_SEMI_INVULNERABLE)
	{
		gBattlescriptCurrInstr += 10;
	}
	else
	{
		EmitBattleAnimation(0, animId, *argumentPtr);
		MarkBufferBankForExecution(gActiveBattler);
		gBattlescriptCurrInstr += 10;
	}
}

void atk47_setgraphicalstatchangevalues(void)
{
	u8 value = 0;
	switch (GET_STAT_BUFF_VALUE_WITH_SIGN(gBattleScripting.statChanger))
	{
		case SET_STAT_BUFF_VALUE(1): // +1
			value = STAT_ANIM_PLUS1;
			break;
		case SET_STAT_BUFF_VALUE(2): // +2
			value = STAT_ANIM_PLUS2;
			break;
		case SET_STAT_BUFF_VALUE(3): // +3
			value = STAT_ANIM_PLUS2;
			break;
		case SET_STAT_BUFF_VALUE(4): // +4
			value = STAT_ANIM_PLUS2;
			break;
		case SET_STAT_BUFF_VALUE(5): // +5
			value = STAT_ANIM_PLUS2;
			break;
		case SET_STAT_BUFF_VALUE(6): // +6
			value = STAT_ANIM_PLUS2;
			break;
		case SET_STAT_BUFF_VALUE(1) | STAT_BUFF_NEGATIVE: // -1
			value = STAT_ANIM_MINUS1;
			break;
		case SET_STAT_BUFF_VALUE(2) | STAT_BUFF_NEGATIVE: // -2
			value = STAT_ANIM_MINUS2;
			break;
		case SET_STAT_BUFF_VALUE(3) | STAT_BUFF_NEGATIVE: // -3
			value = STAT_ANIM_MINUS2;
			break;
		case SET_STAT_BUFF_VALUE(4) | STAT_BUFF_NEGATIVE: // -1
			value = STAT_ANIM_MINUS2;
			break;
		case SET_STAT_BUFF_VALUE(5) | STAT_BUFF_NEGATIVE: // -2
			value = STAT_ANIM_MINUS2;
			break;
		case SET_STAT_BUFF_VALUE(6) | STAT_BUFF_NEGATIVE: // -3
			value = STAT_ANIM_MINUS2;
			break;
	}

	gBattleScripting.animArg1 = GET_STAT_BUFF_ID(gBattleScripting.statChanger) + value - 1;
	gBattleScripting.animArg2 = 0;
	gBattlescriptCurrInstr++;
}

void atk5C_hitanimation(void)
{
	gActiveBattler = GetBankForBattleScript(gBattlescriptCurrInstr[1]);

	if (!IsDoubleSpreadMove())
	{
		if (gMoveResultFlags & MOVE_RESULT_NO_EFFECT)
		{
		}
		else if (!(gHitMarker & HITMARKER_IGNORE_SUBSTITUTE || gNewBS->bypassSubstitute) || !IS_BEHIND_SUBSTITUTE(gActiveBattler) || gDisableStructs[gActiveBattler].substituteHP == 0)
		{
			//Do the hit animation on the actual Pokemon sprite, not the Substitute
			EmitHitAnimation(0);
			MarkBufferBankForExecution(gActiveBattler);
		}
	}
	else if (!gNewBS->doneDoublesSpreadHit) //Spread move in doubles
	{
		for (u32 i = 0; i < gBattlersCount; ++i)
		{
			if (!(gNewBS->ResultFlags[i] & MOVE_RESULT_NO_EFFECT) && !gNewBS->noResultString[i])
			{
				gActiveBattler = i;

				if (!(gHitMarker & HITMARKER_IGNORE_SUBSTITUTE || gNewBS->bypassSubstitute) || !IS_BEHIND_SUBSTITUTE(gActiveBattler) || gDisableStructs[gActiveBattler].substituteHP == 0)
				{
					EmitHitAnimation(0);
					MarkBufferBankForExecution(gActiveBattler);
				}
			}
		}
	}

	gBattlescriptCurrInstr += 2;
}

void ResetDoublesSpreadMoveCalcs(void)
{
	gNewBS->doneDoublesSpreadHit = FALSE;
	gNewBS->calculatedSpreadMoveData = FALSE;
	gNewBS->calculatedSpreadMoveAccuracy = FALSE;
}

static void UpdateMoveStartValuesForCalledMove(void)
{
	gBattleStruct->atkCancellerTracker = CANCELLER_GRAVITY_2;
	gBattleStruct->dynamicMoveType = GetMoveTypeSpecial(gBankAttacker, gCurrentMove);
	ResetDoublesSpreadMoveCalcs();
	gHitMarker &= ~(HITMARKER_ATTACKSTRING_PRINTED);

	u8 moveTarget = GetBaseMoveTarget(gCurrentMove, gBankAttacker);
	if (moveTarget & MOVE_TARGET_USER)
		gBankTarget = gBankAttacker;
	else if (IS_DOUBLE_BATTLE && moveTarget & MOVE_TARGET_ALL)
		DetermineFirstMultiTarget();
}

static void TryUpdateCalledMoveWithZMove(void)
{
	if (gNewBS->zMoveData.active && SPLIT(gCurrentMove) != SPLIT_STATUS)
	{
		gNewBS->ai.zMoveHelper = gCurrentMove;
		gCurrentMove = GetTypeBasedZMove(gCurrentMove, gBankAttacker);
	}
}

void atk63_jumptocalledmove(void)
{
	if (gBattlescriptCurrInstr[1])
		gCurrentMove = gCalledMove;
	else
		gChosenMove = gCurrentMove = gCalledMove;

	TryUpdateCalledMoveWithZMove();
	if (gBattlescriptCurrInstr[1] != 0xFF)
		UpdateMoveStartValuesForCalledMove();

	gHitMarker &= ~(HITMARKER_ATTACKSTRING_PRINTED);

	gBattlescriptCurrInstr = gBattleScriptsForMoveEffects[gBattleMoves[gCurrentMove].effect];
}

void atk64_statusanimation(void) {
	if (gBattleExecBuffer) return;

	gActiveBattler = GetBankForBattleScript(gBattlescriptCurrInstr[1]);

	if (!(gStatuses3[gActiveBattler] & STATUS3_SEMI_INVULNERABLE) && gDisableStructs[gActiveBattler].substituteHP == 0 && !(gHitMarker & HITMARKER_NO_ANIMATIONS)) {
			EmitStatusAnimation(0, 0, gBattleMons[gActiveBattler].status1);
			MarkBufferBankForExecution(gActiveBattler);
	}
	gBattlescriptCurrInstr += 2;
}

void atk66_chosenstatusanimation(void) {
	if (gBattleExecBuffer) return;

	u32 status;
	gActiveBattler = GetBankForBattleScript(gBattlescriptCurrInstr[1]);
	status = T1_READ_32(gBattlescriptCurrInstr + 3);

	if (!(gStatuses3[gActiveBattler] & STATUS3_SEMI_INVULNERABLE) && gDisableStructs[gActiveBattler].substituteHP == 0 && !(gHitMarker & HITMARKER_NO_ANIMATIONS)) {
			EmitStatusAnimation(0, gBattlescriptCurrInstr[2], status);
			MarkBufferBankForExecution(gActiveBattler);
	}
	gBattlescriptCurrInstr += 7;
}

void atk6A_removeitem(void)
{
	if (gBattleExecBuffer) return;

	u8 bank = gActiveBattler = GetBankForBattleScript(gBattlescriptCurrInstr[1]);
	u8 oldItemEffect = ITEM_EFFECT(bank);
	
	if (gNewBS->doingPluckItemEffect) //This item removal was triggered by using someone else's item
	{
		gNewBS->doingPluckItemEffect = FALSE;
		gBattlescriptCurrInstr += 2;
		return;
	}
	
	gLastUsedItem = gBattleMons[bank].item;

	if (gLastUsedItem != ITEM_NONE)
	{
		if (!gNewBS->IncinerateCounters[bank]) //Item can be restored
		{
			CONSUMED_ITEMS(bank) = gLastUsedItem;
			SAVED_CONSUMED_ITEMS(bank) = gLastUsedItem;
			AddBankToPickupStack(bank);
		}

		gBattleMons[bank].item = ITEM_NONE;
		HandleUnburdenBoost(bank);
	}

	gBattlescriptCurrInstr += 2;

	u8 partner = PARTNER(bank);
	u16 partnerItem = gBattleMons[partner].item;

	if (IS_DOUBLE_BATTLE
	&& gNewBS->NoSymbiosisByte == FALSE
	&& ABILITY(partner) == ABILITY_SYMBIOSIS
	&& (!gNewBS->IncinerateCounters[bank] || oldItemEffect == ITEM_EFFECT_AIR_BALLOON) //Air Balloons can't be restored by Recycle, but they trigger Symbiosis
	&& !(gNewBS->corrodedItems[SIDE(bank)] & gBitTable[gBattlerPartyIndexes[bank]])
	&& partnerItem != ITEM_NONE
	&& CanTransferItem(SPECIES(bank), partnerItem)
	&& CanTransferItem(SPECIES(partner), partnerItem))
	{
		gBattleMons[bank].item = partnerItem;
		gBattleMons[partner].item = 0;
		HandleUnburdenBoost(bank); //Remove the Unburden boost it may have gained

		gActiveBattler = bank;
		EmitSetMonData(0, REQUEST_HELDITEM_BATTLE, 0, 2, &gBattleMons[gActiveBattler].item);
		MarkBufferBankForExecution(gActiveBattler);

		gActiveBattler = partner;
		EmitSetMonData(0, REQUEST_HELDITEM_BATTLE, 0, 2, &gBattleMons[gActiveBattler].item);
		MarkBufferBankForExecution(gActiveBattler);

		gEffectBank = bank;
		gBattleScripting.bank = partner;

		gLastUsedItem = partnerItem;
		BattleScriptPushCursor();
		gBattlescriptCurrInstr = BattleScript_Symbiosis;
	}
	else
	{
		EmitSetMonData(0, REQUEST_HELDITEM_BATTLE, 0, 2, &gBattleMons[bank].item);
		MarkBufferBankForExecution(bank);
	}

	gNewBS->NoSymbiosisByte = FALSE;
	gNewBS->IncinerateCounters[bank] = 0;
	gNewBS->corrodedItems[SIDE(bank)] &= ~(gBitTable[gBattlerPartyIndexes[bank]]);
}

void atk6B_atknameinbuff1(void)
{
	PREPARE_MON_NICK_BUFFER(gBattleTextBuff1, gBankAttacker, GetIllusionPartyNumber(gBankAttacker))

	gBattlescriptCurrInstr++;
}

void atk70_recordlastability(void)
{
	gActiveBattler = GetBankForBattleScript(gBattlescriptCurrInstr[1]);
	RecordAbilityBattle(gActiveBattler, gLastUsedAbility);
	gBattlescriptCurrInstr += 2;
}

void atk77_setprotect(void)
{
	bool8 notLastTurn = TRUE;
	u16 rate = 0xFFFF;
	u16 divisor = 1;
	int i;

	switch (gLastResultingMoves[gBankAttacker]) {
		case MOVE_PROTECT:
		case MOVE_DETECT:
		case MOVE_ENDURE:
		case MOVE_KINGSSHIELD:
		case MOVE_SPIKYSHIELD:
		case MOVE_BANEFULBUNKER:
		case MOVE_CRAFTYSHIELD:
		case MOVE_MATBLOCK:
		case MOVE_QUICKGUARD:
		case MOVE_WIDEGUARD:
		case MOVE_OBSTRUCT:
		case MOVE_MAX_GUARD:
			break;
		default:
			gDisableStructs[gBankAttacker].protectUses = 0;
	}

	if (gCurrentTurnActionNumber == (gBattlersCount - 1))
		notLastTurn = FALSE;

	if (gCurrentMove == MOVE_WIDEGUARD || gCurrentMove == MOVE_QUICKGUARD || gCurrentMove == MOVE_CRAFTYSHIELD || gCurrentMove == MOVE_MATBLOCK)
		divisor = 1; //These moves increase the fail rate, but can't fail themselves
	else
	{
		for (i = 0; i < gDisableStructs[gBankAttacker].protectUses && i <= 6; ++i)
			divisor *= 3;
	}

	if (udivsi(rate, divisor) >= Random() && notLastTurn)
	{
		u8 atkSide = SIDE(gBankAttacker);
		switch (gCurrentMove) {
			case MOVE_KINGSSHIELD:
				gProtectStructs[gBankAttacker].KingsShield = 1;
				gBattleCommunication[MULTISTRING_CHOOSER] = 0;
				break;

			case MOVE_SPIKYSHIELD:
				gProtectStructs[gBankAttacker].SpikyShield = 1;
				gBattleCommunication[MULTISTRING_CHOOSER] = 0;
				break;

			case MOVE_BANEFULBUNKER:
				gProtectStructs[gBankAttacker].BanefulBunker = 1;
				gBattleCommunication[MULTISTRING_CHOOSER] = 0;
				break;

			case MOVE_OBSTRUCT:
				gProtectStructs[gBankAttacker].obstruct = 1;
				gBattleCommunication[MULTISTRING_CHOOSER] = 0;
				break;

			case MOVE_CRAFTYSHIELD:
				gSideStatuses[atkSide] |= SIDE_STATUS_CRAFTY_SHIELD;
				gBattleCommunication[MULTISTRING_CHOOSER] = 3;
				gBattleStringLoader = CraftyShieldSetString;
				break;

			case MOVE_MATBLOCK:
				if (!gDisableStructs[gBankAttacker].isFirstTurn)
					goto PROTECT_FAILED;

				gSideStatuses[atkSide] |= SIDE_STATUS_MAT_BLOCK;
				gBattleCommunication[MULTISTRING_CHOOSER] = 3;
				gBattleStringLoader = MatBlockSetString;
				break;

			case MOVE_QUICKGUARD:
				gSideStatuses[atkSide] |= SIDE_STATUS_QUICK_GUARD;
				gBattleCommunication[MULTISTRING_CHOOSER] = 3;
				gBattleStringLoader = QuickGuardSetString;
				break;

			case MOVE_WIDEGUARD:
				gSideStatuses[atkSide] |= SIDE_STATUS_WIDE_GUARD;
				gBattleCommunication[MULTISTRING_CHOOSER] = 3;
				gBattleStringLoader = WideGuardSetString;
				break;

			case MOVE_ENDURE:
				gProtectStructs[gBankAttacker].endured = 1;
				gBattleCommunication[MULTISTRING_CHOOSER] = 1;
				break;

			default:
				gProtectStructs[gBankAttacker].protected = 1;
				gBattleCommunication[MULTISTRING_CHOOSER] = 0;
		}

		++gDisableStructs[gBankAttacker].protectUses;
	}
	else
	{
	PROTECT_FAILED:
		gDisableStructs[gBankAttacker].protectUses = 0;
		gBattleCommunication[MULTISTRING_CHOOSER] = 2;
		gMoveResultFlags |= MOVE_RESULT_MISSED;
	}

	gBattlescriptCurrInstr++;
}

//The Damp check has been moved to attackcanceler
void atk78_faintifabilitynotdamp(void)
{
	if (gBattleExecBuffer) return;

	gActiveBattler = gBankAttacker;
	gBattleMoveDamage = gBattleMons[gActiveBattler].hp;
	EmitHealthBarUpdate(0, INSTANT_HP_BAR_DROP);
	MarkBufferBankForExecution(gActiveBattler);
	gBattlescriptCurrInstr++;
}

void atk7A_jumpifnexttargetvalid(void)
{
	u8* jump_loc = T1_READ_PTR(gBattlescriptCurrInstr + 1);

	if (IS_DOUBLE_BATTLE)
	{
		if (gBankAttacker == GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT)
		&& gBankTarget == GetBattlerAtPosition(B_POSITION_PLAYER_LEFT))
		{
			gBattlescriptCurrInstr += 5;
			return;
		}

		for (gBankTarget++; gBankTarget < gBattlersCount; gBankTarget++)
		{
			if (gBankTarget == gBankAttacker && !(GetBaseMoveTarget(gCurrentMove, gBankAttacker) & MOVE_TARGET_USER))
				continue;
			if (!(gAbsentBattlerFlags & gBitTable[gBankTarget]))
				break;
		}

		if (gBankTarget >= gBattlersCount)
		{
			if (gBankAttacker == GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT)
			&&  !(gAbsentBattlerFlags & gBitTable[GetBattlerAtPosition(B_POSITION_PLAYER_LEFT)]))
			{
				gBankTarget = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
				gBattlescriptCurrInstr = jump_loc;
			}
			else
				gBattlescriptCurrInstr += 5;
		}
		else
			gBattlescriptCurrInstr = jump_loc;
	}
	else
		gBattlescriptCurrInstr += 5;
}

void atk7B_tryhealhalfhealth(void)
{
	const u8* failPtr = T1_READ_PTR(gBattlescriptCurrInstr + 1);

	if (gBattlescriptCurrInstr[5] == BS_GET_ATTACKER)
		gBankTarget = gBankAttacker;

	if (BATTLER_MAX_HP(gBankTarget))
		gBattlescriptCurrInstr = failPtr;
	else
	{
		if (gCurrentMove == MOVE_LIFEDEW || gCurrentMove == MOVE_JUNGLEHEALING)
			gBattleMoveDamage = MathMax(1, GetBaseMaxHP(gBankTarget) / 4); //25 %
		else
			gBattleMoveDamage = MathMax(1, GetBaseMaxHP(gBankTarget) / 2); //50 %

		gBattleMoveDamage *= -1;
		gBattlescriptCurrInstr += 6;
	}
}

void atk7C_trymirrormove(void)
{
	s32 validMovesCount;
	s32 i;
	u16 move;
	u16 movesArray[4] = {0};

	for (validMovesCount = 0, i = 0; i < gBattlersCount; i++)
	{
		if (i != gBankAttacker)
		{
			move = gBattleStruct->lastTakenMoveFrom[gBankAttacker][i];

			if (move != 0 && move != 0xFFFF)
				movesArray[validMovesCount++] = move;
		}
	}

	move = gBattleStruct->lastTakenMove[gBankAttacker];

	if (move != 0 && move != 0xFFFF)
	{
		gCurrentMove = move;
		gBankTarget = GetMoveTarget(gCurrentMove, 0);
		TryUpdateCalledMoveWithZMove();
		UpdateMoveStartValuesForCalledMove();
		gBattlescriptCurrInstr = gBattleScriptsForMoveEffects[gBattleMoves[gCurrentMove].effect];
	}
	else if (validMovesCount)
	{
		i = umodsi(Random(), validMovesCount);
		gCurrentMove = movesArray[i];
		gBankTarget = GetMoveTarget(gCurrentMove, 0);
		TryUpdateCalledMoveWithZMove();
		UpdateMoveStartValuesForCalledMove();
		gBattlescriptCurrInstr = gBattleScriptsForMoveEffects[gBattleMoves[gCurrentMove].effect];
	}
	else
	{
		gSpecialStatuses[gBankAttacker].ppNotAffectedByPressure = 1;
		gBattlescriptCurrInstr++;
	}
}

bool8 SetRainyWeather(void)
{
	if (gBattleWeather & (WEATHER_RAIN_ANY | WEATHER_PRIMAL_ANY | WEATHER_CIRCUS))
	{
		gBattleCommunication[MULTISTRING_CHOOSER] = 2;
		return FALSE;
	}

	gBattleWeather = WEATHER_RAIN_TEMPORARY;
	gBattleCommunication[MULTISTRING_CHOOSER] = 0;

	if (ITEM_EFFECT(gBankAttacker) == ITEM_EFFECT_DAMP_ROCK)
		gWishFutureKnock.weatherDuration = 8;
	else
		gWishFutureKnock.weatherDuration = 5;

	gBattleScripting.animArg1 = B_ANIM_RAIN_CONTINUES;
	return TRUE;
}

void atk7D_setrain(void)
{
	u8 bank;

	if (!SetRainyWeather())
	{
		gMoveResultFlags |= MOVE_RESULT_FAILED;
		gBattlescriptCurrInstr++;
	}
	else if ((bank = BankOnFieldHasEvaporate()))
	{
		//Undo weather
		gBattleWeather = 0;
		gWishFutureKnock.weatherDuration = 0;
		gMoveResultFlags |= MOVE_RESULT_FAILED;
		gBattleScripting.bank = bank - 1;
		gBattlescriptCurrInstr = BattleScript_RainDanceBlockedByEvaporate;
	}
	else
		gBattlescriptCurrInstr++;
}

void atk7E_setreflect(void) {
	if (gSideStatuses[SIDE(gBankAttacker)] & SIDE_STATUS_REFLECT) {
		gMoveResultFlags |= MOVE_RESULT_MISSED;
		gBattleCommunication[MULTISTRING_CHOOSER] = 0;
	}
	else {
		gSideStatuses[SIDE(gBankAttacker)] |= SIDE_STATUS_REFLECT;

		if (ITEM_EFFECT(gBankAttacker) == ITEM_EFFECT_LIGHT_CLAY)
			gSideTimers[SIDE(gBankAttacker)].reflectTimer = 8;
		else
			gSideTimers[SIDE(gBankAttacker)].reflectTimer = 5;

		PREPARE_STAT_BUFFER(gBattleTextBuff1, STAT_DEF);
		gBattleStringLoader = gText_ScreenRaisedStat;
		gBattleCommunication[MULTISTRING_CHOOSER] = 1;
	}
	gBattlescriptCurrInstr++;
}

const u16 gReflectLightScreenStringIds[] =
{
	STRINGID_BUTITFAILED, 0x184
};

void atk7F_setseeded(void) {
	if ((gMoveResultFlags & MOVE_RESULT_NO_EFFECT) || (gStatuses3[gBankTarget] & STATUS3_LEECHSEED)) {
		gMoveResultFlags |= MOVE_RESULT_MISSED;
		gBattleCommunication[MULTISTRING_CHOOSER] = 1;
	}

	else if (IsOfType(gBankTarget, TYPE_GRASS)) {
		gMoveResultFlags |= MOVE_RESULT_MISSED;
		gBattleCommunication[MULTISTRING_CHOOSER] = 2;
	}

	else {
		gStatuses3[gBankTarget] |= gBankAttacker;
		gStatuses3[gBankTarget] |= STATUS3_LEECHSEED;
		gBattleCommunication[MULTISTRING_CHOOSER] = 0;
	}

	gBattlescriptCurrInstr++;
}

void atk81_trysetrest(void)
{
	bool8 fail = FALSE;
	gActiveBattler = gBankTarget = gBankAttacker;
	gBattleMoveDamage = gBattleMons[gBankTarget].maxHP * (-1);

	if (gTerrainType == ELECTRIC_TERRAIN && IsAffectedByElectricTerrain(gActiveBattler))
	{
		gBattlescriptCurrInstr = BattleScript_ButItFailed;
		fail = TRUE;
	}
	else if (gTerrainType == MISTY_TERRAIN && CheckGrounding(gActiveBattler))
	{
		gBattlescriptCurrInstr = BattleScript_ButItFailed;
		fail = TRUE;
	}
	else if (IsOfType(gActiveBattler, TYPE_GRASS) && ABILITY(gActiveBattler) == ABILITY_FLOWERVEIL)
	{
		gBattleScripting.bank = gActiveBattler;
		gBattlescriptCurrInstr = BattleScript_TeamProtectedByFlowerVeil;
		fail = TRUE;
	}
	else if (IsOfType(gActiveBattler, TYPE_GRASS) && IS_DOUBLE_BATTLE && ABILITY(PARTNER(gActiveBattler)) == ABILITY_FLOWERVEIL)
	{
		gBattleScripting.bank = PARTNER(gActiveBattler);
		gBattlescriptCurrInstr = BattleScript_TeamProtectedByFlowerVeil;
		fail = TRUE;
	}
	else if (ABILITY(gActiveBattler) == ABILITY_SWEETVEIL)
	{
		gBattleScripting.bank = gActiveBattler;
		gBattlescriptCurrInstr = BattleScript_TeamProtectedBySweetVeil;
		fail = TRUE;
	}
	else if (IS_DOUBLE_BATTLE && ABILITY(PARTNER(gActiveBattler)) == ABILITY_SWEETVEIL)
	{
		gBattleScripting.bank = PARTNER(gActiveBattler);
		gBattlescriptCurrInstr = BattleScript_TeamProtectedBySweetVeil;
		fail = TRUE;
	}
	else if (IsUproarBeingMade())
	{
		gBattleScripting.bank = gActiveBattler;
		gBattleCommunication[MULTISTRING_CHOOSER] = 0;
		gBattlescriptCurrInstr = BattleScript_RestCantSleep;
		fail = TRUE;
	}
	else if (BATTLER_MAX_HP(gActiveBattler))
	{
		gBattleScripting.bank = gActiveBattler;
		gBattlescriptCurrInstr = BattleScript_AlreadyAtFullHp;
		fail = TRUE;
	}

	if (!fail)
	{
		switch (ABILITY(gActiveBattler)) {
			case ABILITY_INSOMNIA:
			#ifdef ABILITY_VITALSPIRIT
			case ABILITY_VITALSPIRIT:
			#endif
				gBattlescriptCurrInstr = BattleScript_TargetStayedAwakeUsingAbility;
				fail = TRUE;
				break;
			case ABILITY_LEAFGUARD:
				if (WEATHER_HAS_EFFECT && gBattleWeather & WEATHER_SUN_ANY)
				{
					gBattlescriptCurrInstr = BattleScript_ProtectedByAbility;
					fail = TRUE;
				}
				break;
			case ABILITY_COMATOSE:
				gBattlescriptCurrInstr = BattleScript_ButItFailed;
				fail = TRUE;
				break;
			#ifdef SPECIES_MINIOR_SHIELD
			case ABILITY_SHIELDSDOWN:
				if (GetBankPartyData(gBankAttacker)->species == SPECIES_MINIOR_SHIELD)
				{
					gBattlescriptCurrInstr = BattleScript_ButItFailed;
					fail = TRUE;
				}
				break;
			#endif
		}
	}

	if (!fail)
	{
		if (gBattleMons[gActiveBattler].status1 & ((u8)(~STATUS1_SLEEP)))
			gBattleCommunication[MULTISTRING_CHOOSER] = 1;
		else
			gBattleCommunication[MULTISTRING_CHOOSER] = 0;

		gBattleMons[gBankTarget].status1 = 3;
		EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[gActiveBattler].status1);
		MarkBufferBankForExecution(gActiveBattler);
		gBattlescriptCurrInstr += 5;
	}
}

void atk82_jumpifnotfirstturn(void)
{
	u8* failJump = T1_READ_PTR(gBattlescriptCurrInstr + 1);

	if (gDisableStructs[gBankAttacker].isFirstTurn
	&& !gNewBS->InstructInProgress)
		gBattlescriptCurrInstr += 5;
	else
		gBattlescriptCurrInstr = failJump;
}

void atk84_jumpifcantmakeasleep(void)
{
	u8 bankDef = gBankTarget;
	u8 defPartner = PARTNER(bankDef);
	u8 defAbility = ABILITY(bankDef);
	u8 defPartnerAbility = ABILITY(defPartner);

	u8* jump_loc = T1_READ_PTR(gBattlescriptCurrInstr + 1);

	if (UproarWakeUpCheck(bankDef)
	#ifdef SPECIES_MINIOR_SHIELD
	|| SPECIES(bankDef) == SPECIES_MINIOR_SHIELD
	#endif
	)
	{
		gBattlescriptCurrInstr = jump_loc;
	}
	else if (defAbility == ABILITY_INSOMNIA
	#ifdef ABILITY_VITALSPIRIT
	|| defAbility == ABILITY_VITALSPIRIT
	#endif
	|| defAbility == ABILITY_COMATOSE
	|| defAbility == ABILITY_SWEETVEIL
	|| (defAbility == ABILITY_LEAFGUARD && WEATHER_HAS_EFFECT && gBattleWeather & WEATHER_SUN_ANY)
	|| (defAbility == ABILITY_FLOWERVEIL && IsOfType(bankDef, TYPE_GRASS) && gCurrentMove != MOVE_REST))
	{
		gLastUsedAbility = defAbility;
		gBattleCommunication[MULTISTRING_CHOOSER] = 2;
		gBattlescriptCurrInstr = jump_loc;
		RecordAbilityBattle(bankDef, gLastUsedAbility);
	}
	else if (defPartnerAbility == ABILITY_SWEETVEIL
	|| (defPartnerAbility == ABILITY_FLOWERVEIL && IsOfType(bankDef, TYPE_GRASS) && gCurrentMove != MOVE_REST))
	{
		gLastUsedAbility = defPartnerAbility;
		gBattlescriptCurrInstr = jump_loc;
		RecordAbilityBattle(defPartner, gLastUsedAbility);
	}
	else if (gTerrainType == ELECTRIC_TERRAIN && IsAffectedByElectricTerrain(bankDef))
	{
		gBattlescriptCurrInstr = jump_loc;
	}
	else if (gTerrainType == MISTY_TERRAIN && CheckGrounding(bankDef))
	{
		gBattlescriptCurrInstr = jump_loc;
	}
	else
	{
		gBattlescriptCurrInstr += 5;
	}
}

//This function used to calculate damage, but now all it does is check if Spit Up can be used
void atk86_stockpiletobasedamage(void) {
	u8* jump_loc = T1_READ_PTR(gBattlescriptCurrInstr + 1);

	if (gDisableStructs[gBankAttacker].stockpileCounter == 0)
		gBattlescriptCurrInstr = jump_loc;
	else
	{
		gBattleScripting.animTurn = gDisableStructs[gBankAttacker].stockpileCounter;
		gBattlescriptCurrInstr += 5;
	}
}

void atk87_stockpiletohpheal(void)
{
	u8* jumpPtr = T1_READ_PTR(gBattlescriptCurrInstr + 1);

	if (gDisableStructs[gBankAttacker].stockpileCounter == 0)
	{
		gBattlescriptCurrInstr = jumpPtr;
		gBattleCommunication[MULTISTRING_CHOOSER] = 0;
	}
	else if (BATTLER_MAX_HP(gBankAttacker))
	{
		gDisableStructs[gBankAttacker].stockpileCounter = 0;
		gBattlescriptCurrInstr = jumpPtr;
		gBankTarget = gBankAttacker;
		gBattleCommunication[MULTISTRING_CHOOSER] = 1;
	}
	else
	{
		gBattleMoveDamage = MathMax(1, udivsi(GetBaseMaxHP(gBankAttacker), (1 << (3 - gDisableStructs[gBankAttacker].stockpileCounter))));
		gBattleMoveDamage *= -1;

		gBattleScripting.animTurn = gDisableStructs[gBankAttacker].stockpileCounter;
		gBattlescriptCurrInstr += 5;
		gBankTarget = gBankAttacker;
	}
}

void atk88_negativedamage(void) {
	if (gCurrentMove == MOVE_OBLIVIONWING || MOVE_DRAININGKISS)
		gBattleMoveDamage = (75 * gHpDealt) / 100;
	else
		gBattleMoveDamage = (gHpDealt / 2);

	if (ITEM_EFFECT(gBankAttacker) == ITEM_EFFECT_BIG_ROOT)
		gBattleMoveDamage = (13 * gBattleMoveDamage) / 10;

	gBattleMoveDamage *= -1;

	if (gBattleMoveDamage == 0)
		gBattleMoveDamage = -1;

	gBattlescriptCurrInstr++;
}

void atk8A_normalisebuffs(void) //Haze
{
	int i, j;

	for (i = 0; i < gBattlersCount; ++i)
	{
		for (j = 0; j < BATTLE_STATS_NO-1; ++j)
			gBattleMons[i].statStages[j] = 6;
	}

	++gBattlescriptCurrInstr;
}

void atk8D_setmultihitcounter(void) {
	if (gBattlescriptCurrInstr[1])
		gMultiHitCounter = gBattlescriptCurrInstr[1];

	if (ABILITY(gBankAttacker) == ABILITY_SKILLLINK)
		gMultiHitCounter = 5;

	#ifdef SPECIES_ASHGRENINJA
	else if (ABILITY(gBankAttacker) == ABILITY_BATTLEBOND
	&& gCurrentMove == MOVE_WATERSHURIKEN
	&& gBattleMons[gBankAttacker].species == SPECIES_ASHGRENINJA)
	{
		gMultiHitCounter = 3;
	}
	else
	#endif
	{
		gMultiHitCounter = Random() & 3;
		if (gMultiHitCounter > 1)
			gMultiHitCounter = (Random() & 3) + 2;
		else
			gMultiHitCounter += 2;
	}

	gBattlescriptCurrInstr += 2;
}

void atk90_tryconversiontypechange(void)
{
	u8* failScript = T1_READ_PTR(gBattlescriptCurrInstr + 1);

	if (gCurrentMove != MOVE_REFLECTTYPE) //Conversion
	{
		u8 moveType = gBattleMoves[gBattleMons[gBankAttacker].moves[0]].type;

		if (IS_BLANK_TYPE(moveType))
		{
			gBattlescriptCurrInstr = failScript;
		}
		else if(
			moveType == gBattleMons[gBankAttacker].type1
		||  moveType == gBattleMons[gBankAttacker].type2
		||  moveType == gBattleMons[gBankAttacker].type3)
		{
			gBattlescriptCurrInstr = failScript;
		}
		else
		{
			gBattleMons[gBankAttacker].type1 = moveType;
			gBattleMons[gBankAttacker].type2 = moveType;
			gBattleMons[gBankAttacker].type3 = TYPE_BLANK;

			PREPARE_TYPE_BUFFER(gBattleTextBuff1, moveType);

			gBattlescriptCurrInstr += 5;
		}
	}
	else //Reflect Type
	{
		u8 defType1 = gBattleMons[gBankTarget].type1;
		u8 defType2 = gBattleMons[gBankTarget].type2;
		u8 defType3 = gBattleMons[gBankTarget].type3;

		//If target has no type
		if((IS_BLANK_TYPE(defType1))
		&& (IS_BLANK_TYPE(defType2))
		&& (IS_BLANK_TYPE(defType3)))
		{
			gBattlescriptCurrInstr = failScript;
		}
		else
		{
			//If target has no main types, but has a third type
			if ((IS_BLANK_TYPE(defType1))
			&&  (IS_BLANK_TYPE(defType2))
			&& !(IS_BLANK_TYPE(defType3)))
			{
				defType1 = TYPE_NORMAL;
				defType2 = TYPE_NORMAL;
			}
			else //Target Has Main Type
			{
				if (IS_BLANK_TYPE(defType1))
					defType1 = defType2;
				else if (IS_BLANK_TYPE(defType2))
					defType2 = defType1;

				if (IS_BLANK_TYPE(defType3)) //Just in case it has a burned out Fire type
					defType3 = TYPE_BLANK;
			}

			gBattleMons[gBankAttacker].type1 = defType1;
			gBattleMons[gBankAttacker].type2 = defType2;
			gBattleMons[gBankAttacker].type3 = defType3;

			gBattlescriptCurrInstr += 5;
		}
	}
}

void atk91_givepaydaymoney(void)
{
	int i;
	u32 money = 0;

	if (!(gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_FRONTIER | BATTLE_TYPE_TRAINER_TOWER)))
	{
		for (i = 0; i < PARTY_SIZE; ++i)
			money += (gPlayerParty[i].level * 5) * gNewBS->PayDayByPartyIndices[i];
		money *= gBattleStruct->moneyMultiplier;
		money += gNewBS->maxGoldrushMoney * gBattleStruct->moneyMultiplier;
		#ifdef PAYDAY_MONEY_CAP
		money = MathMin(money, 99999); //Pay Day and Gold Rush cap at $99999
		#endif

		if (money > 0)
		{
			AddMoney(&gSaveBlock1->money, money);
			PREPARE_WORD_NUMBER_BUFFER(gBattleTextBuff1, 10, money);
			BattleScriptPush(gBattlescriptCurrInstr + 1);
			gBattlescriptCurrInstr = BattleScript_PrintPayDayMoneyString;
			return;
		}
	}
	gBattlescriptCurrInstr++;
}

void atk92_setlightscreen(void)
{
	if (gSideStatuses[SIDE(gBankAttacker)] & SIDE_STATUS_LIGHTSCREEN)
	{
		gMoveResultFlags |= MOVE_RESULT_MISSED;
		gBattleCommunication[MULTISTRING_CHOOSER] = 0;
	}
	else
	{
		gSideStatuses[SIDE(gBankAttacker)] |= SIDE_STATUS_LIGHTSCREEN;

		if (ITEM_EFFECT(gBankAttacker) == ITEM_EFFECT_LIGHT_CLAY)
			gSideTimers[SIDE(gBankAttacker)].lightscreenTimer = 8;
		else
			gSideTimers[SIDE(gBankAttacker)].lightscreenTimer = 5;

		PREPARE_STAT_BUFFER(gBattleTextBuff1, STAT_SPDEF);
		gBattleStringLoader = gText_ScreenRaisedStat;
		gBattleCommunication[MULTISTRING_CHOOSER] = 1;
	}
	gBattlescriptCurrInstr++;
}

void atk93_tryKO(void)
{
	u8 bankAtk = gBankAttacker;
	u8 bankDef = gBankTarget;

	u8 atkAbility = ABILITY(bankAtk);
	u8 defAbility = ABILITY(bankDef);
	u8 defEffect = ITEM_EFFECT(bankDef);
	u8 defQuality = ITEM_QUALITY(bankDef);
	u8 mystery = ItemId_GetMystery2(ITEM(bankDef));

	gStringBank = bankDef;

	if (defAbility == ABILITY_STURDY)
	{
		gMoveResultFlags |= MOVE_RESULT_MISSED;
		gBattleScripting.bank = gBankTarget;
		gBattlescriptCurrInstr = BattleScript_SturdyPreventsOHKO;
	}
	else
	{
		u16 chance = FALSE;
		u8 baseAcc;
		#ifdef NO_SHEER_COLD_NERF
			baseAcc = gBattleMoves[gCurrentMove].accuracy;
		#else
			if (gCurrentMove == MOVE_SHEERCOLD && !IsOfType(bankAtk, TYPE_ICE))
				baseAcc = 20;
			else
				baseAcc = gBattleMoves[gCurrentMove].accuracy;
		#endif

		if (gBattleMons[bankAtk].level >= gBattleMons[bankDef].level)
		{
			if (HasRaidShields(gBankTarget)) //Shields can be used outside of Raid Battles now
			{
				chance = TRUE; //Always breaks 2 raid shields
			}
			else if (IsRaidBattle() && gBankTarget == BANK_RAID_BOSS)
			{
				chance = FALSE; //Never works on regular raid mon
			}
			else if (IsDynamaxed(gBankTarget))
			{
				chance = FALSE; //Doesn't work on Dynamaxed foe
			}
			#ifndef NO_SHEER_COLD_NERF
			else if (gCurrentMove == MOVE_SHEERCOLD && IsOfType(bankDef, TYPE_ICE))
			{
				chance = FALSE;
			}
			#endif
			else if (((gStatuses3[bankDef] & STATUS3_ALWAYS_HITS && gDisableStructs[bankDef].bankWithSureHit == bankAtk)
			|| atkAbility == ABILITY_NOGUARD || defAbility == ABILITY_NOGUARD))
			{
				chance = TRUE;
			}
			else
			{
				chance = baseAcc + (gBattleMons[bankAtk].level - gBattleMons[bankDef].level);
				if (umodsi(Random(), 100) + 1 < chance)
					chance = TRUE;
				else
					chance = FALSE;
			}
		}
		else
			chance = FALSE;

		if (chance)
		{
			if (HasRaidShields(gBankTarget))
			{
				//Just break shields
			}
			else if (defEffect == ITEM_EFFECT_FOCUS_BAND && !mystery && umodsi(Random(), 100) < defQuality)
			{
				RecordItemEffectBattle(bankDef, defEffect);
				gSpecialStatuses[bankDef].focusBanded = 1;
			}
			//Focus Sash
			else if (IsBankHoldingFocusSash(bankDef) && BATTLER_MAX_HP(bankDef))
			{
				RecordItemEffectBattle(bankDef, defEffect);
				gSpecialStatuses[bankDef].focusBanded = 1;
				gNewBS->EnduranceHelper[gBankTarget] = ENDURE_FOCUS_SASH;
			}

			if (HasRaidShields(gBankTarget))
			{
				//Just break shields
			}
			else if (gProtectStructs[bankDef].endured)
			{
				gBattleMoveDamage = gBattleMons[bankDef].hp - 1;
				gMoveResultFlags |= MOVE_RESULT_FOE_ENDURED;
			}
			else if (gSpecialStatuses[bankDef].focusBanded)
			{
				gBattleMoveDamage = gBattleMons[bankDef].hp - 1;
				gMoveResultFlags |= MOVE_RESULT_FOE_HUNG_ON;
				gLastUsedItem = gBattleMons[bankDef].item;
			}
			else
			{
				gBattleMoveDamage = gBattleMons[bankDef].hp;
				gMoveResultFlags |= MOVE_RESULT_ONE_HIT_KO;
			}

			gBattlescriptCurrInstr += 5;
		}
		else
		{
			gMoveResultFlags |= MOVE_RESULT_MISSED;
			#ifdef NO_SHEER_COLD_NERF
				if (gBattleMons[bankAtk].level >= gBattleMons[bankDef].level && !IsDynamaxed(bankDef))
					gBattleCommunication[MULTISTRING_CHOOSER] = 0;
				else
					gBattleCommunication[MULTISTRING_CHOOSER] = 1;
			#else
				if (gBattleMons[bankAtk].level < gBattleMons[bankDef].level
				|| IsDynamaxed(bankDef)
				|| (gCurrentMove == MOVE_SHEERCOLD && IsOfType(bankDef, TYPE_ICE)))
					gBattleCommunication[MULTISTRING_CHOOSER] = 1;
				else
					gBattleCommunication[MULTISTRING_CHOOSER] = 0;
			#endif

			gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
		}
	}
}

void atk94_damagetohalftargethp(void) { //Super Fang
	switch (gCurrentMove) {
		case MOVE_GUARDIAN_OF_ALOLA:
			gBattleMoveDamage = (GetBaseCurrentHP(gBankTarget) * 75) / 100;
			if (ProtectsAgainstZMoves(gCurrentMove, gBankAttacker, gBankTarget))
				gBattleMoveDamage = (gBattleMoveDamage * 25) / 100;
			break;

		default:
			gBattleMoveDamage = GetBaseCurrentHP(gBankTarget) / 2;
	}

	if (gBattleMoveDamage == 0)
		gBattleMoveDamage = 1;
	gBattlescriptCurrInstr++;
}

bool8 SetSandstormWeather(void)
{
	if (gBattleWeather & (WEATHER_SANDSTORM_ANY | WEATHER_PRIMAL_ANY | WEATHER_CIRCUS))
	{
		gBattleCommunication[MULTISTRING_CHOOSER] = 2;
		return FALSE;
	}

	gBattleWeather = WEATHER_SANDSTORM_TEMPORARY;
	gBattleCommunication[MULTISTRING_CHOOSER] = 3;

	if (ITEM_EFFECT(gBankAttacker) == ITEM_EFFECT_SMOOTH_ROCK)
		gWishFutureKnock.weatherDuration = 8;
	else
		gWishFutureKnock.weatherDuration = 5;

	gBattleScripting.animArg1 = B_ANIM_SANDSTORM_CONTINUES;
	return TRUE;
}

void atk95_setsandstorm(void)
{
	if (!SetSandstormWeather())
		gMoveResultFlags |= MOVE_RESULT_FAILED;

	gBattlescriptCurrInstr++;
}

static bool8 TakesGeneralWeatherDamage(u8 bank)
{
	u8 ability = ABILITY(bank);
	u8 effect = ITEM_EFFECT(bank);

	return WEATHER_HAS_EFFECT
		&& 	ability != ABILITY_MAGICGUARD
		&&	ability != ABILITY_OVERCOAT
		&&	effect  != ITEM_EFFECT_SAFETY_GOGGLES
		&& !(gStatuses3[bank] & (STATUS3_UNDERGROUND | STATUS3_UNDERWATER)); //For some strange reason, Pokemon using Shadow/Phantom Force still take weather damage
}

bool8 SandstormHurts(u8 bank)
{
	u8 ability = ABILITY(bank);

	if (TakesGeneralWeatherDamage(bank))
	{
		if (!IsOfType(bank, TYPE_ROCK) && !IsOfType(bank, TYPE_GROUND) && !IsOfType(bank, TYPE_STEEL)
		&& ability != ABILITY_SANDVEIL && ability != ABILITY_SANDRUSH && ability != ABILITY_SANDFORCE)
			return TRUE;
	}

	return FALSE;
}

bool8 TakesDamageFromSandstorm(u8 bank)
{
	if (WEATHER_HAS_EFFECT
	&&  gBattleWeather & WEATHER_SANDSTORM_ANY)
	{
		return SandstormHurts(bank);
	}

	return FALSE;
}

bool8 HailHurts(u8 bank)
{
	u8 ability = ABILITY(bank);

	if (TakesGeneralWeatherDamage(bank))
	{
		if (!IsOfType(bank, TYPE_ICE) && ability != ABILITY_ICEBODY && ability != ABILITY_SNOWCLOAK)
			return TRUE;
	}

	return FALSE;
}

bool8 TakesDamageFromHail(u8 bank)
{
	if (WEATHER_HAS_EFFECT
	&&  gBattleWeather & WEATHER_HAIL_ANY)
	{
		return HailHurts(bank);
	}

	return FALSE;
}

u32 GetSandstormDamage(u8 bank)
{
	u32 damage = 0;

	if (TakesDamageFromSandstorm(bank))
	{
		if (gBattleWeather & WEATHER_SANDSTORM_PRIMAL)
			damage = MathMax(1, GetBaseMaxHP(bank) / 8);
		else
			damage = MathMax(1, GetBaseMaxHP(bank) / 16);
	}

	return damage;
}

u32 GetHailDamage(u8 bank)
{
	u32 damage = 0;

	if (TakesDamageFromHail(bank))
		damage = MathMax(1, GetBaseMaxHP(bank) / 16);

	return damage;
}

void atk96_weatherdamage(void)
{
	u8 bank = gBankAttacker;
	gBattleMoveDamage = 0;

	if (gAbsentBattlerFlags & gBitTable[bank])
	{
		gBattleMoveDamage = 0;
	}
	#ifndef NO_GHOST_BATTLES //Ghosts can't take damage from Sand Stream or Snow Warning
	else if ((gBattleTypeFlags & (BATTLE_TYPE_SCRIPTED_WILD_1 | BATTLE_TYPE_GHOST)) == BATTLE_TYPE_GHOST
	&&  SIDE(bank) == B_SIDE_OPPONENT)
	{
		gBattleMoveDamage = 0;
	}
	#endif
	else
	{
		if (gBattleWeather & WEATHER_SANDSTORM_ANY)
		{
			gBattleMoveDamage = GetSandstormDamage(bank);

			if (gBattleWeather & WEATHER_SANDSTORM_PRIMAL)
			{
				gBattleStringLoader = gText_BuffetedByViciousSandstorm;
				gBattleCommunication[MULTISTRING_CHOOSER] = 2;
			}
		}
		else if (gBattleWeather & WEATHER_HAIL_ANY)
		{
			gBattleMoveDamage = GetHailDamage(bank);
		}
	}

	gNewBS->turnDamageTaken[bank] = gBattleMoveDamage; //For Emergency Exit
	gBattlescriptCurrInstr++;
}

void atk97_tryinfatuating(void)
{
	u8 bankAtk, bankDef;

	if (gBattlescriptCurrInstr[1] == BS_GET_TARGET)
	{
		bankAtk = gBankAttacker;
		bankDef = gBankTarget;
	}
	else
	{
		bankAtk = gBankTarget;
		bankDef = gBankAttacker;
	}

	if (ABILITY(bankDef) == ABILITY_OBLIVIOUS)
	{
		gBattlescriptCurrInstr = BattleScript_ObliviousPrevents;
	}
	else if (!CanBeInfatuated(bankDef, bankAtk))
	{
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 2);
	}
	else
	{
		gBattleMons[bankDef].status2 |= STATUS2_INFATUATED_WITH(bankAtk);
		gBattlescriptCurrInstr += 6;
	}
}

void atk99_setmist(void)
{
	if (BankSideHasMist(gBankAttacker))
	{
		gMoveResultFlags |= MOVE_RESULT_FAILED;
		gBattleCommunication[MULTISTRING_CHOOSER] = 1;
	}
	else
	{
		gSideStatuses[SIDE(gBankAttacker)] |= SIDE_STATUS_MIST;
		gSideTimers[SIDE(gBankAttacker)].mistTimer = 5;
		gSideTimers[SIDE(gBankAttacker)].mistBank = gBankAttacker;
		gBattleCommunication[MULTISTRING_CHOOSER] = 0;
	}
	gBattlescriptCurrInstr++;
}

void atk9B_transformdataexecution(void)
{
	gChosenMove = 0xFFFF;
	gBattlescriptCurrInstr++;
	if (ProtectedByMaxGuard(gBankTarget, gCurrentMove))
	{
		gMoveResultFlags |= MOVE_RESULT_FAILED;
		gBattleCommunication[6] = 1; //Protected
		gBattlescriptCurrInstr = BattleScript_ButItFailed;
	}
	else if (gBattleMons[gBankTarget].status2 & STATUS2_TRANSFORMED
	|| gStatuses3[gBankTarget] & (STATUS3_SEMI_INVULNERABLE | STATUS3_ILLUSION)
	|| gSideStatuses[SIDE(gBankTarget)] & SIDE_STATUS_CRAFTY_SHIELD
	#ifdef UNBOUND
	|| SPECIES(gBankTarget) == SPECIES_SHADOW_WARRIOR
	#endif
	|| (IsDynamaxed(gBankAttacker) && IsBannedDynamaxSpecies(SPECIES(gBankTarget)))
	|| HasRaidShields(gBankTarget)) //Shields can be used outside of a Raid Battle now
	{
		gMoveResultFlags |= MOVE_RESULT_FAILED;
		gBattleCommunication[MULTISTRING_CHOOSER] = 1;
	}
	else
	{
		gNewBS->backupAbility = ABILITY(gBankAttacker); //For removing primal weathers and neutralizing gas
		TransformPokemon(gBankAttacker, gBankTarget);
		gActiveBattler = gBankAttacker;
		EmitResetActionMoveSelection(0, RESET_MOVE_SELECTION);
		MarkBufferBankForExecution(gActiveBattler);
	}
}

static u32 CreatePersonalityRetainingVisualDataForOtId(u32 originalPersonality, u32 originalOtId, u32 newOtId, u16 species)
{
	//Tricks the game into duplicating the visual appearance of the copied Pokemon
	u32 personality;
	u16 sid = HIHALF(newOtId);
	u16 tid = LOHALF(newOtId);

	u8 nature = GetNatureFromPersonality(originalPersonality);
	u8 gender = GetGenderFromSpeciesAndPersonality(species, originalPersonality);
	u8 letter = GetUnownLetterFromPersonality(originalPersonality);
	bool8 isMinior = IsMinior(species);
	u16 miniorCore = GetMiniorCoreFromPersonality(originalPersonality);
	bool8 isShiny = IsShinyOtIdPersonality(originalOtId, originalPersonality);

	//Try to copy Spinda's spots if you can
	if (species == SPECIES_SPINDA)
	{
		if (isShiny) //Spinda is Shiny
		{
			if (IsShinyOtIdPersonality(newOtId, originalPersonality)) //The Ditto would become Shiny by transforming
				return originalPersonality; //Use the Spinda's actual personality
		}
		else //Spinda isn't Shiny
		{
			if (!IsShinyOtIdPersonality(newOtId, originalPersonality)) //The Ditto wouldn't become Shiny by transforming
				return originalPersonality; //Use the Spinda's actual personality
		}
	}

	//Randomize a personality until a match is found
	do
	{
		personality = Random32();

		if (isShiny)
		{
			u8 shinyRange = Random() % SHINY_ODDS;
			personality = (((shinyRange ^ (sid ^ tid)) ^ LOHALF(personality)) << 16) | LOHALF(personality);
		}
	} while (GetNatureFromPersonality(personality) != nature
	|| GetGenderFromSpeciesAndPersonality(species, personality) != gender
	|| (isShiny && !IsShinyOtIdPersonality(newOtId, personality))
	|| (!isShiny && IsShinyOtIdPersonality(newOtId, personality))
	|| (species == SPECIES_UNOWN && GetUnownLetterFromPersonality(personality) != letter)
	|| (isMinior && GetMiniorCoreFromPersonality(personality) != miniorCore));

	return personality;
}

void TransformPokemon(u8 bankAtk, u8 bankDef)
{
	u32 i;
	u8 *battleMonAttacker, *battleMonTarget;
	u16 species = SPECIES_NONE;

	gBattleMons[bankAtk].status2 |= STATUS2_TRANSFORMED;
	gDisableStructs[bankAtk].disabledMove = 0;
	gDisableStructs[bankAtk].disableTimer1 = 0;
	gDisableStructs[bankAtk].mimickedMoves = 0;
	gDisableStructs[bankAtk].transformedMonPersonality =
		CreatePersonalityRetainingVisualDataForOtId(gBattleMons[bankDef].personality, gBattleMons[bankDef].otId, gBattleMons[bankAtk].otId, SPECIES(bankDef));

	battleMonAttacker = (u8*)(&gBattleMons[bankAtk]);
	battleMonTarget = (u8*)(&gBattleMons[bankDef]);

	//The transformed mon's Hidden Power stays the same, so back it up here
	u8 originalHPIV = gBattleMons[bankAtk].hpIV;
	u8 originalAtkIV = gBattleMons[bankAtk].attackIV;
	u8 originalDefIV = gBattleMons[bankAtk].defenseIV;
	u8 originalSpAtkIV = gBattleMons[bankAtk].spAttackIV;
	u8 originalSpDefIV = gBattleMons[bankAtk].spDefenseIV;
	u8 originalSpeedIV = gBattleMons[bankAtk].speedIV;

	for (i = 0; i < offsetof(struct BattlePokemon, pp); i++)
		battleMonAttacker[i] = battleMonTarget[i];

	gBattleMons[bankAtk].hpIV = originalHPIV;
	gBattleMons[bankAtk].attackIV = originalAtkIV;
	gBattleMons[bankAtk].defenseIV = originalDefIV;
	gBattleMons[bankAtk].spAttackIV = originalSpAtkIV;
	gBattleMons[bankAtk].spDefenseIV = originalSpDefIV;
	gBattleMons[bankAtk].speedIV = originalSpeedIV;

	if (IsGigantamaxed(bankDef))
		species = GetGigantamaxBaseForm(SPECIES(bankDef));

	if (species == SPECIES_NONE)
		species = SPECIES(bankDef);

	gBattleMons[bankAtk].species = species; //Overrite the species with the potentially non-Gigantamaxed species
	PREPARE_SPECIES_BUFFER(gBattleTextBuff1, species)

	if (IS_BLANK_TYPE(gBattleMons[bankDef].type1)
	&&  IS_BLANK_TYPE(gBattleMons[bankDef].type2))
	{
		gBattleMons[bankAtk].type1 = TYPE_NORMAL;
		gBattleMons[bankAtk].type2 = TYPE_NORMAL;
	}

	for (i = 0; i < MAX_MON_MOVES; i++)
	{
		if (gBattleMoves[gBattleMons[bankAtk].moves[i]].pp < 5)
			gBattleMons[bankAtk].pp[i] = gBattleMoves[gBattleMons[bankAtk].moves[i]].pp;
		else
			gBattleMons[bankAtk].pp[i] = 5;
	}
	
	gNewBS->chiStrikeCritBoosts[bankAtk] = gNewBS->chiStrikeCritBoosts[bankDef];

	gBattleCommunication[MULTISTRING_CHOOSER] = 0;
	gBattleSpritesDataPtr->bankData[bankAtk].transformSpecies = species;
}

void atk9D_mimicattackcopy(void)
{
	gChosenMove = 0xFFFF;

	if (gBattleMons[gBankAttacker].status2 & STATUS2_TRANSFORMED
	|| gLastUsedMoves[gBankTarget] == 0
	|| gLastUsedMoves[gBankTarget] == 0xFFFF
	|| gSpecialMoveFlags[gLastUsedMoves[gBankTarget]].gMimicBannedMoves
	|| IsZMove(gLastUsedMoves[gBankTarget])
	|| IsAnyMaxMove(gLastUsedMoves[gBankTarget])
	|| IsZMove(gLastPrintedMoves[gBankTarget])
	|| IsAnyMaxMove(gLastPrintedMoves[gBankTarget]))
	{
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
	}
	else
	{
		int i;

		for (i = 0; i < MAX_MON_MOVES; i++)
		{
			if (gBattleMons[gBankAttacker].moves[i] == gLastUsedMoves[gBankTarget])
				break;
		}

		if (i == MAX_MON_MOVES)
		{
			gBattleMons[gBankAttacker].moves[gCurrMovePos] = gLastUsedMoves[gBankTarget];
			gBattleMons[gBankAttacker].pp[gCurrMovePos] = gBattleMoves[gLastUsedMoves[gBankTarget]].pp;

			PREPARE_MOVE_BUFFER(gBattleTextBuff1, gLastUsedMoves[gBankTarget])

			gDisableStructs[gBankAttacker].mimickedMoves |= gBitTable[gCurrMovePos];
			gBattlescriptCurrInstr += 5;
		}
		else
		{
			gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
		}
	}
}

void atk9E_metronome(void)
{
	bool8 isMetronomeBattle =
		(gBattleTypeFlags & BATTLE_TYPE_FRONTIER && VarGet(VAR_BATTLE_FACILITY_TIER) == BATTLE_FACILITY_METRONOME)
		#ifdef FLAG_METRONOME_BATTLE
		|| FlagGet(FLAG_METRONOME_BATTLE)
		#endif
		;

	do
	{
		gCurrentMove = umodsi(Random(), LAST_MOVE_INDEX) + 1;
	} while (IsZMove(gCurrentMove) || IsAnyMaxMove(gCurrentMove)
		|| (isMetronomeBattle && gCurrentMove == MOVE_IMPRISON) //Imprison would seal all Metronome which would be bad
		|| gSpecialMoveFlags[gCurrentMove].gMetronomeBannedMoves);

	TryUpdateCalledMoveWithZMove();
	UpdateMoveStartValuesForCalledMove();
	gBattlescriptCurrInstr = gBattleScriptsForMoveEffects[gBattleMoves[gCurrentMove].effect];
	gBankTarget = GetMoveTarget(gCurrentMove, 0);
}

void atkA0_psywavedamageeffect(void)
{
	gBattleMoveDamage = GetPsywaveDamage(gBattleMons[gBankAttacker].level, Random() % 101);
	++gBattlescriptCurrInstr;
}

s32 GetPsywaveDamage(u8 level, u8 randDamage)
{
	return MathMax(1, (level * (randDamage + 50)) / 100);
}

void atkA1_counterdamagecalculator(void) {
	u8 atkSide = SIDE(gBankAttacker);
	u8 defSide = SIDE(gProtectStructs[gBankAttacker].physicalBank);

	if (gProtectStructs[gBankAttacker].physicalDmg && atkSide != defSide && gBattleMons[gProtectStructs[gBankAttacker].physicalBank].hp)
	{
		gBattleMoveDamage = gProtectStructs[gBankAttacker].physicalDmg * 2;

		if (IsMoveRedirectedByFollowMe(gCurrentMove, gBankAttacker, defSide))
			gBankTarget = gSideTimers[defSide].followmeTarget;
		else
			gBankTarget = gProtectStructs[gBankAttacker].physicalBank;
		gBattlescriptCurrInstr += 5;
	}
	else
	{
		gSpecialStatuses[gBankAttacker].ppNotAffectedByPressure = 1;
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
	}
}

void atkA2_mirrorcoatdamagecalculator(void) {
	u8 atkSide = SIDE(gBankAttacker);
	u8 defSide = SIDE(gProtectStructs[gBankAttacker].specialBank);

	if (gProtectStructs[gBankAttacker].specialDmg && atkSide != defSide && gBattleMons[gProtectStructs[gBankAttacker].specialBank].hp)
	{
		gBattleMoveDamage = gProtectStructs[gBankAttacker].specialDmg * 2;

		if (IsMoveRedirectedByFollowMe(gCurrentMove, gBankAttacker, defSide))
			gBankTarget = gSideTimers[defSide].followmeTarget;
		else
			gBankTarget = gProtectStructs[gBankAttacker].specialBank;
		gBattlescriptCurrInstr += 5;
	}
	else
	{
		gSpecialStatuses[gBankAttacker].ppNotAffectedByPressure = 1;
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
	}
}

void atkA3_disablelastusedattack(void)
{
	u8 i, bankDef;
	u16 lastMove;

	if (gBattlescriptCurrInstr[1] == BS_GET_TARGET)
	{
		bankDef = gBankTarget;
		lastMove = gLastUsedMoves[bankDef];
	}
	else
	{
		bankDef = gBankAttacker; //For Cursed Body
		lastMove = gCurrentMove;
	}

	i = FindMovePositionInMoveset(lastMove, bankDef);

	if (i < MAX_MON_MOVES
	&& gDisableStructs[bankDef].disabledMove == 0
	&& gBattleMons[bankDef].pp[i] != 0
	&& !IsZMove(lastMove)
	&& !IsAnyMaxMove(lastMove)
	&& !IsZMove(gLastPrintedMoves[bankDef])
	&& !IsAnyMaxMove(gLastPrintedMoves[bankDef])
	&& !AbilityBattleEffects(ABILITYEFFECT_CHECK_BANK_SIDE, bankDef, ABILITY_AROMAVEIL, 0, 0))
	{
		PREPARE_MOVE_BUFFER(gBattleTextBuff1, gBattleMons[bankDef].moves[i])

		gDisableStructs[bankDef].disabledMove = gBattleMons[bankDef].moves[i];
		gDisableStructs[bankDef].disableTimer1 = 4;
		gDisableStructs[bankDef].disableTimer2 = gDisableStructs[bankDef].disableTimer1; // used to save the random amount of turns?
		gBattlescriptCurrInstr += 6;
	}
	else
	{
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 2);
	}
}

bool8 CanLastMoveNotBeEncored(u8 bank)
{
	u16 lastUsedMove = gLastUsedMoves[bank];
	u16 lastPrintedMove = gLastPrintedMoves[bank];

	return lastUsedMove == MOVE_STRUGGLE
		|| lastUsedMove == MOVE_ENCORE
		|| lastUsedMove == MOVE_DYNAMAXCANNON
		|| gSpecialMoveFlags[lastUsedMove].gMovesThatCallOtherMoves
		|| IsZMove(lastUsedMove)
		|| IsAnyMaxMove(lastUsedMove)
		|| IsZMove(lastPrintedMove)
		|| IsAnyMaxMove(lastPrintedMove)
		|| IsDynamaxed(bank);
}

void atkA4_trysetencore(void)
{
	int i;

	if (CanLastMoveNotBeEncored(gBankTarget)
	|| gNewBS->playedShellTrapMessage & gBitTable[gBankTarget])
	{
		i = MAX_MON_MOVES;
	}
	else
		i = FindMovePositionInMoveset(gLastUsedMoves[gBankTarget], gBankTarget);

	if (gDisableStructs[gBankTarget].encoredMove == 0
	&& i < MAX_MON_MOVES
	&& gBattleMons[gBankTarget].pp[i] != 0)
	{
		gDisableStructs[gBankTarget].encoredMove = gBattleMons[gBankTarget].moves[i];
		gDisableStructs[gBankTarget].encoredMovePos = i;
		gDisableStructs[gBankTarget].encoreTimer = 3;
		gDisableStructs[gBankTarget].encoreTimerStartValue = gDisableStructs[gBankTarget].encoreTimer;
		gBattlescriptCurrInstr += 5;
	}
	else
	{
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
	}
}

void atkA5_painsplitdmgcalc(void)
{
	if (!MoveBlockedBySubstitute(gCurrentMove, gBankAttacker, gBankTarget))
	{
		s32 hpDiff = (GetBaseCurrentHP(gBankAttacker) + GetBaseCurrentHP(gBankTarget)) / 2;
		s32 painSplitHp = gBattleMoveDamage = GetBaseCurrentHP(gBankTarget) - hpDiff;
		u8* storeLoc = (void*)(&gBattleScripting.painSplitHp);

		storeLoc[0] = (painSplitHp);
		storeLoc[1] = (painSplitHp & 0x0000FF00) >> 8;
		storeLoc[2] = (painSplitHp & 0x00FF0000) >> 16;
		storeLoc[3] = (painSplitHp & 0xFF000000) >> 24;

		gBattleMoveDamage = GetBaseCurrentHP(gBankAttacker) - hpDiff;
		gSpecialStatuses[gBankTarget].moveturnLostHP = 0xFFFF;

		gBattlescriptCurrInstr += 5;
	}
	else
	{
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
	}
}

void atkA6_settypetorandomresistance(void) //Conversion 2
{
	u8 moveType, defType;
	u32 i, rands;

	u8 bankAtk = gBankAttacker;
	u8 bankDef = gBankTarget;
	bool8 isInverseBattle = IsInverseBattle();

	if (gLastUsedMoves[bankDef] == MOVE_NONE || gLastUsedMoves[bankDef] == 0xFFFF)
	{
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
		return;
	}

	moveType = gNewBS->LastUsedTypes[bankDef];

	if (moveType == TYPE_MYSTERY || moveType == TYPE_BLANK || moveType == TYPE_ROOSTLESS)
	{
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
		return;
	}

	for (rands = 0; rands < 1000; ++rands)
	{
		defType = Random() % NUMBER_OF_MON_TYPES;

		if (IsOfType(bankAtk, defType))
			continue;

		if (!isInverseBattle)
		{
			if (gTypeEffectiveness[moveType][defType] == TYPE_MUL_NO_EFFECT
			||  gTypeEffectiveness[moveType][defType] == TYPE_MUL_NOT_EFFECTIVE)
			{
				SET_BATTLER_TYPE(bankAtk, defType);
				PREPARE_TYPE_BUFFER(gBattleTextBuff1, defType);
				gBattlescriptCurrInstr += 5;
				return;
			}
		}
		else //Inverse Battle
		{
			if (gTypeEffectiveness[moveType][defType] == TYPE_MUL_SUPER_EFFECTIVE)
			{
				SET_BATTLER_TYPE(bankAtk, defType);
				PREPARE_TYPE_BUFFER(gBattleTextBuff1, defType);
				gBattlescriptCurrInstr += 5;
				return;
			}
		}
	}

	//Couldn't find a type randomly so check all types
	for (i = 0; i < NUMBER_OF_MON_TYPES; ++i)
	{
		for (defType = 0; defType < NUMBER_OF_MON_TYPES; ++defType)
		{
			if (IsOfType(bankAtk, defType))
				continue;

			if (!isInverseBattle)
			{
				if (gTypeEffectiveness[moveType][defType] == TYPE_MUL_NO_EFFECT
				||  gTypeEffectiveness[moveType][defType] == TYPE_MUL_NOT_EFFECTIVE)
				{
					SET_BATTLER_TYPE(bankAtk, defType);
					PREPARE_TYPE_BUFFER(gBattleTextBuff1, defType);
					gBattlescriptCurrInstr += 5;
					return;
				}
			}
			else //Inverse Battle
			{
				if (gTypeEffectiveness[moveType][defType] == TYPE_MUL_SUPER_EFFECTIVE)
				{
					SET_BATTLER_TYPE(bankAtk, defType);
					PREPARE_TYPE_BUFFER(gBattleTextBuff1, defType);
					gBattlescriptCurrInstr += 5;
					return;
				}
			}
		}
	}

	gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1); //Basically against Normal-type moves in an Inverse Battle
}

void atkA8_copymovepermanently(void) // sketch
{
	//Use gLastPrintedMoves to allow copying Metronome, Transform & Mimic.
	u16 lastMove = (gLastPrintedMoves[gBankTarget] != 0 && gLastPrintedMoves[gBankTarget] != 0xFFFF) ? gLastPrintedMoves[gBankTarget] : gLastResultingMoves[gBankTarget];
	gChosenMove = 0xFFFF;

	if (!(gBattleMons[gBankAttacker].status2 & STATUS2_TRANSFORMED)
	&& !IsZMove(lastMove)
	&& !IsAnyMaxMove(lastMove)
	&& lastMove != MOVE_STRUGGLE
	&& lastMove != MOVE_CHATTER
	&& lastMove != MOVE_SKETCH
	&& lastMove != 0
	&& lastMove != 0xFFFF)
	{
		int i;

		for (i = 0; i < MAX_MON_MOVES; i++)
		{
			if (gBattleMons[gBankAttacker].moves[i] == MOVE_SKETCH)
				continue;
			if (gBattleMons[gBankAttacker].moves[i] == lastMove)
				break;
		}

		if (i != MAX_MON_MOVES)
		{
			gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
		}
		else // sketch worked
		{
			struct MovePpInfo movePpData;

			gBattleMons[gBankAttacker].moves[gCurrMovePos] = lastMove;
			gBattleMons[gBankAttacker].pp[gCurrMovePos] = gBattleMoves[lastMove].pp;
			gActiveBattler = gBankAttacker;

			for (i = 0; i < MAX_MON_MOVES; i++)
			{
				movePpData.moves[i] = gBattleMons[gBankAttacker].moves[i];
				movePpData.pp[i] = gBattleMons[gBankAttacker].pp[i];
			}
			movePpData.ppBonuses = gBattleMons[gBankAttacker].ppBonuses;

			EmitSetMonData(0, REQUEST_MOVES_PP_BATTLE, 0, sizeof(struct MovePpInfo), &movePpData);
			MarkBufferBankForExecution(gActiveBattler);

			PREPARE_MOVE_BUFFER(gBattleTextBuff1, lastMove)

			gBattlescriptCurrInstr += 5;
		}
	}
	else
	{
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
	}
}

void atkA9_trychoosesleeptalkmove(void)
{
	u32 i;
	u8 unusableMoves = 0;

	for (i = 0; i < MAX_MON_MOVES; ++i)
	{
		if (gSpecialMoveFlags[gBattleMons[gBankAttacker].moves[i]].gSleepTalkBannedMoves)
			unusableMoves |= gBitTable[i];
	}

	unusableMoves = CheckMoveLimitations(gBankAttacker, unusableMoves, 0xFF ^ MOVE_LIMITATION_PP ^ MOVE_LIMITATION_DISABLED ^ MOVE_LIMITATION_CHOICE);
	if (unusableMoves == 0xF) //All 4 moves cannot be chosen
	{
		gBattlescriptCurrInstr += 5;
	}
	else //At least one move can be chosen
	{
		u32 randomPos;
		do
		{
			randomPos = Random() & 3;
		} while ((gBitTable[randomPos] & unusableMoves));

		u8 sleepTalkPos = FindMovePositionInMoveset(gCurrentMove, gBankAttacker);
		if (sleepTalkPos < MAX_MON_MOVES)
			gNewBS->usedMoveIndices[gBankAttacker] |= gBitTable[sleepTalkPos]; //Allow Sleep Talk + Last Resort

		gCalledMove = gBattleMons[gBankAttacker].moves[randomPos];
		gCurrMovePos = randomPos;
		gHitMarker &= ~(HITMARKER_ATTACKSTRING_PRINTED);
		gBankTarget = GetMoveTarget(gCalledMove, 0);
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
		gHitMarker |= HITMARKER_NO_PPDEDUCT;
	}
}

void atkAA_setdestinybond(void) {
	if (gNewBS->DestinyBondCounters[gBankAttacker])
	{
		gBattlescriptCurrInstr = BattleScript_ButItFailed + 2;
	}
	else
	{
		gNewBS->DestinyBondCounters[gBankAttacker] = 2;
		gBattleMons[gBankAttacker].status2 |= STATUS2_DESTINY_BOND;
		gBattlescriptCurrInstr++;
	}
}

void atkAD_tryspiteppreduce(void)
{
	u8 toLose = (gCurrentMove == MOVE_EERIESPELL) ? 3 : 4;

	if (TrySpitePPReduce(gBankTarget, toLose))
		gBattlescriptCurrInstr += 5;
	else
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
}

bool8 TrySpitePPReduce(u8 bank, u8 lostPP)
{
	u8 movePos = FindMovePositionInMoveset(gLastUsedMoves[bank], bank);

	if (gLastUsedMoves[bank] != MOVE_NONE && gLastUsedMoves[bank] != 0xFFFF
	&& movePos < MAX_MON_MOVES && gBattleMons[bank].pp[movePos] > 0
	&& (!BATTLER_SEMI_INVULNERABLE(bank) || CanHitSemiInvulnerableTarget(gBankAttacker, bank, MOVE_SPITE)))
	{
		if (gBattleMons[bank].pp[movePos] < lostPP)
			lostPP = gBattleMons[bank].pp[movePos];

		PREPARE_MOVE_BUFFER(gBattleTextBuff1, gLastUsedMoves[bank]);
		ConvertIntToDecimalStringN(gBattleTextBuff2, lostPP, STR_CONV_MODE_LEFT_ALIGN, 1);
		PREPARE_BYTE_NUMBER_BUFFER(gBattleTextBuff2, 1, lostPP)

		gBattleMons[bank].pp[movePos] -= lostPP;
		gActiveBattler = bank;
		if (!(gDisableStructs[gActiveBattler].mimickedMoves & gBitTable[movePos]) && !IS_TRANSFORMED(gActiveBattler))
		{
			EmitSetMonData(0, REQUEST_PPMOVE1_BATTLE + movePos, 0, 1, &gBattleMons[gActiveBattler].pp[movePos]);
			MarkBufferBankForExecution(gActiveBattler);
		}

		if (gBattleMons[bank].pp[movePos] == 0)
			CancelMultiTurnMoves(bank);

		return TRUE;
	}

	return FALSE;
}

void atkAE_healpartystatus(void)
{
	u32 zero = 0;
	u8 toHeal = 0;

	if (CheckSoundMove(gCurrentMove))
	{
		int i;
		struct Pokemon *party;

		gBattleCommunication[MULTISTRING_CHOOSER] = 0;

		if (SIDE(gBankAttacker) == B_SIDE_PLAYER)
			party = gPlayerParty;
		else
			party = gEnemyParty;

		if (ABILITY(gBankAttacker) != ABILITY_SOUNDPROOF)
		{
			gBattleMons[gBankAttacker].status1 = 0;
			gBattleMons[gBankAttacker].status2 &= ~(STATUS2_NIGHTMARE);
		}
		else
		{
			RecordAbilityBattle(gBankAttacker, ABILITY(gBankAttacker));
			gBattleCommunication[MULTISTRING_CHOOSER] |= 1;
		}

		gActiveBattler = gBattleScripting.bank = GetBattlerAtPosition(GetBattlerPosition(gBankAttacker) ^ BIT_FLANK);

		if (IS_DOUBLE_BATTLE
		&& !(gAbsentBattlerFlags & gBitTable[gActiveBattler]))
		{
			if (ABILITY(gActiveBattler) != ABILITY_SOUNDPROOF)
			{
				gBattleMons[gActiveBattler].status1 = 0;
				gBattleMons[gActiveBattler].status2 &= ~(STATUS2_NIGHTMARE);
			}
			else
			{
				RecordAbilityBattle(gActiveBattler, ABILITY(gActiveBattler));
				gBattleCommunication[MULTISTRING_CHOOSER] |= 2;
			}
		}

		for (i = 0; i < PARTY_SIZE; ++i)
		{
			u16 species = GetMonData(&party[i], MON_DATA_SPECIES2, NULL);

			if (species != SPECIES_NONE && species != SPECIES_EGG)
			{
				u8 ability;

				if (i == gBattlerPartyIndexes[gBankAttacker])
					ability = ABILITY(gBankAttacker);
				else if (IS_DOUBLE_BATTLE
				&& i == gBattlerPartyIndexes[gActiveBattler]
				&& !(gAbsentBattlerFlags & gBitTable[gActiveBattler]))
					ability = ABILITY(gActiveBattler);
				else
					ability = GetMonAbility(&party[i]);

				if (ability != ABILITY_SOUNDPROOF)
					toHeal |= (1 << i);
			}
		}
	}
	else // Aromatherapy
	{
		gBattleCommunication[MULTISTRING_CHOOSER] = 4;
		toHeal = 0x3F;

		gBattleMons[gBankAttacker].status1 = 0;
		gBattleMons[gBankAttacker].status2 &= ~(STATUS2_NIGHTMARE);

		gActiveBattler = GetBattlerAtPosition(GetBattlerPosition(gBankAttacker) ^ BIT_FLANK);
		if (IS_DOUBLE_BATTLE
		&& !(gAbsentBattlerFlags & gBitTable[gActiveBattler]))
		{
			gBattleMons[gActiveBattler].status1 = 0;
			gBattleMons[gActiveBattler].status2 &= ~(STATUS2_NIGHTMARE);
		}
	}

	if (toHeal)
	{
		gActiveBattler = gBankAttacker;
		EmitSetMonData(0, REQUEST_STATUS_BATTLE, toHeal, 4, &zero);
		MarkBufferBankForExecution(gActiveBattler);
	}

	gBattlescriptCurrInstr++;
}

void atkAF_cursetarget(void)
{
	if (gBattleMons[gBankTarget].status2 & STATUS2_CURSED
	|| (BATTLER_SEMI_INVULNERABLE(gBankTarget) && !CanHitSemiInvulnerableTarget(gBankAttacker, gBankTarget, gCurrentMove))
	|| HasRaidShields(gBankTarget)) //Shields can be used outside of Raid Battles now
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
	else
	{
		gBattleMons[gBankTarget].status2 |= STATUS2_CURSED;
		gBattleMoveDamage = MathMax(1, gBattleMons[gBankAttacker].maxHP / 2);
		gBattlescriptCurrInstr += 5;
	}
}

void atkB0_trysetspikes(void)
{
	u8 atkSide = SIDE(gBankAttacker);
	u8 defSide = atkSide ^ BIT_SIDE;
	u8 stringcase = 0xFF;

	switch (gCurrentMove) {
		case MOVE_STEALTHROCK:
		case MOVE_G_MAX_STONESURGE_P:
		case MOVE_G_MAX_STONESURGE_S:
			if (gSideTimers[defSide].srAmount)
			{
				gSpecialStatuses[gBankAttacker].ppNotAffectedByPressure = 1;
				gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
			}
			else
			{
				gSideStatuses[defSide] |= SIDE_STATUS_SPIKES;
				gSideTimers[defSide].srAmount = 1;
				gBattlescriptCurrInstr += 5;
				stringcase = 1;
			}
			break;

		case MOVE_TOXICSPIKES:
			if (gSideTimers[defSide].tspikesAmount >= 2)
			{
				gSpecialStatuses[gBankAttacker].ppNotAffectedByPressure = 1;
				gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
			}
			else
			{
				gSideStatuses[defSide] |= SIDE_STATUS_SPIKES;
				gSideTimers[defSide].tspikesAmount++;
				gBattlescriptCurrInstr += 5;
				stringcase = 2;
			}
			break;

		case MOVE_STICKYWEB:
			if (gSideTimers[defSide].stickyWeb)
			{
				gSpecialStatuses[gBankAttacker].ppNotAffectedByPressure = 1;
				gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
			}
			else
			{
				gSideStatuses[defSide] |= SIDE_STATUS_SPIKES;
				gSideTimers[defSide].stickyWeb = 1;
				gSideTimers[defSide].stickyWebBank = gBankAttacker; //Used by Court Change
				gBattlescriptCurrInstr += 5;
				stringcase = 3;
			}
			break;

		case MOVE_G_MAX_STEELSURGE_P:
		case MOVE_G_MAX_STEELSURGE_S:
			if (gSideTimers[defSide].steelsurge)
			{
				gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
			}
			else
			{
				gSideStatuses[defSide] |= SIDE_STATUS_SPIKES;
				gSideTimers[defSide].steelsurge = 1;
				gBattlescriptCurrInstr += 5;
				stringcase = 4;
			}
			break;

		default:
			if (gSideTimers[defSide].spikesAmount >= 3)
			{
				gSpecialStatuses[gBankAttacker].ppNotAffectedByPressure = 1;
				gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
			}
			else {
				gSideStatuses[defSide] |= SIDE_STATUS_SPIKES;
				gSideTimers[defSide].spikesAmount++;
				gBattlescriptCurrInstr += 5;
				stringcase = 0;
			}
	}

	if (stringcase != 0xFF)
		gBattleStringLoader = (u8*) sEntryHazardsStrings[stringcase];
}

void atkB1_setforesight(void)
{
	gBattleMons[gBattlerTarget].status2 |= STATUS2_FORESIGHT;
	++gBattlescriptCurrInstr;
}

void atkB2_trysetperishsong(void)
{
	s32 i;
	s32 notAffectedCount = 0;
	bool8 priority = PriorityCalc(gBankAttacker, ACTION_USE_MOVE, gCurrentMove) > 0;
	bool8 defAbilityBlocksPriority = IsPriorityBlockingAbility(ABILITY(gBankTarget));
	bool8 attackerIsPrankster = ABILITY(gBankAttacker) == ABILITY_PRANKSTER;

	for (i = 0; i < gBattlersCount; ++i)
	{
		if (gStatuses3[i] & STATUS3_PERISH_SONG
		|| ABILITY(i) == ABILITY_SOUNDPROOF
		|| (priority > 0 && i != gBankAttacker && ((gTerrainType == PSYCHIC_TERRAIN && CheckGrounding(i)) || defAbilityBlocksPriority)) //Not affected by priority moves
		|| (attackerIsPrankster && IsOfType(i, TYPE_DARK))) //Not affected by Prankster
		{
			notAffectedCount++;
		}
		else
		{
			gStatuses3[i] |= STATUS3_PERISH_SONG;
			gDisableStructs[i].perishSongTimer = 3;
			gDisableStructs[i].perishSongTimerStartValue = 3;
		}
	}

	PressurePPLoseOnUsingPerishSong(gBankAttacker);

	if (notAffectedCount == gBattlersCount)
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
	else
		gBattlescriptCurrInstr += 5;
}

//Actual calc has been moved to GetBasePower function
void atkB3_rolloutdamagecalculation(void)
{
	if (gMoveResultFlags & MOVE_RESULT_NO_EFFECT)
	{
		CancelMultiTurnMoves(gBankAttacker);
		gBattlescriptCurrInstr = BattleScript_MoveMissedPause + 2; //Skip the PP Reduce
	}
	else
	{
		if (!(gBattleMons[gBankAttacker].status2 & STATUS2_MULTIPLETURNS)) // first hit
		{
			gDisableStructs[gBankAttacker].rolloutTimer = 5;
			gDisableStructs[gBankAttacker].rolloutTimerStartValue = 5;
			gBattleMons[gBankAttacker].status2 |= STATUS2_MULTIPLETURNS;
			gLockedMoves[gBankAttacker] = gCurrentMove;
		}
		if (--gDisableStructs[gBankAttacker].rolloutTimer == 0) // last hit
		{
			gBattleMons[gBankAttacker].status2 &= ~(STATUS2_MULTIPLETURNS);
			gNewBS->rolloutFinalHit = TRUE;
		}

		gBattlescriptCurrInstr++;
	}
}

void atkB4_jumpifconfusedandstatmaxed(void)
{
	if (IsConfused(gBankTarget) && !STAT_CAN_RISE(gBankTarget, gBattlescriptCurrInstr[1]))
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 2);
	else
		gBattlescriptCurrInstr += 6;
}

void atkB5_furycuttercalc(void)
{
	if (gMoveResultFlags & MOVE_RESULT_NO_EFFECT)
	{
		gDisableStructs[gBankAttacker].furyCutterCounter = 0;
	}
	else if (gLastPrintedMoves[gBankAttacker] != gChosenMove)
	{
		gDisableStructs[gBankAttacker].furyCutterCounter = 1; //Used once now
	}
	else
	{
		if (gDisableStructs[gBankAttacker].furyCutterCounter < 4)
			gDisableStructs[gBankAttacker].furyCutterCounter++;
	}

	gBattlescriptCurrInstr++;
}

void atkB7_presentdamagecalculation(void)
{
	int rand = Random() & 0xFF;

	if (rand < 102)
		gDynamicBasePower = 40;
	else if (rand < 178)
		gDynamicBasePower = 80;
	else if (rand < 204)
		gDynamicBasePower = 120;
	else
	{
		gBattleMoveDamage = MathMax(GetBaseMaxHP(gBankTarget) / 4, 1);
		gBattleMoveDamage *= -1;
	}

	if (rand < 204)
		gBattlescriptCurrInstr = BattleScript_HitFromCritCalc;
	else if (IsHealBlocked(gBankTarget))
		gBattlescriptCurrInstr = BattleScript_NoHealTargetAfterHealBlock;
	else if (BATTLER_MAX_HP(gBankTarget))
		gBattlescriptCurrInstr = BattleScript_AlreadyAtFullHp;
	else
	{
		gNewBS->ParentalBondOn = FALSE;
		gMultiHitCounter = 0;
		gMoveResultFlags &= ~(MOVE_RESULT_DOESNT_AFFECT_FOE);
		gBattlescriptCurrInstr = BattleScript_PresentHealTarget;
	}
}


const u16 gSafeguardStringIds[] =
{
	STRINGID_BUTITFAILED, STRINGID_PKMNCOVEREDBYVEIL, 0x184
};

void atkB8_setsafeguard(void)
{
	if (BankSideHasSafeguard(gBankAttacker))
	{
		gMoveResultFlags |= MOVE_RESULT_MISSED;
		gBattleCommunication[MULTISTRING_CHOOSER] = 0;
	}
	else
	{
		if (IsShadowShieldBattle() && !IsAffectedByShadowShieldBattle(gBankAttacker))
		{
			gMoveResultFlags |= MOVE_RESULT_MISSED;
			gBattleStringLoader = gText_ShadowShieldBattleBlocksSafeguard;
			gBattleCommunication[MULTISTRING_CHOOSER] = 2;
		}
		else
		{
			u8 side = SIDE(gBankAttacker);
			gSideStatuses[side] |= SIDE_STATUS_SAFEGUARD;
			gSideTimers[side].safeguardTimer = 5;
			gSideTimers[side].safeguardBank = gBankAttacker;
			gBattleCommunication[MULTISTRING_CHOOSER] = 1;
		}
	}

	gBattlescriptCurrInstr++;
}

void atkB9_magnitudedamagecalculation(void)
{
	if (gDynamicBasePower) //Power has already been calculated
	{
		gBattlescriptCurrInstr = (u8*) 0x81D6930;
		return;
	}

	int magnitude = umodsi(Random(), 100);

	if (magnitude < 5)
	{
		gDynamicBasePower = 10;
		magnitude = 4;
	}
	else if (magnitude < 15)
	{
		gDynamicBasePower = 30;
		magnitude = 5;
	}
	else if (magnitude < 35)
	{
		gDynamicBasePower = 50;
		magnitude = 6;
	}
	else if (magnitude < 65)
	{
		gDynamicBasePower = 70;
		magnitude = 7;
	}
	else if (magnitude < 85)
	{
		gDynamicBasePower = 90;
		magnitude = 8;
	}
	else if (magnitude < 95)
	{
		gDynamicBasePower = 110;
		magnitude = 9;
	}
	else
	{
		gDynamicBasePower = 150;
		magnitude = 10;
	}

	PREPARE_BYTE_NUMBER_BUFFER(gBattleTextBuff1, 2, magnitude)
	gBattlescriptCurrInstr++;
}


//In this function, the target attacks the attacker
void atkBA_jumpifnopursuitswitchdmg(void)
{
	int i;

	switch (gMultiHitCounter) {
		case 1:
			if (SIDE(gBankAttacker) == B_SIDE_PLAYER)
				gBankTarget = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
			else
				gBankTarget = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
			break;

		default:
			if (SIDE(gBankAttacker) == B_SIDE_PLAYER)
				gBankTarget = GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT);
			else
				gBankTarget = GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT);
	}

	u8 toDoAction = 0;
	for (i = gBattlersCount - 1; i >= 0; --i) //Go backwards - Can't rely on gActionsByTurnOrder for mons that already finished attacking
	{
		if (gBanksByTurnOrder[i] == gBankTarget)
		{
			toDoAction = gActionsByTurnOrder[i];
			break;
		}
		else if (gBanksByTurnOrder[i] == gBankAttacker) //If the attacker is reached it means the Pursuiter already atatcked
		{
			toDoAction = ACTION_FINISHED;
			break;
		}
	}

	if (gChosenActionByBank[gBankTarget] == ACTION_USE_MOVE
	&& toDoAction == ACTION_USE_MOVE
	&& gChosenMovesByBanks[gBankTarget] == MOVE_PURSUIT
	//&& gBankAttacker == gBattleStruct->moveTarget[gBankTarget] //Used pre Gen 4
	&& !(gBattleMons[gBankTarget].status1 & (STATUS_SLEEP | STATUS_FREEZE))
	&& BATTLER_ALIVE(gBankAttacker)
	&& !gDisableStructs[gBankTarget].truantCounter)
	{
		for (i = 0; i < gBattlersCount; ++i)
		{
			if (gBanksByTurnOrder[i] == gBankTarget)
				gActionsByTurnOrder[i] = ACTION_TRY_FINISH;
		}

		gCurrentMove = MOVE_PURSUIT;
		gBattlescriptCurrInstr += 5;
		gBattleScripting.animTurn = 1;
		gHitMarker &= ~(HITMARKER_ATTACKSTRING_PRINTED | HITMARKER_NO_ATTACKSTRING);
	}
	else
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
}

bool8 SetSunnyWeather(void)
{
	if (gBattleWeather & (WEATHER_SUN_ANY | WEATHER_PRIMAL_ANY | WEATHER_CIRCUS))
	{
		gBattleCommunication[MULTISTRING_CHOOSER] = 2;
		return FALSE;
	}

	gBattleWeather = WEATHER_SUN_TEMPORARY;
	gBattleCommunication[MULTISTRING_CHOOSER] = 4;

	if (ITEM_EFFECT(gBankAttacker) == ITEM_EFFECT_HEAT_ROCK)
		gWishFutureKnock.weatherDuration = 8;
	else
		gWishFutureKnock.weatherDuration = 5;

	gBattleScripting.animArg1 = B_ANIM_SUN_CONTINUES;
	return TRUE;
}

void atkBB_setsunny(void)
{
	if (!SetSunnyWeather())
		gMoveResultFlags |= MOVE_RESULT_FAILED;

	gBattlescriptCurrInstr++;
}

void atkBC_maxattackhalvehp(void)
{
	u32 halfHp = gBattleMons[gBankAttacker].maxHP / 2;

	if (gBattleMons[gBankAttacker].maxHP / 2 == 0)
	{
		halfHp = 1;
	}

	gBattleScripting.statChanger = INCREASE_2 | STAT_STAGE_ATK;
	gBattleScripting.animArg1 = 0xE + STAT_STAGE_ATK;
	gBattleScripting.animArg2 = 0;

	if (ABILITY(gBankAttacker) == ABILITY_CONTRARY)
	{
		if (STAT_STAGE(gBankAttacker, STAT_STAGE_ATK) > STAT_STAGE_MIN
		&& gBattleMons[gBankAttacker].hp > halfHp)
		{
			gBattleMons[gBankAttacker].statStages[STAT_STAGE_ATK - 1] = STAT_STAGE_MIN;
			gBattleMoveDamage = MathMax(1, gBattleMons[gBankAttacker].maxHP / 2);
			gBattlescriptCurrInstr += 5;
		}
		else
			gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
	}
	else if (STAT_STAGE(gBankAttacker, STAT_STAGE_ATK) < STAT_STAGE_MAX
	&& gBattleMons[gBankAttacker].hp > halfHp)
	{
		gBattleMons[gBankAttacker].statStages[STAT_STAGE_ATK - 1] = STAT_STAGE_MAX;
		gBattleMoveDamage = MathMax(1, gBattleMons[gBankAttacker].maxHP / 2);
		gBattlescriptCurrInstr += 5;
	}
	else
	{
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
	}
}

void atkBD_copyfoestats(void) //Psych up
{
	s32 i;

	if (gSideStatuses[SIDE(gBankTarget)] & SIDE_STATUS_CRAFTY_SHIELD
	|| ProtectedByMaxGuard(gBankTarget, gCurrentMove))
	{
		gBattleCommunication[6] = 1; //Protected
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
	}
	else
	{
		for (i = STAT_STAGE_ATK; i < BATTLE_STATS_NO; ++i)
			STAT_STAGE(gBankAttacker, i) = STAT_STAGE(gBankTarget, i);

		//Copy critical hit ratio
		gBattleMons[gBankAttacker].status2 &= ~STATUS2_FOCUS_ENERGY; //Remove old boost if had
		gBattleMons[gBankAttacker].status2 |= (gBattleMons[gBankTarget].status2 & STATUS2_FOCUS_ENERGY); //Give boost if opponent has
		gNewBS->chiStrikeCritBoosts[gBankAttacker] = gNewBS->chiStrikeCritBoosts[gBankTarget];
		gBattlescriptCurrInstr += 5;
	}
}

void atkBE_rapidspinfree(void)
{
	u8 bankAtk = gBankAttacker;
	u8 sideAtk = SIDE(bankAtk);
	u8 sideDef = SIDE(gBankTarget);

	if (gCurrentMove == MOVE_RAPIDSPIN)
	{
		if (gBattleMons[bankAtk].status2 & STATUS2_WRAPPED)
		{
			gBattleScripting.bank = gBankTarget;
			gBattleMons[bankAtk].status2 &= ~(STATUS2_WRAPPED);
			gNewBS->brokeFreeMessage &= ~(gBitTable[bankAtk]);
			gNewBS->sandblastCentiferno[bankAtk] = 0;
			gBankTarget = gBattleStruct->wrappedBy[bankAtk];

			PREPARE_MOVE_BUFFER(gBattleTextBuff1, gBattleStruct->wrappedMove[bankAtk]);
			BattleScriptPushCursor();
			gBattlescriptCurrInstr = BattleScript_WrapFree;
		}
		else if (gStatuses3[bankAtk] & STATUS3_LEECHSEED)
		{
			gStatuses3[bankAtk] &= ~(STATUS3_LEECHSEED);
			gStatuses3[bankAtk] &= ~(STATUS3_LEECHSEED_BATTLER);
			BattleScriptPushCursor();
			gBattlescriptCurrInstr = BattleScript_LeechSeedFree;
		}
		else if (gSideStatuses[sideAtk] & SIDE_STATUS_SPIKES)
		{
			gSideStatuses[sideAtk] &= ~(SIDE_STATUS_SPIKES);
			gSideTimers[sideAtk].spikesAmount = 0;
			gSideTimers[sideAtk].tspikesAmount = 0;
			gSideTimers[sideAtk].srAmount = 0;
			gSideTimers[sideAtk].stickyWeb = 0;
			gSideTimers[sideAtk].steelsurge = 0;
			BattleScriptPushCursor();
			gBattlescriptCurrInstr = BattleScript_SpikesFree;
		}
		else
		{
			if (gCurrentMove == MOVE_RAPIDSPIN //From Gen 8
			&& ABILITY(bankAtk) != ABILITY_SHEERFORCE
			&& STAT_CAN_RISE(bankAtk, STAT_STAGE_SPEED))
			{
				gBattleCommunication[MOVE_EFFECT_BYTE] = MOVE_EFFECT_SPD_PLUS_1 | MOVE_EFFECT_AFFECTS_USER;
				SetMoveEffect(TRUE, TRUE); //Automatically increments gBattlescriptCurrInstr
			}
			else
				gBattlescriptCurrInstr++;
		}
	}
	else //Defog + G-Max Windrage
	{
		if (gSideStatuses[sideAtk] & SIDE_STATUS_SPIKES)
		{
			gSideStatuses[sideAtk] &= ~(SIDE_STATUS_SPIKES);
			gSideTimers[sideAtk].spikesAmount = 0;
			gSideTimers[sideAtk].tspikesAmount = 0;
			gSideTimers[sideAtk].srAmount = 0;
			gSideTimers[sideAtk].stickyWeb = 0;
			gSideTimers[sideAtk].steelsurge = 0;
			BattleScriptPushCursor();
			gBattlescriptCurrInstr = BattleScript_PrintCustomString;
			gBattleStringLoader = RemovedEntryHazardsString;
		}
		else if (gSideStatuses[sideDef] & SIDE_STATUS_SPIKES)
		{
			gSideStatuses[sideDef] &= ~(SIDE_STATUS_SPIKES);
			gSideTimers[sideDef].spikesAmount = 0;
			gSideTimers[sideDef].tspikesAmount = 0;
			gSideTimers[sideDef].srAmount = 0;
			gSideTimers[sideDef].stickyWeb = 0;
			gSideTimers[sideDef].steelsurge = 0;
			BattleScriptPushCursor();
			gBattlescriptCurrInstr = BattleScript_PrintCustomString;
			gBattleStringLoader = RemovedEntryHazardsTargetSideString;
		}
		else if (gSideStatuses[sideDef] & (SIDE_STATUS_REFLECT))
		{
			gSideTimers[sideDef].reflectTimer = 0;
			TEXT_BUFFER_SIDE_STATUS(MOVE_REFLECT, SIDE_STATUS_REFLECT, sideDef);
		}
		else if (gSideStatuses[sideDef] & (SIDE_STATUS_LIGHTSCREEN))
		{
			gSideTimers[sideDef].lightscreenTimer = 0;
			TEXT_BUFFER_SIDE_STATUS(MOVE_LIGHTSCREEN, SIDE_STATUS_LIGHTSCREEN, sideDef);
		}
		else if (gSideStatuses[sideDef] & SIDE_STATUS_SAFEGUARD)
		{
			gSideTimers[sideDef].safeguardTimer = 0;
			TEXT_BUFFER_SIDE_STATUS(MOVE_SAFEGUARD, SIDE_STATUS_SAFEGUARD, sideDef);
		}
		else if (gSideStatuses[sideDef] & SIDE_STATUS_MIST)
		{
			gSideTimers[sideDef].mistTimer = 0;
			TEXT_BUFFER_SIDE_STATUS(MOVE_MIST, SIDE_STATUS_MIST, sideDef);
		}
		else if (gNewBS->AuroraVeilTimers[sideDef])
		{
			gNewBS->AuroraVeilTimers[sideDef] = 0;
			TEXT_BUFFER_SIDE_STATUS(MOVE_AURORAVEIL, 0, sideDef);
		}
		else if (gTerrainType != 0 //Since Gen 8
		&& !(gBattleTypeFlags & BATTLE_TYPE_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_TERRAIN)) //Terrain is permanent
		{
			BattleScriptPushCursor();
			gBattlescriptCurrInstr = BattleScript_SetTerrain; //Removes the Terrain
		}
		else
		{
			gNewBS->fogBlownAwayByDefog = TRUE; //No reactivating fog
			gBattlescriptCurrInstr++;
		}
	}
}

void atkC0_recoverbasedonsunlight(void)
{
	gBankTarget = gBankAttacker;

	if (!BATTLER_MAX_HP(gBankAttacker))
	{
		if (gBattleWeather == 0 || gBattleWeather & WEATHER_AIR_CURRENT_PRIMAL || !WEATHER_HAS_EFFECT)
		{
			NO_WEATHER_EFFECT:
			gBattleMoveDamage = GetBaseMaxHP(gBankAttacker) / 2;
		}
		else if (gCurrentMove == MOVE_SHOREUP)
		{
			if (gBattleWeather & WEATHER_SANDSTORM_ANY)
				gBattleMoveDamage = (2 * GetBaseMaxHP(gBankAttacker)) / 3;
			else
				gBattleMoveDamage = GetBaseMaxHP(gBankAttacker) / 2;
		}
		else if (gBattleWeather & WEATHER_SUN_ANY)
		{
			if (AffectedBySun(gBankAttacker))
				gBattleMoveDamage = (2 * GetBaseMaxHP(gBankAttacker)) / 3;
			else
				goto NO_WEATHER_EFFECT;
		}
		else //Not Sunny Weather
		{
			gBattleMoveDamage = GetBaseMaxHP(gBankAttacker) / 4;
		}

		gBattleMoveDamage = MathMax(1, gBattleMoveDamage);
		gBattleMoveDamage *= -1;
		gBattlescriptCurrInstr += 5;
	}
	else
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
}

void atkC2_selectfirstvalidtarget(void) {
	for (gBankTarget = 0; gBankTarget < gBattlersCount; ++gBankTarget) {
		if (gBankTarget == gBankAttacker) continue;

		if (!(gAbsentBattlerFlags & gBitTable[gBankTarget])) break;
	}

	//Stops certain animations from acting like they only target the partner
	if (IS_DOUBLE_BATTLE
	&&  gBankAttacker == GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT)
	&&  gBankTarget == GetBattlerAtPosition(B_POSITION_PLAYER_LEFT))
	{
		if (IsBattlerAlive(GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT)))
			gBankTarget = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
		else if (IsBattlerAlive(GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT)))
			gBankTarget = GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT);
	}

	gBattlescriptCurrInstr++;
}

void atkC3_trysetfutureattack(void)
{
	if (gWishFutureKnock.futureSightCounter[gBankTarget] != 0)
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
	else
	{
		gWishFutureKnock.futureSightMove[gBankTarget] = gCurrentMove;
		gWishFutureKnock.futureSightAttacker[gBankTarget] = gBankAttacker;
		gWishFutureKnock.futureSightPartyIndex[gBankTarget] = gBattlerPartyIndexes[gBankAttacker];
		gWishFutureKnock.futureSightCounter[gBankTarget] = 3;

		if (gCurrentMove == MOVE_DOOMDESIRE)
			gBattleCommunication[MULTISTRING_CHOOSER] = 1;
		else
			gBattleCommunication[MULTISTRING_CHOOSER] = 0;

		gBattlescriptCurrInstr += 5;
	}
}

void atkC5_setsemiinvulnerablebit(void)
{
	switch (gCurrentMove)
	{
		case MOVE_FLY:
		case MOVE_BOUNCE:
			gStatuses3[gBankAttacker] |= STATUS3_IN_AIR;
			break;

		case MOVE_DIG:
			gStatuses3[gBankAttacker] |= STATUS3_UNDERGROUND;
			break;

		case MOVE_DIVE:
			gStatuses3[gBankAttacker] |= STATUS3_UNDERWATER;
			break;

		case MOVE_SHADOWFORCE:
		case MOVE_PHANTOMFORCE:
			gStatuses3[gBankAttacker] |= STATUS3_DISAPPEARED;
			break;

		case MOVE_SKYDROP:
			gStatuses3[gBankAttacker] |= (STATUS3_SKY_DROP_ATTACKER | STATUS3_IN_AIR);
			gStatuses3[gBankTarget] |= (STATUS3_SKY_DROP_TARGET | STATUS3_IN_AIR);

			gNewBS->skyDropAttackersTarget[gBankAttacker] = gBankTarget;
			gNewBS->skyDropTargetsAttacker[gBankTarget] = gBankAttacker;
			gNewBS->NoMoreMovingThisTurn |= gBitTable[gBankTarget]; //The target can't attack anymore even if it's freed from the Sky Drop this turn

			if (gSideTimers[SIDE(gBankTarget)].followmeTarget == gBankTarget) //Removes Follow Me's effect
			{
				gSideTimers[SIDE(gBankTarget)].followmeTimer = 0;
				gNewBS->ragePowdered &= ~gBitTable[SIDE(gBankTarget)];
			}
	}

	gBattlescriptCurrInstr++;
}

void atkC6_clearsemiinvulnerablebit(void)
{
	switch (gCurrentMove)
	{
		case MOVE_FLY:
		case MOVE_BOUNCE:
			gStatuses3[gBankAttacker] &= ~STATUS3_IN_AIR;
			break;
		case MOVE_DIG:
			gStatuses3[gBankAttacker] &= ~STATUS3_UNDERGROUND;
			break;
		case MOVE_DIVE:
			gStatuses3[gBankAttacker] &= ~STATUS3_UNDERWATER;
			break;
		case MOVE_SHADOWFORCE:
		case MOVE_PHANTOMFORCE:
			gStatuses3[gBankAttacker] &= ~STATUS3_DISAPPEARED;
			break;
		case MOVE_SKYDROP:
			gStatuses3[gBankAttacker] &= ~(STATUS3_SKY_DROP_ATTACKER | STATUS3_IN_AIR);
			gStatuses3[gBankTarget] &= ~(STATUS3_SKY_DROP_TARGET | STATUS3_IN_AIR);
			gNewBS->skyDropAttackersTarget[gBankAttacker] = 0;
			gNewBS->skyDropTargetsAttacker[gBankTarget] = 0;
			break;
	}

	gBattlescriptCurrInstr++;
}

bool8 SetHailWeather(void)
{
	if (gBattleWeather & (WEATHER_HAIL_ANY | WEATHER_PRIMAL_ANY | WEATHER_CIRCUS))
	{
		gBattleCommunication[MULTISTRING_CHOOSER] = 2;
		return FALSE;
	}

	gBattleWeather = WEATHER_HAIL_TEMPORARY;
	gBattleCommunication[MULTISTRING_CHOOSER] = 5;

	if (ITEM_EFFECT(gBankAttacker) == ITEM_EFFECT_ICY_ROCK)
		gWishFutureKnock.weatherDuration = 8;
	else
		gWishFutureKnock.weatherDuration = 5;

	gBattleScripting.animArg1 = B_ANIM_HAIL_CONTINUES;
	return TRUE;
}

void atkC8_sethail(void)
{
	if (!SetHailWeather())
		gMoveResultFlags |= MOVE_RESULT_FAILED;

	gBattlescriptCurrInstr++;
}

void atkC9_jumpifattackandspecialattackcannotfall(void) // memento
{
	if (!STAT_CAN_FALL(gBankTarget, STAT_ATK)
	&& !STAT_CAN_FALL(gBankTarget, STAT_SPATK)
	&& gBattleCommunication[6] != 1)
	{
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
	}
	else
	{
		gActiveBattler = gBankAttacker;
		gBattleMoveDamage = gBattleMons[gActiveBattler].hp;
		EmitHealthBarUpdate(0, INSTANT_HP_BAR_DROP);
		MarkBufferBankForExecution(gActiveBattler);
		gBattlescriptCurrInstr += 5;
	}
}

void atkCA_setforcedtarget(void) //Follow me
{
	switch (gCurrentMove) {
		case MOVE_SPOTLIGHT:
			gSideTimers[SIDE(gBankTarget)].followmeTimer = 1;
			gSideTimers[SIDE(gBankTarget)].followmeTarget = gBankTarget;
			gNewBS->ragePowdered &= ~gBitTable[SIDE(gBankTarget)];
			break;
		case MOVE_RAGEPOWDER:
			gSideTimers[SIDE(gBankAttacker)].followmeTimer = 1;
			gSideTimers[SIDE(gBankAttacker)].followmeTarget = gBankAttacker;
			gNewBS->ragePowdered |= gBitTable[SIDE(gBankAttacker)];
			break;
		default:
			gSideTimers[SIDE(gBankAttacker)].followmeTimer = 1;
			gSideTimers[SIDE(gBankAttacker)].followmeTarget = gBankAttacker;
			gNewBS->ragePowdered &= ~gBitTable[SIDE(gBankAttacker)];
	}

	gBattlescriptCurrInstr++;
}

void atkCC_callterrainattack(void) //nature power
{
	gCurrentMove = GetNaturePowerMove();
	TryUpdateCalledMoveWithZMove();
	UpdateMoveStartValuesForCalledMove();
	BattleScriptPush(gBattleScriptsForMoveEffects[gBattleMoves[gCurrentMove].effect]);
	gBattlescriptCurrInstr++;
}

u16 GetNaturePowerMove(void)
{
	u16 move;

	switch (gTerrainType) {
		case ELECTRIC_TERRAIN:
			move = gTerrainTable[0].naturePowerMove;
			break;

		case GRASSY_TERRAIN:
			move = gTerrainTable[1].naturePowerMove;
			break;

		case MISTY_TERRAIN:
			move = gTerrainTable[2].naturePowerMove;
			break;

		case PSYCHIC_TERRAIN:
			move = gTerrainTable[3].naturePowerMove;
			break;

		default:
			if (IsTerrainMoveIndoors())
				move = gTerrainTable[BATTLE_TERRAIN_INSIDE + 4].naturePowerMove;
			else
				move = gTerrainTable[GetBattleTerrainOverride() + 4].naturePowerMove;
	}

	return move;
}

void atkCD_cureifburnedparalysedorpoisoned(void) // refresh
{
	if (gBattleMons[gBankAttacker].status1 & (STATUS1_POISON | STATUS1_BURN | STATUS1_PARALYSIS | STATUS1_TOXIC_POISON))
	{
		gBattleMons[gBankAttacker].status1 = 0;
		gBattlescriptCurrInstr += 5;
		gActiveBattler = gBankAttacker;
		EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[gActiveBattler].status1);
		MarkBufferBankForExecution(gActiveBattler);
	}
	else
	{
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
	}
}

void atkCE_settorment(void)
{
	if (!CanBeTormented(gBankTarget) || !BATTLER_ALIVE(gBankTarget))
	{
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
	}
	else
	{
		gBattleMons[gBankTarget].status2 |= STATUS2_TORMENT;
		gBattlescriptCurrInstr += 5;
	}
}

void atkCF_jumpifnodamage(void)
{
	if (gProtectStructs[gBankAttacker].physicalDmg
	||  gProtectStructs[gBankAttacker].specialDmg
	||  !(gNewBS->playedFocusPunchMessage & gBitTable[gBankAttacker])) //Forced to use Focus Punch by Encore
		gBattlescriptCurrInstr += 5;
	else
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
}

void atkD0_settaunt(void)
{
	if (!IsTaunted(gBankTarget))
	{
		gDisableStructs[gBankTarget].tauntTimer = 4;
		gDisableStructs[gBankTarget].tauntTimer2 = 4;
		gBattlescriptCurrInstr += 5;
	}
	else
	{
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
	}
}

bool8 CanSwapItems(u8 bankAtk, u8 bankDef)
{
	u16 atkItem = ITEM(bankAtk);
	u16 defItem = ITEM(bankDef);
	u16 atkSpecies = SPECIES(bankAtk);
	u16 defSpecies = SPECIES(bankDef);

	//Can't swap if two pokemon don't have an item
	//Or if either of them is a forbidden item
	if ((atkItem == ITEM_NONE && defItem == ITEM_NONE) //Neither holding items
	|| (atkItem != ITEM_NONE && !CanTransferItem(atkSpecies, atkItem))
	|| (defItem != ITEM_NONE && !CanTransferItem(atkSpecies, defItem))
	|| (defItem != ITEM_NONE && !CanTransferItem(defSpecies, defItem))
	|| (atkItem != ITEM_NONE && !CanTransferItem(defSpecies, atkItem))
	|| (gNewBS->corrodedItems[SIDE(bankAtk)] & gBitTable[gBattlerPartyIndexes[bankAtk]])
	|| (gNewBS->corrodedItems[SIDE(bankDef)] & gBitTable[gBattlerPartyIndexes[bankDef]]))
		return FALSE;
	
	return TRUE;
}

void atkD1_trysethelpinghand(void)
{
    gBankTarget = PARTNER(gBankAttacker);

    if (IS_DOUBLE_BATTLE
	&& !(gAbsentBattlerFlags & gBitTable[gBankTarget])
	&& !gProtectStructs[gBankAttacker].helpingHand
	&& !gProtectStructs[gBankTarget].helpingHand
	&& !BankMovedBefore(gBankTarget, gBankAttacker))
    {
        gProtectStructs[gBankTarget].helpingHand = 1;
        gBattlescriptCurrInstr += 5;
    }
    else
    {
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    }
}

void atkD2_tryswapitems(void) //Trick
{
	// Wild Pokemon can't swap items with player
	if ((SIDE(gBankAttacker) == B_SIDE_OPPONENT && !(gBattleTypeFlags & (BATTLE_TYPE_TRAINER)))
	|| IsRaidBattle())
	{
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
	}
	else
	{
		if (!CanSwapItems(gBankAttacker, gBankTarget))
		{
			gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
		}
		else if (ABILITY(gBankTarget) == ABILITY_STICKYHOLD) //Check if ability prevents swapping
		{
			gBattlescriptCurrInstr = BattleScript_StickyHoldActivates;
			gLastUsedAbility = ABILITY(gBankTarget);
			RecordAbilityBattle(gBankTarget, gLastUsedAbility);
		}
		else //Took a while, but all checks passed and items can be safely swapped
		{
			u16 oldItemAtk, *newItemAtk;

			newItemAtk = &gBattleStruct->changedItems[gBankAttacker];
			oldItemAtk = ITEM(gBankAttacker);
			*newItemAtk = ITEM(gBankTarget);

			gBattleMons[gBankAttacker].item = *newItemAtk;
			gBattleMons[gBankTarget].item = oldItemAtk;
			HandleUnburdenBoost(gBankAttacker); //Give or take away Unburden boost
			HandleUnburdenBoost(gBankTarget); //Give or take away Unburden boost
			RecordItemEffectBattle(gBankAttacker, ItemId_GetHoldEffect(ITEM(gBankAttacker))); //Must use ItemId_GetHoldEffect explicitly in case item effect can't be used currently
			RecordItemEffectBattle(gBankTarget, ItemId_GetHoldEffect(ITEM(gBankTarget)));

			gActiveBattler = gBankAttacker;
			EmitSetMonData(0, REQUEST_HELDITEM_BATTLE, 0, 2, &gBattleMons[gBankAttacker].item);
			MarkBufferBankForExecution(gBankAttacker);

			gActiveBattler = gBankTarget;
			EmitSetMonData(0, REQUEST_HELDITEM_BATTLE, 0, 2, &gBattleMons[gBankTarget].item);
			MarkBufferBankForExecution(gBankTarget);

			gBattleStruct->choicedMove[gBankTarget] = 0;
			gBattleStruct->choicedMove[gBankTarget] = 0;

			gBattleStruct->choicedMove[gBankAttacker] = 0;
			gBattleStruct->choicedMove[gBankAttacker] = 0;

			gBattlescriptCurrInstr += 5;

			PREPARE_ITEM_BUFFER(gBattleTextBuff1, *newItemAtk);
			PREPARE_ITEM_BUFFER(gBattleTextBuff2, oldItemAtk);

			if (oldItemAtk != 0 && *newItemAtk != 0)
				gBattleCommunication[MULTISTRING_CHOOSER] = 2; // attacker's item -> <- target's item
			else if (oldItemAtk == 0 && *newItemAtk != 0)
				gBattleCommunication[MULTISTRING_CHOOSER] = 0; // nothing -> <- target's item
			else
				gBattleCommunication[MULTISTRING_CHOOSER] = 1; // attacker's item -> <- nothing
		}
	}
}

void atkD3_trycopyability(void) //Role Play
{
	if (gBattleExecBuffer) return;

	u8* atkAbilityLoc, *defAbilityLoc;
	u8 atkAbility, defAbility;

	//Get correct location of ability
	atkAbilityLoc = GetAbilityLocation(gBankAttacker);
	defAbilityLoc = GetAbilityLocation(gBankTarget);

	atkAbility = *atkAbilityLoc;
	defAbility = *defAbilityLoc;

	if (atkAbility == defAbility
	||  defAbility == ABILITY_NONE
	||  gSpecialAbilityFlags[atkAbility].gRolePlayAttackerBannedAbilities
	||  gSpecialAbilityFlags[defAbility].gRolePlayBannedAbilities)
	{
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
	}
	else
	{
		gNewBS->backupAbility = atkAbility;
		*atkAbilityLoc = defAbility;
		SetTookAbilityFrom(gBankAttacker, gBankTarget);
		gLastUsedAbility = atkAbility; //To display what changed
		TransferAbilityPopUp(gBankAttacker, gLastUsedAbility);
		ResetVarsForAbilityChange(gBankAttacker);
		gBattlescriptCurrInstr += 5;
	}
}

void atkD4_trywish(void)
{
	switch (gBattlescriptCurrInstr[1])
	{
		case 0: // use wish
			if (gWishFutureKnock.wishCounter[gBankAttacker] == 0)
			{
				gWishFutureKnock.wishCounter[gBankAttacker] = 2;
				gWishFutureKnock.wishMonId[gBankAttacker] = gBattlerPartyIndexes[gBankAttacker];
				gBattlescriptCurrInstr += 6;
			}
			else
			{
				gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 2);
			}
			break;
		case 1: // heal effect
			PREPARE_MON_NICK_WITH_PREFIX_BUFFER(gBattleTextBuff1, gBankTarget, gWishFutureKnock.wishMonId[gBankTarget])
			if (!BATTLER_MAX_HP(gBankTarget))
			{
				gBattleMoveDamage = GetWishHPRecovery(gBankTarget, TRUE);
				gBattleMoveDamage *= -1;
				gBattlescriptCurrInstr += 6;
			}
			else
				gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 2);

			break;
	}
}

void atkD8_setdamagetohealthdifference(void)
{
	gBattleMoveDamage = GetBaseCurrentHP(gBankTarget) - GetBaseCurrentHP(gBankAttacker);

	if (gBattleMoveDamage <= 0)
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
	else
		gBattlescriptCurrInstr += 5;
}

void atkDA_tryswapabilities(void) //Skill Swap
{
	u8* atkAbilityLoc, *defAbilityLoc;
	u8 atkAbility, defAbility;

	//Get correct location of ability
	atkAbilityLoc = GetAbilityLocation(gBankAttacker);
	defAbilityLoc = GetAbilityLocation(gBankTarget);

	atkAbility = *atkAbilityLoc;
	defAbility = *defAbilityLoc;

	if (atkAbility == ABILITY_NONE || defAbility == ABILITY_NONE
	|| IsDynamaxed(gBankAttacker) || IsDynamaxed(gBankTarget)
	|| gSpecialAbilityFlags[atkAbility].gSkillSwapBannedAbilities || gSpecialAbilityFlags[defAbility].gSkillSwapBannedAbilities
	|| gMoveResultFlags & MOVE_RESULT_NO_EFFECT)
	{
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
	}
	else
	{
		*atkAbilityLoc = defAbility;
		*defAbilityLoc = atkAbility;
		SwapTookAbilityFrom(gBankAttacker, gBankTarget);

		ResetVarsForAbilityChange(gBankAttacker);
		ResetVarsForAbilityChange(gBankTarget);

		if (*atkAbilityLoc == ABILITY_TRUANT)
			gDisableStructs[gBankAttacker].truantCounter = 0; //Reset counter

		if (*defAbilityLoc == ABILITY_TRUANT)
			gDisableStructs[gBankTarget].truantCounter = 0; //Reset counter

		gBattlescriptCurrInstr += 5;
	}
}

void atkDB_tryimprision(void)
{
	if ((gStatuses3[gBankAttacker] & STATUS3_IMPRISONED))
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
	else
	{
		PressurePPLoseOnUsingImprision(gBankAttacker);
		gStatuses3[gBankAttacker] |= STATUS3_IMPRISONED;
		gBattlescriptCurrInstr += 5;
	}
}

void atkDE_asistattackselect(void)
{
	s32 chooseableMovesNo = 0;
	struct Pokemon* party;
	s32 monId, moveId;
	u16* movesArray = gBattleStruct->assistPossibleMoves;
	u8 firstMonId, lastMonId;

	party = LoadPartyRange(gBankAttacker, &firstMonId, &lastMonId);

	for (monId = firstMonId; monId < lastMonId; ++monId)
	{
		if (monId == gBattlerPartyIndexes[gBankAttacker]
		|| party[monId].species == SPECIES_NONE
		|| GetMonData(&party[monId], MON_DATA_IS_EGG, 0))
			continue;

		for (moveId = 0; moveId < MAX_MON_MOVES; moveId++)
		{
			u16 move = party[monId].moves[moveId];

			if (gSpecialMoveFlags[move].gAssistBannedMoves)
				continue;

			if (move == MOVE_NONE)
				continue;

			movesArray[chooseableMovesNo] = move;
			chooseableMovesNo++;
		}
	}
	if (chooseableMovesNo)
	{
		gHitMarker &= ~(HITMARKER_ATTACKSTRING_PRINTED);
		gCalledMove = movesArray[((Random() & 0xFF) * chooseableMovesNo) >> 8];
		gBankTarget = GetMoveTarget(gCalledMove, 0);
		gBattlescriptCurrInstr += 5;
	}
	else
	{
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
	}
}

void atkE1_trygetintimidatetarget(void)
{
	u8 side;

	gBattleScripting.bank = gBattleStruct->intimidateBank;
	side = SIDE(gBattleScripting.bank);

	PREPARE_ABILITY_BUFFER(gBattleTextBuff1, ABILITY(gBattleScripting.bank))

	for (; gBankTarget < gBattlersCount; ++gBankTarget)
	{
		if (SIDE(gBankTarget) == side)
			continue;
		if (!(gAbsentBattlerFlags & gBitTable[gBankTarget]) && BATTLER_ALIVE(gBankTarget))
			break;
	}

	if (gBankTarget >= gBattlersCount)
	{
		gBankTarget = gNewBS->originalTargetBackup; //Prevent problems during Neutralizing Gas
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
	}
	else
		gBattlescriptCurrInstr += 5;
}

u8 GetSecretPowerEffect(void)
{
	u8 effect;

	switch (gTerrainType) {
		case ELECTRIC_TERRAIN:
			effect = gTerrainTable[0].secretPowerEffect;
			break;

		case GRASSY_TERRAIN:
			effect = gTerrainTable[1].secretPowerEffect;
			break;

		case MISTY_TERRAIN:
			effect = gTerrainTable[2].secretPowerEffect;
			break;

		case PSYCHIC_TERRAIN:
			effect = gTerrainTable[3].secretPowerEffect;
			break;

		default:
			if (IsTerrainMoveIndoors())
				effect = gTerrainTable[BATTLE_TERRAIN_INSIDE + 4].secretPowerEffect;
			else
				effect = gTerrainTable[GetBattleTerrainOverride() + 4].secretPowerEffect;
	}

	return effect;
}

void atkE4_getsecretpowereffect(void)
{
	gBattleCommunication[MOVE_EFFECT_BYTE] = GetSecretPowerEffect();
	gBattlescriptCurrInstr++;
}

void atkE5_pickupitemcalculation(void)
{
	for (; gNewBS->pickupMonId < PARTY_SIZE; ++gNewBS->pickupMonId)
	{
		u16 item, chance, species, level;
		struct Pokemon* mon = &gPlayerParty[gNewBS->pickupMonId];
		species = GetMonData(mon, MON_DATA_SPECIES2, NULL);

		if (species == SPECIES_NONE || species == SPECIES_EGG
		#ifndef PICKUP_ITEMS_STRAIGHT_TO_BAG //No issue with Pokemon already holding an item since they won't carry it anyway
		|| GetMonData(mon, MON_DATA_HELD_ITEM, NULL) != ITEM_NONE
		#endif
		)
			continue;

		level = GetMonData(mon, MON_DATA_LEVEL, NULL);

		switch (GetMonAbility(mon))
		{
			case ABILITY_PICKUP:
				chance = 10; // ~10% chance of pickup to activate
				item = ChoosePickupItem(level);
				break;
			case ABILITY_HONEYGATHER:
				chance = 5 + 5 * ((level - 1) / 10);
				item = ITEM_HONEY;
				break;
			default:
				continue;
		}

		if (Random() % 100 < chance)
		{
			IncrementGameStat(GAME_STAT_PICKUP_ITEMS);

			#ifdef PICKUP_ITEMS_STRAIGHT_TO_BAG
			if (CheckBagHasSpace(item, 1))
			{
				AddBagItem(item, 1);
				gLastUsedItem = item;
				PREPARE_MON_NICK_BUFFER(gBattleTextBuff1, GetBattlerAtPosition(B_POSITION_PLAYER_LEFT), gNewBS->pickupMonId);
				BattleScriptPush(gBattlescriptCurrInstr);
				#ifdef UNBOUND
				gBattleStringLoader = gText_PickUpItemToCube;
				#else
				gBattleStringLoader = gText_PickUpItemToBag;
				#endif
				++gNewBS->pickupMonId;
				gBattlescriptCurrInstr = BattleScript_PrintCustomString;
				return;
			}
			#else
			SetMonData(mon, MON_DATA_HELD_ITEM, &item);
			#endif
		}
	}

	++gBattlescriptCurrInstr;

	#ifdef PICK_UP_KNOCKED_OFF_ITEMS
	if (gNewBS->knockedOffWildItem != ITEM_NONE && CheckBagHasSpace(gNewBS->knockedOffWildItem, 1))
	{
		AddBagItem(gNewBS->knockedOffWildItem, 1);
		gLastUsedItem = gNewBS->knockedOffWildItem;
		BattleScriptPush(gBattlescriptCurrInstr);
		gBattleStringLoader = gText_PickedUpKnockedOffItem;
		gBattlescriptCurrInstr = BattleScript_PrintCustomString;
	}
	#endif
}

static item_t ChoosePickupItem(u8 level)
{
	u32 row_num = (level - 1) / 10;
	const item_t* const common_row = sPickupCommonItems + row_num;
	const item_t* const rare_row = sPickupRareItems + row_num;

	u16 k = Random();

	for (u32 i = 0; i < PU_COMMON_PER_ROW; ++i)
		if (k < pickup_common_item_ceilings[i])
			return common_row[i];

	for (u32 i = 0; i < PU_RARE_PER_ROW; ++i)
		if (k < pickup_rare_item_ceilings[i])
			return rare_row[i];

	// will never be reached
	return ITEM_NONE;
}

void atkE7_trycastformdatachange(void)
{
	if (gBattleExecBuffer) return;

	u8 form;
	u8 bank = gBattleScripting.bank;

	gBattlescriptCurrInstr++;

	if (BATTLER_ALIVE(bank))
	{
		switch (SPECIES(bank)) { //Not ability b/c you can lose ability
			#ifdef SPECIES_CASTFORM
			case SPECIES_CASTFORM:
				form = CastformDataTypeChange(bank);
				if (form)
				{
					BattleScriptPushCursorAndCallback(BattleScript_CastformChange);
					gBattleStruct->castformToChangeInto = form - 1;
				}
				break;
			#endif

			#if (defined SPECIES_CHERRIM && defined SPECIES_CHERRIM_SUN)
			case SPECIES_CHERRIM:
				if (ABILITY(bank) == ABILITY_FLOWERGIFT && !IS_TRANSFORMED(bank)
				&& WEATHER_HAS_EFFECT && gBattleWeather & WEATHER_SUN_ANY
				&& AffectedBySun(bank))
				{
					DoFormChange(bank, SPECIES_CHERRIM_SUN, FALSE, FALSE, FALSE);
					BattleScriptPushCursorAndCallback(BattleScript_FlowerGift);
				}
				break;

			case SPECIES_CHERRIM_SUN:
				if (ABILITY(bank) != ABILITY_FLOWERGIFT
				|| !WEATHER_HAS_EFFECT || !(gBattleWeather & WEATHER_SUN_ANY)
				|| !AffectedBySun(bank))
				{
					if (!IS_TRANSFORMED(bank))
					{
						DoFormChange(bank, SPECIES_CHERRIM, FALSE, FALSE, FALSE);
						BattleScriptPushCursorAndCallback(BattleScript_FlowerGift);
					}
				}
				break;
			#endif

			#if (defined SPECIES_EISCUE && defined SPECIES_EISCUE_NOICE)
			case SPECIES_EISCUE_NOICE:
				if (ABILITY(bank) == ABILITY_ICEFACE && !IS_TRANSFORMED(bank)
				&& WEATHER_HAS_EFFECT && gBattleWeather & WEATHER_HAIL_ANY)
				{
					DoFormChange(bank, SPECIES_EISCUE, FALSE, FALSE, FALSE);
					BattleScriptPushCursorAndCallback(BattleScript_IceFaceRestoreFace);
				}
				break;
			#endif
		}
	}
}

u8 CastformDataTypeChange(unusedArg u8 bank)
{
	u8 formChange = CASTFORM_NO_CHANGE;

	#ifdef SPECIES_CASTFORM
	if (SPECIES(bank) != SPECIES_CASTFORM || !BATTLER_ALIVE(bank))
		return formChange;

	u8 ability = ABILITY(bank);
	bool8 weatherHasEffect = WEATHER_HAS_EFFECT;

	if (gBattleMonForms[bank] != CASTFORM_NORMAL
	&& (!weatherHasEffect
	 || ability != ABILITY_FORECAST
	 || !(gBattleWeather & (WEATHER_RAIN_ANY | WEATHER_SUN_ANY | WEATHER_HAIL_ANY))))
	{
		SET_BATTLER_TYPE(bank, TYPE_NORMAL);
		formChange = CASTFORM_TO_NORMAL;
	}
	else if (weatherHasEffect && ability == ABILITY_FORECAST)
	{
		if (gBattleWeather & WEATHER_SUN_ANY && AffectedBySun(bank))
		{
			if (gBattleMonForms[bank] != CASTFORM_SUN)
			{
				SET_BATTLER_TYPE(bank, TYPE_FIRE);
				formChange = CASTFORM_TO_FIRE;
			}
		}
		else if (gBattleWeather & WEATHER_RAIN_ANY && AffectedByRain(bank))
		{
			if (gBattleMonForms[bank] != CASTFORM_RAIN)
			{
				SET_BATTLER_TYPE(bank, TYPE_WATER);
				formChange = CASTFORM_TO_WATER;
			}
		}
		else if (gBattleWeather & WEATHER_HAIL_ANY)
		{
			if (gBattleMonForms[bank] != CASTFORM_HAIL)
			{
				SET_BATTLER_TYPE(bank, TYPE_ICE);
				formChange = CASTFORM_TO_ICE;
			}
		}
		else if (gBattleMonForms[bank] != CASTFORM_NORMAL)
		{
			SET_BATTLER_TYPE(bank, TYPE_NORMAL);
			formChange = CASTFORM_TO_NORMAL;
		}
	}
	#endif

	return formChange;
}

void atkE8_settypebasedhalvers(void) { //water/mud sport
	bool8 worked = FALSE;

	if (gBattleMoves[gCurrentMove].effect == EFFECT_MUD_SPORT)
	{
		if (!IsMudSportActive())
		{
			gNewBS->MudSportTimer = 5;
			gBattleCommunication[MULTISTRING_CHOOSER] = 0;
			worked = TRUE;
		}
	}
	else //Water Sport
	{
		if (!IsWaterSportActive())
		{
			gNewBS->WaterSportTimer = 5;
			gBattleCommunication[MULTISTRING_CHOOSER] = 1;
			worked = TRUE;
		}
	}

	if (worked)
		gBattlescriptCurrInstr += 5;
	else
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
}

void atkEA_tryrecycleitem(void)
{
	if (SAVED_CONSUMED_ITEMS(gBankAttacker) != ITEM_NONE && ITEM(gBankAttacker) == ITEM_NONE)
	{
		RecycleItem(gBankAttacker);
		gBattlescriptCurrInstr += 5;
	}
	else
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
}

void RecycleItem(u8 bank)
{
	gActiveBattler = bank;
	gLastUsedItem = SAVED_CONSUMED_ITEMS(gActiveBattler);

	SAVED_CONSUMED_ITEMS(gActiveBattler) = ITEM_NONE;
	CONSUMED_ITEMS(gActiveBattler) = ITEM_NONE;	//Remove the temporary item storage
	RemoveBankFromPickupStack(gActiveBattler);

	gBattleMons[gActiveBattler].item = gLastUsedItem;
	HandleUnburdenBoost(gActiveBattler); //Remove the Unburden Boost

	EmitSetMonData(0, REQUEST_HELDITEM_BATTLE, 0, 2, &gBattleMons[gActiveBattler].item);
	MarkBufferBankForExecution(gActiveBattler);
}

void atkEB_settypetoterrain(void) {
	u8 type = GetCamouflageType();

	if (!IsOfType(gBankAttacker, type))
	{
		gBattleMons[gBankAttacker].type1 = type;
		gBattleMons[gBankAttacker].type2 = type;
		gBattleMons[gBankAttacker].type3 = TYPE_BLANK;
		PREPARE_TYPE_BUFFER(gBattleTextBuff1, type);
		gBattlescriptCurrInstr += 5;
	}
	else
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
}

u8 GetCamouflageType(void)
{
	u8 type;

	switch (gTerrainType) {
		case ELECTRIC_TERRAIN:
			type = gTerrainTable[0].camouflageType;
			break;

		case GRASSY_TERRAIN:
			type = gTerrainTable[1].camouflageType;
			break;

		case MISTY_TERRAIN:
			type = gTerrainTable[2].camouflageType;
			break;

		case PSYCHIC_TERRAIN:
			type = gTerrainTable[3].camouflageType;
			break;

		default:
			if (IsTerrainMoveIndoors())
				type = gTerrainTable[BATTLE_TERRAIN_INSIDE + 4].camouflageType;
			else
				type = gTerrainTable[GetBattleTerrainOverride() + 4].camouflageType;
	}

	return type;
}

void atkEE_removelightscreenreflect(void) { //Brick Break
	u8 side = SIDE(gBankTarget);
	if (gSideTimers[side].reflectTimer
	||  gSideTimers[side].lightscreenTimer
	||  gSideStatuses[side] & SIDE_STATUS_REFLECT
	||  gSideStatuses[side] & SIDE_STATUS_LIGHTSCREEN
	||  gNewBS->AuroraVeilTimers[side])
	{
		//Now handled as a move effect
		/*gSideStatuses[side] &= ~(SIDE_STATUS_REFLECT);
		gSideStatuses[side] &= ~(SIDE_STATUS_LIGHTSCREEN);
		gSideTimers[side].reflectTimer = 0;
		gSideTimers[side].lightscreenTimer = 0;
		gNewBS->AuroraVeilTimers[side] = 0;*/
		gBattleScripting.animTurn = 1;
		gBattleScripting.animTargetsHit = 1;
	}
	else {
		gBattleScripting.animTurn = 0;
		gBattleScripting.animTargetsHit = 0;
	}
	gBattlescriptCurrInstr++;
}
