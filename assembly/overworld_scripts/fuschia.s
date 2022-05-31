.thumb
.align 2

.include "../xse_commands.s"
.include "../xse_defines.s"
.include "../asm_defines.s"

.equ FLAG_KOGA_REMATCH_BEAT, 0x98F

.global EventScript_MayFuschia
EventScript_MayFuschia:
	lock
	checkflag 0x964
	compare LASTRESULT 0x1
	if 0x0 _goto EventScript_MayFuschiaStart
	release
	end


EventScript_MayFuschiaStart:
	opendoor 0x9 0x20
	waitdooranim
	showsprite 0x11
	applymovement 0xFF EventScript_FuschiaPlayerMove1
	waitmovement 0x0
	sound 0x15
	applymovement 0x11 EventScript_FuschiaMayMove1
	waitmovement 0x0
	sound 0x15
	closedoor 0x9 0x20
	waitdooranim
	msgbox gText_MayFuschiaBefore MSG_FACE
	trainerbattle3 0x1 0xA 0x0 gText_MayFuschiaLose
	setflag 0x964
	setvar 0x4019 0x1
	msgbox gText_MayFuschiaAfter MSG_FACE
	applymovement 0x11 EventScript_FuschiaMayMove2
	waitmovement 0x0
	hidesprite 0x11
	release
	end

EventScript_FuschiaMayMove1:
	.byte walk_down
	.byte 0x62
	.byte 0xFE

EventScript_FuschiaPlayerMove1:
	.byte walk_down
	.byte look_up
	.byte 0x62
	.byte 0xFE

EventScript_FuschiaMayMove2:
	.byte look_left
	.byte walk_left
	.byte walk_left
	.byte walk_left
	.byte walk_left
	.byte walk_left
	.byte 0x16
	.byte walk_left
	.byte look_up
	.byte walk_up
	.byte walk_up
	.byte walk_up
	.byte walk_up
	.byte walk_up
	.byte walk_up
	.byte walk_up
	.byte walk_up
	.byte 0xFE

.global EventScript_DracoMeteorTutor
EventScript_DracoMeteorTutor:
	lock
	faceplayer
	msgbox gText_AskTeachDraco MSG_YESNO
	compare LASTRESULT 0x1
	if equal _goto EventScript_TeachDraco
	msgbox gText_MissingOutDraco MSG_FACE
	release
	end

EventScript_TeachDraco:
	msgbox gText_SelectDragon MSG_FACE
	setvar 0x8005 148
	special 0x18D
	waitstate
	compare LASTRESULT 0x0
	if equal _goto EventScript_NotADragon
	msgbox gText_CongratsDraco MSG_FACE
	release
	end

EventScript_NotADragon:
	msgbox gText_NotADragon MSG_FACE
	release
	end

.global EventScript_LegendTutor
EventScript_LegendTutor:
	lock
	faceplayer
	msgbox gText_ICanTeachTutor MSG_YESNO
	compare LASTRESULT 0x1
	if equal _goto EventScript_SelectLegendMove
	release
	end

EventScript_SelectLegendMove:
	preparemsg gText_WhichLegendMove 
	waitmsg
	setvar 0x8004 0x0
	setvar 0x8000 0xF
	setvar 0x0001 0x6
	special 0x158
	waitstate 
	compare LASTRESULT 0x6
	if greaterorequal _goto EventScript_End
	switch LASTRESULT
	case 0, EventScript_SecretSword
	case 1, EventScript_RelicSong
	case 2, EventScript_DragonAscent
	case 3, EventScript_1000Arrows
	case 4, EventScript_1000Waves
	case 5, EventScript_CoreEnforcer
	release
	end

EventScript_SecretSword:
	setvar 0x8005 149
	goto EventScript_TeachMoveLegend
	release
	end

EventScript_RelicSong:
	setvar 0x8005 150
	goto EventScript_TeachMoveLegend
	release
	end

EventScript_DragonAscent:
	setvar 0x8005 152
	goto EventScript_TeachMoveLegend
	release
	end

EventScript_1000Arrows:
	setvar 0x8005 153
	goto EventScript_TeachMoveLegend
	release
	end

EventScript_1000Waves:
	setvar 0x8005 154
	goto EventScript_TeachMoveLegend
	release
	end

EventScript_CoreEnforcer:
	setvar 0x8005 155
	goto EventScript_TeachMoveLegend
	release
	end

EventScript_TeachMoveLegend:
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

.global EventScript_SubTM
EventScript_SubTM:
	lock
	checkflag 0x9CC
	if SET _goto EventScript_TalkSubTM
	msgbox gText_GiveSubTM MSG_FACE
	giveitem ITEM_TM90 0x1 MSG_OBTAIN
	setflag 0x9CC
	goto EventScript_TalkSubTM
	release
	end

EventScript_TalkSubTM:
	msgbox gText_TalkSubTM MSG_FACE
	release
	end

.global EventScript_Koga
EventScript_Koga:
	lock
	checkflag 0x824
	if 0x1 _goto EventScript_KogaBeat
	trainerbattle1 0x1 0x1A2 0x0 0x819832E 0x8198444 0x816D580
	release
	end

EventScript_KogaBeat:
	checkflag 0x827
	if equal _goto EventScript_KogaRematch
	msgbox 0x8198481 MSG_KEEPOPEN 
	release
	end

