#pragma once

/**
 * \file battle_controller_opponent.h
 * \brief Contains functions relating the controller of the opponent in battles.
 *		  Includes things such as move choosing, trainer sliding, mon choosing,
 *		  and loading the correct trainer picture.
 */

//Exported Functions
/*NONE*/

//Functions hooked in
void OpponentHandleChooseMove(void);
void OpponentHandleDrawTrainerPic(void);
void OpponentHandleTrainerSlide(void);
void OpponentHandleChoosePokemon(void);

//Exported Constants
#define AI_CHOICE_FLEE 4
#define AI_CHOICE_WATCH 5
