.thumb
.align 2

.include "../xse_commands.s"
.include "../xse_defines.s"
.include "../asm_defines.s"

.equ FLAG_REGICE_PUZZLE, 0x96B
.equ FLAG_REGICE_BEAT, 0x96C
.equ FLAG_REGIROCK_BEAT, 0x96D
.equ FLAG_UB_1_FOUGHT, 0x970
.equ FLAG_UB_2_FOUGHT, 0x971
.equ FLAG_UB_3_FOUGHT, 0x972
.equ FLAG_UB_4_FOUGHT, 0x973
.equ FLAG_UB_5_FOUGHT, 0x974
.equ FLAG_UB_6_FOUGHT, 0x975
.equ FLAG_UB_7_FOUGHT, 0x976
.equ FLAG_REGISTEEL_BEAT, 0x978
.equ VAR_PSYCHIC_FORCE, 0x5108
.equ FLAG_MEW_BEAT, 0x97A
.equ FLAG_GROUDON_BEAT, 0x97B
.equ FLAG_KYOGRE_BEAT, 0x97D
.equ FLAG_RAYQUAZA_BEAT, 0x989
.equ VAR_RAQUAZA_SCRIPT, 0x5109
.equ FLAG_VICTINI_BEAT, 0x996
.equ FLAG_ZYGARDE_GOT, 0x99F
.equ FLAG_ZYGARDE_SPRITE, 0x9A0
.equ FLAG_YVELTAL_GOT, 0x9A1
.equ FLAG_YVELTAL_SPRITE, 0x9A2
.equ FLAG_XERNEAS_GOT, 0x9A3
.equ FLAG_XERNEAS_SPRITE, 0x9A4
.equ FLAG_PALKIA_GOT, 0x9A7
.equ FLAG_PALKIA_SPRITE, 0x9A8
.equ FLAG_DIALGA_GOT, 0x9A5
.equ FLAG_DIALGA_SPRITE, 0x9A6
.equ FLAG_GIRATINA_GOT, 0x9A9
.equ FLAG_GIRATINA_SPRITE, 0x9AA
.equ FLAG_RESHIRAM_GOT, 0x9AC
.equ FLAG_RESHIRAM_SPRITE, 0x9AD
.equ FLAG_ZEKROM_GOT, 0x9AE
.equ FLAG_ZEKROM_SPRITE, 0x9AF
.equ FLAG_KYUREM_GOT, 0x9B0
.equ FLAG_KYUREM_SPRITE, 0x9B1
.equ FLAG_SOLGALEO_GOT, 0x9B2
.equ FLAG_SOLGALEO_SPRITE, 0x9B3
.equ FLAG_LUNALA_GOT, 0x9B4
.equ FLAG_LUNALA_SPRITE, 0x9B5
.equ FLAG_JIRACHI_GOT, 0x9B6
.equ FLAG_MANAPHY_1, 0x9BC
.equ FLAG_MANAPHY_2, 0x9BD
.equ VAR_MANAPHY_SCRIPT, 0x510C
.equ FLAG_HIDE_MOLTRES, 0x52
.equ FLAG_NECROZMA_GOT, 0x100B
.equ FLAG_NECROZMA_SPRITE, 0x100C
.equ FLAG_ZACIAN_GOT, 0x100D
.equ FLAG_ZACIAN_SPRITE, 0x100E
.equ FLAG_ZAMAZENTA_GOT, 0x100F
.equ FLAG_ZAMAZENTA_SPRITE, 0x1010
.equ FLAG_CALYREX_GOT, 0x1011
.equ FLAG_CALYREX_SPRITE, 0x1012
.equ FLAG_DIANCIE, 0x1013
.equ FLAG_GOT_DARKRAI, 0x1016
.equ FLAG_CRESSELIA_SPRITE, 0x1017
.equ FLAG_GOT_CRESSELIA, 0x1018

