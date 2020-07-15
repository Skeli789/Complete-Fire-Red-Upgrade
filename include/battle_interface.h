#pragma once

#include "global.h"
#include "battle_controllers.h"

enum
{
    HP_CURRENT,
    HP_MAX
};

enum
{
    HEALTH_BAR,
    EXP_BAR
};

enum
{
    HP_BAR_EMPTY,
    HP_BAR_RED,
    HP_BAR_YELLOW,
    HP_BAR_GREEN,
    HP_BAR_FULL,
};

#define TAG_HEALTHBOX_PLAYER1_TILE      0xD6FF
#define TAG_HEALTHBOX_PLAYER2_TILE      0xD700
#define TAG_HEALTHBOX_OPPONENT1_TILE    0xD701
#define TAG_HEALTHBOX_OPPONENT2_TILE    0xD702

#define TAG_HEALTHBAR_PLAYER1_TILE      0xD704
#define TAG_HEALTHBAR_OPPONENT1_TILE    0xD705
#define TAG_HEALTHBAR_PLAYER2_TILE      0xD706
#define TAG_HEALTHBAR_OPPONENT2_TILE    0xD707

#define TAG_HEALTHBOX_SAFARI_TILE       0xD70B

#define TAG_STATUS_SUMMARY_BAR_TILE     0xD70C
#define TAG_STATUS_SUMMARY_BALLS_TILE   0xD714

#define TAG_HEALTHBOX_PAL               0xD6FF
#define TAG_STATUS_SUMMARY_BAR_PAL      0xD710
#define TAG_STATUS_SUMMARY_BALLS_PAL    0xD712

enum
{
    HEALTHBOX_ALL,
    HEALTHBOX_CURRENT_HP,
    HEALTHBOX_MAX_HP,
    HEALTHBOX_LEVEL,
    HEALTHBOX_NICK,
    HEALTHBOX_HEALTH_BAR,
    HEALTHBOX_EXP_BAR,
    HEALTHBOX_UNUSED_7,
    HEALTHBOX_UNUSED_8,
    HEALTHBOX_STATUS_ICON,
    HEALTHBOX_SAFARI_ALL_TEXT,
    HEALTHBOX_SAFARI_BALLS_TEXT
};


u8 __attribute__((long_call)) CreateBankHealthboxSprites(u8 bank);
u8 __attribute__((long_call)) CreateSafariPlayerHealthboxSprites(void);
void __attribute__((long_call)) SetBattleBarStruct(u8 bank, u8 healthboxSpriteId, s32 maxVal, s32 currVal, s32 receivedValue);
void __attribute__((long_call)) SetHealthboxSpriteInvisible(u8 healthboxSpriteId);
void __attribute__((long_call)) SetHealthboxSpriteVisible(u8 healthboxSpriteId);
void __attribute__((long_call)) DestoryHealthboxSprite(u8 healthboxSpriteId);
void __attribute__((long_call)) DummyBattleInterfaceFunc(u8 healthboxSpriteId, bool8 isDoubleBattleBankOnly);
void __attribute__((long_call)) UpdateOamPriorityInAllHealthboxes(u8 priority);
void __attribute__((long_call)) SetBankHealthboxSpritePos(u8 bank);
void __attribute__((long_call)) UpdateHpTextInHealthbox(u8 healthboxSpriteId, s16 value, u8 maxOrCurrent);
void __attribute__((long_call)) SwapHpBarsWithHpText(void);
u8 __attribute__((long_call)) CreatePartyStatusSummarySprites(u8 bank, struct HpAndStatus *partyInfo, u8 arg2, bool8 isBattleStart);
void __attribute__((long_call)) sub_8073C30(u8 taskId);
void __attribute__((long_call)) UpdateHealthboxAttribute(u8 healthboxSpriteId, struct Pokemon *mon, u8 elementId);
s32 __attribute__((long_call)) sub_8074AA0(u8 bank, u8 healthboxSpriteId, u8 whichBar, u8 arg3);
u8 __attribute__((long_call)) GetScaledHPFraction(s16 hp, s16 maxhp, u8 scale);
u8 __attribute__((long_call)) GetHPBarLevel(s16 hp, s16 maxhp);
void __attribute__((long_call)) UpdateNickInHealthbox(u8 spriteId, struct Pokemon *mon);
void __attribute__((long_call)) TryAddPokeballIconToHealthbox(u8 spriteId, u8);
