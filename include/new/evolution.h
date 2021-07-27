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
bool8 HasHighNature(struct Pokemon* mon);
bool8 EvolvesViaScoring3Crits(struct Pokemon* mon);
u16 GetMonDevolution(struct Pokemon* mon);
u8 GetMinimumLevel(u16 species);

//Functions Hooked In
u16 GetEvolutionTargetSpecies(struct Pokemon* mon, u8 type, u16 evolutionItem);
void ItemEvolutionRemoval(pokemon_t* mon);
