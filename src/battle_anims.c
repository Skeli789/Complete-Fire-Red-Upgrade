#include "defines.h"
#include "defines_battle.h"
#include "../include/battle_anim.h"
#include "../include/event_data.h"
#include "../include/new/helper_functions.h"

extern const struct CompressedSpriteSheet gBattleAnimPicTable[];
extern const struct CompressedSpritePalette gBattleAnimPaletteTable[];
extern const struct TerrainTableStruct TerrainTable[];
extern u8* AttackAnimationTable[];
extern const u16 gCamouflageColours[];

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
	LoadCompressedSpriteSheetUsingHeap(&gBattleAnimPicTable[ANIM_TAG_GOLD_STARS - ANIM_SPRITES_START]);
    LoadCompressedSpritePaletteUsingHeap(&gBattleAnimPaletteTable[ANIM_TAG_GOLD_STARS - ANIM_SPRITES_START]);
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
		else if (Clock->hour >= TIME_EVENING_START)
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
		case MOVE_SPECTRALTHIEF:
		case MOVE_PULVERIZING_PANCAKE:
			return TRUE;
	}
	
	return FALSE;
}

bool8 IsAnimMoveIonDeluge(void)
{
	u16 move = gBattleBufferA[gBattleAnimAttacker][1] | (gBattleBufferA[gBattleAnimAttacker][2] << 8);	
	return gBattleBufferA[gBattleAnimAttacker][0] == CONTROLLER_MOVEANIMATION && move == MOVE_IONDELUGE;
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

bool8 IsAnimMoveMudBomb(void)
{
	u16 move = gBattleBufferA[gActiveBattler][1] | (gBattleBufferA[gActiveBattler][2] << 8);
	return  move == MOVE_MUDBOMB;
}

bool8 IsAnimMoveCoreEnforcer(void)
{
	u16 move = gBattleBufferA[gActiveBattler][1] | (gBattleBufferA[gActiveBattler][2] << 8);
	return  move == MOVE_COREENFORCER;
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

void AnimTask_GetSecretPowerAnimation(u8 taskId)
{
	u16 move;
	
	switch (TerrainType) {
		case ELECTRIC_TERRAIN:
			move = TerrainTable[0].secretPowerAnim;
			break;
		case GRASSY_TERRAIN:
			move = TerrainTable[1].secretPowerAnim;
			break;
		case MISTY_TERRAIN:
			move = TerrainTable[2].secretPowerAnim;
			break;
		case PSYCHIC_TERRAIN:
			move = TerrainTable[3].secretPowerAnim;
			break;
		default:
			move = TerrainTable[gBattleTerrain + 4].secretPowerAnim;
	}
	
	sBattleAnimScriptPtr = AttackAnimationTable[move];
	DestroyAnimVisualTask(taskId);
}

void AnimTask_SetCamouflageBlend(u8 taskId)
{
	u8 entry = 0;
    u32 selectedPalettes = UnpackSelectedBattleAnimPalettes(gBattleAnimArgs[0]);
	
	switch (TerrainType) {
		case ELECTRIC_TERRAIN:
			entry = 0;
			break;
		case GRASSY_TERRAIN:
			entry = 1;
			break;
		case MISTY_TERRAIN:
			entry = 2;
			break;
		case PSYCHIC_TERRAIN:
			entry = 3;
			break;
	}
	
	if (entry)
		gBattleAnimArgs[4] = gCamouflageColours[TerrainTable[entry].camouflageType];
	else
		gBattleAnimArgs[4] = gCamouflageColours[TerrainTable[gBattleTerrain + 4].camouflageType];

	StartBlendAnimSpriteColor(taskId, selectedPalettes);
}

void SpriteCB_TranslateAnimSpriteToTargetMonLocationDoubles(struct Sprite* sprite)
{
	bool8 v1;
	bank_t target;
    u8 coordType;

    if (!(gBattleAnimArgs[5] & 0xff00))
        v1 = TRUE;
    else
        v1 = FALSE;

    if (!(gBattleAnimArgs[5] & 0xff))
        coordType = BATTLER_COORD_Y_PIC_OFFSET;
    else
        coordType = BATTLER_COORD_Y;

    InitSpritePosToAnimAttacker(sprite, v1);
    if (SIDE(gBattleAnimAttacker) != B_SIDE_PLAYER)
        gBattleAnimArgs[2] = -gBattleAnimArgs[2];
		
	target = LoadBattleAnimTarget(6);
	
	if (GetBankPartyData(target)->hp == 0)
		DestroyAnimSprite(sprite);
	else
	{
		sprite->data[0] = gBattleAnimArgs[4];
		sprite->data[2] = GetBattlerSpriteCoord(target, BATTLER_COORD_X_2) + gBattleAnimArgs[2];
		sprite->data[4] = GetBattlerSpriteCoord(target, coordType) + gBattleAnimArgs[3];
		sprite->callback = StartAnimLinearTranslation;
		StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
	}
}

void InitSpritePosToAnimTargetsCentre(struct Sprite *sprite, bool8 respectMonPicOffsets)
{
	if (!respectMonPicOffsets)
	{
		sprite->pos1.x = (GetBattlerSpriteCoord2(gBattleAnimTarget, BATTLER_COORD_X)
					   +  GetBattlerSpriteCoord2(PARTNER(gBattleAnimTarget), BATTLER_COORD_X)) / 2;
		sprite->pos1.y = (GetBattlerSpriteCoord2(gBattleAnimTarget, BATTLER_COORD_Y)
					   +  GetBattlerSpriteCoord2(PARTNER(gBattleAnimTarget), BATTLER_COORD_Y)) / 2;
	}
	
	SetAnimSpriteInitialXOffset(sprite, gBattleAnimArgs[0]);
	sprite->pos1.y += gBattleAnimArgs[1];
}

void InitSpritePosToAnimAttackersCentre(struct Sprite *sprite, bool8 respectMonPicOffsets)
{
	if (!respectMonPicOffsets)
	{
		sprite->pos1.x = (GetBattlerSpriteCoord2(gBattleAnimAttacker, BATTLER_COORD_X)
					   +  GetBattlerSpriteCoord2(PARTNER(gBattleAnimAttacker), BATTLER_COORD_X)) / 2;
		sprite->pos1.y = (GetBattlerSpriteCoord2(gBattleAnimAttacker, BATTLER_COORD_Y)
					   +  GetBattlerSpriteCoord2(PARTNER(gBattleAnimAttacker), BATTLER_COORD_Y)) / 2;
	}
	else
	{
		sprite->pos1.x = (GetBattlerSpriteCoord2(gBattleAnimAttacker, BATTLER_COORD_X_2)
					   +  GetBattlerSpriteCoord2(PARTNER(gBattleAnimAttacker), BATTLER_COORD_X_2)) / 2;
		sprite->pos1.y = (GetBattlerSpriteCoord2(gBattleAnimAttacker, BATTLER_COORD_Y_PIC_OFFSET)
					   +  GetBattlerSpriteCoord2(PARTNER(gBattleAnimAttacker), BATTLER_COORD_Y_PIC_OFFSET)) / 2;
	}
	
	SetAnimSpriteInitialXOffset(sprite, gBattleAnimArgs[0]);
	sprite->pos1.y += gBattleAnimArgs[1];
}

void SpriteCB_SpriteToCentreOfSide(struct Sprite* sprite)
{
    bool8 var;

    if (!sprite->data[0])
    {
        if (!gBattleAnimArgs[3])
            var = TRUE;
        else
            var = FALSE;
		
        if (gBattleAnimArgs[2] == 0) //Attacker
		{
			if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
				InitSpritePosToAnimAttackersCentre(sprite, var);
			else
				InitSpritePosToAnimAttacker(sprite, var);
		}
        else
		{
			if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
				InitSpritePosToAnimTargetsCentre(sprite, var);
			else
				InitSpritePosToAnimTarget(sprite, var);
		}
		
        sprite->data[0]++;
    }
    else if (sprite->animEnded || sprite->affineAnimEnded)
    {
        DestroySpriteAndMatrix(sprite);
    }
}

void SpriteCB_RandomCentredHits(struct Sprite* sprite)
{
    if (gBattleAnimArgs[1] == -1)
        gBattleAnimArgs[1] = Random() & 3;

    StartSpriteAffineAnim(sprite, gBattleAnimArgs[1]);
	
    if (gBattleAnimArgs[0] == 0)
	{
		if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
			InitSpritePosToAnimAttackersCentre(sprite, FALSE);
		else
			InitSpritePosToAnimAttacker(sprite, FALSE);
	}
    else
	{
		if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
			InitSpritePosToAnimTargetsCentre(sprite, FALSE);
		else
			InitSpritePosToAnimTarget(sprite, FALSE);
	}

    sprite->pos2.x += (Random() % 48) - 24;
    sprite->pos2.y += (Random() % 24) - 12;

    StoreSpriteCallbackInData6(sprite, DestroySpriteAndMatrix);
    sprite->callback = RunStoredCallbackWhenAffineAnimEnds;
}

void SpriteCB_CentredElectricity(struct Sprite* sprite)
{
	if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
		InitSpritePosToAnimTargetsCentre(sprite, FALSE);
	else
		InitSpritePosToAnimTarget(sprite, FALSE);
	
    sprite->oam.tileNum += gBattleAnimArgs[3] * 4;

    if (gBattleAnimArgs[3] == 1)
        sprite->oam.matrixNum = 8;
    else if (gBattleAnimArgs[3] == 2)
        sprite->oam.matrixNum = 16;

    sprite->data[0] = gBattleAnimArgs[2];
    sprite->callback = WaitAnimForDuration;
    StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
}

void SpriteCB_CentredSpiderWeb(struct Sprite* sprite)
{
	if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
		InitSpritePosToAnimTargetsCentre(sprite, FALSE);
	else
		InitSpritePosToAnimTarget(sprite, FALSE);
		
	sprite->callback = SpriteCB_SpiderWeb;
}

static void InitSpritePosToGivenTarget(struct Sprite* sprite, u8 target)
{
	sprite->pos1.x = GetBattlerSpriteCoord2(target, BATTLER_COORD_X);
	sprite->pos1.y = GetBattlerSpriteCoord2(target, BATTLER_COORD_Y);
		
	SetAnimSpriteInitialXOffset(sprite, gBattleAnimArgs[0]);
	sprite->pos1.y += gBattleAnimArgs[1];
}

void SpriteCB_SearingShotRock(struct Sprite* sprite)
{
	u8 target = LoadBattleAnimTarget(4);
	
	if (GetBankPartyData(target)->hp == 0)
		DestroyAnimSprite(sprite);
	else
	{	
		InitSpritePosToGivenTarget(sprite, target);
		
		StartSpriteAnim(sprite, gBattleAnimArgs[2]);
		sprite->data[0] = gBattleAnimArgs[3];

		sprite->callback = WaitAnimForDuration;
		StoreSpriteCallbackInData6(sprite, AnimSpinningKickOrPunchFinish);
	}
}

void SpriteCB_CoreEnforcerHits(struct Sprite* sprite)
{
	StartSpriteAffineAnim(sprite, gBattleAnimArgs[3]);
	
	if (gBattleAnimArgs[2] == 0)
	{
		if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
			InitSpritePosToAnimAttackersCentre(sprite, FALSE);
		else
			InitSpritePosToAnimAttacker(sprite, FALSE);
	}
    else
	{
		if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
			InitSpritePosToAnimTargetsCentre(sprite, FALSE);
		else
			InitSpritePosToAnimTarget(sprite, FALSE);
	}

	sprite->pos1.y += 20;
	sprite->callback = SpriteCB_80BA7BC;
}

void SpriteCB_CoreEnforcerBeam(struct Sprite* sprite)
{
	if (!(gBattleTypeFlags & BATTLE_TYPE_DOUBLE))
		SpriteCB_AnimSolarbeamBigOrb(sprite);
	else
	{
		InitSpritePosToAnimAttacker(sprite, TRUE);
		StartSpriteAnim(sprite, gBattleAnimArgs[3]);

		sprite->data[0] = gBattleAnimArgs[2];
		
		sprite->data[2] = (GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X_2)
						+  GetBattlerSpriteCoord(PARTNER(gBattleAnimTarget), BATTLER_COORD_X_2)) / 2;
		
		
		sprite->data[4] = (GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET)
						+  GetBattlerSpriteCoord(PARTNER(gBattleAnimTarget), BATTLER_COORD_Y_PIC_OFFSET)) / 2;

		sprite->callback = StartAnimLinearTranslation;
		StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
	}
}

