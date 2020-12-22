#ifndef GUARD_INTERNATIONAL_STRING_UTIL_H
#define GUARD_INTERNATIONAL_STRING_UTIL_H

#include "global.h"
#include "string_util.h"

#define GetStringRightAlignXOffset(fontId, string, destWidth) ({ \
    s32 w = GetStringWidth(fontId, string, 0);                   \
    destWidth - w;                                               \
})

#endif // GUARD_INTERNATIONAL_STRING_UTIL_H
