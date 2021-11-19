#pragma once

#include "../global.h"
#include "../list_menu.h"

//#define gSaveBlock1 ((struct SaveBlock1*) *((u32*) 0x3005008))
//#define gSaveBlock2 ((struct SaveBlock2*) *((u32*) 0x300500C))
//#define gSaveBlock3 ((struct SaveBlock3*) *((u32*) 0x3005010))

//#define gEnemyParty ((struct Pokemon*) 0x202402C)
//#define gPlayerParty ((struct Pokemon*) 0x2024284)

//gBitTable;

/*u8*/  //#define gDecompressionBuffer 	 ((u8*) 0x201C000)
/*u32*/ #define gTrainerId 				*((u32*) 0x2020000)

		#define gWindows ((struct Window*) 0x20204B4)
/*u8*/  //#define gSprites ((struct Sprite*) 0x202063C)

///*u8*/  #define gStringVar1 ((u8*) 0x2021CD0)
///*u8*/  #define gStringVar2 ((u8*) 0x2021CF0)
///*u8*/  #define gStringVar3 ((u8*) 0x2021D04)
///*u8*/  #define gStringVar4 ((u8*) 0x2021D18)

		#define ColorFilter *((u8*)0x2036E28)

		#define gWarp1 ((struct WarpData*) 0x2031dbc)

		#define gPlayerAvatar ((struct PlayerAvatar*) 0x02037078)

		#define gBoxStatusAndType ((u8*) 0x0203709C)

/*u8*/  //#define sLearningMoveTableID *((u8*) 0x2024028)
/*u8*/  #define gPlayerPartyCount *((u8*) 0x2024029)
/*u8*/  #define gEnemyPartyCount *((u8*) 0x202402A)

/*u8*/  #define gMultiuseSpriteTemplate ((struct SpriteTemplate*) 0x20244DC)

		extern struct MapHeader gMapHeader;
		//#define gEventObjects ((struct EventObject*) 0x2036E38)
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
		extern u16 gScanlineEffectRegBuffers[2][0x3C0];

		//#define gEncounterRate ((struct EncounterRate*) 0x20386D0)
		#define sWildEncountersDisabled (*((u8*) 0x20386DC))

/*u8*/  //#define gFieldEffectArguments ((struct FieldEffectArguments*) 0x20386E0)

		#define gNamingScreenData ((struct NamingScreenData*) (*(u32*) 0x203998C))
/*u8*/ //#define gSafariBallNumber 	(*((u8*) 0x02039994))	// see src/scripting.c
/*u16*/ #define gSafariSteps	   (*((u16*) 0x02039996))

/*u8*/  #define gAbilitiesPerBank ((u8*) 0x2039A30)
		//#define gStringInfo ((struct BattleMsgData*) *((u32*) 0x2039A34))

		#define sHofSelectedPaletteIndices (*((u32*) 0x0203AB34))
		#define gHasHallOfFameRecords (*(u8*) 0x0203AB44)

/*u8*/	#define gQuestLogMode (*((u8*) 0x203ADFA))
		//#define sPartyMenuInternal ((struct PartyMenuViewing*) 0x203B09C)
		//#define gSelectedOrderFromParty ((u8*) 0x203B0D4)
		#define sLastViewedMonIndex (*(u8*) 0x203B16C)

/*u8*/  #define gBattlePartyCurrentOrder ((u8*) 0x203B0DC)

		#define AlphaBlendingCoeffA *((u8*)0x3000052)
		#define PalTagsStart *((u8*)0x3003E58)

		#define PalTags ((u16*)0x3000DE8)

		#define gMultiuseListMenuTemplate ((struct ListMenuTemplate*) ((u32*) 0x3005E70))

		//#define gScriptEnv1 ((struct ScriptContext*) 0x03000EB0)
		//#define gScriptEnv2 ((struct ScriptContext*) 0x03000F28)
		//#define gLoadPointer (*((const u8**) 0x03000f14))
		//#define sScriptContext2Enabled (*((u8*) 0x3000F9C))
		//#define gKeypadInitialCountdown ((*u16) 0x030030e0)
		//#define gMain (((struct Main*) 0x30030F0)[0])
		//#define gKeypadFollowUpCountdown ((*u16) 0x0300352c)
///*u32*/ #define gBattleMainFunc (*((u32*) 0x3004F84))
///*u8*/  #define gLeveledUpInBattle (*((u8*) 0x3004FD4))
//		#define gBattleBankFunc ((u32*) 0x3004FE0)
//		#define gHealthboxIDs ((u8*) 0x3004FF0)
//		#define gMultiUsePlayerCursor (*((u8*) 0x3004FF4))
//		#define gNumberOfMovesToChoose (*((u8*) 0x3004FF8))

		#define gOverworldTilemapBg3 (*((u32*) 0x300501C))

/*u8*/  #define gSelectedEventObject (*((u8*) 0x3005074))
		//#define gTasks ((struct Task*) 0x3005090) //struct Task gTasks[ACTIVE_SENTINEL];

		#define gCurrentMapName gMapHeader.regionMapSectionId

		//#define gGbaTimer ((struct GbaTimer*) 0x4000108)
		#define gKeyReg (*((u16*) 0x4000130))

		//Expanded text buffers					//Size					//buffercommand X	//String Arg
