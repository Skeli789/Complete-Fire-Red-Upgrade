.text
.thumb
.align 2

.include "..\\defines"

.global GetBankAbility
.global GetBankItem
.global GetCurrentHealth
.global GetBankStatus
.global GetBankStatus2
.global GetSpecialStatus
.global GetBankFromBattleData
.global GetOriginalMoveType
.global GetMoveTypeAfterChange
.global GetMoveSpecialty
.global GetMoveRange
.global CheckIfSunny

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

GetBankAbility:
	mov r1, #0x58
	mul	r0, r1
	ldr r1, =BATTLE_DATA + BDATA_ABILITY
	add	r1, r0
	ldrb r0, [r1]
	bx	lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

GetBankItem:
	mov r1, #0x58
	mul r0, r1
	ldr r1, =BATTLE_DATA + BDATA_ITEM
	add r1, r0
	ldrh r0, [r1]
	bx lr
	
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

GetCurrentHealth:
	mov r1, #0x58
	mul r0, r1
	ldr r1, =BATTLE_DATA + BDATA_CURRENT_HEALTH
	add r1, r0
	ldrh r0, [r1]
	bx lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

GetBankStatus:
	mov r1, #0x58
	mul r0, r1
	ldr r1, =BATTLE_DATA + BDATA_STATUS
	add r1, r0
	ldr r0, [r1]
	bx lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

GetBankStatus2:
	mov r1, #0x58
	mul r0, r1
	ldr r1, =BATTLE_DATA + BDATA_STATUS_2
	add r1, r0
	ldr r0, [r1]
	bx lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

GetSpecialStatus:
	ldr r1, =SPECIAL_STATUS_BITS
	lsl r0, #0x2
	ldr r0, [r1, r0]
	bx lr
	
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

GetBankFromBattleData:
  	ldr r1, =BATTLE_DATA
	cmp r0, r1
	beq Bank0
	add r1, #0x58
	cmp r0, r1
	beq Bank1
	add r1, #0x58
	cmp r0, r1
	beq Bank2
	mov r0, #0x3
	bx lr

Bank0:
	mov r0, #0x0
	bx lr

Bank1:
	mov r0, #0x1
	bx lr

Bank2:
	mov r0, #0x2
	bx lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

GetOriginalMoveType:
	ldr r0, =CURRENT_MOVE
	ldrh r0, [r0]

	ldr r2, =gBattleMoves + 0x2
	lsl r1, r0, #0x1
	add r0, r1
	lsl r0, #0x2
	ldrb r0, [r0, r2]
	bx lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

GetMoveTypeAfterChange:
	ldr r0, =DP08_POINTER
	ldr r0, [r0]
	ldrb r0, [r0, #0x13]
	bx lr
	
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

GetMoveSpecialty:
	ldr r0, =CURRENT_MOVE
	ldrh r0, [r0]

	ldr r2, =gBattleMoves + 0xA
	lsl r1, r0, #0x1
	add r0, r1
	lsl r0, #0x2
	ldrb r0, [r0, r2]
	bx lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

GetMoveRange:
	ldr r0, =CURRENT_MOVE
	ldrh r0, [r0]

	ldr r2, =gBattleMoves + 0x6
	lsl r1, r0, #0x1
	add r0, r1
	lsl r0, #0x2
	ldrb r0, [r0, r2]
	bx lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

CheckIfSunny:
	push {lr}
	mov r0, #0x0
	push {r0-r4}
CheckCloudNine:
	mov r0, #0x13
	mov r1, #0x0
	mov r2, #ABILITY_CLOUDNINE
	mov r3, #0x0
	bl FieldCheck
	cmp r0, #0x0
	bne NoWeather
CheckAirLock:
	mov r0, #0x13
	mov r1, #0x0
	mov r2, #ABILITY_AIRLOCK
	mov r3, #0x0
	bl FieldCheck
	cmp r0, #0x0
	bne NoWeather
CheckSun:
	ldr r0, =WEATHER_FLAGS
	ldrh r0, [r0]
	mov r1, #0x86
	lsl r1, #0x4
	and r0, r1
	cmp r0, #0x0
	beq NoWeather
	pop {r0-r4}
	mov r0, #0x1
	pop {pc}
NoWeather:
	pop {r0-r4,pc}

FieldCheck:
	ldr r4, =0x8019F19
	bx r4
