#include "defines.h"
#include "helper_functions.h"

extern const struct CompressedSpriteSheet gBattleAnimPicTable[];
extern const struct CompressedSpritePalette gBattleAnimPaletteTable[];

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

#define GET_TRUE_SPRITE_INDEX(i) ((i - ANIM_SPRITES_START))
void ScriptCmd_loadspritegfx(void)
{
    u16 index;

    sBattleAnimScriptPtr++;
    index = T1_READ_16(sBattleAnimScriptPtr);
    LoadCompressedSpriteSheetUsingHeap(&gBattleAnimPicTable[GET_TRUE_SPRITE_INDEX(index)]);
    LoadCompressedSpritePaletteUsingHeap(&gBattleAnimPaletteTable[GET_TRUE_SPRITE_INDEX(index)]);
    sBattleAnimScriptPtr += 2;
    AddSpriteIndex(GET_TRUE_SPRITE_INDEX(index));
    gAnimFramesToWait = 1;
    gAnimScriptCallback = (u32) WaitAnimFrameCount;
}

void ShinyAnimFix(void)
{
	LoadCompressedSpriteSheetUsingHeap(&gBattleAnimPicTable[233]);
    LoadCompressedSpritePaletteUsingHeap(&gBattleAnimPaletteTable[233]);
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

void AnimTask_IsTargetPartner(u8 taskId)
{
	if (gBattleAnimTarget == PARTNER(gBattleAnimAttacker))
		gBattleAnimArgs[0] = 1;
	else
		gBattleAnimArgs[0] = 0;

	DestroyAnimVisualTask(taskId);
}

void AnimTask_GetTrappedMoveAnimId(u8 taskId)
{
	switch (gBattleSpritesDataPtr->animationData->animArg) {
		case MOVE_FIRESPIN:
			gBattleAnimArgs[0] = 1;
			break;
		case MOVE_WHIRLPOOL:
			gBattleAnimArgs[0] = 2;
			break;
		case MOVE_CLAMP:
			gBattleAnimArgs[0] = 3;
			break;
		case MOVE_SANDTOMB:
			gBattleAnimArgs[0] = 4;
			break;
		case MOVE_MAGMASTORM:
			gBattleAnimArgs[0] = 5;
			break;
		case MOVE_INFESTATION:
			gBattleAnimArgs[0] = 6;
			break;			
		default:
			gBattleAnimArgs[0] = 0;
	}

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

bool8 IsAnimMoveTectnoicRage(void)
{
	u16 move = gBattleBufferA[gActiveBattler][1] | (gBattleBufferA[gActiveBattler][2] << 8);
	return  move == MOVE_TECTONIC_RAGE_P || move == MOVE_TECTONIC_RAGE_S;
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

bool8 IsMoveNeverEndingNightmareOrDevastatingDrakeOrLightThatBurnsTheSky(void)
{
	u16 move = gBattleBufferA[gActiveBattler][1] | (gBattleBufferA[gActiveBattler][2] << 8);
	return  move == MOVE_NEVER_ENDING_NIGHTMARE_P || move == MOVE_NEVER_ENDING_NIGHTMARE_S 
		 || move == MOVE_DEVASTATING_DRAKE_P 	  || move == MOVE_DEVASTATING_DRAKE_S
		 || move == MOVE_LIGHT_THAT_BURNS_THE_SKY;
}

bool8 IsAnimMoveDestinyBond(void)
{
	u16 move = gBattleBufferA[gActiveBattler][1] | (gBattleBufferA[gActiveBattler][2] << 8);
	return  move == MOVE_DESTINYBOND;
}

bool8 IsAnimMoveThunderWave(void)
{
	u16 move = gBattleBufferA[gActiveBattler][1] | (gBattleBufferA[gActiveBattler][2] << 8);
	return  move == MOVE_THUNDERWAVE;
}

bool8 IsAnimMoveGrudge(void)
{
	u16 move = gBattleBufferA[gActiveBattler][1] | (gBattleBufferA[gActiveBattler][2] << 8);
	return  move == MOVE_GRUDGE;
}

bool8 IsAnimMoveFairyLock(void)
{
	u16 move = gBattleBufferA[gActiveBattler][1] | (gBattleBufferA[gActiveBattler][2] << 8);
	return  move == MOVE_FAIRYLOCK;
}

bool8 IsAnimMoveLightOfRuin(void)
{
	u16 move = gBattleBufferA[gActiveBattler][1] | (gBattleBufferA[gActiveBattler][2] << 8);
	return  move == MOVE_LIGHTOFRUIN;
}

bool8 IsAnimMoveFlashCannon(void)
{
	u16 move = gBattleBufferA[gActiveBattler][1] | (gBattleBufferA[gActiveBattler][2] << 8);
	return  move == MOVE_FLASHCANNON;
}

bool8 IsAnimMoveSkillSwap(void)
{
	u16 move = gBattleBufferA[gActiveBattler][1] | (gBattleBufferA[gActiveBattler][2] << 8);
	return  move == MOVE_SKILLSWAP;
}

bool8 IsAnimMovePowerSwap(void)
{
	u16 move = gBattleBufferA[gActiveBattler][1] | (gBattleBufferA[gActiveBattler][2] << 8);
	return  move == MOVE_POWERSWAP;
}

bool8 IsAnimMoveHeartSwap(void)
{
	u16 move = gBattleBufferA[gActiveBattler][1] | (gBattleBufferA[gActiveBattler][2] << 8);
	return  move == MOVE_HEARTSWAP;
}

void AnimTask_ReloadAttackerSprite(u8 taskId)
{
	u8 spriteId = gBattlerSpriteIds[gBattleAnimAttacker];
	
	struct Task* task = &gTasks[taskId];
	struct Task* newTask;
	
	switch (task->data[10]) {
		case 0:
			// To fix an annoying graphical glitch where the old sprite would flash
			// momentarily, we hide the sprite offscreen while we refresh it.
			// Remember the old position so we can go back to it later.
			task->data[11] = gSprites[spriteId].pos1.x;
			gSprites[spriteId].pos1.x = -64;
				
			// Load the palette and graphics. Note this doesn't cause the sprite to
			// refresh
			LoadBattleMonGfxAndAnimate(gBattleAnimAttacker, 1, spriteId);
			++task->data[10];
			break;
		
		case 1:
			// Actually update the sprite now
			gSprites[spriteId].invisible = FALSE;
			newTask = &gTasks[CreateTask(sub_807331C, 5)];
			newTask->data[0] = 0;
			newTask->data[2] = gBattleAnimAttacker;
			++task->data[10];
			break;
		
		case 2:
			// Make sure the task is done. I'm not sure if this is necessary
			if (!FuncIsActiveTask(sub_807331C))
				++task->data[10];
			break;
		
		case 3:
			// Restore the old X position and end the task
			gSprites[spriteId].pos1.x = task->data[11];
			DestroyAnimVisualTask(taskId);
	}
}

void AnimTask_PlayAttackerCry(u8 taskId)
{
	PlayCry3(GetBankPartyData(gBattleAnimAttacker)->species, 0, 0);
	DestroyAnimVisualTask(taskId);
}

void DoubleWildAnimBallThrowFix(void)
{
	if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE && !(gBattleTypeFlags & BATTLE_TYPE_TRAINER))
	{
		if (gBattleMons[GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT)].hp)
			gBattleAnimTarget = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
		else
			gBattleAnimTarget = GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT);
	}
}