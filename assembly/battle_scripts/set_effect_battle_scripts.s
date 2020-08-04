.text
.thumb
.align 2
/*
set_effect_battle_scripts.s
	scripts for effects being set in battle
*/

.include "../asm_defines.s"
.include "../battle_script_macros.s"

.global BattleScript_TargetSleepHeal
.global BattleScript_TargetBurnHeal
.global BattleScript_StickyHoldActivatesRet
.global BattleScript_TargetFrozen
.global BattleScript_KnockedOff
.global BattleScript_AbilityWasSuppressed
.global BattleScript_StatUpPartner
.global BattleScript_MaxMoveRaiseStatTeam
.global BattleScript_MaxMoveLowerStatFoes
.global BattleScript_EatEffectBankBerry
.global BattleScript_MaxMoveSetWeather
.global BattleScript_MaxMoveSetTerrain
.global BattleScript_MaxMoveEffectSporeFoes
.global BattleScript_MaxMoveParalyzeFoes
.global BattleScript_MaxMovePoisonFoes
.global BattleScript_MaxMovePayDayConfuseFoes
.global BattleScript_MaxMoveCritUp
.global BattleScript_MaxMoveTrap
.global BattleScript_MaxMoveInfatuation
.global BattleScript_MaxMoveRecycle
.global BattleScript_MaxMoveHealTeam
.global BattleScript_MaxMoveSetStealthRock
.global BattleScript_MaxMoveDefog
.global BattleScript_MaxMovePoisonParalyzeFoes
.global BattleScript_MaxMoveSpite
.global BattleScript_MaxMoveSetGravity
.global BattleScript_MaxMoveSetYawn
.global BattleScript_MaxMoveTrapAndDamageFoes
.global BattleScript_MaxMoveLowerEvasivenessFoes
.global BattleScript_MaxMoveAromatherapy
.global BattleScript_MaxMoveConfuseFoes
.global BattleScript_MaxMoveTormentFoes
.global BattleScript_MaxMoveLowerSpeed2Foes

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_TargetSleepHeal:
	setword BATTLE_STRING_LOADER SlappedAwakeString
	printstring 0x184
	waitmessage DELAY_1SECOND
	refreshhpbar BANK_TARGET
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_TargetBurnHeal:
	setword BATTLE_STRING_LOADER BurnCuredString
	printstring 0x184
	waitmessage DELAY_1SECOND
	refreshhpbar BANK_TARGET
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_StickyHoldActivatesRet:
	pause 0x10
	call BattleScript_AbilityPopUp
	setword BATTLE_STRING_LOADER ItemCantBeRemovedString
	printstring 0x184
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_WasFrozen:
	statusanimation 0x2
	printfromtable 0x83FE5CC
	waitmessage DELAY_1SECOND
	refreshhpbar 0x2
	waitstateatk
	formchange 0x2 SPECIES_SHAYMIN_SKY SPECIES_SHAYMIN TRUE TRUE TRUE TargetFrozenReturn
	playanimation 0x2 ANIM_TRANSFORM 0x0
	copyarray BATTLE_SCRIPTING_BANK EFFECT_BANK 0x1
	setword BATTLE_STRING_LOADER TransformedString
	printstring 0x184
	waitmessage DELAY_1SECOND	

TargetFrozenReturn:
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MoveEffectWrap:
	printfromtable gWrappedStringIds
	waitmessage DELAY_1SECOND
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_KnockedOff:
	callasm TransferLastUsedItem
	playanimation BANK_TARGET ANIM_KNOCK_OFF_ITEM 0x0
	printstring 0xB7 @;STRINGID_PKMNKNOCKEDOFF
	waitmessage DELAY_1SECOND
	call 0x81D92DC @;BattleScript_WeatherFormChanges - In case of Utility Umbrella
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_AbilityWasSuppressed:
	setword BATTLE_STRING_LOADER AbilitySuppressedString
	printstring 0x184
	waitmessage DELAY_1SECOND
	call BattleScript_TryRemoveIllusion
	callasm TryRemovePrimalWeatherAfterAbilityChange
	call 0x81D92DC @;Try to revert Cherrim and Castform
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_StatUpPartner:
	playanimation BANK_SCRIPTING ANIM_STAT_BUFF ANIM_ARG_1
	setword BATTLE_STRING_LOADER gText_RaidBattleStatBoost
	printstring 0x184
	waitmessage DELAY_1SECOND
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MaxMoveRaiseStatTeam:
	jumpiffainted BANK_ATTACKER BattleScript_MaxMoveRaiseStatTeam_CheckPartner
	callasm SetMaxMoveStatRaiseEffect
	seteffectprimary

