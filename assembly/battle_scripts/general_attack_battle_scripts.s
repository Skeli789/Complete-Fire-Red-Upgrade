/*
general_attack_battle_scripts.s
	basic battle scripts
*/

.text
.thumb
.align 2

.include "../asm_defines.s"
.include "../battle_script_macros.s"

.global GuardSwapString
.global SpeedSwapString
.global PowerTrickString
.global PowerSwapString
.global HeartSwapString
.global PowerSplitString
.global GuardSplitString
.global PowerHerbString
.global CantUseHyperspaceFuryString
.global CantUseMoveString
.global WrongHoopaFormString

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_001_SetSleep
BS_001_SetSleep:
	attackcanceler
	attackstring @;Protean always activates!
	ppreduce
	jumpifbehindsubstitute BANK_TARGET FAILED
	trysetsleep BANK_TARGET BS_StatusMoveFail
	accuracycheck BS_MOVE_MISSED_PAUSE 0x0
	attackanimation
	waitanimation
	setmoveeffect MOVE_EFFECT_SLEEP
	seteffectprimary
	goto BS_MOVE_END
	
BS_StatusMoveFail:
	pause DELAY_HALFSECOND
	printstring 0x184
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END
	
ProtectedByTerrainBS:
	pause DELAY_HALFSECOND
	setword BATTLE_STRING_LOADER ProtectedByTerrainString
	printstring 0x184
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

.global BattleScript_TeamProtectedBySweetVeil
BattleScript_TeamProtectedBySweetVeil:
	pause 0x10
	call BattleScript_AbilityPopUp
	setword BATTLE_STRING_LOADER gText_SweetVeilProtects
	printstring 0x184
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	goto BS_MOVE_END

.global BattleScript_TeamProtectedByFlowerVeil
BattleScript_TeamProtectedByFlowerVeil:
	pause 0x10
	call BattleScript_AbilityPopUp
	setword BATTLE_STRING_LOADER gText_FlowerVeilProtects
	printstring 0x184
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	goto BS_MOVE_END

.global BattleScript_TeamProtectedByPastelVeil
BattleScript_TeamProtectedByPastelVeil:
	pause 0x10
	call BattleScript_AbilityPopUp
	setword BATTLE_STRING_LOADER gText_PastelVeilProtects
	printstring 0x184
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	goto BS_MOVE_END

.global BattleScript_ProtectedByAbility
BattleScript_ProtectedByAbility:
	pause 0x10
	copyarray BATTLE_SCRIPTING_BANK TARGET_BANK 0x1
	call BattleScript_AbilityPopUp
	printstring 0x1B @;STRINGID_ITDOESNTAFFECT
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	goto BS_MOVE_END

.global BattleScript_TargetStayedAwakeUsingAbility	
BattleScript_TargetStayedAwakeUsingAbility:
	pause 0x10
	copyarray BATTLE_SCRIPTING_BANK TARGET_BANK 0x1
	call BattleScript_AbilityPopUp
	setword BATTLE_STRING_LOADER gText_TargetStaysAwake
	printstring 0x184
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	goto BS_MOVE_END
	
.global BattleScript_PauseResultMessage
BattleScript_PauseResultMessage:
	pause DELAY_HALFSECOND
	goto 0x81D6942 @;Resultmessage

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_002_SetPoisonChance
BS_002_SetPoisonChance:
	setmoveeffect MOVE_EFFECT_POISON
	callasm TrySetPoisonPuppeterEffect
	goto BS_STANDARD_HIT

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_003_DrainHP
BS_003_DrainHP:
	attackcanceler
	accuracycheck BS_MOVE_MISSED 0x0
	jumpifmove MOVE_STRENGTHSAP StrengthSapBS
	call STANDARD_DAMAGE
	jumpiffainted BANK_ATTACKER BS_MOVE_FAINT @;Eg. Parabolic Charge activates Destiny Bond before the last target is struck
	negativedamage

DrainHPBSP2:
	orword HIT_MARKER HITMARKER_IGNORE_SUBSTITUTE
	jumpifability BANK_TARGET ABILITY_LIQUIDOOZE BattleScript_AbsorbLiquidOoze
	setbyte MULTISTRING_CHOOSER 0x0
	goto 0x81D6A2B
	
StrengthSapBS:
	jumpifbehindsubstitute BANK_TARGET FAILED_PRE
	jumpifspecies BANK_TARGET SPECIES_GHOLDENGO FAILED_PRE
	jumpifstat BANK_TARGET EQUALS STAT_ATK STAT_MIN FAILED_PRE
	attackstring
	ppreduce
	attackanimation
	waitanimation
	setgraphicalstatchangevalues
	callasm StrengthSapFunc
	setstatchanger STAT_ATK | DECREASE_1
	statbuffchange STAT_TARGET | STAT_BS_PTR DrainHPBSP2
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 DrainHPBSP2
	setgraphicalstatchangevalues
	playanimation BANK_TARGET ANIM_STAT_BUFF ANIM_ARG_1
	printfromtable gStatDownStringIds
	waitmessage DELAY_1SECOND
	goto DrainHPBSP2

BattleScript_AbsorbLiquidOoze:
	jumpifmovehadnoeffect BS_MOVE_FAINT
	copybyte BATTLE_SCRIPTING_BANK TARGET_BANK
	call BattleScript_AbilityPopUp
	manipulatedamage 0x0 @;ATK80_DMG_CHANGE_SIGN
	setbyte MULTISTRING_CHOOSER, 0x1
	graphicalhpupdate BANK_ATTACKER
	datahpupdate BANK_ATTACKER
	printfromtable 0x83FE5DC @;gLeechSeedDrainStringIds
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	faintpokemon BANK_ATTACKER 0x0 0x0
	prefaintmoveendeffects 0x0
	faintpokemonaftermove
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_004_SetBurnChance
BS_004_SetBurnChance:
	jumpifmove MOVE_BURNINGJEALOUSY BS_BurningJealousy
	jumpifmove MOVE_MATCHAGOTCHA MatchaGotchaBS
	setmoveeffect MOVE_EFFECT_BURN
	goto BS_STANDARD_HIT

BS_BurningJealousy:
	callasm TrySetBurningJealousyMoveEffect
	goto BS_STANDARD_HIT

MatchaGotchaBS:
	attackcanceler
	accuracycheck BS_MOVE_MISSED 0x0
	call STANDARD_DAMAGE
	seteffectwithchancetarget
	setmoveeffect MOVE_EFFECT_BURN
	seteffectwithchancetarget
	jumpifmovehadnoeffect BS_MOVE_FAINT
	negativedamage
	orword HIT_MARKER HITMARKER_IGNORE_SUBSTITUTE
	jumpifability BANK_TARGET ABILITY_LIQUIDOOZE BattleScript_AbsorbLiquidOoze
	graphicalhpupdate BANK_ATTACKER
	datahpupdate BANK_ATTACKER
	setword BATTLE_STRING_LOADER MatchaGotchaString
	printstring 0x184
	waitmessage DELAY_1SECOND
	goto BS_MOVE_FAINT

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_005_SetFreezeChance
BS_005_SetFreezeChance:
	setmoveeffect MOVE_EFFECT_FREEZE
	goto BS_STANDARD_HIT
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_006_SetParalyzeChance
BS_006_SetParalyzeChance:
	setmoveeffect MOVE_EFFECT_PARALYSIS
	goto BS_STANDARD_HIT

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_007_Explode
BS_007_Explode:
	attackcanceler
	attackstring
	ppreduce
	faintifabilitynotdamp
	setuserhptozero
	waitstateatk
	jumpiffainted BANK_TARGET ExplodeDeadTarget
	accuracycheck BS_MOVE_MISSED 0x0
	goto BS_HIT_FROM_DAMAGE_CALC

ExplodeDeadTarget:
	orbyte OUTCOME OUTCOME_NOT_AFFECTED
	goto BS_MOVE_END

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
	jumpifmovehadnoeffect BS_MOVE_FAINT
	negativedamage
	orword HIT_MARKER HITMARKER_IGNORE_SUBSTITUTE
	jumpifability BANK_TARGET ABILITY_LIQUIDOOZE BattleScript_AbsorbLiquidOoze
	graphicalhpupdate BANK_ATTACKER
	datahpupdate BANK_ATTACKER
	printstring 0x3C
	waitmessage DELAY_1SECOND
	goto BS_MOVE_FAINT

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_009_MirrorMove
BS_009_MirrorMove:
	attackcanceler
	jumpifmove MOVE_COPYCAT CopycatBS
	attackstringnoprotean
	pause DELAY_1SECOND
	jumptolastusedattack
	ppreduce
	orbyte OUTCOME OUTCOME_FAILED
	printstring 0xE7
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

CopycatBS:
	attackstringnoprotean
	callasm CopycatFunc
	attackanimation
	waitanimation
	setbyte ANIM_TURN 0x0
	setbyte ANIM_TARGETS_HIT 0x0
	jumptoattack 0x1

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_010_RaiseUserAtk1
BS_010_RaiseUserAtk1:
	setstatchanger STAT_ATK | INCREASE_1
	jumpifnotbattletype BATTLE_DOUBLE BS_BUFF_ATK_STATS
	jumpifmove MOVE_HOWL HowlBS @;Only difference is that it raises partner's attack in Doubles too
	goto BS_BUFF_ATK_STATS

BattleScript_CantRaiseMultipleStats:
	pause DELAY_HALFSECOND
	orbyte OUTCOME OUTCOME_FAILED
	jumpifability BANK_ATTACKER ABILITY_CONTRARY BattleScript_CantLowerMultipleStatsPrintString
BattleScript_CantRaiseMultipleStatsPrintString:
	printstring 0x19 @;STRINGID_STATSWONTINCREASE2
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

BattleScript_CantLowerMultipleStats:
	pause DELAY_HALFSECOND
	orbyte OUTCOME OUTCOME_FAILED
	jumpifability BANK_TARGET ABILITY_CONTRARY BattleScript_CantRaiseMultipleStatsPrintString
BattleScript_CantLowerMultipleStatsPrintString:
	printstring 0x15D @;STRINGID_STATSWONTDECREASE2
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

HowlBS:
	attackcanceler
	attackstring
	ppreduce
	statbuffchange STAT_ATTACKER | STAT_BS_PTR HowlTryPartnerBS
	jumpifbyte NOTEQUALS MULTISTRING_CHOOSER 0x2 HowlAttackAnimation
	pause DELAY_HALFSECOND
	setbyte ANIM_TARGETS_HIT 0x0
	goto HowlPrintAttackerString

HowlAttackAnimation:
	attackanimation
	waitanimation
	setbyte ANIM_TARGETS_HIT 0x1
	setgraphicalstatchangevalues
	playanimation BANK_ATTACKER ANIM_STAT_BUFF ANIM_ARG_1
HowlPrintAttackerString:
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND

HowlTryPartnerBS:
	callasm SetTargetPartner
	jumpifbehindsubstitute BANK_TARGET BS_MOVE_END
	jumpiffainted BANK_TARGET BS_MOVE_END
	jumpifability BANK_TARGET ABILITY_SOUNDPROOF BattleScript_ProtectedByAbility
	statbuffchange STAT_TARGET | STAT_BS_PTR BS_MOVE_END
	jumpifbyte NOTEQUALS MULTISTRING_CHOOSER 0x2 HowlSecondAttackAnimation
	pause DELAY_HALFSECOND
	swapattackerwithtarget @;So the proper string is shown
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND	
	swapattackerwithtarget
	goto BS_MOVE_END

HowlSecondAttackAnimation:
	bicbyte OUTCOME OUTCOME_NO_EFFECT
	attackanimation
	waitanimation
	setgraphicalstatchangevalues
	playanimation BANK_TARGET ANIM_STAT_BUFF ANIM_ARG_1
HowlPrintPartnerString:
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BattleScript_FlowerShieldRototillerStatBoost
.global BS_011_RaiseUserDef1
BS_011_RaiseUserDef1:
	attackcanceler
	attackstring
	ppreduce
	setstatchanger STAT_DEF | INCREASE_1
	jumpifmove MOVE_FLOWERSHIELD FlowerShieldBS
	goto 0x81D6BA1

FlowerShieldBS:
	pause DELAY_HALFSECOND
	setbyte BATTLE_COMMUNICATION 0x0
BattleScript_FlowerShieldLoop:
	flowershieldlooper 0x0 BattleScript_FlowerShieldStatBoost BattleScript_FlowerShieldDidntWork
	goto BS_MOVE_END

BattleScript_FlowerShieldStatBoost:
	setstatchanger STAT_DEF | INCREASE_1
	statbuffchange STAT_TARGET | STAT_BS_PTR BattleScript_FlowerShieldLoop
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 BattleScript_FlowerShieldCantRaiseStats
	attackanimation
	waitanimation
	setbyte ANIM_TARGETS_HIT 0x1
	setgraphicalstatchangevalues
	playanimation BANK_TARGET ANIM_STAT_BUFF ANIM_ARG_1
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	goto BattleScript_FlowerShieldLoop

BattleScript_FlowerShieldCantRaiseStats:
	swapattackerwithtarget @;So the correct string prints
	printfromtable gStatUpStringIds
	swapattackerwithtarget
	waitmessage DELAY_1SECOND
	goto BattleScript_FlowerShieldLoop

BattleScript_FlowerShieldDidntWork:
	jumpifbyte EQUALS MULTISTRING_CHOOSER 4 BattleScript_FlowerShieldDidntWork_ShowAbility
	printfromtable gFlowerShieldStringIds
	waitmessage DELAY_1SECOND
	goto BattleScript_FlowerShieldLoop

BattleScript_FlowerShieldDidntWork_ShowAbility:
	call BattleScript_AbilityPopUp
	printfromtable gFlowerShieldStringIds
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	goto BattleScript_FlowerShieldLoop

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_012_RaiseUserSpd1
BS_012_RaiseUserSpd1:
	setstatchanger STAT_SPD | INCREASE_1
	goto BS_BUFF_ATK_STATS

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_013_RaiseUserSpAtk1
BS_013_RaiseUserSpAtk1:
	setstatchanger STAT_SPATK | INCREASE_1
	goto BS_BUFF_ATK_STATS

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_014_RaiseUserSpDef1
BS_014_RaiseUserSpDef1:
	attackcanceler
	attackstring
	ppreduce
	setstatchanger STAT_SPDEF | INCREASE_1
	jumpifmove MOVE_AROMATICMIST AromaticMistBS
	goto 0x81D6BA1

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

AromaticMistBS:
	jumpifbyte NOTANDS BATTLE_TYPE BATTLE_DOUBLE FAILED
	callasm SetTargetPartner
	jumpifspecialstatusflag BANK_TARGET STATUS3_SEMI_INVULNERABLE 0x0 FAILED
	jumpiffainted BANK_TARGET FAILED
	jumpifprotectedbycraftyshield BANK_TARGET FAILED
	setbyte STAT_ANIM_PLAYED 0x0
	jumpifstatcanberaised BANK_TARGET STAT_SPDEF AromaticMistRaiseSpDef
	goto FAILED
	
AromaticMistRaiseSpDef:
	attackanimation
	waitanimation
	setstatchanger STAT_SPDEF | INCREASE_1
	statbuffchange STAT_TARGET | STAT_BS_PTR FAILED
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 FAILED
	setgraphicalstatchangevalues
	playanimation BANK_TARGET ANIM_STAT_BUFF ANIM_ARG_1
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_015_RaiseUserAcc1
BS_015_RaiseUserAcc1:
	setstatchanger STAT_ACC | INCREASE_1
	goto BS_BUFF_ATK_STATS

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_016_RaiseUserEvsn1
BS_016_RaiseUserEvsn1:
	jumpifmove MOVE_ACUPRESSURE AcupressureBS
	setstatchanger STAT_EVSN | INCREASE_1
	goto BS_BUFF_ATK_STATS

AcupressureBS:
	attackcanceler
	attackstring
	ppreduce
	callasm AcupressureFunc
	attackanimation
	waitanimation
	statbuffchange STAT_TARGET | STAT_BS_PTR BS_MOVE_END
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 BS_MOVE_END
	setgraphicalstatchangevalues
	playanimation BANK_TARGET 0x1 ANIM_ARG_1
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_017_NeverMiss
BS_017_NeverMiss:
	goto BS_STANDARD_HIT

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_018_LowerTargetAtk1
BS_018_LowerTargetAtk1:
	setstatchanger STAT_ATK | DECREASE_1
	goto 0x81D6C13

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_019_LowerTargetDef1
BS_019_LowerTargetDef1:
	setstatchanger STAT_DEF | DECREASE_1
	goto 0x81D6C13
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_020_LowerTargetSpd1
BS_020_LowerTargetSpd1:
	setstatchanger STAT_SPD | DECREASE_1
	jumpifmove MOVE_TARSHOT TarShotBS
	goto 0x81D6C13

TarShotBS:
	jumpifcounter BANK_TARGET TAR_SHOT_TIMERS NOTEQUALS 0 0x81D6C13 @;Already set
	attackcanceler
	jumpifbehindsubstitute BANK_TARGET FAILED_PRE
	accuracycheck BS_MOVE_MISSED 0x0
	attackstring
	ppreduce
	statbuffchange STAT_TARGET | STAT_BS_PTR BS_MOVE_END
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 BattleScript_PauseAndJumpToStatChangeTargetFail
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x3 BS_MOVE_END
	attackanimation
	waitanimation
	setgraphicalstatchangevalues
	playanimation BANK_TARGET ANIM_STAT_BUFF ANIM_ARG_1
	printfromtable gStatDownStringIds
	waitmessage DELAY_1SECOND
	setcounter BANK_TARGET TAR_SHOT_TIMERS 1
	setword BATTLE_STRING_LOADER gText_TarShotAffected
	printstring 0x184
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

BattleScript_PauseAndJumpToStatChangeTargetFail:
	pause 0x20
	goto 0x81D6C55

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
	goto BS_HIT_FROM_DAMAGE_CALC

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_028_Roar
BS_028_Roar:
	attackcanceler
	jumpifmove MOVE_DRAGONTAIL DragonTailBS
	jumpifmove MOVE_CIRCLETHROW DragonTailBS

RoarBS:
	attackstring @;Still activates Protean even if move fails
	ppreduce
	jumpifability BANK_TARGET ABILITY_SUCTIONCUPS BattleScript_AbilityPreventsPhasingOut
	jumpifspecialstatusflag BANK_TARGET STATUS3_ROOTED 0x0 0x81D8F27 @;BattleScript_PrintMonIsRooted
	accuracycheck FAILED 0x0
	forcerandomswitch BANK_TARGET BANK_ATTACKER FAILED

DragonTailBS:
	attackcanceler
	accuracycheck BS_MOVE_MISSED 0x0
	jumpifbehindsubstitute BANK_TARGET BS_HIT_FROM_ATTACKSTRING
	call STANDARD_DAMAGE
	jumpiffainted BANK_TARGET BS_MOVE_FAINT
	jumpifmovehadnoeffect BS_MOVE_FAINT
	jumpifdynamaxed BANK_TARGET BattleScript_DragonTailBlockedByDynamax
	jumpifspecialstatusflag BANK_TARGET STATUS3_ROOTED 0x0 0x81D8F27 @;BattleScript_PrintMonIsRooted
	jumpifability BANK_TARGET ABILITY_SUCTIONCUPS BattleScript_AbilityPreventsPhasingOutSkipFail
	setbyte CMD49_STATE 0x0
	cmd49 0x6 0x0
	setbyte FORCE_SWITCH_HELPER 0x1
	forcerandomswitch BANK_TARGET BANK_ATTACKER BattleScript_DragonTailResetForceSwitchHelper

.global BattleScript_DragonTailResetForceSwitchHelper
BattleScript_DragonTailResetForceSwitchHelper:
	setbyte FORCE_SWITCH_HELPER 0x0
	goto 0x81D6957

BattleScript_DragonTailBlockedByDynamax:
	setword BATTLE_STRING_LOADER gText_RaidBattleAttackCancel
	printstring 0x184
	waitmessage DELAY_1SECOND
	goto BS_MOVE_FAINT

BattleScript_AbilityPreventsPhasingOut:
	orbyte OUTCOME OUTCOME_NOT_AFFECTED

BattleScript_AbilityPreventsPhasingOutSkipFail:
	pause 0x10
	copyarray BATTLE_SCRIPTING_BANK TARGET_BANK 0x1
	call BattleScript_AbilityPopUp
	printstring 0xCC @STRINGID_PKMNANCHORSITSELFWITH
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_029_HitTwoToFiveTimes
BS_029_HitTwoToFiveTimes:
	goto BS_STANDARD_HIT

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_030_Conversion
BS_030_Conversion:
	attackcanceler
	jumpifmove MOVE_REFLECTTYPE ReflectTypeBS
	
ConversionBS:
	attackstring
	ppreduce
	changetypestoenemyattacktype FAILED
	attackanimation
	waitanimation
	printstring 0x49 @;STRINGID_PKMNCHANGEDTYPE
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

ReflectTypeBS:
	jumpifbehindsubstitute BANK_TARGET FAILED_PRE
	accuracycheck BS_MOVE_MISSED 0x0
	changetypestoenemyattacktype FAILED_PRE
	attackstring
	ppreduce	
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
	jumpifmove MOVE_UPPERHAND UpperHandBS
	goto BS_STANDARD_HIT
	
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

UpperHandBS:
	callasm TryUpperHand
	goto BS_STANDARD_HIT

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_032_Recover
.global RecoverBS
BS_032_Recover:
	attackcanceler
	jumpifmove MOVE_PURIFY PurifyBS
	attackstring
	ppreduce
	jumpifmove MOVE_ROOST RoostBS
	jumpifmove MOVE_LIFEDEW LifeDewBS
	jumpifmove MOVE_JUNGLEHEALING JungleHealingBS
	jumpifmove MOVE_LUNARBLESSING LunarBlessingBS

RecoverBS:
	setdamageasrestorehalfmaxhp 0x81D7DD1 BANK_ATTACKER @;BattleScript_AlreadyAtFullHp
	
RecoverAnimBS:
	attackanimation
	waitanimation
	
RecoverHealthUpdateBS:
	orword HIT_MARKER HITMARKER_IGNORE_SUBSTITUTE
	graphicalhpupdate BANK_ATTACKER
	datahpupdate BANK_ATTACKER
	printstring 0x4B @;STRINGID_PKMNREGAINEDHEALTH
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

RoostBS:
	setdamageasrestorehalfmaxhp 0x81D7DD1 0x1 @;BattleScript_AlreadyAtFullHp
	callasm SetRoostFunc+1
	goto RecoverAnimBS

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

PurifyBS:
	jumpifstatus BANK_TARGET STATUS_ANY PurifyHeal
	goto FAILED_PRE

PurifyHeal:
	attackstringnoprotean
	ppreduce
	cureprimarystatus BANK_TARGET FAILED
	tryactivateprotean
	attackanimation
	waitanimation
	setword BATTLE_STRING_LOADER PurifyString
	printstring 0x184
	waitmessage DELAY_1SECOND
	refreshhpbar BANK_TARGET
	setdamageasrestorehalfmaxhp 0x81D7DD1 0x1
	playanimation BANK_ATTACKER ANIM_HEALING_SPARKLES 0x0
	goto RecoverHealthUpdateBS

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

LifeDewBS:
	callasm TryFailLifeDew
	attackanimation
	waitanimation
	setdamageasrestorehalfmaxhp LifeDewAttackerFullHealthBS BANK_ATTACKER
	orword HIT_MARKER HITMARKER_IGNORE_SUBSTITUTE
	graphicalhpupdate BANK_ATTACKER
	datahpupdate BANK_ATTACKER
	printstring 0x4B @;STRINGID_PKMNREGAINEDHEALTH
	waitmessage DELAY_1SECOND

LifeDewRestorePartnerHPBS:
	callasm SetTargetPartner
	jumpiffainted BANK_TARGET BS_MOVE_END
	jumpifcounter BANK_TARGET HEAL_BLOCK_TIMERS NOTEQUALS 0x0 BattleScript_NoHealTargetAfterHealBlock
	accuracycheck LifeDewMissedPartnerBS 0x0
	setdamageasrestorehalfmaxhp LifeDewPartnerFullHealthBS BANK_TARGET
	orword HIT_MARKER HITMARKER_IGNORE_SUBSTITUTE
	graphicalhpupdate BANK_TARGET
	datahpupdate BANK_TARGET
	printstring 0x4B @;STRINGID_PKMNREGAINEDHEALTH
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

LifeDewAttackerFullHealthBS:
	printstring 0x4C @;STRINGID_PKMNHPFULL
	waitmessage DELAY_1SECOND
	goto LifeDewRestorePartnerHPBS

LifeDewMissedPartnerBS:
	orbyte OUTCOME OUTCOME_MISSED
	goto BS_MOVE_MISSED_PAUSE + 4

.global BattleScript_LifeDewFail
BattleScript_LifeDewFail:
	pause DELAY_HALFSECOND
	printstring 0x4C @;STRINGID_PKMNHPFULL
	waitmessage DELAY_1SECOND
	callasm SetTargetPartner
	jumpiffainted BANK_TARGET BS_MOVE_END
LifeDewPartnerFullHealthBS:
	printstring 0x4C @;STRINGID_PKMNHPFULL
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

JungleHealingBS:
	callasm TryFailJungleHealing
	attackanimation
	waitanimation
	setdamageasrestorehalfmaxhp JungleHealingAttackerFullHealthBS BANK_ATTACKER
	orword HIT_MARKER HITMARKER_IGNORE_SUBSTITUTE
	graphicalhpupdate BANK_ATTACKER
	datahpupdate BANK_ATTACKER
	printstring 0x4B @;STRINGID_PKMNREGAINEDHEALTH
	waitmessage DELAY_1SECOND
	goto JungleHealingTryClearAttackerStatusBS

JungleHealingAttackerFullHealthBS:
	printstring 0x4C @;STRINGID_PKMNHPFULL
	waitmessage DELAY_1SECOND

JungleHealingTryClearAttackerStatusBS:
	cureprimarystatus BANK_ATTACKER JungleHealingRestorePartnerHPBS
	refreshhpbar BANK_ATTACKER
	setword BATTLE_STRING_LOADER PurifyString
	printstring 0x184
	waitmessage DELAY_1SECOND

JungleHealingRestorePartnerHPBS:
	callasm SetTargetPartner
	jumpiffainted BANK_TARGET BS_MOVE_END
	jumpifcounter BANK_TARGET HEAL_BLOCK_TIMERS NOTEQUALS 0x0 BattleScript_NoHealPartnerAfterHealBlock_JungleHealing
	accuracycheck JungleHealingMissedPartnerBS 0x0
	setdamageasrestorehalfmaxhp JungleHealingPartnerFullHealthBS BANK_TARGET
	orword HIT_MARKER HITMARKER_IGNORE_SUBSTITUTE
	graphicalhpupdate BANK_TARGET
	datahpupdate BANK_TARGET
	printstring 0x4B @;STRINGID_PKMNREGAINEDHEALTH
	waitmessage DELAY_1SECOND
	goto JungleHealingTryClearPartnerStatusBS

JungleHealingPartnerFullHealthBS:
	printstring 0x4C @;STRINGID_PKMNHPFULL
	waitmessage DELAY_1SECOND

JungleHealingTryClearPartnerStatusBS:
	cureprimarystatus BANK_TARGET BS_MOVE_END
	refreshhpbar BANK_TARGET
	setword BATTLE_STRING_LOADER PurifyString
	printstring 0x184
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

JungleHealingMissedPartnerBS:
	orbyte OUTCOME OUTCOME_MISSED
	goto BS_MOVE_MISSED_PAUSE + 4

BattleScript_NoHealPartnerAfterHealBlock_JungleHealing:
	pause DELAY_HALFSECOND
	setword BATTLE_STRING_LOADER HealBlockTargetString
	printstring 0x184
	waitmessage DELAY_1SECOND
	goto JungleHealingTryClearPartnerStatusBS

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

LunarBlessingBS:
	callasm TryFailLunarBlessing
	attackanimation
	waitanimation
	setdamageasrestorehalfmaxhp LunarBlessingFullHealthBS BANK_ATTACKER
	orword HIT_MARKER HITMARKER_IGNORE_SUBSTITUTE
	graphicalhpupdate BANK_ATTACKER
	datahpupdate BANK_ATTACKER
	printstring 0x4B @;STRINGID_PKMNREGAINEDHEALTH
	waitmessage DELAY_1SECOND
	goto LunarBlessingTryClearStatusBS

LunarBlessingFullHealthBS:
	printstring 0x4C @;STRINGID_PKMNHPFULL
	waitmessage DELAY_1SECOND

LunarBlessingTryClearStatusBS:
	cureprimarystatus BANK_ATTACKER LunarBlessingRaiseEvasionBS
	refreshhpbar BANK_ATTACKER
	setword BATTLE_STRING_LOADER PurifyString
	printstring 0x184
	waitmessage DELAY_1SECOND

