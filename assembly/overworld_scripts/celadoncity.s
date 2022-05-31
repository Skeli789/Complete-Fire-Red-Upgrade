.thumb
.align 2

.include "../xse_commands.s"
.include "../xse_defines.s"
.include "../asm_defines.s"

.equ FLAG_BEAT_MALLOW, 0x995
.equ FLAG_ERIKA_REMATCH_BEAT, 0x98D
.equ FLAG_BEAT_DAVID, 0x9F0

.global EventScript_BattleItemsMart
EventScript_BattleItemsMart:
	lock
	msgbox gText_MartHi MSG_FACE
	pokemart EventScript_BattleItems
	msgbox gText_MartBye MSG_FACE
	release
	end

.global EventScript_MiscPokeballsMart
EventScript_MiscPokeballsMart:
	lock
	msgbox gText_MartHi MSG_FACE
	pokemart EventScript_MiscPokeballs
	msgbox gText_MartBye MSG_FACE
	release
	end

.global EventScript_EvoItemsMart
EventScript_EvoItemsMart:
	lock
	msgbox gText_MartHi MSG_FACE
	pokemart EventScript_EvoItems
	msgbox gText_MartBye MSG_FACE
	release
	end

.global EventScript_NatureChanger
EventScript_NatureChanger:
	lock
	msgbox gText_NatureChangerGreeting MSG_YESNO
	compare LASTRESULT 0x1
	if 0x1 _goto EventScript_SelectPokemonToChangeNature
	msgbox gText_NatureChangerNo MSG_FACE
	release
	end

EventScript_SelectPokemonToChangeNature:
	special 0x9F 
    waitstate 
    compare 0x8004 0x6
    if greaterorequal _goto EventScript_ComeBackSoon
	special 0x148
	compare LASTRESULT 0x1
	if 0x1 _goto EventScript_JustAnEgg
    goto EventScript_NatureChangerYes
	release
	end

EventScript_NatureChangerYes:
	setvar 0x8000 0x9
    setvar 0x8001 0x6
    copyvar 0x8007 0x8004
    setvar 0x8004 0x0 
    preparemsg gText_NatureChangerAsk
    waitmsg
    special 0x158
    waitstate 
    copyvar 0x8004 0x8007  
	compare LASTRESULT 0x25 @Num natures
	if greaterorequal _goto EventScript_ComeBackSoon
	copyvar 0x8005 LASTRESULT
	callasm SetChosenMonNature
	goto EventScript_Bye
	end


EventScript_Bye:
	msgbox gText_NatureChangerBye MSG_KEEPOPEN
	closeonkeypress
	release
	end

EventScript_ComeBackSoon:
	msgbox gText_NatureChangerComeBackSoon MSG_KEEPOPEN
	closeonkeypress
	release
	end

EventScript_JustAnEgg:
	msgbox gText_CantEggNature MSG_FACE
	goto EventScript_SelectPokemonToChangeNature
	release
	end

.global EventScript_BallChanger
EventScript_BallChanger:
	lock
	msgbox gText_AskBallChanger MSG_YESNO
	compare LASTRESULT 0x1
	if 0x1 _goto EventScript_BallChangerStart
	release
	end

EventScript_BallChangerStart:
	msgbox gText_BallSelectAPokemon MSG_FACE
	setvar 0x8003 0x0
	special 0x9F
	waitstate
	compare 0x8004 0x6
	if greaterorequal _goto EventScript_ComeBackSoon
	setvar 0x8000 0xB
    setvar 0x8001 0x6
    copyvar 0x8007 0x8004
    setvar 0x8004 0x0 
    preparemsg gText_BallChangerAsk
    waitmsg
    special 0x158
    waitstate 
	copyvar 0x8004 0x8007  
	compare LASTRESULT 0x1B @Num pokeballs
	if greaterorequal _goto EventScript_ComeBackSoon
	setvar 0x8003 0x0
	copyvar 0x8005 LASTRESULT
	callasm SetBallVar
	copyvar 0x4004 0x8006
	checkitem 0x4004 0x1
	compare LASTRESULT 0x1
	if lessthan _goto EventScript_NeedBall
	callasm PartyNumToSpecies
	copyvar 0x4006 0x800A
	bufferpokemon 0x0 0x4006
	bufferitem 0x1 0x4004
	callasm GetBallFromMon
	copyvar 0x4005 0x8009
	additem 0x4005 0x1
	removeitem 0x4004 0x1
	special 0x14
	msgbox gText_BallChanged MSG_FACE
	release
	end

