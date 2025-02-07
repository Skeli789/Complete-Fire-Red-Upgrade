#include "defines.h"
#include "defines_battle.h"
#include "../include/options_menu.h"
#include "../include/battle_setup.h"
#include "../include/event_data.h"
#include "../include/pokemon.h"
#include "../include/pokemon_storage_system.h"
#include "../include/random.h"
#include "../include/script.h"
#include "../include/string_util.h"
#include "../include/wild_encounter.h"
#include "../include/constants/event_objects.h"
#include "../include/constants/items.h"
#include "../include/constants/maps.h"
#include "../include/constants/pokedex.h"
#include "../include/constants/region_map_sections.h"
#include "../include/constants/species.h"
#include "../include/constants/trainer_classes.h"
#include "../include/constants/trainers.h"
#include "../include/constants/tutors.h"
#include "../include/new/ability_tables.h"
#include "../include/new/ai_advanced.h"
#include "../include/new/build_pokemon.h"
#include "../include/new/build_pokemon_2.h"
#include "../include/new/catching.h"
#include "../include/new/dexnav.h"
#include "../include/new/dynamax.h"
#include "../include/new/form_change.h"
#include "../include/new/frontier.h"
#include "../include/new/item.h"
#include "../include/new/learn_move.h"
#include "../include/new/mega.h"
#include "../include/new/multi.h"
#include "../include/new/pokemon_storage_system.h"
#include "../include/new/util2.h"
#include "../include/text.h"
#include "../include/overworld.h"
#include "../include/tm_case.h"
#include "../include/text_window.h"
#include "../include/bg.h"
#include "../include/pokemon_summary_screen.h"
#include "../include/menu.h"
#include "../include/new/battle_strings.h"
#include "../include/pokedex.h"
#include "../include/base_stats.h"
#include "../include/menu_helpers.h"
#include "../include/gba/m4a_internal.h"
#include "../include/malloc.h"
#include "../include/mgba.h"
#include "../include/sound.h"
#include "../include/constants/songs.h"

void CB2_OptionMenu(void);
bool8 LoadOptionMenuPalette(void);
void LoadOptionMenuItemNames(void);
void Task_OptionMenu(u8 taskId);
void CloseAndSaveOptionMenu(u8 taskId);
u8 OptionMenu_ProcessInput(void);
void BufferOptionMenuString(u8 selection);
void OptionMenu_PickSwitchCancel(void);

// Menu items
enum
{
    MENUITEM_TEXTSPEED = 0,
    MENUITEM_BATTLESCENE,
    MENUITEM_BATTLESTYLE,
    MENUITEM_SOUND,
    MENUITEM_BUTTONMODE,
    MENUITEM_FRAMETYPE,
    MENUITEM_CANCEL,
    MENUITEM_COUNT,
};

enum
{
    MENUITEM_RBUTTONMODE = 0,
    MENUITEM_BATTLEMUSIC,
    MENUITEM_WILDLEVELSCALING,
    MENUITEM_AUTOSORTBAG,
    MENUITEM_CANCEL_PAGE_2,
    MENUITEM_PAGE2_COUNT,
};

// Window Ids
enum
{
    WIN_TEXT_OPTION,
    WIN_OPTIONS
};

struct OptionMenu
{
    /*0x00*/ u16 option[MENUITEM_COUNT];
    /*0x??*/ u16 cursorPos;
    /*0x??*/ u8 loadState;
    /*0x??*/ u8 state;
    /*0x??*/ u8 loadPaletteState;
    /*0x??*/ u8 page;
    /*0x??*/ u16 option_secondPage[MENUITEM_PAGE2_COUNT];
};

extern struct OptionMenu *sOptionMenuPtr;
extern const u8 gText_TextSpeed[];
extern const u8 gText_BattleScene[];
extern const u8 gText_BattleStyle[];
extern const u8 gText_Sound[];
extern const u8 gText_ButtonMode[];
extern const u8 gText_Frame[];
extern const u8 gText_OptionMenuCancel[];
extern const u8 gText_RButtonMode[];
extern const u8 gText_BattleMusic[];
extern const u8 gText_WildLevelScaling[];
extern const u8 gText_AutoSortBag[];

