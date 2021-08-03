#include "../defines.h"
#include "../defines_battle.h"
#include "../../include/constants/items.h"
#include "../../include/constants/item_effects.h"
#include "../../include/constants/region_map_sections.h"
#include "../../include/constants/trainers.h"

#include "../../include/new/accuracy_calc.h"
#include "../../include/new/attackcanceler.h"
#include "../../include/new/ai_advanced.h"
#include "../../include/new/ai_util.h"
#include "../../include/new/ai_master.h"
#include "../../include/new/ai_scripts.h"
#include "../../include/new/battle_controller_opponent.h"
#include "../../include/new/battle_start_turn_start.h"
#include "../../include/new/battle_util.h"
#include "../../include/new/damage_calc.h"
#include "../../include/new/end_turn.h"
#include "../../include/new/frontier.h"
#include "../../include/new/end_turn_battle_scripts.h"
#include "../../include/new/general_bs_commands.h"
#include "../../include/new/move_menu.h"
#include "../../include/new/mega.h"
#include "../../include/new/multi.h"
#include "../../include/new/move_tables.h"
#include "../../include/new/species_tables.h"
#include "../../include/new/set_z_effect.h"
#include "../../include/new/switching.h"
#include "../../include/new/util.h"

/*
ai_master.c
	The master function(s) for the AI logic.
*/

//TODO: Add two best switching mons, one for KOing and the other for stalling
//TODO: Touch up switching logic in doubles

// AI states
enum
{
	AIState_SettingUp,
	AIState_Processing,
	AIState_FinishedProcessing,
	AIState_DoNotProcess,
};

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
static void CheckDeperateAttempt(u8 bankAtk, u8 bankDef, u8 chosenMovePos, struct AIScript* aiScriptData);
static u8 ChooseMoveOrAction_Singles(struct AIScript* aiScriptData);
static u8 ChooseMoveOrAction_Doubles(struct AIScript* aiScriptData);
static void BattleAI_DoAIProcessing(struct AIScript* aiScriptData);
static void CalculateAIPredictions(void);
static bool8 ShouldSwitch(struct Pokemon* party, u8 firstId, u8 lastId);
static bool8 ShouldSwitchIfOnlyBadMovesLeft(struct Pokemon* party);
static bool8 FindMonThatAbsorbsOpponentsMove(struct Pokemon* party, u8 firstId, u8 lastId);
static bool8 SwitchToBestResistMon(const struct Pokemon* party, bool8 willPivot);
static bool8 ShouldSwitchIfNaturalCureOrRegenerator(struct Pokemon* party);
static bool8 PassOnWish(struct Pokemon* party, u8 firstId, u8 lastId);
static bool8 SemiInvulnerableTroll(void);
static u8 GetBestPartyNumberForSemiInvulnerableLockedMoveCalcs(u8 opposingBattler1, u8 opposingBattler2, bool8 checkLockedMoves);
static bool8 RunAllSemiInvulnerableLockedMoveCalcs(u8 opposingBattler1, u8 opposingBattler2, bool8 checkLockedMoves);
static bool8 TheCalcForSemiInvulnerableTroll(u8 bankAtk, u8 flags, bool8 checkLockedMoves);
static bool8 CanStopLockedMove(void);
static bool8 IsYawned(void);
static bool8 ShouldSwitchWhileAsleep(struct Pokemon* party);
static bool8 IsTakingAnnoyingSecondaryDamage(struct Pokemon* party);
static bool8 ShouldSwitchToAvoidDeath(struct Pokemon* party);
static bool8 ShouldSwitchIfWonderGuard(struct Pokemon* party, u8 firstId, u8 lastId);
static bool8 ShouldSwitchWhenOffensiveStatsAreLow(struct Pokemon* party);
static bool8 ShouldSaveSweeperForLater(struct Pokemon* party);
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
static void TryRechooseAIMoveIfPlayerSwitchCheesed(u8 aiBank, u8 playerBank);
static bool8 IsPlayerTryingToCheeseWithRepeatedSwitches(u8 playerBank);
static bool8 IsPlayerTryingToCheeseAI(u8 playerBank, u8 aiBank);
static void PickNewAIMove(u8 aiBank, bool8 allowPursuit, bool8 allowNewHostileMove);
static void UpdateCurrentTargetByMoveTarget(u8 moveTarget, u8 aiBank);

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
		&& !IsFrontierTrainerId(gTrainerBattleOpponent_A)
		&& !IsBagDisabled()) //If the player's bag is disabled, the AI's should also be
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
		gBankTarget = (AIRandom() & BIT_FLANK) + (SIDE(gActiveBattler) ^ BIT_SIDE);
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

		AI_THINKING_STRUCT->simulatedRNG[i] = AIRandom() % 100; //Used to be 100 - (Random() % 16);
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
	else if (gBattleTypeFlags & BATTLE_TYPE_SHADOW_WARRIOR)
		flags = AI_SCRIPT_CHECK_BAD_MOVE | AI_SCRIPT_SEMI_SMART; //Shadow Warrior is pretty smart
	else if (gBattleTypeFlags & BATTLE_TYPE_SCRIPTED_WILD_2) //No idea how these two work
		flags = AI_SCRIPT_CHECK_BAD_MOVE | WildMonIsSmart(gActiveBattler);
	else if (gBattleTypeFlags & BATTLE_TYPE_LEGENDARY_FRLG)
		flags = AI_SCRIPT_CHECK_BAD_MOVE | AI_SCRIPT_SEMI_SMART;
	else
	{
		if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS)
			flags = gTrainers[gTrainerBattleOpponent_A].aiFlags | gTrainers[VarGet(VAR_SECOND_OPPONENT)].aiFlags;
		else
			flags = gTrainers[gTrainerBattleOpponent_A].aiFlags;

		#ifdef VAR_GAME_DIFFICULTY
		if (difficulty == OPTIONS_EASY_DIFFICULTY && gBattleTypeFlags & BATTLE_TYPE_TRAINER)
		{
			if (flags & AI_SCRIPT_CHECK_GOOD_MOVE) //Trainers who are supposed to be better than the average Trainer
			{
				flags &= ~AI_SCRIPT_CHECK_GOOD_MOVE;
				flags |= AI_SCRIPT_SEMI_SMART; //Make normally smart Trainers semi smart
			}
			else
				flags = AI_SCRIPT_CHECK_BAD_MOVE; //Trainers are always barely smart in easy mode
		}
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
				flags = AI_SCRIPT_CHECK_BAD_MOVE | WildMonIsSmart(gActiveBattler); //Even Wild Pokemon are moderately smart in expert mode
		}
		#endif

		if (!(gBattleTypeFlags & BATTLE_TYPE_TRAINER)) //Generic wild battle
			flags |= WildMonIsSmart(gActiveBattler);
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
		CheckDeperateAttempt(gBankAttacker, FOE(gBankAttacker), ret, &aiScriptData); //Record if the AI is using a move out of desperation
	}
	else
	{
		ret = ChooseMoveOrAction_Doubles(&aiScriptData);

		if ((!BATTLER_ALIVE(aiScriptData.foe1) || !BATTLER_ALIVE(aiScriptData.foe2)) //Only one foe left
		&& !BATTLER_ALIVE(aiScriptData.bankAtkPartner)) //Partner gone too making it 1v1
			CheckDeperateAttempt(gBankAttacker, gBankTarget, ret, &aiScriptData); //Record if the AI is using a move out of desperation (treat like single battle)
	}

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
		*GetAbilityLocation(bank) = GetMonAbilityAfterTrace(mon, FOE(bank));

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

void WipeOldDeperateAttemptRecord(u8 bankAtk)
{
	gNewBS->ai.usingDesperateMove[bankAtk] = FALSE;
}

static void CheckDeperateAttempt(u8 bankAtk, u8 bankDef, u8 chosenMovePos, struct AIScript* aiScriptData)
{
	if (chosenMovePos < MAX_MON_MOVES) //Chose a move
	{
		u16 chosenMove = GetBattleMonMove(bankAtk, chosenMovePos);
		u16 foePrediction = IsValidMovePrediction(bankDef, bankAtk);

		if (foePrediction != MOVE_NONE
		&& aiScriptData->defSpeed > aiScriptData->atkSpeed //Foe is faster
		&& MoveKnocksOutXHits(foePrediction, bankDef, bankAtk, 1)) //Smart thing for foe to do would be to KO the player this turn
		{
			if (!(GetBaseMoveTarget(chosenMove, bankAtk) & (MOVE_TARGET_USER | MOVE_TARGET_USER_OR_PARTNER | MOVE_TARGET_OPPONENTS_FIELD | MOVE_TARGET_DEPENDS)) //Chosen move will hit foe
			&& PriorityCalc(bankAtk, ACTION_USE_MOVE, chosenMove) > 0 //And chosen move is priority move
			&& StrongestMoveGoesFirst(chosenMove, bankAtk, bankDef)) //The chosen move is the strongest move that would go first
				gNewBS->ai.usingDesperateMove[bankAtk] = TRUE; //This is clearly a desperate attempt, and the player may notice this (generally set until mon faints)
		}
		else
			WipeOldDeperateAttemptRecord(bankAtk); //AI isn't in a desperate situation anymore
	}
}

