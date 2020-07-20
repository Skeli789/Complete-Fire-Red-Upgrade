#include "defines.h"
#include "defines_battle.h"
#include "../include/battle_string_ids.h"
#include "../include/random.h"
#include "../include/constants/items.h"

#include "../include/new/ability_battle_scripts.h"
#include "../include/new/battle_strings.h"
#include "../include/new/battle_util.h"
#include "../include/new/battle_script_util.h"
#include "../include/new/cmd49.h"
#include "../include/new/cmd49_battle_scripts.h"
#include "../include/new/damage_calc.h"
#include "../include/new/dynamax.h"
#include "../include/new/form_change.h"
#include "../include/new/general_bs_commands.h"
#include "../include/new/item.h"
#include "../include/new/move_battle_scripts.h"
#include "../include/new/move_tables.h"
#include "../include/new/multi.h"
#include "../include/new/new_bs_commands.h"
#include "../include/new/set_effect.h"
#include "../include/new/util.h"

/*
new_bs_commands.c
	functions for any additional battle scripting commands that are used inside battle scripts
*/

extern void (* const gBattleScriptingCommandsTable[])(void);
extern void (* const gBattleScriptingCommandsTable2[])(void);

//callasm FUNCTION_OFFSET
void atkF8_callasm(void)
{
	u32 ptr = (u32) T1_READ_PTR(gBattlescriptCurrInstr + 1);
	ptr |= 1;

	void (*func)(void) = (void (*)(void)) ptr; //The cast of doom
	func();

	gBattlescriptCurrInstr += 5; //Ideally this should be before the function is called, but too late now lol
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
	gActiveBattler = GetBankForBattleScript(gBattlescriptCurrInstr[1]);
	u32 status3 = T1_READ_32(gBattlescriptCurrInstr + 2);

	gStatuses3[gActiveBattler] |= status3;

	gBattlescriptCurrInstr += 6;
}

//clearspecialstatusbit BANK
void atkFC_clearspecialstatusbit(void)
{
	gActiveBattler = GetBankForBattleScript(gBattlescriptCurrInstr[1]);
	u32 status3 = T1_READ_32(gBattlescriptCurrInstr + 2);

	gStatuses3[gActiveBattler] &= ~(status3);

	gBattlescriptCurrInstr += 6;
}

//jumpifabilitypresenttargetfield ABILITY ROM_OFFSET
void atkFD_jumpifabilitypresenttargetfield(void)
{
	u8 ability = gBattlescriptCurrInstr[1];
	u8* ptr = T1_READ_PTR(gBattlescriptCurrInstr + 2);

	if (ABILITY(gBankTarget) == ability)
		gBattleScripting.bank = gBankTarget;
	else if (ABILITY(PARTNER(gBankTarget)) == ability)
		gBattleScripting.bank = PARTNER(gBankTarget);
	else
	{
		gBattlescriptCurrInstr += 6;
		return;
	}

	gLastUsedAbility = ability;
	gBattlescriptCurrInstr = ptr;
}

