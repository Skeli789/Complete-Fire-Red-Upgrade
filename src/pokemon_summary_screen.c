#include "defines.h"
#include "../include/bike.h"
#include "../include/field_player_avatar.h"
#include "../include/fieldmap.h"
#include "../include/field_message_box.h"
#include "../include/field_weather.h"
#include "../include/gpu_regs.h"
#include "../include/item_icon.h"
#include "../include/item_menu.h"
#include "../include/list_menu.h"
#include "../include/map_name_popup.h"
#include "../include/menu.h"
#include "../include/m4a.h"
#include "../include/naming_screen.h"
#include "../include/overworld.h"
#include "../include/pokemon_storage_system.h"
#include "../include/region_map.h"
#include "../include/script.h"
#include "../include/script_menu.h"
#include "../include/sound.h"
#include "../include/sprite.h"
#include "../include/string_util.h"
#include "../include/text.h"
#include "../include/wild_encounter.h"
#include "../include/window.h"
#include "../include/event_data.h"
#include "../include/constants/abilities.h"
#include "../include/constants/items.h"
#include "../include/constants/moves.h"
#include "../include/constants/pokedex.h"
#include "../include/constants/pokemon.h"
#include "../include/constants/songs.h"

#include "../include/new/battle_strings.h"
#include "../include/new/build_pokemon.h"
#include "../include/new/catching.h"
#include "../include/new/damage_calc.h"
#include "../include/new/dns.h"
#include "../include/new/util.h"
#include "../include/new/item.h"
#include "../include/new/learn_move.h"
#include "../include/new_menu_helpers.h"
#include "../include/new/multi.h"
#include "../include/new/overworld.h"
#include "../include/new/pokemon_storage_system.h"
#include "../include/new/ram_locs_battle.h"
#include "../include/new/read_keys.h"
#include "../include/new/roamer.h"
#include "../include/new/text.h"
#include "../include/new/scrolling_multichoice.h"
#include "../include/new/Vanilla_functions_battle.h"
#include "../include/new/wild_encounter.h"

// Summary screen printing tera types!
extern const u8 gText_TeraType[];
#define POKESUM_WIN_RIGHT_PANE 3
#define TEXT_SKIP_DRAW 0xFF
extern const u8 gTypeNames[][TYPE_NAME_LENGTH + 1];

enum PokemonSummaryScreenStat
{
    PSS_STAT_ATK,
    PSS_STAT_DEF,
    PSS_STAT_SPA,
    PSS_STAT_SPD,
    PSS_STAT_SPE
};

enum {
    FONT_SMALL,
    FONT_NORMAL_COPY_1,
    FONT_NORMAL,
    FONT_NORMAL_COPY_2,
    FONT_MALE,
    FONT_FEMALE,
    FONT_BRAILLE,
    FONT_BOLD,
};
#define ALIGNED(n) __attribute__((aligned(n)))
#define ITEM_NAME_LENGTH 14
#define MOVE_NAME_LENGTH 12
#define POKEMON_NAME_LENGTH 10
struct PokemonSummaryScreenData
{
    u16 bg1TilemapBuffer[0x800];
    u16 bg2TilemapBuffer[0x800];
    u16 bg3TilemapBuffer[0x800];
    u8 ALIGNED(4) windowIds[7];

    u8 ALIGNED(4) unk3008;
    u8 ALIGNED(4) ballIconSpriteId;
    u8 ALIGNED(4) monPicSpriteId;
    u8 ALIGNED(4) monIconSpriteId;

    u8 ALIGNED(4) inputHandlerTaskId;
    u8 ALIGNED(4) inhibitPageFlipInput;

    u8 ALIGNED(4) numMonPicBounces;

    bool32 isEnemyParty; /* 0x3024 */

    struct PokeSummary
    {
        u8 ALIGNED(4) speciesNameStrBuf[POKEMON_NAME_LENGTH];
        u8 ALIGNED(4) nicknameStrBuf[POKEMON_NAME_LENGTH + 1];
        u8 ALIGNED(4) otNameStrBuf[12];
        u8 ALIGNED(4) otNameStrBufs[2][12];

        u8 ALIGNED(4) dexNumStrBuf[5];
        u8 ALIGNED(4) unk306C[7];
        u8 ALIGNED(4) itemNameStrBuf[ITEM_NAME_LENGTH + 1];

        u8 ALIGNED(4) genderSymbolStrBuf[3];
        u8 ALIGNED(4) levelStrBuf[7];
        u8 ALIGNED(4) curHpStrBuf[9];
        u8 ALIGNED(4) statValueStrBufs[5][5];

