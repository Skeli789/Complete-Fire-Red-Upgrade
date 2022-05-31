.thumb
.align 2

.include "../xse_commands.s"
.include "../xse_defines.s"
.include "../asm_defines.s"

.equ FLAG_BROCK_REMATCH_BEAT, 0x98A
.equ FLAG_BEAT_ILIMA, 0x992
.equ FLAG_BEAT_STEVEN, 0x9EB

.global EventScript_Trainer_Emma
EventScript_Trainer_Emma:
	trainerbattle0 0x0 0x4 0x0 gText_EmmaBefore gText_EmmaBeat
	msgbox gText_EmmaAfter MSG_FACE
	end
.global EventScript_PewterScientist_0X1662B7_0


	@---------------
EventScript_PewterScientist_0X1662B7_0:
	lockall
	setvar 0x4001 0x1
	call EventScript_PewterScientist_0X1662DE
	releaseall
	end
EventScript_PewterScientist_0X1662B7_1:
	lockall
	setvar 0x4001 0x1
	call EventScript_PewterScientist_0X1662DE
	releaseall
	end
EventScript_PewterScientist_0X1662B7_2:
	lockall
	setvar 0x4001 0x2
	call EventScript_PewterScientist_0X1662DE
	releaseall
	end
EventScript_PewterScientist_0X1662B7_3:
	lockall
	setvar 0x4001 0x3
	call EventScript_PewterScientist_0X1662DE
	releaseall
	end

	@---------------
EventScript_PewterScientist_0X1662DE:
	textcolor 0x0
	compare 0x4001 0x1
	if 0x1 _call EventScript_PewterScientist_0X1663CA
	compare 0x4001 0x2
	if 0x1 _call EventScript_PewterScientist_0X1663CA
	compare 0x4001 0x3
	if 0x1 _call EventScript_PewterScientist_0X1663CA
	checksound
	sound 0x15
	applymovement 0x7 EventScript_PewterScientist_0X1A75DB
	waitmovement 0x0
	applymovement 0x7 EventScript_PewterScientist_0X1A75DD
	waitmovement 0x0
	msgbox gText_PewterScientist_0X17E902 MSG_KEEPOPEN @"Oh, [player]\v\h05!"
	compare 0x4001 0x0
	if 0x1 _call EventScript_PewterScientist_0X1663D5
	compare 0x4001 0x1
	if 0x1 _call EventScript_PewterScientist_0X1663DA
	compare 0x4001 0x2
	if 0x1 _call EventScript_PewterScientist_0X1663E6
	compare 0x4001 0x3
	if 0x1 _call EventScript_PewterScientist_0X1663FC
	msgbox gText_PewterScientist_0X17E90C MSG_KEEPOPEN @"I@m glad I caught up to you.\nI@m ..."
	textcolor 0x3
	fanfare 0x13E
	msgbox gText_RecieveEevee MSG_KEEPOPEN
	givepokemon SPECIES_EEVEE 0x10 0x0 0x0 0x0
	waitfanfare
	closeonkeypress
	call EventScript_PewterScientist_0X1A6675
	msgbox gText_PewterScientist_0X17EA06 MSG_KEEPOPEN @"Well, I must be going back to\nthe..."
	closeonkeypress
	compare 0x4001 0x0
	if 0x1 _call EventScript_PewterScientist_0X166412
	compare 0x4001 0x1
	if 0x1 _call EventScript_PewterScientist_0X16641D
	compare 0x4001 0x2
	if 0x1 _call EventScript_PewterScientist_0X166428
	compare 0x4001 0x3
	if 0x1 _call EventScript_PewterScientist_0X166433
	pause 0x1E
	textcolor 0x3
	hidesprite 0x7
	setflag 0x82F
	setvar 0x406C 0x2
	return

	EventScript_NameEevee:
	call 0x1A74EB
	return

	@---------------
EventScript_PewterScientist_0X1663CA:
	applymovement 0x7 EventScript_PewterScientist_0X1A75ED
	waitmovement 0x0
	return

	@---------------
