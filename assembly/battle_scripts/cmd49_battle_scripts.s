.text
.thumb
.align 2
/*
cmd49_battle_scripts.s
	handles most end of move/turn effects
*/

.include "../asm_defines.s"
.include "../battle_script_macros.s"

.global BattleScript_CouldntFullyProtect
.global BattleScript_PoisonTouch
.global BattleScript_KingsShield
.global BattleScript_SpikyShield
.global BattleScript_BanefulBunker
.global BattleScript_ObstructStatDecrement
.global BattleScript_RageIsBuilding
.global BattleScript_BeakBlastBurn
.global BattleScript_Magician
.global BattleScript_Moxie
.global BattleScript_MindBlownDamage
.global BattleScript_FaintAttackerForExplosion
.global BattleScript_ExplosionAnim
.global BattleScript_ItemSteal
.global BattleScript_LifeOrbDamage
.global BattleScript_Pickpocket
.global BattleScript_DancerActivated
.global BattleScript_MultiHitPrintStrings
.global BattleScript_MultiHitWaitAttackerSubstitute
.global BattleScript_ScaleShotBuff
.global BattleScript_PluckEat
.global BattleScript_RaidShields
.global BattleScript_BrokenRaidBarrier
.global BattleScript_RaidBattleStatIncrease
.global BattleScript_MistProtected

.global ToxicOrbString
.global FlameOrbString

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_CouldntFullyProtect:
	waitmessage DELAY_1SECOND
	setword BATTLE_STRING_LOADER gText_CouldntFullyProtect
	printstring 0x184
	@;Relies on the waitmessage supposed to come after
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_PoisonTouch:
	setbyte POISONED_BY 0x1
	setbyte EFFECT_BYTE 0x2
	seteffectsecondary @;Affected by Safeguard
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_KingsShield:
	setstatchanger STAT_ATK | DECREASE_2
	
BattleScript_KingsShieldPostDecrementSet:
	swapattackerwithtarget @;Allows for abilities like Defiant and Mirror Armor to have their proper effect
	statbuffchange STAT_TARGET | STAT_NOT_PROTECT_AFFECTED | STAT_BS_PTR KingsShieldReturn
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 KingsShieldReturn
	setgraphicalstatchangevalues
	playanimation BANK_TARGET ANIM_STAT_BUFF ANIM_ARG_1
	printfromtable gStatDownStringIds
	waitmessage DELAY_1SECOND

KingsShieldReturn:
	swapattackerwithtarget
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_SpikyShield:
	orword HIT_MARKER, HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_NON_ATTACK_DMG
	graphicalhpupdate BANK_ATTACKER
	datahpupdate BANK_ATTACKER
	setword BATTLE_STRING_LOADER SpikyShieldString
	printstring 0x184
	waitmessage DELAY_1SECOND
	faintpokemon BANK_ATTACKER 0x0 0x0
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_BanefulBunker:
	statusanimation BANK_ATTACKER
	refreshhpbar BANK_ATTACKER
	setword BATTLE_STRING_LOADER BanefulBunkerPSNString
	printstring 0x184
	waitmessage DELAY_1SECOND
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_ObstructStatDecrement:
	setstatchanger STAT_DEF | DECREASE_2
	goto BattleScript_KingsShieldPostDecrementSet

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_RageIsBuilding:
	setstatchanger STAT_ATK | INCREASE_1
	statbuffchange STAT_TARGET | STAT_BS_PTR | STAT_CERTAIN RageReturn
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 RageReturn
	setgraphicalstatchangevalues
	playanimation BANK_TARGET ANIM_STAT_BUFF ANIM_ARG_1
	jumpifability BANK_TARGET ABILITY_CONTRARY RageContraryBS

RagePrintString:
	printstring 0x83
	waitmessage DELAY_1SECOND
	
RageReturn:
	return
	
