#include "defines.h"
#include "../include/bg.h"
#include "../include/event_data.h"
#include "../include/field_weather.h"
#include "../include/gpu_regs.h"
#include "../include/menu.h"
#include "../include/overworld.h"
#include "../include/palette.h"
#include "../include/region_map.h"
#include "../include/scanline_effect.h"
#include "../include/string_util.h"
#include "../include/text.h"
#include "../include/text_window.h"
#include "../include/window.h"
#include "../include/constants/songs.h"
#include "../include/gba/io_reg.h"

#include "../include/new/dns.h"
#include "../include/new/frontier.h"

/*
frontier_records.c
	Displays player's records in the Battle Frontier.
*/

#define BRONZE_STAR_STREAK 20
#define SILVER_STAR_STREAK 50
#define GOLD_STAR_STREAK 100
#define STAR_SPRITE_COUNT 4

struct FrontierRecords
{
	u8* tilemapPtr;
	const u8* tierList;
	u8 battleType;
	u8 battleTier;
	u8 battleTierId;
	u8 numTiers;
	u8 facilityNum;
	u16 scrollArrowDummy;
	u8 starSpriteIds[STAR_SPRITE_COUNT];
};

#define sFrontierRecordsPtr (*((struct FrontierRecords**) 0x203E038))

extern const u8 gInterfaceGfx_FrontierRecordsTiles[];
extern const u8 gInterfaceGfx_FrontierRecordsPal[];
extern const u8 gInterfaceGfx_FrontierRecordsMap[];
extern const u8 Frontier_Record_BronzeStarTiles[];
extern const u8 Frontier_Record_BronzeStarPal[];
extern const u8 Frontier_Record_SilverStarPal[];
extern const u8 Frontier_Record_GoldStarPal[];

extern const u8 gText_BattleTower[];
extern const u8 gText_SingleBattleRecord[];
extern const u8 gText_DoubleBattleRecord[];
extern const u8 gText_MultiBattleRecord[];
extern const u8 gText_LinkMultiBattleRecord[];
extern const u8 gText_SingleRandomBattleRecord[];
extern const u8 gText_DoubleRandomBattleRecord[];
extern const u8 gText_MultiRandomBattleRecord[];
extern const u8 gText_FrontierRecordLevel5[];
extern const u8 gText_FrontierRecordLevel50[];
extern const u8 gText_FrontierRecordLevel100[];
extern const u8 gText_CurrentStreak[];
extern const u8 gText_MaxStreak[];
extern const u8 gText_3v3[];
extern const u8 gText_4v4[];
extern const u8 gText_6v6[];

enum Windows
{
	WIN_BATTLE_FACILITY_NAME,
	WIN_BATTLE_TYPE,
	WIN_LEVEL_50,
	WIN_3V3_LEVEL_50,
	WIN_6V6_LEVEL_50,
	WIN_CURRENT_STREAK_LEVEL_50,
	WIN_MAX_STREAK_LEVEL_50,
	WIN_LEVEL_100,
	WIN_3V3_LEVEL_100,
	WIN_6V6_LEVEL_100,
	WIN_CURRENT_STREAK_LEVEL_100,
	WIN_MAX_STREAK_LEVEL_100,
	WIN_TIER,

//Buffered Data
	WIN_CURRENT_STREAK_3V3_LEVEL_50,
	WIN_MAX_STREAK_3V3_LEVEL_50,
	WIN_CURRENT_STREAK_6V6_LEVEL_50,
	WIN_MAX_STREAK_6V6_LEVEL_50,
	WIN_CURRENT_STREAK_3V3_LEVEL_100,
	WIN_MAX_STREAK_3V3_LEVEL_100,
	WIN_CURRENT_STREAK_6V6_LEVEL_100,
	WIN_MAX_STREAK_6V6_LEVEL_100,
	WINDOW_COUNT,
};

