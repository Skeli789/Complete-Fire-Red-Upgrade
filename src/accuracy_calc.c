#include "defines.h"
#include "defines_battle.h"
#include "../include/random.h"

#include "../include/new/accuracy_calc.h"
#include "../include/new/attackcanceler.h"
#include "../include/new/battle_start_turn_start.h"
#include "../include/new/battle_util.h"
#include "../include/new/damage_calc.h"
#include "../include/new/dynamax.h"
#include "../include/new/util.h"
#include "../include/new/move_tables.h"
#include "../include/new/move_battle_scripts.h"
/*
accuracy_calc.c
	-rewrites how accuracy is calculated, including all relevant abilities, effects, etc.
	-includes protection logic
*/

extern const struct StatFractions gAccuracyStageRatios[];

//This file's functions:
static bool8 AccuracyCalcHelper(move_t, u8 bankDef);
static u32 AccuracyCalcPassDefAbilityItemEffect(u16 move, u8 bankAtk, u8 bankDef, u8 defAbility, u8 defEffect);
static u8 TryAdjustAccuracyForOriginForms(u8 moveAcc, u16 move, u8 bankAtk);

void atk01_accuracycheck(void)
{
	bool8 recalculatedDragonDarts = FALSE;
	u16 move = T2_READ_16(gBattlescriptCurrInstr + 5);

ACCURACY_CHECK_START:
	if (gBattleTypeFlags & BATTLE_TYPE_OAK_TUTORIAL)
	{
		if (!BtlCtrl_OakOldMan_TestState2Flag(1) && SPLIT(move) != SPLIT_STATUS)
		{
			if (SIDE(gBankAttacker) == B_SIDE_PLAYER)
			{
				JumpIfMoveFailed(7, move);
				return;
			}
		}

		if (!BtlCtrl_OakOldMan_TestState2Flag(2) && SPLIT(move) == SPLIT_STATUS)
		{
			if (SIDE(gBankAttacker) == B_SIDE_PLAYER)
			{
				JumpIfMoveFailed(7, move);
				return;
			}
		}
	}
	else if (gBattleTypeFlags & BATTLE_TYPE_POKE_DUDE)
	{
		JumpIfMoveFailed(7, move);
		return;
	}

	if (move == 0xFFFE || move == 0xFFFF)
	{
		if (gStatuses3[gBankTarget] & STATUS3_ALWAYS_HITS
		&& move == 0xFFFF
		&& gDisableStructs[gBankTarget].bankWithSureHit == gBankAttacker)
		{
			gBattlescriptCurrInstr += 7;
		}
		else if (gStatuses3[gBankTarget] & STATUS3_SEMI_INVULNERABLE
			  && ABILITY(gBankAttacker) != ABILITY_NOGUARD
			  && ABILITY(gBankTarget) != ABILITY_NOGUARD)
		{
			gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
		}
		else if (!JumpIfMoveAffectedByProtect(gCurrentMove, gBankAttacker, gBankTarget, TRUE)) //Obviously move shouldn't be used, it's 0xFFFF or 0xFFFE
		{
			gBattlescriptCurrInstr += 7;
		}
	}
	else
	{
		if (move == 0)	//If move isn't 0 its either Future Sight or Doom Desire
			move = gCurrentMove;

		if (gNewBS->ParentalBondOn == 1
		||  (gNewBS->MultiHitOn && (gBattleMoves[move].effect != EFFECT_TRIPLE_KICK
						 || ABILITY(gBankAttacker) == ABILITY_SKILLLINK)))
		{
			//No acc checks for second hit of Parental Bond or multi hit moves
			TrySetDestinyBondToHappen();
			gBattlescriptCurrInstr += 7;
		}
		else
		{
			u8 atkItemEffect = ITEM_EFFECT(gBankAttacker);
			u8 moveTarget = GetBaseMoveTarget(gCurrentMove, gBankAttacker);
			bool8 calcSpreadMove = IS_DOUBLE_BATTLE && moveTarget & (MOVE_TARGET_BOTH | MOVE_TARGET_ALL) && SPLIT(move) != SPLIT_STATUS;
			bool8 clearMicleBerryBits = FALSE;

			for (u32 bankDef = 0; bankDef < gBattlersCount; ++bankDef)
			{
				if (!calcSpreadMove) //Single target
					bankDef = gBankTarget;
				else if (gNewBS->calculatedSpreadMoveAccuracy)
				{
					if (gNewBS->ResultFlags[gBankTarget] & MOVE_RESULT_MISSED)
						gMoveResultFlags = gNewBS->ResultFlags[gBankTarget];
					else
						gMoveResultFlags = 0;
					break; //Already calculated accuracy miss
				}
				else if (!BATTLER_ALIVE(bankDef) || bankDef == gBankAttacker
				|| (bankDef == PARTNER(gBankAttacker) && !(moveTarget & MOVE_TARGET_ALL))
				|| (gNewBS->noResultString[bankDef] && gNewBS->noResultString[bankDef] != 2))
					continue; //Don't bother with this target

				if (!JumpIfMoveAffectedByProtect(move, gBankAttacker, bankDef, FALSE) //Don't jump yet, jump later
				&& !AccuracyCalcHelper(move, bankDef))
				{
					u32 calc = AccuracyCalc(move, gBankAttacker, bankDef);
					clearMicleBerryBits = TRUE;
					
					gStringBank = bankDef;

					// final calculation
					if (Random32() % 101 > calc)
					{
						gNewBS->ResultFlags[bankDef] = MOVE_RESULT_MISSED; //Overwrite old value which can include effectiveness
						gNewBS->missStringId[bankDef] = gBattleCommunication[6] = 2; //STRINGID_PKMNAVOIDEDATTACK

						if (atkItemEffect == ITEM_EFFECT_BLUNDER_POLICY)
							gNewBS->activateBlunderPolicy = TRUE;

						if (gCurrentMove == MOVE_DRAGONDARTS
						&& !recalculatedDragonDarts //So don't jump back and forth between targets
						&& CanTargetPartner(bankDef)
						&& !TargetFullyImmuneToCurrMove(PARTNER(bankDef)))
						{
							//Smart target to partner if miss
							bankDef = PARTNER(bankDef);
							recalculatedDragonDarts = TRUE;
							gNewBS->ResultFlags[bankDef] &= ~MOVE_RESULT_MISSED;
							goto ACCURACY_CHECK_START;
						}
					}
				}

				if (!calcSpreadMove)
					break; //Only one target
			}

			if (gNewBS->ResultFlags[gBankTarget] & MOVE_RESULT_MISSED)
				gMoveResultFlags = MOVE_RESULT_MISSED; //We only care about the miss, not effectiveness
			else if (calcSpreadMove)
				gMoveResultFlags = 0;

			gNewBS->calculatedSpreadMoveAccuracy = TRUE;
			if (clearMicleBerryBits) //Micle Berry was used in some calculation
				gNewBS->MicleBerryBits &= ~gBitTable[gBankAttacker]; //Clear Micle Berry bit

			JumpIfMoveFailed(7, move);
		}
	}
}

