.text
.thumb
.align 2

.include "../asm_defines.s"

.global DisplayPartnerNameInPartyMenuHook
.global TagTeamBackspritePalHook
.global TagTeamExecutionCompleteHook
.global TagTeamPokeballReadoutHook
.global TagTeamPartnerSendoutPokemonHook

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

@;0x08127AE8 with r0
DisplayPartnerNameInPartyMenuHook:
        bl IsTagBattle
        cmp r0, #0
        beq NoPartnerNameDisplay
        bl PartnerLoadName
		mov r1, r0
	mov r0, r4
        ldr r2, =0x08127AF0|1
JumpR2:
        bx r2

NoPartnerNameDisplay:
        ldr r2, =0x080444C4|1
        bl JumpR2

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

@;0x08033530 with r0
TagTeamBackspritePalHook:
    bl GetBackspriteId
	mov r1, r4
	lsr r1, #0x4
	bl LoadTrainerBackPal
    ldr r0, =0x0803353C|1
    bx r0

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

@;Repoint at 0x0802E384
TagTeamExecutionCompleteHook:
    push {r0}
	@;Mock Battle Check
	bl IsMockBattle
	cmp r0, #0x0
	bne AITakeOverForPlayer

    @; Ensure this is a tag team battle
    bl IsTagBattle
    cmp r0, #0
    beq StandardExecuter

    @; Ensure the active bank is the player's ally
    ldrb r0, [r4]
    cmp r0, #2
    bne StandardExecuter

AITakeOverForPlayer:
    @; We should use the partner's thing
    ldr r1, =SetControllerToPlayerPartner
    b ExecutionReturn

StandardExecuter:
    @; The standard player command executer
    ldr r1, =PlayerBufferRunCommand

ExecutionReturn:
    pop {r0}
    str r1, [r0]
    ldr r0, =0x0802E34E|1
    bx r0

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

@;0x08048FAC with r1
TagTeamPokeballReadoutHook:
    bl IsTagBattle
    cmp r0, #0
    bne Split
    mov r3, r10
    ldr r0, [r3]
    mov r1, #BATTLE_MULTI
    and r0, r1
    cmp r0, #0
    bne Split

NoSplit:
    @; Render normally
    ldr r0, =0x08048FEC|1
    bx r0

Split:
    @; Render split
    ldr r0, =0x08048FB6|1
    bx r0

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

TagTeamPartnerSendoutPokemonHook:
	bl IsMultiBattle
	ldr r1, =0x8013760 | 1
	bx r1
