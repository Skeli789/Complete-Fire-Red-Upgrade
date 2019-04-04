#pragma once

#include "../include/global.h"

struct Clock {
	u16 year;
	u8 dayOfWeek;
	u8 month;
	u8 day;
	u8 _;
	u8 hour;
	u8 minute;
	u8 second;
};

struct GbaTimer {
	u16 init;	// 4000108
	u8 timerFlags;	// 400010A
	u8 empty;		// 400010B
	u16 timerVal;	// 400010C
	u8 timerOn;	// 400010E
	
};

// keypad struct
struct KeypadSetter {
	u16 keyMapToForce;	// key(s) to force
	u16 keysToIgnore;		// key(s) to ignore
	u8 keyForcingCounter;	// number of times to force said key(s)
	u8 keyFlags;			// flags for specific key functions
	u16 keyToRunScript;	// key to enable script to run
	u32 scriptToRun;	// script to run from specified key
};

// new pedometers
struct Pedometers {
	u32 alwaysActive;
	u32 large;
	u16 medium;
	u8 smallOne;
	u8 smallTwo;
};

#define gEnigmaBerries ((struct BattleEnigmaBerry*) 0x2023F54) //Length 0x70
#define gBattleScripting ((struct BattleScripting*) 0x2023FC4)
#define gBattleStruct ((struct BattleStruct*) *((u32*) 0x2023FE8))
#define gBattleResources ((struct BattleResources*) *((u32*) 0x2023FF4))

#define gBattleResults ((struct BattleResults*) 0x3004F90)

#define gSaveBlock1 ((struct SaveBlock1*) *((u32*) 0x3005008))
#define gSaveBlock2 ((struct SaveBlock2*) *((u32*) 0x300500C))
#define gSaveBlock3 ((struct SaveBlock3*) *((u32*) 0x3005010))

#define Clock ((struct Clock*) 0x300553C)

#define gSideTimers ((struct SideTimer*) 0x2023DE4)
#define gDisableStructs ((struct DisableStruct*) 0x2023E0C)
#define gProtectStructs ((struct ProtectStruct*) 0x2023E8C)
#define gSpecialStatuses ((struct SpecialStatus*) 0x2023ECC)
#define gWishFutureKnock ((struct WishFutureKnock*) 0x2023F20)
#define gBattleMons ((struct BattlePokemon*) 0x2023BE4)
#define gEnemyParty ((struct Pokemon*) 0x202402C)
#define gPlayerParty ((struct Pokemon*) 0x2024284)

//gBitTable;

/*u8*/  #define gDecompressionBuffer 	 ((u8*) 0x201C000)
/*u32*/ #define gTrainerId 				*((u32*) 0x2020000)

		#define gWindows ((struct Window*) 0x20204B4)
/*u8*/  #define gSprites ((struct Sprite*) 0x202063C)

/*u8*/  #define gStringVar1 ((u8*) 0x2021CD0)
/*u8*/  #define gStringVar2 ((u8*) 0x2021CF0)
/*u8*/  #define gStringVar3 ((u8*) 0x2021D04)
/*u8*/  #define gStringVar4 ((u8*) 0x2021D18)

/*u8*/  #define gLinkPlayers 			((struct LinkPlayer*) 0x202273C)
/*u8*/  #define unk_2022744				((u8*) 0x2022744)
/*u8*/  #define gBattleBuffersTransferData ((u8*) 0x2022874) //u8 gBattleBuffersTransferData[0x170];
/*u16*/	#define gBattle_BG0_X 			(*((u16*) 0x2022974))
/*u16*/	#define gBattle_BG0_Y 			(*((u16*) 0x2022976))
/*u16*/	#define gBattle_BG1_Y 			(*((u16*) 0x202297A))
/*u16*/	#define gBattle_BG2_Y 			(*((u16*) 0x202297E))
/*u8*/  #define gDisplayedStringBattle 	((u8*) 0x202298C)
/*u8*/  #define gBattleTextBuff1 		((u8*) 0x2022AB8)
/*u8*/  #define gBattleTextBuff2 		((u8*) 0x2022AC8)
/*u8*/  #define gBattleTextBuff3 		((u8*) 0x2022AD8)
		
/*u32*/ #define gBattleTypeFlags 		*((u32*) 0x2022B4C)
/*u8*/  #define gBattleTerrain			*((u8*) 0x2022B50)

