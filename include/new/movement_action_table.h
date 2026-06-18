#include "../global.h"
#include "../constants/event_object_movement_constants.h"

#define sMovementActionFuncs_FaceDown ((void*)0x083a6864)
#define sMovementActionFuncs_FaceUp ((void*)0x083a686c)
#define sMovementActionFuncs_FaceLeft ((void*)0x083a6874)
#define sMovementActionFuncs_FaceRight ((void*)0x083a687c)
#define sDirectionAnimFuncsBySpeed ((void*)0x083a6884)
#define sMovementActionFuncs_WalkSlowestDown ((void*)0x083a6898)
#define sMovementActionFuncs_WalkSlowestUp ((void*)0x083a68a4)
#define sMovementActionFuncs_WalkSlowestLeft ((void*)0x083a68b0)
#define sMovementActionFuncs_WalkSlowestRight ((void*)0x083a68bc)
#define sMovementActionFuncs_WalkSlowerDown ((void*)0x083a68c8)
#define sMovementActionFuncs_WalkSlowerUp ((void*)0x083a68d4)
#define sMovementActionFuncs_WalkSlowerLeft ((void*)0x083a68e0)
#define sMovementActionFuncs_WalkSlowerRight ((void*)0x083a68ec)
#define sMovementActionFuncs_WalkSlowUp ((void*)0x083a68f8)
#define sMovementActionFuncs_WalkSlowDown ((void*)0x083a6904)
#define sMovementActionFuncs_WalkSlowLeft ((void*)0x083a6910)
#define sMovementActionFuncs_WalkSlowRight ((void*)0x083a691c)
//#define sMovementActionFuncs_WalkNormalDown ((void*)0x083a6928)
//#define sMovementActionFuncs_WalkNormalUp ((void*)0x083a6934)
//#define sMovementActionFuncs_WalkNormalLeft ((void*)0x083a6940)
//#define sMovementActionFuncs_WalkNormalRight ((void*)0x083a694c)
//#define sJumpInitDisplacements ((void*)0x083a6958)
//#define sJumpDisplacements ((void*)0x083a695e)
//#define sMovementActionFuncs_Jump2Down ((void*)0x083a6964)
//#define sMovementActionFuncs_Jump2Up ((void*)0x083a6970)
//#define sMovementActionFuncs_Jump2Left ((void*)0x083a697c)
//#define sMovementActionFuncs_Jump2Right ((void*)0x083a6988)
#define sMovementActionFuncs_Delay1 ((void*)0x083a6994)
#define sMovementActionFuncs_Delay2 ((void*)0x083a69a0)
#define sMovementActionFuncs_Delay4 ((void*)0x083a69ac)
#define sMovementActionFuncs_Delay8 ((void*)0x083a69b8)
#define sMovementActionFuncs_Delay16 ((void*)0x083a69c4)
#define sMovementActionFuncs_WalkFastDown ((void*)0x083a69d0)
#define sMovementActionFuncs_WalkFastUp ((void*)0x083a69dc)
#define sMovementActionFuncs_WalkFastLeft ((void*)0x083a69e8)
#define sMovementActionFuncs_WalkFastRight ((void*)0x083a69f4)
#define sMovementActionFuncs_GlideDown ((void*)0x083a6a00)
#define sMovementActionFuncs_GlideUp ((void*)0x083a6a0c)
#define sMovementActionFuncs_GlideLeft ((void*)0x083a6a18)
#define sMovementActionFuncs_GlideRight ((void*)0x083a6a24)
#define sMovementActionFuncs_FaceDownFast ((void*)0x083a6a30)
#define sMovementActionFuncs_FaceUpFast ((void*)0x083a6a38)
#define sMovementActionFuncs_FaceLeftFast ((void*)0x083a6a40)
#define sMovementActionFuncs_FaceRightFast ((void*)0x083a6a48)
#define sMovementActionFuncs_WalkInPlaceSlowDown ((void*)0x083a6a50)
#define sMovementActionFuncs_WalkInPlaceSlowUp ((void*)0x083a6a5c)
#define sMovementActionFuncs_WalkInPlaceSlowLeft ((void*)0x083a6a68)
#define sMovementActionFuncs_WalkInPlaceSlowRight ((void*)0x083a6a74)
#define sMovementActionFuncs_WalkInPlaceNormalDown ((void*)0x083a6a80)
#define sMovementActionFuncs_WalkInPlaceNormalUp ((void*)0x083a6a8c)
#define sMovementActionFuncs_WalkInPlaceNormalLeft ((void*)0x083a6a98)
#define sMovementActionFuncs_WalkInPlaceNormalRight ((void*)0x083a6aa4)
#define sMovementActionFuncs_WalkInPlaceFastDown ((void*)0x083a6ab0)
#define sMovementActionFuncs_WalkInPlaceFastUp ((void*)0x083a6abc)
#define sMovementActionFuncs_WalkInPlaceFastLeft ((void*)0x083a6ac8)
#define sMovementActionFuncs_WalkInPlaceFastRight ((void*)0x083a6ad4)
#define sMovementActionFuncs_WalkInPlaceFasterDown ((void*)0x083a6ae0)
#define sMovementActionFuncs_WalkInPlaceFasterUp ((void*)0x083a6aec)
#define sMovementActionFuncs_WalkInPlaceFasterLeft ((void*)0x083a6af8)
#define sMovementActionFuncs_WalkInPlaceFasterRight ((void*)0x083a6b04)
#define sMovementActionFuncs_RideWaterCurrentDown ((void*)0x083a6b10)
#define sMovementActionFuncs_RideWaterCurrentUp ((void*)0x083a6b1c)
#define sMovementActionFuncs_RideWaterCurrentLeft ((void*)0x083a6b28)
#define sMovementActionFuncs_RideWaterCurrentRight ((void*)0x083a6b34)
#define sMovementActionFuncs_WalkFasterDown ((void*)0x083a6b40)
#define sMovementActionFuncs_WalkFasterUp ((void*)0x083a6b4c)
#define sMovementActionFuncs_WalkFasterLeft ((void*)0x083a6b58)
#define sMovementActionFuncs_WalkFasterRight ((void*)0x083a6b64)
#define sMovementActionFuncs_SlideDown ((void*)0x083a6b70)
#define sMovementActionFuncs_SlideUp ((void*)0x083a6b7c)
#define sMovementActionFuncs_SlideLeft ((void*)0x083a6b88)
#define sMovementActionFuncs_SlideRight ((void*)0x083a6b94)
//#define sMovementActionFuncs_PlayerRunDown ((void*)0x083a6ba0)
//#define sMovementActionFuncs_PlayerRunUp ((void*)0x083a6bac)
//#define sMovementActionFuncs_PlayerRunLeft ((void*)0x083a6bb8)
//#define sMovementActionFuncs_PlayerRunRight ((void*)0x083a6bc4)
#define sMovementActionFuncs_PlayerRunDownSlow ((void*)0x083a6bd0)
#define sMovementActionFuncs_PlayerRunUpSlow ((void*)0x083a6bdc)
#define sMovementActionFuncs_PlayerRunLeftSlow ((void*)0x083a6be8)
#define sMovementActionFuncs_PlayerRunRightSlow ((void*)0x083a6bf4)
#define sMovementActionFuncs_StartAnimInDirection ((void*)0x083a6c00)
#define sMovementActionFuncs_JumpSpecialDown ((void*)0x083a6c0c)
#define sMovementActionFuncs_JumpSpecialUp ((void*)0x083a6c18)
#define sMovementActionFuncs_JumpSpecialLeft ((void*)0x083a6c24)
#define sMovementActionFuncs_JumpSpecialRight ((void*)0x083a6c30)
#define sMovementActionFuncs_JumpSpecialWithEffectDown ((void*)0x083a6c3c)
#define sMovementActionFuncs_JumpSpecialWithEffectUp ((void*)0x083a6c48)
#define sMovementActionFuncs_JumpSpecialWithEffectLeft ((void*)0x083a6c54)
#define sMovementActionFuncs_JumpSpecialWithEffectRight ((void*)0x083a6c60)
#define sMovementActionFuncs_FacePlayer ((void*)0x083a6c6c)
#define sMovementActionFuncs_FaceAwayPlayer ((void*)0x083a6c74)
#define sMovementActionFuncs_LockFacingDirection ((void*)0x083a6c7c)
#define sMovementActionFuncs_UnlockFacingDirection ((void*)0x083a6c84)
#define sMovementActionFuncs_JumpDown ((void*)0x083a6c8c)
#define sMovementActionFuncs_JumpUp ((void*)0x083a6c98)
#define sMovementActionFuncs_JumpLeft ((void*)0x083a6ca4)
#define sMovementActionFuncs_JumpRight ((void*)0x083a6cb0)
#define sMovementActionFuncs_JumpInPlaceDown ((void*)0x083a6cbc)
#define sMovementActionFuncs_JumpInPlaceUp ((void*)0x083a6cc8)
#define sMovementActionFuncs_JumpInPlaceLeft ((void*)0x083a6cd4)
#define sMovementActionFuncs_JumpInPlaceRight ((void*)0x083a6ce0)
#define sMovementActionFuncs_JumpInPlaceDownUp ((void*)0x083a6cec)
#define sMovementActionFuncs_JumpInPlaceUpDown ((void*)0x083a6cf8)
#define sMovementActionFuncs_JumpInPlaceLeftRight ((void*)0x083a6d04)
#define sMovementActionFuncs_JumpInPlaceRightLeft ((void*)0x083a6d10)
#define sMovementActionFuncs_FaceOriginalDirection ((void*)0x083a6d1c)
#define sMovementActionFuncs_NurseJoyBowDown ((void*)0x083a6d24)
#define sMovementActionFuncs_EnableJumpLandingGroundEffect ((void*)0x083a6d30)
#define sMovementActionFuncs_DisableJumpLandingGroundEffect ((void*)0x083a6d38)
#define sMovementActionFuncs_DisableAnimation ((void*)0x083a6d40)
#define sMovementActionFuncs_RestoreAnimation ((void*)0x083a6d48)
#define sMovementActionFuncs_SetInvisible ((void*)0x083a6d50)
#define sMovementActionFuncs_SetVisible ((void*)0x083a6d58)
#define sMovementActionFuncs_EmoteExclamationMark ((void*)0x083a6d60)
#define sMovementActionFuncs_EmoteQuestionMark ((void*)0x083a6d68)
#define sMovementActionFuncs_EmoteX ((void*)0x083a6d70)
#define sMovementActionFuncs_EmoteDoubleExclMark ((void*)0x083a6d78)
#define sMovementActionFuncs_EmoteSmile ((void*)0x083a6d80)
#define sMovementActionFuncs_RevealTrainer ((void*)0x083a6d88)
#define sMovementActionFuncs_RockSmashBreak ((void*)0x083a6d94)
#define sMovementActionFuncs_CutTree ((void*)0x083a6da4)
#define sMovementActionFuncs_SetFixedPriority ((void*)0x083a6db4)
#define sMovementActionFuncs_ClearFixedPriority ((void*)0x083a6dbc)
#define sMovementActionFuncs_InitAffineAnim ((void*)0x083a6dc4)
#define sMovementActionFuncs_ClearAffineAnim ((void*)0x083a6dcc)
#define sMovementActionFuncs_WalkDownStartAffine ((void*)0x083a6dd4)
#define sMovementActionFuncs_WalkDownAffine ((void*)0x083a6de0)
#define sMovementActionFuncs_AcroWheelieFaceDown ((void*)0x083a6dec)
#define sMovementActionFuncs_AcroWheelieFaceUp ((void*)0x083a6df4)
#define sMovementActionFuncs_AcroWheelieFaceLeft ((void*)0x083a6dfc)
#define sMovementActionFuncs_AcroWheelieFaceRight ((void*)0x083a6e04)
#define sMovementActionFuncs_AcroPopWheelieDown ((void*)0x083a6e0c)
#define sMovementActionFuncs_AcroPopWheelieUp ((void*)0x083a6e18)
#define sMovementActionFuncs_AcroPopWheelieLeft ((void*)0x083a6e24)
#define sMovementActionFuncs_AcroPopWheelieRight ((void*)0x083a6e30)
#define sMovementActionFuncs_AcroEndWheelieFaceDown ((void*)0x083a6e3c)
#define sMovementActionFuncs_AcroEndWheelieFaceUp ((void*)0x083a6e48)
#define sMovementActionFuncs_AcroEndWheelieFaceLeft ((void*)0x083a6e54)
#define sMovementActionFuncs_AcroEndWheelieFaceRight ((void*)0x083a6e60)
#define MovementActionFuncs_UnusedAcroActionDown ((void*)0x083a6e6c)
#define MovementActionFuncs_UnusedAcroActionUp ((void*)0x083a6e78)
#define MovementActionFuncs_UnusedAcroActionLeft ((void*)0x083a6e84)
#define MovementActionFuncs_UnusedAcroActionRight ((void*)0x083a6e90)
#define sMovementActionFuncs_AcroWheelieHopFaceDown ((void*)0x083a6e9c)
#define sMovementActionFuncs_AcroWheelieHopFaceUp ((void*)0x083a6ea8)
#define sMovementActionFuncs_AcroWheelieHopFaceLeft ((void*)0x083a6eb4)
#define sMovementActionFuncs_AcroWheelieHopFaceRight ((void*)0x083a6ec0)
#define sMovementActionFuncs_AcroWheelieHopDown ((void*)0x083a6ecc)
#define sMovementActionFuncs_AcroWheelieHopUp ((void*)0x083a6ed8)
#define sMovementActionFuncs_AcroWheelieHopLeft ((void*)0x083a6ee4)
#define sMovementActionFuncs_AcroWheelieHopRight ((void*)0x083a6ef0)
#define sMovementActionFuncs_AcroWheelieJumpDown ((void*)0x083a6efc)
#define sMovementActionFuncs_AcroWheelieJumpUp ((void*)0x083a6f08)
#define sMovementActionFuncs_AcroWheelieJumpLeft ((void*)0x083a6f14)
#define sMovementActionFuncs_AcroWheelieJumpRight ((void*)0x083a6f20)
#define sMovementActionFuncs_AcroWheelieInPlaceDown ((void*)0x083a6f2c)
#define sMovementActionFuncs_AcroWheelieInPlaceUp ((void*)0x083a6f38)
#define sMovementActionFuncs_AcroWheelieInPlaceLeft ((void*)0x083a6f44)
#define sMovementActionFuncs_AcroWheelieInPlaceRight ((void*)0x083a6f50)
#define sMovementActionFuncs_AcroPopWheelieMoveDown ((void*)0x083a6f5c)
#define sMovementActionFuncs_AcroPopWheelieMoveUp ((void*)0x083a6f68)
#define sMovementActionFuncs_AcroPopWheelieMoveLeft ((void*)0x083a6f74)
#define sMovementActionFuncs_AcroPopWheelieMoveRight ((void*)0x083a6f80)
#define sMovementActionFuncs_AcroWheelieMoveDown ((void*)0x083a6f8c)
#define sMovementActionFuncs_AcroWheelieMoveUp ((void*)0x083a6f98)
#define sMovementActionFuncs_AcroWheelieMoveLeft ((void*)0x083a6fa4)
#define sMovementActionFuncs_AcroWheelieMoveRight ((void*)0x083a6fb0)
#define sMovementActionFuncs_SpinDown ((void*)0x083a6fbc)
#define sMovementActionFuncs_SpinUp ((void*)0x083a6fc8)
#define sMovementActionFuncs_SpinLeft ((void*)0x083a6fd4)
#define sMovementActionFuncs_SpinRight ((void*)0x083a6fe0)
#define sMovementActionFuncs_RaiseHandAndStop ((void*)0x083a6fec)
#define sMovementActionFuncs_RaiseHandAndJump ((void*)0x083a6ff4)
#define sMovementActionFuncs_RaiseHandAndSwim ((void*)0x083a6ffc)
#define sMovementActionFuncs_ShakeHeadOrWalkInPlace ((void*)0x083a7004)
#define sMovementActionFuncs_FlyUp ((void*)0x083a700c)
#define sMovementActionFuncs_FlyDown ((void*)0x083a7018)