LunarBlessingRaiseEvasionBS:
	setmoveeffect MOVE_EFFECT_EVS_PLUS_1 | MOVE_EFFECT_AFFECTS_USER
	seteffectprimary
	goto BS_MOVE_END

.global BattleScript_LunarBlessingFail
BattleScript_LunarBlessingFail:
	pause DELAY_HALFSECOND
	printstring 0x4C @;STRINGID_PKMNHPFULL
	waitmessage DELAY_1SECOND
	setstatchanger STAT_EVASION | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN, BS_MOVE_END @;Won't work, but needed to buffer the correct string
	printfromtable gStatUpStringIds
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_033_SetBadPoison
BS_033_SetBadPoison:
	attackcanceler
	attackstring @;Protean always activates!
	ppreduce
	jumpifbehindsubstitute BANK_TARGET FAILED
	trysetpoison BANK_TARGET BS_StatusMoveFail
	accuracycheck BS_MOVE_MISSED_PAUSE 0x0
	attackanimation
	waitanimation
	setmoveeffect MOVE_EFFECT_TOXIC
	seteffectprimary
	callasm TrySetPoisonPuppeterEffect
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_034_PayDay
BS_034_PayDay:
	setmoveeffect MOVE_EFFECT_PAYDAY
	goto BS_STANDARD_HIT

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_035_LightScreen
BS_035_LightScreen:
	attackcanceler
	attackstring
	ppreduce
	setlightscreen
	goto ReflectBS
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_036_TriAttack
BS_036_TriAttack:
	setmoveeffect MOVE_EFFECT_TRI_ATTACK
	goto BS_STANDARD_HIT

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_037_SetRest
BS_037_SetRest:
	attackcanceler
	jumpifterrainandgrounded ELECTRIC_TERRAIN BANK_ATTACKER FAILED_PRE
	jumpifterrainandgrounded MISTY_TERRAIN BANK_ATTACKER FAILED_PRE
	attackstring
	ppreduce
	jumpifstatus BANK_ATTACKER, STATUS_SLEEP, 0x81D6EC2 @;BattleScript_RestIsAlreadyAsleep
	setrest 0x0
	pause DELAY_HALFSECOND
	printfromtable 0x83FE562 @;gRestUsedStringIds
	waitmessage DELAY_1SECOND
	refreshhpbar BANK_ATTACKER
	waitstateatk
	goto 0x81D7DB7 @;BattleScript_PresentHealTarget

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_038_OHK0
BS_038_OHK0:
	attackcanceler
	accuracycheck FAILED 0xFFFF
	jumpifability BANK_TARGET, ABILITY_STURDY, BattleScript_SturdyPreventsOHKO
	typecalc2
	tryko OHKOMoveFail
	attackstring
	ppreduce
	callasm FailMoveIfAura
	jumpifmovehadnoeffect BS_HIT_FROM_ATTACKANIMATION
	trysetdestinybondtohappen
	goto BS_HIT_FROM_ATTACKANIMATION

OHKOMoveFail:
	attackstring
	ppreduce
	pause DELAY_HALFSECOND
	printfromtable 0x83FE5D4
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_039_RazorWind
BS_039_RazorWind:
	jumpifsecondarystatus BANK_ATTACKER STATUS2_MULTIPLETURNS TwoTurnMovesSecondTurnBS
	jumpifword ANDS HIT_MARKER HITMARKER_NO_ATTACKSTRING TwoTurnMovesSecondTurnBS
	setbyte TWOTURN_STRINGID 0x0
	call BattleScript_FirstChargingTurn
	call BattleScript_CheckPowerHerb
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_040_SuperFang
BS_040_SuperFang:
	attackcanceler
	accuracycheck BS_MOVE_MISSED 0x0
	attackstring
	ppreduce
	typecalc2
	bicbyte OUTCOME OUTCOME_SUPER_EFFECTIVE | OUTCOME_NOT_VERY_EFFECTIVE
	gethalfcurrentenemyhp
	goto BS_HIT_FROM_ATTACKANIMATION

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_041_DragonRage
BS_041_DragonRage:
	attackcanceler
	accuracycheck BS_MOVE_MISSED 0x0
	attackstring
	ppreduce
	typecalc2
	bicbyte OUTCOME OUTCOME_SUPER_EFFECTIVE | OUTCOME_NOT_VERY_EFFECTIVE
	setword DAMAGE_LOC 0x0
	setbyte DAMAGE_LOC 40
	adjustsetdamage
	goto BS_HIT_FROM_ATTACKANIMATION

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_042_TrapAndDamage
BS_042_TrapAndDamage:
	setmoveeffect MOVE_EFFECT_WRAP
	goto BS_STANDARD_HIT
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_043_HighCritRatio
BS_043_HighCritRatio:
	goto BS_STANDARD_HIT
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_044_HitTwice
BS_044_HitTwice:
	goto BS_STANDARD_HIT

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_045_HighJumpKick
BS_045_HighJumpKick:
	jumpifability BANK_ATTACKER ABILITY_MAGICGUARD BS_STANDARD_HIT
	attackcanceler
	accuracycheck HighJumpKickMiss 0x0
	typecalc2
	bicbyte OUTCOME OUTCOME_SUPER_EFFECTIVE | OUTCOME_NOT_VERY_EFFECTIVE
	jumpifmovehadnoeffect HighJumpKickMiss
	jumpifmove MOVE_AXEKICK AxeKick_BS
	goto BS_HIT_FROM_ATTACKSTRING

HighJumpKickMiss:
	attackstring
	ppreduce
	pause DELAY_HALFSECOND
	resultmessage
	waitmessage DELAY_1SECOND
	prefaintmoveendeffects 0x0
	printstring 0x60
	waitmessage DELAY_1SECOND
	bicbyte OUTCOME OUTCOME_MISSED
	orword HIT_MARKER HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_NON_ATTACK_DMG
	setdamagetobankhealthfraction BANK_ATTACKER 2 0x0 @;50 % of Base Max HP
	graphicalhpupdate BANK_ATTACKER
	datahpupdate BANK_ATTACKER
	faintpokemon BANK_ATTACKER 0x0 0x0
	orbyte OUTCOME OUTCOME_MISSED
	goto BS_MOVE_END

AxeKick_BS:
	setmoveeffect MOVE_EFFECT_CONFUSION
	goto BS_HIT_FROM_ATTACKSTRING

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
	jumpifsecondarystatus BANK_ATTACKER STATUS2_PUMPEDUP FAILED_PRE
	attackstring
	ppreduce
	setincreasedcriticalchance
	attackanimation
	waitanimation
	printfromtable 0x83FE5B0
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_048_25PercentRecoil
BS_048_25PercentRecoil:
	goto BS_STANDARD_HIT

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_049_SetConfusion
BS_049_SetConfusion:
	attackcanceler
	attackstring @;Protean always activates!
	ppreduce
	jumpifbehindsubstitute BANK_TARGET FAILED
	canconfuse BANK_TARGET BS_StatusMoveFail
	accuracycheck BS_MOVE_MISSED_PAUSE 0x0
	attackanimation
	waitanimation
	setmoveeffect MOVE_EFFECT_CONFUSION
	seteffectprimary
	resultmessage
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_050_RaiseUserAtk2
BS_050_RaiseUserAtk2:
	setstatchanger STAT_ATK | INCREASE_2
	goto BS_BUFF_ATK_STATS

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_051_RaiseUserDef2
BS_051_RaiseUserDef2:
	setstatchanger STAT_DEF | INCREASE_2
	jumpifmove MOVE_COTTONGUARD CottonGuardBS
	jumpifmove MOVE_STUFFCHEEKS StuffCheeksBS
	goto BS_BUFF_ATK_STATS
	
CottonGuardBS:
	setstatchanger STAT_DEF | INCREASE_3
	goto BS_BUFF_ATK_STATS

StuffCheeksBS:
	attackcanceler
	callasm FailIfAttackerIsntHoldingEdibleBerry
	attackstring
	ppreduce
	attackanimation
	waitanimation
	callasm SetTempIgnoreAnimations @;So the berry animation doesn't play
	setmoveeffect MOVE_EFFECT_EAT_BERRY
	seteffectprimary
	callasm SetTempIgnoreAnimations @;So the attack animation doesn't play again
	setstatchanger STAT_DEF | INCREASE_2
	goto BS_BUFF_ATK_STATS + 3

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_052_RaiseUserSpd2
BS_052_RaiseUserSpd2:
	setstatchanger STAT_SPD | INCREASE_2
	jumpifnotmove MOVE_AUTOTOMIZE BS_BUFF_ATK_STATS

AutotomizeBS:
	attackcanceler
	attackstring
	ppreduce
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN 0x81D6BCC
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 0x81D6BB2
	attackanimation
	waitanimation
	setgraphicalstatchangevalues
	playanimation BANK_ATTACKER ANIM_STAT_BUFF ANIM_ARG_1
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	callasm IncreaseNimbleCounter
	setword BATTLE_STRING_LOADER BecameNimbleString
	printstring 0x184
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_053_RaiseUserSpAtk2
BS_053_RaiseUserSpAtk2:
	setstatchanger STAT_SPATK | INCREASE_2
	jumpifnotmove MOVE_TAILGLOW BS_BUFF_ATK_STATS
	setstatchanger STAT_SPATK | INCREASE_3
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
	jumpifbehindsubstitute BANK_TARGET FAILED_PRE
	jumpifspecies BANK_TARGET SPECIES_GHOLDENGO FAILED_PRE
	jumpifsecondarystatus BANK_ATTACKER STATUS2_TRANSFORMED FAILED_PRE
	jumpifspecialstatusflag BANK_TARGET STATUS3_ILLUSION 0x0 FAILED_PRE
	attackstring
	ppreduce
	callasm FailTransformIfAura
	transformdataexecution
	attackanimation
	waitanimation
	printfromtable 0x83FE5B4
	waitmessage DELAY_1SECOND
	callasm TryRemovePrimalWeatherAfterTransformation
	call 0x81D92DC @;Try to revert Cherrim and Castform
	goto BS_MOVE_END

.global BattleScript_TransformFailedOnAura
BattleScript_TransformFailedOnAura:
	pause 0x20
	orbyte OUTCOME, OUTCOME_FAILED
	setword BATTLE_STRING_LOADER gText_TransformFailsOnWildBosses
	printstring 0x184
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_058_LowerTargetAtk2
BS_058_LowerTargetAtk2:
	setstatchanger STAT_ATK | DECREASE_2
	goto 0x81D6C13

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_059_LowerTargetDef2
BS_059_LowerTargetDef2:
	jumpifmove MOVE_SPICYEXTRACT SpicyExtractBS
	setstatchanger STAT_DEF | DECREASE_2
	goto 0x81D6C13

SpicyExtractBS:
    jumpifbehindsubstitute BANK_TARGET FAILED_PRE
    attackcanceler
    attackstring
    ppreduce
    setbyte ANIM_TARGETS_HIT 0x0
    setbyte STAT_ANIM_PLAYED 0x0
    jumpifstat BANK_TARGET LESSTHAN STAT_ATK STAT_MAX SpicyExtract_RaiseAtk
    jumpifstat BANK_TARGET GREATERTHAN STAT_DEF STAT_MIN SpicyExtract_RaiseAtk
    goto FAILED_PRE

SpicyExtract_RaiseAtk:
    attackanimation
    waitanimation
    setbyte ANIM_TARGETS_HIT 0x1 @;Prevent the attack animation from playing again
    playstatchangeanimation BANK_TARGET, STAT_ANIM_ATK, STAT_ANIM_UP | STAT_ANIM_IGNORE_ABILITIES | STAT_ANIM_BY_TWO
    setstatchanger STAT_ATK | INCREASE_2
    statbuffchange STAT_TARGET | STAT_BS_PTR | STAT_CERTAIN SpicyExtract_DropDef
    jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 SpicyExtract_DropDef
    printfromtable 0x83FE57C
    waitmessage DELAY_1SECOND

SpicyExtract_DropDef:
    playstatchangeanimation BANK_TARGET, STAT_ANIM_DEF, STAT_ANIM_DOWN | STAT_ANIM_IGNORE_ABILITIES | STAT_ANIM_BY_TWO
    setstatchanger STAT_DEF | DECREASE_2
    statbuffchange STAT_TARGET | STAT_BS_PTR | STAT_CERTAIN BS_MOVE_END
    jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 BS_MOVE_END
    printfromtable 0x83FE57C
    waitmessage DELAY_1SECOND
    goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_060_LowerTargetSpd2
BS_060_LowerTargetSpd2:
	setstatchanger STAT_SPD | DECREASE_2
	jumpifmove MOVE_COTTONSPORE CottonSporeBS
	goto 0x81D6C13

CottonSporeBS:
	attackcanceler
	jumpifbehindsubstitute BANK_TARGET FAILED_PRE
	typecalc2
	jumpifmovehadnoeffect CottonSporeNoEffect
	goto 0x81D6C1E

CottonSporeNoEffect:
	attackstring
	ppreduce
	goto NOEFFECT
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_061_LowerTargetSpAtk2
BS_061_LowerTargetSpAtk2:
	setstatchanger STAT_SPATK | DECREASE_2
	jumpifnotmove MOVE_CAPTIVATE 0x81D6C13
	
CaptivateBS:
	attackcanceler
	jumpifbehindsubstitute BANK_TARGET FAILED_PRE
	accuracycheck BS_MOVE_MISSED 0x0
	attackstring
	ppreduce
	jumpifability BANK_TARGET ABILITY_OBLIVIOUS BattleScript_ObliviousPrevents
	callasm CaptivateFunc
	goto 0x81D6C27

.global BattleScript_ObliviousPrevents
BattleScript_ObliviousPrevents:
	pause 0x10
	copyarray BATTLE_SCRIPTING_BANK TARGET_BANK 0x1
	call BattleScript_AbilityPopUp
	orbyte OUTCOME OUTCOME_NOT_AFFECTED
	printstring 0x1B @;STRINGID_ITDOESNTAFFECT
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	goto BS_MOVE_END

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
	attackcanceler
	jumpifmove MOVE_AURORAVEIL AuroraVeilBS
	attackstring
	ppreduce
	setreflect
	
ReflectBS:
	attackanimation
	waitanimation
	printfromtable gReflectLightScreenStringIds
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

AuroraVeilBS:
	jumpifcounter BANK_ATTACKER AURORA_VEIL_TIMERS NOTEQUALS 0 FAILED_PRE
	attackstring
	ppreduce
	callasm SetAuroraVeil
	goto ReflectBS

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_066_SetPoison
BS_066_SetPoison:
	attackcanceler
	attackstring @;Protean always activates!
	ppreduce
	jumpifbehindsubstitute BANK_TARGET FAILED
	jumpifmove MOVE_TOXICTHREAD ToxicThreadDo

PoisonChecks:
	trysetpoison BANK_TARGET BS_StatusMoveFail
	accuracycheck BS_MOVE_MISSED_PAUSE 0x0
	attackanimation
	waitanimation
	setmoveeffect MOVE_EFFECT_POISON
	seteffectprimary
	callasm TrySetPoisonPuppeterEffect
	goto BS_MOVE_END

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

ToxicThreadDo:
	accuracycheck BS_MOVE_MISSED_PAUSE 0x0
	setstatchanger STAT_SPD | DECREASE_1
	statbuffchange STAT_TARGET | STAT_BS_PTR ToxicThreadPSN
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 ToxicThreadPSN

@ToxicThreadWork
	attackanimation
	waitanimation
	setgraphicalstatchangevalues
	playanimation BANK_TARGET ANIM_STAT_BUFF ANIM_ARG_1
	printfromtable gStatDownStringIds
	waitmessage DELAY_1SECOND
	setmoveeffect MOVE_EFFECT_POISON
	seteffectprimary
	goto BS_MOVE_END

ToxicThreadPSN:
	bicbyte OUTCOME OUTCOME_MISSED
	goto PoisonChecks

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_067_SetParalyze
BS_067_SetParalyze:
	attackcanceler
	attackstring @;Protean always activates!
	ppreduce
	jumpifbehindsubstitute BANK_TARGET FAILED
	trysetparalysis BANK_TARGET BS_StatusMoveFail
	accuracycheck BS_MOVE_MISSED_PAUSE 0x0
	attackanimation
	waitanimation
	setmoveeffect MOVE_EFFECT_PARALYSIS
	seteffectprimary
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_068_LowerTargetAtk1Chance
BS_068_LowerTargetAtk1Chance:
	setmoveeffect MOVE_EFFECT_ATK_MINUS_1
	goto BS_STANDARD_HIT

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_069_LowerTargetDef1Chance
BS_069_LowerTargetDef1Chance:
	setmoveeffect MOVE_EFFECT_DEF_MINUS_1
	jumpifmove MOVE_TRIPLEARROWS TripleArrowsBS
	goto BS_STANDARD_HIT

TripleArrowsBS:
	attackcanceler
	accuracycheck BS_MOVE_MISSED 0x0
	call STANDARD_DAMAGE
	seteffectwithchancetarget
	jumpifsecondarystatus BANK_ATTACKER STATUS2_PUMPEDUP BS_MOVE_FAINT
	jumpifability BANK_ATTACKER ABILITY_SHEERFORCE BS_MOVE_FAINT
	jumpifmovehadnoeffect BS_MOVE_FAINT
	setincreasedcriticalchance
	printfromtable 0x83FE5B0
	waitmessage DELAY_1SECOND
	goto BS_MOVE_FAINT

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_070_LowerTargetSpd1Chance
BS_070_LowerTargetSpd1Chance:
	setmoveeffect MOVE_EFFECT_SPD_MINUS_1
	goto BS_STANDARD_HIT

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_071_LowerTargetSpAtk1Chance
BS_071_LowerTargetSpAtk1Chance:
	setmoveeffect MOVE_EFFECT_SP_ATK_MINUS_1
	goto BS_STANDARD_HIT

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_072_LowerTargetSpDef1Chance
BS_072_LowerTargetSpDef1Chance:
	setmoveeffect MOVE_EFFECT_SP_DEF_MINUS_1
	goto BS_STANDARD_HIT

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_073_LowerTargetAcc1Chance
BS_073_LowerTargetAcc1Chance:
	setmoveeffect MOVE_EFFECT_ACC_MINUS_1
	goto BS_STANDARD_HIT

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_074_LowerTargetEvsn1Chance
BS_074_LowerTargetEvsn1Chance:
	setmoveeffect MOVE_EFFECT_EVS_MINUS_1
	goto BS_STANDARD_HIT

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_075_SkyAttack
BS_075_SkyAttack:
	jumpifsecondarystatus BANK_ATTACKER STATUS2_MULTIPLETURNS TwoTurnMovesSecondTurnBS
	jumpifword ANDS HIT_MARKER HITMARKER_NO_ATTACKSTRING TwoTurnMovesSecondTurnBS
	setbyte TWOTURN_STRINGID 0x3
	call BattleScript_FirstChargingTurn
	call BattleScript_CheckPowerHerb
	goto BS_MOVE_END

BattleScript_FirstChargingTurn:
	attackcanceler
	printstring 0x130 @;Blank String
	ppreduce
	attackanimation
	waitanimation
	jumpifmove MOVE_FREEZESHOCK PrintFreezeShockString
	jumpifmove MOVE_ICEBURN PrintIceBurnString
	jumpifmove MOVE_SHADOWFORCE PrintShadowForceString
	jumpifmove MOVE_PHANTOMFORCE PrintShadowForceString
	jumpifmove MOVE_METEORBEAM PrintMeteorBeamString
	copyarray MULTISTRING_CHOOSER TWOTURN_STRINGID 0x1
	printfromtable 0x83FE590
	return

PrintFreezeShockString:
	setword BATTLE_STRING_LOADER FreezeShockChargingString
	printstring 0x184
	waitmessage DELAY_1SECOND
	return

PrintIceBurnString:
	setword BATTLE_STRING_LOADER IceBurnChargingString
	printstring 0x184
	waitmessage DELAY_1SECOND
	return
	
PrintShadowForceString:
	setword BATTLE_STRING_LOADER ShadowForceString
	printstring 0x184
	waitmessage DELAY_1SECOND
	return

PrintMeteorBeamString:
	setword BATTLE_STRING_LOADER gText_MeteorBeamCharge
	printstring 0x184
	waitmessage DELAY_1SECOND
	return

BattleScript_CheckPowerHerb:
	waitmessage DELAY_1SECOND
	jumpifhelditemeffect BANK_ATTACKER ITEM_EFFECT_POWER_HERB PowerHerbChargeBS
	jumpifraidboss BANK_ATTACKER TwoTurnMovesRaidBossSkipCharge
	orword HIT_MARKER HITMARKER_CHARGING
	setmoveeffect MOVE_EFFECT_CHARGING | MOVE_EFFECT_AFFECTS_USER
	seteffectprimary
	return

PowerHerbChargeBS:
	playanimation BANK_ATTACKER ANIM_ITEM_USE 0x0
	setword BATTLE_STRING_LOADER PowerHerbString
	printstring 0x184
	waitmessage DELAY_1SECOND
	removeitem BANK_ATTACKER

TwoTurnMovesRaidBossSkipCharge:
	setbyte ANIM_TARGETS_HIT 0x0
	setbyte ANIM_TURN 0x1
	callasm ClearCalculatedSpreadMoveData @;So the damage can be calculated
	goto PowerHerbSkipBS

TwoTurnMovesSecondTurnBS:
	attackcanceler
	setmoveeffect MOVE_EFFECT_CHARGING
	setbyte ANIM_TURN 0x1
	clearstatus BANK_ATTACKER

PowerHerbSkipBS:
	orword HIT_MARKER HITMARKER_NO_PPDEDUCT
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
	jumpifmove MOVE_ALLURINGVOICE AlluringVoice_BS
	setmoveeffect MOVE_EFFECT_CONFUSION
	goto BS_STANDARD_HIT

AlluringVoice_BS:
	callasm TrySetAlluringVoiceMoveEffect
	goto BS_STANDARD_HIT

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_077_LowerTargetSpDef2Chance @;Was Twineedle
BS_077_LowerTargetSpDef2Chance:
	setmoveeffect MOVE_EFFECT_SP_DEF_MINUS_2
	goto BS_STANDARD_HIT

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_078_Blank @;Was VitalThrow
BS_078_Blank:
	goto BS_STANDARD_HIT

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_079_Substitute
BS_079_Substitute:
	jumpifmove MOVE_SHEDTAIL ShedTailBS
	attackcanceler
	ppreduce
	attackstring
	waitstateatk
	jumpifbehindsubstitute BANK_ATTACKER 0x81D7321
	callasm FailSubstituteInShadowShieldBattle
	setsubstituteeffect
	jumpifbyte NOTEQUALS MULTISTRING_CHOOSER 0x1 0x81D730E
	pause DELAY_HALFSECOND
	goto 0x81D7314

.global BattleScript_ShadowShieldBlocksSubstitute
BattleScript_ShadowShieldBlocksSubstitute:
	pause DELAY_HALFSECOND
	setword BATTLE_STRING_LOADER gText_ShadowShieldBattleBlocksSubstitute
	printstring 0x184
	waitmessage DELAY_1SECOND
	orbyte OUTCOME OUTCOME_FAILED
	goto BS_MOVE_END

ShedTailBS:
    ppreduce
    attackstring
	callasm FailShedTailIfLowHP
    waitstateatk
    jumpifcannotswitch BANK_ATTACKER | ATK4F_DONT_CHECK_STATUSES FAILED_PRE
    jumpifbehindsubstitute BANK_ATTACKER 0x81D7321
    setsubstituteeffect
    jumpifbyte NOTEQUALS MULTISTRING_CHOOSER 0x1 BattleScript_SubstituteAnim
    pause DELAY_HALFSECOND
    goto BattleScript_SubstituteStringFail

BattleScript_SubstituteAnim:
    playanimation BANK_TARGET ANIM_SUBSTITUTE2 0x0
    waitanimation
	setdamagetobankhealthfraction BANK_ATTACKER 2 0x0 @;50 % of Base Max HP
    graphicalhpupdate BANK_ATTACKER
    datahpupdate BANK_ATTACKER

BattleScript_SubstituteString:
	setword BATTLE_STRING_LOADER SubstituteString
	printstring 0x184
	waitmessage DELAY_1SECOND

ShedTailSwitchOutBS:
    jumpifcannotswitch BANK_ATTACKER | ATK4F_DONT_CHECK_STATUSES FAILED_PRE
    attackanimation
    waitanimation
    callasm SetBatonPassSwitchingBit
    copybyte SWITCHING_BANK USER_BANK
    copyarray CURRENT_MOVE BACKUP_HWORD 2
    callasm ClearAttackerDidDamageOnce
    callasm ClearTargetStatFellThisTurn @;So Eject Pack doesn't activate
    openpartyscreen BANK_SWITCHING FAILED
    switchoutabilities BANK_SWITCHING
    waitstateatk
    switchhandleorder BANK_SWITCHING 0x2
    returntoball BANK_SWITCHING
    callasm TryRemovePrimalWeatherSwitchingBank
    getswitchedmondata BANK_SWITCHING
    switchindataupdate BANK_SWITCHING
    hpthresholds BANK_SWITCHING
    printstring 0x3
    switchinanim BANK_SWITCHING 0x1
    waitstateatk
    switchineffects BANK_SWITCHING
    callasm ClearBatonPassSwitchingBit
    copyarray CURRENT_MOVE BACKUP_HWORD 2
    playanimation BANK_SWITCHING ANIM_SUBSTITUTE2 0x0
    waitanimation 
    callasm setsubstituteonattacker + 1
    goto BS_MOVE_END

BattleScript_SubstituteStringFail:
    setword BATTLE_STRING_LOADER SubstituteFailString
	printstring 0x184
	waitmessage DELAY_1SECOND
    goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_080_Recharge
BS_080_Recharge:
	attackcanceler
	accuracycheck BS_MOVE_MISSED 0x0
	setmoveeffect MOVE_EFFECT_RECHARGE | MOVE_EFFECT_CERTAIN | MOVE_EFFECT_AFFECTS_USER
	goto BS_HIT_FROM_ATTACKSTRING

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_081_Rage
BS_081_Rage:
	attackcanceler
	accuracycheck 0x81D7367 0x0
	setmoveeffect MOVE_EFFECT_RAGE
	call STANDARD_DAMAGE
	jumpifmovehadnoeffect BS_MOVE_FAINT
	seteffectprimary
	goto BS_MOVE_FAINT
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_082_Mimic
BS_082_Mimic:
	attackcanceler
	jumpifbehindsubstitute BANK_TARGET FAILED_PRE
	accuracycheck FAILED_PRE 0x0
	attackstringnoprotean
	ppreduce
	copyattack FAILED
	tryactivateprotean
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
	jumpifbehindsubstitute BANK_TARGET FAILED_PRE
	jumpifspecies BANK_TARGET SPECIES_GHOLDENGO FAILED_PRE
	attackstring
	ppreduce
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
	accuracycheck BS_MOVE_MISSED 0x0
	attackstringnoprotean
	ppreduce
	jumpifabilitypresenttargetfield ABILITY_AROMAVEIL BattleScript_ProtectedByAromaVeil
	callasm FailMoveIfAura
	disablelastusedattack BANK_TARGET FAILED
	tryactivateprotean
	attackanimation
	waitanimation
	printstring 0x84 @;STRINGID_PKMNMOVEWASDISABLED
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

BattleScript_ProtectedByAromaVeil:
	pause 0x10
	call BattleScript_AbilityPopUp
	setword BATTLE_STRING_LOADER gText_AromaVeilProtects
	printstring 0x184
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_087_SeismicToss
BS_087_SeismicToss:
	attackcanceler
	accuracycheck BS_MOVE_MISSED 0x0
	attackstring
	ppreduce
	typecalc2
	bicbyte OUTCOME OUTCOME_SUPER_EFFECTIVE | OUTCOME_NOT_VERY_EFFECTIVE
	nightshadedamageeffect
	adjustsetdamage
	goto BS_HIT_FROM_ATTACKANIMATION

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_088_Psywave
BS_088_Psywave:
	attackcanceler
	accuracycheck BS_MOVE_MISSED 0x0
	attackstring
	ppreduce
	typecalc2
	bicbyte OUTCOME OUTCOME_SUPER_EFFECTIVE | OUTCOME_NOT_VERY_EFFECTIVE
	psywavedamageeffect
	adjustsetdamage
	goto BS_HIT_FROM_ATTACKANIMATION

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_089_Counter
BS_089_Counter:
	attackcanceler
	jumpifmove MOVE_METALBURST MetalBurstBS
	jumpifmove MOVE_COMEUPPANCE MetalBurstBS
	
