#pragma once

#include "../include/global.h"

//typedef u8 pokemon_data_request_t;

//typedef void (*pokemon_setattr_t) (pokemon_t*, pokemon_data_request_t, void*);
//#define pokemon_setattr ((pokemon_setattr_t) (0x0804037C |1))

//typedef u32 (*pokemon_getattr_t) (pokemon_t*, pokemon_data_request_t);
//#define pokemon_getattr ((pokemon_getattr_t) (0x0803FBE8 |1))

#define REQ_PERSONALITY         0
#define REQ_OTID                1
#define REQ_NICK                2
#define REQ_LANG                3
#define REQ_SANITYx4            4
#define REQ_SANITYx5            5
#define REQ_SANITYx6            6
#define REQ_OTNAME              7
#define REQ_MARKS               8
#define REQ_CHECKSUM            9
#define REQ_SPECIES             11
#define REQ_HELDITEM            12
#define REQ_MOVE1               13
#define REQ_MOVE2               14
#define REQ_MOVE3               15
#define REQ_MOVE4               16
#define REQ_PP1                 17
#define REQ_PP2                 18
#define REQ_PP3                 19
#define REQ_PP4                 20
#define REQ_PPBONUSES           21
#define REQ_COOLNESS            22
#define REQ_BEAUTY              23
#define REQ_CUTENESS            24
#define REQ_EXP                 25
#define REQ_HPEV                26
#define REQ_ATKEV               27
#define REQ_DEFEV               28
#define REQ_SPDEV               29
#define REQ_SPATKEV             30
#define REQ_SPDEFEV             31
#define REQ_HAPPINESS           32
#define REQ_SMARTNESS           33
#define REQ_POKERUS             34
#define REQ_METLOC              35
#define REQ_METLEVEL            36
#define REQ_METGAME             37
#define REQ_POKEBALL            38
#define REQ_HPIV                39
#define REQ_ATKIV               40
#define REQ_DEFIV               41
#define REQ_SPDIV               42
#define REQ_SPATKIV             43
#define REQ_SPDEFIV             44
#define REQ_EGG                 45
#define REQ_ABILITY             46
#define REQ_TOUGHNESS           47
#define REQ_SHEEN               48
#define REQ_OTGENDER            49
#define REQ_UNKNOWN             50
#define REQ_STATUS              55
#define REQ_LEVEL               56
#define REQ_CURRHP              57
#define REQ_MAXHP               58
#define REQ_ATK                 59
#define REQ_DEF                 60
#define REQ_SPD                 61
#define REQ_SPATK               62
#define REQ_SPDEF               63
#define REQ_POKERUSLEFT         64
#define REQ_SPECIES2            65
#define REQ_FAMERIBBON          67
#define REQ_OBEDIENCE           80
#define REQ_ATK2                84
#define REQ_DEF2                85
#define REQ_SPD2                86
#define REQ_SPATK2              87
#define REQ_SPDEF2              88
