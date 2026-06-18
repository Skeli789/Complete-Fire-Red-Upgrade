#include "../defines.h"
#include "../../include/global.h"
#include "../../include/fieldmap.h"
#include "../../include/metatile_behavior.h"
#include "../../include/overworld.h"
#include "../../include/link.h"
#include "../../include/script.h"
#include "../../include/sprite.h"
#include "../../include/event_object_movement.h"
#include "../../include/constants/event_object_movement_constants.h"
#include "../../include/constants/event_objects.h"
#include "../../include/new/follow_me.h"
#include "../../include/new/movement_action_table.h"

#define JUMP_HALFWAY  1
#define JUMP_FINISHED ((u8)-1)
extern struct EventObject gObjectEvents[EVENT_OBJECTS_COUNT];
extern struct LinkPlayerEventObject gLinkPlayerEventObjects[4];

bool8 (*const gMovementTypeFuncs_FollowingPokemon[])(struct EventObject *, struct Sprite *);
const u8* GetFollowerScriptPointer(void);

#define movement_type_def(setup, table) \
static u8 setup##_callback(struct EventObject *, struct Sprite *);\
void setup(struct Sprite *sprite)\
{\
    UpdateObjectEventCurrentMovement(&gObjectEvents[sprite->data[0]], sprite, setup##_callback);\
}\
static u8 setup##_callback(struct EventObject *objectEvent, struct Sprite *sprite)\
{\
    return table[sprite->data[1]](objectEvent, sprite);\
}

u8 (*const gMovementActionFuncs_FollowingPokemon[])(struct EventObject *, struct Sprite *);

u8 MovementAction_Finish(struct EventObject *objectEvent __attribute__((unused)), struct Sprite *sprite __attribute__((unused)))
{
    return TRUE;
}

u8 UpdateJumpAnim(struct EventObject *objectEvent, struct Sprite *sprite, u8 callback(struct Sprite *))
{
    s16 displacements[NELEMS(sJumpDisplacements)];
    s16 x;
    s16 y;
    u8 result;

    memcpy(displacements, sJumpDisplacements, sizeof sJumpDisplacements);
    result = callback(sprite);
    if (result == JUMP_HALFWAY && displacements[sprite->data[4]] != 0)
    {
        x = 0;
        y = 0;
        MoveCoordsInDirection(objectEvent->movementDirection, &x, &y, displacements[sprite->data[4]], displacements[sprite->data[4]]);
        ShiftEventObjectCoords(objectEvent, objectEvent->currentCoords.x + x, objectEvent->currentCoords.y + y);
        objectEvent->triggerGroundEffectsOnMove = TRUE;
        objectEvent->disableCoveringGroundEffects = TRUE;
    }
    else if (result == JUMP_FINISHED)
    {
        ShiftStillEventObjectCoords(objectEvent);
        objectEvent->triggerGroundEffectsOnStop = TRUE;
        objectEvent->landingJump = TRUE;
        if (!(gFollowerState.inProgress && objectEvent == &gEventObjects[gFollowerState.objId]))
            sprite->animPaused = TRUE;
    }
    return result;
}

void InitNpcForWalkSlower_2(struct EventObject *objectEvent, struct Sprite *sprite, u8 direction)
{
    s16 x;
    s16 y;

    x = objectEvent->currentCoords.x;
    y = objectEvent->currentCoords.y;
    switch (direction)
    {
        case DIR_SOUTH:
            SetObjectEventDirection(objectEvent, DIR_NORTH);
            break;
        case DIR_NORTH:
            SetObjectEventDirection(objectEvent, DIR_SOUTH);
            break;
        case DIR_WEST:
            SetObjectEventDirection(objectEvent, DIR_EAST);
            break;
        case DIR_EAST:
            SetObjectEventDirection(objectEvent, DIR_WEST);
            break;
    }
    MoveCoords(direction, &x, &y);
    ShiftEventObjectCoords(objectEvent, x, y);
    SetWalkSlowerSpriteData(sprite, direction);
    sprite->animPaused = FALSE;
    objectEvent->triggerGroundEffectsOnMove = TRUE;
    sprite->data[2] = 1;
}

void InitNpcForMovement3(struct EventObject *objectEvent, struct Sprite *sprite, u8 direction, u8 speed)
{
    s16 x;
    s16 y;

    x = objectEvent->currentCoords.x;
    y = objectEvent->currentCoords.y;
    switch (direction)
    {
        case DIR_SOUTH:
            SetObjectEventDirection(objectEvent, DIR_NORTH);
            break;
        case DIR_NORTH:
            SetObjectEventDirection(objectEvent, DIR_SOUTH);
            break;
        case DIR_WEST:
            SetObjectEventDirection(objectEvent, DIR_EAST);
            break;
        case DIR_EAST:
            SetObjectEventDirection(objectEvent, DIR_WEST);
            break;
    }
    MoveCoords(direction, &x, &y);
    ShiftEventObjectCoords(objectEvent, x, y);
    SetSpriteDataForNormalStep(sprite, direction, speed);
    sprite->animPaused = FALSE;
    objectEvent->triggerGroundEffectsOnMove = TRUE;
    sprite->data[2] = 1;
}

void InitNpcForMovement2(struct EventObject *objectEvent, struct Sprite *sprite, u8 direction, u8 speed)
{
    s16 x;
    s16 y;

    x = objectEvent->currentCoords.x;
    y = objectEvent->currentCoords.y;
    switch (direction)
    {
        case DIR_NORTHWEST:
            SetObjectEventDirection(objectEvent, DIR_WEST);
            break;
        case DIR_SOUTHWEST:
            SetObjectEventDirection(objectEvent, DIR_WEST);
            break;
        case DIR_NORTHEAST:
            SetObjectEventDirection(objectEvent, DIR_EAST);
            break;
        case DIR_SOUTHEAST:
            SetObjectEventDirection(objectEvent, DIR_EAST);
            break;
    }
    MoveCoords(direction, &x, &y);
    ShiftEventObjectCoords(objectEvent, x, y);
    SetSpriteDataForNormalStep(sprite, direction, speed);
    sprite->animPaused = FALSE;
    objectEvent->triggerGroundEffectsOnMove = TRUE;
    sprite->data[2] = 1;
}

void InitNpcForMovement4(struct EventObject *objectEvent, struct Sprite *sprite, u8 direction, u8 speed)
{
    s16 x;
    s16 y;

    x = objectEvent->currentCoords.x;
    y = objectEvent->currentCoords.y;
    switch (direction)
    {
        case DIR_NORTHWEST:
            SetObjectEventDirection(objectEvent, DIR_NORTH);
            break;
        case DIR_SOUTHWEST:
            SetObjectEventDirection(objectEvent, DIR_SOUTH);
            break;
        case DIR_NORTHEAST:
            SetObjectEventDirection(objectEvent, DIR_NORTH);
            break;
        case DIR_SOUTHEAST:
            SetObjectEventDirection(objectEvent, DIR_SOUTH);
            break;
    }
    MoveCoords(direction, &x, &y);
    ShiftEventObjectCoords(objectEvent, x, y);
    SetSpriteDataForNormalStep(sprite, direction, speed);
    sprite->animPaused = FALSE;
    objectEvent->triggerGroundEffectsOnMove = TRUE;
    sprite->data[2] = 1;
}

void InitJump2(struct EventObject *objectEvent, struct Sprite *sprite, u8 direction, u8 distance, u8 type)
{
    s16 displacements[NELEMS(sJumpInitDisplacements)];
    s16 x;
    s16 y;

    memcpy(displacements, sJumpInitDisplacements, sizeof sJumpInitDisplacements);
    x = 0;
    y = 0;
    switch (direction)
    {
        case DIR_SOUTH:
            SetObjectEventDirection(objectEvent, DIR_NORTH);
            break;
        case DIR_NORTH:
            SetObjectEventDirection(objectEvent, DIR_SOUTH);
            break;
        case DIR_WEST:
            SetObjectEventDirection(objectEvent, DIR_EAST);
            break;
        case DIR_EAST:
            SetObjectEventDirection(objectEvent, DIR_WEST);
            break;

        default:
            SetObjectEventDirection(objectEvent, DIR_SOUTH);
            break;
    }
    MoveCoordsInDirection(direction, &x, &y, displacements[distance], displacements[distance]);
    ShiftEventObjectCoords(objectEvent, objectEvent->currentCoords.x + x, objectEvent->currentCoords.y + y);
    SetJumpSpriteData(sprite, direction, distance, type);
    sprite->data[2] = 1;
    sprite->animPaused = 0;
    objectEvent->triggerGroundEffectsOnMove = 1;
    objectEvent->disableCoveringGroundEffects = 1;
}

void InitNpcForWalkSlow2(struct EventObject *objectEvent, struct Sprite *sprite, u8 direction)
{
    s16 x;
    s16 y;

    x = objectEvent->currentCoords.x;
    y = objectEvent->currentCoords.y;
    switch (direction)
    {
        case DIR_SOUTH:
            SetObjectEventDirection(objectEvent, DIR_NORTH);
            break;
        case DIR_NORTH:
            SetObjectEventDirection(objectEvent, DIR_SOUTH);
            break;
        case DIR_WEST:
            SetObjectEventDirection(objectEvent, DIR_EAST);
            break;
        case DIR_EAST:
            SetObjectEventDirection(objectEvent, DIR_WEST);
            break;

        default:
            SetObjectEventDirection(objectEvent, DIR_SOUTH);
            break;
    }
    MoveCoords(direction, &x, &y);
    ShiftEventObjectCoords(objectEvent, x, y);
    SetWalkSlowSpriteData(sprite, direction);
    sprite->animPaused = FALSE;
    objectEvent->triggerGroundEffectsOnMove = TRUE;
    sprite->data[2] = 1;
}

void InitJumpRegular2(struct EventObject *objectEvent, struct Sprite *sprite, u8 direction, u8 distance, u8 type)
{
    InitJump2(objectEvent, sprite, direction, distance, type);
    SetStepAnimHandleAlternation(objectEvent, sprite, GetMoveDirectionAnimNum(objectEvent->facingDirection));
    DoShadowFieldEffect(objectEvent);
}

void InitWalkSlower_2(struct EventObject *objectEvent, struct Sprite *sprite, u8 direction)
{
    InitNpcForWalkSlower_2(objectEvent, sprite, direction);
    SetStepAnimHandleAlternation(objectEvent, sprite, GetMoveDirectionAnimNum(objectEvent->facingDirection));
}

void InitWalkSlow2(struct EventObject *objectEvent, struct Sprite *sprite, u8 direction)
{
    InitNpcForWalkSlow2(objectEvent, sprite, direction);
    SetStepAnimHandleAlternation(objectEvent, sprite, GetMoveDirectionAnimNum(objectEvent->facingDirection));
}

void StartNormalAnim(struct EventObject *objectEvent, struct Sprite *sprite, u8 direction)
{
    InitNpcForMovement2(objectEvent, sprite, direction, MOVE_SPEED_NORMAL);
    SetStepAnimHandleAlternation(objectEvent, sprite, GetMoveDirectionAnimNum(objectEvent->facingDirection));
}

void StartNormalAnim2(struct EventObject *objectEvent, struct Sprite *sprite, u8 direction)
{
    InitNpcForMovement3(objectEvent, sprite, direction, MOVE_SPEED_NORMAL);
    SetStepAnimHandleAlternation(objectEvent, sprite, GetMoveDirectionAnimNum(objectEvent->facingDirection));
}

void StartNormalAnim3(struct EventObject *objectEvent, struct Sprite *sprite, u8 direction)
{
    InitNpcForMovement4(objectEvent, sprite, direction, MOVE_SPEED_NORMAL);
    SetStepAnimHandleAlternation(objectEvent, sprite, GetMoveDirectionAnimNum(objectEvent->facingDirection));
}

void StartNormalFastAnim(struct EventObject *objectEvent, struct Sprite *sprite, u8 direction)
{
    InitNpcForMovement2(objectEvent, sprite, direction, MOVE_SPEED_FAST_1);
    SetStepAnimHandleAlternation(objectEvent, sprite, GetMoveDirectionAnimNum(objectEvent->facingDirection));
}

void StartNormalFastAnim2(struct EventObject *objectEvent, struct Sprite *sprite, u8 direction)
{
    InitNpcForMovement3(objectEvent, sprite, direction, MOVE_SPEED_FAST_1);
    SetStepAnimHandleAlternation(objectEvent, sprite, GetMoveDirectionAnimNum(objectEvent->facingDirection));
}

void StartNormalFastAnim3(struct EventObject *objectEvent, struct Sprite *sprite, u8 direction)
{
    InitNpcForMovement4(objectEvent, sprite, direction, MOVE_SPEED_FAST_1);
    SetStepAnimHandleAlternation(objectEvent, sprite, GetMoveDirectionAnimNum(objectEvent->facingDirection));
}

void StartRunningAnim(struct EventObject *objectEvent, struct Sprite *sprite, u8 direction)
{
    InitNpcForMovement2(objectEvent, sprite, direction, MOVE_SPEED_FAST_1);
    SetStepAnimHandleAlternation(objectEvent, sprite, GetRunningDirectionAnimNum(objectEvent->facingDirection));
}

void StartRunningAnim3(struct EventObject *objectEvent, struct Sprite *sprite, u8 direction)
{
    InitNpcForMovement4(objectEvent, sprite, direction, MOVE_SPEED_FAST_1);
    SetStepAnimHandleAlternation(objectEvent, sprite, GetRunningDirectionAnimNum(objectEvent->facingDirection));
}

//Walk Left/Down face Left
static bool8 MovementAction_WalkNormalDownLeft_Left_Step1(struct EventObject *objectEvent, struct Sprite *sprite)
{
    if (UpdateMovementNormal(objectEvent, sprite))
    {
        sprite->data[2] = 2;
        return TRUE;
    }
    return FALSE;
}

static bool8 MovementAction_WalkNormalDownLeft_Left_Step0(struct EventObject *objectEvent, struct Sprite *sprite)
{
    StartNormalAnim(objectEvent, sprite, DIR_SOUTHWEST);
    return MovementAction_WalkNormalDownLeft_Left_Step1(objectEvent, sprite);
}

bool8 (*const sMovementActionFuncs_WalkNormalDownLeft_FaceLeft[])(struct EventObject *, struct Sprite *) = {
    MovementAction_WalkNormalDownLeft_Left_Step0,
    MovementAction_WalkNormalDownLeft_Left_Step1,
    MovementAction_PauseSpriteAnim
};

//Walk Right/Down face Right
static bool8 MovementAction_WalkNormalDownRight_Right_Step1(struct EventObject *objectEvent, struct Sprite *sprite)
{
    if (UpdateMovementNormal(objectEvent, sprite))
    {
        sprite->data[2] = 2;
        return TRUE;
    }
    return FALSE;
}

static bool8 MovementAction_WalkNormalDownRight_Right_Step0(struct EventObject *objectEvent, struct Sprite *sprite)
{
    if(objectEvent->facingDirection != DIR_EAST)
        StartSpriteAnim(sprite, GetFaceDirectionAnimNum(DIR_EAST));

    StartNormalAnim(objectEvent, sprite, DIR_SOUTHEAST);
    return MovementAction_WalkNormalDownRight_Right_Step1(objectEvent, sprite);
}

bool8 (*const sMovementActionFuncs_WalkNormalDownRight_FaceRight[])(struct EventObject *, struct Sprite *) = {
    MovementAction_WalkNormalDownRight_Right_Step0,
    MovementAction_WalkNormalDownRight_Right_Step1,
    MovementAction_PauseSpriteAnim
};

//Walk Left/Up face Left
static bool8 MovementAction_WalkNormalUpLeft_Left_Step1(struct EventObject *objectEvent, struct Sprite *sprite)
{
    if (UpdateMovementNormal(objectEvent, sprite))
    {
        sprite->data[2] = 2;
        return TRUE;
    }
    return FALSE;
}

static bool8 MovementAction_WalkNormalUpLeft_Left_Step0(struct EventObject *objectEvent, struct Sprite *sprite)
{
    if(objectEvent->facingDirection != DIR_WEST)
        StartSpriteAnim(sprite, GetFaceDirectionAnimNum(DIR_WEST));

    StartNormalAnim(objectEvent, sprite, DIR_NORTHWEST);
    return MovementAction_WalkNormalUpLeft_Left_Step1(objectEvent, sprite);
}

bool8 (*const sMovementActionFuncs_WalkNormalUpLeft_FaceLeft[])(struct EventObject *, struct Sprite *) = {
    MovementAction_WalkNormalUpLeft_Left_Step0,
    MovementAction_WalkNormalUpLeft_Left_Step1,
    MovementAction_PauseSpriteAnim
};

//Walk Right/Up face Right
static bool8 MovementAction_WalkNormalUpRight_Right_Step1(struct EventObject *objectEvent, struct Sprite *sprite)
{
    if (UpdateMovementNormal(objectEvent, sprite))
    {
        sprite->data[2] = 2;
        return TRUE;
    }
    return FALSE;
}

static bool8 MovementAction_WalkNormalUpRight_Right_Step0(struct EventObject *objectEvent, struct Sprite *sprite)
{
    StartNormalAnim(objectEvent, sprite, DIR_NORTHEAST);
    return MovementAction_WalkNormalUpRight_Right_Step1(objectEvent, sprite);
}

bool8 (*const sMovementActionFuncs_WalkNormalUpRight_FaceRight[])(struct EventObject *, struct Sprite *) = {
    MovementAction_WalkNormalUpRight_Right_Step0,
    MovementAction_WalkNormalUpRight_Right_Step1,
    MovementAction_PauseSpriteAnim
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////

static bool8 MovementAction_PlayerRunLeftDown_Left_Step1(struct EventObject *objectEvent, struct Sprite *sprite)
{
    if (UpdateMovementNormal(objectEvent, sprite))
    {
        sprite->data[2] = 2;
        return TRUE;
    }
    return FALSE;
}

static bool8 MovementAction_PlayerRunLeftDown_Left_Step0(struct EventObject *objectEvent, struct Sprite *sprite)
{
    StartRunningAnim(objectEvent, sprite, DIR_SOUTHWEST);
    return MovementAction_PlayerRunLeftDown_Left_Step1(objectEvent, sprite);
}

bool8 (*const sMovementActionFuncs_PayerRunLeftDown_FaceLeft[])(struct EventObject *, struct Sprite *) = {
    MovementAction_PlayerRunLeftDown_Left_Step0,
    MovementAction_PlayerRunLeftDown_Left_Step1,
    MovementAction_PauseSpriteAnim
};

static bool8 MovementAction_PlayerRunRightDown_Right_Step1(struct EventObject *objectEvent, struct Sprite *sprite)
{
    if (UpdateMovementNormal(objectEvent, sprite))
    {
        sprite->data[2] = 2;
        return TRUE;
    }
    return FALSE;
}

static bool8 MovementAction_PlayerRunRightDown_Right_Step0(struct EventObject *objectEvent, struct Sprite *sprite)
{
    StartRunningAnim(objectEvent, sprite, DIR_SOUTHEAST);
    return MovementAction_PlayerRunRightDown_Right_Step1(objectEvent, sprite);
}

bool8 (*const sMovementActionFuncs_PayerRunRightDown_FaceRight[])(struct EventObject *, struct Sprite *) = {
    MovementAction_PlayerRunRightDown_Right_Step0,
    MovementAction_PlayerRunRightDown_Right_Step1,
    MovementAction_PauseSpriteAnim
};

static bool8 MovementAction_PlayerRunLeftUp_Left_Step1(struct EventObject *objectEvent, struct Sprite *sprite)
{
    if (UpdateMovementNormal(objectEvent, sprite))
    {
        sprite->data[2] = 2;
        return TRUE;
    }
    return FALSE;
}

static bool8 MovementAction_PlayerRunLeftUp_Left_Step0(struct EventObject *objectEvent, struct Sprite *sprite)
{
    StartRunningAnim(objectEvent, sprite, DIR_NORTHWEST);
    return MovementAction_PlayerRunLeftUp_Left_Step1(objectEvent, sprite);
}

bool8 (*const sMovementActionFuncs_PayerRunLeftUp_FaceLeft[])(struct EventObject *, struct Sprite *) = {
    MovementAction_PlayerRunLeftUp_Left_Step0,
    MovementAction_PlayerRunLeftUp_Left_Step1,
    MovementAction_PauseSpriteAnim
};

static bool8 MovementAction_PlayerRunRightUp_Right_Step1(struct EventObject *objectEvent, struct Sprite *sprite)
{
    if (UpdateMovementNormal(objectEvent, sprite))
    {
        sprite->data[2] = 2;
        return TRUE;
    }
    return FALSE;
}

static bool8 MovementAction_PlayerRunRightUp_Right_Step0(struct EventObject *objectEvent, struct Sprite *sprite)
{
    StartRunningAnim(objectEvent, sprite, DIR_NORTHEAST);
    return MovementAction_PlayerRunRightUp_Right_Step1(objectEvent, sprite);
}

bool8 (*const sMovementActionFuncs_PayerRunRightUp_FaceRight[])(struct EventObject *, struct Sprite *) = {
    MovementAction_PlayerRunRightUp_Right_Step0,
    MovementAction_PlayerRunRightUp_Right_Step1,
    MovementAction_PauseSpriteAnim
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//Walk Fast/Down Left
static bool8 MovementAction_WalkFastDownLeft_Left_Step1(struct EventObject *objectEvent, struct Sprite *sprite)
{
    if (UpdateMovementNormal(objectEvent, sprite))
    {
        sprite->data[2] = 2;
        return TRUE; // Movimento concluído
    }
    return FALSE; // Movimento ainda em progresso
}

static bool8 MovementAction_WalkFastDownLeft_Left_Step0(struct EventObject *objectEvent, struct Sprite *sprite)
{
    StartNormalFastAnim(objectEvent, sprite, DIR_SOUTHWEST);
    return MovementAction_WalkFastDownLeft_Left_Step1(objectEvent, sprite);
}

bool8 (*const sMovementActionFuncs_WalkFasterDownLeft_Left[])(struct EventObject *, struct Sprite *) = {
    MovementAction_WalkFastDownLeft_Left_Step0,
    MovementAction_WalkFastDownLeft_Left_Step1,
    MovementAction_PauseSpriteAnim,
};

static bool8 MovementAction_WalkFastDownRight_Right_Step1(struct EventObject *objectEvent, struct Sprite *sprite)
{
    if (UpdateMovementNormal(objectEvent, sprite))
    {
        sprite->data[2] = 2;
        return TRUE; // Movimento concluído
    }
    return FALSE; // Movimento ainda em progresso
}

static bool8 MovementAction_WalkFastDownRight_Right_Step0(struct EventObject *objectEvent, struct Sprite *sprite)
{
    StartNormalFastAnim(objectEvent, sprite, DIR_SOUTHEAST);
    return MovementAction_WalkFastDownRight_Right_Step1(objectEvent, sprite);
}

bool8 (*const sMovementActionFuncs_WalkFasterDownRight_Right[])(struct EventObject *, struct Sprite *) = {
    MovementAction_WalkFastDownRight_Right_Step0,
    MovementAction_WalkFastDownRight_Right_Step1,
    MovementAction_PauseSpriteAnim,
};

static bool8 MovementAction_WalkFastUpLeft_Left_Step1(struct EventObject *objectEvent, struct Sprite *sprite)
{
    if (UpdateMovementNormal(objectEvent, sprite))
    {
        sprite->data[2] = 2;
        return TRUE; // Movimento concluído
    }
    return FALSE; // Movimento ainda em progresso
}

static bool8 MovementAction_WalkFastUpLeft_Left_Step0(struct EventObject *objectEvent, struct Sprite *sprite)
{
    StartNormalFastAnim(objectEvent, sprite, DIR_NORTHWEST);
    return MovementAction_WalkFastUpLeft_Left_Step1(objectEvent, sprite);
}

bool8 (*const sMovementActionFuncs_WalkFasterUpLeft_Left[])(struct EventObject *, struct Sprite *) = {
    MovementAction_WalkFastUpLeft_Left_Step0,
    MovementAction_WalkFastUpLeft_Left_Step1,
    MovementAction_PauseSpriteAnim,
};

static bool8 MovementAction_WalkFastUpRight_Right_Step1(struct EventObject *objectEvent, struct Sprite *sprite)
{
    if (UpdateMovementNormal(objectEvent, sprite))
    {
        sprite->data[2] = 2;
        return TRUE; // Movimento concluído
    }
    return FALSE; // Movimento ainda em progresso
}

static bool8 MovementAction_WalkFastUpRight_Right_Step0(struct EventObject *objectEvent, struct Sprite *sprite)
{
    StartNormalFastAnim(objectEvent, sprite, DIR_NORTHEAST);
    return MovementAction_WalkFastUpRight_Right_Step1(objectEvent, sprite);
}

bool8 (*const sMovementActionFuncs_WalkFasterUpRight_Right[])(struct EventObject *, struct Sprite *) = {
    MovementAction_WalkFastUpRight_Right_Step0,
    MovementAction_WalkFastUpRight_Right_Step1,
    MovementAction_PauseSpriteAnim,
};

//////////////////////////////////////////////////////////////////////////////////////////

static bool8 MovementAction_WalkSlowerDown_Backwards_Step1(struct EventObject *objectEvent, struct Sprite *sprite)
{
    if (UpdateWalkSlower(objectEvent, sprite))
    {
        sprite->data[2] = 2;
        return TRUE;
    }
    return FALSE;
}

static bool8 MovementAction_WalkSlowerDown_Backwards_Step0(struct EventObject *objectEvent, struct Sprite *sprite)
{
    InitWalkSlower_2(objectEvent, sprite, DIR_SOUTH);
    //SetObjectEventDirection(objectEvent, DIR_NORTH);
    return MovementAction_WalkSlowerDown_Backwards_Step1(objectEvent, sprite);
}

bool8 (*const sMovementActionFuncs_WalkSlowerDown_Backwards[])(struct EventObject *, struct Sprite *) = {
    MovementAction_WalkSlowerDown_Backwards_Step0,
    MovementAction_WalkSlowerDown_Backwards_Step1,
    MovementAction_PauseSpriteAnim,
};

static bool8 MovementAction_WalkSlowerUp_Backwards_Step1(struct EventObject *objectEvent, struct Sprite *sprite)
{
    if (UpdateWalkSlower(objectEvent, sprite))
    {
        sprite->data[2] = 2;
        return TRUE;
    }
    return FALSE;
}

static bool8 MovementAction_WalkSlowerUp_Backwards_Step0(struct EventObject *objectEvent, struct Sprite *sprite)
{
    InitWalkSlower_2(objectEvent, sprite, DIR_NORTH);
    return MovementAction_WalkSlowerUp_Backwards_Step1(objectEvent, sprite);
}

bool8 (*const sMovementActionFuncs_WalkSlowerUp_Backwards[])(struct EventObject *, struct Sprite *) = {
    MovementAction_WalkSlowerUp_Backwards_Step0,
    MovementAction_WalkSlowerUp_Backwards_Step1,
    MovementAction_PauseSpriteAnim,
};

static bool8 MovementAction_WalkSlowerRight_Backwards_Step1(struct EventObject *objectEvent, struct Sprite *sprite)
{
    if (UpdateWalkSlower(objectEvent, sprite))
    {
        sprite->data[2] = 2;
        return TRUE;
    }
    return FALSE;
}

static bool8 MovementAction_WalkSlowerRight_Backwards_Step0(struct EventObject *objectEvent, struct Sprite *sprite)
{
    InitWalkSlower_2(objectEvent, sprite, DIR_EAST);
    return MovementAction_WalkSlowerRight_Backwards_Step1(objectEvent, sprite);
}

bool8 (*const sMovementActionFuncs_WalkSlowerRight_Backwards[])(struct EventObject *, struct Sprite *) = {
    MovementAction_WalkSlowerRight_Backwards_Step0,
    MovementAction_WalkSlowerRight_Backwards_Step1,
    MovementAction_PauseSpriteAnim,
};

static bool8 MovementAction_WalkSlowerLeft_Backwards_Step1(struct EventObject *objectEvent, struct Sprite *sprite)
{
    if (UpdateWalkSlower(objectEvent, sprite))
    {
        sprite->data[2] = 2;
        return TRUE;
    }
    return FALSE;
}

static bool8 MovementAction_WalkSlowerLeft_Backwards_Step0(struct EventObject *objectEvent, struct Sprite *sprite)
{
    InitWalkSlower_2(objectEvent, sprite, DIR_WEST);
    return MovementAction_WalkSlowerLeft_Backwards_Step1(objectEvent, sprite);
}

bool8 (*const sMovementActionFuncs_WalkSlowerLeft_Backwards[])(struct EventObject *, struct Sprite *) = {
    MovementAction_WalkSlowerLeft_Backwards_Step0,
    MovementAction_WalkSlowerLeft_Backwards_Step1,
    MovementAction_PauseSpriteAnim,
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static bool8 MovementAction_WalkSlowUp_Backwards_Step1(struct EventObject *eventObject, struct Sprite *sprite)
{
    if (UpdateWalkSlow(eventObject, sprite))
    {
        sprite->data[2] = 2;
        return TRUE;
    }
    return FALSE;
}

static bool8 MovementAction_WalkSlowUp_Backwards_Step0(struct EventObject *eventObject, struct Sprite *sprite)
{
    InitWalkSlow2(eventObject, sprite, DIR_NORTH);
    return MovementAction_WalkSlowUp_Backwards_Step1(eventObject, sprite);
}

bool8 (*const sMovementActionFuncs_WalkSlowUp_Backwards[])(struct EventObject *, struct Sprite *) = {
    MovementAction_WalkSlowUp_Backwards_Step0,
    MovementAction_WalkSlowUp_Backwards_Step1,
    MovementAction_PauseSpriteAnim,
};

static bool8 MovementAction_WalkSlowDown_Backwards_Step1(struct EventObject *eventObject, struct Sprite *sprite)
{
    if (UpdateWalkSlow(eventObject, sprite))
    {
        sprite->data[2] = 2;
        return TRUE;
    }
    return FALSE;
}

static bool8 MovementAction_WalkSlowDown_Backwards_Step0(struct EventObject *eventObject, struct Sprite *sprite)
{
    InitWalkSlow2(eventObject, sprite, DIR_SOUTH);
    return MovementAction_WalkSlowDown_Backwards_Step1(eventObject, sprite);
}

bool8 (*const sMovementActionFuncs_WalkSlowDown_Backwards[])(struct EventObject *, struct Sprite *) = {
    MovementAction_WalkSlowDown_Backwards_Step0,
    MovementAction_WalkSlowDown_Backwards_Step1,
    MovementAction_PauseSpriteAnim,
};

static bool8 MovementAction_WalkSlowRight_Backwards_Step1(struct EventObject *eventObject, struct Sprite *sprite)
{
    if (UpdateWalkSlow(eventObject, sprite))
    {
        sprite->data[2] = 2;
        return TRUE;
    }
    return FALSE;
}

static bool8 MovementAction_WalkSlowRight_Backwards_Step0(struct EventObject *eventObject, struct Sprite *sprite)
{
    InitWalkSlow2(eventObject, sprite, DIR_EAST);
    return MovementAction_WalkSlowRight_Backwards_Step1(eventObject, sprite);
}

bool8 (*const sMovementActionFuncs_WalkSlowRight_Backwards[])(struct EventObject *, struct Sprite *) = {
    MovementAction_WalkSlowRight_Backwards_Step0,
    MovementAction_WalkSlowRight_Backwards_Step1,
    MovementAction_PauseSpriteAnim,
};

static bool8 MovementAction_WalkSlowLeft_Backwards_Step1(struct EventObject *eventObject, struct Sprite *sprite)
{
    if (UpdateWalkSlow(eventObject, sprite))
    {
        sprite->data[2] = 2;
        return TRUE;
    }
    return FALSE;
}

static bool8 MovementAction_WalkSlowLeft_Backwards_Step0(struct EventObject *eventObject, struct Sprite *sprite)
{
    InitWalkSlow2(eventObject, sprite, DIR_WEST);
    return MovementAction_WalkSlowLeft_Backwards_Step1(eventObject, sprite);
}

bool8 (*const sMovementActionFuncs_WalkSlowLeft_Backwards[])(struct EventObject *, struct Sprite *) = {
    MovementAction_WalkSlowLeft_Backwards_Step0,
    MovementAction_WalkSlowLeft_Backwards_Step1,
    MovementAction_PauseSpriteAnim,
};

///////////////////////////////////////////////////////////////////////////////////////////////

static bool8 MovementAction_WalkNormalUp_Backwards_Step1(struct EventObject *objectEvent, struct Sprite *sprite)
{
    if (UpdateMovementNormal(objectEvent, sprite))
    {
        sprite->data[2] = 2;
        return TRUE;
    }
    return FALSE;
}

static bool8 MovementAction_WalkNormalUp_Backwards_Step0(struct EventObject *objectEvent, struct Sprite *sprite)
{
    StartNormalAnim2(objectEvent, sprite, DIR_NORTH);
    return MovementAction_WalkNormalUp_Backwards_Step1(objectEvent, sprite);
}

bool8 (*const sMovementActionFuncs_WalkNormalUp_Backwards[])(struct EventObject *, struct Sprite *) = {
    MovementAction_WalkNormalUp_Backwards_Step0,
    MovementAction_WalkNormalUp_Backwards_Step1,
    MovementAction_PauseSpriteAnim
};

static bool8 MovementAction_WalkNormalDown_Backwards_Step1(struct EventObject *objectEvent, struct Sprite *sprite)
{
    if (UpdateMovementNormal(objectEvent, sprite))
    {
        sprite->data[2] = 2;
        return TRUE;
    }
    return FALSE;
}

static bool8 MovementAction_WalkNormalDown_Backwards_Step0(struct EventObject *objectEvent, struct Sprite *sprite)
{
    StartNormalAnim2(objectEvent, sprite, DIR_SOUTH);
    return MovementAction_WalkNormalDown_Backwards_Step1(objectEvent, sprite);
}

bool8 (*const sMovementActionFuncs_WalkNormalDown_Backwards[])(struct EventObject *, struct Sprite *) = {
    MovementAction_WalkNormalDown_Backwards_Step0,
    MovementAction_WalkNormalDown_Backwards_Step1,
    MovementAction_PauseSpriteAnim
};

static bool8 MovementAction_WalkNormalRight_Backwards_Step1(struct EventObject *objectEvent, struct Sprite *sprite)
{
    if (UpdateMovementNormal(objectEvent, sprite))
    {
        sprite->data[2] = 2;
        return TRUE;
    }
    return FALSE;
}

static bool8 MovementAction_WalkNormalRight_Backwards_Step0(struct EventObject *objectEvent, struct Sprite *sprite)
{
    StartNormalAnim2(objectEvent, sprite, DIR_EAST);
    return MovementAction_WalkNormalRight_Backwards_Step1(objectEvent, sprite);
}

bool8 (*const sMovementActionFuncs_WalkNormalRight_Backwards[])(struct EventObject *, struct Sprite *) = {
    MovementAction_WalkNormalRight_Backwards_Step0,
    MovementAction_WalkNormalRight_Backwards_Step1,
    MovementAction_PauseSpriteAnim
};

static bool8 MovementAction_WalkNormalLeft_Backwards_Step1(struct EventObject *objectEvent, struct Sprite *sprite)
{
    if (UpdateMovementNormal(objectEvent, sprite))
    {
        sprite->data[2] = 2;
        return TRUE;
    }
    return FALSE;
}

static bool8 MovementAction_WalkNormalLeft_Backwards_Step0(struct EventObject *objectEvent, struct Sprite *sprite)
{
    StartNormalAnim2(objectEvent, sprite, DIR_WEST);
    return MovementAction_WalkNormalLeft_Backwards_Step1(objectEvent, sprite);
}

bool8 (*const sMovementActionFuncs_WalkNormalLeft_Backwards[])(struct EventObject *, struct Sprite *) = {
    MovementAction_WalkNormalLeft_Backwards_Step0,
    MovementAction_WalkNormalLeft_Backwards_Step1,
    MovementAction_PauseSpriteAnim
};

/////////////////////////////////////////////////////////////////////////////////////////////////

static bool8 MovementAction_WalkFastUp_Backwards_Step1(struct EventObject *objectEvent, struct Sprite *sprite)
{
    if (UpdateMovementNormal(objectEvent, sprite))
    {
        sprite->data[2] = 2;
        return TRUE;
    }
    return FALSE;
}

static bool8 MovementAction_WalkFastUp_Backwards_Step0(struct EventObject *objectEvent, struct Sprite *sprite)
{
    StartNormalFastAnim2(objectEvent, sprite, DIR_NORTH);
    return MovementAction_WalkFastUp_Backwards_Step1(objectEvent, sprite);
}

bool8 (*const sMovementActionFuncs_WalkFasterUp_Backwards[])(struct EventObject *, struct Sprite *) = {
    MovementAction_WalkFastUp_Backwards_Step0,
    MovementAction_WalkFastUp_Backwards_Step1,
    MovementAction_PauseSpriteAnim,
};

static bool8 MovementAction_WalkFastDown_Backwards_Step1(struct EventObject *objectEvent, struct Sprite *sprite)
{
    if (UpdateMovementNormal(objectEvent, sprite))
    {
        sprite->data[2] = 2;
        return TRUE;
    }
    return FALSE;
}

static bool8 MovementAction_WalkFastDown_Backwards_Step0(struct EventObject *objectEvent, struct Sprite *sprite)
{
    StartNormalFastAnim2(objectEvent, sprite, DIR_SOUTH);
    return MovementAction_WalkFastDown_Backwards_Step1(objectEvent, sprite);
}

bool8 (*const sMovementActionFuncs_WalkFasterDown_Backwards[])(struct EventObject *, struct Sprite *) = {
    MovementAction_WalkFastDown_Backwards_Step0,
    MovementAction_WalkFastDown_Backwards_Step1,
    MovementAction_PauseSpriteAnim,
};

static bool8 MovementAction_WalkFastRight_Backwards_Step1(struct EventObject *objectEvent, struct Sprite *sprite)
{
    if (UpdateMovementNormal(objectEvent, sprite))
    {
        sprite->data[2] = 2;
        return TRUE;
    }
    return FALSE;
}

static bool8 MovementAction_WalkFastRight_Backwards_Step0(struct EventObject *objectEvent, struct Sprite *sprite)
{
    StartNormalFastAnim2(objectEvent, sprite, DIR_EAST);
    return MovementAction_WalkFastRight_Backwards_Step1(objectEvent, sprite);
}

bool8 (*const sMovementActionFuncs_WalkFasterRight_Backwards[])(struct EventObject *, struct Sprite *) = {
    MovementAction_WalkFastRight_Backwards_Step0,
    MovementAction_WalkFastRight_Backwards_Step1,
    MovementAction_PauseSpriteAnim,
};

static bool8 MovementAction_WalkFastLeft_Backwards_Step1(struct EventObject *objectEvent, struct Sprite *sprite)
{
    if (UpdateMovementNormal(objectEvent, sprite))
    {
        sprite->data[2] = 2;
        return TRUE;
    }
    return FALSE;
}

static bool8 MovementAction_WalkFastLeft_Backwards_Step0(struct EventObject *objectEvent, struct Sprite *sprite)
{
    StartNormalFastAnim2(objectEvent, sprite, DIR_WEST);
    return MovementAction_WalkFastLeft_Backwards_Step1(objectEvent, sprite);
}

bool8 (*const sMovementActionFuncs_WalkFasterLeft_Backwards[])(struct EventObject *, struct Sprite *) = {
    MovementAction_WalkFastLeft_Backwards_Step0,
    MovementAction_WalkFastLeft_Backwards_Step1,
    MovementAction_PauseSpriteAnim,
};

/////////////////////////////////////////////////////////////////////////////////////////////

static bool8 MovementAction_Jump2Up_Backwards_Step1(struct EventObject *objectEvent, struct Sprite *sprite)
{
    if (DoJumpAnim(objectEvent, sprite))
    {
        objectEvent->hasShadow = FALSE;
        sprite->data[2] = 2;
        return TRUE;
    }
    return FALSE;
}

static bool8 MovementAction_Jump2Up_Backwards_Step0(struct EventObject *objectEvent, struct Sprite *sprite)
{
    InitJumpRegular2(objectEvent, sprite, DIR_NORTH, JUMP_DISTANCE_FAR, JUMP_TYPE_HIGH);
    return MovementAction_Jump2Up_Backwards_Step1(objectEvent, sprite);
}

bool8 (*const sMovementActionFuncs_Jump2Up_Backwards[])(struct EventObject *, struct Sprite *) = {
    MovementAction_Jump2Up_Backwards_Step0,
    MovementAction_Jump2Up_Backwards_Step1,
    MovementAction_PauseSpriteAnim,
};

static bool8 MovementAction_Jump2Down_Backwards_Step1(struct EventObject *objectEvent, struct Sprite *sprite)
{
    if (DoJumpAnim(objectEvent, sprite))
    {
        objectEvent->hasShadow = FALSE;
        sprite->data[2] = 2;
        return TRUE;
    }
    return FALSE;
}

static bool8 MovementAction_Jump2Down_Backwards_Step0(struct EventObject *objectEvent, struct Sprite *sprite)
{
    InitJumpRegular2(objectEvent, sprite, DIR_SOUTH, JUMP_DISTANCE_FAR, JUMP_TYPE_HIGH);
    return MovementAction_Jump2Down_Backwards_Step1(objectEvent, sprite);
}

bool8 (*const sMovementActionFuncs_Jump2Down_Backwards[])(struct EventObject *, struct Sprite *) = {
    MovementAction_Jump2Down_Backwards_Step0,
    MovementAction_Jump2Down_Backwards_Step1,
    MovementAction_PauseSpriteAnim,
};

static bool8 MovementAction_Jump2Right_Backwards_Step1(struct EventObject *objectEvent, struct Sprite *sprite)
{
    if (DoJumpAnim(objectEvent, sprite))
    {
        objectEvent->hasShadow = FALSE;
        sprite->data[2] = 2;
        return TRUE;
    }
    return FALSE;
}

static bool8 MovementAction_Jump2Right_Backwards_Step0(struct EventObject *objectEvent, struct Sprite *sprite)
{
    InitJumpRegular2(objectEvent, sprite, DIR_EAST, JUMP_DISTANCE_FAR, JUMP_TYPE_HIGH);
    return MovementAction_Jump2Right_Backwards_Step1(objectEvent, sprite);
}

bool8 (*const sMovementActionFuncs_Jump2Right_Backwards[])(struct EventObject *, struct Sprite *) = {
    MovementAction_Jump2Right_Backwards_Step0,
    MovementAction_Jump2Right_Backwards_Step1,
    MovementAction_PauseSpriteAnim,
};

static bool8 MovementAction_Jump2Left_Backwards_Step1(struct EventObject *objectEvent, struct Sprite *sprite)
{
    if (DoJumpAnim(objectEvent, sprite))
    {
        objectEvent->hasShadow = FALSE;
        sprite->data[2] = 2;
        return TRUE;
    }
    return FALSE;
}

static bool8 MovementAction_Jump2Left_Backwards_Step0(struct EventObject *objectEvent, struct Sprite *sprite)
{
    InitJumpRegular2(objectEvent, sprite, DIR_WEST, JUMP_DISTANCE_FAR, JUMP_TYPE_HIGH);
    return MovementAction_Jump2Left_Backwards_Step1(objectEvent, sprite);
}

bool8 (*const sMovementActionFuncs_Jump2Left_Backwards[])(struct EventObject *, struct Sprite *) = {
    MovementAction_Jump2Left_Backwards_Step0,
    MovementAction_Jump2Left_Backwards_Step1,
    MovementAction_PauseSpriteAnim,
};

static bool8 MovementAction_JumpUp_Backwards_Step1(struct EventObject *objectEvent, struct Sprite *sprite)
{
    if (DoJumpAnim(objectEvent, sprite))
    {
        objectEvent->hasShadow = FALSE;
        sprite->data[2] = 2;
        return TRUE;
    }
    return FALSE;
}

static bool8 MovementAction_JumpUp_Backwards_Step0(struct EventObject *objectEvent, struct Sprite *sprite)
{
    InitJumpRegular2(objectEvent, sprite, DIR_NORTH, JUMP_DISTANCE_NORMAL, JUMP_TYPE_HIGH);
    return MovementAction_JumpUp_Backwards_Step1(objectEvent, sprite);
}

bool8 (*const sMovementActionFuncs_JumpUp_Backwards[])(struct EventObject *, struct Sprite *) = {
    MovementAction_JumpUp_Backwards_Step0,
    MovementAction_JumpUp_Backwards_Step1,
    MovementAction_PauseSpriteAnim,
};

static bool8 MovementAction_JumpDown_Backwards_Step1(struct EventObject *objectEvent, struct Sprite *sprite)
{
    if (DoJumpAnim(objectEvent, sprite))
    {
        objectEvent->hasShadow = FALSE;
        sprite->data[2] = 2;
        return TRUE;
    }
    return FALSE;
}

static bool8 MovementAction_JumpDown_Backwards_Step0(struct EventObject *objectEvent, struct Sprite *sprite)
{
    InitJumpRegular2(objectEvent, sprite, DIR_SOUTH, JUMP_DISTANCE_NORMAL, JUMP_TYPE_HIGH);
    return MovementAction_JumpDown_Backwards_Step1(objectEvent, sprite);
}

bool8 (*const sMovementActionFuncs_JumpDown_Backwards[])(struct EventObject *, struct Sprite *) = {
    MovementAction_JumpDown_Backwards_Step0,
    MovementAction_JumpDown_Backwards_Step1,
    MovementAction_PauseSpriteAnim,
};

static bool8 MovementAction_JumpRight_Backwards_Step1(struct EventObject *objectEvent, struct Sprite *sprite)
{
    if (DoJumpAnim(objectEvent, sprite))
    {
        objectEvent->hasShadow = FALSE;
        sprite->data[2] = 2;
        return TRUE;
    }
    return FALSE;
}

static bool8 MovementAction_JumpRight_Backwards_Step0(struct EventObject *objectEvent, struct Sprite *sprite)
{
    InitJumpRegular2(objectEvent, sprite, DIR_EAST, JUMP_DISTANCE_NORMAL, JUMP_TYPE_HIGH);
    return MovementAction_JumpRight_Backwards_Step1(objectEvent, sprite);
}

bool8 (*const sMovementActionFuncs_JumpRight_Backwards[])(struct EventObject *, struct Sprite *) = {
    MovementAction_JumpRight_Backwards_Step0,
    MovementAction_JumpRight_Backwards_Step1,
    MovementAction_PauseSpriteAnim,
};

static bool8 MovementAction_JumpLeft_Backwards_Step1(struct EventObject *objectEvent, struct Sprite *sprite)
{
    if (DoJumpAnim(objectEvent, sprite))
    {
        objectEvent->hasShadow = FALSE;
        sprite->data[2] = 2;
        return TRUE;
    }
    return FALSE;
}

static bool8 MovementAction_JumpLeft_Backwards_Step0(struct EventObject *objectEvent, struct Sprite *sprite)
{
    InitJumpRegular2(objectEvent, sprite, DIR_WEST, JUMP_DISTANCE_NORMAL, JUMP_TYPE_HIGH);
    return MovementAction_JumpLeft_Backwards_Step1(objectEvent, sprite);
}

bool8 (*const sMovementActionFuncs_JumpLeft_Backwards[])(struct EventObject *, struct Sprite *) = {
    MovementAction_JumpLeft_Backwards_Step0,
    MovementAction_JumpLeft_Backwards_Step1,
    MovementAction_PauseSpriteAnim,
};

static bool8 MovementAction_RunDownLeft_Down_Step1(struct EventObject *objectEvent, struct Sprite *sprite)
{
    if (UpdateMovementNormal(objectEvent, sprite))
    {
        sprite->data[2] = 2;
        return TRUE;
    }
    return FALSE;
}

static bool8 MovementAction_RunDownLeft_Down_Step0(struct EventObject *objectEvent, struct Sprite *sprite)
{
    StartRunningAnim3(objectEvent, sprite, DIR_SOUTHWEST);
    return MovementAction_RunDownLeft_Down_Step1(objectEvent, sprite);
}

bool8 (*const sMovementActionFuncs_RunDownLeft_FaceDown[])(struct EventObject *, struct Sprite *) = {
    MovementAction_RunDownLeft_Down_Step0,
    MovementAction_RunDownLeft_Down_Step1,
    MovementAction_PauseSpriteAnim
};

static bool8 MovementAction_RunUpLeft_Up_Step1(struct EventObject *objectEvent, struct Sprite *sprite)
{
    if (UpdateMovementNormal(objectEvent, sprite))
    {
        sprite->data[2] = 2;
        return TRUE;
    }
    return FALSE;
}

static bool8 MovementAction_RunUpLeft_Up_Step0(struct EventObject *objectEvent, struct Sprite *sprite)
{
    StartRunningAnim3(objectEvent, sprite, DIR_NORTHWEST);
    return MovementAction_RunUpLeft_Up_Step1(objectEvent, sprite);
}

bool8 (*const sMovementActionFuncs_RunUpLeft_FaceUp[])(struct EventObject *, struct Sprite *) = {
    MovementAction_RunUpLeft_Up_Step0,
    MovementAction_RunUpLeft_Up_Step1,
    MovementAction_PauseSpriteAnim
};

static bool8 MovementAction_RunDownRight_Down_Step1(struct EventObject *objectEvent, struct Sprite *sprite)
{
    if (UpdateMovementNormal(objectEvent, sprite))
    {
        sprite->data[2] = 2;
        return TRUE;
    }
    return FALSE;
}

static bool8 MovementAction_RunDownRight_Down_Step0(struct EventObject *objectEvent, struct Sprite *sprite)
{
    StartRunningAnim3(objectEvent, sprite, DIR_SOUTHEAST);
    return MovementAction_RunDownRight_Down_Step1(objectEvent, sprite);
}

bool8 (*const sMovementActionFuncs_RunDownRight_FaceDown[])(struct EventObject *, struct Sprite *) = {
    MovementAction_RunDownRight_Down_Step0,
    MovementAction_RunDownRight_Down_Step1,
    MovementAction_PauseSpriteAnim
};

static bool8 MovementAction_RunUpRight_Up_Step1(struct EventObject *objectEvent, struct Sprite *sprite)
{
    if (UpdateMovementNormal(objectEvent, sprite))
    {
        sprite->data[2] = 2;
        return TRUE;
    }
    return FALSE;
}

static bool8 MovementAction_RunUpRight_Up_Step0(struct EventObject *objectEvent, struct Sprite *sprite)
{
    StartRunningAnim3(objectEvent, sprite, DIR_NORTHEAST);
    return MovementAction_RunUpRight_Up_Step1(objectEvent, sprite);
}

bool8 (*const sMovementActionFuncs_RunUpRight_FaceUp[])(struct EventObject *, struct Sprite *) = {
    MovementAction_RunUpRight_Up_Step0,
    MovementAction_RunUpRight_Up_Step1,
    MovementAction_PauseSpriteAnim
};

///////////////////////////////////////////////////////////////////////////////////////////

static bool8 MovementAction_WalkNormalDownLeft_Down_Step1(struct EventObject *objectEvent, struct Sprite *sprite)
{
    if (UpdateMovementNormal(objectEvent, sprite))
    {
        sprite->data[2] = 2;
        return TRUE;
    }
    return FALSE;
}

static bool8 MovementAction_WalkNormalDownLeft_Down_Step0(struct EventObject *objectEvent, struct Sprite *sprite)
{
    StartNormalAnim3(objectEvent, sprite, DIR_SOUTHWEST);
    return MovementAction_WalkNormalDownLeft_Down_Step1(objectEvent, sprite);
}

bool8 (*const sMovementActionFuncs_WalkNormalDownLeft_FaceDown[])(struct EventObject *, struct Sprite *) = {
    MovementAction_WalkNormalDownLeft_Down_Step0,
    MovementAction_WalkNormalDownLeft_Down_Step1,
    MovementAction_PauseSpriteAnim
};

static bool8 MovementAction_WalkNormalUpLeft_Up_Step1(struct EventObject *objectEvent, struct Sprite *sprite)
{
    if (UpdateMovementNormal(objectEvent, sprite))
    {
        sprite->data[2] = 2;
        return TRUE;
    }
    return FALSE;
}

static bool8 MovementAction_WalkNormalUpLeft_Up_Step0(struct EventObject *objectEvent, struct Sprite *sprite)
{
    StartNormalAnim3(objectEvent, sprite, DIR_NORTHWEST);
    return MovementAction_WalkNormalUpLeft_Up_Step1(objectEvent, sprite);
}

bool8 (*const sMovementActionFuncs_WalkNormalUpLeft_FaceUp[])(struct EventObject *, struct Sprite *) = {
    MovementAction_WalkNormalUpLeft_Up_Step0,
    MovementAction_WalkNormalUpLeft_Up_Step1,
    MovementAction_PauseSpriteAnim
};

static bool8 MovementAction_WalkNormalDownRight_Down_Step1(struct EventObject *objectEvent, struct Sprite *sprite)
{
    if (UpdateMovementNormal(objectEvent, sprite))
    {
        sprite->data[2] = 2;
        return TRUE;
    }
    return FALSE;
}

static bool8 MovementAction_WalkNormalDownRight_Down_Step0(struct EventObject *objectEvent, struct Sprite *sprite)
{
    StartNormalAnim3(objectEvent, sprite, DIR_SOUTHEAST);
    return MovementAction_WalkNormalDownRight_Down_Step1(objectEvent, sprite);
}

bool8 (*const sMovementActionFuncs_WalkNormalDownRight_FaceDown[])(struct EventObject *, struct Sprite *) = {
    MovementAction_WalkNormalDownRight_Down_Step0,
    MovementAction_WalkNormalDownRight_Down_Step1,
    MovementAction_PauseSpriteAnim
};

static bool8 MovementAction_WalkNormalUpRight_Up_Step1(struct EventObject *objectEvent, struct Sprite *sprite)
{
    if (UpdateMovementNormal(objectEvent, sprite))
    {
        sprite->data[2] = 2;
        return TRUE;
    }
    return FALSE;
}

static bool8 MovementAction_WalkNormalUpRight_Up_Step0(struct EventObject *objectEvent, struct Sprite *sprite)
{
    StartNormalAnim3(objectEvent, sprite, DIR_NORTHEAST);
    return MovementAction_WalkNormalUpRight_Up_Step1(objectEvent, sprite);
}

bool8 (*const sMovementActionFuncs_WalkNormalUpRight_FaceUp[])(struct EventObject *, struct Sprite *) = {
    MovementAction_WalkNormalUpRight_Up_Step0,
    MovementAction_WalkNormalUpRight_Up_Step1,
    MovementAction_PauseSpriteAnim
};

static bool8 MovementAction_WalkFast_DownLeft_Down_Step1(struct EventObject *objectEvent, struct Sprite *sprite)
{
    if (UpdateMovementNormal(objectEvent, sprite))
    {
        sprite->data[2] = 2;
        return TRUE;
    }
    return FALSE;
}

static bool8 MovementAction_WalkFast_DownLeft_Down_Step0(struct EventObject *objectEvent, struct Sprite *sprite)
{
    StartNormalFastAnim3(objectEvent, sprite, DIR_SOUTHWEST);
    return MovementAction_WalkFast_DownLeft_Down_Step1(objectEvent, sprite);
}

bool8 (*const sMovementActionFuncs_WalkFast_DownLeft_FaceDown[])(struct EventObject *, struct Sprite *) = {
    MovementAction_WalkFast_DownLeft_Down_Step0,
    MovementAction_WalkFast_DownLeft_Down_Step1,
    MovementAction_PauseSpriteAnim
};

static bool8 MovementAction_WalkFast_UpLeft_Up_Step1(struct EventObject *objectEvent, struct Sprite *sprite)
{
    if (UpdateMovementNormal(objectEvent, sprite))
    {
        sprite->data[2] = 2;
        return TRUE;
    }
    return FALSE;
}

static bool8 MovementAction_WalkFast_UpLeft_Up_Step0(struct EventObject *objectEvent, struct Sprite *sprite)
{
    StartNormalFastAnim3(objectEvent, sprite, DIR_NORTHWEST);
    return MovementAction_WalkFast_UpLeft_Up_Step1(objectEvent, sprite);
}

bool8 (*const sMovementActionFuncs_WalkFast_UpLeft_FaceUp[])(struct EventObject *, struct Sprite *) = {
    MovementAction_WalkFast_UpLeft_Up_Step0,
    MovementAction_WalkFast_UpLeft_Up_Step1,
    MovementAction_PauseSpriteAnim
};

static bool8 MovementAction_WalkFast_DownRight_Down_Step1(struct EventObject *objectEvent, struct Sprite *sprite)
{
    if (UpdateMovementNormal(objectEvent, sprite))
    {
        sprite->data[2] = 2;
        return TRUE;
    }
    return FALSE;
}

static bool8 MovementAction_WalkFast_DownRight_Down_Step0(struct EventObject *objectEvent, struct Sprite *sprite)
{
    StartNormalFastAnim3(objectEvent, sprite, DIR_SOUTHEAST);
    return MovementAction_WalkFast_DownRight_Down_Step1(objectEvent, sprite);
}

bool8 (*const sMovementActionFuncs_WalkFast_DownRight_FaceDown[])(struct EventObject *, struct Sprite *) = {
    MovementAction_WalkFast_DownRight_Down_Step0,
    MovementAction_WalkFast_DownRight_Down_Step1,
    MovementAction_PauseSpriteAnim
};

static bool8 MovementAction_WalkFast_UpRight_Up_Step1(struct EventObject *objectEvent, struct Sprite *sprite)
{
    if (UpdateMovementNormal(objectEvent, sprite))
    {
        sprite->data[2] = 2;
        return TRUE;
    }
    return FALSE;
}

static bool8 MovementAction_WalkFast_UpRight_Up_Step0(struct EventObject *objectEvent, struct Sprite *sprite)
{
    StartNormalFastAnim3(objectEvent, sprite, DIR_NORTHEAST);
    return MovementAction_WalkFast_UpRight_Up_Step1(objectEvent, sprite);
}

bool8 (*const sMovementActionFuncs_WalkFast_UpRight_FaceUp[])(struct EventObject *, struct Sprite *) = {
    MovementAction_WalkFast_UpRight_Up_Step0,
    MovementAction_WalkFast_UpRight_Up_Step1,
    MovementAction_PauseSpriteAnim
};

const u8 gInitialMovementTypeFacingDirections2[MOVEMENT_TYPES_COUNT] = {
    [MOVEMENT_TYPE_NONE] = DIR_SOUTH,
    [MOVEMENT_TYPE_LOOK_AROUND] = DIR_SOUTH,
    [MOVEMENT_TYPE_WANDER_AROUND] = DIR_SOUTH,
    [MOVEMENT_TYPE_WANDER_UP_AND_DOWN] = DIR_NORTH,
    [MOVEMENT_TYPE_WANDER_DOWN_AND_UP] = DIR_SOUTH,
    [MOVEMENT_TYPE_WANDER_LEFT_AND_RIGHT] = DIR_WEST,
    [MOVEMENT_TYPE_WANDER_RIGHT_AND_LEFT] = DIR_EAST,
    [MOVEMENT_TYPE_FACE_UP] = DIR_NORTH,
    [MOVEMENT_TYPE_FACE_DOWN] = DIR_SOUTH,
    [MOVEMENT_TYPE_FACE_LEFT] = DIR_WEST,
    [MOVEMENT_TYPE_FACE_RIGHT] = DIR_EAST,
    [MOVEMENT_TYPE_PLAYER] = DIR_SOUTH,
    [MOVEMENT_TYPE_BERRY_TREE_GROWTH] = DIR_SOUTH,
    [MOVEMENT_TYPE_FACE_DOWN_AND_UP] = DIR_SOUTH,
    [MOVEMENT_TYPE_FACE_LEFT_AND_RIGHT] = DIR_WEST,
    [MOVEMENT_TYPE_FACE_UP_AND_LEFT] = DIR_NORTH,
    [MOVEMENT_TYPE_FACE_UP_AND_RIGHT] = DIR_NORTH,
    [MOVEMENT_TYPE_FACE_DOWN_AND_LEFT] = DIR_SOUTH,
    [MOVEMENT_TYPE_FACE_DOWN_AND_RIGHT] = DIR_SOUTH,
    [MOVEMENT_TYPE_FACE_DOWN_UP_AND_LEFT] = DIR_SOUTH,
    [MOVEMENT_TYPE_FACE_DOWN_UP_AND_RIGHT] = DIR_SOUTH,
    [MOVEMENT_TYPE_FACE_UP_LEFT_AND_RIGHT] = DIR_NORTH,
    [MOVEMENT_TYPE_FACE_DOWN_LEFT_AND_RIGHT] = DIR_SOUTH,
    [MOVEMENT_TYPE_ROTATE_COUNTERCLOCKWISE] = DIR_SOUTH,
    [MOVEMENT_TYPE_ROTATE_CLOCKWISE] = DIR_SOUTH,
    [MOVEMENT_TYPE_WALK_UP_AND_DOWN] = DIR_NORTH,
    [MOVEMENT_TYPE_WALK_DOWN_AND_UP] = DIR_SOUTH,
    [MOVEMENT_TYPE_WALK_LEFT_AND_RIGHT] = DIR_WEST,
    [MOVEMENT_TYPE_WALK_RIGHT_AND_LEFT] = DIR_EAST,
    [MOVEMENT_TYPE_WALK_SEQUENCE_UP_RIGHT_LEFT_DOWN] = DIR_NORTH,
    [MOVEMENT_TYPE_WALK_SEQUENCE_RIGHT_LEFT_DOWN_UP] = DIR_EAST,
    [MOVEMENT_TYPE_WALK_SEQUENCE_DOWN_UP_RIGHT_LEFT] = DIR_SOUTH,
    [MOVEMENT_TYPE_WALK_SEQUENCE_LEFT_DOWN_UP_RIGHT] = DIR_WEST,
    [MOVEMENT_TYPE_WALK_SEQUENCE_UP_LEFT_RIGHT_DOWN] = DIR_NORTH,
    [MOVEMENT_TYPE_WALK_SEQUENCE_LEFT_RIGHT_DOWN_UP] = DIR_WEST,
    [MOVEMENT_TYPE_WALK_SEQUENCE_DOWN_UP_LEFT_RIGHT] = DIR_SOUTH,
    [MOVEMENT_TYPE_WALK_SEQUENCE_RIGHT_DOWN_UP_LEFT] = DIR_EAST,
    [MOVEMENT_TYPE_WALK_SEQUENCE_LEFT_UP_DOWN_RIGHT] = DIR_WEST,
    [MOVEMENT_TYPE_WALK_SEQUENCE_UP_DOWN_RIGHT_LEFT] = DIR_NORTH,
    [MOVEMENT_TYPE_WALK_SEQUENCE_RIGHT_LEFT_UP_DOWN] = DIR_EAST,
    [MOVEMENT_TYPE_WALK_SEQUENCE_DOWN_RIGHT_LEFT_UP] = DIR_SOUTH,
    [MOVEMENT_TYPE_WALK_SEQUENCE_RIGHT_UP_DOWN_LEFT] = DIR_EAST,
    [MOVEMENT_TYPE_WALK_SEQUENCE_UP_DOWN_LEFT_RIGHT] = DIR_NORTH,
    [MOVEMENT_TYPE_WALK_SEQUENCE_LEFT_RIGHT_UP_DOWN] = DIR_WEST,
    [MOVEMENT_TYPE_WALK_SEQUENCE_DOWN_LEFT_RIGHT_UP] = DIR_SOUTH,
    [MOVEMENT_TYPE_WALK_SEQUENCE_UP_LEFT_DOWN_RIGHT] = DIR_NORTH,
    [MOVEMENT_TYPE_WALK_SEQUENCE_DOWN_RIGHT_UP_LEFT] = DIR_SOUTH,
    [MOVEMENT_TYPE_WALK_SEQUENCE_LEFT_DOWN_RIGHT_UP] = DIR_WEST,
    [MOVEMENT_TYPE_WALK_SEQUENCE_RIGHT_UP_LEFT_DOWN] = DIR_EAST,
    [MOVEMENT_TYPE_WALK_SEQUENCE_UP_RIGHT_DOWN_LEFT] = DIR_NORTH,
    [MOVEMENT_TYPE_WALK_SEQUENCE_DOWN_LEFT_UP_RIGHT] = DIR_SOUTH,
    [MOVEMENT_TYPE_WALK_SEQUENCE_LEFT_UP_RIGHT_DOWN] = DIR_WEST,
    [MOVEMENT_TYPE_WALK_SEQUENCE_RIGHT_DOWN_LEFT_UP] = DIR_EAST,
    [MOVEMENT_TYPE_COPY_PLAYER] = DIR_NORTH,
    [MOVEMENT_TYPE_COPY_PLAYER_OPPOSITE] = DIR_SOUTH,
    [MOVEMENT_TYPE_COPY_PLAYER_COUNTERCLOCKWISE] = DIR_WEST,
    [MOVEMENT_TYPE_COPY_PLAYER_CLOCKWISE] = DIR_EAST,
    [MOVEMENT_TYPE_TREE_DISGUISE] = DIR_SOUTH,
    [MOVEMENT_TYPE_MOUNTAIN_DISGUISE] = DIR_SOUTH,
    [MOVEMENT_TYPE_COPY_PLAYER_IN_GRASS] = DIR_NORTH,
    [MOVEMENT_TYPE_COPY_PLAYER_OPPOSITE_IN_GRASS] = DIR_SOUTH,
    [MOVEMENT_TYPE_COPY_PLAYER_COUNTERCLOCKWISE_IN_GRASS] = DIR_WEST,
    [MOVEMENT_TYPE_COPY_PLAYER_CLOCKWISE_IN_GRASS] = DIR_EAST,
    [MOVEMENT_TYPE_HIDDEN] = DIR_SOUTH,
    [MOVEMENT_TYPE_WALK_IN_PLACE_DOWN] = DIR_SOUTH,
    [MOVEMENT_TYPE_WALK_IN_PLACE_UP] = DIR_NORTH,
    [MOVEMENT_TYPE_WALK_IN_PLACE_LEFT] = DIR_WEST,
    [MOVEMENT_TYPE_WALK_IN_PLACE_RIGHT] = DIR_EAST,
    [MOVEMENT_TYPE_JOG_IN_PLACE_DOWN] = DIR_SOUTH,
    [MOVEMENT_TYPE_JOG_IN_PLACE_UP] = DIR_NORTH,
    [MOVEMENT_TYPE_JOG_IN_PLACE_LEFT] = DIR_WEST,
    [MOVEMENT_TYPE_JOG_IN_PLACE_RIGHT] = DIR_EAST,
    [MOVEMENT_TYPE_RUN_IN_PLACE_DOWN] = DIR_SOUTH,
    [MOVEMENT_TYPE_RUN_IN_PLACE_UP] = DIR_NORTH,
    [MOVEMENT_TYPE_RUN_IN_PLACE_LEFT] = DIR_WEST,
    [MOVEMENT_TYPE_RUN_IN_PLACE_RIGHT] = DIR_EAST,
    [MOVEMENT_TYPE_INVISIBLE] = DIR_SOUTH,
    [MOVEMENT_TYPE_WALK_SLOWLY_IN_PLACE_DOWN] = DIR_SOUTH,
    [MOVEMENT_TYPE_WALK_SLOWLY_IN_PLACE_UP] = DIR_SOUTH,
    [MOVEMENT_TYPE_WALK_SLOWLY_IN_PLACE_LEFT] = DIR_SOUTH,
    [MOVEMENT_TYPE_WALK_SLOWLY_IN_PLACE_RIGHT] = DIR_SOUTH,
    [MOVEMENT_TYPE_FOLLOWING_POKEMON] = DIR_SOUTH,
};

u8 GetEventObjectIdByLocalIdAndMap(u8 localId, u8 mapNum, u8 mapGroupId)
{
    if (localId == OBJ_EVENT_ID_FOLLOWER)
    {
        return GetFollowerObjectId();
    }
    else if (localId < EVENT_OBJ_ID_PLAYER)
    {
        return GetEventObjectIdByLocalIdAndMapInternal(localId, mapNum, mapGroupId);
    }
    return GetEventObjectIdByLocalId(localId);
}

u8 DoJumpAnimStep(struct EventObject *objectEvent, struct Sprite *sprite)
{
    return UpdateJumpAnim(objectEvent, sprite, DoJumpSpriteMovement);
}

void RemoveEventObjectsOutsideView(void)
{
    u8 i, j;
    bool8 isActiveLinkPlayer;

    for (i = 0; i < EVENT_OBJECTS_COUNT; i++)
    {
        for (j = 0, isActiveLinkPlayer = FALSE; j < NELEMS(gLinkPlayerEventObjects); j++)
        {
            if (gLinkPlayerEventObjects[j].active && i == gLinkPlayerEventObjects[j].eventObjId)
                isActiveLinkPlayer = TRUE;
        }
        if (!isActiveLinkPlayer)
        {
            struct EventObject *objectEvent = &gEventObjects[i];

            if (objectEvent->active && !objectEvent->isPlayer && i != GetFollowerObjectId())
                RemoveEventObjectIfOutsideView(objectEvent);
        }
    }
}

movement_type_def(MovementType_FollowingPokemon, gMovementTypeFuncs_FollowingPokemon)

bool8 MovementType_FollowingPokemon_Step0(struct EventObject *objectEvent, struct Sprite *sprite)
{
    ClearEventObjectMovement(objectEvent, sprite);
    sprite->data[1] = 1;
    return TRUE;
}

bool8 MovementType_FollowingPokemon_Step1(struct EventObject *objectEvent, struct Sprite *sprite)
{
    EventObjectSetSingleMovement(objectEvent, sprite, MOVEMENT_ACTION_FOLLOWING_POKEMON);
    sprite->data[1] = 2;
    return TRUE;
}

bool8 MovementType_FollowingPokemon_Step2(struct EventObject *objectEvent, struct Sprite *sprite)
{
    if (EventObjectExecSingleMovementAction(objectEvent, sprite))
    {
        objectEvent->singleMovementActive = FALSE;
        sprite->data[1] = 3;
    }
    return FALSE;
}

bool8 MovementType_FollowingPokemon_Step3(struct EventObject *objectEvent __attribute__((unused)), struct Sprite *sprite)
{
    sprite->data[1] = 1;
    return TRUE;
}

bool8 UpdateMovementNormal(struct EventObject *objectEvent, struct Sprite *sprite)
{
    if (NpcTakeStep(sprite))
    {
        ShiftStillEventObjectCoords(objectEvent);
        objectEvent->triggerGroundEffectsOnStop = TRUE;
        if (!(gFollowerState.inProgress && objectEvent == &gEventObjects[gFollowerState.objId]))
            sprite->animPaused = TRUE;
        return TRUE;
    }
    return FALSE;
}

bool8 MovementAction_WalkNormalDown_Step1(struct EventObject *objectEvent, struct Sprite *sprite)
{
    if (UpdateMovementNormal(objectEvent, sprite))
    {
        sprite->data[2] = 2;
        return TRUE;
    }
    return FALSE;
}

bool8 MovementAction_WalkNormalDown_Step0(struct EventObject *objectEvent, struct Sprite *sprite)
{
    if(gFollowerState.inProgress && objectEvent == &gEventObjects[gFollowerState.objId])
    {
		sprite->pos2.x = 0;
	}

    InitMovementNormal(objectEvent, sprite, DIR_SOUTH, MOVE_SPEED_NORMAL);
    return MovementAction_WalkNormalDown_Step1(objectEvent, sprite);
}

bool8 (*const sMovementActionFuncs_WalkNormalDown[])(struct EventObject *, struct Sprite *) = {
    MovementAction_WalkNormalDown_Step0,
    MovementAction_WalkNormalDown_Step1,
    MovementAction_PauseSpriteAnim,
};

static bool8 MovementAction_WalkNormalUp_Step1(struct EventObject *objectEvent, struct Sprite *sprite)
{
    if (UpdateMovementNormal(objectEvent, sprite))
    {
        sprite->data[2] = 2;
        return TRUE;
    }
    return FALSE;
}

bool8 MovementAction_WalkNormalUp_Step0(struct EventObject *objectEvent, struct Sprite *sprite)
{
    if(gFollowerState.inProgress && objectEvent == &gEventObjects[gFollowerState.objId])
    {
		sprite->pos2.x = 0;
	}

    InitMovementNormal(objectEvent, sprite, DIR_NORTH, MOVE_SPEED_NORMAL);
    return MovementAction_WalkNormalUp_Step1(objectEvent, sprite);
}

bool8 (*const sMovementActionFuncs_WalkNormalUp[])(struct EventObject *, struct Sprite *) = {
    MovementAction_WalkNormalUp_Step0,
    MovementAction_WalkNormalUp_Step1,
    MovementAction_PauseSpriteAnim,
};

static bool8 MovementAction_WalkNormalLeft_Step1(struct EventObject *objectEvent, struct Sprite *sprite)
{
    if (UpdateMovementNormal(objectEvent, sprite))
    {
        sprite->data[2] = 2;
        return TRUE;
    }
    return FALSE;
}

bool8 MovementAction_WalkNormalLeft_Step0(struct EventObject *objectEvent, struct Sprite *sprite)
{
    if(gFollowerState.inProgress && objectEvent == &gEventObjects[gFollowerState.objId])
    {
		sprite->pos2.x = 0;
	}

    InitMovementNormal(objectEvent, sprite, DIR_WEST, MOVE_SPEED_NORMAL);
    return MovementAction_WalkNormalLeft_Step1(objectEvent, sprite);
}

bool8 (*const sMovementActionFuncs_WalkNormalLeft[])(struct EventObject *, struct Sprite *) = {
    MovementAction_WalkNormalLeft_Step0,
    MovementAction_WalkNormalLeft_Step1,
    MovementAction_PauseSpriteAnim,
};

static bool8 MovementAction_WalkNormalRight_Step1(struct EventObject *objectEvent, struct Sprite *sprite)
{
    if (UpdateMovementNormal(objectEvent, sprite))
    {
        sprite->data[2] = 2;
        return TRUE;
    }
    return FALSE;
}

bool8 MovementAction_WalkNormalRight_Step0(struct EventObject *objectEvent, struct Sprite *sprite)
{
    if(gFollowerState.inProgress && objectEvent == &gEventObjects[gFollowerState.objId])
    {
		sprite->pos2.x = 0;
	}

    InitMovementNormal(objectEvent, sprite, DIR_EAST, MOVE_SPEED_NORMAL);
    return MovementAction_WalkNormalRight_Step1(objectEvent, sprite);
}

bool8 (*const sMovementActionFuncs_WalkNormalRight[])(struct EventObject *, struct Sprite *) = {
    MovementAction_WalkNormalRight_Step0,
    MovementAction_WalkNormalRight_Step1,
    MovementAction_PauseSpriteAnim,
};

static bool8 MovementAction_Jump2Down_Step1(struct EventObject *objectEvent, struct Sprite *sprite)
{
    if (DoJumpAnim(objectEvent, sprite))
    {
        objectEvent->hasShadow = FALSE;
        sprite->data[2] = 2;
        return TRUE;
    }
    return FALSE;
}

static bool8 MovementAction_Jump2Down_Step0(struct EventObject *objectEvent, struct Sprite *sprite)
{
    if(gFollowerState.inProgress && objectEvent == &gEventObjects[gFollowerState.objId])
    {
		sprite->pos2.x = 0;
	}

    InitJumpRegular(objectEvent, sprite, DIR_SOUTH, JUMP_DISTANCE_FAR, JUMP_TYPE_HIGH);
    return MovementAction_Jump2Down_Step1(objectEvent, sprite);
}

bool8 (*const sMovementActionFuncs_Jump2Down[])(struct EventObject *, struct Sprite *) = {
    MovementAction_Jump2Down_Step0,
    MovementAction_Jump2Down_Step1,
    MovementAction_PauseSpriteAnim,
};

static bool8 MovementAction_Jump2Up_Step1(struct EventObject *objectEvent, struct Sprite *sprite)
{
    if (DoJumpAnim(objectEvent, sprite))
    {
        objectEvent->hasShadow = FALSE;
        sprite->data[2] = 2;
        return TRUE;
    }
    return FALSE;
}

static bool8 MovementAction_Jump2Up_Step0(struct EventObject *objectEvent, struct Sprite *sprite)
{
    if(gFollowerState.inProgress && objectEvent == &gEventObjects[gFollowerState.objId])
    {
        sprite->pos2.x = 0;
    }

    InitJumpRegular(objectEvent, sprite, DIR_NORTH, JUMP_DISTANCE_FAR, JUMP_TYPE_HIGH);
    return MovementAction_Jump2Up_Step1(objectEvent, sprite);
}

bool8 (*const sMovementActionFuncs_Jump2Up[])(struct EventObject *, struct Sprite *) = {
    MovementAction_Jump2Up_Step0,
    MovementAction_Jump2Up_Step1,
    MovementAction_PauseSpriteAnim,
};

static bool8 MovementAction_Jump2Left_Step1(struct EventObject *objectEvent, struct Sprite *sprite)
{
    if (DoJumpAnim(objectEvent, sprite))
    {
        objectEvent->hasShadow = FALSE;
        sprite->data[2] = 2;
        return TRUE;
    }
    return FALSE;
}

static bool8 MovementAction_Jump2Left_Step0(struct EventObject *objectEvent, struct Sprite *sprite)
{
    if(gFollowerState.inProgress && objectEvent == &gEventObjects[gFollowerState.objId])
    {
        sprite->pos2.x = 0;
    }
    InitJumpRegular(objectEvent, sprite, DIR_WEST, JUMP_DISTANCE_FAR, JUMP_TYPE_HIGH);
    return MovementAction_Jump2Left_Step1(objectEvent, sprite);
}

bool8 (*const sMovementActionFuncs_Jump2Left[])(struct EventObject *, struct Sprite *) = {
    MovementAction_Jump2Left_Step0,
    MovementAction_Jump2Left_Step1,
    MovementAction_PauseSpriteAnim,
};

static bool8 MovementAction_Jump2Right_Step1(struct EventObject *objectEvent, struct Sprite *sprite)
{
    if (DoJumpAnim(objectEvent, sprite))
    {
        objectEvent->hasShadow = FALSE;
        sprite->data[2] = 2;
        return TRUE;
    }
    return FALSE;
}

static bool8 MovementAction_Jump2Right_Step0(struct EventObject *objectEvent, struct Sprite *sprite)
{
    if(gFollowerState.inProgress && objectEvent == &gEventObjects[gFollowerState.objId])
    {
        sprite->pos2.x = 0;
    }

    InitJumpRegular(objectEvent, sprite, DIR_EAST, JUMP_DISTANCE_FAR, JUMP_TYPE_HIGH);
    return MovementAction_Jump2Right_Step1(objectEvent, sprite);
}

bool8 (*const sMovementActionFuncs_Jump2Right[])(struct EventObject *, struct Sprite *) = {
    MovementAction_Jump2Right_Step0,
    MovementAction_Jump2Right_Step1,
    MovementAction_PauseSpriteAnim,
};

static bool8 MovementAction_PlayerRunDown_Step1(struct EventObject *eventObject, struct Sprite *sprite)
{
    if (UpdateMovementNormal(eventObject, sprite))
    {
        sprite->data[2] = 2;
        return TRUE;
    }
    return FALSE;
}

static bool8 MovementAction_PlayerRunDown_Step0(struct EventObject *eventObject, struct Sprite *sprite)
{
    if(gFollowerState.inProgress && eventObject == &gEventObjects[gFollowerState.objId])
    {
        sprite->pos2.x = 0;
    }

    OldStartRunningAnim(eventObject, sprite, DIR_SOUTH);
    return MovementAction_PlayerRunDown_Step1(eventObject, sprite);
}

bool8 (*const sMovementActionFuncs_PlayerRunDown[])(struct EventObject *, struct Sprite *) = {
    MovementAction_PlayerRunDown_Step0,
    MovementAction_PlayerRunDown_Step1,
    MovementAction_PauseSpriteAnim,
};

static bool8 MovementAction_PlayerRunUp_Step1(struct EventObject *eventObject, struct Sprite *sprite)
{
    if (UpdateMovementNormal(eventObject, sprite))
    {
        sprite->data[2] = 2;
        return TRUE;
    }
    return FALSE;
}

static bool8 MovementAction_PlayerRunUp_Step0(struct EventObject *eventObject, struct Sprite *sprite)
{
    if(gFollowerState.inProgress && eventObject == &gEventObjects[gFollowerState.objId])
    {
        sprite->pos2.x = 0;
    }

    OldStartRunningAnim(eventObject, sprite, DIR_NORTH);
    return MovementAction_PlayerRunUp_Step1(eventObject, sprite);
}

bool8 (*const sMovementActionFuncs_PlayerRunUp[])(struct EventObject *, struct Sprite *) = {
    MovementAction_PlayerRunUp_Step0,
    MovementAction_PlayerRunUp_Step1,
    MovementAction_PauseSpriteAnim,
};

static bool8 MovementAction_PlayerRunLeft_Step1(struct EventObject *eventObject, struct Sprite *sprite)
{
    if (UpdateMovementNormal(eventObject, sprite))
    {
        sprite->data[2] = 2;
        return TRUE;
    }
    return FALSE;
}

static bool8 MovementAction_PlayerRunLeft_Step0(struct EventObject *eventObject, struct Sprite *sprite)
{
    if(gFollowerState.inProgress && eventObject == &gEventObjects[gFollowerState.objId])
    {
        sprite->pos2.x = 0;
    }

    OldStartRunningAnim(eventObject, sprite, DIR_WEST);
    return MovementAction_PlayerRunLeft_Step1(eventObject, sprite);
}

bool8 (*const sMovementActionFuncs_PlayerRunLeft[])(struct EventObject *, struct Sprite *) = {
    MovementAction_PlayerRunLeft_Step0,
    MovementAction_PlayerRunLeft_Step1,
    MovementAction_PauseSpriteAnim,
};

static bool8 MovementAction_PlayerRunRight_Step1(struct EventObject *eventObject, struct Sprite *sprite)
{
    if (UpdateMovementNormal(eventObject, sprite))
    {
        sprite->data[2] = 2;
        return TRUE;
    }
    return FALSE;
}

static bool8 MovementAction_PlayerRunRight_Step0(struct EventObject *eventObject, struct Sprite *sprite)
{
    if(gFollowerState.inProgress && eventObject == &gEventObjects[gFollowerState.objId])
    {
        sprite->pos2.x = 0;
    }

    OldStartRunningAnim(eventObject, sprite, DIR_EAST);
    return MovementAction_PlayerRunRight_Step1(eventObject, sprite);
}

bool8 (*const sMovementActionFuncs_PlayerRunRight[])(struct EventObject *, struct Sprite *) = {
    MovementAction_PlayerRunRight_Step0,
    MovementAction_PlayerRunRight_Step1,
    MovementAction_PauseSpriteAnim,
};

u8 MovementAction_FollowingPokemon_Step0(struct EventObject *objectEvent __attribute__((unused)), struct Sprite *sprite)
{    
    if(sprite->animNum > 20)
		sprite->animNum -= 45;
        
    switch(sprite->animNum % 4)
    {
        case 0:
        case 1:
            sprite->pos2.x = 0;
            break;
        case 2:
            sprite->pos2.x = 0;
            break;
        case 3:
            sprite->pos2.x = 0;
            break;
    }

    SeekSpriteAnim(sprite, 0);
	SetMovementDelay(sprite, 15);
	sprite->data[2]++;
	return FALSE;
}

u8 MovementAction_FollowingPokemon_Step1(struct EventObject *objectEvent __attribute__((unused)), struct Sprite *sprite)
{
	if(WaitForMovementDelay(sprite))
	{
		SeekSpriteAnim(sprite, 1);
		sprite->data[2]++;
		SetMovementDelay(sprite, 15);
	}
	return FALSE;
}

u8 MovementAction_FollowingPokemon_Step2(struct EventObject *objectEvent __attribute__((unused)), struct Sprite *sprite)
{
	if(WaitForMovementDelay(sprite))
	{
		SeekSpriteAnim(sprite, 2);
		sprite->data[2]++;
		SetMovementDelay(sprite, 15);
	}
	return FALSE;
}

u8 MovementAction_FollowingPokemon_Step3(struct EventObject *objectEvent __attribute__((unused)), struct Sprite *sprite)
{
	if(WaitForMovementDelay(sprite))
	{
		SeekSpriteAnim(sprite, 3);
		sprite->data[2]++;
		SetMovementDelay(sprite, 15);
	}
	return FALSE;
}

u8 MovementAction_FollowingPokemon_Step4(struct EventObject *objectEvent __attribute__((unused)), struct Sprite *sprite)
{
	if(WaitForMovementDelay(sprite))
		sprite->data[2]++;

	return FALSE;
}

void RecreateObjectEvent(struct EventObject *objectEvent, struct Sprite *sprite)
{
    u8 newSpriteId;
    struct EventObjectTemplate clone;
    struct EventObject backupFollower = *objectEvent;
    backupFollower.graphicsId = objectEvent->graphicsId;
    DestroySprite(sprite);
    RemoveEventObject(objectEvent);

    clone = *GetEventObjectTemplateByLocalIdAndMap(objectEvent->localId, objectEvent->mapNum, objectEvent->mapGroup);
    clone.graphicsId = objectEvent->graphicsId;

    objectEvent = &gObjectEvents[TrySpawnEventObjectTemplate(&clone, gSaveBlock1->location.mapNum, gSaveBlock1->location.mapGroup, clone.x, clone.y)];
    newSpriteId = objectEvent->spriteId;
    *objectEvent = backupFollower;
    objectEvent->spriteId = newSpriteId;
}

u8 MovementAction_FollowingPokemon_FaceSouth_Step0(struct EventObject *objectEvent, struct Sprite *sprite)
{
    RecreateObjectEvent(objectEvent, sprite);
    sprite->animNum = 0;
    MoveEventObjectToMapCoords(objectEvent, objectEvent->currentCoords.x, objectEvent->currentCoords.y);
    sprite->data[2]++;
    return FALSE;
}

u8 MovementAction_FollowingPokemon_FaceNorth_Step0(struct EventObject *objectEvent, struct Sprite *sprite)
{
    RecreateObjectEvent(objectEvent, sprite);
    sprite->animNum = 1;
    MoveEventObjectToMapCoords(objectEvent, objectEvent->currentCoords.x, objectEvent->currentCoords.y);
    sprite->data[2]++;
    return FALSE;
}

u8 MovementAction_FollowingPokemon_FaceWest_Step0(struct EventObject *objectEvent, struct Sprite *sprite)
{
    RecreateObjectEvent(objectEvent, sprite);
    sprite->animNum = 2;
    sprite->pos2.x = 0;
    MoveEventObjectToMapCoords(objectEvent, objectEvent->currentCoords.x, objectEvent->currentCoords.y);
    sprite->data[2]++;
    return FALSE;
}

u8 MovementAction_FollowingPokemon_FaceEast_Step0(struct EventObject *objectEvent, struct Sprite *sprite)
{
    RecreateObjectEvent(objectEvent, sprite);
    sprite->animNum = 3;
    sprite->pos2.x = 0;
    MoveEventObjectToMapCoords(objectEvent, objectEvent->currentCoords.x, objectEvent->currentCoords.y);
    sprite->data[2]++;
    return FALSE;
}

u8 (*const gMovementActionFuncs_FollowingPokemon[])(struct EventObject *, struct Sprite *) = {
    MovementAction_FollowingPokemon_Step0,
    MovementAction_FollowingPokemon_Step1,
	MovementAction_FollowingPokemon_Step2,
	MovementAction_FollowingPokemon_Step3,
	MovementAction_FollowingPokemon_Step4,
    MovementAction_Finish,
};

u8 (*const gMovementActionFuncs_FollowingPokemon_FaceSouth[])(struct EventObject *, struct Sprite *) = {
    MovementAction_FollowingPokemon_FaceSouth_Step0,
    MovementAction_Finish,
};

u8 (*const gMovementActionFuncs_FollowingPokemon_FaceNorth[])(struct EventObject *, struct Sprite *) = {
    MovementAction_FollowingPokemon_FaceNorth_Step0,
    MovementAction_Finish,
};

u8 (*const gMovementActionFuncs_FollowingPokemon_FaceWest[])(struct EventObject *, struct Sprite *) = {
    MovementAction_FollowingPokemon_FaceWest_Step0,
    MovementAction_Finish,
};

u8 (*const gMovementActionFuncs_FollowingPokemon_FaceEast[])(struct EventObject *, struct Sprite *) = {
    MovementAction_FollowingPokemon_FaceEast_Step0,
    MovementAction_Finish,
};

// NEW
u16 GetMiniStepCount(u8 speed)
{
    return (u16)sStepTimes[speed];
}

void RunMiniStep(struct Sprite *sprite, u8 speed, u8 currentFrame)
{
    sNpcStepFuncTables[speed][currentFrame](sprite, sprite->data[3]);
}

bool8 (*const gMovementTypeFuncs_FollowingPokemon[])(struct EventObject *, struct Sprite *) = {
    (bool8 (*)(struct EventObject *, struct Sprite *))MovementType_FollowingPokemon_Step0,
    (bool8 (*)(struct EventObject *, struct Sprite *))MovementType_FollowingPokemon_Step1,
    (bool8 (*)(struct EventObject *, struct Sprite *))MovementType_FollowingPokemon_Step2,
    (bool8 (*)(struct EventObject *, struct Sprite *))MovementType_FollowingPokemon_Step3,
};

const u8 *GetInteractedEventObjectScript(struct MapPosition *position, u8 metatileBehavior, u8 direction)
{
    u8 EventObjectId;
    const u8 *script;

    EventObjectId = GetEventObjectIdByPosition(position->x, position->y, position->height);
    if (EventObjectId == EVENT_OBJECTS_COUNT || gEventObjects[EventObjectId].localId == EVENT_OBJ_ID_PLAYER)
    {
        if (MetatileBehavior_IsCounter(metatileBehavior) != TRUE)
            return NULL;

        // Look for an object event on the other side of the counter.
        EventObjectId = GetEventObjectIdByPosition(position->x + gDirectionToVectors[direction].x, position->y + gDirectionToVectors[direction].y, position->height);
        if (EventObjectId == EVENT_OBJECTS_COUNT || gEventObjects[EventObjectId].localId == EVENT_OBJ_ID_PLAYER)
            return NULL;
    }

    if (InUnionRoom() == TRUE && !EventObjectCheckHeldMovementStatus(&gEventObjects[EventObjectId]))
        return NULL;

    gSelectedEventObject = EventObjectId;
    gSpecialVar_LastTalked = gEventObjects[EventObjectId].localId;
    gSpecialVar_PlayerFacing = direction;

    if (EventObjectId == GetFollowerObjectId())//(gObjectEvents[EventObjectId].localId == OBJ_EVENT_ID_FOLLOWER)
	{
		if(gObjectEvents[EventObjectId].invisible == FALSE)
			script = GetFollowerScriptPointer();
		else
			return NULL;
	}
    else
        script = GetEventObjectScriptPointerByEventObjectId(EventObjectId);

    script = GetRamScript(gSpecialVar_LastTalked, script);
    return script;
}

//////////////////////////////////////////////////////////////////////////////////////////////////

struct MovementTable
{
    bool8 (*const *funcs)(struct EventObject *, struct Sprite *);
};

const struct MovementTable sMovementActionFuncs[] =
{
    {sMovementActionFuncs_FaceDown},
    {sMovementActionFuncs_FaceUp},
    {sMovementActionFuncs_FaceLeft},
    {sMovementActionFuncs_FaceRight},
    {sMovementActionFuncs_FaceDownFast},
    {sMovementActionFuncs_FaceUpFast},
    {sMovementActionFuncs_FaceLeftFast},
    {sMovementActionFuncs_FaceRightFast},
    {sMovementActionFuncs_WalkSlowerDown},
    {sMovementActionFuncs_WalkSlowerUp},
    {sMovementActionFuncs_WalkSlowerLeft},
    {sMovementActionFuncs_WalkSlowerRight},
    {sMovementActionFuncs_WalkSlowDown},
    {sMovementActionFuncs_WalkSlowUp},
    {sMovementActionFuncs_WalkSlowLeft},
    {sMovementActionFuncs_WalkSlowRight},
    {sMovementActionFuncs_WalkNormalDown},
    {sMovementActionFuncs_WalkNormalUp},
    {sMovementActionFuncs_WalkNormalLeft},
    {sMovementActionFuncs_WalkNormalRight},
    {sMovementActionFuncs_Jump2Down},
    {sMovementActionFuncs_Jump2Up},
    {sMovementActionFuncs_Jump2Left},
    {sMovementActionFuncs_Jump2Right},
    {sMovementActionFuncs_Delay1},
    {sMovementActionFuncs_Delay2},
    {sMovementActionFuncs_Delay4},
    {sMovementActionFuncs_Delay8},
    {sMovementActionFuncs_Delay16},
    {sMovementActionFuncs_WalkFastDown},
    {sMovementActionFuncs_WalkFastUp},
    {sMovementActionFuncs_WalkFastLeft},
    {sMovementActionFuncs_WalkFastRight},
    {sMovementActionFuncs_WalkInPlaceSlowDown},
    {sMovementActionFuncs_WalkInPlaceSlowUp},
    {sMovementActionFuncs_WalkInPlaceSlowLeft},
    {sMovementActionFuncs_WalkInPlaceSlowRight},
    {sMovementActionFuncs_WalkInPlaceNormalDown},
    {sMovementActionFuncs_WalkInPlaceNormalUp},
    {sMovementActionFuncs_WalkInPlaceNormalLeft},
    {sMovementActionFuncs_WalkInPlaceNormalRight},
    {sMovementActionFuncs_WalkInPlaceFastDown},
    {sMovementActionFuncs_WalkInPlaceFastUp},
    {sMovementActionFuncs_WalkInPlaceFastLeft},
    {sMovementActionFuncs_WalkInPlaceFastRight},
    {sMovementActionFuncs_WalkInPlaceFasterDown},
    {sMovementActionFuncs_WalkInPlaceFasterUp},
    {sMovementActionFuncs_WalkInPlaceFasterLeft},
    {sMovementActionFuncs_WalkInPlaceFasterRight},
    {sMovementActionFuncs_RideWaterCurrentDown},
    {sMovementActionFuncs_RideWaterCurrentUp},
    {sMovementActionFuncs_RideWaterCurrentLeft},
    {sMovementActionFuncs_RideWaterCurrentRight},
    {sMovementActionFuncs_WalkFasterDown},
    {sMovementActionFuncs_WalkFasterUp},
    {sMovementActionFuncs_WalkFasterLeft},
    {sMovementActionFuncs_WalkFasterRight},
    {sMovementActionFuncs_SlideDown},
    {sMovementActionFuncs_SlideUp},
    {sMovementActionFuncs_SlideLeft},
    {sMovementActionFuncs_SlideRight},
    {sMovementActionFuncs_PlayerRunDown},
    {sMovementActionFuncs_PlayerRunUp},
    {sMovementActionFuncs_PlayerRunLeft},
    {sMovementActionFuncs_PlayerRunRight},
    {sMovementActionFuncs_PlayerRunDownSlow},
    {sMovementActionFuncs_PlayerRunUpSlow},
    {sMovementActionFuncs_PlayerRunLeftSlow},
    {sMovementActionFuncs_PlayerRunRightSlow},
    {sMovementActionFuncs_StartAnimInDirection},
    {sMovementActionFuncs_JumpSpecialDown},
    {sMovementActionFuncs_JumpSpecialUp},
    {sMovementActionFuncs_JumpSpecialLeft},
    {sMovementActionFuncs_JumpSpecialRight},
    {sMovementActionFuncs_FacePlayer},
    {sMovementActionFuncs_FaceAwayPlayer},
    {sMovementActionFuncs_LockFacingDirection},
    {sMovementActionFuncs_UnlockFacingDirection},
    {sMovementActionFuncs_JumpDown},
    {sMovementActionFuncs_JumpUp},
    {sMovementActionFuncs_JumpLeft},
    {sMovementActionFuncs_JumpRight},
    {sMovementActionFuncs_JumpInPlaceDown},
    {sMovementActionFuncs_JumpInPlaceUp},
    {sMovementActionFuncs_JumpInPlaceLeft},
    {sMovementActionFuncs_JumpInPlaceRight},
    {sMovementActionFuncs_JumpInPlaceDownUp},
    {sMovementActionFuncs_JumpInPlaceUpDown},
    {sMovementActionFuncs_JumpInPlaceLeftRight},
    {sMovementActionFuncs_JumpInPlaceRightLeft},
    {sMovementActionFuncs_FaceOriginalDirection},
    {sMovementActionFuncs_NurseJoyBowDown},
    {sMovementActionFuncs_EnableJumpLandingGroundEffect},
    {sMovementActionFuncs_DisableJumpLandingGroundEffect},
    {sMovementActionFuncs_DisableAnimation},
    {sMovementActionFuncs_RestoreAnimation},
    {sMovementActionFuncs_SetInvisible},
    {sMovementActionFuncs_SetVisible},
    {sMovementActionFuncs_EmoteExclamationMark},
    {sMovementActionFuncs_EmoteQuestionMark},
    {sMovementActionFuncs_EmoteX},
    {sMovementActionFuncs_EmoteDoubleExclMark},
    {sMovementActionFuncs_EmoteSmile},
    {sMovementActionFuncs_RevealTrainer},
    {sMovementActionFuncs_RockSmashBreak},
    {sMovementActionFuncs_CutTree},
    {sMovementActionFuncs_SetFixedPriority},
    {sMovementActionFuncs_ClearFixedPriority},
    {sMovementActionFuncs_InitAffineAnim},
    {sMovementActionFuncs_ClearAffineAnim},
    {sMovementActionFuncs_WalkDownStartAffine},
    {sMovementActionFuncs_WalkDownAffine},
    {sMovementActionFuncs_AcroWheelieFaceDown},
    {sMovementActionFuncs_AcroWheelieFaceUp},
    {sMovementActionFuncs_AcroWheelieFaceLeft},
    {sMovementActionFuncs_AcroWheelieFaceRight},
    {sMovementActionFuncs_AcroPopWheelieDown},
    {sMovementActionFuncs_AcroPopWheelieUp},
    {sMovementActionFuncs_AcroPopWheelieLeft},
    {sMovementActionFuncs_AcroPopWheelieRight},
    {sMovementActionFuncs_AcroEndWheelieFaceDown},
    {sMovementActionFuncs_AcroEndWheelieFaceUp},
    {sMovementActionFuncs_AcroEndWheelieFaceLeft},
    {sMovementActionFuncs_AcroEndWheelieFaceRight},
    {sMovementActionFuncs_AcroWheelieHopFaceDown},
    {sMovementActionFuncs_AcroWheelieHopFaceUp},
    {sMovementActionFuncs_AcroWheelieHopFaceLeft},
    {sMovementActionFuncs_AcroWheelieHopFaceRight},
    {sMovementActionFuncs_AcroWheelieHopDown},
    {sMovementActionFuncs_AcroWheelieHopUp},
    {sMovementActionFuncs_AcroWheelieHopLeft},
    {sMovementActionFuncs_AcroWheelieHopRight},
    {sMovementActionFuncs_AcroWheelieJumpDown},
    {sMovementActionFuncs_AcroWheelieJumpUp},
    {sMovementActionFuncs_AcroWheelieJumpLeft},
    {sMovementActionFuncs_AcroWheelieJumpRight},
    {sMovementActionFuncs_AcroWheelieInPlaceDown},
    {sMovementActionFuncs_AcroWheelieInPlaceUp},
    {sMovementActionFuncs_AcroWheelieInPlaceLeft},
    {sMovementActionFuncs_AcroWheelieInPlaceRight},
    {sMovementActionFuncs_AcroPopWheelieMoveDown},
    {sMovementActionFuncs_AcroPopWheelieMoveUp},
    {sMovementActionFuncs_AcroPopWheelieMoveLeft},
    {sMovementActionFuncs_AcroPopWheelieMoveRight},
    {sMovementActionFuncs_AcroWheelieMoveDown},
    {sMovementActionFuncs_AcroWheelieMoveUp},
    {sMovementActionFuncs_AcroWheelieMoveLeft},
    {sMovementActionFuncs_AcroWheelieMoveRight},
    {sMovementActionFuncs_SpinDown},
    {sMovementActionFuncs_SpinUp},
    {sMovementActionFuncs_SpinLeft},
    {sMovementActionFuncs_SpinRight},
    {sMovementActionFuncs_RaiseHandAndStop},
    {sMovementActionFuncs_RaiseHandAndJump},
    {sMovementActionFuncs_RaiseHandAndSwim},
    {sMovementActionFuncs_WalkSlowestDown},
    {sMovementActionFuncs_WalkSlowestUp},
    {sMovementActionFuncs_WalkSlowestLeft},
    {sMovementActionFuncs_WalkSlowestRight},
    {sMovementActionFuncs_ShakeHeadOrWalkInPlace},
    {sMovementActionFuncs_GlideDown},
    {sMovementActionFuncs_GlideUp},
    {sMovementActionFuncs_GlideLeft},
    {sMovementActionFuncs_GlideRight},
    {sMovementActionFuncs_FlyUp},
    {sMovementActionFuncs_FlyDown},
    {sMovementActionFuncs_JumpSpecialWithEffectDown},
    {sMovementActionFuncs_JumpSpecialWithEffectUp},
    {sMovementActionFuncs_JumpSpecialWithEffectLeft},
    {sMovementActionFuncs_JumpSpecialWithEffectRight},
    {sMovementActionFuncs_WalkNormalDownLeft_FaceLeft},     //0xAA
    {sMovementActionFuncs_WalkNormalDownRight_FaceRight},   //0xAB
    {sMovementActionFuncs_WalkNormalUpLeft_FaceLeft},       //0xAC
    {sMovementActionFuncs_WalkNormalUpRight_FaceRight},     //0xAD
    {sMovementActionFuncs_PayerRunLeftDown_FaceLeft},       //0xAE
    {sMovementActionFuncs_PayerRunRightDown_FaceRight},     //0xAF
    {sMovementActionFuncs_PayerRunRightUp_FaceRight},       //0xB0
    {sMovementActionFuncs_PayerRunLeftUp_FaceLeft},         //0xB1
    {sMovementActionFuncs_WalkFasterDownLeft_Left},         //0xB2
    {sMovementActionFuncs_WalkFasterDownRight_Right},       //0xB3
    {sMovementActionFuncs_WalkFasterUpLeft_Left},           //0xB4
    {sMovementActionFuncs_WalkFasterUpRight_Right},         //0xB5 - Standard Sideway Stairs End
    {sMovementActionFuncs_WalkSlowerUp_Backwards},          //0xB6
    {sMovementActionFuncs_WalkSlowerDown_Backwards},        //0xB7
    {sMovementActionFuncs_WalkSlowerRight_Backwards},       //0xB8
    {sMovementActionFuncs_WalkSlowerLeft_Backwards},        //0xB9
    {sMovementActionFuncs_WalkSlowUp_Backwards},            //0xBA
    {sMovementActionFuncs_WalkSlowDown_Backwards},          //0xBB
    {sMovementActionFuncs_WalkSlowRight_Backwards},         //0xBC
    {sMovementActionFuncs_WalkSlowLeft_Backwards},          //0xBD
    {sMovementActionFuncs_WalkNormalUp_Backwards},          //0xBE
    {sMovementActionFuncs_WalkNormalDown_Backwards},        //0xBF
    {sMovementActionFuncs_WalkNormalRight_Backwards},       //0xC0
    {sMovementActionFuncs_WalkNormalLeft_Backwards},        //0xC1
    {sMovementActionFuncs_WalkFasterUp_Backwards},          //0xC2
    {sMovementActionFuncs_WalkFasterDown_Backwards},        //0xC3
    {sMovementActionFuncs_WalkFasterRight_Backwards},       //0xC4
    {sMovementActionFuncs_WalkFasterLeft_Backwards},        //0xC5
    {sMovementActionFuncs_Jump2Up_Backwards},               //0xC6
    {sMovementActionFuncs_Jump2Down_Backwards},             //0xC7
    {sMovementActionFuncs_Jump2Right_Backwards},            //0xC8
    {sMovementActionFuncs_Jump2Left_Backwards},             //0xC9
    {sMovementActionFuncs_JumpUp_Backwards},                //0xCA
    {sMovementActionFuncs_JumpDown_Backwards},              //0xCB
    {sMovementActionFuncs_JumpRight_Backwards},             //0xCC
    {sMovementActionFuncs_JumpLeft_Backwards},              //0xCD
    {sMovementActionFuncs_WalkNormalDownLeft_FaceDown},     //0xCE
    {sMovementActionFuncs_WalkNormalUpLeft_FaceUp},         //0xCF
    {sMovementActionFuncs_WalkNormalDownLeft_FaceLeft},     //0xD0
    {sMovementActionFuncs_PayerRunRightDown_FaceRight},     //0xD1
    {sMovementActionFuncs_WalkNormalDownRight_FaceDown},    //0xD2
    {sMovementActionFuncs_WalkNormalUpRight_FaceUp},        //0xD3
    {sMovementActionFuncs_WalkNormalUpLeft_FaceLeft},       //0xD4
    {sMovementActionFuncs_WalkNormalUpRight_FaceRight},     //0xD5
    {sMovementActionFuncs_RunDownLeft_FaceDown},            //0xD6
    {sMovementActionFuncs_RunUpLeft_FaceUp},                //0xD7
    {sMovementActionFuncs_PayerRunLeftDown_FaceLeft},       //0xD8
    {sMovementActionFuncs_PayerRunRightDown_FaceRight},     //0xD9
    {sMovementActionFuncs_RunDownRight_FaceDown},           //0xDA
    {sMovementActionFuncs_RunUpRight_FaceUp},               //0xDB
    {sMovementActionFuncs_PayerRunLeftUp_FaceLeft},         //0xDC
    {sMovementActionFuncs_PayerRunRightUp_FaceRight},       //0xDD
    {sMovementActionFuncs_WalkFast_DownLeft_FaceDown},      //0xDE
    {sMovementActionFuncs_WalkFast_UpLeft_FaceUp},          //0xDF
    {sMovementActionFuncs_WalkFasterDownLeft_Left},         //0xE0
    {sMovementActionFuncs_WalkFasterDownRight_Right},       //0xE1
    {sMovementActionFuncs_WalkFast_DownRight_FaceDown},     //0xE2
    {sMovementActionFuncs_WalkFast_UpRight_FaceUp},         //0xE3
    {sMovementActionFuncs_WalkFasterUpLeft_Left},           //0xE4
    {sMovementActionFuncs_WalkFasterUpRight_Right},         //0xE5
    {gMovementActionFuncs_FollowingPokemon},                //0xE6
    {gMovementActionFuncs_FollowingPokemon_FaceSouth},      //0xE7
    {gMovementActionFuncs_FollowingPokemon_FaceNorth},      //0xE8
    {gMovementActionFuncs_FollowingPokemon_FaceWest},       //0xE9
    {gMovementActionFuncs_FollowingPokemon_FaceEast},       //0xEA
};

void (*const sMovementTypeCallbacks[MOVEMENT_TYPES_COUNT])(struct Sprite *) =
{
    [MOVEMENT_TYPE_NONE]                                  = (void*) 0x0805ffb4 + 1,
    [MOVEMENT_TYPE_LOOK_AROUND]                           = (void*) 0x0806058c + 1,
    [MOVEMENT_TYPE_WANDER_AROUND]                         = (void*) 0x0805ffdc + 1,
    [MOVEMENT_TYPE_WANDER_UP_AND_DOWN]                    = (void*) 0x080606cc + 1,
    [MOVEMENT_TYPE_WANDER_DOWN_AND_UP]                    = (void*) 0x080606cc + 1,
    [MOVEMENT_TYPE_WANDER_LEFT_AND_RIGHT]                 = (void*) 0x08060850 + 1,
    [MOVEMENT_TYPE_WANDER_RIGHT_AND_LEFT]                 = (void*) 0x08060850 + 1,
    [MOVEMENT_TYPE_FACE_UP]                               = (void*) 0x080609d4 + 1,
    [MOVEMENT_TYPE_FACE_DOWN]                             = (void*) 0x080609d4 + 1,
    [MOVEMENT_TYPE_FACE_LEFT]                             = (void*) 0x080609d4 + 1,
    [MOVEMENT_TYPE_FACE_RIGHT]                            = (void*) 0x080609d4 + 1,
    [MOVEMENT_TYPE_PLAYER]                                = (void*) 0x0805b3b8 + 1,
    [MOVEMENT_TYPE_BERRY_TREE_GROWTH]                     = (void*) NULL,
    [MOVEMENT_TYPE_FACE_DOWN_AND_UP]                      = (void*) 0x08060a74 + 1,
    [MOVEMENT_TYPE_FACE_LEFT_AND_RIGHT]                   = (void*) 0x08060bb4 + 1,
    [MOVEMENT_TYPE_FACE_UP_AND_LEFT]                      = (void*) 0x08060cf4 + 1,
    [MOVEMENT_TYPE_FACE_UP_AND_RIGHT]                     = (void*) 0x08060e34 + 1,
    [MOVEMENT_TYPE_FACE_DOWN_AND_LEFT]                    = (void*) 0x08060f74 + 1,
    [MOVEMENT_TYPE_FACE_DOWN_AND_RIGHT]                   = (void*) 0x080610b4 + 1,
    [MOVEMENT_TYPE_FACE_DOWN_UP_AND_LEFT]                 = (void*) 0x080611f4 + 1,
    [MOVEMENT_TYPE_FACE_DOWN_UP_AND_RIGHT]                = (void*) 0x08061334 + 1,
    [MOVEMENT_TYPE_FACE_UP_LEFT_AND_RIGHT]                = (void*) 0x08061474 + 1,
    [MOVEMENT_TYPE_FACE_DOWN_LEFT_AND_RIGHT]              = (void*) 0x080615b4 + 1,
    [MOVEMENT_TYPE_ROTATE_COUNTERCLOCKWISE]               = (void*) 0x080616f4 + 1,
    [MOVEMENT_TYPE_ROTATE_CLOCKWISE]                      = (void*) 0x080617f8 + 1,
    [MOVEMENT_TYPE_WALK_UP_AND_DOWN]                      = (void*) 0x080618fc + 1,
    [MOVEMENT_TYPE_WALK_DOWN_AND_UP]                      = (void*) 0x080618fc + 1,
    [MOVEMENT_TYPE_WALK_LEFT_AND_RIGHT]                   = (void*) 0x080618fc + 1,
    [MOVEMENT_TYPE_WALK_RIGHT_AND_LEFT]                   = (void*) 0x080618fc + 1,
    [MOVEMENT_TYPE_WALK_SEQUENCE_UP_RIGHT_LEFT_DOWN]      = (void*) 0x08061b5c + 1,
    [MOVEMENT_TYPE_WALK_SEQUENCE_RIGHT_LEFT_DOWN_UP]      = (void*) 0x08061be8 + 1,
    [MOVEMENT_TYPE_WALK_SEQUENCE_DOWN_UP_RIGHT_LEFT]      = (void*) 0x08061c74 + 1,
    [MOVEMENT_TYPE_WALK_SEQUENCE_LEFT_DOWN_UP_RIGHT]      = (void*) 0x08061d00 + 1,
    [MOVEMENT_TYPE_WALK_SEQUENCE_UP_LEFT_RIGHT_DOWN]      = (void*) 0x08061d8c + 1,
    [MOVEMENT_TYPE_WALK_SEQUENCE_LEFT_RIGHT_DOWN_UP]      = (void*) 0x08061e18 + 1,
    [MOVEMENT_TYPE_WALK_SEQUENCE_DOWN_UP_LEFT_RIGHT]      = (void*) 0x08061ea4 + 1,
    [MOVEMENT_TYPE_WALK_SEQUENCE_RIGHT_DOWN_UP_LEFT]      = (void*) 0x08061f30 + 1,
    [MOVEMENT_TYPE_WALK_SEQUENCE_LEFT_UP_DOWN_RIGHT]      = (void*) 0x08061fbc + 1,
    [MOVEMENT_TYPE_WALK_SEQUENCE_UP_DOWN_RIGHT_LEFT]      = (void*) 0x08062048 + 1,
    [MOVEMENT_TYPE_WALK_SEQUENCE_RIGHT_LEFT_UP_DOWN]      = (void*) 0x080620d4 + 1,
    [MOVEMENT_TYPE_WALK_SEQUENCE_DOWN_RIGHT_LEFT_UP]      = (void*) 0x08062160 + 1,
    [MOVEMENT_TYPE_WALK_SEQUENCE_RIGHT_UP_DOWN_LEFT]      = (void*) 0x080621ec + 1,
    [MOVEMENT_TYPE_WALK_SEQUENCE_UP_DOWN_LEFT_RIGHT]      = (void*) 0x08062278 + 1,
    [MOVEMENT_TYPE_WALK_SEQUENCE_LEFT_RIGHT_UP_DOWN]      = (void*) 0x08062304 + 1,
    [MOVEMENT_TYPE_WALK_SEQUENCE_DOWN_LEFT_RIGHT_UP]      = (void*) 0x08062390 + 1,
    [MOVEMENT_TYPE_WALK_SEQUENCE_UP_LEFT_DOWN_RIGHT]      = (void*) 0x0806241c + 1,
    [MOVEMENT_TYPE_WALK_SEQUENCE_DOWN_RIGHT_UP_LEFT]      = (void*) 0x080624a8 + 1,
    [MOVEMENT_TYPE_WALK_SEQUENCE_LEFT_DOWN_RIGHT_UP]      = (void*) 0x08062534 + 1,
    [MOVEMENT_TYPE_WALK_SEQUENCE_RIGHT_UP_LEFT_DOWN]      = (void*) 0x080625c0 + 1,
    [MOVEMENT_TYPE_WALK_SEQUENCE_UP_RIGHT_DOWN_LEFT]      = (void*) 0x0806264c + 1,
    [MOVEMENT_TYPE_WALK_SEQUENCE_DOWN_LEFT_UP_RIGHT]      = (void*) 0x080626d8 + 1,
    [MOVEMENT_TYPE_WALK_SEQUENCE_LEFT_UP_RIGHT_DOWN]      = (void*) 0x08062764 + 1,
    [MOVEMENT_TYPE_WALK_SEQUENCE_RIGHT_DOWN_LEFT_UP]      = (void*) 0x080627f0 + 1,
    [MOVEMENT_TYPE_COPY_PLAYER]                           = (void*) 0x0806287c + 1,
    [MOVEMENT_TYPE_COPY_PLAYER_OPPOSITE]                  = (void*) 0x0806287c + 1,
    [MOVEMENT_TYPE_COPY_PLAYER_COUNTERCLOCKWISE]          = (void*) 0x0806287c + 1,
    [MOVEMENT_TYPE_COPY_PLAYER_CLOCKWISE]                 = (void*) 0x0806287c + 1,
    [MOVEMENT_TYPE_TREE_DISGUISE]                         = (void*) 0x08062f08 + 1,
    [MOVEMENT_TYPE_MOUNTAIN_DISGUISE]                     = (void*) 0x08062f84 + 1,
    [MOVEMENT_TYPE_COPY_PLAYER_IN_GRASS]                  = (void*) 0x08062e60 + 1,
    [MOVEMENT_TYPE_COPY_PLAYER_OPPOSITE_IN_GRASS]         = (void*) 0x08062e60 + 1,
    [MOVEMENT_TYPE_COPY_PLAYER_COUNTERCLOCKWISE_IN_GRASS] = (void*) 0x08062e60 + 1,
    [MOVEMENT_TYPE_COPY_PLAYER_CLOCKWISE_IN_GRASS]        = (void*) 0x08062e60 + 1,
    [MOVEMENT_TYPE_HIDDEN]                                = (void*) 0x08062ff4 + 1,
    [MOVEMENT_TYPE_WALK_IN_PLACE_DOWN]                    = (void*) 0x080630a0 + 1,
    [MOVEMENT_TYPE_WALK_IN_PLACE_UP]                      = (void*) 0x080630a0 + 1,
    [MOVEMENT_TYPE_WALK_IN_PLACE_LEFT]                    = (void*) 0x080630a0 + 1,
    [MOVEMENT_TYPE_WALK_IN_PLACE_RIGHT]                   = (void*) 0x080630a0 + 1,
    [MOVEMENT_TYPE_JOG_IN_PLACE_DOWN]                     = (void*) 0x08063110 + 1,
    [MOVEMENT_TYPE_JOG_IN_PLACE_UP]                       = (void*) 0x08063110 + 1,
    [MOVEMENT_TYPE_JOG_IN_PLACE_LEFT]                     = (void*) 0x08063110 + 1,
    [MOVEMENT_TYPE_JOG_IN_PLACE_RIGHT]                    = (void*) 0x08063110 + 1,
    [MOVEMENT_TYPE_RUN_IN_PLACE_DOWN]                     = (void*) 0x08063180 + 1,
    [MOVEMENT_TYPE_RUN_IN_PLACE_UP]                       = (void*) 0x08063180 + 1,
    [MOVEMENT_TYPE_RUN_IN_PLACE_LEFT]                     = (void*) 0x08063180 + 1,
    [MOVEMENT_TYPE_RUN_IN_PLACE_RIGHT]                    = (void*) 0x08063180 + 1,
    [MOVEMENT_TYPE_INVISIBLE]                             = (void*) 0x080631f0 + 1,
    [MOVEMENT_TYPE_WALK_SLOWLY_IN_PLACE_DOWN]             = (void*) 0x08063298 + 1,
    [MOVEMENT_TYPE_WALK_SLOWLY_IN_PLACE_UP]               = (void*) 0x080632bc + 1,
    [MOVEMENT_TYPE_WALK_SLOWLY_IN_PLACE_LEFT]             = (void*) 0x080632e0 + 1,
    [MOVEMENT_TYPE_WALK_SLOWLY_IN_PLACE_RIGHT]            = (void*) 0x08060020 + 1,
    [MOVEMENT_TYPE_FOLLOWING_POKEMON]                     = MovementType_FollowingPokemon,
};
