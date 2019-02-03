#include "defines.h"
#include "helper_functions.h"

extern s8 PriorityCalc(u8 bank, u8 action, u16 move);
extern u8 TypeCalc(move_t, u8 bankAtk, u8 bankDef, pokemon_t* party_data_atk, bool8 CheckParty);

extern move_t StatChangeIgnoreTable[];
extern move_t MinimizeHitTable[];
extern const struct StatFractions gAccuracyStageRatios[];

void atk01_accuracycheck(void);
bool8 JumpIfMoveAffectedByProtect(move_t, bank_t, bank_t);
bool8 ProtectAffects(move_t, bank_t, bank_t, u8 set);
bool8 AccuracyCalcHelper(move_t);
u32 AccuracyCalc(move_t, bank_t, bank_t);
u32 AccuracyCalcNoTarget(u16 move, u8 bankAtk);

/*Other Necessary Functions:
JumpIfMoveFailed
*/

void atk01_accuracycheck(void) {
    u16 move = T2_READ_16(gBattlescriptCurrInstr + 5);

	if (gBattleTypeFlags & BATTLE_TYPE_OAK_TUTORIAL) {
		if (!sub_80EB2E0(1) && gBattleMoves[move].power != 0 && SPLIT(move) != SPLIT_STATUS) 
		{
			if (SIDE(gBankAttacker) == B_SIDE_PLAYER) {
				JumpIfMoveFailed(7, move);
				return;
			}
		}
		
		if (!sub_80EB2E0(2) && (gBattleMoves[move].power == 0 || SPLIT(move) == SPLIT_STATUS))
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
        if (gStatuses3[gBankTarget] & STATUS3_ALWAYS_HITS && move == 0xFFFF && gDisableStructs[gBankTarget].bankWithSureHit == gBankAttacker)
            gBattlescriptCurrInstr += 7;
        else if (gStatuses3[gBankTarget] & STATUS3_SEMI_INVULNERABLE && ABILITY(gBankAttacker) != ABILITY_NOGUARD && ABILITY(gBankTarget) != ABILITY_NOGUARD)
            gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
        else if (!JumpIfMoveAffectedByProtect(move, gBankAttacker, gBankTarget))
            gBattlescriptCurrInstr += 7;
    }
	
    else
    {
		if (ParentalBondOn == 1) //No acc checks for second hit of Parental Bond
			gBattlescriptCurrInstr += 7;
			
		else {
			if (move == 0)	//If move isn't 0 its either Future Sight or Doom Desire
				move = gCurrentMove;
				
			if (!JumpIfMoveAffectedByProtect(move, gBankAttacker, gBankTarget) && !AccuracyCalcHelper(move)) {
				
				u32 calc = AccuracyCalc(move, gBankAttacker, gBankTarget);
				
				MicleBerryBits &= ~(1 << gBankAttacker); //Clear Micle Berry bit
				
				gStringBank = gBankTarget;
				 
				// final calculation
				if (umodsi(Random(), 100 + 1) > calc)
				{
					gMoveResultFlags |= MOVE_RESULT_MISSED;
					if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE &&
						(gBattleMoves[move].target == 0x8 || gBattleMoves[move].target == 0x20))
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
	if (ZMoveData->active && SPLIT(move) != SPLIT_STATUS)
		return FALSE;
	
    bool8 affected = ProtectAffects(move, bankAtk, bankDef, TRUE);
	
    if (affected) {
        gMoveResultFlags |= MOVE_RESULT_MISSED;
        JumpIfMoveFailed(7, move);
        gBattleCommunication[6] = 1;
    }
    return affected;
}

bool8 ProtectAffects(u16 move, u8 bankAtk, u8 bankDef, bool8 set) {
    u8 effect = 0;
    u8 protect_flag = gBattleMoves[move].flags & FLAG_PROTECT_AFFECTED;
    u8 split = SPLIT(move);
    u8 contact = CheckContact(move, bankAtk);
    u8 target = gBattleMoves[move].target;
    u8 defSide = GetBattlerSide(bankDef);
	
    if (gProtectStructs[bankDef].protected && protect_flag)
        effect = 1;
	
    else if (gProtectStructs[bankDef].KingsShield && protect_flag && split != SPLIT_STATUS) {
        effect = 1;
        if (contact && set)
            gProtectStructs[bankAtk].kingsshield_damage = 1;
    }
	
    else if (gProtectStructs[bankDef].SpikyShield && protect_flag) {
        effect = 1;
        if (contact && set)
            gProtectStructs[bankAtk].spikyshield_damage = 1;
    }
	
    else if (gProtectStructs[bankDef].BanefulBunker && protect_flag) {
        effect = 1;
        if (contact && set)
            gProtectStructs[bankAtk].banefulbunker_damage = 1;
    }
	
    else if (gSideAffecting[defSide] & SIDE_STATUS_CRAFTY_SHIELD && target != MOVE_TARGET_USER && split == SPLIT_STATUS)
        effect = 1;
    
	else if (gSideAffecting[defSide] & SIDE_STATUS_QUICK_GUARD && protect_flag && PriorityCalc(bankAtk, ACTION_USE_MOVE, move) > 0)
        effect = 1;
    
	else if (gSideAffecting[defSide] & SIDE_STATUS_MAT_BLOCK && protect_flag && split != SPLIT_STATUS)
        effect = 1;
    
	else if (gSideAffecting[defSide] & SIDE_STATUS_WIDE_GUARD && protect_flag && (target == MOVE_TARGET_BOTH || target == MOVE_TARGET_FOES_AND_ALLY))
        effect = 1;
    
	return effect;
}

bool8 AccuracyCalcHelper(u16 move) {
    u8 done_status = FALSE;
	if (ABILITY(gBankAttacker) != ABILITY_NOGUARD 
	&&  ABILITY(gBankTarget) != ABILITY_NOGUARD 
	&&  !((gStatuses3[gBankTarget] & STATUS3_ALWAYS_HITS) && gDisableStructs[gBankTarget].bankWithSureHit == gBankAttacker)) 
	{
		if (((gStatuses3[gBankTarget] & (STATUS3_IN_AIR | STATUS3_SKY_DROP_ATTACKER | STATUS3_SKY_DROP_TARGET)) && !(gHitMarker & HITMARKER_IGNORE_IN_AIR))
        ||  ((gStatuses3[gBankTarget] & STATUS3_UNDERGROUND) && !(gHitMarker & HITMARKER_IGNORE_UNDERGROUND))
        ||  ((gStatuses3[gBankTarget] & STATUS3_UNDERWATER) && !(gHitMarker & HITMARKER_IGNORE_UNDERWATER))
        ||   (gStatuses3[gBankTarget] & STATUS3_DISAPPEARED)) 
		{
				gMoveResultFlags |= MOVESTATUS_MISSED;
				JumpIfMoveFailed(7, move);
				gHitMarker &= 0xFFF8FFFF; //clearing all above hitmarker flags
				return TRUE;
		}
	}
	
	//lock-on/mind reader checked, then no guard, always hiting toxic on poison types, then stomp on a minimized target, then always hitting telekinesis except 0HKO moves, then 0 acc moves
    if (((gStatuses3[gBankTarget] & STATUS3_ALWAYS_HITS) && gDisableStructs[gBankTarget].bankWithSureHit == gBankAttacker)
    ||   (ABILITY(gBankAttacker) == ABILITY_NOGUARD) || (ABILITY(gBankTarget) == ABILITY_NOGUARD)
    ||   (move == MOVE_TOXIC && IsOfType(gBankAttacker, TYPE_POISON))
	||   (CheckTableForMove(move, MinimizeHitTable) && gStatuses3[gBankTarget] & STATUS3_MINIMIZED)
    ||  ((gStatuses3[gBankTarget] & STATUS3_TELEKINESIS) && gBattleMoves[move].effect != EFFECT_0HKO)
	||	 gBattleMoves[move].accuracy == 0) 
	{
		JumpIfMoveFailed(7, move);
		done_status = TRUE;
    }
	
    else if (WEATHER_HAS_EFFECT && (gBattleWeather & WEATHER_RAIN_ANY) && gBattleMoves[move].effect == EFFECT_THUNDER) //Includes Hurricane
	{
		JumpIfMoveFailed(7, move);
		done_status = TRUE;
    }
	
    gHitMarker &= 0xFFF8FFFF; //clearing all above hitmarker flags
    return done_status;
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
		u8 moveSplit = CalcMoveSplit(move, gBankAttacker);
		
		u8 acc;
		if (defAbility == ABILITY_UNAWARE)
			acc = 6;
		else
			acc = gBattleMons[bankAtk].statStages[STAT_STAGE_ACC-1];
			
        if ((gBattleMons[bankDef].status2 & STATUS2_FORESIGHT)
		||  (gBattleMons[bankDef].status2 & STATUS3_MIRACLE_EYED) 
		||   atkAbility == ABILITY_UNAWARE 
		||   atkAbility == ABILITY_KEENEYE 
		||   gBattleMoves[move].effect == EFFECT_IGNORE_STAT_CHANGES)
				buff = acc;
        else
            buff = acc + 6 - gBattleMons[bankDef].statStages[STAT_STAGE_EVASION-1];

        if (buff < 0)
            buff = 0;
        if (buff > 12)
            buff = 12;
		
		moveAcc = gBattleMoves[move].accuracy;
		
        //Check Thunder + Hurricane in sunny weather
        if (WEATHER_HAS_EFFECT && (gBattleWeather & WEATHER_SUN_ANY) && (move == MOVE_THUNDER || move == MOVE_HURRICANE))
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
	
		if (WEATHER_HAS_EFFECT) {
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
		
		if (GravityTimer)
			calc = udivsi((calc * 5), 3); // 5/3 Gravity boost
		
        if (defEffect == ITEM_EFFECT_EVASION_UP)
            calc = udivsi((calc * (100 - defQuality)), 100); // 0.9 Bright Powder/Lax Incense loss
			
		if (MicleBerryBits & (1 << bankAtk))
			calc = udivsi(calc * 120, 100); // 1.2 Micle Berry Boost
			
	return calc;
}

u32 AccuracyCalcNoTarget(u16 move, u8 bankAtk) {
        u8 moveAcc;
		u8 acc;
        u32 calc;
		u8 atkEffect  = ITEM_EFFECT(bankAtk);
		u8 atkQuality = ITEM_QUALITY(gBattleMons[bankAtk].item);
		u8 atkAbility = ABILITY(bankAtk);
		u8 moveSplit = SPLIT(move);
		
		acc = gBattleMons[bankAtk].statStages[STAT_STAGE_ACC-1];			
		moveAcc = gBattleMoves[move].accuracy;
		
        //Check Thunder + Hurricane in sunny weather
        if (WEATHER_HAS_EFFECT && (gBattleWeather & WEATHER_SUN_ANY) && (move == MOVE_THUNDER || move == MOVE_HURRICANE))
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
		
		if (GravityTimer)
			calc = udivsi((calc * 5), 3); // 5/3 Gravity boost
		
		if (MicleBerryBits & (1 << bankAtk))
			calc = udivsi(calc * 120, 100); // 1.2 Micle Berry Boost
			
	return calc;
}