static u8 ChooseMoveOrAction_Singles(struct AIScript* aiScriptData)
{
	u8 currentMoveArray[4];
	u8 consideredMoveArray[4];
	u8 numOfBestMoves;
	s32 i;
	u32 flags = AI_THINKING_STRUCT->aiFlags;

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

	return consideredMoveArray[AIRandom() % numOfBestMoves];
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
	s32 mostViableTargetsNo, mostViableMovesNo, mostMovePoints;

	//Get best moves for each potential target
	for (i = 0; i < MAX_BATTLERS_COUNT; ++i)
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

				actionOrMoveIndex[i] = mostViableMovesIndices[AIRandom() % mostViableMovesNo];
				bestMovePointsForTarget[i] = mostViableMovesScores[0];

				// Don't use a move against ally if it has less than 101 points.
				if (i == (gBankAttacker ^ BIT_FLANK) && bestMovePointsForTarget[i] < 101)
				{
					bestMovePointsForTarget[i] = -1;
				}
			}
		}
	}

	/*
	Choose best potential targets. If targets have the same score, the better target is determined by:
	1. Move that can knock out target.
		- KOing both targets then chooses foe that can KO (poses bigger threat)
	2. Move that does the most damage.
	*/
	mostMovePoints = bestMovePointsForTarget[0]; //Start with first target
	mostViableTargetsArray[0] = 0;
	mostViableTargetsNo = 1;

	u16 firstMove = actionOrMoveIndex[0];
	//u32 mostDamage = 0; //The most damage AI could do to any target
	u32 mostDmgTarget = 0; //The bank the most damage could be done to
	bool8 mostDmgKnocksOut = FALSE; //Whether or not the strongest move also KOs
	bool8 statusMoveOption = FALSE; //Whether or not a status move will potentially be used

	if (firstMove < MAX_MON_MOVES) //A move was chosen against the first target
	{
		if (SPLIT(firstMove) != SPLIT_STATUS) //Move against first target isn't status move
		{
			//Calculate important data for later
			mostDmgTarget = mostViableTargetsArray[0]; //First target
			mostDmgKnocksOut = MoveKnocksOutXHits(firstMove, gBankAttacker, mostDmgTarget, 1);
			//mostDamage = GetFinalAIMoveDamage(firstMove, gBankAttacker, mostDmgTarget, 1, NULL); //Assumes damage has already been cached
		}
		else
			statusMoveOption = TRUE;
	}

	for (i = 1; i < MAX_BATTLERS_COUNT; ++i) //Start by looping through the rest of the targets
	{
		if (bestMovePointsForTarget[i] == mostMovePoints) //This target is as good to hit as the other one
		{
			u16 move = actionOrMoveIndex[i];
			statusMoveOption = statusMoveOption || (move < MAX_MON_MOVES && SPLIT(move) == SPLIT_STATUS); //Update if status move can potentially be used

			if (SIDE(i) != SIDE(gBankAttacker) //Don't do damage calcs for mons on the same side
			&& move < MAX_MON_MOVES //Move was chosen
			&& SPLIT(move) != SPLIT_STATUS) //And this move isn't a status move
			{
				//Try choosing the target which the most damage can be done to
				bool8 thisDmgKnocksOut = MoveKnocksOutXHits(move, gBankAttacker, i, 1);
				//u32 thisDamage = GetFinalAIMoveDamage(move, gBankAttacker, i, 1, NULL); //Assumes damage has already been cached

				if (!thisDmgKnocksOut && mostDmgKnocksOut)
				{
					//Best move KOs a target but this target can't be KOd so skip this move
					continue;
				}
				else if (thisDmgKnocksOut && !mostDmgKnocksOut)
				{
					//This move KOs a target making it better than even the move that could do a higher damage number to another target
					//Fallthrough and replace the most damaging move
				}
				else if (thisDmgKnocksOut && mostDmgKnocksOut)
				{
					//Both targets can be KOd so they're both viable
					//But the better target is the one that poses a bigger threat
					bool8 thisFoeCanKOAI = CanKnockOut(i, gBankAttacker);
					bool8 bestFoeCanKOAI = CanKnockOut(mostDmgTarget, gBankAttacker);

					if (!(AI_THINKING_STRUCT->aiFlags & AI_SCRIPT_CHECK_GOOD_MOVE))
						goto ADD_TARGET_AS_MOST_VIABLE; //Dumb and Semi-Smart AI doesn't care about foe that can KO it
					else if (!thisFoeCanKOAI && bestFoeCanKOAI)
						continue; //Prioritize foe that can KO the AI
					else if (thisFoeCanKOAI && bestFoeCanKOAI)
						goto ADD_TARGET_AS_MOST_VIABLE; //Both foes can KO the AI, so both are good targets
					else if (!thisFoeCanKOAI && !bestFoeCanKOAI)
						goto ADD_TARGET_AS_MOST_VIABLE; //Neither foe can KO the AI, so both are good targets
					//else if (thisFoeCanKOAI && !bestFoeCanKOAI) //Inferred
						//Fallthrough and replace best damaging moves
				}
				else //if (!thisDmgKnocksOut && !mostDmgKnocksOut) //Inferred
				{
					//Neither target can be knocked out

					/*
					This is commented out because the AI gets too predictable when it always goes for the mon it can do the most damage to

					if (thisDamage == mostDamage) //Both moves do the same damage to multiple targets
						goto ADD_TARGET_AS_MOST_VIABLE;
					else if (thisDamage < mostDamage)
						continue; //Don't store this target if less damage can be done to it
					*/
					goto ADD_TARGET_AS_MOST_VIABLE;
				}

				if (!statusMoveOption) //Only replace all moves if no status moves can potentially be used
				{
					mostViableTargetsNo = 1;
					mostViableTargetsArray[0] = mostDmgTarget = i;
					mostDmgKnocksOut = thisDmgKnocksOut;
					//mostDamage = thisDamage;
					continue;
				}
				else //Replace all non status moves with this best one
				{
					mostDmgTarget = i;
					mostDmgKnocksOut = thisDmgKnocksOut;
					//mostDamage = thisDamage;

					for (j = 0; j < mostViableTargetsNo; ++j)
					{
						u8 bankDef = mostViableTargetsArray[j];
						u16 move = actionOrMoveIndex[bankDef];
						
						if (move < MAX_MON_MOVES && SPLIT(move) != SPLIT_STATUS)
							mostViableTargetsArray[j] = mostDmgTarget;
					}

					//TODO: Filter out duplicates
					continue;
				}
			}

			ADD_TARGET_AS_MOST_VIABLE:
			mostViableTargetsArray[mostViableTargetsNo++] = i;
		}
		else if (bestMovePointsForTarget[i] > mostMovePoints) //This target is the best one so far
		{
			//Make target new best and only target
			mostViableTargetsNo = 1;
			mostViableTargetsArray[0] = i;
			mostMovePoints = bestMovePointsForTarget[i];

			if (i + 1 < MAX_BATTLERS_COUNT) //Don't waste time if loop is about to end
			{
				if (SPLIT(actionOrMoveIndex[i]) != SPLIT_STATUS) //Move against target isn't status move
				{
					//Calculate important data for later
					statusMoveOption = FALSE;
					mostDmgTarget = i; //New target
					mostDmgKnocksOut = MoveKnocksOutXHits(actionOrMoveIndex[i], gBankAttacker, mostDmgTarget, 1);
					//mostDamage = GetFinalAIMoveDamage(actionOrMoveIndex[i], gBankAttacker, mostDmgTarget, 1, NULL); //Assumes damage has already been cached
				}
				else
					statusMoveOption = TRUE;
			}
		}
	}

	gBankTarget = mostViableTargetsArray[AIRandom() % mostViableTargetsNo];
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

		if (ShouldSwitch(party, firstId, lastId)
		&& !(gNewBS->ai.goodToPivot & gBitTable[gActiveBattler]))
		{
			if (gBattleStruct->switchoutIndex[SIDE(gActiveBattler)] == PARTY_SIZE)
			{
				u8 monToSwitchId = GetMostSuitableMonToSwitchIntoByParty(party);
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

static bool8 ShouldSwitch(struct Pokemon* party, u8 firstId, u8 lastId)
{
	u8 battlerIn1, battlerIn2;
	u8 availableToSwitch;
	u32 i;

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
	if (FindMonThatAbsorbsOpponentsMove(party, firstId, lastId)) //First check if can absorb move
		return TRUE;
	if (PassOnWish(party, firstId, lastId)) //Then try to pass on a wish
		return TRUE;
	if (CanStopLockedMove()) //Then try to stop a locked move
		return TRUE;
	if (ShouldSwitchIfPerishSong()) //Then proceed with the other checks as normal
		return TRUE;
	if (ShouldSwitchIfWonderGuard(party, firstId, lastId))
		return TRUE;
	if (ShouldSwitchIfOnlyBadMovesLeft(party))
		return TRUE;
	if (ShouldSwitchIfNaturalCureOrRegenerator(party))
		return TRUE;
	if (SemiInvulnerableTroll())
		return TRUE;
	if (IsYawned())
		return TRUE;
	if (ShouldSwitchWhileAsleep(party))
		return TRUE;
	if (IsTakingAnnoyingSecondaryDamage(party))
		return TRUE;
	if (ShouldSwitchToAvoidDeath(party))
		return TRUE;
	if (ShouldSwitchWhenOffensiveStatsAreLow(party))
		return TRUE;
	if (ShouldSaveSweeperForLater(party))
		return TRUE;

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

static void ConfirmAISwitch(u8 monId, bool8 willPivot)
{
	//Switch to a mon that can KO and save your damager for later
	if (!willPivot) //U-Turn/Volt Switch switch on their own
	{
		gBattleStruct->switchoutIndex[SIDE(gActiveBattler)] = monId;
		EmitTwoReturnValues(1, ACTION_SWITCH, 0);
	}
	else
	{
		gNewBS->ai.pivotTo[gActiveBattler] = monId;
		gNewBS->ai.goodToPivot |= gBitTable[gActiveBattler];
	}
}

static bool8 PredictedMoveWontDoTooMuchToMon(u8 activeBattler, struct Pokemon* mon, u8 foe, u8 switchFlags)
{
	u16 defMove = IsValidMovePrediction(foe, activeBattler);

	//Check if a switch in general is safe
	if (switchFlags & SWITCHING_FLAG_FAINTS_FROM_FOE)
		return FALSE; //Don't switch and sack your other mon

	//Check if a switch against the predicted move is safe
	if (defMove == MOVE_NONE || SPLIT(defMove) == SPLIT_STATUS) //Predicted move won't do damage
		return TRUE;

	//Check if a switch is worth it at all in Benjamin Butterfree
	if (gBattleTypeFlags & BATTLE_TYPE_BENJAMIN_BUTTERFREE
	&&  gBattleMons[activeBattler].moves[0] == MOVE_NONE) //Mon to be switched out has no moves left
		return FALSE; //Don't switch out and just struggle until you die

	//Check if the switch is a perfect wall
	if (switchFlags & SWITCHING_FLAG_RESIST_ALL_MOVES)
		return TRUE; //Can definitely take multiple hits

	if (gBattleMoves[defMove].effect == EFFECT_SUCKER_PUNCH)
		return TRUE; //Sucker Punch never works on a switch

	//Now run actual damage calc
	u32 predictedDmg = (defMove == MOVE_NONE) ? 0 : AI_CalcMonDefDmg(foe, activeBattler, defMove, mon, NULL);

	if (predictedDmg >= mon->hp)
		return FALSE; //Don't switch and sack your other mon

	if (predictedDmg * 2 < mon->hp)
		return TRUE; //Can take multiple hits

	return predictedDmg * 2 >= mon->maxHP //Will take a sizeable chunk of damage on the switch
		&& HealingMoveInMonMoveset(mon) //But can heal off that damage
		&& SpeedCalcMon(SIDE(activeBattler), mon) > SpeedCalc(foe); //And will get the healing move off first
}

static bool8 PredictedMoveWontKOMon(u8 activeBattler, struct Pokemon* mon, u8 foe)
{
	u16 defMove = IsValidMovePrediction(foe, activeBattler);

	//Check if a switch against the predicted move is safe
	if (defMove == MOVE_NONE || SPLIT(defMove) == SPLIT_STATUS) //Predicted move won't do damage
		return TRUE;

	if (gBattleMoves[defMove].effect == EFFECT_SUCKER_PUNCH)
		return TRUE; //Sucker Punch never works on a switch

	//Now run actual damage calc
	u32 predictedDmg = (defMove == MOVE_NONE) ? 0 : AI_CalcMonDefDmg(foe, activeBattler, defMove, mon, NULL);

	if (predictedDmg >= mon->hp)
		return FALSE; //Don't switch and sack your other mon

	return TRUE;
}

static bool8 ShouldSwitchIfOnlyBadMovesLeft(struct Pokemon* party)
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
			u8 bestMon, secondBestMon, switchFlags, secondBestSwitchFlags;

			//Try to switch out to the best mon
			bestMon = GetMostSuitableMonToSwitchIntoByParty(party);
			switchFlags = GetMostSuitableMonToSwitchIntoFlags();

			if (switchFlags & (SWITCHING_FLAG_OUTSPEEDS | SWITCHING_FLAG_WALLS_FOE | SWITCHING_FLAG_RESIST_ALL_MOVES) //New mon will either go first on continuously take low damage
			&& PredictedMoveWontDoTooMuchToMon(gActiveBattler, &party[bestMon], foe1, switchFlags))
			{
				gBattleStruct->switchoutIndex[SIDE(gActiveBattler)] = PARTY_SIZE;
				EmitTwoReturnValues(1, ACTION_SWITCH, 0);
				return TRUE;
			}

			//Try the second best mon too because staying in with no moves is a bad idea
			secondBestMon = GetSecondMostSuitableMonToSwitchIntoByParty(party);
			secondBestSwitchFlags = GetSecondMostSuitableMonToSwitchIntoFlags();

			if (secondBestMon != PARTY_SIZE
			&& secondBestSwitchFlags & (SWITCHING_FLAG_OUTSPEEDS | SWITCHING_FLAG_WALLS_FOE | SWITCHING_FLAG_RESIST_ALL_MOVES) //New mon will either go first on continuously take low damage
			&& PredictedMoveWontDoTooMuchToMon(gActiveBattler, &party[secondBestMon], foe1, secondBestSwitchFlags))
			{
				gBattleStruct->switchoutIndex[SIDE(gActiveBattler)] = secondBestMon;
				EmitTwoReturnValues(1, ACTION_SWITCH, 0);
				return TRUE;
			}

			//Check more lenient restrictions when you're choice locked
			if (gBattleStruct->choicedMove[gActiveBattler] && CanBeChoiceLocked(gActiveBattler))
			{
				//Try to switch out to the best mon
				if (!(switchFlags & SWITCHING_FLAG_FAINTS_FROM_FOE)
				&& PredictedMoveWontDoTooMuchToMon(gActiveBattler, &party[bestMon], foe1, switchFlags))
				{
					gBattleStruct->switchoutIndex[SIDE(gActiveBattler)] = PARTY_SIZE;
					EmitTwoReturnValues(1, ACTION_SWITCH, 0);
					return TRUE;
				}
			
				//Try to switch out to the second best mon
				if (!(secondBestSwitchFlags & SWITCHING_FLAG_FAINTS_FROM_FOE)
				&& PredictedMoveWontDoTooMuchToMon(gActiveBattler, &party[bestMon], foe1, secondBestSwitchFlags))
				{
					gBattleStruct->switchoutIndex[SIDE(gActiveBattler)] = secondBestMon;
					EmitTwoReturnValues(1, ACTION_SWITCH, 0);
					return TRUE;
				}
			}
		}
	}

	return FALSE;
}

/*
Don't switch to mon if:
1. Mon has been switched to for type absorbing before (75% / 25% not to switch).
2. It will faint from entry hazards (100%)
3. The mon to switch to has an HP absorption Ability and will be switched in at full health, and the mon out can take a couple hits (100% in Singles).
*/
static bool8 TypeAbosorbingSwitchAbilityCheck(struct Pokemon* mon, u8 monId, u16 predictedMove, u8 absorbingTypeAbility1, u8 absorbingTypeAbility2, u8 absorbingTypeAbility3)
{
	u8 side = SIDE(gActiveBattler);

	//Check mon has been switched to for type absorbing before
	if (gNewBS->ai.didTypeAbsorbSwitchToMonBefore[side] & gBitTable[monId]) //Used this mon for a type switch before
	{
		u8 noSwitchChance = (gNewBS->ai.switchingCooldown[gActiveBattler]) ? 75 : 25; //Just switched in (not type absorb) 75% chance not to switch, otherwise 25% chance

		if (AIRandom() % 100 < noSwitchChance)
			return FALSE; //Only allow switching to this mon again for a type absorbing some of the time
	}

	u8 monAbility = GetMonAbilityAfterTrace(mon, FOE(gActiveBattler));

	if (monAbility == absorbingTypeAbility1
	||  monAbility == absorbingTypeAbility2
	||  monAbility == absorbingTypeAbility3)
	{
		if (!WillFaintFromEntryHazards(mon, side)) //Theres a point to switching in this mon
		{
			u8 foe = FOE(gActiveBattler);

			//Check HP Absorption Switch
			if (IS_SINGLE_BATTLE //Still good to do these kinds of switches in Doubles
			&& IsHPAbsorptionAbility(monAbility) //Ability restores HP when hit
			&& mon->hp == mon->maxHP //Mon is already at max HP
			&& GetMonEntryHazardDamage(mon, side) == 0) //And won't take any damage switching in
			{
				if (gNewBS->ai.switchingCooldown[gActiveBattler]) //AI just switched out
					return FALSE; //Don't double switch when there's no point

				u32 dmg = GetFinalAIMoveDamage(predictedMove, foe, gActiveBattler, 2, NULL);

				if (dmg < gBattleMons[gActiveBattler].hp) //Move doesn't 3HKO - the mon out already isn't going to take a lot of damage from the attack
					return FALSE; //No point of switching to absorbing this attack
			}

			//We found a mon.
			if (IsHPAbsorptionAbility(GetMonAbility(GetBankPartyData(gActiveBattler)))) //The mon being switch out has a type absorption Ability
				gNewBS->ai.didTypeAbsorbSwitchToMonBefore[side] |= gBitTable[gBattlerPartyIndexes[gActiveBattler]]; //Act as if it was switched into to absorb an attack before

			gNewBS->ai.didTypeAbsorbSwitchToMonBefore[side] |= gBitTable[monId];
			gNewBS->ai.typeAbsorbSwitchingCooldown[gActiveBattler] = 2; //Allow switching again for type absorbers on the turn after the next
			gBattleStruct->switchoutIndex[side] = monId;
			EmitTwoReturnValues(1, ACTION_SWITCH, 0);
			return TRUE;
		}
	}

	return FALSE;
}

