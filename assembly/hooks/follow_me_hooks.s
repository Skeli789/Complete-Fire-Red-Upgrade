.thumb
.text
.align 2

FollowMe_SetStateHook:
	mov r1, r5
	mov r5, r0
	mov r0, r4
	push {r0-r1}
	bl ShouldFollowerIgnoreActiveScript
	mov r2, r0
	pop {r0-r1}
	bl FollowMe
	mov r0, r5
	pop {r4, r5}
	pop {r1}
	bx r1

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
FollowMe_LedgeHook:
	mov r3, r6 		@ npc_state pointer from parent function
	mov r6, #0
	mov r1, #0x36
	ldrsh r0, [r4, r1]
	cmp r0, #0
	beq dont_move_oam

	mov r0, r3		@ npc
	mov r1, r4		@ obj
	mov r2, sp 		@ ledge jump frames
	bl FollowMe_Ledges

dont_move_oam:
	ldr r2, =(0x08068D6E + 1)
bxr2:
	bx r2

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
FollowMe_CollisionHook:
	push {r1-r3}
	mov r0, r2
	mov r1, r6
	bl FollowMe_IsCollisionExempt
	pop {r1-r3}
	cmp r0, #0
	beq check_for_collision

no_collision:
	ldr r0, =(0x0806396C + 1)
	bx r0

check_for_collision:
	mov r0, #0x10
	ldrsh r1, [r2, r0]
	lsl r0, r7, #0x10
	asr r3, r0, #0x10
	ldr r0, =(0x08063930 + 1)
	bx r0

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
@0x80A1330 with r0
FollowMe_BikeHook:
	bl FollowMe_HandleBike
	pop {r4, pc}

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
@Surf hook, make follower jump into water as well
@hook via R0, at 0x8086B00
FollowMe_SurfHook:
	ldr r0, =FollowMe_FollowerToWater
	bl bxr0
	ldr r1, =gFieldEffectArguments
	mov r2, #0xA
	ldrsh r0, [r6, r2]
	str r0, [r1]
	mov r2, #0xC
	ldrsh r0, [r6, r2]
	str r0, [r1, #0x4]
	ldr r0, =(0x8086B0C +1)

bxr0:
	bx r0

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
@Surf hook, startmenu/bag access
@hook via R0, at 0x8056D12.
FollowMe_SurfBagHook:
	ldr r0, =0x8057100 | 1
	bl bxr0
	ldr r0, =0x8057114 | 1
	bl bxr0
	ldr r0, =FollowMe_BindToSurbBlobOnReloadScreen
	bl bxr0
	ldr r0, =0x8056D30 | 1
	bx r0

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
@hook via R0, at 0x805D140.
FollowMe_DismountSurf:
	bl PrepareFollowerDismountSurf
	pop {r4-r5,pc}

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
FollowMe_CreateAvatarHook2:
	bl FollowMe_HandleSprite
	add sp, #4
	pop {r4-r5, pc}

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
FollowMe_StairsMoveHook:
	strh r0, [r7, #0x26]
	bl StairsMoveFollower

@return:
	mov r1, r8
	ldrb r0, [r1]
	lsr r0, #7
	ldr r1, =(0x0807EB30 + 1)
	bx r1

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
@0x8084868 with r2
FollowMe_EscalatorMoveHook:
	strh r0, [r5, #0x8]
	strh r1, [r5, #0xC]
	strh r1, [r5, #0xE]
	ldrb r0, [r5, #0xA]
	bl EscalatorMoveFollower
	ldrb r0, [r5, #0xA]
	ldr r1, =0x08084870 + 1
	bx r1

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
@0x8084ACC with r0
FollowMe_EscalatorWarpEndHook:
	bl EscalatorMoveFollowerFinish
	mov r0, #0x1
	add sp, #0x4
	pop {r4-r5, pc}

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
FollowMe_WarpArrowEndHook:
	bl FollowMe_WarpSetEnd
	ldr r3, =UnfreezeEventObjects
	bl bxr3
	ldr r3, =ScriptContext2_Disable
	bl bxr3
	ldr r0, =0x807E2C0 | 1
	bx r0

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
@Used to be 0807E308
@Now is 0807DF74
FollowMe_WarpNormalEndHook:
	bl FollowMe_WarpSetEnd
	ldr r3, =ScriptContext2_Enable
	bl bxr3
	pop {pc}

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
FollowMe_WarpTeleportEndHook:
	bl FollowMe_WarpSetEnd
	ldr r3, =UnfreezeEventObjects
	bl bxr3
	ldr r3, =ScriptContext2_Disable
	bl bxr3
	ldr r0, =0x0807E36C | 1
	bx r0

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
FollowMe_WarpHoleEndHook:
	bl FollowMe_WarpSetEnd
	mov r0, #0x0
	add sp, #0x4
	pop {r4, pc}

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
FollowMe_WarpStairsEndHook:
	bl FollowMe_WarpSetEnd
	ldr r3, =0x0805FAA8 | 1
	bl bxr3
	ldr r3, =0x0806994C | 1
	bl bxr3
	mov r0, r4
	ldr r1, =0x0807EC64 | 1
	bx r1

bxr3:
	bx r3

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
FollowMe_LocalIdHook:
	push {lr}
	lsl r0, #0x18
	lsr r0, #0x18
	lsl r1, #0x18
	cmp r0, #0xFE		@ hijack call with 0xFE
	beq local_id_return
	ldr r3, =(0x0805DF68 + 1)
	bx r3

local_id_return:
	bl GetFollowerObjectId
	ldr r3, =(0x0805DF7C + 1)
	bx r3

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
FollowMe_ScriptHook:
	push {lr}
	lsl r0, #0x18
	lsr r0, #0x18
	lsl r1, #0x18
	push {r0-r2}
	bl GetFollowerLocalId
	mov r3, r0
	pop {r0-r2}
	cmp r3, #0x0
	beq GetRegularScript
	cmp r0, r3
	bne GetRegularScript @Not follower id
	bl GetFollowerScriptPointer
	pop {pc}

GetRegularScript:
	ldr r3, =0x805FC28 | 1
	bx r3

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
@0x8054E1A with r2
FollowMe_WhiteOutHook:
	ldr r2, =VarSet
	bl bxr2
	bl FollowMe_TryRemoveFollowerOnWhiteOut
	pop {pc}

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
@0x80A139C with r1
FollowMe_FishingHook:
	lsr r6, r0, #0x10
	lsl r0, r6, #0x18
	lsr r5, r0, #0x18
	ldrh r0, [sp] @;X
	lsl r0, #0x10
	lsr r0, #0x10
	ldrh r1, [r4] @;Y
	lsl r1, #0x10
	lsr r1, #0x10
	bl IsFollowerAtCoords
	cmp r0, #0x0
	bne FollowMe_FishingHook_NoFishing
	mov r0, r5
	ldr r1, =0x80A13A4 | 1
	bx r1

FollowMe_FishingHook_NoFishing:
	ldr r0, =0x80A1404 | 1
	bx r0
