.thumb
.align 2

.include "../xse_commands.s"
.include "../xse_defines.s"
.include "../asm_defines.s"

.global EventScript_DiglettTeleport1
EventScript_DiglettTeleport1:
	lock
	faceplayer
	msgbox gText_WantTeleport MSG_YESNO
	compare LASTRESULT 0x1
	if equal _goto EventScript_DiglettTeleport1Tele
	release
	end

EventScript_DiglettTeleport1Tele:
	msgbox gText_HereWeGo MSG_FACE
	warp 1 38 0xFF 0x7 0x3
	release
	end

.global EventScript_DiglettTeleport2
EventScript_DiglettTeleport2:
	lock
	faceplayer
	msgbox gText_WantTeleport MSG_YESNO
	compare LASTRESULT 0x1
	if equal _goto EventScript_DiglettTeleport2Tele
	release
	end

EventScript_DiglettTeleport2Tele:
	msgbox gText_HereWeGo MSG_FACE
	warp 1 36 0xFF 0x6 0x1
	release
	end

.global EventScript_DrillRunTutor
EventScript_DrillRunTutor:
	lock
	faceplayer
	msgbox gText_ShouldTeachDrillRun MSG_YESNO
	compare LASTRESULT 0x1
	if equal _goto EventScript_TeachDrillRun
	msgbox gText_DrillRunNo MSG_FACE
	release
	end

EventScript_TeachDrillRun:
	msgbox gText_WhoWantsLearnDrillRun MSG_FACE
	setvar 0x8005 35
	special 0x18D
	waitstate
	compare LASTRESULT 0x0
	if equal _goto EventScript_DrillRunNoTeach
	msgbox gText_TaughtDrillRun MSG_FACE
	release
	end

EventScript_DrillRunNoTeach:
	msgbox gText_DrillRunNo MSG_FACE
	release
	end