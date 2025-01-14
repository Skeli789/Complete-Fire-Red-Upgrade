.thumb
.align 2

/*
end_battle_battle_scripts.s
	Contains battle scripts that run at the end of a battle.
*/

.include "../asm_defines.s"
.include "../battle_script_macros.s"

.global BattleScript_Victory
.global BattleScript_PrintPlayerForfeited
.global BattleScript_PrintPlayerForfeitedLinkBattle
.global BattleScript_LostBattleTower
.global BattleScript_LostMultiBattleTower
.global BattleScript_AskIfWantsToForfeitMatch
.global BattleScript_ConfirmRunAway
.global BattleScript_LoseRaidBattle
.global BattleScript_LoseFrontierRaidBattle
.global BattleScript_RaidMonRanAway
.global BattleScript_RaidMonEscapeBall
.global BattleScript_RanAwayUsingMonAbility
.global BattleScript_TryTakeWildMonItem
.global BattleScript_TakeWildMonItem

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

@0x80159DC with r0
EndBattleFlagClearHook: @Not really a BS but whatever
	bl EndOfBattleThings
	ldr r1, =gBattleMainFunc
	ldr r0, .CallbackReturnToOverworld
	str r0, [r1]
	ldr r1, =gCB2_AfterEvolution
	ldr r0, =0x80159E4 | 1
	bx r0

.align 2
.CallbackReturnToOverworld: .word 0x8015A30 | 1

.pool

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_Victory:
	jumpifword ANDS BATTLE_TYPE BATTLE_TWO_OPPONENTS BeatTwoPeeps
	printstring 0x141
	goto PostBeatString
	
BeatTwoPeeps:
	setword BATTLE_STRING_LOADER BattleText_TwoInGameTrainersDefeated
	printstring 0x184

PostBeatString:
	trainerslidein 0x1
	waitstateatk
	jumpifword ANDS BATTLE_TYPE BATTLE_E_READER 0x81D88FF @Just Pickup Calc
	printstring 0xC
	jumpifword NOTANDS BATTLE_TYPE BATTLE_TWO_OPPONENTS CheckJumpLocForEndBattle
	callasm TrainerSlideOut+1
	waitstateatk
	trainerslidein 0x3
	waitstateatk
	setword BATTLE_STRING_LOADER TrainerBLoseString
	printstring 0x184

CheckJumpLocForEndBattle:
	jumpifword ANDS BATTLE_TYPE BATTLE_FRONTIER 0x81D8900 @No Money Give
	jumpifword NOTANDS BATTLE_TYPE BATTLE_TRAINER_TOWER 0x81D87F8 @Give Money
	jumpifword NOTANDS BATTLE_TYPE BATTLE_DOUBLE 0x81D88FF @Just Pickup Calc
	printstring 0x177 @Buffer Trainer Tower Win Text
	goto 0x81D88FF @Just Pickup Calc
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_PrintPlayerForfeited:	
	setword BATTLE_STRING_LOADER sText_PlayerForfeitedMatch
	printstring 0x184
	waitmessage DELAY_1SECOND
	end2
	
BattleScript_PrintPlayerForfeitedLinkBattle:
	setword BATTLE_STRING_LOADER sText_PlayerForfeitedMatch
	printstring 0x184
	waitmessage DELAY_1SECOND
	flee
	waitmessage DELAY_1SECOND
	end2

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_LostBattleTower:
	returnopponentmon1toball BANK_ATTACKER
	waitstateatk
	callasm ReturnOpponentMon2
	waitstateatk
	trainerslidein BANK_ATTACKER
	waitstateatk
	setword BATTLE_STRING_LOADER TrainerAWinString
	printstring 0x184
	flee
	waitmessage DELAY_1SECOND
	end2
	
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_LostMultiBattleTower:
	returnopponentmon1toball BANK_ATTACKER
	waitstateatk
	callasm ReturnOpponentMon2
	waitstateatk
	trainerslidein BANK_ATTACKER
	waitstateatk
	setword BATTLE_STRING_LOADER TrainerAWinString
	printstring 0x184
	callasm TrainerSlideOut
	waitstateatk
	trainerslidein 0x3
	waitstateatk
	setword BATTLE_STRING_LOADER TrainerBVictoryString
	printstring 0x184
	flee
	waitmessage DELAY_1SECOND
	end2

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_AskIfWantsToForfeitMatch:
	setword BATTLE_STRING_LOADER sText_QuestionForfeitMatch
	printstring 0x184
	callasm DisplayForfeitYesNoBox
	callasm HandleForfeitYesNoBox
	end2

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_ConfirmRunAway:
	setword BATTLE_STRING_LOADER sText_ConfirmRunAway
	printstring 0x184
	callasm DisplayForfeitYesNoBox
	callasm HandleForfeitYesNoBox
	end2

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_LoseRaidBattle:
	playanimation BANK_SCRIPTING ANIM_RAID_BATTLE_STORM 0x0
	playanimation BANK_SCRIPTING ANIM_RAID_BATTLE_BLOW_AWAY 0x0
	printstring 0x184
	waitmessage DELAY_1SECOND
	setbyte BATTLE_OUTCOME 0x5 @;Teleported
	end2

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_LoseFrontierRaidBattle:
	setword BATTLE_STRING_LOADER gText_RaidBattleKO4
	printstring 0x184
	waitmessage DELAY_1SECOND
	setbyte BATTLE_OUTCOME 0x5 @;Teleported
	end2

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_RaidMonRanAway:
	setword BATTLE_STRING_LOADER gText_RaidMonRanAway
	printstring 0x184
	waitmessage DELAY_1SECOND
	end2

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_RaidMonEscapeBall:
	printfromtable 0x83FE5F2 @;gBallEscapeStringIds
	waitmessage DELAY_1SECOND
	copybyte BATTLE_SCRIPTING_BANK TARGET_BANK
	callasm MakeScriptingBankInvisible
	setword BATTLE_STRING_LOADER gText_RaidMonRanAway
	printstring 0x184
	waitmessage DELAY_1SECOND
	setbyte BATTLE_OUTCOME 0x1 @B_OUTCOME_WON
	finishaction

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_RanAwayUsingMonAbility:
	printstring 0x130 @;STRINGID_EMPTYSTRING3
	call BattleScript_AbilityPopUp
	printstring 0xDF @;STRINGID_GOTAWAYSAFELY
	waitmessage DELAY_1SECOND
	end2

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_TryTakeWildMonItem:
	callasm WipeYesNoBattleBoxes
	setword BATTLE_STRING_LOADER gText_OfferTakeCaughtMonItem
	printstring 0x184
	callasm DisplayForfeitYesNoBox
	callasm HandleTryTakeItemFromCaughtMonInput
BattleScript_TakeWildMonItem:
	callasm WipeYesNoBattleBoxes
	callasm TakeItemFromCaughtMon
	printstring 0x184
	waitmessage DELAY_1SECOND
testaassd:
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.align 2
TrainerBLoseString: .byte 0xFD, 0x30, 0xFF
TrainerBVictoryString: .byte 0xFD, 0x31, 0xFF
TrainerAWinString: .byte 0xFD, 0x25, 0xFF
