#include "defines.h"
#include "defines_battle.h"
#include "../include/event_data.h"
#include "../include/random.h"
#include "../include/script.h"

#include "../include/new/Helper_Functions.h"
#include "../include/new/frontier.h"
#include "../include/new/mega.h"
#include "../include/new/pokemon_storage_system.h"

extern u8* gMaleFrontierNamesTable[];
extern u8* gFemaleFrontierNamesTable[];

extern const u8 gText_SingleBattle[];
extern const u8 gText_DoubleBattle[];
extern const u8 gText_MultiBattle[];
extern const u8 gText_LinkMultiBattle[];
extern const u8 gText_RandomSingleBattle[];
extern const u8 gText_RandomDoubleBattle[];
extern const u8 gText_RandomMultiBattle[];

extern const u8 gText_BattleTowerStandard[];
extern const u8 gText_NoRestrictions[];
extern const u8 gText_SmogonGen7OU[];
extern const u8 gText_SmogonGen7Uber[];
extern const u8 gText_SmogonLittleCup[];
extern const u8 gText_MiddleCup[];
extern const u8 gText_SmogonMonotype[];
extern const u8 gText_GSCup[];

extern const u8 gText_On[];
extern const u8 gText_Off[];
extern const u8 gText_Previous[];
extern const u8 gText_Max[];
extern const u8 gText_None[];

const u8 gBattleTowerTiers[] =
{
	BATTLE_TOWER_STANDARD,
	BATTLE_TOWER_NO_RESTRICTIONS,
	BATTLE_TOWER_OU,
	BATTLE_TOWER_UBER,
	BATTLE_TOWER_LITTLE_CUP,
	BATTLE_TOWER_MIDDLE_CUP,
	BATTLE_TOWER_MONOTYPE,
};

const u8 gNumBattleTowerTiers = ARRAY_COUNT(gBattleTowerTiers);

const u8 gBattleMineTiers[] =
{
	BATTLE_TOWER_OU,
	BATTLE_TOWER_CAMOMONS,
};

const u8 gNumBattleMineTiers = ARRAY_COUNT(gBattleMineTiers);

const u8 gBattleCircusTiers[] =
{
	BATTLE_TOWER_STANDARD,
	BATTLE_TOWER_NO_RESTRICTIONS,
	BATTLE_TOWER_LITTLE_CUP,
	BATTLE_TOWER_MIDDLE_CUP,
	BATTLE_TOWER_SCALEMONS,
	BATTLE_TOWER_MONOTYPE,
};

const u8 gNumBattleCircusTiers = ARRAY_COUNT(gBattleCircusTiers);

const u8* const gBattleFrontierTierNames[NUM_FORMATS] =
{
	[BATTLE_TOWER_STANDARD] = gText_BattleTowerStandard,
	[BATTLE_TOWER_NO_RESTRICTIONS] = gText_NoRestrictions,
	[BATTLE_TOWER_OU] = gText_SmogonGen7OU,
	[BATTLE_TOWER_UBER] gText_SmogonGen7Uber,
	[BATTLE_TOWER_LITTLE_CUP] = gText_SmogonLittleCup,
	[BATTLE_TOWER_MIDDLE_CUP] = gText_MiddleCup,
	[BATTLE_TOWER_MONOTYPE] = gText_SmogonMonotype,
};

const u8* const gBattleFrontierFormats[NUM_TOWER_BATTLE_TYPES] =
{
	[BATTLE_TOWER_SINGLE] = gText_SingleBattle,
	[BATTLE_TOWER_DOUBLE] = gText_DoubleBattle,
	[BATTLE_TOWER_MULTI] = gText_MultiBattle,
	[BATTLE_TOWER_LINK_MULTI] = gText_LinkMultiBattle,
	[BATTLE_TOWER_SINGLE_RANDOM] = gText_RandomSingleBattle,
	[BATTLE_TOWER_DOUBLE_RANDOM] = gText_RandomDoubleBattle,
	[BATTLE_TOWER_MULTI_RANDOM] = gText_RandomMultiBattle,
};

//This file's functions:
static u8 AdjustLevelForTier(u8 level, u8 tier);
static void LoadProperStreakData(u8* currentOrMax, u8* battleStyle, u8* tier, u8* partySize, u8* level);

