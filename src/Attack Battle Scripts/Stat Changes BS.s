.text
.thumb
.align 2

.include "..\\defines"

.global PowerTrickString
.global PowerSwapString
.global GuardSwapString
.global SpeedSwapString
.global HeartSwapString
.global PowerSplitString
.global GuardSplitString
.global PowerHerbString

@This File Includes:						Length:		Working:
@BS 10: Raise User Atk 1					Medium		Yes
@BS 11: Raise User Def 1					Long		Yes - Test Doubles
@BS 13: Raise User Sp. Atk 1				Long		Yes - Test Doubles
@BS 16: Raise User Evsn 1					Long		Yes - Test Doubles
@BS 18: Lower Target Atk 1					Medium		Yes
@BS 19: Lower Target Def 1					No Change	No Test Required
@BS 20: Lower Target Spd 1					No Change	No Test Required
@BS 21: Lower Target Sp. Atk 1				Short		Yes
@BS 23: Lower Target Acc 1					No Change	No Test Required
@BS 24: Lower Target Evsn 1					No Change	No Test Required
@BS 25: Haze								No Change	No Test Required
@BS 46: Mist								No Change	No Test Required
@BS 50: Raise User Atk 2					No Change	No Test Required
@BS 51: Raise User Def 2					Short		Yes
@BS 52: Raise User Spd 2					Short		Yes
@BS 53: Raise User Sp. Atk 2				Short		Yes
@BS 54: Raise User Sp. Def 2				No Change	No Test Required
@BS 58: Lower Target Atk 2					No Change	No Test Required
@BS 59: Lower Target Def 2					No Change	No Test Required
@BS 60: Lower Target Spd 2					No Change	No Test Required
@BS 61: Lower Target Sp. Atk 2				Short		Yes
@BS 68: Lower Target Atk 1 Chance			No Change	No Test Required
@BS 69: Lower Target Def 1 Chance			No Change	No Test Required
@BS 70: Lower Target Spd 1 Chance			Short		Yes
@BS 71: Lower Target Sp. Atk 1 Chance		No Change	No Test Required
@BS 72: Lower Target Sp. Def 1 Chance		Short		Yes
@BS 73: Lower Target Acc 1 Chance			No Change	No Test Required
@BS 108: Minimize							Short		Yes	
@BS 118: Swagger							No Change	No Test Required
@BS 138: Raise User Def 1 Chance			Short		Yes
@BS 139 Raise User Atk 1 Chance				Short		Yes
@BS 140: Raise User All Stats Chance		No Change	No Test Required
@BS 143: Psych Up							Medium		Yes
@BS 156: Defense Curl						No Change	No Test Required
@BS 166: Flatter							No Change	No Test Required
@BS 174: Charge								Short		Yes
@BS 182: Superpower							Long		Yes - Fix Substitute for Hyperspace moves
@BS 204: Damage + Lower User Sp. Atk 2 		No Change	No Test Required
@BS 205: Tickle								No Change	No Test Required
@BS 206: Cosmic Power						No Change	No Test Required
@BS 208: Bulk Up							Medium		Yes
@BS 211: Calm Mind							Long		Yes
@BS 212: Dragon Dance						Medium		Yes
@BS 213: Stat Swap Splitters				Long		Yes
@BS 222: Clear Smog							Short		Yes
@BS 230: Feint								Medium		Yes - Check Doubles

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

@.org 0x1D7742, 0xFF
MinimizeUpdate:
	setbyte STAT_CHANGE_BYTE STAT_EVSN | INCREASE_2

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

@MOVE_EFFECT_TABLE Offset: 0x8907900
@.org MOVE_EFFECT_TABLE + (10 * 4) - rom, 0xFF
.global RaiseUserAtk1BS 

@.org MOVE_EFFECT_TABLE + (11 * 4) - rom, 0xFF
.global RaiseUserDef1BS 

@.org MOVE_EFFECT_TABLE + (13 * 4) - rom, 0xFF
.global RaiseUserSpAtk1BS 

@.org MOVE_EFFECT_TABLE + (16 * 4) - rom, 0xFF
.global RaiseUserEvsn1BS 

@.org MOVE_EFFECT_TABLE + (18 * 4) - rom, 0xFF
.global LowerTargetAtk1BS 

@.org MOVE_EFFECT_TABLE + (21 * 4) - rom, 0xFF
.global LowerTargetSpAtk1BS 

@.org MOVE_EFFECT_TABLE + (51 * 4) - rom, 0xFF
.global RaiseUserDef2BS 

@.org MOVE_EFFECT_TABLE + (52 * 4) - rom, 0xFF
.global RaiseUserSpd2BS 

@.org MOVE_EFFECT_TABLE + (53 * 4) - rom, 0xFF
.global RaiseUserSpAtk2BS 

@.org MOVE_EFFECT_TABLE + (61 * 4) - rom, 0xFF
.global LowerTargetSpAtk2BS 

@.org MOVE_EFFECT_TABLE + (70 * 4) - rom, 0xFF
.global LowerTargetSpd1ChanceBS 

@.org MOVE_EFFECT_TABLE + (72 * 4) - rom, 0xFF
.global LowerTargetSpDef1ChanceBS 

@.org MOVE_EFFECT_TABLE + (138 * 4) - rom, 0xFF
.global SteelWingBS 

@.org MOVE_EFFECT_TABLE + (139 * 4) - rom, 0xFF
.global MetalClawBS 

@.org MOVE_EFFECT_TABLE + (143 * 4) - rom, 0xFF
.global PsychUpBS 

@.org MOVE_EFFECT_TABLE + (174 * 4) - rom, 0xFF
.global ChargeBS 

@.org MOVE_EFFECT_TABLE + (182 * 4) - rom, 0xFF
.global SuperpowerBS 

@.org MOVE_EFFECT_TABLE + (208 * 4) - rom, 0xFF
.global BulkUpBS 

@.org MOVE_EFFECT_TABLE + (211 * 4) - rom, 0xFF
.global CalmMindBS 

@.org MOVE_EFFECT_TABLE + (212 * 4) - rom, 0xFF
.global DragonDanceBS 

@.org MOVE_EFFECT_TABLE + (213 * 4) - rom, 0xFF
.global StatSwapSplittersBS 

@.org MOVE_EFFECT_TABLE + (222 * 4) - rom, 0xFF
.global ClearSmogBS 

@.org MOVE_EFFECT_TABLE + (230 * 4) - rom, 0xFF
.global FeintBS 

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

RaiseUserAtk1BS:
	jumpifhalfword NOTEQUALS CURRENT_MOVE MOVE_HONECLAWS 0x81D6B77

HoneClawsBS:
	attackcanceler
	
attackstring
	ppreduce
	jumpifstat BANK_TARGET LESSTHAN STAT_ATK STAT_MAX HoneClaws_Atk
	jumpifstat BANK_TARGET EQUALS STAT_ACC STAT_MAX 0x81D85E7

HoneClaws_Atk:
	attackanimation
	waitanimation
	setbyte 0x2023FDF 0x0
	playstatchangeanimation BANK_ATTACKER STAT_ANIM_ATK | STAT_ANIM_ACC STAT_ANIM_UP
	setbyte STAT_CHANGE_BYTE STAT_ATK | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR HoneClaws_Acc
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 HoneClaws_Acc
	printfromtable 0x83FE57C
	waitmessage DELAY_1SECOND

