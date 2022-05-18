.thumb
.align 2

.include "../xse_commands.s"
.include "../xse_defines.s"
.include "../asm_defines.s"

.equ FLAG_MEET_AKIO_1, 0x968
.equ FLAG_WORMHOLE_SEEN, 0x96E
.equ FLAG_WORMHOLE_SEEN2, 0x96F
.equ SP_DAILY_EVENT, 0xA0
.equ SP_UPDATE_TIME_IN_VARS, 0xA1
.equ SP_GET_TIME_DIFFERENCE, 0xA2
.equ VAR_DAILY_EVENT, 0x50A4 @Also uses 0x50D3
.equ FLAG_UB_1_FOUGHT, 0x970
.equ FLAG_UB_2_FOUGHT, 0x971
.equ FLAG_UB_3_FOUGHT, 0x972
.equ FLAG_UB_4_FOUGHT, 0x973
.equ FLAG_UB_5_FOUGHT, 0x974
.equ FLAG_UB_6_FOUGHT, 0x975
.equ FLAG_UB_7_FOUGHT, 0x976
.equ VAR_CHECK_BOULDER, 0x510A
.equ FLAG_GOT_CHARMS, 0x99E
.global EventScript_Viridian_PkmSign
EventScript_Viridian_PkmSign:
	lock
	msgbox gText_ViridianSign MSG_SIGN
	release
	end

.global EventScript_DexNav
EventScript_DexNav:
	lock
	faceplayer
	setvar 0x4012 0x1
	setvar 0x4013 0x1
	clearflag 0x91E
	setvar 0x4013 0x0
	msgbox gText_IntroAid MSG_KEEPOPEN
	closeonkeypress
	fanfare 0x13E
	msgbox gText_Upgraded MSG_KEEPOPEN
	waitfanfare
	closeonkeypress
	msgbox gText_HaveToGo MSG_KEEPOPEN
	closeonkeypress
	fadescreen 0x1
	hidesprite 0x3
	sound 0x9
	fadescreen 0x0
	setflag 0x91E
	msgbox gText_LetterOak MSG_KEEPOPEN
	closeonkeypress
	release
	end

EventScript_DexNavTile:
	lockall
	checkflag 0x91E
	if 0x1 _goto EventScript_HaveDexNav
	applymovement 0xFF EventScript_MovePlayer
	waitmovement 0x0
	goto EventScript_DexNav
	releaseall
	end

EventScript_HaveDexNav:
	releaseall
	end


EventScript_MovePlayer:
.byte look_right
.byte 0xFE

.global EventScript_AkioUB
EventScript_AkioUB:	
	lock
	checkflag FLAG_UB_7_FOUGHT
	if 0x1 _goto EventScript_AkioAllWormholes
	checkflag FLAG_WORMHOLE_SEEN2
	if 0x1 _goto EventScript_CheckUBOpen
	checkflag FLAG_MEET_AKIO_1
	if 0x1 _goto EventScript_AkioUBCheckMissingNo
	msgbox gText_AkioNicknames MSG_FACE
	release
	end

EventScript_AkioAllWormholes:
	msgbox gText_AkioAllWormholes MSG_FACE
	release
	end

EventScript_AkioUBCheckMissingNo:
	msgbox gText_AkioAsk MSG_FACE
	checkflag 0x967
	if 0x0 _goto EventScript_NoMissingNo
	msgbox gText_AkioYesMissingNo MSG_FACE
	fadescreen 0x1
	setflag FLAG_WORMHOLE_SEEN2
	clearflag FLAG_WORMHOLE_SEEN
	showsprite 0x4
	fadescreen 0x0
	msgbox gText_AkioUBSay MSG_FACE
	release
	end
	
EventScript_NoMissingNo:
	msgbox gText_AkioNoMissingNo MSG_FACE
	release
	end

.global EventScript_AkioLevels
EventScript_AkioLevels:
	mapscript MAP_SCRIPT_ON_LOAD EventScript_AkioUBSet
	.byte MAP_SCRIPT_TERMIN

EventScript_AkioUBSet:
	checkflag FLAG_WORMHOLE_SEEN2
	if 0x0 _call EventScript_SetAkioUBFlag
	release
	end

EventScript_SetAkioUBFlag:
	setflag FLAG_WORMHOLE_SEEN
	hidesprite 0x4
	return

EventScript_CheckUBOpen:
	setvar 0x8000 VAR_DAILY_EVENT
	setvar 0x8001 0x0
	special2 LASTRESULT SP_DAILY_EVENT
	compare LASTRESULT 0x0
	if equal _goto EventScript_UBClosed
	@@setvar 0x8000 VAR_DAILY_EVENT
	@@special SP_UPDATE_TIME_IN_VARS
	msgbox gText_AkioUBOpen MSG_FACE
	release
	end

