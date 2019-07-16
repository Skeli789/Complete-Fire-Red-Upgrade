.text
.thumb
.align 2

.include "../defines"

.global BattleScript_BerryConfuseHealRet
.global BattleScript_BerryConfuseHealEnd2
.global BattleScript_ItemHealHP_Ret
.global BattleScript_ItemHealHP_End2
.global BattleScript_BerryHealHP_RemoveBerryRet
.global BattleScript_BerryHealHP_RemoveBerryEnd2
.global BattleScript_BerryPPHealRet
.global BattleScript_BerryPPHealEnd2
.global BattleScript_BerryFocusEnergyRet
.global BattleScript_BerryFocusEnergyEnd2
.global BattleScript_BerryStatRaiseRet
.global BattleScript_BerryStatRaiseEnd2
.global BattleScript_BerryCureParRet
.global BattleScript_BerryCurePrlzEnd2
.global BattleScript_BerryCurePsnRet
.global BattleScript_BerryCurePsnEnd2
.global BattleScript_BerryCureBrnRet
.global BattleScript_BerryCureBrnEnd2
.global BattleScript_BerryCureFrzRet
.global BattleScript_BerryCureFrzEnd2
.global BattleScript_BerryCureSlpRet
.global BattleScript_BerryCureSlpEnd2
.global BattleScript_BerryCureConfusionRet
.global BattleScript_BerryCureConfusionEnd2
.global BattleScript_BerryCureChosenStatusRet
.global BattleScript_BerryCureChosenStatusEnd2
.global BattleScript_HerbCureChosenStatusRet
.global BattleScript_HerbCureChosenStatusEnd2
.global BattleScript_HerbCureChosenStatusFling
.global BattleScript_RaiseStatsItem
.global BattleScript_RaiseStatsSeedEnd2
.global BattleScript_RaiseStatsSeedRet
.global BattleScript_WhiteHerbFling

.global BattleScript_AirBallooonPop
.global BattleScript_WeaknessPolicy
.global BattleScript_RockyHelmetDamage
.global BattleScript_JabocaRowapBerry
.global BattleScript_BlackSludgeHurt
.global BattleScript_MicleBerryRet
.global BattleScript_MicleBerryEnd2
.global BattleScript_StickyBarbTransfer
.global BattleScript_EjectButton
.global BattleScript_RedCard
.global BattleScript_TypeResistBerry
.global BattleScript_HangedOnFocusSash
.global BattleScript_Gems
.global BattleScript_WeaknessBerryActivate

.global DoCheekPouch

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_BerryConfuseHealRet:
	playanimation 0xA ANIM_BERRY_EAT 0x0
	printstring 0x12A
	waitmessage DELAY_1SECOND
	orword HIT_MARKER 0x100
	graphicalhpupdate 0xA
	datahpupdate 0xA
	printstring 0x144
	waitmessage DELAY_1SECOND
	setbyte EFFECT_BYTE 0x47
	seteffecttarget
	call DoCheekPouch
	return

BattleScript_BerryConfuseHealEnd2:
	call BattleScript_BerryConfuseHealRet
	end2

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_BerryHealHP_RemoveBerryRet:
	playanimation 0xA ANIM_BERRY_EAT 0x0
	playanimation 0xA ANIM_HEALING_SPARKLES 0x0
	orword HIT_MARKER 0x100
	graphicalhpupdate 0xA
	datahpupdate 0xA
	printstring 0x12A
	waitmessage DELAY_1SECOND
	call DoCheekPouch
	return

BattleScript_BerryHealHP_RemoveBerryEnd2:
	call BattleScript_BerryHealHP_RemoveBerryRet
	end2

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_BerryPPHealRet:
	waitstateatk
	playanimation 0xA ANIM_BERRY_EAT 0x0
	printstring 0x12B
	waitmessage DELAY_1SECOND
	call DoCheekPouch
	return
	
BattleScript_BerryPPHealEnd2:
	call BattleScript_BerryPPHealRet
	end2

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_BerryFocusEnergyRet:
	playanimation 0xA ANIM_BERRY_EAT 0x0
	printstring 0x146
	waitmessage DELAY_1SECOND
	call DoCheekPouch
	return
	
BattleScript_BerryFocusEnergyEnd2:
	call BattleScript_BerryFocusEnergyRet
	end2

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_BerryStatRaiseRet:
	playanimation 0xA ANIM_BERRY_EAT 0x0
	setbyte MULTISTRING_CHOOSER 0x4
	call 0x81D6BD1
	call DoCheekPouch
	return

