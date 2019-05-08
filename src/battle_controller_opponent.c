#include "defines.h"
#include "defines_battle.h"
#include "Battle_AI/AI_Helper_Functions.h"

#include "../include/event_data.h"
#include "../include/random.h"

#include "../include/new/battle_controller_opponent.h"
#include "../include/new/frontier.h"
#include "../include/new/helper_functions.h"
#include "../include/new/mega.h"

//TODO: Update Acupressure Targeting

extern u8 GetMostSuitableMonToSwitchInto(void);
extern void BattleAI_SetupAIData(u8 defaultScoreMoves);
//extern BattleAI_ChooseMoveOrAction;
extern u32 WildMonIsSmart(u8 bank);
extern void EmitMoveChosen(u8 bufferId, u8 chosenMoveIndex, u8 target, u8 megaState, u8 ultraState, u8 zMoveState);

//This file's functions:
static u8 LoadCorrectTrainerPicId(void);

void OpponentHandleChooseMove(void)
{
    u8 chosenMoveId;
    struct ChooseMoveStruct* moveInfo = (struct ChooseMoveStruct*)(&gBattleBufferA[gActiveBattler][4]);

    if ((gBattleTypeFlags & (BATTLE_TYPE_TRAINER | BATTLE_TYPE_OAK_TUTORIAL | BATTLE_TYPE_SAFARI | BATTLE_TYPE_ROAMER))
	||   FlagGet(SMART_WILD_FLAG)
	||	 WildMonIsSmart(gActiveBattler))
    {
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
			
			default:
				if (gBattleMoves[moveInfo->moves[chosenMoveId]].target & (MOVE_TARGET_USER_OR_SELECTED | MOVE_TARGET_USER))
					gBankTarget = gActiveBattler;
				if (gBattleMoves[moveInfo->moves[chosenMoveId]].target & MOVE_TARGET_BOTH)
				{
					if (SIDE(gActiveBattler) == B_SIDE_PLAYER) {
						gBankTarget = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
						if (gAbsentBattlerFlags & gBitTable[gBankTarget])
							gBankTarget = GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT);
					}
					else { 
						gBankTarget = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
						if (gAbsentBattlerFlags & gBitTable[gBankTarget])
							gBankTarget = GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT);
					}
				}
				if (moveInfo->possibleZMoves[chosenMoveId])
					gNewBS->ZMoveData->toBeUsed[gActiveBattler] = TRUE;
				else if (moveInfo->canMegaEvolve && moveInfo->megaVariance != MEGA_VARIANT_ULTRA_BURST)
					gNewBS->MegaData->chosen[gActiveBattler] = TRUE;
				else if (moveInfo->canMegaEvolve && moveInfo->megaVariance == MEGA_VARIANT_ULTRA_BURST)
					gNewBS->UltraData->chosen[gActiveBattler] = TRUE;
				EmitMoveChosen(1, chosenMoveId, gBankTarget, gNewBS->MegaData->chosen[gActiveBattler], gNewBS->UltraData->chosen[gActiveBattler], gNewBS->ZMoveData->toBeUsed[gActiveBattler]);
				break;
        }
        OpponentBufferExecCompleted();
    }
	
    else
    {
        u16 move;
        do
        {
            chosenMoveId = Random() & 3;
            move = moveInfo->moves[chosenMoveId];
        } while (move == MOVE_NONE);

        if (gBattleMoves[move].target & (MOVE_TARGET_USER_OR_SELECTED | MOVE_TARGET_USER))
			EmitMoveChosen(1, chosenMoveId, gActiveBattler, 0, 0, 0);
        else if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
			EmitMoveChosen(1, chosenMoveId, GetBattlerAtPosition(Random() & 2), 0, 0, 0);
        else
			EmitMoveChosen(1, chosenMoveId, FOE(gActiveBattler), 0, 0, 0);

        OpponentBufferExecCompleted();
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
    SetMultiuseSpriteTemplateToTrainerBack(trainerPicId, GetBattlerPosition(gActiveBattler)); //0x803F864
    gBattlerSpriteIds[gActiveBattler] = CreateSprite(&gMultiuseSpriteTemplate[0], //0x8006F8C
                                               xPos,
                                               (8 - gTrainerFrontPicCoords[trainerPicId].coords) * 4 + 40,
                                               GetBattlerSpriteSubpriority(gActiveBattler)); //0x807685C

    gSprites[gBattlerSpriteIds[gActiveBattler]].pos2.x = -240;
    gSprites[gBattlerSpriteIds[gActiveBattler]].data[0] = 2;
    gSprites[gBattlerSpriteIds[gActiveBattler]].oam.paletteNum = IndexOfSpritePaletteTag(gTrainerFrontPicPaletteTable[trainerPicId].tag); //0x80089E8
	gSprites[gBattlerSpriteIds[gActiveBattler]].data[5] = gSprites[gBattlerSpriteIds[gActiveBattler]].oam.tileNum;
	gSprites[gBattlerSpriteIds[gActiveBattler]].oam.tileNum = GetSpriteTileStartByTag(gTrainerFrontPicTable[trainerPicId].tag); //0x8008804
    gSprites[gBattlerSpriteIds[gActiveBattler]].oam.affineParam = trainerPicId;
    gSprites[gBattlerSpriteIds[gActiveBattler]].callback = sub_8033EEC; //sub_805D7AC in Emerald

    gBattleBankFunc[gActiveBattler] = (u32) CompleteOnBattlerSpriteCallbackDummy; //0x8035AE8
}

