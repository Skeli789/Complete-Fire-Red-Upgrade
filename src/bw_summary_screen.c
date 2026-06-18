#include "defines.h"
#include "../include/battle.h"
#include "../include/bg.h"
#include "../include/decompress.h"
#include "../include/dynamic_placeholder_text_util.h"
#include "../include/gpu_regs.h"
#include "../include/link.h"
#include "../include/main.h"
#include "../include/malloc.h"
#include "../include/menu.h"
#include "../include/menu_helpers.h"
#include "../include/palette.h"
#include "../include/pokemon.h"
#include "../include/pokemon_icon.h"
#include "../include/pokemon_summary_screen.h"
#include "../include/sound.h"
#include "../include/sprite.h"
#include "../include/start_menu.h"
#include "../include/string_util.h"
#include "../include/task.h"
#include "../include/text.h"
#include "../include/trade.h"
#include "../include/window.h"

#include "../include/constants/pokemon.h"
#include "../include/constants/songs.h"

#include "../include/gba/macro.h"
#include "../include/gba/types.h"

#include "../include/new/bw_summary_screen.h"
#include "../include/new/Vanilla_functions.h"
#include "../include/new/ram_locs.h"
#include "../include/new/rom_locs.h"

#ifdef BW_SUMMARY_SCREEN

static void PokeSum_CreateMonPicSprite(void);
static void PokeSum_CreateWindows(void);
static u8 PSS_LoadTilesAndMap(void);
static void PokeSum_PrintMonTypeIcons(void);
static void HideShowShinyStar(bool8 invisible);
static void PokeSum_DrawPageProgressTiles(void);
static void CB2_SetUpPSS(void);
static void PokeSum_CopyNewBgTilemapBeforePageFlip_2(void);
static void PokeSum_PrintPageName(const u8 * str);
static void PokeSum_PrintControlsString(const u8 * str);
static void PokeSum_InitBgCoordsBeforePageFlips(void);
static void PokeSum_HideSpritesBeforePageFlip(void);
static u8 PokeSum_IsPageFlipFinished(u8 a0);
extern const struct MoveMenuInfoIcon gMoveMenuInfoIcons[];
static void BlitMoveInfoIcon(u8 windowId, u8 iconId, u16 x, u16 y);
void PokeSum_CopyNewBgTilemapBeforePageFlip(void);
static void PokeSum_ShowSpritesBeforePageFlip(void);
static void PrintMonLevelNickOnWindow2(const u8 * str);
static void PSS_PrintMoveNamesAndPP(u8 i);
static void Task_PokeSum_SwitchDisplayedPokemon(u8 taskId);
static void Task_PokeSum_FlipPages(u8 taskId);
static void Task_FlipPages_FromInfo(u8 taskId);
static void Task_InputHandler_Info(u8 taskId);
static void Task_HandleInput_SelectMove(u8 taskId);
static void PokeSum_CreateSprites(void);
static void PokeSum_CreateMonIconSprite(void);
static void PrintInfoPage(void);
static void PokeSum_PrintRightPaneText(void);
static void PrintSkillsPage(void);
static void PrintMovesPage(void);
static void PokeSum_FinishSetup(void);
static void Task_InputHandler_SelectOrForgetMove(u8 taskId);
static void PokeSum_PrintBottomPaneText(void);
static void PokeSum_PrintTrainerMemo(void);
static void PokeSum_PrintTrainerMemo_Mon(void);
static void PokeSum_PrintTrainerMemo_Egg(void);
static u8 PokeSum_HandleCreateSprites(void);
static void PokeSum_PrintPageHeaderText(u8 curPageIndex);
static void PokeSum_PrintAbilityDataOrMoveTypes(void);
static void PokeSum_SeekToNextMon(u8 taskId, s8 direction);
static void CreateHpBarObjs(u16 tileTag, u16 palTag);
static void CreateShinyStarObj(u16 tileTag, u16 palTag);
static void PokeSum_Setup_SetVBlankCallback(void);
static bool8 HasOnlyOneMon(void);

extern const u8 *const sEggHatchTimeTexts[];
extern const struct OamData sMoveSelectionCursorOamData;
extern const union AnimCmd * const sMoveSelectionCursorOamAnimTable[];
extern struct SpritePalette gMonIconPaletteTable[];
extern const u8 *const sEggOriginTexts[];
extern const u16 * const sHpBarPals[];
extern const struct OamData sHpOrExpBarOamData;
extern const union AnimCmd * const sHpOrExpBarAnimTable[];
extern const struct OamData sStarObjOamData;
extern const union AnimCmd * const sStarObjAnimTable[];
extern const u8 *const gMoveDescriptions[];
extern const struct WindowTemplate sWindowTemplates_Dummy[];
extern const s8 sNatureStatTable[][5];


static const u8 sPSSTextColours[][3] =
{
    [DARK] = {0, 14, 13},		//Text Dark
    [WHITE_TITLE] = {0, 1, 2}, 	//Text White (title) 
    [BLUE_SHADOW] = {0, 15, 7}, //Red Shadow
    [WHITE] = {0, 15, 14},		//Text White
    [RED_SHADOW] = {0, 15, 1}, 	//Blue Shadow
    [BLUE] = {0, 7, 6}, 		//Male Symbol
    [RED] = {0, 1, 2}, 			//Female Symbol (red)
    [ORANGE] = {0, 3, 14}, 		//Low PP
    [RED_2] = {0, 1, 14}, 		//Whithout PP
};

static const s8 sPSSNatureStatTable[][5] =
{
    // Atk Def Spd Sp.Atk Sp.Def
    {    0,  0,  0,     0,     0}, // Hardy
    {   +1, -1,  0,     0,     0}, // Lonely
    {   +1,  0, -1,     0,     0}, // Brave
    {   +1,  0,  0,    -1,     0}, // Adamant
    {   +1,  0,  0,     0,    -1}, // Naughty
    {   -1, +1,  0,     0,     0}, // Bold
    {    0,  0,  0,     0,     0}, // Docile
    {    0, +1, -1,     0,     0}, // Relaxed
    {    0, +1,  0,    -1,     0}, // Impish
    {    0, +1,  0,     0,    -1}, // Lax
    {   -1,  0, +1,     0,     0}, // Timid
    {    0, -1, +1,     0,     0}, // Hasty
    {    0,  0,  0,     0,     0}, // Serious
    {    0,  0, +1,    -1,     0}, // Jolly
    {    0,  0, +1,     0,    -1}, // Naive
    {   -1,  0,  0,    +1,     0}, // Modest
    {    0, -1,  0,    +1,     0}, // Mild
    {    0,  0, -1,    +1,     0}, // Quiet
    {    0,  0,  0,     0,     0}, // Bashful
    {    0,  0,  0,    +1,    -1}, // Rash
    {   -1,  0,  0,     0,    +1}, // Calm
    {    0, -1,  0,     0,    +1}, // Gentle
    {    0,  0, -1,     0,    +1}, // Sassy
    {    0,  0,  0,    -1,    +1}, // Careful
    {    0,  0,  0,     0,     0}, // Quirky
};

//Ram
extern u8 sMoveSelectionCursorPos;
extern u8 sMoveSwapCursorPos;
extern struct ShinyStarObjData * sShinyStarObjData;
extern struct MonPicBounceState * sMonPicBounceState;
extern struct MoveSelectionCursor * sMoveSelectionCursorObjs[4];
extern struct HpBarObjs * sHpBarObjs;

//Graphics
extern const u16 gPalSummaryScreen[];
extern const u32 gTilesSummaryScreen[];
extern const u32 gMapSummaryScreenBg[];
extern const u32 gMapSummaryScreenPokemonInfo[];
extern const u32 gMapSummaryScreenEgg[];
extern const u32 gMapSummaryScreenPokemonSkills[];
extern const u32 gMapSummaryScreenKnownMoves[];
extern const u32 gMapSummaryScreenMovesInfo[];
extern const u32 gMapSummaryScreenMoves[];
extern const u8 gFireRedMenuElements_Gfx[];
extern const u16 sMoveSelectionCursorPals[];
extern const u8 PSSIconsTiles[];
extern const u32 gSummaryScreen_HpBar_Gfx[];
extern const u32 sStarObjTiles[];
extern const u16 sStarObjPal[];
extern const u32 gSelectCursorGfxLeft[];
extern const u32 gSelectCursorGfxRight[];

//Strings
extern const u8 gText_PSS_DexNumber[];
extern const u8 gText_PSS_Name[];
extern const u8 gText_PSS_Status[];
extern const u8 gText_PSS_Type[];
extern const u8 gText_PSS_OT[];
extern const u8 gText_PSS_IDNumber[];
extern const u8 gText_PSS_Item[];
extern const u8 gText_PSS_TrainerMemo[];
extern const u8 gText_PSS_HP[];
extern const u8 gText_PSS_Attack[];
extern const u8 gText_PSS_Defense[];
extern const u8 gText_PSS_SpAtk[];
extern const u8 gText_PSS_SpDef[];
extern const u8 gText_PSS_Speed[];
extern const u8 gText_PSS_ExpPoints[];
extern const u8 gText_PSS_ToNextLv[];
extern const u8 gText_PSS_Ability[];
extern const u8 gText_PSS_Category[];
extern const u8 gText_PSS_Power[];
extern const u8 gText_PSS_Accuracy[];
extern const u8 gText_PSS_Effect[];
extern const u8 gText_PokeSum_NoData[];
extern const u8 gText_Slash[];

static const struct BgTemplate SummayScreenBgTemplate[] = 
{
	{ //Text
		.bg = 0,
		.charBaseIndex = 1,
		.mapBaseIndex = 29,
		.screenSize = 0,
		.paletteMode = 0,
		.priority = 0,
		.baseTile = 0	   
	},
	{ //Page Showed
	 	.bg = 1,
	 	.charBaseIndex = 0,
	 	.mapBaseIndex = 30,
	 	.screenSize = 0,
	 	.paletteMode = 0,
	 	.priority = 2,
	 	.baseTile = 0
	 },
	 { // BG
	 	.bg = 2,
	 	.charBaseIndex = 0,
	 	.mapBaseIndex = 31,
	 	.screenSize = 0,
	 	.paletteMode = 0,
	 	.priority = 3,
	 	.baseTile = 0
	 },
	 { //Moves
	 	.bg = 3,
	 	.charBaseIndex = 0,
	 	.mapBaseIndex = 28,
	 	.screenSize = 0,
	 	.paletteMode = 0,
	 	.priority = 1,
	 	.baseTile = 0
	 }
};

static const struct WindowTemplate WindowTemplatePokemonInfo[] =
{
    { //PAGE TITLE
        .bg = 0,
        .tilemapLeft = 0,
        .tilemapTop = 0,
        .width = 13,
        .height = 2,
        .paletteNum = 7,
        .baseBlock = 0x0258
    },
    { //INSTRUCTION
        .bg = 0,
        .tilemapLeft = 19,
        .tilemapTop = 0,
        .width = 11,
        .height = 2,
        .paletteNum = 7,
        .baseBlock = 0x0272
    },
    { // NICKNAME & LV OR TYPES
        .bg = 0,
        .tilemapLeft = 21,
        .tilemapTop = 2,
        .width = 10,
        .height = 4,
        .paletteNum = 6,
        .baseBlock = 0x0288
    }
};

static const struct WindowTemplate sDataMonAndNatureWindowTemplate[] = 
{
    { //Data Pokemon
        .bg = 0,
        .tilemapLeft = 0,
        .tilemapTop = 2,
        .width = 21,
        .height = 10,
        .paletteNum = 6,
        .baseBlock = 0
    },
    { // Nature and Found place
        .bg = 0,
        .tilemapLeft = 0,
        .tilemapTop = 12,
        .width = 28,
        .height = 8,
        .paletteNum = 6,
        .baseBlock = 220
    },
    { //DUMMY_WIN_TEMPLATE
        .bg = 0,
        .tilemapLeft = 0,
        .tilemapTop = 0,
        .width = 0,
        .height = 0,
        .paletteNum = 0,
        .baseBlock = 0
    },
    { //DUMMY_WIN_TEMPLATE
        .bg = 0,
        .tilemapLeft = 0,
        .tilemapTop = 0,
        .width = 0,
        .height = 0,
        .paletteNum = 0,
        .baseBlock = 0
    },
};

static const struct WindowTemplate sMonStatsAndAbilityWindowTemplate[] = 
{
    { // Pokemon Stats
        .bg = 0,
        .tilemapLeft = 0,
        .tilemapTop = 2,
        .width = 17,
        .height = 13,
        .paletteNum = 6,
        .baseBlock = 1
    },
    { //Exp Point & Next Lv
        .bg = 0,
        .tilemapLeft = 0,
        .tilemapTop = 0,
        .width = 0,
        .height = 0,
        .paletteNum = 0,
        .baseBlock = 0
    },
    { // Ability & Description
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 15,
        .width = 29,
        .height = 5,
        .paletteNum = 6,
        .baseBlock = 292
    },
    { // DUMMY_WIN_TEMPLATE
        .bg = 0,
        .tilemapLeft = 0,
        .tilemapTop = 0,
        .width = 0,
        .height = 0,
        .paletteNum = 0,
        .baseBlock = 0
    },
};

static const struct WindowTemplate sMovesInfoWindowTemplate[] = 
{
    { //Move Names
        .bg = 0,
        .tilemapLeft = 5,
        .tilemapTop = 2,
        .width = 10,
        .height = 18,
        .paletteNum = 6,
        .baseBlock = 1
    },
    { //Move Data & Description
        .bg = 0,
        .tilemapLeft = 15,
        .tilemapTop = 6,
        .width = 15,
        .height = 14,
        .paletteNum = 6,
        .baseBlock = 181
    },
    { //Move Icons
        .bg = 0,
        .tilemapLeft = 0,
        .tilemapTop = 2,
        .width = 5,
        .height = 18,
        .paletteNum = 6,
        .baseBlock = 452
    },
    { //DUMMY_WIN_TEMPLATE
        .bg = 0,
        .tilemapLeft = 0,
        .tilemapTop = 0,
        .width = 0,
        .height = 0,
        .paletteNum = 0,
        .baseBlock = 0
    },
};

