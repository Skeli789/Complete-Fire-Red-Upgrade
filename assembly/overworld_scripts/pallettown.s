.thumb
.align 2

.include "../xse_commands.s"
.include "../xse_defines.s"
.include "../asm_defines.s"

.equ FLAG_HARD_MODE, 0x997 @@WIP
.equ FLAG_AUTO_HMS, 0x998
.equ FLAG_GOT_CHARMS, 0x99E
.equ FLAG_GOT_DARKRAI, 0x1016

.global EventScript_PalletTown_MapScript

gMapScripts_PalletTown_SecondFloor:
    mapscript MAP_SCRIPT_ON_TRANSITION, EventScript_PalletTown_MapScript
    .byte MAP_SCRIPT_TERMIN

EventScript_PalletTown_MapScript:
	checkitem ITEM_EXP_SHARE 0x1
	if 0x0 _call EventScript_PalletTown_GiveStartItems
	compare 0x4056 0x0
	if 0x1 _call EventScript_PalletTown_MapScriptPt2
	end

EventScript_PalletTown_GiveStartItems:
	setflag 0x918
	setflag 0x927
	setflag 0x951
	setflag 0x95A
	setflag 0x95C
	setflag 0x95F
	setflag 0x961
	setflag 0x963
	setflag 0x969
	setflag FLAG_AUTO_HMS
	additem ITEM_EXP_SHARE 0x1
	additem ITEM_FREE_SPACE_1 0x1
	return

EventScript_PalletTown_MapScriptPt2:
	sethealingplace 0x1
	return

.global EventScript_PalletTown_Options
EventScript_PalletTown_Options:
	lock
	setvar 0x4011 0x1
	msgbox gText_SelectRegion MSG_KEEPOPEN
	setvar 0x8000 0x2
	setvar 0x8001 0x6
	setvar 0x8004 0x0
	special 0x158
	waitstate
	closeonkeypress
	compare LASTRESULT 0x8 @Num Regions
	if greaterorequal _goto EventScript_PalletTown_Options
	compare LASTRESULT 0x0
	if 0x1 _goto EventScript_PalletTown_Kanto
	compare LASTRESULT 0x1
	if 0x1 _goto EventScript_PalletTown_Johto
	compare LASTRESULT 0x2
	if 0x1 _goto EventScript_PalletTown_Hoenn
	compare LASTRESULT 0x3
	if 0x1 _goto EventScript_PalletTown_Sinnoh
	compare LASTRESULT 0x4
	if 0x1 _goto EventScript_PalletTown_Unova
	compare LASTRESULT 0x5
	if 0x1 _goto EventScript_PalletTown_Kalos
	compare LASTRESULT 0x6
	if 0x1 _goto EventScript_PalletTown_Alola
	compare LASTRESULT 0x7
	if 0x1 _goto EventScript_PalletTown_Galar
	release
	end

EventScript_PalletTown_Kanto:
	sound 0x2
	msgbox gText_KantoSelected MSG_KEEPOPEN
	closeonkeypress
	setflag 0x945
	goto EventScript_PalletTown_AskNoGrindingIV

EventScript_PalletTown_Johto:
	sound 0x2
	msgbox gText_JohtoSelected MSG_KEEPOPEN
	closeonkeypress
	setflag 0x946
	goto EventScript_PalletTown_AskNoGrindingIV

EventScript_PalletTown_Hoenn:
	sound 0x2
	msgbox gText_HoennSelected MSG_KEEPOPEN
	closeonkeypress
	setflag 0x947
	goto EventScript_PalletTown_AskNoGrindingIV

EventScript_PalletTown_Sinnoh:
	sound 0x2
	msgbox gText_SinnohSelected MSG_KEEPOPEN
	closeonkeypress
	setflag 0x948
	goto EventScript_PalletTown_AskNoGrindingIV

EventScript_PalletTown_Unova:
	sound 0x2
	msgbox gText_UnovaSelected MSG_KEEPOPEN
	closeonkeypress
	setflag 0x949
	goto EventScript_PalletTown_AskNoGrindingIV

EventScript_PalletTown_Kalos:
	sound 0x2
	msgbox gText_KalosSelected MSG_KEEPOPEN
	closeonkeypress
	setflag 0x94A
	goto EventScript_PalletTown_AskNoGrindingIV

EventScript_PalletTown_Alola:
	sound 0x2
	msgbox gText_AlolaSelected MSG_KEEPOPEN
	closeonkeypress
	setflag 0x94B
	goto EventScript_PalletTown_AskNoGrindingIV

EventScript_PalletTown_Galar:
	sound 0x2
	msgbox gText_GalarSelected MSG_KEEPOPEN
	closeonkeypress
	setflag 0x94C
	goto EventScript_PalletTown_AskNoGrindingIV

EventScript_PalletTown_AskNoGrindingIV:
	msgbox gText_AskNoGrindingIV MSG_YESNO
	compare LASTRESULT 0x1
	if 0x1 _goto EventScript_PalletTown_NoGrindingYes
	sound 0x3
	goto EventScript_PalletTown_AskNoGrindingEV

