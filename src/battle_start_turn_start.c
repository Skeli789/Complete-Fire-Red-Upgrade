#include "defines.h"
#include "defines_battle.h"
#include "../include/event_data.h"
#include "../include/songs.h"

#include "../include/new/battle_start_turn_start.h"
#include "../include/new/helper_functions.h"
#include "../include/new/multi.h"
#include "../include/new/mega.h"
#include "../include/random.h"

extern void (* const sTurnActionsFuncsTable[])(void);
extern void (* const sEndTurnFuncsTable[])(void);
extern u8* const gBattleScriptsForMoveEffects[];
extern const bank_t gTargetsByBank[4][4];
extern const u16 gClassBasedBattleBGM[];
extern const u16 gWildSpeciesBasedBattleBGM[];

extern u8* DoMegaEvolution(u8 bank);
extern u8* DoPrimalReversion(bank_t, u8 caseId);
extern u8 GetMoveTypeSpecial(u8 bankAtk, move_t);
extern bank_t GetNextMultiTarget(void) ;
extern u8 ItemBattleEffects(u8 caseID, u8 bank, bool8 moveTurn, bool8 DoPluck);
extern void CreateMegaIndicatorAfterAnim(void);

extern u8 BattleScript_AirBalloonFloat[];
extern u8 BattleScript_Totem[];
extern u8 BattleScript_ElectricTerrainBattleBegin[];
extern u8 BattleScript_GrassyTerrainBattleBegin[];
extern u8 BattleScript_MistyTerrainBattleBegin[];
extern u8 BattleScript_PsychicTerrainBattleBegin[];
extern u8 BattleScript_BeakBlastSetUp[];
extern u8 BattleScript_ShellTrapSetUp[];
extern u8 BattleScript_FocusPunchSetUp[];
extern u8 BattleScript_QuickClaw[];
extern u8 BattleScript_NoTargetMoveFailed[];

u16 GetMUS_ForBattle(void);
u8 GetWhoStrikesFirst(bank_t, bank_t, bool8 ignoreMovePriorities);
s8 PriorityCalc(u8 bank, u8 action, u16 move);
s32 BracketCalc(u8 bank);
u32 SpeedCalc(u8 bank);
u32 SpeedCalcForParty(u8 side, pokemon_t*);

void HandleNewBattleRamClearBeforeBattle(void) {
	gNewBS = Calloc(sizeof(struct NewBattleStruct));
	gNewBS->MegaData = Calloc(sizeof(struct MegaData));
	gNewBS->UltraData = Calloc(sizeof(struct UltraData));
	gNewBS->ZMoveData = Calloc(sizeof(struct ZMoveData));
	Memset((u8*) 0x203D800, 0x0, 0x100);
	Memset((u8*) 0x203C020, 0x0, 0xE0);
}

void SavePartyItems(void) {
	u16* items = ExtensionState.itemBackup = Malloc(sizeof(item_t) * PARTY_SIZE);
	
	if (ExtensionState.itemBackup != NULL) {
		for (int i = 0; i < PARTY_SIZE; ++i)
			items[i] = gPlayerParty[i].item;
	}
}

