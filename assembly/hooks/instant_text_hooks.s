.thumb
.text
.align 2

.include "../asm_defines.s"

@Hook at 0x8002d10 with r0
FixInstantTextOptionSpeed:
	cmp r5, #0x7f
	bne FixInstantTextOptionSpeed_notInstantText
	mov r5, #1
FixInstantTextOptionSpeed_notInstantText:
	ldr r0, =sTempTextPrinter
	mov r1, #1
	mov r2, #0
	strb r1, [r0,#0x1b]
	ldr r3, =0x8002d18 | 1
	bx r3

@Hook at 0x8002dee with r0
RunTextPrintersForInstantTextHook:
	bl RunTextPrintersForInstantText
	cmp r0, #0
	bne RunTextPrintersForInstantTextHook_wasInstantText
	ldr r0, =sTextPrinters
	mov r6, #0
	add r5, r0, #4
	mov r8, r0
	mov r7, #0x1f
	ldr r1, =0x8002df8 | 1
	bx r1
RunTextPrintersForInstantTextHook_wasInstantText:
	ldr r1, =0x8002e5a | 1
	bx r1

gUnknown_841F428:
	.byte 4, 1, 0x7f