static const struct WindowTemplate sFrontierRecordsWinTemplates[WINDOW_COUNT + 1] =
{
	[WIN_BATTLE_FACILITY_NAME] =
	{
		.bg = 0,
		.tilemapLeft = 1,
		.tilemapTop = 0,
		.width = 10,
		.height = 3,
		.paletteNum = 15,
		.baseBlock = 1,
	},
	[WIN_BATTLE_TYPE] =
	{
		.bg = 0,
		.tilemapLeft = 12,
		.tilemapTop = 0,
		.width = 18,
		.height = 3,
		.paletteNum = 15,
		.baseBlock = 31,
	},
	[WIN_LEVEL_50] =
	{
		.bg = 0,
		.tilemapLeft = 2,
		.tilemapTop = 5,
		.width = 6,
		.height = 2,
		.paletteNum = 15,
		.baseBlock = 85,
	},
	[WIN_3V3_LEVEL_50] =
	{
		.bg = 0,
		.tilemapLeft = 15,
		.tilemapTop = 5,
		.width = 4,
		.height = 2,
		.paletteNum = 15,
		.baseBlock = 97,
	},
	[WIN_6V6_LEVEL_50] =
	{
		.bg = 0,
		.tilemapLeft = 23,
		.tilemapTop = 5,
		.width = 4,
		.height = 2,
		.paletteNum = 15,
		.baseBlock = 105,
	},
	[WIN_CURRENT_STREAK_LEVEL_50] =
	{
		.bg = 0,
		.tilemapLeft = 2,
		.tilemapTop = 7,
		.width = 10,
		.height = 2,
		.paletteNum = 15,
		.baseBlock = 113,
	},
	[WIN_MAX_STREAK_LEVEL_50] =
	{
		.bg = 0,
		.tilemapLeft = 2,
		.tilemapTop = 9,
		.width = 10,
		.height = 2,
		.paletteNum = 15,
		.baseBlock = 133,
	},
	[WIN_LEVEL_100] =
	{
		.bg = 0,
		.tilemapLeft = 2,
		.tilemapTop = 12,
		.width = 6,
		.height = 2,
		.paletteNum = 15,
		.baseBlock = 153,
	},
	[WIN_3V3_LEVEL_100] =
	{
		.bg = 0,
		.tilemapLeft = 15,
		.tilemapTop = 12,
		.width = 4,
		.height = 2,
		.paletteNum = 15,
		.baseBlock = 165,
	},
	[WIN_6V6_LEVEL_100] =
	{
		.bg = 0,
		.tilemapLeft = 23,
		.tilemapTop = 12,
		.width = 4,
		.height = 2,
		.paletteNum = 15,
		.baseBlock = 173,
	},
	[WIN_CURRENT_STREAK_LEVEL_100] =
	{
		.bg = 0,
		.tilemapLeft = 2,
		.tilemapTop = 14,
		.width = 10,
		.height = 2,
		.paletteNum = 15,
		.baseBlock = 181,
	},
	[WIN_MAX_STREAK_LEVEL_100] =
	{
		.bg = 0,
		.tilemapLeft = 2,
		.tilemapTop = 16,
		.width = 10,
		.height = 2,
		.paletteNum = 15,
		.baseBlock = 201,
	},
	[WIN_TIER] =
	{
		.bg = 0,
		.tilemapLeft = 2,
		.tilemapTop = 3,
		.width = 18,
		.height = 2,
		.paletteNum = 15,
		.baseBlock = 221,
	},


//Buffered Data Windows
	[WIN_CURRENT_STREAK_3V3_LEVEL_50] =
	{
		.bg = 0,
		.tilemapLeft = 15,
		.tilemapTop = 7,
		.width = 4,
		.height = 2,
		.paletteNum = 15,
		.baseBlock = 257,
	},
	[WIN_MAX_STREAK_3V3_LEVEL_50] =
	{
		.bg = 0,
		.tilemapLeft = 15,
		.tilemapTop = 9,
		.width = 4,
		.height = 2,
		.paletteNum = 15,
		.baseBlock = 265,
	},
	[WIN_CURRENT_STREAK_6V6_LEVEL_50] =
	{
		.bg = 0,
		.tilemapLeft = 23,
		.tilemapTop = 7,
		.width = 4,
		.height = 2,
		.paletteNum = 15,
		.baseBlock = 273,
	},
	[WIN_MAX_STREAK_6V6_LEVEL_50] =
	{
		.bg = 0,
		.tilemapLeft = 23,
		.tilemapTop = 9,
		.width = 4,
		.height = 2,
		.paletteNum = 15,
		.baseBlock = 281,
	},
	[WIN_CURRENT_STREAK_3V3_LEVEL_100] =
	{
		.bg = 0,
		.tilemapLeft = 15,
		.tilemapTop = 14,
		.width = 4,
		.height = 2,
		.paletteNum = 15,
		.baseBlock = 289,
	},
	[WIN_MAX_STREAK_3V3_LEVEL_100] =
	{
		.bg = 0,
		.tilemapLeft = 15,
		.tilemapTop = 16,
		.width = 4,
		.height = 2,
		.paletteNum = 15,
		.baseBlock = 297,
	},
	[WIN_CURRENT_STREAK_6V6_LEVEL_100] =
	{
		.bg = 0,
		.tilemapLeft = 23,
		.tilemapTop = 14,
		.width = 4,
		.height = 2,
		.paletteNum = 15,
		.baseBlock = 305,
	},
	[WIN_MAX_STREAK_6V6_LEVEL_100] =
	{
		.bg = 0,
		.tilemapLeft = 23,
		.tilemapTop = 16,
		.width = 4,
		.height = 2,
		.paletteNum = 15,
		.baseBlock = 313,
	},

	DUMMY_WIN_TEMPLATE,
};

