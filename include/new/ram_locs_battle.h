#pragma once

#include "../global.h"

//#define gEnigmaBerries ((struct BattleEnigmaBerry*) 0x2023F54) //Length 0x70
//#define gBattleScripting ((struct BattleScripting*) 0x2023FC4)
//#define gBattleStruct ((struct BattleStruct*) *((u32*) 0x2023FE8))
//#define gBattleResources ((struct BattleResources*) *((u32*) 0x2023FF4))
//
//#define gBattleResults ((struct BattleResults*) 0x3004F90)
//
//#define gSideTimers ((struct SideTimer*) 0x2023DE4)
//#define gDisableStructs ((struct DisableStruct*) 0x2023E0C)
//#define gProtectStructs ((struct ProtectStruct*) 0x2023E8C)
//#define gSpecialStatuses ((struct SpecialStatus*) 0x2023ECC)
//#define gWishFutureKnock ((struct WishFutureKnock*) 0x2023F20)
//#define gBattleMons ((struct BattlePokemon*) 0x2023BE4)
//
///*u8*/  #define gLinkPlayers 			((struct LinkPlayer*) 0x202273C)
///*u8*/  #define unk_2022744				((u8*) 0x2022744)
///*u8*/  #define gBattleBuffersTransferData ((u8*) 0x2022874) //u8 gBattleBuffersTransferData[0x170];
///*u16*/	#define gBattle_BG0_X 			(*((u16*) 0x2022974))
///*u16*/	#define gBattle_BG0_Y 			(*((u16*) 0x2022976))
///*u16*/	#define gBattle_BG1_Y 			(*((u16*) 0x202297A))
///*u16*/	#define gBattle_BG2_Y 			(*((u16*) 0x202297E))
///*u8*/  #define gDisplayedStringBattle 	((u8*) 0x202298C)
///*u8*/  #define gBattleTextBuff1 		((u8*) 0x2022AB8)
///*u8*/  #define gBattleTextBuff2 		((u8*) 0x2022AC8)
///*u8*/  #define gBattleTextBuff3 		((u8*) 0x2022AD8)
//
///*u32*/ #define gBattleTypeFlags 		*((u32*) 0x2022B4C)
///*u8*/  #define gBattleTerrain			*((u8*) 0x2022B50)
//
//typedef u8 Battle_Buffer_T[0x200];
///*u8*/  #define gBattleBufferA ((Battle_Buffer_T*) 0x2022BC4) //u8 gBattleBufferA[MAX_BATTLERS_COUNT][0x200];
///*u8*/  #define gBattleBufferB ((Battle_Buffer_T*) 0x20233C4)
//
///*u8*/  #define gActiveBattler 			(*((u8*) 0x2023BC4))
///*u32*/ #define gBattleExecBuffer 		(*((u32*) 0x2023BC8))
///*u8*/  #define gBattlersCount 			(*((u8*) 0x2023BCC))
///*u16*/ #define gBattlerPartyIndexes 	 ((u16*) 0x2023BCE)
///*u8*/  #define gBanksBySide 			 ((u8*) 0x2023BD6)
///*u8*/  #define gActionsByTurnOrder 	 ((u8*) 0x2023BDA)
///*u8*/  #define gBanksByTurnOrder 	   	 ((u8*) 0x2023BDE)
///*u8*/  #define gCurrentTurnActionNumber (*((u8*) 0x2023BE2))
///*u8*/  #define gCurrentActionFuncId 	(*((u8*) 0x2023BE3))
//
///*u8*/  #define gBattlerSpriteIds ((u8*) 0x2023D44)
///*u8*/  #define gCurrMovePos (*((u8*) 0x2023D48))
///*u8*/  #define gChosenMovePos (*((u8*) 0x2023D49))
///*u16*/ #define gCurrentMove (*((u16*) 0x2023D4A))
///*u16*/ #define gChosenMove (*((u16*) 0x2023D4C))
///*u16*/ #define gRandomMove (*((u16*) 0x2023D4E))
///*s32*/ #define gBattleMoveDamage *((s32*) 0x2023D50)
///*u32*/ #define gHpDealt *((u32*) 0x2023D54)
//
///*u32*/ #define gTakenDmg ((u32*) 0x2023D58)
//
///*u16*/ #define gLastUsedItem (*((u16*) 0x2023D68))
///*u8*/  #define gLastUsedAbility (*((u8*) 0x2023D6A))
///*u8*/	#define gBankAttacker (*((u8*) 0x2023D6B))
///*u8*/  #define gBattlerAttacker (*((u8*) 0x2023D6B))
///*u8*/	#define gBankTarget (*((u8*) 0x2023D6C))
///*u8*/  #define gBattlerTarget (*((u8*) 0x2023D6C))
///*u8*/  #define gBank1 (*((u8*) 0x2023D6D))
///*u8*/  #define gBankFainted (*((u8*) 0x2023D6D))
///*u8*/  #define gEffectBank (*((u8*) 0x2023D6E))
///*u8*/  #define gStringBank (*((u8*) 0x2023D6F))
///*u8*/  #define gAbsentBattlerFlags (*((u8*) 0x2023D70))
///*u8*/  #define gCritMultiplier (*((u8*) 0x2023D71))
///*u8*/  #define gMultiHitCounter (*((u8*) 0x2023D72))
////Padding 0x2023D73
///*u32*/ #define gBattlescriptCurrInstr (*((const u8**) 0x2023D74))
////Padding 0x2023D78?
///*u8*/  #define gActionForBanks ((u8*) 0x2023D7C)
//
///*u8*/  #define gSelectionBattleScripts ((const u8**) 0x2023D80)
///*u8*/  #define gLastPrintedMoves ((u16*) 0x2023D90)
//
///*u16*/ #define gLastUsedMoves ((u16*) 0x2023D98)
///*u16*/ #define gLastLandedMoves ((u16*) 0x2023DA0)
///*u16*/ #define gLastHitByType ((u16*) 0x2023DA8)
///*u16*/ #define gLastResultingMoves ((u16*) 0x2023DB0)
///*u16*/ #define gLockedMoves ((u16*) 0x2023DB8)
///*u8*/  #define gLastHitBy ((u8*) 0x2023DC0)
///*u16*/ #define gChosenMovesByBanks ((u16*) 0x2023DC4)
///*u32*/ #define gMoveResultFlags *((u8*) 0x2023DCC)
///*u32*/ #define gHitMarker *((u32*) 0x2023DD0)
//
///*u8*/  #define sUnusedBattlersArray ((u8*) 0x2023DD4)
///*u8*/  #define gTakenDmgBanks ((u8*) 0x2023DD8)
///*u16*/ #define gUnusedFirstBattleVar2 *((u16*) 0x2023DDC)
//
///*u16*/ #define gSideAffecting ((u16*) 0x2023DDE)
////Padding 0x2023DE2
//
//
///*u32*/ #define gStatuses3 ((u32*) 0x2023DFC)
//
////Disable Struct
//
///*u16*/ #define gPauseCounterBattle *((u16*) 0x2023E7C)
///*u16*/ #define gPaydayMoney *((u16*) 0x2023E7E)
///*u16*/ #define gRandomTurnNumber *((u16*) 0x2023E80)
///*u8*/  #define gBattleCommunication ((u8*) 0x2023E82)
//
///*u16*/ #define gBattleOutcome *((u16*) 0x2023E8A)

