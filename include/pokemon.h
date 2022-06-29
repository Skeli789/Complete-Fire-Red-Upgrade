#pragma once

#include "global.h"

#define MON_DATA_PERSONALITY        0
#define MON_DATA_OT_ID              1
#define MON_DATA_NICKNAME           2
#define MON_DATA_LANGUAGE           3
#define MON_DATA_SANITY_IS_BAD_EGG  4
#define MON_DATA_SANITY_HAS_SPECIES 5
#define MON_DATA_SANITY_IS_EGG      6
#define MON_DATA_OT_NAME            7
#define MON_DATA_MARKINGS           8
#define MON_DATA_CHECKSUM           9
#define MON_DATA_ENCRYPT_SEPARATOR 10
#define MON_DATA_SPECIES           11
#define MON_DATA_HELD_ITEM         12
#define MON_DATA_MOVE1             13
#define MON_DATA_MOVE2             14
#define MON_DATA_MOVE3             15
#define MON_DATA_MOVE4             16
#define MON_DATA_PP1               17
#define MON_DATA_PP2               18
#define MON_DATA_PP3               19
#define MON_DATA_PP4               20
#define MON_DATA_PP_BONUSES        21
#define MON_DATA_COOL              22
#define MON_DATA_BEAUTY            23
#define MON_DATA_CUTE              24
#define MON_DATA_EXP               25
#define MON_DATA_HP_EV             26
#define MON_DATA_ATK_EV            27
#define MON_DATA_DEF_EV            28
#define MON_DATA_SPEED_EV          29
#define MON_DATA_SPATK_EV          30
#define MON_DATA_SPDEF_EV          31
#define MON_DATA_FRIENDSHIP        32
#define MON_DATA_SMART             33
#define MON_DATA_POKERUS           34
#define MON_DATA_MET_LOCATION      35
#define MON_DATA_MET_LEVEL         36
#define MON_DATA_MET_GAME          37
#define MON_DATA_POKEBALL          38
#define MON_DATA_HP_IV             39
#define MON_DATA_ATK_IV            40
#define MON_DATA_DEF_IV            41
#define MON_DATA_SPEED_IV          42
#define MON_DATA_SPATK_IV          43
#define MON_DATA_SPDEF_IV          44
#define MON_DATA_IS_EGG            45
#define MON_DATA_ALT_ABILITY       46
#define MON_DATA_TOUGH             47
#define MON_DATA_SHEEN             48
#define MON_DATA_OT_GENDER         49
#define MON_DATA_COOL_RIBBON       50
#define MON_DATA_BEAUTY_RIBBON     51
#define MON_DATA_CUTE_RIBBON       52
#define MON_DATA_SMART_RIBBON      53
#define MON_DATA_TOUGH_RIBBON      54
#define MON_DATA_STATUS            55
#define MON_DATA_LEVEL             56
#define MON_DATA_HP                57
#define MON_DATA_MAX_HP            58
#define MON_DATA_ATK               59
#define MON_DATA_DEF               60
#define MON_DATA_SPEED             61
#define MON_DATA_SPATK             62
#define MON_DATA_SPDEF             63
#define MON_DATA_MAIL              64
#define MON_DATA_SPECIES2          65
#define MON_DATA_IVS               66
#define MON_DATA_CHAMPION_RIBBON   67
#define MON_DATA_WINNING_RIBBON    68
#define MON_DATA_VICTORY_RIBBON    69
#define MON_DATA_ARTIST_RIBBON     70
#define MON_DATA_EFFORT_RIBBON     71
#define MON_DATA_GIFT_RIBBON_1     72
#define MON_DATA_GIFT_RIBBON_2     73
#define MON_DATA_GIFT_RIBBON_3     74
#define MON_DATA_GIFT_RIBBON_4     75
#define MON_DATA_GIFT_RIBBON_5     76
#define MON_DATA_GIFT_RIBBON_6     77
#define MON_DATA_GIFT_RIBBON_7     78
#define MON_DATA_FATEFUL_ENCOUNTER 79
#define MON_DATA_OBEDIENCE         80
#define MON_DATA_KNOWN_MOVES       81
#define MON_DATA_RIBBON_COUNT      82
#define MON_DATA_RIBBONS           83
#define MON_DATA_ATK2              84
#define MON_DATA_DEF2              85
#define MON_DATA_SPEED2            86
#define MON_DATA_SPATK2            87
#define MON_DATA_SPDEF2            88

#define OT_ID_RANDOM_NO_SHINY 2
#define OT_ID_PRESET 1
#define OT_ID_PLAYER_ID 0

#define MON_GIVEN_TO_PARTY      0x0
#define MON_GIVEN_TO_PC         0x1
#define MON_CANT_GIVE           0x2

#define PLAYER_HAS_TWO_USABLE_MONS              0x0
#define PLAYER_HAS_ONE_MON                      0x1
#define PLAYER_HAS_ONE_USABLE_MON               0x2

#define MON_MALE       0x00
#define MON_FEMALE     0xFE
#define MON_GENDERLESS 0xFF

