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

//Structures
extern const struct MugshotTable sPreBattleMugshotSprites[147];
extern const u16* const sMugshotsBigPals[];
extern const u16* const sMugshotsDpPals[];
extern const u16* const sMugshotsTwoBarsPals[];
extern const u16* const sMugshotPlayerPals[];

#define IS_VALID_TABLE_SPRITE(trainerSpriteID) (trainerSpriteID < ARRAY_COUNT(sPreBattleMugshotSprites) \
											 && sPreBattleMugshotSprites[trainerSpriteID].sprite != NULL \
											 && sPreBattleMugshotSprites[trainerSpriteID].pal != NULL) //Has complete data for image

//This file's functions:
static u8 CreateMugshotTrainerSprite(u8 trainerSpriteID, s16 x, s16 y, u8 subpriority, u8* buffer, bool8 loadingPlayer);
static void UpdateMugshotSpriteTemplate(struct SpriteTemplate* spriteTemplate, u16 tag);
static void UpdateMugshotSpriteData(u8 spriteId, u8 shape, u16 scaleX, u16 scaleY, u16 rotation, bool8 setScale);

//For pre-battle mugshots
static u8 CreateMugshotTrainerSprite(u8 trainerSpriteID, s16 x, s16 y, u8 subpriority, u8* buffer, bool8 loadingPlayer)
{
    struct SpriteTemplate spriteTemplate;
	
	
	u16 mugshotSprite = VarGet(VAR_PRE_BATTLE_MUGSHOT_SPRITE);
	
	switch (mugshotSprite)
	{
		case MUGSHOT_VS_SYMBOL:
			if (loadingPlayer)
			{
				struct CompressedSpritePalette pal = {VS_SpritePal, gTrainerFrontPicPaletteTable[trainerSpriteID].tag};
				struct CompressedSpriteSheet sprite = {VS_SpriteTiles, 64 * 64 / 2, gTrainerFrontPicTable[trainerSpriteID].tag};
				LoadCompressedSpritePaletteOverrideBuffer(&pal, buffer);
				LoadCompressedSpriteSheetOverrideBuffer(&sprite, buffer);
			}		
			else
			{
				if (FlagGet(FLAG_LOAD_MUGSHOT_SPRITE_FROM_TABLE))
				{
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
					{
						LoadCompressedSpritePaletteOverrideBuffer(&gTrainerFrontPicPaletteTable[trainerSpriteID], buffer);
						LoadCompressedSpriteSheetOverrideBuffer(&gTrainerFrontPicTable[trainerSpriteID], buffer);						
					}
				}
				else
				{
					LoadCompressedSpritePaletteOverrideBuffer(&gTrainerFrontPicPaletteTable[trainerSpriteID], buffer);
					LoadCompressedSpriteSheetOverrideBuffer(&gTrainerFrontPicTable[trainerSpriteID], buffer);
				}
			}
			break;
		
		case MUGSHOT_PLAYER:
		default:
			if (FlagGet(FLAG_LOAD_MUGSHOT_SPRITE_FROM_TABLE))
			{
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
				{
					++loadingPlayer; //So compiler doesn't complain
					struct CompressedSpritePalette palStruct = {GetTrainerSpritePal(trainerSpriteID), gTrainerFrontPicPaletteTable[trainerSpriteID].tag};
					LoadCompressedSpritePaletteOverrideBuffer(&palStruct, buffer);
					LoadCompressedSpriteSheetOverrideBuffer(&gTrainerFrontPicTable[trainerSpriteID], buffer);
				}
			}
			else
			{
				++loadingPlayer; //So compiler doesn't complain
				struct CompressedSpritePalette palStruct = {GetTrainerSpritePal(trainerSpriteID), gTrainerFrontPicPaletteTable[trainerSpriteID].tag};
				LoadCompressedSpritePaletteOverrideBuffer(&palStruct, buffer);
				LoadCompressedSpriteSheetOverrideBuffer(&gTrainerFrontPicTable[trainerSpriteID], buffer);
			}
			break;
	}

	UpdateMugshotSpriteTemplate(&spriteTemplate, gTrainerFrontPicTable[trainerSpriteID].tag);
    return CreateSprite(&spriteTemplate, x, y, subpriority);
}


