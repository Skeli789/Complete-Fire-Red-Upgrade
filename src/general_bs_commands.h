#pragma once

#include "defines.h"

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
Faint_PrimalWeather
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

#define BattleScript_SturdyPreventsOHKO (u8*) 0x0
#define BattleScript_SubstituteFade (u8*)  0x81D9526
#define BattleScript_HangedOnMsg (u8*) 0x0
#define BattleScript_OneHitKOMsg (u8*) 0x0
#define BattleScript_EnduredMsg (u8*) 0x0
#define BattleScript_PrintPayDayMoneyString (u8*) 0x0
#define BattleScript_Failed (u8*) 0x0
#define BattleScript_WrapFree (u8*) 0x0
#define BattleScript_LeechSeedFree (u8*) 0x0
#define BattleScript_SpikesFree (u8*) 0x0
#define BattleScript_SideStatusWoreOffRet (u8*) 0x0
#define BattleScript_StickyHoldActivates (u8*) 0x0
#define BattleScript_CastformChange (u8*) 0x0
