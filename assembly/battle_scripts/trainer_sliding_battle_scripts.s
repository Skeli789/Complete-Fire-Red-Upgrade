.text
.thumb
.align 2
/*
trainer_sliding_battle_scripts.s
	scripts for handling mid-battle trainer sliding/messages
*/

.include "../defines"

.global BattleScript_TrainerSlideMsgRet
.global BattleScript_TrainerSlideMsgEnd2

BattleScript_TrainerSlideMsgRet:
	handletrainerslidemsg BANK_SCRIPTING 0
	trainerslidein 0x1
	printstring 0x184
	waitstateatk
	trainerslideout 0x1
	handletrainerslidemsg BANK_SCRIPTING 1
	waitstateatk
	return
	
BattleScript_TrainerSlideMsgEnd2:
	call BattleScript_TrainerSlideMsgRet
	end2