#define ANIM_STD_FACE_SOUTH       0
#define ANIM_STD_FACE_NORTH       1
#define ANIM_STD_FACE_WEST        2
#define ANIM_STD_FACE_EAST        3
#define ANIM_STD_GO_SOUTH         4
#define ANIM_STD_GO_NORTH         5
#define ANIM_STD_GO_WEST          6
#define ANIM_STD_GO_EAST          7
#define ANIM_STD_GO_FAST_SOUTH    8
#define ANIM_STD_GO_FAST_NORTH    9
#define ANIM_STD_GO_FAST_WEST     10
#define ANIM_STD_GO_FAST_EAST     11
#define ANIM_STD_GO_FASTER_SOUTH  12
#define ANIM_STD_GO_FASTER_NORTH  13
#define ANIM_STD_GO_FASTER_WEST   14
#define ANIM_STD_GO_FASTER_EAST   15
#define ANIM_STD_GO_FASTEST_SOUTH 16
#define ANIM_STD_GO_FASTEST_NORTH 17
#define ANIM_STD_GO_FASTEST_WEST  18
#define ANIM_STD_GO_FASTEST_EAST  19
#define ANIM_STD_COUNT            20

#define ANIM_RUN_SOUTH                   (ANIM_STD_COUNT + 0)
#define ANIM_RUN_NORTH                   (ANIM_STD_COUNT + 1)
#define ANIM_RUN_WEST                    (ANIM_STD_COUNT + 2)
#define ANIM_RUN_EAST                    (ANIM_STD_COUNT + 3)
#define ANIM_SPIN_SOUTH                  (ANIM_STD_COUNT + 4)
#define ANIM_SPIN_NORTH                  (ANIM_STD_COUNT + 5)
#define ANIM_SPIN_WEST                   (ANIM_STD_COUNT + 6)
#define ANIM_SPIN_EAST                   (ANIM_STD_COUNT + 7)
#define ANIM_SHAKE_HEAD_OR_WALK_IN_PLACE (ANIM_STD_COUNT + 8)

