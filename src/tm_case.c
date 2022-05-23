#include "defines.h"
#include "../include/random.h"
#include "../include/constants/abilities.h"
#include "../include/global.h"

#include "../include/new/damage_calc.h"
#include "../include/new/evolution.h"
#include "../include/new/util2.h"
#include "../include/new/frontier.h"
#include "../include/new/mega.h"
#include "../include/new/dynamax.h"
#include "../include/constants/items.h"
#include "../include/constants/moves.h"
#include "../include/base_stats.h"
#include "../include/tm_case.h"
#include "../include/menu_helpers.h"
#include "../include/scanline_effect.h"
#include "../include/sprite.h"
#include "../include/palette.h"
#include "../include/task.h"
#include "../include/item.h"
#include "../include/list_menu.h"
#include "../include/new/item.h"
#include "../include/list_menu.h"
#include "../include/mgba.h"
#include "../include/malloc.h"
#include "../include/menu.h"
#include "../include/pokemon_icon.h"
#include "../include/window.h"
#include "../include/string_util.h"
#include "../include/new/Vanilla_functions.h"
#include "../include/text.h"
#include "../include/gba/io_reg.h"

/*
tm_case.c
	adds party icons to the tm case
*/

/*extern const u8 gMoveNames[][12 + 1];

static void DrawPartyMonIcons(void);
static void TintPartyMonIcons(u8 tm);
static void DestroyPartyMonIcons(void);

//#define spriteIdData ((u8*) 0x203E200);
//#define spriteIdData ((u8*) *((u8*)0x203E200))
//#define spriteIdPaletteTest ((u16*) 0x203E400);
//#define spriteIdPalette ((u16*) *((u16*) 0x203E400))
extern u8    spriteIdData[PARTY_SIZE] = {};
extern u16   spriteIdPalette[PARTY_SIZE] = {};

extern struct ListMenuItem * sListMenuItemsBuffer = NULL;
extern u8 (* sListMenuStringsBuffer)[29];

struct UnkStruct_203B10C
{
    void (* savedCallback)(void);
    u8 tmCaseMenuType;
    u8 unk_05;
    u8 unk_06;
    u16 selectedRow;
    u16 scrollOffset;
};

struct UnkStruct_203B118
{
    void (* savedCallback)(void);
    u8 tmSpriteId;
    u8 maxTMsShown;
    u8 numTMs;
    u8 contextMenuWindowId;
    u8 scrollIndicatorArrowPairId;
    u16 currItem;
    const u8 * menuActionIndices;
    u8 numMenuActions;
    s16 seqId;
    u8 filler_14[8];
};

extern struct UnkStruct_203B118 * sTMCaseDynamicResources;
extern struct UnkStruct_203B10C * sTMCaseStaticResources;


static void LoadTMTypePalettes(void);
static void DrawPartyMonIcons(void);
static void TintPartyMonIcons(u8 tm);
static void DestroyPartyMonIcons(void);

extern const struct SpritePalette gMonIconPaletteTable[6];
extern const u16 sSpriteImageSizes[3][4];
/*extern const u8 gText_TMCaseWillBePutAway[];
extern const u8 gUnknown_8E99118[];

void SortPocketAndPlaceHMsFirst()
{
    u16 i;
    u16 j = 0;
    u16 k;
    struct ItemSlot * buff;


    SortBerriesOrTMHMs(gBagPockets->tmRam);


    for (i = 0; i < gBagPockets->tmAmount; i++)
    {
        if (gBagPockets->tmRam[i].itemId == ITEM_NONE && GetBagItemQuantity(&gBagPockets->tmRam[i].quantity) == 0)
            return;
        if (gBagPockets->tmRam[i].itemId >= ITEM_HM01_CUT && GetBagItemQuantity(&gBagPockets->tmRam[i].quantity) != 0)
        {
            for (j = i + 1; j < gBagPockets->tmAmount; j++)
            {
                if (gBagPockets->tmRam[j].itemId == ITEM_NONE && GetBagItemQuantity(&gBagPockets->tmRam[j].quantity) == 0)
                    break;
            }
            break;
        }
    }



    for (k = 0; k < gBagPockets->tmAmount; k++)
        gBagPockets->tmRam[k].quantity = GetBagItemQuantity(&gBagPockets->tmRam[k].quantity);

    buff = AllocZeroed(NUM_TMSHMS * sizeof(struct ItemSlot));

    CpuCopy16(gBagPockets->tmRam + i, buff, (j - i) * sizeof(struct ItemSlot));
    CpuCopy16(gBagPockets->tmRam, buff + (j - i), i * sizeof(struct ItemSlot));

    CpuCopy16(buff, gBagPockets->tmRam, gBagPockets->tmAmount * sizeof(struct ItemSlot));

    for (k = 0; k < gBagPockets->tmAmount; k++)
        SetBagItemQuantity(&gBagPockets->tmRam[k].quantity, gBagPockets->tmRam[k].quantity);
    Free(buff);


}

bool8 DoSetUpTMCaseUI(void)
{
    u8 taskId;



    switch (gMain.state)
    {
    case 0:
        SetVBlankHBlankCallbacksToNull();
        ClearScheduledBgCopiesToVram();
        gMain.state++;
        mgba_printf(MGBA_LOG_INFO, "Calculating doubles killing moves...");
        break;
    case 1:
        ScanlineEffect_Stop();
        gMain.state++;
        mgba_printf(MGBA_LOG_INFO, "Calculating doubles killing moves...");
        break;
    case 2:
        FreeAllSpritePalettes();
        gMain.state++;
        mgba_printf(MGBA_LOG_INFO, "Calculating doubles killing moves...");
        break;
    case 3:
        ResetPaletteFade();
        gMain.state++;
        mgba_printf(MGBA_LOG_INFO, "Calculating doubles killing moves...");
        break;
    case 4:
        ResetSpriteData();
        gMain.state++;
        mgba_printf(MGBA_LOG_INFO, "Calculating doubles killing moves...");
        break;
    case 5:
        ResetTasks();
        gMain.state++;
        mgba_printf(MGBA_LOG_INFO, "Calculating doubles killing moves...");
        break;
    case 6:
        LoadBGTemplates();
        sTMCaseDynamicResources->seqId = 0;
        gMain.state++;
        mgba_printf(MGBA_LOG_INFO, "Calculating doubles killing moves...");
        break;
    case 7:
        InitWindowTemplatesAndPals();
        gMain.state++;
        mgba_printf(MGBA_LOG_INFO, "Calculating doubles killing moves...");
        break;
    case 8:
        if (HandleLoadTMCaseGraphicsAndPalettes())
            gMain.state++;
        break;
    case 9:
        //SortPocketAndPlaceHMsFirst();
        gMain.state++;
        break;
    case 10:
        TMCaseSetup_GetTMCount();
        TMCaseSetup_InitListMenuPositions();
        TMCaseSetup_UpdateVisualMenuOffset();
        gMain.state++;
        break;
    case 11:
        DrawMoveInfoUIMarkers();
        DrawPartyMonIcons();
        gMain.state++;
        break;
    case 12:
        CreateTMCaseListMenuBuffers();
        InitTMCaseListMenuItems();
        gMain.state++;
        break;
    case 13:
        PrintStringTMCaseOnWindow3();
        gMain.state++;
        break;
    case 14:
        if (sTMCaseStaticResources->tmCaseMenuType == 4)
            taskId = CreateTask(Task_TMCaseDude1, 0);
        else
            taskId = CreateTask(Task_TMCaseMain, 0);
        gTasks[taskId].data[0] = ListMenuInit(gMultiuseListMenuTemplate, sTMCaseStaticResources->scrollOffset, sTMCaseStaticResources->selectedRow);
        gMain.state++;
        break;
    case 15:
        CreateTMCaseScrollIndicatorArrowPair_Main();
        gMain.state++;
        break;
    case 16:
        //sTMCaseDynamicResources->tmSpriteId = CreateTMSprite(BagGetItemIdByPocketPosition(POCKET_TM_CASE, sTMCaseStaticResources->scrollOffset + sTMCaseStaticResources->selectedRow));
        gMain.state++;
        break;
    case 17:
        BlendPalettes(0xFFFFFFFF, 16, 0);
        gMain.state++;
        break; 
    case 18:
        BeginNormalPaletteFade(0xFFFFFFFF, 0, 16, 0, RGB_BLACK);
        gMain.state++;
        break;
    default:
        SetVBlankCallback(VBlankCB_Idle);
        SetMainCallback2(CB2_Idle);
        return TRUE;
    }

    return FALSE;
}*/

