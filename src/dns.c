#include "defines.h"
#include "../include/fieldmap.h"
#include "../include/field_weather.h"
#include "../include/overworld.h"
#include "../include/palette.h"
#include "../include/shop.h"

#include "../include/new/dns.h"
#include "../include/new/dns_data.h"
#include "../include/new/dynamic_ow_pals.h"
#include "../include/new/util.h"
/*
dns.c
	handles functions and palette changes for the day, night, and seasons feature
*/

#define COEFF_FLAG_HARD_FADE 0x20
#define COEFF_FLAG_SKIP_TILESET_1 0x40
#define COEFF_FLAG_SKIP_TILESET_2 0x80
#define FIRST_COEFF_FLAG COEFF_FLAG_HARD_FADE

typedef bool8 IgnoredPalT[16];
#define gIgnoredDNSPalIndices ((IgnoredPalT*) 0x203B830)

//This file's functions:
#ifdef TIME_ENABLED
static void FadeDayNightPalettes(void);
static void BlendFadedPalettes(u32 selectedPalettes, u8 coeff, u32 color);
static void BlendFadedPalette(u16 palOffset, u16 numEntries, u8 coeff, u32 blendColor);
static u16 FadeColourForDNS(struct PlttData* blend, u8 coeff, s8 r, s8 g, s8 b);
static void FadeOverworldBackground(u32 selectedPalettes, u8 coeff, u32 color, bool8 palFadeActive);
#endif
static bool8 IsDate1BeforeDate2(u32 y1, u32 m1, u32 d1, u32 y2, u32 m2, u32 d2);
static bool8 IsLeapYear(u32 year);

void TransferPlttBuffer(void)
{
	if (!gPaletteFade->bufferTransferDisabled)
	{
		void *src = gPlttBufferFaded;
		void *dest = (void *)PLTT;
		DmaCopy16(3, src, dest, PLTT_SIZE);

		#ifdef TIME_ENABLED
		FadeDayNightPalettes();
		#endif

		sPlttBufferTransferPending = 0;
		if (gPaletteFade->mode == HARDWARE_FADE && gPaletteFade->active)
			UpdateBlendRegisters();
	}
}