#define TYPE_NORMAL   0x00
#define TYPE_FIGHTING 0x01
#define TYPE_FLYING   0x02
#define TYPE_POISON   0x03
#define TYPE_GROUND   0x04
#define TYPE_ROCK     0x05
#define TYPE_BUG      0x06
#define TYPE_GHOST    0x07
#define TYPE_STEEL    0x08
#define TYPE_MYSTERY  0x09
#define TYPE_FIRE     0x0a
#define TYPE_WATER    0x0b
#define TYPE_GRASS    0x0c
#define TYPE_ELECTRIC 0x0d
#define TYPE_PSYCHIC  0x0e
#define TYPE_ICE      0x0f
#define TYPE_DRAGON   0x10
#define TYPE_DARK     0x11
#define TYPE_ROOSTLESS 0x13
#define TYPE_BLANK	  0x14
#define TYPE_FAIRY	  0x17

#define NUMBER_OF_MON_TYPES (TYPE_FAIRY + 1)
#define NUM_HIDDEN_POWER_TYPES 16

#define PARTY_SIZE 6
#define MAX_TOTAL_EVS 510
#define NUM_STATS 6
#define UNOWN_FORM_COUNT 28
#define MAX_MON_LEVEL 100

#define MAX_MON_MOVES 4

#define MAX_FRIENDSHIP 255

enum
{
    EGG_GROUP_NONE,
    EGG_GROUP_MONSTER,
    EGG_GROUP_WATER_1,
    EGG_GROUP_BUG,
    EGG_GROUP_FLYING,
    EGG_GROUP_FIELD,
    EGG_GROUP_FAIRY,
    EGG_GROUP_GRASS,
    EGG_GROUP_HUMAN_LIKE,
    EGG_GROUP_WATER_3,
    EGG_GROUP_MINERAL,
    EGG_GROUP_AMORPHOUS,
    EGG_GROUP_WATER_2,
    EGG_GROUP_DITTO,
    EGG_GROUP_DRAGON,
    EGG_GROUP_UNDISCOVERED
};

enum
{
    NATURE_HARDY,
    NATURE_LONELY,
    NATURE_BRAVE,
    NATURE_ADAMANT,
    NATURE_NAUGHTY,
    NATURE_BOLD,
    NATURE_DOCILE,
    NATURE_RELAXED,
    NATURE_IMPISH,
    NATURE_LAX,
    NATURE_TIMID,
    NATURE_HASTY,
    NATURE_SERIOUS,
    NATURE_JOLLY,
    NATURE_NAIVE,
    NATURE_MODEST,
    NATURE_MILD,
    NATURE_QUIET,
    NATURE_BASHFUL,
    NATURE_RASH,
    NATURE_CALM,
    NATURE_GENTLE,
    NATURE_SASSY,
    NATURE_CAREFUL,
    NATURE_QUIRKY,
	NUM_NATURES,
};

struct PokemonSubstruct0
{
    u16 species;
    u16 heldItem;
    u32 experience;
    u8 ppBonuses;
    u8 friendship;
	u8 pokeball;
	u8 unknown;
};

struct PokemonSubstruct1
{
    u16 moves[4];
    u8 pp[4];
};

struct PokemonSubstruct2
{
    u8 hpEV;
    u8 attackEV;
    u8 defenseEV;
    u8 speedEV;
    u8 spAttackEV;
    u8 spDefenseEV;
    u8 cool;
    u8 beauty;
    u8 cute;
    u8 smart;
    u8 tough;
    u8 sheen;
};

struct PokemonSubstruct3
{
 /* 0x00 */ u8 pokerusDays : 4;
 /* 0x00 */ u8 pokerusStrain : 4;

 /* 0x01 */	u8 metLocation;

 /* 0x02 */ u16 metLevel:7;
 /* 0x02 */ u16 metGame:4;
 /* 0x03 */ u16 gigantamax:1;
 /* 0x03 */ u16 blank:3;
 /* 0x03 */ u16 otGender:1;

 /* 0x04 */ u32 hpIV:5;
 /* 0x04 */ u32 attackIV:5;
 /* 0x05 */ u32 defenseIV:5;
 /* 0x05 */ u32 speedIV:5;
 /* 0x05 */ u32 spAttackIV:5;
 /* 0x06 */ u32 spDefenseIV:5;
 /* 0x07 */ u32 isEgg:1;
 /* 0x07 */ u32 hiddenAbility:1;

 /* 0x08 */ u32 coolRibbon:3;
 /* 0x08 */ u32 beautyRibbon:3;
 /* 0x08 */ u32 cuteRibbon:3;
 /* 0x09 */ u32 smartRibbon:3;
 /* 0x09 */ u32 toughRibbon:3;
 /* 0x09 */ u32 championRibbon:1;
 /* 0x0A */ u32 winningRibbon:1;
 /* 0x0A */ u32 victoryRibbon:1;
 /* 0x0A */ u32 artistRibbon:1;
 /* 0x0A */ u32 effortRibbon:1;
 /* 0x0A */ u32 giftRibbon1:1;
 /* 0x0A */ u32 giftRibbon2:1;
 /* 0x0A */ u32 giftRibbon3:1;
 /* 0x0A */ u32 giftRibbon4:1;
 /* 0x0B */ u32 giftRibbon5:1;
 /* 0x0B */ u32 giftRibbon6:1;
 /* 0x0B */ u32 giftRibbon7:1;
 /* 0x0B */ u32 fatefulEncounter:4;
 /* 0x0B */ u32 obedient:1;
};

