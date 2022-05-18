.thumb
.align 2

.include "../xse_commands.s"
.include "../xse_defines.s"
.include "../asm_defines.s"

.global EventScript_RestoreFossilNPC
EventScript_RestoreFossilNPC:
	lock
	msgbox gText_RestoreFossilsHello MSG_FACE
	setvar 0x8000 0x3
	setvar 0x8001 0x6
	setvar 0x8004 0x0
	special 0x158
	waitstate
	compare LASTRESULT 0x0
	if 0x1 _goto EventScript_RestoreFossil_Dome
	compare LASTRESULT 0x1
	if 0x1 _goto EventScript_RestoreFossil_Helix
	compare LASTRESULT 0x2
	if 0x1 _goto EventScript_RestoreFossil_Root
	compare LASTRESULT 0x3
	if 0x1 _goto EventScript_RestoreFossil_Claw
	compare LASTRESULT 0x4
	if 0x1 _goto EventScript_RestoreFossil_Skull
	compare LASTRESULT 0x5
	if 0x1 _goto EventScript_RestoreFossil_Armor
	compare LASTRESULT 0x6
	if 0x1 _goto EventScript_RestoreFossil_Cover
	compare LASTRESULT 0x7
	if 0x1 _goto EventScript_RestoreFossil_Plume
	compare LASTRESULT 0x8
	if 0x1 _goto EventScript_RestoreFossil_Jaw
	compare LASTRESULT 0x9
	if 0x1 _goto EventScript_RestoreFossil_Sail
	release
	end

EventScript_RestoreFossil_Dome:
	bufferitem 0x0 ITEM_DOME_FOSSIL
	checkitem ITEM_DOME_FOSSIL 0x1
	compare LASTRESULT 0x1
	if greaterorequal _goto EventScript_RestoreFossil_DomeGive
	msgbox gText_DontHaveFossil MSG_FACE
	setflag 0x95A
	release
	end

EventScript_RestoreFossil_DomeGive:
	removeitem ITEM_DOME_FOSSIL 0x1
	givepokemon SPECIES_KABUTO 0x14 0x0 0x0 0x0
	bufferpokemon 0x0 SPECIES_KABUTO
	fanfare 0x13E
	msgbox gText_GetFossilMon MSG_KEEPOPEN
	waitfanfare 
	closeonkeypress
	release
	end

EventScript_RestoreFossil_Helix:
	bufferitem 0x0 ITEM_HELIX_FOSSIL
	checkitem ITEM_HELIX_FOSSIL 0x1
	compare LASTRESULT 0x1
	if greaterorequal _goto EventScript_RestoreFossil_HelixGive
	msgbox gText_DontHaveFossil MSG_FACE
	setflag 0x95A
	release
	end

EventScript_RestoreFossil_HelixGive:
	removeitem ITEM_HELIX_FOSSIL 0x1
	givepokemon SPECIES_OMANYTE 0x14 0x0 0x0 0x0
	bufferpokemon 0x0 SPECIES_OMANYTE
	fanfare 0x13E
	msgbox gText_GetFossilMon MSG_KEEPOPEN
	waitfanfare 
	closeonkeypress
	release
	end

EventScript_RestoreFossil_Root:
	bufferitem 0x0 ITEM_ROOT_FOSSIL
	checkitem ITEM_ROOT_FOSSIL 0x1
	compare LASTRESULT 0x1
	if greaterorequal _goto EventScript_RestoreFossil_RootGive
	msgbox gText_DontHaveFossil MSG_FACE
	setflag 0x95A
	release
	end

EventScript_RestoreFossil_RootGive:
	removeitem ITEM_ROOT_FOSSIL 0x1
	givepokemon SPECIES_LILEEP 0x14 0x0 0x0 0x0
	bufferpokemon 0x0 SPECIES_LILEEP 
	fanfare 0x13E
	msgbox gText_GetFossilMon MSG_KEEPOPEN
	waitfanfare 
	closeonkeypress
	release
	end

EventScript_RestoreFossil_Claw:
	bufferitem 0x0 ITEM_CLAW_FOSSIL
	checkitem ITEM_CLAW_FOSSIL 0x1
	compare LASTRESULT 0x1
	if greaterorequal _goto EventScript_RestoreFossil_ClawGive
	msgbox gText_DontHaveFossil MSG_FACE
	release
	end

