#include "defines.h"
#include "../include/event_data.h"
#include "../include/pokemon_icon.h"
#include "../include/pokemon_storage_system.h"
#include "../include/pokemon_storage_system_internal.h"
#include "../include/string_util.h"
#include "../include/text.h"
#include "../include/constants/flags.h"
#include "../include/constants/vars.h"

#include "../include/new/build_pokemon.h"
#include "../include/new/form_change.h"
#include "../include/new/frontier.h"
#include "../include/new/item.h"
#include "../include/new/pokemon_storage_system.h"
/*
pokemon_storage_system.c
	handles pokemon storage expansion and related functions
*/

enum
{
	WALLPAPER_FOREST,
	WALLPAPER_CITY,
	WALLPAPER_DESERT,
	WALLPAPER_SAVANNA,
	WALLPAPER_CRAG,
	WALLPAPER_VOLCANO,
	WALLPAPER_SNOW,
	WALLPAPER_CAVE,
	WALLPAPER_BEACH,
	WALLPAPER_SEAFLOOR,
	WALLPAPER_RIVER,
	WALLPAPER_SKY,
	WALLPAPER_STARS,
	WALLPAPER_POKECENTER,
	WALLPAPER_TILES,
	WALLPAPER_SIMPLE,
	WALLPAPER_COUNT
};

typedef u8 BoxNameT[9];

#define ORIGINAL_BOX_POKEMON_RAM ((struct CompressedPokemon*) (0x2029314 + 4))
#define ORIGINAL_BOX_NAME_RAM ((BoxNameT*) (0x2029314 + 0x8344))
#define ORIGINAL_BOX_WALLPAPER_RAM ((u8*) (0x2029314 + 0x83C2))

#define BOX_20_RAM ((struct CompressedPokemon*) 0x203CB44)
#define BOX_21_RAM (BOX_20_RAM + 30)
#define BOX_22_RAM (BOX_21_RAM + 30) //Should end at 0x203DFA8
#define BOX_23_RAM ((struct CompressedPokemon*) 0x2027434)
#define BOX_24_RAM (BOX_23_RAM + 30)
#define BOX_25_RAM ((struct CompressedPokemon*) 0x2024638)

//#define sPSSData ((struct PokemonStorageSystemData*) *((u32*) 0x20397B0))

#define gDisposableBoxMon ((struct BoxPokemon*) 0x2022BC4) //Reuse Battle Buffer A

struct CompressedPokemon* const sPokemonBoxPtrs[TOTAL_BOXES_COUNT] =
{
	ORIGINAL_BOX_POKEMON_RAM,				//Box 1
	ORIGINAL_BOX_POKEMON_RAM + (30 * 1),	//Box 2
	ORIGINAL_BOX_POKEMON_RAM + (30 * 2),	//Box 3
	ORIGINAL_BOX_POKEMON_RAM + (30 * 3),	//Box 4
	ORIGINAL_BOX_POKEMON_RAM + (30 * 4),	//Box 5
	ORIGINAL_BOX_POKEMON_RAM + (30 * 5),	//Box 6
	ORIGINAL_BOX_POKEMON_RAM + (30 * 6),	//Box 7
	ORIGINAL_BOX_POKEMON_RAM + (30 * 7),	//Box 8
	ORIGINAL_BOX_POKEMON_RAM + (30 * 8),	//Box 9
	ORIGINAL_BOX_POKEMON_RAM + (30 * 9),	//Box 10
	ORIGINAL_BOX_POKEMON_RAM + (30 * 10),	//Box 11
	ORIGINAL_BOX_POKEMON_RAM + (30 * 11),	//Box 12
	ORIGINAL_BOX_POKEMON_RAM + (30 * 12),	//Box 13
	ORIGINAL_BOX_POKEMON_RAM + (30 * 13),	//Box 14
	ORIGINAL_BOX_POKEMON_RAM + (30 * 14),	//Box 15
	ORIGINAL_BOX_POKEMON_RAM + (30 * 15),	//Box 16
	ORIGINAL_BOX_POKEMON_RAM + (30 * 16),	//Box 17
	ORIGINAL_BOX_POKEMON_RAM + (30 * 17),	//Box 18
	ORIGINAL_BOX_POKEMON_RAM + (30 * 18),	//Box 19
	BOX_20_RAM,								//Box 20
	BOX_21_RAM,								//Box 21
	BOX_22_RAM,								//Box 22
	BOX_23_RAM,								//Box 23
	BOX_24_RAM,								//Box 24
	BOX_25_RAM,								//Box 25
};

