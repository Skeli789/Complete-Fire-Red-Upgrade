#include "defines.h"
#include "defines_battle.h"
#include "../include/battle_setup.h"
#include "../include/daycare.h"
#include "../include/event_data.h"
#include "../include/event_object_movement.h"
#include "../include/field_effect.h"
#include "../include/field_control_avatar.h"
#include "../include/field_player_avatar.h"
#include "../include/field_poison.h"
#include "../include/fieldmap.h"
#include "../include/fldeff_misc.h"
#include "../include/item.h"
#include "../include/link.h"
#include "../include/m4a.h"
#include "../include/map_scripts.h"
#include "../include/metatile_behavior.h"
#include "../include/overworld.h"
#include "../include/party_menu.h"
#include "../include/quest_log.h"
#include "../include/safari_zone.h"
#include "../include/script.h"
#include "../include/sound.h"

#include "../include/constants/flags.h"
#include "../include/constants/items.h"
#include "../include/constants/maps.h"
#include "../include/constants/metatile_behaviors.h"
#include "../include/constants/region_map_sections.h"
#include "../include/constants/songs.h"
#include "../include/constants/trainers.h"
#include "../include/constants/trainer_classes.h"

#include "../include/new/item.h"
#include "../include/new/follow_me.h"
#include "../include/new/frontier.h"
#include "../include/new/util.h"
#include "../include/new/multi.h"
#include "../include/new/overworld.h"
#include "../include/new/overworld_data.h"
#include "../include/new/party_menu.h"
#include "../include/new/wild_encounter.h"
/*
overworld.c
	functions for anything regarding the overworld, such as trainer spotting, whiteout, step counters, etc.
*/

extern const u16 gClassBasedTrainerEncounterBGM[NUM_TRAINER_CLASSES];

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
static const u8* GetCustomWalkingScript(void);
static bool8 SafariZoneTakeStep(void);
static bool8 IsRunningDisabledByFlag(void);

#ifdef VAR_DEFAULT_WALKING_SCRIPT
//Table full of pointers to custom walking scripts
static const u8* const sDefaultWalkingScripts[] =
{
	NULL,
	NULL,
	NULL,
	NULL,
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


	[MB_TELEVISION] = (void*) 0x81A764E,
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
};

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

