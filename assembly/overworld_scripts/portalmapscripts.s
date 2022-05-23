.thumb
.align 2

.include "../xse_commands.s"
.include "../xse_defines.s"
.include "../asm_defines.s"

.equ FLAG_DIALGA_GOT, 0x9A5
.equ FLAG_DIALGA_SPRITE, 0x9A6
.equ FLAG_GOT_CHARMS, 0x99E
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
.equ FLAG_NECROZMA_GOT, 0x100B
.equ FLAG_NECROZMA_SPRITE, 0x100C
.equ FLAG_ZACIAN_GOT, 0x100D
.equ FLAG_ZACIAN_SPRITE, 0x100E
.equ FLAG_ZAMAZENTA_GOT, 0x100F
.equ FLAG_ZAMAZENTA_SPRITE, 0x1010
.equ FLAG_CALYREX_GOT, 0x1011
.equ FLAG_CALYREX_SPRITE, 0x1012
.equ FLAG_ARCEUS_BATTLED, 0x9AB
.equ FLAG_HIDE_ARCEUS_PORTAL, 0x1014
.equ FLAG_CRESSELIA_SPRITE, 0x1017
.equ FLAG_GOT_CRESSELIA, 0x1018

.global EventScript_Route3MapScript
EventScript_Route3MapScript:
	mapscript MAP_SCRIPT_ON_LOAD EventScript_HideDialgaPortal
	.byte MAP_SCRIPT_TERMIN

EventScript_HideDialgaPortal:
	checkflag FLAG_DIALGA_GOT
	if SET _goto EventScript_HideDialgaPortal2
	checkflag FLAG_GOT_CHARMS
	if SET _goto EventScript_ShowDialgaPortal
	hidesprite 11
	setflag FLAG_DIALGA_SPRITE
	end

EventScript_ShowDialgaPortal:
	showsprite 11
	clearflag FLAG_DIALGA_SPRITE
	end

EventScript_HideDialgaPortal2:
	hidesprite 11
	setflag FLAG_DIALGA_SPRITE
	end

.global EventScript_Route10MapScript
EventScript_Route10MapScript:
	mapscript MAP_SCRIPT_ON_LOAD EventScript_HideGiratinaPortal
	.byte MAP_SCRIPT_TERMIN

EventScript_HideGiratinaPortal:
	checkflag FLAG_GIRATINA_GOT
	if SET _goto EventScript_HideGiratinaPortal2
	checkflag FLAG_GOT_CHARMS
	if SET _goto EventScript_ShowGiratinaPortal
	hidesprite 12
	setflag FLAG_GIRATINA_SPRITE
	end

EventScript_ShowGiratinaPortal:
	showsprite 12
	clearflag FLAG_GIRATINA_SPRITE
	end

EventScript_HideGiratinaPortal2:
	hidesprite 12
	setflag FLAG_GIRATINA_SPRITE
	end


.global EventScript_Route6MapScript
EventScript_Route6MapScript:
	mapscript MAP_SCRIPT_ON_LOAD EventScript_HideReshiramPortal
	.byte MAP_SCRIPT_TERMIN

EventScript_HideReshiramPortal:
	checkflag FLAG_RESHIRAM_GOT
	if SET _goto EventScript_HideReshiramPortal2
	checkflag FLAG_GOT_CHARMS
	if SET _goto EventScript_ShowReshiramPortal
	hidesprite 8
	setflag FLAG_RESHIRAM_SPRITE
	end

EventScript_ShowReshiramPortal:
	showsprite 8
	clearflag FLAG_RESHIRAM_SPRITE
	end

EventScript_HideReshiramPortal2:
	hidesprite 8
	setflag FLAG_RESHIRAM_SPRITE
	end


.global EventScript_Route7MapScript
EventScript_Route7MapScript:
	mapscript MAP_SCRIPT_ON_LOAD EventScript_HideZekromPortal
	.byte MAP_SCRIPT_TERMIN

