#include "defines.h"
#include "helper_functions.h"
#include "../include/palette.h"
#include "dns.h"

#define DNSHelper ((u8*) 0x2021691)

typedef u8 IgnoredPalIndices_t[16];

void TransferPlttBuffer(void);
static void BlendFadedPalettes(u32 selectedPalettes, u8 coeff, u32 color);
static void BlendFadedPalette(u16 palOffset, u16 numEntries, u8 coeff, u32 blendColor, IgnoredPalIndices_t* ignoredIndices);

void TransferPlttBuffer(void)
{
    if (!gPaletteFade->bufferTransferDisabled)
    {
        void *src = gPlttBufferFaded;
        void *dest = (void *)PLTT;
        DmaCopy16(3, src, dest, PLTT_SIZE);	

		#ifdef TIME_ENABLED
		switch (gMapHeader.mapType) {
			case MAP_TYPE_0:			//No fading in these areas
			case MAP_TYPE_UNDERGROUND:
			case MAP_TYPE_INDOOR:
			case MAP_TYPE_SECRET_BASE:
				break;
			default:
				if ((DNSHelper[0] == 0
				/*#ifndef DNS_IN_BATTLE*/
				&& DNSHelper[1] < 3 //Not in battle
				/*#endif*/
				)
				|| DNSHelper[1] == 0x78 
				|| DNSHelper[1] == 0x88 
				|| DNSHelper[1] == 0x98) //The 0x78/0x88/0x98 is when a warp arrow appears
				{	
				/*
					#ifdef DNS_IN_BATTLE
						if (DNSHelper[1] > 2) //In Battles
							break;//BlendFadedPalettes(BATTLE_DNS_PAL_FADE, gDNSNightFadingByTime[Clock->hour][Clock->minute / 10].amount, gDNSNightFadingByTime[Clock->hour][Clock->minute / 10].colour);
						else
					#endif
				*/
					
					BlendFadedPalettes(OW_DNS_BG_PAL_FADE, gDNSNightFadingByTime[Clock->hour][Clock->minute / 10].amount, gDNSNightFadingByTime[Clock->hour][Clock->minute / 10].colour);
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
	u32 i;
    u16 paletteOffset;
	bool8 ignoredIndices[32][16] = {0};
	
	if ((Clock->hour >= TIME_NIGHT_START || Clock->hour < TIME_MORNING_START)/* && DNSHelper[1] < 3*/) //At night not in battle
	{
		for (i = 0; i < ARRAY_COUNT(gSpecificTilesetFades); ++i)
		{
			if ((u32) gMapHeader.mapData->primaryTileset == gSpecificTilesetFades[i].tilesetPointer
			||  (u32) gMapHeader.mapData->secondaryTileset == gSpecificTilesetFades[i].tilesetPointer)
			{
				u8 row = gSpecificTilesetFades[i].paletteNumToFade;
				u8 column = gSpecificTilesetFades[i].paletteIndexToFade;
				gPlttBufferUnfaded[row * 16 + column] = gSpecificTilesetFades[i].colourToFade;
				ignoredIndices[row][column] = TRUE;
			}
		}
	}
	
    for (paletteOffset = 0; selectedPalettes; paletteOffset += 16)
    {
        if (selectedPalettes & 1)
            BlendFadedPalette(paletteOffset, 16, coeff, color, ignoredIndices);
        selectedPalettes >>= 1;
    }
}

static void BlendFadedPalette(u16 palOffset, u16 numEntries, u8 coeff, u32 blendColor, IgnoredPalIndices_t* ignoredIndices)
{
	u16 i;
	
    for (i = 0; i < numEntries; i++)
    {
		if (ignoredIndices[palOffset / 16][i]) continue; //Don't fade this index.
	
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

#ifdef DNS_IN_BATTLE
void DNSBattleBGPalFade(void)
{ 
	u16 i, palOffset;
	u8 coeff = gDNSNightFadingByTime[Clock->hour][Clock->minute / 10].amount;
	u32 blendColor = gDNSNightFadingByTime[Clock->hour][Clock->minute / 10].colour;
	u8 selectedPalettes = BATTLE_DNS_PAL_FADE & 0x1C;
	
	for (palOffset = 0; selectedPalettes; palOffset += 16)
	{
		if (selectedPalettes & 1)
		{
			for (i = 0; i < 16; ++i)
			{
				u16 index = i + palOffset;
				struct PlttData *data1 = (struct PlttData *)&gPlttBufferUnfaded[index];
				s8 r = data1->r;
				s8 g = data1->g;
				s8 b = data1->b;
				struct PlttData *data2 = (struct PlttData *)&blendColor;
				u16 color  = ((r + (((data2->r - r) * coeff) >> 4)) << 0)
						   | ((g + (((data2->g - g) * coeff) >> 4)) << 5)
			  			   | ((b + (((data2->b - b) * coeff) >> 4)) << 10);
				
				gPlttBufferUnfaded[index] = color;
				gPlttBufferFaded[index] = color;
			}
		}
		selectedPalettes >>= 1;
	}
}
#endif
