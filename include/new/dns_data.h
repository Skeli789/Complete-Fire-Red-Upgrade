#pragma once

#include "../global.h"

/**
 * \file dns_data.h
 * \brief A file to be included only by "src/dns.c". It contains constants and
 *		  data structures relating to the palette fading of the day/night system.
 */

//Change These
//Overworld Palettes
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
#define OW_BG_PAL_12 TRUE
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

//Battle Palettes
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

//DON"T TOUCH THESE
#define OBG_SHI(num) (OW_BG_PAL_##num << num)
#define OSP_SHI(num) (OW_SPRITE_PAL_##num << (num + 16))
#define BBG_SHI(num) (BBG_PAL_##num << num)
#define BSP_SHI(num) (B_SPRITE_PAL_##num << (num + 16))

#define OW_DNS_PAL_FADE (OW_DNS_BG_PAL_FADE | OW_DNS_SPRITE_PAL_FADE)

#define OW_DNS_BG_PAL_FADE (OBG_SHI(0) | OBG_SHI(1) | OBG_SHI(2) | OBG_SHI(3) | OBG_SHI(4) | OBG_SHI(5) 	\
						 | OBG_SHI(6) | OBG_SHI(7) | OBG_SHI(8) | OBG_SHI(9) | OBG_SHI(10) | OBG_SHI(11) 	\
						 | OBG_SHI(12))

#define OW_DNS_SPRITE_PAL_FADE (OSP_SHI(0) | OSP_SHI(1) | OSP_SHI(2) | OSP_SHI(3) | OSP_SHI(4) | OSP_SHI(5) \
						 | OSP_SHI(6) | OSP_SHI(7) | OSP_SHI(8) | OSP_SHI(9) | OSP_SHI(10) | OSP_SHI(11) 	\
						 | OSP_SHI(12) | OSP_SHI(13) | OSP_SHI(14) | OSP_SHI(15))

#define BATTLE_DNS_PAL_FADE (BBG_SHI(0) | BBG_SHI(1) | BBG_SHI(2) | BBG_SHI(3) | BBG_SHI(4) | BBG_SHI(5) 	\
						 | BBG_SHI(6) | BBG_SHI(7) | BBG_SHI(8) | BBG_SHI(9) | BBG_SHI(10) | BBG_SHI(11) 	\
						 | BBG_SHI(12) | BBG_SHI(13) | BBG_SHI(14) | BBG_SHI(15)							\
						 | BSP_SHI(0) | BSP_SHI(1) | BSP_SHI(2) | BSP_SHI(3) | BSP_SHI(4) | BSP_SHI(5) 		\
						 | BSP_SHI(6) | BSP_SHI(7) | BSP_SHI(8) | BSP_SHI(9) | BSP_SHI(10) | BSP_SHI(11)	\
						 | BSP_SHI(12) | BSP_SHI(13) | BSP_SHI(14) | BSP_SHI(15))

//The max value for the coefficients (right number) is 16. DO NOT SET ABOVE THIS NUMBER!
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

#define TILESET_PAL_FADE_END {0xFF, 0x0}

#ifndef UNBOUND //Change This

const struct SpecificTilesetFade gSpecificTilesetFades[] =
{
	//These Palette Town ones have been left in as examples. Feel free to remove.
	{ //Palette Town - Player's Door
		.tilesetPointer = 0x82D4AAC, //Tileset 1
		.paletteNumToFade = 8,
		.paletteIndicesToFade =
		{
			{8,  RGB(31, 31, 20)},
			{9,  RGB(31, 31, 11)},
			{10, RGB(31, 31, 10)},
			TILESET_PAL_FADE_END
		},
	},
	{ //Palette Town - Oak's Lab Windows
		.tilesetPointer = 0x82D4AAC, //Tileset 1
		.paletteNumToFade = 9,
		.paletteIndicesToFade =
		{
			{8,  RGB(31, 31, 20)},
			{9,  RGB(31, 31, 14)},
			{10, RGB(31, 30, 0)},
			TILESET_PAL_FADE_END
		},
	},
	{ //Palette Town - Oak's Lab Door
		.tilesetPointer = 0x82D4AAC, //Tileset 1
		.paletteNumToFade = 10,
		.paletteIndicesToFade =
		{
			{8,  RGB(31, 31, 20)},
			{9,  RGB(31, 31, 14)},
			{10, RGB(31, 30, 0)},
			TILESET_PAL_FADE_END
		},
	},
};

#else //For Pokemon Unbound - Feel free to remove

