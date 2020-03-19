#pragma once

#include "../global.h"

/**
 * \file pokemon_storage_system.h
 * \brief Contains functions relating to viewing or modifying data in the PC. It also
 *		  helps expand the total number of available boxes past the original 14. It does
 *		  this mainly be compressing the Pokemon data structure down to 65 bytes. Contest
 *		  stats, ribbons, unused bytes, and fateful encounter are all removed in the
 *		  processed. Thus, these stats can no longer be used for anything permanent, but
 *		  can be used as long as the player doesn't deposit their Pokemon (temporary stats).
 */


struct __attribute__((packed)) CompressedPokemonSubstruct0
{
    u16 species;
    u16 heldItem;
    u32 experience;
    u8 ppBonuses;
    u8 friendship;
	u8 pokeball;
	//u8 unknown; //Removed to save space for box mon
};

struct __attribute__((packed)) CompressedPokemon
{
	u32 personality;
	u32 otid;
	u8 nickname[10];
	u8 language;
	u8 sanity;
	u8 otname[7];
	u8 markings;

//Substructure Growth
    struct CompressedPokemonSubstruct0 substruct0;
	u32 move1 : 10;
	u32 move2 : 10;
	u32 move3 : 10;
	u16 move4 : 10;

//Substructure Condition
    u8 hpEv;
    u8 atkEv;
    u8 defEv;
    u8 spdEv;
    u8 spAtkEv;
    u8 spDefEv;

//Substructure Misc
 /* 0x00 */ u8 pokerus;
 /* 0x01 */	u8 metLocation;
 /* 0x02 */ u16 metInfo; //Met level, met game, OT gender
 /* 0x04 */ u32 ivs;
}; //SIZE = 0x3A / 58 bytes


//Exported Functions
u32 GetBoxMonDataAt(u8 boxId, u8 boxPosition, s32 request);
void SetBoxMonDataAt(u8 boxId, u8 boxPosition, s32 request, const void* value);
void GetBoxMonNickAt(u8 boxId, u8 boxPosition, u8* dst);
void SetBoxMonNickAt(u8 boxId, u8 boxPosition, const u8* nick);
u32 GetAndCopyBoxMonDataAt(u8 boxId, u8 boxPosition, s32 request, void* dst);
void SetBoxMonAt(u8 boxId, u8 boxPosition, struct BoxPokemon* src);
void CreateBoxMonAt(u8 boxId, u8 boxPosition, u16 species, u8 level, u8 fixedIV, u8 hasFixedPersonality, u32 personality, u8 otIDType, u32 otID);
void ZeroBoxMonAt(u8 boxId, u8 boxPosition);
void BoxMonAtToMon(u8 boxId, u8 boxPosition, struct Pokemon *dst);
struct BoxPokemon* GetBoxedMonPtr(u8 boxId, u8 boxPosition);
u8 SendMonToBoxPos(struct Pokemon* mon, u8 boxNo, u8 boxPos);
void BackupPartyToTempTeam(u8 firstId, u8 numPokes);
void RestorePartyFromTempTeam(u8 firstId, u8 numPokes);

//Functions Hooked In
u8* GetBoxNamePtr(u8 boxId);
u8 GetBoxWallpaper(u8 boxId);
void SetBoxWallpaper(u8 boxId, u8 wallpaperId);
s8 sub_80916F4(u8 boxId);
u8 SendMonToPC(struct Pokemon* mon);
