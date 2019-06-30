.text
.thumb
.align 2

.include "..\\defines"

.global BattleScript_PoisonTouch
.global BattleScript_KingsShield
.global BattleScript_SpikyShield
.global BattleScript_BanefulBunker
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
.global BattleScript_PluckEat

.global ToxicOrbString
.global FlameOrbString

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_PoisonTouch:
	setbyte POISONED_BY 0x1
	setbyte EFFECT_BYTE 0x2
	seteffecttarget
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_KingsShield:
	setstatchanger STAT_ATK | DECREASE_2
	statbuffchange STAT_ATTACKER | STAT_NOT_PROTECT_AFFECTED | STAT_BS_PTR KingsShieldReturn
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 KingsShieldReturn
	setgraphicalstatchangevalues
	playanimation BANK_ATTACKER ANIM_STAT_BUFF ANIM_ARG_1
	printfromtable 0x83FE588
	waitmessage DELAY_1SECOND
	jumpifability BANK_ATTACKER ABILITY_DEFIANT KingsShieldCallDefiant
	jumpifability BANK_ATTACKER ABILITY_COMPETITIVE KingsShieldCallCompetitive

KingsShieldReturn:
	return

KingsShieldCallDefiant:
	setstatchanger STAT_ATK | INCREASE_2
	copyarray BATTLE_SCRIPTING_BANK USER_BANK 0x1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR KingsShieldReturn
	call BattleScript_DefiantCompetitiveCall
	return

KingsShieldCallCompetitive:
	setstatchanger STAT_SPATK | INCREASE_2
	copyarray BATTLE_SCRIPTING_BANK USER_BANK 0x1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR KingsShieldReturn
	call BattleScript_DefiantCompetitiveCall
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
	printfromtable 0x83FE57C
	waitmessage DELAY_1SECOND
	goto RagePrintString
	
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_BeakBlastBurn:
	orword HIT_MARKER 0x2100
	swapattackerwithtarget
	setbyte POISONED_BY 0x1
	setbyte EFFECT_BYTE 0x3
	seteffecttarget
	swapattackerwithtarget
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_Magician:
	setbyte MAGICIAN_HELPER 0x1
	setbyte EFFECT_BYTE 0x1F
	seteffecttarget
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_Moxie:
	statbuffchange STAT_ATTACKER | STAT_BS_PTR MoxieReturnPostBuff
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 MoxieReturnPostBuff
	call BattleScript_AbilityPopUp
	playanimation BANK_ATTACKER ANIM_STAT_BUFF ANIM_ARG_1
	printfromtable 0x83FE57C
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
MoxieReturnPostBuff:
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
	setbyte MAGICIAN_HELPER 0x2
	setmoveeffect MOVE_EFFECT_STEAL_ITEM
	swapattackerwithtarget
	setmoveeffect2
	swapattackerwithtarget
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

PoisonedByBS:
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

BadPoisonedByBS:
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

BurnedByBS:
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
ThiefStealBS:
	jumpifbyte EQUALS MAGICIAN_HELPER 0x1 MagicianStealBS
	jumpifbyte EQUALS MAGICIAN_HELPER 0x2 PickpocketStealBS
	playanimation BANK_TARGET 0x10 0x0
	printstring 0x8E
	waitmessage 0x40
	return

MagicianStealBS:
	setbyte MAGICIAN_HELPER 0x0
	playanimation BANK_TARGET 0x10 0x0
	setword BATTLE_STRING_LOADER MagicianStealString
	printstring 0x184
	waitmessage DELAY_1SECOND
	return

PickpocketStealBS:
	setbyte MAGICIAN_HELPER 0x0
	copybyte BATTLE_SCRIPTING_BANK USER_BANK
	call BattleScript_AbilityPopUp
	playanimation BANK_TARGET 0x10 0x0
	setword BATTLE_STRING_LOADER PickpocketStealString
	printstring 0x184
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
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

BattleScript_PluckEat:
	setword BATTLE_STRING_LOADER PluckString
	printstring 0x184
	waitmessage DELAY_1SECOND
	callasm PluckBerryEat
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
