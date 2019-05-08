#pragma once

#include "../global.h"

/**
 * \file character_customization.h
 * \brief Contains functions relating to replacing the overworld sprite, the trainer
 *		  sprite, and the backsprite of the player character.
 */

//Exported Types
typedef const struct EventObjectGraphicsInfo* NPCPtr;

//Exported Functions
u16 GetBackspriteId(void);
void LoadTrainerBackPal(u16 trainerPicId, u8 paletteNum);

//Hooked In Functions
NPCPtr GetEventObjectGraphicsInfo(u16 graphicsId);
void PlayerHandleDrawTrainerPic(void);
void PlayerHandleTrainerSlide(void);
