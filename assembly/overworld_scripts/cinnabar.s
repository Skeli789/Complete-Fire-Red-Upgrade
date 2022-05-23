.thumb
.align 2

.include "../xse_commands.s"
.include "../xse_defines.s"
.include "../asm_defines.s"

.equ FLAG_MEET_AKIO_1, 0x968
.equ FLAG_BARRY_CINNABAR, 0x969
.equ VAR_BARRY_SCRIPT, 0x5101
.equ FLAG_GOT_WATERFALL, 0x97C
.equ FLAG_ARCEUS_BATTLED, 0x9AB
.equ FLAG_BLAINE_REMATCH_BEAT, 0x991
.equ FLAG_GOT_OTHER_FOSSIL, 0x1015

.global EventScript_MissingNo
EventScript_MissingNo:
	lock
	checkflag 0x967
	if 0x0 _goto EventScript_MissingNoEnc
	release
	end

EventScript_MissingNoEnc:
	msgbox gText_MissingNoEnc MSG_NORMAL
	wildbattle 0x4F4 70 0x0
	setflag 0x967
	release
	end


.global EventScript_Akio1
EventScript_Akio1:
	lock
	msgbox gText_AkioHi MSG_FACE
	fadescreen 0x1
	hidesprite 0x2
	setflag FLAG_MEET_AKIO_1
	sound 0x9
	fadescreen 0x0
	release
	end


.global EventScript_BarryCinnabar
EventScript_BarryCinnabar:
	lock
	showsprite 0x5
	applymovement 0x5 EventScript_BarryMoveC1
	waitmovement 0x0
	sound 0x7
	applymovement 0xFF EventScript_PlayerMoveC1
	waitmovement 0x0
	msgbox gText_CinnabarTHUMP MSG_NORMAL
	msgbox gText_BarryCinnabarBefore MSG_KEEPOPEN
	closeonkeypress
	trainerbattle3 0x1 0xC 0x0 gText_BarryLoseCinnabar
	msgbox gText_BarryRoamingDogs MSG_KEEPOPEN
	closeonkeypress
	setvar 0x8000 SPECIES_COBALION
	setvar 0x8001 70
	setvar 0x8002 0x1
	setvar 0x8003 0x0
	special 0x129
	compare LASTRESULT 0x0
	if 0x1 _goto TooManyRoamers
	setvar 0x8000 SPECIES_TERRAKION
	setvar 0x8001 70
	setvar 0x8002 0x1
	setvar 0x8003 0x0
	special 0x129
	compare LASTRESULT 0x0
	if 0x1 _goto TooManyRoamers
	setvar 0x8000 SPECIES_VIRIZION
	setvar 0x8001 70
	setvar 0x8002 0x1
	setvar 0x8003 0x0
	special 0x129
	compare LASTRESULT 0x0
	if 0x1 _goto TooManyRoamers
	msgbox gText_BarryCinnabarBye MSG_KEEPOPEN
	closeonkeypress
	applymovement 0x5 EventScript_BarryMoveC2
	waitmovement 0x0
	setvar 0x5101 0x1
	hidesprite 0x5
	release
	end

TooManyRoamers:
	msgbox gText_BarryCinnabarTooMany MSG_KEEPOPEN
	closeonkeypress
	msgbox gText_BarryCinnabarBye MSG_KEEPOPEN
	closeonkeypress
	applymovement 0x5 EventScript_BarryMoveC2
	waitmovement 0x0
	setvar 0x5101 0x1
	hidesprite 0x5
	release
	end

EventScript_BarryMoveC1:
	.byte walk_down
	.byte 0xFE

EventScript_PlayerMoveC1:
	.byte walk_down
	.byte look_up
	.byte 0xFE

EventScript_BarryMoveC2:
	.byte look_right
	.byte walk_right
	.byte walk_right
	.byte walk_right
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

.global EventScript_CinnabarLevels
EventScript_CinnabarLevels:
	mapscript MAP_SCRIPT_ON_TRANSITION EventScript_cinnabar1_0X166EE1
	mapscript MAP_SCRIPT_ON_LOAD EventScript_BarryCinnabarSet
	.byte MAP_SCRIPT_TERMIN

EventScript_BarryCinnabarSet:
	checkflag FLAG_BARRY_CINNABAR
	if 0x0 _call EventScript_SetBarryFlag
	release
	end

EventScript_SetBarryFlag:
	setflag FLAG_BARRY_CINNABAR
	return

	@---------------
EventScript_cinnabar1_0X166EE1:
	setworldmapflag 0x898
	call EventScript_cinnabar1_0X166F5E
	compare 0x4071 0x1
	if 0x1 _call EventScript_cinnabar1_0X166F24
	compare 0x4071 0x3
	if 0x1 _call EventScript_cinnabar1_0X166F52
	compare 0x408A 0x1
	if 0x1 _call EventScript_cinnabar1_0X166F0B
	end

	@---------------
