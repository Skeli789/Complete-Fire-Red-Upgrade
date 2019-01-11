#include "defines.h"
#include "battle_start_turn_start.h"
#include "helper_functions.h"
#include "multi.h"

extern void (* const sTurnActionsFuncsTable[])(void);
extern void (* const sEndTurnFuncsTable[])(void);

extern u8* DoMegaEvolution(u8 bank);
extern u8* DoPrimalReversion(bank_t, u8 caseId);

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

u8 GetWhoStrikesFirst(bank_t, bank_t, bool8 ignoreMovePriorities);
s8 PriorityCalc(u8 bank, u8 action, u16 move);
s32 BracketCalc(u8 bank);
u32 SpeedCalc(u8 bank);
u32 SpeedCalcForParty(u8 side, pokemon_t*);

void HandleNewBattleRamClearBeforeBattle(void) {
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

/*
void ReadBattleDataFromBuffer(void) //Hook at 0x13070
{
    u8 *ptr;
    s32 i;

    if (gBattleExecBuffer == 0)
    {
        for (gActiveBattler = 0; gActiveBattler < gBattlersCount; gActiveBattler++)
        {
            if ((gBattleTypeFlags & BATTLE_TYPE_SAFARI)
             && GetBattlerSide(gActiveBattler) == 0)
            {
                MEMSET_ALT(&gBattleMons[gActiveBattler], 0, 0x58, i, ptr);
            }
            else
            {
                u8 bank = GetBattlerSide(gActiveBattler);

                MEMSET_ALT(&gBattleMons[gActiveBattler], gBattleBufferB[gActiveBattler][4 + i], 0x58, i, ptr);
                gBattleMons[gActiveBattler].type1 = gBaseStats[gBattleMons[gActiveBattler].species].type1;
                gBattleMons[gActiveBattler].type2 = gBaseStats[gBattleMons[gActiveBattler].species].type2;
				gBattleMons[gActiveBattler].type3 = TYPE_BLANK;
                gBattleMons[gActiveBattler].ability = GetAbilityBySpecies(gBattleMons[gActiveBattler].species, gBattleMons[gActiveBattler].altAbility);

                gBattleStruct->hpOnSwitchout[bank] = gBattleMons[gActiveBattler].hp;
                for (i = 0; i < BATTLE_STATS_NO-1; i++)
                    gBattleMons[gActiveBattler].statStages[i] = 6;
                gBattleMons[gActiveBattler].status2 = 0;
            }

            if (GetBattlerPosition(gActiveBattler) == 0)
            {
                EmitTrainerThrow(0);
                MarkBufferBankForExecution(gActiveBattler);
            }

            if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
            {
                if (GetBattlerPosition(gActiveBattler) == 1)
                {
                    EmitTrainerThrow(0);
                    MarkBufferBankForExecution(gActiveBattler);
                }
                if (GetBattlerSide(gActiveBattler) == 1
                 && !(gBattleTypeFlags & (BATTLE_TYPE_EREADER_TRAINER | BATTLE_TYPE_TRAINER_TOWER | BATTLE_TYPE_FRONTIER | BATTLE_TYPE_LINK)))
                    GetSetPokedexFlag(SpeciesToNationalPokedexNum(gBattleMons[gActiveBattler].species), 2);
            }
            else
            {
                if (GetBattlerSide(gActiveBattler) == 1
                 && !(gBattleTypeFlags & (BATTLE_TYPE_EREADER_TRAINER | BATTLE_TYPE_TRAINER_TOWER | BATTLE_TYPE_FRONTIER | BATTLE_TYPE_LINK)))
                {
                    GetSetPokedexFlag(SpeciesToNationalPokedexNum(gBattleMons[gActiveBattler].species), 2);
                    EmitLoadPokeSprite(0);
                    MarkBufferBankForExecution(gActiveBattler);
                }
            }

            if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
            {
                if (GetBattlerPosition(gActiveBattler) == 2
                 || GetBattlerPosition(gActiveBattler) == 3)
                {
                    EmitTrainerThrow(0);
                    MarkBufferBankForExecution(gActiveBattler);
                }
            }
        }
        gBattleMainFunc = bc_801333C;
    }
}
*/

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
						gBattlerAttacker = gBanksByTurnOrder[*bank];
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
						gBattlerAttacker = gBanksByTurnOrder[*bank];
						++effect;
					}
					++*bank;
					
					if (effect) return;
				}
				*bank = 0; //Reset Bank for next loop
				++*state;
				break;
			
			case TotemPokemon:	;
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
								gBattlerAttacker = gBattleScripting->bank = *bank;
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
					gBattleMons[i].status2 &= ~8;
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