CounterBS:
	counterdamagecalculator FAILED_PRE
	accuracycheck BS_MOVE_MISSED 0x0
	attackstring
	ppreduce
	typecalc2
	bicbyte OUTCOME OUTCOME_SUPER_EFFECTIVE | OUTCOME_NOT_VERY_EFFECTIVE
	adjustsetdamage
	goto BS_HIT_FROM_ATTACKANIMATION

MetalBurstBS:
	accuracycheck BS_MOVE_MISSED 0x0
	attackstring
	ppreduce
	callasm MetalBurstDamageCalculator
	typecalc2
	bicbyte OUTCOME OUTCOME_SUPER_EFFECTIVE | OUTCOME_NOT_VERY_EFFECTIVE
	adjustsetdamage
	goto BS_HIT_FROM_ATTACKANIMATION
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_090_Encore
BS_090_Encore:
	attackcanceler	
	accuracycheck BS_MOVE_MISSED 0x0
	attackstringnoprotean
	ppreduce
	jumpifabilitypresenttargetfield ABILITY_AROMAVEIL BattleScript_ProtectedByAromaVeil
	callasm FailMoveIfAura
	setencore FAILED
	tryactivateprotean
	attackanimation
	waitanimation
	printstring 0x87
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_091_PainSplit
BS_091_PainSplit:
	attackcanceler
	accuracycheck FAILED_PRE 0x0
	attackstringnoprotean
	ppreduce
	painsplitdamagecalculator FAILED
	tryactivateprotean
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

.global BS_092_Snore
BS_092_Snore:
	attackcanceler
	jumpifstatus BANK_ATTACKER STATUS_SLEEP BattleScript_SnoreIsAsleep
	jumpifability BANK_ATTACKER ABILITY_COMATOSE BattleScript_SnoreIsAsleep
	goto FAILED_PRE

BattleScript_SnoreIsAsleep:
	jumpifmove MOVE_SLEEPTALK BattleScript_DoSnore
	printstring 0x6B @;STRINGID_PKMNFASTASLEEP
	waitmessage DELAY_1SECOND
	statusanimation BANK_ATTACKER

BattleScript_DoSnore:
	attackstring
	ppreduce
	accuracycheck BS_MOVE_MISSED 0x0
	setmoveeffect MOVE_EFFECT_FLINCH
	goto BS_HIT_FROM_DAMAGE_CALC

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_094_LockOn
BS_094_LockOn:
	attackcanceler
	attackstring
	ppreduce
	jumpifmove MOVE_LASERFOCUS LaserFocusBS
	
LockOnBS:
	jumpifbehindsubstitute BANK_TARGET FAILED_PRE
	accuracycheck FAILED_PRE 0x0
	attackstring
	ppreduce
	setalwayshitflag
	attackanimation
	waitanimation
	printstring 0x89
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

LaserFocusBS:
	callasm SetLaserFocusTimer
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
	jumpifbehindsubstitute BANK_TARGET FAILED_PRE
	attackstringnoprotean
	ppreduce
	copymovepermanently FAILED
	tryactivateprotean
	attackanimation
	waitanimation
	printstring 0x8A
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_096_RaiseAttackerSpd1Chance
BS_096_RaiseAttackerSpd1Chance:
	setmoveeffect MOVE_EFFECT_SPD_PLUS_1 | MOVE_EFFECT_AFFECTS_USER
	jumpifmove MOVE_AURAWHEEL AuraWheelBS
	goto BS_STANDARD_HIT

AuraWheelBS:
	attackcanceler
	callasm TryFailAuraWheel
	goto BS_STANDARD_HIT + 1

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_097_SleepTalk
BS_097_SleepTalk:
	attackcanceler
	jumpifstatus BANK_ATTACKER STATUS_SLEEP BattleScript_SleepTalkIsAsleep
	jumpifability BANK_ATTACKER ABILITY_COMATOSE BattleScript_SleepTalkIsAsleep
	goto FAILED_PRE

BattleScript_SleepTalkIsAsleep:
	printstring 0x6B @;STRINGID_PKMNFASTASLEEP
	waitmessage DELAY_1SECOND
	statusanimation BANK_ATTACKER
	attackstring
	ppreduce
	orword HIT_MARKER HITMARKER_NO_PPDEDUCT
	trychoosesleeptalkmove BattleScript_SleepTalkUsingMove
	pause DELAY_1SECOND
	goto FAILED

BattleScript_SleepTalkUsingMove:
	attackanimation
	waitanimation
	setbyte ANIM_TURN, 0x0
	setbyte ANIM_TARGETS_HIT, 0x0
	jumptoattack TRUE

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_098_DestinyBond
BS_098_DestinyBond:
	attackcanceler
	attackstringnoprotean
	ppreduce
	callasm FailMoveIfAura
	destinybondeffect
	tryactivateprotean
	attackanimation
	waitanimation
	printstring 0x8B
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_099_Flail @;Was Flail
BS_099_Flail:
	goto BS_STANDARD_HIT

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_100_Spite
BS_100_Spite:
	attackcanceler
	jumpifmove MOVE_EERIESPELL EerieSpellBS
	jumpifspecies BANK_TARGET SPECIES_GHOLDENGO FAILED_PRE
	accuracycheck FAILED_PRE 0x0
	attackstringnoprotean
	ppreduce
	reducepprandom FAILED
	tryactivateprotean
	attackanimation
	waitanimation
	printstring 0x8D
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

EerieSpellBS:
	accuracycheck BS_MOVE_MISSED 0x0
	call STANDARD_DAMAGE
	jumpifmovehadnoeffect BS_MOVE_FAINT
	jumpifbehindsubstitute BANK_TARGET BS_MOVE_FAINT
	reducepprandom EerieSpellPostEffect
	printstring 0x8D
	waitmessage DELAY_1SECOND
EerieSpellPostEffect:
	prefaintmoveendeffects 0x0
	faintpokemonaftermove
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_101_FalseSwipe
BS_101_FalseSwipe:
	goto BS_STANDARD_HIT

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BattleScript_SapSipperAromatherapy
.global BS_102_HealBell
BS_102_HealBell:
	attackcanceler
	attackstring
	ppreduce
	healpartystatus
	waitstateatk
	attackanimation
	waitanimation
	printfromtable 0x83FE5E4
	waitmessage DELAY_1SECOND
	jumpifmove MOVE_AROMATHERAPY AromatherapySapSipperCheckBS
	jumpifnotmove MOVE_HEALBELL BattleScript_PartyHealEnd
	jumpifbyte NOTANDS MULTISTRING_CHOOSER 0x1 BattleScript_CheckHealBellMon2Unaffected
	copybyte FORM_COUNTER BATTLE_SCRIPTING_BANK
	copybyte BATTLE_SCRIPTING_BANK USER_BANK
	call BattleScript_AbilityPopUp
	printstring 0x15E @;STRINGID_PKMNSXBLOCKSY2
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	copybyte BATTLE_SCRIPTING_BANK FORM_COUNTER

BattleScript_CheckHealBellMon2Unaffected:
	jumpifbyte NOTANDS MULTISTRING_CHOOSER 0x2 BattleScript_PartyHealEnd
	call BattleScript_AbilityPopUp
	printstring 0x15E @;STRINGID_PKMNSXBLOCKSY2
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert

BattleScript_PartyHealEnd:
	refreshhpbar 0x4
	waitstateatk
	goto BS_MOVE_END

AromatherapySapSipperCheckBS:
	callasm TryActivatePartnerSapSipper
	goto BattleScript_PartyHealEnd
	
BattleScript_SapSipperAromatherapy:
	statbuffchange STAT_TARGET | STAT_BS_PTR SapSipperReturnBS
	call BattleScript_AbilityPopUp
	playanimation BANK_TARGET ANIM_STAT_BUFF ANIM_ARG_1
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
SapSipperReturnBS:
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_103_PriorityMove
BS_103_PriorityMove:
	goto BS_STANDARD_HIT

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_104_TripleKick
BS_104_TripleKick:
	attackcanceler
	accuracycheck BS_MOVE_MISSED 0x0
	attackstring
	ppreduce
	jumpifmove MOVE_TRIPLEAXEL BS_TripleAxel
	addbyte TRIPLE_KICK_POWER 10
	goto BS_HIT_FROM_DAMAGE_CALC

BS_TripleAxel:
	addbyte TRIPLE_KICK_POWER 20
	goto BS_HIT_FROM_DAMAGE_CALC

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_105_StealItem
BS_105_StealItem:
	setmoveeffect MOVE_EFFECT_STEAL_ITEM
	goto BS_STANDARD_HIT

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_106_MeanLook
BS_106_MeanLook:
	jumpifmove MOVE_ANCHORSHOT DamageAndTrapBS
	jumpifmove MOVE_SPIRITSHACKLE DamageAndTrapBS
	jumpifmove MOVE_THOUSANDWAVES DamageAndTrapBS
	jumpifmove MOVE_JAWLOCK JawLockBS
	attackcanceler
	accuracycheck FAILED_PRE 0x0
	jumpiftype BANK_TARGET TYPE_GHOST FAILED_PRE
	jumpifsecondarystatus BANK_TARGET STATUS2_TRAPPED | STATUS2_SUBSTITUTE FAILED_PRE
	attackstring
	ppreduce
	attackanimation
	waitanimation
	setmoveeffect MOVE_EFFECT_PREVENT_ESCAPE
	seteffectprimary
	printstring 0x8F @;STRINGID_TARGETCANTESCAPENOW
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END
	
DamageAndTrapBS:
	jumpifsecondarystatus BANK_TARGET STATUS2_TRAPPED | STATUS2_SUBSTITUTE BS_STANDARD_HIT
	jumpiftype BANK_TARGET TYPE_GHOST BS_STANDARD_HIT
	setmoveeffect MOVE_EFFECT_PREVENT_ESCAPE
	attackcanceler
	accuracycheck BS_MOVE_MISSED 0x0
	call STANDARD_DAMAGE
	jumpiffainted BANK_TARGET BS_MOVE_FAINT
	jumpifmovehadnoeffect BS_MOVE_FAINT
	seteffectprimary
	printstring 0x8F
	waitmessage DELAY_1SECOND
	goto BS_MOVE_FAINT

JawLockBS:
	attackcanceler
	accuracycheck BS_MOVE_MISSED 0x0
	call STANDARD_DAMAGE
	jumpiffainted BANK_TARGET BS_MOVE_FAINT
	jumpifmovehadnoeffect BS_MOVE_FAINT
	@;Make both banks lock each other
	setmoveeffect MOVE_EFFECT_PREVENT_ESCAPE
	seteffectprimary
	swapattackerwithtarget
	setmoveeffect MOVE_EFFECT_PREVENT_ESCAPE
	seteffectprimary
	swapattackerwithtarget
	setword BATTLE_STRING_LOADER gText_JawLockNoEscape
	printstring 0x184
	waitmessage DELAY_1SECOND
	goto BS_MOVE_FAINT

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_107_Nightmare
BS_107_Nightmare:
	attackcanceler
	jumpifsecondarystatus 0x0 STATUS2_SUBSTITUTE | STATUS2_NIGHTMARE FAILED_PRE
	attackstring
	ppreduce
	jumpifstatus BANK_TARGET STATUS_SLEEP SetNightmareBS
	jumpifability BANK_TARGET ABILITY_COMATOSE SetNightmareBS
	goto NOEFFECT

SetNightmareBS:
	attackanimation
	waitanimation
	setmoveeffect MOVE_EFFECT_NIGHTMARE
	seteffectprimary
	printstring 0x90 @;STRINGID_PKMNFELLINTONIGHTMARE
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_108_Minimize
BS_108_Minimize:
	attackcanceler
	setminimize
	setstatchanger STAT_EVSN | INCREASE_2
	goto BS_BUFF_ATK_STATS + 1

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_109_Curse
BS_109_Curse:
	attackcanceler
	attackstring @;So Protean activates first
	jumpiftype2 BANK_ATTACKER, TYPE_GHOST, BattleScript_GhostCurse
	goto 0x81D7756 @;PP Reduce

BattleScript_GhostCurse:
	jumpifspecies BANK_TARGET SPECIES_GHOLDENGO FAILED_PRE
	jumpifbytenotequal USER_BANK, TARGET_BANK, BattleScript_DoGhostCurse
	getmovetarget BANK_ATTACKER

BattleScript_DoGhostCurse:
	goto 0x81D77D8 @;PP Reduce

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_110_RaiseAttackerSpAtk1Chance
BS_110_RaiseAttackerSpAtk1Chance:
	setmoveeffect MOVE_EFFECT_SP_ATK_PLUS_1 | MOVE_EFFECT_AFFECTS_USER
	goto BS_STANDARD_HIT

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_111_Protect
BS_111_Protect:
	attackcanceler
	attackstringnoprotean
	ppreduce
	setprotect
	tryactivateprotean
	attackanimation
	waitanimation
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x3 ProtectPrintCustomMessage
	printfromtable 0x83FE546
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END
	
ProtectPrintCustomMessage:
	printstring 0x184
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_112_Spikes
BS_112_Spikes:
	attackcanceler
	attackstringnoprotean
	ppreduce
	setspikes FAILED
	tryactivateprotean
	attackanimation
	waitanimation
	printstring 0x184
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_113_Foresight
BS_113_Foresight:
	jumpifmove MOVE_MIRACLEEYE MiracleEyeBS

ForesightBS:
	attackcanceler
	accuracycheck BS_MOVE_MISSED 0x0
	attackstring
	ppreduce
	setforesight
	attackanimation
	waitanimation
	printstring 0x96
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END
	
MiracleEyeBS:
	attackcanceler
	accuracycheck BS_MOVE_MISSED 0x0
	jumpifspecialstatusflag BANK_TARGET STATUS3_MIRACLE_EYED 0x0 FAILED_PRE
	attackstring
	ppreduce
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
	attackstringnoprotean
	ppreduce
	callasm FailMoveIfAura
	setperishsong FAILED
	tryactivateprotean
	attackanimation
	waitanimation
	printstring 0xFE
	waitmessage DELAY_1SECOND
	setbyte BATTLE_SCRIPTING_BANK 0x0
	
PerishSongLoopBS:
	jumpifability BANK_SCRIPTING ABILITY_SOUNDPROOF BattleScript_PerishSongNotAffected
	
BattleScript_PerishSongLoopIncrement:
	addbyte BATTLE_SCRIPTING_BANK 0x1
	jumpifarraynotequal BATTLE_SCRIPTING_BANK NUM_POKEMON 0x1 PerishSongLoopBS
	goto BS_MOVE_END
	
BattleScript_PerishSongNotAffected:
	copyarray TARGET_BANK BATTLE_SCRIPTING_BANK 0x1
	call BattleScript_AbilityPopUp
	printstring 0x1B @;STRINGID_ITDOESNTAFFECT
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	goto BattleScript_PerishSongLoopIncrement

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_115_Sandstorm
BS_115_Sandstorm:
	attackcanceler
	attackstringnoprotean
	ppreduce
	jumpifweather WEATHER_SANDSTORM_ANY, SandstormSkipPrimalWeatherCheck @;Fails normally
	tryblockweatherwithprimalweather
SandstormSkipPrimalWeatherCheck:
	setsandstorm
	tryactivateprotean
	goto BS_MOVE_WEATHER_CHANGE

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_ExtremelyHarshSunlightWasNotLessened:
	call BattleScript_ExtremelyHarshSunlightWasNotLessenedRet
	goto BS_MOVE_END

BattleScript_NoReliefFromHeavyRain:
	call BattleScript_NoReliefFromHeavyRainRet
	goto BS_MOVE_END

BattleScript_MysteriousAirCurrentBlowsOn:
	call BattleScript_MysteriousAirCurrentBlowsOnRet
	goto BS_MOVE_END

BattleScript_ViciousSandstormRefusesToLetUp:
	call BattleScript_ViciousSandstormRefusesToLetUpRet
	goto BS_MOVE_END

.global BattleScript_ExtremelyHarshSunlightWasNotLessenedRet
.global BattleScript_ExtremelyHarshSunlightWasNotLessenedRetSkipPause
BattleScript_ExtremelyHarshSunlightWasNotLessenedRet:
	pause DELAY_HALFSECOND
BattleScript_ExtremelyHarshSunlightWasNotLessenedRetSkipPause:
	setword BATTLE_STRING_LOADER gText_HarshSunStopsWeatherChange
	printstring 0x184
	waitmessage DELAY_1SECOND
	return

.global BattleScript_NoReliefFromHeavyRainRet
.global BattleScript_NoReliefFromHeavyRainRetSkipPause
BattleScript_NoReliefFromHeavyRainRet:
	pause DELAY_HALFSECOND
BattleScript_NoReliefFromHeavyRainRetSkipPause:
	setword BATTLE_STRING_LOADER gText_HeavyRainStopsWeatherChange
	printstring 0x184
	waitmessage DELAY_1SECOND
	return

.global BattleScript_MysteriousAirCurrentBlowsOnRet
.global BattleScript_MysteriousAirCurrentBlowsOnRetSkipPause
BattleScript_MysteriousAirCurrentBlowsOnRet:
	pause DELAY_HALFSECOND
BattleScript_MysteriousAirCurrentBlowsOnRetSkipPause:
	setword BATTLE_STRING_LOADER gText_AirCurrentStopsWeatherChange
	printstring 0x184
	waitmessage DELAY_1SECOND
	return

.global BattleScript_ViciousSandstormRefusesToLetUpRet
.global BattleScript_ViciousSandstormRefusesToLetUpRetSkipPause
BattleScript_ViciousSandstormRefusesToLetUpRet:
	pause DELAY_HALFSECOND
BattleScript_ViciousSandstormRefusesToLetUpRetSkipPause:
	setword BATTLE_STRING_LOADER gText_ViciousSandstormStopsWeatherChange
	printstring 0x184
	waitmessage DELAY_1SECOND
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BattleScript_EnduredMsg
.global BS_116_Blank @;Was Endure
BS_116_Blank:
	goto BS_STANDARD_HIT

BattleScript_EnduredMsg:
	printstring 0x99 @;STRINGID_PKMNENDUREDHIT
	waitmessage 0x40
	return

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
	goto BS_HIT_FROM_DAMAGE_CALC

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_118_Swagger
BS_118_Swagger:
	attackcanceler
	jumpifbehindsubstitute BANK_TARGET BS_MAKE_MOVE_MISS
	accuracycheck BS_MOVE_MISSED 0x0
	jumpifconfusedandstatmaxed STAT_ATK FAILED_PRE
	attackstring
	ppreduce
	attackanimation
	waitanimation
	setstatchanger STAT_ATK | INCREASE_2
	statbuffchange STAT_TARGET | STAT_BS_PTR SwaggerTryConfuseBS
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 SwaggerTryConfuseBS
	setgraphicalstatchangevalues
	playanimation BANK_TARGET, ANIM_STAT_BUFF, ANIM_ARG_1
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	
SwaggerTryConfuseBS:
	canconfuse BANK_TARGET BS_StatusMoveFail
	setmoveeffect MOVE_EFFECT_CONFUSION
	seteffectprimary
	goto BS_MOVE_END

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
	callasm RoundBSFunction
	accuracycheck MoveMissedSetRoundBit 0x0
	call STANDARD_DAMAGE
	callasm SetRoundUsed
	goto 0x81D6946
	
MoveMissedSetRoundBit:
	callasm SetRoundUsed
	goto BS_MOVE_MISSED

EchoedVoiceBS:
	attackcanceler
	callasm EchoedVoiceFunc
	goto 0x81D6927

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_120_Attract
BS_120_Attract:
	attackcanceler
	accuracycheck FAILED_PRE 0x0
	attackstring
	ppreduce
	jumpifability BANK_TARGET ABILITY_OBLIVIOUS BattleScript_ObliviousPrevents
	jumpifabilitypresenttargetfield ABILITY_AROMAVEIL BattleScript_ProtectedByAromaVeil
	tryinfatuatebank BANK_TARGET NOEFFECT
	attackanimation
	waitanimation
	printstring 0x45
	waitmessage DELAY_1SECOND
	jumpifsecondarystatus BANK_ATTACKER STATUS2_INLOVE BS_MOVE_END
	jumpifability BANK_ATTACKER ABILITY_OBLIVIOUS BS_MOVE_END
	jumpifabilitypresentattackerfield ABILITY_AROMAVEIL BS_MOVE_END
	jumpifhelditemeffect BANK_TARGET ITEM_EFFECT_DESTINY_KNOT AttractDestinyKnot
	goto BS_MOVE_END
	
AttractDestinyKnot:
	tryinfatuatebank BANK_ATTACKER BS_MOVE_END
	playanimation BANK_TARGET ANIM_ITEM_USE 0x0
	status2animation BANK_ATTACKER STATUS2_INLOVE
	copyarray BATTLE_SCRIPTING_BANK USER_BANK 0x1
	setword BATTLE_STRING_LOADER DestinyKnotString
	printstring 0x184
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_121_Blank @;Was Return
BS_121_Blank:
	goto BS_STANDARD_HIT

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_122_Present
BS_122_Present:
	attackcanceler
	accuracycheck BS_MOVE_MISSED 0x0
	attackstring
	ppreduce
	typecalc2
	presentdamagecalculation

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_123_Blank @;Was Frustration
BS_123_Blank:
	goto BS_STANDARD_HIT

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_124_Safeguard
BS_124_Safeguard:
	attackcanceler
	attackstring
	ppreduce
	setsafeguard
	attackanimation
	waitanimation
	printfromtable gSafeguardStringIds
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_125_BurnUp
BS_125_BurnUp:
	attackcanceler
	jumpiftype BANK_ATTACKER TYPE_FIRE DoBurnUp
	jumpiftype BANK_ATTACKER TYPE_ELECTRIC DoDoubleShock
	goto FAILED_PRE

DoBurnUp:
	accuracycheck BS_MOVE_MISSED 0x0
	call STANDARD_DAMAGE
	seteffectwithchancetarget
	prefaintmoveendeffects 0x0
	faintpokemonaftermove
	jumpifmovehadnoeffect BS_MOVE_END
	setbyte CMD49_STATE 0x0
	cmd49 0x0 0x0
	jumpiffainted BANK_ATTACKER BurnUpEnd	
	callasm BurnUpFunc
	setword BATTLE_STRING_LOADER BurnUpString
	printstring 0x184
	waitmessage DELAY_1SECOND

BurnUpEnd:
	end

DoDoubleShock:
	accuracycheck BS_MOVE_MISSED 0x0
	call STANDARD_DAMAGE
	seteffectwithchancetarget
	prefaintmoveendeffects 0x0
	faintpokemonaftermove
	jumpifmovehadnoeffect BS_MOVE_END
	setbyte CMD49_STATE 0x0
	cmd49 0x0 0x0
	jumpiffainted BANK_ATTACKER DoubleShockEnd	
	callasm DoubleShockFunc
	setword BATTLE_STRING_LOADER DoubleShockString
	printstring 0x184
	waitmessage DELAY_1SECOND

DoubleShockEnd:
	end

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_126_Magnitude
BS_126_Magnitude:
	attackcanceler
	attackstring
	ppreduce
	magnitudedamagecalculation
	pause DELAY_HALFSECOND
	printstring 0x9A @;STRINGID_MAGNITUDESTRENGTH
	waitmessage DELAY_1SECOND
	accuracycheck BS_MOVE_MISSED 0x0
	goto BS_HIT_FROM_DAMAGE_CALC

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_127_BatonPass
BS_127_BatonPass:
	attackcanceler
	copyarray BACKUP_HWORD CURRENT_MOVE 2
	jumpifnotmove MOVE_BATONPASS UTurnBS
	jumpifcannotswitch BANK_ATTACKER | ATK4F_DONT_CHECK_STATUSES FAILED_PRE
	attackstring
	ppreduce
	attackanimation
	waitanimation
	callasm SetBatonPassSwitchingBit
	copybyte SWITCHING_BANK USER_BANK

BatonPassSwitchOutBS:
	copyarray CURRENT_MOVE BACKUP_HWORD 2
	callasm ClearAttackerDidDamageOnce
	callasm ClearTargetStatFellThisTurn @;So Eject Pack doesn't activate
	openpartyscreen BANK_SWITCHING FAILED
	callasm ClearInPivotingMove
	switchoutabilities BANK_SWITCHING
	waitstateatk
	switchhandleorder BANK_SWITCHING 0x2
	returntoball BANK_SWITCHING
	callasm TryRemovePrimalWeatherSwitchingBank
	getswitchedmondata BANK_SWITCHING
	switchindataupdate BANK_SWITCHING
	hpthresholds BANK_SWITCHING
	printstring 0x3
	switchinanim BANK_SWITCHING 0x1
	waitstateatk
	switchineffects BANK_SWITCHING
	callasm ClearBatonPassSwitchingBit
	copyarray CURRENT_MOVE BACKUP_HWORD 2
	jumpifmove MOVE_PARTINGSHOT PartingShotEndBS
	jumpifnotmove MOVE_BATONPASS 0x81D6957 @;U-Turn & Volt Switch
	goto BS_MOVE_END

UTurnBS:
	accuracycheck BS_MOVE_MISSED 0x0
	jumpifmove MOVE_PARTINGSHOT PartingShotBS
	call STANDARD_DAMAGE
	jumpifmovehadnoeffect BS_MOVE_FAINT
	seteffectwithchancetarget
	prefaintmoveendeffects 0x0
	faintpokemonaftermove
	setbyte CMD49_STATE 0x0
	cmd49 BANK_TARGET 0x0
	
UTurnCheckSwitchBS:
	copybyte SWITCHING_BANK USER_BANK
	jumpifcannotswitch BANK_SWITCHING | ATK4F_DONT_CHECK_STATUSES 0x81D6957
	jumpifnoviablemonsleft BANK_TARGET 0x81D6957
	jumpiffainted BANK_SWITCHING 0x81D6957
	jumpiffainted BANK_TARGET UTurnGiveEXPBS

UTurnSwitchBS:
	copybyte BATTLE_SCRIPTING_BANK USER_BANK
	setword BATTLE_STRING_LOADER UTurnString
	printstring 0x184
	waitmessage DELAY_1SECOND
	goto BattleScript_SwitchOutAttackCheckPursuit
	
UTurnGiveEXPBS:
	setbyte EXP_STATE 0x0
	getexp 0x0
	callasm SetSkipCertainSwitchInAbilities
	goto UTurnSwitchBS
	
BattleScript_SwitchOutAttackCheckPursuit:
	jumpiffainted BANK_TARGET BattleScript_UTurnSwitchOutAnim
	
	setbyte DMG_MULTIPLIER 0x2
	jumpifbattletype BATTLE_DOUBLE BattleScript_PursuitSwitchDmgSetMultihitUTurn
	setbyte MULTI_HIT_COUNTER 0x1
	goto BattleScript_PursuitSwitchDmgLoopUTurn

BattleScript_PursuitSwitchDmgSetMultihitUTurn:
	setbyte MULTI_HIT_COUNTER 0x2

BattleScript_PursuitSwitchDmgLoopUTurn:
	jumpifnopursuitswitchdmg BattleScript_DoSwitchOutUTurn
	swapattackerwithtarget
	trysetdestinybondtohappen
	call BattleScript_PursuitDmgOnSwitchOutUTurn
	swapattackerwithtarget

BattleScript_DoSwitchOutUTurn:
	decrementmultihit BattleScript_PursuitSwitchDmgLoopUTurn
	
BattleScript_UTurnSwitchOutAnim:
	playanimation BANK_SWITCHING ANIM_CALL_BACK_POKEMON 0x0
	callasm MakeSwitchingBankInvisible @;So the sprite stays hidden
	callasm SetInPivotingMove
	goto BatonPassSwitchOutBS
	
BattleScript_PursuitDmgOnSwitchOutUTurn:
	pause DELAY_HALFSECOND
	orword HIT_MARKER, HITMARKER_OBEYS
	setbyte FORCE_SWITCH_HELPER 0x0
	callasm MoldBreakerRemoveAbilitiesOnForceSwitchIn
	call STANDARD_DAMAGE
	prefaintmoveendeffects 0x0
	faintpokemonaftermove
	setbyte CMD49_STATE, 0x0
	cmd49 0x4 0x0
	various BANK_TARGET 4
	jumpifbyte EQUALS BATTLE_COMMUNICATION 0x0 BattleScript_PursuitDmgOnSwitchOutUTurnRet
	setbyte EXP_STATE 0x0
	getexp BANK_TARGET
	goto BS_MOVE_END
	
BattleScript_PursuitDmgOnSwitchOutUTurnRet:
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	
PartingShotBS:
	attackstring
	ppreduce
	setbyte ANIM_TARGETS_HIT 0x0
	setbyte STAT_ANIM_PLAYED 0x0
	jumpifstatcanbelowered BANK_TARGET STAT_ATK PartingShot_LowerAtk
	jumpifstatcanbelowered BANK_TARGET STAT_SPATK PartingShot_LowerAtk
	pause 0x10
	goto PartingShot_SkipAnim
	
