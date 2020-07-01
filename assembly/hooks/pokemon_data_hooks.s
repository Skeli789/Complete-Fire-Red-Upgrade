.thumb
.text
.align 2

.include "../asm_defines.s"

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

@Hook at 0x08040784 with r2
SetMonHeldItemHook:
	lsl r0, #0x8
	add r1, r0
	mov r0, r8 @Substruct0
	mov r2, r7 @struct Pokemon
	bl SetMonHeldItem
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

@Hook at 0x0803DAD2 with r0
CreateBoxMonHook:
	ldr r0, [sp, #0x40 + 0x0] @hasFixedPersonality
	str r0, [sp, #0x0]
	ldr r0, [sp, #0x40 + 0x4] @fixedPersonality
	str r0, [sp, #0x4]
	ldr r0, [sp, #0x40 + 0x8] @otIdType
	str r0, [sp, #0x8]
	ldr r0, [sp, #0x40 + 0xC] @fixedOtId
	str r0, [sp, #0xC]
	mov r0, r7
	bl CreateBoxMon
	ldr r0, =0x803DD84 | 1
	bx r0

@Repoint at 0x803FDEC
CheckHasSpeciesJump:
	mov r0, r8
	bl GetAlternateHasSpecies
	mov r4, r0
	ldr r1, =0x8040368 | 1
	bx r1


CheckRegularMonDataReturn:
	ldr r1, =0x803FDC2 | 1
	bx r1

