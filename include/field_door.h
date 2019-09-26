#pragma once

#include "global.h"

struct DoorGraphics
{
    u16 metatileNum;
    u8 sound;
    u8 size;
    const void *tiles;
    const void *palette;
};

struct DoorAnimFrame
{
    u8 time;
    u16 offset;
};

void __attribute__((long_call)) FieldSetDoorOpened(u32, u32);
void __attribute__((long_call)) FieldSetDoorClosed(u32, u32);
s8 __attribute__((long_call)) FieldAnimateDoorClose(u32, u32);
s8 __attribute__((long_call)) FieldAnimateDoorOpen(u32, u32);
bool8 __attribute__((long_call)) FieldIsDoorAnimationRunning(void);
u32 __attribute__((long_call)) GetDoorSoundEffect(u32 x, u32 y);

