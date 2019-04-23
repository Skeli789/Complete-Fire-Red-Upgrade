#include "defines.h"
#include "defines_battle.h"
#include "../include/new/helper_functions.h"

extern u8* gBattleScriptsForMoveEffects[];
#define BattleScript_FlushMessageBox (u8*) 0x81D96A8
#define BattleScript_MoveEnd (u8*) 0x81D694E
#define BattleScript_ButItDoesntAffect (u8*) 0x81D7E04

extern u8 BattleScript_DarkVoidFail[];
extern u8 BattleScript_MagneticFluxStatBoost[];
extern u8 BattleScript_GearUpStatBoost[];
extern u8 BattleScript_SapSipperAromatherapy[];
extern u8 BattleScript_DefogAdditionalEffects[];
extern u8 BattleScript_PledgeCombined[];
extern u8 BattleScript_PledgeReady[];
extern u8 BattleScript_PrintCustomString[];
extern u8 BattleScript_SeedStatBoost[];
extern u8 BattleScript_NoTargetMoveFailed[];

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
extern u8 WorrySeedString[];
extern u8 AbilitySuppressedString[];
extern u8 EntrainmentString[];
extern u8 SimpleBeamString[];
extern u8 gText_SetAuroraVeil[];

extern ability_t MoldBreakerIgnoreAbilities[];
extern move_t CopycatBanTable[];
extern move_t MeFirstBanTable[];
extern move_t MovesThatCallOtherMovesTable[];
extern move_t MovesThatRequireRecharging[];
extern move_t InstructBanList[];
extern FlingStruct FlingTable[];

extern ability_t WorrySeedGastroAcidBanTable[];
extern ability_t EntrainmentBanTableAttacker[];
extern ability_t EntrainmentBanTableTarget[];
extern ability_t SimpleBeamBanTable[];

extern u8 ItemBattleEffects(u8 caseID, u8 bank, bool8 moveTurn, bool8 DoPluck);

void SetTargetPartner(void);
bool8 CheckCraftyShield(u8 bank);
void LiftProtectTarget(void);
void IncreaseNimbleCounter(void);
void FlowerShieldLooper(void);
void CheckIfTypePresent(void);
void AcupressureFunc(void);
void SetStatSwapSplit(void);
void ResetTargetStats(void);
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

void CheckIfDarkVoidShouldFail(void)
{
	if (gCurrentMove == MOVE_DARKVOID
	&&  SPECIES(gBankAttacker) != SPECIES_DARKRAI
	&&  !gNewBS->MoveBounceInProgress)
		gBattlescriptCurrInstr = BattleScript_DarkVoidFail - 5;
}


void SetTargetPartner(void) {
	gBankTarget = PARTNER(gBankAttacker);
}

bool8 CheckCraftyShield(u8 bank) {
	if (gSideAffecting[SIDE(bank)] & SIDE_STATUS_CRAFTY_SHIELD)
		return TRUE;
		
	return FALSE;
}

void LiftProtectTarget(void) {
	if (!LiftProtect(gBankTarget))
		gBattlescriptCurrInstr = (u8*) (0x81D6947 - 0x5);
}

void IncreaseNimbleCounter(void) {
	if (gNewBS->NimbleCounters[gBankAttacker] != 0xFF)
		gNewBS->NimbleCounters[gBankAttacker] += 1;
}

void MagnetFluxLooper(void) {
	
	for (; *SeedHelper < gBattlersCount; ++*SeedHelper) {
		u8 bank = gBanksByTurnOrder[*SeedHelper];
		if ((bank == gBankAttacker || bank == PARTNER(gBankAttacker))
		&&  (ABILITY(bank) == ABILITY_PLUS || ABILITY(bank) == ABILITY_MINUS)
		&& gBattleMons[bank].hp
		&& !(gStatuses3[bank] & STATUS3_SEMI_INVULNERABLE)
		&& !(gBattleMons[bank].status2 & STATUS2_SUBSTITUTE))
		{	
			++*SeedHelper;
			gBankTarget = bank;
			if (gCurrentMove == MOVE_MAGNETICFLUX)
				gBattlescriptCurrInstr = BattleScript_MagneticFluxStatBoost - 5;
			else
				gBattlescriptCurrInstr = BattleScript_GearUpStatBoost - 5;
			return;
		}
	}
	
	if (!gBattleScripting->animTargetsHit) //Not a single mon was affected
		gBattlescriptCurrInstr = BattleScript_ButItFailed - 5;
	else
	{
		gBattlescriptCurrInstr = BattleScript_MoveEnd - 5;
		gBankTarget = gBankAttacker;
	}
}