//jumpifspecies BANK SPECIES ROM_OFFSET
void atkFF22_jumpifspecies(void)
{
	u8 bank = GetBankForBattleScript(gBattlescriptCurrInstr[1]);
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

//cureprimarystatus BANK FAIL_POINTER
void atkFF02_cureprimarystatus(void)
{
	if (gBattleExecBuffer) return;

	u8 bank = GetBankForBattleScript(gBattlescriptCurrInstr[1]);
	u8* ptr = T1_READ_PTR(gBattlescriptCurrInstr + 2);

	if (gBattleMons[bank].status1 == 0)
	{
		if (ptr != NULL)
		{
			gBattlescriptCurrInstr = ptr;
			return;
		}
	}
	else
	{
		ClearBankStatus(bank);
		gBattleScripting.bank = bank;
	}

	gBattlescriptCurrInstr += 6;
}

//jumpifpartnerattack BANK MOVE ROM_OFFSET
void atkFF03_jumpifpartnerattack(void)
{
	if (IS_SINGLE_BATTLE)
		gBattlescriptCurrInstr += 8;

	u8 bank = PARTNER(GetBankForBattleScript(gBattlescriptCurrInstr[1]));
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

	if (gBattleTypeFlags & BATTLE_TYPE_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_TERRAIN)
		type = 0xFF; //Can't be changed
	else
	{
		if (IsAnyMaxMove(gCurrentMove))
		{
			switch (gBattleMoves[gCurrentMove].z_move_effect) {
				case MAX_EFFECT_ELECTRIC_TERRAIN:
					goto SET_ELECTRIC_TERRAIN;
				case MAX_EFFECT_GRASSY_TERRAIN:
					goto SET_GRASSY_TERRAIN;
				case MAX_EFFECT_MISTY_TERRAIN:
					goto SET_MISTY_TERRAIN;
				case MAX_EFFECT_PSYCHIC_TERRAIN:
					goto SET_PSYCHIC_TERRAIN;
				case MAX_EFFECT_DEFOG:
					goto REMOVE_TERRAIN;
			}
		}

		switch (gCurrentMove) {
			case MOVE_ELECTRICTERRAIN:
			SET_ELECTRIC_TERRAIN:
				type = ELECTRIC_TERRAIN;
				gBattleScripting.animArg1 = B_ANIM_ELECTRIC_SURGE;
				gBattleStringLoader = ElectricTerrainSetString;
				break;
			case MOVE_GRASSYTERRAIN:
			SET_GRASSY_TERRAIN:
				type = GRASSY_TERRAIN;
				gBattleScripting.animArg1 = B_ANIM_GRASSY_SURGE;
				gBattleStringLoader = GrassyTerrainSetString;
				break;
			case MOVE_MISTYTERRAIN:
			SET_MISTY_TERRAIN:
				type = MISTY_TERRAIN;
				gBattleScripting.animArg1 = B_ANIM_MISTY_SURGE;
				gBattleStringLoader = MistyTerrainSetString;
				break;
			case MOVE_PSYCHICTERRAIN:
			case MOVE_GENESIS_SUPERNOVA:
			SET_PSYCHIC_TERRAIN:
				type = PSYCHIC_TERRAIN;
				gBattleScripting.animArg1 = B_ANIM_PSYCHIC_SURGE;
				gBattleStringLoader = PsychicTerrainSetString;
				break;
			case MOVE_SPLINTERED_STORMSHARDS:
			case MOVE_DEFOG:
			case MOVE_STEELROLLER:
			REMOVE_TERRAIN:
				type = 0;
				gNewBS->terrainForcefullyRemoved = TRUE;
				gBattleScripting.animArg1 = B_ANIM_LOAD_DEFAULT_BG;
				gBattleStringLoader = TerrainEndString;
				break;
		}
	}

	if (gTerrainType == type || type == 0xFF)
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
	else
	{
		gTerrainType = type;

		if (type == 0) //No Terrain means no timer
			gNewBS->TerrainTimer = 0;
		else
			gNewBS->TerrainTimer = duration;
		gBattlescriptCurrInstr += 5;
	}
}

//jumpifhelditemeffect BANK ITEM_EFFECT ROM_OFFSET
void atkFF07_jumpifhelditemeffect(void)
{
	u8 bank = GetBankForBattleScript(gBattlescriptCurrInstr[1]);
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

//counterclear BANK COUNTER_ID FAIL_OFFSET
void atkFF08_counterclear(void)
{
	u8 bank = GetBankForBattleScript(gBattlescriptCurrInstr[1]);
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
		case Counters_AuroraVeil:
			if (gNewBS->AuroraVeilTimers[SIDE(bank)])
				gNewBS->AuroraVeilTimers[SIDE(bank)] = 0;
			else
				failed = TRUE;
			break;
		case Counters_TarShot:
			if (gNewBS->tarShotBits & gBitTable[bank])
				gNewBS->tarShotBits &= ~(gBitTable[bank]);
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
	u8 bank = GetBankForBattleScript(gBattlescriptCurrInstr[1]);
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
			counter = IsHealBlocked(bank);
			break;
		case Counters_LaserFocus:
			counter = gNewBS->LaserFocusTimers[bank];
			break;
		case Counters_ThroatChop:
			counter = CantUseSoundMoves(bank);
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
		case Counters_AuroraVeil:
			counter = gNewBS->AuroraVeilTimers[SIDE(bank)];
			break;
		case Counters_TarShot:
			counter = gNewBS->tarShotBits & gBitTable[bank];
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
	u8 bank = GetBankForBattleScript(gBattlescriptCurrInstr[1]);
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
	u8 bank = GetBankForBattleScript(gBattlescriptCurrInstr[1]);
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
		case Counters_TarShot:
			gNewBS->tarShotBits |= gBitTable[bank];
			break;
	}

	gBattlescriptCurrInstr += 4;
}

//jumpifgrounded BANK ROM_OFFSET
void atkFF0F_jumpifgrounded(void)
{
	u8 bank = GetBankForBattleScript(gBattlescriptCurrInstr[1]);
	u8* ptr = T1_READ_PTR(gBattlescriptCurrInstr + 2);

	if (CheckGrounding(bank))
		gBattlescriptCurrInstr = ptr;
	else
		gBattlescriptCurrInstr += 6;
}

//jumpifhelditem BANK ITEM ROM_OFFSET
void atkFF10_jumpifhelditem(void)
{
	u8 bank = GetBankForBattleScript(gBattlescriptCurrInstr[1]);
	u16 item = T1_READ_16(gBattlescriptCurrInstr + 2);
	u8* ptr = T1_READ_PTR(gBattlescriptCurrInstr + 4);

	if (ITEM(bank) == item)
		gBattlescriptCurrInstr = ptr;
	else
		gBattlescriptCurrInstr += 8;
}

//reloadhealthbar BANK
//void atkFF11_reloadhealthbar(void)

//jumpifhealthcomparestomax BANK PREDICATE ROM_OFFSET
void atkFF12_jumpifhealthcomparestomax(void)
{
	u8 bank = GetBankForBattleScript(gBattlescriptCurrInstr[1]);
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

//atkFF13_setdamagetobankhealthfraction BANK FRACTION USE_ACTUAL_MAX_HP
void atkFF13_setdamagetobankhealthfraction(void)
{
	u8 bank = GetBankForBattleScript(gBattlescriptCurrInstr[1]);
	s8 fraction = gBattlescriptCurrInstr[2];
	bool8 useActualMaxHP = gBattlescriptCurrInstr[3];

	if (useActualMaxHP)
		gBattleMoveDamage = gBattleMons[bank].maxHP / fraction;
	else
		gBattleMoveDamage = GetBaseMaxHP(bank) / fraction;

	gBattlescriptCurrInstr += 4;
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
	gFormCounter = 0;

	gActiveBattler = GetBankForBattleScript(gBattlescriptCurrInstr[1]);
	currStat = T2_READ_8(gBattlescriptCurrInstr + 2);
	u8 ability = ABILITY(gActiveBattler);

	if (T2_READ_8(gBattlescriptCurrInstr + 3) & ATK48_STAT_NEGATIVE) // goes down
	{
		if (ability == ABILITY_CONTRARY)
			goto STAT_ANIM_UP;

	STAT_ANIM_DOWN:
		if (gBattleMons[gActiveBattler].statStages[currStat - 1] == 0)
			gFormCounter = 1;

		else if (BankSideHasMist(gActiveBattler) && (gBattlescriptCurrInstr[1] != BS_GET_TARGET || ABILITY(gBankAttacker) != ABILITY_INFILTRATOR))
			gFormCounter = 2;

		else if (ability == ABILITY_CLEARBODY
		|| ability == ABILITY_WHITESMOKE
		//|| ability == ABILITY_FULLMETALBODY
		|| (ability == ABILITY_FLOWERVEIL && IsOfType(gActiveBattler, TYPE_GRASS)))
		{
			gBattleScripting.bank = gActiveBattler;
			gFormCounter = 3;
		}
		else if (ABILITY(PARTNER(gActiveBattler)) == ABILITY_FLOWERVEIL
		&& IsOfType(gActiveBattler, TYPE_GRASS))
		{
			gBattleScripting.bank = PARTNER(gActiveBattler);
			gFormCounter = 3;
		}
		else if ((ability == ABILITY_KEENEYE && currStat == STAT_STAGE_ACC)
		|| (ability == ABILITY_HYPERCUTTER && currStat == STAT_STAGE_ATK)
		|| (ability == ABILITY_BIGPECKS && currStat == STAT_STAGE_DEF))
			gFormCounter = 4;

		PREPARE_STAT_BUFFER(gBattleTextBuff1, currStat)

		if (gFormCounter)
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
			gFormCounter = 5;
			gBattlescriptCurrInstr += 8;
		}
		else
			gBattlescriptCurrInstr = T2_READ_PTR(gBattlescriptCurrInstr + 4);
	}
}

//jumpifnoviablemonsleft BANK ROM_ADDRESS
void atkFF16_jumpifnoviablemonsleft(void)
{
	if (ViableMonCountFromBank(GetBankForBattleScript(gBattlescriptCurrInstr[1])) == 0)
		gBattlescriptCurrInstr = T2_READ_PTR(gBattlescriptCurrInstr + 2);
	else
		gBattlescriptCurrInstr += 6;
}

//setsidestatus BANK STATUS
void atkFF17_setsidestatus(void)
{
	u8 side = SIDE(GetBankForBattleScript(gBattlescriptCurrInstr[1]));
	u16 status = T1_READ_16(gBattlescriptCurrInstr + 2);

	gSideStatuses[side] |= status;

	gBattlescriptCurrInstr += 3;
}

//clearsidestatus BANK STATUS
void atkFF18_clearsidestatus(void)
{
	u8 side = SIDE(GetBankForBattleScript(gBattlescriptCurrInstr[1]));
	u16 status = T1_READ_16(gBattlescriptCurrInstr + 2);

	gSideStatuses[side] &= ~status;

	gBattlescriptCurrInstr += 3;
}

//formchange BANK ORIGIN_SPECIES TARGET_SPECIES RELOAD_TYPE RELOAD_STATS RELOAD_ABILITY ROM_ADDRESS
void atkFF19_formchange(void)
{
	if (gBattleExecBuffer) return;

	u8 bank = GetBankForBattleScript(gBattlescriptCurrInstr[1]);
	u16 originalSpecies = T1_READ_16(gBattlescriptCurrInstr + 2);
	u16 targetSpecies = T1_READ_16(gBattlescriptCurrInstr + 4);
	bool8 reloadType = T2_READ_8(gBattlescriptCurrInstr + 6);
	bool8 reloadStats = T2_READ_8(gBattlescriptCurrInstr + 7);
	bool8 reloadAbility = T2_READ_8(gBattlescriptCurrInstr + 8);


	if (gBattleMons[bank].species != originalSpecies || gBattleMons[bank].hp == 0)
		gBattlescriptCurrInstr = T2_READ_PTR(gBattlescriptCurrInstr + 9);
	else
	{
		DoFormChange(bank, targetSpecies, reloadType, reloadStats, reloadAbility);
		gBattlescriptCurrInstr += 13;
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

//tryactivateswitchinability
void atkFF1B_tryactivateswitchinability(void)
{
	u8 bank = GetBankForBattleScript(gBattlescriptCurrInstr[1]);
	gBattlescriptCurrInstr += 2;
	AbilityBattleEffects(ABILITYEFFECT_ON_SWITCHIN, bank, 0, 0, 0);
}

//atkFF1C - atkFF1E: Trainer Sliding

const u16 gFlowerShieldStringIds[] =
{
	STRINGID_PKMNAVOIDEDATTACK,
	STRINGID_PKMNPROTECTEDITSELF,
	STRINGID_ITDOESNTAFFECT,
	STRINGID_STATSWONTINCREASE2,
};

//flowershieldlooper PLUS_MINUS SUCCESS_ADDRESS FAIL_ADDRESS
void atkFF1F_flowershieldlooper(void)
{
	bool8 plusMinus = gBattlescriptCurrInstr[1];
	u8 battlerCount = (plusMinus) ? gBattlersCount / 2 : gBattlersCount;

	for (; gBattleCommunication[0] < battlerCount; ++gBattleCommunication[0])
	{
		u8 bank = gBanksByTurnOrder[gBattleCommunication[0]];

		if (plusMinus)
		{
			if (gBattleCommunication[0] == 0)
				bank = gBankAttacker;
			else
				bank = PARTNER(gBankAttacker);
		}

		if (BATTLER_ALIVE(bank))
		{
			gBankTarget = bank;
			++gBattleCommunication[0];

			if (IsMaxGuardUp(bank))
			{
				gBattleCommunication[MULTISTRING_CHOOSER] = 1; //Protected itself
				gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 6);
				gNewBS->StompingTantrumTimers[gBankAttacker] = 2;
			}
			else if (MoveBlockedBySubstitute(gCurrentMove, gBankAttacker, bank))
			{
				gBattleCommunication[MULTISTRING_CHOOSER] = 2; //No effect
				gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 6);
				gNewBS->StompingTantrumTimers[gBankAttacker] = 2;
			}
			else if (BATTLER_SEMI_INVULNERABLE(bank) && ABILITY(gBankAttacker) != ABILITY_NOGUARD && ABILITY(bank) != ABILITY_NOGUARD)
			{
				gBattleCommunication[MULTISTRING_CHOOSER] = 0; //Avoided attack
				gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 6);
			}
			else if ((!plusMinus && IsOfType(bank, TYPE_GRASS))
			|| (plusMinus && (ABILITY(bank) == ABILITY_PLUS || ABILITY(bank) == ABILITY_MINUS)))
			{
				gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 2);
				gMoveResultFlags = 0;
			}
			else
			{
				gBattleCommunication[MULTISTRING_CHOOSER] = 2; //No effect
				gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 6);
				gNewBS->StompingTantrumTimers[gBankAttacker] = 2;
			}

			return;
		}
	}

	gBankTarget = gBankAttacker;
	gBattlescriptCurrInstr += 10;
}