PartingShot_LowerAtk:
	attackanimation
	waitanimation
	setbyte ANIM_TARGETS_HIT 0x1 @;So we know parting shot worked
	playstatchangeanimation BANK_TARGET, STAT_ANIM_ATK | STAT_ANIM_SPATK, STAT_ANIM_DOWN | STAT_ANIM_ONLY_MULTIPLE

PartingShot_SkipAnim:
	setbyte FORM_COUNTER 0x0
	playstatchangeanimation BANK_TARGET, STAT_ANIM_ATK STAT_ANIM_DOWN
	setstatchanger STAT_ATK | DECREASE_1
	statbuffchange STAT_TARGET | STAT_BS_PTR PartingShot_LowerSpAtk
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x3 PartingShot_LowerSpAtk
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x4 BS_MOVE_END
	printfromtable gStatDownStringIds
	waitmessage DELAY_1SECOND

PartingShot_LowerSpAtk:
	playstatchangeanimation BANK_TARGET, STAT_ANIM_SPATK STAT_ANIM_DOWN
	setstatchanger STAT_SPATK | DECREASE_1
	statbuffchange STAT_TARGET | STAT_BS_PTR CheckPartingShotFail
	jumpifbyte GREATERTHAN MULTISTRING_CHOOSER 0x2 CheckPartingShotFail
	printfromtable gStatDownStringIds
	waitmessage DELAY_1SECOND
	
CheckPartingShotFail:
	jumpifbyte EQUALS ANIM_TARGETS_HIT 0x0 BS_MOVE_END @;Anim didn't play means no stats were lowered
	setbyte CMD49_STATE 0x0
	cmd49 BANK_TARGET 0x0
	goto UTurnCheckSwitchBS

PartingShotEndBS:
	setbyte CMD49_STATE, 0x0
	cmd49 0x7 0x0
	end

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_128_Pursuit
BS_128_Pursuit:
	goto BS_STANDARD_HIT

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_129_RapidSpin
.global BattleScript_DefogAdditionalEffects_PlayAttackAnim
.global BattleScript_DefogAdditionalEffects
.global BattleScript_SideStatusWoreOffRet
BS_129_RapidSpin:
	jumpifmove MOVE_DEFOG DefogBS
	jumpifmove MOVE_MORTALSPIN MortalSpinBS
	jumpifmove MOVE_TIDYUP TidyUpBS
	
RapidSpinBS:
	setmoveeffect MOVE_EFFECT_RAPIDSPIN | MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_CERTAIN
	goto BS_STANDARD_HIT
	
DefogBS:
	attackcanceler
	attackstring
	ppreduce
	jumpifbehindsubstitute BANK_TARGET SecondDefogCheck_TryFail
	accuracycheck SecondDefogCheck_FailPlayResultMessage 0x0
	setstatchanger STAT_EVSN | DECREASE_1
	statbuffchange STAT_TARGET | STAT_BS_PTR SecondDefogCheck_TryFail
	jumpifbyte LESSTHAN MULTISTRING_CHOOSER 0x2 DefogLoweredStat
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x3 SecondDefogCheck_TryFail
	pause DELAY_HALFSECOND
	printfromtable gStatDownStringIds
	waitmessage DELAY_1SECOND
SecondDefogCheck:
	callasm DefogHelperFunc @;Automatically redirects to BattleScript_DefogAdditionalEffects if applicable
	goto BS_MOVE_END

SecondDefogCheck_TryFail:
	setword OUTCOME, OUTCOME_NO_EFFECT
SecondDefogCheck_FailPlayResultMessage:
	pause 0x20
	resultmessage
	waitmessage DELAY_1SECOND
	goto SecondDefogCheck

DefogLoweredStat:
	attackanimation
	waitanimation
	setbyte ANIM_TARGETS_HIT 0x1 @;So animation doesn't play again
	setgraphicalstatchangevalues
	playanimation BANK_TARGET ANIM_STAT_BUFF ANIM_ARG_1
	printfromtable gStatDownStringIds
	waitmessage DELAY_1SECOND
	goto SecondDefogCheck

BattleScript_DefogAdditionalEffects_PlayAttackAnim:
	bicword OUTCOME, OUTCOME_NO_EFFECT
BattleScript_DefogAdditionalEffects:
	attackanimation @;Should only play after the Second Defog Check
	waitanimation
	jumpifweather weather_circus SkipRemoveFogBS
	jumpifweather weather_fog | weather_permament_fog RemoveFogBS
SkipRemoveFogBS:
	breakfree
	goto BS_MOVE_END

RemoveFogBS:
	sethalfword WEATHER_FLAGS 0x0
	setword BATTLE_STRING_LOADER RemoveFogString
	printstring 0x184
	waitmessage DELAY_1SECOND
	breakfree
	goto BS_MOVE_END

BattleScript_SideStatusWoreOffRet:
	setword BATTLE_STRING_LOADER gText_DefogRemovedSideStatus
	printstring 0x184
	waitmessage DELAY_1SECOND
	return

MortalSpinBS:
	attackcanceler
	accuracycheck BS_MOVE_MISSED 0x0
	setmoveeffect MOVE_EFFECT_RAPIDSPIN | MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_CERTAIN
	call STANDARD_DAMAGE
	seteffectwithchancetarget
	setmoveeffect MOVE_EFFECT_POISON
	seteffectwithchancetarget
	goto BS_MOVE_FAINT

TidyUpBS:
	attackcanceler
	attackstring
	ppreduce
	jumpifbehindsubstitute BANK_TARGET SecondTidyUpCheck_TryFail
	accuracycheck SecondTidyUpCheck_FailPlayResultMessage 0x0
	setstatchanger STAT_EVSN | DECREASE_1
	statbuffchange STAT_TARGET | STAT_BS_PTR SecondTidyUpCheck_TryFail
	jumpifbyte LESSTHAN MULTISTRING_CHOOSER 0x2 TidyUpLoweredStat
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x3 SecondTidyUpCheck_TryFail
	pause DELAY_HALFSECOND
	printfromtable gStatDownStringIds
	waitmessage DELAY_1SECOND
SecondTidyUpCheck:
	callasm DefogHelperFunc @;Automatically redirects to BattleScript_TidyUpAdditionalEffects if applicable
	goto BS_MOVE_END

SecondTidyUpCheck_TryFail:
	setword OUTCOME, OUTCOME_NO_EFFECT
SecondTidyUpCheck_FailPlayResultMessage:
	pause 0x20
	resultmessage
	waitmessage DELAY_1SECOND
	goto SecondTidyUpCheck

TidyUpLoweredStat:
	attackanimation
	waitanimation
	setbyte STAT_ANIM_PLAYED 0x0
	playstatchangeanimation BANK_ATTACKER, STAT_ANIM_ATK | STAT_ANIM_SPD, STAT_ANIM_UP | STAT_ANIM_IGNORE_ABILITIES
	setstatchanger STAT_ATK | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN TidyUpSpd_Up
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 TidyUpSpd_Up
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND

TidyUpSpd_Up:
	setstatchanger STAT_SPD | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN BS_MOVE_END
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 BS_MOVE_END
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

BattleScript_TidyUpAdditionalEffects_PlayAttackAnim:
	bicword OUTCOME, OUTCOME_NO_EFFECT
BattleScript_TidyUpAdditionalEffects:
	attackanimation @;Should only play after the Second TidyUp Check
	waitanimation
	jumpifweather weather_circus SkipRemoveFogBS
	jumpifweather weather_fog | weather_permament_fog RemoveFogBS

IS_Remove:
	seteffectprimary
	playanimation2 BANK_ATTACKER ANIM_ARG_1 0x0
	printstring 0x184
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_130_Sonicboom
BS_130_Sonicboom:
	attackcanceler
	accuracycheck BS_MOVE_MISSED 0x0
	attackstring
	ppreduce
	typecalc2
	bicbyte OUTCOME OUTCOME_SUPER_EFFECTIVE | OUTCOME_NOT_VERY_EFFECTIVE
	setword DAMAGE_LOC 0x0
	setbyte DAMAGE_LOC 20
	adjustsetdamage
	goto BS_HIT_FROM_ATTACKANIMATION

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_131_Blank
BS_131_Blank:
	goto BS_STANDARD_HIT

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
	goto BS_STANDARD_HIT

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	
.global BS_134_Blank @;Was Moonlight
BS_134_Blank:
	goto BS_STANDARD_HIT
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_135_AttackerRaiseDef2Chance
BS_135_AttackerRaiseDef2Chance:
	setmoveeffect MOVE_EFFECT_DEF_PLUS_2 | MOVE_EFFECT_AFFECTS_USER
	goto BS_STANDARD_HIT

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_136_RainDance
BS_136_RainDance:
	attackcanceler
	attackstringnoprotean
	ppreduce
	jumpifweather WEATHER_RAIN_ANY, RainDanceSkipPrimalWeatherCheck @;Fails normally
	tryblockweatherwithprimalweather
RainDanceSkipPrimalWeatherCheck:
	setrain
	tryactivateprotean
	goto BS_MOVE_WEATHER_CHANGE

.global BattleScript_RainDanceBlockedByEvaporate
BattleScript_RainDanceBlockedByEvaporate:
	pause 0x20
	call BattleScript_AbilityPopUp
	setword BATTLE_STRING_LOADER gText_ButAbilityEvaporatesRain
	printstring 0x184
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_137_SunnyDay
BS_137_SunnyDay:
	attackcanceler
	attackstringnoprotean
	ppreduce
	jumpifweather WEATHER_SUN_ANY, SunnyDaySkipPrimalWeatherCheck @;Fails normally
	tryblockweatherwithprimalweather
SunnyDaySkipPrimalWeatherCheck:
	setsunny
	tryactivateprotean
	attackanimation
	waitanimation
	printfromtable 0x83FE528 @;gMoveWeatherChangeStringIds
	waitmessage DELAY_1SECOND
	jumpifmovehadnoeffect BS_MOVE_END
	callasm TryActivateProtosynthesis
	call BS_WEATHER_FORM_CHANGES
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_138_AttackerRaiseDef1Chance
BS_138_AttackerRaiseDef1Chance:
	setmoveeffect MOVE_EFFECT_DEF_PLUS_1 | MOVE_EFFECT_AFFECTS_USER
	goto BS_STANDARD_HIT

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_139_AttackerRaiseAtk1Chance
BS_139_AttackerRaiseAtk1Chance:
	jumpifmove MOVE_FELLSTINGER FellStingerBS
	jumpifspecies BANK_ATTACKER SPECIES_TATSUGIRI OrderUpAtk_BS
	jumpifspecies BANK_ATTACKER SPECIES_TATSUGIRI_RED OrderUpDef_BS
	jumpifspecies BANK_ATTACKER SPECIES_TATSUGIRI_YELLOW OrderUpSpd_BS
	setmoveeffect MOVE_EFFECT_ATK_PLUS_1 | MOVE_EFFECT_AFFECTS_USER
	goto BS_STANDARD_HIT

FellStingerBS:
	attackcanceler
	accuracycheck BS_MOVE_MISSED 0x0
	call STANDARD_DAMAGE
	prefaintmoveendeffects 0x0
	faintpokemonaftermove
	jumpifnoviablemonsleft BANK_TARGET BS_MOVE_END
	jumpiffainted BANK_ATTACKER BS_MOVE_END
	jumpiffainted BANK_TARGET FellStingerKill
	goto BS_MOVE_END

FellStingerKill:
	setbyte STAT_ANIM_PLAYED 0x0
	jumpifstat BANK_ATTACKER EQUALS STAT_ATK STAT_MAX BS_MOVE_END
	playstatchangeanimation BANK_ATTACKER, STAT_ANIM_ATK, STAT_ANIM_UP | STAT_ANIM_IGNORE_ABILITIES | STAT_ANIM_BY_TWO
	setstatchanger STAT_ATK | INCREASE_3
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN, BS_MOVE_END
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 BS_MOVE_END
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

OrderUpAtk_BS:
	setmoveeffect MOVE_EFFECT_ATK_PLUS_1 | MOVE_EFFECT_AFFECTS_USER
	goto BS_STANDARD_HIT

OrderUpDef_BS:
	setmoveeffect MOVE_EFFECT_DEF_PLUS_1 | MOVE_EFFECT_AFFECTS_USER
	goto BS_STANDARD_HIT

OrderUpSpd_BS:
	setmoveeffect MOVE_EFFECT_SPD_PLUS_1 | MOVE_EFFECT_AFFECTS_USER
	goto BS_STANDARD_HIT

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	
.global BS_140_AttackerRaiseAllStatsChance
BS_140_AttackerRaiseAllStatsChance:
	setmoveeffect MOVE_EFFECT_ALL_STATS_UP | MOVE_EFFECT_AFFECTS_USER
	goto BS_STANDARD_HIT

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_141_AttackerHitRaiseHigherOffensesDefenses
BS_141_AttackerHitRaiseHigherOffensesDefenses:
	setmoveeffect MOVE_EFFECT_HIGHER_OFFENSES_DEFENSES_UP_1 | MOVE_EFFECT_AFFECTS_USER
	goto BS_STANDARD_HIT

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_142_BellyDrum
BS_142_BellyDrum:
	attackcanceler
	attackstringnoprotean
	ppreduce
	maxattackhalvehp FAILED
	orword HIT_MARKER HITMARKER_IGNORE_SUBSTITUTE
	tryactivateprotean
	attackanimation
	waitanimation
	graphicalhpupdate BANK_ATTACKER
	datahpupdate BANK_ATTACKER
	playanimation BANK_ATTACKER ANIM_STAT_BUFF ANIM_ARG_1
	jumpifability BANK_ATTACKER ABILITY_CONTRARY BattleScript_PrintContraryBellyDrumLoweredAttack
	printstring 0x9B
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END
	
BattleScript_PrintContraryBellyDrumLoweredAttack:
	setword BATTLE_STRING_LOADER gText_BellyDrumMinimizedAttack
	printstring 0x184
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_143_PsychUp
BS_143_PsychUp:
	jumpifnotmove MOVE_SPECTRALTHIEF 0x81D7A74

SpectralThiefBS:
	attackcanceler
	accuracycheck BS_MOVE_MISSED 0x0
	attackstring
	ppreduce
	typecalc2
	jumpifmovehadnoeffect SpectralThiefBS_SkipSteal
	bicbyte OUTCOME OUTCOME_SUPER_EFFECTIVE | OUTCOME_NOT_VERY_EFFECTIVE
	tryspectralthiefsteal PlaySpectBoost
SpectralThiefBS_SkipSteal:
	setbyte ANIM_TURN 0x1
	goto BS_HIT_FROM_DAMAGE_CALC

PlaySpectBoost:
	attackanimation
	waitanimation
	setbyte FORM_COUNTER 0x0
	callasm ToggleSpectralThiefByte
	playstatchangeanimation BANK_ATTACKER, 0xFF, STAT_ANIM_UP | STAT_ANIM_IGNORE_ABILITIES
	callasm ToggleSpectralThiefByte
	setword BATTLE_STRING_LOADER SpectralThiefString
	printstring 0x184
	waitmessage DELAY_1SECOND
	setbyte ANIM_TURN 0x1
	callasm ClearCalculatedSpreadMoveData @;So the damage can be calculated
	goto BS_HIT_FROM_DAMAGE_CALC

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_144_MirrorCoat
BS_144_MirrorCoat:
	attackcanceler
	mirrorcoatdamagecalculator FAILED_PRE
	accuracycheck BS_MOVE_MISSED 0x0
	attackstring
	ppreduce
	typecalc2
	bicbyte OUTCOME OUTCOME_SUPER_EFFECTIVE | OUTCOME_NOT_VERY_EFFECTIVE
	adjustsetdamage
	goto BS_HIT_FROM_ATTACKANIMATION

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_145_SkullBash
BS_145_SkullBash:
	jumpifsecondarystatus BANK_ATTACKER STATUS2_MULTIPLETURNS TwoTurnMovesSecondTurnBS
	jumpifword ANDS HIT_MARKER HITMARKER_NO_ATTACKSTRING TwoTurnMovesSecondTurnBS
	setbyte TWOTURN_STRINGID, 0x2
	call BattleScript_FirstChargingTurn
	jumpifmove MOVE_METEORBEAM BS_MeteorBeam
	jumpifmove MOVE_ELECTROSHOT BS_ElectroShot
	setstatchanger STAT_DEF | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN SkipSkullBashStatBuff
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 SkipSkullBashStatBuff
	setgraphicalstatchangevalues
	playanimation BANK_ATTACKER ANIM_STAT_BUFF ANIM_ARG_1
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
SkipSkullBashStatBuff:
	call BattleScript_CheckPowerHerb
	goto BS_MOVE_END

BS_MeteorBeam:
	setstatchanger STAT_SPATK | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN SkipMeteorBeamStatBuff
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 SkipMeteorBeamStatBuff
	setgraphicalstatchangevalues
	playanimation BANK_ATTACKER ANIM_STAT_BUFF ANIM_ARG_1
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
SkipMeteorBeamStatBuff:
	call BattleScript_CheckPowerHerb
	goto BS_MOVE_END

BS_ElectroShot:
	setstatchanger STAT_SPATK | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN SkipElectroShotStatBuff
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 SkipElectroShotStatBuff
	setgraphicalstatchangevalues
	playanimation BANK_ATTACKER ANIM_STAT_BUFF ANIM_ARG_1
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
SkipElectroShotStatBuff:
	call BattleScript_CheckPowerHerb
	call BattleScriptCheckRain
	goto BS_MOVE_END

BattleScriptCheckRain:
	jumpifweather WEATHER_RAIN_ANY, TwoTurnMovesRaidBossSkipCharge
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_146_Twister
BS_146_Twister:
	orword HIT_MARKER HITMARKER_IGNORE_IN_AIR
	goto BS_031_Flinch

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_147_Earthquake
BS_147_Earthquake:
	orword HIT_MARKER HITMARKER_IGNORE_UNDERGROUND
	goto BS_STANDARD_HIT

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_148_FutureSight
BS_148_FutureSight:
	attackcanceler
	attackstringnoprotean
	ppreduce
	setfutureattack FAILED
	tryactivateprotean
	attackanimation
	waitanimation
	printfromtable 0x83FE5EE
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_149_Gust
BS_149_Gust:
	orword HIT_MARKER HITMARKER_IGNORE_IN_AIR
	goto BS_STANDARD_HIT

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_150_Splinters @;Was Stomp
BS_150_Splinters:
	setmoveeffect MOVE_EFFECT_SPLINTERS
	goto BS_STANDARD_HIT

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_151_Solarbeam
BS_151_Solarbeam:
	jumpifabilitypresent ABILITY_CLOUDNINE, BSSolarbeamDecideTurn
	@;jumpifabilitypresent ABILITY_AIRLOCK, BSSolarbeamDecideTurn
	jumpifhelditemeffect BANK_ATTACKER, ITEM_EFFECT_UTILITY_UMBRELLA, BSSolarbeamDecideTurn
	jumpifweather WEATHER_SUN_ANY BSSolarbeamOnFirstTurn

BSSolarbeamDecideTurn:
	jumpifsecondarystatus BANK_ATTACKER STATUS2_MULTIPLETURNS TwoTurnMovesSecondTurnBS
	jumpifword ANDS HIT_MARKER HITMARKER_NO_ATTACKSTRING TwoTurnMovesSecondTurnBS
	setbyte TWOTURN_STRINGID, 0x1
	call BattleScript_FirstChargingTurn
	call BattleScript_CheckPowerHerb
	goto BS_MOVE_END

BSSolarbeamOnFirstTurn:
	orword HIT_MARKER HITMARKER_CHARGING
	setmoveeffect MOVE_EFFECT_CHARGING | MOVE_EFFECT_AFFECTS_USER
	seteffectprimary
	ppreduce
	callasm ClearCalculatedSpreadMoveData @;So the damage can be calculated
	goto TwoTurnMovesSecondTurnBS

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_152_Thunder
BS_152_Thunder:
	orword HIT_MARKER HITMARKER_IGNORE_IN_AIR
	jumpifmove MOVE_HURRICANE BS_076_SetConfusionChance
	setmoveeffect MOVE_EFFECT_PARALYSIS
	goto BS_STANDARD_HIT	

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_153_Teleport
.global BattleScript_TeleportFlee
.global BattleScript_TeleportSwitch
BS_153_Teleport:
	attackcanceler
	callasm SetCorrectTeleportBattleScript

BattleScript_TeleportFlee:
	jumpifbattletype BATTLE_DOUBLE, FAILED_PRE
	callasm SetTeleportBit
	getifcantrunfrombattle BANK_ATTACKER
	jumpifbyte EQUALS BATTLE_COMMUNICATION 0x1 FAILED_PRE
	jumpifbyte EQUALS BATTLE_COMMUNICATION 0x2 FAILED_PRE
	attackstring
	ppreduce
	attackanimation
	waitanimation
	printstring 0xA0 @;STRINGID_PKMNFLEDFROMBATTLE
	waitmessage DELAY_1SECOND
	setbyte BATTLE_OUTCOME 0x5 @;Teleported
	goto BS_MOVE_END

BattleScript_TeleportSwitch:
	jumpifcannotswitch BANK_ATTACKER | ATK4F_DONT_CHECK_STATUSES, FAILED_PRE
	attackstring
	ppreduce
	attackanimation
	waitanimation
	copybyte SWITCHING_BANK USER_BANK
	goto BatonPassSwitchOutBS

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_154_BeatUp
BS_154_BeatUp:
	attackcanceler
	accuracycheck BS_MOVE_MISSED 0x0
	addbyte BATTLE_COMMUNICATION 0x1
	goto BS_HIT_FROM_ATTACKSTRING
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_155_Fly
BS_155_Fly:
	jumpifsecondarystatus BANK_ATTACKER STATUS2_MULTIPLETURNS BS_SecondTurnSemiInvulnerable
	jumpifword ANDS HIT_MARKER HITMARKER_NO_ATTACKSTRING BS_SecondTurnSemiInvulnerable
	jumpifmove MOVE_FLY BS_FirstTurnFly
	jumpifmove MOVE_DIVE BS_FirstTurnDive
	jumpifmove MOVE_BOUNCE, BS_FirstTurnBounce
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
	call BattleScript_FirstChargingTurn
	call BattleScript_CheckPowerHerb
	setsemiinvulnerablebit
	goto BS_MOVE_END

BS_SecondTurnSemiInvulnerable:
	attackcanceler
	setmoveeffect MOVE_EFFECT_CHARGING
	setbyte ANIM_TURN, 0x1
	clearstatus BANK_ATTACKER
	orword HIT_MARKER HITMARKER_NO_PPDEDUCT
	jumpifmove MOVE_SKYDROP SkyDropDropTurn2
	jumpifnotmove MOVE_BOUNCE BS_SemiInvulnerableTryHit
	setmoveeffect MOVE_EFFECT_PARALYSIS

BS_SemiInvulnerableTryHit:
	clearsemiinvulnerablebit
	goto 0x81D6927

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_156_DefenseCurl
BS_156_DefenseCurl:
	attackcanceler
	attackstring
	ppreduce
	setcurled
	setstatchanger STAT_DEF | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN 0x81D6BBC
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 0x81D6BC4
	attackanimation
	waitanimation
	goto 0x81D6BBC

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_157_SpringtideStorm @;Was Softboiled
BS_157_SpringtideStorm:
	callasm ChooseMoveEffectForSpringtideStorm

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_158_FakeOut
BS_158_FakeOut:
	attackcanceler
	jumpifnotfirstturn FAILED_PRE
	jumpifmove MOVE_FIRSTIMPRESSION BS_STANDARD_HIT
	setmoveeffect MOVE_EFFECT_FLINCH | MOVE_EFFECT_CERTAIN
	goto BS_STANDARD_HIT

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_159_Uproar
BS_159_Uproar:
	attackcanceler
	accuracycheck BS_MOVE_MISSED 0x0
	setmoveeffect MOVE_EFFECT_UPROAR | MOVE_EFFECT_AFFECTS_USER
	attackstring
	jumpifsecondarystatus BANK_ATTACKER STATUS2_MULTIPLETURNS BS_HIT_FROM_DAMAGE_CALC
	ppreduce
	call STANDARD_DAMAGE + 2
	seteffectwithchancetarget
	prefaintmoveendeffects 0x0
	faintpokemonaftermove
	callasm WakeUpSleepingPokemon
	goto BS_MOVE_END

.global BattleScript_UproarWokeUp
BattleScript_UproarWokeUp:
	cureprimarystatus BANK_SCRIPTING .LReturn
	refreshhpbar BANK_SCRIPTING
	setword BATTLE_STRING_LOADER gText_WokenByUproar
	printstring 0x184
	waitmessage DELAY_1SECOND
.LReturn:
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_160_Stockpile
BS_160_Stockpile:
	attackcanceler
	attackstring
	ppreduce
	stockpile
	attackanimation
	waitanimation
	printfromtable 0x83FE56A
	waitmessage DELAY_1SECOND
	jumpifmovehadnoeffect BS_MOVE_END
	
StockpileRaiseDef:
	setbyte STAT_ANIM_PLAYED 0x0
	playstatchangeanimation BANK_ATTACKER, STAT_ANIM_DEF | STAT_ANIM_SPDEF, STAT_ANIM_UP | STAT_ANIM_IGNORE_ABILITIES
	setstatchanger STAT_DEF | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN StockpileRaiseSpDef
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 StockpileRaiseSpDef
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND

StockpileRaiseSpDef:
	setstatchanger STAT_SPDEF | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN BS_MOVE_END
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 BS_MOVE_END
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_161_SpitUp
BS_161_SpitUp:
	attackcanceler
	jumpifbyte EQUALS, BATTLE_COMMUNICATION + 6, 0x1 0x81D7E78
	accuracycheck BS_MOVE_MISSED 0x0
	attackstringnoprotean
	ppreduce
	stockpiletobasedamage 0x81D7E6A @;Spit Up Fail
	tryactivateprotean
	call STANDARD_DAMAGE + 2
	seteffectwithchancetarget
	prefaintmoveendeffects 0x0
	faintpokemonaftermove
	
SpitUpCompareStats:
	jumpifstat BANK_TARGET GREATERTHAN STAT_DEF STAT_MIN SpitUpLowerDef
	jumpifstat BANK_TARGET EQUALS STAT_SPDEF STAT_MIN StockpileWearOff

SpitUpLowerDef:
	setbyte STAT_ANIM_PLAYED 0x0
	playstatchangeanimation BANK_ATTACKER, STAT_ANIM_DEF | STAT_ANIM_SPDEF, STAT_ANIM_DOWN | STAT_ANIM_IGNORE_ABILITIES
	setstatchanger STAT_DEF
	callasm ModifyPostStockpileBoostDecrement
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN SpitUpLowerSpDef
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 SpitUpLowerSpDef
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	
SpitUpLowerSpDef:
	setstatchanger STAT_SPDEF
	callasm ModifyPostStockpileBoostDecrement
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN StockpileWearOff
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 StockpileWearOff
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND

StockpileWearOff:
	callasm RemoveStockpileBoosts
	setword BATTLE_STRING_LOADER StockpileWoreOffString
	printstring 0x184
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_162_Swallow
BS_162_Swallow:
	attackcanceler
	attackstringnoprotean
	ppreduce
	stockpiletohprecovery 0x81D7E98
	tryactivateprotean
	attackanimation
	waitanimation
	orword HIT_MARKER HITMARKER_IGNORE_SUBSTITUTE
	graphicalhpupdate BANK_ATTACKER
	datahpupdate BANK_ATTACKER
	printstring 0x4B
	waitmessage DELAY_1SECOND
	goto SpitUpCompareStats

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_163_Blank
BS_163_Blank:
	goto BS_STANDARD_HIT

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_164_SetHail
BS_164_SetHail:
	attackcanceler
	attackstringnoprotean
	ppreduce
	jumpifweather WEATHER_HAIL_ANY, HailSkipPrimalWeatherCheck @;Fails normally
	tryblockweatherwithprimalweather
