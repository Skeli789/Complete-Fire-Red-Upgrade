#pragma once

/**
 * \file helper_functions.h
 * \brief Contains battle utility functions.
 */
 
//Exported Functions
bool8 IsTrickRoomActive(void);
bool8 IsMagicRoomActive(void);
bool8 IsWonderRoomActive(void);
bool8 IsGravityActive(void);
bool8 IsIonDelugeActive(void);
bool8 IsFairyLockActive(void);
bool8 IsMudSportActive(void);
bool8 IsWaterSportActive(void);
bool8 IsInverseBattle(void);
bool8 BankSideHasSeaOfFire(u8 bank);
bool8 BankSideHasRainbow(u8 bank);
bool8 BankSideHasSwamp(u8 bank);
bool8 SideHasSwamp(u8 side);
bool8 IsConfused(u8 bank);
bool8 IsTaunted(u8 bank);
bool8 IsTormented(u8 bank);
bool8 IsHealBlocked(u8 bank);
bool8 CantUseSoundMoves(u8 bank);
bool8 IsLaserFocused(u8 bank);
bool8 IsAbilitySuppressed(u8 bank);
bool8 CantScoreACrit(u8 bank, struct Pokemon* mon);
