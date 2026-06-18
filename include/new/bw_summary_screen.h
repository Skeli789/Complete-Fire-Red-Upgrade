#include "../global.h"

void __attribute__((long_call)) BlitMenuInfoIcon(u8 windowId, u8 iconId, u16 x, u16 y);
void __attribute__((long_call)) BufferSelectedMonData(struct Pokemon * mon);
void __attribute__((long_call)) SetHelpContext(u8);
void __attribute__((long_call)) PokeSum_Setup_ResetCallbacks(void);
void __attribute__((long_call)) PokeSum_Setup_SpritesReset(void);
void __attribute__((long_call)) ListMenuLoadStdPalAt(u8 palOffset, u8 palId);
u8  __attribute__((long_call)) PokeSum_Setup_BufferStrings(void);
void __attribute__((long_call)) ShowOrHideHpBarObjs(u8 invisible);
void __attribute__((long_call)) ShowOrHideExpBarObjs(u8 invisible);
void __attribute__((long_call)) CommitStaticWindowTilemaps(void);
void __attribute__((long_call)) PokeSum_ShowOrHideMonIconSprite(bool8 invisible);
void __attribute__((long_call)) ShoworHideMoveSelectionCursor(bool8 invisible);
void __attribute__((long_call)) PokeSum_ShowOrHideMonPicSprite(u8 invisible);
void __attribute__((long_call)) PokeSum_ShowOrHideMonMarkingsSprite(u8 invisible);
void __attribute__((long_call)) ShowOrHideBallIconObj(u8 invisible);
void __attribute__((long_call)) ShowOrHideStatusIcon(u8 invisible);
void __attribute__((long_call)) HideShowPokerusIcon(bool8 invisible);
u16 __attribute__((long_call)) GetMonPpByMoveSlot(struct Pokemon * mon, u8 moveSlot);
void __attribute__((long_call)) PokeSum_SetHelpContext(void);
void __attribute__((long_call)) PokeSum_TryPlayMonCry(void);
bool8 __attribute__((long_call)) IsActiveOverworldLinkBusy(void);
bool32 __attribute__((long_call)) IsLinkRecvQueueAtOverworldMax(void);
void __attribute__((long_call)) PokeSum_DestroyMonPicSprite(void);
void __attribute__((long_call)) PokeSum_DestroyMonIconSprite(void);
void __attribute__((long_call)) DestroyBallIconObj(void);
void __attribute__((long_call)) BufferMonSkills(void);
bool32 __attribute__((long_call)) Overworld_LinkRecvQueueLengthMoreThan2(void);
bool32 __attribute__((long_call)) IsPageFlipInput(u8 direction);
void __attribute__((long_call)) Task_DestroyResourcesOnExit(u8 taskId);
void __attribute__((long_call)) PokeSum_RemoveWindows(u8 curPageIndex);
void __attribute__((long_call)) SwapMonMoveSlots(void);
void __attribute__((long_call)) SwapBoxMonMoveSlots(void);
void __attribute__((long_call)) UpdateCurrentMonBufferFromPartyOrBox(struct Pokemon * mon);
void __attribute__((long_call)) BufferMonMoves(void);
void __attribute__((long_call)) BufferMonInfo(void);
void __attribute__((long_call)) UpdateHpBarObjs(void);
void __attribute__((long_call)) PokeSum_UpdateMonMarkingsAnim(void);
void __attribute__((long_call)) UpdateMonStatusIconObj(void);
void __attribute__((long_call)) ShowPokerusIconObjIfHasOrHadPokerus(void);
void __attribute__((long_call)) ShowShinyStarObjIfMonShiny(void);
void __attribute__((long_call)) CreateBallIconObj(void);
void __attribute__((long_call)) UpdateExpBarObjs(void);
bool32 __attribute__((long_call)) MapSecIsInKantoOrSevii(u8 mapSec);
u8 __attribute__((long_call)) *GetMapNameGeneric_(u8 *dest, u16 mapsec);
bool32 __attribute__((long_call)) IsMultiBattlePartner(void);
bool32 __attribute__((long_call)) PokeSum_IsMonBoldOrGentle(u8 nature);
bool32 __attribute__((long_call)) CurrentMonIsFromGBA(void);
bool8 __attribute__((long_call)) IsMonSpriteNotFlipped(u16 species);
bool8 __attribute__((long_call)) ShouldIgnoreDeoxysForm(u8 caseId, u8 battlerId);
void __attribute__((long_call)) PokeSum_SetMonPicSpriteCallback(u16 spriteId);
void __attribute__((long_call)) SpriteCB_MoveSelectionCursor(struct Sprite *sprite);
u8 __attribute__((long_call)) PokeSum_CanForgetSelectedMove(void);
void __attribute__((long_call)) CB2_RunPokemonSummaryScreen(void);
u8 __attribute__((long_call)) PokeSum_BufferOtName_IsEqualToCurrentOwner(struct Pokemon * mon);;
void __attribute__((long_call)) CreatePokerusIconObj(u16 tileTag, u16 palTag);
void __attribute__((long_call)) PokeSum_CreateMonMarkingsSprite(void);
void __attribute__((long_call)) CreateMonStatusIconObj(u16 tileTag, u16 palTag);
void __attribute__((long_call)) CreateExpBarObjs(u16 tileTag, u16 palTag);
s16 __attribute__((long_call)) SeekToNextMonInBox(struct BoxPokemon * boxMons, u8 curIndex, u8 maxIndex, u8 flags);
s8 __attribute__((long_call)) SeekToNextMonInMultiParty(s8 direction);
s8 __attribute__((long_call)) SeekToNextMonInSingleParty(s8 direction);
u16 __attribute__((long_call)) CreateMonPicSprite(u16 species, u32 otId, u32 personality, bool8 isFrontPic, s16 x, s16 y, u8 paletteSlot, u16 paletteTag, bool8 ignoreDeoxys);
bool8 IsMultiBattle(void);
void TransferPlttBuffer(void);

