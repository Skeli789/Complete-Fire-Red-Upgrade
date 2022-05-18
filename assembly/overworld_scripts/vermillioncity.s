.thumb
.align 2

.include "../xse_commands.s"
.include "../xse_defines.s"
.include "../asm_defines.s"

.equ FLAG_TICKET_CHECKER_GAVE_TICKETS, 0x979
.equ VAR_DAILY_EVENT, 0x504E
.equ SP_DAILY_EVENT, 0xA0
.equ SP_UPDATE_TIME_IN_VARS, 0xA1
.equ SP_GET_TIME_DIFFERENCE, 0xA2
.equ VAR_DAILY_EVENT_2, 0x5050
.equ FLAG_GET_DNA_SPLICER, 0x97E
.equ FLAG_GET_N_SOLARIZER, 0x97F
.equ FLAG_GET_N_LUNARIZER, 0x980
.equ FLAG_GET_DRIVES, 0x981
.equ FLAG_GET_ADAMANT_ORB, 0x982
.equ FLAG_GET_LUSTROUS_ORB, 0x983
.equ FLAG_GET_GRISEOUS_ORB, 0x984
.equ FLAG_GET_SOUL_DEW, 0x985
.equ FLAG_GET_REVEAL_GLASS, 0x986
.equ FLAG_SURGE_REMATCH_BEAT, 0x98C
.equ FLAG_BEAT_KIAWE, 0x994
.equ FLAG_TM64, 0x9C6
.equ FLAG_BEAT_BREEDER, 0x9C7
.equ FLAG_GOT_LUCARIONITE, 0x9C8

.global EventScript_VermillionBerries
EventScript_VermillionBerries:
	lock
	faceplayer
	msgbox gText_MartHelpYou MSG_KEEPOPEN
	pokemart EventScript_VermillionBerriesShop
	msgbox gText_MartBye MSG_FACE
	release
	end

.global EventScript_TicketViewer_0X166941
EventScript_TicketViewer_0X166941:
	lock
	faceplayer
	checkflag FLAG_TICKET_CHECKER_GAVE_TICKETS
	if 0x0 _call EventScript_TicketViewer_CheckBadge
	compare 0x407E 0x3
	if 0x1 _goto EventScript_TicketViewer_0X166996
	msgbox gText_TicketViewer_0X17FA22 MSG_KEEPOPEN @"Welcome to the S.S. Anne!"
	release
	end

EventScript_TicketViewer_CheckBadge:
	checkflag 0x827
	if 0x1 _call EventScript_TicketViewer_Achievements
	return

EventScript_TicketViewer_Achievements:
	msgbox gText_TicketViewer_CheckBadges MSG_KEEPOPEN
	closeonkeypress
	pause 0x20
	msgbox gText_TicketViewer_CheckAchievements MSG_KEEPOPEN
	closeonkeypress
	pause 0x40
	msgbox gText_TicketViewer_GiveTickets MSG_KEEPOPEN
	closeonkeypress
	giveitem ITEM_AURORA_TICKET 0x1 MSG_OBTAIN
	giveitem ITEM_MYSTIC_TICKET 0x1 MSG_OBTAIN
	setflag FLAG_TICKET_CHECKER_GAVE_TICKETS
	return

	@---------------
EventScript_TicketViewer_0X166996:
	setvar 0x8004 0x0
	compare 0x4076 0x5
	if 0x4 _goto EventScript_TicketViewer_0X1669BB
	checkitem ITEM_MYSTIC_TICKET 0x1
	compare LASTRESULT 0x1
	if equal _goto EventScript_TicketViewer_0X166AB1
	compare 0x4076 0x1
	if 0x4 _goto EventScript_TicketViewer_0X166B3B
	msgbox gText_TicketViewer_0X17FB10 MSG_KEEPOPEN @"The ship set sail."
	release
	end

	@---------------
EventScript_TicketViewer_0X1669BB:
	special 0x187
	compare LASTRESULT 0x2
	if 0x1 _goto EventScript_TicketViewer_0X1A7AE0
	special 0x188
	call EventScript_TicketViewer_0X166958
	compare LASTRESULT 0x1
	if 0x1 _goto EventScript_TicketViewer_0X1669FD
	call EventScript_TicketViewer_0X166977
	compare LASTRESULT 0x1
	if 0x1 _goto EventScript_TicketViewer_0X166A5F
	setvar 0x8004 0x0
	preparemsg gText_TicketViewer_0X17FBB8 @"Ah, you have a Rainbow Pass.\pWoul..."
	waitmsg
	goto EventScript_TicketViewer_0X1A911E

	@---------------
EventScript_TicketViewer_0X166B3B:
	preparemsg gText_TicketViewer_0X17FB23 @"Ah, you have a Tri-Pass.\pWould yo..."
	waitmsg
	setvar 0x8004 0x0
	multichoice 0x13 0x5 0x39 0x0
	copyvar 0x8000 LASTRESULT
	compare 0x8000 0x0
	if 0x1 _goto EventScript_TicketViewer_0X1A8FEE
	compare 0x8000 0x1
	if 0x1 _goto EventScript_TicketViewer_0X1A8FF9
	compare 0x8000 0x2
	if 0x1 _goto EventScript_TicketViewer_0X1A9004
	compare 0x8000 0x3
	if 0x1 _goto EventScript_TicketViewer_0X1A90F6
	compare 0x8000 0x7F
	if 0x1 _goto EventScript_TicketViewer_0X1A90F6
	end

	@---------------_goto EventScript_TicketViewer_0X
