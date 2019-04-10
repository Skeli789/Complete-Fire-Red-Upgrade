#pragma once

#include "global.h"

enum {
    AILMENT_NONE,
    AILMENT_PSN,
    AILMENT_PRZ,
    AILMENT_SLP,
    AILMENT_FRZ,
    AILMENT_BRN
};

enum
{
    PARTY_CHOOSE_MON,
    PARTY_MUST_CHOOSE_MON,
    PARTY_CANT_SWITCH,
    PARTY_USE_ITEM_ON,
    PARTY_ABILITY_PREVENTS,
    PARTY_GIVE_ITEM,
};

void __attribute__((long_call)) sub_811FA78(u8 taskId);
s8 __attribute__((long_call)) sub_812029C(s8 a, s8 b);
void __attribute__((long_call)) sub_8122D78(void);

/*
bool8 pokemon_has_move(struct Pokemon *, u16);
void sub_81B58A8(void);
void DoWallyTutorialBagMenu(void);
u8 pokemon_ailments_get_primary(u32 status);
u16 ItemIdToBattleMoveId(u16 itemId);
*/
