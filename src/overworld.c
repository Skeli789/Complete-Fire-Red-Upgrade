#include "defines.h"
#include "defines_battle.h"
#include "../include/battle_setup.h"
#include "../include/bg.h"
#include "../include/daycare.h"
#include "../include/event_data.h"
#include "../include/event_object_movement.h"
#include "../include/field_control_avatar.h"
#include "../include/field_effect.h"
#include "../include/field_effect_helpers.h"
#include "../include/field_fadetransition.h"
#include "../include/field_message_box.h"
#include "../include/field_player_avatar.h"
#include "../include/field_poison.h"
#include "../include/field_screen_effect.h"
#include "../include/field_weather.h"
#include "../include/fieldmap.h"
#include "../include/fldeff_misc.h"
#include "../include/item.h"
#include "../include/link.h"
#include "../include/list_menu.h"
#include "../include/m4a.h"
#include "../include/map_name_popup.h"
#include "../include/map_preview_screen.h"
#include "../include/map_scripts.h"
#include "../include/metatile_behavior.h"
#include "../include/overworld.h"
#include "../include/party_menu.h"
#include "../include/quest_log.h"
#include "../include/random.h"
#include "../include/rtc.h"
#include "../include/safari_zone.h"
#include "../include/script.h"
#include "../include/script_menu.h"
#include "../include/sound.h"
#include "../include/string_util.h"

#include "../include/constants/flags.h"
#include "../include/constants/items.h"
#include "../include/constants/maps.h"
#include "../include/constants/metatile_behaviors.h"
#include "../include/constants/region_map_sections.h"
#include "../include/constants/songs.h"
#include "../include/constants/trainers.h"
#include "../include/constants/trainer_classes.h"

#include "../include/new/dexnav.h"
#include "../include/new/dynamic_ow_pals.h"
#include "../include/new/item.h"
#include "../include/new/follow_me.h"
#include "../include/new/frontier.h"
#include "../include/new/util.h"
#include "../include/new/multi.h"
#include "../include/new/overworld.h"
#include "../include/new/overworld_data.h"
#include "../include/new/party_menu.h"
#include "../include/new/read_keys.h"
#include "../include/new/wild_encounter.h"

/*
overworld.c
	functions for anything regarding the overworld, such as trainer spotting, whiteout, step counters, etc.

tables:
	gDefaultWalkingScripts
	sMetatileInteractionScripts
	sContinueLostBattleParams
	sTrainerBContinueScriptBattleParams
	sTrainerBOrdinaryBattleParams
	sMultiBattleParams
	sTwoOpponentBattleParams
	sTagBattleParams

*/

extern const u16 gClassBasedTrainerEncounterBGM[NUM_TRAINER_CLASSES];

//External functions
extern void sp09A_StopSounds(void);

//This file's functions:
static bool8 CheckTrainerSpotting(u8 eventObjId);
static bool8 GetTrainerFlagFromScriptPointer(const u8* data);
static bool8 CheckNPCSpotting(u8 eventObjId);
static void Task_OverworldMultiTrainers(u8 id);
static void ConfigureTwoTrainersBattle(u8 trainerEventObjId, const u8* trainerScript);
static void SetUpTwoTrainersBattle(void);
static void InitTrainerBattleVariables(void);
static u8 GetPlayerMapObjId(void);
static u8 GetNPCDirectionFaceToPlayer(u8 eventObj);
static bool8 GetProperDirection(u16 currentX, u16 currentY, u16 toX, u16 toY);
static void UpdateJPANStepCounters(void);
#ifdef DEXNAV_DETECTOR_MODE
static void Task_StartDexNavHUDAfterScript(u8 taskId);
#endif
static const u8* GetCustomWalkingScript(void);
static bool8 SafariZoneTakeStep(void);
static bool8 IsRunningDisabledByFlag(void);
static bool8 IsPlayerFacingSea(void);
static bool8 UseRegisteredKeyItemOnField(void);

#if (defined VAR_DEFAULT_WALKING_SCRIPT && !defined UNBOUND)
//Table full of pointers to custom walking scripts
static const u8* const sDefaultWalkingScripts[] =
{
	NULL,
	//NULL,
	//NULL,
	//NULL,
	//etc
};
#endif

//Table full of pointers of scripts run when talking to tiles with certain behaviour bytes
static const u8* const sMetatileInteractionScripts[] =
{
	[MB_BOOKSHELF] = (void*) 0x81A7606,
	[MB_POKEMART_SHELF] = (void*) 0x81A760F,
	[MB_PC] = (void*) 0x81A6955,
	[MB_REGION_MAP] = (void*) 0x81A6C32,
	[MB_CABINET] = (void*) 0x81A7657,
	[MB_KITCHEN] = (void*) 0x81A7660,
	[MB_DRESSER] = (void*) 0x81A7669,
	[MB_SNACKS] = (void*) 0x81A7672,
	[MB_QUESTIONNAIRE] = (void*) 0x81A7702,
	[MB_FOOD] = (void*) 0x81A7618,
	[MB_BLUEPRINTS] = (void*) 0x81A763C,
	[MB_PAINTING] = (void*) 0x81A767B,
	[MB_POWER_PLANT_MACHINE] = (void*) 0x81A7684,
	[MB_TELEPHONE] = (void*) 0x81A768D,
	[MB_COMPUTER] = (void*) 0x81A762A,
	[MB_ADVERTISING_POSTER] = (void*) 0x81A7696,
	[MB_FOOD_SMELLS_TASTY] = (void*) 0x81A769F,
	[MB_TRASH_BIN] = (void*) 0x81A76A8,
	[MB_CUP] = (void*) 0x81A76B1,
	[MB_BLINKING_LIGHTS] = (void*) 0x81A76CC,
	[MB_TOOLS] = (void*) 0x81A76D5,
	[MB_IMPRESSIVE_MACHINE] = (void*) 0x81A7633,
	[MB_VIDEO_GAME] = (void*) 0x81A7621,
	[MB_BURGLARY] = (void*) 0x81A7645,
	[MB_TRAINER_TOWER_RECORD] = (void*) 0x81C549C,

#if (defined UNBOUND && defined SWARM_CHANGE_HOURLY)
	[MB_TELEVISION] = EventScript_TVSwarm, //Relates info on the daily swarm
#else
	[MB_TELEVISION] = (void*) 0x81A764E,
#endif
	[MB_BERRY_CRUSH_RECORDS] = (void*) 0x81BBFD8,
	[MB_BATTLE_RECORDS] = (void*) 0x81BB8A7,

	[MB_POKEMON_CENTER_SIGN] = (void*) 0x81A76E7,
	[MB_POKEMART_SIGN] = (void*) 0x81A76DE,
	[MB_INDIGO_PLATEAU_MARK_DPAD] = (void*) 0x81A76F0,
	[MB_INDIGO_PLATEAU_MARK_2_DPAD] = (void*) 0x81A76F9,

#ifdef MB_LOCKED_DOOR
	[MB_LOCKED_DOOR] = EventScript_LockedDoor,
#endif
#ifdef MB_PSYCHIC_BARRIER
	[MB_PSYCHIC_BARRIER] = EventScript_PsychicBarrier,
#endif
#ifdef MB_CLIMBABLE_LADDER
	[MB_CLIMBABLE_LADDER] = EventScript_Ladder,
#endif
#ifdef MB_HIDDEN_GROTTO_FOREST
	[MB_HIDDEN_GROTTO_FOREST] = EventScript_HiddenGrottoForest,
#endif
#ifdef MB_HEADBUTT_TREE
	[MB_HEADBUTT_TREE] = EventScript_HeadbuttTree,
#endif
#ifdef MB_UNDERGROUND_MINING
	[MB_UNDERGROUND_MINING] = EventScript_UndergroundMining,
#endif
};

/*
Below is a table of grass tiles related to the field move Cut.
When the player uses cut in tall grass, any grass tiles around
the player will be replaced. In the table, the first element of
each struct is the original tile number, the second element is the
the tile to replace it with, and third element is the tileset pointer.
For example, the first entry (0xD -> 0x1) replaces the vanilla FR tall
grass tile with the vanilla FR plain grass tile while in Tileset 0.
Look in AdvanceMap for tile numbers.
*/

