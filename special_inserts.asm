.text
.align 2
.thumb

.equ MAX_LEVEL, 100
.include "../asm_defines.s"

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Hidden Abilities - Change Bit
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.org 0x013144, 0xFF
HiddenAbilityChange1:
	mov r1, r9
	ldrb r1, [r1]
	mul r1, r7
	add r1, r8
	ldr r0, [r1, #0x48] @personality
	ldrh r2, [r1] @species
	ldrb r1, [r1, #0x17]
	lsr r1, r1, #0x7 @ability bit
	bl HiddenAbilityChange1 + 0x2DC28 @0x08040D6C

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Max Level Limiters
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.org 0x21CFA, 0xFF
MaxLevelChange2:
	.byte MAX_LEVEL

.org 0x21FB6, 0xFF
MaxLevelChange3:
	.byte MAX_LEVEL

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Hidden Abilities - Player
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.org 0x026E34, 0xFF
HiddenAbilityChange2:
	ldrb r0, [r4, #0x0] @personality
	mov r1, r6 @ability bit
	mov r2, r5 @species
	bl HiddenAbilityChange2 + 0x19F38 @0x08040D6C
	lsl r0, r0, #0x0

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Hidden Abilities - Opponent
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.org 0x026ECC, 0xFF
HiddenAbilityChange3:
	and r0, r1
	cmp r0, #0x0
	beq HiddenAbilityChange3 + 0x20
	ldrb r0, [r4, #0x0] @personality
	mov r1, r6 @ability bit
	mov r2, r5 @species
	bl HiddenAbilityChange3 + 0x19EA0 @0x08040D6C
	lsl r0, r0, #0x0
	cmp r0, #0x2B
	beq HiddenAbilityChange3 + 0x20
	mov r0, #0x1

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Hidden Abilities - Player
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.org 0x02A800, 0xFF
HiddenAbilityChange4_1:
	mov r1, #0x41
	bl HiddenAbilityChange4_1 + 0x153E8 @get_attr
	mov r5, r0
	mov r0, r4
	mov r1, #0x2E
	bl HiddenAbilityChange4_1 + 0x153E8 @get_attr
	ldrb r4, [r4, #0x0] @lowest personality byte
	lsl r0, r0, #0x18
	orr r4, r0 @000000b 00000000 00000000 pppppppp

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Hidden Abilities - Opponent
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.org 0x02A890, 0xFF
HiddenAbilityChange4_2:
	lsl r0, r4, #0x18
	lsr r0, r0, #0x18 @personality lowest byte
	lsr r1, r4, #0x18 @ability bit
	mov r2, r5 @species
	bl HiddenAbilityChange4_2 + 0x164DC @0x08040D6C

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Max Level Limiter
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.org 0x32F6E, 0xFF
MaxLevelChange14:
	.byte MAX_LEVEL - 1

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Hidden Abilities - Generate Poke
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.org 0x03DD58, 0xFF
HiddenAbilityPokemonGeneration:
	mov r0, #0x0
	str r0, [sp, #0x18]
	mov r0, r7
	mov r1, #0x2E
	mov r2, r9
	bl HiddenAbilityPokemonGeneration + 0x2778 @ set_pokemon_data_2
	b HiddenAbilityPokemonGeneration + 0x26 @ 0x0803DD7E

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Max Level Limiters
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.org 0x3E806, 0xFF
MaxLevelChange4:
	.byte MAX_LEVEL

.org 0x3E872, 0xFF
MaxLevelChange5:
	.byte MAX_LEVEL

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Hidden Abilities - Determine Ability Bit
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.org 0x40D38, 0xFF
DetermineAbilityMain:
	push {lr}
	bl offset_to_ability
	ldr r2, b_last_copied_ability
	strb r0, [r2]
	pop {r1}
	bx r1

offset_to_ability:
	push {r4-r5, lr}
	mov r4, r0
	mov r1, #0xB
	bl DetermineAbilityMain - 0x1150 @0x3FBE8 @get_attr
	lsl r5, r0, #0x10
	lsr r5, r5, #0x10
	mov r0, r4
	mov r1, #0x2E
	bl DetermineAbilityMain - 0x1150 @0x3FBE8 @get_attr
	lsl r1, r0, #0x1F
	ldr r0, [r4, #0x0]
	mov r2, r5 @species
	bl determine_ability
	pop {r4-r5}
	pop {r1}
	bx r1

determine_and_copy:
	push {lr}
	bl determine_ability
	ldr r2, b_last_copied_ability
	strb r0, [r2]
	pop {r1}
	bx r1

determine_ability:
	lsl r3, r2, #0x3
	sub r3, r3, r2
	lsl r3, r3, #0x2
	ldr r2, base_stats_table
	ldr r2, [r2]
	add r3, r2, r3 
	cmp r1, #0x0
	beq no_hidden
	ldrb r2, [r3, #0x1A]
	cmp r2, #0x0
	bne copy_hidden

no_hidden:
	lsl r0, r0, #0x1F
	cmp r0, #0x0
	beq first_slotted
	ldrb r0, [r3, #0x17]
	cmp r0, #0x0
	beq first_slotted
	bx lr

first_slotted:
	ldrb r0, [r3, #0x16]
	bx lr

copy_hidden:
	mov r0, r2
	bx lr

.align 2
base_stats_table: .word 0x80001BC
b_last_copied_ability: .word 0x02023D6A

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Hidden Abilities - Summary
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.org 0x041318, 0xFF
@poke summary info
HiddenAbilityChange5:
	ldrh r2, [r7] @species
	ldrb r1, [r7, #0x17]
	lsr r1, r1, #0x7 @ability bit
	ldrb r0, [r4, #0x0] @personality
	bl HiddenAbilityChange5 - 0x5AC @ 0x08040D6C
	add r1, r7, #0x4
	strb r0, [r1, #0x1C]
	
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Max Level Limiters
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.org 0x41834, 0xFF
MaxLevelChange6:
	.byte MAX_LEVEL

.org 0x41B0E, 0xFF
MaxLevelChange13:
	.byte MAX_LEVEL - 1

.org 0x41B1E, 0xFF
MaxLevelChange7:
	.byte MAX_LEVEL

.org 0x41B22, 0xFF
MaxLevelChange8:
	.byte MAX_LEVEL

.org 0x420E8, 0xFF
MaxLevelChange9:
	.byte MAX_LEVEL

.org 0x420EC, 0xFF
MaxLevelChange10:
	.byte MAX_LEVEL

.org 0x4274E, 0xFF
MaxLevelChange11:
	.byte MAX_LEVEL

.org 0x45684, 0xFF
MaxLevelChange12:
	.byte MAX_LEVEL

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Hidden Abilities - Egg Hatching 1
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.org 0x46CA0, 0xFF
HiddenAbilityEggHatching1:
	mov r0, r6
	mov r1, #0x2E

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Hidden Abilities - Egg Hatching 2
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.org 0x46D3C, 0xFF
HiddenAbilityEggHatching2:
	mov r0, r5
	mov r1, #0x2E

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Max Level Hack - Limiter
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.org 0x4A216, 0xFF
MaxLevelChange1:
	.byte MAX_LEVEL	
	
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Createsprite Fix - remove bit shifts in lnpc_new
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.org 0x5EA0A, 0xff
	.word 0x0	

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Hidden Abilities - Stench, Illuminate
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.org 0x82C70, 0xFF
HiddenAbilityChange6:
	bl HiddenAbilityChange6 - 0x41F38 @0x08040D38
	
	
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Fossil Image Hack
@ Inputs:
@	var8004: image number
@	vars8005/8006: x,y position on screen
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.org 0x9d504, 0xff
sp18b_FossilImageHack:
	push {r4-r6, lr}
	mov r6, r8
	push {r6}
	ldr r0, .PicboxTaskLoc
	bl 0x1119d4
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	cmp r0, #0x1
	beq 0x9d5f2
	ldr r0, .FreeTaskLoc
	bl 0x77688
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	cmp r0, #0xff
	bne EndFossilImageHack
	ldr r0, .Var_8004
	ldrh r0, [r0]
	ldr r1, .imageTable
	mov r2, #0x2
	lsl r2, r2, #0x8
	cmp r0, r2
	bgt 0x9d5f4
	lsl r0, r0, #0x3
	add r1, r0, r1
	ldr r0, [r1]
	push {r1}
	bl 0x8720
	pop {r1}
	add r1, #0x4
	ldr r0, [r1]
	mov r1, #0xe8
	lsl r1, r1, #0x1
	mov r2, #0x20
	bl 0x703ec
	b 0x9d572
EndFossilImageHack:
	mov r0, #0x0
	b 0x9d5f4
.align 2
.Var_8004: .word var8004
.imageTable: .word fossilImageTable
.PicboxTaskLoc: .word 0x0809d6d4|1
.FreeTaskLoc: .word 0x0809d494|1
	
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Max Level Hack - Rare Candies
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.org 0x1262D2, 0xFF
MaxLevelRareCandies:
	.byte MAX_LEVEL


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Max Level Hack - Summary Screen
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.org 0x136684, 0xFF
SummaryScreenExpDisplay1:
	.byte MAX_LEVEL - 1


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Hidden Abilities - Ability Names
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.org 0x1366EC, 0xFF
HiddenAbilityChange7:
	lsl r4, r0, #0x10
	ldr r0, [r6]
	add r0, r8
	ldrb r1, [r0, #0x0] @personality
	orr r4, r1 @ssssssss ssssssss 00000000 pppppppp
	mov r1, #0x2E
	bl HiddenAbilityChange7 - 0xF6B04 @get_attr
	lsl r0, r0, #0x18
	lsr r1, r0, #0x18 @ability bit
	lsl r0, r4, #0x18
	lsr r0, r0, #0x18 @personality
	lsr r2, r4, #0x10 @species
	bl HiddenAbilityChange7 - 0xF5980 @ 0x08040D6C
	lsl r0, r0, #0x18
	lsr r4, r0, #0x18
	ldr r0, [r6]


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Max Level Hack - Summary Screen Exp Display
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.org 0x13A9DC, 0xFF
SummaryScreenExpDisplay2:
	.byte MAX_LEVEL - 1, 0x2D, 0x0, 0xD9
	
	
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Multichoice Pointers
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.org 0x3E05B0, 0xFF
.word MULTICHOICE_STRING_LOADER
.word 0x2
.word MULTICHOICE_STRING_LOADER
.word 0x3
.word MULTICHOICE_STRING_LOADER
.word 0x4
.word MULTICHOICE_STRING_LOADER
.word 0x5
.word MULTICHOICE_STRING_LOADER
.word 0x6
.word MULTICHOICE_STRING_LOADER
.word 0x7	
	