static bool8 FindMonThatAbsorbsOpponentsMove(struct Pokemon* party, u8 firstId, u8 lastId)
{
	u8 battlerIn1, battlerIn2;
	u8 foe1, foe2;
	u16 predictedMove1, predictedMove2;
	u8 absorbingTypeAbility1, absorbingTypeAbility2, absorbingTypeAbility3;

	LoadBattlersAndFoes(&battlerIn1, &battlerIn2, &foe1, &foe2);

	predictedMove1 = IsValidMovePrediction(foe1, gActiveBattler);
	predictedMove2 = IsValidMovePrediction(foe2, gActiveBattler);

	if (gNewBS->ai.typeAbsorbSwitchingCooldown[gActiveBattler])
		return FALSE; //Just switch in a mon to absorb an attack so don't allow an infinite loop

	if (STAT_STAGE(gActiveBattler, STAT_STAGE_EVASION) >= 6 + 3)
		return FALSE; //Invested in Evasion so don't switch

	if ((predictedMove1 == MOVE_NONE && predictedMove2 == MOVE_NONE) //No predicted move
	|| (SPLIT(predictedMove1) == SPLIT_STATUS && SPLIT(predictedMove2) == SPLIT_STATUS)) //Predicted move is status move
		return FALSE; //Don't bother running checks

	if (IS_SINGLE_BATTLE)
	{
		u8 foeClass = PredictBankFightingStyle(foe1);

		if (!IsClassDamager(foeClass)) //Enemy is just trying to stall out
		{
			if (gNewBS->ai.switchingCooldown[gActiveBattler]) //Just switched in
				return FALSE;

			if (AIRandom() & 1)
				return FALSE; //Only switch 50% of the time
		}

		if (!MoveWouldHitFirst(predictedMove1, foe1, gActiveBattler)) //AI goes first
		{
			if (CanKnockOut(gActiveBattler, foe1)) //And AI can knock out the foe
				return FALSE; //Just KO the opponent and don't worry about switching out
		}
		else //AI goes second
		{
			if (!CanKnockOut(foe1, gActiveBattler) //The enemy can't KO the AI first
			&&   CanKnockOut(gActiveBattler, foe1) //And the AI can KO the foe
			&&   AnyStatGreaterThan(gActiveBattler, 6 + 0)) //And the AI has any stat boost +7 or more
				return FALSE; //Just KO the opponent and don't worry about switching out if you're boosted up
		}

		if (!CanKnockOut(foe1, gActiveBattler) //The enemy can't KO the AI
		&& AnyStatGreaterThan(gActiveBattler, 6 + 1)) //AI is invested in stat boosts +8 or more
			return FALSE;

		if (IS_BEHIND_SUBSTITUTE(gActiveBattler) //AI is behind a Substitute
		&& !DamagingMoveThaCanBreakThroughSubstituteInMoveset(foe1, gActiveBattler)) //And the foe can't hit it through the Substitute
			return FALSE; //Make use of your substitute before switching

		//Check faulty previous prediction
		if (!gDisableStructs[gActiveBattler].isFirstTurn //AI mon has had at least a turn in battle so far
		&& !gDisableStructs[foe1].isFirstTurn //Player mon has had at least a turn in battle so far
		&& gNewBS->ai.previousMovePredictions[foe1][gActiveBattler] == predictedMove1 //Player was predicted to do the same thing last turn
		&& gLastUsedMoves[foe1] != predictedMove1) //But the player didn't do it
			return FALSE; //The player isn't doing what's expected
	}
	else //Double Battle
	{
		if (gNewBS->ai.switchingCooldown[gActiveBattler]) //Just switched in
			return FALSE;

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

	//Now run the actual type checks
	u8 moveType;
	if (predictedMove1 != MOVE_NONE && SPLIT(predictedMove1) != SPLIT_STATUS) //Foe 1 is going to use a damaging move
	{
		if (IS_DOUBLE_BATTLE
		&& predictedMove2 != MOVE_NONE && SPLIT(predictedMove2) != SPLIT_STATUS
		&& AIRandom() & 1)
		{
			//If both foes are going to use a damaging move, pick one at random to check
			foe1 = foe2;
			predictedMove1 = predictedMove2;
		}
	}
	else
	{
		foe1 = foe2;
		predictedMove1 = predictedMove2;
	}

	moveType = GetMoveTypeSpecial(foe1, predictedMove1);
	if (IS_MOLD_BREAKER(ABILITY(foe1), predictedMove1))
		return FALSE; //Can't absorb move if the Ability is ignored

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

	//Give priority to the mons likely to be switched into first
	u8 bestMonId = GetMostSuitableMonToSwitchIntoByParty(party);

	//Check best mon to switch into
	if (bestMonId != gBattleStruct->monToSwitchIntoId[battlerIn1]
	&&  bestMonId != gBattleStruct->monToSwitchIntoId[battlerIn2]
	&&  TypeAbosorbingSwitchAbilityCheck(&party[bestMonId], bestMonId, predictedMove1,
										 absorbingTypeAbility1, absorbingTypeAbility2, absorbingTypeAbility3))
		return TRUE;

	//Check second best mon to switch into
	u8 secondBestMonId = GetSecondMostSuitableMonToSwitchIntoByParty(party);
	if (secondBestMonId != PARTY_SIZE
	&& secondBestMonId != gBattleStruct->monToSwitchIntoId[battlerIn1]
	&& secondBestMonId != gBattleStruct->monToSwitchIntoId[battlerIn2]
	&& TypeAbosorbingSwitchAbilityCheck(&party[secondBestMonId], secondBestMonId, predictedMove1,
										 absorbingTypeAbility1, absorbingTypeAbility2, absorbingTypeAbility3))
		return TRUE;

	//Check the rest of the party
	for (u32 i = firstId; i < lastId; ++i)
	{
		u16 species = party[i].species;

		if (party[i].hp == 0
		||  species == SPECIES_NONE
		||	GetMonData(&party[i], MON_DATA_IS_EGG, 0)
		||  i == bestMonId //Handled above
		||  i == secondBestMonId //Handled above
		||	i == gBattlerPartyIndexes[battlerIn1]
		||	i == gBattlerPartyIndexes[battlerIn2]
		||	i == gBattleStruct->monToSwitchIntoId[battlerIn1]
		||	i == gBattleStruct->monToSwitchIntoId[battlerIn2])
			continue;

		if (TypeAbosorbingSwitchAbilityCheck(&party[i], i, predictedMove1, absorbingTypeAbility1, absorbingTypeAbility2, absorbingTypeAbility3))
			return TRUE;
	}

	return FALSE;
}

static bool8 SwitchToBestResistMonHelper(bool8 willPivot, u8 monId, u8 switchFlags)
{
	if (switchFlags & SWITCHING_FLAG_RESIST_ALL_MOVES //New mon resists all moves
	//&& AIRandom() % 100 < 75 //75 % chance of taking the switch
	/*&& switchFlags & SWITCHING_FLAG_KO_FOE*/) //And can KO foe
	{
		ConfirmAISwitch(monId, willPivot);
		return TRUE;
	}

	return FALSE;
}

static bool8 SwitchToBestResistMon(const struct Pokemon* party, bool8 willPivot)
{
	//Check best switching option
	u8 bestMonId = GetMostSuitableMonToSwitchIntoByParty(party);
	u8 switchFlags = GetMostSuitableMonToSwitchIntoFlags();
	if (SwitchToBestResistMonHelper(willPivot, bestMonId, switchFlags))
		return TRUE;

	//Check second best switching option
	u8 secondBestMonId = GetSecondMostSuitableMonToSwitchIntoByParty(party);
	switchFlags = GetSecondMostSuitableMonToSwitchIntoFlags();
	if (SwitchToBestResistMonHelper(willPivot, secondBestMonId, switchFlags))
		return TRUE;

	//Check the rest of the switching options
	u8 side = SIDE(gActiveBattler);
	for (u32 i = 0; i < PARTY_SIZE; ++i)
	{
		if (i == bestMonId || i == secondBestMonId) //Skip the mons already checked
			continue;

		if (SwitchToBestResistMonHelper(willPivot, i, gNewBS->ai.monIdToSwitchIntoFlags[side][i]))
			return TRUE;
	}

	return FALSE;
}

static bool8 ShouldSwitchIfNaturalCureOrRegenerator(struct Pokemon* party)
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

			//Don't switch out if you're poisoned and just going to get poisoned again on switch in
			if (gSideTimers[SIDE(gActiveBattler)].tspikesAmount > 0
			&& gBattleMons[gActiveBattler].status1 & STATUS_PSN_ANY
			&& ITEM_EFFECT(gActiveBattler) != ITEM_EFFECT_HEAVY_DUTY_BOOTS
			&& !IsOfType(gActiveBattler, TYPE_POISON)
			&& CheckGrounding(gActiveBattler))
			{
				//Check if Pokemon that will be sent out will negate Toxic Spikes
				u8 bestMonId;
				bestMonId = GetMostSuitableMonToSwitchIntoByParty(party);

				if (!IsMonOfType(&party[bestMonId], TYPE_POISON) || !CheckMonGrounding(&party[bestMonId])) //Can't negate Toxic Spikes
					return FALSE;
			}

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

	if (SwitchToBestResistMon(party, FALSE))
		return TRUE; //Mon has already been found

	struct Pokemon* mostSuitableMon = &party[GetMostSuitableMonToSwitchIntoByParty(party)]; //Mon to be switched to
	if (IS_SINGLE_BATTLE && !PredictedMoveWontDoTooMuchToMon(gActiveBattler, mostSuitableMon, foe1, GetMostSuitableMonToSwitchIntoFlags())) //Move would do a lot to switched in mon
	{
		if (PredictedMoveWontKOMon(gActiveBattler, mostSuitableMon, foe1))
		{
			if (AIRandom() & 1) //Switch only 50% of the time
				return FALSE;
		}
		else //Attack would KO the mon about to switch in
			return FALSE; //Don't sack your best mon just to save this one
	}

	gBattleStruct->switchoutIndex[SIDE(gActiveBattler)] = PARTY_SIZE;
	EmitTwoReturnValues(1, ACTION_SWITCH, 0);
	return TRUE;
}

static bool8 PassOnWishSwitchingCheck(struct Pokemon* party, u8 monId)
{
	if (party[monId].hp < party[monId].maxHP / 2
	&& !WillFaintFromEntryHazards(&party[monId], SIDE(gActiveBattler))
	&& (IS_DOUBLE_BATTLE
	 || PredictedMoveWontKOMon(gActiveBattler, &party[monId], FOE(gActiveBattler))))
	{
		gBattleStruct->switchoutIndex[SIDE(gActiveBattler)] = monId;
		EmitTwoReturnValues(1, ACTION_SWITCH, 0);
		return TRUE;
	}

	return FALSE;
}