#define CUT_GRASS_TERMIN {0xFFFF, 0xFFFF, NULL}
struct CutGrass {u16 originalMetatileId; u16 newMetatileId; struct Tileset* tilesetPtr;};
const struct CutGrass sCutGrassTiles[] =
#ifndef UNBOUND //Modify this
{
	#define Tileset_0 (struct Tileset*) 0x82D4A94
	#define Tileset_7 (struct Tileset*) 0x82D4B3C
	#define Tileset_8 (struct Tileset*) 0x82D4B54
	#define Tileset_34 (struct Tileset*) 0x82D4DC4

	{0xD, 0x1, Tileset_0},
	{0xA, 0x13, Tileset_0},
	{0xB, 0xE, Tileset_0},
	{0xC, 0xF, Tileset_0},
	{0x352, 0x33E, Tileset_7}, //Cycling Road
	{0x300, 0x310, Tileset_8}, //Safari Zone
	{0x301, 0x311, Tileset_8}, //Safari Zone
	{0x302, 0x312, Tileset_8}, //Safari Zone
	{0x284, 0x281, Tileset_34}, //Viridian Forest
	CUT_GRASS_TERMIN
};
#else //For Pokemon Unbound
{
	#define Tileset_General (struct Tileset*) 0x82D4A94
	#define Tileset_CraterTown (struct Tileset*) 0x82D4ADC
	#define Tileset_DrescoTown (struct Tileset*) 0x82D4AF4
	#define Tileset_FallshoreCity (struct Tileset*) 0x8744D8C
	#define Tileset_DeharaCity (struct Tileset*) 0x82D4B3C
	#define Tileset_GurunTown (struct Tileset*) 0x82D4B24
	#define Tileset_Route17 (struct Tileset*) 0x82D4B9C
	#define Tileset_MagnoliaTown (struct Tileset*) 0x82D4D04
	#define Tileset_FlowerParadise (struct Tileset*) 0x8B56314
	#define Tileset_Forest (struct Tileset*) 0x82D5004
	#define Tileset_Snow (struct Tileset*) 0x8725AB4
	#define Tileset_Autumn (struct Tileset*) 0x873CC2C

	#define METATILE_General_PlainGrass 0x1
	#define METATILE_General_TreeTopLeft 0xE
	#define METATILE_General_TreeTopRight 0xF
	#define METATILE_General_TreeSideLeft 0x18
	#define METATILE_General_TreeSideRight 0x19
	#define METATILE_General_BothTreeSides 0x20
	#define METATILE_General_ThinTreeTop 0x1B
	#define METATILE_General_LandEdgeLeft 0x132
	#define METATILE_General_LandEdgeMiddle 0x133
	#define METATILE_General_LandEdgeRight 0x134
	#define METATILE_General_TreeTopLeftOverTreeSideRight 0x27C
	#define METATILE_General_TreeTopRightOverTreeSideLeft 0x27D

	#define METATILE_Route2_HighLandEdgeMiddle 0x2DD

	#define METATILE_Route17_LandEdgeBottomRight 0x28C
	#define METATILE_Route17_LandEdgeBottomLeft 0x28B
	#define METATILE_Route17_LandEdgeBottomMiddle 0x299

	#define METATILE_Forest_Ground 0x280
	#define METATILE_Forest_Pressure_Plate 0x297
	#define METATILE_Forest_TreeTopLeft 0x289
	#define METATILE_Forest_TreeTopRight 0x28A
	#define METATILE_Forest_TreeTopCornerLeft 0x2EE
	#define METATILE_Forest_TreeTopCornerRight 0x2EF
	#define METATILE_Forest_TreeSideLeft 0x290
	#define METATILE_Forest_TreeSideRight 0x293

	#define METATILE_Autumn_Ground 0x296
	#define METATILE_Autumn_TreeTopLeft 0x280
	#define METATILE_Autumn_TreeTopMiddleLeft 0x281
	#define METATILE_Autumn_TreeTopMiddleRight 0x282
	#define METATILE_Autumn_TreeTopRight 0x283
	#define METATILE_Autumn_TreeTopMiddleLeftJoined 0x28C
	#define METATILE_Autumn_TreeTopLeftCorner 0x298
	#define METATILE_Autumn_TreeTopRightCorner 0x282
	#define METATILE_Autumn_TreeSideLeftTop 0x288
	#define METATILE_Autumn_TreeSideLeftBottom 0x290
	#define METATILE_Autumn_TreeSideRightTop 0x28B
	#define METATILE_Autumn_TreeSideRightBottom 0x293
	#define METATILE_Autumn_TreeSideLeftJoined 0x284
	#define METATILE_Autumn_TreeSideRightJoined 0x287

	{0xA, METATILE_Forest_Ground, Tileset_Forest}, //Override regular
	{0x2A8, METATILE_Forest_TreeTopLeft, Tileset_Forest},
	{0x2AA, METATILE_Forest_TreeTopRight, Tileset_Forest},
	{0x2AC, METATILE_Forest_TreeSideLeft, Tileset_Forest},
	{0x2AE, METATILE_Forest_TreeSideRight, Tileset_Forest},
	{0x2F8, METATILE_Forest_Ground, Tileset_Forest},
	{0x2F9, METATILE_Forest_Ground, Tileset_Forest},
	{0x2FA, METATILE_Forest_Ground, Tileset_Forest},
	{0x2FB, METATILE_Forest_Ground, Tileset_Forest},
	{0x2FC, METATILE_Forest_Ground, Tileset_Forest},
	{0x2FD, METATILE_Forest_Ground, Tileset_Forest},
	{0x300, METATILE_Forest_Ground, Tileset_Forest},
	{0x301, METATILE_Forest_Ground, Tileset_Forest},
	{0x302, METATILE_Forest_Ground, Tileset_Forest},
	{0x303, METATILE_Forest_Ground, Tileset_Forest},
	{0x304, METATILE_Forest_Ground, Tileset_Forest},
	{0x308, METATILE_Forest_Ground, Tileset_Forest},
	{0x309, METATILE_Forest_Ground, Tileset_Forest},
	{0x30A, METATILE_Forest_Ground, Tileset_Forest},
	{0x30B, METATILE_Forest_Ground, Tileset_Forest},
	{0x30C, METATILE_Forest_Ground, Tileset_Forest},
	{0x28F, METATILE_Forest_Pressure_Plate, Tileset_Forest},
	{0x310, METATILE_Forest_TreeTopLeft, Tileset_Forest},
	{0x312, METATILE_Forest_TreeTopLeft, Tileset_Forest},
	{0x314, METATILE_Forest_TreeTopRight, Tileset_Forest},
	{0x316, METATILE_Forest_TreeTopRight, Tileset_Forest},
	{0x328, METATILE_Forest_TreeSideRight, Tileset_Forest},
	{0x32A, METATILE_Forest_TreeSideLeft, Tileset_Forest},
	{0x32C, METATILE_Forest_TreeTopCornerLeft, Tileset_Forest},
	{0x32E, METATILE_Forest_TreeTopCornerRight, Tileset_Forest},
	{0x330, METATILE_Forest_TreeSideRight, Tileset_Forest},
	{0x332, METATILE_Forest_TreeSideLeft, Tileset_Forest},
	{0x338, METATILE_Forest_TreeSideRight, Tileset_Forest},
	{0x33A, METATILE_Forest_TreeSideLeft, Tileset_Forest},

	{0x6, METATILE_General_TreeSideRight, Tileset_General},
	{0x7, METATILE_General_TreeSideLeft, Tileset_General},
	{0xA, METATILE_General_PlainGrass, Tileset_General},
	{0xB, METATILE_General_TreeTopLeft, Tileset_General},
	{0xC, METATILE_General_TreeTopRight, Tileset_General},
	{0xD, METATILE_General_PlainGrass, Tileset_General},
	{0x12, METATILE_General_ThinTreeTop, Tileset_General},
	{0x16, METATILE_General_TreeSideRight, Tileset_General},
	{0x17, METATILE_General_TreeSideLeft, Tileset_General},
	{0x24, METATILE_General_TreeSideRight, Tileset_General},
	{0x25, METATILE_General_TreeSideLeft, Tileset_General},
	{0x137, METATILE_General_PlainGrass, Tileset_General},
	{0x1EE, METATILE_General_PlainGrass, Tileset_General},
	{0x1EF, METATILE_General_PlainGrass, Tileset_General},
	{0x1F3, METATILE_General_TreeSideRight, Tileset_General},
	{0x1F4, METATILE_General_TreeSideLeft, Tileset_General},
	{0x1F5, METATILE_General_TreeTopLeft, Tileset_General},
	{0x1F6, METATILE_General_TreeTopRight, Tileset_General},
	{0x1F7, METATILE_General_PlainGrass, Tileset_General},
	{0x1FF, METATILE_General_PlainGrass, Tileset_General},
	{0x205, METATILE_General_PlainGrass, Tileset_General},
	{0x206, METATILE_General_PlainGrass, Tileset_General},
	{0x20F, METATILE_General_TreeSideRight, Tileset_General},
	{0x210, METATILE_General_PlainGrass, Tileset_General},
	{0x211, METATILE_General_PlainGrass, Tileset_General},
	{0x217, METATILE_General_TreeSideLeft, Tileset_General},
	{0x218, METATILE_General_PlainGrass, Tileset_General},
	{0x219, METATILE_General_PlainGrass, Tileset_General},
	{0x220, METATILE_General_LandEdgeMiddle, Tileset_General},
	{0x221, METATILE_General_LandEdgeMiddle, Tileset_General},
	{0x222, METATILE_General_LandEdgeMiddle, Tileset_General},
	{0x223, METATILE_General_LandEdgeMiddle, Tileset_General},
	{0x227, METATILE_General_PlainGrass, Tileset_General},
	{0x228, METATILE_General_PlainGrass, Tileset_General},
	{0x229, METATILE_General_LandEdgeMiddle, Tileset_General},
	{0x22A, METATILE_General_PlainGrass, Tileset_General},
	{0x22B, METATILE_General_LandEdgeMiddle, Tileset_General},
	{0x350, METATILE_General_LandEdgeLeft, Tileset_DrescoTown},
	{0x351, METATILE_General_LandEdgeRight, Tileset_DrescoTown},
	{0x358, METATILE_General_TreeSideRight, Tileset_DrescoTown},
	{0x359, METATILE_General_TreeSideLeft, Tileset_DrescoTown},
	{0x370, METATILE_Route2_HighLandEdgeMiddle, Tileset_DrescoTown},
	{0x371, METATILE_Route2_HighLandEdgeMiddle, Tileset_DrescoTown},
	{0x2BE, METATILE_General_TreeTopLeft, Tileset_FlowerParadise},
	{0x2BF, METATILE_General_TreeTopRight, Tileset_FlowerParadise},
	{0x2C7, METATILE_General_TreeTopRight, Tileset_FlowerParadise},
	{0x2CF, METATILE_General_TreeSideRight, Tileset_FlowerParadise},
	{0x2D5, METATILE_General_TreeSideLeft, Tileset_FlowerParadise},
	{0x2F7, METATILE_General_TreeSideRight, Tileset_FlowerParadise},
	{0x2AF, METATILE_General_LandEdgeLeft, Tileset_CraterTown},
	{0x2B7, METATILE_General_TreeSideLeft, Tileset_CraterTown},
	{0x319, METATILE_General_TreeTopLeft, Tileset_CraterTown},
	{0x31A, METATILE_General_TreeTopRight, Tileset_CraterTown},
	{0x329, METATILE_General_TreeSideRight, Tileset_CraterTown},
	{0x32A, METATILE_General_TreeSideLeft, Tileset_CraterTown},
	{0x331, METATILE_General_TreeTopLeft, Tileset_CraterTown},
	{0x332, METATILE_General_TreeTopRight, Tileset_CraterTown},
	{0x348, METATILE_General_TreeTopLeftOverTreeSideRight, Tileset_CraterTown},
	{0x34A, METATILE_General_TreeTopRightOverTreeSideLeft, Tileset_CraterTown},

	{0x2C0, METATILE_Autumn_Ground, Tileset_Autumn},
	{0x2C1, METATILE_Autumn_Ground, Tileset_Autumn},
	{0x2C2, METATILE_Autumn_Ground, Tileset_Autumn},
	{0x2C3, METATILE_Autumn_Ground, Tileset_Autumn},
	{0x2C4, METATILE_Autumn_Ground, Tileset_Autumn},
	{0x2C8, METATILE_Autumn_Ground, Tileset_Autumn},
	{0x2C9, METATILE_Autumn_Ground, Tileset_Autumn},
	{0x2CA, METATILE_Autumn_Ground, Tileset_Autumn},
	{0x2CB, METATILE_Autumn_Ground, Tileset_Autumn},
	{0x2CC, METATILE_Autumn_Ground, Tileset_Autumn},
	{0x2CD, METATILE_Autumn_Ground, Tileset_Autumn},
	{0x2CE, METATILE_Autumn_Ground, Tileset_Autumn},
	{0x2D0, METATILE_Autumn_Ground, Tileset_Autumn},
	{0x2D1, METATILE_Autumn_Ground, Tileset_Autumn},
	{0x2D2, METATILE_Autumn_Ground, Tileset_Autumn},
	{0x2D3, METATILE_Autumn_Ground, Tileset_Autumn},
	{0x310, METATILE_Autumn_TreeSideRightTop, Tileset_Autumn},
	{0x311, METATILE_Autumn_TreeSideLeftTop, Tileset_Autumn},
	{0x318, METATILE_Autumn_TreeSideRightBottom, Tileset_Autumn},
	{0x319, METATILE_Autumn_TreeSideLeftBottom, Tileset_Autumn},
	{0x320, METATILE_Autumn_TreeSideRightTop, Tileset_Autumn},
	{0x321, METATILE_Autumn_TreeSideLeftTop, Tileset_Autumn},
	{0x328, METATILE_Autumn_TreeSideRightJoined, Tileset_Autumn},
	{0x329, METATILE_Autumn_TreeSideLeftJoined, Tileset_Autumn},
	{0x32A, METATILE_Autumn_TreeTopRight, Tileset_Autumn},
	{0x32B, METATILE_Autumn_TreeTopLeft, Tileset_Autumn},
	{0x330, METATILE_Autumn_TreeSideRightBottom, Tileset_Autumn},
	{0x331, METATILE_Autumn_TreeSideLeftBottom, Tileset_Autumn},
	{0x332, METATILE_Autumn_TreeTopMiddleLeft, Tileset_Autumn},
	{0x333, METATILE_Autumn_TreeTopMiddleLeftJoined, Tileset_Autumn},
	{0x338, METATILE_Autumn_TreeTopLeftCorner, Tileset_Autumn},
	{0x339, METATILE_Autumn_TreeTopMiddleRight, Tileset_Autumn},
	{0x33A, METATILE_Autumn_TreeTopMiddleLeftJoined, Tileset_Autumn},
	{0x33B, METATILE_Autumn_TreeTopRightCorner, Tileset_Autumn},
	{0x344, METATILE_Autumn_TreeTopLeft, Tileset_Autumn},
	{0x348, METATILE_Autumn_TreeSideRightJoined, Tileset_Autumn},
	{0x349, METATILE_Autumn_TreeSideLeftJoined, Tileset_Autumn},
	{0x350, METATILE_Autumn_TreeSideRightJoined, Tileset_Autumn},
	{0x351, METATILE_Autumn_TreeSideLeftTop, Tileset_Autumn},
	{0x3D4, METATILE_Autumn_TreeSideRightTop, Tileset_Autumn},
	{0x3D6, METATILE_Autumn_TreeTopRight, Tileset_Autumn},
	{0x3F8, METATILE_Autumn_TreeSideRightTop, Tileset_Autumn},
	{0x3F9, METATILE_Autumn_TreeTopMiddleLeftJoined, Tileset_Autumn},
	{0x3FA, METATILE_Autumn_TreeTopRight, Tileset_Autumn},

	{0x366, METATILE_General_TreeTopRight, Tileset_FallshoreCity},
	{0x367, METATILE_General_TreeTopRight, Tileset_FallshoreCity},
	{0x376, METATILE_General_TreeSideRight, Tileset_FallshoreCity},
	{0x377, METATILE_General_TreeSideLeft, Tileset_FallshoreCity},
	{0x3B4, METATILE_General_TreeTopLeft, Tileset_DeharaCity},
	{0x3B5, METATILE_General_TreeTopRight, Tileset_DeharaCity},
	{0x3CE, METATILE_General_TreeTopLeft, Tileset_DeharaCity},
	{0x3CF, METATILE_General_TreeTopRight, Tileset_DeharaCity},
	{0x3D3, METATILE_General_TreeTopLeft, Tileset_DeharaCity},
	{0x3D4, METATILE_General_TreeTopRight, Tileset_DeharaCity},
	{0x3DD, METATILE_General_LandEdgeMiddle, Tileset_DeharaCity},
	{0x3DE, METATILE_General_TreeTopLeft, Tileset_DeharaCity},
	{0x3DF, METATILE_General_TreeTopRight, Tileset_DeharaCity},
	{0x318, METATILE_General_LandEdgeLeft, Tileset_GurunTown},
	{0x319, METATILE_General_LandEdgeLeft, Tileset_GurunTown},
	{0x31A, METATILE_General_LandEdgeLeft, Tileset_GurunTown},
	{0x322, METATILE_General_BothTreeSides, Tileset_GurunTown},
	{0x346, METATILE_General_LandEdgeRight, Tileset_GurunTown},
	{0x2F6, METATILE_Route17_LandEdgeBottomRight, Tileset_Route17},
	{0x2F8, METATILE_Route17_LandEdgeBottomMiddle, Tileset_Route17},
	{0x2F9, METATILE_Route17_LandEdgeBottomMiddle, Tileset_Route17},
	{0x2FA, METATILE_Route17_LandEdgeBottomMiddle, Tileset_Route17},
	{0x2FB, METATILE_Route17_LandEdgeBottomLeft, Tileset_Route17},
	{0x2FC, METATILE_Route17_LandEdgeBottomLeft, Tileset_Route17},
	{0x2FD, METATILE_Route17_LandEdgeBottomLeft, Tileset_Route17},
	{0x2FE, METATILE_Route17_LandEdgeBottomRight, Tileset_Route17},
	{0x2FF, METATILE_Route17_LandEdgeBottomRight, Tileset_Route17},
	{0x345, METATILE_Route17_LandEdgeBottomRight, Tileset_Route17},
	{0x30D, METATILE_General_TreeTopRight, Tileset_MagnoliaTown},

	{0x1, 0x0, Tileset_Snow},
	{0x4, 0x2, Tileset_Snow},
	{0x5, 0x3, Tileset_Snow},
	{0x7, 0x6, Tileset_Snow},
	{0xD5, 0x47, Tileset_Snow},
	{0xD6, 0x47, Tileset_Snow},
	{0xDD, 0x57, Tileset_Snow},
	{0xDE, 0x57, Tileset_Snow},
	{0xE4, 0x3F, Tileset_Snow},
	{0xEC, 0x3D, Tileset_Snow},
	{0x180, 0x178, Tileset_Snow},
	{0x181, 0x179, Tileset_Snow},
	{0x1B0, 0x0, Tileset_Snow},
	{0x1B1, 0x0, Tileset_Snow},
	{0x1B2, 0x0, Tileset_Snow},
	{0x1B3, 0x0, Tileset_Snow},
	{0x1B4, 0x0, Tileset_Snow},
	{0x1B5, 0x0, Tileset_Snow},
	{0x1B6, 0x0, Tileset_Snow},
	{0x1B8, 0x0, Tileset_Snow},
	{0x1B9, 0x0, Tileset_Snow},
	{0x1BA, 0x0, Tileset_Snow},
	{0x1BB, 0x0, Tileset_Snow},
	{0x1BE, 0x2, Tileset_Snow},
	{0x1BF, 0x3, Tileset_Snow},
	{0x1C0, 0x0, Tileset_Snow},
	{0x1C1, 0x0, Tileset_Snow},
	{0x1C2, 0x0, Tileset_Snow},
	{0x1C3, 0x0, Tileset_Snow},
	{0x1C4, 0x2, Tileset_Snow},
	{0x1C5, 0x3, Tileset_Snow},
	{0x1C6, 0x2, Tileset_Snow},
	{0x1C7, 0x3, Tileset_Snow},
	{0x1C8, 0x2, Tileset_Snow},
	{0x1C9, 0x3, Tileset_Snow},
	{0x1CA, 0x2, Tileset_Snow},
	{0x1CB, 0x3, Tileset_Snow},
	{0x1CC, 0x2, Tileset_Snow},
	{0x1CD, 0x3, Tileset_Snow},
	{0x1CE, 0x2, Tileset_Snow},
	{0x1CF, 0x3, Tileset_Snow},
	{0x1D0, 0x179, Tileset_Snow},
	{0x1D1, 0x178, Tileset_Snow},
	{0x1D8, 0x179, Tileset_Snow},
	{0x1D9, 0x178, Tileset_Snow},
	{0x1E0, 0x179, Tileset_Snow},
	{0x1E1, 0x178, Tileset_Snow},
	{0x1E8, 0x179, Tileset_Snow},
	{0x1E9, 0x178, Tileset_Snow},
	{0x1F0, 0x179, Tileset_Snow},
	{0x1F1, 0x178, Tileset_Snow},
	{0x1F8, 0x179, Tileset_Snow},
	{0x1F9, 0x178, Tileset_Snow},
	CUT_GRASS_TERMIN
};
#endif

enum
{
	TRAINER_PARAM_LOAD_VAL_8BIT,
	TRAINER_PARAM_LOAD_VAL_16BIT,
	TRAINER_PARAM_LOAD_VAL_32BIT,
	TRAINER_PARAM_CLEAR_VAL_8BIT,
	TRAINER_PARAM_CLEAR_VAL_16BIT,
	TRAINER_PARAM_CLEAR_VAL_32BIT,
	TRAINER_PARAM_LOAD_SCRIPT_RET_ADDR,
};

//An alternative to Oak's tutorial that allows mugshots to be used if FLAG_ACTIVATE_TUTORIAL is commented out
static const struct TrainerBattleParameter sContinueLostBattleParams[] =
{
	{&sTrainerBattleMode, 				TRAINER_PARAM_LOAD_VAL_8BIT},
	{&gTrainerBattleOpponent_A,	  		TRAINER_PARAM_LOAD_VAL_16BIT},
	{&sTrainerEventObjectLocalId,		TRAINER_PARAM_LOAD_VAL_16BIT},
	{&sTrainerIntroSpeech_A,			TRAINER_PARAM_CLEAR_VAL_32BIT},
	{&sTrainerDefeatSpeech_A,			TRAINER_PARAM_LOAD_VAL_32BIT},
	{&sTrainerVictorySpeech,			TRAINER_PARAM_LOAD_VAL_32BIT},
	{&sTrainerCannotBattleSpeech,		TRAINER_PARAM_CLEAR_VAL_32BIT},
	{&sTrainerBattleScriptRetAddr,  	TRAINER_PARAM_CLEAR_VAL_32BIT},
	{&sTrainerBattleEndScript,	   		TRAINER_PARAM_LOAD_SCRIPT_RET_ADDR},
};

static const struct TrainerBattleParameter sTrainerBContinueScriptBattleParams[] =
{
	{&sTrainerBattleMode,				TRAINER_PARAM_LOAD_VAL_8BIT},
	{&gTrainerBattleOpponent_B,	  		TRAINER_PARAM_LOAD_VAL_16BIT},
	{&sTrainerEventObjectLocalId,		TRAINER_PARAM_LOAD_VAL_16BIT},
	{&sTrainerIntroSpeech_B,			TRAINER_PARAM_LOAD_VAL_32BIT},
	{&sTrainerDefeatSpeech_B,			TRAINER_PARAM_LOAD_VAL_32BIT},
	{&sTrainerVictorySpeech_B,			TRAINER_PARAM_CLEAR_VAL_32BIT},
	{&sTrainerCannotBattleSpeech,		TRAINER_PARAM_CLEAR_VAL_32BIT},
	{&sTrainerBattleScriptRetAddr_B,	TRAINER_PARAM_LOAD_VAL_32BIT},
	{&sTrainerBattleEndScript,			TRAINER_PARAM_LOAD_SCRIPT_RET_ADDR},
};

static const struct TrainerBattleParameter sTrainerBOrdinaryBattleParams[] =
{
	{&sTrainerBattleMode,				TRAINER_PARAM_LOAD_VAL_8BIT},
	{&gTrainerBattleOpponent_B,	 		TRAINER_PARAM_LOAD_VAL_16BIT},
	{&sTrainerEventObjectLocalId,		TRAINER_PARAM_LOAD_VAL_16BIT},
	{&sTrainerIntroSpeech_B,		 	TRAINER_PARAM_LOAD_VAL_32BIT},
	{&sTrainerDefeatSpeech_B,			TRAINER_PARAM_LOAD_VAL_32BIT},
	{&sTrainerVictorySpeech,		 	TRAINER_PARAM_CLEAR_VAL_32BIT},
	{&sTrainerVictorySpeech_B,			TRAINER_PARAM_CLEAR_VAL_32BIT},
	{&sTrainerCannotBattleSpeech,		TRAINER_PARAM_CLEAR_VAL_32BIT},
	{&sTrainerBattleScriptRetAddr_B,	TRAINER_PARAM_CLEAR_VAL_32BIT},
	{&sTrainerBattleEndScript,			TRAINER_PARAM_LOAD_SCRIPT_RET_ADDR},
};

