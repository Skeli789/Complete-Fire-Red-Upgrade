#pragma once

#include "global.h"

bool8 __attribute__((long_call)) TestPlayerAvatarFlags(u8);
void __attribute__((long_call)) SetPlayerAvatarTransitionFlags(u16 a);
bool8 __attribute__((long_call)) PartyHasMonWithSurf(void);
bool8 __attribute__((long_call)) IsPlayerFacingSurfableFishableWater(void);
void __attribute__((long_call)) PlayerGetDestCoords(s16* x, s16* y);
u8 __attribute__((long_call)) TestPlayerAvatarFlags(u8 a);
