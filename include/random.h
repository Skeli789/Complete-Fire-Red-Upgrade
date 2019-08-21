#ifndef GUARD_RANDOM_H
#define GUARD_RANDOM_H

#include "global.h"

extern u32 gRngValue;
extern u32 gRng2Value;

//Returns a 16-bit pseudorandom number
u16 __attribute__((long_call)) Random(void);
u16 __attribute__((long_call)) Random2(void);

//Returns a 32-bit pseudorandom number
#define Random32() ((u32) (Random() | (Random() << 16)))

//Sets the initial seed value of the pseudorandom number generator
void SeedRng(u16 seed);
void __attribute((long_call)) SeedRng2(u16 seed);

#endif // GUARD_RANDOM_H
