#include "../defines.h"
#include "../defines_battle.h"
#include "../../include/constants/items.h"
#include "../../include/constants/item_effects.h"
#include "../../include/constants/trainers.h"

#include "../../include/new/accuracy_calc.h"
#include "../../include/new/attackcanceler.h"
#include "../../include/new/ai_advanced.h"
#include "../../include/new/ai_master.h"
#include "../../include/new/ai_scripts.h"
#include "../../include/new/ai_switching.h"
#include "../../include/new/ai_util.h"
#include "../../include/new/battle_controller_opponent.h"
#include "../../include/new/battle_start_turn_start.h"
#include "../../include/new/battle_util.h"
#include "../../include/new/damage_calc.h"
#include "../../include/new/end_turn.h"
#include "../../include/new/frontier.h"
#include "../../include/new/item.h"
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
static void TryTempMegaEvolveAllBanks(struct BattlePokemon* backupBattleMons, u16* backupSpecies, u8* backupAbilities);
static void TryTempMegaEvolveBank(u8 bank, struct BattlePokemon* backupMon, u16* backupSpecies, u8* backupAbility);
static void TryRevertTempMegaEvolveAllBanks(struct BattlePokemon* backupBattleMons, u16* backupSpecies, u8* backupAbilities);
static void TryRevertTempMegaEvolveBank(u8 bank, struct BattlePokemon* backupMon, u16* backupSpecies, u8* backupAbility);
static u8 ChooseMoveOrAction_Singles(struct AIScript* aiScriptData);
static u8 ChooseMoveOrAction_Doubles(struct AIScript* aiScriptData);
static u8 ChooseTarget_Doubles(const s16* bestMovePointsForTarget, const u8* actionOrMoveIndex);
static void BattleAI_DoAIProcessing(struct AIScript* aiScriptData);
static void PredictMovesForBanks(void);
static void RunCalcShouldAIDynamax(void);
static void UpdateMegaPotential(void);
static void UpdateStrongestMoves(void);
static void UpdateBestDoublesKillingMoves(void);
static u32 GetMaxByteIndexInList(const u8 array[], const u32 size);
static bool8 ShouldAIUseItem(void);
#ifdef VAR_GAME_DIFFICULTY
static void TryRechooseAIMoveIfPlayerSwitchCheesed(u8 aiBank, u8 playerBank);
static bool8 IsPlayerTryingToCheeseWithRepeatedSwitches(u8 playerBank);
static bool8 IsPlayerTryingToCheeseAI(u8 playerBank, u8 aiBank);
#endif
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
	&& !(gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_SAFARI | BATTLE_TYPE_FRONTIER | BATTLE_TYPE_TWO_OPPONENTS | BATTLE_TYPE_EREADER_TRAINER))
	&& gTrainerBattleOpponent_A != SECRET_BASE_OPPONENT
	&& !IsFrontierTrainerId(gTrainerBattleOpponent_A)
	&& (!IsBagDisabled() //If the player's bag is disabled, the AI's should also be
	 #ifdef VAR_ITEM_RESTRICTIONS
	 || VarGet(VAR_ITEM_RESTRICTIONS) == OPTIONS_ITEM_RESTRICTIONS_AI_ONLY //Unless the player wants the AI to use items
	 #endif
	 ))
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

	moveLimitations = CheckMoveLimitations(gActiveBattler, 0, AdjustMoveLimitationFlagsForAI(gBankAttacker));

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
	struct AIScript aiScriptData = {0};

	struct BattlePokemon backupBattleMons[gBattlersCount];
	u8* backupAbilities = gNewBS->ai.backupAbilities;
	u16 backupSpecies[gBattlersCount];
	//UpdateMegaPotential(); //Mega potential already updated before switching
	TryTempMegaEvolveAllBanks(backupBattleMons, backupSpecies, backupAbilities); //Mega Evolve everyone on the field during the processing

	PopulateAIScriptStructWithBaseAttackerData(&aiScriptData, gBankAttacker); //Do this now to save time during the processing - done after Mega Evo intentionally
 
	if (IS_SINGLE_BATTLE)
	{
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

	TryRevertTempMegaEvolveAllBanks(backupBattleMons, backupSpecies, backupAbilities); //Revert all the temporary Mega Evolutions if any were done earlier

	gCurrentMove = savedCurrentMove;
	return ret;
}

