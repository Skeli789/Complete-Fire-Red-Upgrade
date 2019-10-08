#include "../config.h"
#include "../../include/global.h"
#include "../../include/constants/maps.h"
#include "../../include/wild_encounter.h"
#include "../../include/constants/species.h"
/*
wild_encounter_tables.c
	day/night and/or regular map wild encounter species

tables to edit:
	gWildMonMorningHeaders
	gWildMonEveningHeaders
	gWildMonNightHeaders
	gSwarmTable

*/

#ifndef UNBOUND //Modify this section

const struct WildPokemonHeader gWildMonMorningHeaders[] =
{
	{
        .mapGroup = 0xFF,
        .mapNum = 0xFF,
        .landMonsInfo = NULL,
        .waterMonsInfo = NULL,
        .rockSmashMonsInfo = NULL,
        .fishingMonsInfo = NULL,
    }
};

const struct WildPokemonHeader gWildMonEveningHeaders[] =
{
	{
        .mapGroup = 0xFF,
        .mapNum = 0xFF,
        .landMonsInfo = NULL,
        .waterMonsInfo = NULL,
        .rockSmashMonsInfo = NULL,
        .fishingMonsInfo = NULL,
    }
};

const struct WildPokemonHeader gWildMonNightHeaders[] =
{
	{
        .mapGroup = 0xFF,
        .mapNum = 0xFF,
        .landMonsInfo = NULL,
        .waterMonsInfo = NULL,
        .rockSmashMonsInfo = NULL,
        .fishingMonsInfo = NULL,
    }
};

const struct SwarmData gSwarmTable[] =
{
	/*{
		.mapName = 0xFF,
		.species = 0xFFFF,
	},*/
};

#else //Below is for Pokemon Unbound. Feel free to remove.

#define MAP_NAME_ROUTE_1 0x65
#define MAP_NAME_ROUTE_2 0x66
#define MAP_NAME_FLOWER_PARADISE 0x88
#define MAP_NAME_GRIM_WOODS 0x7E
#define MAP_NAME_ROUTE_4 0x63
#define MAP_NAME_CINDER_VOLCANO 0x7F
#define MAP_NAME_ROUTE_5 0x69
#define MAP_NAME_VALLEY_CAVE 0x83
#define MAP_NAME_ROUTE_6 0x6A
#define MAP_NAME_ROUTE_7 0x6B
#define MAP_NAME_ROUTE_8 0x6C
#define MAP_NAME_ROUTE_9 0x6D
#define MAP_NAME_ROUTE_10 0x64
#define MAP_NAME_ROUTE_11 0x6F
#define MAP_NAME_THUNDERCAP_MT 0x8A
#define MAP_NAME_ROUTE_12 0x70
#define MAP_NAME_VIVILL_WOODS 0x8C
#define MAP_NAME_ROUTE_16 0x74
#define MAP_NAME_ROUTE_17 0x75

const struct WildPokemon gRoute2_LandMonsNight[] =
{
    {9, 11, SPECIES_HOOTHOOT},
    {9, 11, SPECIES_RATTATA},
    {9, 11, SPECIES_RATTATA},
    {9, 11, SPECIES_HOOTHOOT},
    {9, 11, SPECIES_ELECTRIKE},
    {9, 11, SPECIES_AZURILL},
    {9, 11, SPECIES_STUFFUL},
    {9, 11, SPECIES_STUFFUL},
    {10, 11, SPECIES_SCRAGGY},
    {10, 11, SPECIES_RIOLU},
    {11, 11, SPECIES_SCRAGGY},
    {11, 11, SPECIES_RIOLU},
};

const struct WildPokemonInfo gRoute2_LandMonsInfoNight = {21, gRoute2_LandMonsNight};

const struct WildPokemon gRoute3_LandMonsNight[] =
{
    {10, 12, SPECIES_WINGULL},
    {10, 12, SPECIES_SENTRET},
    {10, 12, SPECIES_BIDOOF},
    {10, 12, SPECIES_BIDOOF},
    {10, 12, SPECIES_BUIZEL},
    {10, 12, SPECIES_SHELLOS},
    {10, 12, SPECIES_SURSKIT},
    {10, 12, SPECIES_KRABBY},
    {11, 12, SPECIES_KRABBY},
    {11, 12, SPECIES_KRABBY},
    {12, 12, SPECIES_KRABBY},
    {12, 12, SPECIES_KRABBY},
};

