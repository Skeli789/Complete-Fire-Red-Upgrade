#include "defines.h"
#include "../include/bg.h"
#include "../include/decompress.h"
#include "../include/event_data.h"
#include "../include/field_weather.h"
#include "../include/gpu_regs.h"
#include "../include/item.h"
#include "../include/item_menu.h"
#include "../include/m4a.h"
#include "../include/malloc.h"
#include "../include/main.h"
#include "../include/menu.h"
#include "../include/overworld.h"
#include "../include/palette.h"
#include "../include/random.h"
#include "../include/scanline_effect.h"
#include "../include/sound.h"
#include "../include/string_util.h"
#include "../include/text.h"
#include "../include/window.h"
#include "../include/constants/items.h"
#include "../include/constants/rgb.h"
#include "../include/constants/songs.h"
#include "../include/gba/io_reg.h"

#include "../include/new/em_mining.h"
#include "../include/new/util.h"



#define NUM_ROWS 8
#define NUM_COLS 12
#define MAX_PRIZE_AMOUNT 4
#define MAX_BEDROCK_AMOUNT 5
#define COLLAPSE_STRESS_LEVEL 50

#define SE_HAMMER 124
#define SE_PICK_AXE 132
#define SE_BEDROCK 113
#define MUS_SLOTS_WIN 269
#define SE_DOOR 241
#define SE_RG_CARD_OPEN SE_RG_CARD1
#define SE_ROTATING_GATE 42

struct UndergroundMining
{
	u8* tilemapPtr;
	u8 numPrizes;
	u8 cursorRow;
	u8 cursorCol;
	u8 currentTool;
	u8 stressLevel;
	u8 hammerIconSpriteId;
	u8 pickAxeIconSpriteId;
	u8 cursorSpriteId;
	u8 prizeSpriteIds[MAX_PRIZE_AMOUNT];
	u8 bedrockSpriteIds[MAX_BEDROCK_AMOUNT];
	u8 prizeIds[MAX_PRIZE_AMOUNT];
	bool8 foundPrizes[MAX_PRIZE_AMOUNT];
	u8 grid[NUM_ROWS][NUM_COLS];
	u8 objectGrid[NUM_ROWS][NUM_COLS];
};

#define sUndergroundMiningPtr (*((struct UndergroundMining**) 0x2024084)) //Reuses gBattleMons

enum
{
	BG_TEXT,
	BG_COLLAPSE,
	BG_TILES,
	BG_BACKGROUND,
};

static const struct BgTemplate sUndergroundMiningBgTemplates[] =
{
	{
        .bg = BG_TEXT,
        .charBaseIndex = 0,
        .mapBaseIndex = 31,
        .screenSize = 0,
        .paletteMode = 0, // 4bpp
        .priority = 1,
        .baseTile = 0
    },
	{
        .bg = BG_COLLAPSE,
        .charBaseIndex = 1,
        .mapBaseIndex = 30,
        .screenSize = 0,
        .paletteMode = 0, // 4bpp
        .priority = 0,
        .baseTile = 0
    },
    {
        .bg = BG_TILES,
        .charBaseIndex = 2,
        .mapBaseIndex = 29,
        .screenSize = 0,
        .paletteMode = 0, // 4bpp
        .priority = 2,
        .baseTile = 0
    },
	{
        .bg = BG_BACKGROUND,
        .charBaseIndex = 3,
        .mapBaseIndex = 28,
        .screenSize = 0,
        .paletteMode = 0, // 4bpp
        .priority = 3,
        .baseTile = 0
    },
};

enum Windows
{
	WIN_TEXT,
	WIN_TOOL_SWAP_INFO,
	WINDOW_COUNT,
};

enum Tools
{
	TOOL_HAMMER,
	TOOL_PICK_AXE,
};

static const struct WindowTemplate sUndergroundMiningWinTemplates[WINDOW_COUNT + 1] =
{
	[WIN_TEXT] =
	{
        .bg = BG_COLLAPSE, //Highest
        .tilemapLeft = 2,
        .tilemapTop = 15,
        .width = 25,
        .height = 4,
        .paletteNum = 2,
        .baseBlock = 1,
    },
	[WIN_TOOL_SWAP_INFO] =
	{
        .bg = BG_TILES, //So it moves when the screen shakes
        .tilemapLeft = 25,
        .tilemapTop = 3,
        .width = 5,
        .height = 3,
        .paletteNum = 2,
        .baseBlock = 101,
    },
    DUMMY_WIN_TEMPLATE
};

enum
{
	DUMMY_PRINT_TAG = 0x2710,
	HAMMER_ICON_TAG,
	PICK_AXE_ICON_TAG,
	CURSOR_TAG,
	MINI_TOOLS_TAG,
	IMPACT_TAG,
	PRIZE_TAG_1,
	PRIZE_TAG_2,
	PRIZE_TAG_3,
	PRIZE_TAG_4,
	BEDROCK_TAG_1,
	BEDROCK_TAG_2,
	BEDROCK_TAG_3,
	BEDROCK_TAG_4,
};

//This file's functions
static void Task_UndergroundMiningFadeOut(u8 taskId);
static void ChangeTool(void);
static bool8 IsCursorVisible(void);
static void SnapCursorToPosition(s8 row, s8 col);
static bool8 IsValidPos(s8 row, s8 col);
static bool8 BedrockAtPos(u8 row, u8 col);
static bool8 PosRightAboveBedrock(u8 row, u8 col);
static bool8 IsPrizeAtPos(u8 row, u8 col);
static bool8 IsPrizeFullyVisible(u8 prizeNum);
static bool8 AtCornerOfRectangle(u8 row, u8 col, u8 baseRow, u8 baseCol, u8 finalRow, u8 finalCol);
static void BreakRocks(void);
static void Task_ShakeScreen(u8 taskId);
static void Task_UndergroundMiningWaitForKeyPress(u8 taskId);
static void CopyGridPositionToTilemapBuffer(u32 row, u32 col);
static void SpriteCB_MiningTools(struct Sprite* sprite);
static void SpriteCB_Impact(struct Sprite* sprite);
static void Task_BlendPrizeInAndOut(u8 task);
static void Task_BlendPrizeInAndOutStep(u8 taskId);
static bool8 AllPrizesAreFound(void);

static const struct OamData sMiningIconOAM =
{
	.affineMode = ST_OAM_AFFINE_OFF,
	.objMode = ST_OAM_OBJ_NORMAL,
	.shape = SPRITE_SHAPE(32x64),
	.size = SPRITE_SIZE(32x64),
	.priority = 2, //On BG 2
};

static const struct OamData sCursorOAM =
{
	.affineMode = ST_OAM_AFFINE_OFF,
	.objMode = ST_OAM_OBJ_NORMAL,
	.shape = SPRITE_SHAPE(16x16),
	.size = SPRITE_SIZE(16x16),
	.priority = 2, //On BG 2
};

static const struct OamData sMiniToolsOAM =
{
	.affineMode = ST_OAM_AFFINE_OFF,
	.objMode = ST_OAM_OBJ_NORMAL,
	.shape = SPRITE_SHAPE(32x32),
	.size = SPRITE_SIZE(32x32),
	.priority = 1, //On BG 1
};

static const struct OamData sImpactOAM =
{
	.affineMode = ST_OAM_AFFINE_OFF,
	.objMode = ST_OAM_OBJ_NORMAL,
	.shape = SPRITE_SHAPE(64x64),
	.size = SPRITE_SIZE(64x64),
	.priority = 1, //On BG 1
};

//The hammer and pick axe brighten and darken depending on which is selected
static const union AnimCmd sAnimCmdBrightIcon[] =
{
	ANIMCMD_FRAME(0, 1),
	ANIMCMD_END
};

static const union AnimCmd sAnimCmdDarkIcon[] =
{
	ANIMCMD_FRAME(32, 1),
	ANIMCMD_END
};

static const union AnimCmd *const sAnimCmdTable_MiningIcons[] =
{
	sAnimCmdBrightIcon,
	sAnimCmdDarkIcon,
};

//The cursor
static const union AnimCmd sAnimCmdCursor[] =
{
	ANIMCMD_FRAME(0, 8),
	ANIMCMD_FRAME(4, 8),
	ANIMCMD_FRAME(8, 8),
	ANIMCMD_FRAME(4, 8),
	ANIMCMD_JUMP(0)
};

static const union AnimCmd *const sAnimCmdTable_Cursor[] =
{
	sAnimCmdCursor,
};

