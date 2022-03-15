.thumb
.text
.align 2
/*
fainting_battle_scripts.s
	scripts for pokemon fainting
*/

.include "../asm_defines.s"
.include "../battle_script_macros.s"

.global BattleScript_FaintAttacker
.global BattleScript_FaintRaidAttacker
.global BattleScript_FaintTarget
.global BattleScript_FaintRaidTarget
.global BattleScript_FaintScriptingBank
.global BattleScript_FaintRaidScriptingBank
.global BattleScript_SuccessBallThrow
.global BattleScript_BenjaminButterfreeDevolution

@Raid Explosion doesn't work on Trainer Dynamax mon because it's KOd

BattleScript_FaintAttacker:
	pokemonfaintcry BANK_ATTACKER
	pause 0x30
	dofaintanimation BANK_ATTACKER
	printstring 0x1C @;STRINGID_ATTACKERFAINTED
	cleareffectsonfaint BANK_ATTACKER
	printstring 0x130
	trytrainerslidefirstdownmsg BANK_ATTACKER
	return

BattleScript_FaintRaidAttacker:
	pokemonfaintcry BANK_ATTACKER
	playanimation BANK_ATTACKER ANIM_POWDER_EXPLOSION 0x0
	cleareffectsonfaint BANK_ATTACKER

BattleScript_FinishFaintRaidBoss:
	callasm ClearPlayerRechargeMultipleTurns @;So the game doesn't lock
	callasm IncrementBattleTurnCounter @;Stops Quick Ball from working (from SwSh)
	callasm FinishTurn
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

BattleScript_FaintRaidTarget:
	pokemonfaintcry BANK_TARGET
	playanimation BANK_TARGET ANIM_POWDER_EXPLOSION 0x0
	cleareffectsonfaint BANK_TARGET
	goto BattleScript_FinishFaintRaidBoss

BattleScript_FaintScriptingBank:
	copybyte FAINTED_BANK, BATTLE_SCRIPTING_BANK @;Using BANK_SCRIPTING can cause problems in cleareffectsonfaint
	pokemonfaintcry BANK_FAINTED
	pause 0x30
	dofaintanimation BANK_FAINTED
	setword BATTLE_STRING_LOADER ScriptingBankFainted
	printstring 0x184
	cleareffectsonfaint BANK_FAINTED
	printstring 0x130
	trytrainerslidefirstdownmsg BANK_FAINTED
	return

BattleScript_FaintRaidScriptingBank:
	copybyte FAINTED_BANK, BATTLE_SCRIPTING_BANK @;Using BANK_SCRIPTING can cause problems in cleareffectsonfaint
	pokemonfaintcry BANK_FAINTED
	playanimation BANK_FAINTED ANIM_POWDER_EXPLOSION 0x0
	cleareffectsonfaint BANK_FAINTED
	goto BattleScript_FinishFaintRaidBoss

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
	callasm ApplyBallSpecialEffect
	pickupitemcalculation
	trysetcaughtmondexflags BattleScript_CaughtPokemonSkipNewDex
	printstring 0x10F @;STRINGID_PKMNDATAADDEDTODEX
	waitstateatk
	setbyte BATTLE_COMMUNICATION, 0
	displaydexinfo
BattleScript_CaughtPokemonSkipNewDex:
	callasm TrySkipBattleNicknameOffer
	goto 0x81D9A63
	
BattleScript_BenjaminButterfreeDevolution:
	callasm RestoreEffectBankHPStatsAndRemoveBackupSpecies
	reloadhealthbar BANK_EFFECT
	playanimation BANK_EFFECT ANIM_TRANSFORM 0x0
	pokemonfaintcry BANK_EFFECT
	setword BATTLE_STRING_LOADER gText_Devolved
	printstring 0x184
	waitmessage DELAY_1SECOND
	cureprimarystatus BANK_EFFECT 0x0
	reloadhealthbar BANK_EFFECT
	setword BATTLE_STRING_LOADER gText_DevolutionsStatsReset
	printstring 0x184
	waitmessage DELAY_1SECOND
	callasm SetSkipCertainSwitchInAbilities  @;Skip certain abilities if you fainted from recoil and KO'd the Foe
	tryactivateswitchinability BANK_EFFECT
	callasm RestoreOriginalAttackerAndTarget
	return

.align 2
ScriptingBankFainted: .byte 0xFD, 0x13, 0xFE, 0xDA, 0xD5, 0xDD, 0xE2, 0xE8, 0xD9, 0xD8, 0xAB, 0xFA, 0xFF