.global EventScript_RegicePuzzleForce
EventScript_RegicePuzzleForce:
	lockall
	checkflag FLAG_REGICE_PUZZLE
	if 0x0 _goto EventScript_MovePlayerRegice
	releaseall
	end

EventScript_MovePlayerRegice:
	msgbox gText_RegiceForce MSG_NORMAL
	applymovement 0xFF EventScript_RegiceMove
	waitmovement 0x0
	releaseall
	end

EventScript_RegiceMove:
	.byte walk_down
	.byte 0xFE

.global EventScript_RegicePuzzleBraille
EventScript_RegicePuzzleBraille:
	lock
	msgbox gText_RegicePuzzle MSG_SIGN
	release
	end

.global EventScript_RegicePuzzle
EventScript_RegicePuzzle:
	lock
	checkflag FLAG_REGICE_PUZZLE
	if 0x0 _goto EventScript_SetRegiceFlag
	release
	end

EventScript_SetRegiceFlag:
	setflag FLAG_REGICE_PUZZLE
	sound 0x12
	msgbox gText_RegiceMoved MSG_SIGN
	release
	end

EventScript_Regice:
	lock
	faceplayer
	cry SPECIES_REGICE 0x0
	msgbox gText_Regice MSG_NORMAL
	wildbattle SPECIES_REGICE 70 ITEM_NEVER_MELT_ICE
	hidesprite 0x1
	setflag 0x96C
	release
	end

.global EventScript_Regirock
EventScript_Regirock:
	lock
	faceplayer
	cry SPECIES_REGIROCK 0x0
	msgbox gText_Regirock MSG_NORMAL
	wildbattle SPECIES_REGIROCK 65 ITEM_HARD_STONE
	setflag FLAG_REGIROCK_BEAT
	hidesprite 27
	release
	end

.global EventScript_Nihilego
EventScript_Nihilego:
	lock
	faceplayer
	cry SPECIES_NIHILEGO 0x0
	msgbox gText_Nihilego MSG_NORMAL
	wildbattle SPECIES_NIHILEGO 75 ITEM_HARD_STONE
	setflag FLAG_UB_1_FOUGHT
	hidesprite 1
	release
	end

.global EventScript_Buzzwole
EventScript_Buzzwole:
	lock
	faceplayer
	cry SPECIES_BUZZWOLE 0x0
	msgbox gText_Buzzwole MSG_NORMAL
	wildbattle SPECIES_BUZZWOLE 75 ITEM_EXPERT_BELT
	setflag FLAG_UB_2_FOUGHT
	hidesprite 1
	release
	end

.global EventScript_Pheremosa
EventScript_Pheremosa:
	lock
	faceplayer
	cry SPECIES_PHEROMOSA 0x0
	msgbox gText_Pheremosa MSG_NORMAL
	wildbattle SPECIES_PHEROMOSA 75 ITEM_SILVER_POWDER
	setflag FLAG_UB_3_FOUGHT
	hidesprite 1
	release
	end

.global EventScript_Xurkitree
EventScript_Xurkitree:
	lock
	faceplayer
	cry SPECIES_XURKITREE 0x0
	msgbox gText_Xurkitree MSG_NORMAL
	wildbattle SPECIES_XURKITREE 75 ITEM_MAGNET
	setflag FLAG_UB_4_FOUGHT
	hidesprite 1
	release
	end

.global EventScript_Celesteela
EventScript_Celesteela:
	lock
	faceplayer
	cry SPECIES_CELESTEELA 0x0
	msgbox gText_Celesteela MSG_NORMAL
	wildbattle SPECIES_CELESTEELA 75 ITEM_METAL_COAT
	setflag FLAG_UB_5_FOUGHT
	hidesprite 1
	release
	end