HoneClaws_Acc:
	setbyte STAT_CHANGE_BYTE STAT_ACC | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR 0x81D694E
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 0x81D694E
	printfromtable 0x83FE57C
	waitmessage DELAY_1SECOND
	goto 0x81D694E

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

RaiseUserDef1BS:
	attackcanceler
	attackstring
	ppreduce
	setbyte STAT_CHANGE_BYTE STAT_DEF | INCREASE_1
	jumpifhalfword EQUALS CURRENT_MOVE MOVE_FLOWERSHIELD RaiseUserDef1_FlowerShield
	jumpifhalfword EQUALS CURRENT_MOVE MOVE_MAGNETICFLUX RaiseUserDef1_MagneticFlux
	jumpifhalfword EQUALS CURRENT_MOVE MOVE_AROMATICMIST RaiseUserDef1_AromaticMist
	goto 0x81D6BA1

RaiseUserDef1_FlowerShield:
	setbyte FORM_COUNTER TYPE_GRASS
	callasm CheckIfTypePresent+1
	jumpifbyte NOTEQUALS FORM_COUNTER TRUE FAILED
	setword SEED_HELPER 0x0
	setbyte FLOWER_SHIELD_HELPER 0x0
	setbyte FORM_COUNTER 0x0

FlowerShieldBSLoop:
	callasm FlowerShieldLooper+1
	jumpifbyte EQUALS FORM_COUNTER 0xFF FlowerShieldEnder

RaiseUserDef1_BoostDefense:
	setbyte 0x2023FDF 0x0
	jumpifstat BANK_TARGET EQUALS STAT_DEF STAT_MAX FlowerShieldBSLoop
	attackanimation
	waitanimation
	playstatchangeanimation BANK_TARGET STAT_ANIM_DEF STAT_ANIM_UP
	setbyte STAT_CHANGE_BYTE STAT_DEF | INCREASE_1
	statbuffchange STAT_TARGET | STAT_BS_PTR FlowerShieldBSLoop
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 FlowerShieldBSLoop
	printfromtable 0x83FE57C
	waitmessage DELAY_1SECOND
	setbyte FLOWER_SHIELD_HELPER TRUE
	goto FlowerShieldBSLoop

FlowerShieldEnder:
	setword SEED_HELPER 0x0
	setbyte FORM_COUNTER 0x0
	jumpifbyte 0x0 FLOWER_SHIELD_HELPER 0x0 FAILED
	setbyte FLOWER_SHIELD_HELPER 0x0
	goto 0x81D694E




RaiseUserDef1_MagneticFlux:
	copyarray TARGET_BANK USER_BANK 0x1
	jumpifabilitypresenttargetfield ABILITY_PLUS RaiseUserDef1_MagneticBoost
	jumpifabilitypresenttargetfield ABILITY_MINUS RaiseUserDef1_MagneticBoost
	goto FAILED

RaiseUserDef1_MagneticBoost:
	setbyte FLOWER_SHIELD_HELPER 0x0
	setbyte SEED_HELPER 0x0
RaiseUserDef1_MagneticLoop:
	jumpifability EQUALS ABILITY_PLUS RaiseUserDef1_BoostDefandSpDef
	jumpifability EQUALS ABILITY_MINUS RaiseUserDef1_BoostDefandSpDef
	goto RaiseUserDef1_Skip2

RaiseUserDef1_BoostDefandSpDef:
	jumpifstat BANK_TARGET NOTEQUALS STAT_DEF STAT_MAX RaiseUserDef1_Def
	jumpifstat BANK_TARGET EQUALS STAT_SP_DEF STAT_MAX RaiseUserDef1_Skip2

RaiseUserDef1_Def:
	attackanimation
	waitanimation
	setbyte FLOWER_SHIELD_HELPER 0x1
	setbyte 0x2023FDF 0x0
	playstatchangeanimation BANK_TARGET STAT_ANIM_DEF | STAT_ANIM_SPDEF STAT_ANIM_UP
	jumpifstat BANK_TARGET EQUALS STAT_DEF STAT_MAX RaiseUserDef1_SpDef
	setbyte STAT_CHANGE_BYTE STAT_DEF | INCREASE_1
	statbuffchange STAT_TARGET | STAT_BS_PTR RaiseUserDef1_SpDef
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 RaiseUserDef1_SpDef
	printfromtable 0x83FE57C
	waitmessage DELAY_1SECOND

RaiseUserDef1_SpDef:
	setbyte STAT_CHANGE_BYTE STAT_SPDEF | INCREASE_1
	jumpifstat BANK_TARGET EQUALS STAT_SP_DEF STAT_MAX RaiseUserDef1_Skip2
	statbuffchange STAT_TARGET | STAT_BS_PTR RaiseUserDef1_Skip2
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 RaiseUserDef1_Skip2
	printfromtable 0x83FE57C
	waitmessage DELAY_1SECOND

RaiseUserDef1_Skip2:
	addbyte SEED_HELPER 0x1
	jumpifbyte NOTEQUALS NUM_POKEMON 0x4 FlowerShieldEnder
	jumpifbyte EQUALS SEED_HELPER 0x2 FlowerShieldEnder
	callasm SetTargetPartner+1
	goto RaiseUserDef1_MagneticLoop




RaiseUserDef1_AromaticMist:
	jumpifbyte 0x1 0x2023BCC 0x4 FAILED
	callasm SetTargetPartner+1
	jumpifspecialstatusflag EQUALS STATUS3_INAIR | STATUS3_UNDERGROUND | STATUS3_UNDERWATER | STATUS3_DISAPPEARED 0x0 FAILED
	jumpiffainted BANK_TARGET FAILED
	callasm CheckCraftyShield+1
	jumpifbyte NOTEQUALS FORM_COUNTER FALSE FAILED
	setbyte FORM_COUNTER 0x0
	setbyte 0x2023FDF 0x0
	jumpifstat BANK_TARGET EQUALS STAT_SPDEF STAT_MAX FAILED
	attackanimation
	waitanimation
	playstatchangeanimation BANK_TARGET STAT_ANIM_SPDEF 0x0
	setbyte STAT_CHANGE_BYTE STAT_SPDEF | INCREASE_1
	statbuffchange STAT_TARGET | STAT_BS_PTR FAILED
	jumpifbyte 0x0 MULTISTRING_CHOOSER 0x2 FAILED
	printfromtable 0x83FE57C
	waitmessage DELAY_1SECOND
	goto 0x81D694E

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

RaiseUserSpAtk1BS:
	attackcanceler
	attackstring
	ppreduce
	jumpifhalfword EQUALS CURRENT_MOVE MOVE_GROWTH GrowthBS
	jumpifhalfword EQUALS CURRENT_MOVE MOVE_WORKUP WorkUpBS
	jumpifhalfword EQUALS CURRENT_MOVE MOVE_ROTOTILLER RototillerBS
	jumpifhalfword EQUALS CURRENT_MOVE MOVE_GEARUP GearUpBS
	goto 0x81D6B8D




GrowthBS:
Growth_CheckStats:
	jumpifstat BANK_ATTACKER LESSTHAN STAT_ATK STAT_MAX Growth_CheckSun
	jumpifstat BANK_ATTACKER EQUALS STAT_SPATK STAT_MAX 0x81D85E7