//jumpifprotectedbycraftyshield BANK ROM_ADDRESS
void atkFF20_jumpifprotectedbycraftyshield(void)
{
	if (CheckCraftyShield(GetBankForBattleScript(gBattlescriptCurrInstr[1])))
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 2);
	else
		gBattlescriptCurrInstr += 5;
}

//tryspectralthiefsteal SUCCESS_ADDRESS
void atkFF21_tryspectralthiefsteal(void)
{
	s8 increment = 1;
	bool8 success = FALSE;
	u8 atkAbility = ABILITY(gBankAttacker);

	for (int i = 0; i < BATTLE_STATS_NO-1; ++i)
	{
		switch (atkAbility) {
			case ABILITY_SIMPLE:
				increment = 2;
				break;

			case ABILITY_CONTRARY:
				increment = -1;
		}

		if (atkAbility == ABILITY_CONTRARY)
		{
			while (gBattleMons[gBankTarget].statStages[i] > 6 && gBattleMons[gBankAttacker].statStages[i] > 0)
			{
				success = TRUE;
				gBattleMons[gBankTarget].statStages[i] -= 1;
				gBattleMons[gBankAttacker].statStages[i] += increment;
				
				gNewBS->statFellThisTurn[gBankAttacker] = TRUE;
				gNewBS->statFellThisRound[gBankAttacker] = TRUE;
			}
		}
		else
		{
			while (gBattleMons[gBankTarget].statStages[i] > 6 && gBattleMons[gBankAttacker].statStages[i] < 12)
			{
				success = TRUE;
				gBattleMons[gBankTarget].statStages[i] -= 1;
				gBattleMons[gBankAttacker].statStages[i] += increment;

				if (gBattleMons[gBankAttacker].statStages[i] > STAT_STAGE_MAX)
					gBattleMons[gBankAttacker].statStages[i] = STAT_STAGE_MAX;
				
				gNewBS->statRoseThisRound[gBankAttacker] = TRUE;
			}
		}
	}

	if (success)
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
	else
		gBattlescriptCurrInstr += 5;
}

