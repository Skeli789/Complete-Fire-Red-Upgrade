.text
.thumb
.align 2

.include "..\\defines"

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SleepChecks:
	push {r2}
	ldrh r1, [r0]
	ldrh r2, .MiniorShield
	cmp r1, r2
	beq PreventSleep
InsomniaCheck:
	add r0, #0x20
	ldrb r1, [r0]
	cmp r1, #ABILITY_INSOMNIA
	beq PreventSleep
VitalSpiritCheck:
	cmp r1, #ABILITY_VITALSPIRIT
	beq PreventSleep
ComatoseCheck:
	cmp r1, #ABILITY_COMATOSE
	beq PreventSleep

Slp_LeafGuardCheck:
	cmp r1, #ABILITY_LEAFGUARD
	bne Slp_FlowerVeilCheck
	bl CheckIfSunny
	cmp r1, #0x0
	bne PreventSleep
Slp_FlowerVeilCheck:
	sub r0, #0x20
	bl FlowerVeilCheck
	cmp r0, #0x0
	bne PreventSleep
SweetVeilCheck:
	mov r2, #ABILITY_SWEETVEIL
	bl CheckAbilityTargetField
	cmp r0, #0x0
	bne PreventSleep
Slp_TerrainCheck:
	ldr r0, .TerrainByte
	ldr r0, [r0]
	cmp r0, #ELECTRIC_TERRAIN
	beq Slp_CheckGround
	cmp r0, #MISTY_TERRAIN
	bne Slp_Return
Slp_CheckGround:
	ldr r0, .TargetBank
	ldrb r0, [r0]
	bl CheckGrounding
	cmp r0, #0x0
	beq Slp_Return
PreventSleep:
	pop {r2}
	ldr r0, =0x08020570 | 1
	bx r0

Slp_Return:
	pop {r2}
	ldr r0, =0x0802017C | 1
	bx r0

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

PoisoningChecks:
	mov r1, r0
	sub r1, #0x20
	ldrh r1, [r1]
	ldrh r2, .MiniorShield
	cmp r1, r2
	beq PreventPoison
PSN_ImmunityCheck:
	ldrb r1, [r0]
	cmp r1, #ABILITY_IMMUNITY
	bne PSN_ComatoseCheck
PSN_Immunity:
	cmp r5, #0x1
	beq PreventPoison
	cmp r6, #0x80
	beq PreventPoison
PSN_ComatoseCheck:
	cmp r1, #ABILITY_COMATOSE
	beq PreventPoison
PSN_LeafGuardCheck:
	cmp r1, #ABILITY_LEAFGUARD
	bne PSN_FlowerVeilCheck
	bl CheckIfSunny
	cmp r1, #0x0
	bne PreventPoison
PSN_FlowerVeilCheck:
	push {r0-r3}
	sub r0, #0x20
	bl FlowerVeilCheck
	cmp r0, #0x0
	bne PreventPoisonPop

PSN_MistyTerrainCheck:
	ldr r0, .TerrainByte
	ldr r0, [r0]
	cmp r0, #MISTY_TERRAIN
	bne PSNReturn
	ldr r0, .TargetBank
	ldrb r0, [r0]
	bl CheckGrounding
	cmp r0, #0x0
	beq PSNReturn

PreventPoisonPop:
	pop {r0-r3}

PreventPoison:
	ldr r1, =0x80201B4 | 1
	bx r1

PSNReturn:
	pop {r0-r3}
	ldr r2, =0x80201E4 | 1
	bx r2

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

CorrosionPSN:
	bl GetMoveSpecialty
	cmp r0, #SPLIT_STATUS
	bne PSN_BeginTypeCheck

PSN_CorrosionCheck:
	ldr r0, .UserBank
	ldrb r0, [r0]
	bl GetBankAbility
	cmp r0, #ABILITY_CORROSION
	beq PSN_HasCorrosion

PSN_BeginTypeCheck:
	mov r1, r9
	ldrb r0, [r1]
	mov r1, #0x58
	mul r0, r1
	ldr r2, =0x80201EC | 1
	bx r2

PSN_HasCorrosion:
	mov r0, r9
	ldrb r1, [r0]
	mov r0, #0x58
	mul r1, r0
	ldr r0, =0x8020250 | 1
	bx r0

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BurnChecks:
	add r0, r8
	ldrh r1, [r0]
	ldrh r2, .MiniorShield
	cmp r1, r2
	beq PreventBurn
WaterVeilCheck:
	add r0, #0x20
	ldrb r1, [r0]
	cmp r1, #ABILITY_WATERVEIL
	beq PreventBurn