        u8 ALIGNED(4) moveCurPpStrBufs[5][11];
        u8 ALIGNED(4) moveMaxPpStrBufs[5][11];
        u8 ALIGNED(4) moveNameStrBufs[5][MOVE_NAME_LENGTH + 1];
        u8 ALIGNED(4) movePowerStrBufs[5][5];
        u8 ALIGNED(4) moveAccuracyStrBufs[5][5];

        u8 ALIGNED(4) expPointsStrBuf[9];
        u8 ALIGNED(4) expToNextLevelStrBuf[9];

        u8 ALIGNED(4) abilityNameStrBuf[13];
        u8 ALIGNED(4) abilityDescStrBuf[52];
    } summary;

    u8 ALIGNED(4) isEgg; /* 0x3200 */
    u8 ALIGNED(4) isBadEgg; /* 0x3204 */

    u8 ALIGNED(4) mode; /* 0x3208 */
    u8 ALIGNED(4) unk320C; /* 0x320C */
    u8 ALIGNED(4) lastIndex; /* 0x3210 */
    u8 ALIGNED(4) curPageIndex; /* 0x3214 */
    u8 ALIGNED(4) unk3218; /* 0x3218 */
    u8 ALIGNED(4) isBoxMon; /* 0x321C */
    u8 ALIGNED(4) monTypes[2]; /* 0x3220 */

    u8 ALIGNED(4) pageFlipDirection; /* 0x3224 */
    u8 ALIGNED(4) unk3228; /* 0x3228 */
    u8 ALIGNED(4) unk322C; /* 0x322C */
    u8 ALIGNED(4) unk3230; /* 0x3230 */

    u8 ALIGNED(4) lockMovesFlag; /* 0x3234 */

    u8 ALIGNED(4) whichBgLayerToTranslate; /* 0x3238 */
    u8 ALIGNED(4) skillsPageBgNum; /* 0x323C */
    u8 ALIGNED(4) infoAndMovesPageBgNum; /* 0x3240 */
    u8 ALIGNED(4) flippingPages; /* 0x3244 */
    u8 ALIGNED(4) unk3248; /* 0x3248 */
    s16 ALIGNED(4) flipPagesBgHofs; /* 0x324C */

    u16 moveTypes[5]; /* 0x3250 */
    u16 moveIds[5]; /* 0x325A */
    u8 ALIGNED(4) numMoves; /* 0x3264 */
    u8 ALIGNED(4) isSwappingMoves; /* 0x3268 */

    u8 ALIGNED(4) curMonStatusAilment; /* 0x326C */

    u8 ALIGNED(4) state3270; /* 0x3270 */
    u8 ALIGNED(4) summarySetupStep; /* 0x3274 */
    u8 ALIGNED(4) loadBgGfxStep; /* 0x3278 */
    u8 ALIGNED(4) spriteCreationStep; /* 0x327C */
    u8 ALIGNED(4) bufferStringsStep; /* 0x3280 */
    u8 ALIGNED(4) state3284; /* 0x3284 */
    u8 ALIGNED(4) selectMoveInputHandlerState; /* 0x3288 */
    u8 ALIGNED(4) switchMonTaskState; /* 0x328C */

    struct Pokemon currentMon; /* 0x3290 */

    union
    {
        struct Pokemon * mons;
        struct BoxPokemon * boxMons;
    } monList;

    MainCallback savedCallback;
    struct Sprite *markingSprite;

    u8 ALIGNED(4) lastPageFlipDirection; /* 0x3300 */
    u8 ALIGNED(4) unk3304; /* 0x3304 */
};
static const u8 __attribute__((unused)) sLevelNickTextColors[][3] =
{
    {0, 14, 10},
    {0, 1, 2},
    {0, 9, 8},
    {0, 5, 4},
    {0, 2, 3},
    {0, 11, 10},
};
extern const u8 *const sEggHatchTimeTexts[];
#define sMonSummaryScreen (*(struct PokemonSummaryScreenData **)0x0203B140)
#define sMonSkillsPrinterXpos (*(struct Struct203B144 **)0x0203B144)

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

