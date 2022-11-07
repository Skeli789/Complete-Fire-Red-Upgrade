.text
.thumb
.align 2
/*
switch_battle_scripts.s
	scripts for switching logic
*/

.include "../asm_defines.s"
.include "../battle_script_macros.s"

.equ OPEN_PARTY_ALLOW_CANCEL, 0x80

.global BattleScript_HealingWishHeal
.global BattleScript_LunarDanceHeal

.global BattleScript_PrimalWeatherEnd
.global PrimalRainEndString
.global PrimalSunEndString
.global PrimalAirCurrentEndString

.global BattleScript_SpikesHurt
.global BattleScript_SRHurt
.global BattleScript_SteelsurgeHurt
.global BattleScript_TSPoison
.global BattleScript_TSHarshPoison
.global BattleScript_TSAbsorb
.global BattleScript_StickyWebSpeedDrop
.global BattleScript_SuccessForceOut

.global BattleScript_HandleFaintedMonSingles
.global BattleScript_HandleFaintedMonDoublesInitial
.global BattleScript_HandleFaintedMonDoublesPart2
.global BattleScript_HandleFaintedMonDoublesSwitchInEffects
.global BattleScript_FaintedMonChooseAnotherRejoin
.global BattleScript_FaintedMonTryChooseAnother

.global BattleScript_EntryHazardsHurtReturn

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_HealingWishHeal:
	playanimation BANK_SCRIPTING ANIM_HEALING_WISH_HEAL 0x0
	setword BATTLE_STRING_LOADER HealingWishHealString
	goto Sparkles

BattleScript_LunarDanceHeal:
	callasm RestorePPLunarDance
	playanimation BANK_SCRIPTING ANIM_LUNAR_DANCE_HEAL 0x0
	setword BATTLE_STRING_LOADER LunarDanceHealString

Sparkles:
	printstring 0x184
	waitmessage DELAY_1SECOND
	playanimation BANK_SCRIPTING ANIM_HEALING_SPARKLES 0x0
	graphicalhpupdate BANK_SCRIPTING
	datahpupdate BANK_SCRIPTING
	refreshhpbar BANK_SCRIPTING
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_SpikesHurt:
	orword HIT_MARKER 0x100100
	graphicalhpupdate BANK_TARGET
	datahpupdate BANK_TARGET
	setword BATTLE_STRING_LOADER gText_HurtBySpikes
	printstring 0x184
	waitmessage DELAY_1SECOND
	faintpokemon BANK_TARGET 0x0 0x0
	faintpokemon BANK_TARGET TRUE BattleScript_DmgHazardsOnTargetFainted
	return

BattleScript_DmgHazardsOnTargetFainted:
	setbyte EXP_STATE, 0x0
	getexp BANK_TARGET
	setbyte CMD49_STATE, 0x0
	cmd49 0x0, 0x0
	callasm TryToStopNewMonFromSwitchingInAfterSRHurt
	goto BattleScript_HandleFaintedMonSingles

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	
BattleScript_SRHurt:
	orword HIT_MARKER 0x100100
	graphicalhpupdate BANK_TARGET
	datahpupdate BANK_TARGET
	setword BATTLE_STRING_LOADER gText_HurtByStealthRock
	printstring 0x184
	waitmessage DELAY_1SECOND
	faintpokemon BANK_TARGET 0x0 0x0
	faintpokemon BANK_TARGET TRUE BattleScript_DmgHazardsOnTargetFainted
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_SteelsurgeHurt:
	orword HIT_MARKER 0x100100
	graphicalhpupdate BANK_TARGET
	datahpupdate BANK_TARGET
	setword BATTLE_STRING_LOADER gText_HurtBySteelsurge
	printstring 0x184
	waitmessage DELAY_1SECOND
	faintpokemon BANK_TARGET 0x0 0x0
	faintpokemon BANK_TARGET TRUE BattleScript_DmgHazardsOnTargetFainted
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	
BattleScript_TSPoison:
	setbyte POISONED_BY 0x2 @To-Do, modify PSN set script
	orword HIT_MARKER HITMARKER_IGNORE_SAFEGUARD | HITMARKER_IGNORE_SUBSTITUTE @;Ignore Shield Dust
	setmoveeffect MOVE_EFFECT_POISON
	seteffectprimary
	bicword HIT_MARKER HITMARKER_IGNORE_SAFEGUARD | HITMARKER_IGNORE_SUBSTITUTE
	setbyte POISONED_BY 0x0
	return
	