EventScript_UBClosed:
	setvar 0x8000 VAR_DAILY_EVENT
	setvar 0x8001 0x1 
	special2 LASTRESULT SP_GET_TIME_DIFFERENCE
	buffernumber 0x0 LASTRESULT
	msgbox gText_AkioUBClosed MSG_FACE
	release
	end

.global EventScript_UltraWormhole
EventScript_UltraWormhole:
	lock
	checkflag FLAG_UB_7_FOUGHT
	if 0x1 _goto EventScript_AllWormholesEntered
	setvar 0x8000 VAR_DAILY_EVENT
	setvar 0x8001 0x0
	special2 LASTRESULT SP_DAILY_EVENT
	compare LASTRESULT 0x0
	if equal _goto EventScript_UltraWormholeClosed
	msgbox gText_UltraWormholeEnter MSG_YESNO
	compare LASTRESULT 0x1
	if 0x1 _goto EventScript_UltraWormholeWarp
	release
	end

EventScript_AllWormholesEntered:
	msgbox gText_AllWormholesEntered MSG_NORMAL
	release
	end

EventScript_UltraWormholeWarp:
	checkflag FLAG_UB_1_FOUGHT
	if 0x0 _goto EvetnScript_UB1Warp
	checkflag FLAG_UB_2_FOUGHT
	if 0x0 _goto EvetnScript_UB2Warp
	checkflag FLAG_UB_3_FOUGHT
	if 0x0 _goto EvetnScript_UB3Warp
	checkflag FLAG_UB_4_FOUGHT
	if 0x0 _goto EvetnScript_UB4Warp
	checkflag FLAG_UB_5_FOUGHT
	if 0x0 _goto EvetnScript_UB5Warp
	checkflag FLAG_UB_6_FOUGHT
	if 0x0 _goto EvetnScript_UB6Warp
	checkflag FLAG_UB_7_FOUGHT
	if 0x0 _goto EvetnScript_UB7Warp
	msgbox gText_AllWormholesEntered MSG_NORMAL
	release
	end

EvetnScript_UB1Warp:
	warp 43 3 0xFF 0xC 0x5
	setvar 0x8000 VAR_DAILY_EVENT
	setvar 0x8001 0x1
	special2 LASTRESULT SP_DAILY_EVENT
	release
	end

EvetnScript_UB2Warp:
	warp 43 4 0xFF 0x3 0xE
	setvar 0x8000 VAR_DAILY_EVENT
	setvar 0x8001 0x1
	special2 LASTRESULT SP_DAILY_EVENT
	release
	end

EvetnScript_UB3Warp:
	warp 43 5 0xFF 0x3 0x4
	setvar 0x8000 VAR_DAILY_EVENT
	setvar 0x8001 0x1
	special2 LASTRESULT SP_DAILY_EVENT
	release
	end

EvetnScript_UB4Warp:
	warp 43 6 0xFF 0x3 0x14
	setvar 0x8000 VAR_DAILY_EVENT
	setvar 0x8001 0x1
	special2 LASTRESULT SP_DAILY_EVENT
	release
	end

EvetnScript_UB5Warp:
	warp 43 7 0xFF 0x16 0x18
	setvar 0x8000 VAR_DAILY_EVENT
	setvar 0x8001 0x1
	special2 LASTRESULT SP_DAILY_EVENT
	release
	end

EvetnScript_UB6Warp:
	warp 43 8 0xFF 0x3 0x10
	setvar 0x8000 VAR_DAILY_EVENT
	setvar 0x8001 0x1
	special2 LASTRESULT SP_DAILY_EVENT
	release
	end

EvetnScript_UB7Warp:
	warp 43 9 0xFF 0x3 0x4
	setvar 0x8000 VAR_DAILY_EVENT
	setvar 0x8001 0x1
	special2 LASTRESULT SP_DAILY_EVENT
	release
	end

EventScript_UltraWormholeClosed:
	msgbox gText_UltraWormholeClosed MSG_NORMAL
	release
	end


EventScript_UB1WarpBack:
	lock
	msgbox gText_UltraWormholeEnter MSG_YESNO
	compare LASTRESULT 0x0
	if equal _goto EventScript_UBNothing
	warp 5 0 0xFF 0x9 0x4
	release
	end

EventScript_UBNothing:
	release
	end

