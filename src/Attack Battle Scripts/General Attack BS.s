@;Add in Destiny Knot here?
@;BattleScript_NoHealTargetAfterHealBlock
@;Do BS_126_Magnitude
@;On BS_160_Stockpile

.text
.thumb
.align 2

.include "..\\defines"
/*

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_001_SetSleep
BS_001_SetSleep:
	attackcanceler
	attackstring
	ppreduce
	jumpifbehindsubstitute BANK_TARGET FAILED
	jumpifability BANK_TARGET ABILITY_COMATOSE 0x81D69B0 @;Already Asleep
	jumpifability BANK_TARGET ABILITY_INSOMNIA 0x81D824B
	jumpifability BANK_TARGET ABILITY_VITALSPIRIT 0x81D824B
	jumpifspecies BANK_TARGET PKMN_MINIORSHIELD FAILED
	jumpifabilitypresent ABILITY_CLOUDNINE SLP_CheckVeils
	jumpifabilitypresent ABILITY_AIRLOCK SLP_CheckVeils
	jumpifability BANK_TARGET ABILITY_LEAFGUARD SLP_SunnyCheck

SLP_CheckVeils:
	jumpifabilitypresenttargetfield ABILITY_SWEETVEIL TeamProtectedBySVBS
	jumpifabilitypresenttargetfield ABILITY_FLOWERVEIL SLP_GrassTypeCheck

SLP_CheckTerrain:
	jumpifbyte EQUALS TERRAIN_BYTE ELECTRIC_TERRAIN SLP_CheckGrounding
	jumpifbyte EQUALS TERRAIN_BYTE MISTY_TERRAIN SLP_CheckGrounding
	goto 0x81D697A

SLP_SunnyCheck:
	jumpifhalfword ANDS WEATHER_FLAGS WEATHER_SUN_ANY ProtectedByAbilityBS
	goto SLP_CheckVeils

SLP_GrassTypeCheck:
	jumpiftype BANK_TARGET TYPE_GRASS TeamProtectedByFVBS
	goto SLP_CheckTerrain

SLP_CheckGrounding:
	jumpifgrounded BANK_TARGET ProtectedByTerrainBS
	goto 0x81D697A
	
ProtectedByTerrainBS:
	pause DELAY_HALFSECOND
	setword BATTLE_STRING_LOADER ProtectedByTerrainString
	printstring 0x184
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

TeamProtectedBySVBS:
	pause DELAY_HALFSECOND
	setword BATTLE_STRING_LOADER ProtectedBySweetVeilString
	printstring 0x184
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

TeamProtectedByFVBS:
	pause DELAY_HALFSECOND
	setword BATTLE_STRING_LOADER ProtectedByFlowerVeilString
	printstring 0x184
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

ProtectedByAbilityBS:
	pause DELAY_HALFSECOND
	setword BATTLE_STRING_LOADER ProtectedByAbilityString
	printstring 0x184
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_002_SetPoisonChance
BS_002_SetPoisonChance:
	setmoveeffect MOVE_EFFECT_POISON
	goto 0x81D6926

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_003_DrainHP
BS_003_DrainHP:
	attackcanceler
	accuracycheck BS_MOVE_MISSED 0x0
	jumpifmove MOVE_STRENGTHSAP StrengthSapBS
	call STANDARD_DAMAGE
	negativedamage

DrainHPBSP2:
	orword HIT_MARKER HITMARKER_IGNORE_SUBSTITUTE
	jumpifability BANK_TARGET ABILITY_LIQUIDOOZE 0x81D6A23
	setbyte MULTISTRING_CHOOSER 0x0
	goto 0x81D6A2B
	
StrengthSapBS:
	attackstring
	ppreduce
	jumpifbehindsubstitute BANK_TARGET FAILED
	jumpifstat BANK_TARGET EQUALS STAT_ATK STAT_MIN FAILED
	attackanimation
	waitanimation
	setgraphicalstatchangevalues
	callasm StrengthSapFunc + 1
	setstatchanger STAT_ATK | DECREASE_1
	statbuffchange STAT_TARGET | STAT_BS_PTR DrainHPBSP2
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 DrainHPBSP2
	playstatchangeanimation BANK_TARGET STAT_ANIM_ATK STAT_ANIM_DOWN
	printfromtable 0x83FE588
	waitmessage DELAY_1SECOND
	goto DrainHPBSP2

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_004_SetBurnChance
BS_004_SetBurnChance:
	setmoveeffect MOVE_EFFECT_BURN
	goto 0x81D6926
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_005_SetFreezeChance
BS_005_SetFreezeChance:
	setmoveeffect MOVE_EFFECT_FREEZE
	goto 0x81D6926
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_006_SetParalyzeChance
BS_006_SetParalyzeChance:
	setmoveeffect MOVE_EFFECT_PARALYSIS
	goto 0x81D6926

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_007_Explode
BS_007_Explode:
	attackcanceler
	attackstring
	ppreduce
	faintifabilitynotdamp
	setuserhptozero
	waitstate
	accuracycheck BS_MOVE_MISSED 0x0
	goto 0x81D6930
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_008_DreamEater
BS_008_DreamEater:
	attackcanceler
	jumpifstatus BANK_TARGET STATUS_SLEEP EatTheDreams
	jumpifability BANK_TARGET ABILITY_COMATOSE EatTheDreams
	attackstring
	ppreduce
	waitmessage DELAY_1SECOND
	goto 0x81D69BE @;Target wasn't affected

EatTheDreams:
	accuracycheck BS_MOVE_MISSED 0x0
	call STANDARD_DAMAGE
	jumpifbyte ANDS OUTCOME OUTCOME_NO_EFFECT 0x81D6B53
	negativedamage
	orword HIT_MARKER 0x100
	jumpifability BANK_TARGET ABILITY_LIQUIDOOZE 0x81D6A23
	graphicalhpupdate BANK_ATTACKER
	datahpupdate BANK_ATTACKER
	printstring 0x3C
	waitmessage DELAY_1SECOND
	goto 0x1D6B53

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_009_MirrorMove
BS_009_MirrorMove:
	jumpifnotmove MOVE_COPYCAT MOVE_COPYCAT 0x81D6B5F @;Mirror Move
	attackcanceler
	attackstring
	callasm CopycatFunc + 1
	attackanimation
	waitanimation
	setbyte ANIM_TURN 0x0
	setbyte ANIM_TARGETS_HIT 0x0
	jumptoattack 0x1

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_010_RaiseUserAtk1
BS_010_RaiseUserAtk1:
	setstatchanger STAT_ATK | INCREASE_1
	jumpifnotmove MOVE_HONECLAWS BS_BUFF_ATK_STATS

HoneClawsBS:
	attackcanceler
	attackstring
	ppreduce
	jumpifstat BANK_TARGET LESSTHAN STAT_ATK STAT_MAX HoneClaws_Atk
	jumpifstat BANK_TARGET EQUALS STAT_ACC STAT_MAX 0x81D85E7

HoneClaws_Atk:
	attackanimation
	waitanimation
	setbyte STAT_ANIM_PLAYED 0x0
	playstatchangeanimation BANK_ATTACKER STAT_ANIM_ATK | STAT_ANIM_ACC STAT_ANIM_UP
	setstatchanger STAT_ATK | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR HoneClaws_Acc
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 HoneClaws_Acc
	printfromtable 0x83FE57C
	waitmessage DELAY_1SECOND

HoneClaws_Acc:
	setstatchanger STAT_ACC | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR BS_MOVE_END
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 BS_MOVE_END
	printfromtable 0x83FE57C
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_011_RaiseUserDef1
BS_011_RaiseUserDef1:
	attackcanceler
	attackstring
	ppreduce
	setstatchanger STAT_DEF | INCREASE_1
	jumpifmove MOVE_FLOWERSHIELD RaiseUserDef1_FlowerShield
	jumpifmove MOVE_MAGNETICFLUX RaiseUserDef1_MagneticFlux
	jumpifmove MOVE_AROMATICMIST RaiseUserDef1_AromaticMist
	goto 0x81D6BA1

RaiseUserDef1_FlowerShield:
	jumpiftypepresent NOTEQUALS TYPE_GRASS FAILED
	setword SEED_HELPER 0x0
	setbyte FLOWER_SHIELD_HELPER 0x0
	setbyte FORM_COUNTER 0x0

FlowerShieldBSLoop:
	callasm FlowerShieldLooper+1
	jumpifbyte EQUALS FORM_COUNTER 0xFF FlowerShieldEnder

RaiseUserDef1_BoostDefense:
	setbyte STAT_ANIM_PLAYED 0x0
	jumpifstat BANK_TARGET EQUALS STAT_DEF STAT_MAX FlowerShieldBSLoop
	attackanimation
	waitanimation
	playstatchangeanimation BANK_TARGET STAT_ANIM_DEF STAT_ANIM_UP
	setstatchanger STAT_DEF | INCREASE_1
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
	goto BS_MOVE_END

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

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
	setbyte STAT_ANIM_PLAYED 0x0
	playstatchangeanimation BANK_TARGET STAT_ANIM_DEF | STAT_ANIM_SPDEF STAT_ANIM_UP
	jumpifstat BANK_TARGET EQUALS STAT_DEF STAT_MAX RaiseUserDef1_SpDef
	setstatchanger STAT_DEF | INCREASE_1
	statbuffchange STAT_TARGET | STAT_BS_PTR RaiseUserDef1_SpDef
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 RaiseUserDef1_SpDef
	printfromtable 0x83FE57C
	waitmessage DELAY_1SECOND

RaiseUserDef1_SpDef:
	setstatchanger STAT_SPDEF | INCREASE_1
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

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

RaiseUserDef1_AromaticMist:
	jumpifbyte NOTANDS BATTLE_TYPE BATTLE_DOUBLE FAILED
	callasm SetTargetPartner+1
	jumpifspecialstatusflag EQUALS STATUS3_SEMI_INVULNERABLE 0x0 FAILED
	jumpiffainted BANK_TARGET FAILED
	callasm CheckCraftyShield+1
	jumpifbyte NOTEQUALS FORM_COUNTER FALSE FAILED
	setbyte FORM_COUNTER 0x0
	setbyte STAT_ANIM_PLAYED 0x0
	jumpifstat BANK_TARGET EQUALS STAT_SPDEF STAT_MAX FAILED
	attackanimation
	waitanimation
	playstatchangeanimation BANK_TARGET STAT_ANIM_SPDEF 0x0
	setstatchanger STAT_SPDEF | INCREASE_1
	statbuffchange STAT_TARGET | STAT_BS_PTR FAILED
	jumpifbyte 0x0 MULTISTRING_CHOOSER 0x2 FAILED
	printfromtable 0x83FE57C
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_012_RaiseUserSpd1
BS_012_RaiseUserSpd1:
	setstatchanger STAT_SPD | INCREASE_1
	goto BS_BUFF_ATK_STATS

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_013_RaiseUserSpAtk1
BS_013_RaiseUserSpAtk1:
	attackcanceler
	attackstring
	ppreduce
	setstatchanger STAT_SPATK | INCREASE_1
	jumpifmove MOVE_GROWTH GrowthBS
	jumpifmove MOVE_WORKUP WorkUpBS
	jumpifmove MOVE_ROTOTILLER RototillerBS
	jumpifmove MOVE_GEARUP GearUpBS
	goto 0x81D6BA1

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

GrowthBS:
Growth_CheckStats:
	jumpifstat BANK_ATTACKER LESSTHAN STAT_ATK STAT_MAX Growth_CheckSun
	jumpifstat BANK_ATTACKER EQUALS STAT_SPATK STAT_MAX 0x81D85E7

Growth_CheckSun:
	attackanimation
	waitanimation
	setbyte STAT_ANIM_PLAYED 0x0
	playstatchangeanimation BANK_ATTACKER STAT_ANIM_ATK | STAT_ANIM_SPATK 0x0
	jumpifabilitypresent ABILITY_CLOUDNINE Growth_NoSunBoostAtk
	jumpifabilitypresent ABILITY_AIRLOCK Growth_NoSunBoostAtk
	jumpifhalfword ANDS WEATHER_FLAGS WEATHER_SUN_ANY Growth_SunBoostAtk

Growth_NoSunBoostAtk:
	setstatchanger STAT_ATK | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR Growth_NoSunBoostSpAtk
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 Growth_NoSunBoostSpAtk
	printfromtable 0x83FE57C
	waitmessage DELAY_1SECOND

Growth_NoSunBoostSpAtk:
	setstatchanger STAT_SPATK | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR BS_MOVE_END
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 BS_MOVE_END
	printfromtable 0x83FE57C
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

Growth_SunBoostAtk:
	setstatchanger STAT_ATK | INCREASE_2
	statbuffchange STAT_ATTACKER | STAT_BS_PTR Growth_SunBoostSpAtk
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 Growth_SunBoostSpAtk
	printfromtable 0x83FE57C
	waitmessage DELAY_1SECOND

Growth_SunBoostSpAtk:
	setstatchanger STAT_SPATK | INCREASE_2
	statbuffchange STAT_ATTACKER | STAT_BS_PTR BS_MOVE_END
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 BS_MOVE_END
	printfromtable 0x83FE57C
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

WorkUpBS:
	jumpifstat BANK_ATTACKER 0x3 STAT_ATK STAT_MAX WorkUp_RaiseAtk
	jumpifstat BANK_ATTACKER EQUALS STAT_SPATK STAT_MAX 0x81D85E7

WorkUp_RaiseAtk:
	attackanimation
	waitanimation
	setbyte STAT_ANIM_PLAYED 0x0
	playstatchangeanimation BANK_ATTACKER STAT_ANIM_ATK | STAT_ANIM_SPATK 0x0
	setstatchanger STAT_ATK | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR WorkUp_RaiseSpAtk
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 WorkUp_RaiseSpAtk
	printfromtable 0x83FE57C
	waitmessage DELAY_1SECOND

WorkUp_RaiseSpAtk:
	setstatchanger STAT_SPATK | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR BS_MOVE_END
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 BS_MOVE_END
	printfromtable 0x83FE57C
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

RototillerBS:
	jumpiftypepresent NOTEQUALS TYPE_GRASS FAILED
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
	setbyte STAT_ANIM_PLAYED 0x0
	setbyte FLOWER_SHIELD_HELPER TRUE
	playstatchangeanimation BANK_TARGET STAT_ANIM_ATK | STAT_ANIM_SPATK 0x0
	setstatchanger STAT_ATK | INCREASE_1
	statbuffchange STAT_TARGET | STAT_BS_PTR Rototiller_RaiseSpAtk
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 Rototiller_RaiseSpAtk
	printfromtable 0x83FE57C
	waitmessage DELAY_1SECOND

Rototiller_RaiseSpAtk:
	setstatchanger STAT_SPATK | INCREASE_1
	statbuffchange STAT_TARGET | STAT_BS_PTR RototillerBSLoop
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 RototillerBSLoop
	printfromtable 0x83FE57C
	waitmessage DELAY_1SECOND
	goto RototillerBSLoop

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

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
	setbyte STAT_ANIM_PLAYED 0x0
	playstatchangeanimation BANK_TARGET STAT_ANIM_ATK | STAT_ANIM_SPATK 0x0
	jumpifstat BANK_TARGET EQUALS STAT_ATK STAT_MAX GearUp_SpAtk
	setstatchanger STAT_ATK | INCREASE_1
	statbuffchange STAT_TARGET | STAT_BS_PTR GearUp_SpAtk
	jumpifbyte 0x0 MULTISTRING_CHOOSER 0x2 GearUp_SpAtk
	printfromtable 0x83FE57C
	waitmessage DELAY_1SECOND

GearUp_SpAtk:
	setstatchanger STAT_SPATK | INCREASE_1
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
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_014_RaiseUserSpDef1
BS_014_RaiseUserSpDef1:
	setstatchanger STAT_SPDEF | INCREASE_1
	goto BS_BUFF_ATK_STATS

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_015_RaiseUserAcc1
BS_015_RaiseUserAcc1:
	setstatchanger STAT_ACC | INCREASE_1
	goto BS_BUFF_ATK_STATS

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_016_RaiseUserEvsn1
BS_016_RaiseUserEvsn1:
	jumpifnotmove MOVE_ACUPRESSURE 0x81D6B98

AcupressureBS:
	attackcanceler
	callasm AcupressureFunc+1
	attackstring
	ppreduce
	attackanimation
	waitanimation
	statbuffchange STAT_TARGET | STAT_BS_PTR | STAT_CERTAIN BS_MOVE_END
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 BS_MOVE_END
	setgraphicalstatchangevalues
	playanimation BANK_TARGET 0x1 0x2023FD4
	printfromtable 0x83FE57C
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_017_NeverMiss
BS_017_NeverMiss:
	goto 0x81D6926

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_018_LowerTargetAtk1
BS_018_LowerTargetAtk1:
	attackcanceler
	jumpifbehindsubstitute BANK_TARGET FAILED-2
	accuracycheck BS_MOVE_MISSED 0x0
	attackstring
	ppreduce
	jumpifmove MOVE_PLAYNICE PlayNiceBS
	jumpifmove MOVE_NOBLEROAR PlayNiceBS
	jumpifmove MOVE_TEARFULLOOK PlayNiceBS
	jumpifmove MOVE_VENOMDRENCH VenomDrenchBS
	setstatchanger STAT_ATK | DECREASE_1
	goto 0x81D6C27

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

PlayNiceBS:
	jumpifstatcanbelowered BANK_TARGET STAT_ATK PlayNice_Atk
	jumpifstatcanbelowered BANK_TARGET STAT_SPATK PlayNice_Atk
	goto CheckReasonForStatLowerFail

PlayNice_Atk:
	setbyte FORM_COUNTER 0x0
	attackanimation
	waitanimation
	setbyte STAT_ANIM_PLAYED 0x0
	playstatchangeanimation BANK_TARGET STAT_ANIM_ATK | STAT_ANIM_SPATK STAT_ANIM_DOWN | STAT_ANIM_CAN_FAIL
	setstatchanger STAT_ATK | DECREASE_1
	statbuffchange STAT_TARGET | STAT_BS_PTR PlayNice_SpAtk
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 PlayNice_SpAtk
	printfromtable 0x83FE588
	waitmessage DELAY_1SECOND

PlayNice_SpAtk:
	setstatchanger STAT_SPATK | DECREASE_1
	statbuffchange STAT_TARGET | STAT_BS_PTR BS_MOVE_END
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 BS_MOVE_END
	printfromtable 0x83FE588
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

VenomDrenchBS:
	jumpifstatus BANK_TARGET STATUS_POISON | STATUS_BAD_POISON DoVenomDrench
	goto NOEFFECT

DoVenomDrench:
	jumpifstatcanbelowered BANK_TARGET STAT_ATK VenomDrench_Atk
	jumpifstatcanbelowered BANK_TARGET STAT_SPATK VenomDrench_Atk
	jumpifstatcanbelowered BANK_TARGET STAT_SPD VenomDrench_Atk
	goto CheckReasonForStatLowerFail

VenomDrench_Atk:
	setbyte FORM_COUNTER 0x0
	attackanimation
	waitanimation
	setbyte STAT_ANIM_PLAYED 0x0
	playstatchangeanimation BANK_TARGET STAT_ANIM_ATK | STAT_ANIM_SPATK | STAT_ANIM_SPD STAT_ANIM_DOWN | STAT_ANIM_CAN_FAIL
	setstatchanger STAT_ATK | DECREASE_1
	statbuffchange STAT_TARGET | STAT_BS_PTR VenomDrench_SpAtk
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 VenomDrench_SpAtk
	printfromtable 0x83FE588
	waitmessage DELAY_1SECOND

VenomDrench_SpAtk:
	setstatchanger STAT_SPATK | DECREASE_1
	statbuffchange STAT_TARGET | STAT_BS_PTR VenomDrench_Spd
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 VenomDrench_Spd
	printfromtable 0x83FE588
	waitmessage DELAY_1SECOND

VenomDrench_Spd:
	setstatchanger STAT_SPD | DECREASE_1
	statbuffchange STAT_TARGET | STAT_BS_PTR BS_MOVE_END
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 BS_MOVE_END
	printfromtable 0x83FE588
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

CheckReasonForStatLowerFail:
	copyarray SEED_HELPER FORM_COUNTER 0x1
	setbyte FORM_COUNTER 0x0
	jumpifability BANK_ATTACKER ABILITY_CONTRARY 0x81D85E7 @;Stats won't go higher
	jumpifbyte SEED_HELPER 0x1 0x81D8499 @;Stats won't go lower
	jumpifbyte SEED_HELPER 0x2 MistProtectedBS
	jumpifbyte SEED_HELPER 0x3 AbilityPreventsStatLossBS
	jumpifbyte SEED_HELPER 0x4 0x81D947E @;Ability prevents specific stat loss
	goto FAILED

AbilityPreventsStatLossBS:
	call 0x81D9416 @;Ability prevents stat loss
	goto BS_MOVE_END

MistProtectedBS:
	call 0x81D8C3E @;Mist protected
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_019_LowerTargetDef1
BS_019_LowerTargetDef1:
	setstatchanger STAT_DEF | DECREASE_1
	goto 0x81D6C13
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_020_LowerTargetSpd1
BS_021_LowerTargetSpd1:
	setstatchanger STAT_SPD | DECREASE_1
	goto 0x81D6C13

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_021_LowerTargetSpAtk1
BS_021_LowerTargetSpAtk1:
	setstatchanger STAT_SPATK | DECREASE_1
	goto 0x81D6C13

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_022_LowerTargetSpDef1
BS_022_LowerTargetSpDef1:
	setstatchanger STAT_SPDEF | DECREASE_1
	goto 0x81D6C13

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_023_LowerTargetAcc1
BS_023_LowerTargetAcc1:
	setstatchanger STAT_ACC | DECREASE_1
	goto 0x81D6C13

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_024_LowerTargetEvsn1
BS_024_LowerTargetEvsn1:
	setstatchanger STAT_EVSN | DECREASE_1
	goto 0x81D6C13

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_025_Haze
BS_025_Haze:
	attackcanceler
	attackstring
	ppreduce
	attackanimation
	waitanimation
	normalisebuffs
	printstring 0xF9
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_026_Bide
BS_026_Bide:
	attackcanceler
	attackstring
	ppreduce
	attackanimation
	waitanimation
	orword HIT_MARKER HITMARKER_CHARGING
	setbide
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_027_Thrash
BS_027_Thrash:
	attackcanceler
	accuracycheck BS_MOVE_MISSED 0x0
	attackstring
	jumpifsecondarystatus BANK_ATTACKER STATUS2_MULTIPLETURNS SkipThrashPPReduce
	ppreduce

SkipThrashPPReduce:
	confuseifrepeatingattackends
	goto 0x81D6930

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_028_Roar
BS_028_Roar:
	jumpifmove MOVE_DRAGONTAIL DragonTailBS
	jumpifmove MOVE_CIRCLETHROW DragonTailBS
	goto 0x81D6CB1

DragonTailBS:
	attackcanceler
	accuracycheck BS_MOVE_MISSED 0x0
	jumpifbehindsubstitute BANK_TARGET 0x81D692E
	call StandardDamage
	jumpiffainted BANK_TARGET BS_MOVE_FAINT
	jumpifmovehadnoeffect BS_MOVE_FAINT
	jumpifspecialstatusflag BANK_TARGET STATUS3_ROOTED 0x0 0x81D8F27
	jumpifability BANK_TARGET ABILITY_SUCTIONCUPS 0x81D9408
	setbyte CMD49_STATE 0x0
	cmd49 BANK_TARGET 0x0
	playanimation BANK_TARGET DRAGON_TAIL_BLOW_AWAY_ANIM 0x0
	forcerandomswitch 0x81D6957

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_029_HitTwoToFiveTimes
BS_029_HitTwoToFiveTimes:
	goto 0x81D6926

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_030_Conversion
BS_030_Conversion:
	jumpifnotmove MOVE_REFLECTTYPE 0x81D6D9D
	attackcanceler
	accuracycheck BS_MOVE_MISSED 0x0
	attackstring
	ppreduce
	jumpifbehindsubstitute BANK_TARGET FAILED
	changetypestoenemyattacktype FAILED
	attackanimation
	waitanimation
	setword BATTLE_STRING_LOADER ReflectTypeString
	printstring 0x184
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_031_Flinch
BS_031_Flinch:
	setmoveeffect MOVE_EFFECT_FLINCH
	jumpifmove MOVE_THUNDERFANG ThunderFangBS
	jumpifmove MOVE_FIREFANG FireFangBS
	jumpifmove MOVE_ICEFANG IceFangBS
	goto 0x81D6926
	
ThunderFangBS:
	attackcanceler
	accuracycheck BS_MOVE_MISSED 0x0
	call STANDARD_DAMAGE
	seteffectwithchancetarget
	setmoveeffect MOVE_EFFECT_PARALYSIS
	seteffectwithchancetarget
	goto BS_MOVE_FAINT

FireFangBS:
	attackcanceler
	accuracycheck BS_MOVE_MISSED 0x0
	call STANDARD_DAMAGE
	seteffectwithchancetarget
	setmoveeffect MOVE_EFFECT_BURN
	seteffectwithchancetarget
	goto BS_MOVE_FAINT

IceFangBS:
	attackcanceler
	accuracycheck BS_MOVE_MISSED 0x0
	call STANDARD_DAMAGE
	seteffectwithchancetarget
	setmoveeffect MOVE_EFFECT_FREEZE
	seteffectwithchancetarget
	goto BS_MOVE_FAINT

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_032_Recover
BS_032_Recover:
	attackcanceler
	attackstring
	ppreduce
	jumpifhalfword EQUALS CURRENT_MOVE MOVE_ROOST RoostBS
	jumpifhalfword EQUALS CURRENT_MOVE MOVE_PURIFY PurifyBS
	goto 0x81D6DC0

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

RoostBS:
	setdamageasrestorehalfmaxhp 0x81D7DD1 0x1
	attackanimation
	waitanimation
	orword HITMARKER HITMARKER_IGNORE_SUBSTITUTE
	graphicalhpupdate BANK_ATTACKER
	datahpupdate BANK_ATTACKER
	printstring 0x4B
	waitmessage DELAY_1SECOND
	jumpiftype BANK_ATTACKER TYPE_FLYING RemoveFlyingType
	goto BS_MOVE_END

RemoveFlyingType:
	callasm SetRoostFunc+1
	goto BS_MOVE_END

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

PurifyBS:
	jumpifstatus BANK_TARGET STATUS_ANY PurifyHeal
	goto FAILED

PurifyHeal:
	cureprimarystatus BANK_TARGET
	attackanimation
	waitanimation
	setword BATTLE_STRING_LOADER PurifyString
	printstring 0x184
	waitmessage DELAY_1SECOND
	refreshhpbar BANK_TARGET
	setdamageasrestorehalfmaxhp 0x81D7DD1 0x1
	playanimation BANK_ATTACKER ANIM_HEALING_SPARKLES 0x0
	orword HITMARKER HITMARKER_IGNORE_SUBSTITUTE
	graphicalhpupdate BANK_ATTACKER
	datahpupdate BANK_ATTACKER
	printstring 0x4B
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_033_SetBadPoison
BS_033_SetBadPoison:
	attackcanceler
	attackstring
	ppreduce
	jumpifbehindsubstitute BANK_TARGET FAILED
	jumpifability BANK_TARGET ABILITY_COMATOSE FAILED
	jumpifspecies BANK_TARGET PKMN_MINIORSHIELD FAILED
	jumpifabilitypresent ABILITY_CLOUDNINE BadPSN_CheckFlowerVeil
	jumpifabilitypresent ABILITY_AIRLOCK BadPSN_CheckFlowerVeil
	jumpifability BANK_TARGET ABILITY_LEAFGUARD BadPSN_SunnyCheck

BadPSN_CheckFlowerVeil:
	jumpifabilitypresenttargetfield ABILITY_FLOWERVEIL BadPSN_GrassTypeCheck

BadPSN_CheckTerrain:
	jumpifbyte EQUALS TERRAIN_BYTE MISTY_TERRAIN BadPSN_CheckGrounding

BadPSN_After:
	jumpifability BANK_TARGET ABILITY_IMMUNITY 0x81D6E4F
	jumpifstatus BANK_TARGET STATUS_POISON | STATUS_BAD_POISON 0x81D6E41 @;Already Poisoned
	jumpifstatus BANK_TARGET STATUS_ANY FAILED
	jumpifability BANK_ATTACKER ABILITY_CORROSION 0x81D6E20
	jumpiftype BANK_TARGET TYPE_POISON NOEFFECT
	jumpiftype BANK_TARGET TYPE_STEEL NOEFFECT
	goto 0x81D6E20

BadPSN_SunnyCheck:
	jumpifhalfword ANDS WEATHER_FLAGS WEATHER_SUN_ANY ProtectedByAbilityBS
	goto BadPSN_CheckFlowerVeil

BadPSN_GrassTypeCheck:
	jumpiftype BANK_TARGET TYPE_GRASS TeamProtectedByFVBS
	goto BadPSN_CheckTerrain

BadPSN_CheckGrounding:
	jumpifgrounded BANK_TARGET ProtectedByTerrainBS
	goto BadPSN_After

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_034_PayDay
BS_034_PayDay:
	setmoveeffect MOVE_EFFECT_PAYDAY
	goto 0x81D6926

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_035_LightScreen
BS_035_LightScreen:
	attackcanceler
	attackstring
	ppreduce
	setlightscreen
	goto 0x81D7172
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_036_TriAttack
BS_036_TriAttack:
	setmoveeffect MOVE_EFFECT_TRI_ATTACK
	goto 0x81D6926

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_037_SetRest
BS_037_SetRest:
	attackcanceler
	attackstring
	ppreduce
	jumpifstatus BANK_ATTACKER STATUS_SLEEP 0x81D6EC2 @;Attacker Already Asleep
	jumpifability BANK_ATTACKER ABILITY_COMATOSE 0x81D6EC2 @;Attacker Already Asleep
	jumpifspecies BANK_ATTACKER PKMN_MINIORSHIELD FAILED
	jumpifabilitypresent ABILITY_CLOUDNINE Rest_CheckVeils
	jumpifabilitypresent ABILITY_AIRLOCK Rest_CheckVeils
	jumpifability BANK_ATTACKER ABILITY_LEAFGUARD Rest_SunnyCheck

Rest_CheckVeils:
	jumpifabilitypresenttargetfield ABILITY_SWEETVEIL FAILED

Rest_CheckTerrain:
	jumpifbyte EQUALS TERRAIN_BYTE ELECTRIC_TERRAIN Rest_CheckGrounding
	jumpifbyte EQUALS TERRAIN_BYTE MISTY_TERRAIN Rest_CheckGrounding

Rest_After:
	goto 0x81D6E95

Rest_SunnyCheck:
	jumpifhalfword ANDS WEATHER_FLAGS WEATHER_SUN_ANY FAILED
	goto Rest_CheckVeils

Rest_CheckGrounding:
	jumpifgrounded BANK_ATTACKER FAILED
	goto Rest_After

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_038_0HK0
BS_038_0HK0:
	attackcanceler
	attackstring
	ppreduce
	accuracycheck FAILED 0xFFFF
	typecalc
	jumpifbyte ANDS OUTCOME OUTCOME_NO_EFFCT 0x81D6934
	tryko 0x81D6EF1
	trysetdestinybondtohappen
	goto 0x81D6934

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_039_RazorWind
BS_039_RazorWind:
	jumpifsecondarystatus BANK_ATTACKER STATUS2_MULTIPLETURNS TwoTurnMovesSecondTurnBS
	jumpifword ANDS HITMARKER HITMARKER_NO_ATTACKSTRING TwoTurnMovesSecondTurnBS
	setbyte TWOTURN_STRINGID 0x0
	call TwoTurnMovesChargingTurnBS
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_040_SuperFang
BS_040_SuperFang:
	attackcanceler
	accuracycheck BS_MOVE_MISSED 0x0
	attackstring
	ppreduce
	typecalc
	bicbyte OUTCOME OUTCOME_SUPER_EFFECTIVE | OUTCOME_NOT_VERY_EFFECTIVE
	gethalfcurrentenemyhp
	goto 0x81D6934

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_041_DragonRage
BS_041_DragonRage:
	attackcanceler
	accuracycheck BS_MOVE_MISSED 0x0
	attackstring
	ppreduce
	typecalc
	bicbyte OUTCOME OUTCOME_SUPER_EFFECTIVE | OUTCOME_NOT_VERY_EFFECTIVE
	setword DAMAGE_LOC 0x0
	setbyte DAMAGE_LOC 40
	adjustsetdamage
	goto 0x81D6934

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_042_TrapAndDamage
BS_042_TrapAndDamage:
	jumpifnotmove MOVE_WHIRLPOOL SetWrapEffectBS
	jumpifspecialstatusflag BANK_TARGET STATUS3_UNDERWATER 0x1 SetWrapEffectBS
	orword HIT_MARKER HITMARKER_IGNORE_UNDERWATER

SetWrapEffectBS:
	setmoveeffect MOVE_EFFECT_WRAP
	goto 0x81D6926
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_043_HighCritRatio
BS_043_HighCritRatio:
	goto 0x81D6926
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_044_HitTwice
BS_044_HitTwice:
	goto 0x81D6926

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_045_HighJumpKick
BS_045_HighJumpKick:
	jumpifability BANK_ATTACKER ABILITY_MAGICGUARD 0x81D6926
	attackcanceler
	accuracycheck HighJumpKickMiss 0x0
	goto 0x81D692E

HighJumpKickMiss:
	attackstring
	ppreduce
	pause DELAY_HALFSECOND
	resultmessage
	waitmessage DELAY_1SECOND
	printstring 0x60
	waitmessage DELAY_1SECOND
	bicbyte OUTCOME OUTCOME_MISSED
	orword HIT_MARKER 0x100
	setdamagetobankhealthpercent BANK_ATTACKER 50
	graphicalhpupdate BANK_ATTACKER
	datahpupdate BANK_ATTACKER
	faintpokemon BANK_ATTACKER 0x0 0x8000000
	orbyte OUTCOME OUTCOME_MISSED
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_046_Mist
BS_046_Mist:
	attackcanceler
	attackstring
	ppreduce
	setmisteffect
	attackanimation
	waitanimation
	printfromtable 0x83FE5AC
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_047_FocusEnergy
BS_047_FocusEnergy:
	attackcanceler
	attackstring
	ppreduce
	jumpifsecondarystatus BANK_ATTACKER STATUS2_PUMPEDUP FAILED
	setincreasedcriticalchance
	attackanimation
	waitanimation
	printfromtable 0x83FE5B0
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_048_25PercentRecoil
BS_048_25PercentRecoil:
	goto 0x81D6926

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_049_SetConfusion
BS_049_SetConfusion:
	attackcanceler
	attackstring
	ppreduce
	jumpifbehindsubstitute BANK_TARGET FAILED
	jumpifsecondarystatus BANK_TARGET STATUS2_CONFUSION 0x81D70EA @;Already confused
	jumpifability BANK_TARGET ABILITY_OWNTEMPO 0x81D9460 @;Ability prevents confusion
	jumpifbyte EQUALS TERRAIN_BYTE MISTY_TERRAIN CFSN_CheckGrounding
	goto 0x81D70BF

CFSN_CheckGrounding:
	jumpifgrounded BANK_TARGET ProtectedByTerrainBS
	goto 0x81D70BF

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_050_RaiseUserAtk2
BS_050_RaiseUserAtk2:
	setstatchanger STAT_ATK | INCREASE_2
	goto BS_BUFF_ATK_STATS

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_051_RaiseUserDef2
BS_051_RaiseUserDef2:
	setstatchanger STAT_DEF | INCREASE_2
	jumpifhalfword NOTEQUALS CURRENT_MOVE MOVE_COTTONGUARD BS_BUFF_ATK_STATS
	setstatchanger STAT_DEF | INCREASE_3
	goto BS_BUFF_ATK_STATS
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_052_RaiseUserSpd2
BS_052_RaiseUserSpd2:
	setstatchanger STAT_SPD | INCREASE_2
	jumpifnotmove MOVE_AUTOTOMIZE BS_BUFF_ATK_STATS

AutotomizeBS:
	attackcanceler
	attackstring
	ppreduce
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
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_053_RaiseUserSpAtk2
BS_053_RaiseUserSpAtk2:
	setstatchanger STAT_SPATK | INCREASE_2
	jumpifnotmove MOVE_TAILGLOW BS_BUFF_ATK_STATS
	setbyte STAT_CHANGE_BYTE STAT_SPATK | INCREASE_3
	goto BS_BUFF_ATK_STATS

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_054_RaiseUserSpDef2
BS_054_RaiseUserSpDef2:
	setstatchanger STAT_SPDEF | INCREASE_2
	goto BS_BUFF_ATK_STATS

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_055_RaiseUserAcc2
BS_055_RaiseUserAcc2:
	setstatchanger STAT_ACC | INCREASE_2
	goto BS_BUFF_ATK_STATS

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_056_RaiseUserEvsn2
BS_056_RaiseUserEvsn2:
	setstatchanger STAT_EVSN | INCREASE_2
	goto BS_BUFF_ATK_STATS

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_057_Transform
BS_057_Transform:
	attackcanceler
	jumpifbehindsubstitute BANK_TARGET FAILED-2
	jumpifsecondarystatus BANK_ATTACKER STATUS2_TRANSFORMED FAILED-2
	jumpifspecialstatusflag BANK_TARGET STATUS3_ILLUSION 0x0 FAILED-2
	attackstring
	ppreduce
	transformdataexecution
	attackanimation
	waitanimation
	printfromtable 0x83FE5B4
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_058_LowerTargetAtk2
BS_058_LowerTargetAtk2:
	setstatchanger STAT_ATK | DECREASE_2
	goto 0x81D6C13

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_058_LowerTargetAtk2
BS_058_LowerTargetAtk2:
	setstatchanger STAT_ATK | DECREASE_2
	goto 0x81D6C13

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_059_LowerTargetDef2
BS_059_LowerTargetDef2:
	setstatchanger STAT_DEF | DECREASE_2
	goto 0x81D6C13

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_060_LowerTargetSpd2
BS_060_LowerTargetSpd2:
	setstatchanger STAT_SPD | DECREASE_2
	goto 0x81D6C13

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_061_LowerTargetSpAtk2
BS_061_LowerTargetSpAtk2:
	setstatchanger STAT_SPATK | DECREASE_2
	jumpifnotmove MOVE_CAPTIVATE 0x81D6C13
	
CaptivateBS:
	attackcanceler
	jumpifbehindsubstitute BANK_TARGET FAILED-2
	accuracycheck BS_MOVE_MISSED 0x0
	attackstring
	ppreduce
	jumpifability BANK_TARGET ABILITY_OBLIVIOUS NOEFFECT
	callasm CaptivateFunc+1
	goto 0x81D6C27

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_062_LowerTargetSpDef2
BS_062_LowerTargetSpDef2:
	setstatchanger STAT_SPDEF | DECREASE_2
	goto 0x81D6C13

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_063_LowerTargetAcc2
BS_063_LowerTargetAcc2:
	setstatchanger STAT_ACC | DECREASE_2
	goto 0x81D6C13

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_064_LowerTargetEvsn2
BS_064_LowerTargetEvsn2:
	setstatchanger STAT_EVSN | DECREASE_2
	goto 0x81D6C13
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_065_Reflect
BS_065_Reflect:
	jumpifnotmove MOVE_AURORAVEIL 0x1D716E
	attackcanceler
	attackstring
	ppreduce
	callasm SetAuroraVeil + 1
	attackanimation
	waitanimation
	setword BATTLE_STRING_LOADER AuroraVeilSetString
	printstring 0x184
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_066_SetPoison
BS_066_SetPoison:
	attackcanceler
	attackstring
	ppreduce
	jumpifmove MOVE_TOXICTHREAD ToxicThreadDo

PoisonChecks:
	jumpifbehindsubstitute BANK_TARGET FAILED
	jumpifability BANK_TARGET ABILITY_COMATOSE FAILED
	jumpifspecies BANK_TARGET PKMN_MINIORSHIELD FAILED
	jumpifabilitypresent ABILITY_CLOUDNINE PSN_CheckFlowerVeil
	jumpifabilitypresent ABILITY_AIRLOCK PSN_CheckFlowerVeil
	jumpifability BANK_TARGET ABILITY_LEAFGUARD PSN_SunnyCheck

PSN_CheckFlowerVeil:
	jumpifabilitypresenttargetfield ABILITY_FLOWERVEIL PSN_GrassTypeCheck

PSN_CheckTerrain:
	jumpifbyte EQUALS TERRAIN_BYTE MISTY_TERRAIN PSN_CheckGrounding

PSN_After:
	jumpifability BANK_TARGET ABILITY_IMMUNITY 0x81D6E4F
	jumpifstatus BANK_TARGET STATUS_POISON 0x81D6E41 @;Already poisoned
	jumpifstatus BANK_TARGET STATUS_BAD_POISON 0x81D6E41 @;Already poisoned
	jumpifstatus BANK_TARGET STATUS_ANY FAILED
	jumpifability BANK_ATTACKER ABILITY_CORROSION 0x81D71C1
	jumpiftype BANK_TARGET TYPE_POISON NOEFFECT
	jumpiftype BANK_TARGET TYPE_STEEL NOEFFECT
	goto 0x81D71C1

PSN_SunnyCheck:
	jumpifhalfword ANDS WEATHER_FLAGS WEATHER_SUN_ANY ProtectedByAbilityBS
	goto PSN_CheckFlowerVeil

PSN_GrassTypeCheck:
	jumpiftype BANK_TARGET TYPE_GRASS TeamProtectedByFVBS
	goto PSN_CheckTerrain

PSN_CheckGrounding:
	jumpifgrounded BANK_TARGET ProtectedByTerrainBS
	goto PSN_After

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

ToxicThreadDo:
	jumpifbehindsubstitute BANK_TARGET FAILED
	accuracycheck BS_MOVE_MISSED+2 0x0
	setstatchanger STAT_SPD | DECREASE_1
	statbuffchange STAT_TARGET | STAT_BS_PTR ToxicThreadPSN
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 ToxicThreadPSN

ToxicThreadWork:
	attackanimation
	waitanimation
	setgraphicalstatchangevalues
	playanimation BANK_TARGET ANIM_STAT_BUFF 0x2023FD4
	printfromtable 0x83FE588
	waitmessage DELAY_1SECOND
	setmoveffect MOVE_EFFECT_POISON
	seteffecttarget
	goto BS_MOVE_END

ToxicThreadPSN:
	bicbyte OUTCOME OUTCOME_MISSED
	goto PoisonChecks

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_067_SetParalyze
BS_067_SetParalyze:
	attackcanceler
	attackstring
	ppreduce
	jumpifbehindsubstitute BANK_TARGET FAILED
	jumpifspecies BANK_TARGET PKMN_MINIORSHIELD FAILED
	jumpifability BANK_TARGET ABILITY_COMATOSE FAILED
	jumpifstatus BANK_TARGET STATUS_PARALYSIS 0x81D7237 @;Already paralyzed
	jumpifstatus BANK_TARGET STATUS_ANY FAILED
	jumpifabilitypresent ABILITY_CLOUDNINE PRZ_CheckFlowerVeil
	jumpifabilitypresent ABILITY_AIRLOCK PRZ_CheckFlowerVeil
	jumpifability BANK_TARGET ABILITY_LEAFGUARD PRZ_SunnyCheck

PRZ_CheckFlowerVeil:
	jumpifabilitypresenttargetfield ABILITY_FLOWERVEIL PRZ_GrassTypeCheck

PRZ_CheckTerrain:
	jumpiftype BANK_TARGET TYPE_ELECTRIC NOEFFECT
	jumpifbyte EQUALS TERRAIN_BYTE MISTY_TERRAIN PRZ_CheckGrounding

PRZ_After:
	jumpifability BANK_TARGET ABILITY_LIMBER 0x81D7245
	typecalc
	jumpifmovehadnoeffect FAILED
	goto 0x81D7216

PRZ_SunnyCheck:
	jumpifhalfword ANDS WEATHER_FLAGS WEATHER_SUN_ANY ProtectedByAbilityBS
	goto PRZ_CheckFlowerVeil

PRZ_GrassTypeCheck:
	jumpiftype BANK_TARGET TYPE_GRASS TeamProtectedByFVBS
	goto PRZ_CheckTerrain

PRZ_CheckGrounding:
	jumpifgrounded BANK_TARGET ProtectedByTerrainBS
	goto PRZ_After

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_068_LowerTargetAtk1Chance
BS_068_LowerTargetAtk1Chance:
	setmoveeffect MOVE_EFFECT_ATK_MINUS_1
	goto 0x81D6926

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_069_LowerTargetDef1Chance
BS_069_LowerTargetDef1Chance:
	setmoveeffect MOVE_EFFECT_DEF_MINUS_1
	goto 0x81D6926

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_070_LowerTargetSpd1Chance
BS_070_LowerTargetSpd1Chance:
	setmoveeffect MOVE_EFFECT_SPD_MINUS_1
	goto 0x81D6926

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_071_LowerTargetSpAtk1Chance
BS_071_LowerTargetSpAtk1Chance:
	setmoveeffect MOVE_EFFECT_SPATK_MINUS_1
	goto 0x81D6926

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_072_LowerTargetSpDef1Chance
BS_072_LowerTargetSpDef1Chance:
	jumpifhalfword EQUALS CURRENT_MOVE MOVE_ACIDSPRAY AcidSprayBS
	jumpifhalfword EQUALS CURRENT_MOVE MOVE_SEEDFLARE AcidSprayBS
	setmoveeffect MOVE_EFFECT_SPDEF_MINUS_1
	goto 0x81D6926

AcidSprayBS:
	setmoveeffect MOVE_EFFECT_SPDEF_MINUS_2
	goto 0x81D6926

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_073_LowerTargetAcc1Chance
BS_073_LowerTargetAcc1Chance:
	setmoveeffect MOVE_EFFECT_ACC_MINUS_1
	goto 0x81D6926

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_074_LowerTargetEvsn1Chance
BS_074_LowerTargetEvsn1Chance:
	setmoveeffect MOVE_EFFECT_EVSN_MINUS_1
	goto 0x81D6926

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_075_SkyAttack
BS_075_SkyAttack:
	jumpifsecondarystatus BANK_ATTACKER STATUS2_MULTIPLETURNS TwoTurnMovesSecondTurnBS
	jumpifword ANDS HITMARKER HITMARKER_NO_ATTACKSTRING TwoTurnMovesSecondTurnBS
	setbyte TWOTURN_STRINGID 0x3
	call TwoTurnMovesChargingTurnBS
	goto BS_MOVE_END

TwoTurnMovesChargingTurnBS:
	attackcanceler
	printstring 0x130 @;Blank String
	ppreduce
	attackanimation
	waitanimation
	jumpifmove MOVE_FREEZESHOCK PrintFreezeShockString
	jumpifmove MOVE_ICEBURN PrintIceBurnString
	jumpifmove MOVE_SHADOWFORCE PrintShadowForceString
	jumpifmove MOVE_PHANTOMFORCE PrintShadowForceString
	copyarray MULTISTRING_CHOOSER TWOTURN_STRINGID 0x1
	printfromtable 0x83FE590
	goto PowerHerbCheckBS
	
PrintFreezeShockString:
	setword BATTLE_STRING_LOADER FreezeShockChargingString
	printstring 0x184
	goto PowerHerbCheckBS

PrintIceBurnString:
	setword BATTLE_STRING_LOADER IceBurnChargingString
	printstring 0x184
	goto PowerHerbCheckBS
	
PrintShadowForceString:
	setword BATTLE_STRING_LOADER ShadowForceString
	printstring 0x184

PowerHerbCheckBS:
	waitmessage DELAY_1SECOND
	jumpifhelditemeffect BANK_ATTACKER ITEM_EFFECT_POWER_HERB PowerHerbChargeBS
	orword HITMARKER HITMARKER_CHARGING
	setmoveeffect MOVE_EFFECT_CHARGING | MOVE_EFFECT_AFFECTS_USER
	seteffecttarget
	return

PowerHerbChargeBS:
	playanimation BANK_ATTACKER ANIM_ITEM_USE 0x0
	setword BATTLE_STRING_LOADER PowerHerbString
	printstring 0x184
	waitmessage DELAY_1SECOND
	setbyte ANIM_TARGETS_HIT 0x0
	setbyte ANIM_TURN 0x1
	goto PowerHerbSkipBS

TwoTurnMovesSecondTurnBS:
	attackcanceler
	setmoveeffect MOVE_EFFECT_CHARGING
	setbyte ANIM_TURN 0x1
	clearstatus BANK_ATTACKER
	
PowerHerbSkipBS:
	orword HITMARKER HITMARKER_NO_PPDEDUCT
	clearsemiinvulnerablebit
	jumpifmove MOVE_RAZORWIND RazorWindEffectBS
	jumpifmove MOVE_BOUNCE FreezeShockEffectBS
	jumpifmove MOVE_FREEZESHOCK FreezeShockEffectBS
	jumpifmove MOVE_ICEBURN IceBurnEffectBS
	goto 0x81D6927
	
RazorWindEffectBS:
	setmoveeffect MOVE_EFFECT_FLINCH
	goto 0x81D6927
	
FreezeShockEffectBS:
	setmoveeffect MOVE_EFFECT_PARALYSIS
	goto 0x81D6927
	
IceBurnEffectBS:
	setmoveeffect MOVE_EFFECT_BURN
	goto 0x81D6927

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_076_SetConfusionChance
BS_076_SetConfusionChance:
	setmoveeffect MOVE_EFFECT_CONFUSION
	goto 0x81D6926

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_077_Twineedle
BS_077_Twineedle:
	setmoveeffect MOVE_EFFECT_POISON
	goto 0x81D6926

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_078_VitalThrow
BS_078_VitalThrow:
	goto 0x81D6926

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_079_Substitute
BS_079_Substitute:
	attackcanceler
	ppreduce
	attackstring
	waitstate
	jumpifbehindsubstitute BANK_ATTACKER 0x81D7321
	setsubstituteeffect
	jumpifbyte NOTEQUALS MULTISTRING_CHOOSER 0x1 0x81D730E
	pause DELAY_HALFSECOND
	goto 0x81D7314

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_080_Recharge
BS_080_Recharge:
	attackcanceler
	accuracycheck 0x81D695E 0x0
	setmoveeffect MOVE_EFFECT_RECHARGE | MOVE_EFFECT_CERTAIN | MOVE_EFFECT_AFFECTS_USER
	goto 0x81D692E

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_081_Rage
BS_081_Rage:
	attackcanceler
	accuracycheck 0x81D7367 0x0
	setmoveeffect MOVE_EFFECT_RAGE
	call STANDARD_DAMAGE
	jumpifbyte ANDS OUTCOME OUTCOME_NO_EFFECT BS_MOVE_FAINT
	seteffecttarget
	goto BS_MOVE_FAINT
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_082_Mimic
BS_082_Mimic:
	attackcanceler
	attackstring
	ppreduce
	jumpifbehindsubstitute BANK_TARGET FAILED
	accuracycheck FAILED 0xFFFF
	copyattack FAILED
	attackanimation
	waitanimation
	printstring 0x16
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_083_Metronome
BS_083_Metronome:
	attackcanceler
	attackstring
	pause DELAY_HALFSECOND
	attackanimation
	waitanimation
	setbyte ANIM_TURN 0x0
	setbyte ANIM_TARGETS_HIT 0x0
	metronomeeffect

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_084_LeechSeed
BS_084_LeechSeed:
	attackcanceler
	attackstring
	pause DELAY_HALFSECOND
	ppreduce
	jumpifbehindsubstitute BANK_TARGET FAILED
	accuracycheck SetLeechSeedBS 0x0
	
SetLeechSeedBS:
	setleechseed
	attackanimation
	waitanimation
	printfromtable 0x83FE558
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_085_Splash
BS_085_Splash:
	attackcanceler
	attackstring
	ppreduce
	attackanimation
	waitanimation
	incrementgamestat 0x1A @;Used Splash
	printstring 0xE4
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_086_Disable
BS_086_Disable:
	attackcanceler
	attackstring
	ppreduce
	jumpifabilitypresenttargetfield ABILITY_AROMAVEIL ProtectedByAromaVeil
	goto 0x81D73EA

ProtectedByAromaVeil:
	pause DELAY_HALFSECOND
	setword BATTLE_STRING_LOADER 0x0890A9C0
	printstring 0x184
	waitmessage DELAY_1SECPMD
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_087_SeismicToss
BS_087_SeismicToss:
	attackcanceler
	accuracycheck BS_MOVE_MISSED 0x0
	attackstring
	ppreduce
	typecalc
	bicbyte OUTCOME OUTCOME_SUPER_EFFECTIVE | OUTCOME_NOT_VERY_EFFECTIVE
	nightshadedamageeffect
	adjustsetdamage
	goto 0x81D6934

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_088_Psywave
BS_088_Psywave:
	attackcanceler
	accuracycheck BS_MOVE_MISSED 0x0
	attackstring
	ppreduce
	typecalc
	bicbyte OUTCOME OUTCOME_SUPER_EFFECTIVE | OUTCOME_NOT_VERY_EFFECTIVE
	psywavedamageeffect
	adjustsetdamage
	goto 0x81D6934

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_089_Counter
BS_089_Counter:
	jumpifnotmove MOVE_METALBURST 0x81D7433
	attackcanceler
	accuracycheck BS_MOVE_MISSED 0x0
	attackstring
	ppreduce
	callasm MetalBurstDamageCalculator + 1
	typecalc2
	adjustsetdamage
	goto 0x81D6934
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_090_Encore
BS_090_Encore:
	attackcanceler	
	accuracycheck BS_MOVE_MISSED 0x0
	attackstring
	ppreduce
	jumpifabilitypresenttargetfield ABILITY_AROMAVEIL ProtectedByAromaVeil
	goto 0x81D7453

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_092_Snore
BS_092_Snore:
	attackcanceler
	jumpifstatus BANK_ATTACKER STATUS_SLEEP 0x81D74AE
	jumpifability BANK_ATTACKER ABILITY_COMATOSE 0x81D74AE
	goto FAILED - 2

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_093_PainSplit
BS_093_PainSplit:
	attackcanceler
	attackstring
	ppreduce
	accuracycheck FAILED 0xFFFF
	painsplitdamagecalculator FAILED
	attackanimation
	waitanimation
	orword HIT_MARKER HITMARKER_IGNORE_SUBSTITUTE
	graphicalhpupdate BANK_ATTACKER
	datahpupdate BANK_ATTACKER
	copyarray DAMAGE_LOC PAIN_SPLIT_HP 0x4
	graphicalhpupdate BANK_TARGET
	datahpupdate BANK_TARGET
	printstring 0xFC
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_094_LockOn
BS_094_LockOn:
	jumpifnotmove MOVE_LASERFOCUS 0x1D74EB
	attackcanceler
	attackstring
	ppreduce
	setcounter BANK_ATTACKER LASER_FOCUS_COUNTER 0x2
	attackanimation
	waitanimation
	setword BATTLE_STRING_LOADER LaserFocusString
	printstring 0x184
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_095_Sketch
BS_095_Sketch:
	attackcanceler
	attackstring
	ppreduce
	jumpifbehindsubstitute BANK_TARGET FAILED
	copymovepermanently FAILED
	attackanimation
	waitanimation
	printstring 0x8A
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_096_RaiseAttackerSpd1Chance
BS_096_RaiseAttackerSpd1Chance:
	setmoveeffect MOVE_EFFECT_SPD_PLUS_1 | MOVE_EFFECT_AFFECTS_USER
	goto 0x81D6926

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_097_SleepTalk
BS_097_SleepTalk:
	attackcanceler
	jumpifstatus BANK_ATTACKER STATUS_SLEEP 0x81D753E
	jumpifability BANK_ATTACKER ABILITY_COMATOSE 0x81D753E
	goto FAILED - 2

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_098_DestinyBond
BS_098_DestinyBond:
	attackcanceler
	attackstring
	ppreduce
	destinybondeffect
	attackanimation
	waitanimation
	printstring 0x8B
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_099_Flail
BS_099_Flail:
	goto 0x81D6926

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_100_Spite
BS_100_Spite:
	attackcanceler
	attackstring
	ppreduce
	accuracycheck FAILED 0x0
	reducepprandom FAILED
	attackanimation
	waitanimation
	printstring 0x8D
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_101_FalseSwipe
BS_101_FalseSwipe:
	goto 0x81D6926

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_102_HealBell
BS_102_HealBell:
	attackcanceler
	attackstring
	ppreduce
	clearstatusifnotsoundproofed
	waitstateatk
	attackanimation
	waitanimation
	printfromtable 0x83FE5E4
	waitmessage DELAY_1SECOND
	jumpifmove MOVE_AROMATHERAPY AromatherapySapSipperCheckBS
	jumpifnotmove MOVE_HEALBELL 0x81D75DE
	jumpifbyte NOTANDS MULTISTRING_CHOOSER 0x1 0x81D75CD
	printstring 0x132
	waitmessage DELAY_1SECOND
	goto 0x81D75CD
	
AromatherapySapSipperCheckBS:
	callasm TryActivatePartnerSapSipper + 1
	goto 0x81D75DE
	
BattleScript_SapSipperAromatherapy:
	statbuffchange STAT_TARGET | STAT_BS_PTR SapSipperReturnBS
	setword BATTLE_STRING_LOADER AbilityRaisedStatString
	printstring 0x184
	waitmessage DELAY_1SECOND
SapSipperReturnBS:
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_103_PriorityMove
BS_103_PriorityMove:
	goto 0x81D6926

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_104_TripleKick
BS_104_TripleKick:
	attackcanceler
	attackstring
	ppreduce
	accuracycheck BS_MOVE_MISSED 0x0
	addbyte TRIPLE_KICK_POWER, 10
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_105_StealItem
BS_105_StealItem:
	setmoveeffect MOVE_EFFECT_STEAL_ITEM @;Move to CMD49?
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_106_MeanLook
BS_106_MeanLook:
	jumpifmove MOVE_ANCHORSHOT DamageAndTrapBS
	jumpifmove MOVE_SPIRITSHACKLE DamageAndTrapBS
	jumpifmove MOVE_THOUSANDWAVES DamageAndTrapBS
	attackcanceler
	attackstring
	ppreduce
	accuracycheck 0x81D7DF2 0xFFFE
	jumpiftype BANK_TARGET TYPE_GHOST FAILED
	goto 0x1D76DE
	
DamageAndTrapBS:
	jumpifsecondarystatus BANK_TARGET STATUS2_TRAPPED | STATUS2_SUBSTITUTE 0x81D6926
	jumpiftype BANK_TARGET TYPE_GHOST 0x81D6926
	setmoveeffect MOVE_EFFECT_PREVENT_ESCAPE
	attackcanceler
	accuracycheck BS_MOVE_MISSED 0x0
	call STANDARD_DAMAGE
	jumpiffainted BANK_TARGET BS_MOVE_FAINT
	jumpifmovehadnoeffect BS_MOVE_FAINT
	seteffecttarget
	printstring 0x8F
	waitmessage DELAY_1SECOND
	goto BS_MOVE_FAINT

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_107_Nightmare
BS_107_Nightmare:
	attackcanceler
	attackstring
	ppreduce
	jumpifsecondarystatus 0x0 STATUS2_SUBSTITUTE | STATUS2_NIGHTMARE 0x81D7DF2
	jumpifstatus BANK_TARGET STATUS_SLEEP 0x81D772C
	jumpifability BANK_TARGET ABILITY_COMATOSE 0x81D772C
	goto FAILED

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_108_Minimize
BS_108_Minimize:
	attackcanceler
	setminimize
	setstatchanger STAT_EVSN | INCREASE_2
	goto BS_BUFF_ATK_STATS + 1

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_108_Minimize
BS_108_Minimize:
	attackcanceler
	setminimize
	setstatchanger STAT_EVSN | INCREASE_2
	goto BS_BUFF_ATK_STATS + 1

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_110_RaiseAttackerSpAtk1Chance
BS_110_RaiseAttackerSpAtk1Chance:
	setmoveeffect MOVE_EFFECT_SPATK_PLUS_1 | MOVE_EFFECT_AFFECTS_USER
	goto 0x81D6926

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_111_Protect
BS_111_Protect:
	attackcanceler
	attackstring
	ppreduce
	setprotect
	attackanimation
	waitanimation
	printfromtable 0x83FE546
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_112_Spikes
BS_112_Spikes:
	attackcanceler
	attackstring
	ppreduce
	setspikes FAILED
	attackanimation
	waitanimation
	printstring 0x184
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_113_Foresight
BS_113_Foresight:
	jumpifnotmove MOVE_MIRACLEEYE 0x1D783E
	attackcanceler
	accuracycheck BS_MOVE_MISSED 0x0
	attackstring
	ppreduce
	jumpifspecialstatusflag BANK_TARGET STATUS3_MIRACLE_EYED 0x0 FAILED
	attackanimation
	waitanimation
	setspecialstatusbit BANK_TARGET STATUS3_MIRACLE_EYED
	printstring 0x96
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_114_PerishSong
BS_114_PerishSong:
	attackcanceler
	attackstring
	ppreduce
	setperishsong FAILED
	attackanimation
	waitanimation
	printstring 0xFE
	waitmessage DELAY_1SECOND
	setbyte BATTLE_SCRIPTING_BANK 0x0
	
PerishSongLoopBS:
	jumpifability BANK_SCRIPTING ABILITY_SOUNDPROOF 0x81D788C
	addbyte BATTLE_SCRIPTING_BANK 0x1
	jumpifarraynotequal BATTLE_SCRIPTING_BANK NUM_POKEMON 0x1 PerishSongLoopBS
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_115_Sandstorm
BS_115_Sandstorm:
	attackcanceler
	attackstring
	ppreduce
	setsandstorm
	goto BS_MOVE_WEATHER_CHANGE
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_116_Endure @;Free now
BS_116_Endure:
	goto 0x81D6926
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_117_Rollout
BS_117_Rollout:
	attackcanceler
	attackstring
	jumpifsecondarystatus BANK_ATTACKER STATUS2_MULTIPLETURNS RolloutCheckAccBS
	ppreduce

RolloutCheckAccBS:
	accuracycheck RolloutHitBS 0x0

RolloutHitBS:
	typecalc2
	rolloutdamagecalculation
	goto 0x81D6930

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_118_Swagger
BS_118_Swagger:
	attackcanceler
	jumpifbenindsubstitute BANK_TARGET BS_MAKE_MOVE_MISS
	accuracycheck BS_MOVE_MISSED 0x0
	attackstring
	ppreduce
	jumpifconfusedandstatmaxed STAT_ATK FAILED
	attackanimation
	waitanimation
	setstatchanger STAT_ATK | INCREASE_2
	statbuffchange STAT_TARGET | STAT_BS_PTR SwaggerTryConfuseBS
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 SwaggerTryConfuseBS
	setgraphicalstatchangevalues
	playanimation BS_TARGET, B_ANIM_STATS_CHANGE, 0x2023FD4
	printfromtable 0x83FE57C
	waitmessage DELAY_1SECOND
	
SwaggerTryConfuseBS:
	jumpifability BANK_TARGET ABILITY_OWN_TEMPO 0x81D9460 @;Ability prevents confusion
	jumpifsideaffecting BS_TARGET SIDE_SAFEGUARD 0x81D8B39 @;Protected By Safeguard
	jumpifbyte EQUALS TERRAIN_BYTE MISTY_TERRAIN Swagger_CheckGrounding
	setmoveeffect MOVE_EFFECT_CONFUSION
	seteffecttarget
	goto BS_MOVE_END

Swagger_CheckGrounding:
	jumpifgrounded BANK_TARGET ProtectedByTerrainBS
	goto 0x81D70BF

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_119_FuryCutter
BS_119_FuryCutter:
	jumpifmove MOVE_ROUND RoundBS
	jumpifmove MOVE_ECHOEDVOICE EchoedVoiceBS

FuryCutterBS:
	attackcanceler
	accuracycheck FuryCutterResetBS 0x0
	call STANDARD_DAMAGE
	seteffectwithchancetarget
	furycutterdamagecalculation
	goto BS_MOVE_FAINT
	
FuryCutterResetBS:
	furycutterdamagecalculation
	goto BS_MOVE_MISSED

RoundBS:
	attackcanceler
	callasm RoundBSFunction + 1
	goto 0x1D6927

EchoedVoiceBS:
	attackcanceler
	callasm EchoedVoiceFunc1 + 1
	goto 0x1D6927

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_120_Attract
BS_120_Attract:
	attackcanceler
	attackstring
	ppreduce
	accuracycheck FAILED 0x0
	jumpifabilitypresenttargetfield ABILITY_AROMAVEIL ProtectedByAromaVeil
	tryinfatuatetarget FAILED
	attackanimation
	waitanimation
	printstring 0x45
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_121_Return @;Free now
BS_122_Return:
	goto 0x81D6926

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_122_Present
BS_122_Present:
	attackcanceler
	accuracycheck BS_MOVE_MISSED 0x0
	attackstring
	ppreduce
	typecalc
	presentdamagecalculation

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_123_Frustration @;Free now
BS_123_Frustration:
	goto 0x81D6926

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_124_Safeguard
BS_124_Safeguard:
	attackcanceler
	attackstring
	ppreduce
	setsafeguard
	goto 0x81D7172

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_125_BurnUp
BS_125_BurnUp:
	attackcanceler
	jumpiftype BANK_ATTACKER TYPE_FIRE DoBurnUp
	goto FAILED-2

DoBurnUp:
	accuracycheck BS_MOVE_MISSED 0x0
	call STANDARD_DAMAGE
	jumpifmovehadnoeffect BS_MOVE_END
	callasm BurnUpFunc + 1
	setword BATTLE_STRING_LOADER BurnUpString
	printstring 0x184
	waitmessage DELAY_1SECOND
	goto BS_MOVE_FAINT

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_126_Magnitude
BS_126_Magnitude:
	end

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_127_BatonPass
BS_127_BatonPass:
	jumpifmove MOVE_BATONPASS 0x81D7995
	attackcanceler
	accuracycheck BS_MOVE_MISSED 0x0
	jumpifmove MOVE_PARTINGSHOT PartingShotBS
	call STANDARD_DAMAGE
	jumpifmovehadnoeffect BS_MOVE_FAINT
	seteffectwithchancetarget
	faintpokemon BANK_TARGET 0x0 0x0
	setbyte CMD49_STATE 0x0
	cmd49 BANK_TARGET 0x0
	jumpifcannotswitch BANK_ATTACKER 0x81D6957
	callasm VoltSwitchFix + 1
	jumpiffainted BANK_TARGET UTurnGiveEXPBS

UTurnSwitchBS:
	setword BATTLE_STRING_LOADER UTurnString
	printstring 0x184
	waitmessage DELAY_1SECOND
	playanimation BANK_ATTACKER ANIM_CALL_BACK_POKEMON 0x0
	setspecialstatusbit BANK_ATTACKER STATUS3_INAIR @;So the sprite stays hidden
	openpartyscreen BANK_ATTACKER 0x81D7DF2
	switchoutabilities BANK_ATTACKER
	waitstateatk
	switchhandleorder BANK_ATTACKER 0x2
	returntoball BANK_ATTACKER
	switch1 BANK_ATTACKER
	switch2 BANK_ATTACKER
	hpthresholds BANK_ATTACKER
	printstring 0x3
	switch3 BANK_ATTACKER 0x1
	waitstateatk
	switchineffects BANK_ATTACKER
	goto 0x81D6957
	
UTurnGiveEXPBS:
	setbyte EXP_STATE 0x0
	getexp 0x0
	goto UTurnSwitchBS
	
PartingShotBS:
	attackstring
	ppreduce
	callasm TryPartingShotStatDrop
	attackanimation
	waitanimation
	goto UTurnSwitchBS

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_128_Pursuit
BS_128_Pursuit:
	goto 0x81D6926

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_129_RapidSpin
.global BattleScript_DefogAdditionalEffects
BS_129_RapidSpin:
	jumpifnotmove MOVE_DEFOG 0x1D79C2
	attackcanceler
	jumpifbehindsubstitute BANK_TARGET SecondDefogCheck
	accuracycheck SecondDefogCheck 0x0
	attackstring
	ppreduce
	setstatchanger STAT_EVSN | DECREASE_1
	statbuffchange STAT_TARGET | STAT_BS_PTR SecondDefogCheck
	jumpifbyte LESSTHAN MULTISTRING_CHOOSER 0x2 DefogLoweredStat
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x3 SecondDefogCheck
	pause DELAY_HALFSECOND
	printfromtable 0x83FE588
	waitmessage DELAY_1SECOND
	goto SecondDefogCheck
	
DefogLoweredStat:
	attackanimation
	waitanimation
	setgraphicalstatchangevalues
	playanimation BANK_ATTACKER 0x1 0x2023FD4
	printfromtable 0x83FE588
	waitmessage DELAY_1SECOND
	
BattleScript_DefogAdditionalEffects:
	attackanimation @;Should only play after the Second Defog Check
	waitanimation
	jumpifweather weather_fog | weather_permament_fog RemoveFogBS
	breakfree
	goto BS_MOVE_END

RemoveFogBS:
	sethalfword WEATHER_FLAGS 0x0
	setword BATTLE_STRING_LOADER RemoveFogString
	printstring 0x184
	waitmessage DELAY_1SECOND
	breakfree
	goto BS_MOVE_END
	
SecondDefogCheck:
	callasm DefogHelperFunc + 1 @;Automatically redirects to BattleScript_DefogAdditionalEffects if applicable
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_130_Sonicboom
BS_130_Sonicboom:
	attackcanceler
	accuracycheck BS_MOVE_MISSED 0x0
	attackstring
	ppreduce
	typecalc
	bicbyte OUTCOME OUTCOME_SUPER_EFFECTIVE | OUTCOME_NOT_VERY_EFFECTIVE
	setword DAMAGE_LOC 0x0
	setbyte DAMAGE_LOC 20
	adjustsetdamage
	goto 0x81D6934

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_131_Blank
BS_131_Blank:
	goto 0x81D6926

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_132_MorningSun
BS_132_MorningSun:
	attackcanceler
	attackstring
	ppreduce
	recoverbasedonsunlight 0x81D7DD1 @;AlreadyAtFullHp
	goto 0x81D6DC6 @;Part of Recover

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_133_Blank @;Was Synthesis
BS_133_Blank:
	goto 0x81D6926

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	
.global BS_134_Blank @;Was Moonlight
BS_134_Blank:
	goto 0x81D6926
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_135_AttackerRaiseDef2Chance
BS_135_AttackerRaiseDef2Chance:
	setmoveeffect MOVE_EFFECT_DEF_PLUS_2 | MOVE_EFFECT_AFFECTS_USER
	goto 0x81D6926

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_136_RainDance
BS_136_RainDance:
	attackcanceler
	attackstring
	ppreduce
	setrain
	goto BS_MOVE_WEATHER_CHANGE

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_137_SunnyDay
BS_137_SunnyDay:
	attackcanceler
	attackstring
	ppreduce
	setsunny
	goto BS_MOVE_WEATHER_CHANGE

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_138_AttackerRaiseDef1Chance
BS_138_AttackerRaiseDef1Chance:
	setmoveeffect MOVE_EFFECT_DEF_PLUS_1 | MOVE_EFFECT_AFFECTS_USER
	goto 0x81D6926

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_139_AttackerRaiseAtk1Chance
BS_139_AttackerRaiseAtk1Chance:
	jumpifmove MOVE_FELLSTINGER FellStingerBS
	setmoveeffect MOVE_EFFECT_ATK_PLUS_1 | MOVE_EFFECT_AFFECTS_USER
	goto 0x81D6926

FellStingerBS: @;Add in Volt Switch Fix?
	attackcanceler
	accuracycheck BS_MOVE_MISSED 0x0
	call STANDARD_DAMAGE
	faintpokemon BANK_TARGET 0x0 0x0
	jumpiffainted BANK_TARGET FellStingerKill
	goto 0x81D6947

FellStingerKill:
	setbyte STAT_ANIM_PLAYED 0x0
	jumpifstat BANK_ATTACKER EQUALS STAT_ATK STAT_MAX BS_MOVE_END
	playstatchangeanimation BANK_ATTACKER STAT_ANIM_ATK STAT_ANIM_UP
	setstatchanger STAT_ATK | INCREASE_3
	statbuffchange STAT_ATTACKER | STAT_BS_PTR BS_MOVE_END
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 BS_MOVE_END
	printfromtable 0x83FE57C
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	
.global BS_140_AttackerRaiseAllStatsChance @;Add in Clangorous Soulblaze?
BS_140_AttackerRaiseAllStatsChance:
	setmoveeffect MOVE_EFFECT_ALL_STATS_UP | MOVE_EFFECT_AFFECTS_USER
	goto 0x81D6926

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_141_Blank
BS_131_Blank:
	goto 0x81D6926

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_142_BellyDrum
BS_142_BellyDrum:
	attackcanceler
	attackstring
	ppreduce
	maxattackhalvehp FAILED
	orword HIT_MARKER HITMARKER_IGNORE_SUBSTITUTE
	attackanimation
	waitanimation
	graphicalhpupdate BANK_ATTACKER
	datahpupdate BANK_ATTACKER
	printstring 0x9B
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_143_PsychUp
BS_143_PsychUp:
	jumpifnotmove MOVE_SPECTRALTHIEF 0x1D7A74

SpectralThiefBS:
	attackcanceler
	accuracycheck BS_MOVE_MISSED 0x0
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

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_144_MirrorCoat
BS_144_MirrorCoat:
	attackcanceler
	mirrorcoatdamagecalculator FAILED-2
	accuracycheck BS_MOVE_MISSED 0x0
	attackstring
	ppreduce
	damagecalc2
	adjustsetdamage
	goto 0x81D6934

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_145_SkullBash
BS_145_SkullBash:
	jumpifsecondarystatus BANK_ATTACKER STATUS2_MULTIPLETURNS TwoTurnMovesSecondTurnBS
	jumpifword ANDS HITMARKER HITMARKER_NO_ATTACKSTRING TwoTurnMovesSecondTurnBS
	setbyte TWOTURN_STRINGID, 0x2
	call TwoTurnMovesChargingTurnBS
	setstatchanger STAT_DEF | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR BS_MOVE_END
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 BS_MOVE_END
	setgraphicalstatchangevalues
	playanimation BANK_ATTACKER ANIM_STAT_BUFF 0x2023FD4
	printfromtable 0x83FE57C
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_146_Twister
BS_146_Twister:
	orword HIT_MARKER HITMARKER_IGNORE_IN_AIR
	goto BS_031_Flinch

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_147_Earthquake
BS_147_Earthquake:
	orword HIT_MARKER HITMARKER_IGNORE_UNDERGROUND
	goto 0x81D6926

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_148_FutureSight
BS_148_FutureSight:
	attackcanceler
	attackstring
	ppreduce
	setfutureattack FAILED
	attackanimation
	waitanimation
	printfromtable 0x83FE5EE
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_149_Gust
BS_149_Gust:
	orword HIT_MARKER HITMARKER_IGNORE_IN_AIR
	goto 0x81D6926

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_150_Stomp
BS_150_Stomp:
	goto BS_031_Flinch

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_151_Solarbeam
BS_151_Solarbeam:
	jumpifabilitypresent ABILITY_CLOUD_NINE, BSSolarbeamDecideTurn
	jumpifabilitypresent ABILITY_AIR_LOCK, BSSolarbeamDecideTurn
	jumpifweather WEATHER_SUN_ANY BSSolarbeamOnFirstTurn

BSSolarbeamDecideTurn:
	jumpifsecondarystatus BANK_ATTACKER STATUS2_MULTIPLETURNS TwoTurnMovesSecondTurnBS
	jumpifword ANDS HITMARKER HITMARKER_NO_ATTACKSTRING TwoTurnMovesSecondTurnBS
	setbyte TWOTURN_STRINGID, 0x1
	call TwoTurnMovesChargingTurnBS
	goto BS_MOVE_END

BSSolarbeamOnFirstTurn:
	orword HITMARKER HITMARKER_CHARGING
	setmoveeffect MOVE_EFFECT_CHARGING | MOVE_EFFECT_AFFECTS_USER
	seteffectprimary
	ppreduce
	goto TwoTurnMovesSecondTurnBS

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_152_Thunder
BS_152_Thunder:
	orword HITMARKER HITMARKER_IGNORE_IN_AIR
	jumpifmove MOVE_HURRICANE BS_076_SetConfusionChance
	setmoveefffect MOVE_EFFECT_PARALYSIS
	goto 0x81D6926	

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_153_Teleport
BS_153_Teleport:
	attackcanceler
	attackstring
	ppreduce
	jumpifbattletype BATTLE_TYPE_TRAINER FAILED
	getifcantrunfrombattle BS_ATTACKER
	jumpifbyte EQUALS BATTLE_COMMUNICATION 0x1 FAILED
	jumpifbyte EQUALS BATTLE_COMMUNICATION 0x2 0x81D8255 @;BattleScript_PrintAbilityMadeIneffective
	attackanimation
	waitanimation
	printstring 0xA0 @;STRINGID_PKMNFLEDFROMBATTLE
	waitmessage DELAY_1SECOND
	setbyte BATTLE_OUTCOME 0x5 @;Teleported
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_154_BeatUp
BS_154_BeatUp:
	@;TODO
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_155_Fly
BS_155_Fly:
	jumpifsecondarystatus BANK_ATTACKER STATUS2_MULTIPLETURNS BS_SecondTurnSemiInvulnerable
	jumpifword ANDS HITMARKER HITMARKER_NO_ATTACKSTRING BS_SecondTurnSemiInvulnerable
	jumpifmove MOVE_FLY BattleScript_FirstTurnFly
	jumpifmove MOVE_DIVE BattleScript_FirstTurnDive
	jumpifmove MOVE_BOUNCE, BattleScript_FirstTurnBounce
	jumpifmove MOVE_DIG BS_FirstTurnDig
	goto BS_FirstTurnSemiInvulnerable

BS_FirstTurnBounce:
	setbyte TWOTURN_STRINGID 0x7
	goto BS_FirstTurnSemiInvulnerable

BS_FirstTurnDive:
	setbyte TWOTURN_STRINGID 0x6
	goto BS_FirstTurnSemiInvulnerable

BS_FirstTurnFly:
	setbyte TWOTURN_STRINGID 0x4
	goto BS_FirstTurnSemiInvulnerable

BS_FirstTurnDig:
	setbyte TWOTURN_STRINGID 0x5
	
BS_FirstTurnSemiInvulnerable:
	call TwoTurnMovesChargingTurnBS
	setsemiinvulnerablebit
	goto BS_MOVE_END

BS_SecondTurnSemiInvulnerable:
	attackcanceler
	setmoveeffect MOVE_EFFECT_CHARGING
	setbyte ANIM_TURN, 0x1
	clearstatus BS_ATTACKER
	orword HIT_MARKER HITMARKER_NO_PPDEDUCT
	jumpifnotmove MOVE_BOUNCE BS_SemiInvulnerableTryHit
	setmoveeffect MOVE_EFFECT_PARALYSIS

BS_SemiInvulnerableTryHit:
	clearsemiinvulnerablebit
	goto 0x81D6927

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_156_DefenseCurl @;Check this at max Def?
BS_156_DefenseCurl:
	attackcanceler
	attackstring
	ppreduce
	setcurled
	setstatchanger STAT_DEF | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR 0x81D6BBC
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 0x81D6BC4
	attackanimation
	waitanimation
	goto 0x81D6BBC

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_157_Blank @;Was softboiled
BS_157_Blank:
	goto 0x81D6926

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_158_FakeOut
BS_158_FakeOut:
	attackcanceler
	jumpifnotfirstturn FAILED - 2
	jumpifmove MOVE_FIRSTIMPRESSION 0x81D6926
	setmoveeffect MOVE_EFFECT_FLINCH | MOVE_EFFECT_CERTAIN
	goto 0x81D6926

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_159_Uproar
BS_159_Uproar:
	attackcanceler
	accuracycheck BS_MOVE_MISSED 0x0
	setmoveeffect MOVE_EFFECT_UPROAR | MOVE_EFFECT_AFFECTS_USER
	attackstring
	jumpifsecondarystatus BANK_ATTACKER STATUS2_MULTIPLETURNS 0x81D6930
	ppreduce
	goto 0x81D6930

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_160_Stockpile
BS_160_Stockpile:
	@;TODO

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_161_SpitUp
BS_161_SpitUp:
	@;TODO

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_165_Torment
BS_165_Torment:
	attackcanceler
	accuracycheck FAILED - 2 0x0
	attackstring
	ppreduce
	jumpifabilitypresenttargetfield ABILITY_AROMAVEIL ProtectedByAromaVeil
	goto 0x81D7EBB

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_167_SetBurn
BS_167_SetBurn:
	attackcanceler
	attackstring
	ppreduce
	jumpifspecies BANK_TARGET PKMN_MINIORSHIELD FAILED
	jumpifability BANK_TARGET ABILITY_COMATOSE FAILED
	jumpifstatus BANK_TARGET STATUS_BURN 0x81D7F91 @;Already Burned
	jumpifstatus BANK_TARGET STATUS_ANY FAILED
	jumpiftype BANK_TARGET TYPE_FIRE NOEFFECT
	jumpifability BANK_TARGET ABILITY_WATERVEIL 0x81D7F77
	jumpifability BANK_TARGET ABILITY_WATERBUBBLE 0x81D7F77
	jumpifabilitypresent ABILITY_CLOUDNINE BRN_CheckFlowerVeil
	jumpifabilitypresent ABILITY_AIRLOCK BRN_CheckFlowerVeil
	jumpifability BANK_TARGET ABILITY_LEAFGUARD BRN_SunnyCheck

BRN_CheckFlowerVeil:
	jumpifabilitypresenttargetfield ABILITY_FLOWERVEIL BRN_GrassTypeCheck

BRN_CheckTerrain:
	jumpifbyte EQUALS TERRAIN_BYTE MISTY_TERRAIN BRN_CheckGrounding
	goto 0x81D7F5A

BRN_SunnyCheck:
	jumpifhalfword ANDS WEATHER_FLAGS WEATHER_SUN_ANY ProtectedByAbilityBS
	goto BRN_CheckFlowerVeil

BRN_GrassTypeCheck:
	jumpiftype BANK_TARGET TYPE_GRASS TeamProtectedByFVBS
	goto BRN_CheckTerrain

BRN_CheckGrounding:
	jumpifgrounded BANK_TARGET ProtectedByTerrainBS
	goto 0x81D7F5A

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_170_FocusPunch
BS_170_FocusPunch:
	jumpifmove MOVE_BEAKBLAST BeakBlastDoBS
	jumpifmove MOVE_SHELLTRAP ShellTrapDoBS
	goto 0x1D805C
	
BeakBlastDoBS:
	callasm ClearBeakBlastBit + 1
	goto 0x1D6926

ShellTrapDoBS: @;Do Powder fix
	attackcanceler
	counterdamagecalculator FAILED - 2
	goto 0x81D6927

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_175_Taunt
BS_175_Taunt:
	attackcanceler
	accuracycheck FAILED - 2 0x0
	attackstring
	ppreduce
	jumpifability BANK_TARGET ABILITY_OBLIVIOUS ProtectedByOblivious
	jumpifabilitypresenttargetfield ABILITY_AROMAVEIL ProtectedByAromaVeil
	goto 0x81D80D1
	
ProtectedByOblivious:
	pause DELAY_HALFSECOND
	setword BATTLE_STRING_LOADER ProtectedByObliviousString
	printstring 0x184
	waitmessage DELAY_1SECPMD
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_177_Trick
BS_177_Trick:
	jumpifnotmove MOVE_BESTOW 0x1D80F8
	attackcanceler
	jumpifbehindsubstitute BANK_TARGET FAILED - 2
	accuracycheck FAILED - 2 0x0
	attackstring
	ppreduce
	callasm BestowItem
	attackanimation
	waitanimation
	setword BATTLE_STRING_LOADER BestowString
	printstring 0x184
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_184_Recycle
BS_184_Recycle:
	attackcanceler
	jumpifnotmove MOVE_BELCH RecycleBS
	callasm BelchFunction + 1
	goto 0x1D6927
	
RecycleBS:
	jumpifcounter BANK_ATTACKER INCINERATE_COUNTER NOTEQUALS 0x0 FAILED - 2
	goto 0x81D81A1
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_187_Yawn
BS_187_Yawn:
	attackcanceler
	attackstring
	ppreduce
	jumpifspecies BANK_TARGET PKMN_MINIORSHIELD FAILED
	jumpifability BANK_TARGET ABILITY_COMATOSE 0x81D69B0
	jumpifstatus BANK_TARGET STATUS_SLEEP 0x81D69B0
	jumpifstatus BANK_TARGET STATUS_ANY FAILED
	jumpifability BANK_TARGET ABILITY_INSOMNIA 0x81D824B
	jumpifability BANK_TARGET ABILITY_VITALSPIRIT 0x81D824B
	jumpifabilitypresent ABILITY_CLOUDNINE YWN_CheckVeils
	jumpifabilitypresent ABILITY_AIRLOCK YWN_CheckVeils
	jumpifability BANK_TARGET ABILITY_LEAFGUARD YWN_SunnyCheck

YWN_CheckVeils:
	jumpifabilitypresenttargetfield ABILITY_SWEETVEIL TeamProtectedBySVBS
	jumpifabilitypresenttargetfield ABILITY_FLOWERVEIL YWN_GrassTypeCheck

YWN_CheckTerrain:
	jumpifbyte EQUALS TERRAIN_BYTE ELECTRIC_TERRAIN YWN_CheckGrounding
	jumpifbyte EQUALS TERRAIN_BYTE MISTY_TERRAIN YWN_CheckGrounding
	
YawnReturn:
	jumpifsideaffecting BANK_TARGET SIDE_SAFEGUARD 0x81D8B39 @;Protected by Safeguard
	accuracycheck 0x81D7DF2 0xFFFF
	goto 0x81D8239

YWN_SunnyCheck:
	jumpifhalfword ANDS WEATHER_FLAGS WEATHER_SUN_ANY ProtectedByAbilityBS
	goto YWN_CheckVeils

YWN_GrassTypeCheck:
	jumpiftype BANK_TARGET TYPE_GRASS TeamProtectedByFVBS
	goto YWN_CheckTerrain

YWN_CheckGrounding:
	jumpifgrounded BANK_TARGET ProtectedByTerrainBS
	goto YawnReturn

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.align 2
ProtectedByTerrainString: .byte 0xFD, 0x10, 0x00, 0xDD, 0xE7, 0xFE, 0xE4, 0xE6, 0xE3, 0xE8, 0xD9, 0xD7, 0xE8, 0xD9, 0xD8, 0x00, 0xD6, 0xED, 0x00, 0xE8, 0xDC, 0xD9, 0x00, 0xE8, 0xD9, 0xE6, 0xE6, 0xD5, 0xDD, 0xE2, 0xAB, 0xFF
ProtectedBySweetVeilString: .byte 0xFD, 0x10, 0xB4, 0xE7, 0x00, 0xE4, 0xD5, 0xE6, 0xE8, 0xED, 0x00, 0xDD, 0xE7, 0xFE, 0xE4, 0xE6, 0xE3, 0xE8, 0xD9, 0xD7, 0xE8, 0xD9, 0xD8, 0x00, 0xD6, 0xED, 0x00, 0xCD, 0xEB, 0xD9, 0xD9, 0xE8, 0x00, 0xD0, 0xD9, 0xDD, 0xE0, 0xAB, 0xFF
ProtectedByFlowerVeilString: .byte 0xFD, 0x10, 0xB4, 0xE7, 0x00, 0xE4, 0xD5, 0xE6, 0xE8, 0xED, 0x00, 0xDD, 0xE7, 0xFE, 0xE4, 0xE6, 0xE3, 0xE8, 0xD9, 0xD7, 0xE8, 0xD9, 0xD8, 0x00, 0xD6, 0xED, 0x00, 0xC0, 0xE0, 0xE3, 0xEB, 0xD9, 0xE6, 0x00, 0xD0, 0xD9, 0xDD, 0xE0, 0xAB, 0xFF
ProtectedByAbilityString: .byte 0xFD, 0x10, 0x00, 0xDD, 0xE7, 0xFE, 0xE4, 0xE6, 0xE3, 0xE8, 0xD9, 0xD7, 0xE8, 0xD9, 0xD8, 0x00, 0xD6, 0xED, 0x00, 0xFD, 0x19, 0xAB, 0xFF
ReflectTypeString: .byte 0xFD, 0x0F, 0xB4, 0xE7, 0x00, 0xE8, 0xED, 0xE4, 0xD9, 0x00, 0xD6, 0xD9, 0xD7, 0xD5, 0xE1, 0xD9, 0xFE, 0xE8, 0xDC, 0xD9, 0x00, 0xE7, 0xD5, 0xE1, 0xD9, 0x00, 0xD5, 0xE7, 0x00, 0xFD, 0x10, 0xB4, 0xE7, 0xAB, 0xFF
PurifyString: .byte 0xFD, 0x10, 0x00, 0xEB, 0xD5, 0xE7, 0x00, 0xD7, 0xE9, 0xE6, 0xD9, 0xD8, 0xFE, 0xE3, 0xDA, 0x00, 0xDD, 0xE8, 0xE7, 0x00, 0xE7, 0xE8, 0xD5, 0xE8, 0xE9, 0xE7, 0xAD, 0xFF
BecameNimbleString: .byte 0xFD, 0x0F, 0x00, 0xD6, 0xD9, 0xD7, 0xD5, 0xE1, 0xD9, 0x00, 0xE2, 0xDD, 0xE1, 0xD6, 0xE0, 0xD9, 0xAB, 0xFF
AuroraVeilSetString: .byte 0xFD, 0x14, 0x00, 0xE4, 0xE6, 0xE3, 0xE8, 0xD9, 0xD7, 0xE8, 0xE7, 0x00, 0xD5, 0xDB, 0xD5, 0xDD, 0xE2, 0xE7, 0xE8, 0x00, 0xE4, 0xDC, 0xED, 0xE7, 0xDD, 0xD7, 0xD5, 0xE0, 0xFE, 0xD5, 0xE2, 0xD8, 0x00, 0xE7, 0xE4, 0xD9, 0xD7, 0xDD, 0xD5, 0xE0, 0x00, 0xE1, 0xE3, 0xEA, 0xD9, 0xE7, 0xAB, 0xFF
ProtectedByAromaVeilString: .byte 0xFD, 0x10, 0xB4, 0xE7, 0x00, 0xE4, 0xD5, 0xE6, 0xE8, 0xED, 0x00, 0xDD, 0xE7, 0xFE, 0xE4, 0xE6, 0xE3, 0xE8, 0xD9, 0xD7, 0xE8, 0xD9, 0xD8, 0x00, 0xD6, 0xED, 0x00, 0xBB, 0xE6, 0xE3, 0xE1, 0xD5, 0x00, 0xD0, 0xD9, 0xDD, 0xE0, 0xAB, 0xFF
FreezeShockChargingString: .byte 0xFD, 0x0F, 0x00, 0xD6, 0xD9, 0xD7, 0xD5, 0xE1, 0xD9, 0xFE, 0xD7, 0xE0, 0xE3, 0xD5, 0xDF, 0xD9, 0xD8, 0x00, 0xDD, 0xE2, 0x00, 0xD5, 0x00, 0xDA, 0xE6, 0xD9, 0xD9, 0xEE, 0xDD, 0xE2, 0xDB, 0x00, 0xE0, 0xDD, 0xDB, 0xDC, 0xE8, 0xAB, 0xFF
IceBurnChargingString: .byte 0xFD, 0x0F, 0x00, 0xD6, 0xD9, 0xD7, 0xD5, 0xE1, 0xD9, 0xFE, 0xD7, 0xE0, 0xE3, 0xD5, 0xDF, 0xD9, 0xD8, 0x00, 0xDD, 0xE2, 0x00, 0xDA, 0xE6, 0xD9, 0xD9, 0xEE, 0xDD, 0xE2, 0xDB, 0x00, 0xD5, 0xDD, 0xE6, 0xAB, 0xFF
ShadowForceString: .byte 0xFD, 0x0F, 0x00, 0xEA, 0xD5, 0xE2, 0xDD, 0xE7, 0xDC, 0xD9, 0xD8, 0xFE, 0xDD, 0xE2, 0xE7, 0xE8, 0xD5, 0xE2, 0xE8, 0xE0, 0xED, 0xAB, 0xFF
LaserFocusString: .byte 0xFD, 0x0F, 0x00, 0xD7, 0xE3, 0xE2, 0xD7, 0xD9, 0xE2, 0xE8, 0xE6, 0xD5, 0xE8, 0xD9, 0xD8, 0x00, 0xDD, 0xE2, 0xE8, 0xD9, 0xE2, 0xE7, 0xD9, 0xE0, 0xED, 0xAB, 0xFF
BurnUpString: .byte 0xFD, 0x0F, 0x00, 0xD6, 0xE9, 0xE6, 0xE2, 0xD9, 0xD8, 0x00, 0xDD, 0xE8, 0xE7, 0xD9, 0xE0, 0xDA, 0x00, 0xE3, 0xE9, 0xE8, 0xAB, 0xFF
UTurnString: .byte 0x
RemoveFogString: .byte 0xFD, 0x0F, 0x00, 0xD6, 0xE0, 0xD9, 0xEB, 0x00, 0xD5, 0xEB, 0xD5, 0xED, 0xFE, 0xE8, 0xDC, 0xD9, 0x00, 0xD8, 0xD9, 0xD9, 0xE4, 0x00, 0xDA, 0xE3, 0xDB, 0x00, 0xEB, 0xDD, 0xE8, 0xDC, 0x00, 0xFD, 0x14, 0xAB, 0xFF
SpectralThiefString: .byte 0xFD, 0x0F, 0x00, 0xE7, 0xE8, 0xE3, 0xE0, 0xD9, 0xFE, 0xFD, 0x10, 0xB4, 0xE7, 0x00, 0xE7, 0xE8, 0xD5, 0xE8, 0x00, 0xD6, 0xE3, 0xE3, 0xE7, 0xE8, 0xE7, 0xAB, 0xFF
ProtectedByObliviousString: .byte 0xFD, 0x10, 0x0, 0xEB, 0xD5, 0xE7, 0x00, 0xE4, 0xE6, 0xE3, 0xE8, 0xD9, 0xD7, 0xE8, 0xD9, 0xD8, 0xFE, 0xD6, 0xED, 0x00, 0xFD, 0x19, 0xAB, 0xFF
BestowString: .byte 0xFD, 0x10, 0x00, 0xE6, 0xD9, 0xD7, 0xD9, 0xDD, 0xEA, 0xD9, 0xD8, 0x00, 0xE8, 0xDC, 0xD9, 0xFE, 0xFD, 0x16, 0x00, 0xDA, 0xE6, 0xE3, 0xE1, 0x00, 0xFD, 0x0F, 0xAB, 0xFF
*/
