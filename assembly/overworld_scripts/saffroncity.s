.thumb
.align 2

.include "../xse_commands.s"
.include "../xse_defines.s"
.include "../asm_defines.s"

.equ rotom_determine_special_move, 0x8C01228
.equ rotom_learn_special_move_func, 0x8C01288
.equ rotom_change, 0x8C01228
.equ store_move_to_var8003_from_var8005_in_slot_var8004, 0x8C012B4
.equ FLAG_BEAT_JASMINE, 0x9E6
.equ FLAG_GOT_ALAKAZITE, 0x9EF
.equ FLAG_SABRINA_REMATCH_BEAT, 0x990

.global EventScript_LadyTVStatic
EventScript_LadyTVStatic:
	checkflag 0x965
	if 0x1 _goto EventScript_LadyTVStaticOff
	msgbox gText_LadyTVStatic MSG_FACE
	release
	end

EventScript_LadyTVStaticOff:
	msgbox gText_LadyTVStaticOff MSG_FACE
	release
	end

.global EventScript_ManTVStatic
EventScript_ManTVStatic:
	checkflag 0x965
	if 0x1 _goto EventScript_ManTVStaticOff
	msgbox gText_ManTVStatic MSG_FACE
	release
	end

EventScript_ManTVStaticOff:
	msgbox gText_ManTVStaticOff MSG_FACE
	release
	end

.global EventScript_RotomTV1
EventScript_RotomTV1:
	lock
	checkflag 0x965
	if 0x0 _goto EventScript_RotomTVAsk
	msgbox gText_TV MSG_NORMAL
	release
	end

EventScript_RotomTVAsk:
	msgbox gText_RotomThump MSG_YESNO
	compare LASTRESULT 0x1
	if 0x1 _goto EventScript_RotomTV2
	release
	end

EventScript_RotomTV2:
	msgbox gText_RotomThumped MSG_NORMAL
	wildbattle SPECIES_ROTOM 0x41 0x0
	setflag 0x965
	msgbox gText_TVOn MSG_NORMAL
	release

EventScript_ComeBackSoon:
	msgbox gText_RotomComeBackSoon MSG_FACE
	release
	end

EventScript_NotARotom:
	msgbox gText_NotARotom MSG_FACE
	release
	end

.global EventScript_PoipoleGiver
EventScript_PoipoleGiver:
	lock
	faceplayer
	checkflag 0x966
	if equal _goto EventScript_HasPoipole
	msgbox gText_PoipoleGiverAsk MSG_YESNO
	compare LASTRESULT 0x1
	if 0x1 _goto EventScript_PoipoleGiverTalk
	release
	end

EventScript_HasPoipole:
	msgbox gText_HasPoipole MSG_FACE
	release
	end

EventScript_PoipoleGiverTalk:
	showpokepic SPECIES_POIPOLE 0xA 0x3
	msgbox gText_PoipoleGiverTalk MSG_YESNO
	hidepokepic
	compare LASTRESULT 0x1
	if 0x1 _goto EventScript_GivePoipole
	release
	end

EventScript_GivePoipole:
	givepokemon SPECIES_POIPOLE 0x41 0x0 0x0 0x0
	fanfare 0x13E
	msgbox gText_PoipoleGiverRecieve MSG_NORMAL
	waitfanfare
	setflag 0x966
	release
	end

.global EventScript_AshGreninjaSwap
EventScript_AshGreninjaSwap:
	lock
	faceplayer
	msgbox gText_GreninjaAsk MSG_YESNO
	compare LASTRESULT 0x1
	if 0x1 _goto EventScript_ChooseGreninja
	msgbox gText_ComeBackGreninja MSG_FACE
	release
	end

EventScript_ChooseGreninja:
	special 0x9F
	waitstate
	compare LASTRESULT 0x6
	if greaterorequal _goto EventScript_GreninjaComeBackSoon
	special2 LASTRESULT 0x18
	compare LASTRESULT SPECIES_GRENINJA
	if 0x1 _goto EventScript_GreninjaDoStuff
	msgbox gText_ThatsNoGreninja MSG_FACE
	release
	end

