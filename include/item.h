#ifndef ITEM_H
#define ITEM_H

#include "global.h"


typedef void (*ItemUseFunc)(u8);

struct Item
{
    u8 name[14];
    u16 itemId;	//0xE
    u16 price;	//0x10
    u8 holdEffect;	//0x12
    u8 holdEffectParam;	//0x13
    const u8 *description;	//0x14
    u8 importance;	//0x18
    u8 unk19;		//0x19
    u8 pocket;		//0x1a
    u8 type;		//0x1b
    ItemUseFunc fieldUseFunc;	//0x1c
    u8 battleUsage;	//0x20
    ItemUseFunc battleUseFunc;	//0x24
    u8 secondaryId;	//0x28
};

struct BagPocket
{
    struct ItemSlot *itemSlots;
    u16 capacity;
};

bool8 __attribute__((long_call)) IsMail(u16 item_id);
void __attribute__((long_call)) CopyItemName(u16 itemId, u8* dst);
void __attribute__((long_call)) CopyItemNameHandlePlural(u16 itemId, u8 *string, u32 quantity);
bool8 __attribute__((long_call)) CheckBagHasItem(u16 itemId, u16 count);
bool8 __attribute__((long_call)) AddBagItem(u16 itemId, u16 count);
bool8 __attribute__((long_call)) RemoveBagItem(u16 itemId, u16 count);
u16 __attribute__((long_call)) CountTotalItemQuantityInBag(u16 itemId);
u16 __attribute__((long_call)) SanitizeItemId(u16 item_id);
u8 /*__attribute__((long_call))*/ ItemId_GetHoldEffect(u16 item_id);
u8 /*__attribute__((long_call))*/ ItemId_GetHoldEffectParam(u16 item_id);
u8 __attribute__((long_call)) ItemId_GetMystery2(u16 item_id);
u8 __attribute__((long_call)) GetPocketByItemId(u16 item_id);
u8 __attribute__((long_call)) ItemId_GetType(u16 item_id);
void __attribute__((long_call)) RemoveUsedItem(void);
u16 __attribute__((long_call)) GetBagItemQuantity(u16 *quantity);
u8 __attribute__((long_call)) ItemIsUnique(u16 item);
void __attribute__((long_call)) Task_BuyHowManyDialogueInit(u8 taskId);
u16 __attribute__((long_call)) ItemId_GetPrice(u16 itemId);
void __attribute__((long_call)) BuyMenuConfirmPurchase(u8 taskId);
u8 __attribute__((long_call)) *ItemId_GetDescription(u16 itemId);
void __attribute__((long_call)) DisplayItemMessageInBag(u8 taskId, u8 a, const u8* str, void(*callback)(u8 taskId));
void __attribute__((long_call)) Task_ReturnToBagFromContextMenu(u8 taskId);
bool8 __attribute__((long_call)) CheckBagHasSpace(u16 itemId, u16 count);
u16 __attribute__((long_call)) BagGetItemIdByPocketPosition(u8 pocketId, u16 itemId);
ItemUseFunc __attribute__((long_call)) ItemId_GetFieldFunc(u16 itemId);
void __attribute__((long_call)) ClearItemSlots(struct ItemSlot *itemSlots, u8 b);
bool8 __attribute__((long_call)) AddPCItem(u16 itemId, u16 count);
void __attribute__((long_call)) RemovePCItem(u8 index, u16 count);

/*
void GetBerryCountString(u8* dst, const u8* berryName, u32 quantity);

bool8 IsBagPocketNonEmpty(u8 pocket);
bool8 CheckBagHasItem(u16 itemId, u16 count);
bool8 AddBagItem(u16 itemId, u16 count);
u8 GetPocketByItemId(u16 itemId);
u8 CountUsedPCItemSlots(void);
bool8 CheckPCHasItem(u16 itemId, u16 count);
void SwapRegisteredBike(void);
const struct Item *ItemId_GetItem(u16 itemId);
u16 ItemId_GetId(u16 itemId);
u8 ItemId_GetHoldEffect(u16 itemId);
u8 ItemId_GetHoldEffectParam(u16 itemId);
bool32 ItemId_CopyDescription(u8 *a, u32 itemId, u32 c);
u8 ItemId_GetImportance(u16 itemId);
u8 ItemId_GetUnknownValue(u16 itemId);
u8 ItemId_GetPocket(u16 itemId);
u8 ItemId_GetType(u16 itemId);
u8 ItemId_GetBattleUsage(u16 itemId);
ItemUseFunc ItemId_GetBattleFunc(u16 itemId);
u8 ItemId_GetSecondaryId(u16 itemId);
u16 itemid_get_market_price(u16 itemId);
void sub_809A2DC(void);
void sub_809A2A4(void);
void sub_8099E90(u16, u8 *);
*/
#endif // ITEM_H