//The tools that appear when the player presses A
static const union AnimCmd sAnimCmdMiniHammer[] =
{
	ANIMCMD_FRAME(16, 4),
	ANIMCMD_FRAME(0,  2),
	ANIMCMD_FRAME(16, 4),
	ANIMCMD_FRAME(0,  1),
	ANIMCMD_END
};

static const union AnimCmd sAnimCmdMiniPickAxe[] =
{
	ANIMCMD_FRAME(48, 4),
	ANIMCMD_FRAME(32, 2),
	ANIMCMD_FRAME(48, 4),
	ANIMCMD_FRAME(32, 1),
	ANIMCMD_END
};

static const union AnimCmd *const sAnimCmdTable_MiniTools[] =
{
	sAnimCmdMiniHammer,
	sAnimCmdMiniPickAxe,
};

//The impact that appears when a tool is used
static const union AnimCmd sAnimCmdHammerImpact[] =
{
	ANIMCMD_FRAME(0, 1),
	ANIMCMD_END
};

static const union AnimCmd sAnimCmdPickAxeImpact[] =
{
	ANIMCMD_FRAME(64, 1),
	ANIMCMD_END
};

static const union AnimCmd *const sAnimCmdTable_Impact[] =
{
	sAnimCmdHammerImpact,
	sAnimCmdPickAxeImpact,
};

static const struct SpriteTemplate sHammerIconTemplate =
{
	.tileTag = HAMMER_ICON_TAG,
	.paletteTag = HAMMER_ICON_TAG,
	.oam = &sMiningIconOAM,
	.anims = sAnimCmdTable_MiningIcons,
	.images = NULL,
	.affineAnims = gDummySpriteAffineAnimTable,
	.callback = SpriteCallbackDummy,
};

static const struct SpriteTemplate sPickAxeIconTemplate =
{
	.tileTag = PICK_AXE_ICON_TAG,
	.paletteTag = PICK_AXE_ICON_TAG,
	.oam = &sMiningIconOAM,
	.anims = sAnimCmdTable_MiningIcons,
	.images = NULL,
	.affineAnims = gDummySpriteAffineAnimTable,
	.callback = SpriteCallbackDummy,
};

static const struct SpriteTemplate sCursorTemplate =
{
	.tileTag = CURSOR_TAG,
	.paletteTag = MINI_TOOLS_TAG,
	.oam = &sCursorOAM,
	.anims = sAnimCmdTable_Cursor,
	.images = NULL,
	.affineAnims = gDummySpriteAffineAnimTable,
	.callback = SpriteCallbackDummy,
};

static const struct SpriteTemplate sMiniToolsTemplate =
{
	.tileTag = MINI_TOOLS_TAG,
	.paletteTag = MINI_TOOLS_TAG,
	.oam = &sMiniToolsOAM,
	.anims = sAnimCmdTable_MiniTools,
	.images = NULL,
	.affineAnims = gDummySpriteAffineAnimTable,
	.callback = SpriteCB_MiningTools,
};

static const struct SpriteTemplate sImpactTemplate =
{
	.tileTag = IMPACT_TAG,
	.paletteTag = IMPACT_TAG,
	.oam = &sImpactOAM,
	.anims = sAnimCmdTable_Impact,
	.images = NULL,
	.affineAnims = gDummySpriteAffineAnimTable,
	.callback = SpriteCB_Impact,
};

static const struct CompressedSpriteSheet sHammerIconSpriteSheet = {(const u8*)MiningHammerTiles, (32 * 64 * 2) / 2, HAMMER_ICON_TAG};

static const struct CompressedSpritePalette sHammerIconSpritePalette = {(const u8*)MiningHammerPal, HAMMER_ICON_TAG};
static const struct CompressedSpriteSheet sPickAxeIconSpriteSheet = {(const u8*)MiningPickAxeTiles, (32 * 64 * 2) / 2, PICK_AXE_ICON_TAG};
static const struct CompressedSpritePalette sPickAxeIconSpritePalette = {(const u8*)MiningPickAxePal, PICK_AXE_ICON_TAG};
static const struct CompressedSpriteSheet sCursorSpriteSheet = {(const u8*)MiningCursorTiles, (16 * 16 * 3) / 2, CURSOR_TAG};
static const struct CompressedSpriteSheet sMiniToolsSpriteSheet = {(const u8*)MiningMiniToolsTiles, (32 * 32 * 4) / 2, MINI_TOOLS_TAG};
static const struct CompressedSpritePalette sMiniToolsSpritePalette = {(const u8*)MiningMiniToolsPal, MINI_TOOLS_TAG};
static const struct CompressedSpriteSheet sImpactSpriteSheet = {(const u8*)MiningImpactTiles, (64 * 64 * 2) / 2, IMPACT_TAG};
static const struct CompressedSpritePalette sImpactSpritePalette = {(const u8*)MiningImpactPal, IMPACT_TAG};
static const struct CompressedSpritePalette sBedrockSpritePalette = {(const u8*)MiningBedrock0Pal, BEDROCK_TAG_1};

static void SpriteCB_MiningTools(struct Sprite* sprite)
{
	switch (sprite->data[0]++) {
		case 2:
			//Both Disappear
			//Both Reappear
			sprite->invisible = TRUE;
			break;
		case 4:
			sprite->invisible = FALSE;
			break;
		case 6:
			//Hammer Move Left Down
			//Impact Disappear
			sprite->pos1.x -= 2;
			sprite->pos1.y += 2;
			break;
		case 8:
			//Impact Reappear
			break;
		case 10:
			//Normalize Sprite
			//Impact Disappear
			break;
		case 12:
			//Impact Reappear
			break;
		case 14:
			//Hammer Shake
			//Impact Disappear
			sprite->pos1.x += 2;
			break;
		case 16:
			sprite->pos1.x -= 2;
			break;
		case 18:
			sprite->pos1.x += 2;
			break;
		case 20:
			DestroySprite(sprite);
			if (!AllPrizesAreFound())
				gSprites[sUndergroundMiningPtr->cursorSpriteId].invisible = FALSE; //Reenable cursor
	}
}

static void SpriteCB_Impact(struct Sprite* sprite)
{
	switch (sprite->data[0]++) {
		case 2:
			//Both Disappear
			//Both Reappear
			sprite->invisible = TRUE;
			break;
		case 4:
			sprite->invisible = FALSE;
			break;
		case 6:
			//Hammer Move Left Down
			//Impact Disappear
			sprite->invisible = TRUE;
			break;
		case 8:
			//Impact Reappear
			sprite->invisible = FALSE;
			break;
		case 10:
			//Normalize Sprite
			//Impact Disappear
			sprite->invisible = TRUE;
			break;
		case 12:
			//Impact Reappear
			sprite->invisible = FALSE;
			break;
		case 14:
			//Hammer Shake
			//Impact Disappear
			DestroySprite(sprite);
			break;
	}
}

static void CommitWindows(void)
{
	for (u8 i = 0; i < WINDOW_COUNT; ++i)
	{
		CopyWindowToVram(i, 3);
		PutWindowTilemap(i);
	}
}

static void PrintMessage(const u8* str)
{
	ShowBg(BG_TEXT);
	FillWindowPixelBuffer(WIN_TEXT, PIXEL_FILL(0));
	CommitWindows();
	AddTextPrinterParameterized2(WIN_TEXT, 1, str, GetPlayerTextSpeedDelay(), NULL, 1, 14, 3);
	RunTextPrinters();
}

static void Task_BlendPrizeInAndOut(u8 taskId)
{
	gTasks[taskId].data[0] = (gSprites[gTasks[taskId].data[0]].oam.paletteNum * 0x10) + 0x101;
	gTasks[taskId].data[1] = RGB_WHITE; //Colour
	gTasks[taskId].data[2] = 0;
	gTasks[taskId].data[3] = 0x8; //Blend coefficient
	gTasks[taskId].data[4] = 0;
	gTasks[taskId].data[5] = 2; //Delay
	gTasks[taskId].data[6] = 0;
	gTasks[taskId].func = Task_BlendPrizeInAndOutStep;
}

static void Task_BlendPrizeInAndOutStep(u8 taskId)
{
	struct Task *task = &gTasks[taskId];

	if (++task->data[4] >= task->data[5])
	{
		task->data[4] = 0;
		if (!task->data[6])
		{
			task->data[2]++;
			BlendPalette(task->data[0], 15, task->data[2], task->data[1]);
			if (task->data[2] == task->data[3])
				task->data[6] = 1;
		}
		else
		{
			task->data[2]--;
			BlendPalette(task->data[0], 15, task->data[2], task->data[1]);
			if (!task->data[2])
				DestroyTask(taskId);
		}
	}
}