static const u8 *const sOptionMenuItemsNames[MENUITEM_COUNT] =
{
    [MENUITEM_TEXTSPEED]   = gText_TextSpeed,
    [MENUITEM_BATTLESCENE] = gText_BattleScene,
    [MENUITEM_BATTLESTYLE] = gText_BattleStyle,
    [MENUITEM_SOUND]       = gText_Sound,
    [MENUITEM_BUTTONMODE]  = gText_ButtonMode,
    [MENUITEM_FRAMETYPE]   = gText_Frame,
    [MENUITEM_CANCEL]      = gText_OptionMenuCancel,
};
static const u8 *const sOptionMenuItemsNames_SecondPage[MENUITEM_COUNT] =
{
    [MENUITEM_RBUTTONMODE] = gText_RButtonMode,
    [MENUITEM_BATTLEMUSIC] = gText_BattleMusic,
    [MENUITEM_WILDLEVELSCALING] = gText_WildLevelScaling,
    [MENUITEM_AUTOSORTBAG] = gText_AutoSortBag,
    [MENUITEM_CANCEL_PAGE_2] = gText_OptionMenuCancel,
};

extern const u8 gText_TextSpeedSlow[];
extern const u8 gText_TextSpeedMid[];
extern const u8 gText_TextSpeedFast[];
extern const u8 gText_BattleSceneOn[];
extern const u8 gText_BattleSceneOff[];
extern const u8 gText_BattleStyleShift[];
extern const u8 gText_BattleStyleSet[];
extern const u8 gText_SoundMono[];
extern const u8 gText_SoundStereo[];
extern const u8 gText_ButtonTypeHelp[];
extern const u8 gText_ButtonTypeLR[];
extern const u8 gText_ButtonTypeLEqualsA[];
extern const u8 gText_MenuOption[];
extern const u8 gText_FrameType[];
extern const u8 gText_RButtonDexNav[];
extern const u8 gText_RButtonPokemon[];
extern const u8 gText_RButtonItems[];
extern const u8 gText_FRLG[];
extern const u8 gText_RSE[];
extern const u8 gText_OnOption[];
extern const u8 gText_OffOption[];
extern const u8 gText_ByName[];
extern const u8 gText_ByType[];
extern const u8 gText_ByAmount[];

static const u8 *const sTextSpeedOptions[] =
{
    gText_TextSpeedSlow, 
    gText_TextSpeedMid, 
    gText_TextSpeedFast
};
static const u8 *const sBattleSceneOptions[] =
{
    gText_BattleSceneOn, 
    gText_BattleSceneOff
};
static const u8 *const sBattleStyleOptions[] =
{
    gText_BattleStyleShift,
    gText_BattleStyleSet
};
static const u8 *const sSoundOptions[] =
{
    gText_SoundMono, 
    gText_SoundStereo
};
static const u8 *const sButtonTypeOptions[] =
{
    gText_ButtonTypeHelp,
	gText_ButtonTypeLR,
	gText_ButtonTypeLEqualsA
};

static const u8 *const sRButtonModeOptions[] =
{
    gText_RButtonDexNav,
	gText_RButtonPokemon,
	gText_RButtonItems
};

static const u8 *const sBattleMusicOptions[] =
{
    gText_FRLG,
	gText_RSE
};
static const u8 *const sWildScalingOptions[] =
{
    gText_OnOption,
	gText_OffOption
};

static const u8 *const sAutoSortBagOptions[] =
{
    gText_OffOption,
	gText_ByName,
    gText_ByType,
    gText_ByAmount,
};

static const u16 sOptionMenuItemCounts[MENUITEM_COUNT] = {3, 2, 2, 2, 3, 10, 0};
static const u16 sOptionMenuItemCounts_SecondPage[MENUITEM_PAGE2_COUNT] = {3, 2, 2, 4, 0};