static bool8 PassOnWish(struct Pokemon* party, u8 firstId, u8 lastId)
{
	u8 opposingBattler1, opposingBattler2;
	u8 battlerIn1, battlerIn2;
	LoadBattlersAndFoes(&battlerIn1, &battlerIn2, &opposingBattler1, &opposingBattler2);

	if (gWishFutureKnock.wishCounter[gActiveBattler])
	{
		//Don't switch if your health is less than half and you can survive an opponent's hit
		if (gBattleMons[gActiveBattler].hp < gBattleMons[gActiveBattler].maxHP / 2 //Health is less than half
		&& ((!CanKnockOut(opposingBattler1, gActiveBattler) && !(IS_DOUBLE_BATTLE && CanKnockOut(opposingBattler2, gActiveBattler))) //Both foes can't knock out
		  || HasProtectionMoveInMoveset(gActiveBattler, CHECK_REGULAR_PROTECTION | CHECK_MAT_BLOCK))) //Or you can protect during the wish
			return FALSE;

		//Prioritize the best mons to switch into first
		u8 bestMonId = GetMostSuitableMonToSwitchIntoByParty(party);

		//Check best mon to switch into
		if (bestMonId != gBattleStruct->monToSwitchIntoId[battlerIn1]
		&&  bestMonId != gBattleStruct->monToSwitchIntoId[battlerIn2]
		&&  PassOnWishSwitchingCheck(party, bestMonId))
			return TRUE;

		//Check second best mon to switch into
		u8 secondBestMonId = GetSecondMostSuitableMonToSwitchIntoByParty(party);
		if (secondBestMonId != PARTY_SIZE
		&& secondBestMonId != gBattleStruct->monToSwitchIntoId[battlerIn1]
		&& secondBestMonId != gBattleStruct->monToSwitchIntoId[battlerIn2]
		&& PassOnWishSwitchingCheck(party, secondBestMonId))
			return TRUE;

		for (u32 i = firstId; i < lastId; ++i)
		{
			if (party[i].hp == 0
			||  party[i].species == SPECIES_NONE
			||	GetMonData(&party[i], MON_DATA_IS_EGG, 0)
			||	i == gBattlerPartyIndexes[battlerIn1]
			||	i == gBattlerPartyIndexes[battlerIn2]
			||	i == gBattleStruct->monToSwitchIntoId[battlerIn1]
			||	i == gBattleStruct->monToSwitchIntoId[battlerIn2]
			||  i == bestMonId //Already checked
			||  i == secondBestMonId) //Already checked
				continue;

			if (PassOnWishSwitchingCheck(party, i))
				return TRUE;
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

	struct Pokemon* party = LoadPartyRange(gActiveBattler, &firstId, &lastId);

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
	u8 itemEffect;

	if (gStatuses3[gActiveBattler] & STATUS3_YAWN
	&& ABILITY(gActiveBattler) != ABILITY_NATURALCURE //Can switch out when asleep to cure
	&& (itemEffect = ITEM_EFFECT(gActiveBattler)) != ITEM_EFFECT_CURE_SLP //Will wake up as soon as it falls asleep
	&& itemEffect != ITEM_EFFECT_CURE_STATUS
	&& (!IsDynamaxed(gActiveBattler) || AIRandom() & 1) //50% chance to switch out if Dynamaxed
	&& gBattleMons[gActiveBattler].hp > gBattleMons[gActiveBattler].maxHP / 4 //Don't bother saving a mon with less than 25% of HP
	&& CanBePutToSleep(gActiveBattler, FALSE)) //Could have been yawned and then afflicted with another status condition
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

static bool8 ShouldSwitchWhileAsleep(struct Pokemon* party)
{
	if (gNewBS->ai.switchingCooldown[gActiveBattler]) //Just switched in
		return FALSE;

	if (IsBankAsleep(gActiveBattler)
	&& AIRandom() & 1 //50 % chance of switching out here
	&& (!IsDynamaxed(gActiveBattler) //Not Dynamaxed
	 || (s8) (gBattleMons[gActiveBattler].status1 & STATUS1_SLEEP) > gNewBS->dynamaxData.timer[gActiveBattler])) //Or will wake up after the Dynamax ends
	{
		u8 ability = ABILITY(gActiveBattler);

		if (STAT_STAGE(gActiveBattler, STAT_STAGE_ATK) >= 8
		||  STAT_STAGE(gActiveBattler, STAT_STAGE_SPATK) >= 8
		||  STAT_STAGE(gActiveBattler, STAT_STAGE_SPEED) >= 8
		||  STAT_STAGE(gActiveBattler, STAT_STAGE_EVASION) >= 9)
		{
			if (AIRandom() & 1) //50 % chance of not switching out here
				return FALSE; //Lower chance of switching out if AI has some good stat boosted
		}

		if (ability == ABILITY_SHEDSKIN
		|| ability == ABILITY_EARLYBIRD
		|| (ability == ABILITY_HYDRATION && gBattleWeather & WEATHER_RAIN_ANY && gWishFutureKnock.weatherDuration != 1)
		|| (IS_DOUBLE_BATTLE && BATTLER_ALIVE(PARTNER(gActiveBattler)) && ABILITY(PARTNER(gActiveBattler)) == ABILITY_HEALER)
		|| MoveEffectInMoveset(EFFECT_SLEEP_TALK, gActiveBattler)
		|| MoveEffectInMoveset(EFFECT_SNORE, gActiveBattler)
		|| MoveEffectInMoveset(EFFECT_REST, gActiveBattler) //You probably put yourself to sleep
		|| (IsClassStall(GetBankFightingStyle(gActiveBattler)) && IsTrapped(FOE(gActiveBattler), TRUE))) //You trapped the opponent in with you
			return FALSE; //Don't switch

		if (IS_SINGLE_BATTLE)
		{
			u8 bestMonId = GetMostSuitableMonToSwitchIntoByParty(party);
			u8 switchFlags = GetMostSuitableMonToSwitchIntoFlags();

			if (party[bestMonId].condition & STATUS1_SLEEP && party[bestMonId].condition > 1)
				return FALSE; //Don't switch into a mon that's still asleep

			if (!PredictedMoveWontDoTooMuchToMon(gActiveBattler, &party[bestMonId], FOE(gActiveBattler), switchFlags)) //Move will do a lot
				return FALSE; //Could lose the new mon switched in
		}

		gBattleStruct->switchoutIndex[SIDE(gActiveBattler)] = PARTY_SIZE;
		EmitTwoReturnValues(1, ACTION_SWITCH, 0);
		return TRUE;
	}

	return FALSE;
}

static bool8 AnnoyingSecondaryDamageSwitchCheck(u8 monId, u8 switchFlags, struct Pokemon* party, bool8 urgent)
{
	u8 foe = FOE(gActiveBattler);
	u8 wantedFlags = (SWITCHING_FLAG_OUTSPEEDS | SWITCHING_FLAG_KO_FOE);
	bool8 goodToSwitch = FALSE;

	if (urgent)
		goodToSwitch = TRUE;
	else if ((switchFlags & wantedFlags) == wantedFlags //New mon will go first and KO
	&& (IS_DOUBLE_BATTLE
	 || PredictedMoveWontDoTooMuchToMon(gActiveBattler, &party[monId], foe, switchFlags))) //And won't take a lot of damage on the switch in
		goodToSwitch = TRUE;
	else if (switchFlags & (SWITCHING_FLAG_RESIST_ALL_MOVES | SWITCHING_FLAG_WALLS_FOE)) //Just walls in general
		goodToSwitch = TRUE;
	else
	{
		//New mon might die on the switch in, so determine if it's worth it to sack the mon
		if (BATTLER_ALIVE(foe) && Can2HKO(gActiveBattler, foe)) //This mon could KO a foe in a couple of turns
			goodToSwitch = TRUE;

		if (IS_DOUBLE_BATTLE)
		{
			foe = PARTNER(foe);
			if (BATTLER_ALIVE(foe) && Can2HKO(gActiveBattler, foe)) //This mon could KO a foe in a couple of turns
				goodToSwitch = TRUE;
		}
	}

	if (goodToSwitch)
	{
		gBattleStruct->switchoutIndex[SIDE(gActiveBattler)] = monId;
		EmitTwoReturnValues(1, ACTION_SWITCH, 0);
		return TRUE;
	}

	return FALSE;
}

static bool8 IsTakingAnnoyingSecondaryDamage(struct Pokemon* party)
{
	u8 ability = GetPredictedAIAbility(gActiveBattler, FOE(gActiveBattler));

	if (ability != ABILITY_MAGICGUARD
	&& !CanKillAFoe(gActiveBattler)
	&& !IsDynamaxed(gActiveBattler)
	&& AI_THINKING_STRUCT->aiFlags > AI_SCRIPT_CHECK_BAD_MOVE) //Has smart AI
	{
		bool8 trySwitch = FALSE;
		bool8 urgent = FALSE;
		
		if (gStatuses3[gActiveBattler] & STATUS3_LEECHSEED && (AIRandom() & 3) == 0) //25% chance to switch out when seeded
		{
			trySwitch = TRUE;
		}
		else if (((gBattleMons[gActiveBattler].status1 & STATUS1_SLEEP) > 1 && gBattleMons[gActiveBattler].status2 & STATUS2_NIGHTMARE)
		|| (gBattleMons[gActiveBattler].status2 & STATUS2_CURSED)
		|| ((gBattleMons[gActiveBattler].status1 & STATUS1_TOXIC_COUNTER) > 0x600 && ability != ABILITY_POISONHEAL) //Been sitting with toxic for 6 turns
		|| (gBattleMons[gActiveBattler].status1 & STATUS1_PSN_ANY && ability != ABILITY_POISONHEAL
		 && GetMonAbility(GetBankPartyData(gActiveBattler)) == ABILITY_POISONHEAL)) //Had Poison Heal but lost it
		{
			trySwitch = TRUE;
			urgent = TRUE;
		}

		if (trySwitch && !WillTakeSignificantDamageFromEntryHazards(gActiveBattler, 4)) //Don't switch out if you'll take a lot of damage on switch in
		{
			if (AnnoyingSecondaryDamageSwitchCheck(GetMostSuitableMonToSwitchIntoByParty(party), GetMostSuitableMonToSwitchIntoFlags(), party, urgent) //Try to switch to the first best mon
			|| AnnoyingSecondaryDamageSwitchCheck(GetSecondMostSuitableMonToSwitchIntoByParty(party), GetSecondMostSuitableMonToSwitchIntoFlags(), party, urgent)) //Try to switch to the second best mon
				return TRUE;
		}
	}

	return FALSE;
}

static bool8 ShouldSwitchToAvoidDeathHelper(struct Pokemon* party, u8 bankDef, u16 defMove, u8 monId, u8 switchFlags)
{
	struct Pokemon* mon = &party[monId];

	if
	//OPTION A
		((switchFlags & (SWITCHING_FLAG_OUTSPEEDS | SWITCHING_FLAG_WALLS_FOE | SWITCHING_FLAG_RESIST_ALL_MOVES) //Will either not waste the switch or can wall
		&& (AI_TypeCalc(defMove, bankDef, mon) & MOVE_RESULT_NO_EFFECT)) //Move will have no effect on switched in mon

	//OPTION B
	||  (!(gBattleTypeFlags & BATTLE_TYPE_BENJAMIN_BUTTERFREE) //Death is only a figment of the imagination in this format
		&& ((switchFlags & (SWITCHING_FLAG_WALLS_FOE | SWITCHING_FLAG_RESIST_ALL_MOVES)) //Walls foe
		 || (switchFlags & SWITCHING_FLAG_KO_FOE && switchFlags & SWITCHING_FLAG_OUTSPEEDS)) //Or can go first and KO
		&& PredictedMoveWontDoTooMuchToMon(gActiveBattler, mon, bankDef, switchFlags))) //Move will affect but not do too much damage
	{
		gBattleStruct->switchoutIndex[SIDE(gActiveBattler)] = monId;
		EmitTwoReturnValues(1, ACTION_SWITCH, 0);
		return TRUE;
	}

	return FALSE;
}

static bool8 ShouldSwitchToAvoidDeath(struct Pokemon* party)
{
	if (gNewBS->ai.switchingCooldown[gActiveBattler]) //Just switched in
		return FALSE;

	if (IS_SINGLE_BATTLE
	&& AI_THINKING_STRUCT->aiFlags > AI_SCRIPT_CHECK_BAD_MOVE) //Has smart AI
	{
		u8 bankDef = FOE(gActiveBattler);
		u16 atkMove = IsValidMovePrediction(gActiveBattler, bankDef);
		u16 defMove = IsValidMovePrediction(bankDef, gActiveBattler);

		if (gBattleMons[gActiveBattler].status1 & STATUS1_PARALYSIS
		&& gBattleMons[gActiveBattler].hp < gBattleMons[gActiveBattler].maxHP / 3)
		{
			u8 ability = ABILITY(gActiveBattler);
			if (ability != ABILITY_QUICKFEET && ability != ABILITY_NATURALCURE)
				return FALSE; //Don't switch out a paralyzed Pokemon that'll probably be KO'd when it switches back in
		}

		if ((SPECIES(gActiveBattler) == SPECIES_AEGISLASH || SPECIES(gActiveBattler) == SPECIES_AEGISLASH_BLADE) //Mon to be switched is Aegislash
		&& gDisableStructs[gActiveBattler].protectUses == 0 //And it didn't use King's Shield last
		&& defMove != MOVE_NONE //Aegislash would be hit
		&& CheckContact(defMove, bankDef) //With a contact move
		&& RealPhysicalMoveInMoveset(bankDef) //That's probably physical
		&& ABILITY(gActiveBattler) == ABILITY_STANCECHANGE
		&& MoveInMoveset(MOVE_KINGSSHIELD, gActiveBattler))
			return FALSE; //Don't switch and use King's Shield instead

		if (defMove != MOVE_NONE //Foe going to attack
		&& (atkMove == MOVE_NONE || !MoveWouldHitFirst(atkMove, gActiveBattler, bankDef)) //Attacker wouldn't go first
		&& (!IS_BEHIND_SUBSTITUTE(gActiveBattler) || !MoveBlockedBySubstitute(defMove, bankDef, gActiveBattler))
		&&  MoveKnocksOutXHits(defMove, bankDef, gActiveBattler, 1) //Foe will kill
		&& !WillTakeSignificantDamageFromEntryHazards(gActiveBattler, 3) //33% health loss
		&& (GetHealthPercentage(gActiveBattler) > 20 //Don't switch out mons that are super close to death
		 || GetMostSuitableMonToSwitchIntoScore() >= SWITCHING_SCORE_MAX) //Unless its a really good switch
		 /*|| TODO: Damager and faster than majority of living enemy team*/)
		{
			u8 bestMonId = GetMostSuitableMonToSwitchIntoByParty(party);
			u8 secondBestMonId = GetSecondMostSuitableMonToSwitchIntoByParty(party);
			if (ShouldSwitchToAvoidDeathHelper(party, bankDef, defMove, bestMonId, GetMostSuitableMonToSwitchIntoFlags()))
				return TRUE;
			else if (ShouldSwitchToAvoidDeathHelper(party, bankDef, defMove, secondBestMonId, GetSecondMostSuitableMonToSwitchIntoFlags()))
				return TRUE;

			u8 side = SIDE(gActiveBattler);
			for (u32 i = 0; i < PARTY_SIZE; ++i)
			{
				if (i == bestMonId || i == secondBestMonId)
					continue;

				if (ShouldSwitchToAvoidDeathHelper(party, bankDef, defMove, i, gNewBS->ai.monIdToSwitchIntoFlags[side][i]))
					return TRUE;
			}
		}
	}

	return FALSE;
}

static bool8 ShouldSwitchIfWonderGuard(struct Pokemon* party, u8 firstId, u8 lastId)
{
	u8 battlerIn1;
	u8 opposingBattler;
	u8 moveFlags;
	u32 i, j;
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
			if (gSpecialMoveFlags[move].gMoldBreakerMoves)
				return FALSE;

			if (SPLIT(move) != SPLIT_STATUS)
			{
				u8 atkAbility = GetAIAbility(bankAtk, bankDef, move);
				if (IsMoldBreakerAbility(atkAbility))
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
						if (HasMonToSwitchTo(bankDef))
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
							&& CanTransferItem(defSpecies, ITEM(bankDef))
							&& !(gNewBS->corrodedItems[SIDE(bankAtk)] & gBitTable[gBattlerPartyIndexes[bankAtk]])
							&& !(gNewBS->corrodedItems[SIDE(bankDef)] & gBitTable[gBattlerPartyIndexes[bankDef]]))
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
								if (BankSideHasTailwind(bankAtk))
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

	//Find a pokemon in the party that has a super effective move
	for (i = firstId; i < lastId; ++i)
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

static bool8 ShouldSwitchWhenOffensiveStatsAreLow(struct Pokemon* party)
{
	//-3 in offensive stat
	//Sweeper class
	//Only has moves of physicality
	//Can't knock out
	//Minimal damage switch
	//Add something about pivoting

	if (gNewBS->ai.switchingCooldown[gActiveBattler]) //Just switched in
		return FALSE;

	u8 class = GetBankFightingStyle(gActiveBattler);

	if (!IsDynamaxed(gActiveBattler) //Don't waste the Dynamax
	&& IsClassDamager(class) //Role is to dish out as much damage as possible
	&& (STAT_STAGE(gActiveBattler, STAT_STAGE_ATK) <= OFFENSIVE_STAT_MIN_NUM
	 || STAT_STAGE(gActiveBattler, STAT_STAGE_SPATK) <= OFFENSIVE_STAT_MIN_NUM
	 || STAT_STAGE(gActiveBattler, STAT_STAGE_ACC) <= OFFENSIVE_STAT_MIN_NUM)) //Has at least one bad offensive stat (placed here to save runtime)
	{
		u8 i;
		u8 battlerIn1, battlerIn2;
		u8 foe1, foe2;
		bool8 hasPhysicalMove, hasSpecialMove;
		u8 moveLimitations = CheckMoveLimitations(gActiveBattler, 0, 0xFF);
		LoadBattlersAndFoes(&battlerIn1, &battlerIn2, &foe1, &foe2);

		//Check physical and special moves
		for (i = 0, hasPhysicalMove = FALSE, hasSpecialMove = FALSE; i < MAX_MON_MOVES; ++i)
		{
			u16 move = GetBattleMonMove(gActiveBattler, i);
			if (move == MOVE_NONE)
				break;

			if (!(gBitTable[i] & moveLimitations) //Can use move
			&& gBattleMoves[move].power != 0) //Move actually does damage
			{
				u8 split = CalcMoveSplit(gActiveBattler, move);

				if (split == SPLIT_PHYSICAL)
				{
					if (gBattleMoves[move].effect != EFFECT_COUNTER && move != MOVE_BODYPRESS && move != MOVE_FOULPLAY)
						hasPhysicalMove = TRUE;
				}
				else if (split == SPLIT_SPECIAL)
				{
					if (gBattleMoves[move].effect != EFFECT_MIRROR_COAT)
						hasSpecialMove = TRUE;
				}
			}
		}

		if (STAT_STAGE(gActiveBattler, STAT_STAGE_ATK) <= OFFENSIVE_STAT_MIN_NUM //Attack is at -3 or less
		&& hasPhysicalMove && !hasSpecialMove) //Only has physical moves
		{
			//Cleared check
		}
		else if (STAT_STAGE(gActiveBattler, STAT_STAGE_SPATK) <= OFFENSIVE_STAT_MIN_NUM //Sp. Atk is at -3 or less
		&& hasSpecialMove && !hasPhysicalMove) //Only has special moves
		{
			//Cleared check
		}
		else if (STAT_STAGE(gActiveBattler, STAT_STAGE_ATK) <= OFFENSIVE_STAT_MIN_NUM //Attack is at -3 or less
		&& STAT_STAGE(gActiveBattler, STAT_STAGE_SPATK) <= OFFENSIVE_STAT_MIN_NUM //Sp. Atk is also at -3 or less
		&& hasPhysicalMove && hasSpecialMove) //Has both kinds of moves
		{
			//Cleared check
		}
		else if (STAT_STAGE(gActiveBattler, STAT_STAGE_ACC) <= OFFENSIVE_STAT_MIN_NUM //Accuracy is at -3 or less
		&& AllMovesInMovesetWithAccuracyLessThan(gActiveBattler, foe1, 70, TRUE) //Can't use any move with Acc 70 or more
		&& (!IS_DOUBLE_BATTLE || AllMovesInMovesetWithAccuracyLessThan(gActiveBattler, foe2, 70, TRUE)) //And can't use any move with Acc 70 or more against second foe in doubles
		&& !IsClassStall(class)) //Doesn't really matter if stall mons can hit the foe as long as they can stall out
		{
			//Cleared check
		}
		else //Didn't clear stat stage check
			return FALSE; //Switch not worth it

		bool8 willPivot = !IS_DOUBLE_BATTLE && FastPivotingMoveInMovesetThatAffects(gActiveBattler, foe1);

		if ((!CanKnockOut(foe1, gActiveBattler) && !(IS_DOUBLE_BATTLE && CanKnockOut(foe2, gActiveBattler))) //Can't knock out only (or both) foe
		|| STAT_STAGE(gActiveBattler, STAT_STAGE_ACC) <= OFFENSIVE_STAT_MIN_NUM) //Or just has really bad accuracy
		{
			u8 bestMonId = GetMostSuitableMonToSwitchIntoByParty(party);
			u8 switchFlags = GetMostSuitableMonToSwitchIntoFlags();

			if (switchFlags & (SWITCHING_FLAG_WALLS_FOE | SWITCHING_FLAG_RESIST_ALL_MOVES) //New mon can take a hit
			|| (IS_SINGLE_BATTLE && switchFlags & SWITCHING_FLAG_OUTSPEEDS && PredictedMoveWontDoTooMuchToMon(gActiveBattler, &party[bestMonId], foe1, switchFlags))) //Can take at least the next hit and follow up
			{
				ConfirmAISwitch(bestMonId, willPivot);
				return TRUE;
			}
		}
	}

	return FALSE;
}

static bool8 ShouldSaveChoiceSweeper(u8 monId, u8 switchFlags, struct Pokemon* party)
{
	u8 wantedFlags = (SWITCHING_FLAG_OUTSPEEDS | SWITCHING_FLAG_KO_FOE);

	//Check if should switch to the best mon
	if (switchFlags & SWITCHING_FLAG_FAINTS_FROM_FOE) //The mon faints from the current foe
	{
		if (!(switchFlags & wantedFlags)) //And it couldn't outspeed and KO if the current mon was sacked
		{
			SWITCH_TO_MON:
			//Switch to mon and save choice sweeper
			gBattleStruct->switchoutIndex[SIDE(gActiveBattler)] = monId; //Sack the mon"
			EmitTwoReturnValues(1, ACTION_SWITCH, 0);
			return TRUE;
		}
	}
	else if (PredictedMoveWontDoTooMuchToMon(gActiveBattler, &party[monId], FOE(gActiveBattler), switchFlags))
		goto SWITCH_TO_MON;

	return FALSE;
}

static bool8 ShouldSaveSweeperForLater(struct Pokemon* party)
{
	//The smart AI should switch out if it's trying to sweep and
	//has a better mon out back than the one already on the field.
	//This is the partner to ShouldSwitchToAvoidDeath, as that function
	//only works on mons that are slower.

	if (gNewBS->ai.switchingCooldown[gActiveBattler]) //Just switched in
		return FALSE;

	u8 ability;
	u8 foe = FOE(gActiveBattler);

	if (IS_SINGLE_BATTLE //Not good for Doubles
	&& AI_THINKING_STRUCT->aiFlags > AI_SCRIPT_CHECK_BAD_MOVE //Has smart AI
	#ifdef VAR_GAME_DIFFICULTY
	&& (gBattleTypeFlags & BATTLE_TYPE_FRONTIER || VarGet(VAR_GAME_DIFFICULTY) != OPTIONS_EASY_DIFFICULTY) //This logic is not present on easy
	#endif
	&& !IsDynamaxed(gActiveBattler) //Don't waste the Dynamax
	&& !(gBattleMons[gActiveBattler].status1 & STATUS1_FREEZE) //Better to not try to save frozen target
	&& (!(gBattleMons[gActiveBattler].status1 & STATUS1_PARALYSIS) //Better to not try to save paralyzed target
	 || ((ability = ABILITY(gActiveBattler)) == ABILITY_QUICKFEET || ability == ABILITY_GUTS)) //Unless it has Quick Feet or Guts
	&& IsClassDamager(GetBankFightingStyle(gActiveBattler)) //Role is to dish out as much damage as possible
	&& !CanKnockOut(gActiveBattler, foe) //But this powerhouse can't KO right now
	&& (!IS_BEHIND_SUBSTITUTE(gActiveBattler) || DamagingMoveThaCanBreakThroughSubstituteInMoveset(foe, gActiveBattler)) //It's not behind a Substitute
	&& !WillTakeSignificantDamageFromEntryHazards(gActiveBattler, 4) //Will take less than 25% damage on reentry
	&& !(IsTrapped(foe, TRUE) && IsTakingSecondaryDamage(foe)) //This mon isn't keeping the foe locked in taking damage
	&& 
	(
		//OPTION A:
		(CanKnockOut(foe, gActiveBattler) //Foe is likely slower and will KO
		 && !CanHealFirstToPreventKnockOut(gActiveBattler, foe)) //And the AI can't just heal to avoid the KO

		//OPTION B:
		|| (!AnyUsefulOffseniveStatIsRaised(gActiveBattler) //It isn't invested in useful buffs
		 && STAT_STAGE(gActiveBattler, STAT_STAGE_EVASION) < 6 + 3 //Including +3 Evasion
		 && !(IsTakingSecondaryDamage(foe) && IsTrapped(foe, TRUE)) //And it isn't necessary to stay in so the foe will take trap damage
		 && !OffensiveSetupMoveInMoveset(gActiveBattler, foe) //It can't set up stats either
		 && ((GetMostSuitableMonToSwitchIntoFlags() & SWITCHING_FLAG_KO_FOE) //And the new mon can KO (helps against PP stallers)
		  || ((AIRandom() & 1) && !ResistsAllMoves(foe, gActiveBattler)))) //Or it doesn't already resist all of the foe's moves (Since a mon that resists all moves will be chosen,
		                                                                 //don't get into an infinite loop if this mon already does). Do this randomly to throw off opponent.
	)) 
	{
		u16 movePrediction = IsValidMovePrediction(gActiveBattler, foe);
		if (movePrediction == MOVE_FAKEOUT || gBattleMoves[movePrediction].effect == EFFECT_PROTECT)
			return FALSE; //Use the move and then switch

		bool8 willPivot = FastPivotingMoveInMovesetThatAffects(gActiveBattler, foe); //U-Turn/Volt Switch switch on their own

		if (CanKnockOut(foe, gActiveBattler)) //Only in case where foe can KO AI mon
		{
			//Try to switch out to the best mon
			u8 bestMonId, secondBestMonId, switchFlags, secondBestSwitchFlags, wantedFlags;
			wantedFlags = (SWITCHING_FLAG_OUTSPEEDS | SWITCHING_FLAG_KO_FOE);

			//Check best mon can come in and KO the foe
			bestMonId = GetMostSuitableMonToSwitchIntoByParty(party);
			switchFlags = GetMostSuitableMonToSwitchIntoFlags();
			if ((switchFlags & wantedFlags) == wantedFlags //New mon will go first and KO
			&& PredictedMoveWontDoTooMuchToMon(gActiveBattler, &party[bestMonId], foe, switchFlags))
			{
				ConfirmAISwitch(bestMonId, willPivot);
				return TRUE;
			}

			//Check second best mon can come in and KO the foe
			secondBestMonId = GetSecondMostSuitableMonToSwitchIntoByParty(party);
			secondBestSwitchFlags = GetSecondMostSuitableMonToSwitchIntoFlags();
			if ((secondBestSwitchFlags & wantedFlags) == wantedFlags //New mon will go first and KO
			&& PredictedMoveWontDoTooMuchToMon(gActiveBattler, &party[secondBestMonId], foe, secondBestSwitchFlags))
			{
				ConfirmAISwitch(secondBestMonId, willPivot);
				return TRUE;
			}

			//Check if it's worth breaking a Choice Lock
			if (CHOICED_MOVE(gActiveBattler) != MOVE_NONE //The AI is choice locked
			&& CanBeChoiceLocked(gActiveBattler)) //And the choice lock is legit
			{
				u16 backupChoiceMove = CHOICED_MOVE(gActiveBattler); //Backup the choice move
				CHOICED_MOVE(gActiveBattler) = MOVE_NONE; //Wipe the move locked into so it can compare other moves
				s16 strongestMove = CalcStrongestMove(gActiveBattler, foe, FALSE); //Get the strongest move assuming any move could be chosen
				CHOICED_MOVE(gActiveBattler) = backupChoiceMove; //Restore the old choiced move

				if (GetFinalAIMoveDamage(strongestMove, gActiveBattler, foe, 1, NULL) >= gBattleMons[foe].hp //The strongest move could KO the current foe
				&& MoveWouldHitFirst(strongestMove, gActiveBattler, foe)) //And it would hit first against the move the foe is likely to use
				{
					//Switch the AI out becuase when it switches back in it can KO the current foe

					if (ShouldSaveChoiceSweeper(bestMonId, switchFlags, party) //Handles the emit in the function
					||  ShouldSaveChoiceSweeper(secondBestMonId, secondBestSwitchFlags, party))
						return TRUE;

					//Sack the current mon and the next mon that comes in will do clean up
				}
			}
		}

		if (SwitchToBestResistMon(party, willPivot))
			return TRUE;
	}

	return FALSE;
}

/*
In order:
70: KO Foe + Resist/Immune to All Moves + Revenge Kill + Outspeeds
62: KO Foe + Resist/Immune to All Moves + Outspeeds
56: KO Foe + Resist/Immune to All Moves + Revenge Kill
55: KO Foe + Outspeeds + Revenge Kill + Walls Foe 
53: KO Foe + Outspeeds + Revenge Kill
52: KO Foe + Outspeeds + Revenge Kill + Weak to Move
48: KO Foe + Resist/Immune to All Moves
45: KO Foe + Outspeeds
41: KO Foe + Revenge Kill + Walls Foe
39: KO Foe + Revenge Kill
38: KO Foe + Revenge Kill + Weak to Move
33: KO Foe + Walls Foe
32: Resist/Immune to All Moves + Outspeeds + Has Super-Effective Move
31: KO Foe
	Resist/Immune to All Moves + Outspeeds
30: KO Foe + Weak to Move
18: Resist/Immune to All Moves + Has Super-Effective Move
17: Resist/Immune to All Moves
17: Outspeeds + Has Super-Effective Move + Walls Foe
15: Outspeeds + Has Super-Effective Move
14: Outspeeds + Has Super-Effective Move + Weak to Move
	Outspeeds
3: Walls Foe + Has Super-Effective Move
2: Walls Foe
1: Has Super-Effective Move
*/

//Add logic about switching in a partner to resist spread move in doubles
u8 GetMostSuitableMonToSwitchInto(void)
{
	u8 firstId, lastId;
	struct Pokemon* party = LoadPartyRange(gActiveBattler, &firstId, & lastId);
	return GetMostSuitableMonToSwitchIntoByParty(party);
}

u8 GetMostSuitableMonToSwitchIntoByParty(const struct Pokemon* party)
{
	u8 battlerIn1, battlerIn2;
	u8 foe1, foe2;
	LoadBattlersAndFoes(&battlerIn1, &battlerIn2, &foe1, &foe2);

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

u8 GetSecondMostSuitableMonToSwitchInto(void)
{
	u8 firstId, lastId;
	struct Pokemon* party = LoadPartyRange(gActiveBattler, &firstId, & lastId);
	return GetSecondMostSuitableMonToSwitchIntoByParty(party);
}

u8 GetSecondMostSuitableMonToSwitchIntoByParty(const struct Pokemon* party)
{
	u8 battlerIn1, battlerIn2;
	u8 foe1, foe2;
	LoadBattlersAndFoes(&battlerIn1, &battlerIn2, &foe1, &foe2);

	CalcMostSuitableMonSwitchIfNecessary();
	u8 option2 = gNewBS->ai.bestMonIdToSwitchInto[gActiveBattler][1];

	if (option2 == PARTY_SIZE
	|| option2 == gBattleStruct->monToSwitchIntoId[battlerIn1] //Second best mon 2 has already been chosen
	|| option2 == gBattleStruct->monToSwitchIntoId[battlerIn2]
	|| GetMonData(&party[option2], MON_DATA_HP, NULL) == 0) //Second best mon is dead
		return PARTY_SIZE;

	return option2;
}

s16 GetMostSuitableMonToSwitchIntoScore(void)
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

u8 GetMostSuitableMonToSwitchIntoFlags(void)
{
	u8 battlerIn1, battlerIn2;
	u8 foe1, foe2;
	LoadBattlersAndFoes(&battlerIn1, &battlerIn2, &foe1, &foe2);

	CalcMostSuitableMonSwitchIfNecessary();
	u8 option1 = gNewBS->ai.bestMonIdToSwitchInto[gActiveBattler][0];

	if (option1 == gBattleStruct->monToSwitchIntoId[battlerIn1]
	||  option1 == gBattleStruct->monToSwitchIntoId[battlerIn2])
		return gNewBS->ai.bestMonIdToSwitchIntoFlags[gActiveBattler][1];

	return gNewBS->ai.bestMonIdToSwitchIntoFlags[gActiveBattler][0];
}

u8 GetSecondMostSuitableMonToSwitchIntoFlags(void)
{
	u8 battlerIn1, battlerIn2;
	u8 foe1, foe2;
	LoadBattlersAndFoes(&battlerIn1, &battlerIn2, &foe1, &foe2);

	CalcMostSuitableMonSwitchIfNecessary();
	u8 option2 = gNewBS->ai.bestMonIdToSwitchInto[gActiveBattler][1];

	if (option2 == gBattleStruct->monToSwitchIntoId[battlerIn1]
	||  option2 == gBattleStruct->monToSwitchIntoId[battlerIn2])
		return 0;

	return gNewBS->ai.bestMonIdToSwitchIntoFlags[gActiveBattler][1];
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
	s16 scores[PARTY_SIZE] = {0};
	u8 flags[PARTY_SIZE] = {0};
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
	/*u8 option = GetBestPartyNumberForSemiInvulnerableLockedMoveCalcs(opposingBattler, opposingBattler, TRUE);
	if (option != PARTY_SIZE)
		return option;*/

	//Find the mon who is most suitable
	bestMonId = PARTY_SIZE;
	secondBestMonId = PARTY_SIZE;
	for (i = firstId; i < lastId; ++i)
	{
		struct Pokemon* consideredMon = &party[i];

		if (consideredMon->species != SPECIES_NONE
		&& consideredMon->hp > 0
		&& !GetMonData(consideredMon, MON_DATA_IS_EGG, NULL)
		&& (!(consideredMon->condition & (STATUS1_SLEEP | STATUS1_FREEZE)) || consideredMon->condition == 1) //Not asleep or is about to wake up
		&& i != gBattlerPartyIndexes[battlerIn1]
		&& i != gBattlerPartyIndexes[battlerIn2]
		&& i != gBattleStruct->monToSwitchIntoId[battlerIn1]
		&& i != gBattleStruct->monToSwitchIntoId[battlerIn2])
		{
			u8 foes[] = {foe1, foe2};
			u8 moveLimitations = CheckMoveLimitationsFromParty(consideredMon, 0, 0xFF);
			u8 ability = GetMonAbilityAfterTrace(consideredMon, foe1);
			u32 speed = SpeedCalcMon(SIDE(gActiveBattler), consideredMon);
			bool8 isOneFoeOnField = IS_SINGLE_BATTLE || ViableMonCountFromBank(foe1) == 1;
			secondLastValidMon = lastValidMon;
			lastValidMon = i;
			canNegateToxicSpikes[i] = IsMonOfType(consideredMon, TYPE_POISON) && CheckMonGrounding(consideredMon);

			if (WillFaintFromEntryHazards(consideredMon, SIDE(gActiveBattler)))
				continue; //Don't switch in the mon if it'll faint on reentry

			struct DamageCalc damageData = {0};
			damageData.bankAtk = gActiveBattler;
			damageData.monAtk = consideredMon;
			PopulateDamageCalcStructWithBaseAttackerData(&damageData);
			s32 passiveRecovery = GetMonPassiveRecovery(consideredMon, gActiveBattler);
			u16 wishRecovery = GetWishHPRecovery(gActiveBattler, FALSE);

			for (j = 0; j < gBattlersCount / 2; ++j) //Loop through all enemies on field
			{
				foe = foes[j];

				if (BATTLER_ALIVE(foe)
				&& (j == 0 || foes[0] != foes[j])) //Don't check same opponent twice
				{
					u8 typeEffectiveness = 0;
					bool8 isWeakToMove = FALSE;
					bool8 faintsFromMove = FALSE;
					bool8 isNormalEffectiveness = 0; //Records how many moves the foe has that can do normal damage
					//bool8 isNotVeryEffective = 0; //Records how many moves the foe has that can do minimal damage

					damageData.bankDef = foe;
					PopulateDamageCalcStructWithBaseDefenderData(&damageData);

					//Check Offensive Capabilities
					if (CanKnockOutFromParty(consideredMon, foe, &damageData))
					{
						scores[i] += SWITCHING_INCREASE_KO_FOE;
						flags[i] |= SWITCHING_FLAG_KO_FOE;

						if (IsMoxieAbility(ability))
						{
							scores[i] += SWITCHING_INCREASE_REVENGE_KILL;
							flags[i] |= SWITCHING_FLAG_REVENGE_KILL;
						}
						else
						{
							for (k = 0; k < MAX_MON_MOVES; ++k)
							{
								if (gBitTable[k] & moveLimitations)
									continue;

								move = GetMonData(consideredMon, MON_DATA_MOVE1 + k, 0);

								if (gBattleMoves[move].effect == EFFECT_RAPID_SPIN //Includes Defog
								&&  gSideStatuses[SIDE(gActiveBattler)] & SIDE_STATUS_SPIKES)
								{
									if (IS_SINGLE_BATTLE) //Single Battle
										canRemoveHazards[i] = ViableMonCountFromBank(gActiveBattler) >= 2; //There's a point in removing the hazards
									else //Double Battle
										canRemoveHazards[i] = ViableMonCountFromBank(gActiveBattler) >= 3; //There's a point in removing the hazards
								}

								if (move == MOVE_FELLSTINGER)
								{
									if (MoveKnocksOutXHitsFromParty(move, consideredMon, foe, 1, &damageData))
									{
										scores[i] += SWITCHING_INCREASE_REVENGE_KILL;
										flags[i] |= SWITCHING_FLAG_REVENGE_KILL;
										break;
									}
								}
								else if (SPLIT(move) != SPLIT_STATUS
								&& PriorityCalcMon(consideredMon, move) > 0
								&& MoveKnocksOutXHitsFromParty(move, consideredMon, foe, 1, &damageData))
								{
									//Priority move that KOs
									scores[i] += SWITCHING_INCREASE_REVENGE_KILL;
									flags[i] |= SWITCHING_FLAG_REVENGE_KILL;
									break;
								}
							}
						}
					}
					else //This mon can't KO the foe
					{
						bool8 hasUsableMove = FALSE;

						for (k = 0; k < MAX_MON_MOVES; ++k)
						{
							if (gBitTable[k] & moveLimitations)
								continue;

							move = GetMonData(consideredMon, MON_DATA_MOVE1 + k, 0);
							hasUsableMove = TRUE;

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
							&& TypeCalc(move, gActiveBattler, foe, consideredMon, TRUE) & MOVE_RESULT_SUPER_EFFECTIVE)
							{
								scores[i] += SWITCHING_INCREASE_HAS_SUPER_EFFECTIVE_MOVE; //Only checked if can't KO
								flags[i] |= SWITCHING_FLAG_HAS_SUPER_EFFECTIVE_MOVE;
								break;
							}
						}

						if (!hasUsableMove)
						{
							scores[i] = -1; //Bad idea to switch to this mon
							goto CHECK_NEXT_MON;
						}
					}

					//Check Speed Capabilities
					if (speed >= SpeedCalc(foe)) //The considered mon is faster than the enemy
					{
						scores[i] += SWITCHING_INCREASE_OUTSPEEDS;
						flags[i] |= SWITCHING_FLAG_OUTSPEEDS;
					}

					//Check Defensive Capabilities
					u16 moves[MAX_MON_MOVES];
					bool8 isPriority[MAX_MON_MOVES];
					u8 numUsableMoves = 0;
					bool8 physMoveInMoveset = FALSE;
					bool8 specMoveInMoveset = FALSE;
					u8 foeMoveLimitations = CheckMoveLimitations(foe, 0, 0xFF);
					struct DamageCalc foeDamageData = {0};
					foeDamageData.bankAtk = foe;
					foeDamageData.bankDef = gActiveBattler; //For the side
					foeDamageData.monDef = consideredMon;
					PopulateDamageCalcStructWithBaseAttackerData(&foeDamageData);
					PopulateDamageCalcStructWithBaseDefenderData(&foeDamageData);

					for (k = 0; k < MAX_MON_MOVES; ++k) //Get a list of all usable moves with priority moves at the front
					{
						move = GetBattleMonMove(foe, k);

						if (move == MOVE_NONE)
							break; //End of moveset

						if (gBattleMons[foe].status2 & STATUS2_MULTIPLETURNS
						&&  MoveInMoveset(gLockedMoves[foe], foe)
						&&  move != gLockedMoves[foe])
							continue; //Skip non-locked moves

						if (!(gBitTable[k] & foeMoveLimitations))
						{
							u8 split = CalcMoveSplit(foe, move);
							if (split == SPLIT_PHYSICAL)
								physMoveInMoveset = TRUE;
							else if (split == SPLIT_SPECIAL)
								specMoveInMoveset = TRUE;
							else
								continue; //Skip status moves

							if (PriorityCalc(foe, ACTION_USE_MOVE, move) > 0) //Move has priority
							{
								//Add move at the beginning of the list
								for (u8 m = numUsableMoves; m > 0; --m)
								{
									moves[m] = moves[m - 1]; //Shift moves down
									isPriority[m] = isPriority[m - 1];
								}

								moves[0] = move;
								isPriority[0] = TRUE;
								numUsableMoves++;
							}
							else //Not priority move
							{
								//Tack move on to the end of the list
								isPriority[numUsableMoves] = FALSE;
								moves[numUsableMoves++] = move;
							}
						}
					}

					for (k = 0; k < numUsableMoves; ++k) //Check how much damage each move does
					{
						move = moves[k];

						if (isOneFoeOnField && AI_THINKING_STRUCT->aiFlags > AI_SCRIPT_CHECK_BAD_MOVE) //Only smart AI gets this calc to not slow down regular Trainers
						{
							u32 dmg = AI_CalcMonDefDmg(foe, gActiveBattler, move, consideredMon, &foeDamageData); //VERY SLOW
							
							if (dmg >= consideredMon->hp)
							{
								faintsFromMove = TRUE;

								if (flags[i] & SWITCHING_FLAG_OUTSPEEDS //This mon would normally go first
								&& isPriority[k] > 0 //But the foe has a move that KOs and has priority
								&& (gTerrainType != PSYCHIC_TERRAIN || !CheckMonGrounding(consideredMon))) //And the mon can be hit by priority
									flags[i] &= ~SWITCHING_FLAG_OUTSPEEDS; //Then remove the outspeed flag so the AI doesn't think it can outspeed

								break; //Only need 1 check for this to pass - priority moves are sorted first so a break here won't cause problems
							}

							s32 adjustedHP = MathMin(((s32) consideredMon->hp) + wishRecovery, consideredMon->maxHP); //Factor in Wish up to max HP if possible
							if (adjustedHP + passiveRecovery < (s32) (dmg * 2)) //Move could 2HKO new mon
							{
								isWeakToMove = TRUE;
							}
							else if (adjustedHP + (passiveRecovery * 2) < (s32) (dmg * 3)) //Move could 3HKO mon
							{
								++isNormalEffectiveness;
							}
							//else //Move does less than a third of HP remaining
								//++isNotVeryEffective;
						}
						else //In Doubles or regular Trainers use type matchups because too slow otherwise
						{
							typeEffectiveness = AI_TypeCalc(move, foe, consideredMon);

							if (typeEffectiveness & MOVE_RESULT_SUPER_EFFECTIVE)
							{
								isWeakToMove = TRUE;
								break; //Only need 1 check for this to pass
							}
							else if (typeEffectiveness == 0)
								++isNormalEffectiveness;
							//else
								//++isNotVeryEffective; //By default it either resists or is immune
						}
					}

					if (faintsFromMove) //At least one move KOs the considered mon
					{
						if (IS_DOUBLE_BATTLE)
							goto WEAK_TO_MOVE_DECREMENT;

						if (!(flags[i] & SWITCHING_FLAG_OUTSPEEDS)) //Mon doesn't outspeed foe
						{
							flags[i] |= SWITCHING_FLAG_FAINTS_FROM_FOE;

							if (scores[i] >= SWITCHING_DECREASE_FAINTS_FROM_FOE)
								scores[i] -= SWITCHING_DECREASE_FAINTS_FROM_FOE;
							else
								scores[i] = 0;
						}
						else if (!(flags[i] & SWITCHING_FLAG_KO_FOE)) //Mon outspeeds foe but wouldn't KO
						{
							if (scores[i] >= SWITCHING_DECREASE_FAINTS_FROM_FOE_BUT_OUTSPEEDS)
								scores[i] -= SWITCHING_DECREASE_FAINTS_FROM_FOE_BUT_OUTSPEEDS;
							else
								scores[i] = 0;
						}
						else //Outspeeds and KOs
							goto WEAK_TO_MOVE_DECREMENT;
					}
					else if (isWeakToMove) //At least one move does a good chunk of damage to the considered mon
					{
						WEAK_TO_MOVE_DECREMENT:
						if (scores[i] >= SWITCHING_DECREASE_WEAK_TO_MOVE)
							scores[i] -= SWITCHING_DECREASE_WEAK_TO_MOVE;
						else
							scores[i] = 0;
					}
					else if (!isNormalEffectiveness) //Foe has no Super-Effective or Normal-Effectiveness moves
					{
						scores[i] += SWITCHING_INCREASE_RESIST_ALL_MOVES;
						flags[i] |= SWITCHING_FLAG_RESIST_ALL_MOVES;
					}
					else
					{
						bool8 cantWall = FALSE;
						u32 attack, spAttack;
						APPLY_STAT_MOD(attack, &gBattleMons[foe], gBattleMons[foe].attack, STAT_STAGE_ATK);
						APPLY_STAT_MOD(spAttack, &gBattleMons[foe], gBattleMons[foe].spAttack, STAT_STAGE_SPATK);

						if (physMoveInMoveset && GetMonData(consideredMon, MON_DATA_DEF, NULL) <= attack)
							cantWall = TRUE;
						else if (specMoveInMoveset && GetMonData(consideredMon, MON_DATA_SPDEF, NULL) <= spAttack)
							cantWall = TRUE;

						if (!cantWall)
						{
							scores[i] += SWITCHING_INCREASE_WALLS_FOE;
							flags[i] |= SWITCHING_FLAG_WALLS_FOE;
						}
					}
				}
			}

			if (scores[i] >= SWITCHING_SCORE_MAX * (isOneFoeOnField ? 1 : 2) //Adjust when multiple foes are on the field
			&& canRemoveHazards[i]) //This mon is perfect
			{
				secondBestMonId = bestMonId; //Move down the old best mon
				bestMonId = i; //This is the best mon
			}
			else if (bestMonId == PARTY_SIZE //No best mon yet
			|| scores[i] > scores[bestMonId]
			|| (scores[i] == scores[bestMonId] && (AIRandom() % 100 < 50))) //50% chance when having similar scores
			{
				secondBestMonId = bestMonId; //If no best mon will still be PARTY_SIZE
				bestMonId = i;
			}
			else if (secondBestMonId == PARTY_SIZE //No second best mon yet
			|| scores[i] > scores[secondBestMonId] //This mon isn't the best but it's better than the current second best
			|| (scores[i] == scores[secondBestMonId] && (AIRandom() % 100 < 50))) //50% chance when having similar scores
				secondBestMonId = i;
		}

		CHECK_NEXT_MON: ;
	}

	Memcpy(gNewBS->ai.monIdToSwitchIntoScores[SIDE(gActiveBattler)], scores, sizeof(gNewBS->ai.monIdToSwitchIntoScores[SIDE(gActiveBattler)]));
	Memcpy(gNewBS->ai.monIdToSwitchIntoFlags[SIDE(gActiveBattler)], flags, sizeof(gNewBS->ai.monIdToSwitchIntoFlags[SIDE(gActiveBattler)]));

	if (bestMonId != PARTY_SIZE)
	{
		if (!(flags[bestMonId] & SWITCHING_FLAG_RESIST_ALL_MOVES) //Best mon doesn't resist all moves
		&& (flags[bestMonId] & (SWITCHING_FLAG_KO_FOE | SWITCHING_FLAG_OUTSPEEDS)) != (SWITCHING_FLAG_KO_FOE | SWITCHING_FLAG_OUTSPEEDS)) //And doesn't outspeed and KO
		{
			bool8 tSpikesActive = gSideTimers[SIDE(gActiveBattler)].tspikesAmount > 0;
			for (i = firstId; i < lastId; ++i)
			{
				if ((canRemoveHazards[i] && !(flags[i] & SWITCHING_FLAG_FAINTS_FROM_FOE)) //Mon can stay alive and remove hazards
				|| (tSpikesActive && canNegateToxicSpikes[i]))
				{
					if (IS_DOUBLE_BATTLE)
					{
						gNewBS->ai.bestMonIdToSwitchIntoScores[gActiveBattler][0] = SWITCHING_INCREASE_CAN_REMOVE_HAZARDS * 2;
						gNewBS->ai.bestMonIdToSwitchIntoFlags[gActiveBattler][0] = SWITCHING_FLAG_CAN_REMOVE_HAZARDS;

						if (!BankSideHasTwoTrainers(gActiveBattler))
						{
							gNewBS->ai.bestMonIdToSwitchIntoScores[PARTNER(gActiveBattler)][0] = SWITCHING_INCREASE_CAN_REMOVE_HAZARDS * 2;
							gNewBS->ai.bestMonIdToSwitchIntoFlags[PARTNER(gActiveBattler)][0] = SWITCHING_FLAG_CAN_REMOVE_HAZARDS;
						}
					}
					else
					{
						gNewBS->ai.bestMonIdToSwitchIntoScores[gActiveBattler][0] = SWITCHING_INCREASE_CAN_REMOVE_HAZARDS;
						gNewBS->ai.bestMonIdToSwitchIntoFlags[gActiveBattler][0] = SWITCHING_FLAG_CAN_REMOVE_HAZARDS;
					}

					gNewBS->ai.bestMonIdToSwitchInto[gActiveBattler][0] = i;
					if (!BankSideHasTwoTrainers(gActiveBattler))
						gNewBS->ai.bestMonIdToSwitchInto[PARTNER(gActiveBattler)][0] = i;

					secondBestMonId = bestMonId;
					bestMonId = i; //New best mon is mon that can remove hazard
					break;
				}
			}
		}

		gNewBS->ai.bestMonIdToSwitchInto[gActiveBattler][0] = bestMonId;
		gNewBS->ai.bestMonIdToSwitchInto[gActiveBattler][1] = secondBestMonId;
		gNewBS->ai.bestMonIdToSwitchIntoScores[gActiveBattler][0] = scores[bestMonId];
		gNewBS->ai.bestMonIdToSwitchIntoScores[gActiveBattler][1] = scores[secondBestMonId];
		gNewBS->ai.bestMonIdToSwitchIntoFlags[gActiveBattler][0] = flags[bestMonId];
		gNewBS->ai.bestMonIdToSwitchIntoFlags[gActiveBattler][1] = flags[secondBestMonId];

		if (IS_DOUBLE_BATTLE && !BankSideHasTwoTrainers(gActiveBattler)) //Store data for second mon too
		{
			gNewBS->ai.bestMonIdToSwitchInto[PARTNER(gActiveBattler)][0] = bestMonId;
			gNewBS->ai.bestMonIdToSwitchInto[PARTNER(gActiveBattler)][1] = secondBestMonId;
			gNewBS->ai.bestMonIdToSwitchIntoScores[PARTNER(gActiveBattler)][0] = scores[bestMonId];
			gNewBS->ai.bestMonIdToSwitchIntoScores[PARTNER(gActiveBattler)][1] = scores[secondBestMonId];
			gNewBS->ai.bestMonIdToSwitchIntoFlags[PARTNER(gActiveBattler)][0] = flags[bestMonId];
			gNewBS->ai.bestMonIdToSwitchIntoFlags[PARTNER(gActiveBattler)][1] = flags[secondBestMonId];
		}

		return bestMonId;
	}

	//If for some reason we weren't able to find a mon above,
	//pick the last checked available mon now.
	gNewBS->ai.bestMonIdToSwitchIntoScores[gActiveBattler][0] = 0;
	gNewBS->ai.bestMonIdToSwitchIntoFlags[gActiveBattler][0] = 0;
	gNewBS->ai.bestMonIdToSwitchInto[gActiveBattler][0] = lastValidMon;
	gNewBS->ai.bestMonIdToSwitchIntoScores[gActiveBattler][1] = 0;
	gNewBS->ai.bestMonIdToSwitchIntoFlags[gActiveBattler][1] = 0;
	gNewBS->ai.bestMonIdToSwitchInto[gActiveBattler][1] = secondLastValidMon;

	if (!BankSideHasTwoTrainers(gActiveBattler)) //Store data for second mon too
	{
		gNewBS->ai.bestMonIdToSwitchIntoScores[PARTNER(gActiveBattler)][0] = 0;
		gNewBS->ai.bestMonIdToSwitchIntoFlags[PARTNER(gActiveBattler)][0] = 0;
		gNewBS->ai.bestMonIdToSwitchInto[PARTNER(gActiveBattler)][0] = lastValidMon;
		gNewBS->ai.bestMonIdToSwitchIntoScores[PARTNER(gActiveBattler)][1] = 0;
		gNewBS->ai.bestMonIdToSwitchIntoFlags[PARTNER(gActiveBattler)][1] = 0;
		gNewBS->ai.bestMonIdToSwitchInto[PARTNER(gActiveBattler)][1] = secondLastValidMon;
	}

	return lastValidMon; //If lastValidMon is still PARTY_SIZE when reaching here, there is a bigger problem
}

static void ResetBestMonToSwitchIntoData(u8 bank)
{
	gNewBS->ai.bestMonIdToSwitchIntoScores[bank][0] = 0;
	gNewBS->ai.bestMonIdToSwitchIntoFlags[bank][0] = 0;
	gNewBS->ai.bestMonIdToSwitchInto[bank][0] = PARTY_SIZE;
	gNewBS->ai.bestMonIdToSwitchIntoScores[bank][1] = 0;
	gNewBS->ai.bestMonIdToSwitchIntoFlags[bank][1] = 0;
	gNewBS->ai.bestMonIdToSwitchInto[bank][1] = PARTY_SIZE;
	Memset(gNewBS->ai.monIdToSwitchIntoScores[SIDE(bank)], 0, sizeof(gNewBS->ai.monIdToSwitchIntoScores[SIDE(bank)]));
	Memset(gNewBS->ai.monIdToSwitchIntoFlags[SIDE(bank)], 0, sizeof(gNewBS->ai.monIdToSwitchIntoFlags[SIDE(bank)]));
}

void ResetBestMonToSwitchInto(u8 bank)
{
	ResetBestMonToSwitchIntoData(bank);

	if (!BankSideHasTwoTrainers(bank)) //Wipe data for second mon too
		ResetBestMonToSwitchIntoData(PARTNER(bank));
}

static void RemoveBestMonToSwitchIntoData(u8 bank)
{
	//secondBestMonId -> bestMonId
	gNewBS->ai.bestMonIdToSwitchIntoScores[bank][0] = gNewBS->ai.bestMonIdToSwitchIntoScores[bank][1];
	gNewBS->ai.bestMonIdToSwitchIntoFlags[bank][0] = gNewBS->ai.bestMonIdToSwitchIntoFlags[bank][1];
	gNewBS->ai.bestMonIdToSwitchInto[bank][0] = gNewBS->ai.bestMonIdToSwitchInto[bank][1];

	gNewBS->ai.bestMonIdToSwitchIntoScores[bank][1] = 0;
	gNewBS->ai.bestMonIdToSwitchIntoFlags[bank][1] = 0;
	gNewBS->ai.bestMonIdToSwitchInto[bank][1] = PARTY_SIZE;
}

void RemoveBestMonToSwitchInto(u8 bank)
{
	RemoveBestMonToSwitchIntoData(bank);

	if (!BankSideHasTwoTrainers(bank)) //Wipe data for second mon too
		RemoveBestMonToSwitchIntoData(PARTNER(bank));
}

u32 WildMonIsSmart(u8 bank)
{
	#ifdef FLAG_SMART_WILD
	if (FlagGet(FLAG_SMART_WILD))
		return AI_SCRIPT_CHECK_BAD_MOVE | AI_SCRIPT_SEMI_SMART;
	#endif

	u16 species = SPECIES(bank);
	if (IsMegaSpecies(species)
	|| IsRedPrimalSpecies(species)
	|| IsBluePrimalSpecies(species)
	|| IsUltraNecrozmaSpecies(species)
	#ifdef SPECIES_HOOPA_UNBOUND
	|| species == SPECIES_HOOPA_UNBOUND
	#endif
	)
		return AI_SCRIPT_CHECK_BAD_MOVE | AI_SCRIPT_SEMI_SMART;

	#ifdef VAR_GAME_DIFFICULTY
	if (VarGet(VAR_GAME_DIFFICULTY) >= OPTIONS_HARD_DIFFICULTY
	&& gSpecialSpeciesFlags[species].smartWild)
		return AI_SCRIPT_CHECK_BAD_MOVE | AI_SCRIPT_SEMI_SMART;
	#endif

	if (gSpecialSpeciesFlags[species].smartWild)
	{
		if (IsRaidBattle())
			return AI_SCRIPT_CHECK_BAD_MOVE | AI_SCRIPT_SEMI_SMART;
		else
			return AI_SCRIPT_CHECK_BAD_MOVE;
	}

	#ifdef WILD_ALWAYS_SMART
		return AI_SCRIPT_CHECK_BAD_MOVE;
	#else
		return 0;
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
	data->atkSpeed = SpeedCalc(bankAtk);
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
	data->defSpeed = SpeedCalc(bankDef);
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
		&& GetBaseMoveTarget(data->partnerMove, data->bankAtkPartner) & MOVE_TARGET_SELECTED //Partner isn't using spread move
		&& CountAliveMonsInBattle(BATTLE_ALIVE_DEF_SIDE, 0, bankDef) >= 2 //With one target left, both Pokemon should aim for the same target
		&& MoveKnocksOutXHits(data->partnerMove, data->bankAtkPartner, gBattleStruct->moveTarget[data->bankAtkPartner], 1);
}

static void PredictMovesForBanks(void)
{
	int i, j;
	u8 viabilities[MAX_MON_MOVES] = {0};
	u8 bestMoves[MAX_MON_MOVES] = {0};
	struct AIScript aiScriptData = {0};

	Memcpy(gNewBS->ai.previousMovePredictions, gNewBS->ai.movePredictions, sizeof(gNewBS->ai.previousMovePredictions)); //Save old predictions
	Memset(gNewBS->ai.movePredictions, 0, sizeof(gNewBS->ai.movePredictions)); //Wipe for new round

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
						StoreMovePrediction(bankAtk, bankDef, gBattleMons[bankAtk].moves[bestMoves[AIRandom() % (j + 1)] - 1]);

					Memset(viabilities, 0, sizeof(viabilities));
				}
			}
		}
	}
}

