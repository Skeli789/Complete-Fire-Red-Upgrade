#include "defines.h"
#include "helper_functions.h"

extern u8* gBattleScriptsForMoveEffects[];
#define BattleScript_FlushMessageBox (u8*) 0x81D96A8
#define BattleScript_MoveEnd (u8*) 0x81D694E

extern u8 BattleScript_SapSipperAromatherapy[];
extern u8 BattleScript_DefogAdditionalEffects[];
extern u8 BattleScript_PledgeCombined[];
extern u8 BattleScript_PledgeReady[];
extern u8 BattleScript_PrintCustomString[];

extern u8 PowerTrickString[];
extern u8 PowerSwapString[];
extern u8 GuardSwapString[];
extern u8 SpeedSwapString[];
extern u8 HeartSwapString[];
extern u8 PowerSplitString[];
extern u8 GuardSplitString[];
extern u8 SwampString[];
extern u8 SeaOfFireString[];
extern u8 RainbowString[];
extern u8 TrickRoomEndString[];
extern u8 WonderRoomEndString[];
extern u8 MagicRoomEndString[];
extern u8 GravityEndString[];
extern u8 TrickRoomSetString[];
extern u8 WonderRoomSetString[];
extern u8 MagicRoomSetString[];
extern u8 GravitySetString[];
extern u8 IonDelugeShowerString[];
extern u8 NoMoreAirborneFromGravityString[];
extern u8 Bank0AWasBroughtDownString[];
extern u8 TargetTransformedIntoType[];
extern u8 ThirdTypeAddedString[];
extern u8 FairyLockString[];
extern u8 HappyHourString[];
extern u8 CelebrateString[];
extern u8 HoldHandsString[];
extern u8 TailwindSetString[];
extern u8 LuckyChantSetString[];

extern ability_t MoldBreakerIgnoreAbilities[];
extern move_t CopycatBanTable[];
extern move_t MeFirstBanTable[];
extern move_t MovesThatCallOtherMovesTable[];
extern move_t MovesThatRequireRecharging[];
extern move_t InstructBanList[];

void SetTargetPartner(void);
bool8 CheckCraftyShield(void);
void LiftProtectTarget(void);
void IncreaseNimbleCounter(void);
void FlowerShieldLooper(void);
void CheckIfTypePresent(void);
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
void StrengthSapFunc(void);
void PlayAttackAnimationForExplosion(void);
void CopycatFunc(void);

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
	if (gNewBS->NimbleCounters[gBankAttacker] != 0xFF)
		gNewBS->NimbleCounters[gBankAttacker] += 1;
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
	if (ABILITY(bank) == ABILITY_CHEEKPOUCH && !gNewBS->HealBlockTimers[bank]) { //Berry check should already be done
		gBattleMoveDamage = MathMax(1, udivsi(gBattleMons[bank].maxHP, 3));
		gBattleMoveDamage *= -1;
		FormCounter = TRUE;
	}
	else
		FormCounter = FALSE;
}

void SetUnburdenBoostTarget(void) {
	gNewBS->UnburdenBoosts |= 1 << gBankTarget;
}

void MoldBreakerRemoveAbilitiesOnForceSwitchIn(void) {
	u8 bank;
	if (gNewBS->ForceSwitchHelper == 2)
		bank = gBattleScripting->bank;
	else
		bank = gBankAttacker;

	if (ABILITY(bank) == ABILITY_MOLDBREAKER
	||  ABILITY(bank) == ABILITY_TURBOBLAZE
	||  ABILITY(bank) == ABILITY_TERAVOLT)
	{
		if (CheckTableForAbility(ABILITY(gBankTarget), MoldBreakerIgnoreAbilities)) {
			gNewBS->DisabledMoldBreakerAbilities[gBankTarget] = ABILITY(gBankTarget);
			gBattleMons[gBankTarget].ability = 0;
		}
	}
}


