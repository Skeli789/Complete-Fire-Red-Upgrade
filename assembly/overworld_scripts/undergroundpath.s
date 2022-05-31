.thumb
.align 2

.include "../xse_commands.s"
.include "../xse_defines.s"
.include "../asm_defines.s"

.global EventScript_UPBarry

EventScript_UPBarry:
	lock
	checkflag 0x960
	if NOT_SET _goto EventScript_UPBarryEnc
	release
	end
	

EventScript_UPBarryEnc:
	showsprite 0x1
	applymovement 0x1 EventScript_UPBarryMove1
	waitmovement 0x0
	applymovement 0xFF EventScript_UPPlayerMove1
	waitmovement 0x0
	sound 0x15
	msgbox gText_UPBarryHi MSG_FACE
	trainerbattle3 0x1 0x7 0x0 gText_UPBarryLose
	setflag 0x960
	msgbox gText_UPBarryAfter MSG_FACE
	giveitem ITEM_VENUSAURITE 0x1 MSG_OBTAIN
	giveitem ITEM_BLASTOISINITE 0x1 MSG_OBTAIN
	giveitem ITEM_CHARIZARDITE_X 0x1 MSG_OBTAIN
	giveitem ITEM_CHARIZARDITE_Y 0x1 MSG_OBTAIN
	msgbox gText_UPBarryGo MSG_FACE
	applymovement 0x1 EventScript_UPBarryMove2
	waitmovement 0x0
	hidesprite 0x1
	release
	end

EventScript_UPBarryMove1:
	.byte walk_left
	.byte walk_left
	.byte walk_left
	.byte walk_left
	.byte walk_left
	.byte walk_left
	.byte walk_left
	.byte walk_left
	.byte walk_left
	.byte 0xFE

EventScript_UPBarryMove2:
	.byte walk_right
	.byte walk_right
	.byte walk_right
	.byte walk_right
	.byte walk_right
	.byte walk_right
	.byte walk_right
	.byte walk_right
	.byte walk_right
	.byte 0xFE


EventScript_UPPlayerMove1:
	.byte look_right
	.byte 0x62
	.byte 0xFE
