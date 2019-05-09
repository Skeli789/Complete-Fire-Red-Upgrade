.text
.thumb
.align 2

.include "..\\xse_commands.s"
.include "..\\xse_defines.s"

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

.global SystemScript_PoisonSurvial
SystemScript_PoisonSurvial:
	bufferpartypokemon 0x0 0x8004
	msgbox gText_PoisonSurvial MSG_SIGN
	end