void MoldBreakerRestoreAbilitiesOnForceSwitchIn(void) {
	if (gNewBS->DisabledMoldBreakerAbilities[gBankTarget]) {
		gBattleMons[gBankTarget].ability = gNewBS->DisabledMoldBreakerAbilities[gBankTarget];
		gNewBS->DisabledMoldBreakerAbilities[gBankTarget] = 0;
	}
}

void TrainerSlideOut(void) {
    gActiveBattler = B_POSITION_OPPONENT_LEFT;
    EmitTrainerSlideBack(0);
    MarkBufferBankForExecution(gActiveBattler);
}

void SetAuroraVeil(void) {
    if (!gNewBS->AuroraVeilTimers[SIDE(gBankAttacker)] 
	&& gBattleWeather & WEATHER_HAIL_ANY
	&& WEATHER_HAS_EFFECT)
	{
		if (ITEM_EFFECT(gBankAttacker) == ITEM_EFFECT_LIGHT_CLAY)
			gNewBS->AuroraVeilTimers[SIDE(gBankAttacker)] = 8;
		else
			gNewBS->AuroraVeilTimers[SIDE(gBankAttacker)] = 5;
    }
	else
		gBattlescriptCurrInstr = BattleScript_ButItFailed - 5;
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
	switch (gNewBS->EchoedVoiceCounter) {
		case 0:
			gNewBS->EchoedVoiceCounter = 2;
			gNewBS->EchoedVoiceDamageScale = 0;
			break;
		case 2:
			break;
		default:
			gNewBS->EchoedVoiceCounter = 2;
			gNewBS->EchoedVoiceDamageScale = MathMin(10, gNewBS->EchoedVoiceDamageScale + 1);
	}
}
void TryPartingShotStatDrop(void) {

}