typedef u8 Buffer_t[0x200];
/*u8*/  #define gBattleBufferA ((Buffer_t*) 0x2022BC4) //u8 gBattleBufferA[MAX_BATTLERS_COUNT][0x200];
/*u8*/  #define gBattleBufferB ((Buffer_t*) 0x20233C4)

		#define gPlayerAvatar ((struct PlayerAvatar*) 0x02037078)

/*u8*/  #define gActiveBattler 			(*((u8*) 0x2023BC4))
/*u32*/ #define gBattleExecBuffer 		(*((u32*) 0x2023BC8))
/*u8*/  #define gBattlersCount 			(*((u8*) 0x2023BCC))
/*u16*/ #define gBattlerPartyIndexes 	 ((u16*) 0x2023BCE)
/*u8*/  #define gBanksBySide 			 ((u8*) 0x2023BD6)
/*u8*/  #define gActionsByTurnOrder 	 ((u8*) 0x2023BDA)
/*u8*/  #define gBanksByTurnOrder 	   	 ((u8*) 0x2023BDE)
/*u8*/  #define gCurrentTurnActionNumber (*((u8*) 0x2023BE2))
/*u8*/  #define gCurrentActionFuncId 	(*((u8*) 0x2023BE3))

/*u8*/  #define gBattlerSpriteIds ((u8*) 0x2023D44)
/*u8*/  #define gCurrMovePos (*((u8*) 0x2023D48))
/*u8*/  #define gChosenMovePos (*((u8*) 0x2023D49))
/*u16*/ #define gCurrentMove (*((u16*) 0x2023D4A))
/*u16*/ #define gChosenMove (*((u16*) 0x2023D4C))
/*u16*/ #define gRandomMove (*((u16*) 0x2023D4E))
/*s32*/ #define gBattleMoveDamage *((s32*) 0x2023D50)
/*u32*/ #define gHpDealt *((u32*) 0x2023D54)

/*u32*/ #define gTakenDmg ((u32*) 0x2023D58)

/*u16*/ #define gLastUsedItem (*((u16*) 0x2023D68))
/*u8*/  #define gLastUsedAbility (*((u8*) 0x2023D6A))
/*u8*/	#define gBankAttacker (*((u8*) 0x2023D6B))
/*u8*/  #define gBattlerAttacker (*((u8*) 0x2023D6B))
/*u8*/	#define gBankTarget (*((u8*) 0x2023D6C))
/*u8*/  #define gBattlerTarget (*((u8*) 0x2023D6C))
/*u8*/  #define gBank1 (*((u8*) 0x2023D6D))
/*u8*/  #define gBankFainted (*((u8*) 0x2023D6D))
/*u8*/  #define gEffectBank (*((u8*) 0x2023D6E))
/*u8*/  #define gStringBank (*((u8*) 0x2023D6F))
/*u8*/  #define gAbsentBattlerFlags (*((u8*) 0x2023D70))
/*u8*/  #define gCritMultiplier (*((u8*) 0x2023D71))
/*u8*/  #define gMultiHitCounter (*((u8*) 0x2023D72))
//Padding 0x2023D73
/*u32*/ #define gBattlescriptCurrInstr (*((u8**) 0x2023D74))
//Padding 0x2023D78?
/*u8*/  #define gActionForBanks ((u8*) 0x2023D7C)

/*u8*/  #define gSelectionBattleScripts ((const u8**) 0x2023D80)
/*u8*/  #define gLastPrintedMoves ((u16*) 0x2023D90)

/*u16*/ #define gLastUsedMoves ((u16*) 0x2023D98)
/*u16*/ #define gLastLandedMoves ((u16*) 0x2023DA0)
/*u16*/ #define gLastHitByType ((u16*) 0x2023DA8)
/*u16*/ #define gLastResultingMoves ((u16*) 0x2023DB0)
/*u16*/ #define gLockedMoves ((u16*) 0x2023DB8)
/*u8*/  #define gLastHitBy ((u8*) 0x2023DC0)
/*u16*/ #define gChosenMovesByBanks ((u16*) 0x2023DC4)
/*u32*/ #define gMoveResultFlags *((u8*) 0x2023DCC)
/*u32*/ #define gHitMarker *((u32*) 0x2023DD0)

