#include "defines.h"
#include "../include/bg.h"
#include "../include/event_data.h"
#include "../include/gpu_regs.h"
#include "../include/gba/io_reg.h"
#include "../include/palette.h"
#include "../include/scanline_effect.h"
#include "../include/text.h"
#include "../include/text_window.h"
#include "../include/window.h"

extern u8 gText_AdditionalOptions[];
extern u8 gText_AdditionalOptions[];
extern u8 gText_Difficulty[];
extern u8 gText_StoryMode[];
extern u8 gText_LevelLimit[];
extern u8 gText_FlagToTest[];
extern u8 gText_PokedexGiven[];
extern u8 gText_WildDoublesActivated[];
extern u8 gText_OptionMenuConfirm[];
extern u8 gText_EasyMode[];
extern u8 gText_NormalMode[];
extern u8 gText_HardMode[];
extern u8 sText_LightStory[];
extern u8 sText_DarkStory[];
extern u8 sText_OptionsOff[];
extern u8 sText_OptionsOn[];

extern void TransferPlttBuffer(void);

//Exported Functions
void CB2_InitMoreOptionsMenu(void);
int GetStringRightAlignXOffset(int fontId, const u8 *str, int totalWidth);
int GetStringWidthDifference(int fontId, const u8 *str, int totalWidth, int letterSpacing);

//This File's Static Functions
static void MainCB2(void);
static void VBlankCB(void);
static void Task_MoreOptionsMenuFadeIn(u8 taskId);
static void Task_MoreOptionsMenuProcessInput(u8 taskId);
static void Task_MoreOptionsMenuSave(u8 taskId);
static void Task_MoreOptionsMenuFadeOut(u8 taskId);
static void DrawTextAdditionalOptions(void);
static void DrawMoreOptionsMenuTexts(void);
static void DrawChoices(u8 selection);
static void sub_80BB154(void);

// Menu items
enum
{
    MENUITEM_DIFFICULTY,
    MENUITEM_STORYMODE,
    MENUITEM_LEVELLIMIT,
    MENUITEM_FLAGTOTEST,
    MENUITEM_POKEDEXGIVEN,
    MENUITEM_WILDDOUBLESACTIVATED,
    MENUITEM_CONFIRM,
    MENUITEM_COUNT,
};

enum
{
    TD_MENUSELECTION,
    TD_DIFFICULTY,
    TD_STORYMODE,
    TD_LEVELLIMIT,
    TD_FLAGTOTEST,
    TD_POKEDEXGIVEN,
    TD_WILDDOUBLESACTIVATED,
};

// Window Ids
enum
{
    WIN_TEXT_OPTION,
    WIN_OPTIONS
};

static const u8* const sMoreOptionsMenuItemsNames[MENUITEM_COUNT] =
{
    gText_Difficulty,
	gText_StoryMode,
    gText_LevelLimit,
	gText_FlagToTest,
    gText_PokedexGiven,
    gText_WildDoublesActivated,
    gText_OptionMenuConfirm,
};

static const u8* const sDifficultyOptionNames[] =
{
	gText_EasyMode,
	gText_NormalMode,
	gText_HardMode,
};

static const u8* const sStoryModeNames[] =
{
	sText_LightStory,
	sText_DarkStory,
};

static const u8* const sOnOff[] =
{
	sText_OptionsOff,
	sText_OptionsOn,
};

static const struct WindowTemplate sMoreOptionsMenuWinTemplates[] =
{
    {
        .bg = 1,
        .tilemapLeft = 2,
        .tilemapTop = 1,
        .width = 26,
        .height = 2,
        .paletteNum = 1,
        .baseBlock = 2
    },
    {
        .bg = 0,
        .tilemapLeft = 2,
        .tilemapTop = 5,
        .width = 26,
        .height = 14,
        .paletteNum = 1,
        .baseBlock = 0x36
    },
    DUMMY_WIN_TEMPLATE
};

static const struct BgTemplate sMoreOptionsMenuBgTemplates[] =
{
   {
       .bg = 1,
       .charBaseIndex = 1,
       .mapBaseIndex = 30,
       .screenSize = 0,
       .paletteMode = 0,
       .priority = 0,
       .baseTile = 0
   },
   {
       .bg = 0,
       .charBaseIndex = 1,
       .mapBaseIndex = 31,
       .screenSize = 0,
       .paletteMode = 0,
       .priority = 1,
       .baseTile = 0
   }
};

#define sMoreOptionsMenuTextColour ((u16*) 0x83CC2E4)
static const u16 sMoreOptionsMenuBGColour[] = {0x7E51};

