#include "defines.h"
#include "defines_battle.h"
#include "../include/battle_anim.h"
#include "../include/battle_interface.h"
#include "../include/battle_gfx_sfx_util.h"
#include "../include/decompress.h"
#include "../include/event_data.h"
#include "../include/gpu_regs.h"
#include "../include/random.h"
#include "../include/sound.h"

#include "../include/constants/region_map_sections.h"
#include "../include/constants/pokedex.h"
#include "../include/constants/songs.h"

#include "../include/new/battle_anims.h"
#include "../include/new/battle_indicators.h"
#include "../include/new/battle_terrain.h"
#include "../include/new/battle_util.h"
#include "../include/new/dns.h"
#include "../include/new/dynamax.h"
#include "../include/new/end_turn.h"
#include "../include/new/mega.h"

/*
battle_anims.c
	Functions and structures to modify attack animations.
*/

//TODO: Shell Side Arm in Link battles

#define ITEM_TAG ANIM_TAG_ITEM_BAG

#define gMonFrontPicTable ((const struct CompressedSpriteSheet*) *((u32*) 0x8000128))
#define gMonBackPicTable ((const struct CompressedSpriteSheet*) *((u32*) 0x800012C))
#define gMonFrontPicCoords ((const struct MonCoords*) *((u32*) 0x8011F4C))
#define gMonBackPicCoords ((const struct MonCoords*) *((u32*) 0x8074634))
#define gEnemyMonElevation ((const u8*) *((u32*) 0x80356F8))

extern const u8* sBattleAnimScriptPtr;
extern u16 sAnimMoveIndex;
extern s8 gAnimFramesToWait;
extern u16 sAnimMoveIndex;

extern const struct CompressedSpriteSheet gBattleAnimPicTable[];
extern const struct CompressedSpritePalette gBattleAnimPaletteTable[];
extern const u8* const gMoveAnimations[];
extern const u8* const gBattleAnims_General[];

//External functions:
u8 __attribute__((long_call)) AddItemIconSprite(u16 tilesTag, u16 paletteTag, u16 itemId);

//This file's functions:

static void AnimTask_DynamaxGrowthStep(u8 taskId);

///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////Utils/////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

static bank_t LoadBattleAnimTarget(u8 arg)
{
	u8 battler;

	if (IS_DOUBLE_BATTLE)
	{
		switch (gBattleAnimArgs[arg]) {
			case ANIM_ATTACKER:
				battler = gBattleAnimAttacker;
				break;
			default:
				battler = gBattleAnimTarget;
				break;
			case ANIM_ATK_PARTNER:
				battler = PARTNER(gBattleAnimAttacker);
				break;
			case ANIM_DEF_PARTNER:
				battler = PARTNER(gBattleAnimTarget);
				break;
		}
	}
	else
	{
		if (gBattleAnimArgs[arg] == ANIM_ATTACKER || gBattleAnimArgs[arg] == ANIM_ATK_PARTNER)
			battler = gBattleAnimAttacker;
		else
			battler = gBattleAnimTarget;
	}

	return battler;
}

static void InitSpritePosToGivenTarget(struct Sprite* sprite, u8 target)
{
	sprite->pos1.x = GetBattlerSpriteCoord2(target, BATTLER_COORD_X);
	sprite->pos1.y = GetBattlerSpriteCoord2(target, BATTLER_COORD_Y);

	SetAnimSpriteInitialXOffset(sprite, gBattleAnimArgs[0]);
	sprite->pos2.y = gBattleAnimArgs[1];
}

static void InitSpritePosToGivenTargetRespectPicOffsets(struct Sprite* sprite, u8 target)
{
	sprite->pos1.x = GetBattlerSpriteCoord2(target, BATTLER_COORD_X_2);
	sprite->pos1.y = GetBattlerSpriteCoord2(target, BATTLER_COORD_Y_PIC_OFFSET);

	SetAnimSpriteInitialXOffset(sprite, gBattleAnimArgs[0]);
	sprite->pos2.y = gBattleAnimArgs[1];
}

void SetAverageBattlerPositions(u8 battlerId, bool8 respectMonPicOffsets, s16 *x, s16 *y)
{
	u8 xCoordType, yCoordType;
	s16 battlerX, battlerY;
	s16 partnerX, partnerY;

	if (!respectMonPicOffsets)
	{
		xCoordType = BATTLER_COORD_X;
		yCoordType = BATTLER_COORD_Y;
	}
	else
	{
		xCoordType = BATTLER_COORD_X_2;
		yCoordType = BATTLER_COORD_Y_PIC_OFFSET;
	}

	battlerX = GetBattlerSpriteCoord(battlerId, xCoordType);
	battlerY = GetBattlerSpriteCoord(battlerId, yCoordType);
	if (IsDoubleBattle() && (!IsRaidBattle() || SIDE(battlerId) == B_SIDE_PLAYER))
	{
		partnerX = GetBattlerSpriteCoord(BATTLE_PARTNER(battlerId), xCoordType);
		partnerY = GetBattlerSpriteCoord(BATTLE_PARTNER(battlerId), yCoordType);
	}
	else
	{
		partnerX = battlerX;
		partnerY = battlerY;
	}

	*x = (battlerX + partnerX) / 2;
	*y = (battlerY + partnerY) / 2;
}