//prefaintmoveendeffects ARG NULL_BYTE
void atkFE_prefaintmoveendeffects(void)
{
	if (gBattleExecBuffer) return;

	u32 effect = FALSE;
	u8 arg1 = gBattlescriptCurrInstr[1];

	do
	{
		switch (gNewBS->preFaintEffectsState)
		{
		case FAINT_SET_UP: //For Emergency Exit to use later
			if (gNewBS->MultiHitOn)
				gNewBS->turnDamageTaken[gBankTarget] += gHpDealt; //Total up damage taken
			else
				gNewBS->turnDamageTaken[gBankTarget] = gHpDealt;

			gNewBS->totalDamageGiven += gHpDealt;
			gNewBS->ResultFlags[gBankTarget] = gMoveResultFlags;
			gNewBS->preFaintEffectsState++;
			break;

		case FAINT_ATTACKER_ABILITIES:
			if (arg1 != ARG_IN_FUTURE_ATTACK
			&& TOOK_DAMAGE(gBankTarget)
			&& MOVE_HAD_EFFECT
			&& gBattleMons[gBankTarget].hp
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
			gNewBS->preFaintEffectsState++;
			break;

		case FAINT_ADVERSE_PROTECTION:
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
	
			gNewBS->preFaintEffectsState++;
			break;

		case FAINT_RAGE: // rage check
			if (gBattleMons[gBankTarget].status2 & STATUS2_RAGE
			&& gBattleMons[gBankTarget].hp
			&& gBankAttacker != gBankTarget
			&& SIDE(gBankAttacker) != SIDE(gBankTarget)
			&& MOVE_HAD_EFFECT
			&& TOOK_DAMAGE(gBankTarget)
			&& SPLIT(gCurrentMove) != SPLIT_STATUS
			&& STAT_CAN_RISE(gBankTarget, STAT_ATK))
			{
				gBattleMons[gBankTarget].statStages[STAT_ATK - 1]++;
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_RageIsBuilding;
				effect = TRUE;
			}
			gNewBS->preFaintEffectsState++;
			break;

		case FAINT_SYNCHRONIZE_TARGET: // target synchronize
			if (gCurrentMove != MOVE_PSYCHOSHIFT || !MOVE_HAD_EFFECT) //The lazy way of taking care of Psycho Shift Status Transfer->Synchronize->Heal Status
			{
				if (AbilityBattleEffects(ABILITYEFFECT_SYNCHRONIZE, gBankTarget, 0, 0, 0))
					effect = TRUE;
			}
			gNewBS->preFaintEffectsState++;
			break;

		case FAINT_BEAK_BLAST_BURN:
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

			gNewBS->preFaintEffectsState++;
			break;

		case FAINT_SYNCHRONIZE_ATTACKER: // attacker synchronize
		case FAINT_SYNCHRONIZE_ATTACKER_2:
			if (AbilityBattleEffects(ABILITYEFFECT_SYNCHRONIZE, gBankAttacker, 0, 0, 0))
				effect = TRUE;
			gNewBS->preFaintEffectsState++;
			break;

		case FAINT_MOVE_END_ABILITIES: // Such as abilities activating on contact(Poison Spore, Rough Skin, etc.).
			if (AbilityBattleEffects(ABILITYEFFECT_CONTACT, gBankTarget, 0, 0, 0))
				effect = TRUE;
			gNewBS->preFaintEffectsState++;
			break;

		case FAINT_ITEM_EFFECTS_CONTACT_TARGET:
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
			gNewBS->preFaintEffectsState++;
			break;

		case FAINT_COUNT:
			break;
		}
	} while (gNewBS->preFaintEffectsState != FAINT_COUNT && effect == FALSE);

	if (gNewBS->preFaintEffectsState == FAINT_COUNT && effect == FALSE)
		gBattlescriptCurrInstr += 3;
}

// faintpokemonaftermove NULL_BYTE NULL_WORD
void atkFF23_faintpokemonaftermove(void)
{
	gActiveBattler = gBankTarget;

	if (!(gAbsentBattlerFlags & gBitTable[gActiveBattler])
	&& gBattleMons[gActiveBattler].hp == 0)
	{
		if (TryDoBenjaminButterfree(3))
			return;

		gNewBS->lastFainted = gActiveBattler;
		gHitMarker |= HITMARKER_FAINTED(gActiveBattler);
		BattleScriptPush(gBattlescriptCurrInstr + 3);

		if (IsCatchableRaidBattle() && gActiveBattler == BANK_RAID_BOSS)
			gBattlescriptCurrInstr = BattleScript_FaintRaidTarget;
		else
			gBattlescriptCurrInstr = BattleScript_FaintTarget;

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
			gBattleResults.lastOpponentSpecies = GetBankPartyData(gActiveBattler)->species;
		}

		gNewBS->RetaliateCounters[SIDE(gActiveBattler)] = 2;

		if ((gHitMarker & HITMARKER_DESTINYBOND)
		&& !IsDynamaxed(gBankAttacker)
		&& gBattleMons[gBankAttacker].hp != 0)
		{
			gHitMarker &= ~(HITMARKER_DESTINYBOND);
			const u8* backupScript = gBattlescriptCurrInstr;
			gBattlescriptCurrInstr = BattleScript_DestinyBondTakesLife;
			BattleScriptPushCursor();
			gBattleMoveDamage = gBattleMons[gBankAttacker].hp;
			gBattlescriptCurrInstr = backupScript;
		}

		if ((gStatuses3[gBankTarget] & STATUS3_GRUDGE)
		&& !(gHitMarker & HITMARKER_GRUDGE)
		&& SIDE(gBankAttacker) != SIDE(gBankTarget)
		&& gBattleMons[gBankAttacker].hp != 0
		&& gCurrentMove != MOVE_STRUGGLE)
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
		gBattlescriptCurrInstr += 3;
	}
}

//jumpifattackeralreadydiddamage ROM_ADDRESS
void atkFF24_jumpifattackeralreadydiddamage(void)
{
	if (gNewBS->AttackerDidDamageAtLeastOnce)
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
	else
		gBattlescriptCurrInstr += 5;
}

//jumpifterrainandgrounded TERRAIN_ID BANK ROM_ADDRESS
void atkFF25_jumpifterrainandgrounded(void)
{
	if (gTerrainType == gBattlescriptCurrInstr[1]
	&&  CheckGrounding(GetBankForBattleScript(gBattlescriptCurrInstr[2])))
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 3);
	else
		gBattlescriptCurrInstr += 7;
}

//attackstringnoprotean
void atkFF26_attackstringnoprotean(void)
{
	if (gBattleExecBuffer) return;

	u8 moveType = gBattleStruct->dynamicMoveType;

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

			if (!CheckTableForMove(gCurrentMove, gMovesThatCallOtherMoves))
			{
				u8 chargingBonus = 20 * gNewBS->metronomeItemBonus[gBankAttacker];
				if (gLastPrintedMoves[gBankAttacker] == gCurrentMove)
					gNewBS->MetronomeCounter[gBankAttacker] = MathMin(100, gNewBS->MetronomeCounter[gBankAttacker] + 20 + chargingBonus);
				else
					gNewBS->MetronomeCounter[gBankAttacker] = 0 + chargingBonus;
			}
		}

		gHitMarker |= HITMARKER_ATTACKSTRING_PRINTED;
	}
	else
	{
		gBattlescriptCurrInstr++;
		gBattleCommunication[MSG_DISPLAY] = 0;
	}
}

