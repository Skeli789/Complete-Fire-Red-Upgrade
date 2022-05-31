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
	gWildMonMorningHeaders
	gWildMonEveningHeaders
	gWildMonNightHeaders
	gSwarmTable

*/

const struct WildPokemon gRoute1_LandMonsDay[] =
{
	{3, 3, SPECIES_RATTATA},
	{3, 3, SPECIES_PIDGEY},
	{3, 3, SPECIES_SENTRET},
	{3, 3, SPECIES_HOOTHOOT},
	{3, 3, SPECIES_BIDOOF},
	{3, 3, SPECIES_ZIGZAGOON},
	{3, 3, SPECIES_TAILLOW},
	{3, 3, SPECIES_PATRAT},
	{3, 3, SPECIES_STARLY},
	{3, 3, SPECIES_FLETCHLING},
	{3, 3, SPECIES_BUNNELBY},
	{3, 3, SPECIES_PICHU},
};

const struct WildPokemonInfo gRoute1_LandMonsDayInfo = { 21, gRoute1_LandMonsDay };

const struct WildPokemon gRoute1_LandMonsNight[] =
{
	{3, 3, SPECIES_RATTATA_A},
	{3, 3, SPECIES_PIDGEY},
	{3, 3, SPECIES_SENTRET},
	{3, 3, SPECIES_HOOTHOOT},
	{3, 3, SPECIES_BIDOOF},
	{3, 3, SPECIES_ZIGZAGOON_G},
	{3, 3, SPECIES_TAILLOW},
	{3, 3, SPECIES_LILLIPUP},
	{3, 3, SPECIES_STARLY},
	{3, 3, SPECIES_FLETCHLING},
	{3, 3, SPECIES_BUNNELBY},
	{3, 3, SPECIES_ELEKID},
};

const struct WildPokemonInfo gRoute1_LandMonsNightInfo = { 21, gRoute1_LandMonsNight };

const struct WildPokemon gRoute22_LandMonsDay[] =
{
	{3, 4, SPECIES_SPEAROW},
	{3, 4, SPECIES_MANKEY},
	{3, 4, SPECIES_PIDOVE},
	{3, 4, SPECIES_EKANS},
	{3, 4, SPECIES_AZURILL},
	{3, 4, SPECIES_HOPPIP},
	{3, 4, SPECIES_SNUBBULL},
	{3, 4, SPECIES_MAKUHITA},
	{3, 4, SPECIES_SCRAGGY},
	{3, 4, SPECIES_TIMBURR},
	{3, 4, SPECIES_MINCCINO},
	{3, 4, SPECIES_BUDEW},
};

const struct WildPokemonInfo gRoute22_LandMonsDayInfo = { 21, gRoute22_LandMonsDay };

const struct WildPokemon gRoute22_LandMonsNight[] =
{
	{3, 4, SPECIES_SPEAROW},
	{3, 4, SPECIES_MACHOP},
	{3, 4, SPECIES_PIDOVE},
	{3, 4, SPECIES_EKANS},
	{3, 4, SPECIES_WYNAUT},
	{3, 4, SPECIES_HOPPIP},
	{3, 4, SPECIES_SNUBBULL},
	{3, 4, SPECIES_MAKUHITA},
	{3, 4, SPECIES_SCRAGGY},
	{3, 4, SPECIES_TIMBURR},
	{3, 4, SPECIES_POOCHYENA},
	{3, 4, SPECIES_BUDEW},
};

const struct WildPokemonInfo gRoute22_LandMonsNightInfo = { 22, gRoute22_LandMonsNight };

const struct WildPokemon gViridianCity_LandMonsDay[] =
{
	{3, 4, SPECIES_SKWOVET},
	{3, 4, SPECIES_YUNGOOS},
	{3, 4, SPECIES_PIKIPEK},
	{3, 4, SPECIES_WOOLOO},
	{3, 4, SPECIES_ROOKIDEE},
	{3, 4, SPECIES_NICKIT},
	{3, 4, SPECIES_GOSSIFLEUR},
	{3, 4, SPECIES_YAMPER},
	{3, 4, SPECIES_SILICOBRA},
	{3, 4, SPECIES_CHEWTLE},
	{3, 4, SPECIES_IMPIDIMP},
	{3, 4, SPECIES_HATENNA},
};

const struct WildPokemonInfo gViridianCity_LandMonsDayInfo = { 21, gViridianCity_LandMonsDay };

const struct WildPokemon gViridianCity_LandMonsNight[] =
{
	{3, 4, SPECIES_SKWOVET},
	{3, 4, SPECIES_YUNGOOS},
	{3, 4, SPECIES_PIKIPEK},
	{3, 4, SPECIES_GRUBBIN},
	{3, 4, SPECIES_ROOKIDEE},
	{3, 4, SPECIES_CUTIEFLY},
	{3, 4, SPECIES_ROCKRUFF},
	{3, 4, SPECIES_BOUNSWEET},
	{3, 4, SPECIES_STUFFUL},
	{3, 4, SPECIES_WIMPOD},
	{3, 4, SPECIES_MORELULL},
	{3, 4, SPECIES_FOMANTIS},
};

const struct WildPokemonInfo gViridianCity_LandMonsNightInfo = { 22, gViridianCity_LandMonsNight };

const struct WildPokemon gRoute2_LandMonsDay[] =
{
	{3, 5, SPECIES_PURRLOIN},
	{3, 5, SPECIES_SHINX},
	{3, 5, SPECIES_PANSAGE},
	{3, 5, SPECIES_PANSEAR},
	{3, 5, SPECIES_PANPOUR},
	{3, 5, SPECIES_GLAMEOW},
	{3, 5, SPECIES_CROAGUNK},
	{3, 5, SPECIES_CHERUBI},
	{3, 5, SPECIES_SKORUPI},
	{3, 5, SPECIES_BONSLY},
	{3, 5, SPECIES_HAPPINY},
	{3, 5, SPECIES_MIME_JR},
};

const struct WildPokemonInfo gRoute2_LandMonsDayInfo = { 21, gRoute2_LandMonsDay };

const struct WildPokemon gRoute2_LandMonsNight[] =
{
	{3, 5, SPECIES_PURRLOIN},
	{3, 5, SPECIES_MAREEP},
	{3, 5, SPECIES_PANSAGE},
	{3, 5, SPECIES_PANSEAR},
	{3, 5, SPECIES_PANPOUR},
	{3, 5, SPECIES_GLAMEOW},
	{3, 5, SPECIES_GULPIN},
	{3, 5, SPECIES_SKIDDO},
	{3, 5, SPECIES_SKORUPI},
	{3, 5, SPECIES_BONSLY},
	{3, 5, SPECIES_CHINGLING},
	{3, 5, SPECIES_TOGEPI},
};

const struct WildPokemonInfo gRoute2_LandMonsNightInfo = { 22, gRoute2_LandMonsNight };

const struct WildPokemon gViridianForest1_LandMonsDay[] =
{
	{3, 6, SPECIES_CATERPIE},
	{3, 6, SPECIES_WEEDLE},
	{3, 6, SPECIES_BELLSPROUT},
	{3, 6, SPECIES_VENONAT},
	{3, 6, SPECIES_LEDYBA},
	{3, 6, SPECIES_WURMPLE},
	{3, 6, SPECIES_PIKACHU},
	{3, 6, SPECIES_SEEDOT},
	{3, 6, SPECIES_SHROOMISH},
	{3, 6, SPECIES_ILLUMISE},
	{3, 6, SPECIES_KRICKETOT},
	{3, 6, SPECIES_COMBEE},
};

const struct WildPokemonInfo gViridianForest1_LandMonsDayInfo = { 5, gViridianForest1_LandMonsDay };

const struct WildPokemon gViridianForest1_LandMonsNight[] =
{
	{3, 6, SPECIES_CATERPIE},
	{3, 6, SPECIES_WEEDLE},
	{3, 6, SPECIES_ODDISH},
	{3, 6, SPECIES_PARAS},
	{3, 6, SPECIES_SPINARAK},
	{3, 6, SPECIES_WURMPLE},
	{3, 6, SPECIES_PIKACHU},
	{3, 6, SPECIES_LOTAD},
	{3, 6, SPECIES_SHROOMISH},
	{3, 6, SPECIES_VOLBEAT},
	{3, 6, SPECIES_KRICKETOT},
	{3, 6, SPECIES_COMBEE},
};

const struct WildPokemonInfo gViridianForest1_LandMonsNightInfo = { 5, gViridianForest1_LandMonsNight };

const struct WildPokemon gViridianForest2_LandMonsDay[] =
{
	{3, 6, SPECIES_SEWADDLE},
	{3, 6, SPECIES_VENIPEDE},
	{3, 6, SPECIES_DWEBBLE},
	{3, 6, SPECIES_KARRABLAST},
	{3, 6, SPECIES_JOLTIK},
	{3, 6, SPECIES_FERROSEED},
	{3, 6, SPECIES_SCATTERBUG},
	{3, 6, SPECIES_FLABEBE},
	{3, 6, SPECIES_GRUBBIN},
	{3, 6, SPECIES_DEWPIDER},
	{3, 6, SPECIES_BLIPBUG},
	{3, 6, SPECIES_SIZZLIPEDE},
};

const struct WildPokemonInfo gViridianForest2_LandMonsDayInfo = { 5, gViridianForest2_LandMonsDay };

const struct WildPokemon gViridianForest2_LandMonsNight[] =
{
	{3, 6, SPECIES_SEWADDLE},
	{3, 6, SPECIES_VENIPEDE},
	{3, 6, SPECIES_DWEBBLE},
	{3, 6, SPECIES_SHELMET},
	{3, 6, SPECIES_JOLTIK},
	{3, 6, SPECIES_FERROSEED},
	{3, 6, SPECIES_SCATTERBUG},
	{3, 6, SPECIES_FLABEBE},
	{3, 6, SPECIES_GRUBBIN},
	{3, 6, SPECIES_SNOM},
	{3, 6, SPECIES_SURSKIT},
	{3, 6, SPECIES_SIZZLIPEDE},
};

const struct WildPokemonInfo gViridianForest2_LandMonsNightInfo = { 5, gViridianForest2_LandMonsNight };

const struct WildPokemon gDiglettCave1_LandMonsDay[] =
{
	{5, 7, SPECIES_DIGLETT},
	{5, 7, SPECIES_SANDSHREW},
	{5, 7, SPECIES_PHANPY},
	{5, 7, SPECIES_NUMEL},
	{5, 7, SPECIES_WHISMUR},
	{5, 7, SPECIES_SWINUB},
	{5, 7, SPECIES_HIPPOPOTAS},
	{5, 7, SPECIES_GIBLE},
	{5, 7, SPECIES_SANDILE},
	{5, 7, SPECIES_MUDBRAY},
	{5, 7, SPECIES_SANDYGAST},
	{5, 7, SPECIES_YAMASK},
};

const struct WildPokemonInfo gDiglettCave1_LandMonsDayInfo = { 5, gDiglettCave1_LandMonsDay };

const struct WildPokemon gDiglettCave1_LandMonsNight[] =
{
	{5, 7, SPECIES_DIGLETT_A},
	{5, 7, SPECIES_SANDSHREW_A},
	{5, 7, SPECIES_PHANPY},
	{5, 7, SPECIES_NUMEL},
	{5, 7, SPECIES_WHISMUR},
	{5, 7, SPECIES_SWINUB},
	{5, 7, SPECIES_HIPPOPOTAS_F},
	{5, 7, SPECIES_GIBLE},
	{5, 7, SPECIES_SANDILE},
	{5, 7, SPECIES_MUDBRAY},
	{5, 7, SPECIES_SANDYGAST},
	{5, 7, SPECIES_YAMASK_G},
};

const struct WildPokemonInfo gDiglettCave1_LandMonsNightInfo = { 5, gDiglettCave1_LandMonsNight };

const struct WildPokemon gPewterCity_LandMonsDay[] =
{
	{7, 10, SPECIES_AIPOM},
	{7, 10, SPECIES_WOOPER},
	{7, 10, SPECIES_YANMA},
	{7, 10, SPECIES_SUNKERN},
	{7, 10, SPECIES_MARILL},
	{7, 10, SPECIES_SPOINK},
	{7, 10, SPECIES_SPINDA},
	{7, 10, SPECIES_RALTS},
	{7, 10, SPECIES_WINGULL},
	{7, 10, SPECIES_TAILLOW},
	{7, 10, SPECIES_SWABLU},
	{7, 10, SPECIES_BURMY},
};

const struct WildPokemonInfo gPewterCity_LandMonsDayInfo = { 22, gPewterCity_LandMonsDay };

const struct WildPokemon gPewterCity_LandMonsNight[] =
{
	{7, 10, SPECIES_BLITZLE},
	{7, 10, SPECIES_WOOPER},
	{7, 10, SPECIES_DARUMAKA},
	{7, 10, SPECIES_SWIRLIX},
	{7, 10, SPECIES_SOLOSIS},
	{7, 10, SPECIES_GOTHITA},
	{7, 10, SPECIES_FOONGUS},
	{7, 10, SPECIES_DUCKLETT},
	{7, 10, SPECIES_GOLETT},
	{7, 10, SPECIES_SALANDIT},
	{7, 10, SPECIES_SPRITZEE},
	{7, 10, SPECIES_SWIRLIX},
};

const struct WildPokemonInfo gPewterCity_LandMonsNightInfo = { 22, gPewterCity_LandMonsNight };

const struct WildPokemon gRoute3_LandMonsDay[] =
{
	{14, 17, SPECIES_LITLEO},
	{14, 17, SPECIES_PANCHAM},
	{14, 17, SPECIES_GOOMY},
	{14, 17, SPECIES_INKAY},
	{14, 17, SPECIES_PYUKUMUKU},
	{14, 17, SPECIES_ZORUA},
	{14, 17, SPECIES_TRUBBISH},
	{14, 17, SPECIES_DRIFLOON},
	{14, 17, SPECIES_NINCADA},
	{14, 17, SPECIES_SKITTY},
	{14, 17, SPECIES_MEDITITE},
	{14, 17, SPECIES_PINECO},
};

const struct WildPokemonInfo gRoute3_LandMonsDayInfo = { 22, gRoute3_LandMonsDay };

const struct WildPokemon gRoute3_LandMonsNight[] =
{
	{14, 17, SPECIES_SKIDDO},
	{14, 17, SPECIES_ESPURR},
	{14, 17, SPECIES_NIDORAN_M},
	{14, 17, SPECIES_NIDORAN_F},
	{14, 17, SPECIES_PINCURCHIN},
	{14, 17, SPECIES_ZORUA_H},
	{14, 17, SPECIES_KLINK},
	{14, 17, SPECIES_CHATOT},
	{14, 17, SPECIES_NINCADA},
	{14, 17, SPECIES_SLAKOTH},
	{14, 17, SPECIES_MEDITITE},
	{14, 17, SPECIES_PINECO},
};

const struct WildPokemonInfo gRoute3_LandMonsNightInfo = { 22, gRoute3_LandMonsNight };

const struct WildPokemon gMtMoon1_LandMonsDay[] =
{
	{14, 17, SPECIES_GEODUDE},
	{14, 17, SPECIES_ZUBAT},
	{14, 17, SPECIES_GLIGAR},
	{14, 17, SPECIES_DUNSPARCE},
	{14, 17, SPECIES_NOSEPASS},
	{14, 17, SPECIES_SPHEAL},
	{14, 17, SPECIES_BRONZOR},
	{14, 17, SPECIES_ROGGENROLA},
	{14, 17, SPECIES_ONIX},
	{14, 17, SPECIES_CUBONE},
	{14, 17, SPECIES_CLEFAIRY},
	{14, 17, SPECIES_JIGGLYPUFF},
};

const struct WildPokemonInfo gMtMoon1_LandMonsDayInfo = { 5, gMtMoon1_LandMonsDay };

const struct WildPokemon gMtMoon1_LandMonsNight[] =
{
	{14, 17, SPECIES_GEODUDE_A},
	{14, 17, SPECIES_ZUBAT},
	{14, 17, SPECIES_GLIGAR},
	{14, 17, SPECIES_DUNSPARCE},
	{14, 17, SPECIES_NOSEPASS},
	{14, 17, SPECIES_SPHEAL},
	{14, 17, SPECIES_BRONZOR},
	{14, 17, SPECIES_ROGGENROLA},
	{14, 17, SPECIES_ONIX},
	{14, 17, SPECIES_CUBONE_A},
	{14, 17, SPECIES_CLEFAIRY},
	{14, 17, SPECIES_JIGGLYPUFF},
};

const struct WildPokemonInfo gMtMoon1_LandMonsNightInfo = { 5, gMtMoon1_LandMonsNight };

const struct WildPokemon gMtMoon2_LandMonsDay[] =
{
	{19, 21, SPECIES_ZUBAT},
	{19, 21, SPECIES_PSYDUCK},
	{19, 21, SPECIES_SABLEYE},
	{19, 21, SPECIES_SOLROCK}, 
	{19, 21, SPECIES_TYROGUE},
	{19, 21, SPECIES_BALTOY},
	{19, 21, SPECIES_CLEFFA},
	{19, 21, SPECIES_IGGLYBUFF},
	{19, 21, SPECIES_UNOWN_B},
	{19, 21, SPECIES_TRAPINCH},
	{19, 21, SPECIES_BELDUM},
	{19, 21, SPECIES_LARVITAR},
};

const struct WildPokemonInfo gMtMoon2_LandMonsDayInfo = { 5, gMtMoon2_LandMonsDay };