static u8 GetProperCentredCoord(u8 bank, u8 coordType)
{
	if (IS_DOUBLE_BATTLE)
	{
		return (GetBattlerSpriteCoord2(bank, coordType)
			  +  GetBattlerSpriteCoord2(PARTNER(bank), coordType)) / 2;
	}

	return GetBattlerSpriteCoord(bank, coordType);
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

static u16 GetAnimAttackerSpecies(void)
{
	u16 species = gBattleSpritesDataPtr->bankData[gBattleAnimAttacker].transformSpecies;
	if (species == SPECIES_NONE)
		species = GetIllusionPartyData(gBattleAnimAttacker)->species;

	return species;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////Gen 1 - 3///////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

//Bullet Seed//
bool8 IsAnimMoveBulletSeed(void) //Used for Water Gun
{
	return sAnimMoveIndex == MOVE_BULLETSEED;
}


//Camouflage//
void AnimTask_SetCamouflageBlend(u8 taskId)
{
	u8 entry = 0;
	u32 selectedPalettes = UnpackSelectedBattleAnimPalettes(gBattleAnimArgs[0]);

	switch (gTerrainType) {
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
		gBattleAnimArgs[4] = gCamouflageColours[gTerrainTable[GetBattleTerrainOverride() + 4].camouflageType];

	StartBlendAnimSpriteColor(taskId, selectedPalettes);
}


//Destiny Bond//
bool8 IsAnimMoveDestinyBond(void) //Used for anims like Shadow Sneak and Spectral Thief
{
	return sAnimMoveIndex == MOVE_DESTINYBOND;
}


//Double-Slap//
static const union AffineAnimCmd sSpriteAffineAnim_DoubleSlap[] =
{
	AFFINEANIMCMD_FRAME(0, 0, 64, 1), //Rotate left 90 degrees
	AFFINEANIMCMD_FRAME(0, 0, 0, 3), //Do Nothing
	AFFINEANIMCMD_FRAME(0, -28, 0, 8), //Flatten horizontally (on its side)
	AFFINEANIMCMD_FRAME(0, 0, 0, 11), //Do Nothing
	AFFINEANIMCMD_FRAME(0, -288, 0, 1), //Unflatten in other direction
	AFFINEANIMCMD_FRAME(0, 0, 0, 2), //Do Nothing
	AFFINEANIMCMD_FRAME(0, 28, 0, 8), //Flatten horizontally (on its side)
	AFFINEANIMCMD_END
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_DoubleSlap[] =
{
	sSpriteAffineAnim_DoubleSlap,
};


//Grudge//
bool8 IsAnimMoveGrudge(void) //Used for Hex
{
	return sAnimMoveIndex == MOVE_GRUDGE;
}


//Hydro Cannon//
static const union AffineAnimCmd sSpriteAffineAnim_HydroCannonBall[] =
{
	AFFINEANIMCMD_FRAME(16, 16, 0, 16), //Double in size
	AFFINEANIMCMD_END
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_HydroCannonBall[] =
{
	sSpriteAffineAnim_HydroCannonBall,
};

void SpriteCB_HydroCannonImpact(struct Sprite *sprite)
{
	if (gBattleAnimArgs[2] == 0)
		InitSpritePosToAnimAttacker(sprite, 1);
	else
		InitSpritePosToAnimTarget(sprite, TRUE);

	sprite->callback = DestroyAnimSprite;
}


//Knock Off//
void AnimTask_CreateKnockOffItem(u8 taskId)
{
	u8 iconSpriteId = AddItemIconSprite(ITEM_TAG, ITEM_TAG, gLastUsedItem);

	if (iconSpriteId != MAX_SPRITES)
	{
		struct Sprite* sprite = &gSprites[iconSpriteId];

		sprite->pos1.x = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X_2);
		sprite->pos1.y = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET);
		sprite->oam.priority = 2;
		sprite->oam.affineMode = ST_OAM_AFFINE_NORMAL;
		sprite->affineAnims = (void*) 0x83E2E80; //sFallingBagAffineAnimTable
		sprite->callback = (void*) (0x80A35F4 | 1); //AnimKnockOffItem

		CalcCenterToCornerVec(sprite, sprite->oam.shape, sprite->oam.size, sprite->oam.affineMode);
		InitSpriteAffineAnim(sprite);
		++gAnimVisualTaskCount;
	}

	DestroyAnimVisualTask(taskId);
}


//Leech Life//

//Creates the mini Leech Life Needle
void SpriteCB_LeechLifeNeedle(struct Sprite *sprite)
{
	if (SIDE(gBattleAnimTarget) == B_SIDE_PLAYER)
	{
		gBattleAnimArgs[1] = -gBattleAnimArgs[1];
		gBattleAnimArgs[0] = -gBattleAnimArgs[0];
		StartSpriteAffineAnim(sprite, 1);
	}

	sprite->pos1.x = GetBattlerSpriteCoord2(gBattleAnimTarget, 2) + gBattleAnimArgs[0];
	sprite->pos1.y = GetBattlerSpriteCoord2(gBattleAnimTarget, 3) + gBattleAnimArgs[1];
	sprite->data[0] = gBattleAnimArgs[2];

	sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimTarget, 2);
	sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimTarget, 3);

	sprite->callback = StartAnimLinearTranslation;
	StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
}


//Rock Throw//
static const union AnimCmd sAnimCmdSmallRock[] =
{
	ANIMCMD_FRAME(4, 1),
	ANIMCMD_END,
};

const union AnimCmd *const gAnimCmdTable_SmallRock[] =
{
	sAnimCmdSmallRock,
};


//Secret Power//
void AnimTask_GetSecretPowerAnimation(u8 taskId)
{
	u16 move;

	switch (gTerrainType) {
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
				move = gTerrainTable[GetBattleTerrainOverride() + 4].secretPowerAnim;
	}

	sBattleAnimScriptPtr = gMoveAnimations[move];
	DestroyAnimVisualTask(taskId);
}


//Skill Swap//
bool8 IsAnimMoveSkillSwap(void) //Used for other swap moves like Power Swap or Heart Swap
{
	return sAnimMoveIndex == MOVE_SKILLSWAP;
}


//Steel Wing//

//Converts the given palette tag to grayscale
void AnimTask_GrayscaleParticle(u8 taskId)
{
	u16 palTag = gBattleAnimArgs[0];
	TintPalette_GrayScale(&gPlttBufferUnfaded2[IndexOfSpritePaletteTag(palTag) * 16], 16);
	TintPalette_GrayScale(&gPlttBufferFaded2[IndexOfSpritePaletteTag(palTag) * 16], 16);
	TintPalette_GrayScale(&((u16*) PLTT)[IndexOfSpritePaletteTag(palTag) * 16], 16);
	DestroyAnimVisualTask(taskId);
}


//Thief//
void AnimTask_CreateStealItem(u8 taskId)
{
	u8 iconSpriteId = AddItemIconSprite(ITEM_TAG, ITEM_TAG, gLastUsedItem);

	if (iconSpriteId != MAX_SPRITES)
	{
		struct Sprite* sprite = &gSprites[iconSpriteId];

		sprite->oam.priority = 2;
		sprite->oam.affineMode = ST_OAM_AFFINE_NORMAL;
		sprite->affineAnims = (void*) 0x83E2E80; //sFallingBagAffineAnimTable
		sprite->callback = (void*) (0x80A36B4 | 1); //AnimItemSteal

		CalcCenterToCornerVec(sprite, sprite->oam.shape, sprite->oam.size, sprite->oam.affineMode);
		InitSpriteAffineAnim(sprite);
		++gAnimVisualTaskCount;
	}

	DestroyAnimVisualTask(taskId);
}

void UpdatedAnimStealItemFinalCallback(struct Sprite* sprite)
{
	sprite->data[0]++;
	if (sprite->data[0] > 50)
		DestroyAnimSprite(sprite);
}


//Wing Attack//
static const union AffineAnimCmd sSpriteAffineAnim_WingAttackFeather[] =
{
	AFFINEANIMCMD_FRAME(0, 0, -1, 14), //Rotate a little right
	AFFINEANIMCMD_FRAME(0, 0, 1, 28), //Rotate a little left
	AFFINEANIMCMD_END,
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_WingAttackFeather[] =
{
	sSpriteAffineAnim_WingAttackFeather,
};


//Traping Moves
void AnimTask_GetTrappedMoveAnimId(u8 taskId)
{
	switch (gBattleSpritesDataPtr->animationData->animArg)
	{
		case MOVE_FIRESPIN:
		case MOVE_G_MAX_CENTIFERNO_P:
		case MOVE_G_MAX_CENTIFERNO_S:
			gBattleAnimArgs[0] = 1;
			break;
		case MOVE_WHIRLPOOL:
			gBattleAnimArgs[0] = 2;
			break;
		case MOVE_CLAMP:
			gBattleAnimArgs[0] = 3;
			break;
		case MOVE_SANDTOMB:
		case MOVE_G_MAX_SANDBLAST_P:
		case MOVE_G_MAX_SANDBLAST_S:
			gBattleAnimArgs[0] = 4;
			break;
		case MOVE_MAGMASTORM:
			gBattleAnimArgs[0] = 5;
			break;
		case MOVE_INFESTATION:
			gBattleAnimArgs[0] = 6;
			break;
		case MOVE_SNAPTRAP:
			gBattleAnimArgs[0] = 7;
			break;
		case MOVE_OCTOLOCK:
			gBattleAnimArgs[0] = 8;
			break;
		case MOVE_THUNDERCAGE:
			gBattleAnimArgs[0] = 9;
			break;
		default:
			gBattleAnimArgs[0] = 0;
	}

	DestroyAnimVisualTask(taskId);
}


//Whirlwind

//To move a mon off-screen when pushed out by Roar/Whirlwind
void AnimTask_SlideOffScreen(u8 taskId)
{
	u8 bank, spriteId;

	bank = LoadBattleAnimTarget(0);
	spriteId = GetAnimBattlerSpriteId(gBattleAnimArgs[0]);

	if (spriteId < MAX_SPRITES)
	{
		gTasks[taskId].data[0] = spriteId;
		if (SIDE(bank) != B_SIDE_PLAYER)
			gTasks[taskId].data[1] = gBattleAnimArgs[1];
		else
			gTasks[taskId].data[1] = -gBattleAnimArgs[1];

		gTasks[taskId].func = (void*) (0x80996B8 | 1); //AnimTask_SlideOffScreen_Step
	}
	else
		DestroyAnimVisualTask(taskId);
}


///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////Gen 4/////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

//Aura Sphere//
static const union AffineAnimCmd sSpriteAffineAnim_AuraSphereBall[] =
{
	AFFINEANIMCMD_FRAME(16, 16, 0, 0), //Start at 1 pixel
	AFFINEANIMCMD_FRAME(16, 16, 0, 15),

	//Pulsate
	AFFINEANIMCMD_FRAME(-8, -8, 10, 3), //Shrink & Spin
	AFFINEANIMCMD_FRAME(8, 8, 10, 3), //Grow & Spin
	AFFINEANIMCMD_JUMP(2),
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_AuraSphereBall[] =
{
	sSpriteAffineAnim_AuraSphereBall,
};


//Bullet Punch//
static const union AffineAnimCmd sSpriteAffineAnim_HalfSize[] =
{
	AFFINEANIMCMD_FRAME(-128, -128, 0, 1), //Half size
	AFFINEANIMCMD_END,
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_HalfSize[] =
{
	sSpriteAffineAnim_HalfSize,
};


//Crush Grip//
static const union AffineAnimCmd sSpriteAffineAnim_CrushGripHandEnemyAttack[] =
{
	AFFINEANIMCMD_FRAME(0, 0, 96, 1), //180 degree turn
	AFFINEANIMCMD_END
};

static const union AffineAnimCmd sSpriteAffineAnim_DoNothing[] =
{
	AFFINEANIMCMD_FRAME(0, 0, 0, 1), //Do nothing
	AFFINEANIMCMD_END
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_CrushGripHand[] =
{
	sSpriteAffineAnim_DoNothing,
	sSpriteAffineAnim_CrushGripHandEnemyAttack,
};

static const union AffineAnimCmd sCompressTargetHorizontallyAffineAnimCmds[] =
{
	AFFINEANIMCMD_FRAME(64, 0, 0, 16), //Compress
	AFFINEANIMCMD_FRAME(0, 0, 0, 64),
	AFFINEANIMCMD_FRAME(-64, 0, 0, 16),
	AFFINEANIMCMD_END,
};

void AnimTask_CompressTargetHorizontally(u8 taskId)
{
	struct Task* task = &gTasks[taskId];
	u8 spriteId = GetAnimBattlerSpriteId(ANIM_TARGET);
	PrepareAffineAnimInTaskData(task, spriteId, sCompressTargetHorizontallyAffineAnimCmds);
	task->func = AnimTask_DynamaxGrowthStep;
}


//Discharge//
void SpriteCB_RandomCentredHits(struct Sprite* sprite)
{
	if (gBattleAnimArgs[1] == -1)
		gBattleAnimArgs[1] = Random() & 3;

	StartSpriteAffineAnim(sprite, gBattleAnimArgs[1]);

	if (gBattleAnimArgs[0] == 0)
	{
		if (IS_DOUBLE_BATTLE)
			InitSpritePosToAnimAttackersCentre(sprite, FALSE);
		else
			InitSpritePosToAnimAttacker(sprite, FALSE);
	}
	else
	{
		if (IS_DOUBLE_BATTLE)
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
	if (IS_DOUBLE_BATTLE)
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


//Draco Meteor//
const struct OamData sDracoMeteorRockOAM =
{
	.affineMode = ST_OAM_AFFINE_OFF,
	.objMode = ST_OAM_OBJ_NORMAL,
	.shape = SPRITE_SHAPE(32x32),
	.size = SPRITE_SIZE(32x32),
	.priority = 1,
};

const struct OamData sDracoMeteorTailOAM =
{
	.affineMode = ST_OAM_AFFINE_DOUBLE,
	.objMode = ST_OAM_OBJ_BLEND,
	.shape = SPRITE_SHAPE(16x16),
	.size = SPRITE_SIZE(16x16),
	.priority = 2,
};

//Moves a meteorite across the screen
//arg 0: initial x pixel offset
//arg 1: initial y pixel offset
//arg 2: destination x pixel offset
//arg 3: destination y pixel offset
//arg 4: duration
static void AnimDracoMeteorRockStep(struct Sprite *sprite);
void SpriteCB_DracoMeteorRock(struct Sprite *sprite)
{
	if (SIDE(gBattleAnimTarget) == B_SIDE_PLAYER)
	{
		sprite->data[0] = sprite->pos1.x - gBattleAnimArgs[0];
		sprite->data[2] = sprite->pos1.x - gBattleAnimArgs[2];
	}
	else
	{
		sprite->data[0] = sprite->pos1.x + gBattleAnimArgs[0];
		sprite->data[2] = sprite->pos1.x + gBattleAnimArgs[2];
	}

	sprite->data[1] = sprite->pos1.y + gBattleAnimArgs[1];
	sprite->data[3] = sprite->pos1.y + gBattleAnimArgs[3];
	sprite->data[4] = gBattleAnimArgs[4];

	sprite->data[6] = gBattleAnimArgs[2];
	sprite->data[7] = gBattleAnimArgs[3];

	sprite->pos1.x = sprite->data[0];
	sprite->pos1.y = sprite->data[1];
	sprite->callback = AnimDracoMeteorRockStep;
}

extern const struct SpriteTemplate gDracoMeteorTailSpriteTemplate;
static void AnimDracoMeteorRockStep(struct Sprite *sprite)
{
	sprite->pos2.x = ((sprite->data[2] - sprite->data[0]) * sprite->data[5]) / sprite->data[4];
	sprite->pos2.y = ((sprite->data[3] - sprite->data[1]) * sprite->data[5]) / sprite->data[4];

	if (sprite->data[5] == sprite->data[4])
		DestroyAnimSprite(sprite);

	sprite->data[5]++;
}


//Drain Punch//
static const union AffineAnimCmd sSpriteAffineAnim_DrainPunchFist[] =
{
	AFFINEANIMCMD_FRAME(256, 256, 0, 1), //Double sprite size
	AFFINEANIMCMD_FRAME(-32, -32, 0, 8),
	AFFINEANIMCMD_END,
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_DrainPunchFist[] =
{
	sSpriteAffineAnim_DrainPunchFist,
};


//Feint//
static const union AffineAnimCmd sSpriteAffineAnim_FeintFist[] =
{
	AFFINEANIMCMD_FRAME(256, 256, 0, 1), //Double sprite size
	AFFINEANIMCMD_FRAME(-16, -16, 0, 16),
	AFFINEANIMCMD_END,
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_FeintFist[] =
{
	sSpriteAffineAnim_FeintFist,
};


//Flash Cannon//
bool8 IsAnimMoveFlashCannonOrSteelBeam(void)
{
	return sAnimMoveIndex == MOVE_FLASHCANNON
		|| sAnimMoveIndex == MOVE_STEELBEAM;
}


//Fling//
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


//Focus Blast//
#define AnimSuperpowerOrb_Step ((void*) (0x80B111C | 1))
void SpriteCB_FocusBlastOrb(struct Sprite* sprite)
{
	sprite->pos1.x = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X_2);
	sprite->pos1.y = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_Y_PIC_OFFSET);
    sprite->data[0] = 0;
    sprite->data[1] = 12;
    sprite->data[2] = 8;
	sprite->data[7] = gBattleAnimTarget;
    sprite->callback = AnimSuperpowerOrb_Step;
}


//Force Palm//

//Creates an object on the target that moves linearly in some direction and wobbles at the end.
//arg 0: Initial x-offset
//arg 1: Initial y-offset
//arg 2: Final x-offset
//arg 3: Final y-offset
//arg 4: Motion speed delay
//arg 5: Pause before motion
void SpriteCB_ForcePalmStep1(struct Sprite* sprite);
void SpriteCB_ForcePalmStep2(struct Sprite* sprite);
void SpriteCB_ForcePalmStep3(struct Sprite* sprite);
void SpriteCB_ForcePalmStep4(struct Sprite* sprite);
void SpriteCB_ForcePalmStep5(struct Sprite* sprite);
void SpriteCB_ForcePalm(struct Sprite* sprite)
{
	InitSpritePosToAnimTarget(sprite, TRUE);

	sprite->data[0] = gBattleAnimArgs[4]; //Speed delay
	sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X_2) + gBattleAnimArgs[2];
	sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET) + gBattleAnimArgs[3];

	sprite->data[5] = gBattleAnimArgs[5]; //Pause before motion
	sprite->data[6] = 0;
	sprite->callback = SpriteCB_ForcePalmStep1;
}

void SpriteCB_ForcePalmStep1(struct Sprite* sprite)
{
	if (++sprite->data[6] >= sprite->data[5])
	{
		sprite->callback = StartAnimLinearTranslation;
		StoreSpriteCallbackInData6(sprite, SpriteCB_ForcePalmStep2);
	}
}

void SpriteCB_ForcePalmStep2(struct Sprite* sprite)
{
	sprite->data[0] = 0;
	sprite->callback = SpriteCB_ForcePalmStep3;
}

void SpriteCB_ForcePalmStep3(struct Sprite* sprite)
{
	if (++sprite->data[0] >= 8)
	{
		sprite->data[0] = 0;
		sprite->callback = SpriteCB_ForcePalmStep4;
	}
	else
		sprite->pos1.x -= 1;
}

void SpriteCB_ForcePalmStep4(struct Sprite* sprite)
{
	if (++sprite->data[0] >= 8)
	{
		sprite->data[0] = 0;
		sprite->callback = SpriteCB_ForcePalmStep5;
	}
	else
		sprite->pos1.x += 1;
}

void SpriteCB_ForcePalmStep5(struct Sprite* sprite)
{
	if (++sprite->data[0] >= 6)
		DestroyAnimSprite(sprite);
}


//Gravity//

//Shakes a mon bg horizontally and moves it downward linearly.
//arg 0: battler
//arg 1: x offset
//arg 2: frame delay between each movement
//arg 3: downward speed (subpixel)
//arg 4: duration
void AnimTask_ShakeAndSinkMon(u8 taskId)
{
	u8 spriteId = GetAnimBattlerSpriteId(gBattleAnimArgs[0]);
	if (spriteId < MAX_SPRITES)
	{	
		gSprites[spriteId].pos2.x = gBattleAnimArgs[1];
		gTasks[taskId].data[0] = spriteId;
		gTasks[taskId].data[1] = gBattleAnimArgs[1];
		gTasks[taskId].data[2] = gBattleAnimArgs[2];
		gTasks[taskId].data[3] = gBattleAnimArgs[3];
		gTasks[taskId].data[4] = gBattleAnimArgs[4];
		gTasks[taskId].func = (void*) (0x8098EF0 | 1); //AnimTask_ShakeAndSinkMon_Step
		gTasks[taskId].func(taskId);
	}
	else
		DestroyAnimVisualTask(taskId);
}


//Hammer Arm//
static const union AffineAnimCmd sSquishTargetAffineAnimCmds[] =
{
	AFFINEANIMCMD_FRAME(0, 64, 0, 16), //Flatten
	AFFINEANIMCMD_FRAME(0, 0, 0, 64),
	AFFINEANIMCMD_FRAME(0, -64, 0, 16),
	AFFINEANIMCMD_END,
};

void AnimTask_SquishTarget(u8 taskId)
{
	struct Task* task = &gTasks[taskId];
	u8 spriteId = GetAnimBattlerSpriteId(ANIM_TARGET);
	PrepareAffineAnimInTaskData(task, spriteId, sSquishTargetAffineAnimCmds);
	task->func = AnimTask_DynamaxGrowthStep;
}


//Heal Block//

//Creates the Block X on the chosen target
//arg 0: Initial x-pixel offset
//arg 1: Initial y-pixel offset
//arg 2: Target
void SpriteCB_BlockXOnTarget(struct Sprite *sprite)
{
	s16 y;
	u8 target = LoadBattleAnimTarget(2);

	if (SIDE(target) == B_SIDE_PLAYER)
	{
		sprite->subpriority = GetBattlerSpriteSubpriority(target) - 2;
		y = -144;
	}
	else
	{
		sprite->subpriority = GetBattlerSpriteSubpriority(target) + 2;
		y = -96;
	}

	if (!IsBattlerSpriteVisible(target))
		DestroyAnimSprite(sprite);
	else
	{
		InitSpritePosToGivenTarget(sprite, target);
		sprite->pos2.y = y;
		sprite->callback = (void*) (0x80E3534 | 1); //AnimBlockX_Step;
	}
}


//Heart Swap//
bool8 IsAnimMoveHeartSwap(void)
{
	return sAnimMoveIndex == MOVE_HEARTSWAP;
}


//Judgment//
static const union AffineAnimCmd sSpriteAffineAnim_JudgmentBall[] =
{
	AFFINEANIMCMD_FRAME(16, 16, 0, 0),
	AFFINEANIMCMD_FRAME(8, 8, 0, 15), //Half size
	AFFINEANIMCMD_FRAME(0, 0, 0, 120), //Delay
	AFFINEANIMCMD_FRAME(24, 24, 0, 5), //Normal size
	AFFINEANIMCMD_FRAME(0, 0, 0, 10), //Delay
	AFFINEANIMCMD_FRAME(-16, -16, 0, 15), //Revert to 1 px
	AFFINEANIMCMD_END,
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_JudgmentBall[] =
{
	sSpriteAffineAnim_JudgmentBall,
};


//Lunar Dance//
void SpriteCB_SurroundingRing(struct Sprite *sprite)
{
	sprite->pos1.x = GetBattlerSpriteCoord(gBattleAnimAttacker, 0);
	sprite->pos1.y = GetBattlerSpriteCoord(gBattleAnimAttacker, 1) + 40;

	sprite->data[0] = 13;
	sprite->data[2] = sprite->pos1.x;
	sprite->data[4] = sprite->pos1.y - 72;

	sprite->callback = StartAnimLinearTranslation;
	StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
}


//Mach Punch//
static const union AffineAnimCmd sSpriteAffineAnim_MachPunchFist[] =
{
	AFFINEANIMCMD_FRAME(256, 256, 0, 1), //Double sprite size
	AFFINEANIMCMD_FRAME(-32, -32, 4, 8),
	AFFINEANIMCMD_END,
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_MachPunchFist[] =
{
	sSpriteAffineAnim_MachPunchFist,
};


//Mud Bomb//
static const union AffineAnimCmd sSpriteAffineAnim_MudBombBall[] =
{
	AFFINEANIMCMD_FRAME(0, -8, -4, 4), //Compress Vertically
	AFFINEANIMCMD_FRAME(-8, 8, -4, 4), //Compress Horizontally, Normalize Vertically
	AFFINEANIMCMD_FRAME(8, -8, -4, 4), //Normalize Horizontally, Compress Vertically
	AFFINEANIMCMD_JUMP(1),
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_MudBombBall[] =
{
	sSpriteAffineAnim_MudBombBall,
};

bool8 IsAnimMoveMudBomb(void)
{
	return sAnimMoveIndex == MOVE_MUDBOMB;
}


//Night Slash//
static const union AffineAnimCmd sSpriteAffineAnim_NightSlashLeft[] =
{
	AFFINEANIMCMD_FRAME(32, 32, 0, 1), //Increase size by 1/4
	AFFINEANIMCMD_FRAME(0, 0, -32, 1), //Rotate
	AFFINEANIMCMD_END,
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_NightSlashLeft[] =
{
	sSpriteAffineAnim_NightSlashLeft,
};

static const union AffineAnimCmd sSpriteAffineAnim_NightSlashRight[] =
{
	AFFINEANIMCMD_FRAME(32, 32, 0, 1), //Increase size by 1/4
	AFFINEANIMCMD_FRAME(0, 0, 96, 1), //Rotate
	AFFINEANIMCMD_END,
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_NightSlashRight[] =
{
	sSpriteAffineAnim_NightSlashRight,
};

//Creates a sprite that moves left or right along the target.
//arg 0: Initial x-pixel offset
//arg 1: Initial y-pixel offset
//arg 2: Slice distance
//arg 3: Speed
//arg 4: direction
void SpriteCB_HorizontalSliceStep(struct Sprite *sprite);
void SpriteCB_HorizontalSlice(struct Sprite *sprite)
{
	sprite->pos2.x = gBattleAnimArgs[0];
	sprite->pos2.y = gBattleAnimArgs[1];

	sprite->data[0] = gBattleAnimArgs[2]; //Slice distance
	sprite->data[1] = gBattleAnimArgs[3]; //Slice speed
	sprite->data[2] = gBattleAnimArgs[4]; //Slice direction
	sprite->data[3] = 0; //Timer

	sprite->callback = SpriteCB_HorizontalSliceStep;
}

void SpriteCB_HorizontalSliceStep(struct Sprite *sprite)
{
	if (sprite->data[2] == 1) //Move left
		sprite->pos2.x -= sprite->data[1];
	else
		sprite->pos2.x += sprite->data[1];

	sprite->data[3] += sprite->data[1];

	if (sprite->data[3] >= sprite->data[0])
		DestroySpriteAndMatrix(sprite);
}


//Power Swap//
bool8 IsAnimMovePowerSwap(void)
{
	return sAnimMoveIndex == MOVE_POWERSWAP;
}


//Power Whip//
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


//Psycho Cut//
static const union AffineAnimCmd sSpriteAffineAnim_PsychoCutOpponent[] =
{
	AFFINEANIMCMD_FRAME(0, 0, 128, 1), //180 degree turn
	AFFINEANIMCMD_FRAME(4, 4, 0, 64), //Double in size
	AFFINEANIMCMD_END,
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_PsychoCutOpponent[] =
{
	sSpriteAffineAnim_PsychoCutOpponent,
};


//Shadow Sneak//
bool8 ShadowSneakAnimHelper(void)
{
	switch (sAnimMoveIndex)
	{
		case MOVE_SHADOWSNEAK:
		case MOVE_HYPERSPACEHOLE:
		case MOVE_SPECTRALTHIEF:
		case MOVE_POLTERGEIST:
		case MOVE_PULVERIZING_PANCAKE:
			return TRUE;
	}

	return FALSE;
}


//Stone Edge//

//Creates a rock for Stone Edge
//arg 0: Initial x-offset
//arg 1: Initial y-offset
void SpriteCB_StoneEdgeRockStep1(struct Sprite* sprite);
void SpriteCB_StoneEdgeRockStep2(struct Sprite* sprite);
void SpriteCB_StoneEdgeRock(struct Sprite* sprite)
{
	InitSpritePosToAnimTarget(sprite, TRUE);

	sprite->data[0] = 20; //Timer
	sprite->data[1] = 20;

	sprite->callback = SpriteCB_StoneEdgeRockStep1;
}

void SpriteCB_StoneEdgeRockStep1(struct Sprite* sprite)
{
	if (--sprite->data[0] > 0) //Timer
	{
		if ((sprite->data[0] & 3) == 0)
			++sprite->pos1.y; //Initial descent
	}
	else
		sprite->callback = SpriteCB_StoneEdgeRockStep2;
}

void SpriteCB_StoneEdgeRockStep2(struct Sprite* sprite)
{
	if (--sprite->data[1] > 0) //Timer
		sprite->pos1.y -= 4; //Rise up
	else
		DestroyAnimSprite(sprite);
}


//Wake-up Slap//
static const union AffineAnimCmd sSpriteAffineAnim_WakeUpSlap[] =
{
	AFFINEANIMCMD_FRAME(0, 0, 64, 1), //Rotate left 90 degrees
	AFFINEANIMCMD_FRAME(0, -32, 0, 7), //Flatten horizontally (on its side)
	AFFINEANIMCMD_FRAME(0, 0, 0, 8), //Do Nothing
	AFFINEANIMCMD_FRAME(0, -288, 0, 1), //Unflatten in other direction
	AFFINEANIMCMD_FRAME(0, 32, 0, 7), //Flatten horizontally (on its side)
	AFFINEANIMCMD_FRAME(0, 0, 0, 8), //Do Nothing
	AFFINEANIMCMD_FRAME(0, 288, 0, 1), //Unflatten in other direction
	AFFINEANIMCMD_JUMP(1),
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_WakeUpSlap[] =
{
	sSpriteAffineAnim_WakeUpSlap,
};

//Creates a sprite that moves right then then along the target.
//arg 0: Swipe distance
//arg 1: Speed
//arg 2: Less swipes (for Double Slap)
void SpriteCB_WakeUpSlap(struct Sprite *sprite)
{
	switch (sprite->data[7]) { //State
		case 0:
			sprite->pos2.x = gBattleAnimArgs[0];
			sprite->data[0] = -gBattleAnimArgs[0]; //Swipe distance
			sprite->data[1] = gBattleAnimArgs[1]; //Swipe speed
			sprite->data[6] = gBattleAnimArgs[2]; //Less Slaps
			++sprite->data[7];
			break;
		//Right
		case 1:
		case 3:
			sprite->pos2.x += sprite->data[1];

			if (sprite->pos2.x >= sprite->data[0])
			{
				sprite->data[0] = -sprite->data[0];
				++sprite->data[7];
			}
			break;
		//Left
		case 2:
		case 4:
			sprite->pos2.x -= sprite->data[1];

			if (sprite->pos2.x <= sprite->data[0])
			{
				sprite->data[0] = -sprite->data[0];

				if (sprite->data[6])
					sprite->data[7] = 5; //Destroy
				else
					++sprite->data[7];
			}
			break;
		case 5:
			DestroyAnimSprite(sprite);
			break;
	}
}


///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////Gen 5/////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

//Bestow//
void AnimTask_CreateBestowItem(u8 taskId)
{
	u8 iconSpriteId = AddItemIconSprite(ITEM_TAG, ITEM_TAG, gLastUsedItem);

	if (iconSpriteId != MAX_SPRITES)
	{
		gSprites[iconSpriteId].oam.priority = 2;
		gSprites[iconSpriteId].callback = (void*) 0x8075E81;
		++gAnimVisualTaskCount;
	}

	DestroyAnimVisualTask(taskId);
}


//Electroweb//
void SpriteCB_CentredSpiderWeb(struct Sprite* sprite)
{
	if (IS_DOUBLE_BATTLE)
		InitSpritePosToAnimTargetsCentre(sprite, FALSE);
	else
		InitSpritePosToAnimTarget(sprite, FALSE);

	sprite->callback = SpriteCB_SpiderWeb;
}


//Fusion Flare + Fusion Bolt//
static const union AffineAnimCmd sSpriteAffineAnimTable_FusionFlareGrow[] =
{
    AFFINEANIMCMD_FRAME(32, 8, 0, 0), //Start small - slightly stretched horizontally and compressed vertically
    AFFINEANIMCMD_FRAME(2, 2, -4, 120),
	AFFINEANIMCMD_END
};

static const union AffineAnimCmd sSpriteAffineAnimTable_FusionFlareSpin[] =
{
	AFFINEANIMCMD_FRAME(272, 256, 0, 0), //Keep shape from before
    AFFINEANIMCMD_FRAME(0, 0, -8, 1),
	AFFINEANIMCMD_JUMP(1)
};

static const union AffineAnimCmd sSpriteAffineAnimTable_FusionFlareShrinkOnTarget[] =
{
	AFFINEANIMCMD_FRAME(272, 256, -64, 0), //Keep shape from before
	AFFINEANIMCMD_FRAME(-60,-60, -8, 4), //Shrink in size
	AFFINEANIMCMD_FRAME(0, 0, -8, 4), //Pause
	AFFINEANIMCMD_END
};

static const union AffineAnimCmd sSpriteAffineAnimTable_FusionFlareGrowOnTarget[] =
{
	AFFINEANIMCMD_FRAME(16, 16, 0, 0),
	AFFINEANIMCMD_FRAME(16, 16, 0, 30), //Double in size
	AFFINEANIMCMD_END
};

static const union AffineAnimCmd sSpriteAffineAnimTable_FusionBoltGrow[] =
{
    AFFINEANIMCMD_FRAME(32, 8, 0, 0), //Start small - slightly stretched horizontally and compressed vertically
    AFFINEANIMCMD_FRAME(16, 16, -4, 15),
	AFFINEANIMCMD_END
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_FusionBall[] =
{
	sSpriteAffineAnimTable_FusionFlareGrow,
	sSpriteAffineAnimTable_FusionFlareSpin,
	sSpriteAffineAnimTable_FusionFlareShrinkOnTarget,
	sSpriteAffineAnimTable_FusionFlareGrowOnTarget,
	sSpriteAffineAnimTable_FusionBoltGrow,
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_FusionFlareExplosion[] =
{
	sSpriteAffineAnimTable_FusionFlareGrowOnTarget,
};

void AnimTask_IsPoweredUpFusionMove(u8 taskId)
{
	gBattleAnimArgs[0] = gAnimMovePower > gBattleMoves[sAnimMoveIndex].power; //Power has been changed from what it is normally
	DestroyAnimVisualTask(taskId);
}

//arg 0: initial x pixel offset
//arg 1: initial y pixel offset
//arg 2: pause before flying downwards
//arg 3: fly down duration
//arg 4: wave amplitude
static void SpriteCB_FusionFlareBall_Step1(struct Sprite* sprite);
static void SpriteCB_FusionFlareBall_Step2(struct Sprite* sprite);
static void SpriteCB_FusionFlareBall_Step3(struct Sprite* sprite);
static void SpriteCB_FusionFlareBall_Step4(struct Sprite* sprite);
void SpriteCB_FusionFlareBall(struct Sprite* sprite)
{		
	InitSpritePosToAnimAttacker(sprite, 0);

	if (SIDE(gBattleAnimAttacker) == B_SIDE_PLAYER)
		sprite->pos1.y -= 20; //Make it higher because more space

	//Set data for pause after growing
	sprite->data[6] = gBattleAnimArgs[2];

	//Set data for horizontal arc downwards
	sprite->data[0] = gBattleAnimArgs[3];
	sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X_2); //Target X
	sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET); //Target Y
	sprite->data[5] = gBattleAnimArgs[4];

	sprite->callback = SpriteCB_FusionFlareBall_Step1;
}

static void SpriteCB_FusionFlareBall_Step1(struct Sprite* sprite)
{
	if (sprite->affineAnimEnded)
	{
		StartSpriteAffineAnim(sprite, 1); //Spin
		sprite->callback = SpriteCB_FusionFlareBall_Step2;
	}
}

static void SpriteCB_FusionFlareBall_Step2(struct Sprite* sprite)
{
	if (sprite->data[7]++ >= sprite->data[6]) //Wait duration
	{
		sprite->data[6] = 0; //Reset
		sprite->data[7] = 0;  //Reset
		InitAnimArcTranslation(sprite);
		sprite->callback = SpriteCB_FusionFlareBall_Step3;
	}
}

static void SpriteCB_FusionFlareBall_Step3(struct Sprite* sprite)
{
	sprite->invisible = FALSE;

	if (TranslateAnimHorizontalArc(sprite)) //Arc down is done
	{
		StartSpriteAffineAnim(sprite, 2); //Shrink on target
		sprite->callback = SpriteCB_FusionFlareBall_Step4;
	}
}

static void SpriteCB_FusionFlareBall_Step4(struct Sprite* sprite)
{
	sprite->invisible = FALSE;

	if (sprite->affineAnimEnded)
		DestroyAnimSprite(sprite);
}

//arg 0: initial x pixel offset
//arg 1: initial y pixel offset
//arg 2: fly to x offset after initial grow
//arg 3: fly to y offset after initial grow
//arg 4: pause before flying downwards
//arg 5: fly down duration
//arg 6: wave amplitude
static void SpriteCB_FusionBoltBall_Step1(struct Sprite* sprite);
static void SpriteCB_FusionBoltBall_Step2(struct Sprite* sprite);
static void SpriteCB_FusionBoltBall_Step3(struct Sprite* sprite);
static void SpriteCB_FusionBoltBall_Step4(struct Sprite* sprite);
void SpriteCB_FusionBoltBall(struct Sprite* sprite)
{
	if (SIDE(gBattleAnimAttacker) == B_SIDE_OPPONENT)
		gBattleAnimArgs[2] *= -1; //Move the other direction

	StartSpriteAffineAnim(sprite, 4); //Fusion Bolt grow
	InitSpritePosToAnimAttacker(sprite, 0);

	//Set data for horizontal arc downwards
	sprite->data[0] = gBattleAnimArgs[5];
	sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X_2) + gBattleAnimArgs[2]; //Target X
	sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_Y_PIC_OFFSET) + gBattleAnimArgs[3]; //Target Y
	sprite->data[5] = gBattleAnimArgs[6];

	if (SIDE(gBattleAnimAttacker) == B_SIDE_PLAYER)
		sprite->data[4] -= 40; //Make it higher because more space

	InitAnimArcTranslation(sprite);
	sprite->callback = SpriteCB_FusionBoltBall_Step1;
}

static void SpriteCB_FusionBoltBall_Step1(struct Sprite* sprite)
{
	sprite->invisible = FALSE;

	if (TranslateAnimHorizontalArc(sprite)) //Arc up is done
		sprite->callback = SpriteCB_FusionBoltBall_Step2;
}

static void SpriteCB_FusionBoltBall_Step2(struct Sprite* sprite)
{
	if (sprite->affineAnimEnded)
	{
		StartSpriteAffineAnim(sprite, 1);
		sprite->data[1] = 0; //Reset timer
		sprite->callback = SpriteCB_FusionBoltBall_Step3;
	}
}

static void SpriteCB_FusionBoltBall_Step3(struct Sprite* sprite)
{
	if (sprite->data[1]++ >= gBattleAnimArgs[4]) //Wait duration - should still be there
	{
		sprite->pos1.x += sprite->pos2.x; //Merge to prepare for second arc
		sprite->pos1.y += sprite->pos2.y;
		sprite->pos2.x = 0;
		sprite->pos2.y = 0;
		sprite->data[0] = gBattleAnimArgs[5] / 2; //Reset - should still be there
		sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X_2); //Target X
		sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET); //Target Y
		sprite->data[5] = (gBattleAnimArgs[6] - 10) * -1; //Reset - should still be there
		InitAnimArcTranslation(sprite);
		sprite->callback = SpriteCB_FusionBoltBall_Step4;
	}
}

static void SpriteCB_FusionBoltBall_Step4(struct Sprite* sprite)
{
	sprite->invisible = FALSE;

	if (TranslateAnimHorizontalArc(sprite)) //Arc up is done
	{
		StartSpriteAffineAnim(sprite, 2); //Shrink on target
		sprite->callback = SpriteCB_FusionFlareBall_Step4; //Wait until affine anim is over then destroy
	}
}


//Hex//

//Creates purple flames that surround the target.
//No args.
void AnimTask_PurpleFlamesOnTarget(u8 taskId)
{
	struct Task *task = &gTasks[taskId];

	task->data[0] = 0;
	task->data[1] = 16;
	task->data[9] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X_2);
	task->data[10] = GetBattlerYCoordWithElevation(gBattleAnimTarget);
	task->data[11] = (GetBattlerSpriteCoordAttr(gBattleAnimTarget, BATTLER_COORD_ATTR_WIDTH) / 2) + 8;
	task->data[7] = 0;
	task->data[5] = GetBattlerSpriteBGPriority(gBattleAnimTarget);
	task->data[6] = GetBattlerSpriteSubpriority(gBattleAnimTarget) - 2;
	task->data[3] = 0;
	task->data[4] = 16;
	SetGpuReg(REG_OFFSET_BLDCNT, (BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL));
	SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(0, 0x10));
	task->data[8] = 0;
	task->func = (void*) (0x80B696C | 1); //AnimTask_GrudgeFlames_Step
}


//Icicle Crash//
static const union AffineAnimCmd sSpriteAffineAnim_IcicleCrash[] =
{
	AFFINEANIMCMD_FRAME(0, 0, 128, 1), //180 degree turn
	AFFINEANIMCMD_END
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_IcicleCrash[] =
{
	sSpriteAffineAnim_IcicleCrash,
};


//Incinerate//

//Targeted fire in Doubles for Incinerate
//Launches a projectile from the attacker to the target's centre.
//arg 0: initial x pixel offset
//arg 1: initial y pixel offset
//arg 2: target x pixel offset
//arg 3: target y pixel offset
//arg 4: duration
//arg 5: wave amplitude
//arg 6: target
void SpriteCB_IncinerateBall(struct Sprite* sprite)
{
	InitSpritePosToAnimAttacker(sprite, 1);

	if (SIDE(gBattleAnimAttacker) == B_SIDE_OPPONENT)
		gBattleAnimArgs[2] = -gBattleAnimArgs[2];

	if (IS_DOUBLE_BATTLE)
	{
		sprite->data[2] = (GetBattlerSpriteCoord2(gBattleAnimTarget, BATTLER_COORD_X_2)
					   +  GetBattlerSpriteCoord2(PARTNER(gBattleAnimTarget), BATTLER_COORD_X_2)) / 2;
		sprite->data[4] = (GetBattlerSpriteCoord2(gBattleAnimTarget, BATTLER_COORD_Y)
					   +  GetBattlerSpriteCoord2(PARTNER(gBattleAnimTarget), BATTLER_COORD_Y_PIC_OFFSET)) / 2;
	}
	else
	{
		sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X_2) + gBattleAnimArgs[2];
		sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET) + gBattleAnimArgs[3];
	}

	sprite->data[0] = gBattleAnimArgs[4];
	sprite->data[5] = gBattleAnimArgs[5];
	InitAnimArcTranslation(sprite);

	sprite->callback = SpriteCB_AnimMissileArcStep;
	sprite->invisible = TRUE;
}

//Creates fire that moves diagonally down the opponent
//arg 0: initial x pixel offset
//arg 1: initial y pixel offset
//arg 2: target x pixel offset
//arg 3: target y pixel offset
//arg 4: duration
//arg 5: bank
//arg 6: respect pic coords
void SpriteCB_IncinerateFlare(struct Sprite *sprite)
{
	bool8 respectPicCoords;
	u8 coordType;
	u8 target = LoadBattleAnimTarget(5);

	if (!IsBattlerSpriteVisible(target))
		DestroyAnimSprite(sprite);
	else
	{
		if (SIDE(gBattleAnimAttacker) == SIDE(gBattleAnimTarget)
		&& (gBattleAnimAttacker == GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT)
		 || gBattleAnimAttacker == GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT)))
		{
			gBattleAnimArgs[2] = -gBattleAnimArgs[2];
		}

		if (!gBattleAnimArgs[6])
		{
			respectPicCoords = TRUE;
			coordType = BATTLER_COORD_Y_PIC_OFFSET;
		}
		else
		{
			respectPicCoords = FALSE;
			coordType = BATTLER_COORD_Y;
		}
		
		if (respectPicCoords)
			InitSpritePosToGivenTargetRespectPicOffsets(sprite, target);
		else
			InitSpritePosToGivenTarget(sprite, target);

		if (SIDE(gBattleAnimAttacker) == B_SIDE_OPPONENT)
			gBattleAnimArgs[2] = -gBattleAnimArgs[2];

		sprite->data[0] = gBattleAnimArgs[4];
		sprite->data[2] = GetBattlerSpriteCoord(target, BATTLER_COORD_X_2) + gBattleAnimArgs[2];
		sprite->data[4] = GetBattlerSpriteCoord(target, coordType) + gBattleAnimArgs[3];
		sprite->callback = StartAnimLinearTranslation;
		StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
	}
}

//Same as burn chance anim but can be used on multiple targets
void SpriteCB_TargetedFireSpread(struct Sprite *sprite)
{
	u8 target = LoadBattleAnimTarget(5);
	
	if (IS_SINGLE_BATTLE
	&& (gBattleAnimArgs[5] == ANIM_ATK_PARTNER || gBattleAnimArgs[5] == ANIM_DEF_PARTNER)) //These targets don't exist and LoadBattleAnimTarget will treat as partner
		DestroyAnimSprite(sprite);
	else if (!IsBattlerSpriteVisible(target))
		DestroyAnimSprite(sprite);
	else
	{
		InitSpritePosToGivenTarget(sprite, target);

		sprite->pos2.y = 0; //Reset from being set in above command
		sprite->pos1.y += gBattleAnimArgs[1];
		sprite->data[0] = gBattleAnimArgs[4];
		sprite->data[1] = gBattleAnimArgs[2];
		sprite->data[2] = gBattleAnimArgs[3];

		sprite->callback = TranslateSpriteLinearFixedPoint;
		StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
	}
}


//Psyshock//
static const union AnimCmd sAnimCmdPsyshockOrb[] =
{
	ANIMCMD_FRAME(8, 1),
	ANIMCMD_END,
};

const union AnimCmd *const gAnimCmdTable_PsyshockOrb[] =
{
	sAnimCmdPsyshockOrb,
};


//Quick Guard//
static const union AnimCmd sAnimCmdQuickGuardLeft[] =
{
	ANIMCMD_END,
};

static const union AnimCmd sAnimCmdQuickGuardRight[] =
{
	ANIMCMD_FRAME(0, 1, .hFlip = TRUE),
	ANIMCMD_END,
};

const union AnimCmd *const gAnimCmdTable_QuickGuard[] =
{
	sAnimCmdQuickGuardLeft,
	sAnimCmdQuickGuardRight,
};


//Searing Shot//
static const union AffineAnimCmd sSpriteAffineAnim_SearingShotFlyingFlame[] =
{
	AFFINEANIMCMD_FRAME(16, 16, 20, 16), //Double in size and spin
	AFFINEANIMCMD_END
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_SearingShotFlyingFlame[] =
{
	sSpriteAffineAnim_SearingShotFlyingFlame,
};


//Shell Smash//
static const union AffineAnimCmd sShellSmashShrinkAffineAnimCmds[] =
{
	AFFINEANIMCMD_FRAME(64, 64, 0, 16), //Flatten
	AFFINEANIMCMD_FRAME(0, 0, 0, 48),
	AFFINEANIMCMD_FRAME(-64, -64, 0, 16),
	AFFINEANIMCMD_END,
};

//Shrinks the attacker, pauses, then scales up the attacker again
void AnimTask_ShellSmashShrinkAttacker(u8 taskId)
{
	struct Task* task = &gTasks[taskId];
	
	task->data[0] = gBattleAnimArgs[0]; //Pause

	u8 spriteId = GetAnimBattlerSpriteId(ANIM_TARGET);
	PrepareAffineAnimInTaskData(task, spriteId, sShellSmashShrinkAffineAnimCmds);
	task->func = AnimTask_DynamaxGrowthStep;
}

//Moves the shells towards the attacker and leaves them there until they fade out
//arg 0: Initial x-pos
//arg 1: Final x-pos
//arg 2: Movement duration
//arg 3: Affine anim
static void SpriteCB_ShellSmashShell_DestroyDuringFadeOut(struct Sprite* sprite);
void SpriteCB_ShellSmashShell(struct Sprite* sprite)
{
	//Init Position
	sprite->pos1.x = GetBattlerSpriteCoord2(gBattleAnimAttacker, BATTLER_COORD_X_2) + gBattleAnimArgs[0];
	sprite->pos1.y = GetBattlerSpriteCoord2(gBattleAnimAttacker, BATTLER_COORD_Y_PIC_OFFSET) + 2; //2 to slightly encompass the entire sprite

	//Prepare linear movement
	sprite->data[0] = gBattleAnimArgs[2]; //Duration
	sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X_2) + gBattleAnimArgs[1];
	sprite->data[4] = sprite->pos1.y;
	sprite->callback = StartAnimLinearTranslation;
	StoreSpriteCallbackInData6(sprite, SpriteCB_ShellSmashShell_DestroyDuringFadeOut);

	//Rotate properly
	StartSpriteAffineAnim(sprite, gBattleAnimArgs[3]);
}

static void SpriteCB_ShellSmashShell_DestroyDuringFadeOut(struct Sprite* sprite)
{
	if (GetGpuReg(REG_OFFSET_BLDALPHA) >= BLDALPHA_BLEND(0, 8)) //Fade out 1/2 done
		DestroyAnimSprite(sprite);
}


//Struggle Bug//

//Creates the hit splat for Struggle Bug
void SpriteCB_HitSplatOnMonEdgeDoubles(struct Sprite *sprite)
{	
	u8 target = LoadBattleAnimTarget(0);

	if (!IsBattlerSpriteVisible(target))
		DestroyAnimSprite(sprite);
	else
	{
		sprite->data[0] = gBattlerSpriteIds[target];
		sprite->pos1.x = gSprites[sprite->data[0]].pos1.x + gSprites[sprite->data[0]].pos2.x;
		sprite->pos1.y = gSprites[sprite->data[0]].pos1.y + gSprites[sprite->data[0]].pos2.y;
		sprite->pos2.x = gBattleAnimArgs[1];
		sprite->pos2.y = gBattleAnimArgs[2];
		StartSpriteAffineAnim(sprite, gBattleAnimArgs[3]);
		StoreSpriteCallbackInData6(sprite, DestroySpriteAndMatrix);
		sprite->callback = RunStoredCallbackWhenAffineAnimEnds;
	}
}


//Techno Blast//
void AnimTask_TechnoBlast(u8 taskId)
{
	gBattleAnimArgs[0] = gItems[GetBankPartyData(gBattleAnimAttacker)->item].holdEffectParam;
	DestroyAnimVisualTask(taskId);
}


//Volt Switch//

//Launches the projectiles for Volt Switch
//arg 0: initial x pixel offset
//arg 1: initial y pixel offset
//arg 2: target x pixel offset
//arg 3: target y pixel offset
//arg 4: duration
//arg 5: wave amplitude
static void SpriteCB_VoltSwitch_Step(struct Sprite* sprite);
void SpriteCB_VoltSwitch(struct Sprite* sprite)
{
	InitSpritePosToAnimAttacker(sprite, 0);

	if (SIDE(gBattleAnimAttacker) == B_SIDE_OPPONENT)
		gBattleAnimArgs[2] = -gBattleAnimArgs[2];
	else
		sprite->pos1.y += 10; //Move slightly down

	sprite->data[0] = gBattleAnimArgs[4];
	sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X_2) + gBattleAnimArgs[2]; //Target X
	sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET) + gBattleAnimArgs[3]; //Target Y
	sprite->data[5] = gBattleAnimArgs[5];
	InitAnimArcTranslation(sprite);

	sprite->callback = SpriteCB_VoltSwitch_Step;
}

static void SpriteCB_VoltSwitch_Step(struct Sprite* sprite)
{
	sprite->invisible = FALSE;

	if (TranslateAnimHorizontalArc(sprite))
	{
		//Merge coords into one
		sprite->pos1.x += sprite->pos2.x;
		sprite->pos1.y += sprite->pos2.y;
		sprite->pos2.x = 0;
		sprite->pos2.y = 0;

		//Come straight back to the attacker
		sprite->data[0] = 0x14; //Duration
		sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X_2);
		sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_Y_PIC_OFFSET);

		sprite->callback = StartAnimLinearTranslation;
		StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
	}
}


///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////Gen 6/////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

//Boomburst//

//Inverts the screen colours and all banks in a double battle
void AnimTask_InvertScreenColorDoubles(u8 taskId)
{
	u32 selectedPalettes = 0;

	if (gBattleAnimArgs[0] & 0x100)
		selectedPalettes |= SelectBattleAnimSpriteAndBgPalettes(TRUE, 0, 0, 0, 0, 0, 0);
	if (gBattleAnimArgs[1] & 0x100)
		selectedPalettes |= SelectBattleAnimSpriteAndBgPalettes(0, TRUE, 0, TRUE, 0, 0, 0); //Attacker and partner
	if (gBattleAnimArgs[2] & 0x100)
		selectedPalettes |= SelectBattleAnimSpriteAndBgPalettes(0, 0, TRUE, 0, TRUE, 0, 0); //Target and partner

	InvertPlttBuffer(selectedPalettes);
	DestroyAnimVisualTask(taskId);
}


//Dragon Ascent//
void AnimTask_IsAttackerRayquaza(u8 taskId)
{
	#ifdef NATIONAL_DEX_RAYQUAZA
	if (SpeciesToNationalPokedexNum(GetAnimAttackerSpecies()) == NATIONAL_DEX_RAYQUAZA)
		gBattleAnimArgs[0] = 1;
	else
	#endif
		gBattleAnimArgs[0] = 0;

	DestroyAnimVisualTask(taskId);
}

void AnimTask_IsAttackerShiny(u8 taskId)
{
	gBattleAnimArgs[0] = IsMonShiny(GetIllusionPartyData(gBattleAnimAttacker));
	DestroyAnimVisualTask(taskId);
}


//Fairy Lock//
bool8 IsAnimMoveFairyLock(void)
{
	return sAnimMoveIndex == MOVE_FAIRYLOCK;
}

//Destroys the chains for Fairy Lock
void SpriteCB_FairyLockChain(struct Sprite *sprite)
{
	if (++sprite->data[0] > 12)
	{
		--gTasks[sprite->data[6]].data[sprite->data[7]];
		DestroySprite(sprite);
	}
}


//Hoopa SOS//
static const union AnimCmd sAnimCmdHoopaRing[] =
{
	ANIMCMD_FRAME(0, 8),
	ANIMCMD_FRAME(16, 8),
	ANIMCMD_FRAME(32, 8),
	ANIMCMD_FRAME(48, 8),
	ANIMCMD_JUMP(0)
};

const union AnimCmd *const gAnimCmdTable_HoopaRing[] =
{
	sAnimCmdHoopaRing,
};