void CB2_InitMoreOptionsMenu(void)
{
    switch (gMain.state)
    {
    default:
    case 0:
        SetVBlankCallback(NULL);
        gMain.state++;
        break;
    case 1:
        DmaClearLarge16(3, (void*)(VRAM), VRAM_SIZE, 0x1000);
        DmaClear32(3, OAM, OAM_SIZE);
        DmaClear16(3, PLTT, PLTT_SIZE);
        SetGpuReg(REG_OFFSET_DISPCNT, 0);
        ResetBgsAndClearDma3BusyFlags(0);
        InitBgsFromTemplates(0, sMoreOptionsMenuBgTemplates, ARRAY_COUNT(sMoreOptionsMenuBgTemplates));
        ChangeBgX(0, 0, 0);
        ChangeBgY(0, 0, 0);
        ChangeBgX(1, 0, 0);
        ChangeBgY(1, 0, 0);
        ChangeBgX(2, 0, 0);
        ChangeBgY(2, 0, 0);
        ChangeBgX(3, 0, 0);
        ChangeBgY(3, 0, 0);
        InitWindows(sMoreOptionsMenuWinTemplates);
        DeactivateAllTextPrinters();
        SetGpuReg(REG_OFFSET_WIN0H, 0);
        SetGpuReg(REG_OFFSET_WIN0V, 0);
        SetGpuReg(REG_OFFSET_WININ, 1);
        SetGpuReg(REG_OFFSET_WINOUT, 35);
        SetGpuReg(REG_OFFSET_BLDCNT, 193);
        SetGpuReg(REG_OFFSET_BLDALPHA, 0);
        SetGpuReg(REG_OFFSET_BLDY, 4);
        SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_WIN0_ON | DISPCNT_OBJ_ON | DISPCNT_OBJ_1D_MAP);
        ShowBg(0);
        ShowBg(1);
        gMain.state++;
        break;
    case 2:
        ResetPaletteFade();
        ScanlineEffect_Stop();
        ResetTasks();
        ResetSpriteData();
        gMain.state++;
        break;
    case 3:
        LoadBgTiles(1, GetWindowFrameTilesPal(gSaveBlock2->optionsWindowFrameType)->tiles, 0x120, 0x1A2);
        gMain.state++;
        break;
    case 4:
        LoadPalette(sMoreOptionsMenuBGColour, 0, sizeof(sMoreOptionsMenuBGColour));
        LoadPalette(GetWindowFrameTilesPal(gSaveBlock2->optionsWindowFrameType)->pal, 0x70, 0x20);
        gMain.state++;
        break;
    case 5:
        LoadPalette(sMoreOptionsMenuTextColour, 0x10, 0x20);
        gMain.state++;
        break;
    case 6:
        PutWindowTilemap(0);
        DrawTextAdditionalOptions();
        gMain.state++;
        break;
    case 7:
        gMain.state++;
        break;
    case 8:
        PutWindowTilemap(1);
        DrawMoreOptionsMenuTexts();
        gMain.state++;
		break;
    case 9:
        sub_80BB154();
        gMain.state++;
        break;
    case 10:
    {
        u8 taskId = CreateTask(Task_MoreOptionsMenuFadeIn, 0);

/*
        gTasks[taskId].data[TD_MENUSELECTION] = 0;
        gTasks[taskId].data[TD_DIFFICULTY] = gSaveBlock2Ptr->optionsTextSpeed;
        gTasks[taskId].data[TD_STORYMODE] = gSaveBlock2Ptr->optionsBattleSceneOff;
        gTasks[taskId].data[TD_LEVELLIMIT] = gSaveBlock2Ptr->optionsBattleStyle;
        gTasks[taskId].data[TD_FLAGTOTEST] = gSaveBlock2Ptr->optionsSound;
        gTasks[taskId].data[TD_POKEDEXGIVEN] = gSaveBlock2Ptr->optionsButtonMode;
        gTasks[taskId].data[TD_WILDDOUBLESACTIVATED] = FlagGet(DOUBLE_WILD_BATTLE_FLAG);
*/
        DrawChoices(taskId);
        CopyWindowToVram(WIN_OPTIONS, 3);
        gMain.state++;
        break;
    }
    case 11:
        BeginNormalPaletteFade(0xFFFFFFFF, 0, 0x10, 0, RGB_BLACK);
        SetVBlankCallback(VBlankCB);
        SetMainCallback2(MainCB2);
        return;
    }
}

static void MainCB2(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
}

static void VBlankCB(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

static void Task_MoreOptionsMenuFadeIn(u8 taskId)
{
    if (!gPaletteFade->active)
        gTasks[taskId].func = Task_MoreOptionsMenuProcessInput;
}

static void Task_MoreOptionsMenuProcessInput(u8 taskId)
{
    if (gMain.newKeys & (A_BUTTON | B_BUTTON))
    {
        gTasks[taskId].func = Task_MoreOptionsMenuSave;
    }
}

static void Task_MoreOptionsMenuSave(u8 taskId)
{
    BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 0x10, RGB_BLACK);
    gTasks[taskId].func = Task_MoreOptionsMenuFadeOut;
}

