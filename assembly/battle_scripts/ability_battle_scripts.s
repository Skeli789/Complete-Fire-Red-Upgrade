.text
.thumb
.align 2

.include "..\\defines"

.global BattleScript_OverworldWeatherStarts
.global BattleScript_NewWeatherAbilityActivates
.global BattleScript_IntimidateActivatesEnd3
.global BattleScript_TraceActivates
.global BattleScript_TerrainFromAbility
.global BattleScript_ImposterActivates
.global BattleScript_AttackerAbilityStatRaiseEnd3
.global BattleScript_SwitchInAbilityMsg
.global BattleScript_AbilityCuredStatus
.global BattleScript_AbilityCuredStatusEnd3
.global BattleScript_StartedSchoolingEnd3
.global BattleScript_StoppedSchoolingEnd3
.global BattleScript_ShieldsDownToCoreEnd3
.global BattleScript_ShieldsDownToMeteorEnd3
.global BattleScript_EmergencyExit
.global BattleScript_AbilityTransformed
.global BattleScript_TransformedEnd2
.global BattleScript_TransformedEnd3

.global BattleScript_RainDishActivates
.global BattleScript_DrySkinDamage
.global BattleScript_SolarPowerDamage
.global BattleScript_Healer
.global BattleScript_MoodySingleStat
.global BattleScript_MoodyRegular
.global BattleScript_BadDreams
.global BattleScript_Harvest
.global BattleScript_Pickup
.global BattleScript_AttackerCantUseMove
.global BattleScript_FlashFireBoost_PPLoss
.global BattleScript_FlashFireBoost
.global BattleScript_MoveHPDrain_PPLoss
.global BattleScript_MoveHPDrain
.global BattleScript_MonMadeMoveUseless_PPLoss
.global BattleScript_MonMadeMoveUseless
.global BattleScript_MoveStatDrain_PPLoss
.global BattleScript_MoveStatDrain
.global BattleScript_TargetAbilityStatRaise
.global BattleScript_AbilityApplySecondaryEffect
.global BattleScript_RoughSkinActivates
.global BattleScript_CuteCharmActivates
.global BattleScript_WeakArmorActivates
.global BattleScript_CursedBodyActivates
.global BattleScript_MummyActivates
.global BattleScript_GooeyActivates
.global BattleScript_IllusionBroken
.global BattleScript_IllusionBrokenFaint
.global BattleScript_AngerPointActivates
.global BattleScript_SynchronizeActivates

.global BattleScript_AbilityChangedType
.global BattleScript_AbilityChangedTypeContact
.global BattleScript_MimikyuTookDamage
.global BattleScript_MimikyuTransform
.global MimikyuDisguisedTookDamageString
.global BattleScript_EnduredSturdy
.global BattleScript_Receiver
.global BattleScript_Symbiosis
.global BattleScript_DefiantCompetitive
.global BattleScript_DefiantCompetitiveCall
.global BattleScript_SoulHeart
.global BattleScript_AbilityNoStatLoss
.global BattleScript_AbilityNoSpecificStatLoss
.global BattleScript_CastformChange
.global BattleScript_SturdyPreventsOHKO
.global BattleScript_StickyHoldActivates
.global BattleScript_DampStopsExplosion
.global BattleScript_TookAttack
.global BattleScript_AvoidedMoveWithAbility

.global BattleScript_AbilityPopUp
.global BattleScript_AbilityPopUpRevert

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_OverworldWeatherStarts:
	printfromtable gWeatherContinuesStringIds
	waitmessage DELAY_1SECOND
	playanimation2 BANK_ATTACKER ANIM_ARG_1 0x0
	end3
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_NewWeatherAbilityActivates:
	call BattleScript_AbilityPopUp
	printfromtable gWeatherAbilityStrings
	waitstateatk
	playanimation2 BANK_ATTACKER ANIM_ARG_1 0x0
	call BattleScript_AbilityPopUpRevert
	call 0x81D92DC @;BattleScript_WeatherFormChanges
	end3

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_IntimidateActivatesEnd3:
	call BattleScript_PauseIntimidateActivates
	end3

BattleScript_PauseIntimidateActivates:
	call BattleScript_AbilityPopUp
	setbyte TARGET_BANK 0x0
	setstatchanger STAT_ATK | DECREASE_1

BS_IntimidateActivatesLoop:
	trygetintimidatetarget BattleScript_IntimidateActivatesReturn
	jumpifbehindsubstitute BANK_TARGET IntimidateActivatesLoopIncrement
	statbuffchange STAT_TARGET | STAT_NOT_PROTECT_AFFECTED | STAT_BS_PTR IntimidateActivatesLoopIncrement
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 BattleScript_IntimidatePrevented
	setgraphicalstatchangevalues
	playanimation BANK_TARGET ANIM_STAT_BUFF ANIM_ARG_1
	printfromtable 0x83FE588
	waitmessage DELAY_1SECOND
	jumpifhelditemeffect BANK_TARGET ITEM_EFFECT_ADRENALINE_ORB BattleScript_AdrenalineOrb
	goto IntimidateActivatesLoopIncrement
	