const struct SpecificTilesetFade gSpecificTilesetFades[] =
{
	{ //Gym
		.tilesetPointer = 0x82D4A94,
		.paletteNumToFade = 6,
		.paletteIndicesToFade =
		{
			{2, RGB(31, 27, 10)}, //old RGB(31, 31, 20)
			{3, RGB(29, 24, 5)}, //old RGB(31, 31, 11)
			{6, RGB(29, 24, 0)}, //old RGB(31, 30, 1)
			TILESET_PAL_FADE_END
		},
	},
	{ //Frozen Heights - Lab
		.tilesetPointer = 0x873230C,
		.paletteNumToFade = 7,
		.paletteIndicesToFade =
		{
			
			{13, RGB(31, 27, 10)}, //old RGB(31, 30, 25)
			{14, RGB(29, 24, 5)},  //old RGB(31, 30, 17)
			{15, RGB(29, 24, 0)},  //old RGB(31, 30, 12)
			TILESET_PAL_FADE_END
		},
	},
	{ //Frozen Heights - House Top
		.tilesetPointer = 0x873230C,
		.paletteNumToFade = 9,
		.paletteIndicesToFade =
		{
			{5,  RGB(31, 27, 10)}, //old RGB(31, 30, 25)
			{6,  RGB(29, 24, 5)},  //old RGB(31, 30, 17)
			{7,  RGB(29, 24, 0)},  //old RGB(31, 30, 12)
			{8,  RGB(29, 22, 4)},  //old RGB(31, 30, 0)
			TILESET_PAL_FADE_END
		},
	},
	{ //Frozen Heights - House Bottom
		.tilesetPointer = 0x873230C,
		.paletteNumToFade = 10,
		.paletteIndicesToFade =
		{
			{4,  RGB(31, 27, 10)}, //old RGB(31, 30, 25)
			{5,  RGB(29, 24, 0)},  //old RGB(31, 30, 12)
			{6,  RGB(29, 22, 4)},  //old RGB(31, 30, 0)
			TILESET_PAL_FADE_END
		},
	},
	{ //Snow Town Marts
		.tilesetPointer = 0x8725AB4, //Tileset 188588
		.paletteNumToFade = 3,
		.paletteIndicesToFade =
		{
			{5,  RGB(25, 19, 4)},	//old RGB(27, 27, 0) 
			{13, RGB(29, 23, 4)},	//old RGB(31, 31, 0)
			TILESET_PAL_FADE_END
		},
	},
	{ //Snow Town Pokemon Centre Door
		.tilesetPointer = 0x8725AB4, //Tileset 188588
		.paletteNumToFade = 6,
		.paletteIndicesToFade =
		{
			{10,  RGB(31, 27, 5)}, //old RGB(31, 31, 20)
			{12,  RGB(29, 24, 5)}, //old RGB(31, 31, 14)
			{13,  RGB(29, 22, 5)}, //old RGB(31, 30, 1)
			TILESET_PAL_FADE_END
		},
	},
	{ //Bellin Town Lamp
		.tilesetPointer = 0x82D4AC4, //Tileset 2
		.paletteNumToFade = 9,
		.paletteIndicesToFade =
		{
			{3,  RGB(29, 22, 5)},	//old RGB(31, 30, 1)
			{10, RGB(28, 20, 4)},	//old RGB(30, 28, 0)
			{13, RGB(29, 24, 5)},	//old RGB(31, 31, 14)
			TILESET_PAL_FADE_END
		},
	},
	{ //Snow Town House Windows
		.tilesetPointer = 0x82D4AC4, //Tileset 2
		.paletteNumToFade = 12,
		.paletteIndicesToFade =
		{
			{4,  RGB(29, 22, 4)}, 	//old RGB(31, 30, 0)
			{8,  RGB(28, 20, 4)},	//old RGB(30, 28, 0)
			TILESET_PAL_FADE_END
		},
	},
	{ //Main Pokemon Centre Windows
		.tilesetPointer = 0x82D4A94, //Tileset 0
		.paletteNumToFade = 2,
		.paletteIndicesToFade =
		{
			{7,  RGB(29, 24, 5)}, //old RGB(31, 31, 14)
			{15, RGB(29, 22, 4)}, //old RGB(31, 30, 0)
			TILESET_PAL_FADE_END
		},
	},
	/* Copy from here
	{ //Main Night Tiles
		.tilesetPointer = 0x82D4A94, //Tileset 0
		.paletteNumToFade = 12,
		.paletteIndicesToFade =
		{
			{8,  RGB(29, 24, 5)}, //old RGB(31, 31, 14)
			{9,  RGB(29, 22, 4)}, //old RGB(31, 30, 0)
			{10, RGB(31, 27, 5)}, //old RGB(31, 31, 20)
			{12, RGB(29, 24, 5)}, //old RGB(31, 31, 14)
			{13, RGB(29, 22, 5)}, //old RGB(31, 30, 1)
			{14, RGB(29, 22, 4)}, //old RGB(31, 30, 0)
			TILESET_PAL_FADE_END
		},
	},
	*/
	/*{ //Dresco Town Moltres - Issue: Lights up bench too :(
		.tilesetPointer = 0x82D4AF4, //Tileset 4
		.paletteNumToFade = 7,
		.paletteIndicesToFade =
		{
			{1,  RGB(26 + 4, 11 + 4, 10 + 4)},
			{3,  RGB(24 + 4, 8 + 4, 7 + 4)},
			{4,  RGB(28 + 3, 16 + 4, 12 + 4)},
			{8,  RGB(26 + 4, 9 + 4, 8 + 4)},
			{10, RGB(11 + 4, 4 + 4, 3 + 4)},
			{11, RGB(14 + 4, 7 + 4, 7 + 4)},
			{13, RGB(19 + 4, 3 + 4, 4 + 4)},
			TILESET_PAL_FADE_END
		},
	},*/
	{ //Dresco Town Battle House Tiles
		.tilesetPointer = 0x82D4AF4, //Tileset 4
		.paletteNumToFade = 11,
		.paletteIndicesToFade =
		{
			{1,  RGB(29, 24, 5)}, //old RGB(31, 31, 14)
			{4,  RGB(29, 22, 5)}, //old RGB(31, 30, 1)
			TILESET_PAL_FADE_END
		},
	},
	{ //Dresco Town Pal 12 Tiles
		.tilesetPointer = 0x82D4AF4, //Tileset 4
		.paletteNumToFade = 12,
		.paletteIndicesToFade =
		{
			{8,  RGB(29, 24, 5)}, //old RGB(31, 31, 14)
			{9,  RGB(29, 22, 4)}, //old RGB(31, 30, 0)
			{10, RGB(31, 27, 5)}, //old RGB(31, 31, 20) 
			{11, RGB(29, 24, 2)}, //old RGB(31, 31, 17)
			{12, RGB(29, 24, 5)}, //old RGB(31, 31, 14)
			{13, RGB(29, 22, 5)}, //old RGB(31, 30, 1)
			{14, RGB(29, 22, 4)}, //old RGB(31, 30, 0)
			TILESET_PAL_FADE_END
		},
	},
	{ //Route 5 / Day-Care House
		.tilesetPointer = 0x8B56314, //Tileset 371632
		.paletteNumToFade = 11,
		.paletteIndicesToFade =
		{
			{5,  RGB(29, 24, 5)}, //old RGB(31, 31, 14)
			{7,  RGB(29, 22, 4)}, //old RGB(31, 30, 0)
			TILESET_PAL_FADE_END
		},
	},
	{ //Crater Town House Windows
		.tilesetPointer = 0x82D4ADC, //Tileset 3
		.paletteNumToFade = 7,
		.paletteIndicesToFade =
		{
			{4,  RGB(29, 24, 5)}, //old RGB(31, 31, 14)
			{9,  RGB(29, 22, 4)}, //old RGB(31, 30, 0)
			TILESET_PAL_FADE_END
		},
	},
	{ //Crater Town Lamps
		.tilesetPointer = 0x82D4ADC, //Tileset 3
		.paletteNumToFade = 9,
		.paletteIndicesToFade =
		{
			{8,  RGB(29, 24, 5)}, //old RGB(31, 31, 14)
			{15, RGB(29, 22, 4)}, //old RGB(31, 30, 0)
			TILESET_PAL_FADE_END
		},
	},
	{ //Crater Town Pal 12 Tiles
		.tilesetPointer = 0x82D4ADC, //Tileset 3
		.paletteNumToFade = 12,
		.paletteIndicesToFade =
		{
			{8,  RGB(29, 24, 5)}, //old RGB(31, 31, 14)
			{9,  RGB(29, 22, 4)}, //old RGB(31, 30, 0)
			{10, RGB(31, 27, 5)}, //old RGB(31, 31, 20)
			{12, RGB(29, 24, 5)}, //old RGB(31, 31, 14)
			{13, RGB(29, 22, 5)}, //old RGB(31, 30, 1)
			{14, RGB(29, 22, 4)}, //old RGB(31, 30, 0)
			TILESET_PAL_FADE_END
		},
	},
	{ //Blizzard City Gym Windows
		.tilesetPointer = 0x82D4AAC, //Tileset 1
		.paletteNumToFade = 8,
		.paletteIndicesToFade =
		{
			{8,  RGB(29, 22, 4)}, //old RGB(31, 30, 0)
			{10, RGB(24, 0, 0)},
			{11, RGB(21, 0, 0)},
			{12, RGB(16, 0, 0)},
			TILESET_PAL_FADE_END
		},
	},
	{ //Blizzard City Lamp
		.tilesetPointer = 0x82D4AAC, //Tileset 1
		.paletteNumToFade = 9,
		.paletteIndicesToFade =
		{
			{5,  RGB(31, 30, 17)},
			{6,  RGB(31, 30, 1)},
			{7,  RGB(31, 30, 0)},
			{8,  RGB(30, 28, 0)},
			{11, RGB(31, 31, 14)},
			TILESET_PAL_FADE_END
		},
	},
	{ //Blizzard City House Door
		.tilesetPointer = 0x82D4AAC, //Tileset 1
		.paletteNumToFade = 10,
		.paletteIndicesToFade =
		{
			{2, RGB(29, 22, 5)}, //old RGB(31, 30, 1)
			TILESET_PAL_FADE_END
		},
	},
	{ //Blizzard City Gym Sign
		.tilesetPointer = 0x82D4AAC, //Tileset 1
		.paletteNumToFade = 11,
		.paletteIndicesToFade =
		{
			{1, RGB(28, 16, 12)},
			{2, RGB(26, 11, 10)},
			{3, RGB(24, 8, 7)},
			TILESET_PAL_FADE_END
		},
	},
	{ //Blizzard City Windows
		.tilesetPointer = 0x82D4AAC, //Tileset 1
		.paletteNumToFade = 12,
		.paletteIndicesToFade =
		{
			{1, RGB(29, 26, 5)},	//old RGB(31, 30, 20)
			{2, RGB(29, 24, 5)},	//old RGB(31, 30, 17) 
			{3, RGB(29, 24, 2)},	//old RGB(31, 30, 14)
			{4, RGB(29, 22, 4)}, 	//old RGB(31, 30, 0)
			TILESET_PAL_FADE_END
		},
	},
	{ //Tehl Town House Windows
		.tilesetPointer = 0x873CC2C, //Tileset 192529
		.paletteNumToFade = 9,
		.paletteIndicesToFade =
		{
			{7,  RGB(31, 27, 5)}, //old RGB(31, 31, 20)
			{11, RGB(29, 22, 4)}, //old RGB(31, 30, 0)
			{12, RGB(29, 24, 5)}, //old RGB(31, 31, 14)
			TILESET_PAL_FADE_END
		},
	},
	{ //Tehl Town Pal 12 Tiles
		.tilesetPointer = 0x873CC2C, //Tileset 192529
		.paletteNumToFade = 12,
		.paletteIndicesToFade =
		{
			{8,  RGB(29, 24, 5)}, //old RGB(31, 31, 14)
			{9,  RGB(29, 22, 4)}, //old RGB(31, 30, 0)
			{10, RGB(31, 27, 5)}, //old RGB(31, 31, 20)
			{12, RGB(29, 24, 5)}, //old RGB(31, 31, 14)
			{13, RGB(29, 22, 5)}, //old RGB(31, 30, 1)
			{14, RGB(29, 22, 4)}, //old RGB(31, 30, 0)
			TILESET_PAL_FADE_END
		},
	},
	{ //Fallshore City House Windows
		.tilesetPointer = 0x8744D8C, //Tileset 193909
		.paletteNumToFade = 8,
		.paletteIndicesToFade =
		{
			{1,  RGB(31, 27, 5)}, //old RGB(31, 31, 24)
			{2,  RGB(31, 27, 5)}, //old RGB(31, 31, 20)
			{3,  RGB(29, 24, 5)}, //old RGB(31, 31, 14)
			{4,  RGB(29, 24, 2)}, //old RGB(31, 30, 11)
			{5,  RGB(29, 22, 5)}, //old RGB(31, 30, 1)
			TILESET_PAL_FADE_END
		},
	},
	{ //Fallshore City Pal 12 Tiles
		.tilesetPointer = 0x8744D8C, //Tileset 193909
		.paletteNumToFade = 12,
		.paletteIndicesToFade =
		{
			{3,  RGB(31, 31, 14)},
			{4,  RGB(31, 30, 0)},
			{8,  RGB(29, 24, 5)}, //old RGB(31, 31, 14)
			{9,  RGB(29, 22, 4)}, //old RGB(31, 30, 0)
			{10, RGB(31, 27, 5)}, //old RGB(31, 31, 20)
			{11, RGB(31, 30, 1)},
			{12, RGB(29, 24, 5)}, //old RGB(31, 31, 14)
			{13, RGB(29, 22, 5)}, //old RGB(31, 30, 1)
			{14, RGB(29, 22, 4)}, //old RGB(31, 30, 0)
			TILESET_PAL_FADE_END
		},
	},
	{ //Epidimy Town Lamps
		.tilesetPointer = 0x8B5419C, //Tileset 371275
		.paletteNumToFade = 8,
		.paletteIndicesToFade =
		{
			{1, RGB(31, 31, 14)},
			{2, RGB(31, 30, 0)},
			TILESET_PAL_FADE_END
		},
	},
	{ //Epidimy Town Pal 12 Tiles
		.tilesetPointer = 0x8B5419C, //Tileset 371275
		.paletteNumToFade = 12,
		.paletteIndicesToFade =
		{
			{1,  RGB(29, 24, 5)}, //old RGB(31, 31, 14)
			{2,  RGB(29, 22, 5)}, //old RGB(31, 30, 1)
			{8,  RGB(29, 24, 5)}, //old RGB(31, 31, 14)
			{9,  RGB(29, 22, 4)}, //old RGB(31, 30, 0)
			{10, RGB(31, 27, 5)}, //old RGB(31, 31, 20)
			{12, RGB(29, 24, 5)}, //old RGB(31, 31, 14)
			{13, RGB(29, 22, 5)}, //old RGB(31, 30, 1)
			{14, RGB(29, 22, 4)}, //old RGB(31, 30, 0)
			TILESET_PAL_FADE_END
		},
	},
	{ //Tarmigan House Windows
		.tilesetPointer = 0x82D4B6C, //Tileset 9
		.paletteNumToFade = 7,
		.paletteIndicesToFade =
		{
			{15, RGB(29, 24, 5)}, //old RGB(31, 31, 14)
			TILESET_PAL_FADE_END
		},
	},
	{ //Tarmigan Mansion Windows
		.tilesetPointer = 0x82D4B6C, //Tileset 9
		.paletteNumToFade = 8,
		.paletteIndicesToFade =
		{
			{15, RGB(29, 24, 5)}, //old RGB(31, 31, 14)
			TILESET_PAL_FADE_END
		},
	},
	{ //Dream Research Lab Windows
		.tilesetPointer = 0x82D4B6C, //Tileset 9
		.paletteNumToFade = 9,
		.paletteIndicesToFade =
		{
			{9,  RGB(29, 24, 5)}, //old RGB(31, 31, 14)
			{10, RGB(29, 22, 4)}, //old RGB(31, 30, 0)
			TILESET_PAL_FADE_END
		},
	},
	{ //Tarmigan Town Pal 12 Tiles
		.tilesetPointer = 0x82D4B6C, //Tileset 9
		.paletteNumToFade = 12,
		.paletteIndicesToFade =
		{
			{8,  RGB(29, 24, 5)}, //old RGB(31, 31, 14)
			{9,  RGB(29, 22, 4)}, //old RGB(31, 30, 0)
			{10, RGB(31, 27, 5)}, //old RGB(31, 31, 20)
			{12, RGB(29, 24, 5)}, //old RGB(31, 31, 14)
			{13, RGB(29, 22, 5)}, //old RGB(31, 30, 1)
			{14, RGB(29, 22, 4)}, //old RGB(31, 30, 0)
			TILESET_PAL_FADE_END
		},
	},
	{ //Dehara City Game Corner
		.tilesetPointer = 0x82D4B3C, //Tileset 7
		.paletteNumToFade = 8,
		.paletteIndicesToFade =
		{
			{5,  RGB(31, 27, 5)},	//old RGB(14, 23, 26)
			{6,  RGB(29, 24, 5)},	//old RGB(9, 18, 23)}
			{7,  RGB(29, 22, 5)},	//old RGB(0, 14, 18)
			{8,  RGB(28, 28, 29)},
			{14, RGB(29, 30, 6)},
			{15, RGB(24, 25, 1)},
			TILESET_PAL_FADE_END
		},
	},
	{ //Dehara City Lamps
		.tilesetPointer = 0x82D4B3C, //Tileset 7
		.paletteNumToFade = 10,
		.paletteIndicesToFade =
		{
			{1, RGB(31, 27, 5)}, //old RGB(31, 31, 20)
			{2, RGB(29, 24, 5)}, //old RGB(31, 31, 14)
			{3, RGB(29, 22, 5)}, //old RGB(31, 30, 1)
			TILESET_PAL_FADE_END
		},
	},
	{ //Dehara City Windows
		.tilesetPointer = 0x82D4B3C, //Tileset 7
		.paletteNumToFade = 11,
		.paletteIndicesToFade =
		{
			{12, RGB(31, 27, 5)}, //old RGB(31, 31, 20)
			{13, RGB(29, 24, 5)}, //old RGB(31, 31, 14)
			{14, RGB(29, 22, 5)}, //old RGB(31, 30, 1)
			{15, RGB(29, 22, 4)}, //old RGB(31, 30, 0)
			TILESET_PAL_FADE_END
		},
	},
	{ //Dehara City Pal 12 Tiles
		.tilesetPointer = 0x82D4B3C, //Tileset 7
		.paletteNumToFade = 12,
		.paletteIndicesToFade =
		{
			{8,  RGB(29, 24, 5)}, //old RGB(31, 31, 14)
			{9,  RGB(29, 22, 4)}, //old RGB(31, 30, 0)
			{10, RGB(31, 27, 5)}, //old RGB(31, 31, 20)
			{12, RGB(29, 24, 5)}, //old RGB(31, 31, 14)
			{13, RGB(29, 22, 5)}, //old RGB(31, 30, 1)
			{14, RGB(29, 22, 4)}, //old RGB(31, 30, 0)
			TILESET_PAL_FADE_END
		},
	},
	{ //Gurun Town House Window
		.tilesetPointer = 0x82D4B24, //Tileset 6
		.paletteNumToFade = 8,
		.paletteIndicesToFade =
		{
			{14, RGB(29, 24, 5)}, //old RGB(31, 31, 14)
			TILESET_PAL_FADE_END
		},
	},
	{ //Gurun Town Fire
		.tilesetPointer = 0x82D4B24, //Tileset 6
		.paletteNumToFade = 10,
		.paletteIndicesToFade =
		{
			{12, RGB(30, 30, 25)},
			{13, RGB(30, 29, 21)},
			{14, RGB(28, 25, 11)},
			TILESET_PAL_FADE_END
		},
	},
	{ //Gurun Town Pal 12 Tiles
		.tilesetPointer = 0x82D4B24, //Tileset 6
		.paletteNumToFade = 12,
		.paletteIndicesToFade =
		{
			{8,  RGB(29, 24, 5)}, //old RGB(31, 31, 14)
			{9,  RGB(29, 22, 4)}, //old RGB(31, 30, 0)
			{10, RGB(31, 27, 5)}, //old RGB(31, 31, 20)
			{12, RGB(29, 24, 5)}, //old RGB(31, 31, 14)
			{13, RGB(29, 22, 5)}, //old RGB(31, 30, 1)
			{14, RGB(29, 22, 4)}, //old RGB(31, 30, 0)
			TILESET_PAL_FADE_END
		},
	},
	{ //Vivill Town House Windows
		.tilesetPointer = 0x82D5064, //Tileset 62
		.paletteNumToFade = 7,
		.paletteIndicesToFade =
		{
			{13, RGB(31, 27, 5)}, //old RGB(31, 31, 20)
			{14, RGB(29, 24, 5)}, //old RGB(31, 31, 14)
			{15, RGB(29, 22, 4)}, //old RGB(31, 30, 0)
			TILESET_PAL_FADE_END
		},
	},
	{ //Vivill Town Warehouse Windows
		.tilesetPointer = 0x82D5064, //Tileset 62
		.paletteNumToFade = 8,
		.paletteIndicesToFade =
		{
			{14,  RGB(29, 24, 5)}, //old RGB(31, 31, 14)
			{15,  RGB(29, 22, 4)}, //old RGB(31, 30, 0)
			TILESET_PAL_FADE_END
		},
	},
	{ //Vivill Town Lamp
		.tilesetPointer = 0x82D5064, //Tileset 62
		.paletteNumToFade = 9,
		.paletteIndicesToFade =
		{
			{5,  RGB(31, 31, 14)},
			{6,  RGB(31, 30, 1)},
			{7,  RGB(30, 28, 0)},
			TILESET_PAL_FADE_END
		},
	},
	{ //Vivill Town Pal 12 Tiles
		.tilesetPointer = 0x82D5064, //Tileset 62
		.paletteNumToFade = 12,
		.paletteIndicesToFade =
		{
			{8,  RGB(29, 24, 5)}, //old RGB(31, 31, 14)
			{9,  RGB(29, 22, 4)}, //old RGB(31, 30, 0)
			{10, RGB(31, 27, 5)}, //old RGB(31, 31, 20)
			{12, RGB(29, 24, 5)}, //old RGB(31, 31, 14)
			{13, RGB(29, 22, 5)}, //old RGB(31, 30, 1)
			{14, RGB(29, 22, 4)}, //old RGB(31, 30, 0)
			TILESET_PAL_FADE_END
		},
	},
	{ //Antisis City House Windows
		.tilesetPointer = 0x82D4B0C, //Tileset 5
		.paletteNumToFade = 7,
		.paletteIndicesToFade =
		{
			{14, RGB(29, 24, 5)},	//old RGB(31, 31, 14)
			{15, RGB(29, 22, 5)},	//old RGB(31, 30, 1)
			TILESET_PAL_FADE_END
		},
	},
	{ //Antisis City Pal 12 Tiles
		.tilesetPointer = 0x82D4B0C, //Tileset 5
		.paletteNumToFade = 12,
		.paletteIndicesToFade =
		{
			{1,  RGB(31, 31, 14)},
			{2,  RGB(31, 30, 1)},

			{8,  RGB(29, 24, 5)},	//old RGB(31, 31, 14)
			{9,  RGB(29, 22, 4)},	//old RGB(31, 30, 0)
			{10, RGB(31, 27, 5)},   //old RGB(31, 31, 20)
			{12, RGB(29, 24, 5)},	//old RGB(31, 31, 14)
			{13, RGB(29, 22, 5)},	//old RGB(31, 30, 1)
			{14, RGB(29, 22, 4)},	//old RGB(31, 30, 0)
			TILESET_PAL_FADE_END
		},
	},
	{ //Antisis Port Lighthouse
		.tilesetPointer = 0x82D50AC, //Tileset 65
		.paletteNumToFade = 10,
		.paletteIndicesToFade =
		{
			{13, RGB(31, 30, 1)},
			TILESET_PAL_FADE_END
		},
	},
	{ //Battle Tower - the new basis for all the palettes
		.tilesetPointer = 0x82D4E3C, //Tileset 39
		.paletteNumToFade = 7,
		.paletteIndicesToFade =
		{
			{1,  RGB(31, 27, 5)}, //gradient from bright
			{2,  RGB(29, 24, 5)},
			{3,  RGB(29, 22, 5)},
			{4,  RGB(26, 20, 3)},
			{5,  RGB(23, 18, 2)},
			{6,  RGB(15, 11, 2)}, //gradient to dark
			TILESET_PAL_FADE_END
		},
	},
	{ //Battle Frontier Lamps
		.tilesetPointer = 0x82D4E3C, //Tileset 39
		.paletteNumToFade = 8,
		.paletteIndicesToFade =
		{
			{1,  RGB(29, 24, 5)}, //old RGB(31, 31, 14)
			{2,  RGB(29, 22, 4)}, //old RGB(31, 30, 0)
			TILESET_PAL_FADE_END
		},
	},
	{ //Battle Tent
		.tilesetPointer = 0x82D4E3C, //Tileset 39
		.paletteNumToFade = 10,
		.paletteIndicesToFade =
		{
			{7,  RGB(29, 25, 0)},
			{8,  RGB(23, 20, 0)},
			{9,  RGB(24, 15, 5)},
			{10,  RGB(19, 11, 2)},
			TILESET_PAL_FADE_END
		},
	},
	{ //Battle Frontier Sliding Doors
		.tilesetPointer = 0x82D4E3C, //Tileset 39
		.paletteNumToFade = 12,
		.paletteIndicesToFade =
		{
			{10,  RGB(31, 27, 5)},
			{12,  RGB(29, 24, 5)},
			{13,  RGB(29, 22, 5)},
			{14,  RGB(26, 20, 3)},
			TILESET_PAL_FADE_END
		},
	},
	{ //Seaport City Windows
		.tilesetPointer = 0x82D4C2C, //Tileset 17
		.paletteNumToFade = 7,
		.paletteIndicesToFade =
		{
			{12,  RGB(31, 27, 5)}, //old RGB(31, 31, 20)
			{13,  RGB(29, 24, 5)}, //old RGB(31, 31, 14)
			{14,  RGB(29, 22, 5)}, //old RGB(31, 30, 1)
			TILESET_PAL_FADE_END
		},
	},
	{ //Seaport City Pokemon Centre Doors
		.tilesetPointer = 0x82D4C2C, //Tileset 17
		.paletteNumToFade = 12,
		.paletteIndicesToFade =
		{
			{1,  RGB(31, 27, 5)}, //old RGB(31, 31, 20)
			{2,  RGB(29, 24, 5)}, //old RGB(31, 31, 14)
			{3,  RGB(29, 22, 5)}, //old RGB(31, 30, 1)

			{8,  RGB(29, 24, 5)},  //old RGB(31, 31, 14)
			{9,  RGB(29, 22, 4)},  //old RGB(31, 30, 0)
			{10,  RGB(31, 27, 5)}, //old RGB(31, 31, 20)
			{12,  RGB(29, 24, 5)}, //old RGB(31, 31, 14)
			{13,  RGB(29, 22, 5)}, //old RGB(31, 30, 1)
			{14,  RGB(29, 22, 4)}, //old RGB(31, 30, 0)
			TILESET_PAL_FADE_END
		},
	},
	{ //Seaport City Port
		.tilesetPointer = 0x82D4C5C, //Tileset 19
		.paletteNumToFade = 8,
		.paletteIndicesToFade =
		{
			{1,  RGB(8, 18, 20)},
			{2,  RGB(4, 14, 16)},
			TILESET_PAL_FADE_END
		},
	},
	{ //Polder Town
		.tilesetPointer = 0x82D4B54, //Tileset 8
		.paletteNumToFade = 12,
		.paletteIndicesToFade =
		{
			{1,  RGB(31, 27, 5)},  //old RGB(31, 31, 20)
			{2,  RGB(29, 24, 5)},  //old RGB(31, 31, 14)
			{3,  RGB(29, 22, 5)},  //old RGB(31, 30, 1)

			{11,  RGB(29, 24, 5)}, //old RGB(31, 31, 14)

			{8,  RGB(29, 24, 5)},  //old RGB(31, 31, 14)
			{9,  RGB(29, 22, 4)},  //old RGB(31, 30, 0)
			{10,  RGB(31, 27, 5)}, //old RGB(31, 31, 20)
			{12,  RGB(29, 24, 5)}, //old RGB(31, 31, 14)
			{13,  RGB(29, 22, 5)}, //old RGB(31, 30, 1)
			{14,  RGB(29, 22, 4)}, //old RGB(31, 30, 0)
			TILESET_PAL_FADE_END
		},
	},
	{ //Crystal Peak - Blue Crystals
		.tilesetPointer = 0x82D4E0C, //Tileset 37
		.paletteNumToFade = 8,
		.paletteIndicesToFade =
		{
			{1,  RGB(24, 27, 30)},
			{2,  RGB(19, 23, 30)},
			{3,  RGB(15, 20, 29)},
			{4,  RGB(11, 18, 27)},
			{5,  RGB(8, 15, 26)},
			{6,  RGB(5, 13, 24)},
			{7,  RGB(3, 11, 21)},
			TILESET_PAL_FADE_END
		},
	},
	{ //Crystal Peak - Red Crystals
		.tilesetPointer = 0x82D4E0C, //Tileset 37
		.paletteNumToFade = 9,
		.paletteIndicesToFade =
		{
			{1,  RGB(30, 25, 24)},
			{2,  RGB(30, 21, 19)},
			{3,  RGB(29, 17, 15)},
			{4,  RGB(27, 14, 11)},
			{5,  RGB(26, 12, 8)},
			{6,  RGB(24, 9, 5)},
			{7,  RGB(21, 6, 3)},
			TILESET_PAL_FADE_END
		},
	},
	{ //Crystal Peak - Yellow Crystals
		.tilesetPointer = 0x82D4E0C, //Tileset 37
		.paletteNumToFade = 10,
		.paletteIndicesToFade =
		{
			{1,  RGB(30, 30, 18)},
			{2,  RGB(28, 27, 14)},
			{3,  RGB(26, 24, 10)},
			{4,  RGB(23, 22, 7)},
			{5,  RGB(21, 19, 4)},
			{6,  RGB(19, 16, 2)},
			{7,  RGB(17, 13, 0)},
			TILESET_PAL_FADE_END
		},
	},
	{ //Magnolia Town Windows
		.tilesetPointer = 0x82D4D04, //Tileset 26
		.paletteNumToFade = 8,
		.paletteIndicesToFade =
		{
			{1,  RGB(29, 24, 5)}, //old RGB(31, 31, 14)
			{2,  RGB(29, 22, 4)}, //old RGB(31, 30, 0)
			TILESET_PAL_FADE_END
		},
	},
	{ //Magnolia Town Pal 12
		.tilesetPointer = 0x82D4D04, //Tileset 26
		.paletteNumToFade = 12,
		.paletteIndicesToFade =
		{
			{8,  RGB(29, 24, 5)}, //old RGB(31, 31, 14)
			{9,  RGB(29, 22, 4)}, //old RGB(31, 30, 0)
			{10, RGB(31, 27, 5)}, //old RGB(31, 31, 20)
			{12, RGB(29, 24, 5)}, //old RGB(31, 31, 14)
			{13, RGB(29, 22, 5)}, //old RGB(31, 30, 1)
			{14, RGB(29, 22, 4)}, //old RGB(31, 30, 0)
			TILESET_PAL_FADE_END
		},
	},
	{ //Redwood Village Windows
		.tilesetPointer = 0x82D4DF4, //Tileset 36
		.paletteNumToFade = 10,
		.paletteIndicesToFade =
		{
			{1,  RGB(31, 27, 5)}, //old RGB(31, 31, 20)
			{2,  RGB(29, 24, 5)}, //old RGB(31, 31, 14)
			{3,  RGB(29, 22, 4)}, //old RGB(31, 30, 0)
			TILESET_PAL_FADE_END
		},
	},
	{ //Redwood Village Pal 12
		.tilesetPointer = 0x82D4DF4, //Tileset 36
		.paletteNumToFade = 12,
		.paletteIndicesToFade =
		{
			{8,  RGB(29, 24, 5)}, //old RGB(31, 31, 14)
			{9,  RGB(29, 22, 4)}, //old RGB(31, 30, 0)
			{10, RGB(31, 27, 5)}, //old RGB(31, 31, 20)
			{12, RGB(29, 24, 5)}, //old RGB(31, 31, 14)
			{13, RGB(29, 22, 5)}, //old RGB(31, 30, 1)
			{14, RGB(29, 22, 4)}, //old RGB(31, 30, 0)
			TILESET_PAL_FADE_END
		},
	},
	{ //Pokemon League Fire
		.tilesetPointer = 0x82D4B84, //Tileset 10
		.paletteNumToFade = 9,
		.paletteIndicesToFade =
		{
			{12, RGB(30, 30, 25)},
			{13, RGB(30, 29, 21)},
			{14, RGB(28, 25, 11)},
			TILESET_PAL_FADE_END
		},
	},
	{ //Cube Corp Building
		.tilesetPointer = 0x82D4D4C, //Tileset 29
		.paletteNumToFade = 7,
		.paletteIndicesToFade =
		{
			{1, RGB(13, 29, 17)},
			{2, RGB(5, 25, 17)},
			{3, RGB(5, 17, 12)},
			{4, RGB(4, 17, 12)},
			TILESET_PAL_FADE_END
		},
	},
	{ //Cube Corp Lamp
		.tilesetPointer = 0x82D4D4C, //Tileset 29
		.paletteNumToFade = 8,
		.paletteIndicesToFade =
		{
			{6, RGB(29, 13, 20)},
			{7, RGB(27, 5, 14)},
			{14, RGB(16, 26, 30)},	//old RGB(31, 30, 1)
			{15, RGB(20, 29, 31)},	//old RGB(31, 31, 14)
			TILESET_PAL_FADE_END
		},
	},
	{ //Cube Corp Service Stand
		.tilesetPointer = 0x82D4D4C, //Tileset 29
		.paletteNumToFade = 9,
		.paletteIndicesToFade =
		{
			{12, RGB(4, 17, 12)},
			{13, RGB(6, 25, 17)},
			{14, RGB(13, 29, 17)},
			TILESET_PAL_FADE_END
		},
	},
	{ //Cube Corp Water
		.tilesetPointer = 0x82D4D4C, //Tileset 29
		.paletteNumToFade = 10,
		.paletteIndicesToFade =
		{
			{1, RGB(17, 23, 28)},
			{2, RGB(19, 24, 28)},
			{3, RGB(18, 25, 30)},
			{4, RGB(20, 25, 30)},
			{5, RGB(23, 26, 29)},
			{6, RGB(25, 27, 28)},
			{7, RGB(27, 29, 29)},
			TILESET_PAL_FADE_END
		},
	},
	{ //Cube Corp Doors
		.tilesetPointer = 0x82D4D4C, //Tileset 29
		.paletteNumToFade = 12,
		.paletteIndicesToFade =
		{
			{10,  RGB(31, 27, 5)},
			{12,  RGB(29, 24, 5)},
			{13,  RGB(29, 22, 5)},
			{14,  RGB(26, 20, 3)},
			TILESET_PAL_FADE_END
		},
	},
};

#endif