EventScript_TicketViewer_0X1A7AE0:
	release
	end

	@---------------
EventScript_TicketViewer_0X166958:
	checkflag 0x84A
	if 0x0 _goto EventScript_TicketViewer_0X1A77B0
	checkitem 0x172 0x1
	compare LASTRESULT 0x0
	if 0x1 _goto EventScript_TicketViewer_0X1A77B0
	goto EventScript_TicketViewer_0X1A77AA

	@---------------
EventScript_TicketViewer_0X1669FD:
	call EventScript_TicketViewer_0X166977
	compare LASTRESULT 0x1
	if 0x1 _goto EventScript_TicketViewer_0X166AB1
	checkflag 0x2F0
	if 0x0 _call EventScript_TicketViewer_0X166A53
	preparemsg gText_TicketViewer_0X17FCC1 @"Would you like to board\na Seagall..."
	waitmsg
	multichoice 0x11 0x6 0x36 0x0
	copyvar 0x8000 LASTRESULT
	compare 0x8000 0x0
	if 0x1 _goto EventScript_TicketViewer_0X1A911E
	compare 0x8000 0x1
	if 0x1 _goto EventScript_TicketViewer_0X166B0B
	compare 0x8000 0x2
	if 0x1 _goto EventScript_TicketViewer_0X1A90F6
	compare 0x8000 0x7F
	if 0x1 _goto EventScript_TicketViewer_0X1A90F6
	end

	@---------------
EventScript_TicketViewer_0X166977:
	checkflag 0x84B
	if 0x0 _goto EventScript_TicketViewer_0X1A77B0
	checkitem 0x173 0x1
	compare LASTRESULT 0x0
	if 0x1 _goto EventScript_TicketViewer_0X1A77B0
	goto EventScript_TicketViewer_0X1A77AA

	@---------------
EventScript_TicketViewer_0X166A5F:
	checkflag 0x2F1
	if 0x0 _call EventScript_TicketViewer_0X166AA5
	preparemsg gText_TicketViewer_0X17FCC1 @"Would you like to board\na Seagall..."
	waitmsg
	multichoice 0xD 0x6 0x37 0x0
	copyvar 0x8000 LASTRESULT
	compare 0x8000 0x0
	if 0x1 _goto EventScript_TicketViewer_0X1A911E
	compare 0x8000 0x1
	if 0x1 _goto EventScript_TicketViewer_0X166B23
	compare 0x8000 0x2
	if 0x1 _goto EventScript_TicketViewer_0X1A90F6
	compare 0x8000 0x7F
	if 0x1 _goto EventScript_TicketViewer_0X1A90F6
	end

	@---------------
EventScript_TicketViewer_0X1A911E:
	setvar 0x8005 0x0
	special 0x1A7
	waitstate
	special2 0x8006 0x1A8
	copyvar 0x8000 0x8006
	compare 0x8000 0x0
	if 0x1 _goto EventScript_TicketViewer_0X1A91D5
	compare 0x8000 0x1
	if 0x1 _goto EventScript_TicketViewer_0X1A91E0
	compare 0x8000 0x2
	if 0x1 _goto EventScript_TicketViewer_0X1A91EB
	compare 0x8000 0x3
	if 0x1 _goto EventScript_TicketViewer_0X1A91F6
	compare 0x8000 0x4
	if 0x1 _goto EventScript_TicketViewer_0X1A9201
	compare 0x8000 0xFE
	if 0x1 _goto EventScript_TicketViewer_0X1A917F
	compare 0x8000 0x7F
	if 0x1 _goto EventScript_TicketViewer_0X1A90F6
	end

	@---------------
EventScript_TicketViewer_0X1A8FEE:
	setvar 0x8006 0x1
	goto EventScript_TicketViewer_0X1A909E

	@---------------
EventScript_TicketViewer_0X1A8FF9:
	setvar 0x8006 0x2
	goto EventScript_TicketViewer_0X1A909E

	@---------------
EventScript_TicketViewer_0X1A9004:
	setvar 0x8006 0x3
	goto EventScript_TicketViewer_0X1A909E

	@---------------
EventScript_TicketViewer_0X1A90F6:
	special2 LASTRESULT 0x1AD
	compare LASTRESULT 0x1
	if 0x1 _goto EventScript_TicketViewer_0X1A9108
	release
	end

	@---------------
EventScript_TicketViewer_0X1A77B0:
	setvar LASTRESULT 0x0
	return

	@---------------
EventScript_TicketViewer_0X1A77AA:
	setvar LASTRESULT 0x1
	return

	@---------------
