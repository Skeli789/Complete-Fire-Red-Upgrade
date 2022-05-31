.thumb
.align 2

.include "../xse_commands.s"
.include "../xse_defines.s"
.include "../asm_defines.s"

.global EventScript_ButterfreeEnc
EventScript_ButterfreeEnc:
	lock
	checkflag 0x94F
	if 0x1 _goto EventScript_ButterfreeBattled
	msgbox gText_LookInsideButterfree MSG_YESNO
	compare LASTRESULT 0x1
	if 0x1 _goto EventScript_ButterfreeBattle
	release
	end

EventScript_ButterfreeBattle:
	msgbox gText_PokemonCameOut MSG_KEEPOPEN
	closeonkeypress
	setflag 0x919
	setflag 0x91A
	wildbattle 0x4D5 0xA 0x0
	setflag 0x94F
	clearflag 0x919
	clearflag 0x91A
	release
	end

EventScript_ButterfreeBattled:
	msgbox gText_AlreadyBattledButterfree MSG_KEEPOPEN
	closeonkeypress
	release
	end

.global EventScript_Volcarona
EventScript_Volcarona:
	lock
	cry SPECIES_VOLCARONA 0x0
	msgbox gText_VolcaronaCry MSG_KEEPOPEN
	wildbattle SPECIES_VOLCARONA 0x32 0x0
	hidesprite 0x800F
	setflag 0x9B9
	release
	end

.global EventScript_MayForest
EventScript_MayForestTile:
	lock
	checkitem ITEM_DYNAMAX_BAND 0x1
	compare LASTRESULT 0x1
    if lessthan _goto EventScript_MayBattle1
	release
	end

EventScript_MayBattle1:
	showsprite 0xC 
	applymovement 0xC EventScript_MayMovement
	waitmovement 0x0
	sound 0x15
	msgbox gText_MayBeforeBattle MSG_FACE
	trainerbattle3 0x1 0x3 0x0 gText_MayLose1
	msgbox gText_MayAfter1 MSG_FACE
	giveitem ITEM_DYNAMAX_BAND 0x1 MSG_OBTAIN
	msgbox gText_MayGtg MSG_FACE
	applymovement 0xC EventScript_MayMovement2
	waitmovement 0x0
	setflag 0x951
	hidesprite 0xC
	release
	end
	
EventScript_MayMovement:
	.byte walk_down
	.byte walk_down
    .byte walk_down
	.byte walk_down
	.byte walk_down
	.byte walk_down
	.byte walk_down
	.byte walk_down
	.byte 0x62
	.byte 0xFE

EventScript_MayMovement2:
	.byte walk_up
	.byte walk_up
	.byte walk_up
	.byte walk_up
	.byte walk_up
	.byte walk_up
	.byte walk_up
	.byte walk_up
	.byte 0xFE

.global EventScript_BugBiteTutor
EventScript_BugBiteTutor:
	lock
	faceplayer
	msgbox gText_AskWhyBugBite MSG_YESNO
	compare LASTRESULT 0x1
	if equal _goto EventScript_AskTeachBugBite
	msgbox gText_NoBugBite MSG_FACE
	release
	end
	

EventScript_AskTeachBugBite:
	msgbox gText_AskTeachBugBite MSG_YESNO
	compare LASTRESULT 0x1
	if equal _goto EventScript_TeachBugBite
	msgbox gText_NoBugBite MSG_FACE
	release
	end

EventScript_TeachBugBite:
	msgbox gText_BugBiteSelect MSG_FACE
	setvar 0x8005 0xD
	special 0x18D
	waitstate
	release
	end

.global EventScript_ElectrowebTutor
EventScript_ElectrowebTutor:
	lock
	faceplayer
	msgbox gText_ElectrowebAsk1 MSG_YESNO
	compare LASTRESULT 0x1
	if equal _goto EventScript_YesLikeCobwebs
	msgbox gText_ElectrowebNoCobweb MSG_FACE
	goto EventScript_ElectrowebAsk
	release
	end

EventScript_YesLikeCobwebs:
	msgbox gText_ElectrowebYesCobweb MSG_FACE
	goto EventScript_ElectrowebAsk
	release
	end

EventScript_ElectrowebAsk:
	msgbox gText_ElectrowebAsk2 MSG_YESNO
	compare LASTRESULT 0x1
	if equal _goto EventScript_TeachElectroweb
	msgbox gText_NoTeachWeb MSG_FACE
	release
	end

EventScript_TeachElectroweb:
	setvar 0x8005 5
	special 0x18D
	waitstate
	compare LASTRESULT 0x0
	if equal _goto EventScript_End
	msgbox gText_TaughtWeb MSG_FACE
	release
	end


EventScript_End:
	msgbox gText_NatureChangerComeBackSoon MSG_FACE
	release
	end

.global EventScript_FocusEnergyCeladon
EventScript_FocusEnergyCeladon:
	lock
	msgbox gText_FocusEnergyCeladon MSG_FACE
	release
	end
