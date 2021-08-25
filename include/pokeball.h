#pragma once

#include "global.h"

#define POKEBALL_PLAYER_SENDOUT     0xFF
#define POKEBALL_OPPONENT_SENDOUT   0xFE

void __attribute__((long_call)) StartHealthboxSlideIn(u8 battlerId);
void __attribute__((long_call)) CreatePokeballSpriteToReleaseMon(u8 monSpriteId, u8 bank, u8 x, u8 y, u8 oamPriority, u8 subpriortiy, u8 g, u32 h);
u8 __attribute__((long_call)) CreateTradePokeballSprite(u8 a, u8 b, u8 x, u8 y, u8 oamPriority, u8 subPriority, u8 g, u32 h);
void __attribute__((long_call)) SpriteCB_PlayerMonSendOut_2(struct Sprite *sprite);
void __attribute__((long_call)) StartHealthboxSlideIn(u8 battler);

/*
u8 DoPokeballSendOutAnimation(s16 pan, u8 kindOfThrow);
u8 sub_807671C(u8 a, u8 b, u8 x, u8 y, u8 oamPriority, u8 subPriority, u8 g, u32 h);
void sub_8076918(u8 bank);
void DoHitAnimHealthboxEffect(u8 bank);
void LoadBallGfx(u8 ballId);
void FreeBallGfx(u8 ballId);
*/
