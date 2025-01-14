#pragma once

#include "../global.h"

/**
 * \file catching.h
 * \brief Contains functions relating to capturing Pokemon. Also contains
 *		  details about Poke Ball types.
 */

//Battle Script Commands
void atkEF_handleballthrow(void);
void atkF0_givecaughtmon(void);
void atkF1_trysetcaughtmondexflags(void);

//Exported Functions
struct Pokemon* LoadTargetPartyData(void);
u32 GetBaseBallCatchOdds(u8 ballType, u8 bankAtk, u8 bankDef);
bool8 IsCriticalCapture(void);
bool8 IsCriticalCaptureSuccess(void);
bool8 CantCatchBecauseFlag(void);
bool8 CantCatchPokemonRightNow(void);

//Functions Hooked In
u8 GiveMonToPlayer(pokemon_t* mon);
void CreateCapturedMonDexPic(u16 species);
u8 ItemIdToBallId(u16 ballItem);
void PlayerHandleSuccessBallThrowAnim(void);
void PlayerHandleBallThrowAnim(void);
void CreateThrowPokeBall(u8 taskId);
void DestroyThrowPokeBall(u8 taskId);
void StartPokeballThrowAnimation(u8 taskId);
void LoadBallGfx(u8 ballId);
item_t BallIdToItemId(u8 ballId);
u16 GetBattlerPokeballItemId(u8 bank);
bool8 DoubleWildPokeBallItemUseFix(u8 taskId);
bool8 TryGetPokeBallBattleScript(void);
const struct SpriteTemplate* OakIntroPokeballFix(void);

//Exported Structs
struct BallIdItemIdRelation
{
	u16 itemId;
	u8 ballId;
};

//Exported Constants
enum BallTypes
{
	BALL_TYPE_MASTER_BALL,	//0
	BALL_TYPE_ULTRA_BALL,	//1
	BALL_TYPE_GREAT_BALL,	//2
	BALL_TYPE_POKE_BALL,	//3
	BALL_TYPE_SAFARI_BALL,	//4
	BALL_TYPE_NET_BALL,		//5
	BALL_TYPE_DIVE_BALL,	//6
	BALL_TYPE_NEST_BALL,	//7
	BALL_TYPE_REPEAT_BALL,	//8
	BALL_TYPE_TIMER_BALL,	//9
	BALL_TYPE_LUXURY_BALL,	//10
	BALL_TYPE_PREMIER_BALL,	//11
	BALL_TYPE_DUSK_BALL,	//12
	BALL_TYPE_HEAL_BALL,	//13
	BALL_TYPE_QUICK_BALL,	//14
	BALL_TYPE_CHERISH_BALL,	//15
	BALL_TYPE_PARK_BALL,	//16
	BALL_TYPE_FAST_BALL,	//17
	BALL_TYPE_LEVEL_BALL,	//18
	BALL_TYPE_LURE_BALL,	//19
	BALL_TYPE_HEAVY_BALL,	//20
	BALL_TYPE_LOVE_BALL,	//21
	BALL_TYPE_FRIEND_BALL,	//22
	BALL_TYPE_MOON_BALL,	//23
	BALL_TYPE_SPORT_BALL,	//24
	BALL_TYPE_BEAST_BALL,	//25
	BALL_TYPE_DREAM_BALL,	//26
	BALL_TYPE_DYNAMAX_BALL, //27 - Don't give to player!

	BALL_TYPE_RANDOM = 0xFF,
};

//Make sure these values are always correct!
#define BALL_TYPE_MASTER_BALL 0
#define BALL_TYPE_POKE_BALL 3
#define BALL_TYPE_LUXURY_BALL 10
#define BALL_TYPE_DYNAMAX_BALL 27

#define LAST_BALL_INDEX BALL_TYPE_DREAM_BALL
#define NUM_BALLS (BALL_TYPE_DREAM_BALL + 1) //Make sure to update 0804B5AA in bytereplacement!
