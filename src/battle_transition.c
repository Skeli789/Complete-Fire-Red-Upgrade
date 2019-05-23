#include "defines.h"
#include "../include/battle_transition.h"
#include "../include/field_effect.h"
#include "../include/new/mega.h"
#include "../include/constants/songs.h"

#include "../include/new/battle_transition.h"
#include "../include/new/battle_transition_data.h"
#include "../include/new/character_customization.h"
#include "../include/new/helper_functions.h"
#include "../include/new/frontier.h"
#include "../include/new/multi.h"
#include "../include/new/overworld.h"

#define CENTRED_VS_SYMBOL (MUGSHOT_SPRITE != MUGSHOT_VS_SYMBOL && MUGSHOT_STYLE == MUGSHOT_TWO_BARS)

#define sTransitionStructPtr ((struct TransitionData*) *((u32*) 0x2039A2C))

#define tState 				data[0]
#define tData1              data[1]
#define tData2              data[2]
#define tData3              data[3]
#define tVSSymbol  			data[10]
#define tPartnerSpriteId  	data[11]
#define tOpponentSpriteId2  data[12]
#define tOpponentSpriteId   data[13]
#define tPlayerSpriteId     data[14]
#define tMugshotId 			data[15]

#ifdef LOAD_MUGSHOT_SPRITE_FROM_TABLE
struct MugshotTable
{
	const u8* sprite;
	const u8* pal;
	u16 size;
	s16 x;
	s16 y;
};

static const struct MugshotTable sPreBattleMugshotSprites[147] =
{
	//[106] = {Silver_MugshotTiles, Silver_MugshotPal, 64 * 64 / 2, 0, -32}, //Example: Replaces Blue's sprite with Silver's mugshot
	//[124] = {Silver_MugshotTiles, Silver_MugshotPal, 64 * 64 / 2, 0, -32}, //Example: Replaces Player's sprite with Silver's mugshot
};

#define IS_VALID_TABLE_SPRITE(trainerSpriteID) (trainerSpriteID < ARRAY_COUNT(sPreBattleMugshotSprites) \
											 && sPreBattleMugshotSprites[trainerSpriteID].sprite != NULL \
											 && sPreBattleMugshotSprites[trainerSpriteID].pal != NULL) //Has complete data for image
#endif

static const u16* const sAlternateMugshotsPals[] =
{
	[0x0] = sMugshotsDefaultPal, //This is called as an error pal
	[0x1] = sMugshotsDefaultPal, //0x100
	[0x2] = sMugshotsDefaultPal, //0x200
	[0x3] = Big_Mugshot_Yellow_GrayPal, //0x300
	[0x4] = sMugshotsDefaultPal, //0x400
	[0x5] = sMugshotsDefaultPal, //0x500
	[0x6] = sMugshotsDefaultPal, //0x600
	[0x7] = sMugshotsDefaultPal, //0x700
	[0x8] = sMugshotsDefaultPal, //0x800
	[0x9] = sMugshotsDefaultPal, //0x900
	[0xA] = sMugshotsDefaultPal, //0xA00
	[0xB] = sMugshotsDefaultPal, //0xB00
	[0xC] = sMugshotsDefaultPal, //0xC00
	[0xD] = sMugshotsDefaultPal, //0xD00
	[0xE] = sMugshotsDefaultPal, //0xE00
	[0xF] = sMugshotsDefaultPal, //0xF00
};

//This file's functions:
static u8 CreateMugshotTrainerSprite(u8 trainerSpriteID, s16 x, s16 y, u8 subpriority, u8* buffer, bool8 loadingPlayer);
static void UpdateMugshotSpriteTemplate(struct SpriteTemplate* spriteTemplate, u16 tag);
static void UpdateMugshotSpriteData(u8 spriteId, u8 shape, u16 scaleX, u16 scaleY, u16 rotation, bool8 setScale);