/*
void DefogHelperFunc(void) {
	if (gNewBS->AuroraVeilTimers[SIDE(gBankTarget)]
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
	gNewBS->BeakBlastByte &= ~(gBitTable[gBankAttacker]);
}

void BestowItem(void) {

}

void BelchFunction(void) {
	if (!(gNewBS->BelchCounters & gBitTable[gBattlerPartyIndexes[gBankAttacker]]))
		gBattlescriptCurrInstr = BattleScript_ButItFailed - 5;	
}

void StrengthSapFunc(void) {
	u16 atk = gBattleMons[gBankTarget].attack;
	APPLY_STAT_MOD(atk, &gBattleMons[gBankTarget], atk, STAT_STAGE_ATK);
	atk = MathMax(1, atk);
	
	if (ITEM_EFFECT(gBankAttacker) == ITEM_EFFECT_BIGROOT)
		atk = udivsi(130 * atk, 100);
	
	gBattleMoveDamage = atk * -1;
}

void PlayAttackAnimationForExplosion(void) {
    if (gBattleExecBuffer)
	{
		gBattlescriptCurrInstr -= 5;
        return;
	}

    if ((gHitMarker & HITMARKER_NO_ANIMATIONS))
        return;

    u8 multihit;
	gActiveBattler = gBankAttacker;
	multihit = gMultiHitCounter;

    EmitMoveAnimation(0, gCurrentMove, gBattleScripting->animTurn, gBattleMovePower, gBattleMoveDamage, gBattleMons[gBankAttacker].friendship, &gDisableStructs[gBankAttacker], multihit);
    MarkBufferBankForExecution(gBankAttacker);
}

void CopycatFunc(void) {
	if (gNewBS->LastUsedMove == 0 
	|| gNewBS->LastUsedMove == 0xFFFF
	|| CheckTableForMove(gNewBS->LastUsedMove, CopycatBanTable))
	{
		gBattlescriptCurrInstr = BattleScript_ButItFailed - 5;	
	}
	else
		gRandomMove = gNewBS->LastUsedMove;
}

void SetRoostFunc(void) {
	if (gBattleMons[gBankAttacker].type1 == TYPE_FLYING
	&&  gBattleMons[gBankAttacker].type2 == TYPE_FLYING)
	{
		gNewBS->RoostCounter |= gBitTable[gBankAttacker];
		gBattleMons[gBankAttacker].type1 = TYPE_NORMAL;
		gBattleMons[gBankAttacker].type2 = TYPE_NORMAL;
	}
	else
	{
		if (gBattleMons[gBankAttacker].type1 == TYPE_FLYING)
			gBattleMons[gBankAttacker].type1 = TYPE_ROOSTLESS;

		if (gBattleMons[gBankAttacker].type2 == TYPE_FLYING)
			gBattleMons[gBankAttacker].type2 = TYPE_ROOSTLESS;
	}
	
	if (gBattleMons[gBankAttacker].type3 == TYPE_FLYING)
		gBattleMons[gBankAttacker].type3 = TYPE_ROOSTLESS;
}

void CaptivateFunc(void) {
    struct Pokemon *monAttacker, *monTarget;
    u16 speciesAtk, speciesDef;
    u32 personalityAtk, personalityDef;

    if (SIDE(gBankAttacker) == B_SIDE_PLAYER)
        monAttacker = &gPlayerParty[gBattlerPartyIndexes[gBankAttacker]];
    else
        monAttacker = &gEnemyParty[gBattlerPartyIndexes[gBankAttacker]];

    if (SIDE(gBankTarget) == B_SIDE_PLAYER)
        monTarget = &gPlayerParty[gBattlerPartyIndexes[gBankTarget]];
    else
        monTarget = &gEnemyParty[gBattlerPartyIndexes[gBankTarget]];

    speciesAtk = GetMonData(monAttacker, MON_DATA_SPECIES, 0);
    personalityAtk = GetMonData(monAttacker, MON_DATA_PERSONALITY, 0);

    speciesDef = GetMonData(monTarget, MON_DATA_SPECIES, 0);
    personalityDef = GetMonData(monTarget, MON_DATA_PERSONALITY, 0);

    if (GetGenderFromSpeciesAndPersonality(speciesAtk, personalityAtk) == GetGenderFromSpeciesAndPersonality(speciesDef, personalityDef)
    || GetGenderFromSpeciesAndPersonality(speciesAtk, personalityAtk) == MON_GENDERLESS
    || GetGenderFromSpeciesAndPersonality(speciesDef, personalityDef) == MON_GENDERLESS)
    {
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    }
    else
    {
        gBattlescriptCurrInstr += 5;
    }
}

void MeFirstFunc(void) {
	u16 move = gChosenMovesByBanks[gBankTarget];
		
	if (SPLIT(move) == SPLIT_STATUS
	|| BankMovedBefore(gBankTarget, gBankAttacker)
	|| CheckTableForMove(move, MeFirstBanTable)
	|| CheckTableForMove(move, MovesThatCallOtherMovesTable))
	{
		gBattlescriptCurrInstr = BattleScript_ButItFailed - 5;	
	}
	else
		gRandomMove = move;

}

enum PledgeEffects
{
	Pledge_Swamp = 2,
	Pledge_SeaOfFire,
	Pledge_Rainbow,
};

/*
void LoadPledgeScript(void) {
	int i; 
	int k = 0;

	u16 partnerMove = gChosenMovesByBanks[PARTNER(gBankAttacker)];

	if (gNewBS->PledgeHelper)
	{
		if ((gCurrentMove == MOVE_GRASSPLEDGE && partnerMove == MOVE_WATERPLEDGE)
		||  (partnerMove == MOVE_GRASSPLEDGE && gCurrentMove == MOVE_WATERPLEDGE))
		{
			gNewBS->PledgeHelper = Pledge_Swamp;
		}
		else if ((gCurrentMove == MOVE_GRASSPLEDGE && partnerMove == MOVE_FIREPLEDGE)
			  || (partnerMove == MOVE_GRASSPLEDGE && gCurrentMove == MOVE_FIREPLEDGE))
		{
			gNewBS->PledgeHelper = Pledge_SeaOfFire;
		}
		else if ((gCurrentMove == MOVE_FIREPLEDGE && partnerMove == MOVE_WATERPLEDGE)
			  || (partnerMove == MOVE_FIREPLEDGE && gCurrentMove == MOVE_WATERPLEDGE))
		{
			gNewBS->PledgeHelper = Pledge_Rainbow;
		}
	
		gBattlescriptCurrInstr = BattleScript_PledgeCombined - 5;
	}
	else if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE
	&& gBattleMons[PARTNER(gBankAttacker)].hp
	&& partnerMove != gCurrentMove
	&& gBattleMoves[partnerMove].effect == EFFECT_PLEDGE)
	{
		u8 newTurnOrder[] = {0};
		
		for (i = 0; gBanksByTurnOrder[i] != gBankAttacker; ++i)
		{
			newTurnOrder[k++] = gBanksByTurnOrder[i];
			gBanksByTurnOrder[i] = 0xFF;
		}
		gBanksByTurnOrder[i] = 0xFF;
		
		for (i = 0; gBanksByTurnOrder[i] != PARTNER(gBankAttacker); ++i) ;
		gBanksByTurnOrder[i] = 0xFF;
			
		newTurnOrder[k++] = gBankAttacker;
		newTurnOrder[k++] = PARTNER(gBankAttacker);
		
		for (i = 0; i < 4; ++i)
		{
			if (gBanksByTurnOrder[i] != 0xFF)
				newTurnOrder[k++] = gBanksByTurnOrder[i];
		}
		
		for (i = 0; i < 4; ++i)
			gBanksByTurnOrder[i] = newTurnOrder[i];
			
		gNewBS->PledgeHelper = TRUE;
		gBattlescriptCurrInstr = BattleScript_PledgeReady - 5;
	}
}

void SetPledgeEffect(void) {
	switch (gNewBS->PledgeHelper) {
		case Pledge_Swamp:
			if (!gNewBS->SwampTimers[SIDE(gBankTarget)])
			{
				gNewBS->SwampTimers[SIDE(gBankTarget)] = 5;
				BattleStringLoader = SwampString;
				gBattleScripting->animArg1 = B_ANIM_SWAMP;
			}
			else
				gBattlescriptCurrInstr = (u8*) 0x81D6947 - 5;
			break;
		
		case Pledge_SeaOfFire:
			if (!gNewBS->SeaOfFireTimers[SIDE(gBankTarget)])
			{
				gNewBS->SeaOfFireTimers[SIDE(gBankTarget)] = 5;
				BattleStringLoader = SeaOfFireString;
				gBattleScripting->animArg1 = B_ANIM_SEA_OF_FIRE;
			}
			else
				gBattlescriptCurrInstr = (u8*) 0x81D6947 - 5;
			break;
		
		case Pledge_Rainbow:
			if (!gNewBS->RainbowTimers[SIDE(gBankTarget)])
			{
				gNewBS->RainbowTimers[SIDE(gBankTarget)] = 5;
				BattleStringLoader = RainbowString;
				gBattleScripting->animArg1 = B_ANIM_RAINBOW;
			}
			else
				gBattlescriptCurrInstr = (u8*) 0x81D6947 - 5;
			break;
		
		default:
			gBattlescriptCurrInstr = (u8*) 0x81D6947 - 5;
	}
	
	gNewBS->PledgeHelper = 0;
}*/

