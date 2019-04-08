.thumb
.text
.align 2

.include "..\\defines"

@Repoint at 0x803FE70
GetMonPokeBallHook:
	mov r0, r9
	bl GetMonPokeBall
	mov r4, r0
	ldr r1, =0x804035C | 1
	bx r1

@Hook at 0x8040858 with r0
SetMonPokeBallHook:
	mov r0, r8 @Substruct0
	ldrb r1, [r4] @Ball To Set
	bl SetMonPokeBall
	ldr r0, =0x8040ADA | 1
	bx r0

@Hook at 0x8031E40 with r0
PlayerHandleFormChange:
	bl HandleFormChange
	ldr r0, =0x8031E56 | 1
	bx r0

@Hook at 0x08037760 with r0
OpponentHandleFormChange:
	bl HandleFormChange
	ldr r0, =0x8037774 | 1
	bx r0

@Hook at 0x0803C318 with r0
LinkOpponentHandleFormChange:
	bl HandleFormChange
	ldr r0, =0x803C32C | 1
	bx r0

