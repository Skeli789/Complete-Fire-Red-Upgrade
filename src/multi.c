#include "defines.h"
#include "defines_battle.h"
#include "../include/event_data.h"

#include "../include/new/AI_Helper_Functions.h"
#include "../include/new/ai_master.h"
#include "../include/new/build_pokemon.h"
#include "../include/new/frontier.h"
#include "../include/new/Helper_Functions.h"
#include "../include/m4a.h"
#include "../include/new/mega.h"
#include "../include/new/move_menu.h"
#include "../include/new/multi.h"
#include "../include/new/switching.h"

#define BANK_PLAYER_ALLY 2
#define sBattler data[6]

#define Script_TrainerSpotted (u8*) 0x081A4EB4

//This file's functions:
static void BattleIntroOpponent2SendsOutMonAnimation(void);
static u32 CalcMultiMoneyForTrainer(u16 trainerId);
static void PlayerPartnerBufferExecComplete(void);
static void PlayerPartnerBufferRunCommand(void);
static void PlayerPartnerHandleChooseMove(void);
static void PlayerPartnerHandlePrintSelectionString(void);
static void PlayerPartnerHandleChooseAction(void);
static void PlayerPartnerHandleChoosePokemon(void);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void BattleIntroOpponent1SendsOutMonAnimation(void)
{
	if (gBattleExecBuffer)
		return;

	for (gActiveBattler = 0; gActiveBattler < gBattlersCount; ++gActiveBattler)
	{
		if (GetBattlerPosition(gActiveBattler) == B_POSITION_OPPONENT_LEFT)
		{
			EmitIntroTrainerBallThrow(0);
			MarkBufferBankForExecution(gActiveBattler);
			if (gBattleTypeFlags & (BATTLE_TYPE_MULTI | BATTLE_TYPE_TWO_OPPONENTS))
			{
				gBattleMainFunc = (u32) BattleIntroOpponent2SendsOutMonAnimation;
				return;
			}
		}
	}

	gBattleMainFunc = (u32) BattleIntroRecordMonsToDex;
}

static void BattleIntroOpponent2SendsOutMonAnimation(void)
{
	for (gActiveBattler = 0; gActiveBattler < gBattlersCount; ++gActiveBattler)
	{
		if (GetBattlerPosition(gActiveBattler) == B_POSITION_OPPONENT_RIGHT)
		{
			EmitIntroTrainerBallThrow(0);
			MarkBufferBankForExecution(gActiveBattler);
		}
	}

	gBattleMainFunc = (u32) BattleIntroRecordMonsToDex;
}

void MultiInitPokemonOrder(void)
{
	/*
	 * In a regular double battle, the first two pokemon are the first
	 * two be set out. However in a multi battle, both trainers need
	 * to send out their first Pokemon, which corresponds to slot 0
	 * and 3.
	 */
	if (gBattleTypeFlags & (BATTLE_TYPE_MULTI | BATTLE_TYPE_TWO_OPPONENTS))
	{
		gBattlerPartyIndexes[B_POSITION_OPPONENT_LEFT] = 0;
		gBattlerPartyIndexes[B_POSITION_OPPONENT_RIGHT] = 3;
	}

	if (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER)
		gBattlerPartyIndexes[BANK_PLAYER_ALLY] = 3;
}

void MultiBattleAddSecondOpponent(void)
{
	if (gBattleTypeFlags & (BATTLE_TYPE_MULTI | BATTLE_TYPE_TWO_OPPONENTS))
	{
		if (GetBattlerPosition(gActiveBattler) == 3)
		{
			EmitDrawTrainerPic(0);
			MarkBufferBankForExecution(gActiveBattler);
		}
	}

	/* Send command to partner in case of tag team battle */
	if (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER)
	{
		if (GetBattlerPosition(gActiveBattler) == 2)
		{
			EmitDrawTrainerPic(0);
			MarkBufferBankForExecution(gActiveBattler);
		}
	}
}

