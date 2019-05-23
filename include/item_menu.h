#pragma once

#include "global.h"

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

void __attribute__((long_call)) GoToBagMenu(u8 bagMenuType, u8 pocketId, void ( *postExitMenuMainCallback2)());
void __attribute__((long_call)) GoToBerryMenu(u8 bagMenuType, void ( *postExitMenuMainCallback2)(), u8);
void __attribute__((long_call)) GoToTMMenu(u8 bagMenuType, void ( *postExitMenuMainCallback2)(), u8);