struct Struct203B144
{
    u16 unk00;
    u16 curHpStr;
    u16 atkStr;
    u16 defStr;
    u16 spAStr;
    u16 spDStr;
    u16 speStr;
    u16 expStr;
    u16 toNextLevel;

    u16 curPp[5];
    u16 maxPp[5];

    u16 unk26;
};

extern const u16 sIVRankings_E_MinusPal[];
extern const u8 sIVRankings_E_MinusTiles[];
extern const u8 sIVRankings_ETiles[];
extern const u8 sIVRankings_E_PlusTiles[];
extern const u8 sIVRankings_D_MinusTiles[];
extern const u8 sIVRankings_DTiles[];
extern const u8 sIVRankings_D_PlusTiles[];
extern const u8 sIVRankings_C_MinusTiles[];
extern const u8 sIVRankings_CTiles[];
extern const u8 sIVRankings_C_PlusTiles[];
extern const u8 sIVRankings_B_MinusTiles[];
extern const u8 sIVRankings_BTiles[];
extern const u8 sIVRankings_B_PlusTiles[];
extern const u8 sIVRankings_A_MinusTiles[];
extern const u8 sIVRankings_ATiles[];
extern const u8 sIVRankings_A_PlusTiles[];
extern const u8 sIVRankings_STiles[];
#define sIVRankingsPal sIVRankings_E_MinusPal

static const u8 * const sIVRankings_Images[] = 
{
    [0 ... 2] = sIVRankings_E_MinusTiles,
    [3 ... 4] = sIVRankings_ETiles,
    [5 ... 6] = sIVRankings_E_PlusTiles,
    [7 ... 8] = sIVRankings_D_MinusTiles,
    [9 ... 10] = sIVRankings_DTiles,
    [11 ... 12] = sIVRankings_D_PlusTiles,
    [13 ... 14] = sIVRankings_C_MinusTiles,
    [15 ... 16] = sIVRankings_CTiles,
    [17 ... 18] = sIVRankings_C_PlusTiles,
    [19 ... 20] = sIVRankings_B_MinusTiles,
    [21 ... 22] = sIVRankings_BTiles,
    [23 ... 24] = sIVRankings_B_PlusTiles,
    [25 ... 26] = sIVRankings_A_MinusTiles,
    [27 ... 28] = sIVRankings_ATiles,
    [29 ... 30] = sIVRankings_A_PlusTiles,
    [31] = sIVRankings_STiles,
};

extern struct Struct203B144 *sMonSkillsPrinterXpos;
extern const u8 sLevelNickTextColors[][3];

static void PokeSum_Setup_InitGpu(void)
{
    DmaClearLarge16(3, (void *)VRAM, VRAM_SIZE, 0x1000);
    DmaClear32(3, (void *)OAM, OAM_SIZE);
    DmaClear16(3, (void *)PLTT, PLTT_SIZE);

    SetGpuReg(REG_OFFSET_DISPCNT, 0);

    ResetBgsAndClearDma3BusyFlags(0);
    InitBgsFromTemplates(0, SummayScreenBgTemplate, NELEMS(SummayScreenBgTemplate));

    ChangeBgX(0, 0, 0);
    ChangeBgY(0, 0, 0);
    ChangeBgX(1, 0, 0);
    ChangeBgY(1, 0, 0);
    ChangeBgX(2, 0, 0);
    ChangeBgY(2, 0, 0);
    ChangeBgX(3, 0, 0);
    ChangeBgY(3, 0, 0);

    DeactivateAllTextPrinters();
//Transparency
	SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_EFFECT_BLEND | BLDCNT_TGT1_BG1 | BLDCNT_TGT2_BG2);
	SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(13, 16));
    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_1D_MAP | DISPCNT_OBJ_ON);

    ShowBg(0);
    ShowBg(1);
    ShowBg(2);
    ShowBg(3);
}

static void PokeSum_CreateWindows(void)
{
    u8 i;

    InitWindows(sWindowTemplates_Dummy);

    for (i = 0; i < 3; i++)
        sMonSummaryScreen->windowIds[i] = AddWindow(&WindowTemplatePokemonInfo[i]);

    for (i = 0; i < 4; i++)
        switch (sMonSummaryScreen->curPageIndex)
        {
        case PSS_PAGE_INFO:
            sMonSummaryScreen->windowIds[i + 3] = AddWindow(&sDataMonAndNatureWindowTemplate[i]);
            break;
        case PSS_PAGE_SKILLS:
            sMonSummaryScreen->windowIds[i + 3] = AddWindow(&sMonStatsAndAbilityWindowTemplate[i]);
            break;
        case PSS_PAGE_MOVES:
        case PSS_PAGE_MOVES_INFO:
            sMonSummaryScreen->windowIds[i + 3] = AddWindow(&sMovesInfoWindowTemplate[i]);
            break;
        default:
            break;
        }
}

static void PokeSum_AddWindows(u8 curPageIndex)
{
    u8 i;
    for (i = 0; i < 3; i++)
		sMonSummaryScreen->windowIds[i] = AddWindow(&WindowTemplatePokemonInfo[i]);
    for (i = 0; i < 4; i++)
        switch (curPageIndex)
        {
        case PSS_PAGE_INFO:
            sMonSummaryScreen->windowIds[i + 3] = AddWindow(&sDataMonAndNatureWindowTemplate[i]);
            break;
        case PSS_PAGE_SKILLS:
        default:
            sMonSummaryScreen->windowIds[i + 3] = AddWindow(&sMonStatsAndAbilityWindowTemplate[i]);
            break;
        case PSS_PAGE_MOVES:
        case PSS_PAGE_MOVES_INFO:
            sMonSummaryScreen->windowIds[i + 3] = AddWindow(&sMovesInfoWindowTemplate[i]);
            break;
        }
}

static void PrintSkillsPage(void)
{
    u8 nature = GetNature(&sMonSummaryScreen->currentMon);

	u8 hpIv, atkIv, defIv, spAtkIv, spDefIv, spdIv;
    hpIv = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_HP_IV, NULL);
    atkIv = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_ATK_IV, NULL);
    defIv = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_DEF_IV, NULL);
    spAtkIv = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_SPATK_IV, NULL);
    spDefIv = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_SPDEF_IV, NULL);
    spdIv = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_SPEED_IV, NULL);

    AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[POKESUM_WIN_RIGHT_PANE], 2, 44, 0, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_HP);
	AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[POKESUM_WIN_RIGHT_PANE], 2, 10, 79, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_ExpPoints);
	AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[POKESUM_WIN_RIGHT_PANE], 2, 10, 91, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_ToNextLv);
	AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[POKESUM_WIN_RIGHT_PANE], 2, 10, 19, sPSSTextColours[WHITE + sPSSNatureStatTable[nature][0]], TEXT_SPEED_FF, gText_PSS_Attack);
	AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[POKESUM_WIN_RIGHT_PANE], 2, 10, 31, sPSSTextColours[WHITE + sPSSNatureStatTable[nature][1]], TEXT_SPEED_FF, gText_PSS_Defense);
	AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[POKESUM_WIN_RIGHT_PANE], 2, 10, 43, sPSSTextColours[WHITE + sPSSNatureStatTable[nature][3]], TEXT_SPEED_FF, gText_PSS_SpAtk);
	AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[POKESUM_WIN_RIGHT_PANE], 2, 10, 55, sPSSTextColours[WHITE + sPSSNatureStatTable[nature][4]], TEXT_SPEED_FF, gText_PSS_SpDef);
	AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[POKESUM_WIN_RIGHT_PANE], 2, 10, 67, sPSSTextColours[WHITE + sPSSNatureStatTable[nature][2]], TEXT_SPEED_FF, gText_PSS_Speed);
	AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[POKESUM_WIN_RIGHT_PANE], 2, 72 + sMonSkillsPrinterXpos->curHpStr, 0, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.curHpStrBuf);
    AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[POKESUM_WIN_RIGHT_PANE], 2, 90 + sMonSkillsPrinterXpos->atkStr, 20, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.statValueStrBufs[PSS_STAT_ATK]);
    AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[POKESUM_WIN_RIGHT_PANE], 2, 90 + sMonSkillsPrinterXpos->defStr, 32, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.statValueStrBufs[PSS_STAT_DEF]);
    AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[POKESUM_WIN_RIGHT_PANE], 2, 90 + sMonSkillsPrinterXpos->spAStr, 44, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.statValueStrBufs[PSS_STAT_SPA]);
    AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[POKESUM_WIN_RIGHT_PANE], 2, 90 + sMonSkillsPrinterXpos->spDStr, 56, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.statValueStrBufs[PSS_STAT_SPD]);
    AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[POKESUM_WIN_RIGHT_PANE], 2, 90 + sMonSkillsPrinterXpos->speStr, 68, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.statValueStrBufs[PSS_STAT_SPE]);
    AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[POKESUM_WIN_RIGHT_PANE], 2, 55 + sMonSkillsPrinterXpos->expStr, 80, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.expPointsStrBuf);
    AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[POKESUM_WIN_RIGHT_PANE], 2, 55 + sMonSkillsPrinterXpos->toNextLevel, 92, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.expToNextLevelStrBuf);

	BlitBitmapRectToWindow(sMonSummaryScreen->windowIds[POKESUM_WIN_RIGHT_PANE], sIVRankings_Images[hpIv], 0, 0, 8, 8, STATS_IV_POS_X , HP_IV_POS_Y , 16, 8);
    BlitBitmapRectToWindow(sMonSummaryScreen->windowIds[POKESUM_WIN_RIGHT_PANE], sIVRankings_Images[atkIv], 0, 0, 8, 8, STATS_IV_POS_X , ATK_IV_POS_Y , 16, 8);
    BlitBitmapRectToWindow(sMonSummaryScreen->windowIds[POKESUM_WIN_RIGHT_PANE], sIVRankings_Images[defIv], 0, 0, 8, 8, STATS_IV_POS_X , DEF_IV_POS_Y , 16, 8);
    BlitBitmapRectToWindow(sMonSummaryScreen->windowIds[POKESUM_WIN_RIGHT_PANE], sIVRankings_Images[spAtkIv], 0, 0, 8, 8, STATS_IV_POS_X , SPATK_IV_POS_Y , 16, 8);
    BlitBitmapRectToWindow(sMonSummaryScreen->windowIds[POKESUM_WIN_RIGHT_PANE], sIVRankings_Images[spDefIv], 0, 0, 8, 8, STATS_IV_POS_X , SPDEF_IV_POS_Y , 16, 8);
    BlitBitmapRectToWindow(sMonSummaryScreen->windowIds[POKESUM_WIN_RIGHT_PANE], sIVRankings_Images[spdIv], 0, 0, 8, 8, STATS_IV_POS_X , SPD_IV_POS_Y , 16, 8);
}

void ShowPokemonSummaryScreen(struct Pokemon * party, u8 cursorPos, u8 lastIdx, MainCallback savedCallback, u8 mode)
{
    *((struct PokemonSummaryScreenData**) 0x203B140) = AllocZeroed(sizeof(struct PokemonSummaryScreenData));
    sMonSkillsPrinterXpos = AllocZeroed(sizeof(struct Struct203B144));

    if (sMonSummaryScreen == NULL)
    {
        SetMainCallback2(savedCallback);
        return;
    }

    sLastViewedMonIndex = cursorPos;

    sMoveSelectionCursorPos = 0;
    sMoveSwapCursorPos = 0;
    sMonSummaryScreen->savedCallback = savedCallback;
    sMonSummaryScreen->monList.mons = party;

    if (party == gEnemyParty)
        sMonSummaryScreen->isEnemyParty = TRUE;
    else
        sMonSummaryScreen->isEnemyParty = FALSE;

    sMonSummaryScreen->lastIndex = lastIdx;
    sMonSummaryScreen->mode = mode;

    switch (sMonSummaryScreen->mode)
    {
    case PSS_MODE_NORMAL:
    default:
        SetHelpContext(HELPCONTEXT_POKEMON_INFO);
        sMonSummaryScreen->curPageIndex = PSS_PAGE_INFO;
        sMonSummaryScreen->isBoxMon = FALSE;
        sMonSummaryScreen->lockMovesFlag = FALSE;
        break;
    case PSS_MODE_BOX:
        SetHelpContext(HELPCONTEXT_POKEMON_INFO);
        sMonSummaryScreen->curPageIndex = PSS_PAGE_INFO;
        sMonSummaryScreen->isBoxMon = TRUE;
        sMonSummaryScreen->lockMovesFlag = FALSE;
        break;
    case PSS_MODE_SELECT_MOVE:
    case PSS_MODE_FORGET_MOVE:
        SetHelpContext(HELPCONTEXT_POKEMON_MOVES);
        sMonSummaryScreen->curPageIndex = PSS_PAGE_MOVES_INFO;
        sMonSummaryScreen->isBoxMon = FALSE;
        sMonSummaryScreen->lockMovesFlag = TRUE;
        break;
    }

    sMonSummaryScreen->state3270 = 0;
    sMonSummaryScreen->summarySetupStep = 0;
    sMonSummaryScreen->loadBgGfxStep = 0;
    sMonSummaryScreen->spriteCreationStep = 0;

    sMonSummaryScreen->whichBgLayerToTranslate = 0;
    sMonSummaryScreen->skillsPageBgNum = 2;
    sMonSummaryScreen->infoAndMovesPageBgNum = 1;
    sMonSummaryScreen->flippingPages = FALSE;

    sMonSummaryScreen->unk3228 = 0;
    sMonSummaryScreen->unk322C = 1;

    BufferSelectedMonData(&sMonSummaryScreen->currentMon);
    sMonSummaryScreen->isEgg = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_IS_EGG, NULL);
    sMonSummaryScreen->isBadEgg = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_SANITY_IS_BAD_EGG, NULL);

    if (sMonSummaryScreen->isBadEgg == TRUE)
        sMonSummaryScreen->isEgg = TRUE;

    sMonSummaryScreen->lastPageFlipDirection = 0xFF;
    SetMainCallback2(CB2_SetUpPSS);
}

