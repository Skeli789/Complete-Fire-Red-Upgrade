#include "defines.h"
#include "helper_functions.h"
#include "multi.h"
#include "frontier.h"

#define SCRCMD_TRAINERBATTLE 0x5C

#define sOrdinaryBattleParams (struct TrainerBattleParameter*) 0x83C6900 //Battle Type 0 & 5
#define sContinueScriptBattleParams (struct TrainerBattleParameter*) 0x83C6948 //Battle Type 1 & 2
#define sOrdinaryNoIntroBattleParams (struct TrainerBattleParameter*) 0x83C69D8 //Battle Type 3
#define sDoubleBattleParams (struct TrainerBattleParameter*) 0x83C6990 //Battle Type 4 & 7
#define sContinueScriptDoubleBattleParams (struct TrainerBattleParameter*) 0x83C6A68 //Batttle Type 6 & 8
#define sOakTutorialParams (struct TrainerBattleParameter*) 0x83C6A20 //Batttle Type 9

#define EventScript_TryDoNormalTrainerBattle (u8*) 0x81A4EC1
#define EventScript_DoTrainerBattle (u8*) 0x81A4F21
#define EventScript_TryDoDoubleTrainerBattle (u8*) 0x81A4EE9
#define EventScript_TryDoRematchBattle (u8*) 0x81A4F3E
#define EventScript_TryDoDoubleRematchBattle (u8*) 0x81A4F73

extern const u16 gClassBasedTrainerEncounterBGM[NUM_TRAINER_CLASSES];

extern u8 Script_TrainerSpottedMulti[];
extern u8 EventScript_DoTwoOpponentBattle[];
extern u8 EventScript_TryDoTwoOpponentBattle[];

extern void CopyFrontierTrainerText(u8 whichText, u16 trainerId, u8 battlerNum);

u8 CheckForTrainersWantingBattle(void);
bool8 CheckTrainerSpotting(u8 eventObjId);
bool8 GetTrainerFlagFromScriptPointer(const u8* data);
void Task_OverworldMultiTrainers(u8 id);
void ConfigureTwoTrainersBattle(u8 trainerEventObjId, const u8* trainerScript);
void SetUpTwoTrainersBattle(void);
u8* BattleSetup_ConfigureTrainerBattle(const u8* data);
void InitTrainerBattleVariables(void);
u8* GetIntroSpeechOfApproachingTrainer(void);
const u8* GetTrainerCantBattleSpeech(void);
void BattleSetup_StartTrainerBattle(void);
void SetTrainerFlags(void);

void AllowTrainerIncrementation(void);
void PrepTrainerB(void);
void MoveSecondNPCForTwoOpponentSighting(void);
void LoadProperIntroSpeechForTwoOpponentSighting(void);
void HasOneTrainerBeenDefeated(void);
void MoveCameraToTrainerB(void);
bool8 GetProperDirection(u16 currentX, u16 currentY, u16 toX, u16 toY);
void TrainerFaceFix(void);
void FollowerPositionFix(void);


