.thumb
.align 2

.include "../xse_commands.s"
.include "../xse_defines.s"
.include "../asm_defines.s"

.global EventScript_MayTRH
EventScript_MayTRH:
	lock
	checkitem ITEM_HM06 0x1
	compare LASTRESULT 0x1
	if lessthan _goto EventScript_MayTRHEnc
	release
	end

EventScript_MayTRHEnc:
	showsprite 0x8
	applymovement 0x8 EventScript_MayTRHMove1
	waitmovement 0x0
	msgbox gText_MayTRHi MSG_KEEPOPEN
	closeonkeypress
	trainerbattle3 0x1 0x9 0x0 gText_MayTRHLose
	msgbox gText_MayTRHAfter1 MSG_KEEPOPEN
	closeonkeypress
	giveitem ITEM_HM06 0x1 MSG_OBTAIN
	msgbox gText_MayTRHAfter2 MSG_KEEPOPEN
	closeonkeypress
	applymovement 0x8 EventScript_MayTRHMove2
	waitmovement 0x0
	hidesprite 0x8
	release
	end

EventScript_MayTRHMove1:
	.byte walk_up
	.byte walk_up
	.byte walk_up
	.byte walk_up
	.byte walk_up
	.byte 0xFE

EventScript_MayTRHMove2:
	.byte walk_down
	.byte walk_down
	.byte walk_down
	.byte walk_down
	.byte walk_down
	.byte 0xFE