const struct WildPokemonInfo gRoute3_LandMonsInfoNight = {21, gRoute3_LandMonsNight};

const struct WildPokemon gRoute4_LandMonsNight[] =
{
    {15, 17, SPECIES_HOOTHOOT},
    {15, 17, SPECIES_HOOTHOOT},
    {15, 17, SPECIES_LILLIPUP},
    {15, 17, SPECIES_LILLIPUP},
    {15, 17, SPECIES_EKANS},
    {15, 17, SPECIES_NUMEL},
    {15, 17, SPECIES_MANKEY},
    {15, 17, SPECIES_MANKEY},
    {15, 17, SPECIES_NUMEL},
    {15, 17, SPECIES_RALTS},
    {17, 17, SPECIES_NUMEL},
    {17, 17, SPECIES_RALTS},
};

const struct WildPokemonInfo gRoute4_LandMonsInfoNight = {21, gRoute4_LandMonsNight};

const struct WildPokemon gGrimWoods_LandMonsNight[] =
{
    {13, 16, SPECIES_WEEDLE},
    {14, 16, SPECIES_SEEDOT},
    {14, 16, SPECIES_PUMPKABOO},
    {14, 16, SPECIES_PUMPKABOO_M},
    {14, 16, SPECIES_PUMPKABOO_L},
    {14, 16, SPECIES_PUMPKABOO_XL},
    {15, 16, SPECIES_GASTLY},
    {15, 16, SPECIES_SHUPPET},
    {15, 16, SPECIES_DUSKULL},
    {15, 16, SPECIES_PHANTUMP},
    {16, 16, SPECIES_DUSKULL},
    {16, 16, SPECIES_PHANTUMP},
};

const struct WildPokemonInfo gGrimWoods_LandMonsInfoNight = {21, gGrimWoods_LandMonsNight};

const struct WildPokemon gRoute5_LandMonsNight[] =
{
    {15, 18, SPECIES_MURKROW},
    {15, 18, SPECIES_ZUBAT},
    {15, 18, SPECIES_NIDORAN_M},
    {15, 18, SPECIES_NIDORAN_F},
    {16, 19, SPECIES_HOUNDOUR},
    {15, 18, SPECIES_ZUBAT},
    {15, 18, SPECIES_NIDORAN_M},
    {15, 18, SPECIES_NIDORAN_F},
    {17, 19, SPECIES_MURKROW},
    {17, 19, SPECIES_DEDENNE},
    {19, 19, SPECIES_NIDORINA},
    {19, 19, SPECIES_NIDORINO},
};

const struct WildPokemonInfo gRoute5_LandMonsInfoNight = {21, gRoute5_LandMonsNight};

const struct WildPokemon gRoute6_LandMonsMorning[] =
{
    {20, 22, SPECIES_ZIGZAGOON},
    {20, 22, SPECIES_LEDYBA},
    {20, 22, SPECIES_SHINX},
    {20, 22, SPECIES_DEERLING},
    {20, 22, SPECIES_ZIGZAGOON},
    {20, 22, SPECIES_KRICKETOT},
    {21, 22, SPECIES_KRICKETOT},
    {21, 22, SPECIES_KRICKETOT},
    {21, 22, SPECIES_DEERLING},
    {21, 22, SPECIES_KRICKETOT},
    {22, 22, SPECIES_LEDIAN},
    {22, 22, SPECIES_LUXIO},
};

const struct WildPokemonInfo gRoute6_LandMonsInfoMorning = {21, gRoute6_LandMonsMorning};

const struct WildPokemon gRoute6_LandMonsNight[] =
{
    {20, 22, SPECIES_ZIGZAGOON},
    {20, 22, SPECIES_SPINARAK},
    {20, 22, SPECIES_SHINX},
    {20, 22, SPECIES_DEERLING},
    {20, 22, SPECIES_ZIGZAGOON},
    {20, 22, SPECIES_KRICKETOT},
    {21, 22, SPECIES_KRICKETOT},
    {21, 22, SPECIES_KRICKETOT},
    {21, 22, SPECIES_DEERLING},
    {21, 22, SPECIES_KRICKETOT},
    {22, 22, SPECIES_ARIADOS},
    {22, 22, SPECIES_LUXIO},
};

