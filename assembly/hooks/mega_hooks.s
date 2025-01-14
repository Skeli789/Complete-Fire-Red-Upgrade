.thumb
.text
.align 2

.include "../asm_defines.s"

.global MegaRetrieveDataHook

.pool
@;0x148C0 with r0
MegaRetrieveDataHook:
	push {r3}
	bl MegaRetrieveData
	pop {r3}
	ldr r0, =BATTLE_COMMUNICATION
	ldrb r1, [r3]
	ldr r2, =0x8014B30 | 1
	bx r2

LoadMegaGraphicsHook:
	push {r0}
	mov r0, r5
	bl LoadMegaGraphics
	pop {r0}

	pop {r4-r6}
	pop {r1}
	bx r1

.pool
@;0x80483A4 with r0
MegaLevelStringHook:
	lsl r4, #0x18
	lsr r4, #0x18

	@ Get owner of the health bar
	ldr r2, objects
	lsl r1, r5, #4
	add r1, r5
	lsl r1, r1, #2
	add r1, r2
	ldrh r0, [r1, #0x3A]
	bl HasMegaSymbol
	cmp r0, #0
	bne LoadSpecialMegaSymbol
	ldr r2, objects
	lsl r1, r5, #4
	add r1, r5
	lsl r1, r1, #2
	add r1, r2
	ldrh r0, [r1, #0x3A]
	bl HasDynamaxSymbol
	cmp r0, #0
	bne LoadSpecialMegaSymbol

	ldr r1, level_string
	mov r0, sp
	mov r2, #0x10
	bl MemCopy
	mov r0, sp
	add r0, #2

	mov r1, r4
	mov r2, #0
	mov r3, #3
	bl int_to_str

	mov r1, r0
	sub r1, #2
	b LevelStringReturn

LoadSpecialMegaSymbol:
	adr r1, special_string
	mov r0, sp
	mov r2, #0x10
	bl MemCopy
	mov r0, sp
	add r0, #0

	mov r1, r4
	mov r2, #0
	mov r3, #3
	bl int_to_str

	mov r1, r0
	sub r1, #2

	@
	mov r0, sp
	sub r1, r0
	mov r0, #3
	sub r0, r1
	lsl r1, r0, #2
	add r1, r1, r0
	sub r1, #2
	add r3, sp, #0x10
	mov r0, sp
	mov r2, #3
	ldr r6, =0x080483D6 | 1
	bx r6

LevelStringReturn:
	ldr r6, =0x080483C4 | 1
	bx r6

int_to_str:
	ldr r6, =ConvertIntToDecimalStringN
	bx r6

MemCopy:
	ldr r3, =Memcpy
	bx r3

	.align 2
level_string: .word 0x0826051C
objects: .word 0x0202063C

special_string:
.byte 0xFF
.rept 16
.byte 0
.endr

.align 2
.pool
@;0804BE80 with r3
CreateShakerHook:
	lsl r0, #0x18
	lsr r0, #0x18

	@;Save Index
	ldr r3, .ShakerData
	strb r0, [r3, #1]

	@ Set boolean to true
	mov r2, #1
	strb r2, [r3]

	@ Return
	ldr r2, =0x0202063C
	lsl r1, r0, #4
	ldr r3, =0x0804BE88 | 1
	bx r3

.pool
@;0x804BEDC with r2
ObjcShakerHook:
	mov r2, #0
	asr r0, #0x10
	cmp r0, #0x15
	bne ObjcShakerHookReturn

	strh r2, [r1, #0x24]
	strh r2, [r1, #0x26]

	@;Set boolean to false
	ldr r1, .ShakerData
	strb r2, [r1]

	ldr r2, =(0x0804BEE6 + 1)
	bx r2

ObjcShakerHookReturn:
	pop {pc}

.align 2
.ShakerData: .word SHAKER_HELPER

.pool
@;0x803301C with r1
PlayerHandleStatusIconUpdateHook:
	ldr r0, [r0, #0x4]
	lsl r1, r2, #0x1
	add r1, r2
	lsl r1, #0x2
	add r1, r0
	push {r1}
	bl CreateMegaIndicatorAfterAnim
	pop {r1}
	ldr r0, =0x8033026 | 1
	bx r0

.pool
@;0x8038974 with r1
OpponentHandleStatusIconUpdateHook:
	ldr r0, [r0, #0x4]
	lsl r1, r2, #0x1
	add r1, r2
	lsl r1, #0x2
	add r1, r0
	push {r1}
	bl CreateMegaIndicatorAfterAnim
	pop {r1}
	ldr r0, =0x803897E | 1
	bx r0

.pool
@;0x802F858 with r0
LoadHealthBoxesIndicatorHook:
	push {r4-r6,lr}
	sub sp, #0x14
	mov r5, r0
	mov r4, r1
	lsl r5, #0x18
	lsr r5, #0x18
	bl CreateMegaIndicatorAfterAnim
	ldr r1, =0x80483A4 | 1
	bx r1

@;Dynamax Hooks///////////////////////////////////////////////////////////////
.pool
@;0x0803443C with r0
DynamaxPalFadeHook1_LoadOpponentGfx:
	mov r0, r8
	mov r1, r7
	bl TryFadeBankPaletteForDynamax
	ldr r0, =0x8034468 | 1
	bx r0

.pool
@;0x080345FC with r0
DynamaxPalFadeHook2_LoadPlayerGfx:
	mov r0, r8
	mov r1, r7
	bl TryFadeBankPaletteForDynamax
	ldr r0, =0x8034626 | 1
	bx r0

.pool
@;0x080744B0 with r0
RaidBattleCoordHook1:
	mov r0, r5
	bl GetBattlerXCoord
	ldr r1, =0x08074588 | 1
	bx r1

.pool
@;0x080744D8 with r0
RaidBattleCoordHook2:
	mov r0, r5
	bl GetBattlerYCoord
	ldr r1, =0x08074588 | 1
	bx r1

.pool
@;0x08077CC4 with r1
DeadRaidMonSpriteHook:
	add r0, r9
	bl ShouldShowOpponentBattlerSprite
	ldr r1, =0x8077CCC | 1
	bx r1

.pool
@;0x0807524C with r2
DeadRaidMonSpriteHook2:
	mul r0, r1
	ldr r1, =PARTY_OPPONENT
	add r0, r1
	bl ShouldShowOpponentBattlerSprite
	ldr r1, =0x8075276 | 1
	bx r1

.pool
@;0x804A116 with r1
RaidBossHPColourHook:
	mov r7, r5
	mov r1, r6 @Bank
	bl GetBattleHealthbarColour
	mov r9, r0
	ldr r0, =0x804A12C | 1
	bx r0

@;0x803493C with r0
RaidHealthbarSpriteSheetFix:
	cmp r5, #0x9
	bne RaidHealthbarSpriteSheetFixReturnTrue
	ldr r0, =gBattlerPositions
	ldrb r0, [r0, #0x3]
	cmp r0, #0x4
	bcs RaidHealthbarSpriteSheetFixReturnTrue
	ldr r1, =0x08034944 | 1
	bx r1

RaidHealthbarSpriteSheetFixReturnTrue:
	ldr r0, =0x8034958 | 1
	bx r0

@;0x0802DF74 with r1
RaidCapturedSentToBoxStringHook:
	lsl r0, #0x18
	lsr r0, #0x18
	cmp r0, #0x6
	beq RaidCapturedSentToBoxReturn
	bl IsTagBattle
	cmp r0, #0x0
	bne RaidCapturedSentToBoxReturn
	ldr r0, =0x802DF8C | 1
	bx r0

RaidCapturedSentToBoxReturn:
	ldr r0, =0x802DF7C | 1
	bx r0

.pool
@;0x80D84F6 with r0
BufferMaxMoveNameBattle:
	mov r0, r1
	mov r1, r6 
	bl BufferMoveNameBattle
	ldr r0, =0x80D86AE | 1
	bx r0

.pool
@;0x8139D2C with r3
CreateSummaryScreenGigantamaxIconHook:
	mov r0, #0xD
	neg r0, r0
	and r0, r2
	strb r0, [r1, #0x5]
	bl CreateSummaryScreenGigantamaxIcon
	ldr r0, =0x8139D34 | 1
	bx r0
