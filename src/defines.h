#pragma once

#include "config.h"

#include "../include/global.h"
#include "../include/battle.h"
#include "../include/pokedex.h"

#include "../include/item.h"
#include "../include/task.h"
#include "../include/sprite.h"
#include "../include/gba/m4a_internal.h"
#include "../include/window.h"

#include "../include/new/ram_locs.h"
#include "../include/new/rom_locs.h"
#include "../include/new/Vanilla_functions.h"

#define IS_EVEN(val) (umodsi(val, 2) == 0)
#define IS_ODD(val) (umodsi(val, 2) != 0)

#define uBYTE0_16(value)(( (u8) (((u16)(value) & (0x000000FF)) >> 0x00)))
#define uBYTE1_16(value)(( (u8) (((u16)(value) & (0x0000FF00)) >> 0x08)))

#define EOS 0xFF

#ifdef OPEN_WORLD_TRAINERS

#define NUM_BADGE_OPTIONS 8 + 2
#define NUM_MONS_PER_BADGE 10
#define DYNAMIC_TRAINER_LIMIT 0x59

#endif