BattleScript_BerryStatRaiseEnd2:
	call BattleScript_BerryStatRaiseRet
	end2

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_BerryCureParRet:
	playanimation 0xA ANIM_BERRY_EAT 0x0
	refreshhpbar 0xA
	printstring 0x123
	waitmessage DELAY_1SECOND
	call DoCheekPouch
	return

BattleScript_BerryCurePrlzEnd2:
	call BattleScript_BerryCureParRet
	end2
	
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@


BattleScript_BerryCurePsnRet:
	playanimation 0xA ANIM_BERRY_EAT 0x0
	refreshhpbar 0xA
	printstring 0x124
	waitmessage DELAY_1SECOND
	call DoCheekPouch
	return

BattleScript_BerryCurePsnEnd2:
	call BattleScript_BerryCurePsnRet
	end2

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_BerryCureBrnRet:
	playanimation 0xA ANIM_BERRY_EAT 0x0
	refreshhpbar 0xA
	printstring 0x125
	waitmessage DELAY_1SECOND
	call DoCheekPouch
	return

BattleScript_BerryCureBrnEnd2:
	call BattleScript_BerryCureBrnRet
	end2

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_BerryCureFrzRet:
	playanimation 0xA ANIM_BERRY_EAT 0x0
	refreshhpbar 0xA
	printstring 0x126
	waitmessage DELAY_1SECOND
	call DoCheekPouch
	return

BattleScript_BerryCureFrzEnd2:
	call BattleScript_BerryCureFrzRet
	end2

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_BerryCureSlpRet:
	playanimation 0xA ANIM_BERRY_EAT 0x0
	refreshhpbar 0xA
	printstring 0x127
	waitmessage DELAY_1SECOND
	call DoCheekPouch
	return

BattleScript_BerryCureSlpEnd2:
	call BattleScript_BerryCureSlpRet
	end2

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_BerryCureConfusionRet:
	playanimation 0xA ANIM_BERRY_EAT 0x0
	printstring 0x128
	waitmessage DELAY_1SECOND
	call DoCheekPouch
	return

BattleScript_BerryCureConfusionEnd2:
	call BattleScript_BerryCureConfusionRet
	end2

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_BerryCureChosenStatusRet:
	playanimation 0xA ANIM_BERRY_EAT 0x0
	refreshhpbar 0xA
	printfromtable 0x83FE634
	waitmessage DELAY_1SECOND
	call DoCheekPouch
	return

BattleScript_BerryCureChosenStatusEnd2:
	call BattleScript_BerryCureChosenStatusRet
	end2

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

DoCheekPouch:
	callasm CheeckPouchFunc
	jumpifbyte EQUALS FORM_COUNTER 0x0 CheeckPouchRet
	setbyte FORM_COUNTER 0x0
	call BattleScript_AbilityPopUp
	playanimation BANK_SCRIPTING ANIM_HEALING_SPARKLES 0x0
	graphicalhpupdate BANK_SCRIPTING
	datahpupdate BANK_SCRIPTING
	setword BATTLE_STRING_LOADER CheekPouchString
	printstring 0x184
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
CheeckPouchRet:
	removeitem BANK_SCRIPTING
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_HerbCureChosenStatusRet:
	playanimation BANK_SCRIPTING ANIM_ITEM_USE 0x0
	refreshhpbar BANK_SCRIPTING
	printfromtable 0x83FE634
	waitmessage DELAY_1SECOND
	removeitem BANK_SCRIPTING
	return

BattleScript_HerbCureChosenStatusEnd2:
	call BattleScript_HerbCureChosenStatusRet
	end2
	
BattleScript_HerbCureChosenStatusFling:
	playanimation BANK_SCRIPTING ANIM_ITEM_USE 0x0
	refreshhpbar BANK_SCRIPTING
	printfromtable 0x83FE634
	waitmessage DELAY_1SECOND
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_RaiseStatsItem:
	playanimation BANK_SCRIPTING ANIM_ITEM_USE 0x0
	setbyte MULTISTRING_CHOOSER 0x4
	call 0x81D6BD1
	removeitem BANK_SCRIPTING
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_RaiseStatsSeedEnd2:
	call BattleScript_RaiseStatsSeedRet
	end2

BattleScript_RaiseStatsSeedRet:
	statbuffchange STAT_TARGET | STAT_BS_PTR | STAT_CERTAIN RaiseStatsSeedReturn
	playanimation BANK_TARGET ANIM_ITEM_USE 0x0
	playanimation BANK_TARGET ANIM_STAT_BUFF ANIM_ARG_1
	setbyte MULTISTRING_CHOOSER 0x4
	printfromtable 0x83FE57C
	waitmessage DELAY_1SECOND
	removeitem BANK_TARGET
