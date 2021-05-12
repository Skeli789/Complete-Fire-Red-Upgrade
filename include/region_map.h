#pragma once

#include "global.h"

#include "bg.h"

enum
{
    REGIONMAP_TYPE_NORMAL,
    REGIONMAP_TYPE_WALL,
    REGIONMAP_TYPE_FLY,
    REGIONMAP_TYPE_COUNT
};

u8* __attribute__((long_call)) GetMapName(u8* dest, u16 regionMapId, u16 padLength);
u8 __attribute__((long_call)) GetSelectedRegionMap(void);
void __attribute__((long_call)) InitRegionMapWithExitCB(u8 a0, void (*a1)(void));
