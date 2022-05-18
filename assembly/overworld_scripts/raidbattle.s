.thumb
.align 2

.include "../xse_commands.s"
.include "../xse_defines.s"
.include "../asm_defines.s"

.equ SPECIAL_IS_RAID_BATTLE_AVAILABLE, 0x115
.equ SPECIAL_RAID_BATTLE_INTRO, 0x116
.equ SPECIAL_CREATE_RAID_MON, 0x117
.equ SPECIAL_START_RAID_BATTLE, 0x118 
.equ SPECIAL_SET_RAID_BATTLE_FLAG, 0x119
.equ SPECIAL_GIVE_RAID_BATTLE_REWARDS, 0x11C
.equ FLAG_TAG_BATTLE, 0x908
.equ FLAG_DYNAMAX_BATTLE, 0x918
.equ VAR_DAILY_EVENT, 0x504C
.equ SPECIAL_CLEAR_RAID_FLAG, 0x11A
.equ SP_DAILY_EVENT, 0xA0
.equ SP_UPDATE_TIME_IN_VARS, 0xA1
.equ SP_GET_TIME_DIFFERENCE, 0xA2
.equ VAR_ROUTE3, 0x5106

.global EventScript_BeginARaidBattle
EventScript_BeginARaidBattle: 
    special SPECIAL_IS_RAID_BATTLE_AVAILABLE
    compare LASTRESULT 0x1
    if equal _goto RaidScript_DoStuff
    setvar 0x8000 VAR_DAILY_EVENT
	setvar 0x8001 0x0
	special2 LASTRESULT SP_DAILY_EVENT
	compare LASTRESULT 0x0
	if notequal _goto EventScript_ClearRaidFlag
    goto RaidScript_NoBattle
    @@special SPECIAL_IS_RAID_BATTLE_AVAILABLE
    @@compare LASTRESULT 0x0
    @@if equal _goto RaidScript_DoStuff
    release
    end

RaidScript_DoStuff:
    msgbox gText_StartRaidBattle MSG_YESNO
    compare LASTRESULT 0x1 
    if 0x1 _goto EventScript_RaidScreen
    release
    end

EventScript_RaidScreen:
    fadescreen FADEOUT_BLACK
    special SPECIAL_RAID_BATTLE_INTRO
    waitstate
    compare LASTRESULT 0x0
    if equal _goto RaidScript_End
    setflag FLAG_TAG_BATTLE
    msgbox gText_JoinedYouTake MSG_SIGN
    special SPECIAL_CREATE_RAID_MON
    special SPECIAL_START_RAID_BATTLE
    waitstate
    special2 LASTRESULT 0xB4 @Get battle outcome
    compare LASTRESULT 0x4 @Ran
    if equal _goto RaidScript_End
    compare LASTRESULT 0x5 @Teleported / Lost Battle
    if equal _goto RaidScript_End
    setvar 0x8000 VAR_DAILY_EVENT
	setvar 0x8001 0x1
	special2 LASTRESULT SP_DAILY_EVENT
    @@setvar 0x8000 VAR_DAILY_EVENT
	@@setvar 0x8001 0x0
	@@special2 LASTRESULT SP_DAILY_EVENT
	@@compare LASTRESULT 0x0
	call EventScript_SetRaidFlag
    setvar 0x4000 0x0
    
RaidScript_GiveReward:
    special SPECIAL_GIVE_RAID_BATTLE_REWARDS
    compare LASTRESULT 0x0
    if notequal _goto RaidScript_End @Done giving rewards
    callstd MSG_FIND
    goto RaidScript_GiveReward

RaidScript_End:
    release
    end

RaidScript_End2:
	special 0x28
    release
    end

RaidScript_NoBattle:
    setvar 0x8000 VAR_DAILY_EVENT
    setvar 0x8001 0x1 
    special2 LASTRESULT SP_GET_TIME_DIFFERENCE
    buffernumber 0x1 LASTRESULT
    msgbox gText_DenEmpty MSG_NORMAL
    release
    end


EventScript_SetRaidFlag:
    special SPECIAL_SET_RAID_BATTLE_FLAG
    return

EventScript_ClearRaidFlag:
    setvar 0x8000 0x1
    special SPECIAL_CLEAR_RAID_FLAG
    goto RaidScript_DoStuff
    release
    end