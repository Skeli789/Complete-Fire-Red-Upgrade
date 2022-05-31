.thumb
.align 2

.include "../xse_commands.s"
.include "../xse_defines.s"
.include "../asm_defines.s"

.equ FLAG_RAID_BUG_FIXED, 0x977
.equ SPECIAL_CLEAR_RAID_FLAG, 0x11A
.equ FLAG_AUTO_HMS, 0x998

.equ EventScript_CableClub_AbortLink, 0x81BB82F

.global EventScript_Pokecentre
EventScript_pokecentre_0X16A2CF:
	lock
	faceplayer
	call EventScript_pokecentre_0X1A6578
	release
	end

	@---------------
EventScript_pokecentre_0X1A6578:
	call GiveAllMons
	special 0x187
	compare LASTRESULT 0x2
	if 0x1 _goto EventScript_pokecentre_0X1A7AE0
	preparemsg gText_pokecentre_0X1A5483
	waitmsg
	multichoice 0x13 0x8 0x0 0x2
	copyvar 0x8000 LASTRESULT
	compare 0x8000 0x0
	if 0x1 _goto EventScript_pokecentre_0X1A65B8
	compare 0x8000 0x1
	if 0x1 _goto EventScript_pokecentre_0X1A6663
	compare 0x8000 0x7F
	if 0x1 _goto EventScript_pokecentre_0X1A6663
	end

	@---------------
EventScript_pokecentre_0X1A7AE0:
	release
	end

	@---------------
EventScript_pokecentre_0X1A65B8:
	@@cmdc3 0xF
	setflag 0x94E
	checkitem ITEM_POCKET_CENTRE 0x1
	compare LASTRESULT 0x0
	if 0x1 _call EventScript_pokecentre_GivePocketCentre
	preparemsg gText_pokecentre_0X1A54E1
	waitmsg
	call EventScript_pokecentre_0X1A65CE
	special 0x169
	goto EventScript_pokecentre_0X1A65EC


EventScript_pokecentre_GivePocketCentre:
	msgbox gText_GivePocketCentre MSG_KEEPOPEN
	closeonkeypress
	giveitem ITEM_POCKET_CENTRE 0x1 MSG_OBTAIN
	return

	@---------------
EventScript_pokecentre_0X1A6663:
	msgbox gText_pokecentre_0X1A5511 MSG_KEEPOPEN
	return

	@---------------
EventScript_pokecentre_0X1A65CE:
	applymovement LASTTALKED EventScript_pokecentre_0X1A75E7
	waitmovement 0x0
	doanimation 0x19
	waitanimation 0x19
	applymovement LASTTALKED EventScript_pokecentre_0X1A75ED
	waitmovement 0x0
	special 0x0
	return

	@---------------
EventScript_pokecentre_0X1A65EC:
	special2 LASTRESULT 0x1B1
	compare LASTRESULT 0x0
	if 0x1 _goto EventScript_pokecentre_0X1A661D
	special2 LASTRESULT 0x183
	copyvar 0x8008 LASTRESULT
	compare 0x8008 0x0
	if 0x1 _goto EventScript_pokecentre_0X1A661D
	compare 0x8008 0x1
	if 0x1 _goto EventScript_pokecentre_0X1A6636
	end

	@---------------
EventScript_pokecentre_0X1A661D:
	preparemsg gText_pokecentre_0X1A552B
	waitmsg
	applymovement LASTTALKED EventScript_pokecentre_0X1A666C
	waitmovement 0x0
	msgbox gText_pokecentre_0X1A5511 MSG_KEEPOPEN
	return

	@---------------
EventScript_pokecentre_0X1A6636:
	checkflag 0x842
	if 0x1 _goto EventScript_pokecentre_0X1A661D
	msgbox gText_pokecentre_0X1A552B MSG_KEEPOPEN
	setflag 0x842
	preparemsg gText_pokecentre_0X1BCCCA 
	waitmsg
	applymovement LASTTALKED EventScript_pokecentre_0X1A666C
	waitmovement 0x0
	msgbox gText_pokecentre_0X1A5511 MSG_KEEPOPEN
	return


	@---------
	@ Strings
	@---------






	@-----------
	@ Movementsmsgbox gText_AIOMan_0X8
	@-----------
