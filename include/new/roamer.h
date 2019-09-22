#pragma once

/**
 * \file roamer.h
 * \brief Functions dealing with roaming Pokemon. Many of these functions are based
 *		  off of the vanilla roaming functions, with the added bonus of supporting
 *		  up to ten custom roamers.
 */

//Exported Functions
void ClearRoamerData(void);
void sp129_InitRoamer(void);
void UpdateLocationHistoryForRoamer(void);
void RoamersMoveToOtherLocationSet(void);
void RoamersMove(void);
bool8 TryStartRoamerEncounter(u8 environment);
void UpdateFoughtRoamerHPStatus(struct Pokemon *mon);
void SetFoughtRoamerInactive(void);
void GetRoamerLocation(u8* mapGroup, u8* mapNum, u8 id);
bool8 IsSpeciesRoaming(u16 species);
void GetMapGroupAndMapNumOfRoamer(u16 species, u8* mapGroup, u8* mapNum);
void BattleSetup_StartRoamerBattle(void);

//Exported Constants
#ifdef SAVE_BLOCK_EXPANSION
	#define MAX_NUM_ROAMERS 10
	#define gRoamers ((struct Roamer*) 0x203BA30) //struct Roamer* gRoamers[MAX_NUM_ROAMERS]
#else
	#define MAX_NUM_ROAMERS 1
	#define gRoamers ((struct Roamer*) &gSaveBlock1->roamer) //struct Roamer* gRoamers[MAX_NUM_ROAMERS]
#endif

#define gLastSelectedRoamer (*((u8*) 0x203F3AE))

//Exported Data Structures
struct Roamer
{
    /*0x00*/ u32 ivs;
    /*0x04*/ u32 personality;
    /*0x08*/ u16 species;
    /*0x0A*/ u16 hp;
    /*0x0C*/ u8 level;
    /*0x0D*/ u8 status;
	/*0x0E*/ u8 canAppearOnLand;
	/*0x0F*/ u8 canAppearOnWater;
	/*0x10*/ u8 locationHistory[3][2];
	/*0x16*/ u8 location[2];
	/*SIZE = 0x18*/
};
