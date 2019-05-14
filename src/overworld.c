#include "defines.h"
#include "../include/battle_setup.h"
#include "../include/daycare.h"
#include "../include/event_data.h"
#include "../include/event_object_movement.h"
#include "../include/field_effect.h"
#include "../include/field_poison.h"
#include "../include/fldeff_misc.h"
#include "../include/link.h"
#include "../include/map_scripts.h"
#include "../include/metatile_behavior.h"
#include "../include/party_menu.h"
#include "../include/quest_log.h"
#include "../include/safari_zone.h"
#include "../include/script.h"
#include "../include/sound.h"

#include "../include/constants/flags.h"
#include "../include/constants/songs.h"
#include "../include/constants/trainers.h"
#include "../include/constants/trainer_classes.h"

#include "../include/new/frontier.h"
#include "../include/new/helper_functions.h"
#include "../include/new/multi.h"
#include "../include/new/overworld.h"
#include "../include/new/overworld_data.h"
#include "../include/new/wild_encounter.h"

extern const u16 gClassBasedTrainerEncounterBGM[NUM_TRAINER_CLASSES];

//This file's functions:
static bool8 CheckTrainerSpotting(u8 eventObjId);
static bool8 GetTrainerFlagFromScriptPointer(const u8* data);
static void Task_OverworldMultiTrainers(u8 id);
static void ConfigureTwoTrainersBattle(u8 trainerEventObjId, const u8* trainerScript);
static void SetUpTwoTrainersBattle(void);
static void InitTrainerBattleVariables(void);
static u8 GetPlayerMapObjId(void);
static bool8 GetProperDirection(u16 currentX, u16 currentY, u16 toX, u16 toY);
static void UpdateJPANStepCounters(void);
static const u8* GetCustomWalkingScript(void);
static bool8 SafariZoneTakeStep(void);
static bool8 IsRunningDisabledByFlag(void);

