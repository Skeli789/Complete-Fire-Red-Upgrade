/* Custom File. Give credits if used!
   ~ 1RWT16KU1D
*/

// For Terastallization
#include "defines.h"
#include "defines_battle.h"

#include "../include/battle_message.h"
#include "../include/battle.h"
#include "../include/event_data.h"
#include "../include/item.h"
#include "../include/new/item.h"
#include "../include/mgba.h"
#include "../include/palette.h"
#include "../include/pokemon.h"
#include "../include/pokemon_summary_screen.h"

#include "../include/constants/items.h"
#include "../include/constants/vars.h"

#include "../include/gba/macro.h"

#include "../include/new/ai_util.h"
#include "../include/new/battle_indicators.h"
#include "../include/new/battle_script_util.h"
#include "../include/new/dynamax.h"
#include "../include/new/frontier.h"
#include "../include/new/mega.h"
#include "../include/new/move_battle_scripts.h"
#include "../include/new/ram_locs.h"
#include "../include/new/terastallization.h"
#include "../include/field_weather.h"
#include "../include/random.h"
#include "../include/string_util.h"

// Exported Functions
extern u16 VarGet(u16 var);
extern bool8 VarSet(u16 var, u16 value);

// BattleScript(s)
extern u8 BattleScript_Terastallize[];

// Bag Helper Functions
extern u8 GetPocketByItemId(u16 itemId);
extern u8* GetBagItemQuantityPointer(u8 pocket, u8 bagId);
extern u16* GetBagItemsPointer(u8 pocket, u8 bagId);

#define TRAINER_ITEM_COUNT 4

static const item_t sTeraOrbTable[] =
{
	ITEM_TERA_ORB,
};

const u16 gTeraBlendColors[] =
{
    [TYPE_NORMAL]   = RGB(25, 25, 25),  // Light Gray
    [TYPE_FIGHTING] = RGB(31, 0, 12),    // Crimson Red
    [TYPE_FLYING]   = RGB(18, 22, 31),  // Sky Blue
    [TYPE_POISON]   = RGB(22, 2, 28),   // Deep Violet
    [TYPE_GROUND]   = RGB(16, 7, 1),    // Deep Brown
    [TYPE_ROCK]     = RGB(15, 13, 6),   // Dusty Beige
    [TYPE_BUG]      = RGB(18, 25, 6),   // Leafy Green
    [TYPE_GHOST]    = RGB(10, 6, 16),   // Haunting Indigo
    [TYPE_STEEL]    = RGB(19, 19, 23),  // Bluish Steel Gray
    [TYPE_MYSTERY]  = RGB(31, 31, 31),  // White
    [TYPE_FIRE]     = RGB(30, 12, 4),   // Fiery Orange
    [TYPE_GRASS]    = RGB(6, 26, 8),    // Forest Green
    [TYPE_WATER]    = RGB(8, 17, 31),   // Watery Blue?
    [TYPE_ELECTRIC] = RGB(31, 28, 5),   // Vivid Yellow
    [TYPE_PSYCHIC]  = RGB(31, 6, 18),   // Magenta
    [TYPE_ICE]      = RGB(20, 28, 31),  // Ice Blue
    [TYPE_DRAGON]   = RGB(8, 10, 31),   // Indigo
    [TYPE_DARK]     = RGB(5, 5, 5),     // Black
    [TYPE_FAIRY]    = RGB(31, 18, 24),  // Rose Pink
    [TYPE_BLANK]    = RGB(31, 31, 31),  // White
    [TYPE_STELLAR]  = RGB(28, 28, 31),  // Still unsure - Gave it a Silvery Blue Tint
};

// Check if the Pokemon has terastallized or not
bool8 IsTerastallized(u8 bank)
{
    return gNewBS->teraData.done[GetBattlerSide(bank)][gBattlerPartyIndexes[bank]];
}

// Fetch the Pokemon's current Tera Type
u8 GetTeraType(u8 bank)
{
    struct Pokemon *mon;

    if (GetBattlerSide(bank) == B_SIDE_PLAYER)
        mon = &gPlayerParty[gBattlerPartyIndexes[bank]];
    else
        mon = &gEnemyParty[gBattlerPartyIndexes[bank]];

    return mon->teraType;
}