EventScript_NeedBall:
	bufferitem 0x0 0x4004
	msgbox gText_DontHaveBall MSG_FACE
	release
	end

EventScript_LittleGirlPidgeotite:
	lock
	checkflag 0x962
	if SET _goto EventScript_LittleGirlTalk
	msgbox gText_LittleGirlFoundShiny MSG_YESNO
	compare LASTRESULT 0x1
	if 0x1 _goto EventScript_GivePidgeotite
	release
	end

EventScript_LittleGirlTalk:
	msgbox gText_LittleGirlTalk MSG_FACE
	release
	end

EventScript_GivePidgeotite:
	additem ITEM_PIDGEOTITE 0x1
	fanfare 0x13E
	msgbox gText_GetPidgeotite MSG_FACE
	waitfanfare
	setflag 0x962
	release
	end

.global EventScript_TMShop
EventScript_TMShop:
	lock
	msgbox gText_TMShopHi MSG_FACE
	pokemart EventScript_TMShop1
	msgbox gText_TMShopBye MSG_FACE
	release
	end

.global EventScript_Mallow
EventScript_Mallow:
	lock
	msgbox gText_MallowBefore MSG_NORMAL
	faceplayer
	applymovement 17 EventScript_MallowFace
	waitmovement 0x0
	sound 0x15
	msgbox gText_MallowHiThere MSG_FACE
	trainerbattle3 0x1 0x21 0x0 gText_MallowLose
	msgbox gText_MallowGiveTM MSG_FACE
	giveitem ITEM_TM86 0x1 MSG_OBTAIN
	msgbox gText_MallowBye MSG_FACE
	applymovement 0xFF EventScript_PlayerDown
	waitmovement 0x0
	applymovement 17 EventScript_MallowLeave
	waitmovement 0x0
	hidesprite 17
	setflag FLAG_BEAT_MALLOW
	release
	end

EventScript_MallowFace:
	.byte 0x62
	.byte 0xFE

EventScript_MallowLeave:
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

EventScript_PlayerDown:
	.byte look_down
	.byte walk_down
	.byte look_up
	.byte 0xFE

.global EventScript_Erika
EventScript_Erika:
	lock
	checkflag 0x823
	if 0x1 _goto EventScript_ErikaBeat
	trainerbattle1 0x1 0x1A1 0x0 0x8197114 0x8197260 0x816D0A0
	release
	end

EventScript_ErikaBeat:
	checkflag 0x827
	if equal _goto EventScript_ErikaRematch
	msgbox 0x81972B8 MSG_KEEPOPEN
	release
	end

EventScript_ErikaRematch:
	checkflag FLAG_ERIKA_REMATCH_BEAT
	if 0x1 _goto EventScript_ErikaRematchDone
	msgbox gText_ErikaBeforeRematch MSG_YESNO
	compare LASTRESULT 0x1
	if equal _goto EventScript_DoBattleErika
	msgbox gText_ErikaNoBattle MSG_FACE
	release
	end

EventScript_DoBattleErika:
	trainerbattle3 0x1 0x10 0x0 gText_ErikaRematchLose
	msgbox gText_ErikaRematchAfter1 MSG_FACE
	giveitem ITEM_ABOMASITE 0x1 MSG_OBTAIN
	giveitem ITEM_MIRACLE_SEED 0x3 MSG_OBTAIN
	setflag FLAG_ERIKA_REMATCH_BEAT
	release
	end

EventScript_ErikaRematchDone:
	msgbox gText_ErikaRematchAfter2 MSG_FACE
	release
	end

.global EventScript_David
EventScript_David:
	lock
	faceplayer
	checkflag FLAG_BEAT_DAVID
	if equal _goto EventScript_ExplainAltaria
	msgbox gText_DavidBefore MSG_YESNO
	compare LASTRESULT 0x1
	if equal _goto EventScript_DavidBattle
	msgbox gText_DavidNo MSG_FACE
	release
	end

EventScript_DavidBattle:
	trainerbattle3 0x1 59 0x0 gText_DavidLose
	msgbox gText_DavidGive MSG_FACE
	giveitem ITEM_ALTARIANITE 0x1 MSG_OBTAIN
	setflag FLAG_BEAT_DAVID
	goto EventScript_ExplainAltaria
	release
	end

EventScript_ExplainAltaria:
	msgbox gText_DavidExplain MSG_FACE
	release
	end