//For pre-battle mugshots
static u8 CreateMugshotTrainerSprite(u8 trainerSpriteID, s16 x, s16 y, u8 subpriority, u8* buffer, bool8 loadingPlayer)
{
    struct SpriteTemplate spriteTemplate;
	
	#if (MUGSHOT_SPRITE == MUGSHOT_VS_SYMBOL) //VS_SYMBOL
	if (loadingPlayer)
	{
		struct CompressedSpritePalette pal = {VS_SpritePal, gTrainerFrontPicPaletteTable[trainerSpriteID].tag};
		struct CompressedSpriteSheet sprite = {VS_SpriteTiles, 64 * 64 / 2, gTrainerFrontPicTable[trainerSpriteID].tag};
		LoadCompressedSpritePaletteOverrideBuffer(&pal, buffer);
		LoadCompressedSpriteSheetOverrideBuffer(&sprite, buffer);
	}
	else
	{
		#ifdef LOAD_MUGSHOT_SPRITE_FROM_TABLE
		if (IS_VALID_TABLE_SPRITE(trainerSpriteID)) //Complete data for image
		{
			struct CompressedSpritePalette pal = {sPreBattleMugshotSprites[trainerSpriteID].pal, gTrainerFrontPicPaletteTable[trainerSpriteID].tag};
			struct CompressedSpriteSheet sprite = {sPreBattleMugshotSprites[trainerSpriteID].sprite, sPreBattleMugshotSprites[trainerSpriteID].size, gTrainerFrontPicTable[trainerSpriteID].tag};
			LoadCompressedSpritePaletteOverrideBuffer(&pal, buffer);
			LoadCompressedSpriteSheetOverrideBuffer(&sprite, buffer);
			x += sPreBattleMugshotSprites[trainerSpriteID].x;
			y += sPreBattleMugshotSprites[trainerSpriteID].y;
		}
		else
		#endif
		{
			LoadCompressedSpritePaletteOverrideBuffer(&gTrainerFrontPicPaletteTable[trainerSpriteID], buffer);
			LoadCompressedSpriteSheetOverrideBuffer(&gTrainerFrontPicTable[trainerSpriteID], buffer);
		}
	}
	#else //MUGSHOT_PLAYER
	{
		#ifdef LOAD_MUGSHOT_SPRITE_FROM_TABLE
		if (IS_VALID_TABLE_SPRITE(trainerSpriteID)) //Complete data for image
		{
			struct CompressedSpritePalette pal = {sPreBattleMugshotSprites[trainerSpriteID].pal, gTrainerFrontPicPaletteTable[trainerSpriteID].tag};
			struct CompressedSpriteSheet sprite = {sPreBattleMugshotSprites[trainerSpriteID].sprite, sPreBattleMugshotSprites[trainerSpriteID].size, gTrainerFrontPicTable[trainerSpriteID].tag};
			LoadCompressedSpritePaletteOverrideBuffer(&pal, buffer);
			LoadCompressedSpriteSheetOverrideBuffer(&sprite, buffer);
			x += sPreBattleMugshotSprites[trainerSpriteID].x;
			y += sPreBattleMugshotSprites[trainerSpriteID].y;
		}
		else
		#endif
		{
			++loadingPlayer; //So compiler doesn't complain
			struct CompressedSpritePalette palStruct = {GetTrainerSpritePal(trainerSpriteID), gTrainerFrontPicPaletteTable[trainerSpriteID].tag};
			LoadCompressedSpritePaletteOverrideBuffer(&palStruct, buffer);
			LoadCompressedSpriteSheetOverrideBuffer(&gTrainerFrontPicTable[trainerSpriteID], buffer);
		}
	}
	#endif

	UpdateMugshotSpriteTemplate(&spriteTemplate, gTrainerFrontPicTable[trainerSpriteID].tag);
    return CreateSprite(&spriteTemplate, x, y, subpriority);
}

bool8 Phase2_Mugshot_Func2(struct Task* task)
{
    s16 i, j;
    u16 *dst1, *dst2;
    const u16* MugshotsMap;

    MugshotsMap = sMugshotsTilemap;
    sub_80D3E28(&dst1, &dst2); //sub_8149F58 in Emerald
    CpuSet(sMugshotsTiles, dst2, 0xF0);

	if (sTrainerEventObjectLocalId != 0) //Used for mugshots
	{
		LoadPalette(sAlternateMugshotsPals[sTrainerEventObjectLocalId >> 8], 0xF0, 0x20);
	}
	else
	{
		LoadPalette(sOpponentMugshotsPals[task->tMugshotId], 0xF0, 0x20);
	}
    
    LoadPalette(sPlayerMugshotsPals[gSaveBlock2->playerGender], 0xFA, 0xC);
    for (i = 0; i < 20; i++)
    {
        for (j = 0; j < 32; j++, MugshotsMap++)
        {
            dst1[i * 32 + j] = *MugshotsMap | 0xF000;
        }
    }

    EnableInterrupts(INTR_FLAG_HBLANK);
    SetHBlankCallback(HBlankCB_Phase2_Mugshots);
    task->tState++;
    return FALSE;
}