void sub_8035C30Fix(void)
{
	bool8 var = FALSE;
	bool8 var2;

	if (!IsDoubleBattle() || ((IsDoubleBattle() && (gBattleTypeFlags & BATTLE_TYPE_MULTI)) || (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS)))
	{
		if (gSprites[gHealthboxIDs[gActiveBattler]].callback == SpriteCallbackDummy)
			var = TRUE;
		var2 = FALSE;
	}
	else
	{
		if (gSprites[gHealthboxIDs[gActiveBattler]].callback == SpriteCallbackDummy
		 && gSprites[gHealthboxIDs[gActiveBattler ^ BIT_FLANK]].callback == SpriteCallbackDummy)
			var = TRUE;
		var2 = TRUE;
	}

	//gUnknown_020244D8 = &gBattleSpritesDataPtr->healthBoxesData[gActiveBattler]; //No idea what the correct ram address is for FR
	//gUnknown_020244DC = &gBattleSpritesDataPtr->healthBoxesData[gActiveBattler ^ BIT_FLANK];

	if (var)
	{
		if (var2 == TRUE)
		{
			if (var2 && gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].field_1_x1 && gBattleSpritesDataPtr->healthBoxesData[gActiveBattler ^ BIT_FLANK].field_1_x1)
			{
				gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].flag_x80 = 0;
				gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].field_1_x1 = 0;
				gBattleSpritesDataPtr->healthBoxesData[gActiveBattler ^ BIT_FLANK].flag_x80 = 0;
				gBattleSpritesDataPtr->healthBoxesData[gActiveBattler ^ BIT_FLANK].field_1_x1 = 0;
				FreeSpriteTilesByTag(0x27F9);
				FreeSpritePaletteByTag(0x27F9);
			}
			else
				return;
		}
		else if (gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].field_1_x1)
		{
			if (GetBattlerPosition(gActiveBattler) == 3)
			{
				if (gBattleSpritesDataPtr->healthBoxesData[gActiveBattler ^ BIT_FLANK].flag_x80 == 0 && gBattleSpritesDataPtr->healthBoxesData[gActiveBattler ^ BIT_FLANK].field_1_x1 == 0)
				{
					FreeSpriteTilesByTag(0x27F9);
					FreeSpritePaletteByTag(0x27F9);
					m4aMPlayVolumeControl(&gMPlay_BGM, 0xFFFF, 256);
				}
				else
					return;
			}
				gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].flag_x80 = 0;
				gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].field_1_x1 = 0;
		}
		else
			return;

		gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].field_9 = 3;
		gBattleBankFunc[gActiveBattler] = (u32) sub_8035BE8;
	}
}

u8* GetTrainerBLoseText(void)
{
	return sTrainerDefeatSpeech_B; //ExtensionState.trainerBDefeatSpeech
}

u32 MultiMoneyCalc(void)
{
	u32 money = CalcMultiMoneyForTrainer(gTrainerBattleOpponent_A);

	if (gBattleTypeFlags & (BATTLE_TYPE_MULTI | BATTLE_TYPE_TWO_OPPONENTS)) {
		money += CalcMultiMoneyForTrainer(VarGet(SECOND_OPPONENT_VAR));
	}

	return money;
}

