#include "defines.h"
#include "helper_functions.h"

//Need emit function for atkFF11_reloadhealthbar

extern void (* const gBattleScriptingCommandsTable[])(void);
extern void (* const gBattleScriptingCommandsTable2[])(void);

extern u8 ElectricTerrainSetString[];
extern u8 GrassyTerrainSetString[];
extern u8 MistyTerrainSetString[];
extern u8 PsychicTerrainSetString[];

#define gText_TargetAlreadyAsleep (u8*) 0x83FB57C
extern u8 gText_TargetAlreadyHasStatusCondition[];
extern u8 gText_TargetWrappedInMistyTerrain[];
extern u8 gText_TargetWrappedInElectricTerrain[];
extern u8 gText_FlowerVeilProtects[];
extern u8 gText_SweetVeilProtects[];
#define gText_CantFallAsleepDuringUproar (u8*) 0x83FBDC4
#define gText_TargetStayedAwakeUsingAbility (u8*) 0x83FBDE2
extern u8 gText_TargetProtectedByAbility[];

//callasm FUNCTION_OFFSET
void atkF8_callasm(void) 
{
    u32 ptr = (u32) T1_READ_PTR(gBattlescriptCurrInstr + 1);
	ptr |= 1;
	
	void (*foo)(void) = (void (*)(void)) ptr; //The cast of doom
	foo();
	
    gBattlescriptCurrInstr += 5;
}

//sethalfword RAM_OFFSET HWORD
void atkF9_sethalfword(void)
{
	u16* offset = (u16*) T1_READ_PTR(gBattlescriptCurrInstr + 1);
	u16 hword = T1_READ_16(gBattlescriptCurrInstr + 5);
	
	if (umodsi((u32) offset, 2) == 0) //Only store is hword aligned
		*offset = hword;
	
	gBattlescriptCurrInstr += 7;
}

//setword RAM_OFFSET WORD
void atkFA_setword(void)
{
	u32* offset = (u32*) T1_READ_PTR(gBattlescriptCurrInstr + 1);
	u32 word = T1_READ_32(gBattlescriptCurrInstr + 5);
	
	if (umodsi((u32) offset, 4) == 0) //Only store is word aligned
		*offset = word;
	
	gBattlescriptCurrInstr += 9;
}

//setspecialstatusbit BANK
void atkFB_setspecialstatusbit(void)
{
	gActiveBattler = GetBattleBank(gBattlescriptCurrInstr[1]);
	u32 status3 = T1_READ_32(gBattlescriptCurrInstr + 2);
	
	gStatuses3[gActiveBattler] |= status3;

	gBattlescriptCurrInstr += 6;
}

//clearspecialstatusbit BANK
void atkFC_clearspecialstatusbit(void)
{
	gActiveBattler = GetBattleBank(gBattlescriptCurrInstr[1]);
	u32 status3 = T1_READ_32(gBattlescriptCurrInstr + 2);
	
	gStatuses3[gActiveBattler] &= ~(status3);

	gBattlescriptCurrInstr += 6;
}

//jumpifabilitypresenttargetfield ABILITY ROM_OFFSET
void atkFD_jumpifabilitypresenttargetfield(void)
{
	u8 ability = gBattlescriptCurrInstr[1];
	u8* ptr = T1_READ_PTR(gBattlescriptCurrInstr + 2);
	
	if (AbilityBattleEffects(ABILITYEFFECT_CHECK_OTHER_SIDE, gBankAttacker, ability, 0, 0))
		gBattlescriptCurrInstr = ptr;
	else
		gBattlescriptCurrInstr += 6;
}

//jumpifspecies BANK SPECIES ROM_OFFSET
void atkFE_jumpifspecies(void)
{
	u8 bank = GetBattleBank(gBattlescriptCurrInstr[1]);
	u16 species = T1_READ_16(gBattlescriptCurrInstr + 2);
	u8* ptr = T1_READ_PTR(gBattlescriptCurrInstr + 4);
	
	if (gBattleMons[bank].species == species)
		gBattlescriptCurrInstr = ptr;
	else
		gBattlescriptCurrInstr += 8;
}

void atkFF_callsecondarytable(void)
{
	gBattlescriptCurrInstr += 1;
	u8 command = gBattlescriptCurrInstr[0];
	
	void (*foo)(void) = gBattleScriptingCommandsTable2[command]; //The cast of doom
	foo();
}