.global EventScript_CheckBadgeAndRematch
EventScript_CheckBadgeAndRematch:
	lockall
	applymovement 0x1 0x81A75E7
	checkflag 0x827
	if 0x1 _goto EventScript_CheckRematches
	checkflag 0x820
	if 0x1 _goto EventScript_Pass
	bufferstring 0x0 gText_BoulderBadge
	msgbox gText_NoBadge MSG_KEEPOPEN 
	sound 0x16
	msgbox 0x81A5776 MSG_KEEPOPEN 
	closeonkeypress
	applymovement 0xFF 0x81A7AB7
	applymovement 0x8009 0x81A75ED
	waitmovement 0x0
	releaseall
	release
	end

EventScript_Pass:
	msgbox gText_HaveBadge MSG_KEEPOPEN
	setvar 0x510A 0x1
	releaseall
	end

EventScript_CheckRematches:
	checkflag FLAG_GOT_CHARMS
	if NOT_SET _goto EventScript_NeedParentalPerm
	@@callasm CheckRematchDone
	@@compare LASTRESULT 0x1
	goto EventScript_Pass
	@@msgbox gText_PokemonLeagueRebuilt MSG_KEEPOPEN
	@@closeonkeypress
	@@applymovement 0xFF 0x81A7AB7
	@@waitmovement 0x0
	releaseall
	end

EventScript_NeedParentalPerm:
	msgbox gText_PokemonLeagueNoPerm1 MSG_FACE
	applymovement 1 EventScript_ExclamantionMark
	sound 0x15
	waitmovement 0x0
	msgbox gText_PokemonLeagueNoPerm2 MSG_FACE
	applymovement 0xFF 0x81A7AB7
	waitmovement 0x0
	releaseall
	end

EventScript_ExclamantionMark:
	.byte 0x62
	.byte 0xFE

.global EventScript_GiveDreamEaterTM
EventScript_GiveDreamEaterTM:
	lock
	checkflag 0x9C
	if SET _goto EventScript_HasDreamEater
	msgbox gText_GiveDreamEaterTM MSG_FACE
	giveitem ITEM_TM85 0x1 MSG_OBTAIN
	setflag 0x9CE
	goto EventScript_HasDreamEater
	release
	end

EventScript_HasDreamEater:
	msgbox gText_HasDreamEater MSG_FACE
	release
	end

.global EventScript_FinalRivalFight
EventScript_FinalRivalFight:
	lock
	msgbox gText_WaitUpPlayer2 MSG_FACE
	applymovement 9 EventScript_MayMoveViridian
	applymovement 10 EventScript_BarryMoveViridian
	waitmovement 0x0
	msgbox gText_BarryBattleE4 MSG_FACE
	special 0x0
	trainerbattle3 0x1 0x14 0x0 gText_BarryLoseE4
	msgbox gText_BarryHeal MSG_FACE
	fadescreen 0x1
	sound 0x1
	special 0x0
	fadescreen 0x0
	msgbox gText_MayE4 MSG_FACE
	trainerbattle3 0x1 0x15 0x0 gText_MayLoseE4
	msgbox gText_MayGiveMega MSG_FACE
	giveitem ITEM_LATIOSITE 0x1 MSG_OBTAIN
	giveitem ITEM_LATIASITE 0x1 MSG_OBTAIN
	giveitem ITEM_MEWTWONITE_X 0x1 MSG_OBTAIN
	giveitem ITEM_MEWTWONITE_Y 0x1 MSG_OBTAIN
	msgbox gText_MayByeBarryBye MSG_FACE
	applymovement 9 EventScript_MayMoveViridian2
	applymovement 10 EventScript_BarryMoveViridian2
	waitmovement 0x0
	setflag 0x9F1
	setvar 0x510D 0x1
	hidesprite 9
	hidesprite 10
	release
	end

EventScript_MayMoveViridian:
	.byte walk_down
	.byte walk_down
    .byte walk_down
	.byte walk_down
    .byte walk_down
	.byte walk_down
	.byte 0xFE

EventScript_BarryMoveViridian:
	.byte walk_down
	.byte walk_down
    .byte walk_down
	.byte walk_down
    .byte walk_down
	.byte walk_down
	.byte 0xFE

EventScript_MayMoveViridian2:
	.byte walk_up
	.byte walk_up
    .byte walk_up
	.byte walk_up
    .byte walk_up
	.byte walk_up
	.byte 0xFE

EventScript_BarryMoveViridian2:
	.byte walk_up
	.byte walk_up
    .byte walk_up
	.byte walk_up
    .byte walk_up
	.byte walk_up
	.byte 0xFE

.global EventScript_NurseJoyHeal
EventScript_NurseJoyHeal:
	lock
	msgbox gText_TakeARestJoy MSG_FACE
	fadescreen 0x1
	@@givepokemon SPECIES_ZOROARK_H 70 0x0 0x0 0x0
	setflag 0x94E
	sound 0x1
	special 0x0
	fadescreen 0x0
	msgbox gText_StaySafeJourney MSG_FACE
	release
	end