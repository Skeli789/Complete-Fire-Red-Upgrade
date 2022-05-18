.thumb
.align 2

.include "../xse_commands.s"
.include "../xse_defines.s"
.include "../asm_defines.s"

.global EventScript_UltiElementTeach
EventScript_UltiElementTeach:
	lock
	faceplayer
	bufferfirstpokemon 0x0
	msgbox 0x81A46C6 MSG_KEEPOPEN
	setvar 0x8003 0x0 
	setvar 0x8004 0x0
	special2 LASTRESULT 0x18
	compare LASTRESULT SPECIES_CHARIZARD
	if 0x1 _goto EventScript_TeachFireEle
	compare LASTRESULT SPECIES_TYPHLOSION
	if 0x1 _goto EventScript_TeachFireEle
	compare LASTRESULT SPECIES_TYPHLOSION_H
	if 0x1 _goto EventScript_TeachFireEle
	compare LASTRESULT SPECIES_BLAZIKEN
	if 0x1 _goto EventScript_TeachFireEle
	compare LASTRESULT SPECIES_INFERNAPE
	if 0x1 _goto EventScript_TeachFireEle
	compare LASTRESULT SPECIES_EMBOAR
	if 0x1 _goto EventScript_TeachFireEle
	compare LASTRESULT SPECIES_DELPHOX
	if 0x1 _goto EventScript_TeachFireEle
	compare LASTRESULT SPECIES_INCINEROAR
	if 0x1 _goto EventScript_TeachFireEle
	compare LASTRESULT SPECIES_CINDERACE
	if 0x1 _goto EventScript_TeachFireEle
	compare LASTRESULT SPECIES_BLASTOISE
	if 0x1 _goto EventScript_TeachWaterEle
	compare LASTRESULT SPECIES_FERALIGATR
	if 0x1 _goto EventScript_TeachWaterEle
	compare LASTRESULT SPECIES_SWAMPERT
	if 0x1 _goto EventScript_TeachWaterEle
	compare LASTRESULT SPECIES_EMPOLEON
	if 0x1 _goto EventScript_TeachWaterEle
	compare LASTRESULT SPECIES_SAMUROTT
	if 0x1 _goto EventScript_TeachWaterEle
	compare LASTRESULT SPECIES_SAMUROTT_H
	if 0x1 _goto EventScript_TeachWaterEle
	compare LASTRESULT SPECIES_GRENINJA
	if 0x1 _goto EventScript_TeachWaterEle
	compare LASTRESULT SPECIES_PRIMARINA
	if 0x1 _goto EventScript_TeachWaterEle
	compare LASTRESULT SPECIES_INTELEON
	if 0x1 _goto EventScript_TeachWaterEle
	compare LASTRESULT SPECIES_VENUSAUR
	if 0x1 _goto EventScript_TeachGrassEle
	compare LASTRESULT SPECIES_MEGANIUM
	if 0x1 _goto EventScript_TeachGrassEle
	compare LASTRESULT SPECIES_SCEPTILE
	if 0x1 _goto EventScript_TeachGrassEle
	compare LASTRESULT SPECIES_TORTERRA
	if 0x1 _goto EventScript_TeachGrassEle
	compare LASTRESULT SPECIES_SERPERIOR
	if 0x1 _goto EventScript_TeachGrassEle
	compare LASTRESULT SPECIES_CHESNAUGHT
	if 0x1 _goto EventScript_TeachGrassEle
	compare LASTRESULT SPECIES_DECIDUEYE
	if 0x1 _goto EventScript_TeachGrassEle
	compare LASTRESULT SPECIES_DECIDUEYE_H
	if 0x1 _goto EventScript_TeachGrassEle
	compare LASTRESULT SPECIES_RILLABOOM
	if 0x1 _goto EventScript_TeachGrassEle
	msgbox 0x81A4737 MSG_KEEPOPEN 
	release
	end

EventScript_TeachFireEle:
	bufferattack 0x1 MOVE_BLASTBURN
	msgbox 0x81A4751 MSG_YESNO
	compare LASTRESULT 0x1
	if equal _goto BlastBurn
	goto EventScript_End
	release
	end

EventScript_TeachWaterEle:
	bufferattack 0x1 MOVE_HYDROCANNON
	msgbox 0x81A4751 MSG_YESNO
	compare LASTRESULT 0x1
	if equal _goto HydroCannon
	goto EventScript_End
	release
	end

EventScript_TeachGrassEle:
	bufferattack 0x1 MOVE_FRENZYPLANT
	msgbox 0x81A4751 MSG_YESNO
	compare LASTRESULT 0x1
	if equal _goto FrenzyPlant
	goto EventScript_End
	release
	end

BlastBurn:
	setvar 0x8005 62
	goto EventScript_TeachMove
	release
	end

FrenzyPlant:
	setvar 0x8005 61
	goto EventScript_TeachMove
	release
	end

HydroCannon:
	setvar 0x8005 63
	goto EventScript_TeachMove
	release
	end

EventScript_TeachMove:
	msgbox 0x81A4814 MSG_KEEPOPEN
	special 0x18D
	waitstate
	compare LASTRESULT 0x0
	if equal _goto EventScript_End
	msgbox 0x81A4865 MSG_KEEPOPEN
	msgbox 0x81A48F3 MSG_KEEPOPEN 
	release
	end

EventScript_End:
	msgbox 0x81A47E5 MSG_KEEPOPEN
	release
	end