enum MegaStates {
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
	u8* megaBank = &(MegaData->activeBank);
	
    if (gBattleOutcome != 0)
        gCurrentActionFuncId = ACTION_FINISHED;
	
	if (gCurrentActionFuncId == ACTION_USE_MOVE)
	{
		switch (MegaData->state) {
			case Mega_Check:
				for (i = *megaBank; i < gBattlersCount; ++i, ++*megaBank) {
					u8 bank = gBanksByTurnOrder[i];
					gActiveBattler = bank;
					if (MegaData->chosen[bank] && !MegaData->done[bank] && !(ZMoveData->partyIndex[SIDE(bank)] &= gBitTable[gBattlerPartyIndexes[bank]])) {
						u8* script = DoMegaEvolution(bank);
						if (script != NULL) 
						{	
							MegaData->chosen[bank] = 0;
							MegaData->done[bank] = TRUE;
							MegaData->partyIndex[SIDE(bank)] |= gBitTable[gBattlerPartyIndexes[bank]]; //Stops Mega Rayquaza from Using a Z-Move
							MegaData->megaEvoInProgress = TRUE;
							MegaData->script = script;
							if (!(gBattleTypeFlags & (BATTLE_TYPE_INGAME_PARTNER | BATTLE_TYPE_MULTI)) 
							&& SIDE(bank) == B_SIDE_PLAYER) 
							{
								MegaData->chosen[PARTNER(bank)] = 0;
								MegaData->done[PARTNER(bank)] = TRUE;
								MegaData->partyIndex[SIDE(bank)] |= gBitTable[gBattlerPartyIndexes[PARTNER(bank)]]; //Stops Mega Rayquaza from Using a Z-Move
							}
							else if (!(gBattleTypeFlags & (BATTLE_TYPE_TWO_OPPONENTS | BATTLE_TYPE_MULTI)) 
							&& SIDE(bank) == B_SIDE_OPPONENT) 
							{
								MegaData->chosen[PARTNER(bank)] = 0;
								MegaData->done[PARTNER(bank)] = TRUE;
								MegaData->partyIndex[SIDE(bank)] |= gBitTable[gBattlerPartyIndexes[PARTNER(bank)]]; //Stops Mega Rayquaza from Using a Z-Move
							}
							BattleScriptExecute(MegaData->script);
							return;
						}
					}
					else if (UltraData->chosen[bank] && !UltraData->done[bank]) {
						u8* script = DoMegaEvolution(bank);
						if (script != NULL) 
						{	
							UltraData->chosen[bank] = 0;
							UltraData->done[bank] = TRUE;
							MegaData->megaEvoInProgress = TRUE;
							MegaData->script = script;
							if (!(gBattleTypeFlags & (BATTLE_TYPE_INGAME_PARTNER | BATTLE_TYPE_MULTI)) 
							&& SIDE(bank) == B_SIDE_PLAYER) 
							{
								UltraData->chosen[PARTNER(bank)] = 0;
								UltraData->done[PARTNER(bank)] = TRUE;
							}
							else if (!(gBattleTypeFlags & (BATTLE_TYPE_TWO_OPPONENTS | BATTLE_TYPE_MULTI)) 
							&& SIDE(bank) == B_SIDE_OPPONENT) 
							{
								UltraData->chosen[PARTNER(bank)] = 0;
								UltraData->done[PARTNER(bank)] = TRUE;
							}
							BattleScriptExecute(MegaData->script);
							return;
						}
					}
				}
				if (MegaData->megaEvoInProgress)
					++MegaData->state;
				else
					MegaData->state = Mega_End;
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
							&& gActionsByTurnOrder[j] != ACTION_SWITCH)
						{
							if (GetWhoStrikesFirst(bank1, bank2, FALSE))
								SwapTurnOrder(i, j);
						}
					}
				}
				*megaBank = 0; //Reset the bank for the next loop
				++MegaData->state;
				return;
			
			case Mega_SwitchInAbilities:
				while (*megaBank < gBattlersCount) {
					if (AbilityBattleEffects(ABILITYEFFECT_ON_SWITCHIN, gBanksByTurnOrder[*megaBank], 0, 0, 0))
						++effect;
					++*megaBank;
						
					if (effect) return;
				}
				*megaBank = 0;
				++MegaData->state;
				return;
					
			case Mega_Intimidate:
				if (AbilityBattleEffects(ABILITYEFFECT_INTIMIDATE1, 0, 0, 0, 0))
					return;
				if (AbilityBattleEffects(ABILITYEFFECT_INTIMIDATE2, 0, 0, 0, 0))
					return;
						
				MegaData->script = 0;
				MegaData->state = Mega_End;
				MegaData->activeBank = 0;
				MegaData->megaEvoInProgress = FALSE;
		}
	
		while (gBattleStruct->focusPunchBank < gBattlersCount)
		{
			gActiveBattler = gBanksByTurnOrder[gBattleStruct->focusPunchBank];
			++gBattleStruct->focusPunchBank;
			u16 chosenMove = gChosenMovesByBanks[gActiveBattler];
			if ((chosenMove == MOVE_FOCUSPUNCH || chosenMove == MOVE_BEAKBLAST || chosenMove == MOVE_SHELLTRAP)
			&& !(gBattleMons[gActiveBattler].status1 & STATUS1_SLEEP)
			&& !(gDisableStructs[gActiveBattler].truantCounter)
			&& !(gProtectStructs[gActiveBattler].onlyStruggle)) //or noValidMoves in Emerald
			{
				gBattleScripting->bank = gActiveBattler;
				if (chosenMove == MOVE_BEAKBLAST) {
					BeakBlastByte |= gBitTable[gActiveBattler];
					BattleScriptExecute(BattleScript_BeakBlastSetUp);
				}
				else if (chosenMove == MOVE_SHELLTRAP)
					BattleScriptExecute(BattleScript_ShellTrapSetUp);
				else
					BattleScriptExecute(BattleScript_FocusPunchSetUp);
				return;
			}
		}
		
		u8 atkBank = gBanksByTurnOrder[gCurrentTurnActionNumber];
		if (CustapQuickClawIndicator & gBitTable[atkBank]) 
		{
			CustapQuickClawIndicator &= ~(gBitTable[atkBank]);
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
        gHitMarker &= ~(HITMARKER_x100000);
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
		bank2_priority = PriorityCalc(bank2, gActionForBanks[bank2], gBattleMons[bank1].moves[gBattleStruct->chosenMovePositions[bank2]]);
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
	if (TrickRoomTimer) {
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
	if (action) {
		if (gProtectStructs[bank].onlyStruggle)
			move = MOVE_STRUGGLE;
		
		priority = gBattleMoves[move].priority;
		
		ability_t ability = BanksAbility(bank);
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
				CustapQuickClawIndicator |= (1 << bank);
				return 1;
			}
			break;
		case ITEM_EFFECT_CUSTAP_BERRY:
			if (!AbilityBattleEffects(ABILITYEFFECT_CHECK_OTHER_SIDE, bank, ABILITY_UNNERVE, 0, 0)) {
				if (ability == ABILITY_GLUTTONY && gBattleMons[bank].hp > gBattleMons[bank].maxHP / 2) {
					CustapQuickClawIndicator |= (1 << bank << 4);
					return 1;
				}
				else if (gBattleMons[bank].hp > gBattleMons[bank].maxHP / 4) {
					CustapQuickClawIndicator |= (1 << bank << 4);
					return 1;
				}
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
			if (UnburdenBoosts & (1 << bank))
				speed *= 2;
			break;
		case ABILITY_SLOWSTART:
			if (SlowStartTimers[bank])
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
			if (gBattleMons[bank].species == PKMN_DITTO && !(gBattleMons[bank].status2 & STATUS2_TRANSFORMED))
				speed *= 2;
	}
	
	if (TailwindTimers[bank & 1])
		speed *= 2;
	if (SwampTimers[bank & 1])
		speed /= 4;
	
	#ifdef BADGE_BOOSTS
		if (!(gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_TRAINER_TOWER | BATTLE_TYPE_FRONTIER | BATTLE_TYPE_EREADER_TRAINER)) 
		&& FlagGet(FLAG_BADGE03_GET)
		&& gBattleTypeFlags & BATTLE_TYPE_TRAINER
		&& GetBattlerSide(bank) == B_SIDE_PLAYER
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
			if (party->species == PKMN_DITTO)
				speed *= 2;
	}
	
	if (TailwindTimers[side])
		speed *= 2;
	if (SwampTimers[side])
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