/*u8*/  #define gStringVar7 ((u8*) 0x2022BC4)	//32 bytes					//Buffer 0x5 	//FD 07
/*u8*/  #define gStringVar8 ((u8*) 0x2022BE4)	//32 bytes					//Buffer 0x6	//FD 08
/*u8*/  #define gStringVar9 ((u8*) 0x2022C04)	//32 bytes					//Buffer 0x7	//FD 09
/*u8*/  #define gStringVarA ((u8*) 0x2022C24)	//32 bytes					//Buffer 0x8	//FD 0A
/*u8*/  #define gStringVarB ((u8*) 0x2022C44)	//32 bytes					//Buffer 0x9	//FD 0B
/*u8*/  #define gStringVarC ((u8*) 0x2022C64)	//100 bytes					//Buffer 0xA	//FD 0C
/*u8*/  #define gStringVarD ((u8*) 0x2022CC8)	//100 bytes (to 0x2022D2C)	//Buffer 0xB	//FD 0D

#define gExpandedFlags ((u8*) 0x0203B174)
#define gExpandedVars ((u16*) (0x0203B174 + 0x200))
extern u16 gLastUsedBall; //0x203B774

//FREE: 0x203B776

#define gPcSelectionTracker ((u8* 0x203B7AC)	// state tracker for pc selection
#define gCreateSpriteTableSetter 0x203B7AD  // allow createsprite to load from a table as well
#define gTimerValue (*(u16*) 0x203B7AE)
//#define gKeypadSetter ((struct KeypadSetter*) 0x203B7B0)	//12 bytes
//#define gPedometers ((struct Pedometers*) 0x203B7BC)		//12 bytes
#define gWalkingScript (*((const u8**) 0x203B7C8))

// Dynamic Overworld Palettes
//#define sPalRefs ((PalRef*) 0x203B7D4)	// 16 sprites * 4 bytes each = 0x40 bytes

#define gPlayerCoins (*((u32*) 0x203B814))
//#define gFollowerState ((struct Follower*) 0x203B818) //Approximately ~20 bytes, use 24 to be safe
//extern bool8 gIgnoredDNSPalIndices[32][16] //0x203B830
//extern struct Roamer gRoamers[10] //0x203BA30
//extern struct ItemSlot gBagRegularItems[450] //0x203BB20
//extern struct ItemSlot gBagKeyItems[75] //0x203C228
//extern struct ItemSlot gBagPokeBalls[50] //0x203C354
//extern struct ItemSlot gBagTMHM[128] //0x203C41C
//extern struct ItemSlot gBagBerries[75] //0x203C61C

#define gLastUsedRepel (*(u16*) 0x203C748)
//#define sBagItemAmounts ((u16*) 0x203C74A)
#define gSelectedOrderFromParty ((u8*) 0x203C750)
extern bool8 gDontFadeWhite; //0x203C756
#define gWindowsLitUp (*((bool8*) 0x203C757))
#define gWildDataSwitch (*((const u8**) 0x203C758))
extern u8 gDexNavSearchLevels[999]; //0x203C75C -999 slots
//extern struct CompressedPokemon gBox20[30] //0x203CB44
//extern struct CompressedPokemon gBox21[30] //0x203D210
//extern struct CompressedPokemon gBox22[30] //0x203D8DC

#define gPokeBackupPtr (*((const void**) 0x203E034)) //Not used during battle
//#define sDexNavStatePtr/sFrontierRecordsPtr/gNewBS //0x203E038
extern bool8 gInShop; //0x203E043
#define gBattleCircusFlags (*((u32*) 0x203E044))
#define gFishingByte *((u8*) 0x203E048)
#define gLastRecordedFadeCoeff (*((u8*) 0x203E049))
#define gLastRecordedFadeColour (*((u16*) 0x203E04A))
#define gRaidBattleSpecies (*((u16*) 0x203E04C))
#define gRaidBattleStars (*((u8*) 0x203E04E))
#define gRaidBattleLevel (*((u8*) 0x203E04F))
#define gInvertAMPM (*((bool8*) 0x203E050))
#define gCurrentDexNavChain (*((u8*) 0x203E051))
#define gDexNavStartedBattle (*((bool8*) 0x203E052))
//#define sItemDescriptionPocket (*((bool8*) 0x203E053))
//#define sStartMenuOpen (*((bool8*) 0x203E054))
//#define sPrintedRTCWarning (*((bool8*) 0x203E055))
extern u16 gLastFishingSpecies; //0x203E056
extern u8 gFishingStreak; //0x203E058
//#define sLocked (*((bool8*) 0x203E059))
//#define sRTCErrorStatus (*((u16*) 0x203E05A))
//#define sRTCSavedIme (*((u16*) 0x203E05C))
//#define sRTCProbeResult (*((u8*) 0x203E05E))
extern bool8 gDexNavCooldown; //0x203E05F
extern struct Coords16 gMiningSpots[8]; //0x203E060
//#define sSavedWildDataDaytimeHeader (*((const struct WildPokemonHeader**) 0x203E080))
//#define sSavedWildDataMapGroup (*((u8*) 0x203E084))
//#define sSavedWildDataMapNum (*((u8*) 0x203E085))
extern u16 gLastDexNavSpecies; //0x203E086
//extern struct ListMenuItem[7] gMultiChoice; //0x203E088 - up to 7 pointers, 8 bytes each
//extern u8 gPartyPresetTeamIndices[PARTY_SIZE]; //0x203E0C0
extern u8 gScored3CritsInBattle; //0x203E0C6
//FREE: 0x203E0C7

//DON'T GO PAST 0x203E0D4 for BTS!
//extern struct CompressedPokemon gTempTeamBackup[6] //0x203E1A4
