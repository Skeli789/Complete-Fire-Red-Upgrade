#pragma once

#include "global.h"
#include "pokemon.h"

enum
{
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

#define MENU_SUMMARY 0
#define MENU_SWITCH 1
#define MENU_CANCEL1 2
#define MENU_ITEM 3
#define MENU_GIVE 4
#define MENU_TAKE_ITEM 5
#define MENU_MAIL 6
#define MENU_TAKE_MAIL 7
#define MENU_READ 8
#define MENU_CANCEL2 9
#define MENU_SHIFT 10
#define MENU_SEND_OUT 11
#define MENU_ENTER 12
#define MENU_NO_ENTRY 13
#define MENU_STORE 14
#define MENU_REGISTER 15
#define MENU_TRADE1 16
#define MENU_TRADE2 17
#define MENU_MOVE_ITEM 18
#define MENU_FIELD_MOVES 19

#define MENU_FIELD_MOVES_MINUS_1 18

struct MonSummaryData {
	u8 field0[0x3210];
	u8 maxPartyIndex;
	u8 field3211[3];
	u8 currentPage;
	u8 field3215[0xDF];
	struct Pokemon* partyData;
	u8 field32f8[0x10];
};

struct Unk_Rodata1
{
	void (*unk0)(u8, u8, u8, u8, u8, u8);
	u8 unk4[24];
	u8 unk1C;
	u8 unk1D;
	u8 unk1E;
	u8 unk1F;
};

struct Struct203B0B4
{
	const struct Unk_Rodata1 *unk0;
	const u8 *unk4;
	u8 windowId;
	u8 monSpriteId;
	u8 itemSpriteId;
	u8 pokeballSpriteId;
	u8 statusSpriteId;
	u8 fillerD[3];
};

struct PartyMenuViewing
{
	u8* unk0;
	MainCallback exitCallback;
	u32 unk8_0:1;
	u32 lastViewed:3; //The last mon you selected before going back to the left
	u32 unk8_2:7;
	u32 unk9_0:7;
	u32 unkA_0:14;
	u8 windowId[3]; //windowId[1] is highlighted mon
	u8 actions[8];
	u8 listSize;
    u16 palBuffer[0x100]; //Maybe 0x150?
    s16 data[16];
};

#define gPartyMenuView ((struct PartyMenuViewing*) *((u32*) 0x203B09C)) //extern struct PartyMenuViewing gPartyMenuView;
extern MainCallback gPostMenuFieldCallback;

void __attribute__((long_call)) DisplayPartyPokemonSelectData(u8 slot, u8 stringID);
void __attribute__((long_call)) sub_811FA78(u8 taskId);
void __attribute__((long_call)) sub_811FB28(u8 taskId);
s8 __attribute__((long_call)) sub_812029C(s8 a, s8 b);
u8 __attribute__((long_call)) DisplayPartyMenuMsgBox(const u8* str, u8 b);
void __attribute__((long_call)) sub_81203B8(u8 taskId);
u8 __attribute__((long_call)) GetMonPrimaryAilments(u32 status);
void __attribute__((long_call)) DisplayPartyPokemonNickname(struct Pokemon* mon, struct Struct203B0B4* ptr, u8 c);
void __attribute__((long_call)) DisplayPartyPokemonLevelCheck(struct Pokemon*, struct Struct203B0B4*, u8);
void __attribute__((long_call)) DisplayPartyPokemonGenderNidoranCheck(struct Pokemon*, struct Struct203B0B4*, u8);
void __attribute__((long_call)) DisplayPartyPokemonOtherText(u8, struct Struct203B0B4*, u8);
void __attribute__((long_call)) sub_8121CE4(u8* ptr);
void __attribute__((long_call)) PrintPartyMenuPromptText(u32 stringID);
void __attribute__((long_call)) sub_8122D78(void);
void __attribute__((long_call)) sub_8124258(void);
u8* __attribute__((long_call)) GetMonNickname(struct Pokemon *mon, u8 *dest);
u8 __attribute__((long_call)) GetCursorSelectionMonId(void);
bool8 __attribute__((long_call)) FieldCallback_PrepareFadeInFromMenu(void);

void __attribute__((long_call)) sub_811F818(u8 slot, u8 b);
void __attribute__((long_call)) sub_811FD88(u8 taskId, s8 *ptr);
u16 __attribute__((long_call)) PartyMenuButtonHandler(s8 *ptr);
u8 __attribute__((long_call)) sub_81202F8(const u8* str, u8 b);
void __attribute__((long_call)) sub_81224B4(struct Pokemon *mon, struct Struct203B0B4* ptr);
void __attribute__((long_call)) sub_8123BF0(u8 taskId);
bool8 __attribute__((long_call)) some_other_kind_of_link_test(void);