//trainerbattle 0xA FOE_1_ID FOE_2_ID PARTNER_ID PARTNER_BACKSPRITE_ID 0x0 DEFEAT_TEXT_A DEFEAT_TEXT_B
static const struct TrainerBattleParameter sMultiBattleParams[] =
{
	{&sTrainerBattleMode,				TRAINER_PARAM_LOAD_VAL_8BIT},

	{&gTrainerBattleOpponent_A,			TRAINER_PARAM_LOAD_VAL_16BIT},
	{&gTrainerBattleOpponent_B,	  		TRAINER_PARAM_LOAD_VAL_16BIT},
	{&gTrainerBattlePartner,	 	 	TRAINER_PARAM_LOAD_VAL_16BIT},
	{&sPartnerBackSpriteId,			 	TRAINER_PARAM_LOAD_VAL_16BIT},

	{&sTrainerEventObjectLocalId,		TRAINER_PARAM_LOAD_VAL_16BIT},
	{&sTrainerIntroSpeech_A,		 	TRAINER_PARAM_CLEAR_VAL_32BIT},
	{&sTrainerIntroSpeech_B,	   	 	TRAINER_PARAM_CLEAR_VAL_32BIT},

	{&sTrainerDefeatSpeech_A,			TRAINER_PARAM_LOAD_VAL_32BIT},
	{&sTrainerDefeatSpeech_B, 		 	TRAINER_PARAM_LOAD_VAL_32BIT},

	{&sTrainerVictorySpeech,		 	TRAINER_PARAM_CLEAR_VAL_32BIT},
	{&sTrainerVictorySpeech_B,	   		TRAINER_PARAM_CLEAR_VAL_32BIT},
	{&sTrainerCannotBattleSpeech,		TRAINER_PARAM_CLEAR_VAL_32BIT},
	{&sTrainerBattleScriptRetAddr,   	TRAINER_PARAM_CLEAR_VAL_32BIT},
	{&sTrainerBattleScriptRetAddr_B, 	TRAINER_PARAM_CLEAR_VAL_32BIT},
	{&sTrainerBattleEndScript,	   		TRAINER_PARAM_LOAD_SCRIPT_RET_ADDR},
};

//trainerbattle 0xB FOE_1_ID FOE_2_ID FOE_1_NPC_ID FOE_2_NPC_ID 0x0 INTRO_TEXT_A INTRO_TEXT_B DEFEAT_TEXT_A DEFEAT_TEXT_B CANNOT_BATTLE_TEXT
static const struct TrainerBattleParameter sTwoOpponentBattleParams[] =
{
	{&sTrainerBattleMode,		  	 	TRAINER_PARAM_LOAD_VAL_8BIT},

	{&gTrainerBattleOpponent_A,		 	TRAINER_PARAM_LOAD_VAL_16BIT},
	{&gTrainerBattleOpponent_B,		 	TRAINER_PARAM_LOAD_VAL_16BIT},
	{&gTrainerBattlePartner,	 	 	TRAINER_PARAM_CLEAR_VAL_16BIT},

	{&gFirstTrainerNPCId,			 	TRAINER_PARAM_LOAD_VAL_8BIT},
	{&gSecondTrainerNPCId,			 	TRAINER_PARAM_LOAD_VAL_8BIT},

	{&sTrainerEventObjectLocalId,  	 	TRAINER_PARAM_LOAD_VAL_16BIT},
	{&sTrainerIntroSpeech_A,	   	 	TRAINER_PARAM_LOAD_VAL_32BIT},
	{&sTrainerIntroSpeech_B,	   	 	TRAINER_PARAM_LOAD_VAL_32BIT},

	{&sTrainerDefeatSpeech_A,	 	 	TRAINER_PARAM_LOAD_VAL_32BIT},
	{&sTrainerDefeatSpeech_B,	  	 	TRAINER_PARAM_LOAD_VAL_32BIT},

	{&sTrainerVictorySpeech,	   	 	TRAINER_PARAM_CLEAR_VAL_32BIT},
	{&sTrainerVictorySpeech_B,	   		TRAINER_PARAM_CLEAR_VAL_32BIT},
	{&sTrainerCannotBattleSpeech,		TRAINER_PARAM_LOAD_VAL_32BIT},
	{&sTrainerCannotBattleSpeech_B,  	TRAINER_PARAM_LOAD_VAL_32BIT},

	{&sTrainerBattleScriptRetAddr,   	TRAINER_PARAM_CLEAR_VAL_32BIT},
	{&sTrainerBattleScriptRetAddr_B, 	TRAINER_PARAM_CLEAR_VAL_32BIT},
	{&sTrainerBattleEndScript,	   		TRAINER_PARAM_LOAD_SCRIPT_RET_ADDR},
};

//trainerbattle 0xC FOE_ID PARTNER_ID PARTNER_BACKSPRITE_ID 0x0 DEFEAT_TEXT_A
static const struct TrainerBattleParameter sTagBattleParams[] =
{
	{&sTrainerBattleMode,		   		TRAINER_PARAM_LOAD_VAL_8BIT},

	{&gTrainerBattleOpponent_A,	  		TRAINER_PARAM_LOAD_VAL_16BIT},
	{&gTrainerBattleOpponent_B,	  		TRAINER_PARAM_CLEAR_VAL_16BIT},
	{&gTrainerBattlePartner,	 	 	TRAINER_PARAM_LOAD_VAL_16BIT},
	{&sPartnerBackSpriteId,			 	TRAINER_PARAM_LOAD_VAL_16BIT},

	{&sTrainerEventObjectLocalId,		TRAINER_PARAM_LOAD_VAL_16BIT},
	{&sTrainerIntroSpeech_A,		 	TRAINER_PARAM_CLEAR_VAL_32BIT},
	{&sTrainerIntroSpeech_B,	   	 	TRAINER_PARAM_CLEAR_VAL_32BIT},

	{&sTrainerDefeatSpeech_A,			TRAINER_PARAM_LOAD_VAL_32BIT},
	{&sTrainerDefeatSpeech_B, 		 	TRAINER_PARAM_CLEAR_VAL_32BIT},

	{&sTrainerVictorySpeech,		 	TRAINER_PARAM_CLEAR_VAL_32BIT},
	{&sTrainerVictorySpeech_B,	   		TRAINER_PARAM_CLEAR_VAL_32BIT},
	{&sTrainerCannotBattleSpeech,		TRAINER_PARAM_CLEAR_VAL_32BIT},
	{&sTrainerBattleScriptRetAddr,   	TRAINER_PARAM_CLEAR_VAL_32BIT},
	{&sTrainerBattleScriptRetAddr_B, 	TRAINER_PARAM_CLEAR_VAL_32BIT},
	{&sTrainerBattleEndScript,	   		TRAINER_PARAM_LOAD_SCRIPT_RET_ADDR},
};

u8 CheckForTrainersWantingBattle(void)
{
	u8 viableMons = 0xFF;

	if (FuncIsActiveTask(Task_OverworldMultiTrainers))
		return FALSE;

	ExtensionState.spotted.count = 0;

	for (u8 eventObjId = 0; eventObjId < MAP_OBJECTS_COUNT; ++eventObjId) //For each NPC on the map
	{
		if (!gEventObjects[eventObjId].active
		||  gEventObjects[eventObjId].isPlayer
		||  gEventObjects[eventObjId].trainerType == 0)
			continue;

		if (CheckTrainerSpotting(eventObjId))
		{
			if (viableMons == 0xFF) //No data loaded yet
				viableMons = ViableMonCount(gPlayerParty); //Load data now to not waste unnecessary time

			if (viableMons == 0)
				return FALSE; //NPC's won't challenge you if, for some reason, you have no Pokemon

			if (viableMons < 2 && !FlagGet(FLAG_TAG_BATTLE)) //The tag partner gives you the second mon
				break;

			// We've found enough opponents
			if (ExtensionState.spotted.count > 1)
				break;
		}
		else if (CheckNPCSpotting(eventObjId))
		{
			u16 flag = GetEventObjectTemplateByLocalIdAndMap(gEventObjects[eventObjId].localId,
															 gEventObjects[eventObjId].mapNum,
															 gEventObjects[eventObjId].mapGroup)->flagId2;
			if (flag != 0)
				FlagSet(flag); //Set this flag so the spotting only happens once

			u8 direction = GetNPCDirectionFaceToPlayer(eventObjId);
			FaceDirection(&gEventObjects[eventObjId], &gSprites[gEventObjects[eventObjId].spriteId], direction);
			gSelectedEventObject = eventObjId;
			TrainerApproachPlayer(&gEventObjects[eventObjId], GetTrainerApproachDistance(&gEventObjects[eventObjId]) - 1);
			ScriptContext1_SetupScript(GetEventObjectScriptPointerByEventObjectId(eventObjId));
			ScriptCall(&gScriptEnv1, EventScript_SetUpNPCSpotting);
			return TRUE;
		}
	}

	//These battle types have built in features
	if (ExtensionState.spotted.count > 0)
	{
		if (ExtensionState.spotted.trainers[0].script[1] == TRAINER_BATTLE_TWO_OPPONENTS
		||  ExtensionState.spotted.trainers[1].script[1] == TRAINER_BATTLE_TWO_OPPONENTS)
			ExtensionState.spotted.count = 1;
	}

	switch (ExtensionState.spotted.count) {
		case 1: ;
			struct TrainerSpotted* trainer = &ExtensionState.spotted.trainers[0];

			TrainerWantsBattle(trainer->id, trainer->script);
			TrainerApproachPlayer(&gEventObjects[trainer->id], trainer->distance - 1);

			if (trainer->script[1] == TRAINER_BATTLE_TWO_OPPONENTS)
				ScriptContext1_SetupScript(EventScript_DoTwoOpponentBattle);

			return TRUE;

		case 2:
			CreateTask(Task_OverworldMultiTrainers, 0x50);
			return TRUE;
	}

	return FALSE;
}

static bool8 CheckTrainerSpotting(u8 eventObjId) //Or just CheckTrainer
{
	const u8* scriptPtr = GetEventObjectScriptPointerByEventObjectId(eventObjId); //Get NPC Script Pointer from its Object Id
	u8 battleType = scriptPtr[1];

	if (battleType == TRAINER_BATTLE_TWO_OPPONENTS
	&& (FlagGet(FLAG_TRAINER_FLAG_START + T1_READ_16(scriptPtr + 2)) /*|| FlagGet(FLAG_TRAINER_FLAG_START + T1_READ_16(scriptPtr + 4))*/))
		return FALSE; //If either trainer flag is set

	if (GetTrainerFlagFromScriptPointer(scriptPtr)) //Trainer has already been beaten
		return FALSE;

	struct EventObject* trainerObj = &gEventObjects[eventObjId];
	u8 approachDistance = GetTrainerApproachDistance(trainerObj);

	if (approachDistance > 0)
	{
		switch (battleType) {
			case TRAINER_BATTLE_DOUBLE:
			case TRAINER_BATTLE_REMATCH_DOUBLE:
			case TRAINER_BATTLE_CONTINUE_SCRIPT_DOUBLE:
			case TRAINER_BATTLE_TWO_OPPONENTS:
			case TRAINER_BATTLE_REMATCH_TWO_OPPONENTS:
			case TRAINER_BATTLE_DOUBLE_SCALED:
				if (ViableMonCount(gPlayerParty) < 2 && !FlagGet(FLAG_TAG_BATTLE))
					return FALSE;
				break;
			case TRAINER_BATTLE_TAG:
			case TRAINER_BATTLE_MULTI:
				return FALSE;  //You can't be stopped by someone using the tag battle feature
		}

		struct TrainerSpotted trainer = {eventObjId, approachDistance, (u8*) scriptPtr};
		ExtensionState.spotted.trainers[ExtensionState.spotted.count++] = trainer;
		return TRUE;
	}

	return FALSE;
}

static bool8 GetTrainerFlagFromScriptPointer(const u8* data)
{
	if (TrainerBattleLoadArg8(data) != SCRCMD_TRAINERBATTLE) //Prevents game from crashing if you are spotted by someone
		return TRUE;							 			 //who's not a trainer

	u16 flag = TrainerBattleLoadArg16(data + 2);
	return FlagGet(FLAG_TRAINER_FLAG_START + flag);
}

static u8 CheckNPCSpotting(u8 eventObjId)
{
	#ifdef NON_TRAINER_SPOTTING
	const u8* scriptPtr = GetEventObjectScriptPointerByEventObjectId(eventObjId); //Get NPC Script Pointer from its Object Id
	u16 flag = GetEventObjectTemplateByLocalIdAndMap(gEventObjects[eventObjId].localId,
													 gEventObjects[eventObjId].mapNum,
													 gEventObjects[eventObjId].mapGroup)->flagId2; //Special flag just for this

	if (scriptPtr != NULL && scriptPtr[0] != SCRCMD_TRAINERBATTLE //NPC has a regular script
	&& (flag == 0 || !FlagGet(flag)))
	{
		return GetTrainerApproachDistance(&gEventObjects[eventObjId]);
	}
	#else
		++eventObjId;
	#endif

	return 0;
}

static void Task_OverworldMultiTrainers(u8 id)
{
	struct Task* task = &gTasks[id];

	switch (task->data[0]) {
		case 0:
			gApproachingTrainerId = 0; //Reset Value
			for (int i = 0; i < 2; ++i, ++gApproachingTrainerId)
				ConfigureTwoTrainersBattle(ExtensionState.spotted.trainers[i].id, ExtensionState.spotted.trainers[i].script);

			task->data[0]++;
			break;

		case 1:
			ExtensionState.multiTaskStateHelper = 0;
			gApproachingTrainerId = 0;
			gSelectedEventObject = ExtensionState.spotted.trainers[0].id;
			SetUpTwoTrainersBattle();
			TrainerApproachPlayer(&gEventObjects[ExtensionState.spotted.trainers[0].id], ExtensionState.spotted.trainers[0].distance - 1);
			task->data[0]++;
			break;

		case 2:
			if (ExtensionState.multiTaskStateHelper)
				task->data[0]++;
			break;

		case 3:
			ExtensionState.multiTaskStateHelper = 0;
			gApproachingTrainerId = 1;
			SetUpTwoTrainersBattle();
			gSelectedEventObject = ExtensionState.spotted.trainers[1].id;
			TrainerApproachPlayer(&gEventObjects[ExtensionState.spotted.trainers[1].id], ExtensionState.spotted.trainers[1].distance - 1);
			task->data[0]++;
			break;

		case 4:
			if (ExtensionState.multiTaskStateHelper)
				task->data[0]++;
			break;

		case 5:
			/*
			 * Jump halfway into the battle init script to actually start
			 * the battle.
			 */
			ExtensionState.multiTaskStateHelper = 0;
			gApproachingTrainerId = 2; //For the battle type flag set
			ScriptContext1_SetupScript((u8*) 0x081A4FCC);
			ScriptContext2_Enable();
			DestroyTask(id);
			break;
	}
}

//EventScript_TryDoNormalTrainerBattle
static void ConfigureTwoTrainersBattle(u8 trainerEventObjId, const u8* trainerScript)
{
	gSelectedEventObject = trainerEventObjId;
	gSpecialVar_LastTalked = gEventObjects[trainerEventObjId].localId;
	BattleSetup_ConfigureTrainerBattle(trainerScript + 1);
}

static void SetUpTwoTrainersBattle(void)
{
	ScriptContext1_SetupScript(Script_TrainerSpottedMulti);
	ScriptContext2_Enable();
	DismissMapNamePopup();
}