#if (defined SHOW_TERA_TYPE_ICON_ON_SUMMARY_SCREEN && !defined BW_SUMMARY_SCREEN)
void PrintInfoPage(void)
{
    AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[POKESUM_WIN_RIGHT_PANE], FONT_NORMAL, 47, 19, sLevelNickTextColors[0], TEXT_SKIP_DRAW, sMonSummaryScreen->summary.speciesNameStrBuf);

    if (!sMonSummaryScreen->isEgg)
    {
        AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[POKESUM_WIN_RIGHT_PANE], FONT_NORMAL, 47 + sMonSkillsPrinterXpos->unk00, 5, sLevelNickTextColors[0], TEXT_SKIP_DRAW, sMonSummaryScreen->summary.dexNumStrBuf);
        AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[POKESUM_WIN_RIGHT_PANE], FONT_NORMAL, 47, 49, sLevelNickTextColors[0], TEXT_SKIP_DRAW, sMonSummaryScreen->summary.otNameStrBuf);
        AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[POKESUM_WIN_RIGHT_PANE], FONT_SMALL, 47, 63, sLevelNickTextColors[0], TEXT_SKIP_DRAW, sMonSummaryScreen->summary.unk306C);
        AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[POKESUM_WIN_RIGHT_PANE], FONT_SMALL, 47, 75, sLevelNickTextColors[0], TEXT_SKIP_DRAW, sMonSummaryScreen->summary.itemNameStrBuf);
				// === Print Tera Type ===
		{
			u8 teraType = sMonSummaryScreen->currentMon.teraType;

			if (teraType < NUMBER_OF_MON_TYPES)
			{
				AddTextPrinterParameterized3(
					sMonSummaryScreen->windowIds[POKESUM_WIN_RIGHT_PANE],
					FONT_SMALL,
					40, 84,  // Adjust X, Y as needed to fit below Item
					sLevelNickTextColors[0],
					TEXT_SKIP_DRAW,
					gText_TeraType);

				AddTextPrinterParameterized3(
					sMonSummaryScreen->windowIds[POKESUM_WIN_RIGHT_PANE],
					FONT_SMALL,
					90, 84, // right of label
					sLevelNickTextColors[0],
					TEXT_SKIP_DRAW,
					gTypeNames[teraType]);
			}
		}

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

        AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[POKESUM_WIN_RIGHT_PANE], FONT_NORMAL, 7, 45, sLevelNickTextColors[0], TEXT_SKIP_DRAW, sEggHatchTimeTexts[hatchMsgIndex]);
    }
}
#endif

#if (defined NATURE_COLORS_ON_SUMMARY_SCREEN && !defined BW_SUMMARY_SCREEN)
static const u8 sNatureTextColors[][3] =
{
    {0, 14, 10},
    {0, 1, 10},
    {0, 7, 10},
};

extern const s8 sNatureStatTable[][5];

u8 GetNatureTextColor(s8 natureMod)
{
    if (natureMod == 0)
        return 0;
    else if (natureMod > 0)
        return 1;
    else
        return 2;
}

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

#define HP_TEXT_X_POS 14
#define STATS_TEXT_POS 50
#define EXP_TEXT_X_POS1 15
#define EXP_TEXT_X_POS2 15
#define HP_TEXT_Y_POS 4
#define ATK_TEXT_Y_POS 22
#define DEF_TEXT_Y_POS 35
#define SPATK_TEXT_Y_POS 48
#define SPDEF_TEXT_Y_POS 61
#define SPD_TEXT_Y_POS 74
#define EXP1_TEXT_Y_POS 87
#define EXP2_TEXT_Y_POS 100
#define STATS_IV_POS_X 15
#define HP_IV_POS_Y 6
#define ATK_IV_POS_Y 23
#define DEF_IV_POS_Y 36
#define SPATK_IV_POS_Y 49
#define SPDEF_IV_POS_Y 62
#define SPD_IV_POS_Y 75

