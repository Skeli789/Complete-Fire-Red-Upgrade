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
	msgbox gText_AutoRunEnable MSG_SIGN
	checksound
	end
	
.global SystemScript_DisableAutoRun
SystemScript_DisableAutoRun:
	lock
	checksound
	sound 0x3
	msgbox gText_AutoRunDisable MSG_SIGN
	checksound
	end

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global SystemScript_PoisonSurvial
SystemScript_PoisonSurvial:
	bufferpartypokemon 0x0 0x8004
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
	msgbox gText_UsedAnotherRepel MSG_SIGN
	goto EndScript
	
EventScript_RepelWoreOff:
	lock
	msgbox gText_RepelWoreOff MSG_KEEPOPEN
	closeonkeypress
	
EndScript:
	release
	end
