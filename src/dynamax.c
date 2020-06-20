#include "defines.h"
#include "defines_battle.h"
#include "../include/battle_anim.h"
#include "../include/overworld.h"
#include "../include/random.h"
#include "../include/constants/items.h"
#include "../include/constants/region_map_sections.h"

#include "../include/new/battle_indicators.h"
#include "../include/new/battle_util.h"
#include "../include/new/build_pokemon.h"
#include "../include/new/damage_calc.h"
#include "../include/new/dexnav.h"
#include "../include/new/dynamax.h"
#include "../include/new/form_change.h"
#include "../include/new/frontier.h"
#include "../include/new/general_bs_commands.h"
#include "../include/new/item.h"
#include "../include/new/mega.h"
#include "../include/new/mega_battle_scripts.h"
#include "../include/new/move_battle_scripts.h"
#include "../include/new/move_menu.h"
#include "../include/new/move_tables.h"
#include "../include/new/set_z_effect.h"
#include "../include/new/stat_buffs.h"
#include "../include/new/util.h"

/*
dynamax.c
	Functions that support Dynamax logic and execution,
	as well as raid battles.
*/

#define TRAINER_ITEM_COUNT 4
#define MAX_RAID_DROPS 12

#define FIRST_RAID_BATTLE_FLAG 0x1800

//This file's functions:
static bool8 IsBannedHeldItemForDynamax(u16 item);
static const u8* DoDynamax(u8 bank);
static const u8* DoGigantamax(u8 bank);
static bool8 IsItemDynamaxBand(u16 item);
static item_t FindTrainerDynamaxBand(u16 trainerId);
static item_t FindPlayerDynamaxBand(void);
static item_t FindBankDynamaxBand(u8 bank);
static move_t GetTypeBasedMaxMove(u8 moveType, u8 moveSplit);
static move_t GetGMaxMove(u8 moveType, u8 moveSplit, u16 species);
static u8 GetRaidMapSectionId(void);
static u32 GetRaidRandomNumber(void);

static const item_t sDynamaxBandTable[] =
{
	ITEM_DYNAMAX_BAND,
};

struct GMaxMove
{
	u16 species;
	u8 moveType;
	u16 gmaxMove;
};

static const struct GMaxMove sGMaxMoveTable[] =
{
	{SPECIES_VENUSAUR_GIGA,		TYPE_GRASS, 		MOVE_G_MAX_VINE_LASH_P},
	{SPECIES_CHARIZARD_GIGA,	TYPE_FIRE, 		MOVE_G_MAX_WILDFIRE_P},
	{SPECIES_BLASTOISE_GIGA,	TYPE_WATER, 		MOVE_G_MAX_CANNONADE_P},
	{SPECIES_BUTTERFREE_GIGA,	TYPE_BUG,		MOVE_G_MAX_BEFUDDLE_P},
	{SPECIES_PIKACHU_GIGA,		TYPE_ELECTRIC,		MOVE_G_MAX_VOLT_CRASH_P},
	{SPECIES_MEOWTH_GIGA,		TYPE_NORMAL,		MOVE_G_MAX_GOLD_RUSH_P},
	{SPECIES_MACHAMP_GIGA,		TYPE_FIGHTING,		MOVE_G_MAX_CHI_STRIKE_P},
	{SPECIES_GENGAR_GIGA,		TYPE_GHOST,		MOVE_G_MAX_TERROR_P},
	{SPECIES_KINGLER_GIGA,		TYPE_WATER,		MOVE_G_MAX_FOAM_BURST_P},
	{SPECIES_LAPRAS_GIGA,		TYPE_ICE,		MOVE_G_MAX_RESONANCE_P},
	{SPECIES_EEVEE_GIGA,		TYPE_NORMAL,		MOVE_G_MAX_CUDDLE_P},
	{SPECIES_SNORLAX_GIGA,		TYPE_NORMAL,		MOVE_G_MAX_REPLENISH_P},
	{SPECIES_GARBODOR_GIGA,		TYPE_POISON,		MOVE_G_MAX_MALODOR_P},
	{SPECIES_MELMETAL_GIGA,		TYPE_STEEL,		MOVE_G_MAX_MELTDOWN_P},
	{SPECIES_RILLABOOM_GIGA,	TYPE_GRASS, 		MOVE_G_MAX_DRUM_SOLO_P},
	{SPECIES_CINDERACE_GIGA,	TYPE_FIRE, 		MOVE_G_MAX_FIREBALL_P},
	{SPECIES_INTELEON_GIGA,		TYPE_WATER, 		MOVE_G_MAX_HYDROSNIPE_P},
	{SPECIES_CORVIKNIGHT_GIGA,	TYPE_FLYING,		MOVE_G_MAX_WIND_RAGE_P},
	{SPECIES_ORBEETLE_GIGA,		TYPE_PSYCHIC,		MOVE_G_MAX_GRAVITAS_P},
	{SPECIES_DREDNAW_GIGA,		TYPE_WATER,		MOVE_G_MAX_STONESURGE_P},
	{SPECIES_COALOSSAL_GIGA,	TYPE_ROCK,		MOVE_G_MAX_VOLCALITH_P},
	{SPECIES_FLAPPLE_GIGA,		TYPE_GRASS,		MOVE_G_MAX_TARTNESS_P},
	{SPECIES_APPLETUN_GIGA,		TYPE_GRASS,		MOVE_G_MAX_SWEETNESS_P},
	{SPECIES_SANDACONDA_GIGA,	TYPE_GROUND,		MOVE_G_MAX_SANDBLAST_P},
	{SPECIES_TOXTRICITY_GIGA,	TYPE_ELECTRIC,		MOVE_G_MAX_STUN_SHOCK_P},
	{SPECIES_CENTISKORCH_GIGA,	TYPE_FIRE, 		MOVE_G_MAX_CENTIFERNO_P},
	{SPECIES_HATTERENE,		TYPE_FAIRY,		MOVE_G_MAX_SMITE_P},
	{SPECIES_GRIMMSNARL_GIGA,	TYPE_DARK,		MOVE_G_MAX_SNOOZE_P},
	{SPECIES_ALCREMIE_GIGA,		TYPE_FAIRY,		MOVE_G_MAX_FINALE_P},
	{SPECIES_COPPERAJAH_GIGA,	TYPE_STEEL,		MOVE_G_MAX_STEELSURGE_P},
	{SPECIES_DURALUDON_GIGA,	TYPE_DRAGON,		MOVE_G_MAX_DEPLETION_P},
	{SPECIES_URSHIFU_SINGLE_GIGA,	TYPE_DARK,		MOVE_G_MAX_ONE_BLOW_P},
	{SPECIES_URSHIFU_RAPID_GIGA,	TYPE_WATER,		MOVE_G_MAX_RAPID_FLOW_P},
};

const u8 gRaidBattleStarsByBadges[NUM_BADGE_OPTIONS][2] =
{
	[0] = {NO_RAID, 		NO_RAID},
	[1] = {ONE_STAR_RAID, 	ONE_STAR_RAID},
	[2] = {ONE_STAR_RAID,   TWO_STAR_RAID},
	[3] = {TWO_STAR_RAID,   TWO_STAR_RAID},
	[4] = {TWO_STAR_RAID,   THREE_STAR_RAID},
	[5] = {THREE_STAR_RAID, THREE_STAR_RAID},
	[6] = {THREE_STAR_RAID, FOUR_STAR_RAID},
	[7] = {FOUR_STAR_RAID,  FOUR_STAR_RAID},
	[8] = {FOUR_STAR_RAID,  FIVE_STAR_RAID},
	[9] = {FIVE_STAR_RAID,  SIX_STAR_RAID}, //Beat Game
};

const u8 gRaidBattleLevelRanges[RAID_STAR_COUNT][2] =
{
	[ONE_STAR_RAID]   = {15, 20},
	[TWO_STAR_RAID]   = {25, 30},
	[THREE_STAR_RAID] = {35, 40},
	[FOUR_STAR_RAID]  = {50, 55},
	[FIVE_STAR_RAID]  = {60, 65},
	[SIX_STAR_RAID]   = {75, 90},
};

//The chance that each move is replaced with an Egg Move
const u8 gRaidBattleEggMoveChances[RAID_STAR_COUNT] =
{
	[ONE_STAR_RAID]   = 0,
	[TWO_STAR_RAID]   = 10,
	[THREE_STAR_RAID] = 30,
	[FOUR_STAR_RAID]  = 50,
	[FIVE_STAR_RAID]  = 70,
	[SIX_STAR_RAID]   = 70,
};

static const u8 sRaidBattleDropRates[MAX_RAID_DROPS] =
{	//In percent
	100,
	80,
	80,
	50,
	50,
	30,
	30,
	25,
	25,
	5,
	4,
	1,
};