static void CB2_SetUpPSS(void)
{
    switch (sMonSummaryScreen->summarySetupStep)
    {
    case 0:
        PokeSum_Setup_ResetCallbacks();
        break;
    case 1:
        PokeSum_Setup_InitGpu();
        break;
    case 2:
        PokeSum_Setup_SpritesReset();
        break;
    case 3:
        if (!PSS_LoadTilesAndMap())
            return;
        break;
    case 4:
        if (!PokeSum_HandleCreateSprites())
            return;
        break;
    case 5:
        PokeSum_CreateWindows();
        break;
    case 6:
        if (!PokeSum_Setup_BufferStrings())
            return;
        break;
    case 7:
        PokeSum_PrintRightPaneText();
        break;
    case 8:
        PokeSum_PrintBottomPaneText();
        break;
    case 9:
        PokeSum_PrintAbilityDataOrMoveTypes();
        PokeSum_PrintMonTypeIcons();
        break;
    case 10:
        PokeSum_DrawPageProgressTiles();
        break;
    case 11:
        break;
    case 12:
        BlendPalettes(0xffffffff, 16, 0);
        PokeSum_PrintPageHeaderText(sMonSummaryScreen->curPageIndex);
        CommitStaticWindowTilemaps();
        break;
    case 13:
        BeginNormalPaletteFade(0xffffffff, 0, 16, 0, 0);
        CopyWindowToVram(sMonSummaryScreen->windowIds[0], 2);
        CopyWindowToVram(sMonSummaryScreen->windowIds[1], 2);
        CopyWindowToVram(sMonSummaryScreen->windowIds[2], 2);
        CopyWindowToVram(sMonSummaryScreen->windowIds[3], 2);
        CopyWindowToVram(sMonSummaryScreen->windowIds[4], 2);
        CopyWindowToVram(sMonSummaryScreen->windowIds[5], 2);
        CopyWindowToVram(sMonSummaryScreen->windowIds[6], 2);
        break;
    case 14:
        CopyBgTilemapBufferToVram(0);
        break;
    case 15:
        if (sMonSummaryScreen->mode == PSS_MODE_SELECT_MOVE || sMonSummaryScreen->mode == PSS_MODE_FORGET_MOVE)
        {
            PokeSum_ShowOrHideMonIconSprite(0);
            ShoworHideMoveSelectionCursor(0);
        }
        else
        {
            PokeSum_ShowOrHideMonPicSprite(0);
            PokeSum_ShowOrHideMonMarkingsSprite(0);
            ShowOrHideBallIconObj(0);
        }

		ShowOrHideHpBarObjs(1);
		ShowOrHideExpBarObjs(1);
        ShowOrHideStatusIcon(0);
        HideShowPokerusIcon(0);
        HideShowShinyStar(0);
        break;
    default:
        PokeSum_Setup_SetVBlankCallback();
        PokeSum_FinishSetup();
        return;
    }

    sMonSummaryScreen->summarySetupStep++;
}

static u8 PSS_LoadTilesAndMap(void)
{
    switch (sMonSummaryScreen->loadBgGfxStep)
    {
    case 0:
        LoadPalette(gPalSummaryScreen, 0, 0x20);
        break;
    case 1:
        ListMenuLoadStdPalAt(0x60, 1);
        LoadPalette(((u8*)0x84636C0), 0x70, 0x20);
        break;
    case 2:
        LZ77UnCompVram(gTilesSummaryScreen, (void *)(VRAM));
        break;
    case 3:
        LZ77UnCompVram(gMapSummaryScreenBg, (void *)(VRAM + 0xF800));
        break;
    case 4:
        LZ77UnCompVram(gMapSummaryScreenBg, (void *)(VRAM + 0xF800));
        break;
    case 5:
        LoadPalette(((u8*)0x8463700), 0x80, 0x20);
        return TRUE;
    }

    sMonSummaryScreen->loadBgGfxStep++;
    return FALSE;
}

static void PokeSum_PrintMonTypeIcons(void)
{
    switch (sMonSummaryScreen->curPageIndex)
    {
    case PSS_PAGE_INFO:
        if (!sMonSummaryScreen->isEgg)
        {
            BlitMenuInfoIcon(sMonSummaryScreen->windowIds[POKESUM_WIN_RIGHT_PANE], sMonSummaryScreen->monTypes[0] + 1, 78, 33);

            if (sMonSummaryScreen->monTypes[0] != sMonSummaryScreen->monTypes[1])
                BlitMenuInfoIcon(sMonSummaryScreen->windowIds[POKESUM_WIN_RIGHT_PANE], sMonSummaryScreen->monTypes[1] + 1, 110, 33);
        }
        break;
    case PSS_PAGE_SKILLS:
        break;
    case PSS_PAGE_MOVES:
        break;
    case PSS_PAGE_MOVES_INFO:
        break;
    }
}

static void PokeSum_DrawPageProgressTiles(void)
{
    switch (sMonSummaryScreen->curPageIndex)
    {
    case PSS_PAGE_INFO:
        if (!sMonSummaryScreen->isEgg)
        {
            LZ77UnCompVram(gMapSummaryScreenPokemonInfo, (void *)(VRAM + 0xF000));
        }
        else
        {
            LZ77UnCompVram(gMapSummaryScreenEgg, (void *)(VRAM + 0xF000));
        }
		ShowOrHideHpBarObjs(1);
		ShowOrHideExpBarObjs(1);
        break;
    case PSS_PAGE_SKILLS:
        LZ77UnCompVram(gMapSummaryScreenPokemonSkills, (void *)(VRAM + 0xF000));
		ShowOrHideHpBarObjs(0);
		ShowOrHideExpBarObjs(0);
		HideBg(3);
        break;
    case PSS_PAGE_MOVES:
        LZ77UnCompVram(gMapSummaryScreenKnownMoves, (void *)(VRAM + 0xF000));
        LZ77UnCompVram(gMapSummaryScreenMoves, (void *)(VRAM + 0xE000));
		ShowOrHideHpBarObjs(1);
		ShowOrHideExpBarObjs(1);
		ShowBg(3);
        break;
    case PSS_PAGE_MOVES_INFO:
		LZ77UnCompVram(gMapSummaryScreenMovesInfo, (void *)(VRAM + 0xF000));
        LZ77UnCompVram(gMapSummaryScreenMoves, (void *)(VRAM + 0xE000));
		ShowOrHideHpBarObjs(1);
		ShowOrHideExpBarObjs(1);
		ShowBg(3);
        break;
    }
}

static void HideShowShinyStar(bool8 invisible)
{
    if (IsMonShiny(&sMonSummaryScreen->currentMon) == TRUE
        && !sMonSummaryScreen->isEgg)
        sShinyStarObjData->sprite->invisible = invisible;
    else
        sShinyStarObjData->sprite->invisible = TRUE;

    if (sMonSummaryScreen->curPageIndex == PSS_PAGE_MOVES_INFO)
    {
        sShinyStarObjData->sprite->pos1.x = 126;
        sShinyStarObjData->sprite->pos1.y = 20;
    }
    else
    {
        sShinyStarObjData->sprite->pos1.x = 166;
        sShinyStarObjData->sprite->pos1.y = 50;
    }
}

static void Task_BackOutOfSelectMove(unusedArg u8 taskId)
{
    switch (sMonSummaryScreen->state3284)
    {
    case 0:
        sMonSummaryScreen->lockMovesFlag = TRUE;
        sMonSummaryScreen->inhibitPageFlipInput = TRUE;
        PokeSum_AddWindows(sMonSummaryScreen->curPageIndex);
        break;
    case 1:
        break;
    case 2:
        PokeSum_CopyNewBgTilemapBeforePageFlip_2();
        break;
    case 3:
        PokeSum_PrintRightPaneText();
        PokeSum_PrintBottomPaneText();
        PokeSum_PrintAbilityDataOrMoveTypes();
        CopyWindowToVram(sMonSummaryScreen->windowIds[3], 2);
        CopyWindowToVram(sMonSummaryScreen->windowIds[4], 2);
        CopyWindowToVram(sMonSummaryScreen->windowIds[5], 2);
        CopyBgTilemapBufferToVram(0);
        break;
    case 4:
		PokeSum_PrintPageName(((u8*)0x8419C39));
		PokeSum_PrintControlsString(((u8*)0x8419C82));
        break;
    case 5:
        CopyWindowToVram(sMonSummaryScreen->windowIds[0], 2);
        CopyWindowToVram(sMonSummaryScreen->windowIds[1], 2);
        break;
    case 6:
        PokeSum_InitBgCoordsBeforePageFlips();
        sMonSummaryScreen->flippingPages = TRUE;
        PokeSum_HideSpritesBeforePageFlip();
        PokeSum_PrintMonTypeIcons();
        break;
    case 7:
        break;
    case 8:
        if (PokeSum_IsPageFlipFinished(sMonSummaryScreen->pageFlipDirection) == 0)
            return;

        PrintMonLevelNickOnWindow2(gText_PokeSum_NoData);
        break;
    case 9:
        CopyWindowToVram(sMonSummaryScreen->windowIds[6], 2);
        CopyWindowToVram(sMonSummaryScreen->windowIds[2], 2);
        CopyBgTilemapBufferToVram(0);
        break;
    case 10:
        PokeSum_CopyNewBgTilemapBeforePageFlip();
        PokeSum_DrawPageProgressTiles();
        PokeSum_ShowSpritesBeforePageFlip();
        break;
    default:
        PokeSum_SetHelpContext();
        gTasks[sMonSummaryScreen->inputHandlerTaskId].func = Task_InputHandler_Info;
        sMonSummaryScreen->state3284 = 0;
        sMonSummaryScreen->lockMovesFlag = FALSE;
        sMonSummaryScreen->inhibitPageFlipInput = FALSE;
        return;
    }

    sMonSummaryScreen->state3284++;
    return;
}

static void PokeSum_CopyNewBgTilemapBeforePageFlip_2(void)
{
    u8 newPage;

    if (sMonSummaryScreen->pageFlipDirection == 1)
        newPage = sMonSummaryScreen->curPageIndex - 1;
    else
        newPage = sMonSummaryScreen->curPageIndex + 1;

    switch (newPage)
    {
    case PSS_PAGE_INFO:
        break;
    case PSS_PAGE_SKILLS:
        break;
    case PSS_PAGE_MOVES:
        break;
    case PSS_PAGE_MOVES_INFO:
        break;
    }
}

static void PrintInfoPage(void)
{
    u32 trainerId = (gSaveBlock2->playerTrainerId[0]
    | (gSaveBlock2->playerTrainerId[1] << 8)
    | (gSaveBlock2->playerTrainerId[2] << 16)
    | (gSaveBlock2->playerTrainerId[3] << 24));
    
    if (!sMonSummaryScreen->isEgg)
    {
        AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 80, 20, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.speciesNameStrBuf);
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 10, 8 - 1, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_DexNumber);
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 10, 20 - 1, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_Name);
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 10, 32 - 1, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_Type);
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 10, 44 - 1, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_OT);
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 10, 56 - 1, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_IDNumber);
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 10, 68 - 1, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_Item);
		
		if ((HIHALF(GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_OT_ID, NULL)) ^ LOHALF(GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_OT_ID, NULL)) ^ HIHALF(GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_PERSONALITY, NULL)) ^ LOHALF(GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_PERSONALITY, NULL))) < 8)
			AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 80 + sMonSkillsPrinterXpos->unk00, 8, sPSSTextColours[RED], TEXT_SPEED_FF, sMonSummaryScreen->summary.dexNumStrBuf);
		else	
			AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 80 + sMonSkillsPrinterXpos->unk00, 8, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.dexNumStrBuf);
		if (trainerId == (u32)GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_OT_ID, NULL))
		{
			if (gSaveBlock2->playerGender == FEMALE)
				AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 80, 44, sPSSTextColours[RED], TEXT_SPEED_FF, sMonSummaryScreen->summary.otNameStrBuf);
			if (gSaveBlock2->playerGender == MALE)
				AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 80, 44, sPSSTextColours[BLUE], TEXT_SPEED_FF, sMonSummaryScreen->summary.otNameStrBuf);
		}
		else
			AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 80, 44, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.otNameStrBuf);
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 80, 56, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.unk306C);
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 80, 68, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.itemNameStrBuf);
    }
    else
    {
        u8 eggCycles;
        u8 hatchMsgIndex;

        eggCycles = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_FRIENDSHIP, NULL);

        if (eggCycles <= 5)
            hatchMsgIndex = 3;
        else if (eggCycles <= 10)
            hatchMsgIndex = 2;
        else if (eggCycles <= 40)
            hatchMsgIndex = 1;
        else
            hatchMsgIndex = 0;

        if (sMonSummaryScreen->isBadEgg)
            hatchMsgIndex = 0;

		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 10, 7, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_Name);
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 10, 31, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_Status);
		AddTextPrinterParameterized4(sMonSummaryScreen->windowIds[3], 2, 80, 32, 0, -2, sPSSTextColours[DARK], TEXT_SPEED_FF, sEggHatchTimeTexts[hatchMsgIndex]);
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 80, 8, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.speciesNameStrBuf);
    }
}

static void PrintMovesPage(void)
{
    u8 i;

    for (i = 0; i < 4; i++)
        PSS_PrintMoveNamesAndPP(i);

    if (sMonSummaryScreen->curPageIndex == PSS_PAGE_MOVES_INFO)
    {
        if (sMonSummaryScreen->mode == PSS_MODE_SELECT_MOVE)
            PSS_PrintMoveNamesAndPP(4);
        else
			AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 3, MACRO_8137270(4), sPSSTextColours[WHITE], TEXT_SPEED_FF, ((u8*)0x84161C1));
    }
}