static const struct BgTemplate sFrontierRecordsBgTemplates[] =
{
    { //Card Background
        .bg = 2,
        .charBaseIndex = 0,
        .mapBaseIndex = 31,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 3,
        .baseTile = 0,
    },
    { //Text
        .bg = 1,
        .charBaseIndex = 2,
        .mapBaseIndex = 6,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 1,
        .baseTile = 0,
    },
    { //Instructions
        .bg = 0,
        .charBaseIndex = 1,
        .mapBaseIndex = 24,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0,
    }
};

static const u8* const sBattleTypeStrings[NUM_TOWER_BATTLE_TYPES] =
{
	gText_SingleBattleRecord,
	gText_DoubleBattleRecord,
	gText_MultiBattleRecord,
	gText_LinkMultiBattleRecord,
	gText_SingleRandomBattleRecord,
	gText_DoubleRandomBattleRecord,
	gText_MultiRandomBattleRecord,
};

enum
{
	STAR_TAG_BRONZE = 0x2710,
	STAR_TAG_SILVER,
	STAR_TAG_GOLD,
};

static const struct OamData sStarOAM =
{
	.affineMode = ST_OAM_AFFINE_DOUBLE,
	.objMode = ST_OAM_OBJ_NORMAL,
	.shape = SPRITE_SHAPE(8x8),
	.size = SPRITE_SIZE(8x8),
	.priority = 0, //Above everything
};
static const struct SpriteTemplate sBronzeStarTemplate =
{
	.tileTag = STAR_TAG_BRONZE,
	.paletteTag = STAR_TAG_BRONZE,
	.oam = &sStarOAM,
	.anims = gDummySpriteAnimTable,
	.images = NULL,
	.affineAnims = gDummySpriteAffineAnimTable,
	.callback = SpriteCallbackDummy,
};

static const struct SpriteTemplate sSilverStarTemplate =
{
	.tileTag = STAR_TAG_BRONZE,
	.paletteTag = STAR_TAG_SILVER,
	.oam = &sStarOAM,
	.anims = gDummySpriteAnimTable,
	.images = NULL,
	.affineAnims = gDummySpriteAffineAnimTable,
	.callback = SpriteCallbackDummy,
};

static const struct SpriteTemplate sGoldStarTemplate =
{
	.tileTag = STAR_TAG_BRONZE,
	.paletteTag = STAR_TAG_GOLD,
	.oam = &sStarOAM,
	.anims = gDummySpriteAnimTable,
	.images = NULL,
	.affineAnims = gDummySpriteAffineAnimTable,
	.callback = SpriteCallbackDummy,
};

static const struct CompressedSpriteSheet sStarsSpriteSheet = {Frontier_Record_BronzeStarTiles, (8 * 24) / 2, STAR_TAG_BRONZE};
static const struct CompressedSpritePalette sBronzeStarSpritePalette = {Frontier_Record_BronzeStarPal, STAR_TAG_BRONZE};
static const struct CompressedSpritePalette sSilverStarSpritePalette = {Frontier_Record_SilverStarPal, STAR_TAG_SILVER};
static const struct CompressedSpritePalette sGoldStarSpritePalette = {Frontier_Record_GoldStarPal, STAR_TAG_GOLD};