EventScript_GreninjaComeBackSoon:
	msgbox gText_ComeBackGreninja MSG_FACE
	release
	end

EventScript_GreninjaDoStuff:
	callasm SetGreninjaAbility
	compare 0x8008 0x0
	if equal _goto EventScript_GreninjaBattleBond
	compare 0x8008 0x1
	if equal _goto EventScript_GreninjaProtean
	release
	end

EventScript_GreninjaBattleBond:
	fanfare 0x13E
	msgbox gText_AbilityBattlebond MSG_FACE
	waitfanfare
	release
	end

EventScript_GreninjaProtean:
	fanfare 0x13E
	msgbox gText_AbilityProtean MSG_FACE
	waitfanfare
	release
	end

.global EventScript_BuyAbilityPatch
EventScript_BuyAbilityPatch:
	lock
	msgbox gText_BuyAbilityPatch MSG_FACE
	pokemart EventScript_AbilityMart
	release
	end

.global EventScript_EVReseter
EventScript_EVReseter:
	lock
	faceplayer
	msgbox gText_EVResetAsk MSG_YESNO
	compare LASTRESULT 0x1
	if 0x1 _goto EventScript_EVReseterSelectMon
	msgbox gText_EVResetComeBack MSG_FACE
	release
	end

EventScript_EVReseterSelectMon:
	setvar 0x8003 0x0
	msgbox gText_EVResetAskMon MSG_FACE
	special 0x9F
	waitstate
	compare 0x8004 0x6
    if greaterorequal _goto EventScript_EVReseterComeBackSoon
	special 0x148
	compare LASTRESULT 0x1
	if 0x1 _goto EventScript_JustAnEgg
	goto EventScript_ResetEVS
	release
	end

EventScript_ResetEVS:
	@@special 0xF
	@@callasm CalculateMonStats
	callasm ResetEVs
	msgbox gText_EVReset MSG_FACE
	release
	end

EventScript_JustAnEgg:
	msgbox gText_ThatsAnEgg2 MSG_FACE
	goto EventScript_EVReseterSelectMon
	release
	end

EventScript_EVReseterComeBackSoon:
	msgbox gText_EVResetComeBack MSG_FACE
	release
	end

EventScript_EggMoveTeacher:
	lock
	faceplayer
	msgbox gText_EggMoveHi MSG_YESNO
	compare LASTRESULT 0x1
	if 0x1 _goto EventScript_EggMoveTeacher2
	release
	end

EventScript_EggMoveTeacher2:
	setflag 0x917
	goto EventScript_AIOMan_0X1716BE
	clearflag 0x917
	release
	end

EventScript_AIOMan_0X1716BE:
	msgbox gText_AIOMan_0X1A2CC3 MSG_KEEPOPEN @"Which Pok�mon needs tutoring?"
	special 0xDB
	waitstate
	compare 0x8004 0x6
	if 0x4 _goto EventScript_AIOMan_0X803C71
	special 0x148
	compare LASTRESULT 0x1
	if 0x1 _goto EventScript_AIOMan_0X171790
	compare 0x8005 0x0
	if 0x1 _goto EventScript_AIOMan_0X171782
	goto EventScript_AIOMan_0X1716F4

EventScript_AIOMan_0X803C71:
	msgbox gText_AIOMan_0X8034B1 MSG_FACE @"I hope to see you again!"
	clearflag 0x917
	release
	end

EventScript_AIOMan_0X171790:
	msgbox gText_AIOMan_0X1A2E20 MSG_KEEPOPEN @"Hunh? There isn@t a single move\nt..."
	goto EventScript_AIOMan_0X1716BE

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
	clearflag 0x917
	end

	@---------------
EventScript_AIOMan_0X803590:
	msgbox gText_AIOMan_0X19951F MSG_KEEPOPEN @"What?\nNo Egg should know any move..."
	release
	goto EventScript_AIOMan_0X803C71
	clearflag 0x917