#define ANIM_BUNNY_HOP_BACK_WHEEL_SOUTH         (ANIM_STD_COUNT + 0)
#define ANIM_BUNNY_HOP_BACK_WHEEL_NORTH         (ANIM_STD_COUNT + 1)
#define ANIM_BUNNY_HOP_BACK_WHEEL_WEST          (ANIM_STD_COUNT + 2)
#define ANIM_BUNNY_HOP_BACK_WHEEL_EAST          (ANIM_STD_COUNT + 3)
#define ANIM_BUNNY_HOP_FRONT_WHEEL_SOUTH        (ANIM_STD_COUNT + 4)
#define ANIM_BUNNY_HOP_FRONT_WHEEL_NORTH        (ANIM_STD_COUNT + 5)
#define ANIM_BUNNY_HOP_FRONT_WHEEL_WEST         (ANIM_STD_COUNT + 6)
#define ANIM_BUNNY_HOP_FRONT_WHEEL_EAST         (ANIM_STD_COUNT + 7)
#define ANIM_STANDING_WHEELIE_BACK_WHEEL_SOUTH  (ANIM_STD_COUNT + 8)
#define ANIM_STANDING_WHEELIE_BACK_WHEEL_NORTH  (ANIM_STD_COUNT + 9)
#define ANIM_STANDING_WHEELIE_BACK_WHEEL_WEST   (ANIM_STD_COUNT + 10)
#define ANIM_STANDING_WHEELIE_BACK_WHEEL_EAST   (ANIM_STD_COUNT + 11)
#define ANIM_STANDING_WHEELIE_FRONT_WHEEL_SOUTH (ANIM_STD_COUNT + 12)
#define ANIM_STANDING_WHEELIE_FRONT_WHEEL_NORTH (ANIM_STD_COUNT + 13)
#define ANIM_STANDING_WHEELIE_FRONT_WHEEL_WEST  (ANIM_STD_COUNT + 14)
#define ANIM_STANDING_WHEELIE_FRONT_WHEEL_EAST  (ANIM_STD_COUNT + 15)
#define ANIM_MOVING_WHEELIE_SOUTH               (ANIM_STD_COUNT + 16)
#define ANIM_MOVING_WHEELIE_NORTH               (ANIM_STD_COUNT + 17)
#define ANIM_MOVING_WHEELIE_WEST                (ANIM_STD_COUNT + 18)
#define ANIM_MOVING_WHEELIE_EAST                (ANIM_STD_COUNT + 19)