static const u16 sFrontierRecordsTextColour[] = {0x7FFF, 0x7FFF, 0x318C, 0x675A, 0x043C, 0x3AFF, 0x0664, 0x4BD2, 0x6546, 0x7B14, RGB(21, 23, 23)};

//This file's functions
static void CleanWindows(void);
static void CommitWindows(void);
static void Task_FrontierRecordsFadeOut(u8 taskId);
static void Task_FrontierRecordsWaitForKeyPress(u8 taskId);
static void Task_FrontierRecordsFadeIn(u8 taskId);
static void LoadStarGfx(void);
static void TryCreateStarSprite(s16 x, s16 y, u8 arrayIndex, u16 maxStreak);
static void DestroyStarSprites(void);
static void PrintCurrentRecords(void);
static void CreateScrollArrows(void);
static void MainCB2_FrontierRecords(void);
static void VBlankCB_FrontierRecords(void);
static void InitFrontierRecords(void);
static void LoadFrontierRecordsGfx(void);
static void ClearTasksAndGraphicalStructs(void);
static void ClearVramOamPlttRegs(void);
static void CB2_FrontierRecords(void);
static void Task_InitFrontierRecords(u8 taskId);

//General Util
static void CleanWindows(void)
{
	for (int i = 0; i < WINDOW_COUNT; ++i)
		FillWindowPixelBuffer(i, PIXEL_FILL(0));
}

static void CommitWindows(void)
{
	for (u8 i = 0; i < WINDOW_COUNT; ++i)
	{
		CopyWindowToVram(i, COPYWIN_BOTH);
		PutWindowTilemap(i);
	}
}

//GUI
static void Task_FrontierRecordsFadeOut(u8 taskId)
{
	if (!gPaletteFade->active)
	{
		SetMainCallback2(CB2_ReturnToFieldContinueScript);
		Free(sFrontierRecordsPtr->tilemapPtr);
		Free(sFrontierRecordsPtr);
		FreeAllWindowBuffers();
		DestroyTask(taskId);
	}
}

static void Task_FrontierRecordsWaitForKeyPress(u8 taskId)
{
	if (gMain.newKeys & (A_BUTTON | B_BUTTON))
	{
		PlaySE(SE_PC_OFF);
		BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 16, RGB_BLACK);
		gTasks[taskId].func = Task_FrontierRecordsFadeOut;
	}
	else if (gMain.newKeys & DPAD_LEFT)
	{
		if (sFrontierRecordsPtr->battleTierId == 0)
			sFrontierRecordsPtr->battleTierId = sFrontierRecordsPtr->numTiers; //Wrap around

		--sFrontierRecordsPtr->battleTierId;

		sFrontierRecordsPtr->battleTier = sFrontierRecordsPtr->tierList[sFrontierRecordsPtr->battleTierId];

		DestroyStarSprites();
		PrintCurrentRecords();
		PlaySE(SE_WIN_OPEN);
	}
	else if (gMain.newKeys & DPAD_RIGHT)
	{
		if (sFrontierRecordsPtr->battleTierId == sFrontierRecordsPtr->numTiers - 1)
			sFrontierRecordsPtr->battleTierId = 0; //Wrap around
		else
			++sFrontierRecordsPtr->battleTierId;
		
		sFrontierRecordsPtr->battleTier = sFrontierRecordsPtr->tierList[sFrontierRecordsPtr->battleTierId];

		DestroyStarSprites();
		PrintCurrentRecords();
		PlaySE(SE_WIN_OPEN);
	}
}

static void Task_FrontierRecordsFadeIn(u8 taskId)
{
	if (!gPaletteFade->active)
		gTasks[taskId].func = Task_FrontierRecordsWaitForKeyPress;
}

static void LoadStarGfx(void)
{
	LoadCompressedSpriteSheetUsingHeap(&sStarsSpriteSheet);
	LoadCompressedSpritePaletteUsingHeap(&sBronzeStarSpritePalette);
	LoadCompressedSpritePaletteUsingHeap(&sSilverStarSpritePalette);
	LoadCompressedSpritePaletteUsingHeap(&sGoldStarSpritePalette);
}

