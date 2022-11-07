#pragma once

/**
 * \file battle_start_turn_start.h
 * \brief Functions relating to the beginning of battle and the beginning of each
 *		  Pokemon's turn. Helps set up things like Mega Evolution and Z-Moves, as
 *		  well as contains the the turn order calculation.
 */

//Exported Functions
bool8 TryActivateOWTerrain(void);
u8 GetTotemStat(u8 bank, bool8 multiBoost);
u8 GetTotemRaiseAmount(u8 bank, bool8 multiBoost);
s8 TotemRaiseAmountToStatMod(u8 raiseAmount);
u8 CanActivateTotemBoost(u8 bank);
void DetermineFirstMultiTarget(void);
u16 GetMUS_ForBattle(void);
u8 GetTrainerBattleTransition(void);
u8 GetWhoStrikesFirst(u8 bank1, u8 bank2, bool8 ignoreMovePriorities);
s8 PriorityCalc(u8 bank, u8 action, u16 move);
s8 PriorityCalcMon(struct Pokemon* mon, u16 move);
bool8 QuickClawActivatesThisTurn(u8 bank);
s32 BracketCalc(u8 bank, u8 action, u16 move);
u32 SpeedCalc(u8 bank);
u32 SpeedCalcMon(u8 side, struct Pokemon* mon);

//Hooked in Functions
void HandleNewBattleRamClearBeforeBattle(void);
void BattleBeginFirstTurn(void);
void CleanUpExtraTurnValues(void);
void SetActionsAndBanksTurnOrder(void);
void RunTurnActionsFunctions(void);
void HandleAction_UseMove(void);
u16 LoadProperMusicForLinkBattles(void);

//Exported Constants
enum TotemBoostType
{
	TOTEM_NO_BOOST,
	TOTEM_SINGLE_BOOST,
	TOTEM_MULTI_BOOST,
	TOTEM_OMNIBOOST,
};