void BattleBeginFirstTurn(void)
{
    s32 i;
    s32 j;
    u8 effect = 0;
	u8* state = &(gBattleStruct->switchInAbilitiesCounter);
	u8* bank = &(gBattleStruct->switchInItemsCounter);
	
    if (!gBattleExecBuffer) { //Inlclude Safari Check Here?
		switch(*state) {
			case GetTurnOrder:
				for (i = 0; i < gBattlersCount; ++i)
					gBanksByTurnOrder[i] = i;
				for (i = 0; i < gBattlersCount - 1; ++i) {
					for (j = i + 1; j < gBattlersCount; ++j) {
						if (GetWhoStrikesFirst(gBanksByTurnOrder[i], gBanksByTurnOrder[j], 1))
							SwapTurnOrder(i, j);
					}
				}
				
				//OW Weather
				if (gBattleStruct->field_B6 == 0 && AbilityBattleEffects(ABILITYEFFECT_ON_SWITCHIN, 0, 0, 0xFF, 0)) {
					gBattleStruct->field_B6 = 1;
					return;
				}
				
				//OW Terrain
				u8 req_terrain = VarGet(TERRAIN_VAR);
				if (req_terrain && TerrainType != req_terrain) {
					switch (req_terrain) {
						case ELECTRIC_TERRAIN:
							BattleScriptPushCursorAndCallback(BattleScript_ElectricTerrainBattleBegin);
							++effect;
							break;
						case GRASSY_TERRAIN:
							BattleScriptPushCursorAndCallback(BattleScript_GrassyTerrainBattleBegin);
							++effect;
							break;
						case MISTY_TERRAIN:
							BattleScriptPushCursorAndCallback(BattleScript_MistyTerrainBattleBegin);
							++effect;
							break;
						case PSYCHIC_TERRAIN:
							BattleScriptPushCursorAndCallback(BattleScript_PsychicTerrainBattleBegin);
							++effect;
					}
					if (effect) {
						TerrainType = req_terrain;
						return;
					}
					
				}
				
				//Primal Reversion
				while (*bank < gBattlersCount) {
					u8* script = DoPrimalReversion(gBanksByTurnOrder[*bank], 0);
					if(script) {
						BattleScriptPushCursorAndCallback(script);
						gBattleScripting->bank = gBanksByTurnOrder[*bank];
						gBankAttacker = gBanksByTurnOrder[*bank];
						++effect;
					}
					++*bank;
					
					if (effect) return;
				}
				++*state;
				break;
			
			case ThirdTypeRemoval:
				for (*bank = 0; *bank < gBattlersCount; ++*bank)
					gBattleMons[*bank].type3 =  TYPE_BLANK;
					
				*bank = 0;
				++*state;
			
			__attribute__ ((fallthrough));
			case SwitchInAbilities:
				while (*bank < gBattlersCount) {
					if (AbilityBattleEffects(ABILITYEFFECT_ON_SWITCHIN, gBanksByTurnOrder[*bank], 0, 0, 0))
						effect++;
					++*bank;
					
					if (effect) return;
				}
				++*state;
				break;
				
			case Intimidate:
				if (AbilityBattleEffects(ABILITYEFFECT_INTIMIDATE1, 0, 0, 0, 0))
					return;
				if (AbilityBattleEffects(ABILITYEFFECT_INTIMIDATE2, 0, 0, 0, 0))
					return;
					
				*bank = 0; //Reset Bank for next loop
				++*state;
				break;
				
			case AmuletCoin_WhiteHerb:
				while (*bank < gBattlersCount) {
					if (ItemBattleEffects(ItemEffects_SwitchIn, gBanksByTurnOrder[*bank], FALSE, FALSE))
						++effect;
					++*bank;
					
					if (effect) return;
				}
				*bank = 0; //Reset Bank for next loop
				++*state;
				break;
				
			case AirBalloon:
				while (*bank < gBattlersCount) {
					if (GetBankItemEffect(gBanksByTurnOrder[*bank]) == ITEM_EFFECT_AIR_BALLOON) {
						BattleScriptPushCursorAndCallback(BattleScript_AirBalloonFloat);
						gBattleScripting->bank = gBanksByTurnOrder[*bank];
						gBankAttacker = gBanksByTurnOrder[*bank];
						++effect;
					}
					++*bank;
					
					if (effect) return;
				}
				*bank = 0; //Reset Bank for next loop
				++*state;
				break;
			
			case TotemPokemon:	;
				if (!(gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_FRONTIER)))
				{
					u16 totem_var = VarGet(TOTEM_VAR);
					u16 stat_to_raise = VarGet(TOTEM_VAR + 1 + *bank);
					if (totem_var) {
						while (*bank < gBattlersCount) {
							u8 amount_to_raise = stat_to_raise & ~(0x7);
							if ((totem_var & (1 << *bank)) && stat_to_raise > 0 && 
								 stat_to_raise <= STAT_STAGE_EVASION && 
								((amount_to_raise >= 0x10 && amount_to_raise <= 0x60) || 
								 (amount_to_raise >= 0x90 && amount_to_raise <= 0xE0))) {
									gStatChangeByte = stat_to_raise | amount_to_raise;
									BattleScriptPushCursorAndCallback(BattleScript_Totem);
									gBankAttacker = gBattleScripting->bank = *bank;
									++effect;
								}
							++*bank;
							
							if (effect) return;
						}
					}
					
					//Reset Totem Vars
					VarSet(TOTEM_VAR, 0);		//Totem Banks
					VarSet(TOTEM_VAR + 1, 0);	//Bank 0's Stat
					VarSet(TOTEM_VAR + 2, 0);	//Bank 1's Stat
					VarSet(TOTEM_VAR + 3, 0);	//Bank 2's Stat
					VarSet(TOTEM_VAR + 4, 0);	//Bank 3's Stat
				}
				*bank = 0; //Reset Bank for next loop
				++*state;
				break;
				
			case StartTurnEnd:
				for (i = 0; i < 4; i++)
				{
					gBattleStruct->monToSwitchIntoId[i] = 6;
					gActionForBanks[i] = 0xFF;
					gChosenMovesByBanks[i] = 0;
				}
				TurnValuesCleanUp(0);
				SpecialStatusesClear();
				gBattleStruct->field_91 = gAbsentBattlerFlags;
				gBattleMainFunc = BC_Menu;
				ResetSentPokesToOpponentValue();
				for (i = 0; i < 8; i++)
					gBattleCommunication[i] = 0;
				for (i = 0; i < gBattlersCount; i++)
				{
					gBattleMons[i].status2 &= ~8;
					gNewBS->pickupStack[i] = 0xFF;
				}
				gBattleStruct->turnEffectsTracker = 0;
				gBattleStruct->turnEffectsBank = 0;
				gBattleStruct->field_180 = 0;
				gBattleStruct->field_181 = 0;
				gBattleScripting->atk49_state = 0;
				gBattleStruct->faintedActionsState = 0;
				gBattleStruct->turncountersTracker = 0;
				gMoveResultFlags = 0;
				gRandomTurnNumber = Random();
				*state = 0;
		}
    }
}