EventScript_pokecentre_0X1A75E7:
.byte 0x2F
.byte 0xFE

EventScript_pokecentre_0X1A75ED:
.byte 0x2D
.byte 0xFE

EventScript_pokecentre_0X1A666C:
.byte 0x5B
.byte 0x1A
.byte 0xFE

.global EventScript_AIOMan
	@---------------
EventScript_AIOMan_0X8037C3:
	setflag 0x927
	setflag FLAG_AUTO_HMS
	msgbox gText_AIOMan_0X803472 MSG_FACE @"Hello! How may I help you?"
	multichoiceoption gText_AIOMan_0X80348F 0
	multichoiceoption gText_AIOMan_0X803742 1
	multichoiceoption gText_AIOMan_0X803C60 2
	@@multichoiceoption gText_ChangeR 3
	multichoiceoption gText_AIOMan_0X8034AA 3
	multichoice 0x0 0x0 FOUR_MULTICHOICE_OPTIONS 0x0
	copyvar 0x4001 LASTRESULT
	compare 0x4001 0x0 
	if 0x1 _goto EventScript_AIOMan_0X1716BE
	compare 0x4001 0x1
	if 0x1 _goto EventScript_AIOMan_0X16D8B0
	compare 0x4001 0x2
	if 0x1 _goto EventScript_AIOMan_0X16B287
	@@compare 0x4001 0x3
	@@if 0x1 _goto EventScript_SelectRButtonMode
	compare 0x4001 0x3
	if 0x1 _goto EventScript_AIOMan_0X803C71
	release
	end

@@ unused for now
EventScript_FixSprites:
	setflag 0x951
	setflag 0x95A
	setflag 0x95C
	setflag 0x95F
	setflag 0x961
	setflag 0x963
	setflag 0x969
	msgbox gText_AIOMan_SpritesFixed MSG_FACE
	release
	end

EventScript_SelectRButtonMode:
	multichoiceoption gText_DexNavEnc 0
	multichoiceoption gText_PokemonMenu 1
	multichoiceoption gText_ItemMenu 2
	multichoiceoption sNoThanks 3
	preparemsg gText_WhatHappenR 
	waitmsg
	multichoice 0x0 0x0 FOUR_MULTICHOICE_OPTIONS 0x0
	switch LASTRESULT
	case 0, RMode_DexNav
	case 1, RMode_PkmMenu
	case 2, RMode_ItemBag
	case 3, EventScript_AIOMan_0X8037C3
	goto EventScript_AIOMan_0X8037C3
	release
	end

RMode_DexNav:
	setvar 0x5150 0
	msgbox gText_DexNavEnc2 MSG_FACE
	goto EventScript_AIOMan_0X8037C3
	release
	end

RMode_PkmMenu:
	setvar 0x5150 1
	msgbox gText_PkmMenu MSG_FACE
	goto EventScript_AIOMan_0X8037C3
	release
	end

RMode_ItemBag:
	setvar 0x5150 2
	msgbox gText_ItemBag MSG_FACE
	goto EventScript_AIOMan_0X8037C3
	release
	end

	@---------------
EventScript_AIOMan_0X1716BE:
	msgbox gText_AIOMan_0X1A2CC3 MSG_KEEPOPEN @"Which Pokemon needs tutoring?"
	special 0xDB
	waitstate
	compare 0x8004 0x6
	if 0x4 _goto EventScript_AIOMan_0X8037C3
	special 0x148
	compare LASTRESULT 0x1
	if 0x1 _goto EventScript_AIOMan_0X171790
	compare 0x8005 0x0
	if 0x1 _goto EventScript_AIOMan_0X171782
	goto EventScript_AIOMan_0X1716F4

	@---------------