bool8 JumpIfMoveAffectedByProtect(u16 move, u8 bankAtk, u8 bankDef, bool8 actuallyJump)
{
	if ((IsAnyMaxMove(move) && !IsDynamaxed(bankDef)) //Otherwise using a Max Move on Max Guard
	|| IsMaxMoveWithEffect(move, MAX_EFFECT_BYPASS_PROTECT)
	|| (gNewBS->zMoveData.active && SPLIT(move) != SPLIT_STATUS))
		return FALSE;

	bool8 affected = ProtectAffects(move, bankAtk, bankDef, TRUE);

	if (affected)
	{
		gMoveResultFlags |= MOVE_RESULT_MISSED;
		gNewBS->ResultFlags[bankDef] = MOVE_RESULT_MISSED;
		
		if (actuallyJump)
			JumpIfMoveFailed(7, move);
	}

	return affected;
}

bool8 ProtectAffects(u16 move, u8 bankAtk, u8 bankDef, bool8 set)
{
	u8 effect = 0;
	u8 protectFlag = gBattleMoves[move].flags & FLAG_PROTECT_AFFECTED;
	u8 split = SPLIT(move);
	u8 contact = CheckContact(move, bankAtk, bankDef);
	u8 target = GetBaseMoveTarget(move, bankAtk);
	u8 defSide = SIDE(bankDef);

	#ifdef ABILITY_UNSEENFIST
	if (protectFlag && IsContactMove(move, bankAtk, bankDef) && ABILITY(bankAtk) == ABILITY_UNSEENFIST) //Uses IsContactMove instead of CheckContact because Protective Pads don't affect this Ability
		protectFlag = FALSE;
	#endif

	if (ProtectedByMaxGuard(bankDef, move))
	{
		effect = 1;
		gNewBS->missStringId[bankDef] = gBattleCommunication[6] = 1;
	}
	if (gProtectStructs[bankDef].protected && protectFlag)
	{
		effect = 1;
		gNewBS->missStringId[bankDef] = gBattleCommunication[6] = 1;
	}
	else if (gProtectStructs[bankDef].KingsShield && protectFlag && split != SPLIT_STATUS)
	{
		effect = 1;
		gNewBS->missStringId[bankDef] = 1;
		if (contact && set)
		{
			gProtectStructs[bankDef].kingsshield_damage = 1;
			gBattleCommunication[6] = 1;
		}
	}
	else if (gProtectStructs[bankDef].SpikyShield && protectFlag)
	{
		effect = 1;
		gNewBS->missStringId[bankDef] = 1;
		if (contact && set)
		{
			gProtectStructs[bankDef].spikyshield_damage = 1;
			gBattleCommunication[6] = 1;
		}
	}
	else if (gProtectStructs[bankDef].BanefulBunker && protectFlag)
	{
		effect = 1;
		gNewBS->missStringId[bankDef] = 1;
		if (contact && set)
		{
			gProtectStructs[bankDef].banefulbunker_damage = 1;
			gBattleCommunication[6] = 1;
		}
	}
	else if (gProtectStructs[bankDef].obstruct && protectFlag)
	{
		effect = 1;
		gNewBS->missStringId[bankDef] = 1;
		if (contact && set)
		{
			gProtectStructs[bankDef].obstructDamage = TRUE;
			gBattleCommunication[6] = 1;
		}
	}
	else if (gSideStatuses[defSide] & SIDE_STATUS_CRAFTY_SHIELD && !(target & (MOVE_TARGET_USER | MOVE_TARGET_OPPONENTS_FIELD)) && split == SPLIT_STATUS)
	{
		effect = 1;
		gBattleStringLoader = CraftyShieldProtectedString;
		gNewBS->missStringId[bankDef] = gBattleCommunication[6] = 5;
	}
	else if (gSideStatuses[defSide] & SIDE_STATUS_MAT_BLOCK && protectFlag && split != SPLIT_STATUS)
	{
		effect = 1;
		gBattleStringLoader = MatBlockProtectedString;
		gNewBS->missStringId[bankDef] = gBattleCommunication[6] = 6;
	}
	else if (gSideStatuses[defSide] & SIDE_STATUS_QUICK_GUARD && protectFlag && PriorityCalc(bankAtk, ACTION_USE_MOVE, move) > 0)
	{
		effect = 1;
		gBattleStringLoader = QuickGuardProtectedString;
		gNewBS->missStringId[bankDef] = gBattleCommunication[6] = 7;
	}
	else if (gSideStatuses[defSide] & SIDE_STATUS_WIDE_GUARD && protectFlag && (target == MOVE_TARGET_BOTH || target == MOVE_TARGET_FOES_AND_ALLY))
	{
		effect = 1;
		gBattleStringLoader = WideGuardProtectedString;
		gNewBS->missStringId[bankDef] = gBattleCommunication[6] = 8;
	}
	else if (split == SPLIT_STATUS
	&& gBankAttacker != bankDef
	&& HasRaidShields(bankDef)
	&& !gSpecialMoveFlags[move].gMovesThatCallOtherMoves) //Moves like Nature Power are still allowed
	{
		effect = 1;
		gBattleStringLoader = gText_RaidShieldProtected;
		gNewBS->missStringId[bankDef] = gBattleCommunication[6] = 9;
	}

	return effect;
}