u8 GetFrontierTrainerClassId(u16 trainerId, u8 battlerNum) 
{
	switch (trainerId) {
		case BATTLE_TOWER_TID:
			return gTowerTrainers[VarGet(TOWER_TRAINER_ID_VAR + battlerNum)].trainerClass;
		case BATTLE_TOWER_SPECIAL_TID:
			return gSpecialTowerTrainers[VarGet(TOWER_TRAINER_ID_VAR + battlerNum)].trainerClass;
		case FRONTIER_BRAIN_TID:
			return gFrontierBrains[VarGet(TOWER_TRAINER_ID_VAR + battlerNum)].trainerClass;
		case BATTLE_TOWER_MULTI_TRAINER_TID:
			return gFrontierMultiBattleTrainers[VarGet(TOWER_TRAINER_ID_PARTNER_VAR)].trainerClass;
		default:
			return gTrainers[trainerId].trainerClass;
	}	
}

void CopyFrontierTrainerName(u8* dst, u16 trainerId, u8 battlerNum)
{
	int i;
	const u8* name = GetFrontierTrainerName(trainerId, battlerNum);
	
	for (i = 0; name[i] != EOS; ++i)
		dst[i] = name[i];
		
	dst[i] = EOS;
}

const u8* GetFrontierTrainerName(u16 trainerId, u8 battlerNum)
{
	const u8* name;
	
	switch (trainerId) {
		case BATTLE_TOWER_TID: ;
			u16 nameId = (battlerNum == 0) ? VarGet(BATTLE_TOWER_TRAINER1_NAME) : VarGet(BATTLE_TOWER_TRAINER2_NAME);

			if (gTowerTrainers[VarGet(TOWER_TRAINER_ID_VAR + battlerNum)].gender == BATTLE_TOWER_MALE)
			{
				if (nameId == 0xFFFF)
					nameId = Random() % NUM_MALE_NAMES;
				
				name = gMaleFrontierNamesTable[nameId];
			}
			else
			{
				if (nameId == 0xFFFF)
					nameId = Random() % NUM_FEMALE_NAMES;

				name = gFemaleFrontierNamesTable[nameId];
			}
			
			if (battlerNum == 0)
				VarSet(BATTLE_TOWER_TRAINER1_NAME, nameId);
			else
				VarSet(BATTLE_TOWER_TRAINER2_NAME, nameId);
			break;
		case BATTLE_TOWER_SPECIAL_TID:
			name = gSpecialTowerTrainers[VarGet(TOWER_TRAINER_ID_VAR + battlerNum)].name;
			break;
		case FRONTIER_BRAIN_TID:
			name = gFrontierBrains[VarGet(TOWER_TRAINER_ID_VAR + battlerNum)].name;
			break;
		case BATTLE_TOWER_MULTI_TRAINER_TID: ;
			u16 partnerId = VarGet(TOWER_TRAINER_ID_PARTNER_VAR);
			name = TryGetRivalNameByTrainerClass(gFrontierMultiBattleTrainers[partnerId].trainerClass);
			
			if (name == NULL) //Rival name isn't tied to a trainer class
				name = gFrontierMultiBattleTrainers[partnerId].name;
			break;
		default:
			name = gTrainers[trainerId].trainerName;
	}

	return ReturnEmptyStringIfNull(name);
}

void CopyFrontierTrainerText(u8 whichText, u16 trainerId, u8 battlerNum)
{
	switch (trainerId) {
		case BATTLE_TOWER_TID:
			switch (whichText) {
				case FRONTIER_BEFORE_TEXT:
					StringCopy(gStringVar4, (gTowerTrainers[VarGet(TOWER_TRAINER_ID_VAR + battlerNum)].preBattleText));
					break;
			
				case FRONTIER_PLAYER_LOST_TEXT:
					StringCopy(gStringVar4, (gTowerTrainers[VarGet(TOWER_TRAINER_ID_VAR + battlerNum)].playerLoseText));
					break;
			
				case FRONTIER_PLAYER_WON_TEXT:
					StringCopy(gStringVar4, (gTowerTrainers[VarGet(TOWER_TRAINER_ID_VAR + battlerNum)].playerWinText));
			}
			break;
		case BATTLE_TOWER_SPECIAL_TID:
			switch (whichText) {
				case FRONTIER_BEFORE_TEXT:
					StringCopy(gStringVar4, (gSpecialTowerTrainers[VarGet(TOWER_TRAINER_ID_VAR + battlerNum)].preBattleText));
					break;
			
				case FRONTIER_PLAYER_LOST_TEXT:
					StringCopy(gStringVar4, (gSpecialTowerTrainers[VarGet(TOWER_TRAINER_ID_VAR + battlerNum)].playerLoseText));
					break;
			
				case FRONTIER_PLAYER_WON_TEXT:
					StringCopy(gStringVar4, (gSpecialTowerTrainers[VarGet(TOWER_TRAINER_ID_VAR + battlerNum)].playerWinText));
			}
			break;
		case FRONTIER_BRAIN_TID:
		default:
			switch (whichText) {
				case FRONTIER_BEFORE_TEXT:
					if (gFrontierBrains[VarGet(TOWER_TRAINER_ID_VAR + battlerNum)].preBattleText != NULL)
						StringCopy(gStringVar4, gFrontierBrains[VarGet(TOWER_TRAINER_ID_VAR + battlerNum)].preBattleText);
					break;
			
				case FRONTIER_PLAYER_LOST_TEXT:
					if (gFrontierBrains[VarGet(TOWER_TRAINER_ID_VAR + battlerNum)].playerLoseText != NULL)
						StringCopy(gStringVar4, (gFrontierBrains[VarGet(TOWER_TRAINER_ID_VAR + battlerNum)].playerLoseText));
					else //Frontier Brain text can be loaded from the OW
						StringCopy(gStringVar4, GetTrainerAWinText());
					break;
			
				case FRONTIER_PLAYER_WON_TEXT:
					if (gFrontierBrains[VarGet(TOWER_TRAINER_ID_VAR + battlerNum)].playerWinText != NULL)
						StringCopy(gStringVar4, (gFrontierBrains[VarGet(TOWER_TRAINER_ID_VAR + battlerNum)].playerWinText));
					else //Frontier Brain text can be loaded from the OW
						StringCopy(gStringVar4, GetTrainerALoseText());
			}
			break;		
	}
}

