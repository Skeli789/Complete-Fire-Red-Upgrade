#pragma once

#include "../global.h"
#include "../pokemon.h"

/**
 * \file build_pokemon.h
 * \brief Contains functions relating to the building of Pokemon teams and the
 *		  setting of Pokemon data.
 */
 
//Exported Functions
void sp067_GenerateRandomBattleTowerTeam(void);
void GiveMonNatureAndAbility(pokemon_t* mon, u8 nature, u8 abilityNum);

//Functions Hooked In
void BuildTrainerPartySetup(void);
u8 GetMonPokeBall(struct PokemonSubstruct0* data);
void SetMonPokeBall(struct PokemonSubstruct0* data, u8 ballId);
void CreateBoxMon(struct BoxPokemon *boxMon, u16 species, u8 level, u8 fixedIV, u8 hasFixedPersonality, u32 fixedPersonality, u8 otIdType, u32 fixedOtId);