static void Task_UndergroundMiningFadeOut(u8 taskId)
{
	if (!gPaletteFade->active)
	{
		SetMainCallback2(CB2_ReturnToFieldContinueScript);
		Free(sUndergroundMiningPtr->tilemapPtr);
		FREE_AND_SET_NULL(sUndergroundMiningPtr);
		FreeAllWindowBuffers();
		DestroyTask(taskId);
	}
}

static void Task_GiveItemsBeforeFadeOut(u8 taskId)
{
	if (!RunTextPrintersAndIsPrinter0Active())
	{
		if (JOY_NEW(A_BUTTON | B_BUTTON))
		{
			for (u8 i = 0; i < NELEMS(sUndergroundMiningPtr->prizeIds); ++i)
			{
				if (sUndergroundMiningPtr->foundPrizes[i]) //Is valid found item
				{
					u16 item = sMiningPrizes[sUndergroundMiningPtr->prizeIds[i]].itemId;
					sUndergroundMiningPtr->foundPrizes[i] = FALSE; //Record item has been given
					CopyItemName(item, gStringVar1);
					if (gStringVar1[0] == CHAR_A || gStringVar1[0] == CHAR_E || gStringVar1[0] == CHAR_I || gStringVar1[0] == CHAR_O || gStringVar1[0] == CHAR_U) //Starts with vowel
					{
						gStringVar2[0] = CHAR_n; //An
						gStringVar2[1] = 0xFF;
					}
					else
						gStringVar2[0] = 0xFF; //A

					if (!CheckBagHasSpace(item, 1))
						StringExpandPlaceholders(gStringVar4, gText_UndergroundMining_NoSpace);
					else
					{
						AddBagItem(item, 1);
						StringExpandPlaceholders(gStringVar4, gText_UndergroundMining_FoundItem);
					}

					PrintMessage(gStringVar4);
					return; //May be more messages soon
				}
			}

			BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 16, RGB_BLACK);
			gTasks[taskId].func = Task_UndergroundMiningFadeOut;
		}
	}
}

static void Task_PrintFoundAllItems(u8 taskId)
{
	if (IsFanfareTaskInactive())
	{
		PrintMessage(gText_UndergroundMining_GotAllItems);
		gTasks[taskId].func = Task_GiveItemsBeforeFadeOut;
	}
}

static void Task_PlayFoundAllItemsFanfare(u8 taskId)
{
	if (!FuncIsActiveTask(Task_ShakeScreen))
	{
		PlayFanfare(MUS_SLOTS_WIN);
		gTasks[taskId].func = Task_PrintFoundAllItems;
	}
}

static void Task_PrintWallCollapsed(u8 taskId)
{
	PrintMessage(gText_UndergroundMining_WallCollapsed);
	gTasks[taskId].func = Task_GiveItemsBeforeFadeOut;
}

static bool8 RemoveLayerOfScreen(u16 step)
{
	//Handle Background
	for (u32 i = 0x40 * step; i < (u32) (0x40 * step + 0x40); ++i)
	{
		if ((i & 1) == 0) //Lower Byte
			sUndergroundMiningPtr->tilemapPtr[i] = 0x1;
		else //Upper Byte
			sUndergroundMiningPtr->tilemapPtr[i] = 0x30; //Palette 3
	}

	CopyBgTilemapBufferToVram(BG_COLLAPSE);

	if (0x40 * step >= 0x500) //Screen has been wiped
		return TRUE;
	else
		return FALSE;
}

static void Task_PrepareToCollapseWall(u8 taskId)
{
	switch (gTasks[taskId].data[0]) {
		case 0: //Shake Screen Second Time
			if (!FuncIsActiveTask(Task_ShakeScreen))
			{
				CreateTask(Task_ShakeScreen, 0xFF); //Start shaking the screen
				gTasks[taskId].data[0]++;
			}
			break;
		case 1: //Shake Screen Third Time
			if (!FuncIsActiveTask(Task_ShakeScreen))
			{
				CreateTask(Task_ShakeScreen, 0xFF); //Start shaking the screen
				gTasks[taskId].data[0]++;
			}
			break;
		case 2: //Shake Screen Fourth Time
			if (!FuncIsActiveTask(Task_ShakeScreen))
			{
				CreateTask(Task_ShakeScreen, 0xFF); //Start shaking the screen
				gTasks[taskId].data[0]++;
			}
			break;
		case 3:
			if (!FuncIsActiveTask(Task_ShakeScreen))
			{
				ShowBg(BG_COLLAPSE);
				decompress_and_copy_tile_data_to_vram(BG_COLLAPSE, &UndergroundMiningCollapseTiles, 0, 0, 0);
				memset(sUndergroundMiningPtr->tilemapPtr, 0x0, 0x1000);
				SetBgTilemapBuffer(BG_COLLAPSE, sUndergroundMiningPtr->tilemapPtr);
				CopyBgTilemapBufferToVram(BG_COLLAPSE);
				gTasks[taskId].data[0]++;
			}
			break;
		case 4:
			if (!IsDma3ManagerBusyWithBgCopy())
				gTasks[taskId].data[0]++;
			break;
		case 5 ... 32767:
			if (RemoveLayerOfScreen(gTasks[taskId].data[0]++ - 5))
			{
				//Stop current sound effects
				m4aMPlayStop(&gMPlayInfo_SE1);
				m4aMPlayStop(&gMPlayInfo_SE2);
				PlaySE(SE_DOOR);

				//Hide Hidden Graphics
				HideBg(BG_BACKGROUND);
				HideBg(BG_TILES);
				DestroySprite(&gSprites[sUndergroundMiningPtr->hammerIconSpriteId]);
				DestroySprite(&gSprites[sUndergroundMiningPtr->pickAxeIconSpriteId]);
				DestroySprite(&gSprites[sUndergroundMiningPtr->cursorSpriteId]);
				for (u8 i = 0; i < NELEMS(sUndergroundMiningPtr->prizeSpriteIds); ++i)
					DestroySprite(&gSprites[sUndergroundMiningPtr->prizeSpriteIds[i]]);
				for (u8 i = 0; i < NELEMS(sUndergroundMiningPtr->bedrockSpriteIds); ++i)
					DestroySprite(&gSprites[sUndergroundMiningPtr->bedrockSpriteIds[i]]);
				memset(sUndergroundMiningPtr->tilemapPtr, 0x0, 0x1000);
				SetBgTilemapBuffer(BG_COLLAPSE, sUndergroundMiningPtr->tilemapPtr);
				CopyBgTilemapBufferToVram(BG_COLLAPSE);
				gTasks[taskId].func = Task_PrintWallCollapsed; //Screen black
			}
			break;
	}
}

static void ChangeTool(void)
{
	PlaySE(SE_ROTATING_GATE); //Some funny sound that I like
	sUndergroundMiningPtr->currentTool ^= 1;
	if (sUndergroundMiningPtr->currentTool == TOOL_HAMMER)
	{
		StartSpriteAnim(&gSprites[sUndergroundMiningPtr->hammerIconSpriteId], 0); //Brighten Hammer
		StartSpriteAnim(&gSprites[sUndergroundMiningPtr->pickAxeIconSpriteId], 1); //Darken Pick Axe
	}
	else
	{
		StartSpriteAnim(&gSprites[sUndergroundMiningPtr->hammerIconSpriteId], 1); //Darken Hammer
		StartSpriteAnim(&gSprites[sUndergroundMiningPtr->pickAxeIconSpriteId], 0); //Brighten Pick Axe
	}
}

static void SnapCursorToPosition(s8 row, s8 col)
{
	struct Sprite* cursor = &gSprites[sUndergroundMiningPtr->cursorSpriteId];
	
	//Handle wrap around
	if (row < 0)
		row = NUM_ROWS - 1;
	else if (row >= NUM_ROWS)
		row = 0;
	if (col < 0)
		col = NUM_COLS - 1;
	else if (col >= NUM_COLS)
		col = 0;

	sUndergroundMiningPtr->cursorRow = row;
	sUndergroundMiningPtr->cursorCol = col;
	
	cursor->pos2.x = col * 16; //Each tile is 16x16
	cursor->pos2.y = row * 16;
}

static bool8 IsCursorVisible(void)
{
	return !gSprites[sUndergroundMiningPtr->cursorSpriteId].invisible;
}

