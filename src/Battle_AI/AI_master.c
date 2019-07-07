#include "../defines.h"
#include "../../include/random.h"

#include "../../include/new/AI_Helper_Functions.h"
#include "../../include/new/ai_master.h"
#include "../../include/new/battle_start_turn_start.h"
#include "../../include/new/damage_calc.h"
#include "../../include/new/frontier.h"
#include "../../include/new/general_bs_commands.h"
#include "../../include/new/helper_functions.h"
#include "../../include/new/mega.h"
#include "../../include/new/move_tables.h"

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

struct SmartWildMons sSmartWildAITable[] =
{
	{SPECIES_ARTICUNO, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_ZAPDOS, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_MOLTRES, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_MEWTWO, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_MEW, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_ENTEI, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_RAIKOU, AI_SCRIPT_CHECK_BAD_MOVE},
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
	{0xFFFF, 0}
};

extern u8 AI_Script_Negatives(const u8 bankAtk, const u8 bankDef, const u16 move, const u8 originalViability);
extern u8 AI_Script_Positives(const u8 bankAtk, const u8 bankDef, const u16 move, const u8 originalViability);
extern u8 AI_Script_Roaming(const u8 bankAtk, const u8 bankDef, const u16 move, const u8 originalViability);
extern u8 AI_Script_Safari(const u8 bankAtk, const u8 bankDef, const u16 move, const u8 originalViability);
extern u8 AI_Script_FirstBattle(const u8 bankAtk, const u8 bankDef, const u16 move, const u8 originalViability);

static u8 (*const sBattleAIScriptTable[])(const u8, const u8, const u16, const u8) =
{
	[0] = AI_Script_Negatives,
	[1] = AI_Script_Positives,
	
	[29] = AI_Script_Roaming,
	[30] = AI_Script_Safari,
	[31] = AI_Script_FirstBattle,
};

//This file's functions:
static bool8 ShouldSwitch(void);
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
static bool8 ShouldSwitchIfWonderGuard(void);
static void PredictMovesForBanks(void);
static u32 GetMaxByteIndexInList(const u8 array[], const u32 size);

void BattleAI_HandleItemUseBeforeAISetup(u8 defaultScoreMoves)
{
	u32 i;
	u8 *data = (u8*)BATTLE_HISTORY;

	for (i = 0; i < sizeof(struct BattleHistory); i++)
		data[i] = 0;

	// Items are allowed to use in ONLY trainer battles.
	if ((gBattleTypeFlags & BATTLE_TYPE_TRAINER)
		&& !(gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_SAFARI | BATTLE_TYPE_FRONTIER | BATTLE_TYPE_TWO_OPPONENTS | BATTLE_TYPE_INGAME_PARTNER | BATTLE_TYPE_EREADER_TRAINER))
		&& gTrainerBattleOpponent_A != 0x400)
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

	BattleAI_SetupAIData(defaultScoreMoves);
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

	moveLimitations = CheckMoveLimitations(gActiveBattler, 0, 0xFF);

	// Ignore moves that aren't possible to use.
	for (i = 0; i < MAX_MON_MOVES; i++)
	{
		if (gBitTable[i] & moveLimitations)
			AI_THINKING_STRUCT->score[i] = 0;

		AI_THINKING_STRUCT->simulatedRNG[i] = 100 - umodsi(Random(), 16);
	}

	gBattleResources->AI_ScriptsStack->size = 0;
	gBankAttacker = gActiveBattler;

	// Decide a random target battlerId in doubles.
	if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
	{
		gBankTarget = (Random() & BIT_FLANK) + (GetBattlerSide(gActiveBattler) ^ BIT_SIDE);
		if (gAbsentBattlerFlags & gBitTable[gBankTarget])
			gBankTarget ^= BIT_FLANK;
	}
	// There's only one choice in single battles.
	else
		gBankTarget = gBankAttacker ^ BIT_SIDE;

	// Choose proper trainer ai scripts.
	AI_THINKING_STRUCT->aiFlags = GetAIFlags();

	if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
		AI_THINKING_STRUCT->aiFlags |= AI_SCRIPT_DOUBLE_BATTLE; // act smart in doubles and don't attack your partner
}