HailSkipPrimalWeatherCheck:
	sethail
	tryactivateprotean
	attackanimation @;Don't use BS_MOVE_WEATHER_CHANGE because of special logic for Ice Face
	waitanimation
	printfromtable 0x83FE528 @;gMoveWeatherChangeStringIds
	waitmessage DELAY_1SECOND
	jumpifmovehadnoeffect BS_MOVE_END @;Prevents Ice Face from activatig on fail
	call BS_WEATHER_FORM_CHANGES
	jumpifmove MOVE_CHILLYRECEPTION BattleScript_TeleportSwitch
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_165_Torment
BS_165_Torment:
	attackcanceler
	accuracycheck FAILED_PRE 0x0
	attackstringnoprotean
	ppreduce
	jumpifabilitypresenttargetfield ABILITY_AROMAVEIL BattleScript_ProtectedByAromaVeil
	callasm FailMoveIfAura
	settorment FAILED
	tryactivateprotean
	attackanimation
	waitanimation
	printstring 0xA9 @;STRINGID_PKMNSUBJECTEDTOTORMENT
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_166_Flatter
BS_166_Flatter:
	attackcanceler
	jumpifbehindsubstitute BANK_TARGET BS_MAKE_MOVE_MISS
	accuracycheck BS_MOVE_MISSED 0x0
	jumpifconfusedandstatmaxed STAT_SPATK FAILED_PRE
	attackstring
	ppreduce
	attackanimation
	waitanimation
	setstatchanger STAT_SPATK | INCREASE_1
	statbuffchange STAT_TARGET | STAT_BS_PTR SwaggerTryConfuseBS
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 SwaggerTryConfuseBS
	setgraphicalstatchangevalues
	playanimation BANK_TARGET ANIM_STAT_BUFF ANIM_ARG_1
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	goto SwaggerTryConfuseBS

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_167_SetBurn
BS_167_SetBurn:
	attackcanceler
	attackstring @;Protean always activates!
	ppreduce
	jumpifbehindsubstitute BANK_TARGET FAILED
	trysetburn BANK_TARGET BS_StatusMoveFail
	accuracycheck BS_MOVE_MISSED_PAUSE 0x0
	attackanimation
	waitanimation
	setmoveeffect MOVE_EFFECT_BURN
	seteffectprimary
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_168_Memento
BS_168_Memento:
	attackcanceler
	jumpifmove MOVE_FINALGAMBIT FinalGambitBS
	jumpifmove MOVE_HEALINGWISH HealingWishBS
	jumpifmove MOVE_LUNARDANCE HealingWishBS
	
MementoBS:
	jumpifbyte EQUALS, BATTLE_COMMUNICATION + 6, 0x1, 0x81D8026
	jumpifbehindsubstitute BANK_TARGET FAILED_PRE
	jumpifspecies BANK_TARGET SPECIES_GHOLDENGO FAILED_PRE
	attackstring
	ppreduce
	attackanimation
	waitanimation
	setbyte STAT_ANIM_PLAYED 0x0
	playstatchangeanimation BANK_TARGET, STAT_ANIM_ATK | STAT_ANIM_SPATK, STAT_ANIM_DOWN | STAT_ANIM_BY_TWO | STAT_ANIM_ONLY_MULTIPLE
	
Memento_Atk:
	playstatchangeanimation BANK_TARGET, STAT_ANIM_ATK, STAT_ANIM_DOWN | STAT_ANIM_BY_TWO
	setstatchanger STAT_ATK | DECREASE_2
	statbuffchange STAT_TARGET | STAT_BS_PTR MementoPrintAtkMsg
MementoPrintAtkMsg:
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x3 Memento_SpAtk
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x4 MementoFaintUser
	printfromtable gStatDownStringIds
	waitmessage DELAY_1SECOND

Memento_SpAtk:
	playstatchangeanimation BANK_TARGET, STAT_ANIM_SPATK, STAT_ANIM_DOWN | STAT_ANIM_BY_TWO
	setstatchanger STAT_SPATK | DECREASE_2
	statbuffchange STAT_TARGET | STAT_BS_PTR MementoPrintSpAtkMsg
MementoPrintSpAtkMsg:
	jumpifbyte GREATERTHAN MULTISTRING_CHOOSER 0x2 MementoFaintUser
	printfromtable gStatDownStringIds
	waitmessage DELAY_1SECOND
	
MementoFaintUser:
	setuserhptozero
	graphicalhpupdate BANK_ATTACKER
	datahpupdate BANK_ATTACKER
	faintpokemon BANK_ATTACKER 0x0 0x0
	goto BS_MOVE_END
	
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

HealingWishBS:
	jumpifcannotswitch BANK_ATTACKER | ATK4F_DONT_CHECK_STATUSES FAILED_PRE
	attackstring
	ppreduce
	setuserhptozero
	attackanimation
	waitanimation
	callasm SetHealingWishLunarDanceFunc
	setuserhptozero
	graphicalhpupdate BANK_ATTACKER
	datahpupdate BANK_ATTACKER
	faintpokemon BANK_ATTACKER 0x0 0x0
	goto BS_MOVE_END

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

FinalGambitBS:
	accuracycheck BS_MOVE_MISSED 0x0
	attackstring
	ppreduce
	typecalc2
	bicbyte OUTCOME OUTCOME_SUPER_EFFECTIVE | OUTCOME_NOT_VERY_EFFECTIVE
	callasm FinalGambitDamageCalc
	adjustsetdamage
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
	seteffectwithchancetarget
	prefaintmoveendeffects 0x0
	faintpokemonaftermove
	jumpifmovehadnoeffect BS_MOVE_END
	setuserhptozero
	graphicalhpupdate BANK_ATTACKER
	datahpupdate BANK_ATTACKER
	faintpokemon BANK_ATTACKER 0x0 0x0
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_169_Blank @;Was Facade
BS_169_Blank:
	goto BS_STANDARD_HIT

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_170_FocusPunch
BS_170_FocusPunch:
	jumpifmove MOVE_BEAKBLAST BeakBlastDoBS
	jumpifmove MOVE_SHELLTRAP ShellTrapDoBS
	goto 0x81D805C
	
BeakBlastDoBS:
	callasm ClearBeakBlastBit
	goto 0x81D6926

ShellTrapDoBS:
	attackcanceler
	callasm TryLaunchShellTrap
	goto 0x81D6927

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_171_SmellingSalt
BS_171_SmellingSalt:
	jumpifbehindsubstitute BANK_TARGET BS_STANDARD_HIT
	setmoveeffect MOVE_EFFECT_REMOVE_PARALYSIS | MOVE_EFFECT_CERTAIN
	goto BS_STANDARD_HIT

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_172_FollowMe
BS_172_FollowMe:
	attackcanceler
	attackstring
	ppreduce
	setforcedtarget
	attackanimation
	waitanimation
	printstring 0xA4 @;STRINGID_PKMNCENTERATTENTION
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_173_NaturePower
BS_173_NaturePower:
	attackcanceler
	attackstring
	pause DELAY_HALFSECOND
	callterrainattack
	printstring 0xA6 @;STRINGID_NATUREPOWERTURNEDINTO
	waitmessage DELAY_1SECOND
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_174_Charge
BS_174_Charge:
	attackcanceler
	attackstring
	ppreduce
	setcharge
	attackanimation
	waitanimation
	printstring 0xA5
	waitmessage DELAY_1SECOND
	setmoveeffect MOVE_EFFECT_SP_DEF_PLUS_1 | MOVE_EFFECT_AFFECTS_USER
	seteffectprimary
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_175_Taunt
BS_175_Taunt:
	attackcanceler
	accuracycheck FAILED_PRE 0x0
	attackstringnoprotean
	ppreduce
	jumpifability BANK_TARGET ABILITY_OBLIVIOUS BattleScript_ObliviousPrevents
	jumpifabilitypresenttargetfield ABILITY_AROMAVEIL BattleScript_ProtectedByAromaVeil
	settaunt FAILED
	tryactivateprotean
	attackanimation
	waitanimation
	printstring 0xAC
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_176_HelpingHand
BS_176_HelpingHand:
	attackcanceler
	jumpifmove MOVE_DECORATE BS_Decorate
	jumpifmove MOVE_COACHING BS_Coaching
	jumpifmove MOVE_DRAGONCHEER BS_DragonCheer
	attackstringnoprotean
	ppreduce
	sethelpinghand FAILED
	tryactivateprotean
	attackanimation
	waitanimation
	printstring 0xAE @;STRINGID_PKMNREADYTOHELP
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BS_Decorate:
	jumpifspecialstatusflag BANK_TARGET STATUS3_SEMI_INVULNERABLE 0x0 FAILED
	jumpiffainted BANK_TARGET FAILED
	jumpifprotectedbycraftyshield BANK_TARGET FAILED
	attackstring
	ppreduce
	jumpifstat BANK_TARGET LESSTHAN STAT_ATK STAT_MAX Decorate_Atk
	jumpifstat BANK_TARGET EQUALS STAT_SPATK STAT_MAX BattleScript_CantRaiseMultipleTargetStats

Decorate_Atk:
	attackanimation
	waitanimation
	setbyte STAT_ANIM_PLAYED 0x0
	playstatchangeanimation BANK_TARGET, STAT_ANIM_ATK | STAT_ANIM_SPATK, STAT_ANIM_UP | STAT_ANIM_IGNORE_ABILITIES
	setstatchanger STAT_ATK | INCREASE_2
	statbuffchange STAT_TARGET | STAT_BS_PTR | STAT_CERTAIN Decorate_SpAtk
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 Decorate_SpAtk
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND

Decorate_SpAtk:
	setstatchanger STAT_SPATK | INCREASE_2
	statbuffchange STAT_TARGET | STAT_BS_PTR | STAT_CERTAIN BS_MOVE_END
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 BS_MOVE_END
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BS_Coaching:
	jumpifbyte NOTANDS BATTLE_TYPE BATTLE_DOUBLE FAILED_PRE
	callasm SetTargetPartner
	jumpiffainted BANK_TARGET FAILED_PRE
	jumpifspecialstatusflag BANK_TARGET STATUS3_SEMI_INVULNERABLE 0x0 FAILED_PRE
	attackstring
	ppreduce
	jumpifstat BANK_TARGET LESSTHAN STAT_ATK STAT_MAX Coaching_Atk
	jumpifstat BANK_TARGET EQUALS STAT_DEF STAT_MAX BattleScript_CantRaiseMultipleTargetStats

Coaching_Atk:
	attackanimation
	waitanimation
	setbyte STAT_ANIM_PLAYED 0x0
	playstatchangeanimation BANK_TARGET, STAT_ANIM_ATK | STAT_ANIM_DEF, STAT_ANIM_UP | STAT_ANIM_IGNORE_ABILITIES
	setstatchanger STAT_ATK | INCREASE_1
	statbuffchange STAT_TARGET | STAT_BS_PTR | STAT_CERTAIN Coaching_Def
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 Coaching_Def
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND

Coaching_Def:
	setstatchanger STAT_DEF | INCREASE_1
	statbuffchange STAT_TARGET | STAT_BS_PTR | STAT_CERTAIN BS_MOVE_END
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 BS_MOVE_END
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

BS_DragonCheer:
	jumpifspecialstatusflag BANK_TARGET STATUS3_SEMI_INVULNERABLE 0x0 FAILED
	jumpiffainted BANK_TARGET FAILED
	jumpifprotectedbycraftyshield BANK_TARGET FAILED
	attackstring
	ppreduce
	jumpifstat BANK_TARGET LESSTHAN STAT_ATK STAT_MAX DragonCheer_ACC
	jumpifstat BANK_TARGET EQUALS STAT_SPATK STAT_MAX BattleScript_CantRaiseMultipleTargetStats

DragonCheer_ACC:
	attackanimation
	waitanimation
	setbyte STAT_ANIM_PLAYED 0x0
	jumpiftype BANK_TARGET TYPE_DRAGON DragonCheer_ACC2
	playstatchangeanimation BANK_TARGET, STAT_ANIM_ACC, STAT_ANIM_UP | STAT_ANIM_IGNORE_ABILITIES
	setstatchanger STAT_ACC | INCREASE_1
	statbuffchange STAT_TARGET | STAT_BS_PTR | STAT_CERTAIN BS_MOVE_END
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 BS_MOVE_END
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

DragonCheer_ACC2:
	playstatchangeanimation BANK_TARGET, STAT_ANIM_ACC, STAT_ANIM_UP | STAT_ANIM_IGNORE_ABILITIES
	setstatchanger STAT_ACC | INCREASE_2
	statbuffchange STAT_TARGET | STAT_BS_PTR | STAT_CERTAIN BS_MOVE_END
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 BS_MOVE_END
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

BattleScript_CantRaiseMultipleTargetStats:
	pause DELAY_HALFSECOND
	orbyte OUTCOME OUTCOME_FAILED
	swapattackerwithtarget @;So the proper string is shown
	printstring 25 @;STRINGID_STATSWONTINCREASE2
	waitmessage DELAY_1SECOND
	swapattackerwithtarget
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_177_Trick
BS_177_Trick:
	jumpifmove MOVE_BESTOW BestowBS

TrickBS:
	attackcanceler
	jumpifbehindsubstitute BANK_TARGET FAILED_PRE
	accuracycheck FAILED_PRE 0x0
	attackstringnoprotean
	ppreduce
	itemswap FAILED
	tryactivateprotean
	attackanimation
	waitanimation
	printstring 0xAF @;STRINGID_PKMNSWITCHEDITEMS
	waitmessage DELAY_1SECOND
	printfromtable 0x83FE64A @;gItemSwapStringIds
	waitmessage DELAY_1SECOND
	call 0x81D92DC @;BattleScript_WeatherFormChanges - In case of Utility Umbrella
	goto BS_MOVE_END

BestowBS:
	attackcanceler
	jumpifbehindsubstitute BANK_TARGET FAILED_PRE
	accuracycheck FAILED_PRE 0x0
	attackstringnoprotean
	ppreduce
	callasm BestowItem
	callasm TransferLastUsedItem
	tryactivateprotean
	attackanimation
	waitanimation
	setword BATTLE_STRING_LOADER BestowString
	printstring 0x184
	waitmessage DELAY_1SECOND
	call 0x81D92DC @;BattleScript_WeatherFormChanges - In case of Utility Umbrella
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_178_RolePlay
BS_178_RolePlay:
	attackcanceler
	accuracycheck FAILED_PRE 0x0
	attackstringnoprotean
	ppreduce
	copyability FAILED
	tryactivateprotean
	attackanimation
	waitanimation
	copyarray BATTLE_SCRIPTING_BANK USER_BANK 0x1
	playanimation BANK_SCRIPTING ANIM_LOAD_ABILITY_POP_UP 0x0
	call BattleScript_AbilityPopUpRevert
	call BattleScript_AbilityPopUp
	pause DELAY_HALFSECOND
	call BattleScript_AbilityPopUpRevert
	printstring 0xB0 @;STRINGID_PKMNCOPIEDFOE
	waitmessage DELAY_1SECOND
	copybyte BATTLE_SCRIPTING_BANK USER_BANK
	call BattleScript_TryRemoveIllusion
	callasm TryRemovePrimalWeatherAfterAbilityChange
	call 0x81D92DC @;Try to revert Cherrim and Castform
	callasm RestoreOriginalAttackerAndTarget
	tryactivateswitchinability BANK_ATTACKER
	callasm RestoreOriginalAttackerAndTarget
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_179_Wish
BS_179_Wish:
	attackcanceler
	attackstringnoprotean
	ppreduce
	trywish 0x0 FAILED
	tryactivateprotean
	attackanimation
	waitanimation
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_180_Assist
BS_180_Assist:
	attackcanceler
	attackstringnoprotean
	assistattackselect FAILED-1
	attackanimation
	waitanimation
	setbyte ANIM_TURN 0x0
	setbyte ANIM_TARGETS_HIT 0x0
	jumptoattack 0x1

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_181_Ingrain
BS_181_Ingrain:
	attackcanceler
	jumpifmove MOVE_AQUARING AquaRingBS

IngrainBS:
	attackcanceler
	attackstringnoprotean
	ppreduce
	setroots FAILED
	tryactivateprotean
	attackanimation
	waitanimation
	printstring 0xB3
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END
	
AquaRingBS:
	jumpifspecialstatusflag BANK_ATTACKER STATUS3_AQUA_RING 0x0 FAILED_PRE
	attackstring
	ppreduce
	attackanimation
	waitanimation
	setword BATTLE_STRING_LOADER AquaRingSetString
	printstring 0x184
	waitmessage DELAY_1SECOND
	setspecialstatusbit BANK_ATTACKER STATUS3_AQUA_RING
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_182_Superpower
BS_182_Superpower:
	jumpifmove MOVE_CLOSECOMBAT CloseCombatBS
	jumpifmove MOVE_DRAGONASCENT CloseCombatBS
	jumpifmove MOVE_HEADLONGRUSH CloseCombatBS
	jumpifmove MOVE_ARMORCANNON CloseCombatBS
	jumpifmove MOVE_HAMMERARM HammerArmBS
	jumpifmove MOVE_ICEHAMMER HammerArmBS
	jumpifmove MOVE_CLANGINGSCALES ClangingScalesBS
	jumpifmove MOVE_VCREATE VCreateBS
	jumpifmove MOVE_HYPERSPACEFURY HyperspaceFuryBS
	jumpifmove MOVE_MAKEITRAIN MakeItRainBS
	jumpifmove MOVE_SPINOUT SpinOutBS
	setmoveeffect MOVE_EFFECT_ATK_DEF_DOWN | MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_CERTAIN
	goto BS_STANDARD_HIT

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

CloseCombatBS:
	attackcanceler
	accuracycheck BS_MOVE_MISSED 0x0
	call STANDARD_DAMAGE
	jumpifmovehadnoeffect BS_MOVE_FAINT
	jumpifstat BANK_ATTACKER GREATERTHAN STAT_DEF STAT_MIN CC_LowerDef
	jumpifstat BANK_ATTACKER EQUALS STAT_SPDEF STAT_MIN BS_MOVE_FAINT

CC_LowerDef:
	setbyte STAT_ANIM_PLAYED 0x0
	playstatchangeanimation BANK_ATTACKER, STAT_ANIM_DEF | STAT_ANIM_SPDEF, STAT_ANIM_DOWN | STAT_ANIM_IGNORE_ABILITIES
CC_SkipTo:
	setstatchanger STAT_DEF | DECREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN CC_LowerSpDef
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 CC_LowerSpDef
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND

CC_LowerSpDef:
	setstatchanger STAT_SPDEF | DECREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN BS_MOVE_FAINT
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 BS_MOVE_FAINT
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	goto BS_MOVE_FAINT

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

MakeItRainBS:
	attackcanceler
    accuracycheck BS_MOVE_MISSED 0x0
    call STANDARD_DAMAGE
    jumpifstat BANK_ATTACKER GREATERTHAN STAT_SPATK STAT_MIN MIT_LowerSpAtk
    setmoveeffect MOVE_EFFECT_PAYDAY
    seteffectprimary
    goto BS_MOVE_END


MIT_LowerSpAtk:
    setbyte STAT_ANIM_PLAYED 0x0
	playstatchangeanimation BANK_ATTACKER, STAT_ANIM_SPATK, STAT_ANIM_DOWN | STAT_ANIM_IGNORE_ABILITIES
	setstatchanger STAT_SPATK | DECREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN BS_MOVE_FAINT
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 BS_MOVE_FAINT
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	setmoveeffect MOVE_EFFECT_PAYDAY
    seteffectprimary
    goto BS_MOVE_FAINT

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

HammerArmBS:
	setmoveeffect MOVE_EFFECT_SPD_MINUS_1 | MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_CERTAIN
	goto BS_STANDARD_HIT

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

ClangingScalesBS:
	setmoveeffect MOVE_EFFECT_DEF_MINUS_1 | MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_CERTAIN
	goto BS_STANDARD_HIT

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

VCreateBS:
	attackcanceler
	accuracycheck BS_MOVE_MISSED 0x0
	call STANDARD_DAMAGE
	jumpifmovehadnoeffect BS_MOVE_FAINT
	jumpifstat BANK_ATTACKER GREATERTHAN STAT_SPD STAT_MIN VC_LowerSpd
	jumpifstat BANK_ATTACKER GREATERTHAN STAT_DEF STAT_MIN VC_LowerSpd
	jumpifstat BANK_ATTACKER EQUALS STAT_SPDEF STAT_MIN BS_MOVE_FAINT

VC_LowerSpd:
	setbyte STAT_ANIM_PLAYED 0x0
	playstatchangeanimation BANK_ATTACKER, STAT_ANIM_DEF | STAT_ANIM_SPDEF | STAT_ANIM_SPD, STAT_ANIM_DOWN | STAT_ANIM_IGNORE_ABILITIES
	setstatchanger STAT_SPD | DECREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN VC_LowerDef
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 VC_LowerDef
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND

VC_LowerDef:
	setbyte STAT_ANIM_PLAYED 0x0
	goto CC_SkipTo

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

HyperspaceHoleBS:
	attackcanceler
	goto FeintSkipBS

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

HyperspaceFuryBS:
	attackcanceler
	setmoveeffect MOVE_EFFECT_DEF_MINUS_1 | MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_CERTAIN
	goto FeintSkipBS

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SpinOutBS:
	setmoveeffect MOVE_EFFECT_SPD_MINUS_2 | MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_CERTAIN
	goto BS_STANDARD_HIT

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_183_MagicCoat
BS_183_MagicCoat:
	attackcanceler
	attackstringnoprotean
	ppreduce
	setmagiccoat FAILED
	tryactivateprotean
	attackanimation
	waitanimation
	printstring 0xBD
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_184_Recycle
BS_184_Recycle:
	attackcanceler
	jumpifnotmove MOVE_BELCH RecycleBS
	callasm BelchFunction
	goto 0x81D6927
	
RecycleBS:
	jumpifcounter BANK_ATTACKER INCINERATE_COUNTERS NOTEQUALS 0x0 FAILED_PRE
	attackstringnoprotean
	ppreduce
	recycleitem FAILED
	tryactivateprotean
	attackanimation
	waitanimation
	printstring 0x140
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_185_Blank @;Was Revenge
BS_185_Blank:
	goto BS_STANDARD_HIT

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_186_BrickBreak
BS_186_BrickBreak:
	setmoveeffect MOVE_EFFECT_BREAK_SCREENS
	removereflectlightscreen @;For the anim only
	goto BS_STANDARD_HIT

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_187_Yawn
BS_187_Yawn:
	attackcanceler
	attackstring
	ppreduce
	jumpifbehindsubstitute BANK_TARGET FAILED
	cansetyawn BS_StatusMoveFail
	accuracycheck FAILED 0x0
	callasm ActuallySetYawn
	attackanimation
	waitanimation
	printstring 0xB6
	waitmessage 0x40
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_188_KnockOff
BS_188_KnockOff:
	jumpifmove MOVE_CORROSIVEGAS CorrosiveGasBS
	setmoveeffect MOVE_EFFECT_KNOCK_OFF
	goto BS_STANDARD_HIT

CorrosiveGasBS:
	attackcanceler
	attackstringnoprotean
	ppreduce
	jumpifbehindsubstitute BANK_TARGET NOEFFECT
	jumpifability BANK_TARGET ABILITY_STICKYHOLD BattleScript_ProtectedByAbility
	tryactivateprotean
	callasm TryFailCorrosiveGas
	accuracycheck BS_MOVE_MISSED + 2 0x0
	attackanimation
	waitanimation
	callasm CorrodeItem
	setword BATTLE_STRING_LOADER gText_CorrodedItem
	printstring 0x184
	waitmessage DELAY_1SECOND
	call 0x81D92DC @;BattleScript_WeatherFormChanges - In case of Utility Umbrella
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_189_Endeavor
BS_189_Endeavor:
	attackcanceler
	attackstring
	ppreduce
	setdamagetohealthdifference FAILED
	copyarray HP_DEALT DAMAGE_LOC 0x4
	accuracycheck BS_MOVE_MISSED + 2 0x0
	typecalc2
	jumpifmovehadnoeffect BS_HIT_FROM_ATTACKANIMATION
	bicbyte OUTCOME OUTCOME_SUPER_EFFECTIVE | OUTCOME_NOT_VERY_EFFECTIVE
	copyarray DAMAGE_LOC HP_DEALT 0x4
	adjustsetdamage
	goto BS_HIT_FROM_ATTACKANIMATION @;Hit from attackanimation

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_190_Blank @;Was Eruption
BS_190_Blank: @;Was Eruption
	goto BS_STANDARD_HIT

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_191_SkillSwap
BS_191_SkillSwap:
	jumpifmove MOVE_WORRYSEED WorrySeedBS
	jumpifmove MOVE_GASTROACID GastroAcidBS
	jumpifmove MOVE_ENTRAINMENT EntrainmentBS
	jumpifmove MOVE_COREENFORCER CoreEnforcerBS
	jumpifmove MOVE_SIMPLEBEAM SimpleBeamBS
	jumpifmove MOVE_DOODLE EntrainmentBS
	
SkillSwapBS:
	attackcanceler
	jumpifbehindsubstitute BANK_TARGET FAILED_PRE
	accuracycheck BS_MOVE_MISSED 0x0
	attackstring
	ppreduce
	callasm FailMoveIfAura
	abilityswap FAILED
	attackanimation
	waitanimation
	copyarray BATTLE_SCRIPTING_BANK USER_BANK 0x1
	call BattleScript_AbilityPopUp
	pause DELAY_HALFSECOND
	call BattleScript_AbilityPopUpRevert

	copyarray BATTLE_SCRIPTING_BANK TARGET_BANK 0x1
	call BattleScript_AbilityPopUp
	pause DELAY_HALFSECOND
	call BattleScript_AbilityPopUpRevert
	
	printstring 0xB8
	waitmessage DELAY_1SECOND
	tryactivateswitchinability BANK_ATTACKER
	callasm RestoreOriginalAttackerAndTarget
	tryactivateswitchinability BANK_TARGET
	callasm RestoreOriginalAttackerAndTarget
	goto BS_MOVE_END

GastroAcidBS:
	attackcanceler
	jumpifbehindsubstitute BANK_TARGET FAILED_PRE
	accuracycheck BS_MOVE_MISSED 0x0
	attackstringnoprotean
	ppreduce
	callasm AbilityChangeBSFunc
	tryactivateprotean
	attackanimation
	waitanimation
	printstring 0x184
	waitmessage DELAY_1SECOND
	copybyte BATTLE_SCRIPTING_BANK TARGET_BANK
	call BattleScript_TryRemoveIllusion
	callasm TryRemovePrimalWeatherAfterAbilityChange
	call 0x81D92DC @;Try to revert Cherrim and Castform
	goto BS_MOVE_END

EntrainmentBS:
	attackcanceler
	jumpifbehindsubstitute BANK_TARGET FAILED_PRE
	accuracycheck BS_MOVE_MISSED 0x0
	attackstringnoprotean
	ppreduce
	callasm FailMoveIfAura
	goto WorrySeedBS_ChangeAbility

WorrySeedBS:
SimpleBeamBS:
	attackcanceler
	jumpifbehindsubstitute BANK_TARGET FAILED_PRE
	accuracycheck BS_MOVE_MISSED 0x0
	attackstringnoprotean
	ppreduce
WorrySeedBS_ChangeAbility:
	callasm AbilityChangeBSFunc
	tryactivateprotean
	attackanimation
	waitanimation

	playanimation BANK_TARGET ANIM_LOAD_ABILITY_POP_UP 0x0
	call BattleScript_AbilityPopUpRevert
	copyarray BATTLE_SCRIPTING_BANK TARGET_BANK 0x1
	callasm EntrainmentSetCorrectTookAbilityFrom
	call BattleScript_AbilityPopUp
	pause DELAY_HALFSECOND
	call BattleScript_AbilityPopUpRevert

	printstring 0x184
	waitmessage DELAY_1SECOND
	copyarray BATTLE_SCRIPTING_BANK TARGET_BANK 0x1
	call BattleScript_TryRemoveIllusion
	callasm TryRemovePrimalWeatherAfterAbilityChange
	call 0x81D92DC @;Try to revert Cherrim and Castform
	callasm RestoreOriginalAttackerAndTarget
	tryactivateswitchinability BANK_TARGET
	callasm RestoreOriginalAttackerAndTarget
	goto BS_MOVE_END

CoreEnforcerBS:
	setmoveeffect MOVE_EFFECT_SUPPRESS_ABILITY | MOVE_EFFECT_CERTAIN
	goto BS_STANDARD_HIT

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_192_Imprison
BS_192_Imprison:
	attackcanceler
	attackstringnoprotean
	ppreduce
	imprisoneffect FAILED
	tryactivateprotean
	attackanimation
	waitanimation
	printstring 0xB9 @;STRINGID_PKMNSEALEDOPPONENTMOVE
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_193_Refresh
BS_193_Refresh:
	attackcanceler
	jumpifmove MOVE_PSYCHOSHIFT PsychoShiftBS
	
RefreshBS:
	attackstringnoprotean
	ppreduce
	cureifburnedparalysedorpoisoned FAILED
	tryactivateprotean
	attackanimation
	waitanimation
	printstring 0xA7 @;STRINGID_PKMNSTATUSNORMAL
	waitmessage DELAY_1SECOND
	refreshhpbar BANK_ATTACKER
	goto BS_MOVE_END
	