//cureprimarystatus BANK
void atkFF02_cureprimarystatus(void)
{
	if (gBattleExecBuffer) return;
	
	u8 bank = GetBattleBank(gBattlescriptCurrInstr[1]);
	u8* ptr = T1_READ_PTR(gBattlescriptCurrInstr + 2);
	
	if (gBattleMons[bank].status1 == 0)
		gBattlescriptCurrInstr = ptr;
	else
	{
		ClearBankStatus(bank);
		gBattleScripting->bank = bank;
		gBattlescriptCurrInstr += 6;
	}
}

//jumpifpartnerattack BANK MOVE ROM_OFFSET
void atkFF03_jumpifpartnerattack(void)
{
	if (!(gBattleTypeFlags & BATTLE_TYPE_DOUBLE))
		gBattlescriptCurrInstr += 8;
		
	u8 bank = PARTNER(GetBattleBank(gBattlescriptCurrInstr[1]));
	u16 move = T1_READ_16(gBattlescriptCurrInstr + 2);
	u8* ptr = T1_READ_PTR(gBattlescriptCurrInstr + 4);
	
	if (gBattleMons[bank].hp && gChosenMovesByBanks[bank] == move)
		gBattlescriptCurrInstr = ptr;
	else
		gBattlescriptCurrInstr += 8;

}

//setterrain ROM_ADDRESS
void atkFF06_setterrain(void)
{
	u8 type = 0xFF;
	u8 duration = 5;
	if (ITEM_EFFECT(gBankAttacker) == ITEM_EFFECT_TERRAIN_EXTENDER)
		duration = 8;
		
	switch (gCurrentMove) {
		case MOVE_ELECTRICTERRAIN:
			type = ELECTRIC_TERRAIN;
			BattleStringLoader = ElectricTerrainSetString;
			break;
		case MOVE_GRASSYTERRAIN:
			type = GRASSY_TERRAIN;
			BattleStringLoader = GrassyTerrainSetString;
			break;
		case MOVE_MISTYTERRAIN:
			type = MISTY_TERRAIN;
			BattleStringLoader = MistyTerrainSetString;
			break;
		case MOVE_PSYCHICTERRAIN:
		case MOVE_GENESIS_SUPERNOVA:
			type = PSYCHIC_TERRAIN;
			BattleStringLoader = PsychicTerrainSetString;
			break;
	}
	
	if (TerrainType == type || type == 0xFF)
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
	else
	{
		TerrainType = type;
		gNewBS->TerrainTimer = duration;
		gBattlescriptCurrInstr += 5;
	}
}

//jumpifhelditemeffect BANK ITEM_EFFECT ROM_OFFSET
void atkFF07_jumpifhelditemeffect(void)
{
	u8 bank = GetBattleBank(gBattlescriptCurrInstr[1]);
	u8 effect = T1_READ_8(gBattlescriptCurrInstr + 2);
	u8* ptr = T1_READ_PTR(gBattlescriptCurrInstr + 3);

	if (ITEM_EFFECT(bank) == effect)
	{
		gLastUsedItem = ITEM(bank);
		gBattlescriptCurrInstr = ptr;
	}
	else
		gBattlescriptCurrInstr += 7;
}

enum Counters
{
	Counters_Telekinesis,		//0
	Counters_MagnetRise,		//1
	Counters_HealBlock,			//2
	Counters_LaserFocus,		//3
	Counters_ThroatChop,		//4
	Counters_Embargo,			//5
	Counters_Electrify,			//6
	Counters_SlowStart,			//7
	Counters_Stakeout,			//8
	Counters_StompingTantrum,	//9
	Counters_Nimble,			//10
	Counters_DestinyBond,		//11
	Counters_Metronome,			//12
	Counters_Incinerate,		//13
	Counters_Powder,			//14
	Counters_BeakBlast,			//15
};