//Protect Struct
//Special Status Struct

///*u16*/ #define gBattleWeather *((u16*) 0x2023F1C)
//
////WishFutureKnock Struct
//
///*u8*/  #define gUnknown_02023F40 ((u8*) 0x2023F40) //Length 0x8
///*u8*/  #define gWeatherCounter *((u8*) 0x2023F48)
//
//
////Padding 0x3
///*u16*/ #define gIntroSlideFlags *((u16*) 0x2023F4C)
//
///*u16*/ #define gSentPokesToOpponent ((u8*) 0x2023F4E) //gSentPokesToOpponent[2]
///*u16*/ #define gDynamicBasePower *((u16*) 0x2023F50)
///*u16*/ #define gExpShareExp *((u16*) 0x2023F52)
//
///*u8*/  #define gBideDamage *((u32*) 0x2023FC8) //Length 0x4 //Part of gBattleScripting
///*u8*/  #define gStatAnimByte	*((u8*) 0x2023FD4)
///*u8*/  #define gStatChangeByte	*((u8*) 0x2023FDE)
///*u8*/  #define gLinkBattleSendBuffer  (u8*) (*((u32*) 0x2023FEC))
///*u8*/  #define gLinkBattleRecvBuffer  (u8*) (*((u32*) 0x2023FF0))
///*u8*/  #define gActionSelectionCursor ((u8*) 0x2023FF8)
///*u8*/  #define gMoveSelectionCursor ((u8*) 0x2023FFC)
///*u8*/  #define gBattlerStatusSummaryTaskId ((u8*) 0x2024000)
//
///*u8*/  #define gBattlerInMenuId *((u8*) 0x2024004)
//
///*u8*/  #define gBattleSpritesDataPtr ((struct BattleSpriteData*) *((u32*) 0x2024018))
///*u8*/  #define gMonSpritesGfxPtr ((struct BattleSpriteData*) *((u32*) 0x2024018))
///*u16*/ #define gBattleMovePower *((u16*) 0x2024020)
///*u16*/ #define gMoveToLearn *((u16*) 0x2024022)
///*u8*/  #define gBattleMonForms ((u8*) 0x2024024)
//
///*u32*/	#define sBattleAnimScriptPtr (*((const u8**) 0x2037ED4))
//		extern void (*gAnimScriptCallback)(void); //0x2037EDC
///*s8*/	#define gAnimFramesToWait (*((s8*) 0x2037EE0))
///*u8*/	#define gAnimScriptActive (*((u8*) 0x2037EE1))
///*s16*/ #define gBattleAnimArgs ((s16*) 0x2037F02)
///*u16*/	#define sAnimMoveIndex (*((u16*) 0x2037F18))
///*u8*/  #define gBattleAnimAttacker (*((u8*) 0x2037F1A))
///*u8*/  #define gBattleAnimTarget (*((u8*) 0x2037F1B))