// Fetch the Pokemon's Tera Type from OW (scripts)
void GetTeraTypeInOW(void)
{
    u8 partySlot = VarGet(Var8002); // Get Pokemon's index from script

    // Ensure Slot is Valid (0-5)
    if (partySlot >= PARTY_SIZE)
    {
        VarSet(VAR_TEMP_1, TYPE_BLANK); // Set to Blank to indicate an error
        return;
    }

    // Fetch Correct Pokemon's Tera Type
    u8 monTeraType = gPlayerParty[partySlot].teraType;

    // Ensure Tera Type is within range
    VarSet(VAR_TEMP_1, (monTeraType < NUMBER_OF_MON_TYPES) ? monTeraType : TYPE_BLANK);
}

// Change the Pokemon's Tera type in OW
void ChangeTeraTypeInOW(void)
{
    u8 partySlot = VarGet(Var8002);   // Get Pokemon's party index from script
    u8 newTeraType = VarGet(Var8001); // Get new Tera Type from script

    // Error Checks
    if (partySlot >= PARTY_SIZE)
        return;

    if (newTeraType >= NUMBER_OF_MON_TYPES)
        return;

    gPlayerParty[partySlot].teraType = newTeraType;
}

// Fades palette according to teraType
void FadeBankPaletteForTera(u8 bank, u16 paletteOffset)
{
    u8 teraType = GetTeraType(bank);

	BlendPalette(paletteOffset, 16, 4, gTeraBlendColors[teraType]);
	CpuCopy32(gPlttBufferFaded + paletteOffset, gPlttBufferUnfaded + paletteOffset, 32);
}

static const u8 *const sTypeNames[NUMBER_OF_MON_TYPES] =
{
    [TYPE_NORMAL]   = sText_Normal,
    [TYPE_FIRE]     = sText_Fire,
    [TYPE_WATER]    = sText_Water,
    [TYPE_ELECTRIC] = sText_Electric,
    [TYPE_GRASS]    = sText_Grass,
    [TYPE_ICE]      = sText_Ice,
    [TYPE_FIGHTING] = sText_Fighting,
    [TYPE_POISON]   = sText_Poison,
    [TYPE_GROUND]   = sText_Ground,
    [TYPE_FLYING]   = sText_Flying,
    [TYPE_PSYCHIC]  = sText_Psychic,
    [TYPE_BUG]      = sText_Bug,
    [TYPE_ROCK]     = sText_Rock,
    [TYPE_GHOST]    = sText_Ghost,
    [TYPE_DRAGON]   = sText_Dragon,
    [TYPE_DARK]     = sText_Dark,
    [TYPE_STEEL]    = sText_Steel,
    [TYPE_FAIRY]    = sText_Fairy,
    [TYPE_STELLAR]   = sText_Stellar,
};

// Main Function - Try type changes
u8 *DoTerastallize(u8 bank)
{
    if (!IsTerastallized(bank))
    {
        u8 teraType = GetTeraType(bank);
        u8 partyIndex = gBattlerPartyIndexes[bank];
        struct Pokemon *mon;

        if (GetBattlerSide(bank) == B_SIDE_PLAYER)
            mon = &gPlayerParty[partyIndex];
        else
            mon = &gEnemyParty[partyIndex];

        u16 species = GetMonData(mon, MON_DATA_SPECIES, NULL);

        gBattleScripting.bank = bank;

        // Because Stellar Tera Defensive Typing remains same
        if (teraType != TYPE_STELLAR)
            SET_BATTLER_TYPE(bank, teraType);
        GetSpeciesName(gStringVar1, species);
        StringCopy(gStringVar2, sTypeNames[teraType]);

        return BattleScript_Terastallize;
    }
    return NULL;
}