const struct WildPokemon gMtMoon2_LandMonsNight[] =
{
	{19, 21, SPECIES_ZUBAT},
	{19, 21, SPECIES_PSYDUCK},
	{19, 21, SPECIES_MAWILE},
	{19, 21, SPECIES_LUNATONE},
	{19, 21, SPECIES_TYROGUE},
	{19, 21, SPECIES_BALTOY},
	{19, 21, SPECIES_CLEFFA},
	{19, 21, SPECIES_IGGLYBUFF},
	{19, 21, SPECIES_UNOWN_B},
	{19, 21, SPECIES_TRAPINCH},
	{19, 21, SPECIES_BELDUM},
	{19, 21, SPECIES_LARVITAR},
};

const struct WildPokemonInfo gMtMoon2_LandMonsNightInfo = { 5, gMtMoon2_LandMonsNight };

const struct WildPokemon gMtMoon3_LandMonsDay[] =
{
	{19, 21, SPECIES_ZUBAT},
	{19, 21, SPECIES_TEDDIURSA},
	{19, 21, SPECIES_POLIWAG},
	{19, 21, SPECIES_MISDREAVUS},
	{19, 21, SPECIES_ARON},
	{19, 21, SPECIES_SHELLOS},
	{19, 21, SPECIES_WOOBAT},
	{19, 21, SPECIES_DRILBUR},
	{19, 21, SPECIES_TYNAMO},
	{19, 21, SPECIES_BINACLE},
	{19, 21, SPECIES_ROLYCOLY},
	{19, 21, SPECIES_GIBLE},
};

const struct WildPokemonInfo gMtMoon3_LandMonsDayInfo = { 5, gMtMoon3_LandMonsDay };

const struct WildPokemon gMtMoon3_LandMonsNight[] =
{
	{19, 21, SPECIES_ZUBAT},
	{19, 21, SPECIES_TEDDIURSA},
	{19, 21, SPECIES_TYMPOLE},
	{19, 21, SPECIES_MURKROW},
	{19, 21, SPECIES_ARON},
	{19, 21, SPECIES_SHELLOS_EAST},
	{19, 21, SPECIES_WOOBAT},
	{19, 21, SPECIES_DRILBUR},
	{19, 21, SPECIES_TYNAMO},
	{19, 21, SPECIES_BINACLE},
	{19, 21, SPECIES_ROLYCOLY},
	{19, 21, SPECIES_GIBLE},
};

const struct WildPokemonInfo gMtMoon3_LandMonsNightInfo = { 5, gMtMoon3_LandMonsNight };

const struct WildPokemon gRoute4_LandMonsDay[] =
{
	{20, 21, SPECIES_VULPIX},
	{20, 21, SPECIES_MEOWTH},
	{20, 21, SPECIES_SLUGMA},
	{20, 21, SPECIES_NATU},
	{20, 21, SPECIES_GRIMER},
	{20, 21, SPECIES_VOLTORB},
	{20, 21, SPECIES_MAGNEMITE},
	{20, 21, SPECIES_ELECTRIKE},
	{20, 21, SPECIES_BUNEARY},
	{20, 21, SPECIES_STUNKY},
	{20, 21, SPECIES_SNOVER},
	{20, 21, SPECIES_STANTLER},
};

const struct WildPokemonInfo gRoute4_LandMonsDayInfo = { 22, gRoute4_LandMonsDay };

const struct WildPokemon gRoute4_LandMonsNight[] =
{
	{20, 21, SPECIES_VULPIX_A},
	{20, 21, SPECIES_MEOWTH_G},
	{20, 21, SPECIES_SLUGMA},
	{20, 21, SPECIES_NATU},
	{20, 21, SPECIES_GRIMER_A},
	{20, 21, SPECIES_VOLTORB_H},
	{20, 21, SPECIES_MAGNEMITE},
	{20, 21, SPECIES_ELECTRIKE},
	{20, 21, SPECIES_BUNEARY},
	{20, 21, SPECIES_STUNKY},
	{20, 21, SPECIES_SNOVER},
	{20, 21, SPECIES_STANTLER},
};

const struct WildPokemonInfo gRoute4_LandMonsNightInfo = { 22, gRoute4_LandMonsNight };

const struct WildPokemon gRoute24_LandMonsDay[] =
{
	{22, 24, SPECIES_BUNEARY},
	{22, 24, SPECIES_STUNKY},
	{22, 24, SPECIES_ABRA},
	{22, 24, SPECIES_MEOWTH_A},
	{22, 24, SPECIES_SNEASEL_H},
	{22, 24, SPECIES_SMOOCHUM},
	{22, 24, SPECIES_GIRAFARIG},
	{22, 24, SPECIES_PLUSLE},
	{22, 24, SPECIES_CACNEA},
	{22, 24, SPECIES_SNORUNT},
	{22, 24, SPECIES_MUNNA},
	{22, 24, SPECIES_DEERLING},
};

const struct WildPokemonInfo gRoute24_LandMonsDayInfo = { 22, gRoute24_LandMonsDay };

const struct WildPokemon gRoute24_LandMonsNight[] =
{
	{22, 24, SPECIES_BUNEARY},
	{22, 24, SPECIES_STUNKY},
	{22, 24, SPECIES_ABRA},
	{22, 24, SPECIES_MEOWTH_A},
	{22, 24, SPECIES_SNEASEL},
	{22, 24, SPECIES_MAGBY},
	{22, 24, SPECIES_GIRAFARIG},
	{22, 24, SPECIES_MINUN},
	{22, 24, SPECIES_KECLEON},
	{22, 24, SPECIES_SNORUNT},
	{22, 24, SPECIES_MUNNA},
	{22, 24, SPECIES_DEERLING},
};

const struct WildPokemonInfo gRoute24_LandMonsNightInfo = { 22, gRoute24_LandMonsNight };

const struct WildPokemon gRoute25_LandMonsDay[] =
{
	{22, 24, SPECIES_COTTONEE},
	{22, 24, SPECIES_CRABRAWLER},
	{22, 24, SPECIES_PHANTUMP},
	{22, 24, SPECIES_DEDENNE},
	{22, 24, SPECIES_BERGMITE},
	{22, 24, SPECIES_ORICORIO},
	{22, 24, SPECIES_CUBCHOO},
	{22, 24, SPECIES_KOMALA},
	{22, 24, SPECIES_TOXEL},
	{22, 24, SPECIES_APPLIN},
	{22, 24, SPECIES_MILCERY},
	{22, 24, SPECIES_AUDINO},
};

const struct WildPokemonInfo gRoute25_LandMonsDayInfo = { 22, gRoute25_LandMonsDay };

const struct WildPokemon gRoute25_LandMonsNight[] =
{
	{22, 24, SPECIES_PETILIL},
	{22, 24, SPECIES_CRABRAWLER},
	{22, 24, SPECIES_PHANTUMP},
	{22, 24, SPECIES_DEDENNE},
	{22, 24, SPECIES_BERGMITE},
	{22, 24, SPECIES_ORICORIO_Y},
	{22, 24, SPECIES_CUBCHOO},
	{22, 24, SPECIES_KOMALA},
	{22, 24, SPECIES_TOXEL},
	{22, 24, SPECIES_APPLIN},
	{22, 24, SPECIES_MILCERY},
	{22, 24, SPECIES_AUDINO},
};

const struct WildPokemonInfo gRoute25_LandMonsNightInfo = { 22, gRoute25_LandMonsNight };

const struct WildPokemon gRoute5_LandMonsDay[] =
{
	{26, 28, SPECIES_KRABBY},
	{26, 28, SPECIES_GROWLITHE},
	{26, 28, SPECIES_PONYTA},
	{26, 28, SPECIES_FARFETCHD},
	{26, 28, SPECIES_SLOWPOKE},
	{26, 28, SPECIES_VANILLITE},
	{26, 28, SPECIES_MIENFOO},
	{26, 28, SPECIES_PACHIRISU},
	{26, 28, SPECIES_ZANGOOSE},
	{26, 28, SPECIES_HOUNDOUR},
	{26, 28, SPECIES_CHIMECHO},
	{26, 28, SPECIES_HAWLUCHA},
};

const struct WildPokemonInfo gRoute5_LandMonsDayInfo = { 22, gRoute5_LandMonsDay };

const struct WildPokemon gRoute5_LandMonsNight[] =
{
	{26, 28, SPECIES_KRABBY},
	{26, 28, SPECIES_GROWLITHE_H},
	{26, 28, SPECIES_PONYTA_G},
	{26, 28, SPECIES_FARFETCHD_G},
	{26, 28, SPECIES_SLOWPOKE_G},
	{26, 28, SPECIES_VANILLITE},
	{26, 28, SPECIES_MIENFOO},
	{26, 28, SPECIES_PACHIRISU},
	{26, 28, SPECIES_SEVIPER},
	{26, 28, SPECIES_HOUNDOUR},
	{26, 28, SPECIES_CHIMECHO},
	{26, 28, SPECIES_HAWLUCHA},
};

const struct WildPokemonInfo gRoute5_LandMonsNightInfo = { 22, gRoute5_LandMonsNight };

const struct WildPokemon gRoute6_LandMonsDay[] =
{
	{26, 28, SPECIES_STUFFUL},
	{26, 28, SPECIES_JOLTIK},
	{26, 28, SPECIES_STARAVIA},
	{26, 28, SPECIES_SCYTHER},
	{26, 28, SPECIES_SMEARGLE},
	{26, 28, SPECIES_EXEGGCUTE},
	{26, 28, SPECIES_SUDOWOODO},
	{26, 28, SPECIES_HATENNA},
	{26, 28, SPECIES_TOGETIC},
	{26, 28, SPECIES_SKIPLOOM},
	{26, 28, SPECIES_MUNCHLAX},
	{26, 28, SPECIES_CHATOT},
};

const struct WildPokemonInfo gRoute6_LandMonsDayInfo = { 22, gRoute6_LandMonsDay };

const struct WildPokemon gRoute6_LandMonsNight[] =
{
	{26, 28, SPECIES_STUFFUL},
	{26, 28, SPECIES_JOLTIK},
	{26, 28, SPECIES_STARAVIA},
	{26, 28, SPECIES_SCYTHER},
	{26, 28, SPECIES_SMEARGLE},
	{26, 28, SPECIES_EXEGGCUTE},
	{26, 28, SPECIES_SUDOWOODO},
	{26, 28, SPECIES_MORGREM},
	{26, 28, SPECIES_TOGETIC},
	{26, 28, SPECIES_SKIPLOOM},
	{26, 28, SPECIES_MUNCHLAX},
	{26, 28, SPECIES_CHATOT},
};

const struct WildPokemonInfo gRoute6_LandMonsNightInfo = { 22, gRoute6_LandMonsNight };

const struct WildPokemon gRoute11_LandMonsDay[] =
{
	{29, 32, SPECIES_SKRELP},
	{29, 32, SPECIES_FURFROU},
	{29, 32, SPECIES_CUFANT},
	{29, 32, SPECIES_CLOBBOPUS},
	{29, 32, SPECIES_MORPEKO},
	{29, 32, SPECIES_VULLABY},
	{29, 32, SPECIES_ELGYEM},
	{29, 32, SPECIES_PAWNIARD},
	{29, 32, SPECIES_DODUO},
	{29, 32, SPECIES_DROWZEE},
	{29, 32, SPECIES_EXEGGCUTE},
	{29, 32, SPECIES_TANGELA},
};

const struct WildPokemonInfo gRoute11_LandMonsDayInfo = { 22, gRoute11_LandMonsDay };

const struct WildPokemon gRoute11_LandMonsNight[] =
{
	{29, 32, SPECIES_SKRELP},
	{29, 32, SPECIES_FURFROU},
	{29, 32, SPECIES_CUFANT},
	{29, 32, SPECIES_CLOBBOPUS},
	{29, 32, SPECIES_MORPEKO},
	{29, 32, SPECIES_RUFFLET},
	{29, 32, SPECIES_ELGYEM},
	{29, 32, SPECIES_PAWNIARD},
	{29, 32, SPECIES_DODUO},
	{29, 32, SPECIES_DROWZEE},
	{29, 32, SPECIES_EXEGGCUTE},
	{29, 32, SPECIES_TANGELA},
};

const struct WildPokemonInfo gRoute11_LandMonsNightInfo = { 22, gRoute11_LandMonsNight };

const struct WildPokemon gDiglettCave2_LandMonsDay[] =
{
	{29, 32, SPECIES_DIGLETT},
	{29, 32, SPECIES_PHANPY},
	{29, 32, SPECIES_CUBONE},
	{29, 32, SPECIES_SANDSHREW},
	{29, 32, SPECIES_DRILBUR},
	{29, 32, SPECIES_SANDILE},
	{29, 32, SPECIES_TRAPINCH},
	{29, 32, SPECIES_HIPPOPOTAS},
	{29, 32, SPECIES_SILICOBRA},
	{29, 32, SPECIES_MUDBRAY},
	{29, 32, SPECIES_RHYHORN},
	{29, 32, SPECIES_DUGTRIO},
};

const struct WildPokemonInfo gDiglettCave2_LandMonsDayInfo = { 5, gDiglettCave2_LandMonsDay };

const struct WildPokemon gDiglettCave2_LandMonsNight[] =
{
	{29, 32, SPECIES_DIGLETT_A},
	{29, 32, SPECIES_PHANPY},
	{29, 32, SPECIES_CUBONE_A},
	{29, 32, SPECIES_SANDSHREW_A},
	{29, 32, SPECIES_DRILBUR},
	{29, 32, SPECIES_SANDILE},
	{29, 32, SPECIES_TRAPINCH},
	{29, 32, SPECIES_HIPPOPOTAS},
	{29, 32, SPECIES_SILICOBRA},
	{29, 32, SPECIES_MUDBRAY},
	{29, 32, SPECIES_RHYHORN},
	{29, 32, SPECIES_DUGTRIO},
};

const struct WildPokemonInfo gDiglettCave2_LandMonsNightInfo = { 5, gDiglettCave2_LandMonsNight };

const struct WildPokemon gRoute9_LandMonsDay[] =
{
	{32, 35, SPECIES_HELIOPTILE},
	{32, 35, SPECIES_STANTLER},
	{32, 35, SPECIES_PLUSLE},
	{32, 35, SPECIES_KLINK},
	{32, 35, SPECIES_DARUMAKA},
	{32, 35, SPECIES_RIOLU},
	{32, 35, SPECIES_MR_MIME},
	{32, 35, SPECIES_VIGOROTH},
	{32, 35, SPECIES_MUNCHLAX},
	{32, 35, SPECIES_MAGMAR},
	{32, 35, SPECIES_LOMBRE},
	{32, 35, SPECIES_GRAVELER_A},
};

const struct WildPokemonInfo gRoute9_LandMonsDayInfo = { 22, gRoute9_LandMonsDay };

const struct WildPokemon gRoute9_LandMonsNight[] =
{
	{32, 35, SPECIES_HELIOPTILE},
	{32, 35, SPECIES_STANTLER},
	{32, 35, SPECIES_MINUN},
	{32, 35, SPECIES_KLINK},
	{32, 35, SPECIES_DARUMAKA_G},
	{32, 35, SPECIES_RIOLU},
	{32, 35, SPECIES_MR_MIME_G},
	{32, 35, SPECIES_VIGOROTH},
	{32, 35, SPECIES_MUNCHLAX},
	{32, 35, SPECIES_ELECTABUZZ},
	{32, 35, SPECIES_NUZLEAF},
	{32, 35, SPECIES_GRAVELER_A},
};

const struct WildPokemonInfo gRoute9_LandMonsNightInfo = { 22, gRoute9_LandMonsNight };

const struct WildPokemon gRoute10_LandMonsDay[] =
{
	{32, 35, SPECIES_LICKITUNG},
	{32, 35, SPECIES_STANTLER},
	{32, 35, SPECIES_SMEARGLE},
	{32, 35, SPECIES_PUMPKABOO},
	{32, 35, SPECIES_PUMPKABOO},
	{32, 35, SPECIES_KLEFKI},
	{32, 35, SPECIES_FLETCHINDER},
	{32, 35, SPECIES_PYROAR},
	{32, 35, SPECIES_APPLIN},
	{32, 35, SPECIES_ELDEGOSS},
	{32, 35, SPECIES_MARACTUS},
	{32, 35, SPECIES_DRATINI},
};

const struct WildPokemonInfo gRoute10_LandMonsDayInfo = { 22, gRoute10_LandMonsDay };

const struct WildPokemon gRoute10_LandMonsNight[] =
{
	{32, 35, SPECIES_KOFFING},
	{32, 35, SPECIES_STANTLER},
	{32, 35, SPECIES_SMEARGLE},
	{32, 35, SPECIES_PUMPKABOO},
	{32, 35, SPECIES_PUMPKABOO},
	{32, 35, SPECIES_KLEFKI},
	{32, 35, SPECIES_FLETCHINDER},
	{32, 35, SPECIES_PYROAR},
	{32, 35, SPECIES_APPLIN},
	{32, 35, SPECIES_ELDEGOSS},
	{32, 35, SPECIES_MARACTUS},
	{32, 35, SPECIES_DRATINI},
};

const struct WildPokemonInfo gRoute10_LandMonsNightInfo = { 22, gRoute10_LandMonsNight };

