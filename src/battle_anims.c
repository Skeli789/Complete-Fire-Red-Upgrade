#include "defines.h"
#include "helper_functions.h"

bank_t LoadBattleAnimTarget(u8 arg)
{
    u8 battler;
	if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
	{
		switch (gBattleAnimArgs[arg]) {
			case 0:
				battler = gBattleAnimAttacker;
				break;
			default:
				battler = gBattleAnimTarget;
				break;
			case 2:
				battler = PARTNER(gBattleAnimAttacker);
				break;
			case 3:
				battler = PARTNER(gBattleAnimTarget);
				break;
		}
	}
	else
	{
		if (gBattleAnimArgs[arg] == 0)
			battler = gBattleAnimAttacker;
		else
			battler = gBattleAnimTarget;
	}
	return battler;
}

/*
void ScriptCmd_pokespritefromBGsingle(void)
{
    u8 animBattlerId;
    u8 battlerId;
    u8 taskId;

    sBattleAnimScriptPtr++;
    animBattlerId = LoadBattleAnimTarget(sBattleAnimScriptPtr[0]);

    if (sMonAnimTaskIdArray[0] != 0xFF)
        gSprites[gBattlerSpriteIds[animBattlerId]].invisible = FALSE;

    taskId = CreateTask(sub_80A4980, 5);
    gTasks[taskId].data[0] = animBattlerId;
    gTasks[taskId].data[2] = animBattlerId;

    sBattleAnimScriptPtr++;
}*/

void AnimTask_TechnoBlast(u8 taskId)
{
    gBattleAnimArgs[0] = gItems[GetBankPartyData(gBattleAnimAttacker)->item].holdEffectParam;
    DestroyAnimVisualTask(taskId);
}

void AnimTask_GetTimeOfDay(u8 taskId)
{
	gBattleAnimArgs[0] = 0;
	
	#ifdef TIME_ENABLED
		if (Clock->hour >= TIME_NIGHT_START || Clock->hour < TIME_MORNING_START)
			gBattleAnimArgs[0] = 1;
		else if (Clock->hour >= TIME_AFTERNOON_START)
			gBattleAnimArgs[0] = 2;
	#endif

	DestroyAnimVisualTask(taskId);
}

void AnimTask_GetLycanrocForm(u8 taskId)
{
	if (GetBankPartyData(gBattleAnimAttacker)->species == PKMN_LYCANROC_N)
		gBattleAnimArgs[0] = 1;
	else
		gBattleAnimArgs[0] = 0;
	
	DestroyAnimVisualTask(taskId);
}

bool8 ShadowSneakAnimHelper(void)
{
	switch (gBattleBufferA[gActiveBattler][1] | (gBattleBufferA[gActiveBattler][2] << 8)) {
		case MOVE_SHADOWSNEAK:
		case MOVE_HYPERSPACEHOLE:
		case MOVE_PULVERIZING_PANCAKE:
			return TRUE;
	}
	
	return FALSE;
}

bool8 IsAnimMoveIonDeluge(void)
{
	u16 move = gBattleBufferA[gActiveBattler][1] | (gBattleBufferA[gActiveBattler][2] << 8);
	return move == MOVE_IONDELUGE;
}

bool8 IsAnimMoveBloomDoom(void)
{
	u16 move = gBattleBufferA[gActiveBattler][1] | (gBattleBufferA[gActiveBattler][2] << 8);
	return  move == MOVE_BLOOM_DOOM_P || move == MOVE_BLOOM_DOOM_S;
}

bool8 IsAnimMoveOceanicOperretta(void)
{
	u16 move = gBattleBufferA[gActiveBattler][1] | (gBattleBufferA[gActiveBattler][2] << 8);
	return  move == MOVE_OCEANIC_OPERETTA;
}

bool8 IsMoveNeverEndingNightmareOrDevastatingDrake(void)
{
	u16 move = gBattleBufferA[gActiveBattler][1] | (gBattleBufferA[gActiveBattler][2] << 8);
	return  move == MOVE_NEVER_ENDING_NIGHTMARE_P || move == MOVE_NEVER_ENDING_NIGHTMARE_S 
		 || move == MOVE_DEVASTATING_DRAKE_P 	  || move == MOVE_DEVASTATING_DRAKE_S;
}