u32 GetAIFlags(void)
{
	u32 flags;

	if (gBattleTypeFlags & BATTLE_TYPE_SAFARI)
		flags = AI_SCRIPT_SAFARI;
	else if (gBattleTypeFlags & BATTLE_TYPE_ROAMER)
		flags = AI_SCRIPT_ROAMING | WildMonIsSmart(gActiveBattler);
	else if (gBattleTypeFlags & BATTLE_TYPE_OAK_TUTORIAL)
		flags = AI_SCRIPT_FIRST_BATTLE;
	else if (gBattleTypeFlags & (BATTLE_TYPE_FRONTIER))
		flags = GetAIFlagsInBattleFrontier(gActiveBattler);
	else if (gBattleTypeFlags & (BATTLE_TYPE_EREADER_TRAINER | BATTLE_TYPE_TRAINER_TOWER) && gTrainerBattleOpponent_A != 0x400)
		flags = AI_SCRIPT_CHECK_BAD_MOVE | AI_SCRIPT_CHECK_GOOD_MOVE;
	else if (gBattleTypeFlags & BATTLE_TYPE_SCRIPTED_WILD_2) //No idea how these two work
		flags = AI_SCRIPT_CHECK_BAD_MOVE;
	else if (gBattleTypeFlags & BATTLE_TYPE_SCRIPTED_WILD_3) 
		flags = AI_SCRIPT_CHECK_BAD_MOVE | AI_SCRIPT_CHECK_GOOD_MOVE;
	else if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS)
		flags = gTrainers[gTrainerBattleOpponent_A].aiFlags | gTrainers[VarGet(SECOND_OPPONENT_VAR)].aiFlags;
	else
	   flags = gTrainers[gTrainerBattleOpponent_A].aiFlags;

	return flags;
}