#ifdef TIME_ENABLED
static void FadeDayNightPalettes(void)
{
	u32 palsToFade;
	bool8 inOverworld, fadePalettes;

	switch (gMapHeader.mapType) { //Save time by not calling the function
		case MAP_TYPE_TOWN: //Try to force a jump table to manually placing these values
		case MAP_TYPE_CITY:
		case MAP_TYPE_ROUTE:
		case MAP_TYPE_UNDERWATER:
		case MAP_TYPE_6:
		case MAP_TYPE_7:
		default:
			inOverworld = gMain.callback2 == CB2_Overworld
						|| gMain.callback2 == CB2_OverworldBasic
						|| gMain.callback2 == CB2_Credits; //Quicker than checking if Task_WeatherMain is active
			fadePalettes = inOverworld || gInShop;

			if (fadePalettes)
			{
				u8 coeff = gDNSNightFadingByTime[gClock.hour][gClock.minute / 10].amount;
				u16 colour = gDNSNightFadingByTime[gClock.hour][gClock.minute / 10].colour;
				bool8 palFadeActive = gPaletteFade->active || gWeatherPtr->palProcessingState == WEATHER_PAL_STATE_SCREEN_FADING_IN;

				if (inOverworld)
					palsToFade = OW_DNS_PAL_FADE;
				else
					palsToFade = OW_DNS_PAL_FADE & ~(OBG_SHI(11)); //Used by shop

				if (gLastRecordedFadeCoeff != coeff
				||  gLastRecordedFadeColour != colour) //Only fade the background if colour should change
				{
					bool8 hardFade = gLastRecordedFadeCoeff >= FIRST_COEFF_FLAG; //Reload tileset palettes if necessary

					if (!palFadeActive)
					{
						if (hardFade)
						{
							if (!(gLastRecordedFadeCoeff & COEFF_FLAG_SKIP_TILESET_1)) //Indicator to not reload tileset 1's palette (because just loaded)
								apply_map_tileset1_palette(gMapHeader.mapLayout);
							if (!(gLastRecordedFadeCoeff & COEFF_FLAG_SKIP_TILESET_2)) //Indicator to not reload tileset 2's palette (because just loaded)
								apply_map_tileset2_palette(gMapHeader.mapLayout);
						}
						else
							LoadMapTilesetPalettes(gMapHeader.mapLayout);
					}

					gWindowsLitUp = FALSE;
					FadeOverworldBackground(palsToFade, coeff, colour, palFadeActive); //Load/remove the palettes to fade once during the day and night
					gLastRecordedFadeCoeff = coeff;
					gLastRecordedFadeColour = colour;

					//The weather fading needs to be reloaded when the tileset palette is reloaded
					if (!palFadeActive)
					{
						for (u8 paletteIndex = 0; paletteIndex <= 12; paletteIndex++)
							ApplyWeatherGammaShiftToPal(paletteIndex);
					}

					if (hardFade) //Changed routes and part of the tileset was reloaded
						DmaCopy16(3, gPlttBufferFaded, (void *)PLTT, 0x20 * 13); //13 tileset palettes

					if (gInShop)
						BuyMenuDecompressBgGraphics(); //Otherwise the shop palette will get messed up
				}

				if (coeff == 0)
					break; //Don't bother fading a null fade

				palsToFade = (palsToFade & ~OW_DNS_BG_PAL_FADE) >> 16;
				BlendFadedPalettes(palsToFade, coeff, colour);
			}
			break;
		case MAP_TYPE_INDOOR: //No fading in these areas
		case MAP_TYPE_UNDERGROUND:
		case MAP_TYPE_0:
		case MAP_TYPE_SECRET_BASE:
			gLastRecordedFadeCoeff = 0;
			break;
	}
}

/*u8*/  #define gPlttBufferUnfaded ((u16*) 0x20371F8)
/*u8*/  #define gPlttBufferUnfaded2 ((u16*) 0x20373F8)

static void BlendFadedPalettes(u32 selectedPalettes, u8 coeff, u32 color)
{
	u16 paletteOffset;

	for (paletteOffset = 256; selectedPalettes; paletteOffset += 16) //Sprites
	{
		if (selectedPalettes & 1)
		{
			switch (GetPalTypeByPaletteOffset(paletteOffset)) {
				case PalTypeUnused:
					//if (paletteOffset < 256) //The background
					//	BlendFadedPalette(paletteOffset, 16, coeff, color);
					//break;
				case PalTypeOther: //Fade everything except Poke pics
					break;
				default:
					BlendFadedPalette(paletteOffset, 16, coeff, color);
			}
		}

		selectedPalettes >>= 1;
	}
}

static void BlendFadedPalette(u16 palOffset, u16 numEntries, u8 coeff, u32 blendColor)
{
	u16 i;
	u16 ignoreOffset = palOffset / 16;
	bool8 dontFadeWhite = gDontFadeWhite && !gMain.inBattle;

	for (i = 0; i < numEntries; ++i)
	{
		u16 index = i + palOffset;
		if (gPlttBufferFaded[index] == RGB_BLACK) continue; //Don't fade black

		if (gIgnoredDNSPalIndices[ignoreOffset][i]) continue; //Don't fade this index.

		//Fixes an issue with pre-battle mugshots
		if (dontFadeWhite && gPlttBufferFaded[index] == RGB_WHITE) continue;

		struct PlttData *data1 = (struct PlttData*) &gPlttBufferFaded[index];
		s8 r = data1->r;
		s8 g = data1->g;
		s8 b = data1->b;
		struct PlttData* data2 = (struct PlttData*) &blendColor;
		((u16*) PLTT)[index] = FadeColourForDNS(data2, coeff, r, g, b);
	}
}

