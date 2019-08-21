#include "defines.h"
#include "defines_battle.h"
#include "../include/random.h"

#include "../include/new/accuracy_calc.h"
#include "../include/new/battle_start_turn_start.h"
#include "../include/new/damage_calc.h"
#include "../include/new/Helper_Functions.h"
#include "../include/new/move_tables.h"
#include "../include/new/move_battle_scripts.h"

extern const struct StatFractions gAccuracyStageRatios[];

//This file's functions:
static bool8 AccuracyCalcHelper(move_t);

void atk01_accuracycheck(void) {
    u16 move = T2_READ_16(gBattlescriptCurrInstr + 5);

	if (gBattleTypeFlags & BATTLE_TYPE_OAK_TUTORIAL) {
		if (!sub_80EB2E0(1) && SPLIT(move) != SPLIT_STATUS) 
		{
			if (SIDE(gBankAttacker) == B_SIDE_PLAYER) {
				JumpIfMoveFailed(7, move);
				return;
			}
		}
		
		if (!sub_80EB2E0(2) && SPLIT(move) == SPLIT_STATUS)
		{
			if (SIDE(gBankAttacker) == B_SIDE_PLAYER) {
				JumpIfMoveFailed(7, move);
				return;
			}
		}
	}
	
	if (gBattleTypeFlags & BATTLE_TYPE_POKE_DUDE) 
	{
		JumpIfMoveFailed(7, move);
		return;
	}

    if (move == 0xFFFE || move == 0xFFFF) {
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
        else if (!JumpIfMoveAffectedByProtect(move, gBankAttacker, gBankTarget))
		{
            gBattlescriptCurrInstr += 7;
		}
    }
	
    else
    {
		if (gNewBS->ParentalBondOn == 1 
		||  (gNewBS->MultiHitOn && (move != MOVE_TRIPLEKICK 
						 || ABILITY(gBankAttacker) == ABILITY_SKILLLINK)))
		{//No acc checks for second hit of Parental Bond or multi hit moves
			gBattlescriptCurrInstr += 7;
		}
			
		else 
		{
			if (move == 0)	//If move isn't 0 its either Future Sight or Doom Desire
				move = gCurrentMove;
				
			if (!JumpIfMoveAffectedByProtect(move, gBankAttacker, gBankTarget) 
			&& !AccuracyCalcHelper(move)) 
			{	
				u32 calc = AccuracyCalc(move, gBankAttacker, gBankTarget);
				
				gNewBS->MicleBerryBits &= ~gBitTable[gBankAttacker]; //Clear Micle Berry bit
				
				gStringBank = gBankTarget;
				 
				// final calculation
				if (umodsi(Random(), 100 + 1) > calc)
				{
					gMoveResultFlags |= MOVE_RESULT_MISSED;
					if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE &&
						(gBattleMoves[move].target == MOVE_TARGET_BOTH || gBattleMoves[move].target == MOVE_TARGET_FOES_AND_ALLY))
						gBattleCommunication[6] = 2;
					else
						gBattleCommunication[6] = 0;
					gMoveResultFlags |= (TypeCalc(move, gBankAttacker, gBankTarget, GetBankPartyData(gBankAttacker), FALSE) & (MOVE_RESULT_MISSED | MOVE_RESULT_FAILED | MOVE_RESULT_DOESNT_AFFECT_FOE));
				}
				
				JumpIfMoveFailed(7, move);
			}
		}
    }
}

bool8 JumpIfMoveAffectedByProtect(move_t move, bank_t bankAtk, bank_t bankDef) {
	if (gNewBS->ZMoveData->active && SPLIT(move) != SPLIT_STATUS)
		return FALSE;
	
    bool8 affected = ProtectAffects(move, bankAtk, bankDef, TRUE);
	
    if (affected) {
        gMoveResultFlags |= MOVE_RESULT_MISSED;
        JumpIfMoveFailed(7, move);
    }
    return affected;
}

