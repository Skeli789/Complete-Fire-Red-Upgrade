#pragma once

#include "global.h"

u32 __attribute__((long_call)) GetMoney(u32* moneyPtr);
void __attribute__((long_call)) AddMoney(u32* moneySaveblock, u32 to_give);
void __attribute__((long_call)) RemoveMoney(u32* moneyPtr, u32 toSub);
bool8 __attribute__((long_call)) IsEnoughMoney(u32* moneyPtr, u32 cost);
void __attribute__((long_call)) PrintMoneyAmountInMoneyBox(u8 windowId, int amount, u8 speed);
void __attribute__((long_call)) PrintMoneyAmount(u8 windowId, u8 x, u8 y, int amount, u8 speed);

/*
void SetMoney(u32* moneyPtr, u32 newValue);
bool8 IsEnoughForCostInVar0x8005(void);
void SubtractMoneyFromVar0x8005(void);
void PrintMoneyAmountInMoneyBoxInMoneyBox(u8 windowId, int amount, u8 speed);
void PrintMoneyAmountInMoneyBoxInMoneyBoxWithBorder(u8 windowId, u16 tileStart, u8 pallete, int amount);
void ChangeAmountInMoneyBox(int amount);
void DrawMoneyBox(int amount, u8 x, u8 y);
void HideMoneyBox(void);
void AddMoneyLabelObject(u16 x, u16 y);
void RemoveMoneyLabelObject(void);
*/
