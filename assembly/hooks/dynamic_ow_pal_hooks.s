.text
.align 2
.thumb
.thumb_func

.include "../asm_defines.s"

@@ Dynamic Overworld Palettes (credit to Navenatox)
.global SetPalFossilImage
.global LoadPalFossilImage
.global DeleteOBJFreeTiles
.global DeleteAllOBJs
.global DeleteReflection
.global DeleteWarpArrow
.global SetPalMisc
.global SetPalEmotionBubble
.global SetPalWarpArrow
.global SetPalItemfinderArrow
.global SetPalFameCheckerUnknown
.global SetPalNPC1
.global SetPalNPC2
.global SetPalNPC3
.global SetPalNPC4
.global SetPalReflection
.global SetPalSurf
.global SetPalFly
.global SetPalFly2
.global SetPalShadow
.global RefreshFameCheckerNPCs
.global SetPalSSAnneWake
.global SetPalSSAnneSmoke
.global GetDarkeningType1
.global GetDarkeningType2
.global GetDarkeningType3
.global FogBrightenAll
.global FogBrightenOnStep
.global FogBrightenAndFadeIn
.global GetFadeType1
.global GetFadeType2

@@@@@@@@@@@@@@@@@@@@@@@@@@
@ hook at 9d5a8 via r2
.align 2
.pool
SetPalFossilImage:
	push {r0}
	mov r0, #5
	ldr r1, =0x20370C0 @ var 8004
	ldrh r1, [r1]
	bl FindPalRef
	lsl r1, r0, #4
	pop {r0}
	ldrb r2, [r0,#5]
	mov r3, #0xF
	and r2, r3
	orr r1, r2
	ldr r2, =0x809D5B2+1
	bx r2

@@@@@@@@@@@@@@@@@@@@@@@@@@
@ hook at 9D538, 9D568 via r1
.align 2
.pool
LoadPalFossilImage:
	push {r0}
	mov r0, #5
	ldr r1, =0x20370C0 @ var 8004
	ldrh r1, [r1]
	bl AddPalRef
	mov r1, r0
	pop {r0}
	cmp r1, #0xFF @ no more space
	beq LoadPalFossilImage_End
	lsl r1, #4
	ldr r2, =(0x80089A0+1) @ LoadOBJPalette
	bl CallByR2

LoadPalFossilImage_End:
	ldr r1, =0x809D572+1
	bx r1

CallByR2:
	bx r2


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ hook at 0x7280 via r1
.align 2
.pool
DeleteOBJFreeTiles:
	push {r4-r7,lr}
	mov r5, r0
	ldr r0, [r0,#0x14]
	ldr r1, =0x8231D00 @ template for empty OBJ
	cmp r0, r1
	beq DeleteOBJFreeTiles_Return
	ldrb r0, [r5,#5]
	lsr r0, #4
	bl PalRefDecreaseCount
DeleteOBJFreeTiles_Return:
	mov r0, r5
	add r0, #0x3E
	ldrb r1, [r0]
	ldr r0, =0x8007288+1
	bx r0

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
@ hook at 0x779C via r1
DeleteAllOBJs:
	ldr r1, =0x80073DC+1 @ DeleteOBJ
	bl CallByR1
	bl ClearAllPalRefs
	pop {r4,r5,pc}

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
@ hook at 0xDAF88 via r0
DeleteReflection:
	ldrb r0, [r4,#5]
	lsr r0, #4
	bl PalRefDecreaseCount
	mov r2, r4
	add r2, #0x3E
	ldrb r1, [r2]
	mov r0, #2
	neg r0, r0
	and r0, r1
	ldr r1, =0x80DAF94+1
	bx r1

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
@ hook at 0xDB120 via r0
DeleteWarpArrow:
	push {lr}
	add r1, r2
	mov r2, r1
	add r2, #0x3E
	ldrb r0, [r2]
	mov r3, #5
	and r0, r3
	cmp r0, #1
	bne DeleteWarpArrow_SetBits @ only decrease count when arrow disappears
	ldrb r0, [r1,#5]
	lsr r0, #4
	push {r2}
	bl PalRefDecreaseCount
	pop {r2}
DeleteWarpArrow_SetBits:
	ldrb r0, [r2]
	mov r1, #4
	orr r0, r1
	strb r0, [r2]
	pop {pc}

CallByR1:
	bx r1

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
@ hook at 0x71B8 via r0
SetPalMisc:
	mov r0, r8
	bl GetPalSlotMisc
	cmp r0, #0xFF
	beq SetPalMisc_DoNotSet
	ldr r1, =0x80071CA+1
	bx r1

SetPalMisc_DoNotSet:
	ldr r0, =0x80071D6+1
	bx r0

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
@ hook at 0x82664 via r3
SetPalEmotionBubble:
	push {r0-r2}
	bl LoadPaletteForEmotionBubbles
	lsl r1, r0, #4
	pop {r0}
	ldrb r3, [r0,#5]
	mov r2, #0xF
	and r2, r3
	mov r3, #0xD
	neg r3, r3
	and r2, r3
	mov r3, #4
	orr r2, r3
	orr r2, r1
	strb r2, [r0,#5]
	pop {r1,r2}
	ldr r3, =0x8082670+1
	bx r3

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
@ hook at 0xDB1AC via r3
SetPalWarpArrow:
	push {r2}
	bl LoadPaletteForEmotionBubbles
	pop {r2}
	lsl r0, #4
	ldrb r3, [r2,#5]
	mov r1, #0xF
	and r1, r3
	orr r0, r1
	strb r0, [r2,#5]
	mov r3, r2
	add r3, #0x3E
	ldrb r1, [r3]
	mov r0, #5
	neg r0, r0
	and r0, r1
	ldr r1, =0x80DB1B6+1
	bx r1

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
@ hook at 0x13F434 via r0
SetPalItemfinderArrow:
	bl LoadPaletteForEmotionBubbles
	lsl r0, #4
	ldrb r1, [r4,#5]
	mov r2, #0xF
	and r1, r2
	orr r0, r1
	strb r0, [r4,#5]
	mov r0, r4
	ldr r1, =0x813F43C+1
	bx r1

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
@ hook at 0x12D7A8 via r1
SetPalFameCheckerUnknown:
	push {r0}
	ldr r0, .PalTagFameChecker
	push {r2}
	bl FindOrLoadNPCPalette
	pop {r2}
	lsl r0, #4
	ldrb r3, [r2,#5]
	mov r1, #0xD
	neg r1, r1
	and r1, r3
	mov r3, #8
	orr r1, r3
	mov r3, #0xF
	and r1, r3
	orr r1, r0
	pop {r0}
	ldr r3, =0x812D7BA+1
	bx r3

.align 2
.PalTagFameChecker: .word 0x1103

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
@ hook at 0x5E6B0 via r0, for regular NPCs
SetPalNPC1:
	ldrh r0, [r6,#2]	@palette tag
	bl FindOrLoadNPCPalette
	lsl r1, r0, #4
	ldrb r2, [r4,#5]
	ldr r0, =0x805E6B8+1
	bx r0

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
 @ hook at 0x5EF78 via r0, for regular NPCs when returning to the map
SetPalNPC2:
	ldrh r0, [r5,#2]
	bl FindOrLoadNPCPalette
	lsl r1, r0, #4
	ldrb r2, [r4,#5]
	mov r0, #0xF
	and r0, r0, r2
	ldr r2, =0x805EF82+1
	bx r2

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
@ hook at 0x5EAB0 via r0, for script command 0xAA createsprite
SetPalNPC3:
	ldrh r0, [r4,#2]
	bl FindOrLoadNPCPalette
	lsl r1, r0, #4
	ldrb r3, [r5,#5]
	mov r2, #0xF
	ldr r0, =0x805EAB8+1
	bx r0

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
@ hook at 0x5EBC0 via r0, for Fame Checker NPCs
SetPalNPC4:
	mov r0, r4
	ldr r1, =0x80079FC+1 @ LoadAnimFrame, needed for instant refreshing
	bl CallByR1
	ldrh r0, [r7,#2]
	bl FindOrLoadNPCPalette
	lsl r1, r0, #4
	ldrb r3, [r4,#5]
	mov r2, #0xF
	ldr r0, =0x805EBC8+1
	bx r0

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
@ hook at 0xDADB4 via r2
SetPalReflection:
	lsr r0, r1, #4
	bl FindOrCreateReflectionPalette
	ldrb r1, [r7,#5]
	ldr r2, = 0x80DADBC+1
	bx r2

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
@ hook at 0xDC410 via r2
SetPalSurf:
	push {r1}
	strb r0, [r3]
	bl LoadPaletteForEmotionBubbles
	pop {r1}
	lsl r0, #4
	ldrb r3, [r1,#5]
	mov r2, #0xF
	and r2, r3
	orr r0, r2
	ldr r2, =0x80DC418+1
	bx r2

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
@ hook at 0x87188 via r1
SetPalFly:
	push {r0}
	push {r2}
	bl LoadPaletteForEmotionBubbles
	pop {r2}
	lsl r0, #4
	ldrb r3, [r2,#5]
	mov r1, #0xF
	and r1, r3
	mov r3, #0xD
	neg r3, r3
	and r1, r3
	orr r1, r0
	pop {r0}
	ldr r3, =0x8087192+1
	bx r3

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
@ hook at 0x86D58 via r0
SetPalFly2:
	push {r2}
	bl LoadPaletteForEmotionBubbles
	pop {r2}
	lsl r0, #4
	ldrb r1, [r2,#5]
	mov r3, #0xF
	and r1, r3
	mov r1, #0xD
	neg r1, r1
	and r1, r3
	mov r3, #4
	orr r1, r3
	orr r0, r1
	ldr r3, =0x8086D68+1
	bx r3

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
@ hook at 0xDB260 via r0
SetPalShadow:
	push {r2}
	ldr r0, .ShadowPalette
	bl FindOrLoadNPCPalette
	pop {r2}
	lsl r0, #4
	ldrb r1, [r2,#5]
	mov r3, #0xF
	and r1, r3
	orr r0, r1
	strb r0, [r2,#5]
	mov r0, #0
	pop {r4,r5,pc}

.pool
.align 2
.ShadowPalette: .word 0x00001100

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
@ hook at 0x12D2AC via r0, needed for instant refreshing
RefreshFameCheckerNPCs:
	ldr r0, =0x0812c5ab
	ldr r1, [sp,#0x14]
	cmp r0, r1
	bne RefreshFameCheckerNPCs_DoNotColor
	push {r4}
	mov r0, #0x10
	lsl r0, #4
	mov r1, #0x60
	mov r2, #0x10
	mov r3, #0
	ldr r4, =0x8045274+1 @ FadePalette, color palettes black if fading in
	bl CallByR4
	pop {r4}

RefreshFameCheckerNPCs_DoNotColor:
	ldr r1, =0x8006BA8+1 @ RefreshOBJs
	bl CallByR1
	cmp r5, #1
	bne RefreshFameCheckerNPCs_Return2
	ldr r3, =0x203B0FC
	ldr r2, [r3]
	ldrb r0, [r2,#7]
	ldr r1, =0x812D2B4+1
	bx r1

RefreshFameCheckerNPCs_Return2:
	ldr r0, =0x812D2E0+1
	bx r0

CallByR4:
	bx r4

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
@ hook at 0x15D4E8 via r0
SetPalSSAnneWake:
	push {r1}
	ldr r0, .PalSSAnneWake
	bl FindOrLoadNPCPalette
	pop {r1}
	lsl r0, #4
	ldrb r2, [r1,#5]
	mov r3, #0xD
	neg r3, r3
	and r2, r3
	mov r3, #8
	orr r2, r3
	mov r3, #0xF
	and r2, r3
	orr r0, r2
	ldr r2, =0x815D4FC+1
	bx r2

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
@ hook at 0x15D600 via r2
SetPalSSAnneSmoke:
	push {r0}
	ldr r0, .PalSSAnneWake
	bl FindOrLoadNPCPalette
	lsl r1, r0, #4
	pop {r0}
	ldrb r2, [r0,#5]
	mov r3, #0xF
	and r2, r3
	orr r1, r2
	ldr r2, =0x815D60A+1
	bx r2

.align 2
.PalSSAnneWake: .word 0x1115

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
@ hook at 0x7A2D0 via r0
GetDarkeningType1:
	mov r0, r4
	bl GetDarkeningTypeBySlot
	cmp r0, #0
	ldr r1, =0x807A2D8+1
	bx r1

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
@ hook at 0x7A428 via r0
GetDarkeningType2:
	mov r0, r4
	bl GetDarkeningTypeBySlot
	ldr r1, =0x807A430+1
	bx r1

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
@ hook at 0x7A544 via r0
GetDarkeningType3:
	mov r0, r4
	bl GetDarkeningTypeBySlot
	ldr r1, =0x807A54C+1
	bx r1

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
 @ hook at 0x7AEC4 via r1, dynamically brighten or un-brighten palettes when fog emerges or vanishes
FogBrightenAll:
	mov r1, #3
	and r1, r3
	cmp r1, #0
	bne FogBrightenAll_End
	lsr r2, #24
	cmp r2, #0
	bne FogBrightenAll_End
	bl FogBrightenPalettes

FogBrightenAll_End:
	ldr r6, .hword732
	add r0, r5, r6
	ldrh r1, [r0]
	lsl r1, r1, #8
	ldr r0, =0x807AECC | 1
	bx r0

.align 2
.hword732: .word 0x732

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
@ hook at 0x7AAAC via r0
FogBrightenOnStep:
	add r2, #10
	push {r1,r2}
	bl GetFadeTypeForCurrentWeather
	pop {r1,r2}
	cmp r0, #2
	beq FogBrightenOnStep_Brighten @ only brighten if there is fog weather
	ldr r0, =0x807AAB6+1
	bx r0

FogBrightenOnStep_Brighten:
	mov r0, r4
	mov r1, #0
	mov r2, #0
	bl FogBrightenAndFade
	ldr r0, =0x807AB68+1
	bx r0

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
@ hook at 0x7A644 via r0
FogBrightenAndFadeIn:
	mov r0, r4
	mov r1, r7
	ldrh r2, [sp]
	lsl r2, #0x10
	lsr r2, #0x10
	bl FogBrightenAndFade
	add r0, r4, #1
	mov r9, r0
	ldr r0, =0x807A6F8+1
	bx r0

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
@ hook at 0x7A0A8 via r1, call the correct fade-in function for all fog types
GetFadeType1:
	push {r2}
	ldrb r0, [r0]
	bl GetFadeTypeByWeather
	pop {r2}
	lsl r0, #2
	ldr r1, =GetFadeType1_ReturnTable
	add r0, r1
	ldr r0, [r0]
	bx r0

.align 2, 0
GetFadeType1_ReturnTable:
	.word 0x807A16C+1
	.word 0x807A0F8+1
	.word 0x807A144+1
	.word 0x807A120+1

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
@ hook at 0x7A860 via r2, copy palettes before fading out for all fog types
GetFadeType2:
	push {r1}
	ldrb r0, [r0]
	bl GetFadeTypeByWeather
	pop {r1}
	cmp r0, #0
	beq GetFadeType2_NormalFade
	ldr r0, =0x807A872+1
	bx r0

GetFadeType2_NormalFade:
	ldr r0, =0x807A880+1
	bx r0

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
@0x807D5F0 with r0
ReplaceOWSandstormPaletteHook:
	bl LoadPaletteForOverworldSandstorm
	mov r7, #0x0
	ldr r0, =0x807D5F8 | 1
	bx r0
