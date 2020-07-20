#pragma once

#include "../global.h"

/**
 * \file new_bs_commands.h
 * \brief Contains functions that represent newer battle scripting commands past the
 *		  vanilla 248 (starts at atkF8). Functions after atkFF are loaded from a second
 *		  command table using atkFF.
 */

//Exported Functions

//callasm FUNCTION_OFFSET
void atkF8_callasm(void);

//sethalfword RAM_OFFSET HWORD
void atkF9_sethalfword(void);

//setword RAM_OFFSET WORD
void atkFA_setword(void);

//setspecialstatusbit BANK
void atkFB_setspecialstatusbit(void);

//clearspecialstatusbit BANK
void atkFC_clearspecialstatusbit(void);

//jumpifabilitypresenttargetfield ABILITY ROM_OFFSET
void atkFD_jumpifabilitypresenttargetfield(void);

//jumpifspecies BANK SPECIES ROM_OFFSET
void atkFF22_jumpifspecies(void);

void atkFF_callsecondarytable(void);

//cureprimarystatus BANK
void atkFF02_cureprimarystatus(void);

//jumpifpartnerattack BANK MOVE ROM_OFFSET
void atkFF03_jumpifpartnerattack(void);

//setterrain ROM_ADDRESS
void atkFF06_setterrain(void);

//jumpifhelditemeffect BANK ITEM_EFFECT ROM_OFFSET
void atkFF07_jumpifhelditemeffect(void);

//counterclear BANK COUNTER_ID FAIL_OFFSET
void atkFF08_counterclear(void);

//jumpifcounter BANK COUNTER_ID PREDICATE BYTE_TO_COMPARE ROM_OFFSET
void atkFF09_jumpifcounter(void);

//setability BANK ABILITY
void atkFF0A_setability(void);

//jumpiftargetpartner ROM_OFFSET
void atkFF0C_jumpiftargetpartner(void);

//setcounter BANK COUNTER_ID AMOUNT
void atkFF0E_setcounter(void);

//jumpifgrounded BANK ROM_OFFSET
void atkFF0F_jumpifgrounded(void);

//jumpifhelditem BANK ITEM ROM_OFFSET
void atkFF10_jumpifhelditem(void);

//reloadhealthbar BANK
//void atkFF11_reloadhealthbar(void);

//jumpifhealthcomparestomax BANK PREDICATE ROM_OFFSET
void atkFF12_jumpifhealthcomparestomax(void);

//atkFF13_setdamagetobankhealthfraction BANK PERCENT
void atkFF13_setdamagetobankhealthfraction(void);

//jumpiftypepresent TYPE ROM_OFFSET
void atkFF14_jumpiftypepresent(void);

//jumpifstatcanbelowered BANK STAT ROM_ADDRESS
void atkFF15_jumpifstatcanbemodified(void);

//jumpifnoviablemonsleft BANK ROM_ADDRESS
void atkFF16_jumpifnoviablemonsleft(void);

//setsidestatus BANK STATUS
void atkFF17_setsidestatus(void);

//clearsidestatus BANK STATUS
void atkFF18_clearsidestatus(void);

//formchange BANK ORIGIN_SPECIES TARGET_SPECIES RELOAD_TYPE RELOAD_STATS ROM_ADDRESS
void atkFF19_formchange(void);

//jumpifabilitypresentattackerfield ABILITY ROM_OFFSET
void atkFF1A_jumpifabilitypresentattackerfield(void);

//tryactivateswitchinability
void atkFF1B_tryactivateswitchinability(void);

//flowershieldlooper MOVE_EFFECT SUCCESS_ADDRESS FAIL_ADDRESS
void atkFF1F_flowershieldlooper(void);

//jumpifprotectedbycraftyshield BANK ROM_ADDRESS
void atkFF20_jumpifprotectedbycraftyshield(void);

//tryspectralthiefsteal SUCCESS_ADDRESS
void atkFF21_tryspectralthiefsteal(void);

//prefaintmoveendeffects ARG NULL_BYTE
void atkFE_prefaintmoveendeffects(void);

// faintpokemonaftermove NULL_BYTE NULL_WORD
void atkFF23_faintpokemonaftermove(void);

//jumpifattackeralreadydiddamage ROM_ADDRESS
void atkFF24_jumpifattackeralreadydiddamage(void);

//jumpifterrainandgrounded TERRAIN_ID BANK ROM_ADDRESS
void atkFF25_jumpifterrainandgrounded(void);

//attackstringnoprotean
void atkFF26_attackstringnoprotean(void);

//tryactivateprotean
void atkFF27_tryactivateprotean(void);

//jumpifweight BANK PREDICATE WEIGHT ROM_OFFSET
void atkFF28_jumpifweight(void);

//trysetsleep BANK FAIL_ADDRESS
void atkFF29_trysetsleep(void);

//trysetparalysis BANK FAIL_ADDRESS
void atkFF2A_trysetparalysis(void);

//trysetburn BANK FAIL_ADDRESS
void atkFF2B_trysetburn(void);

//trysetpoison BANK FAIL_ADDRESS
void atkFF2C_trysetpoison(void);

//addindicatorforattackerswitchineffects
void atkFF2D_addindicatorforattackerswitchineffects(void);

//setmoveeffect2
void atkFF2E_setmoveeffect2(void);

//Exported Constants
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
	Counters_AuroraVeil, 		//16
	Counters_TarShot,			//17
};
