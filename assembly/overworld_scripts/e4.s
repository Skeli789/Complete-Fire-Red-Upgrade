.thumb
.align 2

.include "../xse_commands.s"
.include "../xse_defines.s"
.include "../asm_defines.s"

.global EventScript_IndigoPlateauMapScript
EventScript_IndigoPlateauMapScript:
	.byte MAP_SCRIPT_TERMIN

.global EventScript_GirlBlockingDoor
EventScript_GirlBlockingDoor: 
	lock
	checkflag 0x1008
	if SET _goto EventScript_DescribeReins
	msgbox gText_IfHaveSteeds MSG_FACE
	fanfare 0x101
	additem ITEM_REINS_OF_UNITY 0x1
	setvar 0x8004 ITEM_REINS_OF_UNITY
	setvar 0x8006 0x0
	special 0xE4
	msgbox gText_PlayerRecievedReins MSG_KEEPOPEN
	waitfanfare
	closeonkeypress
	special 0xE5
	setflag 0x1008
	setvar 0x8000 0x2EC
	setvar 0x8001 MOVE_WATERPULSE
	setvar 0x8002 0xFFFF
	setvar 0x8003 0xFFFF
	setvar 0x8004 0x0
	setvar 0x8005 0x0
	setvar 0x8006 31
	setvar 0x8007 31
	setvar 0x8008 31
	setvar 0x8009 31
	setvar 0x800A 31
	setvar 0x800B 31
	givepokemon SPECIES_YVELTAL 100 0x0 0x0 0x1
	msgbox gText_HeresRustySwordShield MSG_KEEPOPEN
	giveitem ITEM_RUSTED_SWORD 0x1 MSG_OBTAIN
	giveitem ITEM_RUSTED_SHIELD 0x1 MSG_OBTAIN
	closeonkeypress
	goto EventScript_DescribeReins
	release
	end

EventScript_DescribeReins:
	msgbox gText_HowUseReins MSG_FACE
	release
	end

.global EventScript_MegaSeller
EventScript_MegaSeller:
	lock
	msgbox gText_SellMegaStones MSG_FACE
	pokemart EventScript_MegaMart
	release
	end

.global EventScript_Chansey
EventScript_Chansey:
	lock
	msgbox gText_ChanseyE4 MSG_FACE
	fadescreen 0x1
	special 0x0
	fadescreen 0x0
	msgbox gText_ChanseyE4 MSG_FACE
	release
	end















.align 2
EventScript_MegaMart:
	.hword ITEM_BEEDRILLITE
	.hword ITEM_PIDGEOTITE
	.hword ITEM_ALAKAZITE
	.hword ITEM_SLOWBRONITE
	.hword ITEM_GENGARITE
	.hword ITEM_KANGASKHANITE
	.hword ITEM_PINSIRITE
	.hword ITEM_GYARADOSITE
	.hword ITEM_AERODACTYLITE
	.hword 0x0