species_t GetDynamaxSpecies(unusedArg u8 bank, unusedArg bool8 checkGMaxInstead)
{
	#ifndef DYNAMAX_FEATURE
		return SPECIES_NONE;
	#else
	u16 species = SPECIES(bank); //Prevents ditto too

	if (IsBannedHeldItemForDynamax(ITEM(bank)))
		return SPECIES_NONE;

	if (!checkGMaxInstead) //Checking regular Dynmax
	{
		if (IsBannedDynamaxSpecies(species))
			return SPECIES_NONE; //Certain Pokemon can't Dynamax

		return species; //Returning the species of the Pokemon is an indicator that they can Dynamax
	}
	else //Check Gigantamax
	{
		struct Pokemon* mon = GetBankPartyData(bank);
		const struct Evolution* evolutions = gEvolutionTable[mon->species];
		int i;

		if (mon->gigantamax) //Mon can Gigantamax
		{
			for (i = 0; i < EVOS_PER_MON; ++i)
			{
				if (evolutions[i].method == EVO_GIGANTAMAX)
				{
					//Ignore reversion information
					if (evolutions[i].param == 0) continue;

					//Any value other than 0 indicates G-Max potential
					return evolutions[i].targetSpecies;
				}
			}
		}
	}

	return SPECIES_NONE;
	#endif
}

static bool8 IsBannedHeldItemForDynamax(u16 item)
{
	if (IsMegaZMoveBannedBattle())
		return FALSE; //These items have no effect so don't ban them

	return IsMegaStone(item)
		|| IsZCrystal(item)
		|| IsPrimalOrb(item);
}

bool8 IsBannedDynamaxSpecies(u16 species)
{
	switch (species) {
		case SPECIES_NONE:
		#ifdef SPECIES_ZACIAN
		case SPECIES_ZACIAN:
		#endif
		#ifdef SPECIES_ZAMAZENTA
		case SPECIES_ZAMAZENTA:
		#endif
		#ifdef SPECIES_ZACIAN_CROWNED
		case SPECIES_ZACIAN_CROWNED:
		#endif
		#ifdef SPECIES_ZAMAZENTA_CROWNED
		case SPECIES_ZAMAZENTA_CROWNED:
		#endif
		#ifdef SPECIES_ETERNATUS
		case SPECIES_ETERNATUS:
		#endif
			return TRUE;
	}

	if (IsMegaSpecies(species)
	||  IsRedPrimalSpecies(species)
	||  IsBluePrimalSpecies(species)
	||  IsUltraNecrozmaSpecies(species))
		return TRUE;

	return FALSE;
}

bool8 CanDynamax(u8 bank)
{
	if (gBattleTypeFlags & BATTLE_TYPE_DYNAMAX && !gNewBS->dynamaxData.used[bank])
		return GetDynamaxSpecies(bank, FALSE) != SPECIES_NONE;

	return FALSE;
}

bool8 CanGigantamax(u8 bank)
{
	if (gBattleTypeFlags & BATTLE_TYPE_DYNAMAX && !gNewBS->dynamaxData.used[bank])
		return GetDynamaxSpecies(bank, TRUE) != SPECIES_NONE;

	return FALSE;
}

static const u8* DoDynamax(u8 bank)
{
	u16 dynamaxSpecies = GetDynamaxSpecies(bank, FALSE);

	if (dynamaxSpecies == SPECIES_NONE) //Can't Dynamax
		return NULL;

	gBattleScripting.bank = bank;
	gLastUsedItem = FindBankDynamaxBand(bank);

	return BattleScript_Dynamax;
}

static const u8* DoGigantamax(u8 bank)
{
	u16 gigantamaxSpecies = GetDynamaxSpecies(bank, TRUE);

	if (gigantamaxSpecies == SPECIES_NONE) //Can't Gigantamax
		return NULL;

	DoFormChange(bank, gigantamaxSpecies, FALSE, FALSE, FALSE);

	gBattleScripting.bank = bank;
	gLastUsedItem = FindBankDynamaxBand(bank);
	PREPARE_SPECIES_BUFFER(gBattleTextBuff1, gigantamaxSpecies);

	return BattleScript_Dynamax;
}

const u8* GetDynamaxScript(u8 bank)
{
	const u8* script = DoGigantamax(bank);
	if (script != NULL)
		return script;

	return DoDynamax(bank);
}

//In theory, this function will do nothing as the regular forms revert should
//should take care of the reversion. This is to prevent bugs if the player
//gives themselves a Gigantmax to start the battle.
void GigantamaxRevert(struct Pokemon* party)
{
	int i;

	for (i = 0; i < PARTY_SIZE; ++i)
		TryRevertGigantamax(&party[i]);
}

void TryRevertGigantamax(struct Pokemon* mon)
{
	u16 baseSpecies = GetGigantamaxBaseForm(mon->species);

	if (baseSpecies != SPECIES_NONE)
	{
		mon->species = baseSpecies;
		mon->gigantamax = TRUE; //If encountered in the wild, now can permanently Gigantamax
		CalculateMonStats(mon);
	}
}

u16 GetGigantamaxBaseForm(u16 species)
{
	const struct Evolution* evolutions = gEvolutionTable[species];

	for (u8 i = 0; i < EVOS_PER_MON; ++i)
	{
		if (evolutions[i].method == EVO_GIGANTAMAX && evolutions[i].param == 0)
			return evolutions[i].targetSpecies;
	}

	return SPECIES_NONE;
}

static bool8 IsItemDynamaxBand(u16 item)
{
	for (u8 i = 0; i < ARRAY_COUNT(sDynamaxBandTable); ++i)
	{
		if (item == sDynamaxBandTable[i])
			return TRUE;
	}

	return FALSE;
}

static item_t FindTrainerDynamaxBand(u16 trainerId)
{
	if (gBattleTypeFlags & (BATTLE_TYPE_FRONTIER | BATTLE_TYPE_LINK))
		return ITEM_DYNAMAX_BAND;

	for (u8 i = 0; i < TRAINER_ITEM_COUNT; ++i)
	{
		if (IsItemDynamaxBand(gTrainers[trainerId].items[i]))
			return gTrainers[trainerId].items[i];
	}

	return ITEM_NONE;
}

static item_t FindPlayerDynamaxBand(void)
{
	if (gBattleTypeFlags & (BATTLE_TYPE_FRONTIER | BATTLE_TYPE_LINK))
		return ITEM_DYNAMAX_BAND;

	#if (defined UNBOUND && defined VAR_KEYSTONE) //Mega Ring doubles as Dynamax Band in Unbound
		u16 dynamaxBand = VarGet(VAR_KEYSTONE);
		if (dynamaxBand != ITEM_NONE)
			return dynamaxBand;
	#else
		for (u8 i = 0; i < ARRAY_COUNT(sDynamaxBandTable); ++i)
		{
			if (CheckBagHasItem(sDynamaxBandTable[i], 1))
				return sDynamaxBandTable[i];
		}
	#endif

	#ifdef DEBUG_DYNAMAX
		return ITEM_DYNAMAX_BAND; //Give player Dynamax Band if they have none
	#endif

	return ITEM_NONE;
}

static item_t FindBankDynamaxBand(u8 bank)
{
	#ifdef DEBUG_DYNAMAX
		if (bank + 1)
			return ITEM_DYNAMAX_BAND;
	#endif

	if (SIDE(bank) == SIDE_OPPONENT)
	{
		if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS)
		{
			if (GetBattlerPosition(bank) == B_POSITION_OPPONENT_LEFT)
				return FindTrainerDynamaxBand(gTrainerBattleOpponent_A);
			else
				return FindTrainerDynamaxBand(SECOND_OPPONENT);
		}
		else
			return FindTrainerDynamaxBand(gTrainerBattleOpponent_A);
	}
	else //SIDE_PLAYER
	{
		if (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER)
		{
			if (GetBattlerPosition(bank) == B_POSITION_PLAYER_RIGHT)
				return FindTrainerDynamaxBand(VarGet(VAR_PARTNER));
			else
				return FindPlayerDynamaxBand();
		}
		else
			return FindPlayerDynamaxBand();
	}
}

bool8 DynamaxEnabled(u8 bank)
{
	if (gBattleTypeFlags & BATTLE_TYPE_DYNAMAX)
	{
		if (FindBankDynamaxBand(bank) == ITEM_NONE)
		{
			#ifdef DEBUG_DYNAMAX
				return TRUE;
			#else
				return FALSE;
			#endif
		}

		return TRUE;
	}

	return FALSE;
}

bool8 HasBankDynamaxedAlready(u8 bank)
{
	if ((SIDE(bank) == B_SIDE_PLAYER && gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER)
	||  (SIDE(bank) == B_SIDE_OPPONENT && gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS))
	{
		return gNewBS->dynamaxData.used[bank];
	}

	return gNewBS->dynamaxData.used[bank]
		|| (IS_DOUBLE_BATTLE && gNewBS->dynamaxData.used[PARTNER(bank)]);
}

bool8 IsGigantamaxSpecies(u16 species)
{
	const struct Evolution* evolutions = gEvolutionTable[species];

	for (u8 i = 0; i < EVOS_PER_MON; ++i)
	{
		if (evolutions[i].method == EVO_GIGANTAMAX && evolutions[i].param == 0)
			return TRUE;
	}

	return FALSE;
}