enum
{
	DARK,
	WHITE_TITLE,
	BLUE_SHADOW,
	WHITE,
	RED_SHADOW,
	BLUE,
	RED,
	ORANGE,
	RED_2,
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

struct ShinyStarObjData
{
    struct Sprite *sprite; /* 0x00 */
    u16 tileTag; /* 0x04 */
    u16 palTag; /* 0x06 */
};

struct MonPicBounceState
{
    u8 ALIGNED(4) animFrame; /* 0x00 */
    u8 ALIGNED(4) initDelay; /* 0x04 */
    u8 ALIGNED(4) vigor; /* 0x08 */
};

struct MoveSelectionCursor
{
    struct Sprite * sprite; /* 0x00 */
    u16 tileTag; /* 0x04 */
    u16 palTag; /* 0x06 */
    u16 unk08; /* 0x08 */
};

struct HpBarObjs
{
    struct Sprite *sprites[10]; /* 0x00 */
    u16 xpos[10]; /* 0x28 */
    u16 tileTag; /* 0x3c */
    u16 palTag; /* 0x3e */
};

#define POKESUM_WIN_RIGHT_PANE       3
#define TEXT_SKIP_DRAW               0xFF
#define HELPCONTEXT_POKEMON_INFO     6
#define HELPCONTEXT_POKEMON_MOVES    8
#define METLOC_FATEFUL_ENCOUNTER     0xFF
#define POKESUM_WIN_TRAINER_MEMO     4
#define DEOXYS_CHECK_TRADE_MAIN      3
#define METLOC_SPECIAL_EGG           0xFD

#define MACRO_8137270(x) ((x) * 28 + 5)
#define MACRO_81372E4(x) ((x) * 28 + 16)

#define FREE_AND_SET_NULL_IF_SET(ptr) \
{                                     \
    if (ptr != NULL)                  \
    {                                 \
        free(ptr);                    \
        (ptr) = NULL;                 \
    }                                 \
}

#define HP_TEXT_X_POS 39
#define STATS_TEXT_POS 50
#define EXP_TEXT_X_POS1 0
#define EXP_TEXT_X_POS2 36
#define HP_TEXT_Y_POS 0
#define ATK_TEXT_Y_POS 20
#define DEF_TEXT_Y_POS 32
#define SPATK_TEXT_Y_POS 44
#define SPDEF_TEXT_Y_POS 56
#define SPD_TEXT_Y_POS 68
#define EXP1_TEXT_Y_POS 92
#define EXP2_TEXT_Y_POS 92
#define STATS_IV_POS_X 76
#define HP_IV_POS_Y 2
#define ATK_IV_POS_Y 22
#define DEF_IV_POS_Y 34
#define SPATK_IV_POS_Y 46
#define SPDEF_IV_POS_Y 58
#define SPD_IV_POS_Y 70