BoxNameT* const sPokemonBoxNamePtrs[TOTAL_BOXES_COUNT] =
{
	ORIGINAL_BOX_NAME_RAM,		//Box 1
	ORIGINAL_BOX_NAME_RAM + 1,	//Box 2
	ORIGINAL_BOX_NAME_RAM + 2,	//Box 3
	ORIGINAL_BOX_NAME_RAM + 3,	//Box 4
	ORIGINAL_BOX_NAME_RAM + 4,	//Box 5
	ORIGINAL_BOX_NAME_RAM + 5,	//Box 6
	ORIGINAL_BOX_NAME_RAM + 6,	//Box 7
	ORIGINAL_BOX_NAME_RAM + 7,	//Box 8
	ORIGINAL_BOX_NAME_RAM + 8,	//Box 9
	ORIGINAL_BOX_NAME_RAM + 9,	//Box 10
	ORIGINAL_BOX_NAME_RAM + 10,	//Box 11
	ORIGINAL_BOX_NAME_RAM + 11,	//Box 12
	ORIGINAL_BOX_NAME_RAM + 12,	//Box 13
	ORIGINAL_BOX_NAME_RAM + 13,	//Box 14
	ORIGINAL_BOX_NAME_RAM - 1,	//Box 15
	ORIGINAL_BOX_NAME_RAM - 2,	//Box 16
	ORIGINAL_BOX_NAME_RAM - 3,	//Box 17
	ORIGINAL_BOX_NAME_RAM - 4,	//Box 18
	ORIGINAL_BOX_NAME_RAM - 5,	//Box 19
	ORIGINAL_BOX_NAME_RAM - 6,	//Box 20
	ORIGINAL_BOX_NAME_RAM - 7,	//Box 21
	ORIGINAL_BOX_NAME_RAM - 8,	//Box 22
	ORIGINAL_BOX_NAME_RAM - 9,	//Box 23
	ORIGINAL_BOX_NAME_RAM - 10,	//Box 24
	ORIGINAL_BOX_NAME_RAM - 11,	//Box 25
};

//Declaring this as "static" makes it not compile properly for some reason.
u8* const sPokemonBoxWallpaperPtrs[TOTAL_BOXES_COUNT] =
{
	ORIGINAL_BOX_WALLPAPER_RAM,							//Box 1
	ORIGINAL_BOX_WALLPAPER_RAM + 1,						//Box 2
	ORIGINAL_BOX_WALLPAPER_RAM + 2,						//Box 3
	ORIGINAL_BOX_WALLPAPER_RAM + 3,						//Box 4
	ORIGINAL_BOX_WALLPAPER_RAM + 4,						//Box 5
	ORIGINAL_BOX_WALLPAPER_RAM + 5,						//Box 6
	ORIGINAL_BOX_WALLPAPER_RAM + 6,						//Box 7
	ORIGINAL_BOX_WALLPAPER_RAM + 7,						//Box 8
	ORIGINAL_BOX_WALLPAPER_RAM + 8,						//Box 9
	ORIGINAL_BOX_WALLPAPER_RAM + 9,						//Box 10
	ORIGINAL_BOX_WALLPAPER_RAM + 10,					//Box 11
	ORIGINAL_BOX_WALLPAPER_RAM + 11,					//Box 12
	ORIGINAL_BOX_WALLPAPER_RAM + 12,					//Box 13
	ORIGINAL_BOX_WALLPAPER_RAM + 13,					//Box 14
	(u8*) (ORIGINAL_BOX_POKEMON_RAM + (30 * 19)),		//Box 15
	(u8*) (ORIGINAL_BOX_POKEMON_RAM + (30 * 19)) + 1,	//Box 16
	(u8*) (ORIGINAL_BOX_POKEMON_RAM + (30 * 19)) + 2,	//Box 17
	(u8*) (ORIGINAL_BOX_POKEMON_RAM + (30 * 19)) + 3,	//Box 18
	(u8*) (ORIGINAL_BOX_POKEMON_RAM + (30 * 19)) + 4,	//Box 19
	(u8*) (ORIGINAL_BOX_POKEMON_RAM + (30 * 19)) + 5,	//Box 20
	(u8*) (ORIGINAL_BOX_POKEMON_RAM + (30 * 19)) + 6,	//Box 21
	(u8*) (ORIGINAL_BOX_POKEMON_RAM + (30 * 19)) + 7,	//Box 22
	(u8*) (ORIGINAL_BOX_POKEMON_RAM + (30 * 19)) + 8,	//Box 23
	(u8*) (ORIGINAL_BOX_POKEMON_RAM + (30 * 19)) + 9,	//Box 24
	(u8*) (ORIGINAL_BOX_POKEMON_RAM + (30 * 19)) + 10,	//Box 25
};

