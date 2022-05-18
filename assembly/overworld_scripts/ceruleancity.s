.thumb
.align 2

.include "../xse_commands.s"
.include "../xse_defines.s"
.include "../asm_defines.s"

.equ FLAG_MISTY_REMATCH_BEAT, 0x98B
.equ FLAG_BEAT_LANA, 0x993
.equ FLAG_SLOWBRO_SPRITE_CERULEAN, 0x999
.equ FLAG_SLOWBRO_FOUND, 0x99A
.equ FLAG_HEARD_STORY_SLOWBRO, 0x99B
.equ FLAG_SLOWBRO_SPRITE_2, 0x99C
.equ FLAG_SLOWBRO_ALREADY_HAVE_MONEY, 0x99D

.global EventScript_CeruleanEgg
EventScript_CeruleanEgg:
	lock
	faceplayer
	showmoney 0x14 0x0 0x00
	msgbox gText_CeruleanEggHi MSG_YESNO
	compare LASTRESULT 0x1
	if 0x1 _goto EventScript_CeruleanEgg_CheckMoney
	msgbox gText_CeruleanEggBye MSG_FACE
	hidemoney 0x14 0x0 
	release
	end

EventScript_CeruleanEgg_CheckMoney:
	checkmoney 0x1388 
	compare LASTRESULT 0x1
	if lessthan _goto EventScript_CeruleanEgg_NotEnough
	callasm GetRandomStarter
	removemoney 0x1388 0x00
	updatemoney 0x14 0x0 0x00
	copyvar 0x4001 0x8008
	giveegg 0x4001
	fanfare 0x13E
	msgbox gText_GetEgg MSG_KEEPOPEN
	waitfanfare
	hidemoney 0x14 0x0 
	closeonkeypress
	release
	end

EventScript_CeruleanEgg_NotEnough:
	msgbox gText_CeruleanEggNoMoney MSG_FACE
	hidemoney 0x14 0x0 
	release
	end

EventScript_CeruleanPowerItems:
	lock
	faceplayer
	msgbox gText_MartHelpYou MSG_KEEPOPEN
	pokemart EventScript_CeruleanPowerItemsShop
	msgbox gText_MartBye MSG_FACE
	release
	end

.global EventScript_CeruleanTutor
EventScript_CeruleanTutor:
	lock
	faceplayer
	msgbox gText_CeruleanTutorAsk MSG_YESNO
	compare LASTRESULT 0x1 
	if equal _goto EventScript_SelectCeruleanTutor
	release
	end

EventScript_SelectCeruleanTutor:
	preparemsg gText_TutorWhichMove
	waitmsg
	setvar 0x8004 0x0
	setvar 0x8000 0x10
	setvar 0x0001 0x6
	special 0x158
	waitstate 
	compare LASTRESULT 0xA
	if greaterorequal _goto EventScript_End
	switch LASTRESULT
	case 0, Electroweb
	case 1, IcyWind
	case 2, LowKick
	case 3, Bounce
	case 4, SignalBeam
	case 5, Snore
	case 6, Covet
	case 7, WorrySeed
	case 8, HelpingHand
	case 9, IronDefense
	release
	end

Electroweb:	
	setvar 0x8005 5
	goto EventScript_TeachMove
	release
	end

IcyWind:	
	setvar 0x8005 30
	goto EventScript_TeachMove
	release
	end

LowKick:	
	setvar 0x8005 6
	goto EventScript_TeachMove
	release
	end

Bounce:	
	setvar 0x8005 23
	goto EventScript_TeachMove
	release
	end

SignalBeam:	
	setvar 0x8005 18
	goto EventScript_TeachMove
	release
	end

Snore:	
	setvar 0x8005 3
	goto EventScript_TeachMove
	release
	end

Covet:	
	setvar 0x8005 12
	goto EventScript_TeachMove
	release
	end

WorrySeed:	
	setvar 0x8005 11
	goto EventScript_TeachMove
	release
	end

HelpingHand:	
	setvar 0x8005 9
	goto EventScript_TeachMove
	release
	end

IronDefense:	
	setvar 0x8005 20
	goto EventScript_TeachMove
	release
	end