.global EventScript_Kartana
EventScript_Kartana:
	lock
	faceplayer
	cry SPECIES_KARTANA 0x0
	msgbox gText_Kartana MSG_NORMAL
	wildbattle SPECIES_KARTANA 75 ITEM_MIRACLE_SEED
	setflag FLAG_UB_6_FOUGHT
	hidesprite 1
	release
	end

.global EventScript_Guzzlord
EventScript_Guzzlord:
	lock
	faceplayer
	cry SPECIES_GUZZLORD 0x0
	msgbox gText_Guzzlord MSG_NORMAL
	wildbattle SPECIES_GUZZLORD 75 ITEM_DRAGON_FANG
	setflag FLAG_UB_7_FOUGHT
	hidesprite 1
	release
	end

.global EventScript_Registeel
EventScript_Registeel:
	lock
	faceplayer
	cry SPECIES_REGISTEEL 0x0
	msgbox gText_Registeel MSG_NORMAL
	wildbattle SPECIES_REGISTEEL 65 ITEM_METAL_COAT
	setflag FLAG_REGISTEEL_BEAT
	hidesprite 9
	release
	end


.global EventScript_MewForce
EventScript_MewForce:
	lockall
	checkflag 0x827
	if 0x0 _goto EventScript_CantMove
	releaseall
	end
	
EventScript_CantMove:
	msgbox gText_PsychicForce MSG_NORMAL
	applymovement 0xFF EventScript_MewForceMove
	waitmovement 0x0
	releaseall
	end

EventScript_MewForceMove:
	.byte walk_left
	.byte 0xFE

EventScript_Mew:
	lock
	faceplayer
	cry SPECIES_MEW 0x0
	msgbox gText_Mew MSG_NORMAL
	wildbattle SPECIES_MEW 70 0x0
	setflag FLAG_MEW_BEAT
	hidesprite 1
	release
	end

.global EventScript_Groudon
EventScript_Groudon:
	lock
	faceplayer
	cry SPECIES_GROUDON 0x0
	msgbox gText_Groudon MSG_NORMAL
	wildbattle SPECIES_GROUDON 70 0x0
	setflag FLAG_GROUDON_BEAT	
	hidesprite 10
	release
	end

.global EventScript_Kyogre
EventScript_Kyogre:
	lock
	faceplayer
	cry SPECIES_KYOGRE 0x0
	msgbox gText_Kyogre MSG_NORMAL
	wildbattle SPECIES_KYOGRE 70 0x0
	setflag FLAG_KYOGRE_BEAT	
	hidesprite 1
	release
	end

.global EventScript_Rayquaza
EventScript_Rayquaza:
	lock
	applymovement 0x1 EventScript_MoveRayquaza
	waitmovement 0x0
	cry SPECIES_RAYQUAZA 0x0
	msgbox gText_Rayquaza MSG_NORMAL
	wildbattle SPECIES_RAYQUAZA 75 0x0
	setflag FLAG_RAYQUAZA_BEAT
	setvar VAR_RAQUAZA_SCRIPT 0x1
	hidesprite 1
	release
	end

.global EventScript_HoopaPortal
EventScript_HoopaPortalEmpty:
	msgbox gText_EmptyPortal MSG_FACE
	release
	end

.global EventScript_Victini
EventScript_VictiniHoopaPortal:
	lock
	checkflag 0x827
	if 0x0 _goto EventScript_HoopaPortalEmpty
	msgbox gText_EnterPortal MSG_YESNO
	compare LASTRESULT 0x1
	if equal _goto EventScript_WarpVictini
	release
	end

EventScript_WarpVictini:
	warp 43 14 0xFF 0x7 0x6
	release
	end

EventScript_VictiniHoopaPortalBack:
	lock
	msgbox gText_WarpBack MSG_YESNO
	compare LASTRESULT 0x1
	if equal _goto EventScript_VictiniWarpBack
	release
	end

EventScript_VictiniWarpBack:
	warp 1 37 0xFF 0x24 0x9
	release
	end