u8 GetFrontierTrainerFrontSpriteId(u16 trainerId, u8 battlerNum)
{
	switch (trainerId) {
		case BATTLE_TOWER_TID:
			return gTowerTrainers[VarGet(TOWER_TRAINER_ID_VAR + battlerNum)].trainerSprite;
		case BATTLE_TOWER_SPECIAL_TID:
			return gSpecialTowerTrainers[VarGet(TOWER_TRAINER_ID_VAR + battlerNum)].trainerSprite;
		case FRONTIER_BRAIN_TID:
			return gFrontierBrains[VarGet(TOWER_TRAINER_ID_VAR + battlerNum)].trainerSprite;
		default:
			return gTrainers[trainerId].trainerPic;
	}
}

u16 TryGetSpecialFrontierTrainerMusic(u16 trainerId, u8 battlerNum)
{
	switch (trainerId) {
		case BATTLE_TOWER_SPECIAL_TID:
			return gSpecialTowerTrainers[VarGet(TOWER_TRAINER_ID_VAR + battlerNum)].songId;
		case FRONTIER_BRAIN_TID:
			return gFrontierBrains[VarGet(TOWER_TRAINER_ID_VAR + battlerNum)].songId;
		default:
			return 0;
	}
}

u32 GetAIFlagsInBattleFrontier(unusedArg u8 bank)
{
	return AI_SCRIPT_CHECK_BAD_MOVE | AI_SCRIPT_CHECK_GOOD_MOVE;
}

u8 GetNumMonsOnTeamInFrontier(void)
{
	return MathMin(MathMax(1, VarGet(BATTLE_TOWER_POKE_NUM)), PARTY_SIZE);
}

bool8 IsFrontierSingles(u8 battleType)
{
	return battleType == BATTLE_TOWER_SINGLE
		|| battleType == BATTLE_TOWER_SINGLE_RANDOM;
}

bool8 IsFrontierDoubles(u8 battleType)
{
	return battleType == BATTLE_TOWER_DOUBLE
		|| battleType == BATTLE_TOWER_DOUBLE_RANDOM; 
}

bool8 IsFrontierMulti(u8 battleType)
{
	return battleType == BATTLE_TOWER_MULTI
		|| battleType == BATTLE_TOWER_MULTI_RANDOM
		|| battleType == BATTLE_TOWER_LINK_MULTI;
}

bool8 IsRandomBattleTowerBattle()
{
	u8 battleType = VarGet(BATTLE_TOWER_BATTLE_TYPE);
	
	return battleType == BATTLE_TOWER_SINGLE_RANDOM
		|| battleType == BATTLE_TOWER_DOUBLE_RANDOM
		|| battleType == BATTLE_TOWER_MULTI_RANDOM;
}

bool8 IsGSCupBattle()
{
	u8 battleType = VarGet(BATTLE_TOWER_BATTLE_TYPE);

	return (IsFrontierDoubles(battleType) || IsFrontierMulti(battleType))
	    &&  VarGet(BATTLE_TOWER_TIER) == BATTLE_TOWER_GS_CUP;
}