void CB2_OptionsMenuFromStartMenu(void)
{
    u8 i;
    
    if (gMain.savedCallback == NULL)
        gMain.savedCallback = CB2_ReturnToFieldWithOpenMenu;
    sOptionMenuPtr = AllocZeroed(sizeof(struct OptionMenu));
    sOptionMenuPtr->loadState = 0;
    sOptionMenuPtr->loadPaletteState = 0;
    sOptionMenuPtr->state = 0;
    sOptionMenuPtr->cursorPos = 0;
    sOptionMenuPtr->option[MENUITEM_TEXTSPEED] = gSaveBlock2->optionsTextSpeed;
    sOptionMenuPtr->option[MENUITEM_BATTLESCENE] = gSaveBlock2->optionsBattleSceneOff;
    sOptionMenuPtr->option[MENUITEM_BATTLESTYLE] = gSaveBlock2->optionsBattleStyle;
    sOptionMenuPtr->option[MENUITEM_SOUND] = gSaveBlock2->optionsSound;
    sOptionMenuPtr->option[MENUITEM_BUTTONMODE] = gSaveBlock2->optionsButtonMode;
    sOptionMenuPtr->option[MENUITEM_FRAMETYPE] = gSaveBlock2->optionsWindowFrameType;
    sOptionMenuPtr->option_secondPage[MENUITEM_RBUTTONMODE] = VarGet(VAR_R_BUTTON_MODE);
    sOptionMenuPtr->option_secondPage[MENUITEM_BATTLEMUSIC] = VarGet(VAR_BATTLE_MUSIC);
    sOptionMenuPtr->option_secondPage[MENUITEM_WILDLEVELSCALING] = VarGet(VAR_WILD_LEVEL_SCALING);
    sOptionMenuPtr->option_secondPage[MENUITEM_AUTOSORTBAG] = VarGet(VAR_AUTO_SORT_BAG);
    
    for (i = 0; i < MENUITEM_COUNT - 1; i++)
    {
        if (sOptionMenuPtr->option[i] > (sOptionMenuItemCounts[i]) - 1)
            sOptionMenuPtr->option[i] = 0;
    }
    SetMainCallback2(CB2_OptionMenu);
}

void Task_OptionMenu(u8 taskId)
{
    u8 i;
    switch (sOptionMenuPtr->loadState)
    {
    case 0:
        BeginNormalPaletteFade(0xFFFFFFFF, 0, 0x10, 0, RGB_BLACK);
        OptionMenu_SetVBlankCallback();
        sOptionMenuPtr->loadState++;
        break;
    case 1:
        if (gPaletteFade->active)
            return;
        sOptionMenuPtr->loadState++;
        break;
    case 2:
        if (((bool32)MenuHelpers_CallLinkSomething()) == TRUE)
            break;
        switch (OptionMenu_ProcessInput())
        {
        case 0:
            break;
        case 1:
            sOptionMenuPtr->loadState++;
            break;
        case 2:
            LoadBgTiles(1, GetUserFrameGraphicsInfo(sOptionMenuPtr->option[MENUITEM_FRAMETYPE])->tiles, 0x120, 0x1AA);
            LoadPalette(GetUserFrameGraphicsInfo(sOptionMenuPtr->option[MENUITEM_FRAMETYPE])->palette, 0x20, 0x20);
            BufferOptionMenuString(sOptionMenuPtr->cursorPos);
            break;
        case 3:
            UpdateSettingSelectionDisplay(sOptionMenuPtr->cursorPos);
            break;
        case 4:
            BufferOptionMenuString(sOptionMenuPtr->cursorPos);
            break;
        case 5:
            sOptionMenuPtr->page = 1;
            LoadOptionMenuItemNames();
            for(i = 0; i < MENUITEM_PAGE2_COUNT; i++)
            BufferOptionMenuString(i);
            sOptionMenuPtr->cursorPos = 0;
            UpdateSettingSelectionDisplay(sOptionMenuPtr->cursorPos);
            OptionMenu_PickSwitchCancel();
            break;
        case 6:
            sOptionMenuPtr->page = 0;
            LoadOptionMenuItemNames();
            for(i = 0; i < MENUITEM_COUNT; i++)
            BufferOptionMenuString(i);
            sOptionMenuPtr->cursorPos = 0;
            UpdateSettingSelectionDisplay(sOptionMenuPtr->cursorPos);
            OptionMenu_PickSwitchCancel();
            break;
        }
        break;
    case 3:
        BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 0x10, RGB_BLACK);
        sOptionMenuPtr->loadState++;
        break;
    case 4:
        if (gPaletteFade->active)
            return;
        sOptionMenuPtr->loadState++;
        break;
    case 5:
        CloseAndSaveOptionMenu(taskId);
        break;
    }
}
void CloseAndSaveOptionMenu(u8 taskId)
{
    gFieldCallback = FieldCB_DefaultWarpExit;
    SetMainCallback2(gMain.savedCallback);
    FreeAllWindowBuffers();
    gSaveBlock2->optionsTextSpeed = sOptionMenuPtr->option[MENUITEM_TEXTSPEED];
    gSaveBlock2->optionsBattleSceneOff = sOptionMenuPtr->option[MENUITEM_BATTLESCENE];
    gSaveBlock2->optionsBattleStyle = sOptionMenuPtr->option[MENUITEM_BATTLESTYLE];
    gSaveBlock2->optionsSound = sOptionMenuPtr->option[MENUITEM_SOUND];
    gSaveBlock2->optionsButtonMode = sOptionMenuPtr->option[MENUITEM_BUTTONMODE];
    gSaveBlock2->optionsWindowFrameType = sOptionMenuPtr->option[MENUITEM_FRAMETYPE];
    VarSet(VAR_R_BUTTON_MODE, sOptionMenuPtr->option_secondPage[MENUITEM_RBUTTONMODE]);
    VarSet(VAR_BATTLE_MUSIC, sOptionMenuPtr->option_secondPage[MENUITEM_BATTLEMUSIC]);
    VarSet(VAR_WILD_LEVEL_SCALING, sOptionMenuPtr->option_secondPage[MENUITEM_WILDLEVELSCALING]);
    VarSet(VAR_AUTO_SORT_BAG, sOptionMenuPtr->option_secondPage[MENUITEM_AUTOSORTBAG]);
    SetPokemonCryStereo(gSaveBlock2->optionsSound);
    FREE_AND_SET_NULL(sOptionMenuPtr);
    DestroyTask(taskId);
}