void OpponentHandleTrainerSlide(void)
{	

    u32 trainerPicId = LoadCorrectTrainerPicId();
	
    DecompressTrainerFrontPic(trainerPicId, gActiveBattler);
    SetMultiuseSpriteTemplateToTrainerBack(trainerPicId, GetBattlerPosition(gActiveBattler));
    gBattlerSpriteIds[gActiveBattler] = CreateSprite(&gMultiuseSpriteTemplate[0], 176, (8 - gTrainerFrontPicCoords[trainerPicId].coords) * 4 + 40, 0x1E);

    gSprites[gBattlerSpriteIds[gActiveBattler]].pos2.x = 96;
    gSprites[gBattlerSpriteIds[gActiveBattler]].pos1.x += 32;
    gSprites[gBattlerSpriteIds[gActiveBattler]].data[0] = -2;
    gSprites[gBattlerSpriteIds[gActiveBattler]].oam.paletteNum = IndexOfSpritePaletteTag(gTrainerFrontPicPaletteTable[trainerPicId].tag);
    gSprites[gBattlerSpriteIds[gActiveBattler]].data[5] = gSprites[gBattlerSpriteIds[gActiveBattler]].oam.tileNum;
	gSprites[gBattlerSpriteIds[gActiveBattler]].oam.tileNum = GetSpriteTileStartByTag(gTrainerFrontPicTable[trainerPicId].tag);
	gSprites[gBattlerSpriteIds[gActiveBattler]].oam.affineParam = trainerPicId;
    gSprites[gBattlerSpriteIds[gActiveBattler]].callback = sub_8033EEC;

    gBattleBankFunc[gActiveBattler] = (u32) CompleteOnBankSpriteCallbackDummy2;
}

extern u32 break_helper(u32 a);
extern u32 break_helper2(u32 a);

void OpponentHandleChoosePokemon(void)
{
    u8 chosenMonId;
	
    if (gBattleStruct->switchoutIndex[SIDE(gActiveBattler)] == PARTY_SIZE)
    {
        chosenMonId = GetMostSuitableMonToSwitchInto();
		
        if (chosenMonId == PARTY_SIZE)
        {
            u8 battlerIn1, battlerIn2, firstId, lastId;
			
			pokemon_t* party = LoadPartyRange(gActiveBattler, &firstId, &lastId);

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

    gBattleStruct->monToSwitchIntoId[gActiveBattler] = chosenMonId;
    EmitChosenMonReturnValue(1, chosenMonId, 0);
    OpponentBufferExecCompleted();
}

static u8 LoadCorrectTrainerPicId(void) {
	u8 trainerPicId;
	
    if (gTrainerBattleOpponent_A == 0x400) //Was Secret Base in Ruby
    {
        trainerPicId = GetSecretBaseTrainerPicIndex();
    }
  /*else if (gTrainerBattleOpponent_A == TRAINER_FRONTIER_BRAIN)
    {
        trainerPicId = GetFrontierBrainTrainerPicIndex();
    }*/
    else if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER)
    {
        if (gBattleTypeFlags & (BATTLE_TYPE_TWO_OPPONENTS | BATTLE_TYPE_TOWER_LINK_MULTI))
        {
            if (gActiveBattler == 1)
                trainerPicId = GetFrontierTrainerFrontSpriteId(gTrainerBattleOpponent_A, 0);
            else
                trainerPicId = GetFrontierTrainerFrontSpriteId(VarGet(SECOND_OPPONENT_VAR), 1);
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
        if (gActiveBattler == 1)
			trainerPicId = gTrainers[gTrainerBattleOpponent_A].trainerPic;
        else
            trainerPicId = gTrainers[VarGet(SECOND_OPPONENT_VAR)].trainerPic;
    }
    else
    {
        trainerPicId = gTrainers[gTrainerBattleOpponent_A].trainerPic;
    }
	
	return trainerPicId;
}
