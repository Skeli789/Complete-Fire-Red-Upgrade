#include "..\\defines.h"

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
        .landMonsInfo = NULL
        .waterMonsInfo = NULL,
        .rockSmashMonsInfo = NULL,
        .fishingMonsInfo = NULL,
    }
};

const struct SwarmData gSwarmTable[] =
{
	{
		.mapName = 0xFF,
		.species = 0xFFFF,
	},
};

#else //Below is for Pokemon Unbound. Feel free to remove.

#define MAP_ROUTE_2			((3 << 8) | 69)
#define MAP_ROUTE_3			((3 << 8) | 70)
#define MAP_ROUTE_4			((3 << 8) | 72)
#define MAP_GRIM_WOODS		((45 << 8) | 1)
#define MAP_ROUTE_5			((3 << 8) | 73)
#define MAP_ROUTE_6			((3 << 8) | 75)
#define MAP_ROUTE_7			((3 << 8) | 76)
#define MAP_ROUTE_8			((3 << 8) | 77)
#define MAP_FROZEN_FOREST	((1 << 8) | 81)
#define MAP_ROUTE_11		((3 << 8) | 85)
#define MAP_ROUTE_12		((3 << 8) | 88)
#define MAP_ROUTE_14_B		((3 << 8) | 96)
#define MAP_ROUTE_14_C		((3 << 8) | 97)
#define MAP_VIVILL_WOODS	((1 << 8) | 99)
#define MAP_ROUTE_17		((3 << 8) | 104)

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
    {9, 11, PKMN_HOOTHOOT},
    {9, 11, PKMN_RATTATA},
    {9, 11, PKMN_RATTATA},
    {9, 11, PKMN_HOOTHOOT},
    {9, 11, PKMN_ELECTRIKE},
    {9, 11, PKMN_AZURILL},
    {9, 11, PKMN_STUFFUL},
    {9, 11, PKMN_STUFFUL},
    {10, 11, PKMN_SCRAGGY},
    {10, 11, PKMN_RIOLU},
    {11, 11, PKMN_SCRAGGY},
    {11, 11, PKMN_RIOLU},
};

const struct WildPokemonInfo gRoute2_LandMonsInfoNight = {21, gRoute2_LandMonsNight};

const struct WildPokemon gRoute3_LandMonsNight[] =
{
    {10, 12, PKMN_WINGULL},
    {10, 12, PKMN_SENTRET},
    {10, 12, PKMN_BIDOOF},
    {10, 12, PKMN_BIDOOF},
    {10, 12, PKMN_BUIZEL},
    {10, 12, PKMN_SHELLOS},
    {10, 12, PKMN_SURSKIT},
    {10, 12, PKMN_KRABBY},
    {11, 12, PKMN_KRABBY},
    {11, 12, PKMN_KRABBY},
    {12, 12, PKMN_KRABBY},
    {12, 12, PKMN_KRABBY},
};

const struct WildPokemonInfo gRoute3_LandMonsInfoNight = {21, gRoute3_LandMonsNight};

const struct WildPokemon gRoute4_LandMonsNight[] =
{
    {15, 17, PKMN_HOOTHOOT},
    {15, 17, PKMN_HOOTHOOT},
    {15, 17, PKMN_LILLIPUP},
    {15, 17, PKMN_LILLIPUP},
    {15, 17, PKMN_EKANS},
    {15, 17, PKMN_NUMEL},
    {15, 17, PKMN_MANKEY},
    {15, 17, PKMN_MANKEY},
    {15, 17, PKMN_NUMEL},
    {15, 17, PKMN_RALTS},
    {17, 17, PKMN_NUMEL},
    {17, 17, PKMN_RALTS},
};

const struct WildPokemonInfo gRoute4_LandMonsInfoNight = {21, gRoute4_LandMonsNight};

