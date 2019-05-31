#include "defines.h"
#include "defines_battle.h"
#include "../include/event_data.h"
#include "../include/random.h"
#include "../include/script.h"

#include "../include/new/helper_functions.h"
#include "../include/new/frontier.h"

extern u8* gMaleFrontierNamesTable[];
extern u8* gFemaleFrontierNamesTable[];

//This file's functions:
static u16 GetBattleTowerStreak(u8 currentOrMax, u16 battleStyle, u16 tier, u8 inverse, u8 level);
static void LoadProperStreakData(u8* currentOrMax, u8* battleStyle, u8* tier, u8* inverse, u8* level);

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
					StringCopy(gStringVar4, GetTrainerALoseText());
					break;
			
				case FRONTIER_PLAYER_WON_TEXT:
					StringCopy(gStringVar4, GetTrainerAWinText());
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
	return AI_SCRIPT_CHECK_BAD_MOVE;
}

u8 GetNumMonsOnTeamInFrontier(void)
{
	return MathMin(MathMax(1, VarGet(BATTLE_TOWER_POKE_NUM)), PARTY_SIZE);
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

enum
{
	CURR_STREAK,
	MAX_STREAK,
};

//u16 Streaks[BATTLE_STYLE (4)][TIER (6)][INVERSE (2)][LEVEL (4)][CURRENT_OR_MAX (2)]

//@Details: Gets the streak for the requested Battle Tower format.
//@Input:
//		Var8000: 0 = Current Streak
//				 1 = Max Streak
//		Var8001: 0xFFFF = Load Style From Var
//				 0+ = Given Style
//		Var8002: 0xFFFF = Load Tier From Var
//				 0+ = Given Tier
//		Var8003: 0 = Not Inverse
//				 1 = Inverse
//				 2 = Load Inverse From Flag
//		Var8004: 0 = Load level from var
//				 1+ = Given Level
u16 sp054_GetBattleTowerStreak(void)
{
	return GetBattleTowerStreak(Var8000, Var8001, Var8002, Var8003, Var8004);
}

u16 GetCurrentBattleTowerStreak(void)
{
	return GetBattleTowerStreak(CURR_STREAK, 0xFFFF, 0xFFFF, 2, 0);
}

u16 GetMaxBattleTowerStreakForTier(u8 tier)
{
	bool8 inverse;
	u8 battleType, level;
	u16 streak = 0;
	u16 max = 0;
	
	for (battleType = 0; battleType < NUM_TOWER_BATTLE_TYPES; ++battleType)
	{
		for (level = 50; level <= 100; level += 50) 
		{
			for (inverse = FALSE; inverse <= TRUE; ++inverse)
			{
				streak = GetBattleTowerStreak(MAX_STREAK, battleType, tier, inverse, level);
				
				if (streak > max)
					max = streak;
			}	
		}
	}

	return max;
}

static u16 GetBattleTowerStreak(u8 currentOrMax, u16 inputBattleStyle, u16 inputTier, u8 inverse, u8 level)
{
	u8 battleStyle = (inputBattleStyle == 0xFFFF) ? VarGet(BATTLE_TOWER_BATTLE_TYPE) : inputBattleStyle;
	u8 tier = (inputTier == 0xFFFF) ? VarGet(BATTLE_TOWER_TIER) : inputTier;
	inverse = (inverse == 2) ? FlagGet(INVERSE_FLAG) : inverse;	
	level = (level == 0) ? VarGet(BATTLE_TOWER_POKE_LEVEL) : level;
	LoadProperStreakData(&currentOrMax, &battleStyle, &tier, &inverse, &level);

	return gBattleTowerStreaks[battleStyle][tier][inverse][level][currentOrMax];
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
	u8 inverse = FlagGet(INVERSE_FLAG);
	u8 level = VarGet(BATTLE_TOWER_POKE_LEVEL);
	LoadProperStreakData(&dummy, &battleStyle, &tier, &inverse, &level);

	u16* currentStreak = &gBattleTowerStreaks[battleStyle][tier][inverse][level][CURR_STREAK]; //Current Streak
	u16* maxStreak = &gBattleTowerStreaks[battleStyle][tier][inverse][level][MAX_STREAK]; //Max Streak
	
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
		toGive = 20;
	else if (streakLength <= 30)
		toGive = 4;
	else if (streakLength <= 40)
		toGive = 5;
	else if (streakLength <= 49)
		toGive = 6;
	else if (streakLength == 50)
		toGive = 50;
	else
		toGive = 7;
		
	return toGive;
}

static void LoadProperStreakData(u8* currentOrMax, u8* battleStyle, u8* tier, u8* inverse, u8* level)
{
	*currentOrMax = MathMin(*currentOrMax, 1);
	*battleStyle = MathMin(*battleStyle, NUM_TOWER_BATTLE_TYPES);
	*tier = MathMin(*tier, NUM_FORMATS);
	*inverse = MathMin(*inverse, TRUE);
	*level = (*level <= 25) ? 0 : (*level <= 50) ? 1 : (*level <= 75) ? 2 : 3;
}
