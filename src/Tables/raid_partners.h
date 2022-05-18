#include "../config.h"
#include "../../include/constants/species.h"
#include "../../include/constants/items.h"
#include "../../include/constants/moves.h"

#ifdef UNBOUND

static const struct BattleTowerSpread sRaidPartnerSpread_Catherine_Rank12[] =
{
	{
		.species = SPECIES_SHELLOS_EAST,
		.nature = NATURE_BOLD,
		.hpIv = 31,
		.atkIv = 0,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.hpEv = 252,
		.defEv = 252,
		.spdEv = 4,
		.ability = FRONTIER_ABILITY_2, //ABILITY_STORMDRAIN
		.item = ITEM_EVIOLITE,
		.moves =
		{
			MOVE_SCALD,
			MOVE_CLEARSMOG,
			MOVE_MUDBOMB,
			MOVE_RECOVER,
		},
		.ball = BALL_TYPE_DIVE_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_ALOMOMOLA,
		.nature = NATURE_IMPISH,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 0,
		.spDefIv = 31,
		.spdIv = 31,
		.hpEv = 252,
		.defEv = 252,
		.spDefEv = 4,
		.ability = FRONTIER_ABILITY_1, //ABILITY_HEALER
		.item = ITEM_LEFTOVERS,
		.moves =
		{
			MOVE_AQUAJET,
			MOVE_MIRRORCOAT,
			MOVE_WISH,
			MOVE_HEALPULSE,
		},
		.ball = BALL_TYPE_DIVE_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_WARTORTLE,
		.nature = NATURE_ADAMANT,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 0,
		.spDefIv = 31,
		.spdIv = 31,
		.hpEv = 252,
		.atkEv = 252,
		.spDefEv = 4,
		.ability = FRONTIER_ABILITY_HIDDEN, //ABILITY_RAINDISH
		.item = ITEM_IAPAPA_BERRY,
		.moves =
		{
			MOVE_AQUATAIL,
			MOVE_ICEPUNCH,
			MOVE_POWERUPPUNCH,
			MOVE_RAINDANCE,
		},
		.ball = BALL_TYPE_DIVE_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
};

static const struct BattleTowerSpread sRaidPartnerSpread_Catherine_Rank3[] =
{
	{
		.species = SPECIES_GASTRODON_EAST,
		.nature = NATURE_BOLD,
		.hpIv = 31,
		.atkIv = 0,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.hpEv = 252,
		.defEv = 252,
		.spAtkEv = 4,
		.ability = FRONTIER_ABILITY_2, //ABILITY_STORMDRAIN
		.item = ITEM_WIKI_BERRY,
		.moves =
		{
			MOVE_SCALD,
			MOVE_MUDBOMB,
			MOVE_CLEARSMOG,
			MOVE_RECOVER,
		},
		.ball = BALL_TYPE_DIVE_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_ALOMOMOLA,
		.nature = NATURE_IMPISH,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 0,
		.spDefIv = 31,
		.spdIv = 31,
		.hpEv = 252,
		.defEv = 252,
		.ability = FRONTIER_ABILITY_1, //ABILITY_HEALER
		.item = ITEM_LEFTOVERS,
		.moves =
		{
			MOVE_AQUAJET,
			MOVE_MIRRORCOAT,
			MOVE_WISH,
			MOVE_HEALPULSE,
		},
		.ball = BALL_TYPE_DIVE_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_BLASTOISE,
		.nature = NATURE_ADAMANT,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 0,
		.spDefIv = 31,
		.spdIv = 31,
		.hpEv = 252,
		.atkEv = 252,
		.spDefEv = 4,
		.ability = FRONTIER_ABILITY_HIDDEN, //ABILITY_RAINDISH
		.item = ITEM_DAMP_ROCK,
		.moves =
		{
			MOVE_AQUATAIL,
			MOVE_ICEPUNCH,
			MOVE_POWERUPPUNCH,
			MOVE_RAINDANCE,
		},
		.ball = BALL_TYPE_DIVE_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
};

static const struct BattleTowerSpread sRaidPartnerSpread_Catherine_Rank456[] =
{
	{
		.species = SPECIES_GASTRODON_EAST,
		.nature = NATURE_BOLD,
		.hpIv = 31,
		.atkIv = 0,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.hpEv = 252,
		.defEv = 252,
		.spAtkEv = 4,
		.ability = FRONTIER_ABILITY_2, //ABILITY_STORMDRAIN
		.item = ITEM_WIKI_BERRY,
		.moves =
		{
			MOVE_SCALD,
			MOVE_EARTHPOWER,
			MOVE_CLEARSMOG,
			MOVE_RECOVER,
		},
		.ball = BALL_TYPE_DIVE_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_ALOMOMOLA,
		.nature = NATURE_IMPISH,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 0,
		.spDefIv = 31,
		.spdIv = 31,
		.hpEv = 252,
		.defEv = 252,
		.ability = FRONTIER_ABILITY_1, //ABILITY_HEALER
		.item = ITEM_LEFTOVERS,
		.moves =
		{
			MOVE_AQUAJET,
			MOVE_MIRRORCOAT,
			MOVE_WISH,
			MOVE_HEALPULSE,
		},
		.ball = BALL_TYPE_DIVE_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_BLASTOISE,
		.nature = NATURE_MODEST,
		.hpIv = 31,
		.atkIv = 0,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.hpEv = 252,
		.spAtkEv = 252,
		.spDefEv = 4,
		.ability = FRONTIER_ABILITY_HIDDEN, //ABILITY_RAINDISH
		.item = ITEM_BLASTOISINITE,
		.moves =
		{
			MOVE_WATERPULSE,
			MOVE_AURASPHERE,
			MOVE_DARKPULSE,
			MOVE_FLASHCANNON,
		},
		.ball = BALL_TYPE_DIVE_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
};

static const struct BattleTowerSpread sRaidPartnerSpread_Ginger_Rank123[] =
{
	{
		.species = SPECIES_VIVILLON,
		.nature = NATURE_TIMID,
		.hpIv = 31,
		.atkIv = 0,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.hpEv = 252,
		.defEv = 4,
		.spdEv = 252,
		.ability = FRONTIER_ABILITY_HIDDEN, //ABILITY_FRIENDGUARD
		.item = ITEM_BINDING_BAND,
		.moves =
		{
			MOVE_INFESTATION,
			MOVE_ELECTROWEB,
			MOVE_LIGHTSCREEN,
			MOVE_PROTECT,
		},
		.ball = BALL_TYPE_NET_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_CLEFAIRY,
		.nature = NATURE_CALM,
		.hpIv = 31,
		.atkIv = 0,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.hpEv = 252,
		.defEv = 4,
		.spDefEv = 252,
		.ability = FRONTIER_ABILITY_HIDDEN, //ABILITY_FRIENDGUARD
		.item = ITEM_EVIOLITE,
		.moves =
		{
			MOVE_MOONBLAST,
			MOVE_HELPINGHAND,
			MOVE_FOLLOWME,
			MOVE_LIFEDEW,
		},
		.ball = BALL_TYPE_MOON_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_AUDINO,
		.nature = NATURE_CALM,
		.hpIv = 31,
		.atkIv = 0,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.hpEv = 252,
		.spAtkEv = 4,
		.spDefEv = 252,
		.ability = FRONTIER_ABILITY_1, //ABILITY_HEALER
		.item = ITEM_LEFTOVERS,
		.moves =
		{
			MOVE_DAZZLINGGLEAM,
			MOVE_FLAMETHROWER,
			MOVE_HEALPULSE,
			MOVE_WISH,
		},
		.ball = BALL_TYPE_PREMIER_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
};

static const struct BattleTowerSpread sRaidPartnerSpread_Ginger_Rank456[] =
{
	{
		.species = SPECIES_VIVILLON,
		.nature = NATURE_TIMID,
		.hpIv = 31,
		.atkIv = 0,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.hpEv = 252,
		.defEv = 4,
		.spdEv = 252,
		.ability = FRONTIER_ABILITY_HIDDEN, //ABILITY_FRIENDGUARD
		.item = ITEM_BINDING_BAND,
		.moves =
		{
			MOVE_INFESTATION,
			MOVE_ELECTROWEB,
			MOVE_LIGHTSCREEN,
			MOVE_PROTECT,
		},
		.ball = BALL_TYPE_NET_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_CLEFABLE,
		.nature = NATURE_CALM,
		.hpIv = 31,
		.atkIv = 0,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.hpEv = 252,
		.spAtkEv = 4,
		.spDefEv = 252,
		.ability = FRONTIER_ABILITY_2, //ABILITY_MAGICGUARD
		.item = ITEM_LEFTOVERS,
		.moves =
		{
			MOVE_MOONBLAST,
			MOVE_FOLLOWME,
			MOVE_MAGICCOAT,
			MOVE_LIFEDEW,
		},
		.ball = BALL_TYPE_MOON_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_AUDINO,
		.nature = NATURE_CALM,
		.hpIv = 31,
		.atkIv = 0,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.hpEv = 252,
		.defEv = 4,
		.spDefEv = 252,
		.ability = FRONTIER_ABILITY_1, //ABILITY_HEALER
		.item = ITEM_AUDINITE,
		.moves =
		{
			MOVE_DAZZLINGGLEAM,
			MOVE_HELPINGHAND,
			MOVE_HEALPULSE,
			MOVE_WISH,
		},
		.ball = BALL_TYPE_PREMIER_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
};

static const struct BattleTowerSpread sRaidPartnerSpread_Abimbola_Rank23[] =
{
	{
		.species = SPECIES_FOONGUS,
		.nature = NATURE_CALM,
		.hpIv = 31,
		.atkIv = 0,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.hpEv = 252,
		.spAtkEv = 4,
		.spDefEv = 252,
		.ability = FRONTIER_ABILITY_HIDDEN, //ABILITY_REGENERATOR
		.item = ITEM_LEFTOVERS,
		.moves =
		{
			MOVE_GIGADRAIN,
			MOVE_VENOSHOCK,
			MOVE_CLEARSMOG,
			MOVE_TOXIC,
		},
		.ball = BALL_TYPE_GREAT_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_AXEW,
		.nature = NATURE_ADAMANT,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 0,
		.spDefIv = 31,
		.spdIv = 31,
		.hpEv = 252,
		.atkEv = 252,
		.spDefEv = 4,
		.ability = FRONTIER_ABILITY_2, //ABILITY_MOLDBREAKER
		.item = ITEM_EXPERT_BELT,
		.moves =
		{
			MOVE_DRAGONCLAW,
			MOVE_AQUATAIL,
			MOVE_POISONJAB,
			MOVE_XSCISSOR,
		},
		.ball = BALL_TYPE_DUSK_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_ONIX,
		.nature = NATURE_CAREFUL,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 0,
		.spDefIv = 31,
		.spdIv = 31,
		.atkEv = 252,
		.spDefEv = 252,
		.ability = FRONTIER_ABILITY_2, //ABILITY_STURDY
		.item = ITEM_EVIOLITE,
		.moves =
		{
			MOVE_ROCKSLIDE,
			MOVE_STOMPINGTANTRUM,
			MOVE_IRONHEAD,
			MOVE_BIND,
		},
		.ball = BALL_TYPE_DUSK_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
};

static const struct BattleTowerSpread sRaidPartnerSpread_Abimbola_Rank456[] =
{
	{
		.species = SPECIES_AMOONGUSS,
		.nature = NATURE_CALM,
		.hpIv = 31,
		.atkIv = 0,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.hpEv = 252,
		.spAtkEv = 4,
		.spDefEv = 252,
		.ability = FRONTIER_ABILITY_HIDDEN, //ABILITY_REGENERATOR
		.item = ITEM_LEFTOVERS,
		.moves =
		{
			MOVE_GIGADRAIN,
			MOVE_SLUDGEBOMB,
			MOVE_RAGEPOWDER,
			MOVE_SPORE,
		},
		.ball = BALL_TYPE_GREAT_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_HAXORUS,
		.nature = NATURE_ADAMANT,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 0,
		.spDefIv = 31,
		.spdIv = 31,
		.atkEv = 252,
		.defEv = 252,
		.spdEv = 4,
		.ability = FRONTIER_ABILITY_2, //ABILITY_MOLDBREAKER
		.item = ITEM_DRAGONIUM_Z,
		.moves =
		{
			MOVE_DRAGONCLAW,
			MOVE_AQUATAIL,
			MOVE_POISONJAB,
			MOVE_SWORDSDANCE,
		},
		.ball = BALL_TYPE_DUSK_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_STEELIX,
		.nature = NATURE_BRAVE,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 0,
		.spDefIv = 31,
		.spdIv = 0,
		.hpEv = 252,
		.atkEv = 252,
		.spDefEv = 4,
		.ability = FRONTIER_ABILITY_2, //ABILITY_STURDY
		.item = ITEM_STEELIXITE,
		.moves =
		{
			MOVE_GYROBALL,
			MOVE_STOMPINGTANTRUM,
			MOVE_AQUATAIL,
			MOVE_FIREFANG,
		},
		.ball = BALL_TYPE_DUSK_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
};

static const struct BattleTowerSpread sRaidPartnerSpread_Mahina_Rank456[] =
{
	{
		.species = SPECIES_GYARADOS,
		.nature = NATURE_CAREFUL,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 0,
		.spDefIv = 31,
		.spdIv = 31,
		.hpEv = 252,
		.atkEv = 4,
		.spDefEv = 252,
		.ability = FRONTIER_ABILITY_1, //ABILITY_INTIMIDATE
		.item = ITEM_WATERIUM_Z,
		.moves =
		{
			MOVE_AQUATAIL,
			MOVE_CRUNCH,
			MOVE_IRONHEAD,
			MOVE_POWERWHIP,
		},
		.ball = BALL_TYPE_LURE_BALL,
		.shiny = TRUE,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_VOLCARONA,
		.nature = NATURE_MODEST,
		.hpIv = 31,
		.atkIv = 0,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.defEv = 4,
		.spAtkEv = 252,
		.spDefEv = 252,
		.ability = FRONTIER_ABILITY_1, //ABILITY_FLAMEBODY
		.item = ITEM_LEFTOVERS,
		.moves =
		{
			MOVE_BUGBUZZ,
			MOVE_FIERYDANCE,
			MOVE_LIGHTSCREEN,
			MOVE_ROOST,
		},
		.ball = BALL_TYPE_LUXURY_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_VENUSAUR,
		.nature = NATURE_BOLD,
		.hpIv = 31,
		.atkIv = 0,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.hpEv = 252,
		.defEv = 252,
		.spAtkEv = 4,
		.ability = FRONTIER_ABILITY_HIDDEN, //ABILITY_CHLOROPHYLL
		.item = ITEM_VENUSAURITE,
		.moves =
		{
			MOVE_GRASSPLEDGE,
			MOVE_SLUDGEBOMB,
			MOVE_WEATHERBALL,
			MOVE_LEECHSEED,
		},
		.ball = BALL_TYPE_POKE_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
};

static const struct BattleTowerSpread sRaidPartnerSpread_Alford_Rank456[] =
{
	{
		.species = SPECIES_EMPOLEON,
		.nature = NATURE_BOLD,
		.hpIv = 31,
		.atkIv = 0,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.hpEv = 252,
		.spAtkEv = 4,
		.spDefEv = 252,
		.ability = FRONTIER_ABILITY_1, //ABILITY_TORRENT
		.item = ITEM_LEFTOVERS,
		.moves =
		{
			MOVE_WATERPLEDGE,
			MOVE_FLASHCANNON,
			MOVE_ICEBEAM,
			MOVE_SWAGGER,
		},
		.ball = BALL_TYPE_CHERISH_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_AEGISLASH,
		.nature = NATURE_ADAMANT,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 0,
		.spDefIv = 31,
		.spdIv = 0,
		.hpEv = 252,
		.atkEv = 252,
		.spDefEv = 4,
		.ability = FRONTIER_ABILITY_1, //ABILITY_STANCECHANGE
		.item = ITEM_FOCUS_SASH,
		.moves =
		{
			MOVE_IRONHEAD,
			MOVE_SHADOWCLAW,
			MOVE_SACREDSWORD,
			MOVE_KINGSSHIELD,
		},
		.ball = BALL_TYPE_CHERISH_BALL,
		.shiny = TRUE,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_GALLADE,
		.nature = NATURE_ADAMANT,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 0,
		.spDefIv = 31,
		.spdIv = 31,
		.hpEv = 252,
		.spDefEv = 252,
		.spdEv = 4,
		.ability = FRONTIER_ABILITY_HIDDEN, //ABILITY_JUSTIFIED
		.item = ITEM_GALLADITE,
		.moves =
		{
			MOVE_PSYCHOCUT,
			MOVE_DRAINPUNCH,
			MOVE_POISONJAB,
			MOVE_POWERUPPUNCH,
		},
		.ball = BALL_TYPE_CHERISH_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
};

static const struct BattleTowerSpread sRaidPartnerSpread_Rival_Rank56[] =
{
	{
		.species = SPECIES_GARCHOMP,
		.nature = NATURE_ADAMANT,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 0,
		.spDefIv = 31,
		.spdIv = 31,
		.hpEv = 252,
		.atkEv = 252,
		.spDefEv = 4,
		.ability = FRONTIER_ABILITY_HIDDEN, //ABILITY_ROUGHSKIN
		.item = ITEM_GARCHOMPITE,
		.moves =
		{
			MOVE_DRAGONCLAW,
			MOVE_STOMPINGTANTRUM,
			MOVE_POISONJAB,
			MOVE_CRUNCH,
		},
		.ball = BALL_TYPE_POKE_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_MAMOSWINE,
		.nature = NATURE_ADAMANT,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 0,
		.spDefIv = 31,
		.spdIv = 31,
		.hpEv = 196,
		.atkEv = 252,
		.spdEv = 60,
		.ability = FRONTIER_ABILITY_HIDDEN, //ABILITY_THICKFAT
		.item = ITEM_FOCUS_SASH,
		.moves =
		{
			MOVE_ICICLESPEAR,
			MOVE_STOMPINGTANTRUM,
			MOVE_STONEEDGE,
			MOVE_ICESHARD,
		},
		.ball = BALL_TYPE_POKE_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_GRANBULL,
		.nature = NATURE_ADAMANT,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 0,
		.spDefIv = 31,
		.spdIv = 31,
		.hpEv = 252,
		.atkEv = 252,
		.spDefEv = 4,
		.ability = FRONTIER_ABILITY_1, //ABILITY_INTIMIDATE
		.item = ITEM_ASSAULT_VEST,
		.moves =
		{
			MOVE_PLAYROUGH,
			MOVE_POWERUPPUNCH,
			MOVE_ICEPUNCH,
			MOVE_THUNDERPUNCH,
		},
		.ball = BALL_TYPE_POKE_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_TYRANITAR,
		.nature = NATURE_ADAMANT,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 0,
		.spDefIv = 31,
		.spdIv = 31,
		.hpEv = 252,
		.atkEv = 252,
		.spDefEv = 4,
		.ability = FRONTIER_ABILITY_1, //ABILITY_SANDSTREAM
		.item = ITEM_TYRANITARITE,
		.moves =
		{
			MOVE_STONEEDGE,
			MOVE_CRUNCH,
			MOVE_POWERUPPUNCH,
			MOVE_FIREPUNCH,
		},
		.ball = BALL_TYPE_POKE_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_METAGROSS,
		.nature = NATURE_ADAMANT,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 0,
		.spDefIv = 31,
		.spdIv = 31,
		.atkEv = 252,
		.spDefEv = 252,
		.spdEv = 4,
		.ability = FRONTIER_ABILITY_1, //ABILITY_CLEARBODY
		.item = ITEM_METAGROSSITE,
		.moves =
		{
			MOVE_IRONHEAD,
			MOVE_ZENHEADBUTT,
			MOVE_POWERUPPUNCH,
			MOVE_ICEPUNCH,
		},
		.ball = BALL_TYPE_POKE_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
};

static const struct BattleTowerSpread sRaidPartnerSpread_Marlon_Rank56[] =
{
	{
		.species = SPECIES_MACHAMP,
		.nature = NATURE_ADAMANT,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 0,
		.spDefIv = 31,
		.spdIv = 0,
		.hpEv = 252,
		.atkEv = 4,
		.spDefEv = 252,
		.ability = FRONTIER_ABILITY_1, //ABILITY_GUTS
		.item = ITEM_SITRUS_BERRY,
		.moves =
		{
			MOVE_BRICKBREAK,
			MOVE_ICEPUNCH,
			MOVE_BULLETPUNCH,
			MOVE_WIDEGUARD,
		},
		.ball = BALL_TYPE_DUSK_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
		.gigantamax = TRUE,
	},
	{
		.species = SPECIES_KROOKODILE,
		.nature = NATURE_ADAMANT,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 0,
		.spDefIv = 31,
		.spdIv = 31,
		.hpEv = 252,
		.atkEv = 252,
		.spDefEv = 4,
		.ability = FRONTIER_ABILITY_1, //ABILITY_INTIMIDATE
		.item = ITEM_ASSAULT_VEST,
		.moves =
		{
			MOVE_STOMPINGTANTRUM,
			MOVE_DARKESTLARIAT,
			MOVE_AQUATAIL,
			MOVE_THUNDERFANG,
		},
		.ball = BALL_TYPE_DUSK_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_ZAPDOS,
		.nature = NATURE_MODEST,
		.hpIv = 31,
		.atkIv = 0,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.hpEv = 252,
		.spAtkEv = 252,
		.spDefEv = 4,
		.ability = FRONTIER_ABILITY_HIDDEN, //ABILITY_STATIC
		.item = ITEM_ELECTRIUM_Z,
		.moves =
		{
			MOVE_THUNDERBOLT,
			MOVE_HEATWAVE,
			MOVE_OMINOUSWIND,
			MOVE_ROOST,
		},
		.ball = BALL_TYPE_DUSK_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
};

static const struct BattleTowerSpread sRaidPartnerSpread_Jax_Rank6[] =
{
	{
		.species = SPECIES_GOLURK,
		.nature = NATURE_ADAMANT,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 0,
		.spDefIv = 31,
		.spdIv = 31,
		.hpEv = 252,
		.atkEv = 252,
		.spDefEv = 4,
		.ability = FRONTIER_ABILITY_1, //ABILITY_IRONFIST
		.item = ITEM_GROUNDIUM_Z,
		.moves =
		{
			MOVE_HIGHHORSEPOWER,
			MOVE_PHANTOMFORCE,
			MOVE_POWERUPPUNCH,
			MOVE_GRAVITY,
		},
		.ball = BALL_TYPE_CHERISH_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_MAGNEZONE,
		.nature = NATURE_MODEST,
		.hpIv = 31,
		.atkIv = 0,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.hpEv = 252,
		.spAtkEv = 252,
		.ability = FRONTIER_ABILITY_2, //ABILITY_STURDY
		.item = ITEM_AIR_BALLOON,
		.moves =
		{
			MOVE_THUNDERBOLT,
			MOVE_FLASHCANNON,
			MOVE_SIGNALBEAM,
			MOVE_MIRRORCOAT,
		},
		.ball = BALL_TYPE_CHERISH_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_SALAMENCE,
		.nature = NATURE_ADAMANT,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 0,
		.spDefIv = 31,
		.spdIv = 31,
		.atkEv = 252,
		.defEv = 252,
		.spdEv = 4,
		.ability = FRONTIER_ABILITY_1, //ABILITY_INTIMIDATE
		.item = ITEM_SALAMENCITE,
		.moves =
		{
			MOVE_DRAGONCLAW,
			MOVE_DOUBLEEDGE,
			MOVE_AQUATAIL,
			MOVE_FIREFANG,
		},
		.ball = BALL_TYPE_CHERISH_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
};

static const struct BattleTowerSpread sRaidPartnerSpread_Red_Rank6[] =
{
	{
		.species = SPECIES_PIKACHU_CAP_ORIGINAL,
		.nature = NATURE_MODEST,
		.hpIv = 31,
		.atkIv = 0,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.spAtkEv = 252,
		.spDefEv = 4,
		.spdEv = 252,
		.ability = FRONTIER_ABILITY_HIDDEN, //ABILITY_LIGHTNINGROD
		.item = ITEM_PIKASHUNIUM_Z,
		.moves =
		{
			MOVE_THUNDERBOLT,
			MOVE_GRASSKNOT,
			MOVE_VOLTSWITCH,
			MOVE_REFLECT,
		},
		.ball = BALL_TYPE_POKE_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_SNORLAX,
		.nature = NATURE_ADAMANT,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 0,
		.spDefIv = 31,
		.spdIv = 31,
		.hpEv = 252,
		.atkEv = 252,
		.spDefEv = 4,
		.ability = FRONTIER_ABILITY_HIDDEN, //ABILITY_GLUTTONY
		.item = ITEM_AGUAV_BERRY,
		.moves =
		{
			MOVE_BODYSLAM,
			MOVE_CRUNCH,
			MOVE_HIGHHORSEPOWER,
			MOVE_RECYCLE,
		},
		.ball = BALL_TYPE_MASTER_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
		.gigantamax = TRUE,
	},
	{
		.species = SPECIES_MEWTWO,
		.nature = NATURE_ADAMANT,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 0,
		.spDefIv = 31,
		.spdIv = 31,
		.hpEv = 252,
		.atkEv = 252,
		.ability = FRONTIER_ABILITY_1, //ABILITY_PRESSURE
		.item = ITEM_MEWTWONITE_X,
		.moves =
		{
			MOVE_ZENHEADBUTT,
			MOVE_POWERUPPUNCH,
			MOVE_POISONJAB,
			MOVE_RECOVER,
		},
		.ball = BALL_TYPE_MASTER_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
};

extern const u8 sTrainerName_Catherine[];
extern const u8 sTrainerName_Ginger[];
extern const u8 sTrainerName_Abimbola[];
extern const u8 sTrainerName_Mahina[];
extern const u8 sTrainerName_Alford[];
extern const u8 sTrainerName_Marlon[];
extern const u8 sTrainerName_Jax[];
extern const u8 sTrainerName_Red[];

const struct MultiRaidTrainer gRaidPartners[] =
{
	{
		.owNum = EVENT_OBJ_GFX_CATHERINE,
		.trainerClass = CLASS_PKMN_TRAINER_2,
		.backSpriteId = TRAINER_BACK_PIC_CATHERINE,
		.gender = FEMALE,
		.otId = 0xCD2F27AB,
		.name = sTrainerName_Catherine,
		.spreads =
		{
			[ONE_STAR_RAID ... TWO_STAR_RAID] =	 sRaidPartnerSpread_Catherine_Rank12,
			[THREE_STAR_RAID] =				 	 sRaidPartnerSpread_Catherine_Rank3,
			[FOUR_STAR_RAID ... SIX_STAR_RAID] =	 sRaidPartnerSpread_Catherine_Rank456,
		},
		.spreadSizes =
		{
			[ONE_STAR_RAID ... TWO_STAR_RAID] =   NELEMS(sRaidPartnerSpread_Catherine_Rank12),
			[THREE_STAR_RAID] = 				 NELEMS(sRaidPartnerSpread_Catherine_Rank3),
			[FOUR_STAR_RAID ... SIX_STAR_RAID] =  NELEMS(sRaidPartnerSpread_Catherine_Rank456),
		},
	},
	{
		.owNum = EVENT_OBJ_GFX_GINGER,
		.trainerClass = CLASS_PKMN_TRAINER_2,
		.backSpriteId = TRAINER_BACK_PIC_GINGER,
		.gender = FEMALE,
		.otId = 0xAD5C417B,
		.name = sTrainerName_Ginger,
		.spreads =
		{
			[ONE_STAR_RAID ... THREE_STAR_RAID] = sRaidPartnerSpread_Ginger_Rank123,
			[FOUR_STAR_RAID ... SIX_STAR_RAID] =  sRaidPartnerSpread_Ginger_Rank456,
		},
		.spreadSizes =
		{
			[ONE_STAR_RAID ... THREE_STAR_RAID] =  NELEMS(sRaidPartnerSpread_Ginger_Rank123),
			[FOUR_STAR_RAID ... SIX_STAR_RAID] = NELEMS(sRaidPartnerSpread_Ginger_Rank456),
		},
	},
	{
		.owNum = EVENT_OBJ_GFX_ABIMBOLA,
		.trainerClass = CLASS_PKMN_TRAINER_2,
		.backSpriteId = TRAINER_BACK_PIC_ABIMBOLA,
		.gender = MALE,
		.otId = 0x2EC3AEBC,
		.name = sTrainerName_Abimbola,
		.spreads =
		{
			[TWO_STAR_RAID ... THREE_STAR_RAID] = sRaidPartnerSpread_Abimbola_Rank23,
			[FOUR_STAR_RAID ... SIX_STAR_RAID] =  sRaidPartnerSpread_Abimbola_Rank456,
		},
		.spreadSizes =
		{
			[TWO_STAR_RAID ... THREE_STAR_RAID] =  NELEMS(sRaidPartnerSpread_Abimbola_Rank23),
			[FOUR_STAR_RAID ... SIX_STAR_RAID] = NELEMS(sRaidPartnerSpread_Abimbola_Rank456),
		},
	},
	{
		.owNum = EVENT_OBJ_GFX_MAHINA,
		.trainerClass = CLASS_PKMN_TRAINER_2,
		.backSpriteId = TRAINER_BACK_PIC_MAHINA,
		.gender = FEMALE,
		.otId = 0x87116209,
		.name = sTrainerName_Mahina,
		.spreads =
		{
			[FOUR_STAR_RAID ... SIX_STAR_RAID] =  sRaidPartnerSpread_Mahina_Rank456,
		},
		.spreadSizes =
		{
			[FOUR_STAR_RAID ... SIX_STAR_RAID] = NELEMS(sRaidPartnerSpread_Mahina_Rank456),
		},
	},
	{
		.owNum = EVENT_OBJ_GFX_ALFORD,
		.trainerClass = CLASS_PKMN_TRAINER_2,
		.backSpriteId = TRAINER_BACK_PIC_ALFORD,
		.gender = MALE,
		.otId = 0x61AC8B90,
		.name = sTrainerName_Alford,
		.spreads =
		{
			[FOUR_STAR_RAID ... SIX_STAR_RAID] =  sRaidPartnerSpread_Alford_Rank456,
		},
		.spreadSizes =
		{
			[FOUR_STAR_RAID ... SIX_STAR_RAID] = NELEMS(sRaidPartnerSpread_Alford_Rank456),
		},
	},
	{
		.owNum = EVENT_OBJ_GFX_RIVAL,
		.trainerClass = CLASS_RIVAL,
		.backSpriteId = TRAINER_BACK_PIC_RIVAL,
		.gender = MALE,
		.otId = 0xECBAA1AD,
		.name = NULL, //Predefined ingame
		.spreads =
		{
			[FIVE_STAR_RAID ... SIX_STAR_RAID] =  sRaidPartnerSpread_Rival_Rank56,
		},
		.spreadSizes =
		{
			[FIVE_STAR_RAID ... SIX_STAR_RAID] = NELEMS(sRaidPartnerSpread_Rival_Rank56),
		},
	},
	{
		.owNum = EVENT_OBJ_GFX_MARLON,
		.trainerClass = CLASS_SHADOW_ADMIN,
		.backSpriteId = TRAINER_BACK_PIC_MARLON,
		.gender = MALE,
		.otId = 0x4156010,
		.name = sTrainerName_Marlon,
		.spreads =
		{
			[FIVE_STAR_RAID ... SIX_STAR_RAID] =  sRaidPartnerSpread_Marlon_Rank56,
		},
		.spreadSizes =
		{
			[FIVE_STAR_RAID ... SIX_STAR_RAID] = NELEMS(sRaidPartnerSpread_Marlon_Rank56),
		},
	},
	{
		.owNum = EVENT_OBJ_GFX_JAX,
		.trainerClass = CLASS_PKMN_TRAINER_2,//CLASS_CHAMPION,
		.backSpriteId = TRAINER_BACK_PIC_JAX,
		.gender = MALE,
		.otId = 0x95746426,
		.name = sTrainerName_Jax,
		.spreads =
		{
			[SIX_STAR_RAID] =  sRaidPartnerSpread_Jax_Rank6,
		},
		.spreadSizes =
		{
			[SIX_STAR_RAID] = NELEMS(sRaidPartnerSpread_Jax_Rank6),
		},
	},
	{
		.owNum = EVENT_OBJ_GFX_RED,
		.trainerClass = CLASS_PKMN_TRAINER_2,
		.backSpriteId = TRAINER_BACK_PIC_RED_NEW,
		.gender = MALE,
		.otId = 0x95746426,
		.name = sTrainerName_Red,
		.spreads =
		{
			[SIX_STAR_RAID] =  sRaidPartnerSpread_Red_Rank6,
		},
		.spreadSizes =
		{
			[SIX_STAR_RAID] = NELEMS(sRaidPartnerSpread_Red_Rank6),
		},
	},
};

#else

static const struct BattleTowerSpread sRaidPartnerSpread_Catherine_Rank2[] =
{
	/*{
		.species = SPECIES_SHELLOS_EAST,
		.nature = NATURE_BOLD,
		.hpIv = 31,
		.atkIv = 0,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.hpEv = 252,
		.defEv = 252,
		.spdEv = 4,
		.ability = FRONTIER_ABILITY_2, //ABILITY_STORMDRAIN
		.item = ITEM_EVIOLITE,
		.moves =
		{
			MOVE_SCALD,
			MOVE_CLEARSMOG,
			MOVE_MUDBOMB,
			MOVE_RECOVER,
		},
		.ball = BALL_TYPE_DIVE_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},*/
	{
		.species = SPECIES_YANMA,
		.nature = NATURE_TIMID,
		.hpIv = 31,
		.atkIv = 0,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.spAtkEv = 252,
		.defEv = 4,
		.spdEv = 252,
		.ability = FRONTIER_ABILITY_HIDDEN,
		.item = ITEM_FOCUS_SASH,
		.moves =
		{
			MOVE_BUGBUZZ,
			MOVE_AIRSLASH,
			MOVE_PROTECT,
			MOVE_HYPNOSIS,
		},
		.ball = BALL_TYPE_POKE_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_SWIRLIX,
		.nature = NATURE_MODEST,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.defEv = 104,
		.spAtkEv = 252,
		.spdEv = 152,
		.ability = FRONTIER_ABILITY_HIDDEN,
		.item = ITEM_GRASSY_SEED,
		.moves =
		{
			MOVE_CALMMIND,
			MOVE_DRAININGKISS,
			MOVE_FLAMETHROWER,
			MOVE_THUNDERBOLT,
		},
		.ball = BALL_TYPE_GREAT_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_MAGBY,
		.nature = NATURE_JOLLY,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.atkEv = 252,
		.spDefEv = 4,
		.spdEv = 252,
		.ability = FRONTIER_ABILITY_2,
		.item = ITEM_SITRUS_BERRY,
		.moves =
		{
			MOVE_BELLYDRUM,
			MOVE_FIREPUNCH,
			MOVE_MACHPUNCH,
			MOVE_THUNDERPUNCH,
		},
		.ball = BALL_TYPE_GREAT_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	}
};

static const struct BattleTowerSpread sRaidPartnerSpread_Catherine_Rank5[] =
{
	{
		.species = SPECIES_YANMEGA,
		.nature = NATURE_TIMID,
		.hpIv = 31,
		.atkIv = 0,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.spAtkEv = 252,
		.spDefEv = 4,
		.spdEv = 252,
		.ability = FRONTIER_ABILITY_2,
		.item = ITEM_CHOICE_SPECS,
		.moves = 
		{
			MOVE_BUGBUZZ,
			MOVE_AIRSLASH,
			MOVE_UTURN,
			MOVE_GIGADRAIN,
		},
		.ball = BALL_TYPE_GREAT_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_SLURPUFF,
		.nature = NATURE_ADAMANT,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 0,
		.spDefIv = 31,
		.spdIv = 31,
		.atkEv = 252,
		.hpEv = 4,
		.spdEv = 252,
		.ability = FRONTIER_ABILITY_2,
		.item = ITEM_SITRUS_BERRY,
		.moves =
		{
			MOVE_BELLYDRUM,
			MOVE_RETURN,
			MOVE_PLAYROUGH,
			MOVE_DRAINPUNCH,
		},
		.ball = BALL_TYPE_GREAT_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_MAGMORTAR,
		.nature = NATURE_MODEST,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.spAtkEv = 252,
		.spDefEv = 4,
		.spdEv = 252,
		.ability = FRONTIER_ABILITY_2,
		.item = ITEM_SITRUS_BERRY,
		.moves =
		{
			MOVE_FIREBLAST,
			MOVE_FOCUSBLAST,
			MOVE_THUNDERBOLT,
			MOVE_TAUNT,
		},
		.ball = BALL_TYPE_GREAT_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	}
};

static const struct BattleTowerSpread sRaidPartnerSpread_Catherine_Rank6[] =
{
	{
		.species = SPECIES_SILVALLY_BUG,
		.nature = NATURE_JOLLY,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.atkEv = 252,
		.spDefEv = 4,
		.spdEv = 252,
		.ability = FRONTIER_ABILITY_1,
		.item = ITEM_BUG_MEMORY,
		.moves =
		{
			MOVE_MULTIATTACK,
			MOVE_PARTINGSHOT,
			MOVE_DEFOG,
			MOVE_FIREFANG,
		},
		.ball = BALL_TYPE_ULTRA_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_TAPU_FINI,
		.nature = NATURE_TIMID,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.hpEv = 248,
		.spDefEv = 44,
		.spdEv = 216,
		.ability = FRONTIER_ABILITY_1,
		.item = ITEM_CHOICE_SCARF,
		.moves =
		{
			MOVE_SCALD,
			MOVE_MOONBLAST,
			MOVE_TRICK,
			MOVE_TAUNT,
		},
		.ball = BALL_TYPE_ULTRA_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_RESHIRAM,
		.nature = NATURE_MODEST,
		.hpIv = 31,
		.atkIv = 0,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.hpEv = 4,
		.spAtkEv = 252,
		.spdEv = 252,
		.ability = FRONTIER_ABILITY_1, //ABILITY_TURBOBLAZE
		.item = ITEM_WHITE_HERB,
		.moves =
		{
			MOVE_DRACOMETEOR,
			MOVE_BLUEFLARE,
			MOVE_EARTHPOWER,
			MOVE_FOCUSBLAST,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = FALSE,
		.modifyMovesDoubles = FALSE,
	},
};

static const struct BattleTowerSpread sRaidPartnerSpread_Tasha_Rank2[] =
{
	{
		.species = SPECIES_SNEASEL,
		.nature = NATURE_JOLLY,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.defEv = 4,
		.atkEv = 252,
		.spdEv = 252,
		.ability = FRONTIER_ABILITY_1, //ABILITY_PRESSURE
		.item = ITEM_CHOICE_BAND,
		.moves =
		{
			MOVE_BITE,
			MOVE_ICESHARD,
			MOVE_LOWKICK,
			MOVE_METALCLAW,
		},
		.ball = BALL_TYPE_POKE_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_MEOWTH_A,
		.nature = NATURE_HASTY,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.atkEv = 4,
		.spAtkEv = 252,
		.spdEv = 252,
		.ability = FRONTIER_ABILITY_HIDDEN, //ABILITY_RATTLED
		.item = ITEM_LEFTOVERS,
		.moves =
		{
			MOVE_DARKPULSE,
			MOVE_FACADE,
			MOVE_PROTECT,
			MOVE_NASTYPLOT,
		},
		.ball = BALL_TYPE_POKE_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_SNORUNT,
		.nature = NATURE_LONELY,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.hpEv = 252,
		.atkEv = 252,
		.spAtkEv = 4,
		.ability = FRONTIER_ABILITY_1, //ABILITY_INNERFOCUS
		.item = ITEM_LEFTOVERS,
		.moves =
		{
			MOVE_CRUNCH,
			MOVE_HEX,
			MOVE_ASTONISH,
			MOVE_ROLLOUT,
		},
		.ball = BALL_TYPE_GREAT_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
};

static const struct BattleTowerSpread sRaidPartnerSpread_Tasha_Rank5[] =
{
	{
		.species = SPECIES_WEAVILE,
		.nature = NATURE_LONELY,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.spAtkEv = 4,
		.atkEv = 252,
		.spdEv = 252,
		.ability = FRONTIER_ABILITY_1, //ABILITY_PRESSURE
		.item = ITEM_QUICK_CLAW,
		.moves =
		{
			MOVE_AGILITY,
			MOVE_DARKPULSE,
			MOVE_AERIALACE,
			MOVE_CUT,
		},
		.ball = BALL_TYPE_ULTRA_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_PERSIAN_A,
		.nature = NATURE_HASTY,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.atkEv = 4,
		.spAtkEv = 252,
		.spdEv = 252,
		.ability = FRONTIER_ABILITY_HIDDEN, //ABILITY_RATTLED
		.item = ITEM_LEFTOVERS,
		.moves =
		{
			MOVE_DARKPULSE,
			MOVE_FACADE,
			MOVE_PROTECT,
			MOVE_NASTYPLOT,
		},
		.ball = BALL_TYPE_POKE_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_GLALIE,
		.nature = NATURE_LONELY,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.hpEv = 252,
		.atkEv = 252,
		.spAtkEv = 4,
		.ability = FRONTIER_ABILITY_1, //ABILITY_INNERFOCUS
		.item = ITEM_LEFTOVERS,
		.moves =
		{
			MOVE_CRUNCH,
			MOVE_HEX,
			MOVE_ASTONISH,
			MOVE_ROLLOUT,
		},
		.ball = BALL_TYPE_GREAT_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
};

static const struct BattleTowerSpread sRaidPartnerSpread_Tasha_Rank6[] =
{
	{
		.species = SPECIES_REGICE,
		.nature = NATURE_BOLD,
		.hpIv = 31,
		.atkIv = 0,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.hpEv = 252,
		.defEv = 252,
		.spDefEv = 4,
		.ability = FRONTIER_ABILITY_1, //ABILITY_CLEARBODY
		.item = ITEM_LEFTOVERS,
		.moves =
		{
			MOVE_FROSTBREATH,
			MOVE_CHARGEBEAM,
			MOVE_AMNESIA,
			MOVE_PROTECT,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_DARKRAI,
		.nature = NATURE_TIMID,
		.hpIv = 31,
		.atkIv = 0,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.defEv = 4,
		.spAtkEv = 252,
		.spdEv = 252,
		.ability = FRONTIER_ABILITY_1, //ABILITY_BADDREAMS
		.item = ITEM_LIFE_ORB,
		.moves =
		{
			MOVE_DARKPULSE,
			MOVE_FOCUSBLAST,
			MOVE_SLUDGEBOMB,
			MOVE_NASTYPLOT,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_MARSHADOW,
		.nature = NATURE_NAIVE,
		.hpIv = 31,
		.atkIv = 30,
		.defIv = 30,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.atkEv = 252,
		.defEv = 4,
		.spdEv = 252,
		.ability = FRONTIER_ABILITY_1, //ABILITY_TECHNICIAN
		.item = ITEM_MARSHADIUM_Z,
		.moves =
		{
			MOVE_CLOSECOMBAT,
			MOVE_SPECTRALTHIEF,
			MOVE_SHADOWSNEAK,
			MOVE_HIDDENPOWER, //TYPE_ICE
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
};



static const struct BattleTowerSpread sRaidPartnerSpread_Red_Rank2[] =
{
	{
		.species = SPECIES_IVYSAUR,
		.nature = NATURE_BOLD,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.hpEv = 252,
		.defEv = 252,
		.atkEv = 4,
		.ability = FRONTIER_ABILITY_1, 
		.item = ITEM_EVIOLITE,
		.moves =
		{
			MOVE_LEECHSEED,
			MOVE_RAZORLEAF,
			MOVE_POISONPOWDER,
			MOVE_SLEEPPOWDER,
		},
		.ball = BALL_TYPE_POKE_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_WARTORTLE,
		.nature = NATURE_MODEST,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.spAtkEv = 252,
		.hpEv = 252,
		.spdEv = 4,
		.ability = FRONTIER_ABILITY_1,
		.item = ITEM_LEFTOVERS,
		.moves =
		{
			MOVE_WATERGUN,
			MOVE_RAINDANCE,
			MOVE_SUBSTITUTE,
			MOVE_AURASPHERE,
		},
		.ball = BALL_TYPE_DIVE_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_CHARMELEON,
		.nature = NATURE_MODEST,
		.hpIv = 31,
		.atkIv = 0,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.spdEv = 252,
		.spAtkEv = 252,
		.spDefEv = 4,
		.ability = FRONTIER_ABILITY_HIDDEN, //ABILITY_RAINDISH
		.item = ITEM_CHOICE_SCARF,
		.moves =
		{
			MOVE_FLAMETHROWER,
			MOVE_WILLOWISP,
			MOVE_DRAGONPULSE,
			MOVE_SWIFT,
		},
		.ball = BALL_TYPE_POKE_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
};

static const struct BattleTowerSpread sRaidPartnerSpread_Red_Rank5[] =
{
	{
		.species = SPECIES_VENUSAUR,
		.nature = NATURE_MODEST,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.hpEv = 252,
		.defEv = 252,
		.spAtkEv = 4,
		.ability = FRONTIER_ABILITY_1,
		.item = ITEM_LEFTOVERS,
		.moves =
		{
			MOVE_LEECHSEED,
			MOVE_TOXIC,
			MOVE_SLUDGEBOMB,
			MOVE_SOLARBEAM,
		},
		.ball = BALL_TYPE_POKE_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_BLASTOISE,
		.nature = NATURE_MODEST,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.spAtkEv = 252,
		.hpEv = 252,
		.spdEv = 4,
		.ability = FRONTIER_ABILITY_1,
		.item = ITEM_BLASTOISINITE,
		.moves =
		{
			MOVE_SURF,
			MOVE_SHELLSMASH,
			MOVE_SUBSTITUTE,
			MOVE_AURASPHERE,
		},
		.ball = BALL_TYPE_DIVE_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_CHARIZARD,
		.nature = NATURE_MODEST,
		.hpIv = 31,
		.atkIv = 0,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.spdEv = 252,
		.spAtkEv = 252,
		.spDefEv = 4,
		.ability = FRONTIER_ABILITY_HIDDEN, //ABILITY_RAINDISH
		.item = ITEM_CHOICE_SCARF,
		.moves =
		{
			MOVE_FLAMETHROWER,
			MOVE_WILLOWISP,
			MOVE_DRAGONPULSE,
			MOVE_WEATHERBALL,
		},
		.ball = BALL_TYPE_POKE_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
};

static const struct BattleTowerSpread sRaidPartnerSpread_Red_Rank6[] =
{
	{
		.species = SPECIES_RAIKOU,
		.nature = NATURE_TIMID,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.spAtkEv = 252,
		.spDefEv = 4,
		.spdEv = 252,
		.ability = FRONTIER_ABILITY_1,
		.item = ITEM_LEFTOVERS,
		.moves =
		{
			MOVE_VOLTSWITCH,
			MOVE_THUNDERBOLT,
			MOVE_SCALD,
			MOVE_TOXIC
		},
		.ball = BALL_TYPE_ULTRA_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_SUICUNE,
		.nature = NATURE_MODEST,
		.hpIv = 31,
		.atkIv = 0,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.hpEv = 252,
		.spAtkEv = 252,
		.spdEv = 4,
		.ability = FRONTIER_ABILITY_HIDDEN, //ABILITY_WATERABSORB
		.item = ITEM_SITRUS_BERRY,
		.moves =
		{
			MOVE_SURF,
			MOVE_ICEBEAM,
			MOVE_SIGNALBEAM,
			MOVE_MIRRORCOAT,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_ENTEI,
		.nature = NATURE_ADAMANT,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.atkEv = 252,
		.defEv = 4,
		.spdEv = 252,
		.ability = FRONTIER_ABILITY_1, 
		.item = ITEM_CHOICE_SCARF,
		.moves =
		{
			MOVE_SACREDFIRE,
			MOVE_EXTREMESPEED,
			MOVE_STONEEDGE,
			MOVE_FLAREBLITZ,
		},
		.ball = BALL_TYPE_ULTRA_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
};

static const struct BattleTowerSpread sRaidPartnerSpread_Ash_Rank2[] =
{
	{
		.species = SPECIES_PIKACHU,
		.nature = NATURE_MODEST,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.spAtkEv = 252,
		.spDefEv = 4,
		.spdEv = 252,
		.ability = FRONTIER_ABILITY_1,
		.item = ITEM_LIGHT_BALL,
		.moves =
		{
			MOVE_THUNDERSHOCK,
			MOVE_THUNDERWAVE,
			MOVE_FAKEOUT,
			MOVE_AGILITY,
		},
		.ball = BALL_TYPE_POKE_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_MR_MIME,
		.nature = NATURE_MODEST,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.spAtkEv = 252,
		.hpEv = 252,
		.defEv = 4,
		.ability = FRONTIER_ABILITY_1,
		.item = ITEM_LEFTOVERS,
		.moves =
		{
			MOVE_PSYBEAM,
			MOVE_DAZZLINGGLEAM,
			MOVE_REFLECT,
			MOVE_LIGHTSCREEN,
		},
		.ball = BALL_TYPE_ULTRA_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_FARFETCHD_G,
		.nature = NATURE_JOLLY,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.spdEv = 252,
		.atkEv = 252,
		.spDefEv = 4,
		.ability = FRONTIER_ABILITY_HIDDEN, 
		.item = ITEM_CHOICE_BAND,
		.moves =
		{
			MOVE_BRUTALSWING,
			MOVE_KNOCKOFF,
			MOVE_FURYCUTTER,
			MOVE_STEELWING,
		},
		.ball = BALL_TYPE_POKE_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
};

static const struct BattleTowerSpread sRaidPartnerSpread_Ash_Rank5[] =
{
	{
		.species = SPECIES_PIKACHU,
		.nature = NATURE_MODEST,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.spAtkEv = 252,
		.spDefEv = 4,
		.spdEv = 252,
		.ability = FRONTIER_ABILITY_1,
		.item = ITEM_LIGHT_BALL,
		.moves =
		{
			MOVE_THUNDERBOLT,
			MOVE_THUNDERWAVE,
			MOVE_SURF,
			MOVE_GRASSKNOT,
		},
		.ball = BALL_TYPE_POKE_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_GENGAR,
		.nature = NATURE_TIMID,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.spAtkEv = 252,
		.spdEv = 252,
		.hpEv = 4,
		.ability = FRONTIER_ABILITY_HIDDEN,
		.item = ITEM_CHOICE_SPECS,
		.moves =
		{
			MOVE_SHADOWBALL,
			MOVE_DAZZLINGGLEAM,
			MOVE_DARKPULSE,
			MOVE_SLUDGEWAVE,
		},
		.ball = BALL_TYPE_ULTRA_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
		.gigantamax = TRUE,
	},
	{
		.species = SPECIES_DRAGONITE,
		.nature = NATURE_JOLLY,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.spdEv = 252,
		.atkEv = 252,
		.spDefEv = 4,
		.ability = FRONTIER_ABILITY_HIDDEN,
		.item = ITEM_CHOICE_BAND,
		.moves =
		{
			MOVE_OUTRAGE,
			MOVE_EXTREMESPEED,
			MOVE_FIREPUNCH,
			MOVE_STEELWING,
		},
		.ball = BALL_TYPE_ULTRA_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
};

static const struct BattleTowerSpread sRaidPartnerSpread_Lyra_Rank2[] =
{
	{
		.species = SPECIES_MARILL,
		.nature = NATURE_ADAMANT,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.atkEv = 252,
		.spDefEv = 4,
		.spdEv = 252,
		.ability = FRONTIER_ABILITY_2,
		.item = ITEM_LIFE_ORB,
		.moves =
		{
			MOVE_AQUATAIL,
			MOVE_PLAYROUGH,
			MOVE_ROLLOUT,
			MOVE_SLAM,
		},
		.ball = BALL_TYPE_DIVE_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_PORYGON,
		.nature = NATURE_MODEST,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.spAtkEv = 252,
		.hpEv = 4,
		.spdEv = 252,
		.ability = FRONTIER_ABILITY_2,
		.item = ITEM_LEFTOVERS,
		.moves =
		{
			MOVE_TRIATTACK,
			MOVE_HYPERBEAM,
			MOVE_ICYWIND,
			MOVE_ELECTROWEB,
		},
		.ball = BALL_TYPE_ULTRA_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_TANGELA,
		.nature = NATURE_BOLD,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.defEv = 252,
		.spDefEv = 252,
		.spAtkEv = 4,
		.ability = FRONTIER_ABILITY_1,
		.item = ITEM_LEFTOVERS,
		.moves =
		{
			MOVE_GIGADRAIN,
			MOVE_LEECHSEED,
			MOVE_POISONPOWDER,
			MOVE_INGRAIN,
		},
		.ball = BALL_TYPE_POKE_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
};

static const struct BattleTowerSpread sRaidPartnerSpread_Lyra_Rank5[] =
{
	{
		.species = SPECIES_AZUMARILL,
		.nature = NATURE_ADAMANT,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.atkEv = 252,
		.spDefEv = 4,
		.spdEv = 252,
		.ability = FRONTIER_ABILITY_2,
		.item = ITEM_SITRUS_BERRY,
		.moves =
		{
			MOVE_BELLYDRUM,
			MOVE_PLAYROUGH,
			MOVE_AQUAJET,
			MOVE_SUPERPOWER,
		},
		.ball = BALL_TYPE_DIVE_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_PORYGON2,
		.nature = NATURE_MODEST,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.spAtkEv = 252,
		.hpEv = 4,
		.spdEv = 252,
		.ability = FRONTIER_ABILITY_2,
		.item = ITEM_LIFE_ORB,
		.moves =
		{
			MOVE_TRIATTACK,
			MOVE_SHADOWBALL,
			MOVE_ICEBEAM,
			MOVE_THUNDERBOLT,
		},
		.ball = BALL_TYPE_ULTRA_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_TANGROWTH,
		.nature = NATURE_BOLD,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.defEv = 252,
		.spDefEv = 252,
		.spAtkEv = 4,
		.ability = FRONTIER_ABILITY_1,
		.item = ITEM_LEFTOVERS,
		.moves =
		{
			MOVE_GIGADRAIN,
			MOVE_LEECHSEED,
			MOVE_SLUDGEBOMB,
			MOVE_INGRAIN,
		},
		.ball = BALL_TYPE_ULTRA_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
};

static const struct BattleTowerSpread sRaidPartnerSpread_Fox_Rank2[] =
{
	{
		.species = SPECIES_PORYGON,
		.nature = NATURE_MODEST,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 0,
		.spAtkEv = 252,
		.spDefEv = 252,
		.hpEv = 4,
		.ability = FRONTIER_ABILITY_1,
		.item = ITEM_LIFE_ORB,
		.moves =
		{
			MOVE_TRICKROOM,
			MOVE_SHADOWBALL,
			MOVE_ELECTROWEB,
			MOVE_ICYWIND,
		},
		.ball = BALL_TYPE_ULTRA_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_HATTREM,
		.nature = NATURE_MODEST,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 0,
		.spAtkEv = 252,
		.spDefEv = 252,
		.defEv = 4,
		.ability = FRONTIER_ABILITY_2,
		.item = ITEM_LEFTOVERS,
		.moves =
		{
			MOVE_DAZZLINGGLEAM,
			MOVE_CONFUSION,
			MOVE_MYSTICALFIRE,
			MOVE_DRAININGKISS,
		},
		.ball = BALL_TYPE_ULTRA_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_LITWICK,
		.nature = NATURE_MODEST,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 0,
		.spAtkEv = 252,
		.defEv = 252,
		.hpEv = 4,
		.ability = FRONTIER_ABILITY_2,
		.item = ITEM_LEFTOVERS,
		.moves =
		{
			MOVE_MYSTICALFIRE,
			MOVE_SHADOWBALL,
			MOVE_DARKPULSE,
			MOVE_TRICKROOM,
		},
		.ball = BALL_TYPE_ULTRA_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
};

static const struct BattleTowerSpread sRaidPartnerSpread_Fox_Rank5[] =
{
	{
		.species = SPECIES_PORYGON2,
		.nature = NATURE_MODEST,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 0,
		.spAtkEv = 252,
		.spDefEv = 252,
		.hpEv = 4,
		.ability = FRONTIER_ABILITY_1,
		.item = ITEM_EVIOLITE,
		.moves =
		{
			MOVE_TRICKROOM,
			MOVE_TRIATTACK,
			MOVE_THUNDERBOLT,
			MOVE_ICEBEAM,
		},
		.ball = BALL_TYPE_ULTRA_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_HATTERENE,
		.nature = NATURE_MODEST,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 0,
		.spAtkEv = 252,
		.spDefEv = 252,
		.defEv = 4,
		.ability = FRONTIER_ABILITY_2,
		.item = ITEM_LIFE_ORB,
		.moves =
		{
			MOVE_DAZZLINGGLEAM,
			MOVE_PSYCHIC,
			MOVE_MYSTICALFIRE,
			MOVE_GIGADRAIN,
		},
		.ball = BALL_TYPE_ULTRA_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_CHANDELURE,
		.nature = NATURE_MODEST,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 0,
		.spAtkEv = 252,
		.defEv = 252,
		.hpEv = 4,
		.ability = FRONTIER_ABILITY_2,
		.item = ITEM_LEFTOVERS,
		.moves =
		{
			MOVE_FLAMETHROWER,
			MOVE_SHADOWBALL,
			MOVE_DARKPULSE,
			MOVE_TRICKROOM,
		},
		.ball = BALL_TYPE_ULTRA_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
};

static const struct BattleTowerSpread sRaidPartnerSpread_Fox_Rank6[] =
{
	{
		.species = SPECIES_CRESSELIA,
		.nature = NATURE_RELAXED,
		.hpIv = 31,
		.atkIv = 0,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 0,
		.hpEv = 248,
		.defEv = 252,
		.spDefEv = 8,
		.ability = FRONTIER_ABILITY_1, //ABILITY_LEVITATE
		.item = ITEM_ROOM_SERVICE,
		.moves =
		{
			MOVE_PSYCHIC,
			MOVE_ICEBEAM,
			MOVE_TRICKROOM,
			MOVE_MOONLIGHT,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_MELMETAL,
		.nature = NATURE_ADAMANT,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 0,
		.spDefIv = 31,
		.spdIv = 0,
		.hpEv = 252,
		.atkEv = 252,
		.spDefEv = 4,
		.ability = FRONTIER_ABILITY_1, //ABILITY_IRONFIST
		.item = ITEM_EXPERT_BELT,
		.moves =
		{
			MOVE_DOUBLEIRONBASH,
			MOVE_ICEPUNCH,
			MOVE_EARTHQUAKE,
			MOVE_THUNDERPUNCH,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
		.gigantamax = TRUE,
	},
	{
		.species = SPECIES_GLASTRIER,
		.nature = NATURE_ADAMANT,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 0,
		.spAtkEv = 252,
		.defEv = 252,
		.hpEv = 4,
		.ability = FRONTIER_ABILITY_1,
		.item = ITEM_HEAVY_DUTY_BOOTS,
		.moves =
		{
			MOVE_ICICLECRASH,
			MOVE_HIGHHORSEPOWER,
			MOVE_CLOSECOMBAT,
			MOVE_DOUBLEEDGE,
		},
		.ball = BALL_TYPE_ULTRA_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
};

static const struct BattleTowerSpread sRaidPartnerSpread_Talia_Rank2[] =
{
	{
		.species = SPECIES_WINGULL,
		.nature = NATURE_HASTY,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.atkEv = 4,
		.spAtkEv = 252,
		.spdEv = 252,
		.ability = FRONTIER_ABILITY_2,
		.item = ITEM_EVIOLITE,
		.moves =
		{
			MOVE_WATERPULSE,
			MOVE_AIRCUTTER,
			MOVE_RAINDANCE,
			MOVE_STEELWING,
		},
		.ball = BALL_TYPE_POKE_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_ARROKUDA,
		.nature = NATURE_ADAMANT,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.atkEv = 252,
		.spDefEv = 4,
		.spdEv = 252,
		.ability = FRONTIER_ABILITY_1,
		.item = ITEM_CHOICE_BAND,
		.moves =
		{
			MOVE_LIQUIDATION,
			MOVE_POISONJAB,
			MOVE_DRILLRUN,
			MOVE_ICEFANG
		},
		.ball = BALL_TYPE_GREAT_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_LOMBRE,
		.nature = NATURE_BOLD,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.hpEv = 252,
		.defEv = 252,
		.spAtkEv = 4,
		.ability = FRONTIER_ABILITY_2,
		.item = ITEM_LEFTOVERS,
		.moves =
		{
			MOVE_SCALD,
			MOVE_GIGADRAIN,
			MOVE_ICEBEAM,
			MOVE_LEECHSEED,
		},
		.ball = BALL_TYPE_GREAT_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
};

static const struct BattleTowerSpread sRaidPartnerSpread_Talia_Rank5[] =
{
	{
		.species = SPECIES_PELIPPER,
		.nature = NATURE_MODEST,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.hpEv = 248,
		.spAtkEv = 252,
		.spDefEv = 8,
		.ability = FRONTIER_ABILITY_2,
		.item = ITEM_WIKI_BERRY,
		.moves =
		{
			MOVE_SCALD,
			MOVE_HURRICANE,
			MOVE_ICEBEAM,
			MOVE_DEFOG,
		},
		.ball = BALL_TYPE_POKE_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_BARRASKEWDA,
		.nature = NATURE_ADAMANT,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.atkEv = 252,
		.spDefEv = 4,
		.spdEv = 252,
		.ability = FRONTIER_ABILITY_1,
		.item = ITEM_CHOICE_BAND,
		.moves =
		{
			MOVE_FLIPTURN,
			MOVE_POISONJAB,
			MOVE_CRUNCH,
			MOVE_ICEFANG
		},
		.ball = BALL_TYPE_GREAT_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_LUDICOLO,
		.nature = NATURE_BOLD,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.hpEv = 252,
		.defEv = 252,
		.spAtkEv = 4,
		.ability = FRONTIER_ABILITY_2,
		.item = ITEM_LEFTOVERS,
		.moves =
		{
			MOVE_ENERGYBALL,
			MOVE_HYDROPUMP,
			MOVE_ICEBEAM,
			MOVE_FAKEOUT,
		},
		.ball = BALL_TYPE_GREAT_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
};

static const struct BattleTowerSpread sRaidPartnerSpread_Talia_Rank6[] =
{
	{
		.species = SPECIES_KYOGRE,
		.nature = NATURE_MODEST,
		.hpIv = 31,
		.atkIv = 0,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.defEv = 4,
		.spAtkEv = 252,
		.spdEv = 252,
		.ability = FRONTIER_ABILITY_1, //ABILITY_DRIZZLE
		.item = ITEM_BLUE_ORB,
		.moves =
		{
			MOVE_ORIGINPULSE,
			MOVE_THUNDER,
			MOVE_ICEBEAM,
			MOVE_REST,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_KELDEO_RESOLUTE,
		.nature = NATURE_TIMID,
		.hpIv = 31,
		.atkIv = 0,
		.defIv = 31,
		.spAtkIv = 30,
		.spDefIv = 31,
		.spdIv = 31,
		.spAtkEv = 252,
		.spDefEv = 4,
		.spdEv = 252,
		.ability = FRONTIER_ABILITY_1, //ABILITY_JUSTIFIED
		.item = ITEM_CHOICE_SPECS,
		.moves =
		{
			MOVE_HYDROPUMP,
			MOVE_SECRETSWORD,
			MOVE_ICYWIND,
			MOVE_HIDDENPOWER, //TYPE_GRASS
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_VOLCANION,
		.nature = NATURE_MODEST,
		.hpIv = 31,
		.atkIv = 0,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.defEv = 4,
		.spAtkEv = 252,
		.spdEv = 252,
		.ability = FRONTIER_ABILITY_1, //ABILITY_WATERABSORB
		.item = ITEM_CHOICE_SPECS,
		.moves =
		{
			MOVE_FIREBLAST,
			MOVE_STEAMERUPTION,
			MOVE_SLUDGEWAVE,
			MOVE_EARTHPOWER,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
};

static const struct BattleTowerSpread sRaidPartnerSpread_Hailey_Rank2[] =
{
	{
		.species = SPECIES_SNOVER,
		.nature = NATURE_QUIET,
		.hpIv = 31,
		.atkIv = 30,
		.defIv = 31,
		.spAtkIv = 30,
		.spDefIv = 30,
		.spdIv = 31,
		.hpEv = 36,
		.atkEv = 104,
		.defEv = 40,
		.spAtkEv = 184,
		.spDefEv = 120,
		.spdEv = 24,
		.ability = FRONTIER_ABILITY_1, //ABILITY_SNOWWARNING
		.item = ITEM_EVIOLITE,
		.moves =
		{
			MOVE_RAZORLEAF,
			MOVE_ICYWIND,
			MOVE_ICESHARD,
			MOVE_HIDDENPOWER, //TYPE_GROUND
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_CUBCHOO,
		.nature = NATURE_NAIVE,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.hpEv = 76,
		.atkEv = 196,
		.defEv = 4,
		.spAtkEv = 36,
		.spdEv = 196,
		.ability = FRONTIER_ABILITY_2, //ABILITY_SLUSHRUSH
		.item = ITEM_NEVER_MELT_ICE,
		.moves =
		{
			MOVE_ICEPUNCH,
			MOVE_ROCKTOMB,
			MOVE_SHADOWCLAW,
			MOVE_BRINE,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_VULPIX_A,
		.nature = NATURE_TIMID,
		.hpIv = 31,
		.atkIv = 0,
		.defIv = 31,
		.spAtkIv = 30,
		.spDefIv = 30,
		.spdIv = 31,
		.hpEv = 52,
		.spAtkEv = 200,
		.spDefEv = 16,
		.spdEv = 240,
		.ability = FRONTIER_ABILITY_HIDDEN, //ABILITY_SNOWWARNING
		.item = ITEM_LIGHT_CLAY,
		.moves =
		{
			MOVE_ICYWIND,
			MOVE_DRAININGKISS,
			MOVE_HIDDENPOWER, //TYPE_GROUND
			MOVE_AURORAVEIL,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
};

static const struct BattleTowerSpread sRaidPartnerSpread_Hailey_Rank5[] =
{
	{
		.species = SPECIES_ABOMASNOW,
		.nature = NATURE_MILD,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.atkEv = 4,
		.spAtkEv = 252,
		.spdEv = 252,
		.ability = FRONTIER_ABILITY_1, //ABILITY_SNOWWARNING
		.item = ITEM_CHOICE_SCARF,
		.moves =
		{
			MOVE_BLIZZARD,
			MOVE_LEAFSTORM,
			MOVE_EARTHQUAKE,
			MOVE_ICESHARD,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_BEARTIC,
		.nature = NATURE_JOLLY,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 0,
		.spDefIv = 31,
		.spdIv = 31,
		.atkEv = 252,
		.spDefEv = 4,
		.spdEv = 252,
		.ability = FRONTIER_ABILITY_HIDDEN, //ABILITY_SWIFTSWIM
		.item = ITEM_ICIUM_Z,
		.moves =
		{
			MOVE_ICICLECRASH,
			MOVE_SUPERPOWER,
			MOVE_THROATCHOP,
			MOVE_SWORDSDANCE,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = FALSE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_NINETALES_A,
		.nature = NATURE_TIMID,
		.hpIv = 31,
		.atkIv = 0,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.spAtkEv = 252,
		.spDefEv = 4,
		.spdEv = 252,
		.ability = FRONTIER_ABILITY_HIDDEN, //ABILITY_SNOWWARNING
		.item = ITEM_ICIUM_Z,
		.moves =
		{
			MOVE_BLIZZARD,
			MOVE_MOONBLAST,
			MOVE_PSYSHOCK,
			MOVE_AURORAVEIL,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
};

static const struct BattleTowerSpread sRaidPartnerSpread_Hailey_Rank6[] =
{
	{
		.species = SPECIES_KYUREM_BLACK,
		.nature = NATURE_HASTY,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.atkEv = 112,
		.spAtkEv = 144,
		.spdEv = 252,
		.ability = FRONTIER_ABILITY_1, //ABILITY_TERAVOLT
		.item = ITEM_LIFE_ORB,
		.moves =
		{
			MOVE_ICICLESPEAR,
			MOVE_FUSIONBOLT,
			MOVE_EARTHPOWER,
			MOVE_PROTECT,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = FALSE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_KYUREM_WHITE,
		.nature = NATURE_MODEST,
		.hpIv = 31,
		.atkIv = 0,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.defEv = 4,
		.spAtkEv = 252,
		.spdEv = 252,
		.ability = FRONTIER_ABILITY_1, //ABILITY_TURBOBLAZE
		.item = ITEM_CHOICE_SCARF,
		.moves =
		{
			MOVE_BLIZZARD,
			MOVE_DRACOMETEOR,
			MOVE_ICEBEAM,
			MOVE_FUSIONFLARE,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = FALSE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_ARTICUNO,
		.nature = NATURE_TIMID,
		.hpIv = 31,
		.atkIv = 0,
		.defIv = 30,
		.spAtkIv = 30,
		.spDefIv = 30,
		.spdIv = 30,
		.spAtkEv = 252,
		.spDefEv = 4,
		.spdEv = 252,
		.ability = FRONTIER_ABILITY_HIDDEN, //ABILITY_SNOWCLOAK
		.item = ITEM_FLYINIUM_Z,
		.moves =
		{
			MOVE_FREEZEDRY,
			MOVE_HURRICANE,
			MOVE_HIDDENPOWER, //TYPE_FIGHTING
			MOVE_ROOST,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
};

static const struct BattleTowerSpread sRaidPartnerSpread_Juniper_Rank2[] =
{
	{
		.species = SPECIES_SNIVY,
		.nature = NATURE_TIMID,
		.hpIv = 31,
		.atkIv = 0,
		.defIv = 30,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.defEv = 16,
		.spAtkEv = 240,
		.spdEv = 252,
		.ability = FRONTIER_ABILITY_HIDDEN, //ABILITY_CONTRARY
		.item = ITEM_EVIOLITE,
		.moves =
		{
			MOVE_LEAFSTORM,
			MOVE_HIDDENPOWER, //TYPE_ICE
			MOVE_GLARE,
			MOVE_SYNTHESIS,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_TEPIG,
		.nature = NATURE_JOLLY,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 0,
		.spDefIv = 31,
		.spdIv = 31,
		.atkEv = 252,
		.defEv = 20,
		.spdEv = 236,
		.ability = FRONTIER_ABILITY_1, //ABILITY_BLAZE
		.item = ITEM_LIFE_ORB,
		.moves =
		{
			MOVE_FLAREBLITZ,
			MOVE_HEADSMASH,
			MOVE_SUCKERPUNCH,
			MOVE_WILDCHARGE,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_OSHAWOTT,
		.nature = NATURE_RASH,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.atkEv = 20,
		.spAtkEv = 252,
		.spdEv = 236,
		.ability = FRONTIER_ABILITY_1, //ABILITY_TORRENT
		.item = ITEM_LIFE_ORB,
		.moves =
		{
			MOVE_HYDROPUMP,
			MOVE_ICEBEAM,
			MOVE_AIRSLASH,
			MOVE_AQUAJET,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
};

static const struct BattleTowerSpread sRaidPartnerSpread_Juniper_Rank5[] =
{
	{
		.species = SPECIES_SERPERIOR,
		.nature = NATURE_TIMID,
		.hpIv = 31,
		.atkIv = 0,
		.defIv = 31,
		.spAtkIv = 30,
		.spDefIv = 31,
		.spdIv = 30,
		.spAtkEv = 252,
		.spDefEv = 4,
		.spdEv = 252,
		.ability = FRONTIER_ABILITY_HIDDEN, //ABILITY_CONTRARY
		.item = ITEM_LIFE_ORB,
		.moves =
		{
			MOVE_LEAFSTORM,
			MOVE_GIGADRAIN,
			MOVE_DRAGONPULSE,
			MOVE_HIDDENPOWER, //TYPE_FIRE
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = FALSE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_EMBOAR,
		.nature = NATURE_ADAMANT,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 0,
		.spDefIv = 31,
		.spdIv = 31,
		.atkEv = 252,
		.defEv = 4,
		.spdEv = 252,
		.ability = FRONTIER_ABILITY_HIDDEN, //ABILITY_RECKLESS
		.item = ITEM_CHOICE_BAND,
		.moves =
		{
			MOVE_FLAREBLITZ,
			MOVE_SUPERPOWER,
			MOVE_WILDCHARGE,
			MOVE_SUCKERPUNCH,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = FALSE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_SAMUROTT,
		.nature = NATURE_MODEST,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.defEv = 4,
		.spAtkEv = 252,
		.spdEv = 252,
		.ability = FRONTIER_ABILITY_HIDDEN, //ABILITY_SHELLARMOR
		.item = ITEM_ZOOM_LENS,
		.moves =
		{
			MOVE_HYDROPUMP,
			MOVE_BLIZZARD,
			MOVE_GRASSKNOT,
			MOVE_AQUAJET,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = FALSE,
		.modifyMovesDoubles = FALSE,
	},
};

static const struct BattleTowerSpread sRaidPartnerSpread_Juniper_Rank6[] =
{
	{
		.species = SPECIES_KYUREM,
		.nature = NATURE_TIMID,
		.hpIv = 31,
		.atkIv = 0,
		.defIv = 31,
		.spAtkIv = 30,
		.spDefIv = 31,
		.spdIv = 30,
		.spAtkEv = 252,
		.spDefEv = 4,
		.spdEv = 252,
		.ability = FRONTIER_ABILITY_1, //ABILITY_PRESSURE
		.item = ITEM_EJECT_PACK,
		.moves =
		{
			MOVE_DRACOMETEOR,
			MOVE_ICEBEAM,
			MOVE_EARTHPOWER,
			MOVE_HIDDENPOWER, //TYPE_FIRE
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_RESHIRAM,
		.nature = NATURE_MODEST,
		.hpIv = 31,
		.atkIv = 0,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.hpEv = 4,
		.spAtkEv = 252,
		.spdEv = 252,
		.ability = FRONTIER_ABILITY_1, //ABILITY_TURBOBLAZE
		.item = ITEM_WHITE_HERB,
		.moves =
		{
			MOVE_DRACOMETEOR,
			MOVE_BLUEFLARE,
			MOVE_EARTHPOWER,
			MOVE_FOCUSBLAST,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = FALSE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_ZEKROM,
		.nature = NATURE_NAUGHTY,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.atkEv = 252,
		.spAtkEv = 4,
		.spdEv = 252,
		.ability = FRONTIER_ABILITY_1, //ABILITY_TERAVOLT
		.item = ITEM_CHOICE_SCARF,
		.moves =
		{
			MOVE_OUTRAGE,
			MOVE_BOLTSTRIKE,
			MOVE_DRACOMETEOR,
			MOVE_VOLTSWITCH,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,//
	},
};

static const struct BattleTowerSpread sRaidPartnerSpread_Juniper2_Rank2[] =
{
	{
		.species = SPECIES_PANSAGE,
		.nature = NATURE_JOLLY,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 0,
		.spDefIv = 31,
		.spdIv = 31,
		.atkEv = 252,
		.defEv = 12,
		.spdEv = 244,
		.ability = FRONTIER_ABILITY_HIDDEN, //ABILITY_OVERGROW
		.item = ITEM_LIFE_ORB,
		.moves =
		{
			MOVE_BULLETSEED,
			MOVE_KNOCKOFF,
			MOVE_LOWSWEEP,
			MOVE_GUNKSHOT,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_PANSEAR,
		.nature = NATURE_JOLLY,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 0,
		.spDefIv = 31,
		.spdIv = 31,
		.defEv = 212,
		.spDefEv = 212,
		.spdEv = 84,
		.ability = FRONTIER_ABILITY_HIDDEN, //ABILITY_BLAZE
		.item = ITEM_BERRY_JUICE,
		.moves =
		{
			MOVE_FLAMECHARGE,
			MOVE_WILLOWISP,
			MOVE_RECYCLE,
			MOVE_SUBSTITUTE,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_PANPOUR,
		.nature = NATURE_MODEST,
		.hpIv = 31,
		.atkIv = 0,
		.defIv = 31,
		.spAtkIv = 30,
		.spDefIv = 31,
		.spdIv = 31,
		.hpEv = 36,
		.defEv = 52,
		.spAtkEv = 176,
		.spdEv = 244,
		.ability = FRONTIER_ABILITY_HIDDEN, //ABILITY_TORRENT
		.item = ITEM_BERRY_JUICE,
		.moves =
		{
			MOVE_HYDROPUMP,
			MOVE_HIDDENPOWER, //TYPE_GRASS
			MOVE_ICEBEAM,
			MOVE_NASTYPLOT,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
};

static const struct BattleTowerSpread sRaidPartnerSpread_Juniper2_Rank5[] =
{
	{
		.species = SPECIES_SIMISAGE,
		.nature = NATURE_JOLLY,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 0,
		.spDefIv = 31,
		.spdIv = 31,
		.atkEv = 252,
		.defEv = 4,
		.spdEv = 252,
		.ability = FRONTIER_ABILITY_1, //ABILITY_GLUTTONY
		.item = ITEM_LIECHI_BERRY,
		.moves =
		{
			MOVE_KNOCKOFF,
			MOVE_SEEDBOMB,
			MOVE_POWERUPPUNCH,
			MOVE_ROCKSLIDE,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_SIMISEAR,
		.nature = NATURE_JOLLY,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 0,
		.spDefIv = 31,
		.spdIv = 31,
		.atkEv = 252,
		.defEv = 4,
		.spdEv = 252,
		.ability = FRONTIER_ABILITY_1, //ABILITY_GLUTTONY
		.item = ITEM_AGUAV_BERRY,
		.moves =
		{
			MOVE_FIREPUNCH,
			MOVE_KNOCKOFF,
			MOVE_ACROBATICS,
			MOVE_POWERUPPUNCH,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = FALSE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_SIMIPOUR,
		.nature = NATURE_TIMID,
		.hpIv = 31,
		.atkIv = 0,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.defEv = 4,
		.spAtkEv = 252,
		.spdEv = 252,
		.ability = FRONTIER_ABILITY_HIDDEN, //ABILITY_TORRENT
		.item = ITEM_LIFE_ORB,
		.moves =
		{
			MOVE_HYDROPUMP,
			MOVE_ICEBEAM,
			MOVE_FOCUSBLAST,
			MOVE_GRASSKNOT,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = FALSE,
		.modifyMovesDoubles = FALSE,
	},
};

static const struct BattleTowerSpread sRaidPartnerSpread_Juniper2_Rank6[] =
{
	{
		.species = SPECIES_VICTINI,
		.nature = NATURE_JOLLY,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 0,
		.spDefIv = 31,
		.spdIv = 31,
		.atkEv = 252,
		.defEv = 4,
		.spdEv = 252,
		.ability = FRONTIER_ABILITY_1, //ABILITY_VICTORYSTAR
		.item = ITEM_CHOICE_BAND,
		.moves =
		{
			MOVE_VCREATE,
			MOVE_BOLTSTRIKE,
			MOVE_ZENHEADBUTT,
			MOVE_UTURN,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_MELOETTA,
		.nature = NATURE_JOLLY,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.atkEv = 252,
		.defEv = 4,
		.spdEv = 252,
		.ability = FRONTIER_ABILITY_1, //ABILITY_SERENEGRACE
		.item = ITEM_EXPERT_BELT,
		.moves =
		{
			MOVE_RETURN,
			MOVE_CLOSECOMBAT,
			MOVE_KNOCKOFF,
			MOVE_RELICSONG,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_GENESECT,
		.nature = NATURE_MODEST,
		.hpIv = 31,
		.atkIv = 0,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.defEv = 4,
		.spAtkEv = 252,
		.spdEv = 252,
		.ability = FRONTIER_ABILITY_1, //ABILITY_DOWNLOAD
		.item = ITEM_BURN_DRIVE,
		.moves =
		{
			MOVE_FLASHCANNON,
			MOVE_TECHNOBLAST,
			MOVE_THUNDERBOLT,
			MOVE_ICEBEAM,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
};

static const struct BattleTowerSpread sRaidPartnerSpread_Kai_Rank2[] =
{
	{
		.species = SPECIES_PANCHAM,
		.nature = NATURE_ADAMANT,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.hpEv = 252,
		.atkEv = 252,
		.defEv = 4,
		.ability = FRONTIER_ABILITY_1, //ABILITY_IRONFIST
		.item = ITEM_LIFE_ORB,
		.moves =
		{
			MOVE_FIREPUNCH,
			MOVE_ICEPUNCH,
			MOVE_THUNDERPUNCH,
			MOVE_DRAINPUNCH,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_SABLEYE,
		.nature = NATURE_BOLD,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.hpEv = 252,
		.defEv = 128,
		.spDefEv = 128,
		.ability = FRONTIER_ABILITY_HIDDEN, //ABILITY_PRANKSTER
		.item = ITEM_LAGGING_TAIL,
		.moves =
		{
			MOVE_QUASH,
			MOVE_PROTECT,
			MOVE_TRICK,
			MOVE_NIGHTSHADE,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_MURKROW,
		.nature = NATURE_ADAMANT,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.hpEv = 4,
		.atkEv = 252,
		.spdEv = 252,
		.ability = FRONTIER_ABILITY_2, //ABILITY_SUPERLUCK
		.item = ITEM_LEFTOVERS,
		.moves =
		{
			MOVE_ROOST,
			MOVE_ASSURANCE,
			MOVE_PLUCK,
			MOVE_SUCKERPUNCH,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
};

static const struct BattleTowerSpread sRaidPartnerSpread_Kai_Rank5[] =
{
	{
		.species = SPECIES_PANGORO,
		.nature = NATURE_ADAMANT,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.hpEv = 252,
		.atkEv = 252,
		.defEv = 4,
		.ability = FRONTIER_ABILITY_1, //ABILITY_IRONFIST
		.item = ITEM_LIFE_ORB,
		.moves =
		{
			MOVE_SUCKERPUNCH,
			MOVE_DRAINPUNCH,
			MOVE_ICEPUNCH,
			MOVE_FIREPUNCH,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_SABLEYE,
		.nature = NATURE_BOLD,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.hpEv = 252,
		.defEv = 128,
		.spDefEv = 128,
		.ability = FRONTIER_ABILITY_HIDDEN, //ABILITY_PRANKSTER
		.item = ITEM_LAGGING_TAIL,
		.moves =
		{
			MOVE_QUASH,
			MOVE_PROTECT,
			MOVE_TRICK,
			MOVE_NIGHTSHADE,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_HONCHKROW,
		.nature = NATURE_ADAMANT,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.hpEv = 4,
		.atkEv = 252,
		.spdEv = 252,
		.ability = FRONTIER_ABILITY_2, //ABILITY_SUPERLUCK
		.item = ITEM_LEFTOVERS,
		.moves =
		{
			MOVE_ROOST,
			MOVE_NIGHTSLASH,
			MOVE_DRILLPECK,
			MOVE_BRAVEBIRD,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
};

static const struct BattleTowerSpread sRaidPartnerSpread_Kai_Rank6[] =
{
	{
		.species = SPECIES_URSHIFU_SINGLE,
		.nature = NATURE_ADAMANT,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.atkEv = 252,
		.defEv = 4,
		.spdEv = 252,
		.ability = FRONTIER_ABILITY_1, //ABILITY_UNSEENFIST
		.item = ITEM_CHOICE_BAND,
		.moves =
		{
			MOVE_WICKEDBLOW,
			MOVE_CLOSECOMBAT,
			MOVE_IRONHEAD,
			MOVE_UTURN,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
		.gigantamax = TRUE,
	},
	{
		.species = SPECIES_YVELTAL,
		.nature = NATURE_TIMID,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.hpEv = 252,
		.spAtkEv = 252,
		.spdEv = 4,
		.ability = FRONTIER_ABILITY_1, //ABILITY_DARKAURA
		.item = ITEM_HEAVY_DUTY_BOOTS,
		.moves =
		{
			MOVE_OBLIVIONWING,
			MOVE_KNOCKOFF,
			MOVE_ROOST,
			MOVE_DARKPULSE,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_DARKRAI,
		.nature = NATURE_MODEST,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.spAtkEv = 252,
		.spdEv = 252,
		.spDefEv = 4,
		.ability = FRONTIER_ABILITY_1, //ABILITY_BADDREAMS
		.item = ITEM_LEFTOVERS,
		.moves =
		{
			MOVE_DARKVOID,
			MOVE_DARKPULSE,
			MOVE_SHADOWBALL,
			MOVE_SLUDGEBOMB,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
};

static const struct BattleTowerSpread sRaidPartnerSpread_Skye_Rank2[] =
{
	{
		.species = SPECIES_BIDOOF,
		.nature = NATURE_ADAMANT,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.hpEv = 252,
		.atkEv = 128,
		.defEv = 128,
		.ability = FRONTIER_ABILITY_HIDDEN, //ABILITY_MOODY
		.item = ITEM_SITRUS_BERRY,
		.moves =
		{
			MOVE_TAUNT,
			MOVE_TAKEDOWN,
			MOVE_DEFENSECURL,
			MOVE_ROLLOUT,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_GOLBAT,
		.nature = NATURE_JOLLY,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.atkEv = 252,
		.spdEv = 252,
		.hpEv = 4,
		.ability = FRONTIER_ABILITY_HIDDEN, //ABILITY_INFILTRATOR
		.item = ITEM_ASSAULT_VEST,
		.moves =
		{
			MOVE_ZENHEADBUTT,
			MOVE_POISONFANG,
			MOVE_LEECHLIFE,
			MOVE_PLUCK,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_VIBRAVA,
		.nature = NATURE_ADAMANT,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.atkEv = 252,
		.spdEv = 252,
		.hpEv = 4,
		.ability = FRONTIER_ABILITY_1, //ABILITY_LEVITATE
		.item = ITEM_EVIOLITE,
		.moves =
		{
			MOVE_DIG,
			MOVE_CRUNCH,
			MOVE_BUGBITE,
			MOVE_FOCUSENERGY,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
};

static const struct BattleTowerSpread sRaidPartnerSpread_Skye_Rank5[] =
{
	{
		.species = SPECIES_BIBAREL,
		.nature = NATURE_ADAMANT,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.hpEv = 252,
		.spdEv = 252,
		.atkEv = 4,
		.ability = FRONTIER_ABILITY_HIDDEN, //ABILITY_MOODY
		.item = ITEM_SITRUS_BERRY,
		.moves =
		{
			MOVE_SUBSTITUTE,
			MOVE_RETURN,
			MOVE_PROTECT,
			MOVE_WATERFALL,

		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_CROBAT,
		.nature = NATURE_JOLLY,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.atkEv = 252,
		.spdEv = 252,
		.spDefEv = 4,
		.ability = FRONTIER_ABILITY_HIDDEN, //ABILITY_INFILTRATOR
		.item = ITEM_LIFE_ORB,
		.moves =
		{
			MOVE_BRAVEBIRD,
			MOVE_CROSSPOISON,
			MOVE_LEECHLIFE,
			MOVE_ZENHEADBUTT,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_FLYGON,
		.nature = NATURE_ADAMANT,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.atkEv = 252,
		.spdEv = 252,
		.hpEv = 4,
		.ability = FRONTIER_ABILITY_1, //ABILITY_LEVITATE
		.item = ITEM_LEFTOVERS,
		.moves =
		{
			MOVE_EARTHQUAKE,
			MOVE_OUTRAGE,
			MOVE_STONEEDGE,
			MOVE_SUPERPOWER,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
};

static const struct BattleTowerSpread sRaidPartnerSpread_Skye_Rank6[] =
{
	{
		.species = SPECIES_ARCEUS,
		.nature = NATURE_ADAMANT,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.atkEv = 252,
		.spdEv = 252,
		.spDefEv = 4,
		.ability = FRONTIER_ABILITY_1, //ABILITY_MULTITYPE
		.item = ITEM_LIFE_ORB,
		.moves =
		{
			MOVE_EXTREMESPEED,
			MOVE_PUNISHMENT,
			MOVE_SHADOWCLAW,
			MOVE_STONEEDGE,

		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_ETERNATUS,
		.nature = NATURE_TIMID,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.spAtkEv = 252,
		.spdEv = 252,
		.spDefEv = 4,
		.ability = FRONTIER_ABILITY_1, //ABILITY_PRESSURE
		.item = ITEM_CHOICE_SPECS,
		.moves =
		{
			MOVE_FLAMETHROWER,
			MOVE_VENOSHOCK,
			MOVE_DYNAMAXCANNON,
			MOVE_ETERNABEAM,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_ZYGARDE,
		.nature = NATURE_ADAMANT,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.atkEv = 252,
		.defEv = 252,
		.spdEv = 4,
		.ability = FRONTIER_ABILITY_HIDDEN, //ABILITY_POWERCONSTRUCT
		.item = ITEM_LEFTOVERS,
		.moves =
		{
			MOVE_THOUSANDARROWS,
			MOVE_OUTRAGE,
			MOVE_SUPERPOWER,
			MOVE_EXTREMESPEED,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
};

static const struct BattleTowerSpread sRaidPartnerSpread_Cynthia_Rank2[] =
{
	{
		.species = SPECIES_SPIRITOMB,
		.nature = NATURE_MODEST,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.spAtkEv = 252,
		.spDefEv = 252,
		.hpEv = 4,
		.ability = FRONTIER_ABILITY_1, //ABILITY_PRESSURE
		.item = ITEM_SITRUS_BERRY,
		.moves =
		{
			MOVE_SHADOWBALL,
			MOVE_DARKPULSE,
			MOVE_WILLOWISP,
			MOVE_SUCKERPUNCH,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_PORYGON,
		.nature = NATURE_MODEST,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.spAtkEv = 252,
		.spDefEv = 4,
		.spdEv = 252,
		.ability = FRONTIER_ABILITY_2, //ABILITY_DOWNLOAD
		.item = ITEM_ASSAULT_VEST,
		.moves =
		{
			MOVE_TRIATTACK,
			MOVE_ICEBEAM,
			MOVE_THUNDERBOLT,
			MOVE_SHADOWBALL,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_TOGETIC,
		.nature = NATURE_TIMID,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.spAtkEv = 252,
		.spDefEv = 4,
		.spdEv = 252,
		.ability = FRONTIER_ABILITY_2, //ABILITY_SERENEGRACE
		.item = ITEM_EVIOLITE,
		.moves =
		{
			MOVE_DAZZLINGGLEAM,
			MOVE_SHADOWBALL,
			MOVE_FLAMETHROWER,
			MOVE_ROOST,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
};

static const struct BattleTowerSpread sRaidPartnerSpread_Cynthia_Rank5[] =
{
	{
		.species = SPECIES_SPIRITOMB,
		.nature = NATURE_MODEST,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.spAtkEv = 252,
		.spDefEv = 252,
		.hpEv = 4,
		.ability = FRONTIER_ABILITY_1, //ABILITY_PRESSURE
		.item = ITEM_SITRUS_BERRY,
		.moves =
		{
			MOVE_SHADOWBALL,
			MOVE_DARKPULSE,
			MOVE_WILLOWISP,
			MOVE_SUCKERPUNCH,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_PORYGON_Z,
		.nature = NATURE_MODEST,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.spAtkEv = 252,
		.spDefEv = 4,
		.spdEv = 252,
		.ability = FRONTIER_ABILITY_1, //ABILITY_ADAPTABILITY
		.item = ITEM_LIFE_ORB,
		.moves =
		{
			MOVE_SHADOWBALL,
			MOVE_TRIATTACK,
			MOVE_HYPERBEAM,
			MOVE_CONVERSION,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_TOGEKISS,
		.nature = NATURE_TIMID,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.spAtkEv = 252,
		.spDefEv = 4,
		.spdEv = 252,
		.ability = FRONTIER_ABILITY_2, //ABILITY_SERENEGRACE
		.item = ITEM_CHOICE_SCARF,
		.moves =
		{
			MOVE_DAZZLINGGLEAM,
			MOVE_AIRSLASH,
			MOVE_FLAMETHROWER,
			MOVE_ROOST,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
};

static const struct BattleTowerSpread sRaidPartnerSpread_Cynthia_Rank6[] =
{
	{
		.species = SPECIES_NECROZMA_DAWN_WINGS,
		.nature = NATURE_TIMID,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.spAtkEv = 252,
		.spdEv = 252,
		.spDefEv = 4,
		.ability = FRONTIER_ABILITY_1, //ABILITY_PRISMARMOR
		.item = ITEM_POWER_HERB,
		.moves =
		{
			MOVE_METEORBEAM,
			MOVE_MOONGEISTBEAM,
			MOVE_PHOTONGEYSER, 
			MOVE_HEATWAVE,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_REGIGIGAS,
		.nature = NATURE_ADAMANT,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.atkEv = 252,
		.spDefEv = 4,
		.hpEv = 252,
		.ability = FRONTIER_ABILITY_1, //ABILITY_SLOWSTART
		.item = ITEM_LIFE_ORB,
		.moves =
		{
			MOVE_KNOCKOFF,
			MOVE_RETURN,
			MOVE_DRAINPUNCH,
			MOVE_THUNDERWAVE,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_ZACIAN_CROWNED,
		.nature = NATURE_TIMID,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.spAtkEv = 252,
		.spDefEv = 4,
		.spdEv = 252,
		.ability = FRONTIER_ABILITY_1, //ABILITY_INTREPEDSWORD
		.item = ITEM_RUSTED_SWORD,
		.moves =
		{
			MOVE_PLAYROUGH,
			MOVE_BEHEMOTHBLADE,
			MOVE_WILDCHARGE,
			MOVE_CLOSECOMBAT,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
};

static const struct BattleTowerSpread sRaidPartnerSpread_Cynthia2_Rank2[] =
{
	{
		.species = SPECIES_RIOLU,
		.nature = NATURE_JOLLY,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.atkEv = 252,
		.spdEv = 252,
		.spDefEv = 4,
		.ability = FRONTIER_ABILITY_2, //ABILITY_INNERFOCUS
		.item = ITEM_FOCUS_SASH,
		.moves =
		{
			MOVE_HIGHJUMPKICK,
			MOVE_METEORMASH,
			MOVE_BLAZEKICK,
			MOVE_THUNDERPUNCH,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_FEEBAS,
		.nature = NATURE_BOLD,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.spAtkEv = 252,
		.defEv = 128,
		.spDefEv = 128,
		.atkEv = 4,
		.ability = FRONTIER_ABILITY_HIDDEN, //ABILITY_ADAPTABILITY
		.item = ITEM_LEFTOVERS,
		.moves =
		{
			MOVE_SCALD,
			MOVE_ICEBEAM,
			MOVE_FLAIL,
			MOVE_DRAGONPULSE,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_GABITE,
		.nature = NATURE_JOLLY,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.atkEv = 252,
		.spDefEv = 4,
		.spdEv = 252,
		.ability = FRONTIER_ABILITY_HIDDEN, //ABILITY_ROUGHSKIN
		.item = ITEM_EVIOLITE,
		.moves =
		{
			MOVE_DRAGONCLAW,
			MOVE_EARTHQUAKE,
			MOVE_IRONHEAD,
			MOVE_ROCKSLIDE,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
};

static const struct BattleTowerSpread sRaidPartnerSpread_Cynthia2_Rank5[] =
{
	{
		.species = SPECIES_LUCARIO,
		.nature = NATURE_JOLLY,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.atkEv = 252,
		.spdEv = 252,
		.spDefEv = 4,
		.ability = FRONTIER_ABILITY_2, //ABILITY_INNERFOCUS
		.item = ITEM_FOCUS_SASH,
		.moves =
		{
			MOVE_CLOSECOMBAT,
			MOVE_METEORMASH,
			MOVE_EXTREMESPEED,
			MOVE_EARTHQUAKE,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_MILOTIC,
		.nature = NATURE_BOLD,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.spAtkEv = 252,
		.defEv = 128,
		.spDefEv = 128,
		.hpEv = 4,
		.ability = FRONTIER_ABILITY_1, //ABILITY_MARVELSCALE
		.item = ITEM_FLAME_ORB,
		.moves =
		{
			MOVE_RECOVER,
			MOVE_MIRRORCOAT,
			MOVE_SCALD,
			MOVE_ICEBEAM,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_GARCHOMP,
		.nature = NATURE_JOLLY,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.atkEv = 252,
		.spDefEv = 4,
		.spdEv = 252,
		.ability = FRONTIER_ABILITY_HIDDEN, //ABILITY_ROUGHSKIN
		.item = ITEM_YACHE_BERRY,
		.moves =
		{
			MOVE_DRAGONCLAW,
			MOVE_EARTHQUAKE,
			MOVE_IRONHEAD,
			MOVE_ROCKSLIDE,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
};

static const struct BattleTowerSpread sRaidPartnerSpread_Cynthia2_Rank6[] =
{
	{
		.species = SPECIES_ZAMAZENTA_CROWNED,
		.nature = NATURE_IMPISH,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.atkEv = 252,
		.spdEv = 252,
		.spDefEv = 4,
		.ability = FRONTIER_ABILITY_2, //ABILITY_DAUNTLESSSHIELD
		.item = ITEM_RUSTED_SHIELD,
		.moves =
		{
			MOVE_CLOSECOMBAT,
			MOVE_CRUNCH,
			MOVE_BEHEMOTHBASH,
			MOVE_PSYCHICFANGS,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_KELDEO,
		.nature = NATURE_TIMID,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.spAtkEv = 252,
		.spdEv = 252,
		.spDefEv = 4,
		.ability = FRONTIER_ABILITY_1, //ABILITY_JUSTIFIED
		.item = ITEM_CHOICE_SPECS,
		.moves =
		{
			MOVE_HYDROPUMP,
			MOVE_SECRETSWORD,
			MOVE_SCALD,
			MOVE_ICYWIND,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_ZYGARDE,
		.nature = NATURE_ADAMANT,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.atkEv = 252,
		.defEv = 252,
		.spdEv = 4,
		.ability = FRONTIER_ABILITY_HIDDEN, //ABILITY_POWERCONSTRUCT
		.item = ITEM_LEFTOVERS,
		.moves =
		{
			MOVE_THOUSANDARROWS,
			MOVE_OUTRAGE,
			MOVE_SUPERPOWER,
			MOVE_EXTREMESPEED,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
};

static const struct BattleTowerSpread sRaidPartnerSpread_Mei_Rank2[] =
{
	{
		.species = SPECIES_SALANDIT,
		.nature = NATURE_TIMID,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.spAtkEv = 252,
		.spdEv = 252,
		.spDefEv = 4,
		.ability = FRONTIER_ABILITY_1, //ABILITY_CORROSION
		.item = ITEM_LEFTOVERS,
		.moves =
		{
			MOVE_FLAMETHROWER,
			MOVE_SLUDGEBOMB,
			MOVE_VENOMDRENCH,
			MOVE_TOXIC,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_KIRLIA,
		.nature = NATURE_TIMID,
		.hpIv = 31,
		.atkIv = 0,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.spAtkEv = 252,
		.spDefEv = 4,
		.spdEv = 252,
		.ability = FRONTIER_ABILITY_HIDDEN, //ABILITY_TELEPATHY
		.item = ITEM_LEFTOVERS,
		.moves =
		{
			MOVE_PSYCHIC,
			MOVE_EXPANDINGFORCE,
			MOVE_DRAININGKISS,
			MOVE_CALMMIND,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_STEENEE,
		.nature = NATURE_ADAMANT,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.hpEv = 252,
		.spDefEv = 4,
		.atkEv = 252,
		.ability = FRONTIER_ABILITY_HIDDEN, //ABILITY_SWEETVEIL
		.item = ITEM_LEFTOVERS,
		.moves =
		{
			MOVE_GRASSYGLIDE,
			MOVE_TRIPLEAXEL,
			MOVE_LOWSWEEP,
			MOVE_BOUNCE,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
};

static const struct BattleTowerSpread sRaidPartnerSpread_Mei_Rank5[] =
{
	{
		.species = SPECIES_SALAZZLE,
		.nature = NATURE_ADAMANT,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.atkEv = 252,
		.spdEv = 252,
		.spDefEv = 4,
		.ability = FRONTIER_ABILITY_1, //ABILITY_CORROSION
		.item = ITEM_LEFTOVERS,
		.moves =
		{
			MOVE_DRAGONCLAW,
			MOVE_FIRELASH,
			MOVE_GUNKSHOT,
			MOVE_DRAGONDANCE,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_GARDEVOIR,
		.nature = NATURE_TIMID,
		.hpIv = 31,
		.atkIv = 0,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.spAtkEv = 252,
		.spDefEv = 4,
		.spdEv = 252,
		.ability = FRONTIER_ABILITY_HIDDEN, //ABILITY_TELEPATHY
		.item = ITEM_LEFTOVERS,
		.moves =
		{
			MOVE_DAZZLINGGLEAM,
			MOVE_EXPANDINGFORCE,
			MOVE_CALMMIND,
			MOVE_PSYCHIC,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_TSAREENA,
		.nature = NATURE_ADAMANT,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.hpEv = 252,
		.spDefEv = 4,
		.atkEv = 252,
		.ability = FRONTIER_ABILITY_2, //ABILITY_QUEENLYMAGESTY
		.item = ITEM_LEFTOVERS,
		.moves =
		{
			MOVE_HIGHJUMPKICK,
			MOVE_GRASSYGLIDE,
			MOVE_TROPKICK,
			MOVE_SYNTHESIS,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
};

static const struct BattleTowerSpread sRaidPartnerSpread_Mei_Rank6[] =
{
	{
		.species = SPECIES_DIANCIE,
		.nature = NATURE_BRAVE,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.atkEv = 252,
		.hpEv = 252,
		.spAtkEv = 4,
		.ability = FRONTIER_ABILITY_1, //ABILITY_CLEARBODY
		.item = ITEM_DIANCITE,
		.moves =
		{
			MOVE_DIAMONDSTORM,
			MOVE_GYROBALL,
			MOVE_DAZZLINGGLEAM,
			MOVE_MOONBLAST,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_SALAZZLE,
		.nature = NATURE_ADAMANT,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.atkEv = 252,
		.spdEv = 252,
		.spDefEv = 4,
		.ability = FRONTIER_ABILITY_1, //ABILITY_CORROSION
		.item = ITEM_LEFTOVERS,
		.moves =
		{
			MOVE_DRAGONCLAW,
			MOVE_FIRELASH,
			MOVE_GUNKSHOT,
			MOVE_DRAGONDANCE,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_GARDEVOIR,
		.nature = NATURE_TIMID,
		.hpIv = 31,
		.atkIv = 0,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.spAtkEv = 252,
		.spDefEv = 4,
		.spdEv = 252,
		.ability = FRONTIER_ABILITY_HIDDEN, //ABILITY_TELEPATHY
		.item = ITEM_LEFTOVERS,
		.moves =
		{
			MOVE_DAZZLINGGLEAM,
			MOVE_EXPANDINGFORCE,
			MOVE_CALMMIND,
			MOVE_PSYCHIC,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
};

static const struct BattleTowerSpread sRaidPartnerSpread_Fernando_Rank2[] =
{
	{
		.species = SPECIES_ANORITH,
		.nature = NATURE_ADAMANT,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.spdEv = 252,
		.hpEv = 128,
		.atkEv = 128,
		.ability = FRONTIER_ABILITY_1, //ABILITY_BATTLEARMOR
		.item = ITEM_LEFTOVERS,
		.moves =
		{
			MOVE_XSCISSOR,
			MOVE_ROCKSLIDE,
			MOVE_CRUSHCLAW,
			MOVE_AERIALACE,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_MILTANK,
		.nature = NATURE_IMPISH,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.hpEv = 252,
		.atkEv = 128,
		.defEv = 128,
		.ability = FRONTIER_ABILITY_1, //ABILITY_THICKFAT
		.item = ITEM_LEFTOVERS,
		.moves =
		{
			MOVE_MILKDRINK,
			MOVE_SEISMICTOSS,
			MOVE_BULLDOZE,
			MOVE_DYNAMICPUNCH,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_ROSELIA,
		.nature = NATURE_TIMID,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.spAtkEv = 252,
		.spdEv = 252,
		.spDefEv = 4,
		.ability = FRONTIER_ABILITY_1, //ABILITY_NATURALCURE
		.item = ITEM_LEFTOVERS,
		.moves =
		{
			MOVE_SLUDGEBOMB,
			MOVE_GIGADRAIN,
			MOVE_EXTRASENSORY,
			MOVE_DAZZLINGGLEAM,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
};


static const struct BattleTowerSpread sRaidPartnerSpread_Fernando_Rank5[] =
{
	{
		.species = SPECIES_ARMALDO,
		.nature = NATURE_ADAMANT,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.spdEv = 252,
		.hpEv = 128,
		.atkEv = 128,
		.ability = FRONTIER_ABILITY_1, //ABILITY_BATTLEARMOR
		.item = ITEM_LEFTOVERS,
		.moves =
		{
			MOVE_STONEEDGE,
			MOVE_RAPIDSPIN,
			MOVE_XSCISSOR,
			MOVE_PROTECT,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_MILTANK,
		.nature = NATURE_IMPISH,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.hpEv = 252,
		.atkEv = 128,
		.defEv = 128,
		.ability = FRONTIER_ABILITY_1, //ABILITY_THICKFAT
		.item = ITEM_LEFTOVERS,
		.moves =
		{
			MOVE_MILKDRINK,
			MOVE_SEISMICTOSS,
			MOVE_BULLDOZE,
			MOVE_DYNAMICPUNCH,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_ROSERADE,
		.nature = NATURE_TIMID,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.spAtkEv = 252,
		.spdEv = 252,
		.spDefEv = 4,
		.ability = FRONTIER_ABILITY_1, //ABILITY_NATURALCURE
		.item = ITEM_LEFTOVERS,
		.moves =
		{
			MOVE_SLUDGEBOMB,
			MOVE_ENERGYBALL,
			MOVE_EXTRASENSORY,
			MOVE_SYNTHESIS,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
};

static const struct BattleTowerSpread sRaidPartnerSpread_Fernando_Rank6[] =
{
	{
		.species = SPECIES_LANDORUS,
		.nature = NATURE_ADAMANT,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.atkEv = 4,
		.spAtkEv = 252,
		.spdEv = 252,
		.ability = FRONTIER_ABILITY_HIDDEN, //ABILITY_SHEERFORCE
		.item = ITEM_CHOICE_BAND,
		.moves =
		{
			MOVE_ROCKSLIDE,
			MOVE_EARTHPOWER,
			MOVE_SLUDGEWAVE,
			MOVE_SUPERPOWER,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_CALYREX_SHADOW_RIDER,
		.nature = NATURE_IMPISH,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.spAtkEv = 252,
		.spDefEv = 4,
		.spdEv = 252,
		.ability = FRONTIER_ABILITY_1, //ABILITY_SOULHEART?
		.item = ITEM_CHOICE_SPECS,
		.moves =
		{
			MOVE_ASTRALBARRAGE,
			MOVE_PSYSHOCK,
			MOVE_TRICK,
			MOVE_DRAININGKISS,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_TAPU_BULU,
		.nature = NATURE_IMPISH,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.hpEv = 252,
		.spdEv = 136,
		.defEv = 120,
		.ability = FRONTIER_ABILITY_1, //ABILITY_GRASSYSURGE
		.item = ITEM_LEFTOVERS,
		.moves =
		{
			MOVE_HORNLEECH,
			MOVE_DAZZLINGGLEAM,
			MOVE_TOXIC,
			MOVE_SYNTHESIS,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
};

static const struct BattleTowerSpread sRaidPartnerSpread_May_Rank2[] =
{
	{
		.species = SPECIES_COMBUSKEN,
		.nature = NATURE_JOLLY,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.atkEv = 252,
		.spdEv = 252,
		.hpEv = 4,
		.ability = FRONTIER_ABILITY_HIDDEN, //ABILITY_SPEEDBOOST
		.item = ITEM_LEFTOVERS,
		.moves =
		{
			MOVE_FIREPUNCH,
			MOVE_THUNDERPUNCH,
			MOVE_POISONJAB,
			MOVE_PROTECT,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_VIBRAVA,
		.nature = NATURE_JOLLY,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.atkEv = 252,
		.spDefEv = 4,
		.spdEv = 252,
		.ability = FRONTIER_ABILITY_1, //ABILITY_LEVITATE
		.item = ITEM_LEFTOVERS,
		.moves =
		{
			MOVE_BULLDOZE,
			MOVE_DRAGONCLAW,
			MOVE_THROATCHOP,
			MOVE_ROCKSLIDE,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_WAILMER,
		.nature = NATURE_MODEST,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.hpEv = 252,
		.spAtkEv = 252,
		.spdEv = 4,
		.ability = FRONTIER_ABILITY_1, //ABILITY_WATERVEIL
		.item = ITEM_LEFTOVERS,
		.moves =
		{
			MOVE_WATERSPOUT,
			MOVE_WHIRLPOOL,
			MOVE_REST,
			MOVE_SLEEPTALK,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
};

static const struct BattleTowerSpread sRaidPartnerSpread_May_Rank5[] =
{
	{
		.species = SPECIES_BLAZIKEN,
		.nature = NATURE_JOLLY,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.atkEv = 252,
		.spdEv = 252,
		.hpEv = 4,
		.ability = FRONTIER_ABILITY_HIDDEN, //ABILITY_SPEEDBOOST
		.item = ITEM_BLAZIKENITE,
		.moves =
		{
			MOVE_FLAREBLITZ,
			MOVE_THUNDERPUNCH,
			MOVE_EARTHQUAKE,
			MOVE_PROTECT,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_FLYGON,
		.nature = NATURE_JOLLY,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.atkEv = 252,
		.spDefEv = 4,
		.spdEv = 252,
		.ability = FRONTIER_ABILITY_1, //ABILITY_LEVITATE
		.item = ITEM_LEFTOVERS,
		.moves =
		{
			MOVE_SUBSTITUTE,
			MOVE_DRAGONDANCE,
			MOVE_DRAGONCLAW,
			MOVE_EARTHQUAKE,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_WAILORD,
		.nature = NATURE_MODEST,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.hpEv = 252,
		.spAtkEv = 252,
		.spdEv = 4,
		.ability = FRONTIER_ABILITY_1, //ABILITY_WATERVEIL
		.item = ITEM_LEFTOVERS,
		.moves =
		{
			MOVE_AQUARING,
			MOVE_SCALD,
			MOVE_WATERSPOUT,
			MOVE_ICEBEAM,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
};

static const struct BattleTowerSpread sRaidPartnerSpread_May_Rank6[] =
{
	{
		.species = SPECIES_GROUDON,
		.nature = NATURE_ADAMANT,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.atkEv = 252,
		.spdEv = 252,
		.spDefEv = 4,
		.ability = FRONTIER_ABILITY_1, //ABILITY_DROUGHT
		.item = ITEM_RED_ORB,
		.moves =
		{
			MOVE_FIREPUNCH,
			MOVE_THUNDERPUNCH,
			MOVE_PRECIPICEBLADES,
			MOVE_HAMMERARM,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_KYOGRE,
		.nature = NATURE_TIMID,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.spAtkEv = 252,
		.spDefEv = 4,
		.spdEv = 252,
		.ability = FRONTIER_ABILITY_1, //ABILITY_DRIZZLE
		.item = ITEM_BLUE_ORB,
		.moves =
		{
			MOVE_ORIGINPULSE,
			MOVE_THUNDER,
			MOVE_ICEBEAM,
			MOVE_SCALD,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_RAYQUAZA,
		.nature = NATURE_JOLLY,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.atkEv = 252,
		.spDefEv = 4,
		.spdEv = 252,
		.ability = FRONTIER_ABILITY_1, //ABILITY_AIRLOCK
		.item = ITEM_CHOICE_BAND,
		.moves =
		{
			MOVE_DRAGONASCENT,
			MOVE_EXTREMESPEED,
			MOVE_VCREATE,
			MOVE_DRAGONCLAW,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
};

static const struct BattleTowerSpread sRaidPartnerSpread_May2_Rank2[] =
{
	{
		.species = SPECIES_COMBUSKEN,
		.nature = NATURE_JOLLY,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.atkEv = 252,
		.spdEv = 252,
		.hpEv = 4,
		.ability = FRONTIER_ABILITY_HIDDEN, //ABILITY_SPEEDBOOST
		.item = ITEM_LEFTOVERS,
		.moves =
		{
			MOVE_FIREPUNCH,
			MOVE_THUNDERPUNCH,
			MOVE_POISONJAB,
			MOVE_PROTECT,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_MARSHTOMP,
		.nature = NATURE_CAREFUL,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.atkEv = 252,
		.spDefEv = 252,
		.spdEv = 4,
		.ability = FRONTIER_ABILITY_1, //ABILITY_TORRENT
		.item = ITEM_EVIOLITE,
		.moves =
		{
			MOVE_ICEPUNCH,
			MOVE_WATERFALL,
			MOVE_EARTHQUAKE,
			MOVE_ROCKSLIDE,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_GROVYLE,
		.nature = NATURE_MODEST,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.hpEv = 252,
		.spAtkEv = 252,
		.spdEv = 4,
		.ability = FRONTIER_ABILITY_HIDDEN, //ABILITY_UNBURDEN
		.item = ITEM_SITRUS_BERRY,
		.moves =
		{
			MOVE_GIGADRAIN,
			MOVE_ENERGYBALL,
			MOVE_MUDSLAP,
			MOVE_DRAGONBREATH,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
};

static const struct BattleTowerSpread sRaidPartnerSpread_May2_Rank5[] =
{
	{
		.species = SPECIES_BLAZIKEN,
		.nature = NATURE_JOLLY,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.atkEv = 252,
		.spdEv = 252,
		.hpEv = 4,
		.ability = FRONTIER_ABILITY_HIDDEN, //ABILITY_SPEEDBOOST
		.item = ITEM_BLAZIKENITE,
		.moves =
		{
			MOVE_FLAREBLITZ,
			MOVE_THUNDERPUNCH,
			MOVE_CLOSECOMBAT,
			MOVE_PROTECT,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_SWAMPERT,
		.nature = NATURE_RELAXED,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.hpEv = 252,
		.defEv = 252,
		.atkEv = 4,
		.ability = FRONTIER_ABILITY_1, //ABILITY_TORRENT
		.item = ITEM_LEFTOVERS,
		.moves =
		{
			MOVE_EARTHQUAKE,
			MOVE_ROCKSLIDE,
			MOVE_FLIPTURN,
			MOVE_TOXIC,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_SCEPTILE,
		.nature = NATURE_MODEST,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.hpEv = 4,
		.spAtkEv = 252,
		.spdEv = 252,
		.ability = FRONTIER_ABILITY_HIDDEN, //ABILITY_UNBURDEN
		.item = ITEM_SITRUS_BERRY,
		.moves =
		{
			MOVE_GIGADRAIN,
			MOVE_ENERGYBALL,
			MOVE_FOCUSBLAST,
			MOVE_DRAGONPULSE,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
};


static const struct BattleTowerSpread sRaidPartnerSpread_May3_Rank5[] =
{
	{
		.species = SPECIES_BLAZIKEN,
		.nature = NATURE_JOLLY,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.atkEv = 252,
		.spdEv = 252,
		.hpEv = 4,
		.ability = FRONTIER_ABILITY_HIDDEN, //ABILITY_SPEEDBOOST
		.item = ITEM_LEFTOVERS,
		.moves =
		{
			MOVE_FLAREBLITZ,
			MOVE_THUNDERPUNCH,
			MOVE_CLOSECOMBAT,
			MOVE_PROTECT,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_SWAMPERT,
		.nature = NATURE_RELAXED,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.hpEv = 252,
		.defEv = 252,
		.atkEv = 4,
		.ability = FRONTIER_ABILITY_1, //ABILITY_TORRENT
		.item = ITEM_SWAMPERTITE,
		.moves =
		{
			MOVE_EARTHQUAKE,
			MOVE_ROCKSLIDE,
			MOVE_WATERFALL,
			MOVE_ICEPUNCH,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_SCEPTILE,
		.nature = NATURE_MODEST,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.hpEv = 4,
		.spAtkEv = 252,
		.spdEv = 252,
		.ability = FRONTIER_ABILITY_HIDDEN, //ABILITY_UNBURDEN
		.item = ITEM_SITRUS_BERRY,
		.moves =
		{
			MOVE_GIGADRAIN,
			MOVE_ENERGYBALL,
			MOVE_FOCUSBLAST,
			MOVE_DRAGONPULSE,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
};


static const struct BattleTowerSpread sRaidPartnerSpread_May4_Rank5[] =
{
	{
		.species = SPECIES_BLAZIKEN,
		.nature = NATURE_JOLLY,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.atkEv = 252,
		.spdEv = 252,
		.hpEv = 4,
		.ability = FRONTIER_ABILITY_HIDDEN, //ABILITY_SPEEDBOOST
		.item = ITEM_BLAZIKENITE,
		.moves =
		{
			MOVE_FLAREBLITZ,
			MOVE_THUNDERPUNCH,
			MOVE_CLOSECOMBAT,
			MOVE_PROTECT,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_SWAMPERT,
		.nature = NATURE_RELAXED,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.hpEv = 252,
		.atkEv = 252,
		.defEv = 4,
		.ability = FRONTIER_ABILITY_1, //ABILITY_TORRENT
		.item = ITEM_LEFTOVERS,
		.moves =
		{
			MOVE_EARTHQUAKE,
			MOVE_ROCKSLIDE,
			MOVE_FLIPTURN,
			MOVE_TOXIC,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_SCEPTILE,
		.nature = NATURE_MODEST,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.hpEv = 4,
		.spAtkEv = 252,
		.spdEv = 252,
		.ability = FRONTIER_ABILITY_HIDDEN, //ABILITY_UNBURDEN
		.item = ITEM_SCEPTILITE,
		.moves =
		{
			MOVE_GIGADRAIN,
			MOVE_ENERGYBALL,
			MOVE_FOCUSBLAST,
			MOVE_DRAGONPULSE,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
};


static const struct BattleTowerSpread sRaidPartnerSpread_May2_Rank6[] =
{
	{
		.species = SPECIES_LATIOS,
		.nature = NATURE_TIMID,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.spAtkEv = 252,
		.spdEv = 252,
		.spDefEv = 4,
		.ability = FRONTIER_ABILITY_1, //ABILITY_LEVITATE
		.item = ITEM_LATIOSITE,
		.moves =
		{
			MOVE_PSYCHIC,
			MOVE_DRACOMETEOR,
			MOVE_AURASPHERE,
			MOVE_ROOST,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_DEOXYS_ATTACK,
		.nature = NATURE_NAIVE,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.atkEv = 4,
		.spAtkEv = 252,
		.spdEv = 252,
		.ability = FRONTIER_ABILITY_1, //ABILITY_PRESSURE
		.item = ITEM_LIFE_ORB,
		.moves =
		{
			MOVE_PSYCHOBOOST,
			MOVE_ICEBEAM,
			MOVE_SUPERPOWER,
			MOVE_DARKPULSE,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_JIRACHI,
		.nature = NATURE_JOLLY,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.atkEv = 252,
		.spDefEv = 4,
		.spdEv = 252,
		.ability = FRONTIER_ABILITY_1, //ABILITY_SERENEGRACE
		.item = ITEM_CHOICE_SCARF,
		.moves =
		{
			MOVE_IRONHEAD,
			MOVE_UTURN,
			MOVE_ICEPUNCH,
			MOVE_ANCIENTPOWER,
		},
		.ball = BALL_TYPE_RANDOM,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
};

extern const u8 sTrainerName_Greg[];
extern const u8 sTrainerName_Tasha[];
extern const u8 sTrainerName_Catherine[];
extern const u8 sTrainerName_Ash[];
extern const u8 sTrainerName_Lyra[];
extern const u8 sTrainerName_Fox[];
extern const u8 sTrainerName_Talia[];
extern const u8 sTrainerName_Hailey[];
extern const u8 sTrainerName_Juniper[];
extern const u8 sTrainerName_Kai[];
extern const u8 sTrainerName_Skye[];
extern const u8 sTrainerName_Cynthia[];
extern const u8 sTrainerName_Mei[];
extern const u8 sTrainerName_Fernando[];
extern const u8 sTrainerName_May[];

const struct MultiRaidTrainer gRaidPartners[] =
{
	{
		.owNum = EVENT_OBJ_GFX_CRUSH_GIRL,
		.trainerClass = CLASS_PKMN_TRAINER_1,
		.backSpriteId = TRAINER_BACK_PIC_FEMALETRAINER,
		.gender = FEMALE,
		.otId = 0x87116209,
		.name = sTrainerName_Catherine,
		.spreads =
		{
			[ONE_STAR_RAID ... THREE_STAR_RAID] = sRaidPartnerSpread_Catherine_Rank2,
			[FOUR_STAR_RAID ... FIVE_STAR_RAID] = sRaidPartnerSpread_Catherine_Rank5,
			[SIX_STAR_RAID] = sRaidPartnerSpread_Catherine_Rank6,
		},
		.spreadSizes =
		{
			[ONE_STAR_RAID ... THREE_STAR_RAID] = NELEMS(sRaidPartnerSpread_Catherine_Rank2),
		    [FOUR_STAR_RAID ... FIVE_STAR_RAID] = NELEMS(sRaidPartnerSpread_Catherine_Rank5),
			[SIX_STAR_RAID] = NELEMS(sRaidPartnerSpread_Catherine_Rank6),
		},
	},
	{
		.owNum = EVENT_OBJ_GFX_LASS,
		.trainerClass = CLASS_PKMN_TRAINER_1,
		.backSpriteId = TRAINER_BACK_PIC_LYRA,
		.gender = FEMALE,
		.otId = 0x87116209,
		.name = sTrainerName_Tasha,
		.spreads =
		{
			[ONE_STAR_RAID ... THREE_STAR_RAID] = sRaidPartnerSpread_Tasha_Rank2,
			[FOUR_STAR_RAID ... FIVE_STAR_RAID] = sRaidPartnerSpread_Tasha_Rank5,
			[SIX_STAR_RAID] = sRaidPartnerSpread_Tasha_Rank6,

		},
		.spreadSizes =
		{
			[ONE_STAR_RAID ... THREE_STAR_RAID] = NELEMS(sRaidPartnerSpread_Tasha_Rank2),
			[FOUR_STAR_RAID ... FIVE_STAR_RAID] = NELEMS(sRaidPartnerSpread_Tasha_Rank5),
			[SIX_STAR_RAID] = NELEMS(sRaidPartnerSpread_Tasha_Rank6),
		},
	},
	{
		.owNum = EVENT_OBJ_GFX_LYRA,
		.trainerClass = CLASS_PKMN_TRAINER_1,
		.backSpriteId = TRAINER_BACK_PIC_LYRA,
		.gender = FEMALE,
		.otId = 0x87116209,
		.name = sTrainerName_Lyra,
		.spreads =
		{
			[ONE_STAR_RAID ... THREE_STAR_RAID] = sRaidPartnerSpread_Lyra_Rank2,
			[FOUR_STAR_RAID ... SIX_STAR_RAID] = sRaidPartnerSpread_Lyra_Rank5,
		},
		.spreadSizes =
		{
			[ONE_STAR_RAID ... THREE_STAR_RAID] = NELEMS(sRaidPartnerSpread_Lyra_Rank2),
			[FOUR_STAR_RAID ... SIX_STAR_RAID] = NELEMS(sRaidPartnerSpread_Lyra_Rank5),
		},
	},
	{
		.owNum = EVENT_OBJ_GFX_ASH,
		.trainerClass = CLASS_PKMN_TRAINER_1,
		.backSpriteId = TRAINER_BACK_PIC_ASH,
		.gender = MALE,
		.otId = 0x87116209,
		.name = sTrainerName_Ash,
		.spreads =
		{
			[ONE_STAR_RAID ... THREE_STAR_RAID] = sRaidPartnerSpread_Ash_Rank2,
			[FOUR_STAR_RAID ... SIX_STAR_RAID] = sRaidPartnerSpread_Ash_Rank5,
		},
		.spreadSizes =
		{
			[ONE_STAR_RAID ... THREE_STAR_RAID] = NELEMS(sRaidPartnerSpread_Ash_Rank2),
			[FOUR_STAR_RAID ... SIX_STAR_RAID] = NELEMS(sRaidPartnerSpread_Ash_Rank5),
		},
	},
	{
		.owNum = EVENT_OBJ_GFX_OLD_MAN,
		.trainerClass = CLASS_PKMN_TRAINER_1,
		.backSpriteId = TRAINER_BACK_PIC_OLD_MAN,
		.gender = MALE,
		.otId = 0x87116209,
		.name = sTrainerName_Greg,
		.spreads =
		{
			[ONE_STAR_RAID ... THREE_STAR_RAID] = sRaidPartnerSpread_Red_Rank2,
			[FOUR_STAR_RAID ... FIVE_STAR_RAID] = sRaidPartnerSpread_Red_Rank5,
			[SIX_STAR_RAID] = sRaidPartnerSpread_Red_Rank6,
		},
		.spreadSizes =
		{
			[ONE_STAR_RAID ... THREE_STAR_RAID] = NELEMS(sRaidPartnerSpread_Red_Rank2),
			[FOUR_STAR_RAID ... FIVE_STAR_RAID] = NELEMS(sRaidPartnerSpread_Red_Rank5),
			[SIX_STAR_RAID] = NELEMS(sRaidPartnerSpread_Red_Rank6),
		},
	},
	{
		.owNum = EVENT_OBJ_GFX_COOLTRAINER_M,
		.trainerClass = CLASS_PKMN_TRAINER_1,
		.backSpriteId = TRAINER_BACK_YOUNG_TRAINER_MALE,
		.gender = MALE,
		.otId = 0x87116209,
		.name = sTrainerName_Fox,
		.spreads =
		{
			[ONE_STAR_RAID ... THREE_STAR_RAID] = sRaidPartnerSpread_Fox_Rank2,
			[FOUR_STAR_RAID ... FIVE_STAR_RAID] = sRaidPartnerSpread_Fox_Rank5,
			[SIX_STAR_RAID] = sRaidPartnerSpread_Fox_Rank6,
		},
		.spreadSizes =
		{
			[ONE_STAR_RAID ... THREE_STAR_RAID] = NELEMS(sRaidPartnerSpread_Fox_Rank2),
			[FOUR_STAR_RAID ... FIVE_STAR_RAID] = NELEMS(sRaidPartnerSpread_Fox_Rank5),
			[SIX_STAR_RAID] = NELEMS(sRaidPartnerSpread_Fox_Rank6),
		},
	},
	{
		.owNum = EVENT_OBJ_GFX_LITTLE_GIRL,
		.trainerClass = CLASS_PKMN_TRAINER_1,
		.backSpriteId = TRAINER_BACK_PIC_YOUNG_TRAINER_FEMALE,
		.gender = FEMALE,
		.otId = 0x87116209,
		.name = sTrainerName_Talia,
		.spreads =
		{
			[ONE_STAR_RAID ... THREE_STAR_RAID] = sRaidPartnerSpread_Talia_Rank2,
			[FOUR_STAR_RAID ... FIVE_STAR_RAID] = sRaidPartnerSpread_Talia_Rank5,
			[SIX_STAR_RAID] = sRaidPartnerSpread_Talia_Rank6,
		},
		.spreadSizes =
		{
			[ONE_STAR_RAID ... THREE_STAR_RAID] = NELEMS(sRaidPartnerSpread_Talia_Rank2),
			[FOUR_STAR_RAID ... FIVE_STAR_RAID] = NELEMS(sRaidPartnerSpread_Talia_Rank5),
			[SIX_STAR_RAID] = NELEMS(sRaidPartnerSpread_Talia_Rank6),
		},
	},
	{
		.owNum = EVENT_OBJ_GFX_LITTLE_GIRL,
		.trainerClass = CLASS_PKMN_TRAINER_1,
		.backSpriteId = TRAINER_BACK_PIC_YOUNG_TRAINER_FEMALE,
		.gender = FEMALE,
		.otId = 0x87116209,
		.name = sTrainerName_Hailey,
		.spreads =
		{
			[ONE_STAR_RAID ... THREE_STAR_RAID] = sRaidPartnerSpread_Hailey_Rank2,
			[FOUR_STAR_RAID ... FIVE_STAR_RAID] = sRaidPartnerSpread_Hailey_Rank5,
			[SIX_STAR_RAID] = sRaidPartnerSpread_Hailey_Rank6,
		},
		.spreadSizes =
		{
			[ONE_STAR_RAID ... THREE_STAR_RAID] = NELEMS(sRaidPartnerSpread_Hailey_Rank2),
			[FOUR_STAR_RAID ... FIVE_STAR_RAID] = NELEMS(sRaidPartnerSpread_Hailey_Rank5),
			[SIX_STAR_RAID] = NELEMS(sRaidPartnerSpread_Hailey_Rank6),
		},
	},
	{
		.owNum = 227,
		.trainerClass = CLASS_PKMN_TRAINER_1,
		.backSpriteId = TRAINER_BACK_PIC_JUNIPER,
		.gender = FEMALE,
		.otId = 0x87116209,
		.name = sTrainerName_Juniper,
		.spreads =
		{
			[ONE_STAR_RAID ... THREE_STAR_RAID] = sRaidPartnerSpread_Juniper_Rank2,
			[FOUR_STAR_RAID ... FIVE_STAR_RAID] = sRaidPartnerSpread_Juniper_Rank5,
			[SIX_STAR_RAID] = sRaidPartnerSpread_Juniper_Rank6,
		},
		.spreadSizes =
		{
			[ONE_STAR_RAID ... THREE_STAR_RAID] = NELEMS(sRaidPartnerSpread_Juniper_Rank2),
			[FOUR_STAR_RAID ... FIVE_STAR_RAID] = NELEMS(sRaidPartnerSpread_Juniper_Rank5),
			[SIX_STAR_RAID] = NELEMS(sRaidPartnerSpread_Juniper_Rank6),
		},
	},
	{
		.owNum = 227,
		.trainerClass = CLASS_PKMN_TRAINER_1,
		.backSpriteId = TRAINER_BACK_PIC_JUNIPER,
		.gender = FEMALE,
		.otId = 0x87116209,
		.name = sTrainerName_Juniper,
		.spreads =
		{
			[ONE_STAR_RAID ... THREE_STAR_RAID] = sRaidPartnerSpread_Juniper2_Rank2,
			[FOUR_STAR_RAID ... FIVE_STAR_RAID] = sRaidPartnerSpread_Juniper2_Rank5,
			[SIX_STAR_RAID] = sRaidPartnerSpread_Juniper2_Rank6,
		},
		.spreadSizes =
		{
			[ONE_STAR_RAID ... THREE_STAR_RAID] = NELEMS(sRaidPartnerSpread_Juniper2_Rank2),
			[FOUR_STAR_RAID ... FIVE_STAR_RAID] = NELEMS(sRaidPartnerSpread_Juniper2_Rank5),
			[SIX_STAR_RAID] = NELEMS(sRaidPartnerSpread_Juniper2_Rank6),
		},
	},
	{
		.owNum = EVENT_OBJ_GFX_YOUNGSTER,
		.trainerClass = CLASS_PKMN_TRAINER_1,
		.backSpriteId = TRAINER_BACK_PIC_HILBERT,
		.gender = MALE,
		.otId = 0x87116209,
		.name = sTrainerName_Kai,
		.spreads =
		{
			[ONE_STAR_RAID ... THREE_STAR_RAID] = sRaidPartnerSpread_Kai_Rank2,
			[FOUR_STAR_RAID ... FIVE_STAR_RAID] = sRaidPartnerSpread_Kai_Rank5,
			[SIX_STAR_RAID] = sRaidPartnerSpread_Kai_Rank6,
		},
		.spreadSizes =
		{
			[ONE_STAR_RAID ... THREE_STAR_RAID] = NELEMS(sRaidPartnerSpread_Kai_Rank2),
			[FOUR_STAR_RAID ... FIVE_STAR_RAID] = NELEMS(sRaidPartnerSpread_Kai_Rank5),
			[SIX_STAR_RAID] = NELEMS(sRaidPartnerSpread_Kai_Rank6),
		},
	},
	{
		.owNum = EVENT_OBJ_GFX_YOUNGSTER,
		.trainerClass = CLASS_PKMN_TRAINER_1,
		.backSpriteId = TRAINER_BACK_PIC_NATE,
		.gender = MALE,
		.otId = 0x87116209,
		.name = sTrainerName_Skye,
		.spreads =
		{
			[ONE_STAR_RAID ... THREE_STAR_RAID] = sRaidPartnerSpread_Skye_Rank2,
			[FOUR_STAR_RAID ... FIVE_STAR_RAID] = sRaidPartnerSpread_Skye_Rank5,
			[SIX_STAR_RAID] = sRaidPartnerSpread_Skye_Rank6,
		},
		.spreadSizes =
		{
			[ONE_STAR_RAID ... THREE_STAR_RAID] = NELEMS(sRaidPartnerSpread_Skye_Rank2),
			[FOUR_STAR_RAID ... FIVE_STAR_RAID] = NELEMS(sRaidPartnerSpread_Skye_Rank5),
			[SIX_STAR_RAID] = NELEMS(sRaidPartnerSpread_Skye_Rank6),
		},
	},
	{
		.owNum = 235,
		.trainerClass = CLASS_PKMN_TRAINER_1,
		.backSpriteId = TRAINER_BACK_PIC_CYNTHIA,
		.gender = FEMALE,
		.otId = 0x87116209,
		.name = sTrainerName_Cynthia,
		.spreads =
		{
			[ONE_STAR_RAID ... THREE_STAR_RAID] = sRaidPartnerSpread_Cynthia_Rank2,
			[FOUR_STAR_RAID ... FIVE_STAR_RAID] = sRaidPartnerSpread_Cynthia_Rank5,
			[SIX_STAR_RAID] = sRaidPartnerSpread_Cynthia_Rank6,
		},
		.spreadSizes =
		{
			[ONE_STAR_RAID ... THREE_STAR_RAID] = NELEMS(sRaidPartnerSpread_Cynthia_Rank2),
			[FOUR_STAR_RAID ... FIVE_STAR_RAID] = NELEMS(sRaidPartnerSpread_Cynthia_Rank5),
			[SIX_STAR_RAID] = NELEMS(sRaidPartnerSpread_Cynthia_Rank6),
		},
	},
	{
		.owNum = 235,
		.trainerClass = CLASS_PKMN_TRAINER_1,
		.backSpriteId = TRAINER_BACK_PIC_CYNTHIA,
		.gender = FEMALE,
		.otId = 0x87116209,
		.name = sTrainerName_Cynthia,
		.spreads =
		{
			[ONE_STAR_RAID ... THREE_STAR_RAID] = sRaidPartnerSpread_Cynthia2_Rank2,
			[FOUR_STAR_RAID ... FIVE_STAR_RAID] = sRaidPartnerSpread_Cynthia2_Rank5,
			[SIX_STAR_RAID] = sRaidPartnerSpread_Cynthia2_Rank6,
		},
		.spreadSizes =
		{
			[ONE_STAR_RAID ... THREE_STAR_RAID] = NELEMS(sRaidPartnerSpread_Cynthia2_Rank2),
			[FOUR_STAR_RAID ... FIVE_STAR_RAID] = NELEMS(sRaidPartnerSpread_Cynthia2_Rank5),
			[SIX_STAR_RAID] = NELEMS(sRaidPartnerSpread_Cynthia2_Rank6),
		},
	},
	{
		.owNum = EVENT_OBJ_GFX_LASS,
		.trainerClass = CLASS_PKMN_TRAINER_1,
		.backSpriteId = TRAINER_BACK_PIC_FEMALETRAINER,
		.gender = FEMALE,
		.otId = 0x87116209,
		.name = sTrainerName_Mei,
		.spreads =
		{
			[ONE_STAR_RAID ... THREE_STAR_RAID] = sRaidPartnerSpread_Mei_Rank2,
			[FOUR_STAR_RAID ... FIVE_STAR_RAID] = sRaidPartnerSpread_Mei_Rank5,
			[SIX_STAR_RAID] = sRaidPartnerSpread_Mei_Rank6,
		},
		.spreadSizes =
		{
			[ONE_STAR_RAID ... THREE_STAR_RAID] = NELEMS(sRaidPartnerSpread_Mei_Rank2),
			[FOUR_STAR_RAID ... FIVE_STAR_RAID] = NELEMS(sRaidPartnerSpread_Mei_Rank5),
			[SIX_STAR_RAID] = NELEMS(sRaidPartnerSpread_Mei_Rank6),
		},
	},
	{
		.owNum = EVENT_OBJ_GFX_COOLTRAINER_M,
		.trainerClass = CLASS_PKMN_TRAINER_1,
		.backSpriteId = TRAINER_BACK_YOUNG_TRAINER_MALE,
		.gender = MALE,
		.otId = 0x87116209,
		.name = sTrainerName_Fernando,
		.spreads =
		{
			[ONE_STAR_RAID ... THREE_STAR_RAID] = sRaidPartnerSpread_Fernando_Rank2,
			[FOUR_STAR_RAID ... FIVE_STAR_RAID] = sRaidPartnerSpread_Fernando_Rank5,
			[SIX_STAR_RAID] = sRaidPartnerSpread_Fernando_Rank6,
		},
		.spreadSizes =
		{
			[ONE_STAR_RAID ... THREE_STAR_RAID] = NELEMS(sRaidPartnerSpread_Fernando_Rank2),
			[FOUR_STAR_RAID ... FIVE_STAR_RAID] = NELEMS(sRaidPartnerSpread_Fernando_Rank5),
			[SIX_STAR_RAID] = NELEMS(sRaidPartnerSpread_Fernando_Rank6),
		},
	},
	{
		.owNum = EVENT_OBJ_GFX_MAY,
		.trainerClass = CLASS_PKMN_TRAINER_1,
		.backSpriteId = TRAINER_BACK_PIC_MAY,
		.gender = FEMALE,
		.otId = 0x87116209,
		.name = sTrainerName_May,
		.spreads =
		{
			[ONE_STAR_RAID ... THREE_STAR_RAID] = sRaidPartnerSpread_May_Rank2,
			[FOUR_STAR_RAID ... FIVE_STAR_RAID] = sRaidPartnerSpread_May_Rank5,
			[SIX_STAR_RAID] = sRaidPartnerSpread_May_Rank6,
		},
		.spreadSizes =
		{
			[ONE_STAR_RAID ... THREE_STAR_RAID] = NELEMS(sRaidPartnerSpread_May_Rank2),
			[FOUR_STAR_RAID ... FIVE_STAR_RAID] = NELEMS(sRaidPartnerSpread_May_Rank5),
			[SIX_STAR_RAID] = NELEMS(sRaidPartnerSpread_May_Rank6),
		},
	},
	{
		.owNum = EVENT_OBJ_GFX_MAY,
		.trainerClass = CLASS_PKMN_TRAINER_1,
		.backSpriteId = TRAINER_BACK_PIC_MAY,
		.gender = FEMALE,
		.otId = 0x87116209,
		.name = sTrainerName_May,
		.spreads =
		{
			[ONE_STAR_RAID ... THREE_STAR_RAID] = sRaidPartnerSpread_May2_Rank2,
			[FOUR_STAR_RAID ... FIVE_STAR_RAID] = sRaidPartnerSpread_May2_Rank5,
			[SIX_STAR_RAID] = sRaidPartnerSpread_May2_Rank6,
		},
		.spreadSizes =
		{
			[ONE_STAR_RAID ... THREE_STAR_RAID] = NELEMS(sRaidPartnerSpread_May2_Rank2),
			[FOUR_STAR_RAID ... FIVE_STAR_RAID] = NELEMS(sRaidPartnerSpread_May2_Rank5),
			[SIX_STAR_RAID] = NELEMS(sRaidPartnerSpread_May2_Rank6),
		},
	},
};

#endif

const u8 gNumRaidPartners = NELEMS(gRaidPartners);