#define ANIM_GET_ON_OFF_POKEMON_SOUTH (ANIM_STD_COUNT + 0)
#define ANIM_GET_ON_OFF_POKEMON_NORTH (ANIM_STD_COUNT + 1)
#define ANIM_GET_ON_OFF_POKEMON_WEST  (ANIM_STD_COUNT + 2)
#define ANIM_GET_ON_OFF_POKEMON_EAST  (ANIM_STD_COUNT + 3)

#define ANIM_NURSE_BOW (ANIM_STD_COUNT + 0)

//#define ANIM_RAISE_HAND (ANIM_STD_COUNT + 0)

#define ANIM_FIELD_MOVE 0

#define ANIM_VS_SEEKER  0

#define ANIM_STAY_STILL      0
#define ANIM_REMOVE_OBSTACLE 1

#define ANIM_TAKE_OUT_ROD_SOUTH   0
#define ANIM_TAKE_OUT_ROD_NORTH   1
#define ANIM_TAKE_OUT_ROD_WEST    2
#define ANIM_TAKE_OUT_ROD_EAST    3
#define ANIM_PUT_AWAY_ROD_SOUTH   4
#define ANIM_PUT_AWAY_ROD_NORTH   5
#define ANIM_PUT_AWAY_ROD_WEST    6
#define ANIM_PUT_AWAY_ROD_EAST    7
#define ANIM_HOOKED_POKEMON_SOUTH 8
#define ANIM_HOOKED_POKEMON_NORTH 9
#define ANIM_HOOKED_POKEMON_WEST  10
#define ANIM_HOOKED_POKEMON_EAST  11

extern const u8 gInitialMovementTypeFacingDirections[MOVEMENT_TYPES_COUNT];

const bool8 gRangedMovementTypes[MOVEMENT_TYPES_COUNT] = {
    [MOVEMENT_TYPE_WANDER_AROUND] = TRUE,
    [MOVEMENT_TYPE_WANDER_UP_AND_DOWN] = TRUE,
    [MOVEMENT_TYPE_WANDER_DOWN_AND_UP] = TRUE,
    [MOVEMENT_TYPE_WANDER_LEFT_AND_RIGHT] = TRUE,
    [MOVEMENT_TYPE_WANDER_RIGHT_AND_LEFT] = TRUE,
    [MOVEMENT_TYPE_WALK_UP_AND_DOWN] = TRUE,
    [MOVEMENT_TYPE_WALK_DOWN_AND_UP] = TRUE,
    [MOVEMENT_TYPE_WALK_LEFT_AND_RIGHT] = TRUE,
    [MOVEMENT_TYPE_WALK_RIGHT_AND_LEFT] = TRUE,
    [MOVEMENT_TYPE_WALK_SEQUENCE_UP_RIGHT_LEFT_DOWN] = TRUE,
    [MOVEMENT_TYPE_WALK_SEQUENCE_RIGHT_LEFT_DOWN_UP] = TRUE,
    [MOVEMENT_TYPE_WALK_SEQUENCE_DOWN_UP_RIGHT_LEFT] = TRUE,
    [MOVEMENT_TYPE_WALK_SEQUENCE_LEFT_DOWN_UP_RIGHT] = TRUE,
    [MOVEMENT_TYPE_WALK_SEQUENCE_UP_LEFT_RIGHT_DOWN] = TRUE,
    [MOVEMENT_TYPE_WALK_SEQUENCE_LEFT_RIGHT_DOWN_UP] = TRUE,
    [MOVEMENT_TYPE_WALK_SEQUENCE_DOWN_UP_LEFT_RIGHT] = TRUE,
    [MOVEMENT_TYPE_WALK_SEQUENCE_RIGHT_DOWN_UP_LEFT] = TRUE,
    [MOVEMENT_TYPE_WALK_SEQUENCE_LEFT_UP_DOWN_RIGHT] = TRUE,
    [MOVEMENT_TYPE_WALK_SEQUENCE_UP_DOWN_RIGHT_LEFT] = TRUE,
    [MOVEMENT_TYPE_WALK_SEQUENCE_RIGHT_LEFT_UP_DOWN] = TRUE,
    [MOVEMENT_TYPE_WALK_SEQUENCE_DOWN_RIGHT_LEFT_UP] = TRUE,
    [MOVEMENT_TYPE_WALK_SEQUENCE_RIGHT_UP_DOWN_LEFT] = TRUE,
    [MOVEMENT_TYPE_WALK_SEQUENCE_UP_DOWN_LEFT_RIGHT] = TRUE,
    [MOVEMENT_TYPE_WALK_SEQUENCE_LEFT_RIGHT_UP_DOWN] = TRUE,
    [MOVEMENT_TYPE_WALK_SEQUENCE_DOWN_LEFT_RIGHT_UP] = TRUE,
    [MOVEMENT_TYPE_WALK_SEQUENCE_UP_LEFT_DOWN_RIGHT] = TRUE,
    [MOVEMENT_TYPE_WALK_SEQUENCE_DOWN_RIGHT_UP_LEFT] = TRUE,
    [MOVEMENT_TYPE_WALK_SEQUENCE_LEFT_DOWN_RIGHT_UP] = TRUE,
    [MOVEMENT_TYPE_WALK_SEQUENCE_RIGHT_UP_LEFT_DOWN] = TRUE,
    [MOVEMENT_TYPE_WALK_SEQUENCE_UP_RIGHT_DOWN_LEFT] = TRUE,
    [MOVEMENT_TYPE_WALK_SEQUENCE_DOWN_LEFT_UP_RIGHT] = TRUE,
    [MOVEMENT_TYPE_WALK_SEQUENCE_LEFT_UP_RIGHT_DOWN] = TRUE,
    [MOVEMENT_TYPE_WALK_SEQUENCE_RIGHT_DOWN_LEFT_UP] = TRUE,
    [MOVEMENT_TYPE_COPY_PLAYER] = TRUE,
    [MOVEMENT_TYPE_COPY_PLAYER_OPPOSITE] = TRUE,
    [MOVEMENT_TYPE_COPY_PLAYER_COUNTERCLOCKWISE] = TRUE,
    [MOVEMENT_TYPE_COPY_PLAYER_CLOCKWISE] = TRUE,
    [MOVEMENT_TYPE_COPY_PLAYER_IN_GRASS] = TRUE,
    [MOVEMENT_TYPE_COPY_PLAYER_OPPOSITE_IN_GRASS] = TRUE,
    [MOVEMENT_TYPE_COPY_PLAYER_COUNTERCLOCKWISE_IN_GRASS] = TRUE,
    [MOVEMENT_TYPE_COPY_PLAYER_CLOCKWISE_IN_GRASS] = TRUE,
    [MOVEMENT_TYPE_WALK_SLOWLY_IN_PLACE_RIGHT] = TRUE,
    [MOVEMENT_TYPE_FOLLOWING_POKEMON] = FALSE,
};

