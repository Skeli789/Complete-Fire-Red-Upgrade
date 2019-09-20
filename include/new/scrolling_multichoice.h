#pragma once

#include "../global.h"

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
