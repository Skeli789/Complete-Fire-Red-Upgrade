/********\

CUSTOM FILE!
THIS IS A CUSTOM FILE THAT ISN'T NORMALLY IN THE CFRU.

Give Credits to:
- Acimut - Actual EV Screen
- Greenphx - Sandbox / Stat Editor functionality
if used.

\********/

#include "../include/global.h"
#include "../include/pokemon.h"
#include "../include/bg.h"
#include "../include/window.h"
#include "../include/sprite.h"
#include "../include/palette.h"
#include "../include/task.h"
#include "../include/scanline_effect.h"
#include "../include/new/ability_util.h"
#include "../include/new/dns.h"
#include "../include/main.h"
#include "../include/new/Vanilla_functions.h"
#include "../include/new/ram_locs.h"
#include "../include/gpu_regs.h"
#include "../include/overworld.h"
#include "../include/malloc.h"
#include "../include/sound.h"
#include "../include/constants/pokemon.h"
#include "../include/string_util.h"
#include "../include/menu.h"
#include "../include/party_menu.h"
#include "../include/constants/species.h"
#include "../include/field_weather.h"
#include "../include/text_window.h"
#include "../include/new/build_pokemon.h"
#include "../include/new/build_pokemon_2.h"
#include "../include/new/util.h"
#include "../include/new/util2.h"
#include "../include/mgba.h"
#include "../include/wild_encounter.h"
#include "../include/new/form_change.h"
#include "../include/random.h"
#include "../include/new/exp.h"
#include "../include/constants/songs.h"

#define FIRERED

#define ESP //comment this to use the english text

// 1 = ACTIVADO, 0 = DESACTIVADO. Activa o desactiva el salto de sprite
// 1 = ON, 0 = OFF.  Activates or deactivates the sprite jump
#define SPRITE_JUMP             1

// 1 = DE DERECHA A IZQUIERDA, 0 = EN EL CENTRO -1 = DE IZQUIERDA A DERECHA.
// 1 = FROM RIGHT TO LEFT, 0 = IN THE CENTER -1 = FROM LEFT TO RIGHT.
#define SPRITE_JUMP_DIRECTION   1

// 1 = A LA DERECHA, 0 = A LA IZQUIERDA.
// 1 = RIGHT, 0 = LEFT.
#define SPRITE_VIEW_DIRECTION   0


//coordenada x del sprite pokémon, se mide en tiles de x8 pixeles
//x coordinate of the pokémon sprite, measured in tiles of x8 pixels
#define PICMON_X    18

//coordenada y del sprite pokémon, se mide en tiles de x8 pixeles
//y coordinate of the pokémon sprite, measured in tiles of x8 pixels
#define PICMON_Y     5

static void Task_EvIvInit(u8);
static u8 EvIvLoadGfx(void);
static void EvIvVblankHandler(void);
static void Task_WaitForExit(u8);
static void Task_EvIvReturnToOverworld(u8);
static void ShowSprite(struct Pokemon *mon);
static void EvIvPrintText(struct Pokemon *mon);
static void ShowPokemonPic2(u16 species, u32 otId, u32 personality, u8 x, u8 y);
void Task_ScriptShowMonPic(u8 taskId);
static void HidePokemonPic2(u8 taskId);
static void PrintGenderText(struct Pokemon *mon);
static void SandboxChangeGender();

static void PrintStat(u8 nature, u8 stat);
static u8 GetDigits(u16 num);
static u8 GetColorByNature(u8 nature, u8 stat);

extern const u8 bgEvIvTiles[];
extern const u8 bgEvIvMap[];
extern const u8 bgEvIvPal[];

enum
{
    WIN_POKEMON_NAME,
    WIN_STATS,
    WIN_BOTTOM_BOX
};

static const struct BgTemplate bg_Templates[] = {
    {
        .bg = 0,
        .charBaseIndex = 0,
        .mapBaseIndex = 22,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 1,
    }, {
        .bg = 1,
        .charBaseIndex = 2,
        .mapBaseIndex = 19,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 2,
        .baseTile = 0,
    }
};

//window 0 = pokémon name
#define WINDOW0_WIDTH   29
#define WINDOW0_HEIGTH  4

//window 1 = stats
#define WINDOW1_WIDTH   17
#define WINDOW1_HEIGTH  11
#define WINDOW1_BASEBLOCK  (WINDOW0_WIDTH * WINDOW0_HEIGTH)

//window 2 = text in the bottom bar / texto en la barra inferior
#define WINDOW2_WIDTH   29
#define WINDOW2_HEIGTH  5
#define WINDOW2_BASEBLOCK WINDOW1_WIDTH * WINDOW1_HEIGTH + WINDOW1_BASEBLOCK

static const struct WindowTemplate windows_templates[] = {
    {//window 0 = pokémon name
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 2,
        .width = WINDOW0_WIDTH,
        .height = WINDOW0_HEIGTH,
        .paletteNum = 15,
        .baseBlock = 0x000
    },{//window 1 = stats
        .bg = 0,
        .tilemapLeft = 2,
        .tilemapTop = 4,
        .width = WINDOW1_WIDTH,
        .height = WINDOW1_HEIGTH,
        .paletteNum = 15,
        .baseBlock = WINDOW1_BASEBLOCK
    },{//window 2 = text in the bottom bar / texto en la barra inferior
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 15,
        .width = WINDOW2_WIDTH,
        .height = WINDOW2_HEIGTH,
        .paletteNum = 15,
        .baseBlock = WINDOW2_BASEBLOCK
    }, DUMMY_WIN_TEMPLATE
};

struct EvIv
{
    u8 state;
    u8 gfxStep;
    u8 callbackStep;
    u8 currentMon;
    u8 spriteTaskId;
    u8 stats_ev[NUM_STATS];
    u8 stats_iv[NUM_STATS];
    u8 stats_bs[NUM_STATS];
    u16 totalStatsEV;
    u16 totalStatsIV;
    u16 totalStatsBS;
    u16 tilemapBuffer[0x400];
    u16 monSpriteId;
    u8 inEditor : 1;
    u8 inSelector : 1;
    u8 selectedColumn;
    u8 selectedStat;
    u16 cursorSpriteId;
};

#define gEvIv (*((struct EvIv**) 0x0203FFF0))

#define gState              gEvIv->state
#define gGfxStep            gEvIv->gfxStep
#define gCallbackStep       gEvIv->callbackStep
#define gCurrentMon         gEvIv->currentMon
#define gSpriteTaskId       gEvIv->spriteTaskId
#define gStats_ev           gEvIv->stats_ev
#define gStats_iv           gEvIv->stats_iv
#define gStats_bs           gEvIv->stats_bs
#define gTotalStatsEV       gEvIv->totalStatsEV
#define gTotalStatsIV       gEvIv->totalStatsIV
#define gTotalStatsBS       gEvIv->totalStatsBS
#define gInEditor           gEvIv->inEditor
#define gInSelector         gEvIv->inSelector
#define gSelectedColumn     gEvIv->selectedColumn
#define gSelectedStat       gEvIv->selectedStat
#define gCursorSpriteId     gEvIv->cursorSpriteId

#define EVS             0
#define IVS             1
#define ABILITY_EDIT    2
#define GENDER          3

#define EDITOR_STAT_HP         0
#define EDITOR_STAT_ATK        1
#define EDITOR_STAT_DEF        2
#define EDITOR_STAT_SPATK      3
#define EDITOR_STAT_SPDEF      4
#define EDITOR_STAT_SPD        5

#define SELECTION_CURSOR_TAG 0x200

static void SpriteCB_SandboxCursor(struct Sprite* sprite);
static void MiniEvIvPrintText(struct Pokemon *mon, bool8 ev, u8 stat, u8 newValue, u8 stat2, bool8 fixev);
static void PrintWindow0(struct Pokemon *mon);
static void PrintWindow1(u8 nature, u8 isEgg);
static void PrintWindow2(u16 species, u8 isEgg, u8 friendship, u8 ability);
static void ClearAndPrintAbilityText(u8 ability, u16 species);
static const struct OamData sCursorOam =
{
	.affineMode = ST_OAM_AFFINE_OFF,
	.objMode = ST_OAM_OBJ_NORMAL,
	.shape = SPRITE_SHAPE(32x32),
	.size = SPRITE_SIZE(32x32),
	.priority = 0, //Above other sprites
};
static const union AnimCmd sAnimCmdHandCursor[] =
{
	ANIMCMD_FRAME(0, 30),
	ANIMCMD_FRAME(16, 30),
	ANIMCMD_JUMP(0)
};

static const union AnimCmd sAnimCmdHandCursorPointed[] =
{
	ANIMCMD_FRAME(16, 30),
	ANIMCMD_END
};