EventScript_HideZekromPortal:
	checkflag FLAG_ZEKROM_GOT
	if SET _goto EventScript_HideZekromPortal2
	checkflag FLAG_GOT_CHARMS
	if SET _goto EventScript_ShowZekromPortal
	hidesprite 3
	setflag FLAG_ZEKROM_SPRITE
	end

EventScript_ShowZekromPortal:
	showsprite 3
	clearflag FLAG_ZEKROM_SPRITE
	end

EventScript_HideZekromPortal2:
	hidesprite 3
	setflag FLAG_ZEKROM_SPRITE
	end

.global EventScript_Route8MapScript
EventScript_Route8MapScript:
	mapscript MAP_SCRIPT_ON_LOAD EventScript_HideKyuremPortal
	.byte MAP_SCRIPT_TERMIN

EventScript_HideKyuremPortal:
	checkflag FLAG_KYUREM_GOT
	if SET _goto EventScript_HideKyuremPortal2
	checkflag FLAG_GOT_CHARMS
	if SET _goto EventScript_ShowKyuremPortal
	hidesprite 17
	setflag FLAG_KYUREM_SPRITE
	end

EventScript_ShowKyuremPortal:
	showsprite 17
	clearflag FLAG_KYUREM_SPRITE
	end

EventScript_HideKyuremPortal2:
	hidesprite 17
	setflag FLAG_KYUREM_SPRITE
	end

.global EventScript_Route18MapScript
EventScript_Route18MapScript:
	mapscript MAP_SCRIPT_ON_LOAD EventScript_HideSolgaleoPortal
	.byte MAP_SCRIPT_TERMIN

EventScript_HideSolgaleoPortal:
	checkflag FLAG_SOLGALEO_GOT
	if SET _goto EventScript_HideSolgaleoPortal2
	checkflag FLAG_GOT_CHARMS
	if SET _goto EventScript_ShowSolgaleoPortal
	hidesprite 4
	setflag FLAG_SOLGALEO_SPRITE
	end

EventScript_ShowSolgaleoPortal:
	showsprite 4
	clearflag FLAG_SOLGALEO_SPRITE
	end

EventScript_HideSolgaleoPortal2:
	hidesprite 4
	setflag FLAG_SOLGALEO_SPRITE
	end

.global EventScript_Route20MapScript
EventScript_Route20MapScript:
	mapscript MAP_SCRIPT_ON_LOAD EventScript_HideLunalaPortal
	.byte MAP_SCRIPT_TERMIN

EventScript_HideLunalaPortal:
	checkflag FLAG_LUNALA_GOT
	if SET _goto EventScript_HideLunalaPortal2
	checkflag FLAG_GOT_CHARMS
	if SET _goto EventScript_ShowLunalaPortal
	hidesprite 12
	setflag FLAG_LUNALA_SPRITE
	end

EventScript_ShowLunalaPortal:
	showsprite 12
	clearflag FLAG_LUNALA_SPRITE
	end

EventScript_HideLunalaPortal2:
	hidesprite 12
	setflag FLAG_LUNALA_SPRITE
	end

.global EventScript_Route22MapScript
EventScript_Route22MapScript:
	mapscript MAP_SCRIPT_ON_LOAD EventScript_HideNecrozmaPortal
	.byte MAP_SCRIPT_TERMIN

EventScript_HideNecrozmaPortal:
	checkflag FLAG_NECROZMA_GOT
	if SET _goto EventScript_HideNecrozmaPortal2
	checkflag FLAG_GOT_CHARMS
	if SET _goto EventScript_ShowNecrozmaPortal
	hidesprite 3
	setflag FLAG_NECROZMA_SPRITE
	end

EventScript_ShowNecrozmaPortal:
	showsprite 3
	clearflag FLAG_NECROZMA_SPRITE
	end

EventScript_HideNecrozmaPortal2:
	hidesprite 3
	setflag FLAG_NECROZMA_SPRITE
	end