//counterclear BANK COUNTER_ID FAIL_OFFSET
void atkFF08_counterclear(void)
{
	u8 bank = GetBattleBank(gBattlescriptCurrInstr[1]);
	u8 id = T1_READ_8(gBattlescriptCurrInstr + 2);
	u8* ptr = T1_READ_PTR(gBattlescriptCurrInstr + 3);
	bool8 failed = FALSE;
	
	switch(id) {
		case Counters_Telekinesis:
			if (gNewBS->TelekinesisTimers[bank])
				gNewBS->TelekinesisTimers[bank] = 0;
			else
				failed = TRUE;
			break;
		case Counters_MagnetRise:
			if (gNewBS->MagnetRiseTimers[bank])
				gNewBS->MagnetRiseTimers[bank] = 0;
			else
				failed = TRUE;
			break;
		case Counters_HealBlock:
			if (gNewBS->HealBlockTimers[bank])
				gNewBS->HealBlockTimers[bank] = 0;
			else
				failed = TRUE;
			break;
		case Counters_LaserFocus:
			if (gNewBS->LaserFocusTimers[bank])
				gNewBS->LaserFocusTimers[bank] = 0;
			else
				failed = TRUE;
			break;
		case Counters_ThroatChop:
			if (gNewBS->ThroatChopTimers[bank])
				gNewBS->ThroatChopTimers[bank] = 0;
			else
				failed = TRUE;
			break;
		case Counters_Embargo:
			if (gNewBS->EmbargoTimers[bank])
				gNewBS->EmbargoTimers[bank] = 0;
			else
				failed = TRUE;
			break;
		case Counters_Electrify:
			if (gNewBS->ElectrifyTimers[bank])
				gNewBS->ElectrifyTimers[bank] = 0;
			else
				failed = TRUE;
			break;
		case Counters_SlowStart:
			if (gNewBS->SlowStartTimers[bank])
				gNewBS->SlowStartTimers[bank] = 0;
			else
				failed = TRUE;
			break;
		case Counters_Stakeout:
			if (gNewBS->StakeoutCounters[bank])
				gNewBS->StakeoutCounters[bank] = 0;
			else
				failed = TRUE;
			break;
		case Counters_StompingTantrum:
			if (gNewBS->StompingTantrumTimers[bank])
				gNewBS->StompingTantrumTimers[bank] = 0;
			else
				failed = TRUE;
			break;
		case Counters_Nimble:
			if (gNewBS->NimbleCounters[bank])
				gNewBS->NimbleCounters[bank] = 0;
			else
				failed = TRUE;
			break;
		case Counters_DestinyBond:
			if (gNewBS->DestinyBondCounters[bank])
				gNewBS->DestinyBondCounters[bank] = 0;
			else
				failed = TRUE;
			break;
		case Counters_Metronome:
			if (gNewBS->MetronomeCounter[bank])
				gNewBS->MetronomeCounter[bank] = 0;
			else
				failed = TRUE;
			break;
		case Counters_Incinerate:
			if (gNewBS->IncinerateCounters[bank])
				gNewBS->IncinerateCounters[bank] = 0;
			else
				failed = TRUE;
			break;
		case Counters_Powder:
			if (gNewBS->PowderByte & gBitTable[bank])
				gNewBS->PowderByte &= ~(gBitTable[bank]);
			else
				failed = TRUE;
			break;
		case Counters_BeakBlast:
			if (gNewBS->BeakBlastByte & gBitTable[bank])
				gNewBS->BeakBlastByte &= ~(gBitTable[bank]);
			else
				failed = TRUE;
			break;
	}
	
	if (failed)
		gBattlescriptCurrInstr = ptr;
	else
		gBattlescriptCurrInstr += 7;
}