BattleScript_TSHarshPoison:
	setbyte POISONED_BY 0x2
	orword HIT_MARKER HITMARKER_IGNORE_SAFEGUARD | HITMARKER_IGNORE_SUBSTITUTE @;Ignore Shield Dust
	setmoveeffect MOVE_EFFECT_TOXIC
	seteffectprimary
	bicword HIT_MARKER HITMARKER_IGNORE_SAFEGUARD | HITMARKER_IGNORE_SUBSTITUTE
	setbyte POISONED_BY 0x0
	return

BattleScript_TSAbsorb:
	setword BATTLE_STRING_LOADER gText_AbsorbedToxicSpikes
	printstring 0x184
	waitmessage DELAY_1SECOND
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_StickyWebSpeedDrop:
	callasm SetStickyWebActive
	setword BATTLE_STRING_LOADER gText_CaughtInStickyWeb
	printstring 0x184
	waitmessage DELAY_HALFSECOND
	orword HIT_MARKER, HITMARKER_NON_ATTACK_DMG | HITMARKER_IGNORE_SAFEGUARD | HITMARKER_IGNORE_SUBSTITUTE @;Ignore Shield Dust
	jumpifability BANK_TARGET ABILITY_MIRRORARMOR BattleScript_MirrorArmorStickyWeb
	setstatchanger STAT_SPD | DECREASE_1
	statbuffchange STAT_TARGET | STAT_NOT_PROTECT_AFFECTED | STAT_BS_PTR BattleScript_StickyWebSpeedDropReturn
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 BattleScript_StickyWebSpeedDropReturn
	setgraphicalstatchangevalues
	playanimation BANK_TARGET ANIM_STAT_BUFF ANIM_ARG_1
	printfromtable gStatDownStringIds
	waitmessage DELAY_1SECOND
BattleScript_StickyWebSpeedDropReturn:
	callasm ClearStickyWebActive
	bicword HIT_MARKER, HITMARKER_NON_ATTACK_DMG | HITMARKER_IGNORE_SAFEGUARD | HITMARKER_IGNORE_SUBSTITUTE
	return
	
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_PrimalWeatherEnd:
	pause DELAY_HALFSECOND
	printstring 0x184
	waitmessage DELAY_1SECOND
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_SuccessForceOut:
	jumpifbyte EQUALS FORCE_SWITCH_HELPER 0x1 DragonTailForceSwitch
	jumpifbyte EQUALS FORCE_SWITCH_HELPER 0x2 RedCardForceSwitch
	attackanimation
	waitanimation

SkipRoarAnim:
	switchoutabilities BANK_SWITCHING
	returntoball BANK_SWITCHING
	waitstateatk
	jumpifword ANDS BATTLE_TYPE BATTLE_TRAINER ForceSwitch
	jumpifword ANDS BATTLE_TYPE BATTLE_DOUBLE ForceSwitch @Wild Double Battles force switches
	setbyte BATTLE_OUTCOME 0x5 @;Teleported
	finishaction

ForceSwitch:
	callasm TryRemovePrimalWeatherSwitchingBank
	getswitchedmondata BANK_SWITCHING
	switchindataupdate BANK_SWITCHING
	switchinanim BANK_SWITCHING 0x0
	waitstateatk
	printstring 0x154
	callasm MoldBreakerRemoveAbilitiesOnForceSwitchIn
	switchineffects BANK_SWITCHING
	callasm RestoreAllOriginalMoveData
	jumpifmove MOVE_DRAGONTAIL BattleScript_DragonTailResetForceSwitchHelper
	jumpifmove MOVE_CIRCLETHROW BattleScript_DragonTailResetForceSwitchHelper
	setbyte FORCE_SWITCH_HELPER 0x0
	goto BS_MOVE_END