BattleScript_MaxMoveRaiseStatTeam_CheckPartner:
	jumpifnotbattletype BATTLE_DOUBLE BattleScript_MaxMoveEffectEnd
	callasm SetAttackerPartner
	jumpiffainted BANK_ATTACKER BattleScript_MaxMoveSetAttackerBackAndReturn
	callasm SetMaxMoveStatRaiseEffect
	seteffectprimary
BattleScript_MaxMoveSetAttackerBackAndReturn:
	callasm SetAttackerPartner
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MaxMoveLowerStatFoes:
	jumpiffainted BANK_TARGET BattleScript_MaxMoveLowerStatFoes_CheckPartner
	callasm SetMaxMoveStatLowerEffect
	seteffectprimary

BattleScript_MaxMoveLowerStatFoes_CheckPartner:
	jumpifnotbattletype BATTLE_DOUBLE BattleScript_MaxMoveEffectEnd
	callasm SetTargetFoePartner
	jumpiffainted BANK_TARGET BattleScript_MaxMoveSetTargetBackAndReturn
	callasm SetMaxMoveStatLowerEffect
	seteffectprimary
BattleScript_MaxMoveSetTargetBackAndReturn:
	callasm SetTargetFoePartner
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_EatEffectBankBerry:
	copybyte BATTLE_SCRIPTING_BANK EFFECT_BANK
	call BattleScript_CheekPouch
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MaxMoveSetWeather:
	playanimation2 BANK_SCRIPTING ANIM_ARG_1 0x0
	printfromtable 0x83fe528 
	waitmessage DELAY_1SECOND 
	call 0x81d92dc
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MaxMoveSetTerrain:
	call BattleScript_SetTerrain
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MaxMoveEffectSporeFoes:
	jumpiffainted BANK_TARGET BattleScript_MaxMoveEffectSporeFoes_CheckPartner
	callasm PickRandomGMaxBefuddleEffect
	seteffectsecondary

BattleScript_MaxMoveEffectSporeFoes_CheckPartner:
	jumpifnotbattletype BATTLE_DOUBLE BattleScript_MaxMoveEffectEnd
	callasm SetTargetFoePartner
	jumpiffainted BANK_TARGET BattleScript_MaxMoveSetTargetBackAndReturn
	callasm PickRandomGMaxBefuddleEffect
	seteffectsecondary
	callasm SetTargetFoePartner
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MaxMoveParalyzeFoes:
	jumpiffainted BANK_TARGET BattleScript_MaxMoveParalyzeFoes_CheckPartner
	callasm SetGMaxVoltCrashEffect
	seteffectsecondary

BattleScript_MaxMoveParalyzeFoes_CheckPartner:
	jumpifnotbattletype BATTLE_DOUBLE BattleScript_MaxMoveEffectEnd
	callasm SetTargetFoePartner
	jumpiffainted BANK_TARGET BattleScript_MaxMoveSetTargetBackAndReturn
	callasm SetGMaxVoltCrashEffect
	seteffectsecondary
	callasm SetTargetFoePartner
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MaxMovePayDayConfuseFoes:
	setmoveeffect MOVE_EFFECT_PAYDAY
	seteffectprimary
	goto BattleScript_MaxMoveConfuseFoes

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MaxMoveCritUp:
	jumpiffainted BANK_ATTACKER BS_MaxMoveCritUp_CheckPartner
	jumpifmaxchistrikecapped BANK_ATTACKER BS_MaxMoveCritUp_CheckPartner
	callasm SetGMaxChiStrikeEffect
	printfromtable 0x83FE5B0
	waitmessage DELAY_1SECOND

BS_MaxMoveCritUp_CheckPartner:
	jumpifnotbattletype BATTLE_DOUBLE BattleScript_MaxMoveEffectEnd
	callasm SetAttackerPartner
	jumpiffainted BANK_ATTACKER BS_MaxMoveCritUp_RestoreOriginalAttacker
	jumpifmaxchistrikecapped BANK_ATTACKER BS_MaxMoveCritUp_RestoreOriginalAttacker
	callasm SetGMaxChiStrikeEffect
	printfromtable 0x83FE5B0
	waitmessage DELAY_1SECOND