WaterBubbleCheck:
	cmp r1, #ABILITY_WATERBUBBLE
	beq PreventBurn
BRN_ComatoseCheck:
	cmp r1, #ABILITY_COMATOSE
	beq PreventBurn
BRN_LeafGuardCheck:
	cmp r1, #ABILITY_LEAFGUARD
	bne BRN_FlowerVeilCheck
	bl CheckIfSunny
	cmp r1, #0x0
	bne PreventBurn
BRN_FlowerVeilCheck:
	sub r0, #0x20
	bl FlowerVeilCheck
	cmp r0, #0x0
	bne PreventBurn

BRN_MistyTerrainCheck:
	ldr r0, .TerrainByte
	ldr r0, [r0]
	cmp r0, #MISTY_TERRAIN
	bne BRN_Return
	ldr r0, .TargetBank
	ldrb r0, [r0]
	bl CheckGrounding
	cmp r0, #0x0
	beq BRN_Return
PreventBurn:
	cmp r4, #0x1
	beq BRN_Skip
	cmp r5, #0x80
	bne BRN_Return
BRN_Skip:
	ldr r0, =0x080201B4 | 1
	bx r0

BRN_Return:
	ldr r0, =0x080202B4 | 1
	bx r0

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

FreezingChecks:
	mov r0, r1
	ldrh r1, [r0]
	ldrh r2, .MiniorShield
	cmp r1, r2
	beq PreventFreeze
MagmaArmorCheck:
	add r0, #0x20
	ldrb r1, [r0]
	cmp r1, #ABILITY_MAGMAARMOR
	beq PreventFreeze
FRZ_ComatoseCheck:
	cmp r1, #ABILITY_COMATOSE
	beq PreventFreeze
FRZ_LeafGuardCheck:
	cmp r1, #ABILITY_LEAFGUARD
	bne FRZ_FlowerVeilCheck
	bl CheckIfSunny
	cmp r1, #0x0
	bne PreventFreeze
FRZ_FlowerVeilCheck:
	sub r0, #0x20
	bl FlowerVeilCheck
	cmp r0, #0x0
	bne PreventFreeze

FRZ_MistyTerrainCheck:
	ldr r0, .TerrainByte
	ldr r0, [r0]
	cmp r0, #MISTY_TERRAIN
	bne FRZ_Return
	ldr r0, .TargetBank
	ldrb r0, [r0]
	bl CheckGrounding
	cmp r0, #0x0
	beq FRZ_Return
PreventFreeze:
	ldr r1, =0x08020570 | 1
	bx r1

FRZ_Return:
	ldr r0, =0x080203CA | 1
	bx r0

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

ParalysisChecks:
	ldrh r1, [r0]
	ldrh r2, .MiniorShield
	cmp r1, r2
	beq PreventParalysis
PARA_LimberCheck:
	add r0, #0x20
	ldrb r1, [r0]
	cmp r1, #ABILITY_LIMBER
	beq PreventParalysis
PARA_ComatoseCheck:
	cmp r1, #ABILITY_COMATOSE
	beq PreventParalysis
PARA_LeafGuardCheck:
	cmp r1, #ABILITY_LEAFGUARD
	bne CheckElectricType
	bl CheckIfSunny
	cmp r1, #0x0
	bne PreventParalysis