Growth_CheckSun:
	attackanimation
	waitanimation
	setbyte 0x2023FDF 0x0
	playstatchangeanimation BANK_ATTACKER STAT_ANIM_ATK | STAT_ANIM_SPATK 0x0
	jumpifabilitypresent ABILITY_CLOUDNINE Growth_NoSunBoostAtk
	jumpifabilitypresent ABILITY_AIRLOCK Growth_NoSunBoostAtk
	jumpifhalfword ANDS WEATHER_FLAGS weather_sun | weather_permament_sun | weather_harsh_sun Growth_SunBoostAtk

Growth_NoSunBoostAtk:
	setbyte STAT_CHANGE_BYTE STAT_ATK | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR Growth_NoSunBoostSpAtk
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 Growth_NoSunBoostSpAtk
	printfromtable 0x83FE57C
	waitmessage DELAY_1SECOND

Growth_NoSunBoostSpAtk:
	setbyte STAT_CHANGE_BYTE STAT_SPATK | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR 0x81D694E
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 0x81D694E
	printfromtable 0x83FE57C
	waitmessage DELAY_1SECOND
	goto 0x81D694E

Growth_SunBoostAtk:
	setbyte STAT_CHANGE_BYTE STAT_ATK | INCREASE_2
	statbuffchange STAT_ATTACKER | STAT_BS_PTR Growth_SunBoostSpAtk
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 Growth_SunBoostSpAtk
	printfromtable 0x83FE57C
	waitmessage DELAY_1SECOND

Growth_SunBoostSpAtk:
	setbyte STAT_CHANGE_BYTE STAT_SPATK | INCREASE_2
	statbuffchange STAT_ATTACKER | STAT_BS_PTR 0x81D694E
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 0x81D694E
	printfromtable 0x83FE57C
	waitmessage DELAY_1SECOND
	goto 0x81D694E




WorkUpBS:
	jumpifstat BANK_ATTACKER 0x3 STAT_ATK STAT_MAX WorkUp_RaiseAtk
	jumpifstat BANK_ATTACKER EQUALS STAT_SPATK STAT_MAX 0x81D85E7

WorkUp_RaiseAtk:
	attackanimation
	waitanimation
	setbyte 0x2023FDF 0x0
	playstatchangeanimation BANK_ATTACKER STAT_ANIM_ATK | STAT_ANIM_SPATK 0x0
	setbyte STAT_CHANGE_BYTE STAT_ATK | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR WorkUp_RaiseSpAtk
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 WorkUp_RaiseSpAtk
	printfromtable 0x83FE57C
	waitmessage DELAY_1SECOND

WorkUp_RaiseSpAtk:
	setbyte STAT_CHANGE_BYTE STAT_SPATK | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR 0x81D694E
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 0x81D694E
	printfromtable 0x83FE57C
	waitmessage DELAY_1SECOND
	goto 0x81D694E




RototillerBS:
	setbyte FORM_COUNTER TYPE_GRASS
	callasm CheckIfTypePresent+1
	jumpifbyte EQUALS FORM_COUNTER FALSE FAILED
	setword SEED_HELPER 0x0
	setbyte FLOWER_SHIELD_HELPER FALSE
	setbyte FORM_COUNTER 0x0

RototillerBSLoop:
	callasm FlowerShieldLooper+1
	jumpifbyte EQUALS FORM_COUNTER 0xFF FlowerShieldEnder
	jumpifgrounded BANK_TARGET RototillerStillGood
	goto RototillerBSLoop

RototillerStillGood:
	jumpifstat BANK_TARGET 0x3 STAT_ATK STAT_MAX Rototiller_BoostAtk 
	jumpifstat BANK_TARGET EQUALS STAT_SPATK STAT_MAX RototillerBSLoop

Rototiller_BoostAtk:
	attackanimation
	waitanimation
	setbyte 0x2023FDF 0x0
	setbyte FLOWER_SHIELD_HELPER TRUE
	playstatchangeanimation BANK_TARGET STAT_ANIM_ATK | STAT_ANIM_SPATK 0x0
	setbyte STAT_CHANGE_BYTE STAT_ATK | INCREASE_1
	statbuffchange STAT_TARGET | STAT_BS_PTR Rototiller_RaiseSpAtk
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 Rototiller_RaiseSpAtk
	printfromtable 0x83FE57C
	waitmessage DELAY_1SECOND

Rototiller_RaiseSpAtk:
	setbyte STAT_CHANGE_BYTE STAT_SPATK | INCREASE_1
	statbuffchange STAT_TARGET | STAT_BS_PTR RototillerBSLoop
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 RototillerBSLoop
	printfromtable 0x83FE57C
	waitmessage DELAY_1SECOND
	goto RototillerBSLoop




GearUpBS:
	copyarray TARGET_BANK USER_BANK 0x1
	jumpifabilitypresenttargetfield ABILITY_PLUS GearUp_MagneticBoost
	jumpifabilitypresenttargetfield ABILITY_MINUS GearUp_MagneticBoost
	goto FAILED

GearUp_MagneticBoost:
	setbyte FLOWER_SHIELD_HELPER 0x0
	setbyte SEED_HELPER 0x0

GearUp_MagneticLoop:
	jumpifability 0x0 ABILITY_PLUS GearUp_BoostAtkandSpAtk
	jumpifability 0x0 ABILITY_MINUS GearUp_BoostAtkandSpAtk
	goto GearUp_Skip2

GearUp_BoostAtkandSpAtk:
	jumpifstat BANK_TARGET NOTEQUALS STAT_ATK STAT_MAX GearUp_Atk
	jumpifstat BANK_TARGET EQUALS STAT_SPATK STAT_MAX GearUp_Skip2

GearUp_Atk:
	attackanimation
	waitanimation
	setbyte FLOWER_SHIELD_HELPER 0x1
	setbyte 0x2023FDF 0x0
	playstatchangeanimation BANK_TARGET STAT_ANIM_ATK | STAT_ANIM_SPATK 0x0
	jumpifstat BANK_TARGET EQUALS STAT_ATK STAT_MAX GearUp_SpAtk
	setbyte STAT_CHANGE_BYTE STAT_ATK | INCREASE_1
	statbuffchange STAT_TARGET | STAT_BS_PTR GearUp_SpAtk
	jumpifbyte 0x0 MULTISTRING_CHOOSER 0x2 GearUp_SpAtk
	printfromtable 0x83FE57C
	waitmessage DELAY_1SECOND

GearUp_SpAtk:
	setbyte STAT_CHANGE_BYTE STAT_SPATK | INCREASE_1
	jumpifstat BANK_TARGET EQUALS STAT_SPATK STAT_MAX GearUp_Skip2
	statbuffchange STAT_TARGET | STAT_BS_PTR GearUp_Skip2
	jumpifbyte 0x0 MULTISTRING_CHOOSER 0x2 GearUp_Skip2
	printfromtable 0x83FE57C
	waitmessage DELAY_1SECOND

GearUp_Skip2:
	addbyte 0x203D85C 0x1
	jumpifbyte NOTEQUALS NUM_POKEMON 0x4 GearUp_end
	jumpifbyte EQUALS SEED_HELPER 0x2 GearUp_end
	callasm SetTargetPartner+1
	goto GearUp_MagneticLoop

GearUp_end:
	setword SEED_HELPER 0x0
	setbyte FORM_COUNTER 0x0
	jumpifbyte EQUALS FLOWER_SHIELD_HELPER 0x0 FAILED
	setbyte FLOWER_SHIELD_HELPER 0x0
	goto 0x81D694E

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

