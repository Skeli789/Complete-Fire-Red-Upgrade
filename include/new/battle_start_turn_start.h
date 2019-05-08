#pragma once

/**
 * \file battle_start_turn_start.h
 * \brief Functions relating to the beginning of battle and the beginning of each
 *		  Pokemon's turn. Helps set up things like Mega Evolution and Z-Moves, as
 *		  well as contains the the turn order calculation.
 */
 
//Exported Functions
u16 GetMUS_ForBattle(void);
u8 GetWhoStrikesFirst(u8 bank1, u8 bank2, bool8 ignoreMovePriorities);
s8 PriorityCalc(u8 bank, u8 action, u16 move);
s32 BracketCalc(u8 bank);
u32 SpeedCalc(u8 bank);
u32 SpeedCalcForParty(u8 side, pokemon_t*);

//Exported Structs
struct SpecialZMove
{
	u16 species;
	u16 item;
	u16 move;
	u16 zmove;
};

extern const struct SpecialZMove gSpecialZMoveTable[];

//Hooked in Functions
void HandleNewBattleRamClearBeforeBattle(void);
void SavePartyItems(void);
void BattleBeginFirstTurn(void);
void SetActionsAndBanksTurnOrder(void);
void RunTurnActionsFunctions(void);
void HandleAction_UseMove(void);
u16 LoadProperMusicForLinkBattles(void);

