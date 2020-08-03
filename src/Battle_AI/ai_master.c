#include "../defines.h"
#include "../defines_battle.h"
#include "../../include/random.h"
#include "../../include/constants/items.h"
#include "../../include/constants/item_effects.h"
#include "../../include/constants/trainers.h"

#include "../../include/new/ai_advanced.h"
#include "../../include/new/ai_util.h"
#include "../../include/new/ai_master.h"
#include "../../include/new/ai_scripts.h"
#include "../../include/new/battle_controller_opponent.h"
#include "../../include/new/battle_start_turn_start.h"
#include "../../include/new/battle_util.h"
#include "../../include/new/damage_calc.h"
#include "../../include/new/frontier.h"
#include "../../include/new/end_turn_battle_scripts.h"
#include "../../include/new/general_bs_commands.h"
#include "../../include/new/util.h"
#include "../../include/new/mega.h"
#include "../../include/new/multi.h"
#include "../../include/new/move_tables.h"
#include "../../include/new/set_z_effect.h"
#include "../../include/new/switching.h"

/*
ai_master.c
	The master function(s) for the AI logic.
*/

// AI states
enum
{
	AIState_SettingUp,
	AIState_Processing,
	AIState_FinishedProcessing,
	AIState_DoNotProcess,
};

struct SmartWildMons
{
	u16 species;
	u32 aiFlags;
};

#ifndef WILD_ALWAYS_SMART
static struct SmartWildMons sSmartWildAITable[] =
{
	{SPECIES_ARTICUNO, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_ZAPDOS, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_MOLTRES, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_MEWTWO, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_MEW, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_RAIKOU, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_ENTEI, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_SUICUNE, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_LUGIA, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_HO_OH, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_CELEBI, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_REGIROCK, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_REGICE, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_REGISTEEL, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_LATIAS, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_LATIOS, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_KYOGRE, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_GROUDON, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_RAYQUAZA, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_JIRACHI, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_DEOXYS, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_DEOXYS_ATTACK, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_DEOXYS_DEFENSE, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_DEOXYS_SPEED, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_AZELF, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_MESPRIT, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_UXIE, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_DIALGA, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_PALKIA, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_GIRATINA, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_GIRATINA_ORIGIN, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_CRESSELIA, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_HEATRAN, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_REGIGIGAS, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_PHIONE, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_MANAPHY, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_DARKRAI, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_SHAYMIN, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_SHAYMIN_SKY, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_ARCEUS, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_ARCEUS_FIGHT, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_ARCEUS_FLYING, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_ARCEUS_POISON, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_ARCEUS_GROUND, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_ARCEUS_ROCK, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_ARCEUS_BUG, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_ARCEUS_GHOST, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_ARCEUS_STEEL, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_ARCEUS_FIRE, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_ARCEUS_WATER, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_ARCEUS_GRASS, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_ARCEUS_ELECTRIC, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_ARCEUS_PSYCHIC, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_ARCEUS_ICE, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_ARCEUS_DRAGON, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_ARCEUS_DARK, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_ARCEUS_FAIRY, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_VICTINI, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_COBALION, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_TERRAKION, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_VIRIZION, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_TORNADUS, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_TORNADUS_THERIAN, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_THUNDURUS, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_THUNDURUS_THERIAN, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_LANDORUS, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_LANDORUS_THERIAN, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_RESHIRAM, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_ZEKROM, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_KYUREM, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_KYUREM_BLACK, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_KYUREM_WHITE, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_KELDEO, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_KELDEO_RESOLUTE, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_MELOETTA, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_MELOETTA_PIROUETTE, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_GENESECT, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_GENESECT_SHOCK, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_GENESECT_BURN, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_GENESECT_CHILL, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_GENESECT_DOUSE, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_XERNEAS, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_YVELTAL, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_ZYGARDE, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_ZYGARDE_10, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_DIANCIE, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_HOOPA, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_HOOPA_UNBOUND, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_VOLCANION, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_TYPE_NULL, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_SILVALLY, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_TAPU_KOKO, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_TAPU_LELE, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_TAPU_BULU, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_TAPU_FINI, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_COSMOG, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_COSMOEM, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_SOLGALEO, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_LUNALA, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_NIHILEGO, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_BUZZWOLE, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_PHEROMOSA, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_XURKITREE, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_CELESTEELA, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_KARTANA, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_GUZZLORD, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_POIPOLE, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_NAGANADEL, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_STAKATAKA, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_BLACEPHALON, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_MAGEARNA, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_MAGEARNA_P, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_MARSHADOW, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_NECROZMA, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_NECROZMA_DUSK_MANE, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_NECROZMA_DAWN_WINGS, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_ZERAORA, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_MELTAN, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_MELMETAL, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_ZACIAN, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_ZACIAN_CROWNED, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_ZAMAZENTA, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_ZAMAZENTA_CROWNED, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_ETERNATUS, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_ZARUDE, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_KUBFU, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_URSHIFU_SINGLE, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_URSHIFU_RAPID, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_CALYREX, AI_SCRIPT_CHECK_BAD_MOVE},
	{0xFFFF, 0}
};
#endif

static u8 (*const sBattleAIScriptTable[])(const u8, const u8, const u16, const u8, struct AIScript*) =
{
	[0] = AIScript_Negatives,
	[1] = AIScript_SemiSmart,
	[2] = AIScript_Positives,

	[29] = AIScript_Roaming,
	[30] = AIScript_Safari,
	[31] = AIScript_FirstBattle,
};

//This file's functions:
static u8 ChooseMoveOrAction_Singles(struct AIScript* aiScriptData);
static u8 ChooseMoveOrAction_Doubles(struct AIScript* aiScriptData);
static void BattleAI_DoAIProcessing(struct AIScript* aiScriptData);
static void CalculateAIPredictions(void);
static bool8 ShouldSwitch(void);
static bool8 ShouldSwitchIfOnlyBadMovesLeft(void);
static bool8 FindMonThatAbsorbsOpponentsMove(void);
static bool8 ShouldSwitchIfNaturalCureOrRegenerator(void);
static bool8 PassOnWish(void);
static bool8 SemiInvulnerableTroll(void);
static u8 GetBestPartyNumberForSemiInvulnerableLockedMoveCalcs(u8 opposingBattler1, u8 opposingBattler2, bool8 checkLockedMoves);
static bool8 RunAllSemiInvulnerableLockedMoveCalcs(u8 opposingBattler1, u8 opposingBattler2, bool8 checkLockedMoves);
static bool8 TheCalcForSemiInvulnerableTroll(u8 bankAtk, u8 flags, bool8 checkLockedMoves);
static bool8 CanStopLockedMove(void);
static bool8 IsYawned(void);
static bool8 IsTakingAnnoyingSecondaryDamage(void);
static bool8 ShouldSwitchToAvoidDeath(void);
static bool8 ShouldSwitchIfWonderGuard(void);
static void CalcMostSuitableMonSwitchIfNecessary(void);
static void PredictMovesForBanks(void);
static void RunCalcShouldAIDynamax(void);
static void UpdateStrongestMoves(void);
static void UpdateBestDoublesKillingMoves(void);
static u32 GetMaxByteIndexInList(const u8 array[], const u32 size);
static u8 GetAI_ItemType(u16 itemId, const u8 *itemEffect); //Fixed from vanilla
static bool8 ShouldAIUseItem(void);
#ifdef VAR_GAME_DIFFICULTY
static bool8 IsGoodIdeaToDoShiftSwitch(u8 switchBank, u8 foe);
#endif

void __attribute__((long_call)) RecordLastUsedMoveByTarget(void);

void BattleAI_HandleItemUseBeforeAISetup(void)
{
	u32 i;
	u8* data = (u8*)BATTLE_HISTORY;

	for (i = 0; i < sizeof(struct BattleHistory); i++)
		data[i] = 0;

	// Items are allowed to use in ONLY trainer battles.
	if ((gBattleTypeFlags & BATTLE_TYPE_TRAINER)
		&& !(gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_SAFARI | BATTLE_TYPE_FRONTIER | BATTLE_TYPE_TWO_OPPONENTS | BATTLE_TYPE_INGAME_PARTNER | BATTLE_TYPE_EREADER_TRAINER))
		&& gTrainerBattleOpponent_A != SECRET_BASE_OPPONENT
		&& !IsFrontierTrainerId(gTrainerBattleOpponent_A))
	{
		for (i = 0; i < 4; i++)
		{
			if (gTrainers[gTrainerBattleOpponent_A].items[i] != 0)
			{
				BATTLE_HISTORY->trainerItems[BATTLE_HISTORY->itemsNo] = gTrainers[gTrainerBattleOpponent_A].items[i];
				BATTLE_HISTORY->itemsNo++;
			}
		}
	}

	BattleAI_SetupAIData(0xF);
}

void BattleAI_SetupAIData(u8 defaultScoreMoves)
{
	u32 i;
	u8* data = (u8*) AI_THINKING_STRUCT;
	u8 moveLimitations;

	// Clear AI data.
	for (i = 0; i < sizeof(struct AI_ThinkingStruct); ++i)
		data[i] = 0;

	// Conditional score reset, unlike Ruby.
	for (i = 0; i < MAX_MON_MOVES; ++i)
	{
		if (defaultScoreMoves & 1)
			AI_THINKING_STRUCT->score[i] = 100;
		else
			AI_THINKING_STRUCT->score[i] = 0;

		defaultScoreMoves >>= 1;
	}

	gBattleResources->AIScriptsStack->size = 0;
	gBankAttacker = gActiveBattler;

	// Decide a random target battlerId in doubles.
	if (IS_DOUBLE_BATTLE)
	{
		gBankTarget = (Random() & BIT_FLANK) + (SIDE(gActiveBattler) ^ BIT_SIDE);
		if (gAbsentBattlerFlags & gBitTable[gBankTarget])
			gBankTarget ^= BIT_FLANK;
	}
	// There's only one choice in single battles.
	else
		gBankTarget = gBankAttacker ^ BIT_SIDE;

	moveLimitations = CheckMoveLimitations(gActiveBattler, 0, AdjustMoveLimitationFlagsForAI(gBankAttacker, gBankTarget));

	// Ignore moves that aren't possible to use.
	for (i = 0; i < MAX_MON_MOVES; i++)
	{
		if (gBitTable[i] & moveLimitations)
			AI_THINKING_STRUCT->score[i] = 0;

		AI_THINKING_STRUCT->simulatedRNG[i] = 100 - umodsi(Random(), 16);
	}

	// Choose proper trainer ai scripts.
	AI_THINKING_STRUCT->aiFlags = GetAIFlags();

	//if (IS_DOUBLE_BATTLE)
		//AI_THINKING_STRUCT->aiFlags |= AI_SCRIPT_DOUBLE_BATTLE; // act smart in doubles and don't attack your partner
}

u32 GetAIFlags(void)
{
	u32 flags;

	#ifdef VAR_GAME_DIFFICULTY
	u8 difficulty = VarGet(VAR_GAME_DIFFICULTY);
	#endif

	if (gBattleTypeFlags & BATTLE_TYPE_SAFARI)
		flags = AI_SCRIPT_SAFARI;
	else if (gBattleTypeFlags & BATTLE_TYPE_ROAMER)
		flags = AI_SCRIPT_ROAMING | WildMonIsSmart(gActiveBattler);
	else if (gBattleTypeFlags & BATTLE_TYPE_OAK_TUTORIAL)
		flags = AI_SCRIPT_FIRST_BATTLE;
	else if (gBattleTypeFlags & (BATTLE_TYPE_FRONTIER) || IsFrontierTrainerId(gTrainerBattleOpponent_A))
		flags = GetAIFlagsInBattleFrontier(gActiveBattler);
	else if (gBattleTypeFlags & (BATTLE_TYPE_EREADER_TRAINER | BATTLE_TYPE_TRAINER_TOWER) && gTrainerBattleOpponent_A != TRAINER_SECRET_BASE)
		flags = AI_SCRIPT_CHECK_BAD_MOVE | AI_SCRIPT_CHECK_GOOD_MOVE;
	else if (IsRaidBattle())
		flags = AI_SCRIPT_CHECK_BAD_MOVE | AI_SCRIPT_CHECK_GOOD_MOVE; //Make partner smart
	else if (gBattleTypeFlags & BATTLE_TYPE_SCRIPTED_WILD_2) //No idea how these two work
		flags = AI_SCRIPT_CHECK_BAD_MOVE;
	else if (gBattleTypeFlags & BATTLE_TYPE_SCRIPTED_WILD_3)
		flags = AI_SCRIPT_CHECK_BAD_MOVE | AI_SCRIPT_SEMI_SMART;
	else
	{
		if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS)
			flags = gTrainers[gTrainerBattleOpponent_A].aiFlags | gTrainers[VarGet(VAR_SECOND_OPPONENT)].aiFlags;
		else
			flags = gTrainers[gTrainerBattleOpponent_A].aiFlags;

		#ifdef VAR_GAME_DIFFICULTY
		if (difficulty == OPTIONS_EASY_DIFFICULTY && gBattleTypeFlags & BATTLE_TYPE_TRAINER)
			flags = AI_SCRIPT_CHECK_BAD_MOVE; //Trainers are always barely smart in easy mode
		else if (difficulty == OPTIONS_HARD_DIFFICULTY && gBattleTypeFlags & BATTLE_TYPE_TRAINER)
		{
			if (!(flags & AI_SCRIPT_CHECK_GOOD_MOVE)) //Not Trainers who are already smart
				flags |= AI_SCRIPT_SEMI_SMART; //Regular Trainers are always semi smart in hard mode
		}
		else if (difficulty == OPTIONS_EXPERT_DIFFICULTY)
		{
			if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
			{
				if (!(flags & AI_SCRIPT_CHECK_GOOD_MOVE)) //Not Trainers who are already smart
					flags |= AI_SCRIPT_SEMI_SMART; //Regular Trainers are always semi smart in expert mode
			}
			else
				flags = AI_SCRIPT_CHECK_BAD_MOVE; //Even Wild Pokemon are moderately smart in expert mode
		}
		#endif
	}

	if (!(gBattleTypeFlags & BATTLE_TYPE_TRAINER) && gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER)
		flags |= AI_SCRIPT_CHECK_BAD_MOVE; //Partners in wild double battles are like normal trainers

	return flags;
}

#define NUM_COPY_STATS STAT_SPDEF
u8 BattleAI_ChooseMoveOrAction(void)
{
	u16 savedCurrentMove = gCurrentMove;
	u8 ret;
	struct AIScript aiScriptData = {0}; //Do this now to save time during the processing
	PopulateAIScriptStructWithBaseAttackerData(&aiScriptData, gBankAttacker);

	struct BattlePokemon backupMonAtk, backupMonDef;
	u8 backupAbilityAtk = ABILITY_NONE; u8 backupAbilityDef = ABILITY_NONE;
	u16 backupSpeciesAtk = SPECIES_NONE; u16 backupSpeciesDef = SPECIES_NONE;

	TryTempMegaEvolveBank(gBankAttacker, &backupMonAtk, &backupSpeciesAtk, &backupAbilityAtk);

	if (IS_SINGLE_BATTLE)
	{
		TryTempMegaEvolveBank(gBankTarget, &backupMonDef, &backupSpeciesDef, &backupAbilityDef);
		ret = ChooseMoveOrAction_Singles(&aiScriptData);
	}
	else
		ret = ChooseMoveOrAction_Doubles(&aiScriptData);

	TryRevertTempMegaEvolveBank(gBankAttacker, &backupMonAtk, &backupSpeciesAtk, &backupAbilityAtk);
	TryRevertTempMegaEvolveBank(gBankTarget, &backupMonDef, &backupSpeciesDef, &backupAbilityDef);

	gCurrentMove = savedCurrentMove;
	return ret;
}

