.text
.align 2
.thumb
.thumb_func
/*
script functions/specials in asm - hooks and returns

*/

.include "../asm_defines.s"

.global sp097_WildGroundBattle
.global sp098_WildSeaBattle
.global sp156_GhostBattleSpecial
.global CreateSpriteLoadTable


/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
Activate Wild Ground Battle
	ptr+1 at 15ffbc
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
.align 2
.pool
sp097_WildGroundBattle:
	push {r4-r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #0x8
	ldr r0, =(0x01000202)	@grass block data
	lsl r4, r0, #0x18
	lsr r4, r4, #0x18
	ldr r1, =(0x0806cbe4|1)
	bx r1
	
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
Activate Wild Sea Battle
	ptr+1 at 15ffc0
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
.align 2
.pool
sp098_WildSeaBattle:
	push {r4-r7, lr}
	mov r7, r8
	push {r7}
	sub SP, SP, #0x8
	ldr r0, =(0x22000410)	@water tile data
	lsl r4, r0, #0x18
	lsr r4, r4, #0x18
	ldr r1, =(0x0806cbe4|1)
	bx r1

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
Createsprite from any Table
	repoint cmd at 15fc5c
	usage:
		writebytetooffset 0x[table] <gCreateSpriteTableSetter>
		createSprite 0x[OW] etc etc
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
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
