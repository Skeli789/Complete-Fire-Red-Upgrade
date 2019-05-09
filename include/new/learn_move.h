#pragma once

#include "../global.h"

/**
 * \file learn_move.h
 * \brief Contains functions relating to Pokemon learning new moves. Also contains
 *		  functions for Evolution moves and the move relearner
 */
 
//Exported Functions
void GiveBoxMonInitialMoveset(struct BoxPokemon* boxMon);
u16 MonTryLearningNewMoveAfterEvolution(struct Pokemon* mon, bool8 firstMove);
u8 GetLevelUpMovesBySpecies(u16 species, u16* moves);


//Functions Hooked In
u16 MonTryLearningNewMove(struct Pokemon* mon, bool8 firstMove);
u8 GetMoveRelearnerMoves(struct Pokemon* mon, u16* moves);
u8 GetNumberOfRelearnableMoves(struct Pokemon* mon);