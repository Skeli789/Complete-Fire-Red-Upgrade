#ifndef GUARD_POKEMON_STORAGE_SYSTEM_H
#define GUARD_POKEMON_STORAGE_SYSTEM_H

#include "global.h"
#include "pokemon.h"

#define TOTAL_BOXES_COUNT       14
#define IN_BOX_COUNT            30

u8* __attribute__((long_call)) GetBoxNamePtr(u8 boxNumber);
struct __attribute__((long_call)) BoxPokemon *GetBoxedMonPtr(u8 boxId, u8 monPosition);
void __attribute__((long_call)) SetBoxMonNickFromAnyBox(u8 boxId, u8 monPosition, u8 * newNick);
void __attribute__((long_call)) CompactPartySlots(void);
u32 __attribute__((long_call)) GetMonDataFromAnyBox(u8 boxId, u8 monPosition, u32 request);
void __attribute__((long_call)) SetMonDataFromAnyBox(u8 boxId, u8 monPosition, u8 request, void* addr);
void __attribute__((long_call)) sub_808BCB4(u8 boxId, u8 monPosition);

#endif // GUARD_POKEMON_STORAGE_SYSTEM_H