bool8 Phase2_Mugshot_Func2(struct Task* task)
{
    s16 i, j;
    u16 *dst1, *dst2;
    const u16* MugshotsMap;

	u8 mugshotStyle = VarGet(VAR_PRE_BATTLE_MUGSHOT_STYLE);
	switch (mugshotStyle)
	{
		case MUGSHOT_BIG:
			MugshotsMap = Big_MugshotMap;
			sub_80D3E28(&dst1, &dst2);
			CpuSet(Big_MugshotTiles, dst2, 0xF0);
			break;
			
		case MUGSHOT_DP:
			MugshotsMap = DP_MugshotMap;
			sub_80D3E28(&dst1, &dst2);
			CpuSet(DP_MugshotTiles, dst2, 0xF0);
			break;
			
		case MUGSHOT_TWO_BARS:
		default:
			MugshotsMap = TwoBars_MugshotMap;
			sub_80D3E28(&dst1, &dst2);
			CpuSet(TwoBars_MugshotTiles, dst2, 0xF0);
			break;
	}

	if (sTrainerEventObjectLocalId != 0) //Used for mugshots
	{
		switch (mugshotStyle)
		{
			case MUGSHOT_BIG:
				LoadPalette(sMugshotsBigPals[sTrainerEventObjectLocalId >> 8], 0xF0, 0x20);
				break;
				
			case MUGSHOT_DP:
				LoadPalette(sMugshotsDpPals[sTrainerEventObjectLocalId >> 8], 0xF0, 0x20);
				break;
				
			case MUGSHOT_TWO_BARS:
			default:
				LoadPalette(sMugshotsTwoBarsPals[sTrainerEventObjectLocalId >> 8], 0xF0, 0x20);
				break;			
		}
	}
	else
	{
		LoadPalette(sOpponentMugshotsPals[task->tMugshotId], 0xF0, 0x20);
	}
	
	u8 playerPal = VarGet(VAR_MUGSHOT_PLAYER_PAL);
	if (mugshotStyle == MUGSHOT_TWO_BARS && playerPal > 0)
		LoadPalette(sMugshotPlayerPals[playerPal] + 5, 0xFA, 0xA);
	else
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
	
	
	u16 mugshotType = VarGet(VAR_PRE_BATTLE_MUGSHOT_STYLE);
	u16 mugshotSprite = VarGet(VAR_PRE_BATTLE_MUGSHOT_SPRITE);
	
	switch (mugshotType)
	{
		case MUGSHOT_BIG:
			x1 = 64; y1 = 48;
			x2 = 304, y2 = 42;
			break;
			
		case MUGSHOT_DP:
			x1 = 64; y1 = 74;
			x2 = 304, y2 = 74;
			break;			
		
		case MUGSHOT_TWO_BARS:
		default:
			x1 = 64; y1 = 42;
			x2 = 304, y2 = 106;
			break;
	}
	
	if (mugshotSprite == MUGSHOT_VS_SYMBOL)
	{
		switch (mugshotType)
		{
			case MUGSHOT_BIG:
				y2 = 47;
				break;
				
			case MUGSHOT_DP:	//Smaller Vs Symbol
				y2 = 48;
				break;
				
			case MUGSHOT_TWO_BARS:
			default:
				y2 = 80;
				break;
		}
	}
	// Check Centered VS Symbol
	if (mugshotSprite == MUGSHOT_PLAYER && mugshotType == MUGSHOT_TWO_BARS)
	{		
		struct SpriteTemplate spriteTemplate;
	
		struct CompressedSpritePalette pal = {VS_SpritePal, GFX_TAG_MEGA_INDICATOR};
		struct CompressedSpriteSheet sprite = {VS_SpriteTiles, 64 * 64 / 2, GFX_TAG_MEGA_INDICATOR};
		LoadCompressedSpritePaletteOverrideBuffer(&pal, gDecompressionBuffer);
		LoadCompressedSpriteSheetOverrideBuffer(&sprite, gDecompressionBuffer);
		UpdateMugshotSpriteTemplate(&spriteTemplate, GFX_TAG_MEGA_INDICATOR); //Not being used currently
		task->tVSSymbol = CreateSprite(&spriteTemplate, sMugshotsOpponentCoords[mugshotId][0] - x1 - 108, 50, 0);
		UpdateMugshotSpriteData(task->tVSSymbol, SPRITE_SHAPE(64x64), 256, 256, 0, TRUE);
	}
	
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

		if (mugshotType == MUGSHOT_BIG)
			UpdateMugshotSpriteData(task->tOpponentSpriteId2, SPRITE_SHAPE(64x64), sMugshotsOpponentRotationScales[mugshotId][0], sMugshotsOpponentRotationScales[mugshotId][1], 0, TRUE);
		else if (FlagGet(FLAG_LOAD_MUGSHOT_SPRITE_FROM_TABLE))
		{
			if (!IS_VALID_TABLE_SPRITE(trainerSpriteID2))
			{
				UpdateMugshotSpriteData(task->tOpponentSpriteId2, SPRITE_SHAPE(64x32), sMugshotsOpponentRotationScales[mugshotId][0], sMugshotsOpponentRotationScales[mugshotId][1], 0, TRUE);
			}
			else
			{
				UpdateMugshotSpriteData(task->tOpponentSpriteId2, SPRITE_SHAPE(64x64), 0, 0, 0, FALSE);
			}
		}
		else
		{
			UpdateMugshotSpriteData(task->tOpponentSpriteId2, SPRITE_SHAPE(64x32), sMugshotsOpponentRotationScales[mugshotId][0], sMugshotsOpponentRotationScales[mugshotId][1], 0, TRUE);
		}
	} 
	
	//Load Player
    task->tPlayerSpriteId = CreateMugshotTrainerSprite(PlayerGenderToFrontTrainerPicId(gSaveBlock2->playerGender, TRUE), x2, y2, 0, gDecompressionBuffer, TRUE);

	//Load Player Partner
	if (mugshotSprite == MUGSHOT_PLAYER)
	{
		if (IsTrainerBattleModeWithPartner())
		{
			trainerSpriteIDPartner = (GetFrontierTrainerFrontSpriteId(gTrainerBattlePartner, 2));
			task->tPartnerSpriteId = CreateMugshotTrainerSprite(trainerSpriteIDPartner, x2 + 50, y2, 0, gDecompressionBuffer, FALSE);
			
			if (mugshotType == MUGSHOT_BIG)
				UpdateMugshotSpriteData(task->tPartnerSpriteId, SPRITE_SHAPE(64x64), -512, 512, 0, TRUE);
			else if (FlagGet(FLAG_LOAD_MUGSHOT_SPRITE_FROM_TABLE))
			{
				if (!IS_VALID_TABLE_SPRITE(trainerSpriteIDPartner))
					UpdateMugshotSpriteData(task->tPartnerSpriteId, SPRITE_SHAPE(64x32), -512, 512, 0, TRUE);
				else
					UpdateMugshotSpriteData(task->tPartnerSpriteId, SPRITE_SHAPE(64x64), 0, 0, 0, FALSE);				
			}
			else
			{
				UpdateMugshotSpriteData(task->tPartnerSpriteId, SPRITE_SHAPE(64x32), -512, 512, 0, TRUE);
			}
		}
	}
	else
	{
		trainerSpriteIDPartner = 0; //So the compiler doesn't complain
		++trainerSpriteIDPartner;
	}
	
    opponentSprite = &gSprites[task->tOpponentSpriteId];
    playerSprite = &gSprites[task->tPlayerSpriteId];

	opponentSprite->callback = SpiteCB_Mugshot; //sub_8148380 in Emerald
	playerSprite->callback = SpiteCB_Mugshot;

    opponentSprite->oam.affineMode = 3;
    playerSprite->oam.affineMode = 3;

    opponentSprite->oam.matrixNum = AllocOamMatrix();
    playerSprite->oam.matrixNum = AllocOamMatrix();

	
	if (mugshotType == MUGSHOT_BIG)
	{
		opponentSprite->oam.shape = SPRITE_SHAPE(64x64);
		playerSprite->oam.shape = SPRITE_SHAPE(64x64);		
	}
	else if (mugshotSprite == MUGSHOT_VS_SYMBOL)
	{
		if (mugshotType == MUGSHOT_DP || mugshotType == MUGSHOT_TWO_BARS)
		{
			if (FlagGet(FLAG_LOAD_MUGSHOT_SPRITE_FROM_TABLE))
				opponentSprite->oam.shape = SPRITE_SHAPE(64x64);
			else
				opponentSprite->oam.shape = SPRITE_SHAPE(64x32);
		}
		else if (FlagGet(FLAG_LOAD_MUGSHOT_SPRITE_FROM_TABLE))
		{
			if (IS_VALID_TABLE_SPRITE(trainerSpriteID))
				opponentSprite->oam.shape = SPRITE_SHAPE(64x64);
			else
			{
				opponentSprite->oam.shape = SPRITE_SHAPE(64x32);
				playerSprite->oam.shape = SPRITE_SHAPE(64x64); //Gets shrunk down later		
			}
		}
	}
	else if (FlagGet(FLAG_LOAD_MUGSHOT_SPRITE_FROM_TABLE))
	{
		if (IS_VALID_TABLE_SPRITE(PlayerGenderToFrontTrainerPicId(gSaveBlock2->playerGender, TRUE)))
			playerSprite->oam.shape = SPRITE_SHAPE(64x64);
		else
			playerSprite->oam.shape = SPRITE_SHAPE(64x32);
			
		if (IS_VALID_TABLE_SPRITE(trainerSpriteID))
			opponentSprite->oam.shape = SPRITE_SHAPE(64x64);
		else
			opponentSprite->oam.shape = SPRITE_SHAPE(64x32);
	}
	else
	{
		opponentSprite->oam.shape = SPRITE_SHAPE(64x32);
		playerSprite->oam.shape = SPRITE_SHAPE(64x32);
	}
	
    opponentSprite->oam.size = SPRITE_SIZE(64x64);
    playerSprite->oam.size = SPRITE_SIZE(64x64);

    CalcCenterToCornerVec(opponentSprite, 1, 3, 3);
    CalcCenterToCornerVec(playerSprite, 1, 3, 3);

	
	if ((FlagGet(FLAG_LOAD_MUGSHOT_SPRITE_FROM_TABLE)))
	{
		if (!IS_VALID_TABLE_SPRITE(trainerSpriteID))
			SetOamMatrixRotationScaling(opponentSprite->oam.matrixNum, sMugshotsOpponentRotationScales[mugshotId][0], sMugshotsOpponentRotationScales[mugshotId][1], 0);				
	}
	else
	{
		SetOamMatrixRotationScaling(opponentSprite->oam.matrixNum, sMugshotsOpponentRotationScales[mugshotId][0], sMugshotsOpponentRotationScales[mugshotId][1], 0);	
	}

	if (mugshotType == MUGSHOT_BIG && mugshotSprite == MUGSHOT_VS_SYMBOL)
		SetOamMatrixRotationScaling(playerSprite->oam.matrixNum, 448, 448, 0);
	else if (mugshotSprite == MUGSHOT_VS_SYMBOL)
		SetOamMatrixRotationScaling(playerSprite->oam.matrixNum, 256, 256, 0);
	else if (FlagGet(FLAG_LOAD_MUGSHOT_SPRITE_FROM_TABLE))
	{
		if (!IS_VALID_TABLE_SPRITE(PlayerGenderToFrontTrainerPicId(gSaveBlock2->playerGender, TRUE)))
			SetOamMatrixRotationScaling(playerSprite->oam.matrixNum, -512, 512, 0);
	}
	else
		SetOamMatrixRotationScaling(playerSprite->oam.matrixNum, -512, 512, 0);
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
	
	
	
	u16 mugshotType = VarGet(VAR_PRE_BATTLE_MUGSHOT_STYLE);
	u16 mugshotSprite = VarGet(VAR_PRE_BATTLE_MUGSHOT_SPRITE);
	
	if (mugshotSprite == MUGSHOT_PLAYER && mugshotType == MUGSHOT_TWO_BARS)
	{
		sub_80D3120(task->tVSSymbol, 0);
		gSprites[task->tVSSymbol].data[5] = 3;		
	}

	
	if (mugshotSprite != MUGSHOT_VS_SYMBOL)
	{
		if (IsTrainerBattleModeWithPartner())
		{
			sub_80D3120(task->tPartnerSpriteId, 1);
			gSprites[task->tPlayerSpriteId].data[5] = 1;
			gSprites[task->tPartnerSpriteId].data[5] = 2;
		}
	}		

    IncreaseMugshotFuncState(task->tOpponentSpriteId); //sub_814849C in Emerald
	if (IsTrainerBattleModeAgainstTwoOpponents())
		IncreaseMugshotFuncState(task->tOpponentSpriteId2);
		
	
	if (mugshotSprite == MUGSHOT_PLAYER && mugshotType == MUGSHOT_TWO_BARS)	//centered vs symbol
		IncreaseMugshotFuncState(task->tVSSymbol);

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
				
					if (VarGet(VAR_PRE_BATTLE_MUGSHOT_SPRITE) == MUGSHOT_PLAYER)
					{
						if (sprite->pos1.x > 91)
						{
							sprite->pos1.x += 2;
							sprite->data[0]++;
						}
					}
					else
					{
						if (sprite->pos1.x > 67)
						{
							sprite->pos1.x += 2;
							sprite->data[0]++;
						}
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
			switch (sprite->data[5])
			{
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


