#ifndef GUARD_FIELD_EVENT_OBJ_H
#define GUARD_FIELD_EVENT_OBJ_H

#define NUM_OBJECT_GRAPHICS_INFO 239
#define SPRITE_VAR 240

enum SpinnerRunnerFollowPatterns
{
    RUNFOLLOW_ANY,
    RUNFOLLOW_NORTH_SOUTH,
    RUNFOLLOW_EAST_WEST,
    RUNFOLLOW_NORTH_WEST,
    RUNFOLLOW_NORTH_EAST,
    RUNFOLLOW_SOUTH_WEST,
    RUNFOLLOW_SOUTH_EAST,
    RUNFOLLOW_NORTH_SOUTH_WEST,
    RUNFOLLOW_NORTH_SOUTH_EAST,
    RUNFOLLOW_NORTH_EAST_WEST,
    RUNFOLLOW_SOUTH_EAST_WEST
};

struct UnkStruct_085094AC
{
    const union AnimCmd *const *anims;
    u8 animPos[4];
};

#define GROUND_EFFECT_FLAG_TALL_GRASS_ON_SPAWN   (1 << 0)
#define GROUND_EFFECT_FLAG_TALL_GRASS_ON_MOVE    (1 << 1)
#define GROUND_EFFECT_FLAG_LONG_GRASS_ON_SPAWN   (1 << 2)
#define GROUND_EFFECT_FLAG_LONG_GRASS_ON_MOVE    (1 << 3)
#define GROUND_EFFECT_FLAG_ICE_REFLECTION        (1 << 4)
#define GROUND_EFFECT_FLAG_REFLECTION            (1 << 5)
#define GROUND_EFFECT_FLAG_SHALLOW_FLOWING_WATER (1 << 6)
#define GROUND_EFFECT_FLAG_SAND                  (1 << 7)
#define GROUND_EFFECT_FLAG_DEEP_SAND             (1 << 8)
#define GROUND_EFFECT_FLAG_RIPPLES               (1 << 9)
#define GROUND_EFFECT_FLAG_PUDDLE                (1 << 10)
#define GROUND_EFFECT_FLAG_SAND_PILE             (1 << 11)
#define GROUND_EFFECT_FLAG_LAND_IN_TALL_GRASS    (1 << 12)
#define GROUND_EFFECT_FLAG_LAND_IN_LONG_GRASS    (1 << 13)
#define GROUND_EFFECT_FLAG_LAND_IN_SHALLOW_WATER (1 << 14)
#define GROUND_EFFECT_FLAG_LAND_IN_DEEP_WATER    (1 << 15)
#define GROUND_EFFECT_FLAG_LAND_ON_NORMAL_GROUND (1 << 16)
#define GROUND_EFFECT_FLAG_SHORT_GRASS           (1 << 17)
#define GROUND_EFFECT_FLAG_HOT_SPRINGS           (1 << 18)
#define GROUND_EFFECT_FLAG_SEAWEED               (1 << 19)

struct PairedPalettes
{
    u16 tag;
    const u16 *data;
};

struct LockedAnimEventObjects
{
    u8 eventObjectIds[EVENT_OBJECTS_COUNT];
    u8 count;
};