static void PSS_PrintMoveNamesAndPP(u8 i)
{
    u8 color = WHITE;
    u8 curPP = GetMonPpByMoveSlot(&sMonSummaryScreen->currentMon, i);
    u16 move = sMonSummaryScreen->moveIds[i];
    u8 ppBonuses = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_PP_BONUSES, NULL);
    u8 maxPP = CalculatePPWithBonus(move, ppBonuses, i);

    if (i == 4)
        curPP = maxPP;
	//Add Move Names
    AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 3, MACRO_8137270(i) - 2, sPSSTextColours[WHITE], TEXT_SPEED_FF, sMonSummaryScreen->summary.moveNameStrBufs[i]);

    if (sMonSummaryScreen->moveIds[i] == 0 || (curPP == maxPP))
        color = WHITE;
    else if (curPP == 0)
        color = RED_2;
    else if (maxPP == 3)
    {
        if (curPP == 2)
            color = ORANGE;
        else if (curPP == 1)
            color = ORANGE;
    }
    else if (maxPP == 2)
    {
        if (curPP == 1)
            color = ORANGE;
    }
    else
    {
        if (curPP <= (maxPP / 4))
            color = ORANGE;
        else if (curPP <= (maxPP / 2))
            color = ORANGE;
    }
	//Add PP text
    AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 20, MACRO_81372E4(i), sPSSTextColours[color], TEXT_SPEED_FF, ((u8*)0x8416238));
	// Add PP counter
    AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 40 + sMonSkillsPrinterXpos->maxPp[i], MACRO_81372E4(i), sPSSTextColours[color], TEXT_SPEED_FF, sMonSummaryScreen->summary.moveCurPpStrBufs[i]);

    if (sMonSummaryScreen->moveIds[i] != MOVE_NONE)
    {
		// Add Slash
        AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 52, MACRO_81372E4(i), sPSSTextColours[color], TEXT_SPEED_FF, gText_Slash);
        // Add PP Max
		AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[3], 2, 58 + sMonSkillsPrinterXpos->maxPp[i], MACRO_81372E4(i), sPSSTextColours[color], TEXT_SPEED_FF, sMonSummaryScreen->summary.moveMaxPpStrBufs[i]);
    }
}

static void PokeSum_PrintExpPoints_NextLv(void)
{
	AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[4], 2, 26,  7, sPSSTextColours[DARK], TEXT_SPEED_FF, ((u8*)0x8419C4D));
	AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[4], 2, 26, 20, sPSSTextColours[DARK], TEXT_SPEED_FF, ((u8*)0x8419C59));
}

static void PokeSum_PrintSelectedMoveStats(void)
{
    if (sMoveSelectionCursorPos < 5)
    {
        if (sMonSummaryScreen->mode != PSS_MODE_SELECT_MOVE && sMoveSelectionCursorPos == 4)
            return;
		//Add Category Icon
        BlitBitmapToWindow(sMonSummaryScreen->windowIds[4], PSSIconsTiles + 24 * 8 * gBattleMoves[sMonSummaryScreen->moveIds[sMoveSelectionCursorPos]].split, 88, 2, 24, 15);
		//Add Power
        AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[4], 2, 91, 18, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.movePowerStrBufs[sMoveSelectionCursorPos]);
		//Add Accuracy
        AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[4], 2, 91, 33, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.moveAccuracyStrBufs[sMoveSelectionCursorPos]);
		//Add Move Description
		AddTextPrinterParameterized4(sMonSummaryScreen->windowIds[4], 2, 14,  2, 0, -2, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_Category);
		AddTextPrinterParameterized4(sMonSummaryScreen->windowIds[4], 2, 14, 17, 0, -2, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_Power);
		AddTextPrinterParameterized4(sMonSummaryScreen->windowIds[4], 2, 14, 32, 0, -2, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_Accuracy);
		AddTextPrinterParameterized4(sMonSummaryScreen->windowIds[4], 2, 10, 48, 0, -2, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_Effect);
		AddTextPrinterParameterized4(sMonSummaryScreen->windowIds[4], 2,  7, 63, 0, -2, sPSSTextColours[DARK], TEXT_SPEED_FF, gMoveDescriptions[sMonSummaryScreen->moveIds[sMoveSelectionCursorPos] - 1]);

        PutWindowTilemap(sMonSummaryScreen->windowIds[4]);
    }
}

static void PokeSum_PrintAbilityNameAndDesc(void)
{
    FillWindowPixelBuffer(sMonSummaryScreen->windowIds[5], 0);
	AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[5], 2, 11,  4, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_Ability);
    AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[5], 2, 60,  4, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.abilityNameStrBuf);
    AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[5], 2, 20, 23, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.abilityDescStrBuf);
}

static void PokeSum_DrawMoveTypeIcons(void)
{
    u8 i;

    FillWindowPixelBuffer(sMonSummaryScreen->windowIds[5], 0);

    for (i = 0; i < 4; i++)
    {
        if (sMonSummaryScreen->moveIds[i] == MOVE_NONE)
            continue;

        BlitMenuInfoIcon(sMonSummaryScreen->windowIds[5], sMonSummaryScreen->moveTypes[i] + 1, 8, MACRO_8137270(i) - 2);
    }

    if (sMonSummaryScreen->mode == PSS_MODE_SELECT_MOVE)
        BlitMenuInfoIcon(sMonSummaryScreen->windowIds[5], sMonSummaryScreen->moveTypes[4] + 1, 8, MACRO_8137270(4) - 2);
}

static void PokeSum_PrintPageName(const u8 * str)
{
    FillWindowPixelBuffer(sMonSummaryScreen->windowIds[0], 0);
    AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[0], 2, 4, 1, sPSSTextColours[WHITE_TITLE], 0, str);
    PutWindowTilemap(sMonSummaryScreen->windowIds[0]);
}

static void PokeSum_PrintControlsString(const u8 * str)
{
    s32 width;
    u8 r1;

    FillWindowPixelBuffer(sMonSummaryScreen->windowIds[1], 0);
    width = GetStringWidth(0, str, 0);
    r1 = sMonSummaryScreen->windowIds[1];
    AddTextPrinterParameterized3(r1, 0, 0x54 - width, 0, sPSSTextColours[WHITE_TITLE], 0, str);
    PutWindowTilemap(sMonSummaryScreen->windowIds[1]);
}

static void PokeSum_InitBgCoordsBeforePageFlips(void)
{
    s8 pageDelta = 1;

    if (sMonSummaryScreen->pageFlipDirection == 1)
        pageDelta = -1;

    if (sMonSummaryScreen->curPageIndex == PSS_PAGE_MOVES_INFO)
    {
        sMonSummaryScreen->flipPagesBgHofs = 240;
        return;
    }

    if ((sMonSummaryScreen->curPageIndex + pageDelta) == PSS_PAGE_MOVES_INFO)
    {
        sMonSummaryScreen->flipPagesBgHofs = 0;
        return;
    }
    if (sMonSummaryScreen->pageFlipDirection == 1)
        sMonSummaryScreen->flipPagesBgHofs = 0;
    if (sMonSummaryScreen->curPageIndex != PSS_PAGE_SKILLS)
	{
		ShowOrHideHpBarObjs(1);
		ShowOrHideExpBarObjs(1);
	}
    else
	{
		ShowOrHideHpBarObjs(0);
		ShowOrHideExpBarObjs(0);
	}
}

static void PokeSum_HideSpritesBeforePageFlip(void)
{
    u8 newPage;

    if (sMonSummaryScreen->pageFlipDirection == 1)
        newPage = sMonSummaryScreen->curPageIndex - 1;
    else
        newPage = sMonSummaryScreen->curPageIndex + 1;

    switch (newPage)
    {
    case PSS_PAGE_INFO:
        ShowOrHideHpBarObjs(1);
        ShowOrHideExpBarObjs(1);
        break;
    case PSS_PAGE_SKILLS:
        ShowOrHideHpBarObjs(0);
        ShowOrHideExpBarObjs(0);
        break;
    case PSS_PAGE_MOVES:
        if (sMonSummaryScreen->pageFlipDirection == 1)
        {
            PokeSum_ShowOrHideMonPicSprite(1);
            PokeSum_ShowOrHideMonMarkingsSprite(1);
            ShowOrHideBallIconObj(1);
            ShowOrHideStatusIcon(1);
            HideShowPokerusIcon(1);
            HideShowShinyStar(1);
			ShowOrHideHpBarObjs(1);
			ShowOrHideExpBarObjs(1);
        }

        break;
    case PSS_PAGE_MOVES_INFO:
        ShoworHideMoveSelectionCursor(1);
        PokeSum_ShowOrHideMonIconSprite(1);
        ShowOrHideStatusIcon(1);
        HideShowPokerusIcon(1);
        HideShowShinyStar(1);
        ShowOrHideHpBarObjs(1);
		ShowOrHideExpBarObjs(1);
        break;
    }
}

static u8 PokeSum_IsPageFlipFinished(unusedArg u8 a0)
{
    s8 pageDelta = 1;

    if (sMonSummaryScreen->pageFlipDirection == 1)
        pageDelta = -1;

    if (sMonSummaryScreen->curPageIndex == PSS_PAGE_MOVES_INFO)
        if (sMonSummaryScreen->flipPagesBgHofs <= 0)
        {
            sMonSummaryScreen->flipPagesBgHofs = 0;
            sMonSummaryScreen->whichBgLayerToTranslate ^= 1;
            sMonSummaryScreen->flippingPages = FALSE;
            return TRUE;
        }

    if ((sMonSummaryScreen->curPageIndex + pageDelta) == PSS_PAGE_MOVES_INFO)
        if (sMonSummaryScreen->flipPagesBgHofs >= 240)
        {
            sMonSummaryScreen->flipPagesBgHofs = 240;
            sMonSummaryScreen->whichBgLayerToTranslate ^= 1;
            sMonSummaryScreen->flippingPages = FALSE;
            return TRUE;
        }

    if (sMonSummaryScreen->pageFlipDirection == 1)
    {
        if (sMonSummaryScreen->flipPagesBgHofs >= 240)
        {
            sMonSummaryScreen->flipPagesBgHofs = 240;
            sMonSummaryScreen->whichBgLayerToTranslate ^= 1;
            sMonSummaryScreen->flippingPages = FALSE;
            return TRUE;
        }
    }
    else if (sMonSummaryScreen->flipPagesBgHofs <= 0)
    {
        sMonSummaryScreen->whichBgLayerToTranslate ^= 1;
        sMonSummaryScreen->flipPagesBgHofs = 0;
        sMonSummaryScreen->flippingPages = FALSE;
        return TRUE;
    }

    return FALSE;
}

static void PrintMonLevelNickOnWindow2(unusedArg const u8 * str)
{
    FillWindowPixelBuffer(sMonSummaryScreen->windowIds[2], 0);

    if (!sMonSummaryScreen->isEgg)
    {
        if (sMonSummaryScreen->curPageIndex != PSS_PAGE_MOVES_INFO)
		{
			AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[2], 2, 0, 14, sPSSTextColours[DARK], 0xff, sMonSummaryScreen->summary.levelStrBuf);
		}
		else
		{
			BlitMoveInfoIcon(sMonSummaryScreen->windowIds[2], sMonSummaryScreen->monTypes[0] + 1, 6, 16);
			if (sMonSummaryScreen->monTypes[0] != sMonSummaryScreen->monTypes[1])
				BlitMoveInfoIcon(sMonSummaryScreen->windowIds[2], sMonSummaryScreen->monTypes[1] + 1, 38, 16);
		}
        AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[2], 2, 0, 2, sPSSTextColours[DARK], 0xff, sMonSummaryScreen->summary.nicknameStrBuf);
			
		if (GetMonGender(&sMonSummaryScreen->currentMon) == MON_FEMALE)
			AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[2], 2, 62, 2, sPSSTextColours[RED], 0, sMonSummaryScreen->summary.genderSymbolStrBuf);
		else
			AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[2], 2, 62, 2, sPSSTextColours[BLUE], 0, sMonSummaryScreen->summary.genderSymbolStrBuf);
	}

    PutWindowTilemap(sMonSummaryScreen->windowIds[2]);
}

static void BlitMoveInfoIcon(u8 windowId, u8 iconId, u16 x, u16 y)
{
    BlitBitmapRectToWindow(windowId, gFireRedMenuElements_Gfx + gMoveMenuInfoIcons[iconId].offset * 32, 0, 0, 128, 128, x, y, gMoveMenuInfoIcons[iconId].width, gMoveMenuInfoIcons[iconId].height);
}

void PokeSum_CopyNewBgTilemapBeforePageFlip(void)
{
    u8 newPage;

    if (sMonSummaryScreen->pageFlipDirection == 1)
        newPage = sMonSummaryScreen->curPageIndex - 1;
    else
        newPage = sMonSummaryScreen->curPageIndex + 1;

    switch (newPage)
    {
    case PSS_PAGE_INFO:
        break;
    case PSS_PAGE_SKILLS:
        break;
    case PSS_PAGE_MOVES:
        break;
    case PSS_PAGE_MOVES_INFO:
        break;
    }
}

static void PokeSum_ShowSpritesBeforePageFlip(void)
{
    u8 newPage;

    if (sMonSummaryScreen->pageFlipDirection == 1)
        newPage = sMonSummaryScreen->curPageIndex - 1;
    else
        newPage = sMonSummaryScreen->curPageIndex + 1;

    switch (newPage)
    {
    case PSS_PAGE_INFO:
        ShowOrHideHpBarObjs(1);
        ShowOrHideExpBarObjs(1);
        break;
    case PSS_PAGE_SKILLS:
        break;
    case PSS_PAGE_MOVES:
        if (sMonSummaryScreen->pageFlipDirection != 0)
        {
            ShoworHideMoveSelectionCursor(0);
            HideShowPokerusIcon(0);
            PokeSum_ShowOrHideMonIconSprite(0);
            HideShowShinyStar(0);
        }
        break;
    case PSS_PAGE_MOVES_INFO:
        PokeSum_ShowOrHideMonPicSprite(0);
        PokeSum_ShowOrHideMonMarkingsSprite(0);
        ShowOrHideStatusIcon(0);
        ShowOrHideBallIconObj(0);
        HideShowPokerusIcon(0);
        HideShowShinyStar(0);
        break;
    }
}