static const union AnimCmd *const sAnimCmdTable_HandCursor[] =
{
	sAnimCmdHandCursor,
	sAnimCmdHandCursorPointed,
};
static const struct SpriteTemplate sGUICursorTemplate =
{
	.tileTag = SELECTION_CURSOR_TAG,
	.paletteTag = SELECTION_CURSOR_TAG,
	.oam = &sCursorOam,
	.anims = sAnimCmdTable_HandCursor,
	.images = NULL,
	.affineAnims = gDummySpriteAffineAnimTable,
	.callback = SpriteCB_SandboxCursor,
};

static const struct SpriteSheet sCursorSpriteSheet = {(void*) 0x83D2BEC, (32 * 32 * 4) / 2, SELECTION_CURSOR_TAG};
static const struct SpritePalette sCursorSpritePalette = {(void*) 0x83CE7F0, SELECTION_CURSOR_TAG};

static void SpriteCB_SandboxCursor(struct Sprite* sprite)
{
    (void)sprite; // Prevents unused parameter warning
}

static void CreateSandboxCursor(void)
{
	LoadSpriteSheet(&sCursorSpriteSheet);
	LoadSpritePalette(&sCursorSpritePalette);
    u8 x = 0;
    u8 y = 0;
    switch(gSelectedColumn)
    {
        case EVS:
            x = 112;
            y = 28 + (14 * gSelectedStat); 
            break;
        case IVS:
            x = 138;
            y = 28 + (14 * gSelectedStat); 
            break;
        case ABILITY_EDIT:
            x = 180;
            y = 121;
            break;
        case GENDER:
            x = 164;
            y = 24;
            break;
    }
	gCursorSpriteId = CreateSprite(&sGUICursorTemplate, x, y, 1);
}

static void EvIvBgInit(void)
{
    ResetSpriteData();
    ResetPaletteFade();
    FreeAllSpritePalettes();
    ResetTasks();
    ScanlineEffect_Stop();
}

static void CB2_EvIv(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
}

void CB2_ShowEvIv(void)
{
    gEvIv = Calloc(sizeof(struct EvIv));
    gState = 0;
    gGfxStep = 0;
    gCallbackStep = 0;
    gCurrentMon = 0;
    EvIvBgInit();
    CreateTask(Task_EvIvInit, 0);
    SetMainCallback2(CB2_EvIv);
}