static void CreateToolsImpactSprites(void)
{
	struct Sprite* cursor = &gSprites[sUndergroundMiningPtr->cursorSpriteId];
	s16 x = cursor->pos1.x + cursor->pos2.x + 16; //Next tile over
	s16 y = cursor->pos1.y + cursor->pos2.y - 8; //Half tile up
	cursor->invisible = TRUE;

	u8 spriteId = CreateSprite(&sMiniToolsTemplate, x, y, 1);
	x -= 16;
	y += 8;
	if (sUndergroundMiningPtr->currentTool == TOOL_HAMMER)
	{
		StartSpriteAnim(&gSprites[spriteId], 0); //Hammer animation
		spriteId = CreateSprite(&sImpactTemplate, x, y, 0);
		StartSpriteAnim(&gSprites[spriteId], 0); //Hammer animation
	}
	else //Pick Axe
	{
		StartSpriteAnim(&gSprites[spriteId], 1); //Pick Axe animation
		spriteId = CreateSprite(&sImpactTemplate, x, y, 0);
		StartSpriteAnim(&gSprites[spriteId], 1); //Pick Axe animation
	}
}

static bool8 IsValidPos(s8 row, s8 col)
{
	return row >= 0 && row < NUM_ROWS
		&& col >= 0 && col < NUM_COLS;
}

static bool8 BedrockAtPos(u8 row, u8 col)
{
	return sUndergroundMiningPtr->grid[row][col] == L1_NO_ROCK
		&& sUndergroundMiningPtr->objectGrid[row][col] == 0xFF;
}

static bool8 PosRightAboveBedrock(u8 row, u8 col)
{
	return sUndergroundMiningPtr->grid[row][col] == L2_NO_ROCK
		&& sUndergroundMiningPtr->objectGrid[row][col] == 0xFF;
}

static bool8 IsPrizeAtPos(u8 row, u8 col)
{
	return sUndergroundMiningPtr->grid[row][col] == L1_NO_ROCK
		&& sUndergroundMiningPtr->objectGrid[row][col] != 0
		&& sUndergroundMiningPtr->objectGrid[row][col] != 0xFF;
}

static bool8 IsPrizeFullyVisible(u8 prizeNum)
{
	u8 row, col;

	for (row = 0; row < NUM_ROWS; ++row)
	{
		for (col = 0; col < NUM_COLS; ++col)
		{
			if (sUndergroundMiningPtr->objectGrid[row][col] == prizeNum
			&&  sUndergroundMiningPtr->grid[row][col] > L1_NO_ROCK)
				return FALSE;
		}
	}
	
	return TRUE;
}

static bool8 AllPrizesAreFound(void)
{
	for (u8 i = 1; i <= sUndergroundMiningPtr->numPrizes; ++i)
	{
		if (!IsPrizeFullyVisible(i))
			return FALSE;
	}

	return TRUE;
}

static bool8 AtCornerOfRectangle(u8 row, u8 col, u8 baseRow, u8 baseCol, u8 finalRow, u8 finalCol)
{
	return (col == baseCol && row == baseRow)
		|| (col == baseCol && row == finalRow)
		|| (col == finalCol && row == baseRow)
		|| (col == finalCol && row == finalRow);
}

static void UpdateGraphicalStressLevel(void)
{
	//Probably the hardest function to get right in the entire file
	//I have no idea what I did anymore
	u8 stressLevel = sUndergroundMiningPtr->stressLevel;
	u8 chunkId = (stressLevel <= 7) ? 0 : ((stressLevel - 8) / 6) + 1; //Each "chunk" is 40x28, starting from the right, chunks start to overlap at 3
	u8 paletteNum = 0x10; //Palette 1
	u8 stressState = (stressLevel <= 7) ? stressLevel : (stressLevel - 2) % 6 + 2;
	s16 offset = 0;

	if (stressState < 7)
	{
		offset = 38 - (chunkId * 6);
	}
	else if (stressState == 7) //7, 13, 20, 27, 34, 41, 48
	{
		offset = 34 - (chunkId * 6);
	}

	offset /= 2;
	for (u8 row = 0; row < 4; ++row)
	{
		for (u8 col = 0; col < 5; ++col)
		{
			s16 actualOffset = offset + col + (0x20 * row);
			if (actualOffset >= 0)
				((u16*) sUndergroundMiningPtr->tilemapPtr)[actualOffset] = sTilesByStressState[stressState][row][col] | (paletteNum << 8);
		}
	}
}

static void BreakRocks(void)
{
	if (!BedrockAtPos(sUndergroundMiningPtr->cursorRow, sUndergroundMiningPtr->cursorCol)) //Direct strike to Bedrock prevents digging
	{
		s8 baseRow = sUndergroundMiningPtr->cursorRow - 1;
		s8 baseCol = sUndergroundMiningPtr->cursorCol - 1;
		s8 finalRow = sUndergroundMiningPtr->cursorRow + 1;
		s8 finalCol = sUndergroundMiningPtr->cursorCol + 1;
		bool8 foundNewItem = FALSE;
		
		if (PosRightAboveBedrock(sUndergroundMiningPtr->cursorRow, sUndergroundMiningPtr->cursorCol)) //About to reveal bedrock
		{
			--sUndergroundMiningPtr->grid[sUndergroundMiningPtr->cursorRow][sUndergroundMiningPtr->cursorCol]; //Go one layer down
			CopyGridPositionToTilemapBuffer(sUndergroundMiningPtr->cursorRow, sUndergroundMiningPtr->cursorCol);
		}
		else
		{
			for (s8 row = baseRow; row <= finalRow; ++row)
			{
				for (s8 col = baseCol; col <= finalCol; ++col)
				{
					if (IsValidPos(row, col))
					{
						if (sUndergroundMiningPtr->currentTool == TOOL_HAMMER
						|| !AtCornerOfRectangle(row, col, baseRow, baseCol, finalRow, finalCol)) //Pick Axe has less range
						{
							if (sUndergroundMiningPtr->grid[row][col] > L1_NO_ROCK && !BedrockAtPos(row, col))
							{
								--sUndergroundMiningPtr->grid[row][col]; //Go one layer down
								CopyGridPositionToTilemapBuffer(row, col);
								
								//Try to play the found a new item sound
								if (IsPrizeAtPos(row, col)
								&& !sUndergroundMiningPtr->foundPrizes[sUndergroundMiningPtr->objectGrid[row][col] - 1]
								&& IsPrizeFullyVisible(sUndergroundMiningPtr->objectGrid[row][col]))
								{
									sUndergroundMiningPtr->foundPrizes[sUndergroundMiningPtr->objectGrid[row][col] - 1] = TRUE;
									foundNewItem = sUndergroundMiningPtr->objectGrid[row][col];
								}
							}
						}
					}
				}
			}
		}

		//Stop current sound effects
		m4aMPlayStop(&gMPlayInfo_SE1);
		m4aMPlayStop(&gMPlayInfo_SE2);
		if (BedrockAtPos(sUndergroundMiningPtr->cursorRow, sUndergroundMiningPtr->cursorCol))
			PlaySE(SE_BEDROCK);
		else if (foundNewItem)
		{
			PlaySE(SE_RG_CARD_OPEN); //Sparkle
			u8 taskId = CreateTask(Task_BlendPrizeInAndOut, 0xFF);
			gTasks[taskId].data[0] = sUndergroundMiningPtr->prizeSpriteIds[foundNewItem - 1];

			if (AllPrizesAreFound())
			{
				u8 taskId = FindTaskIdByFunc(Task_UndergroundMiningWaitForKeyPress);
				if (taskId != 0xFF)
					gTasks[taskId].func = Task_PlayFoundAllItemsFanfare;
			}
		}
		else if (sUndergroundMiningPtr->currentTool == TOOL_HAMMER)
			PlaySE(SE_HAMMER);
		else
			PlaySE(SE_PICK_AXE);
	}
	else
		PlaySE(SE_BEDROCK);

	//Handle stress level
	++sUndergroundMiningPtr->stressLevel;
	if (sUndergroundMiningPtr->currentTool == TOOL_HAMMER)
		++sUndergroundMiningPtr->stressLevel; //Hammer increments stress by 2

	UpdateGraphicalStressLevel();

	SetBgTilemapBuffer(BG_TILES, sUndergroundMiningPtr->tilemapPtr);
	CopyBgTilemapBufferToVram(BG_TILES);

	if (sUndergroundMiningPtr->stressLevel >= COLLAPSE_STRESS_LEVEL) //You get 49 hits
	{
		u8 taskId = FindTaskIdByFunc(Task_UndergroundMiningWaitForKeyPress);
		if (taskId != 0xFF) //May have been removed by finding all items
		{
			if (!FuncIsActiveTask(Task_ShakeScreen))
				CreateTask(Task_ShakeScreen, 0xFF); //Start shaking the screen
			PlaySE(SE_EARTHQUAKE);
			gTasks[taskId].data[0] = 0;
			gTasks[taskId].func = Task_PrepareToCollapseWall;
		}
	}
}

