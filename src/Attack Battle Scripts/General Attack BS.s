@Do 43 - High Crit Ratio
@;Add in Destiny Knot here?

.text
.thumb
.align 2

.include "..\\defines"
/*
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_028_Roar
BS_028_Roar:
	jumpifmove MOVE_DRAGONTAIL DragonTailBS
	jumpifmove MOVE_CIRCLETHROW DragonTailBS
	goto 0x81D6CB1

DragonTailBS:
	attackcanceler
	accuracycheck BS_MOVE_MISSED 0x0
	jumpifsecondarystatus BANK_TARGET STATUS2_SUBSTITUTE 0x81D692E
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

.global BS_030_Conversion
BS_030_Conversion:
	jumpifhalfword NOTEQUALS CURRENT_MOVE MOVE_REFLECTTYPE 0x1D6D9D
	attackcanceler
	accuracycheck BS_MOVE_MISSED 0x0
	attackstring
	ppreduce
	jumpifsecondarystatus BANK_TARGET STATUS2_SUBSTITUTE FAILED
	changetypestoenemyattacktype FAILED
	attackanimation
	waitanimation
	setword BATTLE_STRING_LOADER ReflectTypeString
	printstring 0x184
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

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

.global BS_057_Transform
BS_057_Transform:
	attackcanceler
	jumpifsecondarystatus BANK_TARGET STATUS2_SUBSTITUTE 0x81D7DF0
	jumpifsecondarystatus BANK_ATTACKER STATUS2_TRANSFORMED 0x81D7DF0
	jumpifspecialstatusflag BANK_TARGET STATUS3_ILLUSION 0x0 0x81D7DF0
	attackstring
	ppreduce
	transformdataexecution
	attackanimation
	waitanimation
	printfromtable 0x83FE5B4
	waitmessage DELAY_1SECOND
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_065_Reflect
BS_065_Reflect:
	jumpifhalfword NOTEQUALS CURRENT_MOVE MOVE_AURORAVEIL 0x1D716E
	attackcanceler
	attackstring
	ppreduce
	callasm SetAuroraVeil + 1
	jumpifbyte EQUALS FORM_COUNTER 0x0 FAILED
	attackanimation
	waitanimation
	setword BATTLE_STRING_LOADER AuroraVeilSetString
	printstring 0x184
	waitmessage DELAY_1SECOND
	setbyte FORM_COUNTER 0x0
	goto BS_MOVE_END

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
	jumpifmove MOVE_RAZORWIND RazorWindEffectBS
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

.global BS_077_Twineedle
BS_077_Twineedle:
	setmoveeffect MOVE_EFFECT_POISON
	goto 0x81D6926

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BS_081_Rage
BS_081_Rage:
	attackcanceler
	accuracycheck 0x81D7367 0x0
	setmoveeffect 0x1E
	call StandardDamage
	jumpifbyte ANDS OUTCOME OUTCOME_NO_EFFECT BS_MOVE_FAINT
	seteffecttarget
	goto BS_MOVE_FAINT
	
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

.global BS_094_LaserFocus
BS_094_LaserFocus:
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

.global BS_097_SleepTalk
BS_097_SleepTalk:
	attackcanceler
	jumpifstatus BANK_ATTACKER STATUS_SLEEP 0x81D753E
	jumpifability BANK_ATTACKER ABILITY_COMATOSE 0x81D753E
	goto FAILED - 2

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
	seteffecttarget
	jumpifmovehadnoeffect BS_MOVE_FAINT
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

.global BS_119_FuryCutter
BS_119_FuryCutter:
	jumpifmove MOVE_ROUND RoundBS
	jumpifmove MOVE_ECHOEDVOICE EchoedVoiceBS
	goto 0x1D7919
	
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

.global BS_125_FlameWheel
BS_125_FlameWheel:
	attackcanceler
	accuracycheck BS_MOVE_MISSED 0x0
	jumpifmove MOVE_BURNUP BurnUpBS
	setmoveeffect MOVE_EFFECT_BURN
	jumpifmove MOVE_FLAREBLITZ FlareBlitzBS
	goto 0x81D692E

BurnUpBS:
	jumpiftype BANK_ATTACKER TYPE_FIRE DoBurnUp
	goto FAILED-2

DoBurnUp:
	call STANDARD_DAMAGE
	jumpifmovehadnoeffect BS_MOVE_END
	callasm BurnUpFunc + 1
	setword BATTLE_STRING_LOADER BurnUpString
	printstring 0x184
	waitmessage DELAY_1SECOND
	goto BS_MOVE_FAINT
	
FlareBlitzBS:
	call STANDARD_DAMAGE
	seteffectwithchancetarget
	setmoveeffect MOVE_EFFECT_RECOIL_33 | MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_CERTAIN
	seteffectuser
	goto BS_MOVE_FAINT

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

.global BS_129_RapidSpin
.global BattleScript_DefogAdditionalEffects
BS_129_RapidSpin:
	jumpifnotmove MOVE_DEFOG 0x1D79C2
	attackcanceler
	jumpifbehindsubstitute BANK_TARGET SecondDefogCheck
	accuracycheck SecondDefogCheck 0x0
	attackstring
	ppreduce
	setbyte STAT_CHANGE_BYTE STAT_EVSN | DECREASE_1
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

.global BS_142_BellyDrum
BS_142_BellyDrum:
	jumpifnotmove MOVE_MINDBLOWN 0x1D7A52
	jumpifability BANK_ATTACKER ABILITY_MAGICGUARD 0x1D6926
	attackcanceler
	accuracycheck 0x81D695E 0x0
	call STANDARD_DAMAGE
	@;TODO

.align 2
ReflectTypeString: .byte 0xFD, 0x0F, 0xB4, 0xE7, 0x00, 0xE8, 0xED, 0xE4, 0xD9, 0x00, 0xD6, 0xD9, 0xD7, 0xD5, 0xE1, 0xD9, 0xFE, 0xE8, 0xDC, 0xD9, 0x00, 0xE7, 0xD5, 0xE1, 0xD9, 0x00, 0xD5, 0xE7, 0x00, 0xFD, 0x10, 0xB4, 0xE7, 0xAB, 0xFF
AuroraVeilSetString: .byte 0xFD, 0x14, 0x00, 0xE4, 0xE6, 0xE3, 0xE8, 0xD9, 0xD7, 0xE8, 0xE7, 0x00, 0xD5, 0xDB, 0xD5, 0xDD, 0xE2, 0xE7, 0xE8, 0x00, 0xE4, 0xDC, 0xED, 0xE7, 0xDD, 0xD7, 0xD5, 0xE0, 0xFE, 0xD5, 0xE2, 0xD8, 0x00, 0xE7, 0xE4, 0xD9, 0xD7, 0xDD, 0xD5, 0xE0, 0x00, 0xE1, 0xE3, 0xEA, 0xD9, 0xE7, 0xAB, 0xFF
ProtectedByAromaVeilString: .byte 0xFD, 0x10, 0xB4, 0xE7, 0x00, 0xE4, 0xD5, 0xE6, 0xE8, 0xED, 0x00, 0xDD, 0xE7, 0xFE, 0xE4, 0xE6, 0xE3, 0xE8, 0xD9, 0xD7, 0xE8, 0xD9, 0xD8, 0x00, 0xD6, 0xED, 0x00, 0xBB, 0xE6, 0xE3, 0xE1, 0xD5, 0x00, 0xD0, 0xD9, 0xDD, 0xE0, 0xAB, 0xFF
FreezeShockChargingString: .byte 0xFD, 0x0F, 0x00, 0xD6, 0xD9, 0xD7, 0xD5, 0xE1, 0xD9, 0xFE, 0xD7, 0xE0, 0xE3, 0xD5, 0xDF, 0xD9, 0xD8, 0x00, 0xDD, 0xE2, 0x00, 0xD5, 0x00, 0xDA, 0xE6, 0xD9, 0xD9, 0xEE, 0xDD, 0xE2, 0xDB, 0x00, 0xE0, 0xDD, 0xDB, 0xDC, 0xE8, 0xAB, 0xFF
IceBurnChargingString: .byte 0xFD, 0x0F, 0x00, 0xD6, 0xD9, 0xD7, 0xD5, 0xE1, 0xD9, 0xFE, 0xD7, 0xE0, 0xE3, 0xD5, 0xDF, 0xD9, 0xD8, 0x00, 0xDD, 0xE2, 0x00, 0xDA, 0xE6, 0xD9, 0xD9, 0xEE, 0xDD, 0xE2, 0xDB, 0x00, 0xD5, 0xDD, 0xE6, 0xAB, 0xFF
LaserFocusString: .byte 0xFD, 0x0F, 0x00, 0xD7, 0xE3, 0xE2, 0xD7, 0xD9, 0xE2, 0xE8, 0xE6, 0xD5, 0xE8, 0xD9, 0xD8, 0x00, 0xDD, 0xE2, 0xE8, 0xD9, 0xE2, 0xE7, 0xD9, 0xE0, 0xED, 0xAB, 0xFF
BurnUpString: .byte 0xFD, 0x0F, 0x00, 0xD6, 0xE9, 0xE6, 0xE2, 0xD9, 0xD8, 0x00, 0xDD, 0xE8, 0xE7, 0xD9, 0xE0, 0xDA, 0x00, 0xE3, 0xE9, 0xE8, 0xAB, 0xFF
UTurnString: .byte 0x
RemoveFogString: .byte 0xFD, 0x0F, 0x00, 0xD6, 0xE0, 0xD9, 0xEB, 0x00, 0xD5, 0xEB, 0xD5, 0xED, 0xFE, 0xE8, 0xDC, 0xD9, 0x00, 0xD8, 0xD9, 0xD9, 0xE4, 0x00, 0xDA, 0xE3, 0xDB, 0x00, 0xEB, 0xDD, 0xE8, 0xDC, 0x00, 0xFD, 0x14, 0xAB, 0xFF
*/