RaiseUserEvsn1BS:
	jumpifhalfword NOTEQUALS CURRENT_MOVE MOVE_ACUPRESSURE 0x81D6B98

AcupressureBS:
	attackcanceler
	callasm AcupressureFunc+1
	attackstring
	ppreduce
	attackanimation
	waitanimation
	statbuffchange STAT_TARGET | STAT_BS_PTR | STAT_CERTAIN 0x81D694E
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 0x81D694E
	setgraphicalstatchangevalues
	playanimation BANK_TARGET 0x1 0x2023FD4
	printfromtable 0x83FE57C
	waitmessage DELAY_1SECOND
	goto 0x81D694E

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

LowerTargetAtk1BS:
	attackcanceler
	attackstring
	ppreduce
	jumpifsecondarystatus 0x0 0x1000000 FAILED
	accuracycheck FAILED 0x0
	jumpifhalfword EQUALS CURRENT_MOVE MOVE_PLAYNICE PlayNiceBS
	jumpifhalfword EQUALS CURRENT_MOVE MOVE_NOBLEROAR PlayNiceBS
	jumpifhalfword EQUALS CURRENT_MOVE MOVE_TEARFULLOOK PlayNiceBS
	jumpifhalfword EQUALS CURRENT_MOVE MOVE_VENOMDRENCH VenomDrenchBS
	setbyte STAT_CHANGE_BYTE STAT_ATK | DECREASE_1
	goto 0x81D6C27




PlayNiceBS:
	jumpifstat BANK_TARGET GREATERTHAN STAT_ATK STAT_MIN PlayNice_Atk
	jumpifstat BANK_TARGET EQUALS STAT_SPATK STAT_MIN 0x81D8499

PlayNice_Atk:
	attackanimation
	waitanimation
	setbyte 0x2023FDF 0x0
	playstatchangeanimation BANK_TARGET STAT_ANIM_ATK | STAT_ANIM_SPATK STAT_ANIM_DOWN | STAT_ANIM_CAN_FAIL
	setbyte STAT_CHANGE_BYTE STAT_ATK | DECREASE_1
	statbuffchange STAT_TARGET | STAT_BS_PTR PlayNice_SpAtk
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 PlayNice_SpAtk
	printfromtable 0x83FE588
	waitmessage DELAY_1SECOND

PlayNice_SpAtk:
	setbyte STAT_CHANGE_BYTE STAT_SPATK | DECREASE_1
	statbuffchange STAT_TARGET | STAT_BS_PTR 0x81D694E
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 0x81D694E
	printfromtable 0x83FE588
	waitmessage DELAY_1SECOND
	goto 0x81D694E




VenomDrenchBS:
	jumpifstatus BANK_TARGET STATUS_POISON | STATUS_BAD_POISON DoVenomDrench
	goto 0x81D7E04

DoVenomDrench:
	jumpifstat BANK_TARGET GREATERTHAN STAT_ATK STAT_MIN VenomDrench_Atk
	jumpifstat BANK_TARGET GREATERTHAN STAT_SPATK STAT_MIN VenomDrench_Atk
	jumpifstat BANK_TARGET EQUALS STAT_SPD STAT_MIN 0x81D8499

VenomDrench_Atk:
	attackanimation
	waitanimation
	setbyte 0x2023FDF 0x0
	playstatchangeanimation BANK_TARGET STAT_ANIM_ATK | STAT_ANIM_SPATK | STAT_ANIM_SPD STAT_ANIM_DOWN | STAT_ANIM_CAN_FAIL
	setbyte STAT_CHANGE_BYTE STAT_ATK | DECREASE_1
	statbuffchange STAT_TARGET | STAT_BS_PTR VenomDrench_SpAtk
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 VenomDrench_SpAtk
	printfromtable 0x83FE588
	waitmessage DELAY_1SECOND

VenomDrench_SpAtk:
	setbyte STAT_CHANGE_BYTE STAT_SPATK | DECREASE_1
	statbuffchange STAT_TARGET | STAT_BS_PTR VenomDrench_Spd
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 VenomDrench_Spd
	printfromtable 0x83FE588
	waitmessage DELAY_1SECOND

VenomDrench_Spd:
	setbyte STAT_CHANGE_BYTE STAT_SPD | DECREASE_1
	statbuffchange STAT_TARGET | STAT_BS_PTR 0x81D694E
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 0x81D694E
	printfromtable 0x83FE588
	waitmessage DELAY_1SECOND
	goto 0x81D694E

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

LowerTargetSpAtk1BS:
	setbyte STAT_CHANGE_BYTE STAT_SPATK | DECREASE_1
	goto 0x81D6C13

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

RaiseUserDef2BS:
	jumpifhalfword NOTEQUALS CURRENT_MOVE MOVE_COTTONGUARD 0x81D7103

RaiseUserDef3:
	setbyte STAT_CHANGE_BYTE STAT_DEF | INCREASE_3
	attackcanceler
	attackstring
	ppreduce
	statbuffchange STAT_ATTACKER | STAT_BS_PTR 0x81D6BCC
	jumpifbyte 0x1 0x2023E87 0x2 RaiseUserDef3_Anim
	pause 0x20
	goto 0x81D6BC4

RaiseUserDef3_Anim:
	attackanimation
	waitanimation
	setgraphicalstatchangevalues
	@sethalfword 0x2023FD4 0x28
	goto 0x81D6BBD


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

RaiseUserSpd2BS:
	setbyte STAT_CHANGE_BYTE STAT_SPD | INCREASE_2
	attackcanceler
	attackstring
	ppreduce
	jumpifhalfword NOTEQUALS CURRENT_MOVE MOVE_AUTOTOMIZE 0x81D6BA1

AutotomizeBS:
	statbuffchange STAT_ATTACKER | STAT_BS_PTR 0x81D6BCC
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 0x81D6BB2
	attackanimation
	waitanimation
	setgraphicalstatchangevalues
	playanimation BANK_ATTACKER 0x1 0x2023FD4
	printfromtable 0x83FE57C
	waitmessage DELAY_1SECOND
	callasm IncreaseNimbleCounter+1
	setword BATTLE_STRING_LOADER BecameNimbleString
	printstring 0x184
	waitmessage DELAY_1SECOND
	goto 0x81D694E

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

RaiseUserSpAtk2BS:
	jumpifhalfword NOTEQUALS CURRENT_MOVE MOVE_TAILGLOW 0x81D7119

RaiseUserSpAtk3:
	setbyte STAT_CHANGE_BYTE STAT_SPATK | INCREASE_3
	attackcanceler
	attackstring
	ppreduce
	statbuffchange STAT_ATTACKER | STAT_BS_PTR 0x81D6BCC
	jumpifbyte NOTEQUALS MULTISTRING_CHOOSER 0x2 RaiseUserSpAtk3_Anim
	pause DELAY_HALFSECOND
	goto 0x81D6BC4

RaiseUserSpAtk3_Anim:
	attackanimation
	waitanimation
	setgraphicalstatchangevalues
	@sethalfword 0x2023FD4 0x2A
	goto 0x81D6BBD

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

LowerTargetSpAtk2BS:
	setbyte STAT_CHANGE_BYTE STAT_SPATK | DECREASE_2
	jumpifhalfword NOTEQUALS CURRENT_MOVE MOVE_CAPTIVATE 0x81D6C13