bool8 IsDynamaxed(u8 bank)
{
	return gNewBS->dynamaxData.timer[bank] != 0;
}

bool8 IsGigantamaxed(u8 bank)
{
	return IsGigantamaxSpecies(SPECIES(bank));
}

bool8 HasDynamaxSymbol(u8 bank)
{
	return IsDynamaxed(bank) || IsGigantamaxed(bank);
}

bool8 DoesDynamaxUsageStopMegaEvolution(u8 bank)
{
	return gNewBS->dynamaxData.used[bank]
		&& gNewBS->dynamaxData.partyIndex[SIDE(bank)] & gBitTable[gBattlerPartyIndexes[bank]];
}

static move_t GetTypeBasedMaxMove(u8 moveType, u8 moveSplit)
{
	if (moveType < TYPE_FIRE)
		return MOVE_MAX_STRIKE_P + (moveType * 2) + moveSplit;
	else if (moveType >= TYPE_FAIRY)
		return MOVE_MAX_STARFALL_P + ((moveType - TYPE_FAIRY) * 2) + moveSplit;
	else
		return MOVE_MAX_STRIKE_P + ((moveType - 1) * 2) + moveSplit;
}

static u8 GetMaxMoveType(u16 move, u8 bank, struct Pokemon* mon)
{
	u8 moveType = GetMoveTypeSpecialPreAbility(move, bank, mon);

	if (moveType == 0xFF) //No overridden type
	{
		u8 moveSplit, ability;

		if (mon != NULL)
		{
			moveSplit = CalcMoveSplitFromParty(mon, move);
			ability = GetMonAbility(mon);
		}
		else
		{
			moveSplit = CalcMoveSplit(bank, move);
			ability = ABILITY(bank);
		}

		//Try to modify Max Move's type using ability
		u16 maxMove = GetTypeBasedMaxMove(gBattleMoves[move].type, moveSplit);
		moveType = GetMoveTypeSpecialPostAbility(maxMove, ability, FALSE);
	}

	return moveType;
}

static move_t GetGMaxMove(u8 moveType, u8 moveSplit, u16 species)
{
	for (u32 i = 0; i < ARRAY_COUNT(sGMaxMoveTable); ++i)
	{
		if (sGMaxMoveTable[i].species == species
		&&  sGMaxMoveTable[i].moveType == moveType)
		{
			return sGMaxMoveTable[i].gmaxMove + moveSplit;
		}
	}

	return MOVE_NONE;
}

move_t GetMaxMove(u8 bank, u8 moveIndex)
{
	u16 baseMove = gBattleMons[bank].moves[moveIndex];
	return GetMaxMoveByMove(bank, baseMove);
}

move_t GetMaxMoveByMove(u8 bank, u16 baseMove)
{
	u8 moveSplit = CalcMoveSplit(bank, baseMove);

	if (baseMove == MOVE_NONE)
		return MOVE_NONE;

	if (!IsRaidBattle() || bank != BANK_RAID_BOSS) //Raid mon's don't care about these rules
	{
		if (IsBannedDynamaxSpecies(SPECIES(bank))
		|| IsBannedHeldItemForDynamax(ITEM(bank))
		|| DoesZMoveUsageStopDynamaxing(bank)) //No using Z-Move and Dynamaxing
			return MOVE_NONE;
	}

	if (moveSplit == SPLIT_STATUS)
		return MOVE_MAX_GUARD;

	u8 moveType = GetMaxMoveType(baseMove, bank, NULL);
	u16 maxMove = GetGMaxMove(moveType, moveSplit, SPECIES(bank));
	if (maxMove != MOVE_NONE)
		return maxMove;

	return GetTypeBasedMaxMove(moveType, moveSplit);
}

static move_t GetMonMaxMove(struct Pokemon* mon, u16 baseMove)
{
	u8 moveType = GetMaxMoveType(baseMove, 0, mon);
	u8 moveSplit = CalcMoveSplitFromParty(mon, baseMove);
	u16 maxMove = GetGMaxMove(moveType, moveSplit, mon->species);
	if (maxMove != MOVE_NONE)
		return maxMove;

	return GetTypeBasedMaxMove(moveType, moveSplit);
}

bool8 MonCanUseMaxMoveWithEffect(struct Pokemon* mon, u8 maxEffect)
{
	u16 species = GetMonData(mon, MON_DATA_SPECIES, NULL);
	u16 item = GetMonData(mon, MON_DATA_HELD_ITEM, NULL);

	if (IsBannedDynamaxSpecies(species)
	|| IsBannedHeldItemForDynamax(item))
		return FALSE;

	for (int i = 0; i < MAX_MON_MOVES; ++i)
	{
		u16 move = GetMonData(mon, MON_DATA_MOVE1 + i, NULL);
		if (move == MOVE_NONE)
			break;

		if (SPLIT(move) == SPLIT_STATUS)
			continue;

		move = GetMonMaxMove(mon, move);
		if (move != MOVE_NONE && gBattleMoves[move].z_move_effect == maxEffect)
			return TRUE;
	}

	return FALSE;
}

bool8 MonCanDynamax(struct Pokemon* mon)
{
	if (gBattleTypeFlags & BATTLE_TYPE_DYNAMAX)
	{
		u16 species = GetMonData(mon, MON_DATA_SPECIES, NULL);
		u16 item = GetMonData(mon, MON_DATA_HELD_ITEM, NULL);

		if (IsBannedDynamaxSpecies(species)
		|| IsBannedHeldItemForDynamax(item))
			return FALSE;

		return TRUE;
	}

	return FALSE;
}

u8 GetDynamaxHPBoost(u8 bank)
{
	if (SPECIES(bank) == SPECIES_SHEDINJA)
		return 1;
	else if (IsRaidBattle() && bank == BANK_RAID_BOSS)
		return GetRaidBattleHPBoost();
	else
		return 2;
}

u8 GetMonDynamaxHPBoost(unusedArg struct Pokemon* mon)
{
	if (GetMonData(mon, MON_DATA_SPECIES, NULL) == SPECIES_SHEDINJA)
		return 1;
	
	return 2;
}

u8 GetRaidBattleHPBoost(void)
{
	return 4;
}

bool8 IsAnyMaxMove(u16 move)
{
	return move >= FIRST_MAX_MOVE && move <= LAST_G_MAX_MOVE;
}

bool8 IsGMaxMove(u16 move)
{
	return move >= FIRST_G_MAX_MOVE && move <= LAST_G_MAX_MOVE;
}

void TryFadeBankPaletteForDynamax(u8 bank, u16 paletteOffset)
{
	if (IsDynamaxed(bank)
	|| (IsRaidBattle() && bank == BANK_RAID_BOSS)) //So it stays lit up when you try to catch it
	{
        BlendPalette(paletteOffset, 16, 4, RGB(31, 0, 12)); //Dynamax Pinkish-Red
        CpuCopy32(gPlttBufferFaded + paletteOffset, gPlttBufferUnfaded + paletteOffset, 32);
	}
}

extern const struct UCoords8 sBattlerCoords[][4];
s16 GetBattlerXCoord(u8 bank)
{
	if (IsRaidBattle() && SIDE(bank) == B_SIDE_OPPONENT)
		return sBattlerCoords[FALSE][B_POSITION_OPPONENT_LEFT].x; //Only 1 Pokemon so Single battle position

	return sBattlerCoords[IS_DOUBLE_BATTLE][GetBattlerPosition(bank)].x;
}

s16 GetBattlerYCoord(u8 bank)
{
	if (IsRaidBattle() && SIDE(bank) == B_SIDE_OPPONENT)
		return sBattlerCoords[FALSE][B_POSITION_OPPONENT_LEFT].y; //Only 1 Pokemon so Single battle position

	return sBattlerCoords[IS_DOUBLE_BATTLE][GetBattlerPosition(bank)].y;
}

void LoadAndCreateEnemyShadowSprites(void)
{
    u8 battlerId;

    LoadCompressedSpriteSheetUsingHeap((void*) 0x8250A0C); //gSpriteSheet_EnemyShadow

    battlerId = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
    gBattleSpritesDataPtr->healthBoxesData[battlerId].shadowSpriteId = CreateSprite((void*) 0x8250A1C, GetBattlerSpriteCoord(battlerId, BATTLER_COORD_X), GetBattlerSpriteCoord(battlerId, BATTLER_COORD_Y) + 29, 0xC8); //gSpriteTemplate_EnemyShadow
    gSprites[gBattleSpritesDataPtr->healthBoxesData[battlerId].shadowSpriteId].data[0] = battlerId;

    if (IS_DOUBLE_BATTLE)
    {
        battlerId = GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT);
        gBattleSpritesDataPtr->healthBoxesData[battlerId].shadowSpriteId = CreateSprite((void*) 0x8250A1C, GetBattlerSpriteCoord(battlerId, BATTLER_COORD_X), GetBattlerSpriteCoord(battlerId, BATTLER_COORD_Y) + 29, 0xC8);
        gSprites[gBattleSpritesDataPtr->healthBoxesData[battlerId].shadowSpriteId].data[0] = battlerId;

		//The game is insistent that there must be two shadow sprites in Doubles, otherwise things break
		//So overlap the two shadow sprites in a Raid Battle
		if (IsRaidBattle())
			gSprites[gBattleSpritesDataPtr->healthBoxesData[battlerId].shadowSpriteId].data[0] = BANK_RAID_BOSS;
    }
}