#define gTempTeamBackup ((struct CompressedPokemon*) 0x203E1A4)

//This file's functions:
void CreateBoxMonFromCompressedMon(struct BoxPokemon* boxMon, struct CompressedPokemon* compMon);
void CreateCompressedMonFromBoxMon(struct BoxPokemon* boxMon, struct CompressedPokemon* compMon);
struct CompressedPokemon* GetCompressedMonPtr(u8 boxId, u8 boxPosition);

u32 GetBoxMonDataAt(u8 boxId, u8 boxPosition, s32 request)
{
	if (boxId < TOTAL_BOXES_COUNT && boxPosition < IN_BOX_COUNT)
	{
		if (request == MON_DATA_SPECIES)
		{
			//Save time since checking if a slot is empty is very common
			return sPokemonBoxPtrs[boxId][boxPosition].substruct0.species;
		}
		else
		{
			struct BoxPokemon mon;
			CreateBoxMonFromCompressedMon(&mon, &sPokemonBoxPtrs[boxId][boxPosition]);
			return GetBoxMonData(&mon, request, NULL);
		}
	}
	else
		return 0;
}

void SetBoxMonDataAt(u8 boxId, u8 boxPosition, s32 request, const void* value)
{
	if (boxId < TOTAL_BOXES_COUNT && boxPosition < IN_BOX_COUNT)
	{
		struct Pokemon mon; //Not Box Mon just in case stats are modified by hold item form change and it corrupts data
		CreateBoxMonFromCompressedMon((struct BoxPokemon*) &mon, &sPokemonBoxPtrs[boxId][boxPosition]); //Create temporary mon
		SetBoxMonData((struct BoxPokemon*) &mon, request, value);
		CreateCompressedMonFromBoxMon((struct BoxPokemon*) &mon, &sPokemonBoxPtrs[boxId][boxPosition]); //Copy new data back
	}
}

void GetBoxMonNickAt(u8 boxId, u8 boxPosition, u8* dst)
{
	if (boxId < TOTAL_BOXES_COUNT && boxPosition < IN_BOX_COUNT)
	{
		struct BoxPokemon mon;
		CreateBoxMonFromCompressedMon(&mon, &sPokemonBoxPtrs[boxId][boxPosition]);
		GetBoxMonData(&mon, MON_DATA_NICKNAME, dst);
	}
	else
		*dst = EOS;
}

void SetBoxMonNickAt(u8 boxId, u8 boxPosition, const u8* nick)
{
	if (boxId < TOTAL_BOXES_COUNT && boxPosition < IN_BOX_COUNT)
	{
		struct BoxPokemon mon;
		CreateBoxMonFromCompressedMon(&mon, &sPokemonBoxPtrs[boxId][boxPosition]);
		SetBoxMonData(&mon, MON_DATA_NICKNAME, nick);
		CreateCompressedMonFromBoxMon(&mon, &sPokemonBoxPtrs[boxId][boxPosition]); //Copy new data back
	}
}

u32 GetAndCopyBoxMonDataAt(u8 boxId, u8 boxPosition, s32 request, void* dst)
{
	if (boxId < TOTAL_BOXES_COUNT && boxPosition < IN_BOX_COUNT)
	{
		struct BoxPokemon mon;
		CreateBoxMonFromCompressedMon(&mon, &sPokemonBoxPtrs[boxId][boxPosition]);
		return GetBoxMonData(&mon, request, dst);
	}
	else
		return 0;
}

void SetBoxMonAt(u8 boxId, u8 boxPosition, struct BoxPokemon* src)
{
	if (boxId < TOTAL_BOXES_COUNT && boxPosition < IN_BOX_COUNT)
	{
		#ifdef BOX_PRESET_TEAMS
		if (boxId != BOX_PRESET_TEAMS) //Don't revert in Battle Box
		#endif
			HoopaShayminPCRevertCheck((struct Pokemon*) src, FALSE); //Don't recalc stats for a box mon

		struct CompressedPokemon mon;
		CreateCompressedMonFromBoxMon(src, &mon);
		sPokemonBoxPtrs[boxId][boxPosition] = mon;
	}
}

