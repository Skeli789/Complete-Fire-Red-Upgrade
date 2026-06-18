#include "../config.h"
#include "../../include/global.h"
#include "../../include/constants/maps.h"
#include "../../include/wild_encounter.h"
#include "../../include/constants/region_map_sections.h"
#include "../../include/constants/species.h"

/*
wild_encounter_tables.c
	day/night and/or regular map wild encounter species

tables to edit:
	gWildMonMorningHeaders 4-8 AM
	gWildMonDayHeaders 8-5 PM
	gWildMonEveningHeaders 5-8 PM
	gWildMonNightHeaders 8-4 AM
	gSwarmTable (Added an example entry, you can add more!)

*/

const struct WildPokemon gRoute1_LandMonsMorning[] =
{
	{2, 3, SPECIES_ZIGZAGOON},
	{2, 3, SPECIES_TAILLOW},
	{2, 3, SPECIES_LOTAD},
	{2, 3, SPECIES_SHROOMISH},
	{2, 3, SPECIES_PAWMI},
	{2, 3, SPECIES_MINCCINO},
	{2, 3, SPECIES_MAKUHITA},
	{2, 3, SPECIES_FLABEBE},
	{2, 3, SPECIES_WINGULL},
	{2, 2, SPECIES_VULPIX},
	{2, 3, SPECIES_ELECTRIKE},
	{2, 3, SPECIES_RIOLU},
};

const struct WildPokemonInfo gRoute1_LandMonsInfoMorning = {15, gRoute1_LandMonsMorning};

const struct WildPokemon gRoute1_LandMonsDay[] =
{
    {2, 3, SPECIES_ZIGZAGOON},       // Classic early Normal-type
    {2, 3, SPECIES_BUIZEL},          // Water-type, Gen 4 freshness
    {2, 3, SPECIES_ROOKIDEE},        // Flying/Steel later on, Gen 8 bird
    {2, 3, SPECIES_BUNEARY},         // Fast Normal-type with potential
    {2,  3, SPECIES_SEWADDLE},        // Bug/Grass option, early status
    {2,  3, SPECIES_SNOM},            // Unique Ice/Bug type
    {2,  3, SPECIES_JOLTIK},          // Tiny Electric/Bug variety
    {2,  3, SPECIES_HOUNDOUR},        // Fire/Dark, adds good offensive typing
    {1,  3, SPECIES_DEERLING},        // Daytime flavor (Spring form)
    {2,  3, SPECIES_MUNCHLAX},        // Bulky rare Normal-type
    {2,  3, SPECIES_DREEPY},          // Ghost/Dragon rare and fun
    {1,  3, SPECIES_TINKATINK},       // Fairy/Steel rare treasure
};

const struct WildPokemonInfo gRoute1_LandMonsInfoDay = {15, gRoute1_LandMonsDay};

const struct WildPokemon gRoute1_LandMonsEvening[] =
{
    {2, 3, SPECIES_NIDORAN_M},     // Slightly edgy dusk encounter
    {2, 3, SPECIES_NIDORAN_F},     // Pair with above, classic duo
    {2, 3, SPECIES_MURKROW},       // Dusk-themed Flying/Dark
    {2, 3, SPECIES_SNUBBULL},      // Fairy type, temperamental vibes
    {2, 3, SPECIES_BUNEARY},       // Cute, dusk-active rabbit
    {2, 3, SPECIES_NICKIT},        // Sly Gen 8 fox, fits dusk perfectly
    {2, 3, SPECIES_GOSSIFLEUR},    // Soft Grass-type, calm evening wind
    {2, 3, SPECIES_YAMPER},        // Cheerful Electric-type for contrast
    {2, 3, SPECIES_DRIFLOON},      // Ghost-type, creepy dusk lore
    {2, 2, SPECIES_HOUNDOUR},      // Edgy Fire/Dark, good evening fit
    {2, 3, SPECIES_GLAMEOW},       // Mysterious cat, very dusk-like
    {2, 3, SPECIES_TOXEL},         // Spiky, dusk-electric baby
};

const struct WildPokemonInfo gRoute1_LandMonsInfoEvening = {15, gRoute1_LandMonsEvening};