void HandleDeadRaidMonAndDeadPlayer(void)
{
	if (RAID_BATTLE_END && !BATTLER_ALIVE(GetBattlerAtPosition(B_POSITION_PLAYER_LEFT)))
		gBattleStruct->field_91 &= ~gBitTable[GetBattlerAtPosition(B_POSITION_PLAYER_LEFT)]; //So the player can still catch even if they have no Pokemon left
}

//Called from Battle Script
void UpdateHPForDynamax(void)
{
	if (gBattleExecBuffer)
	{
		gBattlescriptCurrInstr -= 5;
		return;
	}

	u32 newCurrentHp;
	gActiveBattler = gBattleScripting.bank;

	#ifdef SPECIES_SHEDINJA
	if (SPECIES(gActiveBattler) == SPECIES_SHEDINJA) //Shedinja doesn't get a Dynamax HP boost
	{
		gBattleMoveDamage = 0;
		return;
	}
	#endif

	u8 hpBoost = GetDynamaxHPBoost(gActiveBattler);

	if (IsDynamaxed(gActiveBattler))
	{
		gBattleMons[gActiveBattler].maxHP = MathMin(gBattleMons[gActiveBattler].maxHP * hpBoost, 0xFFFF);
		newCurrentHp = MathMin(gBattleMons[gActiveBattler].hp * hpBoost, 0xFFFF);

		gBattleMoveDamage = (newCurrentHp - gBattleMons[gActiveBattler].hp) * - 1;
	}
	else
	{
		//Get ceiling of HP divided by boost
		gBattleMons[gActiveBattler].maxHP = MathMax(gBattleMons[gActiveBattler].maxHP / hpBoost + (gBattleMons[gActiveBattler].maxHP & 1), 1);
		gBattleMons[gActiveBattler].hp = MathMax(gBattleMons[gActiveBattler].hp / hpBoost + (gBattleMons[gActiveBattler].hp & 1), 1);
	}

	EmitSetMonData(0, REQUEST_MAX_HP_BATTLE, 0, 2, &gBattleMons[gActiveBattler].maxHP);
	MarkBufferBankForExecution(gActiveBattler);
}

void EndBattleDynamaxRevert(u8 bank)
{
	if (BATTLER_ALIVE(bank) && IsDynamaxed(bank))
	{
		u16 hp, maxHP;
		struct Pokemon* mon = GetBankPartyData(bank);

		hp = GetBaseCurrentHP(bank);
		maxHP = GetBaseMaxHP(bank);
		SetMonData(mon, MON_DATA_HP, &hp);
		SetMonData(mon, MON_DATA_MAX_HP, &maxHP);
		gNewBS->dynamaxData.timer[bank] = 0;
	}
}

void TryBoostDynamaxHPAfterLevelUp(u8 bank) //Should only be called once all battle mon stats are updated
{
	struct Pokemon* mon;
	gActiveBattler = bank;

	#ifdef SPECIES_SHEDINJA
	if (SPECIES(gActiveBattler) == SPECIES_SHEDINJA) //Shedinja doesn't get a Dynamax HP boost
		return;
	#endif

	if (IsDynamaxed(gActiveBattler))
	{
		u8 hpBoost = GetDynamaxHPBoost(gActiveBattler);
		mon = GetBankPartyData(gActiveBattler);

		gBattleMons[gActiveBattler].hp = MathMin(gBattleMons[gActiveBattler].hp + (mon->maxHP * hpBoost - gBattleMons[gActiveBattler].maxHP), 0xFFFF); //Calculate proper new hp
		gBattleMons[gActiveBattler].maxHP = MathMin(mon->maxHP * hpBoost, 0xFFFF);

		//This is gonna bite me in the butt later
		mon->hp = gBattleMons[gActiveBattler].hp;
		mon->maxHP = gBattleMons[gActiveBattler].maxHP;

		/*
			It should be done this way, but the game crashes when trying to learn a new move, so I'll deal with
			this if I ever do link Raid battles.
		*/
		//EmitSetMonData(0, REQUEST_ALL_BATTLE, 0, sizeof(gBattleMons[gActiveBattler]), &gBattleMons[gActiveBattler]);
		//MarkBufferBankForExecution(gActiveBattler);
	}
}

//Called from Battle Script
void UpdateCurrentHealthForDynamaxEnd(void)
{
	if (gBattleExecBuffer)
	{
		gBattlescriptCurrInstr -= 5;
		return;
	}

	gActiveBattler = gBattleScripting.bank;

	if (!IsDynamaxed(gActiveBattler))
	{
		EmitSetMonData(0, REQUEST_HP_BATTLE, 0, 2, &gBattleMons[gActiveBattler].hp);
		MarkBufferBankForExecution(gActiveBattler);
	}
}

//Called from Battle Script
void ClearBallAnimActiveBit(void)
{
	gBattleSpritesDataPtr->healthBoxesData[gBattleScripting.bank].ballAnimActive = FALSE;
}

