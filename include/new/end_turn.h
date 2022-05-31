#pragma once

#include "../global.h"

/**
 * \file end_turns.h
 * \brief Contains functions relating to the end of each turn after all Pokemon
 *		  on the field have acted. Also contains functions the implement the Gen 4+
 *		  switch-in after end turn feature (the end turn effects activate, and then
 *		  any fainted Pokemon are replaced). The order of the events found here are based on:
 *		  https://bulbapedia.bulbagarden.net/wiki/User:SnorlaxMonster/End-turn_resolution_order.
 */

//Exported Functions
u8 CountAliveMonsOnField(void);
u32 GetLeechSeedDamage(u8 bank);
u32 GetPoisonDamage(u8 bank, bool8 aiCalc);
u32 GetBurnDamage(u8 bank);
u32 GetFrostbiteDamage(u8 bank);
u32 GetNightmareDamage(u8 bank);
u32 GetCurseDamage(u8 bank);
u32 GetTrapDamage(u8 bank);
u32 GetSeaOfFireDamage(u8 bank);
u32 GetBadDreamsDamage(u8 bank);
u32 GetSplintersDamage(u8 bank);
u32 GetBadThoughtsDamage(u8 bank);
u32 GetGMaxVineLashDamage(u8 bank);
u32 GetGMaxWildfireDamage(u8 bank);
u32 GetGMaxCannonadeDamage(u8 bank);
u32 GetGMaxVolcalithDamage(u8 bank);

//Functions Hooked In
u8 TurnBasedEffects(void);
bool8 HandleFaintedMonActions(void);
void RemoveSwitchInForFaintedBank(void);
bool8 IsInMiddleOfEndTurnSwitchIn(u8 bank);
bool8 HandleDynamaxOnTurnEnd(void);

