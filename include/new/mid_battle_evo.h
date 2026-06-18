#include "../global.h"
#include "../battle_string_ids.h"

void __attribute__((long_call)) InitBattleBgsVideo(void);
void __attribute__((long_call)) LoadBattleTextboxAndBackground(void);
void __attribute__((long_call)) SetUpBattleVars(void);
void __attribute__((long_call)) CB2_HandleStartMultiBattle(void);
void __attribute__((long_call)) CB2_HandleStartBattle(void);
void __attribute__((long_call)) ReshowBattleScreenAfterMenu(void);
void __attribute__((long_call)) HandleTurnActionSelectionState(void);
u8 __attribute__((long_call)) CreateNPCTrainerParty(struct Pokemon *party, u16 trainerNum);
void __attribute__((long_call)) AllocateMonSpritesGfx(void);
u8 __attribute__((long_call)) *StringCopy_Nickname(u8 *dest, const u8 *src);
void __attribute__((long_call)) SpriteCallbackDummy_2(struct Sprite *sprite);
void __attribute__((long_call)) LoadEvoSparkleSpriteAndPal(void);
void  __attribute__((long_call)) VBlankCB_EvolutionScene(void);
void __attribute__((long_call)) HelpSystem_Disable(void);
bool32 __attribute__((long_call)) IsNationalPokedexEnabled(void);
void __attribute__((long_call)) StopBgAnimation(void);
void __attribute__((long_call)) StartBgAnimation(bool8 isLink);
u8 __attribute__((long_call)) EvolutionSparkles_SpiralUpward(u16 a0);
u8 __attribute__((long_call)) EvolutionSparkles_ArcDown(void);
u8 __attribute__((long_call)) CycleEvolutionMonSprite(u8 preEvoSpriteId, u8 postEvoSpriteId);
u8 __attribute__((long_call)) EvolutionSparkles_CircleInward(void);
u8 __attribute__((long_call)) EvolutionSparkles_SprayAndFlash(u16 species);
void __attribute__((long_call)) RestoreBgAfterAnim(void);
void __attribute__((long_call)) EvolutionRenameMon(struct Pokemon *mon, u16 oldSpecies, u16 newSpecies);
void __attribute__((long_call)) HelpSystem_Enable(void);
void __attribute__((long_call)) PlayBattleBGM(void);
u16 __attribute__((long_call)) GiveMoveToBattleMon(struct BattlePokemon *mon, u16 move);
void CreateShedinja(u16 preEvoSpecies, struct Pokemon* mon);
void __attribute__((long_call)) FreeMonSpritesGfx(void);
void __attribute__((long_call)) BattleCreateYesNoCursorAt2(void);
void __attribute__((long_call)) BattleDestroyYesNoCursorAt2(void);
bool32 __attribute__((long_call)) IsHMMove2(u16 move);
void __attribute__((long_call)) RemoveBattleMonPPBonus(struct BattlePokemon *mon, u8 moveIndex);
void __attribute__((long_call)) SetBattleMonMoveSlot(struct BattlePokemon *mon, u16 move, u8 slot);
u8 __attribute__((long_call)) GetAbilityBySpecies(u16 species, bool8 altAbility);
u8 __attribute__((long_call)) GetPartyIdFromBattlePartyId(u8 battlePartyId);
void VBlankCB_Battle(void);
void ClearTemporarySpeciesSpriteData(u8 bank, bool8 dontClearSubstitute);
void __attribute__((long_call)) DecompressPicFromTable(const struct CompressedSpriteSheet *src, void *buffer, s32 species);
void BuildTrainerPartySetup(void);

struct SpeciesInfo
{
 /* 0x00 */ u8 baseHP;
 /* 0x01 */ u8 baseAttack;
 /* 0x02 */ u8 baseDefense;
 /* 0x03 */ u8 baseSpeed;
 /* 0x04 */ u8 baseSpAttack;
 /* 0x05 */ u8 baseSpDefense;
 /* 0x06 */ u8 types[2];
 /* 0x08 */ u8 catchRate;
 /* 0x09 */ u8 expYield;
 /* 0x0A */ u16 evYield_HP:2;
 /* 0x0A */ u16 evYield_Attack:2;
 /* 0x0A */ u16 evYield_Defense:2;
 /* 0x0A */ u16 evYield_Speed:2;
 /* 0x0B */ u16 evYield_SpAttack:2;
 /* 0x0B */ u16 evYield_SpDefense:2;
 /* 0x0C */ u16 itemCommon;
 /* 0x0E */ u16 itemRare;
 /* 0x10 */ u8 genderRatio;
 /* 0x11 */ u8 eggCycles;
 /* 0x12 */ u8 friendship;
 /* 0x13 */ u8 growthRate;
 /* 0x14 */ u8 eggGroups[2];
 /* 0x16 */ u8 abilities[2];
 /* 0x18 */ u8 safariZoneFleeRate;
 /* 0x19 */ u8 bodyColor : 7;
            u8 noFlip : 1;
};