static void VCBC_EvIvOam(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

static void Task_EvIvInit(u8 taskId)
{
    switch (gCallbackStep)
    {
    case 0:
        SetVBlankCallback(NULL);
        break;
    case 1:
        EvIvVblankHandler();
        break;
    case 2:
        if (!EvIvLoadGfx())
        {
            return;
        }
        break;
    case 3:
        CopyToBgTilemapBuffer(1, bgEvIvMap, 0, 0);
        break;
    case 4:
        SetGpuReg(REG_OFFSET_BG1HOFS, 0);
        break;
    case 5:
        ShowSprite(&gPlayerParty[gCurrentMon]);
        EvIvPrintText(&gPlayerParty[gCurrentMon]);
        PrintGenderText(&gPlayerParty[gCurrentMon]);
        break;
    case 6:
        CopyBgTilemapBufferToVram(0);
        CopyBgTilemapBufferToVram(1);
        break;
    case 7:
        BeginNormalPaletteFade(0xFFFFFFFF, 0, 16, 0, RGB_BLACK);
        break;
    case 8:
        SetVBlankCallback(VCBC_EvIvOam);
        //if(FlagGet(FLAG_SANDBOX_MODE))
        //    CreateSandboxCursor();
        break;
    default:
        if (gPaletteFade->active)
        {
            break;
        }
        gTasks[taskId].func = Task_WaitForExit;
    }
    gCallbackStep++;
}

static void UpdateCursorSpritePos(u16 spriteId, u8 stat, bool8 goingUp, bool8 resetY)
{
    struct Sprite * sprite = &gSprites[spriteId];
    u8 newPosX = sprite->pos1.x; 
    u8 newPosY = sprite->pos1.y;
    u8 ability = GetMonAbility(&gPlayerParty[gCurrentMon]);
    u16 species = gPlayerParty[gCurrentMon].species;

    if(stat == 0xFF)
    {
        switch(gSelectedColumn)
        {
            case EVS:
                newPosX = 112;
                break;
            case IVS:
                newPosX = 138;
                break;
            case ABILITY_EDIT:
                CopyAbilityName(gStringVar1, ability, species);
                newPosX = (157 + (GetStringWidth(2, gStringVar1, 0) / 2)) - 5;
                newPosY = 121;
                break;
            case GENDER:
                newPosX = 164;
                newPosY = 24;
                break;
        }
    }
    else
    {
        if(!goingUp)
        {
            if(stat == EDITOR_STAT_HP)
                newPosY = 28;
            else
                newPosY += 14;
        }
        else
        {
            if(stat == EDITOR_STAT_SPD)
                newPosY = 98;
            else
                newPosY -= 14;
        }
    }
    if(resetY)
    {
        if(goingUp)
        {
            newPosX = 112;
            newPosY = 28;
        }
        else
        {
            newPosX = 138;
            newPosY = 28;
        }
    }

    sprite->pos1.x = newPosX;
    sprite->pos1.y = newPosY;
}

static void SandboxChangeAbility(bool8 goingRight)
{
    struct Pokemon * mon = &gPlayerParty[gCurrentMon];
    u16 species = GetMonData(mon, MON_DATA_SPECIES, NULL);
    u8 currentAbilNum = 0;
    u32 personality = GetMonData(mon, MON_DATA_PERSONALITY, NULL);
    u8 nature = GetNatureFromPersonality(personality);
    bool8 isShiny = IsMonShiny(mon);
    if(GetMonAbility(mon) == GetAbility1(species))
        currentAbilNum = 0;
    else if(GetMonAbility(mon) == GetAbility2(species))
        currentAbilNum = 1;
    else if(GetMonAbility(mon) == GetHiddenAbility(species))
        currentAbilNum = 2;
    if(GetAbility2(species) == ABILITY_NONE && GetHiddenAbility(species) == ABILITY_NONE)
    {
        PlaySE(SE_ERROR);
        return;
    }
    if(goingRight)
    {
        if(currentAbilNum == 0 && GetAbility2(species) == ABILITY_NONE)
            mon->hiddenAbility = TRUE;
        else if(currentAbilNum == 0)
            GiveMonNatureAndAbility(mon, nature, 1, isShiny, TRUE, FALSE);      
        else if(currentAbilNum == 1)
            mon->hiddenAbility = TRUE;
        else if(currentAbilNum == 2)
        {
            mon->hiddenAbility = FALSE;
            GiveMonNatureAndAbility(mon, nature, 0, isShiny, TRUE, FALSE);      
        }
    }
    else
    {
        if(currentAbilNum == 0 && GetAbility2(species) == ABILITY_NONE)
            mon->hiddenAbility = TRUE;
        else if(currentAbilNum == 0)
            mon->hiddenAbility = TRUE;
        else if(currentAbilNum == 1)
            GiveMonNatureAndAbility(mon, nature, 0, isShiny, TRUE, FALSE);      
        else if(currentAbilNum == 2)
        {
            mon->hiddenAbility = FALSE;
            GiveMonNatureAndAbility(mon, nature, 1, isShiny, TRUE, FALSE);      
        }
    }

    u8 ability = GetMonAbility(mon);
    ClearAndPrintAbilityText(ability, species);

    /*u8 isEgg    = GetMonData(mon, MON_DATA_IS_EGG, 0);
    u8 friendship = GetMonData(mon, MON_DATA_FRIENDSHIP, 0);
    
    PrintWindow2(species, isEgg, friendship, ability);*/


    PutWindowTilemap(2);
}

static u16 GetCurrentEVCap()
{
    u8 badges = GetBadgeCount();
    u16 cap = 510;
    switch(badges)
    {
        case 0: //no badges, 32 cap
            cap = 32;
            break;
        case 1: //1 badges, 100 cap
            cap = 100;
            break;
        case 2: //2 badges, 252 cap
            cap = 252;
            break;
        default: 
            cap = 510; //not sure if needed, just in case
            break;
    }
    return cap;
}

static void FixOddEVs(void)
{
    struct Pokemon * mon = &gPlayerParty[gCurrentMon];
    for(int i = MON_DATA_HP_EV; i < MON_DATA_SPDEF_EV + 1; i++)
    {
        u16 ev = GetMonData(mon, i, 0);
        u16 newEv;
        if(ev % 2 && ev != 0)
        {
            newEv = ev - 1;
            SetMonData(mon, i, &newEv);
            MiniEvIvPrintText(mon, TRUE, i, newEv, i - MON_DATA_HP_EV, FALSE);
        }
            

    }

}


static void ChangeSelectedStat(u8 stat, u8 ev, bool8 increase)
{
    u8 newValue;
    u8 statToEdit;
    u8 increaseBy = (ev) ? 2 : 1;
    u16 total = 0;
    u16 newTotal = 0;
    switch(stat)
    {
        case EDITOR_STAT_SPATK:
            statToEdit = (ev) ? MON_DATA_SPATK_EV : MON_DATA_SPATK_IV;
            break;
        case EDITOR_STAT_SPDEF:
            statToEdit = (ev) ? MON_DATA_SPDEF_EV : MON_DATA_SPDEF_IV;
            break;
        case EDITOR_STAT_SPD:
            statToEdit = (ev) ? MON_DATA_SPEED_EV : MON_DATA_SPEED_IV;
            break;
        default:
            statToEdit = (ev) ? MON_DATA_HP_EV + stat : MON_DATA_HP_IV + stat;
            break;
    }

    struct Pokemon * mon = &gPlayerParty[gCurrentMon];
    u8 currValue = GetMonData(mon, statToEdit, NULL);
    if(!ev)
    {
        if (currValue > 30 && increase)
            newValue = 0;
        else if (currValue == 0 && !increase)
            newValue = 31;
        else if (increase)
            newValue = currValue + increaseBy;
        else
            newValue = currValue - increaseBy;
    }
    if(ev)
    {
        if(currValue > 250 && increase)
            newValue = 0;
        else if (currValue < 2 && !increase)
            newValue = 252;
        else if (increase)
            newValue = currValue + increaseBy;
        else if (!increase)
            newValue = currValue - increaseBy;
        u16 cap = GetCurrentEVCap();
        total += mon->hpEv;
        total += mon->atkEv;
        total += mon->defEv;
        total += mon->spAtkEv;
        total += mon->spDefEv;
        total += mon->spdEv;
        newTotal += (statToEdit == MON_DATA_HP_EV) ? newValue : mon->hpEv;
        newTotal += (statToEdit == MON_DATA_ATK_EV) ? newValue : mon->atkEv;
        newTotal += (statToEdit == MON_DATA_DEF_EV) ? newValue : mon->defEv;
        newTotal += (statToEdit == MON_DATA_SPATK_EV) ? newValue : mon->spAtkEv;
        newTotal += (statToEdit == MON_DATA_SPDEF_EV) ? newValue : mon->spDefEv;
        newTotal += (statToEdit == MON_DATA_SPEED_EV) ? newValue : mon->spdEv;
        if(newTotal > cap)
            newValue = cap - total;
    }
    /*u8 maxValue = (ev) ? 252 : 31;

    increaseBy = (ev) ? 2 : 1;
    if(increase && currValue != 251 && currValue != 252)
         newValue = currValue + increaseBy;
    else if (currValue == 252)
    {
        newValue = 0;
    }
    else if(!increase && currValue != 0 && currValue != 1)
        newValue = currValue - increaseBy;
    else if(currValue == 0)
    {
        newValue = GetCurrentEVCap();
    }
    if(ev)
    {
        u16 cap = GetCurrentEVCap();
        total += mon->hpEv;
        total += mon->atkEv;
        total += mon->defEv;
        total += mon->spAtkEv;
        total += mon->spDefEv;
        total += mon->spdEv;
        newTotal += (statToEdit == MON_DATA_HP_EV) ? newValue : mon->hpEv;
        newTotal += (statToEdit == MON_DATA_ATK_EV) ? newValue : mon->atkEv;
        newTotal += (statToEdit == MON_DATA_DEF_EV) ? newValue : mon->defEv;
        newTotal += (statToEdit == MON_DATA_SPATK_EV) ? newValue : mon->spAtkEv;
        newTotal += (statToEdit == MON_DATA_SPDEF_EV) ? newValue : mon->spDefEv;
        newTotal += (statToEdit == MON_DATA_SPEED_EV) ? newValue : mon->spdEv;
        if(newTotal >= cap)
            newValue = cap - total;
        if(!(newValue % 2 == 0))
            newValue -= 1;
    }*/
    SetMonData(mon, statToEdit, &newValue);
    MiniEvIvPrintText(mon, ev, statToEdit, newValue, stat, FALSE);
}

static void SandboxChangeNature(bool8 goingRight)
{
    struct Pokemon *mon = &gPlayerParty[gCurrentMon];
    u8 currNature = GetNature(mon);
    u8 newNature = 0;
    if(goingRight)
    {
        switch(currNature)
        {
            case NATURE_HARDY:
                newNature = NATURE_LONELY;
                break;
            case NATURE_LONELY:
                newNature = NATURE_ADAMANT;
                break;
            case NATURE_ADAMANT:
                newNature = NATURE_NAUGHTY;
                break;
            case NATURE_NAUGHTY:
                newNature = NATURE_BRAVE;
                break;
            case NATURE_BRAVE:
                newNature = NATURE_BOLD;
                break;
            case NATURE_BOLD:
            case NATURE_DOCILE:
                newNature = NATURE_IMPISH;
                break;
            case NATURE_IMPISH:
                newNature = NATURE_LAX;
                break;
            case NATURE_LAX:
                newNature = NATURE_RELAXED;
                break;
            case NATURE_RELAXED:
                newNature = NATURE_MODEST;
                break;
            case NATURE_MODEST:
                newNature = NATURE_MILD;
                break;
            case NATURE_MILD:
            case NATURE_BASHFUL:
                newNature = NATURE_RASH;
                break;
            case NATURE_RASH:
                newNature = NATURE_QUIET;
                break;
            case NATURE_QUIET:
                newNature = NATURE_CALM;
                break;
            case NATURE_CALM:
                newNature = NATURE_GENTLE;
                break;
            case NATURE_GENTLE:
                newNature = NATURE_CAREFUL;
                break;
            case NATURE_CAREFUL:
            case NATURE_QUIRKY:
                newNature = NATURE_SASSY;
                break;
            case NATURE_SASSY:
                newNature = NATURE_TIMID;
                break;
            case NATURE_TIMID:
                newNature = NATURE_HASTY;
                break;
            case NATURE_HASTY:
                newNature = NATURE_JOLLY;
                break;
            case NATURE_JOLLY:
                newNature = NATURE_NAIVE;
                break;
            case NATURE_NAIVE:
            case NATURE_SERIOUS:
                newNature = NATURE_ADAMANT;
                break;
        }
    }
    else
    {
        switch(currNature)
        {
            case NATURE_SERIOUS:
            case NATURE_NAIVE:
                newNature = NATURE_JOLLY;
                break;
            case NATURE_JOLLY:
                newNature = NATURE_HASTY;
                break;
            case NATURE_HASTY:
                newNature = NATURE_TIMID;
                break;
            case NATURE_TIMID:
                newNature = NATURE_SASSY;
                break;
            case NATURE_SASSY:
            case NATURE_QUIRKY:
                newNature = NATURE_CAREFUL;
                break;
            case NATURE_CAREFUL:
                newNature = NATURE_GENTLE;
                break;
            case NATURE_GENTLE:
                newNature = NATURE_CALM;
                break;
            case NATURE_CALM:
                newNature = NATURE_QUIET;
                break;
            case NATURE_QUIET:
                newNature = NATURE_RASH;
                break;
            case NATURE_RASH:
            case NATURE_BASHFUL:
                newNature = NATURE_MILD;
                break;
            case NATURE_MILD:
                newNature = NATURE_MODEST;
                break;
            case NATURE_MODEST:
                newNature = NATURE_RELAXED;
                break;
            case NATURE_RELAXED:
                newNature = NATURE_LAX;
                break;
            case NATURE_LAX:
                newNature = NATURE_IMPISH;
                break;
            case NATURE_IMPISH:
            case NATURE_DOCILE:
                newNature = NATURE_BOLD;
                break;
            case NATURE_BOLD:
                newNature = NATURE_BRAVE;
                break;
            case NATURE_BRAVE: 
                newNature = NATURE_NAUGHTY;
                break;
            case NATURE_NAUGHTY:
                newNature = NATURE_ADAMANT;
                break;
            case NATURE_ADAMANT:
                newNature = NATURE_LONELY;
                break;
            case NATURE_LONELY:
            case NATURE_HARDY:
                newNature = NATURE_NAIVE;
                break;
        }
    }
    GiveMonNatureAndAbility(mon, newNature, GetMonData(mon, MON_DATA_PERSONALITY, NULL) & 1, IsMonShiny(mon), TRUE, FALSE);
    //MiniEvIvPrintText(mon, ev, statToEdit, newValue, stat);
    //FillWindowPixelBuffer(0, 0);
    FillWindowPixelBuffer(1, 0);
    //FillWindowPixelBuffer(2, 0);

    //PrintWindow0(mon);
    PrintWindow1(newNature, FALSE);
    //PrintWindow2(species, isEgg, friendship, ability);

    //PutWindowTilemap(0);
    PutWindowTilemap(1);
    //PutWindowTilemap(2);
}

static void Task_WaitForExit(u8 taskId)
{
    switch (gState)
    {
    case 0:
        gState++;
        break;
    case 1:
        if (FlagGet(FLAG_STAT_EDITOR_UNLOCKED))
        {
            if (JOY_NEW(A_BUTTON))
            {
                if (!gInSelector && !gInEditor)
                {
                    gInSelector = TRUE;
                    PlaySE(5);
                    CreateSandboxCursor();
                }
                else if(!gInEditor && gInSelector)
                {
                    PlaySE(5);
                    gInEditor = TRUE;
                    gInSelector = FALSE;
                    DestroySprite(&gSprites[gCursorSpriteId]);
                }
            }
            if (JOY_NEW(B_BUTTON))
            {
                if(!gInSelector && !gInEditor)
                {
                    PlaySE(242);
                    DestroySprite(&gSprites[gCursorSpriteId]);
                    BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 16, RGB_BLACK);
                    for(u8 i = 0; i < gPlayerPartyCount; i++)
                    {
                        CalculateMonStatsNew(&gPlayerParty[i]);
                        u8 max = GetMonData(&gPlayerParty[i], MON_DATA_MAX_HP, NULL);
                        u8 curr = GetMonData(&gPlayerParty[i], MON_DATA_HP, NULL);
                        if(curr > max)
                        {
                            SetMonData(&gPlayerParty[i], MON_DATA_HP, &max);
                        }
                    }
                    gState++;
                }
                else if (gInEditor)
                {
                    PlaySE(5);
                    gInEditor = FALSE;
                    gInSelector = TRUE;
                    CreateSandboxCursor();
                    if(gSelectedColumn == 2)
                        UpdateCursorSpritePos(gCursorSpriteId, 0xFF, FALSE, FALSE);
                    
                }
                else
                {
                    PlaySE(5);
                    gInSelector = FALSE;
                    DestroySprite(&gSprites[gCursorSpriteId]);
                }
            }
            if(JOY_NEW(R_BUTTON))
            {
                SandboxChangeNature(TRUE);
            }
            if(JOY_NEW(L_BUTTON))
            {
                SandboxChangeNature(FALSE);
            }
            if (!gInSelector && !gInEditor)
            {
                if (JOY_REPT(DPAD_DOWN) && gPlayerPartyCount > 1)
                {
                    if (gCurrentMon == (gPlayerPartyCount - 1))
                        gCurrentMon = 0;
                    else
                        gCurrentMon++;
                    HidePokemonPic2(gSpriteTaskId);
                    ShowSprite(&gPlayerParty[gCurrentMon]);
                    EvIvPrintText(&gPlayerParty[gCurrentMon]);
                    PrintGenderText(&gPlayerParty[gCurrentMon]);
                    //reset selected column & selected stat
                    gSelectedColumn = 0;
                    gSelectedStat = STAT_HP;
                }
                if (JOY_REPT(DPAD_UP) && gPlayerPartyCount > 1)
                {
                    if (gCurrentMon == 0)
                        gCurrentMon = (gPlayerPartyCount - 1);
                    else
                        gCurrentMon--;
                    HidePokemonPic2(gSpriteTaskId);
                    ShowSprite(&gPlayerParty[gCurrentMon]);
                    EvIvPrintText(&gPlayerParty[gCurrentMon]);
                    PrintGenderText(&gPlayerParty[gCurrentMon]);
                    //reset selected column & selected stat
                    gSelectedColumn = 0;
                    gSelectedStat = STAT_HP;
                }
            }
            else if(gInSelector)
            {
                u8 resetY = FALSE;
                if (JOY_NEW(START_BUTTON))
                {
                    FixOddEVs();
                }
                if (JOY_REPT(DPAD_DOWN))
                {
                    if(gSelectedColumn == 2 || gSelectedColumn == 3)
                    {
                        //dont do anything
                    }
                    else if(gSelectedStat == EDITOR_STAT_SPD)
                        gSelectedStat = EDITOR_STAT_HP;
                    else
                        gSelectedStat++;
                    if(gSelectedColumn != 2 && gSelectedColumn != 3)
                        UpdateCursorSpritePos(gCursorSpriteId, gSelectedStat, FALSE, FALSE);
                }
                if (JOY_REPT(DPAD_UP))
                {
                    if(gSelectedColumn == 2 || gSelectedColumn == 3)
                    {
                        //dont do anything
                    }
                    else if(gSelectedStat == EDITOR_STAT_HP)
                        gSelectedStat = EDITOR_STAT_SPD;
                    else
                        gSelectedStat--;
                    if(gSelectedColumn != 2 && gSelectedColumn != 3)
                        UpdateCursorSpritePos(gCursorSpriteId, gSelectedStat, TRUE, FALSE);
                }
                if (JOY_REPT(DPAD_LEFT))
                {
                    if(gSelectedColumn == 0)
                        gSelectedColumn = (GetGenderFromSpeciesAndPersonality(gPlayerParty[gCurrentMon].species, gPlayerParty[gCurrentMon].personality) != MON_GENDERLESS) ? 3 : 2;
                    else if(gSelectedColumn == 2)
                    {
                        gSelectedColumn = 1;
                        resetY = TRUE;
                        gSelectedStat = STAT_HP;
                    }
                    else
                        gSelectedColumn--;
                    UpdateCursorSpritePos(gCursorSpriteId, 0xFF, FALSE, resetY);
                }
                if (JOY_REPT(DPAD_RIGHT))
                {
                    if(GetGenderFromSpeciesAndPersonality(gPlayerParty[gCurrentMon].species, gPlayerParty[gCurrentMon].personality) == MON_GENDERLESS && gSelectedColumn == 2)
                    {
                        gSelectedColumn = 0;
                        resetY = TRUE;
                        gSelectedStat = STAT_HP;
                    }
                    else if(gSelectedColumn == 3)
                    {
                        gSelectedColumn = 0;
                        resetY = TRUE;
                        gSelectedStat = STAT_HP;
                    }
                    else
                        gSelectedColumn++;
                    UpdateCursorSpritePos(gCursorSpriteId, 0xFF, TRUE, resetY);
                }
            }
            else if(gInEditor)
            {
                if (JOY_REPT(DPAD_LEFT))
                {
                    if(gSelectedColumn == 2)
                        SandboxChangeAbility(FALSE);
                    else if(gSelectedColumn == 3)
                        SandboxChangeGender();
                    else
                        ChangeSelectedStat(gSelectedStat, gSelectedColumn == 0, FALSE);
                }
                if (JOY_REPT(DPAD_RIGHT))
                {
                    if(gSelectedColumn == 2)
                        SandboxChangeAbility(TRUE);
                    else if(gSelectedColumn == 3)
                        SandboxChangeGender();
                    else
                        ChangeSelectedStat(gSelectedStat, gSelectedColumn == 0, TRUE);
                } 
            }
        }
        else
        {
            if (JOY_NEW(A_BUTTON) || JOY_NEW(B_BUTTON))
            {
                PlaySE(242);
                BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 16, RGB_BLACK);
                gState++;
            }
            if (JOY_REPT(DPAD_DOWN) && gPlayerPartyCount > 1)
            {
                if (gCurrentMon == (gPlayerPartyCount - 1))
                    gCurrentMon = 0;
                else
                    gCurrentMon++;
                HidePokemonPic2(gSpriteTaskId);
                ShowSprite(&gPlayerParty[gCurrentMon]);
                EvIvPrintText(&gPlayerParty[gCurrentMon]);
                PrintGenderText(&gPlayerParty[gCurrentMon]);
            }
            if (JOY_REPT(DPAD_UP) && gPlayerPartyCount > 1)
            {
                if (gCurrentMon == 0)
                    gCurrentMon = (gPlayerPartyCount - 1);
                else
                    gCurrentMon--;
                HidePokemonPic2(gSpriteTaskId);
                ShowSprite(&gPlayerParty[gCurrentMon]);
                EvIvPrintText(&gPlayerParty[gCurrentMon]);
                PrintGenderText(&gPlayerParty[gCurrentMon]);
            }
        }
        break;
    case 2:
        if (!IsCryPlaying())
            Task_EvIvReturnToOverworld(taskId);
        break;
    }
}