bool8 DuplicateItemsAreBannedInTier(u8 tier, u8 battleType)
{
	if (tier == BATTLE_TOWER_STANDARD
	||  tier == BATTLE_TOWER_MIDDLE_CUP)
		return TRUE;
		
	return !IsFrontierSingles(battleType) && tier == BATTLE_TOWER_GS_CUP;
}

bool8 RayquazaCanMegaEvolveInFrontierBattle()
{
	return IsGSCupBattle()
	    || VarGet(BATTLE_TOWER_TIER) == BATTLE_TOWER_NO_RESTRICTIONS
		|| IsScaleMonsBattle();
}

u8 GetBattleTowerLevel(u8 tier)
{
	return AdjustLevelForTier(VarGet(BATTLE_TOWER_POKE_LEVEL), tier);
}

void UpdateTypesForCamomons(u8 bank)
{
	gBattleMons[bank].type1 = gBattleMoves[gBattleMons[bank].moves[0]].type;

	if (gBattleMons[bank].moves[1] != MOVE_NONE)
		gBattleMons[bank].type2 = gBattleMoves[gBattleMons[bank].moves[1]].type;
	else
		gBattleMons[bank].type2 = gBattleMons[bank].type1;
}

u8 GetCamomonsTypeByMon(struct Pokemon* mon, u8 whichType)
{
	if (whichType == 0)
	{
		return gBattleMoves[GetMonData(mon, MON_DATA_MOVE1, NULL)].type;
	}
	else
	{
		u16 move2 = GetMonData(mon, MON_DATA_MOVE2, NULL);
		if (move2 != MOVE_NONE)
			return gBattleMoves[move2].type;
		
		return gBattleMoves[GetMonData(mon, MON_DATA_MOVE1, NULL)].type;
	}
}

u8 GetCamomonsTypeBySpread(const struct BattleTowerSpread* spread, u8 whichType)
{
	if (whichType == 0)
	{
		return gBattleMoves[spread->moves[0]].type;
	}
	else
	{
		if (spread->moves[1] != MOVE_NONE)
			return gBattleMoves[spread->moves[1]].type;
		
		return gBattleMoves[spread->moves[0]].type;
	}
}

bool8 TryUpdateOutcomeForFrontierBattle(void)
{
	u32 i;
	u32 playerHPCount, enemyHPCount;
	
	if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER)
	{
	 
		for (playerHPCount = 0, i = 0; i < PARTY_SIZE; ++i)
		{
			if (GetMonData(&gPlayerParty[i], MON_DATA_SPECIES, NULL) != SPECIES_NONE
			&& !GetMonData(&gPlayerParty[i], MON_DATA_IS_EGG, NULL))
				playerHPCount += GetMonData(&gPlayerParty[i], MON_DATA_HP, NULL);
		}

		for (enemyHPCount = 0, i = 0; i < PARTY_SIZE; ++i)
		{
			if (GetMonData(&gEnemyParty[i], MON_DATA_SPECIES, NULL) != SPECIES_NONE
			&& !GetMonData(&gEnemyParty[i], MON_DATA_IS_EGG, NULL))
				enemyHPCount += GetMonData(&gEnemyParty[i], MON_DATA_HP, NULL);
		}

		if (playerHPCount == 0 && enemyHPCount > 0)
			gBattleOutcome |= B_OUTCOME_LOST;
		else if (playerHPCount > 0 && enemyHPCount == 0)
			gBattleOutcome |= B_OUTCOME_WON;
		else if (playerHPCount == 0 && enemyHPCount == 0)
		{
			if (SIDE(gNewBS->lastFainted) == B_SIDE_PLAYER)
				gBattleOutcome |= B_OUTCOME_WON;
			else
				gBattleOutcome |= B_OUTCOME_LOST;
		}
	
		gBattlescriptCurrInstr += 5;
		return TRUE;
	}

	return FALSE;
}

bool8 ShouldDisablePartyMenuItemsBattleTower(void)
{
	return FlagGet(BATTLE_TOWER_FLAG);
}

const u8* GetFrontierTierName(u8 tier, u8 format)
{
	const u8* string = gBattleFrontierTierNames[tier];

	if (tier == BATTLE_TOWER_MIDDLE_CUP && !IsFrontierSingles(format))
		string = gText_GSCup;
		
	return string;
}

bool8 InBattleSands(void)
{
	return (gBattleTypeFlags & BATTLE_TYPE_BATTLE_SANDS) != 0;
}

