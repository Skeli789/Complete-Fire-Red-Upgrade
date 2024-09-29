.thumb
.align 2

.include "../xse_commands.s"
.include "../xse_defines.s"

.global EventScript_PalletTown_FatGuy
EventScript_PalletTown_FatGuy:
    setflag 0xa03
    msgbox gText_PalletTown_FatGuy MSG_FACE
    end
    
