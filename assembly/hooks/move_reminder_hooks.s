.thumb
.text
.thumb_func
.align 2

.include "../asm_defines.s"

@@ Move Reminder

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Move Reminder - fix move to load data from
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
MoveReminderHook_FixDataLoading:
	bl GetMoveIdFromRelearnerStruct
	ldr r1, =(0x080E54D8 +1)
	bx r1


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Move Reminder - Fix Move Name Loading
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
MoveReminderHook_FixMoveNameLoading:
	mov r0, r3	@cursor pos
	bl CopyMoveReminderMoveName
	ldr r1, =(0x080E5250 +1)
	bx r1


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Move Reminder - Fix relearnable moves counter
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
MoveReminderHook_FixNumRelearnableMoves:
	push {r4-r7}
	bl GetRelearnableMoves
	pop {r4-r7}
	mov r12, r0		@total list num count
	mov r8, r0
	ldr r2, =(0x080E51AA +1)
bxr2:
	bx r2

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Move Reminder - Fix Window Templates and Palette
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
MoveReminderHook_FixWindowTemplates:
	push {r1-r7}
	bl InitMoveRelearnerWindows
	pop {r1-r7}
	lsl r0, r0, #0x10
	cmp r0, #0x0
	beq ExitMoveRelearnGuiInit
	ldr r1, =(0x080E46C0 +1)
	bx r1
ExitMoveRelearnGuiInit:
	ldr r1, =(0x80E476C +1)
	bx r1

.pool
@0x80E4760 with r2
MoveReminderHook_FixBgPalette:
	ldr r2, =SetGpuReg
	bl bxr2
	mov r0, #0x14
	mov r1, #0x0
	ldr r2, =SetGpuReg
	bl bxr2
	bl HideMoveReminderBg1Palette
	ldr r0, =0x80E476C | 1
	bx r0

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Move Reminder Calloc
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
@ E47A4 via r0
MoveReminderHook_FixCalloc:
	bl InitLearnMoveFix
	ldr r0, =(0x080E47AC +1)
	bx r0

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Move Reminder - Fix Move Id Loading
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
MoveReminderHook_FixLoadMoveId1:
	push {r0, r2-r7}
	ldrb r0, [r1]	@cursor pos
	bl GetMoveIdFromRelearnerStruct
	mov r1, r0
	pop {r0, r2-r7}
	ldr r2, =(0x080E4AAA +1)
	bx r2

.pool
MoveReminderHook_FixLoadMoveId2:
	push {r0-r3, r5-r7}
	ldrb r0, [r4]	@ cursor pos
	bl GetMoveIdFromRelearnerStruct
	mov r4, r0
	pop {r0-r3,r5-r7}
	ldr r6, =(0x080E4CBC +1)
	bx r6

.pool
MoveReminderHook_FixLoadMoveId3:
	push {r0, r3-r7}	@gMoveRelearnerStruct
	mov r0, r2
	bl GetMoveIdFromRelearnerStruct
	mov r1, r0
	pop {r0, r3-r7}
	ldr r2, =(0x080E4E14 +1)
	bx r2

.pool
MoveReminderHook_FixLoadMoveId4:
	mov r0, r1
	bl GetMoveIdFromRelearnerStruct
	mov r1, r0
	mov r2, r9
	ldr r3, =(0x080E4E30 +1)
	bx r3

.pool
MoveReminderHook_FixLoadMoveId5:
	bl InitMoveRelearnerMoveIDs
	ldr r4, =(0x080E4F9C +1)
	bx r4

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Move Reminder - Add PSS Icon
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
@0x80E5434 with r0
MoveReminderHook_AddSplitIcon:
	mov r0, r5 @Move
	bl PrintMoveReminderSplitIcon
	pop {r4, r5, pc}