EventScript_cinnabar1_0X166F5E:
	checkflag 0x1A8
	if 0x1 _goto EventScript_cinnabar1_0X166F68
	return

	@---------------
EventScript_cinnabar1_0X166F24:
	getplayerpos 0x8004 0x8005
	compare 0x8004 0x12
	if 0x4 _goto EventScript_cinnabar1_0X166F4A
	setflag 0x2
	@movesprite2 0x3 0x12 0xC
	spritebehave 0x3 0x7
	@movesprite2 0x4 0x1E 0xC
	return

	@---------------
EventScript_cinnabar1_0X166F52:
	@movesprite2 0x3 0x15 0x8
	spritebehave 0x3 0x7
	return

	@---------------
EventScript_cinnabar1_0X166F0B:
	playsong2 0x110
	setflag 0x2
	@movesprite2 0x3 0xF 0xC
	spritebehave 0x3 0x9
	@movesprite2 0x4 0x1E 0xC
	return

	@---------------
EventScript_cinnabar1_0X166F68:
	setvar 0x4001 0x1
	return

	@---------------
EventScript_cinnabar1_0X166F4A:
	@movesprite2 0x4 0x1E 0x8
	return

.global EventScript_WaterfallGive
EventScript_WaterfallGive:
	lock
	faceplayer
	checkflag FLAG_GOT_WATERFALL
	if 0x1 _goto EventScript_AlreadyHaveWaterfall
	msgbox gText_AskWaterfall MSG_YESNO
	compare LASTRESULT 0x1 
	if equal _goto EventScript_GiveWaterfallHM
	msgbox gText_WhyWaterfall MSG_FACE
	release
	end

EventScript_GiveWaterfallHM:
	msgbox gText_HeresWaterfall MSG_FACE
	fanfare 0x101
	msgbox gText_RecieveWaterfall MSG_FACE
	waitfanfare 
	additem ITEM_HM07 0x1
	setflag FLAG_GOT_WATERFALL
	release
	end

EventScript_AlreadyHaveWaterfall:
	msgbox gText_AlreadyHaveWaterfall MSG_FACE
	release
	end

.global EventScript_ArceusFluteMan
EventScript_ArceusFluteMan:
	lock
	checkflag FLAG_ARCEUS_BATTLED
	if SET _goto EventScript_ArceusAlreadyBattled
	callasm HasCreationTrio
	compare LASTRESULT 0x0
	if equal _goto EventScript_ArceusFluteNotrio
	msgbox gText_HasCreationTrio1 MSG_FACE
	applymovement 1 EventScript_ExclamantionMark
	sound 0x15
	waitmovement 0x0
	msgbox gText_HasCreationTrio2 MSG_KEEPOPEN
	fadeoutbgm 1
	cry SPECIES_ARCEUS 0x0
	closeonkeypress
	msgbox gText_WaitArceus MSG_FACE
	applymovement 0xFF EventScript_FaceDownShocked
	sound 0x15
	waitmovement 0x0
	fadeinbgm 1
	wildbattle SPECIES_ARCEUS 80 0x0
	setflag FLAG_ARCEUS_BATTLED
	goto EventScript_ArceusAlreadyBattled
	release
	end

EventScript_ArceusFluteNotrio:
	msgbox gText_NoCreationTrio MSG_FACE
	release
	end

EventScript_ArceusAlreadyBattled:
	msgbox gText_ArceusDone MSG_FACE
	release
	end

EventScript_ExclamantionMark:
	.byte 0x62
	.byte 0xFE

EventScript_FaceDownShocked:
	.byte look_down
	.byte 0x62
	.byte 0xFE

.global EventScript_Blaine
EventScript_Blaine:
	lock
	checkflag 0x826
	if 0x1 _goto EventScript_BlaineBeat
	trainerbattle1 0x1 0x1A3 0x0 0x8199543 0x81995FA 0x816DA06
	release
	end

EventScript_BlaineBeat:
	checkflag 0x827
	if equal _goto EventScript_BlaineRematch
	msgbox 0x819971D MSG_KEEPOPEN
	release
	end

EventScript_BlaineRematch:
	checkflag FLAG_BLAINE_REMATCH_BEAT
	if 0x1 _goto EventScript_BlaineRematchDone
	msgbox gText_BlaineBeforeRematch MSG_FACE
	goto EventScript_DoBattleBlaine
	release
	end

EventScript_DoBattleBlaine:
	trainerbattle3 0x1 0x13 0x0 gText_BlaineRematchLose
	msgbox gText_BlaineRematchAfter1 MSG_FACE
	giveitem ITEM_HEAT_ROCK 0x5 MSG_OBTAIN
	setflag FLAG_BLAINE_REMATCH_BEAT
	goto EventScript_BlaineRematchDone
	release
	end

EventScript_BlaineRematchDone:
	msgbox gText_BlaineRematchAfter2 MSG_FACE
	release
	end