EventScript_KogaRematch:
	checkflag FLAG_KOGA_REMATCH_BEAT
	if 0x1 _goto EventScript_KogaRematchDone
	msgbox gText_KogaBeforeRematch MSG_FACE
	goto EventScript_DoBattleKoga
	release
	end

EventScript_DoBattleKoga:
	trainerbattle3 0x1 0x11 0x0 gText_KogaRematchLose
	msgbox gText_KogaRematchAfter1 MSG_FACE
	giveitem ITEM_TM103 0x1 MSG_OBTAIN
	giveitem ITEM_BLACK_SLUDGE 0x3 MSG_OBTAIN
	giveitem ITEM_POISON_BARB 0x3 MSG_OBTAIN
	setflag FLAG_KOGA_REMATCH_BEAT
	goto EventScript_KogaRematch
	release
	end

EventScript_KogaRematchDone:
	msgbox gText_KogaRematchAfter2 MSG_FACE
	release
	end

.global EventScript_FuschiaMapScript
EventScript_FuschiaMapScript:
	mapscript MAP_SCRIPT_ON_TRANSITION EventScript_SetWorldMapFlagAndOtherFuschia
	mapscript MAP_SCRIPT_ON_LOAD EventScript_HideMayIfNeededFuschia
	.byte MAP_SCRIPT_TERMIN

EventScript_HideMayIfNeededFuschia:
	checkflag 0x964
	if NOT_SET _goto EventScript_FuschiaHideMay
	release
	end

EventScript_FuschiaHideMay:
	setflag 0x963
	hidesprite 17
	release
	end

EventScript_SetWorldMapFlagAndOtherFuschia:
	setworldmapflag 0x897
	checkflag 0x272
	if 0x1 _call EventScript_0x272_1
	checkflag 0x272
	if 0x0 _call EventScript_0x272_2
	end

EventScript_0x272_1:
	setvar 0x4010 0x76
	return

EventScript_0x272_2:
	setvar 0x4010 0x93
	return

.global EventScript_FuschiaTutor
EventScript_FuschiaTutor:
	lock
	faceplayer 
	msgbox gText_ImAMoveTutor MSG_YESNO
	compare LASTRESULT 0x1
	if equal _goto EventScript_SelectFuschiaTutor
	goto EventScript_ComeBackFuschiaTutor
	release
	end
EventScript_ComeBackFuschiaTutor:
	msgbox gText_EverInNeedForAMove MSG_FACE
	release
	end

EventScript_SelectFuschiaTutor:
	preparemsg gText_SelectMoveFuschia
	waitmsg
	setvar 0x8004 0x0
	setvar 0x8000 0x15
	setvar 0x0001 0x6
	special 0x158
	waitstate 
	switch LASTRESULT
	case 0, IronHead
	case 1, SeedBomb
	case 2, DualChop
	case 3, GunkShot
	case 4, LastResort
	case 5, AquaTail
	case 6, StompingTantrum
	case 7, BreakingSwipe
	case 8, RazorShell
	goto EventScript_ComeBackFuschiaTutor
	release
	end

IronHead:
	setvar 0x8005 25	
	goto EventScript_FuschiaTeachMove
	release
	end
SeedBomb:
	setvar 0x8005 32	
	goto EventScript_FuschiaTeachMove
	release
	end
DualChop:
	setvar 0x8005 49	
	goto EventScript_FuschiaTeachMove
	release
	end
GunkShot:
	setvar 0x8005 48	
	goto EventScript_FuschiaTeachMove
	release
	end
LastResort:
	setvar 0x8005 56	
	goto EventScript_FuschiaTeachMove
	release
	end
AquaTail:
	setvar 0x8005 26	
	goto EventScript_FuschiaTeachMove
	release
	end
StompingTantrum:
	setvar 0x8005 46	
	goto EventScript_FuschiaTeachMove
	release
	end
BreakingSwipe:
	setvar 0x8005 98	
	goto EventScript_FuschiaTeachMove
	release
	end
RazorShell:
	setvar 0x8005 99	
	goto EventScript_FuschiaTeachMove
	release
	end

EventScript_FuschiaTeachMove:
	special 0x18D
	waitstate
	release
	end

.global EventScript_OldAmberRestore
EventScript_OldAmberRestore:
	lock
	faceplayer
	msgbox gText_MayIOldAmber MSG_YESNO
	compare LASTRESULT 0x1
	if equal _goto EventScript_CheckOldAmber
	release
	end

EventScript_CheckOldAmber:
	checkitem ITEM_OLD_AMBER 0x1
	compare LASTRESULT 0x1
	if lessthan _goto EventScript_NoOldAmber
	goto EventScript_GiveAerodactyl
	release
	end

EventScript_NoOldAmber:
	msgbox gText_YouDontHaveOldAmber MSG_FACE
	release
	end

EventScript_GiveAerodactyl:
	msgbox gText_HandOverOldAmber MSG_FACE
	removeitem ITEM_OLD_AMBER 0x1
	msgbox gText_HandedOver MSG_FACE
	msgbox gText_USeMachineAmber MSG_FACE
	pause 0x40
	msgbox gText_HereYouGoAmber MSG_FACE
	givepokemon SPECIES_AERODACTYL 55 0x0 0x0 0x0
	fanfare 0x101
	msgbox gText_RecievedAero MSG_FACE
	waitfanfare
	release
	end

.global EventScript_HintdifferentType
EventScript_HintdifferentType:
	lock
	msgbox gText_DifferentParty MSG_FACE
	release
	end
