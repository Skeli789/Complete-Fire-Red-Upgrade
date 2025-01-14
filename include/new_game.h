#ifndef GUARD_NEW_GAME_H
#define GUARD_NEW_GAME_H

#include "global.h"

extern bool8 gDifferentSaveFile;

void __attribute__((long_call)) CB2_NewGameOld(void);
void __attribute__((long_call)) SetTrainerId(u32 trainerId, u8 *dst);
void __attribute__((long_call)) CopyTrainerId(u8 *dst, u8 *src);
void __attribute__((long_call)) NewGameInitData(void);
void __attribute__((long_call)) ResetMenuAndMonGlobals(void);
void __attribute__((long_call)) Sav2_ClearSetDefault(void);

#endif // GUARD_NEW_GAME_H
