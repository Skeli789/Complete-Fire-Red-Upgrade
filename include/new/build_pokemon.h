#pragma once

#include "../global.h"
#include "../pokemon.h"

#include "frontier.h"

/**
 * \file build_pokemon.h
 * \brief Contains functions relating to the building of Pokemon teams and the
 *		  setting of Pokemon data.
 */
 
//Exported Functions
void sp067_GenerateRandomBattleTowerTeam(void);
u16 sp068_GivePlayerFrontierMonGivenSpecies(void);
void GiveMonNatureAndAbility(pokemon_t* mon, u8 nature, u8 abilityNum, bool8 forceShiny);
bool8 IsMonBannedInTier(struct Pokemon* mon, u8 tier);
bool8 IsMonAllowedInBattleTower(struct Pokemon* mon);
void TryStatusInducer(struct Pokemon* mon);
void CalculateMonStatsNew(struct Pokemon *mon);
u8 GetOpenWorldBadgeCount(void);

//Functions Hooked In
void BuildTrainerPartySetup(void);
u8 GetMonPokeBall(struct PokemonSubstruct0* data);
void SetMonPokeBall(struct PokemonSubstruct0* data, u8 ballId);
void SetMonHeldItem(struct PokemonSubstruct0* data, u16 item, struct Pokemon* mon);
void CreateBoxMon(struct BoxPokemon *boxMon, u16 species, u8 level, u8 fixedIV, u8 hasFixedPersonality, u32 fixedPersonality, u8 otIdType, u32 fixedOtId);
void CreateMonWithNatureLetter(struct Pokemon* mon, u16 species, u8 level, u8 fixedIV, u8 nature, u8 unownLetter);

//Exported Constants
enum TierBanCheckingType 
{
	CHECK_BATTLE_TOWER_SPREADS,
	CHECK_PARTY_OFFSET,
};