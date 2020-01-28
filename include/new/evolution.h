#pragma once

#include "../global.h"

/**
 * \file evolution.h
 * \brief Implements new evolution methods.
 */

//Exported Functions
bool8 IsLevelUpEvolutionMethod(u8 method);
bool8 IsItemEvolutionMethod(u8 method);
bool8 IsFriendshipEvolutionMethod(u8 method);
bool8 IsOtherEvolutionMethod(u8 method);
u16 GetMonDevolution(struct Pokemon* mon);

//Functions Hooked In
u16 GetEvolutionTargetSpecies(struct Pokemon* mon, u8 type, u16 evolutionItem);
void ItemEvolutionRemoval(pokemon_t* mon);