void ModifyGrowthInSun(void) {
	if (WEATHER_HAS_EFFECT && gBattleWeather & WEATHER_SUN_ANY)
		gBattleScripting->statChanger += INCREASE_1;
}

void AcupressureFunc(void) {
	if (gBankAttacker != gBankTarget) {
		if (MoveBlockedBySubstitute(gCurrentMove, gBankAttacker, gBankTarget) || CheckCraftyShield(gBankTarget)) {
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

void ToggleSpectralThiefByte(void) {
	gNewBS->SpectralThiefActive ^= TRUE;
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
	if (ForceSwitchHelper == 2)
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
		
		BattleStringLoader = gText_SetAuroraVeil;
		gBattleCommunication[MULTISTRING_CHOOSER] = 1;
    }
	else
	{
		gMoveResultFlags |= MOVE_RESULT_MISSED;
        gBattleCommunication[MULTISTRING_CHOOSER] = 0;
	}
}

void MetalBurstDamageCalculator(void) {
	if (GetBattlerTurnOrderNum(gBankTarget) > gCurrentTurnActionNumber)
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
        gSpecialStatuses[gBankAttacker].ppNotAffectedByPressure = 1;
        gBattlescriptCurrInstr = BattleScript_ButItFailed - 5;
    }
}


void TryActivatePartnerSapSipper(void) {
	if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE
	&& !(gAbsentBattlerFlags & gBitTable[PARTNER(gBankAttacker)])
	&& ABILITY(PARTNER(gBankAttacker)) == ABILITY_SAPSIPPER
	&& gBattleStruct->dynamicMoveType == TYPE_GRASS
	&& STAT_CAN_RISE(PARTNER(gBankAttacker), STAT_STAGE_ATK))
	{
		gBattleScripting->bank = gBankTarget = PARTNER(gBankAttacker);
        gBattleScripting->statChanger = INCREASE_1 | STAT_STAGE_ATK;
        gBattleScripting->animArg1 = 0xE + STAT_STAGE_ATK;
        gBattleScripting->animArg2 = 0;
		gLastUsedAbility = ABILITY(gBankTarget);
		BattleScriptPush(gBattlescriptCurrInstr + 5);
        gBattlescriptCurrInstr = BattleScript_SapSipperAromatherapy - 5;
	}
}

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
				index = i + 1; //Index after attacker; index to start swapping data
				break;
			}
		}
		
		for (i = index; i < gBattlersCount; ++i) {
			if (gChosenMovesByBanks[gBanksByTurnOrder[i]] == MOVE_ROUND)
				rounders[j++] = gBanksByTurnOrder[i];
			else
				nonRounders[k++] = gBanksByTurnOrder[i];
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
}

void ClearBeakBlastBit(void) {
	if (!(gNewBS->BeakBlastByte & gBitTable[gBankAttacker])) //Forced by Encore
		gBattlescriptCurrInstr = BattleScript_NoTargetMoveFailed;
	else
		gNewBS->BeakBlastByte &= ~(gBitTable[gBankAttacker]);
}

void BestowItem(void) 
{
	if (ITEM(gBankTarget) == 0
	&& ITEM(gBankAttacker) != 0
	&& CanTransferItem(gBattleMons[gBankTarget].species, ITEM(gBankAttacker), GetBankPartyData(gBankTarget))
	&& CanTransferItem(gBattleMons[gBankAttacker].species, ITEM(gBankAttacker), GetBankPartyData(gBankAttacker)))
	{
		gLastUsedItem = gBattleMons[gBankTarget].item = ITEM(gBankAttacker);
		gBattleMons[gBankAttacker].item = 0;
		
		gActiveBattler = gBankAttacker;
		EmitSetMonData(0, REQUEST_HELDITEM_BATTLE, 0, 2, &gBattleMons[gActiveBattler].item);
		MarkBufferBankForExecution(gActiveBattler);
		
		gActiveBattler = gBankTarget;
		EmitSetMonData(0, REQUEST_HELDITEM_BATTLE, 0, 2, &gBattleMons[gActiveBattler].item);
		MarkBufferBankForExecution(gActiveBattler);
	}
	else
		gBattlescriptCurrInstr = BattleScript_ButItFailed - 5;
}