BattleScript_AdrenalineOrb:
	setstatchanger STAT_SPD | INCREASE_1
	statbuffchange STAT_TARGET | STAT_NOT_PROTECT_AFFECTED | STAT_BS_PTR, IntimidateActivatesLoopIncrement
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 IntimidateActivatesLoopIncrement
	playanimation BANK_TARGET ANIM_ITEM_USE 0x0
	setgraphicalstatchangevalues
	playanimation BANK_TARGET ANIM_STAT_BUFF ANIM_ARG_1
	setword BATTLE_STRING_LOADER AdrenalineOrbString
	printstring 0x184
	waitmessage DELAY_1SECOND
	removeitem BANK_TARGET
	goto IntimidateActivatesLoopIncrement
	
BattleScript_IntimidatePrevented:
	pause DELAY_HALFSECOND
	printfromtable 0x83FE588
	waitmessage DELAY_1SECOND

IntimidateActivatesLoopIncrement:
	jumpifword NOTANDS BATTLE_TYPE BATTLE_DOUBLE BattleScript_IntimidateActivatesReturn
	addbyte TARGET_BANK 0x1
	goto BS_IntimidateActivatesLoop

BattleScript_IntimidateActivatesReturn:
	call BattleScript_AbilityPopUpRevert
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_TraceActivates:
	callasm TransferAbilityPopUpHelperAsTrace
	playanimation BANK_SCRIPTING ANIM_LOAD_ABILITY_POP_UP 0x0
	call BattleScript_AbilityPopUpRevert
	clearspecialstatusbit BANK_ATTACKER STATUS3_SWITCH_IN_ABILITY_DONE
	call BattleScript_AbilityPopUp
	printstring 0xD0 @;STRINGID_PKMNTRACED
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	tryactivateswitchinability BANK_SCRIPTING
	end3

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_TerrainFromAbility:
	call BattleScript_AbilityPopUp
	callasm TransferTerrainData
	playanimation2 BANK_ATTACKER ANIM_ARG_1 0x0
	copyarray BATTLE_STRING_LOADER SEED_HELPER 0x4
	printstring 0x184
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	call TerrainSeedCheck
	end3

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@


BattleScript_ImposterActivates:
	callasm TransferAbilityPopUpHelperAsImposter
	playanimation BANK_SCRIPTING ANIM_LOAD_ABILITY_POP_UP 0x0
	playanimation BANK_SCRIPTING ANIM_TRANSFORM_MOVE 0x0
	printfromtable 0x83FE5B4
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	clearspecialstatusbit BANK_SCRIPTING STATUS3_SWITCH_IN_ABILITY_DONE
	tryactivateswitchinability BANK_SCRIPTING
	end3

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_AttackerAbilityStatRaiseEnd3:
	call BattleScript_AttackerAbilityStatRaise
	end3	
	
BattleScript_AttackerAbilityStatRaise:
	call BattleScript_AbilityPopUp
	setgraphicalstatchangevalues
	playanimation BANK_ATTACKER ANIM_STAT_BUFF ANIM_ARG_1
	printstring 0xD5 @;STRINGID_PKMNSSTATCHANGED
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_SwitchInAbilityMsg:
	call BattleScript_AbilityPopUp
	printstring 0x184
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	end3	

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_AbilityCuredStatusEnd3:
	call BattleScript_AbilityCuredStatus
	end3
	
BattleScript_AbilityCuredStatus:
	call BattleScript_AbilityPopUp
	printstring 0x164 @;STRINGID_PKMNSXCUREDITSYPROBLEM
	waitmessage DELAY_1SECOND
	refreshhpbar BANK_SCRIPTING
	call BattleScript_AbilityPopUpRevert
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_StartedSchoolingEnd3:
	call BattleScript_StartedSchoolingRet
	end3

BattleScript_StoppedSchoolingEnd3:
	call BattleScript_StartedSchoolingRet
	end3

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_ShieldsDownToCoreEnd3:
	call BattleScript_ShieldsDownToCoreRet
	end3
	
BattleScript_ShieldsDownToMeteorEnd3:
	call BattleScript_ShieldsDownToMeteorRet
	end3

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_TransformedEnd2:
	call BattleScript_AbilityTransformed
	end2

BattleScript_TransformedEnd3:
	call BattleScript_AbilityTransformed
	end3