union PokemonSubstruct
{
    struct PokemonSubstruct0 type0;
    struct PokemonSubstruct1 type1;
    struct PokemonSubstruct2 type2;
    struct PokemonSubstruct3 type3;
    u16 raw[6];
};

struct BoxPokemon
{
    u32 personality;
    u32 otId;
    u8 nickname[POKEMON_NAME_LENGTH];
    u8 language;
    u8 isBadEgg:1;
    u8 hasSpecies:1;
    u8 isEgg:1;
    u8 unused:5;
    u8 otName[OT_NAME_LENGTH];
    u8 markings;
    u16 checksum;
    u16 unknown;

    struct PokemonSubstruct0 substruct0;
    struct PokemonSubstruct1 substruct1;
    struct PokemonSubstruct2 substruct2;
    struct PokemonSubstruct3 substruct3;
};

typedef struct Pokemon
{
	u32 personality;	//0x0
	u32 otid;			//0x4
	u8 nickname[10];	//0x8
	u8 language;		//0x12
	u8 sanity;			//0x13
	u8 otname[7];
	u8 markings;
	u16 backupSpecies;
	u16 unused;

//Substructure Growth
    u16 species;
    u16 item;
    u32 experience;
    u8 ppBonuses;
    u8 friendship;
	u8 pokeball; //This is new
    u8 unknown;

//Substructure Attacks
    u16 moves[4];
    u8 pp[4];

//Substructure Condition
    u8 hpEv;
    u8 atkEv;
    u8 defEv;
    u8 spdEv;
    u8 spAtkEv;
    u8 spDefEv;
    u8 coolness;
    u8 beauty;
    u8 cuteness;
    u8 smartness;
    u8 toughness;
    u8 feel;

//Substructure Misc
 /* 0x00 */ u8 pokerusDays : 4;
 /* 0x00 */ u8 pokerusStrain : 4;

 /* 0x01 */	u8 metLocation;

 /* 0x02 */ u16 metLevel:7;
 /* 0x02 */ u16 metGame:4;
 /* 0x03 */ u16 gigantamax:1;
 /* 0x03 */ u16 blank:3;
 /* 0x03 */ u16 otGender:1;

 /* 0x04 */ u32 hpIV:5;
 /* 0x04 */ u32 attackIV:5;
 /* 0x05 */ u32 defenseIV:5;
 /* 0x05 */ u32 speedIV:5;
 /* 0x05 */ u32 spAttackIV:5;
 /* 0x06 */ u32 spDefenseIV:5;
 /* 0x07 */ u32 isEgg:1;
 /* 0x07 */ u32 hiddenAbility:1;

 /* 0x08 */ u32 coolRibbon:3;
 /* 0x08 */ u32 beautyRibbon:3;
 /* 0x08 */ u32 cuteRibbon:3;
 /* 0x09 */ u32 smartRibbon:3;
 /* 0x09 */ u32 toughRibbon:3;
 /* 0x09 */ u32 championRibbon:1;
 /* 0x0A */ u32 winningRibbon:1;
 /* 0x0A */ u32 victoryRibbon:1;
 /* 0x0A */ u32 artistRibbon:1;
 /* 0x0A */ u32 effortRibbon:1;
 /* 0x0A */ u32 giftRibbon1:1;
 /* 0x0A */ u32 giftRibbon2:1;
 /* 0x0A */ u32 giftRibbon3:1;
 /* 0x0A */ u32 giftRibbon4:1;
 /* 0x0B */ u32 giftRibbon5:1;
 /* 0x0B */ u32 giftRibbon6:1;
 /* 0x0B */ u32 giftRibbon7:1;
 /* 0x0B */ u32 fatefulEncounter:4;
 /* 0x0B */ u32 obedient:1;

	u32 condition;
	u8 level;
	u8 pokerus_timer;
	u16 hp;
	u16 maxHP;
	u16 attack;
	u16 defense;
	u16 speed;
	u16 spAttack;
	u16 spDefense;
} pokemon_t;

struct SaveBlock3
{
    /*0x0000*/ u8 currentBox;
	/*0x1-03*/ u8 gap[3];
    /*0x0004*/ struct BoxPokemon boxes[14][30];
    /*0x8344*/ u8 boxNames[14][9];
    /*0x83C2*/ u8 boxWallpapers[14];
};

struct UnknownPokemonStruct
{
    u16 species;
    u16 heldItem;
    u16 moves[4];
    u8 level;
    u8 ppBonuses;
    u8 hpEV;
    u8 attackEV;
    u8 defenseEV;
    u8 speedEV;
    u8 spAttackEV;
    u8 spDefenseEV;
    u32 otId;
    u32 hpIV:5;
    u32 attackIV:5;
    u32 defenseIV:5;
    u32 speedIV:5;
    u32 spAttackIV:5;
    u32 spDefenseIV:5;
    u32 gap:1;
    u32 altAbility:1;
    u32 personality;
    u8 nickname[POKEMON_NAME_LENGTH + 1];
    u8 friendship;
};

#define BATTLE_STATS_NO 8

