.align 2
.thumb

.include "../xse_commands.s"
.include "../xse_defines.s"
.include "../asm_defines.s"

.global EventScript_Wonder_Trades

EventScript_Wonder_Trades:
    faceplayer
    lockall
    msgbox gText_String1 MSG_YESNO
    compare LASTRESULT 0x1
    if equal _goto Wonder_Trades_Snippet1
    msgbox gText_String2 MSG_NORMAL
    releaseall
    end

Wonder_Trades_Snippet1:
    countpokemon
    compare LASTRESULT 0x1
    if equal _goto Wonder_Trades_Snippet7
    msgbox gText_String3 MSG_NORMAL
    special 0x9F
    waitstate
    special2 0x8003 0x147
    compare 0x8003 0x19c
    if equal _goto Wonder_Trades_Snippet8
    compare 0x8004 0x6
    if greaterorequal _goto Wonder_Trades_Snippet3
    if lessthan _goto Wonder_Trades_Snippet4

Wonder_Trades_Snippet3:
    msgbox gText_String2 MSG_NORMAL
    releaseall
    end

Wonder_Trades_Snippet4:
    random 0x19C
    compare LASTRESULT 0x0
    if equal _goto Wonder_Trades_Snippet4
    compare LASTRESULT 0xFB
    if lessorequal _goto Wonder_Trades_Snippet5
    compare LASTRESULT 0x115
    if greaterorequal _goto Wonder_Trades_Snippet5
    if lessthan _goto Wonder_Trades_Snippet4

Wonder_Trades_Snippet5:
    copyvar 0x8000 LASTRESULT
    bufferpokemon 0x0 0x8000
    showpokepic 0x8000 0xA 0x3
    waitcry
    callasm TakePokemon+1
    givepokemon 0x8000 0x5 0x0 0x0 0x0 0x0
    pause 0x13
    fanfare 0x13E
    msgbox gText_String5 MSG_KEEPOPEN
    waitfanfare
    closeonkeypress
    hidepokepic
    msgbox gText_String6 MSG_YESNO
    compare LASTRESULT 0x1
    if equal _call Wonder_Trades_Snippet6
    msgbox gText_String2 MSG_NORMAL
    releaseall
    end

Wonder_Trades_Snippet6:
    countpokemon
    subvar LASTRESULT 0x1
    copyvar 0x8004 LASTRESULT
    fadescreen 0x1
    special 0x9E
    waitstate
    return

Wonder_Trades_Snippet7:
    msgbox gText_String7 MSG_NORMAL
    releaseall
    end

Wonder_Trades_Snippet8:
    msgbox gText_String8 MSG_NORMAL
    releaseall
    end