EventScript_TicketViewer_0X166AB1:
	checkflag 0x2F0
	if 0x0 _call EventScript_TicketViewer_0X166A53
	checkflag 0x2F1
	if 0x0 _call EventScript_TicketViewer_0X166AA5
	preparemsg gText_TicketViewer_0X17FCC1 @"Would you like to board\na Seagall..."
	waitmsg
	setvar 0x8006 0x0
	loadpointer 0x0 gText_NavelRock
	special 0x25
	setvar 0x8006 0x1 
	loadpointer 0x0 gText_BirthIsland
	special 0x25
	multichoice 0xD 0x5 0x20 0x0
	copyvar 0x8000 LASTRESULT
	@@compare 0x8000 0x0
	@@if 0x1 _goto EventScript_TicketViewer_0X1A911E
	compare 0x8000 0x0
	if 0x1 _goto EventScript_TicketViewer_0X166B0B
	compare 0x8000 0x1
	if 0x1 _goto EventScript_TicketViewer_0X166B23
	compare 0x8000 0x3
	if 0x1 _goto EventScript_TicketViewer_0X1A90F6
	compare 0x8000 0x7F
	if 0x1 _goto EventScript_TicketViewer_0X1A90F6
	end

	@---------------
EventScript_TicketViewer_0X166A53:
	setflag 0x2F0
	msgbox gText_TicketViewer_0X17FC00 MSG_KEEPOPEN @"Oh! That@s a MysticTicket!\nNow th..."
	return

	@---------------
EventScript_TicketViewer_0X166B0B:
	msgbox gText_TicketViewer_0X17FCEC MSG_KEEPOPEN @"Okay, everything@s in order for yo..."
	setvar 0x8004 0x0
	setvar 0x8006 0x9
	goto EventScript_TicketViewer_0X1A90DA

	@---------------
EventScript_TicketViewer_0X166AA5:
	setflag 0x2F1
	msgbox gText_TicketViewer_0X17FC5F MSG_KEEPOPEN @"Oh! That@s an AuroraTicket!\nNow t..."
	return

	@---------------
EventScript_TicketViewer_0X166B23:
	msgbox gText_TicketViewer_0X17FD5F MSG_KEEPOPEN @"Okay, everything@s in order for yo..."
	setvar 0x8004 0x0
	setvar 0x8006 0xA
	goto EventScript_TicketViewer_0X1A90DA

	@---------------
EventScript_TicketViewer_0X1A91D5:
	setvar 0x8006 0x0
	goto EventScript_TicketViewer_0X1A909E

	@---------------
EventScript_TicketViewer_0X1A91E0:
	setvar 0x8006 0x1
	goto EventScript_TicketViewer_0X1A909E

	@---------------
EventScript_TicketViewer_0X1A91EB:
	setvar 0x8006 0x2
	goto EventScript_TicketViewer_0X1A909E

	@---------------
EventScript_TicketViewer_0X1A91F6:
	setvar 0x8006 0x3
	goto EventScript_TicketViewer_0X1A909E

	@---------------
EventScript_TicketViewer_0X1A9201:
	setvar 0x8006 0x4
	goto EventScript_TicketViewer_0X1A909E

	@---------------
EventScript_TicketViewer_0X1A917F:
	setvar 0x8005 0x1
	special 0x1A7
	waitstate
	special2 0x8006 0x1A8
	copyvar 0x8000 0x8006
	compare 0x8000 0x4
	if 0x1 _goto EventScript_TicketViewer_0X1A9201
	compare 0x8000 0x5
	if 0x1 _goto EventScript_TicketViewer_0X1A920C
	compare 0x8000 0x6
	if 0x1 _goto EventScript_TicketViewer_0X1A9217
	compare 0x8000 0x7
	if 0x1 _goto EventScript_TicketViewer_0X1A9222
	compare 0x8000 0xFE
	if 0x1 _goto EventScript_TicketViewer_0X1A911E
	compare 0x8000 0x7F
	if 0x1 _goto EventScript_TicketViewer_0X1A90F6
	end

	@---------------
EventScript_TicketViewer_0X1A909E:
	special2 LASTRESULT 0x1A9
	buffernumber 0x0 LASTRESULT
	compare 0x8004 0x0
	if 0x1 _goto EventScript_TicketViewer_0X1A90BE
	compare 0x8004 0x0
	if 0x5 _goto EventScript_TicketViewer_0X1A90CC
	end

	@---------------
EventScript_TicketViewer_0X1A9108:
	closeonkeypress
	applymovement 0x6 EventScript_TicketViewer_0X1A75E5
	applymovement 0xFF EventScript_TicketViewer_0X1A911C
	waitmovement 0x0
	releaseall
	end

	@---------------
EventScript_TicketViewer_0X1A90DA:
	closeonkeypress
	pause 0x14
	special 0x187
	compare LASTRESULT 0x2
	if 0x1 _goto EventScript_TicketViewer_0X1A7AE0
	special 0x188
	fadescreen 0x1
	special 0x17B
	waitstate
	end

	@---------------
EventScript_TicketViewer_0X1A920C:
	setvar 0x8006 0x5
	goto EventScript_TicketViewer_0X1A909E

	@---------------
EventScript_TicketViewer_0X1A9217:
	setvar 0x8006 0x6
	goto EventScript_TicketViewer_0X1A909E

	@---------------
EventScript_TicketViewer_0X1A9222:
	setvar 0x8006 0x7
	goto EventScript_TicketViewer_0X1A909E

	@---------------