// AI Logic for Terastallization
bool8 ShouldAIDelayTerastallization(u8 bankAtk, u8 bankDef, u16 move, bool8 optimizeAndLookAtTeraPotential, bool8 runDamageCalcs)
{
    if (optimizeAndLookAtTeraPotential && !CanTerastallize(bankAtk))
        return TRUE;

    if (IsTerastallized(bankAtk)) // Is already Terastallized
        return FALSE;

    if (GetTeraType(bankAtk) == TYPE_BLANK)
        return TRUE;

    if (BATTLER_SEMI_INVULNERABLE(bankAtk))
        return TRUE; // Can't Terastallize this turn

    if (runDamageCalcs)
    {
        // Delay Terastallization if we can KO the opponent without it
        if (MoveWouldHitFirst(move, bankAtk, bankDef) // AI would attack first
        && CalculateMoveKnocksOutXHitsFresh(move, bankAtk, bankDef, 1)) // AI would KO in its base form
            return TRUE;

        // Delay Terastallization if opponent can KO us after we Terastallize
        if (!MoveWouldHitFirst(move, bankAtk, bankDef) // AI wouldn't attack first
        && MoveKnocksOutXHits(IsValidMovePrediction(bankDef, bankAtk), bankDef, bankAtk, 1)) // And foe would KO AI
            return TRUE;
    }
    return FALSE;
}

// Check whether Pokemon can Tera
bool8 CanTerastallize(u8 bank)
{
    #ifndef TERASTAL_FEATURE
		return FALSE;
	#else

    // Terastallization disabled in Dynamax battles
    if (gBattleTypeFlags & BATTLE_TYPE_DYNAMAX)
        return FALSE; 

    if (GetBattlerSide(bank) == B_SIDE_OPPONENT)
        return TRUE;
    else
    {
        if (FlagGet(FLAG_TERA_BATTLE) && !IsTerastallized(bank))
            return TRUE;

        return FALSE;
    }
    #endif
}

// For NPC trainers
static bool8 IsItemTeraOrb(u16 item)
{
	for (u8 i = 0; i < ARRAY_COUNT(sTeraOrbTable); ++i)
	{
		if (item == sTeraOrbTable[i])
			return TRUE;
	}

	return FALSE;
}

// Check if NPC has Tera Orb in item slot
static item_t FindTrainerTeraOrb(u16 trainerId)
{
    // Don't require Tera Orb for these battles
	if (gBattleTypeFlags & (BATTLE_TYPE_FRONTIER | BATTLE_TYPE_LINK) || IsFrontierTrainerId(trainerId))
		return ITEM_TERA_ORB;

    // Normal Battles
	for (u8 i = 0; i < TRAINER_ITEM_COUNT; ++i)
	{
		if (IsItemTeraOrb(gTrainers[trainerId].items[i]))
			return gTrainers[trainerId].items[i];
	}

    // Fallback
	return ITEM_NONE;
}

// Check if player has Tera Orb in bag
static item_t FindPlayerTeraOrb(void)
{
    // Don't require Tera Orb for these battles
	if (gBattleTypeFlags & (BATTLE_TYPE_FRONTIER | BATTLE_TYPE_LINK))
		return ITEM_TERA_ORB;

    // Normal battles
	for (u8 i = 0; i < ARRAY_COUNT(sTeraOrbTable); ++i)
	{
		if (CheckBagHasItem(sTeraOrbTable[i], 1))
			return sTeraOrbTable[i];
	}

	#ifdef DEBUG_TERASTAL
		return ITEM_TERA_ORB;
	#endif

    // Fallback
	return ITEM_NONE;
}

// Used for AI
static item_t FindBankTeraOrb(u8 bank)
{
	#ifdef DEBUG_TERASTAL
		if (bank + 1)
			return ITEM_TERA_ORB;
	#endif

	if (SIDE(bank) == SIDE_OPPONENT)
	{
		if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS)
		{
			if (GetBattlerPosition(bank) == B_POSITION_OPPONENT_LEFT)
				return FindTrainerTeraOrb(gTrainerBattleOpponent_A);
			else
				return FindTrainerTeraOrb(SECOND_OPPONENT);
		}
		else
			return FindTrainerTeraOrb(gTrainerBattleOpponent_A);
	}
	else //SIDE_PLAYER
	{
		if (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER)
		{
			if (GetBattlerPosition(bank) == B_POSITION_PLAYER_RIGHT)
				return FindTrainerTeraOrb(VarGet(VAR_PARTNER));
			else
				return FindPlayerTeraOrb();
		}
		else
			return FindPlayerTeraOrb();
	}
}

