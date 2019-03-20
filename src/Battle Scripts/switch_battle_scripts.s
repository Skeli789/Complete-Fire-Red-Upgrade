.text
.thumb
.align 2

.include "..\\defines"

.global BattleScript_HealingWishHeal
.global BattleScript_LunarDanceHeal

.global BattleScript_PrimalWeatherEnd
.global PrimalRainEndString
.global PrimalSunEndString
.global PrimalAirCurrentEndString

.global BattleScript_SpikesHurt
.global BattleScript_SRHurt
.global BattleScript_TSPoison
.global BattleScript_TSHarshPoison
.global BattleScript_TSAbsorb
.global BattleScript_StickyWebSpeedDrop
.global BattleScript_SuccessForceOut

.global BattleScript_WildDoubleSwitchFix

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_HealingWishHeal:
	playanimation BANK_ATTACKER ANIM_HEALING_WISH_HEAL 0x0
	setword BATTLE_STRING_LOADER HealingWishHealString
	goto Sparkles

BattleScript_LunarDanceHeal:
	playanimation BANK_ATTACKER ANIM_LUNAR_DANCE_HEAL 0x0
	setword BATTLE_STRING_LOADER LunarDanceHealString

Sparkles:
	printstring 0x184
	waitmessage DELAY_1SECOND
	playanimation BANK_ATTACKER ANIM_HEALING_SPARKLES 0x0
	graphicalhpupdate BANK_ATTACKER
	datahpupdate BANK_ATTACKER
	refreshhpbar BANK_ATTACKER
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_SpikesHurt:
	orword HIT_MARKER 0x100100
	graphicalhpupdate BANK_TARGET
	datahpupdate BANK_TARGET
	setword BATTLE_STRING_LOADER SPIKES_HURT_STR
	printstring 0x184
	waitmessage DELAY_1SECOND
	faintpokemon BANK_TARGET 0x0 0x0
	faintpokemon BANK_TARGET 0x1 0x81D8CDF
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	
BattleScript_SRHurt:
	orword HIT_MARKER 0x100100
	graphicalhpupdate BANK_TARGET
	datahpupdate BANK_TARGET
	setword BATTLE_STRING_LOADER STEALTH_ROCK_HURT_STR
	printstring 0x184
	waitmessage DELAY_1SECOND
	faintpokemon BANK_TARGET 0x0 0x0
	faintpokemon BANK_TARGET 0x1 0x81D8CDF
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	
BattleScript_TSPoison:
	setbyte POISONED_BY 0x2 @To-Do, modify PSN set script
	setbyte EFFECT_BYTE 0x2
	seteffecttarget
	setbyte POISONED_BY 0x0
	return
	
BattleScript_TSHarshPoison:
	setbyte POISONED_BY 0x2
	setbyte EFFECT_BYTE 0x6
	seteffecttarget
	setbyte POISONED_BY 0x0
	return

BattleScript_TSAbsorb:
	setword BATTLE_STRING_LOADER TS_ABSORB_STR
	printstring 0x184
	waitmessage DELAY_1SECOND
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_StickyWebSpeedDrop:
	orword HIT_MARKER 0x100100
	setword BATTLE_STRING_LOADER SW_CAUGHT_STR
	printstring 0x184
	waitmessage DELAY_HALFSECOND
	setbyte EFFECT_BYTE 0x18
	seteffecttarget
	return
	
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_PrimalWeatherEnd:
	pause DELAY_HALFSECOND
	printstring 0x184
	waitmessage DELAY_1SECOND
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_SuccessForceOut:
	jumpifbyte EQUALS FORCE_SWITCH_HELPER 0x2 RedCardForceSwitch
	jumpifbyte NOTEQUALS FORCE_SWITCH_HELPER 0x0 SkipRoarAnim
	attackanimation
	waitanimation

SkipRoarAnim:
	switchoutabilities BANK_TARGET
	returntoball BANK_TARGET
	waitstateatk
	jumpifword ANDS BATTLE_TYPE BATTLE_TRAINER ForceSwitch
	jumpifword ANDS BATTLE_TYPE BATTLE_DOUBLE ForceSwitch @Wild Double Battles force switches
	setbyte BATTLE_OUTCOME 0x5
	finishaction

ForceSwitch:
	switch1 BANK_TARGET
	switch2 BANK_TARGET
	switch3 BANK_TARGET 0x0
	waitstateatk
	printstring 0x154
	callasm MoldBreakerRemoveAbilitiesOnForceSwitchIn
	switchineffects BANK_TARGET
	setbyte FORCE_SWITCH_HELPER 0x0
	goto 0x81D694E