void CopyBoxMonAt(u8 boxId, u8 boxPosition, struct CompressedPokemon* dst)
{
	if (boxId < TOTAL_BOXES_COUNT && boxPosition < IN_BOX_COUNT)
		*dst = sPokemonBoxPtrs[boxId][boxPosition];
}

void CreateBoxMonAt(u8 boxId, u8 boxPosition, u16 species, u8 level, u8 fixedIV, u8 hasFixedPersonality, u32 personality, u8 otIDType, u32 otID)
{
	if (boxId < TOTAL_BOXES_COUNT && boxPosition < IN_BOX_COUNT)
	{
		struct BoxPokemon boxMon;
		CreateBoxMon(&boxMon,
					 species,
					 level,
					 fixedIV,
					 hasFixedPersonality, personality,
					 otIDType, otID);

		struct CompressedPokemon compMon;
		CreateCompressedMonFromBoxMon(&boxMon, &compMon);
		sPokemonBoxPtrs[boxId][boxPosition] = compMon;
	}
}

void ZeroBoxMonAt(u8 boxId, u8 boxPosition)
{
	if (boxId < TOTAL_BOXES_COUNT && boxPosition < IN_BOX_COUNT)
		memset(&sPokemonBoxPtrs[boxId][boxPosition], 0, sizeof(struct CompressedPokemon));
}

void BoxMonAtToMon(u8 boxId, u8 boxPosition, struct Pokemon* dst)
{
	if (boxId < TOTAL_BOXES_COUNT && boxPosition < IN_BOX_COUNT)
	{
		struct BoxPokemon mon;
		CreateBoxMonFromCompressedMon(&mon, &sPokemonBoxPtrs[boxId][boxPosition]);
		BoxMonToMon(&mon, dst);
	}
}

void CompressedMonToMon(struct CompressedPokemon* compMon, struct Pokemon* dst)
{
	struct BoxPokemon mon;
	CreateBoxMonFromCompressedMon(&mon, compMon);
	BoxMonToMon(&mon, dst);
}

struct BoxPokemon* GetBoxedMonPtr(u8 boxId, u8 boxPosition)
{
	if (boxId < TOTAL_BOXES_COUNT && boxPosition < IN_BOX_COUNT)
	{
		struct CompressedPokemon* compMon = &sPokemonBoxPtrs[boxId][boxPosition];
		CreateBoxMonFromCompressedMon(gDisposableBoxMon, compMon); //Use a static location in memory
		return gDisposableBoxMon;
	}
	else
		return NULL;
}

struct CompressedPokemon* GetCompressedMonPtr(u8 boxId, u8 boxPosition)
{
	if (boxId < TOTAL_BOXES_COUNT && boxPosition < IN_BOX_COUNT)
		return &sPokemonBoxPtrs[boxId][boxPosition];
	else
		return NULL;
}

void CreateBoxMonFromCompressedMon(struct BoxPokemon* boxMon, struct CompressedPokemon* compMon)
{
	Memset(boxMon, 0, sizeof(struct BoxPokemon)); //Clear garbage data

	Memcpy(boxMon, compMon, 0x1C); //Personality to markings
	Memcpy(&boxMon->substruct0, &compMon->substruct0, sizeof(struct CompressedPokemonSubstruct0));
	Memcpy(&boxMon->substruct2, &compMon->hpEv, NUM_STATS); //Copy EVs
	Memcpy(&boxMon->substruct3, &compMon->pokerus, 8); //Copy some of substruct misc

	boxMon->substruct1.moves[0] = compMon->move1;
	boxMon->substruct1.moves[1] = compMon->move2;
	boxMon->substruct1.moves[2] = compMon->move3;
	boxMon->substruct1.moves[3] = compMon->move4;

	for (int i = 0; i < MAX_MON_MOVES; ++i) //Give PP
		boxMon->substruct1.pp[i] = CalculatePPWithBonus(boxMon->substruct1.moves[i], boxMon->substruct0.ppBonuses, i);
}

