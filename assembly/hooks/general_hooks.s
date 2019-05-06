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

@0x80F1792 with r0
ShinyAnimFixHook:
	bl ShinyAnimFix
	ldr r0, =0x80F179E | 1
	bx r0

@0x8014450 with r0
PartyMenuSwitchingHook:
	bl PartyMenuSwitchingUpdate
	ldr r0, =0x80146AC | 1
	bx r0

@0x80EFF0C with r2
CriticalCaptureAnimHook:
	push {r0-r1}
	bl CriticalCapturAnimUpdate
	cmp r0, #0x0
	pop {r0-r1}
	bne BallThrowSuccessReturn
	cmp r0, #0x4
	bne BallThrowBreakOutReturn
	cmp r1, #0x3
	bne BallThrowBreakOutReturn

BallThrowSuccessReturn:
	ldr r0, =0x80EFF14 | 1
	bx r0

BallThrowBreakOutReturn:
	ldr r0, =0x80EFF20 | 1
	bx r0

@0x80EF5F4 with r1
DoubleWildAnimBallThrowFixHook:
	strh r0, [r6, #0x2E]
	bl DoubleWildAnimBallThrowFix
	ldr r2, =ANIM_TARGET
	mov r8, r2
	ldrb r0, [r2]
	ldr r1, =0x80EF5FC | 1
	bx r1

@0x80A1E2C with r0
DoubleWildPokeBallItemUseFixHook:
	mov r0, r4
	bl DoubleWildPokeBallItemUseFix
	cmp r0, #0x0
	bne DoubleWildPokeBallItemUseFixEnd
	
	ldr r0, =0x203AD30 @Var 0x800E
	ldrh r0, [r0]
	mov r1, #0x1
	bl RemoveBagItem
	ldr r0, =0x80A1E36 | 1
	bx r0

DoubleWildPokeBallItemUseFixEnd:
	pop {r4, pc}

RemoveBagItem:
	ldr r3, =0x809A1D8 | 1
	bx r3

@0x802D95C with r0
DoubleWildDexHook1:
	push {r4-r5,lr}
	bl LoadTargetPartyData
	mov r4, r0
	mov r1, #0xB
	ldr r2, =0x802D964 | 1
	bx r2

@0x802D9D8 with r0
DoubleWildDexHook2:
	push {r4-r5,lr}
	sub sp, #0x18
	bl LoadTargetPartyData
	mov r1, #0xB
	ldr r2, =0x802D9E0 | 1
	bx r2

@0x8013D14 with r1
TrainerSlidingEndTurnHook:
	mov r1, r10
	str r0, [r1]
	bl CheckLastMonLowHPSlide
	bl Random
	ldr r1, =0x8013D1C | 1
	bx r1

Random:
	ldr r0, =0x8044EC8 | 1
	bx r0
	
@0x80813B8 with r0
LoadProperMusicForLinkBattlesHook:
	bl LoadProperMusicForLinkBattles
	ldr r1, =0x80813C6 | 1
	bx r1
	
@0x806E454 with r1
ExpandedVarsHook:
	push {r4-r6, lr}
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	mov r6, r4
	mov r0, r4
	bl GetExpandedVarPointer
	cmp r0, #0x1
	beq VarReturn0
	cmp r0, #0x0
	bne ExpandedVarsPop
	ldr r0, =0x806E45C | 1
	bx r0
	
ExpandedVarsPop:
	pop {r4-r6,pc}
	
VarReturn0:
	ldr r0, =0x806E462 | 1
	bx r0

.pool
@0x806E5C0 with r1
ExpandedFlagsHook:
	push {r4-r6, lr}
	lsl r5, r0, #0x10
	lsr r4, r5, #0x10
	mov r6, r4
	mov r0, r4
	bl GetExpandedFlagPointer
	cmp r0, #0x0
	bne ExpandedVarsPop
	ldr r0, =0x806E5C8 | 1
	bx r0

.pool
@0x80142D8 with r0
DoubleWildUseItemRunFixHook:
	ldr r5, =ACTIVE_BATTLER
	ldrb r4, [r5]
	bl CheckCantMoveThisTurn
	cmp r0, #0x0
	beq SkipEncoreReturn
	ldr r0, =0x801460E | 1
	bx r0

SkipEncoreReturn:
	ldr r0, =0x801432C | 1
	bx r0

.pool
@0x8054B44 with r0
NewGameSaveClearHook:
	bl NewGameWipeNewSaveData
	pop {r3}
	mov r8, r3
	pop {r4-r6}
	pop {r0}
	bx r0

.pool
@0x80CEDD4 with r1
EvolutionMovesHook:
	ldrb r1, [r5, #0x10]
	mov r0, r9
	bl MonTryLearningNewMoveAfterEvolution
	ldr r1, =0x80CEDDC | 1
	bx r1

.pool
@0x81058DC with r0
PokedexLoadAlternateHeightHook:
	lsr r7, r2, #0x18
	lsl r3, r3, #0x18
	lsr r3, r3, #0x18
	mov r8, r3
	mov r5, r1 @Save species
	mov r0, r1
	mov r1, #0x0 @PKDX_GET_HEIGHT
	bl TryGetAlternateSpeciesSize
	cmp r0, #0x0
	bne ReturnPokedexHeight
	mov r0, r5
	ldr r1, =0x81058E6 | 1
	bx r1

ReturnPokedexHeight:
	mov r4, r0 @Store height in r4
	mov r0, r5 @Store species in r0
	ldr r1, =SpeciesToNationalPokedexNum
	bl bxr1
	ldr r1, =0x81058F6 | 1
	bx r1

.pool
@0x8105A58 with r0
PokedexLoadAlternateWeightHook:
	lsl r3, r3, #0x18
	lsr r3, r3, #0x18
	mov r9, r3
	mov r5, r1 @Save species
	mov r0, r1
	mov r1, #0x1 @PKDX_GET_WEIGHT
	bl TryGetAlternateSpeciesSize
	cmp r0, #0x0
	bne ReturnPokedexWeight
	mov r0, r5
	ldr r1, =0x8105A60 | 1
	bx r1

ReturnPokedexWeight:
	mov r4, r0 @Store weight in r4
	mov r0, r5 @Store species in r0
	ldr r1, =SpeciesToNationalPokedexNum
	bl bxr1
	ldr r1, =0x8105A70 | 1
bxr1:
	bx r1
.pool

