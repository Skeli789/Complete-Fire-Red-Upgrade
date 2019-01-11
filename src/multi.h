#pragma once

#include "defines.h"

struct TrainerSpotted {
    /* NPC state id */
    u8 id;

    /* Distance from trainer to player */
    u8 distance;

    /* The script the on the trainer NPC. */
    u8* script;
};

struct BattleExtensionState { //Clear After Battle
    struct {
        u8 count;
		u8 approachingId;
		u8 firstTrainerNPCId; //Used in trainerbattle 0xB
		u8 secondTrainerNPCId;
        struct TrainerSpotted trainers[2];
    } spotted;
	
    void* partyBackup;
	pokemon_t* skyBattlePartyBackup;
	u16* itemBackup;
	u8* trainerBIntroSpeech;
	u8* trainerBDefeatSpeech;
	u8* trainerBVictorySpeech;
	u8* trainerBCantBattleSpeech;
	u8* trainerBRetAddress;
	
	u16   partnerTrainerId;
	u16   partnerBackSpriteId;
	u16   trainerBTrainerId;
	u8	  multiTaskStateHelper;
};

extern struct BattleExtensionState ExtensionState;

#define gTrainerBattleOpponent_B 		ExtensionState.trainerBTrainerId
#define gTrainerBattlePartner 	 		ExtensionState.partnerTrainerId
#define sPartnerBackSpriteId	 		ExtensionState.partnerBackSpriteId
#define sTrainerIntroSpeech_B 	 		ExtensionState.trainerBIntroSpeech
#define sTrainerDefeatSpeech_B  	 	ExtensionState.trainerBDefeatSpeech
#define sTrainerVictorySpeech_B  		ExtensionState.trainerBVictorySpeech
#define sTrainerCannotBattleSpeech_B	ExtensionState.trainerBCantBattleSpeech
#define sTrainerBattleScriptRetAddr_B 	ExtensionState.trainerBRetAddress
#define gApproachingTrainerId 			ExtensionState.spotted.approachingId
#define gFirstTrainerNPCId				ExtensionState.spotted.firstTrainerNPCId
#define gSecondTrainerNPCId				ExtensionState.spotted.secondTrainerNPCId