//Table full of pointers to custom walking scripts
const u8* const gDefaultWalkingScripts[] =
{
	NULL,
	NULL,
	NULL,
	NULL,
	//etc
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

const struct TrainerBattleParameter sTrainerBContinueScriptBattleParams[] =
{
    {&sTrainerBattleMode,            TRAINER_PARAM_LOAD_VAL_8BIT},
    {&gTrainerBattleOpponent_B,      TRAINER_PARAM_LOAD_VAL_16BIT},
    {&sTrainerEventObjectLocalId,    TRAINER_PARAM_LOAD_VAL_16BIT},
    {&sTrainerIntroSpeech_B,         TRAINER_PARAM_LOAD_VAL_32BIT},
    {&sTrainerDefeatSpeech_B,        TRAINER_PARAM_LOAD_VAL_32BIT},
    {&sTrainerVictorySpeech_B,       TRAINER_PARAM_CLEAR_VAL_32BIT},
    {&sTrainerCannotBattleSpeech,    TRAINER_PARAM_CLEAR_VAL_32BIT},
    {&sTrainerBattleScriptRetAddr_B, TRAINER_PARAM_LOAD_VAL_32BIT},
    {&sTrainerBattleEndScript,       TRAINER_PARAM_LOAD_SCRIPT_RET_ADDR},
};

const struct TrainerBattleParameter sTrainerBOrdinaryBattleParams[] =
{
    {&sTrainerBattleMode,            TRAINER_PARAM_LOAD_VAL_8BIT},
    {&gTrainerBattleOpponent_B,      TRAINER_PARAM_LOAD_VAL_16BIT},
    {&sTrainerEventObjectLocalId,    TRAINER_PARAM_LOAD_VAL_16BIT},
    {&sTrainerIntroSpeech_B,         TRAINER_PARAM_LOAD_VAL_32BIT},
    {&sTrainerDefeatSpeech_B,        TRAINER_PARAM_LOAD_VAL_32BIT},
    {&sTrainerVictorySpeech,         TRAINER_PARAM_CLEAR_VAL_32BIT},
	{&sTrainerVictorySpeech_B,       TRAINER_PARAM_CLEAR_VAL_32BIT},
    {&sTrainerCannotBattleSpeech,    TRAINER_PARAM_CLEAR_VAL_32BIT},
    {&sTrainerBattleScriptRetAddr_B, TRAINER_PARAM_CLEAR_VAL_32BIT},
    {&sTrainerBattleEndScript,       TRAINER_PARAM_LOAD_SCRIPT_RET_ADDR},
};

//trainerbattle 0xA FOE_1_ID FOE_2_ID PARTNER_ID PARTNER_BACKSPRITE_ID 0x0 DEFEAT_TEXT_A DEFEAT_TEXT_B
const struct TrainerBattleParameter sMultiBattleParams[] =
{
    {&sTrainerBattleMode,           TRAINER_PARAM_LOAD_VAL_8BIT},

    {&gTrainerBattleOpponent_A,      TRAINER_PARAM_LOAD_VAL_16BIT},
	{&gTrainerBattleOpponent_B,      TRAINER_PARAM_LOAD_VAL_16BIT},
	{&gTrainerBattlePartner,     	 TRAINER_PARAM_LOAD_VAL_16BIT},
	{&sPartnerBackSpriteId,			 TRAINER_PARAM_LOAD_VAL_16BIT},

    {&sTrainerEventObjectLocalId,    TRAINER_PARAM_LOAD_VAL_16BIT},
    {&sTrainerIntroSpeech_A,         TRAINER_PARAM_CLEAR_VAL_32BIT},
	{&sTrainerIntroSpeech_B,       	 TRAINER_PARAM_CLEAR_VAL_32BIT},

    {&sTrainerDefeatSpeech_A,        TRAINER_PARAM_LOAD_VAL_32BIT},
	{&sTrainerDefeatSpeech_B, 		 TRAINER_PARAM_LOAD_VAL_32BIT},

    {&sTrainerVictorySpeech,         TRAINER_PARAM_CLEAR_VAL_32BIT},
	{&sTrainerVictorySpeech_B,       TRAINER_PARAM_CLEAR_VAL_32BIT},
    {&sTrainerCannotBattleSpeech,    TRAINER_PARAM_CLEAR_VAL_32BIT},
    {&sTrainerBattleScriptRetAddr,   TRAINER_PARAM_CLEAR_VAL_32BIT},
	{&sTrainerBattleScriptRetAddr_B, TRAINER_PARAM_CLEAR_VAL_32BIT},
    {&sTrainerBattleEndScript,       TRAINER_PARAM_LOAD_SCRIPT_RET_ADDR},
};

//trainerbattle 0xB FOE_1_ID FOE_2_ID FOE_1_NPC_ID FOE_2_NPC_ID 0x0 INTRO_TEXT_A INTRO_TEXT_B DEFEAT_TEXT_A DEFEAT_TEXT_B CANNOT_BATTLE_TEXT
const struct TrainerBattleParameter sTwoOpponentBattleParams[] =
{
    {&sTrainerBattleMode,          	 TRAINER_PARAM_LOAD_VAL_8BIT},

    {&gTrainerBattleOpponent_A,    	 TRAINER_PARAM_LOAD_VAL_16BIT},
	{&gTrainerBattleOpponent_B,    	 TRAINER_PARAM_LOAD_VAL_16BIT},
	{&gTrainerBattlePartner,     	 TRAINER_PARAM_CLEAR_VAL_16BIT},

	{&gFirstTrainerNPCId,			 TRAINER_PARAM_LOAD_VAL_8BIT},
	{&gSecondTrainerNPCId,			 TRAINER_PARAM_LOAD_VAL_8BIT},

    {&sTrainerEventObjectLocalId,  	 TRAINER_PARAM_LOAD_VAL_16BIT},
    {&sTrainerIntroSpeech_A,       	 TRAINER_PARAM_LOAD_VAL_32BIT},
	{&sTrainerIntroSpeech_B,       	 TRAINER_PARAM_LOAD_VAL_32BIT},

    {&sTrainerDefeatSpeech_A,     	 TRAINER_PARAM_LOAD_VAL_32BIT},
	{&sTrainerDefeatSpeech_B,      	 TRAINER_PARAM_LOAD_VAL_32BIT},

    {&sTrainerVictorySpeech,       	 TRAINER_PARAM_CLEAR_VAL_32BIT},
	{&sTrainerVictorySpeech_B,       TRAINER_PARAM_CLEAR_VAL_32BIT},
    {&sTrainerCannotBattleSpeech,    TRAINER_PARAM_LOAD_VAL_32BIT},
	{&sTrainerCannotBattleSpeech_B,  TRAINER_PARAM_LOAD_VAL_32BIT},

    {&sTrainerBattleScriptRetAddr,   TRAINER_PARAM_CLEAR_VAL_32BIT},
	{&sTrainerBattleScriptRetAddr_B, TRAINER_PARAM_CLEAR_VAL_32BIT},
    {&sTrainerBattleEndScript,       TRAINER_PARAM_LOAD_SCRIPT_RET_ADDR},
};

//trainerbattle 0xC FOE_ID PARTNER_ID PARTNER_BACKSPRITE_ID 0x0 DEFEAT_TEXT_A
const struct TrainerBattleParameter sTagBattleParams[] =
{
    {&sTrainerBattleMode,           TRAINER_PARAM_LOAD_VAL_8BIT},

    {&gTrainerBattleOpponent_A,      TRAINER_PARAM_LOAD_VAL_16BIT},
	{&gTrainerBattleOpponent_B,      TRAINER_PARAM_CLEAR_VAL_16BIT},
	{&gTrainerBattlePartner,     	 TRAINER_PARAM_LOAD_VAL_16BIT},
	{&sPartnerBackSpriteId,			 TRAINER_PARAM_LOAD_VAL_16BIT},

    {&sTrainerEventObjectLocalId,    TRAINER_PARAM_LOAD_VAL_16BIT},
    {&sTrainerIntroSpeech_A,         TRAINER_PARAM_CLEAR_VAL_32BIT},
	{&sTrainerIntroSpeech_B,       	 TRAINER_PARAM_CLEAR_VAL_32BIT},

    {&sTrainerDefeatSpeech_A,        TRAINER_PARAM_LOAD_VAL_32BIT},
	{&sTrainerDefeatSpeech_B, 		 TRAINER_PARAM_CLEAR_VAL_32BIT},

    {&sTrainerVictorySpeech,         TRAINER_PARAM_CLEAR_VAL_32BIT},
	{&sTrainerVictorySpeech_B,       TRAINER_PARAM_CLEAR_VAL_32BIT},
    {&sTrainerCannotBattleSpeech,    TRAINER_PARAM_CLEAR_VAL_32BIT},
    {&sTrainerBattleScriptRetAddr,   TRAINER_PARAM_CLEAR_VAL_32BIT},
	{&sTrainerBattleScriptRetAddr_B, TRAINER_PARAM_CLEAR_VAL_32BIT},
    {&sTrainerBattleEndScript,       TRAINER_PARAM_LOAD_SCRIPT_RET_ADDR},
};

u8 CheckForTrainersWantingBattle(void) {
	if (IsQuestLogActive())
		return FALSE;

	if (FuncIsActiveTask(Task_OverworldMultiTrainers))
		return FALSE;

	if (ViableMonCount(gPlayerParty) == 0) //NPC's won't challenge you if, for some reason, you have no Pokemon
		return FALSE;

    ExtensionState.spotted.count = 0;

    for (u8 eventObjId = 0; eventObjId < MAP_OBJECTS_COUNT; ++eventObjId) //For each NPC on the map
	{
		if (!gEventObjects[eventObjId].active)
            continue;
        if (gEventObjects[eventObjId].trainerType != 1 && gEventObjects[eventObjId].trainerType != 3)
            continue;

		if (CheckTrainerSpotting(eventObjId))
		{
            if (ViableMonCount(gPlayerParty) < 2)
                break;

            // We've found enough opponents
            if (ExtensionState.spotted.count > 1)
                break;
		}
    }

	//These battle types have built in features
	if (ExtensionState.spotted.trainers[0].script[1] == TRAINER_BATTLE_TWO_OPPONENTS
	||  ExtensionState.spotted.trainers[1].script[1] == TRAINER_BATTLE_TWO_OPPONENTS)
		ExtensionState.spotted.count = 1;

	//Put extra logic here for Unbound's Claydol SQ

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
		if (battleType == TRAINER_BATTLE_DOUBLE
        || battleType == TRAINER_BATTLE_REMATCH_DOUBLE
        || battleType == TRAINER_BATTLE_CONTINUE_SCRIPT_DOUBLE
		|| battleType == TRAINER_BATTLE_TWO_OPPONENTS)
		{
			if (ViableMonCount(gPlayerParty) < 2)
				return FALSE;
		}

		if (battleType == TRAINER_BATTLE_TAG //You can't be stopped by someone using the tag battle feature
		||  battleType == TRAINER_BATTLE_MULTI)
			return FALSE;

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
				VarSet(SECOND_OPPONENT_VAR, gTrainerBattleOpponent_B);
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
			TrainerBattleLoadArgs(sOakTutorialParams, data);
			return EventScript_DoTrainerBattle;

		case TRAINER_BATTLE_MULTI:
			TrainerBattleLoadArgs(sMultiBattleParams, data);
			VarSet(SECOND_OPPONENT_VAR, gTrainerBattleOpponent_B);
			VarSet(PARTNER_VAR, gTrainerBattlePartner);
			VarSet(PARTNER_BACKSPRITE_VAR, sPartnerBackSpriteId);
			FlagSet(TAG_BATTLE_FLAG);
			FlagSet(TWO_OPPONENT_FLAG);
			return EventScript_DoTrainerBattle;

		case TRAINER_BATTLE_TWO_OPPONENTS:
			TrainerBattleLoadArgs(sTwoOpponentBattleParams, data);
			SetMapVarsToTrainer();
			VarSet(SECOND_OPPONENT_VAR, gTrainerBattleOpponent_B);
			FlagSet(TWO_OPPONENT_FLAG);
			gApproachingTrainerId = 0;
			return EventScript_TryDoTwoOpponentBattle;

		case TRAINER_BATTLE_TAG:
			TrainerBattleLoadArgs(sTagBattleParams, data);
			VarSet(PARTNER_VAR, gTrainerBattlePartner);
			VarSet(PARTNER_BACKSPRITE_VAR, sPartnerBackSpriteId);
			FlagSet(TAG_BATTLE_FLAG);
			return EventScript_DoTrainerBattle;

		default:
			if (gApproachingTrainerId == 0)
			{
				TrainerBattleLoadArgs(sOrdinaryBattleParams, data);
				SetMapVarsToTrainer();
			}
			else
			{
				TrainerBattleLoadArgs(sTrainerBOrdinaryBattleParams, data);
				VarSet(SECOND_OPPONENT_VAR, gTrainerBattleOpponent_B);
			}
			return EventScript_TryDoNormalTrainerBattle;
    }
}