static const union AffineAnimCmd sSpriteAffineAnim_HoopaRing[] =
{
	AFFINEANIMCMD_FRAME(4, 4, 10, 1), //Grow & Spin
	AFFINEANIMCMD_JUMP(0),
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_HoopaRing[] =
{
	sSpriteAffineAnim_HoopaRing,
};

static const union AnimCmd sAnimCmdLargeHoopaRing[] =
{
	ANIMCMD_FRAME(0, 8),
	ANIMCMD_FRAME(64, 8),
	ANIMCMD_FRAME(128, 8),
	ANIMCMD_FRAME(192, 8),
	ANIMCMD_JUMP(0)
};

const union AnimCmd *const gAnimCmdTable_LargeHoopaRing[] =
{
	sAnimCmdLargeHoopaRing,
};

static const union AffineAnimCmd sSpriteAffineAnim_LargeHoopaRing[] =
{
	AFFINEANIMCMD_FRAME(16, 16, 0, 0), //Start small
	AFFINEANIMCMD_FRAME(4, 4, -16, 60), //Spin and grow sprite
	AFFINEANIMCMD_FRAME(0, 0, -16, 4), //Keep spinning until back to proper orientation
	AFFINEANIMCMD_FRAME(0, 0, 0, 0x22), //Pause
	AFFINEANIMCMD_FRAME(0, 0, 0, 64), //Pause
	AFFINEANIMCMD_FRAME(-16, -16, 16, 15), //Spin and shrink sprite
	AFFINEANIMCMD_END,
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_LargeHoopaRing[] =
{
	sSpriteAffineAnim_LargeHoopaRing,
};

static const union AffineAnimCmd sSpriteAffineAnim_GrowingRing[] =
{
	AFFINEANIMCMD_FRAME(8, 8, 0, 16), //Double in size
	AFFINEANIMCMD_END,
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_GrowingRing[] =
{
	sSpriteAffineAnim_GrowingRing,
};

static const union AffineAnimCmd sSpinAndGrowMonSpriteAffineAnimCmds[] =
{
	AFFINEANIMCMD_FRAME(16, 16, 0, 0), //Start small
	AFFINEANIMCMD_FRAME(4, 4, -16, 60), //Spin and grow sprite
	AFFINEANIMCMD_FRAME(0, 0, -16, 4), //Keep spinning until back to proper orientation
	AFFINEANIMCMD_FRAME(0, 0, 0, 2), //Pause
	AFFINEANIMCMD_END,
};

const union AffineAnimCmd* const sSpinAndGrowMonSpriteAffineAnimTable[] =
{
	sSpinAndGrowMonSpriteAffineAnimCmds,
};

#define tSpriteId data[0]
void AnimTask_SpinInAttacker_Step(u8 taskId);
void AnimTask_SpinInAttacker(u8 taskId)
{
	struct Task* task = &gTasks[taskId];
	u8 spriteId = GetAnimBattlerSpriteId(ANIM_ATTACKER);
	PrepareBattlerSpriteForRotScale(spriteId, ST_OAM_OBJ_NORMAL);

	if (spriteId < MAX_SPRITES)
	{
		struct Sprite* sprite = &gSprites[spriteId];

		task->data[1] = ((u32) sprite->affineAnims) & 0xFFFF; //Backup old affine anims
		task->data[2] = ((u32) sprite->affineAnims) >> 16;
		sprite->affineAnims = sSpinAndGrowMonSpriteAffineAnimTable;
		StartSpriteAffineAnim(sprite, 0);
		sprite->affineAnimPaused = FALSE;

		task->tSpriteId = spriteId;
		task->func = AnimTask_SpinInAttacker_Step;
	}
	else
		DestroyAnimVisualTask(taskId);
}

void AnimTask_SpinInAttacker_Step(u8 taskId)
{
	struct Task* task = &gTasks[taskId];

	if (gSprites[task->tSpriteId].affineAnimEnded)
	{
		gSprites[task->tSpriteId].affineAnims = (void*) (task->data[1] | (task->data[2] << 16)); //Restore old affine anims
		ResetSpriteRotScale(task->tSpriteId);
		gSprites[task->tSpriteId].affineAnimPaused = TRUE; //So the Pokemon coming out a Poke Ball animation doesn't play
		DestroyAnimVisualTask(taskId);
	}
}
#undef tSpriteId


//Hyperspace Fury//
static const union AffineAnimCmd sSpriteAffineAnim_HyperspaceFuryEnemySide[] =
{
	AFFINEANIMCMD_FRAME(0, 0, 32, 1), //Rotate 45 degrees left
	AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sSpriteAffineAnim_HyperspaceFuryPlayerSide[] =
{
	AFFINEANIMCMD_FRAME(0, 0, -32, 1), //Rotate 45 degrees right
	AFFINEANIMCMD_END,
};

static const union AffineAnimCmd* const sSpriteAffineAnim_HyperspaceFuryMon[] =
{
	sSpriteAffineAnim_HyperspaceFuryEnemySide,
	sSpriteAffineAnim_HyperspaceFuryPlayerSide,
};

//Creates a sideways sprite of the attacker behind the enemy's side
void AnimTask_CreateHyperspaceFuryMon(u8 taskId)
{
	u8 spriteId, subpriority, isBackPic;
	u16 species;
	s16 x;
	u32 personality, otId;
	struct Pokemon* monAtk;

	switch (gTasks[taskId].data[0]) {
		case 0: //Set up sprite
			monAtk = GetIllusionPartyData(gBattleAnimAttacker);
			personality = GetMonData(monAtk, MON_DATA_PERSONALITY, NULL);
			otId = GetMonData(monAtk, MON_DATA_OT_ID, NULL);
			if (gBattleSpritesDataPtr->bankData[gBattleAnimAttacker].transformSpecies == SPECIES_NONE)
				species = GetMonData(monAtk, MON_DATA_SPECIES, NULL);
			else
				species = gBattleSpritesDataPtr->bankData[gBattleAnimAttacker].transformSpecies;

			if (SIDE(gBattleAnimAttacker) == B_SIDE_PLAYER)
			{
				subpriority = gSprites[GetAnimBattlerSpriteId(ANIM_TARGET)].subpriority + 1;
				isBackPic = 0;
				x = 272;
			}
			else
			{
				subpriority = gSprites[GetAnimBattlerSpriteId(ANIM_TARGET)].subpriority - 1;
				isBackPic = 1;
				x = -32;
			}

			spriteId = CreateMonPicBattleAnim(species, isBackPic, 0, x, GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y), subpriority, personality, otId, gBattleAnimAttacker, 0);
			FreeSpriteOamMatrix(&gSprites[spriteId]);
			gSprites[spriteId].oam.affineMode = ST_OAM_AFFINE_NORMAL;
			gSprites[spriteId].affineAnims = sSpriteAffineAnim_HyperspaceFuryMon;
			CalcCenterToCornerVec(&gSprites[spriteId], gSprites[spriteId].oam.shape, gSprites[spriteId].oam.size, gSprites[spriteId].oam.affineMode);
			InitSpriteAffineAnim(&gSprites[spriteId]);
			if (SIDE(gBattleAnimAttacker) == B_SIDE_OPPONENT)
				StartSpriteAffineAnim(&gSprites[spriteId], 1);
			PlayCry1(species, 0);
			gTasks[taskId].data[1] = spriteId;
			gTasks[taskId].data[0]++;
			break;
		case 1: //Hoopa pop in
			spriteId = gTasks[taskId].data[1];
			if (SIDE(gBattleAnimAttacker) == B_SIDE_PLAYER)
			{
				if (gSprites[spriteId].pos2.x > -30)
					gSprites[spriteId].pos2.x -= 3;
				else if (!IsCryPlaying())
					gTasks[taskId].data[0]++;
			}
			else
			{
				if (gSprites[spriteId].pos2.x < 30)
					gSprites[spriteId].pos2.x += 3;
				else if (!IsCryPlaying())
					gTasks[taskId].data[0]++;
			}
			break;
		case 2: //Hoopa pop out
			spriteId = gTasks[taskId].data[1];
			if (SIDE(gBattleAnimAttacker) == B_SIDE_PLAYER)
			{
				if (gSprites[spriteId].pos2.x < 0)
					gSprites[spriteId].pos2.x += 4;
				else
					gTasks[taskId].data[0]++;
			}
			else
			{
				if (gSprites[spriteId].pos2.x > 0)
					gSprites[spriteId].pos2.x -= 4;
				else
					gTasks[taskId].data[0]++;
			}
			break;
		case 3: //Destroy sprite
			spriteId = gTasks[taskId].data[1];
			ClearPokemonCrySongs();
			DestroySpriteAndFreeResources(&gSprites[spriteId]);
			DestroyAnimVisualTask(taskId);
			break;
	}
}


//Ion Deluge//
bool8 IsAnimMoveIonDeluge(void)
{
	return gBattleBufferA[gBattleAnimAttacker][0] == CONTROLLER_MOVEANIMATION && sAnimMoveIndex == MOVE_IONDELUGE;
}


//King's Shield//
bool8 IsAnimMoveKingsShield(void)
{
	return sAnimMoveIndex == MOVE_KINGSSHIELD;
}


//Parabolic Charge//

//Creates a bolt of lightning on the designated target
//arg 0: Target
void AnimTask_TargetedLightning(u8 taskId)
{
	struct Task *task = &gTasks[taskId];

	switch (task->data[0])
	{
		case 0: ;
			u8 target = LoadBattleAnimTarget(0);

			if (IS_SINGLE_BATTLE
			&& (gBattleAnimArgs[0] == ANIM_ATK_PARTNER || gBattleAnimArgs[0] == ANIM_DEF_PARTNER)) //These targets don't exist and LoadBattleAnimTarget will treat as user
			{
				DestroyAnimVisualTask(taskId);
				return;
			}

			if (!IsBattlerSpriteVisible(target))
			{
				DestroyAnimVisualTask(taskId);
				return;
			}

			task->data[15] = GetBattlerSpriteCoord(target, BATTLER_COORD_Y) + 32;
			task->data[14] = task->data[15];
			while (task->data[14] > 16)
				task->data[14] -= 32;
			task->data[13] = GetBattlerSpriteCoord(target, BATTLER_COORD_X_2);
			task->data[12] = GetBattlerSpriteSubpriority(target) - 2;
			++task->data[0];
			break;
		case 1:
			if (++task->data[1] > 1)
			{
				task->data[1] = 0;
				if (CreateShockWaveLightning(task, taskId))
					++task->data[0];
			}
			break;
		case 2:
			if (task->data[10] == 0)
				DestroyAnimVisualTask(taskId);
			break;
	}
}


///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////Gen 7/////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

//Anchor Shot//
bool8 IsAnimMoveWithChain(void)
{
	return sAnimMoveIndex == MOVE_ANCHORSHOT
		|| sAnimMoveIndex == MOVE_SPIRITSHACKLE;
}


//Core Enforcer//
bool8 IsAnimMoveCoreEnforcer(void)
{
	return sAnimMoveIndex == MOVE_COREENFORCER;
}

void SpriteCB_CoreEnforcerHits(struct Sprite* sprite)
{
	StartSpriteAffineAnim(sprite, gBattleAnimArgs[3]);

	if (gBattleAnimArgs[2] == 0)
	{
		if (IS_DOUBLE_BATTLE)
			InitSpritePosToAnimAttackersCentre(sprite, FALSE);
		else
			InitSpritePosToAnimAttacker(sprite, FALSE);
	}
	else
	{
		if (IS_DOUBLE_BATTLE)
			InitSpritePosToAnimTargetsCentre(sprite, FALSE);
		else
			InitSpritePosToAnimTarget(sprite, FALSE);
	}

	sprite->pos1.y += 20;
	sprite->callback = SpriteCB_80BA7BC;
}

void SpriteCB_CoreEnforcerBeam(struct Sprite* sprite)
{
	if (!(IS_DOUBLE_BATTLE))
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


//Mind Blown//

//Spins a sprite towards the target, pausing in the middle.
//Used in Mind Blown.
//arg 0: duration step 1 (attacker -> center)
//arg 1: duration step 2 (spin center)
//arg 2: duration step 3 (center -> target)
static void AnimMindBlownBallStep(struct Sprite *sprite);
void SpriteCB_MindBlownBall(struct Sprite *sprite)
{
	s16 oldPosX = sprite->pos1.x;
	s16 oldPosY = sprite->pos1.y;
	sprite->pos1.x = GetBattlerSpriteCoord(gBattleAnimAttacker, 2);
	sprite->pos1.y = GetBattlerSpriteCoord(gBattleAnimAttacker, 3);
	sprite->data[0] = 0;
	sprite->data[1] = gBattleAnimArgs[0];
	sprite->data[2] = gBattleAnimArgs[1];
	sprite->data[3] = gBattleAnimArgs[2];
	sprite->data[4] = sprite->pos1.x << 4;
	sprite->data[5] = sprite->pos1.y << 4;
	sprite->data[6] = ((oldPosX - sprite->pos1.x) << 4) / (gBattleAnimArgs[0] << 1);
	sprite->data[7] = ((oldPosY - sprite->pos1.y) << 4) / (gBattleAnimArgs[0] << 1);
	sprite->callback = AnimMindBlownBallStep;
}

static void AnimMindBlownBallStep(struct Sprite *sprite)
{
	switch (sprite->data[0])
	{
		case 0:
			sprite->data[4] += sprite->data[6];
			sprite->data[5] += sprite->data[7];
			sprite->pos1.x = sprite->data[4] >> 4;
			sprite->pos1.y = sprite->data[5] >> 4;
			sprite->data[1] -= 1;
			if (sprite->data[1] > 0)
				break;
			sprite->data[0] += 1;
			break;
		case 1:
			sprite->data[2] -= 1;
			if (sprite->data[2] > 0)
				break;

			sprite->data[1] = GetProperCentredCoord(gBattleAnimTarget, BATTLER_COORD_X_2);
			sprite->data[2] = GetProperCentredCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET);
			sprite->data[4] = sprite->pos1.x << 4;
			sprite->data[5] = sprite->pos1.y << 4;
			sprite->data[6] = ((sprite->data[1] - sprite->pos1.x) << 4) / sprite->data[3];
			sprite->data[7] = ((sprite->data[2] - sprite->pos1.y) << 4) / sprite->data[3];
			sprite->data[0] += 1;
			break;
		case 2:
			sprite->data[4] += sprite->data[6];
			sprite->data[5] += sprite->data[7];
			sprite->pos1.x = sprite->data[4] >> 4;
			sprite->pos1.y = sprite->data[5] >> 4;
			sprite->data[3] -= 1;
			if (sprite->data[3] > 0)
				break;
			sprite->pos1.x = GetProperCentredCoord(gBattleAnimTarget, BATTLER_COORD_X_2);
			sprite->pos1.y = GetProperCentredCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET);
			sprite->data[0] += 1;
			break;
		case 3:
			DestroySpriteAndMatrix(sprite);
			break;
	}
}

void SpriteCB_MindBlownExplosion(struct Sprite* sprite)
{
	u8 a;
	u8 b;
	u16 x;
	u16 y;

	if (gBattleAnimArgs[4] == 0)
	{
		DestroyAnimSprite(sprite);
	}
	else
	{
		a = GetProperCentredCoord(gBattleAnimTarget, BATTLER_COORD_X_2);
		b = GetProperCentredCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET);

		sprite->data[0] = gBattleAnimArgs[4];
		if (gBattleAnimArgs[1] == 0)
		{
			sprite->pos1.x = gBattleAnimArgs[2] + a;
			sprite->pos1.y = gBattleAnimArgs[3] + b;
			sprite->data[5] = a;
			sprite->data[6] = b;
		}
		else
		{
			sprite->pos1.x = a;
			sprite->pos1.y = b;
			sprite->data[5] = gBattleAnimArgs[2] + a;
			sprite->data[6] = gBattleAnimArgs[3] + b;
		}

		x = sprite->pos1.x;
		sprite->data[1] = x * 16;
		y = sprite->pos1.y;
		sprite->data[2] = y * 16;
		sprite->data[3] = (sprite->data[5] - sprite->pos1.x) * 16 / gBattleAnimArgs[4];
		sprite->data[4] = (sprite->data[6] - sprite->pos1.y) * 16 / gBattleAnimArgs[4];

		sprite->callback = (void*) (0x80A43A0 | 1); //AnimNeedleArmSpikeStep
	}
}


//Moongeist Beam//

//arg 0: initial x pixel offset
//arg 1: initial y pixel offset
//arg 2: target x pixel offset
//arg 3: target y pixel offset
//arg 4: duration
//arg 5: wave amplitude
void SpriteCB_MoongeistCharge(struct Sprite *sprite)
{
	sprite->pos1.x = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X_2) + gBattleAnimArgs[0];
	sprite->pos1.y = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_Y_PIC_OFFSET) + gBattleAnimArgs[1];

	sprite->data[0] = gBattleAnimArgs[4];
	sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X_2) + gBattleAnimArgs[2]; //Target X
	sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_Y_PIC_OFFSET) + gBattleAnimArgs[3]; //Target Y
	sprite->data[5] = gBattleAnimArgs[5];

	InitAnimArcTranslation(sprite);
	sprite->callback = SpriteCB_AnimMissileArcStep;
}