EventScript_PewterScientist_0X1663D5:
	closeonkeypress
	pause 0x14
	return

	@---------------
EventScript_PewterScientist_0X1663DA:
	closeonkeypress
	applymovement 0xFF EventScript_PewterScientist_0X1A75E9
	waitmovement 0x0
	return

	@---------------
EventScript_PewterScientist_0X1663E6:
	closeonkeypress
	applymovement 0x7 EventScript_PewterScientist_0X16643E
	waitmovement 0x0
	applymovement 0xFF EventScript_PewterScientist_0X1A75E9
	waitmovement 0x0
	return

	@---------------
EventScript_PewterScientist_0X1663FC:
	closeonkeypress
	applymovement 0x7 EventScript_PewterScientist_0X166441
	waitmovement 0x0
	applymovement 0xFF EventScript_PewterScientist_0X1A75E9
	waitmovement 0x0
	return

	@---------------
EventScript_PewterScientist_0X1A6675:
	copyvar 0x8012 0x8013
	return

	@---------------
EventScript_PewterScientist_0X166412:
	applymovement 0x7 EventScript_PewterScientist_0X166445
	waitmovement 0x0
	return

	@---------------
EventScript_PewterScientist_0X16641D:
	applymovement 0x7 EventScript_PewterScientist_0X166450
	waitmovement 0x0
	return

	@---------------
EventScript_PewterScientist_0X166428:
	applymovement 0x7 EventScript_PewterScientist_0X16645B
	waitmovement 0x0
	return

	@---------------
EventScript_PewterScientist_0X166433:
	applymovement 0x7 EventScript_PewterScientist_0X166465
	waitmovement 0x0
	return

EventScript_PewterScientist_0X1A75DB:
.byte 0x62
.byte 0xFE

EventScript_PewterScientist_0X1A75DD:
.byte 0x1C
.byte 0x1C
.byte 0x1C
.byte 0xFE

EventScript_PewterScientist_0X1A75ED:
.byte 0x2D
.byte 0xFE

EventScript_PewterScientist_0X1A75E9:
.byte 0x2E
.byte 0xFE

EventScript_PewterScientist_0X16643E:
.byte 0x1A
.byte 0x10
.byte 0xFE

EventScript_PewterScientist_0X166441:
.byte 0x1A
.byte 0x10
.byte 0x10
.byte 0xFE

EventScript_PewterScientist_0X166445:
.byte 0x10
.byte 0x12
.byte 0x12
.byte 0x12
.byte 0x12
.byte 0x12
.byte 0x12
.byte 0x12
.byte 0x12
.byte 0x12
.byte 0xFE

EventScript_PewterScientist_0X166450:
.byte 0x12
.byte 0x10
.byte 0x12
.byte 0x12
.byte 0x12
.byte 0x12
.byte 0x12
.byte 0x12
.byte 0x12
.byte 0x12
.byte 0xFE

EventScript_PewterScientist_0X16645B:
.byte 0x12
.byte 0x12
.byte 0x12
.byte 0x12
.byte 0x12
.byte 0x12
.byte 0x12
.byte 0x12
.byte 0x12
.byte 0xFE

EventScript_PewterScientist_0X166465:
.byte 0x12
.byte 0x12
.byte 0x12
.byte 0x12
.byte 0x12
.byte 0x12
.byte 0x12
.byte 0x12
.byte 0x12
.byte 0xFE

.global EventScript_Brock
EventScript_Brock:
	lock
	checkflag 0x820
	if 0x1 _goto EventScript_BrockBeat
	trainerbattle1 0x1 0x19E 0x0 0x8190CD4 0x8190E4F 0x816A5C5
	release
	end

EventScript_BrockBeat:
	checkflag 0x827
	if equal _goto EventScript_BrockRematch
	msgbox 0x819110F MSG_KEEPOPEN
	release
	end