bool8 IsAverageMonsBattle(void)
{
	return FlagGet(BATTLE_TOWER_FLAG) && VarGet(BATTLE_TOWER_TIER) == BATTLE_TOWER_AVERAGE_MONS;
}

bool8 Is350CupBattle(void)
{
	return FlagGet(BATTLE_TOWER_FLAG) && VarGet(BATTLE_TOWER_TIER) == BATTLE_TOWER_350_CUP;
}

bool8 IsScaleMonsBattle(void)
{
	return FlagGet(BATTLE_TOWER_FLAG) && VarGet(BATTLE_TOWER_TIER) == BATTLE_TOWER_SCALEMONS;
}

bool8 IsBenjaminButterfreeBattle(void)
{
	return (gBattleTypeFlags & BATTLE_TYPE_BENJAMIN_BUTTERFREE) != 0;
}

//@Details: Generates a tower trainer id and name for the requested trainer.
//			Also buffers the trainer name to gStringVar1 (BUFFER1).
//@Inputs:
//		Var8000: 0 = Trainer Opponent A
//				 1 = Trainer Opponent B
//				 2 = Partner Trainer
//		Var8001: 0 = Regular Trainers
//				 1 = Special Trainers
//				 2 = Frontier Brain
//		Var8002: If Var8001 == Frontier Brain: Frontier Brain Id
//@Returns: To given var OW sprite num of generated trainer.
u16 sp052_GenerateTowerTrainer(void)
{
	u8 battler = Var8000;
	u16 id = Random();
	
	if (Var8001 == 0)
	{
		id %= NUM_TOWER_TRAINERS;
		VarSet(TOWER_TRAINER_ID_VAR + battler, id);
		
		if (gTowerTrainers[id].gender == BATTLE_TOWER_MALE)
		{
			if (battler == 0)
				VarSet(BATTLE_TOWER_TRAINER1_NAME, Random() % NUM_MALE_NAMES);
			else
				VarSet(BATTLE_TOWER_TRAINER2_NAME, Random() % NUM_MALE_NAMES);
		}
		else
		{
			if (battler == 0)
				VarSet(BATTLE_TOWER_TRAINER1_NAME, Random() % NUM_FEMALE_NAMES);
			else
				VarSet(BATTLE_TOWER_TRAINER2_NAME, Random() % NUM_FEMALE_NAMES);
		}
		
		StringCopy(gStringVar1, GetFrontierTrainerName(BATTLE_TOWER_TID, battler));
		return gTowerTrainers[id].owNum;
	}
	else if (Var8001 == 1)
	{
		id %= NUM_SPECIAL_TOWER_TRAINERS;
		while (VarGet(BATTLE_TOWER_TIER) == BATTLE_TOWER_MONOTYPE && !gSpecialTowerTrainers[id].isMonotype)
		{
			id = Random();
			id %= NUM_SPECIAL_TOWER_TRAINERS;
		}

		VarSet(TOWER_TRAINER_ID_VAR + battler, id);
		StringCopy(gStringVar1, GetFrontierTrainerName(BATTLE_TOWER_SPECIAL_TID, battler));
		return gSpecialTowerTrainers[id].owNum;
	}
	else
	{
		id = Var8002;
		VarSet(TOWER_TRAINER_ID_VAR + battler, id);
		StringCopy(gStringVar1, GetFrontierTrainerName(FRONTIER_BRAIN_TID, battler));
		return gFrontierBrains[id].owNum;
	}
}

//@Details: Loads the battle intro message of the requested trainer.
//@Inputs:
//		Var8000: 0 = Trainer Opponent A
//				 1 = Trainer Opponent B
//		Var8001: 0 = Regular Trainers
//				 1 = Special Trainers
//				 2 = Frontier Brain
void sp053_LoadFrontierIntroBattleMessage(void)
{
	u8 gender;
	u16 id = VarGet(TOWER_TRAINER_ID_VAR + Var8000);
	
	const u8* text;
	if (Var8001 == 0)
	{
		text = gTowerTrainers[id].preBattleText;
		gender = gTowerTrainers[id].gender;
	}
	else if (Var8001 == 1)
	{
		text = gSpecialTowerTrainers[id].preBattleText;
		gender = gSpecialTowerTrainers[id].gender;
	}
	else
	{
		text = gFrontierBrains[id].preBattleText;
		gender = gFrontierBrains[id].gender;
	}
	
	gLoadPointer = text;
	
	//Change text colour
	if (gender == BATTLE_TOWER_MALE)
	{
		gTextColourBackup = gTextColourCurrent;
		gTextColourCurrent = 0; //Blue
	}
	else
	{
		gTextColourBackup = gTextColourCurrent;
		gTextColourCurrent = 1; //Red
	}
}