// Check for both
bool8 TerastalEnabled(u8 bank)
{
    // Terastallization disabled in Dynamax battles
    if (gBattleTypeFlags & BATTLE_TYPE_DYNAMAX)
        return FALSE;

    // Opponents don't rely on held Tera Orbs
    if (GetBattlerSide(bank) == B_SIDE_OPPONENT)
    {
        // Wild Battle check
        if (!((gBattleTypeFlags & (BATTLE_TYPE_TRAINER | BATTLE_TYPE_EREADER_TRAINER | BATTLE_TYPE_TRAINER_TOWER)) == BATTLE_TYPE_TRAINER)
        ||   (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER))     
            return FALSE;

        if (!FlagGet(FLAG_TERA_BATTLE))
            return FALSE;

        return TRUE; // Allow tera
    }

    // The rest of the code assumes B_SIDE_PLAYER
    if (!FlagGet(FLAG_TERA_BATTLE))
        return FALSE;

    // Only one gimmick allowed - Mega and Z take precedence
    if (CanMegaEvolve(bank, FALSE) || CanMegaEvolve(bank, TRUE) || HasMegaSymbol(bank))
        return FALSE;

    if (IsZCrystal(ITEM(bank)))
        return FALSE;

    // Can't Terastallize if this mon is Dynamaxing
    if (IsDynamaxed(bank)
    || gNewBS->dynamaxData.used[bank]
    || gNewBS->dynamaxData.toBeUsed[bank])
        return FALSE;

    if (FindBankTeraOrb(bank) != ITEM_NONE)
        return TRUE;

    #ifdef DEBUG_TERASTAL
        return TRUE;
    #else
        return FALSE;
    #endif
}

// Get a random teraType
u8 GetRandomTeraType(void)
{
    u8 randomType;

    // Reroll if invalid type
    do randomType = Random() % NUMBER_OF_MON_TYPES;
    while ((randomType == TYPE_BLANK) || (randomType == TYPE_MYSTERY) || (randomType == TYPE_BLANK)
            || (randomType == 0x12) || (randomType == 0x16) || (randomType == TYPE_ROOSTLESS));

    return randomType;
}

// givepokemon set-up
void SetGiftMonTeraType(void)
{
    u8 partySlot = VarGet(Var8002); // Slot passed from script
    if (partySlot >= PARTY_SIZE)
        return;

    struct Pokemon* mon = &gPlayerParty[partySlot];
    u16 species = mon->species;

    u8 type1 = gBaseStats[species].type1;
    u8 type2 = gBaseStats[species].type2;

    if (type1 == type2 || type2 == TYPE_MYSTERY)
        mon->teraType = type1;
    else
        mon->teraType = (Random() & 1 ) ? type1 : type2;
}

#ifdef SHOW_TERA_TYPE_ICON_ON_SUMMARY_SCREEN
enum {
    TAG_TERA_TYPE_ICON_NORMAL = 0x3000,
    TAG_TERA_TYPE_ICON_FIGHTING,
    TAG_TERA_TYPE_ICON_FLYING,
    TAG_TERA_TYPE_ICON_POISON,
    TAG_TERA_TYPE_ICON_GROUND,
    TAG_TERA_TYPE_ICON_ROCK,
    TAG_TERA_TYPE_ICON_BUG,
    TAG_TERA_TYPE_ICON_GHOST,
    TAG_TERA_TYPE_ICON_STEEL,
    TAG_TERA_TYPE_ICON_FIRE,
    TAG_TERA_TYPE_ICON_WATER,
    TAG_TERA_TYPE_ICON_GRASS,
    TAG_TERA_TYPE_ICON_ELECTRIC,
    TAG_TERA_TYPE_ICON_PSYCHIC,
    TAG_TERA_TYPE_ICON_ICE,
    TAG_TERA_TYPE_ICON_DRAGON,
    TAG_TERA_TYPE_ICON_DARK,
    TAG_TERA_TYPE_ICON_FAIRY,
    TAG_TERA_TYPE_ICON_STELLAR,
};

