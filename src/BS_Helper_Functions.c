#include "defines.h"
#include "helper_functions.h"

extern u8 BattleScript_SapSipperAromatherapy[];
extern u8 BattleScript_DefogAdditionalEffects[];

extern u8 PowerTrickString[];
extern u8 PowerSwapString[];
extern u8 GuardSwapString[];
extern u8 SpeedSwapString[];
extern u8 HeartSwapString[];
extern u8 PowerSplitString[];
extern u8 GuardSplitString[];

extern ability_t MoldBreakerIgnoreAbilities[];

void SetTargetPartner(void);
bool8 CheckCraftyShield(void);
void LiftProtectTarget(void);
void IncreaseNimbleCounter(void);
void FlowerShieldLooper(void);
void CheckIfTypePresent(void);
void AcupressureFixTarget(void);
void AcupressureFunc(void);
void SetStatSwapSplit(void);
void ResetTargetStats(void);
void SpectralThiefFunc(void);
void CheeckPouchFunc(void);
void SetUnburdenBoostTarget(void);
void MoldBreakerRemoveAbilitiesOnForceSwitchIn(void);
void MoldBreakerRestoreAbilitiesOnForceSwitchIn(void);
void TrainerSlideOut(void);
void SetAuroraVeil(void);
void MetalBurstDamageCalculator(void);
void TryActivatePartnerSapSipper(void);
void RoundBSFunction(void);
void EchoedVoiceFunc(void);
void TryPartingShotStatDrop(void);
void DefogHelperFunc(void);

void SetTargetPartner(void) {
	gBankTarget = PARTNER(gBankAttacker);
}

bool8 CheckCraftyShield(void) {
	if (gSideAffecting[SIDE(gBankTarget)] & SIDE_STATUS_CRAFTY_SHIELD)
		FormCounter = TRUE;
	else
		FormCounter = FALSE;
		
	return FormCounter;
}

void LiftProtectTarget(void) {
	if (!LiftProtect(gBankTarget))
		gBattlescriptCurrInstr = (u8*) (0x81D6947 - 0x5);
}

void IncreaseNimbleCounter(void) {
	if (NimbleCounters[gBankAttacker] != 0xFF)
		NimbleCounters[gBankAttacker] += 1;
}

void FlowerShieldLooper(void) {

	for (; *SeedHelper < gBattlersCount; ++*SeedHelper) {
		u8 bank = gActionsByTurnOrder[*SeedHelper];
		if (IsOfType(bank, TYPE_GRASS) 
		&& !(gStatuses3[bank] & STATUS3_SEMI_INVULNERABLE)
		&& !(gBattleMons[bank].status2 & STATUS2_SUBSTITUTE && gBankAttacker != gBankTarget))
		{
			gBankTarget = bank;
			FormCounter = 0;
		}
	}
	FormCounter = 0xFF;
}

void CheckIfTypePresent(void) {
	u8 type = FormCounter;
	FormCounter = FALSE;
	
	for (int i = 0; i < gBattlersCount; ++i) {
		if (IsOfType(i, type))
			FormCounter = TRUE;
	}
}

void AcupressureFixTarget(void) {
	if (SIDE(gBankTarget) != SIDE(gBankAttacker))
		gBankTarget = gBankAttacker;
}

void AcupressureFunc(void) {
	if (gBankAttacker != gBankTarget) {
		if ((gBattleMons[gBankTarget].status2 & STATUS2_SUBSTITUTE && ABILITY(gBankAttacker) != ABILITY_INFILTRATOR)
		|| CheckCraftyShield()) {
			gBattlescriptCurrInstr = BattleScript_ButItFailed - 5;
			return;
		}
	}
	
	if (StatsMaxed(gBankTarget) && ABILITY(gBankTarget) != ABILITY_CONTRARY) {
		gBattlescriptCurrInstr = BattleScript_ButItFailed - 5;
		return;
	}
	
	else if (StatsMinned(gBankTarget) && ABILITY(gBankTarget) == ABILITY_CONTRARY) {
		gBattlescriptCurrInstr = BattleScript_ButItFailed - 5;
		return;
	}
	
	u8 stat;
	do {
		stat = umodsi(Random(), BATTLE_STATS_NO-1) + 1;			
	} while (gBattleMons[gActiveBattler].statStages[stat - 1] == 12);
	
	SET_STATCHANGER(stat, 2, FALSE);
}