void __attribute__((long_call)) EventObjectClearHeldMovementIfActive(struct EventObject *);
bool8 __attribute__((long_call)) EventObjectSetHeldMovement(struct EventObject *eventObject, u8 specialAnimId);
u8 __attribute__((long_call)) EventObjectClearHeldMovementIfFinished(struct EventObject *);
u8 __attribute__((long_call)) TrySpawnEventObjectTemplate(struct EventObjectTemplate *eventObjectTemplate, u8 mapNum, u8 mapGroup, s16 cameraX, s16 cameraY);
struct EventObjectTemplate* __attribute__((long_call)) GetEventObjectTemplateByLocalIdAndMap(u8 localId, u8 mapNum, u8 mapGroup);
void __attribute__((long_call)) MoveEventObjectToMapCoords(struct EventObject *eventObject, s16 x, s16 y);
void __attribute__((long_call)) EventObjectSetGraphicsId(struct EventObject *eventObject, u16 graphicsId);
void __attribute__((long_call)) EventObjectTurn(struct EventObject *eventObject, u8 direction);
void __attribute__((long_call)) EventObjectClearHeldMovement(struct EventObject *eventObject);
u8 __attribute__((long_call)) GetJumpMovementAction(u32 idx);
u8 __attribute__((long_call)) GetJump2MovementAction(u32 idx);
u8 __attribute__((long_call)) GetJumpSpecialMovementAction(u32 idx);
void __attribute__((long_call)) FreezeEventObjects(void);
void __attribute__((long_call)) UnfreezeEventObjects(void);
u8* __attribute__((long_call)) GetEventObjectScriptPointerByEventObjectId(u8);
u8 __attribute__((long_call)) GetRunningDirectionAnimNum(u8 direction);
void __attribute__((long_call)) npc_apply_anim_looping(struct EventObject *eventObject, struct Sprite *sprite, u8 animNum);
void __attribute__((long_call)) npc_apply_direction(struct EventObject *eventObject, struct Sprite *sprite, u8 direction, u8 speed);
bool8 __attribute__((long_call)) npc_obj_ministep_stop_on_arrival(struct EventObject *eventObject, struct Sprite *sprite);
u8 __attribute__((long_call)) GetEventObjectIdByLocalIdAndMap(u8, u8, u8);
u8 __attribute__((long_call)) GetEventObjectIdByLocalIdAndMapInternal(u8 localId, u8 mapNum, u8 mapGroupId);
u8 __attribute__((long_call)) GetEventObjectIdByLocalId(u8 localId);
u8 __attribute__((long_call)) GetEventObjectIdByXY(s16 x, s16 y);
void __attribute__((long_call)) FaceDirection(struct EventObject *eventObject, struct Sprite *sprite, u8 direction);
u8 __attribute__((long_call)) GetOppositeDirection(u8 direction);
u8 __attribute__((long_call)) CreateVirtualSprite(u8 graphicsId, u8 a1, s16 x, s16 y, u8 z, u8 direction);
void __attribute__((long_call)) MoveCoords(u8, s16 *, s16 *);
u8 __attribute__((long_call)) MovementAction_EmoteExclamationMark_Step0(struct EventObject *, struct Sprite *);
bool8 __attribute__((long_call)) IsZCoordMismatchAt(u8, s16, s16);
void __attribute__((long_call)) SetSpritePosToOffsetMapCoords(s32* x, s32* y, u8 size_x, u8 size_y);
void __attribute__((long_call)) RemoveEventObject(struct EventObject *eventObject);
u8 __attribute__((long_call)) GetCollisionInDirection(struct EventObject*, u8);
u8 __attribute__((long_call)) GetCollisionAtCoords(struct EventObject *, s16, s16, u32);
u8 __attribute__((long_call)) GetFaceDirectionMovementAction(u32);
u8 __attribute__((long_call)) SpawnSpecialEventObject(struct EventObjectTemplate *);
const struct EventObjectGraphicsInfo* __attribute__((long_call)) GetEventObjectGraphicsInfoOriginal(u8 graphicsId);
void __attribute__((long_call)) MakeObjectTemplateFromEventObjectTemplate(struct EventObjectTemplate *, struct SpriteTemplate *, const struct SubspriteTable **);
void __attribute__((long_call)) MakeObjectTemplateFromEventObjectGraphicsInfo(u16 graphicsId, void* callback, void* spriteTemplate, void* subspriteTables);
u8 __attribute__((long_call)) GetMoveDirectionAnimNum(u8 direction);
u8 __attribute__((long_call)) GetMoveDirectionFastAnimNum(u8 direction);
#ifdef PALETTE_SWAPPER
u8 __attribute__((long_call)) TryLoadObjectPalette(const struct SpritePalette *spritePalette);
void __attribute__((long_call)) ApplyGlobalFieldPaletteTint(u8 paletteIdx);
#else
void __attribute__((long_call)) InitEventObjectPalettes(u8 palSlot);
void __attribute__((long_call)) PatchObjectPalette(u16, u8);
#endif
u8 __attribute__((long_call)) FindEventObjectPaletteIndexByTag(u16 tag);
struct EventObjectTemplate* __attribute__((long_call)) FindEventObjectTemplateByLocalId(u8 localId, struct EventObjectTemplate* templates, u8 count);
u8 __attribute__((long_call)) AddPseudoEventObject(u16, void (*)(struct Sprite *), s16 x, s16 y, u8 subpriority);
void __attribute__((long_call)) SetObjectSubpriorityByZCoord(u8, struct Sprite *, u8);
void __attribute__((long_call)) InitObjectPriorityByZCoord(struct Sprite *, u8);
u8 __attribute__((long_call)) EventObjectFaceOppositeDirection(struct EventObject *, u8);
void __attribute__((long_call)) LockSelectedEventObject(void);
bool8 __attribute__((long_call)) FreezeEventObject(struct EventObject *eventObject);
void __attribute__((long_call)) ClearPlayerHeldMovementAndUnfreezeEventObjects(void);
u8 __attribute__((long_call)) ZCoordToPriority(u8);
u8 __attribute__((long_call)) GetFaceDirectionMovementAction(u32);
u8 __attribute__((long_call)) GetWalkNormalMovementAction(u32);
u8 __attribute__((long_call)) GetWalkFastMovementAction(u32);
u8 __attribute__((long_call)) GetRideWaterCurrentMovementAction(u32);
u8 __attribute__((long_call)) GetWalkFastestMovementAction(u32);
u8 __attribute__((long_call)) GetPlayerRunMovementAction(u32);
u8 __attribute__((long_call)) GetJumpInPlaceMovementAction(u32);
u8 __attribute__((long_call)) GetWalkInPlaceNormalMovementAction(u32);
void __attribute__((long_call)) OverrideMovementTypeForEventObject(const struct EventObject* eventObject, u8 movementType);
void __attribute__((long_call)) TryOverrideTemplateCoordsForEventObject(const struct EventObject* eventObject, u8 movementType);
void __attribute__((long_call)) OverrideTemplateCoordsForEventObject(const struct EventObject* eventObject);
u8 __attribute__((long_call)) GetTrainerFacingDirectionMovementType(u8 direction);
bool8 __attribute__((long_call)) EventObjectSetHeldMovement(struct EventObject *eventObject, u8 specialAnimId);
void __attribute__((long_call)) EventObjectForceSetHeldMovement(struct EventObject *eventObject, u8 movementActionId);
bool8 __attribute__((long_call)) EventObjectIsMovementOverridden(struct EventObject *eventObject);
u8 __attribute__((long_call)) EventObjectCheckHeldMovementStatus(struct EventObject *eventObject);
u8 __attribute__((long_call)) EventObjectGetHeldMovementActionId(struct EventObject *eventObject);
void __attribute__((long_call)) DoShadowFieldEffect(struct EventObject*);
void __attribute__((long_call)) DoGroundEffects_OnBeginStep(struct EventObject* eventObject, struct Sprite *sprite);
void __attribute__((long_call)) DoGroundEffects_OnFinishStep(struct EventObject* eventObject, struct Sprite *sprite);

