.text
.thumb
.align 2

.include "../xse_commands.s"
.include "../xse_defines.s"
.include "../asm_defines.s"

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global SystemScript_EnableAutoRun
SystemScript_EnableAutoRun:
	lock
	checksound
	sound 0x2
	signmsg
	msgboxsign
	msgbox gText_AutoRunEnable MSG_SIGN
	checksound
	end
	
.global SystemScript_DisableAutoRun
SystemScript_DisableAutoRun:
	lock
	checksound
	sound 0x3
	msgboxsign
	msgbox gText_AutoRunDisable MSG_SIGN
	checksound
	end

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global SystemScript_PoisonSurvial
SystemScript_PoisonSurvial:
	lock
	msgboxsign
	msgbox gText_PoisonSurvial MSG_KEEPOPEN
	closeonkeypress
	end
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.equ VAR_REPEL_STEP_COUNT, 0x4020

.global EventScript_BwRepelWoreOff
.global EventScript_RepelWoreOff
EventScript_BwRepelWoreOff:
	lock
	checkitem 0x800E 1
	compare LASTRESULT 1
	if greaterorequal _goto AnotherRepel
	goto EventScript_RepelWoreOff
	
AnotherRepel:
	bufferitem 0x2 0x800E
	msgboxsign
	msgbox gText_AskUseAnotherRepel MSG_YESNO
	compare LASTRESULT 0x1
	if notequal _goto EndScript
	compare 0x800E ITEM_REPEL
	if equal _goto UseAnotherRepel
	compare 0x800E ITEM_SUPERREPEL
	if equal _goto UseAnotherSuperRepel
	compare 0x800E ITEM_MAXREPEL
	if equal _goto UseAnotherMaxRepel
	goto EndScript
	
UseAnotherRepel:
	removeitem ITEM_REPEL 1
	setvar VAR_REPEL_STEP_COUNT 100
	goto FinishNewRepel
	
UseAnotherSuperRepel:
	removeitem ITEM_SUPERREPEL 1
	setvar VAR_REPEL_STEP_COUNT 200
	goto FinishNewRepel
	
UseAnotherMaxRepel:
	removeitem ITEM_MAXREPEL 1
	setvar VAR_REPEL_STEP_COUNT 250
	goto FinishNewRepel
	
FinishNewRepel:
	checksound
	sound 0x29 @;Repel spray sound
	msgboxsign
	msgbox gText_UsedAnotherRepel MSG_SIGN
	goto EndScript
	
EventScript_RepelWoreOff:
	lock
	msgboxsign
	msgbox gText_RepelWoreOff MSG_KEEPOPEN
	closeonkeypress
	
EndScript:
	release
	end
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global SystemScript_StartDexNavBattle
SystemScript_StartDexNavBattle:
	lock
	call FollowerPositionFixScript
	checksound
	sound 0x15 @;Exclaim
	applymovement PLAYER PlayerExclaim
	waitmovement 0x0
	checksound
	dowildbattle
	end

PlayerExclaim:
.byte exclaim, end_m

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global SystemScript_FindItemMessage
SystemScript_FindItemMessage:
	hidesprite LASTTALKED
	callasm ShowItemSpriteOnFind
	additem 0x8004 0x8005
	special2 LASTRESULT 0x196
	copyvar 0x8008 LASTRESULT
	compare 0x8008 0x1
	if equal _call 0x81A6821
	compare 0x8008 0x0
	if equal _call 0x81A6827
	waitfanfare
	waitmsg
	msgbox 0x81A5218 MSG_KEEPOPEN 
	callasm ClearItemSpriteAfterFind
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SystemScript_ObtainItem:
	copyvar 0x8013 0x8012
	copyvar 0x8004 0x8000 @;Copy item to 0x8004
	copyvar 0x8005 0x8001 @;Copy amount to 0x8005
	textcolor BLACK
	additem 0x8000 0x8001
	copyvar 0x8007 LASTRESULT
	call 0x81A6697
	copyvar 0x8012 0x8013
	return

.global SystemScript_ObtainItemMessage
SystemScript_ObtainItemMessage:
	callasm ShowItemSpriteOnFind
	compare 0x8005 1
	if lessorequal _call ObtainedSingleItemMsg
	compare 0x8005 1
	if greaterthan _call ObtainedMultipleItemMsg
	waitfanfare
	waitmsg
	callasm TryAppendSOntoEndOfItemString
	msgbox 0x81A5218 MSG_KEEPOPEN @;[PLAYER] put the item in the...
	setvar LASTRESULT 0x1
	callasm ClearItemSpriteAfterFind
	return

ObtainedSingleItemMsg:
	special2 LASTRESULT 0x196
	compare LASTRESULT 0x0
	if equal _call ObtainedRegularItem
	compare LASTRESULT 0x1
	if equal _call ObtainedTMHM
	return

ObtainedTMHM:
	preparemsg gText_ObtainedTMHM
	return

ObtainedRegularItem:
	preparemsg 0x81A51F6 @;Obtained the item!
	return
	
