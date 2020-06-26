#ifndef GUARD_COINS_H
#define GUARD_COINS_H

#include "global.h"

/*void PrintCoinsString(u32 coinAmount);
void ShowCoinsWindow(u32 coinAmount, u8 x, u8 y);
void HideCoinsWindow(void);*/
u32 GetCoins(void);
void SetCoins(u32 coinAmount);
bool8 GiveCoins(u32 toAdd);
bool8 TakeCoins(u32 toSub);

#endif // GUARD_COINS_H