static void BlendFadedUnfadedPalette(u16 palOffset, u16 numEntries, u8 coeff, u32 blendColor, bool8 palFadeActive)
{
	u16 i;
	u16 ignoreOffset = palOffset / 16;

	for (i = 0; i < numEntries; ++i)
	{
		u16 index = i + palOffset;
		if (gPlttBufferUnfaded[index] == RGB_BLACK) continue; //Don't fade black

		if (gIgnoredDNSPalIndices[ignoreOffset][i]) continue; //Don't fade this index.

		struct PlttData* data1 = (struct PlttData*) &gPlttBufferUnfaded[index];
		struct PlttData* data2 = (struct PlttData*) &blendColor;
		s8 r = data1->r;
		s8 g = data1->g;
		s8 b = data1->b;

		u16 newColour = FadeColourForDNS(data2, coeff, r, g, b);
		gPlttBufferUnfaded[index] = newColour;

		if (!palFadeActive)
			gPlttBufferFaded[index] = newColour;
	}
}

static u16 FadeColourForDNS(struct PlttData* blend, u8 coeff, s8 r, s8 g, s8 b)
{
	return ((r + (((blend->r - r) * coeff) >> 4)) << 0)
		 | ((g + (((blend->g - g) * coeff) >> 4)) << 5)
		 | ((b + (((blend->b - b) * coeff) >> 4)) << 10);

/*
	u8 coeffMax = 128;

	return (((r * (coeffMax - coeff) + (((r * blend->r) >> 5) * coeff)) >> 8) << 0)
		 | (((g * (coeffMax - coeff) + (((g * blend->g) >> 5) * coeff)) >> 8) << 5)
		 | (((b * (coeffMax - coeff) + (((b * blend->b) >> 5) * coeff)) >> 8) << 10);
*/
}

//This function gets called once when the game transitions to a new fade colour
static void FadeOverworldBackground(u32 selectedPalettes, u8 coeff, u32 color, bool8 palFadeActive)
{
	u32 i, j, row, column;

	if (IsNightTime())
	{
		if (!gWindowsLitUp)
		{
			for (i = 0; i < ARRAY_COUNT(gSpecificTilesetFades); ++i)
			{
				if ((u32) gMapHeader.mapLayout->primaryTileset == gSpecificTilesetFades[i].tilesetPointer
				||  (u32) gMapHeader.mapLayout->secondaryTileset == gSpecificTilesetFades[i].tilesetPointer)
				{
					row = gSpecificTilesetFades[i].paletteNumToFade;

					if (row == 11 && FuncIsActiveTask(Task_BuyMenu))
						continue; //Don't fade palette 11 in shop menu

					for (j = 0; gSpecificTilesetFades[i].paletteIndicesToFade[j].index != 0xFF; ++j)
					{
						column = gSpecificTilesetFades[i].paletteIndicesToFade[j].index;
						gPlttBufferUnfaded[row * 16 + column] = gSpecificTilesetFades[i].paletteIndicesToFade[j].colour;
						if (!palFadeActive)
							gPlttBufferFaded[row * 16 + column] = gSpecificTilesetFades[i].paletteIndicesToFade[j].colour;
						gIgnoredDNSPalIndices[row][column] = TRUE;
					}
				}
			}

			gWindowsLitUp = TRUE;
		}
	}
	else
	{
		if (!palFadeActive)
			LoadMapTilesetPalettes(gMapHeader.mapLayout);
		Memset(gIgnoredDNSPalIndices, 0, sizeof(bool8) * 16 * 32); //Don't ignore colours during day
		gWindowsLitUp = FALSE;
	}

	for (u16 paletteOffset = 0; paletteOffset < 13 * 16; paletteOffset += 16) //Only background colours
	{
		if (selectedPalettes & 1)
			BlendFadedUnfadedPalette(paletteOffset, 16, coeff, color, palFadeActive);

		selectedPalettes >>= 1;
	}
}
#endif

