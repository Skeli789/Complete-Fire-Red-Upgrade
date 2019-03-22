#pragma once

#include "defines.h"

struct DNSPalFade
{
	u16 colour;
	u8 amount;
};

struct SpecificTilesetFade
{
	u32 tilesetPointer;
	u8 paletteNumToFade;
	u8 paletteIndexToFade;
	u16 colourToFade;
};

//Change These
#define OW_BG_PAL_0 TRUE
#define OW_BG_PAL_1 TRUE
#define OW_BG_PAL_2 TRUE
#define OW_BG_PAL_3 TRUE
#define OW_BG_PAL_4 TRUE
#define OW_BG_PAL_5 TRUE
#define OW_BG_PAL_6 TRUE
#define OW_BG_PAL_7 TRUE
#define OW_BG_PAL_8 TRUE
#define OW_BG_PAL_9 TRUE
#define OW_BG_PAL_10 TRUE
#define OW_BG_PAL_11 TRUE
#define OW_BG_PAL_12 FALSE
#define OW_BG_PAL_13 FALSE
#define OW_BG_PAL_14 FALSE
#define OW_BG_PAL_15 FALSE

#define OW_SPRITE_PAL_0 TRUE
#define OW_SPRITE_PAL_1 TRUE
#define OW_SPRITE_PAL_2 TRUE
#define OW_SPRITE_PAL_3 TRUE
#define OW_SPRITE_PAL_4 TRUE
#define OW_SPRITE_PAL_5 TRUE
#define OW_SPRITE_PAL_6 TRUE
#define OW_SPRITE_PAL_7 TRUE
#define OW_SPRITE_PAL_8 TRUE
#define OW_SPRITE_PAL_9 TRUE
#define OW_SPRITE_PAL_10 TRUE
#define OW_SPRITE_PAL_11 TRUE
#define OW_SPRITE_PAL_12 TRUE
#define OW_SPRITE_PAL_13 TRUE
#define OW_SPRITE_PAL_14 TRUE
#define OW_SPRITE_PAL_15 TRUE

#define BBG_PAL_0 FALSE
#define BBG_PAL_1 FALSE
#define BBG_PAL_2 TRUE
#define BBG_PAL_3 TRUE
#define BBG_PAL_4 TRUE
#define BBG_PAL_5 FALSE
#define BBG_PAL_6 FALSE
#define BBG_PAL_7 FALSE
#define BBG_PAL_8 FALSE
#define BBG_PAL_9 FALSE
#define BBG_PAL_10 FALSE
#define BBG_PAL_11 FALSE
#define BBG_PAL_12 FALSE
#define BBG_PAL_13 FALSE
#define BBG_PAL_14 FALSE
#define BBG_PAL_15 FALSE

#define B_SPRITE_PAL_0 FALSE
#define B_SPRITE_PAL_1 FALSE
#define B_SPRITE_PAL_2 FALSE
#define B_SPRITE_PAL_3 FALSE
#define B_SPRITE_PAL_4 FALSE
#define B_SPRITE_PAL_5 FALSE
#define B_SPRITE_PAL_6 FALSE
#define B_SPRITE_PAL_7 FALSE
#define B_SPRITE_PAL_8 FALSE
#define B_SPRITE_PAL_9 FALSE
#define B_SPRITE_PAL_10 FALSE
#define B_SPRITE_PAL_11 FALSE
#define B_SPRITE_PAL_12 FALSE
#define B_SPRITE_PAL_13 FALSE
#define B_SPRITE_PAL_14 FALSE
#define B_SPRITE_PAL_15 FALSE
//The above values are set to only change the battle background.

#define OBG_SHI(num) (OW_BG_PAL_##num << num)
#define OSP_SHI(num) (OW_SPRITE_PAL_##num << (num + 16))
#define BBG_SHI(num) (BBG_PAL_##num << num)
#define BSP_SHI(num) (B_SPRITE_PAL_##num << (num + 16))

#define OW_DNS_BG_PAL_FADE (OBG_SHI(0) | OBG_SHI(1) | OBG_SHI(2) | OBG_SHI(3) | OBG_SHI(4) | OBG_SHI(5) 		\
						 | OBG_SHI(6) | OBG_SHI(7) | OBG_SHI(8) | OBG_SHI(9) | OBG_SHI(10) | OBG_SHI(11) 	\
						 | OBG_SHI(12) | OBG_SHI(13) | OBG_SHI(14) | OBG_SHI(15) 							\
						 | OSP_SHI(0) | OSP_SHI(1) | OSP_SHI(2) | OSP_SHI(3) | OSP_SHI(4) | OSP_SHI(5) 		\
						 | OSP_SHI(6) | OSP_SHI(7) | OSP_SHI(8) | OSP_SHI(9) | OSP_SHI(10) | OSP_SHI(11) 	\
						 | OSP_SHI(12) | OSP_SHI(13) | OSP_SHI(14) | OSP_SHI(15))


