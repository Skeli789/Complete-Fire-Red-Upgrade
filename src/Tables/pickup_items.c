#include "../defines.h"
#include "../../include/new/pickup_items.h"
#include "../../include/constants/items.h"

item_t sPickupCommonItems[PU_NUM_COMMON_ITEMS] = {
    ITEM_POTION,
    ITEM_ANTIDOTE,
    ITEM_SUPER_POTION,
    ITEM_GREAT_BALL,
	ITEM_REPEL,
    ITEM_ESCAPE_ROPE,
    ITEM_FULL_HEAL,
	ITEM_HYPER_POTION,
    ITEM_ULTRA_BALL,
	ITEM_REVIVE,
    ITEM_RARE_CANDY,
    ITEM_SUN_STONE,
    ITEM_MOON_STONE,
    ITEM_HEART_SCALE,
    ITEM_FULL_RESTORE,
    ITEM_MAX_REVIVE,
	ITEM_PP_UP,
    ITEM_MAX_ELIXIR,
};

item_t sPickupRareItems[PU_NUM_RARE_ITEMS] = {
    ITEM_HYPER_POTION,
    ITEM_NUGGET,
    ITEM_KINGS_ROCK,
    ITEM_FULL_RESTORE,
    ITEM_ETHER,
    ITEM_IRON_BALL,
    ITEM_DESTINY_KNOT,
    ITEM_ELIXIR,
    ITEM_DESTINY_KNOT,
    ITEM_LEFTOVERS,
    ITEM_DESTINY_KNOT,
};

u32 pickup_common_item_ceilings[PU_COMMON_PER_ROW] = {
    19661, 26214, 32768, 39322, 45875, 52429, 58982, 61604, 64225
};

u32 pickup_rare_item_ceilings[PU_RARE_PER_ROW] = { 64881, 65536 };
