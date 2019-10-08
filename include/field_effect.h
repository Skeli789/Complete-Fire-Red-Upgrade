#pragma once

#include "global.h"
#include "sprite.h"
#include "constants/field_effects.h"

extern const struct SpriteTemplate *const gFieldEffectObjectTemplatePointers[36];
extern const struct OamData gNewGameBirchOamAttributes;

#define gFieldEffectArguments ((struct FieldEffectArguments*) 0x20386E0) //extern u32 gFieldEffectArguments[8];

struct Coords32 {
    s32 x;
    s32 y;
};

/*extern struct gFieldEffectArguments {
    struct Coords32 effect_pos;
    u32 priority;
};
*/

u8 __attribute__((long_call)) FieldEffectStart(u8);
void __attribute__((long_call)) FieldEffectFreeGraphicsResources(struct Sprite *s);
void __attribute__((long_call)) FieldEffectStop(struct Sprite* s, u8 animation);
void __attribute__((long_call)) DeleteOBJ(struct Sprite *s);
u32 __attribute__((long_call)) FldEff_SurfBlob(void);
void __attribute__((long_call)) BindFieldEffectToSprite(u8 spriteId, u8 value);
void __attribute__((long_call)) FieldEffectActiveListRemove(u8 id);
void __attribute__((long_call)) UpdateSurfBlobFieldEffect(struct Sprite*);
void __attribute__((long_call)) BindObj(u8 spriteId, u8 value);
u8 __attribute__((long_call)) DoBobbingFieldEffect(u8 spriteId);
bool8 __attribute__((long_call)) CheckObjectGraphicsInFrontOfPlayer(u16 graphicsId);

/*
bool8 FieldEffectActiveListContains(u8 id);
void sub_80B69DC(void);
*/
