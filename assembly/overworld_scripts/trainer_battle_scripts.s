.text
.thumb
.align 2

.include "../xse_commands.s"
.include "../xse_defines.s"

@;trainerbattle11 0xA 0x90 0x7A 0x7 0x9 0x0 0x8188ED9 0x87C0E2F 0x8188EFA 0x87C0E61 0x8188F0A 0x8188F53

@;trainerbattle 0xA FOE_1_ID FOE_2_ID PARTNER_ID PARTNER_BACKSPRITE_ID 0x0 DEFEAT_TEXT_A DEFEAT_TEXT_B
TestScript:
	special 0x29
	waitstate
	trainerbattle10 0xA 0x177 0x176 0x26 0x6 0x300 0x87D6C37 0x87B36A2
	return

@trainerbattle12 0xC 0x177 0x26 0x6 0x0 0x87D6C37
@trainerbattle3 0x3 0x177 0xF00 0x87D6C37

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.global EventScript_SetUpNPCSpotting
EventScript_SetUpNPCSpotting:
	lock
	checksound
	sound 0x15 @Exclaim
	special EndTrainerApproach
	waitstate
	call TrainerFaceFixScript
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global EventScript_TrainerSpottedInitiate
EventScript_TrainerSpottedInitiate:
	lock
	special SetUpTrainerEncounterMusic
	special EndTrainerApproach
	waitstate
	call TrainerFaceFixScript
	goto 0x81A4FC7

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global Script_TrainerSpottedMulti
Script_TrainerSpottedMulti:
	lock
	special SetUpTrainerEncounterMusic
	special EndTrainerApproach
	waitstate
	call TrainerFaceFixScript
	special ShowTrainerIntroSpeech
	waitmsg
	waitkeypress
	closeonkeypress
	callasm AllowTrainerIncrementation
	lock
	end

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global EventScript_DoTwoOpponentBattle
EventScript_DoTwoOpponentBattle:
	lock
	special SetUpTrainerEncounterMusic
	special EndTrainerApproach
	waitstate
	
TwoOpponentLoop:
	callasm MoveSecondNPCForTwoOpponentSighting + 1
	compare LASTRESULT 0xFFFF
	if equal _goto StartTwoOpponentBattle
	call WalkCall	
	goto TwoOpponentLoop

StartTwoOpponentBattle:
	call TrainerFaceFixScript
	setvar 0x8000 0x0

TwoOpponentBattleShowMessage:
	callasm LoadProperIntroSpeechForTwoOpponentSighting
	special ShowTrainerIntroSpeech
	waitmsg
	waitkeypress
	closeonkeypress
	special CAMERA_START
	callasm MoveCameraToTrainerB
	call WalkCall
	callasm PrepTrainerB + 1
	faceplayer
	special ShowTrainerIntroSpeech
	waitmsg
	waitkeypress
	closeonkeypress
	callasm MoveCameraToTrainerB @Here it moves the camera back
	call WalkCall
	special CAMERA_END
	goto 0x81A4FCC
	
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global EventScript_TryDoTwoOpponentBattle
EventScript_TryDoTwoOpponentBattle:
	lock
	faceplayer
	call EventScript_RevealTrainer
	callasm HasOneTrainerBeenDefeated + 1
	compare LASTRESULT 0
	if notequal _goto EventScript_TwoOpponent_NoDoubleTrainerBattle
	special HasEnoughMonsForDoubleBattle
	compare LASTRESULT 0
	if notequal _goto EventScript_TwoOpponent_NotEnoughMonsForDoubleBattle
	special SetUpTrainerEncounterMusic
	special SetUpTrainerMovement
	setvar 0x8000 0x1
	goto TwoOpponentBattleShowMessage

EventScript_TwoOpponent_NoDoubleTrainerBattle:
	callasm ClearTwoOpponentFlag @Otherwise next battle will be double
	goto EventScript_NoDoubleTrainerBattle

EventScript_TwoOpponent_NotEnoughMonsForDoubleBattle:
	callasm ClearTwoOpponentFlag @Otherwise next battle will be double
	goto EventScript_NotEnoughMonsForDoubleBattle

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

TrainerFaceFixScript:
	call FollowerPositionFixScript
	callasm TrainerFaceFix + 1
	compare LASTRESULT 0xFFFF
	if equal _goto TrainerFaceFixScriptReturn
	compare LASTRESULT 0x0
	if equal _call LookDownCall
	compare LASTRESULT 0x1
	if equal _call LookUpCall
	compare LASTRESULT 0x2
	if equal _call LookLeftCall
	compare LASTRESULT 0x3
	if equal _call LookRightCall
	waitmovement PLAYER
TrainerFaceFixScriptReturn:
	return

LookDownCall:
	applymovement PLAYER LookDown
	return

LookUpCall:
	applymovement PLAYER LookUp
	return

LookLeftCall:
	applymovement PLAYER LookLeft
	return

LookRightCall:
	applymovement PLAYER LookRight
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global FollowerPositionFixScript
FollowerPositionFixScript:
	setvar 0x8000 0x0
	callasm FollowerTrainerSightingPositionFix
	compare LASTRESULT 0xFFFF
	if equal _goto FollowerPositionFixScriptReturn
	call WalkCall
	setvar 0x8000 0x1
	callasm FollowerTrainerSightingPositionFix
	
FollowerPositionFixScriptReturn:
	return
	
.global FollowerIntoPlayerScript
FollowerIntoPlayerScript:
	setvar 0x8000 0x0
	callasm FollowerIntoPlayer
	compare LASTRESULT 0xFFFF
	if equal _goto FollowerPositionFixScriptReturn
	setvar 0x8005 FOLLOWER
	call WalkCall
	setvar 0x8000 0x1
	callasm FollowerIntoPlayer
	return

WalkCall:
	compare LASTRESULT 0x0
	if equal _call WalkDownCall
	compare LASTRESULT 0x1
	if equal _call WalkUpCall
	compare LASTRESULT 0x2
	if equal _call WalkLeftCall
	compare LASTRESULT 0x3
	if equal _call WalkRightCall
	waitmovement 0x8005
	return
	
WalkDownCall:
	applymovement 0x8005 WalkDown
	return

WalkUpCall:
	applymovement 0x8005 WalkUp
	return

WalkLeftCall:
	applymovement 0x8005 WalkLeft
	return

WalkRightCall:
	applymovement 0x8005 WalkRight
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

LookDown:
.byte look_down, end_m 

LookUp:
.byte look_up, end_m

LookLeft:
.byte look_left, end_m

LookRight:
.byte look_right, end_m

WalkDown:
.byte walk_down, end_m 

WalkUp:
.byte walk_up, end_m

WalkLeft:
.byte walk_left, end_m

WalkRight:
.byte walk_right, end_m

