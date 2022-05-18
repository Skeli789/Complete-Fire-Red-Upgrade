.thumb
.align 2

.include "../xse_commands.s"
.include "../xse_defines.s"
.include "../asm_defines.s"

.global EventScript_Route10FightingTutor
EventScript_Route10FightingTutor:
	lock
	faceplayer
	msgbox gText_FightingTutorHi MSG_YESNO
	compare LASTRESULT 0x1
	if TRUE _goto EventScript_Route10FightingTutorTeach
	msgbox gText_FightingTutorNo MSG_FACE
	release
	end

EventScript_Route10FightingTutorTeach:
	msgbox gText_SelectMoveFightingTutor MSG_KEEPOPEN
	setvar 0x8000 0x5
	setvar 0x8001 0x6
	setvar 0x8004 0x0
	special 0x158
	waitstate
	closeonkeypress
	compare LASTRESULT 0x0
	if 0x1 _goto EventScript_LowKick
	@@compare LASTRESULT 0x1
	@@if 0x1 _goto EventScript_Superpower
	compare LASTRESULT 0x1
	if 0x1 _goto EventScript_VacuumWave
	compare LASTRESULT 0x2
	if 0x1 _goto EventScript_AuraSphere
	@@compare LASTRESULT 0x4
	@@if 0x1 _goto EventScript_CloseCombat
	release
	end

EventScript_LowKick:
	setvar 0x8005 0x6
	goto EventScript_TeachMove

EventScript_Superpower:
	setvar 0x8005 0x34
	goto EventScript_TeachMove

EventScript_VacuumWave:
	setvar 0x8005 0x37
	goto EventScript_TeachMove

EventScript_AuraSphere:
	setvar 0x8005 0x67
	goto EventScript_TeachMove

EventScript_CloseCombat:
	setvar 0x8005 0x78
	goto EventScript_TeachMove

EventScript_TeachMove:
	special 0x18D
	waitstate
	msgbox gText_FightingTutorComeBack MSG_FACE
	release
	end

.global EventScript_RockSlideTM
EventScript_RockSlideTM:
	lock
	checkflag 0x9CD
	if SET _goto EventScript_HasRockSlide
	msgbox gText_GiveRockSlide MSG_FACE
	giveitem ITEM_TM80 0x1 MSG_OBTAIN
	setflag 0x9CD
	goto EventScript_HasRockSlide
	release
	end

EventScript_HasRockSlide:
	msgbox gText_HasRockSlide MSG_FACE
	release
	end