EventScript_Victini:
	lock
	faceplayer
	cry SPECIES_VICTINI 0x0
	msgbox gText_Victini MSG_NORMAL
	wildbattle SPECIES_VICTINI 75 ITEM_CHARCOAL
	setflag FLAG_VICTINI_BEAT
	hidesprite 2
	release
	end

EventScript_MoveRayquaza:
	.byte walk_down
	.byte walk_down
	.byte 0xFE

.global EventScript_ZygardePortal
EventScript_ZygardePortal:
	lock
	msgbox gText_PortalHoopa MSG_YESNO
	compare LASTRESULT 0x1
	if equal _goto EventScript_ZygardeBattle
	release
	end

EventScript_ZygardeBattle:
	setflag 0x90F
	wildbattle SPECIES_ZYGARDE 80 0x0
	hidesprite 4
	setflag FLAG_ZYGARDE_GOT
	setflag FLAG_ZYGARDE_SPRITE
	release
	end

.global EventScript_YveltalPortal
EventScript_YveltalPortal:
	lock
	msgbox gText_PortalHoopa MSG_YESNO
	compare LASTRESULT 0x1
	if equal _goto EventScript_YveltalBattle
	release
	end

EventScript_YveltalBattle:
	wildbattle SPECIES_YVELTAL 80 0x0
	hidesprite 7
	setflag FLAG_YVELTAL_GOT
	setflag FLAG_YVELTAL_SPRITE
	release
	end

.global EventScript_XerneasPortal
EventScript_XerneasPortal:
	lock
	msgbox gText_PortalHoopa MSG_YESNO
	compare LASTRESULT 0x1
	if equal _goto EventScript_XerneasBattle
	release
	end

EventScript_XerneasBattle:
	wildbattle SPECIES_XERNEAS 80 0x0
	hidesprite 14
	setflag FLAG_XERNEAS_GOT
	setflag FLAG_XERNEAS_SPRITE
	release
	end

.global EventScript_PalkiaPortal
EventScript_PalkiaPortal:
	lock
	msgbox gText_PortalHoopa MSG_YESNO
	compare LASTRESULT 0x1
	if equal _goto EventScript_PalkiaBattle
	release
	end

EventScript_PalkiaBattle:
	wildbattle SPECIES_PALKIA 80 0x0
	hidesprite 7
	setflag FLAG_PALKIA_GOT
	setflag FLAG_PALKIA_SPRITE
	release
	end

.global EventScript_DialgaPortal
EventScript_DialgaPortal:
	lock
	msgbox gText_PortalHoopa MSG_YESNO
	compare LASTRESULT 0x1
	if equal _goto EventScript_DialgaBattle
	release
	end

EventScript_DialgaBattle:
	wildbattle SPECIES_DIALGA 80 0x0
	hidesprite 11
	setflag FLAG_DIALGA_GOT
	setflag FLAG_DIALGA_SPRITE
	release
	end

.global EventScript_GiratinaPortal
EventScript_GiratinaPortal:
	lock
	msgbox gText_PortalHoopa MSG_YESNO
	compare LASTRESULT 0x1
	if equal _goto EventScript_GiratinaBattle
	release
	end

EventScript_GiratinaBattle:
	wildbattle SPECIES_GIRATINA 80 0x0
	hidesprite 12
	setflag FLAG_GIRATINA_GOT
	setflag FLAG_GIRATINA_SPRITE
	release
	end

.global EventScript_ReshiramPortal
EventScript_ReshiramPortal:
	lock
	msgbox gText_PortalHoopa MSG_YESNO
	compare LASTRESULT 0x1
	if equal _goto EventScript_ReshiramBattle
	release
	end

EventScript_ReshiramBattle:
	wildbattle SPECIES_RESHIRAM 80 0x0
	hidesprite 8
	setflag FLAG_RESHIRAM_GOT
	setflag FLAG_RESHIRAM_SPRITE
	release
	end

	