EventScript_BrockRematch:
	checkflag FLAG_BROCK_REMATCH_BEAT
	if 0x1 _goto EventScript_BrockRematchDone
	msgbox gText_BrockBeforeRematch MSG_FACE
	trainerbattle3 0x1 0xD 0x0 gText_BrockRematchLose
	msgbox gText_BrockRematchAfter1 MSG_FACE
	giveitem ITEM_TYRANITARITE 0x1 MSG_OBTAIN
	giveitem ITEM_SMOOTH_ROCK 0x2 MSG_OBTAIN
	giveitem ITEM_HARD_STONE 0x3 MSG_OBTAIN
	setflag FLAG_BROCK_REMATCH_BEAT
	release
	end

EventScript_BrockRematchDone:
	msgbox gText_BrockRematchAfter2 MSG_FACE
	release
	end

.global EventScript_Ilima
EventScript_Ilima:
	lock
	msgbox gText_IlimaHello MSG_FACE
	trainerbattle3 0x1 0x1E 0x0 gText_IlimaLost
	msgbox gText_IlimaAfter MSG_FACE
	fadescreen 0x1
	setflag FLAG_BEAT_ILIMA
	hidesprite 8
	fadescreen 0x0
	release
	end

.global EventScript_ScamFeebas
EventScript_ScamFeebas:
	lock
	faceplayer
	showmoney 0x14 0x0 0x00
	msgbox gText_WantBuyFeebas MSG_YESNO
	compare LASTRESULT 0x1
	if equal _goto EventScript_CheckMoneyFeebas
	msgbox gText_NoFeebas MSG_FACE
	hidemoney 0x14 0x0
	release
	end

EventScript_CheckMoneyFeebas:
	checkmoney 1000 0x0
	compare LASTRESULT 0x1
	if lessthan _goto EventScript_NotEnoguhMoneyFeebas
	removemoney 1000
	updatemoney 0x14 0x0
	setvar 0x8000 MOVE_SPLASH
	setvar 0x8001 MOVE_WATERPULSE
	setvar 0x8002 0xFFFF
	setvar 0x8003 0xFFFF
	setvar 0x8004 0x0
	setvar 0x8005 0x0
	setvar 0x8006 25
	setvar 0x8007 25
	setvar 0x8008 25
	setvar 0x8009 25
	setvar 0x800A 25
	setvar 0x800B 25
	givepokemon SPECIES_FEEBAS 10 0x0 0x0 0x1
	fanfare 0x101
	msgbox gText_RecieveFeebas MSG_FACE
	waitfanfare 
	msgbox gText_GtgFeebas MSG_FACE
	hidemoney 0x14 0x0
	release
	end

EventScript_NotEnoguhMoneyFeebas:
	msgbox gText_FeebasNoMoney MSG_FACE
	hidemoney 0x14 0x0
	release
	end

.global EventScript_TylerSkyBattle
EventScript_TylerSkyBattle:
	lock
	checkflag 0x9BE
	if SET _goto EventScript_AlreadyBattledTyler
	msgbox gText_HiTylerSky MSG_YESNO
	compare LASTRESULT 0x1
	if equal _goto EventScript_StartTylerBattle
	msgbox gText_NoTylerSky MSG_FACE
	release
	end

EventScript_StartTylerBattle:
	special 0x51
	compare LASTRESULT 0x0
	if equal _goto EventScript_CantParticipateSky
	msgbox gText_PrepareTylerSky MSG_FACE
	setflag 0x901
	trainerbattle3 0x1 54 0x0 gText_TylerSkyLose
	clearflag 0x901
	msgbox gText_TylerGiveTm MSG_FACE
	giveitem ITEM_TM51 0x1 MSG_OBTAIN
	setflag 0x9BE
	goto EventScript_AlreadyBattledTyler
	release
	end

EventScript_AlreadyBattledTyler:
	msgbox gText_TylerExplainRoost MSG_FACE
	release
	end