static void Task_ShakeScreen(u8 taskId)
{
	u8 i;

	//Move the sprites one frame before the BG so they actually end up moving at the same time
	switch (gTasks[taskId].data[0]++) {
		case 1:
			//Left 1
			gSprites[sUndergroundMiningPtr->hammerIconSpriteId].pos2.x -= 1;
			gSprites[sUndergroundMiningPtr->pickAxeIconSpriteId].pos2.x -= 1;
			for (i = 0; i < NELEMS(sUndergroundMiningPtr->prizeSpriteIds); ++i)
			{
				if (sUndergroundMiningPtr->prizeSpriteIds[i] < MAX_SPRITES)
					gSprites[sUndergroundMiningPtr->prizeSpriteIds[i]].pos2.x -= 1;
			}
			for (i = 0; i < NELEMS(sUndergroundMiningPtr->bedrockSpriteIds); ++i)
			{
				if (sUndergroundMiningPtr->bedrockSpriteIds[i] < MAX_SPRITES)
					gSprites[sUndergroundMiningPtr->bedrockSpriteIds[i]].pos2.x -= 1;
			}

			//Down 1
			gSprites[sUndergroundMiningPtr->hammerIconSpriteId].pos2.y += 1;
			gSprites[sUndergroundMiningPtr->pickAxeIconSpriteId].pos2.y += 1;
			for (i = 0; i < NELEMS(sUndergroundMiningPtr->prizeSpriteIds); ++i)
			{
				if (sUndergroundMiningPtr->prizeSpriteIds[i] < MAX_SPRITES)
					gSprites[sUndergroundMiningPtr->prizeSpriteIds[i]].pos2.y += 1;
			}
			for (i = 0; i < NELEMS(sUndergroundMiningPtr->bedrockSpriteIds); ++i)
			{
				if (sUndergroundMiningPtr->bedrockSpriteIds[i] < MAX_SPRITES)
					gSprites[sUndergroundMiningPtr->bedrockSpriteIds[i]].pos2.y += 1;
			}
		//	break; //Unbound needs this break to synchronize the sprites with the background
		//case 1:
			//Left 1
			ChangeBgX(BG_TILES, 0x100, 1);
			ChangeBgX(BG_BACKGROUND, 0x100, 1);
			//Down 1
			ChangeBgY(BG_TILES, -0x100, 1);
			ChangeBgY(BG_BACKGROUND, -0x100, 1);
			break;
		case 4:
			//Right 2
			gSprites[sUndergroundMiningPtr->hammerIconSpriteId].pos2.x += 2;
			gSprites[sUndergroundMiningPtr->pickAxeIconSpriteId].pos2.x += 2;
			for (i = 0; i < NELEMS(sUndergroundMiningPtr->prizeSpriteIds); ++i)
			{
				if (sUndergroundMiningPtr->prizeSpriteIds[i] < MAX_SPRITES)
					gSprites[sUndergroundMiningPtr->prizeSpriteIds[i]].pos2.x += 2;
			}
			for (i = 0; i < NELEMS(sUndergroundMiningPtr->bedrockSpriteIds); ++i)
			{
				if (sUndergroundMiningPtr->bedrockSpriteIds[i] < MAX_SPRITES)
					gSprites[sUndergroundMiningPtr->bedrockSpriteIds[i]].pos2.x += 2;
			}

			//Up 2
			gSprites[sUndergroundMiningPtr->hammerIconSpriteId].pos2.y -= 2;
			gSprites[sUndergroundMiningPtr->pickAxeIconSpriteId].pos2.y -= 2;
			for (i = 0; i < NELEMS(sUndergroundMiningPtr->prizeSpriteIds); ++i)
			{
				if (sUndergroundMiningPtr->prizeSpriteIds[i] < MAX_SPRITES)
					gSprites[sUndergroundMiningPtr->prizeSpriteIds[i]].pos2.y -= 2;
			}
			for (i = 0; i < NELEMS(sUndergroundMiningPtr->bedrockSpriteIds); ++i)
			{
				if (sUndergroundMiningPtr->bedrockSpriteIds[i] < MAX_SPRITES)
					gSprites[sUndergroundMiningPtr->bedrockSpriteIds[i]].pos2.y -= 2;
			}
		//	break;
		//case 5:
			//Right 2
			ChangeBgX(BG_TILES, -0x200, 1);
			ChangeBgX(BG_BACKGROUND, -0x200, 1);
			//Up 2
			ChangeBgY(BG_TILES, 0x200, 1);
			ChangeBgY(BG_BACKGROUND, 0x200, 1);
			break;
		case 8:
			//Down 2
			gSprites[sUndergroundMiningPtr->hammerIconSpriteId].pos2.y += 2;
			gSprites[sUndergroundMiningPtr->pickAxeIconSpriteId].pos2.y += 2;
			for (i = 0; i < NELEMS(sUndergroundMiningPtr->prizeSpriteIds); ++i)
			{
				if (sUndergroundMiningPtr->prizeSpriteIds[i] < MAX_SPRITES)
					gSprites[sUndergroundMiningPtr->prizeSpriteIds[i]].pos2.y += 2;
			}
			for (i = 0; i < NELEMS(sUndergroundMiningPtr->bedrockSpriteIds); ++i)
			{
				if (sUndergroundMiningPtr->bedrockSpriteIds[i] < MAX_SPRITES)
					gSprites[sUndergroundMiningPtr->bedrockSpriteIds[i]].pos2.y += 2;
			}
		//	break;
		//case 9:
			//Down 2
			ChangeBgY(BG_TILES, -0x200, 1);
			ChangeBgY(BG_BACKGROUND, -0x200, 1);
			break;
		case 12:
			//Left 2
			gSprites[sUndergroundMiningPtr->hammerIconSpriteId].pos2.x -= 2;
			gSprites[sUndergroundMiningPtr->pickAxeIconSpriteId].pos2.x -= 2;
			for (i = 0; i < NELEMS(sUndergroundMiningPtr->prizeSpriteIds); ++i)
			{
				if (sUndergroundMiningPtr->prizeSpriteIds[i] < MAX_SPRITES)
					gSprites[sUndergroundMiningPtr->prizeSpriteIds[i]].pos2.x -= 2;
			}
			for (i = 0; i < NELEMS(sUndergroundMiningPtr->bedrockSpriteIds); ++i)
			{
				if (sUndergroundMiningPtr->bedrockSpriteIds[i] < MAX_SPRITES)
					gSprites[sUndergroundMiningPtr->bedrockSpriteIds[i]].pos2.x -= 2;
			}

			//Up 2
			gSprites[sUndergroundMiningPtr->hammerIconSpriteId].pos2.y -= 2;
			gSprites[sUndergroundMiningPtr->pickAxeIconSpriteId].pos2.y -= 2;
			for (i = 0; i < NELEMS(sUndergroundMiningPtr->prizeSpriteIds); ++i)
			{
				if (sUndergroundMiningPtr->prizeSpriteIds[i] < MAX_SPRITES)
					gSprites[sUndergroundMiningPtr->prizeSpriteIds[i]].pos2.y -= 2;
			}
			for (i = 0; i < NELEMS(sUndergroundMiningPtr->bedrockSpriteIds); ++i)
			{
				if (sUndergroundMiningPtr->bedrockSpriteIds[i] < MAX_SPRITES)
					gSprites[sUndergroundMiningPtr->bedrockSpriteIds[i]].pos2.y -= 2;
			}
		//	break;
		//case 13:
			//Left 2
			ChangeBgX(BG_TILES, 0x200, 1);
			ChangeBgX(BG_BACKGROUND, 0x200, 1);
			//Up 2
			ChangeBgY(BG_TILES, 0x200, 1);
			ChangeBgY(BG_BACKGROUND, 0x200, 1);
			break;
		case 16:
			//Right 1
			gSprites[sUndergroundMiningPtr->hammerIconSpriteId].pos2.x += 1;
			gSprites[sUndergroundMiningPtr->pickAxeIconSpriteId].pos2.x += 1;
			for (i = 0; i < NELEMS(sUndergroundMiningPtr->prizeSpriteIds); ++i)
			{
				if (sUndergroundMiningPtr->prizeSpriteIds[i] < MAX_SPRITES)
					gSprites[sUndergroundMiningPtr->prizeSpriteIds[i]].pos2.x += 1;
			}
			for (i = 0; i < NELEMS(sUndergroundMiningPtr->bedrockSpriteIds); ++i)
			{
				if (sUndergroundMiningPtr->bedrockSpriteIds[i] < MAX_SPRITES)
					gSprites[sUndergroundMiningPtr->bedrockSpriteIds[i]].pos2.x += 1;
			}

			//Down 1
			gSprites[sUndergroundMiningPtr->hammerIconSpriteId].pos2.y += 1;
			gSprites[sUndergroundMiningPtr->pickAxeIconSpriteId].pos2.y += 1;
			for (i = 0; i < NELEMS(sUndergroundMiningPtr->prizeSpriteIds); ++i)
			{
				if (sUndergroundMiningPtr->prizeSpriteIds[i] < MAX_SPRITES)
					gSprites[sUndergroundMiningPtr->prizeSpriteIds[i]].pos2.y += 1;
			}
			for (i = 0; i < NELEMS(sUndergroundMiningPtr->bedrockSpriteIds); ++i)
			{
				if (sUndergroundMiningPtr->bedrockSpriteIds[i] < MAX_SPRITES)
					gSprites[sUndergroundMiningPtr->bedrockSpriteIds[i]].pos2.y += 1;
			}
		//	break;
		//case 17:
			//Right 1
			ChangeBgX(BG_TILES, -0x100, 1);
			ChangeBgX(BG_BACKGROUND, -0x100, 1);
			//Down 1
			ChangeBgY(BG_TILES, -0x100, 1);
			ChangeBgY(BG_BACKGROUND, -0x100, 1);
			DestroyTask(taskId);
			break;
	}	
}