static void UpdateStrongestMoves(void)
{
	u8 bankAtk, bankDef;

	//First update if Pokemon will be mega evolving
	for (bankAtk = 0; bankAtk < gBattlersCount; ++bankAtk)
	{
		if (!IS_TRANSFORMED(bankAtk)
		&& !BankMegaEvolved(bankAtk, FALSE)
		&&  MegaEvolutionEnabled(bankAtk)
		&& !DoesZMoveUsageStopMegaEvolution(bankAtk)
		&& ShouldPredictBankToMegaEvolve(bankAtk))
		{
			gNewBS->ai.megaPotential[bankAtk] = CanMegaEvolve(bankAtk, FALSE);

			if (gNewBS->ai.megaPotential[bankAtk] == NULL)
				gNewBS->ai.megaPotential[bankAtk] = CanMegaEvolve(bankAtk, TRUE); //Check Ultra Burst
		}
	}

	//Then calculate the damage
	for (bankAtk = 0; bankAtk < gBattlersCount; ++bankAtk)
	{
		struct BattlePokemon backupMonAtk;
		u8 backupAbilityAtk = ABILITY_NONE;
		u16 backupSpeciesAtk = SPECIES_NONE;

		gNewBS->ai.suckerPunchOkay[bankAtk] = AIRandom() & 1; //Randomly choose if turn is okay for a revealed Sucker Punch

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
		u8 i, bankAtk, bankDef;

		for (i = 0; i < gBattlersCount; ++i)
		{
			bankAtk = gBanksByTurnOrder[i]; //Calculate in order of speed so AI can processes team combos better
			u8 bankAtkPartner = PARTNER(bankAtk);

			//mgba_printf(MGBA_LOG_INFO, "");
			for (bankDef = 0; bankDef < gBattlersCount; ++bankDef)
			{
				if (bankAtk == bankDef || bankDef == bankAtkPartner || !BATTLER_ALIVE(bankDef))
					continue; //Don't bother calculating for these Pokemon. Never used

				//mgba_printf(MGBA_LOG_WARN, "");
				UpdateBestDoubleKillingMoveScore(bankAtk, bankDef, bankAtkPartner, PARTNER(bankDef), gNewBS->ai.bestDoublesKillingScores[bankAtk][bankDef], &gNewBS->ai.bestDoublesKillingMoves[bankAtk][bankDef]);
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

		if (i > 0 && validMons > 1 && validMons > (BATTLE_HISTORY->itemsNo - i) + 1) //Spread out item usage
			continue;
		item = BATTLE_HISTORY->trainerItems[i];
		itemEffects = gItemEffectTable[item - ITEM_POTION];

		if (item == ITEM_NONE || itemEffects == NULL)
			continue;

		switch (gBattleStruct->AI_itemType[gActiveBattler / 2] = GetAI_ItemType(item, itemEffects))
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
					else if (gBattleMons[gActiveBattler].hp < gBattleMons[gActiveBattler].maxHP / 2 //Smart AI should only use at less than half health
					&& !IsLaserFocused(gActiveBattler) //Don't waste these statuses with a heal turn
					&& !(gStatuses3[gActiveBattler] & STATUS3_LOCKON)
					#ifdef SPECIES_AEGISLASH_BLADE
					&& !(SPECIES(gActiveBattler) == SPECIES_AEGISLASH_BLADE && MoveInMoveset(MOVE_KINGSSHIELD, gActiveBattler)) //Should revert before having item used on it
					#endif
					#ifdef SPECIES_MINIOR_SHIELD
					&& !IsMiniorCore(SPECIES(gActiveBattler)) //Don't heal a Minior when its shields are down
					#endif
					)
					{
						u8 foe = FOE(gActiveBattler);
						if (IS_SINGLE_BATTLE)
						{
							u16 movePrediction = IsValidMovePrediction(gActiveBattler, foe);
							if (movePrediction != MOVE_NONE && gBattleMoves[movePrediction].effect == EFFECT_EXPLOSION)
								break; //Don't heal if going to commit suicide anyway

							if (BATTLER_ALIVE(foe) && ShouldRecover(gActiveBattler, foe, 0xFFFF))
							{
								if (!HasMoveThatGoesFirstAndKnocksOut(gActiveBattler, foe))
									shouldUse = TRUE;
							}
						}
						else //Doubles
						{
							//Only use recovery item if good idea against both foes
							shouldUse = TRUE;
							if (BATTLER_ALIVE(foe) && !ShouldRecover(gActiveBattler, foe, 0xFFFF))
								shouldUse = FALSE;
							else if (BATTLER_ALIVE(PARTNER(foe)) && !ShouldRecover(gActiveBattler, PARTNER(foe), 0xFFFF))
								shouldUse = FALSE;
						}
					}
				}
				break;
			case AI_ITEM_HEAL_HP:
				paramOffset = GetItemEffectParamOffset(item, 4, 4);
				if (paramOffset > 0 && BATTLER_ALIVE(gActiveBattler) && !BATTLER_MAX_HP(gActiveBattler))
				{
					if (itemEffects[paramOffset] != 0xFF //Max Potion
					&& gBattleMons[gActiveBattler].maxHP - gBattleMons[gActiveBattler].hp > itemEffects[paramOffset]) //Item won't restore all HP
						shouldUse = TRUE;
					else
						goto FULL_RESTORE_LOGIC;
				}
				break;
			case AI_ITEM_CURE_CONDITION: ;
				u32 status1 = gBattleMons[gActiveBattler].status1;
				gBattleStruct->AI_itemFlags[gActiveBattler / 2] = 0;
				if (itemEffects[3] & ITEM3_SLEEP && status1 & STATUS1_SLEEP)
				{
					gBattleStruct->AI_itemFlags[gActiveBattler / 2] |= 0x20;
					shouldUse = TRUE;
				}
				if (itemEffects[3] & ITEM3_POISON && (status1 & STATUS1_PSN_ANY))
				{
					gBattleMons[gActiveBattler].status1 = 0; //Temporarily remove status
					if (!GoodIdeaToPoisonSelf(gActiveBattler)) //Pokemon shouldn't be poisoned
					{
						gBattleStruct->AI_itemFlags[gActiveBattler / 2] |= 0x10; //So heal it
						shouldUse = TRUE;
					}
					gBattleMons[gActiveBattler].status1 = status1; //Restore from backup
				}
				if (itemEffects[3] & ITEM3_BURN && status1 & STATUS1_BURN)
				{
					gBattleMons[gActiveBattler].status1 = 0; //Temporarily remove status
					if (!GoodIdeaToBurnSelf(gActiveBattler)) //Pokemon shouldn't be burned
					{
						gBattleStruct->AI_itemFlags[gActiveBattler / 2] |= 0x8; //So heal it
						shouldUse = TRUE;
					}
					gBattleMons[gActiveBattler].status1 = status1; //Restore from backup
				}
				if (itemEffects[3] & ITEM3_FREEZE && status1 & STATUS1_FREEZE)
				{
					gBattleStruct->AI_itemFlags[gActiveBattler / 2] |= 0x4;
					shouldUse = TRUE;
				}
				if (itemEffects[3] & ITEM3_PARALYSIS && status1 & STATUS1_PARALYSIS)
				{
					gBattleMons[gActiveBattler].status1 = 0; //Temporarily remove status
					if (!GoodIdeaToParalyzeSelf(gActiveBattler)) //Pokemon shouldn't be paralyzed
					{
						gBattleStruct->AI_itemFlags[gActiveBattler / 2] |= 0x2; //So heal it
						shouldUse = TRUE;
					}
					gBattleMons[gActiveBattler].status1 = status1; //Restore from backup
				}
				if (itemEffects[3] & ITEM3_CONFUSION && gBattleMons[gActiveBattler].status2 & STATUS2_CONFUSION)
				{
					gBattleStruct->AI_itemFlags[gActiveBattler / 2] |= 0x1;
					shouldUse = TRUE;
				}
				break;
			case AI_ITEM_X_STAT:
				gBattleStruct->AI_itemFlags[gActiveBattler / 2] = 0;
				if (!gDisableStructs[gActiveBattler].isFirstTurn)
					break;
				if (itemEffects[0] & ITEM0_X_ATTACK)
					gBattleStruct->AI_itemFlags[gActiveBattler / 2] |= 0x1;
				if (itemEffects[1] & ITEM1_X_DEFEND)
					gBattleStruct->AI_itemFlags[gActiveBattler / 2] |= 0x2;
				if (itemEffects[1] & ITEM1_X_SPEED)
					gBattleStruct->AI_itemFlags[gActiveBattler / 2] |= 0x4;
				if (itemEffects[2] & ITEM2_X_SPATK)
				{
					if (item != ITEM_X_SP_DEF) //Sp. Atk
						gBattleStruct->AI_itemFlags[gActiveBattler / 2] |= 0x8;
					else //Sp. Def
						gBattleStruct->AI_itemFlags[gActiveBattler / 2] |= 0x10;
				}
				if (itemEffects[2] & ITEM2_X_ACCURACY)
					gBattleStruct->AI_itemFlags[gActiveBattler / 2] |= 0x20;
				if (itemEffects[0] & ITEM0_HIGH_CRIT)
					gBattleStruct->AI_itemFlags[gActiveBattler / 2] |= 0x80;
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
			gBattleStruct->chosenItem[gActiveBattler / 2] = item;
			BATTLE_HISTORY->trainerItems[i] = 0;
			gDisableStructs[gActiveBattler].protectUses = 0; //Reset's protect uses
			return shouldUse;
		}
	}
	return FALSE;
}