RageContraryBS: @;Rage says "Attack fell!" if the target has Contrary. Only the rage string is printed if the stat rises
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	goto RagePrintString
	
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_BeakBlastBurn:
	orword HIT_MARKER 0x2100
	swapattackerwithtarget
	setbyte POISONED_BY 0x1
	setbyte EFFECT_BYTE 0x3
	seteffectprimary
	swapattackerwithtarget
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_Magician:
	call BattleScript_AbilityPopUp 
	setmoveeffect MOVE_EFFECT_STEAL_ITEM
	setmoveeffect2
	callasm TryHideActiveAbilityPopUps
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_Moxie:
	statbuffchange STAT_ATTACKER | STAT_BS_PTR .LReturn
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 .LReturn
	call BattleScript_AbilityPopUp
	playanimation BANK_ATTACKER ANIM_STAT_BUFF ANIM_ARG_1
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
.LReturn:
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MindBlownDamage:
	orword HIT_MARKER 0x100 | HITMARKER_NON_ATTACK_DMG
	graphicalhpupdate BANK_ATTACKER
	datahpupdate BANK_ATTACKER
	setword BATTLE_STRING_LOADER MindBlownString
	printstring 0x184
	waitmessage DELAY_1SECOND
	faintpokemon BANK_ATTACKER 0x0 0x0
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_ExplosionAnim:
	callasm PlayAttackAnimationForExplosion + 1
	waitanimation
	
BattleScript_FaintAttackerForExplosion:
	faintpokemon BANK_ATTACKER 0x0 0x0
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_LifeOrbDamage:
	orword HIT_MARKER 0x100 | HITMARKER_NON_ATTACK_DMG
	graphicalhpupdate BANK_ATTACKER
	datahpupdate BANK_ATTACKER
	setword BATTLE_STRING_LOADER LifeOrbString
	printstring 0x184
	waitmessage DELAY_1SECOND
	faintpokemon BANK_ATTACKER 0x0 0x0
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_Pickpocket:
	call BattleScript_AbilityPopUp
	setmoveeffect MOVE_EFFECT_STEAL_ITEM
	swapattackerwithtarget
	setmoveeffect2
	swapattackerwithtarget
	callasm TryActivateTargetEndTurnItemEffect
	callasm TryHideActiveAbilityPopUps
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_PoisonedBy:
	jumpifbyte EQUALS POISONED_BY 0x1 PoisonTouchPSN
	statusanimation 0x2
	jumpifbyte EQUALS POISONED_BY 0x2 ToxicSpikesPSN
	jumpifbyte EQUALS POISONED_BY 0x3 ToxicOrbBadPSN
	jumpifbyte EQUALS POISONED_BY 0x4 BanefulBunkerPSN
	printfromtable 0x83FE5BC
	waitmessage DELAY_1SECOND
	goto 0x81D91C3

PoisonTouchPSN:
	setbyte POISONED_BY 0x0
	call BattleScript_AbilityPopUp
	statusanimation 0x2
	printfromtable 0x83FE5BC
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	goto 0x81D91C3

ToxicSpikesPSN:
	setbyte POISONED_BY 0x0
	setword BATTLE_STRING_LOADER ToxicSpikesPSNString
	printstring 0x184
	waitmessage DELAY_1SECOND
	goto 0x81D91C3

BanefulBunkerPSN:
	setbyte POISONED_BY 0x0
	setword BATTLE_STRING_LOADER BanefulBunkerPSNString
	printstring 0x184
	waitmessage DELAY_1SECOND
	goto 0x81D91C3

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_BadPoisonedBy:
	statusanimation 0x2
	jumpifbyte EQUALS POISONED_BY 0x1 PoisonTouchPSN
	jumpifbyte EQUALS POISONED_BY 0x2 ToxicSpikesBadPSN
	jumpifbyte EQUALS POISONED_BY 0x3 ToxicOrbBadPSN
	jumpifbyte EQUALS POISONED_BY 0x4 BanefulBunkerPSN
	printstring 0x2C
	waitmessage DELAY_1SECOND
	goto 0x81D91C3

ToxicSpikesBadPSN:
	setbyte POISONED_BY 0x0
	setword BATTLE_STRING_LOADER ToxicSpikesBadPSNString
	printstring 0x184
	waitmessage DELAY_1SECOND
	goto 0x81D91C3