/*u8*/  #define gUnknown_2023DD4 ((u8*) 0x2023DD4)
/*u8*/  #define gTakenDmgBanks ((u8*) 0x2023DD8)
/*u16*/ #define gUnknown_2023DDC *((u16*) 0x2023DDC)
	
/*u16*/ #define gSideAffecting ((u16*) 0x2023DDE)
//Padding 0x2023DE2


/*u32*/ #define gStatuses3 ((u32*) 0x2023DFC)

//Disable Struct

/*u16*/ #define gPauseCounterBattle *((u16*) 0x2023E7C)
/*u16*/ #define gPaydayMoney *((u16*) 0x2023E7E)
/*u16*/ #define gRandomTurnNumber *((u16*) 0x2023E80)
/*u8*/  #define gBattleCommunication ((u8*) 0x2023E82)

/*u8*/  #define gUnknown_02023E83 ((u8*) 0x2023E83) //Length 0x7

/*u16*/ #define gBattleOutcome *((u16*) 0x2023E8A)

//Protect Struct
//Special Status Struct

/*u16*/ #define gBattleWeather *((u16*) 0x2023F1C)

//WishFutureKnock Struct

/*u8*/  #define gUnknown_02023F40 ((u8*) 0x2023F40) //Length 0x8
/*u8*/  #define gWeatherCounter *((u8*) 0x2023F48)
//Padding 0x3
/*u16*/ #define gIntroSlideFlags *((u16*) 0x2023F4C)

/*u16*/ #define gSentPokesToOpponent ((u8*) 0x2023F4E) //gSentPokesToOpponent[2]
/*u16*/ #define gDynamicBasePower *((u16*) 0x2023F50)
/*u16*/ #define gExpShareExp *((u16*) 0x2023F52)

/*u8*/  #define gBideDamage *((u32*) 0x2023FC8) //Length 0x4 //Part of gBattleScripting
/*u8*/  #define gStatAnimByte	*((u8*) 0x2023FD4)
/*u8*/  #define gStatChangeByte	*((u8*) 0x2023FDE)
/*u8*/  #define gUnknown_2023FEC  (u8*) (*((u32*) 0x2023FEC))
/*u8*/  #define gUnknown_2023FF0  (u8*) (*((u32*) 0x2023FF0))
/*u8*/  #define gActionSelectionCursor ((u8*) 0x2023FF8)
/*u8*/  #define gMoveSelectionCursor ((u8*) 0x2023FFC)
/*u8*/  #define gUnknown_2024000 ((u8*) 0x2024000)

/*u8*/  #define gBattlerInMenuId *((u8*) 0x2024004)

/*u8*/  #define gUnknown_2024005 ((u8*) 0x2024005) //Length 0x3
/*u8*/  #define gUnknown_2024008 ((u8*) 0x2024008) //Length 0x10

		#define gBattleSpritesDataPtr ((struct BattleSpriteData*) *((u32*) 0x2024018))
/*u8*/  #define gMonSpritesGfxPtr ((struct BattleSpriteData*) *((u32*) 0x2024018))
/*u16*/ #define gBattleMovePower *((u16*) 0x2024020)
/*u16*/ #define gMoveToLearn *((u16*) 0x2024022)
/*u8*/  #define gBattleMonForms ((u8*) 0x2024024)
/*u8*/  #define sLearningMoveTableID *((u8*) 0x2024028)
/*u8*/  #define gPlayerPartyCount *((u8*) 0x2024029)
/*u8*/  #define gEnemyPartyCount *((u8*) 0x202402A)

/*u8*/  #define gMultiuseSpriteTemplate ((struct SpriteTemplate*) 0x20244DC)
		
		//SaveBlock1ActualLocation 0x202552C
		extern struct MapHeader gMapHeader;
		
/*u8*/	#define FishingByte *((u8*) 0x2036E38)
		#define gEventObjects ((struct MapObject*) 0x2036E38)
		
		#define Var8000 (*((u16*) 0x20370B8))
		#define Var8001 (*((u16*) 0x20370BA))
		#define Var8002 (*((u16*) 0x20370BC))
		#define Var8003 (*((u16*) 0x20370BE))
		#define Var8004 (*((u16*) 0x20370C0))
		#define Var8005 (*((u16*) 0x20370C2))
		#define Var8006 (*((u16*) 0x20370C4))
		#define Var8007 (*((u16*) 0x20370C6))
		#define Var8008 (*((u16*) 0x20370C8))
		#define Var800D (*((u16*) 0x20370D0))
		#define Var8010 (*((u16*) 0X20370D6))
		
