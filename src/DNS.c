#include "defines.h"
#include "../include/new/helper_functions.h"
#include "../include/palette.h"
#include "../include/new/dns.h"

#define DNSHelper ((u8*) 0x2021691)

void TransferPlttBuffer(void);
static void BlendFadedPalettes(u32 selectedPalettes, u8 coeff, u32 color);
static void BlendFadedPalette(u16 palOffset, u16 numEntries, u8 coeff, u32 blendColor, bool8 ignoredIndices[32][16]);

static bool8 IsDate1BeforeDate2(u32 y1, u32 m1, u32 d1, u32 y2, u32 m2, u32 d2);
static bool8 IsLeapYear(u32 year);
static bool8 IsLastDayInMonth(u32 year, u8 month, u8 day);
static void IncreaseDateByOneDay(u32* year, u8* month, u8* day);
u32 GetMinuteDifference(u32 startYear, u8 startMonth, u8 startDay, u8 startHour, u8 startMin, u32 endYear, u8 endMonth, u8 endDay, u8 endHour, u8 endMin);
u32 GetHourDifference(u32 startYear, u8 startMonth, u8 startDay, u8 startHour, u32 endYear, u8 endMonth, u8 endDay, u8 endHour);
u32 GetDayDifference(u32 startYear, u8 startMonth, u8 startDay, u32 endYear, u8 endMonth, u8 endDay);
u32 GetMonthDifference(u32 startYear, u8 startMonth, u32 endYear, u8 endMonth);
u32 GetYearDifference(u32 startYear, u32 endYear);

const u8 gDaysInAMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

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

static void BlendFadedPalette(u16 palOffset, u16 numEntries, u8 coeff, u32 blendColor, bool8 ignoredIndices[32][16])
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

static bool8 IsDate1BeforeDate2(u32 y1, u32 m1, u32 d1, u32 y2, u32 m2, u32 d2)
{
	return y1 < y2 ? TRUE : (y1 == y2 ? (m1 < m2 ? TRUE : (m1 == m2 ? d1 < d2 : FALSE)) : FALSE);
}

static bool8 IsLeapYear(u32 year)
{
	return (year % 4 == 0) && ((year % 100 != 0) || (year % 400 == 0));
}

static bool8 IsLastDayInMonth(u32 year, u8 month, u8 day)
{
	if (month == 0 || month > 12) 
		return FALSE;

	else if (month == 2)
		return IsLeapYear(year) ? (day == 29) : (day == 28);

	else
		return gDaysInAMonth[month] == day;
}

static void IncreaseDateByOneDay(u32* year, u8* month, u8* day)
{
	if (IsLastDayInMonth(*year, *month, *day))
	{
		if (*month == 12) //IsLastMonthInYear
		{
			*month = 1;
			*day = 1;
			++*year;
		}
		else
		{
			*day = 1;
			++*month;
		}
	}
	else
		++*day;
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
	u32 days = 0;
	
	if (startYear == 0 && endYear >= 2018) //Player recently activated their clock
	{ //Save time and skip ahead
		days = 737060;
		startYear = 2018;
		startMonth = 1;
		startDay = 1;
	}
	
	//Increment until current date is reached
	while (IsDate1BeforeDate2(startYear, startMonth, startDay, endYear, endMonth, endDay) && days < 0xFFFFFFFF)
	{
		++days;
		IncreaseDateByOneDay(&startYear, &startMonth, &startDay); /* works on year, month and day variables */ 
	}

	return days;
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