.global EventScript_ZekromPortal
EventScript_ZekromPortal:
	lock
	msgbox gText_PortalHoopa MSG_YESNO
	compare LASTRESULT 0x1
	if equal _goto EventScript_ZekromBattle
	release
	end

EventScript_ZekromBattle:
	wildbattle SPECIES_ZEKROM 80 0x0
	hidesprite 3
	setflag FLAG_ZEKROM_GOT
	setflag FLAG_ZEKROM_SPRITE
	release
	end

.global EventScript_KyuremPortal
EventScript_KyuremPortal:
	lock
	msgbox gText_PortalHoopa MSG_YESNO
	compare LASTRESULT 0x1
	if equal _goto EventScript_KyuremBattle
	release
	end

EventScript_KyuremBattle:
	wildbattle SPECIES_KYUREM 80 0x0
	hidesprite 17
	setflag FLAG_KYUREM_GOT
	setflag FLAG_KYUREM_SPRITE
	release
	end

.global EventScript_SolgaleoPortal
EventScript_SolgaleoPortal:
	lock
	msgbox gText_PortalHoopa MSG_YESNO
	compare LASTRESULT 0x1
	if equal _goto EventScript_SolgaleoBattle
	release
	end

EventScript_SolgaleoBattle:
	wildbattle SPECIES_SOLGALEO 80 0x0
	hidesprite 4
	setflag FLAG_SOLGALEO_GOT
	setflag FLAG_SOLGALEO_SPRITE
	release
	end

.global EventScript_LunalaPortal
EventScript_LunalaPortal:
	lock
	msgbox gText_PortalHoopa MSG_YESNO
	compare LASTRESULT 0x1
	if equal _goto EventScript_LunalaBattle
	release
	end

EventScript_LunalaBattle:
	wildbattle SPECIES_LUNALA 80 0x0
	hidesprite 12
	setflag FLAG_LUNALA_GOT
	setflag FLAG_LUNALA_SPRITE
	release
	end

.global EventScript_BillTeleporter
EventScript_BillTeleporter:
	lock
	checkflag FLAG_JIRACHI_GOT
	if SET _goto EventScript_DoNothing
	checkflag 0x827
	if SET _goto EventScript_BillTeleporterAsk
	release
	end

EventScript_DoNothing:
	release
	end

EventScript_BillTeleporterAsk:
	msgbox gText_BillTeleporterAsk MSG_YESNO
	compare LASTRESULT 0x1
	if equal _goto EventScript_GoInBillTele
	release
	end

EventScript_GoInBillTele:
	opendoor 0xA 0x3
	waitdooranim
	applymovement 0xFF EventScript_FaceUpMoveUp
	waitmovement 0x0
	sound 0x5B
	applymovement 0xFF EventScript_FaceDown
	waitmovement 0x0
	applymovement 0xFF EventScript_Hideplayer
	closedoor 0xA 0x3
	waitmovement 0x0
	pause DELAY_HALFSECOND
	warpmuted 43 15 0xFF 0x3 0xF
	release
	end

EventScript_FaceUpMoveUp:
	.byte look_up
	.byte walk_up
	.byte 0xFE

EventScript_Hideplayer:
	.byte 0x60
	.byte 0xFE

EventScript_FaceDown:
	.byte look_down
	.byte 0xFE

EventScript_Jirachi:
	lock
	faceplayer
	cry SPECIES_JIRACHI 0x0
	msgbox gText_Jirachi MSG_NORMAL
	wildbattle SPECIES_JIRACHI 80 0x0
	setflag FLAG_JIRACHI_GOT
	hidesprite 1
	setvar 0x8004 0x5  
	setvar 0x8005 0x5
	setvar 0x8006 0x20  
	setvar 0x8007 0x4 
	special 0x136
	msgbox gText_RealityBreaking MSG_NORMAL
	warpmuted 30 0 0xFF 0xA 0x3
	pause DELAY_HALFSECOND
	applymovement 0xFF EventScript_Hideplayer
	waitmovement 0x0
	opendoor 0xA 0x3
	applymovement 0xFF EventScript_Showplayer
	waitmovement 0x0
	applymovement 0xFF EventScript_FaceDownWalkDown
	waitmovement 0x0
	closedoor 0xA 0x3
	release
	end

