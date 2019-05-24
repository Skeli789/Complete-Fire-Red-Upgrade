.text
.align 2
.thumb
.thumb_func
/*
script functions/specials in asm - hooks and returns

*/

.include "../asm_defines.s"

@@ Specials (credit to JPAN)
@ see special_inserts for most of the code
.global sp097_WildGroundBattle
.global sp098_WildSeaBattle
.global sp156_GhostBattleSpecial

@@ Hidden Abilities (credit to azurile13)
.global DaycareInheritHiddenAbility

@@ Dex Nav (Credit to FBI)
.global CheckDexNavSelect
.global PokeToolNoFade

@@ Whiteout Hack
.global DynamicWhiteoutMap

@@ Wrap-around Summary Screen (credit to Sagiri)
.global SummaryScreenWrapAround

@@ Unhidden Power (credit to Sagiri)
.global WriteTypeHook
.global DisplayTypeHook

@@ Evolution Method Extra Hooks
.global RemoveEvoItem

@@ Start Menu Stuff
.global FixStartMenuSize

@@ Physical Special Split Icons
.global PhysicalSpecialSplitIconRoutine

@@ Triple Layer Blocks
@.global DrawBlockHook


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Triple Layer Blocks
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
@DrawBlockHook:
@	bl CheckBlockType


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Increase Start Menu Height for Fewer Items
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
FixStartMenuSize:
	push {r2}
	mov r2, r1	@menu items
	lsl r0, r1, #0x19
	mov r1, #0xff
	lsl r1, r1, #0x18	@FF000000
	add r0, r0, r1
	lsr r0, r0, #0x18
	cmp r2, #0x5
	bhi ReturnStartMenuHeight
	add r0, #0x1

ReturnStartMenuHeight:
	pop {r2}
	str r0, [sp, #0x4]
	ldr r0, =(0x080f7900 +1)
	bx r0


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Evolution Methods - Remove Evo Item
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
RemoveEvoItem:
	ldrh r0, [r4, #0x8]
	add r0, #0x1
	strh r0, [r4, #0x8]
	add r2, r2, r1
	add r2, #0xC
	push {r1-r5}
	mov r0, r9
	bl ItemEvolutionRemoval
	pop {r1-r5}
	mov r0, r9
	ldr r1, =(0x080CED68 +1)
	bx r1

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Unhidden Power - Write Type
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
WriteTypeHook:
	mov r0, r1	@move
	push {r0}
	ldrb r0, [r5]	@active battler
	lsl r0, #1
	ldr r1, =(0x02023BCE)
	add r0, r0, r1
	ldrb r0, [r0]
	mov r1, #100
	mul r0, r1
	ldr r1, =(0x02024284)
	add r1, r0, r1				@pokemon
	pop {r0}
	bl GetSummaryScreenMoveType
	ldr r1, =(0x0803098E +1)
	bx r1


.align 2
.pool
DisplayTypeHook:
	push {r3-r7}
	ldr r0, =(0x0203B140)	@pkmn_status_data
	ldr r0, [r0]
	ldr r1, .monLoc
	add r1, r0, r1		@pokemon
	mov r0, r2	@move
	bl GetSummaryScreenMoveType
	pop {r3-r7}
	strh r0, [r4]
	ldr r0, [r6]
	ldr r2, =(0x081368D8 +1)
	bx r2

.align 2
.pool
.monLoc: .word 0x00003290


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Summary Screen Wrapping - hook at 13B20C via r1
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
SummaryScreenWrapAround:
	push {r4-r7, lr}
	bl ChangeSummaryScreenMon
	pop {r4-r7, pc}


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Whiteout Hack
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
DynamicWhiteoutMap:
	bl WhiteoutLogic
	cmp r0, #0x1
	beq NoHack
	ldr r0, =(0x080bfdf4|1)
	bx r0
NoHack:
	ldr r2, =(0x083eec98)
	sub r0, r6, #0x1
	lsl r0, r0, #0x2
	add r1, r0, r2
	ldrh r1, [r1]
	strb r1, [r4]
	add r2, #0x2
	add r0, r0, r2
	ldr r2, =(0x080bfd64|1)
	bx r2

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
@ hook at 6F3A8 via r3
PokeToolNoFade:
    cmp r1, r0
    beq EndStartOptFadeCheck
    ldr r0, =(PokeToolsFunc)
    cmp r1, r0
    beq EndStartOptFadeCheck
    ldr r0, =(0x080CCB68|1)
    bl CallViaR0
    ldr r0, =(0x0806F3B0|1)
CallViaR0:
    bx r0
EndStartOptFadeCheck:
    pop {pc}




@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ SPECIALS
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
	ldr r1, =SPECIES_MAROWAK
	ldr r2, =(0x0807f938|1)
	bx r2


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ hook at 46116 via r0
.align 2
.pool
DaycareInheritHiddenAbility:
	mov r4, sp
	add r0, r5, #0x0
	ldr r3 , func_roll_gender
	bl bx_r3
	cmp r0, #0xFE
	beq mother_in_slot_one
	mov r0, r5
	add r0, r0, #0x8C
	mov r1, #0xB
	bl get_zero_attr
	cmp r0, #0x84
	beq mother_in_slot_one
	mov r0, r5
	add r0, r0, #0x8C
	b check_for_hidden

mother_in_slot_one:
	add r0, r5, #0x0

check_for_hidden:
	mov r1, #0x2E
	bl get_zero_attr
	cmp r0, #0x0
	beq end
	ldr r3, func_rand
	bl bx_r3
	lsr r0, r0, #0x8
	cmp r0, #0x99
	bhi end
	ldr r3, func_set_attr
	mov r2, #0x1
	push {r2}
	mov r2, sp
	mov r1, #0x2E
	mov r0, r4
	bl bx_r3
	add sp, #0x4

end:
	mov r2, r4
	add r2, r2, #0x6A
	mov r0, #0x1
	strb r0, [r2]
	mov r0, r4
	ldr r1, func_return
	bx r1

get_zero_attr:
	mov r2, #0x0
	ldr r3, func_get_attr

bx_r3:
	bx r3

.align 2
func_roll_gender: .word 0x0803F730 + 1
func_get_attr: .word 0x0803FBE8 + 1
func_rand: .word 0x08044EC8 + 1
func_set_attr: .word 0x0804037C + 1
func_return: .word 0x08046120 + 1