void SetActionsAndBanksTurnOrder(void)
{
    s32 turnOrderId = 0;
    s32 i, j;

    if (gBattleTypeFlags & BATTLE_TYPE_SAFARI)
    {
        for (gActiveBattler = 0; gActiveBattler < gBattlersCount; ++gActiveBattler) {
            gActionsByTurnOrder[turnOrderId] = gActionForBanks[gActiveBattler];
            gBanksByTurnOrder[turnOrderId] = gActiveBattler;
            ++turnOrderId;
        }
    }
    else
    {
        if (gBattleTypeFlags & BATTLE_TYPE_LINK)
        {
            for (gActiveBattler = 0; gActiveBattler < gBattlersCount; ++gActiveBattler) {
                if (gActionForBanks[gActiveBattler] == ACTION_RUN)
                {
                    turnOrderId = 5;
                    break;
                }
            }
        }
        else
        {
            if (gActionForBanks[0] == ACTION_RUN) {
                gActiveBattler = 0;
                turnOrderId = 5;
            }
			
            if (gActionForBanks[2] == ACTION_RUN) {
                gActiveBattler = 2;
                turnOrderId = 5;
            }
        }

        if (turnOrderId == 5)
        {
            gActionsByTurnOrder[0] = gActionForBanks[gActiveBattler];
            gBanksByTurnOrder[0] = gActiveBattler;
            turnOrderId = 1;
            for (i = 0; i < gBattlersCount; i++)
            {
                if (i != gActiveBattler)
                {
                    gActionsByTurnOrder[turnOrderId] = gActionForBanks[i];
                    gBanksByTurnOrder[turnOrderId] = i;
                    ++turnOrderId;
                }
            }
            gBattleMainFunc = (u32) CheckFocusPunch_ClearVarsBeforeTurnStarts;
            gBattleStruct->focusPunchBank = 0;
            return;
        }
        else
        {
            for (gActiveBattler = 0; gActiveBattler < gBattlersCount; ++gActiveBattler)
            {
                if (gActionForBanks[gActiveBattler] == ACTION_USE_ITEM || gActionForBanks[gActiveBattler] == ACTION_SWITCH)
                {
                    gActionsByTurnOrder[turnOrderId] = gActionForBanks[gActiveBattler];
                    gBanksByTurnOrder[turnOrderId] = gActiveBattler;
                    ++turnOrderId;
                }
            }
            for (gActiveBattler = 0; gActiveBattler < gBattlersCount; gActiveBattler++)
            {
                if (gActionForBanks[gActiveBattler] != ACTION_USE_ITEM && gActionForBanks[gActiveBattler] != ACTION_SWITCH)
                {
                    gActionsByTurnOrder[turnOrderId] = gActionForBanks[gActiveBattler];
                    gBanksByTurnOrder[turnOrderId] = gActiveBattler;
                    ++turnOrderId;
                }
            }
            for (i = 0; i < gBattlersCount - 1; i++)
            {
                for (j = i + 1; j < gBattlersCount; j++)
                {
                    u8 bank1 = gBanksByTurnOrder[i];
                    u8 bank2 = gBanksByTurnOrder[j];
                    if (gActionsByTurnOrder[i] != ACTION_USE_ITEM
                        && gActionsByTurnOrder[j] != ACTION_USE_ITEM
                        && gActionsByTurnOrder[i] != ACTION_SWITCH
                        && gActionsByTurnOrder[j] != ACTION_SWITCH)
                    {
                        if (GetWhoStrikesFirst(bank1, bank2, FALSE))
                            SwapTurnOrder(i, j);
                    }
                }
            }
        }
    }

    gBattleMainFunc = (u32) CheckFocusPunch_ClearVarsBeforeTurnStarts;
    gBattleStruct->focusPunchBank = 0;
}

enum MegaStates 
{
	Mega_Check,
	Mega_CalcTurnOrder,
	Mega_SwitchInAbilities,
	Mega_Intimidate,
	Mega_End
};