static void Task_UndergroundMiningWaitForKeyPress(unusedArg u8 taskId)
{
	if (gMain.newKeys & A_BUTTON && IsCursorVisible())
	{
		CreateToolsImpactSprites();
		BreakRocks();
		if (!FuncIsActiveTask(Task_ShakeScreen) && Random() % 100 < 80) //80 % chance of shaking the screen
			CreateTask(Task_ShakeScreen, 0xFF);
	}
	/*else if (gMain.newKeys & B_BUTTON) //No qutting
	{
		BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 16, RGB_BLACK);
		gTasks[taskId].func = Task_UndergroundMiningFadeOut;
	}*/
	else if (JOY_NEW_AND_REPEATED(DPAD_UP) && IsCursorVisible())
	{
		SnapCursorToPosition(sUndergroundMiningPtr->cursorRow - 1, sUndergroundMiningPtr->cursorCol);
	}
	else if (JOY_NEW_AND_REPEATED(DPAD_DOWN) && IsCursorVisible())
	{
		SnapCursorToPosition(sUndergroundMiningPtr->cursorRow + 1, sUndergroundMiningPtr->cursorCol);
	}
	else if (JOY_NEW_AND_REPEATED(DPAD_LEFT) && IsCursorVisible())
	{
		SnapCursorToPosition(sUndergroundMiningPtr->cursorRow, sUndergroundMiningPtr->cursorCol - 1);
	}
	else if (JOY_NEW_AND_REPEATED(DPAD_RIGHT) && IsCursorVisible())
	{
		SnapCursorToPosition(sUndergroundMiningPtr->cursorRow, sUndergroundMiningPtr->cursorCol + 1);
	}
	else if (JOY_NEW_AND_REPEATED(L_BUTTON) || JOY_NEW_AND_REPEATED(R_BUTTON))
	{
		ChangeTool();
	}
}

#ifdef FLAG_EXPLAINED_MINING_RULES
static void Task_WaitExplainRules(u8 taskId)
{
	if (!RunTextPrintersAndIsPrinter0Active())
	{
		if (JOY_NEW(A_BUTTON | B_BUTTON))
		{
			FillWindowPixelBuffer(WIN_TEXT, PIXEL_FILL(0));
			CommitWindows();
			HideBg(BG_TEXT);
			gTasks[taskId].func = Task_UndergroundMiningWaitForKeyPress;
		}
	}
}
#endif

static void Task_UndergroundMiningPrintOpeningMessages(u8 taskId)
{
	if (!RunTextPrintersAndIsPrinter0Active())
	{
		if (JOY_NEW(A_BUTTON | B_BUTTON))
		{	
			#ifdef FLAG_EXPLAINED_MINING_RULES
			if (!FlagGet(FLAG_EXPLAINED_MINING_RULES))
			{
				FlagSet(FLAG_EXPLAINED_MINING_RULES);
				PrintMessage(gText_UndergroundMining_Instructions);
				gTasks[taskId].func = Task_WaitExplainRules;
				return;
			}
			#endif

			FillWindowPixelBuffer(WIN_TEXT, PIXEL_FILL(0));
			CommitWindows();
			HideBg(BG_TEXT);
			gTasks[taskId].func = Task_UndergroundMiningWaitForKeyPress;
		}
	}
}

static void Task_UndergroundMiningPrepareOpeningMessages(u8 taskId)
{
	ConvertIntToDecimalStringN(gStringVar1, sUndergroundMiningPtr->numPrizes, 0, 1);
	StringExpandPlaceholders(gStringVar4, gText_UndergroundMining_DisplayPossiblePrizes);
	PrintMessage(gStringVar4);
	gTasks[taskId].func = Task_UndergroundMiningPrintOpeningMessages;
}

static void Task_UndergroundMiningFadeIn(u8 taskId)
{
	if (!gPaletteFade->active)
		gTasks[taskId].func = Task_UndergroundMiningPrepareOpeningMessages;
}

static void MainCB2_UndergroundMining(void)
{
	RunTasks();
	AnimateSprites();
	BuildOamBuffer();
	UpdatePaletteFade();
}

static void VBlankCB_UndergroundMining(void)
{
	LoadOam();
	ProcessSpriteCopyRequests();
	TransferPlttBuffer();
}

static void LoadSpriteGfx(void)
{
	LoadCompressedSpriteSheetUsingHeap(&sHammerIconSpriteSheet);
	LoadCompressedSpritePaletteUsingHeap(&sHammerIconSpritePalette);
	LoadCompressedSpriteSheetUsingHeap(&sPickAxeIconSpriteSheet);
	LoadCompressedSpritePaletteUsingHeap(&sPickAxeIconSpritePalette);
	LoadCompressedSpriteSheetUsingHeap(&sCursorSpriteSheet);
	LoadCompressedSpriteSheetUsingHeap(&sMiniToolsSpriteSheet);
	LoadCompressedSpritePaletteUsingHeap(&sMiniToolsSpritePalette);
	LoadCompressedSpriteSheetUsingHeap(&sImpactSpriteSheet);
	LoadCompressedSpritePaletteUsingHeap(&sImpactSpritePalette);	
}

static void CreateIconSprites(void)
{
	sUndergroundMiningPtr->hammerIconSpriteId = CreateSprite(&sHammerIconTemplate, 218, 85, 0);
	sUndergroundMiningPtr->pickAxeIconSpriteId = CreateSprite(&sPickAxeIconTemplate, 218, 143, 0);
}

static void CreateCursor(void)
{
	sUndergroundMiningPtr->cursorSpriteId = CreateSprite(&sCursorTemplate, 0, 0, 0);
	if (sUndergroundMiningPtr->cursorSpriteId < MAX_SPRITES)
	{
		gSprites[sUndergroundMiningPtr->cursorSpriteId].pos1.x = 0 + 8; //Base x on screen
		gSprites[sUndergroundMiningPtr->cursorSpriteId].pos1.y = 32 + 8; //Base y on screen
		SnapCursorToPosition(4, 6);
	}
}

static u8 CreateSpriteAtGridPosition(struct SpriteTemplate* template, u8 row, u8 col, u8 subPriority)
{
	s16 x, y;

	if (template->oam->size == SPRITE_SIZE(32x32))
	{
		x = 0  + 16;
		y = 32 + 16;
	}
	else if (template->oam == &gOamData_AffineOff_ObjNormal_64x32)
	{
		x = 0  + 32;
		y = 32 + 16;
	}
	else if (template->oam == &gOamData_AffineOff_ObjNormal_32x64)
	{
		x = 0  + 16;
		y = 32 + 32;
	}
	else //64x64
	{
		x = 0  + 32;
		y = 32 + 32;
	}

	u8 spriteId = CreateSprite(template, x, y, subPriority);
	if (spriteId < MAX_SPRITES)
	{
		gSprites[spriteId].pos2.x = col * 16; //Each tile is 16x16
		gSprites[spriteId].pos2.y = row * 16;
	}

	return spriteId;
}

