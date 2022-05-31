.thumb
.align 2

.include "../xse_commands.s"
.include "../xse_defines.s"
.include "../asm_defines.s"


.global EventScript_GreenphxBattle1
EventScript_GreenphxBattle1:
	lock
	faceplayer
	checktrainerflag 0x8
	if 0x1 _goto EventScript_GreenphxBattle_After
	msgbox gText_GreenphxHello MSG_YESNO
	compare LASTRESULT 0x1
	if 0x1 _goto EventScript_GreenphxBattle_Start
	msgbox gText_GreenphxNo MSG_FACE
	release
	end

EventScript_GreenphxBattle_Start:
	trainerbattle3 0x1 0x8 0x0 gText_GreenphxLose
	goto EventScript_GreenphxBattle_After

EventScript_GreenphxBattle_After:
	msgbox gText_GreenphxAfter MSG_FACE
	release
	end

.global EventScript_DanielAceBattle
 EventScript_DanielAceBattle:
	lock
	faceplayer
	checktrainerflag 0xB
	if 0x1 _goto EventScript_DanielAlreadyBattled
	msgbox gText_DanielHello MSG_YESNO
	compare LASTRESULT 0x1
	if 0x1 _goto EventScript_DanielBattle
	msgbox gText_DanielNo MSG_FACE
	release
	end

EventScript_DanielBattle:
	msgbox gText_DanielBefore MSG_FACE
	trainerbattle3 0x1 0xB 0x0 gText_DanielLose
	msgbox gText_DanielAfter MSG_FACE
	giveitem ITEM_HOUNDOOMINITE 0x1 MSG_OBTAIN
	giveitem ITEM_FIRE_STONE 0x3 MSG_OBTAIN
	giveitem ITEM_SUN_STONE 0x2 MSG_OBTAIN
	giveitem ITEM_HEAT_ROCK 0x2 MSG_OBTAIN
	goto EventScript_DanielAlreadyBattled 
	release
	end

EventScript_DanielAlreadyBattled:
	msgbox gText_DanielAlreadyBattled MSG_FACE
	release
	end
