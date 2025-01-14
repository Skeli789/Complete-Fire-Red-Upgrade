.thumb
.align 2
/*
et_battle_scripts.s
	handles end of turn effects/strings
*/

.include "../asm_defines.s"
.include "../battle_script_macros.s"

.global BattleScript_MysteriousAirCurrentContinues
.global BattleScript_MysteriousAirCurrentContinuesNoString
.global BattleScript_RainContinuesOrEnds
.global BattleScript_RainContinuesOrEndsNoString
.global BattleScript_SunlightContinues
.global BattleScript_SunlightContinuesNoString
.global BattleScript_SandstormHailContinues
.global BattleScript_SandstormHailContinuesNoString
.global BattleScript_WeatherDamage
.global BattleScript_FogEnded
.global BattleScript_FogContinues
.global BattleScript_FogContinuesNoString
.global BattleScript_SeaOfFireDamage
.global BattleScript_GrassyTerrainHeal
.global BattleScript_AquaRing
.global BattleScript_LeechSeedTurnDrain
.global BattleScript_PoisonHeal
.global BattleScript_FrostbiteTurnDmg
.global BattleScript_SplintersTurnDmg
.global BattleScript_BadThoughtsTurnDmg
.global BattleScript_YawnMakesAsleep
.global BattleScript_MagnetRiseEnd
.global BattleScript_TelekinesisEnd
.global BattleScript_HealBlockEnd
.global BattleScript_EmbargoEnd
.global BattleScript_TailwindEnd
.global BattleScript_LuckyChantEnd
.global BattleScript_RainbowEnd
.global BattleScript_SeaOfFireEnd
.global BattleScript_SwampEnd
.global BattleScript_TrickRoomEnd
.global BattleScript_WaterSportEnd
.global BattleScript_MudSportEnd
.global BattleScript_WonderRoomEnd
.global BattleScript_MagicRoomEnd
.global BattleScript_GravityEnd
.global BattleScript_TerrainEnd
.global BattleScript_ToxicOrb
.global BattleScript_FlameOrb
.global BattleScript_PowerConstruct
.global BattleScript_StartedSchooling
.global BattleScript_StoppedSchooling
.global BattleScript_ShieldsDownToCore
.global BattleScript_ShieldsDownToMeteor
.global BattleScript_IceFaceRestoreFace
.global BattleScript_FlowerGift
.global BattleScript_FlowerGiftEnd2
.global BattleScript_MonTookFutureAttack
.global BattleScript_OctolockTurnDmg
.global BattleScript_DynamaxEnd
.global BattleScript_HoopaSOS
.global BattleScript_PrintCustomStringEnd2
.global BattleScript_PrintCustomStringEnd3

.global TrickRoomEndString
.global WonderRoomEndString
.global MagicRoomEndString
.global GravityEndString
.global TerrainEndString
.global TransformedString

.global AbilityActivatedString

.equ BattleScript_DoStatusTurnDmg, 0x81d9059
.equ BattleScript_DoTurnDmg, 0x81D905B

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MysteriousAirCurrentContinues:
	setword BATTLE_STRING_LOADER gText_MysteriousAirCurrentContinues
	printstring 0x184
	waitmessage DELAY_1SECOND
BattleScript_MysteriousAirCurrentContinuesPlayAnim:
	playanimation 0x0 ANIM_STRONG_WINDS_CONTINUE 0x0
	end2

BattleScript_MysteriousAirCurrentContinuesNoString:
	call BS_FLUSH_MESSAGE_BOX
	goto BattleScript_MysteriousAirCurrentContinuesPlayAnim

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_RainContinuesOrEnds:
	printfromtable 0x83FE540 @;gRainContinuesStringIds
	waitmessage DELAY_1SECOND
	jumpifbyte EQUALS, MULTISTRING_CHOOSER, 0x2, BattleScript_RainContinuesOrEndsEnd @;Rain ended
BattleScript_RainContinuesOrEndsPlayAnim:
	playanimation BANK_ATTACKER, ANIM_RAIN, 0x0
BattleScript_RainContinuesOrEndsEnd:
	end2

BattleScript_RainContinuesOrEndsNoString:
	jumpifbyte EQUALS, MULTISTRING_CHOOSER, 0x2, BattleScript_RainContinuesOrEnds @;Rain ended
	call BS_FLUSH_MESSAGE_BOX
	goto BattleScript_RainContinuesOrEndsPlayAnim

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_SunlightContinues:
	printstring 0xF1 @;STRINGID_SUNLIGHTSTRONG
	waitmessage DELAY_1SECOND
