#ifndef FOLLOWER_MON_H
#define FOLLOWER_MON_H

#include "global.h"

void __attribute__((long_call)) CreateSparkleSprite(void);
u16 __attribute__((long_call)) GetFollowerMonSprite(void);
void __attribute__((long_call)) CreateFollowerMonObject(void);
void __attribute__((long_call)) FollowerMonFacePlayer(void);
void __attribute__((long_call)) TurnFollowerMonToPlayer(void);
void __attribute__((long_call)) ShowFollowerMon(void);
void __attribute__((long_call)) UpdateFollowerMonSprite(void);

#endif