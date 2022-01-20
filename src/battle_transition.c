#include "defines.h"
#include "defines_battle.h"
#include "../include/battle_transition.h"
#include "../include/bg.h"
#include "../include/field_effect.h"
#include "../include/scanline_effect.h"
#include "../include/constants/songs.h"
#include "../include/constants/trainers.h"
#include "../include/constants/trainer_classes.h"

#include "../include/new/battle_transition.h"
#include "../include/new/battle_transition_data.h"
#include "../include/new/battle_util.h"
#include "../include/new/character_customization.h"
#include "../include/new/frontier.h"
#include "../include/new/mega.h"
#include "../include/new/multi.h"
#include "../include/new/overworld.h"

/*
battle_transition.c
	handles the transition into battle, eg. trainer mugshots.
*/

typedef bool8 (*TransitionStateFunc)(struct Task *task);
typedef bool8 (*TransitionSpriteCallback)(struct Sprite *sprite);

struct TransitionData
{
	vu8 vblankDma;
	u16 winIn;
	u16 winOut;
	u16 win0H;
	u16 win0V;
	u16 win1H; // not used
	u16 win1V;
	u16 bldCnt;
	u16 bldAlpha;
	u16 bldY;
	s16 bg123HOfs;
	s16 bg123VOfs;
	s16 bg0HOfsOpponent;
	s16 bg0HOfsPlayer;
	s16 bg0VOfs;
	s16 unused_1E;
	s16 counter;
	s16 unused_22;
	s16 data[11]; // for multiple purposes
};

#define sTransitionStructPtr ((struct TransitionData*) *((u32*) 0x2039A2C))

#define tState 				data[0]
#define tData1			  data[1]
#define tData2			  data[2]
#define tData3			  data[3]
#define tVSSymbol  			data[10]
#define tPartnerSpriteId  	data[11]
#define tOpponentSpriteId2  data[12]
#define tOpponentSpriteId   data[13]
#define tPlayerSpriteId	 data[14]
#define tMugshotId 			data[15]

#define GFX_TAG_VS_SYMBOL 0xFDF0

//Structures
extern const struct MugshotTable sPreBattleMugshotSprites[147];
extern const u16* const sMugshotsBigPals[];
extern const u16* const sMugshotsDpPals[];
extern const u16* const sMugshotsTwoBarsPals[];
extern const u16* const sMugshotPlayerPals[];

#define IS_VALID_TABLE_SPRITE(trainerPicId) (trainerPicId < ARRAY_COUNT(sPreBattleMugshotSprites) \
											 && sPreBattleMugshotSprites[trainerPicId].sprite != NULL \
											 && sPreBattleMugshotSprites[trainerPicId].pal != NULL) //Has complete data for image

//Old vanilla functions:
void __attribute__((long_call)) BT_Phase1Task(u8 taskId);
void __attribute__((long_call)) BT_Phase2Blur(u8 taskId);
void __attribute__((long_call)) BT_Phase2DistortedWave(u8 taskId);
void __attribute__((long_call)) BT_Phase2HorizontalCorrugate(u8 taskId);
void __attribute__((long_call)) BT_Phase2BigPokeball(u8 taskId);
bool8 __attribute__((long_call)) BT_Phase2BigPokeball_UpdateWave1IncEva(struct Task *task);
bool8 __attribute__((long_call)) BT_Phase2BigPokeball_UpdateWave2DecEvb(struct Task *task);
bool8 __attribute__((long_call)) BT_Phase2BigPokeball_UpdateWave3(struct Task *task);
bool8 __attribute__((long_call)) BT_Phase2BigPokeball_CircleEffect(struct Task *task);
void __attribute__((long_call)) VBCB_BT_Phase2BigPokeball1(void);
void __attribute__((long_call)) VBCB_BT_Phase2BigPokeball2(void);
void __attribute__((long_call)) BT_Phase2SlidingPokeballs(u8 taskId);
void __attribute__((long_call)) BT_Phase2ClockwiseBlackFade(u8 taskId);
void __attribute__((long_call)) BT_Phase2FullScreenWave(u8 taskId);
void __attribute__((long_call)) BT_Phase2BlackWaveToRight(u8 taskId);
void __attribute__((long_call)) BT_Phase2SlicedScreen(u8 taskId);
void __attribute__((long_call)) BT_Phase2WhiteFadeInStripes(u8 taskId);
void __attribute__((long_call)) BT_Phase2GridSquares(u8 taskId);
void __attribute__((long_call)) BT_Phase2BlackDoodles(u8 taskId);
void __attribute__((long_call)) BT_Phase2StartLoreleiMugshot(u8 taskId);
void __attribute__((long_call)) BT_Phase2StartBrunoMugshot(u8 taskId);
void __attribute__((long_call)) BT_Phase2StartAgathaMugshot(u8 taskId);
void __attribute__((long_call)) BT_Phase2StartLanceMugshot(u8 taskId);
void __attribute__((long_call)) BT_Phase2StartBlueMugshot(u8 taskId);
void __attribute__((long_call)) BT_Phase2AntiClockwiseSpiral(u8 taskId);
void __attribute__((long_call)) BT_SetSpriteAsOpponentOrPlayer(s16 spriteId, s16 value);
void __attribute__((long_call)) IncreaseMugshotFuncState(s16 spriteId);
s16 __attribute__((long_call)) IsOpponentMugshotDoneSlidingRight(s16 spriteId);
void __attribute__((long_call)) HBCB_BT_Phase2Mugshot(void);
void __attribute__((long_call)) BT_GetBg0TilemapAndTilesetBase(u16** a0, u16** a1);
void __attribute__((long_call)) SpiteCB_Mugshot(struct Sprite* sprite);
void __attribute__((long_call)) BT_InitCtrlBlk(void);
void __attribute__((long_call)) BT_BlendPalettesToBlack(void);
void __attribute__((long_call)) BT_LoadWaveIntoBuffer(u16 *buffer, s16 offset, s16 theta, s16 frequency, s16 amplitude, s16 bufSize);
void __attribute__((long_call)) BT_GenerateCircle(u16* buffer, s16 x, s16 y, s16 radius);