BattleScript_AbilityTransformed:
	call BattleScript_AbilityPopUp
	playanimation BANK_SCRIPTING ANIM_TRANSFORM 0x0
	setword BATTLE_STRING_LOADER TransformedString
	printstring 0x184
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_EmergencyExit:
	jumpifbattletype BATTLE_TRAINER EmergencyExitSwitchBS

EmergencyExitFleeBS:
	getifcantrunfrombattle BANK_SCRIPTING
	jumpifbyte EQUALS BATTLE_COMMUNICATION 0x1 EmergencyExitReturn
	call BattleScript_AbilityPopUp
	copyarray USER_BANK BATTLE_SCRIPTING_BANK 0x1
	printstring 0xA0 @;STRINGID_PKMNFLEDFROMBATTLE
	waitmessage DELAY_1SECOND
	setbyte BATTLE_OUTCOME 0x5 @;Teleported
	end

EmergencyExitSwitchBS:
	jumpifcannotswitch BANK_SCRIPTING EmergencyExitReturn
	call BattleScript_AbilityPopUp
	playanimation BANK_SCRIPTING ANIM_CALL_BACK_POKEMON 0x0
	callasm MakeScriptingBankInvisible
	call BattleScript_AbilityPopUpRevert
	openpartyscreen BANK_SCRIPTING EmergencyExitReturn
	switchoutabilities BANK_SCRIPTING
	waitstateatk
	switchhandleorder BANK_SCRIPTING 0x2
	returntoball BANK_SCRIPTING
	switch1 BANK_SCRIPTING
	switch2 BANK_SCRIPTING
	hpthresholds BANK_SCRIPTING
	printstring 0x3
	switch3 BANK_SCRIPTING 0x1
	waitstateatk
	switchineffects BANK_SCRIPTING
EmergencyExitReturn:
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_RainDishActivates:
	call BattleScript_AbilityPopUp
	playanimation BANK_ATTACKER ANIM_HEALING_SPARKLES 0x0
	orword HIT_MARKER HITMARKER_IGNORE_SUBSTITUTE
	graphicalhpupdate BANK_ATTACKER
	datahpupdate BANK_ATTACKER
	printstring 0x133
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	end3

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_DrySkinDamage:
BattleScript_SolarPowerDamage:
	call BattleScript_AbilityPopUp
	orword HIT_MARKER HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_NON_ATTACK_DMG
	graphicalhpupdate BANK_ATTACKER
	datahpupdate BANK_ATTACKER
	setword BATTLE_STRING_LOADER HurtByAbilityString
	printstring 0x184
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	faintpokemon BANK_ATTACKER 0x0 0x0
	end3
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_Healer:
	call BattleScript_AbilityPopUp
	setword BATTLE_STRING_LOADER, gText_HealerCuredStatusProblem
	printstring 0x184
	waitmessage DELAY_1SECOND
	refreshhpbar BANK_EFFECT
	call BattleScript_AbilityPopUpRevert
	end3

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MoodySingleStat:
	call BattleScript_AbilityPopUp
MoodySkipString:	
	statbuffchange STAT_ATTACKER MoodyStatRaiseEnd
	setgraphicalstatchangevalues
	playanimation BANK_ATTACKER 0x1 0x2023FD4
	printfromtable 0x83FE57C
	waitmessage DELAY_1SECOND
MoodyStatRaiseEnd:
	call BattleScript_AbilityPopUpRevert
	end3

BattleScript_MoodyRegular:
	call BattleScript_AbilityPopUp
	statbuffchange STAT_ATTACKER MoodyRegularP2
	setgraphicalstatchangevalues
	playanimation BANK_ATTACKER 0x1 0x2023FD4
	printfromtable 0x83FE57C
	waitmessage DELAY_1SECOND
	
MoodyRegularP2:
	callasm LoadMoodyStatToLower
	setbyte SEED_HELPER 0x0
	goto MoodySkipString

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_BadDreams:
	call BattleScript_AbilityPopUp
	setbyte SEED_HELPER 0x0
	jumpifbyte ANDS USER_BANK 0x1 BadDreams_OpponentSidePlayer1

BadDreams_PlayerSideOpponent1:
	setbyte TARGET_BANK 0x1
	jumpifstatus BANK_TARGET STATUS_SLEEP BadDreams_callhurt
	jumpifability BANK_TARGET ABILITY_COMATOSE BadDreams_callhurt
	addbyte SEED_HELPER 0x1
BadDreams_PlayerSideOpponent2:
	jumpifbyte NOTEQUALS NUM_POKEMON 0x4 BadDreams_end @not double battle
	setbyte TARGET_BANK 0x3
	jumpifstatus BANK_TARGET STATUS_SLEEP BadDreams_callhurt
	jumpifability BANK_TARGET ABILITY_COMATOSE BadDreams_callhurt
	goto BadDreams_end