static void Task_FlipPages_FromInfo(unusedArg u8 taskId)
{
    switch (sMonSummaryScreen->state3284)
    {
    case 0:
        sMonSummaryScreen->lockMovesFlag = TRUE;
        sMonSummaryScreen->inhibitPageFlipInput = TRUE;
        PokeSum_AddWindows(sMonSummaryScreen->curPageIndex);
        break;
    case 1:
        if (sMonSummaryScreen->curPageIndex != PSS_PAGE_MOVES_INFO)
        {
            if (!(sMonSummaryScreen->curPageIndex == PSS_PAGE_MOVES && sMonSummaryScreen->pageFlipDirection == 0))
            {
				
            }
        }
        break;
    case 2:
        PokeSum_HideSpritesBeforePageFlip();
        PokeSum_CopyNewBgTilemapBeforePageFlip();
        PokeSum_DrawPageProgressTiles();
        PokeSum_CopyNewBgTilemapBeforePageFlip_2();
        break;
    case 3:
		PokeSum_PrintPageName((u8*)0x8419C39);

        if (!(gMain.inBattle || gReceivedRemoteLinkPlayers))
		{
			PokeSum_PrintControlsString((u8*)0x8419C92);
		}
        else
		{
			PokeSum_PrintControlsString((u8*)0x8419CA2);
		}
        break;
    case 4:
        CopyWindowToVram(sMonSummaryScreen->windowIds[0], 2);
        CopyWindowToVram(sMonSummaryScreen->windowIds[1], 2);
        break;
    case 5:
        break;
    case 6:
        PokeSum_PrintRightPaneText();
        PokeSum_PrintAbilityDataOrMoveTypes();
        CopyWindowToVram(sMonSummaryScreen->windowIds[3], 2);
        CopyWindowToVram(sMonSummaryScreen->windowIds[5], 2);
        break;
    case 7:
        if (!IsDma3ManagerBusyWithBgCopy())
        {
            CopyBgTilemapBufferToVram(0);
            PokeSum_InitBgCoordsBeforePageFlips();
            sMonSummaryScreen->flippingPages = TRUE;
        }
        else
            return;
        break;
    case 8:
        if (!PokeSum_IsPageFlipFinished(sMonSummaryScreen->pageFlipDirection))
            return;

        PokeSum_PrintBottomPaneText();
        CopyWindowToVram(sMonSummaryScreen->windowIds[4], 2);
        break;
    case 9:
        PokeSum_PrintMonTypeIcons();
        PrintMonLevelNickOnWindow2(gText_PokeSum_NoData);
        break;
    case 10:
        PokeSum_ShowSpritesBeforePageFlip();
        CopyWindowToVram(sMonSummaryScreen->windowIds[6], 2);
        CopyWindowToVram(sMonSummaryScreen->windowIds[2], 2);
        break;
    case 11:
        if (!IsDma3ManagerBusyWithBgCopy())
        {
            CopyBgTilemapBufferToVram(0);
        }
        else
            return;
        break;
    default:
        PokeSum_SetHelpContext();
        gTasks[sMonSummaryScreen->inputHandlerTaskId].func = Task_HandleInput_SelectMove;
        sMonSummaryScreen->state3284 = 0;
        sMonSummaryScreen->lockMovesFlag = FALSE;
        sMonSummaryScreen->inhibitPageFlipInput = FALSE;
        return;
    }

    sMonSummaryScreen->state3284++;
    return;
}

static void Task_InputHandler_Info(u8 taskId)
{
    switch (sMonSummaryScreen->state3270) {
    case PSS_STATE3270_FADEIN:
        BeginNormalPaletteFade(0xffffffff, 0, 16, 0, 0);
        sMonSummaryScreen->state3270 = PSS_STATE3270_PLAYCRY;
        break;
    case PSS_STATE3270_PLAYCRY:
        if (!gPaletteFade->active)
        {
            PokeSum_TryPlayMonCry();
            sMonSummaryScreen->state3270 = PSS_STATE3270_HANDLEINPUT;
            return;
        }

        sMonSummaryScreen->state3270 = PSS_STATE3270_PLAYCRY;
        break;
    case PSS_STATE3270_HANDLEINPUT:
        if (IsActiveOverworldLinkBusy() == TRUE)
            return;
        else if (IsLinkRecvQueueAtOverworldMax() == TRUE)
            return;
        else if (FuncIsActiveTask(Task_PokeSum_SwitchDisplayedPokemon))
            return;

        if (HasOnlyOneMon())
        {
            if (JOY_NEW(DPAD_UP) || JOY_NEW(DPAD_DOWN))
            {
                PlaySE(SE_ERROR); // Toca um som de erro
                return;
            }
        }

        if (sMonSummaryScreen->curPageIndex != PSS_PAGE_MOVES_INFO)
        {
            if (IsPageFlipInput(1) == TRUE)
            {
                if (FuncIsActiveTask(Task_PokeSum_FlipPages))
                {
                    sMonSummaryScreen->lastPageFlipDirection = 1;
                    return;
                }
                else if (sMonSummaryScreen->curPageIndex < PSS_PAGE_MOVES)
                {
                    PlaySE(SE_SELECT);
                    sMonSummaryScreen->pageFlipDirection = 1;
                    PokeSum_RemoveWindows(sMonSummaryScreen->curPageIndex);
                    sMonSummaryScreen->curPageIndex++;
                    sMonSummaryScreen->state3270 = PSS_STATE3270_FLIPPAGES;
                }
                return;
            }
            else if (IsPageFlipInput(0) == TRUE)
            {
                if (FuncIsActiveTask(Task_PokeSum_FlipPages))
                {
                    sMonSummaryScreen->lastPageFlipDirection = 0;
                    return;
                }
                else if (sMonSummaryScreen->curPageIndex > PSS_PAGE_INFO)
                {
                    PlaySE(SE_SELECT);
                    sMonSummaryScreen->pageFlipDirection = 0;
                    PokeSum_RemoveWindows(sMonSummaryScreen->curPageIndex);
                    sMonSummaryScreen->curPageIndex--;
                    sMonSummaryScreen->state3270 = PSS_STATE3270_FLIPPAGES;
                }
                return;
            }
        }

        if ((!FuncIsActiveTask(Task_PokeSum_FlipPages)) || FuncIsActiveTask(Task_PokeSum_SwitchDisplayedPokemon))
        {
            if (JOY_NEW(DPAD_UP))
            {
                PokeSum_SeekToNextMon(taskId, -1);
                return;
            }
            else if (JOY_NEW(DPAD_DOWN))
            {
                PokeSum_SeekToNextMon(taskId, 1);
                return;
            }
            else if (JOY_NEW(A_BUTTON))
            {
                if (sMonSummaryScreen->curPageIndex == PSS_PAGE_INFO)
                {
                    PlaySE(SE_SELECT);
                    sMonSummaryScreen->state3270 = PSS_STATE3270_ATEXIT_FADEOUT;
                }
                else if (sMonSummaryScreen->curPageIndex == PSS_PAGE_MOVES)
                {
                    PlaySE(SE_SELECT);
                    sMonSummaryScreen->pageFlipDirection = 1;
                    PokeSum_RemoveWindows(sMonSummaryScreen->curPageIndex);
                    sMonSummaryScreen->curPageIndex++;
                    sMonSummaryScreen->state3270 = PSS_STATE3270_FLIPPAGES;
                }
                return;
            }
            else if (JOY_NEW(B_BUTTON))
            {
                sMonSummaryScreen->state3270 = PSS_STATE3270_ATEXIT_FADEOUT;
            }
        }
        break;
    case PSS_STATE3270_FLIPPAGES:
        if (sMonSummaryScreen->curPageIndex != PSS_PAGE_MOVES_INFO)
        {
            CreateTask(Task_PokeSum_FlipPages, 0);
            sMonSummaryScreen->state3270 = PSS_STATE3270_HANDLEINPUT;
        }
        else
        {
            gTasks[sMonSummaryScreen->inputHandlerTaskId].func = Task_FlipPages_FromInfo;
            sMonSummaryScreen->state3270 = PSS_STATE3270_HANDLEINPUT;
        }
        break;
    case PSS_STATE3270_ATEXIT_FADEOUT:
        BeginNormalPaletteFade(0xffffffff, 0, 0, 16, 0);
        sMonSummaryScreen->state3270 = PSS_STATE3270_ATEXIT_WAITLINKDELAY;
        break;
    case PSS_STATE3270_ATEXIT_WAITLINKDELAY:
		SetGpuReg(REG_OFFSET_BLDCNT, 0);
		SetGpuReg(REG_OFFSET_BLDALPHA, 0);

        if (Overworld_LinkRecvQueueLengthMoreThan2() == TRUE)
            return;
        else if (IsLinkRecvQueueAtOverworldMax() == TRUE)
            return;

        sMonSummaryScreen->state3270 = PSS_STATE3270_EV_IV;
        break;
    default:
        if (!gPaletteFade->active)
            Task_DestroyResourcesOnExit(taskId);

        break;
    }
}

static void Task_PokeSum_SwitchDisplayedPokemon(u8 taskId)
{
    switch (sMonSummaryScreen->switchMonTaskState)
    {
    case 0:
        StopCryAndClearCrySongs();
        sMoveSelectionCursorPos = 0;
        sMoveSwapCursorPos = 0;
        sMonSummaryScreen->switchMonTaskState++;
        break;
    case 1:
        PokeSum_DestroyMonPicSprite();
        PokeSum_DestroyMonIconSprite();
        DestroyBallIconObj();
        sMonSummaryScreen->switchMonTaskState++;
        break;
    case 2:
        BufferSelectedMonData(&sMonSummaryScreen->currentMon);

        sMonSummaryScreen->isEgg = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_IS_EGG, NULL);
        sMonSummaryScreen->isBadEgg = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_SANITY_IS_BAD_EGG, NULL);

        if (sMonSummaryScreen->isBadEgg == TRUE)
            sMonSummaryScreen->isEgg = TRUE;

        sMonSummaryScreen->switchMonTaskState++;
        break;
    case 3:
        sMonSummaryScreen->switchMonTaskState++;
        break;
    case 4:
        sMonSummaryScreen->switchMonTaskState++;
        break;
    case 5:
        BufferMonInfo();
        sMonSummaryScreen->switchMonTaskState++;
        break;
    case 6:
        if (!sMonSummaryScreen->isEgg)
            BufferMonSkills();

        sMonSummaryScreen->switchMonTaskState++;
        break;
    case 7:
        if (!sMonSummaryScreen->isEgg)
            BufferMonMoves();

        sMonSummaryScreen->switchMonTaskState++;
        break;
    case 8:
        PokeSum_PrintRightPaneText();
        PokeSum_PrintBottomPaneText();
        PokeSum_PrintAbilityDataOrMoveTypes();
        sMonSummaryScreen->switchMonTaskState++;
        break;
    case 9:
        PokeSum_PrintMonTypeIcons();
        PokeSum_DrawPageProgressTiles();
        PokeSum_PrintPageHeaderText(sMonSummaryScreen->curPageIndex);
        sMonSummaryScreen->switchMonTaskState++;
        break;
    case 10:
        CopyWindowToVram(sMonSummaryScreen->windowIds[0], 2);
        CopyWindowToVram(sMonSummaryScreen->windowIds[1], 2);
        CopyWindowToVram(sMonSummaryScreen->windowIds[2], 2);
        CopyWindowToVram(sMonSummaryScreen->windowIds[3], 2);
        CopyWindowToVram(sMonSummaryScreen->windowIds[4], 2);
        CopyWindowToVram(sMonSummaryScreen->windowIds[5], 2);
        CopyWindowToVram(sMonSummaryScreen->windowIds[6], 2);
        CopyBgTilemapBufferToVram(0);
        sMonSummaryScreen->switchMonTaskState++;
        break;
    case 11:
        if (!Overworld_LinkRecvQueueLengthMoreThan2() && !IsLinkRecvQueueAtOverworldMax())
        {
            PokeSum_CreateSprites();
            PokeSum_TryPlayMonCry();
            sMonSummaryScreen->switchMonTaskState++;
        }
        break;
    default:
        sMonSummaryScreen->switchMonTaskState = 0;
        DestroyTask(taskId);
        break;
    }
}