const u8* BattleSetup_ConfigureTrainerBattle(const u8* data)
{
	InitTrainerBattleVariables(); //0x8080110
	sTrainerBattleMode = TrainerBattleLoadArg8(data);

	switch (sTrainerBattleMode) {
		case TRAINER_BATTLE_CONTINUE_SCRIPT_NO_MUSIC: //Gym Leaders
			TrainerBattleLoadArgs(sContinueScriptBattleParams, data);
			SetMapVarsToTrainer();
			return EventScript_TryDoNormalTrainerBattle;

		case TRAINER_BATTLE_CONTINUE_SCRIPT:
			if (gApproachingTrainerId == 0)
			{
				TrainerBattleLoadArgs(sContinueScriptBattleParams, data); //0x8080168
				SetMapVarsToTrainer(); //0x80801F0
			}
			else
			{
				TrainerBattleLoadArgs(sTrainerBContinueScriptBattleParams, data);
				VarSet(VAR_SECOND_OPPONENT, gTrainerBattleOpponent_B);
			}
			return EventScript_TryDoNormalTrainerBattle;

		case TRAINER_BATTLE_SINGLE_NO_INTRO_TEXT:
		case TRAINER_BATTLE_SINGLE_NO_INTRO_TEXT_SCALED:
			TrainerBattleLoadArgs(sOrdinaryNoIntroBattleParams, data);
			gTrainerBattleOpponent_A = VarGet(gTrainerBattleOpponent_A); //Allow dynamic loading
			if (FlagGet(FLAG_TWO_OPPONENTS))
				gTrainerBattleOpponent_B = VarGet(VAR_SECOND_OPPONENT);
			return EventScript_DoTrainerBattle;

		case TRAINER_BATTLE_DOUBLE:
		case TRAINER_BATTLE_DOUBLE_SCALED:
			TrainerBattleLoadArgs(sDoubleBattleParams, data);
			SetMapVarsToTrainer();
			return EventScript_TryDoDoubleTrainerBattle;

		case TRAINER_BATTLE_REMATCH:
			//QuestLogRemtachBattleStore();
			TrainerBattleLoadArgs(sOrdinaryBattleParams, data);
			SetMapVarsToTrainer();
			#ifndef UNBOUND
			gTrainerBattleOpponent_A = GetRematchTrainerId(gTrainerBattleOpponent_A);
			#endif
			return EventScript_TryDoRematchBattle;

		case TRAINER_BATTLE_CONTINUE_SCRIPT_DOUBLE:
		case TRAINER_BATTLE_CONTINUE_SCRIPT_DOUBLE_NO_MUSIC: //Gym Leader Doubles
			TrainerBattleLoadArgs(sContinueScriptDoubleBattleParams, data);
			SetMapVarsToTrainer();
			return EventScript_TryDoDoubleTrainerBattle;

		case TRAINER_BATTLE_REMATCH_DOUBLE:
			//QuestLogRemtachBattleStore();
			TrainerBattleLoadArgs(sDoubleBattleParams, data);
			SetMapVarsToTrainer();
			#ifndef UNBOUND
			gTrainerBattleOpponent_A = GetRematchTrainerId(gTrainerBattleOpponent_A);
			#endif
			return EventScript_TryDoDoubleRematchBattle;

		case TRAINER_BATTLE_OAK_TUTORIAL:
			#ifdef TUTORIAL_BATTLES
			if (Var8000 != 0xFEFE)
				TrainerBattleLoadArgs(sOakTutorialParams, data);
			else //Regular trainer battle 9
			#endif
			{
				TrainerBattleLoadArgs(sContinueLostBattleParams, data);
				gTrainerBattleOpponent_A = VarGet(gTrainerBattleOpponent_A); //Allow dynamic loading
				if (FlagGet(FLAG_TWO_OPPONENTS))
					gTrainerBattleOpponent_B = VarGet(VAR_SECOND_OPPONENT);
			}
			return EventScript_DoTrainerBattle;

		case TRAINER_BATTLE_MULTI:
			TrainerBattleLoadArgs(sMultiBattleParams, data);
			gTrainerBattleOpponent_A = VarGet(gTrainerBattleOpponent_A); //Allow dynamic loading
			gTrainerBattleOpponent_B = VarGet(gTrainerBattleOpponent_B); //Allow dynamic loading
			gTrainerBattlePartner = VarGet(gTrainerBattlePartner); //Allow dynamic loading
			VarSet(VAR_SECOND_OPPONENT, gTrainerBattleOpponent_B);
			VarSet(VAR_PARTNER, gTrainerBattlePartner);
			VarSet(VAR_PARTNER_BACKSPRITE, sPartnerBackSpriteId);
			FlagSet(FLAG_TAG_BATTLE);
			FlagSet(FLAG_TWO_OPPONENTS);
			return EventScript_DoTrainerBattle;

		case TRAINER_BATTLE_TWO_OPPONENTS:
			TrainerBattleLoadArgs(sTwoOpponentBattleParams, data);
			SetMapVarsToTrainer();
			VarSet(VAR_SECOND_OPPONENT, gTrainerBattleOpponent_B);
			FlagSet(FLAG_TWO_OPPONENTS);
			gApproachingTrainerId = 0;
			return EventScript_TryDoTwoOpponentBattle;

		case TRAINER_BATTLE_REMATCH_TWO_OPPONENTS:
			TrainerBattleLoadArgs(sTwoOpponentBattleParams, data);
			SetMapVarsToTrainer();
			#ifndef UNBOUND
			gTrainerBattleOpponent_A = GetRematchTrainerId(gTrainerBattleOpponent_A);
			gTrainerBattleOpponent_B = GetRematchTrainerId(gTrainerBattleOpponent_B);
			#endif
			VarSet(VAR_SECOND_OPPONENT, gTrainerBattleOpponent_B);
			FlagSet(FLAG_TWO_OPPONENTS);
			gApproachingTrainerId = 0;
			return EventScript_TryDoTwoOpponentRematchBattle;

		case TRAINER_BATTLE_TAG:
			TrainerBattleLoadArgs(sTagBattleParams, data);
			gTrainerBattleOpponent_A = VarGet(gTrainerBattleOpponent_A); //Allow dynamic loading
			gTrainerBattlePartner = VarGet(gTrainerBattlePartner); //Allow dynamic loading
			VarSet(VAR_PARTNER, gTrainerBattlePartner);
			VarSet(VAR_PARTNER_BACKSPRITE, sPartnerBackSpriteId);
			FlagSet(FLAG_TAG_BATTLE);
			return EventScript_DoTrainerBattle;

		case TRAINER_BATTLE_SINGLE_SCALED:
		default: //TRAINER_BATTLE_SINGLE
			if (gApproachingTrainerId == 0)
			{
				TrainerBattleLoadArgs(sOrdinaryBattleParams, data);
				SetMapVarsToTrainer();
			}
			else
			{
				TrainerBattleLoadArgs(sTrainerBOrdinaryBattleParams, data);
				VarSet(VAR_SECOND_OPPONENT, gTrainerBattleOpponent_B);
			}
			return EventScript_TryDoNormalTrainerBattle;
	}
}

bool8 IsTrainerBattleModeAgainstTwoOpponents(void)
{
	return sTrainerBattleMode == TRAINER_BATTLE_MULTI
		|| sTrainerBattleMode == TRAINER_BATTLE_TWO_OPPONENTS
		|| sTrainerBattleMode == TRAINER_BATTLE_REMATCH_TWO_OPPONENTS
		|| FlagGet(FLAG_TWO_OPPONENTS);
}

bool8 IsTrainerBattleModeWithPartner(void)
{
	return sTrainerBattleMode == TRAINER_BATTLE_MULTI || sTrainerBattleMode == TRAINER_BATTLE_TAG || FlagGet(FLAG_TAG_BATTLE);
}

static void InitTrainerBattleVariables(void)
{
	sTrainerBattleMode = 0;
	if (ExtensionState.spotted.count <= 1)
	{
		sTrainerIntroSpeech_A = 0;
		sTrainerDefeatSpeech_A = 0;
		sTrainerBattleScriptRetAddr = 0;
		sTrainerVictorySpeech = 0;
	}
	else if (!FlagGet(FLAG_TWO_OPPONENTS))
	{
		sTrainerIntroSpeech_B = 0;
		sTrainerDefeatSpeech_B = 0;
		sTrainerBattleScriptRetAddr_B = 0;
		sTrainerVictorySpeech_B = 0;
	}
	sTrainerEventObjectLocalId = 0;
	sTrainerCannotBattleSpeech = 0;
	sTrainerBattleEndScript = 0;
}

void ClearTwoOpponentFlag(void)
{
	FlagClear(FLAG_TWO_OPPONENTS);
}

void BattleSetup_StartTrainerBattle(void)
{
	if (FlagGet(FLAG_BATTLE_FACILITY))
	{
		gBattleTypeFlags = BATTLE_TYPE_TRAINER;
		switch (BATTLE_FACILITY_NUM) {
			case IN_BATTLE_SANDS:
				gBattleTypeFlags |= (BATTLE_TYPE_BATTLE_SANDS | BATTLE_TYPE_MOCK_BATTLE);
				break;
			//case IN_BATTLE_MINE:
			//	gBattleTypeFlags |= BATTLE_TYPE_BATTLE_MINE;
			//	break;
			case IN_BATTLE_CIRCUS:
				gBattleTypeFlags |= BATTLE_TYPE_BATTLE_CIRCUS;
				break;
			case IN_RING_CHALLENGE:
				gBattleTypeFlags |= BATTLE_TYPE_RING_CHALLENGE;
				//Fallthrough
			default:
				gBattleTypeFlags |= BATTLE_TYPE_BATTLE_TOWER;
				break;
		}

		u16 tier = VarGet(VAR_BATTLE_FACILITY_TIER);
		if (IsCamomonsTier(tier))
			gBattleTypeFlags |= BATTLE_TYPE_CAMOMONS;
		else if (tier == BATTLE_FACILITY_BENJAMIN_BUTTERFREE)
			gBattleTypeFlags |= BATTLE_TYPE_BENJAMIN_BUTTERFREE;
		else if (tier == BATTLE_FACILITY_MEGA_BRAWL)
			gBattleTypeFlags |= BATTLE_TYPE_MEGA_BRAWL;

		if (DynamaxAllowedInTier(tier) || (gBattleTypeFlags & BATTLE_TYPE_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_DYNAMAX))
			gBattleTypeFlags |= BATTLE_TYPE_DYNAMAX;

		switch (VarGet(VAR_BATTLE_FACILITY_BATTLE_TYPE)) {
			case BATTLE_FACILITY_DOUBLE:
			case BATTLE_FACILITY_DOUBLE_RANDOM:
				gBattleTypeFlags |= BATTLE_TYPE_DOUBLE;
				break;
			case BATTLE_FACILITY_MULTI:
			case BATTLE_FACILITY_MULTI_RANDOM:
				if (gTrainerBattleOpponent_A == FRONTIER_BRAIN_TID) //Frontier Brains fight alone
					gBattleTypeFlags |= (BATTLE_TYPE_DOUBLE | BATTLE_TYPE_INGAME_PARTNER /* | BATTLE_TYPE_MULTI*/);
				else
					gBattleTypeFlags |= (BATTLE_TYPE_DOUBLE | BATTLE_TYPE_TWO_OPPONENTS | BATTLE_TYPE_INGAME_PARTNER);
				break;
			case BATTLE_FACILITY_LINK_MULTI:
				if (gTrainerBattleOpponent_A == FRONTIER_BRAIN_TID) //Frontier Brains fight alone
					gBattleTypeFlags |= (BATTLE_TYPE_DOUBLE | BATTLE_TYPE_MULTI | BATTLE_TYPE_LINK);
				else
					gBattleTypeFlags |= (BATTLE_TYPE_DOUBLE | BATTLE_TYPE_TWO_OPPONENTS | BATTLE_TYPE_MULTI | BATTLE_TYPE_LINK);
				break;
		}
	}
	else
	{
		if (gApproachingTrainerId == 2 || FlagGet(FLAG_TWO_OPPONENTS))
			gBattleTypeFlags = (BATTLE_TYPE_DOUBLE | BATTLE_TYPE_TWO_OPPONENTS | BATTLE_TYPE_TRAINER);
		else
			gBattleTypeFlags = (BATTLE_TYPE_TRAINER);

		if (FlagGet(FLAG_TAG_BATTLE))
			gBattleTypeFlags |= (BATTLE_TYPE_DOUBLE | BATTLE_TYPE_INGAME_PARTNER /* | BATTLE_TYPE_MULTI*/);

		#if (defined CONTINUE_LOST_BATTLES && defined TUTORIAL_BATTLES)
			if (Var8000 != 0xFEFE && sTrainerBattleMode == TRAINER_BATTLE_OAK_TUTORIAL && sTrainerBattleOakTutorialHelper & 3)
				gBattleTypeFlags |= BATTLE_TYPE_OAK_TUTORIAL;
		#elif (defined TUTORIAL_BATTLES)
			if (sTrainerBattleMode == TRAINER_BATTLE_OAK_TUTORIAL && sTrainerBattleOakTutorialHelper & 3)
				gBattleTypeFlags |= BATTLE_TYPE_OAK_TUTORIAL;
		#endif

		#ifdef FLAG_ACTIVATE_TUTORIAL
		if (FlagGet(FLAG_ACTIVATE_TUTORIAL))
		{
			gBattleTypeFlags |= BATTLE_TYPE_OAK_TUTORIAL;
			sTrainerBattleMode = TRAINER_BATTLE_OAK_TUTORIAL;
			sTrainerBattleOakTutorialHelper = 3;
		}
		#endif

		#ifdef FLAG_CAMOMONS_BATTLE
		if (FlagGet(FLAG_CAMOMONS_BATTLE))
			gBattleTypeFlags |= BATTLE_TYPE_CAMOMONS;
		#endif

		#ifdef FLAG_BENJAMIN_BUTTERFREE_BATTLE
		if (FlagGet(FLAG_BENJAMIN_BUTTERFREE_BATTLE))
			gBattleTypeFlags |= BATTLE_TYPE_BENJAMIN_BUTTERFREE;
		#endif

		#ifdef FLAG_RING_CHALLENGE_BATTLE
		if (FlagGet(FLAG_RING_CHALLENGE_BATTLE))
			gBattleTypeFlags |= BATTLE_TYPE_RING_CHALLENGE;
		#endif

		#ifdef FLAG_AI_CONTROL_BATTLE
		if (FlagGet(FLAG_AI_CONTROL_BATTLE))
			gBattleTypeFlags |= BATTLE_TYPE_MOCK_BATTLE;
		#endif
	}

	#ifdef FLAG_DYNAMAX_BATTLE
	if (FlagGet(FLAG_DYNAMAX_BATTLE))
		gBattleTypeFlags |= BATTLE_TYPE_DYNAMAX;
	#endif

	gMain.savedCallback = CB2_EndTrainerBattle;
	StartTheBattle();
	ScriptContext1_Stop();
}

//Special 0x34
const u8* GetIntroSpeechOfApproachingTrainer(void)
{
	if (FlagGet(FLAG_BATTLE_FACILITY))
	{
		switch (Var8000) {
			case 0:
				CopyFrontierTrainerText(FRONTIER_BEFORE_TEXT, BATTLE_TOWER_TID, 0);
				return ReturnEmptyStringIfNull((const u8*) gStringVar4);

			case 1:
				CopyFrontierTrainerText(FRONTIER_BEFORE_TEXT, BATTLE_TOWER_TID, 1);
				return ReturnEmptyStringIfNull((const u8*) gStringVar4);

			case 2:
				CopyFrontierTrainerText(FRONTIER_BEFORE_TEXT, Var8001, 0); //For Frontier Brain
				return ReturnEmptyStringIfNull((const u8*) gStringVar4);

			default:
				return ReturnEmptyStringIfNull((const u8*) sTrainerIntroSpeech_A);
		}
	}

	if (gApproachingTrainerId == 0)
		return ReturnEmptyStringIfNull((const u8*) sTrainerIntroSpeech_A);
	else
		return ReturnEmptyStringIfNull((const u8*) sTrainerIntroSpeech_B);
}

//Special 0x35
const u8* GetTrainerCantBattleSpeech(void)
{
	if ((sTrainerBattleMode == TRAINER_BATTLE_TWO_OPPONENTS || sTrainerBattleMode == TRAINER_BATTLE_REMATCH_TWO_OPPONENTS)
	&& gEventObjects[gSelectedEventObject].localId == ExtensionState.spotted.secondTrainerNPCId)
		return ReturnEmptyStringIfNull((const u8*) sTrainerCannotBattleSpeech_B);
	else
		return ReturnEmptyStringIfNull((const u8*) sTrainerCannotBattleSpeech);
}

//Special 0x38
void SetUpTrainerEncounterMusic(void)
{
	u16 trainerId;
	u16 music;

	if (gQuestLogMode == 2 || gQuestLogMode == 3)
		return;

	if (gApproachingTrainerId == 0)
		trainerId = gTrainerBattleOpponent_A;
	else
		trainerId = VarGet(VAR_SECOND_OPPONENT);

	if (sTrainerBattleMode != TRAINER_BATTLE_CONTINUE_SCRIPT_NO_MUSIC
	&&  sTrainerBattleMode != TRAINER_BATTLE_CONTINUE_SCRIPT_DOUBLE_NO_MUSIC)
	{
		#ifndef ENCOUNTER_MUSIC_BY_CLASS
		switch (GetTrainerEncounterMusicId(trainerId)) {
			case TRAINER_ENCOUNTER_MUSIC_MALE: //0
			case TRAINER_ENCOUNTER_MUSIC_INTENSE: //4
			case TRAINER_ENCOUNTER_MUSIC_COOL: //5
			case TRAINER_ENCOUNTER_MUSIC_SWIMMER: //8
			case TRAINER_ENCOUNTER_MUSIC_ELITE_FOUR:  //10
			case TRAINER_ENCOUNTER_MUSIC_HIKER:	   //11
			case TRAINER_ENCOUNTER_MUSIC_INTERVIEWER: //12
			case TRAINER_ENCOUNTER_MUSIC_RICH:		//13
				music = BGM_EYE_BOY; //0x11D
				break;
			case TRAINER_ENCOUNTER_MUSIC_FEMALE: //1
			case TRAINER_ENCOUNTER_MUSIC_GIRL: //2
			case TRAINER_ENCOUNTER_MUSIC_TWINS: //3
				music = BGM_EYE_GIRL; //0x11C
				break;
			default:
				music = BGM_EYE_CREEPY; //0x11B
				break;
		}
		#else //ENCOUNTER_MUSIC_BY_CLASS
			music = gClassBasedTrainerEncounterBGM[gTrainers[trainerId].trainerClass];

			if (music == 0)
				music = BGM_EYE_BOY;
		#endif

		PlayNewMapMusic(music);
	}
}