static void Task_EvIvReturnToOverworld(u8 taskId)
{
    if (gPaletteFade->active)
        return;
    DestroyTask(taskId);
    FreeAllWindowBuffers();
    Free(gEvIv);
#ifdef FIRERED
//FIRERED
        SetMainCallback2(CB2_ReturnToFieldFromDiploma);
#else
//EMERALD
        SetMainCallback2(CB2_ReturnToFieldFadeFromBlack);
#endif
}

static void ResetBGPos(void)
{
    ChangeBgX(0, 0, 0);
    ChangeBgY(0, 0, 0);
    ChangeBgX(1, 0, 0);
    ChangeBgY(1, 0, 0);
    ChangeBgX(2, 0, 0);
    ChangeBgY(2, 0, 0);
    ChangeBgX(3, 0, 0);
    ChangeBgY(3, 0, 0);
}

static void ResetGpu(void)
{
    void *vram = (void *)VRAM;
    DmaClearLarge16(3, vram, VRAM_SIZE, 0x1000);
    DmaClear32(3, (void *)OAM, OAM_SIZE);
    DmaClear16(3, (void *)PLTT, PLTT_SIZE);
    SetGpuReg(REG_OFFSET_DISPCNT, 0);
    SetGpuReg(REG_OFFSET_BG0CNT, 0);
    SetGpuReg(REG_OFFSET_BG0HOFS, 0);
    SetGpuReg(REG_OFFSET_BG0VOFS, 0);
    SetGpuReg(REG_OFFSET_BG1CNT, 0);
    SetGpuReg(REG_OFFSET_BG1HOFS, 0);
    SetGpuReg(REG_OFFSET_BG1VOFS, 0);
    SetGpuReg(REG_OFFSET_BG2CNT, 0);
    SetGpuReg(REG_OFFSET_BG2HOFS, 0);
    SetGpuReg(REG_OFFSET_BG2VOFS, 0);
    SetGpuReg(REG_OFFSET_BG3CNT, 0);
    SetGpuReg(REG_OFFSET_BG3HOFS, 0);
    SetGpuReg(REG_OFFSET_BG3VOFS, 0);
    SetGpuReg(REG_OFFSET_WIN0H, 0);
    SetGpuReg(REG_OFFSET_WIN0V, 0);
    SetGpuReg(REG_OFFSET_WININ, 0);
    SetGpuReg(REG_OFFSET_WINOUT, 0);
    SetGpuReg(REG_OFFSET_BLDCNT, 0);
    SetGpuReg(REG_OFFSET_BLDALPHA, 0);
    SetGpuReg(REG_OFFSET_BLDY, 0);
}