.global EventScript_FocusEnergyTutor
EventScript_FocusEnergyTutor:
	lock
	faceplayer
	msgbox gText_TeachFocusEnergy MSG_YESNO
	compare LASTRESULT 0x1
	if equal _goto EventScript_TeachFocusEnergy
	release
	end

EventScript_TeachFocusEnergy:
	setvar 0x8005 64
	special 0x18D
	waitstate
	release
	end

.global EventScript_HazardsTerrainTutor
EventScript_HazardsTerrainTutor:
	lock
	faceplayer
	msgbox gText_HazardTutor MSG_YESNO
	compare LASTRESULT 0x1
	if equal _goto EventScript_SelectHazardMove
	release
	end

EventScript_SelectHazardMove:
	preparemsg gText_WhichFangMove
	waitmsg
	setvar 0x8004 0x0
	setvar 0x8000 0x14
	setvar 0x0001 0x6
	special 0x158
	waitstate 
	compare LASTRESULT 6
	if greaterorequal _goto EventScript_NoTerrainTutor
	switch LASTRESULT
	case 0, Spikes
	case 1, TSpikes
	case 2, GTerrain
	case 3, MTerrain
	case 4, ETerrain
	case 5, PTerrain
	release
	end

Spikes:
	setvar 0x8005 72
	goto EventScript_TeachMove2
	release
	end
TSpikes:
	setvar 0x8005 73
	goto EventScript_TeachMove2
	release
	end
GTerrain:
	setvar 0x8005 77
	goto EventScript_TeachMove2
	release
	end
MTerrain:
	setvar 0x8005 78
	goto EventScript_TeachMove2
	release
	end
ETerrain:
	setvar 0x8005 79
	goto EventScript_TeachMove2
	release
	end
PTerrain:
	setvar 0x8005 80
	goto EventScript_TeachMove2
	release
	end

EventScript_TeachMove2:
	special 0x18D
	waitstate
	release
	end

EventScript_NoTerrainTutor:
	msgbox gText_ComeBackPowerful MSG_FACE
	release
	end

.global EventScript_PledgeTutor
EventScript_PledgeTutor:
	lock
	faceplayer
	msgbox gText_PledgeTutor MSG_YESNO
	compare LASTRESULT 0x1
	if equal _goto EventScript_PledgeTutorSelect
	goto EventScript_NoPledgeMove
	release
	end

EventScript_NoPledgeMove:
	msgbox gText_SeeYouNextTime MSG_FACE
	release
	end

EventScript_PledgeTutorSelect:
	preparemsg gText_WhatMoveLearn
	waitmsg
	multichoiceoption NAME_LONG_FIRE_PLEDGE 0
	multichoiceoption NAME_LONG_WATER_PLEDGE 1
	multichoiceoption NAME_LONG_GRASS_PLEDGE 2
	multichoiceoption sNoThanks 3
	multichoice 0x0 0x0 FOUR_MULTICHOICE_OPTIONS 0x0
	switch LASTRESULT
	case 0, FirePledge
	case 1, WaterPledge
	case 2, GrassPledge
	goto EventScript_NoPledgeMove
	release
	end

FirePledge:
	setvar 0x8005 59
	goto EventScript_TeachMove
	release
	end
WaterPledge:
	setvar 0x8005 60
	goto EventScript_TeachMove
	release
	end
GrassPledge:
	setvar 0x8005 58
	goto EventScript_TeachMove
	release
	end
EventScript_TeachMove:
	special 0x18D
	waitstate
	compare LASTRESULT 0x0
	if equal _goto EventScript_NoPledgeMove
	msgbox gText_YesPledge MSG_FACE
	release
	end

.global EventScript_SoftboiledTutor
EventScript_SoftboiledTutor:
	lock
	faceplayer
	msgbox 0x8180388 MSG_YESNO
	compare LASTRESULT 0x0
	if 0x1 _goto 0x81C4ADA
	@@call 0x81C4F37
	@@compare LASTRESULT 0x0
	@@if 0x1 _goto 0x81C4ADA
	msgbox 0x8180484 MSG_KEEPOPEN
	setvar 0x8005 146
	call 0x81C4F30
	compare LASTRESULT 0x0
	if 0x1 _goto 0x81C4ADA
	goto 0x81C4AE4
	release
	end

.global EventScript_TVRemake
EventScript_TVRemake:
	lock
	msgbox 0x81958D8 MSG_SIGN
	release
	end