void BelchFunction(void) {
	if (!(gNewBS->BelchCounters & gBitTable[gBattlerPartyIndexes[gBankAttacker]]))
		gBattlescriptCurrInstr = BattleScript_ButItFailed - 5 - 2;	
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
		gBattlescriptCurrInstr = BattleScript_ButItFailed - 1 - 5;	//From PP Reduce
	}
	else
	{
		gHitMarker &= ~(HITMARKER_ATTACKSTRING_PRINTED);
		gRandomMove = gNewBS->LastUsedMove;
	}
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
        gBattlescriptCurrInstr = BattleScript_ButItFailed - 5;
    }
}

void MeFirstFunc(void) {
	u16 move = gChosenMovesByBanks[gBankTarget];
		
	if (SPLIT(move) == SPLIT_STATUS
	|| GetBattlerTurnOrderNum(gBankTarget) < gCurrentTurnActionNumber 
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


void LoadPledgeScript(void) {
	int i; 
	int k = 0;

	u16 partnerMove = gChosenMovesByBanks[PARTNER(gBankAttacker)];

	if (gNewBS->PledgeHelper)
	{
		if ((gCurrentMove == MOVE_GRASSPLEDGE && partnerMove == MOVE_WATERPLEDGE)
		||  (partnerMove == MOVE_GRASSPLEDGE && gCurrentMove == MOVE_WATERPLEDGE))
		{
			gCurrentMove = MOVE_GRASSPLEDGE;
			gNewBS->PledgeHelper = Pledge_Swamp;
		}
		else if ((gCurrentMove == MOVE_GRASSPLEDGE && partnerMove == MOVE_FIREPLEDGE)
			  || (partnerMove == MOVE_GRASSPLEDGE && gCurrentMove == MOVE_FIREPLEDGE))
		{
			gCurrentMove = MOVE_FIREPLEDGE;
			gNewBS->PledgeHelper = Pledge_SeaOfFire;
		}
		else if ((gCurrentMove == MOVE_FIREPLEDGE && partnerMove == MOVE_WATERPLEDGE)
			  || (partnerMove == MOVE_FIREPLEDGE && gCurrentMove == MOVE_WATERPLEDGE))
		{
			gCurrentMove = MOVE_WATERPLEDGE;
			gNewBS->PledgeHelper = Pledge_Rainbow;
		}
	
		gBattlescriptCurrInstr = BattleScript_PledgeCombined - 5;
	}
	else if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE
	&& gBattleMons[PARTNER(gBankAttacker)].hp
	&& partnerMove != gCurrentMove
	&& gBattleMoves[partnerMove].effect == EFFECT_PLEDGE)
	{
		u8 index = 0;
		u8 newTurnOrder[] = {0xFF, 0xFF};
		
		for (i = 0; i < gBattlersCount; ++i) 
		{
			if (gBanksByTurnOrder[i] == gBankAttacker) 
			{
				index = i + 1; //Index after attacker; index to start swapping data
				break;
			}
		}
		
		for (i = index; i < gBattlersCount; ++i) {
			if (gBanksByTurnOrder[i] != PARTNER(gBankAttacker))
				newTurnOrder[k++] = gBanksByTurnOrder[i];
		}
		
		gBanksByTurnOrder[index++] = PARTNER(gBankAttacker);
		
		if (newTurnOrder[0] != 0xFF)
			gBanksByTurnOrder[index++] = newTurnOrder[0];
			
		if (newTurnOrder[1] != 0xFF)
			gBanksByTurnOrder[index] = newTurnOrder[1];
			
		gNewBS->PledgeHelper = TRUE;
		gBattleScripting->bank = PARTNER(gBankAttacker);
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
				//gBattleScripting->animArg1 = B_ANIM_SWAMP;
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
				//BattleScripting->animArg1 = B_ANIM_RAINBOW;
			}
			else
				gBattlescriptCurrInstr = (u8*) 0x81D6947 - 5;
			break;
		
		default:
			gBattlescriptCurrInstr = (u8*) 0x81D6947 - 5;
	}
	
	gNewBS->PledgeHelper = 0;
}

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
			{
				gNewBS->NoMoreMovingThisTurn |= gBitTable[i];
				CancelMultiTurnMoves(i);
			}
			
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
			return;
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
	{
		gLastUsedItem = ITEM(gBankAttacker);
		gBattleCommunication[MOVE_EFFECT_BYTE] = 0;
		
		for (int i = 0; FlingTable[i].item != ITEM_TABLES_TERMIN; ++i) {
			if (FlingTable[i].item == gLastUsedItem) {
				gBattleCommunication[MOVE_EFFECT_BYTE] = FlingTable[i].effect;
				break;
			}
		} 
	}
}