struct BattlePokemon
{
    /*0x00*/ u16 species;
    /*0x02*/ u16 attack;
    /*0x04*/ u16 defense;
    /*0x06*/ u16 speed;
    /*0x08*/ u16 spAttack;
    /*0x0A*/ u16 spDefense;
    /*0x0C*/ u16 moves[4];
    /*0x14*/ u32 hpIV:5;
    /*0x14*/ u32 attackIV:5;
    /*0x15*/ u32 defenseIV:5;
    /*0x15*/ u32 speedIV:5;
    /*0x16*/ u32 spAttackIV:5;
    /*0x17*/ u32 spDefenseIV:5;
    /*0x17*/ u32 isEgg:1;
    /*0x17*/ u32 altAbility:1;
	/*0x18*/ u8 type3;
    /*0x19*/ s8 statStages[BATTLE_STATS_NO-1];
    /*0x20*/ u8 ability;
    /*0x21*/ u8 type1;
    /*0x22*/ u8 type2;
    /*0x23*/ u8 unknown;
    /*0x24*/ u8 pp[4];
    /*0x28*/ u16 hp;
    /*0x2A*/ u8 level;
    /*0x2B*/ u8 friendship;
    /*0x2C*/ u16 maxHP;
    /*0x2E*/ u16 item;
    /*0x30*/ u8 nickname[POKEMON_NAME_LENGTH + 1];
    /*0x3B*/ u8 ppBonuses;
    /*0x3C*/ u8 otName[8];
    /*0x44*/ u32 experience;
    /*0x48*/ u32 personality;
    /*0x4C*/ u32 status1;
    /*0x50*/ u32 status2;
    /*0x54*/ u32 otId;
};

enum
{
    STAT_STAGE_HP,       // 0
    STAT_STAGE_ATK,      // 1
    STAT_STAGE_DEF,      // 2
    STAT_STAGE_SPEED,    // 3
    STAT_STAGE_SPATK,    // 4
    STAT_STAGE_SPDEF,    // 5
    STAT_STAGE_ACC,      // 6
    STAT_STAGE_EVASION,  // 7
};

#define STAT_STAGE_MIN 0
#define STAT_STAGE_MAX 12

#define INCREASE_1 0x10
#define INCREASE_2 0x20
#define INCREASE_3 0x30
#define INCREASE_4 0x40
#define INCREASE_5 0x50
#define INCREASE_6 0x60

#define DECREASE_1 0x90
#define DECREASE_2 0xA0
#define DECREASE_3 0xB0
#define DECREASE_4 0xC0
#define DECREASE_5 0xD0
#define DECREASE_6 0xE0

enum
{
    STAT_HP,     // 0
    STAT_ATK,    // 1
    STAT_DEF,    // 2
    STAT_SPD,    // 3
    STAT_SPATK,  // 4
    STAT_SPDEF,  // 5
};

struct BaseStats
{
 /* 0x00 */ u8 baseHP;
 /* 0x01 */ u8 baseAttack;
 /* 0x02 */ u8 baseDefense;
 /* 0x03 */ u8 baseSpeed;
 /* 0x04 */ u8 baseSpAttack;
 /* 0x05 */ u8 baseSpDefense;
 /* 0x06 */ u8 type1;
 /* 0x07 */ u8 type2;
 /* 0x08 */ u8 catchRate;
 /* 0x09 */ u8 expYield;
 /* 0x0A */ u16 evYield_HP:2;
 /* 0x0A */ u16 evYield_Attack:2;
 /* 0x0A */ u16 evYield_Defense:2;
 /* 0x0A */ u16 evYield_Speed:2;
 /* 0x0B */ u16 evYield_SpAttack:2;
 /* 0x0B */ u16 evYield_SpDefense:2;
 /* 0x0C */ u16 item1;
 /* 0x0E */ u16 item2;
 /* 0x10 */ u8 genderRatio;
 /* 0x11 */ u8 eggCycles;
 /* 0x12 */ u8 friendship;
 /* 0x13 */ u8 growthRate;
 /* 0x14 */ u8 eggGroup1;
 /* 0x15 */ u8 eggGroup2;
 /* 0x16 */ u8 ability1;
 /* 0x17 */ u8 ability2;
 /* 0x18 */ u8 safariZoneFleeRate;
 /* 0x19 */ u8 bodyColor : 7;
            u8 noFlip : 1;
 /* 0x1A */	u8 hiddenAbility;
};

struct SpindaSpot
{
    u8 x, y;
    u16 image[16];
};


struct __attribute__((packed)) LevelUpMove
{
	u16 move;
	u8 level;
};


enum
{
    GROWTH_MEDIUM_FAST,
    GROWTH_ERRATIC,
    GROWTH_FLUCTUATING,
    GROWTH_MEDIUM_SLOW,
    GROWTH_FAST,
    GROWTH_SLOW
};

enum
{
    BODY_COLOR_RED,
    BODY_COLOR_BLUE,
    BODY_COLOR_YELLOW,
    BODY_COLOR_GREEN,
    BODY_COLOR_BLACK,
    BODY_COLOR_BROWN,
    BODY_COLOR_PURPLE,
    BODY_COLOR_GRAY,
    BODY_COLOR_WHITE,
    BODY_COLOR_PINK
};