EventScript_TicketViewer_0X1A90BE:
	msgbox gText_TicketViewer_0X17FB67 MSG_KEEPOPEN @"Okay, everything@s in order.\pSeag..."
	goto EventScript_TicketViewer_0X1A90DA

	@---------------
EventScript_TicketViewer_0X1A90CC:
	msgbox gText_TicketViewer_0X1A641B MSG_KEEPOPEN @"All right!\nAll aboard Seagallop H..."
	goto EventScript_TicketViewer_0X1A90DA

	@-----------
	@ Movements
	@-----------
EventScript_TicketViewer_0X1A75E5:
.byte 0x5A
.byte 0xFE

EventScript_TicketViewer_0X1A911C:
.byte 0x11
.byte 0xFE

.global EventScript_MarketBargain
EventScript_MarketBargain:
	lock
	faceplayer
	setvar 0x8000 0x0
	callasm GetSpecialItems
	compare 0x8000 0x1
	if equal _call EventScript_GiveDNASplicer
	compare 0x8000 0x2
	if equal _call EventScript_GiveNSolarizer
	compare 0x8000 0x3
	if equal _call EventScript_GiveNLunarizer
	compare 0x8000 0x4
	if equal _call EventScript_GiveDrives
	compare 0x8000 0x5
	if equal _call EventScript_GiveAdamantOrb
	compare 0x8000 0x6
	if equal _call EventScript_GiveLustrousOrb
	compare 0x8000 0x7
	if equal _call EventScript_GiveGriseousOrb
	compare 0x8000 0x8
	if equal _call EventScript_GiveSoulDew
	compare 0x8000 0x9
	if equal _call EventScript_GiveRevealGlass
	compare 0x8000 0xA
	if equal _call EventScript_GiveRedOrb
	compare 0x8000 0xB
	if equal _call EventScript_GiveBlueOrb
	showmoney 0x14 0x0 0x00
	msgbox gText_ViewBargain MSG_YESNO
	compare LASTRESULT 0x1
	if equal _goto EventScript_WantIt
	msgbox gText_BargainBye MSG_FACE
	hidemoney 0x14 0x0
	release
	end

EventScript_GiveDNASplicer:
	msgbox gText_DugUpItem MSG_FACE
	giveitem ITEM_DNA_SPLICERS 0x1 MSG_OBTAIN
	callasm GetSpecialItems
	return

EventScript_GiveNSolarizer:
	msgbox gText_DugUpItem MSG_FACE
	giveitem ITEM_N_SOLARIZER 0x1 MSG_OBTAIN
	callasm GetSpecialItems
	return

EventScript_GiveNLunarizer:
	msgbox gText_DugUpItem MSG_FACE
	giveitem ITEM_N_LUNARIZER 0x1 MSG_OBTAIN
	callasm GetSpecialItems
	return

EventScript_GiveDrives:
	msgbox gText_DugUpItems MSG_FACE
	giveitem ITEM_DOUSE_DRIVE 0x1 MSG_OBTAIN
	giveitem ITEM_CHILL_DRIVE 0x1 MSG_OBTAIN
	giveitem ITEM_SHOCK_DRIVE 0x1 MSG_OBTAIN
	giveitem ITEM_BURN_DRIVE 0x1 MSG_OBTAIN
	callasm GetSpecialItems
	return

EventScript_GiveAdamantOrb:
	msgbox gText_DugUpItem MSG_FACE
	giveitem ITEM_ADAMANT_ORB 0x1 MSG_OBTAIN
	callasm GetSpecialItems
	return

EventScript_GiveLustrousOrb:
	msgbox gText_DugUpItem MSG_FACE
	giveitem ITEM_LUSTROUS_ORB 0x1 MSG_OBTAIN
	callasm GetSpecialItems
	return

EventScript_GiveGriseousOrb:
	msgbox gText_DugUpItem MSG_FACE
	giveitem ITEM_GRISEOUS_ORB 0x1 MSG_OBTAIN
	callasm GetSpecialItems
	return

EventScript_GiveSoulDew:
	msgbox gText_DugUpItem MSG_FACE
	giveitem ITEM_SOUL_DEW 0x1 MSG_OBTAIN
	callasm GetSpecialItems
	return

EventScript_GiveRevealGlass:
	msgbox gText_DugUpItem MSG_FACE
	giveitem ITEM_REVEAL_GLASS 0x1 MSG_OBTAIN
	callasm GetSpecialItems
	return

EventScript_GiveRedOrb:
	msgbox gText_DugUpItem MSG_FACE
	giveitem ITEM_RED_ORB 0x1 MSG_OBTAIN
	callasm GetSpecialItems
	return

EventScript_GiveBlueOrb:
	msgbox gText_DugUpItem MSG_FACE
	giveitem ITEM_BLUE_ORB 0x1 MSG_OBTAIN
	callasm GetSpecialItems
	return

EventScript_WantIt:
	setvar 0x8000 VAR_DAILY_EVENT
	setvar 0x8001 0x0
	special2 LASTRESULT SP_DAILY_EVENT
	compare LASTRESULT 0x0
	if equal _goto EventScript_AlreadyBoughtToday
	callasm GetBargainItem
	copyvar 0x8008 0x8004
	bufferitem 0x0 0x8004
	buffernumber 0x1 0x8005
	setvar 0x8006 0x0
	special 0xE4
	msgbox gText_TodayBargain MSG_YESNO
	closeonkeypress
	special 0xE5
	showmoney 0x14 0x0 0x00
	compare LASTRESULT 0x1
	if equal _goto EventScript_CheckMoney
	msgbox gText_BargainBye MSG_FACE
	hidemoney 0x14 0x0
	release
	end