/*u16*/ #define gSpecialVar_LastResult (*((u16*) 0x20370D0))
/*u16*/ #define gSpecialVar_LastTalked (*((u16*) 0x20370D2))
/*u16*/ #define gSpecialVar_MonBoxId (*((u16*) 0x20370D6))
/*u16*/ #define gSpecialVar_MonBoxPos (*((u16*) 0x20370D8))
/*u8*/  #define gPlttBufferUnfaded ((u16*) 0x20371F8)
/*u8*/  #define gPlttBufferUnfaded2 ((u16*) 0x20373F8)
/*u8*/  #define gPlttBufferFaded ((u16*) 0x20375F8)
/*u8*/  #define gPlttBufferFaded2 ((u16*) 0x20377F8)

/*u8*/  #define gPaletteFade ((struct PaletteFadeControl*) 0x2037AB8)
/*u32*/	#define sPlttBufferTransferPending (*((u32*) 0x2037AC8))
/*u32*/	#define sBattleAnimScriptPtr (*((u8**) 0x2037ED4))
		#define gAnimScriptCallback (*((u32*) 0x2037EDC))
/*s8*/	#define gAnimFramesToWait (*((s8*) 0x2037EE0))
/*s16*/ #define gBattleAnimArgs ((s16*) 0x2037F02)
/*u8*/  #define gBattleAnimAttacker (*((u8*) 0x2037F1A))
/*u8*/  #define gBattleAnimTarget (*((u8*) 0x2037F1B))

		#define gWeatherPtr ((struct Weather*) 0x2037F34)

/*u8*/  #define sPaletteGammaTypes ((u8*) 0x20386A4)
/*u16*/ #define sTrainerBattleMode (*((u16*) 0x20386AC))
/*u16*/ #define gTrainerBattleOpponent_A (*((u16*) 0x20386AE))
		#define sTrainerEventObjectLocalId (*((u16*) 0x20386B0))
		#define sTrainerIntroSpeech_A (*((u32*) 0x20386B4))
		#define sTrainerDefeatSpeech_A (*((u32*) 0x20386B8))
		#define sTrainerVictorySpeech (*((u32*) 0x20386BC))
		#define sTrainerCannotBattleSpeech (*((u32*) 0x20386C0))
		#define sTrainerBattleEndScript (*((u32*) 0x20386C4))
		#define sTrainerBattleScriptRetAddr (*((u32*) 0x20386C8))
		#define sTrainerBattleOakTutorialHelper (*((u16*) 0x20386CC))
		
		//#define gEncounterRate ((struct EncounterRate*) 0x20386D0)
		#define sWildEncountersDisabled (*((u8*) 0x20386DC))

/*u8*/  #define gFieldEffectArguments ((u8*) 0x20386E0)

/*u8*/  #define gSafariBallNumber 	(*((u8*) 0x02039994))
/*u16*/ #define gSafariSteps	   (*((u16*) 0x02039996))

/*u8*/  #define gAbilitiesPerBank ((u8*) 0x2039A30)
		//#define gStringInfo ((struct BattleMsgData*) *((u32*) 0x2039A34))

/*u8*/	#define QuestLogMode (*((u8*) 0x203ADFA))
		#define gPartyMenuView ((struct PartyMenuViewing*) 0x203B09C)
		#define gSelectedOrderFromParty ((u8*) 0x203B0D4)
/*u8*/  #define gUnknown_0203B0DC ((u8*) 0x203B0DC)

		#define gScriptEnv1 ((struct ScriptContext*) 0x03000EB0)
		#define gScriptEnv2 ((struct ScriptContext*) 0x03000F28)
		#define gLoadPointer (*((u32*) 0x03000f14))
		#define sScriptContext2Enabled (*((u8*) 0x3000F28))
		#define gKeypadInitialCountdown ((*u16) 0x030030e0)
		#define gMain (((struct Main*) 0x30030F0)[0])
		#define gKeypadFollowUpCountdown ((*u16) 0x0300352c)
/*u32*/ #define gBattleMainFunc (*((u32*) 0x3004F84))
/*u8*/  #define gLeveledUpInBattle (*((u8*) 0x3004FD4))
		#define gBattleBankFunc ((u32*) 0x3004FE0)
		#define gHealthboxIDs ((u8*) 0x3004FF0)
		#define gMultiUsePlayerCursor (*((u8*) 0x3004FF4))
		#define gNumberOfMovesToChoose (*((u8*) 0x3004FF8))