const struct WildPokemon gRockTunnel1_LandMonsDay[] =
{
	{32, 35, SPECIES_GRAVELER},
	{32, 35, SPECIES_DIGGERSBY},
	{32, 35, SPECIES_CUBONE},
	{32, 35, SPECIES_BINACLE},
	{32, 35, SPECIES_MUDSDALE},
	{32, 35, SPECIES_DRILBUR},
	{32, 35, SPECIES_TRAPINCH},
	{32, 35, SPECIES_MINIOR_SHIELD},
	{32, 35, SPECIES_ROCKRUFF},
	{32, 35, SPECIES_SILICOBRA},
	{32, 35, SPECIES_STONJOURNER},
	{32, 35, SPECIES_LYCANROC},
};

const struct WildPokemonInfo gRockTunnel1_LandMonsDayInfo = { 5, gRockTunnel1_LandMonsDay };

const struct WildPokemon gRockTunnel1_LandMonsNight[] =
{
	{32, 35, SPECIES_GRAVELER_A},
	{32, 35, SPECIES_DIGGERSBY},
	{32, 35, SPECIES_CUBONE_A},
	{32, 35, SPECIES_BINACLE},
	{32, 35, SPECIES_MUDSDALE},
	{32, 35, SPECIES_DRILBUR},
	{32, 35, SPECIES_TRAPINCH},
	{32, 35, SPECIES_MINIOR_SHIELD},
	{32, 35, SPECIES_ROCKRUFF},
	{32, 35, SPECIES_SILICOBRA},
	{32, 35, SPECIES_STONJOURNER},
	{32, 35, SPECIES_LYCANROC_N},
};

const struct WildPokemonInfo gRockTunnel1_LandMonsNightInfo = { 5, gRockTunnel1_LandMonsNight };

const struct WildPokemon gRockTunnel2_LandMonsDay[] =
{
	{33, 35, SPECIES_SANDSLASH},
	{33, 35, SPECIES_DIGLETT},
	{33, 35, SPECIES_PILOSWINE},
	{33, 35, SPECIES_SNEASEL_H},
	{33, 35, SPECIES_GLIGAR},
	{33, 35, SPECIES_ONIX},
	{33, 35, SPECIES_NOSEPASS},
	{33, 35, SPECIES_LAIRON},
	{33, 35, SPECIES_SOLROCK},
	{33, 35, SPECIES_BALTOY},
	{33, 35, SPECIES_CARBINK},
	{33, 35, SPECIES_AXEW},
};

const struct WildPokemonInfo gRockTunnel2_LandMonsDayInfo = { 5, gRockTunnel2_LandMonsDay };

const struct WildPokemon gRockTunnel2_LandMonsNight[] =
{
	{33, 35, SPECIES_SANDSLASH_A},
	{33, 35, SPECIES_DIGLETT_A},
	{33, 35, SPECIES_PILOSWINE},
	{33, 35, SPECIES_SNEASEL},
	{33, 35, SPECIES_GLIGAR},
	{33, 35, SPECIES_ONIX},
	{33, 35, SPECIES_NOSEPASS},
	{33, 35, SPECIES_LAIRON},
	{33, 35, SPECIES_LUNATONE},
	{33, 35, SPECIES_BALTOY},
	{33, 35, SPECIES_CARBINK},
	{33, 35, SPECIES_AXEW},
};

const struct WildPokemonInfo gRockTunnel2_LandMonsNightInfo = { 5, gRockTunnel2_LandMonsNight };

const struct WildPokemon gRoute8_LandMonsDay[] =
{
	{40, 43, SPECIES_SCYTHER},
	{40, 43, SPECIES_PONYTA},
	{40, 43, SPECIES_SMEARGLE},
	{40, 43, SPECIES_RAPIDASH},
	{40, 43, SPECIES_MISDREAVUS},
	{40, 43, SPECIES_TROPIUS},
	{40, 43, SPECIES_LOPUNNY},
	{40, 43, SPECIES_DRAPION},
	{40, 43, SPECIES_APPLETUN},
	{40, 43, SPECIES_RIBOMBEE},
	{40, 43, SPECIES_CARBINK},
	{40, 43, SPECIES_AXEW},
};

const struct WildPokemonInfo gRoute8_LandMonsDayInfo = { 22, gRoute8_LandMonsDay };

const struct WildPokemon gRoute8_LandMonsNight[] =
{
	{40, 43, SPECIES_SCYTHER},
	{40, 43, SPECIES_PONYTA_G},
	{40, 43, SPECIES_SMEARGLE},
	{40, 43, SPECIES_RAPIDASH_G},
	{40, 43, SPECIES_MURKROW},
	{40, 43, SPECIES_TROPIUS},
	{40, 43, SPECIES_LOPUNNY},
	{40, 43, SPECIES_DRAPION},
	{40, 43, SPECIES_FLAPPLE},
	{40, 43, SPECIES_RIBOMBEE},
	{40, 43, SPECIES_CARBINK},
	{40, 43, SPECIES_AXEW},
};

const struct WildPokemonInfo gRoute8_LandMonsNightInfo = { 22, gRoute8_LandMonsNight };

const struct WildPokemon gRoute7_LandMonsDay[] =
{
	{40, 43, SPECIES_STUNFISK},
	{40, 43, SPECIES_GOURGEIST},
	{40, 43, SPECIES_MINCCINO},
	{40, 43, SPECIES_PINSIR},
	{40, 43, SPECIES_SHUCKLE},
	{40, 43, SPECIES_DELIBIRD},
	{40, 43, SPECIES_SKARMORY},
	{40, 43, SPECIES_TORKOAL},
	{40, 43, SPECIES_DURALUDON},
	{40, 43, SPECIES_CARNIVINE},
	{40, 43, SPECIES_SAWK},
	{40, 43, SPECIES_EMOLGA},
};

const struct WildPokemonInfo gRoute7_LandMonsDayInfo = { 22, gRoute7_LandMonsDay };

const struct WildPokemon gRoute7_LandMonsNight[] =
{
	{40, 43, SPECIES_STUNFISK_G},
	{40, 43, SPECIES_TREVENANT},
	{40, 43, SPECIES_MINCCINO},
	{40, 43, SPECIES_HERACROSS},
	{40, 43, SPECIES_SHUCKLE},
	{40, 43, SPECIES_DELIBIRD},
	{40, 43, SPECIES_SKARMORY},
	{40, 43, SPECIES_TORKOAL},
	{40, 43, SPECIES_DURALUDON},
	{40, 43, SPECIES_CARNIVINE},
	{40, 43, SPECIES_THROH},
	{40, 43, SPECIES_EMOLGA},
};

const struct WildPokemonInfo gRoute7_LandMonsNightInfo = { 22, gRoute7_LandMonsNight };

const struct WildPokemon gPokemonTower1_LandMonsDay[] =
{
	{47, 49, SPECIES_HAUNTER},
	{47, 49, SPECIES_HAUNTER},
	{47, 49, SPECIES_CORSOLA_G},
	{47, 49, SPECIES_CORSOLA_G},
	{47, 49, SPECIES_CURSOLA},
	{47, 49, SPECIES_BANETTE},
	{47, 49, SPECIES_DUSCLOPS},
	{47, 49, SPECIES_MISMAGIUS},
	{47, 49, SPECIES_SINISTEA},
	{47, 49, SPECIES_YAMASK},
	{47, 49, SPECIES_DRIFBLIM},
	{47, 49, SPECIES_COFAGRIGUS},
};

const struct WildPokemonInfo gPokemonTower1_LandMonsDayInfo = { 5, gPokemonTower1_LandMonsDay };

const struct WildPokemon gPokemonTower1_LandMonsNight[] =
{
	{47, 49, SPECIES_HAUNTER},
	{47, 49, SPECIES_HAUNTER},
	{47, 49, SPECIES_CORSOLA_G},
	{47, 49, SPECIES_CORSOLA_G},
	{47, 49, SPECIES_CURSOLA},
	{47, 49, SPECIES_BANETTE},
	{47, 49, SPECIES_DUSCLOPS},
	{47, 49, SPECIES_MISMAGIUS},
	{47, 49, SPECIES_SINISTEA},
	{47, 49, SPECIES_YAMASK_G},
	{47, 49, SPECIES_DRIFBLIM},
	{47, 49, SPECIES_RUNERIGUS},
};

const struct WildPokemonInfo gPokemonTower1_LandMonsNightInfo = { 5, gPokemonTower1_LandMonsNight };

const struct WildPokemon gPokemonTower2_LandMonsDay[] =
{
	{47, 49, SPECIES_HAUNTER},
	{47, 49, SPECIES_HAUNTER},
	{47, 49, SPECIES_YAMASK},
	{47, 49, SPECIES_DREEPY},
	{47, 49, SPECIES_CURSOLA},
	{47, 49, SPECIES_HONEDGE},
	{47, 49, SPECIES_DOUBLADE},
	{47, 49, SPECIES_FRILLISH},
	{47, 49, SPECIES_FRILLISH},
	{47, 49, SPECIES_SABLEYE},
	{47, 49, SPECIES_SABLEYE},
	{47, 49, SPECIES_ROTOM},
};

const struct WildPokemonInfo gPokemonTower2_LandMonsDayInfo = { 5, gPokemonTower2_LandMonsDay };

const struct WildPokemon gPokemonTower2_LandMonsNight[] =
{
	{47, 49, SPECIES_HAUNTER},
	{47, 49, SPECIES_HAUNTER},
	{47, 49, SPECIES_YAMASK_G},
	{47, 49, SPECIES_DREEPY},
	{47, 49, SPECIES_CURSOLA},
	{47, 49, SPECIES_HONEDGE},
	{47, 49, SPECIES_DOUBLADE},
	{47, 49, SPECIES_FRILLISH},
	{47, 49, SPECIES_FRILLISH},
	{47, 49, SPECIES_SABLEYE},
	{47, 49, SPECIES_SABLEYE},
	{47, 49, SPECIES_ROTOM},
};

const struct WildPokemonInfo gPokemonTower2_LandMonsNightInfo = { 5, gPokemonTower2_LandMonsNight };

const struct WildPokemon gPokemonTower3_LandMonsDay[] =
{
	{47, 49, SPECIES_HAUNTER},
	{47, 49, SPECIES_HAUNTER},
	{47, 49, SPECIES_DUSCLOPS},
	{47, 49, SPECIES_HONEDGE},
	{47, 49, SPECIES_SHEDINJA},
	{47, 49, SPECIES_GOLURK},
	{47, 49, SPECIES_FROSLASS},
	{47, 49, SPECIES_PUMPKABOO},
	{47, 49, SPECIES_LAMPENT},
	{47, 49, SPECIES_SANDYGAST},
	{47, 49, SPECIES_PALOSSAND},
	{47, 49, SPECIES_MIMIKYU},
};

const struct WildPokemonInfo gPokemonTower3_LandMonsDayInfo = { 5, gPokemonTower3_LandMonsDay };

const struct WildPokemon gPokemonTower3_LandMonsNight[] =
{
	{47, 49, SPECIES_HAUNTER},
	{47, 49, SPECIES_HAUNTER},
	{47, 49, SPECIES_DUSCLOPS},
	{47, 49, SPECIES_HONEDGE},
	{47, 49, SPECIES_SHEDINJA},
	{47, 49, SPECIES_GOLURK},
	{47, 49, SPECIES_FROSLASS},
	{47, 49, SPECIES_PUMPKABOO_XL},
	{47, 49, SPECIES_LAMPENT},
	{47, 49, SPECIES_SANDYGAST},
	{47, 49, SPECIES_PALOSSAND},
	{47, 49, SPECIES_DHELMISE},
};

const struct WildPokemonInfo gPokemonTower3_LandMonsNightInfo = { 5, gPokemonTower3_LandMonsNight };

const struct WildPokemon gPokemonTower4_LandMonsDay[] =
{
	{47, 49, SPECIES_HAUNTER},
	{47, 49, SPECIES_DUSCLOPS},
	{47, 49, SPECIES_PHANTUMP},
	{47, 49, SPECIES_LITWICK},
	{47, 49, SPECIES_LAMPENT},
	{47, 49, SPECIES_TREVENANT},
	{47, 49, SPECIES_FROSLASS},
	{47, 49, SPECIES_CHANDELURE},
	{47, 49, SPECIES_SPIRITOMB},
	{47, 49, SPECIES_ROTOM},
	{47, 49, SPECIES_YAMASK},
	{47, 49, SPECIES_DUSKNOIR},
};

const struct WildPokemonInfo gPokemonTower4_LandMonsDayInfo = { 5, gPokemonTower4_LandMonsDay };

const struct WildPokemon gPokemonTower4_LandMonsNight[] =
{
	{47, 49, SPECIES_HAUNTER},
	{47, 49, SPECIES_DUSCLOPS},
	{47, 49, SPECIES_PHANTUMP},
	{47, 49, SPECIES_LITWICK},
	{47, 49, SPECIES_LAMPENT},
	{47, 49, SPECIES_TREVENANT},
	{47, 49, SPECIES_FROSLASS},
	{47, 49, SPECIES_CHANDELURE},
	{47, 49, SPECIES_SPIRITOMB},
	{47, 49, SPECIES_ROTOM},
	{47, 49, SPECIES_YAMASK_G},
	{47, 49, SPECIES_DUSKNOIR},
};

const struct WildPokemonInfo gPokemonTower4_LandMonsNightInfo = { 5, gPokemonTower4_LandMonsNight };

const struct WildPokemon gRoute17_LandMonsDay[] =
{
	{50, 53, SPECIES_EEVEE},
	{50, 53, SPECIES_RUFFLET},
	{50, 53, SPECIES_URSARING},
	{50, 53, SPECIES_TAUROS},
	{50, 53, SPECIES_MILTANK},
	{50, 53, SPECIES_CORSOLA},
	{50, 53, SPECIES_CORSOLA_G},
	{50, 53, SPECIES_MOTHIM},
	{50, 53, SPECIES_PURUGLY},
	{50, 53, SPECIES_GOTHORITA},
	{50, 53, SPECIES_MAGMAR},
	{50, 53, SPECIES_ELECTABUZZ},
};

const struct WildPokemonInfo gRoute17_LandMonsDayInfo = { 22, gRoute17_LandMonsDay };

const struct WildPokemon gRoute17_LandMonsNight[] =
{
	{50, 53, SPECIES_EEVEE},
	{50, 53, SPECIES_VULLABY},
	{50, 53, SPECIES_URSARING},
	{50, 53, SPECIES_TAUROS},
	{50, 53, SPECIES_MILTANK},
	{50, 53, SPECIES_CORSOLA},
	{50, 53, SPECIES_CORSOLA_G},
	{50, 53, SPECIES_MOTHIM},
	{50, 53, SPECIES_PURUGLY},
	{50, 53, SPECIES_DUOSION},
	{50, 53, SPECIES_MAGMAR},
	{50, 53, SPECIES_ELECTABUZZ},
};

const struct WildPokemonInfo gRoute17_LandMonsNightInfo = { 22, gRoute17_LandMonsNight };

const struct WildPokemon gRoute16_LandMonsDay[] =
{
	{50, 53, SPECIES_RUFFLET},
	{50, 53, SPECIES_EISCUE},
	{50, 53, SPECIES_URSARING},
	{50, 53, SPECIES_ABOMASNOW},
	{50, 53, SPECIES_GOLISOPOD},
	{50, 53, SPECIES_MUNCHLAX},
	{50, 53, SPECIES_CORSOLA_G},
	{50, 53, SPECIES_MAGMORTAR},
	{50, 53, SPECIES_SPRITZEE},
	{50, 53, SPECIES_SLURPUFF},
	{50, 53, SPECIES_SLOWKING},
	{50, 53, SPECIES_SLOWBRO},
};

const struct WildPokemonInfo gRoute16_LandMonsDayInfo = { 22, gRoute16_LandMonsDay };

const struct WildPokemon gRoute16_LandMonsNight[] =
{
	{50, 53, SPECIES_RUFFLET},
	{50, 53, SPECIES_STONJOURNER},
	{50, 53, SPECIES_URSARING},
	{50, 53, SPECIES_ABOMASNOW},
	{50, 53, SPECIES_GOLISOPOD},
	{50, 53, SPECIES_MUNCHLAX},
	{50, 53, SPECIES_CORSOLA_G},
	{50, 53, SPECIES_ELECTIVIRE},
	{50, 53, SPECIES_SPRITZEE},
	{50, 53, SPECIES_SLURPUFF},
	{50, 53, SPECIES_SLOWKING_G},
	{50, 53, SPECIES_SLOWBRO_G},
};

const struct WildPokemonInfo gRoute16_LandMonsNightInfo = { 22, gRoute16_LandMonsNight };

const struct WildPokemon gRoute18_LandMonsDay[] =
{
	{50, 53, SPECIES_TRUMBEAK},
	{50, 53, SPECIES_ORICORIO},
	{50, 53, SPECIES_TALONFLAME},
	{50, 53, SPECIES_MINIOR_SHIELD},
	{50, 53, SPECIES_VIVILLON},
	{50, 53, SPECIES_VIVILLON},
	{50, 53, SPECIES_SWANNA},
	{50, 53, SPECIES_SIGILYPH},
	{50, 53, SPECIES_BRAVIARY},
	{50, 53, SPECIES_CHATOT},
	{50, 53, SPECIES_SWELLOW},
	{50, 53, SPECIES_YANMA},
};

const struct WildPokemonInfo gRoute18_LandMonsDayInfo = { 22, gRoute18_LandMonsDay };

const struct WildPokemon gRoute18_LandMonsNight[] =
{
	{50, 53, SPECIES_TRUMBEAK},
	{50, 53, SPECIES_ORICORIO},
	{50, 53, SPECIES_TALONFLAME},
	{50, 53, SPECIES_MINIOR_SHIELD},
	{50, 53, SPECIES_VIVILLON},
	{50, 53, SPECIES_VIVILLON},
	{50, 53, SPECIES_SWANNA},
	{50, 53, SPECIES_SIGILYPH},
	{50, 53, SPECIES_BRAVIARY_H},
	{50, 53, SPECIES_CHATOT},
	{50, 53, SPECIES_SWELLOW},
	{50, 53, SPECIES_MURKROW},
};

