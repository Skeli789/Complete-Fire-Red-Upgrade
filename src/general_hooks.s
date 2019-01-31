.text
.thumb
.align 2

.include "..\\defines"

.global BuildTrainerPartyHook

BuildTrainerPartyHook:
	bl BuildTrainerPartySetup
	ldr r0, =0x800FF98 | 1
	bx r0

@0x800D2CC with r0
AIHandleItemUseHook:
	bl HandleNewBattleRamClearBeforeBattle
	bl SavePartyItems
	mov r0, #0xF
	bl BattleAI_HandleItemUseBeforeAISetup
	ldr r0, =0x2022B54
	str r4, [r0]
	ldr r0, =0x800D2D4 | 1
	bx r0
	
EmitChooseMoveHook:
	mov r3, r5
	bl EmitChooseMove
	pop {r4-r6,pc}

@0x802EF90 with r0
SwitchMoveDataHook:
	bl HandleMoveSwitchingUpdate
	ldr r0, =0x802F2C8 | 1
	bx r0

@0x800E3A4 with r0
EmitPrintStringHook:
	bl EmitPrintStringUpdate
	pop {r3-r5}
	mov r8, r3
	mov r9, r4
	mov r10, r5
	pop {r4-r7, pc}

@0x8032AFC with r0
PlayerHandlePrintStringHook:
	push {r4, lr}
	bl PlayerHandlePrintStringUpdate
	ldr r0, =0x8032B08 | 1
	bx r0