RedCardForceSwitch:
	switchoutabilities BANK_TARGET
	returntoball BANK_TARGET
	waitstateatk
	jumpifword ANDS BATTLE_TYPE BATTLE_TRAINER ForceSwitchRedCard
	jumpifword ANDS BATTLE_TYPE BATTLE_DOUBLE ForceSwitchRedCard @Wild Double Battles force switches
	setbyte BATTLE_OUTCOME 0x5 @;Teleported
	end

ForceSwitchRedCard:
	switch1 BANK_TARGET
	switch2 BANK_TARGET
	switch3 BANK_TARGET 0x0
	waitstateatk
	printstring 0x154
	callasm MoldBreakerRemoveAbilitiesOnForceSwitchIn
	switchineffects BANK_TARGET
	callasm MoldBreakerRestoreAbilitiesOnForceSwitchIn
	setbyte FORCE_SWITCH_HELPER 0x0
	copyarray USER_BANK TARGET_BANK 0x1 @copies target into user
	copyarray TARGET_BANK SEED_HELPER 0x1 @restores target	
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_ActionSwitch:
	hpthresholds2 BANK_ATTACKER
	printstring 0x2 @;STRINGID_RETURNMON
	setbyte DMG_MULTIPLIER 0x2
	jumpifbattletype BATTLE_DOUBLE BattleScript_PursuitSwitchDmgSetMultihit
	storeloopingcounter 0x1
	goto BattleScript_PursuitSwitchDmgLoop

BattleScript_PursuitSwitchDmgSetMultihit:
	storeloopingcounter 0x2

BattleScript_PursuitSwitchDmgLoop:
	jumpifnopursuitswitchdmg BattleScript_DoSwitchOut
	swapattackerwithtarget
	trysetdestinybondtohappen
	call BattleScript_PursuitDmgOnSwitchOut
	swapattackerwithtarget

BattleScript_DoSwitchOut:
	decrementmultihit BattleScript_PursuitSwitchDmgLoop
	switchoutabilities BANK_ATTACKER
	waitstateatk
	returnatktoball
	waitstateatk
	drawpartystatussummary BANK_ATTACKER
	switchhandleorder BANK_ATTACKER 0x1
	switch1 BANK_ATTACKER
	switch2 BANK_ATTACKER
	hpthresholds BANK_ATTACKER
	printstring 0x3 @;STRINGID_SWITCHINMON
	hidepartystatussummary BANK_ATTACKER
	switch3 BANK_ATTACKER 0x0
	waitstateatk
	switchineffects BANK_ATTACKER 
	setbyte CMD49_STATE 0x4
	cmd49 0x1 0x0
	setbyte CMD49_STATE 0xF
	cmd49 0x1 0x0
	end2
	
BattleScript_PursuitDmgOnSwitchOut:
	pause DELAY_HALFSECOND
	call STANDARD_DAMAGE
	faintpokemon BANK_TARGET 0x0 0x0
	setbyte CMD49_STATE, 0x0
	cmd49 0x4 0x0
	various BANK_TARGET 4
	jumpifbyte EQUALS BATTLE_COMMUNICATION 0x0 BattleScript_PursuitDmgOnSwitchOutRet
	setbyte EXP_STATE 0x0
	getexp BANK_TARGET
	
BattleScript_PursuitDmgOnSwitchOutRet:
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_WildDoubleSwitchFix:
	jumpifbattletype BATTLE_TRAINER 0x81D86E6
	jumpifbattletype BATTLE_DOUBLE WildDoubleCheckSwitchPossible
	goto 0x81D86BB

WildDoubleCheckSwitchPossible:
	jumpifcannotswitch 0xB WildDoubleCantSwitch
	goto 0x81D86BB

