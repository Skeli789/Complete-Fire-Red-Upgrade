#pragma once

#include "global.h"

void __attribute__((long_call)) WarpFadeScreen(void);
void __attribute__((long_call)) DoWarp(void);
bool8 __attribute__((long_call)) walkrun_is_standing_still(void);
void __attribute__((long_call)) sub_807DCB0(u8 arg);
void __attribute__((long_call)) FadeScreen(u8, u8);
void __attribute__((long_call)) DoDiveWarp(void);