BadDreams_OpponentSidePlayer1:
	setbyte SEED_HELPER 0x2
	setbyte TARGET_BANK 0x0
	jumpifstatus BANK_TARGET STATUS_SLEEP BadDreams_callhurt
	jumpifability BANK_TARGET ABILITY_COMATOSE BadDreams_callhurt
	addbyte SEED_HELPER 0x1
	BadDreams_OpponentSidePlayer2:
	jumpifbyte NOTEQUALS NUM_POKEMON 0x4 BadDreams_end @not double battle
	setbyte TARGET_BANK 0x2
	jumpifstatus BANK_TARGET STATUS_SLEEP BadDreams_callhurt
	jumpifability BANK_TARGET ABILITY_COMATOSE BadDreams_callhurt
	goto BadDreams_end

BadDreams_callhurt:
	call BadDreams_hurt
	jumpifbyte EQUALS SEED_HELPER 0x1 BadDreams_PlayerSideOpponent2
	jumpifbyte EQUALS SEED_HELPER 0x2 BadDreams_end
	jumpifbyte EQUALS SEED_HELPER 0x3 BadDreams_OpponentSidePlayer2
	jumpifbyte EQUALS SEED_HELPER 0x4 BadDreams_end
	goto BadDreams_end

BadDreams_hurt:
	jumpifability BANK_TARGET ABILITY_MAGICGUARD BadDreams_return
	callasm BadDreamsHurtFunc
	orword HIT_MARKER HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_NON_ATTACK_DMG
	graphicalhpupdate BANK_TARGET
	datahpupdate BANK_TARGET
	setword BATTLE_STRING_LOADER BadDreamsHurtString
	printstring 0x184
	waitmessage DELAY_1SECOND
	faintpokemon BANK_TARGET 0x0 0x0
	BadDreams_return:
	addbyte SEED_HELPER 0x1
	return

BadDreams_end:
	setbyte SEED_HELPER 0x0
	call BattleScript_AbilityPopUpRevert
	end3

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_Harvest:
	recycleitem HarvestBSEnd
	call BattleScript_AbilityPopUp
	setword BATTLE_STRING_LOADER HarvestString
	printstring 0x184
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	callasm HarvestActivateBerry

HarvestBSEnd:
	setbyte FORM_COUNTER 0x0
	end3

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_Pickup:
	call BattleScript_AbilityPopUp
	setword BATTLE_STRING_LOADER PickupString
	printstring 0x184
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	end3

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_AttackerCantUseMove:
	attackstring
	ppreduce
	pause DELAY_HALFSECOND
	call BattleScript_AbilityPopUp
	setword BATTLE_STRING_LOADER CantUseMoveString
	printstring 0x184
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_FlashFireBoost_PPLoss:
	ppreduce
BattleScript_FlashFireBoost:
	attackstring
	pause DELAY_HALFSECOND
	call BattleScript_AbilityPopUp
	printfromtable gFlashFireStringIds
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MoveHPDrain_PPLoss:
	ppreduce
BattleScript_MoveHPDrain:
	attackstring
	pause DELAY_HALFSECOND
	call BattleScript_AbilityPopUp
	playanimation BANK_TARGET ANIM_HEALING_SPARKLES 0x0
	orword HIT_MARKER HITMARKER_IGNORE_SUBSTITUTE
	healthbarupdate BANK_TARGET
	datahpupdate BANK_TARGET
	printstring 0x4B @;STRINGID_PKMNREGAINEDHEALTH
	waitmessage DELAY_1SECOND
	orbyte OUTCOME OUTCOME_NOT_AFFECTED
	call BattleScript_AbilityPopUpRevert
	goto BS_MOVE_END

BattleScript_MonMadeMoveUseless_PPLoss:
	ppreduce
BattleScript_MonMadeMoveUseless:
	orbyte OUTCOME OUTCOME_NOT_AFFECTED
	attackstring
	pause DELAY_HALFSECOND
	call BattleScript_AbilityPopUp
	printstring 0x1B @;STRINGID_ITDOESNTAFFECT
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MoveStatDrain_PPLoss:
	ppreduce
BattleScript_MoveStatDrain:
	orbyte OUTCOME OUTCOME_NOT_AFFECTED
	attackstring
	pause DELAY_HALFSECOND
	call BattleScript_TargetAbilityStatRaise
	goto BS_MOVE_END