.global EventScript_GiveZygardeCube
EventScript_GiveZygardeCube:
	lock
	checkflag 0x1009
	if SET _goto EventScript_ExplainCube
	msgbox gText_IfHaveZygarde MSG_FACE
	fanfare 0x101
	additem ITEM_ZYGARDE_CUBE 0x1
	setvar 0x8004 ITEM_ZYGARDE_CUBE
	setvar 0x8006 0x0
	special 0xE4
	msgbox gText_PlayerRecievedCube MSG_KEEPOPEN
	waitfanfare
	special 0xE5
	closeonkeypress
	setflag 0x1009
	goto EventScript_ExplainCube
	release
	end

EventScript_ExplainCube:
	msgbox gText_ExplainCube MSG_FACE
	release
	end

.global EventScript_CinnabarTutor
EventScript_CinnabarTutor:
	lock
	faceplayer 
	msgbox gText_ImAMoveTutor MSG_YESNO
	compare LASTRESULT 0x1
	if equal _goto EventScript_SelectCinnabarTutor
	goto EventScript_ComeBackCinnabarTutor
	release
	end
EventScript_ComeBackCinnabarTutor:
	msgbox gText_EverInNeedForAMove MSG_FACE
	release
	end

EventScript_SelectCinnabarTutor:
	preparemsg gText_SelectMoveFuschia
	waitmsg
	setvar 0x8004 0x0
	setvar 0x8000 0x18
	setvar 0x0001 0x6
	special 0x158
	waitstate 
	switch LASTRESULT
	case 0, LeafStorm
	case 1, Megahorn
	case 2, PowerWhip
	case 3, SolarBlade
	case 4, FlareBlitz
	case 5, Hurricane
	case 6, BraveBird
	case 7, HydroPump
	case 8, WeatherBall
	case 9, Superpower
	case 10, CloseCombat
	goto EventScript_ComeBackCinnabarTutor
	release
	end

LeafStorm:
	setvar 0x8005 124	
	goto EventScript_CinnabarTeachMove
	release
	end
Megahorn:
	setvar 0x8005 125	
	goto EventScript_CinnabarTeachMove
	release
	end
PowerWhip:
	setvar 0x8005 126
	goto EventScript_CinnabarTeachMove
	release
	end
SolarBlade:
	setvar 0x8005 127	
	goto EventScript_CinnabarTeachMove
	release
	end
FlareBlitz:
	setvar 0x8005 121	
	goto EventScript_CinnabarTeachMove
	release
	end
Hurricane:
	setvar 0x8005 122	
	goto EventScript_CinnabarTeachMove
	release
	end
BraveBird:
	setvar 0x8005 119	
	goto EventScript_CinnabarTeachMove
	release
	end
HydroPump:
	setvar 0x8005 123	
	goto EventScript_CinnabarTeachMove
	release
	end
WeatherBall:
	setvar 0x8005 101	
	goto EventScript_CinnabarTeachMove
	release
	end
Superpower:
	setvar 0x8005 0x34
	goto EventScript_CinnabarTeachMove
	release
	end
CloseCombat:
	setvar 0x8005 0x78
	goto EventScript_CinnabarTeachMove
	release
	end

EventScript_CinnabarTeachMove:
	special 0x18D
	waitstate
	release
	end

.global EventScript_Eeevee8
EventScript_Eeevee8:
	msgbox gText_Eevee8 MSG_FACE
	end

.global EventScript_GiveOtherFossil
EventScript_GiveOtherFossil:
	lock
	checkflag FLAG_GOT_OTHER_FOSSIL
	if SET _goto EventScript_TalkAboutOtherFossil
	checkflag 0x272
	if SET _goto EventScript_GiveHelix
	msgbox gText_TakeOtherFossil MSG_FACE
	giveitem ITEM_DOME_FOSSIL 0x1 MSG_OBTAIN
	setflag FLAG_GOT_OTHER_FOSSIL
	release
	end

EventScript_GiveHelix:
	msgbox gText_TakeOtherFossil MSG_FACE
	giveitem ITEM_HELIX_FOSSIL 0x1 MSG_OBTAIN
	setflag FLAG_GOT_OTHER_FOSSIL
	release
	end

EventScript_TalkAboutOtherFossil:
	msgbox gText_UsedOtherFossilQuestion MSG_FACE
	release
	end

.global EventScript_MoonblastTutor
EventScript_MoonblastTutor:
	lock
	faceplayer
	msgbox gText_TeachMoonblast MSG_YESNO
	compare LASTRESULT 0x1
	if equal _goto EventScript_TeachMoonblast
	release
	end

EventScript_TeachMoonblast:
	setvar 0x8005 147
	special 0x18D
	waitstate
	compare LASTRESULT 0x0
	if equal _goto EventScript_End1
	msgbox gTexst_Moonblast MSG_FACE
	release
	end

EventScript_End1:
	release
	end