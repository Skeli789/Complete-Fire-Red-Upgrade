.thumb
/*
pokedex_screen_stats.s
	displaying base stats in the pokedex screen
credits to DoesntKnowHowToPlay and Squeetz	
*/


@Hook at 0x106370 with r0
PokedexScreenStats:
	mov r5, #4
	str r5, [sp]
	str r6, [sp, #4]
	mov r0, r10
	cmp r0, #0
	beq unknown_base_stats
	
hp:
	mov r0, #0
	ldrh r1, [sp, #0x1C]
	bl print_stat
	mov r3, #4 @ y co-ord
	str r3, [sp]
	mov r3, #0 @ x co-ord
	ldr r5, write_method
	bl call_via_r5
	
atk:
	mov r0, #1
	ldrh r1, [sp, #0x1C]
	bl print_stat
	mov r3, #4 @ y co-ord
	str r3, [sp]
	mov r3, #0x2C @ x co-ord
	ldr r5, write_method
	bl call_via_r5

def:
	mov r0, #2
	ldrh r1, [sp, #0x1C]
	bl print_stat
	mov r3, #18 @ y co-ord
	str r3, [sp]
	mov r3, #0 @ x co-ord
	ldr r5, write_method
	bl call_via_r5

spa:
	mov r0, #4
	ldrh r1, [sp, #0x1C]
	bl print_stat
	mov r3, #18 @ y co-ord
	str r3, [sp]
	mov r3, #0x2C @ x co-ord
	ldr r5, write_method
	bl call_via_r5

spd:
	mov r0, #5
	ldrh r1, [sp, #0x1C]
	bl print_stat
	mov r3, #32 @ y co-ord
	str r3, [sp]
	mov r3, #0 @ x co-ord
	ldr r5, write_method
	bl call_via_r5

spe:
	mov r0, #3
	ldrh r1, [sp, #0x1C]
	bl print_stat
	mov r3, #32 @ y co-ord
	str r3, [sp]
	mov r3, #0x2C @ x co-ord
	ldr r5, write_method
	bl call_via_r5

print_ability_one:
	ldrh r0, [sp, #0x1C] @Species
	bl GetAbility1
	ldrh r1, [sp, #0x1C]
	bl GetAbilityName
	mov r2, r0
 
	ldr r1, [r7]
	add r1, #0x53
	ldrb r1, [r1]
	mov r0, r1
	mov r1, #0
	mov r3, #46
	str r3, [sp]
	mov r3, #0
	ldr r5, write_method
	bl call_via_r5

print_ability_two:
	ldrh r0, [sp, #0x1C]
	bl GetAbility1
	mov r5, r0
	ldr r0, [sp, #0x1C]
	bl GetAbility2

	cmp r5, r0 @Ability 1 == Ability 2
	beq return
	cmp r0, #0 @Ability 2 == 0
	beq return
	ldrh r1, [sp, #0x1C]
	bl GetAbilityName
   	mov r2, r0
	
	ldr r1, [r7]
	add r1, #0x53
	ldrb r1, [r1]
	mov r0, r1
	mov r1, #0
	mov r3, #60
	str r3, [sp]
	mov r3, #0
	ldr r5, write_method
	bl call_via_r5
	b return

unknown_base_stats:
	mov r0, r1
	mov r1, #0
	ldr r2, str_unknown
	mov r3, #4 @ y co-ord
	str r3, [sp]
	mov r3, #0 @ x co-ord
	ldr r5, write_method
	bl call_via_r5

return:
	mov r5, #0
	ldr r0, return_loc
	bx r0

print_stat:
	push {lr}
	mov r4, r0
	bl GetVisualBaseStat
	mov r1, r0
	ldr r0, =gStringVar1
	mov r3, #0
	cmp r1, #99
	bhi no_leading_zeroes
	cmp r1, #9
	bhi one_leading_zero

two_leading_zeroes:
	str r3, [r0]
	add r0, #1

one_leading_zero:
	str r3, [r0]
	add r0, #1

no_leading_zeroes:
	mov r3, #3
	ldr r5, =ConvertIntToDecimalStringN
	bl call_via_r5

	mov r2, r4
	ldr r0, =gStringVar4
	ldr r1, str_table
	lsl r2, r2, #2
	add r1, r2
	ldr r1, [r1]
	ldr r5, =StringExpandPlaceholders
	bl call_via_r5
	ldr r1, [r7]
	add r1, #0x53
	ldrb r1, [r1]
	mov r0, r1
	mov r1, #0
	ldr r2, =gStringVar4
	pop {r5}

call_via_r5:
	bx r5

.align 2
	base_stats:	.word 0x80001BC
	str_table:	.word table
	str_unknown:	.word capture
	write_method:	.word 0x081047C8|1
	return_loc:	.word 0x08106380|1
table:
	.word stat_hp
	.word stat_atk
	.word stat_def
	.word stat_spe
	.word stat_spa
	.word stat_spd
	
stat_hp:
	.byte 0xC2, 0xCA, 0x0, 0x0, 0xFD, 0x2, 0xFF
	
stat_atk:
	.byte 0xBB, 0xE8, 0xDF, 0x0, 0xFD, 0x2, 0xFF 
	
stat_def:
	.byte 0xBE, 0xD9, 0xDA, 0x0, 0xFD, 0x2, 0xFF
	
stat_spe:
	.byte 0xCD, 0xE4, 0xD9, 0x0, 0xFD, 0x2, 0xFF
	
stat_spa:
	.byte 0xCD, 0xE4, 0xBB, 0x0, 0xFD, 0x2, 0xFF
	
stat_spd:
	.byte 0xCD, 0xE4, 0xBE, 0x0, 0xFD, 0x2, 0xFF
	
capture:
	.byte 0xBD, 0xD5, 0xE4, 0xE8, 0xE9, 0xE6, 0xD9, 0x0, 0xDA, 0xE3, 0xE6, 0xFE, 0xE1, 0xE3, 0xE6, 0xD9, 0x0, 0xDD, 0xE2, 0xDA, 0xE3, 0xE6, 0xE1, 0xD5, 0xE8, 0xDD, 0xE3, 0xE2, 0xAB, 0xFF