static u32 CalcMultiMoneyForTrainer(u16 trainerId)
{
	int i;
	struct Trainer trainer = gTrainers[trainerId];
	u8 rate = 0;
	
	/* Find level of the last Pokemon in trainer's party */
	u8 lastMon = trainer.partySize - 1;
	if (gBattleTypeFlags & (BATTLE_TYPE_MULTI | BATTLE_TYPE_TWO_OPPONENTS) && lastMon > 3)
		lastMon = 3;
	
	u8 level = 0;
	#ifdef OPEN_WORLD_TRAINERS
		level = openWorldLevelRanges[GetOpenWorldBadgeCount()][1];
	#else
		if (trainer.partyFlags & PARTY_FLAG_CUSTOM_MOVES)
			level = trainer.party.ItemCustomMoves[lastMon].lvl;
		else
			level = trainer.party.NoItemDefaultMoves[lastMon].lvl;
	#endif
	
	for (i = 0; i < NUM_TRAINER_CLASSES; ++i)
	{
		if (gTrainerMoneyTable[i].trainerClass == trainer.trainerClass)
		{
			rate = gTrainerMoneyTable[i].money;
			break;
		}
		
		if (gTrainerMoneyTable[i].trainerClass == 0xFF)
		{
			rate = gTrainerMoneyTable[0].money;
			break;
		}
	}

	u32 money = rate * level * gBattleStruct->moneyMultiplier * 4;

	if ((gBattleTypeFlags & BATTLE_TYPE_DOUBLE) && !(gBattleTypeFlags & (BATTLE_TYPE_MULTI | BATTLE_TYPE_TWO_OPPONENTS)))
		money *= 2;

	return money;
}

bool8 IsLinkDoubleBattle(void)
{
	u32 flags = (BATTLE_TYPE_MULTI | BATTLE_TYPE_TRAINER | BATTLE_TYPE_LINK | BATTLE_TYPE_DOUBLE);
	
	if ((gBattleTypeFlags & flags) == flags)
		return TRUE;
	else
		return FALSE;
}

bool8 IsMultiBattle(void)
{
	u32 flags = (BATTLE_TYPE_MULTI | BATTLE_TYPE_INGAME_PARTNER | BATTLE_TYPE_TRAINER | BATTLE_TYPE_DOUBLE);
	u32 flags2 = (BATTLE_TYPE_MULTI | BATTLE_TYPE_TRAINER | BATTLE_TYPE_DOUBLE);
	u32 flags3 = (BATTLE_TYPE_INGAME_PARTNER | BATTLE_TYPE_TRAINER | BATTLE_TYPE_DOUBLE);
	u32 flags4 = (BATTLE_TYPE_INGAME_PARTNER | BATTLE_TYPE_DOUBLE);
	
	if (((gBattleTypeFlags & flags) == flags 
	||   (gBattleTypeFlags & flags) == flags2 
	||   (gBattleTypeFlags & flags) == flags3
	||   (gBattleTypeFlags & flags) == flags4) && gMain.inBattle)
		return TRUE;
	else
		return FALSE;
}

bool8 IsTwoOpponentBattle(void)
{
	if ((gBattleTypeFlags & (BATTLE_TYPE_DOUBLE | BATTLE_TYPE_TWO_OPPONENTS)) == (BATTLE_TYPE_DOUBLE | BATTLE_TYPE_TWO_OPPONENTS))
		return TRUE;
	
	return FALSE;
}

