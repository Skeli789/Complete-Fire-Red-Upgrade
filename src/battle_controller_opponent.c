#include "defines.h"
#include "defines_battle.h"
#include "../include/event_data.h"
#include "../include/pokeball.h"
#include "../include/random.h"
#include "../include/constants/trainers.h"

#include "../include/new/ai_util.h"
#include "../include/new/ai_master.h"
#include "../include/new/battle_controller_opponent.h"
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

//TODO: Update Acupressure Targeting for AI

//This file's functions:
static void TryRechoosePartnerMove(u16 chosenMove);
static u8 LoadCorrectTrainerPicId(void);

void OpponentHandleChooseMove(void)
{
	u8 chosenMoveId;
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
	|| (!(gBattleTypeFlags & BATTLE_TYPE_TRAINER) && WildMonIsSmart(gActiveBattler))
	|| (IsRaidBattle() && gRaidBattleStars >= 6))
	{
		if (RAID_BATTLE_END)
			goto CHOOSE_DUMB_MOVE;

		BattleAI_SetupAIData(0xF);
		chosenMoveId = BattleAI_ChooseMoveOrAction();

		switch (chosenMoveId) {
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
				u16 chosenMove = moveInfo->moves[chosenMoveId];

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
				if (moveInfo->possibleZMoves[chosenMoveId]) //Checked first b/c Rayquaza can do all 3
				{
					if (ShouldAIUseZMove(gActiveBattler, gBankTarget, moveInfo->moves[chosenMoveId]))
						gNewBS->zMoveData.toBeUsed[gActiveBattler] = TRUE;
				}
				else if (moveInfo->canMegaEvolve)
				{
					if (!ShouldAIDelayMegaEvolution(gActiveBattler, gBankTarget, chosenMove))
					{
						if (moveInfo->megaVariance != MEGA_VARIANT_ULTRA_BURST)
							gNewBS->megaData.chosen[gActiveBattler] = TRUE;
						else if (moveInfo->megaVariance == MEGA_VARIANT_ULTRA_BURST)
							gNewBS->ultraData.chosen[gActiveBattler] = TRUE;
					}
				}
				else if (moveInfo->possibleMaxMoves[chosenMoveId]) //Handles the "Can I Dynamax" checks
				{
					if (ShouldAIDynamax(gActiveBattler, gBankTarget))
						gNewBS->dynamaxData.toBeUsed[gActiveBattler] = TRUE;
				}

				//This is handled again later, but it's only here to help with the case of choosing Helping Hand when the partner is switching out.
				gBattleStruct->chosenMovePositions[gActiveBattler] = chosenMoveId;
				gBattleStruct->moveTarget[gActiveBattler] = gBankTarget;
				gChosenMovesByBanks[gActiveBattler] = chosenMove;
				TryRemoveDoublesKillingScore(gActiveBattler, gBankTarget, chosenMove);

				EmitMoveChosen(1, chosenMoveId, gBankTarget, gNewBS->megaData.chosen[gActiveBattler], gNewBS->ultraData.chosen[gActiveBattler], gNewBS->zMoveData.toBeUsed[gActiveBattler], gNewBS->dynamaxData.toBeUsed[gActiveBattler]);
				TryRechoosePartnerMove(moveInfo->moves[chosenMoveId]);
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
			chosenMoveId = Random() & 3;
			move = moveInfo->moves[chosenMoveId];
		} while (move == MOVE_NONE);

		if (gBattleMoves[move].target & (MOVE_TARGET_USER_OR_PARTNER | MOVE_TARGET_USER))
			EmitMoveChosen(1, chosenMoveId, gActiveBattler, 0, 0, 0, FALSE);
		else if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
			EmitMoveChosen(1, chosenMoveId, GetBattlerAtPosition(Random() & 2), 0, 0, 0, FALSE);
		else
			EmitMoveChosen(1, chosenMoveId, FOE(gActiveBattler), 0, 0, 0, FALSE);

		OpponentBufferExecCompleted();
	}
}

#define STATE_BEFORE_ACTION_CHOSEN 0
static void TryRechoosePartnerMove(u16 chosenMove)
{
	if (GetBattlerPosition(gActiveBattler) & BIT_FLANK) //Second to choose action on either side
	{
		switch (gChosenMovesByBanks[PARTNER(gActiveBattler)]) {
			case MOVE_HELPINGHAND:
				if (chosenMove == MOVE_NONE || SPLIT(chosenMove) == SPLIT_STATUS)
				{
					struct ChooseMoveStruct moveInfo;
					gChosenMovesByBanks[gActiveBattler] = chosenMove;

					u8 backup = gActiveBattler;
					gActiveBattler = PARTNER(gActiveBattler);
					EmitChooseMove(0, (gBattleTypeFlags & BATTLE_TYPE_DOUBLE) != 0, FALSE, &moveInfo); //Rechoose partner move
					MarkBufferBankForExecution(gActiveBattler);
					gActiveBattler = backup;
				}
				break;
		}
	}
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
	gSprites[gBattlerSpriteIds[gActiveBattler]].callback = sub_8033EEC;

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
	gSprites[gBattlerSpriteIds[gActiveBattler]].callback = sub_8033EEC;

	gBattlerControllerFuncs[gActiveBattler] = CompleteOnBankSpriteCallbackDummy2;
}

void OpponentHandleChoosePokemon(void)
{
	u8 chosenMonId;

	if (gBattleStruct->switchoutIndex[SIDE(gActiveBattler)] == PARTY_SIZE)
	{
		u8 battlerIn1, battlerIn2, firstId, lastId;
		struct Pokemon* party = LoadPartyRange(gActiveBattler, &firstId, &lastId);

		if (gNewBS->ai.bestMonIdToSwitchInto[gActiveBattler][0] == PARTY_SIZE
		||  GetMonData(&party[gNewBS->ai.bestMonIdToSwitchInto[gActiveBattler][0]], MON_DATA_HP, NULL) == 0) //Best mon is dead
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
		if (gBattleTypeFlags & (BATTLE_TYPE_TWO_OPPONENTS | BATTLE_TYPE_TOWER_LINK_MULTI))
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

        sub_804BD94(sprite->sBattler);
        SetHealthboxSpriteVisible(gHealthboxSpriteIds[sprite->sBattler]);
        sprite->callback = SpriteCallbackDummy;
        StartSpriteAnimIfDifferent(sprite, 0);
        if (!BeginNormalPaletteFade(selectedPalettes, 0, 10, 0, RGB(8, 8, 8))) //If a fade is already in progress,
			gPaletteFade_selectedPalettes |= selectedPalettes; //Then add second mon in wild doubles to the palettes to unfade
    }
}
#undef sBattler