.global EventScript_SlotMachine
EventScript_SlotMachine:
	lock
	msgbox gText_ItsASlotMachine MSG_FACE
	release
	end

.global EventScript_LiquidationTutor
EventScript_LiquidationTutor:
	lock
	faceplayer
	msgbox gText_LiquidationAsk MSG_YESNO
	compare LASTRESULT 0x1
	if equal _goto EventScript_TeachLiquidation
	release
	end

EventScript_TeachLiquidation:
	setvar 0x8005 39
	special 0x18D
	waitstate
	compare LASTRESULT 0x0
	if equal _goto EventScript_End1
	msgbox gText_LiquidationTaught MSG_FACE
	release
	end

EventScript_End1:
	release
	end

.align 1
EventScript_TMShop1:	
	.hword ITEM_TM10
	.hword ITEM_TM13
	.hword ITEM_TM15
	.hword ITEM_TM17
	.hword ITEM_TM23
	.hword ITEM_TM24
	.hword ITEM_TM30
	.hword ITEM_TM31
	.hword ITEM_TM35
	.hword ITEM_TM44
	.hword ITEM_TM53
	.hword ITEM_TM55
	.hword ITEM_TM59
	.hword ITEM_TM60
	.hword ITEM_TM61
	.hword ITEM_TM65
	.hword ITEM_TM66
	.hword ITEM_TM69
	.hword ITEM_TM71
	.hword ITEM_TM75
	.hword ITEM_TM76
	.hword ITEM_TM81
	.hword ITEM_TM83
	.hword ITEM_TM88
	.hword ITEM_TM89
	.hword ITEM_TM95
	.hword ITEM_TM96
	.hword ITEM_TM97
	.hword ITEM_TM102
	.hword ITEM_TM106
	.hword ITEM_TM107
	.hword ITEM_TM115
	.hword 0x0


EventScript_BattleItems:
	.hword ITEM_FIRE_GEM
	.hword ITEM_WATER_GEM
	.hword ITEM_ELECTRIC_GEM
	.hword ITEM_GRASS_GEM
	.hword ITEM_ICE_GEM
	.hword ITEM_FIGHTING_GEM
	.hword ITEM_POISON_GEM
	.hword ITEM_GROUND_GEM
	.hword ITEM_FLYING_GEM
	.hword ITEM_PSYCHIC_GEM
	.hword ITEM_BUG_GEM
	.hword ITEM_ROCK_GEM
	.hword ITEM_GHOST_GEM
	.hword ITEM_DRAGON_GEM
	.hword ITEM_DARK_GEM
	.hword ITEM_STEEL_GEM
	.hword ITEM_NORMAL_GEM
	.hword ITEM_FAIRY_GEM
	@@.hword ITEM_LEEK
	.hword ITEM_EVIOLITE
	.hword ITEM_LIGHT_BALL
	.hword ITEM_LUCKY_PUNCH
	.hword ITEM_METAL_POWDER
	.hword ITEM_QUICK_POWDER
	.hword ITEM_THICK_CLUB
	.hword ITEM_BLACK_BELT
	.hword ITEM_BLACK_GLASSES
	.hword ITEM_CHARCOAL
	.hword ITEM_DRAGON_FANG
	.hword ITEM_HARD_STONE
	.hword ITEM_MAGNET
	.hword ITEM_METAL_COAT
	.hword ITEM_MIRACLE_SEED
	.hword ITEM_MYSTIC_WATER
	.hword ITEM_NEVER_MELT_ICE
	.hword ITEM_POISON_BARB
	.hword ITEM_SHARP_BEAK
	.hword ITEM_SILK_SCARF
	.hword ITEM_SILVER_POWDER
	.hword ITEM_SOFT_SAND
	.hword ITEM_SPELL_TAG
	.hword ITEM_TWISTED_SPOON
	.hword ITEM_CHOICE_BAND
	.hword ITEM_CHOICE_SPECS
	.hword ITEM_CHOICE_SCARF
	.hword ITEM_ROCKY_HELMET
	.hword ITEM_LEFTOVERS
	.hword ITEM_BLACK_SLUDGE
	.hword ITEM_FLAME_ORB
	.hword ITEM_TOXIC_ORB
	.hword ITEM_LIFE_ORB
	.hword ITEM_STICKY_BARB
	.hword ITEM_RING_TARGET
	.hword ITEM_ASSAULT_VEST
	.hword ITEM_DAMP_ROCK
	.hword ITEM_HEAT_ROCK
	.hword ITEM_ICY_ROCK
	.hword ITEM_SMOOTH_ROCK
	.hword ITEM_GRIP_CLAW
	.hword ITEM_LIGHT_CLAY
	.hword ITEM_TERRAIN_EXTENDER
	.hword ITEM_AIR_BALLOON
	.hword ITEM_BINDING_BAND
	.hword ITEM_BLUNDER_POLICY
	.hword ITEM_WEAKNESS_POLICY
	.hword ITEM_EXPERT_BELT
	.hword ITEM_MUSCLE_BAND
	.hword ITEM_WISE_GLASSES
	.hword ITEM_HEAVY_DUTY_BOOTS
	.hword ITEM_MENTAL_HERB
	.hword ITEM_WHITE_HERB
	.hword ITEM_POWER_HERB
	.hword ITEM_EJECT_PACK
	.hword ITEM_EJECT_BUTTON
	.hword ITEM_KINGS_ROCK
	.hword ITEM_LAGGING_TAIL
	.hword ITEM_METRONOME
	.hword ITEM_GRASSY_SEED
	.hword ITEM_MISTY_SEED
	.hword ITEM_ELECTRIC_SEED
	.hword ITEM_PSYCHIC_SEED
	.hword ITEM_FOCUS_SASH
	.hword 0x0