EventScript_AIOMan_0X16D8B0:
	msgbox gText_AIOMan_0X199421 MSG_KEEPOPEN @"Which Pokemon should forget a\nmov..."
	special 0x9F
	waitstate
	compare 0x8004 0x6
	if 0x4 _goto EventScript_AIOMan_0X16D941
	special 0x148
	compare LASTRESULT 0x1
	if 0x1 _goto EventScript_AIOMan_0X803590
	special 0xDF
	compare LASTRESULT 0x1
	if 0x1 _goto EventScript_AIOMan_0X8036D5
	msgbox gText_AIOMan_0X199445 MSG_KEEPOPEN @"Which move should be forgotten?"
	fadescreen 0x1
	special 0xDC
	fadescreen 0x0
	compare 0x8005 0x4
	if 0x1 _goto EventScript_AIOMan_0X16D8B0
	special 0xDE
	msgbox gText_AIOMan_0X199485 MSG_YESNO @"Hm! [buffer1]@s [buffer2]?\nThat m..."
	compare LASTRESULT 0x1
	if 0x1 _goto EventScript_AIOMan_0X8037AD
	goto EventScript_AIOMan_0X16D941

	@---------------
EventScript_AIOMan_0X16B287:
	msgbox gText_AIOMan_0X193AA6 MSG_KEEPOPEN @"Which Pokemon's nickname should\nI..."
	special 0x9F
	waitstate
	compare 0x8004 0x6
	if 0x0 _goto EventScript_AIOMan_0X16B2B4
	compare 0x8004 0x6
	if 0x4 _goto EventScript_AIOMan_0X16B2AA
	end

	@---------------
EventScript_AIOMan_0X803C71:
	msgbox gText_AIOMan_0X8034B1 MSG_FACE @"I hope to see you again!"
	release
	end

	@---------------
EventScript_AIOMan_0X171790:
	msgbox gText_AIOMan_0X1A2E20 MSG_KEEPOPEN @"Hunh? There isn@t a single move\nt..."
	goto EventScript_AIOMan_0X1716BE

	@---------------
EventScript_AIOMan_0X171782:
	msgbox gText_AIOMan_0X1A2CFC MSG_KEEPOPEN @"Sorry[.]\pIt doesn@t appear as if ..."
	goto EventScript_AIOMan_0X1716BE

	@---------------
EventScript_AIOMan_0X1716F4:
	msgbox gText_AIOMan_0X1A2CE1 MSG_KEEPOPEN @"Which move should I teach?"
	special 0xE0
	waitstate
	compare 0x8004 0x0
	if 0x1 _goto EventScript_AIOMan_0X1716BE
	end

	@---------------
EventScript_AIOMan_0X16D941:
	msgbox gText_AIOMan_0X1994EA MSG_KEEPOPEN @"Come again if there are other\nmov..."
	release
	end

	@---------------
EventScript_AIOMan_0X803590:
	msgbox gText_AIOMan_0X19951F MSG_KEEPOPEN @"What?\nNo Egg should know any move..."
	release
	goto EventScript_AIOMan_0X8037C3

	@---------------
EventScript_AIOMan_0X8036D5:
	special 0xDE
	msgbox gText_AIOMan_0X199465 MSG_KEEPOPEN @"[buffer1] seems to know only one\n..."
	release
	goto EventScript_AIOMan_0X8037C3

	@---------------
EventScript_AIOMan_0X8037AD:
	special 0xDD
	fanfare 0x10E
	waitfanfare
	msgbox gText_AIOMan_0X1994B1 MSG_KEEPOPEN @"It worked to perfection!\p[buffer1..."
	release
	goto EventScript_AIOMan_0X8037C3

	@---------------