#ifdef VAR_GAME_DIFFICULTY
static bool8 IsGoodIdeaToDoShiftSwitch(u8 switchBank, u8 foe)
{
	u8 switchFlags = GetMostSuitableMonToSwitchIntoFlags();

	if (!WillTakeSignificantDamageFromEntryHazards(switchBank, 2)) //50% health loss
	{
		if (OnlyBadMovesLeftInMoveset(switchBank, foe)) //AI mon has nothing good against this new foe
			return TRUE;

		if (!CanKnockOut(switchBank, foe)) //Current mon out can't KO new mon being switched in
		{
			if (switchFlags & SWITCHING_FLAG_KO_FOE && switchFlags & (SWITCHING_FLAG_OUTSPEEDS | SWITCHING_FLAG_RESIST_ALL_MOVES))
				return TRUE;
		}

		if (SpeedCalc(switchBank) < SpeedCalc(foe) //New foe will outspeed
		&& CanKnockOut(foe, switchBank) //And KO the AI
		&& !PriorityMoveInMoveset(switchBank)) //And the AI can't use a priority move to hit before that happens
		{
			if (switchFlags & SWITCHING_FLAG_KO_FOE && switchFlags & (SWITCHING_FLAG_OUTSPEEDS | SWITCHING_FLAG_RESIST_ALL_MOVES | SWITCHING_FLAG_WALLS_FOE))
				return TRUE; //New AI mon can KO and either outspeeds or walls

			if (switchFlags & SWITCHING_FLAG_RESIST_ALL_MOVES)
				return TRUE; //New AI mon can't KO but can resist all moves
		}
	}

	return FALSE; //Don't switch
}
#endif