BS_MaxMoveCritUp_RestoreOriginalAttacker:
	callasm SetAttackerPartner
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MaxMoveTrap:
	jumpiffainted BANK_TARGET BattleScript_MaxMoveTrap_CheckPartner
	jumpifsecondarystatus BANK_TARGET STATUS2_TRAPPED BattleScript_MaxMoveTrap_CheckPartner
	jumpiftype BANK_TARGET TYPE_GHOST BattleScript_MaxMoveTrap_CheckPartner
	setmoveeffect MOVE_EFFECT_PREVENT_ESCAPE
	seteffectprimary
	printstring 0x8F @;STRINGID_TARGETCANTESCAPENOW
	waitmessage DELAY_1SECOND

BattleScript_MaxMoveTrap_CheckPartner:
	jumpifnotbattletype BATTLE_DOUBLE BattleScript_MaxMoveEffectEnd
	callasm SetTargetFoePartner
	jumpiffainted BANK_TARGET BattleScript_MaxMoveSetTargetBackAndReturn
	jumpifsecondarystatus BANK_TARGET STATUS2_TRAPPED BattleScript_MaxMoveSetTargetBackAndReturn
	jumpiftype BANK_TARGET TYPE_GHOST BattleScript_MaxMoveSetTargetBackAndReturn
	setmoveeffect MOVE_EFFECT_PREVENT_ESCAPE
	seteffectprimary
	printstring 0x8F @;STRINGID_TARGETCANTESCAPENOW
	waitmessage DELAY_1SECOND
	callasm SetTargetFoePartner
	return
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MaxMoveInfatuation:
	jumpiffainted BANK_TARGET BS_MaxMoveInfatuation_CheckPartner
	tryinfatuatebank BANK_TARGET BS_MaxMoveInfatuation_CheckPartner
	status2animation BANK_TARGET STATUS2_INLOVE
	printstring 0x45
	waitmessage DELAY_1SECOND
	jumpifhelditemeffect BANK_TARGET ITEM_EFFECT_DESTINY_KNOT BS_MaxMoveInfatuation_CheckTarget1DestinyKnot

BS_MaxMoveInfatuation_CheckPartner:
	jumpifnotbattletype BATTLE_DOUBLE BattleScript_MaxMoveEffectEnd
	callasm SetTargetFoePartner
	jumpiffainted BANK_TARGET BattleScript_MaxMoveSetTargetBackAndReturn
	tryinfatuatebank BANK_TARGET BattleScript_MaxMoveSetTargetBackAndReturn
	status2animation BANK_TARGET STATUS2_INLOVE
	printstring 0x45
	waitmessage DELAY_1SECOND
	jumpifhelditemeffect BANK_TARGET ITEM_EFFECT_DESTINY_KNOT BS_MaxMoveInfatuation_CheckTarget2DestinyKnot
	callasm SetTargetFoePartner
	return

BS_MaxMoveInfatuation_CheckTarget1DestinyKnot:	
	tryinfatuatebank BANK_ATTACKER BS_MaxMoveInfatuation_CheckPartner
	call BS_MaxMoveInfatuation_ActivateDestinyKnot
	goto BS_MaxMoveInfatuation_CheckPartner

BS_MaxMoveInfatuation_CheckTarget2DestinyKnot:
	tryinfatuatebank BANK_ATTACKER BS_MaxMoveInfatuation_CheckPartner
	call BS_MaxMoveInfatuation_ActivateDestinyKnot
	goto BattleScript_MaxMoveSetTargetBackAndReturn

BS_MaxMoveInfatuation_ActivateDestinyKnot:
	playanimation BANK_TARGET ANIM_ITEM_USE 0x0
	status2animation BANK_ATTACKER STATUS2_INLOVE
	copyarray BATTLE_SCRIPTING_BANK USER_BANK 0x1
	setword BATTLE_STRING_LOADER DestinyKnotString
	printstring 0x184
	waitmessage DELAY_1SECOND
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MaxMoveRecycle:
	recycleberry BANK_ATTACKER BS_MaxMoveRecycle_CheckPartner
	printstring 0x140
	waitmessage DELAY_1SECOND