enum {
    JUMP_DISTANCE_IN_PLACE,
    JUMP_DISTANCE_NORMAL,
    JUMP_DISTANCE_FAR,
};

static const s16 sJumpDisplacements[] = {
    [JUMP_DISTANCE_IN_PLACE] = 0,
    [JUMP_DISTANCE_NORMAL] = 0,
    [JUMP_DISTANCE_FAR] = 1,
};

enum {
    MOVE_SPEED_NORMAL, // walking
    MOVE_SPEED_FAST_1, // running / surfing / sliding (ice tile)
    MOVE_SPEED_FAST_2, // water current / bicycle
    MOVE_SPEED_FASTER, // going down cycling road on bicycle
    MOVE_SPEED_FASTEST,
};

enum {
    JUMP_TYPE_HIGH,
    JUMP_TYPE_LOW,
    JUMP_TYPE_NORMAL,
};

typedef void (*SpriteStepFunc)(struct Sprite *sprite, u8 direction);

#define sNpcStepFuncTables ((const SpriteStepFunc *const *)0x083a719c)
#define sStepTimes ((const s16 *)0x083a71b0)

static const s16 sJumpInitDisplacements[] = {
    [JUMP_DISTANCE_IN_PLACE] = 0,
    [JUMP_DISTANCE_NORMAL] = 1,
    [JUMP_DISTANCE_FAR] = 1,
};

const union AnimCmd sAnim_Rival[] =
{
    ANIMCMD_FRAME( 0, 10),
    ANIMCMD_FRAME(48, 10),
    ANIMCMD_FRAME( 0, 10),
    ANIMCMD_FRAME(64, 10),
    ANIMCMD_JUMP(0)
};

static const u8 sFaceDirectionAnimNums[] = {
    [DIR_NONE]      = ANIM_STD_FACE_SOUTH,
    [DIR_SOUTH]     = ANIM_STD_FACE_SOUTH,
    [DIR_NORTH]     = ANIM_STD_FACE_NORTH,
    [DIR_WEST]      = ANIM_STD_FACE_WEST,
    [DIR_EAST]      = ANIM_STD_FACE_EAST,
    [DIR_SOUTHWEST] = ANIM_STD_FACE_WEST,
    [DIR_SOUTHEAST] = ANIM_STD_FACE_EAST,
    [DIR_NORTHWEST] = ANIM_STD_FACE_WEST,
    [DIR_NORTHEAST] = ANIM_STD_FACE_EAST,
};

static const u8 sMoveDirectionAnimNums[] = {
    [DIR_NONE]      = ANIM_STD_GO_SOUTH,
    [DIR_SOUTH]     = ANIM_STD_GO_SOUTH,
    [DIR_NORTH]     = ANIM_STD_GO_NORTH,
    [DIR_WEST]      = ANIM_STD_GO_WEST,
    [DIR_EAST]      = ANIM_STD_GO_EAST,
    [DIR_SOUTHWEST] = ANIM_STD_GO_WEST,
    [DIR_SOUTHEAST] = ANIM_STD_GO_EAST,
    [DIR_NORTHWEST] = ANIM_STD_GO_WEST,
    [DIR_NORTHEAST] = ANIM_STD_GO_EAST,
};

static const u8 sMoveDirectionFastAnimNums[] = {
    [DIR_NONE]      = ANIM_STD_GO_FAST_SOUTH,
    [DIR_SOUTH]     = ANIM_STD_GO_FAST_SOUTH,
    [DIR_NORTH]     = ANIM_STD_GO_FAST_NORTH,
    [DIR_WEST]      = ANIM_STD_GO_FAST_WEST,
    [DIR_EAST]      = ANIM_STD_GO_FAST_EAST,
    [DIR_SOUTHWEST] = ANIM_STD_GO_FAST_WEST,
    [DIR_SOUTHEAST] = ANIM_STD_GO_FAST_EAST,
    [DIR_NORTHWEST] = ANIM_STD_GO_FAST_WEST,
    [DIR_NORTHEAST] = ANIM_STD_GO_FAST_EAST,
};

static const u8 sMoveDirectionFasterAnimNums[] = {
    [DIR_NONE]      = ANIM_STD_GO_FASTER_SOUTH,
    [DIR_SOUTH]     = ANIM_STD_GO_FASTER_SOUTH,
    [DIR_NORTH]     = ANIM_STD_GO_FASTER_NORTH,
    [DIR_WEST]      = ANIM_STD_GO_FASTER_WEST,
    [DIR_EAST]      = ANIM_STD_GO_FASTER_EAST,
    [DIR_SOUTHWEST] = ANIM_STD_GO_FASTER_WEST,
    [DIR_SOUTHEAST] = ANIM_STD_GO_FASTER_EAST,
    [DIR_NORTHWEST] = ANIM_STD_GO_FASTER_WEST,
    [DIR_NORTHEAST] = ANIM_STD_GO_FASTER_EAST,
};

static const u8 sMoveDirectionFastestAnimNums[] = {
    [DIR_NONE]      = ANIM_STD_GO_FASTEST_SOUTH,
    [DIR_SOUTH]     = ANIM_STD_GO_FASTEST_SOUTH,
    [DIR_NORTH]     = ANIM_STD_GO_FASTEST_NORTH,
    [DIR_WEST]      = ANIM_STD_GO_FASTEST_WEST,
    [DIR_EAST]      = ANIM_STD_GO_FASTEST_EAST,
    [DIR_SOUTHWEST] = ANIM_STD_GO_FASTEST_WEST,
    [DIR_SOUTHEAST] = ANIM_STD_GO_FASTEST_EAST,
    [DIR_NORTHWEST] = ANIM_STD_GO_FASTEST_WEST,
    [DIR_NORTHEAST] = ANIM_STD_GO_FASTEST_EAST,
};

