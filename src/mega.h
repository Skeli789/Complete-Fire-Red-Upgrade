#pragma once

#include "defines.h"

typedef struct evolution {
	u16 type;
	u16 argument;
	u16 species;
	u16 unknown;
} evolution;

//evolution **evolution_table = (evolution**) 0x08259754;
#define evolution_table *((evolution***) 0x08042F6C)

// an offsetstochangetonewminus1 offset in PokeRoms.ini from G3HS
#define evos_per_poke *((u8*) 0x08043116)

// The evolutionary type for Megas
#define MEGA_EVOLUTION 0xFE

enum MegaEvoVariants
{
	MEGA_VARIANT_STANDARD,
	MEGA_VARIANT_PRIMAL,
	MEGA_VARIANT_WISH, //Rayquaza
	MEGA_VARIANT_ULTRA_BURST, //Necrozma
};

enum MegaGraphicsTags
{
	GFX_TAG_MEGA_INDICATOR = 0xFDF0,
	GFX_TAG_ALPHA_INDICATOR,
	GFX_TAG_OMEGA_INDICATOR,
	GFX_TAG_ULTRA_INDICATOR,
	GFX_TAG_MEGA_TRIGGER,
	GFX_TAG_ULTRA_TRIGGER,
};

enum
{
	MegaTriggerNothing,
	MegaTriggerLightUp,
	MegaTriggerNormalColour,
	MegaTriggerGrayscale,
};
