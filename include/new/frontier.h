#pragma once

#include "../global.h"

//Exported Functions
u8 GetFrontierTrainerClassId(u16 trainerId, u8 battlerNum);
void CopyFrontierTrainerName(u8* dst, u16 trainerId, u8 battlerNum);
const u8* GetFrontierTrainerName(u16 trainerId, u8 battlerNum);
void CopyFrontierTrainerText(u8 whichText, u16 trainerId, u8 battlerNum);
u8 GetFrontierTrainerFrontSpriteId(u16 trainerId, u8 battlerNum);
u16 TryGetSpecialFrontierTrainerMusic(u16 trainerId, u8 battlerNum);
u32 GetAIFlagsInBattleFrontier(unusedArg u8 bank);
u8 GetNumMonsOnTeamInFrontier(void);
bool8 IsFrontierSingles(u8 battleType);
bool8 IsFrontierDoubles(u8 battleType);
bool8 IsFrontierMulti(u8 battleType);
bool8 IsGSCupBattle();
bool8 IsRandomBattleTowerBattle();
bool8 RayquazaCanMegaEvolveInFrontierBattle();
u8 GetBattleTowerLevel(u8 tier);
void UpdateTypesForCamomons(u8 bank);
u8 GetCamomonsTypeByMon(struct Pokemon* mon, u8 whichType);
//u8 GetCamomonsTypeBySpread(struct BattleTowerSpread* spread, u8 whichType); - Defined further down
bool8 DuplicateItemsAreBannedInTier(u8 tier, u8 battleType);
u16 GetCurrentBattleTowerStreak(void);
u16 GetMaxBattleTowerStreakForTier(u8 tier);
u16 GetBattleTowerStreak(u8 currentOrMax, u16 inputBattleStyle, u16 inputTier, u16 partySize, u8 level);

//Functions Hooked In:
u16 sp052_GenerateTowerTrainer(void);
void sp053_LoadFrontierIntroBattleMessage(void);
u16 sp054_GetBattleTowerStreak(void);
void sp055_UpdateBattleTowerStreak(void);
u16 sp056_DetermineBattlePointsToGive(void);

//Exported Constants
//#define TOTAL_SPREADS 0x4A0 //sizeof(gFrontierSpreads) / sizeof(struct BattleTowerSpreads)

#define NUM_MALE_NAMES gNumMaleFrontierTrainerNames
#define NUM_FEMALE_NAMES gNumFemaleFrontierTrainerNames
#define NUM_TOWER_TRAINERS gNumTowerTrainers
#define NUM_SPECIAL_TOWER_TRAINERS gNumSpecialTowerTrainers

#define BATTLE_TOWER_MULTI_TRAINER_TID 0x396 //Trainer Index
#define FRONTIER_BRAIN_TID 0x397 //Trainer Index
#define BATTLE_TOWER_SPECIAL_TID 0x398 //Trainer Index
#define BATTLE_TOWER_TID 0x399 //Trainer Index


enum BattleFacilities
{
	IN_BATTLE_TOWER,
	IN_BATTLE_SANDS,
	IN_BATTLE_MINE,
	IN_BATTLE_CIRCUS,
	IN_ISLE_CHALLENGE,
	NUM_BATTLE_FACILITIES,
};

#define BATTLE_FACILITY_NUM VarGet(0x403A) //Temp Var

enum 
{
	FRONTIER_BEFORE_TEXT,
	FRONTIER_PLAYER_LOST_TEXT,
	FRONTIER_PLAYER_WON_TEXT,
};

enum BattleTowerBattleTypes
{
	BATTLE_TOWER_SINGLE,
	BATTLE_TOWER_DOUBLE,
	BATTLE_TOWER_MULTI,
	BATTLE_TOWER_LINK_MULTI,
	BATTLE_TOWER_SINGLE_RANDOM,
	BATTLE_TOWER_DOUBLE_RANDOM,
	BATTLE_TOWER_MULTI_RANDOM,
};

#define NUM_TOWER_BATTLE_TYPES (BATTLE_TOWER_MULTI_RANDOM + 1)

enum BattleTowerFormats
{
	BATTLE_TOWER_STANDARD,
	BATTLE_TOWER_NO_RESTRICTIONS,
	BATTLE_TOWER_OU,
	BATTLE_TOWER_UBER,
	BATTLE_TOWER_LITTLE_CUP,
	BATTLE_TOWER_MIDDLE_CUP, //GS Cup in Doubles
	BATTLE_TOWER_MONOTYPE,
	BATTLE_TOWER_CAMOMONS,
	BATTLE_TOWER_SCALEMONS,
};

#define NUM_FORMATS (BATTLE_TOWER_MONOTYPE + 1)
#define NUM_FORMATS_OLD (BATTLE_TOWER_MIDDLE_CUP + 1) //Monotype not included b/c wasn't included in Battle Tower Demo original release

#define BATTLE_TOWER_GS_CUP BATTLE_TOWER_MIDDLE_CUP //Replaces Middle Cup in Doubles

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
	BATTLE_TOWER_FEMALE,
};

enum BattlerIds
{
	FRONTIER_TRAINER_A,
	FRONTIER_TRAINER_B,
	FRONTIER_PARTNER,
};

enum FrontierSpreadAbilityNumbers
{
	FRONTIER_ABILITY_HIDDEN,
	FRONTIER_ABILITY_1,
	FRONTIER_ABILITY_2,
};

enum
{
	CURR_STREAK,
	MAX_STREAK,
};

