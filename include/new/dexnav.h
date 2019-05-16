#pragma once

#include "battle_strings.h"
#include "build_pokemon.h"

#include "../global.h"
#include "../pokemon.h"
#include "../battle.h"
#include "../bg.h"
#include "../event_data.h"
#include "../fieldmap.h"
#include "../field_effect.h"
#include "../palette.h"
#include "../pokemon_icon.h"
#include "../random.h"
#include "../wild_encounter.h"
#include "../window.h"
#include "../sprite.h"
#include "../text.h"
#include "../main.h"
#include "../constants/abilities.h"
#include "../constants/moves.h"
#include "../constants/songs.h"
#include "../constants/species.h"
#include "../constants/items.h"
#include "../gba/io_reg.h"

/**
 * \file dexnav.h
 * \brief Contains a port of the DexNav system created by FBI on Pokecommunity:
 *		  https://www.pokecommunity.com/showthread.php?p=9998647
 */
 
//Exported Functions
u16 RandRange(u16 min, u16 max);

//Functions Hooked In
u8 SelectHookRestore(void);
u8 CheckRegisteredSelect(void);
//u8 PokeToolsFunc(void);
void OeiCaveEffect(void);
u8 ExecDexNav(void);
void ToolSelection(u8 taskId);