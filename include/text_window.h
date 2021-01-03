#pragma once

#include "global.h"

#define WINDOW_FRAMES_COUNT 20

struct TilesPal
{
    const u8* tiles;
    const u16* pal;
};

extern const u8 gTextWindowFrame1_Gfx[];
extern const u16 gTextWindowFrame1_Pal[];

void __attribute__((long_call)) LoadWindowGfx(u8 windowId, u8 frameId, u16 destOffset, u8 palOffset);
void __attribute__((long_call)) TextWindow_SetUserSelectedFrame(u8 windowId, u16 destOffset, u8 palOffset);
void __attribute__((long_call)) DrawTextBorderOuter(u8 windowId, u16 tileNum, u8 palNum);
void __attribute__((long_call)) DrawTextBorderInner(u8 windowId, u16 tileNum, u8 palNum);
const struct TilesPal* __attribute__((long_call)) GetWindowFrameTilesPal(u8 id);
const u16*  __attribute__((long_call)) stdpal_get(u8 id);