/*u16*/ #define sTrainerBattleMode (*((u16*) 0x20386AC))
/*u16*/ #define gTrainerBattleOpponent_A (*((u16*) 0x20386AE))
		#define sTrainerEventObjectLocalId (*((u16*) 0x20386B0))
		#define sTrainerIntroSpeech_A (*((u8**) 0x20386B4))
		#define sTrainerDefeatSpeech_A (*((u8**) 0x20386B8))
		#define sTrainerVictorySpeech (*((u8**) 0x20386BC))
		#define sTrainerCannotBattleSpeech (*((u8**) 0x20386C0))
		#define sTrainerBattleEndScript (*((u8**) 0x20386C4))
		#define sTrainerBattleScriptRetAddr (*((u32*) 0x20386C8))
		#define sTrainerBattleOakTutorialHelper (*((u16*) 0x20386CC))


#define gBattleStringLoader (*((const u8**) 0x203E020))
#define gSeedHelper ((u8*) 0x203E024)
#define gTerrainType *((u8*) 0x203E028)
#define gFormCounter *((u8*) 0x203E029)
#define gPoisonedBy *((u8*) 0x203E02A)
	// 0x1 = Poison Touch	/ 0x1 = Beak Blast Burn
	// 0x2 = Toxic Spikes
	// 0x3 = Toxic Orb		/ 0x3 = Flame Orb
	// 0x4 = Baneful Bunker
#define gMagicianHelper *((u8*) 0x203E02B)
#define gShakerData (((u8*) 0x203E02C))
#define gForceSwitchHelper (*((u8*) 0x203E02E))
#define gAbilityPopUpHelper (*((u8*) 0x203E02F))
#define gAbilityPopUpSpecies (*((u16*) 0x203E030))
#define gDontRemoveTransformSpecies (*((bool8*) 0x203E032))
#define gBankSwitching (*((u8*) 0x203E033))
#define gBackupHWord (*((u16*) 0x203E034))

//Don't go past 0x203E048

#define FIRST_NEW_BATTLE_RAM_LOC ((u8*) 0x203E020)
#define LAST_NEW_BATTLE_RAM_LOC ((u8*) 0x203E034)