void DoFieldEffect(void) {
	switch (gCurrentMove) {
		case MOVE_TRICKROOM:
			if (gNewBS->TrickRoomTimer)
			{
				gNewBS->TrickRoomTimer = 0;
				BattleStringLoader = TrickRoomEndString;
			}
			else
			{
				gNewBS->TrickRoomTimer = 5;
				BattleStringLoader = TrickRoomSetString;
			}
			break;

		case MOVE_WONDERROOM:
			if (gNewBS->WonderRoomTimer)
			{
				gNewBS->WonderRoomTimer = 0;
				BattleStringLoader = WonderRoomEndString;
			}
			else
			{
				gNewBS->WonderRoomTimer = 5;
				BattleStringLoader = WonderRoomSetString;
			}
			break;
	
		case MOVE_MAGICROOM:
			if (gNewBS->MagicRoomTimer)
			{
				gNewBS->MagicRoomTimer = 0;
				BattleStringLoader = MagicRoomEndString;
			}
			else
			{
				gNewBS->MagicRoomTimer = 5;
				BattleStringLoader = MagicRoomSetString;
			}
			break;

		case MOVE_GRAVITY:
			if (gNewBS->GravityTimer)
			{
				gNewBS->GravityTimer = 0;
				BattleStringLoader = GravityEndString;
			}
			else
			{				
				for (int i = 0; i < gBattlersCount; ++i)
				{
					if (!CheckGrounding(i))
						gNewBS->targetsToBringDown |= gBitTable[i];
				}
				
				gNewBS->GravityTimer = 5;
				BattleStringLoader = GravitySetString;
			}
			break;

		case MOVE_IONDELUGE:
			gNewBS->IonDelugeTimer = 1;
			BattleStringLoader = IonDelugeShowerString;
			break;
	}
}