ToxicOrbBadPSN:
	setbyte POISONED_BY 0x0
	setword BATTLE_STRING_LOADER ToxicOrbString
	printstring 0x184
	waitmessage DELAY_1SECOND
	goto 0x81D91C3

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_BurnedBy:
	statusanimation 0x2
	jumpifbyte EQUALS POISONED_BY 0x1 BeakBlastBurnBS
	jumpifbyte EQUALS POISONED_BY 0x3 FlameOrbBurnBS
	printfromtable 0x83FE5C8
	waitmessage DELAY_1SECOND
	goto 0x81D91C3

BeakBlastBurnBS:
	setbyte POISONED_BY 0x0
	setword BATTLE_STRING_LOADER BeakBlastString
	printstring 0x184
	waitmessage DELAY_1SECOND
	goto 0x81D91C3

FlameOrbBurnBS:
	setbyte POISONED_BY 0x0
	setword BATTLE_STRING_LOADER FlameOrbString
	printstring 0x184
	waitmessage DELAY_1SECOND
	goto 0x81D91C3
	
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_ItemSteal:
	callasm TransferLastUsedItem
	playanimation BANK_TARGET ANIM_STEAL_ITEM 0x0
	printstring 0x8E
	waitmessage DELAY_1SECOND
	call 0x81D92DC @;BattleScript_WeatherFormChanges - In case of Utility Umbrella
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_DancerActivated:
	call BattleScript_AbilityPopUp
	pause 0x40
	call BattleScript_AbilityPopUpRevert
	jumptoattack 0xFF

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MultiHitPrintStrings:
	copyarray 0x2022AB8 MULTIHIT_STRING 0x6
	printstring 0x22
	waitmessage DELAY_1SECOND
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MultiHitWaitAttackerSubstitute:
	pause 0x30 @Wait for the attacker to slide in from the Substitute
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_ScaleShotBuff:
	jumpifstat BANK_ATTACKER GREATERTHAN STAT_DEF STAT_MIN ScaleShot_Def
	jumpifstat BANK_ATTACKER EQUALS STAT_SPD STAT_MAX .LReturn

ScaleShot_Def:
	setbyte STAT_ANIM_PLAYED 0x0
	playstatchangeanimation BANK_ATTACKER, STAT_ANIM_DEF, STAT_ANIM_DOWN | STAT_ANIM_IGNORE_ABILITIES 
	setstatchanger STAT_DEF | DECREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN, ScaleShot_Spd
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 ScaleShot_Spd
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND

ScaleShot_Spd:
	setbyte STAT_ANIM_PLAYED 0x0
	playstatchangeanimation BANK_ATTACKER, STAT_ANIM_SPD, STAT_ANIM_UP | STAT_ANIM_IGNORE_ABILITIES
	setstatchanger STAT_SPD | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN, .LReturn
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 .LReturn
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_PluckEat:
	setword BATTLE_STRING_LOADER PluckString
	printstring 0x184
	waitmessage DELAY_1SECOND
	callasm PluckBerryEat
	callasm ClearDoingPluckItemEffect
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_RaidShields:
	setword BATTLE_STRING_LOADER gText_RaidPokemonGettingDesperate
	printstring 0x184
	waitmessage DELAY_1SECOND
	callasm CreateRaidShieldSprites
	setword BATTLE_STRING_LOADER gText_RaidShield
	printstring 0x184
	waitmessage DELAY_1SECOND
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_BrokenRaidBarrier:
	setword BATTLE_STRING_LOADER gText_RaidShieldBroke
	printstring 0x184
	waitmessage DELAY_1SECOND
	orword HIT_MARKER, HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_NON_ATTACK_DMG
	graphicalhpupdate BANK_TARGET
	datahpupdate BANK_TARGET
	faintpokemon BANK_TARGET 0x0 0x0
	jumpiffainted BANK_TARGET BattleScript_BrokenRaidBarrierEnd
	setbyte STAT_ANIM_PLAYED 0x0
	playstatchangeanimation BANK_TARGET, STAT_ANIM_DEF | STAT_ANIM_SPDEF, STAT_ANIM_DOWN | STAT_ANIM_BY_TWO | STAT_ANIM_ONLY_MULTIPLE

