#pragma once

#include "global.h"

extern u8 gNumSafariBalls;

bool8 __attribute__((long_call)) GetSafariZoneFlag(void);
void __attribute__((long_call)) SetSafariZoneFlag(void);
void __attribute__((long_call)) ResetSafariZoneFlag(void);

/*
void EnterSafariMode(void);
void ExitSafariMode(void);

bool8 SafariZoneTakeStep(void);
void SafariZoneRetirePrompt(void);

struct Pokeblock *SafariZoneGetActivePokeblock(void);
void SafariZoneActivatePokeblockFeeder(u8 pokeblock_index);
*/
