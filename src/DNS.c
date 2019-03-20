#include "defines.h"
#include "helper_functions.h"
#include "../include/palette.h"

//Change These
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
//The above values are set to only change the battle background.

#define BG_SHI(num) (BBG_PAL_##num << num)

#define BATTLE_DNS_PAL_FADE BG_SHI(0) | BG_SHI(1) | BG_SHI(2) | BG_SHI(3) | BG_SHI(4) | BG_SHI(5) 	\
						 | BG_SHI(6) | BG_SHI(7) | BG_SHI(8) | BG_SHI(9) | BG_SHI(10) | BG_SHI(11) 	\
						 | BG_SHI(12) | BG_SHI(13) | BG_SHI(14) | BG_SHI(15)

void TransferPlttBuffer(void);
static void BlendFadedPalettes(u32 selectedPalettes, u8 coeff, u32 color);
static void BlendFadedPalette(u16 palOffset, u16 numEntries, u8 coeff, u32 blendColor);

struct DNSPalFade
{
	u16 colour;
	u8 amount;
};

const struct DNSPalFade gDNSNightFadingByTime[24][6];

void TransferPlttBuffer(void)
{
    if (!gPaletteFade->bufferTransferDisabled)
    {
        void *src = gPlttBufferFaded;
        void *dest = (void *)PLTT;
        DmaCopy16(3, src, dest, PLTT_SIZE);	

		#ifdef TIME_ENABLED
		u8* DNSHelper = ((u8*) 0x2021691);
		switch (gMapHeader.mapType) {
			case MAP_TYPE_0:			//No fading in these areas
			case MAP_TYPE_UNDERGROUND:
			case MAP_TYPE_INDOOR:
			case MAP_TYPE_SECRET_BASE:
				break;
			default:
				if ((!DNSHelper[0] 
				#ifndef DNS_IN_BATTLE 
				&& DNSHelper[1] < 3 
				#endif
				)
				|| DNSHelper[1] == 0x78 
				|| DNSHelper[1] == 0x88 
				|| DNSHelper[1] == 0x98) //The 0x78/0x88/0x98 is when a warp arrow appears
				{	
					#ifdef DNS_IN_BATTLE
						if (DNSHelper[1] > 2) //In Battles
							BlendFadedPalettes(BATTLE_DNS_PAL_FADE, gDNSNightFadingByTime[Clock->hour][Clock->minute / 10].amount, gDNSNightFadingByTime[Clock->hour][Clock->minute / 10].colour);
						else
					#endif
					
					BlendFadedPalettes(0xFFFF0FFF, gDNSNightFadingByTime[Clock->hour][Clock->minute / 10].amount, gDNSNightFadingByTime[Clock->hour][Clock->minute / 10].colour);
				}
		}
		#endif
		
		sPlttBufferTransferPending = 0;
		if (gPaletteFade->mode == HARDWARE_FADE && gPaletteFade->active)
			UpdateBlendRegisters();	
    }
}

static void BlendFadedPalettes(u32 selectedPalettes, u8 coeff, u32 color)
{
    u16 paletteOffset;

    for (paletteOffset = 0; selectedPalettes; paletteOffset += 16)
    {
        if (selectedPalettes & 1)
            BlendFadedPalette(paletteOffset, 16, coeff, color);
        selectedPalettes >>= 1;
    }
}

static void BlendFadedPalette(u16 palOffset, u16 numEntries, u8 coeff, u32 blendColor)
{
    u16 i;

    for (i = 0; i < numEntries; i++)
    {
        u16 index = i + palOffset;
        struct PlttData *data1 = (struct PlttData *)&gPlttBufferFaded[index];
        s8 r = data1->r;
        s8 g = data1->g;
        s8 b = data1->b;
        struct PlttData *data2 = (struct PlttData *)&blendColor;
        ((u16*) 0x5000000)[index] = ((r + (((data2->r - r) * coeff) >> 4)) << 0)
								  | ((g + (((data2->g - g) * coeff) >> 4)) << 5)
                                  | ((b + (((data2->b - b) * coeff) >> 4)) << 10);
    }
}

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