void RunTurnActionsFunctions(void)
{
	u8 effect;
	int i, j;
	u8* megaBank = &(gNewBS->MegaData->activeBank);
	
    if (gBattleOutcome != 0)
        gCurrentActionFuncId = ACTION_FINISHED;
	
	if (gCurrentActionFuncId == ACTION_USE_MOVE)
	{
		switch (gNewBS->MegaData->state) {
			case Mega_Check:
				for (i = *megaBank; i < gBattlersCount; ++i, ++*megaBank) {
					u8 bank = gBanksByTurnOrder[i];
					gActiveBattler = bank;
					if (gNewBS->MegaData->chosen[bank] && !gNewBS->MegaData->done[bank] && !(gNewBS->ZMoveData->partyIndex[SIDE(bank)] & gBitTable[gBattlerPartyIndexes[bank]])) {
						u8* script = DoMegaEvolution(bank);
						if (script != NULL) 
						{	
							gNewBS->MegaData->chosen[bank] = 0;
							gNewBS->MegaData->done[bank] = TRUE;
							gNewBS->MegaData->megaEvoInProgress = TRUE;
							gNewBS->MegaData->script = script;
							if (!(gBattleTypeFlags & (BATTLE_TYPE_INGAME_PARTNER | BATTLE_TYPE_MULTI)) 
							&& SIDE(bank) == B_SIDE_PLAYER) 
							{
								gNewBS->MegaData->chosen[PARTNER(bank)] = 0;
								gNewBS->MegaData->done[PARTNER(bank)] = TRUE;
							}
							else if (!(gBattleTypeFlags & (BATTLE_TYPE_TWO_OPPONENTS | BATTLE_TYPE_MULTI)) 
							&& SIDE(bank) == B_SIDE_OPPONENT) 
							{
								gNewBS->MegaData->chosen[PARTNER(bank)] = 0;
								gNewBS->MegaData->done[PARTNER(bank)] = TRUE;
							}
							BattleScriptExecute(gNewBS->MegaData->script);
							return;
						}
					}
					else if (gNewBS->UltraData->chosen[bank] && !gNewBS->UltraData->done[bank]) {
						u8* script = DoMegaEvolution(bank);
						if (script != NULL) 
						{	
							gNewBS->UltraData->chosen[bank] = 0;
							gNewBS->UltraData->done[bank] = TRUE;
							gNewBS->MegaData->megaEvoInProgress = TRUE;
							gNewBS->MegaData->script = script;
							if (!(gBattleTypeFlags & (BATTLE_TYPE_INGAME_PARTNER | BATTLE_TYPE_MULTI)) 
							&& SIDE(bank) == B_SIDE_PLAYER) 
							{
								gNewBS->UltraData->chosen[PARTNER(bank)] = 0;
								gNewBS->UltraData->done[PARTNER(bank)] = TRUE;
							}
							else if (!(gBattleTypeFlags & (BATTLE_TYPE_TWO_OPPONENTS | BATTLE_TYPE_MULTI)) 
							&& SIDE(bank) == B_SIDE_OPPONENT) 
							{
								gNewBS->UltraData->chosen[PARTNER(bank)] = 0;
								gNewBS->UltraData->done[PARTNER(bank)] = TRUE;
							}
							BattleScriptExecute(gNewBS->MegaData->script);
							return;
						}
					}
				}
				if (gNewBS->MegaData->megaEvoInProgress)
					++gNewBS->MegaData->state;
				else
					gNewBS->MegaData->state = Mega_End;
				return;
			
			case Mega_CalcTurnOrder:
				for (i = 0; i < gBattlersCount - 1; ++i)
				{
					for (j = i + 1; j < gBattlersCount; ++j)
					{
						u8 bank1 = gBanksByTurnOrder[i];
						u8 bank2 = gBanksByTurnOrder[j];
						if (gActionsByTurnOrder[i] != ACTION_USE_ITEM
							&& gActionsByTurnOrder[j] != ACTION_USE_ITEM
							&& gActionsByTurnOrder[i] != ACTION_SWITCH
							&& gActionsByTurnOrder[j] != ACTION_SWITCH
							&& gActionsByTurnOrder[i] != ACTION_FINISHED
							&& gActionsByTurnOrder[j] != ACTION_FINISHED)
						{
							if (GetWhoStrikesFirst(bank1, bank2, FALSE))
								SwapTurnOrder(i, j);
						}
					}
				}
				*megaBank = 0; //Reset the bank for the next loop
				++gNewBS->MegaData->state;
				return;
			
			case Mega_SwitchInAbilities:
				while (*megaBank < gBattlersCount) {
					if (AbilityBattleEffects(ABILITYEFFECT_ON_SWITCHIN, gBanksByTurnOrder[*megaBank], 0, 0, 0))
						++effect;
					++*megaBank;
						
					if (effect) return;
				}
				*megaBank = 0;
				++gNewBS->MegaData->state;
				return;
					
			case Mega_Intimidate:
				if (AbilityBattleEffects(ABILITYEFFECT_INTIMIDATE1, 0, 0, 0, 0))
					return;
				if (AbilityBattleEffects(ABILITYEFFECT_INTIMIDATE2, 0, 0, 0, 0))
					return;
						
				gNewBS->MegaData->script = 0;
				gNewBS->MegaData->state = Mega_End;
				gNewBS->MegaData->activeBank = 0;
				gNewBS->MegaData->megaEvoInProgress = FALSE;
		}
		
		*megaBank = 0;
	
		while (gBattleStruct->focusPunchBank < gBattlersCount)
		{
			gActiveBattler = gBanksByTurnOrder[gBattleStruct->focusPunchBank];
			++gBattleStruct->focusPunchBank;
			u16 chosenMove = gChosenMovesByBanks[gActiveBattler];
			if ((chosenMove == MOVE_FOCUSPUNCH || chosenMove == MOVE_BEAKBLAST || chosenMove == MOVE_SHELLTRAP)
			&& !(gBattleMons[gActiveBattler].status1 & STATUS1_SLEEP)
			&& !(gDisableStructs[gActiveBattler].truantCounter)
			&& !(gProtectStructs[gActiveBattler].onlyStruggle)) //or onlyStruggle in Emerald
			{
				gBankAttacker = gBattleScripting->bank = gActiveBattler;
				if (chosenMove == MOVE_BEAKBLAST) {
					gNewBS->BeakBlastByte |= gBitTable[gActiveBattler];
					BattleScriptExecute(BattleScript_BeakBlastSetUp);
				}
				else if (chosenMove == MOVE_SHELLTRAP)
				{
					gNewBS->playedShellTrapMessage |= gBitTable[gActiveBattler];
					BattleScriptExecute(BattleScript_ShellTrapSetUp);
				}
				else
				{
					gNewBS->playedFocusPunchMessage |= gBitTable[gActiveBattler];
					BattleScriptExecute(BattleScript_FocusPunchSetUp);
				}
				return;
			}
		}
		
		//Ofiically this goes before the Mega, but I think this is better.
		u8 atkBank = gBanksByTurnOrder[gCurrentTurnActionNumber];
		if (gNewBS->CustapQuickClawIndicator & gBitTable[atkBank]) 
		{
			gNewBS->CustapQuickClawIndicator &= ~(gBitTable[atkBank]);
			gBattleScripting->bank = atkBank;
			gLastUsedItem = gBattleMons[atkBank].item;
			if (ITEM_EFFECT(atkBank) != ITEM_EFFECT_CUSTAP_BERRY)
				RecordItemBattle(atkBank, ITEM_EFFECT(atkBank));
			BattleScriptExecute(BattleScript_QuickClaw);
			return;
		}
	}	
	
    gBattleStruct->savedTurnActionNumber = gCurrentTurnActionNumber;
    sTurnActionsFuncsTable[gCurrentActionFuncId]();

    if (gCurrentTurnActionNumber >= gBattlersCount) // everyone did their actions, turn finished
    {
        gHitMarker &= ~(HITMARKER_NON_ATTACK_DMG);
        gBattleMainFunc = (u32) sEndTurnFuncsTable[gBattleOutcome & 0x7F];
    }
    else
    {
        if (gBattleStruct->savedTurnActionNumber != gCurrentTurnActionNumber) // action turn has been done, clear hitmarker bits for another bank
        {
            gHitMarker &= ~(HITMARKER_NO_ATTACKSTRING);
            gHitMarker &= ~(HITMARKER_UNABLE_TO_USE_MOVE);
        }
    }
}