BattleScript_SunlightContinuesPlayAnim:
	playanimation BANK_ATTACKER, ANIM_SUN, 0x0
	end2

BattleScript_SunlightContinuesNoString:
	call BS_FLUSH_MESSAGE_BOX
	goto BattleScript_SunlightContinuesPlayAnim

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_SandstormHailContinues:
	printfromtable gSandstormHailContinuesStringIds
	waitmessage DELAY_1SECOND
BattleScript_SandstormHailContinuesPlayAnim:
	playanimation2 BANK_ATTACKER, ANIM_ARG_1, 0x0
	end2

BattleScript_SandstormHailContinuesNoString:
	call BS_FLUSH_MESSAGE_BOX
	goto BattleScript_SandstormHailContinuesPlayAnim

BattleScript_WeatherDamage:
	weatherdamage
	jumpifword EQUALS DAMAGE_LOC 0x0 BattleScript_WeatherDamage_End
	printfromtable gSandstormHailDmgStringIds
	waitmessage DELAY_1SECOND
	orword HIT_MARKER, HITMARKER_x20 | HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_NON_ATTACK_DMG | HITMARKER_GRUDGE
	effectivenesssound
	flash BANK_ATTACKER
	graphicalhpupdate BANK_ATTACKER
	datahpupdate BANK_ATTACKER
	faintpokemon BANK_ATTACKER, FALSE, 0x0
	bicword HIT_MARKER, HITMARKER_x20 | HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_NON_ATTACK_DMG | HITMARKER_GRUDGE
BattleScript_WeatherDamage_End:
	end2

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_FogEnded:
	setword BATTLE_STRING_LOADER FogEndedString
	printstring 0x184
	waitmessage DELAY_1SECOND
	end2

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_FogContinues:
	setword BATTLE_STRING_LOADER FogContinuesString
	printstring 0x184
	waitmessage DELAY_1SECOND
BattleScript_FogContinuesPlayAnim:
	playanimation 0x0 ANIM_FOG_CONTINUES 0x0
	end2

BattleScript_FogContinuesNoString:
	call BS_FLUSH_MESSAGE_BOX
	goto BattleScript_FogContinuesPlayAnim

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_SeaOfFireDamage:
	playanimation2 BANK_ATTACKER ANIM_ARG_1 0x0
	printstring 0x184
	waitmessage DELAY_1SECOND
	goto BattleScript_DoTurnDmg

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_GrassyTerrainHeal:
	playanimation BANK_ATTACKER ANIM_GRASSY_TERRAIN_HEAL 0x0
	orword HIT_MARKER 0x100
	graphicalhpupdate BANK_ATTACKER
	datahpupdate BANK_ATTACKER
	setword BATTLE_STRING_LOADER GrassyTerrainHealString
	printstring 0x184
	waitmessage DELAY_1SECOND
	end2

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_AquaRing:
	playanimation BANK_ATTACKER ANIM_AQUA_RING_HEAL 0x0
	orword HIT_MARKER 0x100
	graphicalhpupdate BANK_ATTACKER
	datahpupdate BANK_ATTACKER
	setword BATTLE_STRING_LOADER AquaRingHealString
	printstring 0x184
	waitmessage DELAY_1SECOND
	end2

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_LeechSeedTurnDrain:
	playanimation BANK_ATTACKER, ANIM_LEECH_SEED_DRAIN, ANIM_ARG_1
	orword HIT_MARKER, HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_NON_ATTACK_DMG
	graphicalhpupdate BANK_ATTACKER @;Attacker being the mon losing health
	datahpupdate BANK_ATTACKER
	copyword DAMAGE_LOC HP_DEALT
	callasm TryManipulateDamageForLeechSeedBigRoot
	jumpifability BANK_ATTACKER, ABILITY_LIQUIDOOZE, BattleScript_LeechSeedTurnPrintLiquidOoze
	manipulatedamage 0x0
	setbyte MULTISTRING_CHOOSER, 0x3
	goto BattleScript_LeechSeedTurnPrintAndUpdateHp

BattleScript_LeechSeedTurnPrintLiquidOoze:
	setbyte MULTISTRING_CHOOSER, 0x4

