#pragma once

#include "global.h"

bool8 __attribute__((long_call)) TestPlayerAvatarFlags(u8);
void __attribute__((long_call)) SetPlayerAvatarTransitionFlags(u16 a);
bool8 __attribute__((long_call)) PartyHasMonWithSurfOriginal(void);
bool8 __attribute__((long_call)) IsPlayerFacingSurfableFishableWater(void);
bool8 __attribute__((long_call)) IsPlayerSurfingNorth(void);
void __attribute__((long_call)) PlayerGetDestCoords(s16* x, s16* y);
u8 __attribute__((long_call)) GetPlayerMovementDirection(void);
u8 __attribute__((long_call)) PlayerGetZCoord(void);
void __attribute__((long_call)) SetPlayerAvatarStateMask(u8 a);
void __attribute__((long_call)) QuestLogCallUpdatePlayerSprite(u8 state);
void __attribute__((long_call)) ClearPlayerAvatarInfo(void);
void __attribute__((long_call)) GetXYCoordsOneStepInFrontOfPlayer(s16* xPtr, s16* yPtr);
bool32 __attribute__((long_call)) PlayerIsMovingOnRockStairs(u8 direction);
u8 __attribute__((long_call)) DoForcedMovement(u8 direction, void* movementAction);
void __attribute__((long_call)) PlayerGoSpeed1(u8);
void __attribute__((long_call)) PlayerGoSpeed2(u8);
void __attribute__((long_call)) PlayerRideWaterCurrent(u8);
void __attribute__((long_call)) PlayerGoSpeed4(u8);
void __attribute__((long_call)) PlayerSetAnimId(u8 a, u8 b);
void __attribute__((long_call)) PlayCollisionSoundIfNotFacingWarp(u8 a);
void __attribute__((long_call)) HandleEnforcedLookDirectionOnPlayerStopMoving(void);
void __attribute__((long_call)) StopPlayerAvatar(void);
void __attribute__((long_call)) SetPlayerAvatarFieldMove(void);
void __attribute__((long_call)) PlayerFaceDirection(u8 direction);

//In Engine
u16 GetPlayerAvatarGraphicsIdByStateIdAndGender(u8 state, u8 gender);
u16 GetPlayerAvatarGraphicsIdByStateId(u8 state);
