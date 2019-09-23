.thumb
.text
.align 2
/*
FollowMe_SetStateHook:
	mov r1, r5
	mov r5, r0
	mov r0, r4
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
	bl ledge_fun

dont_move_oam:
	ldr r2, =(0x08068D6E + 1)
	bx r2

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
FollowMe_CollisionHook:
	push {r1-r3}
	mov r0, r2
	mov r1, r6
	bl FollowMe_CollisionExempt
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
bike_hook:
	bl FollowMe_HandleBike
	pop {r4, pc}

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
@Surf hook, make follower jump into water as well
@hook via R0, at 0x8086B00. Write 00 00 to 0x8086AFE
FollowMe_SurfHook:
	ldr r0, =(FollowerToWater)
	bl linker
	ldr r1, =(0x20386E0)
	mov r2, #0xA
	ldrsh r0, [r6, r2]
	str r0, [r1]
	mov r2, #0xC
	ldrsh r0,[r6, r2]
	str r0, [r1, #0x4]
	ldr r0, =(0x8086B0C +1)

linker:
	bx r0

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
@Surf hook, startmenu/bag access
@hook via R0, at 0x8086B00. Write 00 00 to 0x8086AFE
FollowMe_SurfBagHook:
	push {r4, r5, lr}
	ldr r0, =(FollowerNoMoveSurf)
	bl linker
	ldr r5, =(0x2037078)
	ldrb r1, [r5, #0x5]
	lsl r0, r1, #0x3
	add r0, r0, r1
	lsl r0, r0, #0x2
	ldr r1, =(0x2036E38)
	add r4, r0, r1
	ldr r0, =(0x8120718 +1)

linker:
	bx r0

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
@WarpInHook
FollowMe_CreateAvatarHook1:
	bl CreateFollowerAvatar
	mov r9, r4
	pop {r4-r6}
	pop {r0}
	bx r0

FollowMe_CreateAvatarHook2:
	bl FollowMe_HandleSprite
	add sp, #4
	pop {r4-r5, pc}

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
FollowMe_StairsMoveHook:
	strh r0, [r7, #0x26]
	mov r0, r7
	bl StairsMoveFollower

@return:
	mov r1, r8
	ldrb r0, [r1]
	lsr r0, #7
	ldr r1, =(0x0807EB30 + 1)
	bx r1

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

FollowMe_WarpDoorEndHook:
	bl FollowMe_WarpSetEnd
	ldr r3, =0x08068A5C | 1
	bl call_via_r3
	ldr r3, =0x0806994C | 1
	bl call_via_r3
	ldr r0, =0x0807E200 | 1
	bx r0

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
FollowMe_WarpNormalEndHook:
	bl FollowMe_WarpSetEnd
	ldr r3, =0x08068A5C | 1
	bl call_via_r3
	ldr r3, =0x0806994C | 1
	bl call_via_r3
	ldr r0, =0x0807E310 | 1
	bx r0

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

FollowMe_WarpStairsEndHook:
	bl FollowMe_WarpSetEnd
	ldr r3, =0x0805FAA8 | 1
	bl call_via_r3
	ldr r3, =0x0806994C | 1
	bl call_via_r3
	mov r0, r4
	ldr r1, =0x0807EC64 | 1
	bx r1

call_via_r3:
	bx r3

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

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
	bl GetFollowerLocalId
	ldr r3, =(0x0805DF7C + 1)
	bx r3
*/