//tryactivateprotean
void atkFF27_tryactivateprotean(void)
{
	u8 moveType = gBattleStruct->dynamicMoveType;

	if (ABILITY(gBankAttacker) == ABILITY_PROTEAN
	&& !(gMoveResultFlags & MOVE_RESULT_FAILED)
	&& gCurrentMove != MOVE_STRUGGLE
	&& !(gHitMarker & HITMARKER_UNABLE_TO_USE_MOVE)
	&& !(CheckTableForMove(gCurrentMove, gMovesThatCallOtherMoves)))
	{
		if (gBattleMons[gBankAttacker].type1 != moveType
		||  gBattleMons[gBankAttacker].type2 != moveType
		|| (gBattleMons[gBankAttacker].type3 != moveType && gBattleMons[gBankAttacker].type3 != TYPE_BLANK))
		{
			BattleScriptPush(gBattlescriptCurrInstr + 1);
			gBattlescriptCurrInstr = BattleScript_AbilityChangedType;
			return;
		}
	}

	gBattlescriptCurrInstr++;
}

//jumpifweight BANK PREDICATE WEIGHT ROM_OFFSET
void atkFF28_jumpifweight(void)
{
	bool8 ret = FALSE;
	u8 bank = GetBankForBattleScript(gBattlescriptCurrInstr[1]);
	u8 predicate = gBattlescriptCurrInstr[2];
	u16 value = T1_READ_16(gBattlescriptCurrInstr + 3);
	const u8* ptr = T1_READ_PTR(gBattlescriptCurrInstr + 5);

	u32 weight = GetActualSpeciesWeight(SPECIES(bank), ABILITY(bank), ITEM_EFFECT(bank), bank, TRUE);

	switch (predicate) {
		case CMP_EQUAL:
			if (value == weight)
				ret = TRUE;
			break;
		case CMP_NOT_EQUAL:
			if (value != weight)
				ret = TRUE;
			break;
		case CMP_GREATER_THAN:
			if (weight > value)
				ret = TRUE;
			break;
		case CMP_LESS_THAN:
			if (weight < value)
				ret = TRUE;
			break;
		case CMP_COMMON_BITS:
			if (value & weight)
				ret = TRUE;
			break;
		case CMP_NO_COMMON_BITS:
			if (!(value & weight))
				ret = TRUE;
			break;
	}

	if (ret)
		gBattlescriptCurrInstr = ptr;
	else
		gBattlescriptCurrInstr += 9;
}

/*
Doesn't Affect (Spore, Minior Shield etc.)
Attack Misses
Already Asleep
Already Has Status (Failed)
Safeguard?
Misty Terrain / Electric Terrain
Flower Veil
Sweet Veil
Ability Protects
*/

//trysetsleep BANK FAIL_ADDRESS
void atkFF29_trysetsleep(void)
{
	u8 bank = GetBankForBattleScript(gBattlescriptCurrInstr[1]);
	u8* ptr = T1_READ_PTR(gBattlescriptCurrInstr + 2);
	bool8 fail = FALSE;

	if (BATTLER_SEMI_INVULNERABLE(bank) && !CanHitSemiInvulnerableTarget(gBankAttacker, bank, gCurrentMove))
	{
		gMoveResultFlags |= MOVE_RESULT_MISSED;
		gBattlescriptCurrInstr = BattleScript_PauseResultMessage;
		return;
	}
	else if (AbilityBattleEffects(ABILITYEFFECT_ABSORBING, bank, 0, 0, gCurrentMove))
	{
		return;
	}
	else if (CheckTableForMove(gCurrentMove, gPowderMoves) && TypeCalc(gCurrentMove, gBankAttacker, bank, NULL, FALSE) & MOVE_RESULT_DOESNT_AFFECT_FOE)
	{
		gMoveResultFlags |= MOVE_RESULT_DOESNT_AFFECT_FOE;
		gBattlescriptCurrInstr = BattleScript_PauseResultMessage;
		return;
	}
	else if (gBattleMons[bank].status1 & STATUS1_SLEEP)
	{
		gBattleStringLoader = gText_TargetAlreadyAsleep;
		fail = TRUE;
	}
	else if (gBattleMons[bank].status1 != STATUS1_NONE)
	{
		gBattleStringLoader = gText_TargetAlreadyHasStatusCondition; //String not in official games; officially "But it failed!"
		fail = TRUE;
	}
	else if (ABILITY(gBankAttacker) != ABILITY_INFILTRATOR && BankSideHasSafeguard(bank))
	{
		gBattleStringLoader = gText_TeamProtectedBySafeguard;
		fail = TRUE;
	}
	else if (CheckGrounding(bank) && gTerrainType == MISTY_TERRAIN)
	{
		gBattleStringLoader = gText_TargetWrappedInMistyTerrain;
		fail = TRUE;
	}
	else if (CheckGrounding(bank) && gTerrainType == ELECTRIC_TERRAIN)
	{
		gBattleStringLoader = gText_TargetWrappedInElectricTerrain;
		fail = TRUE;
	}
	else if (IsOfType(bank, TYPE_GRASS) && ABILITY(bank) == ABILITY_FLOWERVEIL)
	{
		gBattleScripting.bank = bank;
		gBattlescriptCurrInstr = BattleScript_TeamProtectedByFlowerVeil;
		return;
	}
	else if (IsOfType(bank, TYPE_GRASS) && IS_DOUBLE_BATTLE && ABILITY(PARTNER(bank)) == ABILITY_FLOWERVEIL)
	{
		gBattleScripting.bank = PARTNER(bank);
		gBattlescriptCurrInstr = BattleScript_TeamProtectedByFlowerVeil;
		return;
	}
	else if (ABILITY(bank) == ABILITY_SWEETVEIL)
	{
		gBattleScripting.bank = bank;
		gBattlescriptCurrInstr = BattleScript_TeamProtectedBySweetVeil;
		return;
	}
	else if (IS_DOUBLE_BATTLE && ABILITY(PARTNER(bank)) == ABILITY_SWEETVEIL)
	{
		gBattleScripting.bank = PARTNER(bank);
		gBattlescriptCurrInstr = BattleScript_TeamProtectedBySweetVeil;
		return;
	}
	else if (IsUproarBeingMade())
	{
		gBattleScripting.bank = bank;
		gBattleStringLoader = gText_CantFallAsleepDuringUproar;
		fail = TRUE;
	}

	if (!fail)
	{
		switch (ABILITY(bank)) {
			case ABILITY_INSOMNIA:
			case ABILITY_VITALSPIRIT:
				gBattlescriptCurrInstr = BattleScript_TargetStayedAwakeUsingAbility;
				return;
			case ABILITY_LEAFGUARD:
				if (WEATHER_HAS_EFFECT && gBattleWeather & WEATHER_SUN_ANY && ITEM_EFFECT(bank) != ITEM_EFFECT_UTILITY_UMBRELLA)
				{
					gBattlescriptCurrInstr = BattleScript_ProtectedByAbility;
					return;
				}
				break;
			case ABILITY_COMATOSE:
				gBattlescriptCurrInstr = BattleScript_ProtectedByAbility;
				return;
			case ABILITY_SHIELDSDOWN:
				if (SPECIES(bank) == SPECIES_MINIOR_SHIELD)
				{
					gBattlescriptCurrInstr = BattleScript_ProtectedByAbility;
					return;
				}
		}

		if (!fail && DoesSleepClausePrevent(bank))
		{
			gBattleStringLoader = gText_SleepClausePrevents;
			fail = TRUE;
		}
	}

	if (fail)
	{
		gMoveResultFlags |= MOVE_RESULT_DOESNT_AFFECT_FOE;
		gBattlescriptCurrInstr = ptr;
	}
	else
		gBattlescriptCurrInstr += 6;
}