BattleScript_TargetAbilityStatRaise:
	call BattleScript_AbilityPopUp
	statbuffchange BANK_TARGET BattleScript_TargetAbilityStatRaiseReturn
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 BattleScript_TargetAbilityStatRaiseReturn
	setgraphicalstatchangevalues
	playanimation BANK_TARGET ANIM_STAT_BUFF ANIM_ARG_1
	printstring 0xD8 @;STRINGID_PKMNSSTATCHANGED4
	waitmessage DELAY_1SECOND
	
BattleScript_TargetAbilityStatRaiseReturn:
	call BattleScript_AbilityPopUpRevert
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_AbilityApplySecondaryEffect:
	waitstateatk
	call BattleScript_AbilityPopUp
	waitstateatk
	seteffectuser
	call BattleScript_AbilityPopUpRevert
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_RoughSkinActivates:
	call BattleScript_AbilityPopUp
	orword HIT_MARKER, HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_NON_ATTACK_DMG
	healthbarupdate BANK_ATTACKER
	datahpupdate BANK_ATTACKER
	printstring 0xCF @;STRINGID_PKMNHURTSWITH
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	faintpokemon BANK_ATTACKER 0x0 0x0
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global DestinyKnotString
BattleScript_CuteCharmActivates:
	call BattleScript_AbilityPopUp
	status2animation BANK_ATTACKER STATUS2_INLOVE
	printstring 0x136
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	jumpifsecondarystatus BANK_TARGET STATUS2_INLOVE CuteCharmActivatesReturn
	jumpifability BANK_TARGET ABILITY_OBLIVIOUS CuteCharmActivatesReturn
	jumpifabilitypresenttargetfield ABILITY_AROMAVEIL CuteCharmActivatesReturn
	jumpifhelditemeffect BANK_ATTACKER ITEM_EFFECT_DESTINY_KNOT CuteCharmDestinyKnot

CuteCharmActivatesReturn:
	return

CuteCharmDestinyKnot:
	tryinfatuatebank BANK_TARGET CuteCharmActivatesReturn
	playanimation BANK_ATTACKER ANIM_ITEM_USE 0x0
	status2animation BANK_TARGET STATUS2_INLOVE
	copyarray BATTLE_SCRIPTING_BANK TARGET_BANK 0x1
	setword BATTLE_STRING_LOADER DestinyKnotString
	printstring 0x184
	waitmessage DELAY_1SECOND
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_WeakArmorActivates:
	jumpifstat BANK_TARGET GREATERTHAN STAT_DEF STAT_MIN WeakArmorModDef
	jumpifstat BANK_TARGET EQUALS STAT_SPD STAT_MAX WeakArmorReturn

WeakArmorModDef:
	call BattleScript_AbilityPopUp
	setstatchanger STAT_DEF | DECREASE_1
	statbuffchange BANK_TARGET | STAT_CERTAIN WeakArmorModSpd
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 WeakArmorModSpd
	setgraphicalstatchangevalues
	playanimation BANK_TARGET ANIM_STAT_BUFF ANIM_ARG_1
	printfromtable 0x83FE588
	waitmessage DELAY_1SECOND
	
WeakArmorModSpd:
	setstatchanger STAT_SPD | INCREASE_2
	statbuffchange BANK_TARGET | STAT_CERTAIN WeakArmorRevertPopUp
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 WeakArmorRevertPopUp
	setgraphicalstatchangevalues
	playanimation BANK_TARGET ANIM_STAT_BUFF ANIM_ARG_1
	printfromtable 0x83FE57C
	waitmessage DELAY_1SECOND
	
WeakArmorRevertPopUp:
	call BattleScript_AbilityPopUpRevert

WeakArmorReturn:
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_CursedBodyActivates:
	call BattleScript_AbilityPopUp
	disablelastusedattack BANK_ATTACKER CursedBodyReturn
	setword BATTLE_STRING_LOADER CursedBodyString
	printstring 0x184
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	
CursedBodyReturn:
	return
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MummyActivates:
	call BattleScript_AbilityPopUp
	pause DELAY_HALFSECOND
	call BattleScript_AbilityPopUpRevert
	copyarray BATTLE_SCRIPTING_BANK USER_BANK 0x1
	call BattleScript_AbilityPopUp
	pause DELAY_HALFSECOND
	call BattleScript_AbilityPopUpRevert
	setability BANK_ATTACKER ABILITY_MUMMY
	call BattleScript_AbilityPopUp
	setword BATTLE_STRING_LOADER MummyString
	printstring 0x184
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	
BattleScript_GooeyActivates:
	call BattleScript_AbilityPopUp
	setbyte STAT_ANIM_PLAYED 0x0
	playstatchangeanimation BANK_ATTACKER STAT_ANIM_SPD STAT_ANIM_DOWN
	setstatchanger STAT_SPD | DECREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR GooeyReturn
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 GooeyReturn
	printfromtable 0x83FE588
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert

