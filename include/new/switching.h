#pragma once

#include "../global.h"

/**
 * \file switching.h
 * \brief Contains functions relating to Pokemon switching in battle. These include effects
 *		  when the Pokemon switches out, and effects when a new Pokemon switches in. It also
 *		  includes code for Baton Pass.
 */

//Exported Functions
bool8 HasNoMonsToSwitch(u8 battler);
bool8 HandleSpecialSwitchOutAbilities(u8 bank, u8 ability, bool8 leftField);
void TryReactivateCentifernoSandblast(u32* backupStatus2);
void ClearSwitchBytes(u8 bank);
void ClearSwitchBits(u8 bank);
u32 CalcSpikesDamage(u8 bank);
u32 CalcSpikesDamagePartyMon(struct Pokemon* mon, u8 side);
u32 GetStealthRockDamage(u8 bank);
u32 CalcStealthRockDamage(u8 bank);
u32 GetStealthRockDamagePartyMon(struct Pokemon* mon);
u32 CalcMonStealthRockDamage(struct Pokemon* mon);
u32 CalcSteelsurgeDamage(u8 bank);
u32 CalcSteelsurgeDamagePartyMon(struct Pokemon* mon);
u32 GetMonEntryHazardDamage(struct Pokemon* mon, u8 side);
bool8 WillFaintFromEntryHazards(struct Pokemon* mon, u8 side);

//Functions Hooked In
void atkE2_switchoutabilities(void);
void atk61_drawpartystatussummary(void);
void atk4D_switchindataupdate(void);
void atk4F_jumpifcantswitch(void);
void atk8F_forcerandomswitch(void);
void PartyMenuSwitchingUpdate(void);