//Prismatic Laser//
static const union AffineAnimCmd sSpriteAffineAnim_FallingTealAlert[] =
{
	AFFINEANIMCMD_FRAME(0, 0, 32, 1), //45 degree turn
	AFFINEANIMCMD_END
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_FallingTealAlert[] =
{
	sSpriteAffineAnim_FallingTealAlert,
};


//Sunsteel Strike//
const struct OamData sSunsteelStrikeBlastOAM =
{
	.affineMode = ST_OAM_AFFINE_DOUBLE,
	.objMode = ST_OAM_OBJ_NORMAL,
	.shape = SPRITE_SHAPE(64x64),
	.size = SPRITE_SIZE(64x64),
	.priority = 1, //Above sprites
};

static const union AffineAnimCmd sSpriteAffineAnim_SunsteelStrikeBlastEnemySide[] =
{
	AFFINEANIMCMD_FRAME(0, 0, -64, 1), //90 degree turn
	AFFINEANIMCMD_FRAME(0, 0, 0, 7), //Pause
	AFFINEANIMCMD_FRAME(16, 16, 0, 15), //Double in size
	AFFINEANIMCMD_END
};

static const union AffineAnimCmd sSpriteAffineAnim_SunsteelStrikeBlastPlayerSide[] =
{
	AFFINEANIMCMD_FRAME(0, 0, 128, 1), //180 degree turn
	AFFINEANIMCMD_FRAME(0, 0, 0, 7), //Pause
	AFFINEANIMCMD_FRAME(16, 16, 0, 15), //Double in size
	AFFINEANIMCMD_END
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_SunsteelStrikeBlast[] =
{
	sSpriteAffineAnim_SunsteelStrikeBlastEnemySide,
	sSpriteAffineAnim_SunsteelStrikeBlastPlayerSide,
};

void SpriteCB_SunsteelStrikeRings(struct Sprite* sprite)
{
	if (SIDE(gBattleAnimAttacker) != B_SIDE_PLAYER)
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
	sprite->callback = (void*) (0x80B1CC0 | 1); //AnimFlyBallAttack_Step
}


//Toxic Thread//
void SpriteCB_ToxicThreadWrap(struct Sprite *sprite)
{
	if (SIDE(gBattleAnimAttacker) != B_SIDE_PLAYER)
		sprite->pos1.x -= gBattleAnimArgs[0];
	else
		sprite->pos1.x += gBattleAnimArgs[0];

	sprite->pos1.y += gBattleAnimArgs[1];
	sprite->callback = (void*) (0x80B4274 | 1); //AnimStringWrap_Step
}


///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////Gen 8/////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

//Apple Acid / Grav Apple//
const struct OamData sAppleOAM =
{
	.affineMode = ST_OAM_AFFINE_DOUBLE,
	.objMode = ST_OAM_OBJ_NORMAL,
	.shape = SPRITE_SHAPE(32x32),
	.size = SPRITE_SIZE(32x32),
	.priority = 1, //Above sprites
};

static const union AffineAnimCmd sSpriteAffineAnim_ScaledApple[] =
{
	AFFINEANIMCMD_FRAME(32, 32, 0, 10), //Double in size
	AFFINEANIMCMD_END
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_ScaledApple[] =
{
	sSpriteAffineAnim_ScaledApple,
};


//Apple Acid//
//Throws acid at a single target.
void SpriteCB_AcidLaunchSingleTarget(struct Sprite *sprite)
{
	s16 l1, l2;
	if (!gBattleAnimArgs[3])
		StartSpriteAnim(sprite, 2);

	InitSpritePosToAnimTarget(sprite, TRUE);
	l1 = sprite->pos1.x; l2 = sprite->pos1.y;
	InitSpritePosToAnimAttacker(sprite, TRUE);

	if (SIDE(gBattleAnimAttacker))
		gBattleAnimArgs[4] = -gBattleAnimArgs[4];

	sprite->data[0] = gBattleAnimArgs[2];
	sprite->data[2] = l1 + gBattleAnimArgs[4];
	sprite->data[4] = l2 + gBattleAnimArgs[5];
	sprite->data[5] = -30;

	InitAnimArcTranslation(sprite);

	sprite->callback = (void*) (0x80B1728 | 1);
}

//Causes acid to drip down a single target.
void SpriteCB_AcidDripSingleTarget(struct Sprite *sprite)
{
	if (SIDE(gBattleAnimAttacker) != B_SIDE_PLAYER)
		gBattleAnimArgs[0] = -gBattleAnimArgs[0];

	sprite->pos1.x += gBattleAnimArgs[0];
	sprite->pos1.y += gBattleAnimArgs[1];

	sprite->data[0] = gBattleAnimArgs[4];
	sprite->data[2] = sprite->pos1.x + gBattleAnimArgs[2];
	sprite->data[4] = sprite->pos1.y + sprite->data[0];

	sprite->callback = StartAnimLinearTranslation;
	StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
}


//Behemoth Blade//
const union AffineAnimCmd sSpriteAffineAnim_BehemothBlade[] =
{
	AFFINEANIMCMD_FRAME(0x20, 0x200, 0, 0),
	AFFINEANIMCMD_FRAME(0x24, 0x0, 0, 12),
	AFFINEANIMCMD_FRAME(0x0, 0x0, 0, 32),
	AFFINEANIMCMD_END,
};

const union AffineAnimCmd *const gSpriteAffineAnimTable_BehemothBlade[] =
{
	sSpriteAffineAnim_BehemothBlade,
};

static const union AffineAnimCmd sSpriteAffineAnim_LargeExplosion[] =
{
	AFFINEANIMCMD_FRAME(256, 256, 0, 1), //Double sprite size
	AFFINEANIMCMD_FRAME(0x0, 0x0, 0, 32),
	AFFINEANIMCMD_END,
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_LargeExplosion[] =
{
	sSpriteAffineAnim_LargeExplosion,
};


//Court Change//
const struct OamData gCourtChangeBallOam =
{
	.affineMode = ST_OAM_AFFINE_DOUBLE,
	.objMode = ST_OAM_OBJ_NORMAL,
	.shape = SPRITE_SHAPE(16x16),
	.size = SPRITE_SIZE(16x16),
	.priority = 1, //Above sprites
};


//Dragon Darts//
static const union AnimCmd sAnimCmdDreepyMissilePlayer[] =
{
	ANIMCMD_END,
};

static const union AnimCmd sAnimCmdDreepyMissileOpponent[] =
{
	ANIMCMD_FRAME(16, 1),
	ANIMCMD_END,
};

const union AnimCmd *const gAnimCmdTable_DreepyMissile[] =
{
	sAnimCmdDreepyMissilePlayer,
	sAnimCmdDreepyMissileOpponent,
};

//Launches the Dreepy Missile
//arg 0: Target X-Pos
//arg 1: Target Y-Pos
//arg 2: Duration
void SpriteCB_DragonDart(struct Sprite* sprite)
{
	InitSpritePosToAnimAttacker(sprite, TRUE);
	if (SIDE(gBattleAnimTarget) == B_SIDE_OPPONENT)
		StartSpriteAnim(sprite, 1);

	sprite->data[0] = gBattleAnimArgs[2]; //Speed delay
	sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X_2) + gBattleAnimArgs[0];
	sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET) + gBattleAnimArgs[1];
	sprite->callback = StartAnimLinearTranslation;
	StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
}


//Dragon Energy//

//Moves the balls for Dragon Energy sideways along the target side
//arg 0: Duration
void SpriteCB_DragonEnergyShot(struct Sprite* sprite)
{
	s16 startingX, finishingX, y;
	u8 def1 = gBattleAnimTarget;
	u8 def2 = PARTNER(def1);

	if (IS_SINGLE_BATTLE || SIDE(gBattleAnimAttacker) == SIDE(gBattleAnimTarget))
		y = GetBattlerSpriteCoord(def1, BATTLER_COORD_Y_PIC_OFFSET);
	else
	{
		y = 0;
		
		if (IsBattlerSpritePresent(def1))
			y = GetBattlerSpriteCoord(def1, BATTLER_COORD_Y_PIC_OFFSET);
		
		if (IsBattlerSpritePresent(def2))
			y += GetBattlerSpriteCoord(def2, BATTLER_COORD_Y_PIC_OFFSET);

		if (IsBattlerSpritePresent(def1) && IsBattlerSpritePresent(def2)) //Both targets are visible
			y /= 2;
	}

	if (SIDE(gBattleAnimTarget) == B_SIDE_OPPONENT)
	{
		startingX = 0;
		finishingX = 255;
	}
	else
	{
		startingX = 255;
		finishingX = 0;
	}

	sprite->pos1.x = startingX;
	sprite->pos1.y = y;
	sprite->pos2.x = 0;
	sprite->data[0] = gBattleAnimArgs[0];
	sprite->data[2] = finishingX;
	sprite->data[4] = y;
	sprite->callback = StartAnimLinearTranslation;
	StoreSpriteCallbackInData6(sprite, DestroySpriteAndMatrix);
}


//Fishious Rend//
const struct OamData sFishiousRendTeethOam =
{
	.affineMode = ST_OAM_AFFINE_OFF,
	.objMode = ST_OAM_OBJ_NORMAL,
	.shape = SPRITE_SHAPE(64x64),
	.size = SPRITE_SIZE(64x64),
	.priority = 1, //Above sprites
};

//Creates a sprite that moves left then right along the target.
//arg 0: Slice distance
//arg 1: Speed
void SpriteCB_LeftRightSliceStep0(struct Sprite *sprite);
void SpriteCB_LeftRightSliceStep1(struct Sprite *sprite);
void SpriteCB_LeftRightSlice(struct Sprite *sprite)
{
	sprite->pos2.x = gBattleAnimArgs[0];
	sprite->data[0] = -gBattleAnimArgs[0]; //Slice distance
	sprite->data[1] = gBattleAnimArgs[1]; //Slice speed
	sprite->callback = SpriteCB_LeftRightSliceStep0;
}

void SpriteCB_LeftRightSliceStep0(struct Sprite *sprite)
{
	sprite->pos2.x -= sprite->data[1];

	if (sprite->pos2.x <= sprite->data[0])
	{
		sprite->data[0] = -sprite->data[0];
		sprite->callback = SpriteCB_LeftRightSliceStep1;
	}
}

void SpriteCB_LeftRightSliceStep1(struct Sprite *sprite)
{
	sprite->pos2.x += sprite->data[1];

	if (sprite->pos2.x >= sprite->data[0])
		DestroyAnimSprite(sprite);
}


//Glacial Lance//
static const union AffineAnimCmd sSpriteAffineAnim_GlacialLance_Grow[] =
{
    AFFINEANIMCMD_FRAME(0x20, 0x20, 0, 0),
    AFFINEANIMCMD_FRAME(0x10, 0x10, 0, 30), //Double sprite size
	AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sSpriteAffineAnim_GlacialLance_OnFoe[] =
{
	AFFINEANIMCMD_FRAME(256, 256, 0, 1), //Double sprite size
	AFFINEANIMCMD_FRAME(0, 0, -8, 4), //Rotate 45 degrees right
	AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sSpriteAffineAnim_GlacialLance_OnPlayer[] =
{
	AFFINEANIMCMD_FRAME(256, 256, 0, 1), //Double sprite size
	AFFINEANIMCMD_FRAME(0, 0, 24, 4), //Rotate 135 degrees left
	AFFINEANIMCMD_END,
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_GlacialLance[] =
{
	sSpriteAffineAnim_GlacialLance_Grow,
	sSpriteAffineAnim_GlacialLance_OnFoe,
	sSpriteAffineAnim_GlacialLance_OnPlayer,
};

//Moves the ice lance for Glacial Lance
//arg 0: initial x pixel offset (from attacker)
//arg 1: initial y pixel offset (from attacker)
//arg 2: target x pixel offset (from target)
//arg 3: target y pixel offset (from target)
//arg 4: distance moved up
//arg 5: pause in middle
//arg 6: duration to target
static void SpriteCB_GlacialLance_Step1(struct Sprite* sprite);
static void SpriteCB_GlacialLance_Step2(struct Sprite* sprite);
void SpriteCB_GlacialLance(struct Sprite* sprite)
{
	u8 def1 = gBattleAnimTarget;
	u8 def2 = PARTNER(def1);
	
	InitSpritePosToAnimAttacker(sprite, TRUE);
	sprite->data[5] = gBattleAnimArgs[4];
	sprite->data[6] = gBattleAnimArgs[5];
	
	sprite->data[0] = gBattleAnimArgs[6];
	
	if (IS_SINGLE_BATTLE || SIDE(gBattleAnimAttacker) == SIDE(gBattleAnimTarget))
	{
		sprite->data[2] = GetBattlerSpriteCoord(def1, BATTLER_COORD_X_2) + gBattleAnimArgs[2]; //Converge on target
		sprite->data[4] = GetBattlerSpriteCoord(def1, BATTLER_COORD_Y_PIC_OFFSET) + gBattleAnimArgs[3];
	}
	else
	{
		sprite->data[2] = (GetBattlerSpriteCoord(def1, BATTLER_COORD_X_2) + GetBattlerSpriteCoord(def2, BATTLER_COORD_X_2)) / 2 + gBattleAnimArgs[2]; //Converge on target
		sprite->data[4] = (GetBattlerSpriteCoord(def1, BATTLER_COORD_Y_PIC_OFFSET) + GetBattlerSpriteCoord(def2, BATTLER_COORD_Y_PIC_OFFSET)) / 2 + gBattleAnimArgs[3];
	}

	sprite->oam.priority = 1; //Above the ice cube
	sprite->callback = SpriteCB_GlacialLance_Step1;
}

static void SpriteCB_GlacialLance_Step1(struct Sprite* sprite)
{
	if (sprite->data[7]++ >= sprite->data[5])
	{
		sprite->data[7] = 0;
		sprite->callback = SpriteCB_GlacialLance_Step2;
	}
	else
		--sprite->pos1.y; //Move up
}

static void SpriteCB_GlacialLance_Step2(struct Sprite* sprite)
{
	if (sprite->data[7]++ >= sprite->data[6])
	{
		if (SIDE(gBattleAnimTarget) == B_SIDE_PLAYER)
			StartSpriteAffineAnim(sprite, 2);
		else
			StartSpriteAffineAnim(sprite, 1);

		sprite->data[5] = 0;
		sprite->data[6] = 0;
		sprite->data[7] = 0;
		sprite->callback = StartAnimLinearTranslation;
		StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
	}
}

void AnimTask_CentredFrozenIceCube(u8 taskId)
{
	s16 x, y;
	u8 spriteId;
	u8 bank1 = gBattleAnimTarget;
	u8 bank2 = PARTNER(bank1);

	if (IS_SINGLE_BATTLE || SIDE(gBattleAnimAttacker) == SIDE(gBattleAnimTarget))
	{
		x = GetBattlerSpriteCoord(bank1, BATTLER_COORD_X_2);
		y = GetBattlerSpriteCoord(bank1, BATTLER_COORD_Y_PIC_OFFSET);
	}
	else
	{
		x = (GetBattlerSpriteCoord(bank1, BATTLER_COORD_X_2) + GetBattlerSpriteCoord(bank2, BATTLER_COORD_X_2)) / 2;
		y = (GetBattlerSpriteCoord(bank1, BATTLER_COORD_Y_PIC_OFFSET) + GetBattlerSpriteCoord(bank2, BATTLER_COORD_Y_PIC_OFFSET)) / 2;
	}

	x -= 32;
	y -= 36;

	spriteId = CreateSprite((void*) 0x83BF55C, x, y, 4);
	if (GetSpriteTileStartByTag(ANIM_TAG_ICE_CUBE) == SPRITE_INVALID_TAG)
		gSprites[spriteId].invisible = TRUE;

	SetSubspriteTables(&gSprites[spriteId], (void*) 0x83BF554);
	gTasks[taskId].data[15] = spriteId;
	gTasks[taskId].func = (void*) (0x80784D8 | 1);
}


//Grav Apple//

//Causes an object to fall from the sky.
//arg 0: initial x pixel offset
//arg 1: initial y pixel offset
//arg 2: speed
//arg 3: anim bank
static void SpriteCB_FallingObjectStep(struct Sprite *sprite);
void SpriteCB_FallingObject(struct Sprite *sprite)
{
	u8 target = LoadBattleAnimTarget(3);

	if (IS_SINGLE_BATTLE
	&& (gBattleAnimArgs[3] == ANIM_ATK_PARTNER || gBattleAnimArgs[3] == ANIM_DEF_PARTNER)) //These targets don't exist and LoadBattleAnimTarget will treat as user
		DestroyAnimSprite(sprite);
	else if (!IsBattlerSpriteVisible(target))
		DestroyAnimSprite(sprite);
	else
	{
		sprite->pos1.x = GetBattlerSpriteCoord2(target, BATTLER_COORD_X);
		sprite->pos1.y = GetBattlerSpriteCoord2(target, BATTLER_COORD_Y);

		sprite->pos2.x = gBattleAnimArgs[0];
		sprite->pos1.y = gBattleAnimArgs[1];
		sprite->pos2.y = -gBattleAnimArgs[1];

		if (SIDE(gBattleAnimTarget) == B_SIDE_PLAYER)
		{
			sprite->pos1.y += 45;
			sprite->pos2.y -= 45;
		}

		sprite->data[3] = gBattleAnimArgs[2]; //Speed
		sprite->callback = SpriteCB_FallingObjectStep;
		StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
	}
}

static void SpriteCB_FallingObjectStep(struct Sprite *sprite)
{
	switch (sprite->data[0])
	{
		case 0:
			sprite->pos2.y += sprite->data[3];
			if (sprite->pos2.y >= 0)
			{
				sprite->pos2.y = 0;
				sprite->data[0]++;
			}
			break;
		case 1:
			if (++sprite->data[1] > 0)
			{
				sprite->data[1] = 0;
				sprite->invisible ^= 1;
				if (++sprite->data[2] == 10)
					SetCallbackToStoredInData6(sprite);
			}
			break;
	}
}

//Causes an object to fall from the sky.
//arg 0: initial x pixel offset
//arg 1: initial y pixel offset
//arg 2: speed
//arg 3: anim bank
static void SpriteCB_FallingObjectPlayAnimOnEndStep(struct Sprite *sprite);
void SpriteCB_FallingObjectPlayAnimOnEnd(struct Sprite *sprite)
{
	u8 target = LoadBattleAnimTarget(3);

	if (!IsBattlerSpriteVisible(target))
		DestroyAnimSprite(sprite);
	else
	{
		sprite->pos1.x = GetBattlerSpriteCoord2(target, BATTLER_COORD_X);
		sprite->pos1.y = GetBattlerSpriteCoord2(target, BATTLER_COORD_Y);

		sprite->pos2.x = gBattleAnimArgs[0];
		sprite->pos1.y = gBattleAnimArgs[1];
		sprite->pos2.y = -gBattleAnimArgs[1];

		if (SIDE(gBattleAnimTarget) == B_SIDE_PLAYER)
		{
			sprite->pos1.y += 45;
			sprite->pos2.y -= 45;
		}

		sprite->data[3] = gBattleAnimArgs[2]; //Speed
		sprite->callback = SpriteCB_FallingObjectPlayAnimOnEndStep;
	}
}

static void SpriteCB_FallingObjectPlayAnimOnEndStep(struct Sprite *sprite)
{
	switch (sprite->data[0])
	{
		case 0:
			sprite->pos2.y += sprite->data[3];
			if (sprite->pos2.y >= 0)
			{
				sprite->pos2.y = 0;
				sprite->data[0]++;
			}
			break;
		case 1:
			if (++sprite->data[1] > 0)
			{
				StartSpriteAnim(sprite, 1);
				sprite->callback = RunStoredCallbackWhenAnimEnds;
				StoreSpriteCallbackInData6(sprite, DestroySpriteAndMatrix);
			}
			break;
	}
}

static void FallingObjectOnSideCentre(struct Sprite* sprite, u8 side)
{
	sprite->pos1.x = GetProperCentredCoord(GetBattlerAtPosition(side), BATTLER_COORD_X);
	sprite->pos1.y = GetProperCentredCoord(GetBattlerAtPosition(side), BATTLER_COORD_Y);

	sprite->pos2.x = gBattleAnimArgs[0];
	sprite->pos1.y = gBattleAnimArgs[1];
	sprite->pos2.y = -gBattleAnimArgs[1];

	if (side == B_SIDE_PLAYER)
	{
		sprite->pos1.y += 45;
		sprite->pos2.y -= 45;
	}

	sprite->data[3] = gBattleAnimArgs[2]; //Speed
	sprite->callback = SpriteCB_FallingObjectStep;
	StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
}

//Causes an object to fall from the sky on the centre of the side.
//arg 0: initial x pixel offset
//arg 1: initial y pixel offset
//arg 2: speed
void SpriteCB_FallingObjectOnTargetCentre(struct Sprite *sprite)
{
	FallingObjectOnSideCentre(sprite, SIDE(gBattleAnimTarget));
}

//Causes an object to fall from the sky on the centre of the side.
//arg 0: initial x pixel offset
//arg 1: initial y pixel offset
//arg 2: speed
//arg 3: side
void SpriteCB_FallingObjectOnSideCentre(struct Sprite *sprite)
{
	FallingObjectOnSideCentre(sprite, gBattleAnimArgs[3]);
}


//Jaw Lock

//Creates a jaw that bites down and locks on the target.
//args: Idk same as bite and crunch
//arg 6: Time to hold bite for.
static void SpriteCB_LockingJawStep(struct Sprite *sprite);
static void SpriteCB_LockingJawFinish(struct Sprite *sprite);
void SpriteCB_LockingJaw(struct Sprite *sprite)
{
	sprite->pos1.x += gBattleAnimArgs[0];
	sprite->pos1.y += gBattleAnimArgs[1];
	StartSpriteAffineAnim(sprite, gBattleAnimArgs[2]);
	sprite->data[0] = gBattleAnimArgs[3];
	sprite->data[1] = gBattleAnimArgs[4];
	sprite->data[2] = gBattleAnimArgs[5];
	sprite->data[6] = -gBattleAnimArgs[6];
	sprite->callback = SpriteCB_LockingJawStep;
}

static void SpriteCB_LockingJawStep(struct Sprite *sprite)
{
	sprite->data[4] += sprite->data[0];
	sprite->data[5] += sprite->data[1];
	sprite->pos2.x = sprite->data[4] >> 8;
	sprite->pos2.y = sprite->data[5] >> 8;
	if (++sprite->data[3] == sprite->data[2])
		sprite->callback = SpriteCB_LockingJawFinish;
}

static void SpriteCB_LockingJawFinish(struct Sprite *sprite)
{
	if (--sprite->data[3] <= sprite->data[6])
		DestroySpriteAndMatrix(sprite);
}


//Lash Out//

//Creates the slam hit for LashOut
//arg 0: initial x pixel offset
//arg 1: initial y pixel offset
//arg 2: flip
void SpriteCB_LashOutStrike(struct Sprite* sprite)
{
	bool8 flip = SIDE(gBattleAnimTarget) == B_SIDE_PLAYER;
	
	if (gBattleAnimArgs[2])
		flip ^= 1;

	sprite->data[0] = 11;

	if (flip)
	{
		sprite->pos1.x -= gBattleAnimArgs[0];
		sprite->data[0] *= -1;
		StartSpriteAffineAnim(sprite, 1);
	}
	else
	{
		sprite->pos1.x += gBattleAnimArgs[0];
	}

	sprite->pos1.y += gBattleAnimArgs[1];
	sprite->data[1] = 192;
	sprite->callback = (void*) (0x80E42DC | 1); //AnimKnockOffStrike_Step
}


//Life Dew//
static const union AnimCmd sAnimCmdWaterDropletDrip[] =
{
	ANIMCMD_FRAME(0, 3),
	ANIMCMD_FRAME(16, 3),
	ANIMCMD_FRAME(32, 3),
	ANIMCMD_FRAME(48, 3),
	ANIMCMD_END,
};

static const union AnimCmd sAnimCmdWaterDropletSplash[] =
{
	ANIMCMD_FRAME(64, 3),
	ANIMCMD_FRAME(80, 3),
	ANIMCMD_FRAME(96, 3),
	ANIMCMD_FRAME(112, 10),
	ANIMCMD_END,
};

const union AnimCmd *const gAnimCmdWaterDroplet[] =
{
	sAnimCmdWaterDropletDrip,
	sAnimCmdWaterDropletSplash,
};

//Causes an object to fall from the sky.
//arg 0: initial x pixel offset
//arg 1: initial y pixel offset
//arg 2: anim bank
static void SpriteCB_WaterDropletDrip(struct Sprite *sprite);
static void SpriteCB_WaterDropletDrop(struct Sprite *sprite);
void SpriteCB_WaterDroplet(struct Sprite *sprite)
{
	u8 target = LoadBattleAnimTarget(2);

	if (!IsBattlerSpriteVisible(target))
		DestroyAnimSprite(sprite);
	else
	{
		InitSpritePosToGivenTarget(sprite, target);
		sprite->data[1] = sprite->pos1.y; //Target's y coord

		sprite->pos2.x = gBattleAnimArgs[0];
		sprite->pos2.y = 0;

		//Put droplet at the top of the screen
		sprite->pos1.y = 15;

		sprite->callback = SpriteCB_WaterDropletDrip;
	}
}

static void SpriteCB_WaterDropletDrip(struct Sprite *sprite)
{
	sprite->data[0] = 12;
	StartSpriteAnim(sprite, 0);
	sprite->callback = WaitAnimForDuration;
	StoreSpriteCallbackInData6(sprite, SpriteCB_WaterDropletDrop);
}

static void SpriteCB_WaterDropletDrop(struct Sprite *sprite)
{
	sprite->pos2.y += 4;
	if (sprite->pos2.y >= sprite->data[1])
	{
		sprite->data[0] = 19;
		StartSpriteAnim(sprite, 1);
		sprite->callback = WaitAnimForDuration;
		StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
	}
}


//Poltergeist//
static void SpriteCB_PoltergeistItem(struct Sprite* sprite);
static void SpriteCB_PoltergeistItem_Step0(struct Sprite* sprite);
static void SpriteCB_PoltergeistItem_Step1(struct Sprite* sprite);
static void SpriteCB_PoltergeistItem_Step2(struct Sprite* sprite);
static void SpriteCB_PoltergeistItem_Step3(struct Sprite* sprite);
static void SpriteCB_PoltergeistItem_Step4(struct Sprite* sprite);
static void SpriteCB_PoltergeistItem_Step5(struct Sprite* sprite);
static void SpriteCB_PoltergeistItem_Step6(struct Sprite* sprite);
static void CombineSpritePositions(struct Sprite* sprite);

static const union AffineAnimCmd sSpriteAffineAnim_PoltergeistItem[] =
{
	AFFINEANIMCMD_FRAME(-4, -4, 0, 16), //Pulsate slowly
	AFFINEANIMCMD_FRAME(4, 4, 0, 16),
	AFFINEANIMCMD_JUMP(0),
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_PoltergeistItem[] =
{
	sSpriteAffineAnim_PoltergeistItem,
};

void AnimTask_CreatePoltergeistItem(u8 taskId)
{
	u8 iconSpriteId = AddItemIconSprite(ITEM_TAG, ITEM_TAG, gLastUsedItem);

	if (iconSpriteId != MAX_SPRITES)
	{
		struct Sprite* sprite = &gSprites[iconSpriteId];

		sprite->pos1.x = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X_2);
		sprite->pos1.y = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET);
		sprite->oam.priority = 1;
		sprite->oam.affineMode = ST_OAM_AFFINE_NORMAL;
		sprite->affineAnims = gSpriteAffineAnimTable_PoltergeistItem;
		sprite->callback = SpriteCB_PoltergeistItem;

		CalcCenterToCornerVec(sprite, sprite->oam.shape, sprite->oam.size, sprite->oam.affineMode);
		InitSpriteAffineAnim(sprite);
		++gAnimVisualTaskCount;
	}

	DestroyAnimVisualTask(taskId);
}

//Creates the moving item for Poltergeist
static void SpriteCB_PoltergeistItem(struct Sprite* sprite)
{
	//Move sprite upwards
	if (++sprite->data[0] >= 10)
	{
		sprite->data[0] = 0;
		sprite->callback = SpriteCB_PoltergeistItem_Step0;
	}
	else
		sprite->pos1.y -= 2;
}

static void SpriteCB_PoltergeistItem_Step0(struct Sprite* sprite)
{
	//Move sprite in an upwards arc to indicate turning
	sprite->data[2] = sprite->pos1.x - 5;
	sprite->data[4] = sprite->pos1.y;
	sprite->data[0] = 20; //DURATION
	sprite->data[5] = -5; //AMPLITUDE
	InitAnimArcTranslation(sprite);
	sprite->callback = SpriteCB_PoltergeistItem_Step1;
}

static void SpriteCB_PoltergeistItem_Step1(struct Sprite* sprite)
{
	if (TranslateAnimHorizontalArc(sprite))
	{
		//Move sprite in an left arc to slightly right of the lower middle of the target
		CombineSpritePositions(sprite); //Clear movement offset
		sprite->data[2] = sprite->pos1.x + 20; //TARGET_X_PIXEL_OFFSET
		sprite->data[4] = sprite->pos1.y + 35; //TARGET_Y_PIXEL_OFFSET
		sprite->data[0] = 40; //DURATION
		sprite->data[5] = -30; //AMPLITUDE
		InitAnimArcTranslation(sprite);
		sprite->callback = SpriteCB_PoltergeistItem_Step2;
	}
}

static void SpriteCB_PoltergeistItem_Step2(struct Sprite* sprite)
{
	if (TranslateAnimVerticalArc(sprite))
	{
		//Move sprite in a small arc like its turning
		CombineSpritePositions(sprite); //Clear movement offset
		sprite->data[2] = sprite->pos1.x; //TARGET_X_PIXEL_OFFSET
		sprite->data[4] = sprite->pos1.y - 5; //TARGET_Y_PIXEL_OFFSET
		sprite->data[0] = 20; //DURATION
		sprite->data[5] = 5; //AMPLITUDE
		InitAnimArcTranslation(sprite);
		sprite->callback = SpriteCB_PoltergeistItem_Step3;
	}
}

static void SpriteCB_PoltergeistItem_Step3(struct Sprite* sprite)
{
	if (TranslateAnimVerticalArc(sprite))
	{
		//Move sprite in a line to the top left of the target
		CombineSpritePositions(sprite); //Clear movement offset
		sprite->data[0] = 25; //Speed delay
		sprite->data[2] = sprite->pos1.x - 40;
		sprite->data[4] = sprite->pos1.y - 35;
		sprite->callback = StartAnimLinearTranslation;
		StoreSpriteCallbackInData6(sprite, SpriteCB_PoltergeistItem_Step4);
	}
}

static void SpriteCB_PoltergeistItem_Step4(struct Sprite* sprite)
{
	//Move sprite in a upwards arc across the top of the target
	CombineSpritePositions(sprite); //Clear movement offset
	sprite->data[2] = sprite->pos1.x + 60; //TARGET_X_PIXEL_OFFSET
	sprite->data[4] = sprite->pos1.y; //TARGET_Y_PIXEL_OFFSET
	sprite->data[0] = 25; //DURATION
	sprite->data[5] = -10; //AMPLITUDE
	InitAnimArcTranslation(sprite);
	sprite->callback = SpriteCB_PoltergeistItem_Step5;
}

static void SpriteCB_PoltergeistItem_Step5(struct Sprite* sprite)
{
	if (TranslateAnimHorizontalArc(sprite))
	{
		//Move sprite in a right arc to attack the target
		CombineSpritePositions(sprite); //Clear movement offset
		sprite->data[2] = sprite->pos1.x - 40; //TARGET_X_PIXEL_OFFSET
		sprite->data[4] = sprite->pos1.y + 25; //TARGET_Y_PIXEL_OFFSET
		sprite->data[0] = 15; //DURATION
		sprite->data[5] = 20; //AMPLITUDE
		InitAnimArcTranslation(sprite);
		sprite->callback = SpriteCB_PoltergeistItem_Step6;
	}
}

static void SpriteCB_PoltergeistItem_Step6(struct Sprite* sprite)
{
	if (TranslateAnimVerticalArc(sprite))
		DestroyAnimSprite(sprite);
}

static void CombineSpritePositions(struct Sprite* sprite)
{
	sprite->pos1.x += sprite->pos2.x; //Combine positions
	sprite->pos1.y += sprite->pos2.y;
	sprite->pos2.x = 0; //Reset offsets
	sprite->pos2.y = 0;
}


//Pyro Ball//
const struct OamData sPyroBallRockOAM =
{
	.affineMode = ST_OAM_AFFINE_OFF,
	.objMode = ST_OAM_OBJ_NORMAL,
	.shape = SPRITE_SHAPE(16x16),
	.size = SPRITE_SIZE(16x16),
	.priority = 1, //Above sprites
};

const struct OamData sPyroBallFlamesOAM =
{
	.affineMode = ST_OAM_AFFINE_OFF,
	.objMode = ST_OAM_OBJ_NORMAL,
	.shape = SPRITE_SHAPE(32x32),
	.size = SPRITE_SIZE(32x32),
	.priority = 1, //Above sprites
};

#define sVerticalTime sprite->data[0]
#define sHorizontalTime sprite->data[1]
#define sMovingBackHorizontally sprite->data[2]
//Creates a rock that bounces between the attacker's feet.
//arg 0: initial x pixel offset
//arg 1: initial y pixel offset
//arg 2: Ignore horizontal motion if TRUE. Only bounce once.
static void InitSpritePositionForPyroBall(struct Sprite* sprite);
static void SpriteCB_PyroBallRockBounceStep(struct Sprite* sprite);
void SpriteCB_PyroBallRockBounce(struct Sprite* sprite)
{
	InitSpritePositionForPyroBall(sprite);
	sprite->callback = SpriteCB_PyroBallRockBounceStep;
}

static void InitSpritePositionForPyroBall(struct Sprite* sprite)
{
	InitSpritePosToAnimAttacker(sprite, 0);
	sprite->pos1.y += 20; //Move closer to attacker's feet

	if (SIDE(gBattleAnimAttacker) == B_SIDE_PLAYER)
		sprite->pos1.y += 20; //Move below the text box
}

static void SpriteCB_PyroBallRockBounceStep(struct Sprite* sprite)
{
	if (sprite->pos2.y > 0) //Rock returned back down
	{
		if (gBattleAnimArgs[2] || sMovingBackHorizontally) //Finished moving from left to right
			DestroyAnimSprite(sprite);
		else
		{
			sVerticalTime = 0;
			sMovingBackHorizontally = TRUE;
		}
	}

	s8 initialVerticalVelocity = -6;
	s8 initialHorizontalVelocity = -1; //Starts by moving right to left

	//vi = -6
	//g = 0.25 (gravity not negative b/c go up to go closer to 0)
	//h = vi*t + 0.5g*t^2
	//t = sVerticalTime
	sprite->pos2.y = (initialVerticalVelocity * sVerticalTime + (1 * sVerticalTime * sVerticalTime) / 4);

	//x = vi*t
	//vi = sprite->data[2]
	//t = sHorizontalTime
	if (!gBattleAnimArgs[2])
		sprite->pos2.x = (initialHorizontalVelocity * sHorizontalTime);

	sVerticalTime++;

	if (sMovingBackHorizontally)
		sHorizontalTime--; //Move left to right
	else
		sHorizontalTime++; //Move right to left
}
#undef sVerticalTime
#undef sHorizontalTime
#undef sMovingBackHorizontally

//Launches a projectile from the attacker's feet at the target.
//arg 0: initial x pixel offset
//arg 1: initial y pixel offset
//arg 2: target x pixel offset
//arg 3: target y pixel offset
//arg 4: duration
//arg 5: wave amplitude
void SpriteCB_PyroBallLaunch(struct Sprite* sprite)
{
	InitSpritePositionForPyroBall(sprite);

	if (SIDE(gBattleAnimAttacker) == B_SIDE_OPPONENT)
		gBattleAnimArgs[2] = -gBattleAnimArgs[2];

	sprite->data[0] = gBattleAnimArgs[4];
	sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X_2) + gBattleAnimArgs[2]; //Target X
	sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET) + gBattleAnimArgs[3]; //Target Y
	sprite->data[5] = gBattleAnimArgs[5];
	InitAnimArcTranslation(sprite);

	sprite->callback = SpriteCB_AnimMissileArcStep;
}


//Shell Side Arm//
static const union AffineAnimCmd sSpriteAffineAnim_ShellSideArmOpponent[] =
{
	AFFINEANIMCMD_FRAME(0, 0, -64, 1), //Rotate right 90 degrees
	AFFINEANIMCMD_END
};

static const union AffineAnimCmd sSpriteAffineAnim_ShellSideArmPlayer[] =
{
	AFFINEANIMCMD_FRAME(0, 0, 64, 1), //Rotate left 90 degrees
	AFFINEANIMCMD_END
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_ShellSideArm[] =
{
	sSpriteAffineAnim_ShellSideArmOpponent,
	sSpriteAffineAnim_ShellSideArmPlayer,
};

void AnimTask_GetShellSideArmSplit(u8 taskId)
{
	gBattleAnimArgs[0] = gNewBS->shellSideArmSplit[gBattleAnimAttacker][gBattleAnimTarget];
	DestroyAnimVisualTask(taskId);
}

//Moves the shell horizontally for Shell Side Arm
//args: Same as Rolling Kick
void SpriteCB_ShellSideArmSmash(struct Sprite *sprite)
{
    if (PARTNER(gBattleAnimAttacker) == gBattleAnimTarget && GetBattlerPosition(gBattleAnimTarget) < B_POSITION_PLAYER_RIGHT)
        gBattleAnimArgs[0] *= -1;

    InitSpritePosToAnimTarget(sprite, TRUE);

    if (SIDE(gBattleAnimTarget) == B_SIDE_PLAYER)
	{
        gBattleAnimArgs[2] = -gBattleAnimArgs[2];
		StartSpriteAffineAnim(sprite, 1);
	}

    sprite->data[0] = gBattleAnimArgs[3];
    sprite->data[1] = sprite->pos1.x;
    sprite->data[2] = sprite->pos1.x + gBattleAnimArgs[2];
    sprite->data[3] = sprite->pos1.y;
    sprite->data[4] = sprite->pos1.y;
    InitAnimLinearTranslation(sprite);
    sprite->data[5] = gBattleAnimArgs[5];
    sprite->data[6] = gBattleAnimArgs[4];
    sprite->data[7] = 0;
    sprite->callback = (void*) (0x80B0CB4 | 1);
}

//Chooses the sludge animation and launches it towards the target for Shell Side Arm
//args: Same as Shadow Ball
void SpriteCB_ShellSideArmBlast(struct Sprite* sprite)
{
	StartSpriteAnim(sprite, 2);
	sprite->callback = (void*) (0x80B563C | 1); //AnimShadowBall
}


//Skitter Smack//

//Creates a sprite that comes in behind the opposing Pokemon and then runs away in the direction it came
void AnimTask_SkitterBehindOpposingMon(u8 taskId)
{
	u8 spriteId, spriteId2;
	u32 personality;
	u32 otId;
	u16 species;
	u8 subpriority;
	bool8 isBackPic;
	s16 x;
	struct Pokemon* mon;

	switch (gTasks[taskId].data[0])
	{
	case 0:
		spriteId = GetAnimBattlerSpriteId(ANIM_ATTACKER);
		gTasks[taskId].data[1] += 0x800;
		if (SIDE(gBattleAnimTarget) == B_SIDE_OPPONENT)
			gSprites[spriteId].pos2.x += (gTasks[taskId].data[1] >> 8);
		else
			gSprites[spriteId].pos2.x -= (gTasks[taskId].data[1] >> 8);

		gTasks[taskId].data[1] &= 0xFF;
		x = gSprites[spriteId].pos1.x + gSprites[spriteId].pos2.x;
		if ((u16)(x + 32) > 304)
		{
			gTasks[taskId].data[1] = 0;
			gTasks[taskId].data[0]++;
		}
		break;
	case 1:
		mon = GetIllusionPartyData(gBattleAnimAttacker);
		personality = GetMonData(mon, MON_DATA_PERSONALITY, NULL);
		otId = GetMonData(mon, MON_DATA_OT_ID, NULL);
		if (gBattleSpritesDataPtr->bankData[gBattleAnimAttacker].transformSpecies == SPECIES_NONE)
			species = GetMonData(mon, MON_DATA_SPECIES, NULL);
		else
			species = gBattleSpritesDataPtr->bankData[gBattleAnimAttacker].transformSpecies;

		if (SIDE(gBattleAnimTarget) == B_SIDE_OPPONENT)
		{
			subpriority = gSprites[GetAnimBattlerSpriteId(ANIM_TARGET)].subpriority + 1;
			isBackPic = FALSE;
			x = 272;
		}
		else
		{
			subpriority = gSprites[GetAnimBattlerSpriteId(ANIM_TARGET)].subpriority - 1;
			isBackPic = TRUE;
			x = -32;
		}

		spriteId2 = CreateMonPicBattleAnim(species, isBackPic, 0, x, GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y), subpriority, personality, otId, gBattleAnimAttacker, 0);
		gTasks[taskId].data[15] = spriteId2;
		gTasks[taskId].data[0]++;
		break;
	case 2:
		spriteId2 = gTasks[taskId].data[15];
		gTasks[taskId].data[1] += 0x200;
		if (SIDE(gBattleAnimTarget) == B_SIDE_OPPONENT)
			gSprites[spriteId2].pos2.x -= (gTasks[taskId].data[1] >> 8);
		else
			gSprites[spriteId2].pos2.x += (gTasks[taskId].data[1] >> 8);

		gTasks[taskId].data[1] &= 0xFF;
		x = gSprites[spriteId2].pos1.x + gSprites[spriteId2].pos2.x;
		if (gTasks[taskId].data[14] == 0)
		{
			if (SIDE(gBattleAnimTarget) == B_SIDE_OPPONENT)
			{
				if (x < GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X))
				{
					gTasks[taskId].data[0]++;
					gTasks[taskId].data[14]++;
					gBattleAnimArgs[7] = 0xFFFF;
				}
			}
			else
			{
				if (x > GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X))
				{
					gTasks[taskId].data[0]++;
					gTasks[taskId].data[14]++;
					gBattleAnimArgs[7] = 0xFFFF;
				}
			}
		}
		break;
	case 3:
		spriteId2 = gTasks[taskId].data[15];
		gTasks[taskId].data[1] += 0x800;
		if (SIDE(gBattleAnimTarget) == B_SIDE_OPPONENT)
			gSprites[spriteId2].pos2.x += (gTasks[taskId].data[1] >> 8);
		else
			gSprites[spriteId2].pos2.x -= (gTasks[taskId].data[1] >> 8);

		gTasks[taskId].data[1] &= 0xFF;
		x = gSprites[spriteId2].pos1.x + gSprites[spriteId2].pos2.x;
		if (gTasks[taskId].data[14] == 0)
		{
			if (SIDE(gBattleAnimTarget) == B_SIDE_OPPONENT)
			{
				if (x < GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X))
				{
					gTasks[taskId].data[14]++;
					gBattleAnimArgs[7] = 0xFFFF;
				}
			}
			else
			{
				if (x > GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X))
				{
					gTasks[taskId].data[14]++;
					gBattleAnimArgs[7] = 0xFFFF;
				}
			}
		}
		if ((u16)(x + 32) > 304)
		{
			gTasks[taskId].data[1] = 0;
			gTasks[taskId].data[0]++;
		}
		break;
	case 4:
		spriteId = GetAnimBattlerSpriteId(ANIM_ATTACKER);
		spriteId2 = gTasks[taskId].data[15];
		DestroySpriteAndFreeResources(&gSprites[spriteId2]);
		if (SIDE(gBattleAnimTarget) == B_SIDE_OPPONENT)
			gSprites[spriteId].pos2.x = 272 - gSprites[spriteId].pos1.x;
		else
			gSprites[spriteId].pos2.x = -gSprites[spriteId].pos1.x - 32;

		gTasks[taskId].data[0]++;
		break;
	case 5:
		spriteId = GetAnimBattlerSpriteId(ANIM_ATTACKER);
		gTasks[taskId].data[1] += 0x800;
		if (SIDE(gBattleAnimTarget) == B_SIDE_OPPONENT)
		{
			gSprites[spriteId].pos2.x -= (gTasks[taskId].data[1] >> 8);
			if (gSprites[spriteId].pos2.x + gSprites[spriteId].pos1.x <= GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X))
				gSprites[spriteId].pos2.x = 0;
		}
		else
		{
			gSprites[spriteId].pos2.x += (gTasks[taskId].data[1] >> 8);
			if (gSprites[spriteId].pos2.x + gSprites[spriteId].pos1.x >= GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X))
				gSprites[spriteId].pos2.x = 0;
		}

		gTasks[taskId].data[1] = (u8)gTasks[taskId].data[1];
		if (gSprites[spriteId].pos2.x == 0)
			DestroyAnimVisualTask(taskId);
		break;
	}
}


