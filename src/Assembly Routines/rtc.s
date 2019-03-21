.text
.align 2
.thumb
.thumb_func
/*
.org 0x41E, 0xff
	ldr r3, .ptr1
	bx r3
.align 2
.ptr1: .word Main1+rom+1


.org offset, 0xff

0300553C year (2 bytes)
0300553F month (1 byte)
03005540 day (1 byte)
03005542 hour (24-hr format, 1 byte)
03005543 min (1 byte)
03005544 second (1 byte)

*/

.global realTimeClock

realTimeClock:
	bl callRTC
	ldr r0, =(0x03003530)
	ldrb r0, [r0]
	ldr r3, =(0x00b0101c)
	cmp r0, #0x0
	bne jump_r3
	ldrh r1, [r7, #0x28]
	mov r0, #0x1
	and r0, r1
	ldr r3, =(0x0800042c|1)
jump_r3:
	mov pc, r3

callRTC:
	push {lr}
	bl init
	pop {pc}
	
init:
	push {lr}
	bl checkVal
	cmp r0, #0x0
	bne skipOver
	bl doFuncs1
skipOver:
	ldr r0, =(0x0300553d)
	bl doFuncs2	
	mov r0, #0x0
	pop {r1}
	bx r1


doFuncs2:
	push {r4,r5,lr}
	sub sp, #0x8
	mov r5, r0
	ldr r0, =(0x03005504)
	ldr r0, [r0]
	cmp r0, #0x0
	bne otherFuncs2
	mov r1, #0x0
	ldr r0, =(0x000007d9)
	strh r0, [r5]
	mov r0, #0x6
	strb r0, [r5, #0x2]
	mov r0, #0x1a
	strb r0, [r5, #0x3]
	mov r0, #0x1
	strb r0, [r5, #0x4]
	mov r0, #0xc
	strb r0, [r5, #0x5]
	strb r1, [r5, #0x6]
	strb r1, [r5, #0x7]
	b endFuncs2
	
otherFuncs2:
	mov r0, sp
	bl timer3
	mov r0, sp
	ldrb r0, [r0]
	bl valMath
	ldr r1, =(0x000007d0)
	add r0, r0, r1
	strh r0, [r5]
	mov r0, sp
	ldrb r0, [r0, #0x1]
	bl valMath
	strb r0, [r5, #0x2]
	mov r0, sp
	ldrb r1, [r0, #0x2]
	mov r4, #0x3f
	mov r0, r4
	and r0, r1
	bl valMath
	strb r0, [r5, #0x3]
	mov r0, sp
	ldrb r1, [r0, #0x3]
	mov r0, r4
	and r0, r1
	bl valMath
	strb r0, [r5, #0x4]
	mov r0, sp
	ldrb r0, [r0, #0x4]
	and r4, r0
	mov r0, r4
	bl valMath
	strb r0, [r5, #0x5]
	mov r0, sp
	ldrb r0, [r0, #0x5]
	bl valMath
	strb r0, [r5, #0x6]
	mov r0, sp
	ldrb r0, [r0, #0x6]
	bl valMath
	strb r0, [r5, #0x7]
	ldr r2, =(0x03005537)
	ldrb r3, [r5, #0x6]
	ldrb r0, [r2, #0x2]
	add r1, r3, r0
	strb r1, [r5, #0x6]
	lsl r0, r1, #0x18
	lsr r0, r0, #0x18
	cmp r0, #0x3b
	bls funcs2_part2
	mov r0, r1
	sub r0, #0x3c
	strb r0, [r5, #0x6]
	ldrb r0, [r5, #0x5]
	add r0, #0x1
	strb r0, [r5, #0x5]
funcs2_part2:
	ldrb r3, [r5, #0x5]
	ldrb r0, [r2, #0x1]
	add r1, r3, r0
	strb r1, [r5, #0x5]
	lsl r0, r1, #0x18
	lsr r0, r0, #0x18
	cmp r0, #0x17
	bls funcs2_part3
	mov r0, r1
	sub r0, #0x18
	strb r0, [r5, #0x5]
	ldrb r0, [r5, #0x4]
	add r0, #0x1
	strb r0, [r5, #0x4]
funcs2_part3:
	ldrb r2, [r2]
	ldrb r1, [r5, #0x4]
	add r0, r2, r1
	mov r1, #0x7
	bl timer4
	strb r0, [r5, #0x4]
endFuncs2:
	mov r0, #0x0
	add sp, #0x8
	pop {r4-r5, pc}
	
timer4:
	mov r3, #0x1
	cmp r1, #0x0
	beq jumpOver
	bpl postNeg
	neg r1, r1
postNeg:
	push {r4}
	push {r0}
	cmp r0, #0x0
	bpl postNeg2
	neg r0, r0
postNeg2:
	cmp r0, r1
	bcc postTimer4
	mov r4, #0x1
	lsl r4, r4, #0x08
	lsl r4, r4, #0x04
	lsl r4, r4, #0x10
timer4_part2_loop:
	cmp r1, r4
	bcs timer4_part2
	cmp r1, r0
	bcs timer4_part2
	lsl r1, r1, #0x04
	lsl r3, r3, #0x04
	b timer4_part2_loop
timer4_part2:
	lsl r4, r4, #0x03
timer4_part3_loop:
	cmp r1, r4
	bcs timer4_outerloop
	cmp r1, r0
	bcs timer4_outerloop
	lsl r1, r1, #0x01
	lsl r3, r3, #0x01
	b timer4_part3_loop
timer4_outerloop:
	mov r2, #0x0
	cmp r0, r1
	bcc timer4_postSub
	sub r0, r0, r1
timer4_postSub:
	lsr r4, r1, #0x01
	cmp r0, r4
	bcc timer4_part4
	sub r0, r0, r4
	mov r12, r3
	mov r4, #0x1
	ror r3, r4
	orr r2, r3
	mov r3, r12
timer4_part4:
	lsr r4, r1, #0x02
	cmp r0, r4
	bcc timer4_part5
	sub r0, r0, r4
	mov r12, r3
	mov r4, #0x2
	ror r3, r4
	orr r2, r3
	mov r3, r12
timer4_part5:
	lsr r4, r1, #0x03
	cmp r0, r4
	bcc timer4_part6
	sub r0, r0, r4
	mov r12, r3
	mov r4, #0x3
	ror r3, r4
	orr r2, r3
	mov r3, r12
timer4_part6:
	mov r12, r3
	cmp r0, #0x0
	beq timer4_part7
	lsr r3, r3, #0x04
	beq timer4_part7
	lsr r1, r1, #0x04
	b timer4_outerloop
timer4_part7:
	mov r4, #0xe
	lsl r4, r4, #0x08
	lsl r4, r4, #0x04
	lsl r4, r4, #0x10
	and r2, r4
	beq postTimer4
	mov r3, r12
	mov r4, #0x3
	ror r3, r4
	tst r2, r3
	beq timer4_part8
	lsr r4, r1, #0x03
	add r0, r0, r4
timer4_part8:
	mov r3, r12	
	mov r4, #0x2
	ror r3, r4
	tst r2, r3
	beq timer4_part9
	lsr r4, r1, #0x02
	add r0, r0, r4
timer4_part9:
	mov r3, r12
	mov r4, #0x1
	ror r3, r4
	tst r2, r3
	beq postTimer4
	lsr r4, r1, #0x01
	add r0, r0, r4		
postTimer4:		@08b013a0
	pop {r4}
	cmp r4, #0x0
	bpl postNegEnd
	neg r0, r0
postNegEnd:
	pop {r4}
	mov pc, lr
	
jumpOver:
	push {lr}
	bl movPcLr
	mov r0, #0x0
	pop {pc}
movPcLr:
	mov pc, lr
		
	
valMath:
	mov r2, #0xf
	and r2, r0
	asr r0, r0, #0x04
	lsl r1, r0, #0x02
	add r1, r1, r0
	lsl r1, r1, #0x01
	add r2, r2, r1
	mov r0, r2
	bx lr		
	

doFuncs1:
	push {r4-r5, lr}
	ldr r0, =(0x080000c8)
	mov r1, #0x1
	strh r1, [r0]
	sub r0, #0x4
	strh r1, [r0]
	mov r5, #0x5
	strh r5, [r0]
	ldr r4, =(0x080000c6)
	mov r0, #0x7
	strh r0, [r4]
	mov r0, #0x63
	bl timer1		@08b01024
	strh r5, [r4]
	bl timer2		@08b010a4
	ldr r1, =(0x03005547)
	str r0, [r1]
	bl checkVal		@08b010e4
	ldr r1, =(0x03005504)
	str r0, [r1]
	ldr r0, =(0x03005505)
	bl timer3		@08b010f4
	mov r0, #0x0
	pop {r4-r5, pc}
	
	
timer3:
	push {r4-r6,lr}
	mov r6, r0
	ldr r1, =(0x080000c4)
	mov r2, #0x1
	strh r2, [r1]
	ldr r5, =(0x00b01148)
	mov r0, #0x7
	strh r0, [r5]
	strh r2, [r1]
	mov r4, #0x5
	strh r4, [r1]
	mov r0, #0x65
	bl timer1		@08b01024
	strh r4, [r5]
	mov r4, r6
	mov r5, #0x3
timer3_inner:
	bl timer2
	strb r0, [r4]
	add r4, #0x1
	sub r5, #0x1
	cmp r5, #0x0
	bge timer3_inner
	ldr r1, =(0x00b01148)
	mov r0, #0x5
	strh r0, [r1]
	add r4, r6, #0x4
	mov r5, #0x2
timer3_inner2:
	bl timer2
	strb r0, [r4]
	add r4, #0x1
	sub r5, #0x1
	cmp r5, #0x0
	bge timer3_inner2
	mov r0, #0x0
	pop {r4-r6, pc}	
	
timer2:
	push {r4-r7,lr}
	mov r5, #0x0
	mov r2, #0x0
	ldr r3, =(0x080000c4)
	mov r6, #0x4
	mov r7, #0x5
	mov r0, #0x2
	mov r12, r0
timer2_loop_outer:
	add r4, r2, #0x1
	mov r0, #0x4
timer2_loop_inner:
	strh r6, [r3]
	sub r0, #0x1
	cmp r0, #0x0
	bge timer2_loop_inner
	strh r7, [r3]
	ldrh r1, [r3]
	mov r0, r12
	and r0, r1
	lsl r0, r0, #16
	lsr r0, r0, #16
	lsl r0, r2
	orr r5, r0
	mov r2, r4
	cmp r2, #0x7
	ble timer2_loop_outer
	asr r5, r5, #0x01
	mov r0, r5
	pop {r4-r7, pc}
	
timer1:
	push {r4-r7,lr}
	lsl r4, r0, #0x01
	mov r3, #0x7
	mov r7, #0x2
	ldr r2, =(0x080000c4)
	mov r6, #0x4
	mov r5, #0x5
timer1_loop:
	mov r0, r4
	asr r0, r3
	and r0, r7
	lsl r0, r0, #16
	lsr r0, r0, #16
	mov r1, r0
	orr r1, r6
	strh r1, [r2]
	strh r1, [r2]
	strh r1, [r2]
	orr r0, r5
	strh r0, [r2]
	sub r3, #0x1
	cmp r3, #0x0
	bge timer1_loop
	mov r0, #0x0
	pop {r4-r7, pc}	

checkVal:
	ldr r0, =(0x03005547)
	ldr r0, [r0]
	cmp r0, #0x0
	beq bxlr
	mov r0, #0x1
bxlr:
	bx lr
	
	