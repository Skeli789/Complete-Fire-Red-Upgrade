#include "defines.h"
#include "defines_battle.h"
#include "../include/battle_gfx_sfx_util.h"
#include "../include/battle_anim.h"
#include "../include/event_data.h"
#include "../include/random.h"

#include "../include/new/battle_anims.h"
#include "../include/new/battle_terrain.h"
#include "../include/new/dns.h"
#include "../include/new/helper_functions.h"

extern const struct CompressedSpriteSheet gBattleAnimPicTable[];
extern const struct CompressedSpritePalette gBattleAnimPaletteTable[];
extern const u8* const AttackAnimationTable[];
extern const u8* const gBattleAnims_General[];

static const union AnimCmd sAnimCmdPowerWhipOnPlayer[] =
{
	ANIMCMD_FRAME(0, 3, .hFlip = TRUE),
	ANIMCMD_FRAME(16, 3, .hFlip = TRUE),
	ANIMCMD_FRAME(32, 3, .hFlip = TRUE),
	ANIMCMD_FRAME(48, 3, .hFlip = TRUE),
	ANIMCMD_FRAME(64, 3, .hFlip = TRUE),

	ANIMCMD_END,
};

static const union AnimCmd sAnimCmdPowerWhipOnOpponent[] =
{
	ANIMCMD_FRAME(0, 3),
	ANIMCMD_FRAME(16, 3),
	ANIMCMD_FRAME(32, 3),
	ANIMCMD_FRAME(48, 3),
	ANIMCMD_FRAME(64, 3),
	ANIMCMD_END,
};

const union AnimCmd *const gAnimCmdPowerWhip[] =
{
	sAnimCmdPowerWhipOnPlayer,
	sAnimCmdPowerWhipOnOpponent,
};

const struct OamData sSunsteelStrikeBlastOAM =
{
    .affineMode = ST_OAM_AFFINE_DOUBLE,
    .objMode = ST_OAM_OBJ_NORMAL,
    .shape = SPRITE_SHAPE(64x64),
    .size = SPRITE_SIZE(64x64),
    .priority = 1, //Above sprites
};

static const union AffineAnimCmd gSpriteAffineAnim_SunsteelStrikeBlastEnemySide[] =
{
	AFFINEANIMCMD_FRAME(0, 0, -64, 1), //90 degree turn
	AFFINEANIMCMD_FRAME(0, 0, 0, 7), //Pause
	AFFINEANIMCMD_FRAME(16, 16, 0, 15), //Double in size
	AFFINEANIMCMD_END
};

static const union AffineAnimCmd gSpriteAffineAnim_SunsteelStrikeBlastPlayerSide[] =
{
	AFFINEANIMCMD_FRAME(0, 0, 128, 1), //180 degree turn
	AFFINEANIMCMD_FRAME(0, 0, 0, 7), //Pause
	AFFINEANIMCMD_FRAME(16, 16, 0, 15), //Double in size
	AFFINEANIMCMD_END
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_SunsteelStrikeBlast[] =
{
	gSpriteAffineAnim_SunsteelStrikeBlastEnemySide,
	gSpriteAffineAnim_SunsteelStrikeBlastPlayerSide,
};

//This file's functions:
static void InitSpritePosToAnimTargetsCentre(struct Sprite *sprite, bool8 respectMonPicOffsets);
static void InitSpritePosToAnimAttackersCentre(struct Sprite *sprite, bool8 respectMonPicOffsets);
static void InitSpritePosToGivenTarget(struct Sprite* sprite, u8 target);
static void SpriteCB_FlareBlitzUpFlamesP2(struct Sprite* sprite);
static void Task_HandleSpecialBattleAnimation(u8 taskId);
static bool8 ShouldAnimBeDoneRegardlessOfSubsitute(u8 animId);
static bool8 ShouldSubstituteRecedeForSpecialBattleAnim(u8 animId);
static void TrySwapBackupSpeciesWithSpecies(u8 activeBattler, u8 animId);

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
	gAnimScriptCallback = WaitAnimFrameCount;
}