void PrintSkillsPage(void)
{
    const s8 *natureMod = sNatureStatTable[GetNature(&sMonSummaryScreen->currentMon)];

	u8 hpIv, atkIv, defIv, spAtkIv, spDefIv, spdIv;
    hpIv = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_HP_IV, NULL);
    atkIv = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_ATK_IV, NULL);
    defIv = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_DEF_IV, NULL);
    spAtkIv = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_SPATK_IV, NULL);
    spDefIv = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_SPDEF_IV, NULL);
    spdIv = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_SPEED_IV, NULL);

    AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[POKESUM_WIN_RIGHT_PANE], FONT_NORMAL, HP_TEXT_X_POS + sMonSkillsPrinterXpos->curHpStr, HP_TEXT_Y_POS, sLevelNickTextColors[0], TEXT_SKIP_DRAW, sMonSummaryScreen->summary.curHpStrBuf);
    AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[POKESUM_WIN_RIGHT_PANE], FONT_NORMAL, STATS_TEXT_POS + sMonSkillsPrinterXpos->atkStr, ATK_TEXT_Y_POS, sNatureTextColors[GetNatureTextColor(natureMod[STAT_ATK - 1])], TEXT_SKIP_DRAW, sMonSummaryScreen->summary.statValueStrBufs[PSS_STAT_ATK]);
    AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[POKESUM_WIN_RIGHT_PANE], FONT_NORMAL, STATS_TEXT_POS + sMonSkillsPrinterXpos->defStr, DEF_TEXT_Y_POS, sNatureTextColors[GetNatureTextColor(natureMod[STAT_DEF - 1])], TEXT_SKIP_DRAW, sMonSummaryScreen->summary.statValueStrBufs[PSS_STAT_DEF]);
    AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[POKESUM_WIN_RIGHT_PANE], FONT_NORMAL, STATS_TEXT_POS + sMonSkillsPrinterXpos->spAStr, SPATK_TEXT_Y_POS, sNatureTextColors[GetNatureTextColor(natureMod[STAT_SPATK - 1])], TEXT_SKIP_DRAW, sMonSummaryScreen->summary.statValueStrBufs[PSS_STAT_SPA]);
    AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[POKESUM_WIN_RIGHT_PANE], FONT_NORMAL, STATS_TEXT_POS + sMonSkillsPrinterXpos->spDStr, SPDEF_TEXT_Y_POS, sNatureTextColors[GetNatureTextColor(natureMod[STAT_SPDEF - 1])], TEXT_SKIP_DRAW, sMonSummaryScreen->summary.statValueStrBufs[PSS_STAT_SPD]);
    AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[POKESUM_WIN_RIGHT_PANE], FONT_NORMAL, STATS_TEXT_POS + sMonSkillsPrinterXpos->speStr, SPD_TEXT_Y_POS, sNatureTextColors[GetNatureTextColor(natureMod[STAT_SPEED - 1])], TEXT_SKIP_DRAW, sMonSummaryScreen->summary.statValueStrBufs[PSS_STAT_SPE]);
    AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[POKESUM_WIN_RIGHT_PANE], FONT_NORMAL, EXP_TEXT_X_POS1 + sMonSkillsPrinterXpos->expStr, EXP1_TEXT_Y_POS, sLevelNickTextColors[0], TEXT_SKIP_DRAW, sMonSummaryScreen->summary.expPointsStrBuf);
    AddTextPrinterParameterized3(sMonSummaryScreen->windowIds[POKESUM_WIN_RIGHT_PANE], FONT_NORMAL, EXP_TEXT_X_POS2 + sMonSkillsPrinterXpos->toNextLevel, EXP2_TEXT_Y_POS, sLevelNickTextColors[0], TEXT_SKIP_DRAW, sMonSummaryScreen->summary.expToNextLevelStrBuf);

	BlitBitmapRectToWindow(sMonSummaryScreen->windowIds[POKESUM_WIN_RIGHT_PANE], sIVRankings_Images[hpIv], 0, 0, 8, 8, STATS_IV_POS_X , HP_IV_POS_Y , 16, 8);
    BlitBitmapRectToWindow(sMonSummaryScreen->windowIds[POKESUM_WIN_RIGHT_PANE], sIVRankings_Images[atkIv], 0, 0, 8, 8, STATS_IV_POS_X , ATK_IV_POS_Y , 16, 8);
    BlitBitmapRectToWindow(sMonSummaryScreen->windowIds[POKESUM_WIN_RIGHT_PANE], sIVRankings_Images[defIv], 0, 0, 8, 8, STATS_IV_POS_X , DEF_IV_POS_Y , 16, 8);
    BlitBitmapRectToWindow(sMonSummaryScreen->windowIds[POKESUM_WIN_RIGHT_PANE], sIVRankings_Images[spAtkIv], 0, 0, 8, 8, STATS_IV_POS_X , SPATK_IV_POS_Y , 16, 8);
    BlitBitmapRectToWindow(sMonSummaryScreen->windowIds[POKESUM_WIN_RIGHT_PANE], sIVRankings_Images[spDefIv], 0, 0, 8, 8, STATS_IV_POS_X , SPDEF_IV_POS_Y , 16, 8);
    BlitBitmapRectToWindow(sMonSummaryScreen->windowIds[POKESUM_WIN_RIGHT_PANE], sIVRankings_Images[spdIv], 0, 0, 8, 8, STATS_IV_POS_X , SPD_IV_POS_Y , 16, 8);
}
#endif