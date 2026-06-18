.align 2
 .thumb
 
 .include "../xse_commands.s"
 .include "../xse_defines.s"
 
 .global EventScript_PremiumReward
 EventScript_PremiumReward:
 bufferitem 0x0 0x8000
 buffernumber 0x1 0x8001
 msgbox gText_ReceivedBonusItem MSG_NORMAL
 end
 