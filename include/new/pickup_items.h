#pragma once

#include "../global.h"

/**
 * \file pickup_items.h
 * \brief Contains data relating to the Ability Pickup.
 */

#define PU_NUM_COMMON_ITEMS     18
#define PU_NUM_RARE_ITEMS       11

#define PU_COMMON_PER_ROW   9
#define PU_RARE_PER_ROW     2

extern const item_t sPickupCommonItems[PU_NUM_COMMON_ITEMS];
extern const item_t sPickupRareItems[PU_NUM_RARE_ITEMS];

extern const u32 pickup_common_item_ceilings[PU_COMMON_PER_ROW];
extern const u32 pickup_rare_item_ceilings[PU_RARE_PER_ROW];