DragonTailForceSwitch:
	playanimation BANK_TARGET DRAGON_TAIL_BLOW_AWAY_ANIM 0x0
	goto SkipRoarAnim

RedCardForceSwitch:
	playanimation BANK_ATTACKER DRAGON_TAIL_BLOW_AWAY_ANIM 0x0
	callasm ClearAttackerDidDamageOnce
	callasm TryRemovePrimalWeatherOnPivot
	switchoutabilities BANK_SWITCHING
	returntoball BANK_SWITCHING
	waitstateatk
	jumpifword ANDS BATTLE_TYPE BATTLE_TRAINER ForceSwitchRedCard
	jumpifword ANDS BATTLE_TYPE BATTLE_DOUBLE ForceSwitchRedCard @Wild Double Battles force switches
	setbyte BATTLE_OUTCOME 0x5 @;Teleported
	end

ForceSwitchRedCard:
	getswitchedmondata BANK_SWITCHING
	switchindataupdate BANK_SWITCHING
	switchinanim BANK_SWITCHING 0x0
	waitstateatk
	printstring 0x154
	callasm MoldBreakerRemoveAbilitiesOnForceSwitchIn
	switchineffects BANK_SWITCHING
	callasm MoldBreakerRestoreAbilitiesOnForceSwitchIn
	setbyte FORCE_SWITCH_HELPER 0x0
	callasm RestoreAllOriginalMoveData
	copybyte BATTLE_SCRIPTING_BANK, TARGET_BANK
	removeitem BANK_SCRIPTING
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_ActionSwitch:
	call BS_FLUSH_MESSAGE_BOX
	copybyte SWITCHING_BANK USER_BANK
	callasm SetSwitchingBankSwitchingCooldownTo2
	callasm BackupSwitchingBank
	hpthresholds2 BANK_SWITCHING
	atknameinbuff1
	printstring 0x2 @;STRINGID_RETURNMON
	setbyte DMG_MULTIPLIER 0x2
	jumpifbattletype BATTLE_DOUBLE BattleScript_PursuitSwitchDmgSetMultihit
	setbyte MULTI_HIT_COUNTER 0x1
	goto BattleScript_PursuitSwitchDmgLoop

BattleScript_PursuitSwitchDmgSetMultihit:
	setbyte MULTI_HIT_COUNTER 0x2

BattleScript_PursuitSwitchDmgLoop:
	jumpifnopursuitswitchdmg BattleScript_DoSwitchOut
	swapattackerwithtarget
	trysetdestinybondtohappen
	call BattleScript_PursuitDmgOnSwitchOut
	swapattackerwithtarget

BattleScript_DoSwitchOut:
	decrementmultihit BattleScript_PursuitSwitchDmgLoop
	callasm UpdatePreviousMonInOnSwitchout
	switchoutabilities BANK_SWITCHING
	waitstateatk
	returnatktoball
	waitstateatk
	drawpartystatussummary BANK_SWITCHING
	switchhandleorder BANK_SWITCHING 0x1
	getswitchedmondata BANK_SWITCHING
	switchindataupdate BANK_SWITCHING
	hpthresholds BANK_SWITCHING
	printstring 0x3 @;STRINGID_SWITCHINMON
	hidepartystatussummary BANK_SWITCHING
	switchinanim BANK_SWITCHING 0x0
	waitstateatk
	switchineffects BANK_SWITCHING
	callasm RechooseAIMoveAfterSwitchIfNecessary

HandleActionSwitchEnd:
BattleScript_EntryHazardsHurtReturn:
	end2
	
BattleScript_PursuitDmgOnSwitchOut:
	pause DELAY_HALFSECOND
	orword HIT_MARKER, HITMARKER_OBEYS
	setbyte FORCE_SWITCH_HELPER 0x0
	callasm MoldBreakerRemoveAbilitiesOnForceSwitchIn
	callasm SetDynamicTypeForPursuitSwitch
	call STANDARD_DAMAGE
	prefaintmoveendeffects 0x0
	faintpokemonaftermove
	setbyte CMD49_STATE, 0x0
	cmd49 0x4 0x0
	jumpiffainted BANK_TARGET BattleScript_PursuitGiveExp
	return