void apply_map_tileset_palette(struct Tileset const* tileset, u16 destOffset, u16 size)
{
	u16 black = RGB_BLACK;

	if (tileset)
	{
		if (tileset->isSecondary == FALSE)
		{
			LoadPalette(&black, destOffset, 2);
			LoadPalette(((u16*)tileset->palettes) + 1, destOffset + 1, size - 2);
			Fieldmap_ApplyGlobalTintToPaletteEntries(destOffset + 1, (size - 2) >> 1);
			gLastRecordedFadeCoeff |= COEFF_FLAG_SKIP_TILESET_1;
		}
		else if (tileset->isSecondary == TRUE)
		{
			LoadPalette(((u16*) tileset->palettes) + (NUM_PALS_IN_PRIMARY * 16), destOffset, size);
			Fieldmap_ApplyGlobalTintToPaletteEntries(destOffset, size >> 1);
			gLastRecordedFadeCoeff |= COEFF_FLAG_SKIP_TILESET_2;
		}
		else
		{
			LoadCompressedPalette((u32*) tileset->palettes, destOffset, size);
			Fieldmap_ApplyGlobalTintToPaletteEntries(destOffset, size >> 1);
		}

		Memset(gIgnoredDNSPalIndices, 0, sizeof(bool8) * 16 * 32);
		gLastRecordedFadeCoeff |= COEFF_FLAG_HARD_FADE; //So the colours can be reloaded on map re-entry
		gLastRecordedFadeColour = 0;
	}
}

void TryLoadTileset2OnCameraTransition(struct MapLayout* oldMapLayout)
{
	if (oldMapLayout->secondaryTileset != gMapHeader.mapLayout->secondaryTileset)
	{
		//Tileset has changed, so reload it, otherwise don't and save time
		copy_map_tileset2_to_vram_2(gMapHeader.mapLayout);
		apply_map_tileset2_palette(gMapHeader.mapLayout);

		for (u32 paletteIndex = 7; paletteIndex < 13; paletteIndex++)
			ApplyWeatherGammaShiftToPal(paletteIndex);
	}
}

#if (defined TIME_ENABLED && defined DNS_IN_BATTLE)
void DNSBattleBGPalFade(void)
{
	switch (GetCurrentMapType()) {
		case MAP_TYPE_0:			//No fading in these areas
		case MAP_TYPE_UNDERGROUND:
		case MAP_TYPE_INDOOR:
		case MAP_TYPE_SECRET_BASE:
			return;
	}

	u16 i, palOffset;
	u8 coeff = gDNSNightFadingByTime[gClock.hour][gClock.minute / 10].amount;
	u32 blendColor = gDNSNightFadingByTime[gClock.hour][gClock.minute / 10].colour;
	u8 selectedPalettes = BATTLE_DNS_PAL_FADE & 0x1C;

	for (palOffset = 0; selectedPalettes; palOffset += 16)
	{
		if (selectedPalettes & 1)
		{
			for (i = 0; i < 16; ++i)
			{
				u16 index = i + palOffset;
				struct PlttData* data1 = (struct PlttData*) &gPlttBufferUnfaded[index];
				s8 r = data1->r;
				s8 g = data1->g;
				s8 b = data1->b;
				struct PlttData *data2 = (struct PlttData *)&blendColor;
				u16 color = FadeColourForDNS(data2, coeff, r, g, b);

				gPlttBufferUnfaded[index] = color;
				gPlttBufferFaded[index] = color;
			}
		}
		selectedPalettes >>= 1;
	}
}
#endif

