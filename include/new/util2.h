#pragma once

#include "../global.h"
#include "../pokemon.h"

/**
 * \file helper_functions.h
 * \brief Contains many utility functions that mostly aid in battle, but can also
 *		  help in the overworld.
 */

 //Exported Functions
u32 MathMax(u32 num1, u32 num2);
u32 MathMin(u32 num1, u32 num2);
u16 RandRange(u16 min, u16 max);
bool8 CheckTableForMove(move_t move, const u16 table[]);
bool8 CheckTableForMoveEffect(move_t move, const u8 table[]);
bool8 CheckTableForMovesEffect(move_t move, const u8 table[]);
bool8 CheckTableForAbility(u8 ability, const u8 table[]);
bool8 CheckTableForSpecialMoveEffect(u8 effect, const u8 table[]);
bool8 CheckTableForSpecies(u16 species, const u16 table[]);
bool8 CheckTableForItem(u16 item, const u16 table[]);
u8 ViableMonCount(struct Pokemon* party);
void HealMon(struct Pokemon* mon);
void SetMonPokedexFlags(struct Pokemon* mon);
u16 GetNationalPokedexCount(u8 caseID);
bool8 CanEvolve(struct Pokemon* mon);
bool8 CouldHaveEvolvedViaLevelUp(struct Pokemon* mon);
bool8 EvolveSpeciesByLevel(u16* species, u8 level);
u32 GetBaseStatsTotal(const u16 species);
u8 FindMovePositionInMonMoveset(u16 move, struct Pokemon* mon);
bool8 MoveInMonMoveset(u16 move, struct Pokemon* mon);
bool8 AllHittingMoveWithTypeInMonMoveset(struct Pokemon* mon, u8 moveType);
u8 GetMonType(struct Pokemon* mon, u8 typeId);
bool8 IsMonOfType(struct Pokemon* mon, u8 type);
bool8 IsMinior(u16 species);
void DestroyMonIconSprite(struct Sprite* sprite);
bool8 CanPartyMonBeGeneralStatused(struct Pokemon* mon);
bool8 CanPartyMonBePutToSleep(struct Pokemon* mon);
bool8 CanPartyMonBePoisoned(struct Pokemon* mon);
bool8 CanPartyMonBeParalyzed(struct Pokemon* mon);
bool8 CanPartyMonBeBurned(struct Pokemon* mon);
bool8 CanPartyMonBeFrozen(struct Pokemon* mon);