void ShinyAnimFix(void)
{
	LoadCompressedSpriteSheetUsingHeap(&gBattleAnimPicTable[ANIM_TAG_GOLD_STARS - ANIM_SPRITES_START]);
	LoadCompressedSpritePaletteUsingHeap(&gBattleAnimPaletteTable[ANIM_TAG_GOLD_STARS - ANIM_SPRITES_START]);
}

void AnimTask_TechnoBlast(u8 taskId)
{
	gBattleAnimArgs[0] = gItems[GetBankPartyData(gBattleAnimAttacker)->item].holdEffectParam;
	DestroyAnimVisualTask(taskId);
}

void AnimTask_GetTimeOfDay(u8 taskId)
{
	gBattleAnimArgs[0] = 0; //Daytime
	
	#ifdef TIME_ENABLED
		if (IsNightTime())
			gBattleAnimArgs[0] = 1;
		else if (IsEvening())
			gBattleAnimArgs[0] = 2;
	#endif

	DestroyAnimVisualTask(taskId);
}

void AnimTask_GetLycanrocForm(u8 taskId)
{
	if (GetBankPartyData(gBattleAnimAttacker)->species == SPECIES_LYCANROC_N)
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
	switch (sAnimMoveIndex) {
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
	return gBattleBufferA[gBattleAnimAttacker][0] == CONTROLLER_MOVEANIMATION && sAnimMoveIndex == MOVE_IONDELUGE;
}

bool8 IsAnimMoveTectnoicRage(void)
{
	return sAnimMoveIndex == MOVE_TECTONIC_RAGE_P || sAnimMoveIndex == MOVE_TECTONIC_RAGE_S;
}

bool8 IsAnimMoveBloomDoom(void)
{
	return sAnimMoveIndex == MOVE_BLOOM_DOOM_P || sAnimMoveIndex == MOVE_BLOOM_DOOM_S;
}

bool8 IsAnimMoveOceanicOperretta(void)
{
	return sAnimMoveIndex == MOVE_OCEANIC_OPERETTA;
}

bool8 DoesMoveHaveGeyserOnTarget(void)
{
	return sAnimMoveIndex == MOVE_NEVER_ENDING_NIGHTMARE_P || sAnimMoveIndex == MOVE_NEVER_ENDING_NIGHTMARE_S 
		 || sAnimMoveIndex == MOVE_DEVASTATING_DRAKE_P 		|| sAnimMoveIndex == MOVE_DEVASTATING_DRAKE_S
		 || sAnimMoveIndex == MOVE_GIGAVOLT_HAVOC_P 		|| sAnimMoveIndex == MOVE_GIGAVOLT_HAVOC_S
		 || sAnimMoveIndex == MOVE_LIGHT_THAT_BURNS_THE_SKY;
}

bool8 IsAnimMoveDestinyBond(void)
{
	return sAnimMoveIndex == MOVE_DESTINYBOND;
}

bool8 IsAnimMoveThunderWave(void)
{
	return sAnimMoveIndex == MOVE_THUNDERWAVE;
}

bool8 IsAnimMoveGrudge(void)
{
	return sAnimMoveIndex == MOVE_GRUDGE;
}

bool8 IsAnimMoveFairyLock(void)
{
	return sAnimMoveIndex == MOVE_FAIRYLOCK;
}

bool8 IsAnimMoveFlashCannon(void)
{
	return sAnimMoveIndex == MOVE_FLASHCANNON;
}

bool8 IsAnimMoveSkillSwap(void)
{
	return sAnimMoveIndex == MOVE_SKILLSWAP;
}

bool8 IsAnimMovePowerSwap(void)
{
	return sAnimMoveIndex == MOVE_POWERSWAP;
}

bool8 IsAnimMoveHeartSwap(void)
{
	return sAnimMoveIndex == MOVE_HEARTSWAP;
}

bool8 IsAnimMoveMudBomb(void)
{
	return sAnimMoveIndex == MOVE_MUDBOMB;
}

bool8 IsAnimMoveCoreEnforcer(void)
{
	return sAnimMoveIndex == MOVE_COREENFORCER;
}

bool8 IsAnimMoveBulletSeed(void)
{
	return sAnimMoveIndex == MOVE_BULLETSEED;
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
			move = gTerrainTable[0].secretPowerAnim;
			break;
		case GRASSY_TERRAIN:
			move = gTerrainTable[1].secretPowerAnim;
			break;
		case MISTY_TERRAIN:
			move = gTerrainTable[2].secretPowerAnim;
			break;
		case PSYCHIC_TERRAIN:
			move = gTerrainTable[3].secretPowerAnim;
			break;
		default:
			if (IsTerrainMoveIndoors())
				move = gTerrainTable[BATTLE_TERRAIN_INSIDE + 4].secretPowerAnim;
			else
				move = gTerrainTable[gBattleTerrain + 4].secretPowerAnim;
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
		gBattleAnimArgs[4] = gCamouflageColours[gTerrainTable[entry].camouflageType];
	else if (IsTerrainMoveIndoors())
		gBattleAnimArgs[4] = gCamouflageColours[gTerrainTable[BATTLE_TERRAIN_INSIDE + 4].camouflageType];
	else
		gBattleAnimArgs[4] = gCamouflageColours[gTerrainTable[gBattleTerrain + 4].camouflageType];

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

static void InitSpritePosToAnimTargetsCentre(struct Sprite *sprite, bool8 respectMonPicOffsets)
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

static void InitSpritePosToAnimAttackersCentre(struct Sprite *sprite, bool8 respectMonPicOffsets)
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


void SpriteCB_FlareBlitzUpFlames(struct Sprite* sprite)
{
	if (gBattleAnimArgs[0] == 0)
	{
		sprite->pos1.x = GetBattlerSpriteCoord(gBattleAnimAttacker, 0) + gBattleAnimArgs[1];
		sprite->pos1.y = GetBattlerSpriteCoord(gBattleAnimAttacker, 1) + gBattleAnimArgs[2];
	}
	else
	{
		sprite->pos1.x = GetBattlerSpriteCoord(gBattleAnimTarget, 0) + gBattleAnimArgs[1];
		sprite->pos1.y = GetBattlerSpriteCoord(gBattleAnimTarget, 1) + gBattleAnimArgs[2];
	}

	sprite->data[0] = 0;
	sprite->data[1] = gBattleAnimArgs[3];
	sprite->callback = SpriteCB_FlareBlitzUpFlamesP2;
}

static void SpriteCB_FlareBlitzUpFlamesP2(struct Sprite* sprite)
{
	if (++sprite->data[0] > sprite->data[1])
	{
		sprite->data[0] = 0;
		sprite->pos1.y -= 2;
	}

	sprite->pos1.y -= sprite->data[0];
	if (sprite->pos1.y < 0)
		DestroyAnimSprite(sprite);
}

#define ITEM_TAG ANIM_TAG_ITEM_BAG
u8 __attribute__((long_call)) AddItemIconSprite(u16 tilesTag, u16 paletteTag, u16 itemId);
void AnimTask_CreateFlingItem(u8 taskId)
{
	u8 iconSpriteId = AddItemIconSprite(ITEM_TAG, ITEM_TAG, gLastUsedItem);

	if (iconSpriteId != MAX_SPRITES)
	{
		gSprites[iconSpriteId].oam.priority = 0; //Highest priority
		gSprites[iconSpriteId].callback = (void*) 0x80B4495;
		++gAnimVisualTaskCount;
	}

	DestroyAnimVisualTask(taskId);
}

void SpriteCB_SunsteelStrikeRings(struct Sprite* sprite)
{
    if (GetBattlerSide(gBattleAnimAttacker) != B_SIDE_PLAYER)
    {
        sprite->pos1.x = 272;
        sprite->pos1.y = -32;
    }
    else
    {
        sprite->pos1.x = -32;
        sprite->pos1.y = -32;
    }

    sprite->data[0] = gBattleAnimArgs[0];
    sprite->data[1] = sprite->pos1.x;
    sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X_2);
    sprite->data[3] = sprite->pos1.y;
    sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET);

    InitAnimLinearTranslation(sprite);
    sprite->callback = (void*) 0x80B1CC1;
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
	
#define hMain_HealthBarSpriteId	 data[5]
void UpdateOamPriorityInAllHealthboxes(u8 priority)
{
	u32 i, sprite;
	
	#ifndef HIDE_HEALTHBOXES_DURING_ANIMS
		goto DEFAULT_CASE;
	#endif
	
	switch (gBattleBufferA[gActiveBattler][0]) {
		case CONTROLLER_MOVEANIMATION: ;
			if (sAnimMoveIndex == MOVE_TRANSFORM)
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

#define tBattlerId 	data[0]
#define tAnimId 	data[1]
#define tArgumentId	data[2]
#define tState 		data[3]

bool8 TryHandleLaunchBattleTableAnimation(u8 activeBattler, u8 bankAtk, u8 bankDef, u8 tableId, u16 argument)
{
	u8 taskId;

	if (tableId == B_ANIM_CASTFORM_CHANGE && (argument & 0x80))
	{
		gBattleMonForms[activeBattler] = (argument & ~(0x80));
		return TRUE;
	}
	if (gBattleSpritesDataPtr->bankData[activeBattler].behindSubstitute
	&& !ShouldAnimBeDoneRegardlessOfSubsitute(tableId)
	&& !ShouldSubstituteRecedeForSpecialBattleAnim(tableId))
	{
		return TRUE;
	}
	if (gBattleSpritesDataPtr->bankData[activeBattler].behindSubstitute
	&& tableId == B_ANIM_SUBSTITUTE_FADE
	&& gSprites[gBattlerSpriteIds[activeBattler]].invisible)
	{
		LoadBattleMonGfxAndAnimate(activeBattler, TRUE, gBattlerSpriteIds[activeBattler]);
		ClearBehindSubstituteBit(activeBattler);
		return TRUE;
	}

	gBattleAnimAttacker = bankAtk;
	gBattleAnimTarget = bankDef;
	taskId = CreateTask(Task_HandleSpecialBattleAnimation, 10);
	gTasks[taskId].tBattlerId = activeBattler;
	gTasks[taskId].tAnimId = tableId;
	gTasks[taskId].tArgumentId = argument;
	gBattleSpritesDataPtr->healthBoxesData[gTasks[taskId].tBattlerId].animFromTableActive = TRUE;

	return FALSE;
}

static void Task_HandleSpecialBattleAnimation(u8 taskId)
{
	u8 activeBattler = gTasks[taskId].tBattlerId;
	u8 animId = gTasks[taskId]. tAnimId;

	if (gAnimScriptCallback != NULL)
		gAnimScriptCallback();

	switch (gTasks[taskId].tState) {
		case 0:
			if (gBattleSpritesDataPtr->bankData[activeBattler].behindSubstitute
			&& !gBattleSpritesDataPtr->bankData[activeBattler].substituteOffScreen
			&& ShouldSubstituteRecedeForSpecialBattleAnim(animId))
			{
				//Temporarily revert the Pokemon to its original form so the anim works properly.
				//This change is done individually on both roms so there should be no issue in link
				//battles.
				TrySwapBackupSpeciesWithSpecies(activeBattler, animId);
				gBattleSpritesDataPtr->bankData[activeBattler].substituteOffScreen = TRUE;
				InitAndLaunchSpecialAnimation(activeBattler, activeBattler, activeBattler, B_ANIM_SUBSTITUTE_TO_MON);
			}
			++gTasks[taskId].tState;
			break;
			
		case 1:
			if (!gBattleSpritesDataPtr->healthBoxesData[activeBattler].specialAnimActive)
			{
				//Now restore the original species
				if (gBattleSpritesDataPtr->bankData[activeBattler].behindSubstitute
				&&  gBattleSpritesDataPtr->bankData[activeBattler].substituteOffScreen
				&& ShouldSubstituteRecedeForSpecialBattleAnim(animId))
					TrySwapBackupSpeciesWithSpecies(activeBattler, animId);

				gBattleSpritesDataPtr->animationData->animArg = gTasks[taskId].tArgumentId;
				LaunchBattleAnimation(gBattleAnims_General, animId, FALSE);
				++gTasks[taskId].tState;
			}
			break;
			
		case 2:
			if (!gAnimScriptActive)
			{
				if (gBattleSpritesDataPtr->bankData[activeBattler].behindSubstitute
				&&  gBattleSpritesDataPtr->bankData[activeBattler].substituteOffScreen == TRUE)
				{
					InitAndLaunchSpecialAnimation(activeBattler, activeBattler, activeBattler, B_ANIM_MON_TO_SUBSTITUTE);
					gBattleSpritesDataPtr->bankData[activeBattler].substituteOffScreen = FALSE;
				}
				++gTasks[taskId].tState;
			}
			break;
			
		case 3:
			if (!gAnimScriptActive)
			{
				gBattleSpritesDataPtr->healthBoxesData[gTasks[taskId].tBattlerId].animFromTableActive = FALSE;
				DestroyTask(taskId);
			}
	}
}

static bool8 ShouldAnimBeDoneRegardlessOfSubsitute(u8 animId)
{
	switch (animId) {
		case B_ANIM_SUBSTITUTE_FADE:
		case B_ANIM_SNATCH_MOVE:
		case B_ANIM_LOAD_DEAFUALT_BG:
		case B_ANIM_LOAD_ABILITY_POP_UP:
		case B_ANIM_DESTROY_ABILITY_POP_UP:
		case B_ANIM_RAIN_CONTINUES:
		case B_ANIM_SUN_CONTINUES:
		case B_ANIM_SANDSTORM_CONTINUES:
		case B_ANIM_HAIL_CONTINUES:
		case B_ANIM_STRONG_WINDS_CONTINUE:
		case B_ANIM_FOG_CONTINUES:
			return TRUE;
		default:
			return FALSE;
	}
}

static bool8 ShouldSubstituteRecedeForSpecialBattleAnim(u8 animId)
{
	switch (animId) {
		case B_ANIM_TRANSFORM:
		case B_ANIM_WISHIWASHI_FISH:
		case B_ANIM_ZYGARDE_CELL_SWIRL:
		case B_ANIM_BLUE_PRIMAL_REVERSION:
		case B_ANIM_RED_PRIMAL_REVERSION:
		case B_ANIM_ZMOVE_ACTIVATE:
		case B_ANIM_MEGA_EVOLUTION:
		case B_ANIM_ULTRA_BURST:
			return TRUE;
		default:
			return FALSE;
	}
}

static void TrySwapBackupSpeciesWithSpecies(u8 activeBattler, u8 animId)
{
	struct Pokemon* animGuy = GetBankPartyData(activeBattler);

	switch (animId) {
		case B_ANIM_TRANSFORM:
		case B_ANIM_WISHIWASHI_FISH:
		case B_ANIM_ZYGARDE_CELL_SWIRL:
		case B_ANIM_BLUE_PRIMAL_REVERSION:
		case B_ANIM_RED_PRIMAL_REVERSION:
		case B_ANIM_MEGA_EVOLUTION:
		case B_ANIM_ULTRA_BURST:
			if (animGuy->backupSpecies != SPECIES_NONE)
			{
				u16 backup = animGuy->species;
				animGuy->species = animGuy->backupSpecies;
				animGuy->backupSpecies = backup;
			}
	}
}