RaiseStatsSeedReturn:
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_WhiteHerbFling:
	playanimation BANK_SCRIPTING ANIM_ITEM_USE 0x0
	setword BATTLE_STRING_LOADER gText_WhiteHerbFling
	printstring 0x184
	waitmessage DELAY_1SECOND
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_AirBallooonPop:
	setword BATTLE_STRING_LOADER AirBalloonPopString
	printstring 0x184
	waitmessage DELAY_1SECOND
	setcounter BANK_TARGET INCINERATE_COUNTERS 0x1
	callasm SetUnburdenBoostTarget
	removeitem BANK_TARGET
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_WeaknessPolicy:
	playanimation BANK_TARGET ANIM_ITEM_USE 0x0
	setword BATTLE_STRING_LOADER WeaknessPolicyString
	printstring 0x184
	
WP_Atk:
	setbyte 0x2023FDF 0x0
	playstatchangeanimation BANK_TARGET STAT_ANIM_ATK | STAT_ANIM_SPATK  STAT_ANIM_UP | STAT_ANIM_IGNORE_ABILITIES | STAT_ANIM_BY_TWO
	setstatchanger STAT_ATK | INCREASE_2
	statbuffchange STAT_TARGET | STAT_BS_PTR WP_SpAtk
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 WP_SpAtk
	printfromtable 0x83FE57C
	waitmessage DELAY_1SECOND

WP_SpAtk:
	setstatchanger STAT_SP_ATK | INCREASE_2
	statbuffchange STAT_TARGET | STAT_BS_PTR WP_Ret
	jumpifbyte EQUALS MULTISTRING_CHOOSER 0x2 WP_Ret
	printfromtable 0x83FE57C
	waitmessage DELAY_1SECOND

WP_Ret:
	removeitem BANK_TARGET
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_RockyHelmetDamage:
	orword HIT_MARKER 0x100
	graphicalhpupdate BANK_ATTACKER
	datahpupdate BANK_ATTACKER
	setword BATTLE_STRING_LOADER RockyHelmetString
	printstring 0x184
	waitmessage DELAY_1SECOND
	faintpokemon BANK_ATTACKER 0x0 0x0
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_JabocaRowapBerry:
	playanimation BANK_TARGET ANIM_BERRY_EAT 0x0
	orword HIT_MARKER 0x100
	graphicalhpupdate BANK_ATTACKER
	datahpupdate BANK_ATTACKER
	setword BATTLE_STRING_LOADER RockyHelmetString
	printstring 0x184
	waitmessage DELAY_1SECOND
	faintpokemon BANK_ATTACKER 0x0 0x0
	call DoCheekPouch
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_BlackSludgeHurt:
	playanimation 0xA ANIM_WAITING_WAGGLE 0x0
	orword HIT_MARKER 0x100
	graphicalhpupdate 0xA
	datahpupdate 0xA
	setword BATTLE_STRING_LOADER BlackSludgeHurtString
	printstring 0x184
	waitmessage DELAY_1SECOND
	faintpokemon 0xA 0x0 0x0
	end2
	
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MicleBerryRet:
	playanimation 0xA ANIM_BERRY_EAT 0x0
	setword BATTLE_STRING_LOADER MicleBerryString
	printstring 0x184
	waitmessage DELAY_1SECOND
	call DoCheekPouch
	return

BattleScript_MicleBerryEnd2:
	call BattleScript_MicleBerryRet
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_StickyBarbTransfer:
	playanimation 0xA ANIM_STEAL_ITEM 0x0
	setword BATTLE_STRING_LOADER StickyBarbString
	printstring 0x184
	waitmessage DELAY_1SECOND
	removeitem 0xA
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_EjectButton:
	jumpifcannotswitch BANK_SCRIPTING | ATK4F_DONT_CHECK_STATUSES EjectButtonEnd
	playanimation BANK_SCRIPTING ANIM_ITEM_USE 0x0
	setword BATTLE_STRING_LOADER EjectButtonString
	printstring 0x184
	waitmessage DELAY_1SECOND
	removeitem BANK_SCRIPTING
	playanimation BANK_SCRIPTING ANIM_CALL_BACK_POKEMON 0x0
	callasm MakeScriptingBankInvisible
	openpartyscreen BANK_SCRIPTING EjectButtonEnd
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