static const u8 sAcroStandingWheelieBackWheelDirectionAnimNums[] = {
    [DIR_NONE]      = ANIM_STANDING_WHEELIE_BACK_WHEEL_SOUTH,
    [DIR_SOUTH]     = ANIM_STANDING_WHEELIE_BACK_WHEEL_SOUTH,
    [DIR_NORTH]     = ANIM_STANDING_WHEELIE_BACK_WHEEL_NORTH,
    [DIR_WEST]      = ANIM_STANDING_WHEELIE_BACK_WHEEL_WEST,
    [DIR_EAST]      = ANIM_STANDING_WHEELIE_BACK_WHEEL_EAST,
    [DIR_SOUTHWEST] = ANIM_STANDING_WHEELIE_BACK_WHEEL_WEST,
    [DIR_SOUTHEAST] = ANIM_STANDING_WHEELIE_BACK_WHEEL_EAST,
    [DIR_NORTHWEST] = ANIM_STANDING_WHEELIE_BACK_WHEEL_WEST,
    [DIR_NORTHEAST] = ANIM_STANDING_WHEELIE_BACK_WHEEL_EAST,
};

static const u8 sRunningDirectionAnimNums[] = {
    [DIR_NONE]      = ANIM_RUN_SOUTH,
    [DIR_SOUTH]     = ANIM_RUN_SOUTH,
    [DIR_NORTH]     = ANIM_RUN_NORTH,
    [DIR_WEST]      = ANIM_RUN_WEST,
    [DIR_EAST]      = ANIM_RUN_EAST,
    [DIR_SOUTHWEST] = ANIM_RUN_WEST,
    [DIR_SOUTHEAST] = ANIM_RUN_EAST,
    [DIR_NORTHWEST] = ANIM_RUN_WEST,
    [DIR_NORTHEAST] = ANIM_RUN_EAST,
};

static const struct Coords16 sDirectionToVectors[] = {
    [DIR_NONE]      = { 0,  0},
    [DIR_SOUTH]     = { 0,  1},
    [DIR_NORTH]     = { 0, -1},
    [DIR_WEST]      = {-1,  0},
    [DIR_EAST]      = { 1,  0},
    [DIR_SOUTHWEST] = {-1,  1},
    [DIR_SOUTHEAST] = { 1,  1},
    [DIR_NORTHWEST] = {-1, -1},
    [DIR_NORTHEAST] = { 1, -1},
    {-2,  1},
    { 2,  1},
    {-2, -1},
    { 2, -1}
};

static const u8 sFaceDirectionMovementActions[] = {
    [DIR_NONE] = MOVEMENT_ACTION_FACE_DOWN,
    [DIR_SOUTH] = MOVEMENT_ACTION_FACE_DOWN,
    [DIR_NORTH] = MOVEMENT_ACTION_FACE_UP,
    [DIR_WEST] = MOVEMENT_ACTION_FACE_LEFT,
    [DIR_EAST] = MOVEMENT_ACTION_FACE_RIGHT,
    [DIR_SOUTHWEST] = MOVEMENT_ACTION_FACE_LEFT,
    [DIR_SOUTHEAST] = MOVEMENT_ACTION_FACE_RIGHT,
    [DIR_NORTHWEST] = MOVEMENT_ACTION_FACE_LEFT,
    [DIR_NORTHEAST] = MOVEMENT_ACTION_FACE_RIGHT
};

static const u8 sWalkInPlaceSlowMovementActions[] = {
    [DIR_NONE] = MOVEMENT_ACTION_WALK_IN_PLACE_SLOW_DOWN,
    [DIR_SOUTH] = MOVEMENT_ACTION_WALK_IN_PLACE_SLOW_DOWN,
    [DIR_NORTH] = MOVEMENT_ACTION_WALK_IN_PLACE_SLOW_UP,
    [DIR_WEST] = MOVEMENT_ACTION_WALK_IN_PLACE_SLOW_LEFT,
    [DIR_EAST] = MOVEMENT_ACTION_WALK_IN_PLACE_SLOW_RIGHT,
    [DIR_SOUTHWEST] = MOVEMENT_ACTION_WALK_IN_PLACE_SLOW_LEFT,
    [DIR_NORTHWEST] = MOVEMENT_ACTION_WALK_IN_PLACE_SLOW_LEFT,
    [DIR_NORTHEAST] = MOVEMENT_ACTION_WALK_IN_PLACE_SLOW_RIGHT,
    [DIR_SOUTHEAST] = MOVEMENT_ACTION_WALK_IN_PLACE_SLOW_RIGHT
};

static const u8 sWalkInPlaceNormalMovementActions[] = {
    [DIR_NONE]  = MOVEMENT_ACTION_WALK_IN_PLACE_NORMAL_DOWN,
    [DIR_SOUTH] = MOVEMENT_ACTION_WALK_IN_PLACE_NORMAL_DOWN,
    [DIR_NORTH] = MOVEMENT_ACTION_WALK_IN_PLACE_NORMAL_UP,
    [DIR_WEST]  = MOVEMENT_ACTION_WALK_IN_PLACE_NORMAL_LEFT,
    [DIR_EAST]  = MOVEMENT_ACTION_WALK_IN_PLACE_NORMAL_RIGHT,
    [DIR_SOUTHWEST] = MOVEMENT_ACTION_WALK_IN_PLACE_NORMAL_LEFT,
    [DIR_NORTHWEST] = MOVEMENT_ACTION_WALK_IN_PLACE_NORMAL_LEFT,
    [DIR_NORTHEAST] = MOVEMENT_ACTION_WALK_IN_PLACE_NORMAL_RIGHT,
    [DIR_SOUTHEAST] = MOVEMENT_ACTION_WALK_IN_PLACE_NORMAL_RIGHT
};

static const u8 sWalkInPlaceFastMovementActions[] = {
    [DIR_NONE]  = MOVEMENT_ACTION_WALK_IN_PLACE_FAST_DOWN,
    [DIR_SOUTH] = MOVEMENT_ACTION_WALK_IN_PLACE_FAST_DOWN,
    [DIR_NORTH] = MOVEMENT_ACTION_WALK_IN_PLACE_FAST_UP,
    [DIR_WEST]  = MOVEMENT_ACTION_WALK_IN_PLACE_FAST_LEFT,
    [DIR_EAST]  = MOVEMENT_ACTION_WALK_IN_PLACE_FAST_RIGHT,
    [DIR_SOUTHWEST] = MOVEMENT_ACTION_WALK_IN_PLACE_FAST_LEFT,
    [DIR_NORTHWEST] = MOVEMENT_ACTION_WALK_IN_PLACE_FAST_LEFT,
    [DIR_NORTHEAST] = MOVEMENT_ACTION_WALK_IN_PLACE_FAST_RIGHT,
    [DIR_SOUTHEAST] = MOVEMENT_ACTION_WALK_IN_PLACE_FAST_RIGHT
};

