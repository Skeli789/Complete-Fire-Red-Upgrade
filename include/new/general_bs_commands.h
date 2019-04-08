#pragma once

#include "../../src/defines.h"
#include "../../src/defines_battle.h"

extern u8 RemovedEntryHazardsString[];
extern u8 SpikesLayString[];
extern u8 StealthRockLayString[];
extern u8 ToxicSpikesLayString[];
extern u8 StickyWebLayString[];

enum FaintEffectsStates {
Faint_ClearEffects,
Faint_SoulHeart,
Faint_ReceiverActivate,
Faint_SwitchInAbilities,
Faint_PrimalWeather,
Faint_FormsRevert,
Faint_FormsStats,
Faint_FormsHP,
};

enum
{
 CASTFORM_NO_CHANGE, //0
 CASTFORM_TO_NORMAL, //1
 CASTFORM_TO_FIRE,   //2
 CASTFORM_TO_WATER,  //3
 CASTFORM_TO_ICE,    //4
};


enum CastformForms {
CASTFORM_NORMAL,
CASTFORM_SUN,
CASTFORM_RAIN,
CASTFORM_HAIL
};


u8* EntryHazardsStrings[] = {
SpikesLayString,
StealthRockLayString,
ToxicSpikesLayString,
StickyWebLayString,
};

extern u8 BattleScript_SturdyPreventsOHKO[];
#define BattleScript_SubstituteFade (u8*)  0x81D9526
#define BattleScript_HangedOnMsg (u8*) 0x81D9641
#define BattleScript_OneHitKOMsg (u8*) 0x81D8FE4
extern u8 BattleScript_EnduredMsg[];
#define BattleScript_PrintPayDayMoneyString (u8*) 0x81D911D
#define BattleScript_WrapFree (u8*) 0x81D8DF3
#define BattleScript_LeechSeedFree (u8*) 0x81D8E04
#define BattleScript_SpikesFree (u8*) 0x81D8E0B
extern u8 BattleScript_SideStatusWoreOffRet[];
extern u8 BattleScript_StickyHoldActivates[];
extern u8 BattleScript_CastformChange[];

#define TEXT_BUFFER_SIDE_STATUS(move, status, side) 			\
{																\
	gSideAffecting[side] &= ~status;							\
    gBattleTextBuff1[0] = 0xFD;									\
    gBattleTextBuff1[1] = 0x2;									\
    gBattleTextBuff1[2] = (move & 0xFF);						\
    gBattleTextBuff1[3] = move >> 8;							\
    gBattleTextBuff1[4] = EOS;									\
	BattleScriptPushCursor();									\
	gBattlescriptCurrInstr = BattleScript_SideStatusWoreOffRet;	\
}