const struct WildPokemonInfo gRoute18_LandMonsNightInfo = { 22, gRoute18_LandMonsNight };


const struct WildPokemon gRoute12_LandMonsDay[] =
{
	{50, 53, SPECIES_COMFEY},
	{50, 53, SPECIES_RATICATE},
	{50, 53, SPECIES_MINIOR_SHIELD},
	{50, 53, SPECIES_MINIOR_SHIELD},
	{50, 53, SPECIES_PERRSERKER},
	{50, 53, SPECIES_SWANNA},
	{50, 53, SPECIES_FALINKS},
	{50, 53, SPECIES_LEAVANNY},
	{50, 53, SPECIES_PACHIRISU},
	{50, 53, SPECIES_ORANGURU},
	{50, 53, SPECIES_CRAWDAUNT},
	{50, 53, SPECIES_AVALUGG},
};

const struct WildPokemonInfo gRoute12_LandMonsDayInfo = { 22, gRoute12_LandMonsDay };

const struct WildPokemon gRoute12_LandMonsNight[] =
{
	{50, 53, SPECIES_COMFEY},
	{50, 53, SPECIES_RATICATE_A},
	{50, 53, SPECIES_MINIOR_SHIELD},
	{50, 53, SPECIES_MINIOR_SHIELD},
	{50, 53, SPECIES_PERSIAN},
	{50, 53, SPECIES_SWANNA},
	{50, 53, SPECIES_FALINKS},
	{50, 53, SPECIES_LEAVANNY},
	{50, 53, SPECIES_PACHIRISU},
	{50, 53, SPECIES_PASSIMIAN},
	{50, 53, SPECIES_CRAWDAUNT},
	{50, 53, SPECIES_AVALUGG_H},
};

const struct WildPokemonInfo gRoute12_LandMonsNightInfo = { 22, gRoute12_LandMonsNight };

const struct WildPokemon gRoute14_LandMonsDay[] =
{
	{55, 57, SPECIES_VENOMOTH},
	{55, 57, SPECIES_DRAMPA},
	{55, 57, SPECIES_TURTONATOR},
	{55, 57, SPECIES_EISCUE},
	{55, 57, SPECIES_INDEEDEE},
	{55, 57, SPECIES_BOLDORE},
	{55, 57, SPECIES_ZEBSTRIKA},
	{55, 57, SPECIES_KIRLIA},
	{55, 57, SPECIES_BASCULIN_BLUE},
	{55, 57, SPECIES_JANGMO_O},
	{55, 57, SPECIES_TOGEDEMARU},
	{55, 57, SPECIES_BRUXISH},
};

const struct WildPokemonInfo gRoute14_LandMonsDayInfo = { 22, gRoute14_LandMonsDay };

const struct WildPokemon gRoute14_LandMonsNight[] =
{
	{55, 57, SPECIES_VENOMOTH},
	{55, 57, SPECIES_DRAMPA},
	{55, 57, SPECIES_TURTONATOR},
	{55, 57, SPECIES_STONJOURNER},
	{55, 57, SPECIES_INDEEDEE_FEMALE},
	{55, 57, SPECIES_BOLDORE},
	{55, 57, SPECIES_ZEBSTRIKA},
	{55, 57, SPECIES_KIRLIA},
	{55, 57, SPECIES_BASCULIN_H},
	{55, 57, SPECIES_JANGMO_O},
	{55, 57, SPECIES_MIMIKYU},
	{55, 57, SPECIES_DHELMISE},
};

const struct WildPokemonInfo gRoute14_LandMonsNightInfo = { 22, gRoute14_LandMonsNight };

const struct WildPokemon gSafariZone1_LandMonsDay[] =
{
	{60, 60, SPECIES_ONIX},
	{60, 60, SPECIES_EXEGGCUTE},
	{60, 60, SPECIES_EXEGGUTOR},
	{60, 60, SPECIES_WEEZING},
	{60, 60, SPECIES_INDEEDEE},
	{60, 60, SPECIES_SCYTHER},
	{60, 60, SPECIES_HERACROSS},
	{60, 60, SPECIES_SHUCKLE},
	{60, 60, SPECIES_SHIFTRY},
	{60, 60, SPECIES_SCIZOR},
	{60, 60, SPECIES_SEVIPER},
	{60, 60, SPECIES_CHIMECHO},
};

const struct WildPokemonInfo gSafariZone1_LandMonsDayInfo = { 22, gSafariZone1_LandMonsDay };

const struct WildPokemon gSafariZone1_LandMonsNight[] =
{
	{60, 60, SPECIES_ONIX},
	{60, 60, SPECIES_EXEGGCUTE},
	{60, 60, SPECIES_EXEGGUTOR_A},
	{60, 60, SPECIES_WEEZING_G},
	{60, 60, SPECIES_INDEEDEE},
	{60, 60, SPECIES_SCYTHER},
	{60, 60, SPECIES_PINSIR},
	{60, 60, SPECIES_SHUCKLE},
	{60, 60, SPECIES_LUDICOLO},
	{60, 60, SPECIES_KLEAVOR},
	{60, 60, SPECIES_ZANGOOSE},
	{60, 60, SPECIES_CHIMECHO},
};

const struct WildPokemonInfo gSafariZone1_LandMonsNightInfo = { 22, gSafariZone1_LandMonsNight };

const struct WildPokemon gSafariZone2_LandMonsDay[] =
{
	{60, 60, SPECIES_MR_MIME},
	{60, 60, SPECIES_CARBINK},
	{60, 60, SPECIES_NOIBAT},
	{60, 60, SPECIES_KANGASKHAN},
	{60, 60, SPECIES_BOUFFALANT},
	{60, 60, SPECIES_STUNFISK},
	{60, 60, SPECIES_NOIVERN},
	{60, 60, SPECIES_CRYOGONAL},
	{60, 60, SPECIES_GARBODOR},
	{60, 60, SPECIES_HEATMOR},
	{60, 60, SPECIES_DRAPION},
	{60, 60, SPECIES_ROSERADE},
};

const struct WildPokemonInfo gSafariZone2_LandMonsDayInfo = { 22, gSafariZone2_LandMonsDay };

const struct WildPokemon gSafariZone2_LandMonsNight[] =
{
	{60, 60, SPECIES_MR_MIME_G},
	{60, 60, SPECIES_CARBINK},
	{60, 60, SPECIES_NOIBAT},
	{60, 60, SPECIES_KANGASKHAN},
	{60, 60, SPECIES_DRUDDIGON},
	{60, 60, SPECIES_STUNFISK_G},
	{60, 60, SPECIES_NOIVERN},
	{60, 60, SPECIES_CRYOGONAL},
	{60, 60, SPECIES_GARBODOR},
	{60, 60, SPECIES_DURANT},
	{60, 60, SPECIES_MUK_A},
	{60, 60, SPECIES_ROSERADE},
};

const struct WildPokemonInfo gSafariZone2_LandMonsNightInfo = { 22, gSafariZone2_LandMonsNight };

const struct WildPokemon gSafariZone3_LandMonsDay[] =
{
	{60, 60, SPECIES_CHANSEY},
	{60, 60, SPECIES_SAWK},
	{60, 60, SPECIES_KANGASKHAN},
	{60, 60, SPECIES_ZOROARK},
	{60, 60, SPECIES_LOPUNNY},
	{60, 60, SPECIES_PURUGLY},
	{60, 60, SPECIES_TROPIUS},
	{60, 60, SPECIES_ABSOL},
	{60, 60, SPECIES_GLALIE},
	{60, 60, SPECIES_GARDEVOIR},
	{60, 60, SPECIES_FORRETRESS},
	{60, 60, SPECIES_WEAVILE},
};

const struct WildPokemonInfo gSafariZone3_LandMonsDayInfo = { 22, gSafariZone3_LandMonsDay };

const struct WildPokemon gSafariZone3_LandMonsNight[] =
{
	{60, 60, SPECIES_CHANSEY},
	{60, 60, SPECIES_THROH},
	{60, 60, SPECIES_KANGASKHAN},
	{60, 60, SPECIES_ZOROARK_H},
	{60, 60, SPECIES_LOPUNNY},
	{60, 60, SPECIES_AMBIPOM},
	{60, 60, SPECIES_TROPIUS},
	{60, 60, SPECIES_ABSOL},
	{60, 60, SPECIES_FROSLASS},
	{60, 60, SPECIES_GALLADE},
	{60, 60, SPECIES_FORRETRESS},
	{60, 60, SPECIES_WEAVILE},
};

const struct WildPokemonInfo gSafariZone3_LandMonsNightInfo = { 22, gSafariZone3_LandMonsNight };

const struct WildPokemon gSafariZone4_LandMonsDay[] =
{
	{60, 60, SPECIES_STANTLER},
	{60, 60, SPECIES_RHYDON},
	{60, 60, SPECIES_QUAGSIRE},
	{60, 60, SPECIES_XATU},
	{60, 60, SPECIES_RAICHU},
	{60, 60, SPECIES_PHANPY},
	{60, 60, SPECIES_CARNIVINE},
	{60, 60, SPECIES_TOXICROAK},
	{60, 60, SPECIES_KECLEON},
	{60, 60, SPECIES_WHISCASH},
	{60, 60, SPECIES_VENOMOTH},
	{60, 60, SPECIES_CHANSEY},
};

const struct WildPokemonInfo gSafariZone4_LandMonsDayInfo = { 22, gSafariZone4_LandMonsDay };

const struct WildPokemon gSafariZone4_LandMonsNight[] =
{
	{60, 60, SPECIES_STANTLER},
	{60, 60, SPECIES_RHYDON},
	{60, 60, SPECIES_QUAGSIRE},
	{60, 60, SPECIES_XATU},
	{60, 60, SPECIES_RAICHU_A},
	{60, 60, SPECIES_PHANPY},
	{60, 60, SPECIES_CARNIVINE},
	{60, 60, SPECIES_TOXICROAK},
	{60, 60, SPECIES_KECLEON},
	{60, 60, SPECIES_WHISCASH},
	{60, 60, SPECIES_PARASECT},
	{60, 60, SPECIES_CHANSEY},
};

const struct WildPokemonInfo gSafariZone4_LandMonsNightInfo = { 22, gSafariZone4_LandMonsNight };

const struct WildPokemon gRoute21_LandMonsDay[] =
{
	{55, 57, SPECIES_ZOROARK},
	{55, 57, SPECIES_DOUBLADE},
	{55, 57, SPECIES_OVERQWIL},
	{55, 57, SPECIES_EXEGGUTOR},
	{55, 57, SPECIES_WEAVILE},
	{55, 57, SPECIES_TANGROWTH},
	{55, 57, SPECIES_GARDEVOIR},
	{55, 57, SPECIES_GALLADE},
	{55, 57, SPECIES_WHIMSICOTT},
	{55, 57, SPECIES_EISCUE},
	{55, 57, SPECIES_ALCREMIE_STRAWBERRY},
	{55, 57, SPECIES_BRUXISH},
};

const struct WildPokemonInfo gRoute21_LandMonsDayInfo = { 22, gRoute21_LandMonsDay };

const struct WildPokemon gRoute21_LandMonsNight[] =
{
	{55, 57, SPECIES_ZOROARK_H},
	{55, 57, SPECIES_DOUBLADE},
	{55, 57, SPECIES_OVERQWIL},
	{55, 57, SPECIES_EXEGGUTOR_A},
	{55, 57, SPECIES_SNEASLER},
	{55, 57, SPECIES_TANGROWTH},
	{55, 57, SPECIES_GARDEVOIR},
	{55, 57, SPECIES_GALLADE},
	{55, 57, SPECIES_WHIMSICOTT},
	{55, 57, SPECIES_STONJOURNER},
	{55, 57, SPECIES_ALCREMIE_STRAWBERRY},
	{55, 57, SPECIES_BRUXISH},
};

const struct WildPokemonInfo gRoute21_LandMonsNightInfo = { 22, gRoute21_LandMonsNight };

const struct WildPokemon gMansion1_LandMonsDay[] =
{
	{65, 71, SPECIES_DITTO},
	{65, 71, SPECIES_WEEZING},
	{65, 71, SPECIES_TRUBBISH},
	{65, 71, SPECIES_SEVIPER},
	{65, 71, SPECIES_GARBODOR},
	{65, 71, SPECIES_GOLBAT},
	{65, 71, SPECIES_SWALOT},
	{65, 71, SPECIES_SLOWBRO_G},
	{65, 71, SPECIES_GRIMER},
	{65, 71, SPECIES_RAPIDASH},
	{65, 71, SPECIES_CHARMANDER},
	{65, 71, SPECIES_CHARMELEON},
};

const struct WildPokemonInfo gMansion1_LandMonsDayInfo = { 5, gMansion1_LandMonsDay };

const struct WildPokemon gMansion1_LandMonsNight[] =
{
	{65, 71, SPECIES_DITTO},
	{65, 71, SPECIES_WEEZING_G},
	{65, 71, SPECIES_TRUBBISH},
	{65, 71, SPECIES_SEVIPER},
	{65, 71, SPECIES_GARBODOR},
	{65, 71, SPECIES_GOLBAT},
	{65, 71, SPECIES_SWALOT},
	{65, 71, SPECIES_SLOWKING_G},
	{65, 71, SPECIES_GRIMER},
	{65, 71, SPECIES_RAPIDASH},
	{65, 71, SPECIES_CHARMANDER},
	{65, 71, SPECIES_CHARMELEON},
};

const struct WildPokemonInfo gMansion1_LandMonsNightInfo = { 5, gMansion1_LandMonsNight };

const struct WildPokemon gMansion2_LandMonsDay[] =
{
	{65, 71, SPECIES_SKUNTANK},
	{65, 71, SPECIES_MUK},
	{65, 71, SPECIES_DITTO},
	{65, 71, SPECIES_CHARMANDER},
	{65, 71, SPECIES_MAGCARGO},
	{65, 71, SPECIES_TOXAPEX},
	{65, 71, SPECIES_HEATMOR},
	{65, 71, SPECIES_SALANDIT},
	{65, 71, SPECIES_SALAZZLE},
	{65, 71, SPECIES_CAMERUPT},
	{65, 71, SPECIES_VICTREEBEL},
	{65, 71, SPECIES_TENTACRUEL},
};

const struct WildPokemonInfo gMansion2_LandMonsDayInfo = { 5, gMansion2_LandMonsDay };

const struct WildPokemon gMansion2_LandMonsNight[] =
{
	{65, 71, SPECIES_SKUNTANK},
	{65, 71, SPECIES_MUK},
	{65, 71, SPECIES_DITTO},
	{65, 71, SPECIES_CHARMANDER},
	{65, 71, SPECIES_MAGCARGO},
	{65, 71, SPECIES_TOXAPEX},
	{65, 71, SPECIES_HEATMOR},
	{65, 71, SPECIES_SALANDIT},
	{65, 71, SPECIES_SALAZZLE},
	{65, 71, SPECIES_CAMERUPT},
	{65, 71, SPECIES_VICTREEBEL},
	{65, 71, SPECIES_TENTACRUEL},
};

const struct WildPokemonInfo gMansion2_LandMonsNightInfo = { 5, gMansion2_LandMonsNight };

const struct WildPokemon gMansion3_LandMonsDay[] =
{
	{65, 71, SPECIES_DITTO},
	{65, 71, SPECIES_ARCANINE},
	{65, 71, SPECIES_DITTO},
	{65, 71, SPECIES_ARIADOS},
	{65, 71, SPECIES_QWILFISH},
	{65, 71, SPECIES_PYROAR},
	{65, 71, SPECIES_LAMPENT},
	{65, 71, SPECIES_CHANDELURE},
	{65, 71, SPECIES_SCOLIPEDE},
	{65, 71, SPECIES_AMOONGUSS},
	{65, 71, SPECIES_TOXTRICITY},
	{65, 71, SPECIES_TOXTRICITY_LOW_KEY},
};

const struct WildPokemonInfo gMansion3_LandMonsDayInfo = { 5, gMansion3_LandMonsDay };

const struct WildPokemon gMansion3_LandMonsNight[] =
{
	{65, 71, SPECIES_DITTO},
	{65, 71, SPECIES_ARCANINE_H},
	{65, 71, SPECIES_DITTO},
	{65, 71, SPECIES_ARIADOS},
	{65, 71, SPECIES_QWILFISH_H},
	{65, 71, SPECIES_PYROAR},
	{65, 71, SPECIES_LAMPENT},
	{65, 71, SPECIES_GENGAR},
	{65, 71, SPECIES_SCOLIPEDE},
	{65, 71, SPECIES_AMOONGUSS},
	{65, 71, SPECIES_TOXTRICITY},
	{65, 71, SPECIES_TOXTRICITY_LOW_KEY},
};

const struct WildPokemonInfo gMansion3_LandMonsNightInfo = { 5, gMansion3_LandMonsNight };

const struct WildPokemon gSeafomIsland_LandMonsDay[] =
{
	{65, 71, SPECIES_CLOYSTER},
	{65, 71, SPECIES_GOLDUCK},
	{65, 71, SPECIES_EISCUE},
	{65, 71, SPECIES_GOLBAT},
	{65, 71, SPECIES_DEWGONG},
	{65, 71, SPECIES_BEARTIC},
	{65, 71, SPECIES_VANILLUXE},
	{65, 71, SPECIES_DARMANITAN_G},
	{65, 71, SPECIES_CRYOGONAL},
	{65, 71, SPECIES_JYNX},
	{65, 71, SPECIES_WALREIN},
	{65, 71, SPECIES_MAMOSWINE},
};