bool8 DoesProtectionMoveBlockMove(u8 bankAtk, u8 bankDef, u16 atkMove, u16 protectMove)
{
	u8 protectFlag = gBattleMoves[atkMove].flags & FLAG_PROTECT_AFFECTED;
	u8 split = SPLIT(atkMove);
	u8 target = GetBaseMoveTarget(atkMove, bankAtk);

	if (!gSpecialMoveFlags[atkMove].gMovesThatLiftProtectTable
	#ifdef ABILITY_UNSEENFIST
	&& ABILITY(bankAtk) != ABILITY_UNSEENFIST
	#endif
	)
	{
		switch (protectMove) {
			case MOVE_PROTECT:
			case MOVE_SPIKYSHIELD:
			case MOVE_BANEFULBUNKER:
				return protectFlag != 0;

			case MOVE_KINGSSHIELD:
			case MOVE_OBSTRUCT:
				return protectFlag && split != SPLIT_STATUS;

			case MOVE_MATBLOCK:
				return gDisableStructs[bankDef].isFirstTurn && protectFlag && split != SPLIT_STATUS;

			case MOVE_CRAFTYSHIELD:
				return target != MOVE_TARGET_USER && split == SPLIT_STATUS;

			case MOVE_QUICKGUARD:
				return protectFlag && PriorityCalc(bankAtk, ACTION_USE_MOVE, atkMove) > 0;

			case MOVE_WIDEGUARD:
				return protectFlag && target & (MOVE_TARGET_BOTH | MOVE_TARGET_FOES_AND_ALLY);
		}
	}

	return FALSE;
}