void atkD7_setyawn(void)
{
	u8 bank = gBankTarget;
	u8* ptr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
	bool8 fail = FALSE;

	if (BATTLER_SEMI_INVULNERABLE(bank) && !CanHitSemiInvulnerableTarget(gBankAttacker, bank, gCurrentMove))
	{
		gMoveResultFlags |= MOVE_RESULT_MISSED;
		gBattlescriptCurrInstr = BattleScript_PauseResultMessage;
		return;
	}
	else if (gBattleMons[bank].status1 & STATUS1_SLEEP)
	{
		gBattleStringLoader = gText_TargetAlreadyAsleep;
		fail = TRUE;
	}
	else if (gBattleMons[bank].status1 != STATUS1_NONE)
	{
		gBattleStringLoader = gText_TargetAlreadyHasStatusCondition; //String not in official games; officially "But it failed!"
		fail = TRUE;
	}
	else if (ABILITY(gBankAttacker) != ABILITY_INFILTRATOR && BankSideHasSafeguard(bank))
	{
		gBattleStringLoader = gText_TeamProtectedBySafeguard;
		fail = TRUE;
	}
	/*else if (CheckGrounding(bank) && gTerrainType == MISTY_TERRAIN) //Misty Terrain doesn't cause yawn to fail
	{
		gBattleStringLoader = gText_TargetWrappedInMistyTerrain;
		fail = TRUE;
	}*/
	else if (CheckGrounding(bank) && gTerrainType == ELECTRIC_TERRAIN)
	{
		gBattleStringLoader = gText_TargetWrappedInElectricTerrain;
		fail = TRUE;
	}
	else if (IsOfType(bank, TYPE_GRASS) && ABILITY(bank) == ABILITY_FLOWERVEIL)
	{
		gBattleScripting.bank = bank;
		gBattlescriptCurrInstr = BattleScript_TeamProtectedByFlowerVeil;
		return;
	}
	else if (IsOfType(bank, TYPE_GRASS) && IS_DOUBLE_BATTLE && ABILITY(PARTNER(bank)) == ABILITY_FLOWERVEIL)
	{
		gBattleScripting.bank = PARTNER(bank);
		gBattlescriptCurrInstr = BattleScript_TeamProtectedByFlowerVeil;
		return;
	}
	else if (ABILITY(bank) == ABILITY_SWEETVEIL)
	{
		gBattleScripting.bank = bank;
		gBattlescriptCurrInstr = BattleScript_TeamProtectedBySweetVeil;
		return;
	}
	else if (IS_DOUBLE_BATTLE && ABILITY(PARTNER(bank)) == ABILITY_SWEETVEIL)
	{
		gBattleScripting.bank = PARTNER(bank);
		gBattlescriptCurrInstr = BattleScript_TeamProtectedBySweetVeil;
		return;
	}
	/*else if (IsUproarBeingMade())
	{
		gBattleScripting.bank = bank;
		gBattleStringLoader = gText_CantFallAsleepDuringUproar;
		fail = TRUE;
	}*/

	if (!fail)
	{
		switch (ABILITY(bank)) {
			case ABILITY_INSOMNIA:
			case ABILITY_VITALSPIRIT:
				gBattlescriptCurrInstr = BattleScript_TargetStayedAwakeUsingAbility;
				return;
			case ABILITY_LEAFGUARD:
				if (WEATHER_HAS_EFFECT && gBattleWeather & WEATHER_SUN_ANY && ITEM_EFFECT(bank) != ITEM_EFFECT_UTILITY_UMBRELLA)
				{
					gBattlescriptCurrInstr = BattleScript_ProtectedByAbility;
					return;
				}
				break;
			case ABILITY_COMATOSE:
				gBattlescriptCurrInstr = BattleScript_ProtectedByAbility;
				return;
			case ABILITY_SHIELDSDOWN:
				if (SPECIES(bank) == SPECIES_MINIOR_SHIELD)
				{
					gBattlescriptCurrInstr = BattleScript_ProtectedByAbility;
					return;
				}
		}

		if (!fail && gStatuses3[gBattlerTarget] & STATUS3_YAWN)
		{
			gBattleStringLoader = gText_AlreadyDrowsy;
			fail = TRUE;
		}
	}

	if (fail)
	{
		gMoveResultFlags |= MOVE_RESULT_DOESNT_AFFECT_FOE;
		gBattlescriptCurrInstr = ptr;
	}
	else
		gBattlescriptCurrInstr += 5;
}

void ActuallySetYawn(void)
{
	gStatuses3[gBankTarget] |= 0x1000;
}

//trysetparalysis BANK FAIL_ADDRESS
void atkFF2A_trysetparalysis(void)
{
	u8 bank = GetBankForBattleScript(gBattlescriptCurrInstr[1]);
	u8* ptr = T1_READ_PTR(gBattlescriptCurrInstr + 2);
	bool8 fail = FALSE;

	if (BATTLER_SEMI_INVULNERABLE(bank) && !CanHitSemiInvulnerableTarget(gBankAttacker, bank, gCurrentMove))
	{
		gMoveResultFlags |= MOVE_RESULT_MISSED;
		gBattlescriptCurrInstr = BattleScript_PauseResultMessage;
		return;
	}
	else if (AbilityBattleEffects(ABILITYEFFECT_ABSORBING, bank, 0, 0, gCurrentMove))
	{
		return;
	}
	else if ((CheckTableForMove(gCurrentMove, gPowderMoves) || gCurrentMove == MOVE_THUNDERWAVE)
	&& TypeCalc(gCurrentMove, gBankAttacker, bank, NULL, FALSE) & MOVE_RESULT_DOESNT_AFFECT_FOE)
	{
		gMoveResultFlags |= MOVE_RESULT_DOESNT_AFFECT_FOE;
		gBattlescriptCurrInstr = BattleScript_PauseResultMessage;
		return;
	}
	else if (IsOfType(bank, TYPE_ELECTRIC))
	{
		gMoveResultFlags |= MOVE_RESULT_DOESNT_AFFECT_FOE;
		gBattlescriptCurrInstr = BattleScript_PauseResultMessage;
		return;
	}
	else if (gBattleMons[bank].status1 & STATUS1_PARALYSIS)
	{
		gBattleStringLoader = gText_TargetAlreadyParalyzed;
		fail = TRUE;
	}
	else if (gBattleMons[bank].status1 != STATUS1_NONE)
	{
		gBattleStringLoader = gText_TargetAlreadyHasStatusCondition; //String not in official games; officially "But it failed!"
		fail = TRUE;
	}
	else if (ABILITY(gBankAttacker) != ABILITY_INFILTRATOR && BankSideHasSafeguard(bank))
	{
		gBattleStringLoader = gText_TeamProtectedBySafeguard;
		fail = TRUE;
	}
	else if (CheckGrounding(bank) && gTerrainType == MISTY_TERRAIN)
	{
		gBattleStringLoader = gText_TargetWrappedInMistyTerrain;
		fail = TRUE;
	}
	else if (IsOfType(bank, TYPE_GRASS) && ABILITY(bank) == ABILITY_FLOWERVEIL)
	{
		gBattleScripting.bank = bank;
		gBattlescriptCurrInstr = BattleScript_TeamProtectedByFlowerVeil;
		return;
	}
	else if (IsOfType(bank, TYPE_GRASS) && IS_DOUBLE_BATTLE && ABILITY(PARTNER(bank)) == ABILITY_FLOWERVEIL)
	{
		gBattleScripting.bank = PARTNER(bank);
		gBattlescriptCurrInstr = BattleScript_TeamProtectedByFlowerVeil;
		return;
	}

	if (!fail)
	{
		switch (ABILITY(bank)) {
			case ABILITY_LEAFGUARD:
				if (WEATHER_HAS_EFFECT && gBattleWeather & WEATHER_SUN_ANY && ITEM_EFFECT(bank) != ITEM_EFFECT_UTILITY_UMBRELLA)
				{
					gBattlescriptCurrInstr = BattleScript_ProtectedByAbility;
					return;
				}
				break;
			case ABILITY_LIMBER:
			case ABILITY_COMATOSE:
				gBattlescriptCurrInstr = BattleScript_ProtectedByAbility;
				return;
			case ABILITY_SHIELDSDOWN:
				if (SPECIES(bank) == SPECIES_MINIOR_SHIELD)
				{
					gBattlescriptCurrInstr = BattleScript_ProtectedByAbility;
					return;
				}
		}
	}

	if (fail)
	{
		gMoveResultFlags |= MOVE_RESULT_DOESNT_AFFECT_FOE;
		gBattlescriptCurrInstr = ptr;
	}
	else
		gBattlescriptCurrInstr += 6;
}