extern const u8 gTeraTypeIcon_NormalTiles[];
extern const u16 gTeraTypeIcon_NormalPal[];
extern const u8 gTeraTypeIcon_FightingTiles[];
extern const u8 gTeraTypeIcon_FlyingTiles[];
extern const u8 gTeraTypeIcon_PoisonTiles[];
extern const u8 gTeraTypeIcon_GroundTiles[];
extern const u8 gTeraTypeIcon_RockTiles[];
extern const u8 gTeraTypeIcon_BugTiles[];
extern const u8 gTeraTypeIcon_GhostTiles[];
extern const u8 gTeraTypeIcon_SteelTiles[];
extern const u8 gTeraTypeIcon_FireTiles[];
extern const u8 gTeraTypeIcon_WaterTiles[];
extern const u8 gTeraTypeIcon_GrassTiles[];
extern const u8 gTeraTypeIcon_ElectricTiles[];
extern const u8 gTeraTypeIcon_PsychicTiles[];
extern const u8 gTeraTypeIcon_IceTiles[];
extern const u8 gTeraTypeIcon_DragonTiles[];
extern const u8 gTeraTypeIcon_DarkTiles[];
extern const u8 gTeraTypeIcon_FairyTiles[];
extern const u8 gTeraTypeIcon_StellarTiles[];

static const struct SpriteSheet gTeraTypeIcon_NormalSummarySpriteSheet = {gTeraTypeIcon_NormalTiles, 4 * 2 * 32 * NUMBER_OF_MON_TYPES, TAG_TERA_TYPE_ICON_NORMAL};
static const struct SpriteSheet gTeraTypeIcon_FightingSummarySpriteSheet = {gTeraTypeIcon_FightingTiles, 4 * 2 * 32 * NUMBER_OF_MON_TYPES, TAG_TERA_TYPE_ICON_FIGHTING};
static const struct SpriteSheet gTeraTypeIcon_FlyingSummarySpriteSheet = {gTeraTypeIcon_FlyingTiles, 4 * 2 * 32 * NUMBER_OF_MON_TYPES, TAG_TERA_TYPE_ICON_FLYING};
static const struct SpriteSheet gTeraTypeIcon_PoisonSummarySpriteSheet = {gTeraTypeIcon_PoisonTiles, 4 * 2 * 32 * NUMBER_OF_MON_TYPES, TAG_TERA_TYPE_ICON_POISON};
static const struct SpriteSheet gTeraTypeIcon_GroundSummarySpriteSheet = {gTeraTypeIcon_GroundTiles, 4 * 2 * 32 * NUMBER_OF_MON_TYPES, TAG_TERA_TYPE_ICON_GROUND};
static const struct SpriteSheet gTeraTypeIcon_RockSummarySpriteSheet = {gTeraTypeIcon_RockTiles, 4 * 2 * 32 * NUMBER_OF_MON_TYPES, TAG_TERA_TYPE_ICON_ROCK};
static const struct SpriteSheet gTeraTypeIcon_BugSummarySpriteSheet = {gTeraTypeIcon_BugTiles, 4 * 2 * 32 * NUMBER_OF_MON_TYPES, TAG_TERA_TYPE_ICON_BUG};
static const struct SpriteSheet gTeraTypeIcon_GhostSummarySpriteSheet = {gTeraTypeIcon_GhostTiles, 4 * 2 * 32 * NUMBER_OF_MON_TYPES, TAG_TERA_TYPE_ICON_GHOST};
static const struct SpriteSheet gTeraTypeIcon_SteelSummarySpriteSheet = {gTeraTypeIcon_SteelTiles, 4 * 2 * 32 * NUMBER_OF_MON_TYPES, TAG_TERA_TYPE_ICON_STEEL};
static const struct SpriteSheet gTeraTypeIcon_FireSummarySpriteSheet = {gTeraTypeIcon_FireTiles, 4 * 2 * 32 * NUMBER_OF_MON_TYPES, TAG_TERA_TYPE_ICON_FIRE};
static const struct SpriteSheet gTeraTypeIcon_WaterSummarySpriteSheet = {gTeraTypeIcon_WaterTiles, 4 * 2 * 32 * NUMBER_OF_MON_TYPES, TAG_TERA_TYPE_ICON_WATER};
static const struct SpriteSheet gTeraTypeIcon_GrassSummarySpriteSheet = {gTeraTypeIcon_GrassTiles, 4 * 2 * 32 * NUMBER_OF_MON_TYPES, TAG_TERA_TYPE_ICON_GRASS};
static const struct SpriteSheet gTeraTypeIcon_ElectricSummarySpriteSheet = {gTeraTypeIcon_ElectricTiles, 4 * 2 * 32 * NUMBER_OF_MON_TYPES, TAG_TERA_TYPE_ICON_ELECTRIC};
static const struct SpriteSheet gTeraTypeIcon_PsychicSummarySpriteSheet = {gTeraTypeIcon_PsychicTiles, 4 * 2 * 32 * NUMBER_OF_MON_TYPES, TAG_TERA_TYPE_ICON_PSYCHIC};
static const struct SpriteSheet gTeraTypeIcon_IceSummarySpriteSheet = {gTeraTypeIcon_IceTiles, 4 * 2 * 32 * NUMBER_OF_MON_TYPES, TAG_TERA_TYPE_ICON_ICE};
static const struct SpriteSheet gTeraTypeIcon_DragonSummarySpriteSheet = {gTeraTypeIcon_DragonTiles, 4 * 2 * 32 * NUMBER_OF_MON_TYPES, TAG_TERA_TYPE_ICON_DRAGON};
static const struct SpriteSheet gTeraTypeIcon_DarkSummarySpriteSheet = {gTeraTypeIcon_DarkTiles, 4 * 2 * 32 * NUMBER_OF_MON_TYPES, TAG_TERA_TYPE_ICON_DARK};
static const struct SpriteSheet gTeraTypeIcon_FairySummarySpriteSheet = {gTeraTypeIcon_FairyTiles, 4 * 2 * 32 * NUMBER_OF_MON_TYPES, TAG_TERA_TYPE_ICON_FAIRY};
static const struct SpriteSheet gTeraTypeIcon_StellarSummarySpriteSheet = {gTeraTypeIcon_StellarTiles, 4 * 2 * 32 * NUMBER_OF_MON_TYPES, TAG_TERA_TYPE_ICON_STELLAR};