void TryTempMegaEvolveBank(u8 bank, struct BattlePokemon* backupMon, u16* backupSpecies, u8* backupAbility)
{
	if (gNewBS->ai.megaPotential[bank] != NULL) //Mon will probably mega evolve
	{
		struct Pokemon* mon = GetBankPartyData(bank);
		*backupSpecies = mon->species;
		*backupAbility = *GetAbilityLocation(bank);
		Memcpy(backupMon, &gBattleMons[bank], sizeof(gBattleMons[bank]));

		mon->species = ((struct Evolution*) gNewBS->ai.megaPotential[bank])->targetSpecies;
		CalculateMonStats(mon); //Temporarily mega evolve mon
		Memcpy(&gBattleMons[bank].attack, &mon->attack, sizeof(u16) * NUM_COPY_STATS);
		*GetAbilityLocation(bank) = GetMonAbility(mon);
		if (gBattleTypeFlags & BATTLE_TYPE_CAMOMONS)
		{
			UpdateTypesForCamomons(bank);
		}
		else
		{
			gBattleMons[bank].type1 = gBaseStats[mon->species].type1;
			gBattleMons[bank].type2 = gBaseStats[mon->species].type2;
		}
	}
	else
	{
		*backupSpecies = SPECIES_NONE;
		*backupAbility = ABILITY_NONE;
	}
}

void TryRevertTempMegaEvolveBank(u8 bank, struct BattlePokemon* backupMon, u16* backupSpecies, u8* backupAbility)
{
	if (*backupSpecies != SPECIES_NONE)
	{
		struct Pokemon* mon = GetBankPartyData(bank);
		mon->species = *backupSpecies;
		CalculateMonStats(mon); //Revert from temp mega
		*GetAbilityLocation(bank) = *backupAbility;
		Memcpy(&gBattleMons[bank], backupMon, sizeof(gBattleMons[bank]));
	}
}

static u8 ChooseMoveOrAction_Singles(struct AIScript* aiScriptData)
{
	u8 currentMoveArray[4];
	u8 consideredMoveArray[4];
	u8 numOfBestMoves;
	s32 i;
	u32 flags = AI_THINKING_STRUCT->aiFlags;

	RecordLastUsedMoveByTarget();
	PopulateAIScriptStructWithBaseDefenderData(aiScriptData, gBankTarget);

	while (flags != 0)
	{
		if (flags & 1)
		{
			AI_THINKING_STRUCT->aiState = AIState_SettingUp;
			BattleAI_DoAIProcessing(aiScriptData);
		}
		flags >>= 1;
		AI_THINKING_STRUCT->aiLogicId++;
		AI_THINKING_STRUCT->movesetIndex = 0;
	}

	// Check special AI actions.
	if (AI_THINKING_STRUCT->aiAction & AI_ACTION_FLEE)
		return AI_CHOICE_FLEE;
	if (AI_THINKING_STRUCT->aiAction & AI_ACTION_WATCH)
		return AI_CHOICE_WATCH;

	numOfBestMoves = 1;
	currentMoveArray[0] = AI_THINKING_STRUCT->score[0];
	consideredMoveArray[0] = 0;

	for (i = 1; i < MAX_MON_MOVES; i++)
	{
		if (gBattleMons[gBankAttacker].moves[i] != MOVE_NONE)
		{
			// In ruby, the order of these if statements is reversed.
			if (currentMoveArray[0] == AI_THINKING_STRUCT->score[i])
			{
				currentMoveArray[numOfBestMoves] = AI_THINKING_STRUCT->score[i];
				consideredMoveArray[numOfBestMoves++] = i;
			}
			if (currentMoveArray[0] < AI_THINKING_STRUCT->score[i])
			{
				numOfBestMoves = 1;
				currentMoveArray[0] = AI_THINKING_STRUCT->score[i];
				consideredMoveArray[0] = i;
			}
		}
	}

	return consideredMoveArray[Random() % numOfBestMoves];
}

static u8 ChooseMoveOrAction_Doubles(struct AIScript* aiScriptData)
{
	s32 i;
	s32 j;
	u32 flags;
	s16 bestMovePointsForTarget[4];
	s8 mostViableTargetsArray[4];
	u8 actionOrMoveIndex[4];
	u8 mostViableMovesScores[4];
	u8 mostViableMovesIndices[4];
	s32 mostViableTargetsNo;
	s32 mostViableMovesNo;
	s16 mostMovePoints;

	for (i = 0; i < MAX_BATTLERS_COUNT; i++)
	{
		if (i == gBankAttacker || !BATTLER_ALIVE(i))
		{
			actionOrMoveIndex[i] = 0xFF;
			bestMovePointsForTarget[i] = -1;
		}
		else
		{
			BattleAI_SetupAIData(0xF);
			PopulateAIScriptStructWithBaseDefenderData(aiScriptData, gBankTarget = i);

			if ((i & BIT_SIDE) != (gBankAttacker & BIT_SIDE))
				RecordLastUsedMoveByTarget();

			AI_THINKING_STRUCT->aiLogicId = 0;
			AI_THINKING_STRUCT->movesetIndex = 0;
			flags = AI_THINKING_STRUCT->aiFlags;
			while (flags != 0)
			{
				if (flags & 1)
				{
					AI_THINKING_STRUCT->aiState = AIState_SettingUp;
					BattleAI_DoAIProcessing(aiScriptData);
				}

				flags >>= 1;
				AI_THINKING_STRUCT->aiLogicId++;
				AI_THINKING_STRUCT->movesetIndex = 0;
			}

			if (AI_THINKING_STRUCT->aiAction & AI_ACTION_FLEE)
			{
				actionOrMoveIndex[i] = AI_CHOICE_FLEE;
			}
			else if (AI_THINKING_STRUCT->aiAction & AI_ACTION_WATCH)
			{
				actionOrMoveIndex[i] = AI_CHOICE_WATCH;
			}
			else
			{
				mostViableMovesScores[0] = AI_THINKING_STRUCT->score[0];
				mostViableMovesIndices[0] = 0;
				mostViableMovesNo = 1;
				for (j = 1; j < MAX_MON_MOVES; j++)
				{
					if (gBattleMons[gBankAttacker].moves[j] != MOVE_NONE)
					{
						if (mostViableMovesScores[0] == AI_THINKING_STRUCT->score[j])
						{
							mostViableMovesScores[mostViableMovesNo] = AI_THINKING_STRUCT->score[j];
							mostViableMovesIndices[mostViableMovesNo] = j;
							mostViableMovesNo++;
						}
						if (mostViableMovesScores[0] < AI_THINKING_STRUCT->score[j])
						{
							mostViableMovesScores[0] = AI_THINKING_STRUCT->score[j];
							mostViableMovesIndices[0] = j;
							mostViableMovesNo = 1;
						}
					}
				}

				actionOrMoveIndex[i] = mostViableMovesIndices[Random() % mostViableMovesNo];
				bestMovePointsForTarget[i] = mostViableMovesScores[0];

				// Don't use a move against ally if it has less than 101 points.
				if (i == (gBankAttacker ^ BIT_FLANK) && bestMovePointsForTarget[i] < 101)
				{
					bestMovePointsForTarget[i] = -1;
				}
			}
		}
	}

	mostMovePoints = bestMovePointsForTarget[0];
	mostViableTargetsArray[0] = 0;
	mostViableTargetsNo = 1;

	bool8 statusMoveOption = FALSE;
	u32 mostDamage = (actionOrMoveIndex[0] < MAX_MON_MOVES && SPLIT(actionOrMoveIndex[0]) != SPLIT_STATUS) ?
		GetFinalAIMoveDamage(actionOrMoveIndex[0], gBankAttacker, mostViableTargetsArray[0], 1, NULL) : 0;

	struct DamageCalc damageData = {0};
	damageData.bankAtk = gBankAttacker;
	PopulateDamageCalcStructWithBaseAttackerData(&damageData);
	for (i = 1; i < MAX_BATTLERS_COUNT; i++)
	{
		if (bestMovePointsForTarget[i] == mostMovePoints)
		{
			u16 move = actionOrMoveIndex[i];
			u16 pastMove = actionOrMoveIndex[i - 1];
			u32 thisDamage = 0;
			statusMoveOption = move < MAX_MON_MOVES && SPLIT(move) == SPLIT_STATUS;

			if (move < MAX_MON_MOVES //Move was chosen
			&&  pastMove < MAX_MON_MOVES
			&&  SPLIT(move) != SPLIT_STATUS
			&&  SPLIT(pastMove) != SPLIT_STATUS)
			{
				//Choose the target which the most damage can be done to
				damageData.bankDef = i;
				PopulateDamageCalcStructWithBaseDefenderData(&damageData);
				thisDamage = GetFinalAIMoveDamage(move, gBankAttacker, i, 1, &damageData);
				if (thisDamage <= mostDamage)
					continue; //Don't store this target if less damage can be done to it

				if (!statusMoveOption)
				{
					mostViableTargetsArray[0] = i;
					mostViableTargetsNo = 1;
					mostDamage = thisDamage;
					continue;
				}
			}

			mostViableTargetsArray[mostViableTargetsNo] = i;
			mostViableTargetsNo++;
		}
		else if (bestMovePointsForTarget[i] > mostMovePoints)
		{
			mostMovePoints = bestMovePointsForTarget[i];
			mostViableTargetsArray[0] = i;
			mostViableTargetsNo = 1;
		}
	}

	gBankTarget = mostViableTargetsArray[Random() % mostViableTargetsNo];
	return actionOrMoveIndex[gBankTarget];
}