.global EventScript_RotomFormChanger
EventScript_RotomFormChanger:
	lock
	msgbox gText_RotomBoxes MSG_FACE
	callasm IsRotom
	compare LASTRESULT 0x1
	if equal _goto EventScript_RotomSelect
	release
	end

EventScript_RotomSelect:
	msgbox gText_RotomInvestigate MSG_YESNO
	compare LASTRESULT 0x1
	if equal _goto EventScript_ChangeRotom
	release
	end

EventScript_ChangeRotom:
	preparemsg gText_SelectMotor
	waitmsg
	setvar 0x8004 0x0
	setvar 0x8000 0xD
	setvar 0x0001 0x6
	special 0x158
	waitstate 
	compare LASTRESULT 0x6
	if greaterorequal _goto EventScript_End
	fanfare 0x101
	callasm SetRotomFormAndMove
	compare 0x800A 0x1
	if equal _goto EventScript_RevertRotom
	msgbox gText_RotomEntered MSG_FACE
	waitfanfare
	copyvar 0x8004 0x8006
	special 0x16
	callasm ForgetSpecialMove
	special 0xDD
	callasm Has4Moves
	compare LASTRESULT 0x1
	if equal _goto EventScript_ReplaceMove
	callasm TeachMove
	bufferattack 0x0 0x8008
	fanfare 0x101
	msgbox gText_RotomLearned MSG_FACE
	waitfanfare
	release
	end

EventScript_ReplaceMove:
	bufferattack 0x0 0x8008
	msgbox gText_LearnSpace MSG_YESNO
	compare LASTRESULT 0x0
	if equal _goto EventScript_RevertRotom
	special 0xDC
	waitstate
	compare 0x8005 0x3
	if 0x2 _goto EventScript_End
	bufferattack 0x1 0x8005
	msgbox gText_RotomForgot MSG_FACE
	special 0xDD
	callasm TeachMove
	fanfare 0x101
	msgbox gText_RotomLearned2 MSG_FACE
	waitfanfare
	release
	end

EventScript_RevertRotom:
	copyvar 0x8004 0x8006
	callasm ForgetSpecialMove
	special 0xDD
	setvar 0x8006 0x0
	callasm TeachMove
	setvar 0x8005 SPECIES_ROTOM
	special 0x16
	msgbox gText_RotomEmerged MSG_FACE
	release
	end

EventScript_End:
	release
	end

.global EventScript_PikachuFormChanger
EventScript_PikachuFormChanger:
	lock
	faceplayer
	msgbox gText_WantChangePikachu MSG_YESNO
	compare LASTRESULT 0x1
	if equal _goto EventScript_ChoosePikachu
	release
	end

EventScript_ChoosePikachu:
	special 0x9F
	waitstate 
	compare LASTRESULT 0x6
	if greaterorequal _goto EventScript_EndPikachu
	setvar 0x8003 0x0 
	special2 LASTRESULT 0x18
	compare LASTRESULT SPECIES_PIKACHU
	if equal _goto EventScript_PikachuChangeForm
	compare LASTRESULT SPECIES_PIKACHU_SURFING
	if equal _goto EventScript_PikachuChangeForm
	compare LASTRESULT SPECIES_PIKACHU_FLYING
	if equal _goto EventScript_PikachuChangeForm
	compare LASTRESULT SPECIES_PIKACHU_COSPLAY
	if equal _goto EventScript_PikachuChangeForm
	compare LASTRESULT SPECIES_PIKACHU_LIBRE
	if equal _goto EventScript_PikachuChangeForm
	compare LASTRESULT SPECIES_PIKACHU_POP_STAR
	if equal _goto EventScript_PikachuChangeForm
	compare LASTRESULT SPECIES_PIKACHU_ROCK_STAR
	if equal _goto EventScript_PikachuChangeForm
	compare LASTRESULT SPECIES_PIKACHU_BELLE
	if equal _goto EventScript_PikachuChangeForm
	compare LASTRESULT SPECIES_PIKACHU_PHD
	if equal _goto EventScript_PikachuChangeForm
	msgbox gText_ThatsNoPikachu MSG_FACE
	release
	end

