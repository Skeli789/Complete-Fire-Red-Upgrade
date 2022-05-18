.thumb
.align 2

.include "../xse_commands.s"
.include "../xse_defines.s"
.include "../asm_defines.s"


	@---------------
EventScript_guard_1:
	lockall
	setvar 0x4001 0x0
	setvar 0x5100 0x0
	goto EventScript_guard_0X16FABB

EventScript_guard_2:
	lockall
	setvar 0x4001 0x1
	setvar 0x5100 0x0
	goto EventScript_guard_0X16FABB

EventScript_guard_3:
	lockall
	setvar 0x4001 0x2
	setvar 0x5100 0x0
	goto EventScript_guard_0X16FABB

EventScript_guard_4:
	lockall
	setvar 0x4001 0x0
	setvar 0x5100 0x1
	goto EventScript_guard_0X16FABB

EventScript_guard_5:
	lockall
	setvar 0x4001 0x1
	setvar 0x5100 0x1
	goto EventScript_guard_0X16FABB

EventScript_guard_6:
	lockall
	setvar 0x4001 0x2
	setvar 0x5100 0x1
	goto EventScript_guard_0X16FABB

	@---------------
EventScript_guard_0X16FABB:
	checkflag 0x824
	if 0x0 _goto EventScript_guard_noKogaBadge
	textcolor 0x0
	applymovement 0xFF EventScript_guard_0X1A75E9
	waitmovement 0x0
	checkflag 0x2A6
	if 0x1 _goto EventScript_guard_0X16FAE5
	msgbox gText_guard_0X19E2B7 MSG_KEEPOPEN @"I@m on guard duty.\nGee, I@m thirs..."
	closeonkeypress
	applymovement 0xFF EventScript_guard_0X16FB51
	waitmovement 0x0
	releaseall
	end

EventScript_guard_noKogaBadge:
	msgbox gText_guard_Badge MSG_FACE
	compare 0x4001 0x0
	if 0x1 _call EventScript_guard_0X16FB29
	compare 0x4001 0x1
	if 0x1 _call EventScript_guard_0X16FB34
	compare 0x4001 0x2
	if 0x1 _call EventScript_guard_0X16FB3F
	msgbox gText_guard_Badge2 MSG_FACE
	compare 0x5100 0x0
	if 0x1 _call EventScript_guard_moveleft1
	compare 0x5100 0x1
	if 0x1 _call EventScript_guard_moveright1
	releaseall
	end

EventScript_guard_moveleft1:
	applymovement 0xFF EventScript_guard_moveleft
	waitmovement 0x0
	return
EventScript_guard_moveright1:
	applymovement 0xFF EventScript_guard_moveright
	waitmovement 0x0
	return

	@---------------
EventScript_guard_0X16FAE5:
	removeitem 0x171 0x1
	goto EventScript_guard_0X16FAF0

	@---------------
EventScript_guard_0X16FAF0:
	msgbox gText_guard_0X19E307 MSG_KEEPOPEN @"Oh, that Tea[.]\nIt looks awfully ..."
	closeonkeypress
	compare 0x4001 0x0
	if 0x1 _call EventScript_guard_0X16FB29
	compare 0x4001 0x1
	if 0x1 _call EventScript_guard_0X16FB34
	compare 0x4001 0x2
	if 0x1 _call EventScript_guard_0X16FB3F
	msgbox gText_guard_0X19E32D MSG_KEEPOPEN @"Huh? I can have this drink?\nGee, ..."
	setvar 0x4062 0x1
	releaseall
	end

	@---------------
EventScript_guard_0X16FB29:
	applymovement 0xFF EventScript_guard_0X16FB4F
	waitmovement 0x0
	return

	@---------------
EventScript_guard_0X16FB34:
	applymovement 0xFF EventScript_guard_0X16FB4A
	waitmovement 0x0
	return

	@---------------
EventScript_guard_0X16FB3F:
	applymovement 0xFF EventScript_guard_0X16FB4C
	waitmovement 0x0
	return


	@---------
	@ Strings
	@---------


EventScript_guard_moveleft:
.byte walk_left
.byte 0xFE

EventScript_guard_moveright:
.byte walk_right
.byte 0xFE

	@-----------
	@ Movements
	@-----------
EventScript_guard_0X1A75E9:
.byte 0x2E
.byte 0xFE

EventScript_guard_0X16FB51:
.byte 0x12
.byte 0xFE

EventScript_guard_0X16FB4F:
.byte 0x26
.byte 0xFE

EventScript_guard_0X16FB4A:
.byte 0x11
.byte 0xFE

EventScript_guard_0X16FB4C:
.byte 0x11
.byte 0x11
.byte 0xFE

	@---------------
