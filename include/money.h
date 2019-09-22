#pragma once

#include "global.h"

void __attribute__((long_call)) AddMoney(u32* moneySaveblock, u32 to_give);
bool8 __attribute__((long_call)) IsEnoughMoney(u32* moneyPtr, u32 cost);

/*
u32 GetMoney(u32* moneyPtr);
void SetMoney(u32* moneyPtr, u32 newValue);
void RemoveMoney(u32* moneyPtr, u32 toSub);
bool8 IsEnoughForCostInVar0x8005(void);
void SubtractMoneyFromVar0x8005(void);
void PrintMoneyAmountInMoneyBoxInMoneyBox(u8 windowId, int amount, u8 speed);
void PrintMoneyAmountInMoneyBox(u8 windowId, u8 x, u8 y, int amount, u8 speed);
void PrintMoneyAmountInMoneyBoxInMoneyBoxWithBorder(u8 windowId, u16 tileStart, u8 pallete, int amount);
void ChangeAmountInMoneyBox(int amount);
void DrawMoneyBox(int amount, u8 x, u8 y);
void HideMoneyBox(void);
void AddMoneyLabelObject(u16 x, u16 y);
void RemoveMoneyLabelObject(void);
*/