EventScript_PikachuChangeForm:
	preparemsg gText_SelectPikachuForm
	waitmsg
	copyvar 0x800B 0x8004
	copyvar 0x8006 0x8004
	setvar 0x8004 0x0
	setvar 0x8000 0xE
	setvar 0x0001 0x6
	special 0x158
	waitstate 
	compare LASTRESULT 0x8
	if greaterorequal _goto EventScript_End
	fanfare 0x101
	callasm SetPikachuFormAndMove
	compare 0x800A 0x1
	if equal _goto EventScript_RevertPikachu
	msgbox gText_PikachuChangedForms MSG_FACE
	waitfanfare
	copyvar 0x8004 0x8006
	special 0x16
	callasm ForgetSpecialMovePikachu
	special 0xDD
	callasm Has4Moves
	compare LASTRESULT 0x1
	if equal _goto EventScript_ReplaceMovePikachu
	callasm TeachMove
	bufferattack 0x0 0x8008
	fanfare 0x101
	msgbox gText_PikachuLearned MSG_FACE
	waitfanfare
	release
	end

EventScript_EndPikachu:
	msgbox gText_ComeBackPikachu MSG_FACE
	release
	end

EventScript_RevertPikachu:
	copyvar 0x8004 0x800B
	callasm ForgetSpecialMovePikachu
	special 0xDD
	setvar 0x8006 0x0
	callasm TeachMove
	setvar 0x8005 SPECIES_PIKACHU
	special 0x16
	msgbox gText_PikachuChangedBack MSG_FACE
	release
	end

EventScript_ReplaceMovePikachu:
	bufferattack 0x0 0x8008
	msgbox gText_LearnSpacePikachu MSG_YESNO
	compare LASTRESULT 0x0
	if equal _goto EventScript_RevertPikachu
	special 0xDC
	waitstate
	compare 0x8005 0x3
	if 0x2 _goto EventScript_End
	bufferattack 0x1 0x8005
	msgbox gText_PikachuForgot MSG_FACE
	special 0xDD
	callasm TeachMove
	fanfare 0x101
	msgbox gText_PikachuLearned2 MSG_FACE
	waitfanfare
	release
	end

.global EventScript_FightingDojoMons
EventScript_FightingDojoMonThroh:
	lock
	faceplayer
	checkflag 0x278
	if 0x1 _goto 0x816EC3C
	showpokepic SPECIES_THROH 0xA 0x3
	setvar 0x4001 SPECIES_THROH
	applymovement 0x5 0x81A75E9
	waitmovement 0x0
	textcolor 0x0
	msgbox gText_WantThroh MSG_YESNO
	call 0x81A6675
	compare LASTRESULT 0x1
	if 0x1 _goto EventScript_GiveSawk
	hidepokepic
	release
	end

EventScript_FightingDojoMonSawk:
	lock
	faceplayer
	checkflag 0x278
	if 0x1 _goto 0x816EC3C
	showpokepic SPECIES_SAWK 0xA 0x3
	setvar 0x4001 SPECIES_SAWK
	applymovement 0x5 0x81A75E9
	waitmovement 0x0
	textcolor 0x0
	msgbox gText_WantSawk MSG_YESNO 
	call 0x81A6675
	compare LASTRESULT 0x1
	if 0x1 _goto EventScript_GiveSawk
	hidepokepic
	release
	end

EventScript_GiveSawk:
	hidepokepic
	givepokemon 0x4001 55 0x0 0x0 0x0 0x0
	compare LASTRESULT 0x0
	if 0x1 _goto 0x816ECB5
	compare LASTRESULT 0x1
	if 0x1 _goto 0x816ECEC
	compare LASTRESULT 0x2
	if 0x1 _goto 0x81A927C
	release
	end

