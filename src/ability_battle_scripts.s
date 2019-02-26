.text
.thumb
.align 2

.include "..\\defines"

.global BattleScript_OverworldWeatherStarts
.global BattleScript_NewWeatherAbilityActivates
.global BattleScript_IntimidateActivatesEnd3
.global BattleScript_TraceActivates
.global BattleScript_SwitchInAbilityMsg
.global BattleScript_AbilityCuredStatusEnd3
.global BattleScript_StartedSchoolingEnd3
.global BattleScript_StoppedSchoolingEnd3
.global BattleScript_ShieldsDownToCoreEnd3
.global BattleScript_ShieldsDownToMeteorEnd3
.global BattleScript_EmergencyExit
.global BattleScript_TransformedEnd3

.global BattleScript_Protean
.global BattleScript_MimikyuTransform
.global MimikyuDisguisedTookDamageString
.global BattleScript_EnduredSturdy
.global StringEnduredHitWithSturdy
.global BattleScript_Receiver
.global BattleScript_Symbiosis
.global BattleScript_DefiantCompetitive
/*
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_OverworldWeatherStarts:
	printfromtable gWeatherContinuesStringIds
	waitmessage DELAY_1SECOND
	playanimation2 BANK_ATTACKER ANIM_ARG_1 0x0
	end3
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_NewWeatherAbilityActivates:
	pause 0x20
	printstring 0x184
	waitstate
	playanimation2 BANK_ATTACKER ANIM_ARG_1 0x0
	call BattleScript_WeatherFormChanges
	end3

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_IntimidateActivatesEnd3:
	call BattleScript_PauseIntimidateActivates
	end3

BattleScript_PauseIntimidateActivates:
	pause DELAY_HALFSECOND
	setword BATTLE_STRING_LOADER AbilityActivatedString
	printstring 0x184
	waitmessage DELAY_HALFSECOND
	setbyte TARGET_BANK 0x0
	setstatchanger STAT_ATK | DECREASE_1

BS_IntimidateActivatesLoop:
	trygetintimidatetarget BattleScript_IntimidateActivatesReturn
	jumpifbehindsubstitute BANK_TARGET IntimidateActivatesLoopIncrement
	statbuffchange STAT_TARGET | STAT_NOT_PROTECT_AFFECTED | STAT_BS_POINTER IntimidateActivatesLoopIncrement
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 BattleScript_IntimidatePrevented
	setgraphicalstatchangevalues
	playanimation BANK_TARGET ANIM_STAT_BUFF ANIM_ARG_1
	printfromtable 0x83FE588
	waitmessage DELAY_1SECOND
	jumpifhelditemeffect BANK_TARGET ITEM_EFFECT_ADRENALINE_ORB BattleScript_AdrenalineOrb
	goto IntimidateActivatesLoopIncrement
	
BattleScript_AdrenalineOrb:
	setstatchanger STAT_SPD | INCREASE_1
	statbuffchange STAT_TARGET | STAT_NOT_PROTECT_AFFECTED | STAT_BS_POINTER IntimidateActivatesLoopIncrement
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 IntimidateActivatesLoopIncrement
	playanimation BANK_TARGET ANIM_ITEM_USE 0x0
	setgraphicalstatchangevalues
	playanimation BANK_TARGET ANIM_STAT_BUFF ANIM_ARG_1
	setword BATTLE_STRING_LOADER AdrenalineOrbString
	printstring 0x184
	waitmessage DELAY_1SECOND
	removeitem BANK_TARGET
	goto IntimidateActivatesLoopIncrement
	
BattleScript_IntimidatePrevented:
	pause DELAY_HALFSECOND
	printfromtable 0x83FE588
	waitmessage DELAY_SECOND

IntimidateActivatesLoopIncrement:
	addbyte TARGET_BANK 0x1
	goto BattleScript_IntimidateActivatesLoop

BattleScript_IntimidateActivatesReturn:
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_TraceActivates:
	pause DELAY_HALFSECOND
	printstring STRINGID_PKMNTRACED
	waitmessage DELAY_1SECOND
	callasm TryActivateNewSwitchInAbility
	end3

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	
BattleScript_SwitchInAbilityMsg:
	pause DELAY_HALFSECOND
	printstring 0x184
	waitmessage DELAY_1SECOND
	end3	

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_AbilityCuredStatusEnd3:
	printstring STRINGID_PKMNSXCUREDITSYPROBLEM
	waitmessage DELAY_1SECOND
	refreshhpbar BANK_SCRIPTING
	end3

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_StartedSchoolingEnd3:
	call BattleScript_StartedSchoolingRet
	end3

BattleScript_StoppedSchoolingEnd3:
	call BattleScript_StartedSchoolingRet
	end3

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_ShieldsDownToCoreEnd3:
	call BattleScript_ShieldsDownToCoreRet
	end3
	
BattleScript_ShieldsDownToMeteorEnd3:
	call BattleScript_ShieldsDownToMeteorRet
	end3

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_TransformedEnd3:
	playanimation BANK_ATTACKER ANIM_TRANSFORM 0x0
	setword BATTLE_STRING_LOADER TransformedString
	printstring 0x184
	waitmessage DELAY_1SECOND
	end3
*/

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_EmergencyExit:
	jumpifbattletype BATTLE_TRAINER EmergencyExitSwitchBS