EventScript_AIOMan_0X16B2B4:
	special2 LASTRESULT 0x147
	compare LASTRESULT 0x19C
	if 0x1 _goto EventScript_AIOMan_0X16B307
	special 0x7C
	special 0x7D
	compare LASTRESULT 0x1
	if 0x1 _goto EventScript_AIOMan_0X16B311
	special2 LASTRESULT 0x150
	special 0x7C
	compare LASTRESULT 0x1
	if 0x1 _goto EventScript_AIOMan_0X16B311
	msgbox gText_AIOMan_0X193AD2 MSG_YESNO @"[buffer1], is it?\nThat is a decen..."
	compare LASTRESULT 0x1
	if 0x1 _goto EventScript_AIOMan_0X16B31B
	compare LASTRESULT 0x0
	if 0x1 _goto EventScript_AIOMan_0X16B2AA
	end

	@---------------
EventScript_AIOMan_0X16B2AA:
	msgbox gText_AIOMan_0X8034B1 MSG_KEEPOPEN @"I hope to see you again!"
	release
	end

	@---------------
EventScript_AIOMan_0X16B307:
	msgbox gText_AIOMan_0X193CC4 MSG_KEEPOPEN @"Now, now.\nThat is merely an Egg!"
	release
	end

	@---------------
EventScript_AIOMan_0X16B311:
	msgbox gText_AIOMan_0X193C82 MSG_KEEPOPEN @"[buffer1], is it?\nThat is a truly..."
	release
	end

	@---------------
EventScript_AIOMan_0X16B31B:
	msgbox gText_AIOMan_0X193B36 MSG_KEEPOPEN @"Ah, good. Then, what shall the new..."
	call EventScript_AIOMan_0X1A74EB
	special2 LASTRESULT 0x7B
	special 0x7C
	compare LASTRESULT 0x1
	if 0x1 _goto EventScript_AIOMan_0X16B345
	msgbox gText_AIOMan_0X193BF0 MSG_KEEPOPEN @"Done! From now on, this Pokemon\ns..."
	release
	end

	@---------------
EventScript_AIOMan_0X1A74EB:
	fadescreen 0x1
	special 0x9E
	waitstate
	return

	@---------------
EventScript_AIOMan_0X16B345:
	msgbox gText_AIOMan_0X193B66 MSG_KEEPOPEN @"Done! From now on, this Pokemon\ns..."
	release
	end


	@---------
	@ Strings
	@---------







.global EventScript_PC
EventScript_PC:
	special 0x187
	compare LASTRESULT 0x2
	if 0x1 _goto EventScript_ReleaseEnd
	lockall
	checkflag 0x841
	if 0x1 _goto EventScript_PCDisabled
	setvar 0x8004 0x1B
	special 0x17D
	setvar 0x8004 0x0
	special 0xD6
	sound 0x4
	msgbox 0x81A5075 MSG_KEEPOPEN
	goto EventScript_PCMainMenu
	end

EventScript_ReleaseEnd:
	release
	end


EventScript_PCDisabled:
	msgbox 0x81A1390 MSG_KEEPOPEN
	releaseall
	end

EventScript_PCMainMenu:
	preparemsg 0x81A508A
	waitmsg
	special 0x106
	waitstate
	goto EventScript_ChoosePCMenu
	end


EventScript_ChoosePCMenu:
	copyvar 0x8000 LASTRESULT
	compare 0x8000 0x0
	if 0x1 _goto 0x81A6A05
	compare 0x8000 0x1
	if 0x1 _goto 0x81A69F0
	compare 0x8000 0x2
	if 0x1 _goto 0x81A6A7A
	compare 0x8000 0x3
	if 0x1 _goto 0x81A6A56
	compare 0x8000 0x4
	if 0x1 _goto EventScript_TurnOffPC
	compare 0x8000 0x7F
	if 0x1 _goto EventScript_TurnOffPC
	end

EventScript_TurnOffPC:
	msgbox 0x81A1390 MSG_KEEPOPEN
	setvar 0x8004 0
	playse 0x3
	special 0xD7
	special 0x190
	releaseall
	end

EventScript_CableClub_WirelessTrade:
	msgbox gText_LinkNurse_NoTradingUseUnboundCloud MSG_FACE
	goto EventScript_CableClub_AbortLink