static void Task_MoreOptionsMenuFadeOut(u8 taskId)
{
    if (!gPaletteFade->active)
    {
        DestroyTask(taskId);
        FreeAllWindowBuffers();
        SetMainCallback2(gMain.savedCallback);
    }
}

static void DrawTextAdditionalOptions(void)
{
    FillWindowPixelBuffer(WIN_TEXT_OPTION, PIXEL_FILL(1));
    AddTextPrinterParameterized(WIN_TEXT_OPTION, 1, gText_AdditionalOptions, 8, 1, TEXT_SPEED_FF, NULL);
    CopyWindowToVram(WIN_TEXT_OPTION, 3);
}

static void DrawMoreOptionsMenuTexts(void)
{
    u8 i;

    FillWindowPixelBuffer(WIN_OPTIONS, PIXEL_FILL(1));
    for (i = 0; i < MENUITEM_COUNT; i++)
    {
        AddTextPrinterParameterized(WIN_OPTIONS, 1, sMoreOptionsMenuItemsNames[i], 8, (i * 16) + 1, TEXT_SPEED_FF, NULL);
    }
    CopyWindowToVram(WIN_OPTIONS, 3);
}

static void DrawChoices(u8 taskId)
{
	s16* choices = gTasks[taskId].data;
    WindowPrint(WIN_OPTIONS, 2, 0x82, (0 * 16) + 1, (void*) 0x83CC363, TEXT_SPEED_FF, sDifficultyOptionNames[choices[TD_DIFFICULTY]]);
	WindowPrint(WIN_OPTIONS, 2, 0x82, (1 * 16) + 1, (void*) 0x83CC363, TEXT_SPEED_FF, sStoryModeNames[choices[TD_STORYMODE]]);
	WindowPrint(WIN_OPTIONS, 2, 0x82, (2 * 16) + 1, (void*) 0x83CC363, TEXT_SPEED_FF, sOnOff[choices[TD_LEVELLIMIT]]);
	WindowPrint(WIN_OPTIONS, 2, 0x82, (3 * 16) + 1, (void*) 0x83CC363, TEXT_SPEED_FF, sOnOff[choices[TD_FLAGTOTEST]]);
	WindowPrint(WIN_OPTIONS, 2, 0x82, (4 * 16) + 1, (void*) 0x83CC363, TEXT_SPEED_FF, sOnOff[choices[TD_POKEDEXGIVEN]]);
	WindowPrint(WIN_OPTIONS, 2, 0x82, (5 * 16) + 1, (void*) 0x83CC363, TEXT_SPEED_FF, sOnOff[choices[TD_WILDDOUBLESACTIVATED]]);
}

static void sub_80BB154(void)
{
    //                   bg, tileNum, x,    y,    width, height,  pal
    FillBgTilemapBufferRect(1, 0x1A2, 1,    0,      1,      1,      7);
    FillBgTilemapBufferRect(1, 0x1A3, 2,    0,      0x1B,   1,      7);
    FillBgTilemapBufferRect(1, 0x1A4, 28,   0,      1,      1,      7);
    FillBgTilemapBufferRect(1, 0x1A5, 1,    1,      1,      2,      7);
    FillBgTilemapBufferRect(1, 0x1A7, 28,   1,      1,      2,      7);
    FillBgTilemapBufferRect(1, 0x1A8, 1,    3,      1,      1,      7);
    FillBgTilemapBufferRect(1, 0x1A9, 2,    3,      0x1B,   1,      7);
    FillBgTilemapBufferRect(1, 0x1AA, 28,   3,      1,      1,      7);
    FillBgTilemapBufferRect(1, 0x1A2, 1,    4,      1,      1,      7);
    FillBgTilemapBufferRect(1, 0x1A3, 2,    4,      0x1A,   1,      7);
    FillBgTilemapBufferRect(1, 0x1A4, 28,   4,      1,      1,      7);
    FillBgTilemapBufferRect(1, 0x1A5, 1,    5,      1,      0x12,   7);
    FillBgTilemapBufferRect(1, 0x1A7, 28,   5,      1,      0x12,   7);
    FillBgTilemapBufferRect(1, 0x1A8, 1,    19,     1,      1,      7);
    FillBgTilemapBufferRect(1, 0x1A9, 2,    19,     0x1A,   1,      7);
    FillBgTilemapBufferRect(1, 0x1AA, 28,   19,     1,      1,      7);

    CopyBgTilemapBufferToVram(1);
}

int GetStringRightAlignXOffset(int fontId, const u8 *str, int totalWidth)
{
    return GetStringWidthDifference(fontId, str, totalWidth, 0);
}

int GetStringWidthDifference(int fontId, const u8 *str, int totalWidth, int letterSpacing)
{
    int stringWidth = GetStringWidth(fontId, str, letterSpacing);
    if (totalWidth > stringWidth)
        return totalWidth - stringWidth;
    else
        return 0;
}
