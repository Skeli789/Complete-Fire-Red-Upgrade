#pragma once

#include "../global.h"
#include "../battle_controllers.h"

/**
 * \file move_menu.h
 * \brief Contains functions relating to player's action selection menu in battle
 *		  (Fight, Pokemon, Bag, Run). Mainly deals with choosing moves and their
 *		  targets. Also contains the triggers for Mega Evolution and Z-Moves.
 */

//Exported Functions
void HandleInputChooseTarget(void);
bool8 CheckCantMoveThisTurn(void);
bool8 IsBagDisabled(void);

//Functions Hooked In
void InitMoveSelectionsVarsAndStrings(void);
void MoveSelectionDisplayMoveEffectiveness(void);
void HandleInputChooseMove(void);
void EmitMoveChosen(u8 bufferId, u8 chosenMoveIndex, u8 target, u8 megaState, u8 ultraState, u8 zMoveState, u8 dynamaxState, u8 teraState); // For Terastallization
void EmitChooseMove(u8 bufferId, bool8 isDoubleBattle, bool8 NoPpNumber, struct ChooseMoveStruct* movePpData);
void HandleMoveSwitchingUpdate(void);
u8 TrySetCantSelectMoveBattleScript(void);
void PlayerHandleChooseAction(void);
void HandleInputChooseAction(void);

//Exported Constants
#define TYPE_ICON_TAG 0x2720
#define TYPE_ICON_TAG_2 0x2721