bool8 BankSideHasTwoTrainers(u8 bank)
{
	u8 side = SIDE(bank);
	
	return ((side == B_SIDE_OPPONENT && IsTwoOpponentBattle())
		 || (side == B_SIDE_PLAYER && IsTagBattle()));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool8 IsTagBattle(void)
{
	u32 flags = (BATTLE_TYPE_DOUBLE | BATTLE_TYPE_INGAME_PARTNER);
	
	if ((gBattleTypeFlags & flags) == flags)
		return TRUE;
	else
		return FALSE;
}

bool8 IsPartnerAttacker(void)
{
	return gBankAttacker == B_POSITION_PLAYER_RIGHT;
}

u8* PartnerLoadName(void)
{
	CopyFrontierTrainerName(gStringVar4, VarGet(PARTNER_VAR), 2);
	return gStringVar4;
}

u8 LoadPartnerBackspriteIndex(void)
{
	return VarGet(PARTNER_BACKSPRITE_VAR);
}

void ChooseProperPartnerController(void)
{
	gBanksBySide[0] = 0;
	gBanksBySide[1] = 1;
	gBanksBySide[3] = 3;
	gBattlersCount = 4;
	
	gBattleBankFunc[0] = (u32) SetControllerToPlayer;
	gBattleBankFunc[1] = (u32) SetControllerToOpponent;
	gBattleBankFunc[3] = (u32) SetControllerToOpponent;
	
	if (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER) {
		gBattleBankFunc[2] = (u32) SetControllerToPlayerPartner;
		gBanksBySide[2] = 2; //The game crashes when this line is not here for some reason
	}
	
	else {
		gBattleBankFunc[2] = (u32) SetControllerToPlayer;
		gBanksBySide[2] = 2; //The game crashes when this line is not here for some reason
	}
}

void SetControllerToPlayerPartner(void)
{
	gBattleBankFunc[gActiveBattler] = (u32) PlayerPartnerBufferRunCommand;
}

static void PlayerPartnerBufferExecComplete(void)
{
	gBattleBankFunc[gActiveBattler] = (u32) PlayerPartnerBufferRunCommand;
	if (gBattleTypeFlags & BATTLE_TYPE_LINK)
	{
		u8 playerId = GetMultiplayerId();

		PrepareBufferDataTransferLink(2, 4, &playerId);
		gBattleBufferA[gActiveBattler][0] = CONTROLLER_TERMINATOR_NOP;
	}
	else
	{
		gBattleExecBuffer &= ~gBitTable[gActiveBattler];
	}
}

static void PlayerPartnerBufferRunCommand(void)
{
	if (gBattleExecBuffer & gBitTable[gActiveBattler]) 
	{
		u8 buffer = gBattleBufferA[gActiveBattler][0];

		if (buffer > COMMAND_MAX)
			PlayerPartnerBufferExecComplete();
		else
			sPlayerPartnerBufferCommands[buffer]();
	}
}

static void PlayerPartnerHandleChooseMove(void)
{
	u8 chosenMoveId;
	u16 chosenMove;
	struct ChooseMoveStruct* moveInfo = (struct ChooseMoveStruct*)(&gBattleBufferA[gActiveBattler][4]);

	BattleAI_SetupAIData(0xF);
	chosenMoveId = BattleAI_ChooseMoveOrAction();
	chosenMove = moveInfo->moves[chosenMoveId];

	if (gBattleMoves[chosenMove].target & MOVE_TARGET_USER)
	{
		gBankTarget = gActiveBattler;
	}
	else if (gBattleMoves[chosenMove].target & MOVE_TARGET_USER_OR_PARTNER)
	{
		if (SIDE(gBankTarget) != SIDE(gActiveBattler))
			gBankTarget = gActiveBattler;
	}
	else if (gBattleMoves[chosenMove].target & MOVE_TARGET_BOTH)
	{
		gBankTarget = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
		if (gAbsentBattlerFlags & gBitTable[gBankTarget])
			gBankTarget = GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT);
	}
	
	if (moveInfo->possibleZMoves[chosenMoveId])
	{
		if (ShouldAIUseZMove(gActiveBattler, gBankTarget, moveInfo->moves[chosenMoveId]))
			gNewBS->ZMoveData->toBeUsed[gActiveBattler] = TRUE;
	}
	else if (!ShouldAIDelayMegaEvolution(gActiveBattler, gBankTarget, chosenMove))
	{
		if (moveInfo->canMegaEvolve && moveInfo->megaVariance != MEGA_VARIANT_ULTRA_BURST)
			gNewBS->MegaData->chosen[gActiveBattler] = TRUE;
		else if (moveInfo->canMegaEvolve && moveInfo->megaVariance == MEGA_VARIANT_ULTRA_BURST)
			gNewBS->UltraData->chosen[gActiveBattler] = TRUE;
	}
	
	//This is handled again later, but it's only here to help with the case of choosing Helping Hand when the partner is switching out.
	gBattleStruct->chosenMovePositions[gActiveBattler] = chosenMoveId;
	gBattleStruct->moveTarget[gActiveBattler] = gBankTarget;
	gChosenMovesByBanks[gActiveBattler] = chosenMove;
	TryRemoveDoublesKillingScore(gActiveBattler, gBankTarget, chosenMove);

	EmitMoveChosen(1, chosenMoveId, gBankTarget, gNewBS->MegaData->chosen[gActiveBattler], gNewBS->UltraData->chosen[gActiveBattler], gNewBS->ZMoveData->toBeUsed[gActiveBattler]);
	PlayerPartnerBufferExecComplete();
}

