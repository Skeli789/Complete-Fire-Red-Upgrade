#pragma once

#include "global.h"
#include "menu_helpers.h"

#define OPEN_BAG_ITEMS     0
#define OPEN_BAG_KEYITEMS  1
#define OPEN_BAG_POKEBALLS 2
#define OPEN_BAG_LAST      3
#define MAIN_POCKETS_COUNT 3

enum
{
	BAG_OPEN_REGULAR,
	BAG_OPEN_GIVE_ITEM,
	BAG_OPEN_SELL_ITEM,
	BAG_OPEN_DEPOSIT_ITEM,
	BAG_OPEN_GIVE_ITEM_2,
	BAG_OPEN_USE_ITEM_BATTLE,
	BAG_OPEN_OLD_MAN,
	BAG_OPEN_POKE_DUDE_USE_ANTIDOTE,
	BAG_OPEN_POKE_DUDE_USE_BALL,
	BAG_OPEN_POKE_DUDE_REGISTER_ITEM,
	BAG_OPEN_POKE_DUDE_TM_CASE,
	BAG_OPEN_OLD_COUNT,
	BAG_OPEN_SELECT_ITEM, //NEW!
};

enum
{
	BERRY_POUCH_REGULAR,
	BERRY_POUCH_GIVE_ITEM,
	BERRY_POUCH_SELL_ITEM,
	BERRY_POUCH_POKEMON_STORAGE,
	BERRY_POUCH_USE_TOSS,
	BERRY_POUCH_CHOOSE_ITEM,
};

enum
{
	TM_CASE_REGULAR,
	TM_CASE_GIVE_ITEM,
	TM_CASE_SELL_ITEM,
	TM_CASE_POKEMON_STORAGE,
	TM_CASE_POKE_DUDE,
	TM_CASE_OLD_COUNT,
	TM_CASE_SELECT_ITEM, //NEW!
};

struct BagStruct
{
    MainCallback bagCallback;
    u8 location;
    bool8 bagOpen;
    u16 pocket;
    u16 itemsAbove[MAIN_POCKETS_COUNT];
    u16 cursorPos[MAIN_POCKETS_COUNT];
};

extern struct BagStruct gBagMenuState; //0x203ACFC

void __attribute__((long_call)) GoToBagMenu(u8 bagMenuType, u8 pocketId, void ( *postExitMenuMainCallback2)());
void __attribute__((long_call)) GoToBerryMenu(u8 bagMenuType, void ( *postExitMenuMainCallback2)(), u8);
void __attribute__((long_call)) GoToTMMenu(u8 bagMenuType, void ( *postExitMenuMainCallback2)(), u8);
void __attribute__((long_call)) Task_ItemContextMenuByLocation(u8 taskId);
void __attribute__((long_call)) BagDestroyPocketScrollArrowPair(void);
u8 __attribute__((long_call)) ShowBagWindow(u8, u8);
void __attribute__((long_call)) HideBagWindow(u8);
u8 __attribute__((long_call)) GetBagWindow(u8);
void __attribute__((long_call)) BagMenu_PrintCursor_(u8 a, u8 b);
void __attribute__((long_call)) Task_RedrawArrowsAndReturnToBagMenuSelect(u8 taskId);
void __attribute__((long_call)) BagPrintTextOnWindow(u8 windowId, u8 fontId, const u8 *str, u8 left, u8 top, u8 letterSpacing, u8 lineSpacing, u8 speed, u8 h);
//void __attribute__((long_call)) Bag_BuildListMenuTemplate(u8 pocketId);
void __attribute__((long_call)) BagCreateYesNoMenuBottomRight(u8 a, const struct YesNoFuncTable *funcTable);
//void __attribute__((long_call)) PocketCalculateInitialCursorPosAndItemsAbove(u8 pocketId);
void __attribute__((long_call)) CB2_BagMenuFromStartMenu(void);
void __attribute__((long_call)) ItemMenu_StartFadeToExitCallback(u8 taskId);
void __attribute__((long_call)) ItemMenu_SetExitCallback(void* callbackFunc);
void __attribute__((long_call)) PrepareOverworldReturn(void);
void __attribute__((long_call)) ResetBagCursorPositions(void);
void __attribute__((long_call)) Task_BagMenu_HandleInput(u8 taskId);
