//
// Created by scott on 9/6/2017.
//

#ifndef GUARD_FIELD_GROUND_EFFECT_H
#define GUARD_FIELD_GROUND_EFFECT_H

#include "global.h"

// Exported type declarations

// Exported RAM declarations

// Exported ROM declarations
bool8 IsZCoordMismatchAt(u8, s16, s16);
bool8 AreZCoordsCompatible(u8, u8);
void FieldObjectUpdateSubpriority(struct MapObject *, struct Sprite *);
void DoGroundEffects_OnSpawn(struct MapObject *, struct Sprite *);
void DoGroundEffects_OnBeginStep(struct MapObject *, struct Sprite *);
void DoGroundEffects_OnFinishStep(struct MapObject *, struct Sprite *);

#endif //GUARD_FIELD_GROUND_EFFECT_H
