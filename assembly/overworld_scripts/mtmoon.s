.thumb
.align 2

.include "../xse_commands.s"
.include "../xse_defines.s"
.include "../asm_defines.s"

.equ FLAG_GOT_CHARMS, 0x99E
.equ FLAG_ZYGARDE_GOT, 0x99F
.equ FLAG_ZYGARDE_SPRITE, 0x9A0

.global EventScript_MtMoonRootFossil
EventScript_MtMoonRootFossil:
	lock
	giveitem ITEM_ROOT_FOSSIL 0x1 MSG_FIND
	setflag 0x952
	release
	end

.global EventScript_MtMoonClawFossil
EventScript_MtMoonClawFossil:
	lock
	giveitem ITEM_CLAW_FOSSIL 0x1 MSG_FIND
	setflag 0x953
	release
	end

.global EventScript_MtMoonSkullFossil
EventScript_MtMoonSkullFossil:
	lock
	giveitem ITEM_SKULL_FOSSIL 0x1 MSG_FIND
	setflag 0x954
	release
	end

.global EventScript_MtMoonArmorFossil
EventScript_MtMoonArmorFossil:
	lock
	giveitem ITEM_ARMOR_FOSSIL 0x1 MSG_FIND
	setflag 0x955
	release
	end

.global EventScript_MtMoonCoverFossil
EventScript_MtMoonCoverFossil:
	lock
	giveitem ITEM_COVER_FOSSIL 0x1 MSG_FIND
	setflag 0x956
	release
	end

.global EventScript_MtMoonPlumeFossil
EventScript_MtMoonPlumeFossil:
	lock
	giveitem ITEM_PLUME_FOSSIL 0x1 MSG_FIND
	setflag 0x957
	release
	end

.global EventScript_MtMoonJawFossil
EventScript_MtMoonJawFossil:
	lock
	giveitem ITEM_JAW_FOSSIL 0x1 MSG_FIND
	setflag 0x958
	release
	end

.global EventScript_MtMoonSailFossil
EventScript_MtMoonSailFossil:
	lock
	giveitem ITEM_SAIL_FOSSIL 0x1 MSG_FIND
	setflag 0x959
	release
	end

.global EventScript_MtMoon_Barry
EventScript_MtMoon_Barry:
	lockall
	checkitem ITEM_POCKET_PC 0x1
	compare LASTRESULT 0x1
	if lessthan _goto EventScript_MtMoon_BarryEnc
	releaseall
	end

	EventScript_MtMoon_BarryEnc:
	showsprite 0xE 
	applymovement 0xE EventScript_MtMoon_Barry_Movement1
	waitmovement 0x0
	msgbox gText_MtMoon_BarryNoAttention MSG_KEEPOPEN
	closeonkeypress
	applymovement 0xE EventScript_MtMoon_Barry_Movement2
	waitmovement 0x0
	sound 0x15
	msgbox gText_MtMoon_BarryThud MSG_FACE
	applymovement 0xFF EventScript_MtMoon_PlayerFaceBarry
	waitmovement 0x0
	msgbox gText_MtMoon_BarryHi MSG_FACE
	trainerbattle3 0x1 0x5 0x0 gText_MtMoon_BarryLose
	msgbox gText_MtMoon_GivePocketPC MSG_FACE
	giveitem ITEM_POCKET_PC 0x1 MSG_OBTAIN
	msgbox gText_MtMoon_BarryAfter MSG_FACE
	applymovement 0xE EventScript_MtMoon_Barry_Movement3
	waitmovement 0x0
	hidesprite 0xE
	release
	end

EventScript_MtMoon_Barry_Movement1:
	.byte walk_left
	.byte walk_left
	.byte walk_left
	.byte walk_up
	.byte walk_up
	.byte walk_up
	.byte walk_up
	.byte walk_up
	.byte 0xFE

EventScript_MtMoon_Barry_Movement2:
	.byte walk_up
	.byte walk_up
	.byte walk_up
	.byte 0x62
	.byte 0xFE

EventScript_MtMoon_Barry_Movement3:
	.byte walk_down
	.byte walk_down
	.byte walk_down
	.byte walk_down
	.byte walk_down
	.byte walk_down
	.byte walk_down
	.byte walk_up
	.byte 0xFE

EventScript_MtMoon_PlayerFaceBarry:
	.byte look_down
	.byte 0xFE

.global MapScript_MtMoonPortal
MapScript_MtMoonPortal:
	mapscript MAP_SCRIPT_ON_LOAD EventScript_HideZygardePortal
	.byte MAP_SCRIPT_TERMIN

EventScript_HideZygardePortal:
	checkflag FLAG_ZYGARDE_GOT
	if SET _goto EventScript_HideZygardePortal2
	checkflag FLAG_GOT_CHARMS
	if SET _goto EventScript_ShowZygardePortal
	hidesprite 4
	setflag FLAG_ZYGARDE_SPRITE
	end

EventScript_ShowZygardePortal:
	showsprite 4
	clearflag FLAG_ZYGARDE_SPRITE
	end

EventScript_HideZygardePortal2:
	hidesprite 4
	setflag FLAG_ZYGARDE_SPRITE
	end