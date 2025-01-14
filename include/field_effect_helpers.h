#pragma once

#include "global.h"

u8 __attribute__((long_call)) CreateWarpArrowSprite(void);
u32 __attribute__((long_call)) FldEff_JumpTallGrass(void);
u32 __attribute__((long_call)) FldEff_JumpLongGrass(void);
void __attribute__((long_call)) WaitFieldEffectSpriteAnim(struct Sprite * sprite);