static void InitTrainerBattleVariables(void)
{
    sTrainerBattleMode = 0;
    if (ExtensionState.spotted.trainers == 0)
    {
        sTrainerIntroSpeech_A = 0;
        sTrainerDefeatSpeech_A = 0;
        sTrainerBattleScriptRetAddr = 0;
		sTrainerVictorySpeech = 0;
    }
    else if (!FlagGet(TWO_OPPONENT_FLAG))
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
	if (FlagGet(BATTLE_TOWER_FLAG))
	{
		gBattleTypeFlags |= (BATTLE_TYPE_FRONTIER | BATTLE_TYPE_TRAINER);

		switch (VarGet(BATTLE_TOWER_BATTLE_TYPE)) {
			case BATTLE_TOWER_DOUBLE:
				gBattleTypeFlags |= BATTLE_TYPE_DOUBLE;
				break;
			case BATTLE_TOWER_MULTI:
				gBattleTypeFlags |= (BATTLE_TYPE_DOUBLE | BATTLE_TYPE_TWO_OPPONENTS | BATTLE_TYPE_INGAME_PARTNER);
				break;
			case BATTLE_TOWER_LINK_MULTI:
				gBattleTypeFlags |= (BATTLE_TYPE_DOUBLE | BATTLE_TYPE_TWO_OPPONENTS | BATTLE_TYPE_MULTI | BATTLE_TYPE_LINK);
				break;
		}
	}
	else
	{
		if (gApproachingTrainerId == 2 || FlagGet(TWO_OPPONENT_FLAG))
			gBattleTypeFlags = (BATTLE_TYPE_DOUBLE | BATTLE_TYPE_TWO_OPPONENTS | BATTLE_TYPE_TRAINER);
		else
			gBattleTypeFlags = (BATTLE_TYPE_TRAINER);

		if (FlagGet(TAG_BATTLE_FLAG))
			gBattleTypeFlags |= (BATTLE_TYPE_DOUBLE | BATTLE_TYPE_INGAME_PARTNER /* | BATTLE_TYPE_MULTI*/);

		#ifdef CONTINUE_LOST_BATTLES
			#ifdef TUTORIAL_BATTLES
				if (Var8000 != 0xFEFE && sTrainerBattleMode == TRAINER_BATTLE_OAK_TUTORIAL && sTrainerBattleOakTutorialHelper & 3)
					gBattleTypeFlags |= BATTLE_TYPE_OAK_TUTORIAL;
			#endif
		#else
			#ifdef TUTORIAL_BATTLES
				if (sTrainerBattleMode == TRAINER_BATTLE_OAK_TUTORIAL && sTrainerBattleOakTutorialHelper & 3)
					gBattleTypeFlags |= BATTLE_TYPE_OAK_TUTORIAL;
			#endif
		#endif

		if (FlagGet(ACTIVATE_TUTORIAL_FLAG))
		{
			gBattleTypeFlags |= BATTLE_TYPE_OAK_TUTORIAL;
			sTrainerBattleMode = TRAINER_BATTLE_OAK_TUTORIAL;
		}
	}

    gMain.savedCallback = CB2_EndTrainerBattle;
    StartTheBattle();
    ScriptContext1_Stop();
}