/*extern const u8 gText_SelectorArrow2[];

void PrintListMenuCursorAt_WithColorIdx(u8 a0, u8 a1)
{
    if (a1 == 0xFF)
    {
        FillWindowPixelRect(0, 0, 0, a0, GetFontAttribute(2, 0), GetFontAttribute(2, 1));
        CopyWindowToVram(0, COPYWIN_GFX);
    }
    else
    {
        AddTextPrinterParameterized_ColorByIndex(0, 2, gText_SelectorArrow2, 40, a0, 0, 0, 0, a1);
    }
}

extern const u8 gText_FontSmall[];
extern const u8 sText_ClearTo18[];
extern const u8 gText_NumberClear01[];
extern const u8 sText_SingleSpace[];
extern const u8 gText_FontShort[];
/*void GetTMNumberAndMoveString(u8 * dest, u16 itemId)
{
    StringCopy(gStringVar4, gText_FontSmall);
    switch(BagGetItemIdByPocketPosition(POCKET_TM_CASE, itemId)) {
        case ITEM_HM01_CUT:
        case ITEM_HM02_FLY:
        case ITEM_HM03_SURF:
        case ITEM_HM04_STRENGTH:
        case ITEM_HM05_FLASH:
        case ITEM_HM06_ROCK_SMASH:
        case ITEM_HM07_WATERFALL:
        case ITEM_HM08_ROCK_CLIMB:
            StringAppend(gStringVar4, sText_ClearTo18);
            StringAppend(gStringVar4, gText_NumberClear01);
            ConvertIntToDecimalStringN(gStringVar1, itemId - ITEM_HM01_CUT + 1, STR_CONV_MODE_LEADING_ZEROS, 1);
            StringAppend(gStringVar4, gStringVar1);
            break;
        default:
        StringAppend(gStringVar4, gText_NumberClear01);
        ConvertIntToDecimalStringN(gStringVar1, itemId - ITEM_TM01_FOCUS_PUNCH + 1, STR_CONV_MODE_LEADING_ZEROS, 2);
        StringAppend(gStringVar4, gStringVar1);
        break;
    }

    StringAppend(gStringVar4, sText_SingleSpace);
    StringAppend(gStringVar4, gText_FontShort);
    StringAppend(gStringVar4, gMoveNames[ItemIdToBattleMoveId(itemId)]);
    StringCopy(dest, gStringVar4);
}*/




