#pragma once

#include "global.h"

bool8 __attribute__((long_call)) TestPlayerAvatarFlags(u8);
void __attribute__((long_call)) SetPlayerAvatarTransitionFlags(u16 a);
bool8 __attribute__((long_call)) PartyHasMonWithSurf(void);
bool8 __attribute__((long_call)) IsPlayerFacingSurfableFishableWater(void);