EmergencyExitFleeBS:
	getifcantrunfrombattle BANK_SCRIPTING
	jumpifbyte EQUALS BATTLE_COMMUNICATION 0x1 EmergencyExitReturn
	setword BATTLE_STRING_LOADER AbilityActivatedString
	printstring 0x184
	waitmessage DELAY_1SECOND
	copyarray USER_BANK BATTLE_SCRIPTING_BANK 0x1
	printstring 0xA0 @;STRINGID_PKMNFLEDFROMBATTLE
	waitmessage DELAY_1SECOND
	setbyte BATTLE_OUTCOME 0x5 @;Teleported
	end

EmergencyExitSwitchBS:
	jumpifcannotswitch BANK_SCRIPTING EmergencyExitReturn
	setword BATTLE_STRING_LOADER AbilityActivatedString
	printstring 0x184
	waitmessage DELAY_1SECOND
	playanimation BANK_SCRIPTING ANIM_CALL_BACK_POKEMON 0x0
	callasm MakeScriptingBankInvisible
	openpartyscreen BANK_SCRIPTING EmergencyExitReturn
	switchoutabilities BANK_SCRIPTING
	waitstateatk
	switchhandleorder BANK_SCRIPTING 0x2
	returntoball BANK_SCRIPTING
	switch1 BANK_SCRIPTING
	switch2 BANK_SCRIPTING
	hpthresholds BANK_SCRIPTING
	printstring 0x3
	switch3 BANK_SCRIPTING 0x1
	waitstateatk
	switchineffects BANK_SCRIPTING
EmergencyExitReturn:
	return
	

	
	


@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_Protean:
	pause DELAY_HALFSECOND
	setword BATTLE_STRING_LOADER ProteanString
	printstring 0x184
	waitmessage DELAY_HALFSECOND
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MimikyuTransform:
	playanimation BANK_TARGET ANIM_TRANSFORM 0x0
	setword BATTLE_STRING_LOADER DisguiseBustedString
	printstring 0x184
	waitmessage DELAY_1SECOND
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_EnduredSturdy:
	setword BATTLE_STRING_LOADER StringEnduredHitWithSturdy
	printstring 0x184
	waitmessage DELAY_1SECOND
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_Receiver:
	setword BATTLE_STRING_LOADER ReceiverString
	printstring 0x184
	waitmessage DELAY_1SECOND
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_Symbiosis:
	setword BATTLE_STRING_LOADER SymbiosisString
	printstring 0x184
	waitmessage DELAY_1SECOND
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_DefiantCompetitive:
	statbuffchange 0xA DefiantReturn
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 DefiantReturn
	setgraphicalstatchangevalues
	playanimation 0xA 0x1 0x2023FD4
	setword BATTLE_STRING_LOADER DefiantString
	printstring 0x184
	waitmessage DELAY_1SECOND