static void TryCreateStarSprite(s16 x, s16 y, u8 arrayIndex, u16 maxStreak)
{
	u8 spriteId = MAX_SPRITES;

	if (sFrontierRecordsPtr->starSpriteIds[arrayIndex] == MAX_SPRITES) //Star hasn't been created already
	{
		if (maxStreak >= GOLD_STAR_STREAK)
			spriteId = CreateSprite(&sGoldStarTemplate, x, y, 0);
		else if (maxStreak >= SILVER_STAR_STREAK)
			spriteId = CreateSprite(&sSilverStarTemplate, x, y, 0);
		else if (maxStreak >= BRONZE_STAR_STREAK)
			spriteId = CreateSprite(&sBronzeStarTemplate, x, y, 0);
	}

	sFrontierRecordsPtr->starSpriteIds[arrayIndex] = spriteId;
}

static void DestroyStarSprites(void)
{
	u32 i;

	for (i = 0; i < NELEMS(sFrontierRecordsPtr->starSpriteIds); ++i)
	{
		if (sFrontierRecordsPtr->starSpriteIds[i] < MAX_SPRITES)
		{
			DestroySprite(&gSprites[sFrontierRecordsPtr->starSpriteIds[i]]);
			sFrontierRecordsPtr->starSpriteIds[i] = MAX_SPRITES;
		}
	}
}