/* Causes mugshot issues
void BlendPalettesOptimized(u32 selectedPalettes, u32 coeff, u32 blendColor)
{
	s32 newR, newG, newB;
	u16 *palDataSrc, *palDataDst;

	if (!selectedPalettes)
		return;

	newR = (blendColor << 27) >> 27;
	newG = (blendColor << 22) >> 27;
	newB = (blendColor << 17) >> 27;

	palDataSrc = gPlttBufferUnfaded;
	palDataDst = gPlttBufferFaded;

	do
	{
		if (selectedPalettes & 1)
		{
			u16* palDataSrcEnd = palDataSrc + 16;
			while (palDataSrc != palDataSrcEnd)
			{
				u32 palDataSrcColor = *palDataSrc;

				s32 r = (palDataSrcColor << 27) >> 27;
				s32 g = (palDataSrcColor << 22) >> 27;
				s32 b = (palDataSrcColor << 17) >> 27;

				*palDataDst = ((r + (((newR - r) * coeff) >> 5)) << 0)
							| ((g + (((newG - g) * coeff) >> 5)) << 5)
							| ((b + (((newB - b) * coeff) >> 5)) << 10);

				palDataSrc++;
				palDataDst++;
			}
		}
		else
		{
			palDataSrc += 16;
			palDataDst += 16;
		}

		selectedPalettes >>= 1;
	} while (selectedPalettes);
}
*/

bool8 IsDayTime(void)
{
	return gClock.hour >= TIME_MORNING_START && gClock.hour < TIME_NIGHT_START;
}

bool8 IsOnlyDayTime(void)
{
	return gClock.hour >= TIME_DAY_START && gClock.hour < TIME_EVENING_START;
}

bool8 IsNightTime(void)
{
	return gClock.hour >= TIME_NIGHT_START || gClock.hour < TIME_MORNING_START;
}

bool8 IsMorning(void)
{
	return gClock.hour >= TIME_MORNING_START && gClock.hour < TIME_DAY_START;
}

bool8 IsEvening(void)
{
	return gClock.hour >= TIME_EVENING_START && gClock.hour < TIME_NIGHT_START;
}

static bool8 IsDate1BeforeDate2(u32 y1, u32 m1, u32 d1, u32 y2, u32 m2, u32 d2)
{
	return y1 < y2 ? TRUE : (y1 == y2 ? (m1 < m2 ? TRUE : (m1 == m2 ? d1 < d2 : FALSE)) : FALSE);
}

static bool8 IsLeapYear(u32 year)
{
	return (year % 4 == 0) && ((year % 100 != 0) || (year % 400 == 0));
}

u32 GetMinuteDifference(u32 startYear, u8 startMonth, u8 startDay, u8 startHour, u8 startMin, u32 endYear, u8 endMonth, u8 endDay, u8 endHour, u8 endMin)
{
	if (startYear > endYear
	|| (startYear == endYear && startMonth > endMonth)
	|| (startYear == endYear && startMonth == endMonth && startDay > endDay)
	|| (startYear == endYear && startMonth == endMonth && startDay == endDay && startHour > endHour)
	|| (startYear == endYear && startMonth == endMonth && startDay == endDay && startHour == endHour && startMin > endMin))
		return 0;

	u32 days = GetDayDifference(startYear, startMonth, startDay, endYear, endMonth, endDay);

	if (days >= 0xFFFFFFFF / 24 / 60)
		return 0xFFFFFFFF; //Max minutes
	else
	{
		u32 hours = GetHourDifference(startYear, startMonth, startDay, startHour, endYear, endMonth, endDay, endHour);

		if (startMin > endMin)
			return (MathMax(1, hours) - 1) * 60 + ((endMin + 60) - startMin);
		else
			return hours * 60 + (endMin - startMin);
	}
}

u32 GetHourDifference(u32 startYear, u8 startMonth, u8 startDay, u8 startHour, u32 endYear, u8 endMonth, u8 endDay, u8 endHour)
{
	if (startYear > endYear
	|| (startYear == endYear && startMonth > endMonth)
	|| (startYear == endYear && startMonth == endMonth && startDay > endDay)
	|| (startYear == endYear && startMonth == endMonth && startDay == endDay && startHour > endHour))
		return 0;

	u32 days = GetDayDifference(startYear, startMonth, startDay, endYear, endMonth, endDay);

	if (days >= 0xFFFFFFFF / 24)
		return 0xFFFFFFFF; //Max hours

	if (startHour > endHour)
		return (days - 1) * 24 + ((endHour + 24) - startHour);
	else //startHour <= endHour
		return days * 24 + (endHour - startHour);
}