void atkFF2F_setmaxmoveeffect(void)
{
	gBattlescriptCurrInstr += 1;

	if (IsRaidBattle()
	&& gBankTarget == BANK_RAID_BOSS
	&& gNewBS->dynamaxData.raidShieldsUp)
		return; //No special effect when move is blocked by shields

	gHitMarker |= HITMARKER_IGNORE_SUBSTITUTE;

	switch (gBattleMoves[gCurrentMove].z_move_effect) { //Stored here for simplicity
		case MAX_EFFECT_RAISE_TEAM_ATTACK:
		case MAX_EFFECT_RAISE_TEAM_DEFENSE:
		case MAX_EFFECT_RAISE_TEAM_SPEED:
		case MAX_EFFECT_RAISE_TEAM_SP_ATK:
		case MAX_EFFECT_RAISE_TEAM_SP_DEF:
			if (BATTLER_ALIVE(gBankAttacker) || BATTLER_ALIVE(PARTNER(gBankAttacker)))
			{
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_MaxMoveRaiseStatTeam;
			}
			break;

		case MAX_EFFECT_LOWER_ATTACK:
		case MAX_EFFECT_LOWER_DEFENSE:
		case MAX_EFFECT_LOWER_SPEED:
		case MAX_EFFECT_LOWER_SP_ATK:
		case MAX_EFFECT_LOWER_SP_DEF:
			if (BATTLER_ALIVE(gBankTarget) || BATTLER_ALIVE(PARTNER(gBankTarget)))
			{
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_MaxMoveLowerStatFoes;
			}
			break;

		case MAX_EFFECT_SUN:
			if (SetSunnyWeather())
			{
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_MaxMoveSetWeather;
			}
			break;
		case MAX_EFFECT_RAIN:
			if (SetRainyWeather())
			{
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_MaxMoveSetWeather;
			}
			break;
		case MAX_EFFECT_SANDSTORM:
			if (SetSandstormWeather())
			{
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_MaxMoveSetWeather;
			}
			break;
		case MAX_EFFECT_HAIL:
			if (SetHailWeather())
			{
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_MaxMoveSetWeather;
			}
			break;

		case MAX_EFFECT_ELECTRIC_TERRAIN:
		case MAX_EFFECT_GRASSY_TERRAIN:
		case MAX_EFFECT_MISTY_TERRAIN:
		case MAX_EFFECT_PSYCHIC_TERRAIN:
			BattleScriptPushCursor();
			gBattlescriptCurrInstr = BattleScript_MaxMoveSetTerrain;
			break;

		case MAX_EFFECT_WILDFIRE:
			if (!BankSideHasGMaxWildfire(gBankTarget))
			{
				gNewBS->maxWildfireTimers[SIDE(gBankTarget)] = 4;
				gBattleStringLoader = gText_SurroundedByGMaxWildfire;
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_PrintCustomString;
			}
			break;

		case MAX_EFFECT_EFFECT_SPORE_FOES:
			if (BATTLER_ALIVE(gBankTarget) || (IS_DOUBLE_BATTLE && BATTLER_ALIVE(PARTNER(gBankTarget))))
			{
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_MaxMoveEffectSporeFoes;
			}
			break;

		case MAX_EFFECT_PARALYZE_FOES:
			if (BATTLER_ALIVE(gBankTarget) || (IS_DOUBLE_BATTLE && BATTLER_ALIVE(PARTNER(gBankTarget))))
			{
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_MaxMoveParalyzeFoes;
			}
			break;

		case MAX_EFFECT_CONFUSE_FOES_PAY_DAY:
			BattleScriptPushCursor();
			gBattlescriptCurrInstr = BattleScript_MaxMovePayDayConfuseFoes;
			break;

		case MAX_EFFECT_CRIT_PLUS:
			if ((BATTLER_ALIVE(gBankAttacker) || (IS_DOUBLE_BATTLE && BATTLER_ALIVE(PARTNER(gBankAttacker))))
			&& (!(gBattleMons[gBankAttacker].status2 & STATUS2_FOCUS_ENERGY) || (IS_DOUBLE_BATTLE && !(gBattleMons[PARTNER(gBankAttacker)].status2 & STATUS2_FOCUS_ENERGY))))
			{
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_MaxMoveFocusEnergy;
			}
			break;

		case MAX_EFFECT_MEAN_LOOK:
			if (BATTLER_ALIVE(gBankTarget) && !IsOfType(gBankTarget, TYPE_GHOST) && !(gBattleMons[gBankTarget].status2 & STATUS2_ESCAPE_PREVENTION))
			{
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_MaxMoveTrap;
			}
			break;

		case MAX_EFFECT_AURORA_VEIL:
			if (!gNewBS->AuroraVeilTimers[SIDE(gBankAttacker)])
			{
				if (ITEM_EFFECT(gBankAttacker) == ITEM_EFFECT_LIGHT_CLAY)
					gNewBS->AuroraVeilTimers[SIDE(gBankAttacker)] = 8;
				else
					gNewBS->AuroraVeilTimers[SIDE(gBankAttacker)] = 5;

				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_PrintCustomString;
				gBattleStringLoader = gText_SetAuroraVeil;
			}
			break;

		case MAX_EFFECT_INFATUATE_FOES:
			BattleScriptPushCursor();
			gBattlescriptCurrInstr = BattleScript_MaxMoveInfatuation;
			break;

		case MAX_EFFECT_RECYCLE_BERRIES:
			if ((Random() & 1) == 0 //50 % of the time
			&& (BATTLER_ALIVE(gBankAttacker) || (IS_DOUBLE_BATTLE && BATTLER_ALIVE(PARTNER(gBankAttacker)))) //Someone on this side is alive
			&& ((ITEM(gBankAttacker) == ITEM_NONE && IsBerry(SAVED_CONSUMED_ITEMS(gBankAttacker))) //Check for a Berry to restore
			 || (IS_DOUBLE_BATTLE && ITEM(PARTNER(gBankAttacker)) == ITEM_NONE && IsBerry(SAVED_CONSUMED_ITEMS(PARTNER(gBankAttacker))))))
			{
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_MaxMoveRecycle;
			}
			break;

		case MAX_EFFECT_POISON_FOES:
			if (BATTLER_ALIVE(gBankTarget) || (IS_DOUBLE_BATTLE && BATTLER_ALIVE(PARTNER(gBankTarget))))
			{
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_MaxMovePoisonFoes;
			}
			break;

		case MAX_EFFECT_STEALTH_ROCK:
			if (gSideTimers[SIDE(gBankTarget)].srAmount == 0)
			{
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_MaxMoveSetStealthRock;
			}
			break;

		case MAX_EFFECT_DEFOG:
			BattleScriptPushCursor();
			gBattlescriptCurrInstr = BattleScript_MaxMoveDefog;
			break;

		case MAX_EFFECT_POISON_PARALYZE_FOES:
			if (BATTLER_ALIVE(gBankTarget) || (IS_DOUBLE_BATTLE && BATTLER_ALIVE(PARTNER(gBankTarget))))
			{
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_MaxMovePoisonParalyzeFoes;
			}
			break;

		case MAX_EFFECT_HEAL_TEAM:
			if ((BATTLER_ALIVE(gBankAttacker) || (IS_DOUBLE_BATTLE && BATTLER_ALIVE(PARTNER(gBankAttacker))))
			&& (!BATTLER_MAX_HP(gBankAttacker) || (IS_DOUBLE_BATTLE && !BATTLER_MAX_HP(PARTNER(gBankAttacker)))))
			{
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_MaxMoveHealTeam;
			}
			break;

		case MAX_EFFECT_SPITE:
			if (BATTLER_ALIVE(gBankTarget) && TrySpitePPReduce(gBankTarget, 2))
			{
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_MaxMoveSpite;
			}
			break;

		case MAX_EFFECT_GRAVITY:
			if (!IsGravityActive())
			{
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_MaxMoveSetGravity;
			}
			break;

		case MAX_EFFECT_VOLCAITH_FOES:
			if (!BankSideHasGMaxVolcalith(gBankTarget))
			{
				gNewBS->maxVolcalithTimers[SIDE(gBankTarget)] = 4;
				gBattleStringLoader = gText_SurroundedByGMaxVolcalith;
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_PrintCustomString;
			}
			break;

		case MAX_EFFECT_SANDBLAST_FOES:
		case MAX_EFFECT_FIRE_SPIN_FOES:
			if ((BATTLER_ALIVE(gBankTarget) || (IS_DOUBLE_BATTLE && BATTLER_ALIVE(PARTNER(gBankTarget))))
			&&  (!(gBattleMons[gBankTarget].status2 & STATUS2_WRAPPED) || (IS_DOUBLE_BATTLE && !(gBattleMons[PARTNER(gBankTarget)].status2 & STATUS2_WRAPPED))))
			{
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_MaxMoveTrapAndDamageFoes;
			}
			break;

		case MAX_EFFECT_YAWN_FOE:
			if (BATTLER_ALIVE(gBankTarget)
			&& CanBeYawned(gBankTarget)
			&& !(gStatuses3[gBankTarget] & STATUS3_YAWN)
			&& (Random() & 1) == 0) //50 % chance target is put to sleep
			{
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_MaxMoveSetYawn;
			}
			break;

		case MAX_EFFECT_LOWER_EVASIVENESS_FOES:
			if (BATTLER_ALIVE(gBankTarget) || (IS_DOUBLE_BATTLE && BATTLER_ALIVE(PARTNER(gBankTarget))))
			{
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_MaxMoveLowerEvasivenessFoes;
			}
			break;

		case MAX_EFFECT_AROMATHERAPY:
			BattleScriptPushCursor();
			gBattlescriptCurrInstr = BattleScript_MaxMoveAromatherapy;
			break;

		case MAX_EFFECT_CONFUSE_FOES:
			if ((BATTLER_ALIVE(gBankTarget) || (IS_DOUBLE_BATTLE && BATTLER_ALIVE(PARTNER(gBankTarget))))
			&&  (CanBeConfused(gBankTarget, TRUE) || (IS_DOUBLE_BATTLE && CanBeConfused(PARTNER(gBankTarget), TRUE)))) //Is it worth it to push the script
			{
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_MaxMoveConfuseFoes;
			}
			break;

		case MAX_EFFECT_TORMENT_FOES:
			if ((BATTLER_ALIVE(gBankTarget) || (IS_DOUBLE_BATTLE && BATTLER_ALIVE(PARTNER(gBankTarget))))
			&&  (CanBeTormented(gBankTarget) || (IS_DOUBLE_BATTLE && CanBeTormented(PARTNER(gBankTarget))))) //Is it worth it to push the script
			{
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_MaxMoveTormentFoes;
			}
			break;

		case MAX_EFFECT_LOWER_SPEED_2_FOES:
			if (BATTLER_ALIVE(gBankTarget) || (IS_DOUBLE_BATTLE && BATTLER_ALIVE(PARTNER(gBankTarget))))
			{
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_MaxMoveLowerSpeed2Foes;
			}
			break;

		case MAX_EFFECT_STEELSURGE:
			if (gSideTimers[SIDE(gBankTarget)].steelsurge == 0)
			{
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_MaxMoveSetStealthRock;
			}
			break;
	}
}

void SetMaxMoveStatRaiseEffect(void)
{
	u8 statId = (gBattleMoves[gCurrentMove].z_move_effect - MAX_EFFECT_RAISE_TEAM_ATTACK);
	gBattleCommunication[MOVE_EFFECT_BYTE] = (MOVE_EFFECT_ATK_PLUS_1 + statId) | MOVE_EFFECT_AFFECTS_USER;
}

void SetMaxMoveStatLowerEffect(void)
{
	u8 statId = (gBattleMoves[gCurrentMove].z_move_effect - MAX_EFFECT_LOWER_ATTACK);
	gBattleCommunication[MOVE_EFFECT_BYTE] = MOVE_EFFECT_ATK_MINUS_1 + statId;
}

