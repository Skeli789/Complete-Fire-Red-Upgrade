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

//Functions Hooked In
u8* GetBoxNamePtr(u8 boxId);
u8 GetBoxWallpaper(u8 boxId);
void SetBoxWallpaper(u8 boxId, u8 wallpaperId);
s8 sub_80916F4(u8 boxId);