static void EvIvVblankHandler(void)
{
    ResetGpu();
    ResetBgsAndClearDma3BusyFlags(0);
    InitBgsFromTemplates(0, bg_Templates, 2);
    ResetBGPos();
    InitWindows(windows_templates);
    DeactivateAllTextPrinters();
    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_1D_MAP | DISPCNT_OBJ_ON);
    SetBgTilemapBuffer(1, gEvIv->tilemapBuffer);
    ShowBg(0);
    ShowBg(1);
    FillBgTilemapBufferRect_Palette0(0, 0, 0, 0, 30, 20);
    FillBgTilemapBufferRect_Palette0(1, 0, 0, 0, 30, 20);
}

static u8 EvIvLoadGfx(void)
{
    switch (gGfxStep)
    {
    case 0:
        ResetTempTileDataBuffers();
        break;
    case 1:
        DecompressAndCopyTileDataToVram(1, bgEvIvTiles, 0, 0, 0);
        break;
    case 2:
        if (!(FreeTempTileDataBuffersIfPossible() == 1))
        {
            break;
        }
        return 0;
    case 3:
        LoadCompressedPalette(bgEvIvPal, 0, 0x20);
#ifdef FIRERED
//FIRERED
        LoadPalette(stdpal_get(0), 0xf0, 0x20);
#else
//EMERALD
        LoadPalette(GetTextWindowPalette(0), 0xf0, 0x20);
#endif
        break;
    default:
        return 1;
    }
    gGfxStep++;
    return 0;
}

static void ShowSprite(struct Pokemon *mon)
{
    u16 species = GetMonData(mon, MON_DATA_SPECIES, NULL);
    u8 isEgg    = GetMonData(mon, MON_DATA_IS_EGG, NULL);
    u32 otId = GetMonData(mon, MON_DATA_OT_ID, NULL);
    u32 personality = GetMonData(mon, MON_DATA_PERSONALITY, NULL);

    //imprime el sprite del pokémon, si es un huevo no suena grito.
    //Print the sprite of the pokémon, if it is an egg it does not sound a scream.
    if (!isEgg)
    {
        ShowPokemonPic2(species, otId, personality, PICMON_X, PICMON_Y);
#ifdef FIRERED
//FIRERED
        PlayCry7(species, 0);
#else
//EMERALD
        PlayCry1(species, 0);
#endif
    }else
        ShowPokemonPic2(SPECIES_EGG, 0, 0x8000, PICMON_X, PICMON_Y);
}

static void HidePokemonPic2(u8 taskId)
{
    struct Task * task = &gTasks[taskId];
    task->data[0]++;
}

static u8 CreateMonSprite_Field(u16 species, u32 otId, u32 personality, s16 x, s16 y)
{
    const struct CompressedSpritePalette * spritePalette = GetMonSpritePalStructFromOtIdPersonality(species, otId, personality);
    u16 spriteId = CreateMonPicSprite_HandleDeoxys(species, otId, personality, 1, x, y, 0, spritePalette->tag);
    PreservePaletteInWeather(IndexOfSpritePaletteTag(spritePalette->tag) + 0x10);
    if (spriteId == 0xFFFF)
        return MAX_SPRITES;
    else
        return spriteId;
}

static void ShowPokemonPic2(u16 species, u32 otId, u32 personality, u8 x, u8 y)
{
    u8 spriteId;

    spriteId = CreateMonSprite_Field(species, otId, personality, 8 * x + 40, 8 * y + 40);
    gSpriteTaskId = CreateTask(Task_ScriptShowMonPic, 80);

    gSprites[spriteId].hFlip = SPRITE_VIEW_DIRECTION;

    //Ajusta el sprite del pokémon 2 píxeles a la izquierda
    //Adjust the pokémon sprite 2 pixels to the left
    gSprites[spriteId].pos1.x -= 2;

#if SPRITE_JUMP == 1
    gSprites[spriteId].pos1.y -= 32;
    gSprites[spriteId].pos1.x += 48 * SPRITE_JUMP_DIRECTION;
#endif

    gTasks[gSpriteTaskId].data[0] = 0;
    gTasks[gSpriteTaskId].data[1] = species;
    gTasks[gSpriteTaskId].data[2] = spriteId;
    gTasks[gSpriteTaskId].data[3] = 0;
    gSprites[spriteId].callback = SpriteCallbackDummy;
    gSprites[spriteId].oam.priority = 0;
}

void Task_ScriptShowMonPic(u8 taskId)
{
    struct Task * task = &gTasks[taskId];

#if SPRITE_JUMP == 1
    task->data[3]++;
    if (task->data[3] > 0x7FF0)
        task->data[3] = 0xFF;
#endif

    switch (task->data[0])
    {
    case 0:
        task->data[0]++;
        break;
    case 1:

#if SPRITE_JUMP == 1
        if (task->data[3] < 10)
            gSprites[task->data[2]].pos1.x -= 1 * SPRITE_JUMP_DIRECTION;
        else if (task->data[3] < 18)
        {
            gSprites[task->data[2]].pos1.y += 1;
            gSprites[task->data[2]].pos1.x -= 1 * SPRITE_JUMP_DIRECTION;
        }
        else if (task->data[3] < 24)
        {
            gSprites[task->data[2]].pos1.y += 2;
            gSprites[task->data[2]].pos1.x -= 1 * SPRITE_JUMP_DIRECTION;
        }
        else if (task->data[3] < 28)
        {
            gSprites[task->data[2]].pos1.y += 3;
            gSprites[task->data[2]].pos1.x -= 1 * SPRITE_JUMP_DIRECTION;
        }
        else if (task->data[3] < 30)
        {
            gSprites[task->data[2]].pos1.y -= 3;
            gSprites[task->data[2]].pos1.x -= 1 * SPRITE_JUMP_DIRECTION;
        }
        else if (task->data[3] < 32)
        {
            gSprites[task->data[2]].pos1.y -= 2;
            gSprites[task->data[2]].pos1.x -= 1 * SPRITE_JUMP_DIRECTION;
        }
        else if (task->data[3] < 36)
        {
            gSprites[task->data[2]].pos1.y -= 1;
            gSprites[task->data[2]].pos1.x -= 1 * SPRITE_JUMP_DIRECTION;
        }
        else if (task->data[3] < 40)
            gSprites[task->data[2]].pos1.x -= 1 * SPRITE_JUMP_DIRECTION;
        else if (task->data[3] < 44)
        {
            gSprites[task->data[2]].pos1.y += 1;
            gSprites[task->data[2]].pos1.x -= 1 * SPRITE_JUMP_DIRECTION;
        }
        else if (task->data[3] < 46)
        {
            gSprites[task->data[2]].pos1.y += 2;
            gSprites[task->data[2]].pos1.x -= 1 * SPRITE_JUMP_DIRECTION;
        }
        else if (task->data[3] < 48)
        {
            gSprites[task->data[2]].pos1.y += 3;
            gSprites[task->data[2]].pos1.x -= 1 * SPRITE_JUMP_DIRECTION;
        }
#endif

        break;
    case 2:
        FreeResourcesAndDestroySprite(&gSprites[task->data[2]], task->data[2]);
        task->data[0]++;
        break;
    case 3:
        DestroyTask(taskId);
        break;
    }
}


/*

+- - - - - - - - - - - +
|PS       160  255  16 |  HP_Y
|ATAQUE   110   10  30 |  ATK_Y
|DEFENSA   65   20  31 |  DEF_Y
|ATA.ESP.  65    1  20 |  SPATK_Y
|DEF.ESP. 110   80  31 |  SPDEF_Y
|VELOCID.  30   11  13 |  SPEED_Y
+- - - - - - - - - - - +
|        |    |    |
 PS_X     BS_X EV_X IV_X

*/

