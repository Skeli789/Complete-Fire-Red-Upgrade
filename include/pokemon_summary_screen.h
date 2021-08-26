#ifndef GUARD_POKEMON_SUMMARY_SCREEN_H
#define GUARD_POKEMON_SUMMARY_SCREEN_H

#include "constants/moves.h"

extern const u8* const gMoveDescriptionPointers[];
extern const u8* const gNatureNamePointers[];

void __attribute__((long_call)) ShowSelectMovePokemonSummaryScreen(struct Pokemon *, u8, u8, MainCallback, u16);
u8 __attribute__((long_call)) GetMoveSlotToReplace(void);
void __attribute__((long_call)) SummaryScreen_SetUnknownTaskId(u8 a0);
void __attribute__((long_call)) SummaryScreen_DestroyUnknownTask(void);
u8 __attribute__((long_call)) GetLastViewedMonIndex(void);
void __attribute__((long_call)) ShowPokemonSummaryScreen(struct Pokemon * party, u8 cursorPos, u8 lastIdx, void (*callback)(void), u8 a4);
void __attribute__((long_call)) SetPokemonSummaryScreenMode(u8);

// The Pokemon Summary Screen can operate in different modes. Certain features,
// such as move re-ordering, are available in the different modes.
enum PokemonSummaryScreenMode
{
    PSS_MODE_NORMAL,
    PSS_MODE_UNK1,
    PSS_MODE_SELECT_MOVE,
    PSS_MODE_FORGET_MOVE,
    PSS_MODE_TRADE,
    PSS_MODE_BOX,
};

enum PokemonSummaryScreenPage
{
    PSS_PAGE_INFO,
    PSS_PAGE_SKILLS,
    PSS_PAGE_MOVES,
    PSS_PAGE_MOVES_INFO,
    PSS_PAGE_UNK4,
    PSS_PAGE_MOVE_DELETER,
};

enum PokemonSummaryScreenState3270
{
    PSS_STATE3270_FADEIN,
    PSS_STATE3270_PLAYCRY,
    PSS_STATE3270_HANDLEINPUT,
    PSS_STATE3270_FLIPPAGES,
    PSS_STATE3270_ATEXIT_FADEOUT,
    PSS_STATE3270_ATEXIT_WAITLINKDELAY,
    PSS_STATE3270_ATEXIT_WAITFADE
};

enum PokemonSummaryScreenStat
{
    PSS_STAT_ATK,
    PSS_STAT_DEF,
    PSS_STAT_SPA,
    PSS_STAT_SPD,
    PSS_STAT_SPE
};

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
    struct Sprite * markingSprite;

    u8 ALIGNED(4) lastPageFlipDirection; /* 0x3300 */
    u8 ALIGNED(4) unk3304; /* 0x3304 */
};

#define sMonSummaryScreen ((struct PokemonSummaryScreenData*) *((u32*) 0x203B140))

#define TAG_PSS_UNK_64 0x64
#define TAG_PSS_UNK_65 0x65
#define TAG_PSS_UNK_66 0x66
#define TAG_PSS_UNK_67 0x67
#define TAG_PSS_UNK_6E 0x6E
#define TAG_PSS_UNK_78 0x78
#define TAG_PSS_UNK_82 0x82
#define TAG_PSS_UNK_8C 0x8C
#define TAG_PSS_UNK_96 0x96
#define TAG_PSS_UNK_A0 0xA0

#endif // GUARD_POKEMON_SUMMARY_SCREEN_H