static void TryTempMegaEvolveAllBanks(struct BattlePokemon* backupBattleMons, u16* backupSpecies, u8* backupAbilities)
{
	u8 bank;

	for (bank = 0; bank < gBattlersCount; ++bank)
		TryTempMegaEvolveBank(bank, &backupBattleMons[bank], &backupSpecies[bank], &backupAbilities[bank]);
}

static void TryTempMegaEvolveBank(u8 bank, struct BattlePokemon* backupMon, u16* backupSpecies, u8* backupAbility)
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

static void TryRevertTempMegaEvolveAllBanks(struct BattlePokemon* backupBattleMons, u16* backupSpecies, u8* backupAbilities)
{
	u8 bank;

	for (bank = 0; bank < gBattlersCount; ++bank)
		TryRevertTempMegaEvolveBank(bank, &backupBattleMons[bank], &backupSpecies[bank], &backupAbilities[bank]);

	//Clear when done
	Memset(backupSpecies, 0, sizeof(backupSpecies));
	Memset(backupAbilities, 0, sizeof(backupAbilities));
}

static void TryRevertTempMegaEvolveBank(u8 bank, struct BattlePokemon* backupMon, u16* backupSpecies, u8* backupAbility)
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
			if (currentMoveArray[0] == AI_THINKING_STRUCT->score[i])
			{
				currentMoveArray[numOfBestMoves] = AI_THINKING_STRUCT->score[i];
				consideredMoveArray[numOfBestMoves++] = i;
			}
			else if (currentMoveArray[0] < AI_THINKING_STRUCT->score[i])
			{
				numOfBestMoves = 1;
				currentMoveArray[0] = AI_THINKING_STRUCT->score[i];
				consideredMoveArray[0] = i;
			}
		}
	}

	return consideredMoveArray[AIRandom() % numOfBestMoves];
}

static bool8 WontHitTargetWithMove(u8 bankAtk, u8 bankDef, u16 move)
{
	u8 baseTarget;

	return BATTLER_SEMI_INVULNERABLE(bankDef) //The default target is in the air
		&& ((baseTarget = GetBaseMoveTarget(move, bankAtk)) == MOVE_TARGET_SELECTED //And this move is directed specifically at the default target
		 || (baseTarget & MOVE_TARGET_ALL && RangeMoveCanHurtPartner(move, bankAtk, PARTNER(bankAtk)))) //Or it could hurt the partner
		&& !MoveWillHit(move, bankAtk, bankDef) //And this move won't break through the semi-invlunerability
		&& MoveWouldHitFirst(move, bankAtk, bankDef); //And the move would be used before the default target is vulnerable again
}

static bool8 HasChosenToDamageTarget(u8 bankAtk, u8 bankDef)
{
	return BATTLER_ALIVE(bankAtk)
		&& !HighChanceOfBeingImmobilized(bankAtk)
		&& gChosenMovesByBanks[bankAtk] != MOVE_NONE
		&& (gBattleStruct->moveTarget[bankAtk] == bankDef
			|| GetBaseMoveTarget(GetAIChosenMove(bankAtk, bankDef), bankAtk) & MOVE_TARGET_SPREAD);
}

static bool8 PartnerWillKOTargetBeforeItCanAttack(u8 bankAtk, u8 bankDef)
{
	u8 partner = PARTNER(bankAtk);

	return HasChosenToDamageTarget(partner, bankDef)
		&& !CanKnockOutWithFasterMove(bankDef, partner, gChosenMovesByBanks[partner]) //Neither foe can kill the partner before the attack
		&& !CanKnockOutWithFasterMove(PARTNER(bankDef), partner, gChosenMovesByBanks[partner])
		&& MoveWouldHitFirst(gChosenMovesByBanks[partner], partner, bankDef); //And the partner's attack would land before the target has a chance to attack
}

static bool8 HittingTargetWithUselessStatusMove(u8 bankAtk, u8 bankDef, const u8* actionOrMoveIndex)
{
	u16 move = gBattleMons[bankAtk].moves[actionOrMoveIndex[bankDef]];

	return SIDE(bankDef) != SIDE(bankAtk) //Default target is a foe
		&& SPLIT(move) == SPLIT_STATUS //The move to be used against the default target is a status move
		&& GetBaseMoveTarget(move, bankAtk) == MOVE_TARGET_SELECTED //The move is actually targeted at this target
		&& PartnerWillKOTargetBeforeItCanAttack(bankAtk, bankDef); //But the partner will be able to KO it, wasting the status move
}