/*u8*/  #define gSelectedEventObject (*((u8*) 0x3005074))
		#define gTasks ((struct Task*) 0x3005090) //struct Task gTasks[ACTIVE_SENTINEL];
		
		#define gCurrentMapName *((u8*) 0x3005558)
		
		#define gGbaTimer ((struct GbaTimer*) 0x4000108)
		#define gKeyReg (*((u16*) 0x4000130))
				
		extern struct MusicPlayerInfo gMPlay_BGM;

/*Custom Ram Locs*/
/*
#define DisabledMoldBreakerAbilities ((u8*) 0x203C034)
#define SuppressedAbilities ((u8*) 0x203C038)
#define SwitchInEffectsTracker (*((u8*) 0x203C03D))
#define SentInBackup (*((u8*) 0x203C03E))
#define NoMoreMovingThisTurn (*((u8*) 0x203C03F))
#define PayDayByPartyIndices ((u8*) 0x203C040)
#define ZMoveData ((struct ZMoveData*) 0x203C048)
#define MegaData ((struct MegaData*) 0x203C060)

#define OriginalAttackerTargetCount (*((u8*) 0x203C080))
#define MoveBounceTargetCount (*((u8*) 0x203C081))
#define AttackerDidDamageAtLeastOnce (*((u8*) 0x203C082)) //For life orb

#define PledgeHelper *((u8*) 0x203D801)
#define GravityTimer *((u8*) 0x203D802)
#define WonderRoomTimer *((u8*) 0x203D803)
#define SeaOfFireTimers ((u8*) 0x203D804)
#define SwampTimers ((u8*) 0x203D806)
#define RainbowTimers ((u8*) 0x203D808)
#define TrickRoomTimer *((u8*) 0x203D80A)
#define TelekinesisTimers ((u8*) 0x203D80B)
#define MagicRoomTimer *((u8*) 0x203D80F)
#define MoveBounceInProgress *((u8*) 0x203D810)
#define RetaliateCounters ((u8*) 0x203D811)
#define LuckyChantTimers ((u8*) 0x203D813)
#define TailwindTimers ((u8*) 0x203D815)
#define EchoedVoiceCounter *((u8*) 0x203D818)
#define EchoedVoiceDamageScale *((u8*) 0x203D819)
#define CustapQuickClawHelper *((u8*) 0x203D81A)
#define TerrainTimer *((u8*) 0x203D81C)
#define PrimalWeatherLoc *((u8*) 0x203D81D)

#define MultiHitByte *((u8*) 0x203D823)
#define MagnetRiseTimers ((u8*) 0x203D824)
#define HealBlockTimers ((u8*) 0x203D828)
#define LaserFocusTimers ((u8*) 0x203D82C)
#define ThroatChopTimers ((u8*) 0x203D830)
#define BeakBlastByte *((u8*) 0x203D834)
#define ParentalBondOn *((u8*) 0x203D835)
#define CustapQuickClawIndicator *((u8*) 0x203D836)
#define MeFirstByte *((u8*) 0x203D837)
#define AuroraVeilTimers ((u8*) 0x203D838)
#define FormCounter *((u8*) 0x203D83B)
#define StompingTantrumTimers ((u8*) 0x203D83C)
#define MimikyuByte *((u8*) 0x203D840)
#define PoisonedBy *((u8*) 0x203D844)
	// 0x1 = Poison Touch	/ 0x1 = Beak Blast Burn
	// 0x2 = Toxic Spikes
	// 0x3 = Toxic Orb		/ 0x3 = Flame Orb
	// 0x4 = Baneful Bunker
#define GalvanizeHelper *((u8*) 0x203D845)
#define LastUsedMove *((u16*) 0x203D846)
#define WaterSportTimer *((u8*) 0x203D848)
#define MudSportTimer *((u8*) 0x203D849)
#define LastUsedTypes ((u8*) 0x203D84A)
#define TargetsHit *((u8*) 0x203D84B)
#define ReceiverActivated *((u8*) 0x203D84C)
#define MetronomeCounter ((u8*) 0x203D853)
#define StakeoutCounters ((u8*) 0x203D857)
#define IonDelugeTimer *((u8*) 0x203D85B)
#define RoostCounter *((u8*) 0x203D85F)
#define NormalizeHelper *((u8*) 0x203D860)
#define GemHelper *((u8*) 0x203D861)
#define ZMoveActive *((u8*) 0x203D862)
#define SpectralThiefActive *((u8*) 0x203D863)
#define NoSymbiosisByte *((u8*) 0x203D864)
#define DancerByte *((u8*) 0x203D865)
#define FlowerShieldHelper *((u8*) 0x203D866)
#define EnduranceHelper *((u8*) 0x203D867)
#define DestinyBondCounters ((u8*) 0x203D868)
#define SlowStartTimers ((u8*) 0x203D86C)
#define NewWishHealthSave *((u16*) 0x203D87C)

#define HealingWishLoc (*((u8*) 0x203D881))
#define FaintEffectsTracker (*((u8*) 0x203D882)) 

#define IncinerateCounters ((u8*) 0x203D885)

#define MagicianHelper *((u8*) 0x203D88A)
#define PowderByte *((u8*) 0x0203D88C)

#define DancerInProgress (*((u8*) 0x203D890))
#define DancerBankCount (*((u8*) 0x203D891))
#define CurrentTurnAttacker (*((u8*) 0x203D892))

#define EmbargoTimers ((u8*) 0x203D894)
#define ElectrifyTimers ((u8*) 0x203D898)
#define BelchCounters (*((u8*) 0x203D89C)) //Bit field
#define IllusionBrokenBytes ((u8*) 0x203D8A8)

#define MoodyLowerStat *((u8*) 0x203D8B4)
#define blockTracker *((u8*) 0x203D8B5)
#define FairyLockTimer *((u8*) 0x203D8B7)
#define HappyHourByte *((u8*) 0x203D8B8)
#define UnburdenBoosts *((u8*) 0x203D8B9)
#define NotEndTurnByte *((u8*) 0x203D8BA)
#define AttackAnimationPlayed *((u8*) 0x203D8BB)

#define MicleBerryBits (*((u8*) 0x203D8CA))

#define DancerTurnOrder ((u8*) 0x203D8D2)
#define InstructInProgress *((u8*) 0x203D8D6)

#define SavedConsumedItems ((u16*) 0x203D8E2)
#define NimbleCounters ((u8*) 0x203D8FA)

#define EndTurnDoneByte *((u8*) 0x203D8FE)
#define AbsentBattlerHelper *((u8*) 0x203D8FF)

typedef u8 IllusionNickname_t[10];
#define IllusionNickNameBackup ((IllusionNickname_t*) 0x203D900)

#define UltraData ((struct UltraData*) 0x203D8D8)
*/