static const struct SpriteSheet *const sTeraTypeIconSheets[NUMBER_OF_MON_TYPES] = {    
    [TYPE_NORMAL]   = &gTeraTypeIcon_NormalSummarySpriteSheet,
    [TYPE_FIGHTING] = &gTeraTypeIcon_FightingSummarySpriteSheet,
    [TYPE_FLYING]   = &gTeraTypeIcon_FlyingSummarySpriteSheet,
    [TYPE_POISON]   = &gTeraTypeIcon_PoisonSummarySpriteSheet,
    [TYPE_GROUND]   = &gTeraTypeIcon_GroundSummarySpriteSheet,
    [TYPE_ROCK]     = &gTeraTypeIcon_RockSummarySpriteSheet,
    [TYPE_BUG]      = &gTeraTypeIcon_BugSummarySpriteSheet,
    [TYPE_GHOST]    = &gTeraTypeIcon_GhostSummarySpriteSheet,
    [TYPE_STEEL]    = &gTeraTypeIcon_SteelSummarySpriteSheet,
    [TYPE_FIRE]     = &gTeraTypeIcon_FireSummarySpriteSheet,
    [TYPE_WATER]    = &gTeraTypeIcon_WaterSummarySpriteSheet,
    [TYPE_GRASS]    = &gTeraTypeIcon_GrassSummarySpriteSheet,
    [TYPE_ELECTRIC] = &gTeraTypeIcon_ElectricSummarySpriteSheet,
    [TYPE_PSYCHIC]  = &gTeraTypeIcon_PsychicSummarySpriteSheet,
    [TYPE_ICE]      = &gTeraTypeIcon_IceSummarySpriteSheet,
    [TYPE_DRAGON]   = &gTeraTypeIcon_DragonSummarySpriteSheet,
    [TYPE_DARK]     = &gTeraTypeIcon_DarkSummarySpriteSheet,
    [TYPE_FAIRY]    = &gTeraTypeIcon_FairySummarySpriteSheet,
    [TYPE_STELLAR]  = &gTeraTypeIcon_StellarSummarySpriteSheet,
};

static const struct SpritePalette sTeraTypeIconPalTemplate =
{
    gTeraTypeIcon_NormalPal,
    TAG_TERA_TYPE_ICON_NORMAL
};

static const struct OamData sTeraTypeIconOAM =
{
	.affineMode = ST_OAM_AFFINE_OFF,
	.objMode = ST_OAM_OBJ_NORMAL,
	.shape = SPRITE_SHAPE(32x16),
	.size = SPRITE_SIZE(32x16),
	.priority = 1,
};