static u8 GetTargetsKnockedOut(u16 move, u8 bankAtk, u8 baseBankDef)
{
	u8 targetsKOd = 0;
	u8 atkPartner = PARTNER(bankAtk);

	if (BATTLER_ALIVE(baseBankDef))
	{
		if (MoveKnocksOutXHits(move, bankAtk, baseBankDef, 1))
			++targetsKOd;
		else if (gBattleMoves[move].effect != EFFECT_SUPER_FANG //Super Fang probably won't KO even with a partner's help
		&& HasChosenToDamageTarget(atkPartner, baseBankDef) //Check if partner will help KO this target
		&& MoveWouldHitFirst(gChosenMovesByBanks[atkPartner], atkPartner, baseBankDef)) //Damage will be done to this target before it could potentially fight back
		{
			u32 partnerDmg = GetFinalAIMoveDamage(gChosenMovesByBanks[atkPartner], atkPartner, baseBankDef, 1, NULL);

			if (partnerDmg < gBattleMons[baseBankDef].hp) //Partner won't KO on their own
			{
				u32 thisDmg = GetFinalAIMoveDamage(move, bankAtk, baseBankDef, 1, NULL);
				if (partnerDmg < gBattleMons[baseBankDef].hp //Partner won't KO on their own
				&& thisDmg + partnerDmg >= gBattleMons[baseBankDef].hp)
					++targetsKOd; //KOs target together
			}
		}
	}

	u8 defPartner = PARTNER(baseBankDef);
	if (BATTLER_ALIVE(defPartner)
	&& GetBaseMoveTarget(move, bankAtk) & MOVE_TARGET_SPREAD) //Hits multiple targets
	{
		if (MoveKnocksOutXHits(move, bankAtk, defPartner, 1))
			++targetsKOd;
		else if (gBattleMoves[move].effect != EFFECT_SUPER_FANG //Super Fang probably won't KO even with a partner's help
		&& HasChosenToDamageTarget(atkPartner, defPartner) //Check if partner will help KO this target
		&& MoveWouldHitFirst(gChosenMovesByBanks[atkPartner], atkPartner, defPartner)) //Damage will be done to this target before it could potentially fight back
		{
			u32 partnerDmg = GetFinalAIMoveDamage(gChosenMovesByBanks[atkPartner], atkPartner, defPartner, 1, NULL);

			if (partnerDmg < gBattleMons[defPartner].hp) //Partner won't KO on their own
			{
				u32 thisDmg = GetFinalAIMoveDamage(move, bankAtk, defPartner, 1, NULL);
				if (partnerDmg < gBattleMons[defPartner].hp //Partner won't KO on their own
				&& thisDmg + partnerDmg >= gBattleMons[defPartner].hp)
					++targetsKOd; //KOs target together
			}
		}
	}

	return targetsKOd;
}

static u32 GetTotalDamageForTargets(u16 move, u8 bankAtk, u8 baseBankDef)
{
	u32 damage = 0;
	u32 percentage = 0;

	if (BATTLER_ALIVE(baseBankDef))
	{
		damage = GetFinalAIMoveDamage(move, bankAtk, baseBankDef, 1, NULL);
		percentage += ((damage * 100) / gBattleMons[baseBankDef].maxHP);
	}

	u8 defPartner = PARTNER(baseBankDef);
	if (BATTLER_ALIVE(defPartner)
	&& GetBaseMoveTarget(move, bankAtk) & MOVE_TARGET_SPREAD) //Hits multiple targets
	{
		damage = GetFinalAIMoveDamage(move, bankAtk, defPartner, 1, NULL);
		percentage += ((damage * 100) / gBattleMons[baseBankDef].maxHP);
	}

	return percentage;
}

static u8 ChooseMoveOrAction_Doubles(struct AIScript* aiScriptData)
{
	u32 i, j, flags;
	u8 mostViableMovesNo;
	s16 bestMovePointsForTarget[MAX_BATTLERS_COUNT];
	u8 actionOrMoveIndex[MAX_BATTLERS_COUNT];
	u8 mostViableMovesScores[MAX_BATTLERS_COUNT];
	u8 mostViableMovesIndices[MAX_BATTLERS_COUNT];

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

	return ChooseTarget_Doubles(bestMovePointsForTarget, actionOrMoveIndex);
}

