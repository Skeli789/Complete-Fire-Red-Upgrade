#pragma once

#include "../global.h"

/**
 * \file follow_me.c
 * \brief Contains functions for implementing a feature that allows the player to
 *        be followed by NPCs.
 */

//Exported Functions
u8 GetFollowerObjectId(void);
u8 GetFollowerLocalId(void);
const u8* GetFollowerScriptPointer(void);
void HideFollower(void);
void TryFreezeFollowerAnim(struct EventObject* npc);
void IsFollowerStoppingRockClimb(void);
void FollowMe_SetIndicatorToComeOutDoor(void);
void FollowMe_SetIndicatorToRecreateSurfBlob(void);
void FollowMe_TryRemoveFollowerOnWhiteOut(void);
void FollowMe(struct EventObject* npc, u8 state, bool8 ignoreScriptActive);
void FollowMe_Ledges(struct EventObject* npc, struct Sprite* obj, u16* ledgeFramesTbl);
bool8 FollowMe_IsCollisionExempt(struct EventObject* obstacle, struct EventObject* collider);
void FollowMe_FollowerToWater(void);
void FollowMe_BindToSurbBlobOnReloadScreen(void);
void PrepareFollowerDismountSurf(void);
void StairsMoveFollower(void);
void FollowMe_HandleBike(void);
void FollowMe_HandleSprite(void);
void FollowMe_WarpSetEnd(void);
bool8 IsFollowerAtCoords(s16 x, s16 y);
void CreateFollowerAvatar(void);
void TryAttachFollowerToPlayer(void);

//Functions Hooked In
void PlayerGoThroughDoor(u8 taskId);
void sp0D1_SetUpFollowerSprite(void);
void sp0D2_DestroyFollowerSprite(void);

//New Added
u16 GetMiniStepCount(u8 speed);
void RunMiniStep(struct Sprite *sprite, u8 speed, u8 currentFrame);
void CreateSparkleSprite(void);

//Exported Data Structures

struct Follower
{
	bool8 inProgress;
	u8 objId;
	u8 currentSprite;
	u8 delayedState;
	struct
	{
		u8 id;
		u8 number;
		u8 group;
	} map;
	bool8 warpEnd;
	struct Coords16 log;
	const u8* script;
	u16 flag;
	u16 gfxId;
	u8 flags;
	u8 locked;
	bool8 createSurfBlob;
	bool8 comeOutDoorStairs;
};

#define FOLLOWER_FLAG_HAS_RUNNING_FRAMES 0x1
#define FOLLOWER_FLAG_CAN_BIKE 0x2
#define FOLLOWER_FLAG_CAN_LEAVE_ROUTE 0x4
#define FOLLOWER_FLAG_CAN_SURF 0x8
#define FOLLOWER_FLAG_CAN_WATERFALL 0x10
#define FOLLOWER_FLAG_CAN_DIVE 0x20
#define FOLLOWER_FLAG_CAN_ROCK_CLIMB 0x40
#define FOLLOWER_FLAG_CLEAR_ON_WHITE_OUT 0x80

#define DEFAULT_FOLLOWER_LOCAL_ID 0xFE

extern struct Follower gFollowerState; //0x203B818