const struct WildPokemonInfo gRoute6_LandMonsInfoNight = {21, gRoute6_LandMonsNight};

const struct WildPokemon gRoute7_LandMonsNight[] =
{
    {21, 23, SPECIES_YUNGOOS},
    {21, 23, SPECIES_DODUO},
    {21, 23, SPECIES_BUNEARY},
    {21, 23, SPECIES_ESPURR},
    {21, 23, SPECIES_ABRA},
    {21, 23, SPECIES_MAREEP},
    {21, 23, SPECIES_SHINX},
    {21, 23, SPECIES_SHINX},
    {22, 23, SPECIES_YUNGOOS},
    {22, 23, SPECIES_BUNEARY},
    {23, 23, SPECIES_LUXIO},
    {23, 23, SPECIES_GUMSHOOS},
};

const struct WildPokemonInfo gRoute7_LandMonsInfoNight = {21, gRoute7_LandMonsNight};

const struct WildPokemon gRoute8_LandMonsNight[] =
{
    {22, 25, SPECIES_SNOVER},
    {22, 25, SPECIES_SNORUNT},
    {22, 25, SPECIES_SNEASEL},
    {22, 25, SPECIES_SNORUNT},
    {22, 25, SPECIES_CUBCHOO},
    {22, 25, SPECIES_DEERLING_WINTER},
    {22, 25, SPECIES_FURRET},
    {22, 25, SPECIES_FLAAFFY},
    {23, 25, SPECIES_SNOVER},
    {23, 25, SPECIES_SNORUNT},
    {23, 25, SPECIES_MINIOR_SHIELD},
    {23, 25, SPECIES_MINIOR_SHIELD},
};

const struct WildPokemonInfo gRoute8_LandMonsInfoNight = {21, gRoute8_LandMonsNight};

const struct WildPokemon gFrozenForest_LandMonsNight[] =
{
    {23, 26, SPECIES_SNOVER},
    {23, 26, SPECIES_SNOVER},
    {23, 26, SPECIES_SNEASEL},
    {23, 26, SPECIES_CUBCHOO},
    {23, 26, SPECIES_CUBCHOO},
    {23, 26, SPECIES_CUBCHOO},
    {23, 26, SPECIES_SNOVER},
    {23, 26, SPECIES_SNEASEL},
    {23, 26, SPECIES_CUBCHOO},
    {23, 26, SPECIES_CUBCHOO},
    {26, 26, SPECIES_CUBCHOO},
    {26, 26, SPECIES_CUBCHOO},
};

const struct WildPokemonInfo gFrozenForest_LandMonsInfoNight = {21, gFrozenForest_LandMonsNight};

const struct WildPokemon gRoute11_LandMonsNight[] =
{
    {26, 29, SPECIES_EXEGGCUTE},
    {26, 29, SPECIES_MEOWTH},
    {26, 29, SPECIES_VENONAT},
    {26, 29, SPECIES_VENONAT},
    {26, 29, SPECIES_SPOINK},
    {26, 29, SPECIES_MEDITITE},
    {27, 29, SPECIES_SPOINK},
    {27, 29, SPECIES_MEDITITE},
    {29, 29, SPECIES_PERSIAN},
    {29, 29, SPECIES_ABSOL},
    {29, 29, SPECIES_PERSIAN},
    {29, 29, SPECIES_ABSOL},
};

const struct WildPokemonInfo gRoute11_LandMonsInfoNight = {21, gRoute11_LandMonsNight};

const struct WildPokemon gRoute12_LandMonsNight[] =
{
    {29, 32, SPECIES_HOOTHOOT},
    {33, 36, SPECIES_ROCKRUFF},
    {33, 36, SPECIES_INKAY},
    {33, 36, SPECIES_MIENFOO},
    {33, 36, SPECIES_PONYTA},
    {33, 36, SPECIES_SNUBBULL},
    {34, 36, SPECIES_INKAY},
    {34, 36, SPECIES_MIENFOO},
    {34, 36, SPECIES_NOCTOWL},
    {33, 36, SPECIES_TOGEDEMARU},
    {34, 36, SPECIES_NOCTOWL},
    {35, 36, SPECIES_TOGEDEMARU},
};