static void PlayerPartnerHandlePrintSelectionString(void)
{
	PlayerPartnerBufferExecComplete();
}

static void PlayerPartnerHandleChooseAction(void)
{
	AI_TrySwitchOrUseItem();
	PlayerPartnerBufferExecComplete();
}

static void PlayerPartnerHandleChoosePokemon(void)
{
	u8 chosenMonId;
	
	if (gBattleStruct->switchoutIndex[SIDE(gActiveBattler)] == PARTY_SIZE)
	{
		u8 battlerIn1, battlerIn2, firstId, lastId;
		struct Pokemon* party = LoadPartyRange(gActiveBattler, &firstId, &lastId);
			
		if (gNewBS->bestMonIdToSwitchInto[gActiveBattler][0] == PARTY_SIZE
		||  GetMonData(&party[gNewBS->bestMonIdToSwitchInto[gActiveBattler][0]], MON_DATA_HP, NULL) == 0)
			CalcMostSuitableMonToSwitchInto();

		chosenMonId = GetMostSuitableMonToSwitchInto();
		
		if (chosenMonId == PARTY_SIZE)
		{
			if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
			{
				battlerIn1 = gActiveBattler;
				if (gAbsentBattlerFlags & gBitTable[PARTNER(gActiveBattler)])
					battlerIn2 = gActiveBattler;
				else
					battlerIn2 = PARTNER(battlerIn1);
			}
			else
			{
				battlerIn1 = gActiveBattler;
				battlerIn2 = gActiveBattler;
			}
				
			for (chosenMonId = firstId; chosenMonId < lastId; ++chosenMonId)
			{
				if (party[chosenMonId].species != SPECIES_NONE
				&& party[chosenMonId].hp != 0
				&& !GetMonData(&party[chosenMonId], MON_DATA_IS_EGG, 0)
				&& chosenMonId != gBattlerPartyIndexes[battlerIn1]
				&& chosenMonId != gBattlerPartyIndexes[battlerIn2])
					break;
			}
		}
	}
	else
	{
		chosenMonId = gBattleStruct->switchoutIndex[SIDE(gActiveBattler)];
		gBattleStruct->switchoutIndex[SIDE(gActiveBattler)] = PARTY_SIZE;
	}

	RemoveBestMonToSwitchInto(gActiveBattler);

	/* Perform switchout */
	gBattleStruct->monToSwitchIntoId[gActiveBattler] = chosenMonId;
	EmitChosenMonReturnValue(1, chosenMonId, 0);
	PlayerPartnerBufferExecComplete();
}

#define WRAP_COMMAND_ADDRESS(x) ((void (*)(void)) (x))

/*
 * Most of these are just reused from the player's command table to
 * save rewriting them all.
 */
