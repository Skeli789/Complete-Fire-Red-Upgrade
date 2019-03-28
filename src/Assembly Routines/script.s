.text
.align 2
.thumb
.thumb_func
/*
script functions/specials in asm - hooks and returns

*/

.include "../asm_defines.s"

.global sp156_GhostBattleSpecial
.global createSpriteLoadTable


/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
Createsprite from any Table
	repoint cmd at 15fc5c
	usage:
		writebytetooffset 0x[table] <gCreateSpriteTableSetter>
		createSprite 0x[OW] etc etc
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
.align 2
.pool
createSpriteLoadTable:
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

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
Ghost Battle Special 0x156
	hook at 0807f92a via r2
	Inputs:
		var8004: species
		var8005: level
		var8006: hold item
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
.align 2
.pool
sp156_GhostBattleSpecial:
	mov r1, #0xfe
	str r1, [sp]
	mov r1, #0xc
	str r1, [sp, #0x4]
	mov r1, #0x0
	str r1, [sp, #0x8]
	ldr r3, =var8004
	ldrh r1, [r3]		@species
	cmp r1, #0x0
	beq defaultGhost
	ldrh r2, [r3, #0x2]	@level
	cmp r2, #MAX_LEVEL
	bgt defaultGhost
	cmp r2, #0x0
	beq defaultGhost
	ldrh r3, [r3, #0x4]	@hold item
	ldr r4, =(0x0807f93c|1)
	bx r4
defaultGhost:
	ldr r1, =PKMN_MAROWAK
	ldr r2, =(0x0807f938|1)
	bx r2
