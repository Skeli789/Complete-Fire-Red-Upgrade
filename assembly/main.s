.text
.align 2
.thumb
.thumb_func
/*
main.s
	script functions/specials in asm - hooks and returns
*/

.include "../asm_defines.s"

@@ Specials (credit to JPAN)
@ see special_inserts for most of the code
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

@@ Coin Expansion
.global ExpandCoinIntConversion

@@ Safari Zone
.global SetInitialSafariZoneParams
.global FixSafariZoneWindow
.global FixSafariZoneBattleWindow

@@ Scrolling Multichoice
.global CustomScrollingMultichoiceHook
.global ScrollingMultiSizeHook


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@@ 080CB94C via r0
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
CustomScrollingMultichoiceHook:
	bl GetSizeOfMultiList
	mov r1, #0xA
	strh r0, [r5, r1]
	lsl r0, r0, #0x3
	bl Calloc
	ldr r4, =(0x2039A14) @RAM to hold pointer to malloc data
	str r0, [r4]
	bl SetBoxProperties
	mov r6, #0x0
	mov r4, #0x0
	mov r2, #0xA
	ldrsh r0, [r5, r2]
	lsl r3, r7, #0x2
	mov r10, r3
	add r1, SP, #0x18
	mov r9, r1
	cmp r6, r0
	bge MultichoiceLoop
	bl GetScrollingMultiList
	mov r8, r0
	ldr r0, =(0x80CB974 +1)
	bx r0	
	
MultichoiceLoop:
	ldr r0, =(0x80CB9AE +1)
	bx r0		
	
SetBoxProperties:
	ldr r2, =(0x80CBA7C +1)
	bx r2
	
Calloc:
	ldr r2, =(0x08002BB0 +1)
	bx r2

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@	
@@@@@ 080CB82A via r0
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
ScrollingMultiSizeHook:
	mov r0, r5	@task Id
	push {r3-r5}
	bl SetScrollingListSize
	pop {r3-r5}
	mov r1, #1
	ldr r2, =(0x080CB8E6 +1)
	bx r2

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@	
@@@@@ 080CBAC8 via r1
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
ScrollingMultiLeftRightHook:
	and r0, r3
	strb r0, [r2, #0x17]
	ldrb r0, [r2, #0x16]
	mov r1, #0x1 @LIST_MULTIPLE_SCROLL_DPAD
	lsl r1, #0x6
	orr r0, r1
	strb r0, [r2, #0x16]
	bx lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Safari Zone Steps/Ball Count
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool

FixSafariZoneBattleWindow:
	bl SafariZoneBallLabel
	mov r1, r0
	mov r0, sp
	bl StringCopy
	bl SafariZoneBattleBarCount
	ldr r0, =(0x08049D2E +1)
	bx r0
	
StringCopy:
	ldr r2, =(0x08008D84 +1)
	bx r2

.align 2
.pool
FixSafariZoneWindow:
	bl DisplaySafariZoneCounters
	ldr r5, =(0x02021D18)
	ldr r1, =(0x0806EEC4 +1)
	bx r1

.align 2
.pool
SetInitialSafariZoneParams:
	bl SetSafariZone
	pop {r0}
	bx r0
	
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Increase Integer Coin Conversion
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
ExpandCoinIntConversion:
	mov r0, r1
	bl ConvertCoinInt
	ldr r1, =(0x080D06E0 +1)
	bx r1

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

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ SPECIALS
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

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Wonder trades
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.align 2
.global TakePokemon

TakePokemon:
    push {r0-r7, lr}
    mov r5, #0x0
    ldr r6, .VAR
    ldr r7, .POKEMON_AMOUNT
    ldrb r4, [r7]
    cmp r4, #0x2
    blt return
    ldrh r3, [r6]
    cmp r3, r4
    bhs return
    sub r2, r4, #0x1
    strb r2, [r7]
    add r3, #0x1
    sub r2, r4, r3
    mov r5, #0x64
    mul r2, r5
    lsr r2, r2, #0x1
    mul r3, r5
    ldr r7, .POKEMON_PARTY
    add r0, r7, r3
    sub r1, r0, r5
    swi #0xB
    mul r4, r5
    add r7, r4
    sub r7, r5
    mov r0, #0x0
    mov r2, #0x19
    mov r5, #0x1

erase_loop:
    stmia r7!, {r0}
    sub r2, #0x1
    cmp r2, #0x0
    bne erase_loop

return:
    strh r5, [r6, #0x10]
    pop {r0-r7, pc}

.align 2
.POKEMON_AMOUNT:
    .word 0x02024029
.POKEMON_PARTY:
    .word 0x02024284
.VAR:
    .word 0x020270B8 + (0x8004 * 2)
	