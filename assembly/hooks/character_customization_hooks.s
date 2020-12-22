.text
.align 2
.thumb
.thumb_func

.include "../asm_defines.s"

@@ Character Customization (credit to JPAN)
.global NpcSpawnWithTemplate
.global NpcSizeFix
.global NewNpcInfo
.global LinkNpcFix
.global NpcOffscreenFix
.global NpcWaterFix
.global CreateSpriteLoadTable	@credit to ghoulslash

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@Createsprite from any Table
@	repoint cmd at 15fc5c
@	usage:
@		writebytetooffset 0x[table] <gCreateSpriteTableSetter>
@		createSprite 0x[OW] etc etc
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
CreateSpriteLoadTable:
	push {r4-r6,lr}
	mov r6, r8
	push {r6}
	sub sp, #0x8
	mov r5, r0
	ldr r0, [r5, #0x8]
	ldrb r1, [r0]	@sprite number
	ldr r6, =gCreateSpriteTableSetter
	ldrb r6, [r6]
	cmp r6, #0x0
	beq createSprite
getTable:
	lsl r6, r6, #0x18
	lsr r6, r6, #0x10
	orr r1, r6
createSprite:
	mov r8, r1
	ldr r6, =(0x0806b660 +1)
	bx r6

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@0x805E150 with r0
UpdateEventObjectLargerGraphicsId:
	ldrb r0, [r4]
	mov r1, #0x5
	orr r0, r1
	strb r0, [r4]
	ldrb r0, [r5, #0x3]
	add r1, r4, #0x7
	strb r0, [r1, #0x1C]
	ldr r0, =0x805E15C | 1
	bx r0

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@	08 47 at 5e5d4
@	ptr+1 at 5e5f4
.align 2
.pool
NpcSpawnWithTemplate:
	ldr r1, =(0x02036e38) @EventObjects
	add r5, r0, r1
	ldrb r0, [r5, #0x5]
	add r1, r5, #0x7
	ldrb r1, [r1, #0x1C] @0x23
	lsl r1, r1, #0x8
	orr r0, r1
	bl GetEventObjectGraphicsInfo
	ldr r1, =(0x0805e5dc+1)
	bx r1

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@	hook at 5e964 via r4
.align 2
.pool
NewNpcInfo:
	mov r4, r0
	mov r5, r1
	mov r3, r2
	ldrb r0, [r4, #0x1]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	ldrb r1, [r4, #0x3]
	lsl r1, r1, #0x18
	lsr r1, r1, #0x10
	orr r0, r0, r1
	ldrb r1, [r4, #0x9]
	mov r2, r5
	ldr r4, =(0x0805e96e+1)
	bx r4

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@	08 47 at 5ee84
@	ptr+1 at 5efe4
.align 2
.pool
LinkNpcFix:
	ldr r6, =(0x02036e38)
	add r6, r6, r0
	mov r0, #0x0
	str r0, [sp, #0x20]
	ldrb r0, [r6, #0x5]
	add r1, r6, #0x7
	ldrb r1, [r1, #0x1C] @0x23
	lsl r1, r1, #0x8
	orr r0, r1
	bl GetEventObjectGraphicsInfo
	mov r5, r0
	ldrh r2, [r5, #0x6]
	add r4, sp, #0x18
	ldr r0, [r4, #0x4]
	lsr r0, r0, #0x10
	lsl r0, r0, #0x10
	orr r0, r2
	str r0, [r4, #0x4]
	ldrb r0, [r6, #0x5]
	add r1, r6, #0x7
	ldrb r1, [r1, #0x1C] @0x23
	lsl r1, r1, #0x8
	orr r0, r1
	ldr r1, =(0x0805eea2+1)
	bx r1

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@0x805F168 with r0
EventObjectSetGraphicsIdFix:
	ldrh r2, [r2]
	strb r2, [r6, #0x5]
	lsr r2, #0x8
	add r0, r6, #0x7
	strb r2, [r0, #0x1C] @0x23

	mov r1, #0x10
	ldrsh r0, [r6, r1]
	ldr r2, =0x805F170 | 1
	bx r2

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@	hook at 5D3AC via r1
.align 2
.pool
PlayerFishingFix:
	push {r2-r3}
	bl GetEventObjectGraphicsId
	strh r0, [r5, #0x24]
	pop {r2-r3}
	ldrb r0, [r2, #0x5]
	lsl r4, r0, #0x3
	ldr r1, =0x805D3B4 | 1
	bx r1

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@	hook at 9BA6C via r1
.align 2
.pool
NPCMartFix:
	bl GetEventObjectGraphicsId
	bl GetEventObjectGraphicsInfo
	mov r1, #0x0
	ldrsh r2, [r4, r1]
	lsl r1, r2, #0x3
	add r1, r1, r2
	lsl r1, r1, #0x2
	add r1, r8
	push {r0-r3}
	mov r0, r1
	bl GetEventObjectGraphicsId
	mov r4, r0
	pop {r0-r3}
	ldr r2, =0x809BA80 | 1
	bx r2

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

@0x810C2AC with r4
CreateTrainerPicSpriteHook:
	ldr r4, [sp, #0x24]
	str r1, [sp]
	str r4, [sp, #0x4]
	mov r1, r5
	bl CreateTrainerPicSprite
	add sp, #0x14
	pop {r4-r5,pc}

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

@0x80F4A3C with r3
CreditsPlayerPaletteHook:
	ldr r3, =LoadPalette
	bl bxr3
	mov r0, #0x1F
	lsl r0, #0x4 @0x1F0
	bl ChangeTrainerPicPal
	ldr r0, =0x80F4A6C | 1
	bx r0

bxr3:
	bx r3

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

@0x80C42F4 with r0
RegionMapPlayerIconHook:
	bl TryUpdateRegionMapIconPal
	add sp, #0x40
	pop {r4-r6,pc}
