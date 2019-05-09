#pragma once

#include "global.h"
#include "pokemon.h"

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

struct MonSummaryData {
	u8 field0[0x3210];
	u8 maxPartyIndex;
	u8 field3211[3];
	u8 currentPage;
	u8 field3215[0xDF];
	struct Pokemon* partyData;
	u8 field32f8[0x10];
};

void __attribute__((long_call)) sub_811FA78(u8 taskId);
s8 __attribute__((long_call)) sub_812029C(s8 a, s8 b);
u8 __attribute__((long_call)) pokemon_ailments_get_primary(u32 status);
void __attribute__((long_call)) sub_8122D78(void);

/*
bool8 pokemon_has_move(struct Pokemon *, u16);
void sub_81B58A8(void);
void DoWallyTutorialBagMenu(void);
u16 ItemIdToBattleMoveId(u16 itemId);
*/