static void PrintCurrentRecords(void)
{
	u16 currStreak, maxStreak;
	u8 tier = sFrontierRecordsPtr->battleTier;
	const u8* string;

	static const struct TextColor titleColour =
	{
		.bgColor = 0, //Transparent
		.fgColor = 1, //White
		.shadowColor = 10, //Light Gray
	};

	static const struct TextColor tierNameColour =
	{
		.bgColor = 0, //Transparent
		.fgColor = 8, //Blue
		.shadowColor = 9, //Light Blue
	};

	static const struct TextColor levelColour =
	{
		.bgColor = 0, //Transparent
		.fgColor = 4, //Red
		.shadowColor = 5, //Orange
	};

	static const struct TextColor partySizeColour =
	{
		.bgColor = 0, //Transparent
		.fgColor = 6, //Green
		.shadowColor = 7, //Light Green
	};

	static const struct TextColor generalColour =
	{
		.bgColor = 0, //Transparent
		.fgColor = 2, //Gray
		.shadowColor = 3, //Light Gray
	};

	CleanWindows();

	//Print battle facility
	GetMapName(gStringVar4, GetCurrentRegionMapSectionId(), 0);
	WindowPrint(WIN_BATTLE_FACILITY_NAME, 1, 0, 6, &titleColour, 0, gStringVar4);

	//Print Tier Name
	string = GetFrontierTierName(tier, sFrontierRecordsPtr->battleType);
	WindowPrint(WIN_TIER, 0, 0, 4, &tierNameColour, 0, string);

	//Print Current & Max Streak
	WindowPrint(WIN_CURRENT_STREAK_LEVEL_50, 0, 0, 4, &generalColour, 0, gText_CurrentStreak);
	WindowPrint(WIN_MAX_STREAK_LEVEL_50, 0, 0, 4, &generalColour, 0, gText_MaxStreak);

	if (BATTLE_FACILITY_NUM == IN_BATTLE_MINE) //Battle Mine is special
	{
		currStreak = GetBattleMineStreak(CURR_STREAK, sFrontierRecordsPtr->battleTier);
		maxStreak = GetBattleMineStreak(MAX_STREAK, sFrontierRecordsPtr->battleTier);
		ConvertIntToDecimalStringN(gStringVar1, currStreak, 0, 5);
		ConvertIntToDecimalStringN(gStringVar2, maxStreak, 0, 5);

		WindowPrint(WIN_CURRENT_STREAK_3V3_LEVEL_50, 0, 0, 4, &generalColour, 0, gStringVar1);
		WindowPrint(WIN_MAX_STREAK_3V3_LEVEL_50, 0, 0, 4, &generalColour, 0, gStringVar2);
		TryCreateStarSprite(114, 84, 0, maxStreak);
		CommitWindows();
		return;
	}

	//Print record battle type
	string = sBattleTypeStrings[sFrontierRecordsPtr->battleType];
	u8 leftShift = (StringLength(gText_LinkMultiBattleRecord) - StringLength(string)) * 5;
	WindowPrint(WIN_BATTLE_TYPE, 1, leftShift, 6, &titleColour, 0, string);

	//Print Level 50 Party Sizes Numbers
	string = (IsFrontierSingles(sFrontierRecordsPtr->battleType)) ? gText_3v3 : gText_4v4;
	WindowPrint(WIN_3V3_LEVEL_50, 0, 0, 4, &partySizeColour, 0, string);
	WindowPrint(WIN_6V6_LEVEL_50, 0, 0, 4, &partySizeColour, 0, gText_6v6);

	//Print Level 50 General Text
	if (IsLittleCupTier(tier))
		WindowPrint(WIN_LEVEL_50, 0, 0, 4, &levelColour, 0, gText_FrontierRecordLevel5);
	else if (IS_SINGLE_100_RECORD_TIER(tier))
		WindowPrint(WIN_LEVEL_50, 0, 0, 4, &levelColour, 0, gText_FrontierRecordLevel100);
	else
		WindowPrint(WIN_LEVEL_50, 0, 0, 4, &levelColour, 0, gText_FrontierRecordLevel50);

	if (!IsLittleCupTier(tier) && !IS_SINGLE_100_RECORD_TIER(tier)) //Little Cup only has Lv. 5, Monotype only has Lv. 100 in Battle Tower
	{
		//Print Level 100 Party Sizes Numbers
		string = (IsFrontierSingles(sFrontierRecordsPtr->battleType)) ? gText_3v3 : gText_4v4;
		WindowPrint(WIN_3V3_LEVEL_100, 0, 0, 4, &partySizeColour, 0, string);
		WindowPrint(WIN_6V6_LEVEL_100, 0, 0, 4, &partySizeColour, 0, gText_6v6);

		//Print Level 100 General Text
		WindowPrint(WIN_LEVEL_100, 0, 0, 4, &levelColour, 0, gText_FrontierRecordLevel100);
		WindowPrint(WIN_CURRENT_STREAK_LEVEL_100, 0, 0, 4, &generalColour, 0, gText_CurrentStreak);
		WindowPrint(WIN_MAX_STREAK_LEVEL_100, 0, 0, 4, &generalColour, 0, gText_MaxStreak);
	}

	//Get Streaks
	for (u8 level = 50; level <= 100; level += 50)
	{
		for (u8 partySize = 1; partySize <= 6; partySize += 5) //1 represents one record, 6 represents another
		{
			if (IsLittleCupTier(tier))
				level = 5;

			if (IS_SINGLE_100_RECORD_TIER(tier))
				level = 100;

			currStreak = GetBattleTowerStreak(CURR_STREAK, sFrontierRecordsPtr->battleType, sFrontierRecordsPtr->battleTier, partySize, level);
			maxStreak = GetBattleTowerStreak(MAX_STREAK, sFrontierRecordsPtr->battleType, sFrontierRecordsPtr->battleTier, partySize, level);

			ConvertIntToDecimalStringN(gStringVar1, currStreak, 0, 5);
			ConvertIntToDecimalStringN(gStringVar2, maxStreak, 0, 5);

			if (partySize < 6)
			{
				if (level <= 50 || IS_SINGLE_100_RECORD_TIER(tier)) //3v3 Level 50
				{
					WindowPrint(WIN_CURRENT_STREAK_3V3_LEVEL_50, 0, 0, 4, &generalColour, 0, gStringVar1);
					WindowPrint(WIN_MAX_STREAK_3V3_LEVEL_50, 0, 0, 4, &generalColour, 0, gStringVar2);
					TryCreateStarSprite(114, 84, 0, maxStreak);
				}
				else if (!IsLittleCupTier(sFrontierRecordsPtr->battleTier)) //3v3 Level 100
				{
					WindowPrint(WIN_CURRENT_STREAK_3V3_LEVEL_100, 0, 0, 4, &generalColour, 0, gStringVar1);
					WindowPrint(WIN_MAX_STREAK_3V3_LEVEL_100, 0, 0, 4, &generalColour, 0, gStringVar2);
					TryCreateStarSprite(114, 140, 1, maxStreak);
				}
			}
			else
			{
				if (level <= 50 || IS_SINGLE_100_RECORD_TIER(tier)) //6v6 Level 50
				{
					WindowPrint(WIN_CURRENT_STREAK_6V6_LEVEL_50, 0, 0, 4, &generalColour, 0, gStringVar1);
					WindowPrint(WIN_MAX_STREAK_6V6_LEVEL_50, 0, 0, 4, &generalColour, 0, gStringVar2);
					TryCreateStarSprite(178, 84, 2, maxStreak);
				}
				else if (!IsLittleCupTier(sFrontierRecordsPtr->battleTier)) //6v6 Level 100
				{
					WindowPrint(WIN_CURRENT_STREAK_6V6_LEVEL_100, 0, 0, 4, &generalColour, 0, gStringVar1);
					WindowPrint(WIN_MAX_STREAK_6V6_LEVEL_100, 0, 0, 4, &generalColour, 0, gStringVar2);
					TryCreateStarSprite(178, 140, 3, maxStreak);
				}
			}
		}

		if (IsLittleCupTier(tier) || IS_SINGLE_100_RECORD_TIER(tier))
			break; //Only one set of levels for LC & Monotype
	}

	CommitWindows();
}