PsychoShiftBS:
	jumpifbehindsubstitute BANK_TARGET FAILED_PRE
	jumpifstatus BANK_TARGET STATUS_ANY FAILED_PRE
	accuracycheck FAILED_PRE 0x0
	attackstringnoprotean
	ppreduce
	jumpifsideaffecting BANK_TARGET SIDE_SAFEGUARD 0x81D8B39 @;Protected By Safeguard
	callasm LoadStatustoPsychoShiftTransfer
	cureprimarystatus BANK_ATTACKER FAILED
	tryactivateprotean
	attackanimation
	waitanimation
	seteffectprimary
	refreshhpbar BANK_ATTACKER
	setword BATTLE_STRING_LOADER PsychoShiftString
	printstring 0x184
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_194_Grudge
BS_194_Grudge:
	attackcanceler
	attackstringnoprotean
	ppreduce
	setgrudge FAILED
	tryactivateprotean
	attackanimation
	waitanimation
	printstring 0xBB
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_195_Snatch
BS_195_Snatch:
	attackcanceler
	attackstringnoprotean
	ppreduce
	setstealstatchange FAILED
	tryactivateprotean
	attackanimation
	waitanimation
	pause DELAY_HALFSECOND
	printstring 0xBF
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_196_Blank @;Was Low Kick
BS_196_Blank:
	goto BS_STANDARD_HIT
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_197_SecretPower
BS_197_SecretPower:
	secretpower
	goto BS_STANDARD_HIT

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_198_RaiseUserAtkSpAtk @;Was 33% Recoil
BS_198_RaiseUserAtkSpAtk:
	attackcanceler
	attackstring
	ppreduce
	jumpifmove MOVE_ROTOTILLER RototillerBS
	jumpifmove MOVE_GEARUP GearUpBS

@;WorkUpBS
	jumpifstat BANK_ATTACKER LESSTHAN STAT_ATK STAT_MAX WorkUp_RaiseAtk
	jumpifstat BANK_ATTACKER EQUALS STAT_SPATK STAT_MAX BattleScript_CantRaiseMultipleStats

WorkUp_RaiseAtk:
	attackanimation
	waitanimation
	setbyte STAT_ANIM_PLAYED 0x0
	playstatchangeanimation BANK_ATTACKER, STAT_ANIM_ATK | STAT_ANIM_SPATK, STAT_ANIM_UP | STAT_ANIM_IGNORE_ABILITIES
	setstatchanger STAT_ATK | INCREASE_1
	callasm ModifyGrowthInSun
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN WorkUp_RaiseSpAtk
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 WorkUp_RaiseSpAtk
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND

WorkUp_RaiseSpAtk:
	setstatchanger STAT_SPATK | INCREASE_1
	callasm ModifyGrowthInSun
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN BS_MOVE_END
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 BS_MOVE_END
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_199_RaiseUserAtkAcc @;Was Teeter Dance
BS_199_RaiseUserAtkAcc:
	attackcanceler
	attackstring
	ppreduce
	jumpifstat BANK_TARGET LESSTHAN STAT_ATK STAT_MAX RaiseUserAtkAcc_Atk
	jumpifstat BANK_TARGET EQUALS STAT_ACC STAT_MAX BattleScript_CantRaiseMultipleStats

RaiseUserAtkAcc_Atk:
	attackanimation
	waitanimation
	setbyte STAT_ANIM_PLAYED 0x0
	playstatchangeanimation BANK_ATTACKER, STAT_ANIM_ATK | STAT_ANIM_ACC, STAT_ANIM_UP | STAT_ANIM_IGNORE_ABILITIES
	setstatchanger STAT_ATK | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN RaiseUserAtkAcc_Acc
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 RaiseUserAtkAcc_Acc
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND

RaiseUserAtkAcc_Acc:
	setstatchanger STAT_ACC | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN BS_MOVE_END
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 BS_MOVE_END
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_200_RaiseUserDefSpeed @;Was Blaze Kick
BS_200_RaiseUserDefSpeed:
	attackcanceler
	attackstring
	ppreduce
	jumpifstat BANK_TARGET LESSTHAN STAT_DEF STAT_MAX RaiseUserDefSpeed_Def
	jumpifstat BANK_TARGET EQUALS STAT_SPD STAT_MAX BattleScript_CantRaiseMultipleStats

RaiseUserDefSpeed_Def:
	attackanimation
	waitanimation
	setbyte STAT_ANIM_PLAYED 0x0
	playstatchangeanimation BANK_ATTACKER, STAT_ANIM_DEF | STAT_ANIM_SPD, STAT_ANIM_UP | STAT_ANIM_IGNORE_ABILITIES
	setstatchanger STAT_DEF | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN RaiseUserDefSpeed_Evasion
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 RaiseUserDefSpeed_Evasion
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND

RaiseUserDefSpeed_Evasion:
	setstatchanger STAT_SPD | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN BS_MOVE_END
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 BS_MOVE_END
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_201_MudSport
.global BS_210_WaterSport
BS_201_MudSport:
BS_210_WaterSport:
	attackcanceler
	attackstringnoprotean
	ppreduce
	settypebasedhalvers FAILED
	tryactivateprotean
	attackanimation
	waitanimation
	printfromtable 0x83FE5E0 @;gSportsUsedStringIds
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_202_Blank
BS_202_Blank:
	goto BS_STANDARD_HIT

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

RototillerBS:
	pause DELAY_HALFSECOND
	setbyte BATTLE_COMMUNICATION 0x0
BattleScript_RototillerLoop:
	flowershieldlooper 0x0 BattleScript_RototillerStatBoost BattleScript_RototillerDidntWork
	goto BS_MOVE_END

BattleScript_RototillerStatBoost:
	jumpifstatcanberaised BANK_TARGET STAT_ATK Rototiller_Atk
	jumpifstatcanberaised BANK_TARGET STAT_SPATK Rototiller_Atk
	goto RototillerStatsWontGoHigher

Rototiller_Atk:
	attackanimation
	waitanimation
	setbyte ANIM_TARGETS_HIT 0x1
	setbyte STAT_ANIM_PLAYED 0x0
	playstatchangeanimation BANK_TARGET, STAT_ANIM_ATK | STAT_ANIM_SPATK, STAT_ANIM_UP | STAT_ANIM_IGNORE_ABILITIES
	setstatchanger STAT_ATK | INCREASE_1
	statbuffchange STAT_TARGET | STAT_BS_PTR Rototiller_SpAtk
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 Rototiller_SpAtk
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	
Rototiller_SpAtk:
	setstatchanger STAT_SPATK | INCREASE_1
	statbuffchange STAT_TARGET | STAT_BS_PTR BattleScript_RototillerLoop
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 BattleScript_RototillerLoop
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	goto BattleScript_RototillerLoop

RototillerStatsWontGoHigher:
	setbyte MULTISTRING_CHOOSER 0x3
	swapattackerwithtarget @;So the correct string plays
	printfromtable gFlowerShieldStringIds
	swapattackerwithtarget
	waitmessage DELAY_1SECOND
	goto BattleScript_RototillerLoop

BattleScript_RototillerDidntWork:
	jumpifbyte EQUALS MULTISTRING_CHOOSER 4 BattleScript_RototillerDidntWork_ShowAbility
	printfromtable gFlowerShieldStringIds
	waitmessage DELAY_1SECOND
	goto BattleScript_RototillerLoop

BattleScript_RototillerDidntWork_ShowAbility:
	call BattleScript_AbilityPopUp
	printfromtable gFlowerShieldStringIds
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	goto BattleScript_RototillerLoop

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

GearUpBS:
	pause DELAY_HALFSECOND
	setbyte BATTLE_COMMUNICATION 0x0
BattleScript_GearUpLoop:
	flowershieldlooper 0x1 BattleScript_GearUpStatBoost BattleScript_GearUpDidntWork
	goto BS_MOVE_END

BattleScript_GearUpStatBoost:
	jumpifstatcanberaised BANK_TARGET STAT_ATK GearUp_Atk
	jumpifstatcanberaised BANK_TARGET STAT_SPATK GearUp_Atk
	goto GearUpStatsWontGoHigher

GearUp_Atk:
	attackanimation
	waitanimation
	setbyte ANIM_TARGETS_HIT 0x1
	setbyte STAT_ANIM_PLAYED 0x0
	playstatchangeanimation BANK_TARGET, STAT_ANIM_ATK | STAT_ANIM_SPATK, STAT_ANIM_UP | STAT_ANIM_IGNORE_ABILITIES
	setstatchanger STAT_ATK | INCREASE_1
	statbuffchange STAT_TARGET | STAT_BS_PTR GearUp_SpAtk
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 GearUp_SpAtk
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	
GearUp_SpAtk:
	setstatchanger STAT_SPATK | INCREASE_1
	statbuffchange STAT_TARGET | STAT_BS_PTR BattleScript_GearUpLoop
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 BattleScript_GearUpLoop
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	goto BattleScript_GearUpLoop

GearUpStatsWontGoHigher:
	setbyte MULTISTRING_CHOOSER 0x3
	swapattackerwithtarget @;So the correct string plays
	printfromtable gFlowerShieldStringIds
	swapattackerwithtarget
	waitmessage DELAY_1SECOND
	goto BattleScript_GearUpLoop

BattleScript_GearUpDidntWork:
	printfromtable gFlowerShieldStringIds
	waitmessage DELAY_1SECOND
	goto BattleScript_GearUpLoop

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_202_VenomDrench
BS_202_VenomDrench:
	attackcanceler
	jumpifbehindsubstitute BANK_TARGET FAILED_PRE
	attackstring
	ppreduce
	accuracycheck BS_MOVE_MISSED + 2 0x0
	setbyte STAT_ANIM_PLAYED 0x0
	jumpifstatus BANK_TARGET STATUS_POISON_ANY DoVenomDrench
	goto NOEFFECT

DoVenomDrench:
	jumpifstatcanbelowered BANK_TARGET STAT_ATK VenomDrench_Atk
	jumpifstatcanbelowered BANK_TARGET STAT_SPATK VenomDrench_Atk
	jumpifstatcanbelowered BANK_TARGET STAT_SPD VenomDrench_Atk
	pause 0x10
	goto VenomDrench_SkipAnim

VenomDrench_Atk:
	attackanimation
	waitanimation
	playstatchangeanimation BANK_TARGET, STAT_ANIM_ATK | STAT_ANIM_SPATK | STAT_ANIM_SPD, STAT_ANIM_DOWN | STAT_ANIM_ONLY_TRIPLE
	
VenomDrench_SkipAnim:
	setbyte FORM_COUNTER 0x0
	playstatchangeanimation BANK_TARGET, STAT_ANIM_ATK | STAT_ANIM_SPATK, STAT_ANIM_DOWN | STAT_ANIM_ONLY_MULTIPLE
	playstatchangeanimation BANK_TARGET, STAT_ANIM_ATK | STAT_ANIM_SPD, STAT_ANIM_DOWN | STAT_ANIM_ONLY_MULTIPLE
	playstatchangeanimation BANK_TARGET, STAT_ANIM_ATK, STAT_ANIM_DOWN
	setstatchanger STAT_ATK | DECREASE_1
	statbuffchange STAT_TARGET | STAT_BS_PTR VenomDrench_SpAtk
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x3 VenomDrench_SpAtk
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x4 BS_MOVE_END
	printfromtable gStatDownStringIds
	waitmessage DELAY_1SECOND

VenomDrench_SpAtk:
	playstatchangeanimation BANK_TARGET, STAT_ANIM_SPATK | STAT_ANIM_SPD, STAT_ANIM_DOWN | STAT_ANIM_ONLY_MULTIPLE
	playstatchangeanimation BANK_TARGET, STAT_ANIM_SPATK, STAT_ANIM_DOWN
	setstatchanger STAT_SPATK | DECREASE_1
	statbuffchange STAT_TARGET | STAT_BS_PTR VenomDrench_Spd
	jumpifbyte GREATERTHAN MULTISTRING_CHOOSER 0x2 VenomDrench_Spd
	printfromtable gStatDownStringIds
	waitmessage DELAY_1SECOND

VenomDrench_Spd:
	playstatchangeanimation BANK_TARGET, STAT_ANIM_SPD, STAT_ANIM_DOWN
	setstatchanger STAT_SPD | DECREASE_1
	statbuffchange STAT_TARGET | STAT_BS_PTR BS_MOVE_END
	jumpifbyte GREATERTHAN MULTISTRING_CHOOSER 0x2 BS_MOVE_END
	printfromtable gStatDownStringIds
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_203_PlayNice
BS_203_PlayNice:
	attackcanceler
	jumpifbehindsubstitute BANK_TARGET FAILED_PRE
	attackstring
	ppreduce
	accuracycheck BS_MOVE_MISSED + 2 0x0
	setbyte STAT_ANIM_PLAYED 0x0
	jumpifstatcanbelowered BANK_TARGET STAT_ATK PlayNice_Atk
	jumpifstatcanbelowered BANK_TARGET STAT_SPATK PlayNice_Atk
	pause 0x10
	goto PlayNice_SkipAnim

PlayNice_Atk:
	attackanimation
	waitanimation
	playstatchangeanimation BANK_TARGET, STAT_ANIM_ATK | STAT_ANIM_SPATK, STAT_ANIM_DOWN | STAT_ANIM_ONLY_MULTIPLE
	
PlayNice_SkipAnim:
	setbyte FORM_COUNTER 0x0
	playstatchangeanimation BANK_TARGET, STAT_ANIM_ATK STAT_ANIM_DOWN
	setstatchanger STAT_ATK | DECREASE_1
	statbuffchange STAT_TARGET | STAT_BS_PTR PlayNice_SpAtk
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x3 PlayNice_SpAtk
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x4 BS_MOVE_END
	printfromtable gStatDownStringIds
	waitmessage DELAY_1SECOND

PlayNice_SpAtk:
	playstatchangeanimation BANK_TARGET, STAT_ANIM_SPATK STAT_ANIM_DOWN
	setstatchanger STAT_SPATK | DECREASE_1
	statbuffchange STAT_TARGET | STAT_BS_PTR BS_MOVE_END
	jumpifbyte GREATERTHAN MULTISTRING_CHOOSER 0x2 BS_MOVE_END
	printfromtable gStatDownStringIds
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_204_Overheat
BS_204_Overheat:
	setmoveeffect MOVE_EFFECT_SP_ATK_TWO_DOWN | MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_CERTAIN
	goto BS_STANDARD_HIT

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_205_Tickle
BS_205_Tickle:
	attackcanceler
	jumpifbehindsubstitute BANK_TARGET FAILED_PRE
	accuracycheck BS_MOVE_MISSED 0x0
	attackstring
	ppreduce
	setbyte STAT_ANIM_PLAYED 0x0
	jumpifstatcanbelowered BANK_TARGET STAT_ATK Tickle_Atk
	jumpifstatcanbelowered BANK_TARGET STAT_DEF Tickle_Atk
	pause 0x10
	goto Tickle_SkipAnim

Tickle_Atk:
	attackanimation
	waitanimation
	playstatchangeanimation BANK_TARGET, STAT_ANIM_ATK | STAT_ANIM_DEF, STAT_ANIM_DOWN | STAT_ANIM_ONLY_MULTIPLE

Tickle_SkipAnim:
	setbyte FORM_COUNTER 0x0
	playstatchangeanimation BANK_TARGET, STAT_ANIM_ATK STAT_ANIM_DOWN
	setstatchanger STAT_ATK | DECREASE_1
	statbuffchange STAT_TARGET | STAT_BS_PTR Tickle_Def
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x3 Tickle_Def
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x4 BS_MOVE_END
	printfromtable gStatDownStringIds
	waitmessage DELAY_1SECOND

Tickle_Def:
	playstatchangeanimation BANK_TARGET, STAT_ANIM_DEF STAT_ANIM_DOWN
	setstatchanger STAT_DEF | DECREASE_1
	statbuffchange STAT_TARGET | STAT_BS_PTR BS_MOVE_END
	jumpifbyte GREATERTHAN MULTISTRING_CHOOSER 0x2 BS_MOVE_END
	printfromtable gStatDownStringIds
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_206_CosmicPower
BS_206_CosmicPower:
	attackcanceler
	attackstring
	ppreduce
	jumpifmove MOVE_MAGNETICFLUX MagneticFluxBS
	jumpifstat BANK_ATTACKER LESSTHAN STAT_DEF STAT_MAX CosmicPower_Def
	jumpifstat BANK_ATTACKER EQUALS STAT_SPDEF STAT_MAX BattleScript_CantRaiseMultipleStats

CosmicPower_Def:
	attackanimation
	waitanimation
	setbyte STAT_ANIM_PLAYED 0x0
	playstatchangeanimation BANK_ATTACKER, STAT_ANIM_DEF | STAT_ANIM_SPDEF, STAT_ANIM_UP | STAT_ANIM_IGNORE_ABILITIES
	setstatchanger STAT_DEF | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN CosmicPower_SpDef
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 CosmicPower_SpDef
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND

CosmicPower_SpDef:
	setstatchanger STAT_SPDEF | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN BS_MOVE_END
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 BS_MOVE_END
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

MagneticFluxBS:
	pause DELAY_HALFSECOND
	setbyte BATTLE_COMMUNICATION 0x0
BattleScript_MagneticFluxLoop:
	flowershieldlooper 0x1 BattleScript_MagneticFluxStatBoost BattleScript_MagneticFluxDidntWork
	goto BS_MOVE_END

BattleScript_MagneticFluxStatBoost:
	jumpifstatcanberaised BANK_TARGET STAT_DEF MagneticFlux_Def
	jumpifstatcanberaised BANK_TARGET STAT_SPDEF MagneticFlux_Def
	goto MagneticFluxStatsWontGoHigher

MagneticFlux_Def:
	attackanimation
	waitanimation
	setbyte ANIM_TARGETS_HIT 0x1
	setbyte STAT_ANIM_PLAYED 0x0
	playstatchangeanimation BANK_TARGET, STAT_ANIM_DEF | STAT_ANIM_SPDEF, STAT_ANIM_UP | STAT_ANIM_IGNORE_ABILITIES
	setstatchanger STAT_DEF | INCREASE_1
	statbuffchange STAT_TARGET | STAT_BS_PTR MagneticFlux_SpDef
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 MagneticFlux_SpDef
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND

MagneticFlux_SpDef:
	setstatchanger STAT_SPDEF | INCREASE_1
	statbuffchange STAT_TARGET | STAT_BS_PTR BattleScript_MagneticFluxLoop
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 BattleScript_MagneticFluxLoop
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	goto BattleScript_MagneticFluxLoop

MagneticFluxStatsWontGoHigher:
	setbyte MULTISTRING_CHOOSER 0x3
	swapattackerwithtarget @;So the correct string plays
	printfromtable gFlowerShieldStringIds
	swapattackerwithtarget
	waitmessage DELAY_1SECOND
	goto BattleScript_MagneticFluxLoop

BattleScript_MagneticFluxDidntWork:
	printfromtable gFlowerShieldStringIds
	waitmessage DELAY_1SECOND
	goto BattleScript_MagneticFluxLoop

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_207_ExtremeEvoBoost @;Was Sky Uppercut
BS_207_ExtremeEvoBoost:
	attackcanceler
	jumpifmove MOVE_NORETREAT NoRetreatBS
	attackstring
	ppreduce
	jumpifmove MOVE_CLANGOROUSSOUL ClangorousSoulBS
	jumpifstat BANK_ATTACKER LESSTHAN STAT_ATK STAT_MAX ExtremeEvoboost_Atk
	jumpifstat BANK_ATTACKER LESSTHAN STAT_DEF STAT_MAX ExtremeEvoboost_Atk
	jumpifstat BANK_ATTACKER LESSTHAN STAT_SPD STAT_MAX ExtremeEvoboost_Atk
	jumpifstat BANK_ATTACKER LESSTHAN STAT_SPATK STAT_MAX ExtremeEvoboost_Atk
	jumpifstat BANK_ATTACKER EQUALS STAT_SPDEF STAT_MAX BattleScript_CantRaiseMultipleStats

ExtremeEvoboost_Atk:
	attackanimation
	waitanimation
	setbyte STAT_ANIM_PLAYED 0x0
	playstatchangeanimation BANK_ATTACKER, STAT_ANIM_ATK | STAT_ANIM_DEF | STAT_ANIM_SPD | STAT_ANIM_SPATK | STAT_ANIM_SPDEF, STAT_ANIM_UP | STAT_ANIM_IGNORE_ABILITIES | STAT_ANIM_BY_TWO
	setstatchanger STAT_ATK | INCREASE_2
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN ExtremeEvoboost_Def
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 ExtremeEvoboost_Def
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND

ExtremeEvoboost_Def:
	setstatchanger STAT_DEF | INCREASE_2
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN ExtremeEvoboost_SpAtk
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 ExtremeEvoboost_SpAtk
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND

ExtremeEvoboost_SpAtk:
	setstatchanger STAT_SPATK | INCREASE_2
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN ExtremeEvoboost_SpDef
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 ExtremeEvoboost_SpDef
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND

ExtremeEvoboost_SpDef:
	setstatchanger STAT_SPDEF | INCREASE_2
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN ExtremeEvoboost_Spd
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 ExtremeEvoboost_Spd
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND

ExtremeEvoboost_Spd:
	setstatchanger STAT_SPD | INCREASE_2
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN BS_MOVE_END
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 BS_MOVE_END
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

ClangorousSoulBS:
	callasm FailClangorousSoulIfLowHP
	jumpifstat BANK_ATTACKER LESSTHAN STAT_ATK STAT_MAX ClangorusSoul_Atk
	jumpifstat BANK_ATTACKER LESSTHAN STAT_DEF STAT_MAX ClangorusSoul_Atk
	jumpifstat BANK_ATTACKER LESSTHAN STAT_SPD STAT_MAX ClangorusSoul_Atk
	jumpifstat BANK_ATTACKER LESSTHAN STAT_SPATK STAT_MAX ClangorusSoul_Atk
	jumpifstat BANK_ATTACKER EQUALS STAT_SPDEF STAT_MAX BattleScript_CantRaiseMultipleStats

ClangorusSoul_Atk:
	attackanimation
	waitanimation
	call BattleScript_AllStatsUp
	orword HIT_MARKER HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_NON_ATTACK_DMG
	setdamagetobankhealthfraction BANK_ATTACKER 3 0x0 @;33 % of Base Max HP
	graphicalhpupdate BANK_ATTACKER
	datahpupdate BANK_ATTACKER
	faintpokemon BANK_ATTACKER 0x0 0x0
	goto BS_MOVE_END

NoRetreatBS:
	callasm FailIfTrappedByNoRetreat
	attackstring
	ppreduce
	attackanimation
	waitanimation
	call BattleScript_AllStatsUp
	setmoveeffect MOVE_EFFECT_PREVENT_ESCAPE | MOVE_EFFECT_AFFECTS_USER
	seteffectprimary
	jumpifsecondarystatus BANK_ATTACKER STATUS2_TRAPPED PrintNoRetreatMessage @;May not be affected if Ghost or already trapped
	goto BS_MOVE_END

PrintNoRetreatMessage:
	setword BATTLE_STRING_LOADER gText_NoRetreatTrapped
	printstring 0x184
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_208_BulkUp
BS_208_BulkUp:
	attackcanceler
	attackstring
	ppreduce
	jumpifmove MOVE_COIL CoilBS
	jumpifmove MOVE_VICTORYDANCE VictoryDanceBS
	
BulkUpBS:
	jumpifstat BANK_TARGET LESSTHAN STAT_ATK STAT_MAX BulkUp_Atk
	jumpifstat BANK_TARGET EQUALS STAT_DEF STAT_MAX BattleScript_CantRaiseMultipleStats

BulkUp_Atk:
	attackanimation
	waitanimation
	setbyte STAT_ANIM_PLAYED 0x0
	playstatchangeanimation BANK_ATTACKER, STAT_ANIM_ATK | STAT_ANIM_DEF, STAT_ANIM_UP | STAT_ANIM_IGNORE_ABILITIES
	setstatchanger STAT_ATK | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN BulkUp_Def
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 BulkUp_Def
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND

BulkUp_Def:
	setstatchanger STAT_DEF | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN BS_MOVE_END
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 BS_MOVE_END
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

CoilBS:
	jumpifstat BANK_ATTACKER LESSTHAN STAT_ATK STAT_MAX Coil_Atk
	jumpifstat BANK_ATTACKER LESSTHAN STAT_DEF STAT_MAX Coil_Atk
	jumpifstat BANK_ATTACKER EQUALS STAT_ACC STAT_MAX BattleScript_CantRaiseMultipleStats

Coil_Atk:
	attackanimation
	waitanimation
	setbyte STAT_ANIM_PLAYED 0x0
	playstatchangeanimation BANK_ATTACKER, STAT_ANIM_ATK | STAT_ANIM_DEF | STAT_ANIM_ACC, STAT_ANIM_UP | STAT_ANIM_IGNORE_ABILITIES
	setstatchanger STAT_ATK | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN Coil_Def
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 Coil_Def
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND

Coil_Def:
	setstatchanger STAT_DEF | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN Coil_Acc
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 Coil_Acc
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND

Coil_Acc:
	setstatchanger STAT_ACC | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN BS_MOVE_END
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 BS_MOVE_END
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

VictoryDanceBS: @;Physical Quiver Dance
	jumpifstat BANK_ATTACKER LESSTHAN STAT_ATK STAT_MAX VictoryDance_Atk
	jumpifstat BANK_ATTACKER LESSTHAN STAT_DEF STAT_MAX VictoryDance_Atk
	jumpifstat BANK_ATTACKER EQUALS STAT_SPD STAT_MAX BattleScript_CantRaiseMultipleStats

VictoryDance_Atk:
	attackanimation
	waitanimation
	setbyte STAT_ANIM_PLAYED 0x0
	playstatchangeanimation BANK_ATTACKER, STAT_ANIM_ATK | STAT_ANIM_DEF | STAT_ANIM_SPD, STAT_ANIM_UP | STAT_ANIM_IGNORE_ABILITIES
	setstatchanger STAT_ATK | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN VictoryDance_Def
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 VictoryDance_Def
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND

VictoryDance_Def:
	setstatchanger STAT_DEF | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN VictoryDance_Speed
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 VictoryDance_Speed
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND

VictoryDance_Speed:
	setstatchanger STAT_SPD | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN BS_MOVE_END
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 BS_MOVE_END
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_209_BadPoisonChance
BS_209_BadPoisonChance:
	setmoveeffect MOVE_EFFECT_TOXIC
	callasm TrySetPoisonPuppeterEffect
	goto BS_STANDARD_HIT
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_211_CalmMind
BS_211_CalmMind:
	attackcanceler
	jumpifmove MOVE_QUIVERDANCE QuiverDanceBS
	jumpifmove MOVE_GEOMANCY GeomancyBS
	jumpifmove MOVE_TAKEHEART TakeHeartBS

CalmMindBS:
	attackstring
	ppreduce
CalmMindBS_CheckStats:
	jumpifstat BANK_TARGET LESSTHAN STAT_SPATK STAT_MAX CalmMind_SpAtk
	jumpifstat BANK_TARGET EQUALS STAT_SPDEF STAT_MAX BattleScript_CantRaiseMultipleStats

CalmMind_SpAtk:
	attackanimation
	waitanimation
CalmMind_SpAtk_SkipAttackAnim:
	setbyte STAT_ANIM_PLAYED 0x0
	playstatchangeanimation BANK_ATTACKER, STAT_ANIM_SPATK | STAT_ANIM_SPDEF, STAT_ANIM_UP | STAT_ANIM_IGNORE_ABILITIES
	setstatchanger STAT_SPATK | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN CalmMind_SpDef
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 CalmMind_SpDef
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND

CalmMind_SpDef:
	setstatchanger STAT_SPDEF | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN BS_MOVE_END
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 BS_MOVE_END
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

TakeHeartBS: @;Also heals status conditions
	attackstring
	ppreduce
	cureifburnedparalysedorpoisoned CalmMindBS_CheckStats
	attackanimation
	waitanimation
	printstring 0xA7 @;STRINGID_PKMNSTATUSNORMAL
	waitmessage DELAY_1SECOND
	refreshhpbar BANK_ATTACKER
	jumpifstat BANK_TARGET LESSTHAN STAT_SPATK STAT_MAX CalmMind_SpAtk_SkipAttackAnim
	jumpifstat BANK_TARGET LESSTHAN STAT_SPDEF STAT_MAX CalmMind_SpAtk_SkipAttackAnim
	goto BattleScript_CantRaiseMultipleStats

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

QuiverDanceBS:
	attackstring
	ppreduce
	jumpifstat BANK_ATTACKER LESSTHAN STAT_SPATK STAT_MAX QuiverDance_RaiseSpAtk1
	jumpifstat BANK_ATTACKER LESSTHAN STAT_SPDEF STAT_MAX QuiverDance_RaiseSpAtk1
	jumpifstat BANK_ATTACKER EQUALS STAT_SPD STAT_MAX BattleScript_CantRaiseMultipleStats