const struct WildPokemonInfo gRoute12_LandMonsInfoNight = {21, gRoute12_LandMonsNight};

const struct WildPokemon gRoute14_LandMonsNight[] =
{
    {37, 40, SPECIES_PELIPPER},
    {37, 40, SPECIES_BIBAREL},
    {37, 40, SPECIES_GOLDUCK},
    {37, 40, SPECIES_GASTRODON},
    {37, 40, SPECIES_FLOATZEL},
    {36, 40, SPECIES_SLOWPOKE},
    {37, 40, SPECIES_GLOOM},
    {37, 40, SPECIES_GLOOM},
    {38, 40, SPECIES_FLOATZEL},
    {37, 40, SPECIES_SLOWPOKE},
    {25, 30, SPECIES_ODDISH},
    {25, 30, SPECIES_ODDISH},
};

const struct WildPokemonInfo gRoute14_LandMonsInfoNight = {21, gRoute14_LandMonsNight};

const struct WildPokemon gVivillWoods_LandMonsNight[] =
{
    {30, 35, SPECIES_WEEDLE},
    {30, 35, SPECIES_WURMPLE},
    {30, 35, SPECIES_SCATTERBUG},
    {38, 41, SPECIES_COTTONEE},
    {39, 41, SPECIES_SWADLOON},
    {38, 41, SPECIES_PETILIL},
    {39, 41, SPECIES_SPEWPA},
    {39, 41, SPECIES_KAKUNA},
    {39, 41, SPECIES_SILCOON},
    {39, 41, SPECIES_CASCOON},
    {39, 41, SPECIES_SILCOON},
    {39, 41, SPECIES_CASCOON},
};

const struct WildPokemonInfo gVivillWoods_LandMonsInfoNight = {21, gVivillWoods_LandMonsNight};

const struct WildPokemon gRoute17_LandMonsNight[] =
{
    {47, 50, SPECIES_NOCTOWL},
    {47, 50, SPECIES_PELIPPER},
    {47, 50, SPECIES_NOCTOWL},
    {47, 50, SPECIES_PELIPPER},
    {47, 50, SPECIES_NOCTOWL},
    {47, 50, SPECIES_NOCTOWL},
    {47, 50, SPECIES_NOCTOWL},
    {47, 50, SPECIES_NOCTOWL},
    {48, 50, SPECIES_ZANGOOSE},
    {48, 50, SPECIES_SEVIPER},
    {40, 45, SPECIES_BAGON},
    {40, 45, SPECIES_BAGON},
};

const struct WildPokemonInfo gRoute17_LandMonsInfoNight = {21, gRoute17_LandMonsNight};

const struct WildPokemonHeader gWildMonMorningHeaders[] =
{
	{
        .mapGroup = MAP_GROUP(ROUTE_6),
        .mapNum = MAP_NUM(ROUTE_6),
        .landMonsInfo = &gRoute6_LandMonsInfoMorning,
        .waterMonsInfo = NULL,
        .rockSmashMonsInfo = NULL,
        .fishingMonsInfo = NULL,
    },
    {
        .mapGroup = 0xFF,
        .mapNum = 0xFF,
        .landMonsInfo = NULL,
        .waterMonsInfo = NULL,
        .rockSmashMonsInfo = NULL,
        .fishingMonsInfo = NULL,
    }
};

const struct WildPokemonHeader gWildMonEveningHeaders[] =
{
    {
        .mapGroup = 0xFF,
        .mapNum = 0xFF,
        .landMonsInfo = NULL,
        .waterMonsInfo = NULL,
        .rockSmashMonsInfo = NULL,
        .fishingMonsInfo = NULL,
    }
};