//trysetburn BANK FAIL_ADDRESS
void atkFF2B_trysetburn(void)
{
	u8 bank = GetBankForBattleScript(gBattlescriptCurrInstr[1]);
	u8* ptr = T1_READ_PTR(gBattlescriptCurrInstr + 2);
	bool8 fail = FALSE;

	if (BATTLER_SEMI_INVULNERABLE(bank) && !CanHitSemiInvulnerableTarget(gBankAttacker, bank, gCurrentMove))
	{
		gMoveResultFlags |= MOVE_RESULT_MISSED;
		gBattlescriptCurrInstr = BattleScript_PauseResultMessage;
		return;
	}
	else if (AbilityBattleEffects(ABILITYEFFECT_ABSORBING, bank, 0, 0, gCurrentMove))
	{
		return;
	}
	if (IsOfType(bank, TYPE_FIRE))
	{
		gMoveResultFlags |= MOVE_RESULT_DOESNT_AFFECT_FOE;
		gBattlescriptCurrInstr = BattleScript_PauseResultMessage;
		return;
	}
	else if (gBattleMons[bank].status1 & STATUS1_BURN)
	{
		gBattleStringLoader = gText_TargetAlreadyBurned;
		fail = TRUE;
	}
	else if (gBattleMons[bank].status1 != STATUS1_NONE)
	{
		gBattleStringLoader = gText_TargetAlreadyHasStatusCondition; //String not in official games; officially "But it failed!"
		fail = TRUE;
	}
	else if (ABILITY(gBankAttacker) != ABILITY_INFILTRATOR && BankSideHasSafeguard(bank))
	{
		gBattleStringLoader = gText_TeamProtectedBySafeguard;
		fail = TRUE;
	}
	else if (CheckGrounding(bank) && gTerrainType == MISTY_TERRAIN)
	{
		gBattleStringLoader = gText_TargetWrappedInMistyTerrain;
		fail = TRUE;
	}
	else if (IsOfType(bank, TYPE_GRASS) && ABILITY(bank) == ABILITY_FLOWERVEIL)
	{
		gBattleScripting.bank = bank;
		gBattlescriptCurrInstr = BattleScript_TeamProtectedByFlowerVeil;
		return;
	}
	else if (IsOfType(bank, TYPE_GRASS) && IS_DOUBLE_BATTLE && ABILITY(PARTNER(bank)) == ABILITY_FLOWERVEIL)
	{
		gBattleScripting.bank = PARTNER(bank);
		gBattlescriptCurrInstr = BattleScript_TeamProtectedByFlowerVeil;
		return;
	}

	if (!fail)
	{
		switch (ABILITY(bank)) {
			case ABILITY_LEAFGUARD:
				if (WEATHER_HAS_EFFECT && gBattleWeather & WEATHER_SUN_ANY && ITEM_EFFECT(bank) != ITEM_EFFECT_UTILITY_UMBRELLA)
				{
					gBattlescriptCurrInstr = BattleScript_ProtectedByAbility;
					return;
				}
				break;
			case ABILITY_WATERVEIL:
			case ABILITY_WATERBUBBLE:
			case ABILITY_COMATOSE:
				gBattlescriptCurrInstr = BattleScript_ProtectedByAbility;
				return;
			case ABILITY_SHIELDSDOWN:
				if (SPECIES(bank) == SPECIES_MINIOR_SHIELD)
				{
					gBattlescriptCurrInstr = BattleScript_ProtectedByAbility;
					return;
				}
				break;
		}
	}

	if (fail)
	{
		gMoveResultFlags |= MOVE_RESULT_DOESNT_AFFECT_FOE;
		gBattlescriptCurrInstr = ptr;
	}
	else
		gBattlescriptCurrInstr += 6;
}