void PickRandomGMaxBefuddleEffect(void)
{
	do
	{
		gBattleCommunication[MOVE_EFFECT_BYTE] = Random() & 3;
	} while (gBattleCommunication[MOVE_EFFECT_BYTE] == 0);

	switch (gBattleCommunication[MOVE_EFFECT_BYTE]) {
		case MOVE_EFFECT_SLEEP:
			if (CanBePutToSleep(gBankTarget, TRUE))
				gHitMarker |= HITMARKER_IGNORE_SAFEGUARD; //Safeguard checked on line above
			break;
		case MOVE_EFFECT_POISON:
			if (CanBePoisoned(gBankTarget, gBankAttacker, TRUE))
				gHitMarker |= HITMARKER_IGNORE_SAFEGUARD; //Safeguard checked on line above
			break;
		case MOVE_EFFECT_BURN: //Gets changed to Paralysis
			gBattleCommunication[MOVE_EFFECT_BYTE] = MOVE_EFFECT_PARALYSIS;
			if (CanBeParalyzed(gBankTarget, TRUE))
				gHitMarker |= HITMARKER_IGNORE_SAFEGUARD; //Safeguard checked on line above
			break;
	}
}

void SetGMaxVoltCrashEffect(void)
{
	gBattleCommunication[MOVE_EFFECT_BYTE] = MOVE_EFFECT_PARALYSIS;
	if (CanBeParalyzed(gBankTarget, TRUE))
		gHitMarker |= HITMARKER_IGNORE_SAFEGUARD; //Safeguard checked on line above
}

void SetGMaxMalodorEffect(void)
{
	gBattleCommunication[MOVE_EFFECT_BYTE] = MOVE_EFFECT_POISON;
	if (CanBePoisoned(gBankTarget, gBankAttacker, TRUE))
		gHitMarker |= HITMARKER_IGNORE_SAFEGUARD; //Safeguard checked on line above
}

void PickRandomGMaxStunshockEffect(void)
{
	if ((Random() & 1) == 0)
	{
		gBattleCommunication[MOVE_EFFECT_BYTE] = MOVE_EFFECT_POISON;
		if (CanBePoisoned(gBankTarget, gBankAttacker, TRUE))
			gHitMarker |= HITMARKER_IGNORE_SAFEGUARD; //Safeguard checked on line above
	}
	else
	{
		gBattleCommunication[MOVE_EFFECT_BYTE] = MOVE_EFFECT_PARALYSIS;
		if (CanBeParalyzed(gBankTarget, TRUE))
			gHitMarker |= HITMARKER_IGNORE_SAFEGUARD; //Safeguard checked on line above
	}
}

void SetGMaxSmiteEffect(void)
{
	gBattleCommunication[MOVE_EFFECT_BYTE] = MOVE_EFFECT_CONFUSION;
	if (CanBeConfused(gBankTarget, TRUE))
		gHitMarker |= HITMARKER_IGNORE_SAFEGUARD; //Safeguard checked on line above
}

bool8 IsMaxMoveWithWeatherEffect(u16 move)
{
	if (IsAnyMaxMove(move))
	{
		switch (gBattleMoves[move].z_move_effect) {
			case MAX_EFFECT_SUN:
			case MAX_EFFECT_RAIN:
			case MAX_EFFECT_SANDSTORM:
			case MAX_EFFECT_HAIL:
				return TRUE;
		}
	}

	return FALSE;
}

bool8 IsMaxMoveWithTerrainEffect(u16 move)
{
	if (IsAnyMaxMove(move))
	{
		switch (gBattleMoves[move].z_move_effect) {
			case MAX_EFFECT_ELECTRIC_TERRAIN:
			case MAX_EFFECT_GRASSY_TERRAIN:
			case MAX_EFFECT_MISTY_TERRAIN:
			case MAX_EFFECT_PSYCHIC_TERRAIN:
				return TRUE;
		}
	}

	return FALSE;
}

bool8 IsMaxMoveWithStatusEffect(u16 move)
{
	if (IsAnyMaxMove(move))
	{
		switch (gBattleMoves[move].z_move_effect) {
			case MAX_EFFECT_EFFECT_SPORE_FOES:
			case MAX_EFFECT_POISON_FOES:
			case MAX_EFFECT_PARALYZE_FOES:
			case MAX_EFFECT_POISON_PARALYZE_FOES:
				return TRUE;
		}
	}

	return FALSE;
}

bool8 IsMaxMoveWithConfusionEffect(u16 move)
{
	if (IsAnyMaxMove(move))
	{
		switch (gBattleMoves[move].z_move_effect) {
			case MAX_EFFECT_CONFUSE_FOES:
			case MAX_EFFECT_CONFUSE_FOES_PAY_DAY:
				return TRUE;
		}
	}

	return FALSE;
}

bool8 IsMaxMoveWithTrapDamageEffect(u16 move)
{
	if (IsAnyMaxMove(move))
	{
		switch (gBattleMoves[move].z_move_effect) {
			case MAX_EFFECT_SANDBLAST_FOES:
			case MAX_EFFECT_FIRE_SPIN_FOES:
				return TRUE;
		}
	}

	return FALSE;
}

bool8 IsMaxMoveWithEffect(u16 move, u8 effect)
{
	return IsAnyMaxMove(move) && gBattleMoves[move].z_move_effect == effect;
}

bool8 IsMaxGuardUp(u8 bank)
{
	return gProtectStructs[bank].protected && IsDynamaxed(bank);
}

bool8 ProtectedByMaxGuard(u8 bankDef, u16 move)
{
	if (gProtectStructs[bankDef].protected && IsDynamaxed(bankDef) && move != MOVE_FEINT && move != MOVE_MEANLOOK) //Mean Look is probably a GF bug
	{
		if (gBattleMoves[move].target & (MOVE_TARGET_DEPENDS | MOVE_TARGET_OPPONENTS_FIELD))
			return (gBattleMoves[move].flags & FLAG_PROTECT_AFFECTED) != 0;

		if (gBattleMoves[move].target & MOVE_TARGET_ALL && CheckTableForMove(gCurrentMove, gSpecialWholeFieldMoves))
			return FALSE;

		return TRUE;
	}

	return FALSE;
}

//The following functions relate to raid battles:
bool8 IsRaidBattle(void)
{
	#ifdef FLAG_RAID_BATTLE
	return FlagGet(FLAG_RAID_BATTLE) && !(gBattleTypeFlags & BATTLE_TYPE_TRAINER);
	#else
	return FALSE;
	#endif
}

bool8 IsFrontierRaidBattle(void)
{
	return IsRaidBattle() && FlagGet(FLAG_BATTLE_FACILITY);
}

bool8 IsCatchableRaidBattle(void)
{
	return IsRaidBattle() && !FlagGet(FLAG_NO_CATCHING) && !FlagGet(FLAG_NO_CATCHING_AND_RUNNING);
}

bool8 HasRaidShields(u8 bank)
{
	return GetBattlerPosition(bank) == B_POSITION_OPPONENT_LEFT
		&& gNewBS->dynamaxData.raidShieldsUp;
}

static u8 GetRaidShieldHealthRatio(u8 bank)
{
	switch (gBattleMons[bank].level) {
		case 0 ... 19:
			return 1; //Never
		case 20 ... 40:
			return 2; //Every 1/2 health lost
		case 41 ... 70:
			return 3; //Every 1/3 health lost
		default:
			return 4; //Every 1/4 health lost
	}
}

bool8 ShouldCreateRaidShields(u8 bank)
{
	u8 i, healthRatio;

	if (HasRaidShields(bank) || !BATTLER_ALIVE(bank))
		return FALSE;

	healthRatio = GetRaidShieldHealthRatio(bank);

	for (i = 1; i <= healthRatio; ++i)
	{
		u16 cutOff = (gBattleMons[bank].maxHP / healthRatio) * i;

		if (i == healthRatio)
			cutOff = gBattleMons[bank].maxHP; //Fix Math Errors

		if (gBattleMons[bank].hp <= cutOff + (gBattleMons[bank].maxHP / 16) //Give some leeway for throwing up a shield
		&&  gNewBS->dynamaxData.turnStartHP > cutOff)
			return TRUE;
	}

	return FALSE;
}

u16 GetNextRaidShieldHP(u8 bank)
{
	u8 i, healthRatio;
	u16 cutOff, prevCutOff;

	if (HasRaidShields(bank) || !BATTLER_ALIVE(bank))
		return 0;

	healthRatio = GetRaidShieldHealthRatio(bank);
	cutOff = 0;

	for (i = 1; i <= healthRatio; ++i) //Up to max HP
	{
		prevCutOff = cutOff;
		cutOff = (gBattleMons[bank].maxHP / healthRatio) * i;

		if (i == healthRatio)
			cutOff = gBattleMons[bank].maxHP; //Fix Math Errors

		if (gBattleMons[bank].hp > prevCutOff && gBattleMons[bank].hp <= cutOff)
			return prevCutOff;
	}

	return 0;
}