static const u8 sWalkInPlaceFasterMovementActions[] = {
    [DIR_NONE]  = MOVEMENT_ACTION_WALK_IN_PLACE_FASTER_DOWN,
    [DIR_SOUTH] = MOVEMENT_ACTION_WALK_IN_PLACE_FASTER_DOWN,
    [DIR_NORTH] = MOVEMENT_ACTION_WALK_IN_PLACE_FASTER_UP,
    [DIR_WEST]  = MOVEMENT_ACTION_WALK_IN_PLACE_FASTER_LEFT,
    [DIR_EAST]  = MOVEMENT_ACTION_WALK_IN_PLACE_FASTER_RIGHT,
    [DIR_SOUTHWEST] = MOVEMENT_ACTION_WALK_IN_PLACE_FASTER_LEFT,
    [DIR_NORTHWEST] = MOVEMENT_ACTION_WALK_IN_PLACE_FASTER_LEFT,
    [DIR_NORTHEAST] = MOVEMENT_ACTION_WALK_IN_PLACE_FASTER_RIGHT,
    [DIR_SOUTHEAST] = MOVEMENT_ACTION_WALK_IN_PLACE_FASTER_RIGHT
};

static const u8 sAcroWheelieFaceDirectionMovementActions[] = {
    [DIR_NONE]  = MOVEMENT_ACTION_ACRO_WHEELIE_FACE_DOWN,
    [DIR_SOUTH] = MOVEMENT_ACTION_ACRO_WHEELIE_FACE_DOWN,
    [DIR_NORTH] = MOVEMENT_ACTION_ACRO_WHEELIE_FACE_UP,
    [DIR_WEST]  = MOVEMENT_ACTION_ACRO_WHEELIE_FACE_LEFT,
    [DIR_EAST]  = MOVEMENT_ACTION_ACRO_WHEELIE_FACE_RIGHT,
    [DIR_SOUTHWEST] = MOVEMENT_ACTION_ACRO_WHEELIE_FACE_LEFT,
    [DIR_NORTHWEST] = MOVEMENT_ACTION_ACRO_WHEELIE_FACE_LEFT,
    [DIR_NORTHEAST] = MOVEMENT_ACTION_ACRO_WHEELIE_FACE_RIGHT,
    [DIR_SOUTHEAST] = MOVEMENT_ACTION_ACRO_WHEELIE_FACE_RIGHT
};

static const u8 sAcroPopWheelieFaceDirectionMovementActions[] = {
    [DIR_NONE]  = MOVEMENT_ACTION_ACRO_POP_WHEELIE_DOWN,
    [DIR_SOUTH] = MOVEMENT_ACTION_ACRO_POP_WHEELIE_DOWN,
    [DIR_NORTH] = MOVEMENT_ACTION_ACRO_POP_WHEELIE_UP,
    [DIR_WEST]  = MOVEMENT_ACTION_ACRO_POP_WHEELIE_LEFT,
    [DIR_EAST]  = MOVEMENT_ACTION_ACRO_POP_WHEELIE_RIGHT,
    [DIR_SOUTHWEST] = MOVEMENT_ACTION_ACRO_POP_WHEELIE_LEFT,
    [DIR_NORTHWEST] = MOVEMENT_ACTION_ACRO_POP_WHEELIE_LEFT,
    [DIR_SOUTHEAST] = MOVEMENT_ACTION_ACRO_POP_WHEELIE_RIGHT,
    [DIR_NORTHEAST] = MOVEMENT_ACTION_ACRO_POP_WHEELIE_RIGHT,
};

static const u8 sAcroEndWheelieFaceDirectionMovementActions[] = {
    [DIR_NONE]  = MOVEMENT_ACTION_ACRO_END_WHEELIE_FACE_DOWN,
    [DIR_SOUTH] = MOVEMENT_ACTION_ACRO_END_WHEELIE_FACE_DOWN,
    [DIR_NORTH] = MOVEMENT_ACTION_ACRO_END_WHEELIE_FACE_UP,
    [DIR_WEST]  = MOVEMENT_ACTION_ACRO_END_WHEELIE_FACE_LEFT,
    [DIR_EAST]  = MOVEMENT_ACTION_ACRO_END_WHEELIE_FACE_RIGHT,
    [DIR_SOUTHWEST] = MOVEMENT_ACTION_ACRO_END_WHEELIE_FACE_LEFT,
    [DIR_NORTHWEST] = MOVEMENT_ACTION_ACRO_END_WHEELIE_FACE_LEFT,
    [DIR_SOUTHEAST] = MOVEMENT_ACTION_ACRO_END_WHEELIE_FACE_RIGHT,
    [DIR_NORTHEAST] = MOVEMENT_ACTION_ACRO_END_WHEELIE_FACE_RIGHT,
};

static const u8 sAcroWheelieHopFaceDirectionMovementActions[] = {
    [DIR_NONE]  = MOVEMENT_ACTION_ACRO_WHEELIE_HOP_FACE_DOWN,
    [DIR_SOUTH] = MOVEMENT_ACTION_ACRO_WHEELIE_HOP_FACE_DOWN,
    [DIR_NORTH] = MOVEMENT_ACTION_ACRO_WHEELIE_HOP_FACE_UP,
    [DIR_WEST]  = MOVEMENT_ACTION_ACRO_WHEELIE_HOP_FACE_LEFT,
    [DIR_EAST]  = MOVEMENT_ACTION_ACRO_WHEELIE_HOP_FACE_RIGHT,
    [DIR_SOUTHWEST] = MOVEMENT_ACTION_ACRO_WHEELIE_HOP_FACE_LEFT,
    [DIR_NORTHWEST] = MOVEMENT_ACTION_ACRO_WHEELIE_HOP_FACE_LEFT,
    [DIR_SOUTHEAST] = MOVEMENT_ACTION_ACRO_WHEELIE_HOP_FACE_RIGHT,
    [DIR_NORTHEAST] = MOVEMENT_ACTION_ACRO_WHEELIE_HOP_FACE_RIGHT,
};