#define PS_X        6
#define BS_X        51
#define EV_X        BS_X + 30
#define IV_X        EV_X + 30

#define HP_Y        3
#define ATK_Y       HP_Y + 14
#define DEF_Y       ATK_Y + 14
#define SPATK_Y     DEF_Y + 14
#define SPDEF_Y     SPATK_Y + 14
#define SPEED_Y     SPDEF_Y + 14

#define ABILITY_X        IV_X + 30
#define ABILITY_Y        SPEED_Y



//                                     resaltado                fuente                  sombra
//                                     highlight                font                    shadow
static const u8 gBlackTextColor[3]  = {TEXT_COLOR_TRANSPARENT,  TEXT_COLOR_DARK_GREY,   TEXT_COLOR_LIGHT_GREY};
static const u8 gBlueTextColor[3]   = {TEXT_COLOR_TRANSPARENT,  TEXT_COLOR_BLUE,        TEXT_COLOR_LIGHT_GREY};
static const u8 gRedTextColor[3]    = {TEXT_COLOR_TRANSPARENT,  TEXT_COLOR_RED,         TEXT_COLOR_LIGHT_GREY};
static const u8 gGrayTextColor[3]   = {TEXT_COLOR_TRANSPARENT,  TEXT_COLOR_LIGHT_GREY,  TEXT_COLOR_DARK_GREY};

static const u8 *const gTextColorByNature[] = 
{
    gBlackTextColor,
    gRedTextColor,
    gBlueTextColor
};


extern const u8 gText_Slash[];
extern const u8 gText_CensorEgg[];

extern const u8 gText_Bs[];
extern const u8 gText_Ev[];
extern const u8 gText_Iv[];

extern const u8 gText_Total[];

#ifdef ESP
extern const u8 gText_Hp[];
extern const u8 gText_Atq[];
extern const u8 gText_Def[];
extern const u8 gText_SpAtk[];
extern const u8 gText_SpDef[];
extern const u8 gText_Speed[];

extern const u8 gText_Your[];
extern const u8 gText_Is[];
extern const u8 gText_Happy[];

extern const u8 gText_Less_Than[];
extern const u8 gText_Steps_to_hatching[];
#else
// ENG
const u8 gText_Hp[]     = _("HP");
const u8 gText_Atq[]    = _("ATTACK");
const u8 gText_Def[]    = _("DEFENSE");
const u8 gText_SpAtk[]  = _("SP.ATK.");
const u8 gText_SpDef[]  = _("SP.DEF.");
const u8 gText_Speed[]  = _("SPEED");

const u8 gText_Your[]   = _("Your ");
const u8 gText_Is[]     = _(" is ");
const u8 gText_Happy[]  = _("% happy.");

const u8 gText_Less_Than[]  = _(" Less than ");
const u8 gText_Steps_to_hatching[]  = _(" steps to hatching!");
#endif

extern const u8 sGenderColors[2][3];
extern const u8 gText_MaleSymbol[];
extern const u8 gText_FemaleSymbol[];

static void PrintGenderText(struct Pokemon *mon)
{
    u8 genderSymbol[2];

    StringCopy(genderSymbol, gText_MaleSymbol);
    u8 gender = GetGenderFromSpeciesAndPersonality(mon->species, mon->personality);

    if (gender != MON_GENDERLESS)
    {
        if (gender == MON_FEMALE)
        {
            StringCopy(genderSymbol, gText_FemaleSymbol);
            gender = FEMALE;
        }
        AddTextPrinterParameterized3(WIN_POKEMON_NAME, 2, 0x94, 13, sGenderColors[gender], 0, genderSymbol);
    }
}

static void SandboxChangeGender(void)
{
    struct Pokemon *mon = &gPlayerParty[gCurrentMon];
	u16 species = GetMonData(mon, MON_DATA_SPECIES, NULL);
	u32 personality = GetMonData(mon, MON_DATA_PERSONALITY, NULL);
	u8 abilityNum = (personality & 1); //Flip ability bit

	u32 otId = GetMonData(mon, MON_DATA_OT_ID, NULL);
	u16 sid = HIHALF(otId);
	u16 tid = LOHALF(otId);

	u8 gender = GetGenderFromSpeciesAndPersonality(species, personality);
	bool8 isShiny = IsMonShiny(mon);
    u8 letter = GetUnownLetterFromPersonality(personality);
	u8 nature = GetNatureFromPersonality(personality);
	bool8 isMinior = IsMinior(species);
	u16 miniorCore = GetMiniorCoreFromPersonality(personality);

	//Change the ability while keeping other personality values the same
	do
	{
		personality = Random32();

		if (isShiny)
		{
			u8 shinyRange = 1;
			personality = (((shinyRange ^ (sid ^ tid)) ^ LOHALF(personality)) << 16) | LOHALF(personality);
		}

		personality &= ~(1);
		personality |= abilityNum; //Either 0 or 1

	} while (GetNatureFromPersonality(personality) != nature
	|| GetGenderFromSpeciesAndPersonality(species, personality) == gender
	|| (!isShiny && IsShinyOtIdPersonality(otId, personality)) //No free shinies
	|| (species == SPECIES_UNOWN && GetUnownLetterFromPersonality(personality) != letter)
	|| (isMinior && GetMiniorCoreFromPersonality(personality) != miniorCore));

	SetMonData(mon, MON_DATA_PERSONALITY, &personality);
	CalculateMonStats(mon);
    FillWindowPixelRect(WIN_POKEMON_NAME, PIXEL_FILL(0), 0x94, 15, 6, 10);
    PrintGenderText(mon);
}

static void MiniEvIvPrintText(struct Pokemon *mon, bool8 ev, u8 stat, u8 newValue, u8 stat2, bool8 fixev)
{
    u8 nature   = GetNature(mon);
    u8 arrStat;
    if(!fixev)
    {
        switch(stat)
        {
            case MON_DATA_SPATK_EV:
            case MON_DATA_SPATK_IV:
                arrStat = STAT_SPATK;
                break;
            case MON_DATA_SPDEF_EV:
            case MON_DATA_SPDEF_IV:
                arrStat = STAT_SPDEF;
                break;
            case MON_DATA_SPEED_EV:
            case MON_DATA_SPEED_IV:
                arrStat = STAT_SPEED;
                break;
            default:
                arrStat = STAT_HP + stat2;
                break;
        }
    }
    else
    {

    }
    if(ev)
    {
        gTotalStatsEV = 0;
        gStats_ev[arrStat] = newValue;
        for (int i = 0; i < NUM_STATS; i++)
            gTotalStatsEV += gStats_ev[i];
    }
    else
    {
        gTotalStatsIV = 0;
        gStats_iv[arrStat] = newValue;
        for (int i = 0; i < NUM_STATS; i++)
            gTotalStatsIV += gStats_iv[i];
    }
    //FillWindowPixelBuffer(0, 0);
    /*FillWindowPixelBuffer(1, 0);
    FillWindowPixelBuffer(2, 0);

    //PrintWindow0(mon);
    PrintWindow1(nature, isEgg);
    PrintWindow2(species, isEgg, friendship, ability);*/
    ConvertIntToDecimalStringN(gStringVar2, gStats_ev[arrStat], STR_CONV_MODE_RIGHT_ALIGN, 3);
    ConvertIntToDecimalStringN(gStringVar3, gStats_iv[arrStat], STR_CONV_MODE_RIGHT_ALIGN, 2);
    u8 color = GetColorByNature(nature, arrStat);
    switch(arrStat)
    {
        case STAT_HP:
            FillWindowPixelRect(WIN_STATS, PIXEL_FILL(0), (ev) ? EV_X : IV_X, HP_Y, 22, 12);
            AddTextPrinterParameterized3(WIN_STATS, 2, (ev) ? EV_X : IV_X, HP_Y, gTextColorByNature[color], 0, (ev) ? gStringVar2 : gStringVar3);
            break;
        case STAT_ATK:
            FillWindowPixelRect(WIN_STATS, PIXEL_FILL(0), (ev) ? EV_X : IV_X, ATK_Y, 22, 12);
            AddTextPrinterParameterized3(WIN_STATS, 2, (ev) ? EV_X : IV_X, ATK_Y, gTextColorByNature[color], 0, (ev) ? gStringVar2 : gStringVar3);
            break;
        case STAT_DEF:
            FillWindowPixelRect(WIN_STATS, PIXEL_FILL(0), (ev) ? EV_X : IV_X, DEF_Y, 22, 12);
            AddTextPrinterParameterized3(WIN_STATS, 2, (ev) ? EV_X : IV_X, DEF_Y, gTextColorByNature[color], 0, (ev) ? gStringVar2 : gStringVar3);
            break;
        case STAT_SPATK:
            FillWindowPixelRect(WIN_STATS, PIXEL_FILL(0), (ev) ? EV_X : IV_X, SPATK_Y, 22, 12);
            AddTextPrinterParameterized3(WIN_STATS, 2, (ev) ? EV_X : IV_X, SPATK_Y, gTextColorByNature[color], 0, (ev) ? gStringVar2 : gStringVar3);
            break;
        case STAT_SPDEF:
            FillWindowPixelRect(WIN_STATS, PIXEL_FILL(0), (ev) ? EV_X : IV_X, SPDEF_Y, 22, 12);
            AddTextPrinterParameterized3(WIN_STATS, 2, (ev) ? EV_X : IV_X, SPDEF_Y, gTextColorByNature[color], 0, (ev) ? gStringVar2 : gStringVar3);
            break;
        case STAT_SPEED:
            FillWindowPixelRect(WIN_STATS, PIXEL_FILL(0), (ev) ? EV_X : IV_X, SPEED_Y, 22, 12);
            AddTextPrinterParameterized3(WIN_STATS, 2, (ev) ? EV_X : IV_X, SPEED_Y, gTextColorByNature[color], 0, (ev) ? gStringVar2 : gStringVar3);
            break;
    }

    ConvertIntToDecimalStringN(gStringVar2, gTotalStatsEV, STR_CONV_MODE_RIGHT_ALIGN, 3);
    ConvertIntToDecimalStringN(gStringVar3, gTotalStatsIV, STR_CONV_MODE_RIGHT_ALIGN, 3);
    FillWindowPixelRect(WIN_BOTTOM_BOX, PIXEL_FILL(0), (ev) ? EV_X + 8 : IV_X + 2, 4, 18, 13);
    if(ev)
    {
        AddTextPrinterParameterized3(WIN_BOTTOM_BOX, 2, EV_X + 8, 4, gBlackTextColor, 0, gStringVar2);
    }
    else
    {
        AddTextPrinterParameterized3(WIN_BOTTOM_BOX, 2, IV_X + 2, 4, gBlackTextColor, 0, gStringVar3);    
    }

    //PutWindowTilemap(0);
    PutWindowTilemap(1);
    PutWindowTilemap(2);
}