void CheckTelekinesisFail(void) {
	u16 species = gBattleMons[gBankTarget].species;

	if (gStatuses3[gBankTarget] & (STATUS3_TELEKINESIS | STATUS3_ROOTED | STATUS3_SMACKED_DOWN)
	||  gNewBS->GravityTimer
	||  ITEM_EFFECT(gBankTarget) == ITEM_EFFECT_IRON_BALL
	||  species == SPECIES_DIGLETT
	||  species == SPECIES_DUGTRIO
	||  species == SPECIES_DIGLETT_A
	||  species == SPECIES_DUGTRIO_A
	||  species == SPECIES_SANDYGAST
	||  species == SPECIES_PALOSSAND
	||  species == SPECIES_GENGAR_MEGA)
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
			if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE
			&&  gBattleMons[PARTNER(gBankAttacker)].hp)
			{
				BattleStringLoader = HoldHandsString;
			}
			else
				gBattlescriptCurrInstr = BattleScript_ButItFailed - 5;
			
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
	&&  GetBattlerTurnOrderNum(gBankTarget) > gCurrentTurnActionNumber //Attacker moved before target
	&&  !TargetMovesRightAfterAttacker())
	{
		u8 index = 0;
		u8 k = 0;
		u8 newTurnOrder[] = {0xFF, 0xFF};
			
		for (i = 0; i < gBattlersCount; ++i) 
		{
			if (gBanksByTurnOrder[i] == gBankAttacker) 
			{
				index = i + 1; //Index after attacker; index to start swapping data
				break;
			}
		}
			
		for (i = index; i < gBattlersCount; ++i) {
			if (gBanksByTurnOrder[i] != gBankTarget)
				newTurnOrder[k++] = gBanksByTurnOrder[i];
		}
		
		gBanksByTurnOrder[index++] = gBankTarget;
		
		if (newTurnOrder[0] != 0xFF)
			gBanksByTurnOrder[index++] = newTurnOrder[0];
			
		if (newTurnOrder[1] != 0xFF)
			gBanksByTurnOrder[index] = newTurnOrder[1];
	}
	else
		gBattlescriptCurrInstr = BattleScript_ButItFailed - 5;
}

