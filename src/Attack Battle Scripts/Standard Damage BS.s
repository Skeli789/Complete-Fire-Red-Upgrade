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
missmessage
flash BANK_TARGET
waitstateatk
graphicalhpupdate BANK_TARGET
datahpupdate BANK_TARGET
critmessage
waitmessage DELAY_1SECOND
resultmessage
waitmessage DELAY_1SECOND
return
