#ifndef GUARD_START_MENU_H
#define GUARD_START_MENU_H

#include "global.h"

typedef void (*MainCallback)(void);

bool8 __attribute__((long_call)) IsUpdateLinkStateCBActive(void);
void __attribute__((long_call)) ShowStartMenu(void);

#endif // GUARD_START_MENU_H
