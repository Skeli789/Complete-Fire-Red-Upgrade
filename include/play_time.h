#ifndef GUARD_PLAY_TIME_H
#define GUARD_PLAY_TIME_H

#include "global.h"


#define PLAYTIME_RUNNING 1
extern u8 sPlayTimeCounterState;

void __attribute__((long_call)) PlayTimeCounter_Reset(void);
void __attribute__((long_call)) PlayTimeCounter_Start(void);
void __attribute__((long_call)) PlayTimeCounter_Stop(void);
void __attribute__((long_call)) PlayTimeCounter_Update(void);
void __attribute__((long_call)) PlayTimeCounter_SetToMax(void);

#endif // GUARD_PLAY_TIME_H
