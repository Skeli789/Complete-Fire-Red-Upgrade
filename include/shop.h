#ifndef GUARD_SHOP_H
#define GUARD_SHOP_H

#include "global.h"
#include "task.h"

//extern EWRAM_DATA struct ItemSlot gUnknown_02039F80[3];


struct ShopData {
	/*0x0*/ void (*MainCallback)(void);
	/*0x4*/ const u16 *itemList;
	/*0x8*/ u32 itemPrice;
	/*0xC*/ u16 cursorPos;
	/*0xE*/ u16 _;
	/*0x10*/ u16 itemCount;
	/*0x12*/ u16 field12;
	/*0x14*/ u16 field14;
	/*0x16*/ bool8 martType;	// 0x1 if tm list
	//INCOMPLETE
};


void CreatePokemartMenu(const u16 *);
void CreateDecorationShop1Menu(const u16 *);
void CreateDecorationShop2Menu(const u16 *);
void __attribute__((long_call)) BuyMenuReturnToItemList(u8 taskId);
void __attribute__((long_call)) BuyMenuDisplayMessage(u8 taskId, const u8 *text, TaskFunc callback);
bool8 __attribute__((long_call)) BuyMenuBuildListMenuTemplate(void);
//void __attribute__((long_call)) Task_ReturnToItemListAfterItemPurchase(u8 taskId);
void __attribute__((long_call)) CB2_InitBuyMenu(void);
void __attribute__((long_call)) CB2_BuyMenu(void);
bool8 __attribute__((long_call)) InitShopData(void);
void __attribute__((long_call)) Task_ShopMenu(u8 taskId);
void __attribute__((long_call)) Task_ReturnToShopMenu(u8 taskId);
void __attribute__((long_call)) Task_BuyMenu(u8 taskId);
void __attribute__((long_call)) Task_SellMenu(u8 taskId);
void __attribute__((long_call)) BuildMartStrings(u16 item);
//void __attribute__((long_call)) BuyMenuPrintPriceInList(u8 windowId, int item, u8 y);
void __attribute__((long_call)) BuyMenuPrint(u8 windowId, u8 font, const u8 *text, u8 x, u8 y, u8 letterSpacing, u8 lineSpacing, u8 speed, u8 color);
void __attribute__((long_call)) RedrawListMenu(u8 taskId);

#endif // GUARD_SHOP_H