const struct WildPokemon gGrimWoods_LandMonsNight[] =
{
    {13, 16, PKMN_WEEDLE},
    {14, 16, PKMN_SEEDOT},
    {14, 16, PKMN_PUMPKABOO},
    {14, 16, PKMN_PUMPKABOO_M},
    {14, 16, PKMN_PUMPKABOO_L},
    {14, 16, PKMN_PUMPKABOO_XL},
    {15, 16, PKMN_GASTLY},
    {15, 16, PKMN_SHUPPET},
    {15, 16, PKMN_DUSKULL},
    {15, 16, PKMN_PHANTUMP},
    {16, 16, PKMN_DUSKULL},
    {16, 16, PKMN_PHANTUMP},
};

const struct WildPokemonInfo gGrimWoods_LandMonsInfoNight = {21, gGrimWoods_LandMonsNight};

const struct WildPokemon gRoute5_LandMonsNight[] =
{
    {15, 18, PKMN_MURKROW},
    {15, 18, PKMN_ZUBAT},
    {15, 18, PKMN_NIDORANM},
    {15, 18, PKMN_NIDORANF},
    {16, 19, PKMN_HOUNDOUR},
    {15, 18, PKMN_ZUBAT},
    {15, 18, PKMN_NIDORANM},
    {15, 18, PKMN_NIDORANF},
    {17, 19, PKMN_MURKROW},
    {17, 19, PKMN_DEDENNE},
    {19, 19, PKMN_NIDORINA},
    {19, 19, PKMN_NIDORINO},
};

const struct WildPokemonInfo gRoute5_LandMonsInfoNight = {21, gRoute5_LandMonsNight};

const struct WildPokemon gRoute6_LandMonsMorning[] =
{
    {20, 22, PKMN_ZIGZAGOON},
    {20, 22, PKMN_LEDYBA},
    {20, 22, PKMN_SHINX},
    {20, 22, PKMN_DEERLING},
    {20, 22, PKMN_ZIGZAGOON},
    {20, 22, PKMN_KRICKETOT},
    {21, 22, PKMN_KRICKETOT},
    {21, 22, PKMN_KRICKETOT},
    {21, 22, PKMN_DEERLING},
    {21, 22, PKMN_KRICKETOT},
    {22, 22, PKMN_LEDIAN},
    {22, 22, PKMN_LUXIO},
};

const struct WildPokemonInfo gRoute6_LandMonsInfoMorning = {21, gRoute6_LandMonsMorning};

const struct WildPokemon gRoute6_LandMonsNight[] =
{
    {20, 22, PKMN_ZIGZAGOON},
    {20, 22, PKMN_SPINARAK},
    {20, 22, PKMN_SHINX},
    {20, 22, PKMN_DEERLING},
    {20, 22, PKMN_ZIGZAGOON},
    {20, 22, PKMN_KRICKETOT},
    {21, 22, PKMN_KRICKETOT},
    {21, 22, PKMN_KRICKETOT},
    {21, 22, PKMN_DEERLING},
    {21, 22, PKMN_KRICKETOT},
    {22, 22, PKMN_ARIADOS},
    {22, 22, PKMN_LUXIO},
};

const struct WildPokemonInfo gRoute6_LandMonsInfoNight = {21, gRoute6_LandMonsNight};

const struct WildPokemon gRoute7_LandMonsNight[] =
{
    {21, 23, PKMN_YUNGOOS},
    {21, 23, PKMN_DODUO},
    {21, 23, PKMN_BUNEARY},
    {21, 23, PKMN_ESPURR},
    {21, 23, PKMN_ABRA},
    {21, 23, PKMN_MAREEP},
    {21, 23, PKMN_SHINX},
    {21, 23, PKMN_SHINX},
    {22, 23, PKMN_YUNGOOS},
    {22, 23, PKMN_BUNEARY},
    {23, 23, PKMN_LUXIO},
    {23, 23, PKMN_GUMSHOOS},
};

const struct WildPokemonInfo gRoute7_LandMonsInfoNight = {21, gRoute7_LandMonsNight};