EventScript_AlreadyBoughtToday:
	msgbox gText_AlreadyBoughtBargain MSG_FACE
	hidemoney 0x14 0x0
	release
	end

EventScript_CheckMoney:
	checkmoney 3000
	compare LASTRESULT 0x1
	if lessthan _goto EventScript_NotEnoughBargain
	additem 0x8004 0x1
	removemoney 3000
	updatemoney 0x14 0x0 0x00
	fanfare 0x101
	copyvar 0x8004 0x8008
	bufferitem 0x0 0x8004
	msgbox gText_PlayerRecieved MSG_FACE
	waitfanfare
	msgbox gText_ThanksComing MSG_FACE
	setvar 0x8000 VAR_DAILY_EVENT
	setvar 0x8001 0x1
	special2 LASTRESULT SP_DAILY_EVENT
	hidemoney 0x14 0x0
	release
	end

EventScript_NotEnoughBargain:
	msgbox gText_BargainNotEnough MSG_FACE
	hidemoney 0x14 0x0
	release
	end

.global EventScript_SellBargain
EventScript_SellBargain:
	lock
	faceplayer
	setvar 0x8000 VAR_DAILY_EVENT_2
	setvar 0x8001 0x0
	special2 LASTRESULT SP_DAILY_EVENT
	compare LASTRESULT 0x0
	if equal _goto EventScript_ComeBackTomorrow
	showmoney 0x14 0x0 0x00
	callasm GetSellItem
	bufferitem 0x0 0x8004
	buffernumber 0x1 0x8008
	copyvar 0x8009 0x8004
	msgbox gText_LookingForItem MSG_FACE
	callasm HasSellItem
	compare LASTRESULT 0x1
	if equal _goto EventScript_GiveGuyItem
	hidemoney 0x14 0x0
	release
	end

EventScript_GiveGuyItem:
	msgbox gText_HaveItem MSG_YESNO
	compare LASTRESULT 0x1
	if equal _goto EventScript_RecievePayment
	msgbox gText_OhOkThen MSG_FACE
	hidemoney 0x14 0x0
	release
	end

EventScript_RecievePayment:
	msgbox gText_HeresReward MSG_FACE
	callasm GiveBargainItemMoney
	updatemoney 0x14 0x0 0x00
	copyvar 0x8004 0x8009
	bufferitem 0x0 0x8004
	msgbox gText_PlayerRecievedCash MSG_FACE
	removeitem 0x4001 0x1
	setvar 0x8000 VAR_DAILY_EVENT_2
	setvar 0x8001 0x1
	special2 LASTRESULT SP_DAILY_EVENT
	goto EventScript_ComeBackTomorrow
	hidemoney 0x14 0x0
	release
	end

EventScript_ComeBackTomorrow:
	msgbox gText_ComeBackTomorrowBargain MSG_FACE
	hidemoney 0x14 0x0
	release
	end

EventScript_Maractus:
	lock
	cry SPECIES_MARACTUS 0x0
	msgbox gText_Maractus MSG_FACE
	release
	end

.global EventScript_Surge
EventScript_Surge:
	lock
	checkflag 0x822
	if 0x1 _goto EventScript_SurgeBeat
	trainerbattle1 0x1 0x1A0 0x0 0x8194BA4 0x8194E03 0x816B97C
	release
	end

EventScript_SurgeBeat:
	checkflag 0x827
	if equal _goto EventScript_SurgeRematch
	msgbox 0x8194C8E MSG_KEEPOPEN 
	release
	end

EventScript_SurgeRematch:
	checkflag FLAG_SURGE_REMATCH_BEAT
	if 0x1 _goto EventScript_SurgeRematchDone
	msgbox gText_SurgeBeforeRematch MSG_FACE
	trainerbattle3 0x1 0xF 0x0 gText_SurgeRematchLose
	msgbox gText_SurgeRematchAfter1 MSG_FACE
	giveitem ITEM_AMPHAROSITE 0x1 MSG_OBTAIN
	giveitem ITEM_TERRAIN_EXTENDER 0x2 MSG_OBTAIN
	giveitem ITEM_MAGNET 0x3 MSG_OBTAIN
	setflag FLAG_SURGE_REMATCH_BEAT
	release
	end

EventScript_SurgeRematchDone:
	msgbox gText_SurgeRematchAfter2 MSG_FACE
	release
	end

.global EventScript_Kiawe
EventScript_Kiawe:
	lock
	msgbox gText_KiaweBefore MSG_NORMAL
	faceplayer
	applymovement 15 EventScript_KiaweFace
	sound 0x15
	msgbox gText_KiaweAskBattle MSG_YESNO
	compare LASTRESULT 0x1
	if equal _goto EventScript_KiaweBattle
	msgbox gText_KiaweNoBattle MSG_FACE
	release
	end