void CreateRaidShieldSprites(void)
{
	//Base stat total determines how many shields
	//Level determines how often the shields appear

	u8 i, numShields;
	u8 bank = BANK_RAID_BOSS;
	u16 baseStatTotal = GetBaseStatsTotal(SPECIES(bank));

	if (!FlagGet(FLAG_RAID_BATTLE_NO_FORCE_END)) //Less shields for battle that ends in 10 turns
	{
		switch (baseStatTotal) {
			case 0 ... 349:
				numShields = 1;
				break;
			case 350 ... 494:
				numShields = 2;
				break;
			case 495 ... 568:
				numShields = 3;
				break;
			default: //569+
				numShields = 4;
		}
	}
	else
	{
		switch (baseStatTotal) {
			case 0 ... 349:
				numShields = 1;
				break;
			case 350 ... 494:
				numShields = 2;
				break;
			case 495 ... 568:
				numShields = 3;
				break;
			case 569 ... 599:
				numShields = 4;
				break;
			default:
				numShields = MAX_NUM_RAID_SHIELDS;
		}
	}

	gNewBS->dynamaxData.shieldCount = numShields;
	LoadRaidShieldGfx();

	for (i = 0; i < MAX_NUM_RAID_SHIELDS; ++i)
		gNewBS->dynamaxData.shieldSpriteIds[i] = 0;

	for (i = 0; i < numShields - gNewBS->dynamaxData.shieldsDestroyed; ++i)
	{
		u8 spriteId = CreateSpriteAtEnd(&gRaidShieldSpriteTemplate, 0, 0, 0xFF);
		gNewBS->dynamaxData.shieldSpriteIds[i] = spriteId + 1;
		gSprites[spriteId].data[0] = bank;
		gSprites[spriteId].data[1] = -9 * i;
	}
}

u8 GetRaidBattleKOStatIncreaseAmount(u8 bank)
{
	u8 level = gBattleMons[bank].level;
	if (FlagGet(FLAG_BATTLE_FACILITY))
		level = MAX_LEVEL;

	switch (level) {
		case 0 ... 19:
			return 0; //Never
		case 20 ... 40:
			return 1;
		case 41 ... 70:
			return 2;
		default:
			return 3;
	}
}

u8 GetRaidBattleRepeatedAttackChance(u8 bank)
{
	u8 level = gBattleMons[bank].level;
	if (FlagGet(FLAG_BATTLE_FACILITY))
		level = MAX_LEVEL;

	switch (level) {
		case 0 ... 19:
			return 0; //Never
		case 20 ... 40:
			return 30; //30 % of the time after KO or Status Move
		case 41 ... 70:
			return 50; //50 % of the time after KO or Status Move
		default:
			return 70; //70 % of the time after KO or Status Move
	}
}

u8 GetRaidBattleStatNullificationChance(u8 bank)
{
	if (gDisableStructs[bank].isFirstTurn)
		return 0; //Don't use first attack with this

	u8 level = gBattleMons[bank].level;
	if (FlagGet(FLAG_BATTLE_FACILITY))
		level = MAX_LEVEL;

	switch (level) {
		case 0 ... 19:
			return 0; //Never
		case 20 ... 40:
			return 20; //20 % chance before each attack
		case 41 ... 70:
			return 35; //35 % chance before each attack
		default:
			return 50; //50 % chance before each attack
	}
}

bool8 IsRaidBossUsingRegularMove(u8 bank, u16 baseMove)
{
	return SPLIT(baseMove) == SPLIT_STATUS
		|| baseMove == MOVE_STRUGGLE
		|| (gRaidBattleStars < 4 && (gRandomTurnNumber & 3) == 0) //25 % chance to use regular damaging move
		|| (gRaidBattleStars >= 4 && (gRandomTurnNumber % 100 >= 90)) //Harder foes have a lower chance of using regular moves
		|| gBattleMons[bank].status2 & (STATUS2_RECHARGE | STATUS2_MULTIPLETURNS);
}

static u8 GetRaidMapSectionId(void)
{
	u8 currRegionMapSecId = GetCurrentRegionMapSectionId();

	#ifdef UNBOUND
	if (currRegionMapSecId == MAPSEC_ICY_HOLE
	||  currRegionMapSecId == MAPSEC_GRIM_WOODS)
		return currRegionMapSecId - MAPSEC_DYNAMIC;

	return Overworld_GetMapHeaderByGroupAndId((u16) gSaveBlock1->dynamicWarp.mapGroup, (u16) gSaveBlock1->dynamicWarp.mapNum) -> regionMapSectionId - MAPSEC_DYNAMIC;
	#else
	return currRegionMapSecId - MAPSEC_DYNAMIC;
	#endif
}

static u32 GetRaidRandomNumber(void)
{
	//Make sure no values are 0
	u8 dayOfWeek = (gClock.dayOfWeek == 0) ? 8 : gClock.dayOfWeek;
	u8 hour = (gClock.hour == 0) ? 24 : gClock.hour;
	u8 day = (gClock.day == 0) ? 32 : gClock.day;
	u8 month = (gClock.month == 0) ? 13 : gClock.month;
	u8 lastMapGroup = (gSaveBlock1->dynamicWarp.mapGroup == 0) ? 0xFF : gSaveBlock1->dynamicWarp.mapGroup;
	u8 lastMapNum = (gSaveBlock1->dynamicWarp.mapNum == 0) ? 0xFF : gSaveBlock1->dynamicWarp.mapNum;
	u8 lastWarpId = (gSaveBlock1->dynamicWarp.warpId == 0) ? 0xFF : gSaveBlock1->dynamicWarp.warpId;
	u16 lastPos = (gSaveBlock1->dynamicWarp.x + gSaveBlock1->dynamicWarp.y == 0) ? 0xFFFF : (u16) (gSaveBlock1->dynamicWarp.x + gSaveBlock1->dynamicWarp.y);

	return ((hour * (day + month) * lastMapGroup * (lastMapNum + lastWarpId + lastPos)) + ((hour * (day + month)) ^ dayOfWeek)) ^ T1_READ_32(gSaveBlock2->playerTrainerId);
}

void DetermineRaidStars(void)
{
	u8 numBadges = GetOpenWorldBadgeCount();
	if (FlagGet(FLAG_BATTLE_FACILITY))
		numBadges = 9; //Battle Frontier Demo

	u8 min = gRaidBattleStarsByBadges[numBadges][0];
	u8 max = gRaidBattleStarsByBadges[numBadges][1];
	u32 randomNum = GetRaidRandomNumber();

	if (min == max)
		gRaidBattleStars = min;
	else
		gRaidBattleStars = (randomNum % ((max + 1) - min)) + min;
}

//Must call DetermineRaidStars first
void DetermineRaidSpecies(void)
{
	u16 index;
	u8 numStars = gRaidBattleStars;
	const struct RaidData* raid = &gRaidsByMapSection[GetRaidMapSectionId()][numStars];

	if (FlagGet(FLAG_BATTLE_FACILITY)) //Battle Tower Demo
	{
		const struct BattleTowerSpread* spread;

		if (GetRaidRandomNumber() % 100 >= 90) //10 % chance
		{
			index = GetRaidRandomNumber() % gNumFrontierLegendarySpreads;
			spread = &gFrontierLegendarySpreads[index];
		}
		else
		{
			index = GetRaidRandomNumber() % gNumFrontierSpreads;
			spread = &gFrontierSpreads[index];
		}

		u16 megaSpecies = GetMegaSpecies(spread->species, spread->item, spread->moves); //Try Mega Evolve the mon
		if (megaSpecies != SPECIES_NONE)
			gRaidBattleSpecies = megaSpecies;
		else
			gRaidBattleSpecies = spread->species;

		gPokeBackupPtr = spread; //Save spread pointer for later
	}
	else if (raid->data != NULL)
	{
		index = GetRaidRandomNumber() % raid->amount;
		gRaidBattleSpecies = raid->data[index].species;
	}
	else
		gRaidBattleSpecies = SPECIES_NONE;
}

void DetermineRaidLevel(void)
{
	u8 numStars = gRaidBattleStars;
	u8 min = gRaidBattleLevelRanges[numStars][0];
	u8 max = gRaidBattleLevelRanges[numStars][1];
	u32 randomNum = GetRaidRandomNumber();

	if (FlagGet(FLAG_BATTLE_FACILITY)) //Battle Frontier Demo
		gRaidBattleLevel = VarGet(VAR_BATTLE_FACILITY_POKE_LEVEL);
	else if (min == max)
		gRaidBattleLevel = min;
	else
		gRaidBattleLevel = (randomNum % (max - min)) + min;
}

u8 GetRandomRaidLevel(void)
{
	if (FlagGet(FLAG_BATTLE_FACILITY)) //Battle Frontier Demo
		return VarGet(VAR_BATTLE_FACILITY_POKE_LEVEL);

	u8 numStars = gRaidBattleStars;
	return RandRange(gRaidBattleLevelRanges[numStars][0], gRaidBattleLevelRanges[numStars][1]);
}

u8 GetRaidRecommendedLevel(void)
{
	if (FlagGet(FLAG_BATTLE_FACILITY)) //Battle Frontier Demo
		return VarGet(VAR_BATTLE_FACILITY_POKE_LEVEL);

	u8 numStars = gRaidBattleStars;
	return gRaidBattleLevelRanges[numStars][1] + 5; //Max level + 5
}