//Continues until no marked mons are left in air
void BringDownMons(void) {
	for (int i = 0; i < gBattlersCount; ++i)
	{
		if (gNewBS->targetsToBringDown & gBitTable[i])
		{
			if (gStatuses3[i] & (STATUS3_IN_AIR | STATUS3_SKY_DROP_ATTACKER))
				gNewBS->NoMoreMovingThisTurn |= gBitTable[i];
			
			gNewBS->targetsToBringDown ^= gBitTable[i];
			gNewBS->TelekinesisTimers[i] = 0;
			gNewBS->MagnetRiseTimers[i] = 0;
			gStatuses3[i] &= ~(STATUS3_IN_AIR 
							 | STATUS3_SKY_DROP_ATTACKER 
							 | STATUS3_SKY_DROP_TARGET
							 | STATUS3_LEVITATING
							 | STATUS3_TELEKINESIS);
			
            gActiveBattler = i;
            EmitSpriteInvisibility(0, FALSE);
            MarkBufferBankForExecution(gActiveBattler);
			
			gBattleScripting->bank = i;
			if (gCurrentMove == MOVE_GRAVITY)
				BattleStringLoader = NoMoreAirborneFromGravityString;
			else
				BattleStringLoader = Bank0AWasBroughtDownString;
			BattleScriptPushCursor();
			gBattlescriptCurrInstr = BattleScript_PrintCustomString - 5;
		}
	}
}

void TryFling(void) {
	if (!CanFling(ABILITY(gBankAttacker), ITEM(gBankAttacker), 
				 GetBankPartyData(gBankAttacker), gBankAttacker, FALSE))
	{
		gBattlescriptCurrInstr = BattleScript_ButItFailed - 5;
	}
	else
		gLastUsedItem = ITEM(gBankAttacker);
}

void CheckTelekinesisFail(void) {
	u16 species = gBattleMons[gBankTarget].species;

	if (gStatuses3[gBankTarget] & (STATUS3_TELEKINESIS | STATUS3_ROOTED | STATUS3_SMACKED_DOWN)
	||  gNewBS->GravityTimer
	||  ITEM_EFFECT(gBankTarget) == ITEM_EFFECT_IRON_BALL
	||  species == PKMN_DIGLETT
	||  species == PKMN_DUGTRIO
	||  species == PKMN_SANDYGAST
	||  species == PKMN_PALOSSAND
	||  species == PKMN_GENGAR_MEGA)
	{
		gBattlescriptCurrInstr = BattleScript_ButItFailed - 5 - 2;
	}
}