void HandleAction_UseMove(void)
{
    u8 side;
	u8 moveType;
	int i;

    gBankAttacker = gBanksByTurnOrder[gCurrentTurnActionNumber];

    if (gBattleStruct->field_91 & gBitTable[gBankAttacker])
    {
        gCurrentActionFuncId = ACTION_FINISHED;
        return;
    }

    gCritMultiplier = 100;
    gBattleScripting->dmgMultiplier = 1;
    gBattleStruct->atkCancellerTracker = 0;
    gMoveResultFlags = 0;
    gMultiHitCounter = 0;
	gNewBS->OriginalAttackerTargetCount = 0;
	gNewBS->ParentalBondOn = FALSE;
	gNewBS->DancerInProgress = FALSE;
	gNewBS->MoveBounceInProgress = FALSE;
	gNewBS->ZMoveData->active = FALSE;
	gBattleCommunication[MOVE_EFFECT_BYTE] = 0; //Remove secondary effects
    gBattleCommunication[6] = 0;
    gCurrMovePos = gChosenMovePos = gBattleStruct->chosenMovePositions[gBankAttacker];

//Get Move to be Used
    if (gProtectStructs[gBankAttacker].onlyStruggle)
    {
        gProtectStructs[gBankAttacker].onlyStruggle = 0;
        gCurrentMove = gChosenMove = MOVE_STRUGGLE;
        gHitMarker |= HITMARKER_NO_PPDEDUCT;
        gBattleStruct->moveTarget[gBankAttacker] = GetMoveTarget(MOVE_STRUGGLE, 0);
    }
	else if (gBattleMons[gBankAttacker].status2 & STATUS2_RECHARGE)
	{
		gCurrentMove = gChosenMove = gLockedMoves[gBankAttacker];
	}
    else if (gBattleMons[gBankAttacker].status2 & STATUS2_MULTIPLETURNS)
    {
		gCurrentMove = gChosenMove = gLockedMoves[gBankAttacker];
		
		if (FindMovePositionInMoveset(gLockedMoves[gBankAttacker], gBankAttacker) == 4) //The Pokemon doesn't know the move it's locked into
		{
			CancelMultiTurnMoves(gBankAttacker);
			gBattleStruct->dynamicMoveType = GetMoveTypeSpecial(gBankAttacker, gCurrentMove);
			gBankTarget = gBattleStruct->moveTarget[gBankAttacker];
			gBattlescriptCurrInstr = BattleScript_NoTargetMoveFailed;
			gCurrentActionFuncId = ACTION_RUN_BATTLESCRIPT;
			return;
		}
    }
    // Encore forces you to use the same move
    else if (gDisableStructs[gBankAttacker].encoredMove != MOVE_NONE
          && gDisableStructs[gBankAttacker].encoredMove == gBattleMons[gBankAttacker].moves[gDisableStructs[gBankAttacker].encoredMovePos]
		  && !gNewBS->ZMoveData->toBeUsed[gBankAttacker]) //If a Z-Move was chosen, it can still be used
    {
        gCurrentMove = gChosenMove = gDisableStructs[gBankAttacker].encoredMove;
        gCurrMovePos = gChosenMovePos = gDisableStructs[gBankAttacker].encoredMovePos;
        gBattleStruct->moveTarget[gBankAttacker] = GetMoveTarget(gCurrentMove, 0);
    }
    // Check if the encored move wasn't overwritten
    else if (gDisableStructs[gBankAttacker].encoredMove != MOVE_NONE
          && gDisableStructs[gBankAttacker].encoredMove != gBattleMons[gBankAttacker].moves[gDisableStructs[gBankAttacker].encoredMovePos]
		  && !gNewBS->ZMoveData->toBeUsed[gBankAttacker]) //If a Z-Move was chosen, it can still be used
    {
        gCurrMovePos = gChosenMovePos = gDisableStructs[gBankAttacker].encoredMovePos;
        gCurrentMove = gChosenMove = gBattleMons[gBankAttacker].moves[gCurrMovePos];
        gDisableStructs[gBankAttacker].encoredMove = MOVE_NONE;
        gDisableStructs[gBankAttacker].encoredMovePos = 0;
        gDisableStructs[gBankAttacker].encoreTimer = 0;
        gBattleStruct->moveTarget[gBankAttacker] = GetMoveTarget(gCurrentMove, 0);
    }
    else if (gBattleMons[gBankAttacker].moves[gCurrMovePos] != gChosenMovesByBanks[gBankAttacker])
    {
        gCurrentMove = gChosenMove = gBattleMons[gBankAttacker].moves[gCurrMovePos];
        gBattleStruct->moveTarget[gBankAttacker] = GetMoveTarget(gCurrentMove, 0);
    }
    else
    {
        gCurrentMove = gChosenMove = gBattleMons[gBankAttacker].moves[gCurrMovePos];
    }

    if (gBattleMons[gBankAttacker].hp)
    {
        if (SIDE(gBankAttacker) == B_SIDE_PLAYER)
            gBattleResults->lastUsedMovePlayer = gCurrentMove;
        else
            gBattleResults->lastUsedMoveOpponent = gCurrentMove;
    }
	
	if (gNewBS->ZMoveData->toBeUsed[gBankAttacker]/* && !gNewBS->ZMoveData->used[gBankAttacker]*/) {
		gNewBS->ZMoveData->active = TRUE;
		
		if (SPLIT(gCurrentMove) != SPLIT_STATUS) 
		{
			for (i = 0; SpecialZMoveTable[i].item != 0xFFFF; ++i) 
			{
				if (SpecialZMoveTable[i].item == ITEM(gBankAttacker) //No need to check for correct species here as the check;
				&&  SpecialZMoveTable[i].move == gCurrentMove)		 //it should already have been carried out during move selection.
				{
					gCurrentMove = SpecialZMoveTable[i].zmove;
					goto SKIP_SELECT_REGULAR_Z_MOVE;
				}
			}
							
			if (SpecialZMoveTable[i].item == 0xFFFF) { //No special Z-Move
				u16 moveReplaced = gBattleMons[gBankAttacker].moves[gCurrMovePos];
				u8 moveType = gBattleMoves[moveReplaced].type;
				if (moveType < TYPE_FIRE)
					gCurrentMove = MOVE_BREAKNECK_BLITZ_P + (moveType * 2) + SPLIT(moveReplaced);
				else
					gCurrentMove = MOVE_BREAKNECK_BLITZ_P + ((moveType - 1) * 2) + SPLIT(moveReplaced);
			}
		}
	}
	
	SKIP_SELECT_REGULAR_Z_MOVE:
	
	gBattleStruct->dynamicMoveType = GetMoveTypeSpecial(gBankAttacker, gCurrentMove);
	moveType = gBattleStruct->dynamicMoveType;
	
//Get Move Target
    side = SIDE(gBankAttacker) ^ BIT_SIDE;
	bank_t selectedTarget = gBattleStruct->moveTarget[gBankAttacker];
	
    if (gSideTimers[side].followmeTimer != 0
    && gBattleMoves[gCurrentMove].target == MOVE_TARGET_SELECTED
    && SIDE(gBankAttacker) != SIDE(gSideTimers[side].followmeTarget)
    && gBattleMons[gSideTimers[side].followmeTarget].hp != 0
	&& gCurrentMove != MOVE_SKYDROP)
    {
        gBankTarget = gSideTimers[side].followmeTarget;
    }
    else if ((gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
          &&  gSideTimers[side].followmeTimer == 0
          && (SPLIT(gCurrentMove) != SPLIT_STATUS || gBattleMoves[gCurrentMove].target != MOVE_TARGET_USER)
		  && !(gBattleMoves[gCurrentMove].target & (MOVE_TARGET_ALL | MOVE_TARGET_BOTH)))
    { //Try Ability Redirection
		switch (moveType) {
			case TYPE_WATER:
				if (ABILITY(selectedTarget) != ABILITY_STORMDRAIN) 
				{
					if (ABILITY(SIDE(gBankAttacker) ^ BIT_SIDE) == ABILITY_STORMDRAIN)
					{
						gBankTarget = SIDE(gBankAttacker) ^ BIT_SIDE;
						gSpecialStatuses[gBankTarget].lightningRodRedirected = 1;
					}
					else if (ABILITY(PARTNER(SIDE(gBankAttacker) ^ BIT_SIDE)) == ABILITY_STORMDRAIN)
					{
						gBankTarget = PARTNER(SIDE(gBankAttacker) ^ BIT_SIDE);
						gSpecialStatuses[gBankTarget].lightningRodRedirected = 1;
					}
					else if (ABILITY(PARTNER(gBankAttacker)) == ABILITY_STORMDRAIN)
					{
						gBankTarget = PARTNER(gBankAttacker);
						gSpecialStatuses[gBankTarget].lightningRodRedirected = 1;
					}
				}
				
				break;
						
			case TYPE_ELECTRIC:
				if (ABILITY(selectedTarget) != ABILITY_LIGHTNINGROD) 
				{
					if (ABILITY(SIDE(gBankAttacker) ^ BIT_SIDE) == ABILITY_LIGHTNINGROD)
					{
						gBankTarget = SIDE(gBankAttacker) ^ BIT_SIDE;
						gSpecialStatuses[gBankTarget].lightningRodRedirected = 1;
					}
					else if (ABILITY(PARTNER(SIDE(gBankAttacker) ^ BIT_SIDE)) == ABILITY_LIGHTNINGROD)
					{
						gBankTarget = PARTNER(SIDE(gBankAttacker) ^ BIT_SIDE);
						gSpecialStatuses[gBankTarget].lightningRodRedirected = 1;
					}
					else if (ABILITY(PARTNER(gBankAttacker)) == ABILITY_LIGHTNINGROD)
					{
						gBankTarget = PARTNER(gBankAttacker);
						gSpecialStatuses[gBankTarget].lightningRodRedirected = 1;
					}
				}
				break;
		}
		
		if (!gSpecialStatuses[gBankTarget].lightningRodRedirected)
		{
			if (gBattleMoves[gChosenMove].target & MOVE_TARGET_RANDOM)
				goto CHOOSE_RANDOM_TARGET_DOUBLES;
			else
				goto CHOOSE_REGULAR_TARGET_DOUBLES;
		}
    }
    else if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE
          && gBattleMoves[gChosenMove].target & MOVE_TARGET_RANDOM)
	{
	CHOOSE_RANDOM_TARGET_DOUBLES:
        if (SIDE(gBankAttacker) == B_SIDE_PLAYER)
        {
            if (Random() & 1)
                gBankTarget = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
            else
                gBankTarget = GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT);
        }
        else
        {
            if (Random() & 1)
                gBankTarget = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
            else
                gBankTarget = GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT);
        }

        if (gAbsentBattlerFlags & gBitTable[gBankTarget]
        && SIDE(gBankAttacker) != SIDE(gBankTarget))
        {
            gBankTarget = GetBattlerAtPosition(PARTNER(gBankTarget));
        }
    }
	else if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE
		 && gBattleMoves[gCurrentMove].target & MOVE_TARGET_ALL)
	{
		while ((gBankTarget = GetNextMultiTarget()) != 0xFF && gBattleMons[gBankTarget].hp == 0)
		{
			++gNewBS->OriginalAttackerTargetCount;
		}
		
		if (gBankTarget == 0xFF) //No targets left
			gBankTarget = FOE(gBankAttacker); //Doesn't matter who, as long as not attacker
	}
    else if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
    {
	CHOOSE_REGULAR_TARGET_DOUBLES:
        gBankTarget = selectedTarget;
        if (gAbsentBattlerFlags & gBitTable[gBankTarget])
        {
            if (SIDE(gBankAttacker) != SIDE(gBankTarget))
            {
                gBankTarget = PARTNER(gBankTarget);
            }
            else //Targeted Partner
            {
                gBankTarget = GetBattlerAtPosition(GetBattlerPosition(gBankAttacker) ^ BIT_SIDE);
                if (gAbsentBattlerFlags & gBitTable[gBankTarget])
                    gBankTarget = PARTNER(gBankTarget);
            }
        }
    }
	else
		gBankTarget = selectedTarget;

    // choose battlescript
	if (gStatuses3[gBankAttacker] & STATUS3_SKY_DROP_ATTACKER
	&& gBattleMons[gNewBS->skyDropAttackersTarget[gBankAttacker]].hp == 0)
	{
		gStatuses3[gBankAttacker] &= ~(STATUS3_SKY_DROP_ATTACKER | STATUS3_SKY_DROP_TARGET | STATUS3_IN_AIR);
		gNewBS->skyDropTargetsAttacker[gBankTarget] = 0;
		gNewBS->skyDropAttackersTarget[gBankAttacker] = 0;
		gBattlescriptCurrInstr = BattleScript_NoTargetMoveFailed;
	}
	else if (gBattleMons[gBankTarget].hp == 0
	&&  AttacksThisTurn(gBankAttacker, gCurrentMove) == 2) //Not charging move
		gBattlescriptCurrInstr = BattleScript_NoTargetMoveFailed;
    else
        gBattlescriptCurrInstr = gBattleScriptsForMoveEffects[gBattleMoves[gCurrentMove].effect];

    gCurrentActionFuncId = ACTION_RUN_BATTLESCRIPT;
}

