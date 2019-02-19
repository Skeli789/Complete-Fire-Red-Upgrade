.text
.thumb
.align 2

.include "..\\defines"

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
.global BattleScript_ButItFailed
FailedPreBS:
	attackstring
	ppreduce
FAILED:
BattleScript_ButItFailed:
	pause DELAY_HALFSECOND
	orbyte OUTCOME OUTCOME_FAILED
	resultmessage
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END