void QuashFunc(void) {
	int i;
	
	if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE
	&&  GetBattlerTurnOrderNum(gBankTarget) > gCurrentTurnActionNumber) //Attacker moved before target
	{
		int k = 0;
		u8 newTurnOrder[] = {0xFF, 0xFF, 0xFF, 0xFF};
		
		for (i = 0; i < gBattlersCount; ++i)
		{
			if (gBanksByTurnOrder[i] != gBankTarget)
				newTurnOrder[k++] = gBanksByTurnOrder[i];
		}
		
		newTurnOrder[k] = gBankTarget;
		
		for (i = 0; i < gBattlersCount; ++i)
		{
			if (newTurnOrder[i] != 0xFF)
				gBanksByTurnOrder[i] = newTurnOrder[i];
		}
	}
	else
		gBattlescriptCurrInstr = BattleScript_ButItFailed - 5;
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
	|| (GetBattlerTurnOrderNum(gBankTarget) > gCurrentTurnActionNumber && (gChosenMovesByBanks[gBankTarget] == MOVE_FOCUSPUNCH
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
	gNewBS->ZMoveData->active = FALSE;
	gNewBS->InstructInProgress = TRUE;
}

void TrySetMagnetRise(void) {

	if (gNewBS->GravityTimer
	|| gStatuses3[gBankAttacker] & (STATUS3_LEVITATING | STATUS3_SMACKED_DOWN | STATUS3_ROOTED)
	|| ITEM_EFFECT(gBankAttacker) == ITEM_EFFECT_IRON_BALL)
		gBattlescriptCurrInstr = BattleScript_ButItFailed - 5;
	else
	{
		gNewBS->MagnetRiseTimers[gBankAttacker] = 5;
		gStatuses3[gBankAttacker] |= STATUS3_LEVITATING;
	}
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

void ModifyPostStockpileBoostDecrement(void) {
	switch (gDisableStructs[gBankAttacker].stockpileCounter) {
		case 1:
			gBattleScripting->statChanger |= DECREASE_1;
			break;
		case 2:
			gBattleScripting->statChanger |= DECREASE_2;
			break;
		case 3:
			gBattleScripting->statChanger |= DECREASE_3;
			break;
	}
}

void RemoveStockpileBoosts(void) {
	gDisableStructs[gBankAttacker].stockpileCounter = 0;
}

void SetHealingWishLunarDanceFunc(void) {
	switch (gCurrentMove) {
		case MOVE_HEALINGWISH:
			gNewBS->HealingWishLoc |= gBitTable[gBankAttacker];
			break;
		case MOVE_LUNARDANCE:
			gNewBS->HealingWishLoc |= (gBitTable[gBankAttacker] << 4);
	}
}

void FinalGambitDamageCalc(void) {
	gBattleMoveDamage = gBattleMons[gBankAttacker].hp;
}

void AbilityChangeBSFunc(void) {
	u8* atkAbilityLoc, *defAbilityLoc;
	u8 atkAbility, defAbility;

	//Get correct location of ability
	atkAbilityLoc = GetAbilityLocation(gBankAttacker);
	defAbilityLoc = GetAbilityLocation(gBankTarget);
		
	atkAbility = *atkAbilityLoc;
	defAbility = *defAbilityLoc;

	switch (gCurrentMove) {
		case MOVE_WORRYSEED:
			if (CheckTableForAbility(defAbility, WorrySeedGastroAcidBanTable))
				gBattlescriptCurrInstr = BattleScript_ButItFailed - 5;
			else
			{
				*defAbilityLoc = ABILITY_INSOMNIA;
				gLastUsedAbility = ABILITY_INSOMNIA;
				RecordAbilityBattle(gBankTarget, gLastUsedAbility);
				gNewBS->SlowStartTimers[gBankTarget] = 0;
				BattleStringLoader = WorrySeedString;
			}
			break;
		
		case MOVE_GASTROACID:
			if (CheckTableForAbility(defAbility, WorrySeedGastroAcidBanTable)
			|| gStatuses3[gBankTarget] & STATUS3_ABILITY_SUPPRESS)
			{
				gBattlescriptCurrInstr = BattleScript_ButItFailed - 5;
			}
			else
			{
				gStatuses3[gBankTarget] |= STATUS3_ABILITY_SUPPRESS;
				gNewBS->SuppressedAbilities[gBankTarget] = defAbility;
				*defAbilityLoc = 0;
				RecordAbilityBattle(gBankTarget, 0);
				gNewBS->SlowStartTimers[gBankTarget] = 0;
				gBattleScripting->bank = gBankTarget;
				BattleStringLoader = AbilitySuppressedString;
			}
			break;
		
		case MOVE_ENTRAINMENT:
			if (CheckTableForAbility(atkAbility, EntrainmentBanTableAttacker)
			||  CheckTableForAbility(defAbility, EntrainmentBanTableTarget))
				gBattlescriptCurrInstr = BattleScript_ButItFailed - 5;
			else
			{
				*defAbilityLoc = atkAbility;
				gLastUsedAbility = atkAbility;
				RecordAbilityBattle(gBankTarget, gLastUsedAbility);
				gNewBS->SlowStartTimers[gBankTarget] = 0;
				BattleStringLoader = EntrainmentString;
			}
			break;
		
		case MOVE_SIMPLEBEAM:
			if (CheckTableForAbility(defAbility, SimpleBeamBanTable))
			{
				gBattlescriptCurrInstr = BattleScript_ButItFailed - 5;
			}
			else
			{
				*defAbilityLoc = ABILITY_SIMPLE;
				gLastUsedAbility = ABILITY_SIMPLE;
				RecordAbilityBattle(gBankTarget, gLastUsedAbility);
				gNewBS->SlowStartTimers[gBankTarget] = 0;
				BattleStringLoader = SimpleBeamString;
			}
			break;
	}
}

void LoadStatustoPsychoShiftTransfer(void) {
	u32 status = gBattleMons[gBankAttacker].status1;
	
	if (status & STATUS_SLEEP && CanBePutToSleep(gBankTarget))
	{
		gBattleCommunication[MOVE_EFFECT_BYTE] = MOVE_EFFECT_SLEEP;
	}
	else if (status & STATUS_TOXIC_POISON && CanBePoisoned(gBankTarget, gBankAttacker))
	{
		gBattleCommunication[MOVE_EFFECT_BYTE] = MOVE_EFFECT_TOXIC;
	}
	else if (status & STATUS_POISON && CanBePoisoned(gBankTarget, gBankAttacker))
	{
		gBattleCommunication[MOVE_EFFECT_BYTE] = MOVE_EFFECT_POISON;
	}
	else if (status & STATUS_BURN && CanBeBurned(gBankTarget))
	{
		gBattleCommunication[MOVE_EFFECT_BYTE] = MOVE_EFFECT_BURN;
	}
	else if (status & STATUS_PARALYSIS && CanBeParalyzed(gBankTarget))
	{
		gBattleCommunication[MOVE_EFFECT_BYTE] = MOVE_EFFECT_PARALYSIS;
	}
	else
		gBattlescriptCurrInstr = BattleScript_ButItFailed - 5;
}

void PluckBerryEat(void) {
	gBattlescriptCurrInstr += 5;
			
	if (ItemBattleEffects(ItemEffects_EndTurn, gBankAttacker, TRUE, TRUE))
		gBattlescriptCurrInstr -= 5;
	else if (ItemBattleEffects(ItemEffects_ContactTarget, gBankAttacker, TRUE, TRUE))
		gBattlescriptCurrInstr -= 5;
	else
		gBattlescriptCurrInstr -= 5;
}

void BurnUpFunc(void) {
	if (gBattleMons[gBankAttacker].type1 == TYPE_FIRE)
		gBattleMons[gBankAttacker].type1 = TYPE_MYSTERY;
		
	if (gBattleMons[gBankAttacker].type2 == TYPE_FIRE)
		gBattleMons[gBankAttacker].type2 = TYPE_MYSTERY;
		
	if (gBattleMons[gBankAttacker].type3 == TYPE_FIRE)
		gBattleMons[gBankAttacker].type3 = TYPE_BLANK;
}

void SeedLooper(void) {
	u8 stat = 0;
	
	switch (TerrainType) {
		case ELECTRIC_TERRAIN:
		case GRASSY_TERRAIN:
			stat = STAT_STAGE_DEF;
			break;
		case MISTY_TERRAIN:
		case PSYCHIC_TERRAIN:
			stat = STAT_STAGE_SPDEF;
	}
	
	for (; *SeedHelper < gBattlersCount; ++*SeedHelper) {
		u8 bank = gBanksByTurnOrder[*SeedHelper];
		if (ITEM_EFFECT(bank) == ITEM_EFFECT_SEEDS
		&&  ITEM_QUALITY(bank) == TerrainType
		&&  STAT_CAN_RISE(bank, stat))
		{	
			PREPARE_STAT_BUFFER(gBattleTextBuff1, stat);
			gBattleScripting->statChanger = stat | INCREASE_1;
			gBattleScripting->bank = gBankTarget = gEffectBank = bank;
			gBattlescriptCurrInstr = BattleScript_SeedStatBoost;
		}
	}
	
	gBankTarget = gBankAttacker;
}

void LastResortFunc(void) {
	int i;
	bool8 knowsLastResort = FALSE;
	
	for (i = 0; i < MAX_MON_MOVES && gBattleMons[gBankAttacker].moves[i] != 0; ++i)
	{
		u16 move = gBattleMons[gBankAttacker].moves[i];
		
		if (move == MOVE_LASTRESORT) 
			knowsLastResort = TRUE; //Last Resort can't be called from other moves
		
		else if (!(gNewBS->usedMoveIndices[gBankAttacker] & gBitTable[i]))
			gBattlescriptCurrInstr = BattleScript_ButItFailed - 2 - 5;
	}
	
	if (i == 1 //Attacker only knows Last Resort
	|| !knowsLastResort)
		gBattlescriptCurrInstr = BattleScript_ButItFailed - 2 - 5;
}

void SynchronoiseFunc(void)
{
	if (ITEM_EFFECT(gBankTarget) == ITEM_EFFECT_RING_TARGET
	|| (!IS_BLANK_TYPE(gBattleMons[gBankAttacker].type1) && IsOfType(gBankTarget, gBattleMons[gBankAttacker].type1))
	|| (!IS_BLANK_TYPE(gBattleMons[gBankAttacker].type2) && IsOfType(gBankTarget, gBattleMons[gBankAttacker].type2))
	|| (!IS_BLANK_TYPE(gBattleMons[gBankAttacker].type3) && IsOfType(gBankTarget, gBattleMons[gBankAttacker].type3)))
	{
		return;
	}

	gBattlescriptCurrInstr = BattleScript_ButItDoesntAffect - 5;
}

void MakeScriptingBankInvisible(void)
{
	if (gBattleExecBuffer) return;
	
	gActiveBattler = gBattleScripting->bank;
	EmitSpriteInvisibility(0, TRUE);
    MarkBufferBankForExecution(gActiveBattler);
}

void TransferTerrainData(void)
{
	if (gBattleExecBuffer)
		gBattlescriptCurrInstr -= 5;
	else
	{
		gActiveBattler = 0;
		EmitDataTransfer(0, &TerrainType, 1, &TerrainType);
		MarkBufferBankForExecution(gActiveBattler);
	}
}

void TransferIllusionBroken(void)
{
	if (gBattleExecBuffer)
	{
		gBattlescriptCurrInstr -= 5;
		return;
	}
	
	gActiveBattler = gBattleScripting->bank;
	EmitDataTransfer(0, &gStatuses3[gActiveBattler], 4, &gStatuses3[gActiveBattler]);
	MarkBufferBankForExecution(gActiveBattler);
}

void CycleScriptingBankHealthBetween0And1(void)
{
	if (gBattleExecBuffer)
	{
		gBattlescriptCurrInstr -= 5;
		return;
	}
	
	gActiveBattler = gBattleScripting->bank;
	
	if (!gBattleMons[gActiveBattler].hp)
		gBattleMons[gActiveBattler].hp = 1;
	else
		gBattleMons[gActiveBattler].hp = 0;
	
	EmitSetMonData(0, REQUEST_HP_BATTLE, 0, 2, &gBattleMons[gActiveBattler].hp);
	MarkBufferBankForExecution(gActiveBattler);
}

void SetTeleportBit(void)
{
	gNewBS->TeleportBit = TRUE;
}

void TryLaunchShellTrap(void)
{
	u8 atkSide = SIDE(gBankAttacker);
    u8 defSide = SIDE(gProtectStructs[gBankAttacker].physicalBank);
	
	if (!gProtectStructs[gBankAttacker].physicalDmg || atkSide == defSide)
		gBattlescriptCurrInstr = BattleScript_ButItFailed - 5 - 2;
}

void TrySuckerPunch(void)
{
	if ((SPLIT(gChosenMovesByBanks[gBankTarget]) != SPLIT_STATUS && GetBattlerTurnOrderNum(gBankTarget) > gCurrentTurnActionNumber) //Attacker moved before target
	|| gChosenMovesByBanks[gBankTarget] == MOVE_MEFIRST)
		return;

	gBattlescriptCurrInstr = BattleScript_ButItFailed - 5 - 2;
}