BattleScript_LeechSeedTurnPrintAndUpdateHp:
	jumpifcounter BANK_TARGET HEAL_BLOCK_TIMERS NOTEQUALS 0x0 BattleScript_LeechSeedTurnPrintAndUpdateHp_SkipForHealBlock
	orword HIT_MARKER, HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_NON_ATTACK_DMG
	graphicalhpupdate BANK_TARGET
	datahpupdate BANK_TARGET
BattleScript_LeechSeedTurnPrintAndUpdateHp_SkipForHealBlock:
	printfromtable 0x83FE558 @;gLeechSeedStringIds
	waitmessage DELAY_1SECOND
	faintpokemon BANK_ATTACKER 0x0 0x0
	faintpokemon BANK_TARGET 0x0 0x0
	end2

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_PoisonHeal:
	call BattleScript_AbilityPopUp
	playanimation BANK_ATTACKER ANIM_HEALING_SPARKLES 0x0
	orword HIT_MARKER 0x100
	graphicalhpupdate BANK_ATTACKER
	datahpupdate BANK_ATTACKER
	printstring 0x4B @;STRINGID_PKMNREGAINEDHEALTH
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	end2

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_FrostbiteTurnDmg:
	setword BATTLE_STRING_LOADER gText_HurtByFrostbite
	printstring 0x184
	waitmessage 0x40
	goto BattleScript_DoStatusTurnDmg

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_SplintersTurnDmg:
	playanimation BANK_ATTACKER ANIM_SPLINTER_DAMAGE 0x0
	setword BATTLE_STRING_LOADER gText_HurtBySplinters
	printstring 0x184
	waitmessage DELAY_1SECOND
	goto BattleScript_DoTurnDmg

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_BadThoughtsTurnDmg:
	playanimation BANK_ATTACKER ANIM_WAITING_WAGGLE 0x0
	setword BATTLE_STRING_LOADER gText_TormentedByBadThoughts
	printstring 0x184
	waitmessage DELAY_1SECOND
	goto BattleScript_DoTurnDmg

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_YawnMakesAsleep:
	statusanimation BANK_EFFECT
	printstring 0x23 @;STRINGID_PKMNFELLASLEEP
	waitmessage DELAY_HALFSECOND
	refreshhpbar BANK_EFFECT
	waitstateatk
	end2

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MagnetRiseEnd:
	clearspecialstatusbit BANK_ATTACKER STATUS3_LEVITATING
	setword BATTLE_STRING_LOADER MagnetRiseEndString
	printstring 0x184
	waitmessage DELAY_1SECOND
    end2

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_TelekinesisEnd:
	clearspecialstatusbit BANK_ATTACKER STATUS3_TELEKINESIS
	setword BATTLE_STRING_LOADER TelekinesisStringEndString
	printstring 0x184
	waitmessage DELAY_1SECOND
    end2

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_HealBlockEnd:
	setword BATTLE_STRING_LOADER HealBlockEndString
	printstring 0x184
	waitmessage DELAY_1SECOND
	end2

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_EmbargoEnd:
	setword BATTLE_STRING_LOADER EmbargoEndString
	printstring 0x184
	waitmessage DELAY_1SECOND
	end2

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_TailwindEnd:
	setword BATTLE_STRING_LOADER gText_TailwindWoreOff
	goto PrintTimerString

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_LuckyChantEnd:
	setword BATTLE_STRING_LOADER gText_LuckyChantWoreOff
	goto PrintTimerString

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_RainbowEnd:
	setword BATTLE_STRING_LOADER gText_RainbowDisappeared
	goto PrintTimerString

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_SeaOfFireEnd:
	setword BATTLE_STRING_LOADER gText_SeaOfFireDisappeared
	goto PrintTimerString

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_SwampEnd:
	setword BATTLE_STRING_LOADER gText_SwampDisappeared
	goto PrintTimerString

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_TrickRoomEnd:
	setword BATTLE_STRING_LOADER TrickRoomEndString
	goto PrintTimerString


BattleScript_WaterSportEnd:
	setword BATTLE_STRING_LOADER WaterSportEndString
	goto PrintTimerString


BattleScript_MudSportEnd:
	setword BATTLE_STRING_LOADER MudSportEndString
	goto PrintTimerString


