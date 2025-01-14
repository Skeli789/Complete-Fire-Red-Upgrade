#pragma once

#include "../global.h"

/**
 * \file battle_anims.h
 * \brief Contains functions relating to various in-battle animations. Contains
 *		  animation tasks and sprite callbacks for attack animations, as well as
 *		  the code responsible for hiding the healthboxes during animations.
 */

//Exported & Hooked In Functions
void ScriptCmd_loadspritegfx(void);
void LoadShinyStarsSpriteTiles(void);
void TryStartShinyAnimation(u8 battler, unusedArg struct Pokemon* mon);
void UpdateOamPriorityInAllHealthboxes(u8 priority);
u8 CalcHealthBarPixelChange(unusedArg u8 bank);
u16 GetBattlerYDeltaFromSpriteId(u8 spriteId);
void HandleSpeciesGfxDataChange(u8 bankAtk, u8 bankDef, u8 transformType);
void DoMoveAnim(u16 move);
bool8 TryHandleLaunchBattleTableAnimation(u8 activeBattler, u8 bankAtk, u8 bankDef, u8 tableId, u16 argument);
void SpriteCB_CriticalCaptureThrownBallMovement(struct Sprite* sprite);
void SpriteCB_InitThrownBallBouncing(struct Sprite *sprite);
void SpriteCB_EnemyShadow(struct Sprite *shadowSprite);