EventScript_TeachMove:
	special 0x18D
	waitstate
	compare LASTRESULT 0x0
	if equal _goto EventScript_End
	msgbox gText_MoveTaught MSG_FACE
	release
	end

EventScript_End:
	msgbox gText_NatureChangerComeBackSoon MSG_FACE
	release
	end


.global EventScript_Misty
EventScript_Misty:
	lock
	checkflag 0x821
	if 0x1 _goto EventScript_MistyBeat
	trainerbattle1 0x1 0x19F 0x0 0x81921EF 0x819242D EventScript_WonMisty
	release
	end

EventScript_WonMisty:
	setvar 0x8004 0x3
	setvar 0x8005 0x1
	special 0x173
	setflag 0x4B1
	setflag 0x821
	sethealingplace 0x4
	setvar 0x8008 0x2
	goto EventScript_MistyGiveItems

EventScript_MistyGiveItems:
	msgbox 0x81922F7 MSG_KEEPOPEN 
	additem ITEM_TM83 0x1
	setvar 0x8004 ITEM_TM83
	fanfare 0x101
	special 0xE4
	msgbox gText_RecieveScald MSG_KEEPOPEN
	closeonkeypress
	waitfanfare
	special 0xE5
	setflag 0x297
	msgbox gText_ScaldDesc MSG_KEEPOPEN
	release
	end

EventScript_MistyBeat:
	checkflag 0x827
	if equal _goto EventScript_MistyRematch
	msgbox 0x81922BF MSG_KEEPOPEN
	release
	end

EventScript_MistyRematch:
	faceplayer
	checkflag FLAG_MISTY_REMATCH_BEAT
	if 0x1 _goto EventScript_MistyRematchDone
	msgbox gText_RematchAskMisty MSG_YESNO
	compare LASTRESULT 0x1
	if equal _goto EventScript_MistyRematchDo
	release
	end

EventScript_MistyRematchDo:
	trainerbattle3 0x1 0xE 0x0 gText_MistyLoseRematch
	setflag FLAG_MISTY_REMATCH_BEAT
	msgbox gText_MistyLoseAfter1 MSG_FACE
	giveitem ITEM_SHARPEDONITE 0x1 MSG_OBTAIN
	giveitem ITEM_GYARADOSITE 0x1 MSG_OBTAIN
	giveitem ITEM_DAMP_ROCK 0x2 MSG_OBTAIN
	giveitem ITEM_MYSTIC_WATER 0x3 MSG_OBTAIN
	release
	end

EventScript_MistyRematchDone:
	msgbox gText_MistyLoseAfter2 MSG_FACE
	release
	end

.global EventScript_Lana
EventScript_Lana:
	lock
	msgbox gText_LanaBeforeBattle MSG_FACE
	trainerbattle3 0x1 0x1F 0x0 gText_LanaLose
	msgbox gText_LanaGiveItem MSG_FACE
	giveitem ITEM_MYSTIC_WATER 0x1 MSG_OBTAIN
	fadescreen 0x1
	setflag FLAG_BEAT_LANA 
	hidesprite 14
	fadescreen 0x0
	release
	end
	
.global EventScript_SideQuest_SaveSlowbro
EventScript_SideQuest_SaveSlowbroMap:
	mapscript MAP_SCRIPT_ON_LOAD EventScript_HideSlowbro
	.byte MAP_SCRIPT_TERMIN

EventScript_HideSlowbro:
	checkflag FLAG_SLOWBRO_ALREADY_HAVE_MONEY
	if SET _goto EventScript_ShowSlowbro
	setflag FLAG_SLOWBRO_SPRITE_CERULEAN
	hidesprite 2
	end

EventScript_ShowSlowbro:
	showsprite 2
	clearflag FLAG_SLOWBRO_SPRITE_CERULEAN
	end

EventScript_SlowbroQuestMan:
	lock
	faceplayer
	checkflag FLAG_SLOWBRO_FOUND
	if SET _goto EventScript_GiveMoneySlowbro
	checkflag FLAG_HEARD_STORY_SLOWBRO
	if SET _goto EventScript_DidntFindSlowbro
	msgbox gText_AskHelpSlowbro MSG_YESNO
	compare LASTRESULT 0x1
	if equal _goto EventScript_SayQuestSlowbro
	msgbox gText_NoHelpSlowbro MSG_FACE
	release
	end

