.thumb
.text
.align 2

@Repoint at 0x803FE70
GetMonPokeBallHook:
	mov r0, r9
	bl GetMonPokeBall
	mov r4, r0
	ldr r1, =0x804035C | 1
	bx r1

@Hook at 0x8040858 with r0
SetMonPokeBallHook:
	mov r0, r8 @Substruct0
	ldrb r1, [r4] @Ball To Set
	bl SetMonPokeBall
	ldr r0, =0x8040ADA | 1
	bx r0
