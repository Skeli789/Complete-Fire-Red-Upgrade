#pragma once

#include "../global.h"
#include "../pokemon.h"

/**
 * \file form_change.h
 * \brief Contains functions relating to Pokemon changing forms during battle,
 *		  as well as Pokemon reverting back to their original forms upon switch
 *		  out or battle end. Also contains a table of the various Minior forms.
 */
 
//Exported Functions
void DoFormChange(u8 bank, u16 species, bool8 ReloadType, bool8 ReloadStats);
void SwitchOutFormsRevert(u8 bank);
void FormsRevert(pokemon_t* party);
void TryFormRevert(pokemon_t* mon);
void UpdateBurmy(void);
species_t GetMiniorCoreSpecies(struct Pokemon* mon);

//Hooked In Functions
void HandleFormChange(void);

//Exported Structs
extern const species_t gMiniorCores[];