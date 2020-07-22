#ifndef GUARD_POKEMON_SUMMARY_SCREEN_H
#define GUARD_POKEMON_SUMMARY_SCREEN_H

#include "main.h"

extern const u8* const gMoveDescriptionPointers[];
extern const u8* const gNatureNamePointers[];

void __attribute__((long_call)) ShowSelectMovePokemonSummaryScreen(struct Pokemon *, u8, u8, MainCallback, u16);
u8 __attribute__((long_call)) GetMoveSlotToReplace(void);
void __attribute__((long_call)) SummaryScreen_SetUnknownTaskId(u8 a0);
void __attribute__((long_call)) SummaryScreen_DestroyUnknownTask(void);
u8 __attribute__((long_call)) GetLastViewedMonIndex(void);
void __attribute__((long_call)) ShowPokemonSummaryScreen(struct Pokemon * party, u8 cursorPos, u8 lastIdx, void (*callback)(void), u8 a4);
void __attribute__((long_call)) sub_8138B38(u8);

// The Pokemon Summary Screen can operate in different modes. Certain features,
// such as move re-ordering, are available in the different modes.
enum PokemonSummaryScreenMode
{
    PSS_MODE_NORMAL,
    PSS_MODE_UNK1,
    PSS_MODE_SELECT_MOVE,
    PSS_MODE_FORGET_MOVE,
    PSS_MODE_UNK4,
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
    PSS_STATE3270_0,
    PSS_STATE3270_1,
    PSS_STATE3270_2,
    PSS_STATE3270_3,
    PSS_STATE3270_4,
    PSS_STATE3270_5,
    PSS_STATE3270_6
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
    u16 unk0[0x800];
    u16 unk1000[0x800];
    u16 unk2000[0x800];
    u8 ALIGNED(4) unk3000[7];

    u8 ALIGNED(4) unk3008;
    u8 ALIGNED(4) caughtBallSpriteId;
    u8 ALIGNED(4) unk3010;
    u8 ALIGNED(4) unk3014;

    u8 ALIGNED(4) unk3018;
    u8 ALIGNED(4) unk301C;

    u8 ALIGNED(4) unk3020;

    bool32 isEnemyParty; /* 0x3024 */

    struct PokeSummary
    {
        u8 ALIGNED(4) unk3028[POKEMON_NAME_LENGTH];
        u8 ALIGNED(4) unk3034[POKEMON_NAME_LENGTH + 1];
        u8 ALIGNED(4) unk3040[12];
        u8 ALIGNED(4) unk304C[2][12];

        u8 ALIGNED(4) unk3064[5];
        u8 ALIGNED(4) unk306C[7];
        u8 ALIGNED(4) unk3074[ITEM_NAME_LENGTH + 1];

        u8 ALIGNED(4) unk3084[3];
        u8 ALIGNED(4) unk3088[7];
        u8 ALIGNED(4) unk3090[9];
        u8 ALIGNED(4) unk309C[5][5];

        u8 ALIGNED(4) unk30B8[5][11];
        u8 ALIGNED(4) unk30F0[5][11];
        u8 ALIGNED(4) unk3128[5][MOVE_NAME_LENGTH + 1];
        u8 ALIGNED(4) unk316C[5][5];
        u8 ALIGNED(4) unk3188[5][5];

        u8 ALIGNED(4) unk31A4[9];
        u8 ALIGNED(4) unk31B0[9];

        u8 ALIGNED(4) unk31BC[13];
        u8 ALIGNED(4) unk31CC[52];
    } summary;

    u8 ALIGNED(4) isEgg; /* 0x3200 */
    u8 ALIGNED(4) isBadEgg; /* 0x3204 */

    u8 ALIGNED(4) mode; /* 0x3208 */
    u8 ALIGNED(4) unk320C; /* 0x320C */
    u8 ALIGNED(4) lastIndex; /* 0x3210 */
    u8 ALIGNED(4) curPageIndex; /* 0x3214 */
    u8 ALIGNED(4) unk3218; /* 0x3218 */
    u8 ALIGNED(4) isBoxMon; /* 0x321C */
    u8 ALIGNED(4) unk3220[2]; /* 0x3220 */

    u8 ALIGNED(4) unk3224; /* 0x3224 */
    u8 ALIGNED(4) unk3228; /* 0x3228 */
    u8 ALIGNED(4) unk322C; /* 0x322C */
    u8 ALIGNED(4) unk3230; /* 0x3230 */

    u8 ALIGNED(4) lockMovesFlag; /* 0x3234 */

    u8 ALIGNED(4) unk3238; /* 0x3238 */
    u8 ALIGNED(4) unk323C; /* 0x323C */
    u8 ALIGNED(4) unk3240; /* 0x3240 */
    u8 ALIGNED(4) unk3244; /* 0x3244 */
    u8 ALIGNED(4) unk3248; /* 0x3248 */
    s16 ALIGNED(4) unk324C; /* 0x324C */

    u16 unk3250[5]; /* 0x3250 */
    u16 unk325A[5]; /* 0x325A */
    u8 ALIGNED(4) unk3264; /* 0x3264 */
    u8 ALIGNED(4) unk3268; /* 0x3268 */

    u8 ALIGNED(4) unk326C; /* 0x326C */

    u8 ALIGNED(4) state3270; /* 0x3270 */
    u8 ALIGNED(4) unk3274; /* 0x3274 */
    u8 ALIGNED(4) unk3278; /* 0x3278 */
    u8 ALIGNED(4) unk327C; /* 0x327C */
    u8 ALIGNED(4) unk3280; /* 0x3280 */
    u8 ALIGNED(4) unk3284; /* 0x3284 */
    u8 ALIGNED(4) unk3288; /* 0x3288 */
    u8 ALIGNED(4) unk328C; /* 0x328C */

    struct Pokemon currentMon; /* 0x3290 */

    union
    {
        struct Pokemon * mons;
        struct BoxPokemon * boxMons;
    } monList;

    MainCallback savedCallback;
    struct Sprite * markingSprite;

    u8 ALIGNED(4) unk3300[2]; /* 0x3300 */
    u8 ALIGNED(4) unk3304[3]; /* 0x3304 */
};

extern struct PokemonSummaryScreenData* sMonSummaryScreen;

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