static void Task_PokeSum_FlipPages(u8 taskId)
{
    s16 * data = gTasks[taskId].data;

    switch (data[0])
    {
    case 0:
        PokeSum_HideSpritesBeforePageFlip();
        PokeSum_ShowSpritesBeforePageFlip();
        sMonSummaryScreen->lockMovesFlag = TRUE;
        sMonSummaryScreen->inhibitPageFlipInput = TRUE;
        PokeSum_AddWindows(sMonSummaryScreen->curPageIndex);
        break;
    case 1:
        if (sMonSummaryScreen->curPageIndex != PSS_PAGE_MOVES_INFO)
        {
            if (!(sMonSummaryScreen->curPageIndex == PSS_PAGE_MOVES && sMonSummaryScreen->pageFlipDirection == 0))
            {
				
            }
        }
        break;
    case 2:
        PokeSum_CopyNewBgTilemapBeforePageFlip_2();
        PokeSum_CopyNewBgTilemapBeforePageFlip();
        PokeSum_DrawPageProgressTiles();
        PokeSum_PrintPageHeaderText(sMonSummaryScreen->curPageIndex);
        break;
    case 3:
        CopyWindowToVram(sMonSummaryScreen->windowIds[0], 2);
        CopyWindowToVram(sMonSummaryScreen->windowIds[1], 2);
        CopyWindowToVram(sMonSummaryScreen->windowIds[2], 2);
        break;
    case 4:
        break;
    case 5:
        PokeSum_InitBgCoordsBeforePageFlips();
        sMonSummaryScreen->flippingPages = TRUE;
        break;
    case 6:
        if (!PokeSum_IsPageFlipFinished(sMonSummaryScreen->pageFlipDirection))
            return;

        break;
    case 7:
        PokeSum_PrintRightPaneText();
        if (sMonSummaryScreen->curPageIndex != PSS_PAGE_MOVES_INFO)
            PokeSum_PrintBottomPaneText();

        PokeSum_PrintAbilityDataOrMoveTypes();
        PokeSum_PrintMonTypeIcons();
        break;
    case 8:
        CopyWindowToVram(sMonSummaryScreen->windowIds[3], 2);
        CopyWindowToVram(sMonSummaryScreen->windowIds[4], 2);
        CopyWindowToVram(sMonSummaryScreen->windowIds[5], 2);
        CopyWindowToVram(sMonSummaryScreen->windowIds[6], 2);
        break;
    case 9:
        HideBg(0);
		if (!IsDma3ManagerBusyWithBgCopy())
        {
            CopyBgTilemapBufferToVram(0);
            ShowBg(0);
        }
        else
            return;

        break;
    default:
        PokeSum_SetHelpContext();

        if (sMonSummaryScreen->curPageIndex == PSS_PAGE_MOVES_INFO)
            gTasks[sMonSummaryScreen->inputHandlerTaskId].func = Task_HandleInput_SelectMove;

        DestroyTask(taskId);
        data[0] = 0;
        sMonSummaryScreen->lockMovesFlag = FALSE;
        sMonSummaryScreen->inhibitPageFlipInput = FALSE;
        return;
    }

    data[0]++;
}

static void Task_HandleInput_SelectMove(unusedArg u8 taskId)
{
    u8 i;

    switch (sMonSummaryScreen->selectMoveInputHandlerState)
    {
    case 0:
        if (IsActiveOverworldLinkBusy() == TRUE || IsLinkRecvQueueAtOverworldMax() == TRUE)
            return;

        if (JOY_NEW(DPAD_UP))
        {
            if (sMoveSelectionCursorPos > 0)
            {
                sMonSummaryScreen->selectMoveInputHandlerState = 2;
                PlaySE(SE_SELECT);

                for (i = sMoveSelectionCursorPos; i > 0; i--)
                    if (sMonSummaryScreen->moveIds[i - 1] != 0)
                    {
                        PlaySE(SE_SELECT);
                        sMoveSelectionCursorPos = i - 1;
                        return;
                    }
            }
            else
            {
                sMoveSelectionCursorPos = 4;
                sMonSummaryScreen->selectMoveInputHandlerState = 2;
                PlaySE(SE_SELECT);

                if (sMonSummaryScreen->isSwappingMoves == TRUE)
                    for (i = sMoveSelectionCursorPos; i > 0; i--)
                        if (sMonSummaryScreen->moveIds[i - 1] != 0)
                        {
                            PlaySE(SE_SELECT);
                            sMoveSelectionCursorPos = i - 1;
                            return;
                        }
            }
        }
        else if (JOY_NEW(DPAD_DOWN))
        {
            if (sMoveSelectionCursorPos < 4)
            {
                u8 v0 = 4;

                sMonSummaryScreen->selectMoveInputHandlerState = 2;

                if (sMonSummaryScreen->isSwappingMoves == TRUE)
                {
                    if (sMoveSelectionCursorPos == 5 - 2)
                    {
                        sMoveSelectionCursorPos = 0;
                        sMonSummaryScreen->selectMoveInputHandlerState = 2;
                        PlaySE(SE_SELECT);
                        return;
                    }
                    v0--;
                }

                for (i = sMoveSelectionCursorPos; i < v0; i++)
                    if (sMonSummaryScreen->moveIds[i + 1] != 0)
                    {
                        PlaySE(SE_SELECT);
                        sMoveSelectionCursorPos = i + 1;
                        return;
                    }

                if (!sMonSummaryScreen->isSwappingMoves)
                {
                    PlaySE(SE_SELECT);
                    sMoveSelectionCursorPos = i;
                }
                else
                {
                    PlaySE(SE_SELECT);
                    sMoveSelectionCursorPos = 0;
                }

                return;
            }
            else if (sMoveSelectionCursorPos == 4)
            {
                sMoveSelectionCursorPos = 0;
                sMonSummaryScreen->selectMoveInputHandlerState = 2;
                PlaySE(SE_SELECT);
                return;
            }
        }
        else if (JOY_NEW(A_BUTTON))
        {
            PlaySE(SE_SELECT);
            if (sMoveSelectionCursorPos == 4)
            {
                sMoveSelectionCursorPos = 0;
                sMoveSwapCursorPos = 0;
                sMonSummaryScreen->isSwappingMoves = FALSE;
                ShoworHideMoveSelectionCursor(1);
                sMonSummaryScreen->pageFlipDirection = 0;
                PokeSum_RemoveWindows(sMonSummaryScreen->curPageIndex);
                sMonSummaryScreen->curPageIndex--;
                sMonSummaryScreen->selectMoveInputHandlerState = 1;
                return;
            }

            if (sMonSummaryScreen->isSwappingMoves != TRUE)
            {
                if (sMonSummaryScreen->isEnemyParty == FALSE
                    && gMain.inBattle == 0
                    && gReceivedRemoteLinkPlayers == 0)
                {
                    sMoveSwapCursorPos = sMoveSelectionCursorPos;
                    sMonSummaryScreen->isSwappingMoves = TRUE;
                }
                return;
            }
            else
            {
                sMonSummaryScreen->isSwappingMoves = FALSE;

                if (sMoveSelectionCursorPos == sMoveSwapCursorPos)
                    return;

                if (sMonSummaryScreen->isBoxMon == 0)
                    SwapMonMoveSlots();
                else
                    SwapBoxMonMoveSlots();

                UpdateCurrentMonBufferFromPartyOrBox(&sMonSummaryScreen->currentMon);
                BufferMonMoves();
                sMonSummaryScreen->selectMoveInputHandlerState = 2;
                return;
            }
        }
        else if (JOY_NEW(B_BUTTON))
        {
            if (sMonSummaryScreen->isSwappingMoves == TRUE)
            {
                sMoveSwapCursorPos = sMoveSelectionCursorPos;
                sMonSummaryScreen->isSwappingMoves = FALSE;
                return;
            }

            if (sMoveSelectionCursorPos == 4)
            {
                sMoveSelectionCursorPos = 0;
                sMoveSwapCursorPos = 0;
            }

            ShoworHideMoveSelectionCursor(1);
            sMonSummaryScreen->pageFlipDirection = 0;
            PokeSum_RemoveWindows(sMonSummaryScreen->curPageIndex);
            sMonSummaryScreen->curPageIndex--;
            sMonSummaryScreen->selectMoveInputHandlerState = 1;
        }
        break;
    case 1:
        gTasks[sMonSummaryScreen->inputHandlerTaskId].func = Task_BackOutOfSelectMove;
        sMonSummaryScreen->selectMoveInputHandlerState = 0;
        break;
    case 2:
        PokeSum_PrintRightPaneText();
        PokeSum_PrintBottomPaneText();
        PokeSum_PrintAbilityDataOrMoveTypes();
        sMonSummaryScreen->selectMoveInputHandlerState = 3;
        break;
    case 3:
        if (IsActiveOverworldLinkBusy() == TRUE || IsLinkRecvQueueAtOverworldMax() == TRUE)
            return;

        CopyWindowToVram(sMonSummaryScreen->windowIds[3], 2);
        CopyWindowToVram(sMonSummaryScreen->windowIds[4], 2);
        CopyWindowToVram(sMonSummaryScreen->windowIds[5], 2);
        CopyWindowToVram(sMonSummaryScreen->windowIds[6], 2);
        CopyBgTilemapBufferToVram(0);
        sMonSummaryScreen->selectMoveInputHandlerState = 0;
        break;
    default:
        break;
    }
}

static void PokeSum_CreateSprites(void)
{
    CreateBallIconObj();
    ShowOrHideBallIconObj(0);
    PokeSum_CreateMonIconSprite();
    PokeSum_CreateMonPicSprite();
    PokeSum_ShowOrHideMonPicSprite(0);
    UpdateHpBarObjs();
    UpdateExpBarObjs();
    PokeSum_UpdateMonMarkingsAnim();
    UpdateMonStatusIconObj();
    ShowPokerusIconObjIfHasOrHadPokerus();
    ShowShinyStarObjIfMonShiny();
}

static void PokeSum_PrintTrainerMemo_Mon_HeldByOT(void)
{
    u8 nature;
    u8 level;
    u8 metLocation;
    u8 levelStr[5];
    u8 mapNameStr[32];
    u8 natureMetOrHatchedAtLevelStr[152];

    DynamicPlaceholderTextUtil_Reset();
    nature = GetNature(&sMonSummaryScreen->currentMon);
    DynamicPlaceholderTextUtil_SetPlaceholderPtr(0, gNatureNamePointers[nature]);
    level = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_MET_LEVEL, NULL);

    if (level == 0)
        level = 5;

    ConvertIntToDecimalStringN(levelStr, level, STR_CONV_MODE_LEFT_ALIGN, 3);
    DynamicPlaceholderTextUtil_SetPlaceholderPtr(1, levelStr);

    metLocation = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_MET_LOCATION, NULL);

    if (MapSecIsInKantoOrSevii(metLocation) == TRUE)
        GetMapNameGeneric_(mapNameStr, metLocation);
    else
    {
        if (sMonSummaryScreen->isEnemyParty == TRUE || IsMultiBattlePartner() == TRUE)
            StringCopy(mapNameStr, (u8*)0x8419c13);
        else
            StringCopy(mapNameStr, (u8*)0x8419c0b);
    }

    DynamicPlaceholderTextUtil_SetPlaceholderPtr(2, mapNameStr);

    // These pairs of strings are bytewise identical to each other in English,
    // but Japanese uses different grammar for Bold and Gentle natures.
    if (GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_MET_LEVEL, NULL) == 0) // Hatched
    {
        if (GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_OBEDIENCE, NULL) == TRUE)
        {
            if (PokeSum_IsMonBoldOrGentle(nature))
                DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, (u8*)0x841996d);
            else
                DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, (u8*)0x841992f);
        }
        else
        {
            if (PokeSum_IsMonBoldOrGentle(nature))
                DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, (u8*)0x84198d5);
            else
                DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, (u8*)0x84198b4);
        }
    }
    else
    {
        if (metLocation == METLOC_FATEFUL_ENCOUNTER)
        {
            if (PokeSum_IsMonBoldOrGentle(nature))
                DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, (u8*)0x84197ed);
            else
                DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, (u8*)0x84197b8);
        }
        else
        {
            if (PokeSum_IsMonBoldOrGentle(nature))
                DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, (u8*)0x8419841);
            else
                DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, (u8*)0x8419822);
        }
    }

    AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[4], 2, 6, 0, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_TrainerMemo);
	AddTextPrinterParameterized4(sMonSummaryScreen->windowIds[4], 2, 16, 12, 0, -2, sPSSTextColours[DARK], TEXT_SPEED_FF, natureMetOrHatchedAtLevelStr);
}

void PokeSum_PrintTrainerMemo_Mon_NotHeldByOT(void)
{
    u8 nature;
    u8 level;
    u8 metLocation;
    u8 levelStr[5];
    u8 mapNameStr[32];
    u8 natureMetOrHatchedAtLevelStr[152];

    DynamicPlaceholderTextUtil_Reset();
    nature = GetNature(&sMonSummaryScreen->currentMon);
    DynamicPlaceholderTextUtil_SetPlaceholderPtr(0, gNatureNamePointers[nature]);

    level = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_MET_LEVEL, NULL);

    if (level == 0)
        level = 5;

    ConvertIntToDecimalStringN(levelStr, level, STR_CONV_MODE_LEFT_ALIGN, 3);
    DynamicPlaceholderTextUtil_SetPlaceholderPtr(1, levelStr);

    metLocation = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_MET_LOCATION, NULL);

    if (!MapSecIsInKantoOrSevii(metLocation) || !CurrentMonIsFromGBA())
    {
        if (IsMultiBattlePartner() == TRUE)
        {
            PokeSum_PrintTrainerMemo_Mon_HeldByOT();
            return;
        }

        if (metLocation == METLOC_FATEFUL_ENCOUNTER)
        {
            if (PokeSum_IsMonBoldOrGentle(nature))
                DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, (u8*)0x84197ed);
            else
                DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, (u8*)0x84197b8);
        }
        else
        {
            if (PokeSum_IsMonBoldOrGentle(nature))
                DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, (u8*)0x841979d);
            else
                DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, (u8*)0x8419782);
        }

        AddTextPrinterParameterized4(sMonSummaryScreen->windowIds[POKESUM_WIN_TRAINER_MEMO], FONT_NORMAL, 0, 3, 0, 0, sLevelNickTextColors[0], TEXT_SKIP_DRAW, natureMetOrHatchedAtLevelStr);
        return;
    }

    if (MapSecIsInKantoOrSevii(metLocation) == TRUE)
        GetMapNameGeneric_(mapNameStr, metLocation);
    else
        StringCopy(mapNameStr, (u8*)0x8419c0b);

    DynamicPlaceholderTextUtil_SetPlaceholderPtr(2, mapNameStr);

    // These pairs of strings are bytewise identical to each other in English,
    // but Japanese uses different grammar for Bold and Gentle natures.
    if (GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_MET_LEVEL, NULL) == 0) // hatched from an EGG
    {
        if (GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_OBEDIENCE, NULL) == TRUE)
        {
            if (PokeSum_IsMonBoldOrGentle(nature))
                DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, (u8*)0x84199f4);
            else
                DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, (u8*)0x84199ab);
        }
        else
        {
            if (PokeSum_IsMonBoldOrGentle(nature))
                DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, (u8*)0x841988a);
            else
                DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, (u8*)0x8419860);
        }
    }
    else
    {
        if (metLocation == METLOC_FATEFUL_ENCOUNTER)
        {
            if (PokeSum_IsMonBoldOrGentle(nature))
                DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, (u8*)0x84197ed);
            else
                DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, (u8*)0x84197b8);
        }
        else
        {
            if (PokeSum_IsMonBoldOrGentle(nature))
                DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, (u8*)0x841988a);
            else
                DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, (u8*)0x8419860);
        }
    }

    AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[4], 2, 6, 0, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_TrainerMemo);
	AddTextPrinterParameterized4(sMonSummaryScreen->windowIds[4], 2, 16, 12, 0, -2, sPSSTextColours[DARK], TEXT_SPEED_FF, natureMetOrHatchedAtLevelStr);
}

