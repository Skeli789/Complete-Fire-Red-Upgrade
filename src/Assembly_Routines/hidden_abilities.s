.text
.thumb
.align 2

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