/*void TMCase_ItemPrintFunc(u8 windowId, u32 itemId, u8 y)
{
    if (itemId != -2)
    {
        switch(BagGetItemIdByPocketPosition(POCKET_TM_CASE, itemId)) {
            case ITEM_HM01_CUT:
            case ITEM_HM02_FLY:
            case ITEM_HM03_SURF:
            case ITEM_HM04_STRENGTH:
            case ITEM_HM05_FLASH:
            case ITEM_HM06_ROCK_SMASH:
            case ITEM_HM07_WATERFALL:
            case ITEM_HM08_ROCK_CLIMB:
                PlaceHMTileInWindow(windowId, 8, y);
                break;
        }
        AddTextPrinterParameterized_ColorByIndex(windowId, 0, gStringVar4, 0x99, y + 40, 0, 0, 0xFF, 1);
    }
}


void InitTMCaseListMenuItems(void)
{
    struct BagPocket * pocket = &gBagPockets->tmRam;
    u16 i;

    for (i = 0; i < sTMCaseDynamicResources->numTMs; i++)
    {
        GetTMNumberAndMoveString(sListMenuStringsBuffer[i], pocket->itemSlots[i].itemId);
        sListMenuItemsBuffer[i].name = sListMenuStringsBuffer[i];
        sListMenuItemsBuffer[i].id = i;
    }
    // sListMenuItemsBuffer[i].name = gText_Close;
    // sListMenuItemsBuffer[i].id = -2;
    gMultiuseListMenuTemplate->items = sListMenuItemsBuffer;
    gMultiuseListMenuTemplate->totalItems = sTMCaseDynamicResources->numTMs;
    gMultiuseListMenuTemplate->windowId = 0;
    gMultiuseListMenuTemplate->header_X = 0;
    gMultiuseListMenuTemplate->item_X = 8;
    gMultiuseListMenuTemplate->cursor_X = 0;
    gMultiuseListMenuTemplate->lettersSpacing = 0;
    gMultiuseListMenuTemplate->itemVerticalPadding = 2;
    gMultiuseListMenuTemplate->upText_Y = 2;
    gMultiuseListMenuTemplate->maxShowed = sTMCaseDynamicResources->maxTMsShown;
    gMultiuseListMenuTemplate->fontId = 2;
    gMultiuseListMenuTemplate->cursorPal = 2;
    gMultiuseListMenuTemplate->fillValue = 0;
    gMultiuseListMenuTemplate->cursorShadowPal = 3;
    gMultiuseListMenuTemplate->moveCursorFunc = TMCase_MoveCursorFunc;
    gMultiuseListMenuTemplate->itemPrintFunc = TMCase_ItemPrintFunc;
    gMultiuseListMenuTemplate->cursorKind = 0;
    gMultiuseListMenuTemplate->scrollMultiple = 0;
}*/
/*void PlaceHMTileInWindow(u8 windowId, u8 x, u8 y)
{
    BlitBitmapToWindow(windowId, gUnknown_8E99118, x + 40, y, 16, 12);
}



void DrawMoveInfoUIMarkers(void) {
    blit_move_info_icon(4, 19, 0, 0);
    blit_move_info_icon(4, 20, 0, 12);
    blit_move_info_icon(4, 21, 0, 24);
    blit_move_info_icon(4, 22, 0, 36);
    CopyWindowToVram(4, COPYWIN_GFX);
    DrawPartyMonIcons();
}

void TMCase_MoveCursor_UpdatePrintedDescription(s32 itemIndex)
{
    const u8 * str;
    u16 itemId = BagGetItemIdByPocketPosition(POCKET_TM_HM, itemIndex);
    if (itemIndex != -2)
    {
        str = ItemId_GetDescription(itemId);
    }
    else
    {
        str = gText_TMCaseWillBePutAway;
    }
    FillWindowPixelBuffer(1, 0);
    AddTextPrinterParameterized_ColorByIndex(1, 2, str, 6, 3, 1, 0, 0, 0);
    // update icons
    TintPartyMonIcons(itemId);
}


static void FreeAndDestroyMonIconSprite_(struct Sprite *sprite)
{
    struct SpriteFrameImage image = { NULL, sSpriteImageSizes[sprite->oam.shape][sprite->oam.size] };
    sprite->images = &image;
    DestroySprite(sprite);
}

void LoadMonIconPalettesTinted(void)
{
    u8 i;
    for (i = 0; i < ARRAY_COUNT(gMonIconPaletteTable); i++)
    {
        LoadSpritePalette(&gMonIconPaletteTable[i]);
        TintPalette_GrayScale2(&gPlttBufferUnfaded[0x170 + i*16], 16);
    }
}

#define MON_ICON_START_X  0x10
#define MON_ICON_START_Y  0x2a
#define MON_ICON_PADDING  0x20

static void DrawPartyMonIcons(void)
{
    u8 i;
    u16 species;
    u8 icon_x = 0;
    u8 icon_y = 0;

    LoadMonIconPalettesTinted();

    for (i = 0; i < gPlayerPartyCount; i++)
    {
        //calc icon position (centered)
        if (gPlayerPartyCount == 1)
        {
            icon_x = MON_ICON_START_X + MON_ICON_PADDING;
            icon_y = MON_ICON_START_Y + MON_ICON_PADDING*0.5;
        }
        else if (gPlayerPartyCount == 2)
        {
            icon_x = i < 2 ? 0x35 * i : 0x35 * (i - 2);
            icon_y = MON_ICON_START_Y + MON_ICON_PADDING*0.5;
        }
        else if (gPlayerPartyCount == 3)
        {
            icon_x = i < 3 ? 0x25 * i : 0x25 * (i - 3);
            icon_y = MON_ICON_START_Y + MON_ICON_PADDING*0.5;
        }
        /*else if (gPlayerPartyCount == 4)
        {
            icon_x = i < 2 ? MON_ICON_START_X + MON_ICON_PADDING*0.5 + MON_ICON_PADDING * i : MON_ICON_START_X + MON_ICON_PADDING*0.5 + MON_ICON_PADDING * (i - 2);
            icon_y = i < 2 ? MON_ICON_START_Y : MON_ICON_START_Y + MON_ICON_PADDING;
        }
        else
        {
            icon_x = i < 3 ? MON_ICON_START_X + MON_ICON_PADDING * i : MON_ICON_START_X + MON_ICON_PADDING * (i - 3);
            icon_y = i < 3 ? MON_ICON_START_Y : MON_ICON_START_Y + MON_ICON_PADDING;
        }*/
        //get species
        /*species = GetMonData(&gPlayerParty[i], MON_DATA_SPECIES, 0);

        //create icon sprite
        spriteIdData[i] = CreateMonIcon(species, SpriteCB_MonIcon, icon_x + 15, icon_y, 1, GetMonData(&gPlayerParty[0], MON_DATA_PERSONALITY, 0), TRUE);

        //Set priority, stop movement and save original palette position
        gSprites[spriteIdData[i]].oam.priority = 0;
        StartSpriteAnim(&gSprites[spriteIdData[i]], 4); //full stop
        spriteIdPalette[i] = gSprites[spriteIdData[i]].oam.paletteNum; //save correct palette number to array
    }
}

static void TintPartyMonIcons(u8 tm)
{
    u8 i;
    //u16 species;

    for (i = 0; i < gPlayerPartyCount; i++)
    {
        //species = GetMonData(&gPlayerParty[i], MON_DATA_SPECIES, 0);
        if (!CanMonLearnTMHM(&gPlayerParty[i], tm))
        {
            gSprites[spriteIdData[i]].oam.paletteNum = 1 + spriteIdPalette[i];
        }
        else
        {
            gSprites[spriteIdData[i]].oam.paletteNum = spriteIdPalette[i];//gMonIconPaletteIndices[species];
        }
    }
    
}

static void DestroyPartyMonIcons(void)
{
    u8 i;
    for (i = 0; i < gPlayerPartyCount; i++)
    {
        FreeAndDestroyMonIconSprite(&gSprites[spriteIdData[i]]);
        FreeMonIconPalettes();
    }
}
*/