static void EvIvPrintText(struct Pokemon *mon)
{
    u16 species = GetMonData(mon, MON_DATA_SPECIES, 0);
    u8 nature   = GetNature(mon);
    u8 isEgg    = GetMonData(mon, MON_DATA_IS_EGG, 0);
    u8 friendship = GetMonData(mon, MON_DATA_FRIENDSHIP, 0);
    u8 ability = GetMonAbility(mon);

    //reinicia los totales.
    //reset the totals.
    gTotalStatsEV = 0;
    gTotalStatsIV = 0;
    gTotalStatsBS = 0;

    //obtiene las estadísticas del pokémon
    //get pokémon stats

    //STAT_HP
    gStats_bs[STAT_HP] = GetMonData(mon, MON_DATA_HP, 0);
    gStats_ev[STAT_HP] = GetMonData(mon,MON_DATA_HP_EV, 0);
    gStats_iv[STAT_HP] = GetMonData(mon,MON_DATA_HP_IV, 0);
    //STAT_ATK
    gStats_bs[STAT_ATK] = GetVisualBaseStat(STAT_ATK, species);
    gStats_ev[STAT_ATK] = GetMonData(mon,MON_DATA_ATK_EV,0);
    gStats_iv[STAT_ATK] = GetMonData(mon,MON_DATA_ATK_IV,0);
    //STAT_DEF
    gStats_bs[STAT_DEF] = GetVisualBaseStat(STAT_DEF, species);
    gStats_ev[STAT_DEF] = GetMonData(mon,MON_DATA_DEF_EV,0);
    gStats_iv[STAT_DEF] = GetMonData(mon,MON_DATA_DEF_IV,0);
    //STAT_SPATK
    gStats_bs[STAT_SPATK] = GetVisualBaseStat(STAT_SPATK, species);
    gStats_ev[STAT_SPATK] = GetMonData(mon,MON_DATA_SPATK_EV,0);
    gStats_iv[STAT_SPATK] = GetMonData(mon,MON_DATA_SPATK_IV,0);
    //STAT_SPDEF
    gStats_bs[STAT_SPDEF] = GetVisualBaseStat(STAT_SPDEF, species);
    gStats_ev[STAT_SPDEF] = GetMonData(mon,MON_DATA_SPDEF_EV,0);
    gStats_iv[STAT_SPDEF] = GetMonData(mon,MON_DATA_SPDEF_IV,0);
    //STAT_SPEED
    gStats_bs[STAT_SPEED] = GetVisualBaseStat(STAT_SPEED, species);
    gStats_ev[STAT_SPEED] = GetMonData(mon,MON_DATA_SPEED_EV,0);
    gStats_iv[STAT_SPEED] = GetMonData(mon,MON_DATA_SPEED_IV,0);

    //realiza la suma de los totales
    //performs the sum of the totals
    for (int i = 0; i < NUM_STATS; i++)
    {
        gTotalStatsEV += gStats_ev[i];
        gTotalStatsIV += gStats_iv[i];
        gTotalStatsBS += gStats_bs[i];
    }

    FillWindowPixelBuffer(0, 0);
    FillWindowPixelBuffer(1, 0);
    FillWindowPixelBuffer(2, 0);

    PrintWindow0(mon);
    PrintWindow1(nature, isEgg);
    PrintWindow2(species, isEgg, friendship, ability);

    PutWindowTilemap(0);
    PutWindowTilemap(1);
    PutWindowTilemap(2);
}

static void PrintWindow0(struct Pokemon *mon)
{
    u8 temp = 0;

    temp = gCurrentMon + 1;
    ConvertIntToDecimalStringN(gStringVar4, temp, STR_CONV_MODE_LEFT_ALIGN, 1);
    StringAppend(gStringVar4, gText_Slash);
    temp = gPlayerPartyCount;
    ConvertIntToDecimalStringN(gStringVar1, temp, STR_CONV_MODE_LEFT_ALIGN, 1);
    StringAppend(gStringVar4, gStringVar1);
    AddTextPrinterParameterized3(WIN_POKEMON_NAME, 2, 2, 2, gGrayTextColor, 0, gStringVar4);

    //AddTextPrinterParameterized3(WIN_POKEMON_NAME, 2, 0x44, 2, gGrayTextColor, 0, gText_BsEvIv);

    GetMonNickname(mon, gStringVar4);
    AddTextPrinterParameterized3(WIN_POKEMON_NAME, 2, 0x94, 2, gBlackTextColor, 0, gStringVar4);
}

static void PrintWindow1(u8 nature, u8 isEgg)
{
    /*AddTextPrinterParameterized3(WIN_STATS, 2, PS_X, HP_Y,    gTextColorByNature[GetColorByNature(nature, STAT_HP)],    0, gText_Hp);
    AddTextPrinterParameterized3(WIN_STATS, 2, PS_X, ATK_Y,   gTextColorByNature[GetColorByNature(nature, STAT_ATK)],   0, gText_Atq);
    AddTextPrinterParameterized3(WIN_STATS, 2, PS_X, DEF_Y,   gTextColorByNature[GetColorByNature(nature, STAT_DEF)],   0, gText_Def);
    AddTextPrinterParameterized3(WIN_STATS, 2, PS_X, SPATK_Y, gTextColorByNature[GetColorByNature(nature, STAT_SPATK)], 0, gText_SpAtk);
    AddTextPrinterParameterized3(WIN_STATS, 2, PS_X, SPDEF_Y, gTextColorByNature[GetColorByNature(nature, STAT_SPDEF)], 0, gText_SpDef);
    AddTextPrinterParameterized3(WIN_STATS, 2, PS_X, SPEED_Y, gTextColorByNature[GetColorByNature(nature, STAT_SPEED)], 0, gText_Speed);*/

    if (!isEgg)
    {
        PrintStat(nature, STAT_HP);
        PrintStat(nature, STAT_ATK);
        PrintStat(nature, STAT_DEF);
        PrintStat(nature, STAT_SPATK);
        PrintStat(nature, STAT_SPDEF);
        PrintStat(nature, STAT_SPEED);
    }else{
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X, HP_Y,    gBlackTextColor, 0, gText_CensorEgg);
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X, ATK_Y,   gBlackTextColor, 0, gText_CensorEgg);
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X, DEF_Y,   gBlackTextColor, 0, gText_CensorEgg);
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X, SPATK_Y, gBlackTextColor, 0, gText_CensorEgg);
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X, SPDEF_Y, gBlackTextColor, 0, gText_CensorEgg);
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X, SPEED_Y, gBlackTextColor, 0, gText_CensorEgg);
    }
}