const struct WildPokemonInfo gSeafomIsland_LandMonsDayInfo = { 5, gSeafomIsland_LandMonsDay };

const struct WildPokemon gSeafomIsland_LandMonsNight[] =
{
	{65, 71, SPECIES_CLOYSTER},
	{65, 71, SPECIES_GOLDUCK},
	{65, 71, SPECIES_MR_RIME},
	{65, 71, SPECIES_FROSLASS},
	{65, 71, SPECIES_WEAVILE},
	{65, 71, SPECIES_BEARTIC},
	{65, 71, SPECIES_VANILLUXE},
	{65, 71, SPECIES_DARMANITAN_G},
	{65, 71, SPECIES_ABOMASNOW},
	{65, 71, SPECIES_JYNX},
	{65, 71, SPECIES_WALREIN},
	{65, 71, SPECIES_MAMOSWINE},
};

const struct WildPokemonInfo gSeafomIsland_LandMonsNightInfo = { 5, gSeafomIsland_LandMonsNight };

const struct WildPokemon gPowerPlant_LandMonsDay[] =
{
	{60, 60, SPECIES_ELECTRODE},
	{60, 60, SPECIES_TOGEDEMARU},
	{60, 60, SPECIES_MORPEKO},
	{60, 60, SPECIES_COPPERAJAH},
	{60, 60, SPECIES_KLINKLANG},
	{60, 60, SPECIES_RAICHU},
	{60, 60, SPECIES_GOLEM_A},
	{60, 60, SPECIES_AGGRON},
	{60, 60, SPECIES_VIKAVOLT},
	{60, 60, SPECIES_MAGNETON},
	{60, 60, SPECIES_KLEFKI},
	{60, 60, SPECIES_FERROTHORN},
};

const struct WildPokemonInfo gPowerPlant_LandMonsDayInfo = { 5, gPowerPlant_LandMonsDay };

const struct WildPokemon gPowerPlant_LandMonsNight[] =
{
	{60, 60, SPECIES_ELECTRODE_H},
	{60, 60, SPECIES_TOGEDEMARU},
	{60, 60, SPECIES_MORPEKO},
	{60, 60, SPECIES_COPPERAJAH},
	{60, 60, SPECIES_KLINKLANG},
	{60, 60, SPECIES_RAICHU_A},
	{60, 60, SPECIES_GOLEM},
	{60, 60, SPECIES_AGGRON},
	{60, 60, SPECIES_VIKAVOLT},
	{60, 60, SPECIES_MAGNETON},
	{60, 60, SPECIES_KLEFKI},
	{60, 60, SPECIES_FERROTHORN},
};

const struct WildPokemonInfo gPowerPlant_LandMonsNightInfo = { 5, gPowerPlant_LandMonsNight };

const struct WildPokemon gBillsGarden_LandMonsDay[] =
{
	{60, 60, SPECIES_EEVEE},
	{60, 60, SPECIES_EEVEE},
	{60, 60, SPECIES_EEVEE},
	{60, 60, SPECIES_FLAREON},
	{60, 60, SPECIES_VAPOREON},
	{60, 60, SPECIES_JOLTEON},
	{60, 60, SPECIES_UMBREON},
	{60, 60, SPECIES_ESPEON},
	{60, 60, SPECIES_LEAFEON},
	{60, 60, SPECIES_GLACEON},
	{60, 60, SPECIES_EEVEE},
	{60, 60, SPECIES_SYLVEON},
};

const struct WildPokemonInfo gBillsGarden_LandMonsDayInfo = { 21, gBillsGarden_LandMonsDay };

const struct WildPokemon gBillsGarden_LandMonsNight[] =
{
	{60, 60, SPECIES_EEVEE},
	{60, 60, SPECIES_EEVEE},
	{60, 60, SPECIES_EEVEE},
	{60, 60, SPECIES_FLAREON},
	{60, 60, SPECIES_VAPOREON},
	{60, 60, SPECIES_JOLTEON},
	{60, 60, SPECIES_UMBREON},
	{60, 60, SPECIES_ESPEON},
	{60, 60, SPECIES_LEAFEON},
	{60, 60, SPECIES_GLACEON},
	{60, 60, SPECIES_EEVEE},
	{60, 60, SPECIES_SYLVEON},
};

const struct WildPokemonInfo gBillsGarden_LandMonsNightInfo = { 21, gBillsGarden_LandMonsNight };

const struct WildPokemon gRoute23_LandMonsDay[] =
{
	{80, 80, SPECIES_ARIADOS},
	{80, 80, SPECIES_TOXTRICITY},
	{80, 80, SPECIES_ARCANINE},
	{80, 80, SPECIES_FLORGES},
	{80, 80, SPECIES_TOGEKISS},
	{80, 80, SPECIES_WYRDEER},
	{80, 80, SPECIES_SNORLAX},
	{80, 80, SPECIES_KINGDRA},
	{80, 80, SPECIES_VANILLUXE},
	{80, 80, SPECIES_LUXRAY},
	{80, 80, SPECIES_MAGMORTAR},
	{80, 80, SPECIES_DRAGONITE},
};

const struct WildPokemonInfo gRoute23_LandMonsDayInfo = { 22, gRoute23_LandMonsDay };

const struct WildPokemon gRoute23_LandMonsNight[] =
{
	{80, 80, SPECIES_ARIADOS},
	{80, 80, SPECIES_TOXTRICITY_LOW_KEY},
	//{80, 80, SPECIES_TYPHLOSION_H},
	//{80, 80, SPECIES_SAMUROTT_H},
	{80, 80, SPECIES_ARCANINE_H},
	{80, 80, SPECIES_FLORGES},
	{80, 80, SPECIES_TOGEKISS},
	{80, 80, SPECIES_WYRDEER},
	{80, 80, SPECIES_SNORLAX},
	{80, 80, SPECIES_KINGDRA},
	{80, 80, SPECIES_VANILLUXE},
	{80, 80, SPECIES_LUXRAY},
	{80, 80, SPECIES_ELECTIVIRE},
	{80, 80, SPECIES_DRAGONITE},
};

const struct WildPokemonInfo gRoute23_LandMonsNightInfo = { 22, gRoute23_LandMonsNight };

const struct WildPokemon gVictoryRoad1_LandMonsDay[] =
{
	{80, 80, SPECIES_SANDSLASH},
	{80, 80, SPECIES_MACHAMP},
	{80, 80, SPECIES_STEELIX},
	{80, 80, SPECIES_MIENSHAO},
	{80, 80, SPECIES_DURANT},
	{80, 80, SPECIES_STEELIX},
	{80, 80, SPECIES_MEDICHAM},
	{80, 80, SPECIES_RHYPERIOR},
	{80, 80, SPECIES_AGGRON},
	{80, 80, SPECIES_HARIYAMA},
	{80, 80, SPECIES_EXPLOUD},
	{80, 80, SPECIES_GARCHOMP},
};

const struct WildPokemonInfo gVictoryRoad1_LandMonsDayInfo = { 5, gVictoryRoad1_LandMonsDay };

const struct WildPokemon gVictoryRoad1_LandMonsNight[] =
{
	{80, 80, SPECIES_SANDSLASH_A},
	{80, 80, SPECIES_MACHAMP},
	{80, 80, SPECIES_STEELIX},
	{80, 80, SPECIES_MIENSHAO},
	{80, 80, SPECIES_DURANT},
	{80, 80, SPECIES_STEELIX},
	{80, 80, SPECIES_MEDICHAM},
	{80, 80, SPECIES_RHYPERIOR},
	{80, 80, SPECIES_AGGRON},
	{80, 80, SPECIES_HARIYAMA},
	{80, 80, SPECIES_EXPLOUD},
	{80, 80, SPECIES_GARCHOMP},
};

const struct WildPokemonInfo gVictoryRoad1_LandMonsNightInfo = { 5, gVictoryRoad1_LandMonsNight };

const struct WildPokemon gVictoryRoad2_LandMonsDay[] =
{
	{80, 80, SPECIES_FLOATZEL},
	{80, 80, SPECIES_CONKELDURR},
	{80, 80, SPECIES_LICKILICKY},
	{80, 80, SPECIES_EXCADRILL},
	{80, 80, SPECIES_SWOOBAT},
	{80, 80, SPECIES_SHUCKLE},
	{80, 80, SPECIES_ABSOL},
	{80, 80, SPECIES_ALAKAZAM},
	{80, 80, SPECIES_GOLBAT},
	{80, 80, SPECIES_PRIMEAPE},
	{80, 80, SPECIES_MAROWAK},
	{80, 80, SPECIES_MAWILE},
};

const struct WildPokemonInfo gVictoryRoad2_LandMonsDayInfo = { 5, gVictoryRoad2_LandMonsDay };

const struct WildPokemon gVictoryRoad2_LandMonsNight[] =
{
	{80, 80, SPECIES_FLOATZEL},
	{80, 80, SPECIES_GIGALITH},
	{80, 80, SPECIES_LICKILICKY},
	{80, 80, SPECIES_EXCADRILL},
	{80, 80, SPECIES_SWOOBAT},
	{80, 80, SPECIES_SHUCKLE},
	{80, 80, SPECIES_ABSOL},
	{80, 80, SPECIES_ALAKAZAM},
	{80, 80, SPECIES_GOLBAT},
	{80, 80, SPECIES_PRIMEAPE},
	{80, 80, SPECIES_MAROWAK_A},
	{80, 80, SPECIES_SABLEYE},
};

const struct WildPokemonInfo gVictoryRoad2_LandMonsNightInfo = { 5, gVictoryRoad2_LandMonsNight };


const struct WildPokemon gVictoryRoad3_LandMonsDay[] =
{
	{80, 80, SPECIES_MAWILE},
	{80, 80, SPECIES_ARBOK},
	{80, 80, SPECIES_AZUMARILL},
	{80, 80, SPECIES_MAGNEZONE},
	{80, 80, SPECIES_ZWEILOUS},
	{80, 80, SPECIES_DRUDDIGON},
	{80, 80, SPECIES_ONIX},
	{80, 80, SPECIES_NOIVERN},
	{80, 80, SPECIES_GENGAR},
	{80, 80, SPECIES_URSALUNA},
	{80, 80, SPECIES_EXCADRILL},
	{80, 80, SPECIES_BRONZONG},
};

const struct WildPokemonInfo gVictoryRoad3_LandMonsDayInfo = { 5, gVictoryRoad3_LandMonsDay };

const struct WildPokemon gVictoryRoad3_LandMonsNight[] =
{
	{80, 80, SPECIES_SABLEYE},
	{80, 80, SPECIES_ARBOK},
	{80, 80, SPECIES_AZUMARILL},
	{80, 80, SPECIES_MAGNEZONE},
	{80, 80, SPECIES_ZWEILOUS},
	{80, 80, SPECIES_DRUDDIGON},
	{80, 80, SPECIES_ONIX},
	{80, 80, SPECIES_NOIVERN},
	{80, 80, SPECIES_GENGAR},
	{80, 80, SPECIES_URSALUNA},
	{80, 80, SPECIES_EXCADRILL},
	{80, 80, SPECIES_BRONZONG},
};

const struct WildPokemonInfo gVictoryRoad3_LandMonsNightInfo = { 5, gVictoryRoad3_LandMonsNight };

const struct WildPokemon gFlowerField_LandMons[] =
{
	{20, 20, SPECIES_ORICORIO},
	{20, 20, SPECIES_ORICORIO_Y},
	{20, 20, SPECIES_ORICORIO_P},
	{20, 20, SPECIES_ORICORIO_P},
	{20, 20, SPECIES_ORICORIO_S},
	{20, 20, SPECIES_ORICORIO_S},
	{20, 20, SPECIES_COMFEY},
	{20, 20, SPECIES_COMFEY},
	{20, 20, SPECIES_COMFEY},
	{20, 20, SPECIES_COMFEY},
	{20, 20, SPECIES_COMFEY},
	{20, 20, SPECIES_COMFEY},
};

const struct WildPokemonInfo gFlowerField_LandMonsInfo = { 22, gFlowerField_LandMons };

//Region Water
const struct WildPokemon gPalletTown_FishingMons [] =
{
  {5, 5, SPECIES_MAGIKARP},
  {5, 5, SPECIES_CHINCHOU},
  {15, 15, SPECIES_CHINCHOU},
  {15, 15, SPECIES_HORSEA},
  {25, 25, SPECIES_GYARADOS},
  {25, 25, SPECIES_LUVDISC},
  {35, 35, SPECIES_CLAMPERL},
  {35, 35, SPECIES_CLAUNCHER},
  {45, 45, SPECIES_CLAWITZER},
  {45, 45, SPECIES_GYARADOS},
};

const struct WildPokemonInfo gPalletTown_FishingMonsInfo = { 5,  gPalletTown_FishingMons };

static const struct WildPokemon gPalletTown_SurfMons[] =
{
  {35, 35, SPECIES_MAGIKARP},
  {35, 35, SPECIES_MAGIKARP},
  {45, 45, SPECIES_GYARADOS},
  {55, 55, SPECIES_GYARADOS},
  {65, 65, SPECIES_GYARADOS},
};

static const struct WildPokemonInfo gPalletTown_SurfMonsInfo = { 5, gPalletTown_SurfMons };

const struct WildPokemon gViridianCity_FishingMons[] =
{
  {5, 5, SPECIES_QWILFISH_H},
  {5, 5, SPECIES_QWILFISH},
  {15, 15, SPECIES_ARROKUDA},
  {15, 15, SPECIES_GOLDEEN},
  {25, 25, SPECIES_ARROKUDA},
  {25, 25, SPECIES_STARYU},
  {35, 35, SPECIES_WISHIWASHI},
  {35, 35, SPECIES_MAREANIE},
  {45, 45, SPECIES_TOXAPEX},
  {45, 45, SPECIES_CRAMORANT},
};

const struct WildPokemonInfo gViridianCity_FishingMonsInfo = { 5,  gViridianCity_FishingMons };

static const struct WildPokemon gViridianCity_SurfMons[] =
{
  {35, 35, SPECIES_TENTACOOL},
  {35, 35, SPECIES_TENTACOOL},
  {45, 45, SPECIES_TENTACRUEL},
  {55, 55, SPECIES_TENTACRUEL},
  {65, 65, SPECIES_TENTACRUEL},
};

static const struct WildPokemonInfo gViridianCity_SurfMonsInfo = { 5, gViridianCity_SurfMons };

const struct WildPokemon gRoute22Cerulean_FishingMons[] =
{
  {5, 5, SPECIES_SHELLDER},
  {5, 5, SPECIES_FINNEON},
  {15, 15, SPECIES_SHELLDER},
  {15, 15, SPECIES_SEEL},
  {25, 25, SPECIES_FRILLISH},
  {25, 25, SPECIES_GOLISOPOD},
  {35, 35, SPECIES_WISHIWASHI},
  {35, 35, SPECIES_MANTYKE},
  {45, 45, SPECIES_MANTINE},
  {45, 45, SPECIES_SHARPEDO},
};

const struct WildPokemonInfo gRoute22Cerulean_FishingMonsInfo = { 5,  gRoute22Cerulean_FishingMons };

static const struct WildPokemon gRoute22Cerulean_SurfMons[] =
{
  {35, 35, SPECIES_FINNEON},
  {35, 35, SPECIES_FINNEON},
  {45, 45, SPECIES_LUMINEON},
  {55, 55, SPECIES_LUMINEON},
  {65, 65, SPECIES_LUMINEON},
};

static const struct WildPokemonInfo gRoute22Cerulean_SurfMonsInfo = { 5, gRoute22Cerulean_SurfMons };

const struct WildPokemon gCeruleanCity_FishingMons[] =
{
  {5, 5, SPECIES_CORSOLA_G},
  {5, 5, SPECIES_MAREANIE},
  {15, 15, SPECIES_CHINCHOU},
  {15, 15, SPECIES_CLAMPERL},
  {25, 25, SPECIES_PYUKUMUKU},
  {25, 25, SPECIES_FLOATZEL},
  {35, 35, SPECIES_BASCULIN_H},
  {35, 35, SPECIES_TOXAPEX},
  {45, 45, SPECIES_PALPITOAD},
  {45, 45, SPECIES_DREDNAW},
};

const struct WildPokemonInfo gCeruleanCity_FishingMonsInfo = { 5,  gCeruleanCity_FishingMons };

static const struct WildPokemon gCeruleanCity_SurfMons[] =
{
  {35, 35, SPECIES_TYNAMO},
  {35, 35, SPECIES_TYNAMO},
  {45, 45, SPECIES_EELEKTRIK},
  {55, 55, SPECIES_EELEKTRIK},
  {65, 65, SPECIES_EELEKTROSS},
};

static const struct WildPokemonInfo gCeruleanCity_SurfMonsInfo = { 5, gCeruleanCity_SurfMons };

const struct WildPokemon gRoute2425_FishingMons[] =
{
  {5, 5, SPECIES_FRILLISH},
  {5, 5, SPECIES_BASCULIN_RED},
  {15, 15, SPECIES_WAILMER},
  {15, 15, SPECIES_WAILMER},
  {25, 25, SPECIES_CLAMPERL},
  {25, 25, SPECIES_BARBOACH},
  {35, 35, SPECIES_BARBOACH},
  {35, 35, SPECIES_SHELLDER},
  {45, 45, SPECIES_WHISCASH},
  {45, 45, SPECIES_WHISCASH},
};

