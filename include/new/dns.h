#pragma once

#include "../global.h"

/**
 * \file dns.h
 * \brief Contains functions that help implement the Day/Night system. This includes
 *		  palette fading in the overworld (and optionally battle), as well as support
 *		  for daily events.
 */

//Exported Functions
void TransferPlttBuffer(void);
void DNSBattleBGPalFade(void);
bool8 IsDayTime();
bool8 IsOnlyDayTime();
bool8 IsNightTime();
bool8 IsMorning();
bool8 IsEvening();
u32 GetMinuteDifference(u32 startYear, u8 startMonth, u8 startDay, u8 startHour, u8 startMin, u32 endYear, u8 endMonth, u8 endDay, u8 endHour, u8 endMin);
u32 GetHourDifference(u32 startYear, u8 startMonth, u8 startDay, u8 startHour, u32 endYear, u8 endMonth, u8 endDay, u8 endHour);
u32 GetDayDifference(u32 startYear, u8 startMonth, u8 startDay, u32 endYear, u8 endMonth, u8 endDay);
u32 GetMonthDifference(u32 startYear, u8 startMonth, u32 endYear, u8 endMonth);
u32 GetYearDifference(u32 startYear, u32 endYear);
u32 GetDaysSinceTimeInValue(u32 var);

//Hooked In Functions
void apply_map_tileset_palette(struct Tileset const* tileset, u16 destOffset, u16 size);

//Exported Structs
struct DNSPalFade
{
	u16 colour;
	u8 amount;
};

struct IndexPalFade
{
	u8 index;
	u16 colour;
};

struct SpecificTilesetFade
{
	u32 tilesetPointer;
	u8 paletteNumToFade;
	struct IndexPalFade paletteIndicesToFade[16 + 1];
};

//Exported Constants
enum
{
	SUNDAY,
	MONDAY,
	TUESDAY,
	WEDNESDAY,
	THURSDAY,
	FRIDAY,
	SATURDAY,
	NUM_DAYS_IN_WEEK,
};