static void PokeSum_CreateMonPicSprite(void)
{
    u16 spriteId;
    u16 species;
    u32 personality;
    u32 trainerId;

    sMonPicBounceState = AllocZeroed(sizeof(struct MonPicBounceState));

    species = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_SPECIES2, NULL);
    personality = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_PERSONALITY, NULL);
    trainerId = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_OT_ID, NULL);

    if (sMonSummaryScreen->savedCallback == CB2_ReturnToTradeMenuFromSummary)
    {
        if (sMonSummaryScreen->isEnemyParty == TRUE)
            spriteId = CreateMonPicSprite(species, trainerId, personality, TRUE, 204, 78, 12, 0xffff, TRUE);
        else
            spriteId = CreateMonPicSprite_HandleDeoxys(species, trainerId, personality, TRUE, 204, 78, 12, 0xffff);
    }
    else
    {
        if (ShouldIgnoreDeoxysForm(DEOXYS_CHECK_TRADE_MAIN, sLastViewedMonIndex))
            spriteId = CreateMonPicSprite(species, trainerId, personality, TRUE, 204, 78, 12, 0xffff, TRUE);
        else
            spriteId = CreateMonPicSprite_HandleDeoxys(species, trainerId, personality, TRUE, 204, 78, 12, 0xffff);
    }

    FreeSpriteOamMatrix(&gSprites[spriteId]);

    if (!IsMonSpriteNotFlipped(species))
        gSprites[spriteId].hFlip = TRUE;
    else
        gSprites[spriteId].hFlip = FALSE;

    sMonSummaryScreen->monPicSpriteId = spriteId;

    PokeSum_ShowOrHideMonPicSprite(TRUE);
    PokeSum_SetMonPicSpriteCallback(spriteId);
}

static void CreateMoveSelectionCursorObjs(u16 tileTag, u16 palTag)
{
    u8 i;
    u8 spriteId;
    void * gfxBufferPtrs[2];
    gfxBufferPtrs[0] = AllocZeroed(0x20 * 64);
    gfxBufferPtrs[1] = AllocZeroed(0x20 * 64);

    sMoveSelectionCursorObjs[0] = AllocZeroed(sizeof(struct MoveSelectionCursor));
    sMoveSelectionCursorObjs[1] = AllocZeroed(sizeof(struct MoveSelectionCursor));
    sMoveSelectionCursorObjs[2] = AllocZeroed(sizeof(struct MoveSelectionCursor));
    sMoveSelectionCursorObjs[3] = AllocZeroed(sizeof(struct MoveSelectionCursor));

    LZ77UnCompWram(gSelectCursorGfxLeft, gfxBufferPtrs[0]);
    LZ77UnCompWram(gSelectCursorGfxRight, gfxBufferPtrs[1]);

    for (i = 0; i < 4; i++)
    {
        struct SpriteSheet sheet = {
            .data = gfxBufferPtrs[i % 2],
            .size = 0x20 * 64,
            .tag = tileTag + i
        };

        struct SpritePalette palette = {.data = sMoveSelectionCursorPals, .tag = palTag};
        struct SpriteTemplate template = {
            .tileTag = tileTag + i,
            .paletteTag = palTag,
            .oam = &sMoveSelectionCursorOamData,
            .anims = sMoveSelectionCursorOamAnimTable,
            .images = NULL,
            .affineAnims = gDummySpriteAffineAnimTable,
            .callback = SpriteCB_MoveSelectionCursor,
        };

        LoadSpriteSheet(&sheet);
        LoadSpritePalette(&palette);

        spriteId = CreateSprite(&template, 64 * (i % 2) + 32, sMoveSelectionCursorPos * 28 + 34, i % 2);
        sMoveSelectionCursorObjs[i]->sprite = &gSprites[spriteId];
        sMoveSelectionCursorObjs[i]->tileTag = i;
        sMoveSelectionCursorObjs[i]->palTag = tileTag + i;
        sMoveSelectionCursorObjs[i]->unk08 = palTag;
        sMoveSelectionCursorObjs[i]->sprite->subpriority = i;

        if (i > 1)
            StartSpriteAnim(sMoveSelectionCursorObjs[i]->sprite, 1);
    }

    ShoworHideMoveSelectionCursor(1);

    FREE_AND_SET_NULL_IF_SET(gfxBufferPtrs[0]);
    FREE_AND_SET_NULL_IF_SET(gfxBufferPtrs[1]);
}

void Safe_LoadMonIconPalette(u16 species)
{
    u8 palIndex;
    if (species > NUM_SPECIES)
        species = SPECIES_NONE;
    palIndex = gMonIconPaletteIndices[species];
    if (IndexOfSpritePaletteTag(gMonIconPaletteTable[palIndex].tag) == 0xFF)
        LoadSpritePalette(&gMonIconPaletteTable[palIndex]);
}

static void PokeSum_CreateMonIconSprite(void)
{
    u16 species;
    u32 personality;

    species = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_SPECIES2, NULL);
    personality = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_PERSONALITY, NULL);

    LoadMonIconPalette(species);

    if (sMonSummaryScreen->savedCallback == CB2_ReturnToTradeMenuFromSummary)
    {
        if (sMonSummaryScreen->isEnemyParty == TRUE)
            sMonSummaryScreen->monIconSpriteId = CreateMonIcon(species, SpriteCallbackDummy, 140, 28, 0, personality, 0);
        else
            sMonSummaryScreen->monIconSpriteId = CreateMonIcon(species, SpriteCallbackDummy, 140, 28, 0, personality, 1);
    }
    else
    {
        if (ShouldIgnoreDeoxysForm(3, sLastViewedMonIndex))
            sMonSummaryScreen->monIconSpriteId = CreateMonIcon(species, SpriteCallbackDummy, 140, 28, 0, personality, 0);
        else
            sMonSummaryScreen->monIconSpriteId = CreateMonIcon(species, SpriteCallbackDummy, 140, 28, 0, personality, 1);
    }

    if (!IsMonSpriteNotFlipped(species))
        gSprites[sMonSummaryScreen->monIconSpriteId].hFlip = FALSE;
    else
        gSprites[sMonSummaryScreen->monIconSpriteId].hFlip = TRUE;

    PokeSum_ShowOrHideMonIconSprite(1);
}

static void PokeSum_PrintRightPaneText(void)
{
    FillWindowPixelBuffer(sMonSummaryScreen->windowIds[POKESUM_WIN_RIGHT_PANE], 0);

    switch (sMonSummaryScreen->curPageIndex)
    {
    case PSS_PAGE_INFO:
        PrintInfoPage();
        break;
    case PSS_PAGE_SKILLS:
        PrintSkillsPage();
        break;
    case PSS_PAGE_MOVES:
    case PSS_PAGE_MOVES_INFO:
        PrintMovesPage();
        break;
    }

    PutWindowTilemap(sMonSummaryScreen->windowIds[POKESUM_WIN_RIGHT_PANE]);
}

static void Task_InputHandler_SelectOrForgetMove(u8 taskId)
{
    u8 i;

    switch (sMonSummaryScreen->selectMoveInputHandlerState)
    {
    case 0:
        BeginNormalPaletteFade(0xffffffff, 0, 16, 0, 0);
        sMonSummaryScreen->selectMoveInputHandlerState++;
        break;
    case 1:
        if (!gPaletteFade->active)
        {
            PokeSum_TryPlayMonCry();
            sMonSummaryScreen->selectMoveInputHandlerState++;
        }
        break;
    case 2:
        if (JOY_NEW(DPAD_UP))
        {
            if (sMoveSelectionCursorPos > 0)
            {
                sMonSummaryScreen->selectMoveInputHandlerState = 3;
                PlaySE(SE_SELECT);
                for (i = sMoveSelectionCursorPos; i > 0; i--)
                    if (sMonSummaryScreen->moveIds[i - 1] != 0)
                    {
                        PlaySE(SE_SELECT);
                        sMoveSelectionCursorPos = i - 1;
                        return;
                    }
            }
            else
            {
                sMoveSelectionCursorPos = 4;
                sMonSummaryScreen->selectMoveInputHandlerState = 3;
                PlaySE(SE_SELECT);
                return;
            }
        }
        else if (JOY_NEW(DPAD_DOWN))
        {
            if (sMoveSelectionCursorPos < 4)
            {
                u8 v0 = 4;

                sMonSummaryScreen->selectMoveInputHandlerState = 3;

                if (sMonSummaryScreen->isSwappingMoves == TRUE)
                    v0--;

                for (i = sMoveSelectionCursorPos; i < v0; i++)
                    if (sMonSummaryScreen->moveIds[i + 1] != 0)
                    {
                        PlaySE(SE_SELECT);
                        sMoveSelectionCursorPos = i + 1;
                        return;
                    }

                if (!sMonSummaryScreen->isSwappingMoves)
                {
                    PlaySE(SE_SELECT);
                    sMoveSelectionCursorPos = i;
                }

                return;
            }
            else if (sMoveSelectionCursorPos == 4)
            {
                sMoveSelectionCursorPos = 0;
                sMonSummaryScreen->selectMoveInputHandlerState = 3;
                PlaySE(SE_SELECT);
                return;
            }
        }
        else if (JOY_NEW(A_BUTTON))
        {
            if (PokeSum_CanForgetSelectedMove() == TRUE || sMoveSelectionCursorPos == 4)
            {
                PlaySE(SE_SELECT);
                sMoveSwapCursorPos = sMoveSelectionCursorPos;
                Var8005 = sMoveSwapCursorPos;
                sMonSummaryScreen->selectMoveInputHandlerState = 6;
            }
            else
            {
                PlaySE(SE_ERROR);
                sMonSummaryScreen->selectMoveInputHandlerState = 5;
            }
        }
        else if (JOY_NEW(B_BUTTON))
        {
            sMoveSwapCursorPos = 4;
            Var8005 = (u16)sMoveSwapCursorPos;
            sMonSummaryScreen->selectMoveInputHandlerState = 6;
        }
        break;
    case 3:
        PokeSum_PrintRightPaneText();
        PokeSum_PrintBottomPaneText();
        PokeSum_PrintAbilityDataOrMoveTypes();
        sMonSummaryScreen->selectMoveInputHandlerState = 4;
        break;
    case 4:
        if (MenuHelpers_CallLinkSomething() == TRUE || IsLinkRecvQueueAtOverworldMax() == TRUE)
            return;

        CopyWindowToVram(sMonSummaryScreen->windowIds[3], 2);
        CopyWindowToVram(sMonSummaryScreen->windowIds[4], 2);
        CopyWindowToVram(sMonSummaryScreen->windowIds[5], 2);
        CopyWindowToVram(sMonSummaryScreen->windowIds[6], 2);
        CopyBgTilemapBufferToVram(0);
        sMonSummaryScreen->selectMoveInputHandlerState = 2;
        break;
    case 5:
        FillWindowPixelBuffer(sMonSummaryScreen->windowIds[4], 0);
        AddTextPrinterParameterized4(sMonSummaryScreen->windowIds[4], 2,
                                     7, 42,
                                     0, 0,
                                     sPSSTextColours[DARK], TEXT_SPEED_FF,
                                     (u8*)0x8419CB9);
        CopyWindowToVram(sMonSummaryScreen->windowIds[4], 2);
        CopyBgTilemapBufferToVram(0);
        sMonSummaryScreen->selectMoveInputHandlerState = 2;
        break;
    case 6:
        BeginNormalPaletteFade(0xffffffff, 0, 0, 16, 0);
        sMonSummaryScreen->selectMoveInputHandlerState++;
        break;
    default:
        if (!gPaletteFade->active)
            Task_DestroyResourcesOnExit(taskId);
        break;
    }
}

static void PokeSum_FinishSetup(void)
{
    if (sMonSummaryScreen->mode == PSS_MODE_SELECT_MOVE || sMonSummaryScreen->mode == PSS_MODE_FORGET_MOVE)
        sMonSummaryScreen->inputHandlerTaskId = CreateTask(Task_InputHandler_SelectOrForgetMove, 0);
    else
        sMonSummaryScreen->inputHandlerTaskId = CreateTask(Task_InputHandler_Info, 0);

    SetMainCallback2(CB2_RunPokemonSummaryScreen);
}

static void PokeSum_PrintBottomPaneText(void)
{
    FillWindowPixelBuffer(sMonSummaryScreen->windowIds[POKESUM_WIN_TRAINER_MEMO], 0);

    switch (sMonSummaryScreen->curPageIndex)
    {
    case PSS_PAGE_INFO:
        PokeSum_PrintTrainerMemo();
        break;
    case PSS_PAGE_SKILLS:
        PokeSum_PrintExpPoints_NextLv();
        break;
    case PSS_PAGE_MOVES_INFO:
        PokeSum_PrintSelectedMoveStats();
        break;
    case PSS_PAGE_MOVES:
        break;
    }

    PutWindowTilemap(sMonSummaryScreen->windowIds[POKESUM_WIN_TRAINER_MEMO]);
}