u32 GetDayDifference(u32 startYear, u8 startMonth, u8 startDay, u32 endYear, u8 endMonth, u8 endDay)
{
	const u16 cumDays[] = {0,31,59,90,120,151,181,212,243,273,304,334}; //Cumulative Days by month
	const u16 leapcumDays[] = {0,31,60,91,121,152,182,213,244,274,305,335}; //Cumulative Days by month for leap year
	u32 totdays = 0;

	if (!IsDate1BeforeDate2(startYear, startMonth, startDay, endYear, endMonth, endDay))
		return 0;

	if (startYear == endYear)
	{
		if (IsLeapYear(startYear))
			return (leapcumDays[endMonth - 1] + endDay) - (leapcumDays[startMonth - 1] + startDay);
		else
			return (cumDays[endMonth-1] + endDay) - (cumDays[startMonth - 1] + startDay);
	}

	if (IsLeapYear(startYear))
		totdays = totdays + 366 - (leapcumDays[startMonth - 1] + startDay);
	else
		totdays = totdays + 365 - (cumDays[startMonth - 1] + startDay);

	u32 year = startYear + 1;
	while (year < endYear)
	{
		if (IsLeapYear(year))
			totdays = totdays + 366;
		else
			totdays = totdays + 365;

		year = year + 1;
	}

    if (IsLeapYear(endYear))
        totdays = totdays + (leapcumDays[endMonth - 1] + endDay);
    else
        totdays = totdays + (cumDays[endMonth - 1] + endDay);

    return totdays;
}

u32 GetMonthDifference(u32 startYear, u8 startMonth, u32 endYear, u8 endMonth)
{
	if (startYear > endYear
	|| (startYear == endYear && startMonth > endMonth))
		return 0;

	else if (endMonth >= startMonth)
		return GetYearDifference(startYear, endYear) * 12 + (endMonth - startMonth);

	else
		return (MathMax(1, GetYearDifference(startYear, endYear)) - 1) * 12 + ((endMonth + 12) - startMonth);
}

u32 GetYearDifference(u32 startYear, u32 endYear)
{
	if (startYear >= endYear)
		return 0;

	return endYear - startYear;
}

/* Test Cases
int main()
{
	u8 dayToIncrease = 31;
	u8 monthToIncrease = 12;
	u32 yearToIncrease = 2019;
	IncreaseDateByOneDay(&yearToIncrease, &monthToIncrease, &dayToIncrease);

	printf("Date 1 Before Date 2: %d\n", IsDate1BeforeThanDate2(2019, 12, 31, 2019, 05, 30));
	printf("Is %d a Leap Year: %d\n", 2020, IsLeapYear(2020));
	printf("Is last day in month: %d\n", IsLastDayInMonth(2020, 02, 29));
	printf("New Year: %d, New Month: %d, New Day: %d\n", yearToIncrease, monthToIncrease, dayToIncrease);

	printf("06-15-2000 @00:00 to 05-01-2018 @00:00\n");
	printf("Year difference: %d\n", GetYearDifference(2000, 2018)); //18
	printf("Month Difference: %d\n", GetMonthDifference(2000, 06, 2018, 05)); //15
	printf("Day Difference: %d\n", GetDayDifference(2000, 06, 15, 2018, 05, 01)); //6529
	printf("Hour Difference: %d\n", GetHourDifference(2000, 06, 15, 00, 2018, 05, 01, 00)); //156696
	printf("Minute Difference: %d\n", GetMinuteDifference(2000, 06, 15, 00, 01, 2018, 05, 01, 00, 00)); //9401759

	return 0;
}
*/
