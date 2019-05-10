.text
.align 2
.thumb

.equ MAX_LEVEL, 100
.include "../asm_defines.s"

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Game Speed Up
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.org 0x890, 0xFF
main:
	ldr r2, .SuperBits
	ldrh r3, [r2]
	mov r0, #1	
	bic r3, r0
	strh r3, [r2]
loop_label:
	swi #2
	ldrh r3, [r2]
	tst r3, r0
	beq loop_label
	bx lr

.align 2
.SuperBits: .word 0x0300310C

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Dynamic Overworld Palette - part of hook at 0x779c
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.org 0x779A, 0xff
	.byte 0x0, 0x0

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
@ Hidden Abilities - Summary Screen
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
	.byte MAX_LEVEL, 0x29, 0x6, 0xD9, MAX_LEVEL

.org 0x420E8, 0xFF
MaxLevelChange9:
	.byte MAX_LEVEL

.org 0x420EC, 0xFF
MaxLevelChange10:
	.byte MAX_LEVEL

.org 0x4274E, 0xFF
MaxLevelChange11:
	.byte MAX_LEVEL
	
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ EV Cap - Limiters
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.org 0x439FC, 0xFF
ChangeEVCap1:
	.byte EV_CAP
	
.org 0x43A02, 0xFF
ChangeEVCap2:
	.byte EV_CAP

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Max Level - Limiter
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
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
@ Character Customization
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.org 0x5e152, 0xff
	mov r1, #0x5
	orr r0, r1
	strb r0, [r4]
	ldrb r0, [r5, #0x3]
	strb r0, [r4, #0x1a]
	
.org 0x5e5d4, 0xff
	.byte 0x8, 0x47		@ routine_ptr+1 at 5e5f4
	
.org 0x5e744, 0xff		@ fix size
	lsl r4, r4, #0x10
	lsr r4, r4, #0x10
	lsl r5, r5, #0x10
	lsr r5, r5, #0x10
	mov r0, #0x0
	str r0, [sp, #0x20]
	mov r1, r9
	ldrb r0, [r1, #0x1]
	ldrb r1, [r1, #0x3]
	lsl r1, r1, #0x18
	lsr r1, r1, #0x10
	orr r0, r1	

.org 0x5e8f0, 0xff		@ change size
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	
.org 0x5e962, 0xff
	.hword 0x0

.org 0x5ea0a, 0xff		@ fix createsprite
	.word 0x0	
	
.org 0x5ee84, 0xff		@ fix link npcs
	.byte 0x8, 0x47


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Dynamic Overworld Palettes
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.org 0x5f093, 0xff	@don't auto-load NPC palettes into slot 0 or 0xA
	.byte 0xE0

.org 0x5f114, 0xff	@don't reset pal slot during player animation or reflection
	.hword 0x0
	
.org 0x5f4ba, 0xff	@remove byte casts for NPC palette number
	.byte 0x1, 0x0, 0x0, 0x0

.org 0x5f548, 0xff	@remove byte casts for NPC palette number
	.byte 0xc0, 0x0, 0x9, 0x49
	
.org 0x5f54c, 0xff	@remove byte casts for NPC palette number
	.byte 0x0, 0x0
	
.org 0x5f5d2, 0xff	@remove byte casts for NPC palette number
	.byte 0x2, 0x0, 0x0, 0x0

.org 0x5f5e8, 0xff	@don't auto-load NPC palettes into slot 0 or 0xA
	.byte 0x70, 0x47
		
.org 0x5f658, 0xff	@don't auto-load NPC palettes into slot 0 or 0xA
	.byte 0x70, 0x47
	
.org 0x79c18, 0xff	@don't load rain palette on entering map
	.byte 0x0, 0x25, 0xe, 0xe0

.org 0x7a31f, 0xff	@don't treat slot 0xC as weather palette
	.byte 0xe0
	
.org 0x7a85e, 0xff	@make it compatible with daniilS' snowflakes routine
	.byte 0x80, 0x18
	
.org 0x7a872, 0xff	@make it compatible with daniilS' snowflakes routine
	.byte 0x1, 0x22, 0x5, 0xe0
		
.org 0x7aae7, 0xff	@don't record brightened slots
	.byte 0xe0
	
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Ghost Battle - Register Push
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.org 0x7F904, 0xff	
	.byte 0x10
	
.org 0x7f986, 0xff
	.byte 0x10, 0xbd	

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Dynamic Overworld Palettes
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@	
.org 0xdaec4, 0xff	@@don't auto-load NPC palettes into slot 0 or 0xA
	.byte 0x70, 0x47
	
.org 0xdaf20, 0xff	@don't auto-load NPC palettes into slot 0 or 0xA
	.byte 0x70, 0x47
	
.org 0xdafb8, 0xff	@don't reset pal slot during player animation or reflection
	.hword 0x0
	
	
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
@ DexNav - overworld anim for caves
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@	
.org 0x1D9714, 0xff		@ 0x081D96AC + 4 * 0x1A
	.word 0x081d98a4

	
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Dynamic Overworld Palettes
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@	
.org 0x1d9895, 0xff		@don't load sand palette on healing
	.byte 0x7, 0x9c, 0xbe, 0x3c, 0x8, 0x8d, 0x3b, 0x8, 0x8, 0x4
	
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ DexNav - overworld anim for caves
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@	
.org 0x1d98a4, 0xff
	.byte 0x7
	.word 0x083A5348
	@ .word oei_1A_cave|1	@1d98a9
.org 0x1d98ad, 0xff
	.byte 0x4

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


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Ghost Battle Fix
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.org 0x3fd35b, 0xff
.byte 0xfd, 0x6, 0xab, 0xfb, 0xfe, 0xff


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Dynamic Overworld Palettes
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.org 0x45fd52, 0xff		@pal slot of scroll arrow in Fame Checker
	.byte 0x9