//This file's functions:
static u8 CreateMugshotTrainerSprite(u8 trainerPicId, s16 x, s16 y, u8 subpriority, u8* buffer, bool8 loadingPlayer);
static void UpdateMugshotSpriteTemplate(struct SpriteTemplate* spriteTemplate, u16 tag);
static void UpdateMugshotSpriteData(u8 spriteId, u8 shape, u16 scaleX, u16 scaleY, u16 rotation, bool8 setScale);
static void BT_Phase2CustomLogo(u8 taskId);
static bool8 BT_Phase2CustomLogo_Init(struct Task *task);
static bool8 BT_Phase2CustomLogo_LoadTilemapAndWave(struct Task *task);
static bool8 BT_Phase2_FramesCountdown(struct Task *task);
static bool8 BT_Phase2CustomLogo_CircleEffect(struct Task *task);

//For pre-battle mugshots
///////////////////////////////////////////////////////////////////////////////////
static u8 CreateMugshotTrainerSprite(u8 trainerPicId, s16 x, s16 y, u8 subpriority, u8* buffer, bool8 loadingPlayer)
{
	struct SpriteTemplate spriteTemplate;

	u16 mugshotSprite = VarGet(VAR_PRE_BATTLE_MUGSHOT_SPRITE);

	switch (mugshotSprite)
	{
		case MUGSHOT_VS_SYMBOL:
			if (loadingPlayer)
			{
				struct CompressedSpritePalette pal = {VS_SpritePal, gTrainerFrontPicPaletteTable[trainerPicId].tag};
				struct CompressedSpriteSheet sprite = {VS_SpriteTiles, 64 * 64 / 2, gTrainerFrontPicTable[trainerPicId].tag};
				LoadCompressedSpritePaletteOverrideBuffer(&pal, buffer);
				LoadCompressedSpriteSheetOverrideBuffer(&sprite, buffer);
			}
			else
			{
				#ifdef FLAG_LOAD_MUGSHOT_SPRITE_FROM_TABLE
				if (FlagGet(FLAG_LOAD_MUGSHOT_SPRITE_FROM_TABLE))
				{
					if (IS_VALID_TABLE_SPRITE(trainerPicId)) //Complete data for image
					{
						struct CompressedSpritePalette pal = {sPreBattleMugshotSprites[trainerPicId].pal, gTrainerFrontPicPaletteTable[trainerPicId].tag};
						struct CompressedSpriteSheet sprite = {sPreBattleMugshotSprites[trainerPicId].sprite, sPreBattleMugshotSprites[trainerPicId].size, gTrainerFrontPicTable[trainerPicId].tag};
						LoadCompressedSpritePaletteOverrideBuffer(&pal, buffer);
						LoadCompressedSpriteSheetOverrideBuffer(&sprite, buffer);
						x += sPreBattleMugshotSprites[trainerPicId].x;
						y += sPreBattleMugshotSprites[trainerPicId].y;
					}
					else
					{
						LoadCompressedSpritePaletteOverrideBuffer(&gTrainerFrontPicPaletteTable[trainerPicId], buffer);
						LoadCompressedSpriteSheetOverrideBuffer(&gTrainerFrontPicTable[trainerPicId], buffer);
					}
				}
				else
				#endif
				{
					LoadCompressedSpritePaletteOverrideBuffer(&gTrainerFrontPicPaletteTable[trainerPicId], buffer);
					LoadCompressedSpriteSheetOverrideBuffer(&gTrainerFrontPicTable[trainerPicId], buffer);
				}
			}
			break;

		case MUGSHOT_PLAYER:
		default:
			#ifdef FLAG_LOAD_MUGSHOT_SPRITE_FROM_TABLE
			if (FlagGet(FLAG_LOAD_MUGSHOT_SPRITE_FROM_TABLE))
			{
				if (IS_VALID_TABLE_SPRITE(trainerPicId)) //Complete data for image
				{
					struct CompressedSpritePalette pal = {sPreBattleMugshotSprites[trainerPicId].pal, gTrainerFrontPicPaletteTable[trainerPicId].tag};
					struct CompressedSpriteSheet sprite = {sPreBattleMugshotSprites[trainerPicId].sprite, sPreBattleMugshotSprites[trainerPicId].size, gTrainerFrontPicTable[trainerPicId].tag};
					LoadCompressedSpritePaletteOverrideBuffer(&pal, buffer);
					LoadCompressedSpriteSheetOverrideBuffer(&sprite, buffer);
					x += sPreBattleMugshotSprites[trainerPicId].x;
					y += sPreBattleMugshotSprites[trainerPicId].y;
				}
				else
					goto REGULAR_PLAYER_SPRITE;
			}
			else
			#endif
			{
				#ifdef FLAG_LOAD_MUGSHOT_SPRITE_FROM_TABLE
				REGULAR_PLAYER_SPRITE: ;
				#endif
				struct SpritePalette dest;
				LoadCompressedSpriteSheetOverrideBuffer(&gTrainerFrontPicTable[trainerPicId], buffer);
				LZ77UnCompWram(GetTrainerSpritePal(trainerPicId), buffer);
				dest.data = (void*) buffer;
				dest.tag = gTrainerFrontPicPaletteTable[trainerPicId].tag;
				unusedArg u8 index = LoadSpritePalette(&dest);

				#ifdef UNBOUND
				TryModifyMugshotTrainerPicPal(trainerPicId, index);
				#endif
			}
			break;
	}

	UpdateMugshotSpriteTemplate(&spriteTemplate, gTrainerFrontPicTable[trainerPicId].tag);
	return CreateSprite(&spriteTemplate, x, y, subpriority);
}