.global EventScript_GiveThunderWaveTM
EventScript_GiveThunderWaveTM:
	lock
	faceplayer
	checkflag 0x9CF
	if SET _goto EventScript_TalkThunderWave
	msgbox gText_GiveThunderWaveTM MSG_YESNO
	compare LASTRESULT 0x1
	if equal _goto EventScript_GiveThunderWaveTMReal @@my weird naming scheme
	msgbox gText_ThunderWaveNO MSG_FACE
	release
	end

EventScript_GiveThunderWaveTMReal:
	giveitem ITEM_TM73 0x1 MSG_OBTAIN
	setflag 0x9CF
	goto EventScript_TalkThunderWave
	release
	end

EventScript_TalkThunderWave:
	msgbox gText_ThunderWaveTalk MSG_FACE
	release
	end

.global EventScript_LaprasGuy
EventScript_LaprasGuy:
	lock
	faceplayer
	checkflag 0x246
	if 0x1 _goto 0x8161B8D
	msgbox 0x8176266 MSG_KEEPOPEN 
	setvar 0x4001 0x83
	givepokemon 0x83 55 0x0 0x0 0x0 0x0
	compare LASTRESULT 0x0
	if 0x1 _goto 0x8161B12
	compare LASTRESULT 0x1
	if 0x1 _goto 0x8161B45
	compare LASTRESULT 0x2
	if 0x1 _goto 0x81A927C
	release
	end

.global EventScript_JasmineAceTrainer
EventScript_JasmineAceTrainer:
	lock
	faceplayer
	checkflag FLAG_BEAT_JASMINE
	if SET _goto EventScript_ExplainTrickRoom
	msgbox gText_JasmineBefore MSG_YESNO
	compare LASTRESULT 0x1
	if equal _goto EventScript_BattleJasmine
	msgbox gText_JasmineNo MSG_FACE
	release
	end

EventScript_BattleJasmine:
	trainerbattle3 0x1 57 0x0 gText_JasmineLose
	msgbox gText_JasmineHereTM MSG_FACE
	giveitem ITEM_TM116 0x1 MSG_OBTAIN
	setflag FLAG_BEAT_JASMINE
	goto EventScript_ExplainTrickRoom
	release
	end

EventScript_ExplainTrickRoom:
	msgbox gText_ExplainTrickRoom MSG_FACE
	release
	end

.global EventScript_MrPsychic
EventScript_MrPsychic:
	lock
	faceplayer
	checkflag 0x245
	if 0x1 _goto EventScript_HaveTM29
	msgbox 0x819C142 MSG_KEEPOPEN 
	checkitemspace 0x13D 0x1
	compare LASTRESULT 0x0
	if 0x1 _goto 0x816F0AB
	additem 0x13D 0x1
	loadpointer 0x0 0x819C16C
	giveitemwithfanfare 0x13D 0x1 0x101
	msgbox gText_UWantedThisMrPsychic MSG_FACE
	checkitemspace ITEM_ALAKAZITE 0x1
	compare LASTRESULT 0x0
	if 0x1 _goto 0x816F0AB
	additem ITEM_ALAKAZITE 0x1
	loadpointer 0x0 gText_GotAlakazite
	giveitemwithfanfare ITEM_ALAKAZITE 0x1 0x101
	msgbox 0x819C18F MSG_KEEPOPEN
	setflag 0x245
	release
	end

EventScript_HaveTM29:
	checkflag FLAG_GOT_ALAKAZITE
	if NOT_SET _goto EventScript_GiveAlakazite
	msgbox 0x819C18F MSG_KEEPOPEN 
	release
	end

EventScript_GiveAlakazite:
	msgbox 0x819C142 MSG_KEEPOPEN
	checkitemspace ITEM_ALAKAZITE 0x1
	compare LASTRESULT 0x0
	if 0x1 _goto 0x816F0AB
	additem ITEM_ALAKAZITE 0x1
	loadpointer 0x0 gText_GotAlakazite
	giveitemwithfanfare ITEM_ALAKAZITE 0x1 0x101
	setflag FLAG_GOT_ALAKAZITE
	msgbox gText_AlakaziteExplain MSG_FACE
	release
	end

