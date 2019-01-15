.text
.align 2
.thumb
.thumb_func
/*
Contact Abilities:
	Aftermath
	Cursed Body
	Gooey
	Mummy
	Emergency Exit
	Stamina
	Tangling Hair
	Water Compaction
	Anger Point
	Weak Armor
	Steadfast
	Justified
	Rattled
	Wimp Out
	Berserk
	Innards Out

*/

.include "..\\defines"

.global ContactAbilityLoadTable
.global ContactAbilityTable
.global ContactAbilityPreventionCheck
.global AftermathCheck
.global CursedBodyCheck
.global Gooey_TanglingHair_Check
.global MummyCheck
.global EmergencyExit_WimpOut_Check
.global StaminaCheck
.global WaterCompactionCheck
.global AngerPointCheck
.global WeakArmorCheck
.global SteadfastCheck
.global JustifiedCheck
.global RattledCheck
.global BerserkCheck
.global InnardsOutCheck

/*
@ Contact Ability Table @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
ContactAbilityTable:
.byte ABILITY_AFTERMATH,0,0,0
.word AftermathCheck+rom
.byte ABILITY_CURSEDBODY
.word CursedBodyCheck+rom
.byte ABILITY_GOOEY,0,0,0
.word GooeyTanglingHairCheck+rom
.byte ABILITY_EMERGENCYEXIT,0,0,0
.word WimpOutEmergencyExitCheck+rom
.byte ABILITY_STAMINA,0,0,0
.word StaminaCheck+rom
.byte ABILITY_TANGLINGHAIR,0,0,0
.word GooeyTanglingHairCheck+rom
.byte ABILITY_WATERCOMPACTION,0,0,0
.word WaterCompactionCheck+rom
.byte ABILITY_ANGERPOINT,0,0,0
.word AngerPointCheck+rom
.byte ABILITY_WEAKARMOR,0,0,0
.word WeakArmorCheck+rom
.byte ABILITY_STEADFAST,0,0,0
.word SteadfastCheck+rom
.byte ABILITY_JUSTIFIED,0,0,0
.word JustifiedCheck+rom
.byte ABILITY_RATTLED,0,0,0
.word RattledCheck+rom
.byte ABILITY_WIMPOUT,0,0,0
.word WimpOutEmergencyExitCheck+rom
.byte ABILITY_BERSERK,0,0,0
.word BerserkCheck+rom
.byte ABILITY_INNARDSOUT,0,0,0
.word InnardsOutCheck+rom
.byte ABILITY_DISGUISE,0,0,0
.word DisguiseCheck+rom
.word 0x000000ff		@ table ender


@ Load Contact Ability Table @
@ hook at 0x1a8b8 via r1 @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
ContactAbilityLoadTable:
	mov r1, r8
	ldrb r0, [r1]
	push {r0-r7}
	bl SheerForceCheck
	cmp r0, #0x1
	pop {r0}
	beq Pop
	mov r2, #0x0

AtkLoop:
	ldr r1, =ContactAbilityTable
	lsl r3, r2, #0x03
	add r3, r3, r1
	ldrb r1, [r3, #0x0]
	cmp r0, r1
	beq NextAbility
	cmp r1, #0xFF
	beq NoMoreAbilities
	add r2, #0x1
	b AtkLoop
NextAbility:
	ldr r0, [r3, #0x4]
	pop {r1-r7}
	mov pc, r0

NoMoreAbilities:
	pop {r1-r7}
	cmp r0, #ABILITY_IRONBARBS
	bne Next2
SkipAll:
	mov r0, #ABILITY_ROUGHSKIN
	b Next3

Next2:
	mov r1, r8
	ldrb r0, [r1]
Next3:
	sub r0, #0x9
	cmp r0, #0x2F
	bls TableContinue
	bl ReturnNoAbility
TableContinue:
	lsl r0, r0, #0x02
	ldr r1, =(0x0801a8c8 +1)
	bx r1

ReturnNoAbility:
	ldr r4, =(0x801BBAB)
	bx r4

Pop:
	pop {r1-r7}
	b SkipAll


@ CONTACT ABILITY PREVENTION CHECKS @
@ 	LONG REACH, PROTECTIVE PADS 	@
@ hook at 0x1a8c8 via r1 			@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
ContactAbilityPreventionCheck:
	ldr r1, =(0x0801A8D4)	@ ability jump table
	cmp r0, #0x1C			@ 0x1c + 0x9 = 0x25 = huge power
	push {r0-r3}
	beq CheckSheerForce
LongReachPreventionCheck:
	ldr r0, =USER_BANK
	ldrb r0, [r0]
	bl GetBankAbility
	cmp r0, #ABILITY_LONGREACH
	beq PreventEffect

KlutzPreventionCheck:
	cmp r0, #ABILITY_KLUTZ
	beq CheckSheerForce

ProtectivePadsPreventCheck:
	bl GetAttackerHeldItemEffect
	cmp r0, #ITEM_EFFECT_PROTECTIVE_PADS
	bne CheckSheerForce

PreventEffect:
	pop {r0-r3}
	ldr r0, =(0x0801BBAA)
	mov pc, r0

CheckSheerForce:
	bl SheerForceCheck
	cmp r0, #0x1
	beq PreventEffect
	pop {r0-r3}
	add r0, r1
	ldr r0, [r0]	@contact abilities
	mov pc, r0

	
@ AFTERMATH @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
AftermathCheck:
	ldr r0, =USER_BANK
    ldrb r0, [r0, #0x1]
	bl GetBankCurrentHP
	cmp r0, #0x0
	bne ReturnNoScript
	ldr r0, =USER_BANK
	ldrb r0, [r0]
	mov r3, r0
	bl GetBankCurrentHP
	cmp r0, #0x0
	beq ReturnNoScript	

CheckContact:
	bl CheckContactMove
	cmp r0, #0x1
	bne ReturnNoScript

CheckAbilities:
	mov r0, r3
	bl GetBankAbility
	cmp r0, #ABILITY_MAGICGUARD
	beq ReturnNoScript
	cmp r0, #ABILITY_LONGREACH
	beq ReturnNoScript

KlutzCheckAftermath:
	cmp r0, #ABILITY_KLUTZ
	beq Damage
CheckProtectivePads:
	bl GetAttackerHeldItemEffect
	cmp r0, #ITEM_EFFECT_PROTECTIVE_PADS
	beq ReturnNoScript

CheckDamp:
	mov r0, #0x13
	mov r1, #0x0
	mov r2, #ABILITY_DAMP
	mov r3, #0x0
	bl FieldCheck
	cmp r0, #0x0
	bne ReturnNoScript

Damage:
	ldr r0, =USER_BANK
	ldrb r0, [r0]
	bl GetBankCurrentHP
	ldrh r0, [r1, #0x4]
	lsr r0, #0x2
	cmp r0, #0x0
	bne Store
	mov r0, #0x1
Store:
	ldr r1, =DAMAGE_LOC
	str r0, [r1]
	
AftermathBS:
	bl ScriptPusher
	ldr r0, =SCRIPT_LOC
	ldr r1, =(0x081D94A3)
	str r1, [r0]
	ldr r2, =(0x0801ABD9)
	bx r2
	
@ ANGER POINT @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
AngerPointCheck:
	ldr r0, =HIT_FLAG
	ldrb r0, [r0]
	cmp r0, #0x2
	bne ReturnNoScript

NoHPCheck:
	ldr r0, =BATTLE_DATA
	ldr r1, =TARGET_BANK
	ldrb r1, [r1]
	mov r2, #0x58
	mul r1, r2
	add r0, r1
	ldrh r1, [r0, #0x28]
	cmp r1, #0x0
	beq ReturnNoScript

SubstituteCheck:
	mov r1, r0
	add r1, #0x50
	ldr r1, [r1]
	mov r2, #0x80
	lsl r2, #17
	and r1, r2
	cmp r1, #0x0
	bne ReturnNoScript

MaxAttackCheck:
	add r0, #0x19
	ldrb r1, [r0]
	cmp r1, #0xC
	beq ReturnNoScript

IncreaseAttack:
	mov r1, #0xB
	strb r1, [r0]

AngerPointBS:
	bl ScriptPusher
	ldr r0, =SCRIPT_LOC
	ldr r1, =anger_point_script+rom
	str r1, [r0]
	ldr r2, =0x0801ABD9	@play script
	bx r2
	
anger_point_script:
setbyte 0x2023FDF 0x0
playstatchangeanimation 0x0 0x2 0x0
setbyte 0x2023FDE 0x11
statbuffchange BANK_TARGET+FALSE anger_point_end+rom
jumpifbyte 0x0 STAT_CHANGE_BYTE GREATERTHAN anger_point_end+rom
setword 0x203C020 anger_point_str+rom
printstring 0x184
waitmessage 0x40

anger_point_end:
return

@ BERSERK @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
BerserkCheck:
	ldr r2, =DAMAGE_STRUCT
	ldr r0, =USER_BANK
	add r0, #0x1
	ldrb r1, [r0]
	lsl r0, r1, #0x2
	add r0, r1
	lsl r1, r0, #0x2
	add r0, r2, #0x4
	add r0, r1
	ldr r1, [r0]
	cmp r1, #0x0
	beq ReturnNoScript

CheckHalfHP:
	ldr r0, =(BATTLE_DATA + 0x28)
	ldr r1, =USER_BANK
	ldrb r1, [r1, #0x1]
	mov r2, #0x58
	mul r1, r2
	add r0, r1
	ldrh r2, [r0, #0x4]	@r2 is max health
	ldrh r0, [r0]		@r0 is current health
	lsr r3, r2, #0x1	@r3 is half health
	cmp r0, r3
	bgt ReturnNoScript

CheckIfFellThisTurn:
	ldr r1, =DAMAGE_LOC
	ldrh r1, [r1]
	add r0, r1		@r0 is health before attack
	cmp r0, r3
	blt ReturnNoScript

BerserkBS:
	bl ScriptPusher
	ldr r0, =SCRIPT_LOC
	ldr r1, =berserk_bs+rom
	str r1, [r0]
	ldr r2, =(0x801ABD9)
	bx r2

berserk_bs:
jumpiffainted 0x0 end_berserk+rom
jumpifstat 0x0 0x0 0x4 0xC end_berserk+rom
setbyte 0x2023FDF 0x0
playstatchangeanimation 0x0 0x10 0x0
setbyte 0x2023FDE 0x14
statbuffchange BANK_TARGET+FALSE end_berserk+rom
jumpifbyte 0x0 STAT_CHANGE_BYTE GREATERTHAN end_berserk+rom
setword 0x203c020 berserk_str+rom
printstring 0x184
waitmessage 0x40

end_berserk:
return

@ CURSED BODY @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
CursedBodyCheck:
	ldr r2, =DAMAGE_STRUCT
	ldr r0, =USER_BANK
	add r0, #0x1
	ldrb r1, [r0]
	lsl r0, r1, #0x2
	add r0, r1
	lsl r1, r0, #0x2
	add r0, r2, #0x4
	add r0, r1
	ldr r1, [r0]
	cmp r1, #0x0
	beq ReturnNoScript
	ldr	r0, =USER_BANK
	ldrb r0, [r0]
	bl GetBankAbility
	cmp	r0, #ABILITY_AROMAVEIL
	beq	ReturnNoScript

Randomize:
	ldr r0, =(0x020370b8)
	mov r1, #0xFF
	strb r1, [r0]
	strb r1, [r0, #0x1]
	bl GetRandom
	ldr r0, =(0x020370b8)
	add r0, #0x18 	@LastResult
	ldrh r0, [r0]
	ldrh r1, =0x4CCC 	@30%
	cmp r0, r1
	bgt ReturnNoScript

CursedBodyBS:
	bl ScriptPusher
	ldr r0, =SCRIPT_LOC
	ldr r1, =cursed_body_bs+rom
	str r1, [r0]
	ldr r2, =(0x0801ABD9)
	bx r2
		
cursed_body_bs:
jumpiffainted 0x1 cursed_body_end+rom
jumpifsecondarystatus 0x0 0x1000000 cursed_body_end+rom
copyarray FORM_COUNTER TARGET_BANK 0x1
copyarray TARGET_BANK USER_BANK 0x1
copyarray USER_BANK FORM_COUNTER 0x1
jumpifabilitypresenttargetfield ABILITY_AROMAVEIL cursed_body_nrm+rom
disablelastusedattack cursed_body_nrm+rom
setword 0x203C020 cursed_body_str+rom
printstring 0x184
waitmessage 0x40

cursed_body_nrm:
copyarray USER_BANK TARGET_BANK 0x1
copyarray TARGET_BANK FORM_COUNTER 0x1
setbyte FORM_COUNTER 0x0

cursed_body_end:
return

@ CUTE CHARM @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
CuteCharmCheck:
	add r0, r5
	add r0, #0x20
	ldrb r0, [r0]
	cmp r0, #ABILITY_OBLIVIOUS
	beq PreventAffection

LongReachCheck:
	cmp r0, #ABILITY_LONGREACH
	beq PreventAffection

CuteCharmKlutzCheck:
	cmp r0, #ABILITY_KLUTZ
	beq AromaVeilCheck

ProtectivePadsCheck:
	push {r0-r3}
	bl GetAttackerHeldItemEffect		@ get hold item effect of attacker bank
	cmp r0, #ITEM_EFFECT_PROTECTIVE_PADS
	pop {r0-r3}
	beq PreventAffection
	
AromaVeilCheck:
	ldrb r3, [r7, #0x1] @r3 = Pokemon with CuteCharm
	ldrb r0, [r7]       @Attacker
	strb r0, [r7, #0x1]
	push {r3}
	mov r2, #ABILITY_AROMAVEIL
	bl CheckTargetFieldBL	@ check field for aroma veil
	pop {r3}
	strb r3, [r7, #0x1]
	cmp r0, #0x0
	beq ReturnCuteCharm

PreventAffection:
	ldr r0, =(0x0801AF5B)
	bx r0

ReturnCuteCharm:
	ldr r0, =(0x0801AF5F)
	bx r0

@ DISGUISE @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
DisguiseCheck:
	ldr r0, =TARGET_BANK
	ldrb r0, [r0]
    ldr r1, =MIMIKYU_COUNTER
	ldrb r0, [r1, r0]
	cmp r0, #0x1
	bne ReturnNoScript

DisguiseBS:
	bl ScriptPusher
	ldr r0, =SCRIPT_LOC
	ldr r1, =disguise_bs+rom
	str	r1, [r0]
	ldr r0, =(0x0801ABD9)
	bx r0
	
disguise_bs:
jumpiffainted 0x0 disguise_end+rom
setword 0x203c020 disguise_str1+rom
printstring 0x184
callasm asm_disguise+rom+1		@ transform to busted form
setword MIMIKYU_COUNTER 0x0
playanimation 0x0 0x1C 0x0
setword 0x203c020 disguise_str2+rom
printstring 0x184
waitmessage 0x40

disguise_end:
return

.align 2
asm_disguise:
check_pkmn_id:
	push {lr}
	ldr r0, =TARGET_BANK
	ldrb r0, [r0]
	bl GetBankPartyData
	mov r2, r0
	push {r2}
	ldr r0, =TARGET_BANK
	ldrb r0, [r0]
	bl GetBankAbility
	sub r1, #0x20
	mov r0, r1
	pop {r2}

set_busted_forme:
 	add r2, r2, #0x20
	ldr r1, =PKMN_MIMIKYU_BUSTED
 	strh r1, [r0] 		@species in battle struct is now the correct thing
 	strh r1, [r2] 		@species in party is now school's
 	sub r2, r2, #0x20 @makes r2 go to the beginning of the party again
 	push {r0}
 	mov r0, r2
 	mov r1, r2
 	push {r2}
 	ldr r3, =(0x0803E47C)
 	bl jump 		@stats should be calculated
 	pop {r2}
 	pop {r0} 		@r2 is the beginning of the party slot, r0 is the beginning of the battle struct
update_type:
 	add r0, #0x21
 	mov r1, #0x7
 	strb r1, [r0] 		@first type in battle struct is now ghost
 	mov r1, #0x17
 	strb r1, [r0, #0x1] @second type in battle struct is now fairy
update_battle_structure:
 	sub r0, #0x1F
 	add r2, r2, #0x5A 	@r0 is at the attack stat, r2 is at the party attack stat
 	mov r3, #0x0
stat_loop:
 	ldrh r1, [r2]
 	strh r1, [r0]
 	add r0, r0, #0x2
 	add r2, r2, #0x2
 	add r3, r3, #0x1
 	cmp r3, #0x5
 	bne stat_loop
AcknowledgeDamage:
	ldr r2, =DAMAGE_STRUCT
	ldr r0, =TARGET_BANK
	ldrb r1, [r0]
	lsl r0, r1, #0x2
	add r0, r1
	lsl r1, r0, #0x2
	add r0, r2, #0x4
	add r0, r1
	mov r1, #0x1
	str r1, [r0]
end:
 	pop {pc}
jump:
 	mov pc, r3
	


@ FLOWER GIFT CONTACT @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
FlowerGiftContactCheck:
	push {r5}
	ldr r0, =BATTLE_DATA
	ldr r1, =USER_BANK
	ldrb r1, [r1, #0x1]
	mov r2, #0x58
	mul r1, r2
	add r0, r1
CheckHP:
	add r0, #0x28
	ldrh r1, [r0]
	cmp r1, #0x0
	beq PopReturnNoScript
CheckPokemon:
	sub r0, #0x28	
	ldrh r1, [r0]
	add r5, r1, #0x0
	ldr r2, =PKMN_CHERRIM
	cmp r1, r2
	beq Overcast
	ldr r2, =PKMN_CHERRIMSUN
	cmp r1, r2
	bne PopReturnNoScript
Sunshine:
	add r0, #0x20
	ldrb r1, [r0]
	cmp r1, #ABILITY_FLOWERGIFT
	bne ChangeToOvercast
	ldr r0, =WEATHER_FLAGS
	ldrb r0, [r0]
	cmp r0, #WEATHER_PERMAMENT_SUN
	bhi ChangeToOvercast
	cmp r0, #WEATHER_SUN
	beq PopReturnNoScript
ChangeToOvercast:
	ldr r0, =DP08_POINTER
 	ldr r0, [r0]
 	add r0, r0, #0x60
	ldr r1, =USER_BANK
	ldrb r1, [r1]
 	cmp r1, #0x0
 	beq set_player_singles
 	cmp r1, #0x1
 	beq set_enemy_singles
 	cmp r1, #0x2
 	beq set_player_doubles
 	cmp r1, #0x3
 	beq set_enemy_doubles
getbattlestruct:
	ldr r1, =USER_BANK
	ldrb r1, [r1]
 	mov r0, #0x58 @length of battle structs
 	mul r1, r0, r1 @multiplies length of battle structs with which mon did the attack to get the the battle structure i'm working with
 	ldr r0, =BATTLE_DATA
 	add r0, r1, r0 @r0 is the battle struct i'm working with
setovercastform:
 	add r2, r2, #0x20
	mov r1, #0xED
	lsl r1, #0x1
jumpback_fg:
 	strh r1, [r0] 	@species in battle struct is now the correct thing
 	strh r1, [r2] 	@species in party is now overcast's
	push {r0-r2}
	ldr r0, =USER_BANK
	ldrb r1, [r0, #0x1]		@r1=target
	ldrb r2, [r0] 			@r1=user
	strb r1, [r0]
	ldr	r3, =0x08017545
	bl jump
	ldr r0, =SCRIPT_LOC
	ldr r1, =transform_bs+rom
	str r1, [r0]
	strb r2, [r0]
	ldr r2, =(0x801ABD9)
	bx r2
	pop {r0-r2}
 	sub r2, r2, #0x20 @makes r2 go to the beginning of the party again
 	push {r0}
 	mov r0, r2
 	mov r1, r2
 	push {r2}
 	ldr r3, =(0x0803E47C)
 	bl jump_flower_gift 	@stats should be calculated
 	pop {r2}
 	pop {r0} 	@r2 is the beginning of the party slot, r0 is the beginning of the battle struct
update_battle_struct:
 	add r0, #0x2
 	add r2, r2, #0x5A 	@r0 is at the attack stat, r2 is at the party attack stat
 	mov r3, #0x0
flowr_gift_stat_loop:
 	ldrh r1, [r2]
 	strh r1, [r0]
 	add r0, r0, #0x2
 	add r2, r2, #0x2
 	add r3, r3, #0x1
 	cmp r3, #0x5
 	bne flowr_gift_stat_loop
PopReturnNoScript:
	pop {r5}
	b ReturnNoScript

jump_flower_gift:
 	mov pc, r3

Overcast2:
	b Overcast

set_player_singles:
 	ldrb r0, [r0]
 	lsr r0, #0x4 	@only one digit is left
 	mov r1, #100 	@length of party entry
 	ldr r2, =PARTY_PLAYER
 	mul r0, r0, r1
 	add r2, r0, r2 	@gets r2 to the correct slot
	lsr r5, #0x1
	cmp r5, #0xED
	beq getbattlestruct2
 	b getbattlestruct
set_enemy_singles:
 	add r0, r0, #0x3 	@gets to party order for enemy
 	ldrb r0, [r0]
 	lsr r0, #0x4
 	mov r1, #100
 	ldr r2, =PARTY_OPPONENT
 	mul r0, r0, r1
 	add r2, r0, r2
	lsr r5, #0x1
	cmp r5, #0xED
	beq getbattlestruct2
 	b getbattlestruct
set_player_doubles:
 	ldrb r0, [r0]
 	lsl r0, #0x1C
 	lsr r0, #0x1C @gets the second pokemon's location
 	mov r1, #100
 	ldr r2, =PARTY_PLAYER
 	mul r0, r0, r1
 	add r2, r0, r2
	lsr r5, #0x1
	cmp r5, #0xED
	beq getbattlestruct2
 	b getbattlestruct
set_enemy_doubles:
 	add r0, r0, #0x3
 	ldrb r0, [r0]
 	lsl r0, #0x1C
 	lsr r0, #0x1C
 	mov r1, #100
 	ldr r2, =PARTY_OPPONENT
 	mul r0, r0, r1
 	add r2, r0, r2
	lsr r5, #0x1
	cmp r5, #0xED
	beq getbattlestruct2
 	b getbattlestruct

Overcast:
	add r0, #0x20
	ldrb r1, [r0]
	cmp r0, #ABILITY_FLOWERGIFT
	bne PopReturnNoScript
	ldr r0, =WEATHER_FLAGS
	ldrb r0, [r0]
	cmp r0, #0x40
	bhi PopReturnNoScript
	cmp r0, #0x20
	bne PopReturnNoScript
ChangeToSunshine:
	ldr r0, =DP08_POINTER
 	ldr r0, [r0]
 	add r0, r0, #0x60
	ldr r1, =USER_BANK
	ldrb r1, [r1]
 	cmp r1, #0x0
 	beq set_player_singles
 	cmp r1, #0x1
 	beq set_enemy_singles
 	cmp r1, #0x2
 	beq set_player_doubles
 	cmp r1, #0x3
 	beq set_enemy_doubles
getbattlestruct2:
	ldr r1, =USER_BANK
	ldrb r1, [r1]
 	mov r0, #0x58 	@length of battle structs
 	mul r1, r0, r1 	@multiplies length of battle structs with which mon did the attack to get the the battle structure i'm working with
 	ldr r0, =BATTLE_DATA
 	add r0, r1, r0 	@r0 is the battle struct i'm working with
setsunshineform:
 	add r2, r2, #0x20
	ldr r1, =PKMN_CHERRIMSUN
	b jumpback_fg

	
transform_bs:
nop3
nop3
playanimation 0x1 0x23 0x0
printstring 0x13A
waitmessage 0x40
return

@ GOOEY & TANGLING HAIR @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
GooeyTanglingHairCheck:
	ldr r2, =DAMAGE_STRUCT
	ldr r0, =USER_BANK
	add r0, #0x1
	ldrb r1, [r0]
	lsl r0, r1, #0x2
	add r0, r1
	lsl r1, r0, #0x2
	add r0, r2, #0x4
	add r0, r1
	ldr r1, [r0]
	cmp r1, #0x0
	beq ReturnNoScript

CheckContactGTH:
	bl CheckContactMoveBL
	cmp r0, #0x1
	bne	ReturnNoScript

LoadAbility:
	ldr	r0, =(BATTLE_DATA + 0x20)
	ldr r1, =USER_BANK
	ldrb r1, [r1]
	mov r2, #0x58
	mul r1, r2
	add r0, r1
	ldrb r1, [r0]
CheckAbilityGTH:
	cmp r1, #ABILITY_CLEARBODY
	beq ReturnNoScript
	cmp r1, #ABILITY_FULLMETALBODY
	beq ReturnNoScript
	cmp r1, #ABILITY_WHITESMOKE
	beq ReturnNoScript
	cmp	r1, #ABILITY_LONGREACH
	beq	ReturnNoScript

GrassCheck:
	push {r1}
	ldrb r1, [r0, #0x1]
	ldrb r2, [r0, #0x2]
CheckPrimaryTypeGrass:
	cmp r1, #TYPE_GRASS
	beq FlowerVeilCheck
CheckSecondaryTypeGrass:
	cmp r2, #TYPE_GRASS
	bne PreventGooeyCheck
FlowerVeilCheck:
	ldr r1, =USER_BANK
	ldrb r2, [r1, #0x1]	@r2=target
	ldrb r0, [r1]		@r0=attacker
	strb r0, [r1, #0x1]	@attacker is in targetbank
	push {r1-r2}
	mov r2, #ABILITY_FLOWERVEIL
	bl CheckTargetFieldBL
	pop {r1-r2}
	strb r2, [r1, #0x1]	@target is back in targetbank
	cmp r0, #0x0
	bne PopAndReturn

PreventGooeyCheck:
	pop {r1}
	cmp	r1, #ABILITY_KLUTZ
	beq	GooeyBS
	bl GetAttackerHeldItemEffect
	cmp r0, #ITEM_EFFECT_PROTECTIVE_PADS
	beq	ReturnNoScript	

GooeyBS:
	bl ScriptPusher
	ldr r0, =SCRIPT_LOC
	ldr r1, =gooey_bs+rom
	str r1, [r0]
	ldr r2, =(0x801ABD9)
	bx r2

PopAndReturn:
	pop {r1}
	b ReturnNoScript

gooey_bs:
jumpiffainted 0x1 gooey_end+rom
jumpifsecondarystatus 0x0 0x1000000 gooey_end+rom
jumpifstat 0x1 0x0 0x3 0x0 gooey_end+rom
setbyte 0x2023FDF 0x0
playstatchangeanimation 0x1 0x8 0x9
setbyte STAT_CHANGE_BYTE 0x93
statbuffchange 0x80+TRUE gooey_end+rom
jumpifbyte 0x0 STAT_CHANGE_BYTE 0x2 gooey_end+rom
setword 0x203c020 gooey_str+rom
printstring 0x184
waitmessage 0x40

gooey_end:
return

@ INNARDS OUT @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
InnardsOutCheck:
	ldr r2, =DAMAGE_STRUCT
	ldr r0, =USER_BANK
	add r0, #0x1
	ldrb r1, [r0]
	lsl r0, r1, #0x2
	add r0, r1
	lsl r1, r0, #0x2
	add r0, r2, #0x4
	add r0, r1
	ldr r1, [r0]
	cmp r1, #0x0
	beq ReturnNoScript

CheckStatusMove:
	bl GetMoveSpecialty
	cmp r0, #0x2
	beq ReturnNoScript

CheckZeroHP:
	ldr r0, =(BATTLE_DATA + 0x28)
	ldrb r1, =USER_BANK
	ldrb r1, [r1, #0x1]
	mov r2, #0x58
	mul r1, r2
	add r0, r1
	ldrh r0, [r0]		@r0 is current health
	cmp r0, #0x0
	bne ReturnNoScript

InnardsOutBS:
	bl ScriptPusher
	ldr r0, =SCRIPT_LOC
	ldr r1, =innards_out_bs+rom
	str r1, [r0]
	ldr r2, =(0x801ABD9)
	bx r2
	
innards_out_bs:
jumpifability 0x1 ABILITY_MAGICGUARD innards_end+rom
jumpifsecondarystatus 0x1 0x1000000 innards_end+rom
copyarray DAMAGE_LOC 0x2023D54 0x2		@ copy damage
graphicalhpupdate 0x1
datahpupdate 0x1
setword 0x203C020 innards_out_str+rom
printstring 0x184
waitmessage 0x40
faintpokemon 0x1 0x0 0x0

innards_end:
return

@ JUSTIFIED @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
JustifiedCheck:
	ldr r2, =DAMAGE_STRUCT
	ldr r0, =USER_BANK
	add r0, #0x1
	ldrb r1, [r0]
	lsl r0, r1, #0x2
	add r0, r1
	lsl r1, r0, #0x2
	add r0, r2, #0x4
	add r0, r1
	ldr r1, [r0]
	cmp r1, #0x0
	beq ReturnNoScript

CheckDark:
	bl GetMoveType
	cmp r0, #TYPE_DARK
	bne	ReturnNoScript

JustifiedBS:
	bl ScriptPusher
	ldr r0, =SCRIPT_LOC
	ldr r1, =justified_bs+rom
	str r1, [r0]
	ldr r2, =(0x0801ABD9)
	bx r2
	
justified_bs:
jumpiffainted 0x0 justified_end+rom
jumpifsecondarystatus 0x0 0x1000000 justified_end+rom
jumpifstat 0x0 0x0 0x1 0xC justified_end+rom
setbyte 0x2023FDF 0x0
playstatchangeanimation 0x0 0x2 0x0
setbyte 0x2023FDE 0x11
statbuffchange 0x0+FALSE justified_end+rom		@buff target, can fail
jumpifbyte 0x0 STAT_CHANGE_BYTE 0x2 justified_end+rom
setword 0x203c020 justified_str+rom
printstring 0x184
waitmessage 0x40

justified_end:
return

@ MUMMY @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
MummyCheck:
	ldr r2, =DAMAGE_STRUCT
	ldr r0, =USER_BANK
	add r0, #0x1
	ldrb r1, [r0]
	lsl r0, r1, #0x2
	add r0, r1
	lsl r1, r0, #0x2
	add r0, r2, #0x4
	add r0, r1
	ldr r1, [r0]
	cmp r1, #0x0
	beq ReturnNoScript

CheckMummyPrevention:
	ldr	r0, =(BATTLE_DATA + 0x20)
	ldrb r1, =USER_BANK
	ldrb r1, [r1]
	mov r2, #0x58
	mul r1, r2
	add r0, r1
	ldrb r1, [r0]	
	cmp	r1, #ABILITY_MUMMY
	beq	ReturnNoScript
	cmp	r1, #ABILITY_BATTLEBOND
	beq	ReturnNoScript
	cmp	r1, #ABILITY_DISGUISE
	beq	MimikyuCheck
	cmp	r1, #ABILITY_RKSSYSTEM
	beq	ReturnNoScript
	cmp	r1, #ABILITY_SCHOOLING
	beq	ReturnNoScript
	cmp	r1, #ABILITY_SHIELDSDOWN
	beq	ReturnNoScript
	cmp	r1, #ABILITY_MULTITYPE
	beq	ReturnNoScript
	cmp	r1, #ABILITY_STANCECHANGE
	beq	ReturnNoScript
	cmp	r1, #ABILITY_LONGREACH
	beq	ReturnNoScript
	cmp	r1, #ABILITY_KLUTZ
	beq	CheckMummyContact
CheckProtectivePadsMummy:
	bl GetAttackerHeldItemEffect		
	cmp r0, #ITEM_EFFECT_PROTECTIVE_PADS
	beq	ReturnNoScript	

CheckMummyContact:
	bl CheckContactMoveBL
	cmp r0, #0x1
	bne	ReturnNoScript

MummyBS:
	bl ScriptPusher
	ldr r0, =SCRIPT_LOC
	ldr r1, =mummy_bs+rom
	str r1, [r0]
	ldr r2, =(0x801ABD9)
	bx r2

MimikyuCheck:
	mov	r1, r0
	sub	r1, #0x20
	ldrh r1, [r1]
	ldr r2, =PKMN_MIMIKYU
	cmp	r1, r2
	bne	CheckProtectivePadsMummy

mummy_bs:
jumpiffainted 0x1 mummy_end+rom
jumpifsecondarystatus 0x0 0x1000000 mummy_end+rom
setability 0x1 ABILITY_MUMMY
setword BATTLE_STRING_LOADER mummy_str+rom
printstring 0x184
waitmessage 0x40

mummy_end:
return

@ RATTLED @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
RattledCheck:
	ldr r2, =DAMAGE_STRUCT
	ldr r0, =USER_BANK
	add r0, #0x1
	ldrb r1, [r0]
	lsl r0, r1, #0x2
	add r0, r1
	lsl r1, r0, #0x2
	add r0, r2, #0x4
	add r0, r1
	ldr r1, [r0]
	cmp r1, #0x0
	beq ReturnNoScript

CheckDarkBugGhost:
	bl GetMoveType
	cmp r0, #TYPE_BUG
	beq RattledBS
	cmp r0, #TYPE_GHOST
	beq RattledBS
	cmp r0, #TYPE_DARK
	bne	ReturnNoScript

RattledBS:
	bl ScriptPusher
	ldr r0, =SCRIPT_LOC
	ldr r1, =rattled_bs+rom
	str r1, [r0]
	ldr r2, =(0x801ABD9)
	bx r2


rattled_bs:
jumpiffainted 0x0 rattled_end+rom
jumpifsecondarystatus 0x0 0x1000000 rattled_end+rom
jumpifstat 0x0 0x0 0x3 0xC rattled_end+rom
setbyte 0x2023FDF 0x0
playstatchangeanimation 0x0 0x8 0x0
setbyte STAT_CHANGE_BYTE 0x13
statbuffchange 0x0+FALSE rattled_end+rom
jumpifbyte 0x0 STAT_CHANGE_BYTE 0x2 rattled_end+rom
setword 0x203c020 rattled_str+rom
printstring 0x184
waitmessage 0x40

rattled_end:
return

@ STAMINA @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
StaminaCheck:
	ldr r2, =DAMAGE_STRUCT
	ldr r0, =USER_BANK
	add r0, #0x1
	ldrb r1, [r0]
	lsl r0, r1, #0x2
	add r0, r1
	lsl r1, r0, #0x2
	add r0, r2, #0x4
	add r0, r1
	ldr r1, [r0]
	cmp r1, #0x0
	beq ReturnNoScript

StaminaBS:
	bl ScriptPusher
	ldr r0, =SCRIPT_LOC
	ldr r1, =stamina_bs+rom
	str r1, [r0]
	ldr r2, =(0x801ABD9)
	bx r2

	
stamina_bs:
jumpiffainted 0x0 stamina_end+rom
jumpifsecondarystatus 0x0 0x1000000 stamina_end+rom
jumpifstat 0x0 0x0 0x2 0xC stamina_end+rom
setbyte 0x2023FDF 0x0
playstatchangeanimation 0x0 0x4 0x0
setbyte STAT_CHANGE_BYTE 0x12
statbuffchange 0x0+FALSE stamina_end+rom
jumpifbyte 0x0 STAT_CHANGE_BYTE 0x2 stamina_end+rom
setword 0x203c020 stamina_str+rom
printstring 0x184
waitmessage 0x40

stamina_end:
return

@ STEADFAST @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
SteadfastCheck:
	bl ScriptPusher
	ldr r0, =SCRIPT_LOC
	ldr r1, =steadfast_bs+rom
	str r1, [r0]
	ldr r2, =(0x801ABD9)
	bx r2
	
steadfast_bs:
jumpifsecondarystatus 0x0 0x00000008 do_steadfast+rom
return

do_steadfast:
jumpiffainted 0x0 steadfast_end+rom
jumpifstat 0x0 0x0 0x3 0xC steadfast_end+rom
setbyte 0x2023FDF 0x0
playstatchangeanimation 0x0 0x8 0x0
setbyte STAT_CHANGE_BYTE 0x13
statbuffchange 0x0+FALSE steadfast_end+rom
jumpifbyte 0x0 STAT_CHANGE_BYTE 0x2 steadfast_end+rom
setword 0x203c020 steadfast_str+rom
printstring 0x184
waitmessage 0x40

steadfast_end:
return

@ WATER COMPACTION @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
WaterCompactionCheck:
	ldr r2, =DAMAGE_STRUCT
	ldr r0, =USER_BANK
	add r0, #0x1
	ldrb r1, [r0]
	lsl r0, r1, #0x2
	add r0, r1
	lsl r1, r0, #0x2
	add r0, r2, #0x4
	add r0, r1
	ldr r1, [r0]
	cmp r1, #0x0
	beq ReturnNoScript

CheckWater:
	bl 	GetMoveType
	cmp r0, #TYPE_WATER
	bne	ReturnNoScript

WaterCompactionBS:
	bl ScriptPusher
	ldr r0, =SCRIPT_LOC
	ldr r1, =water_compaction_bs+rom
	str r1, [r0]
	ldr r2, =(0x801ABD9)
	bx r2
	
water_compaction_bs:
jumpiffainted 0x0 water_compaction_end+rom
jumpifsecondarystatus 0x0 0x1000000 water_compaction_end+rom
jumpifstat 0x0 0x0 0x2 0xC water_compaction_end+rom
setbyte 0x2023FDF 0x0
playstatchangeanimation 0x0 0x4 0x0
setbyte STAT_CHANGE_BYTE 0x22
statbuffchange 0x0+FALSE water_compaction_end+rom
jumpifbyte 0x0 STAT_CHANGE_BYTE 0x2 water_compaction_end+rom
setword 0x203c020 water_compaction_str+rom
printstring 0x184
waitmessage 0x40

water_compaction_end:
return

@ WEAK ARMOR @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
WeakArmorCheck:
	ldr r2, =DAMAGE_STRUCT
	ldr r0, =USER_BANK
	ldrb r1, [r0, #0x1]
	lsl r0, r1, #0x2
	add r0, r1
	lsl r1, r0, #0x2
	add r0, r2, #0x4
	add r0, r1
	ldr r1, [r0]
	cmp r1, #0x0
	beq ReturnNoScript

CheckPhysical:
	bl GetMoveSpecialty
	cmp r0, #0x0
	bne ReturnNoScript

WeakArmorBS:
	bl ScriptPusher
	ldr r0, =SCRIPT_LOC
	ldr r1, =weak_armor_bs+rom
	str r1, [r0]
	ldr r2, =(0x801ABD9)
	bx r2
	
weak_armor_bs:
jumpiffainted 0x0 weak_armor_end+rom
jumpifsecondarystatus 0x0 0x1000000 weak_armor_end+rom
jumpifstat 0x0 0x0 0x2 0x0 weak_armor_speed+rom
setbyte 0x2023FDF 0x0
playstatchangeanimation 0x0 0x4 0x9
setbyte STAT_CHANGE_BYTE 0x92
statbuffchange 0x0+FALSE weak_armor_speed+rom
jumpifbyte 0x0 STAT_CHANGE_BYTE 0x2 weak_armor_speed+rom
setword 0x203c020 lower_def_str+rom
printstring 0x184
waitmessage 0x40

weak_armor_speed:
jumpifstat 0x0 0x0 0x3 0xC weak_armor_end+rom
setbyte 0x2023FDF 0x0
playstatchangeanimation 0x0 0x8 0x0
setbyte STAT_CHANGE_BYTE 0x23
statbuffchange 0x0+FALSE weak_armor_end+rom
jumpifbyte 0x0 STAT_CHANGE_BYTE 0x2 weak_armor_end+rom
setword 0x203c020 raise_speed_str+rom
printstring 0x184
waitmessage 0x40

weak_armor_end:
return

@ WIMP OUT & EMERGENCY EXIT @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
WimpOutEmergencyExitCheck:
	ldr r2, =DAMAGE_STRUCT
	ldr r0, =USER_BANK
	add r0, #0x1
	ldrb r1, [r0]
	lsl r0, r1, #0x2
	add r0, r1
	lsl r1, r0, #0x2
	add r0, r2, #0x4
	add r0, r1
	ldr r1, [r0]
	cmp r1, #0x0
	beq ReturnNoScript
	
CheckTargetFainted:
	ldr r0, =TARGET_BANK
	ldrb r0, [r0]
	bl GetBankCurrentHP
	cmp r0, #0x0
	beq ReturnNoScript
	
CheckMultiHit:
	ldr r0, =MULTI_HIT_COUNTER
	ldrb r0, [r0]
	cmp r0, #0x1
	bgt ReturnNoScript	@ no emergency exit if in middle of multi hit
	
CheckSwitchAttacks:
	ldr r0, =CURRENT_MOVE
	ldr r1, =MOVE_DRAGONTAIL
	cmp r1, r0
	beq ReturnNoScript
	ldr r1, =MOVE_CIRCLETHROW
	cmp r1, r0
	beq ReturnNoScript

CheckExitThreshold:
	ldr r0, =(BATTLE_DATA + 0x28)
	ldr r1, =USER_BANK
	ldrb r1, [r1, #0x1]
	mov r2, #0x58
	mul r1, r2
	add r0, r1
	ldrh r2, [r0, #0x4]	@r2 is max health
	ldrh r0, [r0]		@r0 is current health
	lsr r3, r2, #0x1	@r3 is half health
	cmp r0, r3
	bgt ReturnNoScript

CheckMultiHitForExit:
	ldr r1, =MULTI_HIT_BYTE
	ldrb r1, [r1]
	mov r2, #0x1
	cmp r1, r2
	beq EmergencyExitBS
	ldr r1, =DAMAGE_LOC
	ldrh r1, [r1]
	add r0, r1		@r0 is health before attack
	cmp r0, r3
	blt ReturnNoScript

EmergencyExitBS:
	bl ScriptPusher
	ldr r0, =SCRIPT_LOC
	ldr r1, =emergency_exit_bs+rom
	str r1, [r0]
	ldr r2, =(0x0801ABD9)
	bx r2
		
emergency_exit_bs:
jumpifcannotswitch 0x0 emergency_exit_end
setword BATTLE_STRING_LOADER emergency_exit_str
printstring 0x184
waitmessage 0x40
playanimation 0x0 0x1D emergency_exit_end
waitanimation
openpartyscreen 0x0 emergency_exit_fail
cmde2 0x0
waitstate
switchhandleorder 0x0 0x2
returntoball 0x0
switch1 0x0
switch2 0x0
hpthresholds 0x0
printstring 0x3
switch3 0x0 0x1
waitstate
switchineffects 0x0
callasm switchout_move_check+1

emergency_exit_end:
setbyte MULTI_HIT_BYTE 0x0
return

emergency_exit_fail:
return


.align 2
switchout_move_check:
	push {lr}
	ldr r0, =CURRENT_MOVE
	ldrh r0, [r0]
	ldr r1, =MOVE_UTURN
	cmp r1, r0
	beq PreventSwitchout
	ldr r1, =MOVE_VOLTSWITCH
	cmp r1, r0
	bne FinishSwitchoutCheck
	
PreventSwitchout:
	bl b_movescr_stack_pop_cursor
	ldr r0, =SCRIPT_LOC
	ldr r1, =(0x81D6954)
	str r1, [r0]

FinishSwitchoutCheck:
	pop {r0}
	bx r0
	
b_movescr_stack_pop_cursor:
	ldr r0, =(0x0801756c +1)
	bx r0

	
@ STRINGS @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

anger_point_str:
.byte 0

berserk_str:
.byte 0

cursed_body_str:
.byte 0

disguise_str1:
.byte 0

disguise_str2:
.byte 0

gooey_str:
.byte 0

innards_out_str:
.byte 0

justified_str:
.byte 0

mummy_str:
.byte 0

rattled_str:
.byte 0

stamina_str:
.byte 0

steadfast_str:
.byte 0

water_compaction_str:
.byte 0

lower_def_str:	@weak armor
.byte 0

raise_speed_str:	@ weak armor
.byte 0

emergency_exit_str:
.byte 0
*/
	
	
	