const struct WildPokemonInfo gRoute2425_FishingMonsInfo = { 5,  gRoute2425_FishingMons };

static const struct WildPokemon gRoute2425_SurfMons[] =
{
  {35, 35, SPECIES_CARVANHA},
  {35, 35, SPECIES_CARVANHA},
  {45, 45, SPECIES_SHARPEDO},
  {55, 55, SPECIES_SHARPEDO},
  {65, 65, SPECIES_SHARPEDO},
};

static const struct WildPokemonInfo gRoute2425_SurfMonsInfo = { 5, gRoute2425_SurfMons };

const struct WildPokemon gRoute25_FishingMons[] =
{
  {5, 5, SPECIES_REMORAID},
  {5, 5, SPECIES_CHEWTLE},
  {15, 15, SPECIES_CORPHISH},
  {15, 15, SPECIES_BARBOACH},
  {25, 25, SPECIES_WOOPER},
  {25, 25, SPECIES_GOLDEEN},
  {35, 35, SPECIES_SEADRA},
  {35, 35, SPECIES_AVALUGG},
  {45, 45, SPECIES_WALREIN},
  {45, 45, SPECIES_KINGDRA},
};

const struct WildPokemonInfo gRoute25_FishingMonsInfo = { 5,  gRoute25_FishingMons };

static const struct WildPokemon gRoute25_SurfMons[] =
{
  {35, 35, SPECIES_FEEBAS},
  {35, 35, SPECIES_FEEBAS},
  {45, 45, SPECIES_MILOTIC},
  {55, 55, SPECIES_MILOTIC},
  {65, 65, SPECIES_MILOTIC},
};

static const struct WildPokemonInfo gRoute25_SurfMonsInfo = { 5, gRoute25_SurfMons };

const struct WildPokemon gRoute6_FishingMons[] =
{
  {5, 5, SPECIES_WINGULL},
  {5, 5, SPECIES_CLOBBOPUS},
  {15, 15, SPECIES_WISHIWASHI},
  {15, 15, SPECIES_MAREANIE},
  {25, 25, SPECIES_MANTYKE},
  {25, 25, SPECIES_CLAUNCHER},
  {35, 35, SPECIES_STARMIE},
  {35, 35, SPECIES_LANTURN},
  {45, 45, SPECIES_OVERQWIL},
  {45, 45, SPECIES_DHELMISE},
};

const struct WildPokemonInfo gRoute6_FishingMonsInfo = { 5,  gRoute6_FishingMons };

static const struct WildPokemon gRoute6_SurfMons[] =
{
  {35, 35, SPECIES_WINGULL},
  {35, 35, SPECIES_WINGULL},
  {45, 45, SPECIES_PELIPPER},
  {55, 55, SPECIES_PELIPPER},
  {65, 65, SPECIES_PELIPPER},
};

static const struct WildPokemonInfo gRoute6_SurfMonsInfo = { 5, gRoute6_SurfMons };

const struct WildPokemon gVermillionCity_FishingMons[] =
{
  {5, 5, SPECIES_BASCULIN_RED},
  {5, 5, SPECIES_BASCULIN_H},
  {15, 15, SPECIES_CHINCHOU},
  {15, 15, SPECIES_REMORAID},
  {25, 25, SPECIES_BASCULIN_BLUE},
  {25, 25, SPECIES_STARYU},
  {35, 35, SPECIES_REMORAID},
  {35, 35, SPECIES_OCTILLERY},
  {45, 45, SPECIES_DRATINI},
  {45, 45, SPECIES_DRATINI},
};

const struct WildPokemonInfo gVermillionCity_FishingMonsInfo = { 5,  gVermillionCity_FishingMons };

static const struct WildPokemon gVermillionCity_SurfMons[] =
{
  {35, 35, SPECIES_SEEL},
  {35, 35, SPECIES_SEEL},
  {45, 45, SPECIES_DEWGONG},
  {55, 55, SPECIES_DEWGONG},
  {65, 65, SPECIES_DEWGONG},
};

static const struct WildPokemonInfo gVermillionCity_SurfMonsInfo = { 5, gVermillionCity_SurfMons };

const struct WildPokemon gRoute11_FishingMons[] =
{
  {5, 5, SPECIES_BERGMITE},
  {5, 5, SPECIES_LUVDISC},
  {15, 15, SPECIES_WAILMER},
  {15, 15, SPECIES_FEEBAS},
  {25, 25, SPECIES_CORPHISH},
  {25, 25, SPECIES_LUMINEON},
  {35, 35, SPECIES_CRAWDAUNT},
  {35, 35, SPECIES_ARAQUANID},
  {45, 45, SPECIES_BARRASKEWDA},
  {45, 45, SPECIES_GYARADOS},
};

const struct WildPokemonInfo gRoute11_FishingMonsInfo = { 5,  gRoute11_FishingMons };

static const struct WildPokemon gRoute11_SurfMons[] =
{
  {35, 35, SPECIES_PIPLUP},
  {35, 35, SPECIES_PIPLUP},
  {45, 45, SPECIES_PRINPLUP},
  {55, 55, SPECIES_PRINPLUP},
  {65, 65, SPECIES_EMPOLEON},
};

static const struct WildPokemonInfo gRoute11_SurfMonsInfo = { 5, gRoute11_SurfMons };

const struct WildPokemon gRoute10_FishingMons[] =
{
  {5, 5, SPECIES_CORPHISH},
  {5, 5, SPECIES_BARBOACH},
  {15, 15, SPECIES_BASCULIN_H},
  {15, 15, SPECIES_FINNEON},
  {25, 25, SPECIES_SHELLOS},
  {25, 25, SPECIES_BARBOACH},
  {35, 35, SPECIES_GASTRODON},
  {35, 35, SPECIES_PALPITOAD},
  {45, 45, SPECIES_SEISMITOAD},
  {45, 45, SPECIES_GYARADOS},
};

const struct WildPokemonInfo gRoute10_FishingMonsInfo = { 5,  gRoute10_FishingMons };

static const struct WildPokemon gRoute10_SurfMons[] =
{
  {35, 35, SPECIES_BARBOACH},
  {35, 35, SPECIES_BARBOACH},
  {45, 45, SPECIES_WHISCASH},
  {55, 55, SPECIES_WHISCASH},
  {65, 65, SPECIES_WHISCASH},
};

static const struct WildPokemonInfo gRoute10_SurfMonsInfo = { 5, gRoute10_SurfMons };

const struct WildPokemon gRoute12_FishingMons[] =
{
  {5, 5, SPECIES_LUVDISC},
  {5, 5, SPECIES_BARBOACH},
  {15, 15, SPECIES_FINNEON},
  {15, 15, SPECIES_CLAUNCHER},
  {25, 25, SPECIES_FEEBAS},
  {25, 25, SPECIES_GOREBYSS},
  {35, 35, SPECIES_GASTRODON},
  {35, 35, SPECIES_RELICANTH},
  {45, 45, SPECIES_BRUXISH},
  {45, 45, SPECIES_GYARADOS},
};

const struct WildPokemonInfo gRoute12_FishingMonsInfo = { 5,  gRoute12_FishingMons };

static const struct WildPokemon gRoute12_SurfMons[] =
{
  {35, 35, SPECIES_LUVDISC},
  {35, 35, SPECIES_LUVDISC},
  {45, 45, SPECIES_ALOMOMOLA},
  {55, 55, SPECIES_ALOMOMOLA},
  {65, 65, SPECIES_ALOMOMOLA},
};

static const struct WildPokemonInfo gRoute12_SurfMonsInfo = { 5, gRoute12_SurfMons };

const struct WildPokemon gSafariZone_FishingMons[] =
{
  {5, 5, SPECIES_DRATINI},
  {5, 5, SPECIES_QWILFISH},
  {15, 15, SPECIES_BIBAREL},
  {15, 15, SPECIES_SHELLOS},
  {25, 25, SPECIES_BARBOACH},
  {25, 25, SPECIES_PALPITOAD},
  {35, 35, SPECIES_GASTRODON},
  {35, 35, SPECIES_EELEKTROSS},
  {45, 45, SPECIES_CLAUNCHER},
  {45, 45, SPECIES_GRAPPLOCT},
};

const struct WildPokemonInfo gSafariZone_FishingMonsInfo = { 5,  gSafariZone_FishingMons };

static const struct WildPokemon gSafariZone_SurfMons[] =
{
  {35, 35, SPECIES_DRATINI},
  {35, 35, SPECIES_DRATINI},
  {45, 45, SPECIES_DRAGONAIR},
  {55, 55, SPECIES_DRAGONAIR},
  {65, 65, SPECIES_DRAGONAIR},
};

static const struct WildPokemonInfo gSafariZone_SurfMonsInfo = { 5, gSafariZone_SurfMons };

const struct WildPokemon gRoute19_FishingMons[] =
{
  {5, 5, SPECIES_LUVDISC},
  {5, 5, SPECIES_MAGIKARP},
  {15, 15, SPECIES_WIMPOD},
  {15, 15, SPECIES_ARAQUANID},
  {25, 25, SPECIES_FEEBAS},
  {25, 25, SPECIES_GOREBYSS},
  {35, 35, SPECIES_JELLICENT},
  {35, 35, SPECIES_LUMINEON},
  {45, 45, SPECIES_BRUXISH},
  {45, 45, SPECIES_GYARADOS},
};

const struct WildPokemonInfo gRoute19_FishingMonsInfo = { 5,  gRoute19_FishingMons };

static const struct WildPokemon gRoute19_SurfMons[] =
{
  {35, 35, SPECIES_CHINCHOU},
  {35, 35, SPECIES_CHINCHOU},
  {45, 45, SPECIES_LANTURN},
  {55, 55, SPECIES_LANTURN},
  {65, 65, SPECIES_LANTURN},
};

static const struct WildPokemonInfo gRoute19_SurfMonsInfo = { 5, gRoute19_SurfMons };

const struct WildPokemon gSeafoamIslands_FishingMons[] =
{
  {5, 5, SPECIES_FEEBAS},
  {5, 5, SPECIES_FEEBAS},
  {15, 15, SPECIES_SEEL},
  {15, 15, SPECIES_SHELLDER},
  {25, 25, SPECIES_CLOYSTER},
  {25, 25, SPECIES_FRILLISH},
  {35, 35, SPECIES_JELLICENT},
  {35, 35, SPECIES_GYARADOS},
  {45, 45, SPECIES_GOLDUCK},
  {45, 45, SPECIES_EISCUE},
};

const struct WildPokemonInfo gSeafoamIslands_FishingMonsInfo = { 5,  gSeafoamIslands_FishingMons };

static const struct WildPokemon gSeafoamIslands_SurfMons[] =
{
  {35, 35, SPECIES_EISCUE},
  {35, 35, SPECIES_EISCUE},
  {45, 45, SPECIES_BRUXISH},
  {55, 55, SPECIES_BRUXISH},
  {65, 65, SPECIES_BRUXISH},
};

static const struct WildPokemonInfo gSeafoamIslands_SurfMonsInfo = { 5, gSeafoamIslands_SurfMons };

static const struct WildPokemon gRoute20_SurfMons[] =
{
  {35, 35, SPECIES_ARROKUDA},
  {35, 35, SPECIES_ARROKUDA},
  {45, 45, SPECIES_BARRASKEWDA},
  {55, 55, SPECIES_BARRASKEWDA},
  {65, 65, SPECIES_BARRASKEWDA},
};

static const struct WildPokemonInfo gRoute20_SurfMonsInfo = { 5, gRoute20_SurfMons };

static const struct WildPokemon gRoute21_SurfMons[] =
{
  {35, 35, SPECIES_FRILLISH},
  {35, 35, SPECIES_FRILLISH},
  {45, 45, SPECIES_JELLICENT},
  {55, 55, SPECIES_JELLICENT},
  {65, 65, SPECIES_JELLICENT},
};

static const struct WildPokemonInfo gRoute21_SurfMonsInfo = { 5, gRoute21_SurfMons };

static const struct WildPokemon gRoute23_SurfMons[] =
{
  {35, 35, SPECIES_SHELLOS},
  {35, 35, SPECIES_SHELLOS},
  {45, 45, SPECIES_GASTRODON},
  {55, 55, SPECIES_GASTRODON},
  {65, 65, SPECIES_GASTRODON},
};

static const struct WildPokemonInfo gRoute23_SurfMonsInfo = { 5, gRoute23_SurfMons };

//Region Water
static const struct WildPokemon gViridianForest_RockSmashMons[] =
{
	{15, 15, SPECIES_DWEBBLE},
	{15, 15, SPECIES_DWEBBLE},
	{15, 15, SPECIES_DWEBBLE},
	{15, 15, SPECIES_DWEBBLE},
	{15, 15, SPECIES_DWEBBLE},
};

static const struct WildPokemonInfo gViridianForest_RockSmashInfo = { 50, gViridianForest_RockSmashMons };

const struct WildPokemon gBillsGarden_FishingMons[] =
{
  {5, 5, SPECIES_VAPOREON},
  {5, 5, SPECIES_VAPOREON},
  {15, 15, SPECIES_VAPOREON},
  {15, 15, SPECIES_VAPOREON},
  {25, 25, SPECIES_VAPOREON},
  {25, 25, SPECIES_VAPOREON},
  {35, 35, SPECIES_VAPOREON},
  {35, 35, SPECIES_VAPOREON},
  {45, 45, SPECIES_VAPOREON},
  {45, 45, SPECIES_VAPOREON},
};

const struct WildPokemonInfo gBillsGarden_FishingMonsInfo = { 5,  gBillsGarden_FishingMons };

static const struct WildPokemon gBillsGarden_SurfMons[] =
{
  {35, 35, SPECIES_VAPOREON},
  {35, 35, SPECIES_VAPOREON},
  {45, 45, SPECIES_VAPOREON},
  {55, 55, SPECIES_VAPOREON},
  {65, 65, SPECIES_VAPOREON},
};

static const struct WildPokemonInfo gBillsGarden_SurfMonsInfo = { 5, gBillsGarden_SurfMons };