EventScript_Showplayer:
	.byte 0x61
	.byte 0xFE

EventScript_FaceDownWalkDown:
	.byte look_down
	.byte walk_down
	.byte 0xFE

EventScript_ExclamationWalk:
	.byte 0x62
	.byte look_right
	.byte walk_right
	.byte walk_right
	.byte walk_right
	.byte look_up
	.byte 0xFE

.global EventScript_Manaphy
EventScript_ManaphyScript:
	lock
	applymovement 4 EventScript_ManaphyMove
	waitmovement 0x0
	setflag FLAG_MANAPHY_1
	hidesprite 4
	setflag VAR_MANAPHY_SCRIPT
	release
	end

EventScript_ManaphyMove:
	.byte 0x52
	.byte 0x52
	.byte walk_up
	.byte walk_up
	.byte walk_up
	.byte walk_up
	.byte 0xFE

EventScript_Manaphy:
	lock
	cry SPECIES_MANAPHY 0x0
	msgbox gText_Manaphy MSG_NORMAL
	wildbattle SPECIES_MANAPHY 65 0x0
	setflag FLAG_MANAPHY_2
	hidesprite 5
	release
	end

.global EventScript_Moltres
EventScript_Moltres:
	lock
	cry SPECIES_MOLTRES 0x0
	msgbox gText_Moltres MSG_NORMAL
	wildbattle SPECIES_MOLTRES 80 0x0
	fadescreen 0x1
	setflag FLAG_HIDE_MOLTRES
	hidesprite 6
	fadescreen 0x0
	msgbox gText_MoltresFlew MSG_NORMAL
	release
	end

.global EventScript_NecrozmaPortal
EventScript_NecrozmaPortal:
	lock
	msgbox gText_PortalHoopa MSG_YESNO
	compare LASTRESULT 0x1
	if equal _goto EventScript_NecrozmaBattle
	release
	end

EventScript_NecrozmaBattle:
	wildbattle SPECIES_NECROZMA 80 0x0
	hidesprite 3
	setflag FLAG_NECROZMA_GOT
	setflag FLAG_NECROZMA_SPRITE
	release
	end

.global EventScript_ZacianPortal
EventScript_ZacianPortal:
	lock
	msgbox gText_PortalHoopa MSG_YESNO
	compare LASTRESULT 0x1
	if equal _goto EventScript_ZacianBattle
	release
	end

EventScript_ZacianBattle:
	wildbattle SPECIES_ZACIAN 80 0x0
	hidesprite 14
	setflag FLAG_ZACIAN_GOT
	setflag FLAG_ZACIAN_SPRITE
	release
	end

.global EventScript_ZamazentaPortal
EventScript_ZamazentaPortal:
	lock
	msgbox gText_PortalHoopa MSG_YESNO
	compare LASTRESULT 0x1
	if equal _goto EventScript_ZamazentaBattle
	release
	end

EventScript_ZamazentaBattle:
	wildbattle SPECIES_ZAMAZENTA 80 0x0
	hidesprite 16
	setflag FLAG_ZAMAZENTA_GOT
	setflag FLAG_ZAMAZENTA_SPRITE
	release
	end

.global EventScript_CalyrexPortal
EventScript_CalyrexPortal:
	lock
	msgbox gText_PortalHoopa MSG_YESNO
	compare LASTRESULT 0x1
	if equal _goto EventScript_CalyrexBattle
	release
	end

