@Do 43 - High Crit Ratio
@Do 75 - Sky Attack
@Do 77 - Twineedle

.text
.thumb
.align 2

.include "..\\defines"

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_028_Roar
BS_028_Roar:
	jumpifmove MOVE_DRAGONTAIL DragonTailBS
	jumpifmove MOVE_CIRCLETHROW DragonTailBS
	goto 0x81D6CB1

DragonTailBS:
	attackcanceler
	accuracycheck 0x81D695E 0x0
	jumpifsecondarystatus BANK_TARGET STATUS2_SUBSTITUTE 0x81D692E
	call StandardDmaage
	jumpiffainted BANK_TARGET 0x81D6947
	jumpifbyte ANDS OUTCOME OUTCOME_NO_EFFECT 0x81D6947
	jumpifspecialstatusflag BANK_TARGET STATUS3_ROOTED 0x0 0x81D8F27
	jumpifability BANK_TARGET ABILITY_SUCTIONCUPS 0x81D9408
	setbyte 0x2023FD8 0x0
	cmd49 BANK_TARGET 0x0
	playanimation BANK_TARGET DRAGON_TAIL_BLOW_AWAY_ANIM 0x0
	forcerandomswitch 0x81D6957

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_030_Conversion
BS_030_Conversion:
	jumpifhalfword NOTEQUALS CURRENT_MOVE MOVE_REFLECTTYPE 0x1D6D9D
	attackcanceler
	accuracycheck 0x81D695E 0x0
	attackstring
	ppreduce
	jumpifsecondarystatus BANK_TARGET STATUS2_SUBSTITUTE FAILED
	changetypestoenemyattacktype FAILED
	attackanimation
	waitanimation
	setword BATTLE_STRING_LOADER ReflectTypeString
	printstring 0x184
	waitmessage DELAY_1SECOND
	goto 0x81D694E

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_045_HighJumpKick
BS_045_HighJumpKick:
	jumpifability BANK_ATTACKER ABILITY_MAGICGUARD 0x81D6926
	attackcanceler
	accuracycheck HighJumpKickMiss 0x0
	goto 0x81D692E

HighJumpKickMiss:
	attackstring
	ppreduce
	pause DELAY_HALFSECOND
	resultmessage
	waitmessage DELAY_1SECOND
	printstring 0x60
	waitmessage DELAY_1SECOND
	bicbyte OUTCOME OUTCOME_MISSED
	orword HIT_MARKER 0x100
	setdamagetobankhealthpercent BANK_ATTACKER 50
	graphicalhpupdate BANK_ATTACKER
	datahpupdate BANK_ATTACKER
	faintpokemon BANK_ATTACKER 0x0 0x8000000
	orbyte OUTCOME OUTCOME_MISSED
	goto 0x81D694E
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_057_Transform
BS_057_Transform:
	attackcanceler
	jumpifsecondarystatus BANK_TARGET STATUS2_SUBSTITUTE 0x81D7DF0
	jumpifsecondarystatus BANK_ATTACKER STATUS2_TRANSFORMED 0x81D7DF0
	jumpifspecialstatusflag BANK_TARGET STATUS3_ILLUSION 0x0 0x81D7DF0
	attackstring
	ppreduce
	transformdataexecution
	attackanimation
	waitanimation
	printfromtable 0x83FE5B4
	waitmessage DELAY_1SECOND
	goto 0x81D694E

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_065_Reflect
BS_065_Reflect:
	jumpifhalfword NOTEQUALS CURRENT_MOVE MOVE_AURORAVEIL 0x1D716E
	attackcanceler
	attackstring
	ppreduce
	callasm SetAuroraVeil + 1
	jumpifbyte EQUALS FORM_COUNTER 0x0 FAILED
	attackanimation
	waitanimation
	setword BATTLE_STRING_LOADER AuroraVeilSetString
	printstring 0x184
	waitmessage DELAY_1SECOND
	setbyte FORM_COUNTER 0x0
	goto 0x81D694E

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_081_Rage
BS_081_Rage:
	attackcanceler
	accuracycheck 0x81D7367 0x0
	setbyte 0x2023E85 0x1E
	call StandardDamage
	jumpifbyte ANDS OUTCOME OUTCOME_NO_EFFECT 0x81D6947
	seteffecttarget
	goto 0x81D6947
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_086_Disable
BS_086_Disable:
	attackcanceler
	attackstring
	ppreduce
	jumpifabilitypresenttargetfield ABILITY_AROMAVEIL ProtectedByAromaVeil
	goto 0x81D73EA

ProtectedByAromaVeil:
	pause DELAY_HALFSECOND
	setword BATTLE_STRING_LOADER 0x0890A9C0
	printstring 0x184
	waitmessage DELAY_1SECPMD
	end2

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.align 2
ReflectTypeString: .byte 0xFD, 0x0F, 0xB4, 0xE7, 0x00, 0xE8, 0xED, 0xE4, 0xD9, 0x00, 0xD6, 0xD9, 0xD7, 0xD5, 0xE1, 0xD9, 0xFE, 0xE8, 0xDC, 0xD9, 0x00, 0xE7, 0xD5, 0xE1, 0xD9, 0x00, 0xD5, 0xE7, 0x00, 0xFD, 0x10, 0xB4, 0xE7, 0xAB, 0xFF
AuroraVeilSetString: .byte 0xFD, 0x14, 0x00, 0xE4, 0xE6, 0xE3, 0xE8, 0xD9, 0xD7, 0xE8, 0xE7, 0x00, 0xD5, 0xDB, 0xD5, 0xDD, 0xE2, 0xE7, 0xE8, 0x00, 0xE4, 0xDC, 0xED, 0xE7, 0xDD, 0xD7, 0xD5, 0xE0, 0xFE, 0xD5, 0xE2, 0xD8, 0x00, 0xE7, 0xE4, 0xD9, 0xD7, 0xDD, 0xD5, 0xE0, 0x00, 0xE1, 0xE3, 0xEA, 0xD9, 0xE7, 0xAB, 0xFF
ProtectedByAromaVeilString: .byte 0xFD, 0x10, 0xB4, 0xE7, 0x00, 0xE4, 0xD5, 0xE6, 0xE8, 0xED, 0x00, 0xDD, 0xE7, 0xFE, 0xE4, 0xE6, 0xE3, 0xE8, 0xD9, 0xD7, 0xE8, 0xD9, 0xD8, 0x00, 0xD6, 0xED, 0x00, 0xBB, 0xE6, 0xE3, 0xE1, 0xD5, 0x00, 0xD0, 0xD9, 0xDD, 0xE0, 0xAB, 0xFF