GooeyReturn:
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_IllusionBroken:
	callasm TransferIllusionBroken
	call BattleScript_AbilityPopUp
	reloadhealthbar BANK_TARGET
	playanimation BANK_TARGET ANIM_TRANSFORM 0x0
	setword BATTLE_STRING_LOADER IllusionWoreOffString
	printstring 0x184
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	return
	
BattleScript_IllusionBrokenFaint:
	callasm TransferIllusionBroken
	call BattleScript_AbilityPopUp
	reloadhealthbar BANK_TARGET
	callasm CycleScriptingBankHealthBetween0And1
	playanimation BANK_TARGET ANIM_TRANSFORM 0x0
	callasm CycleScriptingBankHealthBetween0And1
	setword BATTLE_STRING_LOADER IllusionWoreOffString
	printstring 0x184
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_AngerPointActivates:
	call BattleScript_AbilityPopUp
	setstatchanger STAT_ATK | INCREASE_1
	setgraphicalstatchangevalues
	playanimation BANK_TARGET ANIM_STAT_BUFF ANIM_ARG_1
	setword BATTLE_STRING_LOADER AngerPointString
	printstring 0x184
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_SynchronizeActivates:
	pause 0x10
	call BattleScript_AbilityPopUp
	jumpifstatus BANK_TARGET STATUS_ANY SynchronizeNoEffect
	jumpifsideaffecting BANK_TARGET SIDE_SAFEGUARD SynchronizeSafeguard
	seteffecttarget
	jumpifstatus BANK_TARGET STATUS_ANY SynchronizeReturn

SynchronizeNoEffect:
	setword BATTLE_STRING_LOADER SynchronizeNoEffectString
	printstring 0x184
	waitmessage DELAY_1SECOND
	goto SynchronizeReturn
	
SynchronizeSafeguard:
	printstring 0x50
	waitmessage DELAY_1SECOND

SynchronizeReturn:
	call BattleScript_AbilityPopUpRevert
	callasm RestoreBanksFromSynchronize
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_AbilityChangedType:
	pause DELAY_HALFSECOND
	callasm DoProteanTypeChange
	call BattleScript_AbilityPopUp
	setword BATTLE_STRING_LOADER gText_AbilityChangedType
	printstring 0x184
	waitmessage DELAY_HALFSECOND
	call BattleScript_AbilityPopUpRevert
	return
	
BattleScript_AbilityChangedTypeContact:
	call BattleScript_AbilityPopUp
	setword BATTLE_STRING_LOADER gText_AbilityChangedType
	printstring 0x184
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MimikyuTookDamage:
	call BattleScript_AbilityPopUp
	setword BATTLE_STRING_LOADER MimikyuDisguisedTookDamageString
	printstring 0x184
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	return
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MimikyuTransform:
	playanimation BANK_TARGET ANIM_TRANSFORM 0x0
	setword BATTLE_STRING_LOADER DisguiseBustedString
	printstring 0x184
	waitmessage DELAY_1SECOND
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_EnduredSturdy:
	pause 0x10
	call BattleScript_AbilityPopUp
	printstring 0x99 @;STRINGID_PKMNENDUREDHIT
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_Receiver:
	playanimation BANK_SCRIPTING ANIM_LOAD_ABILITY_POP_UP 0x0
	pause DELAY_HALFSECOND
	call BattleScript_AbilityPopUpRevert
	call BattleScript_AbilityPopUp
	setword BATTLE_STRING_LOADER ReceiverString
	printstring 0x184
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	clearspecialstatusbit BANK_SCRIPTING STATUS3_SWITCH_IN_ABILITY_DONE
	tryactivateswitchinability BANK_SCRIPTING
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_Symbiosis:
	call BattleScript_AbilityPopUp
	setword BATTLE_STRING_LOADER SymbiosisString
	printstring 0x184
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_DefiantCompetitive:
	statbuffchange STAT_TARGET | STAT_BS_PTR DefiantReturn
	
BattleScript_DefiantCompetitiveCall:
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 DefiantReturn
	pause DELAY_HALFSECOND
	setbyte STAT_ANIM_PLAYED 0x0
	call BattleScript_AbilityPopUp
	setgraphicalstatchangevalues
	playanimation BANK_SCRIPTING ANIM_STAT_BUFF ANIM_ARG_1
	printfromtable 0x83FE57C
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert

DefiantReturn:
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_SoulHeart:
	call BattleScript_AbilityPopUp
	playanimation BANK_SCRIPTING ANIM_STAT_BUFF ANIM_ARG_1
	setword BATTLE_STRING_LOADER AbilityRaisedStatString
	printstring 0x184
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_AbilityNoStatLoss:
	pause 0x10
	call BattleScript_AbilityPopUp
	copyarray BATTLE_SCRIPTING_BANK BATTLE_COMMUNICATION 0x1
	printstring 0xCE
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	setbyte MULTISTRING_CHOOSER 0x4
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_AbilityNoSpecificStatLoss:
	pause 0x10
	call BattleScript_AbilityPopUp
	printstring 0x135
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	setbyte MULTISTRING_CHOOSER 0x3
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_CastformChange:
	call BattleScript_CastformChangeRet
	end3

BattleScript_CastformChangeRet:
	jumpifability BANK_SCRIPTING ABILITY_FORECAST DoCastformTransform
	goto CastformChangeSkipAbilityPopUp @;Reverting back b/c no longer has ability
	
DoCastformTransform:
	call BattleScript_AbilityPopUp
	
CastformChangeSkipAbilityPopUp:
	castformtransform
	waitstateatk
	printstring 0x13A @;STRINGID_PKMNTRANSFORMED
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_SturdyPreventsOHKO:
	pause 0x10
	call BattleScript_AbilityPopUp
	printstring 0x1B @;STRINGID_ITDOESNTAFFECT
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_StickyHoldActivates:
	pause 0x10
	call BattleScript_AbilityPopUp
	printstring 0x1B @;STRINGID_ITDOESNTAFFECT
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	goto BS_MOVE_END

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_DampStopsExplosion:
	attackstring
	ppreduce
	pause DELAY_HALFSECOND
	call BattleScript_AbilityPopUp
	setword BATTLE_STRING_LOADER CantUseMoveString
	printstring 0x184
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	goto BS_MOVE_END
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_TookAttack:
	attackstring
	pause 0x10
	call BattleScript_AbilityPopUp
	printstring 0x14E @;STRINGID_PKMNSXTOOKATTACK
	waitmessage DELAY_HALFSECOND
	call BattleScript_AbilityPopUpRevert
	orword HIT_MARKER HITMARKER_ATTACKSTRING_PRINTED
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_AvoidedMoveWithAbility:
	call BattleScript_AbilityPopUp
	printstring 0x1B @;STRINGID_ITDOESNTAFFECT
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_AbilityPopUp:
	callasm TransferAbilityPopUpHelper
	recordlastability BANK_SCRIPTING
	playanimation BANK_SCRIPTING ANIM_LOAD_ABILITY_POP_UP 0x0
	return

BattleScript_AbilityPopUpRevert:
	playanimation BANK_SCRIPTING ANIM_REMOVE_ABILITY_POP_UP 0x0
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

AdrenalineOrbString: .byte 0xFD, 0x10, 0xB4, 0xE7, 0x00, 0xBB, 0xD8, 0xE6, 0xD9, 0xE2, 0xD5, 0xE0, 0xDD, 0xE2, 0xD9, 0x00, 0xC9, 0xE6, 0xD6, 0xFE, 0xE6, 0xD5, 0xDD, 0xE7, 0xD9, 0xD8, 0x00, 0xDD, 0xE8, 0xE7, 0x00, 0xCD, 0xE4, 0xD9, 0xD9, 0xD8, 0xAB, 0xFF
HurtByAbilityString: .byte 0xFD, 0x10, 0x00, 0xEB, 0xD5, 0xE7, 0x00, 0xDC, 0xE9, 0xE6, 0xE8, 0xAB, 0xFF
BadDreamsHurtString: .byte 0xFD, 0x10, 0x00, 0xDD, 0xE7, 0x00, 0xE8, 0xE3, 0xE6, 0xE1, 0xD9, 0xE2, 0xE8, 0xD9, 0xD8, 0xAB, 0xFF
HarvestString: .byte 0xFD, 0x0F, 0x00, 0xDC, 0xD5, 0xE6, 0xEA, 0xD9, 0xE7, 0xE8, 0xD9, 0xD8, 0xFE, 0xE3, 0xE2, 0xD9, 0x00, 0xFD, 0x16, 0xAB, 0xFF
PickupString: .byte 0xFD, 0x13, 0x00, 0xDA, 0xE3, 0xE9, 0xE2, 0xD8, 0x00, 0xE3, 0xE2, 0xD9, 0xFE, 0xFD, 0x16, 0xAB, 0xFF
DestinyKnotString: .byte 0xFD, 0x13, 0x00, 0xDA, 0xD9, 0xE0, 0xE0, 0x00, 0xDD, 0xE2, 0x00, 0xE0, 0xE3, 0xEA, 0xD9, 0xFE, 0xDA, 0xE6, 0xE3, 0xE1, 0x00, 0xE8, 0xDC, 0xD9, 0x00, 0xFD, 0x16, 0xAB, 0xFF
CursedBodyString: .byte 0xFD, 0x10, 0xB4, 0xE7, 0x00, 0xFD, 0x19, 0xFE, 0xD8, 0xDD, 0xE7, 0xD5, 0xD6, 0xE0, 0xD9, 0xD8, 0x00, 0xFD, 0x0F, 0xB4, 0xE7, 0x00, 0xFD, 0x14, 0xAB, 0xFF
MummyString: .byte 0xFD, 0x0F, 0xB4, 0xE7, 0x00, 0xD5, 0xD6, 0xDD, 0xE0, 0xDD, 0xE8, 0xED, 0xFE, 0xD6, 0xD9, 0xD7, 0xD5, 0xE1, 0xD9, 0x00, 0xFD, 0x19, 0xAB, 0xFF
AngerPointString: .byte 0xFD, 0x10, 0xB4, 0xE7, 0x00, 0xFD, 0x19, 0xFE, 0xE1, 0xD5, 0xEC, 0xD9, 0xD8, 0x00, 0xDD, 0xE8, 0xE7, 0x00, 0xFD, 0x0, 0xAB, 0xFF
IllusionWoreOffString: .byte 0xFD, 0x10, 0xB4, 0xE7, 0x00, 0xDD, 0xE0, 0xE0, 0xE9, 0xE7, 0xDD, 0xE3, 0xE2, 0xFE, 0xEB, 0xE3, 0xE6, 0xD9, 0x00, 0xE3, 0xDA, 0xDA, 0xAB, 0xFF
SynchronizeNoEffectString: .byte 0xC3, 0xE8, 0x00, 0xDC, 0xD5, 0xD8, 0x00, 0xE2, 0xE3, 0x00, 0xD9, 0xDA, 0xDA, 0xD9, 0xD7, 0xE8, 0x00, 0xE3, 0xE2, 0xFE, 0xFD, 0x10, 0xB0, 0xFF