static void CreateScrollArrows(void)
{
	sFrontierRecordsPtr->scrollArrowDummy = 5;

	AddScrollIndicatorArrowPairParameterized(
		SCROLL_ARROW_RIGHT,
		80, //Y
		9, //Left X
		9 + 223, //Right X
		1000, //Ridiculously high number for Total Items
		110,
		110,
		&sFrontierRecordsPtr->scrollArrowDummy
	);
}

static void MainCB2_FrontierRecords(void)
{
	RunTasks();
	AnimateSprites();
	BuildOamBuffer();
	UpdatePaletteFade();
}

static void VBlankCB_FrontierRecords(void)
{
	LoadOam();
	ProcessSpriteCopyRequests();
	TransferPlttBuffer();
}

static void InitFrontierRecords(void)
{
	FillWindowPixelBuffer(0, PIXEL_FILL(0));
	LoadStarGfx();
	PrintCurrentRecords();
	CreateScrollArrows();
}

static void LoadFrontierRecordsGfx(void)
{
	decompress_and_copy_tile_data_to_vram(2, &gInterfaceGfx_FrontierRecordsTiles, 0, 0, 0);
	LZDecompressWram(gInterfaceGfx_FrontierRecordsMap, sFrontierRecordsPtr->tilemapPtr);
	LoadPalette(gInterfaceGfx_FrontierRecordsPal, 0, 0x20);
	LoadPalette(sFrontierRecordsTextColour, 0xF0, 0x20);
}

static void ClearTasksAndGraphicalStructs(void)
{
    ScanlineEffect_Stop();
    ResetTasks();
    ResetSpriteData();
    ResetTempTileDataBuffers();
    ResetPaletteFade();
    FreeAllSpritePalettes();
}

static void ClearVramOamPlttRegs(void)
{
	DmaFill16(3, 0, VRAM, VRAM_SIZE);
	DmaFill32(3, 0, OAM, OAM_SIZE);
	DmaFill16(3, 0, PLTT, PLTT_SIZE);
	SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_ON | DISPCNT_OBJ_1D_MAP);
	SetGpuReg(REG_OFFSET_BG3CNT, DISPCNT_MODE_0);
	SetGpuReg(REG_OFFSET_BG2CNT, DISPCNT_MODE_0);
	SetGpuReg(REG_OFFSET_BG1CNT, DISPCNT_MODE_0);
	SetGpuReg(REG_OFFSET_BG0CNT, DISPCNT_MODE_0);
	SetGpuReg(REG_OFFSET_BG3HOFS, DISPCNT_MODE_0);
	SetGpuReg(REG_OFFSET_BG3VOFS, DISPCNT_MODE_0);
	SetGpuReg(REG_OFFSET_BG2HOFS, DISPCNT_MODE_0);
	SetGpuReg(REG_OFFSET_BG2VOFS, DISPCNT_MODE_0);
	SetGpuReg(REG_OFFSET_BG1HOFS, DISPCNT_MODE_0);
	SetGpuReg(REG_OFFSET_BG1VOFS, DISPCNT_MODE_0);
	SetGpuReg(REG_OFFSET_BG0HOFS, DISPCNT_MODE_0);
	SetGpuReg(REG_OFFSET_BG0VOFS, DISPCNT_MODE_0);
}

