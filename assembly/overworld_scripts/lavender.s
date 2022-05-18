.thumb
.align 2

.include "../xse_commands.s"
.include "../xse_defines.s"
.include "../asm_defines.s"

.equ FLAG_BEAT_ACEROLA, 0x9BB

.global EventScript_MaxIVs
EventScript_MaxIVs:
	lock
	faceplayer
	showmoney 0x14 0x0
	msgbox gText_ChangeIVsHello MSG_YESNO
	compare LASTRESULT 0x1
	if 0x1 _goto EventScript_SelectPokemonToMaxIVs
	msgbox gText_SeeYouNextTimeMaxIVS MSG_FACE
	hidemoney 0x14 0x0
	release
	end

EventScript_SelectPokemonToMaxIVs:
	special 0x9F 
    waitstate 
    compare 0x8004 0x6
    if greaterorequal _goto EventScript_ComeBackSoon
	special 0x148
	compare LASTRESULT 0x1
	if 0x1 _goto EventScript_JustAnEgg
    goto EventScript_MaxIVsYes
	release
	end

EventScript_MaxIVsYes:
	showmoney 0x14 0x0
	callasm GetSpeciesNumFromParty
	copyvar 0x4001 0x8008
	bufferpokemon 0x0 0x4001
	setvar 0x8000 0xA
    setvar 0x8001 0x6
    copyvar 0x8007 0x8004
    setvar 0x8004 0x0 
    preparemsg gText_SelectIVs
    waitmsg
    special 0x158
    waitstate 
    copyvar 0x8004 0x8007  
	compare LASTRESULT 0x6 @Num natures
	if greaterorequal _goto EventScript_ComeBackSoon
	copyvar 0x8005 LASTRESULT
	callasm MaxMonIV
	removemoney 0x1388
	updatemoney 0x14 0x0
	goto EventScript_Bye
	end

EventScript_ComeBackSoon:
	msgbox gText_NatureChangerComeBackSoon MSG_KEEPOPEN
	closeonkeypress
	hidemoney 0x14 0x0
	release
	end

EventScript_Bye:
	msgbox gText_IVChangerBye MSG_KEEPOPEN
	closeonkeypress
	hidemoney 0x14 0x0
	release
	end

EventScript_NotEnough:
	msgbox gText_IVMaxerNotEnough MSG_KEEPOPEN
	closeonkeypress
	hidemoney 0x14 0x0
	release
	end

EventScript_JustAnEgg:
	msgbox gText_ThatsAnEgg MSG_FACE
	goto EventScript_SelectPokemonToMaxIVs
	release
	end

.global EventScript_Acerola
EventScript_Acerola:
	lock
	msgbox gText_AcerolaHi MSG_FACE
	trainerbattle3 0x1 0x22 0x0 gText_AcerolaLose
	msgbox gText_AcerolaBye MSG_FACE
	giveitem ITEM_GENGARITE 0x1 MSG_OBTAIN
	msgbox gText_AcerolaBye2 MSG_FACE
	applymovement 0xFF EventScript_PlayerMoveAcerola
	waitmovement 0x0
	applymovement 6 EventScript_AcerolaMove
	waitmovement 0x0
	hidesprite 6
	setflag FLAG_BEAT_ACEROLA
	release
	end

EventScript_PlayerMoveAcerola:
	.byte look_left
	.byte walk_left
	.byte look_right
	.byte 0xFE

EventScript_AcerolaMove:
	.byte walk_down
	.byte walk_down
	.byte walk_down
	.byte walk_down
	.byte walk_down
	.byte walk_down
	.byte walk_down
	.byte walk_down
	.byte walk_down
	.byte 0xFE

.global EventScript_BodySlamTutor
EventScript_BodySlamTutor:
	lock
	faceplayer
	msgbox gText_BodySlamTutorAsk MSG_YESNO
	compare LASTRESULT 0x1
	if equal _goto EventScript_TeachBodySlam
	release

EventScript_TeachBodySlam:
	msgbox gText_WhichBodySlam MSG_FACE
	setvar 0x8005 91
	special 0x18D
	waitstate
	compare LASTRESULT 0x0
	if equal _goto EventScript_ComeBackBodySlam
	release
	end

EventScript_ComeBackBodySlam:
	msgbox gText_NoBodySlam MSG_FACE
	release
	end

.global EventScript_UtilityTutor
EventScript_UtilityTutor:
	lock
	faceplayer
	msgbox gText_UtilityTutor MSG_YESNO
	compare LASTRESULT 0x1
	if equal _goto EventScript_TeachUtility
	release
	end
EventScript_TeachUtility:
	preparemsg gText_WhichMoveUtility
	waitmsg
	setvar 0x8004 0x0
	setvar 0x8000 0x17
	setvar 0x0001 0x6
	special 0x158
	waitstate 
	switch LASTRESULT
	case 0, PainSplit
	case 1, Tailwind
	case 2, Trick
	case 3, WonderRoom
	case 4, MagicRoom
	release
	end

PainSplit:
	setvar 0x8005 27	
	goto EventScript_TeachUtilityMove
	release
	end
Tailwind:
	setvar 0x8005 28	
	goto EventScript_TeachUtilityMove
	release
	end
Trick:
	setvar 0x8005 34	
	goto EventScript_TeachUtilityMove
	release
	end
WonderRoom:
	setvar 0x8005 38
	goto EventScript_TeachUtilityMove
	release
	end
MagicRoom:
	setvar 0x8005 37	
	goto EventScript_TeachUtilityMove
	release
	end
EventScript_TeachUtilityMove:	
	special 0x18D
	waitstate
	compare LASTRESULT 0x0
	if equal _goto EventScript_End
	msgbox gText_HopeYouEnjoyMove MSG_FACE
	release
	end

EventScript_End:
	release
	end