CaptivateBS:
	attackcanceler
	attackstring
	ppreduce
	jumpifsecondarystatus BANK_TARGET STATUS2_SUBSTITUTE FAILED
	accuracycheck FAILED 0x0
	jumpifability BANK_TARGET ABILITY_OBLIVIOUS 0x81D7E04
	tryinfatuatetarget 0x81D7E04
	goto 0x81D6C27

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

LowerTargetSpd1ChanceBS:
	jumpifhalfword NOTEQUALS CURRENT_MOVE MOVE_FLAMECHARGE 0x81D7275
	setbyte EFFECT_BYTE 0xD1
	goto 0x81D6926

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

LowerTargetSpDef1ChanceBS:
	jumpifhalfword EQUALS CURRENT_MOVE MOVE_CHARGEBEAM ChargeBeamBS
	jumpifhalfword EQUALS CURRENT_MOVE MOVE_ACIDSPRAY AcidSprayBS
	jumpifhalfword EQUALS CURRENT_MOVE MOVE_FIERYDANCE ChargeBeamBS
	jumpifhalfword EQUALS CURRENT_MOVE MOVE_SEEDFLARE AcidSprayBS
	goto 0x81D728B

ChargeBeamBS:
	setbyte EFFECT_BYTE 0x52
	goto 0x81D6926

AcidSprayBS:
	setbyte EFFECT_BYTE 0x32
	goto 0x81D6926

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SteelWingBS:
	jumpifhalfword NOTEQUALS CURRENT_MOVE MOVE_DIAMONDSTORM 0x81D7A31

DiamondStormBS:
	setbyte EFFECT_BYTE 0x28 | MOVE_EFFECT_AFFECTS_USER
	goto 0x81D6926

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

MetalClawBS:
	jumpifhalfword NOTEQUALS CURRENT_MOVE MOVE_FELLSTINGER 0x81D7A3C

FellStingerBS:
	attackcanceler
	accuracycheck 0x81D695E 0x0
	call STANDARD_DAMAGE
	faintpokemon BANK_TARGET 0x0 0x8000000
	jumpiffainted BANK_TARGET FellStingerKill
	goto 0x81D6947

FellStingerKill:
	setbyte 0x2023FDF 0x0
	jumpifstat BANK_ATTACKER EQUALS STAT_ATK STAT_MAX 0x81D694E
	playstatchangeanimation BANK_ATTACKER STAT_ANIM_ATK STAT_ANIM_UP
	setbyte STAT_CHANGE_BYTE STAT_ATK | INCREASE_3
	statbuffchange STAT_ATTACKER | STAT_BS_PTR 0x81D694E
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 0x81D694E
	printfromtable 0x83FE57C
	waitmessage DELAY_1SECOND
	goto 0x81D694E

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

PsychUpBS:
	jumpifhalfword NOTEQUALS CURRENT_MOVE MOVE_SPECTRALTHIEF 0x1D7A74

SpectralThiefBS:
	attackcanceler
	accuracycheck 0x81D695E 0x0
	attackstring
	ppreduce
	setbyte FORM_COUNTER 0x0
	callasm SpectralThiefFunc+1
	jumpifbyte NOTEQUALS FORM_COUNTER 0x0 PlaySpectBoost
	goto 0x81D6930

PlaySpectBoost:
	attackanimation
	waitanimation
	setbyte FORM_COUNTER 0x0
	setbyte SPECTRAL_THIEF_BYTE 0x1
	playstatchangeanimation BANK_ATTACKER 0xFF STAT_ANIM_UP
	setbyte SPECTRAL_THIEF_BYTE 0x0
	setword BATTLE_STRING_LOADER SpectralThiefString
	printstring 0x184
	waitmessage DELAY_1SECOND
	goto 0x81D6930

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

ChargeBS:
	attackcanceler
	attackstring
	ppreduce
	setcharge
	attackanimation
	waitanimation
	printstring 0xA5
	waitmessage DELAY_1SECOND

ChargeSpDefBoost:
	setbyte EFFECT_BYTE 0xE + STAT_SPDEF
	seteffectuser
	goto 0x81D694E

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SuperpowerBS:
	jumpifhalfword EQUALS CURRENT_MOVE MOVE_CLOSECOMBAT CloseCombatBS
	jumpifhalfword EQUALS CURRENT_MOVE MOVE_DRAGONASCENT CloseCombatBS
	jumpifhalfword EQUALS CURRENT_MOVE MOVE_HAMMERARM HammerArmBS
	jumpifhalfword EQUALS CURRENT_MOVE MOVE_ICEHAMMER HammerArmBS
	jumpifhalfword EQUALS CURRENT_MOVE MOVE_CLANGINGSCALES ClangingScalesBS
	jumpifhalfword EQUALS CURRENT_MOVE MOVE_VCREATE VCreateBS
	jumpifhalfword EQUALS CURRENT_MOVE MOVE_HYPERSPACEHOLE HyperspaceHoleBS
	jumpifhalfword EQUALS CURRENT_MOVE MOVE_HYPERSPACEFURY HyperspaceFuryBS
	goto 0x81D817E




CloseCombatBS:
	attackcanceler

	accuracycheck 0x81D695E 0x0
	call STANDARD_DAMAGE
	jumpifbyte ANDS OUTCOME 0x29 0x81D6947
	jumpifstat BANK_ATTACKER GREATERTHAN STAT_DEF STAT_MIN CC_LowerDef
	jumpifstat BANK_ATTACKER EQUALS STAT_SPDEF STAT_MIN 0x81D6947

CC_LowerDef:
	setbyte 0x2023FDF 0x0
	playstatchangeanimation BANK_ATTACKER STAT_ANIM_DEF | STAT_ANIM_SPDEF STAT_ANIM_DOWN | STAT_ANIM_CAN_FAIL
CC_SkipTo:
	setbyte STAT_CHANGE_BYTE STAT_DEF | DECREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR CC_LowerSpDef
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 CC_LowerSpDef
	printfromtable 0x83FE57C
	waitmessage DELAY_1SECOND

CC_LowerSpDef:
	setbyte STAT_CHANGE_BYTE STAT_SPDEF | DECREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR 0x81D6947
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 0x81D6947
	printfromtable 0x83FE57C
	waitmessage DELAY_1SECOND
	goto 0x81D6947




HammerArmBS:
	setbyte 0x2023E85 0x18 | 0x40
	goto 0x81D6926


ClangingScalesBS:
	setbyte EFFECT_BYTE 0x57
	goto 0x81D6926




VCreateBS:
	attackcanceler
	accuracycheck 0x81D695E 0x0
	call STANDARD_DAMAGE
	jumpifbyte ANDS OUTCOME 0x29 0x81D6947
	jumpifstat BANK_ATTACKER GREATERTHAN STAT_SPD STAT_MIN VC_LowerSpd
	jumpifstat BANK_ATTACKER GREATERTHAN STAT_DEF STAT_MIN VC_LowerSpd
	jumpifstat BANK_ATTACKER EQUALS STAT_SPDEF STAT_MIN 0x81D6947