//Snipe Shot//
static const union AffineAnimCmd sSpriteAffineAnim_LargeSpikePointedRight[];
static const union AnimCmd sAnimCmdSnipeShot[] =
{
	ANIMCMD_FRAME(64, 4),
	ANIMCMD_FRAME(48, 4),
	ANIMCMD_FRAME(32, 4),
	ANIMCMD_FRAME(16, 4),
	ANIMCMD_FRAME(0, 4),
	ANIMCMD_END,
};

const union AnimCmd *const gAnimCmdTable_SnipeShot[] =
{
	sAnimCmdSnipeShot,
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_SnipeShot[] =
{
	sSpriteAffineAnim_LargeSpikePointedRight,
};


//Spirit Break//
static const union AffineAnimCmd sSpriteAffineAnim_SpiritBreakBall[] =
{
	AFFINEANIMCMD_FRAME(16, 16, 0, 0),
	AFFINEANIMCMD_FRAME(2, 2, 0, 50), //Grow slowly to half size
	AFFINEANIMCMD_END,
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_SpiritBreakBall[] =
{
	sSpriteAffineAnim_SpiritBreakBall,
};


//Steel Roller//

//Creates the moving Steel Wheel for Steel Roller
//arg 0: initial x pixel offset
//arg 1: initial y pixel offset
//arg 2: falling speed
//arg 3: horizontal distance
//arg 4: horizontal speed
static void SpriteCB_SteelRoller_Down(struct Sprite *sprite);
static void SpriteCB_SteelRoller_LeftRight(struct Sprite* sprite);
void SpriteCB_SteelRoller(struct Sprite* sprite)
{
	sprite->pos1.x = GetBattlerSpriteCoord2(gBattleAnimTarget, BATTLER_COORD_X);
	sprite->pos1.y = GetBattlerSpriteCoord2(gBattleAnimTarget, BATTLER_COORD_Y);

	sprite->pos2.x = gBattleAnimArgs[0];
	sprite->pos2.y += gBattleAnimArgs[1];
	
	sprite->data[4] = gBattleAnimArgs[3]; //Left/Right distance
	sprite->data[5] = gBattleAnimArgs[4]; //Left/Right speed

	sprite->data[3] = gBattleAnimArgs[2]; //Falling Speed
	sprite->callback = SpriteCB_SteelRoller_Down;
}

static void SpriteCB_SteelRoller_Down(struct Sprite *sprite)
{
	sprite->pos2.y += sprite->data[3];
	if (sprite->pos2.y >= 0)
	{
		sprite->pos2.y = 0;
		sprite->callback = SpriteCB_SteelRoller_LeftRight;
	}
}

static void SpriteCB_SteelRoller_LeftRight(struct Sprite* sprite)
{
	sprite->data[0] = -sprite->data[4]; //Slice distance
	sprite->data[1] = sprite->data[5]; //Slice speed
	sprite->callback = SpriteCB_LeftRightSliceStep0;
}


//Surging Strikes//
static const union AnimCmd sAnimCmdSurgingStrike[] =
{
	//Only tthe first three frames of the animation
	ANIMCMD_FRAME(64, 4),
	ANIMCMD_FRAME(48, 4),
	ANIMCMD_FRAME(32, 4),
	ANIMCMD_END,
};

const union AnimCmd *const gAnimCmdTable_SurgingStrike[] =
{
	sAnimCmdSurgingStrike,
};

//Creates arc impacts for Surging Strikes
//arg 0: initial x pixel offset (from target)
//arg 1: initial y pixel offset (from target)
//arg 2: target x pixel offset (from target)
//arg 3: target y pixel offset (from target)
//arg 4: duration
//arg 5: wave amplitude
void SpriteCB_SurgingStrikes(struct Sprite* sprite)
{
	InitSpritePosToAnimTarget(sprite, TRUE);
	sprite->data[0] = gBattleAnimArgs[4];
	sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X_2) + gBattleAnimArgs[2]; //Target X
	sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET) + gBattleAnimArgs[3]; //Target Y
	sprite->data[5] = gBattleAnimArgs[5];
	InitAnimArcTranslation(sprite);
	sprite->callback = SpriteCB_AnimMissileArcStep;
}


//Terrain Pulse//
void AnimTask_GetTerrainType(u8 taskId)
{
	gBattleAnimArgs[0] = gTerrainType;
	DestroyAnimVisualTask(taskId);
}


///////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////PLA//////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

//Bitter Malice//

#define gBattleAnim_ScaryFaceGfx (void*) 0x8D24BCC
#define gBattleAnim_ScaryFacePal (void*) 0x8D24BA4
#define gBattleAnimBgTilemap_ScaryFaceOnOpponent (void*) 0x8E7F4AC
#define gBattleAnimBgTilemap_ScaryFaceOnPlayer (void*) 0x8E7f690
#define ScaryFace_Step (void*) (0x80AA024 | 1)
void AnimTask_ScaryFaceOnAttacker(u8 taskId)
{
	struct BattleAnimBgData animBg;

	SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT2_ALL | BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND);
	SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(0, 16));
	SetAnimBgAttribute(1, BG_ANIM_PRIORITY, 1);
	SetAnimBgAttribute(1, BG_ANIM_SCREEN_SIZE, 0);
	SetAnimBgAttribute(1, BG_ANIM_CHAR_BASE_BLOCK, 1);

	gBattle_BG1_X = 0;
	gBattle_BG1_Y = 0;

	if (IS_DOUBLE_BATTLE) //By default is in the centre of both Pokemon
	{
		switch (GetBattlerPosition(gBattleAnimAttacker))
		{
			case B_POSITION_PLAYER_LEFT:
				gBattle_BG1_X = 30;
				break;
			case B_POSITION_PLAYER_RIGHT:
				gBattle_BG1_X = -20;
				break;
			case B_POSITION_OPPONENT_LEFT:
				gBattle_BG1_X = -20;
				break;
			case B_POSITION_OPPONENT_RIGHT:
				gBattle_BG1_X = 25;
				break;
		}
	}

	SetGpuReg(REG_OFFSET_BG1HOFS, gBattle_BG1_X);
	SetGpuReg(REG_OFFSET_BG1VOFS, gBattle_BG1_Y);
	GetBattleAnimBg1Data(&animBg);

	if (SIDE(gBattleAnimAttacker) == B_SIDE_PLAYER)
		AnimLoadCompressedBgTilemap(animBg.bgId, gBattleAnimBgTilemap_ScaryFaceOnPlayer);
	else
		AnimLoadCompressedBgTilemap(animBg.bgId, gBattleAnimBgTilemap_ScaryFaceOnOpponent);

	AnimLoadCompressedBgGfx(animBg.bgId, gBattleAnim_ScaryFaceGfx, animBg.tilesOffset);
	LoadCompressedPalette(gBattleAnim_ScaryFacePal, animBg.paletteId * 16, 32);
	RelocateBattleBgPal(animBg.paletteId, animBg.bgTilemap, 0, 0);

	gTasks[taskId].func = ScaryFace_Step;
}


//Ceaseless Edge//

#define AnimFalseSwipeSliceStep3 (void*) (0x80A5A8C | 1)

//Creates slashes that can be flipped.
//arg 0: x pixel offset (from target)
//arg 1: y pixel offset (from target)
//arg 2: flip x
//arg 3: flip y
void SpriteCB_FlippableSlash(struct Sprite* sprite)
{
	InitSpritePosToAnimTarget(sprite, TRUE);

	if (gBattleAnimArgs[2]) //Flip X
		sprite->hFlip = TRUE;

	if (gBattleAnimArgs[3]) //Flip Y
		sprite->vFlip = TRUE;

	sprite->data[0] = 0;
	sprite->data[1] = 0;
	StoreSpriteCallbackInData6(sprite, AnimFalseSwipeSliceStep3);
	sprite->callback = RunStoredCallbackWhenAnimEnds;
}


//Mystical Power//

static const union AffineAnimCmd sSpriteAffineAnim_MysticalPowerRing[] =
{
	AFFINEANIMCMD_FRAME(16, 16, 0, 0), //Start small
	AFFINEANIMCMD_FRAME(16, 16, 0, 15), //Grow sprite
    AFFINEANIMCMD_END,
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_MysticalPowerRing[] =
{
	sSpriteAffineAnim_MysticalPowerRing,
};


//Power Shift//

//Launches the stat ball for Power Shift
//arg 0: X starting offset
//arg 1: Y starting offset
//arg 2: X finishing offset
//arg 3: Y finishing offset
//arg 4: Duration
//arg 5: Wave Amplitude
static void SpriteCB_PowerShiftBallStep(struct Sprite *sprite);
void SpriteCB_PowerShiftBall(struct Sprite* sprite)
{
	InitSpritePosToAnimAttacker(sprite, TRUE);

	if (SIDE(gBattleAnimAttacker) == B_SIDE_OPPONENT)
		gBattleAnimArgs[2] *= -1; //Flip X

	sprite->data[0] = gBattleAnimArgs[4]; //Duration
	sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X_2) + gBattleAnimArgs[2]; //Target X
	sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_Y_PIC_OFFSET) + gBattleAnimArgs[3]; //Target Y
	sprite->data[5] = gBattleAnimArgs[5]; //Wave Amplitude

	InitAnimArcTranslation(sprite);
	sprite->callback = SpriteCB_PowerShiftBallStep;
}

static void SpriteCB_PowerShiftBallStep(struct Sprite* sprite) //Also used by Triple Arrows
{
	if (TranslateAnimHorizontalArc(sprite))
		DestroySpriteAndMatrix(sprite);
}


//Stone Axe//

static const union AnimCmd sAnimCmdFlippedX[] =
{
	ANIMCMD_FRAME(0, 1, .hFlip = TRUE),
	ANIMCMD_END,
};

const union AnimCmd *const gAnimCmdTable_FlippedX[] =
{
	sAnimCmdFlippedX,
};

//Same args as AnimNeedleArmSpike
#define AnimNeedleArmSpikeStep (void*) (0x80A43A0 | 1)
void SpriteCB_SplinterIn(struct Sprite* sprite)
{
	s16 targetX, targetY, initialX, initialY, rot;
	u8 target = LoadBattleAnimTarget(0);

	if (gBattleAnimArgs[3] == 0 || !IsBattlerSpriteVisible(target))
	{
		DestroyAnimSprite(sprite);
		return;
	}

	//Aim the splinters towards the target
	sprite->data[0] = gBattleAnimArgs[3];
	targetX = sprite->data[5] = GetBattlerSpriteCoord(target, BATTLER_COORD_X_2);
	targetY = sprite->data[6] = GetBattlerSpriteCoord(target, BATTLER_COORD_Y_PIC_OFFSET);
	initialX = sprite->pos1.x = gBattleAnimArgs[1] + targetX;
	initialY = sprite->pos1.y = gBattleAnimArgs[2] + targetY;

	sprite->data[1] = initialX * 16;
	sprite->data[2] = initialY * 16;
	sprite->data[3] = (targetX - initialX) * 16 / gBattleAnimArgs[3];
	sprite->data[4] = (targetY - initialY) * 16 / gBattleAnimArgs[3];
	rot = ArcTan2Neg(targetX - initialX, targetY - initialY);
	rot += 0xC000;

	TrySetSpriteRotScale(sprite, 0, 0x100, 0x100, rot);
	sprite->callback = AnimNeedleArmSpikeStep;
}


//Triple Arrows//

//Delivers a rainbow kick for Triple Arrows
//arg 0: Initial x pixel offset
//arg 1: Initial y pixel offset
//arg 2: Duration
//arg 3: Wave Amplitude
void SpriteCB_TripleArrowKick(struct Sprite* sprite)
{
	InitSpritePosToAnimTarget(sprite, TRUE);
	StartSpriteAnim(sprite, 1); //Feet

	sprite->data[0] = gBattleAnimArgs[2];
	sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X_2); //Target X
	sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET); //Target Y
	sprite->data[5] = gBattleAnimArgs[3];

	InitAnimArcTranslation(sprite);
	sprite->callback = SpriteCB_PowerShiftBallStep; //Arc until complete
}


//Wildbolt Storm//

static const union AnimCmd sAnimCmdAnimatedSpark2[] =
{
	ANIMCMD_FRAME((16 * 16) / (8 * 8) * 0, 8),
	ANIMCMD_FRAME((16 * 16) / (8 * 8) * 1, 8),
	ANIMCMD_FRAME((16 * 16) / (8 * 8) * 2, 8),
	ANIMCMD_JUMP(0)
};

const union AnimCmd *const gAnimCmdTable_AnimatedSpark2[] =
{
	sAnimCmdAnimatedSpark2,
};


///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////Z-Moves////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

//Bloom Doom//
bool8 IsAnimMoveBloomDoom(void)
{
	return sAnimMoveIndex == MOVE_BLOOM_DOOM_P || sAnimMoveIndex == MOVE_BLOOM_DOOM_S;
}


//Breakneck Blitz//
const struct OamData sGrowingSuperpowerOAM =
{
	.affineMode = ST_OAM_AFFINE_DOUBLE,
	.objMode = ST_OAM_OBJ_NORMAL,
	.shape = SPRITE_SHAPE(64x64),
	.size = SPRITE_SIZE(64x64),
	.priority = 2,
};

static const union AffineAnimCmd sSpriteAffineAnim_GrowingSuperpowerEnemyAttack[] =
{
	AFFINEANIMCMD_FRAME(0, 0, 128, 1), //180 degree turn
	AFFINEANIMCMD_FRAME(0, 0, 0, 2), //Pause
	AFFINEANIMCMD_FRAME(16, 16, 0, 15), //Double in size
	AFFINEANIMCMD_END
};

static const union AffineAnimCmd sSpriteAffineAnim_GrowingSuperpowerPlayerAttack[] =
{
	AFFINEANIMCMD_FRAME(0, 0, 0, 2), //Pause
	AFFINEANIMCMD_FRAME(16, 16, 0, 15), //Double in size
	AFFINEANIMCMD_END
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_GrowingSuperpower[] =
{
	sSpriteAffineAnim_GrowingSuperpowerPlayerAttack,
	sSpriteAffineAnim_GrowingSuperpowerEnemyAttack,
};

void SpriteCB_GrowingSuperpower(struct Sprite *sprite)
{
	u8 battler;

	if (gBattleAnimArgs[0] == 0)
	{
		sprite->pos1.x = GetBattlerSpriteCoord(gBattleAnimAttacker, 2);
		sprite->pos1.y = GetBattlerSpriteCoord(gBattleAnimAttacker, 3);
		battler = gBattleAnimTarget;
		sprite->oam.priority = GetBattlerSpriteBGPriority(gBattleAnimAttacker);
	}
	else
	{
		battler = gBattleAnimAttacker;
		sprite->oam.priority = GetBattlerSpriteBGPriority(gBattleAnimTarget);
	}

	if (SIDE(gBattleAnimTarget) == B_SIDE_PLAYER)
		StartSpriteAffineAnim(sprite, 1);

	sprite->data[0] = 16;
	sprite->data[1] = sprite->pos1.x;
	sprite->data[2] = GetBattlerSpriteCoord(battler, 2);
	sprite->data[3] = sprite->pos1.y;
	sprite->data[4] = GetBattlerSpriteCoord(battler, 3);

	InitAnimLinearTranslation(sprite);
	StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
	sprite->callback = (void*) 0x807563D;
}


//Clangorous Soulblaze//

//Moves the rings for Clangorous Soulblaze
//arg 0: initial x offset
//arg 1: initial y offset
//arg 2: target x offset
//arg 3: target y offset
//arg 4: duration
//arg 5: lower 8 bits = location on attacking mon, upper 8 bits = location on target mon pick to target
void SpriteCB_TranslateAnimSpriteToTargetsCentre(struct Sprite* sprite)
{
	bool8 respectMonPicOffsets;
	u8 coordType;

	if (!(gBattleAnimArgs[5] & 0xFF00))
		respectMonPicOffsets = TRUE;
	else
		respectMonPicOffsets = FALSE;

	if (!(gBattleAnimArgs[5] & 0xFF))
		coordType = BATTLER_COORD_Y_PIC_OFFSET;
	else
		coordType = BATTLER_COORD_Y;

	InitSpritePosToAnimAttacker(sprite, respectMonPicOffsets);
	if (GetBattlerSide(gBattleAnimAttacker) != B_SIDE_PLAYER)
		gBattleAnimArgs[2] = -gBattleAnimArgs[2];

	if (SIDE(gBattleAnimAttacker) == SIDE(gBattleAnimTarget) || IS_SINGLE_BATTLE)
	{
		sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X_2);
		sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimTarget, coordType);
	}
	else
	{
		sprite->data[2] = (GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X_2) + GetBattlerSpriteCoord(PARTNER(gBattleAnimTarget), BATTLER_COORD_X_2)) / 2;
		sprite->data[4] = (GetBattlerSpriteCoord(gBattleAnimTarget, coordType) + GetBattlerSpriteCoord(PARTNER(gBattleAnimTarget), coordType)) / 2;
	}

	sprite->data[2] += gBattleAnimArgs[2];
	sprite->data[4] += gBattleAnimArgs[3];
	sprite->data[0] = gBattleAnimArgs[4];
	sprite->callback = StartAnimLinearTranslation;
	StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
}


//Continental Crush//
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


//Extreme Evoboost//

//Creates The Extreme Evoboost Circles
void SpriteCB_ExtremeEvoboostCircle(struct Sprite *sprite)
{
	InitSpritePosToAnimAttacker(sprite, FALSE);
	sprite->pos1.y += 20;
	sprite->data[1] = 191;
	sprite->callback = (void*) (0x80B190C | 1);
	sprite->callback(sprite);
}


//Genesis Supernova//

//Moves up the orbs up in Genesis Supernova
//arg 0: Initial X-Pos
//arg 1: Initial Y-Pos
//arg 2: Destination Y-Pos
//arg 3: Duration
void SpriteCB_GenesisSupernovaOrbUp(struct Sprite* sprite)
{
	InitSpritePosToAnimAttacker(sprite, TRUE);
	sprite->data[0] = gBattleAnimArgs[3]; //Duration
	sprite->data[2] = sprite->pos1.x; //Dest X - Don't move
	sprite->data[4] = gBattleAnimArgs[2]; //Dest Y
	StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
	sprite->callback = InitAndRunAnimFastLinearTranslation;
}


//Let's Snuggle Forever//

//Scales up the target mon sprite
//Used in Let's Snuggle Forever
//arg 0: Duration until size reset.
static void AnimTask_GrowStep(u8 taskId);
void AnimTask_GrowTarget(u8 taskId)
{
	u8 spriteId = GetAnimBattlerSpriteId(ANIM_TARGET);
	PrepareBattlerSpriteForRotScale(spriteId, ST_OAM_OBJ_BLEND);
	SetSpriteRotScale(spriteId, 208, 208, 0);
	gTasks[taskId].data[0] = gBattleAnimArgs[0];
	gTasks[taskId].func = AnimTask_GrowStep;
}

static void AnimTask_GrowStep(u8 taskId)
{
	if (--gTasks[taskId].data[0] == -1)
	{
		u8 spriteId = GetAnimBattlerSpriteId(ANIM_TARGET);
		ResetSpriteRotScale(spriteId);
		DestroyAnimVisualTask(taskId);
	}
}


//Light that Burns the Sky//
const struct OamData sGeyserOam =
{
	.affineMode = ST_OAM_AFFINE_OFF,
	.objMode = ST_OAM_OBJ_NORMAL,
	.shape = SPRITE_SHAPE(32x32),
	.size = SPRITE_SIZE(32x32),
	.priority = 1, //Above sprites
};

bool8 DoesMoveHaveGeyserOnTarget(void)
{
	return sAnimMoveIndex == MOVE_HEADLONGRUSH
		|| sAnimMoveIndex == MOVE_NEVER_ENDING_NIGHTMARE_P || sAnimMoveIndex == MOVE_NEVER_ENDING_NIGHTMARE_S
		|| sAnimMoveIndex == MOVE_DEVASTATING_DRAKE_P      || sAnimMoveIndex == MOVE_DEVASTATING_DRAKE_S
		|| sAnimMoveIndex == MOVE_GIGAVOLT_HAVOC_P         || sAnimMoveIndex == MOVE_GIGAVOLT_HAVOC_S
		|| sAnimMoveIndex == MOVE_GUARDIAN_OF_ALOLA
		|| sAnimMoveIndex == MOVE_LIGHT_THAT_BURNS_THE_SKY
		|| IsAnyMaxMove(sAnimMoveIndex);
}


//Oceanic Operretta//
bool8 IsAnimMoveOceanicOperretta(void)
{
	return sAnimMoveIndex == MOVE_OCEANIC_OPERETTA;
}


//Soul-Stealing-7-Star-Strike//
void SpriteCB_SoulStealingStar(struct Sprite *sprite)
{
	sprite->pos1.x += gBattleAnimArgs[0];
	sprite->pos1.y += gBattleAnimArgs[1];
	sprite->data[0] = gBattleAnimArgs[3];
	sprite->data[1] = gBattleAnimArgs[4];
	sprite->data[2] = gBattleAnimArgs[5];
	sprite->callback = (void*) (0x80B7C10 | 1); //AnimBite_Step1
}


//Splintered Stormshards//
void AnimTask_GetLycanrocForm(u8 taskId)
{
	#ifdef SPECIES_LYCANROC_N
	if (GetAnimAttackerSpecies() == SPECIES_LYCANROC_N)
		gBattleAnimArgs[0] = 1;
	else
	#endif
		gBattleAnimArgs[0] = 0;

	DestroyAnimVisualTask(taskId);
}


//Twinkle Tackle//
static const s8 sHomerunEnemyHorizontalMovement[] =
{
	3, 3, 3, 3,
	3, 3, 2, 2,
	1, 1, 1, 1,
	1, 1, 1, 1,
	0, 1, 0, 1,
	0, 1, 0, 0,
	1, 0, 0, 1,
	0, 0, 0, 1,
	0, 0, 0, 1,
};

static const s8 sHomerunEnemyVerticalMovement[] =
{
	-4, -4, -4, -4,
	-4, -3, -3, -2,
	-2, -1, -1, -1,
	-1, -1, -1, -1,
	 0, -1,  0, -1,
	 0, -1,  0,  0,
	 0,  0, -1,  0,
	 0, -1,  0,  0,
	-1,  0,  0,  0,
};

//Creates a twinkle at the location the target was knocked too in Twinkle Tackle
void SpriteCB_TwinkleOnBank(struct Sprite *sprite)
{
	struct Sprite* monSprite;

	if (gBattleAnimArgs[2] == ANIM_TARGET)
		monSprite = &gSprites[GetAnimBattlerSpriteId(ANIM_TARGET)];
	else
		monSprite = &gSprites[GetAnimBattlerSpriteId(ANIM_ATTACKER)];

	sprite->pos1 = monSprite->pos1;
	sprite->pos2 = monSprite->pos2;
	sprite->callback = RunStoredCallbackWhenAnimEnds;
	StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
}

#define tSpriteId data[0]
#define tTimer data[1]
#define tInitialXPos data[2]
#define tInitialYPos data[3]
#define tSide data[4]
#define tAnimLengthTime data[5]
void AnimTask_TwinkleTackleLaunchStep(u8 taskId)
{
	u16 rotation;
	s16 xScale, yScale;
	struct Task* task = &gTasks[taskId];
	struct Sprite* sprite = &gSprites[task->tSpriteId];

	if (task->tTimer > task->tAnimLengthTime)
	{
		if (task->tTimer > task->tAnimLengthTime + 5) //Wait an extra few frames so the glint can be placed on the target
		{
			sprite->pos1.x = task->tInitialXPos;
			sprite->pos1.y = task->tInitialYPos;
			ResetSpriteRotScale(task->tSpriteId);
			DestroyAnimVisualTask(taskId);
		}
		else
			++task->tTimer;
		return;
	}
	else if ((u16) task->tTimer < NELEMS(sHomerunEnemyHorizontalMovement))
	{
		s8 movement = sHomerunEnemyHorizontalMovement[task->tTimer];
		if (task->tSide == B_SIDE_PLAYER)
			movement *= -1;
		sprite->pos1.x += movement;

		movement = sHomerunEnemyVerticalMovement[task->tTimer];
		if (task->tSide == B_SIDE_PLAYER)
			movement *= -1;
		sprite->pos1.y += movement;
	}

	xScale = 0x180;
	yScale = 0x180;
	rotation = (task->tTimer << 4) + (task->tTimer << 3);

	xScale += rotation;
	yScale += rotation;
	rotation <<= 7;

	if (task->tSide == B_SIDE_OPPONENT)
		rotation *= -1;

	SetSpriteRotScale(task->tSpriteId, xScale, yScale, rotation);

	if (++task->tTimer > task->tAnimLengthTime)
		sprite->invisible = TRUE;
}

//Launches the target in Twinkle Tackle
//arg 0: Anim time
void AnimTask_TwinkleTackleLaunch(u8 taskId)
{
	struct Task* task = &gTasks[taskId];

	task->tSpriteId = GetAnimBattlerSpriteId(ANIM_TARGET);
	task->tSide = SIDE(gBattleAnimTarget);
	task->tAnimLengthTime = gBattleAnimArgs[0];
	task->tInitialXPos = gSprites[task->tSpriteId].pos1.x;
	task->tInitialYPos = gSprites[task->tSpriteId].pos1.y;
	task->tTimer = 0;
	task->func = AnimTask_TwinkleTackleLaunchStep;

	PrepareBattlerSpriteForRotScale(task->tSpriteId, ST_OAM_OBJ_NORMAL);
}
#undef tSpriteId
#undef tTimer
#undef tInitialXPos
#undef tInitialYPos
#undef tSide
#undef tAnimLengthTime


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////Max Moves///////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

//Max Flare//
static const union AffineAnimCmd sSpriteAffineAnim_MaxFlareBall[] =
{
	AFFINEANIMCMD_FRAME(4, 4, 0, 32), //1.5x Size
	//Fluctuate Size
	AFFINEANIMCMD_FRAME(-4, -4, 0, 32), //Normal size
	AFFINEANIMCMD_FRAME(0, 0, 0, 8), //Pause
	AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sSpriteAffineAnim_MaxFlareBalLooped[] =
{
	AFFINEANIMCMD_FRAME(8, 8, 0, 32), //Get to double size
	AFFINEANIMCMD_FRAME(-4, -4, 0, 8),
	AFFINEANIMCMD_FRAME(4, 4, 0, 8),
	AFFINEANIMCMD_JUMP(2),
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_MaxFlareBall[] =
{
	sSpriteAffineAnim_MaxFlareBall,
	sSpriteAffineAnim_MaxFlareBalLooped,
};

//Creates a fireball on the attacker for Max Flare
//arg 0: X pixel offset
//arg 1: Y pixel offset
void SpriteCB_MaxFlareBallOnAttacker(struct Sprite* sprite)
{
	InitSpritePosToAnimAttacker(sprite, TRUE);
	StoreSpriteCallbackInData6(sprite, DestroySpriteAndMatrix);
	sprite->callback = RunStoredCallbackWhenAffineAnimEnds;
}

//Launches the fireball for Max Flare
//arg 0: X pixel offset
//arg 1: Y pixel offset
//arg 2: Duration
//arg 3: Wave Amplitude
static void SpriteCB_MaxFlareBallStep(struct Sprite *sprite);
void SpriteCB_MaxFlareBall(struct Sprite* sprite)
{
	InitSpritePosToAnimAttacker(sprite, TRUE);
	StartSpriteAffineAnim(sprite, 1);

	sprite->data[0] = gBattleAnimArgs[2];
	sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X_2); //Target X
	sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET); //Target Y
	sprite->data[5] = gBattleAnimArgs[3];

	InitAnimArcTranslation(sprite);
	sprite->callback = SpriteCB_MaxFlareBallStep;
}

static void SpriteCB_MaxFlareBallStep(struct Sprite *sprite)
{
	sprite->invisible = FALSE;

	if (TranslateAnimHorizontalArc(sprite))
	{
		DestroyAnimSprite(sprite);
	}
	else
	{
		s16 tempData[8];
		s16* data = sprite->data;
		u16 x1 = sprite->pos1.x;
		s16 x2 = sprite->pos2.x;
		u16 y1 = sprite->pos1.y;
		s16 y2 = sprite->pos2.y;
		int i;

		for (i = 0; i < 8; i++)
			tempData[i] = data[i];

		x2 += x1;
		y2 += y1;

		if (!TranslateAnimHorizontalArc(sprite))
		{
			for (i = 0; i < 8; i++)
				data[i] = tempData[i];
		}
	}
}


//Max Flutterby//
static const union AffineAnimCmd sSpriteAffineAnim_FlutterbyPulsate[] =
{
	AFFINEANIMCMD_FRAME(16, 16, 0, 4),
	AFFINEANIMCMD_FRAME(-16, -16, 0, 4),
	AFFINEANIMCMD_JUMP(0),
};

static const union AffineAnimCmd sSpriteAffineAnim_FlutterbyGrow[] =
{
	AFFINEANIMCMD_FRAME(8, 8, 0, 16), //Double in size
	AFFINEANIMCMD_END,
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_Flutterby[] =
{
	sSpriteAffineAnim_FlutterbyPulsate,
	sSpriteAffineAnim_FlutterbyGrow,
};

static const union AffineAnimCmd sSpriteAffineAnim_MushroomCloud[] =
{
	AFFINEANIMCMD_FRAME(16, 16, 0, 0),
	AFFINEANIMCMD_FRAME(32, 32, 0, 15), //Double in size
	AFFINEANIMCMD_FRAME(0, 0, 0, 48),
	AFFINEANIMCMD_FRAME(-32, -32, 0, 15), //Double in size
	AFFINEANIMCMD_END,
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_MushroomCloud[] =
{
	sSpriteAffineAnim_MushroomCloud,
};

//Moves the butterflies created in Max Flutterby
//arg 0: initial x pixel offset
//arg 1: initial y pixel offset
//arg 2: wave amplitude
static void SpriteCB_MaxFlutterbyStep1(struct Sprite* sprite);
static void SpriteCB_MaxFlutterbyStep2(struct Sprite* sprite);
void SpriteCB_MaxFlutterby(struct Sprite* sprite)
{
	InitSpritePosToAnimAttacker(sprite, FALSE);

	sprite->data[0] = 0x10; //Speed delay
	sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X_2); //Target X
	sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET); //Target Y
	sprite->data[5] = gBattleAnimArgs[2]; //Wave amplitude

	sprite->callback = SpriteCB_MaxFlutterbyStep1;
}

