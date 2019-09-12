#pragma once

#include "../global.h"

#define tMaxShowed 		data[0]
#define tTilemapTop 	data[2]
#define tTilemapLeft 	data[3]
#define tHeight			data[5]

#define MIN_NUM_SHOWED 2
#define MAX_NUM_SHOWED 9

// Text
extern const u8 sExampleText_1[];
extern const u8 sExampleText_2[];
extern const u8 sExampleText_3[];
extern const u8 sExampleText_4[];
extern const u8 sExampleText_5[];
extern const u8 sExampleText_6[];
extern const u8 sExampleText_7[];
extern const u8 sExampleText_8[];
extern const u8 sExampleText_9[];
extern const u8 sExampleText_10[];


struct ScrollingMulti {
	const u8** set;
	u8 count;
};




