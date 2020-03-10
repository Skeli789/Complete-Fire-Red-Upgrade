#pragma once

#include "../global.h"

/**
 * \file daycare.h
 * \brief Contains functions relating to the Pokemon daycare (mainly focusing on
 *		  breeding Pokemon).
 */

//Exported Functions
u8 GetAllEggMoves(struct Pokemon* mon, u16* moves, bool8 ignoreAlreadyKnownMoves);

//Functions Hooked In
void BuildEggMoveset(struct Pokemon* egg, struct BoxPokemon* father, struct BoxPokemon* mother);
void CreateEgg(struct Pokemon *mon, u16 species);
void GiveEggFromDaycare(struct DayCare* daycare);
void TriggerPendingDaycareEgg(unusedArg struct DayCare *daycare);
void CreatedHatchedMon(struct Pokemon *egg, struct Pokemon *temp);
u32 SubtractEggSteps(u32 steps, struct Pokemon* mon);

//Exported Constants
#define EGG_MOVES_ARRAY_COUNT 50
