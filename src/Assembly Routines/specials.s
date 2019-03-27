.text
.align 2
.thumb
.thumb_func
/*
script specials in asm - hooks and returns

*/

.include "../asm_defines.s"

.global sp156_GhostBattleSpecial

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
Ghost Battle Special 0x156
	hook at 0807f92a via r2
	Inputs:
		var8004: species
		var8005: level
		var8006: hold item
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
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