void SetStatSwapSplit(void) {
	u8 bankAtk = gBankAttacker;
	u8 bankDef = gBankTarget;
	u32 temp, i;
	
	switch (gCurrentMove) {
		case MOVE_POWERTRICK:
			temp = gBattleMons[bankAtk].attack;
			gBattleMons[bankAtk].attack = gBattleMons[bankAtk].defense;
			gBattleMons[bankAtk].defense = temp;
			
			gStatuses3[bankAtk] ^= STATUS3_POWER_TRICK;
				
			BattleStringLoader = PowerTrickString;
			break;
		
		case MOVE_POWERSWAP:	;
			u8 atkAtkBuff = gBattleMons[bankAtk].statStages[STAT_STAGE_ATK-1];
			u8 atkSpAtkBuff = gBattleMons[bankAtk].statStages[STAT_STAGE_SPATK-1];
			gBattleMons[bankAtk].statStages[STAT_STAGE_ATK-1] = gBattleMons[bankDef].statStages[STAT_STAGE_ATK-1];
			gBattleMons[bankAtk].statStages[STAT_STAGE_SPATK-1] = gBattleMons[bankDef].statStages[STAT_STAGE_SPATK-1];
			gBattleMons[bankDef].statStages[STAT_STAGE_ATK-1] = atkAtkBuff;
			gBattleMons[bankDef].statStages[STAT_STAGE_SPATK-1] = atkSpAtkBuff;
			
			BattleStringLoader = PowerSwapString;
			break;
		
		case MOVE_GUARDSWAP:	;
			u8 atkDefBuff = gBattleMons[bankAtk].statStages[STAT_STAGE_DEF-1];
			u8 atkSpDefBuff = gBattleMons[bankAtk].statStages[STAT_STAGE_SPDEF-1];
			gBattleMons[bankAtk].statStages[STAT_STAGE_DEF-1] = gBattleMons[bankDef].statStages[STAT_STAGE_DEF-1];
			gBattleMons[bankAtk].statStages[STAT_STAGE_SPDEF-1] = gBattleMons[bankDef].statStages[STAT_STAGE_SPDEF-1];
			gBattleMons[bankDef].statStages[STAT_STAGE_DEF-1] = atkDefBuff;
			gBattleMons[bankDef].statStages[STAT_STAGE_SPDEF-1] = atkSpDefBuff;
			
			BattleStringLoader = GuardSwapString;
			break;
			
		case MOVE_SPEEDSWAP:
			temp = gBattleMons[bankAtk].speed;
			gBattleMons[bankAtk].speed = gBattleMons[bankDef].speed;
			gBattleMons[bankDef].speed = temp;
			
			BattleStringLoader = SpeedSwapString;
			break;
			
		case MOVE_HEARTSWAP:
			for (i = 0; i < BATTLE_STATS_NO-1; ++i) {
				temp = gBattleMons[bankAtk].statStages[i];
				gBattleMons[bankAtk].statStages[i] = gBattleMons[bankDef].statStages[i];
				gBattleMons[bankDef].statStages[i] = temp;
			}
			
			BattleStringLoader = HeartSwapString;
			break;
		
		case MOVE_POWERSPLIT:	;
			u16 newAtk = (gBattleMons[bankAtk].attack + gBattleMons[bankDef].attack) / 2;
			u16 newSpAtk = (gBattleMons[bankAtk].spAttack + gBattleMons[bankDef].spAttack) / 2;
			
			gBattleMons[bankAtk].attack = MathMax(1, newAtk);
			gBattleMons[bankAtk].spAttack = MathMax(1, newSpAtk);
			gBattleMons[bankDef].attack = MathMax(1, newAtk);
			gBattleMons[bankDef].spAttack = MathMax(1, newSpAtk);
			
			BattleStringLoader = PowerSplitString;
			break;
		
		case MOVE_GUARDSPLIT:	;
			u16 newDef = (gBattleMons[bankAtk].defense + gBattleMons[bankDef].defense) / 2;
			u16 newSpDef = (gBattleMons[bankAtk].spDefense + gBattleMons[bankDef].spDefense) / 2;
			
			gBattleMons[bankAtk].defense = MathMax(1, newDef);
			gBattleMons[bankAtk].spDefense = MathMax(1, newSpDef);
			gBattleMons[bankDef].defense = MathMax(1, newDef);
			gBattleMons[bankDef].spDefense = MathMax(1, newSpDef);
			
			BattleStringLoader = GuardSplitString;
	}
}

void ResetTargetStats(void) {
	for (int i = 0; i < BATTLE_STATS_NO-1; ++i)
		gBattleMons[gBankTarget].statStages[i] = 6;
}