u8 CheckForTrainersWantingBattle(void) {
	//if (IsQuestLogActive())
	//	return FALSE;

	if (FuncIsActiveTask(Task_OverworldMultiTrainers))
		return FALSE;

	u8 viableMons = ViableMonCount(gPlayerParty);
	if (viableMons == 0) //NPC's won't challenge you if, for some reason, you have no Pokemon
		return FALSE;

	ExtensionState.spotted.count = 0;

	for (u8 eventObjId = 0; eventObjId < MAP_OBJECTS_COUNT; ++eventObjId) //For each NPC on the map
	{
		if (!gEventObjects[eventObjId].active || gEventObjects[eventObjId].isPlayer)
			continue;

		if (CheckTrainerSpotting(eventObjId))
		{
			if (viableMons < 2)
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
			TrainerApproachPlayer(&gEventObjects[eventObjId], TrainerCanApproachPlayer(&gEventObjects[eventObjId]) - 1);
			ScriptContext1_SetupScript(GetEventObjectScriptPointerByEventObjectId(eventObjId));
			ScriptCall(gScriptEnv1, EventScript_SetUpNPCSpotting);
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
	&& (FlagGet(FLAG_TRAINER_FLAG_START + T1_READ_16(scriptPtr + 2)) || FlagGet(FLAG_TRAINER_FLAG_START + T1_READ_16(scriptPtr + 4))))
		return FALSE; //If either trainer flag is set

	if (GetTrainerFlagFromScriptPointer(scriptPtr)) //Trainer has already been beaten
		return FALSE;

	struct EventObject* trainerObj = &gEventObjects[eventObjId];
	bool8 canApproach = TrainerCanApproachPlayer(trainerObj);

	if (canApproach)
	{
		switch (battleType) {
			case TRAINER_BATTLE_DOUBLE:
			case TRAINER_BATTLE_REMATCH_DOUBLE:
			case TRAINER_BATTLE_CONTINUE_SCRIPT_DOUBLE:
			case TRAINER_BATTLE_TWO_OPPONENTS:
				if (ViableMonCount(gPlayerParty) < 2)
					return FALSE;
				break;
			case TRAINER_BATTLE_TAG:
			case TRAINER_BATTLE_MULTI:
				return FALSE;  //You can't be stopped by someone using the tag battle feature
		}

		struct TrainerSpotted trainer = {eventObjId, canApproach, (u8*) scriptPtr};
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

static bool8 CheckNPCSpotting(u8 eventObjId)
{
	#ifdef NON_TRAINER_SPOTTING
	const u8* scriptPtr = GetEventObjectScriptPointerByEventObjectId(eventObjId); //Get NPC Script Pointer from its Object Id
	u16 flag = GetEventObjectTemplateByLocalIdAndMap(gEventObjects[eventObjId].localId,
													 gEventObjects[eventObjId].mapNum,
													 gEventObjects[eventObjId].mapGroup)->flagId2; //Special flag just for this

	if (scriptPtr != NULL && scriptPtr[0] != SCRCMD_TRAINERBATTLE //NPC has a regular script
	&& (flag == 0 || !FlagGet(flag)))
	{
		return TrainerCanApproachPlayer(&gEventObjects[eventObjId]);
	}
	#else
		++eventObjId;
	#endif

	return FALSE;
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
			TrainerBattleLoadArgs(sOrdinaryNoIntroBattleParams, data);
			return EventScript_DoTrainerBattle;

		case TRAINER_BATTLE_DOUBLE:
			TrainerBattleLoadArgs(sDoubleBattleParams, data);
			SetMapVarsToTrainer();
			return EventScript_TryDoDoubleTrainerBattle;

		case TRAINER_BATTLE_REMATCH:
			//QuestLogRemtachBattleStore();
			TrainerBattleLoadArgs(sOrdinaryBattleParams, data);
			SetMapVarsToTrainer();
			gTrainerBattleOpponent_A = GetRematchTrainerId(gTrainerBattleOpponent_A);
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
			gTrainerBattleOpponent_A = GetRematchTrainerId(gTrainerBattleOpponent_A);
			return EventScript_TryDoDoubleRematchBattle;

		case TRAINER_BATTLE_OAK_TUTORIAL:
			#ifdef TUTORIAL_BATTLES
				if (Var8000 != 0xFEFE)
					TrainerBattleLoadArgs(sOakTutorialParams, data);
				else //Regular trainer battle 9
			#endif
					TrainerBattleLoadArgs(sContinueLostBattleParams, data);
			return EventScript_DoTrainerBattle;

		case TRAINER_BATTLE_MULTI:
			TrainerBattleLoadArgs(sMultiBattleParams, data);
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

		case TRAINER_BATTLE_TAG:
			TrainerBattleLoadArgs(sTagBattleParams, data);
			VarSet(VAR_PARTNER, gTrainerBattlePartner);
			VarSet(VAR_PARTNER_BACKSPRITE, sPartnerBackSpriteId);
			FlagSet(FLAG_TAG_BATTLE);
			return EventScript_DoTrainerBattle;

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
	return sTrainerBattleMode == TRAINER_BATTLE_MULTI || sTrainerBattleMode == TRAINER_BATTLE_TWO_OPPONENTS || FlagGet(FLAG_TWO_OPPONENTS);
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
			default:
				gBattleTypeFlags |= BATTLE_TYPE_BATTLE_TOWER;
				break;
		}

		u16 tier = VarGet(VAR_BATTLE_FACILITY_TIER);
		if (IsCamomonsTier(tier))
			gBattleTypeFlags |= BATTLE_TYPE_CAMOMONS;
		else if (tier == BATTLE_TOWER_BENJAMIN_BUTTERFREE)
			gBattleTypeFlags |= BATTLE_TYPE_BENJAMIN_BUTTERFREE;
		else if (tier == BATTLE_TOWER_MEGA_BRAWL)
			gBattleTypeFlags |= BATTLE_TYPE_MEGA_BRAWL;

		switch (VarGet(VAR_BATTLE_FACILITY_BATTLE_TYPE)) {
			case BATTLE_TOWER_DOUBLE:
			case BATTLE_TOWER_DOUBLE_RANDOM:
				gBattleTypeFlags |= BATTLE_TYPE_DOUBLE;
				break;
			case BATTLE_TOWER_MULTI:
			case BATTLE_TOWER_MULTI_RANDOM:
				if (gTrainerBattleOpponent_A == FRONTIER_BRAIN_TID) //Frontier Brains fight alone
					gBattleTypeFlags |= (BATTLE_TYPE_DOUBLE | BATTLE_TYPE_INGAME_PARTNER /* | BATTLE_TYPE_MULTI*/);
				else
					gBattleTypeFlags |= (BATTLE_TYPE_DOUBLE | BATTLE_TYPE_TWO_OPPONENTS | BATTLE_TYPE_INGAME_PARTNER);
				break;
			case BATTLE_TOWER_LINK_MULTI:
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
	}

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
	if (sTrainerBattleMode == TRAINER_BATTLE_TWO_OPPONENTS
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
	if (gTrainerBattleOpponent_B)
		FlagSet(FLAG_TRAINER_FLAG_START + gTrainerBattleOpponent_B);
	FlagSet(FLAG_TRAINER_FLAG_START + gTrainerBattleOpponent_A);
}

//Script Callasms
enum
{
	GoDown,
	GoUp,
	GoLeft,
	GoRight
};


void AllowTrainerIncrementation(void)
{
	ExtensionState.multiTaskStateHelper = TRUE;
}

void MoveSecondNPCForTwoOpponentSighting(void)
{
	u8 localId, obj;
	if (gEventObjects[ExtensionState.spotted.trainers[0].id].localId == ExtensionState.spotted.firstTrainerNPCId)
		localId = ExtensionState.spotted.secondTrainerNPCId;
	else
		localId = ExtensionState.spotted.firstTrainerNPCId;

	obj = GetEventObjectIdByLocalId(localId);
	Var8005 = localId;

	u16 playerX = gEventObjects[0].currentCoords.x;
	u16 playerY = gEventObjects[0].currentCoords.y;
	u16 npcX = gEventObjects[obj].currentCoords.x;
	u16 npcY = gEventObjects[obj].currentCoords.y;

	gSpecialVar_LastResult = 0xFFFF;
	switch(gEventObjects[obj].facingDirection) {
		case 0:
		case 1:
			if (npcY != playerY - 1)
				gSpecialVar_LastResult = GoDown;
			break;
		case 2:
			if (npcY != playerY + 1)
				gSpecialVar_LastResult = GoUp;
			break;
		case 3:
			if (npcX != playerX + 1)
				gSpecialVar_LastResult = GoLeft;
			break;
		case 4:
			if (npcX != playerX - 1)
				gSpecialVar_LastResult = GoRight;
			break;
		default:
			if (npcY != playerY - 1)
				gSpecialVar_LastResult = GoDown;
			break;
	}
}

void LoadProperIntroSpeechForTwoOpponentSighting(void)
{
	switch (Var8000) {
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
		gSelectedEventObject = GetEventObjectIdByLocalId(ExtensionState.spotted.secondTrainerNPCId);
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

	u16 currentX = gEventObjects[gSelectedEventObject].currentCoords.x;
	u16 currentY = gEventObjects[gSelectedEventObject].currentCoords.y;
	u16 toX = gEventObjects[newObj].currentCoords.x;
	u16 toY = gEventObjects[newObj].currentCoords.y;

	GetProperDirection(currentX, currentY, toX, toY);
	Var8005 = 0x7F; //Camera
}

static u8 GetPlayerMapObjId(void)
{
	return gPlayerAvatar->eventObjectId;
}

static const u8 sMovementToDirection[] =
{
	[GoDown] = DIR_SOUTH,
	[GoUp] = DIR_NORTH,
	[GoLeft] = DIR_WEST,
	[GoRight] = DIR_EAST,
};

static u8 GetNPCDirectionFaceToPlayer(u8 eventObj)
{
	u8 playerObjId = GetPlayerMapObjId();
	u16 playerX = gEventObjects[playerObjId].currentCoords.x;
	u16 playerY = gEventObjects[playerObjId].currentCoords.y;
	u16 npcX = gEventObjects[eventObj].currentCoords.x;
	u16 npcY = gEventObjects[eventObj].currentCoords.y;

	if (GetProperDirection(playerX, playerY, npcX, npcY))
		return GetOppositeDirection(sMovementToDirection[gSpecialVar_LastResult]);

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
			gSpecialVar_LastResult = GoDown;
		else
			gSpecialVar_LastResult = GoUp;

		ret = TRUE;
	}
	else if (currentY == toY)
	{
		if (currentX < toX)
			gSpecialVar_LastResult = GoRight;
		else
			gSpecialVar_LastResult = GoLeft;

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
					gSpecialVar_LastResult = GoDown;
				else
					gEventObjects[followerObjid].currentCoords.y = playerY - offset;
			}
		}
		else // Player Y <= npcY
		{
			if (playerY != npcY - offset) //Player and follower are not 1 tile apart
			{
				if (Var8000 == 0)
					gSpecialVar_LastResult = GoUp;
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
					gSpecialVar_LastResult = GoRight;
				else
					gEventObjects[followerObjid].currentCoords.x = playerX - offset;
			}
		}
		else // Player X <= npcX
		{
			if (playerX != npcX - offset) //Player and follower are not 1 tile apart
			{
				if (Var8000 == 0)
					gSpecialVar_LastResult = GoLeft;
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

	UpdateHappinessStepCounter();
	UpdateJPANStepCounters();
	if (!(gPlayerAvatar->flags & PLAYER_AVATAR_FLAG_6) && !MetatileBehavior_IsForcedMovementTile(metatileBehavior))
	{
		if (CheckVSSeeker() == TRUE)
		{
			ScriptContext1_SetupScript(EventScript_VSSeeker);
			return TRUE;
		}
		if (UpdatePoisonStepCounter() == TRUE)
		{
			ScriptContext1_SetupScript(EventScript_Poison);
			return TRUE;
		}
		if (ShouldEggHatch())
		{
			IncrementGameStat(GAME_STAT_HATCHED_EGGS);
			ScriptContext1_SetupScript(EventScript_EggHatch);
			return TRUE;
		}

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

static const u8* GetCustomWalkingScript(void)
{
	#ifdef VAR_DEFAULT_WALKING_SCRIPT
	if (gWalkingScript >= (u8*) 0x8000000) //A real script
		return gWalkingScript;

	u8 scriptInd = VarGet(VAR_DEFAULT_WALKING_SCRIPT);
	if (scriptInd != 0 || scriptInd > ARRAY_COUNT(sDefaultWalkingScripts))
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

bool8 TryRunOnFrameMapScript(void)
{
	TryUpdateSwarm();

	if (gQuestLogMode != 3)
	{
		u8 *ptr = MapHeaderCheckScriptTable(MAP_SCRIPT_ON_FRAME_TABLE);

		if (!ptr)
			return FALSE;

		ScriptContext1_SetupScript(ptr);
		return TRUE;
	}

	return FALSE;
}

// Whiteout Hack
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

const u8* LoadProperWhiteoutString(const u8* string)
{
#ifdef SET_HEALING_PLACE_HACK
	if (string == sText_ScurriedHome)
	{
		if (gSaveBlock1->location.mapNum != MAP_NUM(PLAYER_HOME)
		||  gSaveBlock1->location.mapGroup != MAP_GROUP(PLAYER_HOME))
			string = gText_ScurriedToNearestHealer;
	}
#endif
	return string;
}

bool8 IsAutoRunEnabled(void)
{
	#ifdef FLAG_AUTO_RUN
		return FlagGet(FLAG_AUTO_RUN);
	#else
		return FALSE;
	#endif
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

	return gMapHeader.isBikeable
#ifdef BIKE_ON_ANY_NON_INSIDE_MAP
	|| !IsMapTypeIndoors(GetCurrentMapType());
#endif
	;
}

bool8 CanUseEscapeRopeOnCurrMap(void)
{
	if (gFollowerState.inProgress && !(gFollowerState.flags & FOLLOWER_FLAG_CAN_LEAVE_ROUTE))
		return FALSE;

	return (gMapHeader.escapeRope & 1) != 0;
}

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
		if (GetMonData(mon, MON_DATA_SANITY_HAS_SPECIES, NULL) && GetMonPrimaryAilments(mon->condition) == AILMENT_PSN)
		{
			hp = mon->hp;

			#ifdef POISON_1_HP_SURVIVAL
				if (hp == 1 || --hp == 1)
				{
					mon->hp = hp;
					mon->condition = STATUS1_NONE;
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
		return FLDPSN_NONE;
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
		DisplayItemMessage(taskId, 1, gStringVar4, bag_menu_inits_lists_menu);
	}
};

//Updated Repel - hook at 080830B8 via r1
bool8 UpdateRepelCounter(void)
{
	u8 steps = VarGet(VAR_REPEL_STEP_COUNT); //0x4020

	if (steps != 0)
	{
		steps--;
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
		return GetCurrentRegionMapSectionId() == MAPSEC_CINDER_VOLCANO;
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
			|| mapSec == MAPSEC_ROUTE_10
			|| mapSec == MAPSEC_AUBURN_WATERWAY;
	#else
		return FALSE;
	#endif
}

bool8 IsCurrentAreaWinter(void)
{
	#ifdef UNBOUND
		u8 mapSec = GetCurrentRegionMapSectionId();
		return mapSec == MAPSEC_FROZEN_HEIGHTS
			|| mapSec == MAPSEC_ROUTE_1
			|| mapSec == MAPSEC_BELLIN_TOWN
			|| mapSec == MAPSEC_ROUTE_8
			|| mapSec == MAPSEC_BLIZZARD_CITY
			|| mapSec == MAPSEC_FROZEN_FOREST;
	#else
		return FALSE;
	#endif
}

bool8 IsCurrentAreaDarkerCave(void)
{
	#ifdef UNBOUND
		u8 mapSec = GetCurrentRegionMapSectionId();
		return mapSec == MAPSEC_ICICLE_CAVE
			|| mapSec == MAPSEC_VALLEY_CAVE
			|| mapSec == MAPSEC_FROST_MOUNTAIN
			|| mapSec == MAPSEC_THUNDERCAP_MOUNTAIN
			|| mapSec == MAPSEC_DISTORTION_WORLD;
	#else
		return FALSE;
	#endif
}

bool8 InTanobyRuins(void)
{
	#ifdef TANOBY_RUINS_ENABLED
		if (FlagGet(FLAG_TANOBY_KEY))
		{
			u8 mapSec = GetCurrentRegionMapSectionId();
			return mapSec >= MAPSEC_MONEAN_CHAMBER && mapSec <= MAPSEC_VIAPOIS_CHAMBER;
		}
	#endif

	return FALSE;
}

void PlayGrassFootstepNoise(void)
{
	#ifdef UNBOUND
		PlaySE(SE_LEAVES);
	#elif defined FOOTSTEP_NOISES
		PlaySE(SE_LEAVES);
	#endif
}

void PlaySandFootstepNoise(void)
{
	#ifdef UNBOUND
		PlaySE(SE_MUD_SLAP);
	#elif defined FOOTSTEP_NOISES
		PlaySE(SE_MUD_SLAP);
	#endif
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

const u8* GetInteractedMetatileScript(unusedArg int position, u8 metatileBehavior, u8 direction)
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

static bool8 MetatileBehavior_IsRockClimbableWall(u8 behaviour)
{
	return behaviour == MB_ROCK_CLIMB_WALL;
}

bool8 IsPlayerFacingRockClimbableWall(void)
{
	struct EventObject *playerEventObj = &gEventObjects[gPlayerAvatar->eventObjectId];
	s16 x = playerEventObj->currentCoords.x;
	s16 y = playerEventObj->currentCoords.y;

	MoveCoords(playerEventObj->facingDirection, &x, &y);
	return MetatileBehavior_IsRockClimbableWall(MapGridGetMetatileBehaviorAt(x, y));
}

void ShouldRockClimbContinue(void)
{
	gSpecialVar_LastResult = IsPlayerFacingRockClimbableWall();
}

void ShouldRockClimbContinueDiagonally(void)
{
	#ifdef UNBOUND
	struct EventObject *playerEventObj = &gEventObjects[gPlayerAvatar->eventObjectId];
	s16 x = playerEventObj->currentCoords.x;
	s16 y = playerEventObj->currentCoords.y;

	MoveCoords(playerEventObj->facingDirection, &x, &y);

	if (MetatileBehavior_IsRockClimbableWall(MapGridGetMetatileBehaviorAt(x, y + 1)))
		gSpecialVar_LastResult = 2; //Move diagonal up
	else if (y != 0 && MetatileBehavior_IsRockClimbableWall(MapGridGetMetatileBehaviorAt(x, y - 1)))
		gSpecialVar_LastResult = 1; //Move diagonal down
	else
	#endif
		gSpecialVar_LastResult = 0;
}

u8 PartyHasMonWithFieldMovePotential(u16 move, unusedArg u16 item, u8 surfingType)
{
	bool8 isSurfing = TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_SURFING);

	if (surfingType == 0
	|| (surfingType == SHOULDNT_BE_SURFING && !isSurfing)
	|| (surfingType == SHOULD_BE_SURFING && isSurfing))
	{
		#ifdef UNBOUND
		bool8 hasHM = CheckBagHasItem(item, 1) > 0;
		#endif

		for (u32 i = 0; i < PARTY_SIZE; ++i)
		{
			struct Pokemon* mon = &gPlayerParty[i];

			if (GetMonData(mon, MON_DATA_SPECIES, NULL) != SPECIES_NONE
			&& !GetMonData(mon, MON_DATA_IS_EGG, NULL))
			{
				if (MonKnowsMove(mon, move))
					return i;

				#ifdef UNBOUND
				if (hasHM && CanMonLearnTMTutor(mon, item, 0) == CAN_LEARN_MOVE)
					return i;
				#endif
			}
		}
	}

	return PARTY_SIZE;
}

#ifdef MB_LAVA
static bool8 MetatileBehavior_IsLava(u8 behaviour)
{
	return behaviour == MB_LAVA;
}
#endif

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

const u8* GetInteractedWaterScript(unusedArg u32 unused1, u8 metatileBehavior, unusedArg u8 direction)
{
	u16 item = ITEM_NONE;

	#ifdef UNBOUND
	if (FALSE) {}
	#else
	if (MetatileBehavior_IsFastCurrent(metatileBehavior))
	{
		if (PartyHasMonWithFieldMovePotential(MOVE_SURF, item, SHOULDNT_BE_SURFING) < PARTY_SIZE)
			return SystemScript_CurrentTooFast;
	}
	#endif
	#ifdef MB_LAVA
	else if (IsPlayerFacingSurfableLava())
	{
		if (HasBadgeToUseSurf())
		{
			if (!gFollowerState.inProgress || gFollowerState.flags & FOLLOWER_FLAG_CAN_SURF)
				return SystemScript_UseLavaSurf; //Fire-type check done in script

			return SystemScript_MagmaGlistens;
		}
	}
	#endif
	else if (IsPlayerFacingSurfableFishableWater())
	{
		if (HasBadgeToUseSurf())
		{
			#ifdef UNBOUND
			item = ITEM_HM03_SURF;
			#endif

			u8 partyId = PartyHasMonWithFieldMovePotential(MOVE_SURF, item, SHOULDNT_BE_SURFING);
			if (partyId < PARTY_SIZE
			&& (!gFollowerState.inProgress || gFollowerState.flags & FOLLOWER_FLAG_CAN_SURF))
			{
				Var8004 = partyId;
				return SystemScript_UseSurf;
			}

			return SystemScript_WaterDyedBlue;
		}
	}
	else if (MetatileBehavior_IsWaterfall(metatileBehavior))
	{
		if (HasBadgeToUseWaterfall())
		{
			if (IsPlayerSurfingNorth())
			{
				#ifdef UNBOUND
				item = ITEM_HM07_WATERFALL;
				#endif

				u8 partyId = PartyHasMonWithFieldMovePotential(MOVE_WATERFALL, item, SHOULD_BE_SURFING);
				if (partyId < PARTY_SIZE
				&& (!gFollowerState.inProgress || gFollowerState.flags & FOLLOWER_FLAG_CAN_WATERFALL))
				{
					Var8004 = partyId;
					return SystemScript_UseWaterfall;
				}

				return SystemScript_WallOfWater;
			}
			else
				return SystemScript_CannotUseWaterfall;
		}
	}
	else if (IsPlayerFacingRockClimbableWall())
	{
		if (HasBadgeToUseRockClimb()
		&& (!gFollowerState.inProgress || gFollowerState.flags & FOLLOWER_FLAG_CAN_ROCK_CLIMB))
		{
			#ifdef UNBOUND
			item = ITEM_HM08_ROCK_CLIMB;
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

extern const u8 EventScript_UseDive[];
extern const u8 EventScript_CantDive[];
extern const u8 EventScript_UseDiveUnderwater[];
extern const u8 EventScript_CantSurface[];
bool8 TrySetupDiveDownScript(void)
{
	if (HasBadgeToUseDive()
	&& (!gFollowerState.inProgress || gFollowerState.flags & FOLLOWER_FLAG_CAN_DIVE)
	&& TrySetDiveWarp() == 2)
	{
		u16 item = ITEM_NONE;
		#ifdef UNBOUND
		item = ITEM_HM05_DIVE;
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
		#ifdef UNBOUND
		item = ITEM_HM05_DIVE;
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
	sub_8150498(4);
}

void PlayerAvatarTransition_HandleUnderwater(void)
{
	struct EventObject* player = &gEventObjects[GetPlayerMapObjId()];

    EventObjectSetGraphicsId(player, GetPlayerAvatarGraphicsIdByStateId(PLAYER_AVATAR_STATE_UNDERWATER));
    EventObjectTurn(player, player->movementDirection);
    SetPlayerAvatarStateMask(PLAYER_AVATAR_FLAG_UNDERWATER);

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

#ifdef GEN_4_PLAYER_RUNNING_FIX
const union AnimCmd gEventObjectImageAnim_RunSouth[] =
{
	ANIMCMD_FRAME(9, 3),
	ANIMCMD_FRAME(10, 5),
	ANIMCMD_FRAME(9, 3),
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