//New Added
u8 GetSidewaysStairsCollision(struct EventObject *eventObject, u8 dir, u8 currentBehavior, u8 nextBehavior, u8 collision);
bool8 __attribute__((long_call)) DoesObjectCollideWithObjectAt(struct EventObject *, s16, s16);

bool8 __attribute__((long_call)) MovementAction_WalkInPlace_Step1(struct EventObject *eventObject, struct Sprite *sprite);
bool8 __attribute__((long_call)) MovementAction_RideWaterCurrentLeft_Step1(struct EventObject *eventObject, struct Sprite *sprite);
bool8 __attribute__((long_call)) MovementAction_RideWaterCurrentRight_Step1(struct EventObject *eventObject, struct Sprite *sprite);
bool8 __attribute__((long_call)) MovementAction_WalkFasterLeft_Step1(struct EventObject *eventObject, struct Sprite *sprite);
bool8 __attribute__((long_call)) MovementAction_WalkFasterRight_Step1(struct EventObject *eventObject, struct Sprite *sprite);
bool8 __attribute__((long_call)) MovementAction_SlideLeft_Step1(struct EventObject *eventObject, struct Sprite *sprite);
bool8 __attribute__((long_call)) MovementAction_SlideRight_Step1(struct EventObject *eventObject, struct Sprite *sprite);

void __attribute__((long_call)) GetGroundEffectFlags_Tracks(struct EventObject *objEvent, u32 *flags);
void __attribute__((long_call)) SetObjectEventDirection(struct EventObject *eventObject, u8 direction);
bool8 __attribute__((long_call)) UpdateWalkSlower(struct EventObject *eventObject, struct Sprite *sprite);
void __attribute__((long_call)) InitWalkSlower(struct EventObject *eventObject, struct Sprite *sprite, u8 direction);
void __attribute__((long_call)) ShiftEventObjectCoords(struct EventObject *, s16, s16);
void __attribute__((long_call)) SetWalkSlowerSpriteData(struct Sprite *sprite, u8 direction);
void __attribute__((long_call)) SetSpriteDataForNormalStep(struct Sprite *sprite, u8 direction, u8 speed);
u8 __attribute__((long_call)) GetVanillaCollision(struct EventObject *eventObject, s16 x, s16 y, u32 dir);
u8 __attribute__((long_call)) GetMoveDirectionFasterAnimNum(u8 direction);
void __attribute__((long_call)) InitMoveInPlace(struct EventObject *eventObject, struct Sprite *sprite, u8 direction, u8 animNum, u16 duration);
bool8 __attribute__((long_call)) UpdateWalkSlow(struct EventObject *eventObject, struct Sprite *sprite);
void __attribute__((long_call)) SetWalkSlowSpriteData(struct Sprite *sprite, u8 direction);
bool8 __attribute__((long_call)) DoJumpAnim(struct EventObject *eventObject, struct Sprite *sprite);
void __attribute__((long_call)) MoveCoordsInDirection(u32 dir, s16 *x, s16 *y, s16 deltaX, s16 deltaY);
void __attribute__((long_call)) SetJumpSpriteData(struct Sprite *sprite, u8 direction, u8 distance, u8 type);
void __attribute__((long_call)) SetObjectEventDynamicGraphicsId(struct EventObject *);
void __attribute__((long_call)) ClearEventObjectMovement(struct EventObject *, struct Sprite *);
void __attribute__((long_call)) EventObjectSetSingleMovement(struct EventObject *, struct Sprite *, u8);
bool8 __attribute__((long_call)) EventObjectExecSingleMovementAction(struct EventObject *, struct Sprite *);
void __attribute__((long_call)) ShiftStillEventObjectCoords(struct EventObject *pObject);
bool8 __attribute__((long_call)) NpcTakeStep(struct Sprite *sprite);
u8 __attribute__((long_call)) DoJumpSpriteMovement(struct Sprite *sprite);
void __attribute__((long_call)) InitJumpRegular(struct EventObject *objectEvent, struct Sprite *sprite, u8 direction, u8 distance, u8 type);
void MovementType_FollowingPokemon(struct Sprite *);
void __attribute__((long_call)) UpdateObjectEventCurrentMovement(struct EventObject *objectEvent, struct Sprite *sprite, bool8 (*callback)(struct EventObject *, struct Sprite *));
void __attribute__((long_call)) OldStartRunningAnim(struct EventObject *objectEvent, struct Sprite *sprite, u8 direction);
void __attribute__((long_call)) SetMovementDelay(struct Sprite *sprite, s16 delay);
void __attribute__((long_call)) RemoveEventObjectIfOutsideView(struct EventObject *objectEvent);
u8 __attribute__((long_call)) GetEventObjectIdByPosition(u16 x, u16 y, u8 elevation);
u8 __attribute__((long_call)) GetPlayerFacingDirection(void);

