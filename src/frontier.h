#pragma once

#include "defines.h"

#define TOTAL_SPREADS 0x4A0 //sizeof(gFrontierSpreads) / sizeof(struct BattleTowerSpreads)

#define NUM_MALE_NAMES 100
#define NUM_FEMALE_NAMES 100

#define species_array ((species_t*) 0x203DA00)
#define item_array ((item_t*) 0x203DA10)

enum 
{
	FRONTIER_BEFORE_TEXT,
	FRONTIER_PLAYER_LOST_TEXT,
	FRONTIER_PLAYER_WON_TEXT,
};

struct BattleTowerTrainer
{
    u16 owNum;
	u8 trainerClass;
	u8 trainerSprite;
	u8 gender;
	u8* preBattleText;
	u8* playerWinText;
	u8* playerLoseText;
};

extern const struct BattleTowerTrainer gTowerTrainers[];

struct BattleTowerSpreads 
{
    u16 species;
	u8 nature;
	u8 hpIv;
	u8 atkIv;
	u8 defIv;
	u8 spdIv;
	u8 spAtkIv;
	u8 spDefIv;
	u8 hpEv;
	u8 atkEv;
	u8 defEv;
	u8 spdEv;
	u8 spAtkEv;
	u8 spDefEv;
	u8 ability;
	u16 item;
	u16 moves[4];
	u8 ball;
    u8 _1; // 0x00
	u32 _2; // 0x00000000
};

extern const struct BattleTowerSpreads gFrontierSpreads[];
#define gFrontierSpreads ((const struct BattleTowerSpreads*) 0x89DFA00)

enum BattleTowerBattleTypes
{
BATTLE_TOWER_SINGLE,
BATTLE_TOWER_DOUBLE,
BATTLE_TOWER_MULTI,
BATTLE_TOWER_LINK_MULTI,
};

enum BattleTowerFormats
{
BATTLE_TOWER_STANDARD,
BATTLE_TOWER_FREE_FOR_ALL,
BATTLE_TOWER_OU,
BATTLE_TOWER_UBER,
BATTLE_TOWER_LITTLE_CUP,
BATTLE_TOWER_MIDDLE_CUP,
};

enum BattleTowerPartySizes
{
BATTLE_TOWER_SIZE_STANDARD,
BATTLE_TOWER_SIZE_1V1,
BATTLE_TOWER_SIZE_2V2,
BATTLE_TOWER_SIZE_3V3,
BATTLE_TOWER_SIZE_4V4,
BATTLE_TOWER_SIZE_5V5,
BATTLE_TOWER_SIZE_6V6,
};

enum BattleTowerGenders
{
BATTLE_TOWER_MALE,
BATTLE_TOWER_FEMALE
};

enum TierBanCheckingType 
{
CHECK_BATTLE_TOWER_SPREADS,
CHECK_PARTY_OFFSET,
};
 
extern species_t StandardSpeciesBanList[];
extern species_t OU_SpeciesBanList[];
extern species_t LittleCup_SpeciesList[];
extern ability_t OU_AbilityBanList[];
extern item_t StandardItemBanList[];
extern item_t OU_ItemBanList[];
extern move_t SmogonMoveBanList[];
extern move_t LittleCup_MoveBanList[];