EventScript_KiaweBattle:
	trainerbattle3 0x1 0x20 0x0 gText_KiaweLose
	msgbox gText_KiaweAfter MSG_FACE
	giveitem ITEM_TM77 0x1 MSG_OBTAIN
	fadescreen 0x1
	setflag FLAG_BEAT_KIAWE
	hidesprite 15
	fadescreen 0x0
	release
	end

EventScript_KiaweFace:
	.byte 0x62
	.byte 0xFE

.global EventScript_SpecialItemSeller
EventScript_SpecialItemSeller:
	lock
	faceplayer
	msgbox gText_SellingArcuesPlates MSG_YESNO
	compare LASTRESULT 0x1
	if equal _goto EventScript_OpenArceusMenu
	release
	end

EventScript_NotEnoughBadges:
	msgbox gText_ArcuesPlateSellerHi MSG_FACE
	msgbox gText_ArcuesPlatesNotEnoguh MSG_FACE
	release
	end

EventScript_OpenArceusMenu:
	pokemart EventScript_ArceusPlatesSilvallyMemories
	release
	end

.global EventScript_IOATutor
EventScript_IOATutor:
	lock
	faceplayer
	msgbox gText_IOAMoves MSG_YESNO
	compare LASTRESULT 0x1
	if equal _goto EventScript_IOAMoveList
	msgbox gText_IOANo MSG_FACE
	release
	end

EventScript_IOAMoveList:
	preparemsg gText_IOAWhichMove
	waitmsg
	setvar 0x8004 0x0
	setvar 0x8000 0x11
	setvar 0x0001 6
	special 0x158
	waitstate 
	compare LASTRESULT 18
	if greaterorequal _goto EventScript_End
	switch LASTRESULT
	case 0, ExpandingForce
	case 1, SteelRoller
	case 2, ScaleShot
	case 3, MeteorBeam
	case 4, MistyExplosion
	case 5, GrassyGlide
	case 6, RisingVoltage
	case 7, TerrainPulse
	case 8, SkitterSmack
	case 9, BurningJealousy
	case 10, LashOut
	case 11, Poltergeist
	case 12, CorrosiveGas
	case 13, Coaching
	case 14, FlipTurn
	case 15, TripleAxel
	case 16, DualWingbeat
	case 17, ScorchingSands
	release
	end

EventScript_End:
	msgbox gText_NatureChangerComeBackSoon MSG_FACE
	release
	end

ExpandingForce:	
	setvar 0x8005 128
	goto EventScript_TeachMove
	release
	end

SteelRoller:	
	setvar 0x8005 129
	goto EventScript_TeachMove
	release
	end

ScaleShot:	
	setvar 0x8005 130
	goto EventScript_TeachMove
	release
	end

MeteorBeam:	
	setvar 0x8005 131
	goto EventScript_TeachMove
	release
	end

MistyExplosion:	
	setvar 0x8005 132
	goto EventScript_TeachMove
	release
	end

GrassyGlide:	
	setvar 0x8005 133
	goto EventScript_TeachMove
	release
	end

RisingVoltage:	
	setvar 0x8005 134
	goto EventScript_TeachMove
	release
	end

TerrainPulse:	
	setvar 0x8005 135
	goto EventScript_TeachMove
	release
	end

SkitterSmack:	
	setvar 0x8005 136
	goto EventScript_TeachMove
	release
	end

BurningJealousy:	
	setvar 0x8005 137
	goto EventScript_TeachMove
	release
	end

LashOut:	
	setvar 0x8005 138
	goto EventScript_TeachMove
	release
	end

Poltergeist:	
	setvar 0x8005 139
	goto EventScript_TeachMove
	release
	end

CorrosiveGas:	
	setvar 0x8005 140
	goto EventScript_TeachMove
	release
	end

Coaching:	
	setvar 0x8005 141
	goto EventScript_TeachMove
	release
	end

FlipTurn:	
	setvar 0x8005 142
	goto EventScript_TeachMove
	release
	end

TripleAxel:	
	setvar 0x8005 143
	goto EventScript_TeachMove
	release
	end

DualWingbeat:	
	setvar 0x8005 144
	goto EventScript_TeachMove
	release
	end

ScorchingSands:	
	setvar 0x8005 145
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

.global EventScript_ExplosionGuy
EventScript_ExplosionGuy:
	lock
	checkflag FLAG_TM64
	if SET _goto EventScript_ExplainTM64
	msgbox gText_ExplosionTMShow MSG_FACE
	special 0x9F
	waitstate 
    compare 0x8004 0x6
    if greaterorequal _goto EventScript_ComeBackSoonExplosion
	msgbox gText_LetMeSee MSG_FACE
	callasm HasHighAtkOrLowHP
	compare LASTRESULT 0x0
	if equal _goto EventScript_ExplosionBadStats
	buffernumber 0x0 0x8005
	buffernumber 0x1 0x8006
	msgbox gText_GoodStatsExplosion MSG_FACE
	giveitem ITEM_TM64 0x1 MSG_OBTAIN
	setflag FLAG_TM64
	goto EventScript_ExplainTM64
	release
	end

EventScript_ExplainTM64:
	msgbox gText_TM64Explain MSG_FACE
	release
	end

EventScript_ComeBackSoonExplosion:
	msgbox gText_ExplosionNoTM MSG_FACE
	release
	end

