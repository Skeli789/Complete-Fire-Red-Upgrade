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

#define PARTY_LAYOUT_SINGLE          0
#define PARTY_LAYOUT_DOUBLE          1
#define PARTY_LAYOUT_MULTI           2
#define PARTY_LAYOUT_MULTI_SHOWCASE  3  // The layout during the screen that appears just before a multi battle
#define PARTY_LAYOUT_COUNT           4
#define KEEP_PARTY_LAYOUT            0xFF

#define PARTY_MENU_TYPE_FIELD                     0 
#define PARTY_MENU_TYPE_IN_BATTLE                 1 
#define PARTY_MENU_TYPE_CONTEST                   2 
#define PARTY_MENU_TYPE_CHOOSE_MON                3 
#define PARTY_MENU_TYPE_CHOOSE_HALF               4  // multi battles, eReader battles, and some battle facilities
#define PARTY_MENU_TYPE_MULTI_SHOWCASE            5
#define PARTY_MENU_TYPE_DAYCARE                   6 
#define PARTY_MENU_TYPE_MOVE_RELEARNER            7 
#define PARTY_MENU_TYPE_UNION_ROOM_REGISTER       8  // trading board
#define PARTY_MENU_TYPE_UNION_ROOM_TRADE          9  // trading board
#define PARTY_MENU_TYPE_SPIN_TRADE                10 // Unused beta for Gen IV's Spin Trade
#define PARTY_MENU_TYPE_MINIGAME                  11

#define PARTY_ACTION_CHOOSE_MON         0
#define PARTY_ACTION_SEND_OUT           1
#define PARTY_ACTION_CANT_SWITCH        2
#define PARTY_ACTION_USE_ITEM           3
#define PARTY_ACTION_ABILITY_PREVENTS   4
#define PARTY_ACTION_GIVE_ITEM          5  
#define PARTY_ACTION_GIVE_PC_ITEM       6
#define PARTY_ACTION_GIVE_MAILBOX_MAIL  7
#define PARTY_ACTION_SWITCH             8
#define PARTY_ACTION_SWITCHING          9
#define PARTY_ACTION_SOFTBOILED         10
#define PARTY_ACTION_CHOOSE_AND_CLOSE   11
#define PARTY_ACTION_MOVE_TUTOR         12
#define PARTY_ACTION_MINIGAME           13
#define PARTY_ACTION_REUSABLE_ITEM      14

// IDs for DisplayPartyMenuStdMessage, to display the message at the bottom of the party menu
#define PARTY_MSG_CHOOSE_MON                0
#define PARTY_MSG_CHOOSE_MON_OR_CANCEL      1
#define PARTY_MSG_CHOOSE_MON_AND_CONFIRM    2
#define PARTY_MSG_MOVE_TO_WHERE             3 
#define PARTY_MSG_TEACH_WHICH_MON           4
#define PARTY_MSG_USE_ON_WHICH_MON          5
#define PARTY_MSG_GIVE_TO_WHICH_MON         6
#define PARTY_MSG_NOTHING_TO_CUT            7
#define PARTY_MSG_CANT_SURF_HERE            8
#define PARTY_MSG_ALREADY_SURFING           9
#define PARTY_MSG_CURRENT_TOO_FAST          10
#define PARTY_MSG_ENJOY_CYCLING             11
#define PARTY_MSG_ALREADY_IN_USE            12
#define PARTY_MSG_CANT_USE_HERE             13
#define PARTY_MSG_NO_MON_FOR_BATTLE         14
#define PARTY_MSG_CHOOSE_MON_2              15
#define PARTY_MSG_NOT_ENOUGH_HP             16
#define PARTY_MSG_THREE_MONS_ARE_NEEDED     17
#define PARTY_MSG_TWO_MONS_ARE_NEEDED       18
#define PARTY_MSG_MONS_CANT_BE_SAME         19
#define PARTY_MSG_NO_SAME_HOLD_ITEMS        20
#define PARTY_MSG_UNUSED                    21
#define PARTY_MSG_DO_WHAT_WITH_MON          22
#define PARTY_MSG_RESTORE_WHICH_MOVE        23
#define PARTY_MSG_BOOST_PP_WHICH_MOVE       24
#define PARTY_MSG_DO_WHAT_WITH_ITEM         25
#define PARTY_MSG_DO_WHAT_WITH_MAIL         26
#define PARTY_MSG_NONE                      127

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

struct PartyMenu
{
	MainCallback exitCallback;
	TaskFunc task;
	u8 menuType:4;
	u8 layout:2;
	u8 unk_8_6:2;
	s8 slotId;
	s8 slotId2;
	u8 action;
	u16 bagItem;
	s16 data1;           // used variously as a moveId, counter, moveSlotId, or cursorPos
	s16 learnMoveState;  // data2, used only as a learn move state
};

struct PartyMenuInternal
{
	TaskFunc task;
	MainCallback exitCallback;
	u32 chooseHalf:1;
	u32 lastSelectedSlot:3;  //Used to return to same slot when going left/right bewtween columns
	u32 spriteIdConfirmPokeball:7;
	u32 spriteIdCancelPokeball:7;
	u32 messageId:14;
	u8 windowId[3]; //windowId[1] is highlighted mon
	u8 actions[8];
	u8 numActions;
	u16 palBuffer[BG_PLTT_SIZE / sizeof(u16)];
	s16 data[16];
};

extern struct PartyMenuInternal* sPartyMenuInternal;

extern struct PartyMenu gPartyMenu;
extern bool8 gPartyMenuUseExitCallback;
extern u8 gSelectedMonPartyId;
extern MainCallback gPostMenuFieldCallback;
extern void (*gItemUseCB)(u8, TaskFunc);

void __attribute__((long_call)) InitPartyMenu(u8 menuType, u8 layout, u8 partyAction, bool8 keepCursorPos, u8 messageId, TaskFunc task, MainCallback callback);
void __attribute__((long_call)) DisplayPartyPokemonSelectData(u8 slot, u8 stringID);
void __attribute__((long_call)) Task_HandleChooseMonInput(u8 taskId);
bool8 __attribute__((long_call)) IsPartyMenuTextPrinterActive(void);
u8 __attribute__((long_call)) GetMonPrimaryAilments(u32 status);
void __attribute__((long_call)) DisplayPartyMenuStdMessage(u32 stringID);
u8* __attribute__((long_call)) GetMonNickname(struct Pokemon *mon, u8 *dest);
u8 __attribute__((long_call)) GetCursorSelectionMonId(void);
bool8 __attribute__((long_call)) FieldCallback_PrepareFadeInFromMenu(void);

void __attribute__((long_call)) AnimatePartySlot(u8 slot, u8 b);
u16 __attribute__((long_call)) PartyMenuButtonHandler(s8 *ptr);
bool8 __attribute__((long_call)) some_other_kind_of_link_test(void);
void __attribute__((long_call)) InitChooseHalfPartyForBattle(u8 a1);
u8 __attribute__((long_call)) GetItemEffectType(u16 item);
void __attribute__((long_call)) PartyMenuModifyHP(u8 taskId, u8 slot, s8 hpIncrement, s16 hpDifference, TaskFunc task);

void __attribute__((long_call)) Task_ClosePartyMenu(u8 taskId);