VC_LowerSpd:
	setbyte 0x2023FDF 0x0
	playstatchangeanimation BANK_ATTACKER STAT_ANIM_DEF | STAT_ANIM_SPDEF | STAT_ANIM_SPD STAT_ANIM_DOWN | STAT_ANIM_CAN_FAIL
	setbyte STAT_CHANGE_BYTE STAT_SPD | DECREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR VC_LowerDef
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 VC_LowerDef
	printfromtable 0x83FE57C
	waitmessage DELAY_1SECOND

VC_LowerDef:
	setbyte 0x2023FDF 0x0
	goto CC_SkipTo



HyperspaceHoleBS:
	attackcanceler
	goto FeintSkip

HyperspaceFuryBS:
	attackcanceler
	jumpifspecies BANK_ATTACKER PKMN_HOOPA_UNBOUND HyperspaceFurySuccess
	orbyte OUTCOME 0x8
	attackstring
	ppreduce
	pause DELAY_HALFSECOND
	jumpifspecies BANK_ATTACKER PKMN_HOOPA HoopaCant
	setword BATTLE_STRING_LOADER CantUseHyperspaceFuryString
	printstring 0x184
	waitmessage DELAY_1SECOND
	goto 0x81D694E

HoopaCant:
	setword BATTLE_STRING_LOADER WrongHoopaFormString
	printstring 0x184
	waitmessage DELAY_1SECOND
	goto 0x81D694E

HyperspaceFurySuccess:
	setbyte EFFECT_BYTE 0x17 | 0x40
	goto FeintSkip

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BulkUpBS:
	jumpifhalfword NOTEQUALS CURRENT_MOVE MOVE_COIL 0x81D851F

CoilBS:
	attackcanceler
	attackstring
	ppreduce
	jumpifstat BANK_ATTACKER LESSTHAN STAT_ATK STAT_MAX Coil_Atk
	jumpifstat BANK_ATTACKER LESSTHAN STAT_DEF STAT_MAX Coil_Atk
	jumpifstat BANK_ATTACKER EQUALS STAT_ACC STAT_MAX 0x81D85E7

Coil_Atk:
	attackanimation
	waitanimation
	setbyte 0x2023FDF 0x0
	playstatchangeanimation BANK_ATTACKER STAT_ANIM_ATK | STAT_ANIM_DEF | STAT_ANIM_ACC STAT_ANIM_UP | STAT_ANIM_CAN_FAIL
	setbyte STAT_CHANGE_BYTE STAT_ATK | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR Coil_Def
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 Coil_Def
	printfromtable 0x83FE57C
	waitmessage DELAY_1SECOND

Coil_Def:
	setbyte STAT_CHANGE_BYTE STAT_DEF | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR Coil_Acc
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 Coil_Acc
	printfromtable 0x83FE57C
	waitmessage DELAY_1SECOND

Coil_Acc:
	setbyte STAT_CHANGE_BYTE STAT_ACC | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR 0x81D694E
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 0x81D694E
	printfromtable 0x83FE57C
	waitmessage DELAY_1SECOND
	goto 0x81D694E

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

CalmMindBS:
	attackcanceler
	jumpifhalfword EQUALS CURRENT_MOVE MOVE_QUIVERDANCE QuiverDanceBS
	jumpifhalfword EQUALS CURRENT_MOVE MOVE_GEOMANCY GeomancyBS
	goto 0x81D8584




QuiverDanceBS:
	attackstring
	ppreduce
	jumpifstat BANK_ATTACKER LESSTHAN STAT_SPATK STAT_MAX QuiverDance_RaiseSpAtk1
	jumpifstat BANK_ATTACKER LESSTHAN STAT_SPDEF STAT_MAX QuiverDance_RaiseSpAtk1
	jumpifstat BANK_ATTACKER EQUALS STAT_SPD STAT_MAX 0x81D85E7

QuiverDance_RaiseSpAtk1:
	attackanimation
	waitanimation
	setbyte 0x2023FDF 0x0
	playstatchangeanimation BANK_ATTACKER STAT_ANIM_SPATK | STAT_ANIM_SPDEF | STAT_ANIM_SPD 0x0
	setbyte STAT_CHANGE_BYTE STAT_SPATK | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR QuiverDance_RaiseSpDef1
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 QuiverDance_RaiseSpDef1
	printfromtable 0x83FE57C
	waitmessage DELAY_1SECOND

QuiverDance_RaiseSpDef1:
	setbyte STAT_CHANGE_BYTE STAT_SPDEF | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR QuiverDance_RaiseSpd1
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 QuiverDance_RaiseSpd1
	printfromtable 0x83FE57C
	waitmessage DELAY_1SECOND

QuiverDance_RaiseSpd1:
	setbyte STAT_CHANGE_BYTE STAT_SPD | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR 0x81D85E2
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 0x81D85E2
	printfromtable 0x83FE57C
	waitmessage DELAY_1SECOND
	goto 0x81D694E




GeomancyBS:
	jumpifsecondarystatus BANK_ATTACKER STATUS2_MULTIPLETURNS Geomancy_PowerUp
	jumpifword 0x0 HIT_MARKER 0x200 Geomancy_PowerUp

Geomancy_Charge:
	printstring 0x130
	ppreduce
	attackanimation
	waitanimation
	orword HIT_MARKER 0x8000000
	setbyte EFFECT_BYTE 0x4C
	seteffecttarget
	copyarray MULTISTRING_CHOOSER 0x2023FD3 0x1
	setword BATTLE_STRING_LOADER GeomancyChargeString
	printstring 0x184
	waitmessage DELAY_1SECOND
	jumpifhelditemeffect BANK_ATTACKER ITEM_EFFECT_POWER_HERB Geomancy_PowerHerb
	goto 0x81D694E

Geomancy_PowerUp:
	setbyte EFFECT_BYTE 0xC
	setbyte 0x2023FDC 0x1
	clearstatus BANK_ATTACKER
	orword HIT_MARKER 0x800
	setbyte EFFECT_BYTE 0x0
	attackstring
	ppreduce
	jumpifstat BANK_ATTACKER LESSTHAN STAT_SPATK STAT_MAX Geomancy_RaiseSpAtk2
	jumpifstat BANK_ATTACKER LESSTHAN STAT_SPDEF STAT_MAX Geomancy_RaiseSpAtk2
	jumpifstat BANK_ATTACKER EQUALS STAT_SPD STAT_MAX 0x81D85E7

Geomancy_RaiseSpAtk2:
	attackanimation
	waitanimation
	setbyte 0x2023FDF 0x0
	playstatchangeanimation BANK_ATTACKER STAT_ANIM_SPATK | STAT_ANIM_SPDEF | STAT_ANIM_SPD 0x0
	setbyte STAT_CHANGE_BYTE STAT_SPATK | INCREASE_2
	statbuffchange STAT_ATTACKER | STAT_BS_PTR Geomancy_RaiseSpDef2
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 Geomancy_RaiseSpDef2
	printfromtable 0x83FE57C
	waitmessage DELAY_1SECOND

Geomancy_RaiseSpDef2:
	setbyte STAT_CHANGE_BYTE STAT_SPDEF | INCREASE_2
	statbuffchange STAT_ATTACKER | STAT_BS_PTR Geomancy_RaiseSpd2
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 Geomancy_RaiseSpd2
	printfromtable 0x83FE57C
	waitmessage DELAY_1SECOND