static u8 ChooseTarget_Doubles(const s16* bestMovePointsForTarget, const u8* actionOrMoveIndex)
{
	/*
	Choose best potential targets. If targets have the same score, the better target is determined by:
	1. Move that can knock out target.
		- KOing both targets then chooses foe that can KO (poses bigger threat)
	2. Move that does the most damage.
	*/
	u8 bankDef, mostViableTargetsNo;
	s16 mostMovePoints;
	s8 mostViableTargetsArray[MAX_BATTLERS_COUNT];
	bool8 usingDefaultTarget = TRUE; //Indicates whether the target has changed from 0
	bool8 tryKnockOut = ShouldPrioritizeKOingFoesDoubles(gBankAttacker);
	bool8 tryDoMostDamage = ShouldPrioritizeMostDamageDoubles(gBankAttacker);
	bool8 tryKODangerous = ShouldPrioritizeDangerousTarget(gBankAttacker);

	mostMovePoints = bestMovePointsForTarget[0]; //Start with first target
	mostViableTargetsArray[0] = 0; //Set the list of most viable targets to just the first target
	mostViableTargetsNo = 1;

	u16 firstMovePos = actionOrMoveIndex[mostViableTargetsArray[0]];
	u16 firstMove = gBattleMons[gBankAttacker].moves[firstMovePos];

	if (WontHitTargetWithMove(gBankAttacker, mostViableTargetsArray[0], firstMove)) //The default target will be semi-invulnerable and avoid the attack
	{
		//Change the default target
		mostViableTargetsArray[0] = 1;
		mostMovePoints = bestMovePointsForTarget[1];
		firstMovePos = actionOrMoveIndex[mostViableTargetsArray[0]];
		firstMove = gBattleMons[gBankAttacker].moves[firstMovePos];
	}

	u8 mostDmgTarget = mostViableTargetsArray[0]; //The bank the most damage could be done to
	u32 mostDamage = 0; //The damage AI could do to mostDmgTarget
	u8 mostDmgKnocksOut = 0; //How many targets are KOd by the move that does the most combined damage
	bool8 statusMoveOption = FALSE; //Whether or not a status move will potentially be used

	if (firstMovePos < MAX_MON_MOVES) //A move was chosen against the first target
	{
		if (SPLIT(firstMove) != SPLIT_STATUS) //Move against first target isn't status move
		{
			if (SIDE(mostDmgTarget) != SIDE(gBankAttacker)) //Don't do damage calcs for mons on the same side
			{
				//Calculate important data for later
				firstMove = TryReplaceMoveWithZMove(gBankAttacker, mostDmgTarget, firstMove);

				if (tryKnockOut)
					mostDmgKnocksOut = GetTargetsKnockedOut(firstMove, gBankAttacker, mostDmgTarget);
				if (tryDoMostDamage || tryKODangerous)
					mostDamage = GetTotalDamageForTargets(firstMove, gBankAttacker, mostDmgTarget); //Assumes damage has already been cached
			}
		}
		else
			statusMoveOption = TRUE;
	}

	for (bankDef = 1; bankDef < MAX_BATTLERS_COUNT; ++bankDef) //Start by looping through the rest of the targets
	{
		u8 movePos = actionOrMoveIndex[bankDef];
		u16 move = gBattleMons[gBankAttacker].moves[movePos];

		if (WontHitTargetWithMove(gBankAttacker, bankDef, move)) //This target will be semi-invlunerable and avoid the attack
		{
			if (!usingDefaultTarget) //Already changed targets at least once
				continue; //Don't attack this target if you won't actually be able to hit it

			//Still using the default target, so check if the default target is semi-invulnerable too
			if (!WontHitTargetWithMove(gBankAttacker, mostViableTargetsArray[0], firstMove)) //The default target isn't going to avoid the attack with semi-invlunerability
				continue; //Don't replace the default target if it's actually viable
		}

		if (bestMovePointsForTarget[bankDef] == mostMovePoints) //This target is as good to hit as the best one so far
		{
			statusMoveOption = statusMoveOption || (movePos < MAX_MON_MOVES && SPLIT(move) == SPLIT_STATUS); //Update if status move can potentially be used

			if (SIDE(bankDef) != SIDE(gBankAttacker) //Don't do damage calcs for mons on the same side
			&& movePos < MAX_MON_MOVES //Move was chosen
			&& SPLIT(move) != SPLIT_STATUS) //And this move isn't a status move
			{
				move = TryReplaceMoveWithZMove(gBankAttacker, bankDef, move);

				//Calc important details for this move if necessary
				u8 thisDmgKnocksOut = 0;
				u32 thisDamage = 0;
				if (tryKnockOut)
					thisDmgKnocksOut = GetTargetsKnockedOut(move, gBankAttacker, bankDef); //How many targets this move KOs
				if (tryDoMostDamage || tryKODangerous)
					thisDamage = GetTotalDamageForTargets(move, gBankAttacker, bankDef); //Assumes damage has already been cached

				if (!tryKnockOut) //This AI mon shouldn't prioritize KOing foes
					goto CANT_KNOCK_OUT_EITHER_TARGET;

				if (thisDmgKnocksOut < mostDmgKnocksOut)
				{
					//Using the move against this target KOs less than the move targeting the best foe would, so skip this move
					continue;
				}
				else if (thisDmgKnocksOut > mostDmgKnocksOut)
				{
					//The move used against this target KOs more, making it better than even a move that could do a higher damage number to another target
					//Fallthrough and replace the most damaging move
				}
				else if (thisDmgKnocksOut == mostDmgKnocksOut && thisDmgKnocksOut > 0 && mostDmgKnocksOut > 0) //Both KO at least one foe
				{
					//Both moves can KO the same number of targets so they're both viable

					if (tryKODangerous)
					{
						TRY_HIT_DANGEROUS_TARGET: ;
						//But the better move is the one targeting a foe that poses a bigger threat
						//Spread moves are automatically accounted for because a specific target is chosen for the spread move, and not a generic left slot
						bool8 thisFoeCanKOAI = CanKnockOut(bankDef, gBankAttacker);
						bool8 bestFoeCanKOAI = CanKnockOut(mostDmgTarget, gBankAttacker);

						if (!(AI_THINKING_STRUCT->aiFlags & AI_SCRIPT_CHECK_GOOD_MOVE))
							goto ADD_TARGET_AS_MOST_VIABLE; //Dumb and Semi-Smart AI doesn't care about foe that can KO it
						else if (!thisFoeCanKOAI && bestFoeCanKOAI)
							continue; //Prioritize foe that can KO the AI
						else if (thisFoeCanKOAI && bestFoeCanKOAI)
						{
							//Both foes can KO the AI, so both are good targets
							//But try KOing the Mega Evolved one if there is
							
							bool8 thisFoeIsMega = IsMega(bankDef) || IsDynamaxed(bankDef);
							bool8 bestFoeIsMega = IsMega(mostDmgTarget) || IsDynamaxed(mostDmgTarget);

							if (!thisFoeIsMega && bestFoeIsMega)
								continue; //Prioritize the Mega
							else if (thisFoeIsMega && bestFoeIsMega) //Both targets
								goto ADD_TARGET_AS_MOST_VIABLE; //Then pick a target to KO at random
							else if (!thisFoeIsMega && !bestFoeIsMega) //Neither target is
								goto ADD_TARGET_AS_MOST_VIABLE; //Then pick a target to KO at random
							//else if (thisFoeIsMega && !bestFoeIsMega)
								//Fallthrough and replace best damaging moves
						}
						else if (!thisFoeCanKOAI && !bestFoeCanKOAI)
							goto TRY_HIT_MOST_DAMAGE; //Neither foe can KO the AI, so both are good targets, prioritize more damage if needed
						//else if (thisFoeCanKOAI && !bestFoeCanKOAI) //Inferred
							//Fallthrough and replace best damaging moves
					}
					else
						goto ADD_TARGET_AS_MOST_VIABLE;
				}
				else //if (thisDmgKnocksOut == mostDmgKnocksOut && thisDmgKnocksOut == 0 && mostDmgKnocksOut == 0) //Inferred
				{
					CANT_KNOCK_OUT_EITHER_TARGET:
					//Neither target can be knocked out

					if (tryKODangerous && thisDamage >= 50 && mostDamage >= 50) //Both moves do 50% of max HP damage total
						goto TRY_HIT_DANGEROUS_TARGET;

					TRY_HIT_MOST_DAMAGE:
					if (tryDoMostDamage) //This doesn't apply for enemies because the AI gets too predictable when it always goes for the mon it can do the most damage to
					{
						if (thisDamage < mostDamage)
							continue; //Don't store this target if less damage can be done to it
						else if (thisDamage == mostDamage) //Both moves do the same damage to multiple targets
							goto ADD_TARGET_AS_MOST_VIABLE; //Both targets are viable
						//else
							//Try change to only target
					}
					else
						goto ADD_TARGET_AS_MOST_VIABLE;
				}

				if (!statusMoveOption) //Only replace all moves if no status moves can potentially be used
				{
					mostViableTargetsNo = 1;
					mostViableTargetsArray[0] = mostDmgTarget = bankDef;
					mostDmgKnocksOut = thisDmgKnocksOut;
					mostDamage = thisDamage;
					usingDefaultTarget = FALSE;
					continue;
				}
				else //Replace all non status moves with this best one
				{
					u32 i;
					mostDmgTarget = bankDef;
					mostDmgKnocksOut = thisDmgKnocksOut;
					mostDamage = thisDamage;

					bool8 keepTargets[MAX_BATTLERS_COUNT] = {FALSE};
					keepTargets[mostDmgTarget] = TRUE;
					for (i = 0; i < mostViableTargetsNo; ++i)
					{
						u8 target = mostViableTargetsArray[i];
						u8 movePos = actionOrMoveIndex[target]; //The move to be used against target
						u16 move = gBattleMons[gBankAttacker].moves[movePos];

						if (movePos >= MAX_MON_MOVES || SPLIT(move) == SPLIT_STATUS)
							keepTargets[target] = TRUE;
					}

					//Rebuild list
					for (i = 0, mostViableTargetsNo = 0; i < NELEMS(keepTargets); ++i)
					{
						if (keepTargets[i])
							mostViableTargetsArray[mostViableTargetsNo++] = i;
					}

					usingDefaultTarget = FALSE;
					continue;
				}
			}
			else if (SPLIT(move) == SPLIT_STATUS && move != MOVE_QUASH && PartnerWillKOTargetBeforeItCanAttack(gBankAttacker, bankDef))
			{
				if (!usingDefaultTarget //Has added new targets
				|| !HittingTargetWithUselessStatusMove(gBankAttacker, mostViableTargetsArray[0], actionOrMoveIndex)) //The only target (the default target) isn't being targeted with a useless move
					continue; //No point in overriding the default target and using a status move on this target

				//Fallthrough and add as viable move alongside the first useless move
			}

			ADD_TARGET_AS_MOST_VIABLE:
			mostViableTargetsArray[mostViableTargetsNo++] = bankDef;
			usingDefaultTarget = FALSE;
		}
		else if (bestMovePointsForTarget[bankDef] > mostMovePoints) //This target is the best one so far
		{
			if (mostMovePoints >= 0 //Chosen move has some viability - helps if initial move had negative score
			&& SPLIT(move) == SPLIT_STATUS && move != MOVE_QUASH && PartnerWillKOTargetBeforeItCanAttack(gBankAttacker, bankDef))
			{
				if (!usingDefaultTarget //Has added new targets
				|| !HittingTargetWithUselessStatusMove(gBankAttacker, mostViableTargetsArray[0], actionOrMoveIndex)) //The only target (the default target) isn't being targeted with a useless move
					continue; //No point in overriding the default target and using a status move on this target

				//Replace the initial useless move with this new useless move since it has a better score
			}

			//Make target new best and only target
			mostViableTargetsNo = 1;
			mostViableTargetsArray[0] = bankDef;
			mostMovePoints = bestMovePointsForTarget[bankDef];
			usingDefaultTarget = FALSE;

			if (!(bankDef & BIT_FLANK)) //Don't waste time if just finished dealing with the last enemy mon
			{
				if (SPLIT(move) != SPLIT_STATUS) //Move against target isn't status move
				{
					//Calculate important data for later
					statusMoveOption = FALSE;
					mostDmgTarget = bankDef; //New target
					if (tryKnockOut)
						mostDmgKnocksOut = GetTargetsKnockedOut(move, gBankAttacker, mostDmgTarget);
					if (tryDoMostDamage || tryKODangerous)
						mostDamage = GetTotalDamageForTargets(move, gBankAttacker, mostDmgTarget); //Assumes damage has already been cached
				}
				else
					statusMoveOption = TRUE;
			}
		}
	}

	gBankTarget = mostViableTargetsArray[AIRandom() % mostViableTargetsNo];
	if (gBankTarget == PARTNER(gBankAttacker) && mostMovePoints < 0)
	{
		//Never target your partner if its a bad idea to
		gBankTarget = FOE(gBankAttacker);
		if (!BATTLER_ALIVE(gBankTarget))
			gBankTarget = PARTNER(gBankTarget);
	}

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
	struct BattlePokemon backupBattleMons[gBattlersCount];
	u8* backupAbilities = gNewBS->ai.backupAbilities;
	u16 backupSpecies[gBattlersCount];

	if (RAID_BATTLE_END)
		goto DONT_THINK;

	UpdateMegaPotential();
	TryTempMegaEvolveAllBanks(backupBattleMons, backupSpecies, backupAbilities); //Mega Evolve everyone on the field during the processing

	//Calulate everything important now to save as much processing time as possible later
	CalculateAIPredictions();

	party = LoadPartyRange(gActiveBattler, &firstId, &lastId);

	if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
	{
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

		TryRevertTempMegaEvolveAllBanks(backupBattleMons, backupSpecies, backupAbilities); //Revert all the temporary Mega Evolutions if any were done earlier
		if (ret) return;
	}
	else
		TryRevertTempMegaEvolveAllBanks(backupBattleMons, backupSpecies, backupAbilities); //Revert all the temporary Mega Evolutions if any were done earlier

DONT_THINK:
	//mgba_printf(MGBA_LOG_INFO, "AI thinking complete.");
	EmitTwoReturnValues(1, ACTION_USE_MOVE, (gActiveBattler ^ BIT_SIDE) << 8);
}

