.text
.align 2
.thumb
.thumb_func

.include "../asm_defines.s"

@@ Move Reminder
.global FixReminderCalloc
.global FixNumMovesLimiter
.global FixMultiUseTemplate
.global FixLoadMoveId1
.global FixLoadMoveId2
.global FixLoadMoveId3
.global FixLoadMoveId4
.global FixLoadMoveId5
.global FixMoveNameLoading
.global FixMoveReminderDataLoading
.global FixWindowTemplates

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Move Reminder - fix move to load data from
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
FixMoveReminderDataLoading:
	bl GetMoveIdFromRelearnerStruct
	ldr r1, =(0x080E54D8 +1)
	bx r1


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Move Reminder - Fix Move Name Loading
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
FixMoveNameLoading:
	mov r0, r3	@cursor pos
	bl CopyMoveReminderMoveName
	ldr r1, =(0x080E5250 +1)
	bx r1


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Move Reminder - Fix relearnable moves counter
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
FixNumRelearnableMoves:
	push {r4-r7}
	bl GetRelearnableMoves
	pop {r4-r7}
	mov r12, r0		@total list num count
	mov r8, r0
	ldr r2, =(0x080E51AA +1)
	bx r2

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Move Reminder - Fix Window Templates
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
FixWindowTemplates:
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


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Move Reminder Calloc
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
@ E47A4 via r0
FixReminderCalloc:
	bl InitLearnMoveFix
	ldr r0, =(0x080E47AC +1)
	bx r0

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Move Reminder - Fix Move Id Loading
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
FixLoadMoveId1:
	push {r0, r2-r7}
	ldrb r0, [r1]	@cursor pos
	bl GetMoveIdFromRelearnerStruct
	mov r1, r0
	pop {r0, r2-r7}
	ldr r2, =(0x080E4AAA +1)
	bx r2

.align 2
.pool
FixLoadMoveId2:
	push {r0-r3, r5-r7}
	ldrb r0, [r4]	@ cursor pos
	bl GetMoveIdFromRelearnerStruct
	mov r4, r0
	pop {r0-r3,r5-r7}
	ldr r6, =(0x080E4CBC +1)
	bx r6

.align 2
.pool
FixLoadMoveId3:
	push {r0, r3-r7}	@gMoveRelearnerStruct
	mov r0, r2
	bl GetMoveIdFromRelearnerStruct
	mov r1, r0
	pop {r0, r3-r7}
	ldr r2, =(0x080E4E14 +1)
	bx r2

.align 2
.pool
FixLoadMoveId4:
	mov r0, r1
	bl GetMoveIdFromRelearnerStruct
	mov r1, r0
	ldr r2, =(0x080E4E2E +1)
	bx r2

.align 2
.pool
FixLoadMoveId5:
	bl InitMoveRelearnerMoveIDs
	ldr r4, =(0x080E4F9C +1)
	bx r4