const struct WildPokemonHeader gWildMonNightHeaders[] =
{
    {
        .mapGroup = MAP_GROUP(ROUTE_2),
        .mapNum = MAP_NUM(ROUTE_2),
        .landMonsInfo = &gRoute2_LandMonsInfoNight,
        .waterMonsInfo = NULL,
        .rockSmashMonsInfo = NULL,
        .fishingMonsInfo = NULL,
    },
    {
        .mapGroup = MAP_GROUP(ROUTE_3),
        .mapNum = MAP_NUM(ROUTE_3),
        .landMonsInfo = &gRoute3_LandMonsInfoNight,
        .waterMonsInfo = NULL,
        .rockSmashMonsInfo = NULL,
        .fishingMonsInfo = NULL,
    },
    {
        .mapGroup = MAP_GROUP(ROUTE_4),
        .mapNum = MAP_NUM(ROUTE_4),
        .landMonsInfo = &gRoute4_LandMonsInfoNight,
        .waterMonsInfo = NULL,
        .rockSmashMonsInfo = NULL,
        .fishingMonsInfo = NULL,
    },
    {
        .mapGroup = MAP_GROUP(GRIM_WOODS),
        .mapNum = MAP_NUM(GRIM_WOODS),
        .landMonsInfo = &gGrimWoods_LandMonsInfoNight,
        .waterMonsInfo = NULL,
        .rockSmashMonsInfo = NULL,
        .fishingMonsInfo = NULL,
    },
    {
        .mapGroup = MAP_GROUP(ROUTE_5),
        .mapNum = MAP_NUM(ROUTE_5),
        .landMonsInfo = &gRoute5_LandMonsInfoNight,
        .waterMonsInfo = NULL,
        .rockSmashMonsInfo = NULL,
        .fishingMonsInfo = NULL,
    },
	{
        .mapGroup = MAP_GROUP(ROUTE_6),
        .mapNum = MAP_NUM(ROUTE_6),
        .landMonsInfo = &gRoute6_LandMonsInfoNight,
        .waterMonsInfo = NULL,
        .rockSmashMonsInfo = NULL,
        .fishingMonsInfo = NULL,
    },
	{
        .mapGroup = MAP_GROUP(ROUTE_7),
        .mapNum = MAP_NUM(ROUTE_7),
        .landMonsInfo = &gRoute7_LandMonsInfoNight,
        .waterMonsInfo = NULL,
        .rockSmashMonsInfo = NULL,
        .fishingMonsInfo = NULL,
    },
	{
        .mapGroup = MAP_GROUP(ROUTE_8),
        .mapNum = MAP_NUM(ROUTE_8),
        .landMonsInfo = &gRoute8_LandMonsInfoNight,
        .waterMonsInfo = NULL,
        .rockSmashMonsInfo = NULL,
        .fishingMonsInfo = NULL,
    },
	{
        .mapGroup = MAP_GROUP(FROZEN_FOREST),
        .mapNum = MAP_NUM(FROZEN_FOREST),
        .landMonsInfo = &gFrozenForest_LandMonsInfoNight,
        .waterMonsInfo = NULL,
        .rockSmashMonsInfo = NULL,
        .fishingMonsInfo = NULL,
    },
	{
        .mapGroup = MAP_GROUP(ROUTE_11),
        .mapNum = MAP_NUM(ROUTE_11),
        .landMonsInfo = &gRoute11_LandMonsInfoNight,
        .waterMonsInfo = NULL,
        .rockSmashMonsInfo = NULL,
        .fishingMonsInfo = NULL,
    },
	{
        .mapGroup = MAP_GROUP(ROUTE_12),
        .mapNum = MAP_NUM(ROUTE_12),
        .landMonsInfo = &gRoute12_LandMonsInfoNight,
        .waterMonsInfo = NULL,
        .rockSmashMonsInfo = NULL,
        .fishingMonsInfo = NULL,
    },
	{
        .mapGroup = MAP_GROUP(ROUTE_14_B),
        .mapNum = MAP_NUM(ROUTE_14_B),
        .landMonsInfo = &gRoute14_LandMonsInfoNight,
        .waterMonsInfo = NULL,
        .rockSmashMonsInfo = NULL,
        .fishingMonsInfo = NULL,
    },
	{
        .mapGroup = MAP_GROUP(ROUTE_14_C),
        .mapNum = MAP_NUM(ROUTE_14_C),
        .landMonsInfo = &gRoute14_LandMonsInfoNight,
        .waterMonsInfo = NULL,
        .rockSmashMonsInfo = NULL,
        .fishingMonsInfo = NULL,
    },
	{
        .mapGroup = MAP_GROUP(VIVILL_WOODS),
        .mapNum = MAP_NUM(VIVILL_WOODS),
        .landMonsInfo = &gVivillWoods_LandMonsInfoNight,
        .waterMonsInfo = NULL,
        .rockSmashMonsInfo = NULL,
        .fishingMonsInfo = NULL,
    },
	{
        .mapGroup = MAP_GROUP(ROUTE_17),
        .mapNum = MAP_NUM(ROUTE_17),
        .landMonsInfo = &gRoute17_LandMonsInfoNight,
        .waterMonsInfo = NULL,
        .rockSmashMonsInfo = NULL,
        .fishingMonsInfo = NULL,
    },
	{
        .mapGroup = 0xFF,
        .mapNum = 0xFF,
        .landMonsInfo = NULL,
        .waterMonsInfo = NULL,
        .rockSmashMonsInfo = NULL,
        .fishingMonsInfo = NULL,
    }
};

