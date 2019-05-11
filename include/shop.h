#ifndef GUARD_SHOP_H
#define GUARD_SHOP_H

#include "global.h"

extern EWRAM_DATA struct ItemSlot gUnknown_02039F80[3];

void CreatePokemartMenu(const u16 *);
void CreateDecorationShop1Menu(const u16 *);
void CreateDecorationShop2Menu(const u16 *);
void __attribute__((long_call)) BuyMenuReturnToItemList(u8 taskId);
void __attribute__((long_call)) BuyMenuDisplayMessage(u8 taskId, const u8 *text, TaskFunc callback);

#endif // GUARD_SHOP_H