void CoreEnforcerLoadBeamTarget(struct Sprite* sprite)
{
	sprite->data[0] = gBattleAnimArgs[2];
    sprite->data[1] = sprite->pos1.x;
    sprite->data[2] = (GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X_2)
					+  GetBattlerSpriteCoord(PARTNER(gBattleAnimTarget), BATTLER_COORD_X_2)) / 2;
    sprite->data[3] = sprite->pos1.y;
    sprite->data[4] = (GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET)
					+  GetBattlerSpriteCoord(PARTNER(gBattleAnimTarget), BATTLER_COORD_Y_PIC_OFFSET)) / 2;
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

#define RESTORE_HIDDEN_HEALTHBOXES									\
{																	\
	for (sprite = 0; sprite < MAX_SPRITES; ++sprite)				\
		{															\
			switch (gSprites[sprite].template->tileTag) {			\
				case TAG_HEALTHBOX_PLAYER1_TILE:					\
				case TAG_HEALTHBOX_PLAYER2_TILE:					\
				case TAG_HEALTHBOX_OPPONENT1_TILE:					\
				case TAG_HEALTHBOX_OPPONENT2_TILE:					\
				case TAG_HEALTHBAR_PLAYER1_TILE:					\
				case TAG_HEALTHBAR_OPPONENT1_TILE:					\
				case TAG_HEALTHBAR_PLAYER2_TILE:					\
				case TAG_HEALTHBAR_OPPONENT2_TILE:					\
					switch (priority) {								\
						case 0:										\
							if (!gSprites[sprite].invisible)		\
							{										\
								gSprites[sprite].data[7] = TRUE;	\
								gSprites[sprite].invisible = TRUE;	\
							}										\
							break;									\
						default:									\
							if (gSprites[sprite].data[7])			\
							{										\
								gSprites[sprite].invisible = FALSE;	\
								gSprites[sprite].data[7] = FALSE;	\
							}										\
					}												\
																	\
			}														\
		}															\
}
	