BattleScript_BrokenRaidBarrier_Def:
	playstatchangeanimation BANK_TARGET, STAT_ANIM_DEF, STAT_ANIM_DOWN | STAT_ANIM_BY_TWO
	setstatchanger STAT_DEF | DECREASE_2
	statbuffchange STAT_TARGET | STAT_BS_PTR BattleScript_BrokenRaidBarrier_SpDef
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x3 BattleScript_BrokenRaidBarrier_SpDef
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x4 BattleScript_BrokenRaidBarrierEnd
	printfromtable gStatDownStringIds
	waitmessage DELAY_1SECOND

BattleScript_BrokenRaidBarrier_SpDef:
	playstatchangeanimation BANK_TARGET, STAT_ANIM_SPDEF, STAT_ANIM_DOWN | STAT_ANIM_BY_TWO
	setstatchanger STAT_SPDEF | DECREASE_2
	statbuffchange STAT_TARGET | STAT_BS_PTR BattleScript_BrokenRaidBarrierEnd
	jumpifbyte GREATERTHAN MULTISTRING_CHOOSER 0x2 BattleScript_BrokenRaidBarrierEnd
	printfromtable gStatDownStringIds
	waitmessage DELAY_1SECOND

BattleScript_BrokenRaidBarrierEnd:
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_RaidBattleStatIncrease:
	setgraphicalstatchangevalues
	playanimation BANK_SCRIPTING ANIM_STAT_BUFF ANIM_ARG_1
	setword BATTLE_STRING_LOADER gText_RaidBattleStatBoost
	printstring 0x184
	waitmessage DELAY_1SECOND
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MistProtected:
	pause DELAY_HALFSECOND
	printstring 98 @;STRINGID_PKMNPROTECTEDBYMIST
	waitmessage DELAY_1SECOND
	callasm TryHideActiveAbilityPopUps @;For Gooey
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.align 2
PoisonTouchString: .byte 0xFD, 0x0F, 0xB4, 0xE7, 0x00, 0xFD, 0x18, 0xFE, 0xE4, 0xE3, 0xDD, 0xE7, 0xE3, 0xE2, 0xD9, 0xD8, 0x00, 0xFD, 0x11, 0xAB, 0xFF
SpikyShieldString: .byte 0xFD, 0x0F, 0x00, 0xEB, 0xD5, 0xE7, 0x00, 0xDC, 0xE9, 0xE6, 0xE8, 0x00, 0xD6, 0xED, 0xFE, 0xFD, 0x10, 0xB4, 0xE7, 0x00, 0xCD, 0xE4, 0xDD, 0xDF, 0xED, 0x00, 0xCD, 0xDC, 0xDD, 0xD9, 0xE0, 0xD8, 0xAB, 0xFF
ToxicSpikesPSNString: .byte 0xFD, 0x11, 0x00, 0xEB, 0xD5, 0xE7, 0xFE, 0xE4, 0xE3, 0xDD, 0xE7, 0xE3, 0xE2, 0xD9, 0xD8, 0x00, 0xD6, 0xED, 0x00, 0xE8, 0xDC, 0xD9, 0x00, 0xCE, 0xE3, 0xEC, 0xDD, 0xD7, 0x00, 0xCD, 0xE4, 0xDD, 0xDF, 0xD9, 0xE7, 0xAB, 0xFF
ToxicSpikesBadPSNString: .byte 0xFD, 0x11, 0x00, 0xDD, 0xE7, 0x00, 0xD6, 0xD5, 0xD8, 0xE0, 0xED, 0xFE, 0xE4, 0xE3, 0xDD, 0xE7, 0xE3, 0xE2, 0xD9, 0xD8, 0x00, 0xD6, 0xED, 0x00, 0xE8, 0xDC, 0xD9, 0x00, 0xCE, 0xE3, 0xEC, 0xDD, 0xD7, 0x00, 0xCD, 0xE4, 0xDD, 0xDF, 0xD9, 0xE7, 0xAB, 0xFF
ToxicOrbString: .byte 0xFD, 0x11, 0x00, 0xEB, 0xD5, 0xE7, 0x00, 0xD6, 0xD5, 0xD8, 0xE0, 0xED, 0xFE, 0xE4, 0xE3, 0xDD, 0xE7, 0xE3, 0xE2, 0xD9, 0xD8, 0x00, 0xD6, 0xED, 0x00, 0xE8, 0xDC, 0xD9, 0x00, 0xFD, 0x16, 0xAB, 0xFF
BanefulBunkerPSNString: .byte 0xFD, 0x11, 0x00, 0xEB, 0xD5, 0xE7, 0xFE, 0xE4, 0xE3, 0xDD, 0xE7, 0xE3, 0xE2, 0xD9, 0xD8, 0x00, 0xD6, 0xED, 0x00, 0xE8, 0xDC, 0xD9, 0x00, 0xBC, 0xD5, 0xE2, 0xD9, 0xDA, 0xE9, 0xE0, 0x00, 0xBC, 0xE9, 0xE2, 0xDF, 0xD9, 0xE6, 0xAB, 0xFF
BeakBlastString: .byte 0xFD, 0x11, 0x00, 0xEB, 0xD5, 0xE7, 0x00, 0xD6, 0xE9, 0xE6, 0xE2, 0xD9, 0xD8, 0xFE, 0xD6, 0xED, 0x00, 0xFD, 0x0F, 0xB4, 0xE7, 0x00, 0xD6, 0xD9, 0xD5, 0xDF, 0xAB, 0xFF
FlameOrbString: .byte 0xFD, 0x11, 0x00, 0xEB, 0xD5, 0xE7, 0x00, 0xD6, 0xE9, 0xE6, 0xE2, 0xD9, 0xD8, 0xFE, 0xD6, 0xED, 0x00, 0xE8, 0xDC, 0xD9, 0x00, 0xFD, 0x16, 0xAB, 0xFF
MagicianStealString: .byte 0xFD, 0x10, 0xB4, 0xE7, 0x00, 0xFD, 0x16, 0xFE, 0xEB, 0xD5, 0xE7, 0x00, 0xE7, 0xE8, 0xE3, 0xE0, 0xD9, 0xE2, 0x00, 0xD6, 0xED, 0x00, 0xFD, 0x18, 0xAB, 0xFF
PickpocketStealString: .byte 0xFD, 0x10, 0xB4, 0xE7, 0x00, 0xFD, 0x16, 0xFE, 0xEB, 0xD5, 0xE7, 0x00, 0xE7, 0xE8, 0xE3, 0xE0, 0xD9, 0xE2, 0xAB, 0xFF
MindBlownString: .byte 0xFD, 0x0F, 0x0, 0xD7, 0xE9, 0xE8, 0x00, 0xDD, 0xE8, 0xE7, 0x00, 0xE3, 0xEB, 0xE2, 0xFE, 0xC2, 0xCA, 0x00, 0xE8, 0xE3, 0x00, 0xE4, 0xE3, 0xEB, 0xD9, 0xE6, 0x00, 0xE9, 0xE4, 0x00, 0xDD, 0xE8, 0xE7, 0x00, 0xD5, 0xE8, 0xE8, 0xD5, 0xD7, 0xDF, 0xAB, 0xFF
LifeOrbString: .byte 0xFD, 0x0F, 0x00, 0xE0, 0xE3, 0xE7, 0xE8, 0x00, 0xE7, 0xE3, 0xE1, 0xD9, 0xFE, 0xE3, 0xDA, 0x00, 0xDD, 0xE8, 0xE7, 0x00, 0xC2, 0xCA, 0xAB, 0xFF
PluckString: .byte 0xFD, 0x0F, 0x00, 0xE7, 0xE8, 0xE3, 0xE0, 0xD9, 0x00, 0xD5, 0xE2, 0xD8, 0x00, 0xD5, 0xE8, 0xD9, 0xFE, 0xDD, 0xE8, 0xE7, 0x00, 0xDA, 0xE3, 0xD9, 0xB4, 0xE7, 0x00, 0xFD, 0x16, 0xAB, 0xFF