DefiantReturn:
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

ProteanString: .byte 0xFD, 0x0F, 0xB4, 0xE7, 0x0, 0xFD, 0x18, 0x0, 0xD7, 0xDC, 0xD5, 0xE2, 0xDB, 0xD9, 0xD8, 0xFE, 0xDD, 0xE8, 0x0, 0xDD, 0xE2, 0xE8, 0xE3, 0x0, 0xE8, 0xDC, 0xD9, 0x0, 0xFD, 0x0, 0x0, 0xE8, 0xED, 0xE4, 0xD9, 0xAB, 0xFF
MimikyuDisguisedTookDamageString: .byte 0xFD, 0x10, 0xB4, 0xE7, 0x00, 0xD8, 0xDD, 0xE7, 0xDB, 0xE9, 0xDD, 0xE7, 0xD9, 0xFE, 0xE7, 0xD9, 0xE6, 0xEA, 0xD9, 0xD8, 0x00, 0xD5, 0xE7, 0x00, 0xDD, 0xE8, 0xE7, 0x00, 0xD8, 0xD9, 0xD7, 0xE3, 0xED, 0xAB, /*0xFB,*/ 0xFF
DisguiseBustedString: .byte 0xFD, 0x10, 0xB4, 0xE7, 0x00, 0xD8, 0xDD, 0xE7, 0xDB, 0xE9, 0xDD, 0xE7, 0xD9, 0xFE, 0xEB, 0xD5, 0xE7, 0x00, 0xD6, 0xE9, 0xE7, 0xE8, 0xD9, 0xD8, 0xAB, /*0xFB,*/ 0xFF
StringEnduredHitWithSturdy: .byte 0xFD, 0x10, 0x00, 0xD9, 0xE2, 0xD8, 0xE9, 0xE6, 0xD9, 0xD8, 0xFE, 0xE8, 0xDC, 0xD9, 0x00, 0xDC, 0xDD, 0xE8, 0x00, 0xEB, 0xDD, 0xE8, 0xDC, 0x00, 0xFD, 0x19, 0xAB, 0xFF
ReceiverString: .byte 0xFD, 0x13, 0x00, 0xE8, 0xE3, 0xE3, 0xDF, 0x00, 0xE3, 0xEA, 0xD9, 0xE6, 0x00, 0xFE, 0xFD, 0x11, 0xB4, 0xE7, 0x00, 0xFD, 0x1A, 0xAB, 0xFF
SymbiosisString: .byte 0xFD, 0x13, 0x00, 0xE7, 0xDC, 0xD5, 0xE6, 0xD9, 0xD8, 0x00, 0xDD, 0xE8, 0xE7, 0xFE, 0xFD, 0x16, 0x00, 0xEB, 0xDD, 0xE8, 0xDC, 0x00, 0xFD, 0x11, 0xFA, 0xE9, 0xE7, 0xDD, 0xE2, 0xDB, 0x00, 0xFD, 0x1A, 0xAB, 0xFF
DefiantString: .byte 0xFD, 0x13, 0xB4, 0xE7, 0x00, 0xFD, 0x1A, 0xFE, 0xE7, 0xDC, 0xD5, 0xE6, 0xE4, 0xE0, 0xED, 0x00, 0xE6, 0xD5, 0xDD, 0xE7, 0xD9, 0xD8, 0x00, 0xDD, 0xE8, 0xE7, 0x00, 0xFD, 0x00, 0xAB, 0xFF
