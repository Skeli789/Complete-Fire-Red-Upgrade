#include "defines.h"
#include "defines_battle.h"
#include "../include/battle_anim.h"
#include "../include/event_data.h"
#include "../include/pokeball.h"
#include "../include/random.h"
#include "../include/constants/trainers.h"

#include "../include/new/ai_util.h"
#include "../include/new/ai_master.h"
#include "../include/new/ai_switching.h"
#include "../include/new/battle_controller_opponent.h"
#include "../include/new/battle_start_turn_start.h"
#include "../include/new/battle_util.h"
#include "../include/new/frontier.h"
#include "../include/new/mega.h"
#include "../include/new/move_menu.h"
#include "../include/new/multi.h"
#include "../include/new/switching.h"

/*
battle_controller_opponent.c
	handles the functions responsible for the user moving between battle menus, choosing moves, etc.
*/

//This file's functions:
static void TryRechoosePartnerMove(u16 chosenMove);
static u8 LoadCorrectTrainerPicId(void);

void OpponentHandleChooseMove(void)
{
	u8 chosenMovePos;
	struct ChooseMoveStruct* moveInfo = (struct ChooseMoveStruct*)(&gBattleBufferA[gActiveBattler][4]);

	#ifdef VAR_GAME_DIFFICULTY
	u8 difficulty = VarGet(VAR_GAME_DIFFICULTY);
	#endif

	if ((gBattleTypeFlags & (BATTLE_TYPE_TRAINER | BATTLE_TYPE_OAK_TUTORIAL | BATTLE_TYPE_SAFARI | BATTLE_TYPE_ROAMER))
	#ifdef FLAG_SMART_WILD
	||  FlagGet(FLAG_SMART_WILD)
	#endif
	#ifdef VAR_GAME_DIFFICULTY //Wild Pokemon are smart in expert mode
	||  difficulty == OPTIONS_EXPERT_DIFFICULTY
	#endif
	|| (gBattleTypeFlags & BATTLE_TYPE_SHADOW_WARRIOR)
	|| (!(gBattleTypeFlags & BATTLE_TYPE_TRAINER) && WildMonIsSmart(gActiveBattler))
	|| (IsRaidBattle() && gRaidBattleStars >= 6))
	{
		if (RAID_BATTLE_END)
			goto CHOOSE_DUMB_MOVE;

		BattleAI_SetupAIData(0xF);
		chosenMovePos = BattleAI_ChooseMoveOrAction();

		switch (chosenMovePos) {
			case AI_CHOICE_WATCH:
				EmitTwoReturnValues(1, ACTION_WATCHES_CAREFULLY, 0);
				break;

			case AI_CHOICE_FLEE:
				EmitTwoReturnValues(1, ACTION_RUN, 0);
				break;

			case 6:
				EmitTwoReturnValues(1, 15, gBankTarget);
				break;

			default: ;
				u16 chosenMove = moveInfo->moves[chosenMovePos];
				u8 moveTarget = GetBaseMoveTarget(chosenMove, gActiveBattler);

				if (moveTarget & MOVE_TARGET_USER)
				{
					gBankTarget = gActiveBattler;
				}
				else if (moveTarget & MOVE_TARGET_USER_OR_PARTNER)
				{
					if (SIDE(gBankTarget) != SIDE(gActiveBattler))
						gBankTarget = gActiveBattler;
				}
				else if (moveTarget & MOVE_TARGET_BOTH)
				{
					if (SIDE(gActiveBattler) == B_SIDE_PLAYER)
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

				//You get 1 of 3 of the following gimmicks per Pokemon
				if (moveInfo->possibleZMoves[chosenMovePos]) //Checked first b/c Rayquaza can do all 3
				{
					u8 foe = gBankTarget;

					if (IS_SINGLE_BATTLE)
					{
						if (gActiveBattler == gBankTarget)
							foe = FOE(gActiveBattler); //Use actual enemy in calc

						//Allows for fresh calc factoring in foe move prediction
						ClearShouldAIUseZMoveByMoveAndMovePos(gActiveBattler, foe, chosenMovePos);
					}

					if (ShouldAIUseZMoveByMoveAndMovePos(gActiveBattler, foe, moveInfo->moves[chosenMovePos], chosenMovePos))
						gNewBS->zMoveData.toBeUsed[gActiveBattler] = TRUE;
				}
				else if (moveInfo->canMegaEvolve)
				{
					if (!ShouldAIDelayMegaEvolution(gActiveBattler, gBankTarget, chosenMove, FALSE, TRUE))
					{
						if (moveInfo->megaVariance != MEGA_VARIANT_ULTRA_BURST)
							gNewBS->megaData.chosen[gActiveBattler] = TRUE;
						else if (moveInfo->megaVariance == MEGA_VARIANT_ULTRA_BURST)
							gNewBS->ultraData.chosen[gActiveBattler] = TRUE;
					}
				}
				else if (moveInfo->possibleMaxMoves[chosenMovePos]) //Handles the "Can I Dynamax" checks
				{
					if (ShouldAIDynamax(gActiveBattler))
						gNewBS->dynamaxData.toBeUsed[gActiveBattler] = TRUE;
				}

				//This is handled again later, but it's only here to help with the case of choosing Helping Hand when the partner is switching out.
				gBattleStruct->chosenMovePositions[gActiveBattler] = chosenMovePos;
				gBattleStruct->moveTarget[gActiveBattler] = gBankTarget;
				gChosenMovesByBanks[gActiveBattler] = chosenMove;
				TryRemovePartnerDoublesKillingScoreComplete(gActiveBattler, gBankTarget, chosenMove, moveTarget, TRUE); //Allow the partner to choose a new target if its best move was this target

				EmitMoveChosen(1, chosenMovePos, gBankTarget, gNewBS->megaData.chosen[gActiveBattler], gNewBS->ultraData.chosen[gActiveBattler], gNewBS->zMoveData.toBeUsed[gActiveBattler], gNewBS->dynamaxData.toBeUsed[gActiveBattler]);
				TryRechoosePartnerMove(moveInfo->moves[chosenMovePos]);
				break;
		}

		OpponentBufferExecCompleted();
	}
	else
	{
		CHOOSE_DUMB_MOVE: ;
		u16 move;
		do
		{
			chosenMovePos = Random() & 3;
			move = moveInfo->moves[chosenMovePos];
		} while (move == MOVE_NONE);

		if (GetBaseMoveTarget(move, gActiveBattler) & (MOVE_TARGET_USER_OR_PARTNER | MOVE_TARGET_USER))
			EmitMoveChosen(1, chosenMovePos, gActiveBattler, 0, 0, 0, FALSE);
		else if (IS_DOUBLE_BATTLE)
			EmitMoveChosen(1, chosenMovePos, GetBattlerAtPosition(Random() & 2), 0, 0, 0, FALSE);
		else
			EmitMoveChosen(1, chosenMovePos, FOE(gActiveBattler), 0, 0, 0, FALSE);

		OpponentBufferExecCompleted();
	}
}

#define STATE_BEFORE_ACTION_CHOSEN 0
static void TryRechoosePartnerMove(u16 chosenMove)
{
	u32 speedCalcBank = SpeedCalc(gActiveBattler);
	u32 speedCalcPartner = SpeedCalc(PARTNER(gActiveBattler));

	if (speedCalcBank < speedCalcPartner //Second to choose action on either side
	|| (speedCalcBank == speedCalcPartner && (GetBattlerPosition(gActiveBattler) & BIT_FLANK) == B_FLANK_RIGHT)) //Same speed and second mon on side
	{
		switch (gChosenMovesByBanks[PARTNER(gActiveBattler)]) {
			case MOVE_HELPINGHAND:
				if (chosenMove == MOVE_NONE || SPLIT(chosenMove) == SPLIT_STATUS)
				{
					struct ChooseMoveStruct moveInfo;
					gChosenMovesByBanks[gActiveBattler] = chosenMove;

					u8 backup = gActiveBattler;
					gActiveBattler = PARTNER(gActiveBattler);
					ForceCompleteDamageRecalculation(gActiveBattler);
					EmitChooseMove(0, (IS_DOUBLE_BATTLE) != 0, FALSE, &moveInfo); //Rechoose partner move
					MarkBufferBankForExecution(gActiveBattler);
					gActiveBattler = backup;
				}
				break;
		}
	}
	else if (!IsBankIncapacitated(gActiveBattler) //The first Pokemon will actually attack
	&& (chosenMove == MOVE_FUSIONFLARE || chosenMove == MOVE_FUSIONBOLT)) //The first Pokemon chose one of these moves
	{
		u8 partner = PARTNER(gActiveBattler);
		u8 foe1 = FOE(gActiveBattler);

		//Force recalculation since Fusion moves are now twice as strong
		if (chosenMove == MOVE_FUSIONFLARE)
		{
			u8 movePos = FindMovePositionInMoveset(MOVE_FUSIONBOLT, partner);
			if (movePos < MAX_MON_MOVES)
				ForceSpecificDamageRecalculation(partner, foe1, movePos);
		}
		else //Fusion Bolt
		{
			u8 movePos = FindMovePositionInMoveset(MOVE_FUSIONFLARE, partner);
			if (movePos < MAX_MON_MOVES)
				ForceSpecificDamageRecalculation(partner, foe1, movePos);
		}
	}
}

#define STATE_WAIT_ACTION_CONFIRMED 4
bool8 ShouldAIChooseAction(u8 position)
{
	//Try prioritizing AI mons in order from fastest to slowest (gets better calcs)
	u8 bank = GetBattlerAtPosition(position);
	u8 partner = GetBattlerAtPosition(BATTLE_PARTNER(position));

	if (gBattleTypeFlags & BATTLE_TYPE_MULTI
	|| gBattleStruct->field_91 & gBitTable[partner] //Only mon on side
	|| gBattleCommunication[partner] == STATE_WAIT_ACTION_CONFIRMED) //Partner already chose action
		return TRUE;

	if (!(gBattleTypeFlags & BATTLE_TYPE_LINK) //Vs AI
	&& IS_DOUBLE_BATTLE
	&& (SIDE(bank) == B_SIDE_OPPONENT || IsMockBattle())) //AI controlled side
	{
		u32 speedCalcBank = SpeedCalc(bank);
		u32 speedCalcPartner = SpeedCalc(partner);

		if (speedCalcBank > speedCalcPartner) //This mon would probably hit before partner
			return TRUE;
		else if (speedCalcBank == speedCalcPartner //Speed tie
		&& (position & BIT_FLANK) == B_FLANK_LEFT) //Then assume left slot would move first
			return TRUE;
	}
	else if ((position & BIT_FLANK) == B_FLANK_LEFT) //Left slot
		return TRUE;

	return FALSE;
}

void OpponentHandleDrawTrainerPic(void)
{
	u32 trainerPicId = LoadCorrectTrainerPicId();
	s16 xPos;

	if (gBattleTypeFlags & (BATTLE_TYPE_MULTI | BATTLE_TYPE_TWO_OPPONENTS))
	{
		if ((GetBattlerPosition(gActiveBattler) & BIT_FLANK) != 0) // second mon
			xPos = 152;
		else // first mon
			xPos = 200;
	}
	else
	{
		xPos = 176;
	}

	DecompressTrainerFrontPic(trainerPicId, gActiveBattler); //0x80346C4
	SetMultiuseSpriteTemplateToTrainerBack(trainerPicId, GetBattlerPosition(gActiveBattler));
	gBattlerSpriteIds[gActiveBattler] = CreateSprite(&gMultiuseSpriteTemplate[0],
											   xPos,
											   (8 - gTrainerFrontPicCoords[trainerPicId].coords) * 4 + 40,
											   GetBattlerSpriteSubpriority(gActiveBattler));

	gSprites[gBattlerSpriteIds[gActiveBattler]].pos2.x = -240;
	gSprites[gBattlerSpriteIds[gActiveBattler]].data[0] = 3; //2; //Speed scrolling in
	gSprites[gBattlerSpriteIds[gActiveBattler]].oam.paletteNum = IndexOfSpritePaletteTag(gTrainerFrontPicPaletteTable[trainerPicId].tag);
	gSprites[gBattlerSpriteIds[gActiveBattler]].data[5] = gSprites[gBattlerSpriteIds[gActiveBattler]].oam.tileNum;
	gSprites[gBattlerSpriteIds[gActiveBattler]].oam.tileNum = GetSpriteTileStartByTag(gTrainerFrontPicTable[trainerPicId].tag);
	gSprites[gBattlerSpriteIds[gActiveBattler]].oam.affineParam = trainerPicId;
	gSprites[gBattlerSpriteIds[gActiveBattler]].callback = SpriteCB_TrainerSlideIn;

	gBattlerControllerFuncs[gActiveBattler] = CompleteOnBattlerSpriteCallbackDummy;
}

void OpponentHandleTrainerSlide(void)
{

	u32 trainerPicId = LoadCorrectTrainerPicId();

	DecompressTrainerFrontPic(trainerPicId, gActiveBattler);
	SetMultiuseSpriteTemplateToTrainerBack(trainerPicId, GetBattlerPosition(gActiveBattler));
	gBattlerSpriteIds[gActiveBattler] = CreateSprite(&gMultiuseSpriteTemplate[0], 176, (8 - gTrainerFrontPicCoords[trainerPicId].coords) * 4 + 40, 30);

	gSprites[gBattlerSpriteIds[gActiveBattler]].pos2.x = 96;
	gSprites[gBattlerSpriteIds[gActiveBattler]].pos1.x += 32;
	gSprites[gBattlerSpriteIds[gActiveBattler]].data[0] = -2;
	gSprites[gBattlerSpriteIds[gActiveBattler]].oam.paletteNum = IndexOfSpritePaletteTag(gTrainerFrontPicPaletteTable[trainerPicId].tag);
	gSprites[gBattlerSpriteIds[gActiveBattler]].data[5] = gSprites[gBattlerSpriteIds[gActiveBattler]].oam.tileNum;
	gSprites[gBattlerSpriteIds[gActiveBattler]].oam.tileNum = GetSpriteTileStartByTag(gTrainerFrontPicTable[trainerPicId].tag);
	gSprites[gBattlerSpriteIds[gActiveBattler]].oam.affineParam = trainerPicId;
	gSprites[gBattlerSpriteIds[gActiveBattler]].callback = SpriteCB_TrainerSlideIn;

	gBattlerControllerFuncs[gActiveBattler] = CompleteOnBankSpriteCallbackDummy2;
}

void OpponentHandleChoosePokemon(void)
{
	u8 chosenMonId;

	if (gBattleStruct->switchoutIndex[SIDE(gActiveBattler)] == PARTY_SIZE)
	{
		u8 battlerIn1, battlerIn2, firstId, lastId;
		struct Pokemon* party = LoadPartyRange(gActiveBattler, &firstId, &lastId);

		if (IS_DOUBLE_BATTLE)
		{
			battlerIn1 = gActiveBattler; //The dead mon
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

		if (gNewBS->inPivotingMove //TODO: Add logic for Baton Pass
		&& gNewBS->ai.pivotTo[gActiveBattler] != PARTY_SIZE //Set at some point before
		&& gNewBS->ai.pivotTo[gActiveBattler] != battlerIn1 //Hasn't been switched in, in the mean time
		&& gNewBS->ai.pivotTo[gActiveBattler] != battlerIn2
		&& party[gNewBS->ai.pivotTo[gActiveBattler]].hp != 0) //Still alive
		{
			chosenMonId = gNewBS->ai.pivotTo[gActiveBattler];
		}
		else
		{
			if (gNewBS->ai.bestMonIdToSwitchInto[gActiveBattler][0] == PARTY_SIZE
			|| GetMonData(&party[gNewBS->ai.bestMonIdToSwitchInto[gActiveBattler][0]], MON_DATA_HP, NULL) == 0 //Best mon is dead
			|| gNewBS->ai.bestMonIdToSwitchInto[gActiveBattler][0] == gBattlerPartyIndexes[battlerIn1]
			|| gNewBS->ai.bestMonIdToSwitchInto[gActiveBattler][0] == gBattlerPartyIndexes[battlerIn2]) //The best mon is already in
				CalcMostSuitableMonToSwitchInto();

			chosenMonId = GetMostSuitableMonToSwitchInto();
		}

		if (chosenMonId >= PARTY_SIZE
		|| chosenMonId < firstId || chosenMonId >= lastId) //Trying to pick from partner's team
		{
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
	gBattleStruct->monToSwitchIntoId[gActiveBattler] = chosenMonId;
	EmitChosenMonReturnValue(1, chosenMonId, 0);
	OpponentBufferExecCompleted();
	TryRechoosePartnerMove(MOVE_NONE);
}

static u8 LoadCorrectTrainerPicId(void)
{
	u8 trainerPicId;
	u8 position = GetBattlerPosition(gActiveBattler);

	if (gTrainerBattleOpponent_A == TRAINER_SECRET_BASE)
	{
		trainerPicId = GetSecretBaseTrainerPicIndex();
	}
	else if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER
	|| (position == B_POSITION_OPPONENT_LEFT && IsFrontierTrainerId(gTrainerBattleOpponent_A))
	|| (position == B_POSITION_OPPONENT_RIGHT && IsFrontierTrainerId(gTrainerBattleOpponent_B)))
	{
		if (IsTwoOpponentBattle()
		|| (gBattleTypeFlags & BATTLE_TYPE_LINK && gBattleTypeFlags & BATTLE_TYPE_MULTI))
		{
			if (position == B_POSITION_OPPONENT_LEFT)
				trainerPicId = GetFrontierTrainerFrontSpriteId(gTrainerBattleOpponent_A, 0);
			else
				trainerPicId = GetFrontierTrainerFrontSpriteId(gTrainerBattleOpponent_B, 1);
		}
		else
		{
			trainerPicId = GetFrontierTrainerFrontSpriteId(gTrainerBattleOpponent_A, 0);
		}
	}
	else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_TOWER)
	{
		trainerPicId = GetTrainerTowerTrainerPicIndex(); //0x815DA3C
	}
	else if (gBattleTypeFlags & BATTLE_TYPE_EREADER_TRAINER)
	{
		trainerPicId = GetEreaderTrainerFrontSpriteId(); //0x80E7420
	}
	else if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS)
	{
		if (position == 1)
			trainerPicId = gTrainers[gTrainerBattleOpponent_A].trainerPic;
		else
			trainerPicId = gTrainers[gTrainerBattleOpponent_B].trainerPic;
	}
	else
	{
		trainerPicId = gTrainers[gTrainerBattleOpponent_A].trainerPic;
	}

	return trainerPicId;
}

void SpriteCB_SlideInTrainer(struct Sprite* sprite)
{
    if (!(gIntroSlideFlags & 1))
    {
        sprite->pos2.x += sprite->data[0];
		
		if (sprite->data[0] > 0) //Positive
		{
			if (sprite->pos2.x > 0)
			{
				sprite->callback = SpriteCallbackDummy;
				sprite->pos2.x = 0;
			}
		}
		else //Negative
		{
			if (sprite->pos2.x < 0)
			{
				sprite->callback = SpriteCallbackDummy;
				sprite->pos2.x = 0;
			}
		}
    }
}

#define sBattler data[0]
void SpriteCB_WildMon(struct Sprite *sprite)
{
	u32 selectedPalettes = 0x10000 << sprite->oam.paletteNum;

    sprite->callback = (void*) (0x8011D94 | 1); //SpriteCB_MoveWildMonToRight
    StartSpriteAnimIfDifferent(sprite, 0);
    if (!BeginNormalPaletteFade(selectedPalettes, 0, 10, 10, RGB(8, 8, 8))) //If a fade is already in progress,
		gPaletteFade_selectedPalettes |= selectedPalettes; //Then add second mon in wild doubles to the palettes to fade
}

void SpriteCB_WildMonShowHealthbox(struct Sprite *sprite)
{
    if (sprite->animEnded)
    {
		u32 selectedPalettes = 0x10000 << sprite->oam.paletteNum;

        StartHealthboxSlideIn(sprite->sBattler);
        SetHealthboxSpriteVisible(gHealthboxSpriteIds[sprite->sBattler]);
        sprite->callback = SpriteCallbackDummy;
        StartSpriteAnimIfDifferent(sprite, 0);
        if (!BeginNormalPaletteFade(selectedPalettes, 0, 10, 0, RGB(8, 8, 8))) //If a fade is already in progress,
			gPaletteFade_selectedPalettes |= selectedPalettes; //Then add second mon in wild doubles to the palettes to unfade
    }
}
#undef sBattler

#define sBattler data[6]
void SpriteCB_OpponentMonSendOut_1(struct Sprite* sprite)
{
	sprite->data[0] = 25;
	sprite->data[2] = GetBattlerSpriteCoord(sprite->sBattler, BATTLER_COORD_X_2);
	sprite->data[4] = GetBattlerSpriteCoord(sprite->sBattler, BATTLER_COORD_Y) + 24;
	sprite->data[5] = -30;

	if (IS_DOUBLE_BATTLE && GetBattlerPosition(sprite->sBattler) == B_POSITION_OPPONENT_LEFT)
		sprite->data[0] = 23; //Slightly faster than the second mon

	sprite->oam.affineParam = sprite->sBattler;
	InitAnimArcTranslation(sprite);
	sprite->callback = SpriteCB_PlayerMonSendOut_2;
}
#undef sBattler