//jumpifcounter BANK COUNTER_ID PREDICATE BYTE_TO_COMPARE ROM_OFFSET
void atkFF09_jumpifcounter(void)
{
	u8 bank = GetBattleBank(gBattlescriptCurrInstr[1]);
	u8 id = T1_READ_8(gBattlescriptCurrInstr + 2);
	u8 predicate = T1_READ_8(gBattlescriptCurrInstr + 3);
	u8 value = T1_READ_8(gBattlescriptCurrInstr + 4);
	u8* ptr = T1_READ_PTR(gBattlescriptCurrInstr + 5);
	u8 counter;
	bool8 ret = FALSE;
	
	switch(id) {
		case Counters_Telekinesis:
			counter = gNewBS->TelekinesisTimers[bank];
			break;
		case Counters_MagnetRise:
			counter = gNewBS->MagnetRiseTimers[bank];
			break;
		case Counters_HealBlock:
			counter = gNewBS->HealBlockTimers[bank];
			break;
		case Counters_LaserFocus:
			counter = gNewBS->LaserFocusTimers[bank];
			break;
		case Counters_ThroatChop:
			counter = gNewBS->ThroatChopTimers[bank];
			break;
		case Counters_Embargo:
			counter = gNewBS->EmbargoTimers[bank];
			break;
		case Counters_Electrify:
			counter = gNewBS->ElectrifyTimers[bank];
			break;
		case Counters_SlowStart:
			counter = gNewBS->SlowStartTimers[bank];
			break;
		case Counters_Stakeout:
			counter = gNewBS->StakeoutCounters[bank];
			break;
		case Counters_StompingTantrum:
			counter = gNewBS->StompingTantrumTimers[bank];
			break;
		case Counters_Nimble:
			counter = gNewBS->NimbleCounters[bank];
			break;
		case Counters_DestinyBond:
			counter = gNewBS->DestinyBondCounters[bank];
			break;
		case Counters_Metronome:
			counter = gNewBS->MetronomeCounter[bank];
			break;
		case Counters_Incinerate:
			counter = gNewBS->IncinerateCounters[bank];
			break;
		case Counters_Powder:
			counter = gNewBS->PowderByte & gBitTable[bank];
			break;
		case Counters_BeakBlast:
			counter = gNewBS->BeakBlastByte & gBitTable[bank];
			break;
		default:
			counter = 0; //Shouldn't happen...
	}
	
	switch(predicate) {
		case CMP_EQUAL:
			if (value == counter)
				ret++;
			break;
		case CMP_NOT_EQUAL:
			if (value != counter)
				ret++;
			break;
		case CMP_GREATER_THAN:
			if (value > counter)
				ret++;
			break;
		case CMP_LESS_THAN:
			if (value < counter)
				ret++;
			break;
		case CMP_COMMON_BITS:
			if (value & counter)
				ret++;
			break;
		case CMP_NO_COMMON_BITS:
			if (!(value & counter))
				ret++;
			break;
	}
	
	if (ret)
		gBattlescriptCurrInstr = ptr;
	else
		gBattlescriptCurrInstr += 9;
}

//setability BANK ABILITY
void atkFF0A_setability(void)
{
	u8 bank = GetBattleBank(gBattlescriptCurrInstr[1]);
	u8 ability = gBattlescriptCurrInstr[2];
	ability_t* abilityLoc = GetAbilityLocation(bank);
	
	*abilityLoc = ability;
	
	gBattlescriptCurrInstr += 3;
}

//jumpiftargetpartner ROM_OFFSET
void atkFF0C_jumpiftargetpartner(void)
{
	u8* ptr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
	
	if (gBankTarget == PARTNER(gBankAttacker))
		gBattlescriptCurrInstr = ptr;
	else
		gBattlescriptCurrInstr += 5;
}

//setcounter BANK COUNTER_ID AMOUNT
void atkFF0E_setcounter(void)
{
	u8 bank = GetBattleBank(gBattlescriptCurrInstr[1]);
	u8 id = gBattlescriptCurrInstr[2];
	u8 amount = gBattlescriptCurrInstr[3];

	switch(id) {
		case Counters_Telekinesis:
			gNewBS->TelekinesisTimers[bank] = amount;
			break;
		case Counters_MagnetRise:
			gNewBS->MagnetRiseTimers[bank] = amount;
			break;
		case Counters_HealBlock:
			gNewBS->HealBlockTimers[bank] = amount;
			break;
		case Counters_LaserFocus:
			gNewBS->LaserFocusTimers[bank] = amount;
			break;
		case Counters_ThroatChop:
			gNewBS->ThroatChopTimers[bank] = amount;
			break;
		case Counters_Embargo:
			gNewBS->EmbargoTimers[bank] = amount;
			break;
		case Counters_Electrify:
			gNewBS->ElectrifyTimers[bank] = amount;
			break;
		case Counters_SlowStart:
			gNewBS->SlowStartTimers[bank] = amount;
			break;
		case Counters_Stakeout:
			gNewBS->StakeoutCounters[bank] = amount;
			break;
		case Counters_StompingTantrum:
			gNewBS->StompingTantrumTimers[bank] = amount;
			break;
		case Counters_Nimble:
			gNewBS->NimbleCounters[bank] = amount;
			break;
		case Counters_DestinyBond:
			gNewBS->DestinyBondCounters[bank] = amount;
			break;
		case Counters_Metronome:
			gNewBS->MetronomeCounter[bank] = amount;
			break;
		case Counters_Incinerate:
			gNewBS->IncinerateCounters[bank] = amount;
			break;
		case Counters_Powder:
			gNewBS->PowderByte |= gBitTable[bank];
			break;
		case Counters_BeakBlast:
			gNewBS->BeakBlastByte |= gBitTable[bank];
			break;
	}

	gBattlescriptCurrInstr += 4;
}