void ChangeTargetTypeFunc(void) {
	
	switch (gCurrentMove) {
		case MOVE_SOAK:
			if (ABILITY(gBankTarget) == ABILITY_MULTITYPE
			||  ABILITY(gBankTarget) == ABILITY_RKS_SYSTEM
			|| (gBattleMons[gBankTarget].type1 == TYPE_WATER &&
				gBattleMons[gBankTarget].type2 == TYPE_WATER &&
				gBattleMons[gBankTarget].type3 == TYPE_BLANK))
			{
				gBattlescriptCurrInstr = BattleScript_ButItFailed - 5;
			}
			else
			{
				SET_BATTLER_TYPE(gBankTarget, TYPE_WATER);
				PREPARE_TYPE_BUFFER(gBattleTextBuff1, TYPE_WATER);
				BattleStringLoader = TargetTransformedIntoType;
			}
			break;
		
		case MOVE_TRICKORTREAT:
			if (IsOfType(gBankTarget, TYPE_GHOST))
				gBattlescriptCurrInstr = BattleScript_ButItFailed - 5;
			else
			{
				if (gBattleMons[gBankTarget].type1 == gBattleMons[gBankTarget].type2)
					gBattleMons[gBankTarget].type2 = TYPE_GHOST;
				else
					gBattleMons[gBankTarget].type3 = TYPE_GHOST;
				
				gBattleScripting->bank = gBankTarget;
				PREPARE_TYPE_BUFFER(gBattleTextBuff1, TYPE_GHOST);
				BattleStringLoader = ThirdTypeAddedString;
			}
			break;
			
		case MOVE_FORESTSCURSE:
			if (IsOfType(gBankTarget, TYPE_GRASS))
				gBattlescriptCurrInstr = BattleScript_ButItFailed - 5;
			else
			{
				if (gBattleMons[gBankTarget].type1 == gBattleMons[gBankTarget].type2)
					gBattleMons[gBankTarget].type2 = TYPE_GRASS;
				else
					gBattleMons[gBankTarget].type3 = TYPE_GRASS;
				
				gBattleScripting->bank = gBankTarget;
				PREPARE_TYPE_BUFFER(gBattleTextBuff1, TYPE_GRASS);
				BattleStringLoader = ThirdTypeAddedString;
			}
			break;
	}
}

void HealTargetFunc(void) {
	u16 maxHP = gBattleMons[gBankTarget].maxHP;

	if (BATTLER_MAX_HP(gBankTarget)) {
		gBattlescriptCurrInstr = BattleScript_ButItFailed - 5;
		return;
	}

	gBattleMoveDamage = 0;
	
	switch (gCurrentMove) {
		case MOVE_HEALPULSE:
			if (ABILITY(gBankTarget) == ABILITY_MEGALAUNCHER)
				gBattleMoveDamage = udivsi(maxHP * 75, 100);
			else
				gBattleMoveDamage = maxHP / 2;
			break;
		
		case MOVE_FLORALHEALING:
			if (TerrainType == GRASSY_TERRAIN)
				gBattleMoveDamage = udivsi(maxHP * 2, 3);
			else
				gBattleMoveDamage = maxHP / 2;
			break;
		
		case MOVE_POLLENPUFF:
			gBattleMoveDamage = maxHP / 2;
			break;
	}
	
	gBattleMoveDamage = MathMax(gBattleMoveDamage, 1);
	
	gBattleMoveDamage *= -1;
}

void TopsyTurvyFunc(void) {
	int i;
	
	for (i = 0; i < BATTLE_STATS_NO-1; ++i) 
	{
		if (gBattleMons[gBankTarget].statStages[i] != 6)
			break;
	}
	
	if (i == BATTLE_STATS_NO-1)
		gBattlescriptCurrInstr = BattleScript_ButItFailed - 5;
	else
	{
		for (i = 0; i < BATTLE_STATS_NO-1; ++i) 
			gBattleMons[gBankTarget].statStages[i] = ((gBattleMons[gBankTarget].statStages[i] - 6) * -1) + 6;
	}
}

