#pragma once

#include "../global.h"

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

#define gSaveBlock1 ((struct SaveBlock1*) *((u32*) 0x3005008))
#define gSaveBlock2 ((struct SaveBlock2*) *((u32*) 0x300500C))
#define gSaveBlock3 ((struct SaveBlock3*) *((u32*) 0x3005010))

#define Clock ((struct Clock*) 0x300553C)

#define gEnemyParty ((struct Pokemon*) 0x202402C)
#define gPlayerParty ((struct Pokemon*) 0x2024284)

//gBitTable;

/*u8*/  //#define gDecompressionBuffer 	 ((u8*) 0x201C000)
/*u32*/ #define gTrainerId 				*((u32*) 0x2020000)

		#define gWindows ((struct Window*) 0x20204B4)
/*u8*/  #define gSprites ((struct Sprite*) 0x202063C)

/*u8*/  #define gStringVar1 ((u8*) 0x2021CD0)
/*u8*/  #define gStringVar2 ((u8*) 0x2021CF0)
/*u8*/  #define gStringVar3 ((u8*) 0x2021D04)
/*u8*/  #define gStringVar4 ((u8*) 0x2021D18)

		#define ColorFilter *((u8*)0x2036E28)

		#define gWarp1 ((struct WarpData*) 0x2031dbc)
		
		#define gPlayerAvatar ((struct PlayerAvatar*) 0x02037078)
		
		#define gBoxStatusAndType ((u8*) 0x0203709C)		

/*u8*/  #define gUnknown_2024005 ((u8*) 0x2024005) //Length 0x3
/*u8*/  #define gUnknown_2024008 ((u8*) 0x2024008) //Length 0x10

/*u8*/  #define sLearningMoveTableID *((u8*) 0x2024028)
/*u8*/  #define gPlayerPartyCount *((u8*) 0x2024029)
/*u8*/  #define gEnemyPartyCount *((u8*) 0x202402A)

/*u8*/  #define gMultiuseSpriteTemplate ((struct SpriteTemplate*) 0x20244DC)
		
		//SaveBlock1ActualLocation 0x202552C
		extern struct MapHeader gMapHeader;
		
/*u8*/	#define FishingByte *((u8*) 0x2036E38)
		#define gEventObjects ((struct MapObject*) 0x2036E38)
/*		
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


#define gSpecialVar_LastResult (*((u16*) 0x20370D0))
#define gSpecialVar_LastTalked (*((u16*) 0x20370D2))
#define gSpecialVar_MonBoxId (*((u16*) 0x20370D6))
#define gSpecialVar_MonBoxPos (*((u16*) 0x20370D8))
*/

/*u8*/  #define gPlttBufferUnfaded ((u16*) 0x20371F8)
/*u8*/  #define gPlttBufferUnfaded2 ((u16*) 0x20373F8)
/*u8*/  #define gPlttBufferFaded ((u16*) 0x20375F8)
/*u8*/  #define gPlttBufferFaded2 ((u16*) 0x20377F8)

/*u8*/  #define gPaletteFade ((struct PaletteFadeControl*) 0x2037AB8)
/*u32*/	#define sPlttBufferTransferPending (*((u32*) 0x2037AC8))

		#define gWeatherPtr ((struct Weather*) 0x2037F34)

/*u8*/  #define sPaletteGammaTypes ((u8*) 0x20386A4)
		
		//#define gEncounterRate ((struct EncounterRate*) 0x20386D0)
		#define sWildEncountersDisabled (*((u8*) 0x20386DC))

/*u8*/  //#define gFieldEffectArguments ((struct FieldEffectArguments*) 0x20386E0)

/*u8*/  #define gSafariBallNumber 	(*((u8*) 0x02039994))
/*u16*/ #define gSafariSteps	   (*((u16*) 0x02039996))

/*u8*/  #define gAbilitiesPerBank ((u8*) 0x2039A30)
		//#define gStringInfo ((struct BattleMsgData*) *((u32*) 0x2039A34))

/*u8*/	#define QuestLogMode (*((u8*) 0x203ADFA))
		//#define gPartyMenuView ((struct PartyMenuViewing*) 0x203B09C)
		#define gSelectedOrderFromParty ((u8*) 0x203B0D4)
		#define gSummaryScreenData ((struct MonSummaryData*) *((u32*) 0x203B140))
		#define gCurrentPartyIndex (*(u8*) 0x203B16C)
		
/*u8*/  #define gUnknown_0203B0DC ((u8*) 0x203B0DC)

		#define AlphaBlendingCoeffA *((u8*)0x3000052)
		#define PalTagsStart *((u8*)0x3003E58)
		
		#define PalTags ((u16*)0x3000DE8)

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
		//#define gTasks ((struct Task*) 0x3005090) //struct Task gTasks[ACTIVE_SENTINEL];
		
		#define gCurrentMapName *((u8*) 0x3005558)
		
		#define gGbaTimer ((struct GbaTimer*) 0x4000108)
		#define gKeyReg (*((u16*) 0x4000130))
				
		extern struct MusicPlayerInfo gMPlay_BGM;

#define gExpandedFlags ((u8*) 0x0203B174)
#define gExpandedVars ((u16*) (0x0203B174 + 0x200))

#define gMultiChoice ((struct Multichoice*) 0x203B774)	// up to 7 pointers, 8 bytes each
#define gPcSelectionTracker ((u8* 0x203B7AC)	// state tracker for pc selection
#define gCreateSpriteTableSetter 0x203B7AD  // allow createsprite to load from a table as well
#define gTimerValue (*(u16*) 0x203B7AE)
#define gKeypadSetter ((struct KeypadSetter*) 0x203B7B0)	//12 bytes
#define gPedometers ((struct Pedometers*) 0x203B7BC)		//12 bytes
#define gWalkingScript (*(u32*) 0x203B7C8)

// Dynamic Overworld Palettes
//#define PalRefs ((PalRef*)0x203FF00)
#define PalRefs ((PalRef*)0x203B7D4)	// 16 sprites * 4 bytes each = 0x40 bytes

#define DEXNAV_SAVERAM 0x203B814
//#define gFollowerState ((struct Follower*) 0x203B818) //Approximately ~20 bytes, use 24 to be safe
//#define [FILL_IN_NEXT] 0x203B830