void ShouldDoAIShiftSwitch(void)
{
	#ifdef VAR_GAME_DIFFICULTY
	if (gBattleTypeFlags & BATTLE_TYPE_TRAINER
	&& !IS_DOUBLE_BATTLE
	&& BATTLER_ALIVE(GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT)) //AI has a mon that can be switched out
	&& gBankFainted != GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT) //Handles case of both player and AI mons fainting at same time (eg. due to Explosion)
	&& ((gBattleScripting.battleStyle == OPTIONS_BATTLE_STYLE_SHIFT && VarGet(VAR_GAME_DIFFICULTY) >= OPTIONS_HARD_DIFFICULTY)
	 || (gBattleScripting.battleStyle == OPTIONS_BATTLE_STYLE_SEMI_SHIFT && VarGet(VAR_GAME_DIFFICULTY) >= OPTIONS_EXPERT_DIFFICULTY)
	#ifdef UNBOUND
	 || GetCurrentRegionMapSectionId() == MAPSEC_SS_MARINE
	#endif
	))
	{
		gActiveBattler = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
		u8 foe = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);

		CalculateAIPredictions();
		if (IsGoodIdeaToDoShiftSwitch(gActiveBattler, foe))
		{
			ClearCachedAIData();
			return; //Continue in script
		}
		else
			gNewBS->ai.switchingCooldown[gActiveBattler] = 3; //AI just decided not to switch out
	}
	#endif

	gBattlescriptCurrInstr = BattleScript_FaintedMonChooseAnotherRejoin - 5;
}