EventScript_MiscPokeballs:
	.hword ITEM_FAST_BALL
	.hword ITEM_LEVEL_BALL
	.hword ITEM_LURE_BALL
	.hword ITEM_HEAVY_BALL
	.hword ITEM_LOVE_BALL
	.hword ITEM_FRIEND_BALL
	.hword ITEM_MOON_BALL
	.hword ITEM_NET_BALL
	.hword ITEM_NEST_BALL
	.hword ITEM_REPEAT_BALL
	.hword ITEM_TIMER_BALL
	.hword ITEM_LUXURY_BALL
	.hword ITEM_PREMIER_BALL
	.hword ITEM_DIVE_BALL
	.hword ITEM_DUSK_BALL
	.hword ITEM_HEAL_BALL
	.hword ITEM_QUICK_BALL
	.hword ITEM_CHERISH_BALL
	.hword ITEM_DREAM_BALL
	.hword 0x0

EventScript_EvoItems:
	.hword ITEM_POKE_DOLL
	.hword ITEM_RETRO_MAIL
	.hword ITEM_FIRE_STONE
	.hword ITEM_WATER_STONE
	.hword ITEM_THUNDER_STONE
	.hword ITEM_LEAF_STONE
	.hword ITEM_MOON_STONE
	.hword ITEM_SUN_STONE
	.hword ITEM_SHINY_STONE
	.hword ITEM_DUSK_STONE
	.hword ITEM_DAWN_STONE
	.hword ITEM_ICE_STONE
	.hword ITEM_LINK_CABLE
	.hword ITEM_DRAGON_SCALE
	.hword ITEM_KINGS_ROCK
	.hword ITEM_METAL_COAT
	.hword ITEM_UPGRADE
	.hword ITEM_DEEP_SEA_SCALE
	.hword ITEM_DEEP_SEA_TOOTH
	.hword ITEM_DUBIOUS_DISC
	.hword ITEM_ELECTIRIZER
	.hword ITEM_MAGMARIZER
	.hword ITEM_OVAL_STONE
	.hword ITEM_PROTECTOR
	.hword ITEM_RAZOR_CLAW
	.hword ITEM_RAZOR_FANG
	.hword ITEM_REAPER_CLOTH
	.hword ITEM_PRISM_SCALE
	.hword ITEM_SACHET
	.hword ITEM_WHIPPED_DREAM
	.hword ITEM_CHIPPED_POT
	.hword ITEM_CRACKED_POT
	.hword ITEM_GALARICA_CUFF
	.hword ITEM_GALARICA_WREATH
	.hword ITEM_SWEET_APPLE
	.hword ITEM_TART_APPLE
	.hword ITEM_STRAWBERRY_SWEET
	.hword ITEM_LOVE_SWEET
	.hword ITEM_BERRY_SWEET
	.hword ITEM_CLOVER_SWEET
	.hword ITEM_FLOWER_SWEET
	.hword ITEM_STAR_SWEET
	.hword ITEM_RIBBON_SWEET
	.hword ITEM_BLACK_AUGURITE
	.hword 0x0