/*
extern const struct SpriteFrameImage gEventObjectPicTable_PechaBerryTree[];
extern const struct OamData gEventObjectBaseOam_32x8;
extern const struct OamData gEventObjectBaseOam_32x32;
extern const struct SpriteTemplate *const gFieldEffectObjectTemplatePointers[];
extern const u8 gReflectionEffectPaletteMap[];

void ResetEventObjects(void);
bool8 TryGetEventObjectIdByLocalIdAndMap(u8, u8, u8, u8 *);
void SetEventObjectDirection(struct EventObject *, u8);
u8 GetFirstInactiveEventObjectId(void);
void RemoveEventObjectByLocalIdAndMap(u8, u8, u8);
void LoadPlayerObjectReflectionPalette(u16, u8);
void LoadSpecialObjectReflectionPalette(u16, u8);
void sub_808EBA8(u8, u8, u8, s16, s16);
void sub_808E16C(s16, s16);
void sub_808F28C(u8 localId, u8 mapNum, u8 mapGroup, u8 decorCat);
void sub_8092FF0(s16, s16, s16 *, s16 *);
u8 GetFaceDirectionAnimNum(u8);
void sub_80930E0(s16 *, s16 *, s16, s16);
void EventObjectClearHeldMovement(struct EventObject *);
void EventObjectClearHeldMovementIfActive(struct EventObject *);
void TrySpawnEventObjects(s16, s16);
u8 show_sprite(u8, u8, u8);
u8 SpawnSpecialEventObjectParameterized(u8 graphicsId, u8 movementBehavior, u8 localId, s16 x, s16 y, u8 z);
void sub_8093038(s16, s16, s16 *, s16 *);
void CameraObjectReset1(void);
void EventObjectSetGraphicsId(struct EventObject *, u8 graphicsId);
void EventObjectTurn(struct EventObject *, u8);
void EventObjectTurnByLocalIdAndMap(u8, u8, u8, u8);
void npc_by_local_id_and_map_set_field_1_bit_x20(u8, u8, u8, u8);
void FreeAndReserveObjectSpritePalettes(void);
void sub_808E82C(u8, u8, u8, s16, s16);
void sub_808E7E4(u8, u8, u8);
void sub_808E78C(u8, u8, u8, u8);
void sub_808E75C(s16, s16);
void EventObjectGetLocalIdAndMap(struct EventObject *eventObject, void *localId, void *mapNum, void *mapGroup);
void ShiftEventObjectCoords(struct EventObject *, s16, s16);
void sub_808EB08(struct EventObject *, s16, s16);
void sub_808F254(u8, u8, u8);
void UpdateEventObjectCurrentMovement(struct EventObject *, struct Sprite *, bool8(struct EventObject *, struct Sprite *));
u8 GetWalkInPlaceFastestMovementAction(u32);
u8 GetWalkInPlaceFastMovementAction(u32);
u8 GetWalkInPlaceSlowMovementAction(u32);
bool8 EventObjectIsHeldMovementActive(struct EventObject *);
u8 EventObjectClearHeldMovementIfFinished(struct EventObject *);
u8 GetEventObjectIdByXYZ(u16 x, u16 y, u8 z);
void SetTrainerMovementType(struct EventObject *eventObject, u8 movementType);
u8 GetTrainerFacingDirectionMovementType(u8 direction);
const u8 *GetEventObjectScriptPointerByEventObjectId(u8 eventObjectId);
u8 GetCollisionFlagsAtCoords(struct EventObject *eventObject, s16 x, s16 y, u8 direction);
u8 GetAcroWheelieFaceDirectionMovementAction(u32);
u8 GetAcroPopWheelieFaceDirectionMovementAction(u32);
u8 GetAcroEndWheelieFaceDirectionMovementAction(u32);
u8 GetAcroWheelieHopFaceDirectionMovementAction(u32);
u8 GetAcroWheelieHopDirectionMovementAction(u32);
u8 GetAcroWheelieJumpDirectionMovementAction(u32);
u8 GetJumpInPlaceTurnAroundMovementAction(u32);
u8 GetAcroWheelieInPlaceDirectionMovementAction(u32);
u8 GetAcroPopWheelieMoveDirectionMovementAction(u32);
u8 GetAcroWheelieMoveDirectionMovementAction(u32);
u8 GetAcroEndWheelieMoveDirectionMovementAction(u32);
u8 GetFishingDirectionAnimNum(u8 direction);
u8 GetAcroWheelieDirectionAnimNum(u8 direction);
u8 GetFishingBiteDirectionAnimNum(u8 direction);
u8 GetFishingNoCatchDirectionAnimNum(u8 direction);
void TryOverrideTemplateCoordsForEventObject(const struct EventObject *eventObject, u8 movementType);
void OverrideTemplateCoordsForEventObject(const struct EventObject *eventObject);
void ShiftStillEventObjectCoords(struct EventObject *pObject);
void EventObjectMoveDestCoords(struct EventObject *pObject, u32 unk_19, s16 *pInt, s16 *pInt1);
u8 AddCameraObject(u8 linkedSpriteId);
void UpdateEventObjectsForCameraUpdate(s16 x, s16 y);
u8 GetWalkSlowMovementAction(u32);
u8 GetJumpMovementAction(u32);
bool8 AreZCoordsCompatible(u8, u8);
void EventObjectUpdateZCoord(struct EventObject *pObject);
void SetObjectSubpriorityByZCoord(u8, struct Sprite *, u8);
void UnfreezeEventObject(struct EventObject *);
u8 FindLockedEventObjectIndex(struct EventObject *);
bool8 obj_npc_ministep(struct Sprite *sprite);
bool8 sub_80976EC(struct Sprite *sprite);
void sub_80976DC(struct Sprite *, u8);
void sub_809783C(struct Sprite *, u8, u8, u8);
u8 sub_809785C(struct Sprite *);
u8 sub_80978E4(struct Sprite *);
void SetAndStartSpriteAnim(struct Sprite *, u8, u8);
bool8 SpriteAnimEnded(struct Sprite *);
void sub_8097750(struct Sprite *);
bool8 sub_8097758(struct Sprite *);
void CreateLevitateMovementTask(struct EventObject *);
void DestroyExtraMovementTask(u8);
void FreezeEventObjectsExceptOne(u8 eventObjectId);
void sub_8097B78(u8, u8);
void sub_8098074(u8 var1, u8 var2);
bool8 FreezeEventObject(struct EventObject *eventObject);
u8 GetMoveDirectionFastAnimNum(u8);
u8 GetMoveDirectionFasterAnimNum(u8);
u8 GetMoveDirectionFastestAnimNum(u8);
u8 GetLedgeJumpDirection(s16, s16, u8);
void CameraObjectSetFollowedObjectId(u8 objectId);
u16 GetObjectPaletteTag(u8 palSlot);
void UpdateEventObjectSpriteVisibility(struct Sprite *sprite, bool8 invisible);
s16 sub_809773C(s16 a1);
s16 sub_8097728(s16 a1);
void CameraObjectReset2(void);


void MovementType_None(struct Sprite *);
void MovementType_LookAround(struct Sprite *);
void MovementType_WanderAround(struct Sprite *);
void MovementType_WanderUpAndDown(struct Sprite *);
void MovementType_WanderLeftAndRight(struct Sprite *);
void MovementType_FaceDirection(struct Sprite *);
void MovementType_Player(struct Sprite *);
void MovementType_BerryTreeGrowth(struct Sprite *);
void MovementType_FaceDownAndUp(struct Sprite *);
void MovementType_FaceLeftAndRight(struct Sprite *);
void MovementType_FaceUpAndLeft(struct Sprite *);
void MovementType_FaceUpAndRight(struct Sprite *);
void MovementType_FaceDownAndLeft(struct Sprite *);
void MovementType_FaceDownAndRight(struct Sprite *);
void MovementType_FaceDownUpAndLeft(struct Sprite *);
void MovementType_FaceDownUpAndRight(struct Sprite *);
void MovementType_FaceUpRightAndLeft(struct Sprite *);
void MovementType_FaceDownRightAndLeft(struct Sprite *);
void MovementType_RotateCounterclockwise(struct Sprite *);
void MovementType_RotateClockwise(struct Sprite *);
void MovementType_WalkBackAndForth(struct Sprite *);
void MovementType_WalkSequenceUpRightLeftDown(struct Sprite *);
void MovementType_WalkSequenceRightLeftDownUp(struct Sprite *);
void MovementType_WalkSequenceDownUpRightLeft(struct Sprite *);
void MovementType_WalkSequenceLeftDownUpRight(struct Sprite *);
void MovementType_WalkSequenceUpLeftRightDown(struct Sprite *);
void MovementType_WalkSequenceLeftRightDownUp(struct Sprite *);
void MovementType_WalkSequenceDownUpLeftRight(struct Sprite *);
void MovementType_WalkSequenceRightDownUpLeft(struct Sprite *);
void MovementType_WalkSequenceLeftUpDownRight(struct Sprite *);
void MovementType_WalkSequenceUpDownRightLeft(struct Sprite *);
void MovementType_WalkSequenceRightLeftUpDown(struct Sprite *);
void MovementType_WalkSequenceDownRightLeftUp(struct Sprite *);
void MovementType_WalkSequenceRightUpDownLeft(struct Sprite *);
void MovementType_WalkSequenceUpDownLeftRight(struct Sprite *);
void MovementType_WalkSequenceLeftRightUpDown(struct Sprite *);
void MovementType_WalkSequenceDownLeftRightUp(struct Sprite *);
void MovementType_WalkSequenceUpLeftDownRight(struct Sprite *);
void MovementType_WalkSequenceDownRightUpLeft(struct Sprite *);
void MovementType_WalkSequenceLeftDownRightUp(struct Sprite *);
void MovementType_WalkSequenceRightUpLeftDown(struct Sprite *);
void MovementType_WalkSequenceUpRightDownLeft(struct Sprite *);
void MovementType_WalkSequenceDownLeftUpRight(struct Sprite *);
void MovementType_WalkSequenceLeftUpRightDown(struct Sprite *);
void MovementType_WalkSequenceRightDownLeftUp(struct Sprite *);
void MovementType_CopyPlayer(struct Sprite *);
void MovementType_TreeDisguise(struct Sprite *);
void MovementType_MountainDisguise(struct Sprite *);
void MovementType_CopyPlayerInGrass(struct Sprite *);
void MovementType_Hidden(struct Sprite *);
void MovementType_WalkInPlace(struct Sprite *);
void MovementType_JogInPlace(struct Sprite *);
void MovementType_RunInPlace(struct Sprite *);
void MovementType_Invisible(struct Sprite *);
void MovementType_WalkSlowlyInPlace(struct Sprite *);
u8 GetSlideMovementAction(u32);
u8 GetJumpInPlaceMovementAction(u32);
u8 CreateCopySpriteAt(struct Sprite *sprite, s16 x, s16 y, u8 subpriority);

u8 MovementType_WanderAround_Step0(struct EventObject *, struct Sprite *);
u8 MovementType_WanderAround_Step1(struct EventObject *, struct Sprite *);
u8 MovementType_WanderAround_Step2(struct EventObject *, struct Sprite *);
u8 MovementType_WanderAround_Step3(struct EventObject *, struct Sprite *);
u8 MovementType_WanderAround_Step4(struct EventObject *, struct Sprite *);
u8 MovementType_WanderAround_Step5(struct EventObject *, struct Sprite *);
u8 MovementType_WanderAround_Step6(struct EventObject *, struct Sprite *);
u8 GetVectorDirection(s16, s16, s16, s16);
u8 GetLimitedVectorDirection_SouthNorth(s16, s16, s16, s16);
u8 GetLimitedVectorDirection_WestEast(s16, s16, s16, s16);
u8 GetLimitedVectorDirection_WestNorth(s16, s16, s16, s16);
u8 GetLimitedVectorDirection_EastNorth(s16, s16, s16, s16);
u8 GetLimitedVectorDirection_WestSouth(s16, s16, s16, s16);
u8 GetLimitedVectorDirection_EastSouth(s16, s16, s16, s16);
u8 GetLimitedVectorDirection_SouthNorthWest(s16, s16, s16, s16);
u8 GetLimitedVectorDirection_SouthNorthEast(s16, s16, s16, s16);
u8 GetLimitedVectorDirection_NorthWestEast(s16, s16, s16, s16);
u8 GetLimitedVectorDirection_SouthWestEast(s16, s16, s16, s16);
u8 MovementType_LookAround_Step0(struct EventObject *, struct Sprite *);
u8 MovementType_LookAround_Step1(struct EventObject *, struct Sprite *);
u8 MovementType_LookAround_Step2(struct EventObject *, struct Sprite *);
u8 MovementType_LookAround_Step3(struct EventObject *, struct Sprite *);
u8 MovementType_LookAround_Step4(struct EventObject *, struct Sprite *);
u8 MovementType_WanderUpAndDown_Step0(struct EventObject *, struct Sprite *);
u8 MovementType_WanderUpAndDown_Step1(struct EventObject *, struct Sprite *);
u8 MovementType_WanderUpAndDown_Step2(struct EventObject *, struct Sprite *);
u8 MovementType_WanderUpAndDown_Step3(struct EventObject *, struct Sprite *);
u8 MovementType_WanderUpAndDown_Step4(struct EventObject *, struct Sprite *);
u8 MovementType_WanderUpAndDown_Step5(struct EventObject *, struct Sprite *);
u8 MovementType_WanderUpAndDown_Step6(struct EventObject *, struct Sprite *);
u8 MovementType_WanderLeftAndRight_Step0(struct EventObject *, struct Sprite *);
u8 MovementType_WanderLeftAndRight_Step1(struct EventObject *, struct Sprite *);
u8 MovementType_WanderLeftAndRight_Step2(struct EventObject *, struct Sprite *);
u8 MovementType_WanderLeftAndRight_Step3(struct EventObject *, struct Sprite *);
u8 MovementType_WanderLeftAndRight_Step4(struct EventObject *, struct Sprite *);
u8 MovementType_WanderLeftAndRight_Step5(struct EventObject *, struct Sprite *);
u8 MovementType_WanderLeftAndRight_Step6(struct EventObject *, struct Sprite *);
u8 MovementType_FaceDirection_Step0(struct EventObject *, struct Sprite *);
u8 MovementType_FaceDirection_Step1(struct EventObject *, struct Sprite *);
u8 MovementType_FaceDirection_Step2(struct EventObject *, struct Sprite *);
u8 MovementType_BerryTreeGrowth_Step0(struct EventObject *, struct Sprite *);
u8 MovementType_BerryTreeGrowth_Step1(struct EventObject *, struct Sprite *);
u8 MovementType_BerryTreeGrowth_Step2(struct EventObject *, struct Sprite *);
u8 MovementType_BerryTreeGrowth_Step3(struct EventObject *, struct Sprite *);
u8 MovementType_BerryTreeGrowth_Step4(struct EventObject *, struct Sprite *);
u8 MovementType_FaceDownAndUp_Step0(struct EventObject *, struct Sprite *);
u8 MovementType_FaceDownAndUp_Step1(struct EventObject *, struct Sprite *);
u8 MovementType_FaceDownAndUp_Step2(struct EventObject *, struct Sprite *);
u8 MovementType_FaceDownAndUp_Step3(struct EventObject *, struct Sprite *);
u8 MovementType_FaceDownAndUp_Step4(struct EventObject *, struct Sprite *);
u8 MovementType_FaceLeftAndRight_Step0(struct EventObject *, struct Sprite *);
u8 MovementType_FaceLeftAndRight_Step1(struct EventObject *, struct Sprite *);
u8 MovementType_FaceLeftAndRight_Step2(struct EventObject *, struct Sprite *);
u8 MovementType_FaceLeftAndRight_Step3(struct EventObject *, struct Sprite *);
u8 MovementType_FaceLeftAndRight_Step4(struct EventObject *, struct Sprite *);
u8 MovementType_FaceUpAndLeft_Step0(struct EventObject *, struct Sprite *);
u8 MovementType_FaceUpAndLeft_Step1(struct EventObject *, struct Sprite *);
u8 MovementType_FaceUpAndLeft_Step2(struct EventObject *, struct Sprite *);
u8 MovementType_FaceUpAndLeft_Step3(struct EventObject *, struct Sprite *);
u8 MovementType_FaceUpAndLeft_Step4(struct EventObject *, struct Sprite *);
u8 MovementType_FaceUpAndRight_Step0(struct EventObject *, struct Sprite *);
u8 MovementType_FaceUpAndRight_Step1(struct EventObject *, struct Sprite *);
u8 MovementType_FaceUpAndRight_Step2(struct EventObject *, struct Sprite *);
u8 MovementType_FaceUpAndRight_Step3(struct EventObject *, struct Sprite *);
u8 MovementType_FaceUpAndRight_Step4(struct EventObject *, struct Sprite *);
u8 MovementType_FaceDownAndLeft_Step0(struct EventObject *, struct Sprite *);
u8 MovementType_FaceDownAndLeft_Step1(struct EventObject *, struct Sprite *);
u8 MovementType_FaceDownAndLeft_Step2(struct EventObject *, struct Sprite *);
u8 MovementType_FaceDownAndLeft_Step3(struct EventObject *, struct Sprite *);
u8 MovementType_FaceDownAndLeft_Step4(struct EventObject *, struct Sprite *);
u8 MovementType_FaceDownAndRight_Step0(struct EventObject *, struct Sprite *);
u8 MovementType_FaceDownAndRight_Step1(struct EventObject *, struct Sprite *);
u8 MovementType_FaceDownAndRight_Step2(struct EventObject *, struct Sprite *);
u8 MovementType_FaceDownAndRight_Step3(struct EventObject *, struct Sprite *);
u8 MovementType_FaceDownAndRight_Step4(struct EventObject *, struct Sprite *);
u8 MovementType_FaceDownUpAndLeft_Step0(struct EventObject *, struct Sprite *);
u8 MovementType_FaceDownUpAndLeft_Step1(struct EventObject *, struct Sprite *);
u8 MovementType_FaceDownUpAndLeft_Step2(struct EventObject *, struct Sprite *);
u8 MovementType_FaceDownUpAndLeft_Step3(struct EventObject *, struct Sprite *);
u8 MovementType_FaceDownUpAndLeft_Step4(struct EventObject *, struct Sprite *);
u8 MovementType_FaceDownUpAndRight_Step0(struct EventObject *, struct Sprite *);
u8 MovementType_FaceDownUpAndRight_Step1(struct EventObject *, struct Sprite *);
u8 MovementType_FaceDownUpAndRight_Step2(struct EventObject *, struct Sprite *);
u8 MovementType_FaceDownUpAndRight_Step3(struct EventObject *, struct Sprite *);
u8 MovementType_FaceDownUpAndRight_Step4(struct EventObject *, struct Sprite *);
u8 MovementType_FaceUpLeftAndRight_Step0(struct EventObject *, struct Sprite *);
u8 MovementType_FaceUpLeftAndRight_Step1(struct EventObject *, struct Sprite *);
u8 MovementType_FaceUpLeftAndRight_Step2(struct EventObject *, struct Sprite *);
u8 MovementType_FaceUpLeftAndRight_Step3(struct EventObject *, struct Sprite *);
u8 MovementType_FaceUpLeftAndRight_Step4(struct EventObject *, struct Sprite *);
u8 MovementType_FaceDownLeftAndRight_Step0(struct EventObject *, struct Sprite *);
u8 MovementType_FaceDownLeftAndRight_Step1(struct EventObject *, struct Sprite *);
u8 MovementType_FaceDownLeftAndRight_Step2(struct EventObject *, struct Sprite *);
u8 MovementType_FaceDownLeftAndRight_Step3(struct EventObject *, struct Sprite *);
u8 MovementType_FaceDownLeftAndRight_Step4(struct EventObject *, struct Sprite *);
u8 MovementType_RotateCounterclockwise_Step0(struct EventObject *, struct Sprite *);
u8 MovementType_RotateCounterclockwise_Step1(struct EventObject *, struct Sprite *);
u8 MovementType_RotateCounterclockwise_Step2(struct EventObject *, struct Sprite *);
u8 MovementType_RotateCounterclockwise_Step3(struct EventObject *, struct Sprite *);
u8 MovementType_RotateClockwise_Step0(struct EventObject *, struct Sprite *);
u8 MovementType_RotateClockwise_Step1(struct EventObject *, struct Sprite *);
u8 MovementType_RotateClockwise_Step2(struct EventObject *, struct Sprite *);
u8 MovementType_RotateClockwise_Step3(struct EventObject *, struct Sprite *);
u8 MovementType_WalkBackAndForth_Step0(struct EventObject *, struct Sprite *);
u8 MovementType_WalkBackAndForth_Step1(struct EventObject *, struct Sprite *);
u8 MovementType_WalkBackAndForth_Step2(struct EventObject *, struct Sprite *);
u8 MovementType_WalkBackAndForth_Step3(struct EventObject *, struct Sprite *);
u8 MovementType_WalkSequence_Step0(struct EventObject *, struct Sprite *);
u8 MovementType_WalkSequence_Step2(struct EventObject *, struct Sprite *);
u8 MovementType_WalkSequenceUpRightLeftDown_Step1(struct EventObject *, struct Sprite *);
u8 MovementType_WalkSequenceRightLeftDownUp_Step1(struct EventObject *, struct Sprite *);
u8 MovementType_WalkSequenceDownUpRightLeft_Step1(struct EventObject *, struct Sprite *);
u8 MovementType_WalkSequenceLeftDownUpRight_Step1(struct EventObject *, struct Sprite *);
u8 MovementType_WalkSequenceUpLeftRightDown_Step1(struct EventObject *, struct Sprite *);
u8 MovementType_WalkSequenceLeftRightDownUp_Step1(struct EventObject *, struct Sprite *);
u8 MovementType_WalkSequenceDownUpLeftRight_Step1(struct EventObject *, struct Sprite *);
u8 MovementType_WalkSequenceRightDownUpLeft_Step1(struct EventObject *, struct Sprite *);
u8 MovementType_WalkSequenceLeftUpDownRight_Step1(struct EventObject *, struct Sprite *);
u8 MovementType_WalkSequenceUpDownRightLeft_Step1(struct EventObject *, struct Sprite *);
u8 MovementType_WalkSequenceRightLeftUpDown_Step1(struct EventObject *, struct Sprite *);
u8 MovementType_WalkSequenceDownRightLeftUp_Step1(struct EventObject *, struct Sprite *);
u8 MovementType_WalkSequenceRightUpDownLeft_Step1(struct EventObject *, struct Sprite *);
u8 MovementType_WalkSequenceUpDownLeftRight_Step1(struct EventObject *, struct Sprite *);
u8 MovementType_WalkSequenceLeftRightUpDown_Step1(struct EventObject *, struct Sprite *);
u8 MovementType_WalkSequenceDownLeftRightUp_Step1(struct EventObject *, struct Sprite *);
u8 MovementType_WalkSequenceUpLeftDownRight_Step1(struct EventObject *, struct Sprite *);
u8 MovementType_WalkSequenceDownRightUpLeft_Step1(struct EventObject *, struct Sprite *);
u8 MovementType_WalkSequenceLeftDownRightUp_Step1(struct EventObject *, struct Sprite *);
u8 MovementType_WalkSequenceRightUpLeftDown_Step1(struct EventObject *, struct Sprite *);
u8 MovementType_WalkSequenceUpRightDownLeft_Step1(struct EventObject *, struct Sprite *);
u8 MovementType_WalkSequenceDownLeftUpRight_Step1(struct EventObject *, struct Sprite *);
u8 MovementType_WalkSequenceLeftUpRightDown_Step1(struct EventObject *, struct Sprite *);
u8 MovementType_WalkSequenceRightDownLeftUp_Step1(struct EventObject *, struct Sprite *);
u8 MovementType_CopyPlayer_Step0(struct EventObject *, struct Sprite *);
u8 MovementType_CopyPlayer_Step1(struct EventObject *, struct Sprite *);
u8 MovementType_CopyPlayer_Step2(struct EventObject *, struct Sprite *);
bool8 CopyablePlayerMovement_None(struct EventObject *, struct Sprite *, u8, bool8(u8));
bool8 CopyablePlayerMovement_FaceDirection(struct EventObject *, struct Sprite *, u8, bool8(u8));
bool8 CopyablePlayerMovement_GoSpeed0(struct EventObject *, struct Sprite *, u8, bool8(u8));
bool8 CopyablePlayerMovement_GoSpeed1(struct EventObject *, struct Sprite *, u8, bool8(u8));
bool8 CopyablePlayerMovement_GoSpeed2(struct EventObject *, struct Sprite *, u8, bool8(u8));
bool8 CopyablePlayerMovement_Slide(struct EventObject *, struct Sprite *, u8, bool8(u8));
bool8 cph_IM_DIFFERENT(struct EventObject *, struct Sprite *, u8, bool8(u8));
bool8 CopyablePlayerMovement_GoSpeed4(struct EventObject *, struct Sprite *, u8, bool8(u8));
bool8 CopyablePlayerMovement_Jump(struct EventObject *, struct Sprite *, u8, bool8(u8));
u8 MovementType_CopyPlayerInGrass_Step1(struct EventObject *, struct Sprite *);
u8 MovementType_Hidden_Step0(struct EventObject *, struct Sprite *);
u8 MovementType_WalkInPlace_Step0(struct EventObject *, struct Sprite *);
u8 MovementType_MoveInPlace_Step1(struct EventObject *, struct Sprite *);
u8 MovementType_WalkSlowlyInPlace_Step0(struct EventObject *, struct Sprite *);
u8 MovementType_JogInPlace_Step0(struct EventObject *, struct Sprite *);
u8 MovementType_RunInPlace_Step0(struct EventObject *, struct Sprite *);
u8 MovementType_Invisible_Step0(struct EventObject *, struct Sprite *);
u8 MovementType_Invisible_Step1(struct EventObject *, struct Sprite *);
u8 MovementType_Invisible_Step2(struct EventObject *, struct Sprite *);
*/
#endif //GUARD_FIELD_EVENT_OBJ_H