EventScript_PalletTown_NoGrindingYes:
	sound 0x2
	msgbox gText_AskNoGrindingIVYes MSG_KEEPOPEN
	closeonkeypress
	setflag 0x944
	goto EventScript_PalletTown_AskNoGrindingEV

EventScript_PalletTown_AskNoGrindingEV:
	msgbox gText_AskNoGrindingEV MSG_YESNO
	compare LASTRESULT 0x1
	if 0x1 _goto EventScript_PalletTown_NoGrindingEVYes
	sound 0x3
	goto EventScript_PalletTown_AskHardMode
	release
	end

EventScript_PalletTown_NoGrindingEVYes:
	sound 0x2
	msgbox gText_AskNoGrindingEVYes MSG_KEEPOPEN
	closeonkeypress
	setflag 0x94D
	goto EventScript_PalletTown_AskHardMode
	release
	end

EventScript_PalletTown_AskHardMode:
	msgbox gText_AskHardMode MSG_YESNO
	compare LASTRESULT 0x1
	if 0x1 _goto EventScript_PalletTown_EnableHardMode
	sound 0x3
	goto EventScript_PalletTown_AskDynamax
	release
	end

EventScript_PalletTown_EnableHardMode:
	sound 0x2
	msgbox gText_HardModeEnabled MSG_KEEPOPEN
	closeonkeypress
	setflag FLAG_HARD_MODE
	goto EventScript_PalletTown_AskDynamax
	release
	end

EventScript_PalletTown_AskDynamax:
	msgbox gText_DynamaxAsk MSG_YESNO
	compare LASTRESULT 0x1
	if 0x1 _goto EventScript_PalletTown_EnableDynamax
	setflag 0x101C
	sound 0x3
	release
	end

EventScript_PalletTown_EnableDynamax:
	sound 0x2
	msgbox gText_DynamaxEnabled MSG_KEEPOPEN
	clearflag 0x101C
	release
	end

.global EventScript_ProfOak_0X169595
EventScript_ProfOak_0X169595:
	lock
	setvar 0x8000 0x0
	faceplayer
	checkflag 0x2
	if 0x1 _goto EventScript_ProfOak_0X169600
	compare 0x4055 0x9
	if 0x1 _goto EventScript_ProfOak_0X169903
	compare 0x4055 0x8
	if 0x1 _goto EventScript_ProfOak_0X169A6E
	checkflag 0x82C
	if 0x1 _goto EventScript_ProfOak_0X1699CE
	compare 0x4052 0x1
	if 0x1 _goto EventScript_ProfOak_0X169903
	compare 0x4055 0x6
	if 0x1 _goto EventScript_ProfOak_0X1698D6
	compare 0x4057 0x1
	if 0x4 _goto EventScript_ProfOak_0X16961E
	compare 0x4055 0x4
	if 0x1 _goto EventScript_ProfOak_0X169614
	compare 0x4055 0x3
	if 0x1 _goto EventScript_ProfOak_0X16960A
	msgbox gText_ProfOak_0X18E116 MSG_KEEPOPEN @"Oak: Now, [player].\pInside those ..."
	release
	end

.equ RANGE_START, 1
.equ RANGE_END, 1200

.global GiveAllMons
GiveAllMons:
    setvar 0x8002 RANGE_START
    goto give_all_items
    return

give_all_items:
    givepokemon 0x8002 100 0x0 0x0 0x0
    addvar 0x8002 1
    compare 0x8002 RANGE_END
    if equal _goto finish_giving
    @compare 0x800D TRUE
    @if equal _goto finish_giving
    goto give_all_items
    end

finish_giving:
    end

	@---------------
EventScript_ProfOak_0X169600:
msgbox gText_ProfOak_0X1A72AD MSG_KEEPOPEN @"Thank you, [player]!\nSincerely, t..."
	release
	end

	@---------------
EventScript_ProfOak_0X169903:
	call EventScript_ProfOak_0X1A737B
	checkflag 0x2F4
	if 0x1 _goto EventScript_ProfOak_0X169913
	release
	end

	@---------------
EventScript_ProfOak_0X169A6E:
	msgbox gText_ProfOak_0X18E871 MSG_KEEPOPEN @"Pok�mon around the world wait for\..."
	release
	end

	@---------------
EventScript_ProfOak_0X1699CE:
	call EventScript_ProfOak_0X1A737B
	closeonkeypress
	compare 0x8009 0x3C
	if 0x0 _goto EventScript_ProfOak_0X1699F9
	checkflag 0x89B
	if 0x0 _goto EventScript_ProfOak_0X1699F9
	pause 0x1E
	msgbox gText_ProfOak_0X18EDF5 MSG_KEEPOPEN @"Ah, now this is excellent!\p[playe..."
	goto EventScript_ProfOak_0X169035

	@---------------