CheckElectricType:
	sub r0, #0x8
	ldrb r3, [r0]
	add r0, #0x8
	ldrb r1, [r0, #0x1]
	ldrb r2, [r0, #0x2]
CheckPrimaryTypeElectric:
	cmp r1, #TYPE_ELECTRIC
	beq PreventParalysis
CheckSecondaryTypeElectric:
	cmp r2, #TYPE_ELECTRIC
	beq PreventParalysis
CheckTertiaryTypeElectric:
	cmp r3, #TYPE_ELECTRIC
	beq PreventParalysis

PARA_FlowerVeilCheck:
	sub r0, #0x20
	bl FlowerVeilCheck
	cmp r0, #0x0
	bne PreventParalysis

MistyTerrainCheck:
	ldr r0, .TerrainByte
	ldr r0, [r0]
	cmp r0, #MISTY_TERRAIN
	bne PARA_Return
	ldr r0, .TargetBank
	ldrb r0, [r0]
	bl CheckGrounding
	cmp r0, #0x0
	beq PARA_Return
PreventParalysis:
	cmp r4, #0x1
	beq PARA_Skip
	cmp r5, #0x80
	bne PARA_OtherReturn
PARA_Skip:
	ldr r0, =0x080201B5 | 1
	bx r0

PARA_Return:
	mov r0, r9
	ldrb r1, [r0]
	mov r0, #0x58
	mul r1, r0
	ldr r0, =0x0802042C | 1
	bx r0

PARA_OtherReturn:
	ldr r0, =0x08020570 | 1
	bx r0

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BadPoisoningChecks:
	mov r1, r0
	sub r1, #0x20
	ldrh r1, [r1]
	ldrh r2, .MiniorShield
	cmp r1, r2
	beq PreventBadPoison
BadPSN_ImmunityCheck:
	ldrb r1, [r0]
	cmp r1, #ABILITY_IMMUNITY
	bne BadPSN_ComatoseCheck
BadPSN_Immunity:
	cmp r5, #0x1
	beq PreventBadPoison
	cmp r6, #0x80
	beq PreventBadPoison
BadPSN_ComatoseCheck:
	cmp r1, #ABILITY_COMATOSE
	beq PreventBadPoison
BadPSN_LeafGuardCheck:
	cmp r1, #ABILITY_LEAFGUARD
	bne BadPSN_FlowerVeilCheck
	bl CheckIfSunny
	cmp r1, #0x0
	bne PreventBadPoison
BadPSN_FlowerVeilCheck:
	push {r0-r3}
	sub r0, #0x20
	bl FlowerVeilCheck
	cmp r0, #0x0
	bne PreventBadPoisonPop

BadPSN_MistyTerrainCheck:
	ldr r0, .TerrainByte
	ldr r0, [r0]
	cmp r0, #MISTY_TERRAIN
	bne BadPSNReturn
	ldr r0, .TargetBank
	ldrb r0, [r0]
	bl CheckGrounding
	cmp r0, #0x0
	beq BadPSNReturn

PreventBadPoisonPop:
	pop {r0-r3}

PreventBadPoison:
	ldr r1, =0x08020468 | 1
	bx r1
BadPSNReturn:
	pop {r0-r3}
	ldr r2, =0x080204AE | 1
	bx r2

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

CorrosionBadPSN:
	bl GetMoveSpecialty
	cmp r0, #SPLIT_STATUS
	bne BadPSN_BeginTypeCheck

BadPSN_CorrosionCheck:
	ldr r0, .UserBank
	ldrb r0, [r0]
	bl GetBankAbility
	cmp r0, #ABILITY_CORROSION
	beq BadPSN_HasCorrosion

BadPSN_BeginTypeCheck:
	mov r2, r9
	ldrb r0, [r2]
	mov r1, #0x58
	mul r0, r1
	add r0, r8
	mov r1, r0
	ldr r2, =0x80204BA | 1
	bx r2

BadPSN_HasCorrosion:
	mov r6, #0x58
	mov r0, r9
	ldrb r0, [r0]
	bl GetBankAbility
	ldr r0, =0x8020548 | 1
	bx r0

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

ConfusionChecks:
	ldr r0, .TerrainByte
	ldrb r0, [r0]
	cmp r0, #MISTY_TERRAIN
	bne CFSN_CheckAbility
	ldr r0, .TargetBank
	ldrb r0, [r0]
	bl CheckGrounding
	cmp r0, #0x1
	bne CFSN_CheckAbility
	bl CFSN_Return

CFSN_CheckAbility:
	mul r1, r5
	mov r2, r8
	add r0, r1, r2
	add r0, #0x20
	ldr r3, =0x80207C8 | 1
	bx r3

CFSN_Return:
	ldr r0, =0x80210EC | 1
	bx r0

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

@_bool FlowerVeilCheck(battle_data* b_data)
FlowerVeilCheck:
	push {r1-r3,lr}
	add r0, #0x18
	ldrb r3, [r0]
	add r0, #0x8
	ldrb r1, [r0, #0x1]
	ldrb r2, [r0, #0x2]
CheckPrimaryTypeGrass:
	cmp r1, #TYPE_GRASS
	beq CheckFlowerVeilOnField
CheckSecondaryTypeGrass:
	cmp r2, #TYPE_GRASS
	beq CheckFlowerVeilOnField
CheckTertiaryTypeGrass:
	cmp r3, #TYPE_GRASS
	bne NoStatusPrevention
CheckFlowerVeilOnField:
	mov r2, #ABILITY_FLOWERVEIL
	bl CheckAbilityTargetField
	cmp r0, #0x0
	bne PreventStatus
NoStatusPrevention:
	mov r0, #0x0
	pop {r1-r3,pc}
PreventStatus:
	mov r0, #0x1
	pop {r1-r3,pc}

.align 2
.UserBank: .word USER_BANK
.TargetBank: .word TARGET_BANK
.TerrainByte: .word TERRAIN_BYTE
.MiniorShield: .word PKMN_MINIORSHIELD