//The butterflies prepare to move towards the target
static void SpriteCB_MaxFlutterbyStep1(struct Sprite* sprite)
{
	if (!FuncIsActiveTask(AnimTask_DynamaxGrowthStep))
	{
		if (sAnimMoveIndex != MOVE_INFERNALPARADE)
			PlaySE(0x98); //Shadow Ball hit

		StartSpriteAffineAnim(sprite, 1);
		InitAnimArcTranslation(sprite);
		sprite->callback = SpriteCB_MaxFlutterbyStep2;
	}
}

//Destroys the butterflies when they reach the target
static void SpriteCB_MaxFlutterbyStep2(struct Sprite* sprite)
{
	sprite->invisible = FALSE;

	if (TranslateAnimHorizontalArc(sprite))
	{
		if (sAnimMoveIndex == MOVE_INFERNALPARADE)
			PlaySE(0x8A); //Fire

		DestroySpriteAndMatrix(sprite);
	}
}


//Max Hailstorm//
static const union AffineAnimCmd sSpriteAffineAnim_LargeHailRock[] =
{
	AFFINEANIMCMD_FRAME(256, 256, 0, 1), //Double sprite size
	AFFINEANIMCMD_END,
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_LargeHailRock[] =
{
	sSpriteAffineAnim_LargeHailRock,
};

static const union AffineAnimCmd sSpriteAffineAnim_EvenLargerHailRock[] =
{
	AFFINEANIMCMD_FRAME(256, 256, 0, 1), //Double sprite size
	AFFINEANIMCMD_FRAME(128, 128, 0, 1), //1.5x sprite size
	AFFINEANIMCMD_END,
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_EvenLargerHailRock[] =
{
	sSpriteAffineAnim_EvenLargerHailRock,
};

static const union AnimCmd sAnimCmdIceRock[] =
{
	ANIMCMD_FRAME(0, 3),
	ANIMCMD_FRAME(64, 3),
	ANIMCMD_FRAME(128, 3),
	ANIMCMD_FRAME(192, 3),
	ANIMCMD_FRAME(256, 3),
	ANIMCMD_END,
};

static const union AnimCmd sAnimCmdDoNothing[] =
{
	ANIMCMD_FRAME(0, 0),
	ANIMCMD_END,
};

const union AnimCmd* const gAnimCmdTable_IceRock[] =
{
	sAnimCmdDoNothing,
	sAnimCmdIceRock,
};

static const union AnimCmd sAnimCmdIceRockMulti[] =
{
	ANIMCMD_FRAME(0, 3),
	ANIMCMD_FRAME(64, 3),
	ANIMCMD_FRAME(128, 3),
	ANIMCMD_END,
};

const union AnimCmd* const gAnimCmdTable_IceRockMulti[] =
{
	sAnimCmdDoNothing,
	sAnimCmdIceRockMulti,
};

void AnimTask_IsHailstormRockLoaded(u8 taskId)
{
	gBattleAnimArgs[0] = (IndexOfSpriteTileTag(ANIM_TAG_ICE_ROCK) == 0xFF) ? 0 : 1;
	DestroyAnimVisualTask(taskId);
}


//Max Knuckle//
const struct OamData sTransparentFistOAM =
{
	.affineMode = ST_OAM_AFFINE_DOUBLE,
	.objMode = ST_OAM_OBJ_BLEND,
	.shape = SPRITE_SHAPE(32x32),
	.size = SPRITE_SIZE(32x32),
	.priority = 1, //Above sprites
};

static const union AffineAnimCmd sSpriteAffineAnim_GrowingFist[] =
{
	AFFINEANIMCMD_FRAME(4, 4, 0, 64), //Double in size
	AFFINEANIMCMD_END,
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_GrowingFist[] =
{
	sSpriteAffineAnim_GrowingFist,
};

//Creates a twinkle in the upper corner of the screen
void SpriteCB_SparkleInCorner(struct Sprite *sprite)
{
	if (SIDE(gBattleAnimAttacker) == B_SIDE_PLAYER)
	{
		sprite->pos1.x = 15;
	}
	else //Enemy side
	{
		sprite->pos1.x = 225;
	}

	sprite->pos1.y = 13;
	sprite->callback = RunStoredCallbackWhenAnimEnds;
	StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
}


//Max Mindstorm//

//Launches an object upwards
//arg 0: initial x pixel offset
//arg 1: initial y pixel offset
void SpriteCB_LaunchObjectUpwards(struct Sprite* sprite)
{
	InitSpritePosToAnimAttacker(sprite, FALSE);
	sprite->data[0] = gBattleAnimArgs[0] > 0 ? 1 : -1;
	sprite->callback = (void*) (0x80B8DE8 | 1); //AnimMudSportDirtRising
}


//Max Ooze//
const struct OamData gPoisonColumnOam =
{
	.affineMode = ST_OAM_AFFINE_DOUBLE,
	.objMode = ST_OAM_OBJ_NORMAL,
	.shape = SPRITE_SHAPE(32x64),
	.size = SPRITE_SIZE(32x64),
	.priority = 2, //Save level as sprites
};

const struct OamData gPoisonColumnOamPlayer =
{
	.affineMode = ST_OAM_AFFINE_DOUBLE,
	.objMode = ST_OAM_OBJ_NORMAL,
	.shape = SPRITE_SHAPE(32x64),
	.size = SPRITE_SIZE(32x64),
	.priority = 3, //Below sprites
};

static const union AnimCmd sAnimCmdPoisonColumn[] =
{
	ANIMCMD_FRAME(0, 6),
	ANIMCMD_FRAME(32, 3),
	ANIMCMD_FRAME(64, 3),
	ANIMCMD_FRAME(96, 3),
	ANIMCMD_FRAME(128, 3),
	ANIMCMD_FRAME(160, 3),
	ANIMCMD_FRAME(192, 3),
	ANIMCMD_JUMP(3),
};

const union AnimCmd *const gAnimCmdTable_PoisonColumn[] =
{
	sAnimCmdPoisonColumn,
};


//Max Overgrowth//
static const union AnimCmd sAnimCmdGrowingMushroom[] =
{
	ANIMCMD_FRAME(0, 3),
	ANIMCMD_FRAME(16, 3),
	ANIMCMD_FRAME(32, 3),
	ANIMCMD_FRAME(48, 3),
	ANIMCMD_END
};

const union AnimCmd *const gAnimCmdTable_GrowingMushroom[] =
{
	sAnimCmdGrowingMushroom,
};


//Max Phantasm//
const struct OamData sMaxPhantasmObjectsOam =
{
	.affineMode = ST_OAM_AFFINE_NORMAL,
	.objMode = ST_OAM_OBJ_NORMAL,
	.shape = SPRITE_SHAPE(32x32),
	.size = SPRITE_SIZE(32x32),
	.priority = 1, //Above sprites
};

static const union AffineAnimCmd sSpriteAffineAnim_GrowToNormalSize[] =
{
	AFFINEANIMCMD_FRAME(16, 16, 0, 0), //Start at 1px
	AFFINEANIMCMD_FRAME(8, 8, 0, 30),
	AFFINEANIMCMD_END,
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_GrowToNormalSize[] =
{
	sSpriteAffineAnim_GrowToNormalSize,
};

//Moves the objects created in Max Phantasm
//arg 0: initial x pixel offset
//arg 1: initial y pixel offset
//arg 2: dest x pixel offset
//arg 3: dest y pixel offset
//arg 4: wave amplitude
void SpriteCB_MaxPhantasmObjectStep1(struct Sprite* sprite);
void SpriteCB_MaxPhantasmObjectStep2(struct Sprite* sprite);
void SpriteCB_MaxPhantasmObjectStep3(struct Sprite* sprite);
void SpriteCB_MaxPhantasmObject(struct Sprite* sprite)
{
	InitSpritePosToAnimAttacker(sprite, FALSE);

	if (SIDE(gBattleAnimAttacker))
		gBattleAnimArgs[2] = -gBattleAnimArgs[2];

	sprite->data[0] = 0x20; //Speed delay
	sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X_2) + gBattleAnimArgs[2]; //Target X
	sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET) + gBattleAnimArgs[3]; //Target Y
	sprite->data[5] = gBattleAnimArgs[4]; //Wave amplitude

	sprite->callback = SpriteCB_MaxPhantasmObjectStep1;
}

//The objects prepare to move towards the target
void SpriteCB_MaxPhantasmObjectStep1(struct Sprite* sprite)
{
	if (!FuncIsActiveTask(AnimTask_DynamaxGrowthStep))
	{
		PlaySE(0x98); //Shadow Ball hit
		InitAnimArcTranslation(sprite);
		sprite->data[7] = 0;
		sprite->callback = SpriteCB_MaxPhantasmObjectStep2;
	}
}

//The objects move towards the target in an arg
void SpriteCB_MaxPhantasmObjectStep2(struct Sprite* sprite)
{
	sprite->invisible = FALSE;

	if (TranslateAnimHorizontalArc(sprite))
	{
		sprite->data[7] = 0;
		sprite->callback = SpriteCB_MaxPhantasmObjectStep3;
	}
}

//The objects count down, then converge on the target
void SpriteCB_MaxPhantasmObjectStep3(struct Sprite* sprite)
{
	if (sprite->data[7]++ >= 0x10) //Wait quarter of a second
	{
		PlaySE(0xCF); //Hard bam
		sprite->pos1.x += sprite->pos2.x; //Combine positions
		sprite->pos1.y += sprite->pos2.y;
		sprite->pos2.x = 0; //Reset offsets
		sprite->pos2.y = 0;

		sprite->data[0] = 0x8; //Speed delay
		sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X_2); //Converge on target
		sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET);
		sprite->callback = StartAnimLinearTranslation;
		StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
	}
}


//Max Quake//
bool8 DoesAnimMoveGetMaxRollout(void)
{
	return sAnimMoveIndex == MOVE_TECTONIC_RAGE_P || sAnimMoveIndex == MOVE_TECTONIC_RAGE_S
		|| sAnimMoveIndex == MOVE_MAX_QUAKE_P || sAnimMoveIndex == MOVE_MAX_QUAKE_S
		|| sAnimMoveIndex == MOVE_G_MAX_SANDBLAST_P || sAnimMoveIndex == MOVE_G_MAX_SANDBLAST_S;
}

void SpriteCB_RolloutExplosion(struct Sprite *sprite)
{
	if (TranslateAnimHorizontalArc(sprite))
	{
		//u8 taskId = FindTaskIdByFunc((void*) 0x80B4D01); //AnimTask_RolloutRocksStep
		//if (taskId != 0xFF && gTasks[taskId].data[11] > 0)
		//	gTasks[taskId].data[11]--;

		DestroySprite(sprite);
	}
}

//Creates explosions on the same path as Rollout rocks
//No args
static void AnimTask_RolloutPathExplosionsStep(u8 taskId);
static void CreateRolloutExplosionSprite(struct Task* task);
void AnimTask_RolloutPathExplosions(u8 taskId)
{
	int var5;
	u16 var0, var1, var2, var3, var4;
	struct Task *task;

	task = &gTasks[taskId];

	var0 = GetBattlerSpriteCoord(gBattleAnimAttacker, 2);
	var1 = GetBattlerSpriteCoord(gBattleAnimAttacker, 1) + 24;
	var2 = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X_2);
	var3 = GetBattlerSpriteCoord(gBattleAnimTarget, 1) + 24;

	if (BATTLE_PARTNER(gBattleAnimAttacker) == gBattleAnimTarget)
		var3 = var1;

	var4 = 4;
	task->data[8] = 48 - (var4 * 8);

	task->data[0] = 0;
	task->data[11] = 0;
	task->data[9] = 0;
	task->data[12] = 1;

	var5 = task->data[8];
	if (var5 < 0)
		var5 += 7;

	task->data[10] = (var5 >> 3) - 1;

	task->data[2] = var0 * 8;
	task->data[3] = var1 * 8;
	task->data[4] = ((var2 - var0) * 8) / task->data[8];
	task->data[5] = ((var3 - var1) * 8) / task->data[8];
	task->data[6] = 0;
	task->data[7] = 0;

	task->data[1] = var4;
	task->data[15] = GetAnimBattlerSpriteId(0);

	task->func = AnimTask_RolloutPathExplosionsStep;
}

static void AnimTask_RolloutPathExplosionsStep(u8 taskId)
{
	struct Task *task;

	task = &gTasks[taskId];

	switch (task->data[0]) {
		case 0:
			if (++task->data[9] == 10)
			{
				task->data[11] = 20;
				task->data[0]++;
			}
			break;
		case 1:
			if (--task->data[11] == 0)
				task->data[0]++;
			break;
		case 2:
			if (--task->data[9] == 0)
			{
				task->data[6] = 0;
				task->data[7] = 0;
				task->data[0]++;
			}
			break;
		case 3:
			task->data[2] += task->data[4];
			task->data[3] += task->data[5];
			if (++task->data[9] >= task->data[10])
			{
				task->data[9] = 0;
				CreateRolloutExplosionSprite(task);
				task->data[13] += task->data[14];
			}

			if (--task->data[8] == 0)
			{
				task->data[0]++;
			}
			break;
		case 4:
			DestroyAnimVisualTask(taskId);
			break;
	}
}

extern const struct SpriteTemplate gRolloutExplosionSpriteTemplate;
static void CreateRolloutExplosionSprite(struct Task* task)
{
	u8 spriteId;
	u16 x, y;
	const struct SpriteTemplate* spriteTemplate = &gRolloutExplosionSpriteTemplate;

	x = task->data[2] >> 3;
	y = task->data[3] >> 3;
	x += (task->data[12] * 4);

	spriteId = CreateSprite(spriteTemplate, x, y, 35);
	if (spriteId != MAX_SPRITES)
	{
		gSprites[spriteId].data[0] = 18;
		gSprites[spriteId].data[2] = ((task->data[12] * 20) + x) + (task->data[1] * 3);
		gSprites[spriteId].data[4] = y;
		gSprites[spriteId].data[5] = -16 - (task->data[1] * 2);

		InitAnimArcTranslation(&gSprites[spriteId]);
		task->data[11]++;
	}

	task->data[12] *= -1;
}


//Max Rockfall//
static const union AnimCmd sAnimCmdStonePillar[] =
{
	ANIMCMD_FRAME(192, 12),
	ANIMCMD_FRAME(128, 12),
	ANIMCMD_FRAME(64, 12),
	ANIMCMD_FRAME(0, 12),	
	ANIMCMD_END,
};

const union AnimCmd *const gAnimCmdTable_StonePillar[] =
{
	sAnimCmdStonePillar,
};

static const union AnimCmd sAnimCmdStonePillarMulti[] =
{
	ANIMCMD_FRAME(128, 12),
	ANIMCMD_FRAME(64, 12),
	ANIMCMD_FRAME(0, 12),	
	ANIMCMD_END,
};

const union AnimCmd *const gAnimCmdTable_StonePillarMulti[] =
{
	sAnimCmdStonePillarMulti,
};

static const union AffineAnimCmd sSpriteAffineAnim_StonePillarGrow[] =
{
	AFFINEANIMCMD_FRAME(196, 196, 0, 1), //1.75x sprite siz
	AFFINEANIMCMD_END
};

static const union AffineAnimCmd sSpriteAffineAnim_StonePillarFallLeft[] =
{
	AFFINEANIMCMD_FRAME(196, 196, 0, 1), //1.75x sprite size
	AFFINEANIMCMD_FRAME(0, 0, 1, 0x40),
	AFFINEANIMCMD_END
};

static const union AffineAnimCmd sSpriteAffineAnim_StonePillarFallRight[] =
{
	AFFINEANIMCMD_FRAME(196, 196, 0, 1), //1.75x sprite size
	AFFINEANIMCMD_FRAME(0, 0, -1, 0x40),
	AFFINEANIMCMD_END
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_StonePillar[] =
{
	sSpriteAffineAnim_StonePillarGrow,
	sSpriteAffineAnim_StonePillarFallLeft,
	sSpriteAffineAnim_StonePillarFallRight,
};

void AnimTask_IsRockfallPillarLoaded(u8 taskId)
{
	gBattleAnimArgs[0] = (IndexOfSpriteTileTag(ANIM_TAG_STONE_PILLAR) == 0xFF) ? 0 : 1;
	DestroyAnimVisualTask(taskId);
}

//Creates a stone pillar that falls on the target
//arg 0: initial x pixel offset
//arg 1: initial y pixel offset
void SpriteCB_StonePillarStep1(struct Sprite* sprite);
void SpriteCB_StonePillarStep2(struct Sprite* sprite);
void SpriteCB_StonePillar(struct Sprite* sprite)
{
	InitSpritePosToAnimTarget(sprite, TRUE);

	sprite->pos2.x = gBattleAnimArgs[0];
	if (SIDE(gBattleAnimTarget) == B_SIDE_PLAYER)
	{
		sprite->pos2.x *= -1;
		sprite->pos2.y *= -1;
	}

	sprite->callback = SpriteCB_StonePillarStep1;
}

void SpriteCB_StonePillarStep1(struct Sprite* sprite)
{
	if (sprite->data[0]++ == 0x48)
	{
		if (SIDE(gBattleAnimTarget) == B_SIDE_PLAYER)
			StartSpriteAffineAnim(sprite, 1);
		else
			StartSpriteAffineAnim(sprite, 2);

		sprite->data[0] = 0; sprite->data[1] = 0;
		sprite->callback = SpriteCB_StonePillarStep2;
	}
}

void SpriteCB_StonePillarStep2(struct Sprite* sprite)
{
	if (!sprite->affineAnimEnded)
	{
		//Counteract the rotation movement
		if (SIDE(gBattleAnimTarget) == B_SIDE_PLAYER)
			sprite->pos2.x--;
		else
			sprite->pos2.x++;

		switch (sprite->data[1]++) {
			case 8:
			case 11:
			case 13:
			case 16:
			case 19:
			case 22:
			case 25 ... 70:
				sprite->pos2.y++; //Counteract the rotation lifting
		}
	}
	else if (sprite->data[0]++ == 0x25) //Wait 0x25 frames and then disappear
		sprite->callback = DestroySpriteAndMatrix;
}


//Max Starfall//
static const union AnimCmd sAnimCmdStarfallBeam[] =
{
	ANIMCMD_FRAME(0, 1),
	ANIMCMD_END
};

static const union AnimCmd sAnimCmdSmiteBeam[] =
{
	ANIMCMD_FRAME(4, 1),
	ANIMCMD_END
};

static const union AnimCmd sAnimCmdPhotonGeyserBeam[] =
{
	ANIMCMD_FRAME(8, 1),
	ANIMCMD_END
};

static const union AnimCmd sAnimCmdEternabeamBeam[] =
{
	ANIMCMD_FRAME(12, 1),
	ANIMCMD_END
};

const union AnimCmd *const gAnimCmdTable_StarfallBeam[] =
{
	sAnimCmdStarfallBeam,
	sAnimCmdSmiteBeam,
	sAnimCmdPhotonGeyserBeam,
	sAnimCmdEternabeamBeam,
};

static const union AffineAnimCmd sSpriteAffineAnim_StarfallBeam[] =
{
	AFFINEANIMCMD_FRAME(0, 0, 0, 16), //Delay
	AFFINEANIMCMD_FRAME(64, 64, 0, 4), //Double in size
	//Pulsate
	AFFINEANIMCMD_FRAME(-128, -128, 0, 1),
	AFFINEANIMCMD_FRAME(128, 128, 0, 1),
	AFFINEANIMCMD_JUMP(2),
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_StarfallBeam[] =
{
	sSpriteAffineAnim_DoNothing,
	sSpriteAffineAnim_StarfallBeam,
};

//Creates a twinkle above the target
void SpriteCB_TwinkleAboveTarget(struct Sprite *sprite)
{
	InitSpritePosToAnimTarget(sprite, TRUE);
	sprite->pos1.y = 6;
	sprite->pos2.y = 0;
	sprite->callback = RunStoredCallbackWhenAnimEnds;
	StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
}

//Imitates launching a beam of light towards the sky
//arg 0: initial x pixel offset
//arg 1: initial y pixel offset
//arg 2: bank
//arg 3: duration
//arg 4: Anim num
//arg 5: affine anim start delay
void SpriteCB_BeamUpStep(struct Sprite* sprite);
void SpriteCB_StarfallBeam(struct Sprite* sprite)
{
	u8 target = LoadBattleAnimTarget(2);

	if (!IsBattlerSpriteVisible(target))
		DestroyAnimSprite(sprite);
	else
	{
		InitSpritePosToGivenTarget(sprite, target);
		StartSpriteAnim(sprite, gBattleAnimArgs[4]);
		sprite->data[0] = gBattleAnimArgs[3];
		sprite->data[1] = gBattleAnimArgs[5];
		sprite->callback = SpriteCB_BeamUpStep;
	}
}

//Animates the beam of light
void SpriteCB_BeamUpStep(struct Sprite* sprite)
{
	if (sprite->data[1]-- == 0)
		StartSpriteAffineAnim(sprite, 1);

	if (sprite->data[0]-- <= 0)
		DestroySpriteAndMatrix(sprite);
}


//Max Steelspike//
static const union AnimCmd sAnimCmdLargeSpike[] =
{
	ANIMCMD_FRAME(0, 3),
	ANIMCMD_FRAME(32, 3),
	ANIMCMD_FRAME(64, 3),
	ANIMCMD_FRAME(96, 3),
	ANIMCMD_END,
};

const union AnimCmd *const gAnimCmdTable_LargeSpike[] =
{
	sAnimCmdLargeSpike,
};

static const union AffineAnimCmd sSpriteAffineAnim_LargeSpikePointedLeft[] =
{
	AFFINEANIMCMD_FRAME(0, 0, 32, 1), //45 degree turn
	AFFINEANIMCMD_END
};

static const union AffineAnimCmd sSpriteAffineAnim_LargeSpikePointedRight[] =
{
	AFFINEANIMCMD_FRAME(0, 0, -32, 1), //45 degree turn
	AFFINEANIMCMD_END
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_LargeSpike[] =
{
	sSpriteAffineAnim_LargeSpikePointedLeft,
	sSpriteAffineAnim_LargeSpikePointedRight,
};

void SpriteCB_MaxSteelspike(struct Sprite* sprite)
{
	if (--sprite->data[7] == 0)
	{
		FreeSpriteOamMatrix(sprite);
		DestroySprite(sprite);
	}
}

//Creates explosions on the same path as Rollout rocks
//No args
static void AnimTask_MaxSteelspikeStep(u8 taskId);
static void CreateSteelspikeSprite(struct Task* task);
void AnimTask_MaxSteelspike(u8 taskId)
{
	int var5;
	u16 var0, var1, var2, var3, var4;
	struct Task *task;

	task = &gTasks[taskId];

	var0 = GetBattlerSpriteCoord(gBattleAnimAttacker, 2);
	var1 = GetBattlerSpriteCoord(gBattleAnimAttacker, 1) + 24;
	var2 = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X_2);
	var3 = GetBattlerSpriteCoord(gBattleAnimTarget, 1) + 24;

	if (BATTLE_PARTNER(gBattleAnimAttacker) == gBattleAnimTarget)
		var3 = var1;

	var4 = 4;
	task->data[8] = 48 - (var4 * 8);

	task->data[0] = 0;
	task->data[11] = 0;
	task->data[9] = 0;
	task->data[12] = 1;

	var5 = task->data[8];
	if (var5 < 0)
		var5 += 7;

	task->data[10] = (var5 >> 3) - 1;

	task->data[2] = var0 * 8;
	task->data[3] = var1 * 8;
	task->data[4] = ((var2 - var0) * 8) / task->data[8];
	task->data[5] = ((var3 - var1) * 8) / task->data[8];
	task->data[6] = 0;
	task->data[7] = 0;

	task->data[1] = var4;
	task->data[15] = GetAnimBattlerSpriteId(0);

	task->func = AnimTask_MaxSteelspikeStep;
}

extern const struct SpriteTemplate gMaxSteelspikeCurvedSpriteTemplate;
static void AnimTask_MaxSteelspikeStep(u8 taskId)
{
	struct Task *task;
	u8 spriteId1, spriteId2;
	u16 x, y;

	task = &gTasks[taskId];

	switch (task->data[0]) {
		case 0:
			task->data[2] += task->data[4];
			task->data[3] += task->data[5];
			if (++task->data[9] >= task->data[10])
			{
				task->data[9] = 0;
				CreateSteelspikeSprite(task);
				task->data[13] += task->data[14];
			}

			if (--task->data[8] == 0)
			{
				task->data[11] = 0;
				task->data[0]++;
			}
			break;
		case 1:
			if (++task->data[11] == 10)
				task->data[0]++;
			break;
		case 2: ;
			x = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X);
			y = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y);

			spriteId1 = CreateSprite(&gMaxSteelspikeCurvedSpriteTemplate, x + 5, y, 35);
			spriteId2 = CreateSprite(&gMaxSteelspikeCurvedSpriteTemplate, x - 5, y, 35);
			StartSpriteAffineAnim(&gSprites[spriteId1], 0); //Point Left
			gSprites[spriteId1].data[7] = 50;
			StartSpriteAffineAnim(&gSprites[spriteId2], 1); //Point Right
			gSprites[spriteId2].data[7] = 50;

			if (SIDE(gBattleAnimTarget) == B_SIDE_PLAYER)
			{
				gSprites[spriteId1].oam.priority = 1; //Above player's Pokemon
				gSprites[spriteId2].oam.priority = 1; //Above player's Pokemon
				gSprites[spriteId1].pos1.y += 15;
				gSprites[spriteId2].pos2.y += 15;
			}

			task->data[0]++;
			task->data[11] = 0;
			break;
		case 3:
			if (++task->data[11] == 7)
				task->data[0]++;
			break;
		case 4:
			DestroyAnimVisualTask(taskId);
	}
}

extern const struct SpriteTemplate gMaxSteelspikeSpriteTemplate;
static void CreateSteelspikeSprite(struct Task* task)
{
	u8 spriteId;
	u16 x, y;

	x = task->data[2] >> 3;
	y = task->data[3] >> 3;
	x += (task->data[12] * 8);

	spriteId = CreateSprite(&gMaxSteelspikeSpriteTemplate, x, y, 35);
	if (spriteId != MAX_SPRITES)
	{
		gSprites[spriteId].data[0] = 18;
		gSprites[spriteId].data[2] = ((task->data[12] * 20) + x) + (task->data[1] * 3);
		gSprites[spriteId].data[4] = y;
		gSprites[spriteId].data[5] = -16 - (task->data[1] * 2);
		gSprites[spriteId].data[7] = task->data[8] + 60;

		PlaySE(0x71);
		task->data[11]++;
	}

	task->data[12] *= -1;
}


//Max Wyrmwind//
static const union AnimCmd sAnimCmdTornado[] =
{
	ANIMCMD_FRAME(0, 3),
	ANIMCMD_FRAME(64, 3),
	ANIMCMD_FRAME(0, 3),
	ANIMCMD_FRAME(128, 3),
	ANIMCMD_JUMP(0)
};

const union AnimCmd *const gAnimCmdTable_Tornado[] =
{
	sAnimCmdTornado,
};

//Creates a Tornado on the attacker and launches it at the target
//arg 0: initial x pixel offset
//arg 1: initial y pixel offset
//arg 2: dest x pixel offset
//arg 3: dest y pixel offset
//arg 4: move speed
//arg 5: existance time on target
void SpriteCB_MaxWyrmwindTornadoStep1(struct Sprite* sprite);
void SpriteCB_MaxWyrmwindTornadoStep2(struct Sprite* sprite);
void SpriteCB_MaxWyrmwindTornadoStep3(struct Sprite* sprite);
void SpriteCB_MaxWyrmwindTornado(struct Sprite* sprite)
{
	InitSpritePosToAnimAttacker(sprite, FALSE);

	sprite->data[0] = gBattleAnimArgs[4]; //Speed delay
	sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X_2) + gBattleAnimArgs[2];
	sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET) + gBattleAnimArgs[3];

	sprite->callback = SpriteCB_MaxWyrmwindTornadoStep1;
}

void SpriteCB_MaxWyrmwindTornadoStep1(struct Sprite* sprite)
{
	if (!FuncIsActiveTask(AnimTask_DynamaxGrowthStep))
	{
		sprite->callback = StartAnimLinearTranslation;
		StoreSpriteCallbackInData6(sprite, SpriteCB_MaxWyrmwindTornadoStep2);
	}

	if (!sprite->affineAnimEnded && (sprite->data[6]++ & 1) == 0)
		sprite->pos1.y--; //Counteract growth
}

void SpriteCB_MaxWyrmwindTornadoStep2(struct Sprite* sprite)
{
	sprite->data[6] = 0;
	sprite->data[7] = gBattleAnimArgs[5]; //Existance time on target - shouldn't be overwritten prior
	sprite->callback = SpriteCB_MaxWyrmwindTornadoStep3;
}

void SpriteCB_MaxWyrmwindTornadoStep3(struct Sprite* sprite)
{
	if (sprite->data[6]++ >= sprite->data[7])
		DestroySpriteAndMatrix(sprite);
}


//G-Max Cannonade//
//Launches a projectile from the attacker's cannons to the target.
//arg 0: initial x pixel offset
//arg 1: initial y pixel offset
//arg 2: target x pixel offset
//arg 3: target y pixel offset
//arg 4: duration
//arg 5: wave amplitude
static void SpriteCB_CustomArc_Step(struct Sprite *sprite);
void SpriteCB_CannonadeBall(struct Sprite* sprite)
{
	InitSpritePosToAnimAttacker(sprite, 0);

	if (SIDE(gBattleAnimAttacker))
		gBattleAnimArgs[2] = -gBattleAnimArgs[2];

	sprite->data[0] = gBattleAnimArgs[4];
	sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X_2) + gBattleAnimArgs[2]; //Target X
	sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET) + gBattleAnimArgs[3]; //Target Y
	sprite->data[5] = gBattleAnimArgs[5];
	InitAnimArcTranslation(sprite);

	sprite->callback = SpriteCB_CustomArc_Step;
}

static void SpriteCB_CustomArc_Step(struct Sprite *sprite)
{
	sprite->invisible = FALSE;

	if (TranslateAnimHorizontalArc(sprite))
		DestroyAnimSprite(sprite);
}