enum EvolutionMethods
{
	EVO_NONE = 0,
	EVO_FRIENDSHIP,
	EVO_FRIENDSHIP_DAY,
	EVO_FRIENDSHIP_NIGHT,
	EVO_LEVEL,
	EVO_TRADE,
	EVO_TRADE_ITEM,
	EVO_ITEM,		// for dawn stone, add MON_MALE(0x0) or MON_FEMALE(0xFE) to .unknown in evo table entry
	EVO_LEVEL_ATK_GT_DEF,
	EVO_LEVEL_ATK_EQ_DEF,
	EVO_LEVEL_ATK_LT_DEF,
	EVO_LEVEL_SILCOON,
	EVO_LEVEL_CASCOON,
	EVO_LEVEL_NINJASK,
	EVO_LEVEL_SHEDINJA,
	EVO_BEAUTY,
	// new evolutions
	EVO_RAINY_FOGGY_OW,		// raining or foggy in overworld
	EVO_MOVE_TYPE,	// knows a move with a specific type (eg. sylveon: fairy type move). Param is the move type
	EVO_TYPE_IN_PARTY,	//specific type (unknown) in party after given level (param).
	EVO_MAP, 	// specific map evolution. bank in param, map in unknown
	EVO_MALE_LEVEL,		// above given level if male
	EVO_FEMALE_LEVEL,	// above given level if female
	EVO_LEVEL_NIGHT,	// above given level at night
	EVO_LEVEL_DAY,		// above given level during day
	EVO_HOLD_ITEM_NIGHT,	// level up holding item at night (eg. sneasel)
	EVO_HOLD_ITEM_DAY,	// level up while holding a specific item during the day (eg. happiny)
	EVO_MOVE,	// knows a given move
	EVO_OTHER_PARTY_MON,	//another poke in the party, arg is a specific species
	EVO_LEVEL_SPECIFIC_TIME_RANGE, // above given level with a range (unknown is [start][end]. eg lycanroc -> 1700-1800 hrs -> 0x1112)
	EVO_FLAG_SET, //If a certain flag is set. Can be used for touching the Mossy/Icy Rock for Leafeon/Glaceon evolutions
	EVO_CRITICAL_HIT, // successfully land 3 critical hits in one battle
	EVO_NATURE_HIGH, // evolution based on high key nature at a certain level
	EVO_NATURE_LOW, // evolution based on low key nature at a certain level
	EVO_DAMAGE_LOCATION, // recieve 49+ damage in battle without fainting, walk to specific tile
	EVO_ITEM_LOCATION, // Stand on a tile with a certain behaviour and use an item on a Pokemon
	EVO_LEVEL_HOLD_ITEM, // Level up to a certain level while holding an item
	EVO_ITEM_HOLD_ITEM, // Use and item on a Pokemon while its holding an item
	EVO_MOVE_MALE, // Knows a given move and is male
	EVO_MOVE_FEMALE, // Knows a given move and is female
	EVO_ITEM_NIGHT, // Item is used on it at night
};
#define EVO_GIGANTAMAX 0xFD
#define EVO_MEGA 0xFE

struct Evolution
{
    u16 method;
    u16 param;
    u16 targetSpecies;
	u16 unknown; // used for mega evo, Dawn Stone, level in EVO_TYPE_IN_PARTY, or time range in EVO_LEVEL_SPECIFIC_TIME_RANGE
};

struct BattleMove
{
    u8 effect;
    u8 power;
    u8 type;
    u8 accuracy;
    u8 pp;
    u8 secondaryEffectChance;
    u8 target;
    s8 priority;
    u8 flags;
	u8 z_move_power;
	u8 split;
	u8 z_move_effect;
};

extern u8 gPlayerPartyCount;
extern struct Pokemon gPlayerParty[PARTY_SIZE];
extern u8 gEnemyPartyCount;
extern struct Pokemon gEnemyParty[PARTY_SIZE];
extern const struct BaseStats gBaseStats[];
#define gItemEffectTable ((const u8**) *((u32*) 0x803A2B0)) //extern const u8 *const gItemEffectTable[];
//extern const struct Evolution gEvolutionTable[][EVOS_PER_MON];
extern const u8 gStatStageRatios[][2];
extern struct SpriteTemplate gMultiuseSpriteTemplate;
extern struct SaveBlock3* gSaveBlock3;
//extern const u32 gExperienceTables[][MAX_MON_LEVEL + 1];
extern const s8 gNatureStatTable[][5];

extern const u8 gFacilityClassToPicIndex[];
extern const u8 gFacilityClassToTrainerClass[];

u8 GetMonAbility(const struct Pokemon* const);
u8 CountAliveMons(u8 caseId);
#define BATTLE_ALIVE_EXCEPT_ACTIVE  0
#define BATTLE_ALIVE_ATK_SIDE       1
#define BATTLE_ALIVE_DEF_SIDE       2