QuiverDance_RaiseSpAtk1:
	attackanimation
	waitanimation
	setbyte STAT_ANIM_PLAYED 0x0
	playstatchangeanimation BANK_ATTACKER, STAT_ANIM_SPATK | STAT_ANIM_SPDEF | STAT_ANIM_SPD, STAT_ANIM_UP | STAT_ANIM_IGNORE_ABILITIES
	setstatchanger STAT_SPATK | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN QuiverDance_RaiseSpDef1
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 QuiverDance_RaiseSpDef1
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND

QuiverDance_RaiseSpDef1:
	setstatchanger STAT_SPDEF | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN QuiverDance_RaiseSpd1
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 QuiverDance_RaiseSpd1
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND

QuiverDance_RaiseSpd1:
	setstatchanger STAT_SPD | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN BS_MOVE_END
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 BS_MOVE_END
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

GeomancyBS:
	attackcanceler
	jumpifsecondarystatus BANK_ATTACKER STATUS2_MULTIPLETURNS Geomancy_PowerUp
	jumpifword ANDS HIT_MARKER HITMARKER_NO_ATTACKSTRING Geomancy_PowerUp

Geomancy_Charge:	
	printstring 0x130 @;Blank String
	ppreduce
	attackanimation
	waitanimation
	jumpifhelditemeffect BANK_ATTACKER ITEM_EFFECT_POWER_HERB Geomancy_PowerHerb
	jumpifraidboss BANK_ATTACKER Geomancy_RaidBossSkipCharge
	orword HIT_MARKER HITMARKER_CHARGING
	setmoveeffect MOVE_EFFECT_CHARGING | MOVE_EFFECT_AFFECTS_USER
	seteffectprimary
	setword BATTLE_STRING_LOADER GeomancyChargeString
	printstring 0x184
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

Geomancy_PowerUp:
	setmoveeffect MOVE_EFFECT_CHARGING
	setbyte ANIM_TURN 0x1
	clearstatus BANK_ATTACKER
	
Geomancy_PowerHerbSkip:
	setmoveeffect 0x0
	attackstring
	jumpifstat BANK_ATTACKER LESSTHAN STAT_SPATK STAT_MAX Geomancy_RaiseSpAtk2
	jumpifstat BANK_ATTACKER LESSTHAN STAT_SPDEF STAT_MAX Geomancy_RaiseSpAtk2
	jumpifstat BANK_ATTACKER EQUALS STAT_SPD STAT_MAX BattleScript_CantRaiseMultipleStats

Geomancy_RaiseSpAtk2:
	attackanimation
	waitanimation
	setbyte STAT_ANIM_PLAYED 0x0
	playstatchangeanimation BANK_ATTACKER, STAT_ANIM_SPATK | STAT_ANIM_SPDEF | STAT_ANIM_SPD, STAT_ANIM_UP | STAT_ANIM_IGNORE_ABILITIES | STAT_ANIM_BY_TWO
	setstatchanger STAT_SPATK | INCREASE_2
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN Geomancy_RaiseSpDef2
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 Geomancy_RaiseSpDef2
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND

Geomancy_RaiseSpDef2:
	setstatchanger STAT_SPDEF | INCREASE_2
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN Geomancy_RaiseSpd2
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 Geomancy_RaiseSpd2
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND

Geomancy_RaiseSpd2:
	setstatchanger STAT_SPD | INCREASE_2
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN BS_MOVE_END
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 BS_MOVE_END
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

Geomancy_PowerHerb:
	playanimation BANK_ATTACKER ANIM_ITEM_USE 0x0
	setword BATTLE_STRING_LOADER PowerHerbString
	printstring 0x184
	waitmessage DELAY_1SECOND
	removeitem BANK_ATTACKER

Geomancy_RaidBossSkipCharge:
	setbyte ANIM_TARGETS_HIT 0x0
	setbyte ANIM_TURN 0x1
	callasm ClearCalculatedSpreadMoveData @;So the damage can be calculated
	goto Geomancy_PowerHerbSkip

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_212_DragonDance
BS_212_DragonDance:
	attackcanceler
	attackstring
	ppreduce
	jumpifmove MOVE_SHIFTGEAR ShiftGearBS
	jumpifmove MOVE_SHELLSMASH ShellSmashBS
	jumpifmove MOVE_FILLETAWAY FilletAwayBS

DragonDanceBS:
	jumpifstat BANK_ATTACKER LESSTHAN STAT_ATK STAT_MAX DragonDance_Atk
	jumpifstat BANK_ATTACKER EQUALS STAT_SPD STAT_MAX BattleScript_CantRaiseMultipleStats

DragonDance_Atk:
	attackanimation
	waitanimation
	setbyte STAT_ANIM_PLAYED 0x0
	playstatchangeanimation BANK_ATTACKER, STAT_ANIM_ATK | STAT_ANIM_SPD, STAT_ANIM_UP | STAT_ANIM_IGNORE_ABILITIES
	setstatchanger STAT_ATK | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN DragonDance_Spd
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 DragonDance_Spd
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND

DragonDance_Spd:
	setstatchanger STAT_SPD | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN BS_MOVE_END
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 BS_MOVE_END
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

ShiftGearBS:
	jumpifstat BANK_ATTACKER LESSTHAN STAT_ATK STAT_MAX ShiftGear_Atk
	jumpifstat BANK_ATTACKER EQUALS STAT_SPD STAT_MAX BattleScript_CantRaiseMultipleStats

ShiftGear_Atk:
	attackanimation
	waitanimation
	setbyte STAT_ANIM_PLAYED 0x0
	playstatchangeanimation BANK_ATTACKER, STAT_ANIM_ATK | STAT_ANIM_SPD, STAT_ANIM_UP | STAT_ANIM_IGNORE_ABILITIES | STAT_ANIM_BY_TWO
	setstatchanger STAT_ATK | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN ShiftGear_Spd
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 ShiftGear_Spd
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND

ShiftGear_Spd:
	setstatchanger STAT_SPD | INCREASE_2
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN BS_MOVE_END
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 BS_MOVE_END
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

ShellSmashBS:
	jumpifstat BANK_ATTACKER GREATERTHAN STAT_DEF STAT_MIN ShellSmash_DropDef
	jumpifstat BANK_ATTACKER EQUALS STAT_SPDEF STAT_MIN ShellSmash_BoostStats

ShellSmash_DropDef:
	attackanimation
	waitanimation
	setbyte ANIM_TARGETS_HIT 0x1 @;Prevent the attack animation from playing again
	setbyte STAT_ANIM_PLAYED 0x0
	playstatchangeanimation BANK_ATTACKER, STAT_ANIM_DEF | STAT_ANIM_SPDEF, STAT_ANIM_DOWN | STAT_ANIM_IGNORE_ABILITIES
	setstatchanger STAT_DEF | DECREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN ShellSmash_DropSpDef
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 ShellSmash_DropSpDef
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND

ShellSmash_DropSpDef:
	setstatchanger STAT_SPDEF | DECREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN ShellSmash_BoostStats
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 ShellSmash_BoostStats
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND

ShellSmash_BoostStats:
	jumpifstat BANK_ATTACKER LESSTHAN STAT_ATK STAT_MAX ShellSmash_SharpAtk
	jumpifstat BANK_ATTACKER LESSTHAN STAT_SPATK STAT_MAX ShellSmash_SharpAtk
	jumpifstat BANK_ATTACKER EQUALS STAT_SPD STAT_MAX BattleScript_CantRaiseMultipleStats

ShellSmash_SharpAtk:
	attackanimation
	waitanimation
	setbyte STAT_ANIM_PLAYED 0x0
	playstatchangeanimation BANK_ATTACKER, STAT_ANIM_ATK | STAT_ANIM_SPATK | STAT_ANIM_SPD, STAT_ANIM_UP | STAT_ANIM_IGNORE_ABILITIES | STAT_ANIM_BY_TWO
	setstatchanger STAT_ATK | INCREASE_2
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN ShellSmash_SharpSpAtk
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 ShellSmash_SharpSpAtk
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND

ShellSmash_SharpSpAtk:
	setstatchanger STAT_SPATK | INCREASE_2
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN ShiftGear_Spd
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 ShiftGear_Spd
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	goto ShiftGear_Spd

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

FilletAwayBS:
	callasm FailShedTailIfLowHP
	jumpifstat BANK_ATTACKER LESSTHAN STAT_ATK STAT_MAX FilletAway_HP
	jumpifstat BANK_ATTACKER EQUALS STAT_SPD STAT_MAX BattleScript_CantRaiseMultipleStats

FilletAway_HP:
	setdamagetobankhealthfraction BANK_ATTACKER 2 0x0 @;50 % of Base Max HP
	graphicalhpupdate BANK_ATTACKER
	datahpupdate BANK_ATTACKER
	faintpokemon BANK_ATTACKER 0x0 0x0
	goto FilletAway_Atk

FilletAway_Atk:
	attackanimation
	waitanimation
	setbyte STAT_ANIM_PLAYED 0x0
	playstatchangeanimation BANK_ATTACKER, STAT_ANIM_ATK | STAT_ANIM_SPD, STAT_ANIM_UP | STAT_ANIM_IGNORE_ABILITIES
	setstatchanger STAT_ATK | INCREASE_2
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN FilletAway_SharpSpAtk
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 FilletAway_SharpSpAtk
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND

FilletAway_SharpSpAtk:
	setstatchanger STAT_SPATK | INCREASE_2
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN ShiftGear_Spd
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 ShiftGear_Spd
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	goto ShiftGear_Spd

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_213_StatSwapSplitters
BS_213_StatSwapSplitters:
	attackcanceler
	attackstringnoprotean
	ppreduce
	accuracycheck BS_MOVE_MISSED_PAUSE 0x0
	callasm SetStatSwapSplit
	tryactivateprotean
	attackanimation
	waitanimation
	printstring 0x184
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_214_Blank
BS_214_Blank:
	goto BS_STANDARD_HIT

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_215_Blank
BS_215_Blank:
	goto BS_STANDARD_HIT

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_216_Blank
BS_216_Blank:
	goto BS_STANDARD_HIT

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_217_Blank
BS_217_Blank:
	goto BS_STANDARD_HIT
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_218_MeFirst
BS_218_MeFirst:
	attackcanceler
	attackstringnoprotean
	accuracycheck FAILED_PRE + 1 0x0
	callasm MeFirstFunc + 1
	attackanimation
	waitanimation
	setbyte ANIM_TURN 0x0
	setbyte ANIM_TARGETS_HIT 0x0
	jumptoattack 0x1

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_219_DestroyBerry
BS_219_DestroyBerry:
	jumpifmove MOVE_INCINERATE IncinerateBS
	setmoveeffect MOVE_EFFECT_EAT_BERRY | MOVE_EFFECT_CERTAIN
	goto BS_STANDARD_HIT
	
IncinerateBS:
	setmoveeffect MOVE_EFFECT_BURN_BERRY | MOVE_EFFECT_CERTAIN
	goto BS_STANDARD_HIT

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

@;Fail check is done in attackcanceler
.global BS_220_NaturalGift
BS_220_NaturalGift:
	attackcanceler
	accuracycheck NaturalGiftMiss 0x0
	call STANDARD_DAMAGE
	seteffectwithchancetarget
	prefaintmoveendeffects 0x0
	faintpokemonaftermove
	setbyte CMD49_STATE 0x0
	cmd49 0x0 0x0
	removeitem BANK_ATTACKER
	end

NaturalGiftMiss:
	attackstring
	ppreduce
	pause DELAY_HALFSECOND
	effectivenesssound
	resultmessage
	waitmessage DELAY_1SECOND
	removeitem BANK_ATTACKER
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_221_SmackDown
BS_221_SmackDown:
	setmoveeffect MOVE_EFFECT_BRING_DOWN | MOVE_EFFECT_CERTAIN
	goto BS_STANDARD_HIT

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_222_DamageResetTargetStatChanges
BS_222_DamageResetTargetStatChanges:
	setmoveeffect MOVE_EFFECT_RESET_STAT_CHANGES | MOVE_EFFECT_CERTAIN
	goto BS_STANDARD_HIT
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_223_RelicSong
BS_223_RelicSong:
	setmoveeffect MOVE_EFFECT_SLEEP
	attackcanceler
	accuracycheck BS_MOVE_MISSED 0x0
	call STANDARD_DAMAGE
	seteffectwithchancetarget
	prefaintmoveendeffects 0x0
	faintpokemonaftermove
	callasm GotoMoveEndIfMoveDidntDamageAtLeastOnce
	jumpifability BANK_ATTACKER ABILITY_SHEERFORCE BS_MOVE_END
	jumpifnoviablemonsleft BANK_TARGET BS_MOVE_END
	jumpifmove MOVE_WICKEDTORQUE BS_MOVE_END
	jumpifspecies BANK_ATTACKER SPECIES_MELOETTA TransformToPirouetteBS
	jumpifspecies BANK_ATTACKER SPECIES_MELOETTA_PIROUETTE TransformToAriaBS
	goto BS_MOVE_END
	
TransformToPirouetteBS:
	setbyte CMD49_STATE 0x0
	cmd49 0x0 0x0
	jumpiffainted BANK_ATTACKER RelicSongEndBS
	formchange BANK_ATTACKER SPECIES_MELOETTA SPECIES_MELOETTA_PIROUETTE TRUE TRUE TRUE RelicSongEndBS
	goto MeloettaTransformAnim

TransformToAriaBS:
	setbyte CMD49_STATE 0x0
	cmd49 0x0 0x0
	jumpiffainted BANK_ATTACKER RelicSongEndBS
	formchange BANK_ATTACKER SPECIES_MELOETTA_PIROUETTE SPECIES_MELOETTA TRUE TRUE TRUE RelicSongEndBS
	
MeloettaTransformAnim:
	playanimation BANK_ATTACKER ANIM_TRANSFORM 0x0
	setword BATTLE_STRING_LOADER TransformedString
	printstring 0x184
	waitmessage DELAY_1SECOND
	
RelicSongEndBS:
	end

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_224_Blank
BS_224_Blank:
	goto BS_STANDARD_HIT

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_225_Blank
BS_225_Blank:
	goto BS_STANDARD_HIT

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_226_Terrain
.global TerrainSeedCheck
BS_226_Terrain:
	attackcanceler
	attackstringnoprotean
	ppreduce
	setterrain FAILED
	callasm TransferTerrainData
	waitstateatk
	tryactivateprotean
	attackanimation
	waitanimation
	printstring 0x184
	waitmessage DELAY_1SECOND
	setbyte SEED_HELPER 0
	callasm SeedRoomServiceLooper
	copybyte USER_BANK TARGET_BANK @;Restore original target
	callasm TryActivateMimicry
	goto BS_MOVE_END
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_227_Pledges
.global BattleScript_PledgeReady
.global BattleScript_PledgeCombined
.global SwampString
.global SeaOfFireString
.global RainbowString
BS_227_Pledges:
	attackcanceler
	callasm LoadPledgeScript	
	goto 0x81D6927

BattleScript_PledgeReady:
	attackstring
	ppreduce
	pause DELAY_HALFSECOND
	setword BATTLE_STRING_LOADER PledgeReadyString
	printstring 0x184
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END
	
BattleScript_PledgeCombined:
	setword BATTLE_STRING_LOADER PledgeCombinedString
	printstring 0x184
	waitmessage DELAY_1SECOND
	accuracycheck BS_MOVE_MISSED + 1 0x0
	call STANDARD_DAMAGE + 1
	callasm SetPledgeEffect
	@;playanimation2 BANK_TARGET ANIM_ARG_1 0x0
	printstring 0x184
	waitmessage DELAY_1SECOND
	goto BS_MOVE_FAINT
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_228_FieldEffects
BS_228_FieldEffects:
	jumpifmove MOVE_PLASMAFISTS PlasmaFistsBS
	attackcanceler
	attackstring
	ppreduce
	callasm DoBattleFieldEffect
	attackanimation
	waitanimation
	printstring 0x184
	waitmessage DELAY_1SECOND
	jumpifmove MOVE_GRAVITY GravityBringDownBS
	setbyte SEED_HELPER 0
	callasm SeedRoomServiceLooper
	copybyte USER_BANK TARGET_BANK @;Restore original target
	goto BS_MOVE_END
	
GravityBringDownBS:
	callasm BringDownMons
	setbyte BATTLE_COMMUNICATION 0
	callasm PrintElectromagnetismFloatingStrings
	goto BS_MOVE_END
	
PlasmaFistsBS:
	setmoveeffect MOVE_EFFECT_ION_DELUGE | MOVE_EFFECT_CERTAIN
	goto BS_STANDARD_HIT
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_229_Fling
BS_229_Fling:
	attackcanceler
	attackstring
	ppreduce
	callasm TryFling
	pause DELAY_HALFSECOND
	setword BATTLE_STRING_LOADER FlingString
	printstring 0x184
	accuracycheck FlingMissBS 0x0
	callasm TransferLastUsedItem
	call STANDARD_DAMAGE + 2
	seteffectwithchancetarget
	callasm TrySetAlternateFlingEffect
	callasm ClearDoingPluckItemEffect
	removeitem BANK_ATTACKER
	prefaintmoveendeffects 0x0
	faintpokemonaftermove
	goto BS_MOVE_END

FlingMissBS:
	pause DELAY_HALFSECOND
	effectivenesssound
	resultmessage
	waitmessage DELAY_1SECOND
	removeitem BANK_ATTACKER
	goto BS_MOVE_END
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_230_Feint
BS_230_Feint:
	attackcanceler
	jumpifbehindsubstitute BANK_TARGET 0x81D6927
	
FeintSkipBS:
	accuracycheck BS_MOVE_MISSED 0x0
	call STANDARD_DAMAGE
	seteffectwithchancetarget
	jumpifmovehadnoeffect BS_MOVE_FAINT
	callasm LiftProtectTarget
	setword BATTLE_STRING_LOADER ProtectLiftedString
	printstring 0x184
	waitmessage DELAY_1SECOND
	goto BS_MOVE_FAINT	

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_231_AttackBlockers
BS_231_AttackBlockers:
	attackcanceler
	jumpifmove MOVE_THROATCHOP ThroatChopBS
	jumpifmove MOVE_EMBARGO EmbargoBS
	jumpifmove MOVE_POWDER PowderBS
	jumpifmove MOVE_TELEKINESIS TelekinesisBS
	jumpifmove MOVE_PSYCHICNOISE PsychicNoiseBS
	
HealBlockBS:
	jumpifbehindsubstitute BANK_TARGET FAILED_PRE
	jumpifcounter BANK_TARGET HEAL_BLOCK_TIMERS NOTEQUALS 0x0 FAILED_PRE
	jumpifabilitypresenttargetfield ABILITY_AROMAVEIL BattleScript_ProtectedByAromaVeil
	accuracycheck BS_MOVE_MISSED 0x0
	attackstring
	ppreduce
	attackanimation
	waitanimation
	callasm SetHealBlockTimer
	setword BATTLE_STRING_LOADER HealBlockSetString
	printstring 0x184
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

EmbargoBS:
	jumpifbehindsubstitute BANK_TARGET FAILED_PRE
	jumpifcounter BANK_TARGET EMBARGO_TIMERS NOTEQUALS 0x0 FAILED_PRE
	accuracycheck BS_MOVE_MISSED 0x0
	attackstring
	ppreduce
	attackanimation
	waitanimation
	setcounter BANK_TARGET EMBARGO_TIMERS 5
	setword BATTLE_STRING_LOADER EmbargoSetString
	printstring 0x184
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

PowderBS:
	jumpifcounter BANK_TARGET POWDER_TIMERS NOTEQUALS 0x0 FAILED_PRE
	accuracycheck BS_MOVE_MISSED 0x0
	typecalc2
	jumpifmovehadnoeffect CottonSporeNoEffect
	attackstring
	ppreduce
	attackanimation
	waitanimation
	setcounter BANK_TARGET POWDER_TIMERS 1
	setword BATTLE_STRING_LOADER PowderSetString
	printstring 0x184
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

TelekinesisBS:
	jumpifbehindsubstitute BANK_TARGET FAILED_PRE
	callasm CheckTelekinesisFail
	accuracycheck BS_MOVE_MISSED 0x0
	attackstring
	ppreduce
	attackanimation
	waitanimation
	setcounter BANK_TARGET TELEKINESIS_TIMERS 3
	setspecialstatusbit BANK_TARGET STATUS3_TELEKINESIS
	setword BATTLE_STRING_LOADER TelekinesisSetString
	printstring 0x184
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

ThroatChopBS:
	jumpifbehindsubstitute BANK_TARGET 0x81D6927
	accuracycheck BS_MOVE_MISSED 0x0
	call STANDARD_DAMAGE
	callasm SetThroatChopTimer
	seteffectwithchancetarget
	prefaintmoveendeffects 0x0
	faintpokemonaftermove
	goto BS_MOVE_END

PsychicNoiseBS:
	jumpifbehindsubstitute BANK_TARGET 0x81D6927
	accuracycheck BS_MOVE_MISSED 0x0
	call STANDARD_DAMAGE
	attackstring
	ppreduce
	attackanimation
	waitanimation
	callasm PSHealBlockTimer
	setword BATTLE_STRING_LOADER HealBlockSetString
	printstring 0x184
	waitmessage DELAY_1SECOND
	faintpokemonaftermove
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_232_TypeChangers
BS_232_TypeChangers:
	attackcanceler
	jumpifbehindsubstitute BANK_TARGET FAILED_PRE
	accuracycheck BS_MOVE_MISSED 0x0
	attackstring @;Activates Protean even if it fails
	ppreduce
	jumpifmove MOVE_SOAK SoakBS
	callasm ChangeTargetTypeFunc
	attackanimation
	waitanimation
	printstring 0x184
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END
	
SoakBS:
	attackanimation
	callasm ChangeTargetTypeFunc
	printstring 0x184
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_233_HealTarget
.global BattleScript_NoHealTargetAfterHealBlock
BS_233_HealTarget:
	attackcanceler
	jumpifmove MOVE_POLLENPUFF PollenPuffBS

HealPulseBS:
	jumpifbehindsubstitute BANK_TARGET FAILED_PRE
	attackstringnoprotean
	ppreduce
	jumpifcounter BANK_TARGET HEAL_BLOCK_TIMERS NOTEQUALS 0x0 BattleScript_NoHealTargetAfterHealBlock
	accuracycheck BS_MOVE_MISSED+2 0x0
	callasm HealTargetFunc
	tryactivateprotean
	attackanimation
	waitanimation
	orword HIT_MARKER HITMARKER_IGNORE_SUBSTITUTE
	graphicalhpupdate BANK_TARGET
	datahpupdate BANK_TARGET
	printstring 0x4B
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

BattleScript_NoHealTargetAfterHealBlock:
	pause DELAY_HALFSECOND
	orbyte OUTCOME OUTCOME_FAILED
	setword BATTLE_STRING_LOADER HealBlockTargetString
	printstring 0x184
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

PollenPuffBS:
	jumpiftargetpartner HealPulseBS
	goto 0x81D6927

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_234_TopsyTurvyElectrify
BS_234_TopsyTurvyElectrify:
	attackcanceler
	accuracycheck BS_MOVE_MISSED 0x0
	jumpifbehindsubstitute BANK_TARGET FAILED_PRE
	attackstring
	ppreduce
	jumpifmove MOVE_ELECTRIFY ElectrifyBS
	callasm FailMoveIfAura
	callasm TopsyTurvyFunc
	attackanimation
	waitanimation
	setword BATTLE_STRING_LOADER TopsyTurvyString
	printstring 0x184
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END
	
ElectrifyBS:
	jumpifcounter BANK_TARGET ELECTRIFY_TIMERS NOTEQUALS 0x0 FAILED
	setcounter BANK_TARGET ELECTRIFY_TIMERS 1
	attackanimation
	waitanimation
	setword BATTLE_STRING_LOADER ElectrifyString
	printstring 0x184
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

.global BattleScript_MoveFailedOnAura
BattleScript_MoveFailedOnAura:
	pause 0x20
	orbyte OUTCOME, OUTCOME_FAILED
	setword BATTLE_STRING_LOADER gText_MoveFailsOnWildBosses
	printstring 0x184
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_235_FairyLockHappyHourCelebrateHoldHands
BS_235_FairyLockHappyHourCelebrateHoldHands:
	attackcanceler
	attackstringnoprotean
	ppreduce
	callasm DoFairyLockHappyHourFunc
	jumpifmove MOVE_HOLDHANDS HoldHandsBS
	
HappyHourAnimBS:
	tryactivateprotean
	attackanimation
	waitanimation
	printstring 0x184
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END
	
HoldHandsBS:
	callasm SetTargetPartner
	accuracycheck FAILED 0x0
	goto HappyHourAnimBS
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_236_InstructAfterYouQuash
BS_236_InstructAfterYouQuash:
	attackcanceler
	attackstringnoprotean
	ppreduce
	jumpifmove MOVE_INSTRUCT InstructBS
	jumpifmove MOVE_QUASH QuashBS
	
AfterYouBS:
	callasm AfterYouFunc
	accuracycheck FAILED 0x0
	tryactivateprotean
	attackanimation
	waitanimation
	setword BATTLE_STRING_LOADER AfterYouString
	printstring 0x184
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END
	
QuashBS:
	callasm QuashFunc
	accuracycheck FAILED 0x0
	tryactivateprotean
	attackanimation
	waitanimation
	setword BATTLE_STRING_LOADER QuashString
	printstring 0x184
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END
	
InstructBS:
	accuracycheck BS_MOVE_MISSED+2 0x0
	callasm TryExecuteInstruct
	attackanimation
	waitanimation
	callasm InitiateInstruct
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_237_SuckerPunch
BS_237_SuckerPunch:
	attackcanceler
	callasm TrySuckerPunch
	goto 0x81D6927
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_238_Blank
BS_238_Blank:
	goto BS_STANDARD_HIT
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_239_TeamEffectsAndMagnetRise
BS_239_TeamEffectsAndMagnetRise:
	attackcanceler
	jumpifmove MOVE_MAGNETRISE MagnetRiseBS
	attackstringnoprotean
	ppreduce
	callasm TailwindLuckyChantFunc
	tryactivateprotean
	attackanimation
	waitanimation
	printstring 0x184
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END
	
MagnetRiseBS:
	attackstringnoprotean
	ppreduce
	callasm TrySetMagnetRise
	tryactivateprotean
	attackanimation
	waitanimation
	setword BATTLE_STRING_LOADER MagnetRiseSetString
	printstring 0x184
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_240_Camouflage
BS_240_Camouflage:
	attackcanceler
	attackstringnoprotean
	ppreduce
	settypetoterrain FAILED
	tryactivateprotean
	attackanimation
	waitanimation
	printstring 0x49 @;STRINGID_PKMNCHANGEDTYPE
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_241_FlameBurst
BS_241_FlameBurst:
	attackcanceler
	accuracycheck BS_MOVE_MISSED 0x0
	call STANDARD_DAMAGE
	jumpifmovehadnoeffect BS_MOVE_FAINT
	seteffectwithchancetarget
	prefaintmoveendeffects 0x0
	faintpokemonaftermove
	callasm FlameBurstFunc
	orword HIT_MARKER, HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_NON_ATTACK_DMG
	graphicalhpupdate BANK_SCRIPTING
	datahpupdate BANK_SCRIPTING
	setword BATTLE_STRING_LOADER FlameBurstString
	printstring 0x184
	waitmessage DELAY_1SECOND
	faintpokemon BANK_SCRIPTING 0x0 0x0
	goto BS_MOVE_END
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_242_LastResort
BS_242_LastResort:
	jumpifmove MOVE_GIGATONHAMMER GigatonHammerBS
	jumpifmove MOVE_BLOODMOON GigatonHammerBS
	jumpifmove MOVE_LASTRESORT LastResortBS

LastResortBS:
	attackcanceler
	callasm LastResortFunc
	goto BS_STANDARD_HIT + 1

GigatonHammerBS:
	attackcanceler
    callasm GigatonHammerFunc
    goto 0x81D6927
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_243_DamageSetTerrain
BS_243_DamageSetTerrain:
	attackcanceler
	jumpifmove MOVE_ICESPINNER IceSpinner_BS
	callasm TryFailSteelRoller
	accuracycheck BS_MOVE_MISSED 0x0
	call STANDARD_DAMAGE
	jumpifmovehadnoeffect BS_MOVE_FAINT
	seteffectwithchancetarget
	prefaintmoveendeffects 0x0
	faintpokemonaftermove
	call BattleScript_SetTerrain
	goto BS_MOVE_END

IceSpinner_BS:
	call STANDARD_DAMAGE
	jumpifmovehadnoeffect BS_MOVE_FAINT
	seteffectwithchancetarget
	prefaintmoveendeffects 0x0
	faintpokemonaftermove
	call BattleScript_SetTerrain
	goto BS_MOVE_END