//trysetpoison BANK FAIL_ADDRESS
void atkFF2C_trysetpoison(void)
{
	u8 bank = GetBankForBattleScript(gBattlescriptCurrInstr[1]);
	u8* ptr = T1_READ_PTR(gBattlescriptCurrInstr + 2);
	bool8 fail = FALSE;

	if (BATTLER_SEMI_INVULNERABLE(bank) && !CanHitSemiInvulnerableTarget(gBankAttacker, bank, gCurrentMove))
	{
		gMoveResultFlags |= MOVE_RESULT_MISSED;
		gBattlescriptCurrInstr = BattleScript_PauseResultMessage;
		return;
	}
	else if (AbilityBattleEffects(ABILITYEFFECT_ABSORBING, bank, 0, 0, gCurrentMove))
	{
		return;
	}
	else if (ABILITY(gBankAttacker) != ABILITY_CORROSION
	&& (IsOfType(bank, TYPE_POISON) || IsOfType(bank, TYPE_STEEL)))
	{
		gMoveResultFlags |= MOVE_RESULT_DOESNT_AFFECT_FOE;
		gBattlescriptCurrInstr = BattleScript_PauseResultMessage;
		return;
	}
	else if (gBattleMons[bank].status1 & STATUS1_PSN_ANY)
	{
		gBattleStringLoader = gText_TargetAlreadyPoisoned;
		fail = TRUE;
	}
	else if (gBattleMons[bank].status1 != STATUS1_NONE)
	{
		gBattleStringLoader = gText_TargetAlreadyHasStatusCondition; //String not in official games; officially "But it failed!"
		fail = TRUE;
	}
	else if (ABILITY(gBankAttacker) != ABILITY_INFILTRATOR && BankSideHasSafeguard(bank))
	{
		gBattleStringLoader = gText_TeamProtectedBySafeguard;
		fail = TRUE;
	}
	else if (CheckGrounding(bank) && gTerrainType == MISTY_TERRAIN)
	{
		gBattleStringLoader = gText_TargetWrappedInMistyTerrain;
		fail = TRUE;
	}
	else if (IsOfType(bank, TYPE_GRASS) && ABILITY(bank) == ABILITY_FLOWERVEIL)
	{
		gBattleScripting.bank = bank;
		gBattlescriptCurrInstr = BattleScript_TeamProtectedByFlowerVeil;
		return;
	}
	//Put Pastel Veil here
	else if (IsOfType(bank, TYPE_GRASS) && IS_DOUBLE_BATTLE && ABILITY(PARTNER(bank)) == ABILITY_FLOWERVEIL)
	{
		gBattleScripting.bank = PARTNER(bank);
		gBattlescriptCurrInstr = BattleScript_TeamProtectedByFlowerVeil;
		return;
	}
	//Put Pastel Veil here

	if (!fail)
	{
		switch (ABILITY(bank)) {
			case ABILITY_LEAFGUARD:
				if (WEATHER_HAS_EFFECT && gBattleWeather & WEATHER_SUN_ANY && ITEM_EFFECT(bank) != ITEM_EFFECT_UTILITY_UMBRELLA)
				{
					gBattlescriptCurrInstr = BattleScript_ProtectedByAbility;
					return;
				}
				break;
			case ABILITY_IMMUNITY:
			case ABILITY_COMATOSE:
				gBattlescriptCurrInstr = BattleScript_ProtectedByAbility;
				return;
			case ABILITY_SHIELDSDOWN:
				if (SPECIES(bank) == SPECIES_MINIOR_SHIELD)
				{
					gBattlescriptCurrInstr = BattleScript_ProtectedByAbility;
					return;
				}
		}
	}

	if (fail)
	{
		gMoveResultFlags |= MOVE_RESULT_DOESNT_AFFECT_FOE;
		gBattlescriptCurrInstr = ptr;
	}
	else
		gBattlescriptCurrInstr += 6;
}

//addindicatorforattackerswitchineffects
void atkFF2D_addindicatorforattackerswitchineffects(void) //Used for when the game asks you if you want to switch to counter what the foe is sending in
{
	gNewBS->doSwitchInEffects |= gBitTable[gBankAttacker];
	gBattlescriptCurrInstr += 1;
}

//setmoveeffect2
void atkFF2E_setmoveeffect2(void)
{
	gBattlescriptCurrInstr += 1;
	SetMoveEffect2();
}

//setmaxmoveeffect
//void atkFF2F_setmaxmoveeffect

//jumpifdynamaxed BANK ROM_OFFSET
void atkFF30_jumpifdynamaxed(void)
{
	u8 bank = GetBankForBattleScript(gBattlescriptCurrInstr[1]);
	const u8* ptr = T1_READ_PTR(gBattlescriptCurrInstr + 2);

	if (IsDynamaxed(bank))
		gBattlescriptCurrInstr = ptr;
	else
		gBattlescriptCurrInstr += 6;
}

//jumpifraidboss BANK ROM_OFFSET
void atkFF31_jumpifraidboss(void)
{
	u8 bank = GetBankForBattleScript(gBattlescriptCurrInstr[1]);
	const u8* ptr = T1_READ_PTR(gBattlescriptCurrInstr + 2);

	if (IsRaidBattle() && bank == BANK_RAID_BOSS)
		gBattlescriptCurrInstr = ptr;
	else
		gBattlescriptCurrInstr += 6;
}

//recycleberry BANKish FAIL_OFFSET
void atkFF32_recycleberry(void)
{
	u8 bank = (gBattlescriptCurrInstr[1] == BS_GET_ATTACKER) ? gBankAttacker : PARTNER(gBankAttacker);
	u16 item = SAVED_CONSUMED_ITEMS(bank);

	if (item != ITEM_NONE && IsBerry(item) && ITEM(bank) == ITEM_NONE)
	{
		RecycleItem(bank);
		gBattlescriptCurrInstr += 6;
	}
	else
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 2);
}

//seteffectprimaryscriptingbank
void atkFF33_SetEffectPrimaryScriptingBank(void)
{
	u8 backupBank = gBankTarget;
	gBankTarget = gBattleScripting.bank;
	SetMoveEffect(TRUE, (gBattleCommunication[MOVE_EFFECT_BYTE] & MOVE_EFFECT_CERTAIN) != 0);
	gBankTarget = backupBank;
}

//canconfuse BANK FAIL_ADDRESS
void atkFF34_canconfuse(void)
{
	u8 bank = GetBankForBattleScript(gBattlescriptCurrInstr[1]);
	u8* ptr = T1_READ_PTR(gBattlescriptCurrInstr + 2);
	bool8 fail = FALSE;

	if (BATTLER_SEMI_INVULNERABLE(bank) && !CanHitSemiInvulnerableTarget(gBankAttacker, bank, gCurrentMove))
	{
		gMoveResultFlags |= MOVE_RESULT_MISSED;
		gBattlescriptCurrInstr = BattleScript_PauseResultMessage;
		return;
	}
	else if (IsConfused(bank))
	{
		gBattleStringLoader = gText_TargetAlreadyConfused;
		fail = TRUE;
	}
	else if (ABILITY(gBankAttacker) != ABILITY_INFILTRATOR && BankSideHasSafeguard(bank))
	{
		gBattleStringLoader = gText_TeamProtectedBySafeguard;
		fail = TRUE;
	}
	else if (CheckGrounding(bank) && gTerrainType == MISTY_TERRAIN)
	{
		gBattleStringLoader = gText_TargetWrappedInMistyTerrain;
		fail = TRUE;
	}

	if (!fail)
	{
		switch (ABILITY(bank)) {
			case ABILITY_OWNTEMPO:
				gBattlescriptCurrInstr = BattleScript_ProtectedByAbility;
				return;
		}

		gBattlescriptCurrInstr += 6;
	}
	else //Fail
	{
		gMoveResultFlags |= MOVE_RESULT_DOESNT_AFFECT_FOE;
		gBattlescriptCurrInstr = ptr;
	}
}

//jumpifmaxchistrikecapped BANK FAIL_ADDRESS
void atkFF35_jumpifmaxchistrikecapped(void)
{
	u8 bank = GetBankForBattleScript(gBattlescriptCurrInstr[1]);
	u8* ptr = T1_READ_PTR(gBattlescriptCurrInstr + 2);

	if (gNewBS->chiStrikeCritBoosts[bank] >= 3)
		gBattlescriptCurrInstr = ptr;
	else
		gBattlescriptCurrInstr += 6;
}
