#pragma once

#include "global.h"

#include "sprite.h"

int __attribute__((long_call)) CountTrailingZeroBits(u32 value);

/*
extern const u8 gMiscBlank_Gfx[]; // unused in Emerald
extern const u32 gBitTable[];

u8 __attribute__((long_call)) CreateInvisibleSpriteWithCallback(void (*)(struct Sprite *));
void __attribute__((long_call)) StoreWordInTwoHalfwords(u16 *, u32);
void __attribute__((long_call)) LoadWordFromTwoHalfwords(u16 *, u32 *);

u16 __attribute__((long_call)) CalcCRC16(u8 *data, s32 length);
u16 __attribute__((long_call)) CalcCRC16WithTable(u8 *data, u32 length);
u32 __attribute__((long_call)) CalcByteArraySum(const u8* data, u32 length);
*/