BS_MaxMoveRecycle_CheckPartner:
	jumpifnotbattletype BATTLE_DOUBLE BattleScript_MaxMoveEffectEnd
	recycleberry BANK_TARGET BattleScript_MaxMoveEffectEnd @;Partner
	callasm SetAttackerPartner @;So the string prints the correct thing
	printstring 0x140
	callasm SetAttackerPartner
	waitmessage DELAY_1SECOND
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MaxMoveHealTeam:
	jumpiffainted BANK_ATTACKER BS_MaxMoveHealTeam_CheckPartner
	jumpifhealthcomparestomax BANK_ATTACKER EQUALS BS_MaxMoveHealTeam_CheckPartner
	setdamagetobankhealthfraction BANK_ATTACKER, -6, 0x1 @;1/6 of Actual Max HP
	orword HIT_MARKER HITMARKER_IGNORE_SUBSTITUTE
	graphicalhpupdate BANK_ATTACKER
	datahpupdate BANK_ATTACKER
	swapattackerwithtarget @;So the string prints the correct thing
	printstring 0x4B @;STRINGID_PKMNREGAINEDHEALTH
	swapattackerwithtarget
	waitmessage DELAY_1SECOND

BS_MaxMoveHealTeam_CheckPartner:
	jumpifnotbattletype BATTLE_DOUBLE BattleScript_MaxMoveEffectEnd
	callasm SetAttackerPartner
	jumpiffainted BANK_ATTACKER BS_MaxMoveHealTeam_RestoreOriginalAttacker
	jumpifhealthcomparestomax BANK_ATTACKER EQUALS BS_MaxMoveHealTeam_RestoreOriginalAttacker
	setdamagetobankhealthfraction BANK_ATTACKER, -6, 0x1 @;1/6 of Actual Max HP
	orword HIT_MARKER HITMARKER_IGNORE_SUBSTITUTE
	graphicalhpupdate BANK_ATTACKER
	datahpupdate BANK_ATTACKER
	swapattackerwithtarget @;So the string prints the correct thing
	printstring 0x4B @;STRINGID_PKMNREGAINEDHEALTH
	swapattackerwithtarget
	waitmessage DELAY_1SECOND

BS_MaxMoveHealTeam_RestoreOriginalAttacker:
	callasm SetAttackerPartner
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MaxMovePoisonFoes:
	jumpiffainted BANK_TARGET BattleScript_MaxMovePoisonFoes_CheckPartner
	callasm SetGMaxMalodorEffect
	seteffectsecondary

BattleScript_MaxMovePoisonFoes_CheckPartner:
	jumpifnotbattletype BATTLE_DOUBLE BattleScript_MaxMoveEffectEnd
	callasm SetTargetFoePartner
	jumpiffainted BANK_TARGET BattleScript_MaxMoveSetTargetBackAndReturn
	callasm SetGMaxMalodorEffect
	seteffectsecondary
	callasm SetTargetFoePartner
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MaxMoveSetStealthRock:
	setspikes BattleScript_MaxMoveEffectEnd
	printstring 0x184
	waitmessage DELAY_1SECOND
BattleScript_MaxMoveEffectEnd:
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MaxMoveDefog:
	breakfree
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MaxMovePoisonParalyzeFoes:
	jumpiffainted BANK_TARGET BattleScript_MaxMovePoisonParalyzeFoes_CheckPartner
	callasm PickRandomGMaxStunshockEffect
	seteffectsecondary

BattleScript_MaxMovePoisonParalyzeFoes_CheckPartner:
	jumpifnotbattletype BATTLE_DOUBLE BattleScript_MaxMoveEffectEnd
	callasm SetTargetFoePartner
	jumpiffainted BANK_TARGET BattleScript_MaxMoveSetTargetBackAndReturn
	callasm PickRandomGMaxStunshockEffect
	seteffectsecondary
	callasm SetTargetFoePartner
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MaxMoveSpite:
	jumpiffainted BANK_TARGET BattleScript_MaxMoveEffectEnd
	printstring 0x8D
	waitmessage DELAY_1SECOND
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MaxMoveSetGravity:
	callasm DoFieldEffect
	printstring 0x184
	waitmessage DELAY_1SECOND
	callasm BringDownMons
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MaxMoveSetYawn:
	callasm ActuallySetYawn
	printstring 0xB6 @;STRINGID_PKMNWASMADEDROWSY
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MaxMoveTrapAndDamageFoes:
	jumpiffainted BANK_TARGET BattleScript_MaxMoveTrapAndDamageFoes_CheckPartner
	setmoveeffect MOVE_EFFECT_WRAP
	seteffectprimary