void SpectralThiefFunc(void) {
	s8 increment = 1;
	bool8 success = FALSE;
	u8 atkAbility = ABILITY(gBankAttacker);
	
	for (int i = 0; i < BATTLE_STATS_NO-1; ++i) {
		switch (atkAbility) {
			case ABILITY_SIMPLE:
				increment = 2;
				break;
			
			case ABILITY_CONTRARY:
				increment = -1;
		}
		
		if (atkAbility == ABILITY_CONTRARY) {
			while (gBattleMons[gBankTarget].statStages[i] > 6 && gBattleMons[gBankAttacker].statStages[i] > 0) {
				success = TRUE;
				gBattleMons[gBankTarget].statStages[i] -= 1;
				gBattleMons[gBankAttacker].statStages[i] += increment; //gBattleMons[gBankAttacker].statStages[i] -= 1;
			}
		}
		else {
			while (gBattleMons[gBankTarget].statStages[i] > 6 && gBattleMons[gBankAttacker].statStages[i] < 12) {
				success = TRUE;
				gBattleMons[gBankTarget].statStages[i] -= 1;
				gBattleMons[gBankAttacker].statStages[i] += increment;
				
				if (gBattleMons[gBankAttacker].statStages[i] > 12)
					gBattleMons[gBankAttacker].statStages[i] = 12;
			}
		}
	}
	
	FormCounter = success;
}

void CheeckPouchFunc(void) {
	u8 bank = gBattleScripting->bank;
	if (ABILITY(bank) == ABILITY_CHEEKPOUCH && !HealBlockTimers[bank]) { //Berry check should already be done
		gBattleMoveDamage = MathMax(1, udivsi(gBattleMons[bank].maxHP, 3));
		gBattleMoveDamage *= -1;
		FormCounter = TRUE;
	}
	else
		FormCounter = FALSE;
}

void SetUnburdenBoostTarget(void) {
	UnburdenBoosts |= 1 << gBankTarget;
}

void MoldBreakerRemoveAbilitiesOnForceSwitchIn(void) {
	u8 bank;
	if (ForceSwitchHelper == 2)
		bank = gBattleScripting->bank;
	else
		bank = gBankAttacker;

	if (ABILITY(bank) == ABILITY_MOLDBREAKER
	||  ABILITY(bank) == ABILITY_TURBOBLAZE
	||  ABILITY(bank) == ABILITY_TERAVOLT)
	{
		if (CheckTableForAbility(ABILITY(gBankTarget), MoldBreakerIgnoreAbilities)) {
			DisabledMoldBreakerAbilities[gBankTarget] = ABILITY(gBankTarget);
			gBattleMons[gBankTarget].ability = 0;
		}
	}
}


void MoldBreakerRestoreAbilitiesOnForceSwitchIn(void) {
	if (DisabledMoldBreakerAbilities[gBankTarget]) {
		gBattleMons[gBankTarget].ability = DisabledMoldBreakerAbilities[gBankTarget];
		DisabledMoldBreakerAbilities[gBankTarget] = 0;
	}
}

void TrainerSlideOut(void) {
    gActiveBattler = B_POSITION_OPPONENT_LEFT;
    EmitTrainerSlideBack(0);
    MarkBufferBankForExecution(gActiveBattler);
}

void SetAuroraVeil(void) {
	FormCounter = FALSE;
	
    if (!AuroraVeilTimers[SIDE(gBankAttacker)] 
	&& gBattleWeather & WEATHER_HAIL_ANY
	&& WEATHER_HAS_EFFECT)
	{
		if (ITEM_EFFECT(gBankAttacker) == ITEM_EFFECT_LIGHT_CLAY)
			AuroraVeilTimers[SIDE(gBankAttacker)] = 8;
		else
			AuroraVeilTimers[SIDE(gBankAttacker)] = 5;
			
		FormCounter = TRUE;
    }
}

void MetalBurstDamageCalculator(void) {
	if (BankMovedBefore(gBankAttacker, gBankTarget))
		gBattlescriptCurrInstr = BattleScript_ButItFailed - 5;

    u8 atkSide = SIDE(gBankAttacker);
    u8 defSide = SIDE(gProtectStructs[gBankAttacker].physicalBank);
	u8 defSide2 = SIDE(gProtectStructs[gBankAttacker].specialBank);
	
    if (gProtectStructs[gBankAttacker].physicalDmg && atkSide != defSide && gBattleMons[gProtectStructs[gBankAttacker].physicalBank].hp) 
	{
		if (!gSpecialStatuses[gBankAttacker].moveturnLostHP)
			gBattleMoveDamage = 1; //Suffered physical damage, but negated damage
		else
			gBattleMoveDamage = udivsi(gProtectStructs[gBankAttacker].physicalDmg * 15, 10);
		
        if (gSideTimers[defSide].followmeTimer && gBattleMons[gSideTimers[defSide].followmeTarget].hp)
            gBankTarget = gSideTimers[defSide].followmeTarget;
        else
            gBankTarget = gProtectStructs[gBankAttacker].physicalBank;
    }
	
	else if (gProtectStructs[gBankAttacker].specialDmg && atkSide != defSide2 && gBattleMons[gProtectStructs[gBankAttacker].specialBank].hp) 
	{
		if (!gSpecialStatuses[gBankAttacker].moveturnLostHP)
			gBattleMoveDamage = 1; //Suffered special damage, but negated damage
		else
			gBattleMoveDamage = udivsi(gProtectStructs[gBankAttacker].specialDmg * 15, 10);
		
        if (gSideTimers[defSide2].followmeTimer && gBattleMons[gSideTimers[defSide2].followmeTarget].hp)
            gBankTarget = gSideTimers[defSide2].followmeTarget;
        else
            gBankTarget = gProtectStructs[gBankAttacker].specialBank;
    }
	
    else {
        gSpecialStatuses[gBankAttacker].flag20 = 1;
        gBattlescriptCurrInstr = BattleScript_ButItFailed - 5;
    }
}

