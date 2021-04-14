#ifndef GUARD_INTERNATIONAL_STRING_UTIL_H
#define GUARD_INTERNATIONAL_STRING_UTIL_H

#include "global.h"
#include "string_util.h"

#define GetStringRightAlignXOffset(fontId, string, destWidth) ({ \
	s32 w = GetStringWidth(fontId, string, 0);                   \
	destWidth - w;                                               \
})


#define GetStringCenterAlignXOffset(fontId, string, totalWidth) ({        \
	int stringWidth = GetStringWidth(fontId, string, 0);                  \
	int diff = (totalWidth > stringWidth) ? totalWidth - stringWidth : 0; \
	diff / 2;                                                             \
})

#endif // GUARD_INTERNATIONAL_STRING_UTIL_H