EventScript_GiveMoneySlowbro:
	checkflag FLAG_SLOWBRO_ALREADY_HAVE_MONEY
	if SET _goto EventScript_CantThankEnoughSlowbro
	msgbox gText_SlowbroFound MSG_FACE
	sound 15
	showsprite 2
	clearflag FLAG_SLOWBRO_SPRITE_CERULEAN
	setflag FLAG_SLOWBRO_ALREADY_HAVE_MONEY
	addmoney 50000
	msgbox gText_Heres50000 MSG_FACE
	fanfare 0x101
	msgbox gText_50000 MSG_FACE
	waitfanfare
	release
	end

EventScript_CantThankEnoughSlowbro:
	msgbox gText_ThnaksSlowbro MSG_FACE
	release
	end

EventScript_SayQuestSlowbro:
	msgbox gText_QuestSlowbro MSG_FACE
	fanfare 0x101
	msgbox gText_RecieveBallSlowbro MSG_FACE
	waitfanfare
	setflag FLAG_HEARD_STORY_SLOWBRO
	goto EventScript_DidntFindSlowbro
	release
	end

EventScript_DidntFindSlowbro:
	msgbox gText_IllGive50000 MSG_FACE
	release
	end

EventScript_SlowbroFind:
	lock
	faceplayer
	checkflag FLAG_HEARD_STORY_SLOWBRO
	if NOT_SET _goto EventScript_JustTalkSlowbro
	msgbox gText_SlowbroTalk MSG_FACE
	sound 15
	hidesprite 15
	setflag FLAG_SLOWBRO_SPRITE_2
	setflag FLAG_SLOWBRO_FOUND
	msgbox gText_SlowbroInBall MSG_NORMAL
	release
	end

EventScript_JustTalkSlowbro:
	msgbox gText_SlowbroTalk MSG_FACE
	release
	end

.global EventScript_BehindBillHouse
EventScript_BehindBillHouse:
	lock
	msgbox gText_GoBehindBillsHouse MSG_YESNO
	compare LASTRESULT 0x1
	if equal _goto EventScript_BillWarp
	release
	end

EventScript_BillWarp:
	warpmuted 43 16 0xFF 0x7 0x1A
	release
	end

.global EventScript_trade_Start
EventScript_trade_Start:
	lock
	faceplayer
	setvar 0x8008 0x9
	call 0x1A8CAD
	checkflag 0x100A
	if 0x1 _goto EventScript_trade_How
	msgbox gText_trade_Yesno 0x5
	compare LASTRESULT 0x0
	if 0x1 _goto EventScript_trade_Too
	call 0x1A8CBD
	compare 0x8004 0x6
	if 0x4 _goto EventScript_trade_Too
	call 0x1A8CC9
	comparevars LASTRESULT 0x8009
	if 0x5 _goto EventScript_trade_Not
	call 0x1A8CD9
	msgbox gText_trade_Thanksalot 0x4
	setflag 0x100A
	release
	end

EventScript_trade_Not:
	bufferpokemon 0x0 0x8009
	msgbox gText_trade_Not2 0x4
	release
	end

EventScript_trade_Too:
	bufferpokemon 0x0 0x8009
	msgbox gText_trade_Too2 0x4
	release
	end

EventScript_trade_How:
	msgbox gText_trade_Old 0x4
	release
	end


.global EventScript_MySonOldAmber
EventScript_MySonOldAmber:
	lock
	msgbox gText_MySonOldAmber MSG_FACE
	release
	end


.align 1
EventScript_CeruleanPowerItemsShop:
	.hword ITEM_POWER_WEIGHT
	.hword ITEM_POWER_BRACER
	.hword ITEM_POWER_BELT
	.hword ITEM_POWER_LENS
	.hword ITEM_POWER_BAND
	.hword ITEM_POWER_ANKLET
	.hword ITEM_MACHO_BRACE
	.hword ITEM_DESTINY_KNOT
	.hword 0x0
