.thumb
.align 2

.include "../xse_commands.s"
.include "../xse_defines.s"
.include "../asm_defines.s"


.global EventScript_AutoRunTip
EventScript_AutoRunTip:
	lockall
	msgbox gText_LAutoRun MSG_KEEPOPEN 
	setvar 0x4070 0x1
	releaseall
	end

.global EventScript_AutoRunWild
EventScript_AutoRunWild:
	msgbox gText_RRun MSG_SIGN
	end

.global EventScript_TalkPeopleTip
EventScript_TalkPeopleTip:
	msgbox gText_TalkPeople MSG_SIGN
	end

.global EventScript_PokedexStats
EventScript_PokedexStats:
	msgbox gText_StatsPokedex MSG_SIGN
	end

.global EventScript_70NewTms
EventScript_70NewTms:
	msgbox gText_70NewTMS MSG_SIGN
	end

.global EventScript_NewAreaForest
EventScript_NewAreaForest:
	msgbox gText_NewForestArea MSG_SIGN
	end

.global EventScript_SortBagTip
EventScript_SortBagTip:
	msgbox gText_SortBagTip MSG_SIGN
	end

.global EventScript_ManPokecentre
EventScript_ManPokecentre:
	msgbox gText_ManPokecentre MSG_SIGN
	end

.global EventScript_LAEvolve
EventScript_LAEvolve:
	msgbox gText_LAEvolve MSG_SIGN
	end
