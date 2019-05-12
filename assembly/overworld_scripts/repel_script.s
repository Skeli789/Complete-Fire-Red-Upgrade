.text
.thumb
.align 2

.include "../xse_commands.s"
.include "../xse_defines.s"
.include "../asm_defines.s"

.equ VAR_REPEL_STEP_COUNT, 0x4020

.global EventScript_BwRepelWoreOff
EventScript_BwRepelWoreOff:
	lock
	checkitem 0x800E 1
	compare LASTRESULT 1
	if greaterorequal _goto AnotherRepel
	goto NoRepelLeft
	
AnotherRepel:
	bufferitem 0x2 0x800E
	msgbox AskUseAnother 0x5
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
	msgbox NewRepel 0x3
	goto EndScript
	
NoRepelLeft:
	msgbox 0x81BFB6E 0x3
	goto EndScript
	
EndScript:
	release
	end
	
AskUseAnother:
	.byte R_, e_, p_, e_, l_, Apos, s_, Space, e_, f_, f_, e_, c_, t_, Space, w_, o_, r_, e_, Space, o_, f_, f_, Exclam, JumpLine, U_, s_, e_, Space, A_, n_, o_, t_, h_, e_, r_, Space, 0xfd, 0x4, Interro, 0xff

NewRepel:
	.byte 0xFD, 0x1, Space, u_, s_, e_, d_, Space, t_, h_, e_, JumpLine, 0xfd, 0x4, Dot, 0xff