WildDoubleCantSwitch:
	openpartyscreen 0x3 0x81D87B7
	switchhandleorder 0x3 0x2
	goto 0x81D8792

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.align 2
HealingWishHealString: .byte 0xCE, 0xDC, 0xD9, 0x00, 0xC2, 0xD9, 0xD5, 0xE0, 0xDD, 0xE2, 0xDB, 0x00, 0xD1, 0xDD, 0xE7, 0xDC, 0x00, 0xD7, 0xD5, 0xE1, 0xD9, 0x00, 0xE8, 0xE6, 0xE9, 0xD9, 0xFE, 0xDA, 0xE3, 0xE6, 0x00, 0xFD, 0x0F, 0xAB, 0xFF
LunarDanceHealString: .byte 0xFD, 0x0F, 0x00, 0xD6, 0xD9, 0xD7, 0xD5, 0xE1, 0xD9, 0xFE, 0xD7, 0xE0, 0xE3, 0xD5, 0xDF, 0xD9, 0xD8, 0x00, 0xDD, 0xE2, 0x00, 0xE1, 0xED, 0xE7, 0xE8, 0xDD, 0xD7, 0xD5, 0xE0, 0x00, 0xE1, 0xE3, 0xE3, 0xE2, 0xE0, 0xDD, 0xDB, 0xDC, 0xE8, 0xAB, 0xFF

SPIKES_HURT_STR: .byte 0xFD, 0x10, 0x00, 0xDD, 0xE7, 0x00, 0xDC, 0xE9, 0xE6, 0xE8, 0xFE, 0xD6, 0xED, 0x00, 0xE8, 0xDC, 0xD9, 0x00, 0xCD, 0xE4, 0xDD, 0xDF, 0xD9, 0xE7, 0xAB, 0xFF
STEALTH_ROCK_HURT_STR: .byte 0xCA, 0xE3, 0xDD, 0xE2, 0xE8, 0xD9, 0xD8, 0x00, 0xE7, 0xE8, 0xE3, 0xE2, 0xD9, 0xE7, 0x00, 0xD8, 0xE9, 0xDB, 0x00, 0xDD, 0xE2, 0xE8, 0xE3, 0xFE, 0xFD, 0x10, 0xAB, 0xFF
TS_ABSORB_STR: .byte 0xCE, 0xDC, 0xD9, 0x00, 0xE7, 0xE4, 0xDD, 0xDF, 0xD9, 0xE7, 0x00, 0xD8, 0xDD, 0xE7, 0xD5, 0xE4, 0xE4, 0xD9, 0xD5, 0xE6, 0xD9, 0xD8, 0x00, 0xDA, 0xE6, 0xE3, 0xE1, 0x00, 0xD5, 0xE6, 0xE3, 0xE9, 0xE2, 0xD8, 0xFE, 0xFD, 0x10, 0xB4, 0xE7, 0x00, 0xDA, 0xD9, 0xD9, 0xE8, 0xAB, 0xFF
SW_CAUGHT_STR: .byte 0xFD, 0x10, 0x00, 0xEB, 0xD5, 0xE7, 0x00, 0xD7, 0xD5, 0xE9, 0xDB, 0xDC, 0xE8, 0x00, 0xDD, 0xE2, 0xFE, 0xD5, 0x00, 0xE7, 0xE8, 0xDD, 0xD7, 0xDF, 0xED, 0x00, 0xEB, 0xD9, 0xD6, 0xAB, 0xFF

PrimalRainEndString: .byte 0xCE, 0xDC, 0xD9, 0x00, 0xDC, 0xD9, 0xD5, 0xEA, 0xED, 0x00, 0xE6, 0xD5, 0xDD, 0xE2, 0x00, 0xDC, 0xD5, 0xE7, 0x00, 0xE0, 0xDD, 0xDA, 0xE8, 0xD9, 0xD8, 0xAB, 0xFF
PrimalSunEndString: .byte 0xCE, 0xDC, 0xD9, 0x00, 0xD9, 0xEC, 0xE8, 0xE6, 0xD9, 0xE1, 0xD9, 0xE0, 0xED, 0x00, 0xDC, 0xD5, 0xE6, 0xE7, 0xDC, 0x00, 0xE7, 0xE9, 0xE2, 0xE0, 0xDD, 0xDB, 0xDC, 0xE8, 0x00, 0xDA, 0xD5, 0xD8, 0xD9, 0xD8, 0xAB, 0xFF
PrimalAirCurrentEndString: .byte 0xCE, 0xDC, 0xD9, 0x00, 0xE1, 0xED, 0xE7, 0xE8, 0xD9, 0xE6, 0xDD, 0xE3, 0xE9, 0xE7, 0x00, 0xD5, 0xDD, 0xE6, 0x00, 0xD7, 0xE9, 0xE6, 0xE6, 0xD9, 0xE2, 0xE8, 0x00, 0xDC, 0xD5, 0xE7, 0xFE, 0xD8, 0xDD, 0xE7, 0xE7, 0xDD, 0xE4, 0xD5, 0xE8, 0xD9, 0xD8, 0xAB, 0xFF