void (*const sPlayerPartnerBufferCommands[COMMAND_MAX])(void) =
{
	WRAP_COMMAND_ADDRESS(0x8030B91), /* 00 */
	WRAP_COMMAND_ADDRESS(0x80313B1), /* 01 */
	WRAP_COMMAND_ADDRESS(0x8031439), /* 02 */
	WRAP_COMMAND_ADDRESS(0x8031E8D), /* 03 */
	WRAP_COMMAND_ADDRESS(0x8031F01), /* 04 */
	WRAP_COMMAND_ADDRESS(0x8031F69), /* 05 */
	WRAP_COMMAND_ADDRESS(0x8032161), /* 06 */
	WRAP_COMMAND_ADDRESS(0x803227D), /* 07 */
	WRAP_COMMAND_ADDRESS(0x8032429), /* 08 */
	WRAP_COMMAND_ADDRESS(0x8032591), /* 09 */
	WRAP_COMMAND_ADDRESS(0x8032651), /* 0a */
	WRAP_COMMAND_ADDRESS(0x803273D), /* 0b */
	WRAP_COMMAND_ADDRESS(0x803275D), /* 0c */
	WRAP_COMMAND_ADDRESS(0x80327B1), /* 0d */
	WRAP_COMMAND_ADDRESS(0x8032811), /* 0e */
	WRAP_COMMAND_ADDRESS(0x8032841), /* 0f */
	WRAP_COMMAND_ADDRESS(0x8032AFD), /* 10 */
	PlayerPartnerHandlePrintSelectionString,	/* 11 */
	PlayerPartnerHandleChooseAction,	 		/* 12 */
	WRAP_COMMAND_ADDRESS(0x8032C49), 			/* 13 */
	PlayerPartnerHandleChooseMove,		 		/* 14 */
	WRAP_COMMAND_ADDRESS(0x8032CED), 			/* 15 */
	PlayerPartnerHandleChoosePokemon,		   /* 16 */
	WRAP_COMMAND_ADDRESS(0x8032E29), /* 17 */
	WRAP_COMMAND_ADDRESS(0x8032E4D), /* 18 */
	WRAP_COMMAND_ADDRESS(0x8032F4D), /* 19 */
	WRAP_COMMAND_ADDRESS(0x8032FE9), /* 1a */
	WRAP_COMMAND_ADDRESS(0x8033061), /* 1b */
	WRAP_COMMAND_ADDRESS(0x80330C9), /* 1c */
	WRAP_COMMAND_ADDRESS(0x8033135), /* 1d */
	WRAP_COMMAND_ADDRESS(0x8033141), /* 1e */
	WRAP_COMMAND_ADDRESS(0x80331F5), /* 1f */
	WRAP_COMMAND_ADDRESS(0x8033225), /* 20 */
	WRAP_COMMAND_ADDRESS(0x8033231), /* 21 */
	WRAP_COMMAND_ADDRESS(0x8033245), /* 22 */
	WRAP_COMMAND_ADDRESS(0x8033259), /* 23 */
	WRAP_COMMAND_ADDRESS(0x803326D), /* 24 */
	WRAP_COMMAND_ADDRESS(0x8033281), /* 25 */
	WRAP_COMMAND_ADDRESS(0x803329D), /* 26 */
	WRAP_COMMAND_ADDRESS(0x80332D5), /* 27 */
	WRAP_COMMAND_ADDRESS(0x80332ED), /* 28 */
	WRAP_COMMAND_ADDRESS(0x8033315), /* 29 */
	WRAP_COMMAND_ADDRESS(0x8033385), /* 2a */
	WRAP_COMMAND_ADDRESS(0x8033391), /* 2b */
	WRAP_COMMAND_ADDRESS(0x80333D5), /* 2c */
	WRAP_COMMAND_ADDRESS(0x8033405), /* 2d */
	WRAP_COMMAND_ADDRESS(0x8033445), /* 2e */
	WRAP_COMMAND_ADDRESS(0x8033479), /* 2f */
	WRAP_COMMAND_ADDRESS(0x803376D), /* 30 */
	WRAP_COMMAND_ADDRESS(0x8033879), /* 31 */
	WRAP_COMMAND_ADDRESS(0x80338C9), /* 32 */
	WRAP_COMMAND_ADDRESS(0x80338ED), /* 33 */
	WRAP_COMMAND_ADDRESS(0x803394D), /* 34 */
	WRAP_COMMAND_ADDRESS(0x80339B5), /* 35 */
	WRAP_COMMAND_ADDRESS(0x8033A11), /* 36 */
	WRAP_COMMAND_ADDRESS(0x8033A79), /* 37 */
	WRAP_COMMAND_ADDRESS(0x8033AC5), /* 38 */
};
