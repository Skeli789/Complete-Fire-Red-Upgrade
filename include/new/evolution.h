#pragma once

#include "../global.h"

/**
 * \file evolution.h
 * \brief Implements new evolution methods.
 */

//Functions Hooked In
u16 GetEvolutionTargetSpecies(struct Pokemon* mon, u8 type, u16 evolutionItem);
void ItemEvolutionRemoval(pokemon_t* mon);
u16 GetMonDevolution(struct Pokemon* mon);
