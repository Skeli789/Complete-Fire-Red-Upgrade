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
u16 GetEventObjectGraphicsId(struct EventObject* eventObj);
u16 GetBackspriteId(void);
void LoadTrainerBackPal(u16 trainerPicId, u8 paletteNum);
const u8* GetTrainerSpritePal(u16 trainerPicId);

//Hooked In Functions
NPCPtr GetEventObjectGraphicsInfo(u16 graphicsId);
u16 GetPlayerAvatarGraphicsIdByStateIdAndGender(u8 state, u8 gender);
u16 GetPlayerAvatarGraphicsIdByStateId(u8 state);
u8 PlayerGenderToFrontTrainerPicId(u8 gender, bool8 modify);
void PlayerHandleDrawTrainerPic(void);
void PlayerHandleTrainerSlide(void);
void TryUpdateTrainerPicPalTrainerCard(u16 trainerPicId, u16 palOffset);