const struct SwarmData gSwarmTable[] =
{
	{
		.mapName = MAP_NAME_ROUTE_1,
		.species = SPECIES_SNEASEL,
	},
	{
		.mapName = MAP_NAME_ROUTE_2,
		.species = SPECIES_BURMY,
	},
	{
		.mapName = MAP_NAME_FLOWER_PARADISE,
		.species = SPECIES_CHERUBI,
	},
	{
		.mapName = MAP_NAME_GRIM_WOODS,
		.species = SPECIES_NINCADA,
	},
	{
		.mapName = MAP_NAME_ROUTE_4,
		.species = SPECIES_LITLEO,
	},
	{
		.mapName = MAP_NAME_CINDER_VOLCANO,
		.species = SPECIES_VULPIX,
	},
	{
		.mapName = MAP_NAME_ROUTE_5,
		.species = SPECIES_PURRLOIN,
	},
	{
		.mapName = MAP_NAME_VALLEY_CAVE,
		.species = SPECIES_DUNSPARCE,
	},
	{
		.mapName = MAP_NAME_ROUTE_6,
		.species = SPECIES_SMEARGLE,
	},
	{
		.mapName = MAP_NAME_ROUTE_7,
		.species = SPECIES_MR_MIME,
	},
	{
		.mapName = MAP_NAME_ROUTE_8,
		.species = SPECIES_STANTLER,
	},
	{
		.mapName = MAP_NAME_ROUTE_9,
		.species = SPECIES_KARRABLAST,
	},
	{
		.mapName = MAP_NAME_ROUTE_10,
		.species = SPECIES_SHELMET,
	},
	{
		.mapName = MAP_NAME_ROUTE_11,
		.species = SPECIES_GLAMEOW,
	},
	{
		.mapName = MAP_NAME_THUNDERCAP_MT,
		.species = SPECIES_JOLTIK,
	},
	{
		.mapName = MAP_NAME_THUNDERCAP_MT,
		.species = SPECIES_FERROSEED,
	},
	{
		.mapName = MAP_NAME_ROUTE_12,
		.species = SPECIES_SWIRLIX,
	},
	{
		.mapName = MAP_NAME_ROUTE_12,
		.species = SPECIES_SPRITZEE,
	},
	{
		.mapName = MAP_NAME_VIVILL_WOODS,
		.species = SPECIES_PIKACHU,
	},
	{
		.mapName = MAP_NAME_ROUTE_16,
		.species = SPECIES_GOTHORITA,
	},
	{
		.mapName = MAP_NAME_ROUTE_16,
		.species = SPECIES_DUOSION,
	},
	{
		.mapName = MAP_NAME_ROUTE_17,
		.species = SPECIES_VOLBEAT,
	},
	{
		.mapName = MAP_NAME_ROUTE_17,
		.species = SPECIES_ILLUMISE,
	},
};

#endif

const u16 gSwarmTableLength = ARRAY_COUNT(gSwarmTable);
