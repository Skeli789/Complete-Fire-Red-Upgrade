.text
.thumb
.align 2

.include "../defines"

.global BattleScript_TargetSleepHeal
.global BattleScript_TargetBurnHeal
.global BattleScript_StickyHoldActivatesRet
.global BattleScript_TargetFrozen
.global BattleScript_AbilityWasSuppressed

BattleScript_TargetSleepHeal:
	setword BATTLE_STRING_LOADER SlappedAwakeString
	printstring 0x184
	waitmessage DELAY_1SECOND
	refreshhpbar BANK_TARGET
	return

BattleScript_TargetBurnHeal:
	setword BATTLE_STRING_LOADER BurnCuredString
	printstring 0x184
	waitmessage DELAY_1SECOND
	refreshhpbar BANK_TARGET
	return
	
BattleScript_StickyHoldActivatesRet:
	pause 0x10
	call BattleScript_AbilityPopUp
	setword BATTLE_STRING_LOADER ItemCantBeRemovedString
	printstring 0x184
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	return

BattleScript_WasFrozen:
	statusanimation 0x2
	printfromtable 0x83FE5CC
	waitmessage DELAY_1SECOND
	refreshhpbar 0x2
	waitstateatk
	formchange 0x2 SPECIES_SHAYMIN SPECIES_SHAYMINSKY TRUE TRUE TargetFrozenReturn
	playanimation 0x2 ANIM_TRANSFORM 0x0
	copyarray BATTLE_SCRIPTING_BANK EFFECT_BANK 0x1
	setword BATTLE_STRING_LOADER TransformedString
	printstring 0x184
	waitmessage DELAY_1SECOND	

TargetFrozenReturn:
	return

BattleScript_MoveEffectWrap:
	printfromtable gWrappedStringIds
	waitmessage DELAY_1SECOND
	return
	
BattleScript_AbilityWasSuppressed:
	setword BATTLE_STRING_LOADER AbilitySuppressedString
	printstring 0x184
	waitmessage DELAY_1SECOND
	call BSTryRemoveIllusion
	call 0x81D92DC @;Try to revert Cherrim and Castform
	return

.align 2
SlappedAwakeString: .byte 0xFD, 0x10, 0x00, 0xEB, 0xD5, 0xE7, 0x00, 0xE7, 0xE0, 0xD5, 0xE4, 0xE4, 0xD9, 0xD8, 0xFE, 0xD5, 0xEB, 0xD5, 0xDF, 0xD9, 0xAB
BurnCuredString: .byte 0xFD, 0x10, 0xB4, 0xE7, 0x00, 0xD6, 0xE9, 0xE6, 0xE2, 0xFE, 0xEB, 0xD5, 0xE7, 0x00, 0xDC, 0xD9, 0xD5, 0xE0, 0xD9, 0xD8, 0xAB, 0xFF
ItemCantBeRemovedString: .byte 0xFD, 0x10, 0xB4, 0xE7, 0x00, 0xDD, 0xE8, 0xD9, 0xE1, 0xFE, 0xD7, 0xD5, 0xE2, 0xB4, 0xE8, 0x00, 0xD6, 0xD9, 0x00, 0xE6, 0xD9, 0xE1, 0xE3, 0xEA, 0xD9, 0xD8, 0xAB, 0xFF