extern u32 PlayAnimationTable[];
u16 GetMUS_ForBattle(void)
{
    if (gBattleTypeFlags & BATTLE_TYPE_LINK)
	{
		if (VarGet(BATTLE_TOWER_SONG_OVERRIDE))
			return VarGet(BATTLE_TOWER_SONG_OVERRIDE); //Play custom song
		else
		{
			#ifdef UNBOUND
				return BGM_BATTLE_BORRIUS_TRAINER;
			#else
				return BGM_BATTLE_RSE_TRAINER;
			#endif
		}
	}
	
    if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
    {
		u8 trainerClass = gTrainers[gTrainerBattleOpponent_A].trainerClass;
		
		if (gClassBasedBattleBGM[trainerClass])
			return gClassBasedBattleBGM[trainerClass];
		else if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER && VarGet(BATTLE_TOWER_SONG_OVERRIDE))
			return VarGet(BATTLE_TOWER_SONG_OVERRIDE);
		else
		#ifdef UNBOUND
			return BGM_BATTLE_BORRIUS_TRAINER;
		#else
			return BGM_BATTLE_TRAINER;
		#endif
    }
	
	u16 species = gEnemyParty[0].species;
	
	if (gWildSpeciesBasedBattleBGM[species])
		return gWildSpeciesBasedBattleBGM[species];
	
	#ifdef UNBOUND
		return BGM_BATTLE_BORRIUS_WILD;
	#else
		return BGM_BATTLE_WILD;
	#endif
}

