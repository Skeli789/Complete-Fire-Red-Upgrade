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

// Mega evolution variants
#define MEGA_VARIANT_STANDARD 0
#define MEGA_VARIANT_PRIMAL 1
#define MEGA_VARIANT_WISH 2 // Rayquaza
#define MEGA_VARIANT_ULTRA_BURST 3 // Necrozma