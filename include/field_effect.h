#pragma once

#include "global.h"
#include "sprite.h"
#include "constants/field_effects.h"

extern const struct SpriteTemplate *const gFieldEffectObjectTemplatePointers[36];
extern const struct OamData gNewGameBirchOamAttributes;

#define gFieldEffectArguments ((u32*) 0x20386E0) //extern u32 gFieldEffectArguments[8];

struct  __attribute__((packed)) FieldEffectScript
{
	u8 command;
	void (*func)(void);
	u8 endCommand;
};

struct  __attribute__((packed)) FieldEffectScript2
{
	u8 command;
	const struct SpritePalette* palette;
	void (*func)(void);
	u8 endCommand;
};

#define FLDEFF_CALLASM 3
#define FLDEFF_END 4
#define FLDEFF_LOAD_FADED_PAL_CALLASM 7

u8 __attribute__((long_call)) FieldEffectStart(u8);
void __attribute__((long_call)) FieldEffectFreeGraphicsResources(struct Sprite *s);
void __attribute__((long_call)) FieldEffectStop(struct Sprite* s, u8 animation);
u8 __attribute__((long_call)) CreateTrainerSprite(u8 trainerSpriteID, s16 x, s16 y, u8 subpriority, u8 *buffer);
bool8 __attribute__((long_call)) FieldEffectActiveListContains(u8 id);
void __attribute__((long_call)) DeleteOBJ(struct Sprite *s);
u32 __attribute__((long_call)) FldEff_SurfBlob(void);
void __attribute__((long_call)) FieldEffectActiveListRemove(u8 id);
void __attribute__((long_call)) UpdateSurfBlobFieldEffect(struct Sprite*);
void __attribute__((long_call)) UpdateJumpImpactEffect(struct Sprite* sprite);
void __attribute__((long_call)) SetSurfBobState(u8 spriteId, u8 value);
u8 __attribute__((long_call)) DoBobbingFieldEffect(u8 spriteId);
bool8 __attribute__((long_call)) CheckObjectGraphicsInFrontOfPlayer(u16 graphicsId);
void __attribute__((long_call)) FieldEffectScript_LoadFadedPalette(u8 **script);
void __attribute__((long_call)) ReturnToFieldFromFlyMapSelect(void);
u8 __attribute__((long_call)) MapTransitionIsExit(u8 lightLevel, u8 mapType);
u8 __attribute__((long_call)) MapTransitionIsEnter(u8 mapType1, u8 mapType2);
void __attribute__((long_call)) FieldCallback_SweetScent(void);
void __attribute__((long_call)) StartSweetScentFieldEffect(void);

/*
void sub_80B69DC(void);
*/
