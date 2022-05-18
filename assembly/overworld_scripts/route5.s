.thumb
.align 2

.include "../xse_commands.s"
.include "../xse_defines.s"
.include "../asm_defines.s"

.global EventScript_R5May
EventScript_R5May:	
	lock
	checkitem ITEM_MEGA_RING 0x1
	compare LASTRESULT 0x1
    if lessthan _goto EventScript_R5MayEnc
	release
	end

EventScript_R5MayEnc:
	opendoor 0x1F 0x1F
	waitdooranim
	showsprite 0x1
	applymovement 0xFF EventScript_R5PlayerMove1
	waitmovement 0x0
	sound 0x15
	applymovement 0x1 EventScript_R5MayMove1
	waitmovement 0x0
	sound 0x15
	closedoor 0x1F 0x1F
	msgbox gText_R5MayBefore MSG_FACE
	trainerbattle3 0x1 0x6 0x0 gText_R5MayLose
	msgbox gText_R5MayAfter MSG_FACE
	giveitem ITEM_MEGA_RING 0x1 MSG_OBTAIN
	msgbox gText_R5MayGtg MSG_FACE
	opendoor 0x1F 0x1F
	waitdooranim
	applymovement 0x1 EventScript_R5MayMove2
	waitmovement 0x0
	hidesprite 0x1
	closedoor 0x1F 0x1F
	release
	end

EventScript_R5MayMove1:
	.byte walk_down
	.byte 0x62
	.byte 0xFE

EventScript_R5PlayerMove1:
	.byte walk_down
	.byte look_up
	.byte 0x62
	.byte 0xFE

EventScript_R5MayMove2:
	.byte look_down
	.byte walk_up
	.byte 0xFE