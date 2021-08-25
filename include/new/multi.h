#pragma once

#include "../global.h"
#include "../battle.h"
#include "../../src/defines.h"

/**
 * \file multi.h
 * \brief Contains a large part of the implementation of Multi Battles. This includes
 *		  loading the partner's name, backsprite, and providing accessor functions to
 *		  determine if the current battle is a tag or multi battle.
 */

//Exported Functions
 u8* GetTrainerBLoseText(void);
 bool8 IsLinkDoubleBattle(void);
 bool8 IsMultiBattle(void);
 bool8 IsTwoOpponentBattle(void);
 bool8 BankSideHasTwoTrainers(u8 bank);
 bool8 IsTagBattle(void);
 bool8 IsPartnerAttacker(void);
 u8* PartnerLoadName(void);
 u8 LoadPartnerBackspriteIndex(void);
 void SetControllerToPlayerPartner(void);

 #ifdef OPEN_WORLD_TRAINERS

extern const u8 gOpenWorldLevelRanges[NUM_BADGE_OPTIONS][2];

#endif

//Functions Hooked In
void BattleIntroOpponent1SendsOutMonAnimation(void);
void MultiInitPokemonOrder(void);
void MultiBattleAddSecondOpponent(void);
u32 MultiMoneyCalc(void);
void ChooseProperPartnerController(void);

//Exported Constants
#define COMMAND_MAX 0x39

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