BattleScript_PursuitGiveExp:
	setbyte EXP_STATE 0x0
	getexp BANK_TARGET
	swapattackerwithtarget
	goto HandleActionSwitchEnd

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.equ BATTLE_STYLE_SHIFT, 0
.equ BATTLE_STYLE_SET, 1
.equ BATTLE_STYLE_SEMI_SHIFT, 2

BattleScript_HandleFaintedMonSingles:
	ifwildbattleend 0x81D87B8 @;BattleScript_LinkBattleHandleFaint
	jumpifbyte NOTEQUALS, BATTLE_OUTCOME, 0, BattleScript_FaintedMonEnd
	jumpifbattletype BATTLE_TRAINER, BattleScript_FaintedMonTryChooseAnother
	jumpifword NOTANDS, HIT_MARKER, HITMARKER_PLAYER_FAINTED, BattleScript_FaintedMonTryChooseAnother
	jumpifbattletype BATTLE_DOUBLE BattleScript_FaintedMonTryChooseAnother
	callasm SkipUseNextPkmnPromptIfCantRun
	printstring 337 @;STRINGID_USENEXTPKMN
	setbyte BATTLE_COMMUNICATION, 0
	yesnobox
	jumpifbyte EQUALS, BATTLE_COMMUNICATION + 1, 0, BattleScript_FaintedMonTryChooseAnother
	jumpifplayerran BattleScript_FaintedMonEnd
	printstring 274 @;STRINGID_CANTESCAPE2
BattleScript_FaintedMonTryChooseAnother:
	openpartyscreen BANK_FAINTED, BattleScript_FaintedMonEnd
	switchhandleorder BANK_FAINTED, 2
	jumpifnotbattletype BATTLE_TRAINER, BattleScript_FaintedMonChooseAnother
	jumpifbattletype BATTLE_WIRELESS, BattleScript_FaintedMonChooseAnother
	jumpifbattletype BATTLE_FRONTIER, BattleScript_FaintedMonChooseAnother
	jumpifbattletype BATTLE_DOUBLE, BattleScript_FaintedMonChooseAnother
	jumpifbyte EQUALS, BATTLE_STYLE, BATTLE_STYLE_SET, BattleScript_FaintedMonChooseAnother
	jumpifword ANDS, HIT_MARKER, HITMARKER_PLAYER_FAINTED, BattleScript_TryDoAIShiftSwitch
	jumpifcannotswitch BANK_PLAYER_1, BattleScript_FaintedMonChooseAnother
	jumpifbyte EQUALS, BATTLE_STYLE, BATTLE_STYLE_SEMI_SHIFT, BattleScript_FaintedMonChooseAnother_SemiSwitchString
	callasm HandleIllusionShiftSwitch @;Disguises the Pokemon's name if they have Illusion
	printstring 282 @;STRINGID_ENEMYABOUTTOSWITCHPKMN
BattleScript_FaintedMonChooseAnother_PostOfferString:
	setbyte BATTLE_COMMUNICATION, 0
	yesnobox
	jumpifbyte EQUALS, BATTLE_COMMUNICATION + 1, 1, BattleScript_FaintedMonChooseAnother
	callasm SetSwitchingBankToPlayer0
	setatktoplayer0
	openpartyscreen BANK_ATTACKER | OPEN_PARTY_ALLOW_CANCEL, BattleScript_FaintedMonChooseAnother
	switchhandleorder BANK_ATTACKER, 2
	jumpifbyte EQUALS, BATTLE_COMMUNICATION, 6, BattleScript_FaintedMonChooseAnother
	atknameinbuff1
	resetintimidatetracebits BANK_ATTACKER
	hpthresholds2 BANK_ATTACKER
	printstring 2 @;STRINGID_RETURNMON
	switchoutabilities BANK_ATTACKER
	waitstateatk
	returnatktoball
	waitstateatk
	drawpartystatussummary BANK_ATTACKER
	getswitchedmondata BANK_ATTACKER
	switchindataupdate BANK_ATTACKER
	hpthresholds BANK_ATTACKER
	printstring 3 @;STRINGID_SWITCHINMON
	hidepartystatussummary BANK_ATTACKER
	switchinanim BANK_ATTACKER, 0
	waitstateatk
	addindicatorforattackerswitchineffects
	resetsentmonsvalue