//jumpifgrounded BANK ROM_OFFSET
void atkFF0F_jumpifgrounded(void)
{
	u8 bank = GetBattleBank(gBattlescriptCurrInstr[1]);
	u8* ptr = T1_READ_PTR(gBattlescriptCurrInstr + 2);

	if (CheckGrounding(bank))
		gBattlescriptCurrInstr = ptr;
	else
		gBattlescriptCurrInstr += 6;
}

//jumpifhelditem BANK ITEM ROM_OFFSET
void atkFF10_jumpifhelditem(void)
{
	u8 bank = GetBattleBank(gBattlescriptCurrInstr[1]);
	u16 item = T1_READ_16(gBattlescriptCurrInstr + 2);
	u8* ptr = T1_READ_PTR(gBattlescriptCurrInstr + 4);

	if (ITEM(bank) == item)
		gBattlescriptCurrInstr = ptr;
	else
		gBattlescriptCurrInstr += 8;
}

//reloadhealthbar BANK
void atkFF11_reloadhealthbar(void)
{
	if (gBattleExecBuffer) return;

	gActiveBattler = GetBattleBank(gBattlescriptCurrInstr[1]);
	UpdateHealthboxAttribute(gHealthboxIDs[gActiveBattler], 
							GetBankPartyData(gActiveBattler), 
							HEALTHBOX_ALL);
	gBattlescriptCurrInstr += 2;
}

//jumpifhealthcomparestomax BANK PREDICATE ROM_OFFSET
void atkFF12_jumpifhealthcomparestomax(void)
{
	u8 bank = GetBattleBank(gBattlescriptCurrInstr[1]);
	u8 predicate = gBattlescriptCurrInstr[2];
	u8* ptr = T1_READ_PTR(gBattlescriptCurrInstr + 3);
	u16 value = gBattleMons[bank].hp;
	u16 comparison = gBattleMons[bank].maxHP;
	bool8 ret = FALSE;

    switch (predicate) {
		case CMP_EQUAL:
			if (value == comparison)
				ret++;
			break;
		case CMP_NOT_EQUAL:
			if (value != comparison)
				ret++;
			break;
    }

	if (ret)
		gBattlescriptCurrInstr = ptr;
	else
		gBattlescriptCurrInstr += 7;
}

//atkFF13_setdamagetobankhealthpercent BANK PERCENT
void atkFF13_setdamagetobankhealthpercent(void)
{
	u8 bank = GetBattleBank(gBattlescriptCurrInstr[1]);
	u8 percent = gBattlescriptCurrInstr[2];
	gBattleMoveDamage = udivsi(gBattleMons[bank].maxHP * percent, 100);
	gBattlescriptCurrInstr += 3;
}

//jumpiftypepresent TYPE ROM_OFFSET
void atkFF14_jumpiftypepresent(void)
{
	u8 type = gBattlescriptCurrInstr[1];
	u8* ptr = T1_READ_PTR(gBattlescriptCurrInstr + 2);

	for (int i = 0; i < 4; ++i)
	{
		if (IsOfType(i, type))
		{
			gBattlescriptCurrInstr = ptr;
			return;
		}
	}
	
	gBattlescriptCurrInstr += 6;
}