//u16 Streaks[BATTLE_STYLE (4)][TIER (6)][PARTY_SIZE (2)][LEVEL (4)][CURRENT_OR_MAX (2)]

//@Details: Gets the streak for the requested Battle Tower format.
//@Input:
//		Var8000: 0 = Current Streak
//				 1 = Max Streak
//		Var8001: 0xFFFF = Load Style From Var
//				 0+ = Given Style
//		Var8002: 0xFFFF = Load Tier From Var
//				 0+ = Given Tier
//		Var8003: 1 - 6 = Party Size (Options are split into 6 v 6 and NOT 6 v 6)
//				 0xFFFF = Load Party Size From Var
//		Var8004: 0 = Load level from var
//				 1+ = Given Level
u16 sp054_GetBattleTowerStreak(void)
{
	return GetBattleTowerStreak(Var8000, Var8001, Var8002, Var8003, Var8004);
}

u16 GetCurrentBattleTowerStreak(void)
{
	return GetBattleTowerStreak(CURR_STREAK, 0xFFFF, 0xFFFF, 0xFFFF, 0);
}

u16 GetMaxBattleTowerStreakForTier(u8 tier)
{
	u8 battleType, level, partySize;
	u16 streak = 0;
	u16 max = 0;
	
	for (battleType = 0; battleType < NUM_TOWER_BATTLE_TYPES; ++battleType)
	{
		for (level = 50; level <= 100; level += 50) 
		{
			for (partySize = 3; partySize <= 6; partySize += 3) //3 represents one record, 6 represents another
			{
				streak = GetBattleTowerStreak(MAX_STREAK, battleType, tier, partySize, level);
				
				if (streak > max)
					max = streak;
			}	
		}
	}

	return max;
}

static u8 AdjustLevelForTier(u8 level, u8 tier)
{
	if (tier == BATTLE_TOWER_LITTLE_CUP)
		return 5;
		
	if (tier == BATTLE_TOWER_MONOTYPE)
		return 100;

	return level;
}

u16 GetBattleTowerStreak(u8 currentOrMax, u16 inputBattleStyle, u16 inputTier, u16 partySize, u8 level)
{
	u8 battleStyle = (inputBattleStyle == 0xFFFF) ? VarGet(BATTLE_TOWER_BATTLE_TYPE) : inputBattleStyle;
	u8 tier = (inputTier == 0xFFFF) ? VarGet(BATTLE_TOWER_TIER) : inputTier;
	u8 size = (partySize == 0xFFFF) ? VarGet(BATTLE_TOWER_POKE_NUM) : partySize;	
	level = (level == 0) ? VarGet(BATTLE_TOWER_POKE_LEVEL) : level;
	level = AdjustLevelForTier(level, tier);
	
	LoadProperStreakData(&currentOrMax, &battleStyle, &tier, &size, &level);
	
	switch (BATTLE_FACILITY_NUM) {
		case IN_BATTLE_TOWER:
		default:
			return gBattleTowerStreaks[battleStyle][tier][size][level][currentOrMax];
		case IN_BATTLE_SANDS:
			return gBattleSandsStreaks[currentOrMax].streakLength;
	}
}