const struct WildPokemon gRoute8_LandMonsNight[] =
{
    {22, 25, PKMN_SNOVER},
    {22, 25, PKMN_SNORUNT},
    {22, 25, PKMN_SNEASEL},
    {22, 25, PKMN_SNORUNT},
    {22, 25, PKMN_CUBCHOO},
    {22, 25, PKMN_DEERLING_WINTER},
    {22, 25, PKMN_FURRET},
    {22, 25, PKMN_FLAAFFY},
    {23, 25, PKMN_SNOVER},
    {23, 25, PKMN_SNORUNT},
    {23, 25, PKMN_MINIORSHIELD},
    {23, 25, PKMN_MINIORSHIELD},
};

const struct WildPokemonInfo gRoute8_LandMonsInfoNight = {21, gRoute8_LandMonsNight};

const struct WildPokemon gFrozenForest_LandMonsNight[] =
{
    {23, 26, PKMN_SNOVER},
    {23, 26, PKMN_SNOVER},
    {23, 26, PKMN_SNEASEL},
    {23, 26, PKMN_CUBCHOO},
    {23, 26, PKMN_CUBCHOO},
    {23, 26, PKMN_CUBCHOO},
    {23, 26, PKMN_SNOVER},
    {23, 26, PKMN_SNEASEL},
    {23, 26, PKMN_CUBCHOO},
    {23, 26, PKMN_CUBCHOO},
    {26, 26, PKMN_CUBCHOO},
    {26, 26, PKMN_CUBCHOO},
};

const struct WildPokemonInfo gFrozenForest_LandMonsInfoNight = {21, gFrozenForest_LandMonsNight};

const struct WildPokemon gRoute11_LandMonsNight[] =
{
    {26, 29, PKMN_EXEGGCUTE},
    {26, 29, PKMN_MEOWTH},
    {26, 29, PKMN_VENONAT},
    {26, 29, PKMN_VENONAT},
    {26, 29, PKMN_SPOINK},
    {26, 29, PKMN_MEDITITE},
    {27, 29, PKMN_SPOINK},
    {27, 29, PKMN_MEDITITE},
    {29, 29, PKMN_PERSIAN},
    {29, 29, PKMN_ABSOL},
    {29, 29, PKMN_PERSIAN},
    {29, 29, PKMN_ABSOL},
};

const struct WildPokemonInfo gRoute11_LandMonsInfoNight = {21, gRoute11_LandMonsNight};

const struct WildPokemon gRoute12_LandMonsNight[] =
{
    {29, 32, PKMN_HOOTHOOT},
    {33, 36, PKMN_ROCKRUFF},
    {33, 36, PKMN_INKAY},
    {33, 36, PKMN_MIENFOO},
    {33, 36, PKMN_PONYTA},
    {33, 36, PKMN_SNUBBULL},
    {34, 36, PKMN_INKAY},
    {34, 36, PKMN_MIENFOO},
    {34, 36, PKMN_NOCTOWL},
    {33, 36, PKMN_TOGEDEMARU},
    {34, 36, PKMN_NOCTOWL},
    {35, 36, PKMN_TOGEDEMARU},
};

const struct WildPokemonInfo gRoute12_LandMonsInfoNight = {21, gRoute12_LandMonsNight};

const struct WildPokemon gRoute14_LandMonsNight[] =
{
    {37, 40, PKMN_PELIPPER},
    {37, 40, PKMN_BIBAREL},
    {37, 40, PKMN_GOLDUCK},
    {37, 40, PKMN_GASTRODON},
    {37, 40, PKMN_FLOATZEL},
    {36, 40, PKMN_SLOWPOKE},
    {37, 40, PKMN_GLOOM},
    {37, 40, PKMN_GLOOM},
    {38, 40, PKMN_FLOATZEL},
    {37, 40, PKMN_SLOWPOKE},
    {25, 30, PKMN_ODDISH},
    {25, 30, PKMN_ODDISH},
};

const struct WildPokemonInfo gRoute14_LandMonsInfoNight = {21, gRoute14_LandMonsNight};

