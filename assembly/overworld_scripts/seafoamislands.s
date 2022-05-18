.thumb
.align 2

.include "../xse_commands.s"
.include "../xse_defines.s"
.include "../asm_defines.s"

.equ FLAG_YVELTAL_GOT, 0x9A1
.equ FLAG_YVELTAL_SPRITE, 0x9A2
.equ FLAG_GOT_CHARMS, 0x99E

.global EventScript_SeafomIslandsMapScript
EventScript_SeafomIslandsMapScript:
	mapscript MAP_SCRIPT_ON_LOAD EventScript_HideYveltalPortal
	.byte MAP_SCRIPT_TERMIN

EventScript_HideYveltalPortal:
	checkflag FLAG_YVELTAL_GOT
	if SET _goto EventScript_HideYveltalPortal2
	checkflag FLAG_GOT_CHARMS
	if SET _goto EventScript_ShowYveltalPortal
	hidesprite 7
	setflag FLAG_YVELTAL_SPRITE
	end

EventScript_ShowYveltalPortal:
	showsprite 7
	clearflag FLAG_YVELTAL_SPRITE
	end

EventScript_HideYveltalPortal2:
	hidesprite 7
	setflag FLAG_YVELTAL_SPRITE
	end




	@---------------
EventScript_186MapScript0_0X162F5A:
	checkflag 0x2D2
	if 0x0 _call 0x8162F6D
	checkflag 0x2D2
	if 0x1 _call 0x8162F94
	end

	@---------------
EventScript_186MapScript0_0X162F6D:
	setvar 0x4002 0x0
	checkflag 0x46
	if 0x0 _call 0x8162FF0
	checkflag 0x47
	if 0x0 _call 0x8162FF0
	compare 0x4002 0x2
	if 0x1 _call 0x8162F90
	return

	@---------------
EventScript_186MapScript0_0X162F94:
	setmapfooter 0x116
	return

	@---------------
EventScript_186MapScript0_0X162FF0:
	addvar 0x4002 0x1
	return

	@---------------
EventScript_186MapScript0_0X162F90:
	setflag 0x2D2
	return


		@---------------
EventScript_186MapScript1_0X162FA2:
	lockall
	setvar 0x4002 0x0
	checkflag 0x46
	if 0x0 _call 0x8162FF0
	checkflag 0x47
	if 0x0 _call 0x8162FF0
	compare 0x4002 0x2
	if 0x1 _goto 0x816300C
	getplayerpos 0x8008 0x8009
	compare 0x8008 0x18
	if 0x0 _call 0x8162FF6
	compare 0x8008 0x18
	if 0x4 _call 0x8163001
	setvar 0x4063 0x1
	warp 0x1 0x57 0xFF 0x1B 0x15
	waitstate
	releaseall
	end

	@---------------
EventScript_186MapScript1_0X162FF0:
	addvar 0x4002 0x1
	return

	@---------------
EventScript_186MapScript1_0X16300C:
	setvar 0x4001 0x0
	releaseall
	end

	@---------------
EventScript_186MapScript1_0X162FF6:
	applymovement 0xFF 0x8163013
	waitmovement 0x0
	return

	@---------------
EventScript_186MapScript1_0X163001:
	applymovement 0xFF 0x8163021
	waitmovement 0x0
	return


	@-----------
	@ Movements
	@-----------
EventScript_186MapScript1_0X163013:
.byte 0x1D
.byte 0x1D
.byte 0x1D
.byte 0x1D
.byte 0x20
.byte 0x20
.byte 0x20
.byte 0x1D
.byte 0x1D
.byte 0x1D
.byte 0x1D
.byte 0x1D
.byte 0x1D
.byte 0xFE

EventScript_186MapScript1_0X163021:
.byte 0x1D
.byte 0x1D
.byte 0x1D
.byte 0x1D
.byte 0x20
.byte 0x20
.byte 0x1D
.byte 0x1D
.byte 0x1D
.byte 0x1D
.byte 0x1D
.byte 0x1D
.byte 0xFE
