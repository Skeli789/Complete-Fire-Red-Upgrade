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
void DoFormChange(u8 bank, u16 species, bool8 reloadType, bool8 reloadStats, bool8 reloadAbility);
void SwitchOutFormsRevert(u8 bank);
void FormsRevert(pokemon_t* party);
bool8 TryFormRevert(pokemon_t* mon);
void UpdateBurmy(void);
void TryRevertOriginFormes(unusedArg struct Pokemon* mon, unusedArg bool8 ignoreDistortionWorld);
species_t GetMiniorCoreFromPersonality(u32 personality);
species_t GetMiniorCoreSpecies(struct Pokemon* mon);
void TryCrownZacianZamazenta(struct Pokemon* party);
void TrySetCorrectToxtricityForm(struct BoxPokemon* mon);
void HoldItemFormChange(struct Pokemon* mon, u16 item);
void HoopaShayminPCRevertCheck(struct Pokemon* mon, bool8 recalcStats);
//Hooked In Functions
void HandleFormChange(void);

//Exported Structs
extern const species_t gMiniorCores[];
extern const u16 gTypeToSilvallyForm[];