.global EventScript_Sabrina
EventScript_Sabrina:
	lock
	checkflag 0x825
	if 0x1 _goto EventScript_SabrinaBeat
	trainerbattle1 0x1 0x1A4 0x0 0x819B53C 0x819B602 0x816EE0A
	release
	end

EventScript_SabrinaBeat:
	checkflag 0x827
	if equal _goto EventScript_SabrinaRematch
	msgbox 0x819B7F2 MSG_KEEPOPEN
	release
	end

EventScript_SabrinaRematch:
	checkflag FLAG_SABRINA_REMATCH_BEAT
	if 0x1 _goto EventScript_SabrinaRematchDone
	msgbox gText_SabrinaBeforeRematch MSG_FACE
	goto EventScript_DoBattleSabrina
	release
	end

EventScript_DoBattleSabrina:
	trainerbattle3 0x1 0x12 0x0 gText_SabrinaRematchLose
	msgbox gText_SabrinaRematchAfter1 MSG_FACE
	giveitem ITEM_TM104 0x1 MSG_OBTAIN
	giveitem ITEM_CHOICE_BAND 0x1 MSG_OBTAIN
	giveitem ITEM_CHOICE_SPECS 0x1 MSG_OBTAIN
	giveitem ITEM_CHOICE_SCARF 0x1 MSG_OBTAIN
	giveitem ITEM_TWISTED_SPOON 0x1 MSG_OBTAIN
	setflag FLAG_SABRINA_REMATCH_BEAT
	goto EventScript_SabrinaRematchDone
	release
	end

EventScript_SabrinaRematchDone:
	msgbox gText_SabrinaRematchAfter2 MSG_FACE
	release
	end

.global EventScript_BlockKubfu
EventScript_BlockKubfu:
	lock
	checkflag 0x827
	if SET _goto EventScript_EnoughBadgesKubfu
	msgbox gText_ScramKubfu MSG_FACE
	release
	end

EventScript_EnoughBadgesKubfu:
	msgbox gText_GoThroughKubfu MSG_FACE
	fadescreen 0x1
	hidesprite 9
	setflag 0x1006
	fadescreen 0x0
	release
	end

.global EventScript_KubfuBall
EventScript_KubfuBall:
	lock
	cry SPECIES_KUBFU 0x0
	showpokepic SPECIES_KUBFU 0xA 0x3
	waitcry
	msgbox gText_KubfuBall MSG_YESNO
	compare LASTRESULT 0x1
	if equal _goto EventScript_GiveKubfu
	hidepokepic
	release
	end

EventScript_GiveKubfu:
	hidepokepic
	bufferpokemon 0x0 SPECIES_KUBFU
	setflag 0x1007
	hidesprite 10
	givepokemon SPECIES_KUBFU 80 0x0 0x0 0x0 0x0
	fanfare 0x101
	preparemsg gText_PlayerObtainedKubfu
	waitmsg
	waitfanfare
	release
	end

.global EventScript_GalarFossilSeller
EventScript_GalarFossilSeller:
	lock
	msgbox gText_GalarFossil MSG_YESNO
	compare LASTRESULT 0x1
	if equal _goto EventScript_FossilListGalar
	release
	end

EventScript_FossilListGalar:
    preparemsg gText_WhichFossilGalar
    waitmsg
	hidepokepic
	setvar 0x8004 0x0
	multichoiceoption sDracozolt 0
	multichoiceoption sDracovish 1
	multichoiceoption sArctozolt 2
	multichoiceoption sArctovish 3
	multichoiceoption sNoThanks 4
	multichoice 0x0 0x0 FIVE_MULTICHOICE_OPTIONS 0x0
	switch LASTRESULT
	case 0, EventScript_BuyDraczolt
	case 1, EventScript_BuyDracovish
	case 2, EventScript_BuyArctozolt
	case 3, EventScript_BuyArctovish
	case 4, EventScript_NoFossilGalar
	goto EventScript_NoFossilGalar
	release
	end