bool8 BT_Phase2Mugshot_LoadGfx(struct Task* task)
{
	s16 i, j;
	u16 *dst1, *dst2;
	const u16* mugshotsMap;

	u8 mugshotStyle = VarGet(VAR_PRE_BATTLE_MUGSHOT_STYLE);
	switch (mugshotStyle)
	{
		case MUGSHOT_BIG:
			mugshotsMap = Big_MugshotMap;
			BT_GetBg0TilemapAndTilesetBase(&dst1, &dst2);
			CpuSet(Big_MugshotTiles, dst2, 0xF0);
			break;

		case MUGSHOT_DP:
			mugshotsMap = DP_MugshotMap;
			BT_GetBg0TilemapAndTilesetBase(&dst1, &dst2);
			CpuSet(DP_MugshotTiles, dst2, 0xF0);
			break;

		case MUGSHOT_TWO_BARS:
		default:
			mugshotsMap = TwoBars_MugshotMap;
			BT_GetBg0TilemapAndTilesetBase(&dst1, &dst2);
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

	for (i = 0; i < 20; ++i)
	{
		for (j = 0; j < 32; ++j, ++mugshotsMap)
			dst1[i * 32 + j] = *mugshotsMap | 0xF000;
	}

	EnableInterrupts(INTR_FLAG_HBLANK);
	SetHBlankCallback(HBCB_BT_Phase2Mugshot);
	task->tState++;
	return FALSE;
}

void Mugshots_CreateOpponentPlayerSprites(struct Task* task)
{
	struct Sprite *opponentSprite, *playerSprite;
	s16 mugshotId = task->tMugshotId;
	u8 trainerPicId, trainerPicId2, trainerPicIdPartner;
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

	//Check Centered VS Symbol
	if (mugshotSprite == MUGSHOT_PLAYER && mugshotType == MUGSHOT_TWO_BARS)
	{
		struct SpriteTemplate spriteTemplate;

		struct CompressedSpritePalette pal = {VS_SpritePal, GFX_TAG_VS_SYMBOL};
		struct CompressedSpriteSheet sprite = {VS_SpriteTiles, 64 * 64 / 2, GFX_TAG_VS_SYMBOL};
		LoadCompressedSpritePaletteOverrideBuffer(&pal, gDecompressionBuffer);
		LoadCompressedSpriteSheetOverrideBuffer(&sprite, gDecompressionBuffer);
		UpdateMugshotSpriteTemplate(&spriteTemplate, GFX_TAG_VS_SYMBOL); //Not being used currently
		task->tVSSymbol = CreateSprite(&spriteTemplate, sMugshotsOpponentCoords[mugshotId][0] - x1 - 108, 50, 0);
		UpdateMugshotSpriteData(task->tVSSymbol, SPRITE_SHAPE(64x64), 256, 256, 0, TRUE);
	}

	//Load Opponent A
	if (sTrainerEventObjectLocalId != 0 //Used for mugshots
	#ifdef FR_PRE_BATTLE_MUGSHOT_STYLE
	|| gTrainers[gTrainerBattleOpponent_A].trainerClass == CLASS_CHAMPION
	|| gTrainers[gTrainerBattleOpponent_A].trainerClass == CLASS_ELITE_FOUR
	#endif
	)
		trainerPicId = GetFrontierTrainerFrontSpriteId(gTrainerBattleOpponent_A, 0);
	else
		trainerPicId = sMugshotsTrainerPicIDsTable[mugshotId];

	task->tOpponentSpriteId = CreateMugshotTrainerSprite(trainerPicId,
														sMugshotsOpponentCoords[mugshotId][0] - x1,
														sMugshotsOpponentCoords[mugshotId][1] + y1,
														0, gDecompressionBuffer, FALSE);

	//Load Opponent B
	if (IsTrainerBattleModeAgainstTwoOpponents())
	{
		trainerPicId2 = GetFrontierTrainerFrontSpriteId(VarGet(VAR_SECOND_OPPONENT), 1);
		task->tOpponentSpriteId2 = CreateMugshotTrainerSprite(trainerPicId2,
															sMugshotsOpponentCoords[mugshotId][0] - x1 - 50,
															sMugshotsOpponentCoords[mugshotId][1] + y1,
															0, gDecompressionBuffer, FALSE);

		if (mugshotType == MUGSHOT_BIG)
			UpdateMugshotSpriteData(task->tOpponentSpriteId2, SPRITE_SHAPE(64x64), sMugshotsOpponentRotationScales[mugshotId][0], sMugshotsOpponentRotationScales[mugshotId][1], 0, TRUE);
		#ifdef FLAG_LOAD_MUGSHOT_SPRITE_FROM_TABLE
		else if (FlagGet(FLAG_LOAD_MUGSHOT_SPRITE_FROM_TABLE))
		{
			if (!IS_VALID_TABLE_SPRITE(trainerPicId2))
				UpdateMugshotSpriteData(task->tOpponentSpriteId2, SPRITE_SHAPE(64x32), sMugshotsOpponentRotationScales[mugshotId][0], sMugshotsOpponentRotationScales[mugshotId][1], 0, TRUE);
			else
				UpdateMugshotSpriteData(task->tOpponentSpriteId2, SPRITE_SHAPE(64x64), 0, 0, 0, FALSE);
		}
		#endif
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
			trainerPicIdPartner = (GetFrontierTrainerFrontSpriteId(gTrainerBattlePartner, 2));
			task->tPartnerSpriteId = CreateMugshotTrainerSprite(trainerPicIdPartner, x2 + 50, y2, 0, gDecompressionBuffer, FALSE);

			if (mugshotType == MUGSHOT_BIG)
				UpdateMugshotSpriteData(task->tPartnerSpriteId, SPRITE_SHAPE(64x64), -512, 512, 0, TRUE);
			#ifdef FLAG_LOAD_MUGSHOT_SPRITE_FROM_TABLE
			else if (FlagGet(FLAG_LOAD_MUGSHOT_SPRITE_FROM_TABLE))
			{
				if (!IS_VALID_TABLE_SPRITE(trainerPicIdPartner))
					UpdateMugshotSpriteData(task->tPartnerSpriteId, SPRITE_SHAPE(64x32), -512, 512, 0, TRUE);
				else
					UpdateMugshotSpriteData(task->tPartnerSpriteId, SPRITE_SHAPE(64x64), 0, 0, 0, FALSE);
			}
			#endif
			else
			{
				UpdateMugshotSpriteData(task->tPartnerSpriteId, SPRITE_SHAPE(64x32), -512, 512, 0, TRUE);
			}
		}
	}
	else
	{
		trainerPicIdPartner = 0; //So the compiler doesn't complain
		++trainerPicIdPartner;
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
			#ifdef FLAG_LOAD_MUGSHOT_SPRITE_FROM_TABLE
			if (FlagGet(FLAG_LOAD_MUGSHOT_SPRITE_FROM_TABLE))
				opponentSprite->oam.shape = SPRITE_SHAPE(64x64);
			else
			#endif
				opponentSprite->oam.shape = SPRITE_SHAPE(64x32);
		}
		#ifdef FLAG_LOAD_MUGSHOT_SPRITE_FROM_TABLE
		else if (FlagGet(FLAG_LOAD_MUGSHOT_SPRITE_FROM_TABLE))
		{
			if (IS_VALID_TABLE_SPRITE(trainerPicId))
				opponentSprite->oam.shape = SPRITE_SHAPE(64x64);
			else
			{
				opponentSprite->oam.shape = SPRITE_SHAPE(64x32);
				playerSprite->oam.shape = SPRITE_SHAPE(64x64); //Gets shrunk down later
			}
		}
		#endif
	}
	#ifdef FLAG_LOAD_MUGSHOT_SPRITE_FROM_TABLE
	else if (FlagGet(FLAG_LOAD_MUGSHOT_SPRITE_FROM_TABLE))
	{
		if (IS_VALID_TABLE_SPRITE(PlayerGenderToFrontTrainerPicId(gSaveBlock2->playerGender, TRUE)))
			playerSprite->oam.shape = SPRITE_SHAPE(64x64);
		else
			playerSprite->oam.shape = SPRITE_SHAPE(64x32);

		if (IS_VALID_TABLE_SPRITE(trainerPicId))
			opponentSprite->oam.shape = SPRITE_SHAPE(64x64);
		else
			opponentSprite->oam.shape = SPRITE_SHAPE(64x32);
	}
	#endif
	else
	{
		opponentSprite->oam.shape = SPRITE_SHAPE(64x32);
		playerSprite->oam.shape = SPRITE_SHAPE(64x32);
	}

	opponentSprite->oam.size = SPRITE_SIZE(64x64);
	playerSprite->oam.size = SPRITE_SIZE(64x64);

	CalcCenterToCornerVec(opponentSprite, 1, 3, 3);
	CalcCenterToCornerVec(playerSprite, 1, 3, 3);

	#ifdef FLAG_LOAD_MUGSHOT_SPRITE_FROM_TABLE
	if ((FlagGet(FLAG_LOAD_MUGSHOT_SPRITE_FROM_TABLE)))
	{
		if (!IS_VALID_TABLE_SPRITE(trainerPicId))
			SetOamMatrixRotationScaling(opponentSprite->oam.matrixNum, sMugshotsOpponentRotationScales[mugshotId][0], sMugshotsOpponentRotationScales[mugshotId][1], 0);
	}
	else
	#endif
	{
		SetOamMatrixRotationScaling(opponentSprite->oam.matrixNum, sMugshotsOpponentRotationScales[mugshotId][0], sMugshotsOpponentRotationScales[mugshotId][1], 0);
	}

	if (mugshotType == MUGSHOT_BIG && mugshotSprite == MUGSHOT_VS_SYMBOL)
		SetOamMatrixRotationScaling(playerSprite->oam.matrixNum, 448, 448, 0);
	else if (mugshotSprite == MUGSHOT_VS_SYMBOL)
		SetOamMatrixRotationScaling(playerSprite->oam.matrixNum, 256, 256, 0);
	#ifdef FLAG_LOAD_MUGSHOT_SPRITE_FROM_TABLE
	else if (FlagGet(FLAG_LOAD_MUGSHOT_SPRITE_FROM_TABLE))
	{
		if (!IS_VALID_TABLE_SPRITE(PlayerGenderToFrontTrainerPicId(gSaveBlock2->playerGender, TRUE)))
			SetOamMatrixRotationScaling(playerSprite->oam.matrixNum, -512, 512, 0);
	}
	#endif
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
bool8 BT_Phase2Mugshot_StartSpriteSlide(struct Task* task)
{
	u8 i;
	u16* winVal;

	sTransitionStructPtr->vblankDma = FALSE;
	for (i = 0, winVal = gScanlineEffectRegBuffers[0]; i < 160; ++i, ++winVal)
		*winVal = 0xF0;

	task->tState++;
	task->tData1 = 0;
	task->tData2 = 0;
	task->tData3 = 0;
	sTransitionStructPtr->bg0HOfsOpponent -= 8;
	sTransitionStructPtr->bg0HOfsPlayer += 8;

	BT_SetSpriteAsOpponentOrPlayer(task->tOpponentSpriteId, 0);

	if (IsTrainerBattleModeAgainstTwoOpponents())
	{
		BT_SetSpriteAsOpponentOrPlayer(task->tOpponentSpriteId2, 0);
		gSprites[task->tOpponentSpriteId].data[5] = 1;
		gSprites[task->tOpponentSpriteId2].data[5] = 2;
	}

	BT_SetSpriteAsOpponentOrPlayer(task->tPlayerSpriteId, 1);

	u16 mugshotType = VarGet(VAR_PRE_BATTLE_MUGSHOT_STYLE);
	u16 mugshotSprite = VarGet(VAR_PRE_BATTLE_MUGSHOT_SPRITE);

	if (mugshotSprite == MUGSHOT_PLAYER && mugshotType == MUGSHOT_TWO_BARS)
	{
		BT_SetSpriteAsOpponentOrPlayer(task->tVSSymbol, 0);
		gSprites[task->tVSSymbol].data[5] = 3;
	}

	if (mugshotSprite != MUGSHOT_VS_SYMBOL)
	{
		if (IsTrainerBattleModeWithPartner())
		{
			BT_SetSpriteAsOpponentOrPlayer(task->tPartnerSpriteId, 1);
			gSprites[task->tPlayerSpriteId].data[5] = 1;
			gSprites[task->tPartnerSpriteId].data[5] = 2;
		}
	}

	IncreaseMugshotFuncState(task->tOpponentSpriteId);
	if (IsTrainerBattleModeAgainstTwoOpponents())
		IncreaseMugshotFuncState(task->tOpponentSpriteId2);

	if (mugshotSprite == MUGSHOT_PLAYER && mugshotType == MUGSHOT_TWO_BARS)	//centered vs symbol
		IncreaseMugshotFuncState(task->tVSSymbol);

	PlaySE(SE_BT_START);

	sTransitionStructPtr->vblankDma++;
	return FALSE;
}

