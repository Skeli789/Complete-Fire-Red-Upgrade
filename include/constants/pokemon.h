#pragma once

// Pokemon types
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
#define TYPE_STELLAR 0x18

#define NUMBER_OF_MON_TYPES (TYPE_STELLAR + 1)

#define TYPE_NAME_LENGTH 6

// Pokemon egg groups
#define EGG_GROUP_NONE 0
#define EGG_GROUP_MONSTER 1
#define EGG_GROUP_WATER_1 2
#define EGG_GROUP_BUG 3
#define EGG_GROUP_FLYING 4
#define EGG_GROUP_FIELD 5
#define EGG_GROUP_FAIRY 6
#define EGG_GROUP_GRASS 7
#define EGG_GROUP_HUMAN_LIKE 8
#define EGG_GROUP_WATER_3 9
#define EGG_GROUP_MINERAL 10
#define EGG_GROUP_AMORPHOUS 11
#define EGG_GROUP_WATER_2 12
#define EGG_GROUP_DITTO 13
#define EGG_GROUP_DRAGON 14
#define EGG_GROUP_UNDISCOVERED 15

#define EGG_GROUPS_PER_MON 2

// Pokemon natures
#define NATURE_HARDY 0
#define NATURE_LONELY 1
#define NATURE_BRAVE 2
#define NATURE_ADAMANT 3
#define NATURE_NAUGHTY 4
#define NATURE_BOLD 5
#define NATURE_DOCILE 6
#define NATURE_RELAXED 7
#define NATURE_IMPISH 8
#define NATURE_LAX 9
#define NATURE_TIMID 10
#define NATURE_HASTY 11
#define NATURE_SERIOUS 12
#define NATURE_JOLLY 13
#define NATURE_NAIVE 14
#define NATURE_MODEST 15
#define NATURE_MILD 16
#define NATURE_QUIET 17
#define NATURE_BASHFUL 18
#define NATURE_RASH 19
#define NATURE_CALM 20
#define NATURE_GENTLE 21
#define NATURE_SASSY 22
#define NATURE_CAREFUL 23
#define NATURE_QUIRKY 24

#define NUM_NATURES NATURE_QUIRKY + 1

// Pokemon Stats
#define STAT_HP 0
#define STAT_ATK 1
#define STAT_DEF 2
#define STAT_SPEED 3
#define STAT_SPATK 4
#define STAT_SPDEF 5
#define STAT_ACC 6 // only in battles
#define STAT_EVASION 7 // only in battles

#define FRIENDSHIP_EVENT_GROW_LEVEL           0
#define FRIENDSHIP_EVENT_VITAMIN              1
#define FRIENDSHIP_EVENT_BATTLE_ITEM          2
#define FRIENDSHIP_EVENT_LEAGUE_BATTLE        3
#define FRIENDSHIP_EVENT_LEARN_TMHM           4
#define FRIENDSHIP_EVENT_WALKING              5
#define FRIENDSHIP_EVENT_MASSAGE              6
#define FRIENDSHIP_EVENT_FAINT_SMALL          7
#define FRIENDSHIP_EVENT_FAINT_OUTSIDE_BATTLE 8
#define FRIENDSHIP_EVENT_FAINT_LARGE          9

#define EVO_MODE_NORMAL     0
#define EVO_MODE_TRADE      1
#define EVO_MODE_ITEM_USE   2
#define EVO_MODE_ITEM_CHECK 3 // If an Everstone is being held, still want to show that the stone *could* be used on that Pokémon to evolve

#define SHINY_ODDS 8 // Actual probability is SHINY_ODDS/65536

// Learning moves
#define MON_ALREADY_KNOWS_MOVE 0xFFFE
#define MON_HAS_MAX_MOVES      0xFFFF