static void PokeSum_PrintTrainerMemo(void)
{
    if (!sMonSummaryScreen->isEgg)
        PokeSum_PrintTrainerMemo_Mon();
    else
        PokeSum_PrintTrainerMemo_Egg();
}

static void PokeSum_PrintTrainerMemo_Mon(void)
{
    if (PokeSum_BufferOtName_IsEqualToCurrentOwner(&sMonSummaryScreen->currentMon) == TRUE)
        PokeSum_PrintTrainerMemo_Mon_HeldByOT();
    else
        PokeSum_PrintTrainerMemo_Mon_NotHeldByOT();
}

static void PokeSum_PrintTrainerMemo_Egg(void)
{
    u8 metLocation;
    u8 version;
    u8 chosenStrIndex = 0;

    metLocation = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_MET_LOCATION, NULL);

    if (sMonSummaryScreen->monList.mons != gEnemyParty)
    {
        if (metLocation == METLOC_FATEFUL_ENCOUNTER || GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_OBEDIENCE, NULL) == TRUE)
            chosenStrIndex = 4;
        else
        {
            version = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_MET_GAME, NULL);

            if (version != VERSION_LEAF_GREEN && version != VERSION_FIRE_RED)
                chosenStrIndex = 1;
            else if (metLocation == METLOC_SPECIAL_EGG)
                chosenStrIndex = 2;

            if (chosenStrIndex == 0 || chosenStrIndex == 2)
                if (PokeSum_BufferOtName_IsEqualToCurrentOwner(&sMonSummaryScreen->currentMon) == FALSE)
                    chosenStrIndex++;
        }
    }
    else
    {
        if (metLocation == METLOC_FATEFUL_ENCOUNTER || GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_OBEDIENCE, NULL) == TRUE)
            chosenStrIndex = 4;
        else
        {
            version = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_MET_GAME, NULL);

            if (version != VERSION_LEAF_GREEN && version != VERSION_FIRE_RED)
            {
                if (metLocation == METLOC_SPECIAL_EGG)
                    chosenStrIndex = 5;
            }
            else if (metLocation == METLOC_SPECIAL_EGG)
                chosenStrIndex = 2;

            if (PokeSum_BufferOtName_IsEqualToCurrentOwner(&sMonSummaryScreen->currentMon) == FALSE)
                chosenStrIndex++;
        }
    }

    if (sMonSummaryScreen->isBadEgg)
        chosenStrIndex = 0;

    AddTextPrinterParameterized4(sMonSummaryScreen->windowIds[4], 2, 16, 12, 0, -2, sPSSTextColours[DARK], TEXT_SPEED_FF, sEggOriginTexts[chosenStrIndex]);
	AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[4], 2, 6, 0, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_TrainerMemo);
}

static u8 PokeSum_HandleCreateSprites(void)
{
    switch (sMonSummaryScreen->spriteCreationStep)
    {
    case 0:
        CreateShinyStarObj(TAG_PSS_UNK_A0, TAG_PSS_UNK_A0);
        break;
    case 1:
        CreatePokerusIconObj(TAG_PSS_UNK_96, TAG_PSS_UNK_96);
        break;
    case 2:
        PokeSum_CreateMonMarkingsSprite();
        break;
    case 3:
        CreateMoveSelectionCursorObjs(TAG_PSS_UNK_64, TAG_PSS_UNK_64);
        break;
    case 4:
        CreateMonStatusIconObj(TAG_PSS_UNK_6E, TAG_PSS_UNK_6E);
        break;
    case 5:
        CreateHpBarObjs(TAG_PSS_UNK_78, TAG_PSS_UNK_78);
        break;
    case 6:
        CreateExpBarObjs(TAG_PSS_UNK_82, TAG_PSS_UNK_82);
        break;
    case 7:
        CreateBallIconObj();
        break;
    case 8:
        PokeSum_CreateMonIconSprite();
        break;
    default:
        PokeSum_CreateMonPicSprite();
        return TRUE;
    }

    sMonSummaryScreen->spriteCreationStep++;
    return FALSE;
}

static void PokeSum_PrintPageHeaderText(u8 curPageIndex)
{
    switch (curPageIndex)
    {
    case PSS_PAGE_INFO:
        PokeSum_PrintPageName((u8*)0x8419c1d);
        if (!sMonSummaryScreen->isEgg)
            PokeSum_PrintControlsString((u8*)0x8419c62);
        else
            PokeSum_PrintControlsString((u8*)0x8419c72);

        PrintMonLevelNickOnWindow2(gText_PokeSum_NoData);
        break;
    case PSS_PAGE_SKILLS:
        PokeSum_PrintPageName((u8*)0x8419c2a);
        PokeSum_PrintControlsString((u8*)0x8419c7b);
        PrintMonLevelNickOnWindow2(gText_PokeSum_NoData);
        break;
    case PSS_PAGE_MOVES:
        PokeSum_PrintPageName((u8*)0x8419c39);
        PokeSum_PrintControlsString((u8*)0x8419c82);
        PrintMonLevelNickOnWindow2(gText_PokeSum_NoData);
        break;
    case PSS_PAGE_MOVES_INFO:
        PokeSum_PrintPageName((u8*)0x8419c39);
        if (!gMain.inBattle)
            PokeSum_PrintControlsString((u8*)0x8419c92);
        else
            PokeSum_PrintControlsString((u8*)0x8419ca2);
        PrintMonLevelNickOnWindow2(gText_PokeSum_NoData);
        break;
    case PSS_PAGE_MOVE_DELETER:
        PokeSum_PrintPageName((u8*)0x8419c39);
        PokeSum_PrintControlsString((u8*)0x8419ca9);
        PrintMonLevelNickOnWindow2(gText_PokeSum_NoData);
        break;
    default:
        break;
    }
}

static void PokeSum_PrintAbilityDataOrMoveTypes(void)
{
    switch (sMonSummaryScreen->curPageIndex)
    {
    case PSS_PAGE_INFO:
        break;
    case PSS_PAGE_SKILLS:
        PokeSum_PrintAbilityNameAndDesc();
        break;
    case PSS_PAGE_MOVES:
    case PSS_PAGE_MOVES_INFO:
        PokeSum_DrawMoveTypeIcons();
        break;
    }

    PutWindowTilemap(sMonSummaryScreen->windowIds[5]);
}

static void PokeSum_SeekToNextMon(unusedArg u8 taskId, s8 direction)
{
    s8 scrollResult = -1;

    if (sMonSummaryScreen->isBoxMon == TRUE)
    {
        if (sMonSummaryScreen->curPageIndex != PSS_PAGE_INFO)
        {
            if (direction == 1)
                direction = 0;
            else
                direction = 2;
        }
        else
        {
            // Allow Eggs
            if (direction == 1)
                direction = 1;
            else
                direction = 3;
        }

        scrollResult = SeekToNextMonInBox(sMonSummaryScreen->monList.boxMons, GetLastViewedMonIndex(), sMonSummaryScreen->lastIndex, (u8)direction);
    }
    else
    {
        if (IsUpdateLinkStateCBActive() == FALSE
            && gReceivedRemoteLinkPlayers == 1
            && IsMultiBattle() == TRUE)
            scrollResult = SeekToNextMonInMultiParty(direction);
        else
            scrollResult = SeekToNextMonInSingleParty(direction);
    }

    if (scrollResult == -1)
        return;

    sLastViewedMonIndex = scrollResult;
    CreateTask(Task_PokeSum_SwitchDisplayedPokemon, 0);
    sMonSummaryScreen->switchMonTaskState = 0;
}

static void CreateHpBarObjs(u16 tileTag, u16 palTag)
{
    u8 i;
    u8 spriteId;
    void *gfxBufferPtr;
    u32 curHp;
    u32 maxHp;
    u8 hpBarPalTagOffset = 0;

    sHpBarObjs = AllocZeroed(sizeof(struct HpBarObjs));
    gfxBufferPtr = AllocZeroed(0x20 * 12);
    LZ77UnCompWram(gSummaryScreen_HpBar_Gfx, gfxBufferPtr);

    curHp = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_HP, NULL);
    maxHp = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_MAX_HP, NULL);

    if (maxHp / 4 > curHp)
        hpBarPalTagOffset = 2;
    else if (maxHp / 2 > curHp)
        hpBarPalTagOffset = 1;

    if (gfxBufferPtr != NULL)
    {
        struct SpriteSheet sheet = {
            .data = gfxBufferPtr,
            .size = 0x20 * 12,
            .tag = tileTag
        };

        struct SpritePalette palette1 = {.data = sHpBarPals[0], .tag = palTag};
        struct SpritePalette palette2 = {.data = sHpBarPals[1], .tag = palTag + 1};
        struct SpritePalette palette3 = {.data = sHpBarPals[2], .tag = palTag + 2};

        LoadSpriteSheet(&sheet);
        LoadSpritePalette(&palette1);
        LoadSpritePalette(&palette2);
        LoadSpritePalette(&palette3);
    }

    for (i = 0; i < 9; i++)
    {
        struct SpriteTemplate template = {
            .tileTag = tileTag,
            .paletteTag = palTag + hpBarPalTagOffset,
            .oam = &sHpOrExpBarOamData,
            .anims = sHpOrExpBarAnimTable,
            .images = NULL,
            .affineAnims = gDummySpriteAffineAnimTable,
            .callback = SpriteCallbackDummy,
        };

        sHpBarObjs->xpos[i] = i * 8 + 81;
        spriteId = CreateSprite(&template, sHpBarObjs->xpos[i], 33, 0);
        sHpBarObjs->sprites[i] = &gSprites[spriteId];
        sHpBarObjs->sprites[i]->invisible = FALSE;
        sHpBarObjs->sprites[i]->oam.priority = 2;
        sHpBarObjs->tileTag = tileTag;
        sHpBarObjs->palTag = palTag;
        StartSpriteAnim(sHpBarObjs->sprites[i], 8);
    }

    UpdateHpBarObjs();
    ShowOrHideHpBarObjs(TRUE);

    FREE_AND_SET_NULL_IF_SET(gfxBufferPtr);
}

static void CreateShinyStarObj(u16 tileTag, u16 palTag)
{
    u16 spriteId;
    void *gfxBufferPtr;

    sShinyStarObjData = AllocZeroed(sizeof(struct ShinyStarObjData));
    gfxBufferPtr = AllocZeroed(0x20 * 2);

    LZ77UnCompWram(sStarObjTiles, gfxBufferPtr);

    if (sShinyStarObjData != NULL)
    {
        struct SpriteSheet sheet = {
            .data = gfxBufferPtr,
            .size = 0x20 * 2,
            .tag = tileTag
        };

        struct SpritePalette palette = {.data = sStarObjPal, .tag = palTag};
        struct SpriteTemplate template = {
            .tileTag = tileTag,
            .paletteTag = palTag,
            .oam = &sStarObjOamData,
            .anims = sStarObjAnimTable,
            .images = NULL,
            .affineAnims = gDummySpriteAffineAnimTable,
            .callback = SpriteCallbackDummy,
        };

        LoadSpriteSheet(&sheet);
        LoadSpritePalette(&palette);
        spriteId = CreateSprite(&template, 166, 50, 0);
        sShinyStarObjData->sprite = &gSprites[spriteId];
        sShinyStarObjData->tileTag = tileTag;
        sShinyStarObjData->palTag = palTag;
    }

    HideShowShinyStar(TRUE);
    ShowShinyStarObjIfMonShiny();

    FREE_AND_SET_NULL_IF_SET(gfxBufferPtr);
}

static void PokeSum_FlipPages_SlideLayerLeft(void)
{
    if (sMonSummaryScreen->flipPagesBgHofs < 240)
    {
        sMonSummaryScreen->flipPagesBgHofs += 240;
        if (sMonSummaryScreen->flipPagesBgHofs > 240)
            sMonSummaryScreen->flipPagesBgHofs = 240;
    }
}

static void PokeSum_FlipPages_SlideLayeRight(void)
{
    if (sMonSummaryScreen->flipPagesBgHofs >= 240)
    {
        sMonSummaryScreen->flipPagesBgHofs -= 240;
        if (sMonSummaryScreen->flipPagesBgHofs < 0)
            sMonSummaryScreen->flipPagesBgHofs = 0;
    }
}

static void PokeSum_FlipPages_HandleBgHofs(void)
{
    if (sMonSummaryScreen->pageFlipDirection == 1) // Right
    {
        if (sMonSummaryScreen->curPageIndex != PSS_PAGE_MOVES_INFO)
            PokeSum_FlipPages_SlideLayerLeft();
        else
            PokeSum_FlipPages_SlideLayeRight();
    }
    else
    {
        if (sMonSummaryScreen->curPageIndex != PSS_PAGE_MOVES)
            PokeSum_FlipPages_SlideLayeRight();
        else
            PokeSum_FlipPages_SlideLayerLeft();
    }
}

static void VBlankCB_PokemonSummaryScreen(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();

    if (sMonSummaryScreen->flippingPages == FALSE)
        return;

    PokeSum_FlipPages_HandleBgHofs();
}

static void PokeSum_Setup_SetVBlankCallback(void)
{
    SetVBlankCallback(VBlankCB_PokemonSummaryScreen);
}

static bool8 HasOnlyOneMon(void)
{
    if (sMonSummaryScreen->isBoxMon)
    {
        // Lógica para caixa de PC (se aplicável)
        return FALSE; // Ou implemente a verificação adequada para a caixa
    }
    else
    {
        // Para party normal
        u8 count = 0;
        u8 i;
        for (i = 0; i < PARTY_SIZE; i++)
        {
            if (GetMonData(&sMonSummaryScreen->monList.mons[i], MON_DATA_SPECIES, NULL) != SPECIES_NONE)
                count++;
        }
        return (count <= 1);
    }
}

#endif