BattleScript_FaintedMonChooseAnother:
	drawpartystatussummary BANK_FAINTED
	getswitchedmondata BANK_FAINTED
	switchindataupdate BANK_FAINTED
BattleScript_FaintedMonChooseAnotherRejoin:
	hpthresholds BANK_FAINTED
	printstring 3 @;STRINGID_SWITCHINMON
	hidepartystatussummary BANK_FAINTED
	switchinanim BANK_FAINTED, 0
	waitstateatk
	resetplayerfaintedflag BANK_ATTACKER
	callasm ClearSwitchInEffectsState
BattleScript_FaintedMonEnd:
	end2

BattleScript_TryDoAIShiftSwitch:
	jumpifcannotswitch BANK_OPPONENT_1, BattleScript_FaintedMonChooseAnother
	drawpartystatussummary BANK_FAINTED
	getswitchedmondata BANK_FAINTED @;Load player data first, so AI can work off of it
	switchindataupdate BANK_FAINTED
	callasm ShouldDoAIShiftSwitch @;Jumps to BattleScript_FaintedMonChooseAnotherRejoin if fails
	callasm SetAttackerAndSwitchingBankToOpponent0
	openpartyscreen BANK_ATTACKER | OPEN_PARTY_ALLOW_CANCEL, BattleScript_FaintedMonChooseAnotherRejoin
	switchhandleorder BANK_ATTACKER, 2
	jumpifbyte EQUALS, BATTLE_COMMUNICATION, 6, BattleScript_FaintedMonChooseAnotherRejoin
	atknameinbuff1
	resetintimidatetracebits BANK_ATTACKER
	hpthresholds2 BANK_ATTACKER
	printstring 2 @;STRINGID_RETURNMON
	switchoutabilities BANK_ATTACKER
	waitstateatk
	returnatktoball
	waitstateatk
	drawpartystatussummary BANK_ATTACKER
	getswitchedmondata BANK_ATTACKER
	switchindataupdate BANK_ATTACKER
	hpthresholds BANK_ATTACKER
	printstring 3 @;STRINGID_SWITCHINMON
	hidepartystatussummary BANK_ATTACKER
	switchinanim BANK_ATTACKER, 0
	waitstateatk
	addindicatorforattackerswitchineffects
	resetsentmonsvalue
	copybyte BATTLE_SCRIPTING_BANK FAINTED_BANK
	callasm FaintedBankNameInBuff1
	goto BattleScript_FaintedMonChooseAnotherRejoin

BattleScript_FaintedMonChooseAnother_SemiSwitchString:
	setword BATTLE_STRING_LOADER gText_SemiSwitchOffer
	printstring 0x184
	goto BattleScript_FaintedMonChooseAnother_PostOfferString

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_HandleFaintedMonDoublesInitial:
	ifwildbattleend BattleScript_LinkBattleHandleFaintDoubles
	jumpifbyte NOTEQUALS BATTLE_OUTCOME 0 BattleScript_FaintedMonCancelSwitchIn
	jumpifnoviablemonsleft BANK_FAINTED BattleScript_FaintedMonCancelSwitchIn
	openpartyscreen BANK_FAINTED BattleScript_FaintedMonCancelSwitchIn
	switchhandleorder BANK_FAINTED 0x2
	getswitchedmondata BANK_FAINTED
	goto BattleScript_FaintedMonEnd
	
BattleScript_FaintedMonCancelSwitchIn:
	callasm RemoveSwitchInForFaintedBank
	end2
	
