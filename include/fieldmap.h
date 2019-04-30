#pragma once

#include "global.h"

extern struct MapCoords {
    int width;
    int height;
} gUnknown_03005DC0;

u32 __attribute__((long_call)) MetatileBehavior_GetLowerBytes(u32 a, u8 b);
u32 __attribute__((long_call)) MapGridGetMetatileIdAt(int x, int y);
u16 __attribute__((long_call)) MapGridGetMetatileField(u16 x, u16 y, u8 a);
u32 __attribute__((long_call)) MapGridGetMetatileBehaviorAt(int, int);
u8 __attribute__((long_call)) MapGridGetZCoordAt(int x, int y);

/*
u32 MapGridGetMetatileIdAt(int, int);
u32 MapGridGetMetatileBehaviorAt(int, int);
void MapGridSetMetatileIdAt(int, int, u16);
void MapGridSetMetatileEntryAt(int, int, u16);
void GetCameraCoords(u16*, u16*);
bool8 MapGridIsImpassableAt(s16, s16);
s32 GetMapBorderIdAt(s16, s16);
bool32 CanCameraMoveInDirection(u8);
u16 GetBehaviorByMetatileId(u16 metatileId);
*/