const struct WildPokemonHeader gWildMonMorningHeaders[] =
{
	{
		.mapGroup = MAP_GROUP(PALLET_TOWN),
		.mapNum = MAP_NUM(PALLET_TOWN),
		.landMonsInfo = NULL,
		.waterMonsInfo = &gPalletTown_SurfMonsInfo,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = &gPalletTown_FishingMonsInfo,
	},
	{
		.mapGroup = MAP_GROUP(ROUTE_1),
		.mapNum = MAP_NUM(ROUTE_1),
		.landMonsInfo = &gRoute1_LandMonsDayInfo,
		.waterMonsInfo = NULL,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = NULL,
	},
	{
		.mapGroup = MAP_GROUP(ROUTE_22),
		.mapNum = MAP_NUM(ROUTE_22),
		.landMonsInfo = &gRoute22_LandMonsDayInfo,
		.waterMonsInfo = &gRoute22Cerulean_SurfMonsInfo,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = &gRoute22Cerulean_FishingMonsInfo,
	},
	{
		.mapGroup = MAP_GROUP(CERULEAN_CITY),
		.mapNum = MAP_NUM(CERULEAN_CITY),
		.landMonsInfo = NULL,
		.waterMonsInfo = &gCeruleanCity_SurfMonsInfo,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = &gCeruleanCity_FishingMonsInfo,
	},
		{
		.mapGroup = MAP_GROUP(VIRIDIAN_CITY),
		.mapNum = MAP_NUM(VIRIDIAN_CITY),
		.landMonsInfo = &gViridianCity_LandMonsDayInfo,
		.waterMonsInfo = &gViridianCity_SurfMonsInfo,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = &gViridianCity_FishingMonsInfo,
	},
		{
		.mapGroup = MAP_GROUP(ROUTE_2),
		.mapNum = MAP_NUM(ROUTE_2),
		.landMonsInfo = &gRoute2_LandMonsDayInfo,
		.waterMonsInfo = NULL,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = NULL,
	},
	{
		.mapGroup = MAP_GROUP(VIRIDIAN_FOREST),
		.mapNum = MAP_NUM(VIRIDIAN_FOREST),
		.landMonsInfo = &gViridianForest1_LandMonsDayInfo,
		.waterMonsInfo = NULL,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = NULL,
	},
	{
		.mapGroup = 43,
		.mapNum = 0,
		.landMonsInfo = &gViridianForest2_LandMonsDayInfo,
		.waterMonsInfo = NULL,
		.rockSmashMonsInfo = &gViridianForest_RockSmashInfo,
		.fishingMonsInfo = NULL,
	},
	{
		.mapGroup = 1,
		.mapNum = 36,
		.landMonsInfo = &gDiglettCave1_LandMonsDayInfo,
		.waterMonsInfo = NULL,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = NULL,
	},
						{
		.mapGroup = MAP_GROUP(PEWTER_CITY),
		.mapNum = MAP_NUM(PEWTER_CITY),
		.landMonsInfo = &gPewterCity_LandMonsDayInfo,
		.waterMonsInfo = NULL,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = NULL,
						},
	{
		.mapGroup = MAP_GROUP(ROUTE_3),
		.mapNum = MAP_NUM(ROUTE_3),
		.landMonsInfo = &gRoute3_LandMonsDayInfo,
		.waterMonsInfo = NULL,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = NULL,
	},
	{
		.mapGroup = MAP_GROUP(MT_MOON_1F),
		.mapNum = MAP_NUM(MT_MOON_1F),
		.landMonsInfo = &gMtMoon1_LandMonsDayInfo,
		.waterMonsInfo = NULL,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = NULL,
	},
	{
		.mapGroup = MAP_GROUP(MT_MOON_B1F),
		.mapNum = MAP_NUM(MT_MOON_B1F),
		.landMonsInfo = &gMtMoon2_LandMonsDayInfo,
		.waterMonsInfo = NULL,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = NULL,
	},
	{
		.mapGroup = MAP_GROUP(MT_MOON_B2F),
		.mapNum = MAP_NUM(MT_MOON_B2F),
		.landMonsInfo = &gMtMoon3_LandMonsDayInfo,
		.waterMonsInfo = NULL,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = NULL,
	},
	{
		.mapGroup = MAP_GROUP(ROUTE_4),
		.mapNum = MAP_NUM(ROUTE_4),
		.landMonsInfo = &gRoute4_LandMonsDayInfo,
		.waterMonsInfo = NULL,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = NULL,
	},
	{
		.mapGroup = 43,
		.mapNum = 17,
		.landMonsInfo = &gFlowerField_LandMonsInfo,
		.waterMonsInfo = NULL,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = NULL,
	},
	{
		.mapGroup = MAP_GROUP(ROUTE_24),
		.mapNum = MAP_NUM(ROUTE_24),
		.landMonsInfo = &gRoute24_LandMonsDayInfo,
		.waterMonsInfo = &gRoute2425_SurfMonsInfo,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = &gRoute2425_FishingMonsInfo,
	},
	{
		.mapGroup = MAP_GROUP(ROUTE_25),
		.mapNum = MAP_NUM(ROUTE_25),
		.landMonsInfo = &gRoute25_LandMonsDayInfo,
		.waterMonsInfo = &gRoute25_SurfMonsInfo,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = &gRoute25_FishingMonsInfo,
	},
	{
		.mapGroup = MAP_GROUP(ROUTE_5),
		.mapNum = MAP_NUM(ROUTE_5),
		.landMonsInfo = &gRoute5_LandMonsDayInfo,
		.waterMonsInfo = NULL,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = NULL,
	},
	{
		.mapGroup = MAP_GROUP(ROUTE_6),
		.mapNum = MAP_NUM(ROUTE_6),
		.landMonsInfo = &gRoute6_LandMonsDayInfo,
		.waterMonsInfo = &gRoute6_SurfMonsInfo,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = &gRoute6_FishingMonsInfo,
	},
	{
		.mapGroup = MAP_GROUP(ROUTE_11),
		.mapNum = MAP_NUM(ROUTE_11),
		.landMonsInfo = &gRoute11_LandMonsDayInfo,
		.waterMonsInfo = &gRoute11_SurfMonsInfo,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = &gRoute11_FishingMonsInfo,
	},
	{
		.mapGroup = 1,
		.mapNum = 37,
		.landMonsInfo = &gDiglettCave2_LandMonsDayInfo,
		.waterMonsInfo = NULL,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = NULL,
	},
	{
		.mapGroup = MAP_GROUP(VERMILION_CITY),
		.mapNum = MAP_NUM(VERMILION_CITY),
		.landMonsInfo = NULL,
		.waterMonsInfo = &gVermillionCity_SurfMonsInfo,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = &gVermillionCity_FishingMonsInfo,
	},
	{
		.mapGroup = MAP_GROUP(ROUTE_9),
		.mapNum = MAP_NUM(ROUTE_9),
		.landMonsInfo = &gRoute9_LandMonsDayInfo,
		.waterMonsInfo = NULL,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = NULL,
	},
	{
		.mapGroup = MAP_GROUP(ROUTE_10),
		.mapNum = MAP_NUM(ROUTE_10),
		.landMonsInfo = &gRoute10_LandMonsDayInfo,
		.waterMonsInfo = &gRoute10_SurfMonsInfo,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = &gRoute10_FishingMonsInfo,
	},
	{
		.mapGroup = MAP_GROUP(ROCK_TUNNEL_1F),
		.mapNum = MAP_NUM(ROCK_TUNNEL_1F),
		.landMonsInfo = &gRockTunnel1_LandMonsDayInfo,
		.waterMonsInfo = NULL,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = NULL,
	},
	{
		.mapGroup = MAP_GROUP(ROCK_TUNNEL_B1F),
		.mapNum = MAP_NUM(ROCK_TUNNEL_B1F),
		.landMonsInfo = &gRockTunnel2_LandMonsDayInfo,
		.waterMonsInfo = NULL,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = NULL,
	},
	{
		.mapGroup = MAP_GROUP(ROUTE_8),
		.mapNum = MAP_NUM(ROUTE_8),
		.landMonsInfo = &gRoute8_LandMonsDayInfo,
		.waterMonsInfo = NULL,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = NULL,
	},
	{
		.mapGroup = MAP_GROUP(ROUTE_7),
		.mapNum = MAP_NUM(ROUTE_7),
		.landMonsInfo = &gRoute7_LandMonsDayInfo,
		.waterMonsInfo = NULL,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = NULL,
	},
	{
		.mapGroup = MAP_GROUP(POKEMON_TOWER_3F),
		.mapNum = MAP_NUM(POKEMON_TOWER_3F),
		.landMonsInfo = &gPokemonTower1_LandMonsDayInfo,
		.waterMonsInfo = NULL,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = NULL,
	},
	{
		.mapGroup = MAP_GROUP(POKEMON_TOWER_4F),
		.mapNum = MAP_NUM(POKEMON_TOWER_4F),
		.landMonsInfo = &gPokemonTower2_LandMonsDayInfo,
		.waterMonsInfo = NULL,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = NULL,
	},
	{
		.mapGroup = MAP_GROUP(POKEMON_TOWER_5F),
		.mapNum = MAP_NUM(POKEMON_TOWER_5F),
		.landMonsInfo = &gPokemonTower3_LandMonsDayInfo,
		.waterMonsInfo = NULL,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = NULL,
	},
	{
		.mapGroup = MAP_GROUP(POKEMON_TOWER_6F),
		.mapNum = MAP_NUM(POKEMON_TOWER_6F),
		.landMonsInfo = &gPokemonTower4_LandMonsDayInfo,
		.waterMonsInfo = NULL,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = NULL,
	},
		{
		.mapGroup = MAP_GROUP(POKEMON_TOWER_7F),
		.mapNum = MAP_NUM(POKEMON_TOWER_7F),
		.landMonsInfo = &gPokemonTower4_LandMonsDayInfo,
		.waterMonsInfo = NULL,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = NULL,
	},
	{
		.mapGroup = MAP_GROUP(ROUTE_16),
		.mapNum = MAP_NUM(ROUTE_16),
		.landMonsInfo = &gRoute16_LandMonsDayInfo,
		.waterMonsInfo = NULL,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = NULL,
	},
	{
		.mapGroup = MAP_GROUP(ROUTE_17),
		.mapNum = MAP_NUM(ROUTE_17),
		.landMonsInfo = &gRoute17_LandMonsDayInfo,
		.waterMonsInfo = NULL,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = NULL,
	},
	{
		.mapGroup = MAP_GROUP(ROUTE_18),
		.mapNum = MAP_NUM(ROUTE_18),
		.landMonsInfo = &gRoute18_LandMonsDayInfo,
		.waterMonsInfo = NULL,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = NULL,
	},
	{
		.mapGroup = MAP_GROUP(ROUTE_12),
		.mapNum = MAP_NUM(ROUTE_12),
		.landMonsInfo = &gRoute12_LandMonsDayInfo,
		.waterMonsInfo = &gRoute12_SurfMonsInfo,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = &gRoute12_FishingMonsInfo,
	},
	{
		.mapGroup = MAP_GROUP(ROUTE_13),
		.mapNum = MAP_NUM(ROUTE_13),
		.landMonsInfo = &gRoute12_LandMonsDayInfo,
		.waterMonsInfo = &gRoute12_SurfMonsInfo,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = &gRoute12_FishingMonsInfo,
	},
	{
		.mapGroup = MAP_GROUP(ROUTE_14),
		.mapNum = MAP_NUM(ROUTE_14),
		.landMonsInfo = &gRoute14_LandMonsDayInfo,
		.waterMonsInfo = NULL,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = NULL,
	},
	{
		.mapGroup = MAP_GROUP(ROUTE_15),
		.mapNum = MAP_NUM(ROUTE_15),
		.landMonsInfo = &gRoute14_LandMonsDayInfo,
		.waterMonsInfo = NULL,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = NULL,
	},
	{
		.mapGroup = 1,
		.mapNum = 63,
		.landMonsInfo = &gSafariZone1_LandMonsDayInfo,
		.waterMonsInfo = &gSafariZone_SurfMonsInfo,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = &gSafariZone_FishingMonsInfo,
	},
	{
		.mapGroup = 1,
		.mapNum = 64,
		.landMonsInfo = &gSafariZone2_LandMonsDayInfo,
		.waterMonsInfo = &gSafariZone_SurfMonsInfo,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = &gSafariZone_FishingMonsInfo,
	},
	{
		.mapGroup = 1,
		.mapNum = 65,
		.landMonsInfo = &gSafariZone3_LandMonsDayInfo,
		.waterMonsInfo = &gSafariZone_SurfMonsInfo,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = &gSafariZone_FishingMonsInfo,
	},
	{
		.mapGroup = 1,
		.mapNum = 66,
		.landMonsInfo = &gSafariZone4_LandMonsDayInfo,
		.waterMonsInfo = &gSafariZone_SurfMonsInfo,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = &gSafariZone_FishingMonsInfo,
	},
	{
		.mapGroup = MAP_GROUP(ROUTE_19),
		.mapNum = MAP_NUM(ROUTE_19),
		.landMonsInfo = NULL,
		.waterMonsInfo = &gRoute19_SurfMonsInfo,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = &gRoute19_FishingMonsInfo,
	},
	{
		.mapGroup = MAP_GROUP(ROUTE_20),
		.mapNum = MAP_NUM(ROUTE_20),
		.landMonsInfo = NULL,
		.waterMonsInfo = &gRoute20_SurfMonsInfo,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = &gRoute19_FishingMonsInfo,
	},
	{
		.mapGroup = MAP_GROUP(ROUTE_21_A),
		.mapNum = MAP_NUM(ROUTE_21_A),
		.landMonsInfo = &gRoute21_LandMonsDayInfo,
		.waterMonsInfo = &gRoute21_SurfMonsInfo,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = &gRoute19_FishingMonsInfo,
	},
	{
		.mapGroup = MAP_GROUP(ROUTE_21_B),
		.mapNum = MAP_NUM(ROUTE_21_B),
		.landMonsInfo = NULL,
		.waterMonsInfo = &gRoute21_SurfMonsInfo,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = &gRoute19_FishingMonsInfo,
	},
	{
		.mapGroup = MAP_GROUP(POKEMON_MANSION_1F),
		.mapNum = MAP_NUM(POKEMON_MANSION_1F),
		.landMonsInfo = &gMansion1_LandMonsDayInfo,
		.waterMonsInfo = NULL,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = NULL,
	},
	{
		.mapGroup = MAP_GROUP(POKEMON_MANSION_2F),
		.mapNum = MAP_NUM(POKEMON_MANSION_2F),
		.landMonsInfo = &gMansion2_LandMonsDayInfo,
		.waterMonsInfo = NULL,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = NULL,
	},
	{
		.mapGroup = MAP_GROUP(POKEMON_MANSION_3F),
		.mapNum = MAP_NUM(POKEMON_MANSION_3F),
		.landMonsInfo = &gMansion3_LandMonsDayInfo,
		.waterMonsInfo = NULL,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = NULL,
	},
	{
		.mapGroup = MAP_GROUP(POKEMON_MANSION_B1F),
		.mapNum = MAP_NUM(POKEMON_MANSION_B1F),
		.landMonsInfo = &gMansion3_LandMonsDayInfo,
		.waterMonsInfo = NULL,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = NULL,
	},
	{
		.mapGroup = MAP_GROUP(SEAFOAM_ISLANDS_1F),
		.mapNum = MAP_NUM(SEAFOAM_ISLANDS_1F),
		.landMonsInfo = &gSeafomIsland_LandMonsDayInfo,
		.waterMonsInfo = &gSeafoamIslands_SurfMonsInfo,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = &gSeafoamIslands_FishingMonsInfo,
	},
	{
		.mapGroup = MAP_GROUP(SEAFOAM_ISLANDS_B1F),
		.mapNum = MAP_NUM(SEAFOAM_ISLANDS_B1F),
		.landMonsInfo = &gSeafomIsland_LandMonsDayInfo,
		.waterMonsInfo = &gSeafoamIslands_SurfMonsInfo,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = &gSeafoamIslands_FishingMonsInfo,
	},
	{
		.mapGroup = MAP_GROUP(SEAFOAM_ISLANDS_B2F),
		.mapNum = MAP_NUM(SEAFOAM_ISLANDS_B2F),
		.landMonsInfo = &gSeafomIsland_LandMonsDayInfo,
		.waterMonsInfo = &gSeafoamIslands_SurfMonsInfo,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = &gSeafoamIslands_FishingMonsInfo,
	},
	{
		.mapGroup = MAP_GROUP(SEAFOAM_ISLANDS_B3F),
		.mapNum = MAP_NUM(SEAFOAM_ISLANDS_B3F),
		.landMonsInfo = &gSeafomIsland_LandMonsDayInfo,
		.waterMonsInfo = &gSeafoamIslands_SurfMonsInfo,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = &gSeafoamIslands_FishingMonsInfo,
	},
	{
		.mapGroup = MAP_GROUP(SEAFOAM_ISLANDS_B4F),
		.mapNum = MAP_NUM(SEAFOAM_ISLANDS_B4F),
		.landMonsInfo = &gSeafomIsland_LandMonsDayInfo,
		.waterMonsInfo = &gSeafoamIslands_SurfMonsInfo,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = &gSeafoamIslands_FishingMonsInfo,
	},
	{
		.mapGroup = MAP_GROUP(POWER_PLANT),
		.mapNum = MAP_NUM(POWER_PLANT),
		.landMonsInfo = &gPowerPlant_LandMonsDayInfo,
		.waterMonsInfo = NULL,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = NULL,
	},
	{
		.mapGroup = 43,
		.mapNum = 16,
		.landMonsInfo = &gBillsGarden_LandMonsDayInfo,
		.waterMonsInfo = &gBillsGarden_SurfMonsInfo,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = &gBillsGarden_FishingMonsInfo,
	},
	{
		.mapGroup = MAP_GROUP(ROUTE_23),
		.mapNum = MAP_NUM(ROUTE_23),
		.landMonsInfo = &gRoute23_LandMonsDayInfo,
		.waterMonsInfo = &gRoute23_SurfMonsInfo,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = &gSeafoamIslands_FishingMonsInfo,
	},
	{
		.mapGroup = MAP_GROUP(VICTORY_ROAD_1F),
		.mapNum = MAP_NUM(VICTORY_ROAD_1F),
		.landMonsInfo = &gVictoryRoad1_LandMonsDayInfo,
		.waterMonsInfo = NULL,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = NULL,
	},
	{
		.mapGroup = MAP_GROUP(VICTORY_ROAD_2F),
		.mapNum = MAP_NUM(VICTORY_ROAD_2F),
		.landMonsInfo = &gVictoryRoad2_LandMonsDayInfo,
		.waterMonsInfo = NULL,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = NULL,
	},
	{
		.mapGroup = MAP_GROUP(VICTORY_ROAD_3F),
		.mapNum = MAP_NUM(VICTORY_ROAD_3F),
		.landMonsInfo = &gVictoryRoad3_LandMonsDayInfo,
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
	},

};

