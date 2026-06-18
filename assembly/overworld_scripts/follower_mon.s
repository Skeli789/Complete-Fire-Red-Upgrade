.align 2
.thumb

.include "../xse_commands.s"
.include "../xse_defines.s"

.global EventScript_FollowerMon

EventScript_FollowerMon:
    followerfaceplayer
    lock
    storemonid
    bufferpokemon 0x0 0x4004
    callasm ShowMysteryGiftMon
    cry 0x4004 0x0
    callasm ShowAnonymousFollowerMessage
    msgbox gStringVar4 MSG_NORMAL
    callasm Remove_PokemonPic
    release
    end
    