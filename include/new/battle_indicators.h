#pragma once

#include "../global.h"
#include "../pokemon.h"

/**
 * \file battle_indicators.h
 * \brief Contains various functions relating to indicators and triggers
 *		  for Mega Evolution, Z-Moves, and Dynamax.
 */

//Exported Functions
void LoadRaidShieldGfx(void);
void LoadMegaGraphics(u8 state);
void CreateMegaIndicatorAfterAnim(void);
void TryLoadIndicatorForEachBank(void);
void DestroyMegaIndicator(u8 bank);
void TryLoadMegaTriggers(void);
void TryLoadZTrigger(void);
void TryLoadDynamaxTrigger(void);
void TryLoadLastUsedBallTrigger(void);
u16 GetLastUsedBall(void);
bool8 CantLoadLastBallTrigger(void);
void TryLoadTeamPreviewTrigger(void);
bool8 CantLoadTeamPreviewTrigger(void);
void DisplayInBattleTeamPreview(void);
void HideInBattleTeamPreview(void);
void DestroyRaidShieldSprite(void);
void TryLoadTeraTrigger(void); // For Terastallization

//Exported Data Structures
extern const struct SpriteTemplate gRaidShieldSpriteTemplate;