EventScript_ProfOak_0X1698D6:
	setvar 0x8004 0x0
	special2 LASTRESULT 0xD4
	copyvar 0x8008 0x8005
	copyvar 0x8009 0x8006
	buffernumber 0x0 0x8008
	buffernumber 0x1 0x8009
	compare 0x8009 0x1
	if 0x1 _goto EventScript_ProfOak_0X1699FB
	goto EventScript_ProfOak_0X169903

	@---------------
EventScript_ProfOak_0X16961E:
	msgbox gText_ProfOak_0X18E405 MSG_KEEPOPEN @"Oak: Oh, [player]!\nHow is my old ..."
	textcolor 0x3
	fanfare 0x105
	preparemsg gText_ProfOak_0X18E4AF @"[player] delivered Oak@s Parcel."
	waitmsg
	waitfanfare
	call EventScript_ProfOak_0X1A6675
	removeitem 0x15D 0x1
	msgbox gText_ProfOak_0X18E4CA MSG_KEEPOPEN @"Ah! \nIt@s the custom Pok� Ball!\p..."
	playsong 0x13B 0x0
	msgbox gText_ProfOak_0X18DE8D MSG_KEEPOPEN
	closeonkeypress
	compare PLAYERFACING 0x2
	if 0x1 _call EventScript_ProfOak_0X169A82
	compare PLAYERFACING 0x1
	if 0x1 _call EventScript_ProfOak_0X169AC1
	compare PLAYERFACING 0x4
	if 0x1 _call EventScript_ProfOak_0X169A9E
	compare PLAYERFACING 0x3
	if 0x1 _call EventScript_ProfOak_0X169A9E
	fadedefault
	msgbox gText_ProfOak_0X18DE99 MSG_KEEPOPEN @"[rival]: I almost forgot!\nWhat di..."
	closeonkeypress
	pause 0x1E
	sound 0x15
	applymovement 0x4 EventScript_ProfOak_0X1A75DB
	waitmovement 0x0
	applymovement 0x4 EventScript_ProfOak_0X1A75DD
	waitmovement 0x0
	compare PLAYERFACING 0x1
	if 0x1 _call EventScript_ProfOak_0X169ADD
	compare PLAYERFACING 0x4
	if 0x1 _call EventScript_ProfOak_0X169B14
	compare PLAYERFACING 0x3
	if 0x1 _call EventScript_ProfOak_0X169AF5
	compare PLAYERFACING 0x2
	if 0x1 _call EventScript_ProfOak_0X169B86
	msgbox gText_ProfOak_0X18E508 MSG_KEEPOPEN @"Oak: Oh, right!\nI have a request ..."
	closeonkeypress
	compare PLAYERFACING 0x2
	if 0x1 _call EventScript_ProfOak_0X169882
	compare PLAYERFACING 0x1
	if 0x1 _call EventScript_ProfOak_0X16988D
	compare PLAYERFACING 0x4
	if 0x1 _call EventScript_ProfOak_0X1698A6
	compare PLAYERFACING 0x3
	if 0x1 _call EventScript_ProfOak_0X1698B8
	msgbox gText_ProfOak_0X18E536 MSG_KEEPOPEN @"On the desk there is my invention,..."
	closeonkeypress
	pause 0x28
	msgbox gText_ProfOak_0X18E5C5 MSG_KEEPOPEN @"Oak: [player] and [rival].\nTake t..."
	closeonkeypress
	applymovement 0x4 EventScript_ProfOak_0X1A75E9
	waitmovement 0x0
	hidesprite 0x9
	pause 0xA
	hidesprite 0xA
	pause 0x19
	compare PLAYERFACING 0x2
	if 0x1 _call EventScript_ProfOak_0X169845
	compare PLAYERFACING 0x1
	if 0x1 _call EventScript_ProfOak_0X169850
	compare PLAYERFACING 0x4
	if 0x1 _call EventScript_ProfOak_0X16985B
	compare PLAYERFACING 0x3
	if 0x1 _call EventScript_ProfOak_0X16986D
	pause 0xA
	textcolor 0x3
	fanfare 0x13E
	preparemsg gText_ProfOak_0X18E5EA @"[player] received the Pok�dex\nfro..."
	waitmsg
	waitfanfare
	call EventScript_ProfOak_0X1A6675
	setflag 0x829
	special 0x16F
	setvar 0x407C 0x1
	msgbox gText_ProfOak_0X18E612 MSG_KEEPOPEN @"Oak: You can@t get detailed data\n..."
	additem 0x4 0x5
	loadpointer 0x0 gText_ProfOak_0X18E6B3 @"[player] received five Pok� Balls."
	additem ITEM_POKE_BALL 0x5
	msgbox gText_ProfOak_0X18E6D0 MSG_KEEPOPEN @"When a wild Pok�mon appears,\nit@s..."
	setvar 0x8004 0x0
	setvar 0x8005 0x1
	special 0x173
	msgbox gText_ProfOak_0X18E784 MSG_KEEPOPEN @"To make a complete guide on all\nt..."
	msgbox gText_ProfOak_0X18DEC8 MSG_KEEPOPEN @"[rival]: All right, Gramps!\nLeave..."
	compare PLAYERFACING 0x2
	if 0x1 _call EventScript_ProfOak_0X169B33
	compare PLAYERFACING 0x1
	if 0x1 _call EventScript_ProfOak_0X169B45
	compare PLAYERFACING 0x4
	if 0x1 _call EventScript_ProfOak_0X169B57
	compare PLAYERFACING 0x3
	if 0x1 _call EventScript_ProfOak_0X169B57
	msgbox gText_ProfOak_0X18DEF3 MSG_KEEPOPEN @"[player], I hate to say it, but yo..."
	closeonkeypress
	playsong 0x13C 0x0
	compare PLAYERFACING 0x2
	if 0x1 _call EventScript_ProfOak_0X169B69
	compare PLAYERFACING 0x1
	if 0x1 _call EventScript_ProfOak_0X169B7B
	compare PLAYERFACING 0x4
	if 0x1 _call EventScript_ProfOak_0X169B7B
	compare PLAYERFACING 0x3
	if 0x1 _call EventScript_ProfOak_0X169B7B
	hidesprite 0x8
	fadedefault
	setvar 0x4055 0x6
	setvar 0x4057 0x2
	setvar 0x4051 0x1
	setvar 0x4058 0x1
	setvar 0x4054 0x1
	release
	end

	@---------------
