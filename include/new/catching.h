#pragma once

#include "../../src/defines.h"
#include "../../src/defines_battle.h"

#define BattleScript_TutorialThrow 		((u8*) 0x81D9A88)
#define BattleScript_ShakeBallThrow 	((u8*) 0x81D9A93)
#define BattleScript_TrainerBallBlock 	((u8*) 0x81D9AC1)
#define BattleScript_DodgedBall 		((u8*) 0x81D9AD1)

#define sBallCatchBonuses ((u8*) 0x8250892)

enum BallTypes 
{
	BALL_TYPE_MASTER_BALL = 1,
	BALL_TYPE_ULTRA_BALL,	//2
	BALL_TYPE_GREAT_BALL,	//3
	BALL_TYPE_POKE_BALL,	//4
	BALL_TYPE_SAFARI_BALL,	//5
	BALL_TYPE_NET_BALL,		//6
	BALL_TYPE_DIVE_BALL,	//7
	BALL_TYPE_NEST_BALL,	//8
	BALL_TYPE_REPEAT_BALL,	//9
	BALL_TYPE_TIMER_BALL,	//10
	BALL_TYPE_LUXURY_BALL,	//11
	BALL_TYPE_PREMIER_BALL,	//12
	BALL_TYPE_DUSK_BALL,	//13
	BALL_TYPE_HEAL_BALL,	//14
	BALL_TYPE_QUICK_BALL,	//15
	BALL_TYPE_CHERISH_BALL,	//16
	BALL_TYPE_PARK_BALL,	//17
	BALL_TYPE_FAST_BALL,	//18
	BALL_TYPE_LEVEL_BALL,	//19
	BALL_TYPE_LURE_BALL,	//20
	BALL_TYPE_HEAVY_BALL,	//21
	BALL_TYPE_LOVE_BALL,	//22
	BALL_TYPE_FRIEND_BALL,	//23
	BALL_TYPE_MOON_BALL,	//24
	BALL_TYPE_SPORT_BALL,	//25
	BALL_TYPE_BEAST_BALL,	//26
	BALL_TYPE_DREAM_BALL,	//27
};

#define NUM_BALLS BALL_TYPE_DREAM_BALL

struct BallIdItemIdRelation
{
	u16 itemId;
	u8 ballId;
};