//@Details: Updates the streak for the current Battle Tower format.
//@Input:
//		Var8000: 0 = Increment by 1
//				 1 = Reset
void sp055_UpdateBattleTowerStreak(void)
{
	u8 dummy = 0;
	u8 battleStyle = VarGet(BATTLE_TOWER_BATTLE_TYPE);
	u8 tier = VarGet(BATTLE_TOWER_TIER);
	u8 partySize = VarGet(BATTLE_TOWER_POKE_NUM);
	u8 level = AdjustLevelForTier(VarGet(BATTLE_TOWER_POKE_LEVEL), tier);
	LoadProperStreakData(&dummy, &battleStyle, &tier, &partySize, &level);

	u16* currentStreak, *maxStreak; 
	bool8 inBattleSands = FALSE;
	
	switch (BATTLE_FACILITY_NUM) {
		case IN_BATTLE_TOWER:
		default:
			currentStreak = &gBattleTowerStreaks[battleStyle][tier][partySize][level][CURR_STREAK]; //Current Streak
			maxStreak = &gBattleTowerStreaks[battleStyle][tier][partySize][level][MAX_STREAK]; //Max Streak
			break;
		case IN_BATTLE_SANDS:
			currentStreak = &gBattleSandsStreaks[CURR_STREAK].streakLength;
			maxStreak = &gBattleSandsStreaks[MAX_STREAK].streakLength;
			inBattleSands = TRUE;
			break;
	}
	
	switch (Var8000) {
		case 0:
			if (*currentStreak < 0xFFFF) //Prevent overflow
				*currentStreak += 1;
			
			if (*maxStreak < *currentStreak)
			{
				*maxStreak = *currentStreak;
				
				if (inBattleSands)
				{
					gBattleSandsStreaks[MAX_STREAK].tier = VarGet(BATTLE_TOWER_TIER); //Actual Tier
					gBattleSandsStreaks[MAX_STREAK].format = battleStyle;
					gBattleSandsStreaks[MAX_STREAK].level = level;
					gBattleSandsStreaks[MAX_STREAK].inverse = FlagGet(INVERSE_FLAG);
					gBattleSandsStreaks[MAX_STREAK].species1 = GetMonData(&gPlayerParty[0], MON_DATA_SPECIES, NULL);
					gBattleSandsStreaks[MAX_STREAK].species2 = GetMonData(&gPlayerParty[1], MON_DATA_SPECIES, NULL);
					gBattleSandsStreaks[MAX_STREAK].species3 = GetMonData(&gPlayerParty[2], MON_DATA_SPECIES, NULL);
				}
			}
			break;
			
		case 1:
			*currentStreak = 0; //Rest current streak

			if (inBattleSands)
			{
				gBattleSandsStreaks[CURR_STREAK].tier = VarGet(BATTLE_TOWER_TIER); //Actual Tier
				gBattleSandsStreaks[CURR_STREAK].format = battleStyle;
				gBattleSandsStreaks[CURR_STREAK].level = level;
				gBattleSandsStreaks[CURR_STREAK].inverse = FlagGet(INVERSE_FLAG);
				gBattleSandsStreaks[CURR_STREAK].species1 = GetMonData(&gPlayerParty[0], MON_DATA_SPECIES, NULL);
				gBattleSandsStreaks[CURR_STREAK].species2 = GetMonData(&gPlayerParty[1], MON_DATA_SPECIES, NULL);
				gBattleSandsStreaks[CURR_STREAK].species3 = GetMonData(&gPlayerParty[2], MON_DATA_SPECIES, NULL);
			}
			break;
	}
}

//@Details: Determines the number of battle points to give for 
//			the current Battle Tower format.
//@Returns: The number of battle points to give.
u16 sp056_DetermineBattlePointsToGive(void)
{
	u16 toGive;
	u16 streakLength = GetCurrentBattleTowerStreak();

	if (streakLength <= 10)
		toGive = 2;
	else if (streakLength <= 19)
		toGive = 3;
	else if (streakLength == 20)
	{
		toGive = 3; //Just a special trainer

		switch (BATTLE_FACILITY_NUM) {
			case IN_BATTLE_TOWER:
				if (VarGet(BATTLE_TOWER_TIER) != BATTLE_TOWER_MONOTYPE)
					toGive = 20; //Battle against frontier brain
				break;
				
			case IN_BATTLE_SANDS:
				if (VarGet(BATTLE_TOWER_TIER) != BATTLE_TOWER_MONOTYPE)
					toGive = 20; //Battle against frontier brain
				break;
		}
	}
	else if (streakLength <= 30)
		toGive = 4;
	else if (streakLength <= 40)
		toGive = 5;
	else if (streakLength <= 49)
		toGive = 6;
	else if (streakLength == 50)
	{
		toGive = 6; //Just a special trainer

		switch (BATTLE_FACILITY_NUM) {
			case IN_BATTLE_TOWER:
				if (VarGet(BATTLE_TOWER_TIER) != BATTLE_TOWER_MONOTYPE)
					toGive = 50; //Battle against frontier brain
				break;
				
			case IN_BATTLE_SANDS:
				if (VarGet(BATTLE_TOWER_TIER) != BATTLE_TOWER_MONOTYPE)
					toGive = 50; //Battle against frontier brain
				break;
		}
	}
	else if (streakLength <= 70)
		toGive = 7;
	else if (streakLength <= 80)
		toGive = 8;
	else if (streakLength <= 99)
		toGive = 9;
	else if (streakLength == 100)
		toGive = 100;
	else
		toGive = 10;
		
	return toGive;
}

