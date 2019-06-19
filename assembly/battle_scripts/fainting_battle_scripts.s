.thumb
.text
.align 2

.include "..\\defines"

.global BattleScript_FaintAttacker
.global BattleScript_FaintTarget
.global BattleScript_FaintScriptingBank
.global BattleScript_SuccessBallThrow

BattleScript_FaintAttacker:
	pokemonfaintcry BANK_ATTACKER
	pause 0x30
	dofaintanimation BANK_ATTACKER
	printstring 0x1C @;STRINGID_ATTACKERFAINTED
	cleareffectsonfaint BANK_ATTACKER
	printstring 0x130
	trytrainerslidefirstdownmsg BANK_ATTACKER
	return

BattleScript_FaintTarget:
	pokemonfaintcry BANK_TARGET
	pause 0x30
	dofaintanimation BANK_TARGET
	printstring 0x1D @;STRINGID_TARGETFAINTED
	cleareffectsonfaint BANK_TARGET
	printstring 0x130
	trytrainerslidefirstdownmsg BANK_TARGET
	return
	
BattleScript_FaintScriptingBank:
	pokemonfaintcry BANK_SCRIPTING
	pause 0x30
	dofaintanimation BANK_SCRIPTING
	setword BATTLE_STRING_LOADER ScriptingBankFainted
	printstring 0x184
	cleareffectsonfaint BANK_SCRIPTING
	printstring 0x130
	trytrainerslidefirstdownmsg BANK_SCRIPTING
	return
	
BattleScript_SuccessBallThrow:
	jumpifhalfword EQUALS ITEM_BUFFER ITEM_SAFARI_BALL BattleScript_PrintCaughtMonInfo
	incrementgamestat 0xB @;GAME_STAT_POKEMON_CAPTURES

BattleScript_PrintCaughtMonInfo:
	printstring 0x10B @;STRINGID_GOTCHAPKMNCAUGHT
	copyarray BATTLE_SCRIPTING_BANK TARGET_BANK 0x1
	callasm MakeScriptingBankInvisible
	setbyte EXP_STATE, 0x0
	getexp BANK_TARGET
	@;sethword gBattle_BG2_X, 0x0
	goto 0x81D9A53

.align 2
ScriptingBankFainted: .byte 0xFD, 0x13, 0xFE, 0xDA, 0xD5, 0xDD, 0xE2, 0xE8, 0xD9, 0xD8, 0xAB, 0xFA, 0xFF