u16 LoadProperMusicForLinkBattles(void)
{
	gBattleTypeFlags |= BATTLE_TYPE_LINK;
	return GetMUS_ForBattle();
}

// Determines which of the two given mons will strike first in a battle.
// Returns:
// 0 = first mon moves first
// 1 = second mon moves first
// 2 = second mon moves first because it won a 50/50 roll
u8 GetWhoStrikesFirst(bank_t bank1, bank_t bank2, bool8 ignoreMovePriorities) {
    s8 bank1_priority, bank2_priority;
	s32 bank1_bracket, bank2_bracket;
	u32 bank1_speed, bank2_speed;

//Priority Calc
	if(!ignoreMovePriorities) {
		bank1_priority = PriorityCalc(bank1, gActionForBanks[bank1], gBattleMons[bank1].moves[gBattleStruct->chosenMovePositions[bank1]]);
		bank2_priority = PriorityCalc(bank2, gActionForBanks[bank2], gBattleMons[bank2].moves[gBattleStruct->chosenMovePositions[bank2]]);		
		if (bank1_priority > bank2_priority)
			return FirstMon;
		else if (bank1_priority < bank2_priority)
			return SecondMon;
	}
	
//BracketCalc
	bank1_bracket = BracketCalc(bank1);
	bank2_bracket = BracketCalc(bank2);
		
	if (bank1_bracket > bank2_bracket)
		return FirstMon;
	else if (bank1_bracket < bank2_bracket)
		return SecondMon;
	
//SpeedCalc
	bank1_speed = SpeedCalc(bank1);
	bank2_speed = SpeedCalc(bank2);
	u32 temp;
	if (gNewBS->TrickRoomTimer) {
		temp = bank2_speed;
		bank2_speed = bank1_speed;
		bank1_speed = temp;
	}
	if (bank1_speed > bank2_speed)
		return FirstMon;
	else if (bank1_speed < bank2_speed) {
		return SecondMon;
	}
		
	return SpeedTie;
}

s8 PriorityCalc(u8 bank, u8 action, u16 move) {
	u8 priority = 0;
	if (action == ACTION_USE_MOVE) {	
		if (gProtectStructs[bank].onlyStruggle)
			move = MOVE_STRUGGLE;
		
		priority = gBattleMoves[move].priority;
		
		if (move != MOVE_BIDE && gBattleMons[bank].status2 & STATUS2_BIDE)
			priority = 1;
		
		ability_t ability = ABILITY(bank);
		switch (ability) {
			case ABILITY_PRANKSTER:
				if (gBattleMoves[move].split == SPLIT_STATUS)
					++priority;
				break;
			case ABILITY_GALEWINGS:
				if (gBattleMoves[move].type == TYPE_FLYING) {
					#ifndef OLD_GALE_WINGS
						if (gBattleMons[bank].hp == gBattleMons[bank].maxHP)
							++priority;
					#else
						++priority;
					#endif
				}
				break;
			case ABILITY_TRIAGE:
				if (gBattleMoves[move].flags & FLAG_TRIAGE_AFFECTED)
					priority += 3;
		}
	}
	
	return priority;
}

s32 BracketCalc(bank_t bank) {
	item_effect_t item_effect = ITEM_EFFECT(bank);
	u8 item_quality = ITEM_QUALITY(bank);
	ability_t ability = BanksAbility(bank);
	switch (item_effect) {
		case ITEM_EFFECT_QUICK_CLAW:
			if (umodsi(gRandomTurnNumber, 100) < item_quality) {
				gNewBS->CustapQuickClawIndicator |= gBitTable[bank];
				return 1;
			}
			break;
		case ITEM_EFFECT_CUSTAP_BERRY:
			if (!AbilityBattleEffects(ABILITYEFFECT_CHECK_OTHER_SIDE, bank, ABILITY_UNNERVE, 0, 0)
			&& PINCH_BERRY_CHECK(bank))
			{
				gNewBS->CustapQuickClawIndicator |= gBitTable[bank];
				return 1;
			}
			break;
		case ITEM_EFFECT_LAGGING_TAIL:
			return -2;
	}
	
	if (ability == ABILITY_STALL)
		return -1;
	
	return 0;
}