//jumpifstatcanbelowered BANK STAT ROM_ADDRESS
void atkFF15_jumpifstatcanbemodified(void)
{
    u32 currStat = 0;
	FormCounter = 0;

    gActiveBattler = GetBattleBank(gBattlescriptCurrInstr[1]);
    currStat = T2_READ_8(gBattlescriptCurrInstr + 2);
	u8 ability = ABILITY(gActiveBattler);

    if (T2_READ_8(gBattlescriptCurrInstr + 3) & ATK48_STAT_NEGATIVE) // goes down
    {
		if (ability == ABILITY_CONTRARY)
			goto STAT_ANIM_UP;
	
	STAT_ANIM_DOWN:
        if (gBattleMons[gActiveBattler].statStages[currStat - 1] == 0)
			FormCounter = 1;
		
		else if (gSideTimers[SIDE(gActiveBattler)].mistTimer && (gBattlescriptCurrInstr[1] != 0x0 || ABILITY(gBankAttacker) != ABILITY_INFILTRATOR))
			FormCounter = 2;
			
		else if (ability == ABILITY_CLEARBODY
        || ability == ABILITY_WHITESMOKE
		|| ability == ABILITY_FULLMETALBODY)
			FormCounter = 3;
			
		else if ((ability == ABILITY_KEENEYE && currStat == STAT_STAGE_ACC)
        || (ability == ABILITY_HYPERCUTTER && currStat == STAT_STAGE_ATK)
		|| (ability == ABILITY_BIGPECKS && currStat == STAT_STAGE_DEF))
			FormCounter = 4;
        
		PREPARE_STAT_BUFFER(gBattleTextBuff1, currStat)
		
		if (FormCounter)
			gBattlescriptCurrInstr += 8;
		else
			gBattlescriptCurrInstr = T2_READ_PTR(gBattlescriptCurrInstr + 4);
    }
	
    else // goes up
    {
		if (ability == ABILITY_CONTRARY)
			goto STAT_ANIM_DOWN;
	
	STAT_ANIM_UP:	;
		if (gBattleMons[gActiveBattler].statStages[currStat - 1] >= 12)
		{
			FormCounter = 5;
			gBattlescriptCurrInstr += 8;
		}
		else
			gBattlescriptCurrInstr = T2_READ_PTR(gBattlescriptCurrInstr + 4);
    }
}

//jumpifnoviablemonsleft BANK ROM_ADDRESS
void atkFF16_jumpifnoviablemonsleft(void)
{
	if (ViableMonCountFromBank(GetBattleBank(gBattlescriptCurrInstr[1])) == 0)
		gBattlescriptCurrInstr = T2_READ_PTR(gBattlescriptCurrInstr + 2);
	else
		gBattlescriptCurrInstr += 6;
}

//setsidestatus BANK STATUS
void atkFF17_setsidestatus(void)
{
	u8 side = SIDE(GetBattleBank(gBattlescriptCurrInstr[1]));
	u16 status = T1_READ_16(gBattlescriptCurrInstr + 2);

	gSideAffecting[side] |= status;

	gBattlescriptCurrInstr += 3;
}

//clearsidestatus BANK STATUS
void atkFF18_clearsidestatus(void)
{
	u8 side = SIDE(GetBattleBank(gBattlescriptCurrInstr[1]));
	u16 status = T1_READ_16(gBattlescriptCurrInstr + 2);

	gSideAffecting[side] &= ~status;

	gBattlescriptCurrInstr += 3;
}

//formchange BANK ORIGIN_SPECIES TARGET_SPECIES RELOAD_TYPE ROM_ADDRESS
void atkFF19_formchange(void)
{
	if (gBattleExecBuffer) return;

	u8 bank = GetBattleBank(gBattlescriptCurrInstr[1]);
	u16 originalSpecies = T1_READ_16(gBattlescriptCurrInstr + 2);
	u16 targetSpecies = T1_READ_16(gBattlescriptCurrInstr + 4);
	bool8 reloadType = T2_READ_8(gBattlescriptCurrInstr + 6);
	bool8 reloadStats = T2_READ_8(gBattlescriptCurrInstr + 7);
	
	if (gBattleMons[bank].species != originalSpecies || gBattleMons[bank].hp == 0)
		gBattlescriptCurrInstr = T2_READ_PTR(gBattlescriptCurrInstr + 7);
	else
	{
		DoFormChange(bank, targetSpecies, reloadType, reloadStats);
		gBattlescriptCurrInstr += 12;
	}
}