static void LoadProperStreakData(u8* currentOrMax, u8* battleStyle, u8* tier, u8* partySize, u8* level)
{
	if (*tier == BATTLE_TOWER_MONOTYPE)
	{
		*tier = BATTLE_TOWER_LITTLE_CUP; //Hijack Little Cup level 100 slot
		*level = 100;
	}
	
	if (IsFrontierMulti(*battleStyle))
		*partySize *= 2; //Each player gets half the team

	*currentOrMax = MathMin(*currentOrMax, 1);
	*battleStyle = MathMin(*battleStyle, NUM_TOWER_BATTLE_TYPES);
	*tier = MathMin(*tier, NUM_FORMATS);
	*partySize = (*partySize < 6) ? 0 : 1;
	*level = (*level < 100) ? 0 : 1;
}

//@Details: To be used after sp06B. Merges the player's choice of partner Pokemon onto their team.
void sp06C_SpliceFrontierTeamWithPlayerTeam(void)
{
	struct Pokemon partnerPokes[3];
	Memset(partnerPokes, 0, sizeof(struct Pokemon) * 3);

	for (int i = 0; i < 3; ++i)
	{
		if (gSelectedOrderFromParty[i] != 0)
			partnerPokes[i] = gPlayerParty[i]; //Player's party has already been remodeled by the special so call indices directly
	}

	RestorePartyFromTempTeam(0, 0, 3);
	Memcpy(&gPlayerParty[3], partnerPokes, sizeof(struct Pokemon) * 3); //Fill second half of team with multi mons
	
	//Recalculate party count the special way because there may be a gap in the party
	gPlayerPartyCount = 0;
	for (int i = 0; i < PARTY_SIZE; ++i)
	{
		if (GetMonData(&gPlayerParty[i], MON_DATA_SPECIES, NULL) != SPECIES_NONE)
			++gPlayerPartyCount;
	}
}

//@Details: Loads any relevant multi trainer data by the given Id value.
//			Also byffers the multi trainer name to gStringVar2 ([BUFFER2] / bufferstring 0x1).
//@Inputs:
//		Var8000: 0xFF - Random Id.
//				 0-0xFE - Given Id.
//@Returns: The OW id of the trainer.
u16 sp06D_LoadFrontierMultiTrainerById(void)
{
	u16 id = Var8000;
	
	if (id == 0xFF)
		id = Random() % gNumFrontierMultiTrainers;
	else if (id > 0xFF) //Invalid value
		id = 0;

	VarSet(TOWER_TRAINER_ID_PARTNER_VAR, id);
	VarSet(PARTNER_VAR, BATTLE_TOWER_MULTI_TRAINER_TID);
	VarSet(PARTNER_BACKSPRITE_VAR, gFrontierMultiBattleTrainers[id].backSpriteId);

	StringCopy(gStringVar2, GetFrontierTrainerName(BATTLE_TOWER_MULTI_TRAINER_TID, 0));
	return gFrontierMultiBattleTrainers[id].owNum;
}

//@Details: Buffers text relating to battle sands records.
//@Inputs:
//		Var8000: 0 = Previous Streak
//				 1 = Max Streak
//@Returns: LastResult: TRUE if the requested record exists.
//			gStringVar1: Tier name.
//			gStringVar2: Battle format name.
//			gStringVar3: Level.
//			gStringVar7: Inverse on or off.
//			gStringVar8: Species 1.
//			gStringVar9: Species 2.
//			gStringVarA: Species 3.
//			gStringVarB: Streak length.
//			gStringVarC: "previous" or "max"
void sp06E_BufferBattleSandsRecords(void)
{
	static const u8* const requestStrings[] =
	{
		gText_Previous,
		gText_Max,
	};

	gSpecialVar_LastResult = FALSE;

	struct BattleSandsStreak* streak = (Var8000 == 0) ? &gBattleSandsStreaks[CURR_STREAK] : &gBattleSandsStreaks[MAX_STREAK];
	if (streak->species1 != SPECIES_NONE)
	{
		StringCopy(gStringVar1, GetFrontierTierName(streak->tier, streak->format));
		StringCopy(gStringVar2, gBattleFrontierFormats[streak->format]);
		ConvertIntToDecimalStringN(gStringVar3, (streak->level == 0) ? 50 : 100, 0, 3);
		StringCopy(gStringVar7, (streak->inverse) ? gText_On : gText_Off);
		GetSpeciesName(gStringVar8, streak->species1);
		GetSpeciesName(gStringVar9, streak->species2);
		if (IsFrontierSingles(streak->format))
			StringCopy(gStringVarA, gText_None);
		else
			GetSpeciesName(gStringVarA, streak->species3);
		ConvertIntToDecimalStringN(gStringVarB, streak->streakLength, 0, 5);
		StringCopy(gStringVarC, requestStrings[(Var8000 == 0) ? 0 : 1]);

		gSpecialVar_LastResult = TRUE;
	}
}