MimikyuDisguisedTookDamageString: .byte 0xFD, 0x10, 0xB4, 0xE7, 0x00, 0xD8, 0xDD, 0xE7, 0xDB, 0xE9, 0xDD, 0xE7, 0xD9, 0xFE, 0xE7, 0xD9, 0xE6, 0xEA, 0xD9, 0xD8, 0x00, 0xD5, 0xE7, 0x00, 0xDD, 0xE8, 0xE7, 0x00, 0xD8, 0xD9, 0xD7, 0xE3, 0xED, 0xAB, /*0xFB,*/ 0xFF
DisguiseBustedString: .byte 0xFD, 0x10, 0xB4, 0xE7, 0x00, 0xD8, 0xDD, 0xE7, 0xDB, 0xE9, 0xDD, 0xE7, 0xD9, 0xFE, 0xEB, 0xD5, 0xE7, 0x00, 0xD6, 0xE9, 0xE7, 0xE8, 0xD9, 0xD8, 0xAB, /*0xFB,*/ 0xFF
ReceiverString: .byte 0xFD, 0x13, 0x00, 0xE8, 0xE3, 0xE3, 0xDF, 0x00, 0xE3, 0xEA, 0xD9, 0xE6, 0x00, 0xFE, 0xFD, 0x11, 0xB4, 0xE7, 0x00, 0xFD, 0x1A, 0xAB, 0xFF
SymbiosisString: .byte 0xFD, 0x13, 0x00, 0xE7, 0xDC, 0xD5, 0xE6, 0xD9, 0xD8, 0x00, 0xDD, 0xE8, 0xE7, 0xFE, 0xFD, 0x16, 0x00, 0xEB, 0xDD, 0xE8, 0xDC, 0x00, 0xFD, 0x11, 0xFA, 0xE9, 0xE7, 0xDD, 0xE2, 0xDB, 0x00, 0xFD, 0x1A, 0xAB, 0xFF
DefiantString: .byte 0xFD, 0x13, 0xB4, 0xE7, 0x00, 0xFD, 0x1A, 0xFE, 0xE7, 0xDC, 0xD5, 0xE6, 0xE4, 0xE0, 0xED, 0x00, 0xE6, 0xD5, 0xDD, 0xE7, 0xD9, 0xD8, 0x00, 0xDD, 0xE8, 0xE7, 0x00, 0xFD, 0x00, 0xAB, 0xFF

.align 2
LoadMoodyStatToLower:
	ldr r0, =SEED_HELPER
	ldrb r0, [r0]
	ldr r1, =0x2023FD4
	mov r2, #0x15
	add r2, r0
	strb r2, [r1]
	
	ldr r1, =STAT_CHANGE_BYTE
	mov r2, #0x90
	orr r2, r0
	strb r2, [r1]
	bx lr