/*void CreateTMSprite(u16 itemId) {
    return;
}

extern const u8 gText_TMCaseWillBePutAway[];
void DrawMoveInfoUIMarkers(void) {
    blit_move_info_icon(4, 19, 0, 0);
    blit_move_info_icon(4, 20, 0, 12);
    blit_move_info_icon(4, 21, 0, 24);
    blit_move_info_icon(4, 22, 0, 36);
    CopyWindowToVram(4, COPYWIN_GFX);
    DrawPartyMonIcons();
}

void TMCase_MoveCursor_UpdatePrintedDescription(s32 itemIndex)
{
    const u8 * str;
    u16 itemId = BagGetItemIdByPocketPosition(POCKET_TM_CASE, itemIndex);
    if (itemIndex != -2)
    {
        str = ItemId_GetDescription(itemId);
    }
    else
    {
        str = gText_TMCaseWillBePutAway;
    }
    FillWindowPixelBuffer(1, 0);
    AddTextPrinterParameterized_ColorByIndex(1, 2, str, 2, 3, 1, 0, 0, 0);

    // update icons
    TintPartyMonIcons(itemId - ITEM_TM01_FOCUS_PUNCH);
}


static const struct WindowTemplate sWindowTemplates[] = {
    {0x00, 0x0e, 0x01, 0x0f, 0x0a, 0x0f, 0x0081},
    {0x00, 0x0c, 0x0c, 0x12, 0x08, 0x0a, 0x013f},
    {0x01, 0x05, 0x0f, 0x0f, 0x04, 0x0f, 0x01f9},
    {0x00, 0x00, 0x01, 0x0a, 0x02, 0x0f, 0x0235},
    {0x00, 0x01, 0x0d, 0x05, 0x06, 0x0c, 0x0249},
    {0x00, 0x07, 0x0d, 0x05, 0x06, 0x0c, 0x0267},
    {0x01, 0x02, 0x0f, 0x1a, 0x04, 0x0b, 0x0285},
    {0x01, 0x11, 0x09, 0x0c, 0x04, 0x0f, 0x02ed},
    {0x01, 0x01, 0x01, 0x08, 0x03, 0x0d, 0x031d},
    DUMMY_WIN_TEMPLATE
};

extern const u16 gTMCaseMainWindowPalette[];
extern ALIGNED(4) const u16 sPal3Override[];


void InitWindowTemplatesAndPals(void)
{
    u8 i;

    InitWindows(sWindowTemplates);
    DeactivateAllTextPrinters();
    TextWindow_SetUserSelectedFrame(0, 0x5B, 0xE0);
    TextWindow_LoadResourcesStdFrame0(0, 0x64, 0xB0);
    TextWindow_SetStdFrame0_WithPal(0, 0x78, 0xD0);
    LoadPalette(gTMCaseMainWindowPalette, 0xF0, 0x20);
    LoadPalette(gTMCaseMainWindowPalette, 0xA0, 0x20);
    LoadPalette(sPal3Override, 0xF6, 0x04);
    LoadPalette(sPal3Override, 0xD6, 0x04);
    ListMenuLoadStdPalAt(0xc0, 0x01);
    for (i = 0; i < 9; i++)
        FillWindowPixelBuffer(i, 0x00);
    PutWindowTilemap(0);
    PutWindowTilemap(1);
    PutWindowTilemap(3);
    PutWindowTilemap(4);
    PutWindowTilemap(5);
    ScheduleBgCopyTilemapToVram(0);
}


static void FreeAndDestroyMonIconSprite_(struct Sprite *sprite)
{
    struct SpriteFrameImage image = { NULL, sSpriteImageSizes[sprite->oam.shape][sprite->oam.size] };
    sprite->images = &image;
    DestroySprite(sprite);
}

void LoadMonIconPalettesTinted(void)
{
    u8 i;
    for (i = 0; i < ARRAY_COUNT(gMonIconPaletteTable); i++)
    {
        LoadSpritePalette(&gMonIconPaletteTable[i]);
        TintPalette_GrayScale2(&gPlttBufferUnfaded[0x170 + i*16], 16);
    }
}

#define MON_ICON_START_X  0x10
#define MON_ICON_START_Y  0x2a
#define MON_ICON_PADDING  0x20

static void DrawPartyMonIcons(void)
{
    u8 i;
    u16 species;
    u8 icon_x = 0;
    u8 icon_y = 0;

    LoadMonIconPalettesTinted();

    for (i = 0; i < gPlayerPartyCount; i++)
    {
        //calc icon position (centered)
        if (gPlayerPartyCount == 1)
        {
            icon_x = MON_ICON_START_X + MON_ICON_PADDING;
            icon_y = MON_ICON_START_Y + MON_ICON_PADDING*0.5;
        }
        else if (gPlayerPartyCount == 2)
        {
            icon_x = i < 2 ? 0x35 * i : 0x35 * (i - 2);
            icon_y = MON_ICON_START_Y + MON_ICON_PADDING*0.5;
        }
        else if (gPlayerPartyCount == 3)
        {
            icon_x = i < 3 ? 0x25 * i : 0x25 * (i - 3);
            icon_y = MON_ICON_START_Y + MON_ICON_PADDING*0.5;
        }
        /*else if (gPlayerPartyCount == 4)
        {
            icon_x = i < 2 ? MON_ICON_START_X + MON_ICON_PADDING*0.5 + MON_ICON_PADDING * i : MON_ICON_START_X + MON_ICON_PADDING*0.5 + MON_ICON_PADDING * (i - 2);
            icon_y = i < 2 ? MON_ICON_START_Y : MON_ICON_START_Y + MON_ICON_PADDING;
        }
        else
        {
            icon_x = i < 3 ? MON_ICON_START_X + MON_ICON_PADDING * i : MON_ICON_START_X + MON_ICON_PADDING * (i - 3);
            icon_y = i < 3 ? MON_ICON_START_Y : MON_ICON_START_Y + MON_ICON_PADDING;
        }*/
        //get species
        /*species = GetMonData(&gPlayerParty[i], MON_DATA_SPECIES, 0);

        //create icon sprite
        spriteIdData[i] = CreateMonIcon(species, SpriteCB_MonIcon, icon_x + 15, icon_y, 1, GetMonData(&gPlayerParty[0], MON_DATA_PERSONALITY, 0), TRUE);

        //Set priority, stop movement and save original palette position
        gSprites[spriteIdData[i]].oam.priority = 0;
        StartSpriteAnim(&gSprites[spriteIdData[i]], 4); //full stop
        spriteIdPalette[i] = gSprites[spriteIdData[i]].oam.paletteNum; //save correct palette number to array
    }
}

static void TintPartyMonIcons(u8 tm)
{
    u8 i;
    u16 species;
    u8 j;

    for (i = 0; i < gPlayerPartyCount; i++)
    {
        species = GetMonData(&gPlayerParty[i], MON_DATA_SPECIES, 0);
        //SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT2_ALL);
        //SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(7, 11));
        //if (!CanSpeciesLearnTMHM(species, tm)) 
        //{
            gSprites[spriteIdData[i]].oam.objMode = ST_OAM_OBJ_BLEND;
            gSprites[spriteIdData[i]].oam.paletteNum = 7 + spriteIdPalette[i];
        //}
        //else
        //{
            gSprites[spriteIdData[i]].oam.objMode = ST_OAM_OBJ_NORMAL;//gMonIconPaletteIndices[species];
            gSprites[spriteIdData[i]].oam.paletteNum = spriteIdPalette[i];
        //}
    }
    
}

static void DestroyPartyMonIcons(void)
{
    u8 i;
    for (i = 0; i < gPlayerPartyCount; i++)
    {
        FreeAndDestroyMonIconSprite(&gSprites[spriteIdData[i]]);
        FreeMonIconPalettes();
    }
}*/