const struct WildPokemonHeader gWildMonEveningHeaders[] =
{
	{
		.mapGroup = MAP_GROUP(PALLET_TOWN),
		.mapNum = MAP_NUM(PALLET_TOWN),
		.landMonsInfo = NULL,
		.waterMonsInfo = &gPalletTown_SurfMonsInfo,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = &gPalletTown_FishingMonsInfo,
	},
	{
		.mapGroup = MAP_GROUP(ROUTE_1),
		.mapNum = MAP_NUM(ROUTE_1),
		.landMonsInfo = &gRoute1_LandMonsNightInfo,
		.waterMonsInfo = NULL,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = NULL,
	},
	{
		.mapGroup = MAP_GROUP(ROUTE_22),
		.mapNum = MAP_NUM(ROUTE_22),
		.landMonsInfo = &gRoute22_LandMonsNightInfo,
		.waterMonsInfo = &gRoute22Cerulean_SurfMonsInfo,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = &gRoute22Cerulean_FishingMonsInfo,
	},
	{
		.mapGroup = MAP_GROUP(CERULEAN_CITY),
		.mapNum = MAP_NUM(CERULEAN_CITY),
		.landMonsInfo = NULL,
		.waterMonsInfo = &gCeruleanCity_SurfMonsInfo,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = &gCeruleanCity_FishingMonsInfo,
	},
	{
		.mapGroup = MAP_GROUP(VIRIDIAN_CITY),
		.mapNum = MAP_NUM(VIRIDIAN_CITY),
		.landMonsInfo = &gViridianCity_LandMonsNightInfo,
		.waterMonsInfo = &gViridianCity_SurfMonsInfo,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = &gViridianCity_FishingMonsInfo,
	},
	{
		.mapGroup = MAP_GROUP(ROUTE_2),
		.mapNum = MAP_NUM(ROUTE_2),
		.landMonsInfo = &gRoute2_LandMonsNightInfo,
		.waterMonsInfo = NULL,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = NULL,
	},
	{
		.mapGroup = MAP_GROUP(VIRIDIAN_FOREST),
		.mapNum = MAP_NUM(VIRIDIAN_FOREST),
		.landMonsInfo = &gViridianForest1_LandMonsNightInfo,
		.waterMonsInfo = NULL,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = NULL,
	},
	{
		.mapGroup = 43,
		.mapNum = 0,
		.landMonsInfo = &gViridianForest2_LandMonsNightInfo,
		.waterMonsInfo = NULL,
		.rockSmashMonsInfo = &gViridianForest_RockSmashInfo,
		.fishingMonsInfo = NULL,
	},
	{
		.mapGroup = 1,
		.mapNum = 36,
		.landMonsInfo = &gDiglettCave1_LandMonsNightInfo,
		.waterMonsInfo = NULL,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = NULL,
	},
	{
		.mapGroup = MAP_GROUP(PEWTER_CITY),
		.mapNum = MAP_NUM(PEWTER_CITY),
		.landMonsInfo = &gPewterCity_LandMonsNightInfo,
		.waterMonsInfo = NULL,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = NULL,
	},
	{
		.mapGroup = MAP_GROUP(ROUTE_3),
		.mapNum = MAP_NUM(ROUTE_3),
		.landMonsInfo = &gRoute3_LandMonsNightInfo,
		.waterMonsInfo = NULL,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = NULL,
	},
	{
		.mapGroup = MAP_GROUP(MT_MOON_1F),
		.mapNum = MAP_NUM(MT_MOON_1F),
		.landMonsInfo = &gMtMoon1_LandMonsNightInfo,
		.waterMonsInfo = NULL,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = NULL,
	},
	{
		.mapGroup = MAP_GROUP(MT_MOON_B1F),
		.mapNum = MAP_NUM(MT_MOON_B1F),
		.landMonsInfo = &gMtMoon2_LandMonsNightInfo,
		.waterMonsInfo = NULL,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = NULL,
	},
	{
		.mapGroup = MAP_GROUP(MT_MOON_B2F),
		.mapNum = MAP_NUM(MT_MOON_B2F),
		.landMonsInfo = &gMtMoon3_LandMonsNightInfo,
		.waterMonsInfo = NULL,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = NULL,
	},
	{
		.mapGroup = MAP_GROUP(ROUTE_4),
		.mapNum = MAP_NUM(ROUTE_4),
		.landMonsInfo = &gRoute4_LandMonsNightInfo,
		.waterMonsInfo = NULL,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = NULL,
	},
	{
		.mapGroup = 43,
		.mapNum = 17,
		.landMonsInfo = &gFlowerField_LandMonsInfo,
		.waterMonsInfo = NULL,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = NULL,
	},
	{
		.mapGroup = MAP_GROUP(ROUTE_24),
		.mapNum = MAP_NUM(ROUTE_24),
		.landMonsInfo = &gRoute24_LandMonsNightInfo,
		.waterMonsInfo = &gRoute2425_SurfMonsInfo,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = &gRoute2425_FishingMonsInfo,
	},
	{
		.mapGroup = MAP_GROUP(ROUTE_25),
		.mapNum = MAP_NUM(ROUTE_25),
		.landMonsInfo = &gRoute25_LandMonsNightInfo,
		.waterMonsInfo = &gRoute25_SurfMonsInfo,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = &gRoute25_FishingMonsInfo,
	},
	{
		.mapGroup = MAP_GROUP(ROUTE_5),
		.mapNum = MAP_NUM(ROUTE_5),
		.landMonsInfo = &gRoute5_LandMonsNightInfo,
		.waterMonsInfo = NULL,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = NULL,
	},
	{
		.mapGroup = MAP_GROUP(ROUTE_6),
		.mapNum = MAP_NUM(ROUTE_6),
		.landMonsInfo = &gRoute6_LandMonsNightInfo,
		.waterMonsInfo = &gRoute6_SurfMonsInfo,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = &gRoute6_FishingMonsInfo,
	},
	{
		.mapGroup = MAP_GROUP(ROUTE_11),
		.mapNum = MAP_NUM(ROUTE_11),
		.landMonsInfo = &gRoute11_LandMonsNightInfo,
		.waterMonsInfo = &gRoute11_SurfMonsInfo,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = &gRoute11_FishingMonsInfo,
	},
	{
		.mapGroup = 1,
		.mapNum = 37,
		.landMonsInfo = &gDiglettCave2_LandMonsNightInfo,
		.waterMonsInfo = NULL,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = NULL,
	},
	{
		.mapGroup = MAP_GROUP(VERMILION_CITY),
		.mapNum = MAP_NUM(VERMILION_CITY),
		.landMonsInfo = NULL,
		.waterMonsInfo = &gVermillionCity_SurfMonsInfo,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = &gVermillionCity_FishingMonsInfo,
	},
	{
		.mapGroup = MAP_GROUP(ROUTE_9),
		.mapNum = MAP_NUM(ROUTE_9),
		.landMonsInfo = &gRoute9_LandMonsNightInfo,
		.waterMonsInfo = NULL,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = NULL,
	},
	{
		.mapGroup = MAP_GROUP(ROUTE_10),
		.mapNum = MAP_NUM(ROUTE_10),
		.landMonsInfo = &gRoute10_LandMonsNightInfo,
		.waterMonsInfo = &gRoute10_SurfMonsInfo,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = &gRoute10_FishingMonsInfo,
	},
	{
		.mapGroup = MAP_GROUP(ROCK_TUNNEL_1F),
		.mapNum = MAP_NUM(ROCK_TUNNEL_1F),
		.landMonsInfo = &gRockTunnel1_LandMonsNightInfo,
		.waterMonsInfo = NULL,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = NULL,
	},
	{
		.mapGroup = MAP_GROUP(ROCK_TUNNEL_B1F),
		.mapNum = MAP_NUM(ROCK_TUNNEL_B1F),
		.landMonsInfo = &gRockTunnel2_LandMonsNightInfo,
		.waterMonsInfo = NULL,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = NULL,
	},
	{
		.mapGroup = MAP_GROUP(ROUTE_8),
		.mapNum = MAP_NUM(ROUTE_8),
		.landMonsInfo = &gRoute8_LandMonsNightInfo,
		.waterMonsInfo = NULL,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = NULL,
	},
	{
		.mapGroup = MAP_GROUP(ROUTE_7),
		.mapNum = MAP_NUM(ROUTE_7),
		.landMonsInfo = &gRoute7_LandMonsNightInfo,
		.waterMonsInfo = NULL,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = NULL,
	},
	{
		.mapGroup = MAP_GROUP(POKEMON_TOWER_3F),
		.mapNum = MAP_NUM(POKEMON_TOWER_3F),
		.landMonsInfo = &gPokemonTower1_LandMonsDayInfo,
		.waterMonsInfo = NULL,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = NULL,
	},
	{
		.mapGroup = MAP_GROUP(POKEMON_TOWER_4F),
		.mapNum = MAP_NUM(POKEMON_TOWER_4F),
		.landMonsInfo = &gPokemonTower2_LandMonsDayInfo,
		.waterMonsInfo = NULL,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = NULL,
	},
	{
		.mapGroup = MAP_GROUP(POKEMON_TOWER_5F),
		.mapNum = MAP_NUM(POKEMON_TOWER_5F),
		.landMonsInfo = &gPokemonTower3_LandMonsDayInfo,
		.waterMonsInfo = NULL,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = NULL,
	},
	{
		.mapGroup = MAP_GROUP(POKEMON_TOWER_6F),
		.mapNum = MAP_NUM(POKEMON_TOWER_6F),
		.landMonsInfo = &gPokemonTower4_LandMonsDayInfo,
		.waterMonsInfo = NULL,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = NULL,
	},
	{
		.mapGroup = MAP_GROUP(POKEMON_TOWER_7F),
		.mapNum = MAP_NUM(POKEMON_TOWER_7F),
		.landMonsInfo = &gPokemonTower4_LandMonsDayInfo,
		.waterMonsInfo = NULL,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = NULL,
	},
	{
		.mapGroup = MAP_GROUP(ROUTE_16),
		.mapNum = MAP_NUM(ROUTE_16),
		.landMonsInfo = &gRoute16_LandMonsNightInfo,
		.waterMonsInfo = NULL,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = NULL,
	},
	{
		.mapGroup = MAP_GROUP(ROUTE_17),
		.mapNum = MAP_NUM(ROUTE_17),
		.landMonsInfo = &gRoute17_LandMonsNightInfo,
		.waterMonsInfo = NULL,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = NULL,
	},
	{
		.mapGroup = MAP_GROUP(ROUTE_18),
		.mapNum = MAP_NUM(ROUTE_18),
		.landMonsInfo = &gRoute18_LandMonsNightInfo,
		.waterMonsInfo = NULL,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = NULL,
	},
	{
		.mapGroup = MAP_GROUP(ROUTE_12),
		.mapNum = MAP_NUM(ROUTE_12),
		.landMonsInfo = &gRoute12_LandMonsNightInfo,
		.waterMonsInfo = &gRoute12_SurfMonsInfo,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = &gRoute12_FishingMonsInfo,
	},
	{
		.mapGroup = MAP_GROUP(ROUTE_13),
		.mapNum = MAP_NUM(ROUTE_13),
		.landMonsInfo = &gRoute12_LandMonsNightInfo,
		.waterMonsInfo = &gRoute12_SurfMonsInfo,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = &gRoute12_FishingMonsInfo,
	},
	{
		.mapGroup = MAP_GROUP(ROUTE_14),
		.mapNum = MAP_NUM(ROUTE_14),
		.landMonsInfo = &gRoute14_LandMonsNightInfo,
		.waterMonsInfo = NULL,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = NULL,
	},
	{
		.mapGroup = MAP_GROUP(ROUTE_15),
		.mapNum = MAP_NUM(ROUTE_15),
		.landMonsInfo = &gRoute14_LandMonsNightInfo,
		.waterMonsInfo = NULL,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = NULL,
	},
	{
		.mapGroup = 1,
		.mapNum = 63,
		.landMonsInfo = &gSafariZone1_LandMonsNightInfo,
		.waterMonsInfo = &gSafariZone_SurfMonsInfo,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = &gSafariZone_FishingMonsInfo,
	},
	{
		.mapGroup = 1,
		.mapNum = 64,
		.landMonsInfo = &gSafariZone2_LandMonsNightInfo,
		.waterMonsInfo = &gSafariZone_SurfMonsInfo,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = &gSafariZone_FishingMonsInfo,
	},
	{
		.mapGroup = 1,
		.mapNum = 65,
		.landMonsInfo = &gSafariZone3_LandMonsNightInfo,
		.waterMonsInfo = &gSafariZone_SurfMonsInfo,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = &gSafariZone_FishingMonsInfo,
	},
	{
		.mapGroup = 1,
		.mapNum = 66,
		.landMonsInfo = &gSafariZone4_LandMonsNightInfo,
		.waterMonsInfo = &gSafariZone_SurfMonsInfo,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = &gSafariZone_FishingMonsInfo,
	},
	{
		.mapGroup = MAP_GROUP(ROUTE_19),
		.mapNum = MAP_NUM(ROUTE_19),
		.landMonsInfo = NULL,
		.waterMonsInfo = &gRoute19_SurfMonsInfo,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = &gRoute19_FishingMonsInfo,
	},
	{
		.mapGroup = MAP_GROUP(ROUTE_20),
		.mapNum = MAP_NUM(ROUTE_20),
		.landMonsInfo = NULL,
		.waterMonsInfo = &gRoute20_SurfMonsInfo,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = &gRoute19_FishingMonsInfo,
	},
	{
		.mapGroup = MAP_GROUP(ROUTE_21_A),
		.mapNum = MAP_NUM(ROUTE_21_A),
		.landMonsInfo = &gRoute21_LandMonsNightInfo,
		.waterMonsInfo = &gRoute21_SurfMonsInfo,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = &gRoute19_FishingMonsInfo,
	},
	{
		.mapGroup = MAP_GROUP(ROUTE_21_B),
		.mapNum = MAP_NUM(ROUTE_21_B),
		.landMonsInfo = NULL,
		.waterMonsInfo = &gRoute21_SurfMonsInfo,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = &gRoute19_FishingMonsInfo,
	},
	{
		.mapGroup = MAP_GROUP(POKEMON_MANSION_1F),
		.mapNum = MAP_NUM(POKEMON_MANSION_1F),
		.landMonsInfo = &gMansion1_LandMonsNightInfo,
		.waterMonsInfo = NULL,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = NULL,
	},
	{
		.mapGroup = MAP_GROUP(POKEMON_MANSION_2F),
		.mapNum = MAP_NUM(POKEMON_MANSION_2F),
		.landMonsInfo = &gMansion2_LandMonsNightInfo,
		.waterMonsInfo = NULL,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = NULL,
	},
	{
		.mapGroup = MAP_GROUP(POKEMON_MANSION_3F),
		.mapNum = MAP_NUM(POKEMON_MANSION_3F),
		.landMonsInfo = &gMansion3_LandMonsNightInfo,
		.waterMonsInfo = NULL,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = NULL,
	},
	{
		.mapGroup = MAP_GROUP(POKEMON_MANSION_B1F),
		.mapNum = MAP_NUM(POKEMON_MANSION_B1F),
		.landMonsInfo = &gMansion3_LandMonsNightInfo,
		.waterMonsInfo = NULL,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = NULL,
	},
	{
		.mapGroup = MAP_GROUP(SEAFOAM_ISLANDS_1F),
		.mapNum = MAP_NUM(SEAFOAM_ISLANDS_1F),
		.landMonsInfo = &gSeafomIsland_LandMonsNightInfo,
		.waterMonsInfo = &gSeafoamIslands_SurfMonsInfo,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = &gSeafoamIslands_FishingMonsInfo,
	},
	{
		.mapGroup = MAP_GROUP(SEAFOAM_ISLANDS_B1F),
		.mapNum = MAP_NUM(SEAFOAM_ISLANDS_B1F),
		.landMonsInfo = &gSeafomIsland_LandMonsNightInfo,
		.waterMonsInfo = &gSeafoamIslands_SurfMonsInfo,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = &gSeafoamIslands_FishingMonsInfo,
	},
	{
		.mapGroup = MAP_GROUP(SEAFOAM_ISLANDS_B2F),
		.mapNum = MAP_NUM(SEAFOAM_ISLANDS_B2F),
		.landMonsInfo = &gSeafomIsland_LandMonsNightInfo,
		.waterMonsInfo = &gSeafoamIslands_SurfMonsInfo,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = &gSeafoamIslands_FishingMonsInfo,
	},
	{
		.mapGroup = MAP_GROUP(SEAFOAM_ISLANDS_B3F),
		.mapNum = MAP_NUM(SEAFOAM_ISLANDS_B3F),
		.landMonsInfo = &gSeafomIsland_LandMonsNightInfo,
		.waterMonsInfo = &gSeafoamIslands_SurfMonsInfo,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = &gSeafoamIslands_FishingMonsInfo,
	},
	{
		.mapGroup = MAP_GROUP(SEAFOAM_ISLANDS_B4F),
		.mapNum = MAP_NUM(SEAFOAM_ISLANDS_B4F),
		.landMonsInfo = &gSeafomIsland_LandMonsNightInfo,
		.waterMonsInfo = &gSeafoamIslands_SurfMonsInfo,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = &gSeafoamIslands_FishingMonsInfo,
	},
	{
		.mapGroup = MAP_GROUP(POWER_PLANT),
		.mapNum = MAP_NUM(POWER_PLANT),
		.landMonsInfo = &gPowerPlant_LandMonsNightInfo,
		.waterMonsInfo = NULL,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = NULL,
	},
	{
		.mapGroup = MAP_GROUP(ROUTE_23),
		.mapNum = MAP_NUM(ROUTE_23),
		.landMonsInfo = &gRoute23_LandMonsNightInfo,
		.waterMonsInfo = &gRoute23_SurfMonsInfo,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = &gSeafoamIslands_FishingMonsInfo,
	},
	{
		.mapGroup = 43,
		.mapNum = 16,
		.landMonsInfo = &gBillsGarden_LandMonsDayInfo,
		.waterMonsInfo = &gBillsGarden_SurfMonsInfo,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = &gBillsGarden_FishingMonsInfo,
	},
	{
		.mapGroup = MAP_GROUP(VICTORY_ROAD_1F),
		.mapNum = MAP_NUM(VICTORY_ROAD_1F),
		.landMonsInfo = &gVictoryRoad1_LandMonsNightInfo,
		.waterMonsInfo = NULL,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = NULL,
	},
	{
		.mapGroup = MAP_GROUP(VICTORY_ROAD_2F),
		.mapNum = MAP_NUM(VICTORY_ROAD_2F),
		.landMonsInfo = &gVictoryRoad2_LandMonsNightInfo,
		.waterMonsInfo = NULL,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = NULL,
	},
	{
		.mapGroup = MAP_GROUP(VICTORY_ROAD_3F),
		.mapNum = MAP_NUM(VICTORY_ROAD_3F),
		.landMonsInfo = &gVictoryRoad3_LandMonsNightInfo,
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
	},
};

/*const struct WildPokemonHeader gWildMonNightHeaders[] =
{
	{
		.mapGroup = 0xFF,
		.mapNum = 0xFF,
		.landMonsInfo = NULL,
		.waterMonsInfo = NULL,
		.rockSmashMonsInfo = NULL,
		.fishingMonsInfo = NULL,
	}
};*/

const struct SwarmData gSwarmTable[] =
{
	/*{
		.mapName = 0xFF,
		.species = 0xFFFF,
	},*/
};


const u16 gSwarmTableLength = NELEMS(gSwarmTable);