//special 0x18F
void SetTrainerFlags(void)
{
	if (IsTwoOpponentBattle()) //Prevent bugs from happening when the second Trainer wasn't actually fought
		FlagSet(FLAG_TRAINER_FLAG_START + SECOND_OPPONENT);
	FlagSet(FLAG_TRAINER_FLAG_START + gTrainerBattleOpponent_A);
}

//Script Callasms
void AllowTrainerIncrementation(void)
{
	ExtensionState.multiTaskStateHelper = TRUE;
}

void MoveSecondNPCForTwoOpponentSighting(void)
{
	u8 localId, firstTrainerEventObj, secondTrainerEventObj;
	if (gEventObjects[ExtensionState.spotted.trainers[0].id].localId == ExtensionState.spotted.firstTrainerNPCId)
	{
		firstTrainerEventObj = GetEventObjectIdByLocalId(ExtensionState.spotted.firstTrainerNPCId);
		localId = ExtensionState.spotted.secondTrainerNPCId;
	}
	else
	{
		firstTrainerEventObj = GetEventObjectIdByLocalId(ExtensionState.spotted.secondTrainerNPCId);
		localId = ExtensionState.spotted.firstTrainerNPCId;
	}

	secondTrainerEventObj = GetEventObjectIdByLocalId(localId);
	Var8005 = localId;

	u16 dir;
	s16 playerX = gEventObjects[GetPlayerMapObjId()].currentCoords.x;
	s16 playerY = gEventObjects[GetPlayerMapObjId()].currentCoords.y;
	s16 npcX = gEventObjects[secondTrainerEventObj].currentCoords.x;
	s16 npcY = gEventObjects[secondTrainerEventObj].currentCoords.y;

	dir = 0xFFFF;
	switch(gEventObjects[firstTrainerEventObj].facingDirection) //The first Trainer is the one who spotted the player, so move in the direction they're facing
	{
		case 0:
		case 1:
		default:
			if (npcY != playerY - 1) //Above player
				dir = DIR_SOUTH;
			break;
		case 2:
			if (npcY != playerY + 1) //Below Player
				dir = DIR_NORTH;
			break;
		case 3:
			if (npcX != playerX + 1) //Right of Player
				dir = DIR_WEST;
			break;
		case 4:
			if (npcX != playerX - 1) //Left of Player
				dir = DIR_EAST;
			break;
	}

	if (GetCollisionInDirection(&gEventObjects[secondTrainerEventObj], dir))
		dir = 0xFFFF; //Must stop moving

	gSpecialVar_LastResult = dir;
}

void LoadProperIntroSpeechForTwoOpponentSighting(void)
{
	switch (Var8000)
	{
		case 0:
			if (gEventObjects[ExtensionState.spotted.trainers[0].id].localId != ExtensionState.spotted.firstTrainerNPCId)
			{
				gSelectedEventObject = GetEventObjectIdByLocalId(ExtensionState.spotted.secondTrainerNPCId);
				gApproachingTrainerId = 1;
			}
			else
			{
				gSelectedEventObject = GetEventObjectIdByLocalId(ExtensionState.spotted.firstTrainerNPCId);
				gApproachingTrainerId = 0;
			}
			break;

		default:
			if (gEventObjects[gSelectedEventObject].localId != ExtensionState.spotted.firstTrainerNPCId)
				gApproachingTrainerId = 1;
			else
				gApproachingTrainerId = 0;
			break;
	}
}

void PrepTrainerB(void)
{
	if (gEventObjects[gSelectedEventObject].localId == ExtensionState.spotted.firstTrainerNPCId)
	{
		gSelectedEventObject = GetEventObjectIdByLocalId(ExtensionState.spotted.secondTrainerNPCId);
		OverrideMovementTypeForEventObject(&gEventObjects[gSelectedEventObject], GetTrainerFacingDirectionMovementType(gEventObjects[gSelectedEventObject].facingDirection));
		OverrideTemplateCoordsForEventObject(&gEventObjects[gSelectedEventObject]);
	}
	else
		gSelectedEventObject = GetEventObjectIdByLocalId(ExtensionState.spotted.firstTrainerNPCId);

	gApproachingTrainerId ^= 1;
}

void HasOneTrainerBeenDefeated(void)
{
	gSpecialVar_LastResult = FlagGet(FLAG_TRAINER_FLAG_START + gTrainerBattleOpponent_A) || FlagGet(FLAG_TRAINER_FLAG_START + gTrainerBattleOpponent_B);
}

void MoveCameraToTrainerB(void)
{
	u8 newObj;
	if (gEventObjects[gSelectedEventObject].localId == ExtensionState.spotted.firstTrainerNPCId)
		newObj = GetEventObjectIdByLocalId(ExtensionState.spotted.secondTrainerNPCId);
	else
		newObj = GetEventObjectIdByLocalId(ExtensionState.spotted.firstTrainerNPCId);

	s16 currentX = gEventObjects[gSelectedEventObject].currentCoords.x;
	s16 currentY = gEventObjects[gSelectedEventObject].currentCoords.y;
	s16 toX = gEventObjects[newObj].currentCoords.x;
	s16 toY = gEventObjects[newObj].currentCoords.y;

	GetProperDirection(currentX, currentY, toX, toY);
	Var8005 = 0x7F; //Camera
}

static u8 GetPlayerMapObjId(void)
{
	return gPlayerAvatar->eventObjectId;
}

static u8 GetNPCDirectionFaceToPlayer(u8 eventObj)
{
	u8 playerObjId = GetPlayerMapObjId();
	u16 playerX = gEventObjects[playerObjId].currentCoords.x;
	u16 playerY = gEventObjects[playerObjId].currentCoords.y;
	u16 npcX = gEventObjects[eventObj].currentCoords.x;
	u16 npcY = gEventObjects[eventObj].currentCoords.y;

	if (GetProperDirection(playerX, playerY, npcX, npcY))
		return GetOppositeDirection(gSpecialVar_LastResult);

	return DIR_SOUTH; //Error handling...sort of
}

void TrainerFaceFix(void)
{
	u8 playerObjId = GetPlayerMapObjId();
	u16 playerX = gEventObjects[playerObjId].currentCoords.x;
	u16 playerY = gEventObjects[playerObjId].currentCoords.y;
	u16 npcX = gEventObjects[gSelectedEventObject].currentCoords.x;
	u16 npcY = gEventObjects[gSelectedEventObject].currentCoords.y;

	if (!GetProperDirection(playerX, playerY, npcX, npcY))
		gSpecialVar_LastResult = 0xFFFF;
}

static bool8 GetProperDirection(u16 currentX, u16 currentY, u16 toX, u16 toY)
{
	u8 ret = FALSE;

	if (currentX == toX)
	{
		if (currentY < toY)
			gSpecialVar_LastResult = DIR_SOUTH;
		else
			gSpecialVar_LastResult = DIR_NORTH;

		ret = TRUE;
	}
	else if (currentY == toY)
	{
		if (currentX < toX)
			gSpecialVar_LastResult = DIR_EAST;
		else
			gSpecialVar_LastResult = DIR_WEST;

		ret = TRUE;
	}

	return ret;
}

void FollowerPositionFix(u8 offset)
{
	gSpecialVar_LastResult = 0xFFFF;

	if (!gFollowerState.inProgress)
		return;

	u8 playerObjId = GetPlayerMapObjId();
	u8 followerObjid = gFollowerState.objId;
	u16 playerX = gEventObjects[playerObjId].currentCoords.x;
	u16 playerY = gEventObjects[playerObjId].currentCoords.y;
	u16 npcX = gEventObjects[followerObjid].currentCoords.x;
	u16 npcY = gEventObjects[followerObjid].currentCoords.y;

	if (playerX == npcX)
	{
		if (playerY > npcY)
		{
			if (playerY != npcY + offset) //Player and follower are not 1 tile apart
			{
				if (Var8000 == 0)
					gSpecialVar_LastResult = DIR_SOUTH;
				else
					gEventObjects[followerObjid].currentCoords.y = playerY - offset;
			}
		}
		else // Player Y <= npcY
		{
			if (playerY != npcY - offset) //Player and follower are not 1 tile apart
			{
				if (Var8000 == 0)
					gSpecialVar_LastResult = DIR_NORTH;
				else
					gEventObjects[followerObjid].currentCoords.y = playerY + offset;
			}
		}
	}
	else //playerY == npcY
	{
		if (playerX > npcX)
		{
			if (playerX != npcX + offset) //Player and follower are not 1 tile apart
			{
				if (Var8000 == 0)
					gSpecialVar_LastResult = DIR_EAST;
				else
					gEventObjects[followerObjid].currentCoords.x = playerX - offset;
			}
		}
		else // Player X <= npcX
		{
			if (playerX != npcX - offset) //Player and follower are not 1 tile apart
			{
				if (Var8000 == 0)
					gSpecialVar_LastResult = DIR_WEST;
				else
					gEventObjects[followerObjid].currentCoords.x = playerX + offset;
			}
		}
	}
}

void FollowerTrainerSightingPositionFix(void)
{
	FollowerPositionFix(1);
}

void FollowerIntoPlayer(void)
{
	FollowerPositionFix(0);
}

extern void __attribute__((long_call)) UpdateHappinessStepCounter(void);
extern bool8 __attribute__((long_call)) CheckVSSeeker(void);
extern bool8 __attribute__((long_call)) UpdatePoisonStepCounter(void);
//Hack take step function to include custom walking scripts
bool8 TryStartStepCountScript(u16 metatileBehavior)
{
	if (InUnionRoom() == TRUE
	||  gQuestLogMode == 2)
		return FALSE;

	gDexNavCooldown = FALSE; //Pokemon can be found with the DexNav again
	UpdateHappinessStepCounter();
	UpdateJPANStepCounters();
	#ifdef UNBOUND
	extern void TryPlayShoreNoise(void);
	TryPlayShoreNoise();
	#endif
	if (!(gPlayerAvatar->flags & PLAYER_AVATAR_FLAG_FISHING) && !MetatileBehavior_IsForcedMovementTile(metatileBehavior))
	{
		if (CheckVSSeeker() == TRUE)
		{
			ScriptContext1_SetupScript(EventScript_VSSeeker);
			return TRUE;
		}

		if (UpdatePoisonStepCounter() == TRUE)
		{
			#ifndef POISON_1_HP_SURVIVAL
			ScriptContext1_SetupScript(EventScript_Poison); //Tries to faint Pokemon
			#endif
			return TRUE;
		}

		if (ShouldEggHatch())
		{
			sp09A_StopSounds();
			PlaySE(SE_EXCLAIM);
			IncrementGameStat(GAME_STAT_HATCHED_EGGS);
			ScriptContext1_SetupScript(EventScript_EggHatch);
			return TRUE;
		}

		#ifdef DEXNAV_DETECTOR_MODE
		extern bool8 GetGen8SpeciesForDexNavDetectorMode(u8 blockProperties);
		if (FlagGet(FLAG_SYS_DEXNAV)
		&& FlagGet(FLAG_GEN_8_PLACED_IN_GAME)
		&& VarGet(VAR_R_BUTTON_MODE) == OPTIONS_R_BUTTON_MODE_DEXNAV //Only when player is trying to use the DexNav to find Pokemon
		&& (Random() % 10) == 0 //Adjust rate so it doesn't happen as often
		&& Overworld_GetFlashLevel() == 0 //DexNav can't be used here in general
		&& (VarGet(VAR_REPEL_STEP_COUNT) == 0 || VarGet(VAR_REPEL_STEP_COUNT) >= 50) //Only when the repel isn't on or repel will take time to wear off
		&& !IsDexNavHudActive())
		{
			s16 x, y;
			PlayerGetDestCoords(&x, &y);
			u32 currMetatileField = MapGridGetMetatileField(x, y, 0xFF);
			u8 blockProperties = GetMetatileAttributeFromRawMetatileBehavior(currMetatileField, METATILE_ATTRIBUTE_ENCOUNTER_TYPE);

			if (((blockProperties == TILE_FLAG_ENCOUNTER_TILE && LoadProperMonsData(LAND_MONS_HEADER) != NULL)
			|| (blockProperties == TILE_FLAG_SURFABLE && LoadProperMonsData(WATER_MONS_HEADER) != NULL)) //Only when standing in grass/on water and wild Pokemon can be found there
			&& GetGen8SpeciesForDexNavDetectorMode(blockProperties))
			{
				u8 taskId = CreateTask(Task_StartDexNavHUDAfterScript, 0xFF);
				if (taskId != 0xFF)
				{
					gTasks[taskId].data[0] = Var8000;
					gTasks[taskId].data[1] = Var8001;
					ScriptContext1_SetupScript(SystemScript_DexNavDetector);
					return TRUE;
				}
			}
		}
		#endif

		const u8* customWalkingScript = GetCustomWalkingScript();
		if (customWalkingScript != NULL)
		{
			ScriptContext1_SetupScript(customWalkingScript);
			return TRUE;
		}
	}

	if (SafariZoneTakeStep() == TRUE)
		return TRUE;

	return FALSE;
}

static void UpdateJPANStepCounters(void)
{
	// increment new pedometer, always on
	if (gPedometers->alwaysActive < 0xFFFFFFFF)
		++gPedometers->alwaysActive;

	// check new pedometers
	if (FlagGet(FLAG_LONG_PEDOMETER) && gPedometers->large < 0xFFFFFFFF)
		++gPedometers->large;
	if (FlagGet(FLAG_MED_PEDOMETER) && gPedometers->medium < 0xFFFF)
		++gPedometers->medium;
	if (FlagGet(FLAG_SMALL_PEDOMETER_1) && gPedometers->smallOne < 0xFF)
		++gPedometers->smallOne;
	if (FlagGet(FLAG_SMALL_PEDOMETER_2) && gPedometers->smallTwo < 0xFF)
		++gPedometers->smallTwo;
}

#ifdef DEXNAV_DETECTOR_MODE
static void Task_StartDexNavHUDAfterScript(u8 taskId)
{
	if (!ScriptContext2_IsEnabled())
	{
		gLastDexNavSpecies = SPECIES_NONE; //Because it's special
		InitDexNavHUD(gTasks[taskId].data[0], gTasks[taskId].data[1], TRUE);
		DestroyTask(taskId);
	}
}
#endif

static const u8* GetCustomWalkingScript(void)
{
	#if (defined VAR_DEFAULT_WALKING_SCRIPT && !defined UNBOUND)
	if (gWalkingScript >= (u8*) 0x8000000) //A real script
		return gWalkingScript;

	u8 scriptInd = VarGet(VAR_DEFAULT_WALKING_SCRIPT);
	if (scriptInd != 0 && scriptInd <= ARRAY_COUNT(sDefaultWalkingScripts))
		return sDefaultWalkingScripts[scriptInd - 1];
	#endif

	return NULL;
}

static bool8 SafariZoneTakeStep(void)
{
	// check in safari zone
	if (GetSafariZoneFlag() && gSafariSteps != 0)
	{
		gSafariSteps -= 1;
		if (gSafariSteps == 0)	// safari steps went to zero
		{
			ScriptContext1_SetupScript(SafariZoneEndScript);
			return TRUE;
		}
	}

	return FALSE;
}

#ifdef ONLY_CHECK_ITEM_FOR_HM_USAGE
static const u8* TryUseFlashInDarkCave(void)
{
	sp100_CanPlayerUseFlashInCurrentLocation();

	if (gSpecialVar_LastResult && HasBadgeToUseFlash())
	{
		if ((Var8004 = gFieldEffectArguments[0] = PartyHasMonWithFieldMovePotential(MOVE_FLASH, ITEM_TM70, 0)) < PARTY_SIZE)
			return EventScript_UseFlash;
	}

	return NULL;
}
#endif

void RunOnTransitionMapScript(void)
{
	//Reset streaks upon moving to a new map
	gCurrentDexNavChain = 0;
	gFishingStreak = 0;
	gLastFishingSpecies = 0;
	gDontFadeWhite = FALSE;
	ResetMiningSpots();
	ForceClockUpdate();
	MapHeaderRunScriptByTag(3);
}

void RunOnResumeMapScript(void)
{
	ForceClockUpdate();
	MapHeaderRunScriptByTag(5);
}

bool8 TryRunOnFrameMapScript(void)
{
	TryUpdateSwarm();

	//if (gQuestLogMode != 3)
	{
		const u8* ptr;

		#ifdef ONLY_CHECK_ITEM_FOR_HM_USAGE
		ptr = TryUseFlashInDarkCave();

		if (ptr == NULL)
		#endif
			ptr = MapHeaderCheckScriptTable(MAP_SCRIPT_ON_FRAME_TABLE);

		if (ptr != NULL)
		{
			ScriptContext1_SetupScript(ptr);
			return TRUE;
		}
	}

	return FALSE;
}