static void CB2_FrontierRecords(void)
{
	switch (gMain.state) {
		case 0:
		default:
			SetVBlankCallback(NULL);
			ClearVramOamPlttRegs();
			gMain.state++;
			break;
		case 1:
			ClearTasksAndGraphicalStructs();
			gMain.state++;
			break;
		case 2:
			sFrontierRecordsPtr->tilemapPtr = Malloc(0x1000);
			ResetBgsAndClearDma3BusyFlags(0);
			InitBgsFromTemplates(0, sFrontierRecordsBgTemplates, NELEMS(sFrontierRecordsBgTemplates));
			SetBgTilemapBuffer(2, sFrontierRecordsPtr->tilemapPtr);
			gMain.state++;
			break;
		case 3:
			LoadFrontierRecordsGfx();
			gMain.state++;
			break;
		case 4:
			if (!free_temp_tile_data_buffers_if_possible())
			{
				ShowBg(0);
				ShowBg(1);
				ShowBg(2);
				CopyBgTilemapBufferToVram(2);
				gMain.state++;
			}
			break;
		case 5:
			InitWindows(sFrontierRecordsWinTemplates);
			DeactivateAllTextPrinters();
			gMain.state++;
			break;
		case 6:
			BeginNormalPaletteFade(0xFFFFFFFF, 0, 16, 0, RGB_BLACK);
			gMain.state++;
			break;
		case 7:
			SetVBlankCallback(VBlankCB_FrontierRecords);
			InitFrontierRecords();
			CreateTask(Task_FrontierRecordsFadeIn, 0);
			SetMainCallback2(MainCB2_FrontierRecords);
			gMain.state = 0;
			break;
	}
}

static void Task_InitFrontierRecords(u8 taskId)
{
	if (!gPaletteFade->active)
	{
		sFrontierRecordsPtr = Calloc(sizeof(struct FrontierRecords));
		sFrontierRecordsPtr->battleType = (Var8000 >= NUM_TOWER_BATTLE_TYPES) ? BATTLE_FACILITY_SINGLE : Var8000;
		sFrontierRecordsPtr->facilityNum = BATTLE_FACILITY_NUM;
		Memset(sFrontierRecordsPtr->starSpriteIds, MAX_SPRITES, sizeof(sFrontierRecordsPtr->starSpriteIds));

		switch (sFrontierRecordsPtr->facilityNum) {
			case IN_BATTLE_MINE:
				sFrontierRecordsPtr->numTiers = gNumBattleMineTiers;
				sFrontierRecordsPtr->tierList = gBattleMineTiers;
				break;
			case IN_BATTLE_CIRCUS:
				sFrontierRecordsPtr->numTiers = gNumBattleCircusTiers;
				sFrontierRecordsPtr->tierList = gBattleCircusTiers;
				break;
			default: //Battle Tower + Battle Sands
				sFrontierRecordsPtr->numTiers = gNumBattleTowerTiers;
				sFrontierRecordsPtr->tierList = gBattleTowerTiers;
				break;
		}

		sFrontierRecordsPtr->battleTier = sFrontierRecordsPtr->tierList[0];

		PlayRainStoppingSoundEffect();
		SetMainCallback2(CB2_FrontierRecords);
		DestroyTask(taskId);
	}
}

//@Details: Opens up the Battle Tower records screen.
//@Inputs:
//		Var8000: Battle Type
//@Returns: If the Pokemon was added or not.
void sp057_ShowFrontierRecords(void)
{
	VarSet(VAR_BATTLE_FACILITY_BATTLE_TYPE, BATTLE_FACILITY_SINGLE); //So the records get loaded correctly
	FadeScreen(FADE_TO_BLACK, 0);
	CreateTask(Task_InitFrontierRecords, 0);
}