BattleScript_WonderRoomEnd:
	setword BATTLE_STRING_LOADER WonderRoomEndString
	goto PrintTimerString


BattleScript_MagicRoomEnd:
	setword BATTLE_STRING_LOADER MagicRoomEndString
	goto PrintTimerString


BattleScript_GravityEnd:
	setword BATTLE_STRING_LOADER GravityEndString
	printstring 0x184
	waitmessage DELAY_1SECOND
	setbyte BATTLE_COMMUNICATION 0
	callasm PrintElectromagnetismFloatingStrings
	end2

BattleScript_TerrainEnd:
	setbyte TERRAIN_BYTE 0x0
	callasm TransferTerrainData
	waitstateatk
	playanimation 0x0 ANIM_LOAD_DEFAULT_BG 0x0
	setword BATTLE_STRING_LOADER TerrainEndString
	printstring 0x184
	waitmessage DELAY_1SECOND
	callasm TryActivateMimicry
	end2

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

PrintTimerString:
	printstring 0x184
	waitmessage DELAY_1SECOND
	end2
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_ToxicOrb:
	statusanimation BANK_EFFECT
	setword BATTLE_STRING_LOADER ToxicOrbString
	printstring 0x184
	waitmessage DELAY_1SECOND
	refreshhpbar BANK_EFFECT
	waitstateatk
	end2

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_FlameOrb:
	statusanimation BANK_EFFECT
	setword BATTLE_STRING_LOADER FlameOrbString
	printstring 0x184
	waitmessage DELAY_1SECOND
	refreshhpbar BANK_EFFECT
	waitstateatk
	end2

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_PowerConstruct:
	setword BATTLE_STRING_LOADER PresenceOfManyString
	printstring 0x184
	waitmessage DELAY_HALFSECOND
	call BattleScript_AbilityPopUp
	pause DELAY_HALFSECOND
	call BattleScript_AbilityPopUpRevert
	playanimation BANK_ATTACKER ANIM_ZYGARDE_CELL_SWIRL 0x0
	reloadhealthbar BANK_ATTACKER
	setword BATTLE_STRING_LOADER PowerConstructCompleteString
	printstring 0x184
	waitmessage DELAY_1SECOND
	end2

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_StartedSchooling:
	call BattleScript_StartedSchoolingRet
	end2

.global BattleScript_StartedSchoolingRet
BattleScript_StartedSchoolingRet:
	call BattleScript_AbilityPopUp
	playanimation BANK_SCRIPTING ANIM_WISHIWASHI_FISH 0x0
	setword BATTLE_STRING_LOADER gText_StartedSchooling
	printstring 0x184
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	return

BattleScript_StoppedSchooling:
	call BattleScript_StoppedSchoolingRet
	end2

.global BattleScript_StoppedSchoolingRet
BattleScript_StoppedSchoolingRet:
	call BattleScript_AbilityPopUp
	playanimation BANK_SCRIPTING ANIM_WISHIWASHI_FISH 0x0
	setword BATTLE_STRING_LOADER gText_StoppedSchooling
	printstring 0x184
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_ShieldsDownToCore:
	call BattleScript_ShieldsDownToCoreRet
	end2

.global BattleScript_ShieldsDownToCoreRet
BattleScript_ShieldsDownToCoreRet:
	call BattleScript_AbilityPopUp
	playanimation BANK_SCRIPTING ANIM_TRANSFORM 0x0
	setword BATTLE_STRING_LOADER ToCoreString
	printstring 0x184
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	return

BattleScript_ShieldsDownToMeteor:
	call BattleScript_ShieldsDownToMeteorRet
	end2

.global BattleScript_ShieldsDownToMeteorRet
BattleScript_ShieldsDownToMeteorRet:
	call BattleScript_AbilityPopUp
	playanimation BANK_SCRIPTING ANIM_TRANSFORM 0x0
	setword BATTLE_STRING_LOADER ToMeteorString
	printstring 0x184
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_FlowerGiftRet:
	jumpifability BANK_SCRIPTING ABILITY_FLOWERGIFT DoFlowerGiftChange
	goto SkipFlowerGiftPopUp

DoFlowerGiftChange:
	call BattleScript_AbilityPopUp

SkipFlowerGiftPopUp:
	playanimation BANK_SCRIPTING ANIM_TRANSFORM 0x0
	setword BATTLE_STRING_LOADER TransformedString
	printstring 0x184
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	return