// Whiteout Hack
void __attribute__((long_call)) Task_RushInjuredPokemonToCenter(u8 taskId);
void FieldCB_RushInjuredPokemonToCenter(void)
{
    u8 taskId;

    ScriptContext2_Enable();
    palette_bg_faded_fill_black();
	gPaletteFade->active = TRUE; //So the DNS doesn't get messed up at night
	DismissMapNamePopup();
    taskId = CreateTask(Task_RushInjuredPokemonToCenter, 10);
    gTasks[taskId].data[0] = 0;
}

bool8 WhiteoutLogic(void)
{
#ifdef SET_HEALING_PLACE_HACK
	u16 loc = VarGet(VAR_HEALINGMAP);
	gWarp1->mapNum = (loc >> 8) & 0xFF;	// upper byte
	gWarp1->mapGroup = loc & 0xFF;	// lower byte
	gWarp1->warpId = 0xFF;
	gWarp1->x = VarGet(VAR_HEALING_XPOS);
	gWarp1->y = VarGet(VAR_HEALING_YPOS);
	return FALSE;
#else
	return TRUE;	// load from original table
#endif
}

#define sText_ScurriedHome (const u8*) 0x841B5B6
extern const u8 gText_ScurriedToNearestHealer[];
extern const u8 gText_AllJustADream[];

const u8* LoadProperWhiteoutString(const u8* string)
{
	#ifdef SET_HEALING_PLACE_HACK
	if (string == sText_ScurriedHome)
	{
		if (gSaveBlock1->location.mapNum != MAP_NUM(PLAYER_HOME)
		||  gSaveBlock1->location.mapGroup != MAP_GROUP(PLAYER_HOME))
		{
			string = gText_ScurriedToNearestHealer;
		}
	}
	#endif

	return string;
}

bool8 ShouldPlayerRun(u16 heldKeys)
{
	if (IsRunningDisallowed(gEventObjects[gPlayerAvatar->eventObjectId].currentMetatileBehavior))
		return FALSE;

	if (IsDexNavHudActive())
		return FALSE; //Prevent running while DexNav is open. People are just too stupid to realize they can't run

	#ifdef FLAG_AUTO_RUN
	if (FlagGet(FLAG_AUTO_RUN))
	{
		if (heldKeys & B_BUTTON)
			return FALSE; //Walk when holding B while auto-run is on
		else
			return TRUE;
	}
	else
	#endif
	{
		if (heldKeys & B_BUTTON)
			return TRUE;
		else
			return FALSE;
	}
}

static bool8 IsRunningDisabledByFlag(void)
{
	#ifdef FLAG_RUNNING_ENABLED
		return !FlagGet(FLAG_RUNNING_ENABLED);
	#else
		return FALSE;
	#endif
}

bool8 IsRunningDisallowed(u8 tile)
{
	return IsRunningDisabledByFlag() || IsRunningDisallowedByMetatile(tile)
		|| gMapHeader.mapType == MAP_TYPE_UNDERWATER
	#ifndef CAN_RUN_IN_BUILDINGS
		|| GetCurrentMapType() == MAP_TYPE_INDOOR
	#endif
	;
}

bool8 IsRunningDisallowedByMetatile(u8 tile)
{
	return MetatileBehavior_IsRunningDisallowed(tile);
}

bool8 Overworld_IsBikingAllowed(void)
{
	if (gFollowerState.inProgress && !(gFollowerState.flags & FOLLOWER_FLAG_CAN_BIKE))
		return FALSE;

	if (gMapHeader.mapType == MAP_TYPE_UNDERWATER)
		return FALSE;

	return gMapHeader.bikingAllowed
#ifdef BIKE_ON_ANY_NON_INSIDE_MAP
	|| !IsMapTypeIndoors(GetCurrentMapType());
#endif
	;
}

//If you use Spherical Ice's sideways stairs
static bool8 MetatileBehavior_IsWestSidewaysStairs(u8 behaviour)
{
	return behaviour == 0xB5 || behaviour == 0xB2 || behaviour == 0xB0 || behaviour == 0xB3 //Stairs
		|| behaviour == 0xB1; //Tile at bottom
}

static bool8 MetatileBehavior_IsEastSidewaysStairs(u8 behaviour)
{
	return behaviour == 0xB5 || behaviour == 0xB2 || behaviour == 0xB0 || behaviour == 0xB3 //Stairs
		|| behaviour == 0xB4; //Tile at bottom
}

static bool8 PlayerIsMovingOnSidewaysStairs(u8 direction)
{
	struct EventObject* eventObject = &gEventObjects[gPlayerAvatar->eventObjectId];
	s16 x = eventObject->currentCoords.x;
	s16 y = eventObject->currentCoords.y;

	switch (direction)
	{
		case DIR_WEST:
			return MetatileBehavior_IsWestSidewaysStairs(MapGridGetMetatileBehaviorAt(x, y));
		case DIR_EAST:
			return MetatileBehavior_IsEastSidewaysStairs(MapGridGetMetatileBehaviorAt(x, y));
	}

	return FALSE;
}

static bool8 MovingFastOnWater(void)
{
	if (gFollowerState.inProgress) //Probably would cause a whole host of issues otherwise
		return FALSE;

	#ifdef FLAG_SURF_TURBO_BOOST
	if (FlagGet(FLAG_SURF_TURBO_BOOST))
	{
		if (JOY_HELD(B_BUTTON))
			return FALSE; //Hold B to move normal speed
		
		return TRUE;
	}
	#endif

	return JOY_HELD(B_BUTTON);
}

s16 GetPlayerSpeed(void)
{
	s16 exp[] = { 1, 2, 4 };

	#ifdef FLAG_BIKE_TURBO_BOOST
	if (gPlayerAvatar->flags & PLAYER_AVATAR_FLAG_BIKE && gPlayerAvatar->bikeSpeed == 4 && gPlayerAvatar->runningState > 0)
		return 4;
	else
	#endif
	if (gPlayerAvatar->flags & PLAYER_AVATAR_FLAG_MACH_BIKE)
		return exp[gPlayerAvatar->bikeFrameCounter];
	else if (gPlayerAvatar->flags & PLAYER_AVATAR_FLAG_ACRO_BIKE)
		return 3;
	else if (gPlayerAvatar->flags & PLAYER_AVATAR_FLAG_SURFING)
	{
		if (MovingFastOnWater() && JOY_HELD(DPAD_UP | DPAD_RIGHT | DPAD_LEFT | DPAD_RIGHT)) //Has to actually be moving
			return 4;
		else
			return 2;
	}
	else if (gPlayerAvatar->flags & PLAYER_AVATAR_FLAG_DASH)
		return 2;
	else
		return 1;
}

void MoveOnBike(u8 direction)
{
	gPlayerAvatar->bikeSpeed = 0; //Reset

	if (PlayerIsMovingOnRockStairs(direction))
		PlayerGoSpeed2(direction);
	else if (PlayerIsMovingOnSidewaysStairs(direction))
		PlayerRideWaterCurrent(direction);
	#ifdef FLAG_BIKE_TURBO_BOOST
	else if (!gFollowerState.inProgress //Probably would cause a whole host of issues otherwise
			&& ((!FlagGet(FLAG_BIKE_TURBO_BOOST) && JOY_HELD(B_BUTTON))
			  || (FlagGet(FLAG_BIKE_TURBO_BOOST) && !JOY_HELD(B_BUTTON))))
	{
		PlayerGoSpeed4(direction);
		gPlayerAvatar->bikeSpeed = 4;
	}
	#endif
	else
		PlayerRideWaterCurrent(direction);
}

void MovePlayerWhileSurfing(u8 direction)
{
	if (MovingFastOnWater())
		PlayerGoSpeed4(direction);
	else
		PlayerGoSpeed2(direction);
}

void PlayerOnBikeCollide(u8 direction)
{
	gPlayerAvatar->bikeSpeed = 0; //Reset
    PlayCollisionSoundIfNotFacingWarp(direction);
    PlayerSetAnimId(GetWalkInPlaceNormalMovementAction(direction), 2);
}

bool8 CanUseEscapeRopeOnCurrMap(void)
{
	if (gFollowerState.inProgress && !(gFollowerState.flags & FOLLOWER_FLAG_CAN_LEAVE_ROUTE))
		return FALSE;

	return (gMapHeader.flags & MAP_ALLOW_ESCAPE_ROPE) != 0;
}

bool8 MetatileBehavior_IsMuddySlope(u8 metatileBehavior)
{
	return metatileBehavior == MB_MUDDY_SLOPE;
}

bool8 ForcedMovement_MuddySlope(void)
{
	struct EventObject* playerEventObj = &gEventObjects[gPlayerAvatar->eventObjectId];

	if (playerEventObj->movementDirection != DIR_NORTH || GetPlayerSpeed() <= 3)
	{
		playerEventObj->facingDirectionLocked = 1;
		return DoForcedMovement(1, PlayerGoSpeed2);
	}

	return FALSE;
}

//Taken From EM for if people want to use them
//Although they need to be hooked in manually, as the previous functions
//are too small to replace.
bool8 MetatileBehavior_IsBumpySlope(u8 metatileBehavior) { return metatileBehavior == MB_BUMPY_SLOPE; }
bool8 MetatileBehavior_IsCrackedFloor(u8 metatileBehavior) { return metatileBehavior == MB_CRACKED_FLOOR; }
bool8 MetatileBehavior_IsIsolatedVerticalRail(u8 metatileBehavior) { return metatileBehavior == MB_ISOLATED_VERTICAL_RAIL; }
bool8 MetatileBehavior_IsIsolatedHorizontalRail(u8 metatileBehavior) { return metatileBehavior == MB_ISOLATED_HORIZONTAL_RAIL; }
bool8 MetatileBehavior_IsVerticalRail(u8 metatileBehavior) { return metatileBehavior == MB_VERTICAL_RAIL; }
bool8 MetatileBehavior_IsHorizontalRail(u8 metatileBehavior) { return metatileBehavior == MB_HORIZONTAL_RAIL; }

s32 DoPoisonFieldEffect(void)
{
#ifndef NO_POISON_IN_OW
	int i;
	u32 hp;
	struct Pokemon* mon;
	u32 numPoisoned = 0;
	u32 numFainted = 0;
	u32 numSurvived = 0;

	for (i = 0; i < PARTY_SIZE; i++)
	{
		mon = &gPlayerParty[i];
		u8 ability = GetMonAbility(mon);
		if (GetMonData(mon, MON_DATA_SPECIES, NULL) != SPECIES_NONE
		&& GetMonPrimaryAilments(mon->condition) == AILMENT_PSN
		&& ability != ABILITY_POISONHEAL && ability != ABILITY_MAGICGUARD)
		{
			hp = mon->hp;

			#ifdef POISON_1_HP_SURVIVAL
				if (hp == 1 || --hp == 1)
				{
					mon->hp = hp;
					mon->condition = STATUS1_NONE;

					if (mon->friendship >= 200)
						mon->friendship -= 10;
					else if (mon->friendship >= 5)
						mon->friendship -= 5;

					++numSurvived;
					ScriptContext1_SetupScript(SystemScript_PoisonSurvial);
					GetMonData(&gPlayerParty[i], MON_DATA_NICKNAME, gStringVar1);
					break;
				}
			#else
				if (hp == 0 || --hp == 0)
				{
					numFainted++;
				}
			#endif

			mon->hp = hp;
			numPoisoned++;
		}
	}

	if (numSurvived != 0)
	{
		return FLDPSN_FNT; //Triggers the script
	}
	if (numFainted != 0 || numPoisoned != 0)
	{
		FldEffPoison_Start();
	}
	if (numFainted != 0)
	{
		return FLDPSN_FNT;
	}
	if (numPoisoned != 0)
	{
		return FLDPSN_PSN;
	}
#endif

	return FLDPSN_NONE;
}

void TaskRepel(u8 taskId)
{
	if (!IsSEPlaying())
	{
		//WriteQuestLog(4, 0, Var800E, 0xFFFF);
		VarSet(VAR_REPEL_STEP_COUNT, ItemId_GetHoldEffectParam(Var800E));
		#ifdef BW_REPEL_SYSTEM
		gLastUsedRepel = Var800E;
		#endif
		RemoveUsedItem();
		DisplayItemMessageInBag(taskId, 1, gStringVar4, Task_ReturnToBagFromContextMenu);
	}
};

//Updated Repel - hook at 080830B8 via r1
bool8 UpdateRepelCounter(void)
{
	u8 steps = VarGet(VAR_REPEL_STEP_COUNT); //0x4020

	if (steps != 0)
	{
		--steps;
		
		if (steps == 0 && IsDexNavHudActive())
			steps = 1; //Keep steps at 1 while DexNav is active so the pop-up doesn't interrupt the search

		VarSet(VAR_REPEL_STEP_COUNT, steps);
		if (steps == 0)
		{
			#ifdef BW_REPEL_SYSTEM
				Var800E = gLastUsedRepel;
				ScriptContext1_SetupScript(EventScript_BwRepelWoreOff);
				return TRUE;
			#else
				if (steps == 0)
				ScriptContext1_SetupScript(EventScript_RepelWoreOff);
				return TRUE;
			#endif
		}
	}

	return FALSE;
}

bool8 IsCurrentAreaVolcano(void)
{
	#ifdef UNBOUND
		u8 mapSec = GetCurrentRegionMapSectionId();
		return mapSec == MAPSEC_CINDER_VOLCANO
			|| (mapSec == MAPSEC_VICTORY_ROAD && MAP_IS(VICTORY_ROAD_VOLCANO))
			|| (mapSec == MAPSEC_TOMB_OF_BORRIUS && MAP_IS(TOMB_OF_BORRIUS_B3F));
	#else
		return FALSE;
	#endif
}

bool8 IsCurrentAreaAutumn(void)
{
	#ifdef UNBOUND
		u8 mapSec = GetCurrentRegionMapSectionId();
		return mapSec == MAPSEC_TEHL_TOWN
			|| mapSec == MAPSEC_ROUTE_9
			|| mapSec == MAPSEC_AUTL_WOODS
			|| mapSec == MAPSEC_ROUTE_10
			|| mapSec == MAPSEC_AUBURN_WATERWAY
			|| (mapSec == MAPSEC_HIDDEN_GROTTO
			 && MAP_IS(HIDDEN_GROTTO_AUTUMN));
	#else
		return FALSE;
	#endif
}

bool8 IsCurrentAreaWinter(void)
{
	#ifdef NEW_BATTLE_BACKGROUNDS
		u8 mapSec = GetCurrentRegionMapSectionId();
		return mapSec == MAPSEC_ROUTE_12;
		return mapSec == MAPSEC_ROUTE_12 || mapSec == MAPSEC_RUIN_VALLEY;		
	#else
		return FALSE;
	#endif
}

bool8 IsCurrentAreaDesert(void)
{
	#ifdef NEW_BATTLE_BACKGROUNDS
		u8 mapSec = GetCurrentRegionMapSectionId();
		return mapSec == MAPSEC_BERRY_FOREST;
	#else
		return FALSE;
	#endif
}

bool8 IsCurrentAreaHotCave(void)
{
	#ifdef NEW_BATTLE_BACKGROUNDS
		u8 mapSec = GetCurrentRegionMapSectionId();
		return mapSec == MAPSEC_ROUTE_12; // Scalding Spa is here
	#else
		return FALSE;
	#endif
}
bool8 IsCurrentAreaSwamp(void)
{
	#ifdef UNBOUND
		u8 mapSec = GetCurrentRegionMapSectionId();
		return mapSec == MAPSEC_POLDER_TOWN
			|| mapSec == MAPSEC_COOTES_BOG;
	#else
		return FALSE;
	#endif
}

static bool8 IsPlayerFacingMurkyBrownWater(void)
{
	return IsCurrentAreaSwamp() && !IsPlayerFacingSea();
}

void IsPlayerFacingMurkyBrownWaterToVar(void)
{
	gSpecialVar_LastResult = IsPlayerFacingMurkyBrownWater();
}

bool8 IsCurrentAreaDarkerCave(void)
{
	#ifdef UNBOUND
		u8 mapSec = GetCurrentRegionMapSectionId();
		return mapSec == MAPSEC_ICICLE_CAVE
			|| mapSec == MAPSEC_ICY_HOLE
			|| mapSec == MAPSEC_VALLEY_CAVE
			|| mapSec == MAPSEC_FROST_MOUNTAIN
			|| mapSec == MAPSEC_THUNDERCAP_MOUNTAIN
			|| mapSec == MAPSEC_DISTORTION_WORLD
			|| mapSec == MAPSEC_FROZEN_TOMB
			|| mapSec == MAPSEC_ISLAND_CAVE
			|| (mapSec == MAPSEC_VICTORY_ROAD
			 && (MAP_IS(VICTORY_ROAD_CAVE_A)
			  || MAP_IS(VICTORY_ROAD_CAVE_B)
			  || MAP_IS(VICTORY_ROAD_CAVE_C)
			  || MAP_IS(VICTORY_ROAD_CAVE_D)
			  || MAP_IS(VICTORY_ROAD_ICE_CAVE_A)
			  || MAP_IS(VICTORY_ROAD_ICE_CAVE_B)));
	#else
		return FALSE;
	#endif
}

