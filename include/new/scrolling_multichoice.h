#pragma once

#include "../global.h"

/**
 * \file scrolling_multichoice.h
 * \brief Contains functions and data structures relating to custom scrollable list menus.
 */

//Exported Functions
extern u32 GetSizeOfMultiList(void);
extern const u8* const* GetScrollingMultiList(void);
extern void SetScrollingListSize(u8 taskId);

//Exported Constants
struct ScrollingMulti
{
	const u8* const* set;
	u8 count;
};

//Exported Data Structures
extern const struct ScrollingMulti gScrollingSets[];