ObtainedMultipleItemMsg:
	buffernumber 0x0 0x8005
	preparemsg gText_ObtainedMultipleItems
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.equ FLAG_FR_BADGE_6, 0x825
.equ PREVENT_ROUTE_ESCAPE_FLAG, 0x988
.equ HM08_ROCK_CLIMB, 128
.equ SPECIAL_POKEMON_IN_PARTY_THAT_CAN_LEARN_TM_HM, 0xD0
EventScript_RockClimb:
	checkflag FLAG_FR_BADGE_6
	if NOT_SET _goto EventScript_RockClimbDoNothing
	checkflag PREVENT_ROUTE_ESCAPE_FLAG @;Can't climb if someone's following you
	if SET _goto EventScript_RockClimbDoNothing
	setvar 0x8000 HM08_ROCK_CLIMB
	special2 LASTRESULT SPECIAL_POKEMON_IN_PARTY_THAT_CAN_LEARN_TM_HM
	compare LASTRESULT PARTY_SIZE
	if equal _goto EventScript_RockClimbDoNothing
	setanimation 0x0 LASTRESULT
	bufferpartypokemon 0x0 LASTRESULT
	bufferattack 0x1 MOVE_ROCKCLIMB
	msgbox gText_WantToScaleCliff MSG_YESNO
	compare LASTRESULT NO
	if equal _goto EventScript_RockClimbEnd
	msgbox 0x81BDFD7 MSG_NORMAL@;[BUFFER1] used [BUFFER2]!
	goto SystemScript_RockClimb

EventScript_RockClimbDoNothing:
	msgbox gText_RockIsRocky MSG_NORMAL
EventScript_RockClimbEnd:
	end

.equ SPECIAL_GET_PLAYER_FACING, 0x1AA
.global SystemScript_RockClimb
SystemScript_RockClimb:
	lockall
	doanimation 0x25
	waitstate
	special2 PLAYERFACING SPECIAL_GET_PLAYER_FACING
	compare PLAYERFACING DOWN
	if equal _goto SystemScript_RockClimbDown
	compare PLAYERFACING LEFT
	if equal _goto SystemScript_RockClimbLeft
	compare PLAYERFACING RIGHT
	if equal _goto SystemScript_RockClimbRight

SystemScript_RockClimbUp:
	applymovement PLAYER m_RockClimbUp
	waitmovement PLAYER
	callasm ShouldRockClimbContinue
	compare LASTRESULT 0
	if equal _goto SystemScript_RockClimbUpFinish
	goto SystemScript_RockClimbUp

SystemScript_RockClimbDown:
	applymovement PLAYER m_RockClimbDown
	waitmovement PLAYER
	callasm ShouldRockClimbContinue
	compare LASTRESULT 0
	if equal _goto SystemScript_RockClimbDownFinish
	goto SystemScript_RockClimbDown

SystemScript_RockClimbLeft:
	applymovement PLAYER m_RockClimbLeft
SystemScript_RockClimbLeftWaitmovement:
	waitmovement PLAYER
	callasm ShouldRockClimbContinueDiagonally
	compare LASTRESULT 1
	if equal _goto SystemScript_RockClimbLeftUp
	compare LASTRESULT 2
	if equal _goto SystemScript_RockClimbLeftDown
	callasm ShouldRockClimbContinue
	compare LASTRESULT 0
	if equal _goto SystemScript_RockClimbLeftFinish
	goto SystemScript_RockClimbLeft
	
SystemScript_RockClimbLeftUp:
	applymovement PLAYER m_RockClimbLeftUp
	goto SystemScript_RockClimbLeftWaitmovement

SystemScript_RockClimbLeftDown:
	applymovement PLAYER m_RockClimbLeftDown
	goto SystemScript_RockClimbLeftWaitmovement

SystemScript_RockClimbRight:
	applymovement PLAYER m_RockClimbRight
SystemScript_RockClimbRightWaitmovement:
	waitmovement PLAYER
	callasm ShouldRockClimbContinueDiagonally
	compare LASTRESULT 1
	if equal _goto SystemScript_RockClimbRightUp
	compare LASTRESULT 2
	if equal _goto SystemScript_RockClimbRightDown
	callasm ShouldRockClimbContinue
	compare LASTRESULT 0
	if equal _goto SystemScript_RockClimbRightFinish
	goto SystemScript_RockClimbRight
	
SystemScript_RockClimbRightUp:
	applymovement PLAYER m_RockClimbRightUp
	goto SystemScript_RockClimbRightWaitmovement

SystemScript_RockClimbRightDown:
	applymovement PLAYER m_RockClimbRightDown
	goto SystemScript_RockClimbRightWaitmovement

SystemScript_RockClimbDownFinish:
	applymovement PLAYER m_RockClimbDown
	waitmovement PLAYER
	releaseall
	end

SystemScript_RockClimbUpFinish:
	applymovement PLAYER m_RockClimbUp
	waitmovement PLAYER
	releaseall
	end

SystemScript_RockClimbLeftFinish:
	applymovement PLAYER m_RockClimbLeft
	waitmovement PLAYER
	releaseall
	end

SystemScript_RockClimbRightFinish:
	applymovement PLAYER m_RockClimbRight
	waitmovement PLAYER
	releaseall
	end

m_RockClimbDown: .byte 0x1D, end_m
m_RockClimbUp: .byte 0x1e, end_m
m_RockClimbLeft: .byte 0x1f, end_m
m_RockClimbRight: .byte 0x20, end_m

m_RockClimbLeftUp: .byte 0xC8, end_m
m_RockClimbLeftDown: .byte 0xC4, end_m
m_RockClimbRightUp: .byte 0xC9, end_m
m_RockClimbRightDown: .byte 0xC5, end_m

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global SystemScript_Defog
SystemScript_Defog:
	lockall
	doanimation 0x28
	waitstate
	checksound
	sound 0x7E
	setweather 0x0
	doweather
	checksound
	msgbox gText_DefogBlewAwayFog MSG_KEEPOPEN
	releaseall
	end