#define BATTLE_DNS_PAL_FADE (BBG_SHI(0) | BBG_SHI(1) | BBG_SHI(2) | BBG_SHI(3) | BBG_SHI(4) | BBG_SHI(5) 	\
						 | BBG_SHI(6) | BBG_SHI(7) | BBG_SHI(8) | BBG_SHI(9) | BBG_SHI(10) | BBG_SHI(11) 	\
						 | BBG_SHI(12) | BBG_SHI(13) | BBG_SHI(14) | BBG_SHI(15)							\
						 | BSP_SHI(0) | BSP_SHI(1) | BSP_SHI(2) | BSP_SHI(3) | BSP_SHI(4) | BSP_SHI(5) 		\
						 | BSP_SHI(6) | BSP_SHI(7) | BSP_SHI(8) | BSP_SHI(9) | BSP_SHI(10) | BSP_SHI(11)	\
						 | BSP_SHI(12) | BSP_SHI(13) | BSP_SHI(14) | BSP_SHI(15))

const struct DNSPalFade gDNSNightFadingByTime[24][6] =
{
	[0]  = {
			{RGB(1, 0, 17), 6},		//12:00 AM
			{RGB(1, 0, 17), 6},		//12:10 AM
			{RGB(1, 0, 17), 6},		//12:20 AM
			{RGB(1, 0, 17), 6},		//12:30 AM
			{RGB(1, 0, 17), 6},		//12:40 AM
			{RGB(1, 0, 17), 6},		//12:50 AM
		   },
	
	[1]  = {
			{RGB(1, 0, 17), 6},		//1:00 AM
			{RGB(1, 0, 17), 6},		//1:10 AM
			{RGB(1, 0, 17), 6},		//1:20 AM
			{RGB(1, 0, 17), 6},		//1:30 AM
			{RGB(1, 0, 17), 6},		//1:40 AM
			{RGB(1, 0, 17), 6},		//1:50 AM
		   },

	[2]  = {
			{RGB(1, 0, 17), 6},		//2:00 AM
			{RGB(1, 0, 17), 6},		//2:10 AM
			{RGB(1, 0, 17), 6},		//2:20 AM
			{RGB(1, 0, 17), 6},		//2:30 AM
			{RGB(1, 0, 17), 6},		//2:40 AM
			{RGB(1, 0, 17), 6},		//2:50 AM
		   },

	[3]  = {
			{RGB(1, 0, 17), 5},		//3:00 AM
			{RGB(1, 0, 17), 5},		//3:10 AM
			{RGB(1, 0, 17), 5},		//3:20 AM
			{RGB(1, 0, 17), 5},		//3:30 AM
			{RGB(1, 0, 17), 5},		//3:40 AM
			{RGB(1, 0, 17), 5},		//3:50 AM
		   },

	[4]  = {
			{RGB(0, 6, 16), 4},		//4:00 AM
			{RGB(0, 7, 16), 4},		//4:10 AM
			{RGB(0, 8, 16), 4},		//4:20 AM
			{RGB(0, 9, 16), 4},		//4:30 AM
			{RGB(0, 10, 16), 4},	//4:40 AM
			{RGB(0, 11, 16), 4},	//4:50 AM
		   },

	[5]  = {
			{RGB(0, 12, 16), 3},	//5:00 AM
			{RGB(0, 13, 16), 3},	//5:10 AM
			{RGB(0, 14, 16), 3},	//5:20 AM
			{RGB(0, 15, 16), 3},	//5:30 AM
			{RGB(0, 16, 16), 3},	//5:40 AM
			{RGB(0, 17, 16), 3},	//5:50 AM
		   },

	[6]  = {
			{RGB(0, 18, 16), 2},	//6:00 AM
			{RGB(0, 19, 16), 2},	//6:10 AM
			{RGB(0, 20, 16), 2},	//6:20 AM
			{RGB(0, 21, 16), 2},	//6:30 AM
			{RGB(0, 22, 16), 2},	//6:40 AM
			{RGB(0, 23, 16), 2},	//6:50 AM
		   },

	[7]  = {
			{RGB(0, 24, 16), 1},	//7:00 AM
			{RGB(0, 24, 16), 1},	//7:10 AM
			{RGB(0, 24, 16), 1},	//7:20 AM
			{RGB(0, 24, 16), 1},	//7:30 AM
			{RGB(0, 24, 16), 1},	//7:40 AM
			{RGB(0, 24, 16), 1},	//7:50 AM
		   },

//Day has no fade

	[17]  = {
			{RGB(19, 0, 10), 1},	//5:00 PM
			{RGB(19, 0, 10), 1},	//5:10 PM
			{RGB(19, 0, 10), 1},	//5:20 PM
			{RGB(19, 0, 10), 1},	//5:30 PM
			{RGB(19, 0, 10), 1},	//5:40 PM
			{RGB(19, 0, 10), 1},	//5:50 PM
		   },
	

	[18]  = {
			{RGB(18, 0, 11), 2},	//6:00 PM
			{RGB(17, 0, 12), 2},	//6:10 PM
			{RGB(16, 0, 13), 2},	//6:20 PM
			{RGB(15, 0, 14), 2},	//6:30 PM
			{RGB(14, 0, 15), 2},	//6:40 PM
			{RGB(13, 0, 16), 2},	//6:50 PM
		   },

	[19]  = {
			{RGB(12, 0, 17), 3},	//7:00 PM
			{RGB(11, 0, 17), 3},	//7:10 PM
			{RGB(10, 0, 18), 3},	//7:20 PM
			{RGB(9, 0, 18), 3},		//7:30 PM
			{RGB(8, 0, 19), 3},		//7:40 PM
			{RGB(7, 0, 19), 3},		//7:50 PM
		   },

	[20]  = {
			{RGB(5, 0, 21), 3},		//8:00 PM
			{RGB(5, 0, 21), 3},		//8:10 PM
			{RGB(5, 0, 21), 3},		//8:20 PM
			{RGB(5, 0, 21), 3},		//8:30 PM
			{RGB(5, 0, 21), 3},		//8:40 PM
			{RGB(5, 0, 21), 3},		//8:50 PM
		   },

	[21]  = {
			{RGB(5, 0, 21), 4},		//9:00 PM
			{RGB(5, 0, 21), 4},		//9:10 PM
			{RGB(5, 0, 21), 4},		//9:20 PM
			{RGB(4, 0, 20), 4},		//9:30 PM
			{RGB(4, 0, 20), 4},		//9:40 PM
			{RGB(4, 0, 20), 4},		//9:50 PM
		   },

	[22]  = {
			{RGB(3, 0, 19), 5},		//10:00 PM
			{RGB(3, 0, 19), 5},		//10:10 PM
			{RGB(3, 0, 19), 5},		//10:20 PM
			{RGB(2, 0, 18), 5},		//10:30 PM
			{RGB(2, 0, 18), 5},		//10:40 PM
			{RGB(2, 0, 18), 5},		//10:50 PM
		   },

	[23]  = {
			{RGB(1, 0, 17), 5},		//11:00 PM
			{RGB(1, 0, 17), 5},		//11:10 PM
			{RGB(1, 0, 17), 5},		//11:20 PM
			{RGB(1, 0, 17), 5},		//11:30 PM
			{RGB(1, 0, 17), 5},		//11:40 PM
			{RGB(1, 0, 17), 5},		//11:50 PM
		   },
};