EventScript_RestoreFossil_ClawGive:
	removeitem ITEM_CLAW_FOSSIL 0x1
	givepokemon SPECIES_ANORITH 0x14 0x0 0x0 0x0
	bufferpokemon 0x0 SPECIES_ANORITH 
	fanfare 0x13E
	msgbox gText_GetFossilMon MSG_KEEPOPEN
	waitfanfare 
	closeonkeypress
	release
	end

EventScript_RestoreFossil_Skull:
	bufferitem 0x0 ITEM_SKULL_FOSSIL
	checkitem ITEM_SKULL_FOSSIL 0x1
	compare LASTRESULT 0x1
	if greaterorequal _goto EventScript_RestoreFossil_SkullGive
	msgbox gText_DontHaveFossil MSG_FACE
	release
	end

EventScript_RestoreFossil_SkullGive:
	removeitem ITEM_SKULL_FOSSIL 0x1
	givepokemon SPECIES_CRANIDOS 0x14 0x0 0x0 0x0
	bufferpokemon 0x0 SPECIES_CRANIDOS 
	fanfare 0x13E
	msgbox gText_GetFossilMon MSG_KEEPOPEN
	waitfanfare 
	closeonkeypress
	release
	end

EventScript_RestoreFossil_Armor:
	bufferitem 0x0 ITEM_ARMOR_FOSSIL
	checkitem ITEM_ARMOR_FOSSIL 0x1
	compare LASTRESULT 0x1
	if greaterorequal _goto EventScript_RestoreFossil_ArmorGive
	msgbox gText_DontHaveFossil MSG_FACE
	release
	end

EventScript_RestoreFossil_ArmorGive:
	removeitem ITEM_ARMOR_FOSSIL 0x1
	givepokemon SPECIES_SHIELDON 0x14 0x0 0x0 0x0
	bufferpokemon  0x0 SPECIES_SHIELDON
	fanfare 0x13E
	msgbox gText_GetFossilMon MSG_KEEPOPEN
	waitfanfare 
	closeonkeypress
	release
	end

EventScript_RestoreFossil_Cover:
	bufferitem 0x0 ITEM_COVER_FOSSIL
	checkitem ITEM_COVER_FOSSIL 0x1
	compare LASTRESULT 0x1
	if greaterorequal _goto EventScript_RestoreFossil_CoverGive
	msgbox gText_DontHaveFossil MSG_FACE
	release
	end

EventScript_RestoreFossil_CoverGive:
	removeitem ITEM_COVER_FOSSIL 0x1
	givepokemon SPECIES_TIRTOUGA 0x14 0x0 0x0 0x0
	bufferpokemon 0x0 SPECIES_TIRTOUGA 
	fanfare 0x13E
	msgbox gText_GetFossilMon MSG_KEEPOPEN
	waitfanfare 
	closeonkeypress
	release
	end

EventScript_RestoreFossil_Plume:
	bufferitem 0x0 ITEM_PLUME_FOSSIL
	checkitem ITEM_PLUME_FOSSIL 0x1
	compare LASTRESULT 0x1
	if greaterorequal _goto EventScript_RestoreFossil_PlumeGive
	msgbox gText_DontHaveFossil MSG_FACE
	release
	end

EventScript_RestoreFossil_PlumeGive:
	removeitem ITEM_PLUME_FOSSIL 0x1
	givepokemon SPECIES_ARCHEN 0x14 0x0 0x0 0x0
	bufferpokemon 0x0 SPECIES_ARCHEN
	fanfare 0x13E
	msgbox gText_GetFossilMon MSG_KEEPOPEN
	waitfanfare 
	closeonkeypress
	release
	end

EventScript_RestoreFossil_Jaw:
	bufferitem 0x0 ITEM_JAW_FOSSIL
	checkitem ITEM_JAW_FOSSIL 0x1
	compare LASTRESULT 0x1
	if greaterorequal _goto EventScript_RestoreFossil_JawGive
	msgbox gText_DontHaveFossil MSG_FACE
	release
	end

