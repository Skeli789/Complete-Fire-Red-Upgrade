.text
.thumb
.align 2
.global DisplayStatStageMessage

DisplayStatStageMessage:
	ldr r0, .SeverelyIndex
	cmp r0, r1
	beq BufferSeverely
	add r0, #0x1
	cmp r0, r1
	bne DisplayStatStageMessageNormal
	ldr r1, .DrasticallyStringPtr
	b DisplayStatStageMessageReturn

BufferSeverely:
	ldr r1, .SeverelyStringPtr
	b DisplayStatStageMessageReturn

DisplayStatStageMessageNormal:
	ldr r0, .gBattleStringsTable
	sub r1, #0xC
	lsl r1, r1, #0x2
	add r1, r1, r0
	ldr r1, [r1]

DisplayStatStageMessageReturn:
	ldr r0, =0x080D868E | 1
	bx r0
	
.align 2
.SeverelyIndex: .word 0x185
.gBattleStringsTable: .word 0x83FDF3C
.DrasticallyStringPtr: .word DrasticallyString
.SeverelyStringPtr: .word SeverelyString