BattleScript_FlowerGift:
	call BattleScript_FlowerGiftRet
	end3

BattleScript_FlowerGiftEnd2:
	call BattleScript_FlowerGiftRet
	end2

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_IceFaceRet:
	call BattleScript_AbilityPopUp
	playanimation BANK_SCRIPTING ANIM_TRANSFORM 0x0
	setword BATTLE_STRING_LOADER TransformedString
	printstring 0x184
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	return

BattleScript_IceFaceRestoreFace:
	call BattleScript_IceFaceRet
	end3

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MonTookFutureAttack:
	printstring 0xA2 @;STRINGID_PKMNTOOKATTACK
	waitmessage 0x10
	jumpifmove MOVE_DOOMDESIRE BattleScript_CheckDoomDesireMiss
	accuracycheck BattleScript_FutureAttackMiss MOVE_FUTURESIGHT
	goto BattleScript_CalcDamage

BattleScript_CheckDoomDesireMiss:
	accuracycheck BattleScript_FutureAttackMiss MOVE_DOOMDESIRE

BattleScript_CalcDamage:
	critcalc
	callasm TryUseGemFutureSight
	callasm FutureSightDamageCalc
	callasm FutureSightTypeCalc
	adjustnormaldamage2
	callasm TryActivateWeakenessBerryFutureSight
	jumpifmove MOVE_DOOMDESIRE BattleScript_FutureHitAnimDoomDesire
	playanimation BANK_ATTACKER ANIM_FUTURE_SIGHT_HIT 0x0
	goto BattleScript_DoFutureAttackHit

BattleScript_FutureHitAnimDoomDesire:
	playanimation BANK_ATTACKER ANIM_DOOM_DESIRE_HIT 0x0

BattleScript_DoFutureAttackHit:
	effectivenesssound
	flash BANK_TARGET
	waitstateatk
	graphicalhpupdate BANK_TARGET
	datahpupdate BANK_TARGET
	resultmessage
	waitmessage DELAY_1SECOND
	prefaintmoveendeffects 0x3
	faintpokemonaftermove
	ifwildbattleend BattleScript_FutureAttackEnd

BattleScript_FutureAttackEnd:
	setbyte CMD49_STATE 0x0
	cmd49 0x3 0x0
	setbyte OUTCOME 0
	end2
	
BattleScript_FutureAttackMiss:
	pause DELAY_HALFSECOND
	setbyte OUTCOME 0
	orbyte OUTCOME OUTCOME_FAILED
	resultmessage
	waitmessage DELAY_1SECOND
	setbyte OUTCOME 0
	end2

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

@;Affected by Mist and Abilities
BattleScript_OctolockTurnDmg:
	playanimation BANK_ATTACKER, ANIM_TURN_TRAP, ANIM_ARG_1
	setword BATTLE_STRING_LOADER gText_SqueezedByOctolock
	printstring 0x184
	waitmessage DELAY_1SECOND
	setbyte STAT_ANIM_PLAYED 0x0
	playstatchangeanimation BANK_ATTACKER, STAT_ANIM_DEF | STAT_ANIM_SPDEF, STAT_ANIM_DOWN | STAT_ANIM_ONLY_MULTIPLE

BattleScript_OctolockTurnDmg_Def:
	playstatchangeanimation BANK_ATTACKER, STAT_ANIM_DEF, STAT_ANIM_DOWN
	setstatchanger STAT_DEF | DECREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_NOT_PROTECT_AFFECTED BattleScript_OctolockTurnDmgPrintDefMsg
BattleScript_OctolockTurnDmgPrintDefMsg:
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x3 BattleScript_OctolockTurnDmg_SpDef
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x4 BattleScript_OctolockTurnDmgEnd
	printfromtable gStatDownStringIds
	waitmessage DELAY_1SECOND

BattleScript_OctolockTurnDmg_SpDef:
	playstatchangeanimation BANK_ATTACKER, STAT_ANIM_SPDEF, STAT_ANIM_DOWN
	setstatchanger STAT_SPDEF | DECREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_NOT_PROTECT_AFFECTED BattleScript_OctolockTurnDmgPrintSpDefMsg
BattleScript_OctolockTurnDmgPrintSpDefMsg:
	jumpifbyte GREATERTHAN MULTISTRING_CHOOSER 0x2 BattleScript_OctolockTurnDmgEnd
	printfromtable gStatDownStringIds
	waitmessage DELAY_1SECOND

