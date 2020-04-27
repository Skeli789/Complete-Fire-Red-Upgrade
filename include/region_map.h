#pragma once

#include "global.h"

#include "bg.h"

// Exported RAM declarations

// Exported ROM declarations
u8* __attribute__((long_call)) GetMapName(u8* dest, u16 regionMapId, u16 padLength);
u8 __attribute__((long_call)) GetSelectedRegionMap(void);