EventScript_ProfOak_0X169614:
	msgbox gText_ProfOak_0X18E3AD MSG_KEEPOPEN @"Oak: [player], raise your young\nP..."
	release
	end

	@---------------
EventScript_ProfOak_0X16960A:
	msgbox gText_ProfOak_0X18E330 MSG_KEEPOPEN 
	release
	end

	@---------------
EventScript_ProfOak_0X1A737B:
	special 0x187
	compare LASTRESULT 0x2
	if 0x1 _goto EventScript_ProfOak_0X1A7AE0
	special 0x188
	checkflag 0x2F4
	if 0x1 _call EventScript_ProfOak_0X1A73A4
	checkflag 0x2F4
	if 0x0 _call EventScript_ProfOak_0X1A73AD
	call EventScript_ProfOak_0X1A73E0
	return

	@---------------
EventScript_ProfOak_0X169913:
	closeonkeypress
	pause 0x28
	preparemsg gText_ProfOak_0X1A7291 @"Wroooooooaaaaaarrrr!"
	waitmsg
	compare PLAYERFACING 0x2
	if 0x1 _call EventScript_ProfOak_0X169964
	compare PLAYERFACING 0x1
	if 0x1 _call EventScript_ProfOak_0X16996F
	compare PLAYERFACING 0x4
	if 0x1 _call EventScript_ProfOak_0X16997A
	compare PLAYERFACING 0x3
	if 0x1 _call EventScript_ProfOak_0X169985
	applymovement 0x4 EventScript_ProfOak_0X1A75E1
	waitmovement 0x0
	closeonkeypress
	pause 0x46
	msgbox gText_ProfOak_0X1A72AD MSG_KEEPOPEN @"Thank you, [player]!\nSincerely, t..."
	setflag 0x2
	release
	end

	@---------------
EventScript_ProfOak_0X1699F9:
	release
	end

	@---------------
EventScript_ProfOak_0X169035:
	msgbox gText_ProfOak_0X18EE60 MSG_KEEPOPEN @"Recently, there have been sighting..."
	closeonkeypress
	compare PLAYERFACING 0x2
	if 0x1 _call EventScript_ProfOak_0X169174
	compare PLAYERFACING 0x1
	if 0x1 _call EventScript_ProfOak_0X1691BB
	compare PLAYERFACING 0x4
	if 0x1 _call EventScript_ProfOak_0X169194
	compare PLAYERFACING 0x3
	if 0x1 _call EventScript_ProfOak_0X169194
	msgbox gText_ProfOak_0X18EF59 MSG_KEEPOPEN @"[rival]: Hey, I heard that!\pGramp..."
	fadedefault
	msgbox gText_ProfOak_0X18F004 MSG_KEEPOPEN @"Oak: I know, I know.\nOf course I ..."
	compare PLAYERFACING 0x2
	if 0x1 _call EventScript_ProfOak_0X1691DB
	compare PLAYERFACING 0x3
	if 0x1 _call EventScript_ProfOak_0X1691E6
	textcolor 0x3
	msgbox gText_ProfOak_0X18F062 MSG_KEEPOPEN @"Prof. Oak took both Pok�dex\nunits..."
	closeonkeypress
	call EventScript_ProfOak_0X1A6675
	compare PLAYERFACING 0x2
	if 0x1 _call EventScript_ProfOak_0X1691F1
	compare PLAYERFACING 0x1
	if 0x1 _call EventScript_ProfOak_0X1691FC
	compare PLAYERFACING 0x4
	if 0x1 _call EventScript_ProfOak_0X169215
	compare PLAYERFACING 0x3
	if 0x1 _call EventScript_ProfOak_0X169227
	showsprite 0x9
	showsprite 0xA
	pause 0x1E
	msgbox gText_ProfOak_0X18F085 MSG_KEEPOPEN @"[.] [.] [.] [.] [.] [.]\p[.] [.] ..."
	closeonkeypress
	hidesprite 0x9
	hidesprite 0xA
	pause 0x1E
	compare PLAYERFACING 0x2
	if 0x1 _call EventScript_ProfOak_0X169845
	compare PLAYERFACING 0x1
	if 0x1 _call EventScript_ProfOak_0X169850
	compare PLAYERFACING 0x4
	if 0x1 _call EventScript_ProfOak_0X16985B
	compare PLAYERFACING 0x3
	if 0x1 _call EventScript_ProfOak_0X16986D
	textcolor 0x3
	fanfare 0x13E
	preparemsg gText_ProfOak_0X18F0E8 @"[player]@s Pok�dex was upgraded!"
	waitmsg
	waitfanfare
	call EventScript_ProfOak_0X1A6675
	special 0x16F
	msgbox gText_ProfOak_0X18F103 MSG_KEEPOPEN @"Now, [player] and [rival]!\pThis t..."
	msgbox gText_ProfOak_0X18F1DB MSG_KEEPOPEN @"[rival]: Gramps, calm down.\nDon@t..."
	closeonkeypress
	playsong 0x13C 0x0
	compare PLAYERFACING 0x2
	if 0x1 _call EventScript_ProfOak_0X169B69
	compare PLAYERFACING 0x1
	if 0x1 _call EventScript_ProfOak_0X169B7B
	compare PLAYERFACING 0x4
	if 0x1 _call EventScript_ProfOak_0X169B7B
	compare PLAYERFACING 0x3
	if 0x1 _call EventScript_ProfOak_0X169B7B
	hidesprite 0x8
	fadedefault
	setvar 0x4055 0x8
	releaseall
	end

	@---------------