void DoFairyLockHappyHourFunc(void) {

	switch (gCurrentMove) {
		case MOVE_FAIRYLOCK:
			if (gNewBS->FairyLockTimer)
				gBattlescriptCurrInstr = BattleScript_ButItFailed - 5;
			else
			{
				gNewBS->FairyLockTimer = 2;
				BattleStringLoader = FairyLockString;
			}
			break;
		
		case MOVE_HAPPYHOUR:
			if (!gNewBS->HappyHourByte)
			{
				gNewBS->HappyHourByte = TRUE;
				gBattleStruct->moneyMultiplier *= 2;
			}
			BattleStringLoader = HappyHourString;
			break;
			
		case MOVE_CELEBRATE:
			BattleStringLoader = CelebrateString;
			break;
		
		case MOVE_HOLDHANDS:
			BattleStringLoader = HoldHandsString;
			break;
	}
}

bool8 TargetMovesRightAfterAttacker(void) {
	for (int i = 1; i < gBattlersCount; ++i)
	{
		if (gBanksByTurnOrder[i] == gBankTarget
		&&  gBanksByTurnOrder[i - 1] == gBankAttacker)
			return TRUE;
	}
	return FALSE;
}

void AfterYouFunc(void) {
	int i;
	
	if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE
	&&  BankMovedBefore(gBankAttacker, gBankTarget)
	&&  !TargetMovesRightAfterAttacker())
	{
		int k = 0;
		u8 newTurnOrder[] = {0};
			
		for (i = 0; gBanksByTurnOrder[i] != gBankAttacker; ++i)
		{
			newTurnOrder[k++] = gBanksByTurnOrder[i];
			gBanksByTurnOrder[i] = 0xFF;
		}
		gBanksByTurnOrder[i] = 0xFF;
			
		for (i = 0; gBanksByTurnOrder[i] != gBankTarget; ++i) ;
		gBanksByTurnOrder[i] = 0xFF;
				
			newTurnOrder[k++] = gBankAttacker;
			newTurnOrder[k++] = gBankTarget;
			
		for (i = 0; i < 4; ++i)
		{
			if (gBanksByTurnOrder[i] != 0xFF)
				newTurnOrder[k++] = gBanksByTurnOrder[i];
		}
			
		for (i = 0; i < 4; ++i)
			gBanksByTurnOrder[i] = newTurnOrder[i];
	}
	else
		gBattlescriptCurrInstr = BattleScript_ButItFailed - 5;
}

void QuashFunc(void) {
	int i;
	
	if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE
	&&  BankMovedBefore(gBankAttacker, gBankTarget))
	{
		int k = 0;
		u8 newTurnOrder[] = {0};
		
		for (i = 0; i < gBattlersCount; ++i)
		{
			if (gBanksByTurnOrder[i] != gBankTarget)
				newTurnOrder[k++] = gBanksByTurnOrder[i];
		}
		
		newTurnOrder[k] = gBankTarget;
		
		for (i = 0; i < 4; ++i)
			gBanksByTurnOrder[i] = newTurnOrder[i];
	}
	else
		gBattlescriptCurrInstr = BattleScript_ButItFailed - 5;
}

u8 FindMovePositionInMoveset(u16 move, u8 bank) {
	int i;
	for (i = 0; i < MAX_MON_MOVES; ++i)
	{
		if (gBattleMons[bank].moves[i] == move)
			break;
	}
	return i;
}
			
