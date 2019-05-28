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