void Mugshots_CreateOpponentPlayerSprites(struct Task* task)
{
    struct Sprite* opponentSprite, *playerSprite;
    s16 mugshotId = task->tMugshotId;
	u8 trainerSpriteID, trainerSpriteID2, trainerSpriteIDPartner;
	s16 x1, x2, y1, y2;
	
	#if (MUGSHOT_STYLE == MUGSHOT_BIG)
		x1 = 64; y1 = 48;
		x2 = 304, y2 = 42;
	#elif (MUGSHOT_STYLE == MUGSHOT_DP)
		x1 = 64; y1 = 74;
		x2 = 304, y2 = 74;
	#else
		x1 = 64; y1 = 42;
		x2 = 304, y2 = 106;
	#endif
	
	#if (MUGSHOT_SPRITE == MUGSHOT_VS_SYMBOL)
		#if (MUGSHOT_STYLE == MUGSHOT_BIG)
			y2 = 47;			
		#elif (MUGSHOT_STYLE == MUGSHOT_DP) //Smaller Vs Symbol
			y2 = 48;
		#elif (MUGSHOT_STYLE == MUGSHOT_TWO_BARS)
			y2 = 80;
		#endif
	#endif
	
	#if (CENTRED_VS_SYMBOL)
	//Create a centred VS symbol
		struct SpriteTemplate spriteTemplate;
	
		struct CompressedSpritePalette pal = {VS_SpritePal, GFX_TAG_MEGA_INDICATOR};
		struct CompressedSpriteSheet sprite = {VS_SpriteTiles, 64 * 64 / 2, GFX_TAG_MEGA_INDICATOR};
		LoadCompressedSpritePaletteOverrideBuffer(&pal, gDecompressionBuffer);
		LoadCompressedSpriteSheetOverrideBuffer(&sprite, gDecompressionBuffer);
		UpdateMugshotSpriteTemplate(&spriteTemplate, GFX_TAG_MEGA_INDICATOR); //Not being used currently
		task->tVSSymbol = CreateSprite(&spriteTemplate, sMugshotsOpponentCoords[mugshotId][0] - x1 - 108, 50, 0);
		UpdateMugshotSpriteData(task->tVSSymbol, SPRITE_SHAPE(64x64), 256, 256, 0, TRUE);
	#endif
	
	//Load Opponent A
	if (sTrainerEventObjectLocalId != 0) //Used for mugshots
		trainerSpriteID = GetFrontierTrainerFrontSpriteId(gTrainerBattleOpponent_A, 0);
	else
		trainerSpriteID = sMugshotsTrainerPicIDsTable[mugshotId];		

    task->tOpponentSpriteId = CreateMugshotTrainerSprite(trainerSpriteID,
														sMugshotsOpponentCoords[mugshotId][0] - x1,
														sMugshotsOpponentCoords[mugshotId][1] + y1,
														0, gDecompressionBuffer, FALSE);

	//Load Opponent B
	if (IsTrainerBattleModeAgainstTwoOpponents())
	{
		trainerSpriteID2 = GetFrontierTrainerFrontSpriteId(gTrainerBattleOpponent_B, 1);
		task->tOpponentSpriteId2 = CreateMugshotTrainerSprite(trainerSpriteID2,
															sMugshotsOpponentCoords[mugshotId][0] - x1 - 50,
															sMugshotsOpponentCoords[mugshotId][1] + y1,
															0, gDecompressionBuffer, FALSE);

		#if (MUGSHOT_STYLE == MUGSHOT_BIG)
			UpdateMugshotSpriteData(task->tOpponentSpriteId2, SPRITE_SHAPE(64x64), sMugshotsOpponentRotationScales[mugshotId][0], sMugshotsOpponentRotationScales[mugshotId][1], 0, TRUE);
		#elif defined LOAD_MUGSHOT_SPRITE_FROM_TABLE
			if (!IS_VALID_TABLE_SPRITE(trainerSpriteID2))
			{
				UpdateMugshotSpriteData(task->tOpponentSpriteId2, SPRITE_SHAPE(64x32), sMugshotsOpponentRotationScales[mugshotId][0], sMugshotsOpponentRotationScales[mugshotId][1], 0, TRUE);
			}
			else
				UpdateMugshotSpriteData(task->tOpponentSpriteId2, SPRITE_SHAPE(64x64), 0, 0, 0, FALSE);
		#else
			UpdateMugshotSpriteData(task->tOpponentSpriteId2, SPRITE_SHAPE(64x32), sMugshotsOpponentRotationScales[mugshotId][0], sMugshotsOpponentRotationScales[mugshotId][1], 0, TRUE);
		#endif
	}

	//Load Player
    task->tPlayerSpriteId = CreateMugshotTrainerSprite(PlayerGenderToFrontTrainerPicId(gSaveBlock2->playerGender, TRUE), x2, y2, 0, gDecompressionBuffer, TRUE);

	//Load Player Partner
	#if (MUGSHOT_SPRITE != MUGSHOT_VS_SYMBOL)
	if (IsTrainerBattleModeWithPartner())
	{
		trainerSpriteIDPartner = (GetFrontierTrainerFrontSpriteId(gTrainerBattlePartner, 2));
		task->tPartnerSpriteId = CreateMugshotTrainerSprite(trainerSpriteIDPartner, x2 + 50, y2, 0, gDecompressionBuffer, FALSE);
	
		#if (MUGSHOT_STYLE == MUGSHOT_BIG)
			UpdateMugshotSpriteData(task->tPartnerSpriteId, SPRITE_SHAPE(64x64), -512, 512, 0, TRUE);
		#elif defined LOAD_MUGSHOT_SPRITE_FROM_TABLE
			if (!IS_VALID_TABLE_SPRITE(trainerSpriteIDPartner))
				UpdateMugshotSpriteData(task->tPartnerSpriteId, SPRITE_SHAPE(64x32), -512, 512, 0, TRUE);
			else
				UpdateMugshotSpriteData(task->tPartnerSpriteId, SPRITE_SHAPE(64x64), 0, 0, 0, FALSE);
		#else
			UpdateMugshotSpriteData(task->tPartnerSpriteId, SPRITE_SHAPE(64x32), -512, 512, 0, TRUE);
		#endif
	}
	#else
		trainerSpriteIDPartner = 0; //So the compiler doesn't complain
		++trainerSpriteIDPartner;
	#endif
	
    opponentSprite = &gSprites[task->tOpponentSpriteId];
    playerSprite = &gSprites[task->tPlayerSpriteId];

	opponentSprite->callback = SpiteCB_Mugshot; //sub_8148380 in Emerald
	playerSprite->callback = SpiteCB_Mugshot;

    opponentSprite->oam.affineMode = 3;
    playerSprite->oam.affineMode = 3;

    opponentSprite->oam.matrixNum = AllocOamMatrix();
    playerSprite->oam.matrixNum = AllocOamMatrix();

#if (MUGSHOT_STYLE == MUGSHOT_BIG)
    opponentSprite->oam.shape = SPRITE_SHAPE(64x64);
    playerSprite->oam.shape = SPRITE_SHAPE(64x64);
#elif (MUGSHOT_SPRITE == MUGSHOT_VS_SYMBOL) //VS Symbol
	#ifdef LOAD_MUGSHOT_SPRITE_FROM_TABLE
	if (IS_VALID_TABLE_SPRITE(trainerSpriteID))
		opponentSprite->oam.shape = SPRITE_SHAPE(64x64);
	else
	#endif
		opponentSprite->oam.shape = SPRITE_SHAPE(64x32);
    playerSprite->oam.shape = SPRITE_SHAPE(64x64); //Gets shrunk down later
#elif defined LOAD_MUGSHOT_SPRITE_FROM_TABLE
	if (IS_VALID_TABLE_SPRITE(PlayerGenderToFrontTrainerPicId(gSaveBlock2->playerGender, TRUE)))
		playerSprite->oam.shape = SPRITE_SHAPE(64x64);
	else
		playerSprite->oam.shape = SPRITE_SHAPE(64x32);
		
	if (IS_VALID_TABLE_SPRITE(trainerSpriteID))
		opponentSprite->oam.shape = SPRITE_SHAPE(64x64);
	else
		opponentSprite->oam.shape = SPRITE_SHAPE(64x32);
#else
    opponentSprite->oam.shape = SPRITE_SHAPE(64x32);
    playerSprite->oam.shape = SPRITE_SHAPE(64x32);
#endif

    opponentSprite->oam.size = SPRITE_SIZE(64x64);
    playerSprite->oam.size = SPRITE_SIZE(64x64);

    CalcCenterToCornerVec(opponentSprite, 1, 3, 3);
    CalcCenterToCornerVec(playerSprite, 1, 3, 3);

#ifndef LOAD_MUGSHOT_SPRITE_FROM_TABLE
	SetOamMatrixRotationScaling(opponentSprite->oam.matrixNum, sMugshotsOpponentRotationScales[mugshotId][0], sMugshotsOpponentRotationScales[mugshotId][1], 0);
#else
	if (!IS_VALID_TABLE_SPRITE(trainerSpriteID))
		SetOamMatrixRotationScaling(opponentSprite->oam.matrixNum, sMugshotsOpponentRotationScales[mugshotId][0], sMugshotsOpponentRotationScales[mugshotId][1], 0);
#endif

#if (MUGSHOT_STYLE == MUGSHOT_BIG && MUGSHOT_SPRITE == MUGSHOT_VS_SYMBOL)
    SetOamMatrixRotationScaling(playerSprite->oam.matrixNum, 448, 448, 0);
#elif (MUGSHOT_SPRITE == MUGSHOT_VS_SYMBOL)
	SetOamMatrixRotationScaling(playerSprite->oam.matrixNum, 256, 256, 0);
#elif defined LOAD_MUGSHOT_SPRITE_FROM_TABLE
	if (!IS_VALID_TABLE_SPRITE(PlayerGenderToFrontTrainerPicId(gSaveBlock2->playerGender, TRUE)))
		SetOamMatrixRotationScaling(playerSprite->oam.matrixNum, -512, 512, 0);
#else
	SetOamMatrixRotationScaling(playerSprite->oam.matrixNum, -512, 512, 0);
#endif
}