u32 SpeedCalc(bank_t bank) {
	u32 speed;
	ability_t ability = BanksAbility(bank);
	item_effect_t item_effect = ITEM_EFFECT(bank);
	u8 item_quality = ITEM_QUALITY(bank);
    // Calculate adjusted speed stat
    speed = udivsi((gBattleMons[bank].speed * gStatStageRatios[gBattleMons[bank].statStages[STAT_STAGE_SPEED-1]][0]), gStatStageRatios[gBattleMons[bank].statStages[STAT_STAGE_SPEED-1]][1]);
    // Check for abilities that boost speed in weather.
    if (WEATHER_HAS_EFFECT) {
		switch (ability) {
			case ABILITY_SWIFTSWIM:
				if (gBattleWeather & WEATHER_RAIN_ANY)
					speed *= 2;
				break;
			case ABILITY_CHLOROPHYLL:
				if (gBattleWeather & WEATHER_SUN_ANY)
					speed *= 2;
				break;
			case ABILITY_SANDRUSH:
				if (gBattleWeather & WEATHER_SANDSTORM_ANY)
					speed *= 2;
				break;
			case ABILITY_SLUSHRUSH:
				if (gBattleWeather & WEATHER_HAIL_ANY)
					speed *= 2;
				break;
		}
	}
	
	switch (ability) {
		case ABILITY_UNBURDEN:
			if (gNewBS->UnburdenBoosts & (1 << bank))
				speed *= 2;
			break;
		case ABILITY_SLOWSTART:
			if (gNewBS->SlowStartTimers[bank])
				speed /= 2;
			break;
		case ABILITY_SURGESURFER:
			if (TerrainType == ELECTRIC_TERRAIN)
				speed *= 2;
	}
	
	switch (item_effect) {
		case ITEM_EFFECT_MACHO_BRACE:
			speed /= 2;
			break;			
		case ITEM_EFFECT_CHOICE_BAND:
			if (item_quality == QUALITY_CHOICE_SCARF)
				speed = udivsi((speed * 150), 100);
			break;
		case ITEM_EFFECT_IRON_BALL:
			speed /= 2;
			break;			
		case ITEM_EFFECT_QUICK_POWDER:
			if (gBattleMons[bank].species == SPECIES_DITTO && !(gBattleMons[bank].status2 & STATUS2_TRANSFORMED))
				speed *= 2;
	}
	
	if (gNewBS->TailwindTimers[bank & 1])
		speed *= 2;
	if (gNewBS->SwampTimers[bank & 1])
		speed /= 4;
	
	#ifdef BADGE_BOOSTS
		if (!(gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_TRAINER_TOWER | BATTLE_TYPE_FRONTIER | BATTLE_TYPE_EREADER_TRAINER)) 
		&& FlagGet(FLAG_BADGE03_GET)
		&& gBattleTypeFlags & BATTLE_TYPE_TRAINER
		&& SIDE(bank) == B_SIDE_PLAYER
		&& gTrainerBattleOpponent != 0x400)
			speed = udivsi((speed * 110), 100);
	#endif
	
	if ((gBattleMons[bank].status1 & STATUS_ANY) && ability == ABILITY_QUICKFEET)
		speed *= 2;
	else if (gBattleMons[bank].status1 & STATUS_PARALYSIS) {
		#ifndef OLD_PARALYSIS_SPD_DROP
			speed /= 2;
		#else
			speed /= 4;
		#endif
	}
	
    return speed;
}

u32 SpeedCalcForParty(u8 side, pokemon_t* party) {
	u32 speed = party->speed;
	ability_t ability = GetPartyAbility(party);
	item_effect_t item_effect;
	if (ability != ABILITY_KLUTZ)
		item_effect = ItemId_GetHoldEffect(party->item);
	else
		item_effect = 0;
	u8 item_quality = ItemId_GetHoldEffectParam(party->item);
	
    // Check for abilities that boost speed in weather.
    if (WEATHER_HAS_EFFECT) {
		switch (ability) {
			case ABILITY_SWIFTSWIM:
				if (gBattleWeather & WEATHER_RAIN_ANY)
					speed *= 2;
				break;
			case ABILITY_CHLOROPHYLL:
				if (gBattleWeather & WEATHER_SUN_ANY)
					speed *= 2;
				break;
			case ABILITY_SANDRUSH:
				if (gBattleWeather & WEATHER_SANDSTORM_ANY)
					speed *= 2;
				break;
			case ABILITY_SLUSHRUSH:
				if (gBattleWeather & WEATHER_HAIL_ANY)
					speed *= 2;
				break;
		}
	}
	
	switch (ability) {
		case ABILITY_SLOWSTART:
			speed /= 2;
			break;
		case ABILITY_SURGESURFER:
			if (TerrainType == ELECTRIC_TERRAIN)
				speed *= 2;
	}
	
	switch (item_effect) {
		case ITEM_EFFECT_MACHO_BRACE:
			speed /= 2;
			break;			
		case ITEM_EFFECT_CHOICE_BAND:
			if (item_quality == QUALITY_CHOICE_SCARF)
				speed = udivsi((speed * 150), 100);
			break;
		case ITEM_EFFECT_IRON_BALL:
			speed /= 2;
			break;			
		case ITEM_EFFECT_QUICK_POWDER:
			if (party->species == SPECIES_DITTO)
				speed *= 2;
	}
	
	if (gNewBS->TailwindTimers[side])
		speed *= 2;
	if (gNewBS->SwampTimers[side])
		speed /= 4;
	
	if ((party->condition & STATUS_ANY) && ability == ABILITY_QUICKFEET)
		speed *= 2;
	else if (party->condition & STATUS_PARALYSIS) {
		#ifndef OLD_PARALYSIS_SPD_DROP
			speed /= 2;
		#else
			speed /= 4;
		#endif
	}
    return speed;
}

const u8 gStatStageRatios[][2] =
{
    {10, 40}, // -6
    {10, 35}, // -5
    {10, 30}, // -4
    {10, 25}, // -3
    {10, 20}, // -2
    {10, 15}, // -1
    {10, 10}, //  0
    {15, 10}, //  1
    {20, 10}, //  2
    {25, 10}, //  3
    {30, 10}, //  4
    {35, 10}, //  5
    {40, 10}  //  6
};