const struct WildPokemon gRoute1_LandMonsNight[] =
{
    {2, 3, SPECIES_HOOTHOOT},       // Classic owl, perfect for night
    {2, 3, SPECIES_POOCHYENA},      // Night-prowling Dark-type
    {2, 3, SPECIES_SPINARAK},       // Nocturnal Bug/Poison
    {2, 3, SPECIES_CUTIEFLY},       // Gentle fluttery fairy at night
    {2, 3, SPECIES_SANDSHREW},      // Ground type, fits cooler nights
    {2, 3, SPECIES_CLEFFA},         // Dreamlike baby Fairy
    {2, 3, SPECIES_FRIGIBAX},       // Cold-blooded Dragon/Ice baby
    {2, 3, SPECIES_LITLEO},         // Night hunting Fire-type
    {2, 3, SPECIES_GASTLY},         // Mandatory spooky ghost 👻
    {2, 2, SPECIES_MISDREAVUS},     // Pure ghost — mysterious and elegant
    {2, 3, SPECIES_ZORUA},          // Trickster at night
    {2, 3, SPECIES_MEOWTH_A},  // Dark-type variant, night stalker
};

const struct WildPokemonInfo gRoute1_LandMonsInfoNight = {15, gRoute1_LandMonsNight};


const struct WildPokemonHeader gWildMonMorningHeaders[] =
{
	{
		.mapGroup = MAP_GROUP(ROUTE_1),
		.mapNum = MAP_NUM(ROUTE_1),
		.landMonsInfo = &gRoute1_LandMonsInfoMorning,
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

const struct WildPokemonHeader gWildMonDayHeaders[] =
{
	{
		.mapGroup = MAP_GROUP(ROUTE_1),
		.mapNum = MAP_NUM(ROUTE_1),
		.landMonsInfo = &gRoute1_LandMonsInfoDay,
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
		.mapGroup = MAP_GROUP(ROUTE_1),
		.mapNum = MAP_NUM(ROUTE_1),
		.landMonsInfo = &gRoute1_LandMonsInfoEvening,
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

const struct WildPokemonHeader gWildMonNightHeaders[] =
{
	{
		.mapGroup = MAP_GROUP(ROUTE_1),
		.mapNum = MAP_NUM(ROUTE_1),
		.landMonsInfo = &gRoute1_LandMonsInfoNight,
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
		.mapName = MAPSEC_ROUTE_1,
		.species = SPECIES_FRIGIBAX,
	},
};

#ifdef SWARM_CHANGE_HOURLY
const u8 gSwarmOrders[31][24] =
{
	{2, 1, 14, 21, 5, 16, 3, 23, 7, 4, 18, 19, 13, 17, 6, 0, 8, 9, 20, 22, 10, 11, 15, 12},
	{16, 19, 13, 23, 7, 9, 21, 17, 22, 8, 4, 3, 6, 0, 11, 20, 12, 1, 10, 2, 15, 14, 18, 5},
	{17, 8, 16, 6, 12, 19, 18, 3, 14, 22, 13, 11, 23, 2, 7, 21, 20, 10, 5, 15, 9, 0, 1, 4},
	{13, 6, 15, 3, 14, 12, 0, 18, 10, 5, 2, 17, 16, 7, 9, 1, 4, 19, 11, 23, 21, 8, 20, 22},
	{12, 7, 18, 13, 10, 2, 5, 14, 11, 23, 20, 22, 8, 15, 17, 4, 19, 16, 0, 6, 21, 1, 9, 3},
	{8, 18, 12, 15, 20, 0, 1, 13, 5, 11, 14, 21, 17, 10, 4, 16, 22, 6, 2, 19, 23, 3, 7, 9},
	{13, 11, 7, 20, 22, 3, 14, 10, 18, 6, 5, 12, 15, 23, 19, 17, 21, 0, 9, 8, 2, 16, 4, 1},
	{11, 20, 4, 2, 16, 17, 13, 12, 0, 10, 15, 14, 1, 19, 5, 8, 9, 18, 7, 21, 3, 6, 22, 23},
	{22, 18, 11, 17, 2, 9, 20, 16, 0, 13, 1, 6, 3, 14, 15, 10, 5, 8, 23, 4, 12, 21, 19, 7},
	{2, 5, 11, 7, 3, 23, 19, 1, 9, 16, 13, 15, 21, 8, 20, 12, 18, 4, 14, 10, 6, 17, 22, 0},
	{10, 18, 5, 1, 13, 15, 22, 2, 4, 3, 17, 9, 19, 21, 0, 8, 16, 11, 12, 14, 7, 23, 6, 20},
	{6, 13, 0, 4, 22, 10, 23, 17, 8, 1, 5, 11, 2, 3, 12, 19, 15, 20, 18, 9, 14, 7, 21, 16},
	{0, 22, 17, 11, 8, 15, 9, 4, 2, 7, 21, 13, 12, 6, 14, 18, 10, 23, 1, 3, 5, 20, 16, 19},
	{15, 16, 20, 5, 4, 6, 2, 21, 3, 19, 7, 8, 14, 13, 10, 23, 11, 18, 22, 0, 17, 12, 1, 9},
	{7, 12, 2, 0, 18, 4, 16, 8, 20, 17, 22, 23, 9, 1, 5, 14, 6, 21, 15, 3, 13, 19, 10, 11},
	{21, 4, 8, 14, 15, 7, 17, 19, 6, 0, 9, 1, 20, 11, 2, 23, 12, 18, 3, 16, 22, 13, 5, 10},
	{5, 3, 9, 19, 6, 14, 15, 10, 21, 7, 12, 0, 20, 16, 23, 13, 1, 22, 8, 11, 4, 2, 17, 18},
	{18, 0, 1, 19, 16, 21, 4, 5, 2, 9, 23, 7, 20, 12, 22, 15, 3, 6, 8, 11, 13, 17, 14, 10},
	{15, 16, 21, 9, 19, 6, 12, 20, 13, 18, 8, 0, 10, 22, 3, 7, 23, 5, 4, 17, 11, 2, 14, 1},
	{3, 14, 20, 16, 15, 13, 8, 4, 1, 21, 10, 5, 12, 9, 23, 6, 0, 17, 19, 7, 11, 22, 2, 18},
	{9, 7, 23, 12, 0, 1, 4, 15, 19, 17, 10, 2, 22, 5, 16, 11, 3, 6, 21, 13, 20, 18, 8, 14},
	{1, 21, 19, 17, 15, 23, 10, 6, 9, 12, 11, 0, 4, 3, 8, 22, 7, 14, 16, 20, 18, 5, 13, 2},
	{4, 9, 22, 12, 23, 11, 7, 5, 6, 2, 21, 20, 18, 14, 1, 10, 3, 15, 13, 17, 16, 19, 8, 0},
	{1, 2, 3, 10, 17, 5, 4, 22, 12, 20, 14, 9, 0, 18, 23, 15, 7, 13, 21, 11, 8, 19, 16, 6},
	{20, 23, 0, 19, 8, 18, 11, 22, 15, 14, 6, 2, 4, 12, 13, 3, 17, 7, 21, 16, 1, 9, 5, 10},
	{23, 10, 17, 19, 9, 14, 6, 7, 16, 12, 15, 18, 0, 4, 11, 2, 1, 3, 8, 5, 20, 22, 13, 21},
	{15, 21, 6, 18, 11, 22, 8, 4, 1, 7, 19, 16, 5, 20, 3, 10, 14, 2, 17, 12, 0, 9, 23, 13},
	{14, 17, 10, 9, 1, 8, 11, 7, 23, 15, 0, 5, 20, 19, 2, 16, 13, 3, 6, 18, 22, 4, 12, 21},
	{19, 15, 6, 22, 0, 20, 23, 11, 17, 14, 13, 16, 10, 1, 21, 9, 2, 12, 4, 7, 18, 5, 3, 8},
	{6, 4, 19, 16, 21, 22, 23, 9, 8, 20, 3, 10, 11, 12, 18, 5, 14, 2, 13, 1, 7, 15, 0, 17},
	{3, 18, 22, 8, 6, 1, 2, 0, 12, 14, 16, 4, 7, 9, 5, 13, 23, 17, 21, 20, 19, 10, 11, 15},
};
#endif

const u16 gSwarmTableLength = NELEMS(gSwarmTable);