EventScript_ProfOak_0X1699FB:
	special 0x187
	compare LASTRESULT 0x2
	if 0x1 _goto EventScript_ProfOak_0X1A7AE0
	special 0x188
	checkflag 0x24F
	if 0x1 _goto EventScript_ProfOak_0X169A6E
	checkflag 0x247
	if 0x1 _goto EventScript_ProfOak_0X169A78
	checkitem 0x4 0x1
	compare LASTRESULT 0x0
	if 0x1 _goto EventScript_ProfOak_0X169A34
	goto EventScript_ProfOak_0X169A6E

	@---------------
EventScript_ProfOak_0X1A6675:
	copyvar 0x8012 0x8013
	return

	@---------------
EventScript_ProfOak_0X169A82:
	movesprite2 0x8 0x5 0xA
	showsprite 0x8
	applymovement 0xFF EventScript_ProfOak_0X169B94
	applymovement 0x8 EventScript_ProfOak_0X169B9D
	waitmovement 0x0
	return

	@---------------
EventScript_ProfOak_0X169AC1:
	movesprite2 0x8 0x6 0xA
	showsprite 0x8
	applymovement 0x4 EventScript_ProfOak_0X1A75ED
	applymovement 0x8 EventScript_ProfOak_0X169B9D
	waitmovement 0x0
	return

	@---------------
EventScript_ProfOak_0X169A9E:
	movesprite2 0x8 0x6 0xA
	showsprite 0x8
	applymovement 0x4 EventScript_ProfOak_0X1A75ED
	applymovement 0xFF EventScript_ProfOak_0X169B91
	applymovement 0x8 EventScript_ProfOak_0X169B9D
	waitmovement 0x0
	return

	@---------------
EventScript_ProfOak_0X169ADD:
	applymovement 0x4 EventScript_ProfOak_0X1A75E1
	waitmovement 0x0
	pause 0xF
	applymovement 0x4 EventScript_ProfOak_0X1A75ED
	waitmovement 0x0
	return

	@---------------
EventScript_ProfOak_0X169B14:
	applymovement 0x4 EventScript_ProfOak_0X1A75E1
	waitmovement 0x0
	pause 0xF
	applymovement 0xFF EventScript_ProfOak_0X1A75EB
	applymovement 0x4 EventScript_ProfOak_0X1A75ED
	waitmovement 0x0
	return

	@---------------
EventScript_ProfOak_0X169AF5:
	applymovement 0x4 EventScript_ProfOak_0X1A75E1
	waitmovement 0x0
	pause 0xF
	applymovement 0xFF EventScript_ProfOak_0X1A75E7
	applymovement 0x4 EventScript_ProfOak_0X1A75ED
	waitmovement 0x0
	return

	@---------------
EventScript_ProfOak_0X169B86:
	applymovement 0xFF EventScript_ProfOak_0X1A75E9
	waitmovement 0x0
	return

	@---------------
EventScript_ProfOak_0X169882:
	applymovement 0x4 EventScript_ProfOak_0X1698CB
	waitmovement 0x0
	return

	@---------------
EventScript_ProfOak_0X16988D:
	applymovement 0x4 EventScript_ProfOak_0X1698D0
	applymovement 0xFF EventScript_ProfOak_0X1698C7
	applymovement 0x8 EventScript_ProfOak_0X1698C7
	waitmovement 0x4
	return

	@---------------