void ClearCachedAIData(void)
{
	u32 i, j, k;

	for (i = 0; i < gBattlersCount; ++i)
	{
		gNewBS->ai.calculatedAISwitchings[i] = FALSE;
		gNewBS->recalculatedBestDoublesKillingScores[i] = FALSE;
		gNewBS->ai.fightingStyle[i] = 0xFF;
		gNewBS->ai.megaPotential[i] = NULL;
		gNewBS->ai.secondaryEffectDamage[i] = 0xFFFFFFFF;

		for (j = 0; j < gBattlersCount; ++j)
		{
			gNewBS->ai.strongestMove[i][j] = 0xFFFF;
			gNewBS->ai.strongestMoveGoesFirst[i][j] = 0xFFFF;
			gNewBS->ai.canKnockOut[i][j] = 0xFF;
			gNewBS->ai.can2HKO[i][j] = 0xFF;
			gNewBS->ai.onlyBadMovesLeft[i][j] = 0xFF;
			gNewBS->ai.shouldFreeChoiceLockWithDynamax[i][j] = FALSE;
			gNewBS->ai.dynamaxPotential[i][j] = FALSE;

			for (k = 0; k < MAX_MON_MOVES; ++k)
			{
				gNewBS->ai.damageByMove[i][j][k] = 0xFFFFFFFF;
				gNewBS->ai.moveKnocksOut1Hit[i][j][k] = 0xFF;
				gNewBS->ai.moveKnocksOut2Hits[i][j][k] = 0xFF;
				gNewBS->ai.shouldUseZMove[i][j][k] = 0xFF;
			}
		}
	}
}

//People have a tendency to cheese the AI by doing one of the following things:
//1. Leading with a deceiving Pokemon and immediately switching it out to either PP stall the opponent or lock it into a bad move.
//2. Constantly switching Pokemon out when they know they'll constantly be immune to PP stall the AI.
//Using these functions, the AI will decide on a new move if it sees this happening.

enum
{
	NOT_CHEESING,
	CHEESING,
	CHEESING_BY_SWITCHING_OFTEN,
};

void RechooseAIMoveAfterSwitchIfNecessary(void)
{
	if (SIDE(gBankSwitching) == B_SIDE_PLAYER && BATTLER_ALIVE(gBankSwitching))
	{
		u8 foe2;
		u8 foe1 = FOE(gBankSwitching);
	
		if (BATTLER_ALIVE(foe1) && gChosenActionByBank[foe1] == ACTION_USE_MOVE) //Recalc Foe 1
			TryRechooseAIMoveIfPlayerSwitchCheesed(foe1, gBankSwitching);

		if (IS_DOUBLE_BATTLE && BATTLER_ALIVE(foe2 = PARTNER(foe1)) && gChosenActionByBank[foe2] == ACTION_USE_MOVE) //Recalc Foe 2
			TryRechooseAIMoveIfPlayerSwitchCheesed(foe2, gBankSwitching);
	}
}

static void TryRechooseAIMoveIfPlayerSwitchCheesed(u8 aiBank, u8 playerBank)
{
	u8 cheeseType = IsPlayerTryingToCheeseAI(playerBank, aiBank);

	if (cheeseType != NOT_CHEESING)
	{
		bool8 allowHostileMove = (cheeseType == CHEESING) ? TRUE : FALSE; //Only allow a hostile move if player is blatantly abusing
		PickNewAIMove(aiBank, FALSE, allowHostileMove);
	}
}

static bool8 IsPlayerTryingToCheeseWithRepeatedSwitches(u8 playerBank)
{
	return gChosenActionByBank[playerBank] == ACTION_SWITCH //Player decided to switch
		&& !(gStatuses3[playerBank] & STATUS3_YAWN) //The player can always switch out freely if they're Yawned
		&& (gNewBS->ai.switchesInARow[playerBank] >= 3 //And they just sent in at the end of a turn or switched twice before (triple-switched)
		|| gNewBS->ai.secondPreviousMonIn[playerBank] == gBattlerPartyIndexes[playerBank]); //Or they double-switched between two Pokemon
}

static bool8 IsPlayerTryingToCheeseChoiceLockFirstTurn(u8 aiBank)
{
	return gBattleResults.battleTurnCounter == 0
		&& (IsChoiceItemEffectOrAbility(ITEM_EFFECT(aiBank), ABILITY(aiBank))
		|| gChosenMovesByBanks[aiBank] == MOVE_FAKEOUT);
}

extern u8 GetChanceOfPredictingPlayerNormalSwitch(void);
static bool8 ShouldPredictRandomPlayerSwitch(u8 playerBank)
{
	return gChosenActionByBank[playerBank] == ACTION_SWITCH //Player decided to switch
		&& (gBattleTypeFlags & BATTLE_TYPE_FRONTIER //In Frontier battles
		#ifdef VAR_GAME_DIFFICULTY
		 || VarGet(VAR_GAME_DIFFICULTY) >= OPTIONS_HARD_DIFFICULTY) //Or only on harder game modes
		#endif
		#ifdef UNBOUND
		&& AI_THINKING_STRUCT->aiFlags & AI_SCRIPT_CHECK_GOOD_MOVE
		&& AIRandom() % 100 < GetChanceOfPredictingPlayerNormalSwitch()
		#endif
		;
}

static u8 IsPlayerTryingToCheeseAI(u8 playerBank, u8 aiBank)
{
	if (AI_THINKING_STRUCT->aiFlags & AI_SCRIPT_CHECK_GOOD_MOVE //Very Smart AI
	&& IsPlayerInControl(playerBank)) //AI isn't in charge of player mon
	{
		if (!(gBattleTypeFlags & BATTLE_TYPE_FRONTIER) //Not fair in Frontier where player doesn't know opponent's lead
		&& VarGet(VAR_GAME_DIFFICULTY) >= OPTIONS_EXPERT_DIFFICULTY //Only on hardest game mode
		&& (IsPlayerTryingToCheeseWithRepeatedSwitches(playerBank) || IsPlayerTryingToCheeseChoiceLockFirstTurn(aiBank)))
			return CHEESING;

		if (ShouldPredictRandomPlayerSwitch(playerBank))
			return CHEESING_BY_SWITCHING_OFTEN;
	}

	return NOT_CHEESING;
}

static void PickNewAIMove(u8 aiBank, bool8 allowPursuit, bool8 allowHostileMove)
{
	u8 backupAtk = gBankAttacker;
	u8 backupDef = gBankTarget;

	gBankAttacker = gActiveBattler = aiBank;
	ClearCachedAIData(); //Need recalculation since data was for old mon
	Memset(gNewBS->ai.movePredictions, 0, sizeof(gNewBS->ai.movePredictions)); //Clear old predictions
	UpdateBestDoublesKillingMoves();

	BattleAI_SetupAIData(0xF);
	u8 chosenMovePos = BattleAI_ChooseMoveOrAction();
	if (chosenMovePos < MAX_MON_MOVES)
	{
		bool8 allow = TRUE;
		u16 chosenMove = gBattleMons[aiBank].moves[chosenMovePos];
		u8 moveTarget = GetBaseMoveTarget(chosenMove, aiBank);

		if (!allowPursuit && gBattleMoves[chosenMove].effect == EFFECT_PURSUIT)
			allow = FALSE; //Using Pursuit after a switch would make the anti-abuse obvious
		else if (!allowHostileMove && (IS_SINGLE_BATTLE || gBankTarget != PARTNER(gBankAttacker)))
		{
			//Be subtle and only allow picking a new move if it's not reliant on the foe that switched in
			if (moveTarget == MOVE_TARGET_SELECTED) //Single target move - must be ==
				allow = FALSE;
			else if (moveTarget & (MOVE_TARGET_BOTH | MOVE_TARGET_ALL | MOVE_TARGET_RANDOM) //Could hit multiple targets
			&& CountAliveMonsInBattle(BATTLE_ALIVE_DEF_SIDE, gBankAttacker, gBankTarget) <= 1) //But there's only one target to hit
				allow = FALSE; //Only one target so it's obvious this move was changed
		}

		if (allow)
		{
			gBattleStruct->chosenMovePositions[aiBank] = chosenMovePos;
			gChosenMovesByBanks[aiBank] = chosenMove;
			UpdateCurrentTargetByMoveTarget(moveTarget, aiBank);
			gBattleStruct->moveTarget[aiBank] = gBankTarget;
			gNewBS->zMoveData.toBeUsed[gActiveBattler] = ShouldAIUseZMoveByMoveAndMovePos(aiBank, gBankTarget, chosenMove, chosenMovePos);
		}
	}

	gBankAttacker = backupAtk;
	gBankTarget = backupDef;
}

static void UpdateCurrentTargetByMoveTarget(u8 moveTarget, u8 aiBank)
{
	if (moveTarget & MOVE_TARGET_USER)
	{
		gBankTarget = aiBank;
	}
	else if (moveTarget & MOVE_TARGET_USER_OR_PARTNER)
	{
		if (SIDE(gBankTarget) != SIDE(aiBank))
			gBankTarget = aiBank;
	}
	else if (moveTarget & MOVE_TARGET_BOTH)
	{
		if (SIDE(aiBank) == B_SIDE_PLAYER)
		{
			gBankTarget = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
			if (gAbsentBattlerFlags & gBitTable[gBankTarget])
				gBankTarget = GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT);
		}
		else
		{
			gBankTarget = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
			if (gAbsentBattlerFlags & gBitTable[gBankTarget])
				gBankTarget = GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT);
		}
	}
}

//If the Player used Protect to cheese AI, cheese back and change target if necessary
void TryChangeMoveTargetToCounterPlayerProtectCheese(void)
{
	#ifdef VAR_GAME_DIFFICULTY
	u8 playerBank = gBattleStruct->moveTarget[gBankAttacker];

	if (IS_DOUBLE_BATTLE
	&& !(gBattleTypeFlags & BATTLE_TYPE_FRONTIER) //Unfair in Frontier battles
	&& VarGet(VAR_GAME_DIFFICULTY) >= OPTIONS_EXPERT_DIFFICULTY //On hardest game modes
	&& AI_THINKING_STRUCT->aiFlags & AI_SCRIPT_CHECK_GOOD_MOVE //Only very smart Trainers
	&& SIDE(gBankAttacker) == B_SIDE_OPPONENT //Fake Out user is AI
	&& IsPlayerInControl(playerBank) //Protect user is player
	&& ProtectAffects(gCurrentMove, gBankAttacker, playerBank, FALSE)) //Player protected from Fake Out
	{
		if (gCurrentMove == MOVE_FAKEOUT //AI is using Fake Out
		|| (gBattleResults.battleTurnCounter == 0 && gBattleMoves[gCurrentMove].target & MOVE_TARGET_SELECTED)) //Or some other single target move and the player protected on the first turn
		{
			if (BATTLER_ALIVE(PARTNER(playerBank))
			&& !TargetFullyImmuneToCurrMove(PARTNER(playerBank))) //New target could take the hit (doesn't account for Psychic Terrain, but at this point it really doesn't matter)
				gBattleStruct->moveTarget[gBankAttacker] = PARTNER(playerBank); //Change target to partner
		}
	}
	#endif
}

//The smart AI should actually pick a good move
void PickRaidBossRepeatedMove(u8 moveLimitations)
{
	if (AI_THINKING_STRUCT->aiFlags > AI_SCRIPT_CHECK_BAD_MOVE //Has smart AI
	#ifdef VAR_GAME_DIFFICULTY
	&& VarGet(VAR_GAME_DIFFICULTY) != OPTIONS_EASY_DIFFICULTY //And the player doesn't want a challenge
	#endif
	)
	{
		PickNewAIMove(gBankAttacker, TRUE, TRUE);
	}
	else //Dumb AI picks random move
	{
		u8 curPos;

		do
		{
			curPos = gBattleStruct->chosenMovePositions[gBankAttacker] = AIRandom() & 3;
			gCurrentMove = gBattleMons[gBankAttacker].moves[curPos]; //Choose a new move
		} while (gCurrentMove == MOVE_NONE || (gBitTable[curPos] & moveLimitations));

		gBattleStruct->moveTarget[gBankAttacker] = GetMoveTarget(gCurrentMove, FALSE);
	}
}