static void BattleAI_DoAIProcessing(struct AIScript* aiScriptData)
{
	while (AI_THINKING_STRUCT->aiState != AIState_FinishedProcessing)
	{
		switch (AI_THINKING_STRUCT->aiState)
		{
			case AIState_DoNotProcess: // Needed to match.
				break;
			case AIState_SettingUp:
				if (gBattleMons[gBankAttacker].pp[AI_THINKING_STRUCT->movesetIndex] == 0)
					AI_THINKING_STRUCT->moveConsidered = MOVE_NONE;
				else
					AI_THINKING_STRUCT->moveConsidered = gBattleMons[gBankAttacker].moves[AI_THINKING_STRUCT->movesetIndex];

				AI_THINKING_STRUCT->aiState++;
				break;

			case AIState_Processing:
				if (AI_THINKING_STRUCT->moveConsidered != MOVE_NONE
				&&  AI_THINKING_STRUCT->score[AI_THINKING_STRUCT->movesetIndex] > 0)
				{
					if (AI_THINKING_STRUCT->aiLogicId < ARRAY_COUNT(sBattleAIScriptTable) //AI Script exists
					&&  sBattleAIScriptTable[AI_THINKING_STRUCT->aiLogicId] != NULL)
					{
						AI_THINKING_STRUCT->score[AI_THINKING_STRUCT->movesetIndex] =
							sBattleAIScriptTable[AI_THINKING_STRUCT->aiLogicId](gBankAttacker,
																				gBankTarget,
																				AI_THINKING_STRUCT->moveConsidered,
																				AI_THINKING_STRUCT->score[AI_THINKING_STRUCT->movesetIndex],
																				aiScriptData); //Run AI script
					}
				}
				else
				{
					AI_THINKING_STRUCT->score[AI_THINKING_STRUCT->movesetIndex] = 0;
				}

				AI_THINKING_STRUCT->movesetIndex++;

				if (AI_THINKING_STRUCT->movesetIndex < MAX_MON_MOVES && !(AI_THINKING_STRUCT->aiAction & AI_ACTION_DO_NOT_ATTACK))
					AI_THINKING_STRUCT->aiState = AIState_SettingUp;
				else
					AI_THINKING_STRUCT->aiState++;

			break;
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void AI_TrySwitchOrUseItem(void)
{
	struct Pokemon* party;
	u8 battlerIn1, battlerIn2;
	u8 firstId, lastId;
	bool8 ret = FALSE;

	struct BattlePokemon backupMonAtk;
	u8 backupAbilityAtk = ABILITY_NONE;
	u16 backupSpeciesAtk = SPECIES_NONE;

	if (RAID_BATTLE_END)
		goto DONT_THINK;

	//Calulate everything important now to save as much processing time as possible later
	CalculateAIPredictions();

	party = LoadPartyRange(gActiveBattler, &firstId, &lastId);

	if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
	{
		TryTempMegaEvolveBank(gActiveBattler, &backupMonAtk, &backupSpeciesAtk, &backupAbilityAtk);

		if (ShouldSwitch()) //0x8039A80
		{
			if (gBattleStruct->switchoutIndex[SIDE(gActiveBattler)] == PARTY_SIZE)
			{
				u8 monToSwitchId = GetMostSuitableMonToSwitchInto();
				if (monToSwitchId == PARTY_SIZE)
				{
					if (!(IS_DOUBLE_BATTLE))
					{
						battlerIn1 = gActiveBattler;
						battlerIn2 = battlerIn1;
					}
					else
					{
						battlerIn1 = gActiveBattler;
						if (gAbsentBattlerFlags & gBitTable[PARTNER(gActiveBattler)])
							battlerIn2 = battlerIn1;
						else
							battlerIn2 = PARTNER(battlerIn1);
					}

					for (monToSwitchId = firstId; monToSwitchId < lastId; ++monToSwitchId)
					{
						if (party[monToSwitchId].hp == 0
						||  GetMonData(&party[monToSwitchId], MON_DATA_IS_EGG, 0)
						||  monToSwitchId == gBattlerPartyIndexes[battlerIn1]
						||	monToSwitchId == gBattlerPartyIndexes[battlerIn2]
						||	monToSwitchId == gBattleStruct->monToSwitchIntoId[battlerIn1]
						||	monToSwitchId == gBattleStruct->monToSwitchIntoId[battlerIn2])
							continue;

						break;
					}
				}

				gBattleStruct->switchoutIndex[SIDE(gActiveBattler)] = monToSwitchId;
			}

			gBattleStruct->monToSwitchIntoId[gActiveBattler] = gBattleStruct->switchoutIndex[SIDE(gActiveBattler)];
			ret = TRUE;
		}
		else if (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER && GetBattlerPosition(gActiveBattler) == B_POSITION_PLAYER_RIGHT)
		{
			//Partner isn't allowed to use items
		}
		else if (ShouldAIUseItem())
			ret = TRUE;

		TryRevertTempMegaEvolveBank(gActiveBattler, &backupMonAtk, &backupSpeciesAtk, &backupAbilityAtk);
		if (ret) return;
	}

DONT_THINK:
	//mgba_printf(MGBA_LOG_INFO, "AI thinking complete.");
	EmitTwoReturnValues(1, ACTION_USE_MOVE, (gActiveBattler ^ BIT_SIDE) << 8);
}

static void CalculateAIPredictions(void)
{
	if (!gNewBS->calculatedAIPredictions) //Only calculate these things once per turn
	{
		//mgba_printf(MGBA_LOG_INFO, "Calculating strongest moves...");
		UpdateStrongestMoves();
		//mgba_printf(MGBA_LOG_WARN, "Calculating doubles killing moves...");
		UpdateBestDoublesKillingMoves(); //Takes long time
		//mgba_printf(MGBA_LOG_INFO, "Predicting moves..");
		PredictMovesForBanks(); //Takes long time
		//mgba_printf(MGBA_LOG_WARN, "Calculating Dynamax mon...");
		RunCalcShouldAIDynamax(); //Allows move predictions to change outcome
		//mgba_printf(MGBA_LOG_INFO, "Calculating switching...");

		gNewBS->calculatedAIPredictions = TRUE;

		u8 backupBattler = gActiveBattler;
		for (int i = 0; i < gBattlersCount; ++i)
		{
			if (GetBattlerPosition(i) == B_POSITION_PLAYER_LEFT && !(gBattleTypeFlags & BATTLE_TYPE_MOCK_BATTLE))
				continue; //Only calculate for player if player not in control

			if (GetBattlerPosition(i) == B_POSITION_PLAYER_RIGHT && !IsTagBattle())
				continue; //Only calculate for player if player not in control

			if (gNewBS->ai.calculatedAISwitchings[i] && BATTLER_ALIVE(i)) //So Multi Battles still work properly
			{
				ResetBestMonToSwitchInto(i);
				gNewBS->ai.calculatedAISwitchings[gActiveBattler] = FALSE;

				if (!BankSideHasTwoTrainers(gActiveBattler))
					gNewBS->ai.calculatedAISwitchings[PARTNER(gActiveBattler)] = FALSE;
			}
		}
		gActiveBattler = backupBattler;
	}
}

static bool8 ShouldSwitch(void)
{
	u8 battlerIn1, battlerIn2;
	u8 firstId, lastId;
	u8 availableToSwitch;
	struct Pokemon *party;
	int i;

	if (IsTrapped(gActiveBattler, TRUE))
		return FALSE;

	availableToSwitch = 0;
	if (IS_DOUBLE_BATTLE)
	{
		battlerIn1 = gActiveBattler;
		if (gAbsentBattlerFlags & gBitTable[GetBattlerAtPosition(GetBattlerPosition(gActiveBattler) ^ BIT_FLANK)])
			battlerIn2 = gActiveBattler;
		else
			battlerIn2 = GetBattlerAtPosition(GetBattlerPosition(gActiveBattler) ^ BIT_FLANK);
	}
	else
	{
		battlerIn1 = gActiveBattler;
		battlerIn2 = gActiveBattler;
	}

	party = LoadPartyRange(gActiveBattler, &firstId, &lastId);

	for (i = firstId; i < lastId; ++i)
	{
		if (party[i].hp == 0
		||	GetMonData(&party[i], MON_DATA_SPECIES2, 0) == SPECIES_NONE
		|| 	GetMonData(&party[i], MON_DATA_IS_EGG, 0)
		||	i == gBattlerPartyIndexes[battlerIn1]
		||	i == gBattlerPartyIndexes[battlerIn2]
		||	i == gBattleStruct->monToSwitchIntoId[battlerIn1]
		||  i == gBattleStruct->monToSwitchIntoId[battlerIn2])
			continue;

		++availableToSwitch;
	}

	if (availableToSwitch == 0)
		return FALSE;
	if (ShouldSwitchIfOnlyBadMovesLeft())
		return TRUE;
	if (ShouldSwitchIfPerishSong())
		return TRUE;
	if (ShouldSwitchIfWonderGuard())
		return TRUE;
	if (FindMonThatAbsorbsOpponentsMove())
		return TRUE;
	if (ShouldSwitchIfNaturalCureOrRegenerator())
		return TRUE;
	if (PassOnWish())
		return TRUE;
	if (SemiInvulnerableTroll())
		return TRUE;
	if (CanStopLockedMove())
		return TRUE;
	if (IsYawned())
		return TRUE;
	if (IsTakingAnnoyingSecondaryDamage())
		return TRUE;
	if (ShouldSwitchToAvoidDeath())
		return TRUE;
	//if (CanKillAFoe(gActiveBattler))
	//	return FALSE;
	//if (AreStatsRaised())
	//	return FALSE;

	return FALSE;
}

void LoadBattlersAndFoes(u8* battlerIn1, u8* battlerIn2, u8* foe1, u8* foe2)
{
	if (IS_DOUBLE_BATTLE)
	{
		*battlerIn1 = gActiveBattler;
		if (gAbsentBattlerFlags & gBitTable[PARTNER(gActiveBattler)])
			*battlerIn2 = gActiveBattler;
		else
			*battlerIn2 = PARTNER(gActiveBattler);

		if (gAbsentBattlerFlags & gBitTable[FOE(gActiveBattler)])
			*foe1 = *foe2 = PARTNER(FOE(gActiveBattler));
		else if (gAbsentBattlerFlags & gBitTable[PARTNER(FOE(gActiveBattler))])
			*foe1 = *foe2 = FOE(gActiveBattler);
		else
		{
			*foe1 = FOE(gActiveBattler);
			*foe2 = PARTNER(FOE(gActiveBattler));
		}
	}
	else
	{
		*battlerIn1 = gActiveBattler;
		*battlerIn2 = gActiveBattler;
		*foe1 = FOE(gActiveBattler);
		*foe2 = *foe1;
	}
}

static bool8 PredictedMoveWontDoTooMuchToMon(u8 activeBattler, struct Pokemon* mon, u8 foe)
{
	u16 defMove = IsValidMovePrediction(foe, activeBattler);
	u32 predictedDmg = (defMove == MOVE_NONE) ? 0 : AI_CalcMonDefDmg(foe, activeBattler, defMove, mon, NULL);

	if (predictedDmg == 0)
		return TRUE;

	if (predictedDmg >= mon->hp)
		return FALSE; //Don't switch and sack your other mon

	if (predictedDmg < mon->hp / 2)
		return TRUE; //Can take multiple hits

	return predictedDmg < mon->maxHP / 2
		&& HealingMoveInMonMoveset(mon)
		&& SpeedCalcMon(SIDE(activeBattler), mon) > SpeedCalc(foe); //Has time to heal
}

static bool8 ShouldSwitchIfOnlyBadMovesLeft(void)
{
	u8 battlerIn1, battlerIn2;
	u8 foe1, foe2;
	LoadBattlersAndFoes(&battlerIn1, &battlerIn2, &foe1, &foe2);
	
	if (gNewBS->ai.switchingCooldown[gActiveBattler]) //Just switched in
		return FALSE;

	if (IS_DOUBLE_BATTLE)
	{
		if ((!BATTLER_ALIVE(foe1) || OnlyBadMovesLeftInMoveset(gActiveBattler, foe1))
		&&  (!BATTLER_ALIVE(foe2) || OnlyBadMovesLeftInMoveset(gActiveBattler, foe2)))
		{
			gBattleStruct->switchoutIndex[SIDE(gActiveBattler)] = PARTY_SIZE;
			EmitTwoReturnValues(1, ACTION_SWITCH, 0);
			return TRUE;
		}
	}
	else
	{
		if (OnlyBadMovesLeftInMoveset(gActiveBattler, foe1))
		{
			u8 firstId, lastId, bestMon;
			struct Pokemon *party;
			party = LoadPartyRange(gActiveBattler, &firstId, &lastId);
			bestMon = GetMostSuitableMonToSwitchInto();

			if (PredictedMoveWontDoTooMuchToMon(gActiveBattler, &party[bestMon], foe1))
			{
				gBattleStruct->switchoutIndex[SIDE(gActiveBattler)] = PARTY_SIZE;
				EmitTwoReturnValues(1, ACTION_SWITCH, 0);
				return TRUE;
			}
		}
	}

	return FALSE;
}

static bool8 FindMonThatAbsorbsOpponentsMove(void)
{
	u8 battlerIn1, battlerIn2;
	u8 foe1, foe2;
	u16 predictedMove1, predictedMove2;
	u8 absorbingTypeAbility1, absorbingTypeAbility2, absorbingTypeAbility3;
	u8 firstId, lastId;
	struct Pokemon *party;
	int i;

	LoadBattlersAndFoes(&battlerIn1, &battlerIn2, &foe1, &foe2);

	predictedMove1 = IsValidMovePrediction(foe1, gActiveBattler);
	predictedMove2 = IsValidMovePrediction(foe2, gActiveBattler);

	if (IS_SINGLE_BATTLE)
	{
		if (!MoveWouldHitFirst(predictedMove1, foe2, gActiveBattler)) //AI goes first
		{
			if (CanKnockOut(gActiveBattler, foe1))
				return FALSE; //Just KO the opponent and don't worry about switching out
		}
		else
		{
			if (!CanKnockOut(foe1, gActiveBattler) //The enemy can't KO you first
			&&   CanKnockOut(gActiveBattler, foe1)
			&&   AnyStatGreaterThan(gActiveBattler, 6 + 0)) //Has stat boosts +7 or more
				return FALSE; //Just KO the opponent and don't worry about switching out if you're boosted up
		}

		if (!CanKnockOut(foe1, gActiveBattler) //The enemy can't KO you
		&& AnyStatGreaterThan(gActiveBattler, 6 + 1)) //AI is invested in stat boosts +8 or more
			return FALSE;

		if (IS_BEHIND_SUBSTITUTE(gActiveBattler) //Make use of your substitute before switching
		&& !DamagingMoveThaCanBreakThroughSubstituteInMoveset(foe1, gActiveBattler))
			return FALSE;
	}
	else //Double Battle
	{
		u16 bestMove1 = gNewBS->ai.bestDoublesKillingMoves[gActiveBattler][foe1];
		u16 bestMove2 = gNewBS->ai.bestDoublesKillingMoves[gActiveBattler][foe2];
		if (GetDoubleKillingScore(bestMove1, gActiveBattler, foe1) >= BEST_DOUBLES_KO_SCORE - 2 //10: Hit 2 Foes, KO 1 Foe/Strongest Move 2 Foes
		||  GetDoubleKillingScore(bestMove2, gActiveBattler, foe2) >= BEST_DOUBLES_KO_SCORE - 2) //10: Hit 2 Foes, KO 1 Foe/Strongest Move 2 Foes
			return FALSE; //Don't switch if this mon can do some major damage to the enemy side

		if (AnyStatGreaterThan(gActiveBattler, 6 + 1) //AI is invested in stat boosts +8 or more
		&& (GetDoubleKillingScore(bestMove1, gActiveBattler, foe1) >= BEST_DOUBLES_KO_SCORE / 2 //6: Hit 1 Foe, KO 1 Foe
		 || GetDoubleKillingScore(bestMove2, gActiveBattler, foe2) >= BEST_DOUBLES_KO_SCORE / 2)) //6: Hit 1 Foe, KO 1 Foe
			return FALSE;
	}

	if (STAT_STAGE(gActiveBattler, STAT_STAGE_EVASION) >= 6 + 3)
		return FALSE; //Invested in Evasion so don't switch

	if (((predictedMove1 == MOVE_NONE || predictedMove1 == MOVE_PREDICTION_SWITCH) && (predictedMove2 == MOVE_NONE || predictedMove2 == MOVE_PREDICTION_SWITCH))
	|| (SPLIT(predictedMove1) == SPLIT_STATUS && SPLIT(predictedMove2) == SPLIT_STATUS))
		return FALSE;

	u8 moveType;
	if (predictedMove1 != MOVE_NONE && predictedMove1 != MOVE_PREDICTION_SWITCH)
		moveType = GetMoveTypeSpecial(foe1, predictedMove1);
	else
		moveType = GetMoveTypeSpecial(foe2, predictedMove2);

	switch (moveType) {
		case TYPE_FIRE:
			absorbingTypeAbility1 = ABILITY_FLASHFIRE;
			absorbingTypeAbility2 = ABILITY_FLASHFIRE;
			absorbingTypeAbility3 = ABILITY_FLASHFIRE;
			break;
		case TYPE_ELECTRIC:
			absorbingTypeAbility1 = ABILITY_VOLTABSORB;
			absorbingTypeAbility2 = ABILITY_LIGHTNINGROD;
			absorbingTypeAbility3 = ABILITY_MOTORDRIVE;
			break;
		case TYPE_WATER:
			absorbingTypeAbility1 = ABILITY_WATERABSORB;
			absorbingTypeAbility2 = ABILITY_DRYSKIN;
			absorbingTypeAbility3 = ABILITY_STORMDRAIN;
			break;
		case TYPE_GRASS:
			absorbingTypeAbility1 = ABILITY_SAPSIPPER;
			absorbingTypeAbility2 = ABILITY_SAPSIPPER;
			absorbingTypeAbility3 = ABILITY_SAPSIPPER;
			break;
		default:
			return FALSE;
	}

	u8 atkAbility = GetPredictedAIAbility(gActiveBattler, foe1);
	if (atkAbility == absorbingTypeAbility1
	||  atkAbility == absorbingTypeAbility2
	||  atkAbility == absorbingTypeAbility3)
		return FALSE;

	party = LoadPartyRange(gActiveBattler, &firstId, &lastId);

	for (i = firstId; i < lastId; i++)
	{
		u16 species = party[i].species;
		u8 monAbility = GetMonAbility(&party[i]);

		if (party[i].hp == 0
		||  species == SPECIES_NONE
		||	GetMonData(&party[i], MON_DATA_IS_EGG, 0)
		||	i == gBattlerPartyIndexes[battlerIn1]
		||	i == gBattlerPartyIndexes[battlerIn2]
		||	i == gBattleStruct->monToSwitchIntoId[battlerIn1]
		||	i == gBattleStruct->monToSwitchIntoId[battlerIn2])
			continue;

		if (monAbility == absorbingTypeAbility1
		||  monAbility == absorbingTypeAbility2
		||  monAbility == absorbingTypeAbility3)
		{
			// we found a mon.
			gBattleStruct->switchoutIndex[SIDE(gActiveBattler)] = i;
			EmitTwoReturnValues(1, ACTION_SWITCH, 0);
			return TRUE;
		}
	}

	return FALSE;
}

static bool8 ShouldSwitchIfNaturalCureOrRegenerator(void)
{
	u8 battlerIn1, battlerIn2;
	u8 foe1, foe2;
	u16 aiMovePrediction;
	u16 foe1MovePrediction, foe2MovePrediction;

	LoadBattlersAndFoes(&battlerIn1, &battlerIn2, &foe1, &foe2);

	switch (ABILITY(gActiveBattler)) {
		case ABILITY_NATURALCURE:
			if (WillTakeSignificantDamageFromEntryHazards(gActiveBattler, 4))
				return FALSE; //Don't switch out if you'll take a lot of damage of switch in

			if (gBattleMons[gActiveBattler].status1 & (STATUS1_SLEEP | STATUS1_FREEZE))
				break;
			if (gBattleMons[gActiveBattler].status1 //Has regular status and over half health
			&& gBattleMons[gActiveBattler].hp >= gBattleMons[gActiveBattler].maxHP / 2
			&& !IsDynamaxed(gActiveBattler))
				break;
			return FALSE;

		//Try switch if less than half health, enemy can kill, and mon can't kill enemy first
		case ABILITY_REGENERATOR:
			if (gBattleMons[gActiveBattler].hp > gBattleMons[gActiveBattler].maxHP / 2
			|| IsDynamaxed(gActiveBattler))
				return FALSE;

			if (WillTakeSignificantDamageFromEntryHazards(gActiveBattler, 3))
				return FALSE; //Don't switch out if you'll lose more than you gain by switching out here

			foe1MovePrediction = IsValidMovePrediction(foe1, gActiveBattler);
			foe2MovePrediction = IsValidMovePrediction(foe2, gActiveBattler);

			if ((BATTLER_ALIVE(foe1) && foe1MovePrediction != MOVE_NONE && MoveKnocksOutXHits(foe1MovePrediction, foe1, gActiveBattler, 1)) //Foe can kill AI
			|| (IsDoubleBattle() && BATTLER_ALIVE(foe2) && foe2MovePrediction != MOVE_NONE && MoveKnocksOutXHits(foe2MovePrediction, foe2, gActiveBattler, 1)))
			{
				if (BATTLER_ALIVE(foe1))
				{
					aiMovePrediction = IsValidMovePrediction(gActiveBattler, foe1);
					if (aiMovePrediction != MOVE_NONE && MoveWouldHitFirst(aiMovePrediction, gActiveBattler, foe1) && MoveKnocksOutXHits(aiMovePrediction, gActiveBattler, foe1, 1))
						return FALSE; //Don't switch if can knock out enemy first or enemy can't kill
					else
						break;
				}

				if (IsDoubleBattle() && BATTLER_ALIVE(foe2))
				{
					aiMovePrediction = IsValidMovePrediction(gActiveBattler, foe2);
					if (aiMovePrediction != MOVE_NONE && MoveWouldHitFirst(aiMovePrediction, gActiveBattler, foe2) && MoveKnocksOutXHits(aiMovePrediction, gActiveBattler, foe2, 1))
						return FALSE; //Don't switch if can knock out enemy first or enemy can't kill
					else
						break;
				}
			}

			return FALSE;

		default:
			return FALSE;
	}

	gBattleStruct->switchoutIndex[SIDE(gActiveBattler)] = PARTY_SIZE;
	EmitTwoReturnValues(1, ACTION_SWITCH, 0);
	return TRUE;
}

bool8 HasSuperEffectiveMoveAgainstOpponents(bool8 noRng)
{
	u8 opposingBattler;
	int i;
	u8 moveFlags;
	u16 move;

	opposingBattler = FOE(gActiveBattler);
	if (!(gAbsentBattlerFlags & gBitTable[opposingBattler]))
	{
		for (i = 0; i < MAX_MON_MOVES; ++i)
		{
			move = gBattleMons[gActiveBattler].moves[i];
			if (move == MOVE_NONE || SPLIT(move) == SPLIT_STATUS)
				continue;

			moveFlags = AI_SpecialTypeCalc(move, gActiveBattler, opposingBattler);

			if (moveFlags & MOVE_RESULT_SUPER_EFFECTIVE)
			{
				if (noRng)
					return TRUE;
				if (umodsi(Random(), 10) != 0)
					return TRUE;
			}
		}
	}

	if (!(IS_DOUBLE_BATTLE))
		return FALSE;

	opposingBattler = PARTNER(FOE(gActiveBattler));
	if (!(gAbsentBattlerFlags & gBitTable[opposingBattler]))
	{
		for (i = 0; i < MAX_MON_MOVES; ++i)
		{
			move = gBattleMons[gActiveBattler].moves[i];
			if (move == MOVE_NONE || SPLIT(move) == SPLIT_STATUS)
				continue;

			moveFlags = AI_SpecialTypeCalc(move, gActiveBattler, opposingBattler);

			if (moveFlags & MOVE_RESULT_SUPER_EFFECTIVE)
			{
				if (noRng)
					return TRUE;
				if (umodsi(Random(), 10) != 0)
					return TRUE;
			}
		}
	}

	return FALSE;
}

static bool8 PassOnWish(void)
{
	pokemon_t* party;
	u8 firstId, lastId;
	u8 bank = gActiveBattler;
	u8 battlerIn1, battlerIn2;
	u8 opposingBattler1, opposingBattler2;
	int i;

	if (IS_DOUBLE_BATTLE)
	{
		opposingBattler1 = FOE(bank);
		opposingBattler2 = PARTNER(opposingBattler1);
		battlerIn1 = gActiveBattler;
		if (gAbsentBattlerFlags & gBitTable[PARTNER(gActiveBattler)])
			battlerIn2 = gActiveBattler;
		else
			battlerIn2 = PARTNER(gActiveBattler);
	}
	else
	{
		opposingBattler1 = FOE(bank);
		opposingBattler2 = opposingBattler1;
		battlerIn1 = gActiveBattler;
		battlerIn2 = gActiveBattler;
	}

	if (gWishFutureKnock.wishCounter[bank])
	{
		//Don't switch if your health is less than half and you can survive an opponent's hit
		if (gBattleMons[bank].hp < gBattleMons[bank].maxHP / 2
		&& ((!CanKnockOut(opposingBattler1, bank) && !(IS_DOUBLE_BATTLE && CanKnockOut(opposingBattler2, bank))) || HasProtectionMoveInMoveset(bank, CHECK_MAT_BLOCK)))
			return FALSE;

		party = LoadPartyRange(gActiveBattler, &firstId, &lastId);

		for (i = firstId; i < lastId; ++i)
		{
			if (party[i].hp == 0
			||  party[i].species == SPECIES_NONE
			||	GetMonData(&party[i], MON_DATA_IS_EGG, 0)
			||	i == gBattlerPartyIndexes[battlerIn1]
			||	i == gBattlerPartyIndexes[battlerIn2]
			||	i == gBattleStruct->monToSwitchIntoId[battlerIn1]
			||	i == gBattleStruct->monToSwitchIntoId[battlerIn2])
				continue;

			if (party[i].hp < party[i].maxHP / 2)
			{
				gBattleStruct->switchoutIndex[SIDE(bank)] = i;
				EmitTwoReturnValues(1, ACTION_SWITCH, 0);
				return TRUE;
			}
		}
	}
	return FALSE;
}

static bool8 SemiInvulnerableTroll(void)
{
	u8 opposingBattler1, opposingBattler2;

	if (IS_DOUBLE_BATTLE)
	{
		opposingBattler1 = FOE(gActiveBattler);
		opposingBattler2 = PARTNER(opposingBattler1);
	}
	else
	{
		opposingBattler1 = FOE(gActiveBattler);
		opposingBattler2 = opposingBattler1;
	}

	if (IS_DOUBLE_BATTLE)
	{
		if (!(gStatuses3[opposingBattler1] & STATUS3_SEMI_INVULNERABLE) && !(gStatuses3[opposingBattler2] & STATUS3_SEMI_INVULNERABLE))
			return FALSE;
		#ifndef REALLY_SMART_AI
			if (ViableMonCountFromBank(gActiveBattler) > 1)
				return FALSE; //AI doesn't know which mon is being targeted if there's more than one on the field
		#endif
	}
	else if (!(gStatuses3[opposingBattler1] & STATUS3_SEMI_INVULNERABLE))
		return FALSE;

	if (IsDynamaxed(gActiveBattler) && gNewBS->dynamaxData.timer[gActiveBattler] > 1) //Going to be Dynamaxed after this turn
		return FALSE;

	if (RunAllSemiInvulnerableLockedMoveCalcs(opposingBattler1, opposingBattler2, FALSE))
		return TRUE;

	return FALSE;
}

static u8 GetBestPartyNumberForSemiInvulnerableLockedMoveCalcs(u8 opposingBattler1, u8 opposingBattler2, bool8 checkLockedMoves)
{
	u8 option1 = TheCalcForSemiInvulnerableTroll(opposingBattler1, MOVE_RESULT_DOESNT_AFFECT_FOE, checkLockedMoves);
	if (option1 != PARTY_SIZE)
	{
		return option1;
	}

	if (IS_DOUBLE_BATTLE)
	{
		u8 option2 = TheCalcForSemiInvulnerableTroll(opposingBattler2, MOVE_RESULT_DOESNT_AFFECT_FOE, checkLockedMoves);
		if (option2 != PARTY_SIZE)
		{
			return option2;
		}
	}

	u8 option3 = TheCalcForSemiInvulnerableTroll(opposingBattler1, MOVE_RESULT_NOT_VERY_EFFECTIVE, checkLockedMoves);
	if (option3 != PARTY_SIZE)
	{
		return option3;
	}

	if (IS_DOUBLE_BATTLE)
	{
		u8 option4 = TheCalcForSemiInvulnerableTroll(opposingBattler2, MOVE_RESULT_NOT_VERY_EFFECTIVE, checkLockedMoves);
		if (option4 != PARTY_SIZE)
		{
			return option4;
		}
	}

	return PARTY_SIZE;
}

static bool8 RunAllSemiInvulnerableLockedMoveCalcs(u8 opposingBattler1, u8 opposingBattler2, bool8 checkLockedMoves)
{
	u8 option = GetBestPartyNumberForSemiInvulnerableLockedMoveCalcs(opposingBattler1, opposingBattler2, checkLockedMoves);
	if (option != PARTY_SIZE)
	{
		gBattleStruct->switchoutIndex[SIDE(gActiveBattler)] = option;
		EmitTwoReturnValues(1, ACTION_SWITCH, 0);
	}

	return FALSE;
}

static u8 TheCalcForSemiInvulnerableTroll(u8 bankAtk, u8 flags, bool8 checkLockedMoves)
{
	u8 firstId, lastId;
	u8 battlerIn1, battlerIn2;
	int i;

	if (IS_DOUBLE_BATTLE)
	{
		battlerIn1 = gActiveBattler;
		if (gAbsentBattlerFlags & gBitTable[PARTNER(gActiveBattler)])
			battlerIn2 = gActiveBattler;
		else
			battlerIn2 = PARTNER(gActiveBattler);
	}
	else
	{
		battlerIn1 = gActiveBattler;
		battlerIn2 = gActiveBattler;
	}

	pokemon_t* party = LoadPartyRange(gActiveBattler, &firstId, &lastId);

	if (((gStatuses3[bankAtk] & STATUS3_SEMI_INVULNERABLE) || (checkLockedMoves && gLockedMoves[bankAtk] != MOVE_NONE))
	&&	gBattleStruct->moveTarget[bankAtk] == gActiveBattler)
	{
		u8 newFlags = AI_SpecialTypeCalc(gLockedMoves[bankAtk], bankAtk, gActiveBattler);
		if (BATTLER_ALIVE(gActiveBattler)) //Not end turn switch
		{
			if (newFlags & flags) //Target already has these type flags so why switch?
				return PARTY_SIZE;
		}

		for (i = firstId; i < lastId; ++i)
		{
			if (party[i].hp == 0
			||  party[i].species == SPECIES_NONE
			||	GetMonData(&party[i], MON_DATA_IS_EGG, 0)
			||	i == gBattlerPartyIndexes[battlerIn1]
			||	i == gBattlerPartyIndexes[battlerIn2]
			||	i == gBattleStruct->monToSwitchIntoId[battlerIn1]
			||	i == gBattleStruct->monToSwitchIntoId[battlerIn2])
				continue;

			if (AI_TypeCalc(gLockedMoves[bankAtk], bankAtk, &party[i]) & flags)
			{
				return i;
			}
		}
	}

	return PARTY_SIZE;
}

static bool8 CanStopLockedMove(void)
{
	u8 opposingBattler1, opposingBattler2;

	if (IS_DOUBLE_BATTLE)
	{
		opposingBattler1 = FOE(gActiveBattler);
		opposingBattler2 = PARTNER(opposingBattler1);
	}
	else
	{
		opposingBattler1 = FOE(gActiveBattler);
		opposingBattler2 = opposingBattler1;
	}

	if (IS_DOUBLE_BATTLE)
	{
		if (!(gLockedMoves[opposingBattler1] && SPLIT(gLockedMoves[opposingBattler1]) != SPLIT_STATUS) && !(gLockedMoves[opposingBattler2] && SPLIT(gLockedMoves[opposingBattler2]) != SPLIT_STATUS))
			return FALSE;
		#ifndef REALLY_SMART_AI
			if (ViableMonCountFromBank(gActiveBattler) > 1)
				return FALSE; //AI doesn't know which mon is being targeted if there's more than one on the field
		#endif
	}
	else if (!(gLockedMoves[opposingBattler1] && SPLIT(gLockedMoves[opposingBattler1]) != SPLIT_STATUS))
		return FALSE;

	if (IsDynamaxed(gActiveBattler) && gNewBS->dynamaxData.timer[gActiveBattler] > 1) //Going to be Dynamaxed after this turn
		return FALSE;

	if (RunAllSemiInvulnerableLockedMoveCalcs(opposingBattler1, opposingBattler2, TRUE))
		return TRUE;

	return FALSE;
}

static bool8 IsYawned(void)
{
	if (ABILITY(gActiveBattler) != ABILITY_NATURALCURE
	&& gStatuses3[gActiveBattler] & STATUS3_YAWN
	&& CanBePutToSleep(gActiveBattler, FALSE) //Could have been yawned and then afflicted with another status condition
	&& gBattleMons[gActiveBattler].hp > gBattleMons[gActiveBattler].maxHP / 4)
	{
		u8 battlerIn1, battlerIn2;
		u8 foe1, foe2;
		LoadBattlersAndFoes(&battlerIn1, &battlerIn2, &foe1, &foe2);

		//Don't switch if one foe left and you can KO it
		if (ViableMonCountFromBank(foe1) <= 1)
		{
			if (BATTLER_ALIVE(foe1))
			{
				if (CanKnockOut(gActiveBattler, foe1))
					return FALSE;
			}

			if (BATTLER_ALIVE(foe2))
			{
				if (CanKnockOut(gActiveBattler, foe2))
					return FALSE;
			}
		}

		//Don't switch if there's an enemy taking trap damage from this mon
		u8 activeSide = SIDE(gActiveBattler);
		for (int i = 0; i < gBattlersCount; ++i)
		{
			if (SIDE(i) != activeSide)
			{
				if (gBattleMons[i].status2 & STATUS2_WRAPPED
				&& ABILITY(i) != ABILITY_MAGICGUARD //Taking trap damage
				&& gBattleStruct->wrappedBy[i] == gActiveBattler)
					return FALSE;
			}
		}

		//Don't switch if invested in Evasion and it'll hold up
		if (STAT_STAGE(gActiveBattler, STAT_STAGE_EVASION) >= 6 + 3
		&& !ABILITY_ON_OPPOSING_FIELD(gActiveBattler, ABILITY_UNAWARE)
		&& !ABILITY_ON_OPPOSING_FIELD(gActiveBattler, ABILITY_KEENEYE))
			return FALSE;

		//Don't switch if you can fight through the sleep
		u8 ability = ABILITY(gActiveBattler);
		u8 itemEffect = ITEM_EFFECT(gActiveBattler);
		if (itemEffect == ITEM_EFFECT_CURE_SLP
		|| itemEffect == ITEM_EFFECT_CURE_STATUS
		|| ability == ABILITY_EARLYBIRD
		|| ability == ABILITY_SHEDSKIN
		|| MoveEffectInMoveset(EFFECT_SNORE, gActiveBattler)
		|| MoveEffectInMoveset(EFFECT_SLEEP_TALK, gActiveBattler)
		|| (CheckGrounding(gActiveBattler)
		 && (MoveInMoveset(MOVE_ELECTRICTERRAIN, gActiveBattler) || MoveInMoveset(MOVE_MISTYTERRAIN, gActiveBattler))))
			return FALSE;

		//Check if can do major damage instead of switching
		if (IS_SINGLE_BATTLE)
		{
			u16 foePrediction = IsValidMovePrediction(foe1, gActiveBattler);

			if (!IsPredictedToSwitch(foe1, gActiveBattler) //Enemy isn't going to switch
			&&  gBattleMoves[foePrediction].effect != EFFECT_PROTECT //Enemy isn't going to protect
			&& (gBattleMoves[foePrediction].effect != EFFECT_SEMI_INVULNERABLE || !MoveWouldHitFirst(foePrediction, foe1, gActiveBattler))) //Enemy isn't going to dodge attack first with Fly etc.
			{
				if (!MoveWouldHitFirst(foePrediction, foe2, gActiveBattler)) //AI goes first
				{
					if (CanKnockOut(gActiveBattler, foe1))
						return FALSE; //You're going to fall asleep, but at least take down the enemy with you
				}
				else
				{
					if (!CanKnockOut(foe1, gActiveBattler) //The enemy can't KO you first
					&&  CanKnockOut(gActiveBattler, foe1))
						return FALSE; //You're going to fall asleep, but at least take down the enemy with you
				}
			}
		}
		else //Double Battle
		{
			u16 bestMove1 = gNewBS->ai.bestDoublesKillingMoves[gActiveBattler][foe1];
			u16 bestMove2 = gNewBS->ai.bestDoublesKillingMoves[gActiveBattler][foe2];
			if (GetDoubleKillingScore(bestMove1, gActiveBattler, foe1) >= BEST_DOUBLES_KO_SCORE
			||  GetDoubleKillingScore(bestMove2, gActiveBattler, foe2) >= BEST_DOUBLES_KO_SCORE)
				return FALSE; //Don't switch if this mon can do some major damage to the enemy side
		}

		gBattleStruct->switchoutIndex[SIDE(gActiveBattler)] = PARTY_SIZE;
		EmitTwoReturnValues(1, ACTION_SWITCH, 0);
		return TRUE;
	}
	return FALSE;
}

static bool8 IsTakingAnnoyingSecondaryDamage(void)
{
	if (GetPredictedAIAbility(gActiveBattler, FOE(gActiveBattler)) != ABILITY_MAGICGUARD
	&& !CanKillAFoe(gActiveBattler)
	&& !IsDynamaxed(gActiveBattler)
	&& AI_THINKING_STRUCT->aiFlags > AI_SCRIPT_CHECK_BAD_MOVE) //Has smart AI
	{
		if ((gStatuses3[gActiveBattler] & STATUS3_LEECHSEED && (Random() & 3) == 0) //25% chance to switch out when seeded
		|| ((gBattleMons[gActiveBattler].status1 & STATUS1_SLEEP) > 1 && gBattleMons[gActiveBattler].status2 & STATUS2_NIGHTMARE)
		||  gBattleMons[gActiveBattler].status2 & (STATUS2_CURSED)
		||  (gBattleMons[gActiveBattler].status1 & STATUS1_TOXIC_COUNTER) > 0x600) //Been sitting with toxic for 6 turns
		{
			if (!WillTakeSignificantDamageFromEntryHazards(gActiveBattler, 4)) //Don't switch out if you'll take a lot of damage of switch in
			{
				gBattleStruct->switchoutIndex[SIDE(gActiveBattler)] = PARTY_SIZE;
				EmitTwoReturnValues(1, ACTION_SWITCH, 0);
				return TRUE;
			}
		}
	}

	return FALSE;
}

static bool8 ShouldSwitchToAvoidDeath(void)
{
	if (gNewBS->ai.switchingCooldown[gActiveBattler]) //Just switched in
		return FALSE;

	if (IS_SINGLE_BATTLE
	&& AI_THINKING_STRUCT->aiFlags > AI_SCRIPT_CHECK_BAD_MOVE) //Has smart AI
	{
		u16 atkMove = IsValidMovePrediction(gActiveBattler, FOE(gActiveBattler));
		u16 defMove = IsValidMovePrediction(FOE(gActiveBattler), gActiveBattler);

		if (defMove != MOVE_NONE //Foe going to attack
		&& (atkMove == MOVE_NONE || !MoveWouldHitFirst(atkMove, gActiveBattler, FOE(gActiveBattler))) //Attacker wouldn't go first
		&& (!IS_BEHIND_SUBSTITUTE(gActiveBattler) || !MoveBlockedBySubstitute(defMove, FOE(gActiveBattler), gActiveBattler))
		&&  MoveKnocksOutXHits(defMove, FOE(gActiveBattler), gActiveBattler, 1) //Foe will kill
		&& !WillTakeSignificantDamageFromEntryHazards(gActiveBattler, 2)) //50% health loss
		{
			u8 firstId, lastId;
			struct Pokemon* party = LoadPartyRange(gActiveBattler, &firstId, &lastId);
			u8 bestMon = GetMostSuitableMonToSwitchInto();
			u32 resultFlags = AI_TypeCalc(defMove, FOE(gActiveBattler), &party[bestMon]);

			if ((resultFlags & MOVE_RESULT_NO_EFFECT && GetMostSuitableMonToSwitchIntoScore() >= SWITCHING_INCREASE_HAS_SUPER_EFFECTIVE_MOVE) //Has some sort of followup
			||  (PredictedMoveWontDoTooMuchToMon(gActiveBattler, &party[bestMon], FOE(gActiveBattler)) && GetMostSuitableMonToSwitchIntoScore() >= SWITCHING_INCREASE_WALLS_FOE))
			{
				gBattleStruct->switchoutIndex[SIDE(gActiveBattler)] = PARTY_SIZE;
				EmitTwoReturnValues(1, ACTION_SWITCH, 0);
				return TRUE;
			}
		}
	}

	return FALSE;
}

//Most likely this function won't get used anymore. GetMostSuitableMonToSwitchInto
//now handles all of the switching checks.
bool8 FindMonWithFlagsAndSuperEffective(u8 flags, u8 moduloPercent)
{
	u8 battlerIn1, battlerIn2;
	u8 foe1, foe2;
	int i, j;
	u8 start, end;
	u16 move;
	u8 moveFlags;

	if (IS_DOUBLE_BATTLE)
	{
		battlerIn1 = gActiveBattler;
		if (gAbsentBattlerFlags & gBitTable[PARTNER(gActiveBattler)])
			battlerIn2 = gActiveBattler;
		else
			battlerIn2 = PARTNER(gActiveBattler);

		if (gAbsentBattlerFlags & gBitTable[FOE(gActiveBattler)])
			foe1 = foe2 = PARTNER(FOE(gActiveBattler));
		else if (gAbsentBattlerFlags & gBitTable[PARTNER(FOE(gActiveBattler))])
			foe1 = foe2 = FOE(gActiveBattler);
		else
		{
			foe1 = FOE(gActiveBattler);
			foe2 = PARTNER(FOE(gActiveBattler));
		}
	}
	else
	{
		battlerIn1 = gActiveBattler;
		battlerIn2 = gActiveBattler;
		foe1 = FOE(gActiveBattler);
		foe2 = foe1;
	}

	u16 predictedMove1 = IsValidMovePrediction(foe1, gActiveBattler);
	u16 predictedMove2 = IsValidMovePrediction(foe2, gActiveBattler);

	if (((predictedMove1 == MOVE_NONE || predictedMove1 == MOVE_PREDICTION_SWITCH) && (predictedMove2 == MOVE_NONE || predictedMove2 == MOVE_PREDICTION_SWITCH))
	|| (SPLIT(predictedMove1) == SPLIT_STATUS && SPLIT(predictedMove2) == SPLIT_STATUS))
		return FALSE;

	pokemon_t* party = LoadPartyRange(gActiveBattler, &start, &end);

//Party Search
	for (i = start; i < end; ++i)
	{
		if (party[i].hp == 0
		|| party[i].species == SPECIES_NONE
		|| GetMonData(&party[i], MON_DATA_IS_EGG, NULL)
		|| i == gBattlerPartyIndexes[battlerIn1]
		|| i == gBattlerPartyIndexes[battlerIn2]
		|| i == gBattleStruct->monToSwitchIntoId[battlerIn1]
		|| i == gBattleStruct->monToSwitchIntoId[battlerIn2])
			continue;

		if (predictedMove1 != MOVE_NONE && predictedMove1 != MOVE_PREDICTION_SWITCH)
			moveFlags = AI_TypeCalc(predictedMove1, foe1, &party[i]);
		else
			moveFlags = AI_TypeCalc(predictedMove2, foe2, &party[i]);

		if (moveFlags & flags)
		{
			if (predictedMove1 != MOVE_NONE && predictedMove1 != MOVE_PREDICTION_SWITCH)
				battlerIn1 = foe1;
			else
				battlerIn1 = foe2;

			for (j = 0; j < 4; j++) {
				move = party[i].moves[j];
				if (move == 0 || SPLIT(move) == SPLIT_STATUS)
					continue;

				moveFlags = TypeCalc(move, gActiveBattler, battlerIn1, &party[i], TRUE);

				if (moveFlags & MOVE_RESULT_SUPER_EFFECTIVE && umodsi(Random(), moduloPercent) == 0) {
					gBattleStruct->switchoutIndex[SIDE(gActiveBattler)] = i; //There was a GetBattlerPosition here but its a pretty useless function
					EmitTwoReturnValues(1, ACTION_SWITCH, 0);
					return TRUE;
				}
			}
		}
	}

	return FALSE;
}

static bool8 ShouldSwitchIfWonderGuard(void)
{
	u8 battlerIn1;
	u8 opposingBattler;
	u8 moveFlags;
	int i, j;
	u8 start, end;
	opposingBattler = FOE(gActiveBattler);

	if (IS_DOUBLE_BATTLE)
		return FALSE;

	if (ABILITY(opposingBattler) != ABILITY_WONDERGUARD)
		return FALSE;

	if (WillFaintFromSecondaryDamage(opposingBattler))
		return FALSE;

	//Check if pokemon has a super effective move, Mold Breaker Move, or move that can hurt Shedinja
	u8 bankAtk = gActiveBattler;
	u8 bankDef = opposingBattler;
	u8 moveLimitations = CheckMoveLimitations(bankAtk, 0, AdjustMoveLimitationFlagsForAI(bankAtk, bankDef));
	for (i = 0; i < MAX_MON_MOVES; ++i)
	{
		u16 move = GetBattleMonMove(gActiveBattler, i);
		if (move == MOVE_NONE)
			break;

		if (!(gBitTable[i] & moveLimitations))
		{
			if (CheckTableForMove(move, gMoldBreakerMoves))
				return FALSE;

			if (SPLIT(move) != SPLIT_STATUS)
			{
				u8 atkAbility = GetAIAbility(bankAtk, bankDef, move);
				if (atkAbility == ABILITY_MOLDBREAKER
				||  atkAbility == ABILITY_TERAVOLT
				||  atkAbility == ABILITY_TURBOBLAZE)
					return FALSE;

				moveFlags = AI_SpecialTypeCalc(move, bankAtk, bankDef);
				if (moveFlags & MOVE_RESULT_SUPER_EFFECTIVE)
					return FALSE;
			}
			else if (!MoveBlockedBySubstitute(move, bankAtk, bankDef))
			{
				switch (gBattleMoves[move].effect) {
					case EFFECT_SLEEP:
					case EFFECT_YAWN:
						if (CanBePutToSleep(bankDef, TRUE))
							return FALSE;
						break;
					case EFFECT_ROAR:
						if (BankHasMonToSwitchTo(bankDef))
							return FALSE;
						break;
					case EFFECT_TOXIC:
					case EFFECT_POISON:
					PSN_CHECK:
						if (CanBePoisoned(bankDef, bankAtk, TRUE))
							return FALSE;
						break;
					case EFFECT_WILL_O_WISP:
					BRN_CHECK:
						if (CanBeBurned(bankDef, TRUE))
							return FALSE;
						break;
					case EFFECT_CONFUSE:
					case EFFECT_SWAGGER:
					case EFFECT_FLATTER:
						if (CanBeConfused(bankDef, TRUE))
							return FALSE;
						break;
					case EFFECT_PARALYZE:
						if (CanBeParalyzed(bankDef, TRUE))
							return FALSE;
						break;
					case EFFECT_LEECH_SEED:
						if (!IsOfType(bankDef, TYPE_GRASS))
							return FALSE;
						break;
					case EFFECT_NIGHTMARE:
						if (gBattleMons[bankDef].status1 & STATUS_SLEEP)
							return FALSE;
						break;
					case EFFECT_CURSE:
						if (IsOfType(bankAtk, TYPE_GHOST))
							return FALSE;
						break;
					case EFFECT_SPIKES:
						switch (move) {
							case MOVE_STEALTHROCK:
								if (gSideTimers[SIDE(bankDef)].srAmount == 0)
									return FALSE;
								break;

							case MOVE_TOXICSPIKES:
								if (gSideTimers[SIDE(bankDef)].tspikesAmount < 2)
									return FALSE;
								break;

							case MOVE_STICKYWEB:
								if (gSideTimers[SIDE(bankDef)].stickyWeb == 0)
									return FALSE;
								break;

							default: //Spikes
								if (gSideTimers[SIDE(bankDef)].spikesAmount < 3)
									return FALSE;
								break;
						}
						break;
					case EFFECT_PERISH_SONG:
						if (!(gStatuses3[bankDef] & STATUS3_PERISH_SONG))
							return FALSE;
						break;
					case EFFECT_SANDSTORM:
						if (SandstormHurts(bankDef))
							return FALSE;
						break;
					case EFFECT_HAIL:
						if (HailHurts(bankDef))
							return FALSE;
						break;
					case EFFECT_BATON_PASS:
					case EFFECT_TELEPORT:
						return FALSE;
					case EFFECT_MEMENTO:
						if (SPLIT(move) == SPLIT_STATUS)
							return FALSE;
						break;
					case EFFECT_TRICK:
						if (move == MOVE_TRICK)
						{
							u8 atkSpecies = SPECIES(bankAtk);
							u8 defSpecies = SPECIES(bankDef);

							if (CanTransferItem(atkSpecies, ITEM(bankAtk))
							&& CanTransferItem(atkSpecies, ITEM(bankDef))
							&& CanTransferItem(defSpecies, ITEM(bankAtk))
							&& CanTransferItem(defSpecies, ITEM(bankDef)))
							{
								switch (ITEM_EFFECT(bankAtk)) {
									case ITEM_EFFECT_TOXIC_ORB:
										goto PSN_CHECK;
									case ITEM_EFFECT_FLAME_ORB:
										goto BRN_CHECK;
									case ITEM_EFFECT_BLACK_SLUDGE:
										if (!IsOfType(bankDef, TYPE_POISON))
											return FALSE;
										break;
									case ITEM_EFFECT_STICKY_BARB:
										return FALSE;
								}
							}
						}
						break;
					case EFFECT_WISH:
						if (gWishFutureKnock.wishCounter[bankAtk] == 0)
							return FALSE;
						break;
					case EFFECT_SKILL_SWAP:
						if (move != MOVE_SKILLSWAP)
							return FALSE;
						break;

					case EFFECT_TYPE_CHANGES:
						switch (move) {
							case MOVE_TRICKORTREAT:
								if (!IsOfType(bankDef, TYPE_GHOST))
									return FALSE;
								break;
							case MOVE_FORESTSCURSE:
								if (!IsOfType(bankDef, TYPE_GRASS))
									return FALSE;
								break;
						}
						break;
					case EFFECT_TEAM_EFFECTS:
						switch (move) {
							case MOVE_TAILWIND:
								if (gNewBS->TailwindTimers[SIDE(bankAtk)] == 0)
									return FALSE;
								break;
							case MOVE_LUCKYCHANT:
								if (gNewBS->LuckyChantTimers[SIDE(bankAtk)] == 0)
									return FALSE;
								break;
						}
						break;
				}
			}
		}
	}


	battlerIn1 = gActiveBattler;
	pokemon_t* party = LoadPartyRange(gActiveBattler, &start, &end);

	//Find a pokemon in the party that has a super effective move
	for (i = start; i < end; ++i)
	{
		if (party[i].hp == 0
			|| party[i].species == SPECIES_NONE
			|| GetMonData(&party[i], MON_DATA_IS_EGG, 0)
			|| i == gBattlerPartyIndexes[battlerIn1]
			|| i == gBattleStruct->monToSwitchIntoId[battlerIn1])
			continue;

		for (j = 0; j < MAX_MON_MOVES; j++) {
			u16 move = party[i].moves[j];
			if (move == MOVE_NONE || SPLIT(move) == SPLIT_STATUS)
				continue;

			moveFlags = TypeCalc(move, gActiveBattler, opposingBattler, &party[i], TRUE);
			if (moveFlags & MOVE_RESULT_SUPER_EFFECTIVE)
			{
				// we found a mon
				gBattleStruct->switchoutIndex[SIDE(gActiveBattler)] = i;
				EmitTwoReturnValues(1, ACTION_SWITCH, 0);
				return TRUE;
			}
		}
	}

	return FALSE; // at this point there is not a single pokemon in the party that has a super effective move against a pokemon with wonder guard
}

/*
10. KO Foe + Resist/Immune to All Moves + Revenge Kill
9.
8.  KO Foe + Resist/Immune to All Moves
7.
6.  KO Foe + Revenge Kill
5.  Resist/Immune to All Moves + Has Super-Effective Move, KO Foe + Revenge Kill + Weak to Move
4.  KO Foe, Resist/Immune to All Moves
3.  KO Foe + Weak to Move
2.
1.  Has Super-Effective Move
*/

//Add logic about switching in a partner to resist spread move in doubles
u8 GetMostSuitableMonToSwitchInto(void)
{
	u8 battlerIn1, battlerIn2;
	u8 foe1, foe2;
	u8 firstId, lastId;
	LoadBattlersAndFoes(&battlerIn1, &battlerIn2, &foe1, &foe2);
	struct Pokemon* party = LoadPartyRange(gActiveBattler, &firstId, & lastId);

	CalcMostSuitableMonSwitchIfNecessary();
	u8 option1 = gNewBS->ai.bestMonIdToSwitchInto[gActiveBattler][0];
	u8 option2 = gNewBS->ai.bestMonIdToSwitchInto[gActiveBattler][1];

	if (option1 == PARTY_SIZE
	||  GetMonData(&party[option1], MON_DATA_HP, NULL) == 0) //Best mon is dead
	{
		CalcMostSuitableMonToSwitchInto();
		gNewBS->ai.calculatedAISwitchings[gActiveBattler] = TRUE;

		if (!BankSideHasTwoTrainers(gActiveBattler))
			gNewBS->ai.calculatedAISwitchings[PARTNER(gActiveBattler)] = TRUE;
	}

	if (option1 == gBattleStruct->monToSwitchIntoId[battlerIn1]
	||  option1 == gBattleStruct->monToSwitchIntoId[battlerIn2])
		return option2;

	return option1;
}

s8 GetMostSuitableMonToSwitchIntoScore(void)
{
	u8 battlerIn1, battlerIn2;
	u8 foe1, foe2;
	LoadBattlersAndFoes(&battlerIn1, &battlerIn2, &foe1, &foe2);

	CalcMostSuitableMonSwitchIfNecessary();
	u8 option1 = gNewBS->ai.bestMonIdToSwitchInto[gActiveBattler][0];

	if (option1 == gBattleStruct->monToSwitchIntoId[battlerIn1]
	||  option1 == gBattleStruct->monToSwitchIntoId[battlerIn2])
		return gNewBS->ai.bestMonIdToSwitchIntoScores[gActiveBattler][1];

	return gNewBS->ai.bestMonIdToSwitchIntoScores[gActiveBattler][0];
}

static void CalcMostSuitableMonSwitchIfNecessary(void)
{
	//Calc switching only if necessary
	if (!gNewBS->ai.calculatedAISwitchings[gActiveBattler] && BATTLER_ALIVE(gActiveBattler)) //So Multi Battles still work properly
	{
		CalcMostSuitableMonToSwitchInto();
		gNewBS->ai.calculatedAISwitchings[gActiveBattler] = TRUE;

		if (!BankSideHasTwoTrainers(gActiveBattler))
			gNewBS->ai.calculatedAISwitchings[PARTNER(gActiveBattler)] = TRUE;
	}
}

u8 CalcMostSuitableMonToSwitchInto(void)
{
	//u8 opposingBattler;
	u32 i, j, k;
	u8 bestMonId, secondBestMonId;
	u8 battlerIn1, battlerIn2;
	u8 foe, foe1, foe2;
	u8 firstId, lastId; // + 1
	u16 move;
	struct Pokemon* party;

	u8 lastValidMon = PARTY_SIZE;
	u8 secondLastValidMon = PARTY_SIZE;
	s8 scores[PARTY_SIZE] = {0};
	bool8 canNegateToxicSpikes[PARTY_SIZE] = {FALSE};
	bool8 canRemoveHazards[PARTY_SIZE] = {FALSE};

	if (gBattleStruct->monToSwitchIntoId[gActiveBattler] != PARTY_SIZE)
		return gBattleStruct->monToSwitchIntoId[gActiveBattler];

	LoadBattlersAndFoes(&battlerIn1, &battlerIn2, &foe1, &foe2);
	party = LoadPartyRange(gActiveBattler, &firstId, & lastId);

	//Check if point even running calcs
	u8 count = 0;
	for (i = firstId; i < lastId; ++i)
	{
		if (party[i].species != SPECIES_NONE
		&& !GetMonData(&party[i], MON_DATA_IS_EGG, NULL)
		&& party[i].hp > 0
		&& gBattlerPartyIndexes[battlerIn1] != i
		&& gBattlerPartyIndexes[battlerIn2] != i
		&& i != gBattleStruct->monToSwitchIntoId[battlerIn1]
		&& i != gBattleStruct->monToSwitchIntoId[battlerIn2])
		{
			++count;
		}
	}

	if (count == 0)
	{
		ResetBestMonToSwitchInto(gActiveBattler);
		return PARTY_SIZE;
	}

	//Try to counter a locked move
	//u8 option = GetBestPartyNumberForSemiInvulnerableLockedMoveCalcs(opposingBattler, opposingBattler, TRUE);
	//if (option != PARTY_SIZE)
	//	return option;

	//Find the mon who is most suitable
	bestMonId = PARTY_SIZE;
	secondBestMonId = PARTY_SIZE;
	for (i = firstId; i < lastId; ++i)
	{
		if (party[i].species != SPECIES_NONE
		&& party[i].hp > 0
		&& !GetMonData(&party[i], MON_DATA_IS_EGG, NULL)
		&& i != gBattlerPartyIndexes[battlerIn1]
		&& i != gBattlerPartyIndexes[battlerIn2]
		&& i != gBattleStruct->monToSwitchIntoId[battlerIn1]
		&& i != gBattleStruct->monToSwitchIntoId[battlerIn2])
		{
			u8 foes[] = {foe1, foe2};
			u8 moveLimitations = CheckMoveLimitationsFromParty(&party[i], 0, 0xFF);
			secondLastValidMon = lastValidMon;
			lastValidMon = i;

			u16 species = GetMonData(&party[i], MON_DATA_SPECIES, NULL);
			canNegateToxicSpikes[i] = CheckGroundingFromPartyData(&party[i])
									&& (gBaseStats[species].type1 == TYPE_POISON || gBaseStats[species].type2 == TYPE_POISON);

			if (WillFaintFromEntryHazards(&party[i], SIDE(gActiveBattler)))
				continue; //Don't switch in the mon if it'll faint on reentry

			struct DamageCalc damageData = {0};
			damageData.bankAtk = gActiveBattler;
			damageData.monAtk = &party[i];
			PopulateDamageCalcStructWithBaseAttackerData(&damageData);

			for (j = 0; j < gBattlersCount / 2; ++j) //Loop through all enemies on field
			{
				foe = foes[j];

				if (BATTLER_ALIVE(foe)
				&& (j == 0 || foes[0] != foes[j])) //Don't check same opponent twice
				{
					u8 typeEffectiveness = 0;
					u8 ability = GetMonAbility(&party[i]);
					bool8 isWeakToMove = FALSE;
					bool8 isNormalEffectiveness = FALSE;

					damageData.bankDef = foe;
					PopulateDamageCalcStructWithBaseDefenderData(&damageData);

					//Check Offensive Capabilities
					if (CanKnockOutFromParty(&party[i], foe, &damageData))
					{
						scores[i] += SWITCHING_INCREASE_KO_FOE;

						if (ability == ABILITY_MOXIE
						||  ability == ABILITY_SOULHEART
						||  ability == ABILITY_BEASTBOOST)
							scores[i] += SWITCHING_INCREASE_REVENGE_KILL;
						else
						{
							for (k = 0; k < MAX_MON_MOVES; ++k)
							{
								move = GetMonData(&party[i], MON_DATA_MOVE1 + k, 0);

								if (gBattleMoves[move].effect == EFFECT_RAPID_SPIN //Includes Defog
								&&  gSideStatuses[SIDE(gActiveBattler)] & SIDE_STATUS_SPIKES)
								{
									if (IS_SINGLE_BATTLE) //Single Battle
										canRemoveHazards[i] = ViableMonCountFromBank(gActiveBattler) >= 2; //There's a point in removing the hazards
									else //Double Battle
										canRemoveHazards[i] = ViableMonCountFromBank(gActiveBattler) >= 3; //There's a point in removing the hazards
								}

								if (move == MOVE_FELLSTINGER
								&&  !(gBitTable[k] & moveLimitations))
								{
									if (MoveKnocksOutXHitsFromParty(move, &party[i], foe, 1, &damageData))
									{
										scores[i] += SWITCHING_INCREASE_REVENGE_KILL;
										break;
									}
								}
								else if (SPLIT(move) != SPLIT_STATUS
								&& PriorityCalcMon(&party[i], move) > 0
								&& MoveKnocksOutXHitsFromParty(move, &party[i], foe, 1, &damageData))
								{
									//Priority move that KOs
									scores[i] += SWITCHING_INCREASE_REVENGE_KILL;
									break;
								}
							}
						}
					}
					else //This mon can't KO the foe
					{
						for (k = 0; k < MAX_MON_MOVES; ++k)
						{
							move = GetMonData(&party[i], MON_DATA_MOVE1 + k, 0);

							if (gBattleMoves[move].effect == EFFECT_RAPID_SPIN //Includes Defog
							&&  gSideStatuses[SIDE(gActiveBattler)] & SIDE_STATUS_SPIKES)
							{
								if (!IS_DOUBLE_BATTLE) //Single Battle
									canRemoveHazards[i] = ViableMonCountFromBank(gActiveBattler) >= 2; //There's a point in removing the hazards
								else //Double Battle
									canRemoveHazards[i] = ViableMonCountFromBank(gActiveBattler) >= 3; //There's a point in removing the hazards
							}

							if (move != MOVE_NONE
							&& SPLIT(move) != SPLIT_STATUS
							&& !(gBitTable[k] & moveLimitations)
							&& TypeCalc(move, gActiveBattler, foe, &party[i], TRUE) & MOVE_RESULT_SUPER_EFFECTIVE)
							{
								scores[i] += SWITCHING_INCREASE_HAS_SUPER_EFFECTIVE_MOVE; //Only checked if can't KO
								break;
							}
						}
					}

					//Check Defensive Capabilities
					bool8 physMoveInMoveset = FALSE;
					bool8 specMoveInMoveset = FALSE;
					u8 foeMoveLimitations = CheckMoveLimitations(foe, 0, 0xFF);
					struct DamageCalc foeDamageData = {0};
					foeDamageData.bankAtk = foe;
					foeDamageData.bankDef = gActiveBattler; //For the side
					foeDamageData.monDef = &party[i];
					PopulateDamageCalcStructWithBaseAttackerData(&foeDamageData);
					PopulateDamageCalcStructWithBaseDefenderData(&foeDamageData);

					for (k = 0; k < MAX_MON_MOVES; ++k)
					{
						move = GetBattleMonMove(foe, k);

						if (move == MOVE_NONE)
							break;

						if (gBattleMons[foe].status2 & STATUS2_MULTIPLETURNS
						&&  MoveInMoveset(gLockedMoves[foe], foe)
						&&  move != gLockedMoves[foe])
							continue; //Skip non-locked moves

						u8 split = CalcMoveSplit(foe, move);
						if (split == SPLIT_PHYSICAL)
							physMoveInMoveset = TRUE;
						else if (split == SPLIT_SPECIAL)
							specMoveInMoveset = TRUE;
						else
							continue; //Skip status moves

						if (!(gBitTable[k] & foeMoveLimitations))
						{
							typeEffectiveness = AI_TypeCalc(move, foe, &party[i]);

							if (typeEffectiveness & MOVE_RESULT_SUPER_EFFECTIVE)
							{
								isWeakToMove = TRUE;
								break; //Only need 1 check for this to pass
							}
							else if (typeEffectiveness & MOVE_RESULT_NOT_VERY_EFFECTIVE)
							{
								if (!isNormalEffectiveness && IS_SINGLE_BATTLE) //Only need 1 check to pass and don't waste extra time in doubles
								{
									//This function takes time for each move for each Pokemon so we try to call it as little as possible
									u32 dmg = AI_CalcMonDefDmg(foe, gActiveBattler, move, &party[i], &foeDamageData);
									if (dmg >= party[i].hp / 2) //Move does half of over half of the health remaining
										isNormalEffectiveness = TRUE;
								}
							}
							else if (typeEffectiveness == 0)
								isNormalEffectiveness = TRUE;
							//By default it either resists or is immune
						}
					}

					if (isWeakToMove)
						scores[i] -= SWITCHING_DECREASE_WEAK_TO_MOVE;
					else if (!isNormalEffectiveness) //Foe has no Super-Effective or Normal-Effectiveness moves
						scores[i] += SWITCHING_INCREASE_RESIST_ALL_MOVES;
					else
					{
						bool8 cantWall = FALSE;
						u32 attack, spAttack;
						APPLY_STAT_MOD(attack, &gBattleMons[foe], gBattleMons[foe].attack, STAT_STAGE_ATK);
						APPLY_STAT_MOD(spAttack, &gBattleMons[foe], gBattleMons[foe].spAttack, STAT_STAGE_SPATK);

						if (physMoveInMoveset && GetMonData(&party[i], MON_DATA_DEF, NULL) <= attack)
							cantWall = TRUE;
						else if (specMoveInMoveset && GetMonData(&party[i], MON_DATA_SPDEF, NULL) <= spAttack)
							cantWall = TRUE;

						if (!cantWall)
							scores[i] += SWITCHING_INCREASE_WALLS_FOE;
					}
				}
			}

			if (!IS_DOUBLE_BATTLE || ViableMonCountFromBank(foe1) == 1) //Single Battle or 1 target left
			{
				if (scores[i] >= SWITCHING_SCORE_MAX && canRemoveHazards[i]) //This mon is perfect
				{
					if (IS_SINGLE_BATTLE)
					{
						bestMonId = i;
						break;
					}
					else //Double Battle
					{
						if (bestMonId == PARTY_SIZE)
							bestMonId = i;
						else
						{
							secondBestMonId = i;
							break;
						}
					}
				}
			}
			else //Double Battle
			{
				if (scores[i] >= SWITCHING_SCORE_MAX * 2 && canRemoveHazards[i]) //This mon is perfect
				{
					if (IS_SINGLE_BATTLE)
					{
						bestMonId = i;
						break;
					}
					else //Double Battle
					{
						if (bestMonId == PARTY_SIZE)
							bestMonId = i;
						else
						{
							secondBestMonId = i;
							break;
						}
					}
				}
			}

			if (bestMonId == PARTY_SIZE)
				bestMonId = i;
			else if (scores[i] > scores[bestMonId])
			{
				if (IS_DOUBLE_BATTLE)
					secondBestMonId = bestMonId;

				bestMonId = i;
			}
			else if (IS_DOUBLE_BATTLE && secondBestMonId == PARTY_SIZE)
				secondBestMonId = i;
		}
	}

	if (bestMonId != PARTY_SIZE)
	{
		if (scores[bestMonId] < 8)
		{
			bool8 tSpikesActive = gSideTimers[SIDE(gActiveBattler)].tspikesAmount > 0;
			for (i = firstId; i < lastId; ++i)
			{
				if (canRemoveHazards[i]
				|| (tSpikesActive && canNegateToxicSpikes[i]))
				{
					if (IS_DOUBLE_BATTLE)
					{
						gNewBS->ai.bestMonIdToSwitchIntoScores[gActiveBattler][0] = SWITCHING_INCREASE_CAN_REMOVE_HAZARDS * 2;

						if (!BankSideHasTwoTrainers(gActiveBattler))
							gNewBS->ai.bestMonIdToSwitchIntoScores[PARTNER(gActiveBattler)][0] = SWITCHING_INCREASE_CAN_REMOVE_HAZARDS * 2;
					}
					else
						gNewBS->ai.bestMonIdToSwitchIntoScores[gActiveBattler][0] = SWITCHING_INCREASE_CAN_REMOVE_HAZARDS;

					gNewBS->ai.bestMonIdToSwitchInto[gActiveBattler][0] = i;
					if (!BankSideHasTwoTrainers(gActiveBattler))
						gNewBS->ai.bestMonIdToSwitchInto[PARTNER(gActiveBattler)][0] = i;

					if (IS_SINGLE_BATTLE)
						return i; //Just send out the Pokemon that can remove the hazards
					else
					{
						secondBestMonId = bestMonId;
						bestMonId = i;
						break;
					}
				}
			}
		}

		if (IS_DOUBLE_BATTLE)
		{
			gNewBS->ai.bestMonIdToSwitchInto[gActiveBattler][0] = bestMonId;
			gNewBS->ai.bestMonIdToSwitchInto[gActiveBattler][1] = secondBestMonId;
			gNewBS->ai.bestMonIdToSwitchIntoScores[gActiveBattler][0] = scores[bestMonId];
			gNewBS->ai.bestMonIdToSwitchIntoScores[gActiveBattler][1] = scores[secondBestMonId];

			if (!BankSideHasTwoTrainers(gActiveBattler)) //Store data for second mon too
			{
				gNewBS->ai.bestMonIdToSwitchInto[PARTNER(gActiveBattler)][0] = bestMonId;
				gNewBS->ai.bestMonIdToSwitchInto[PARTNER(gActiveBattler)][1] = secondBestMonId;
				gNewBS->ai.bestMonIdToSwitchIntoScores[PARTNER(gActiveBattler)][0] = scores[bestMonId];
				gNewBS->ai.bestMonIdToSwitchIntoScores[PARTNER(gActiveBattler)][1] = scores[secondBestMonId];
			}
		}
		else
		{
			gNewBS->ai.bestMonIdToSwitchInto[gActiveBattler][0] = bestMonId;
			gNewBS->ai.bestMonIdToSwitchIntoScores[gActiveBattler][0] = scores[bestMonId];
		}

		return bestMonId;
	}

	//If for some reason we weren't able to find a mon above,
	//pick the last checked available mon now.
	gNewBS->ai.bestMonIdToSwitchIntoScores[gActiveBattler][0] = 0;
	gNewBS->ai.bestMonIdToSwitchInto[gActiveBattler][0] = lastValidMon;
	gNewBS->ai.bestMonIdToSwitchIntoScores[gActiveBattler][0] = 0;
	gNewBS->ai.bestMonIdToSwitchInto[gActiveBattler][0] = secondLastValidMon;

	if (!BankSideHasTwoTrainers(gActiveBattler)) //Store data for second mon too
	{
		gNewBS->ai.bestMonIdToSwitchIntoScores[PARTNER(gActiveBattler)][0] = 0;
		gNewBS->ai.bestMonIdToSwitchInto[PARTNER(gActiveBattler)][0] = lastValidMon;
		gNewBS->ai.bestMonIdToSwitchIntoScores[PARTNER(gActiveBattler)][0] = 0;
		gNewBS->ai.bestMonIdToSwitchInto[PARTNER(gActiveBattler)][0] = secondLastValidMon;
	}

	return lastValidMon; //If lastValidMon is still PARTY_SIZE when reaching here, there is a bigger problem
}

void ResetBestMonToSwitchInto(u8 bank)
{
	gNewBS->ai.bestMonIdToSwitchIntoScores[bank][0] = 0;
	gNewBS->ai.bestMonIdToSwitchInto[bank][0] = PARTY_SIZE;
	gNewBS->ai.bestMonIdToSwitchIntoScores[bank][1] = 0;
	gNewBS->ai.bestMonIdToSwitchInto[bank][1] = PARTY_SIZE;

	if (!BankSideHasTwoTrainers(bank)) //Store data for second mon too
	{
		gNewBS->ai.bestMonIdToSwitchIntoScores[PARTNER(bank)][0] = 0;
		gNewBS->ai.bestMonIdToSwitchInto[PARTNER(bank)][0] = PARTY_SIZE;
		gNewBS->ai.bestMonIdToSwitchIntoScores[PARTNER(bank)][1] = 0;
		gNewBS->ai.bestMonIdToSwitchInto[PARTNER(bank)][1] = PARTY_SIZE;
	}
}

void RemoveBestMonToSwitchInto(u8 bank)
{
	//secondBestMonId -> bestMonId
	gNewBS->ai.bestMonIdToSwitchIntoScores[bank][0] = gNewBS->ai.bestMonIdToSwitchIntoScores[bank][1];
	gNewBS->ai.bestMonIdToSwitchInto[bank][0] = gNewBS->ai.bestMonIdToSwitchInto[bank][1];

	gNewBS->ai.bestMonIdToSwitchIntoScores[bank][1] = 0;
	gNewBS->ai.bestMonIdToSwitchInto[bank][1] = PARTY_SIZE;

	if (!BankSideHasTwoTrainers(bank)) //Store data for second mon too
	{
		gNewBS->ai.bestMonIdToSwitchIntoScores[PARTNER(bank)][0] = gNewBS->ai.bestMonIdToSwitchIntoScores[PARTNER(bank)][1];
		gNewBS->ai.bestMonIdToSwitchInto[PARTNER(bank)][0] = gNewBS->ai.bestMonIdToSwitchInto[PARTNER(bank)][1];

		gNewBS->ai.bestMonIdToSwitchIntoScores[PARTNER(bank)][1] = 0;
		gNewBS->ai.bestMonIdToSwitchInto[PARTNER(bank)][1] = PARTY_SIZE;
	}
}

u32 WildMonIsSmart(unusedArg u8 bank)
{
	#ifdef WILD_ALWAYS_SMART
		return TRUE;
	#else
		u16 species = SPECIES(bank);
		if (IsMegaSpecies(species)
		|| IsRedPrimalSpecies(species)
		|| IsBluePrimalSpecies(species)
		|| IsUltraNecrozmaSpecies(species))
			return AI_SCRIPT_CHECK_BAD_MOVE | AI_SCRIPT_SEMI_SMART;

		for (u32 i = 0; sSmartWildAITable[i].species != 0xFFFF; ++i)
		{
			if (sSmartWildAITable[i].species == species)
				return sSmartWildAITable[i].aiFlags;
		}

		return FALSE;
	#endif
}

void PopulateAIScriptStructWithBaseAttackerData(struct AIScript* data, u8 bankAtk)
{
	data->atkSpecies = SPECIES(bankAtk);
	data->atkItem = ITEM(bankAtk);
	data->atkItemEffect = ITEM_EFFECT(bankAtk);
	data->atkItemQuality = ITEM_QUALITY(bankAtk);

	data->atkStatus1 = gBattleMons[bankAtk].status1;
	data->atkStatus2 = gBattleMons[bankAtk].status2;
	data->atkStatus3 = gStatuses3[bankAtk];
	data->atkGender = GetGenderFromSpeciesAndPersonality(data->atkSpecies, gBattleMons[bankAtk].personality);

	data->atkAttack = gBattleMons[bankAtk].attack;
	data->atkDefense = gBattleMons[bankAtk].defense;
	data->atkSpeed = gBattleMons[bankAtk].speed;
	data->atkSpAtk = gBattleMons[bankAtk].spAttack;
	data->atkSpDef = gBattleMons[bankAtk].spDefense;

	data->bankAtkPartner = (IS_DOUBLE_BATTLE) ? PARTNER(bankAtk) : bankAtk;
	data->atkPartnerAbility = (IS_DOUBLE_BATTLE) ? ABILITY(data->bankAtkPartner) : ABILITY_NONE;

	//Load Alternative targets
	data->foe1 = FOE(bankAtk);
	data->foe2 = (IS_DOUBLE_BATTLE) ? PARTNER(data->foe1) : data->foe1;
}

void PopulateAIScriptStructWithBaseDefenderData(struct AIScript* data, u8 bankDef)
{
	data->defSpecies = SPECIES(bankDef);
	data->defItem = ITEM(bankDef);
	data->defItemEffect = ITEM_EFFECT(bankDef);
	data->defItemQuality = ITEM_QUALITY(bankDef);

	data->defStatus1 = gBattleMons[bankDef].status1;
	data->defStatus2 = gBattleMons[bankDef].status2;
	data->defStatus3 = gStatuses3[bankDef];
	data->defGender = GetGenderFromSpeciesAndPersonality(data->defSpecies, gBattleMons[bankDef].personality);

	data->defAttack = gBattleMons[bankDef].attack;
	data->defDefense = gBattleMons[bankDef].defense;
	data->defSpeed = gBattleMons[bankDef].speed;
	data->defSpAtk = gBattleMons[bankDef].spAttack;
	data->defSpDef = gBattleMons[bankDef].spDefense;

	data->bankDefPartner = (IS_DOUBLE_BATTLE) ? PARTNER(bankDef) : bankDef;
	data->defPartnerAbility = (IS_DOUBLE_BATTLE) ? ABILITY(data->bankDefPartner) : ABILITY_NONE;

	data->partnerMove = MOVE_NONE;
	if (!IsBankIncapacitated(data->bankAtkPartner))
		data->partnerMove = GetAIChosenMove(data->bankAtkPartner, bankDef);

	data->partnerHandling = IS_DOUBLE_BATTLE
		&& BATTLER_ALIVE(data->bankAtkPartner)
		&& !IsBankIncapacitated(data->bankAtkPartner)
		&& gChosenMovesByBanks[data->bankAtkPartner] != MOVE_NONE //Partner actually selected a move
		&& gBattleStruct->moveTarget[data->bankAtkPartner] == bankDef
		&& gBattleMoves[data->partnerMove].target & MOVE_TARGET_SELECTED //Partner isn't using spread move
		&& CountAliveMonsInBattle(BATTLE_ALIVE_DEF_SIDE, 0, bankDef) >= 2 //With one target left, both Pokemon should aim for the same target
		&& MoveKnocksOutXHits(data->partnerMove, data->bankAtkPartner, gBattleStruct->moveTarget[data->bankAtkPartner], 1);
}

static void PredictMovesForBanks(void)
{
	int i, j;
	u8 viabilities[MAX_MON_MOVES] = {0};
	u8 bestMoves[MAX_MON_MOVES] = {0};
	struct AIScript aiScriptData = {0};

	Memset(gNewBS->ai.movePredictions, 0, sizeof(gNewBS->ai.movePredictions)); //Clear old predictions

	for (u8 bankAtk = 0; bankAtk < gBattlersCount; ++bankAtk)
	{
		if (BATTLER_ALIVE(bankAtk))
		{
			u32 moveLimitations = CheckMoveLimitations(bankAtk, 0, 0xFF); //Don't predict Dynamax
			PopulateAIScriptStructWithBaseAttackerData(&aiScriptData, bankAtk);

			for (u8 bankDef = 0; bankDef < gBattlersCount; ++bankDef)
			{
				if (bankAtk == bankDef || !BATTLER_ALIVE(bankDef)) continue;

				if (gBattleMons[bankAtk].status2 & STATUS2_RECHARGE
				||  gDisableStructs[bankAtk].truantCounter != 0)
				{
					StoreMovePrediction(bankAtk, bankDef, MOVE_NONE);
				}
				else if (IsBankAsleep(bankAtk)
				&& !MoveEffectInMoveset(EFFECT_SLEEP_TALK, bankAtk) && !MoveEffectInMoveset(EFFECT_SNORE, bankAtk)) //Can't get around sleep
				{
					StoreMovePrediction(bankAtk, bankDef, MOVE_NONE);
				}
				else if (gBattleMons[bankAtk].status2 & STATUS2_MULTIPLETURNS
				&& MoveInMoveset(gLockedMoves[bankAtk], bankAtk)) //Still knows locked move
				{
					StoreMovePrediction(bankAtk, bankDef, gLockedMoves[bankAtk]);
				}
				else
				{
					u32 backupFlags = AI_THINKING_STRUCT->aiFlags; //Backup flags so killing in negatives is ignored
					AI_THINKING_STRUCT->aiFlags = 7;
					PopulateAIScriptStructWithBaseDefenderData(&aiScriptData, bankDef);

					for (i = 0; i < MAX_MON_MOVES && gBattleMons[bankAtk].moves[i] != MOVE_NONE; ++i)
					{
						viabilities[i] = 0;
						bestMoves[i] = 0;

						if (gBitTable[i] & moveLimitations) continue;

						u16 move = gBattleMons[bankAtk].moves[i];
						move = TryReplaceMoveWithZMove(bankAtk, bankDef, move);
						viabilities[i] = AIScript_Negatives(bankAtk, bankDef, move, 100, &aiScriptData);
						viabilities[i] = AIScript_Positives(bankAtk, bankDef, move, viabilities[i], &aiScriptData);
					}

					AI_THINKING_STRUCT->aiFlags = backupFlags;

					bestMoves[j = 0] = GetMaxByteIndexInList(viabilities, MAX_MON_MOVES) + 1;
					for (i = 0; i < MAX_MON_MOVES; ++i)
					{
						if (i + 1 != bestMoves[0] //i is not the index returned from GetMaxByteIndexInList
						&& viabilities[i] == viabilities[bestMoves[j] - 1])
							bestMoves[++j] = i + 1;
					}

					if (viabilities[GetMaxByteIndexInList(viabilities, MAX_MON_MOVES)] < 100) //Best move has viability < 100
						StoreSwitchPrediction(bankAtk, bankDef);
					else
						StoreMovePrediction(bankAtk, bankDef, gBattleMons[bankAtk].moves[bestMoves[Random() % (j + 1)] - 1]);

					Memset(viabilities, 0, sizeof(viabilities));
				}
			}
		}
	}
}

static void UpdateStrongestMoves(void)
{
	u8 bankAtk, bankDef;

	for (bankAtk = 0; bankAtk < gBattlersCount; ++bankAtk)
	{
		struct BattlePokemon backupMonAtk;
		u8 backupAbilityAtk = ABILITY_NONE;
		u16 backupSpeciesAtk = SPECIES_NONE;

		if (!IS_TRANSFORMED(bankAtk)
		&& !BankMegaEvolved(bankAtk, FALSE)
		&&  MegaEvolutionEnabled(bankAtk)
		&& !DoesZMoveUsageStopMegaEvolution(bankAtk))
		{
			gNewBS->ai.megaPotential[bankAtk] = CanMegaEvolve(bankAtk, FALSE);

			if (gNewBS->ai.megaPotential[bankAtk] == NULL)
				gNewBS->ai.megaPotential[bankAtk] = CanMegaEvolve(bankAtk, TRUE); //Check Ultra Burst
		}

		if (IS_SINGLE_BATTLE) //There's a high chance these values will be used in singles so calc now.
		{
			TryTempMegaEvolveBank(bankAtk, &backupMonAtk, &backupSpeciesAtk, &backupAbilityAtk);

			for (bankDef = 0; bankDef < gBattlersCount; ++bankDef)
			{
				if (bankAtk == bankDef || bankDef == PARTNER(bankAtk))
					continue; //Don't bother calculating for these Pokemon. Never used

				struct BattlePokemon backupMonDef;
				u8 backupAbilityDef = ABILITY_NONE;
				u16 backupSpeciesDef = SPECIES_NONE;
				TryTempMegaEvolveBank(bankDef, &backupMonDef, &backupSpeciesDef, &backupAbilityDef);

				gNewBS->ai.strongestMove[bankAtk][bankDef] = CalcStrongestMove(bankAtk, bankDef, FALSE);
				gNewBS->ai.canKnockOut[bankAtk][bankDef] = MoveKnocksOutXHits(gNewBS->ai.strongestMove[bankAtk][bankDef], bankAtk, bankDef, 1);
				gNewBS->ai.can2HKO[bankAtk][bankDef] = (gNewBS->ai.canKnockOut[bankAtk][bankDef]) ? TRUE
													  : MoveKnocksOutXHits(gNewBS->ai.strongestMove[bankAtk][bankDef], bankAtk, bankDef, 2); //If you can KO in 1 hit you can KO in 2

				TryRevertTempMegaEvolveBank(bankDef, &backupMonDef, &backupSpeciesDef, &backupAbilityDef);
			}

			TryRevertTempMegaEvolveBank(bankAtk, &backupMonAtk, &backupSpeciesAtk, &backupAbilityAtk);
		}
	}
}

static void UpdateBestDoublesKillingMoves(void)
{
	if (IS_DOUBLE_BATTLE)
	{
		u8 bankAtk, bankDef;

		for (bankAtk = 0; bankAtk < gBattlersCount; ++bankAtk)
		{
			//mgba_printf(MGBA_LOG_INFO, "");
			for (bankDef = 0; bankDef < gBattlersCount; ++bankDef)
			{
				if (bankAtk == bankDef || bankDef == PARTNER(bankAtk) || !BATTLER_ALIVE(bankDef))
					continue; //Don't bother calculating for these Pokemon. Never used

				//mgba_printf(MGBA_LOG_WARN, "");
				UpdateBestDoubleKillingMoveScore(bankAtk, bankDef, PARTNER(bankAtk), PARTNER(bankDef), gNewBS->ai.bestDoublesKillingScores[bankAtk][bankDef], &gNewBS->ai.bestDoublesKillingMoves[bankAtk][bankDef]);
			}
		}
	}
}

static void RunCalcShouldAIDynamax(void)
{
	for (u8 i = 0; i < NUM_BATTLE_SIDES; ++i)
		CalcAIDynamaxMon(i);

	for (u8 i = 0; i < gBattlersCount; ++i)
	{
		for (u8 j = 0; j < gBattlersCount; ++j)
			CalcShouldAIDynamax(i, j);
	}
}

static u32 GetMaxByteIndexInList(const u8 array[], const u32 size)
{
	u8 maxIndex = 0;

	for (u32 i = 0; i < size; ++i)
	{
		if (array[i] > array[maxIndex])
			maxIndex = i;
	}

	return maxIndex;
}

static u8 GetAI_ItemType(u16 itemId, const u8 *itemEffect) //Fixed from vanilla
{
    if (itemId == ITEM_FULL_RESTORE)
        return AI_ITEM_FULL_RESTORE;
    else if (itemEffect[4] & ITEM4_HEAL_HP)
        return AI_ITEM_HEAL_HP;
    else if (itemEffect[3] & ITEM3_STATUS_ALL)
        return AI_ITEM_CURE_CONDITION;
    else if (itemEffect[0] & (ITEM0_HIGH_CRIT | ITEM0_X_ATTACK) || itemEffect[1] != 0 || itemEffect[2] != 0)
        return AI_ITEM_X_STAT;
    else if (itemEffect[3] & ITEM3_MIST)
        return AI_ITEM_GUARD_SPECS;
    else
        return AI_ITEM_NOT_RECOGNIZABLE;
}

static bool8 ShouldAIUseItem(void)
{
	u32 i;
	u8 validMons = 0;
	bool8 shouldUse = FALSE;
	
	if (SIDE(gActiveBattler) == B_SIDE_PLAYER)
		return FALSE;

	struct Pokemon* party;
	u8 firstId, lastId;
	party = LoadPartyRange(gActiveBattler, &firstId, &lastId);

	for (i = 0; i < PARTY_SIZE; ++i)
	{
		if (MON_CAN_BATTLE(&party[i]))
			++validMons;
	}

	for (i = 0; i < 4; ++i) //Number of Trainer items
	{
		u16 item;
		const u8 *itemEffects;
		u8 paramOffset;

		//if (i > 0 && validMons > (BATTLE_HISTORY->itemsNo - i) + 1) //Spread out item usage
		//	continue;
		item = BATTLE_HISTORY->trainerItems[i];
		itemEffects = gItemEffectTable[item - ITEM_POTION];

		if (item == ITEM_NONE || itemEffects == NULL)
			continue;

		switch (gBattleStruct->AI_itemType[gActiveBattler & BIT_FLANK] = GetAI_ItemType(item, itemEffects))
		{
			case AI_ITEM_FULL_RESTORE:
				if (BATTLER_ALIVE(gActiveBattler) && !BATTLER_MAX_HP(gActiveBattler))
				{
					FULL_RESTORE_LOGIC:
					if (AI_THINKING_STRUCT->aiFlags <= AI_SCRIPT_CHECK_BAD_MOVE) //Dumb AI
					{
						if (gBattleMons[gActiveBattler].hp < gBattleMons[gActiveBattler].maxHP / 4)
							shouldUse = TRUE;
					}
					else if (gBattleMons[gActiveBattler].hp < gBattleMons[gActiveBattler].maxHP / 2) //Smart AI should only use at less than half health
					{
						u8 foe = FOE(gActiveBattler);
						if ((BATTLER_ALIVE(foe) && ShouldRecover(gActiveBattler, foe, 0xFFFF))
						|| (IS_DOUBLE_BATTLE && BATTLER_ALIVE(PARTNER(foe)) && ShouldRecover(gActiveBattler, PARTNER(foe), 0xFFFF)))
						{
							shouldUse = TRUE;
						}
					}
				}
				break;
			case AI_ITEM_HEAL_HP:
				paramOffset = GetItemEffectParamOffset(item, 4, 4);
				if (paramOffset > 0 && BATTLER_ALIVE(gActiveBattler) && !BATTLER_MAX_HP(gActiveBattler))
				{
					if (gBattleMons[gActiveBattler].maxHP - gBattleMons[gActiveBattler].hp > itemEffects[paramOffset]) //Item won't restore all HP
						shouldUse = TRUE;
					else
						goto FULL_RESTORE_LOGIC;
				}
				break;
			case AI_ITEM_CURE_CONDITION: ;
				u32 status1 = gBattleMons[gActiveBattler].status1;
				gBattleStruct->AI_itemFlags[gActiveBattler & BIT_FLANK] = 0;
				if (itemEffects[3] & ITEM3_SLEEP && status1 & STATUS1_SLEEP)
				{
					gBattleStruct->AI_itemFlags[gActiveBattler & BIT_FLANK] |= 0x20;
					shouldUse = TRUE;
				}
				if (itemEffects[3] & ITEM3_POISON && (status1 & STATUS1_PSN_ANY))
				{
					gBattleMons[gActiveBattler].status1 = 0; //Temporarily remove status
					if (!GoodIdeaToPoisonSelf(gActiveBattler)) //Pokemon shouldn't be poisoned
					{
						gBattleStruct->AI_itemFlags[gActiveBattler & BIT_FLANK] |= 0x10; //So heal it
						shouldUse = TRUE;
					}
					gBattleMons[gActiveBattler].status1 = status1; //Restore from backup
				}
				if (itemEffects[3] & ITEM3_BURN && status1 & STATUS1_BURN)
				{
					gBattleMons[gActiveBattler].status1 = 0; //Temporarily remove status
					if (!GoodIdeaToBurnSelf(gActiveBattler)) //Pokemon shouldn't be burned
					{
						gBattleStruct->AI_itemFlags[gActiveBattler & BIT_FLANK] |= 0x8; //So heal it
						shouldUse = TRUE;
					}
					gBattleMons[gActiveBattler].status1 = status1; //Restore from backup
				}
				if (itemEffects[3] & ITEM3_FREEZE && status1 & STATUS1_FREEZE)
				{
					gBattleStruct->AI_itemFlags[gActiveBattler & BIT_FLANK] |= 0x4;
					shouldUse = TRUE;
				}
				if (itemEffects[3] & ITEM3_PARALYSIS && status1 & STATUS1_PARALYSIS)
				{
					gBattleMons[gActiveBattler].status1 = 0; //Temporarily remove status
					if (!GoodIdeaToParalyzeSelf(gActiveBattler)) //Pokemon shouldn't be paralyzed
					{
						gBattleStruct->AI_itemFlags[gActiveBattler & BIT_FLANK] |= 0x2; //So heal it
						shouldUse = TRUE;
					}
					gBattleMons[gActiveBattler].status1 = status1; //Restore from backup
				}
				if (itemEffects[3] & ITEM3_CONFUSION && gBattleMons[gActiveBattler].status2 & STATUS2_CONFUSION)
				{
					gBattleStruct->AI_itemFlags[gActiveBattler & BIT_FLANK] |= 0x1;
					shouldUse = TRUE;
				}
				break;
			case AI_ITEM_X_STAT:
				gBattleStruct->AI_itemFlags[gActiveBattler & BIT_FLANK] = 0;
				if (!gDisableStructs[gActiveBattler].isFirstTurn)
					break;
				if (itemEffects[0] & ITEM0_X_ATTACK)
					gBattleStruct->AI_itemFlags[gActiveBattler & BIT_FLANK] |= 0x1;
				if (itemEffects[1] & ITEM1_X_DEFEND)
					gBattleStruct->AI_itemFlags[gActiveBattler & BIT_FLANK] |= 0x2;
				if (itemEffects[1] & ITEM1_X_SPEED)
					gBattleStruct->AI_itemFlags[gActiveBattler & BIT_FLANK] |= 0x4;
				if (itemEffects[2] & ITEM2_X_SPATK)
				{
					if (item != ITEM_X_SP_DEF) //Sp. Atk
						gBattleStruct->AI_itemFlags[gActiveBattler & BIT_FLANK] |= 0x8;
					else //Sp. Def
						gBattleStruct->AI_itemFlags[gActiveBattler & BIT_FLANK] |= 0x10;
				}
				if (itemEffects[2] & ITEM2_X_ACCURACY)
					gBattleStruct->AI_itemFlags[gActiveBattler & BIT_FLANK] |= 0x20;
				if (itemEffects[0] & ITEM0_HIGH_CRIT)
					gBattleStruct->AI_itemFlags[gActiveBattler & BIT_FLANK] |= 0x80;
				shouldUse = TRUE;
				break;
			case AI_ITEM_GUARD_SPECS:
				if (gDisableStructs[gActiveBattler].isFirstTurn && gSideTimers[SIDE(gActiveBattler)].mistTimer == 0)
					shouldUse = TRUE;
				break;
			case AI_ITEM_NOT_RECOGNIZABLE:
				return FALSE;
		}

		if (shouldUse)
		{
			EmitTwoReturnValues(1, ACTION_USE_ITEM, 0);
			gBattleStruct->chosenItem[gActiveBattler & BIT_FLANK] = item;
			BATTLE_HISTORY->trainerItems[i] = 0;
			return shouldUse;
		}
	}
	return FALSE;
}

#ifdef VAR_GAME_DIFFICULTY
static bool8 IsGoodIdeaToDoShiftSwitch(u8 switchBank, u8 foe)
{
	if (!CanKnockOut(switchBank, foe) //Current mon out can't KO new mon being switched in
	&& !WillTakeSignificantDamageFromEntryHazards(switchBank, 2)) //50% health loss
	{
		u8 mostSuitableScore = GetMostSuitableMonToSwitchIntoScore();

		if (mostSuitableScore > SWITCHING_INCREASE_KO_FOE) //Potential switch in has at least two advantages
			return TRUE;
		
		if (OnlyBadMovesLeftInMoveset(switchBank, foe)) //AI mon has nothing good against this new foe
			return TRUE;

		if (CanKnockOut(foe, switchBank) && mostSuitableScore >= SWITCHING_INCREASE_KO_FOE) //New foe can KO current AI mon
			return TRUE;
	}

	return FALSE; //Don't switch
}
#endif

void ShouldDoAIShiftSwitch(void)
{
	#ifdef VAR_GAME_DIFFICULTY
	if (gBattleTypeFlags & BATTLE_TYPE_TRAINER
	&& !IS_DOUBLE_BATTLE
	&& gBattleScripting.battleStyle == OPTIONS_BATTLE_STYLE_SHIFT
	&& BATTLER_ALIVE(GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT)) //AI has a mon that can be switched out
	&& VarGet(VAR_GAME_DIFFICULTY) >= OPTIONS_EXPERT_DIFFICULTY)
	{
		gActiveBattler = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
		u8 foe = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);

		CalculateAIPredictions();
		if (IsGoodIdeaToDoShiftSwitch(gActiveBattler, foe))
			return; //Continue in script
	}
	#endif

	gBattlescriptCurrInstr = BattleScript_FaintedMonChooseAnotherRejoin - 5;
}