enum
{
	DOUBLES_ANY_TEAM,
	DOUBLES_SUN_TEAM,
	DOUBLES_SAND_TEAM,
	DOUBLES_RAIN_TEAM,
	DOUBLES_HAIL_TEAM,
	DOUBLES_ELECTRIC_TERRAIN_TEAM,
	DOUBLES_TRICK_ROOM_TEAM,
	DOUBLES_TAILWIND_TEAM,
	DOUBLES_HYPER_OFFENSE_TEAM,
	DOUBLES_JUSTIFIED_TEAM,
};

//Exported Data Structures
extern const u16 gNumMaleFrontierTrainerNames;
extern const u16 gNumFemaleFrontierTrainerNames;
extern const u16 gNumTowerTrainers;
extern const u16 gNumSpecialTowerTrainers;

struct BattleTowerSpread
{
    u16 species;
	u8 nature;
	u8 ball;
	u32 hpIv : 5;
	u32 atkIv : 5;
	u32 defIv : 5;
	u32 spdIv : 5;
	u32 spAtkIv : 5;
	u32 spDefIv : 5;
	u8 hpEv;
	u8 atkEv;
	u8 defEv;
	u8 spdEv;
	u8 spAtkEv;
	u8 spDefEv;
	u16 item;
	u16 moves[MAX_MON_MOVES];
	u8 specificTeamType;
	u8 shiny : 1;
	u8 forSingles : 1;
	u8 forDoubles : 1;
	u8 modifyMovesDoubles : 1;
	u8 ability : 2;
};

extern const struct BattleTowerSpread gFrontierSpreads[];
//#define gFrontierSpreads ((const struct BattleTowerSpread*) 0x89DFA00)

struct BattleTowerTrainer
{
    u16 owNum;
	u8 trainerClass;
	u8 trainerSprite;
	u8 gender;
	const u8* preBattleText;
	const u8* playerWinText;
	const u8* playerLoseText;
};

extern const struct BattleTowerTrainer gTowerTrainers[];

struct SpecialBattleTowerTrainer
{
    u16 owNum;
	u8 trainerClass;
	u8 trainerSprite;
	u8 gender;
	u8 isMonotype;
	const u8* name;
	const u8* preBattleText;
	const u8* playerWinText;
	const u8* playerLoseText;
	const struct BattleTowerSpread* regularSpreads;
	const struct BattleTowerSpread* middleCupSpreads;
	const struct BattleTowerSpread* littleCupSpreads;
	const struct BattleTowerSpread* legendarySpreads;
	u16 regSpreadSize;
	u16 mcSpreadSize;
	u16 lcSpreadSize;
	u16 legSpreadSize;
	u16 songId;
};

extern const struct SpecialBattleTowerTrainer gSpecialTowerTrainers[];

struct MultiBattleTowerTrainer
{
	u16 owNum;
	u8 trainerClass;
	u8 backSpriteId;
	u8 gender;
	const u8* name;
	u32 otId;
	const struct BattleTowerSpread* regularSpreads;
	const struct BattleTowerSpread* legendarySpreads;
	const struct BattleTowerSpread* littleCupSpreads;
	u16 regSpreadSize;
	u16 legSpreadSize;
	u16 lcSpreadSize;
};

extern const struct MultiBattleTowerTrainer gFrontierMultiBattleTrainers[];
extern const u8 gNumFrontierMultiTrainers;

struct FrontierBrain
{
	u8 trainerClass;
	u8 trainerSprite;
	u8 gender;
	const u8* name;
	const struct BattleTowerSpread* spreads;
	u16 spreadSize;
	u16 songId;
};

extern const struct FrontierBrain gFrontierBrains[];

extern u16 gBattleTowerStreaks[NUM_TOWER_BATTLE_TYPES][NUM_FORMATS_OLD][/*PARTY_SIZE*/ 2][/*LEVEL*/ 2][/*CURRENT_OR_MAX*/ 2]; //0x2026840
//FREE SPACE FROM SLIDESHOW 0x202682C - 0x2027434

extern const u8 gBattleTowerTiers[];
extern const u8 gBattleMineTiers[];
extern const u8 gBattleCircusTiers[];
#define gBattleSandsTiers gBattleTowerTiers //Battle Sands & Battle Tower have same tiers
extern const u8 gNumBattleTowerTiers;
extern const u8 gNumBattleMineTiers;
extern const u8 gNumBattleCircusTiers;
#define gNumBattleSandsTiers gNumBattleTowerTiers

extern const species_t gBattleTowerStandardSpeciesBanList[];
extern const species_t gGSCup_LegendarySpeciesList[];
extern const species_t gSmogonOU_SpeciesBanList[];
extern const species_t gSmogonOUDoubles_SpeciesBanList[];
extern const species_t gSmogonLittleCup_SpeciesList[];
extern const species_t gMiddleCup_SpeciesList[];
extern const species_t gSmogonMonotype_SpeciesBanList[];
extern const ability_t gSmogonOU_AbilityBanList[];
extern const ability_t gSmogonOUDoubles_AbilityBanList[];
extern const ability_t gSmogonMonotype_AbilityBanList[];
extern const ability_t gMiddleCup_AbilityBanList[];
extern const item_t gBattleTowerStandard_ItemBanList[];
extern const item_t gSmogonOU_ItemBanList[];
extern const item_t gSmogonOUDoubles_ItemBanList[];
extern const item_t gSmogonLittleCup_ItemBanList[];
extern const item_t gSmogonMonotype_ItemBanList[];
extern const item_t gMiddleCup_ItemBanList[];
extern const move_t gSmogon_MoveBanList[];
extern const move_t gSmogonOUDoubles_MoveBanList[];
extern const move_t gSmogonLittleCup_MoveBanList[];

u8 GetCamomonsTypeBySpread(const struct BattleTowerSpread* spread, u8 whichType);