void CalculateAIPredictions(void)
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

	/*data->partnerHandling = IS_DOUBLE_BATTLE
		&& BATTLER_ALIVE(data->bankAtkPartner)
		&& !IsBankIncapacitated(data->bankAtkPartner)
		&& gChosenMovesByBanks[data->bankAtkPartner] != MOVE_NONE //Partner actually selected a move
		&& gBattleStruct->moveTarget[data->bankAtkPartner] == bankDef
		&& GetBaseMoveTarget(data->partnerMove, data->bankAtkPartner) & MOVE_TARGET_SELECTED //Partner isn't using spread move
		&& CountAliveMonsInBattle(BATTLE_ALIVE_DEF_SIDE, 0, bankDef) >= 2 //With one target left, both Pokemon should aim for the same target
		&& MoveKnocksOutXHits(data->partnerMove, data->bankAtkPartner, gBattleStruct->moveTarget[data->bankAtkPartner], 1);*/
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

static void UpdateMegaPotential(void)
{
	u8 bankAtk;

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
}

static void UpdateStrongestMoves(void)
{
	u8 bankAtk, bankDef;

	for (bankAtk = 0; bankAtk < gBattlersCount; ++bankAtk)
	{
		gNewBS->ai.suckerPunchOkay[bankAtk] = AIRandom() & 1; //Randomly choose if turn is okay for a revealed Sucker Punch

		if (IS_SINGLE_BATTLE) //There's a high chance these values will be used in singles so calc now.
		{
			for (bankDef = 0; bankDef < gBattlersCount; ++bankDef)
			{
				if (bankAtk == bankDef || bankDef == PARTNER(bankAtk))
					continue; //Don't bother calculating for these Pokemon. Never used

				gNewBS->ai.strongestMove[bankAtk][bankDef] = CalcStrongestMove(bankAtk, bankDef, FALSE);
				gNewBS->ai.canKnockOut[bankAtk][bankDef] = MoveKnocksOutXHits(gNewBS->ai.strongestMove[bankAtk][bankDef], bankAtk, bankDef, 1);
				gNewBS->ai.can2HKO[bankAtk][bankDef] = (gNewBS->ai.canKnockOut[bankAtk][bankDef]) ? TRUE
													  : MoveKnocksOutXHits(gNewBS->ai.strongestMove[bankAtk][bankDef], bankAtk, bankDef, 2); //If you can KO in 1 hit you can KO in 2
			}
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

u8 GetAI_ItemType(u16 itemId, const u8 *itemEffect) //Fixed from vanilla
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
							if (movePrediction != MOVE_NONE)
							{
								u8 effect = gBattleMoves[movePrediction].effect;
								
								if (effect == EFFECT_EXPLOSION //Don't heal if going to commit suicide anyway
								||  effect == EFFECT_FAKE_OUT) //Don't heal if basically getting a free turn
									break; 
							}

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

void ClearMonCachedDamageCalcs(void)
{
	Memset(gNewBS->ai.monDamageByMove, 0xFF, sizeof(gNewBS->ai.monDamageByMove));
	Memset(gNewBS->ai.monMaxDamage, 0x0, sizeof(gNewBS->ai.monMaxDamage));
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
	#ifdef VAR_GAME_DIFFICULTY
	if (SIDE(gBankSwitching) == B_SIDE_PLAYER && BATTLER_ALIVE(gBankSwitching))
	{
		u8 foe2;
		u8 foe1 = FOE(gBankSwitching);
	
		if (BATTLER_ALIVE(foe1) && gChosenActionByBank[foe1] == ACTION_USE_MOVE) //Recalc Foe 1
			TryRechooseAIMoveIfPlayerSwitchCheesed(foe1, gBankSwitching);

		if (IS_DOUBLE_BATTLE && BATTLER_ALIVE(foe2 = PARTNER(foe1)) && gChosenActionByBank[foe2] == ACTION_USE_MOVE) //Recalc Foe 2
			TryRechooseAIMoveIfPlayerSwitchCheesed(foe2, gBankSwitching);
	}
	#endif
}

#ifdef VAR_GAME_DIFFICULTY
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
		|| (IsZCrystal(ITEM(aiBank)) && gNewBS->zMoveData.toBeUsed[aiBank])
		|| gChosenMovesByBanks[aiBank] == MOVE_FAKEOUT);
}

extern u8 GetChanceOfPredictingPlayerNormalSwitch(void);
static bool8 ShouldPredictRandomPlayerSwitch(u8 playerBank)
{
	return gChosenActionByBank[playerBank] == ACTION_SWITCH //Player decided to switch
		&& (gBattleTypeFlags & BATTLE_TYPE_FRONTIER //In Frontier battles
		 || VarGet(VAR_GAME_DIFFICULTY) >= OPTIONS_HARD_DIFFICULTY) //Or only on harder game modes
		#ifdef UNBOUND
		&& AI_THINKING_STRUCT->aiFlags & AI_SCRIPT_CHECK_GOOD_MOVE
		&& AIRandom() % 100 < GetChanceOfPredictingPlayerNormalSwitch()
		#endif
		;
}

static u8 IsPlayerTryingToCheeseAI(unusedArg u8 playerBank, unusedArg u8 aiBank)
{
	if (AI_THINKING_STRUCT->aiFlags & AI_SCRIPT_CHECK_GOOD_MOVE //Very Smart AI
	&& IsPlayerInControl(playerBank)) //AI isn't in charge of player mon
	{
		if (!(gBattleTypeFlags & BATTLE_TYPE_FRONTIER) //Not fair in Frontier where player doesn't know opponent's lead
		&& VarGet(VAR_GAME_DIFFICULTY) >= OPTIONS_EXPERT_DIFFICULTY //Only on hardest game mode
		&& (IsPlayerTryingToCheeseWithRepeatedSwitches(playerBank)
		|| IsPlayerTryingToCheeseChoiceLockFirstTurn(aiBank)))
			return CHEESING;

		if (ShouldPredictRandomPlayerSwitch(playerBank))
			return CHEESING_BY_SWITCHING_OFTEN;
	}

	return NOT_CHEESING;
}
#endif

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
	if (gBattleMons[gBankAttacker].status2 & STATUS2_MULTIPLETURNS)
	{
		//Use the same move again, but allow the target to be changed up
		gBattleStruct->moveTarget[gBankAttacker] = GetMoveTarget(gCurrentMove, FALSE);
		return;
	}

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