EventScript_ProfOak_0X1698A6:
	applymovement 0x4 EventScript_ProfOak_0X1698CB
	applymovement 0xFF EventScript_ProfOak_0X1698C3
	waitmovement 0x4
	return

	@---------------
EventScript_ProfOak_0X1698B8:
	applymovement 0x4 EventScript_ProfOak_0X1698CB
	waitmovement 0x0
	return

	@---------------
EventScript_ProfOak_0X169845:
	applymovement 0x4 EventScript_ProfOak_0X16987C
	waitmovement 0x0
	return

	@---------------
EventScript_ProfOak_0X169850:
	applymovement 0x4 EventScript_ProfOak_0X16987F
	waitmovement 0x0
	return

	@---------------
EventScript_ProfOak_0X16985B:
	applymovement 0x4 EventScript_ProfOak_0X16987C
	applymovement 0xFF EventScript_ProfOak_0X169878
	waitmovement 0x0
	return

	@---------------
EventScript_ProfOak_0X16986D:
	applymovement 0x4 EventScript_ProfOak_0X16987C
	waitmovement 0x0
	return

	@---------------
EventScript_ProfOak_0X169B33:
	applymovement 0x8 EventScript_ProfOak_0X1A75E1
	applymovement 0xFF EventScript_ProfOak_0X1A75E7
	waitmovement 0x0
	return

	@---------------
EventScript_ProfOak_0X169B45:
	applymovement 0x8 EventScript_ProfOak_0X1A75E9
	applymovement 0xFF EventScript_ProfOak_0X1A75ED
	waitmovement 0x0
	return

	@---------------
EventScript_ProfOak_0X169B57:
	applymovement 0x8 EventScript_ProfOak_0X1A75E1
	applymovement 0xFF EventScript_ProfOak_0X1A75ED
	waitmovement 0x0
	return

	@---------------
EventScript_ProfOak_0X169B69:
	applymovement 0xFF EventScript_ProfOak_0X169B91
	applymovement 0x8 EventScript_ProfOak_0X169BA4
	waitmovement 0x0
	return

	@---------------
EventScript_ProfOak_0X169B7B:
	applymovement 0x8 EventScript_ProfOak_0X169BA4
	waitmovement 0x0
	return

	@---------------
EventScript_ProfOak_0X1A7AE0:
	release
	end

	@---------------
EventScript_ProfOak_0X1A73A4:
	msgbox gText_ProfOak_0X1A72F2 MSG_KEEPOPEN @"Oak: Ah, welcome!\pTell me, how is..."
	return

	@---------------
EventScript_ProfOak_0X1A73AD:
	msgbox gText_ProfOak_0X1A6C51 MSG_KEEPOPEN @"Oak: Good to see you!\nHow is your..."
	return

	@---------------
EventScript_ProfOak_0X1A73E0:
	setvar 0x8004 0x1F
	special 0x17E
	special 0x17D
	setvar 0x8004 0x0
	special2 LASTRESULT 0xD4
	copyvar 0x8008 0x8005
	copyvar 0x8009 0x8006
	copyvar 0x800A LASTRESULT
	buffernumber 0x0 0x8008
	buffernumber 0x1 0x8009
	msgbox gText_ProfOak_0X1A6CA3 MSG_KEEPOPEN @"The amount of progress you@ve made..."
	checkflag 0x2FF
	if 0x0 _call EventScript_ProfOak_0X1A746D
	call EventScript_ProfOak_0X1A73B6
	compare 0x800A 0x0
	if 0x1 _goto EventScript_ProfOak_0X1A748F
	setvar 0x8004 0x1
	special2 LASTRESULT 0xD4
	copyvar 0x8008 0x8005
	copyvar 0x8009 0x8006
	buffernumber 0x0 0x8008
	buffernumber 0x1 0x8009
	msgbox gText_ProfOak_0X1A71AA MSG_KEEPOPEN @"And your National Pok�dex is:\p[bu..."
	special2 LASTRESULT 0x1B0
	compare LASTRESULT 0x0
	if 0x1 _goto EventScript_ProfOak_0X1A7470
	compare LASTRESULT 0x1
	if 0x1 _goto EventScript_ProfOak_0X1A747E
	end

	@---------------
EventScript_ProfOak_0X169964:
	applymovement 0x4 EventScript_ProfOak_0X169990
	waitmovement 0x0
	return

	@---------------
EventScript_ProfOak_0X16996F:
	applymovement 0x4 EventScript_ProfOak_0X169997
	waitmovement 0x0
	return

	@---------------
EventScript_ProfOak_0X16997A:
	applymovement 0x4 EventScript_ProfOak_0X16999E
	waitmovement 0x0
	return

	@---------------
EventScript_ProfOak_0X169985:
	applymovement 0x4 EventScript_ProfOak_0X1699A5
	waitmovement 0x0
	return

	@---------------