//Special 0x34
const u8* GetIntroSpeechOfApproachingTrainer(void)
{
	if (FlagGet(BATTLE_TOWER_FLAG))
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
        trainerId = VarGet(SECOND_OPPONENT_VAR);

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
			case TRAINER_ENCOUNTER_MUSIC_HIKER:       //11
			case TRAINER_ENCOUNTER_MUSIC_INTERVIEWER: //12
			case TRAINER_ENCOUNTER_MUSIC_RICH:        //13
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
	for (u8 eventObjId = 0; eventObjId < MAP_OBJECTS_COUNT; ++eventObjId)
	{
		if (gEventObjects[eventObjId].isPlayer)
			return eventObjId;
	}

	return 0;
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

void FollowerPositionFix(void)
{
	gSpecialVar_LastResult = 0xFFFF;

	if (!VarGet(NPC_FOLLOWING_VAR))
		return;

	Var8005 = VarGet(NPC_FOLLOWING_VAR);

	u8 playerObjId = GetPlayerMapObjId();
	u16 playerX = gEventObjects[playerObjId].currentCoords.x;
	u16 playerY = gEventObjects[playerObjId].currentCoords.y;

	for (u8 eventObjId = 0; eventObjId < MAP_OBJECTS_COUNT; ++eventObjId)
	{
		if (gEventObjects[eventObjId].active && gEventObjects[eventObjId].localId == Var8005)
		{
			u16 npcX = gEventObjects[eventObjId].currentCoords.x;
			u16 npcY = gEventObjects[eventObjId].currentCoords.y;

			if (playerX == npcX)
			{
				if (playerY > npcY)
				{
					if (playerY != npcY + 1) //Player and follower are not 1 tile apart
					{
						if (Var8000 == 0)
							gSpecialVar_LastResult = GoDown;
						else
							gEventObjects[eventObjId].currentCoords.y = playerY - 1;
					}
				}
				else // Player Y <= npcY
				{
					if (playerY != npcY - 1) //Player and follower are not 1 tile apart
					{
						if (Var8000 == 0)
							gSpecialVar_LastResult = GoUp;
						else
							gEventObjects[eventObjId].currentCoords.y = playerY + 1;
					}
				}
			}
			else //playerY == npcY
			{
				if (playerX > npcX)
				{
					if (playerX != npcX + 1) //Player and follower are not 1 tile apart
					{
						if (Var8000 == 0)
							gSpecialVar_LastResult = GoRight;
						else
							gEventObjects[eventObjId].currentCoords.x = playerX - 1;
					}
				}
				else // Player X <= npcX
				{
					if (playerX != npcX - 1) //Player and follower are not 1 tile apart
					{
						if (Var8000 == 0)
							gSpecialVar_LastResult = GoLeft;
						else
							gEventObjects[eventObjId].currentCoords.x = playerX + 1;
					}
				}
			}
		}
	}
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
	if (gWalkingScript != NULL)
		return gWalkingScript;

	u8 scriptInd = VarGet(DEFAULT_WALKING_SCRIPT);
	if (scriptInd != 0 || scriptInd > ARRAY_COUNT(gDefaultWalkingScripts))
		return gDefaultWalkingScripts[scriptInd - 1];

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

bool8 IsAutoRunEnabled(void)
{
	#ifdef AUTO_RUN_FLAG
		return FlagGet(AUTO_RUN_FLAG);
	#else
		return FALSE;
	#endif
}

static bool8 IsRunningDisabledByFlag(void)
{
	#ifdef RUNNING_ENABLED_FLAG
		return !FlagGet(RUNNING_ENABLED_FLAG);
	#else
		return FALSE;
	#endif
}

bool8 IsRunningDisallowed(u8 tile)
{
    return IsRunningDisabledByFlag() || IsRunningDisallowedByMetatile(tile)
#ifndef CAN_RUN_IN_BUILDINGS
	|| gMapHeader.mapType == MAP_TYPE_INDOOR
#endif
    ;
}

bool8 IsRunningDisallowedByMetatile(u8 tile)
{
	return MetatileBehavior_IsRunningDisallowed(tile);
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
        if (GetMonData(mon, MON_DATA_SANITY_HAS_SPECIES, NULL) && pokemon_ailments_get_primary(mon->condition) == AILMENT_PSN)
        {
            hp = mon->hp;

			#ifdef POISON_1_HP_SURVIVAL
				if (hp == 1 || --hp == 1)
				{
					mon->hp = hp;
					mon->condition = STATUS1_NONE;
					++numSurvived;
					ScriptContext1_SetupScript(SystemScript_PoisonSurvial);
					Var8004 = i;
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

//Follow Me Updates/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
static u8 GetFollowerMapObjId();
void FollowMe(struct EventObject* npc, u8 state);
static u8 FollowMe_DetermineDirection(struct EventObject* player, struct EventObject* follower);
static void PlayerLogCoordinates(struct EventObject* player);
bool8 FollowMe_CollisionExempt(struct EventObject* obstacle, struct EventObject* collider);
void CopyPlayer_Ledges(struct EventObject* npc, struct Sprite* obj, u16* ledgeFramesTbl);
static bool8 CopyPlayer_StateIsMovement(u8 state);
static u8 CopyPlayer_ReturnDelayedState(u8 direction);
static u8 DetermineFollowerState(struct EventObject* follower, u8 state, u8 direction);
void FollowMe_HandleBike();
void FollowMe_HandleSprite();
void FollowerToWater();
void FollowerNoMoveSurf();
static void SetSurfJump(struct EventObject* npc);
static void FollowMe_SetSurf(struct EventObject* npc);
static void Task_BindSurfBlobToFollower(u8 taskId);
static void FollowMe_SetUpFieldEffect(struct EventObject* npc);
static void SetFollowerSprite(u8 spriteIndex);
static u8 GetFollowerSprite();
u8 GetFollowerLocalId(void);
void CreateFollowerAvatar();
void StairsMoveFollower(struct Sprite* obj);
void FollowMe_WarpSetEnd();

struct Follower
{
	u8 objId;
	u8 currentSprite;
	u8 locked;
	u8 delayedState;
	struct
	{
		u8 id;
		u8 number;
		u8 bank;
	} map;
	bool8 warpEnd;
	struct Coords16 log;
	u8* script;
	u16 flag;
	u8 gfxId;
	bool8 inProgress;
};

extern struct Follower gFollowerState;

#define MOVEMENT_INVALID 0xFE

static u8 GetFollowerMapObjId()
{
	return gFollowerState.objId;
}

void FollowMe(struct EventObject* npc, u8 state)
{
	struct EventObject* player = &gEventObjects[GetPlayerMapObjId()];

	if (player != npc) //Only when the player moves
		return;

	if (!gFollowerState.inProgress)
		return;

	struct EventObject* follower = &gEventObjects[GetFollowerMapObjId()];

	// Check if state would cause movement
	if (CopyPlayer_StateIsMovement(state) && gFollowerState.warpEnd)
	{
		follower->invisible = FALSE;
		gFollowerState.warpEnd = 0;
	}

	u8 dir = FollowMe_DetermineDirection(player, follower);

	if (dir == DIR_NONE)
		goto RESET;

	u8 newState = DetermineFollowerState(follower, state, dir);

	if (newState == MOVEMENT_INVALID)
		goto RESET;

	EventObjectClearHeldMovementIfActive(follower);
	EventObjectSetHeldMovement(follower, newState);
	PlayerLogCoordinates(player);

RESET:
	EventObjectClearHeldMovementIfFinished(follower);
}

static u8 FollowMe_DetermineDirection(struct EventObject* player, struct EventObject* follower)
{
	s8 delta_x = follower->currentCoords.x - player->currentCoords.x;
	s8 delta_y = follower->currentCoords.y - player->currentCoords.y;

	if (delta_x < 0)
		return DIR_EAST;
	else if (delta_x > 0)
		return DIR_WEST;

	if (delta_y < 0)
		return DIR_SOUTH;
	else if (delta_y > 0)
		return DIR_NORTH;

	return DIR_NONE;
}

static void PlayerLogCoordinates(struct EventObject* player)
{
	gFollowerState.log.x = player->currentCoords.x;
	gFollowerState.log.y = player->currentCoords.y;
}


//static bool8 PlayerMoved(struct EventObject* player)
//{
//	return gFollowerState.log.x != player->currentCoords.x
//		|| gFollowerState.log.y != player->currentCoords.y;
//}


bool8 FollowMe_CollisionExempt(struct EventObject* obstacle, struct EventObject* collider)
{
	if (!gFollowerState.inProgress)
		return FALSE;

	struct EventObject* follower = &gEventObjects[GetFollowerMapObjId()];
	struct EventObject* player = &gEventObjects[GetPlayerMapObjId()];

	if (obstacle == follower && collider == player)
		return TRUE;

	return FALSE;
}

#define LEDGE_FRAMES_MULTIPLIER 2

extern void (**stepspeeds[5])(struct Sprite*, u8);
extern const u16 stepspeed_seq_length[5];

void CopyPlayer_Ledges(struct EventObject* npc, struct Sprite* obj, u16* ledgeFramesTbl)
{
	u8 speed;

	if (!gFollowerState.inProgress)
		return;

	struct EventObject* follower = &gEventObjects[GetFollowerMapObjId()];

	if (follower == npc)
		speed = gPlayerAvatar->runningState ? 3 : 1;
	else
		speed = 0;

	// Calculate the frames for the jump
	u16 frameCount = (u16) stepspeed_seq_length[speed] * LEDGE_FRAMES_MULTIPLIER;
	ledgeFramesTbl[obj->data[4]] = frameCount;

	// Call the step shifter
	u8 currentFrame = obj->data[6] / LEDGE_FRAMES_MULTIPLIER;
	stepspeeds[speed][currentFrame](obj, obj->data[3]);
}

static bool8 CopyPlayer_StateIsMovement(u8 state)
{
	return state > 3;
}

#define RETURN_STATE(state) return newState == MOVEMENT_INVALID ? state : CopyPlayer_ReturnDelayedState(direction);

static u8 CopyPlayer_ReturnDelayedState(u8 direction)
{
	u8 newState = gFollowerState.delayedState;
	gFollowerState.delayedState = 0;
	return newState + direction;
}

static u8 DetermineFollowerState(struct EventObject* follower, u8 state, u8 direction)
{
	u8 newState = MOVEMENT_INVALID;

	if (CopyPlayer_StateIsMovement(state) && gFollowerState.delayedState)
		newState = gFollowerState.delayedState + direction;

	// Clear ice tile stuff
	follower->disableAnim = FALSE; //follower->field1 &= 0xFB;

	switch (state) {
		case 0x08:
		case 0x09:
		case 0x0A:
		case 0x0B:
			// Slow walk
			RETURN_STATE(0x8 + direction);
		case 0x0C:
		case 0x0D:
		case 0x0E:
		case 0x0F:
			// Slow slow
			RETURN_STATE(0xC + direction);
		case 0x10:
		case 0x11:
		case 0x12:
		case 0x13:
			// Normal walk
			RETURN_STATE(0x10 + direction);
		case 0x14:
		case 0x15:
		case 0x16:
		case 0x17:
			// Ledge jump
			gFollowerState.delayedState = 0x14;
			RETURN_STATE(0x10 + direction);
		case 0x1d:
		case 0x1e:
		case 0x1f:
		case 0x20:
			 //Handle ice tile (some walking animation)
			 //Set a bit to freeze the follower's animation
			 //FIXME: Use a hook (at 08063E28) to set this bit
			follower->disableAnim = TRUE;
			RETURN_STATE(0x1d + direction);
		case 0x3d:
		case 0x3E:
		case 0x3F:
		case 0x40:
			// Running frames
			RETURN_STATE(0x3d + direction);
		case 0x31:
		case 0x32:
		case 0x33:
		case 0x34:
			// Bike speed
			RETURN_STATE(0x31 + direction);
		case 0x41:
		case 0x42:
		case 0x43:
		case 0x44:
			// Stairs (slow walking)
			// Running sideways on stairs does not use the slow
			// frames, so split this into two.
			if (direction < 2)
			{
				RETURN_STATE(0x41 + direction);
			}
			else
			{
				RETURN_STATE(0x3d + direction);
			}
		case 0x94:
		case 0x95:
		case 0x96:
		case 0x97:
			// Warp pad spinning movement
			RETURN_STATE(0x94 + direction);
		case 0x46:
		case 0x47:
		case 0x48:
		case 0x49:
			RETURN_STATE(0x46 + direction);
		default:
			return 0xFE;
	}

	return newState;
}

void FollowMe_HandleBike()
{
	if (gPlayerAvatar->flags & 6)
		SetFollowerSprite(1); //Bike on
	else
		SetFollowerSprite(0);
}

void FollowMe_HandleSprite()
{
	if (gPlayerAvatar->flags & 6)
		SetFollowerSprite(1);
	else
		SetFollowerSprite(0);
}

void FollowerToWater()
{
	if (!gFollowerState.inProgress)
		return;

	//Make the follower do the jump and spawn the surf head
	//right in front of the follower's location.
	SetSurfJump(&gEventObjects[GetFollowerMapObjId()]);
}

void FollowerNoMoveSurf()
{
	//Spawn surfhead under follower
	FollowMe_SetSurf(&gEventObjects[GetFollowerMapObjId()]);
}

static void SetSurfJump(struct EventObject* npc)
{
	//reset NPC movement bits
	EventObjectClearHeldMovement(npc);

	//jump animation according to direction
	u8 direction = npc->movementDirection;
	u8 jumpState = GetJumpSpecialMovementAction(direction);
	EventObjectSetHeldMovement(npc, jumpState);
	FollowMe_SetUpFieldEffect(npc);

	//adjust surf head spawn location infront of npc
	switch (direction) {
		case DIR_SOUTH:
			gFieldEffectArguments[1]++; //effect_y
			break;

		case DIR_NORTH:
			gFieldEffectArguments[1]--;
			break;

		case DIR_WEST:
			gFieldEffectArguments[0]--; //effect_x
			break;

		default: //DIR_EAST
			gFieldEffectArguments[0]++;
	};

	//execute, store obj ID in field1A and bind obj
	u8 surfBlobObjId = FieldEffectStart(FLDEFF_SURF_BLOB);
	npc->fieldEffectSpriteId = surfBlobObjId;
	u8 taskId = CreateTask(Task_BindSurfBlobToFollower, 0x1);
	gTasks[taskId].data[0] = npc->localId;
}

static void FollowMe_SetSurf(struct EventObject* npc)
{
	FollowMe_SetUpFieldEffect(npc);
	u8 surfBlobObjId = FieldEffectStart(FLDEFF_SURF_BLOB);
	npc->fieldEffectSpriteId = surfBlobObjId;

	u8 taskId = CreateTask(Task_BindSurfBlobToFollower, 0x1);
	gTasks[taskId].data[0] = npc->localId;
}

static void Task_BindSurfBlobToFollower(u8 taskId)
{
	struct EventObject* npc = &gEventObjects[GetFollowerMapObjId()];

	//Wait animation
	bool8 animStatus = EventObjectClearHeldMovementIfFinished(npc);
	if (animStatus == 0)
		return;

	//Bind objs
	BindFieldEffectToSprite(npc->fieldEffectSpriteId, 0x1);
	UnfreezeEventObjects();
	DestroyTask(taskId);
	return;
}

static void FollowMe_SetUpFieldEffect(struct EventObject* npc)
{
	//Set up gFieldEffectArguments for execution
	gFieldEffectArguments[0] = npc->currentCoords.x; 	//effect_x
	gFieldEffectArguments[1] = npc->currentCoords.y;	//effect_y
	gFieldEffectArguments[2] = npc->localId; 			//npc_id
}

static void SetFollowerSprite(u8 spriteIndex)
{
	if (!gFollowerState.inProgress)
		return;

	// Save sprite
	gFollowerState.currentSprite = spriteIndex;

	struct EventObject* follower = &gEventObjects[GetFollowerMapObjId()];
	EventObjectSetGraphicsId(follower, GetFollowerSprite());

	// Update direction to prevent graphical glitches
	EventObjectTurn(follower, follower->facingDirection);
}

static const u8 gCopyPlayerSpriteTable[] =
{
	[212] = 9, //Poke Kid -> Temporary Female Surfing Sprite
};

static u8 GetFollowerSprite()
{
	if (gFollowerState.currentSprite == 0)
		return gFollowerState.gfxId;

	return gCopyPlayerSpriteTable[gFollowerState.gfxId];
}

u8 GetFollowerLocalId(void)
{
	return gEventObjects[gFollowerState.objId].localId;
}

void CreateFollowerAvatar()
{
	struct EventObjectTemplate clone;
	struct EventObjectTemplate* npc;

	npc = GetEventObjectTemplateByLocalIdAndMap(1, 0, 3); //Load dummy data

	//Create in-memory copy of constant data
	Memcpy(&clone, npc, sizeof(struct EventObjectTemplate));

	struct EventObject* player = &gEventObjects[GetPlayerMapObjId()];
	clone.x = player->currentCoords.x - 7;
	clone.y = player->currentCoords.y - 7;
	clone.graphicsId = gFollowerState.gfxId;
	clone.script = gFollowerState.script;

	// Create NPC and store ID
	gFollowerState.objId = TrySpawnEventObjectTemplate(&clone, 0, 3, 0, 0);

	if (gFollowerState.objId == EVENT_OBJECTS_COUNT)
		gFollowerState.inProgress = FALSE; //Cancel the following because couldn't load sprite

	gEventObjects[gFollowerState.objId].invisible = TRUE;
	//gFollowerState.warpEnd = 0;
}

void StairsMoveFollower(struct Sprite* obj)
{
	struct Sprite* follower = &gSprites[gEventObjects[GetFollowerMapObjId()].spriteId];
	follower->pos2.x = obj->pos2.x;
	follower->pos2.y = obj->pos2.y;
}

void FollowMe_WarpSetEnd()
{
	struct EventObject* player = &gEventObjects[GetPlayerMapObjId()];
	struct EventObject* follower = &gEventObjects[GetFollowerMapObjId()];

	gFollowerState.warpEnd = 1;
	PlayerLogCoordinates(player);
	MoveEventObjectToMapCoords(follower, player->currentCoords.x, player->currentCoords.y);

	follower->facingDirection = player->facingDirection;
	follower->movementDirection = player->movementDirection;
}
*/

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