//G-Max Vine Lash//
static const union AffineAnimCmd sSpriteAffineAnim_VineLashLeft[] =
{
	AFFINEANIMCMD_FRAME(256, 256, 0, 1), //Double sprite size
	AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sSpriteAffineAnim_VineLashRight[] =
{
	AFFINEANIMCMD_FRAME(-256 - 256 - 256, 256, 0, 1), //Double sprite size
	AFFINEANIMCMD_END,
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_VineLash[] =
{
	sSpriteAffineAnim_VineLashLeft,
	sSpriteAffineAnim_VineLashRight,
};

//Creates vines to whip the target for G-Max Vine Lash
//arg 0: initial x pixel offset
//arg 1: initial y pixel offset
//arg 2: flip horizontal
void SpriteCB_VineLashWhip(struct Sprite* sprite)
{
	if (SIDE(gBattleAnimTarget) == B_SIDE_PLAYER)
		gBattleAnimArgs[0] *= -1;

	StartSpriteAffineAnim(sprite, gBattleAnimArgs[2]);
	InitSpritePosToAnimTarget(sprite, TRUE);
	StoreSpriteCallbackInData6(sprite, DestroySpriteAndMatrix);
	sprite->callback = RunStoredCallbackWhenAnimEnds;
}


///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////Special Anims/////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

//Berry Eating//
static const union AnimCmd sAnimCmdBerryChomp[] =
{
	ANIMCMD_FRAME(0, 0x30),
	ANIMCMD_FRAME(0, 0x25),
	ANIMCMD_FRAME(16, 3),
	ANIMCMD_END,
};

const union AnimCmd *const gAnimCmdBerryChomp[] =
{
	sAnimCmdBerryChomp,
};

static const union AnimCmd sAnimCmdBerryEaten[] =
{
	ANIMCMD_FRAME(16, 3),
	ANIMCMD_END,
};

const union AnimCmd *const gAnimCmdBerryEaten[] =
{
	sAnimCmdBerryEaten,
};


//Primal Reversion//
#define PRIMAL_PULSE_SCALE_SIZE 16
#define PRIMAL_PULSE_FRAME_COUNT 4
static const union AffineAnimCmd sSpriteAffineAnim_PrimalSymbol[] =
{
	AFFINEANIMCMD_FRAME(16, 16, 0, 0), //Start small
	AFFINEANIMCMD_FRAME(32, 32, 0, 15), //Grow sprite
	AFFINEANIMCMD_FRAME(0, 0, 0, 2), //Pause

	//Pulsate
	AFFINEANIMCMD_FRAME(-PRIMAL_PULSE_SCALE_SIZE, -PRIMAL_PULSE_SCALE_SIZE, 0, PRIMAL_PULSE_FRAME_COUNT),
	AFFINEANIMCMD_FRAME( PRIMAL_PULSE_SCALE_SIZE,  PRIMAL_PULSE_SCALE_SIZE, 0, PRIMAL_PULSE_FRAME_COUNT),
	AFFINEANIMCMD_FRAME(-PRIMAL_PULSE_SCALE_SIZE, -PRIMAL_PULSE_SCALE_SIZE, 0, PRIMAL_PULSE_FRAME_COUNT),
	AFFINEANIMCMD_FRAME( PRIMAL_PULSE_SCALE_SIZE,  PRIMAL_PULSE_SCALE_SIZE, 0, PRIMAL_PULSE_FRAME_COUNT),
	AFFINEANIMCMD_FRAME(-PRIMAL_PULSE_SCALE_SIZE, -PRIMAL_PULSE_SCALE_SIZE, 0, PRIMAL_PULSE_FRAME_COUNT),
	AFFINEANIMCMD_FRAME( PRIMAL_PULSE_SCALE_SIZE,  PRIMAL_PULSE_SCALE_SIZE, 0, PRIMAL_PULSE_FRAME_COUNT),
	AFFINEANIMCMD_FRAME(-PRIMAL_PULSE_SCALE_SIZE, -PRIMAL_PULSE_SCALE_SIZE, 0, PRIMAL_PULSE_FRAME_COUNT),
	AFFINEANIMCMD_FRAME( PRIMAL_PULSE_SCALE_SIZE,  PRIMAL_PULSE_SCALE_SIZE, 0, PRIMAL_PULSE_FRAME_COUNT),
	AFFINEANIMCMD_END,
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_PrimalSymbol[] =
{
	sSpriteAffineAnim_PrimalSymbol,
};


//Mega Evolution//
static const union AffineAnimCmd sSpriteAffineAnim_MegaSymbol[] =
{
	AFFINEANIMCMD_FRAME(16, 16, 0, 0), //Start small
	AFFINEANIMCMD_FRAME(32, 32, 0, 15), //Grow sprite
	AFFINEANIMCMD_FRAME(0, 0, 0, 2), //Pause

	//Pulsate
	AFFINEANIMCMD_FRAME(-PRIMAL_PULSE_SCALE_SIZE, -PRIMAL_PULSE_SCALE_SIZE, 0, PRIMAL_PULSE_FRAME_COUNT),
	AFFINEANIMCMD_FRAME( PRIMAL_PULSE_SCALE_SIZE,  PRIMAL_PULSE_SCALE_SIZE, 0, PRIMAL_PULSE_FRAME_COUNT),
	AFFINEANIMCMD_FRAME(-PRIMAL_PULSE_SCALE_SIZE, -PRIMAL_PULSE_SCALE_SIZE, 0, PRIMAL_PULSE_FRAME_COUNT),
	AFFINEANIMCMD_FRAME( PRIMAL_PULSE_SCALE_SIZE,  PRIMAL_PULSE_SCALE_SIZE, 0, PRIMAL_PULSE_FRAME_COUNT),
	AFFINEANIMCMD_FRAME(-PRIMAL_PULSE_SCALE_SIZE, -PRIMAL_PULSE_SCALE_SIZE, 0, PRIMAL_PULSE_FRAME_COUNT),
	AFFINEANIMCMD_FRAME( PRIMAL_PULSE_SCALE_SIZE,  PRIMAL_PULSE_SCALE_SIZE, 0, PRIMAL_PULSE_FRAME_COUNT),
	AFFINEANIMCMD_FRAME(-32, -32, 0, 15), //Shrink down again
	AFFINEANIMCMD_END,
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_MegaSymbol[] =
{
	sSpriteAffineAnim_MegaSymbol,
};

static void AnimTask_WaitAttackerCry(u8 taskId);
void AnimTask_PlayAttackerCry(u8 taskId)
{
	s8 pan;

	if (SIDE(gBattleAnimAttacker) == B_SIDE_PLAYER)
		pan = -25;
	else
		pan = 25;

	PlayCry3(GetIllusionPartyData(gBattleAnimAttacker)->species, pan, 0);
	gTasks[taskId].func = AnimTask_WaitAttackerCry;	
}

static void AnimTask_WaitAttackerCry(u8 taskId)
{
	if (!IsCryPlaying())
	{
		ClearPokemonCrySongs(); //Reset memory state
		DestroyAnimVisualTask(taskId);
	}
}

u8 ModifyMegaCries(u16 species, u8 mode)
{
	if (mode <= 1)
	{
		#ifdef HIGH_PITCH_MEGA_PRIMAL_CRY
		if (IsMegaSpecies(species)
		|| IsBluePrimalSpecies(species)
		|| IsRedPrimalSpecies(species)
		|| IsGigantamaxSpecies(species))
		{
			mode = 3;
		}
		#endif
	}

	return mode;
}


//Ultra Burst//
static const union AffineAnimCmd sSpriteAffineAnim_UltraBurstSymbol[] =
{
	AFFINEANIMCMD_FRAME(16, 16, 0, 0), //Start small
	AFFINEANIMCMD_FRAME(32, 32, 0, 15), //Grow sprite
	AFFINEANIMCMD_FRAME(0, 0, 0, 2), //Pause

	//Pulsate
	AFFINEANIMCMD_FRAME(-PRIMAL_PULSE_SCALE_SIZE, -PRIMAL_PULSE_SCALE_SIZE, 0, PRIMAL_PULSE_FRAME_COUNT),
	AFFINEANIMCMD_FRAME( PRIMAL_PULSE_SCALE_SIZE,  PRIMAL_PULSE_SCALE_SIZE, 0, PRIMAL_PULSE_FRAME_COUNT),
	AFFINEANIMCMD_FRAME(-PRIMAL_PULSE_SCALE_SIZE, -PRIMAL_PULSE_SCALE_SIZE, 0, PRIMAL_PULSE_FRAME_COUNT),
	AFFINEANIMCMD_FRAME( PRIMAL_PULSE_SCALE_SIZE,  PRIMAL_PULSE_SCALE_SIZE, 0, PRIMAL_PULSE_FRAME_COUNT),
	AFFINEANIMCMD_FRAME(-PRIMAL_PULSE_SCALE_SIZE, -PRIMAL_PULSE_SCALE_SIZE, 0, PRIMAL_PULSE_FRAME_COUNT),
	AFFINEANIMCMD_FRAME( PRIMAL_PULSE_SCALE_SIZE,  PRIMAL_PULSE_SCALE_SIZE, 0, PRIMAL_PULSE_FRAME_COUNT),
	AFFINEANIMCMD_FRAME(-PRIMAL_PULSE_SCALE_SIZE, -PRIMAL_PULSE_SCALE_SIZE, 0, PRIMAL_PULSE_FRAME_COUNT),
	AFFINEANIMCMD_FRAME( PRIMAL_PULSE_SCALE_SIZE,  PRIMAL_PULSE_SCALE_SIZE, 0, PRIMAL_PULSE_FRAME_COUNT),
	AFFINEANIMCMD_FRAME(-PRIMAL_PULSE_SCALE_SIZE, -PRIMAL_PULSE_SCALE_SIZE, 0, PRIMAL_PULSE_FRAME_COUNT),
	AFFINEANIMCMD_FRAME( PRIMAL_PULSE_SCALE_SIZE,  PRIMAL_PULSE_SCALE_SIZE, 0, PRIMAL_PULSE_FRAME_COUNT),
	AFFINEANIMCMD_FRAME(-PRIMAL_PULSE_SCALE_SIZE, -PRIMAL_PULSE_SCALE_SIZE, 0, PRIMAL_PULSE_FRAME_COUNT),
	AFFINEANIMCMD_FRAME( PRIMAL_PULSE_SCALE_SIZE,  PRIMAL_PULSE_SCALE_SIZE, 0, PRIMAL_PULSE_FRAME_COUNT),
	AFFINEANIMCMD_FRAME(-PRIMAL_PULSE_SCALE_SIZE, -PRIMAL_PULSE_SCALE_SIZE, 0, PRIMAL_PULSE_FRAME_COUNT),
	AFFINEANIMCMD_FRAME( PRIMAL_PULSE_SCALE_SIZE,  PRIMAL_PULSE_SCALE_SIZE, 0, PRIMAL_PULSE_FRAME_COUNT),
	AFFINEANIMCMD_FRAME(-PRIMAL_PULSE_SCALE_SIZE, -PRIMAL_PULSE_SCALE_SIZE, 0, PRIMAL_PULSE_FRAME_COUNT),
	AFFINEANIMCMD_FRAME( PRIMAL_PULSE_SCALE_SIZE,  PRIMAL_PULSE_SCALE_SIZE, 0, PRIMAL_PULSE_FRAME_COUNT),
	AFFINEANIMCMD_END,
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_UltraBurstSymbol[] =
{
	sSpriteAffineAnim_UltraBurstSymbol,
};


//Dynamax//
static const union AffineAnimCmd sDynamaxGrowthAffineAnimCmds[] =
{
	AFFINEANIMCMD_FRAME(-2, -2, 0, 64), //Double in size over 1 second
	AFFINEANIMCMD_FRAME(0, 0, 0, 64), //Pause for 1 seconds
	AFFINEANIMCMD_FRAME(16, 16, 0, 8), //Shrink back down in 1/8 of a second
	AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sDynamaxGrowthAttackAnimationAffineAnimCmds[] =
{
	AFFINEANIMCMD_FRAME(-4, -4, 0, 32), //Double in size quicker
	AFFINEANIMCMD_FRAME(0, 0, 0, 32), //Pause for less
	AFFINEANIMCMD_FRAME(16, 16, 0, 8),
	AFFINEANIMCMD_END,
};

//Arg 0: Animation for attack
void AnimTask_DynamaxGrowth(u8 taskId)
{
	struct Task* task = &gTasks[taskId];
	u8 spriteId = GetAnimBattlerSpriteId(ANIM_ATTACKER);

	if (gBattleAnimArgs[0] == 0)
		PrepareAffineAnimInTaskData(task, spriteId, sDynamaxGrowthAffineAnimCmds);
	else
		PrepareAffineAnimInTaskData(task, spriteId, sDynamaxGrowthAttackAnimationAffineAnimCmds);
	task->func = AnimTask_DynamaxGrowthStep;
}

static void AnimTask_DynamaxGrowthStep(u8 taskId)
{
	struct Task* task = &gTasks[taskId];
	if (!RunAffineAnimFromTaskData(task))
		DestroyAnimVisualTask(taskId);
}


//Raid Battles//
void AnimTask_GetRaidBattleStormLevel(u8 taskId)
{
	gBattleAnimArgs[0] = gNewBS->dynamaxData.stormLevel;
	DestroyAnimVisualTask(taskId);
}


///////////////////////////////////////////////////////////////////////////////
//////////////////////////////General Sprite Anims/////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void SpriteCB_AnimSpriteOnSelectedMonPos(struct Sprite *sprite)
{
	if (!sprite->data[0])
	{
		u8 target = LoadBattleAnimTarget(2);

		if (!IsBattlerSpriteVisible(target))
			DestroyAnimSprite(sprite);
		else
		{
			InitSpritePosToGivenTargetRespectPicOffsets(sprite, target);
			sprite->data[0]++;
		}
	}
	else if (sprite->animEnded || sprite->affineAnimEnded)
	{
		DestroySpriteAndMatrix(sprite);
	}
}

void SpriteCB_SpriteOnMonForDuration(struct Sprite *sprite) //Like the screen from Brick Break
{
	u8 target = LoadBattleAnimTarget(0);

	if (!IsBattlerSpriteVisible(target))
		DestroyAnimSprite(sprite);
	else
	{
		sprite->pos1.x = GetBattlerSpriteCoord(target, BATTLER_COORD_X);
		sprite->pos1.y = GetBattlerSpriteCoord(target, BATTLER_COORD_Y);

		sprite->pos1.x += gBattleAnimArgs[1];
		sprite->pos1.y += gBattleAnimArgs[2];
		sprite->data[0] = 0;
		sprite->data[1] = gBattleAnimArgs[3];
		sprite->data[2] = gBattleAnimArgs[4];
		sprite->data[3] = 0;
		sprite->callback = (void*) (0x80B0EF0 | 1);
	}
}

void SpriteCB_SpriteOnMonForDurationUseY(struct Sprite *sprite)
{
	u8 target = LoadBattleAnimTarget(0);

	if (!IsBattlerSpriteVisible(target))
		DestroyAnimSprite(sprite);
	else
	{
		sprite->pos1.x = GetBattlerSpriteCoord(target, BATTLER_COORD_X);
		sprite->pos1.y = GetBattlerSpriteCoord(target, BATTLER_COORD_Y_PIC_OFFSET);

		sprite->pos2.x = gBattleAnimArgs[1];
		sprite->pos2.y = gBattleAnimArgs[2];
		sprite->data[0] = 0;
		sprite->data[1] = gBattleAnimArgs[3];
		sprite->data[2] = gBattleAnimArgs[4];
		sprite->data[3] = 0;
		sprite->callback = (void*) (0x80B0EF0 | 1);
	}
}

void SpriteCB_SpriteOnMonUntilAffineAnimEnds(struct Sprite* sprite)
{
	u8 target = LoadBattleAnimTarget(0);

	if (!IsBattlerSpriteVisible(target))
		DestroyAnimSprite(sprite);
	else
	{
		sprite->pos1.x = GetBattlerSpriteCoord(target, BATTLER_COORD_X_2);
		sprite->pos1.y = GetBattlerSpriteCoord(target, BATTLER_COORD_Y_PIC_OFFSET);
		StoreSpriteCallbackInData6(sprite, DestroySpriteAndMatrix);
		sprite->callback = RunStoredCallbackWhenAffineAnimEnds;
	}
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
			if (IS_DOUBLE_BATTLE)
				InitSpritePosToAnimAttackersCentre(sprite, var);
			else
				InitSpritePosToAnimAttacker(sprite, var);
		}
		else
		{
			if (IS_DOUBLE_BATTLE)
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

void SpriteCB_AnimSpriteOnTargetSideCentre(struct Sprite *sprite)
{
	u8 target = LoadBattleAnimTarget(2);

	if (!sprite->data[0])
	{
		if (SIDE(gBattleAnimAttacker) == SIDE(target))
		{
			if (IS_DOUBLE_BATTLE)
				InitSpritePosToAnimAttackersCentre(sprite, FALSE);
			else
				InitSpritePosToAnimAttacker(sprite, FALSE);
		}
		else
		{
			if (IS_DOUBLE_BATTLE)
				InitSpritePosToAnimTargetsCentre(sprite, FALSE);
			else
				InitSpritePosToAnimTarget(sprite, FALSE);
		}

		sprite->data[0]++;
	}
	else if (sprite->animEnded || sprite->affineAnimEnded)
	{
		DestroySpriteAndMatrix(sprite);
	}
}

//Moves a sprite upward for set amount of time
//arg 0: Bank
//arg 1: X-Offset
//arg 2: Y-Offset
//arg 3: Speed
//arg 4: Duration
static void SpriteCB_MoveSpriteUpwardsForDurationStep(struct Sprite* sprite);
void SpriteCB_MoveSpriteUpwardsForDuration(struct Sprite* sprite)
{
	if (gBattleAnimArgs[0] == ANIM_ATTACKER)
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
	sprite->data[1] = gBattleAnimArgs[3]; //Speed
	sprite->data[2] = gBattleAnimArgs[4]; //Duration
	sprite->callback = SpriteCB_MoveSpriteUpwardsForDurationStep;
}

static void SpriteCB_MoveSpriteUpwardsForDurationStep(struct Sprite* sprite)
{
	sprite->pos1.y -= sprite->data[1];
	if (sprite->data[0]++ > sprite->data[2])
		DestroyAnimSprite(sprite);
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

	if (!IsBattlerSpriteVisible(target))
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

void SpriteCB_TranslateAnimSpriteToTargetMonLocationDestroyMatrix(struct Sprite *sprite)
{
	bool8 v1;
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
	if (GetBattlerSide(gBattleAnimAttacker) != B_SIDE_PLAYER)
		gBattleAnimArgs[2] = -gBattleAnimArgs[2];

	sprite->data[0] = gBattleAnimArgs[4];
	sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X_2) + gBattleAnimArgs[2];
	sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimTarget, coordType) + gBattleAnimArgs[3];
	sprite->callback = StartAnimLinearTranslation;
	StoreSpriteCallbackInData6(sprite, DestroySpriteAndMatrix);
}

//Linearly translates a mon to a target offset. The horizontal offset
//is mirrored for the opponent's pokemon, and the vertical offset
//is only mirrored if arg 3 is set to 1.
//arg 0: 0 = attacker, 1 = target
//arg 1: target x pixel offset
//arg 2: target y pixel offset
//arg 3: mirror vertical translation for opposite battle side
//arg 4: duration
void SlideMonToOffset(struct Sprite *sprite)
{
	u8 battler;
	u8 monSpriteId;
	battler = LoadBattleAnimTarget(0);
	if (!IsBattlerSpriteVisible(battler))
		DestroyAnimSprite(sprite);
	else
	{
		monSpriteId = gBattlerSpriteIds[battler];
		if (SIDE(battler) != B_SIDE_PLAYER)
		{
			gBattleAnimArgs[1] = -gBattleAnimArgs[1];
			if (gBattleAnimArgs[3] == 1)
				gBattleAnimArgs[2] = -gBattleAnimArgs[2];
		}

		sprite->data[0] = gBattleAnimArgs[4];
		sprite->data[1] = gSprites[monSpriteId].pos1.x;
		sprite->data[2] = gSprites[monSpriteId].pos1.x + gBattleAnimArgs[1];
		sprite->data[3] = gSprites[monSpriteId].pos1.y;
		sprite->data[4] = gSprites[monSpriteId].pos1.y + gBattleAnimArgs[2];
		InitSpriteDataForLinearTranslation(sprite);
		sprite->data[3] = 0;
		sprite->data[4] = 0;
		sprite->data[5] = monSpriteId;
		sprite->invisible = TRUE;
		StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
		sprite->callback = TranslateMonSpriteLinearFixedPoint;
	}
}

//Linearly slides a mon's bg picture back to its original sprite position.
//The sprite parameter is a dummy sprite used for facilitating the movement with its callback.
//arg 0: 1 = target or 0 = attacker
//arg 1: direction (0 = horizontal and vertical, 1 = horizontal only, 2 = vertical only)
//arg 2: duration
void SlideMonToOriginalPos(struct Sprite *sprite)
{
	u8 battler,  monSpriteId;
	battler = LoadBattleAnimTarget(0);

	if (!IsBattlerSpriteVisible(battler))
		DestroyAnimSprite(sprite);
	else
	{
		monSpriteId = gBattlerSpriteIds[battler];

		sprite->data[0] = gBattleAnimArgs[2];
		sprite->data[1] = gSprites[monSpriteId].pos1.x + gSprites[monSpriteId].pos2.x;
		sprite->data[2] = gSprites[monSpriteId].pos1.x;
		sprite->data[3] = gSprites[monSpriteId].pos1.y + gSprites[monSpriteId].pos2.y;
		sprite->data[4] = gSprites[monSpriteId].pos1.y;
		InitSpriteDataForLinearTranslation(sprite);
		sprite->data[3] = 0;
		sprite->data[4] = 0;
		sprite->data[5] = gSprites[monSpriteId].pos2.x;
		sprite->data[6] = gSprites[monSpriteId].pos2.y;
		sprite->invisible = TRUE;

		if (gBattleAnimArgs[1] == 1)
			sprite->data[2] = 0;
		else if (gBattleAnimArgs[1] == 2)
			sprite->data[1] = 0;

		sprite->data[7] = gBattleAnimArgs[1];
		sprite->data[7] |= monSpriteId << 8;
		sprite->callback = (void*) (0x8099270 | 1); //SlideMonToOriginalPos_Step
	}
}

void AnimTask_ReloadAttackerSprite(u8 taskId)
{
	u8 spriteId = gBattlerSpriteIds[gBattleAnimAttacker];

	struct Task* task = &gTasks[taskId];
	struct Task* newTask;

	switch (task->data[10]) {
		case 0:
			//To fix an annoying graphical glitch where the old sprite would flash
			//momentarily, we hide the sprite offscreen while we refresh it.
			//Remember the old position so we can go back to it later.
			task->data[11] = gSprites[spriteId].pos1.x;
			gSprites[spriteId].pos1.x = -64;

			//Load the palette and graphics. Note this doesn't cause the sprite to refresh
			LoadBattleMonGfxAndAnimate(gBattleAnimAttacker, 1, spriteId);
			++task->data[10];
			break;

		case 1:
			//Actually update the sprite now
			gSprites[spriteId].invisible = FALSE;
			newTask = &gTasks[CreateTask(sub_807331C, 5)];
			newTask->data[0] = 0;
			newTask->data[2] = gBattleAnimAttacker;
			++task->data[10];
			break;

		case 2:
			//Make sure the task is done. I'm not sure if this is necessary
			if (!FuncIsActiveTask(sub_807331C))
				++task->data[10];
			break;

		case 3:
			//Restore the old X position and end the task
			gSprites[spriteId].pos1.x = task->data[11];
			DestroyAnimVisualTask(taskId);
	}
}

//Launches an object upwards like they were being shot from a geyser
//arg 0: target
//arg 1: initial x pixel offset
//arg 2: initial y pixel offset
void SpriteCB_Geyser(struct Sprite* sprite)
{
	u8 target = LoadBattleAnimTarget(0);

	if (!IsBattlerSpriteVisible(target))
	{
		DestroyAnimSprite(sprite);
		return;
	}
	else
	{
		sprite->pos1.x = GetBattlerSpriteCoord(target, BATTLER_COORD_X_2) + gBattleAnimArgs[1];
		sprite->pos1.y = GetBattlerSpriteCoord(target, BATTLER_COORD_Y_PIC_OFFSET) + gBattleAnimArgs[2];
	}

	sprite->data[0] = gBattleAnimArgs[1] > 0 ? 1 : -1;
	sprite->callback = (void*) (0x80B8DE8 | 1); //AnimMudSportDirtRising
}

//Launches an object upwards like they were being shot from a geyser from a specific side
//arg 0: initial x pixel offset
//arg 1: initial y pixel offset
//arg 2: side
void SpriteCB_SideGeyser(struct Sprite* sprite)
{
	u8 side = gBattleAnimArgs[2];
	u8 bank = GetBattlerAtPosition(side);

	sprite->pos1.x = GetProperCentredCoord(bank, BATTLER_COORD_X) + gBattleAnimArgs[0];
	sprite->pos1.y = GetProperCentredCoord(bank, BATTLER_COORD_Y) + gBattleAnimArgs[1];
	sprite->data[0] = bank > 0 ? 1 : -1;
	sprite->callback = (void*) (0x80B8DE8 | 1); //AnimMudSportDirtRising
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////General Anim Tasks//////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void AnimTask_IsUnbound(u8 taskId)
{
	#ifdef UNBOUND
	gBattleAnimArgs[0] = TRUE;
	#else
	gBattleAnimArgs[0] = FALSE;
	#endif
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

void AnimTask_IsAttackerPlayerSide(u8 taskId)
{
	if (SIDE(gBattleAnimAttacker) == B_SIDE_OPPONENT)
		gBattleAnimArgs[7] = 0;
	else
		gBattleAnimArgs[7] = 1;

	DestroyAnimVisualTask(taskId);
}

void AnimTask_AllBanksInvisible(u8 taskId)
{
	for (u32 i = 0; i < gBattlersCount; ++i)
	{
		u8 spriteId = gBattlerSpriteIds[i];

		if (spriteId == 0xFF || !IsBattlerSpriteVisible(i)) //Pokemon that are already hidden
			gNewBS->hiddenAnimBattlerSprites |= gBitTable[i]; //Set bit to keep hidden after animation
		else
			gSprites[spriteId].invisible = TRUE;
	}

	DestroyAnimVisualTask(taskId);
}

void AnimTask_AllBanksVisible(u8 taskId)
{
	for (u32 i = 0; i < gBattlersCount; ++i)
	{
		u8 spriteId = gBattlerSpriteIds[i];

		if (!IsBattlerSpriteVisible(i) //This means "on the field" instead of "not invisible"
		|| spriteId == 0xFF
		|| gNewBS->hiddenAnimBattlerSprites & gBitTable[i]) //Pokemon that are already hidden
			gNewBS->hiddenAnimBattlerSprites &= ~gBitTable[i]; //Clear bit to keep hidden after animation
		else
			gSprites[spriteId].invisible = FALSE;		
	}

	DestroyAnimVisualTask(taskId);
}

void AnimTask_AllBanksInvisibleExceptAttackerAndTarget(u8 taskId)
{
	for (u32 i = 0; i < gBattlersCount; ++i)
	{
		u8 spriteId = gBattlerSpriteIds[i];

		if (spriteId == GetAnimBattlerSpriteId(ANIM_BANK_ATTACKER)
		||  spriteId == GetAnimBattlerSpriteId(ANIM_BANK_TARGET))
			continue;

		if (spriteId == 0xFF || !IsBattlerSpriteVisible(i)) //Pokemon that are already hidden
			gNewBS->hiddenAnimBattlerSprites |= gBitTable[i]; //Set bit to keep hidden after animation
		else
			gSprites[spriteId].invisible = TRUE;
	}

	DestroyAnimVisualTask(taskId);
}

static void AnimTask_FadeOutHelper(u8 taskId)
{
	if (gTasks[taskId].data[1] == 0) //Target reached
	{
		u32 i;
		u16 bldcnt = GetGpuReg(REG_OFFSET_BLDCNT);

		for (i = 3; i <= 6; ++i)
		{
			if (gTasks[taskId].data[i] < MAX_SPRITES)
				gSprites[gTasks[taskId].data[i]].invisible = TRUE;
		}

		if (bldcnt & BLDCNT_TGT1_BG1)
			ResetBattleAnimBg(FALSE);

		if (bldcnt & BLDCNT_TGT1_BG2)
			ResetBattleAnimBg(TRUE);

		DestroyAnimVisualTask(taskId);
	}
	else
	{
		gTasks[taskId].data[1]--;
		gTasks[taskId].data[2]++;
		SetGpuReg(REG_OFFSET_BLDALPHA, (gTasks[taskId].data[2] * 256) + gTasks[taskId].data[1]);
	}
}

void AnimTask_FadeOutAllBanksExceptAttackerAndTarget(u8 taskId)
{
	//This uses BGs because at most two banks other than attacker and target
	//Not to mention this way only those two would get faded. Using sprites would fade all banks regardless.
	u32 bank;
	bool8 usedBg1, usedBg2;
	u32 blendTarget = 0;
	gTasks[taskId].data[3] = MAX_SPRITES;
	gTasks[taskId].data[4] = MAX_SPRITES;
	gTasks[taskId].data[5] = MAX_SPRITES;
	gTasks[taskId].data[6] = MAX_SPRITES;

	for (bank = 0, usedBg1 = FALSE, usedBg2 = FALSE; bank < gBattlersCount; ++bank)
	{
		bool8 toBg2;
		u8 spriteId = gBattlerSpriteIds[bank];

		if (spriteId == GetAnimBattlerSpriteId(ANIM_BANK_ATTACKER)
		||  spriteId == GetAnimBattlerSpriteId(ANIM_BANK_TARGET)
		|| !IsBattlerSpriteVisible(bank))
			continue;

		//Move to a BG - max 1 mon per bg
		if (usedBg1)
			toBg2 = TRUE;
		else if (usedBg2)
			toBg2 = FALSE; //Use Bg 1
		else
		{	
			u8 position = GetBattlerPosition(bank);
			if (position == B_POSITION_OPPONENT_LEFT
			||  position == B_POSITION_PLAYER_RIGHT)
			{
				toBg2 = FALSE;
				usedBg1 = FALSE;
			}
			else
			{
				toBg2 = TRUE;
				usedBg2 = TRUE;
			}
		}

		MoveBattlerSpriteToBG(bank, toBg2);

		//Fade Out that Bg
		if (!toBg2) //Bg 1
			blendTarget |= BLDCNT_TGT1_BG1 | BLDCNT_TGT2_ALL | BLDCNT_EFFECT_BLEND; //Blend Bg 1 out
		else //Bg 2
			blendTarget |= BLDCNT_TGT1_BG2 | BLDCNT_TGT2_ALL | BLDCNT_EFFECT_BLEND; //Blend Bg 2 out

		if (gTasks[taskId].data[3] == MAX_SPRITES)
			gTasks[taskId].data[3] = spriteId;
		else
			gTasks[taskId].data[4] = spriteId;
	}

	if (blendTarget != 0) //At least one sprite is getting faded
	{
		SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(16, 0));
		SetGpuReg(REG_OFFSET_BLDCNT, blendTarget);
		gTasks[taskId].data[1] = 16;
		gTasks[taskId].data[2] = 0;
		gTasks[taskId].func = AnimTask_FadeOutHelper;
	}
	else
		DestroyAnimVisualTask(taskId); //No point in fading
}

static void AnimTask_FadeOutBank(u8 taskId, u8 bank)
{
	u32 blendTarget = 0;
	gTasks[taskId].data[3] = MAX_SPRITES;
	gTasks[taskId].data[4] = MAX_SPRITES;
	gTasks[taskId].data[5] = MAX_SPRITES;
	gTasks[taskId].data[6] = MAX_SPRITES;

	if (IsBattlerSpriteVisible(bank))
	{
		bool8 toBg2;
		u8 spriteId = gBattlerSpriteIds[bank];

		u8 position = GetBattlerPosition(bank);
		if (position == B_POSITION_OPPONENT_LEFT
		||  position == B_POSITION_PLAYER_RIGHT)
			toBg2 = FALSE;
		else
			toBg2 = TRUE;

		gTasks[taskId].data[3] = spriteId;
		MoveBattlerSpriteToBG(bank, toBg2);

		if (!toBg2)
			blendTarget = BLDCNT_TGT1_BG1 | BLDCNT_TGT2_ALL | BLDCNT_EFFECT_BLEND; //Blend Bg 1 out
		else
			blendTarget = BLDCNT_TGT1_BG2 | BLDCNT_TGT2_ALL | BLDCNT_EFFECT_BLEND; //Blend Bg 2 out
	}

	if (blendTarget != 0) //At least one sprite is getting faded
	{
		SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(16, 0));
		SetGpuReg(REG_OFFSET_BLDCNT, blendTarget);
		gTasks[taskId].data[1] = 16;
		gTasks[taskId].data[2] = 0;
		gTasks[taskId].func = AnimTask_FadeOutHelper;
	}
	else
		DestroyAnimVisualTask(taskId); //No point in fading
}

void AnimTask_FadeOutTarget(u8 taskId)
{
	AnimTask_FadeOutBank(taskId, gBattleAnimTarget);
}

void AnimTask_FadeOutAttackerPartner(u8 taskId)
{
	AnimTask_FadeOutBank(taskId, PARTNER(gBattleAnimAttacker));
}

void AnimTask_FadeOutAllBanks(u8 taskId)
{
	//This uses the sprites instead of Bgs because all sprites need to fade out anyway
	u8 bank;
	u8 dataSlot = 3;
	gTasks[taskId].data[3] = MAX_SPRITES;
	gTasks[taskId].data[4] = MAX_SPRITES;
	gTasks[taskId].data[5] = MAX_SPRITES;
	gTasks[taskId].data[6] = MAX_SPRITES;

	for (bank = 0; bank < gBattlersCount; ++bank)
	{
		u8 spriteId = gBattlerSpriteIds[bank];

		if (spriteId == GetAnimBattlerSpriteId(ANIM_BANK_ATTACKER)
		||  spriteId == GetAnimBattlerSpriteId(ANIM_BANK_TARGET)
		|| !IsBattlerSpriteVisible(bank))
			continue;

		gTasks[taskId].data[dataSlot++] = spriteId;
	}

	if (dataSlot != 3) //At least one sprite will be faded
	{
		SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(16, 0));
		SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL | BLDCNT_EFFECT_BLEND); //Blend sprites out
		gTasks[taskId].data[1] = 16;
		gTasks[taskId].data[2] = 0;
		gTasks[taskId].func = AnimTask_FadeOutHelper;
	}
	else
		DestroyAnimVisualTask(taskId); //No point in fading
}

void AnimTask_SingleBankToBg(u8 taskId)
{
	u8 bank = LoadBattleAnimTarget(0);

	if (IsBattlerSpriteVisible(bank))
	{
		bool8 toBg2;
		u8 position = GetBattlerPosition(bank);
		if (position == B_POSITION_OPPONENT_LEFT
		||  position == B_POSITION_PLAYER_RIGHT)
			toBg2 = FALSE;
		else
			toBg2 = TRUE;

		MoveBattlerSpriteToBG(bank, toBg2);

		if (gBattlerSpriteIds[bank] < MAX_SPRITES)
			gSprites[gBattlerSpriteIds[bank]].invisible = TRUE; //Hide while also on bg
	}

	DestroyAnimVisualTask(taskId);
}

void AnimTask_SingleBankFromBg(u8 taskId)
{
	u8 bank = LoadBattleAnimTarget(0);

	if (IsBattlerSpriteVisible(bank))
	{
		bool8 toBg2;
		u8 position = GetBattlerPosition(bank);
		if (position == B_POSITION_OPPONENT_LEFT
		||  position == B_POSITION_PLAYER_RIGHT)
			toBg2 = FALSE;
		else
			toBg2 = TRUE;

		if (gBattlerSpriteIds[bank] < MAX_SPRITES)
			gSprites[gBattlerSpriteIds[bank]].invisible = FALSE; //Show again now that bg is gone

		ResetBattleAnimBg(toBg2);
	}

	DestroyAnimVisualTask(taskId);
}

static void AnimTask_FadeOutParticlesHelper(u8 taskId)
{
	if (gTasks[taskId].data[1] == 0) //Target reached
	{
		DestroyAnimVisualTask(taskId);
	}
	else if (gTasks[taskId].data[4]++ >= gTasks[taskId].data[3])
	{
		gTasks[taskId].data[4] = 0; //Reset timer
		gTasks[taskId].data[1]--;
		gTasks[taskId].data[2]++;
		SetGpuReg(REG_OFFSET_BLDALPHA, (gTasks[taskId].data[2] * 256) + gTasks[taskId].data[1]);
	}
}

void AnimTask_FadeOutParticles(u8 taskId)
{
	u8 existingTaskId = FindTaskIdByFunc(AnimTask_FadeOutParticlesHelper);
	if (existingTaskId != 0xFF)
		DestroyAnimVisualTask(existingTaskId); //Can only have one of these tasks going at a time

	SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(16, 0));
	SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL); //Blend sprites out
	gTasks[taskId].data[1] = 16;
	gTasks[taskId].data[2] = 0;
	gTasks[taskId].data[3] = gBattleAnimArgs[0]; //Delay
	gTasks[taskId].data[4] = 0; //Delay Timer
	gTasks[taskId].func = AnimTask_FadeOutParticlesHelper;
}


///////////////////////////////////////////////////////////////////////////////
//////////////////////////////////Other Anims//////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

//Shiny Animation//
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

void LoadShinyStarsSpriteTiles(void)
{
	if (GetSpriteTileStartByTag(ANIM_TAG_GOLD_STARS) == 0xFFFF)
		LoadCompressedSpriteSheetUsingHeap(&gBattleAnimPicTable[ANIM_TAG_GOLD_STARS - ANIM_SPRITES_START]);

	if (IndexOfSpritePaletteTag(ANIM_TAG_GOLD_STARS) == 0xFF)
		LoadCompressedSpritePaletteUsingHeap(&gBattleAnimPaletteTable[ANIM_TAG_GOLD_STARS - ANIM_SPRITES_START]);
}

void TryStartShinyAnimation(u8 battler, unusedArg struct Pokemon* mon)
{
	u8 taskId1, taskId2;
	gBattleSpritesDataPtr->healthBoxesData[battler].triedShinyMonAnim = TRUE;

	if (IsBattlerSpriteVisible(battler) && IsMonShiny(GetIllusionPartyData(battler)))
	{
		LoadShinyStarsSpriteTiles();
		taskId1 = CreateTask((void*) (0x80F181C | 1), 10);
		taskId2 = CreateTask((void*) (0x80F181C | 1), 10);
		gTasks[taskId1].data[0] = battler;
		gTasks[taskId2].data[0] = battler;
		gTasks[taskId1].data[1] = 0;
		gTasks[taskId2].data[1] = 1;
		return;
	}

	gBattleSpritesDataPtr->healthBoxesData[battler].finishedShinyMonAnim = TRUE;
}


//Hidden Healthboxes//
static void SetHiddenHealthboxFlag(u8 spriteId)
{	
	gNewBS->hiddenHealthboxFlags[spriteId / 8] |= gBitTable[spriteId % 8]; 
}

static void ClearHiddenHealthboxFlag(u8 spriteId)
{
	gNewBS->hiddenHealthboxFlags[spriteId / 8] &= ~gBitTable[spriteId % 8]; //Remove special hidden flag
}

static void ChangeHealthSpriteVisibilityAndPriority(u8 spriteId, bool8 hide, u8 priority)
{
	if (spriteId >= MAX_SPRITES)
		return;

	struct Sprite* sprite = &gSprites[spriteId];
	sprite->oam.priority = priority;

	if (gNewBS == NULL) //Battle struct was already freed at end of battle
		return;

	if (hide)
	{
		if (!sprite->invisible) //Sprite isn't already hidden
		{
			sprite->invisible = TRUE;
			SetHiddenHealthboxFlag(spriteId); //Set special hidden flag
		}
	}
	else
	{
		if (sprite->invisible && gNewBS->hiddenHealthboxFlags[spriteId / 8] & gBitTable[spriteId % 8]) //Sprite was hidden during animation
		{
			sprite->invisible = FALSE;
			ClearHiddenHealthboxFlag(spriteId); //Remove special hidden flag
		}
	}
}
#define hMain_HealthBarSpriteId	 data[5]
void UpdateOamPriorityInAllHealthboxes(u8 priority)
{
	u32 i;

	#ifndef HIDE_HEALTHBOXES_DURING_ANIMS
		goto DEFAULT_CASE;
	#endif

	if (priority > 0) //Restore boxes
		goto HIDE_OR_SHOW_BOXES; //Always try to reshow hidden boxes

	switch (gBattleBufferA[gBattleAnimAttacker][0]) {
		case CONTROLLER_MOVEANIMATION:
			#ifdef DONT_HIDE_HEALTHBOXES_ATTACKER_STATUS_MOVES
			if (gBattleMoves[sAnimMoveIndex].target & MOVE_TARGET_USER)
				goto DEFAULT_CASE;
			#endif
			break;

		case CONTROLLER_BALLTHROWANIM:
			if (gBattleTypeFlags & BATTLE_TYPE_OLD_MAN)
				goto DEFAULT_CASE; //Because the game thinks the old man's a healthbox for some reason
			break;

		case CONTROLLER_CHOOSEACTION:
			#ifdef TEAM_PREVIEW_TRIGGER
			if (!CantLoadTeamPreviewTrigger())
				break;
			#endif
			goto DEFAULT_CASE; 

		case CONTROLLER_BATTLEANIMATION:
			switch (gBattleBufferA[gBattleAnimAttacker][1]) {
				case B_ANIM_TURN_TRAP:
				case B_ANIM_LEECH_SEED_DRAIN:
				case B_ANIM_MON_HIT:
				case B_ANIM_SNATCH_MOVE:
				case B_ANIM_FUTURE_SIGHT_HIT:
				case B_ANIM_DOOM_DESIRE_HIT:
				case B_ANIM_WISH_HEAL:
				case B_ANIM_MON_SCARED:
				case B_ANIM_GHOST_GET_OUT:
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
				case B_ANIM_DYNAMAX_START:
				case B_ANIM_RAID_BATTLE_ENERGY_BURST:
					goto HIDE_OR_SHOW_BOXES;
			}
		//Fallthrough
		default:
		DEFAULT_CASE:
			for (i = 0; i < gBattlersCount; i++)
			{
				u8 healthboxLeftSpriteId = gHealthboxSpriteIds[i];
				u8 healthboxRightSpriteId = gSprites[gHealthboxSpriteIds[i]].oam.affineParam;
				u8 healthbarSpriteId = gSprites[gHealthboxSpriteIds[i]].hMain_HealthBarSpriteId;

				gSprites[healthboxLeftSpriteId].oam.priority = priority;
				gSprites[healthboxRightSpriteId].oam.priority = priority;
				gSprites[healthbarSpriteId].oam.priority = priority;
			}
			return;
	}

HIDE_OR_SHOW_BOXES:
	if (priority == 0)
		Memset(gNewBS->hiddenHealthboxFlags, 0, sizeof(gNewBS->hiddenHealthboxFlags)); //Wipe hidden flags

	for (i = 0; i < gBattlersCount; ++i)
	{
		u8 healthboxLeftSpriteId = gHealthboxSpriteIds[i];
		u8 healthboxRightSpriteId = gSprites[gHealthboxSpriteIds[i]].oam.affineParam;
		u8 healthbarSpriteId = gSprites[gHealthboxSpriteIds[i]].hMain_HealthBarSpriteId;

		if (priority > 0) //Restore Hidden Healthboxes
		{
			if (IsRaidBattle()
			&& i == BANK_RAID_BOSS
			&& GetBankPartyData(BANK_RAID_BOSS)->hp == 0)
			{
				ClearHiddenHealthboxFlag(healthboxLeftSpriteId);
				ClearHiddenHealthboxFlag(healthboxRightSpriteId);
				ClearHiddenHealthboxFlag(healthbarSpriteId);
				continue; //Don't show the Raid Boss' health bars anymore
			}

			ChangeHealthSpriteVisibilityAndPriority(healthboxLeftSpriteId, FALSE, priority);
			ChangeHealthSpriteVisibilityAndPriority(healthboxRightSpriteId, FALSE, priority);
			ChangeHealthSpriteVisibilityAndPriority(healthbarSpriteId, FALSE, priority);
		}
		else
		{
			ChangeHealthSpriteVisibilityAndPriority(healthboxLeftSpriteId, TRUE, priority);
			ChangeHealthSpriteVisibilityAndPriority(healthboxRightSpriteId, TRUE, priority);
			ChangeHealthSpriteVisibilityAndPriority(healthbarSpriteId, TRUE, priority);
		}
	}
}


//Faster HP Bar Change//
u8 CalcHealthBarPixelChange(unusedArg u8 bank)
{
	#ifdef FASTER_HEALTHBOX_CHANGE
		u16 maxHP = GetMonData(GetBankPartyData(bank), MON_DATA_MAX_HP, NULL);
		u16 amount = maxHP / 48; //48 pixels on healthbar
		u16 leftover = maxHP % 48;

		if (leftover >= 40) //So health like 95 (worst case) is included in next level up
			amount += 1;

		return amount;
	#else
		return 1;
	#endif
}


//Faster EXP Bar Change//

bool8 ShouldFillExpBarToMax(unusedArg u8 bank)
{
	#ifdef FASTER_EXP_BAR_CHANGE
	struct BattleBarInfo* expBar = &gBattleSpritesDataPtr->battleBars[bank];

	if (expBar->oldValue == 0 && abs(expBar->receivedValue) >= expBar->maxValue)
	{
		expBar->currValue = expBar->maxValue;
		return TRUE;
	}
	#endif

	return FALSE;
}

//Get Battler Y-Position//

//The original function would run through gBattlerSpriteIds until it found a spriteId
//that matched. This would cause issues if there was a Pokemon missing from the field who
//used to have the same spriteId as the current sprite in the animation.
u16 GetBattlerYDeltaFromSpriteId(u8 spriteId)
{
	u16 species;
	struct BattleSpriteInfo* spriteInfo;
	struct Pokemon* mon;
	u8 bank = gSprites[spriteId].data[0];

	mon = GetIllusionPartyData(bank);
	spriteInfo = gBattleSpritesDataPtr->bankData;
	if (!spriteInfo[bank].transformSpecies)
		species = GetMonData(mon, MON_DATA_SPECIES, NULL);
	else
		species = spriteInfo[bank].transformSpecies;

	if (SIDE(bank) == B_SIDE_PLAYER)
		return gMonBackPicCoords[species].y_offset;
	else
		return gMonFrontPicCoords[species].y_offset;

	return MAX_SPRITES;
}


//Transform Effect//
void HandleSpeciesGfxDataChange(u8 bankAtk, u8 bankDef, u8 transformType)
{
	u8 position;
	u16 targetSpecies, paletteOffset;
	u32 personality, otId;
	const u32 *lzPaletteData;

	if (transformType == 0xFF) //Silph Scope
	{
		const void* src;
		void* dst;

		struct Pokemon* monAtk;
		position = GetBattlerPosition(bankAtk);
		monAtk = GetBankPartyData(bankAtk);
		targetSpecies = GetMonData(monAtk, MON_DATA_SPECIES, NULL);
		personality = GetMonData(monAtk, MON_DATA_PERSONALITY, NULL);
		otId = GetMonData(monAtk, MON_DATA_OT_ID, NULL);

		HandleLoadSpecialPokePic_DontHandleDeoxys(&gMonFrontPicTable[targetSpecies], gMonSpritesGfxPtr->sprites[position], targetSpecies, personality);
		src = gMonSpritesGfxPtr->sprites[position];
		dst = (void *)(VRAM + 0x10000 + gSprites[gBattlerSpriteIds[bankAtk]].oam.tileNum * 32);
		DmaCopy32(3, src, dst, 0x800);
		paletteOffset = 0x100 + bankAtk * 16;
		lzPaletteData = GetMonSpritePalFromSpeciesAndPersonality(targetSpecies, otId, personality);
		LZDecompressWram(lzPaletteData, gDecompressionBuffer);
		LoadPalette(gDecompressionBuffer, paletteOffset, 32);
		TryFadeBankPaletteForDynamax(bankAtk, paletteOffset);
		gSprites[gBattlerSpriteIds[bankAtk]].pos1.y = GetBattlerSpriteDefault_Y(bankAtk);
		StartSpriteAnim(&gSprites[gBattlerSpriteIds[bankAtk]], gBattleMonForms[bankAtk]);
		SetMonData(monAtk, MON_DATA_NICKNAME, gSpeciesNames[targetSpecies]);
		UpdateNickInHealthbox(gHealthboxSpriteIds[bankAtk], monAtk);
		TryAddPokeballIconToHealthbox(gHealthboxSpriteIds[bankAtk], 1);
	}
	else if (transformType == 0xFE) //Transforming animation for form change
	{
		const void* src;
		void* dst;
		struct Pokemon* monAtk;
		const struct CompressedSpriteSheet* spriteSheet;

		position = GetBattlerPosition(bankAtk);
		monAtk = GetIllusionPartyData(bankAtk);
		targetSpecies = GetMonData(monAtk, MON_DATA_SPECIES, NULL);
		otId = GetMonData(monAtk, MON_DATA_OT_ID, NULL);
		personality = GetMonData(monAtk, MON_DATA_PERSONALITY, NULL);

		if (SIDE(bankAtk) == B_SIDE_PLAYER)
			spriteSheet = &gMonBackPicTable[targetSpecies];
		else
			spriteSheet = &gMonFrontPicTable[targetSpecies];

		HandleLoadSpecialPokePic_DontHandleDeoxys(spriteSheet, gMonSpritesGfxPtr->sprites[position], targetSpecies, personality);
		src = gMonSpritesGfxPtr->sprites[position];
		dst = (void *)(VRAM + 0x10000 + gSprites[gBattlerSpriteIds[bankAtk]].oam.tileNum * 32);
		DmaCopy32(3, src, dst, 0x800);
		paletteOffset = 0x100 + bankAtk * 16;

		if (targetSpecies == SPECIES_CASTFORM)
		{
			LoadPalette(gBattleStruct->castformPalette[gBattleSpritesDataPtr->animationData->animArg], paletteOffset, 32);
		}
		else
		{
			lzPaletteData = GetMonSpritePalFromSpeciesAndPersonality(targetSpecies, otId, personality);
			LZDecompressWram(lzPaletteData, gDecompressionBuffer);
			LoadPalette(gDecompressionBuffer, paletteOffset, 32);
		}

		TryFadeBankPaletteForDynamax(bankAtk, paletteOffset);
		gSprites[gBattlerSpriteIds[bankAtk]].pos1.y = GetBattlerSpriteDefault_Y(bankAtk);
		StartSpriteAnim(&gSprites[gBattlerSpriteIds[bankAtk]], gBattleMonForms[bankAtk]);
	}
	else if (transformType != 0) //Castform
	{
		StartSpriteAnim(&gSprites[gBattlerSpriteIds[bankAtk]], gBattleSpritesDataPtr->animationData->animArg);
		paletteOffset = 0x100 + bankAtk * 16;
		LoadPalette(gBattleStruct->castformPalette[gBattleSpritesDataPtr->animationData->animArg], paletteOffset, 32);
		gBattleMonForms[bankAtk] = gBattleSpritesDataPtr->animationData->animArg;
		TryFadeBankPaletteForDynamax(bankAtk, paletteOffset);
		gSprites[gBattlerSpriteIds[bankAtk]].pos1.y = GetBattlerSpriteDefault_Y(bankAtk);
	}
	else //Regular attack Transform
	{
		const void* src;
		void* dst;
		struct Pokemon* monDef;
		const struct CompressedSpriteSheet* spriteSheet;

		position = GetBattlerPosition(bankAtk);
		monDef = GetBankPartyData(bankDef);
		targetSpecies = GetMonData(monDef, MON_DATA_SPECIES, NULL);
		
		if (IsGigantamaxSpecies(targetSpecies))
			targetSpecies = GetGigantamaxBaseForm(targetSpecies);
		
		targetSpecies = TryFixDynamaxTransformSpecies(bankAtk, targetSpecies);
		otId = GetMonData(GetBankPartyData(bankAtk)/*monDef*/, MON_DATA_OT_ID, NULL); //Must use monAtk, otherwise will be shiny now, and not on screen reload

		if (SIDE(bankAtk) == B_SIDE_PLAYER)
			spriteSheet = &gMonBackPicTable[targetSpecies];
		else
			spriteSheet = &gMonFrontPicTable[targetSpecies];

		HandleLoadSpecialPokePic_DontHandleDeoxys(spriteSheet, gMonSpritesGfxPtr->sprites[position], targetSpecies, gTransformedPersonalities[bankAtk]);
		src = gMonSpritesGfxPtr->sprites[position];
		dst = (void *)(VRAM + 0x10000 + gSprites[gBattlerSpriteIds[bankAtk]].oam.tileNum * 32);
		DmaCopy32(3, src, dst, 0x800);
		paletteOffset = 0x100 + bankAtk * 16;
		lzPaletteData = GetMonSpritePalFromSpeciesAndPersonality(targetSpecies, otId, gTransformedPersonalities[bankAtk]);
		LZDecompressWram(lzPaletteData, gDecompressionBuffer);
		LoadPalette(gDecompressionBuffer, paletteOffset, 32);

		if (targetSpecies == SPECIES_CASTFORM)
		{
			LZDecompressWram(lzPaletteData, gBattleStruct->castformPalette[0]);
			LoadPalette(gBattleStruct->castformPalette[0] + gBattleMonForms[bankDef] * 16, paletteOffset, 32);
		}

		TryFadeBankPaletteForDynamax(bankAtk, paletteOffset);
		gBattleSpritesDataPtr->bankData[bankAtk].transformSpecies = targetSpecies;
		gBattleMonForms[bankAtk] = gBattleMonForms[bankDef];
		gSprites[gBattlerSpriteIds[bankAtk]].pos1.y = GetBattlerSpriteDefault_Y(bankAtk);
		StartSpriteAnim(&gSprites[gBattlerSpriteIds[bankAtk]], gBattleMonForms[bankAtk]);
	}
}


//Move Animations & Special Animations//
void DoMoveAnim(u16 move)
{
	gBattleAnimAttacker = gBankAttacker;
	gBattleAnimTarget = gBankTarget;

	if (IS_DOUBLE_BATTLE
	&& gBattleMoves[move].target & MOVE_TARGET_ALL
	&& SIDE(gBattleAnimAttacker) == SIDE(gBattleAnimTarget)) //Targeting same side
	{
		//Double spread move that is targeted at the partner
		//Try setting anim target to other side of field so the anim looks normal
		u8 foe = FOE(gBankAttacker);
		if (!(gAbsentBattlerFlags & gBitTable[foe]))
			gBattleAnimTarget = foe;
		else if (!(gAbsentBattlerFlags & gBitTable[PARTNER(foe)]))
			gBattleAnimTarget = PARTNER(foe);
	}

	LaunchBattleAnimation(gMoveAnimations, move, TRUE);
}

#define tBattlerId 	data[0]
#define tAnimId 	data[1]
#define tArgumentId	data[2]
#define tState 		data[3]
static void Task_HandleSpecialBattleAnimation(u8 taskId);
static bool8 ShouldAnimBeDoneRegardlessOfSubsitute(u8 animId);
static bool8 ShouldSubstituteRecedeForSpecialBattleAnim(u8 animId);
static void TrySwapBackupSpeciesWithSpecies(u8 activeBattler, u8 animId);
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
	gAnimScriptCallback = NULL;
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
#undef tAnimId
#undef tArgumentId
#undef tState

static bool8 ShouldAnimBeDoneRegardlessOfSubsitute(u8 animId)
{
	switch (animId) {
		case B_ANIM_SUBSTITUTE_FADE:
		case B_ANIM_SNATCH_MOVE:
		case B_ANIM_LOAD_ABILITY_POP_UP:
		case B_ANIM_DESTROY_ABILITY_POP_UP:
		case B_ANIM_RAIN_CONTINUES:
		case B_ANIM_SUN_CONTINUES:
		case B_ANIM_SANDSTORM_CONTINUES:
		case B_ANIM_HAIL_CONTINUES:
		case B_ANIM_STRONG_WINDS_CONTINUE:
		case B_ANIM_FOG_CONTINUES:
		case B_ANIM_ELECTRIC_SURGE:
		case B_ANIM_GRASSY_SURGE:
		case B_ANIM_MISTY_SURGE:
		case B_ANIM_PSYCHIC_SURGE:
		case B_ANIM_ELECTRIC_TERRAIN_ACTIVE:
		case B_ANIM_GRASSY_TERRAIN_ACTIVE:
		case B_ANIM_MISTY_TERRAIN_ACTIVE:
		case B_ANIM_PSYCHIC_TERRAIN_ACTIVE:
		case B_ANIM_LOAD_DEFAULT_BG:
		case B_ANIM_TOTEM_BOOST:
		case B_ANIM_DYNAMAX_START:
		case B_ANIM_RAID_BATTLE_STORM:
		case B_ANIM_DYNAMAX_ENERGY_SWIRL:
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
		case B_ANIM_POWDER_EXPLOSION:
		case B_ANIM_BEAK_BLAST_WARM_UP:
		case B_ANIM_SHELL_TRAP_SET:
		case B_ANIM_BATON_PASS:
		case B_ANIM_DRAGON_TAIL_BLOW_AWAY:
		case B_ANIM_ZMOVE_ACTIVATE:
		case B_ANIM_MEGA_EVOLUTION:
		case B_ANIM_ULTRA_BURST:
		case B_ANIM_DYNAMAX_START:
		case B_ANIM_RAID_BATTLE_ENERGY_BURST:
		case B_ANIM_RAID_BATTLE_BLOW_AWAY:
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


//Critical Capture Animations//
void SpriteCB_CriticalCaptureThrownBallMovement(struct Sprite* sprite)
{
	bool8 lastBounce = FALSE;
	u8 maxBounces = 6;
	int bounceCount;

	bounceCount = sprite->data[3] >> 8;

	if (bounceCount == 0)
		PlaySE(SE_POKE_BALL_SHAKE);

	switch (sprite->data[3] & 0xFF)
	{
	case 0:
		if (bounceCount < 3)
			sprite->pos2.x++;

		sprite->data[5]++;
		if (sprite->data[5] >= 3)
			sprite->data[3] += 257;

		break;
	case 1:
		if (bounceCount < 3 || sprite->pos2.x != 0)
			sprite->pos2.x--;

		sprite->data[5]--;

		if (sprite->data[5] <= 0)
		{
			sprite->data[5] = 0;
			sprite->data[3] &= -0x100;
		}

		if (bounceCount >= maxBounces)
			lastBounce = TRUE;

		break;
	}

	if (lastBounce)
	{

		sprite->data[3] = 0;
		sprite->data[4] = 40; //Starting max height
		sprite->data[5] = 0;
		sprite->callback = SpriteCB_ThrowBallMovement;
	}
}

void SpriteCB_InitThrownBallBouncing(struct Sprite *sprite)
{
	int angle;

	if (sprite->animEnded)
	{
		sprite->data[3] = 0;
		sprite->data[4] = 40;
		sprite->data[5] = 0;
		angle = 0;
		sprite->pos1.y += Cosine(angle, 40);
		sprite->pos2.y = -Cosine(angle, sprite->data[4]);
		if (gNewBS->criticalCapture)
			sprite->callback = SpriteCB_CriticalCaptureThrownBallMovement;
		else
			sprite->callback = SpriteCB_ThrowBallMovement;
	}
}


//Enemy Shadow Callback//
void SpriteCB_EnemyShadow(struct Sprite *shadowSprite)
{
	bool8 invisible = FALSE;
	u8 battlerId = shadowSprite->tBattlerId;
	struct Sprite* battlerSprite = &gSprites[gBattlerSpriteIds[battlerId]];

	if (gNewBS->trainerSlideInProgress)
		return; //Don't do anything

	else if (!battlerSprite->inUse || !IsBattlerSpritePresent(battlerId)
	#ifdef FLAG_SKY_BATTLE
	|| FlagGet(FLAG_SKY_BATTLE) //Doesn't make sense to use Shadows in Sky Battle
	#endif
	#ifdef MAPSEC_DISTORTION_WORLD
	|| (GetCurrentRegionMapSectionId() == MAPSEC_DISTORTION_WORLD
	 && !(gBattleTypeFlags & BATTLE_TYPE_TRAINER)
	 && SIDE(battlerId) == B_SIDE_OPPONENT) //Fighting Giratina
	#endif
	)
	{
		shadowSprite->callback = SpriteCB_SetInvisible;
		return;
	}

	if (IsInMiddleOfEndTurnSwitchIn(battlerId))
		invisible = TRUE;
	else if (gAnimScriptActive || battlerSprite->invisible)
		invisible = TRUE;
	else if (gBattleSpritesDataPtr->bankData[battlerId].transformSpecies != SPECIES_NONE
	&& gEnemyMonElevation[gBattleSpritesDataPtr->bankData[battlerId].transformSpecies] == 0)
		invisible = TRUE;

	if (gBattleSpritesDataPtr->bankData[battlerId].behindSubstitute)
		invisible = TRUE;

	shadowSprite->pos1.x = battlerSprite->pos1.x;
	shadowSprite->pos2.x = battlerSprite->pos2.x;
	shadowSprite->invisible = invisible;
}