void CB2_OptionMenu(void)
{
    u8 i, state;
    state = sOptionMenuPtr->state;
    switch (state)
    {
    case 0:
        OptionMenu_InitCallbacks();
        break;
    case 1:
        InitOptionMenuBg();
        break;
    case 2:
        OptionMenu_ResetSpriteData();
        break;
    case 3:
        if (LoadOptionMenuPalette() != TRUE)
            return;
        break;
    case 4:
        PrintOptionMenuHeader();
        break;
    case 5:
        DrawOptionMenuBg();
        break;
    case 6:
        LoadOptionMenuItemNames();
        break;
    case 7:
        for (i = 0; i < MENUITEM_COUNT; i++)
            BufferOptionMenuString(i);
        break;
    case 8:
        UpdateSettingSelectionDisplay(sOptionMenuPtr->cursorPos);
        break;
    case 9:
        OptionMenu_PickSwitchCancel();
        break;
    default:
        SetOptionMenuTask();
		break;
    }
    sOptionMenuPtr->state++;
}

static const u8 sOptionMenuPickSwitchCancelTextColor[] = {TEXT_DYNAMIC_COLOR_6, TEXT_COLOR_WHITE, TEXT_COLOR_DARK_GREY};
static const u8 sOptionMenuTextColor[] = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_LIGHT_RED, TEXT_COLOR_RED};

void BufferOptionMenuString(u8 selection)
{
    u8 str[20];
    u8 buf[12];
    u8 dst[3];
    u8 x, y;
    
    memcpy(dst, sOptionMenuTextColor, 3);
    x = 0x82;
    y = ((GetFontAttribute(2, FONTATTR_MAX_LETTER_HEIGHT) - 1) * (selection)) + 2;
    FillWindowPixelRect(1, 1, x, y, 0x46, GetFontAttribute(2, FONTATTR_MAX_LETTER_HEIGHT));
    if(sOptionMenuPtr->page == 0)
    {
        switch (selection)
        {
        case MENUITEM_TEXTSPEED:
            AddTextPrinterParameterized3(1, 2, x, y, dst, -1, sTextSpeedOptions[sOptionMenuPtr->option[selection]]);
            break;
        case MENUITEM_BATTLESCENE:
            AddTextPrinterParameterized3(1, 2, x, y, dst, -1, sBattleSceneOptions[sOptionMenuPtr->option[selection]]);
            break;
        case MENUITEM_BATTLESTYLE:
            AddTextPrinterParameterized3(1, 2, x, y, dst, -1, sBattleStyleOptions[sOptionMenuPtr->option[selection]]);
            break;
        case MENUITEM_SOUND:
            AddTextPrinterParameterized3(1, 2, x, y, dst, -1, sSoundOptions[sOptionMenuPtr->option[selection]]);
            break;
        case MENUITEM_BUTTONMODE:
            AddTextPrinterParameterized3(1, 2, x, y, dst, -1, sButtonTypeOptions[sOptionMenuPtr->option[selection]]);
            break;
        case MENUITEM_FRAMETYPE:
            StringCopy(str, gText_FrameType);
            ConvertIntToDecimalStringN(buf, sOptionMenuPtr->option[selection] + 1, 1, 2);
            StringAppendN(str, buf, 3);
            AddTextPrinterParameterized3(1, 2, x, y, dst, -1, str);
            break;
        
        default:
            break;
        }
    }
    else
    {
        switch (selection)
        {
            case MENUITEM_RBUTTONMODE:
                AddTextPrinterParameterized3(1, 2, x, y, dst, -1, sRButtonModeOptions[sOptionMenuPtr->option_secondPage[selection]]);
                break;
            case MENUITEM_BATTLEMUSIC:
                AddTextPrinterParameterized3(1, 2, x, y, dst, -1, sBattleMusicOptions[sOptionMenuPtr->option_secondPage[selection]]);
                break;
            case MENUITEM_WILDLEVELSCALING:
                AddTextPrinterParameterized3(1, 2, x, y, dst, -1, sWildScalingOptions[sOptionMenuPtr->option_secondPage[selection]]);
                break;
            case MENUITEM_AUTOSORTBAG:
                AddTextPrinterParameterized3(1, 2, x, y, dst, -1, sAutoSortBagOptions[sOptionMenuPtr->option_secondPage[selection]]);
                break;
            default:
                break;
        }
    }
    PutWindowTilemap(1);
    CopyWindowToVram(1, COPYWIN_BOTH);
}