static void UpdateMugshotSpriteTemplate(struct SpriteTemplate* spriteTemplate, u16 tag)
{
	spriteTemplate->tileTag = tag; 
	spriteTemplate->paletteTag = tag;
	spriteTemplate->oam = &gNewGameBirchOamAttributes;
	spriteTemplate->anims = gDummySpriteAnimTable;
	spriteTemplate->images = NULL;
	spriteTemplate->affineAnims = gDummySpriteAffineAnimTable;
	spriteTemplate->callback = SpriteCallbackDummy;	
}

static void UpdateMugshotSpriteData(u8 spriteId, u8 shape, u16 scaleX, u16 scaleY, u16 rotation, bool8 setScale)
{
	struct Sprite* sprite = &gSprites[spriteId];
	sprite->callback = SpiteCB_Mugshot;
	sprite->oam.affineMode = 3;
	sprite->oam.matrixNum = AllocOamMatrix();
	sprite->oam.shape = shape;
	sprite->oam.size = SPRITE_SIZE(64x64);
	CalcCenterToCornerVec(sprite, 1, 3, 3);
	
	if (setScale)
		SetOamMatrixRotationScaling(sprite->oam.matrixNum, scaleX, scaleY, rotation);
}

//Allow for second trainer in mugshot
bool8 Phase2_Mugshot_Func4(struct Task* task)
{
    u8 i;
    u16* toStore;

    sTransitionStructPtr->VBlank_DMA = FALSE;

    for (i = 0, toStore = gScanlineEffectRegBuffers[0]; i < 160; i++, toStore++)
    {
        *toStore = 0xF0;
    }

    task->tState++;
    task->tData1 = 0;
    task->tData2 = 0;
    task->tData3 = 0;
    sTransitionStructPtr->BG0HOFS_1 -= 8;
    sTransitionStructPtr->BG0HOFS_2 += 8;
	
	sub_80D3120(task->tOpponentSpriteId, 0);
	
	if (IsTrainerBattleModeAgainstTwoOpponents())
	{
		sub_80D3120(task->tOpponentSpriteId2, 0); //sub_8148484 in Emerald
		gSprites[task->tOpponentSpriteId].data[5] = 1;
		gSprites[task->tOpponentSpriteId2].data[5] = 2;
	}

    sub_80D3120(task->tPlayerSpriteId, 1); //sub_8148484 in Emerald
	
	#if (CENTRED_VS_SYMBOL)
		sub_80D3120(task->tVSSymbol, 0);
		gSprites[task->tVSSymbol].data[5] = 3;
	#endif
	
	#if (MUGSHOT_SPRITE != MUGSHOT_VS_SYMBOL)
	if (IsTrainerBattleModeWithPartner())
	{
		sub_80D3120(task->tPartnerSpriteId, 1);
		gSprites[task->tPlayerSpriteId].data[5] = 1;
		gSprites[task->tPartnerSpriteId].data[5] = 2;
	}
	#endif

    IncreaseMugshotFuncState(task->tOpponentSpriteId); //sub_814849C in Emerald
	if (IsTrainerBattleModeAgainstTwoOpponents())
		IncreaseMugshotFuncState(task->tOpponentSpriteId2);
		
	#if (CENTRED_VS_SYMBOL)
		IncreaseMugshotFuncState(task->tVSSymbol);
	#endif

    PlaySE(SE_BT_START);

    sTransitionStructPtr->VBlank_DMA++;
    return FALSE;
}