.global EventScript_Route9MapScript
EventScript_Route9MapScript:
	mapscript MAP_SCRIPT_ON_LOAD EventScript_HideZacianPortal
	.byte MAP_SCRIPT_TERMIN

EventScript_HideZacianPortal:
	checkflag FLAG_ZACIAN_GOT
	if SET _goto EventScript_HideZacianPortal2
	checkflag FLAG_GOT_CHARMS
	if SET _goto EventScript_ShowZacianPortal
	hidesprite 14
	setflag FLAG_ZACIAN_SPRITE
	end

EventScript_ShowZacianPortal:
	showsprite 14
	clearflag FLAG_ZACIAN_SPRITE
	end

EventScript_HideZacianPortal2:
	hidesprite 14
	setflag FLAG_ZACIAN_SPRITE
	end

.global EventScript_Route11MapScript
EventScript_Route11MapScript:
	mapscript MAP_SCRIPT_ON_LOAD EventScript_HideZamazentaPortal
	.byte MAP_SCRIPT_TERMIN

EventScript_HideZamazentaPortal:
	checkflag FLAG_ZAMAZENTA_GOT
	if SET _goto EventScript_HideZamazentaPortal2
	checkflag FLAG_GOT_CHARMS
	if SET _goto EventScript_ShowZamazentaPortal
	hidesprite 16
	setflag FLAG_ZAMAZENTA_SPRITE
	end

EventScript_ShowZamazentaPortal:
	showsprite 16
	clearflag FLAG_ZAMAZENTA_SPRITE
	end

EventScript_HideZamazentaPortal2:
	hidesprite 16
	setflag FLAG_ZAMAZENTA_SPRITE
	end


.global EventScript_Route14MapScript
EventScript_Route14MapScript:
	mapscript MAP_SCRIPT_ON_LOAD EventScript_HideCalyrexPortal
	.byte MAP_SCRIPT_TERMIN

EventScript_HideCalyrexPortal:
	checkflag FLAG_CALYREX_GOT
	if SET _goto EventScript_HideCalyrexPortal2
	checkflag FLAG_GOT_CHARMS
	if SET _goto EventScript_ShowCalyrexPortal
	hidesprite 17
	setflag FLAG_CALYREX_SPRITE
	end

EventScript_ShowCalyrexPortal:
	showsprite 17
	clearflag FLAG_CALYREX_SPRITE
	end

EventScript_HideCalyrexPortal2:
	hidesprite 17
	setflag FLAG_CALYREX_SPRITE
	end


.global EventScript_Route25MapScript
EventScript_Route25MapScript:
	mapscript MAP_SCRIPT_ON_LOAD EventScript_HideArceusPortal
	.byte MAP_SCRIPT_TERMIN

EventScript_HideArceusPortal:
	callasm HasCreationTrio
	compare LASTRESULT 0x0
	if equal _goto EventScript_HideArceusPortal2
	checkflag FLAG_GOT_CHARMS
	if SET _goto EventScript_ShowArceusPortal
	hidesprite 16
	setflag FLAG_HIDE_ARCEUS_PORTAL
	end

EventScript_ShowArceusPortal:
	showsprite 16
	clearflag FLAG_HIDE_ARCEUS_PORTAL
	end

EventScript_HideArceusPortal2:
	hidesprite 16
	setflag FLAG_HIDE_ARCEUS_PORTAL
	end

.global EventScript_Route4MapScript
EventScript_Route4MapScript:
	mapscript MAP_SCRIPT_ON_LOAD EventScript_HideCresselia
	.byte MAP_SCRIPT_TERMIN

EventScript_HideCresselia:
	checkflag 0x827
	if NOT_SET _goto EventScript_HideCresseliaPortal2
	checkflag FLAG_GOT_CRESSELIA
	if SET _goto EventScript_HideCresseliaPortal2
	showsprite 14
	clearflag FLAG_CRESSELIA_SPRITE
	end

EventScript_HideCresseliaPortal2:
	hidesprite 14
	setflag FLAG_CRESSELIA_SPRITE
	end