u8 OptionMenu_ProcessInput(void)
{ 
    u16 current;
    u16* curr;
    if (JOY_REPT(DPAD_RIGHT))
    {
        if(sOptionMenuPtr->page == 0)
        {
            current = sOptionMenuPtr->option[(sOptionMenuPtr->cursorPos)];
            if (current == (sOptionMenuItemCounts[sOptionMenuPtr->cursorPos] - 1))
                sOptionMenuPtr->option[sOptionMenuPtr->cursorPos] = 0;
            else
                sOptionMenuPtr->option[sOptionMenuPtr->cursorPos] = current + 1;
            if (sOptionMenuPtr->cursorPos == MENUITEM_FRAMETYPE)
                return 2;
            else
                return 4;
        }
        else
        {
            current = sOptionMenuPtr->option_secondPage[(sOptionMenuPtr->cursorPos)];
            if (current == (sOptionMenuItemCounts_SecondPage[sOptionMenuPtr->cursorPos] - 1))
                sOptionMenuPtr->option_secondPage[sOptionMenuPtr->cursorPos] = 0;
            else
                sOptionMenuPtr->option_secondPage[sOptionMenuPtr->cursorPos] = current + 1;
            return 4;
        }
    }
    else if (JOY_REPT(DPAD_LEFT))
    {
        if(sOptionMenuPtr->page == 0)
        {
            curr = &sOptionMenuPtr->option[sOptionMenuPtr->cursorPos];
            if (*curr == 0)
                *curr = sOptionMenuItemCounts[sOptionMenuPtr->cursorPos] - 1;
            else
                --*curr;
            
            if (sOptionMenuPtr->cursorPos == MENUITEM_FRAMETYPE)
                return 2;
            else
                return 4;
        }
        else
        {
            curr = &sOptionMenuPtr->option_secondPage[sOptionMenuPtr->cursorPos];
            if (*curr == 0)
                *curr = sOptionMenuItemCounts_SecondPage[sOptionMenuPtr->cursorPos] - 1;
            else
                --*curr;

            return 4;
        }
    }
    else if (JOY_REPT(DPAD_UP))
    {
        if(sOptionMenuPtr->page == 0)
        {
            if (sOptionMenuPtr->cursorPos == MENUITEM_TEXTSPEED)
                sOptionMenuPtr->cursorPos = MENUITEM_CANCEL;
            else
                sOptionMenuPtr->cursorPos = sOptionMenuPtr->cursorPos - 1;
        }
        else
        {
            if (sOptionMenuPtr->cursorPos == MENUITEM_RBUTTONMODE)
                sOptionMenuPtr->cursorPos = MENUITEM_CANCEL_PAGE_2;
            else
                sOptionMenuPtr->cursorPos = sOptionMenuPtr->cursorPos - 1;
        }
        return 3;        
    }
    else if (JOY_REPT(DPAD_DOWN))
    {
        if(sOptionMenuPtr->page == 0)
        {
            if (sOptionMenuPtr->cursorPos == MENUITEM_CANCEL)
                sOptionMenuPtr->cursorPos = MENUITEM_TEXTSPEED;
            else
                sOptionMenuPtr->cursorPos = sOptionMenuPtr->cursorPos + 1;
        }
        else
        {
            if (sOptionMenuPtr->cursorPos == MENUITEM_CANCEL_PAGE_2)
                sOptionMenuPtr->cursorPos = MENUITEM_RBUTTONMODE;
            else
                sOptionMenuPtr->cursorPos = sOptionMenuPtr->cursorPos + 1;
        }
        return 3;
    }
    else if (JOY_NEW(R_BUTTON))
    {   if(sOptionMenuPtr->page == 1)
        return 0;
        sOptionMenuPtr->page = 1;
        PlaySE(SE_SELECT);
        return 5;
    }
    else if (JOY_NEW(L_BUTTON))
    {   if(sOptionMenuPtr->page == 0)
        return 0;
        sOptionMenuPtr->page = 0;
        PlaySE(SE_SELECT);
        return 6;
    }
    else if (JOY_NEW(B_BUTTON) || JOY_NEW(A_BUTTON))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
extern const u16 sOptionMenuPalette[];
bool8 LoadOptionMenuPalette(void)
{
    switch (sOptionMenuPtr->loadPaletteState)
    {
    case 0:
        LoadBgTiles(1, GetUserFrameGraphicsInfo(sOptionMenuPtr->option[MENUITEM_FRAMETYPE])->tiles, 0x120, 0x1AA);
        break;
    case 1:
        LoadPalette(GetUserFrameGraphicsInfo(sOptionMenuPtr->option[MENUITEM_FRAMETYPE])->palette, 0x20, 0x20);
        break;
    case 2:
        LoadPalette(sOptionMenuPalette, 0x10, 0x20);
        LoadPalette(stdpal_get(2), 0xF0, 0x20);
        break;
    case 3:
        DrawWindowBorderWithStdpal3(1, 0x1B3, 0x30);
        break;
    default:
        return TRUE;
    }
    sOptionMenuPtr->loadPaletteState++;
    return FALSE;
}
void LoadOptionMenuItemNames(void)
{
    u8 i;
    
    FillWindowPixelBuffer(1, PIXEL_FILL(1));
    if(sOptionMenuPtr->page == 0)
    {
        for (i = 0; i < MENUITEM_COUNT; i++)
        {
            AddTextPrinterParameterized(WIN_OPTIONS, 2, sOptionMenuItemsNames[i], 8, (u8)((i * (GetFontAttribute(2, FONTATTR_MAX_LETTER_HEIGHT))) + 2) - i, TEXT_SPEED_FF, NULL);    
        }
    }
    else
    {
        for (i = 0; i < MENUITEM_PAGE2_COUNT; i++)
        {
            AddTextPrinterParameterized(WIN_OPTIONS, 2, sOptionMenuItemsNames_SecondPage[i], 8, (u8)((i * (GetFontAttribute(2, FONTATTR_MAX_LETTER_HEIGHT))) + 2) - i, TEXT_SPEED_FF, NULL);       
        } 
    }
}

extern const u8 gText_PickSwitchCancel_Page1[];
extern const u8 gText_PickSwitchCancel_Page2[];

void OptionMenu_PickSwitchCancel(void)
{
    s32 x;
    if(sOptionMenuPtr->page == 0)
    {
        x = 0xE4 - GetStringWidth(0, gText_PickSwitchCancel_Page1, 0);
        FillWindowPixelBuffer(2, PIXEL_FILL(15)); 
        AddTextPrinterParameterized3(2, 0, x, 0, sOptionMenuPickSwitchCancelTextColor, 0, gText_PickSwitchCancel_Page1);
        PutWindowTilemap(2);
        CopyWindowToVram(2, COPYWIN_BOTH);   
    }
    else
    {
        x = 0xE4 - GetStringWidth(0, gText_PickSwitchCancel_Page2, 0);
        FillWindowPixelBuffer(2, PIXEL_FILL(15)); 
        AddTextPrinterParameterized3(2, 0, x, 0, sOptionMenuPickSwitchCancelTextColor, 0, gText_PickSwitchCancel_Page2);
        PutWindowTilemap(2);
        CopyWindowToVram(2, COPYWIN_BOTH);
    }
}