void CreateCompressedMonFromBoxMon(struct BoxPokemon* boxMon, struct CompressedPokemon* compMon)
{
	Memset(compMon, 0, sizeof(struct CompressedPokemon)); //Clear garbage data

	Memcpy(compMon, boxMon, 0x1C); //Personality to markings
	Memcpy(&compMon->substruct0, &boxMon->substruct0, sizeof(struct CompressedPokemonSubstruct0));
	Memcpy(&compMon->hpEv, &boxMon->substruct2, NUM_STATS); //Copy EVs
	Memcpy(&compMon->pokerus, &boxMon->substruct3, 8); //Copy some of substruct misc

	compMon->move1 = boxMon->substruct1.moves[0];
	compMon->move2 = boxMon->substruct1.moves[1];
	compMon->move3 = boxMon->substruct1.moves[2];
	compMon->move4 = boxMon->substruct1.moves[3];
}

u8* GetBoxNamePtr(u8 boxId)
{
	if (boxId < TOTAL_BOXES_COUNT)
		return (u8*) sPokemonBoxNamePtrs[boxId];
	else
		return NULL;
}

u8 GetBoxWallpaper(u8 boxId)
{
	if (boxId < TOTAL_BOXES_COUNT)
		return *sPokemonBoxWallpaperPtrs[boxId];
	else
		return 0;
}

void SetBoxWallpaper(u8 boxId, u8 wallpaperId)
{
	if (boxId < TOTAL_BOXES_COUNT && wallpaperId < WALLPAPER_COUNT)
		*(sPokemonBoxWallpaperPtrs[boxId]) = wallpaperId;
}

s8 sub_80916F4(u8 boxId)
{
	u8 i;
	u8 currentBox = StorageGetCurrentBox();

	for (i = 0; currentBox != boxId; i++)
	{
		currentBox++;
		if (currentBox >= TOTAL_BOXES_COUNT)
			currentBox = 0;
	}

	return (i < TOTAL_BOXES_COUNT / 2) ? 1 : -1;
}

s16 sub_808BDE8(struct CompressedPokemon *boxMons, u8 currIndex, u8 maxIndex, u8 arg3)
{
	s16 i;
	s16 adder = -1;

	if (arg3 < 2)
		adder = 1;

	if (arg3 == 1 || arg3 == 3)
	{
		for (i = (s8)currIndex + adder; i >= 0 && i <= maxIndex; i += adder)
		{
			if (boxMons[i].substruct0.species != SPECIES_NONE)
				return i;
		}
	}
	else
	{
		for (i = (s8) currIndex + adder; i >= 0 && i <= maxIndex; i += adder)
		{
			struct BoxPokemon boxMon;
			CreateBoxMonFromCompressedMon(&boxMon, &boxMons[i]);
			if (boxMon.substruct0.species != SPECIES_NONE
			&& !GetBoxMonData(&boxMon, MON_DATA_IS_EGG, NULL))
				return i;
		}
	}

	return -1;
}

u8 SendMonToPC(struct Pokemon* mon)
{
	s32 boxNo, boxPos;

	SetPCBoxToSendMon(VarGet(VAR_PC_BOX_TO_SEND_MON));

	boxNo = StorageGetCurrentBox();

	do
	{
		for (boxPos = 0; boxPos < IN_BOX_COUNT; boxPos++)
		{
			struct CompressedPokemon* checkingMon = GetCompressedMonPtr(boxNo, boxPos);
			if (checkingMon->substruct0.species == SPECIES_NONE)
			{
				CreateCompressedMonFromBoxMon((struct BoxPokemon*) mon, checkingMon);
				gSpecialVar_MonBoxId = boxNo;
				gSpecialVar_MonBoxPos = boxPos;
				if (GetPCBoxToSendMon() != boxNo)
					FlagClear(FLAG_SHOWN_BOX_WAS_FULL_MESSAGE);
				VarSet(VAR_PC_BOX_TO_SEND_MON, boxNo);
				return MON_GIVEN_TO_PC;
			}
		}

		boxNo++;
		if (boxNo == TOTAL_BOXES_COUNT)
			boxNo = 0;

	} while (boxNo != StorageGetCurrentBox());

	return MON_CANT_GIVE;
}