bool8 BT_Phase2Mugshot_WaitForOpponentInPlace(struct Task *task)
{
	sTransitionStructPtr->bg0HOfsOpponent -= 8;
	sTransitionStructPtr->bg0HOfsPlayer += 8;
	if (IsOpponentMugshotDoneSlidingRight(task->tOpponentSpriteId))
	{
		task->tState++;
		IncreaseMugshotFuncState(task->tPlayerSpriteId);
		IncreaseMugshotFuncState(task->tPartnerSpriteId);
	}

	return FALSE;
}

//Fixes a bug where the DNS causes white fading issues after the mugshot
bool8 BT_Phase2Mugshot_StartBlackFade(struct Task *task)
{
	sTransitionStructPtr->vblankDma = FALSE;
	gDontFadeWhite = TRUE;
	BlendPalettes(-1, 0x10, 0x7FFF);
	sTransitionStructPtr->bldCnt = 0xFF;
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

//For custom transitions
///////////////////////////////////////////////////////////////////////////////////

const TaskFunc sBT_Phase1Tasks[] =
{
	[B_TRANSITION_BLUR] = BT_Phase1Task,
	[B_TRANSITION_DISTORTED_WAVE] = BT_Phase1Task,
	[B_TRANSITION_HORIZONTAL_CORRUGATE] = BT_Phase1Task,
	[B_TRANSITION_BIG_POKEBALL] = BT_Phase1Task,
	[B_TRANSITION_SLIDING_POKEBALLS] = BT_Phase1Task,
	[B_TRANSITION_CLOCKWISE_BLACKFADE] = BT_Phase1Task,
	[B_TRANSITION_FULLSCREEN_WAVE] = BT_Phase1Task,
	[B_TRANSITION_BLACK_WAVE_TO_RIGHT] = BT_Phase1Task,
	[B_TRANSITION_SLICED_SCREEN] = BT_Phase1Task,
	[B_TRANSITION_WHITEFADE_IN_STRIPES] = BT_Phase1Task,
	[B_TRANSITION_GRID_SQUARES] = BT_Phase1Task,
	[B_TRANSITION_BLACK_DOODLES] = BT_Phase1Task,
	[B_TRANSITION_LORELEI] = BT_Phase1Task,
	[B_TRANSITION_BRUNO] = BT_Phase1Task,
	[B_TRANSITION_AGATHA] = BT_Phase1Task,
	[B_TRANSITION_LANCE] = BT_Phase1Task,
	[B_TRANSITION_CHAMPION] = BT_Phase1Task,
	[B_TRANSITION_ANTI_CLOCKWISE_SPIRAL] = BT_Phase1Task,
	[B_TRANSITION_CUSTOM_LOGO] = BT_Phase1Task,
};

const TaskFunc sBT_Phase2Tasks[] =
{
	[B_TRANSITION_BLUR] = BT_Phase2Blur,
	[B_TRANSITION_DISTORTED_WAVE] = BT_Phase2DistortedWave,
	[B_TRANSITION_HORIZONTAL_CORRUGATE] = BT_Phase2HorizontalCorrugate,
	[B_TRANSITION_BIG_POKEBALL] = BT_Phase2BigPokeball,
	[B_TRANSITION_SLIDING_POKEBALLS] = BT_Phase2SlidingPokeballs,
	[B_TRANSITION_CLOCKWISE_BLACKFADE] = BT_Phase2ClockwiseBlackFade,
	[B_TRANSITION_FULLSCREEN_WAVE] = BT_Phase2FullScreenWave,
	[B_TRANSITION_BLACK_WAVE_TO_RIGHT] = BT_Phase2BlackWaveToRight,
	[B_TRANSITION_SLICED_SCREEN] = BT_Phase2SlicedScreen,
	[B_TRANSITION_WHITEFADE_IN_STRIPES] = BT_Phase2WhiteFadeInStripes,
	[B_TRANSITION_GRID_SQUARES] = BT_Phase2GridSquares,
	[B_TRANSITION_BLACK_DOODLES] = BT_Phase2BlackDoodles,
	[B_TRANSITION_LORELEI] = BT_Phase2StartLoreleiMugshot,
	[B_TRANSITION_BRUNO] = BT_Phase2StartBrunoMugshot,
	[B_TRANSITION_AGATHA] = BT_Phase2StartAgathaMugshot,
	[B_TRANSITION_LANCE] = BT_Phase2StartLanceMugshot,
	[B_TRANSITION_CHAMPION] = BT_Phase2StartBlueMugshot,
	[B_TRANSITION_ANTI_CLOCKWISE_SPIRAL] = BT_Phase2AntiClockwiseSpiral,
	[B_TRANSITION_CUSTOM_LOGO] = BT_Phase2CustomLogo,
};

#ifdef VAR_BATTLE_TRANSITION_LOGO
extern const u8 ShadowBattleLogoTiles[];
extern const u16 ShadowBattleLogoPal[];
extern const u8 ShadowBattleLogoMap[];
extern const u8 LORBattleLogoTiles[];
extern const u16 LORBattleLogoPal[];
extern const u8 LORBattleLogoMap[];
extern const u8 BlackEmboarBattleLogoTiles[];
extern const u16 BlackEmboarBattleLogoPal[];
extern const u8 BlackEmboarBattleLogoMap[];
extern const u8 TerrorGranbullBattleLogoTiles[];
extern const u16 TerrorGranbullBattleLogoPal[];
extern const u8 TerrorGranbullBattleLogoMap[];
extern const u8 BlackFerrothornBattleLogoTiles[];
extern const u16 BlackFerrothornBattleLogoPal[];
extern const u8 BlackFerrothornBattleLogoMap[];
extern const u8 ScienceSocietyBattleLogoTiles[];
extern const u16 ScienceSocietyBattleLogoPal[];
extern const u8 ScienceSocietyBattleLogoMap[];
extern const u8 DynamaxBattleLogoTiles[];
extern const u16 DynamaxBattleLogoPal[];
extern const u8 DynamaxBattleLogoMap[];

const struct BattleTransitionLogo gBattleTransitionLogos[] =
{
	#ifndef UNBOUND //Modify this section
	{},
	#else //For Pokemon Unbound
	{0xFF, DynamaxBattleLogoTiles, DynamaxBattleLogoPal, DynamaxBattleLogoMap},
	{CLASS_SHADOW, ShadowBattleLogoTiles, ShadowBattleLogoPal, ShadowBattleLogoMap},
	{CLASS_LOR, LORBattleLogoTiles, LORBattleLogoPal, LORBattleLogoMap},
	{CLASS_TERROR_GRANBULL, TerrorGranbullBattleLogoTiles, TerrorGranbullBattleLogoPal, TerrorGranbullBattleLogoMap},
	{CLASS_BLACK_FERROTHORN, BlackFerrothornBattleLogoTiles, BlackFerrothornBattleLogoPal, BlackFerrothornBattleLogoMap},
	{CLASS_BLACK_FERROTHORN_BOSS, BlackFerrothornBattleLogoTiles, BlackFerrothornBattleLogoPal, BlackFerrothornBattleLogoMap},
	{CLASS_BLACK_EMBOAR, BlackEmboarBattleLogoTiles, BlackEmboarBattleLogoPal, BlackEmboarBattleLogoMap},
	{CLASS_SCIENCE_SOCIETY, ScienceSocietyBattleLogoTiles, ScienceSocietyBattleLogoPal, ScienceSocietyBattleLogoMap},
	#endif
};

const u8 gNumBattleTransitionLogos = NELEMS(gBattleTransitionLogos);
#endif

#undef tTheta
#undef tAmplitude

#define tEvb data[1]
#define tEva data[2]
#define tInterval data[3]
#define tTheta data[4]
#define tAmplitude data[5]
#define tFrames data[8]

static const TransitionStateFunc sBT_Phase2CustomLogoFuncs[] =
{
	BT_Phase2CustomLogo_Init,
	BT_Phase2CustomLogo_LoadTilemapAndWave,
	BT_Phase2BigPokeball_UpdateWave1IncEva,
	BT_Phase2BigPokeball_UpdateWave2DecEvb,
	BT_Phase2BigPokeball_UpdateWave3,
	BT_Phase2_FramesCountdown,
	BT_Phase2CustomLogo_CircleEffect,
};

static void BT_Phase2CustomLogo(u8 taskId)
{
	while (sBT_Phase2CustomLogoFuncs[gTasks[taskId].tState](&gTasks[taskId]));
}

static bool8 BT_Phase2CustomLogo_Init(struct Task *task)
{
	#ifdef VAR_BATTLE_TRANSITION_LOGO
	u16 i, *tilemapAddr, *tilesetAddr;
	const u8* customLogoTiles;
	const u16* customLogoPal;

	u16 logoId = VarGet(VAR_BATTLE_TRANSITION_LOGO);
	customLogoTiles = gBattleTransitionLogos[logoId].tiles;
	customLogoPal = gBattleTransitionLogos[logoId].pal;

	BT_InitCtrlBlk();
	ScanlineEffect_Clear();
	task->tEvb = 16;
	task->tEva = 0;
	task->tTheta = 0;
	task->tAmplitude = 0x4000;
	task->tFrames = 30; //Show logo for half a second
	sTransitionStructPtr->winIn = WININ_WIN0_BG_ALL | WININ_WIN0_OBJ | WININ_WIN0_CLR;
	sTransitionStructPtr->winOut = 0;
	sTransitionStructPtr->win0H = WIN_RANGE(0, 0xF0);
	sTransitionStructPtr->win0V = WIN_RANGE(0, 0xA0);
	sTransitionStructPtr->bldCnt = BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BG0 | BLDCNT_TGT2_BG1 | BLDCNT_TGT2_BG2 | BLDCNT_TGT2_BG3 | BLDCNT_TGT2_OBJ | BLDCNT_TGT2_BD;
	sTransitionStructPtr->bldAlpha = (task->tEvb << 8) | task->tEva;

	for (i = 0; i < 160; ++i)
		gScanlineEffectRegBuffers[1][i] = 0xF0;

	SetVBlankCallback(VBCB_BT_Phase2BigPokeball1);
	BT_GetBg0TilemapAndTilesetBase(&tilemapAddr, &tilesetAddr);
	CpuFill16(0, tilemapAddr, 0x800);
	LZDecompressVram(customLogoTiles, tilesetAddr);
	LoadPalette(customLogoPal, 0xF0, 0x20);
	#endif

	++task->tState;
	return FALSE;
}

static bool8 BT_Phase2CustomLogo_LoadTilemapAndWave(struct Task *task)
{
	if (!IsDma3ManagerBusyWithBgCopy())
	{
		#ifdef VAR_BATTLE_TRANSITION_LOGO
		u16 *tilemapAddr, *tilesetAddr;
		u16 logoId = VarGet(VAR_BATTLE_TRANSITION_LOGO);

		BT_GetBg0TilemapAndTilesetBase(&tilemapAddr, &tilesetAddr);
		LZDecompressVram(gBattleTransitionLogos[logoId].map, tilemapAddr);
		BT_LoadWaveIntoBuffer(gScanlineEffectRegBuffers[0], 0, task->tTheta, 132, task->tAmplitude, 160);
		#endif

		++task->tState;
		return TRUE;
	}
	
	return TRUE;
}

static bool8 BT_Phase2_FramesCountdown(struct Task *task)
{
    if (--task->tFrames == 0)
        task->tState++;

    return FALSE;
}

#undef tEvb
#undef tEva
#undef tInterval

#define tRadius data[1]
#define tDeltaRadius data[2]
#define tKeepVBCB data[3]

static bool8 BT_Phase2CustomLogo_CircleEffect(struct Task *task)
{
    sTransitionStructPtr->vblankDma = FALSE;

    if (task->tDeltaRadius < 2048)
        task->tDeltaRadius += 256;

    if (task->tRadius)
    {
        task->tRadius -= (task->tDeltaRadius >> 8);
        if (task->tRadius < 0)
            task->tRadius = 0;
    }

    BT_GenerateCircle(gScanlineEffectRegBuffers[0], 120, 80, task->tRadius);
    if (task->tRadius == 0)
    {
        DmaStop(0);
        BT_BlendPalettesToBlack();
        DestroyTask(FindTaskIdByFunc(BT_Phase2CustomLogo));
    }

    if (task->tKeepVBCB == 0)
    {
        ++task->tKeepVBCB;
        SetVBlankCallback(VBCB_BT_Phase2BigPokeball2);
    }

    ++sTransitionStructPtr->vblankDma;
    return FALSE;
}