static u8 GetModifiedPrizeId(u8 prizeId)
{
	//Items are distributed based on type of item
	//This chooses an item based on the type
	u8 numOfType;

	switch (prizeId) {
		case PRIZE_FIRST_SHARD:
			numOfType = PRIZE_LAST_SHARD + 1;
			break;
		case PRIZE_FIRST_EVO_STONE:
			numOfType = PRIZE_LAST_EVO_STONE + 1;
			break;
		default:
			return prizeId;
	}

	return RandRange(prizeId, numOfType);
}

static void GetPrizeDimensions(u8 prizeId, u8* width, u8* height)
{
	u8 i, j;

	for (i = 0; i < MAX_OBJECT_HEIGHT; ++i)
	{
		for (j = 0; j < MAX_OBJECT_WIDTH; ++j)
		{
			if (sMiningPrizes[prizeId].shape[i][j] != 0)
			{
				//Update max height and width
				if (i + 1 > *height)
					*height = i + 1;
				if (j + 1 > *width)
					*width = j + 1;
			}
		}
	}
}

static void GetBedrockDimensions(u8 bedrockId, u8 shapeNum, u8* width, u8* height)
{
	u8 i, j;

	for (i = 0; i < MAX_OBJECT_HEIGHT; ++i)
	{
		for (j = 0; j < MAX_OBJECT_WIDTH; ++j)
		{
			if (sBedrocks[bedrockId].shapes[shapeNum][i][j] != 0)
			{
				//Update max height and width
				if (i + 1 > *height)
					*height = i + 1;
				if (j + 1 > *width)
					*width = j + 1;
			}
		}
	}
}

static bool8 DoesObjectFitAtPosition(u8 row, u8 col, u8 width, u8 height)
{
	u8 i, j;

	for (i = row; i < row + height; ++i)
	{
		for (j = col; j < col + width; ++j)
		{
			if (sUndergroundMiningPtr->objectGrid[i][j] != 0) //Spot is occupied
				return FALSE;
		}
	}

	return TRUE;
}

static void ScatterPrizes(void)
{
	u8 i, j, k, m;
	sUndergroundMiningPtr->numPrizes = RandRange(2, MAX_PRIZE_AMOUNT + 1); //2 - 4 prizes
	memset(sUndergroundMiningPtr->prizeSpriteIds, MAX_SPRITES, sizeof(sUndergroundMiningPtr->prizeSpriteIds));

	for (i = 0; i < sUndergroundMiningPtr->numPrizes; ++i)
	{
		bool8 placed = FALSE;

		do //Loop until a prize that fits is found
		{
			u8 row, col, prizeId;
			u8 width = 0;
			u8 height = 0;

			prizeId = sPrizeEncounterChart[Random() % NELEMS(sPrizeEncounterChart)];
			prizeId = GetModifiedPrizeId(prizeId);
			GetPrizeDimensions(prizeId, &width, &height);

			//Search for space on board for prize
			j = 0;
			do
			{
				row = RandRange(0, NUM_ROWS + 1 - height); //Must fit on board
				col = RandRange(0, NUM_COLS + 1 - width); //Must fit on board
			}
			while (!DoesObjectFitAtPosition(row, col, width, height) && j++ < 100); //Max try 100 times

			if (j >= 100) //Space wasn't found for prize
				continue;

			//Space for object, so actually place it now
			for (k = row; k < row + height; ++k)
			{
				for (m = col; m < col + width; ++m)
				{
					if (sMiningPrizes[prizeId].shape[k - row][m - col]) //Is prize on spot
						sUndergroundMiningPtr->objectGrid[k][m] = i + 1;
				}
			}

			struct CompressedSpriteSheet prizeSpriteSheet =
			{
				sMiningPrizes[prizeId].spriteTiles,
				sMiningPrizes[prizeId].spriteSize,
				PRIZE_TAG_1 + i
			};

			struct CompressedSpritePalette prizeSpritePal =
			{
				(const u8*)sMiningPrizes[prizeId].spritePal,
				PRIZE_TAG_1 + i
			};

			struct SpriteTemplate prizeTemplate =
			{
				.tileTag = PRIZE_TAG_1 + i,
				.paletteTag = PRIZE_TAG_1 + i,
				.oam = sMiningPrizes[prizeId].oam,
				.anims = gDummySpriteAnimTable,
				.images = NULL,
				.affineAnims = gDummySpriteAffineAnimTable,
				.callback = SpriteCallbackDummy,
			};

			LoadCompressedSpriteSheetUsingHeap(&prizeSpriteSheet);
			LoadCompressedSpritePaletteUsingHeap(&prizeSpritePal);
			sUndergroundMiningPtr->prizeSpriteIds[i] = CreateSpriteAtGridPosition(&prizeTemplate, row, col, 0);
			gSprites[sUndergroundMiningPtr->prizeSpriteIds[i]].oam.priority = 3; //Put on BG 3

			sUndergroundMiningPtr->prizeIds[i] = prizeId;
			placed = TRUE;
		} while (!placed);
	}
}

static void ScatterBedrock(void)
{
	u8 i, j, k, m;
	u8 numBedrocks = RandRange(2, MAX_BEDROCK_AMOUNT + 1); //2 - 5 pieces of bedrock
	memset(sUndergroundMiningPtr->bedrockSpriteIds, MAX_SPRITES, sizeof(sUndergroundMiningPtr->bedrockSpriteIds));

	LoadCompressedSpritePaletteUsingHeap(&sBedrockSpritePalette); //All bedrock share the same palette

	for (i = 0; i < numBedrocks; ++i)
	{
		bool8 placed = FALSE;

		do //Loop until a bedrock that fits is found
		{
			u8 row, col;
			u8 width = 0;
			u8 height = 0;
			u8 bedrockId = Random() % NELEMS(sBedrocks);
			u8 shapeNum = Random() % sBedrocks[bedrockId].numShapes; //Choose a random shape configuration
			GetBedrockDimensions(bedrockId, shapeNum, &width, &height);

			//Search for space on board for bedrock
			j = 0;
			do
			{
				row = RandRange(0, NUM_ROWS + 1 - height); //Must fit on board
				col = RandRange(0, NUM_COLS + 1 - width); //Must fit on board
			}
			while (!DoesObjectFitAtPosition(row, col, width, height) && j++ < 100); //Max try 100 times

			if (j >= 100) //Space wasn't found for bedrock
				continue;

			//Space for object, so actually place it now
			for (k = row; k < row + height; ++k)
			{
				for (m = col; m < col + width; ++m)
				{
					if (sBedrocks[bedrockId].shapes[shapeNum][k - row][m - col]) //Is bedrock on spot
						sUndergroundMiningPtr->objectGrid[k][m] = 0xFF; //Bedrock indicator
				}
			}

			struct CompressedSpriteSheet bedrockSpriteSheet =
			{
				sBedrocks[bedrockId].spriteTiles,
				sBedrocks[bedrockId].spriteSize,
				BEDROCK_TAG_1 + i
			};

			struct SpriteTemplate bedrockTemplate =
			{
				.tileTag = BEDROCK_TAG_1 + i,
				.paletteTag = BEDROCK_TAG_1,
				.oam = sBedrocks[bedrockId].oam,
				.anims = sBedrocks[bedrockId].anims,
				.images = NULL,
				.affineAnims = gDummySpriteAffineAnimTable,
				.callback = SpriteCallbackDummy,
			};

			LoadCompressedSpriteSheetUsingHeap(&bedrockSpriteSheet);
			sUndergroundMiningPtr->bedrockSpriteIds[i] = CreateSpriteAtGridPosition(&bedrockTemplate, row, col, 0);
			gSprites[sUndergroundMiningPtr->bedrockSpriteIds[i]].oam.priority = 3; //Put on BG 3
			StartSpriteAnim(&gSprites[sUndergroundMiningPtr->bedrockSpriteIds[i]], shapeNum); //Rotate the bedrock if necessary
			placed = TRUE;
		} while (!placed);
	}
}

const u8 sToolSwapInstructionColour[] = {1, 14, 3};

//Prints "SELECT" above the tools
static void PrintToolSwapInstructions(void)
{
	const u8* colour = sToolSwapInstructionColour;

	FillWindowPixelBuffer(WIN_TOOL_SWAP_INFO, PIXEL_FILL(0));
	AddTextPrinterParameterized3(WIN_TOOL_SWAP_INFO, 0, 2, 6, colour, 0, gText_UndergroundMining_ToolSwapInstructions);
	CopyWindowToVram(WIN_TOOL_SWAP_INFO, 3);
	PutWindowTilemap(WIN_TOOL_SWAP_INFO);
}