EventScript_guard2_0X16F9D0_1:
	lockall
	setvar 0x4001 0x0
	setvar 0x5100 0x2
	goto EventScript_guard2_0X16F9F4

EventScript_guard2_0X16F9D0_2:
	lockall
	setvar 0x4001 0x1
	setvar 0x5100 0x2
	goto EventScript_guard2_0X16F9F4

EventScript_guard2_0X16F9D0_3:
	lockall
	setvar 0x4001 0x2
	setvar 0x5100 0x2
	goto EventScript_guard2_0X16F9F4

EventScript_guard2_0X16F9D0_4:
	lockall
	setvar 0x4001 0x0
	setvar 0x5100 0x3
	goto EventScript_guard2_0X16F9F4

EventScript_guard2_0X16F9D0_5:
	lockall
	setvar 0x4001 0x1
	setvar 0x5100 0x3
	goto EventScript_guard2_0X16F9F4

EventScript_guard2_0X16F9D0_6:
	lockall
	setvar 0x4001 0x2
	setvar 0x5100 0x3
	goto EventScript_guard2_0X16F9F4

	@---------------
EventScript_guard2_0X16F9F4:
	checkflag 0x824
	if 0x0 _goto EventScript_guard_noKogaBadge2
	textcolor 0x0
	applymovement 0xFF EventScript_guard2_0X1A75EB
	waitmovement 0x0
	checkflag 0x2A6
	if 0x1 _goto EventScript_guard2_0X16FA1E
	msgbox gText_guard_0X19E2B7 MSG_KEEPOPEN @"I@m on guard duty.\nGee, I@m thirs..."
	closeonkeypress
	applymovement 0xFF EventScript_guard2_0X16FA8A
	waitmovement 0x0
	releaseall
	end

EventScript_guard_noKogaBadge2:
	msgbox gText_guard_Badge MSG_FACE
	compare 0x4001 0x0
	if 0x1 _call EventScript_guard2_0X16FA62
	compare 0x4001 0x1
	if 0x1 _call EventScript_guard2_0X16FA6D
	compare 0x4001 0x2
	if 0x1 _call EventScript_guard2_0X16FA78
	msgbox gText_guard_Badge2 MSG_FACE
	compare 0x5100 0x2
	if 0x1 _call EventScript_guard_movedown1
	compare 0x5100 0x3
	if 0x1 _call EventScript_guard_moveup1
	releaseall
	end

EventScript_guard_movedown1:
	applymovement 0xFF EventScript_guard_movedown
	waitmovement 0x0
	return
EventScript_guard_moveup1:
	applymovement 0xFF EventScript_guard_moveup
	waitmovement 0x0
	return

	@---------------
EventScript_guard2_0X16FA1E:
	removeitem 0x171 0x1
	goto EventScript_guard2_0X16FA29

	@---------------
EventScript_guard2_0X16FA29:
	msgbox gText_guard_0X19E307 MSG_KEEPOPEN @"Oh, that Tea[.]\nIt looks awfully ..."
	closeonkeypress
	compare 0x4001 0x0
	if 0x1 _call EventScript_guard2_0X16FA62
	compare 0x4001 0x1
	if 0x1 _call EventScript_guard2_0X16FA6D
	compare 0x4001 0x2
	if 0x1 _call EventScript_guard2_0X16FA78
	msgbox gText_guard_0X19E32D MSG_KEEPOPEN @"Huh? I can have this drink?\nGee, ..."
	setvar 0x4062 0x1
	releaseall
	end

	@---------------
EventScript_guard2_0X16FA62:
	applymovement 0xFF EventScript_guard2_0X16FA83
	waitmovement 0x0
	return

	@---------------
EventScript_guard2_0X16FA6D:
	applymovement 0xFF EventScript_guard2_0X16FA86
	waitmovement 0x0
	return

	@---------------
EventScript_guard2_0X16FA78:
	applymovement 0xFF EventScript_guard2_0X16FA88
	waitmovement 0x0
	return


	@---------
	@ Strings
	@---------

EventScript_guard_movedown:
.byte walk_down
.byte 0xFE

EventScript_guard_moveup:
.byte walk_up
.byte 0xFE


	@-----------
	@ Movements
	@-----------
EventScript_guard2_0X1A75EB:
.byte 0x30
.byte 0xFE

EventScript_guard2_0X16FA8A:
.byte 0x10
.byte 0xFE

EventScript_guard2_0X16FA83:
.byte 0x13
.byte 0x13
.byte 0xFE

EventScript_guard2_0X16FA86:
.byte 0x13
.byte 0xFE

EventScript_guard2_0X16FA88:
.byte 0x28
.byte 0xFE
