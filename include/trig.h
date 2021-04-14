#ifndef GUARD_TRIG_H
#define GUARD_TRIG_H

#include "global.h"

extern const s16 gSineTable[];

s16 __attribute__((long_call)) Sin(s16 index, s16 amplitude);
s16 __attribute__((long_call)) Cos(s16 index, s16 amplitude);
s16 __attribute__((long_call)) Sin2(u16 angle);
s16 __attribute__((long_call)) Cos2(u16 angle);

#endif // GUARD_TRIG_H
