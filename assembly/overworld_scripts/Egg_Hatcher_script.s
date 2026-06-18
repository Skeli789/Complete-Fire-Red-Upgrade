.thumb
.align 2

.include "../xse_commands.s"
.include "../xse_defines.s"
.include "../asm_defines.s"
.global EventScript_EggHatcher
EventScript_EggHatcher:
    faceplayer
    lock
    msgbox TextAsk MSG_YESNO
    compare LASTRESULT 0x1
    if 0x1 _goto Hatch_Yes
    if 0x0 _goto Hatch_No
    release
    end

Hatch_Yes:
    checkmoney 0x1388
    compare LASTRESULT 0x1
    if 0x0 _goto NoMoney
    removemoney 0x1388
    msgbox TextProcessing MSG_NORMAL
    waitmsg
    special 0x9F
    waitstate
    callasm Egg_hatcher_asm
    compare LASTRESULT 0x0
    if 0x1 _goto Cancel
    compare LASTRESULT 0x7
    if 0x1 _goto Hatch_No
    release
    end

Hatch_No:
    msgbox TextNo MSG_NORMAL
    release
    end

Cancel:
    msgbox TextCancel MSG_NORMAL
    release
    end

NoMoney:
    msgbox TextNoMoney MSG_NORMAL
    release
    end
