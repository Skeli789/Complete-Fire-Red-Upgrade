#pragma once

#include "../../src/config.h"

#define MAP_NONE (0x7F | (0x7F << 8))
#define MAP_UNDEFINED (0xFF | (0xFF << 8))

#define MAP_GROUP(map) (MAP_##map >> 8)
#define MAP_NUM(map) (MAP_##map & 0xFF)

#ifndef UNBOUND //For Fire Red

#define MAP_ROUTE_1  	((3 << 8) | 19)
#define MAP_ROUTE_2  	((3 << 8) | 20)
#define MAP_ROUTE_3  	((3 << 8) | 21)
#define MAP_ROUTE_4  	((3 << 8) | 22)
#define MAP_ROUTE_5  	((3 << 8) | 23)
#define MAP_ROUTE_6  	((3 << 8) | 24)
#define MAP_ROUTE_7 	((3 << 8) | 25)
#define MAP_ROUTE_8  	((3 << 8) | 26)
#define MAP_ROUTE_9 	((3 << 8) | 27)
#define MAP_ROUTE_10	((3 << 8) | 28)
#define MAP_ROUTE_11 	((3 << 8) | 29)
#define MAP_ROUTE_12 	((3 << 8) | 30)
#define MAP_ROUTE_13 	((3 << 8) | 31)
#define MAP_ROUTE_14 	((3 << 8) | 32)
#define MAP_ROUTE_15 	((3 << 8) | 33)
#define MAP_ROUTE_16 	((3 << 8) | 34)
#define MAP_ROUTE_17 	((3 << 8) | 35)
#define MAP_ROUTE_18 	((3 << 8) | 36)
#define MAP_ROUTE_19 	((3 << 8) | 37)
#define MAP_ROUTE_20 	((3 << 8) | 38)
#define MAP_ROUTE_21_A 	((3 << 8) | 39)
#define MAP_ROUTE_21_B 	((3 << 8) | 40)
#define MAP_ROUTE_22 	((3 << 8) | 41)
#define MAP_ROUTE_23 	((3 << 8) | 42)
#define MAP_ROUTE_24 	((3 << 8) | 43)
#define MAP_ROUTE_25 	((3 << 8) | 44)

#else //For Pokemon Unbound

#define MAP_ROUTE_1  			((3 << 8) | 67)
#define MAP_ROUTE_2				((3 << 8) | 69)
#define MAP_ROUTE_3				((3 << 8) | 70)
#define MAP_ROUTE_4				((3 << 8) | 72)
#define MAP_GRIM_WOODS			((45 << 8) | 1)
#define MAP_ROUTE_5				((3 << 8) | 73)
#define MAP_ROUTE_6				((3 << 8) | 75)
#define MAP_ROUTE_7				((3 << 8) | 76)
#define MAP_ROUTE_8				((3 << 8) | 77)
#define MAP_FROZEN_FOREST		((1 << 8) | 81)
#define MAP_ROUTE_9				((3 << 8) | 79)
#define MAP_ROUTE_10			((3 << 8) | 81)
#define MAP_AUBURN_WATERWAY_A	((3 << 8) | 82)
#define MAP_AUBURN_WATERWAY_B	((3 << 8) | 83)
#define MAP_ROUTE_11			((3 << 8) | 85)
#define MAP_ROUTE_12			((3 << 8) | 88)
#define MAP_ROUTE_13_A			((3 << 8) | 93)
#define MAP_ROUTE_13_B			((3 << 8) | 94)
#define MAP_ROUTE_14_A			((3 << 8) | 95)
#define MAP_ROUTE_14_B			((3 << 8) | 96)
#define MAP_ROUTE_14_C			((3 << 8) | 97)
#define MAP_VIVILL_WOODS		((1 << 8) | 99)
#define MAP_ROUTE_15			((3 << 8) | 100)
#define MAP_ROUTE_16			((3 << 8) | 101)
#define MAP_ROUTE_17			((3 << 8) | 104)

#define MAP_ROUTE_13    ((3 << 8) | 67)
#define MAP_ROUTE_14    ((3 << 8) | 67)
#define MAP_ROUTE_18 	((3 << 8) | 67)
#define MAP_ROUTE_19 	((3 << 8) | 67)
#define MAP_ROUTE_20 	((3 << 8) | 67)
#define MAP_ROUTE_21_A 	((3 << 8) | 67)
#define MAP_ROUTE_21_B 	((3 << 8) | 67)
#define MAP_ROUTE_22 	((3 << 8) | 67)
#define MAP_ROUTE_23 	((3 << 8) | 67)
#define MAP_ROUTE_24 	((3 << 8) | 67)
#define MAP_ROUTE_25 	((3 << 8) | 67)

#define MAP_FLOWER_PARADISE_A ((1 << 8) | 4)
#define MAP_FLOWER_PARADISE_B ((1 << 8) | 5)
#define MAP_FLOWER_PARADISE_C ((1 << 8) | 6)

#endif

u8 __attribute__((long_call)) GetCurrentRegionMapSectionId(void);