bool8 Phase2_Mugshot_Func5(struct Task *task)
{
    sTransitionStructPtr->BG0HOFS_1 -= 8;
    sTransitionStructPtr->BG0HOFS_2 += 8;
    if (IsOpponentMugshotDoneSlidingRight(task->tOpponentSpriteId)) //sub_81484B8 in Emerald
    {
        task->tState++;
        IncreaseMugshotFuncState(task->tPlayerSpriteId); //sub_814849C in Emerald
		IncreaseMugshotFuncState(task->tPartnerSpriteId);
    }
    return FALSE;
}

//Fixes a bug where the DNS causes white fading issues after the mugshot
bool8 Phase2_Mugshot_Func8(struct Task *task)
{
    sTransitionStructPtr->VBlank_DMA = FALSE;
	gDontFadeWhite = TRUE;
    BlendPalettes(-1, 0x10, 0x7FFF);
    sTransitionStructPtr->BLDCNT = 0xFF;
    task->tData3 = 0;

    task->tState++;
    return TRUE;
}

bool8 ShiftTrainerMugshotSprite(struct Sprite *sprite)
{
    sprite->pos1.x += sprite->data[1];

	switch (sprite->data[7]) {
		case 0: //Sprite going to the right / opponent sprite
			switch (sprite->data[5]) {
				case 1: //Opponent Sprite A - Two Opponents
					if (sprite->pos1.x > 139) 
						sprite->data[0]++;
					break;

				case 2: //Opponent Sprite B - Two Opponents
				#if (MUGSHOT_SPRITE == MUGSHOT_PLAYER)
					if (sprite->pos1.x > 91)
				#else
					if (sprite->pos1.x > 67)
				#endif
					{
						sprite->pos1.x += 2;
						sprite->data[0]++;
					}
					break;
					
				case 3: //Centred Vs Symbol
					if (sprite->pos1.x > 55) 
						sprite->data[0]++;
					break;

				default: //Regular Opponent
					if (sprite->pos1.x > 103) 
						sprite->data[0]++;
			}
			break;

		default: //Sprite going to the left / player sprite
			switch (sprite->data[5]) {
				case 1: //Player Sprite - Tag Battle
					if (sprite->pos1.x < 97) 
						sprite->data[0]++;
					break;

				case 2: //Partner Sprite - Tag Battle
					if (sprite->pos1.x < 133) 
						sprite->data[0]++;
					break;
				
				default:
					if (sprite->pos1.x < 133) 
						sprite->data[0]++;
			}
    }

    return FALSE;
}