bool8 InTanobyRuins(void)
{
	#ifdef TANOBY_RUINS_ENABLED
		if (FlagGet(FLAG_SYS_UNLOCKED_TANOBY_RUINS))
		{
			u8 mapSec = GetCurrentRegionMapSectionId();
			return mapSec >= MAPSEC_MONEAN_CHAMBER && mapSec <= MAPSEC_VIAPOIS_CHAMBER;
		}
	#elif (defined UNBOUND) //For Pokemon Unbound
		if (FlagGet(FLAG_OPENED_TOMB_OF_BORRIUS_HOLE))
			return GetCurrentRegionMapSectionId() == MAPSEC_TOMB_OF_BORRIUS;
	#endif

	return FALSE;
}

bool8 MetatileBehavior_IsStairs(u8 behaviour)
{
	return behaviour == MB_ROCK_STAIRS
		|| behaviour == MB_SIDEWAYS_STAIRS_LEFT_UP_1
		|| behaviour == MB_SIDEWAYS_STAIRS_LEFT_UP_2
		|| behaviour == MB_SIDEWAYS_STAIRS_RIGHT_UP_1
		|| behaviour == MB_SIDEWAYS_STAIRS_RIGHT_UP_2;
}

void PlayGrassFootstepNoise(void)
{
	if (IsFanfareTaskInactive()) //Sound interrupts fanfare
		PlaySE(SE_GRASS_FOOTSTEP);
}

extern bool8 (*const GetLedgeJumpFuncs[])(u8);
u8 GetLedgeJumpDirection(s16 x, s16 y, u8 direction)
{
	u8 behaviour;

	if (direction == 0)
		return 0;
	else if (direction > 4)
		direction -= 4;

	direction--;
	behaviour = MapGridGetMetatileBehaviorAt(x, y);

	if (GetLedgeJumpFuncs[direction](behaviour) == 1)
		return direction + 1;
	else if (behaviour == MB_OMNIDIRECTIONAL_JUMP
	#ifdef CAN_ONLY_USE_OMNIDRECTIONAL_JUMP_ON_HEIGHT_2
	&& gEventObjects[GetPlayerMapObjId()].currentElevation == 3 //Movement permission 0xC
	#endif
	)
	{
		return direction + 1;
	}

	return 0;
}

void PlayerJumpLedge(u8 direction)
{
	u8 movementAction = GetJump2MovementAction(direction);
 
	if (!TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_BIKE | PLAYER_AVATAR_FLAG_SURFING) //Player is not biking/surfing
	&& ShouldPlayerRun(gMain.heldKeys)) //Player is running
		movementAction += 0x70; //Change sprite to running hop

	PlayerSetAnimId(movementAction, 8); //Set appropriate jumping sprite
	PlaySE(SE_HOP);
}

const u8* GetInteractedMetatileScript(unusedArg struct MapPosition* position, u8 metatileBehavior, u8 direction)
{
	gSpecialVar_PlayerFacing = direction;

	switch (metatileBehavior) {
		case MB_TELEVISION:
		case MB_BERRY_CRUSH_RECORDS:
		case MB_BATTLE_RECORDS:
			if (direction == DIR_NORTH)
				return sMetatileInteractionScripts[metatileBehavior];
			break;
		case MB_POKEMON_CENTER_SIGN:
		case MB_POKEMART_SIGN:
			if (direction == DIR_NORTH)
			{
				SetTextboxSignpostDesign();
				return sMetatileInteractionScripts[metatileBehavior];
			}
			break;
		#ifdef MB_CLIMBABLE_LADDER
		case MB_CLIMBABLE_LADDER:
			//Only use ladder if player is at bottom of it, or the player is on higher elevation
			if (direction != DIR_SOUTH || gEventObjects[GetPlayerMapObjId()].currentElevation != 3)
				return sMetatileInteractionScripts[metatileBehavior];
			break;
		#endif
		#ifdef MB_HEADBUTT_TREE
		case MB_HEADBUTT_TREE: ;
			#ifdef UNBOUND
			u8 mapSec = GetCurrentRegionMapSectionId();
			if (mapSec != MAPSEC_GRIM_WOODS && mapSec != MAPSEC_VIVILL_WOODS) //Can't headbutt in these places
				return sMetatileInteractionScripts[metatileBehavior];
			#endif
			break;
		#endif
		#ifdef MB_UNDERGROUND_MINING
		case MB_UNDERGROUND_MINING:
			if (IsValidMiningSpot(position->x, position->y))
				return sMetatileInteractionScripts[metatileBehavior];
			break;
		#endif
		case MB_INDIGO_PLATEAU_MARK_DPAD:
		case MB_INDIGO_PLATEAU_MARK_2_DPAD:
			SetTextboxSignpostDesign();
			__attribute__ ((fallthrough));
		default:
			if (metatileBehavior < ARRAY_COUNT(sMetatileInteractionScripts))
				return sMetatileInteractionScripts[metatileBehavior];
	}

	return NULL;
}

void SetCutGrassMetatile(s16 x, s16 y)
{
	u32 i;
	s32 metatileId = MapGridGetMetatileIdAt(x, y);

	for (i = 0; i < ARRAY_COUNT(sCutGrassTiles); ++i)
	{
		if (sCutGrassTiles[i].originalMetatileId == metatileId
		&& (gMapHeader.mapLayout->primaryTileset == sCutGrassTiles[i].tilesetPtr
		 || gMapHeader.mapLayout->secondaryTileset == sCutGrassTiles[i].tilesetPtr))
		{
			MapGridSetMetatileIdAt(x, y, sCutGrassTiles[i].newMetatileId);
			break;
		}
	}
}

void FollowHiddenGrottoWarp(void)
{
	s8 warpEventId;
	struct MapPosition position;

	GetPlayerPosition(&position);
	gSpecialVar_LastResult = FALSE;
	warpEventId = GetWarpEventAtMapPosition(&gMapHeader, &position);

	if (warpEventId != -1)
	{
		StoreInitialPlayerAvatarState();
		SetupWarp(&gMapHeader, warpEventId, &position);
		DoWarp();
		gSpecialVar_LastResult = TRUE;
	}
}

void PrepMiningWarp(void)
{
	#ifdef MB_UNDERGROUND_MINING 
	s8 warpEventId;
	struct MapPosition position;

	GetPlayerPosition(&position);
	gSpecialVar_LastResult = FALSE;
	warpEventId = GetWarpEventAtMapPosition(&gMapHeader, &position);

	if (GetPlayerFacing() == DIR_NORTH && warpEventId != -1) //Collapsed doorways are always to north - prevents issues with standing on other warps
	{
		StoreInitialPlayerAvatarState();
		SetupWarp(&gMapHeader, warpEventId, &position);

		if (gMapHeader.mapType == MAP_TYPE_UNDERWATER)
		{
			if (MAP_IS(UNDERWATER_VIVILL_TOWN))
				gSpecialVar_LastResult = 0xFE;
			else
				gSpecialVar_LastResult = 0xFF;
		}
		else
			gSpecialVar_LastResult = TRUE;
	}
	#endif
}

static bool8 MetatileBehavior_IsClimbableLadder(unusedArg u8 behaviour)
{
	#ifndef MB_CLIMBABLE_LADDER
	return FALSE;
	#else
	return behaviour == MB_CLIMBABLE_LADDER;
	#endif
}

void ShouldLadderClimbContinue(void)
{
	struct EventObject *playerEventObj = &gEventObjects[gPlayerAvatar->eventObjectId];
	s16 x = playerEventObj->currentCoords.x;
	s16 y = playerEventObj->currentCoords.y;

	MoveCoords(gSpecialVar_PlayerFacing, &x, &y);
	gSpecialVar_LastResult = MetatileBehavior_IsClimbableLadder(MapGridGetMetatileBehaviorAt(x, y));
}

void IsUnboundToVar(void)
{
	#ifdef UNBOUND
	gSpecialVar_LastResult = TRUE;
	#else
	gSpecialVar_LastResult = FALSE;
	#endif
}

bool8 IsPlayerFacingRockClimbableWall(void)
{
	struct EventObject *playerEventObj = &gEventObjects[gPlayerAvatar->eventObjectId];
	s16 x = playerEventObj->currentCoords.x;
	s16 y = playerEventObj->currentCoords.y;

	MoveCoords(playerEventObj->facingDirection, &x, &y);
	return MetatileBehavior_IsRockClimbableWall(MapGridGetMetatileBehaviorAt(x, y));
}

bool8 MetatileBehavior_IsRockClimbableWall(u8 behaviour)
{
	return behaviour == MB_ROCK_CLIMB_WALL;
}

u8 PartyHasMonWithFieldMovePotential(u16 move, unusedArg u16 item, u8 surfingType)
{
	bool8 isSurfing = TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_SURFING);

	if (surfingType == 0
	|| (surfingType == SHOULDNT_BE_SURFING && !isSurfing)
	|| (surfingType == SHOULD_BE_SURFING && isSurfing))
	{
		#ifdef ONLY_CHECK_ITEM_FOR_HM_USAGE
		bool8 hasHM = CheckBagHasItem(item, 1) > 0;
		#endif

		for (u32 i = 0; i < PARTY_SIZE; ++i)
		{
			struct Pokemon* mon = &gPlayerParty[i];

			if (GetMonData(mon, MON_DATA_SPECIES, NULL) != SPECIES_NONE
			&& !GetMonData(mon, MON_DATA_IS_EGG, NULL))
			{
				#ifdef ONLY_CHECK_ITEM_FOR_HM_USAGE
				if (hasHM //Must have HM to prevent softlocks
				&& (MonKnowsMove(mon, move) || CanMonLearnTMTutor(mon, item, 0) == CAN_LEARN_MOVE))
					return i;
				#else
				if (MonKnowsMove(mon, move))
					return i;
				#endif
			}
		}
	}

	#ifdef DEBUG_HMS
		return 0;
	#endif

	return PARTY_SIZE;
}

bool8 IsPlayerSurfingNorthOrSouth(void)
{
	u8 dir = GetPlayerMovementDirection();

	return (dir == DIR_SOUTH || dir == DIR_NORTH)
		&& TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_SURFING);
}

#ifdef MB_LAVA
static bool8 MetatileBehavior_IsLava(u8 behaviour)
{
	return behaviour == MB_LAVA;
}
#endif

#ifdef MB_LAVA
static bool8 IsPlayerFacingSurfableLava(void)
{
	struct EventObject *playerEventObj = &gEventObjects[gPlayerAvatar->eventObjectId];
	s16 x = playerEventObj->currentCoords.x;
	s16 y = playerEventObj->currentCoords.y;

	MoveCoords(playerEventObj->facingDirection, &x, &y);
	return GetCollisionAtCoords(playerEventObj, x, y, playerEventObj->facingDirection) == 3
		&& PlayerGetZCoord() == 3
		&& MetatileBehavior_IsLava(MapGridGetMetatileBehaviorAt(x, y));
}
#endif

static bool8 IsPlayerFacingSea(void)
{
	struct EventObject *playerEventObj = &gEventObjects[gPlayerAvatar->eventObjectId];
	s16 x = playerEventObj->currentCoords.x;
	s16 y = playerEventObj->currentCoords.y;

	MoveCoords(playerEventObj->facingDirection, &x, &y);
	return MapGridGetMetatileBehaviorAt(x, y) == MB_SPLASHING_WATER;
}

extern const u8 EventScript_UseLavaSurf_Debug[];
const u8* GetInteractedWaterScript(unusedArg u32 unused1, u8 metatileBehavior, unusedArg u8 direction)
{
	u16 item = ITEM_NONE;

	#ifndef UNBOUND
	if (MetatileBehavior_IsFastCurrent(metatileBehavior))
	{
		if (
		#ifdef FLAG_BOUGHT_ADM
		FlagGet(FLAG_BOUGHT_ADM) ||
		#endif
		#ifdef FLAG_SANDBOX_MODE
		FlagGet(FLAG_SANDBOX_MODE) ||
		#endif
		PartyHasMonWithFieldMovePotential(MOVE_SURF, item, SHOULDNT_BE_SURFING) < PARTY_SIZE)
			return EventScript_CurrentTooFast;
	}
	else 
	#endif
	#ifdef MB_LAVA
	if (IsPlayerFacingSurfableLava())
	{
		if (HasBadgeToUseSurf())
		{
			if (!gFollowerState.inProgress || gFollowerState.flags & FOLLOWER_FLAG_CAN_SURF)
			{
				#ifdef DEBUG_HMS
				Var8004 = 0;
				return EventScript_UseLavaSurf_Debug;
				#else
				return EventScript_UseLavaSurf; //Fire-type check done in script
				#endif
			}

			return EventScript_MagmaGlistens;
		}
	}
	else
	#endif
	if (IsPlayerFacingSurfableFishableWater())
	{
		if (HasBadgeToUseSurf())
		{
			#ifdef ONLY_CHECK_ITEM_FOR_HM_USAGE
			item = ITEM_HM03_SURF;
			#endif

			u8 partyId = PartyHasMonWithFieldMovePotential(MOVE_SURF, item, SHOULDNT_BE_SURFING);

			#ifdef FLAG_BOUGHT_ADM
			if (FlagGet(FLAG_BOUGHT_ADM)
			&& (!gFollowerState.inProgress || gFollowerState.flags & FOLLOWER_FLAG_CAN_SURF))
				return EventScript_UseADMSurf;
			#endif

			#ifdef FLAG_SANDBOX_MODE
			if (FlagGet(FLAG_SANDBOX_MODE)
			&& (!gFollowerState.inProgress || gFollowerState.flags & FOLLOWER_FLAG_CAN_SURF))
				return EventScript_UseSandboxSurf;
			#endif

			if (partyId < PARTY_SIZE
			&& (!gFollowerState.inProgress || gFollowerState.flags & FOLLOWER_FLAG_CAN_SURF))
			{
				Var8004 = partyId;
				return EventScript_UseSurf;
			}

			if (IsPlayerFacingMurkyBrownWater())
				return EventScript_WaterMurkyBrown;
			else
				return EventScript_WaterDyedBlue;
		}
	}
	else if (MetatileBehavior_IsWaterfall(metatileBehavior))
	{
		if (HasBadgeToUseWaterfall())
		{
			if (IsPlayerSurfingNorthOrSouth())
			{
				#ifdef ONLY_CHECK_ITEM_FOR_HM_USAGE
				item = ITEM_HM07_WATERFALL;
				#endif

				#ifdef FLAG_BOUGHT_ADM
				if (FlagGet(FLAG_BOUGHT_ADM)
				&& (!gFollowerState.inProgress || gFollowerState.flags & FOLLOWER_FLAG_CAN_WATERFALL))
					return EventScript_UseADMWaterfall;
				#endif

				#ifdef FLAG_SANDBOX_MODE
				if (FlagGet(FLAG_SANDBOX_MODE)
				&& (!gFollowerState.inProgress || gFollowerState.flags & FOLLOWER_FLAG_CAN_WATERFALL))
					return EventScript_UseSandboxWaterfall;
				#endif

				u8 partyId = PartyHasMonWithFieldMovePotential(MOVE_WATERFALL, item, SHOULD_BE_SURFING);
				if (partyId < PARTY_SIZE
				&& (!gFollowerState.inProgress || gFollowerState.flags & FOLLOWER_FLAG_CAN_WATERFALL))
				{
					Var8004 = partyId;
					return EventScript_UseWaterfall;
				}

				return EventScript_WallOfWater;
			}
			else
				return EventScript_CannotUseWaterfall;
		}
		else
			return EventScript_WallOfWater;
	}
	else if (IsPlayerFacingRockClimbableWall())
	{
		if (HasBadgeToUseRockClimb()
		&& (!gFollowerState.inProgress || gFollowerState.flags & FOLLOWER_FLAG_CAN_ROCK_CLIMB))
		{
			#ifdef ONLY_CHECK_ITEM_FOR_HM_USAGE
			item = ITEM_HM08_ROCK_CLIMB;
			#endif

			#ifdef FLAG_BOUGHT_ADM
			if (FlagGet(FLAG_BOUGHT_ADM))
				return EventScript_UseADMRockClimb;
			#endif

			#ifdef FLAG_SANDBOX_MODE
			if (FlagGet(FLAG_SANDBOX_MODE))
				return EventScript_UseSandboxRockClimb;
			#endif

			u8 partyId = PartyHasMonWithFieldMovePotential(MOVE_ROCKCLIMB, item, 0);
			if (partyId < PARTY_SIZE)
			{
				Var8004 = partyId;
				return EventScript_UseRockClimb;
			}
		}

		return EventScript_JustRockWall;
	}
	return NULL;
}

