#pragma once

#define BattleScript_SelectingDisabledMove (u8*) 0x81D8C5A
#define BattleScript_SelectingTormentedMove (u8*) 0x81D8EB7
#define BattleScript_SelectingNotAllowedMoveTaunt (u8*) 0x81D8EC6
#define BattleScript_SelectingImprisionedMove (u8*) 0x81D8F9F
#define BattleScript_SelectingNotAllowedMoveChoiceItem (u8*) 0x81D963D
#define BattleScript_SelectingMoveWithNoPP (u8*) 0x81D8EA4
#define gText_WhatWillPkmnDo (u8*) 0x83FE6D5

extern u8 BattleScript_MustSelectEncoredMove[];
extern u8 BattleScript_SelectingNotAllowedMoveAssaultVest[];
extern u8 BattleScript_SelectingNotAllowedSkyBattle[];
extern u8 BattleScript_SelectingNotAllowedGravity[];
extern u8 BattleScript_SelectingNotAllowedThroatChop[];
extern u8 BattleScript_SelectingNotAllowedHealBlock[];

//Strings
extern u8 StringNull[];
extern u8 gText_Power[];
extern u8 gText_Acc[];
extern u8 gText_NoMiss[];
extern u8 gText_Physical[];
extern u8 gText_Special[];
extern u8 gText_Status[];
extern u8 gText_ResetStats[];
extern u8 gText_StatsPlus[];
extern u8 gText_CritHitsPlus[];
extern u8 gText_FollowMe[];
extern u8 gText_RecoverHP[];
extern u8 gText_HealAllyHP[];
extern u8 gText_BattleMenu[];
extern u8 gText_BattleMenu2[];
extern u8 gText_UnboundBattleMenu[];
extern u8 gText_UnboundBattleMenu2[];

#define TXT_PLUS 0x2E
#define gText_BattleSwitchWhich (u8*) 0x83FE7A0
#define gText_TypeWord (u8*) 0x83FE76A
#define gText_MoveInterfaceType (u8*) 0x83FE770
#define Pal_PPDisplay (u16*) 0x8D2FBB4