static const u8 sAcroWheelieHopMovementActions[] = {
    [DIR_NONE]  = MOVEMENT_ACTION_ACRO_WHEELIE_HOP_DOWN,
    [DIR_SOUTH] = MOVEMENT_ACTION_ACRO_WHEELIE_HOP_DOWN,
    [DIR_NORTH] = MOVEMENT_ACTION_ACRO_WHEELIE_HOP_UP,
    [DIR_WEST]  = MOVEMENT_ACTION_ACRO_WHEELIE_HOP_LEFT,
    [DIR_EAST]  = MOVEMENT_ACTION_ACRO_WHEELIE_HOP_RIGHT,
    [DIR_SOUTHWEST] = MOVEMENT_ACTION_ACRO_WHEELIE_HOP_LEFT,
    [DIR_NORTHWEST] = MOVEMENT_ACTION_ACRO_WHEELIE_HOP_LEFT,
    [DIR_SOUTHEAST] = MOVEMENT_ACTION_ACRO_WHEELIE_HOP_RIGHT,
    [DIR_NORTHEAST] = MOVEMENT_ACTION_ACRO_WHEELIE_HOP_RIGHT,
};

static const u8 sAcroWheelieJumpMovementActions[] = {
    [DIR_NONE]  = MOVEMENT_ACTION_ACRO_WHEELIE_JUMP_DOWN,
    [DIR_SOUTH] = MOVEMENT_ACTION_ACRO_WHEELIE_JUMP_DOWN,
    [DIR_NORTH] = MOVEMENT_ACTION_ACRO_WHEELIE_JUMP_UP,
    [DIR_WEST]  = MOVEMENT_ACTION_ACRO_WHEELIE_JUMP_LEFT,
    [DIR_EAST]  = MOVEMENT_ACTION_ACRO_WHEELIE_JUMP_RIGHT,
    [DIR_SOUTHWEST] = MOVEMENT_ACTION_ACRO_WHEELIE_JUMP_LEFT,
    [DIR_NORTHWEST] = MOVEMENT_ACTION_ACRO_WHEELIE_JUMP_LEFT,
    [DIR_SOUTHEAST] = MOVEMENT_ACTION_ACRO_WHEELIE_JUMP_RIGHT,
    [DIR_NORTHEAST] = MOVEMENT_ACTION_ACRO_WHEELIE_JUMP_RIGHT,
};

static const u8 sAcroWheelieInPlaceMovementActions[] = {
    [DIR_NONE]  = MOVEMENT_ACTION_ACRO_WHEELIE_IN_PLACE_DOWN,
    [DIR_SOUTH] = MOVEMENT_ACTION_ACRO_WHEELIE_IN_PLACE_DOWN,
    [DIR_NORTH] = MOVEMENT_ACTION_ACRO_WHEELIE_IN_PLACE_UP,
    [DIR_WEST]  = MOVEMENT_ACTION_ACRO_WHEELIE_IN_PLACE_LEFT,
    [DIR_EAST]  = MOVEMENT_ACTION_ACRO_WHEELIE_IN_PLACE_RIGHT,
    [DIR_SOUTHWEST] = MOVEMENT_ACTION_ACRO_WHEELIE_IN_PLACE_LEFT,
    [DIR_NORTHWEST] = MOVEMENT_ACTION_ACRO_WHEELIE_IN_PLACE_LEFT,
    [DIR_SOUTHEAST] = MOVEMENT_ACTION_ACRO_WHEELIE_IN_PLACE_RIGHT,
    [DIR_NORTHEAST] = MOVEMENT_ACTION_ACRO_WHEELIE_IN_PLACE_RIGHT,
};

static const u8 sAcroPopWheelieMoveMovementActions[] = {
    [DIR_NONE]  = MOVEMENT_ACTION_ACRO_POP_WHEELIE_MOVE_DOWN,
    [DIR_SOUTH] = MOVEMENT_ACTION_ACRO_POP_WHEELIE_MOVE_DOWN,
    [DIR_NORTH] = MOVEMENT_ACTION_ACRO_POP_WHEELIE_MOVE_UP,
    [DIR_WEST]  = MOVEMENT_ACTION_ACRO_POP_WHEELIE_MOVE_LEFT,
    [DIR_EAST]  = MOVEMENT_ACTION_ACRO_POP_WHEELIE_MOVE_RIGHT,
    [DIR_SOUTHWEST] = MOVEMENT_ACTION_ACRO_POP_WHEELIE_MOVE_LEFT,
    [DIR_NORTHWEST] = MOVEMENT_ACTION_ACRO_POP_WHEELIE_MOVE_LEFT,
    [DIR_SOUTHEAST] = MOVEMENT_ACTION_ACRO_POP_WHEELIE_MOVE_RIGHT,
    [DIR_NORTHEAST] = MOVEMENT_ACTION_ACRO_POP_WHEELIE_MOVE_RIGHT,
};

static const u8 sAcroWheelieMoveMovementActions[] = {
    [DIR_NONE]  = MOVEMENT_ACTION_ACRO_WHEELIE_MOVE_DOWN,
    [DIR_SOUTH] = MOVEMENT_ACTION_ACRO_WHEELIE_MOVE_DOWN,
    [DIR_NORTH] = MOVEMENT_ACTION_ACRO_WHEELIE_MOVE_UP,
    [DIR_WEST]  = MOVEMENT_ACTION_ACRO_WHEELIE_MOVE_LEFT,
    [DIR_EAST]  = MOVEMENT_ACTION_ACRO_WHEELIE_MOVE_RIGHT,
    [DIR_SOUTHWEST] = MOVEMENT_ACTION_ACRO_WHEELIE_MOVE_LEFT,
    [DIR_NORTHWEST] = MOVEMENT_ACTION_ACRO_WHEELIE_MOVE_LEFT,
    [DIR_SOUTHEAST] = MOVEMENT_ACTION_ACRO_WHEELIE_MOVE_RIGHT,
    [DIR_NORTHEAST] = MOVEMENT_ACTION_ACRO_WHEELIE_MOVE_RIGHT,
};

u8 GetFaceDirectionAnimNum(u8 direction)
{
    return sFaceDirectionAnimNums[direction];
}

u8 GetMoveDirectionAnimNum(u8 direction)
{
    return sMoveDirectionAnimNums[direction];
}

u8 GetRunningDirectionAnimNum(u8 direction)
{
    return sRunningDirectionAnimNums[direction];
}

bool8 __attribute__((long_call)) MovementAction_PauseSpriteAnim(struct EventObject *objectEvent, struct Sprite *sprite);
void __attribute__((long_call)) InitMovementNormal(struct EventObject *objectEvent, struct Sprite *sprite, u8 direction, u8 speed);
bool8 UpdateMovementNormal(struct EventObject *objectEvent, struct Sprite *sprite);
//u8 __attribute__((long_call)) GetFaceDirectionAnimNum(u8);
void __attribute__((long_call)) InitNpcForMovement(struct EventObject *objectEvent, struct Sprite *sprite, u8 direction, u8 speed);
void __attribute__((long_call)) SetStepAnimHandleAlternation(struct EventObject *objectEvent, struct Sprite *sprite, u8 animNum);
bool8 __attribute__((long_call)) WaitForMovementDelay(struct Sprite *sprite);
