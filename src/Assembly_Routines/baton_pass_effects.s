.text
.thumb
.align 2

.include "..\\defines"

.global BatonPassEffects

@0x8012850 with r0
BatonPassEffects:
	ldrb r1, [r3]
	lsl r1, #0x2
	add r1, r10
	ldr r0, [r1]
	ldr r2, =(STATUS3_SEEDED_RECOVERY | STATUS3_SEEDED | STATUS3_LOCKON | STATUS3_PERISH_SONG | STATUS3_ROOTED | STATUS3_LEVITATING | STATUS3_AQUA_RING | STATUS3_POWER_TRICK | STATUS3_ABILITY_SUPRESS)
	and r0, r2
	str r0, [r1]
BatonPassReturnReturn:
	ldr r0, =0x801285E | 1
	bx r0