EventScript_CalyrexBattle:
	wildbattle SPECIES_CALYREX 80 0x0
	hidesprite 17
	setflag FLAG_CALYREX_GOT
	setflag FLAG_CALYREX_SPRITE
	release
	end

.global EventScript_Diancie
EventScript_Diancie:
	lock
	cry SPECIES_DIANCIE 0x0
	msgbox gText_Dianice MSG_KEEPOPEN
	waitcry
	closeonkeypress
	wildbattle SPECIES_DIANCIE 60 0x0
	setflag 0x1013
	hidesprite 8
	release
	end

.global EventScript_ArceusPortal
EventScript_ArceusPortal:
	lock
	msgbox gText_EnterPortal MSG_YESNO
	compare LASTRESULT 0x1
	if equal _goto EventScript_TeleportToAHOO
	release
	end

EventScript_TeleportToAHOO:
	warpmuted 43 18 0xFF 0x3 0x21
	release
	end

EventScript_ArceusScript:
	lock
	applymovement 0xFF EventScript_PlayerShockedArceus
	sound 0x15
	waitmovement 0x0
	pause DELAY_HALFSECOND
	applymovement 0xFF EventScript_PlayerWalkToArceus
	waitmovement 0x0
	cry SPECIES_ARCEUS 0x0
	pause 0x60
	applymovement 0xFF EventScript_PlayerWalkToArceus2
	waitmovement 0x0
	cry SPECIES_ARCEUS 0x0
	msgbox gText_Arceus MSG_KEEPOPEN
	closeonkeypress
	wildbattle SPECIES_ARCEUS 85 0x0
	fadescreen 0x1
	hidesprite 2
	setflag 0x9AB
	setvar 0x510E 1
	fadescreen 0x0
	release
	end

EventScript_PlayerShockedArceus:
	.byte 0x62
	.byte 0xFE

EventScript_PlayerWalkToArceus:
	.byte walk_up
	.byte walk_up
	.byte walk_up
	.byte walk_up
	.byte 0xFE

EventScript_PlayerWalkToArceus2:
	.byte walk_up
	.byte 0xFE

.global EventScript_ArceusPortal2
EventScript_ArceusPortal2:
	lock
	msgbox gText_EnterPortal MSG_YESNO
	compare LASTRESULT 0x1
	if equal _goto EventScript_TeleportToAHOO2
	release
	end

EventScript_TeleportToAHOO2:
	warpmuted 3 44 0xFF 0x2D 0x7
	release
	end

.global EventScript_Darkrai
EventScript_Darkrai:
	lock
	cry SPECIES_DARKRAI 0x0
	msgbox gText_Darkrai MSG_FACE
	waitcry
	wildbattle SPECIES_DARKRAI 75 0x0 
	fadescreen FADEOUT_BLACK
	hidesprite 1
	setflag FLAG_GOT_DARKRAI
	fadescreen 0x0
	pause 0x20
	fadescreen FADEOUT_BLACK
	pause 0x10
	fadescreen 0x0
	pause 0x20
	fadescreen FADEOUT_BLACK
	pause 0x10
	fadescreen 0x0
	pause 0x20
	fadescreen FADEOUT_BLACK
	pause 0x40
	warpmuted 4 1 0xFF 0x2 0x5
	fadescreen 0x0
	pause 0x10
	special 0x0
	waitfanfare
	release
	end

.global EventScript_Cresselia
EventScript_Cresselia:
	lock
	cry SPECIES_CRESSELIA 0x0
	msgbox gText_Darkrai MSG_FACE
	waitcry
	wildbattle SPECIES_CRESSELIA 75 0x0 
	fadescreen FADEOUT_BLACK
	hidesprite 14
	setflag FLAG_GOT_CRESSELIA
	fadescreen 0x0
	release
	end

.global EventScript_ForbidYouEntering
EventScript_ForbidYouEntering:
	lock
	msgbox gText_ForbidYOu MSG_FACE
	release
	end