Geomancy_RaiseSpd2:
	setbyte STAT_CHANGE_BYTE STAT_SPD | INCREASE_2
	statbuffchange STAT_ATTACKER | STAT_BS_PTR 0x81D694E
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 0x81D694E
	printfromtable 0x83FE57C
	waitmessage DELAY_1SECOND
	goto 0x81D694E

Geomancy_PowerHerb:
	playanimation BANK_ATTACKER 0x7 0x0
	waitanimation
	setword BATTLE_STRING_LOADER PowerHerbString
	printstring 0x184
	waitmessage DELAY_1SECOND
	setbyte 0x2023FDD 0x0
	removeitem BANK_ATTACKER
	goto Geomancy_PowerUp

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

DragonDanceBS:
	attackcanceler
	attackstring
	ppreduce
	jumpifhalfword EQUALS CURRENT_MOVE MOVE_SHIFTGEAR ShiftGearBS
	jumpifhalfword EQUALS CURRENT_MOVE MOVE_SHELLSMASH ShellSmashBS
	goto 0x81D85FE




ShiftGearBS:
	jumpifstat BANK_ATTACKER LESSTHAN STAT_ATK STAT_MAX ShiftGear_Atk
	jumpifstat BANK_ATTACKER EQUALS STAT_SPD STAT_MAX 0x81D85E7

ShiftGear_Atk:
	attackanimation
	waitanimation
	setbyte 0x2023FDF 0x0
	playstatchangeanimation BANK_ATTACKER STAT_ANIM_ATK | STAT_ANIM_SPD STAT_ANIM_UP
	setbyte STAT_CHANGE_BYTE STAT_ATK | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR ShiftGear_Spd
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 ShiftGear_Spd
	printfromtable 0x83FE57C
	waitmessage DELAY_1SECOND

ShiftGear_Spd:
	setbyte STAT_CHANGE_BYTE STAT_SPD | INCREASE_2
	statbuffchange STAT_ATTACKER | STAT_BS_PTR 0x81D85E2
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 0x81D85E2
	printfromtable 0x83FE57C
	waitmessage DELAY_1SECOND
	goto 0x81D694E




ShellSmashBS:
	jumpifstat BANK_ATTACKER GREATERTHAN STAT_DEF STAT_MIN ShellSmash_DropDef
	jumpifstat BANK_ATTACKER EQUALS STAT_SPDEF STAT_MIN ShellSmash_BoostStats

ShellSmash_DropDef:
	attackanimation
	waitanimation
	orword HIT_MARKER 0x80 @Inhibit Animations
	setbyte 0x2023FDF 0x0
	playstatchangeanimation BANK_ATTACKER STAT_ANIM_DEF | STAT_ANIM_SPDEF STAT_ANIM_DOWN
	setbyte STAT_CHANGE_BYTE STAT_DEF | DECREASE_1
	statbuffchange 0x81 ShellSmash_DropSpDef
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 ShellSmash_DropSpDef
	printfromtable 0x83FE57C
	waitmessage DELAY_1SECOND

ShellSmash_DropSpDef:
	setbyte STAT_CHANGE_BYTE STAT_SPDEF | DECREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR ShellSmash_BoostStats
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 ShellSmash_BoostStats
	printfromtable 0x83FE57C
	waitmessage DELAY_1SECOND

ShellSmash_BoostStats:
	jumpifstat BANK_ATTACKER LESSTHAN STAT_ATK STAT_MAX ShellSmash_SharpAtk
	jumpifstat BANK_ATTACKER LESSTHAN STAT_SPATK STAT_MAX ShellSmash_SharpAtk
	jumpifstat BANK_ATTACKER EQUALS STAT_SPD STAT_MAX 0x81D85E7

ShellSmash_SharpAtk:
	attackanimation
	waitanimation
	bicword HIT_MARKER 0x80 @Uninhibit Animations
	setbyte 0x2023FDF 0x0
	playstatchangeanimation BANK_ATTACKER STAT_ANIM_ATK | STAT_ANIM_SPATK | STAT_ANIM_SPD STAT_ANIM_UP
	setbyte STAT_CHANGE_BYTE STAT_ATK | INCREASE_2
	statbuffchange STAT_ATTACKER | STAT_BS_PTR ShellSmash_SharpSpAtk
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 ShellSmash_SharpSpAtk
	printfromtable 0x83FE57C
	waitmessage DELAY_1SECOND

ShellSmash_SharpSpAtk:
	setbyte STAT_CHANGE_BYTE STAT_SPATK | INCREASE_2
	statbuffchange STAT_ATTACKER | STAT_BS_PTR ShiftGear_Spd
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 ShiftGear_Spd
	printfromtable 0x83FE57C
	waitmessage DELAY_1SECOND
	goto ShiftGear_Spd

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

StatSwapSplittersBS:
	attackcanceler
	attackstring
	ppreduce
	callasm SetStatSwapSplit+1
	attackanimation
	waitanimation
	printstring 0x184
	waitmessage DELAY_1SECOND
	goto 0x81D694E

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

ClearSmogBS:
	attackcanceler
	accuracycheck 0x81D695E 0x0
	call STANDARD_DAMAGE
	jumpifability BANK_ATTACKER ABILITY_SHEERFORCE 0x81D6947
	jumpifability BANK_TARGET ABILITY_SHIELDDUST 0x81D6947
	jumpifbyte ANDS OUTCOME 0x29 0x81D6947
	jumpifsecondarystatus BANK_TARGET STATUS2_SUBSTITUTE 0x81D6947
	callasm ResetTargetStats+1
	setword BATTLE_STRING_LOADER ClearSmogString
	printstring 0x184
	waitmessage DELAY_1SECOND
	goto 0x81D6947

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

FeintBS:
	attackcanceler
	jumpifsecondarystatus 0x0 STATUS2_SUBSTITUTE 0x81D6927