const struct WildPokemon gVivillWoods_LandMonsNight[] =
{
    {30, 35, PKMN_WEEDLE},
    {30, 35, PKMN_WURMPLE},
    {30, 35, PKMN_SCATTERBUG},
    {38, 41, PKMN_COTTONEE},
    {39, 41, PKMN_SWADLOON},
    {38, 41, PKMN_PETILIL},
    {39, 41, PKMN_SPEWPA},
    {39, 41, PKMN_KAKUNA},
    {39, 41, PKMN_SILCOON},
    {39, 41, PKMN_CASCOON},
    {39, 41, PKMN_SILCOON},
    {39, 41, PKMN_CASCOON},
};

const struct WildPokemonInfo gVivillWoods_LandMonsInfoNight = {21, gVivillWoods_LandMonsNight};

const struct WildPokemon gRoute17_LandMonsNight[] =
{
    {47, 50, PKMN_NOCTOWL},
    {47, 50, PKMN_PELIPPER},
    {47, 50, PKMN_NOCTOWL},
    {47, 50, PKMN_PELIPPER},
    {47, 50, PKMN_NOCTOWL},
    {47, 50, PKMN_NOCTOWL},
    {47, 50, PKMN_NOCTOWL},
    {47, 50, PKMN_NOCTOWL},
    {48, 50, PKMN_ZANGOOSE},
    {48, 50, PKMN_SEVIPER},
    {40, 45, PKMN_BAGON},
    {40, 45, PKMN_BAGON},
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
		.species = PKMN_SNEASEL,
	},
	{
		.mapName = MAP_NAME_ROUTE_2,
		.species = PKMN_BURMY,
	},
	{
		.mapName = MAP_NAME_FLOWER_PARADISE,
		.species = PKMN_CHERUBI,
	},
	{
		.mapName = MAP_NAME_GRIM_WOODS,
		.species = PKMN_NINCADA,
	},
	{
		.mapName = MAP_NAME_ROUTE_4,
		.species = PKMN_LITLEO,
	},
	{
		.mapName = MAP_NAME_CINDER_VOLCANO,
		.species = PKMN_VULPIX,
	},
	{
		.mapName = MAP_NAME_ROUTE_5,
		.species = PKMN_PURRLOIN,
	},
	{
		.mapName = MAP_NAME_VALLEY_CAVE,
		.species = PKMN_DUNSPARCE,
	},
	{
		.mapName = MAP_NAME_ROUTE_6,
		.species = PKMN_SMEARGLE,
	},
	{
		.mapName = MAP_NAME_ROUTE_7,
		.species = PKMN_MRMIME,
	},
	{
		.mapName = MAP_NAME_ROUTE_8,
		.species = PKMN_STANTLER,
	},
	{
		.mapName = MAP_NAME_ROUTE_9,
		.species = PKMN_KARRABLAST,
	},
	{
		.mapName = MAP_NAME_ROUTE_10,
		.species = PKMN_SHELMET,
	},
	{
		.mapName = MAP_NAME_ROUTE_11,
		.species = PKMN_GLAMEOW,
	},
	{
		.mapName = MAP_NAME_THUNDERCAP_MT,
		.species = PKMN_JOLTIK,
	},
	{
		.mapName = MAP_NAME_THUNDERCAP_MT,
		.species = PKMN_FERROSEED,
	},
	{
		.mapName = MAP_NAME_ROUTE_12,
		.species = PKMN_SWIRLIX,
	},
	{
		.mapName = MAP_NAME_ROUTE_12,
		.species = PKMN_SPRITZEE,
	},
	{
		.mapName = MAP_NAME_VIVILL_WOODS,
		.species = PKMN_PIKACHU,
	},
	{
		.mapName = MAP_NAME_ROUTE_16,
		.species = PKMN_GOTHORITA,
	},
	{
		.mapName = MAP_NAME_ROUTE_16,
		.species = PKMN_DUOSION,
	},
	{
		.mapName = MAP_NAME_ROUTE_17,
		.species = PKMN_VOLBEAT,
	},
	{
		.mapName = MAP_NAME_ROUTE_17,
		.species = PKMN_ILLUMISE,
	},
	{
		.mapName = 0xFF,
		.species = 0xFFFF,
	},
};

#endif