void DetermineRaidPartners(bool8* checkedPartners, u8 maxPartners)
{
	u32 i, index;
	u8 numStars = gRaidBattleStars;
	u16 numMarked = 0;
	u16 numViable = 0;
	u32 randomNum = GetRaidRandomNumber();

	for (u32 i = 1; i < /*1000*/ 0xFFFFFFFF; ++i)
	{
		if (randomNum == 0) //0 causes an infinite loop
			randomNum = 0xFFFFFFFF;

		randomNum ^= i;
		index = randomNum % gNumRaidPartners;

		if (checkedPartners[index] == 0)
		{
			++numMarked;

			if (gRaidPartners[index].spreads[numStars] != NULL)
			{
				checkedPartners[index] = TRUE;
				++numViable;
			}
			else
				checkedPartners[index] = 0xFF;
		}

		if (numViable >= maxPartners) //Found at least 3 partners
			return;

		if (numMarked >= gNumRaidPartners)
			break;
	}

	if (numMarked < gNumRaidPartners) //Couldn't mark off everyone
	{
		for (i = 0; i < gNumRaidPartners; ++i)
		{
			if (gRaidPartners[i].spreads[numStars] != NULL)
			{
				checkedPartners[i] = TRUE;
				++numViable;
			}

			if (numViable >= maxPartners) //Found at least 3 partners
				return;
		}
	}
}

u8 GetRaidSpeciesAbilityNum(u16 species)
{
	u32 i = 0;
	u8 numStars = gRaidBattleStars;
	const struct RaidData* raid = &gRaidsByMapSection[GetRaidMapSectionId()][numStars];

	if (raid->data != NULL)
	{
		for (i = 0; i < raid->amount; ++i)
		{
			if (raid->data[i].species == species) //Max one species per dataset
				return raid->data[i].ability;
		}
	}

	return RAID_ABILITY_RANDOM_1_2;
}

u8 GetRaidEggMoveChance(void)
{
	return gRaidBattleEggMoveChances[gRaidBattleStars];
}

bool8 ShouldShowOpponentBattlerSprite(struct Pokemon* mon)
{
	return mon->hp > 0 || IsRaidBattle();
}

bool8 HasRaidBattleAlreadyBeenDone(void)
{
	return FlagGet(FIRST_RAID_BATTLE_FLAG + GetRaidMapSectionId());
}

void sp119_SetRaidBattleFlag(void)
{
	FlagSet(FIRST_RAID_BATTLE_FLAG + GetRaidMapSectionId());
}

//Input: Var8000 - 0: This Flag Only
//				   1: All Flags
void sp11A_ClearRaidBattleFlag(void)
{
	switch (Var8000) {
		case 0: //Clear this area's flag
			FlagClear(FIRST_RAID_BATTLE_FLAG + GetRaidMapSectionId());
			break;
		case 1: //Clear all flags:
			for (u32 i = 0; i < KANTO_MAPSEC_COUNT; ++i)
				FlagClear(FIRST_RAID_BATTLE_FLAG + i);
			break;
	}
}

void sp11B_AllRaidBattlesCompleted(void)
{
	gSpecialVar_LastResult = FALSE;

	for (u32 i = 0; i < KANTO_MAPSEC_COUNT; ++i)
	{
		if (gRaidsByMapSection[i] != NULL)
		{
			if (!FlagGet(FIRST_RAID_BATTLE_FLAG + i))
				return;
		}
	}

	gSpecialVar_LastResult = TRUE; //All Raid Battles available were completed
}

extern const u16 gItemsByType[];
u16 GetRaidRewardAmount(u16 item)
{
	if (GetPocketByItemId(item) == POCKET_POKE_BALLS)
		return Random() % 5 + 1; //1 - 5

	if (IsBerry(item))
		return Random() % 5 + 1; //1 - 5

	switch (gItemsByType[item]) {
		case ITEM_TYPE_HEALTH_RECOVERY:
		case ITEM_TYPE_STATUS_RECOVERY:
			return Random() % 5 + 1; //1 - 5
		case ITEM_TYPE_PP_RECOVERY:
		case ITEM_TYPE_STAT_BOOST_DRINK:
			return Random() % 3 + 1; //1 - 3
		case ITEM_TYPE_STAT_BOOST_WING:
			return Random() % 21 + 10; //10 - 30
		case ITEM_TYPE_SHARD:
			return Random() % 10 + 1; //1 - 10
		default:
			return 1;
	}
}

static const u16 s4StarFrontierRaidBattleDrops[] =
{
	/*100 %*/ ITEM_PP_UP,
	/* 80 %*/ ITEM_HP_UP,
	/* 80 %*/ ITEM_POMEG_BERRY,
	/* 50 %*/ ITEM_LIECHI_BERRY,
	/* 50 %*/ ITEM_FULL_RESTORE,
	/* 30 %*/ ITEM_NORMAL_GEM,
	/* 30 %*/ ITEM_NONE,
	/* 25 %*/ ITEM_HEART_SCALE,
	/* 25 %*/ ITEM_NONE,
	/*  5 %*/ ITEM_LEFTOVERS,
	/*  4 %*/ ITEM_BOTTLE_CAP,
	/*  1 %*/ ITEM_GOLD_BOTTLE_CAP,
};

static const u16 s56StarFrontierRaidBattleDrops[] =
{
	/*100 %*/ ITEM_PP_UP,
	/* 80 %*/ ITEM_HP_UP,
	/* 80 %*/ ITEM_POMEG_BERRY,
	/* 50 %*/ ITEM_LIECHI_BERRY,
	/* 50 %*/ ITEM_OCCA_BERRY,
	/* 30 %*/ ITEM_NORMAL_GEM,
	/* 30 %*/ ITEM_HEART_SCALE,
	/* 25 %*/ ITEM_PP_MAX,
	/* 25 %*/ ITEM_LEFTOVERS,
	/*  5 %*/ ITEM_WISHING_PIECE,
	/*  4 %*/ ITEM_BOTTLE_CAP,
	/*  1 %*/ ITEM_GOLD_BOTTLE_CAP,
};

static u16 ModifyFrontierRaidDropItem(u16 item)
{
	switch (item) {
		case ITEM_HP_UP: //HP UP to Carbos
			item += RandRange(0, NUM_STATS);
			if (item >= ITEM_RARE_CANDY) //For some reason it comes before Zinc
				item = ITEM_ZINC;
			break;
		case ITEM_POMEG_BERRY:
			item += RandRange(0, NUM_STATS); //Pomeg - Tamato
			break;
		case ITEM_LIECHI_BERRY:
			item += RandRange(0, NUM_STATS + 1); //Liechi - Starf
			break;
		#ifdef UNBOUND
		case ITEM_OCCA_BERRY:
			item += RandRange(0, 24); //Occa Berry - Maranga Berry
			break;
		case ITEM_NORMAL_GEM:
			item += RandRange(0, NUM_HIDDEN_POWER_TYPES + 2); //Normal Gem - Fairy Gem
			break;
		#endif
	}

	return item;
}

//Input: VAR_TEMP_0 = 0
void sp11C_GiveRaidBattleRewards(void)
{
	u32 i = 0;
	u8 numStars = gRaidBattleStars;
	const struct RaidData* raid = &gRaidsByMapSection[GetRaidMapSectionId()][numStars];
	gSpecialVar_LastResult = FALSE; //Not done giving rewards

	if (raid->data != NULL)
	{
		for (i = 0; i < raid->amount; ++i)
		{
			if (raid->data[i].species == gRaidBattleSpecies) //Max one species per dataset
			{
				for (; VarGet(VAR_TEMP_0) < MAX_RAID_DROPS; ++*(GetVarPointer(VAR_TEMP_0)))
				{
					if (raid->data[i].drops[VarGet(VAR_TEMP_0)] != ITEM_NONE
					&& Random() % 100 < sRaidBattleDropRates[VarGet(VAR_TEMP_0)])
					{
						gSpecialVar_LastTalked = 0xFD; //So no event objects disappear
						Var8000 = raid->data[i].drops[(*(GetVarPointer(VAR_TEMP_0)))++];
						Var8001 = GetRaidRewardAmount(Var8000);
						return;
					}
				}
			}
		}
	}
	else if (FlagGet(FLAG_BATTLE_FACILITY))
	{
		for (; VarGet(VAR_TEMP_0) < MAX_RAID_DROPS; ++*(GetVarPointer(VAR_TEMP_0)))
		{
			const u16* table = (gRaidBattleStars <= 4) ? s4StarFrontierRaidBattleDrops : s56StarFrontierRaidBattleDrops;

			if (table[VarGet(VAR_TEMP_0)] != ITEM_NONE
			&& Random() % 100 < sRaidBattleDropRates[VarGet(VAR_TEMP_0)])
			{
				gSpecialVar_LastTalked = 0xFD; //So no event objects disappear
				Var8000 = table[(*(GetVarPointer(VAR_TEMP_0)))++];
				Var8000 = ModifyFrontierRaidDropItem(Var8000);
				Var8001 = GetRaidRewardAmount(Var8000);
				return;
			}
		}
	}

	gSpecialVar_LastResult = TRUE; //Done giving rewards
}
