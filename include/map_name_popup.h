#pragma once

#include "global.h"

void __attribute__((long_call)) ShowMapNamePopup(bool32 palIntoFadedBuffer);
void __attribute__((long_call)) DismissMapNamePopup(void);
bool32 __attribute__((long_call)) IsMapNamePopupTaskActive(void);
