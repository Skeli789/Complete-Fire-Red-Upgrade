.thumb
.align 2

.include "../xse_commands.s"
.include "../xse_defines.s"
.include "../asm_defines.s"

.global EventScript_SkyBattleJackson
EventScript_SkyBattleJackson:
	lock
	checkflag 0x9D8
	if SET _goto EventScript_SayDefogJackson
	msgbox gText_JacksonHiBird MSG_YESNO
	compare LASTRESULT 0x1
	if equal _goto EventScript_SkyBattleJacksonCheck
	msgbox gText_JacksonKThen MSG_FACE
	release
	end

EventScript_SkyBattleJacksonCheck:
	special 0x51
	compare LASTRESULT 0x0
	if equal _goto EventScript_CantParticipateSky
	setflag 0x901
	trainerbattle3 0x1 56 0x0 gText_LoseJacksonBird
	clearflag 0x901
	msgbox gText_JacksonGiveTM MSG_FACE
	giveitem ITEM_TM109 0x1 MSG_OBTAIN
	setflag 0x9D8
	goto EventScript_SayDefogJackson
	release
	end

EventScript_SayDefogJackson:
	msgbox gText_JacksonDefog MSG_FACE
	release
	end

EventScript_CantParticipateSky:
	msgbox gText_JacksonNoBird MSG_FACE
	release
	end