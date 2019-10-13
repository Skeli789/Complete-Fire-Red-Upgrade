.text
.thumb
.align 2
/*
standard_damage_battle_scripts.s
	scripts for basic damage calc
*/

.include "../asm_defines.s"
.include "../battle_script_macros.s"

.global STANDARD_DAMAGE
STANDARD_DAMAGE:
	attackstring
	ppreduce
	calculatedamage
	attackanimation
	waitanimation
	effectivenesssound
	flash BANK_TARGET
	waitstateatk
	graphicalhpupdate BANK_TARGET
	datahpupdate BANK_TARGET
	critmessage
	waitmessage DELAY_1SECOND
	resultmessage
	waitmessage DELAY_1SECOND
	return

.global FAILED
.global FAILED_PRE
.global BattleScript_ButItFailedAttackstring
.global BattleScript_ButItFailed
FailedPreBS:
BattleScript_ButItFailedAttackstring:
FAILED_PRE:
	attackstring
	ppreduce
FAILED:
BattleScript_ButItFailed:
	pause DELAY_HALFSECOND
	orbyte OUTCOME OUTCOME_FAILED
	resultmessage
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END
