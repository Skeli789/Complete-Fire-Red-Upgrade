#pragma once

#define BattleScript_LinkBattleWonOrLost (const u8*) 0x81D88CC
#define BattleScript_PayDayMoneyAndPickUpItems (const u8*) 0x81D8803
#define BattleScript_LocalBattleLost (const u8*) 0x81D8806
#define BattleScript_GotAwaySafely (const u8*) 0x81D8916
#define BattleScript_SmokeBallEscape (const u8*) 0x81D8901

extern const u8 BattleScript_Victory[];
extern const u8 BattleScript_PrintPlayerForfeited[];
extern const u8 BattleScript_PrintPlayerForfeitedLinkBattle[];
extern const u8 BattleScript_LostMultiBattleTower[];
extern const u8 BattleScript_LostBattleTower[];
extern const u8 BattleScript_AskIfWantsToForfeitMatch[];
extern const u8 BattleScript_RanAwayUsingMonAbility[];
