#pragma once

#include "../global.h"

/**
 * \file party_menu.h
 * \brief Contains functions relating to viewing Pokemon from the party menu. These include
 *		  the viewing the Pokemon menu screen in multi battles, as well as a feature that
 *		  causes the summary screen to wrap around when viewing the first or last Pokemon
 *		  in the party. It also updates the Pokemon selection for the Battle Tower, implements
 *		  the feature that allows the Player to move items between Pokemon on the party menu,
 *		  as well as field moves.
 */


//Functions Hooked In
void SetNewPartySelectTarget2(s8* highlightedMon, s8 movementDir);
void CursorCb_Summary(u8 taskId);
u8 ChangeSummaryScreenMon(u8 delta);
u8 ChoosePokemon_LoadMaxPKMNStr(const u8** strPtr, bool8 loadString);
void CursorCb_Enter(u8 taskId);
void CursorCb_NoEntry(u8 taskId);
void DisplayPartyPokemonSelectForBattle(u8 slot);
u8 CanPokemonSelectedBeEnteredInBattleTower(void);
bool8 HasBadgeToUseFieldMove(u8 id);
bool8 HasBadgeToUseSurf(void);
bool8 HasBadgeToUseFlash(void);
bool8 HasBadgeToUseWaterfall(void);
bool8 HasBadgeToUseRockClimb(void);
bool8 HasBadgeToUseDive(void);

void sp100_CanPlayerUseFlashInCurrentLocation(void);
void sp101_CanPlayerFlyInCurrentLocation(void);
void sp102_IsPlayerFacingSurfableWater(void);
void sp103_IsPlayerFacingClimbableWaterfall(void);
void sp104_IsPlayerOnDiveableWater(void);
void sp105_IsPlayerFacingClimbableWall(void);
void sp109_IsPlayerFacingNPCWithOverworldPic(void);
void sp10A_CanUseCutOnTree(void);
void sp10B_CanUseRockSmashOnRock(void);
void sp10C_CanUseStrengthOnBoulder(void);

//Exported Data Structures
//extern const struct XX gPartyMenuCursorOptions[];
//extern const struct XX gFieldMoveCursorCallbacks[];
extern const u8* const gFieldMoveDescriptions[];
extern const u16 gFieldMoves[];
extern const u8 gFieldMoveBadgeRequirements[];
extern const u8 gFieldMoveBadgeRequirements[];
extern const u8 gPartyMenu_ItemMenuActions[];

