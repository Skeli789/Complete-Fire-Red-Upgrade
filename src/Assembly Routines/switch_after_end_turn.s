.text
.thumb
.align 2

.include "..\\defines"

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

KnockOutModification:
	ldr r1, =END_TURN_DONE_BYTE @Only switch when the End Turn is done
	ldrb r0, [r1]
	cmp r0, #0x0
	beq SkipSwitch
DoSwitch:
	ldr r0, =0x81D869D @Switch Script
	bl CallMoveExec
	ldr r0, [r5]
	ldr r1, =0x8018FC8 | 1
	bx r1

SkipSwitch:
	ldr r0, =USER_BANK
	ldrb r0, [r0, #0x1]
	bl GetCurrentHealth	@This check should be redunant as earlier
	cmp r0, #0x0			@code checks for this, but it doesn't work
	bne ActuallySkip		@without it so here it will stay.

	ldr r0, =USER_BANK
	ldrb r0, [r0, #0x1]
	bl PartyAlive			@Checks if target has any teammates left
	cmp r0, #0x0			@If no teammates
	beq DoSwitch			@Calls the switch script to end the battle

@Makes the game realize the target is dead for now, so no attacking it
	ldr r0, =USER_BANK
	ldrb r0, [r0, #0x1]	
	mov r1, #0x1
	lsl r1, r0
	ldr r2, =ABSENT_BATTLER_FLAGS
	ldrb r0, [r2]
	orr r0, r1
	strb r0, [r2]

@Record which Pokemon need replacements
	ldr r3, =ABSENT_BATTLER_HELPER
	ldrb r0, [r3]
	orr r0, r1
	strb r0, [r3]
ActuallySkip:
	ldrb r3, [r4]	
	ldr r0, =0x8019198 | 1
	bx r0

CallMoveExec:
	ldr r1, =0x801BBE4 | 1
	bx r1
	
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

ClearEndTurnEndByte:
	ldr r1, =END_TURN_DONE_BYTE
	mov r0, #0x0
	strb r0, [r1]
	
	mov r0, #0x0
	str r0, [sp]
	mov r0, #0x9
	mov r1, #0x0
	ldr r2, =0x8019204 | 1
	bx r2

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

@Checks if the attack should fail due to lack of targets
ChooseTargetFix:
	ldr r4, =USER_BANK
	ldrb r0, [r4]
	ldr r1, =DP08_POINTER
	ldr r1, [r1]
	add r1, #0xC
	ldrb r1, [r1, r0]
	cmp r0, r1
	beq ChooseTargetReturn

CheckDeath:
	mov r6, #0x0
	mov r5, r1
	mov r0, r1	@Target Bank

DeathLoop:		
	bl GetCurrentHealth	@Get Target's health
	cmp r0, #0x0			@Is Target Dead?
	bne ChooseTargetReturn

	ldr r0, =NUM_POKEMON	@Single/Double Battle Check
	ldrb r0, [r0]
	add r6, #0x2
	cmp r0, r6
	beq Failed
	
	mov r0, r5
	bl GetPartnerBank		@Check target's partner in doubles
	ldr r1, =USER_BANK
	ldrb r1, [r1]
	cmp r0, r1				@If you attacked your partner
	bne DeathLoop 

ReorganizeDeathLoop: 		@Start checking enemies
	mov r6, #0x0
	bl GetFoeBank
	mov r5, r0
	b DeathLoop

ChooseTargetReturn:
	ldrb r0, [r4]
	bl BattleSideGetOwner
	ldr r1, =0x8015F50 | 1
	bx r1

Failed:
	ldr r4, =BS_SCRIPT_LOC
	ldr r0, =FailScript
	ldr r1, =0x8016346 | 1
	bx r1

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleSideGetOwner:
	ldr r1, =0x80751C4 | 1
	bx r1

FailScript:
	attackcanceler
	goto FAILED-2
	