BattleScript_OctolockTurnDmgEnd:
	end2

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_DynamaxEnd:
	callasm UpdateHPForDynamax
	waitstateatk
	callasm UpdateCurrentHealthForDynamaxEnd
	waitstateatk
	reloadhealthbar BANK_SCRIPTING
	callasm SetAndTransferDontRemoveTransformSpecies
	jumpifsecondarystatus BANK_SCRIPTING STATUS2_TRANSFORMED BattleScript_DynamaxEnd_SpecialTransformAnim
	playanimation BANK_SCRIPTING ANIM_TRANSFORM 0x0
BattleScript_DynamaxEnd_Rejoin:
	waitanimation
	callasm ClearAndTransferDontRemoveTransformSpecies
	setword BATTLE_STRING_LOADER gText_DynamaxEnded
	printstring 0x184
	waitmessage DELAY_1SECOND
	end2

BattleScript_DynamaxEnd_SpecialTransformAnim:
	copybyte USER_BANK BATTLE_SCRIPTING_BANK
	copybyte TARGET_BANK BATTLE_SCRIPTING_BANK
	playanimation BANK_SCRIPTING ANIM_TRANSFORM_ATTACK 0x0
	goto BattleScript_DynamaxEnd_Rejoin

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global BattleScript_HoopaSOS
BattleScript_HoopaSOS:
	getswitchedmondata BANK_SWITCHING
	waitstateatk
	switchindataupdate BANK_SWITCHING
	callasm ShowSOSMon
	playanimation BANK_SWITCHING ANIM_HOOPA_RING_SPAWN 0x0
	callasm ShowSOSMonHealthbox
	copybyte USER_BANK, BATTLE_SCRIPTING_BANK @;For the string
	callasm SetTargetPartner
	setword BATTLE_STRING_LOADER gText_HoopaRespawnedAlly
	printstring 0x184
	waitmessage DELAY_1SECOND
	switchineffects BANK_SWITCHING
	end2

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_PrintCustomStringEnd2:
	call BattleScript_PrintCustomString
	end2

BattleScript_PrintCustomStringEnd3:
	call BattleScript_PrintCustomString
	end3