//jumpifabilitypresentattackerfield ABILITY ROM_OFFSET
void atkFF1A_jumpifabilitypresentattackerfield(void)
{
	u8 ability = gBattlescriptCurrInstr[1];
	u8* ptr = T1_READ_PTR(gBattlescriptCurrInstr + 2);
	
	if (AbilityBattleEffects(ABILITYEFFECT_CHECK_BANK_SIDE, gBankAttacker, ability, 0, 0))
		gBattlescriptCurrInstr = ptr;
	else
		gBattlescriptCurrInstr += 6;
}

void atkFF1B_tryactivateswitchinability(void)
{
	u8 bank = GetBattleBank(gBattlescriptCurrInstr[1]);
	
	if (AbilityBattleEffects(ABILITYEFFECT_ON_SWITCHIN, bank, 0, 0, 0))
		return;
	
	gBattlescriptCurrInstr += 2;
}

/*
Doesn't Affect (Spore, Minior Shield etc.)
Attack Misses
Already Asleep
Already Has Status (Failed)
Misty Terrain / Electric Terrain
Flower Veil
Sweet Veil
Ability Protects
*/

//atkFF1C - atkFF1E: Trainer Sliding

//trysetsleep BANK FAIL_ADDRESS
void atkFF1F_trysetsleep(void)
{
	u8 bank = GetBattleBank(gBattlescriptCurrInstr[1]);
	u8* ptr = T1_READ_PTR(gBattlescriptCurrInstr + 2);
	bool8 fail = FALSE;
	
	if (gBattleMons[bank].status1 & STATUS1_SLEEP || ABILITY(bank) == ABILITY_COMATOSE)
	{
		BattleStringLoader = gText_TargetAlreadyAsleep;
		fail = TRUE;
	}
	else if (gBattleMons[bank].status1)
	{
		BattleStringLoader = gText_TargetAlreadyHasStatusCondition; //String not in official games; officially "But it failed!"
		fail = TRUE;
	}
	else if (CheckGrounding(bank) && TerrainType == MISTY_TERRAIN)
	{
		BattleStringLoader = gText_TargetWrappedInMistyTerrain;
		fail = TRUE;
	}
	else if (CheckGrounding(bank) && TerrainType == ELECTRIC_TERRAIN)
	{
		BattleStringLoader = gText_TargetWrappedInElectricTerrain;
		fail = TRUE;
	}
	else if (IsOfType(bank, TYPE_GRASS) && ABILITY(bank) == ABILITY_FLOWERVEIL)
	{
		gBattleScripting->bank = bank;
		BattleStringLoader = gText_FlowerVeilProtects;
		fail = TRUE;
	}
	else if (IsOfType(bank, TYPE_GRASS) && gBattleTypeFlags & BATTLE_TYPE_DOUBLE && ABILITY(PARTNER(bank)) == ABILITY_FLOWERVEIL)
	{
		gBattleScripting->bank = PARTNER(bank);
		BattleStringLoader = gText_FlowerVeilProtects;
		fail = TRUE;
	}
	else if (ABILITY(bank) == ABILITY_SWEETVEIL)
	{
		gBattleScripting->bank = bank;
		BattleStringLoader = gText_SweetVeilProtects;
		fail = TRUE;
	}
	else if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE && ABILITY(PARTNER(bank)) == ABILITY_SWEETVEIL)
	{
		gBattleScripting->bank = PARTNER(bank);
		BattleStringLoader = gText_SweetVeilProtects;
		fail = TRUE;
	}
	else if (IsUproarBeingMade())
	{
		gBattleScripting->bank = bank;
		BattleStringLoader = gText_CantFallAsleepDuringUproar;
		fail = TRUE;	
	}
	
	if (!fail)
	{
		switch (ABILITY(bank)) {
			case ABILITY_INSOMNIA:
			case ABILITY_VITALSPIRIT:
				BattleStringLoader = gText_TargetStayedAwakeUsingAbility;
				fail = TRUE;
				break;
			case ABILITY_LEAFGUARD:
				if (WEATHER_HAS_EFFECT && gBattleWeather & WEATHER_SUN_ANY)
				{
					BattleStringLoader = gText_TargetProtectedByAbility;
					fail = TRUE;
				}
		}
	}

	if (fail)
		gBattlescriptCurrInstr = ptr;
	else
		gBattlescriptCurrInstr += 6;	
}