#define hMain_HealthBarSpriteId     data[5]
void UpdateOamPriorityInAllHealthboxes(u8 priority)
{
    u32 i, sprite;
	
	#ifndef HIDE_HEALTHBOXES_DURING_ANIMS
		goto DEFAULT_CASE;
	#endif
	
	switch (gBattleBufferA[gActiveBattler][0]) {
		case CONTROLLER_MOVEANIMATION: ;
			u16 move = (gBattleBufferA[gActiveBattler][1]) | (gBattleBufferA[gActiveBattler][2] << 8);
			if (move == MOVE_TRANSFORM)
				goto DEFAULT_CASE;
			#ifdef DONT_HIDE_HEALTHBOXES_ATTACKER_STATUS_MOVES
			if (gBattleMoves[move].target & MOVE_TARGET_USER)
				goto DEFAULT_CASE;
			#endif
			goto HIDE_BOXES;
			
		case CONTROLLER_BATTLEANIMATION:
			switch (gBattleBufferA[gActiveBattler][1]) {
				case B_ANIM_TURN_TRAP:
				case B_ANIM_LEECH_SEED_DRAIN:
				case B_ANIM_MON_HIT:
				case B_ANIM_SNATCH_MOVE:
				case B_ANIM_FUTURE_SIGHT_HIT:
				case B_ANIM_DOOM_DESIRE_HIT:
				case B_ANIM_WISH_HEAL:
				case B_ANIM_ASTONISH_DROPS:
				case B_ANIM_SCARY_FACE_ASTONISH:
				case B_ANIM_WISHIWASHI_FISH:
				case B_ANIM_ZYGARDE_CELL_SWIRL:
				case B_ANIM_ELECTRIC_SURGE:
				case B_ANIM_GRASSY_SURGE:
				case B_ANIM_MISTY_SURGE:
				case B_ANIM_PSYCHIC_SURGE:
				case B_ANIM_SEA_OF_FIRE:
				case B_ANIM_LUNAR_DANCE_HEAL:
				case B_ANIM_HEALING_WISH_HEAL:
				case B_ANIM_RED_PRIMAL_REVERSION:
				case B_ANIM_BLUE_PRIMAL_REVERSION:
				case B_ANIM_POWDER_EXPLOSION:
				case B_ANIM_BEAK_BLAST_WARM_UP:
				case B_ANIM_SHELL_TRAP_SET:
				case B_ANIM_BERRY_EAT:
				case B_ANIM_ZMOVE_ACTIVATE:
				case B_ANIM_MEGA_EVOLUTION:
				case B_ANIM_ULTRA_BURST:
					goto HIDE_BOXES;
			}
		__attribute__ ((fallthrough));
		default:
		DEFAULT_CASE:
			for (i = 0; i < gBattlersCount; i++)
			{
				u8 healthboxLeftSpriteId = gHealthboxIDs[i];
				u8 healthboxRightSpriteId = gSprites[gHealthboxIDs[i]].oam.affineParam;
				u8 healthbarSpriteId = gSprites[gHealthboxIDs[i]].hMain_HealthBarSpriteId;

				gSprites[healthboxLeftSpriteId].oam.priority = priority;
				gSprites[healthboxRightSpriteId].oam.priority = priority;
				gSprites[healthbarSpriteId].oam.priority = priority;
				
				if (priority) //Restore Hidden Healthboxes
				{
					RESTORE_HIDDEN_HEALTHBOXES;
				}
			}
			return;
	}

HIDE_BOXES:
	RESTORE_HIDDEN_HEALTHBOXES;
}
