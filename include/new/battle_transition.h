#pragma once

#include "../global.h"
#include "../pokemon.h"

/**
 * \file battle_transition.h
 * \brief Contains functions relating to the transition from the Oveworld to battles.
 *		  Mainly focuses on trainer mugshots.
 */

//Functions Hooked In
bool8 BT_Phase2Mugshot_LoadGfx(struct Task* task);
void Mugshots_CreateOpponentPlayerSprites(struct Task* task);
bool8 BT_Phase2Mugshot_StartSpriteSlide(struct Task* task);
bool8 BT_Phase2Mugshot_WaitForOpponentInPlace(struct Task *task);
bool8 BT_Phase2Mugshot_StartBlackFade(struct Task *task);
bool8 ShiftTrainerMugshotSprite(struct Sprite *sprite);

//Exported Data Structures
struct BattleTransitionLogo
{
	u8 trainerClass;
	const u8* tiles;
	const u16* pal;
	const u8* map;
};

extern const struct BattleTransitionLogo gBattleTransitionLogos[];
extern const u8 gNumBattleTransitionLogos;

//Exported Constants

//VAR_PRE_BATTLE_MUGSHOT_STYLE
enum
{
    MUGSHOT_BIG = 0,
    MUGSHOT_DP,
    MUGSHOT_TWO_BARS,
};

//VAR_PRE_BATTLE_MUGSHOT_SPRITE
enum
{
    MUGSHOT_PLAYER = 0,
    MUGSHOT_VS_SYMBOL,
};
