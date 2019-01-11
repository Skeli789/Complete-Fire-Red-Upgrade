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