EventScript_ExplosionBadStats:
	msgbox gText_NoExplosion MSG_FACE
	release
	end

.global EventScript_RioluBreeder
EventScript_RioluBreeder:
	lock
	checkflag FLAG_GOT_LUCARIONITE
	if SET _goto EventScript_ExplainLucarionite
	checkflag 0x824
	if SET _goto EventScript_GiveMegaBreeder
	checkflag FLAG_BEAT_BREEDER
	if SET _goto EventScript_RioluBreederHasEgg
	faceplayer
	msgbox gText_PokemonBreederAsk MSG_YESNO
	compare LASTRESULT 0x1
	if equal _goto EventScript_BattleRioluBreeder
	release
	end

EventScript_BattleRioluBreeder:
	msgbox gText_BeforePokemonBreederBattle MSG_FACE	
	trainerbattle3 0x1 55 0x0 gText_PokemonBreederLose
	msgbox gText_BreederHereEgg MSG_FACE
	giveegg SPECIES_RIOLU
	msgbox gText_PlayerRecievedEgg MSG_FACE
	giveitem ITEM_TM101 0x1 MSG_OBTAIN
	msgbox gText_BreederExplain MSG_FACE
	setflag FLAG_BEAT_BREEDER
	goto EventScript_RioluBreederHasEgg
	release
	end

EventScript_RioluBreederHasEgg:
	msgbox gText_ComeBackMegaBreeder MSG_FACE
	release
	end

EventScript_GiveMegaBreeder:
	checkflag FLAG_BEAT_BREEDER
	if NOT_SET _goto EventScript_RioluBreeder2
	msgbox gText_BreederHereMega MSG_FACE
	giveitem ITEM_LUCARIONITE 0x1 MSG_OBTAIN
	setflag FLAG_GOT_LUCARIONITE
	goto EventScript_ExplainLucarionite
	release
	end

EventScript_RioluBreeder2:
	faceplayer
	msgbox gText_PokemonBreederAsk MSG_YESNO
	compare LASTRESULT 0x1
	if equal _goto EventScript_BattleRioluBreeder
	release
	end

EventScript_ExplainLucarionite:
	msgbox gText_ExplainLucarionite MSG_FACE
	release
	end

.global EventScript_RandomBattleMan
EventScript_RandomBattleMan:
	lock
	faceplayer
	msgbox gText_RandomBattleAsk MSG_YESNO
	compare LASTRESULT 0x1
	if equal _goto EventScript_BattleRandom
	release
	end

EventScript_BattleRandom:
	trainerbattle3 0x1 53 0x0 gText_BeatRandomBattle
	release
	end

.global EventScript_KnockOffTutor
EventScript_KnockOffTutor:
	lock
	faceplayer
	msgbox gText_ShallTeachKnockOff MSG_YESNO
	compare LASTRESULT 0x1
	if equal _goto EventScript_TeachKnockOff
	release
	end

EventScript_TeachKnockOff:
	setvar 0x8005 53
	special 0x18D
	waitstate
	compare LASTRESULT 0x0
	if equal _goto EventScript_ComeBackKnockOff
	msgbox gText_ThereYaGoKnockOff MSG_FACE
	release
	end

EventScript_ComeBackKnockOff:
	msgbox gText_ComeBackKnockOff MSG_FACE
	release
	end

.global EventScript_Raticate
EventScript_Raticate:
	cry SPECIES_RATICATE 0x0
	msgbox gText_Raticate MSG_FACE
	end

.global EventScript_FangMoveTutor
EventScript_FangMoveTutor:
	lock
	faceplayer
	msgbox gText_ShouldTeachFang MSG_YESNO
	compare LASTRESULT 0x1
	if equal _goto EventScript_SelectFangMove
	msgbox gText_NoFangMove MSG_FACE
	release
	end

EventScript_SelectFangMove:
	preparemsg gText_WhichFangMove
	waitmsg
	setvar 0x8004 0x0
	setvar 0x8000 0x13
	setvar 0x0001 0x6
	special 0x158
	waitstate 
	compare LASTRESULT 6
	if greaterorequal _goto EventScript_NoFangMoves
	switch LASTRESULT
	case 0, SuperFang
	case 1, ThunderFang
	case 2, IceFang
	case 3, FireFang
	case 4, Crunch
	case 5, PsychicFangs
	release
	end

EventScript_NoFangMoves:
	msgbox gText_NoFangMove MSG_FACE
	release
	end

SuperFang:
	setvar 0x8005 42
	goto EventScript_TeachMove2
	release
	end
ThunderFang:
	setvar 0x8005 88
	goto EventScript_TeachMove2
	release
	end
IceFang:
	setvar 0x8005 89
	goto EventScript_TeachMove2
	release
	end
FireFang:
	setvar 0x8005 90
	goto EventScript_TeachMove2
	release
	end
Crunch:
	setvar 0x8005 107
	goto EventScript_TeachMove2
	release
	end
PsychicFangs:
	setvar 0x8005 117
	goto EventScript_TeachMove2
	release
	end

EventScript_TeachMove2:
	special 0x18D
	waitstate
	compare LASTRESULT 0x0
	if equal _goto EventScript_NoFangMoves
	msgbox gText_ThanksForTeachingFang MSG_FACE
	release
	end