EventScript_ProfOak_0X169174:
	movesprite2 0x8 0x5 0xA
	showsprite 0x8
	playsong 0x13B 0x0
	applymovement 0xFF EventScript_ProfOak_0X169B94
	applymovement 0x8 EventScript_ProfOak_0X169B9D
	waitmovement 0x0
	return

	@---------------
EventScript_ProfOak_0X1691BB:
	movesprite2 0x8 0x6 0xA
	showsprite 0x8
	playsong 0x13B 0x0
	applymovement 0x4 EventScript_ProfOak_0X1A75ED
	applymovement 0x8 EventScript_ProfOak_0X169B9D
	waitmovement 0x0
	return

	@---------------
EventScript_ProfOak_0X169194:
	movesprite2 0x8 0x6 0xA
	showsprite 0x8
	playsong 0x13B 0x0
	applymovement 0x4 EventScript_ProfOak_0X1A75ED
	applymovement 0xFF EventScript_ProfOak_0X169B91
	applymovement 0x8 EventScript_ProfOak_0X169B9D
	waitmovement 0x0
	return

	@---------------
EventScript_ProfOak_0X1691DB:
	applymovement 0xFF EventScript_ProfOak_0X1A75E9
	waitmovement 0x0
	return

	@---------------
EventScript_ProfOak_0X1691E6:
	applymovement 0xFF EventScript_ProfOak_0X1A75E7
	waitmovement 0x0
	return

	@---------------
EventScript_ProfOak_0X1691F1:
	applymovement 0x4 EventScript_ProfOak_0X169232
	waitmovement 0x0
	return

	@---------------
EventScript_ProfOak_0X1691FC:
	applymovement 0x4 EventScript_ProfOak_0X169238
	applymovement 0xFF EventScript_ProfOak_0X1698C7
	applymovement 0x8 EventScript_ProfOak_0X1698C7
	waitmovement 0x4
	return

	@---------------
EventScript_ProfOak_0X169215:
	applymovement 0x4 EventScript_ProfOak_0X169232
	applymovement 0xFF EventScript_ProfOak_0X1698C3
	waitmovement 0x4
	return

	@---------------
EventScript_ProfOak_0X169227:
	applymovement 0x4 EventScript_ProfOak_0X169232
	waitmovement 0x0
	return

	@---------------
EventScript_ProfOak_0X169A78:
	msgbox gText_ProfOak_0X18E98E MSG_KEEPOPEN @"Oak: Come see me sometime.\pAfter ..."
	release
	end

	@---------------
EventScript_ProfOak_0X169A34:
	compare 0x4054 0x2
	if 0x4 _goto EventScript_ProfOak_0X169A45
	goto EventScript_ProfOak_0X169A6E

	@---------------
EventScript_ProfOak_0X1A746D:
	textcolor 0x0
	return

	@---------------
EventScript_ProfOak_0X1A73B6:
	copyvar 0x8004 0x8009
	special 0xD5
	waitmsg
	compare LASTRESULT 0x0
	if 0x1 _call EventScript_ProfOak_0X1A73D8
	compare LASTRESULT 0x1
	if 0x1 _call EventScript_ProfOak_0X1A73DC
	waitfanfare
	waitkeypress
	return

	@---------------
EventScript_ProfOak_0X1A748F:
	special 0x17F
	return

	@---------------
EventScript_ProfOak_0X1A7470:
	msgbox gText_ProfOak_0X1A71EE MSG_KEEPOPEN @"I@ll be looking forward to seeing\..."
	goto EventScript_ProfOak_0X1A748F

	@---------------
EventScript_ProfOak_0X1A747E:
	setflag 0x2F4
	msgbox gText_ProfOak_0X1A722F MSG_KEEPOPEN @"Finally[.]\pYou@ve finally complet..."
	goto EventScript_ProfOak_0X1A748F

	@---------------
EventScript_ProfOak_0X169A45:
	msgbox gText_ProfOak_0X18E89C MSG_KEEPOPEN @"Ah, [player]!\nHow is your Pok�dex..."
	additem 0x4 0x5
	loadpointer 0x0 gText_ProfOak_0X18E6B3 @"[player] received five Pok� Balls."
	additem ITEM_POKE_BALL 0x5
	setflag 0x247
	release
	end

	@---------------
EventScript_ProfOak_0X1A73D8:
	fanfare 0x13D
	return

	@---------------
EventScript_ProfOak_0X1A73DC:
	fanfare 0x103
	return

	@-----------
	@ Movements
	@-----------
EventScript_ProfOak_0X1A75DB:
.byte 0x62
.byte 0xFE

EventScript_ProfOak_0X1A75DD:
.byte 0x1C
.byte 0x1C
.byte 0x1C
.byte 0xFE

EventScript_ProfOak_0X1A75E9:
.byte 0x2E
.byte 0xFE

EventScript_ProfOak_0X1A75E1:
.byte 0x4A
.byte 0xFE

EventScript_ProfOak_0X169B94:
.byte 0x2D
.byte 0x1C
.byte 0x1C
.byte 0x1C
.byte 0x1C
.byte 0x1C
.byte 0x1B
.byte 0x2F
.byte 0xFE

