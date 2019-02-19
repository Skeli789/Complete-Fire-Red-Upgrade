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

@0x80992E4 with r0
SlideMonToOffsetHook:
	mov r0, #0x0
	bl LoadBattleAnimTarget
	mov r2, r0
	ldr r0, =0x8099300 | 1
	bx r0

@0x80991BA with r0
SlideMonToOriginalPosHook:
	mov r0, #0x0
	bl LoadBattleAnimTarget
	ldr r1, =0x2023D44 @BattlerSpriteIds
	ldr r2, =0x80991DE | 1
	bx r2

@0x0804AA1E with r1
GetProperBallIdHook1:
	bl BallIdToItemId
	bl ItemIdToBallId
	lsl r0, #0x18
	lsr r0, #0x18
	mov r5, r0
	ldr r1, =0x804AA28 | 1
	bx r1

@0x080EF3A8 with r1
GetProperBallIdHook2:
	bl BallIdToItemId
	bl ItemIdToBallId
	lsl r0, #0x18
	lsr r0, #0x18
	ldr r1, =0x80EF3B0 | 1
	bx r1

@Summary Screen Poke Ball Hook
@0x08139CDA with r1
GetProperBallIdHook3:
	bl BallIdToItemId
	bl ItemIdToBallId
	mov r4, r0
	lsl r4, #0x18
	lsr r4, #0x18
	ldr r1, =0x8139CE4 | 1
	bx r1
