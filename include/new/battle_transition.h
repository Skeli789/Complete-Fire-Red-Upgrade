#pragma once

#include "../global.h"
#include "../pokemon.h"

/**
 * \file battle_transition.h
 * \brief Contains functions relating to the transition from the Oveworld to battles.
 *		  Mainly focuses on trainer mugshots.
 */
 
//Functions Hooked In
bool8 Phase2_Mugshot_Func2(struct Task* task);
void Mugshots_CreateOpponentPlayerSprites(struct Task* task);
bool8 Phase2_Mugshot_Func4(struct Task* task);
bool8 Phase2_Mugshot_Func5(struct Task *task);
bool8 Phase2_Mugshot_Func8(struct Task *task);
bool8 ShiftTrainerMugshotSprite(struct Sprite *sprite);