.align 2
FogEndedString: .byte 0xCE, 0xDC, 0xD9, 0x00, 0xDA, 0xE3, 0xDB, 0x00, 0xD8, 0xDD, 0xE7, 0xD5, 0xE4, 0xE4, 0xD9, 0xD5, 0xE6, 0xD9, 0xD8, 0xAD, 0xFF
FogContinuesString: .byte 0xCE, 0xDC, 0xD9, 0x00, 0xDA, 0xE3, 0xDB, 0x00, 0xDD, 0xE7, 0x00, 0xD8, 0xD9, 0xD9, 0xE4, 0xAD, 0xAD, 0xAD, 0xFF
GrassyTerrainHealString: .byte 0xCE, 0xDC, 0xD9, 0x00, 0xDB, 0xE6, 0xD5, 0xE7, 0xE7, 0xED, 0x00, 0xE8, 0xD9, 0xE6, 0xE6, 0xD5, 0xDD, 0xE2, 0x00, 0xE6, 0xD9, 0xE7, 0xE8, 0xE3, 0xE6, 0xD9, 0xD8, 0xFE, 0xFD, 0x0F, 0xB4, 0xE7, 0x00, 0xDC, 0xD9, 0xD5, 0xE0, 0xE8, 0xDC, 0xAB, 0xFF
AquaRingHealString: .byte 0xCE, 0xDC, 0xD9, 0x00, 0xBB, 0xE5, 0xE9, 0xD5, 0x00, 0xCC, 0xDD, 0xE2, 0xDB, 0x00, 0xE6, 0xD9, 0xE7, 0xE8, 0xE3, 0xE6, 0xD9, 0xD8, 0xFE, 0xFD, 0x0F, 0xB4, 0xE7, 0x00, 0xDC, 0xD9, 0xD5, 0xE0, 0xE8, 0xDC, 0xAB, 0xFF
MagnetRiseEndString: .byte 0xFD, 0x0F, 0xB4, 0xE7, 0xFE, 0xD9, 0xE0, 0xD9, 0xD7, 0xE8, 0xE6, 0xE3, 0xE1, 0xD5, 0xDB, 0xE2, 0xD9, 0xE8, 0xDD, 0xE7, 0xE1, 0x00, 0xEB, 0xE3, 0xE6, 0xD9, 0x00, 0xE3, 0xDA, 0xDA, 0xAB, 0xFF
TelekinesisStringEndString: .byte 0xFD, 0x0F, 0x00, 0xEB, 0xD5, 0xE7, 0x00, 0xDA, 0xE6, 0xD9, 0xD9, 0xD8, 0xFE, 0xDA, 0xE6, 0xE3, 0xE1, 0x00, 0xE8, 0xDC, 0xD9, 0x00, 0xE8, 0xD9, 0xE0, 0xD9, 0xDF, 0xDD, 0xE2, 0xD9, 0xE7, 0xDD, 0xE7, 0xAB, 0xFF
HealBlockEndString: .byte 0xFD, 0x0F, 0xB4, 0xE7, 0xFE, 0xC2, 0xD9, 0xD5, 0xE0, 0x00, 0xBC, 0xE0, 0xE3, 0xD7, 0xDF, 0x00, 0xEB, 0xE3, 0xE6, 0xD9, 0x00, 0xE3, 0xDA, 0xDA, 0xAB, 0xFF
EmbargoEndString: .byte 0xFD, 0x0F, 0xB4, 0xE7, 0xFE, 0xBF, 0xE1, 0xD6, 0xD5, 0xE6, 0xDB, 0xE3, 0x00, 0xD9, 0xE2, 0xD8, 0xD9, 0xD8, 0xAB, 0xFF
TrickRoomEndString: .byte 0xCE, 0xDC, 0xD9, 0x00, 0xE8, 0xEB, 0xDD, 0xE7, 0xE8, 0xD9, 0xD8, 0x00, 0xD8, 0xDD, 0xE1, 0xD9, 0xE2, 0xE7, 0xDD, 0xE3, 0xE2, 0xE7, 0x00, 0xE6, 0xD9, 0xE8, 0xE9, 0xE6, 0xE2, 0xD9, 0xD8, 0xFE, 0xE8, 0xE3, 0x00, 0xE2, 0xE3, 0xE6, 0xE1, 0xD5, 0xE0, 0xAB, 0xFF
WaterSportEndString: .byte 0xD1, 0xD5, 0xE8, 0xD9, 0xE6, 0x00, 0xCD, 0xE4, 0xE3, 0xE6, 0xE8, 0xB4, 0xE7, 0x00, 0xD9, 0xDA, 0xDA, 0xD9, 0xD7, 0xE8, 0xE7, 0x00, 0xEB, 0xE3, 0xE6, 0xD9, 0x00, 0xE3, 0xDA, 0xDA, 0xAB, 0xFF
MudSportEndString: .byte 0xC7, 0xE9, 0xD8, 0x00, 0xCD, 0xE4, 0xE3, 0xE6, 0xE8, 0xB4, 0xE7, 0x00, 0xD9, 0xDA, 0xDA, 0xD9, 0xD7, 0xE8, 0xE7, 0x00, 0xEB, 0xE3, 0xE6, 0xD9, 0x00, 0xE3, 0xDA, 0xDA, 0xAB, 0xFF
WonderRoomEndString: .byte 0xD1, 0xE3, 0xE2, 0xD8, 0xD9, 0xE6, 0x00, 0xCC, 0xE3, 0xE3, 0xE1, 0x00, 0xEB, 0xE3, 0xE6, 0xD9, 0x00, 0xE3, 0xDA, 0xDA, 0xB8, 0x00, 0xD5, 0xE2, 0xD8, 0x00, 0xE8, 0xDC, 0xD9, 0x00, 0xBE, 0xD9, 0xDA, 0xD9, 0xE2, 0xE7, 0xD9, 0xFE, 0xD5, 0xE2, 0xD8, 0x00, 0xCD, 0xE4, 0xAD, 0x00, 0xBE, 0xD9, 0xDA, 0xAD, 0x00, 0xE7, 0xE8, 0xD5, 0xE8, 0xE7, 0x00, 0xE6, 0xD9, 0xE8, 0xE9, 0xE6, 0xE2, 0xD9, 0xD8, 0x00, 0xE8, 0xE3, 0x00, 0xE2, 0xE3, 0xE6, 0xE1, 0xD5, 0xE0, 0xAB, 0xFF
MagicRoomEndString: .byte 0xC7, 0xD5, 0xDB, 0xDD, 0xD7, 0x00, 0xCC, 0xE3, 0xE3, 0xE1, 0x00, 0xEB, 0xE3, 0xE6, 0xD9, 0x00, 0xE3, 0xDA, 0xDA, 0xB8, 0x00, 0xD5, 0xE2, 0xD8, 0x00, 0xDC, 0xD9, 0xE0, 0xD8, 0x00, 0xDD, 0xE8, 0xD9, 0xE1, 0xE7, 0xB4, 0xFE, 0xD9, 0xDA, 0xDA, 0xD9, 0xD7, 0xE8, 0xE7, 0x00, 0xE6, 0xD9, 0xE8, 0xE9, 0xE6, 0xE2, 0xD9, 0xD8, 0x00, 0xE8, 0xE3, 0x00, 0xE2, 0xE3, 0xE6, 0xE1, 0xD5, 0xE0, 0xAB, 0xFF
GravityEndString: .byte 0xC1, 0xE6, 0xD5, 0xEA, 0xDD, 0xE8, 0xED, 0x00, 0xE6, 0xD9, 0xE8, 0xE9, 0xE6, 0xE2, 0xD9, 0xD8, 0x00, 0xE8, 0xE3, 0x00, 0xE2, 0xE3, 0xE6, 0xE1, 0xD5, 0xE0, 0xAB, 0xFF
TerrainEndString: .byte 0xCE, 0xDC, 0xD9, 0x00, 0xE8, 0xD9, 0xE6, 0xE6, 0xD5, 0xDD, 0xE2, 0x00, 0xE6, 0xD9, 0xE8, 0xE9, 0xE6, 0xE2, 0xD9, 0xD8, 0x00, 0xE8, 0xE3, 0x00, 0xE2, 0xE3, 0xE6, 0xE1, 0xD5, 0xE0, 0xAB, 0xFF
AbilityActivatedString: .byte 0xFD, 0x13, 0xB4, 0xE7, 0x00, 0xFD, 0x1A, 0xFE, 0xD5, 0xD7, 0xE8, 0xDD, 0xEA, 0xD5, 0xE8, 0xD9, 0xD8, 0xAB, 0xFF
PresenceOfManyString: .byte 0xD3, 0xE3, 0xE9, 0x00, 0xE7, 0xD9, 0xE2, 0xE7, 0xD9, 0x00, 0xE8, 0xDC, 0xD9, 0x00, 0xE4, 0xE6, 0xD9, 0xE7, 0xD9, 0xE2, 0xD7, 0xD9, 0x00, 0xE3, 0xDA, 0x00, 0xE1, 0xD5, 0xE2, 0xED, 0xAB, 0xFF
PowerConstructCompleteString: .byte 0xFD, 0x13, 0x00, 0xE8, 0xE6, 0xD5, 0xE2, 0xE7, 0xDA, 0xE3, 0xE6, 0xE1, 0xD9, 0xD8, 0xFE, 0xDD, 0xE2, 0xE8, 0xE3, 0x00, 0xDD, 0xE8, 0xE7, 0x00, 0xBD, 0xE3, 0xE1, 0xE4, 0xE0, 0xD9, 0xE8, 0xD9, 0x00, 0xC0, 0xE3, 0xE6, 0xE1, 0xAB, 0xFF
ToCoreString: .byte 0xFD, 0x13, 0xB4, 0xE7, 0x00, 0xFD, 0x1A, 0xFE, 0xD5, 0xD7, 0xE8, 0xDD, 0xEA, 0xD5, 0xE8, 0xD9, 0xD8, 0xAB, 0xFF
ToMeteorString: .byte 0xFD, 0x13, 0xB4, 0xE7, 0x00, 0xFD, 0x1A, 0xFE, 0xD8, 0xD9, 0xD5, 0xD7, 0xE8, 0xDD, 0xEA, 0xD5, 0xE8, 0xD9, 0xD8, 0xAB, 0xFF
TransformedString: .byte 0xFD, 0x13, 0x00, 0xE8, 0xE6, 0xD5, 0xE2, 0xE7, 0xDA, 0xE3, 0xE6, 0xE1, 0xD9, 0xD8, 0xAB, 0xFF