EventScript_CantParticipateSky:
	msgbox gText_NoSkyTylerSky MSG_FACE
	release
	end

.global EventScript_CosmicPowerTutor
EventScript_CosmicPowerTutor:
	lock
	faceplayer
	msgbox gText_AskCosmicPower MSG_YESNO
	compare LASTRESULT 0x1
	if equal _goto EventScript_TeachCosmicPower
	msgbox gText_NoCosmicPower MSG_FACE
	release
	end
	
EventScript_TeachCosmicPower:
	msgbox gText_WhichMonCosmicPower MSG_FACE
	setvar 0x8005 65
	special 0x18D
	waitstate
	compare LASTRESULT 0x0
	if equal _goto EventScript_End
	msgbox gText_NowKnowsCosmicPower MSG_FACE
	release
	end

EventScript_End:	
	release
	end

.global EventScript_Steven
EventScript_Steven:
	lock
	checkflag FLAG_BEAT_STEVEN
	if SET _goto EventScript_StevenExplain
	faceplayer
	checkflag 0x827 
	if SET _goto EventScript_StevenAsk
	msgbox gText_StevenHiWeak MSG_FACE
	release
	end

EventScript_StevenAsk:
	msgbox gText_StevenHiStrong MSG_YESNO
	compare LASTRESULT 0x1
	if equal _goto EventScript_StevenBattle
	msgbox gText_StevenNoBattle MSG_FACE
	release
	end

EventScript_StevenBattle:
	msgbox gText_StevenBattleBefore MSG_FACE
	trainerbattle3 0x1 58 0x0 gText_StevenLose
	msgbox gText_StevenAfterGive MSG_FACE
	giveitem ITEM_BLAZIKENITE 0x1 MSG_OBTAIN
	giveitem ITEM_SWAMPERTITE 0x1 MSG_OBTAIN
	giveitem ITEM_SCEPTILITE 0x1 MSG_OBTAIN
	msgbox gText_StevenGiveItems1 MSG_FACE
	giveitem ITEM_LIFE_ORB 0x1 MSG_OBTAIN
	giveitem ITEM_LEFTOVERS 0x1 MSG_OBTAIN
	giveitem ITEM_HEAVY_DUTY_BOOTS 0x1 MSG_OBTAIN
	giveitem ITEM_AIR_BALLOON 0x1 MSG_OBTAIN
	giveitem ITEM_SITRUS_BERRY 0x3 MSG_OBTAIN
	msgbox gText_StevenGiveMeta MSG_FACE
	giveitem ITEM_METAGROSSITE 0x1 MSG_OBTAIN
	setflag FLAG_BEAT_STEVEN
	goto EventScript_StevenExplain
	release
	end

EventScript_StevenExplain:
	msgbox gText_StevenExplain MSG_FACE
	release
	end

.global EventScript_HealBellTutor
EventScript_HealBellTutor:
	lock
	faceplayer
	msgbox gText_ICanTeachHealBell MSG_YESNO
	compare LASTRESULT 0x1
	if equal _goto EventScript_CheckHellBellMoney
	msgbox gText_NoHealBell MSG_FACE
	release
	end

EventScript_CheckHellBellMoney:
	checkmoney 500 0x0
	compare LASTRESULT 0x1
	if lessthan _goto EventScript_NoMoneyHealBell
	msgbox gText_WhichLearnHealBell MSG_FACE
	setvar 0x8005 4
	special 0x18D
	waitstate
	compare LASTRESULT 0x0
	if equal _goto EventScript_NoHealBell
	msgbox gText_GotHealBell MSG_FACE
	msgbox gText_Now500 MSG_FACE
	removemoney 500 0x0
	msgbox gText_YouGave500To MSG_FACE
	msgbox gText_ComeBackHealBell MSG_FACE
	release
	end

EventScript_NoHealBell:
	msgbox gText_NoHealBell MSG_FACE
	release
	end

EventScript_NoMoneyHealBell:
	msgbox gText_No500Money MSG_FACE
	release
	end