bool8 MissesDueToSemiInvulnerability(u8 bankAtk, u8 bankDef, u16 move)
{
	if (!CanHitSemiInvulnerableTarget(bankAtk, bankDef, move))
	{
		if (((gStatuses3[bankDef] & (STATUS3_IN_AIR | STATUS3_SKY_DROP_ATTACKER | STATUS3_SKY_DROP_TARGET)) && !gSpecialMoveFlags[move].gIgnoreInAirMoves)
		||  ((gStatuses3[bankDef] & STATUS3_UNDERGROUND) && !gSpecialMoveFlags[move].gIgnoreUndergoundMoves)
		||  ((gStatuses3[bankDef] & STATUS3_UNDERWATER) && !gSpecialMoveFlags[move].gIgnoreUnderwaterMoves)
		||   (gStatuses3[bankDef] & STATUS3_DISAPPEARED))
		{
			return TRUE;
		}
	}
	
	return FALSE;
}

static bool8 AccuracyCalcHelper(u16 move, u8 bankDef)
{
	u8 doneStatus = FALSE;
	gNewBS->ResultFlags[bankDef] &= ~MOVE_RESULT_NO_EFFECT;
	if (MissesDueToSemiInvulnerability(gBankAttacker, bankDef, move))
	{
		gNewBS->ResultFlags[bankDef] = MOVESTATUS_MISSED;
		//JumpIfMoveFailed(7, move);
		gHitMarker &= ~(HITMARKER_IGNORE_IN_AIR | HITMARKER_IGNORE_UNDERGROUND | HITMARKER_IGNORE_UNDERWATER);
		return TRUE;
	}

	//lock-on/mind reader checked,
	//then no guard,
	//always hiting toxic on poison types,
	//then stomp on a minimized target,
	//then always hitting telekinesis except 0HKO moves,
	//then 0 acc moves
	if (((gStatuses3[bankDef] & STATUS3_ALWAYS_HITS) && gDisableStructs[bankDef].bankWithSureHit == gBankAttacker)
	||   (ABILITY(gBankAttacker) == ABILITY_NOGUARD) || (ABILITY(bankDef) == ABILITY_NOGUARD)
	||   (move == MOVE_TOXIC && IsOfType(gBankAttacker, TYPE_POISON))
	||   (gSpecialMoveFlags[move].gAlwaysHitWhenMinimizedMoves && gStatuses3[bankDef] & STATUS3_MINIMIZED)
	||  ((gStatuses3[bankDef] & STATUS3_TELEKINESIS) && gBattleMoves[move].effect != EFFECT_0HKO)
	||	 gBattleMoves[move].accuracy == 0)
	{
		//JumpIfMoveFailed(7, move);
		doneStatus = TRUE;
	}
	else if (WEATHER_HAS_EFFECT)
	{
		if (((gBattleWeather & WEATHER_RAIN_ANY) && gSpecialMoveFlags[move].gAlwaysHitInRainMoves && AffectedByRain(bankDef))
		||  ((gBattleWeather & WEATHER_HAIL_ANY) && move == MOVE_BLIZZARD))
		{
			//JumpIfMoveFailed(7, move);
			doneStatus = TRUE;
		}
	}

	gHitMarker &= ~(HITMARKER_IGNORE_IN_AIR | HITMARKER_IGNORE_UNDERGROUND | HITMARKER_IGNORE_UNDERWATER);
	return doneStatus;
}