#define BattleStringLoader (*((u8**) 0x203C020)) //(*((u8**) 0x203F700))
#define SeedHelper ((u8*) 0x203F704)
#define TerrainType *((u8*) 0x203F708)
#define FormCounter *((u8*) 0x203F709)
#define PoisonedBy *((u8*) 0x203F70A)
	// 0x1 = Poison Touch	/ 0x1 = Beak Blast Burn
	// 0x2 = Toxic Spikes
	// 0x3 = Toxic Orb		/ 0x3 = Flame Orb
	// 0x4 = Baneful Bunker
#define MagicianHelper *((u8*) 0x203F70B)
#define ShakerData (((u8*) 0x203F70C))
#define ForceSwitchHelper (*((u8*) 0x203F70E))
#define AbilityPopUpHelper (*((u8*) 0x203F70F))

#define gExpandedFlags ((u8*) 0x0203B174)
#define gExpandedVars ((u16*) (0x0203B174 + 0x200))

// JPAN Code
#define gMultiChoice ((struct Multichoice*) 0x203B774)	// up to 7 pointers, 8 bytes each
#define gPcSelectionTracker ((u8* 0x203B7AC)	// state tracker for pc selection
#define gCreateSpriteTableSetter 0x203B7AD  // allow createsprite to load from a table as well
#define gTimerValue (*(u16*) 0x203B7AE)
#define gKeypadSetter ((struct KeypadSetter*) 0x203B7B0)	//12 bytes
#define gPedometers ((struct Pedometers*) 0x203B7BC)		//12 bytes
#define gWalkingScript (*(u32*) 0x203B7C8)

// Dynamic Overworld Palettes
#define PalRefs ((PalRef*)0x203FF00)