bool8 ProtectAffects(u16 move, u8 bankAtk, u8 bankDef, bool8 set) {
    u8 effect = 0;
    u8 protectFlag = gBattleMoves[move].flags & FLAG_PROTECT_AFFECTED;
    u8 split = SPLIT(move);
    u8 contact = CheckContact(move, bankAtk);
    u8 target = gBattleMoves[move].target;
    u8 defSide = SIDE(bankDef);
	
    if (gProtectStructs[bankDef].protected && protectFlag)
	{
        effect = 1;
		gBattleCommunication[6] = 1;
	}
    else if (gProtectStructs[bankDef].KingsShield && protectFlag && split != SPLIT_STATUS) 
	{
        effect = 1;
        if (contact && set)
		{
            gProtectStructs[bankDef].kingsshield_damage = 1;
			gBattleCommunication[6] = 1;
		}
    }
    else if (gProtectStructs[bankDef].SpikyShield && protectFlag) 
	{
        effect = 1;
        if (contact && set)
		{
            gProtectStructs[bankDef].spikyshield_damage = 1;
			gBattleCommunication[6] = 1;
		}
    }
    else if (gProtectStructs[bankDef].BanefulBunker && protectFlag) 
	{
        effect = 1;
        if (contact && set)
		{
            gProtectStructs[bankDef].banefulbunker_damage = 1;
			gBattleCommunication[6] = 1;
		}
    }
    else if (gSideAffecting[defSide] & SIDE_STATUS_CRAFTY_SHIELD && target != MOVE_TARGET_USER && split == SPLIT_STATUS)
	{
		effect = 1;
		BattleStringLoader = CraftyShieldProtectedString;
		gBattleCommunication[6] = 5;
	}
    else if (gSideAffecting[defSide] & SIDE_STATUS_MAT_BLOCK && protectFlag && split != SPLIT_STATUS)
	{
		effect = 1;
		BattleStringLoader = MatBlockProtectedString;
		gBattleCommunication[6] = 6;
	}
    else if (gSideAffecting[defSide] & SIDE_STATUS_QUICK_GUARD && protectFlag && PriorityCalc(bankAtk, ACTION_USE_MOVE, move) > 0)
	{
		effect = 1;
		BattleStringLoader = QuickGuardProtectedString;
		gBattleCommunication[6] = 7;
	}
	else if (gSideAffecting[defSide] & SIDE_STATUS_WIDE_GUARD && protectFlag && (target == MOVE_TARGET_BOTH || target == MOVE_TARGET_FOES_AND_ALLY))
	{
        effect = 1;
		BattleStringLoader = WideGuardProtectedString;
		gBattleCommunication[6] = 8;
    }
	
	return effect;
}

