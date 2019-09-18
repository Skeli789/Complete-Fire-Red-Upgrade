.text
.align 2
.thumb
.thumb_func

.include "../defines"

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
@0x805CA68 with r0
.pool
PlayerSpawnTemplateFix:
	mov r0, #0x0
	str r0, [r1]  @;Clear all pre-existing graphics data
	mov r0, #0xFF @;Player local Id
	strb r0, [r1]
	mov r0, #0x0
	mov r1, r8
	ldr r2, =0x805CA70 | 1
	bx r2

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@	08 47 at 5e5d4
@	ptr+1 at 5e5f4
.align 2
.pool
NpcSpawnWithTemplate:
	ldr r1, =(0x02036e38) @EventObjects
	add r5, r0, r1
	ldrb r0, [r5, #0x5] 
	ldrb r1, [r5, #0x1a]
	lsl r1, r1, #0x8
	orr r0, r1
	bl GetEventObjectGraphicsInfo
	ldr r1, =(0x0805e5dc+1)
	bx r1

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@	hook at 5e510 via r1
.align 2
.pool
NpcSizeFix:	
	push {r4, lr}
	sub sp, #0x8
	mov r4, r0
	ldrb r0, [r4, #0x5] 
	ldrb r1, [r4, #0x1a]
	lsl r1, r1, #0x8
	orr r0, r1
	bl GetEventObjectGraphicsInfo
	ldr r1, =(0x0805e51c+1)
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
	ldrb r1, [r6, #0x1a]
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
	ldrb r1, [r6, #0x1a]
	lsl r1, r1, #0x8
	orr r0, r1
	ldr r1, =(0x0805eea2+1)
	bx r1


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@	hook at 67a12 via r5
.align 2
.pool
NpcOffscreenFix:
	add r5, r0, #0x0
	add r4, r1, #0x0
	ldrb r1, [r5,#1]
	mov r0, #0x41
	neg r0, r0
	and r0, r1
	strb r0, [r5,#1]
	ldrb r0, [r5, #0x5]
	ldrb r1, [r5, #0x1a]
	lsl r1, r1, #0x8
	orr r0, r1
	bl GetEventObjectGraphicsInfo
	ldr r1, =(0x08067a26+1)
	bx r1
	
	
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@	hook at 67f92 via r5
.align 2
.pool
NpcWaterFix:	
	mov r5, r0
	mov r4, #0x0
	ldrb r0, [r5, #0x5] 
	ldrb r1, [r5, #0x1a]
	lsl r1, r1, #0x8
	orr r0, r1
	bl GetEventObjectGraphicsInfo
	ldr r1, =(0x08067f9c+1)
	bx r1

	