BattleScript_MaxMoveTrapAndDamageFoes_CheckPartner:
	jumpifnotbattletype BATTLE_DOUBLE BattleScript_MaxMoveEffectEnd
	callasm SetTargetFoePartner
	jumpiffainted BANK_TARGET BattleScript_MaxMoveSetTargetBackAndReturn
	setmoveeffect MOVE_EFFECT_WRAP
	seteffectprimary
	callasm SetTargetFoePartner
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MaxMoveLowerEvasivenessFoes:
	jumpiffainted BANK_TARGET BattleScript_MaxMoveLowerEvasivenessFoes_CheckPartner
	setmoveeffect MOVE_EFFECT_EVS_MINUS_1
	seteffectprimary

BattleScript_MaxMoveLowerEvasivenessFoes_CheckPartner:
	jumpifnotbattletype BATTLE_DOUBLE BattleScript_MaxMoveEffectEnd
	callasm SetTargetFoePartner
	jumpiffainted BANK_TARGET BattleScript_MaxMoveSetTargetBackAndReturn
	setmoveeffect MOVE_EFFECT_EVS_MINUS_1
	seteffectprimary
	callasm SetTargetFoePartner
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MaxMoveAromatherapy:
	healpartystatus
	waitstateatk
	printfromtable 0x83FE5E4
	refreshhpbar 0x4
	waitstateatk
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MaxMoveConfuseFoes:
	jumpiffainted BANK_TARGET BattleScript_MaxMoveConfuseFoes_CheckPartner
	callasm SetGMaxSmiteEffect
	seteffectprimary

BattleScript_MaxMoveConfuseFoes_CheckPartner:
	jumpifnotbattletype BATTLE_DOUBLE BattleScript_MaxMoveEffectEnd
	callasm SetTargetFoePartner
	jumpiffainted BANK_TARGET BattleScript_MaxMoveSetTargetBackAndReturn
	callasm SetGMaxSmiteEffect
	seteffectprimary
	callasm SetTargetFoePartner
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MaxMoveTormentFoes:
	settorment BattleScript_MaxMoveTormentFoes_TryPartner
	printstring 0xA9 @;STRINGID_PKMNSUBJECTEDTOTORMENT
	waitmessage DELAY_1SECOND

BattleScript_MaxMoveTormentFoes_TryPartner:
	jumpifnotbattletype BATTLE_DOUBLE BattleScript_MaxMoveEffectEnd
	callasm SetTargetFoePartner
	jumpiffainted BANK_TARGET BattleScript_MaxMoveSetTargetBackAndReturn
	settorment BattleScript_MaxMoveSetTargetBackAndReturn
	printstring 0xA9 @;STRINGID_PKMNSUBJECTEDTOTORMENT
	waitmessage DELAY_1SECOND
	callasm SetTargetFoePartner
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MaxMoveLowerSpeed2Foes:
	jumpiffainted BANK_TARGET BattleScript_MaxMoveLowerSpeed2Foes_CheckPartner
	setmoveeffect MOVE_EFFECT_SPD_MINUS_2
	seteffectprimary

BattleScript_MaxMoveLowerSpeed2Foes_CheckPartner:
	jumpifnotbattletype BATTLE_DOUBLE BattleScript_MaxMoveEffectEnd
	callasm SetTargetFoePartner
	jumpiffainted BANK_TARGET BattleScript_MaxMoveSetTargetBackAndReturn
	setmoveeffect MOVE_EFFECT_SPD_MINUS_2
	seteffectprimary
	callasm SetTargetFoePartner
	return

.align 2
SlappedAwakeString: .byte 0xFD, 0x10, 0x00, 0xEB, 0xD5, 0xE7, 0x00, 0xE7, 0xE0, 0xD5, 0xE4, 0xE4, 0xD9, 0xD8, 0xFE, 0xD5, 0xEB, 0xD5, 0xDF, 0xD9, 0xAB, 0xFF
BurnCuredString: .byte 0xFD, 0x10, 0xB4, 0xE7, 0x00, 0xD6, 0xE9, 0xE6, 0xE2, 0xFE, 0xEB, 0xD5, 0xE7, 0x00, 0xDC, 0xD9, 0xD5, 0xE0, 0xD9, 0xD8, 0xAB, 0xFF
ItemCantBeRemovedString: .byte 0xFD, 0x10, 0xB4, 0xE7, 0x00, 0xDD, 0xE8, 0xD9, 0xE1, 0xFE, 0xD7, 0xD5, 0xE2, 0xB4, 0xE8, 0x00, 0xD6, 0xD9, 0x00, 0xE6, 0xD9, 0xE1, 0xE3, 0xEA, 0xD9, 0xD8, 0xAB, 0xFF