void BattleAI_DoAIProcessing(void)
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
																				AI_THINKING_STRUCT->score[AI_THINKING_STRUCT->movesetIndex]); //Run AI script
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

	PredictMovesForBanks();
	party = LoadPartyRange(gActiveBattler, &firstId, &lastId);

	if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
	{
		if (ShouldSwitch()) //0x8039A80
		{
			if (gBattleStruct->switchoutIndex[SIDE(gActiveBattler)] == PARTY_SIZE)
			{
				u8 monToSwitchId = GetMostSuitableMonToSwitchInto();
				if (monToSwitchId == PARTY_SIZE)
				{
					if (!(gBattleTypeFlags & BATTLE_TYPE_DOUBLE))
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
			return;
		}
		else if (ShouldUseItem()) //0x803A1F4
			return;
	}

	EmitTwoReturnValues(1, ACTION_USE_MOVE, (gActiveBattler ^ BIT_SIDE) << 8);
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
	if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
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
	if (ShouldSwitchIfPerishSong())
		return TRUE;
	if (ShouldSwitchIfWonderGuard())
		return TRUE;
	if (FindMonThatAbsorbsOpponentsMove())
		return TRUE;
	if (ShouldSwitchIfNaturalCureOrRegenerator())
	  { return TRUE; }
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
	if (CanKillAFoe(gActiveBattler))
		return FALSE;
	if (AreStatsRaised())
		return FALSE;
	if (FindMonWithFlagsAndSuperEffective(MOVE_RESULT_DOESNT_AFFECT_FOE, 1)
	||  FindMonWithFlagsAndSuperEffective(MOVE_RESULT_NOT_VERY_EFFECTIVE, 2))
		return TRUE;

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

	if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
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

	predictedMove1 = IsValidMovePrediction(foe1, gActiveBattler);
	predictedMove2 = IsValidMovePrediction(foe2, gActiveBattler);

	if ((CanKillAFoe(gActiveBattler) && (Random() & 1) != 0)
	|| ((predictedMove1 == MOVE_NONE || predictedMove1 == 0xFFFF) && (predictedMove2 == MOVE_NONE || predictedMove2 == 0xFFFF))
	|| (SPLIT(predictedMove1) == SPLIT_STATUS && SPLIT(predictedMove2) == SPLIT_STATUS))
		return FALSE;

	u8 moveType;
	if (predictedMove1 != MOVE_NONE && predictedMove1 != 0xFFFF)
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
	
	if (ABILITY(gActiveBattler) == absorbingTypeAbility1
	||  ABILITY(gActiveBattler) == absorbingTypeAbility2
	||  ABILITY(gActiveBattler) == absorbingTypeAbility3)
		return FALSE;

	party = LoadPartyRange(gActiveBattler, &firstId, &lastId);

	for (i = firstId; i < lastId; i++)
	{
		u16 species = party[i].species;
		u8 monAbility = GetPartyAbility(&party[i]);

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
	switch (ABILITY(gActiveBattler)) {
		case ABILITY_NATURALCURE:
			if (gBattleMons[gActiveBattler].status1 & (STATUS1_SLEEP | STATUS1_FREEZE))
				break;
			if (gBattleMons[gActiveBattler].hp < gBattleMons[gActiveBattler].maxHP / 2)
				return FALSE;
			break;
		//Try switch if less than half health
		case ABILITY_REGENERATOR:
			if (gBattleMons[gActiveBattler].hp > gBattleMons[gActiveBattler].maxHP / 2)
				return FALSE;
			break;
		
		default:
			return FALSE;
	}

	if (FindMonWithFlagsAndSuperEffective(MOVE_RESULT_DOESNT_AFFECT_FOE, 1))
		return TRUE;
	if (FindMonWithFlagsAndSuperEffective(MOVE_RESULT_NOT_VERY_EFFECTIVE, 1))
		return TRUE;

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

			moveFlags = TypeCalc(move, gActiveBattler, opposingBattler, 0, FALSE);
			
			if (moveFlags & MOVE_RESULT_SUPER_EFFECTIVE)
			{
				if (noRng)
					return TRUE;
				if (umodsi(Random(), 10) != 0)
					return TRUE;
			}
		}
	}

	if (!(gBattleTypeFlags & BATTLE_TYPE_DOUBLE))
		return FALSE;

	opposingBattler = PARTNER(FOE(gActiveBattler));
	if (!(gAbsentBattlerFlags & gBitTable[opposingBattler]))
	{	
		for (i = 0; i < MAX_MON_MOVES; ++i)
		{
			move = gBattleMons[gActiveBattler].moves[i];
			if (move == MOVE_NONE || SPLIT(move) == SPLIT_STATUS)
				continue;

			moveFlags = TypeCalc(move, gActiveBattler, opposingBattler, 0, FALSE);
			
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
	
	if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
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
		
	if (gWishFutureKnock->wishCounter[bank])
	{	
		//Don't switch if your health is less than half and you can survive an opponent's hit
		if (gBattleMons[bank].hp < gBattleMons[bank].maxHP / 2
		&& ((!CanKnockOut(opposingBattler1, bank) && !(gBattleTypeFlags & BATTLE_TYPE_DOUBLE && CanKnockOut(opposingBattler2, bank))) || HasProtectionMoveInMoveset(bank, FALSE)))
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

	if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
	{
		opposingBattler1 = FOE(gActiveBattler);
		opposingBattler2 = PARTNER(opposingBattler1);
	}
	else
	{
		opposingBattler1 = FOE(gActiveBattler);
		opposingBattler2 = opposingBattler1;
	}
	
	if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
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
	
	if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
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
	
	if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
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

	if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
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
		u8 newFlags = TypeCalc(gLockedMoves[bankAtk], bankAtk, gActiveBattler, NULL, FALSE);
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

	if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
	{
		opposingBattler1 = FOE(gActiveBattler);
		opposingBattler2 = PARTNER(opposingBattler1);
	}
	else
	{
		opposingBattler1 = FOE(gActiveBattler);
		opposingBattler2 = opposingBattler1;
	}
	
	if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
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
	
	if (RunAllSemiInvulnerableLockedMoveCalcs(opposingBattler1, opposingBattler2, TRUE))
		return TRUE;

	return FALSE;
}

static bool8 IsYawned(void)
{
	if (ABILITY(gActiveBattler) != ABILITY_NATURALCURE
	&& gStatuses3[gActiveBattler] & STATUS3_YAWN
	&& gBattleMons[gActiveBattler].hp > gBattleMons[gActiveBattler].maxHP / 4)
	{
		if (FindMonWithFlagsAndSuperEffective(MOVE_RESULT_DOESNT_AFFECT_FOE, 1))
			return TRUE;
		if (FindMonWithFlagsAndSuperEffective(MOVE_RESULT_NOT_VERY_EFFECTIVE, 1))
			return TRUE;

		gBattleStruct->switchoutIndex[SIDE(gActiveBattler)] = PARTY_SIZE;
		EmitTwoReturnValues(1, ACTION_SWITCH, 0);
		return TRUE;
	}
	return FALSE;
}

static bool8 IsTakingAnnoyingSecondaryDamage(void)
{
	if (ABILITY(gActiveBattler) != ABILITY_MAGICGUARD
	&& !CanKillAFoe(gActiveBattler)
	&& AI_THINKING_STRUCT->aiFlags > AI_SCRIPT_CHECK_BAD_MOVE) //Has smart AI
	{
		if (gStatuses3[gActiveBattler] & STATUS3_LEECHSEED
		|| ((gBattleMons[gActiveBattler].status1 & STATUS1_SLEEP) > 1 && gBattleMons[gActiveBattler].status2 & STATUS2_NIGHTMARE)
		||  gBattleMons[gActiveBattler].status2 & (STATUS2_CURSED))
		{
			if (FindMonWithFlagsAndSuperEffective(MOVE_RESULT_DOESNT_AFFECT_FOE, 1))
				return TRUE;
			if (FindMonWithFlagsAndSuperEffective(MOVE_RESULT_NOT_VERY_EFFECTIVE, 1))
				return TRUE;

			gBattleStruct->switchoutIndex[SIDE(gActiveBattler)] = PARTY_SIZE;
			EmitTwoReturnValues(1, ACTION_SWITCH, 0);
			return TRUE;
		}
	}

	return FALSE;
}

bool8 FindMonWithFlagsAndSuperEffective(u8 flags, u8 moduloPercent)
{
	u8 battlerIn1, battlerIn2;
	u8 foe1, foe2;
	int i, j;
	u8 start, end;
	u16 move;
	u8 moveFlags;

	if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
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
	
	if (((predictedMove1 == MOVE_NONE || predictedMove1 == 0xFFFF) && (predictedMove2 == MOVE_NONE || predictedMove2 == 0xFFFF))
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

		if (predictedMove1 != MOVE_NONE && predictedMove1 != 0xFFFF)
			moveFlags = AI_TypeCalc(predictedMove1, foe1, &party[i]);
		else
			moveFlags = AI_TypeCalc(predictedMove2, foe2, &party[i]);
		
		if (moveFlags & flags)
		{
			if (predictedMove1 != MOVE_NONE && predictedMove1 != 0xFFFF)
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
	u8 battlerIn1, battlerIn2;
	u8 opposingBattler;
	u8 moveFlags;
	int i, j;
	u8 start, end;
	opposingBattler = FOE(gActiveBattler);

	if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
		return FALSE;

	if (ABILITY(opposingBattler) != ABILITY_WONDERGUARD)
		return FALSE;

	if (ABILITY(gActiveBattler) == ABILITY_MOLDBREAKER
	||  ABILITY(gActiveBattler) == ABILITY_TERAVOLT
	||  ABILITY(gActiveBattler) == ABILITY_TURBOBLAZE)
		return FALSE;
	
	//Check Mega Evolving into a mon with Mold Breaker
	const struct Evolution* evos = CanMegaEvolve(gActiveBattler, FALSE);
	if (evos != NULL)
	{
		u16 megaSpecies = evos->targetSpecies;
		u8 megaAbility = gBaseStats[megaSpecies].ability1; //Megas can only have 1 ability
		
		if (megaAbility == ABILITY_MOLDBREAKER
		||  megaAbility == ABILITY_TERAVOLT
		||  megaAbility == ABILITY_TURBOBLAZE)
			return FALSE;
	}

	//Check if pokemon has a super effective move or Mold Breaker Move
	for (i = 0; i < MAX_MON_MOVES; ++i)
	{
		u16 move = gBattleMons[gActiveBattler].moves[i];
		if (move == MOVE_NONE)
			continue;
			
		if (CheckTableForMove(move, MoldBreakerMoves))
			return FALSE;
			
		if (SPLIT(move) == SPLIT_STATUS)
			continue;
					
		moveFlags = TypeCalc(move, gActiveBattler, opposingBattler, 0, 0);
		if (moveFlags & MOVE_RESULT_SUPER_EFFECTIVE)
			return FALSE;
	}

	if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
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
	
	pokemon_t* party = LoadPartyRange(gActiveBattler, &start, &end);
	
	// find a pokemon in the party that has a super effective move
	for (i = start; i < end; ++i)
	{
		if (party[i].hp == 0
			|| party[i].species == SPECIES_NONE
			|| GetMonData(&party[i], MON_DATA_IS_EGG, 0)
			|| i == gBattlerPartyIndexes[battlerIn1]
			|| i == gBattlerPartyIndexes[battlerIn2]
			|| i == gBattleStruct->monToSwitchIntoId[battlerIn1]
			|| i == gBattleStruct->monToSwitchIntoId[battlerIn2])
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

u8 GetMostSuitableMonToSwitchInto(void)
{
	u8 opposingBattler;
	u32 bestDmg;
	u8 bestMonId;
	u8 battlerIn1, battlerIn2;
	u8 firstId, lastId; // + 1
	pokemon_t* party;
	int i, j;
	u8 invalidMons;
	u16 move;

	if (gBattleStruct->monToSwitchIntoId[gActiveBattler] != PARTY_SIZE)
		return gBattleStruct->monToSwitchIntoId[gActiveBattler];

	if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
	{
		battlerIn1 = gActiveBattler;
		if (gAbsentBattlerFlags & gBitTable[PARTNER(gActiveBattler)])
			battlerIn2 = battlerIn1;
		else
			battlerIn2 = PARTNER(battlerIn1);
		
		opposingBattler = (Random() & BIT_FLANK) ^ SIDE(battlerIn1);
			
		if (gAbsentBattlerFlags & gBitTable[opposingBattler])
			opposingBattler ^= BIT_FLANK;
	}
	else
	{
		opposingBattler = GetBattlerAtPosition(GetBattlerPosition(gActiveBattler) ^ BIT_SIDE);
		battlerIn1 = gActiveBattler;
		battlerIn2 = gActiveBattler;
	}
	
	party = LoadPartyRange(gActiveBattler, &firstId, & lastId);
	
	//Check if point even running calcs
	u8 count = 0, potentialMon = 0;
	for (i = firstId; i < lastId; ++i)
	{
		if (party[i].species != SPECIES_NONE
		&& !GetMonData(&party[i], MON_DATA_IS_EGG, 0)
		&& party[i].hp
		&& gBattlerPartyIndexes[battlerIn1] != i
		&& gBattlerPartyIndexes[battlerIn2] != i
		&& i != gBattleStruct->monToSwitchIntoId[battlerIn1]
		&& i != gBattleStruct->monToSwitchIntoId[battlerIn2])
		{
			++count;
			potentialMon = i;
		}
	}
	
	if (count == 0)
		return PARTY_SIZE;
	else if (count == 1) //Only one poke remaining
		return potentialMon;
	
	invalidMons = 0;

	while (invalidMons != 0x3F) // All mons are invalid.
	{
		bestDmg = 0;
		bestMonId = PARTY_SIZE;
		
		//Try to counter a locked move
		u8 option = GetBestPartyNumberForSemiInvulnerableLockedMoveCalcs(opposingBattler, opposingBattler, TRUE);
		if (option != PARTY_SIZE)
			return option;

		//Find the mon whose type is the most suitable offensively.
		for (i = firstId; i < lastId; i++)
		{
			u16 species = party[i].species;
			if (species != SPECIES_NONE
				&& party[i].hp != 0
				&& !(gBitTable[i] & invalidMons)
				&& gBattlerPartyIndexes[battlerIn1] != i
				&& gBattlerPartyIndexes[battlerIn2] != i
				&& i != gBattleStruct->monToSwitchIntoId[battlerIn1]
				&& i != gBattleStruct->monToSwitchIntoId[battlerIn2])
			{
				u8 type1 = gBaseStats[species].type1;
				u8 type2 = gBaseStats[species].type2;
				u8 typeDmg = 10;
				ModulateByTypeEffectiveness(gBattleMons[opposingBattler].type1, type1, type2, &typeDmg);
				ModulateByTypeEffectiveness(gBattleMons[opposingBattler].type2, type1, type2, &typeDmg);
				ModulateByTypeEffectiveness(gBattleMons[opposingBattler].type3, type1, type2, &typeDmg);
				if (bestDmg < typeDmg)
				{
					bestDmg = typeDmg;
					bestMonId = i;
				}
			}
			else
			{
				invalidMons |= gBitTable[i];
			}
		}

		// Ok, we know the mon has the right typing but does it have at least one super effective move?
		if (bestMonId != PARTY_SIZE)
		{
			for (i = 0; i < MAX_MON_MOVES; i++)
			{
				move = GetMonData(&party[bestMonId], MON_DATA_MOVE1 + i, 0);
				if (move != MOVE_NONE 
				&& SPLIT(move) != SPLIT_STATUS 
				&& TypeCalc(move, gActiveBattler, opposingBattler, &party[bestMonId], TRUE) & MOVE_RESULT_SUPER_EFFECTIVE)
					break;
			}

			if (i != MAX_MON_MOVES)
				return bestMonId; // Has both the typing and at least one super effective move.

			invalidMons |= gBitTable[bestMonId]; // Sorry buddy, we want something better.
		}
		else
		{
			invalidMons = 0x3F; // No viable mon to switch.
		}
	}

	gMoveResultFlags = 0;
	bestDmg = 0;
	bestMonId = 6;

	// If we couldn't find the best mon in terms of typing, find the one that deals most damage.
	for (i = firstId; i < lastId; i++)
	{
		if (party[i].species == SPECIES_NONE
		|| party[i].hp == 0
		|| gBattlerPartyIndexes[battlerIn1] == i
		|| gBattlerPartyIndexes[battlerIn2] == i
		|| i == gBattleStruct->monToSwitchIntoId[battlerIn1]
		|| i == gBattleStruct->monToSwitchIntoId[battlerIn2])
			continue;

		for (j = 0; j < MAX_MON_MOVES; j++)
		{
			move = GetMonData(&party[i], MON_DATA_MOVE1 + j, 0);
			gBattleMoveDamage = 0;
			if (move != MOVE_NONE && SPLIT(move) != SPLIT_STATUS)
			{	
				gBattleMoveDamage = AI_CalcPartyDmg(gActiveBattler, opposingBattler, move, &party[i]);
				TypeCalc(move, gActiveBattler, opposingBattler, &party[i], TRUE);
			}
			if (bestDmg < (u32) gBattleMoveDamage)
			{
				bestDmg = gBattleMoveDamage;
				bestMonId = i;
			}
		}
	}

	return bestMonId;
}

u32 WildMonIsSmart(u8 bank)
{
	#ifdef WILD_ALWAYS_SMART
		bank++; //So no compiler errors
		return TRUE;
	#else
		u16 species = gBattleMons[bank].species;
		for (u32 i = 0; sSmartWildAITable[i].species != 0xFFFF; ++i) {
			if (sSmartWildAITable[i].species == species)
				return sSmartWildAITable[i].aiFlags;
		}
		return FALSE;
	#endif
}

static void PredictMovesForBanks(void)
{
	int i, j;
	u8 viabilities[MAX_MON_MOVES] = {0};
	u8 bestMoves[MAX_MON_MOVES] = {0};

	for (u8 bankAtk = 0; bankAtk < gBattlersCount; ++bankAtk)
	{
		u32 moveLimitations = CheckMoveLimitations(bankAtk, 0, 0xFF);

		for (u8 bankDef = 0; bankDef < gBattlersCount; ++bankDef)
		{
			if (bankAtk == bankDef) continue;
			
			if (gBattleMons[bankAtk].status2 & STATUS2_RECHARGE
			||  gDisableStructs[bankAtk].truantCounter != 0)
			{
				StoreMovePrediction(bankAtk, bankDef, MOVE_NONE);
			}
			else if (gBattleMons[bankAtk].status1 & STATUS1_SLEEP && gBattleMons[bankAtk].status1 > 1 //No attacking while asleep
			&& !MoveEffectInMoveset(EFFECT_SLEEP_TALK, bankAtk) && !MoveEffectInMoveset(EFFECT_SNORE, bankAtk))
			{
				StoreMovePrediction(bankAtk, bankDef, MOVE_NONE);
			}
			else if (gBattleMons[bankAtk].status2 & STATUS2_MULTIPLETURNS
			&& FindMovePositionInMoveset(gLockedMoves[bankAtk], bankAtk) < 4)
			{
				StoreMovePrediction(bankAtk, bankDef, gLockedMoves[bankAtk]);
			}
			else
			{	
				u32 backupFlags = AI_THINKING_STRUCT->aiFlags; //Backup flags so killing in negatives is ignored
				AI_THINKING_STRUCT->aiFlags = 7;
				
				for (i = 0; i < MAX_MON_MOVES && gBattleMons[bankAtk].moves[i] != MOVE_NONE; ++i)
				{
					viabilities[i] = 0;
					bestMoves[i] = 0;

					if (gBitTable[i] & moveLimitations) continue;

					u16 move = gBattleMons[bankAtk].moves[i];
					viabilities[i] = AI_Script_Negatives(bankAtk, bankDef, move, 100);
					viabilities[i] = AI_Script_Positives(bankAtk, bankDef, move, viabilities[i]);
				}

				AI_THINKING_STRUCT->aiFlags = backupFlags;
				
				bestMoves[j = 0] = GetMaxByteIndexInList(viabilities, MAX_MON_MOVES) + 1;
				for (i = 0; i < MAX_MON_MOVES; ++i)
				{
					if (i + 1 != bestMoves[0] //i is not the index returned from GetMaxByteIndexInList
					&& viabilities[i] == viabilities[bestMoves[j] - 1])
						bestMoves[++j] = i + 1;
				}

				StoreMovePrediction(bankAtk, bankDef, gBattleMons[bankAtk].moves[bestMoves[Random() % (j + 1)] - 1]);
			}
		}
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