void __attribute__((long_call)) CalculateMonStats(struct Pokemon *mon);
void __attribute__((long_call)) CreateMon(struct Pokemon *mon, u16 species, u8 level, u8 fixedIV, u8 hasFixedPersonality, u32 fixedPersonality, u8 otIdType, u32 fixedOtId);
void __attribute__((long_call)) CreateMonWithNature(struct Pokemon *mon, u16 species, u8 level, u8 fixedIV, u8 nature);
void __attribute__((long_call)) CreateMonWithGenderNatureLetter(struct Pokemon *mon, u16 species, u8 level, u8 fixedIV, u8 gender, u8 nature, u8 unownLetter);
void __attribute__((long_call)) CreateMonWithIVsPersonality(struct Pokemon* mon, u16 species, u8 level, u32 ivs, u32 personality);
void __attribute__((long_call)) CreateMaleMon(struct Pokemon* poke_address, u16 species, u8 level);
u8 __attribute__((long_call)) GetLevelFromBoxMonExp(struct BoxPokemon *boxMon);
u16 __attribute__((long_call)) GiveMoveToMon(struct Pokemon *mon, u16 move);
u16 __attribute__((long_call)) GiveMoveToBoxMon(struct BoxPokemon *boxMon, u16 move);
void __attribute__((long_call)) DeleteFirstMoveAndGiveMoveToMon(struct Pokemon *mon, u16 move);
void __attribute__((long_call)) DeleteFirstMoveAndGiveMoveToBoxMon(struct BoxPokemon *boxMon, u16 move);
u32 __attribute__((long_call)) GetBoxMonData(struct BoxPokemon *boxMon, s32 field, u8 *data);
u8 __attribute__((long_call)) CalculatePlayerPartyCount(void);
u8 __attribute__((long_call)) GetNature(struct Pokemon *mon);
u8 __attribute__((long_call)) GetNatureFromPersonality(u32 personality);
u8 __attribute__((long_call)) GetMonGender(struct Pokemon* mon);
u8 __attribute__((long_call)) GetBoxMonGender(struct BoxPokemon* boxMon);
u8 __attribute__((long_call)) GetGenderFromSpeciesAndPersonality(u16 species, u32 personality);
void __attribute__((long_call)) SetMultiuseSpriteTemplateToPokemon(u16 speciesTag, u8 battlerPosition);
void __attribute__((long_call)) SetBoxMonData(struct BoxPokemon *boxMon, s32 field, const void *data);
void __attribute__((long_call)) EncryptBoxMon(struct BoxPokemon *boxMon);
void __attribute__((long_call)) ZeroBoxMonData(struct BoxPokemon *boxMon);
void __attribute__((long_call)) GetSpeciesName(u8 *name, u16 species);
u16 __attribute__((long_call)) CalculateBoxMonChecksum(struct BoxPokemon *boxMon);
bool8 __attribute__((long_call)) IsMonShiny(struct Pokemon *mon);
bool8 __attribute__((long_call)) IsShinyOtIdPersonality(u32 otId, u32 personality);
u16 __attribute__((long_call)) GetTutorMove(u8 tutor);
bool8 __attribute__((long_call)) MonKnowsMove(struct Pokemon *mon, u16 move);
bool8 __attribute__((long_call)) CanLearnTutorMove(u16, u8);
void __attribute__((long_call)) BoxMonToMon(struct BoxPokemon *srcMon, struct Pokemon *dstMon);
bool8 __attribute__((long_call)) IsPlayerPartyAndPokemonStorageFull(void);
bool8 __attribute__((long_call)) IsPokemonStorageFull(void);

species_t __attribute__((long_call)) GetStarterChoice(void);

u32 __attribute__((long_call)) GetMonData(const struct Pokemon*, s32, const void *data);
void __attribute__((long_call)) SetMonData(struct Pokemon *mon, s32 field, const void *data);
void __attribute__((long_call)) SetMonData2(struct Pokemon *mon, s32 PokemonDataRequest, const void *data);
void __attribute__((long_call)) CopyMon(void *dest, void *src, size_t size);

u8 __attribute__((long_call)) CheckPartyHasHadPokerus(struct Pokemon *party, u8 selection);
u16 __attribute__((long_call)) GetMonEVCount(struct Pokemon* mon);
u8 __attribute__((long_call)) GetLevelFromMonExp(struct Pokemon *mon);
u16 __attribute__((long_call)) ModifyStatByNature(u8 nature, u16 n, u8 statIndex);
const struct CompressedSpritePalette* __attribute__((long_call)) GetMonSpritePalStructFromOtIdPersonality(u16 species, u32 otId , u32 personality);
const u32* __attribute__((long_call)) GetMonSpritePalFromSpeciesAndPersonality(u16 species, u32 otId, u32 personality);

void __attribute__((long_call)) EmitSetMonData(u8 a, u8 request, u8 c, u8 bytes, void *data);
void __attribute__((long_call)) ZeroPlayerPartyMons(void);
void __attribute__((long_call)) ZeroEnemyPartyMons(void);

u8 __attribute__((long_call)) CalculatePPWithBonus(u16 move, u8 ppBonuses, u8 moveIndex);
u16 __attribute__((long_call)) SpeciesToNationalPokedexNum(u16 species);
u16 __attribute__((long_call)) NationalPokedexNumToSpecies(u16 nationalNum);
s8 __attribute__((long_call)) GetPokeFlavourRelation(u32 pid, u8 flavor);
bool8 __attribute__((long_call)) IsTradedMon(struct Pokemon *mon);
bool8 __attribute__((long_call)) IsOtherTrainer(u32 otId, u8* otName);
void __attribute__((long_call)) MonRestorePP(struct Pokemon *mon);
bool8 __attribute__((long_call)) GetSetPokedexFlag(u16 nationalNum, u8 caseID);
void __attribute__((long_call)) HandleSetPokedexFlag(u16 nationalNum, u8 caseId, u32 personality);
u16 __attribute__((long_call)) GetPokedexHeightWeight(u16 dexNum, u8 data);
void __attribute__((long_call)) HealPlayerParty(void);
void __attribute__((long_call)) ReducePartyToThree(void);
u8 __attribute__((long_call)) GetBattlePartyIdFromPartyId(u8 partyId); //Misnamed in decomps!
u8 __attribute__((long_call)) GetPartyIdFromBattleSlot(u8 slot);
u8 __attribute__((long_call)) GetEggMoves(struct Pokemon* poke, void* storageAddr);
void __attribute__((long_call)) Special_0DD_DeleteMove();

