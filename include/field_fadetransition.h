#ifndef GUARD_FIELD_FADETRANSITION_H
#define GUARD_FIELD_FADETRANSITION_H

#include "global.h"

void __attribute__((long_call)) palette_bg_faded_fill_black(void);
bool32 __attribute__((long_call)) FieldFadeTransitionBackgroundEffectIsFinished(void);

#endif