#ifndef UNBOUND //Change This

const struct SpecificTilesetFade gSpecificTilesetFades[] =
{
	{ //Palette Town - Player's Door
		.tilesetPointer = 0x82D4AAC,
		.paletteNumToFade = 8,
		.paletteIndexToFade = 8,
		.colourToFade = RGB(31, 31, 20),
	},
	{ //Palette Town - Player's Door
		.tilesetPointer = 0x82D4AAC,
		.paletteNumToFade = 8,
		.paletteIndexToFade = 9,
		.colourToFade = RGB(31, 31, 11),
	},
	{ //Palette Town - Player's Door
		.tilesetPointer = 0x82D4AAC,
		.paletteNumToFade = 8,
		.paletteIndexToFade = 10,
		.colourToFade = RGB(31, 31, 1),
	},
};

#else

const struct SpecificTilesetFade gSpecificTilesetFades[] =
{
	{ //Gym
		.tilesetPointer = 0x82D4A94,
		.paletteNumToFade = 6,
		.paletteIndexToFade = 2,
		.colourToFade = RGB(31, 31, 20),
	},
	{ //Gym
		.tilesetPointer = 0x82D4A94,
		.paletteNumToFade = 6,
		.paletteIndexToFade = 3,
		.colourToFade = RGB(31, 31, 11),
	},
	{ //Gym
		.tilesetPointer = 0x82D4A94,
		.paletteNumToFade = 6,
		.paletteIndexToFade = 6,
		.colourToFade = RGB(31, 30, 1),
	},
	{ //Dehara Game Corner
		.tilesetPointer = 0x82D4B3C,
		.paletteNumToFade = 8,
		.paletteIndexToFade = 5,
		.colourToFade = RGB(14, 23, 26),
	},
	{ //Dehara Game Corner
		.tilesetPointer = 0x82D4B3C,
		.paletteNumToFade = 8,
		.paletteIndexToFade = 6,
		.colourToFade = RGB(9, 18, 23),
	},
	{ //Dehara Game Corner
		.tilesetPointer = 0x82D4B3C,
		.paletteNumToFade = 8,
		.paletteIndexToFade = 7,
		.colourToFade = RGB(0, 14, 18),
	},
	{ //Dehara Game Corner
		.tilesetPointer = 0x82D4B3C,
		.paletteNumToFade = 8,
		.paletteIndexToFade = 8,
		.colourToFade = RGB(28, 28, 29),
	},
	{ //Dehara Game Corner
		.tilesetPointer = 0x82D4B3C,
		.paletteNumToFade = 8,
		.paletteIndexToFade = 14,
		.colourToFade = RGB(29, 30, 6),
	},
	{ //Dehara Game Corner
		.tilesetPointer = 0x82D4B3C,
		.paletteNumToFade = 8,
		.paletteIndexToFade = 15,
		.colourToFade = RGB(24, 25, 1),
	},	
};

#endif