#include "defines.h"
#include "helper_functions.h"
#include "frontier.h"

//Maybe add stuff in for Frontier brains

extern u8* gMaleFrontierNamesTable[];
extern u8* gFemaleFrontierNamesTable[];

u8 GetFrontierTrainerClassId(u16 trainerId, u8 battlerNum);
void GetFrontierTrainerName(u8* dst, u16 trainerId, u8 battlerNum);
void CopyFrontierTrainerText(u8 whichText, u16 trainerId, u8 battlerNum);
u8 GetFrontierTrainerFrontSpriteId(u16 trainerId, u8 battlerNum);

u8 GetFrontierTrainerClassId(u16 trainerId, u8 battlerNum) 
{
    if (trainerId != BATTLE_TOWER_TID)
        return gTrainers[trainerId].trainerClass;
    else
		return gTowerTrainers[VarGet(TOWER_TRAINER_ID_VAR + battlerNum)].trainerClass;
}

void GetFrontierTrainerName(u8* dst, u16 trainerId, u8 battlerNum)
{
    int i = 0;

    if (trainerId != BATTLE_TOWER_TID)
    {
        for (i = 0; i < PLAYER_NAME_LENGTH; i++)
            dst[i] = gTrainers[trainerId].trainerName[i];
    }
    else
    {
		u8* name;
		
		if (gTowerTrainers[VarGet(TOWER_TRAINER_ID_VAR + battlerNum)].gender == MALE)
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

		for (i = 0; i < PLAYER_NAME_LENGTH; i++)
            dst[i] = name[i];
    }

    dst[i] = EOS;
}

void CopyFrontierTrainerText(u8 whichText, u16 trainerId, u8 battlerNum)
{
	//TO DO: Add in custom strings for Frontier Brains
	if (trainerId == BATTLE_TOWER_TID) {
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
	}
}

u8 GetFrontierTrainerFrontSpriteId(u16 trainerId, u8 battlerNum)
{
    if (trainerId != BATTLE_TOWER_TID)
        return gTrainers[trainerId].trainerPic;
    else
		return gTowerTrainers[VarGet(TOWER_TRAINER_ID_VAR + battlerNum)].trainerSprite;
}

u32 GetAIFlagsInBattleFrontier(u8 bank) {
	return AI_SCRIPT_CHECK_BAD_MOVE;
}
