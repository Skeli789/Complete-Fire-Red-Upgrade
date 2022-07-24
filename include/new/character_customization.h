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
bool8 IsPaletteTagAffectedByCharacterCustomization(u16 tag);
u8 GetOutfitStyleByTrainerPic(u16 trainerPicId);
void ChangeTrainerPicPal(u16 paletteOffset, u8 outfitStyle);
void ChangeEventObjPal(u16 paletteOffset, u16 palTag);
void TryModifyMugshotTrainerPicPal(u16 trainerPicId, u8 index);

//Hooked In Functions
NPCPtr GetEventObjectGraphicsInfo(u16 graphicsId);
u16 GetPlayerAvatarGraphicsIdByStateIdAndGender(u8 state, u8 gender);
u16 GetPlayerAvatarGraphicsIdByStateId(u8 state);
u8 PlayerGenderToFrontTrainerPicId(u8 gender, bool8 modify);
void PlayerHandleDrawTrainerPic(void);
void PlayerHandleTrainerSlide(void);
void TryUpdateTrainerPicPalTrainerCard(u16 trainerPicId, u16 palOffset);