BattleScript_LinkBattleHandleFaintDoubles:
	openpartyscreen 0x5, .+4
	switchhandleorder BANK_FAINTED, 0x0
	openpartyscreen 0x6 BattleScript_FaintedMonEnd
	switchhandleorder BANK_FAINTED, 0x0	
	switchhandleorder BANK_FAINTED, 0x2
	getswitchedmondata BANK_FAINTED
	end2

BattleScript_HandleFaintedMonDoublesPart2:
	drawpartystatussummary BANK_FAINTED
	switchindataupdate BANK_FAINTED
	hpthresholds BANK_FAINTED
	printstring 0x3 @;STRINGID_SWITCHINMON
	hidepartystatussummary BANK_FAINTED
	switchinanim BANK_FAINTED, FALSE
	waitstateatk
	resetplayerfaintedflag BANK_ATTACKER
	end2

BattleScript_HandleFaintedMonDoublesSwitchInEffects:
	switchineffects BANK_FAINTED
	end2

.global BattleScript_DoPlayerAndFoeSwitchInEffects
BattleScript_DoPlayerAndFoeSwitchInEffects:
	setbyte USER_BANK 0x0
	switchineffects BANK_ATTACKER
	switchineffects BANK_FAINTED
	end2

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.align 2
HealingWishHealString: .byte 0xCE, 0xDC, 0xD9, 0x00, 0xC2, 0xD9, 0xD5, 0xE0, 0xDD, 0xE2, 0xDB, 0x00, 0xD1, 0xDD, 0xE7, 0xDC, 0x00, 0xD7, 0xD5, 0xE1, 0xD9, 0x00, 0xE8, 0xE6, 0xE9, 0xD9, 0xFE, 0xDA, 0xE3, 0xE6, 0x00, 0xFD, 0x0F, 0xAB, 0xFF
LunarDanceHealString: .byte 0xFD, 0x0F, 0x00, 0xD6, 0xD9, 0xD7, 0xD5, 0xE1, 0xD9, 0xFE, 0xD7, 0xE0, 0xE3, 0xD5, 0xDF, 0xD9, 0xD8, 0x00, 0xDD, 0xE2, 0x00, 0xE1, 0xED, 0xE7, 0xE8, 0xDD, 0xD7, 0xD5, 0xE0, 0x00, 0xE1, 0xE3, 0xE3, 0xE2, 0xE0, 0xDD, 0xDB, 0xDC, 0xE8, 0xAB, 0xFF

PrimalRainEndString: .byte 0xCE, 0xDC, 0xD9, 0x00, 0xDC, 0xD9, 0xD5, 0xEA, 0xED, 0x00, 0xE6, 0xD5, 0xDD, 0xE2, 0x00, 0xDC, 0xD5, 0xE7, 0x00, 0xE0, 0xDD, 0xDA, 0xE8, 0xD9, 0xD8, 0xAB, 0xFF
PrimalSunEndString: .byte 0xCE, 0xDC, 0xD9, 0x00, 0xD9, 0xEC, 0xE8, 0xE6, 0xD9, 0xE1, 0xD9, 0xE0, 0xED, 0x00, 0xDC, 0xD5, 0xE6, 0xE7, 0xDC, 0x00, 0xE7, 0xE9, 0xE2, 0xE0, 0xDD, 0xDB, 0xDC, 0xE8, 0x00, 0xDA, 0xD5, 0xD8, 0xD9, 0xD8, 0xAB, 0xFF
PrimalAirCurrentEndString: .byte 0xCE, 0xDC, 0xD9, 0x00, 0xE1, 0xED, 0xE7, 0xE8, 0xD9, 0xE6, 0xDD, 0xE3, 0xE9, 0xE7, 0x00, 0xD5, 0xDD, 0xE6, 0x00, 0xD7, 0xE9, 0xE6, 0xE6, 0xD9, 0xE2, 0xE8, 0x00, 0xDC, 0xD5, 0xE7, 0xFE, 0xD8, 0xDD, 0xE7, 0xE7, 0xDD, 0xE4, 0xD5, 0xE8, 0xD9, 0xD8, 0xAB, 0xFF