.global BattleScript_SetTerrain
BattleScript_SetTerrain:
	setterrain BattleScript_SetTerrainReturn
	callasm TransferTerrainData
	waitstateatk
	playanimation2 BANK_ATTACKER ANIM_ARG_1 0x0
	printstring 0x184
	waitmessage DELAY_1SECOND
	setbyte SEED_HELPER 0
	copyhword BACKUP_HWORD USER_BANK @;Backup original atatcker + target
	callasm SeedRoomServiceLooper
	copyhword USER_BANK BACKUP_HWORD @;Restore original attacker + target
	callasm TryActivateMimicry
BattleScript_SetTerrainReturn:
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_244_Teatime
BS_244_Teatime:
	goto BS_STANDARD_HIT

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_245_Poltergeist
BS_245_Poltergeist:
	attackcanceler
	callasm TryFailPoltergeist
	accuracycheck BS_MOVE_MISSED 0x0
	attackstring
	ppreduce
	pause DELAY_HALFSECOND
	callasm TransferLastUsedItem
	setword BATTLE_STRING_LOADER gText_PoltergeistWarn
	printstring 0x184
	waitmessage DELAY_HALFSECOND
	goto BS_HIT_FROM_DAMAGE_CALC

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_246_SkyDrop
BS_246_SkyDrop:
	attackcanceler
	jumpifsecondarystatus BANK_ATTACKER STATUS2_MULTIPLETURNS SkyDropDropTurn2
	jumpifword ANDS HIT_MARKER HITMARKER_NO_ATTACKSTRING SkyDropDropTurn2
	jumpifbehindsubstitute BANK_TARGET FAILED_PRE
	jumpifspecialstatusflag BANK_TARGET STATUS3_SEMI_INVULNERABLE 0x0 FAILED_PRE
	jumpiffainted BANK_TARGET FAILED_PRE @;Can't rely on the normal check since this is a two-turn move
	jumpifweight BANK_TARGET GREATERTHAN 1999 FAILED_PRE @;199.9 kg
	accuracycheck BS_MOVE_MISSED 0x0
	attackstring
	ppreduce
	attackanimation
	waitanimation
	various BANK_TARGET 0x0 @;Cancel Multi Turn Moves
	setsemiinvulnerablebit
	setword BATTLE_STRING_LOADER SkyDropUpString
	printstring 0x184
	waitmessage DELAY_1SECOND
	orword HIT_MARKER HITMARKER_CHARGING
	setmoveeffect MOVE_EFFECT_CHARGING | MOVE_EFFECT_AFFECTS_USER
	seteffectprimary
	goto BS_MOVE_END

SkyDropDropTurn2:
	attackstring
	pause DELAY_HALFSECOND
	setmoveeffect MOVE_EFFECT_CHARGING
	setbyte ANIM_TURN 0x1
	clearstatus BANK_ATTACKER
	clearsemiinvulnerablebit
	makevisible BANK_TARGET
	copyarray BATTLE_SCRIPTING_BANK TARGET_BANK 0x1
	setword BATTLE_STRING_LOADER FreedFromSkyDropString
	printstring 0x184
	waitmessage DELAY_1SECOND
	goto BS_HIT_FROM_DAMAGE_CALC

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_247_Glaive_Rush
BS_247_Glaive_Rush:
	attackcanceler
	accuracycheck BS_MOVE_MISSED 0x0
	call STANDARD_DAMAGE
	jumpifmovehadnoeffect BS_MOVE_FAINT
	callasm GlaiveRushTimer
	goto BS_MOVE_FAINT

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_248_Blank
BS_248_Blank:
	goto BS_STANDARD_HIT
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_249_Blank
BS_249_Blank:
	goto BS_STANDARD_HIT
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_250_Blank
BS_250_Blank:
	goto BS_STANDARD_HIT

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_251_Blank
BS_251_Blank:
	goto BS_STANDARD_HIT

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_252_Blank
BS_252_Blank:
	goto BS_STANDARD_HIT
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_253_MaxMove
BS_253_MaxMove:
	attackcanceler
	accuracycheck BS_MOVE_MISSED 0x0
	call STANDARD_DAMAGE
	jumpifmovehadnoeffect BS_MOVE_FAINT
	setmaxmoveeffect
	prefaintmoveendeffects 0x0
	faintpokemonaftermove
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_254_Blank
BS_254_Blank:
	goto BS_STANDARD_HIT
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_255_Blank
BS_255_Blank:
	goto BS_STANDARD_HIT


.align 2
ProtectedByTerrainString: .byte 0xFD, 0x10, 0x00, 0xDD, 0xE7, 0xFE, 0xE4, 0xE6, 0xE3, 0xE8, 0xD9, 0xD7, 0xE8, 0xD9, 0xD8, 0x00, 0xD6, 0xED, 0x00, 0xE8, 0xDC, 0xD9, 0x00, 0xE8, 0xD9, 0xE6, 0xE6, 0xD5, 0xDD, 0xE2, 0xAB, 0xFF
ReflectTypeString: .byte 0xFD, 0x0F, 0xB4, 0xE7, 0x00, 0xE8, 0xED, 0xE4, 0xD9, 0x00, 0xD6, 0xD9, 0xD7, 0xD5, 0xE1, 0xD9, 0xFE, 0xE8, 0xDC, 0xD9, 0x00, 0xE7, 0xD5, 0xE1, 0xD9, 0x00, 0xD5, 0xE7, 0x00, 0xFD, 0x10, 0xB4, 0xE7, 0xAB, 0xFF
PurifyString: .byte 0xFD, 0x10, 0x00, 0xEB, 0xD5, 0xE7, 0x00, 0xD7, 0xE9, 0xE6, 0xD9, 0xD8, 0xFE, 0xE3, 0xDA, 0x00, 0xDD, 0xE8, 0xE7, 0x00, 0xFD, 0x0, 0x0, 0xE4, 0xE6, 0xE3, 0xD6, 0xE0, 0xD9, 0xE1, 0xAB, 0xFF
BecameNimbleString: .byte 0xFD, 0x0F, 0x00, 0xD6, 0xD9, 0xD7, 0xD5, 0xE1, 0xD9, 0x00, 0xE2, 0xDD, 0xE1, 0xD6, 0xE0, 0xD9, 0xAB, 0xFF
FreezeShockChargingString: .byte 0xFD, 0x0F, 0x00, 0xD6, 0xD9, 0xD7, 0xD5, 0xE1, 0xD9, 0xFE, 0xD7, 0xE0, 0xE3, 0xD5, 0xDF, 0xD9, 0xD8, 0x00, 0xDD, 0xE2, 0x00, 0xD5, 0x00, 0xDA, 0xE6, 0xD9, 0xD9, 0xEE, 0xDD, 0xE2, 0xDB, 0x00, 0xE0, 0xDD, 0xDB, 0xDC, 0xE8, 0xAB, 0xFF
IceBurnChargingString: .byte 0xFD, 0x0F, 0x00, 0xD6, 0xD9, 0xD7, 0xD5, 0xE1, 0xD9, 0xFE, 0xD7, 0xE0, 0xE3, 0xD5, 0xDF, 0xD9, 0xD8, 0x00, 0xDD, 0xE2, 0x00, 0xDA, 0xE6, 0xD9, 0xD9, 0xEE, 0xDD, 0xE2, 0xDB, 0x00, 0xD5, 0xDD, 0xE6, 0xAB, 0xFF
ShadowForceString: .byte 0xFD, 0x0F, 0x00, 0xEA, 0xD5, 0xE2, 0xDD, 0xE7, 0xDC, 0xD9, 0xD8, 0xFE, 0xDD, 0xE2, 0xE7, 0xE8, 0xD5, 0xE2, 0xE8, 0xE0, 0xED, 0xAB, 0xFF
LaserFocusString: .byte 0xFD, 0x0F, 0x00, 0xD7, 0xE3, 0xE2, 0xD7, 0xD9, 0xE2, 0xE8, 0xE6, 0xD5, 0xE8, 0xD9, 0xD8, 0xFE, 0xDD, 0xE2, 0xE8, 0xD9, 0xE2, 0xE7, 0xD9, 0xE0, 0xED, 0xAB, 0xFF
BurnUpString: .byte 0xFD, 0x0F, 0x00, 0xD6, 0xE9, 0xE6, 0xE2, 0xD9, 0xD8, 0xFE, 0xDD, 0xE8, 0xE7, 0xD9, 0xE0, 0xDA, 0x00, 0xE3, 0xE9, 0xE8, 0xAB, 0xFF
UTurnString: .byte 0xFD, 0x0F, 0x00, 0xEB, 0xD9, 0xE2, 0xE8, 0x00, 0xD6, 0xD5, 0xD7, 0xDF, 0xFE, 0xE8, 0xE3, 0x00, 0xFD, 0x39, 0xAB, 0xFF
RemoveFogString: .byte 0xFD, 0x0F, 0x00, 0xD6, 0xE0, 0xD9, 0xEB, 0x00, 0xD5, 0xEB, 0xD5, 0xED, 0xFE, 0xE8, 0xDC, 0xD9, 0x00, 0xD8, 0xD9, 0xD9, 0xE4, 0x00, 0xDA, 0xE3, 0xDB, 0x00, 0xEB, 0xDD, 0xE8, 0xDC, 0x00, 0xFD, 0x14, 0xAB, 0xFF
SpectralThiefString: .byte 0xFD, 0x0F, 0x00, 0xE7, 0xE8, 0xE3, 0xE0, 0xD9, 0xFE, 0xFD, 0x10, 0xB4, 0xE7, 0x00, 0xE7, 0xE8, 0xD5, 0xE8, 0x00, 0xD6, 0xE3, 0xE3, 0xE7, 0xE8, 0xE7, 0xAB, 0xFF
StockpileWoreOffString: .byte 0xFD, 0x0F, 0xB4, 0xE7, 0x00, 0xE7, 0xE8, 0xE3, 0xD7, 0xDF, 0xE4, 0xDD, 0xE0, 0xD9, 0xD8, 0xFE, 0xD9, 0xDA, 0xDA, 0xD9, 0xD7, 0xE8, 0x00, 0xEB, 0xE3, 0xE6, 0xD9, 0x00, 0xE3, 0xDA, 0xDA, 0xAB, 0xFF
BestowString: .byte 0xFD, 0x10, 0x00, 0xE6, 0xD9, 0xD7, 0xD9, 0xDD, 0xEA, 0xD9, 0xD8, 0x00, 0xE8, 0xDC, 0xD9, 0xFE, 0xFD, 0x16, 0x00, 0xDA, 0xE6, 0xE3, 0xE1, 0x00, 0xFD, 0x0F, 0xAB, 0xFF
AquaRingSetString: .byte 0xFD, 0x0F, 0x00, 0xE7, 0xE9, 0xE6, 0xE6, 0xE3, 0xE9, 0xE2, 0xD8, 0xD9, 0xD8, 0xFE, 0xDD, 0xE8, 0xE7, 0xD9, 0xE0, 0xDA, 0x00, 0xEB, 0xDD, 0xE8, 0xDC, 0x00, 0xD5, 0x00, 0xEA, 0xD9, 0xDD, 0xE0, 0x00, 0xE3, 0xDA, 0x00, 0xEB, 0xD5, 0xE8, 0xD9, 0xE6, 0xAB, 0xFF
CantUseHyperspaceFuryString: .byte 0xBC, 0xE9, 0xE8, 0x00 
CantUseMoveString: .byte 0xFD, 0x0F, 0x00, 0xD7, 0xD5, 0xE2, 0xB4, 0xE8, 0x00, 0xE9, 0xE7, 0xD9, 0xFE, 0xE8, 0xDC, 0xD9, 0x00, 0xE1, 0xE3, 0xEA, 0xD9, 0xAB, 0xFF
WrongHoopaFormString: .byte 0xBC, 0xE9, 0xE8, 0x00, 0xFD, 0x0F, 0x00, 0xD7, 0xD5, 0xE2, 0xB4, 0xE8, 0x00, 0xE9, 0xE7, 0xD9, 0x00, 0xDD, 0xE8, 0x00, 0xE8, 0xDC, 0xD9, 0xFE, 0xEB, 0xD5, 0xED, 0x00, 0xDD, 0xE8, 0x00, 0xDD, 0xE7, 0x00, 0xE2, 0xE3, 0xEB, 0xAB, 0xFF
PsychoShiftString: .byte 0xFD, 0x0F, 0x00, 0xE7, 0xDC, 0xDD, 0xDA, 0xE8, 0xD9, 0xD8, 0xFE, 0xDD, 0xE8, 0xE7, 0x00, 0xE7, 0xE8, 0xD5, 0xE8, 0xE9, 0xE7, 0x00, 0xE4, 0xE6, 0xE3, 0xD6, 0xE0, 0xD9, 0xE1, 0x00, 0xE3, 0xE2, 0xE8, 0xE3, 0xFA, 0xFD, 0x10, 0xAB, 0xFF
GeomancyChargeString: .byte 0xFD, 0x0F, 0x00, 0xDD, 0xE7, 0x00, 0xD5, 0xD6, 0xE7, 0xE3, 0xE6, 0xD6, 0xDD, 0xE2, 0xDB, 0xFE, 0xE4, 0xE3, 0xEB, 0xD9, 0xE6, 0xAB, 0xFF
PowerHerbString: .byte 0xFD, 0x0F, 0x00, 0xD6, 0xD9, 0xD7, 0xD5, 0xE1, 0xD9, 0x00, 0xDA, 0xE9, 0xE0, 0xE0, 0xED, 0xFE, 0xD7, 0xDC, 0xD5, 0xE6, 0xDB, 0xD9, 0xD8, 0x00, 0xD8, 0xE9, 0xD9, 0x00, 0xE8, 0xE3, 0x00, 0xDD, 0xE8, 0xE7, 0x00, 0xCA, 0xE3, 0xEB, 0xD9, 0xE6, 0x00, 0xC2, 0xD9, 0xE6, 0xD6, 0xAB, 0xFF
PowerTrickString: .byte 0xFD, 0x0F, 0x00, 0xE7, 0xEB, 0xDD, 0xE8, 0xD7, 0xDC, 0xD9, 0xD8, 0xFE, 0xDD, 0xE8, 0xE7, 0x00, 0xBB, 0xE8, 0xE8, 0xD5, 0xD7, 0xDF, 0x00, 0xD5, 0xE2, 0xD8, 0x00, 0xBE, 0xD9, 0xDA, 0xD9, 0xE2, 0xE7, 0xD9, 0xAB, 0xFF
PowerSwapString: .byte 0xFD, 0x0F, 0x00, 0xE7, 0xEB, 0xDD, 0xE8, 0xD7, 0xDC, 0xD9, 0xD8, 0x00, 0xD5, 0xE0, 0xE0, 0xFE, 0xD7, 0xDC, 0xD5, 0xE2, 0xDB, 0xD9, 0xE7, 0x00, 0xE8, 0xE3, 0x00, 0xDD, 0xE8, 0xE7, 0x00, 0xBB, 0xE8, 0xE8, 0xD5, 0xD7, 0xDF, 0x00, 0xD5, 0xE2, 0xD8, 0x00, 0xCD, 0xE4, 0xAD, 0x00, 0xBB, 0xE8, 0xDF, 0xFA, 0xEB, 0xDD, 0xE8, 0xDC, 0x00, 0xFD, 0x10, 0xAB, 0xFF
GuardSwapString: .byte 0xFD, 0x0F, 0x00, 0xE7, 0xEB, 0xDD, 0xE8, 0xD7, 0xDC, 0xD9, 0xD8, 0x00, 0xD5, 0xE0, 0xE0, 0xFE, 0xD7, 0xDC, 0xD5, 0xE2, 0xDB, 0xD9, 0xE7, 0x00, 0xE8, 0xE3, 0x00, 0xDD, 0xE8, 0xE7, 0x00, 0xBE, 0xD9, 0xDA, 0xD9, 0xE2, 0xE7, 0xD9, 0x00, 0xD5, 0xE2, 0xD8, 0x00, 0xCD, 0xE4, 0xAD, 0x00, 0xBE, 0xD9, 0xDA, 0xFA, 0xEB, 0xDD, 0xE8, 0xDC, 0x00, 0xFD, 0x10, 0xAB, 0xFF
SpeedSwapString: .byte 0xFD, 0x0F, 0x00, 0xE7, 0xEB, 0xDD, 0xE8, 0xD7, 0xDC, 0xD9, 0xD8, 0xFE, 0xE7, 0xE4, 0xD9, 0xD9, 0xD8, 0x00, 0xEB, 0xDD, 0xE8, 0xDC, 0x00, 0xFD, 0x10, 0xAB, 0xFF
HeartSwapString: .byte 0xFD, 0x0F, 0x00, 0xE7, 0xEB, 0xDD, 0xE8, 0xD7, 0xDC, 0xD9, 0xD8, 0xFE, 0xE7, 0xE8, 0xD5, 0xE8, 0x00, 0xD7, 0xDC, 0xD5, 0xE2, 0xDB, 0xD9, 0xE7, 0x00, 0xEB, 0xDD, 0xE8, 0xDC, 0xFA, 0xFD, 0x10, 0xAB, 0xFF
PowerSplitString: .byte 0xFD, 0x0F, 0x00, 0xE7, 0xDC, 0xD5, 0xE6, 0xD9, 0xD8, 0x00, 0xDD, 0xE8, 0xE7, 0xFE, 0xE4, 0xE3, 0xEB, 0xD9, 0xE6, 0x00, 0xEB, 0xDD, 0xE8, 0xDC, 0x00, 0xE8, 0xDC, 0xD9, 0x00, 0xE8, 0xD5, 0xE6, 0xDB, 0xD9, 0xE8, 0xAB, 0xFF
GuardSplitString: .byte 0xFD, 0x0F, 0x00, 0xE7, 0xDC, 0xD5, 0xE6, 0xD9, 0xD8, 0x00, 0xDD, 0xE8, 0xE7, 0xFE, 0xDB, 0xE9, 0xD5, 0xE6, 0xD8, 0x00, 0xEB, 0xDD, 0xE8, 0xDC, 0x00, 0xE8, 0xDC, 0xD9, 0x00, 0xE8, 0xD5, 0xE6, 0xDB, 0xD9, 0xE8, 0xAB, 0xFF
PledgeReadyString: .byte 0xFD, 0x0F, 0x00, 0xDD, 0xE7, 0x00, 0xEB, 0xD5, 0xDD, 0xE8, 0xDD, 0xE2, 0xDB, 0xFE, 0xDA, 0xE3, 0xE6, 0x00, 0xFD, 0x13, 0xB4, 0xE7, 0x00, 0xE1, 0xE3, 0xEA, 0xD9, 0xAB, 0xFF
PledgeCombinedString: .byte 0xFD, 0x0F, 0x00, 0xE9, 0xE7, 0xD9, 0xD8, 0xFE, 0xFD, 0x14, 0xAB, 0xFB, 0xCE, 0xDC, 0xD9, 0x00, 0xE8, 0xEB, 0xE3, 0x00, 0xE1, 0xE3, 0xEA, 0xD9, 0xE7, 0x00, 0xD5, 0xE6, 0xD9, 0x00, 0xDE, 0xE3, 0xDD, 0xE2, 0xD9, 0xD8, 0xAB, 0xFE, 0xC3, 0xE8, 0xB4, 0xE7, 0x00, 0xD5, 0x00, 0xD7, 0xE3, 0xE1, 0xD6, 0xDD, 0xE2, 0xD9, 0xD8, 0x00, 0xE1, 0xE3, 0xEA, 0xD9, 0xAB, 0xFF
SwampString: .byte 0xBB, 0x00, 0xE7, 0xEB, 0xD5, 0xE1, 0xE4, 0x00, 0xD9, 0xE2, 0xEA, 0xD9, 0xE0, 0xE3, 0xE4, 0xD9, 0xD8, 0xFE, 0xFD, 0x35, 0x00, 0xE8, 0xD9, 0xD5, 0xE1, 0xAB, 0xFF
SeaOfFireString: .byte 0xBB, 0x00, 0xE7, 0xD9, 0xD5, 0x00, 0xE3, 0xDA, 0x00, 0xDA, 0xDD, 0xE6, 0xD9, 0x00, 0xD9, 0xE2, 0xEA, 0xD9, 0xE0, 0xE3, 0xE4, 0xD9, 0xD8, 0xFE, 0xFD, 0x35, 0x00, 0xE8, 0xD9, 0xD5, 0xE1, 0xAB, 0xFF
RainbowString: .byte 0xBB, 0x00, 0xE6, 0xD5, 0xDD, 0xE2, 0xD6, 0xE3, 0xEB, 0x00, 0xD5, 0xE4, 0xE4, 0xD9, 0xD5, 0xE6, 0xD9, 0xD8, 0x00, 0xDD, 0xE2, 0x00, 0xE8, 0xDC, 0xD9, 0x00, 0xE7, 0xDF, 0xED, 0xFE, 0xE3, 0xE2, 0x00, 0xFD, 0x36, 0x00, 0xE8, 0xD9, 0xD5, 0xE1, 0xB4, 0xE7, 0x00, 0xE7, 0xDD, 0xD8, 0xD9, 0xAB, 0xFF
FlingString: .byte 0xFD, 0x0F, 0x00, 0xDA, 0xE0, 0xE9, 0xE2, 0xDB, 0xFE, 0xDD, 0xE8, 0xE7, 0x00, 0xFD, 0x16, 0xAB, 0xFF
ProtectLiftedString: .byte 0xCE, 0xDC, 0xD9, 0x00, 0xE4, 0xE6, 0xE3, 0xE8, 0xD9, 0xD7, 0xE8, 0xDD, 0xE3, 0xE2, 0x00, 0xD9, 0xDA, 0xDA, 0xD9, 0xD7, 0xE8, 0xE7, 0x00, 0xEB, 0xD9, 0xE6, 0xD9, 0x00, 0xE0, 0xDD, 0xDA, 0xE8, 0xD9, 0xD8, 0xFE, 0xDA, 0xE6, 0xE3, 0xE1, 0x00, 0xE8, 0xDC, 0xD9, 0x00, 0xE8, 0xD5, 0xE6, 0xDB, 0xD9, 0xE8, 0xB4, 0xE7, 0x00, 0xE7, 0xDD, 0xD8, 0xD9, 0xAB, 0xFF
HealBlockSetString: .byte 0xFD, 0x10, 0x00, 0xEB, 0xD5, 0xE7, 0xFE, 0xE4, 0xE6, 0xD9, 0xEA, 0xD9, 0xE2, 0xE8, 0xD9, 0xD8, 0x00, 0xDA, 0xE6, 0xE3, 0xE1, 0x00, 0xDC, 0xD9, 0xD5, 0xE0, 0xDD, 0xE2, 0xDB, 0xAB, 0xFF
EmbargoSetString: .byte 0xFD, 0x10, 0x00, 0xD7, 0xD5, 0xE2, 0xB4, 0xE8, 0x00, 0xE9, 0xE7, 0xD9, 0xFE, 0xDD, 0xE8, 0xD9, 0xE1, 0xE7, 0x00, 0xD5, 0xE2, 0xED, 0xE1, 0xE3, 0xE6, 0xD9, 0xAB, 0xFF
PowderSetString: .byte 0xFD, 0x10, 0x00, 0xDD, 0xE7, 0x00, 0xD7, 0xE3, 0xEA, 0xD9, 0xE6, 0xD9, 0xD8, 0xFE, 0xDD, 0xE2, 0x00, 0xE4, 0xE3, 0xEB, 0xD8, 0xD9, 0xE6, 0xAB, 0xFF
TelekinesisSetString: .byte 0xFD, 0x10, 0x00, 0xEB, 0xD5, 0xE7, 0x00, 0xDC, 0xE9, 0xE6, 0xE0, 0xD9, 0xD8, 0xFE, 0xDD, 0xE2, 0xE8, 0xE3, 0x00, 0xE8, 0xDC, 0xD9, 0x00, 0xD5, 0xDD, 0xE6, 0xAB, 0xFF
HealBlockTargetString: .byte 0xFD, 0x10, 0x00, 0xEB, 0xD5, 0xE7, 0x00, 0xE4, 0xE6, 0xD9, 0xEA, 0xD9, 0xE2, 0xE8, 0xD9, 0xD8, 0xFE, 0xDA, 0xE6, 0xE3, 0xE1, 0x00, 0xDC, 0xD9, 0xD5, 0xE0, 0xDD, 0xE2, 0xDB, 0x00, 0xD8, 0xE9, 0xD9, 0x00, 0xE8, 0xE3, 0x00, 0xC2, 0xD9, 0xD5, 0xE0, 0x00, 0xBC, 0xE0, 0xE3, 0xD7, 0xDF, 0xAB, 0xFF
ElectrifyString: .byte 0xFD, 0x10, 0xB4, 0xE7, 0x00, 0xE1, 0xE3, 0xEA, 0xD9, 0xE7, 0xFE, 0xDC, 0xD5, 0xEA, 0xD9, 0x00, 0xD6, 0xD9, 0xD9, 0xE2, 0x00, 0xD9, 0xE0, 0xD9, 0xD7, 0xE8, 0xE6, 0xDD, 0xDA, 0xDD, 0xD9, 0xD8, 0xAB, 0xFF
TopsyTurvyString: .byte 0xFD, 0x10, 0xB4, 0xE7, 0x00, 0xE7, 0xE8, 0xD5, 0xE8, 0xE7, 0x00, 0xD7, 0xDC, 0xD5, 0xE2, 0xDB, 0xD9, 0xE7, 0xFE, 0xEB, 0xD9, 0xE6, 0xD9, 0x00, 0xE6, 0xD9, 0xEA, 0xD9, 0xE6, 0xE7, 0xD9, 0xD8, 0xAB, 0xFF
AfterYouString: .byte 0xFD, 0x10, 0xFE, 0xE8, 0xE3, 0xE3, 0xDF, 0x00, 0xE8, 0xDC, 0xD9, 0x00, 0xDF, 0xDD, 0xE2, 0xD8, 0x00, 0xE3, 0xDA, 0xDA, 0xD9, 0xE6, 0xAB, 0xFF
QuashString: .byte 0xFD, 0x10, 0xB4, 0xE7, 0x00, 0xE1, 0xE3, 0xEA, 0xD9, 0xFE, 0xEB, 0xD5, 0xE7, 0x00, 0xE4, 0xE3, 0xE7, 0xE8, 0xE4, 0xE3, 0xE2, 0xD9, 0xD8, 0xAB, 0xFF
MagnetRiseSetString: .byte 0xFD, 0x0F, 0x00, 0xE0, 0xD9, 0xEA, 0xDD, 0xE8, 0xD5, 0xE8, 0xD9, 0xD8, 0xFE, 0xEB, 0xDD, 0xE8, 0xDC, 0x00, 0xD9, 0xE0, 0xD9, 0xD7, 0xE8, 0xE6, 0xE3, 0xE1, 0xD5, 0xDB, 0xE2, 0xD9, 0xE8, 0xDD, 0xE7, 0xE1, 0xAB, 0xFF
FlameBurstString: .byte 0xCE, 0xDC, 0xD9, 0x00, 0xD6, 0xE9, 0xE6, 0xE7, 0xE8, 0xDD, 0xE2, 0xDB, 0x00, 0xDA, 0xE0, 0xD5, 0xE1, 0xD9, 0x00, 0xDC, 0xDD, 0xE8, 0xFE, 0xFD, 0x13, 0xAB, 0xFF
DoubleShockString: .byte 0xFD, 0x0F, 0x00, u_, s_, e_, d_, 0x00, u_, p_, 0x00, a_, l_, l_, 0x00, i_, t_, s_, 0x00, e_, l_, e_, c_, t_, r_, i_, c_, i_, t_, y_, 0xAB, 0xFF
MatchaGotchaString: .byte 0xFD, 0x0F, 0x00, s_, u_, c_, k_, e_, d_, 0x00, h_, e_, a_, l_, t_, h_, 0x00, f_, r_, o_, m_, 0xFE, e_, n_, e_, m_, y_, 0x00, 0xFD, 0x10, 0xAD, 0xFF
ButFailedString: .byte B_, u_, t_, 0x0, i_, t_, 0x00, f_, a_, i_, l_, e_, d_, 0xAB, 0xFF
SubstituteString: .byte 0xFD, 0x0F, 0x00, i_, s_, 0x00, h_, i_, d_, d_, e_, n_, 0x00, i_, n_, 0x00, a_, 0x00, s_, u_, b_, s_, t_, i_, t_, u_, t_, e_, 0xAD, 0xFF
SubstituteFailString: .byte B_, u_, t_, 0x00, f_, a_, i_, l_, e_, d_, 0xAD, 0xFF