EjectButtonEnd:
	return
	
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_RedCard:
	jumpifcannotswitch BANK_ATTACKER | ATK4F_DONT_CHECK_STATUSES RedCardRet
	playanimation BANK_SCRIPTING ANIM_ITEM_USE 0x0
	setword BATTLE_STRING_LOADER RedCardString
	printstring 0x184
	waitmessage DELAY_1SECOND
	jumpifspecialstatusflag BANK_ATTACKER STATUS3_ROOTED 0x0 RedCard_Ingrain
	jumpifability BANK_ATTACKER ABILITY_SUCTIONCUPS RedCard_SuctionCups
	copybyte SEED_HELPER TARGET_BANK
	copybyte TARGET_BANK USER_BANK
	playanimation BANK_TARGET DRAGON_TAIL_BLOW_AWAY_ANIM 0x0
	callasm ClearAttackerDidDamageOnce
	callasm TryRemovePrimalWeatherOnPivot
	forcerandomswitch RedCardSwapBanksBack

RedCardSwapBanksBack:
	copybyte TARGET_BANK SEED_HELPER
	
RedCardEnd:
	removeitem BANK_SCRIPTING
	
RedCardRet:
	return

RedCard_Ingrain:
	setword BATTLE_STRING_LOADER RedCardIngrainString
	printstring 0x184
	waitmessage DELAY_1SECOND
	goto RedCardEnd

RedCard_SuctionCups:
	setword BATTLE_STRING_LOADER RedCardSuctionCupsString
	printstring 0x184
	waitmessage DELAY_1SECOND
	goto RedCardEnd

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_TypeResistBerry:
	playanimation 0xA ANIM_BERRY_EAT 0x0
	setword BATTLE_STRING_LOADER TypeResistBerryString
	printstring 0x184
	waitmessage DELAY_1SECOND
	call DoCheekPouch
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_HangedOnFocusSash:
	playanimation BANK_TARGET ANIM_ITEM_USE 0x0
	printstring 0x12F
	waitmessage DELAY_1SECOND
	removeitem BANK_TARGET
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_Gems:
	callasm BufferAttackerItem
	pause DELAY_HALFSECOND
	playanimation BANK_ATTACKER ANIM_ITEM_USE 0x0
	setword BATTLE_STRING_LOADER GemString
	printstring 0x184
	waitmessage DELAY_1SECOND
	removeitem BANK_ATTACKER
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_WeaknessBerryActivate:
	playanimation BANK_SCRIPTING ANIM_BERRY_EAT 0x0
	setword BATTLE_STRING_LOADER WeaknessBerryString
	printstring 0x184
	waitmessage DELAY_1SECOND
	call DoCheekPouch
	return

