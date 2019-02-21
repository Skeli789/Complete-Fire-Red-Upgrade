.thumb
.text
.align 2

.include "..\\defines"

.global BattleScript_FaintAttacker
.global BattleScript_FaintTarget
.global BattleScript_FaintScriptingBank

BattleScript_FaintAttacker:
	pokemonfaintcry BANK_ATTACKER
	pause 0x30
	dofaintanimation BANK_ATTACKER
	cleareffectsonfaint BANK_ATTACKER
	printstring 0x1C @;STRINGID_ATTACKERFAINTED
	printstring 0x130
	return

BattleScript_FaintTarget:
	pokemonfaintcry BANK_TARGET
	pause 0x30
	dofaintanimation BANK_TARGET
	cleareffectsonfaint BANK_TARGET
	printstring 0x1D @;STRINGID_TARGETFAINTED
	printstring 0x130
	return
	
BattleScript_FaintScriptingBank:
	pokemonfaintcry BANK_SCRIPTING
	pause 0x30
	dofaintanimation BANK_SCRIPTING
	cleareffectsonfaint BANK_SCRIPTING
	setword BATTLE_STRING_LOADER ScriptingBankFainted
	printstring 0x184
	printstring 0x130
	return

.align 2
ScriptingBankFainted: .byte 0xFD, 0x13, 0xFE, 0xDA, 0xD5, 0xDD, 0xE2, 0xE8, 0xD9, 0xD8, 0xAB, 0xFA