/*
void TryActivatePartnerSapSipper(void) {
	if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE
	&& !(gAbsentBattlerFlags & gBitTable[PARTNER(gBankAttacker)])
	&& ABILITY(PARTNER(gBankAttacker)) == ABILITY_SAPSIPPER
	&& gBattleStruct->dynamicMoveType == TYPE_GRASS
	&& STAT_CAN_RISE(PARTNER(gBankAttacker), STAT_STAGE_ATK))
	{
		gBankTarget = PARTNER(gBankAttacker);
		gBattleScripting->bank = gBankTarget;
        gBattleScripting->statChanger = INCREASE_1 | STAT_STAGE_ATK;
        gBattleScripting->animArg1 = 0xE + STAT_STAGE_ATK;
        gBattleScripting->animArg2 = 0;
		gLastUsedAbility = ABILITY(gBankTarget);
		RecordAbilityBattle(gBankTarget, gLastUsedAbility);
		gBattlescriptCurrInstr += 5;
		BattleScriptPushCursor();
        gBattlescriptCurrInstr = BattleScript_SapSipperAromatherapy - 5;
	}
}*/

void RoundBSFunction(void) {
	int i;
	u8 j = 0;
	u8 k = 0;
	u8 index = 0;
	u8 rounders[3] = {0xFF, 0xFF, 0xFF};
	u8 nonRounders[3] = {0xFF, 0xFF, 0xFF};
	
	if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE) {
		for (i = 0; i < gBattlersCount; ++i) {
			if (gBanksByTurnOrder[i] == gBankAttacker) {
				index = i + 1;
				break;
			}
		}
		
		for (i = index; i < gBattlersCount; ++i) {
			if (gChosenMovesByBanks[i] == MOVE_ROUND)
				rounders[j++] = gChosenMovesByBanks[i];
			else
				nonRounders[k++] = gChosenMovesByBanks[i];
		}
		
		for (i = 0; rounders[i] != 0xFF && i < 3; ++i)
			gBanksByTurnOrder[index++] = rounders[i];

		for (i = 0; nonRounders[i] != 0xFF && i < 3; ++i)
			gBanksByTurnOrder[index++] = nonRounders[i];
		
	}
}

void EchoedVoiceFunc(void) {
	switch (EchoedVoiceCounter) {
		case 0:
			EchoedVoiceCounter = 2;
			EchoedVoiceDamageScale = 0;
			break;
		case 2:
			break;
		default:
			EchoedVoiceCounter = 2;
			EchoedVoiceDamageScale = MathMin(10, EchoedVoiceDamageScale + 1);
	}
}
void TryPartingShotStatDrop(void) {

}

/*
void DefogHelperFunc(void) {
	if (AuroraVeilTimers[SIDE(gBankTarget)]
	|| gSideAffecting[SIDE(gBankAttacker)] & SIDE_STATUS_SPIKES
	|| gSideAffecting[SIDE(gBankTarget)] & (SIDE_STATUS_SPIKES 
										  | SIDE_STATUS_REFLECT
										  | SIDE_STATUS_LIGHTSCREEN
										  | SIDE_STATUS_SAFEGUARD
										  | SIDE_STATUS_MIST))
	{
		gBattlescriptCurrInstr = BattleScript_DefogAdditionalEffects - 5;
	}
}*/

void ClearBeakBlastBit(void) {
	BeakBlastByte &= ~(gBitTable[gBankAttacker]);
}

void BestowItem(void) {

}

void BelchFunction(void) {
	if (!(BelchCounters & gBitTable[gBattlerPartyIndexes[gBankAttacker]]))
		gBattlescriptCurrInstr = BattleScript_ButItFailed - 5;	
}