FeintSkip:
	accuracycheck 0x01D695E 0x0
	call STANDARD_DAMAGE
	seteffectwithchancetarget
	jumpifbyte ANDS OUTCOME 0x29 0x81D6947
	callasm LiftProtectTarget+1
	setword BATTLE_STRING_LOADER ProtectLiftedString
	printstring 0x184
	waitmessage DELAY_1SECOND
	goto 0x81D6947	

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BecameNimbleString: .byte 0xFD, 0x0F, 0x00, 0xD6, 0xD9, 0xD7, 0xD5, 0xE1, 0xD9, 0x00, 0xE2, 0xDD, 0xE1, 0xD6, 0xE0, 0xD9, 0xAB, 0xFF
CantUseHyperspaceFuryString: .byte 0xBC, 0xE9, 0xE8, 0x00, 0xFD, 0x0F, 0x00, 0xD7, 0xD5, 0xE2, 0xB4, 0xE8, 0x00, 0xE9, 0xE7, 0xD9, 0xFE, 0xE8, 0xDC, 0xD9, 0x00, 0xE1, 0xE3, 0xEA, 0xD9, 0xAB, 0xFF
WrongHoopaFormString: .byte 0xBC, 0xE9, 0xE8, 0x00, 0xFD, 0x0F, 0x00, 0xD7, 0xD5, 0xE2, 0xB4, 0xE8, 0x00, 0xE9, 0xE7, 0xD9, 0x00, 0xDD, 0xE8, 0x00, 0xE8, 0xDC, 0xD9, 0xFE, 0xEB, 0xD5, 0xED, 0x00, 0xDD, 0xE8, 0x00, 0xDD, 0xE7, 0x00, 0xE2, 0xE3, 0xEB, 0xAB, 0xFF
GeomancyChargeString: .byte 0xFD, 0x0F, 0x00, 0xDD, 0xE7, 0x00, 0xD5, 0xD6, 0xE7, 0xE3, 0xE6, 0xD6, 0xDD, 0xE2, 0xDB, 0x00, 0xE4, 0xE3, 0xEB, 0xD9, 0xE6, 0xAB, 0xFF
PowerTrickString: .byte 0xFD, 0x0F, 0x00, 0xE7, 0xEB, 0xDD, 0xE8, 0xD7, 0xDC, 0xD9, 0xD8, 0xFE, 0xDD, 0xE8, 0xE7, 0x00, 0xBB, 0xE8, 0xE8, 0xD5, 0xD7, 0xDF, 0x00, 0xD5, 0xE2, 0xD8, 0x00, 0xBE, 0xD9, 0xDA, 0xD9, 0xE2, 0xE7, 0xD9, 0xAB, 0xFF
PowerSwapString: .byte 0xFD, 0x0F, 0x00, 0xE7, 0xEB, 0xDD, 0xE8, 0xD7, 0xDC, 0xD9, 0xD8, 0x00, 0xD5, 0xE0, 0xE0, 0xFE, 0xD7, 0xDC, 0xD5, 0xE2, 0xDB, 0xD9, 0xE7, 0x00, 0xE8, 0xE3, 0x00, 0xDD, 0xE8, 0xE7, 0x00, 0xBB, 0xE8, 0xE8, 0xD5, 0xD7, 0xDF, 0x00, 0xD5, 0xE2, 0xD8, 0x00, 0xCD, 0xE4, 0xAD, 0x00, 0xBB, 0xE8, 0xDF, 0xFA, 0xEB, 0xDD, 0xE8, 0xDC, 0x00, 0xFD, 0x10, 0xAB, 0xFF
GuardSwapString: .byte 0xFD, 0x0F, 0x00, 0xE7, 0xEB, 0xDD, 0xE8, 0xD7, 0xDC, 0xD9, 0xD8, 0x00, 0xD5, 0xE0, 0xE0, 0xFE, 0xD7, 0xDC, 0xD5, 0xE2, 0xDB, 0xD9, 0xE7, 0x00, 0xE8, 0xE3, 0x00, 0xDD, 0xE8, 0xE7, 0x00, 0xBE, 0xD9, 0xDA, 0xD9, 0xE2, 0xE7, 0xD9, 0x00, 0xD5, 0xE2, 0xD8, 0x00, 0xCD, 0xE4, 0xAD, 0x00, 0xBE, 0xD9, 0xDA, 0xFA, 0xEB, 0xDD, 0xE8, 0xDC, 0x00, 0xFD, 0x10, 0xAB, 0xFF
SpeedSwapString: .byte 0xFD, 0x0F, 0x00, 0xE7, 0xEB, 0xDD, 0xE8, 0xD7, 0xDC, 0xD9, 0xD8, 0xFE, 0xE7, 0xE4, 0xD9, 0xD9, 0xD8, 0x00, 0xEB, 0xDD, 0xE8, 0xDC, 0x00, 0xFD, 0x10, 0xAB, 0xFF
HeartSwapString: .byte 0xFD, 0x0F, 0x00, 0xE7, 0xEB, 0xDD, 0xE8, 0xD7, 0xDC, 0xD9, 0xD8, 0xFE, 0xE7, 0xE8, 0xD5, 0xE8, 0x00, 0xD7, 0xDC, 0xD5, 0xE2, 0xDB, 0xD9, 0xE7, 0x00, 0xEB, 0xDD, 0xE8, 0xDC, 0xFA, 0xFD, 0x10, 0xAB, 0xFF
PowerSplitString: .byte 0xFD, 0x0F, 0x00, 0xE7, 0xDC, 0xD5, 0xE6, 0xD9, 0xD8, 0x00, 0xDD, 0xE8, 0xE7, 0xFE, 0xE4, 0xE3, 0xEB, 0xD9, 0xE6, 0x00, 0xEB, 0xDD, 0xE8, 0xDC, 0x00, 0xE8, 0xDC, 0xD9, 0x00, 0xE8, 0xD5, 0xE6, 0xDB, 0xD9, 0xE8, 0xAB, 0xFF
GuardSplitString: .byte 0xFD, 0x0F, 0x00, 0xE7, 0xDC, 0xD5, 0xE6, 0xD9, 0xD8, 0x00, 0xDD, 0xE8, 0xE7, 0xFE, 0xDB, 0xE9, 0xD5, 0xE6, 0xD8, 0x00, 0xEB, 0xDD, 0xE8, 0xDC, 0x00, 0xE8, 0xDC, 0xD9, 0x00, 0xE8, 0xD5, 0xE6, 0xDB, 0xD9, 0xE8, 0xAB, 0xFF
ProtectLiftedString: .byte 0xCE, 0xDC, 0xD9, 0x00, 0xE4, 0xE6, 0xE3, 0xE8, 0xD9, 0xD7, 0xE8, 0xDD, 0xE3, 0xE2, 0x00, 0xD9, 0xDA, 0xDA, 0xD9, 0xD7, 0xE8, 0xE7, 0x00, 0xEB, 0xD9, 0xE6, 0xD9, 0x00, 0xE0, 0xDD, 0xDA, 0xE8, 0xD9, 0xD8, 0xFE, 0xDA, 0xE6, 0xE3, 0xE1, 0x00, 0xE8, 0xDC, 0xD9, 0x00, 0xE8, 0xD5, 0xE6, 0xDB, 0xD9, 0xE8, 0xB4, 0xE7, 0x00, 0xE7, 0xDD, 0xD8, 0xD9, 0xAB, 0xFF
SpectralThiefString: .byte 0xFD, 0x0F, 0x00, 0xE7, 0xE8, 0xE3, 0xE0, 0xD9, 0xFE, 0xFD, 0x10, 0xB4, 0xE7, 0x00, 0xE7, 0xE8, 0xD5, 0xE8, 0x00, 0xD6, 0xE3, 0xE3, 0xE7, 0xE8, 0xE7, 0xAB, 0xFF
ClearSmogString: .byte 0xFD, 0x10, 0xB4, 0xE7, 0x00, 0xE7, 0xE8, 0xD5, 0xE8, 0x00, 0xD7, 0xDC, 0xD5, 0xE2, 0xDB, 0xD9, 0xE7, 0xFE, 0xEB, 0xD9, 0xE6, 0xD9, 0x00, 0xE6, 0xD9, 0xE1, 0xE3, 0xEA, 0xD9, 0xD8, 0xAB
PowerHerbString: .byte 0xFD, 0x0F, 0x00, 0xD6, 0xD9, 0xD7, 0xD5, 0xE1, 0xD9, 0x00, 0xDA, 0xE9, 0xE0, 0xE0, 0xED, 0xFE, 0xD7, 0xDC, 0xD5, 0xE6, 0xDB, 0xD9, 0xD8, 0x00, 0xD8, 0xE9, 0xD9, 0x00, 0xE8, 0xE3, 0x00, 0xDD, 0xE8, 0xE7, 0x00, 0xCA, 0xE3, 0xEB, 0xD9, 0xE6, 0x00, 0xC2, 0xD9, 0xE6, 0xD6, 0xAB, 0xFF