extern u8 gBattleTerrainBackup;
extern const struct SpeciesInfo gSpeciesInfo[];
extern const union AffineAnimCmd * const sDummySpriteAffineAnimTable[];
extern const u32 sBitTable[];

#define sEvoCursorPos           gBattleCommunication[1] // when learning a new move
#define sEvoGraphicsTaskId      gBattleCommunication[2]
// Task data from CycleEvolutionMonSprite
#define tEvoStopped data[8]
#define TASK_BIT_CAN_STOP       (1 << 0)
#define TASK_BIT_LEARN_MOVE     (1 << 7)
#define B_WIN_MSG                 0
#define B_WIN_YESNO              14
#define MUS_LEVEL_UP              257
#define MUS_EVOLVED               259
#define MUS_EVOLUTION_INTRO       263
#define MUS_EVOLUTION             264
#define BATTLESTRINGS_TABLE_START  STRINGID_TRAINER1LOSETEXT
#define DEFAULT_STAT_STAGE 6

enum {
    T_EVOSTATE_INTRO_MSG,
    T_EVOSTATE_INTRO_CRY,
    T_EVOSTATE_INTRO_SOUND,
    T_EVOSTATE_START_MUSIC,
    T_EVOSTATE_START_BG_AND_SPARKLE_SPIRAL,
    T_EVOSTATE_SPARKLE_ARC,
    T_EVOSTATE_CYCLE_MON_SPRITE,
    T_EVOSTATE_WAIT_CYCLE_MON_SPRITE,
    T_EVOSTATE_SPARKLE_CIRCLE,
    T_EVOSTATE_SPARKLE_SPRAY,
    T_EVOSTATE_EVO_SOUND,
    T_EVOSTATE_EVO_MON_ANIM,
    T_EVOSTATE_SET_MON_EVOLVED,
    T_EVOSTATE_TRY_LEARN_MOVE,
    T_EVOSTATE_END,
    T_EVOSTATE_CANCEL,
    T_EVOSTATE_CANCEL_MON_ANIM,
    T_EVOSTATE_CANCEL_MSG,
    T_EVOSTATE_LEARNED_MOVE,
    T_EVOSTATE_TRY_LEARN_ANOTHER_MOVE,
    T_EVOSTATE_REPLACE_MOVE,
};

enum {
    EVOSTATE_FADE_IN,
    EVOSTATE_INTRO_MSG,
    EVOSTATE_INTRO_MON_ANIM,
    EVOSTATE_INTRO_SOUND,
    EVOSTATE_START_MUSIC,
    EVOSTATE_START_BG_AND_SPARKLE_SPIRAL,
    EVOSTATE_SPARKLE_ARC,
    EVOSTATE_CYCLE_MON_SPRITE,
    EVOSTATE_WAIT_CYCLE_MON_SPRITE,
    EVOSTATE_SPARKLE_CIRCLE,
    EVOSTATE_SPARKLE_SPRAY,
    EVOSTATE_EVO_SOUND,
    EVOSTATE_RESTORE_SCREEN,
    EVOSTATE_EVO_MON_ANIM,
    EVOSTATE_SET_MON_EVOLVED,
    EVOSTATE_TRY_LEARN_MOVE,
    EVOSTATE_END,
    EVOSTATE_CANCEL,
    EVOSTATE_CANCEL_MON_ANIM,
    EVOSTATE_CANCEL_MSG,
    EVOSTATE_LEARNED_MOVE,
    EVOSTATE_TRY_LEARN_ANOTHER_MOVE,
    EVOSTATE_REPLACE_MOVE,
};

enum {
    MVSTATE_INTRO_MSG_1,
    MVSTATE_INTRO_MSG_2,
    MVSTATE_INTRO_MSG_3,
    MVSTATE_PRINT_YES_NO,
    MVSTATE_HANDLE_YES_NO,
    MVSTATE_SHOW_MOVE_SELECT,
    MVSTATE_HANDLE_MOVE_SELECT,
    MVSTATE_FORGET_MSG_1,
    MVSTATE_FORGET_MSG_2,
    MVSTATE_LEARNED_MOVE,
    MVSTATE_ASK_CANCEL,
    MVSTATE_CANCEL,
    MVSTATE_RETRY_AFTER_HM,
};