void GetGroudonPartyIndexIn8004(void)
{
	Var8004 = PARTY_SIZE;

	#ifdef SPECIES_GROUDON
	u32 i;

	for (i = 0; i < PARTY_SIZE; ++i)
	{
		if (GetMonData(&gPlayerParty[i], MON_DATA_SPECIES2, NULL) == SPECIES_GROUDON)
		{
			Var8004 = i;
			break;
		}
	}
	#endif
}

void GetFirstNonEggIn8004(void)
{
	u32 i;
	Var8004 = 0;

	for (i = 0; i < PARTY_SIZE; ++i)
	{
		u16 species = GetMonData(&gPlayerParty[i], MON_DATA_SPECIES, NULL);
		if (species != SPECIES_NONE && species != SPECIES_EGG)
		{
			Var8004 = i;
			return;
		}
	}
}

extern void Task_UseWaterfall(u8 taskId);
static void Task_PrepareUseWaterfall(u8 taskId)
{
	struct EventObject* playerObj = &gEventObjects[GetPlayerMapObjId()];

	ScriptContext2_Enable();
	if (!EventObjectIsMovementOverridden(playerObj))
	{
		EventObjectClearHeldMovementIfFinished(playerObj);
		gTasks[taskId].data[0] = 2;
		gTasks[taskId].func = Task_UseWaterfall;
	}
}

void DoWaterfallWithNoShowMon(void)
{
	u8 taskId = CreateTask(Task_PrepareUseWaterfall, 0xFF);
	if (taskId != 0xFF)
	{
		ScriptContext2_Enable();
		gPlayerAvatar->preventStep = TRUE;
	}
}

void DoDiveWarpSkipShowMon(void)
{
	struct MapPosition mapPosition;
	PlayerGetDestCoords(&mapPosition.x, &mapPosition.y);
	dive_warp(&mapPosition, gEventObjects[gPlayerAvatar->eventObjectId].currentMetatileBehavior);
}

bool8 Waterfall3_MovePlayer(struct Task* task, struct EventObject* playerObj)
{
	EventObjectSetHeldMovement(playerObj, GetWalkNormalMovementAction(GetPlayerMovementDirection()));
	task->data[0]++;
	return FALSE;
}

bool8 TrySetupDiveDownScript(void)
{
	if (HasBadgeToUseDive()
	&& (!gFollowerState.inProgress || gFollowerState.flags & FOLLOWER_FLAG_CAN_DIVE)
	#if (defined FLAG_BOUGHT_ADM && !defined DEBUG_HMS)
	&& FlagGet(FLAG_BOUGHT_ADM)
	#endif
	&& TrySetDiveWarp() == 2)
	{
		u16 item = ITEM_NONE;
		#ifdef ONLY_CHECK_ITEM_FOR_HM_USAGE
		item = ITEM_HM05_DIVE;
		#endif

		#ifdef FLAG_BOUGHT_ADM
		if (FlagGet(FLAG_BOUGHT_ADM))
		{
			ScriptContext1_SetupScript(EventScript_UseADMDive);
			return TRUE;
		}
		#endif

		u8 partyId = PartyHasMonWithFieldMovePotential(MOVE_DIVE, item, SHOULD_BE_SURFING);
		if (partyId < PARTY_SIZE)
		{
			Var8004 = partyId;
			ScriptContext1_SetupScript(EventScript_UseDive);
		}
		else
			ScriptContext1_SetupScript(EventScript_CantDive);

		return TRUE;
	}

	return FALSE;
}

bool8 TrySetupDiveEmergeScript(void)
{
	if (HasBadgeToUseDive()
	&& (!gFollowerState.inProgress || gFollowerState.flags & FOLLOWER_FLAG_CAN_DIVE)
	&& gMapHeader.mapType == MAP_TYPE_UNDERWATER
	&& TrySetDiveWarp() == 1)
	{
		u16 item = ITEM_NONE;
		#ifdef ONLY_CHECK_ITEM_FOR_HM_USAGE
		item = ITEM_HM05_DIVE;
		#endif

		#ifdef FLAG_BOUGHT_ADM
		if (FlagGet(FLAG_BOUGHT_ADM))
		{
			ScriptContext1_SetupScript(EventScript_UseADMDiveUnderwater);
			return TRUE;
		}
		#endif

		u8 partyId = PartyHasMonWithFieldMovePotential(MOVE_DIVE, item, 0);
		if (partyId < PARTY_SIZE)
		{
			Var8004 = partyId;
			ScriptContext1_SetupScript(EventScript_UseDiveUnderwater);
		}
		else
			ScriptContext1_SetupScript(EventScript_CantSurface);

		return TRUE;
	}

	return FALSE;
}

bool8 MetatileBehavior_IsDiveable(u8 metatileBehavior)
{
	return metatileBehavior == MB_DIVEABLE;
}

bool8 MetatileBehavior_IsUnableToEmerge(u8 metatileBehavior)
{
	//In vanilla EM, tiles by default are emergable (it's stupid).
	//I changed this so now it just compares for the dive tile.
	return metatileBehavior != MB_DIVEABLE;
}

void PlayerAvatarTransition_Underwater(void)
{
	QuestLogCallUpdatePlayerSprite(4);
}

void PlayerAvatarTransition_HandleUnderwater(void)
{
	struct EventObject* player = &gEventObjects[GetPlayerMapObjId()];

	EventObjectSetGraphicsId(player, GetPlayerAvatarGraphicsIdByStateId(PLAYER_AVATAR_STATE_UNDERWATER));
	EventObjectTurn(player, player->movementDirection);
	SetPlayerAvatarStateMask(PLAYER_AVATAR_FLAG_UNDERWATER);
	
	#ifdef EVENT_OBJ_PAL_TAG_DIVE
	FindOrLoadNPCPalette(EVENT_OBJ_PAL_TAG_DIVE);
	
	u8 palSlot = FindPalRef(PalTypeNPC, EVENT_OBJ_PAL_TAG_DIVE);
	if (palSlot != 0xFF)
		gSprites[player->spriteId].oam.paletteNum = palSlot;
	#endif

	player->fieldEffectSpriteId = DoBobbingFieldEffect(player->spriteId);
}

bool8 IsUnderwater(void)
{
	return gMapHeader.mapType == MAP_TYPE_UNDERWATER;
}

u8 GetAdjustedInitialTransitionFlags(struct InitialPlayerAvatarState *playerStruct, u16 metatileBehavior, u8 mapType)
{
	if (mapType != MAP_TYPE_INDOOR && FlagGet(0x802))
		return PLAYER_AVATAR_FLAG_ON_FOOT;
	else if (mapType == MAP_TYPE_UNDERWATER)
		return PLAYER_AVATAR_FLAG_UNDERWATER;
	#ifndef UNBOUND
	else if (MetatileBehavior_IsSeafoamIsland(metatileBehavior))
		return PLAYER_AVATAR_FLAG_ON_FOOT;
	#endif
	else if (MetatileBehavior_IsSurfableWaterOrUnderwater(metatileBehavior))
		return PLAYER_AVATAR_FLAG_SURFING;
	else if (!Overworld_IsBikingAllowed())
		return PLAYER_AVATAR_FLAG_ON_FOOT;
	else if (playerStruct->transitionFlags & PLAYER_AVATAR_FLAG_BIKE)
		return PLAYER_AVATAR_FLAG_BIKE;
	else
		return PLAYER_AVATAR_FLAG_ON_FOOT;
}

void WarpFadeOutScreen(void)
{
    const struct MapHeader *header = GetDestinationWarpMapHeader();
    if (header->regionMapSectionId != gMapHeader.regionMapSectionId && MapHasPreviewScreen(header->regionMapSectionId, MPS_TYPE_CAVE))
        FadeScreen(FADE_TO_BLACK, 0);
    else
    {
        switch (MapTransitionIsEnter(GetCurrentMapType(), header->mapType))
        {
        case FALSE:
            FadeScreen(FADE_TO_BLACK, 0);
            break;
        case TRUE:
			gDontFadeWhite = TRUE; //Prevent DNS issues at night
            FadeScreen(FADE_TO_WHITE, 0);
            break;
        }
    }
}

//Stuff to do with pressing buttons in the field//
static const u8* sRegisteredItemStringVars[][2] =
{
	{gStringVar7, gText_RegisteredItemSelectButton},
	{gStringVar8, gText_RegisteredItemLButton},
	{gStringVar9, gText_RegisteredItemRButton},
	{gStringVarA, gText_RegisteredItemStartButton},
	{gStringVarB, gText_RegisteredItemLeftButton},
	{gStringVarC, gText_RegisteredItemRightButton},
};

void FieldCheckIfPlayerPressedLButton(struct FieldInput* input, u16 newKeys)
{
	if (newKeys & L_BUTTON)
		input->pressedLButton = TRUE;
}

bool8 ProcessNewFieldPlayerInput(struct FieldInput* input)
{
	if (IsDexNavHudActive())
		return FALSE; //Can't force close this

	if (input->pressedSelectButton && UseRegisteredKeyItemOnField())
    {
        gInputToStoreInQuestLogMaybe.pressedSelectButton = TRUE;
        return TRUE;
    }

	if (input->pressedLButton && StartLButtonFunc())
	{
		gInputToStoreInQuestLogMaybe.pressedRButton = TRUE;
		return TRUE;
	}

	if (input->pressedRButton && JOY_NEW(R_BUTTON) //Not when held!
	&& StartRButtonFunc())
	{
		gInputToStoreInQuestLogMaybe.pressedRButton = TRUE;
		return TRUE;
	}

	return FALSE;
}

void UseRegisteredItem(u16 registeredItem)
{
	u8 taskId;

	if (IsMapNamePopupTaskActive())
	{
		ChangeBgY(0, 0, 0);
		DismissMapNamePopup();
	}

	ScriptContext2_Enable();
	FreezeEventObjects();
	HandleEnforcedLookDirectionOnPlayerStopMoving();
	StopPlayerAvatar();
	Var800E = registeredItem;
	taskId = CreateTask(ItemId_GetFieldFunc(registeredItem), 8);
	if (taskId != 0xFF)
		gTasks[taskId].data[3] = 1;
}

static bool8 UseRegisteredKeyItemOnField(void)
{
	u8 i, numRegisteredItems;
	u16 registeredItem;

	if (InUnionRoom()
	#ifdef FLAG_SYS_BAG_HIDE
	|| FlagGet(FLAG_SYS_BAG_HIDE) //Can't use item with no bag
	#endif
	)
		return FALSE;

	DismissMapNamePopup();
	ChangeBgY(0, 0, 0);

	for (i = 0, numRegisteredItems = 0, registeredItem = ITEM_NONE; i < REGISTERED_ITEM_COUNT; ++i)
	{
		if (gSaveBlock1->registeredItems[i] != ITEM_NONE)
		{
			registeredItem = gSaveBlock1->registeredItems[i];
			if (CheckBagHasItem(registeredItem, 1) == 0)
				gSaveBlock1->registeredItems[i] = ITEM_NONE; //Don't have item so remove it from list
			else
			{
				u8* stringVar = (u8*) sRegisteredItemStringVars[numRegisteredItems][0];
				StringCopy(stringVar, sRegisteredItemStringVars[numRegisteredItems][1]);
				StringAppend(stringVar, ItemId_GetName(gSaveBlock1->registeredItems[i]));

				gMultiChoice[numRegisteredItems].name = stringVar;
				gMultiChoice[numRegisteredItems].id = numRegisteredItems;
				numRegisteredItems++;
			}

			CompactRegisteredItems();
		}
	}

	if (registeredItem != ITEM_NONE)
	{
		if (numRegisteredItems == 1) //Only 1 item registered
		{
			if (CheckBagHasItem(registeredItem, 1) > 0)
			{
				UseRegisteredItem(registeredItem);
				return TRUE;
			}

			RemoveRegisteredItem(registeredItem);
		}
		else //Pull up list offering multiple items
		{
			Var8004 = numRegisteredItems;
			ScriptContext1_SetupScript(EventScript_ShowSelectItems);
			return TRUE;
		}
	}

	ScriptContext1_SetupScript(EventScript_BagItemCanBeRegistered);
	return TRUE;
}

void Task_UseChosenRegisteredItem(u8 taskId)
{
	if (!ScriptContext2_IsEnabled())
	{
		if (gSpecialVar_LastResult < Var8004) //Didn't cancel
			UseRegisteredItem(gSaveBlock1->registeredItems[gSpecialVar_LastResult]);

		DestroyTask(taskId);
	}
	else
	{
		//Check button combos for quick access
		u16 usedItem = ITEM_NONE;
	
		if (JOY_NEW(A_BUTTON | B_BUTTON))
			usedItem = ITEM_NONE; //Prevent bug with pressing A/B at the same time as choosing an item
		else if (JOY_NEW(SELECT_BUTTON))
			usedItem = gSaveBlock1->registeredItems[0];
		else if (JOY_NEW(L_BUTTON))
			usedItem = gSaveBlock1->registeredItems[1];
		else if (JOY_NEW(R_BUTTON))
			usedItem = gSaveBlock1->registeredItems[2];
		else if (JOY_NEW(START_BUTTON))
			usedItem = gSaveBlock1->registeredItems[3];
		else if (JOY_NEW(DPAD_LEFT))
			usedItem = gSaveBlock1->registeredItems[4];
		else if (JOY_NEW(DPAD_RIGHT))
			usedItem = gSaveBlock1->registeredItems[5];

		if (usedItem != ITEM_NONE)
		{
			u8 multichoiceTaskId = FindTaskIdByFunc(Task_MultichoiceMenu_HandleInput);
			if (multichoiceTaskId != 0xFF)
			{
				DestroyScriptMenuWindow(gTasks[multichoiceTaskId].data[6]);
				DestroyTask(multichoiceTaskId);
			}

			HideFieldMessageBox();
			UseRegisteredItem(usedItem);
			DestroyTask(taskId);
		}
	}
}

void UseChosenRegisteredItem(void)
{
	CreateTask(Task_UseChosenRegisteredItem, 0xFF);
}

#ifdef GEN_4_PLAYER_RUNNING_FIX
const union AnimCmd gEventObjectImageAnim_RunSouth[] =
{
	ANIMCMD_FRAME( 9, 3),
	ANIMCMD_FRAME(10, 5),
	ANIMCMD_FRAME( 9, 3),
	ANIMCMD_FRAME(11, 5),
	ANIMCMD_JUMP(0),
};

const union AnimCmd gEventObjectImageAnim_RunNorth[] =
{
	ANIMCMD_FRAME(12, 2),
	ANIMCMD_FRAME(13, 6),
	ANIMCMD_FRAME(12, 2),
	ANIMCMD_FRAME(14, 6),
	ANIMCMD_JUMP(0),
};

const union AnimCmd gEventObjectImageAnim_RunWest[] =
{
	ANIMCMD_FRAME(15, 3),
	ANIMCMD_FRAME(16, 5),
	ANIMCMD_FRAME(15, 3),
	ANIMCMD_FRAME(17, 5),
	ANIMCMD_JUMP(0),
};

const union AnimCmd gEventObjectImageAnim_RunEast[] =
{
	ANIMCMD_FRAME(15, 3, .hFlip = TRUE),
	ANIMCMD_FRAME(16, 5, .hFlip = TRUE),
	ANIMCMD_FRAME(15, 3, .hFlip = TRUE),
	ANIMCMD_FRAME(17, 5, .hFlip = TRUE),
	ANIMCMD_JUMP(0),
};
#else
const union AnimCmd gEventObjectImageAnim_RunSouth[] =
{
	ANIMCMD_FRAME(9, 5),
	ANIMCMD_FRAME(10, 3),
	ANIMCMD_FRAME(9, 5),
	ANIMCMD_FRAME(11, 3),
	ANIMCMD_JUMP(0),
};

const union AnimCmd gEventObjectImageAnim_RunNorth[] =
{
	ANIMCMD_FRAME(12, 5),
	ANIMCMD_FRAME(13, 3),
	ANIMCMD_FRAME(12, 5),
	ANIMCMD_FRAME(14, 3),
	ANIMCMD_JUMP(0),
};

const union AnimCmd gEventObjectImageAnim_RunWest[] =
{
	ANIMCMD_FRAME(15, 5),
	ANIMCMD_FRAME(16, 3),
	ANIMCMD_FRAME(15, 5),
	ANIMCMD_FRAME(17, 3),
	ANIMCMD_JUMP(0),
};

const union AnimCmd gEventObjectImageAnim_RunEast[] =
{
	ANIMCMD_FRAME(15, 5, .hFlip = TRUE),
	ANIMCMD_FRAME(16, 3, .hFlip = TRUE),
	ANIMCMD_FRAME(15, 5, .hFlip = TRUE),
	ANIMCMD_FRAME(17, 3, .hFlip = TRUE),
	ANIMCMD_JUMP(0),
};
#endif

