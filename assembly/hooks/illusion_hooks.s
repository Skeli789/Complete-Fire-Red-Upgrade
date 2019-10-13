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

@0x804B2DC with r1
UpdateCryForIllusionHook:
	bl GetIllusionPartyData
	mov r7, r0
	ldr r0, =0x804B2F6 | 1
	bx r0

@0x8074598 with r2
UpdateAltitudeForIllusionHook:
	lsl r1, r1, #0x10
	lsr r4, r1, #0x10
	mov r0, r5
	bl TryUpdateIllusionAltitude
	cmp r0, #0x0
	beq AltitudeIllusionReturn
	mov r4, r0 @;Update Species

AltitudeIllusionReturn:
	mov r7, r4
	mov r0, r5
	ldr r1, =0x80745A0 | 1
	bx r1