EventScript_BuyDraczolt:
	showpokepic SPECIES_DRACOZOLT 0xA 0x3
	bufferpokemon 0x0 SPECIES_DRACOZOLT
	setvar 0x4001 SPECIES_DRACOZOLT
	msgbox gText_SoBuyBuffer1 MSG_YESNO
	compare LASTRESULT 0x0
	if equal _goto EventScript_FossilListGalar
	goto EventScript_CheckMoneyAndGiveFossil
	release
	end

EventScript_BuyDracovish:
	showpokepic SPECIES_DRACOVISH 0xA 0x3
	bufferpokemon 0x0 SPECIES_DRACOVISH
	setvar 0x4001 SPECIES_DRACOVISH
	msgbox gText_SoBuyBuffer1 MSG_YESNO
	compare LASTRESULT 0x0
	if equal _goto EventScript_FossilListGalar
	goto EventScript_CheckMoneyAndGiveFossil
	release
	end

EventScript_BuyArctozolt:
	showpokepic SPECIES_ARCTOZOLT 0xA 0x3
	bufferpokemon 0x0 SPECIES_ARCTOZOLT
	setvar 0x4001 SPECIES_ARCTOZOLT
	msgbox gText_SoBuyBuffer1 MSG_YESNO
	compare LASTRESULT 0x0
	if equal _goto EventScript_FossilListGalar
	goto EventScript_CheckMoneyAndGiveFossil
	release
	end

EventScript_BuyArctovish:
	showpokepic SPECIES_ARCTOVISH 0xA 0x3
	bufferpokemon 0x0 SPECIES_ARCTOVISH
	setvar 0x4001 SPECIES_ARCTOVISH
	msgbox gText_SoBuyBuffer1 MSG_YESNO
	compare LASTRESULT 0x0
	if equal _goto EventScript_FossilListGalar
	goto EventScript_CheckMoneyAndGiveFossil
	release
	end
	
EventScript_CheckMoneyAndGiveFossil:
	checkmoney 15000 0x0
	compare LASTRESULT 0x1
	if lessthan _goto EventScript_NotEnoughFossilGalar
	fanfare 0x101
	hidepokepic
	msgbox gText_PlayerRecievedBuffer1 MSG_FACE
	givepokemon 0x4001 65 0x0 0x0 0x0
	waitfanfare
	msgbox gText_HopeYOuEnjoyBuffer1 MSG_FACE
	release
	end


EventScript_NotEnoughFossilGalar:
	hidepokepic
	msgbox gText_NotEnoughBuyBuffer1 MSG_FACE
	release
	end

EventScript_NoFossilGalar:
	hidepokepic
	msgbox gText_NevermindGalarFossil MSG_FACE
	release
	end

.global EventScript_SaffronTutor
EventScript_SaffronTutor:
	lock
	faceplayer 
	msgbox gText_ImbestTutor MSG_YESNO
	compare LASTRESULT 0x1
	if equal _goto EventScript_SelectSaffronTutor
	goto EventScript_ComeBackSaffronTutor
	release
	end
EventScript_ComeBackSaffronTutor:
	msgbox gText_EverInNeedForAMove MSG_FACE
	release
	end

EventScript_SelectSaffronTutor:
	preparemsg gText_SelectMoveFuschia
	waitmsg
	setvar 0x8004 0x0
	setvar 0x8000 0x16
	setvar 0x0001 0x6
	special 0x158
	waitstate 
	switch LASTRESULT
	case 0, ZenHeadbutt
	case 1, FoulPlay
	case 2, Gravity
	case 3, Synthesis
	case 4, HeatWave
	case 5, BodyPress
	case 6, CrossPoison
	case 7, Hex
	case 8, DarkestLariat
	case 9, HighHorsepower
	case 10, LeafBlade
	case 11, MuddyWater
	goto EventScript_ComeBackSaffronTutor
	release
	end

ZenHeadbutt:
	setvar 0x8005 31	
	goto EventScript_SaffronTeachMove
	release
	end
FoulPlay:
	setvar 0x8005 41	
	goto EventScript_SaffronTeachMove
	release
	end