void TryExecuteInstruct(void) {
	u16 move = gLastPrintedMoves[gBankTarget];
	
	if (CheckTableForMove(move, InstructBanList)
	||  CheckTableForMove(move, MovesThatRequireRecharging)
	||  CheckTableForMove(move, MovesThatCallOtherMovesTable)
	|| (move >= FIRST_Z_MOVE && move <= LAST_Z_MOVE)
	|| (gLockedMoves[gBankTarget] != 0 && gLockedMoves[gBankTarget] != 0xFFFF)
	|| gBattleMons[gBankTarget].status2 & STATUS2_MULTIPLETURNS
	|| FindMovePositionInMoveset(move, gBankTarget) == 4 //No longer knows the move
	|| gBattleMons[gBankTarget].pp[FindMovePositionInMoveset(move, gBankTarget)] == 0
	|| (BankMovedBefore(gBankAttacker, gBankTarget) && (gChosenMovesByBanks[gBankTarget] == MOVE_FOCUSPUNCH
													 || gChosenMovesByBanks[gBankTarget] == MOVE_BEAKBLAST
													 || gChosenMovesByBanks[gBankTarget] == MOVE_SHELLTRAP)))
	{
		gBattlescriptCurrInstr = BattleScript_ButItFailed - 5;
		return;
	}
}

void InitiateInstruct(void) {
	gCurrentMove = gLastPrintedMoves[gBankTarget];
	gCurrMovePos = FindMovePositionInMoveset(gCurrentMove, gBankTarget);
    gChosenMovePos = FindMovePositionInMoveset(gCurrentMove, gBankTarget);
	
	gBankAttacker = gBankTarget;
	
	if (gAbsentBattlerFlags & gBitTable[gNewBS->lastTargeted[gBankAttacker]])
		gBankTarget = GetMoveTarget(gCurrentMove, FALSE);
	else
		gBankTarget = gNewBS->lastTargeted[gBankAttacker];
	
	gLastUsedMoves[gBankAttacker] = gChosenMove;
    gLastResultingMoves[gBankAttacker] = gCurrentMove;
	gLastPrintedMoves[gBankAttacker] = gCurrentMove;
					
	gBattleScripting->animTargetsHit = 0;
	gBattleStruct->atkCancellerTracker = 0;
	MoveValuesCleanUp();
	gHitMarker &= ~(HITMARKER_ATTACKSTRING_PRINTED);
	BattleScriptPush(gBattleScriptsForMoveEffects[gBattleMoves[gCurrentMove].effect]);
	gBattlescriptCurrInstr = BattleScript_FlushMessageBox - 5;
	gNewBS->InstructInProgress = TRUE;
}

void TrySetMagnetRise(void) {

	if (gNewBS->GravityTimer
	|| gStatuses3[gBankAttacker] & (STATUS3_LEVITATING | STATUS3_SMACKED_DOWN | STATUS3_ROOTED)
	|| ITEM_EFFECT(gBankAttacker) == ITEM_EFFECT_IRON_BALL)
		gBattlescriptCurrInstr = BattleScript_ButItFailed - 5;
	else
		gNewBS->MagnetRiseTimers[gBankAttacker] = 5;
}

void TailwindLuckyChantFunc(void) {
	switch (gCurrentMove) {
		case MOVE_TAILWIND:
			if (gNewBS->TailwindTimers[SIDE(gBankAttacker)])
				gBattlescriptCurrInstr = BattleScript_ButItFailed - 5;
			else
			{
				gNewBS->TailwindTimers[SIDE(gBankAttacker)] = 4;
				BattleStringLoader = TailwindSetString;
			}
			break;
		case MOVE_LUCKYCHANT:
			if (gNewBS->LuckyChantTimers[SIDE(gBankAttacker)])
				gBattlescriptCurrInstr = BattleScript_ButItFailed - 5;
			else
			{
				gNewBS->LuckyChantTimers[SIDE(gBankAttacker)] = 4;
				BattleStringLoader = LuckyChantSetString;
			}
			break;
	}
}

void FlameBurstFunc(void) {
	u8 par = PARTNER(gBankTarget);

	if (!(gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
	||  ABILITY(par) == ABILITY_MAGICGUARD
	||  gBattleMons[par].hp == 0)
	{
		gBattlescriptCurrInstr = BattleScript_MoveEnd - 5;
	}
	else
	{
		gBattleScripting->bank = par;
		gBattleMoveDamage = MathMax(1, gBattleMons[par].maxHP / 16);
	}
}