.align 2
CheekPouchString: .byte 0xFD, 0x13, 0xB4, 0xE7, 0x00, 0xC2, 0xCA, 0xFE, 0xEB, 0xD5, 0xE7, 0x00, 0xE6, 0xD9, 0xE7, 0xE8, 0xE3, 0xE6, 0xD9, 0xD8, 0xAB, 0xFF
AirBalloonPopString: .byte 0xFD, 0x10, 0xB4, 0xE7, 0x00, 0xFD, 0x16, 0xFE, 0xE4, 0xE3, 0xE4, 0xE4, 0xD9, 0xD8, 0xAB, 0xFF
WeaknessPolicyString: .byte 0xFD, 0x10, 0xB4, 0xE7, 0xFE, 0xFD, 0x16, 0x00, 0xD5, 0xD7, 0xE8, 0xDD, 0xEA, 0xD5, 0xE8, 0xD9, 0xD8, 0xAB, 0xFF
RockyHelmetString: .byte 0xFD, 0x10, 0xB4, 0xE7, 0x00, 0xFD, 0x16, 0xFE, 0xDC, 0xE9, 0xE6, 0xE8, 0x00, 0xFD, 0x0F, 0xAB, 0xFF
BlackSludgeHurtString: .byte 0xFD, 0x13, 0x00, 0xEB, 0xD5, 0xE7, 0x00, 0xDC, 0xE9, 0xE6, 0xE8, 0xFE, 0xD6, 0xED, 0x00, 0xDD, 0xE8, 0xE7, 0x00, 0xFD, 0x16, 0xAB, 0xFF
MicleBerryString: .byte 0xFD, 0x13, 0x00, 0xD6, 0xE3, 0xE3, 0xE7, 0xE8, 0xD9, 0xD8, 0xFE, 0xE8, 0xDC, 0xD9, 0x00, 0xD5, 0xD7, 0xD7, 0xE9, 0xE6, 0xD5, 0xD7, 0xED, 0x00, 0xE3, 0xDA, 0x00, 0xDD, 0xE8, 0xE7, 0x00, 0xE2, 0xD9, 0xEC, 0xE8, 0x00, 0xE1, 0xE3, 0xEA, 0xD9, 0xFA, 0xE9, 0xE7, 0xDD, 0xE2, 0xDB, 0x00, 0xDD, 0xE8, 0xE7, 0x00, 0xFD, 0x16, 0xAB, 0xFF
StickyBarbString: .byte 0xCE, 0xDC, 0xD9, 0x00, 0xFD, 0x16, 0x00, 0xD5, 0xE8, 0xE8, 0xD5, 0xD7, 0xDC, 0xD9, 0xD8, 0x00, 0xDD, 0xE8, 0xE7, 0xD9, 0xE0, 0xDA, 0x00, 0xE8, 0xE3, 0xFE, 0xFD, 0x0F, 0xAB, 0xFF
EjectButtonString: .byte 0xFD, 0x13, 0x00, 0xDD, 0xE7, 0x00, 0xE7, 0xEB, 0xDD, 0xE8, 0xD7, 0xDC, 0xD9, 0xD8, 0xFE, 0xE3, 0xE9, 0xE8, 0x00, 0xEB, 0xDD, 0xE8, 0xDC, 0x00, 0xE8, 0xDC, 0xD9, 0x00, 0xFD, 0x16, 0xAB, 0xFF
RedCardString: .byte 0xFD, 0x13, 0x00, 0xDC, 0xD9, 0xE0, 0xD8, 0x00, 0xE9, 0xE4, 0x00, 0xDD, 0xE8, 0xE7, 0xFE, 0xFD, 0x16, 0x00, 0xD5, 0xDB, 0xD5, 0xDD, 0xE2, 0xE7, 0xE8, 0x00, 0xFD, 0x0F, 0xAB, 0xFF
RedCardIngrainString: .byte 0xBC, 0xE9, 0xE8, 0x00, 0xFD, 0x0F, 0x00, 0xD5, 0xE2, 0xD7, 0xDC, 0xE3, 0xE6, 0xD9, 0xD8, 0xFE, 0xDD, 0xE8, 0xE7, 0xD9, 0xE0, 0xDA, 0x00, 0xEB, 0xDD, 0xE8, 0xDC, 0x00, 0xDD, 0xE8, 0xE7, 0x00, 0xE6, 0xE3, 0xE3, 0xE8, 0xE7, 0xAB, 0xFF
RedCardSuctionCupsString: .byte 0xBC, 0xE9, 0xE8, 0x00, 0xFD, 0x0F, 0x00, 0xD5, 0xE2, 0xD7, 0xDC, 0xE3, 0xE6, 0xE7, 0xFE, 0xDD, 0xE8, 0xE7, 0xD9, 0xE0, 0xDA, 0x00, 0xEB, 0xDD, 0xE8, 0xDC, 0x00, 0xFD, 0x18, 0xAB, 0xFF
TypeResistBerryString: .byte 0xFD, 0x13, 0xB4, 0xE7, 0x00, 0xFD, 0x16, 0xFE, 0xE6, 0xD9, 0xD8, 0xE9, 0xD7, 0xD9, 0xD8, 0x00, 0xFD, 0x14, 0xB4, 0xE7, 0x00, 0xE4, 0xE3, 0xEB, 0xD9, 0xE6, 0xAB, 0xFF
GemString: .byte 0xCE, 0xDC, 0xD9, 0x00, 0xFD, 0x16, 0x00, 0xE7, 0xE8, 0xE6, 0xD9, 0xE2, 0xDB, 0xE8, 0xDC, 0xD9, 0xE2, 0xD9, 0xD8, 0xFE, 0xE8, 0xDC, 0xD9, 0x00, 0xE4, 0xE3, 0xEB, 0xD9, 0xE6, 0x00, 0xE3, 0xDA, 0x00, 0xFD, 0x01, 0xFF
WeaknessBerryString: .byte 0xFD, 0x13, 0xB4, 0xE7, 0x00, 0xFD, 0x16, 0xFE, 0xE6, 0xD9, 0xD8, 0xE9, 0xD7, 0xD9, 0xD8, 0x00, 0xFD, 0x14, 0xB4, 0xE7, 0x00, 0xE4, 0xE3, 0xEB, 0xD9, 0xE6, 0xAB, 0xFF
