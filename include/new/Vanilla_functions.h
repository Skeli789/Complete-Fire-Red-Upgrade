#pragma once

#include "../global.h"

void* __attribute__((long_call)) memcpy_(void *dst, const void* src, u8 size);
void* __attribute__((long_call)) memset_(void *dst, u8 pattern, u8 size);

void* __attribute__((long_call)) Memcpy(void *dst, const void *src, u32 size);
void* __attribute__((long_call)) Memset(void *dst, u8 pattern, u32 size);

void* __attribute__((long_call)) Malloc(u32 size);
void* __attribute__((long_call)) Calloc(u32 size);
void __attribute__((long_call)) MallocInit(void* memStart, u32 memSize);
void __attribute__((long_call)) Free(void *pointer);

//PokeFireRed Function Names

/*Overworld Functions*/
void __attribute__((long_call)) DoScheduledBgTilemapCopiesToVram(void);
void __attribute__((long_call)) SetVBlankCallback(void (*callback));
void __attribute__((long_call)) SetHBlankCallback(void (*callback));
void __attribute__((long_call)) BuildOAMBuffer(void);
void __attribute__((long_call)) AnimateSprites(void);
void __attribute__((long_call)) ScheduleBgCopyTilemapToVram(u8 bgid);
void __attribute__((long_call)) CleanupOverworldWindowsAndTilemaps(void);
void __attribute__((long_call)) ResetBgsAndClearDma3BusyFlags(u8 bg);	// in bg.h

u32 __attribute__((long_call)) GetGameStat(u8 index);
void __attribute__((long_call)) SetGameStat(u8 index, u32 value);
void __attribute__((long_call)) IncrementGameStat(u8 gameStat);
s16 __attribute__((long_call)) Sine(s16 index, s16 amplitude);
s16 __attribute__((long_call)) Cosine(s16 index, s16 amplitude);

void __attribute__((long_call)) PlayBGM(u16 songNum);
void __attribute__((long_call)) PlaySE(u16 songNum);

u8* __attribute__((long_call)) ReturnEmptyStringIfNull(const u8* string);
void __attribute__((long_call)) AddTextPrinterParameterized4(u8 windowId, u8 fontId, u8 left, u8 top, u8 letterSpacing, u8 lineSpacing, const u8 *color, s8 speed, const u8 *str);
void __attribute__((long_call)) CpuSet(const void* src, void* dest, u32 mode);
void __attribute__((long_call)) CpuFastSet(const void* src, void* dest, u32 mode);
u16 __attribute__((long_call)) Sqrt(u32);
u32 __attribute__((long_call)) udivsi(u32, u32);
u32 __attribute__((long_call)) umodsi(u32, u32);

void __attribute__((long_call)) LoadOAM(void);
void __attribute__((long_call)) ProcessSpriteCopyRequests(void);
void __attribute__((long_call)) DeactivateAllTextPrinters(void);

void __attribute__((long_call)) EnableInterrupts(u8 disable);
void __attribute__((long_call)) DisableInterrupts(u8 disable);

//The Deal Breaker
void __attribute__((long_call)) break_func();
//u32 break_helper(u32 a);
