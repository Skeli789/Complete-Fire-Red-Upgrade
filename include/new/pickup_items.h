#pragma once

#include "../global.h"

#define PU_NUM_COMMON_ITEMS     18
#define PU_NUM_RARE_ITEMS       11

#define PU_COMMON_PER_ROW   9
#define PU_RARE_PER_ROW     2

extern item_t sPickupCommonItems[PU_NUM_COMMON_ITEMS];
extern item_t sPickupRareItems[PU_NUM_RARE_ITEMS];

extern u32 pickup_common_item_ceilings[PU_COMMON_PER_ROW];
extern u32 pickup_rare_item_ceilings[PU_RARE_PER_ROW];