EventScript_ProfOak_0X169B9D:
.byte 0x11
.byte 0x11
.byte 0x11
.byte 0x11
.byte 0x11
.byte 0x11
.byte 0xFE

EventScript_ProfOak_0X1A75ED:
.byte 0x2D
.byte 0xFE

EventScript_ProfOak_0X169B91:
.byte 0x1A
.byte 0x2D
.byte 0xFE

EventScript_ProfOak_0X1A75EB:
.byte 0x30
.byte 0xFE

EventScript_ProfOak_0X1A75E7:
.byte 0x2F
.byte 0xFE

EventScript_ProfOak_0X1698CB:
.byte 0x11
.byte 0x12
.byte 0x1C
.byte 0x2D
.byte 0xFE

EventScript_ProfOak_0X1698D0:
.byte 0x12
.byte 0x12
.byte 0x11
.byte 0x1C
.byte 0x30
.byte 0xFE

EventScript_ProfOak_0X1698C7:
.byte 0x1C
.byte 0x1B
.byte 0x2F
.byte 0xFE

EventScript_ProfOak_0X1698C3:
.byte 0x1C
.byte 0x1B
.byte 0x2E
.byte 0xFE

EventScript_ProfOak_0X16987C:
.byte 0x13
.byte 0x10
.byte 0xFE

EventScript_ProfOak_0X16987F:
.byte 0x10
.byte 0x13
.byte 0xFE

EventScript_ProfOak_0X169878:
.byte 0x1C
.byte 0x1B
.byte 0x30
.byte 0xFE

EventScript_ProfOak_0X169BA4:
.byte 0x10
.byte 0x10
.byte 0x10
.byte 0x10
.byte 0x10
.byte 0x10
.byte 0xFE

EventScript_ProfOak_0X169990:
.byte 0x25
.byte 0x29
.byte 0x25
.byte 0x25
.byte 0x29
.byte 0x25
.byte 0xFE

EventScript_ProfOak_0X169997:
.byte 0x26
.byte 0x2A
.byte 0x26
.byte 0x26
.byte 0x2A
.byte 0x26
.byte 0xFE

EventScript_ProfOak_0X16999E:
.byte 0x27
.byte 0x2B
.byte 0x27
.byte 0x27
.byte 0x2B
.byte 0x27
.byte 0xFE

EventScript_ProfOak_0X1699A5:
.byte 0x28
.byte 0x2C
.byte 0x28
.byte 0x28
.byte 0x2C
.byte 0x28
.byte 0xFE

EventScript_ProfOak_0X169232:
.byte 0x11
.byte 0x12
.byte 0x2E
.byte 0x1C
.byte 0x1A
.byte 0xFE

EventScript_ProfOak_0X169238:
.byte 0x12
.byte 0x12
.byte 0x11
.byte 0x1C
.byte 0x1A
.byte 0xFE

.global EventScript_ShowdownPC
EventScript_ShowdownPC:
	lock
	checkflag 0x82C
	if NOT_SET _goto EventScript_JustAPC
	release
	end

EventScript_JustAPC:
	msgbox gText_ShowdownPCOff MSG_NORMAL
	release
	end

.global EventScript_Mum
EventScript_Mum:
	lock
	faceplayer
	checkflag FLAG_GOT_CHARMS
	if SET _goto 0x8168C4A
	checkflag 0x827
	if SET _goto EventScript_GiveCharmsTellPlayerXern
	checkflag 0x258
	if 0x1 _goto 0x8168C4A
	checkgender
	compare LASTRESULT 0x0
	if 0x1 _call 0x8168C38
	compare LASTRESULT 0x1
	if 0x1 _call 0x8168C41
	closeonkeypress
	applymovement 0x1 0x81A75E5
	waitmovement 0x0
	release
	end

EventScript_GiveCharmsTellPlayerXern:
	msgbox gText_MumNews MSG_FACE
	giveitem ITEM_SHINY_CHARM 0x1 MSG_OBTAIN
	giveitem ITEM_OVAL_CHARM 0x1 MSG_OBTAIN
	giveitem 733 0x1 MSG_OBTAIN
	msgbox gText_MumStaySafe MSG_FACE
	setflag FLAG_GOT_CHARMS
	release
	end
	
.global EventScript_HealOrDarkrai
EventScript_HealOrDarkrai:
	lock
	checkflag 0x827
	if SET _goto EventScript_CheckDarkrai
	goto EventScript_HealInBed
	release
	end

EventScript_CheckDarkrai:
	checkflag FLAG_GOT_DARKRAI
	if SET _goto EventScript_HealInBed
	fadescreen FADEOUT_BLACK
	msgbox gText_PlayerFellASleepDarkrai MSG_KEEPOPEN
	warpmuted 43 19 0xFF 0x13 0x15
	fadescreen 0x0
	release
	end

EventScript_HealInBed:
	fadescreen 0x1
	special 0x0
	fanfare 256
	waitfanfare
	msgbox gText_PlayerFellAsleep MSG_KEEPOPEN
	fadescreen 0x0
	release
	end