void __attribute__((long_call)) SizeMinigame_BufferPokeNameSize(u16 species, void* varAddress);
u8 __attribute__((long_call)) SizeMinigame_CalculateMonHeight(u16 species, void* varAddress);
u16 __attribute__((long_call)) SizeMinigame_GetHeightBonus(struct Pokemon* mon);
u32 __attribute__((long_call)) SizeMinigame_CalculateAdjustedHeight(u16 species, u16 heightBonus);
void __attribute__((long_call)) SizeMinigame_BufferHeightInches(u8* stringBuffer, u16 height);
void __attribute__((long_call)) ZeroMonData(struct Pokemon* mon);
u16 __attribute__((long_call)) SpeciesToPokedexNum(u16 species);
u16 __attribute__((long_call)) GetCombinedOTID(void);
u8 __attribute__((long_call)) GetTrainerEncounterMusicId(u16 trainerOpponentId);
bool8 __attribute__((long_call)) ExecuteTableBasedItemEffect(struct Pokemon *mon, u16 item, u8 partyIndex, u8 moveIndex);
void __attribute__((long_call)) SetMonMoveSlotOld(struct Pokemon *mon, u16 move, u8 slot);
void __attribute__((long_call)) ShiftMoveSlot(struct Pokemon *mon, u8 slotTo, u8 slotFrom);
void __attribute__((long_call)) RemoveMonPPBonus(struct Pokemon *mon, u8 moveIndex);
bool8 __attribute__((long_call)) TryIncrementMonLevel(struct Pokemon *mon);
u8 __attribute__((long_call)) CheckPartyPokerus(struct Pokemon *party, u8 selection);
bool8 __attribute__((long_call)) PokemonItemUseNoEffect(struct Pokemon *mon, u16 item, u8 partyIndex, u8 moveIndex);
u8 __attribute__((long_call)) GetItemEffectParamOffset(u16 itemId, u8 effectByte, u8 effectBit);
u16 __attribute__((long_call)) GetEvolutionTargetSpeciesOld(struct Pokemon *mon, u8 type, u16 evolutionItem);
void __attribute__((long_call)) AdjustFriendship(struct Pokemon *mon, u8 event);