Gravity:
	setvar 0x8005 19
	goto EventScript_SaffronTeachMove
	release
	end
Synthesis:
	setvar 0x8005 17
	goto EventScript_SaffronTeachMove
	release
	end
HeatWave:
	setvar 0x8005 50	
	goto EventScript_SaffronTeachMove
	release
	end
BodyPress:
	setvar 0x8005 92	
	goto EventScript_SaffronTeachMove
	release
	end
CrossPoison:
	setvar 0x8005 106	
	goto EventScript_SaffronTeachMove
	release
	end
Hex:
	setvar 0x8005 100	
	goto EventScript_SaffronTeachMove
	release
	end
DarkestLariat:
	setvar 0x8005 108	
	goto EventScript_SaffronTeachMove
	release
	end
HighHorsepower:
	setvar 0x8005 109	
	goto EventScript_SaffronTeachMove
	release
	end
LeafBlade:
	setvar 0x8005 110	
	goto EventScript_SaffronTeachMove
	release
	end
MuddyWater:
	setvar 0x8005 111	
	goto EventScript_SaffronTeachMove
	release
	end
EventScript_SaffronTeachMove:
	special 0x18D
	waitstate
	release
	end

.global EventScript_SaffronTutor2
EventScript_SaffronTutor2:
	lock
	faceplayer 
	msgbox gText_ImAMoveTutor MSG_YESNO
	compare LASTRESULT 0x1
	if equal _goto EventScript_SelectSaffronTutor2
	goto EventScript_ComeBackSaffronTutor
	release
	end

EventScript_SelectSaffronTutor2:
	preparemsg gText_SelectMoveFuschia
	waitmsg
	setvar 0x8004 0x0
	setvar 0x8000 0x19
	setvar 0x0001 0x6
	special 0x158
	waitstate 
	switch LASTRESULT
	case 0, AirSlash
	case 1, BugBuzz
	case 2, BlazeKick
	case 3, MysticalFire
	case 4, PlayRough
	case 5, PollenPuff
	case 6, PowerGem
	case 7, PsychoCut
	case 8, StoredPower
	case 9, HyperVoice
	case 10, DragonDance
	case 11, Agility
	case 12, NastyPlot
	goto EventScript_ComeBackSaffronTutor
	release
	end

AirSlash:
	setvar 0x8005 102	
	goto EventScript_SaffronTeachMove
	release
	end
BugBuzz:
	setvar 0x8005 105	
	goto EventScript_SaffronTeachMove
	release
	end
BlazeKick:
	setvar 0x8005 104	
	goto EventScript_SaffronTeachMove
	release
	end
MysticalFire:
	setvar 0x8005 112	
	goto EventScript_SaffronTeachMove
	release
	end
PlayRough:
	setvar 0x8005 114	
	goto EventScript_SaffronTeachMove
	release
	end
PollenPuff:
	setvar 0x8005 115	
	goto EventScript_SaffronTeachMove
	release
	end
PowerGem:
	setvar 0x8005 116	
	goto EventScript_SaffronTeachMove
	release
	end
PsychoCut:
	setvar 0x8005 118	
	goto EventScript_SaffronTeachMove
	release
	end
StoredPower:
	setvar 0x8005 97	
	goto EventScript_SaffronTeachMove
	release
	end
HyperVoice:
	setvar 0x8005 51	
	goto EventScript_SaffronTeachMove
	release
	end
DragonDance:
	setvar 0x8005 74	
	goto EventScript_SaffronTeachMove
	release
	end
Agility:
	setvar 0x8005 75	
	goto EventScript_SaffronTeachMove
	release
	end
NastyPlot:
	setvar 0x8005 76	
	goto EventScript_SaffronTeachMove
	release
	end

EventScript_SaffronTeachMove2:
	special 0x18D
	waitstate
	release
	end

.align 2
EventScript_AbilityMart:
	.hword ITEM_ABILITY_CAPSULE
	.hword 0x2DC
	.hword 0x0
