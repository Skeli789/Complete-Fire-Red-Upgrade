.thumb
.align 2

.include "../xse_commands.s"
.include "../xse_defines.s"
.include "../asm_defines.s"


.global EventScript_Cut
EventScript_Cut:
	special 0x187
	compare LASTRESULT 0x2
	if 0x1 _goto EventScript_Cut_0X1A7AE0
	lockall
	checkflag 0x821
	if 0x0 _goto EventScript_Cut_0X1BDF87
	checkitem ITEM_HM01 0x1 
    compare LASTRESULT 0x1
	if lessthan _goto EventScript_Cut_0X1BDF87
	setvar 0x8009 0
	callasm MonCanLearnHM
	compare LASTRESULT 0x1
	if equal _goto EventScript_Cut_0X1BDF76
	goto EventScript_Cut_0X1BDF87

EventScript_Cut_0X1A7AE0:
	release
	end

EventScript_Cut_0X1BDF87:
	msgbox 0x81BDFE3 MSG_SIGN
	releaseall
	end

EventScript_Cut_0X1BDF91:
	closeonkeypress
	releaseall
	end

EventScript_Cut_0X1BDF76:
	setanimation 0x0 0x8008
	doanimation 0x2
	waitstate
	sound 0x79 @@#define SE_M_CUT                  121 // SE_W015
	applymovement LASTTALKED EventScript_Cut_0X1BDF85
	waitmovement 0x0
	hidesprite LASTTALKED
	releaseall
	end
EventScript_Cut_0X1BDF85:
.byte 0x69
.byte 0xFE


.global EventScript_RockSmash
EventScript_RockSmash:
	special 0x187
	compare LASTRESULT 0x2
	if 0x1 _goto EventScript_RockSmash_0X1A7AE0
	lockall
	checkflag 0x825
	if 0x0 _goto EventScript_RockSmash_0X1BE091
	checkitem ITEM_HM06 0x1 
    compare LASTRESULT 0x1
	if lessthan _goto EventScript_RockSmash_0X1BE091
	setvar 0x8009 1
	callasm MonCanLearnHM
	compare LASTRESULT 0x1
	if 0x0 _goto EventScript_RockSmash_0X1BE091
	setanimation 0x0 0x8008
	doanimation 0x25
	waitstate
	goto EventScript_RockSmash_0X1BE06F

	@---------------
EventScript_RockSmash_0X1A7AE0:
	release
	end

	@---------------
EventScript_RockSmash_0X1BE091:
	msgbox 0x81BE0E2 MSG_SIGN 
	end

	@---------------
EventScript_RockSmash_0X1BE09A:
	closeonkeypress
	releaseall
	end

	@---------------
EventScript_RockSmash_0X1BE06F:
	applymovement LASTTALKED 0x81BE08F
	waitmovement 0x0
	hidesprite LASTTALKED
	special 0xAB
	compare LASTRESULT 0x0
	if 0x1 _goto EventScript_RockSmash_0X1BE08D
	waitstate
	releaseall
	end

	@---------------
EventScript_RockSmash_0X1BE08D:
	releaseall
	end


.global EventScript_Strength
EventScript_Strength:
	special 0x187
	compare LASTRESULT 0x2
	if 0x1 _goto EventScript_Strength_0X1A7AE0
	lockall
	checkflag 0x823
	if 0x0 _goto EventScript_Strength_0X1BE185
	checkflag 0x805
	if 0x1 _goto EventScript_Strength_0X1BE18E
	checkitem ITEM_HM04 0x1 
    compare LASTRESULT 0x1
	if lessthan _goto EventScript_Strength_0X1BE185
	setvar 0x8009 2
	callasm MonCanLearnHM
	compare LASTRESULT 0x1
	if 0x0 _goto EventScript_Strength_0X1BE185
	goto EventScript_Strength_0X1BE179

	@---------------
EventScript_Strength_0X1A7AE0:
	release
	end

	@---------------
EventScript_Strength_0X1BE185:
	msgbox 0x81BE244 MSG_SIGN 
	end

	@---------------
EventScript_Strength_0X1BE18E:
	msgbox 0x81BE284 MSG_SIGN 
	end

	@---------------
EventScript_Strength_0X1BE197:
	closeonkeypress
	releaseall
	end

	@---------------
EventScript_Strength_0X1BE179:
	setanimation 0x0 0x8008
	doanimation 0x28
	waitstate
	bufferpokemon 0x0 0x800A
	setflag 0x805
	msgbox 0x81BE1FA MSG_SIGN 
	end