/*
void ZeroMonData(struct Pokemon *mon);
void ZeroPlayerPartyMons(void);
void ZeroEnemyPartyMons(void);
void CreateMon(struct Pokemon *mon, u16 species, u8 level, u8 fixedIV, u8 hasFixedPersonality, u32 fixedPersonality, u8 otIdType, u32 fixedOtId);
//void CreateBoxMon(struct BoxPokemon *boxMon, u16 species, u8 level, u8 fixedIV, u8 hasFixedPersonality, u32 fixedPersonality, u8 otIdType, u32 fixedOtId);
void CreateMonWithNature(struct Pokemon *mon, u16 species, u8 level, u8 fixedIV, u8 nature);
void CreateMonWithGenderNatureLetter(struct Pokemon *mon, u16 species, u8 level, u8 fixedIV, u8 gender, u8 nature, u8 unownLetter);
void CreateMaleMon(struct Pokemon *mon, u16 species, u8 level);
void CreateMonWithIVsPersonality(struct Pokemon *mon, u16 species, u8 level, u32 ivs, u32 personality);
void CreateMonWithIVsOTID(struct Pokemon *mon, u16 species, u8 level, u8 *ivs, u32 otId);
void CreateMonWithEVSpread(struct Pokemon *mon, u16 species, u8 level, u8 fixedIV, u8 evSpread);
void CalculateMonStats(struct Pokemon *mon);
u8 GetLevelFromBoxMonExp(struct BoxPokemon *boxMon);
u16 GiveMoveToMon(struct Pokemon *mon, u16 move);
u16 GiveMoveToBoxMon(struct BoxPokemon *boxMon, u16 move);
u16 GiveMoveToBattleMon(struct BattlePokemon *mon, u16 move);
void MonRestorePP(struct Pokemon *mon);
void SetBattleMonMoveSlot(struct BattlePokemon *mon, u16 move, u8 slot);
void GiveMonInitialMoveset(struct Pokemon *mon);
void GiveBoxMonInitialMoveset(struct BoxPokemon *boxMon);
void DeleteFirstMoveAndGiveMoveToMon(struct Pokemon *mon, u16 move);
void DeleteFirstMoveAndGiveMoveToBoxMon(struct BoxPokemon *boxMon, u16 move);
bool8 PokemonUseItemEffects(struct Pokemon *mon, u16 item, u8 partyIndex, u8 moveIndex, u8 e);

u8 GetMonGender(struct Pokemon *mon);
u8 GetBoxMonGender(struct BoxPokemon *boxMon);
u8 GetGenderFromSpeciesAndPersonality(u16 species, u32 personality);
void EncryptBoxMon(struct BoxPokemon *boxMon);
void DecryptBoxMon(struct BoxPokemon *boxMon);


// These are full type signatures for GetMonData() and GetBoxMonData(),
// but they are not used since some code erroneously omits the third arg.
// u32 GetMonData(struct Pokemon *mon, s32 field, u8 *data);
// u32 GetBoxMonData(struct BoxPokemon *boxMon, s32 field, u8 *data);

#ifdef IS_POKEMON_C
u32 GetMonData(struct Pokemon *, s32, u8 *);
u32 GetBoxMonData(struct BoxPokemon *, s32, u8 *);
#else
u32 GetMonData();
u32 GetBoxMonData();
#endif // IS_POKEMON_C

void SetMonData(struct Pokemon *mon, s32 field, const void *data);
void CopyMon(void *dest, void *src, size_t size);
u8 GiveMonToPlayer(struct Pokemon *mon);
u8 SendMonToPC(struct Pokemon *mon);
u8 CalculatePlayerPartyCount(void);
u8 CalculateEnemyPartyCount(void);

u8 GetAbilityBySpecies(u16 species, bool8 altAbility);
u8 GetMonAbility(struct Pokemon *mon);
void CreateSecretBaseEnemyParty(struct SecretBaseRecord *secretBaseRecord);
u8 GetSecretBaseTrainerPicIndex(void);
u8 GetSecretBaseTrainerNameIndex(void);
u8 CalculatePPWithBonus(u16 move, u8 ppBonuses, u8 moveIndex);
void RemoveBattleMonPPBonus(struct BattlePokemon *mon, u8 moveIndex);
void CopyPlayerPartyMonToBattleData(u8 battleIndex, u8 partyIndex);

u8 GetNature(struct Pokemon *mon);
u8 GetNatureFromPersonality(u32 personality);

u16 nature_stat_mod(u8 nature, u16 n, u8 statIndex);

void MonRestorePP(struct Pokemon *);
void BoxMonRestorePP(struct BoxPokemon *);

u16 NationalToHoennOrder(u16);
u16 SpeciesToNationalPokedexNum(u16);
u16 HoennToNationalOrder(u16);
u16 SpeciesToCryId(u16 species);
void DrawSpindaSpots(u16, u32, u8 *, u8);
u8 CheckPartyHasHadPokerus(struct Pokemon *, u8);
void UpdatePartyPokerusTime(u16);
u32 CanMonLearnTMHM(struct Pokemon *, u8);
u32 CanSpeciesLearnTMHM(u16 species, u8 tm);
u8 GetMoveRelearnerMoves(struct Pokemon *mon, u16 *moves);
void ClearBattleMonForms(void);
const u8 *GetMonFrontSpritePal(struct Pokemon *mon);
const u8 *GetFrontSpritePalFromSpeciesAndPersonality(u16, u32, u32);
const struct CompressedSpritePalette *sub_80409C8(u16, u32, u32);
bool8 IsOtherTrainer(u32, u8 *);
void SetWildMonHeldItem(void);

const struct CompressedSpritePalette *sub_806E794(struct Pokemon *mon);
bool32 IsHMMove2(u16 move);
bool8 IsPokeSpriteNotFlipped(u16 species);
bool8 IsShinyOtIdPersonality(u32 otId, u32 personality);

void MonGainEVs(struct Pokemon *mon, u16 defeatedSpecies);
bool8 IsTradedMon(struct Pokemon *mon);
s32 GetBankMultiplayerId(u16 a1);
bool16 sub_806D82C(u8 id);
u16 MonTryLearningNewMove(struct Pokemon* mon, bool8);
void sub_8068AA4(void); // sets stats for deoxys
bool8 HasTwoFramesAnimation(u16 species);
void RandomlyGivePartyPokerus(struct Pokemon *party);
void UpdatePartyPokerusTime(u16 days);
void PartySpreadPokerus(struct Pokemon *party);
s8 GetMonFlavorRelation(struct Pokemon *mon, u8 a2);
s8 GetFlavorRelationByPersonality(u32 personality, u8 a2);
u8 GetItemEffectParamOffset(u16 itemId, u8 effectByte, u8 effectBit);
u8 GetDefaultMoveTarget(u8 atkBank);
u16 PlayerGenderToFrontTrainerPicId(u8 playerGender);
void sub_806A1C0(u16 arg0, u8 bankIdentity);
void sub_806A12C(u16 trainerSpriteId, u8 bankIdentity);
u8 GetSecretBaseTrainerPicIndex(void);
u8 GetLevelUpMovesBySpecies(u16 species, u16 *moves);
bool8 HealStatusConditions(struct Pokemon *mon, u32 battlePartyId, u32 healMask, u8 battlerId);

#include "sprite.h"

void DoMonFrontSpriteAnimation(struct Sprite* sprite, u16 species, bool8 noCry, u8 arg3);
void BattleAnimateFrontSprite(struct Sprite* sprite, u16 species, bool8 noCry, u8 arg3);
void BattleAnimateBackSprite(struct Sprite* sprite, u16 species);

*/
