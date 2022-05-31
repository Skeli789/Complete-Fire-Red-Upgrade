.thumb
.align 2

.include "../xse_commands.s"
.include "../xse_defines.s"
.include "../asm_defines.s"

.equ FLAG_PALKIA_GOT, 0x9A7
.equ FLAG_PALKIA_SPRITE, 0x9A8
.equ FLAG_GOT_CHARMS, 0x99E

.global EventScript_Route21MapScript
EventScript_Route21MapScript:
	mapscript MAP_SCRIPT_ON_LOAD EventScript_HidePalkiaPortal
	.byte MAP_SCRIPT_TERMIN

EventScript_HidePalkiaPortal:
	checkflag FLAG_PALKIA_GOT
	if SET _goto EventScript_HidePalkiaPortal2
	checkflag FLAG_GOT_CHARMS
	if SET _goto EventScript_ShowPalkiaPortal
	hidesprite 7
	setflag FLAG_PALKIA_SPRITE
	end

EventScript_ShowPalkiaPortal:
	showsprite 7
	clearflag FLAG_PALKIA_SPRITE
	end

EventScript_HidePalkiaPortal2:
	hidesprite 7
	setflag FLAG_PALKIA_SPRITE
	end