u32 AccuracyCalc(u16 move, u8 bankAtk, u8 bankDef)
{
	return AccuracyCalcPassDefAbilityItemEffect(move, bankAtk, bankDef, ABILITY(bankDef), ITEM_EFFECT(bankDef));
}

static u32 AccuracyCalcPassDefAbilityItemEffect(u16 move, u8 bankAtk, u8 bankDef, u8 defAbility, u8 defEffect)
{
	u8 moveAcc;
	s8 buff;
	u32 calc;
	u8 atkEffect  = ITEM_EFFECT(bankAtk);
	//u8 defEffect  = ITEM_EFFECT(bankDef);
	u8 atkQuality = ITEM_QUALITY(bankAtk);
	u8 defQuality = ITEM_QUALITY(bankDef);
	u8 atkAbility = ABILITY(bankAtk);
	//u8 defAbility = ABILITY(bankDef);
	u8 moveSplit = CalcMoveSplit(move, bankAtk, bankDef);

	u8 acc;
	if (defAbility == ABILITY_UNAWARE)
		acc = 6;
	else
		acc = STAT_STAGE(bankAtk, STAT_STAGE_ACC);

	if (atkAbility == ABILITY_UNAWARE
	|| (gBattleMons[bankDef].status2 & STATUS2_FORESIGHT)
	|| (gBattleMons[bankDef].status2 & STATUS3_MIRACLE_EYED) 
	||  gSpecialMoveFlags[move].gIgnoreStatChangesMoves)
	{
		buff = acc;
	}
	else if (atkAbility == ABILITY_KEENEYE)
	{
		if (STAT_STAGE(bankDef, STAT_STAGE_EVASION) > 6) //Stops higher evasion, allows lower
			buff = acc;
		else
			buff = acc + 6 - STAT_STAGE(bankDef, STAT_STAGE_EVASION);
	}
	else
		buff = acc + 6 - STAT_STAGE(bankDef, STAT_STAGE_EVASION);

	if (buff < STAT_STAGE_MIN)
		buff = STAT_STAGE_MIN;
	if (buff > STAT_STAGE_MAX)
		buff = STAT_STAGE_MAX;

	moveAcc = gBattleMoves[move].accuracy;
	moveAcc = TryAdjustAccuracyForOriginForms(moveAcc, move, bankAtk);

	//Check Thunder + Hurricane in sunny weather
	if (WEATHER_HAS_EFFECT
	&& (gBattleWeather & WEATHER_SUN_ANY)
	&& !ItemEffectIgnoresSunAndRain(defEffect)
	&& gSpecialMoveFlags[move].gAlwaysHitInRainMoves)
		moveAcc = 50;

	//Check Wonder Skin for Status moves
	if (defAbility == ABILITY_WONDERSKIN && moveSplit == SPLIT_STATUS && moveAcc > 50)
		moveAcc = 50;

	calc = gAccuracyStageRatios[buff].dividend * moveAcc;
	calc = udivsi(calc, gAccuracyStageRatios[buff].divisor);

	switch (atkAbility) {
		case ABILITY_COMPOUNDEYES:
			calc = udivsi((calc * 130), 100); // 1.3 Compound Eyes boost
			break;

		case ABILITY_HUSTLE:
			if (moveSplit == SPLIT_PHYSICAL)
				calc = udivsi((calc * 80), 100); // 0.8 Hustle loss;
			break;

		case ABILITY_VICTORYSTAR:
			calc = udivsi((calc * 110), 100); // 1.1 Victory Star boost
	}

	if (IS_DOUBLE_BATTLE && ABILITY(PARTNER(bankAtk)) == ABILITY_VICTORYSTAR)
		calc = udivsi((calc * 110), 100); // 1.1 Victory Star partner boost

	if (WEATHER_HAS_EFFECT)
	{
		switch (defAbility) {
			case ABILITY_SANDVEIL:
				if (gBattleWeather & WEATHER_SANDSTORM_ANY)
					calc = udivsi((calc * 80), 100); // 0.8 Sand Veil loss
				break;

			case ABILITY_SNOWCLOAK:
				if (gBattleWeather & WEATHER_HAIL_ANY)
					calc = udivsi((calc * 80), 100); // 0.8 Snow Cloak loss
		}

		if (gBattleWeather & WEATHER_FOG_ANY)
		{
			if (!BypassesFog(atkAbility, atkEffect))
			{
				#ifdef VAR_GAME_DIFFICULTY
				if (VarGet(VAR_GAME_DIFFICULTY) == OPTIONS_EASY_DIFFICULTY
				&& !FlagGet(FLAG_SYS_GAME_CLEAR)
				&& !(gBattleTypeFlags & BATTLE_TYPE_FRONTIER))
					calc = (calc * 8) / 10; // 0.8 Fog loss
				else
				#endif
					calc = (calc * 6) / 10; // 0.6 Fog loss
			}
		}
	}

	if (defAbility == ABILITY_TANGLEDFEET && IsConfused(bankDef))
		calc /= 2; // 0.5 Tangled Feet loss

	switch (atkEffect) {
		case ITEM_EFFECT_WIDE_LENS:
			calc = udivsi((calc * (100 + atkQuality)), 100); // 1.1 Wide Lens boost
			break;

		case ITEM_EFFECT_ZOOM_LENS:
			if (BankMovedBefore(bankDef, bankAtk))
				calc = udivsi((calc * (100 + atkQuality)), 100); // 1.2 Zoom Lens boost
	}

	if (IsGravityActive())
		calc = udivsi((calc * 5), 3); // 5/3 Gravity boost

	if (defEffect == ITEM_EFFECT_EVASION_UP)
		calc = udivsi((calc * (100 - defQuality)), 100); // 0.9 Bright Powder/Lax Incense loss

	if (gNewBS->MicleBerryBits & gBitTable[bankAtk])
	{
		if (ABILITY(bankAtk) == ABILITY_RIPEN)
			calc = (calc * 14) / 10; // 1.4 Micle Berry Boost
		else
			calc = (calc * 12) / 10; // 1.2 Micle Berry Boost
	}

	if (gBattleMoves[move].accuracy == 0) //Always hit
		calc = 0xFFFF;

	return calc;
}

