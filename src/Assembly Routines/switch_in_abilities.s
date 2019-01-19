.text
.align 
.thumb
.thumb_func
/*
Switch In Abilities:
	Download
	Snow Warning
	Dark Aura
	Fairy Aura
	Aura Break
	Mold Breaker
	Turboblaze
	Teravolt
	Electric Surge
	Grassy Surge
	Misty Surge
	Psychic Surge
	Anticipation
	Forewarn
	Frisk
	Unnerve
	Imposter
	Primordial Sea
	Desolate Land
	Delta Stream
	Pressure
	Magma Armor
	Cloud Nine
	Air Lock
	Anticipation
	Forewarn
	
*/

/*
.include "..\\defines"

.global SwitchInTable
.global SwitchInLoad
.global AnticipationBS
.global AuraBreakBS
.global CloudNine_AirLockBS
.global DarkAuraBS
.global DeltaStreamBS
.global DesolateLandBS
.global DownloadBS
.global FairyAuraBS
.global ForewarnBS
.global FriskBS
.global ImposterBS
.global MoldBreakerBS
.global PrimordialSeaBS
.global SnowWarningBS
.global TeravoltBS
.global ElectricSurgeBS
.global GrassySurgeBS
.global MistySurgeBS
.global PsychicSurgeBS
.global TurboblazeBS
.global UnnerveBS
.global MagmaArmorBS
.global PressureBS

@.global TraceFix

@ SWITCH IN TABLE @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
SwitchInTable:
.byte ABILITY_DOWNLOAD,0,0,0
.word DownloadBS
.byte ABILITY_SNOWWARNING,0,0,0
.word SnowWarningBS
.byte ABILITY_DARKAURA,0,0,0
.word DarkAuraBS
.byte ABILITY_FAIRYAURA,0,0,0
.word FairyAuraBS
.byte ABILITY_AURABREAK,0,0,0
.word AuraBreakBS
.byte ABILITY_MOLDBREAKER,0,0,0
.word MoldBreakerBS
.byte ABILITY_TURBOBLAZE,0,0,0
.word TurboblazeBS
.byte ABILITY_TERAVOLT,0,0,0
.word TeravoltBS
.byte ABILITY_ELECTRICSURGE,0,0,0
.word ElectricSurgeBS
.byte ABILITY_GRASSYSURGE,0,0,0
.word GrassySurgeBS
.byte ABILITY_MISTYSURGE,0,0,0
.word MistySurgeBS
.byte ABILITY_PSYCHICSURGE,0,0,0
.word PsychicSurgeBS
.byte ABILITY_ANTICIPATION,0,0,0
.word AnticipationBS
.byte ABILITY_FOREWARN,0,0,0
.word ForewarnBS
.byte ABILITY_FRISK,0,0,0
.word FriskBS
.byte ABILITY_UNNERVE,0,0,0
.word UnnerveBS
.byte ABILITY_IMPOSTER,0,0,0
.word ImposterBS
.byte ABILITY_PRIMORDIALSEA,0,0,0
.word PrimordialSeaBS
.byte ABILITY_DESOLATELAND,0,0,0
.word DesolateLandBS
.byte ABILITY_DELTASTREAM,0,0,0
.word DeltaStreamBS
.byte ABILITY_MAGMAARMOR,0,0,0
.word MagmaArmorBS
.byte ABILITY_PRESSURE,0,0,0
.word PressureBS
.byte ABILITY_CLOUDNINE,0,0,0
.word CloudNine_AirLockBS
.byte ABILITY_AIRLOCK,0,0,0
.word CloudNine_AirLockBS
.byte 0xFF

@ LOAD SWITCH IN TABLE @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ hook at 1B4F0 via r0
.pool
SwitchInLoad:
	push {r0-r2}
	ldr r0, =REVERSION_BYTE
	ldrb r0, [r0]
	cmp r0, #0x1
	beq IntimidateCheck
	ldrh r0, [r3, #0xE]
	bl GetItemEffect
	cmp r0, #ITEM_EFFECT_PRIMAL_ORB
	bne IntimidateCheck
	ldr r0, =REVERSION_BYTE
	mov r1, #0x1
	strb r1, [r0]
	pop {r0-r2}
	ldr r0, =primal_reversion_bs
	ldr r1, =(0x0801BABB)
	bx r1

IntimidateCheck:
	pop {r0-r2}
	cmp r1, #ABILITY_INTIMIDATE
	bne LoadTable
	ldr r0, [r2]
	and r0, r6
	cmp r0, #0x0
	bne NextIter
	ldr r0, =(0x0801BAAD)
	bx r0

LoadTable:
	push {r2-r3}
	mov r3, #0x0
	ldr r0, =SwitchInTable
Loop:
	lsl r2, r3, #0x3
	add r2, r2, r0
	ldrb r2, [r2]
	cmp r1, r2
	beq Success
	cmp r2, #0xFF
	beq PopBeforeNextIter
	add r3, #0x1
	b Loop

Success:
	lsl r2, r3, #0x3
	add r2, r2, r0
	ldr r0, [r2, #0x4]	@ability battle script
	pop {r2-r3}
	push {r0}
	ldr r0, [r2]
	and r0, r6
	cmp r0, #0x0
	bne PopR0BeforeNextIter

EndIt:
	mov r4, r8
	strb r1, [r4]
	ldr r0, [r2]
	orr r0, r6
	str r0, [r2]
	ldr r0, =USER_BANK
	strb r5, [r0]		@ store current bank into user bank
	pop {r0}
	ldr r1, =(0x0801BABB)	@0x0801baba
	bx r1

PopR0BeforeNextIter:
	pop {r0}
	b NextIter

PopBeforeNextIter:
	pop {r2-r3}

NextIter:
	ldr r0, =0x0801B4FF
	bx r0

primal_reversion_bs:
.byte 0	
	
	
@ ANTICIPATION @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
AnticipationBS:
	pause 0x20
	setbyte TARGET_BANK 0x0
	cmde1 0x81D936A
	setbyte FORM_COUNTER 0x0
	copyarray 0x203D81E TARGET_BANK 0x1

firstattackcheck:
	callasm AnticipationCheck+1
	jumpifbyte 0x0 FORM_COUNTER 0x9 anticipation_super
	typecalc
	copyarray TARGET_BANK 0x203D81E 0x1
	jumpifbyte 0x0 OUTCOME 0x2 anticipation_super
	addbyte FORM_COUNTER 0x2
	jumpifbyte 0x0 FORM_COUNTER 0x8 checkamount
	goto firstattackcheck

checkamount:
	setbyte FORM_COUNTER 0x0
	jumpifbyte 0x0 NUM_POKEMON 0x2 anticipation_end
	addbyte TARGET_BANK 0x2
	copyarray 0x203D81E TARGET_BANK 0x1

secondattackcheck:
	callasm AnticipationCheck+1
	jumpifbyte 0x0 FORM_COUNTER 0x9 anticipation_super
	typecalc
	copyarray TARGET_BANK 0x203D81E 0x1
	jumpifbyte 0x0 OUTCOME 0x2 anticipation_super
	addbyte FORM_COUNTER 0x2
	jumpifbyte 0x0 FORM_COUNTER 0x8 anticipation_end
	goto secondattackcheck

anticipation_super:
	setbyte OUTCOME 0x0
	sethalfword CURRENT_MOVE 0x0
	setword BATTLE_STRING_LOADER anticipation_str
	printstring 0x184
	waitmessage 0x40

anticipation_end:
	setbyte 0x203D81E 0x0
	setbyte FORM_COUNTER 0x0
	sethalfword CURRENT_MOVE 0x0
	end3

anticipation_str:
.byte 0


.align 2
AnticipationCheck:
	push {lr}
	ldr r0, =BATTLE_DATA
	ldr r1, =USER_BANK
	ldrb r2, [r1, #0x1] 	@r2=target
	ldrb r3, [r1] 			@r3=user
	strb r3, [r1, #0x1] 	@store user in target
	mov r1, r2 				@target in r1
	mov r2, #0x58
	mul r1, r2
	add r0, r1				@r0=First move loc
	ldr r2, =FORM_COUNTER
	ldrb r2, [r2]
	add r0, r2
	ldrh r1, [r0]	@r1=currentmove

CheckOneHitKO:
	cmp r1, #MOVE_GUILLOTINE
	beq SuperEffective
	cmp r1, #MOVE_HORNDRILL
	beq SuperEffective
	cmp r1, #MOVE_FISSURE
	beq SuperEffective
	ldr r2, =MOVE_SHEERCOLD
	cmp r1, r2
	beq SuperEffective

CheckStatusMove:
	ldr r2, =MOVE_DATA
	mov r3, #0xC
	mul r3, r1
	add r2, r3
	ldrb r2, [r2]
	cmp r2, #0x2
	beq Pop

End:
	ldr r0,	=CURRENT_MOVE
	strh r1, [r0]
Pop:	
	pop {pc}

SuperEffective:
	ldr r0, =FORM_COUNTER
	mov r1, #0x9
	strb r1, [r0]
	pop {pc}
	
	
@ AURA BREAK @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@	
.pool
AuraBreakBS:
	pause 0x20
	setbyte TARGET_BANK 0x0
	cmde1 0x81D936A
	setword BATTLE_STRING_LOADER aura_break_str
	printstring 0x184
	waitmessage 0x30
	sethalfword CURRENT_MOVE 0x0
	end3

aura_break_str:
.byte 0


@ DARK AURA @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@	
.pool
DarkAuraBS:
	pause 0x20
	setbyte TARGET_BANK 0x0
	cmde1 0x81D936A
	setword BATTLE_STRING_LOADER dark_aura_str
	printstring 0x184
	waitmessage 0x30
	sethalfword CURRENT_MOVE 0x0
	end3

dark_aura_str:
.byte 0


@ DELTA STREAM @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@	
.pool
DeltaStreamBS:
	pause 0x20
	setbyte TARGET_BANK 0x0
	cmde1 0x81D936A
	jumpifbyte EQUALS PRIMAL_WEATHER_LOC ABILITY_DELTASTREAM delta_stream_end
	setbyte PRIMAL_WEATHER_LOC ABILITY_DELTASTREAM
	sethalfword WEATHER_FLAGS WEATHER_AIR_CURRENT
	setbyte WEATHER_TIMER 0x0
	playanimation 0x1 ANIM_DELTA_STREAM 0x0
	setword BATTLE_STRING_LOADER delta_stream_str
	printstring 0x184
	waitmessage 0x40
	call 0x81D92DC

delta_stream_end:
	sethalfword CURRENT_MOVE 0x0
	end3

delta_stream_str:
.byte 0


@ DESOLATE LAND @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@	
.pool
DesolateLandBS:
	pause 0x20
	setbyte TARGET_BANK 0x0
	cmde1 0x81D936A
	call init_des_land
	sethalfword CURRENT_MOVE 0x0
	end3

init_des_land:
	jumpifbyte 0x0 PRIMAL_WEATHER_LOC ABILITY_DESOLATELAND des_land_end
	setbyte PRIMAL_WEATHER_LOC ABILITY_DESOLATELAND
	setrain
	setsunny
	orhalfword WEATHER_FLAGS WEATHER_HARSH_SUN+WEATHER_PERMAMENT_SUN
	setbyte WEATHER_TIMER 0x0
	playanimation 0x1 ANIM_SUN 0x0
	setword BATTLE_STRING_LOADER des_land_str
	printstring 0x184
	waitmessage 0x40
	call 0x81D92DC

des_land_end:
	return

des_land_str:
.byte 0


@ DOWNLOAD @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@	
.pool
DownloadBS:
    pause 0x20
    setbyte TARGET_BANK 0x0
    cmde1 0x81D936A
    callasm DownloadASM+1
    statbuffchange STAT_ATTACKER+TRUE download_end
	setgraphicalstatchangevalues
    playanimation BANK_ATTACKER 0x1 0x2023FD4
    setword BATTLE_STRING_LOADER download_str
    printstring 0x184
    waitmessage 0x30
download_end:
    end3
	
download_str:
.byte Buffer, 0x13, Apos, s_, Space, Buffer, 0x1a, Space, r_, a_, i_, s_, e_, d_, JumpLine, i_, t_, s_, Space, Buffer, 0x0, Period, 0xff

.align 2
DownloadASM:
	push {r4-r7, lr}
	
CheckStats:
	bl GetBankDefs	@ return modified def to r0, spdef to r1
	ldr r2, =BATTLE_TYPE
	ldr r2, [r2]
	mov r3, #0x1	@double battle
	and r2, r3
	cmp r2, #0x0
	beq CompStats
	mov r6, r0	@r6 = def 1
	mov r7, r1	@r7 = def 2
	ldrb r0, [r7]
	bl GetPartnerBank
	bl GetBankDefs
	add r0, r6	@ party def sum
	add r1, r7	@ party spdef sum
	
CompStats:
	cmp r0, r1
	bhs RaiseSpAtk	@ def >= SpDef
RaiseAtk:
	ldr r1, =(0x083fd593)	@"Attack"
	mov r0, #0x11
	b StoreBuff
RaiseSpAtk:
	ldr r1, =(0x083fd5a8)	@"Sp. Atk"
	mov r0, #0x14

StoreBuff:
	ldr r2, =(0x02022ab8)	@ FD 00
	str r1, [r2]
	ldr r1, =STAT_CHANGE_BYTE
	strb r0, [r1]
	pop {r4-r7,pc}
	
GetBankDefs:
	push {lr}
	mov r1, #0x58
	mul r0, r1
	ldr r1, =BATTLE_DATA
	add r0, r0, r1
	ldrh r2, [r0, #0x4]	@ r2 = bank base def
	ldrh r3, [r0, #0xA]	@ r3 = bank sp. def
	ldrb r4, [r0, #0x1a]	@ r4 = bank def mod
	ldrb r5, [r0, #0x1d]	@ r5 = bank spdef mod
DefBuffMod:
	cmp r4, #0x6
	beq SpDefMod
	mov r0, r2		@ base def
	mov r1, r4		@ def mod
	bl StatBuffMod	@ modify def
	mov r2, r0		@ new def
SpDefMod:
	cmp r5, #0x6
	beq ReturnStats
	mov r0, r3
	mov r1, r5
	bl StatBuffMod	@ modify spdef
	mov r3, r0		@ new spdef
ReturnStats:
	mov r0, r2
	mov r1, r3
	pop {pc}

StatBuffMod:
	push {r2-r3,lr}
	lsl r1, r1, #0x1
	ldr r2, =(0x0825dead)	@stat buff mod table
	add r2, r1
	ldrb r1, [r2, #0x1]	@ denomenator
	ldrb r3, [r2]		@ numerator
	mul r0, r3
	bl DivideInt
	pop {r2-r3, pc}
	

@ FAIRY AURA @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@	
.pool	
FairyAuraBS:
	pause 0x20
	setbyte TARGET_BANK 0x0
	cmde1 0x81D936A
	setword BATTLE_STRING_LOADER fairy_aura_str
	printstring 0x184
	waitmessage 0x30
	sethalfword CURRENT_MOVE 0x0
	end3

fairy_aura_str:
.byte 0
	
	
	
@ FOREWARN @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
ForewarnBS:
	pause 0x20
	setbyte TARGET_BANK 0x0
	cmde1 0x81D936A
	callasm ForewarnASM+1			@ todo
	setword BATTLE_STRING_LOADER forewarn_str
	printstring 0x184
	waitmessage 0x30
	sethalfword CURRENT_MOVE 0x0

forewarn_end:
	end3

forewarn_str:
.byte 0
	
	
	
@ FRISK @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@	
.pool	
FriskBS:
	pause 0x20
	setbyte TARGET_BANK 0x0
	cmde1 0x81D936A
	call found_item
	jumpifbyte NOTEQUALS NUM_POKEMON 0x4 frisk_end
	addbyte TARGET_BANK 0x2
	call found_item

frisk_end:
	sethalfword CURRENT_MOVE 0x0
	end3

found_item:
	callasm FriskCheck+1
	jumpifhalfword 0x0 ITEM_BUFFER 0x0 return_frisk
	setword BATTLE_STRING_LOADER frisk_str
	printstring 0x184
	waitmessage 0x40

return_frisk:
	return

frisk_str:
.byte 0


.align 2
FriskCheck:
	push {lr}
	ldr r0, =(BATTLE_DATA + 0x2E)
	ldr r1, =TARGET_BANK
	ldrb r1, [r1]
	mov r2, #0x58
	mul r1, r1, r2
	add r0, r0, r1
	ldrh r0, [r0]
	ldr r1, =ITEM_BUFFER
	strh r0, [r1]
	pop {r0}
	bx r0

	
	
@ IMPOSTER @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@	
.pool		
ImposterBS:
	pause 0x20
	setbyte TARGET_BANK 0x0
	cmde1 0x81D936A
	jumpifbyte 0x0 NUM_POKEMON 0x2 init_imposter
	setbyte FORM_COUNTER 0x0
	callasm ImposterASM+1
	jumpifbyte 0x0 FORM_COUNTER 0x0 imposter_end
	setbyte FORM_COUNTER 0x0

init_imposter:
	jumpifsecondarystatus BANK_TARGET STATUS2_TRANSFORMED+STATUS2_SUBSTITUTE imposter_end
	jumpifspecialstatusflag BANK_TARGET STATUS3_ILLUSION+STATUS3_DISAPPEARED+STATUS3_UNDERWATER+STATUS3_UNDERGROUND+STATUS3_INAIR 0x0 imposter_end
	sethalfword CURRENT_MOVE MOVE_TRANSFORM
	transformdataexecution
	attackanimation
	waitanimation
	printfromtable 0x83FE5B4
	waitmessage 0x40
	sethalfword CURRENT_MOVE 0x0

imposter_end:
	end3


.align 2
ImposterASM:
	push {r4-r5,lr}
	ldr r5, =USER_BANK
	ldrb r0, [r5]
	cmp r0, #0x0
	beq Set3
	cmp r0, #0x1
	beq Set2
	cmp r0, #0x2
	beq Set1

Set0:
	mov r0, #0x0
	b ReturnImposter

Set1:
	mov r0, #0x1
	b ReturnImposter

Set2:
	mov r0, #0x2
	b ReturnImposter

Set3:
	mov r0, #0x3

ReturnImposter:
	mov r4, r0
	bl GetBankCurrentHP
	cmp r0, #0x0
	beq ImposterExit
	strb r4, [r5, #0x1]
	ldr r0, =FORM_COUNTER
	mov r1, #0x1
	strb r1, [r0]
ImposterExit:
	pop {r4-r5,pc}


@ MOLD BREAKER @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@	
.pool
MoldBreakerBS:
	pause 0x20
	setbyte TARGET_BANK 0x0
	cmde1 0x81D936A
	setword BATTLE_STRING_LOADER mold_breaker_str
	printstring 0x184
	waitmessage 0x40
	end3

mold_breaker_str:
.byte 0



@ PRIMORDIAL SEA @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@	
.pool
PrimordialSeaBS:
	pause 0x20
	setbyte TARGET_BANK 0x0
	cmde1 0x81D936A
	call init_prim_sea
	sethalfword CURRENT_MOVE 0x0
	end3

init_prim_sea:
	jumpifbyte 0x0 PRIMAL_WEATHER_LOC ABILITY_PRIMORDIALSEA prim_sea_end
	setbyte PRIMAL_WEATHER_LOC ABILITY_PRIMORDIALSEA
	setsunny
	setrain
	orhalfword WEATHER_FLAGS WEATHER_HEAVY_RAIN+WEATHER_PERMAMENT_RAIN
	setbyte WEATHER_TIMER 0x0
	playanimation 0x1 ANIM_RAIN 0x0
	setword BATTLE_STRING_LOADER prim_sea_str
	printstring 0x184
	waitmessage 0x40
	call 0x81D92DC

prim_sea_end:
	return	

prim_sea_str:
.byte 0


@ SNOW WARNING @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@	
.pool
SnowWarningBS:
	call snow_warning
	end3

snow_warning:
	jumpifbyte 0x1 PRIMAL_WEATHER_LOC 0x0 snow_warning_end
	jumpifhalfword 0x4 WEATHER_FLAGS WEATHER_HAIL+WEATHER_PERMAMENT_HAIL snow_warning_end
	pause 0x20
	setbyte TARGET_BANK 0x0
	cmde1 0x81D936A
	sethail
	orhalfword WEATHER_FLAGS WEATHER_PERMAMENT_HAIL
	playanimation 0x1 ANIM_HAIL 0x0
	setword BATTLE_STRING_LOADER snow_warning_str
	printstring 0x184
	waitmessage 0x40
	call 0x81D92DC

snow_warning_end:
	return

snow_warning_str:
.byte 0


@ TERAVOLT @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@	
.pool
TeravoltBS:
	pause 0x20
	setbyte TARGET_BANK 0x0
	cmde1 0x81D936A
	setword BATTLE_STRING_LOADER teravolt_str
	printstring 0x184
	waitmessage 0x30
	sethalfword CURRENT_MOVE 0x0
	end3

teravolt_str:
.byte 0



@ SURGE ABILITIES @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@	
.pool
@================
.global ElectricSurgeBS
	ElectricSurgeBS:
	pause 0x20
	setbyte TARGET_BANK 0x0
	cmde1 0x81D936A
	jumpifbyte EQUALS TERRAIN_BYTE ELECTRIC_TERRAIN surge_end
	playanimation 0x1 ANIM_ELECTRIC_SURGE 0x0
	setbyte TERRAIN_BYTE ELECTRIC_TERRAIN
	callasm SetTerrainCounterASM+1		@ set 5 or 8 turns of terrain
	setword BATTLE_STRING_LOADER elec_surge_str
	printstring 0x184
	waitmessage 0x40
	call seed_check

surge_end:
	sethalfword CURRENT_MOVE 0x0
	end3

elec_surge_str:
.byte 0

@================
.global GrassySurgeBS
GrassySurgeBS:
	pause 0x20
	setbyte TARGET_BANK 0x0
	cmde1 0x81D936A
	jumpifbyte EQUALS TERRAIN_BYTE GRASSY_TERRAIN surge_end
	playanimation 0x1 0x28 0x0
	setbyte TERRAIN_BYTE GRASSY_TERRAIN
	callasm SetTerrainCounterASM+1
	setword BATTLE_STRING_LOADER grassy_surge_str
	printstring 0x184
	waitmessage 0x40
	callasm GrassySurgeASM+1
	call seed_check
	goto surge_end

grassy_surge_str:
.byte 0

@================
.global MistySurgeBS
MistySurgeBS:
	pause 0x20
	setbyte TARGET_BANK 0x0
	cmde1 0x81D936A
	jumpifbyte EQUALS TERRAIN_BYTE MISTY_TERRAIN surge_end
	playanimation 0x1 ANIM_MISTY_SURGE 0x0
	setbyte TERRAIN_BYTE MISTY_TERRAIN
	callasm SetTerrainCounterASM+1
	setword BATTLE_STRING_LOADER misty_surge_str
	printstring 0x184
	waitmessage 0x40
	callasm MistySurgeASM+1
	call seed_check
	goto surge_end

misty_surge_str:
.byte 0

@================
.global PsychicSurgeBS
PsychicSurgeBS:
	pause 0x20
	setbyte TARGET_BANK 0x0
	cmde1 0x81D936A
	jumpifbyte EQUALS TERRAIN_BYTE PSYCHIC_TERRAIN surge_end
	playanimation 0x1 ANIM_PSYCHIC_SURGE 0x0
	setbyte TERRAIN_BYTE PSYCHIC_TERRAIN
	callasm SetTerrainCounterASM+1
	setword BATTLE_STRING_LOADER psychic_surge_str
	printstring 0x184
	waitmessage 0x40
	callasm PsychicTerrainASM+1
	call seed_check
	goto surge_end

psychic_surge_str:
.byte 0


@===================
seed_check:
	setbyte SEED_HELPER 0x0
seed_loop:
	callasm CheckBanksForSeedASM+1
	jumpifbyte EQUALS FORM_COUNTER 0xFF return_seed
	statbuffchange STAT_ATTACKER+TRUE seed_loop
	setgraphicalstatchangevalues
    playanimation BANK_ATTACKER 0x1 0x2023FD4
    setword BATTLE_STRING_LOADER download_str
	printstring 0x184
	waitmessage 0x30
	removeitem 0xA
	addbyte SEED_HELPER 0x1		@ next bank
	goto seed_loop
	
return_seed:
	setbyte SEED_HELPER 0x0
	setbyte FORM_COUNTER 0x0
	return
	
seed_string:
.byte Buffer, 0x13, Apos, s_, Space, Buffer, 0xF, JumpLine, r_, a_, i_, s_, e_, d_, Space, i_, t_, s_, Space, Buffer, 0x0, Period, 0xff	

.align 2
SetTerrainASM:
	push {r4, lr}
	ldr r4, =TERRAIN_COUNTER
	bl GetAttackerHeldItemEffect	@ get user bank hold item
	mov r1, #0x5	@ 5 turns nominal
	cmp r0, #ITEM_EFFECT_TERRAIN_EXTENDER
	bne SetTerrainTurns
	mov r1, #0x8	@ 8 turns with terrain extender
SetTerrainTurns:
	strb r1, [r4]
	pop {r4, pc}
	
	
.align 2
CheckBanksForSeedASM:
	push {r4-r6,lr}
	ldr r0, =TERRAIN_BYTE
	ldrb r0, [r0]
	cmp r0, #0x0
	beq EndSeedLoop
	ldr r5, =FORM_COUNTER
	ldr r4, =SEED_HELPER
	
SeedLoop:
	ldrb r0, [r4]
	ldr r1, =NUM_POKEMON
	ldrb r1, [r1]
	cmp r0, r1
	beq EndSeedLoop
	ldrb r0, [r4]
	ldr r1, =TURN_ORDER
	ldrb r0, [r1, r0]
	ldr r1, =( BATTLE_DATA + 0x2E )
	add r0, r0, r1
	ldrh r0, [r0]	@ curr bank held item
	mov r6, r0
	bl GetItemEffect
	cmp r0, #ITEM_EFFECT_SEEDS
	bne NextSeedBank
	
	ldr r0, =TERRAIN_BYTE
	ldrb r0, [r0]
	cmp r0, #ELECTRIC_TERRAIN
	beq CheckElectricSeed
	cmp r0, #GRASSY_TERRAIN
	beq CheckGrassySeed
	cmp r0, #MISTY_TERRAIN
	beq CheckMistySeed
	cmp r0, #PSYCHIC_TERRAIN
	bne NextSeedBank
	
CheckPsychicSeed:
	mov r0, r6
	bl GetHeldItemParam		@ get item parameter
	cmp r0, #ITEM_PARAM_PSYCHIC_SEED
	bne NextSeedBank
PsychicSeedBoost:
	mov r0, #( STAT_SPDEF + INCREASE_1)
	ldr r1, =(0x083fd5b0)	@"Sp. Def"
	ldr r2, =ITEM_PSYCHIC_SEED
	b StoreSurgeBuff

CheckElectricSeed:
	mov r0, r6
	bl GetHeldItemParam	@ get item parameter
	cmp r0, #ITEM_PARAM_ELECTRIC_SEED
	bne NextSeedBank
ElectricSeedBoost:
	mov r0, #( STAT_DEF + INCREASE_1)
	ldr r1, =(0x083fd59a)	@"Defense"
	ldr r2, =ITEM_ELECTRIC_SEED
	b StoreSurgeBuff
	
CheckGrassySeed:
	mov r0, r6
	bl GetHeldItemParam	@ get item parameter
	cmp r0, #ITEM_PARAM_GRASSY_SEED
	bne NextSeedBank
GrassySeedBoost:
	mov r0, #( STAT_DEF + INCREASE_1)
	ldr r1, =(0x083fd59a)	@"Defense"
	ldr r2, =ITEM_GRASSY_SEED
	b StoreSurgeBuff	
	
CheckMistySeed:
	mov r0, r6
	bl GetHeldItemParam	@ get item parameter
	cmp r0, #ITEM_PARAM_MISTY_SEED
	bne NextSeedBank
MistySeedBoost:
	mov r0, #( STAT_SPDEF + INCREASE_1)
	ldr r1, =(0x083fd5b0)	@"Sp. Def"
	ldr r2, =ITEM_MISTY_SEED	
	
StoreSurgeBuff:
	ldr r3, =ITEM_BUFFER
	strh r2, [r3]
	ldr r2, =(0x02022ab8)	@ Buffer 00
	str r1, [r2]
	ldr r1, =STAT_CHANGE_BYTE
	strb r0, [r1]
	b ReturnSeedCheck
	
NextSeedBank:
	ldrb r0, [r4]
	add r0, #0x1
	strb r0, [r4]
	b SeedLoop
	
EndSeedLoop:
	mov r0, #0xFF
	strb r0, [r5]	@nothing left to check

ReturnSeedCheck:	
	pop {r4-r5,pc}

@ TURBOBLAZE @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@	
.pool
TurboblazeBS:
	pause 0x20
	setbyte TARGET_BANK 0x0
	cmde1 0x81D936A
	setword BATTLE_STRING_LOADER turboblaze_str
	printstring 0x184
	waitmessage 0x30
	sethalfword CURRENT_MOVE 0x0
	end3

turboblaze_str:
.byte 0



@ TURBOBLAZE @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@	
.pool
UnnerveBS:
	pause 0x20
	setbyte TARGET_BANK 0x0
	cmde1 0x81D936A
	jumpifbyte EQUALS USER_BANK 0x0 unnerve_enemy
	jumpifbyte EQUALS USER_BANK 0x1 unnerve_ally
	jumpifbyte EQUALS USER_BANK 0x2 unnerve_enemy

unnerve_ally:
	setword BATTLE_STRING_LOADER unnerve_ally_str
	printstring 0x184
	waitmessage 0x30
	goto unnerve_end

unnerve_enemy:
	setword BATTLE_STRING_LOADER unnerve_enemy_str
	printstring 0x184
	waitmessage 0x30

unnerve_end:
	sethalfword CURRENT_MOVE 0x0
	end3


unnerve_ally_str:
.byte 0

unnerve_enemy_str:
.byte 0




@ CLOUD LOCK & AIR LOCK @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@	
.pool
CloudNine_AirLockBS:
	pause 0x20
	setbyte TARGET_BANK 0x0
	cmde1 0x81D936A
	setword BATTLE_STRING_LOADER weather_null_str
	printstring 0x184
	waitmessage 0x30
	sethalfword CURRENT_MOVE 0x0
	end3

weather_null_str:
.byte T_, h_, e_, Space, e_, f_, f_, e_, c_, t_, s_, Space, o_, f_, Space, w_, e_, a_, t_, h_, e_, r_, JumpLine, d_, i_, s_, a_, p_, p_, e_, a_, r_, e_, d_, Period, 0xff



@ MAGMA ARMOR @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@	
.pool
MagmaArmorBS:
	pause 0x20
	setbyte TARGET_BANK 0x0
	cmde1 0x81D936A
	jumpifstatus BANK_ATTACKER STATUS_FREEZE cure_freeze
	goto end_magma_armor
	
cure_freeze:	
	cureprimarystatus BANK_ATTACKER STATUS_FREEZE
	
	
end_magma_armor:	
	sethalfword CURRENT_MOVE 0x0
	end3
	
magma_armor_str:
.byte Buffer, 0x13, Apos, s_, Space, Buffer, 0x1a, c_, u_, r_, e_, d_, JumpLine, i_, t_, Space, o_, f_, Space, f_, r_, e_, e_, z_, e_, Period, 0xff


@ PRESSURE @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@	
.pool
PressureBS:
	pause 0x20
	setbyte TARGET_BANK 0x0
	cmde1 0x81D936A
	setword BATTLE_STRING_LOADER pressure_str
	printstring 0x184
	waitmessage 0x30
	sethalfword CURRENT_MOVE 0x0
	end3

pressure_str:
.byte Buffer, 0x13, Space, i_, s_, Space, e_, x_, e_, r_, t_, i_, n_, g_, JumpLine, i_, t_, s_, Space, P_, r_, e_, s_, s_, u_, r_, e_, Exclam, 0xff


@ TRACE FIX @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@	
.pool
TraceFix:
	push {r4-r7,lr}
	ldr r6, =(BATTLE_DATA + 0x20)
	ldr r7, =SPECIAL_STATUS_BITS
	mov r5, #0x80
	lsl r5, #0xC
	mov r4, #0x58
	ldr r3, =(0x2023FDB)
	ldrb r2, [r3]
	lsl r1, r2, #0x2
	add r1, r7
	ldr r0, [r1] 
	mul r2, r4
	add r2, r6
	ldrb r2, [r2]
	cmp r2, #ABILITY_INTIMIDATE
	beq orrmarker
	bic r0, r5
	b contTrace
orrmarker:
	bic r0, r5
contTrace: 
	str r0, [r1]
	pop {r4-r7,pc}
*/
	