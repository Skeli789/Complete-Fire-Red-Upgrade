.text
.thumb
.align 2

.include "..\\defines"

.global StandardDamage
StandardDamage:
attackstring
ppreduce
calculatedamage
attackanimation
waitanimation
missmessage
flash 0x0
waitstateatk
graphicalhpupdate BANK_TARGET
datahpupdate BANK_TARGET
critmessage
waitmessage DELAY_1SECOND
resultmessage
waitmessage DELAY_1SECOND
return
