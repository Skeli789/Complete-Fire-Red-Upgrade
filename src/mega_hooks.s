.thumb
.text
.align 2

.include "..\\defines"

.global MegaRetrieveDataHook

@;0x148C0 with r0
MegaRetrieveDataHook:
	push {r3}
	bl MegaRetrieveData
	pop {r3}
	ldr r0, =BATTLE_COMMUNICATION
	ldrb r1, [r3]
	ldr r2, =0x8014B30 | 1
	bx r2
	