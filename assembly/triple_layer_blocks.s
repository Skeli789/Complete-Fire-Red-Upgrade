.text
.align 2
.thumb
.thumb_func
/*
triple_layer_blocks.s
	inserts diegoisawesome's triple layer block hack
*/

.include "../asm_defines.s"

@@ Triple Layer Blocks
.global DrawBlockHook

.pool
DrawBlockHook:
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	cmp r0, #0x4
	bhi SkipTripleLayer
	lsl r0, r0, #0x2
	ldr r3, =sOverworldBlockFuncs
	add r0, r0, r3
	ldr r0, [r0]
	mov pc, r0

SkipTripleLayer:
	ldr r1, =(0x0805AABE +1)
	bx r1
		
.align 2
.pool
HandleTripleBlock:
	ldr r0, .address
	ldr r0, [r0]
	lsl r3, r2, #0x1
	add r0, r3, r0
	ldrh r1, [r4]
	strh r1, [r0]
	ldrh r1, [r4, #0x2]
	strh r1, [r0, #0x2]
	add r2, r0, #0x0
	add r2, #0x40
	ldrh r1, [r4, #0x4]
	strh r1, [r2]
	add r0, #0x42
	ldrh r1, [r4, #0x6]
	strh r1, [r0]
	ldr r0, .address1
	ldr r0, [r0]
	add r0, r3, r0
	ldrh r1, [r4, #0x8]
	strh r1, [r0]
	ldrh r1, [r4, #0xA]
	strh r1, [r0, #0x2]
	add r2, r0, #0x0
	add r2, #0x40
	ldrh r1, [r4, #0xC]
	strh r1, [r2]
	add r0, #0x42
	ldrh r1, [r4, #0xE]
	strh r1, [r0]
	ldr r0, .address2
	ldr r0, [r0]
	add r3, r3, r0
	ldrh r0, [r4, #0x10]
	strh r0, [r3]
	ldrh r0, [r4, #0x12]
	strh r0, [r3, #0x2]
	add r1, r3, #0x0
	add r1, #0x40
	ldrh r0, [r4, #0x14]
	strh r0, [r1]
	add r3, #0x42
	ldrh r0, [r4, #0x16]
	strh r0, [r3]
	ldr r0, .return
	bx r0


.align 2
sOverworldBlockFuncs:
.word 0x805aa6c @LAYER_TYPE_NORMAL
.word 0x805aa6c @LAYER_TYPE_NORMAL
.word 0x805aa0c @LAYER_TYPE_COVERED_BY_OBJECTS
.word HandleTripleBlock
.word 0x805a9ce @LAYER_TYPE_BG3_BG1

.align 2
.address:	.word 0x0300501C
.address1:	.word 0x03005014
.address2:	.word 0x03005018
.return:	.word 0x0805AABF