.global EventScript_TrappingMoveTutor
EventScript_TrappingMoveTutor:
	lock
	faceplayer 
	msgbox gText_ImMasterTrappingMoves MSG_YESNO
	compare LASTRESULT 0x1
	if equal _goto EventScript_TrapMoveTutor2
	msgbox gText_ComeBackAnyTrapping MSG_FACE
	release
	end

EventScript_TrapMoveTutor2:
	preparemsg gText_WhichTrappingMove
	waitmsg
	setvar 0x8004 0x0
	multichoiceoption NAME_LONG_WHIRLPOOL 0
	multichoiceoption NAME_LONG_FIRE_SPIN 1
	multichoiceoption NAME_LONG_SAND_TOMB 2
	multichoiceoption sNoThanks 3
	multichoice 0x0 0x0 FOUR_MULTICHOICE_OPTIONS 0x0
	compare LASTRESULT 5
	if greaterorequal _goto EventScript_NOTrapMove
	switch LASTRESULT
	case 0, Whirlpool
	case 1, FireSpin
	case 2, SandTomb
	release
	end

Whirlpool:
	setvar 0x8005 81
	goto TeachTutorMove2
	end

FireSpin:
	setvar 0x8005 82
	goto TeachTutorMove2
	end

SandTomb:
	setvar 0x8005 83
	goto TeachTutorMove2
	end

TeachTutorMove2:
	special 0x18D
	waitstate
	release
	end

EventScript_NOTrapMove:
	msgbox gText_ComeBackAnyTrapping MSG_FACE
	release
	end


.align 1
EventScript_VermillionBerriesShop:
	.hword ITEM_CHERI_BERRY
	.hword ITEM_CHESTO_BERRY
	.hword ITEM_PECHA_BERRY
	.hword ITEM_RAWST_BERRY
	.hword ITEM_ASPEAR_BERRY
	.hword ITEM_PERSIM_BERRY
	.hword ITEM_LUM_BERRY
	.hword ITEM_SITRUS_BERRY
	.hword ITEM_FIGY_BERRY
	.hword ITEM_WIKI_BERRY
	.hword ITEM_MAGO_BERRY
	.hword ITEM_AGUAV_BERRY
	.hword ITEM_IAPAPA_BERRY
	.hword ITEM_OCCA_BERRY 
	.hword ITEM_PASSHO_BERRY 
	.hword ITEM_WACAN_BERRY 
	.hword ITEM_RINDO_BERRY 
	.hword ITEM_YACHE_BERRY
	.hword ITEM_CHOPLE_BERRY 
	.hword ITEM_KEBIA_BERRY 
	.hword ITEM_SHUCA_BERRY 
	.hword ITEM_COBA_BERRY 
	.hword ITEM_PAYAPA_BERRY 
	.hword ITEM_TANGA_BERRY
	.hword ITEM_CHARTI_BERRY 
	.hword ITEM_KASIB_BERRY 
	.hword ITEM_HABAN_BERRY
	.hword ITEM_COLBUR_BERRY 
	.hword ITEM_BABIRI_BERRY 
	.hword ITEM_CHILAN_BERRY 
	.hword ITEM_LIECHI_BERRY
	.hword ITEM_GANLON_BERRY
	.hword ITEM_PETAYA_BERRY
	.hword ITEM_APICOT_BERRY
	.hword ITEM_SALAC_BERRY
	.hword 0x0

EventScript_ArceusPlatesSilvallyMemories:
	.hword ITEM_DRACO_PLATE
	.hword ITEM_DREAD_PLATE
	.hword ITEM_EARTH_PLATE
	.hword ITEM_FIST_PLATE
	.hword ITEM_FLAME_PLATE
	.hword ITEM_ICICLE_PLATE
	.hword ITEM_INSECT_PLATE
	.hword ITEM_IRON_PLATE
	.hword ITEM_MEADOW_PLATE
	.hword ITEM_MIND_PLATE
	.hword ITEM_PIXIE_PLATE
	.hword ITEM_SKY_PLATE
	.hword ITEM_SPLASH_PLATE
	.hword ITEM_SPOOKY_PLATE
	.hword ITEM_STONE_PLATE
	.hword ITEM_TOXIC_PLATE
	.hword ITEM_ZAP_PLATE
	.hword ITEM_BUG_MEMORY
	.hword ITEM_DARK_MEMORY
	.hword ITEM_DRAGON_MEMORY
	.hword ITEM_ELECTRIC_MEMORY
	.hword ITEM_FAIRY_MEMORY
	.hword ITEM_FIGHTING_MEMORY
	.hword ITEM_FIRE_MEMORY
	.hword ITEM_FLYING_MEMORY
	.hword ITEM_GHOST_MEMORY
	.hword ITEM_GRASS_MEMORY
	.hword ITEM_GROUND_MEMORY
	.hword ITEM_ICE_MEMORY
	.hword ITEM_POISON_MEMORY
	.hword ITEM_PSYCHIC_MEMORY
	.hword ITEM_ROCK_MEMORY
	.hword ITEM_STEEL_MEMORY
	.hword ITEM_WATER_MEMORY
	.hword 0x0