// table full of pointers to custom walking scripts
u8* gDefaultWalkingScripts[] =
{
	(u32) 0,
	(u32) 0,
	(u32) 0,
	(u32) 0,
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
const struct TrainerBattleParameter sTagBattleParams[] =
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

u8 CheckForTrainersWantingBattle(void) {
	if (IsQuestLogActive())
		return FALSE;
		
	if (FuncIsActiveTask(Task_OverworldMultiTrainers)) {
		return FALSE;
	}
	
	if (ViableMonCount(gPlayerParty) == 0) //NPC's won't challenge you if, for some reason, you have no Pokemon
		return FALSE;
	
    ExtensionState.spotted.count = 0;
	
    for (u8 eventObjId = 0; eventObjId < MAP_OBJECTS_COUNT; ++eventObjId) {

		if (!gEventObjects[eventObjId].active)
            continue;
        if (gEventObjects[eventObjId].trainerType != 1 && gEventObjects[eventObjId].trainerType != 3)
            continue;
		
		if (CheckTrainerSpotting(eventObjId)) {
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
			// TODO: reset state data
			CreateTask(Task_OverworldMultiTrainers, 0x50);
			return TRUE;
    }
    return FALSE;
}

bool8 CheckTrainerSpotting(u8 eventObjId) //Or just CheckTrainer
{
    const u8* scriptPtr = GetEventObjectScriptPointerByEventObjectId(eventObjId); //Get NPC Script Pointer from its Object Id

	if (scriptPtr[1] == TRAINER_BATTLE_TWO_OPPONENTS
	&&  (FlagGet(FLAG_TRAINER_FLAG_START + T1_READ_16(scriptPtr + 2)) || FlagGet(FLAG_TRAINER_FLAG_START + T1_READ_16(scriptPtr + 4))))
		return FALSE;
		
    else if (GetTrainerFlagFromScriptPointer(scriptPtr)) //Trainer has already been beaten
        return FALSE;

    else
    {
        struct MapObject* trainerObj = &gEventObjects[eventObjId];
        bool8 canApproach = TrainerCanApproachPlayer(trainerObj);

        if (canApproach)
        {
			if (scriptPtr[1] == TRAINER_BATTLE_DOUBLE
            || scriptPtr[1] == TRAINER_BATTLE_REMATCH_DOUBLE
            || scriptPtr[1] == TRAINER_BATTLE_CONTINUE_SCRIPT_DOUBLE
			|| scriptPtr[1] == TRAINER_BATTLE_TWO_OPPONENTS)
			{
				if (ViableMonCount(gPlayerParty) < 2)
					return FALSE;
			}
			
			if (scriptPtr[1] == TRAINER_BATTLE_TAG) //You can't be stopped by someone using the tag battle feature
				return FALSE;
			
			struct TrainerSpotted trainer = {eventObjId, canApproach, (u8*) scriptPtr};
			ExtensionState.spotted.trainers[ExtensionState.spotted.count++] = trainer;
            return TRUE;
        }
        else
            return FALSE;
    }
}

bool8 GetTrainerFlagFromScriptPointer(const u8* data)
{
	if (TrainerBattleLoadArg8(data) != SCRCMD_TRAINERBATTLE) //Prevents game from crashing if you are spotted by someone
		return TRUE;							 			 //who's not a trainer
		
    u16 flag = TrainerBattleLoadArg16(data + 2);
    return FlagGet(FLAG_TRAINER_FLAG_START + flag);
}

void Task_OverworldMultiTrainers(u8 id)
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
void ConfigureTwoTrainersBattle(u8 trainerEventObjId, const u8* trainerScript)
{
    gSelectedEventObject = trainerEventObjId;
    gSpecialVar_LastTalked = gEventObjects[trainerEventObjId].localId;
    BattleSetup_ConfigureTrainerBattle(trainerScript + 1);
}

void SetUpTwoTrainersBattle(void)
{
    ScriptContext1_SetupScript(Script_TrainerSpottedMulti);
    ScriptContext2_Enable();
}

u8* BattleSetup_ConfigureTrainerBattle(const u8* data) {
    InitTrainerBattleVariables(); //0x8080110
    sTrainerBattleMode = TrainerBattleLoadArg8(data);

    switch (sTrainerBattleMode) {
		case TRAINER_BATTLE_CONTINUE_SCRIPT_NO_MUSIC: //Gym Leaders
			TrainerBattleLoadArgs(sContinueScriptBattleParams, data);
			SetMapVarsToTrainer();
			return EventScript_TryDoNormalTrainerBattle;
		
		case TRAINER_BATTLE_CONTINUE_SCRIPT:
			if (gApproachingTrainerId == 0) {
				TrainerBattleLoadArgs(sContinueScriptBattleParams, data); //0x8080168
				SetMapVarsToTrainer(); //0x80801F0
			}
			else {
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
			QuestLogRemtachBattleStore();
			
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
			QuestLogRemtachBattleStore();
			
			TrainerBattleLoadArgs(sDoubleBattleParams, data);
			SetMapVarsToTrainer();
			gTrainerBattleOpponent_A = GetRematchTrainerId(gTrainerBattleOpponent_A);
			return EventScript_TryDoDoubleRematchBattle;
		
		case TRAINER_BATTLE_OAK_TUTORIAL:
			TrainerBattleLoadArgs(sOakTutorialParams, data);
			return EventScript_DoTrainerBattle;
		
		case TRAINER_BATTLE_TAG:
			TrainerBattleLoadArgs(sTagBattleParams, data);
			VarSet(SECOND_OPPONENT_VAR, gTrainerBattleOpponent_B);
			VarSet(PARTNER_VAR, gTrainerBattlePartner);
			VarSet(PARTNER_BACKSPRITE_VAR, sPartnerBackSpriteId);
			FlagSet(TAG_BATTLE_FLAG);
			return EventScript_DoTrainerBattle;
		
		case TRAINER_BATTLE_TWO_OPPONENTS:
			TrainerBattleLoadArgs(sTwoOpponentBattleParams, data);
			SetMapVarsToTrainer();
			VarSet(SECOND_OPPONENT_VAR, gTrainerBattleOpponent_B);
			FlagSet(TWO_OPPONENT_FLAG);
			gApproachingTrainerId = 0;
			return EventScript_TryDoTwoOpponentBattle;
			
		default:
			if (gApproachingTrainerId == 0) {
				TrainerBattleLoadArgs(sOrdinaryBattleParams, data);
				SetMapVarsToTrainer();
			}
			else {
				TrainerBattleLoadArgs(sTrainerBOrdinaryBattleParams, data);
				VarSet(SECOND_OPPONENT_VAR, gTrainerBattleOpponent_B);
			}
			return EventScript_TryDoNormalTrainerBattle;
    }
}

void InitTrainerBattleVariables(void)
{
    sTrainerBattleMode = 0;
    if (ExtensionState.spotted.trainers == 0)
    {
        sTrainerIntroSpeech_A = 0;
        sTrainerDefeatSpeech_A = 0;
        sTrainerBattleScriptRetAddr = 0;
		sTrainerVictorySpeech = 0;
    }
    else
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
		
		switch (BATTLE_TOWER_BATTLE_TYPE) {
			case BATTLE_TOWER_DOUBLE:
				gBattleTypeFlags |= BATTLE_TYPE_DOUBLE;
				break;
			case BATTLE_TOWER_MULTI:
				gBattleTypeFlags |= (BATTLE_TYPE_DOUBLE | BATTLE_TYPE_TWO_OPPONENTS | BATTLE_TYPE_INGAME_PARTNER);
				break;
			case BATTLE_TOWER_LINK_MULTI:
				gBattleTypeFlags |= (BATTLE_TYPE_DOUBLE | BATTLE_TYPE_TWO_OPPONENTS | BATTLE_TYPE_MULTI);
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
			gBattleTypeFlags |= (BATTLE_TYPE_DOUBLE | BATTLE_TYPE_TWO_OPPONENTS | BATTLE_TYPE_INGAME_PARTNER /* | BATTLE_TYPE_MULTI*/);
				
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
		
		if (FlagGet(ACTIVATE_TUTORIAL_FLAG)) {
			gBattleTypeFlags |= BATTLE_TYPE_OAK_TUTORIAL;
			sTrainerBattleMode = TRAINER_BATTLE_OAK_TUTORIAL;
		}
	}
	
    gMain.savedCallback = CB2_EndTrainerBattle;
    StartTheBattle();
    ScriptContext1_Stop();
}

//Special 0x34
u8* GetIntroSpeechOfApproachingTrainer(void)
{
	if (FlagGet(BATTLE_TOWER_FLAG)) {
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
void SetUpTrainerEncounterMusic(void) {
    u16 trainerId;
    u16 music;

	if (QuestLogMode == 2 || QuestLogMode == 3)
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
void SetTrainerFlags(void) {
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

void MoveSecondNPCForTwoOpponentSighting(void) {
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

void LoadProperIntroSpeechForTwoOpponentSighting(void) {
	switch (Var8000) {
		case 0:
			if (gEventObjects[ExtensionState.spotted.trainers[0].id].localId != ExtensionState.spotted.firstTrainerNPCId) {
				gSelectedEventObject = GetEventObjectIdByLocalId(ExtensionState.spotted.secondTrainerNPCId);
				gApproachingTrainerId = 1;
			}
			else {
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

void HasOneTrainerBeenDefeated(void) {
	gSpecialVar_LastResult = FlagGet(FLAG_TRAINER_FLAG_START + gTrainerBattleOpponent_A) | FlagGet(FLAG_TRAINER_FLAG_START + gTrainerBattleOpponent_B);
}

void MoveCameraToTrainerB(void) {
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

void TrainerFaceFix(void) 
{
	u16 playerX = gEventObjects[0].currentCoords.x;
	u16 playerY = gEventObjects[0].currentCoords.y;
	u16 npcX = gEventObjects[gSelectedEventObject].currentCoords.x;
	u16 npcY = gEventObjects[gSelectedEventObject].currentCoords.y;
	
	if (!GetProperDirection(playerX, playerY, npcX, npcY))
		gSpecialVar_LastResult = 0xFFFF;
}

bool8 GetProperDirection(u16 currentX, u16 currentY, u16 toX, u16 toY) {
	u8 ret = FALSE;
	
	if (currentX == toX) {
		if (currentY < toY)
			gSpecialVar_LastResult = GoDown;
		else
			gSpecialVar_LastResult = GoUp;
		
		ret = TRUE;
	}
	else if (currentY == toY) {
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
	if (!VarGet(NPC_FOLLOWING_VAR)) {
		gSpecialVar_LastResult = 0xFFFF;
		return;
	}
	
	Var8005 = VarGet(NPC_FOLLOWING_VAR);
	
	u16 playerX = gEventObjects[0].currentCoords.x;
	u16 playerY = gEventObjects[0].currentCoords.y;
	
	for (u8 eventObjId = 0; eventObjId < MAP_OBJECTS_COUNT; ++eventObjId) {
		if (gEventObjects[eventObjId].active && gEventObjects[eventObjId].playerCopyableMovement) {
			u16 npcX = gEventObjects[gSelectedEventObject].currentCoords.x;
			u16 npcY = gEventObjects[gSelectedEventObject].currentCoords.y;
			
			if (playerX == npcX) 
			{
				if (playerY > npcY) {
					if (playerY != npcY + 1) { //Player and follower are not 1 tile apart
						if (!Var8000)
							gSpecialVar_LastResult = GoDown;
						else
							gEventObjects[gSelectedEventObject].currentCoords.y = playerY - 1;
					}
				}
				else {// Player Y < npcY
					if (playerY != npcY - 1) { //Player and follower are not 1 tile apart
						if (!Var8000)
							gSpecialVar_LastResult = GoUp;
						else
							gEventObjects[gSelectedEventObject].currentCoords.y = playerY + 1;
					}
				}
			}
			else //playerY == npcY
			{ 
				if (playerX > npcX) {
					if (playerX != npcX + 1) { //Player and follower are not 1 tile apart
						if (!Var8000)
							gSpecialVar_LastResult = GoRight;
						else
							gEventObjects[gSelectedEventObject].currentCoords.x = playerX - 1;
					}
				}
				else {// Player X < npcX
					if (playerX != npcX - 1) { //Player and follower are not 1 tile apart
						if (!Var8000)
							gSpecialVar_LastResult = GoLeft;
						else
							gEventObjects[gSelectedEventObject].currentCoords.x = playerX + 1;
					}
				}
			}
		}
	}
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


// hack safari step function to include custom walking scripts
// hook at 080A0F0C via r0
bool8 TakeStep(void)
{
	// increment new pedometer, always on
	if (gPedometers->alwaysActive != 0xFFFFFFFF)
		gPedometers->alwaysActive += 1;
	
	// check new pedometers
	if ((FlagGet(FLAG_LONG_PEDOMETER)) && gPedometers->large != 0xFFFFFFFF)
		gPedometers->large += 1;
	if ((FlagGet(FLAG_MED_PEDOMETER)) && gPedometers->medium != 0xFFFF)
		gPedometers->medium += 1;
	if ((FlagGet(FLAG_SMALL_PEDOMETER_1)) && gPedometers->smallOne != 0xFF)
		gPedometers->smallOne += 1;		
	if ((FlagGet(FLAG_SMALL_PEDOMETER_2)) && gPedometers->smallTwo != 0xFF)
		gPedometers->smallTwo += 1;	
	
	// check in safari zone
	if ((GetSafariZoneFlag() == TRUE) && (gSafariSteps != 0))
	{
		gSafariSteps -= 1;
		if (gSafariSteps == 0)	// safari steps went to zero
		{
			ScriptContext1_SetupScript((void*) SafariZoneEndScript);
			return TRUE;
		}
    }
	else
	{
		// check custom walking scripts
		if (gWalkingScript != 0)
		{
			ScriptContext1_SetupScript((void*) gWalkingScript);
			return TRUE;
		}
		else
		{
			u8 scriptInd = VarGet(DEFAULT_WALKING_SCRIPT);
			if (scriptInd != 0 || scriptInd > ARRAY_COUNT(gDefaultWalkingScripts))
			{
				if (gDefaultWalkingScripts[scriptInd-1] == 0)
					return FALSE;
				ScriptContext1_SetupScript(gDefaultWalkingScripts[scriptInd-1]);
				return TRUE;
			}
			else
				return FALSE;
		}
	}
	return FALSE;
};

