.text
.thumb
.align 2

.include "..\\defines"

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