u32 VisualAccuracyCalc(u16 move, u8 bankAtk, u8 bankDef)
{
	u8 defEffect  = GetRecordedItemEffect(bankDef);
	u8 defAbility = GetRecordedAbility(bankDef);
	u32 acc = AccuracyCalcPassDefAbilityItemEffect(move, bankAtk, bankDef, defAbility, defEffect);

	if (ABILITY(bankAtk) == ABILITY_NOGUARD || defAbility == ABILITY_NOGUARD
	|| (gStatuses3[bankDef] & STATUS3_ALWAYS_HITS && gDisableStructs[bankDef].bankWithSureHit == bankAtk)
	|| (move == MOVE_TOXIC && IsOfType(bankAtk, TYPE_POISON))
	|| (gSpecialMoveFlags[move].gAlwaysHitWhenMinimizedMoves && gStatuses3[bankDef] & STATUS3_MINIMIZED)
	|| ((gStatuses3[bankDef] & STATUS3_TELEKINESIS) && gBattleMoves[move].effect != EFFECT_0HKO))
		acc = 0xFFFF; //No Miss
	else if (WEATHER_HAS_EFFECT)
	{
		if (((gBattleWeather & WEATHER_RAIN_ANY) && gSpecialMoveFlags[move].gAlwaysHitInRainMoves && !ItemEffectIgnoresSunAndRain(defEffect))
		||  ((gBattleWeather & WEATHER_HAIL_ANY) && move == MOVE_BLIZZARD))
			acc = 0xFFFF; //No Miss
	}

	return acc;
}