static void InitUndergroundMining(void)
{
	LoadSpriteGfx();
	CreateIconSprites();
	CreateCursor();
	ScatterPrizes();
	ScatterBedrock();
	PrintToolSwapInstructions();
	StartSpriteAnim(&gSprites[sUndergroundMiningPtr->pickAxeIconSpriteId], 1); //Pick axe starts darkened
}

static void CopyGridPositionToTilemapBuffer(u32 row, u32 col)
{
	u8 dirtType = sUndergroundMiningPtr->grid[row][col];
	u16 firstDirtTile = (dirtType * 4);
	u8 paletteNum = 0x10; //Palette 1
	
	u16 offset = 0x80 + (row * 2 * 0x20) + (col * 2);
	((u16*) sUndergroundMiningPtr->tilemapPtr)[offset] =        (firstDirtTile + 0) | (paletteNum << 8);
	((u16*) sUndergroundMiningPtr->tilemapPtr)[offset + 1] =    (firstDirtTile + 1) | (paletteNum << 8);
	((u16*) sUndergroundMiningPtr->tilemapPtr)[offset + 0x20] = (firstDirtTile + 2) | (paletteNum << 8);
	((u16*) sUndergroundMiningPtr->tilemapPtr)[offset + 0x21] = (firstDirtTile + 3) | (paletteNum << 8);
}

s16 RandRangeSigned(s16 min, s16 max)
{
	if (min == max)
		return min;

	return (Random() % (max - min)) + min;
}

static void CreateInitialDirtGrid(void)
{
	u8 row1, row2, col1, col2;
	u8 i, j, totalTimes;
	
	memset(sUndergroundMiningPtr->tilemapPtr, 0, 0x1000); //Clear buffer

	//Start by placing blank layer 3 rocks
	for (row1 = 0; row1 < NUM_ROWS; ++row1)
	{
		for (col1 = 0; col1 < NUM_COLS; ++col1)
			sUndergroundMiningPtr->grid[row1][col1] = L3_SMALL_ROCK;
	}
	
	//Create patches of lighter dirt areas
	totalTimes = RandRange(3, 8);
	for (i = 0; i < totalTimes; ++i)
	{
		do
		{
			row1 = RandRange(0, NUM_ROWS);
			row2 = RandRange(0, NUM_ROWS);
		} while (row1 >= row2);

		do
		{
			col1 = RandRange(0, NUM_COLS);
			col2 = RandRange(0, NUM_COLS);
		} while (col1 >= col2);

		for (; row1 < row2; ++row1)
		{
			for (j = col1; j < col2; ++j) //col1 is needed in subsequent loops
				sUndergroundMiningPtr->grid[row1][j] = L2_SMALL_ROCK;
		}
	}

	//Create smaller patches of big rocks on top
	/*Always in the shape:
		  0 0 0
		0 0 0 0 0
		0 0 0 0 0
		0 0 0 0 0
		  0 0 0
	*/
	totalTimes = RandRange(2, 6 + 1);
	for (i = 0; i < totalTimes; ++i)
	{
		s8 baseRow = RandRangeSigned(-4, NUM_ROWS); //Rocks can go up to one row over on either top or bottom
		s8 baseCol = RandRangeSigned(-4, NUM_COLS); //Rocks can go up to one col over on either left or right
		s8 finalRow = baseRow + 5;
		s8 finalCol = baseCol + 5;

		for (s8 k = baseRow; k < finalRow; ++k)
		{
			if (k < 0 || k >= NUM_ROWS)
				continue; //Not legal row

			for (s8 m = baseCol; m < finalCol; ++m)
			{
				if (m < 0 || m >= NUM_COLS)
					continue; //Not legal column
				
				if (AtCornerOfRectangle(k, m, baseRow, baseCol, baseRow + 4, baseCol + 4))
					continue; //Leave corner out
				
				sUndergroundMiningPtr->grid[k][m] = L3_BIG_ROCK;
			}
		}
	}

	//Copy Final Grid
	for (row1 = 0; row1 < NUM_ROWS; ++row1)
	{
		for (col1 = 0; col1 < NUM_COLS; ++col1)
		{
			CopyGridPositionToTilemapBuffer(row1, col1);
		}
	}
}

static void LoadUndergroundMiningGfx(void)
{
	decompress_and_copy_tile_data_to_vram(BG_BACKGROUND, &UndergroundMiningBaseTiles, 0, 0, 0);
	LZDecompressWram(UndergroundMiningBaseMap, sUndergroundMiningPtr->tilemapPtr);

	decompress_and_copy_tile_data_to_vram(BG_TILES, &UndergroundMiningDirtTiles, 0, 0, 0);
	decompress_and_copy_tile_data_to_vram(BG_TEXT, &UndergroundMiningTextboxTiles, 0, 0, 0);
	
	LoadPalette(UndergroundMiningBasePal, 0, 0x20);
	LoadPalette(UndergroundMiningDirtPal, 0x10, 0x20);
	LoadPalette(UndergroundMiningTextboxPal, 0x20, 0x20);

	LoadPalette(UndergroundMiningCollapsePal, 0x30, 0x20);
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

static void CB2_UndergroundMining(void)
{
	switch (gMain.state) {
		default:
		case 0:
			SetVBlankCallback(NULL);
			ClearVramOamPlttRegs();
			gMain.state++;
			break;
		case 1:
			ClearTasksAndGraphicalStructs();
			gMain.state++;
			break;
		case 2:
			sUndergroundMiningPtr->tilemapPtr = Malloc(0x1000);
			ResetBgsAndClearDma3BusyFlags(0);
			InitBgsFromTemplates(0, sUndergroundMiningBgTemplates, NELEMS(sUndergroundMiningBgTemplates));
			SetBgTilemapBuffer(BG_BACKGROUND, sUndergroundMiningPtr->tilemapPtr);
			gMain.state++;
			break;
		case 3:
			LoadUndergroundMiningGfx();
			gMain.state++;
			break;
		case 4: //Load Background Map
			if (!IsDma3ManagerBusyWithBgCopy())
			{
				ShowBg(BG_BACKGROUND);
				CopyBgTilemapBufferToVram(BG_BACKGROUND);
				gMain.state++;
			}
			break;
		case 5: //Load Textbox Map
			if (!IsDma3ManagerBusyWithBgCopy())
			{
				LZDecompressWram(UndergroundMiningTextboxMap, sUndergroundMiningPtr->tilemapPtr);
				SetBgTilemapBuffer(BG_TEXT, sUndergroundMiningPtr->tilemapPtr);
				gMain.state++;
			}
			break;
		case 6:
			if (!IsDma3ManagerBusyWithBgCopy())
			{
				CopyBgTilemapBufferToVram(BG_TEXT);
				gMain.state++;
			}
			break;
		case 7: //Prepare collapse map
			if (!IsDma3ManagerBusyWithBgCopy())
			{
				ShowBg(BG_COLLAPSE);
				gMain.state++;
			}
			break;
		case 8: //Load Dirt Map
			if (!IsDma3ManagerBusyWithBgCopy())
			{
				CreateInitialDirtGrid();
				ShowBg(BG_TILES);
				SetBgTilemapBuffer(BG_TILES, sUndergroundMiningPtr->tilemapPtr);
				CopyBgTilemapBufferToVram(BG_TILES);
				gMain.state++;
			}
			break;
		case 9:
			gTextFlags.canABSpeedUpPrint = TRUE;
			InitWindows(sUndergroundMiningWinTemplates);
			gMain.state++;
			break;
		case 10:
			BeginNormalPaletteFade(0xFFFFFFFF, 0, 16, 0, RGB_BLACK);
			gMain.state++;
			break;
		case 11:
			SetVBlankCallback(VBlankCB_UndergroundMining);
			InitUndergroundMining();
			CreateTask(Task_UndergroundMiningFadeIn, 0);
			SetMainCallback2(MainCB2_UndergroundMining);
			gMain.state = 0;
			break;
	}
}

static void Task_UndergroundMiningExecute(u8 taskId)
{
	if (!gPaletteFade->active)
	{
		CleanupOverworldWindowsAndTilemaps();
		sUndergroundMiningPtr = AllocZeroed(sizeof(struct UndergroundMining));
		SetMainCallback2(CB2_UndergroundMining);
		DestroyTask(taskId);
	}
}

bool8 InitStartUndergroundMining(void)
{
	FadeScreen(1,0);
	CreateTask(Task_UndergroundMiningExecute, 0);
	return FALSE;
}