bool8 DoesProtectionMoveBlockMove(u8 bankAtk, u8 bankDef, u16 atkMove, u16 protectMove)
{
    u8 protectFlag = gBattleMoves[atkMove].flags & FLAG_PROTECT_AFFECTED;
    u8 split = SPLIT(atkMove);
    u8 target = gBattleMoves[atkMove].target;
	
	if (!CheckTableForMove(atkMove, MovesThatLiftProtectTable))
	{
		switch (protectMove) {
			case MOVE_PROTECT:
			case MOVE_SPIKYSHIELD:
			case MOVE_BANEFULBUNKER:
				return protectFlag != 0;
				
			case MOVE_KINGSSHIELD:
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

static bool8 AccuracyCalcHelper(u16 move) {
    u8 doneStatus = FALSE;
	if (ABILITY(gBankAttacker) != ABILITY_NOGUARD 
	&&  ABILITY(gBankTarget)   != ABILITY_NOGUARD 
	&&  !((gStatuses3[gBankTarget] & STATUS3_ALWAYS_HITS) && gDisableStructs[gBankTarget].bankWithSureHit == gBankAttacker)) 
	{
		if (((gStatuses3[gBankTarget] & (STATUS3_IN_AIR | STATUS3_SKY_DROP_ATTACKER | STATUS3_SKY_DROP_TARGET)) && !CheckTableForMove(move, IgnoreAirTable))
        ||  ((gStatuses3[gBankTarget] & STATUS3_UNDERGROUND) && !CheckTableForMove(move, IgnoreUndergoundTable))
        ||  ((gStatuses3[gBankTarget] & STATUS3_UNDERWATER) && !CheckTableForMove(move, IgnoreUnderwaterTable))
        ||   (gStatuses3[gBankTarget] & STATUS3_DISAPPEARED)) 
		{
			gMoveResultFlags |= MOVESTATUS_MISSED;
			JumpIfMoveFailed(7, move);
			gHitMarker &= ~(HITMARKER_IGNORE_IN_AIR | HITMARKER_IGNORE_UNDERGROUND | HITMARKER_IGNORE_UNDERWATER);
			return TRUE;
		}
	}
	
	//lock-on/mind reader checked, 
	//then no guard, 
	//always hiting toxic on poison types, 
	//then stomp on a minimized target, 
	//then always hitting telekinesis except 0HKO moves, 
	//then 0 acc moves
    if (((gStatuses3[gBankTarget] & STATUS3_ALWAYS_HITS) && gDisableStructs[gBankTarget].bankWithSureHit == gBankAttacker)
    ||   (ABILITY(gBankAttacker) == ABILITY_NOGUARD) || (ABILITY(gBankTarget) == ABILITY_NOGUARD)
    ||   (move == MOVE_TOXIC && IsOfType(gBankAttacker, TYPE_POISON))
	||   (CheckTableForMove(move, MinimizeHitTable) && gStatuses3[gBankTarget] & STATUS3_MINIMIZED)
    ||  ((gStatuses3[gBankTarget] & STATUS3_TELEKINESIS) && gBattleMoves[move].effect != EFFECT_0HKO)
	||	 gBattleMoves[move].accuracy == 0) 
	{
		JumpIfMoveFailed(7, move);
		doneStatus = TRUE;
    }
	
    else if (WEATHER_HAS_EFFECT)
	{
		if (((gBattleWeather & WEATHER_RAIN_ANY) && CheckTableForMove(move, AlwaysHitRainTable))
		||  ((gBattleWeather & WEATHER_HAIL_ANY) && move == MOVE_BLIZZARD))
		{
			JumpIfMoveFailed(7, move);
			doneStatus = TRUE;
		}
    }
	
    gHitMarker &= ~(HITMARKER_IGNORE_IN_AIR | HITMARKER_IGNORE_UNDERGROUND | HITMARKER_IGNORE_UNDERWATER);
    return doneStatus;
}

u32 AccuracyCalc(u16 move, u8 bankAtk, u8 bankDef) {
        u8 moveAcc;
        s8 buff;
        u32 calc;
		u8 atkEffect  = ITEM_EFFECT(bankAtk);
		u8 defEffect  = ITEM_EFFECT(bankDef);
		u8 atkQuality = ITEM_QUALITY(bankAtk);
        u8 defQuality = ITEM_QUALITY(bankDef);
		u8 atkAbility = ABILITY(bankAtk);
		u8 defAbility = ABILITY(bankDef);
		u8 moveSplit = CalcMoveSplit(gBankAttacker, move);
		
		u8 acc;
		if (defAbility == ABILITY_UNAWARE)
			acc = 6;
		else
			acc = gBattleMons[bankAtk].statStages[STAT_STAGE_ACC-1];
			
        if ((gBattleMons[bankDef].status2 & STATUS2_FORESIGHT)
		||  (gBattleMons[bankDef].status2 & STATUS3_MIRACLE_EYED) 
		||   atkAbility == ABILITY_UNAWARE 
		||   atkAbility == ABILITY_KEENEYE 
		||   CheckTableForMove(move, StatChangeIgnoreTable))
		{
			buff = acc;
		}
        else
            buff = acc + 6 - gBattleMons[bankDef].statStages[STAT_STAGE_EVASION-1];

        if (buff < 0)
            buff = 0;
        if (buff > 12)
            buff = 12;
		
		moveAcc = gBattleMoves[move].accuracy;
		
        //Check Thunder + Hurricane in sunny weather
        if (WEATHER_HAS_EFFECT && (gBattleWeather & WEATHER_SUN_ANY) && CheckTableForMove(move, AlwaysHitRainTable))
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
		
		if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE && ABILITY(PARTNER(bankAtk)) == ABILITY_VICTORYSTAR)
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
				calc = udivsi((calc * 60), 100); // 0.6 Fog loss
		}
		
		if  (defAbility == ABILITY_TANGLEDFEET && (gBattleMons[bankDef].status2 & STATUS2_CONFUSION))
			calc /= 2; // 0.5 Tangled Feet loss
		
		switch (atkEffect) {
			case ITEM_EFFECT_WIDE_LENS:
				calc = udivsi((calc * (100 + atkQuality)), 100); // 1.1 Wide Lens boost
				break;
			
			case ITEM_EFFECT_ZOOM_LENS:
				if (BankMovedBefore(bankDef, bankAtk))
					calc = udivsi((calc * (100 + atkQuality)), 100); // 1.2 Zoom Lens boost
		}   
		
		if (gNewBS->GravityTimer)
			calc = udivsi((calc * 5), 3); // 5/3 Gravity boost
		
        if (defEffect == ITEM_EFFECT_EVASION_UP)
            calc = udivsi((calc * (100 - defQuality)), 100); // 0.9 Bright Powder/Lax Incense loss
			
		if (gNewBS->MicleBerryBits & gBitTable[bankAtk])
			calc = udivsi(calc * 120, 100); // 1.2 Micle Berry Boost
			
	return calc;
}

u32 AccuracyCalcNoTarget(u16 move, u8 bankAtk) {
        u8 moveAcc;
		u8 acc;
        u32 calc;
		u8 atkEffect  = ITEM_EFFECT(bankAtk);
		u8 atkQuality = ITEM_QUALITY(bankAtk);
		u8 atkAbility = ABILITY(bankAtk);
		u8 moveSplit = SPLIT(move);
		
		acc = gBattleMons[bankAtk].statStages[STAT_STAGE_ACC-1];			
		moveAcc = gBattleMoves[move].accuracy;
		
        //Check Thunder + Hurricane in sunny weather
        if (WEATHER_HAS_EFFECT && (gBattleWeather & WEATHER_SUN_ANY) && CheckTableForMove(move, AlwaysHitRainTable))
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
		
		if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE && ABILITY(PARTNER(bankAtk)) == ABILITY_VICTORYSTAR)
            calc = udivsi((calc * 110), 100); // 1.1 Victory Star partner boost
	
		if (WEATHER_HAS_EFFECT &&  gBattleWeather & WEATHER_FOG_ANY)
			calc = udivsi((calc * 60), 100); // 0.6 Fog loss
		
		if (atkEffect == ITEM_EFFECT_WIDE_LENS)
			calc = udivsi((calc * (100 + atkQuality)), 100); // 1.1 Wide Lens boost
		
		if (gNewBS->GravityTimer)
			calc = udivsi((calc * 5), 3); // 5/3 Gravity boost
		
		if (gNewBS->MicleBerryBits & (1 << bankAtk))
			calc = udivsi(calc * 120, 100); // 1.2 Micle Berry Boost
			
		if (WEATHER_HAS_EFFECT)
		{
			if (((gBattleWeather & WEATHER_RAIN_ANY) && CheckTableForMove(move, AlwaysHitRainTable))
			||  ((gBattleWeather & WEATHER_HAIL_ANY) && move == MOVE_BLIZZARD))
				calc = 0; //No Miss
		}
			
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
        if (AbilityBattleEffects(ABILITYEFFECT_ABSORBING, gBattlerTarget, 0, 0, move))
            return;
    }
    gBattlescriptCurrInstr = BS_ptr;
}