u32 VisualAccuracyCalc_NoTarget(u16 move, u8 bankAtk)
{
	u8 moveAcc;
	u8 acc;
	u32 calc;
	u8 atkEffect  = ITEM_EFFECT(bankAtk);
	u8 atkQuality = ITEM_QUALITY(bankAtk);
	u8 atkAbility = ABILITY(bankAtk);
	u8 moveSplit = SPLIT(move);

	acc = STAT_STAGE(bankAtk, STAT_STAGE_ACC);
	moveAcc = gBattleMoves[move].accuracy;
	moveAcc = TryAdjustAccuracyForOriginForms(moveAcc, move, bankAtk);

	//Check Thunder + Hurricane in sunny weather
	if (WEATHER_HAS_EFFECT
	&& (gBattleWeather & WEATHER_SUN_ANY)
	&& gSpecialMoveFlags[move].gAlwaysHitInRainMoves)
		moveAcc = 50;

	calc = gAccuracyStageRatios[acc].dividend * moveAcc;
	calc = udivsi(calc, gAccuracyStageRatios[acc].divisor);

	switch (atkAbility) {
		case ABILITY_COMPOUNDEYES:
			calc = udivsi((calc * 130), 100); // 1.3 Compound Eyes boost
			break;

		case ABILITY_HUSTLE:
			if (moveSplit == SPLIT_PHYSICAL)
				calc = udivsi((calc * 80), 100); // 0.8 Hustle loss;
			break;

		case ABILITY_VICTORYSTAR:
			calc = udivsi((calc * 110), 100); // 1.1 Victory Star boost
	}

	if (IS_DOUBLE_BATTLE && ABILITY(PARTNER(bankAtk)) == ABILITY_VICTORYSTAR)
		calc = udivsi((calc * 110), 100); // 1.1 Victory Star partner boost

	if (WEATHER_HAS_EFFECT && gBattleWeather & WEATHER_FOG_ANY)
	{
		if (!BypassesFog(atkAbility, atkEffect))
		{
			#ifdef VAR_GAME_DIFFICULTY
			if (VarGet(VAR_GAME_DIFFICULTY) == OPTIONS_EASY_DIFFICULTY
			&& !FlagGet(FLAG_SYS_GAME_CLEAR)
			&& !(gBattleTypeFlags & BATTLE_TYPE_FRONTIER))
				calc = (calc * 8) / 10; // 0.8 Fog loss
			else
			#endif
				calc = (calc * 6) / 10; // 0.6 Fog loss
		}
	}

	if (atkEffect == ITEM_EFFECT_WIDE_LENS)
		calc = udivsi((calc * (100 + atkQuality)), 100); // 1.1 Wide Lens boost

	if (IsGravityActive())
		calc = udivsi((calc * 5), 3); // 5/3 Gravity boost

	if (gNewBS->MicleBerryBits & (1 << bankAtk))
	{
		if (ABILITY(bankAtk) == ABILITY_RIPEN)
			calc = (calc * 14) / 10; // 1.4 Micle Berry Boost
		else
			calc = (calc * 12) / 10; // 1.2 Micle Berry Boost
	}

	if (atkAbility == ABILITY_NOGUARD
	|| (move == MOVE_TOXIC && IsOfType(bankAtk, TYPE_POISON)))
		calc = 0xFFFF; //No Miss
	else if (WEATHER_HAS_EFFECT)
	{
		if (((gBattleWeather & WEATHER_RAIN_ANY) && gSpecialMoveFlags[move].gAlwaysHitInRainMoves)
		||  ((gBattleWeather & WEATHER_HAIL_ANY) && move == MOVE_BLIZZARD))
			calc = 0xFFFF; //No Miss
	}

	if (gBattleMoves[move].accuracy == 0) //Always hit
		calc = 0xFFFF;

	return calc;
}

void JumpIfMoveFailed(u8 adder, u16 move)
{
	const u8* BS_ptr = gBattlescriptCurrInstr + adder;
	if (gMoveResultFlags & MOVE_RESULT_NO_EFFECT)
	{
		gLastLandedMoves[gBankTarget] = 0;
		gLastHitByType[gBankTarget] = 0;
		BS_ptr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
	}
	else
	{
		TrySetDestinyBondToHappen();
		if (AbilityBattleEffects(ABILITYEFFECT_ABSORBING, gBankTarget, 0, 0, move))
			return;
	}
	gBattlescriptCurrInstr = BS_ptr;
}

static u8 TryAdjustAccuracyForOriginForms(u8 moveAcc, u16 move, u8 bankAtk)
{
	switch (move)
	{
		case MOVE_ROAROFTIME:
			#ifdef SPECIES_DIALGA_ORIGIN
			if (SPECIES(bankAtk) == SPECIES_DIALGA_ORIGIN)
				moveAcc = 75;
			#endif
			break;

		case MOVE_SPACIALREND:
			#ifdef SPECIES_PALKIA_ORIGIN
			if (SPECIES(bankAtk) == SPECIES_PALKIA_ORIGIN)
				moveAcc = 85;
			#endif
			break;	
	}

	return moveAcc;
}
