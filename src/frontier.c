#include "defines.h"
#include "defines_battle.h"
#include "../include/event_data.h"
#include "../include/random.h"
#include "../include/script.h"

#include "../include/new/Helper_Functions.h"
#include "../include/new/frontier.h"

extern u8* gMaleFrontierNamesTable[];
extern u8* gFemaleFrontierNamesTable[];

//This file's functions:
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
		case BATTLE_TOWER_TID:
			if (gTowerTrainers[VarGet(TOWER_TRAINER_ID_VAR + battlerNum)].gender == BATTLE_TOWER_MALE)
			{
				if (VarGet(BATTLE_TOWER_TRAINER_NAME) == 0xFFFF)
					VarSet(BATTLE_TOWER_TRAINER_NAME, umodsi(MathMin(Random(), NUM_MALE_NAMES), 0xFFFE));
				
				name = gMaleFrontierNamesTable[VarGet(BATTLE_TOWER_TRAINER_NAME)];
			}
			else
			{
				if (VarGet(BATTLE_TOWER_TRAINER_NAME) == 0xFFFF)
					VarSet(BATTLE_TOWER_TRAINER_NAME, umodsi(MathMin(Random(), NUM_FEMALE_NAMES), 0xFFFE));
				
				name = gFemaleFrontierNamesTable[VarGet(BATTLE_TOWER_TRAINER_NAME)];
			}
			break;
		case BATTLE_TOWER_SPECIAL_TID:
			name = gSpecialTowerTrainers[VarGet(TOWER_TRAINER_ID_VAR + battlerNum)].name;
			break;
		case FRONTIER_BRAIN_TID:
			name = gFrontierBrains[VarGet(TOWER_TRAINER_ID_VAR + battlerNum)].name;
			break;
		default:
			name = gTrainers[trainerId].trainerName;
	}

	return name;
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
					break;
			
				case FRONTIER_PLAYER_LOST_TEXT:
					StringCopy(gStringVar4, GetTrainerAWinText());
					break;
			
				case FRONTIER_PLAYER_WON_TEXT:
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

//@Details: Generates a tower trainer id and name for the requested trainer.
//@Inputs:
//		Var8000: 0 = Trainer Opponent A
//				 1 = Trainer Opponent B
//				 2 = Partner Trainer
//		Var8001: 0 = Regular Trainers
//				 1 = Special Trainers
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
			VarSet(BATTLE_TOWER_TRAINER_NAME, Random() % NUM_MALE_NAMES);
		else
			VarSet(BATTLE_TOWER_TRAINER_NAME, Random() % NUM_FEMALE_NAMES);
		
		return gTowerTrainers[id].owNum;
	}
	else
	{
		id %= NUM_SPECIAL_TOWER_TRAINERS;
		while (VarGet(BATTLE_TOWER_TIER) == BATTLE_TOWER_MONOTYPE && !gSpecialTowerTrainers[id].isMonotype)
		{
			id = Random();
			id %= NUM_SPECIAL_TOWER_TRAINERS;
		}

		VarSet(TOWER_TRAINER_ID_VAR + battler, id);
		return gSpecialTowerTrainers[id].owNum;
	}
}

//@Details: Loads the battle intro message of the requested trainer.
//@Inputs:
//		Var8000: 0 = Trainer Opponent A
//				 1 = Trainer Opponent B
//		Var8001: 0 = Regular Trainers
//				 1 = Special Trainers
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
	else
	{
		text = gSpecialTowerTrainers[id].preBattleText;
		gender = gSpecialTowerTrainers[id].gender;
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
	return gBattleTowerStreaks[battleStyle][tier][size][level][currentOrMax];
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

	u16* currentStreak = &gBattleTowerStreaks[battleStyle][tier][partySize][level][CURR_STREAK]; //Current Streak
	u16* maxStreak = &gBattleTowerStreaks[battleStyle][tier][partySize][level][MAX_STREAK]; //Max Streak
	
	switch (Var8000) {
		case 0:
			if (*currentStreak < 0xFFFF) //Prevent overflow
				*currentStreak += 1;
			
			if (*maxStreak < *currentStreak)
				*maxStreak = *currentStreak;
			break;
			
		case 1:
			*currentStreak = 0; //Rest current streak
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
		if (VarGet(BATTLE_TOWER_TIER) == BATTLE_TOWER_STANDARD)
			toGive = 20; //Battle against frontier brain
		else
			toGive = 3; //Just a special trainer
	}
	else if (streakLength <= 30)
		toGive = 4;
	else if (streakLength <= 40)
		toGive = 5;
	else if (streakLength <= 49)
		toGive = 6;
	else if (streakLength == 50)
	{
		if (VarGet(BATTLE_TOWER_TIER) == BATTLE_TOWER_STANDARD)
			toGive = 50; //Battle against frontier brain
		else
			toGive = 6; //Just a special trainer
	}
	else
		toGive = 7;
		
	return toGive;
}

static void LoadProperStreakData(u8* currentOrMax, u8* battleStyle, u8* tier, u8* partySize, u8* level)
{
	if (*tier == BATTLE_TOWER_MONOTYPE)
	{
		*tier = BATTLE_TOWER_LITTLE_CUP; //Hijack Little Cup level 100 slot
		*level = 100;
	}

	*currentOrMax = MathMin(*currentOrMax, 1);
	*battleStyle = MathMin(*battleStyle, NUM_TOWER_BATTLE_TYPES);
	*tier = MathMin(*tier, NUM_FORMATS);
	*partySize = (*partySize < 6) ? 0 : 1;
	*level = (*level < 100) ? 0 : 1;
}