bool8 SendMonToBoxPos(struct Pokemon* mon, u8 boxNo, u8 boxPos)
{
	if (boxNo > TOTAL_BOXES_COUNT || boxPos > IN_BOX_COUNT)
		return TRUE;

	struct CompressedPokemon* checkingMon = GetCompressedMonPtr(boxNo, boxPos);
	CreateCompressedMonFromBoxMon((struct BoxPokemon*) mon, checkingMon);
	gSpecialVar_MonBoxId = boxNo;
	gSpecialVar_MonBoxPos = boxPos;
	if (GetPCBoxToSendMon() != boxNo)
		FlagClear(FLAG_SHOWN_BOX_WAS_FULL_MESSAGE);
	VarSet(VAR_PC_BOX_TO_SEND_MON, boxNo);
	return FALSE;

}

u32 SummaryScreenBoxMonMultiplier(u8 index)
{
	return index * sizeof(struct CompressedPokemon);
}

void BackupPartyToTempTeam(u8 firstId, u8 numPokes)
{
	struct CompressedPokemon* backup = gTempTeamBackup;

	for (int i = 0; i < numPokes; ++i)
		CreateCompressedMonFromBoxMon((struct BoxPokemon*) &gPlayerParty[firstId + i], &backup[firstId + i]);
}

void RestorePartyFromTempTeam(u8 firstId, u8 numPokes)
{
	struct CompressedPokemon* backup = gTempTeamBackup;

	for (int i = 0; i < numPokes; ++i)
	{
		Memset(&gPlayerParty[firstId + i], 0, sizeof(struct Pokemon)); //Wipe old data - mainly for wiping struct data that's not included in Box mon *IMPORTANT
		CreateBoxMonFromCompressedMon((struct BoxPokemon*) &gPlayerParty[firstId + i], &backup[firstId + i]);
		CalculateMonStats(&gPlayerParty[firstId + i]);
	}
}

u16 __attribute__((long_call)) PSS_TryLoadMonIconTiles(u16 species);
void __attribute__((long_call)) LoadCursorMonSprite(void);
void __attribute__((long_call)) RefreshCursorMonData(void);
void PlaceBoxMonIcon(u8 boxId, u8 position)
{
    if (boxId >= TOTAL_BOXES_COUNT) //Party mon
    {
        gPSSData->partySprites[position] = gPSSData->movingMonSprite;
        gPSSData->partySprites[position]->oam.priority = 1;
        gPSSData->partySprites[position]->subpriority = 12;
    }
    else
    {
		u16 species = GetBoxMonDataAt(boxId, position, MON_DATA_SPECIES2);
	
		#if (defined SPECIES_HOOPA || defined SPECIES_SHAYMIN)
		//Try an instant sprite change for post placing Hoopa-Unbound or Shaymin-Sky in the PC
		if (species == SPECIES_HOOPA || species == SPECIES_SHAYMIN)
		{
			u16 tileNum = PSS_TryLoadMonIconTiles(species); //Gets the tile number of the mon icon
			if (tileNum != 0xFFFF)
			{
				//Update mon Icon
				gPSSData->movingMonSprite->oam.tileNum = tileNum;
				gPSSData->movingMonSprite->oam.paletteNum = IndexOfSpritePaletteTag(0xDAC0 + GetMonIconPaletteIndexFromSpecies(species));
			}

			//Update front sprite
			u32 otId = GetBoxMonDataAt(boxId, position, MON_DATA_OT_ID);
			gPSSData->cursorMonSpecies = species;
			gPSSData->cursorMonPalette = GetMonSpritePalFromSpeciesAndPersonality(gPSSData->cursorMonSpecies, otId, gPSSData->cursorMonPersonality);
			RefreshCursorMonData();
		}
		#endif

        gPSSData->boxMonsSprites[position] = gPSSData->movingMonSprite;
        gPSSData->boxMonsSprites[position]->oam.priority = 2;
        gPSSData->boxMonsSprites[position]->subpriority = 19 - (position % IN_BOX_ROWS);
    }

    gPSSData->movingMonSprite->callback = SpriteCallbackDummy;
    gPSSData->movingMonSprite = NULL;
}

void FixItemNameInPokemonStorageSystem(void)
{
	const u8* name = ItemId_GetName(gPSSData->cursorMonItem);
	u8 length = StringLength(name);

	StringCopyPadded(gPSSData->cursorMonTexts[3], name, CHAR_SPACE, 14);
	if (length >= 14) //Too long to look nice
	{
		gPSSData->cursorMonTexts[3][12] = CHAR_ELLIPSIS; //End with trailing ...
		gPSSData->cursorMonTexts[3][13] = EOS;
	}
}