const struct SpriteTemplate gTeraTypeIconSpriteTemplate_Normal =
{
    .tileTag = TAG_TERA_TYPE_ICON_NORMAL,
    .paletteTag = TAG_TERA_TYPE_ICON_NORMAL,
    .oam = &sTeraTypeIconOAM,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

const struct SpriteTemplate gTeraTypeIconSpriteTemplate_Fighting =
{
    .tileTag = TAG_TERA_TYPE_ICON_FIGHTING,
    .paletteTag = TAG_TERA_TYPE_ICON_NORMAL,
    .oam = &sTeraTypeIconOAM,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

const struct SpriteTemplate gTeraTypeIconSpriteTemplate_Flying =
{
    .tileTag = TAG_TERA_TYPE_ICON_FLYING,
    .paletteTag = TAG_TERA_TYPE_ICON_NORMAL,
    .oam = &sTeraTypeIconOAM,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

const struct SpriteTemplate gTeraTypeIconSpriteTemplate_Poison =
{
    .tileTag = TAG_TERA_TYPE_ICON_POISON,
    .paletteTag = TAG_TERA_TYPE_ICON_NORMAL,
    .oam = &sTeraTypeIconOAM,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

const struct SpriteTemplate gTeraTypeIconSpriteTemplate_Ground =
{
    .tileTag = TAG_TERA_TYPE_ICON_GROUND,
    .paletteTag = TAG_TERA_TYPE_ICON_NORMAL,
    .oam = &sTeraTypeIconOAM,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

const struct SpriteTemplate gTeraTypeIconSpriteTemplate_Rock =
{
    .tileTag = TAG_TERA_TYPE_ICON_ROCK,
    .paletteTag = TAG_TERA_TYPE_ICON_NORMAL,
    .oam = &sTeraTypeIconOAM,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

const struct SpriteTemplate gTeraTypeIconSpriteTemplate_Bug =
{
    .tileTag = TAG_TERA_TYPE_ICON_BUG,
    .paletteTag = TAG_TERA_TYPE_ICON_NORMAL,
    .oam = &sTeraTypeIconOAM,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

const struct SpriteTemplate gTeraTypeIconSpriteTemplate_Ghost =
{
    .tileTag = TAG_TERA_TYPE_ICON_GHOST,
    .paletteTag = TAG_TERA_TYPE_ICON_NORMAL,
    .oam = &sTeraTypeIconOAM,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

const struct SpriteTemplate gTeraTypeIconSpriteTemplate_Steel =
{
    .tileTag = TAG_TERA_TYPE_ICON_STEEL,
    .paletteTag = TAG_TERA_TYPE_ICON_NORMAL,
    .oam = &sTeraTypeIconOAM,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

const struct SpriteTemplate gTeraTypeIconSpriteTemplate_Fire =
{
    .tileTag = TAG_TERA_TYPE_ICON_FIRE,
    .paletteTag = TAG_TERA_TYPE_ICON_NORMAL,
    .oam = &sTeraTypeIconOAM,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

const struct SpriteTemplate gTeraTypeIconSpriteTemplate_Water =
{
    .tileTag = TAG_TERA_TYPE_ICON_WATER,
    .paletteTag = TAG_TERA_TYPE_ICON_NORMAL,
    .oam = &sTeraTypeIconOAM,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

const struct SpriteTemplate gTeraTypeIconSpriteTemplate_Grass =
{
    .tileTag = TAG_TERA_TYPE_ICON_GRASS,
    .paletteTag = TAG_TERA_TYPE_ICON_NORMAL,
    .oam = &sTeraTypeIconOAM,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

const struct SpriteTemplate gTeraTypeIconSpriteTemplate_Electric =
{
    .tileTag = TAG_TERA_TYPE_ICON_ELECTRIC,
    .paletteTag = TAG_TERA_TYPE_ICON_NORMAL,
    .oam = &sTeraTypeIconOAM,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

const struct SpriteTemplate gTeraTypeIconSpriteTemplate_Psychic =
{
    .tileTag = TAG_TERA_TYPE_ICON_PSYCHIC,
    .paletteTag = TAG_TERA_TYPE_ICON_NORMAL,
    .oam = &sTeraTypeIconOAM,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

const struct SpriteTemplate gTeraTypeIconSpriteTemplate_Ice =
{
    .tileTag = TAG_TERA_TYPE_ICON_ICE,
    .paletteTag = TAG_TERA_TYPE_ICON_NORMAL,
    .oam = &sTeraTypeIconOAM,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

const struct SpriteTemplate gTeraTypeIconSpriteTemplate_Dragon =
{
    .tileTag = TAG_TERA_TYPE_ICON_DRAGON,
    .paletteTag = TAG_TERA_TYPE_ICON_NORMAL,
    .oam = &sTeraTypeIconOAM,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

const struct SpriteTemplate gTeraTypeIconSpriteTemplate_Dark =
{
    .tileTag = TAG_TERA_TYPE_ICON_DARK,
    .paletteTag = TAG_TERA_TYPE_ICON_NORMAL,
    .oam = &sTeraTypeIconOAM,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

const struct SpriteTemplate gTeraTypeIconSpriteTemplate_Fairy =
{
    .tileTag = TAG_TERA_TYPE_ICON_FAIRY,
    .paletteTag = TAG_TERA_TYPE_ICON_NORMAL,
    .oam = &sTeraTypeIconOAM,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

const struct SpriteTemplate gTeraTypeIconSpriteTemplate_Stellar =
{
    .tileTag = TAG_TERA_TYPE_ICON_STELLAR,
    .paletteTag = TAG_TERA_TYPE_ICON_NORMAL,
    .oam = &sTeraTypeIconOAM,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

static const struct SpriteTemplate *const gTeraTypeIconSpriteTemplates[NUMBER_OF_MON_TYPES] =
{
    [TYPE_NORMAL]   = &gTeraTypeIconSpriteTemplate_Normal,
    [TYPE_FIGHTING] = &gTeraTypeIconSpriteTemplate_Fighting,
    [TYPE_FLYING]   = &gTeraTypeIconSpriteTemplate_Flying,
    [TYPE_POISON]   = &gTeraTypeIconSpriteTemplate_Poison,
    [TYPE_GROUND]   = &gTeraTypeIconSpriteTemplate_Ground,
    [TYPE_ROCK]     = &gTeraTypeIconSpriteTemplate_Rock,
    [TYPE_BUG]      = &gTeraTypeIconSpriteTemplate_Bug,
    [TYPE_GHOST]    = &gTeraTypeIconSpriteTemplate_Ghost,
    [TYPE_STEEL]    = &gTeraTypeIconSpriteTemplate_Steel,
    [TYPE_FIRE]     = &gTeraTypeIconSpriteTemplate_Fire,
    [TYPE_WATER]    = &gTeraTypeIconSpriteTemplate_Water,
    [TYPE_GRASS]    = &gTeraTypeIconSpriteTemplate_Grass,
    [TYPE_ELECTRIC] = &gTeraTypeIconSpriteTemplate_Electric,
    [TYPE_PSYCHIC]  = &gTeraTypeIconSpriteTemplate_Psychic,
    [TYPE_ICE]      = &gTeraTypeIconSpriteTemplate_Ice,
    [TYPE_DRAGON]   = &gTeraTypeIconSpriteTemplate_Dragon,
    [TYPE_DARK]     = &gTeraTypeIconSpriteTemplate_Dark,
    [TYPE_FAIRY]    = &gTeraTypeIconSpriteTemplate_Fairy,
    [TYPE_STELLAR]  = &gTeraTypeIconSpriteTemplate_Stellar, // Supondo que esse seja o 19º tipo
};

void TeraIconSummaryScreen(void)
{
    struct Sprite* ballSprite = &gSprites[sMonSummaryScreen->ballIconSpriteId];
	u8 teraType = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_TERA_TYPE, NULL);
	if (teraType < NUMBER_OF_MON_TYPES)
	{
		LoadSpriteSheet(sTeraTypeIconSheets[teraType]);
        LoadSpritePalette(&sTeraTypeIconPalTemplate);

		s16 x = ballSprite->pos1.x - 6;
		s16 y = ballSprite->pos1.y -46;

		ballSprite->data[1] = CreateSprite(gTeraTypeIconSpriteTemplates[teraType], x, y, 0);
	}
	else
		ballSprite->data[1] = MAX_SPRITES; //No icon
};
#endif
