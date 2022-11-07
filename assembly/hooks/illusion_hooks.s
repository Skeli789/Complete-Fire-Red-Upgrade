.text
.thumb
.align 2

.include "../asm_defines.s"

@0x803432C with r2
BattleLoadOpponentMonSpriteGfxHook:
	lsl r1, #0x18
	lsr r1, #0x18
	mov r8, r1
	mov r0, r1
	bl GetIllusionPartyData
	mov r4, r0
	ldr r1, =0x8034334 | 1
	bx r1

@0x80344A4 with r2
BattleLoadPlayerMonSpriteGfxHook:
	lsl r1, #0x18
	lsr r1, #0x18
	mov r8, r1
	mov r0, r1
	bl GetIllusionPartyData
	mov r6, r0
	ldr r1, =0x80344AC | 1
	bx r1

@0x80496C8 with r2
UpdateNickForIllusionHook:
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	mov r8, r0
	mov r0, r1
	bl UpdateNickForIllusion
	mov r7, r0
	ldr r0, =0x80496D0 | 1
	bx r0

@0x804A9E0 with r1
UpdatePokeBallForIllusionHook:
	bl UpdatePokeBallForIllusion
	ldr r1, =0x804AA1E | 1
	bx r1

@0x804B2E6 with r0
UpdateCryForIllusionHook:
	mov r0, r5
	bl IsDynamaxed
	cmp r0, #0x0
	bne IllusionCryUseRegularSpecies
	mov r0, r5
	bl GetIllusionPartyData @;Don't want to replace the mon in r7 because the illusion mon's health is used for the strength of the cry
	b IllusionCryReturn

IllusionCryUseRegularSpecies:
	mov r0, r7

IllusionCryReturn:
	mov r1, #0xB @MON_DATA_SPECIES
	ldr r2, =GetMonData
	bl bxr2
	ldr r1, =0x804B32C | 1
	bx r1

@0x8074598 with r2
UpdateYDeltaForIllusionHook:
	lsl r1, r1, #0x10
	lsr r4, r1, #0x10
	mov r0, r5
	bl TryUpdateIllusionYDelta
	cmp r0, #0x0
	beq YDeltaIllusionReturn
	mov r4, r0 @;Update Species

YDeltaIllusionReturn:
	mov r7, r4
	mov r0, r5
	ldr r1, =0x80745A0 | 1
	bx r1

@0x8074750 with r2
UpdateElevationForIllusionHook:
	lsr r6, r0, #0x18
	lsl r1, r1, #0x10
	lsr r4, r1, #0x10
	mov r0, r6
	bl TryUpdateIllusionYDelta
	cmp r0, #0x0
	beq ElevationIllusionReturn
	mov r4, r0 @;Update Species

ElevationIllusionReturn:
	mov r7, r4
	ldr r1, =0x08074758 | 1
	bx r1

@0x803570C with r2
ShadowIllusionHook:
	lsr r4, r0, #0x18
	mov r6, r4
	lsl r1, r1, #0x10
	lsr r5, r1, #0x10
	mov r0, r6
	bl TryUpdateIllusionYDelta
	cmp r0, #0x0
	beq ShadowIllusionReturn
	mov r5, r0 @;Update Species

ShadowIllusionReturn:
	ldr r1, =0x08035714 | 1
	bx r1

@0x08076B5C with r0
GetBattlerSpriteCoordAttrPlayerIllusionHook:
	mov r0, r8
	bl GetIllusionPartyData
	mov r5, r0
	mov r1, #0xB @Species
	ldr r2, =GetMonData
	bl bxr2
	lsl r0, r0, #0x10
	lsr r7, r0, #0x10
	mov r0, r5
	ldr r2, =0x08076B7E | 1
bxr2:
	bx r2

@0x08076C1A with r0
GetBattlerSpriteCoordAttrEnemyIllusionHook:
	mov r0, r8
	bl GetIllusionPartyData
	mov r5, r0
	mov r1, #0xB @Species
	ldr r2, =GetMonData
	bl bxr2
	lsl r0, r0, #0x10
	lsr r7, r0, #0x10
	mov r0, r5
	ldr r2, =0x08076C3C | 1
	bx r2

@0x8072604 with r1
LaunchBattleAnimationIllusionHook:
	bl GetIllusionPartyData
	ldr r1, =0x807262C | 1
	bx r1