static void PrintStat(u8 nature, u8 stat)
{
    u8 color = GetColorByNature(nature, stat);

    ConvertIntToDecimalStringN(gStringVar1, gStats_bs[stat], STR_CONV_MODE_RIGHT_ALIGN, 3);
    ConvertIntToDecimalStringN(gStringVar2, gStats_ev[stat], STR_CONV_MODE_RIGHT_ALIGN, 3);
    ConvertIntToDecimalStringN(gStringVar3, gStats_iv[stat], STR_CONV_MODE_RIGHT_ALIGN, 2);

    switch (stat)
    {
    case STAT_HP:
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X, HP_Y, gTextColorByNature[color], 0, gStringVar1);
        AddTextPrinterParameterized3(WIN_STATS, 2, EV_X, HP_Y, gTextColorByNature[color], 0, gStringVar2);
        AddTextPrinterParameterized3(WIN_STATS, 2, IV_X, HP_Y, gTextColorByNature[color], 0, gStringVar3);
        break;
    case STAT_ATK:
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X, ATK_Y, gTextColorByNature[color], 0, gStringVar1);
        AddTextPrinterParameterized3(WIN_STATS, 2, EV_X, ATK_Y, gTextColorByNature[color], 0, gStringVar2);
        AddTextPrinterParameterized3(WIN_STATS, 2, IV_X, ATK_Y, gTextColorByNature[color], 0, gStringVar3);
        break;
    case STAT_DEF:
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X, DEF_Y, gTextColorByNature[color], 0, gStringVar1);
        AddTextPrinterParameterized3(WIN_STATS, 2, EV_X, DEF_Y, gTextColorByNature[color], 0, gStringVar2);
        AddTextPrinterParameterized3(WIN_STATS, 2, IV_X, DEF_Y, gTextColorByNature[color], 0, gStringVar3);
        break;
    case STAT_SPATK:
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X, SPATK_Y, gTextColorByNature[color], 0, gStringVar1);
        AddTextPrinterParameterized3(WIN_STATS, 2, EV_X, SPATK_Y, gTextColorByNature[color], 0, gStringVar2);
        AddTextPrinterParameterized3(WIN_STATS, 2, IV_X, SPATK_Y, gTextColorByNature[color], 0, gStringVar3);
        break;
    case STAT_SPDEF:
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X, SPDEF_Y, gTextColorByNature[color], 0, gStringVar1);
        AddTextPrinterParameterized3(WIN_STATS, 2, EV_X, SPDEF_Y, gTextColorByNature[color], 0, gStringVar2);
        AddTextPrinterParameterized3(WIN_STATS, 2, IV_X, SPDEF_Y, gTextColorByNature[color], 0, gStringVar3);
        break;
    case STAT_SPEED:
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X, SPEED_Y, gTextColorByNature[color], 0, gStringVar1);
        AddTextPrinterParameterized3(WIN_STATS, 2, EV_X, SPEED_Y, gTextColorByNature[color], 0, gStringVar2);
        AddTextPrinterParameterized3(WIN_STATS, 2, IV_X, SPEED_Y, gTextColorByNature[color], 0, gStringVar3);
        break;
    default:
        break;
    }
}

static void PrintWindow2(u16 species, u8 isEgg, u8 friendship, u8 ability)
{
    u16 temp = 0;

    if(!isEgg)
    {
        AddTextPrinterParameterized3(WIN_BOTTOM_BOX, 2, 12, 4, gBlackTextColor, 0, gText_Total);

        ConvertIntToDecimalStringN(gStringVar1, gTotalStatsBS, STR_CONV_MODE_RIGHT_ALIGN, 3);
        ConvertIntToDecimalStringN(gStringVar2, gTotalStatsEV, STR_CONV_MODE_RIGHT_ALIGN, 3);
        ConvertIntToDecimalStringN(gStringVar3, gTotalStatsIV, STR_CONV_MODE_RIGHT_ALIGN, 3);
        AddTextPrinterParameterized3(WIN_BOTTOM_BOX, 2, BS_X + 8, 4, gBlackTextColor, 0, gStringVar1);
        AddTextPrinterParameterized3(WIN_BOTTOM_BOX, 2, EV_X + 8, 4, gBlackTextColor, 0, gStringVar2);
        AddTextPrinterParameterized3(WIN_BOTTOM_BOX, 2, IV_X + 2, 4, gBlackTextColor, 0, gStringVar3); 

        StringCopy(gStringVar1, gText_Bs);
        StringCopy(gStringVar2, gText_Ev);
        StringCopy(gStringVar3, gText_Iv);
        AddTextPrinterParameterized3(WIN_POKEMON_NAME, 2, BS_X + 8, 4, gBlackTextColor, 0, gStringVar1);
        AddTextPrinterParameterized3(WIN_POKEMON_NAME, 2, EV_X + 8, 4, gBlackTextColor, 0, gStringVar2);
        AddTextPrinterParameterized3(WIN_POKEMON_NAME, 2, IV_X + 2, 4, gBlackTextColor, 0, gStringVar3);

        StringCopy(gStringVar4, gText_Your);
        GetSpeciesName(gStringVar1, species);
        StringAppend(gStringVar4, gStringVar1);
        StringAppend(gStringVar4, gText_Is);
        temp = friendship;
        
        ConvertIntToDecimalStringN(gStringVar2, temp, STR_CONV_MODE_LEFT_ALIGN, 3);
        StringAppend(gStringVar4, gStringVar2);
        
        StringAppend(gStringVar4, gText_Happy);
        AddTextPrinterParameterized3(WIN_BOTTOM_BOX, 2, 12, 18, gBlackTextColor, 0, gStringVar4);

        CopyAbilityName(gStringVar1, ability, species);
        AddTextPrinterParameterized3(WIN_BOTTOM_BOX, 2, ABILITY_X, 4, gBlackTextColor, 0, gStringVar1);
        
    }else
    {
        StringCopy(gStringVar4, gText_Less_Than);
        temp = (friendship + 1) * 0xFF;
        ConvertIntToDecimalStringN(gStringVar2, temp, STR_CONV_MODE_LEFT_ALIGN, GetDigits(temp));
        StringAppend(gStringVar4, gStringVar2);
        StringAppend(gStringVar4, gText_Steps_to_hatching);
        AddTextPrinterParameterized3(WIN_BOTTOM_BOX, 2, 2, 18, gBlackTextColor, 0, gStringVar4);
    }
}

static void ClearAndPrintAbilityText(u8 ability, u16 species)
{
    FillWindowPixelRect(WIN_BOTTOM_BOX, PIXEL_FILL(0), ABILITY_X, 4, 91, 15);
    CopyAbilityName(gStringVar1, ability, species);
    AddTextPrinterParameterized3(WIN_BOTTOM_BOX, 2, ABILITY_X, 4, gBlackTextColor, 0, gStringVar1);
}

/**
 * Devuelve el número de dígitos de un número menor a 65536
 * 
 * Returns the number of digits in a number less than 65536
*/
static u8 GetDigits(u16 num)
{
    if (num < 10)
        return 1;
    else if (num < 100)
        return 2;
    else if (num < 1000)
        return 3;
    else if (num < 10000)
        return 4;
    else
        return 5;
}

/**
 * Esta función devuelve un número, de acuerdo a la naturaleza.
 * 0 = negro
 * 1 = rojo
 * 2 = azul
 * 
 * This function returns a number, according to nature.
 * 0 = black
 * 1 = red
 * 2 = blue
*/
static u8 GetColorByNature(u8 nature, u8 stat)
{
    switch (stat)
    {
    case STAT_ATK:
        switch (nature)
        {
        case NATURE_LONELY:
        case NATURE_BRAVE:
        case NATURE_ADAMANT:
        case NATURE_NAUGHTY:
            return 1;
        case NATURE_BOLD:
        case NATURE_TIMID:
        case NATURE_MODEST:
        case NATURE_CALM:
            return 2;
        
        default:
            return 0;
        }
    case STAT_DEF:
        switch (nature)
        {
        case NATURE_BOLD:
        case NATURE_RELAXED:
        case NATURE_IMPISH:
        case NATURE_LAX:
            return 1;
        case NATURE_LONELY:
        case NATURE_HASTY:
        case NATURE_MILD:
        case NATURE_GENTLE:
            return 2;
        
        default:
            return 0;
        }
    case STAT_SPATK:
        switch (nature)
        {
        case NATURE_MODEST:
        case NATURE_MILD:
        case NATURE_QUIET:
        case NATURE_RASH:
            return 1;
        case NATURE_ADAMANT:
        case NATURE_IMPISH:
        case NATURE_JOLLY:
        case NATURE_CAREFUL:
            return 2;
        
        default:
            return 0;
        }
    case STAT_SPDEF:
        switch (nature)
        {
        case NATURE_CALM:
        case NATURE_GENTLE:
        case NATURE_SASSY:
        case NATURE_CAREFUL:
            return 1;
        case NATURE_NAUGHTY:
        case NATURE_LAX:
        case NATURE_NAIVE:
        case NATURE_RASH:
            return 2;
        
        default:
            return 0;
        }
    case STAT_SPEED:
        switch (nature)
        {
        case NATURE_TIMID:
        case NATURE_HASTY:
        case NATURE_JOLLY:
        case NATURE_NAIVE:
            return 1;
        case NATURE_BRAVE:
        case NATURE_RELAXED:
        case NATURE_QUIET:
        case NATURE_SASSY:
            return 2;
        
        default:
            return 0;
        }
    case STAT_HP:
    default:
        return 0;
    }
}