EventScript_RestoreFossil_JawGive:
	removeitem ITEM_JAW_FOSSIL 0x1
	givepokemon SPECIES_TYRUNT 0x14 0x0 0x0 0x0
	bufferpokemon 0x0 SPECIES_TYRUNT 
	fanfare 0x13E
	msgbox gText_GetFossilMon MSG_KEEPOPEN
	waitfanfare 
	closeonkeypress
	release
	end

EventScript_RestoreFossil_Sail:
	bufferitem 0x0 ITEM_SAIL_FOSSIL
	checkitem ITEM_SAIL_FOSSIL 0x1
	compare LASTRESULT 0x1
	if greaterorequal _goto EventScript_RestoreFossil_SailGive
	msgbox gText_DontHaveFossil MSG_FACE
	release
	end

EventScript_RestoreFossil_SailGive:
	removeitem ITEM_SAIL_FOSSIL 0x1
	givepokemon SPECIES_AMAURA 0x14 0x0 0x0 0x0
	bufferpokemon 0x0 SPECIES_AMAURA
	fanfare 0x13E
	msgbox gText_GetFossilMon MSG_KEEPOPEN
	waitfanfare 
	closeonkeypress
	release
	end

.global EventScript_ElementalPunches
EventScript_ElementalPunches:
	lock
	faceplayer
	msgbox gText_ElementalPunchesHi MSG_YESNO
	closeonkeypress
	compare LASTRESULT 0x1
	if 0x1 _goto EventScript_SelectPokemonPunch
	msgbox gText_ElementalPunchNo MSG_FACE
	release
	end

EventScript_SelectPokemonPunch:
	msgbox gText_ElementalPunchSelect MSG_KEEPOPEN
	setvar 0x8000 0x4
	setvar 0x8001 0x6
	setvar 0x8004 0x0
	special 0x158
	waitstate
	closeonkeypress
	compare LASTRESULT 0x0
	if 0x1 _goto EventScript_FirePunch
	compare LASTRESULT 0x1
	if 0x1 _goto EventScript_IcePunch
	compare LASTRESULT 0x2
	if 0x1 _goto EventScript_ThunderPunch
	release
	end

EventScript_FirePunch:
	setvar 0x8005 0x0
	goto EventScript_ElementalTeach

EventScript_IcePunch:
	setvar 0x8005 0x1
	goto EventScript_ElementalTeach

EventScript_ThunderPunch:
	setvar 0x8005 0x2
	goto EventScript_ElementalTeach

EventScript_ElementalTeach:
	special 0x18D
	waitstate
	release
	end
	
.global EventScript_DeoxysFormChange
EventScript_DeoxysFormChange:
	lock
	givepokemon SPECIES_DEOXYS 100 0x0 0x0 0x0
	callasm IsDeoxys
	compare LASTRESULT 0x1
	if equal _goto EventScript_SelectDeoxys
	msgbox gText_StrangeMeteorite MSG_NORMAL
	release
	end

EventScript_SelectDeoxys: 
	msgbox gText_DeoxysReact MSG_NORMAL
	msgbox gText_BringDeoxys MSG_YESNO
	compare LASTRESULT 0x1
	if equal _goto EventScript_DeoxysForms
	release
	end


EventScript_DeoxysForms:
	copyvar 0x8004 0x8006
	setvar 0x8003 0x0
	callasm SetDeoxysForm
	copyvar 0x8003 0x8006
	special 0x16
	compare 0x8008 0x0
	if equal _goto EventScript_DeoxysAttack
	compare 0x8008 0x1
	if equal _goto EventScript_DeoxysDefense
	compare 0x8008 0x2
	if equal _goto EventScript_DeoxysSpeed
	compare 0x8008 0x3
	if equal _goto EventScript_DeoxysNormal
	release
	end

EventScript_DeoxysAttack:
	msgbox gText_DeoxysAttack MSG_NORMAL
	return

EventScript_DeoxysDefense:
	msgbox gText_DeoxysDefense MSG_NORMAL
	return

EventScript_DeoxysSpeed:
	msgbox gText_DeoxysSpeed MSG_NORMAL
	return

EventScript_DeoxysNormal:
	msgbox gText_DeoxysNormal MSG_NORMAL
	return