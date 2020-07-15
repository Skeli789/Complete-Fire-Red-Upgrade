#include "defines.h"
#include "../include/battle.h"
#include "../include/bg.h"
#include "../include/event_data.h"
#include "../include/event_object_movement.h"
#include "../include/field_effect.h"
#include "../include/field_message_box.h"
#include "../include/field_player_avatar.h"
#include "../include/field_weather.h"
#include "../include/fieldmap.h"
#include "../include/gpu_regs.h"
#include "../include/map_name_popup.h"
#include "../include/menu.h"
#include "../include/m4a.h"
#include "../include/main.h"
#include "../include/metatile_behavior.h"
#include "../include/overworld.h"
#include "../include/palette.h"
#include "../include/pokemon.h"
#include "../include/pokemon_icon.h"
#include "../include/random.h"
#include "../include/region_map.h"
#include "../include/scanline_effect.h"
#include "../include/script.h"
#include "../include/sprite.h"
#include "../include/start_menu.h"
#include "../include/string_util.h"
#include "../include/text.h"
#include "../include/wild_encounter.h"
#include "../include/window.h"
#include "../include/constants/abilities.h"
#include "../include/constants/field_effects.h"
#include "../include/constants/items.h"
#include "../include/constants/maps.h"
#include "../include/constants/moves.h"
#include "../include/constants/pokedex.h"
#include "../include/constants/region_map_sections.h"
#include "../include/constants/songs.h"
#include "../include/constants/species.h"
#include "../include/gba/io_reg.h"

#include "../include/new/battle_strings.h"
#include "../include/new/build_pokemon.h"
#include "../include/new/daycare.h"
#include "../include/new/dexnav.h"
#include "../include/new/dexnav_config.h"
#include "../include/new/dexnav_data.h"
#include "../include/new/dns.h"
#include "../include/new/util.h"
#include "../include/new/overworld.h"
#include "../include/new/wild_encounter.h"
/*
dexnav.c
	Functions for the simplified DexNav system
		Credits to FBI for the original source: https://github.com/EternalCode/Dexnav
*/

#define sDexNavHudPtr (*((struct DexnavHudData**) 0x203E038))
#define sDexNavGuiPtr (*((struct DexNavGuiData**) 0x203E038))

#define ROW_LAND 0
#define ROW_WATER 1

#define IS_NEWER_UNOWN_LETTER(species) (species >= SPECIES_UNOWN_B && species <= SPECIES_UNOWN_QUESTION)

//This file's functions:
static void DexNavGetMon(u16 species, u8 potential, u8 level, u8 ability, u16* moves, u8 searchLevel, u8 chain);
static u8 FindHeaderIndexWithLetter(u16 species, u8 letter);
static u8 PickTileScreen(u8 targetBehaviour, u8 areaX, u8 areaY, s16 *xBuff, s16 *yBuff, u8 smallScan);
static u8 DexNavPickTile(u8 environment, u8 xSize, u8 ySize, bool8 smallScan);
static u8 ShakingGrass(u8 environment, u8 xSize, u8 ySize, bool8 smallScan);
//static void DexHUDHBlank(void);
static void DexNavProximityUpdate(void);
static void NullSubHBlank(void);
static void DexNavFreeHUD(void);
static void DexNavShowFieldMessage(u8 id);
static void OutlinedFontDraw(u8 spriteId, u8 tileNum, u16 size);
static void DexNavSightUpdate(u8 sight);
static void DexNavIconsVisionUpdate(u8 proximity, u8 searchLevel);
static void Task_ManageDexNavHUD(u8 taskId);
static u8 GetEncounterLevel(u16 species, u8 environment);
static u8 DexNavGenerateMonLevel(u16 species, u8 chainLevel, u8 environment);
static u16 DexNavGenerateHeldItem(u16 species, u8 searchLevel);
static u8 DexNavGenerateHiddenAbility(u16 species, u8 searchLevel);
static u8 DexNavGeneratePotential(u8 searchLevel);
static void DexNavGenerateMoveset(u16 species, u8 searchLevel, u8 encounterLevel, u16* moveLoc);
static void DexNavDrawBlackBars(u8* spriteIdAddr);
static void DexNavDrawSight(u8 sight_lvl, u8* spriteIdAddr);
static void DexNavDrawAbility(u8 ability, u8* spriteIdAddr);
static void DexNavDrawMove(u16 move, u8 searchLevel, u8* spriteIdAddr);
static void DexNavDrawPotential(u8 potential, u8* spriteIdAddr);
static void DexNavHudDrawSpeciesIcon(u16 species, u8* spriteIdAddr);
static void DexNavDrawHeldItem(u8* spriteIdAddr);
static void DexNavDrawIcons(void);
void InitDexNavHUD(u16 species, u8 environment);
static void ExecDexNavHUD(void);

//GUI Functions
static void CleanWindow(u8 windowId);
static void CommitWindow(u8 windowId);
static void PrintDexNavMessage(u8 messageId);
static void PrintDexNavError(void);
static bool8 CapturedAllLandBasedPokemon(void);
static bool8 CapturedAllWaterBasedPokemon(void);
static void CB1_OpenDexNavScan(void);
static void Task_DexNavFadeOutToStartMenu(u8 taskId);
static void Task_DexNavFadeOutToScan(u8 taskId);
static void Task_DexNavWaitForKeyPress(u8 taskId);
static void Task_DexNavFadeIn(u8 taskId);
static void MainCB2_DexNav(void);
static void VBlankCB_DexNav(void);
static bool8 SpeciesInArray(u16 species, u8 indexCount, u8 unownLetter);
static void DexNavPopulateEncounterList(void);
static void PrintGUISpeciesName(u16 species);
static void PrintGUISearchLevel(u16 species);
static void PrintGUILevelBonus(u16 species);
static void PrintGUIHiddenAbility(u16 species);
static void DexNavDisplaySpeciesData(void);
static void DexNavLoadAreaNamesAndInstructions(void);
static void CreateNoDataIcon(s16 x, s16 y);
static void DexNavLoadMonIcons(void);
static void CreateCursor(void);
static void DexNavLoadCapturedAllSymbol(void);
static void InitDexNavGui(void);
static void LoadDexNavGfx(void);
static void ClearTasksAndGraphicalStructs(void);
static void ClearVramOamPlttRegs(void);
static void CB2_DexNav(void);

// ===================================== //
// ===== Dex Nav Pokemon Generator ===== //
// ===================================== //

void DexNavGetMon(u16 species, u8 potential, u8 level, u8 ability, u16* moves, u8 searchLevel, u8 chain)
{
	struct Pokemon* mon = &gEnemyParty[0];

	//Try Force Shiny
	//https://bulbapedia.bulbagarden.net/wiki/DexNav#Shiny_probability
	u32 i, otherValue, numChecks, charmBonus, chainBonus, randBonus;
	otherValue = 0;
	#ifdef ITEM_SHINY_CHARM
	charmBonus = (CheckBagHasItem(ITEM_SHINY_CHARM, 1) > 0) ? 2 : 0;
	#else
	charmBonus = 0;
	#endif
	chainBonus = (chain == 50) ? 5 : (chain == 100) ? 10 : 0;
	randBonus = (Random() % 100 < 4 ? 4 : 0);
	numChecks = 1 + charmBonus + chainBonus + randBonus;

	if (searchLevel > 200)
	{
		otherValue += searchLevel - 200;
		searchLevel = 200;
	}

	if (searchLevel > 100)
	{
		otherValue += (searchLevel * 2) - 200;
		searchLevel = 100;
	}

	if (searchLevel > 0)
		otherValue += searchLevel * 6;

	otherValue /= 100;

	for (i = 0; i < numChecks; ++i)
	{
		if (Random() % 10000 < otherValue)
		{
			FlagSet(FLAG_SHINY_CREATION);
			break;
		}
	}

	//Create standard wild
	CreateWildMon(species, level, FindHeaderIndexWithLetter(species, sDexNavHudPtr->unownLetter - 1), TRUE);

	//Pick potential ivs to set to 31
	u8 iv[3];
	for (i = 0; i < NELEMS(iv); ++i)
		iv[i] = Random() % 6;

	if ((iv[0] != iv[1]) && (iv[0] != iv[2]) && (iv[1] != iv[2]))
	{
		u8 perfectIv = 0x1F;
		if (potential > 2)
			SetMonData(mon, MON_DATA_HP_IV + iv[2], &perfectIv);
		else if (potential > 1)
			SetMonData(mon, MON_DATA_HP_IV + iv[1], &perfectIv);
		else if (potential)
			SetMonData(mon, MON_DATA_HP_IV + iv[0], &perfectIv);
	}

	//Set ability
	if (gBaseStats[species].hiddenAbility == ability)
		mon->hiddenAbility = TRUE;
	else if (gBaseStats[species].ability2 != ABILITY_NONE) //Helps fix a bug where Unown would crash the game in the below function
		GiveMonNatureAndAbility(mon, GetNature(mon), (gBaseStats[species].ability2 == ability) ? 1 : 0, IsMonShiny(mon), TRUE, TRUE); //Make sure details match what was on the HUD

	//Set moves
	for (i = 0; i < MAX_MON_MOVES; ++i)
	{
		mon->moves[i] = sDexNavHudPtr->moveId[i];
		mon->pp[i] = gBattleMoves[moves[i]].pp;
	}

	CalculateMonStats(mon);
}


static u8 FindHeaderIndexWithLetter(u16 species, u8 letter)
{
	if (!InTanobyRuins())
		return 0;

	for (u8 i = 0; i < NUM_LAND_MONS; ++i)
	{
		if (PickUnownLetter(species, i) == letter + 1)
			return i;
	}

	return 0;
}

// ===== Pokemon Field Tile ===== //
u8 GetPlayerDistance(s16 x, s16 y)
{
	u16 deltaX = abs(x - (gSaveBlock1->pos.x + 7));
	u16 deltaY = abs(y - (gSaveBlock1->pos.y + 7));
	return deltaX + deltaY;
}


static bool8 PickTileScreen(u8 targetBehaviour, u8 areaX, u8 areaY, s16 *xBuff, s16 *yBuff, u8 smallScan)
{
	// area of map to cover starting from camera position {-7, -7}
	s16 topX = gSaveBlock1->pos.x - SCANSTART_X + (smallScan * 5);
	s16 topY = gSaveBlock1->pos.y - SCANSTART_Y + (smallScan * 5);
	s16 botX = topX + areaX;
	s16 botY = topY + areaY;

	// loop through every tile in area and evaluate
	while (topY < botY)
	{
		while (topX < botX)
		{
			u32 tileBehaviour = MapGridGetMetatileField(topX, topY, 0xFF);
			u8 blockProperties = GetMetatileAttributeFromRawMetatileBehavior(tileBehaviour, METATILE_ATTRIBUTE_ENCOUNTER_TYPE);

			Var8005 = tileBehaviour;	//020370c2

			//Check NPCs on tile
			bool8 goNext = FALSE;
			for (u8 i = 0; i < MAX_NPCS; ++i)
			{
				if (gEventObjects[i].currentCoords.x == topX && gEventObjects[i].currentCoords.y == topY)
				{
					goNext = TRUE;
					break;
				}
			}
			if (goNext)
			{
				topX += 1;
				continue;
			}

			//Tile must be target behaviour (wild tile) and must be passable
			if (blockProperties & targetBehaviour)
			{
				//Caves and water need to have their encounter values scaled higher
				u8 weight = 0;
				if (targetBehaviour == TILE_FLAG_SURFABLE)
				{
					//Water
					u8 scale = 320 - (smallScan * 200) - (GetPlayerDistance(topX, topY) / 2);
					u8 elevDiff = (IsZCoordMismatchAt((gEventObjects[gPlayerAvatar->spriteId].elevation << 4
						| gEventObjects[gPlayerAvatar->spriteId].currentElevation), topX, topY));

					weight = (Random() % scale <= 1) && elevDiff && !MapGridIsImpassableAt(topX, topY);
				}
				else if (!IsMapTypeOutdoors(GetCurrentMapType()))
				{
					//Cave basically needs another check to see if the tile is passable
					u8 scale = 440 - (smallScan * 200) - (GetPlayerDistance(topX, topY) / 2)  - (2 * (topX + topY));
					Var8002 = scale;
					weight = ((Random() % scale) < 1) && !MapGridIsImpassableAt(topX, topY);
				}
				else //Grass land
				{
					u8 scale = 100 - (GetPlayerDistance(topX, topY) * 2);
					weight = (Random() % scale <= 5) && !MapGridIsImpassableAt(topX, topY);
				}

				if (weight > 0)
				{
					*xBuff = topX;
					*yBuff = topY;
					return TRUE;
				}
			}
			topX += 1;
		}
		topY += 1;
		topX = gSaveBlock1->pos.x - SCANSTART_X + (smallScan * 5);
	}

	return FALSE;
}


static bool8 DexNavPickTile(u8 environment, u8 xSize, u8 ySize, bool8 smallScan)
{
	//Pick a specific tile based on environment
	u8 targetBehaviour = 0;
	switch (environment)
	{
		case ENCOUNTER_TYPE_LAND:
			targetBehaviour = TILE_FLAG_ENCOUNTER_TILE; // grass
			break;
		default:
			targetBehaviour = TILE_FLAG_SURFABLE; // cave, water
			break;
	}
	//Tile pick chance is 5% + 1/5 search level

	//u8 searchLevel = sDexNavHudPtr->searchLevel / 5;
	//u8 chance = searchLevel + 1;
	return PickTileScreen(targetBehaviour, xSize, ySize, &sDexNavHudPtr->tileX, &sDexNavHudPtr->tileY, smallScan);
}


static bool8 ShakingGrass(u8 environment, u8 xSize, u8 ySize, bool8 smallScan)
{
	if (DexNavPickTile(environment, xSize, ySize, smallScan))
	{
		u8 metatileBehaviour = MapGridGetMetatileField(sDexNavHudPtr->tileX, sDexNavHudPtr->tileY, 0xFF);
		gFieldEffectArguments[0] = sDexNavHudPtr->tileX;
		gFieldEffectArguments[1] = sDexNavHudPtr->tileY;
		gFieldEffectArguments[2] = 0xFF; //Below everything
		gFieldEffectArguments[3] = 2; //Normal height
		switch (environment)
		{
			case ENCOUNTER_TYPE_LAND:
				{
					if (!IsMapTypeOutdoors(GetCurrentMapType()))
					{
						if (MetatileBehavior_IsTallGrass(metatileBehaviour)) //Grass in cave
							FieldEffectStart(FLDEFF_SHAKING_GRASS);
						else if (MetatileBehavior_IsLongGrass(metatileBehaviour)) //Really tall grass
							FieldEffectStart(FLDEFF_SHAKING_LONG_GRASS);
						else if (MetatileBehavior_IsSandOrShallowFlowingWater(metatileBehaviour))
							FieldEffectStart(FLDEFF_SAND_HOLE);
						else
							FieldEffectStart(FLDEFF_CAVE_DUST); //Default in caves is dust
					}
					else
					{
						if (MetatileBehavior_IsTallGrass(metatileBehaviour)) //Regular grass
							FieldEffectStart(FLDEFF_SHAKING_GRASS);
						else if (MetatileBehavior_IsLongGrass(metatileBehaviour)) //Really tall grass
							FieldEffectStart(FLDEFF_SHAKING_LONG_GRASS);
						else if (MetatileBehavior_IsSandOrShallowFlowingWater(metatileBehaviour)) //Desert Sand
							FieldEffectStart(FLDEFF_SAND_HOLE);
						else if (MetatileBehavior_IsMountain(metatileBehaviour)) //Rough Terrain
							FieldEffectStart(FLDEFF_CAVE_DUST);
						else //Flowers, etc.
							FieldEffectStart(FLDEFF_REPEATING_SPARKLES); //Default on land is sparkles
					}
					break;
				}
			case ENCOUNTER_TYPE_WATER:
				#ifdef UNBOUND
				if (GetCurrentRegionMapSectionId() == MAPSEC_FLOWER_PARADISE)
					FieldEffectStart(FLDEFF_REPEATING_SPARKLES);
				else
				#endif
				if (IsCurrentAreaVolcano())
					FieldEffectStart(FLDEFF_LAVA_BUBBLES);
				else
					FieldEffectStart(FLDEFF_SPLASHING_WATER);
				break;
			default:
				FieldEffectStart(FLDEFF_REPEATING_SPARKLES); //So the game doesn't crash on something useless
				break;
		};

		//Get spriteId of shaking grass
		for (u32 i = 0; i < MAX_SPRITES; ++i)
		{
			if (gSprites[i].callback == (void*) 0x080DCD1D)
			{
				sDexNavHudPtr->spriteIdShakingGrass = i;
				return TRUE;
			}
		}

		return TRUE;
	}

	return FALSE;
};

 //Causes the game to lag due to interference from DNS :(
/*static void DexHUDHBlank(void)
{
	if (REG_VCOUNT > (160 - 19)) //Fill 19 pixels
	{
		REG_BLDY = 0xC;
		REG_WININ = WININ_WIN0_BG_ALL | WININ_WIN1_BG_ALL | WININ_WIN0_OBJ | WININ_WIN0_CLR | WININ_WIN1_OBJ | WININ_WIN1_CLR;
		REG_BLDCNT = (BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_TGT1_BG3 | BLDCNT_TGT1_OBJ | BLDCNT_TGT1_BD | BLDCNT_TGT2_BG0 | BLDCNT_TGT2_BG1 | BLDCNT_TGT2_BG2 | BLDCNT_TGT2_BG3 | BLDCNT_EFFECT_DARKEN);
	}
	else //Default values from overworld
	{
		REG_WININ = WININ_WIN0_BG_ALL | WININ_WIN1_BG_ALL | WININ_WIN0_OBJ | WININ_WIN1_OBJ;
		REG_BLDCNT = BLDALPHA_BLEND(BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_TGT1_BG3 | BLDCNT_TGT1_OBJ, 0);
	}
};*/


static void DexNavProximityUpdate(void)
{
	sDexNavHudPtr->proximity = GetPlayerDistance(sDexNavHudPtr->tileX, sDexNavHudPtr->tileY);
};


static void NullSubHBlank(void)
{
};

static void DexNavFreeHUD(void)
{
	switch (sDexNavHudPtr->environment)
	{
		case 0:
			if (!IsMapTypeOutdoors(GetCurrentMapType()))
				FieldEffectStop(&gSprites[sDexNavHudPtr->spriteIdShakingGrass], 0x1A); //cave
			else
				FieldEffectStop(&gSprites[sDexNavHudPtr->spriteIdShakingGrass], 0x13);
			break;
		case 1:
			FieldEffectStop(&gSprites[sDexNavHudPtr->spriteIdShakingGrass], 0x13);
			break;
	}

	//Clear mon icon sprite
	SafeFreeMonIconPalette(sDexNavHudPtr->species);
	DestroyMonIconSprite(&gSprites[sDexNavHudPtr->spriteIdSpecies]);

	//Clear black bars
	FieldEffectFreeGraphicsResources(&gSprites[sDexNavHudPtr->spriteIdBlackBar[0]]);
	FreeSpriteOamMatrix(&gSprites[sDexNavHudPtr->spriteIdBlackBar[0]]);
	FreeSpriteOamMatrix(&gSprites[sDexNavHudPtr->spriteIdBlackBar[1]]);
	DestroySprite(&gSprites[sDexNavHudPtr->spriteIdBlackBar[0]]);
	DestroySprite(&gSprites[sDexNavHudPtr->spriteIdBlackBar[1]]);

	if (sDexNavHudPtr->spriteIdSight < MAX_SPRITES)
	{
		FieldEffectFreeGraphicsResources(&gSprites[sDexNavHudPtr->spriteIdSight]);
		DestroySprite(&gSprites[sDexNavHudPtr->spriteIdSight]);
	}

	if (sDexNavHudPtr->spriteIdBButton < MAX_SPRITES)
	{
		FieldEffectFreeGraphicsResources(&gSprites[sDexNavHudPtr->spriteIdBButton]);
		DestroySprite(&gSprites[sDexNavHudPtr->spriteIdBButton]);
	}

	if (sDexNavHudPtr->spriteIdAbility < MAX_SPRITES)
	{
		FieldEffectFreeGraphicsResources(&gSprites[sDexNavHudPtr->spriteIdAbility]);
		DestroySprite(&gSprites[sDexNavHudPtr->spriteIdAbility]);
	}

	if (sDexNavHudPtr->spriteIdMove < MAX_SPRITES)
	{
		FieldEffectFreeGraphicsResources(&gSprites[sDexNavHudPtr->spriteIdMove]);
		DestroySprite(&gSprites[sDexNavHudPtr->spriteIdMove]);
	}

	if (sDexNavHudPtr->spriteIdItem < MAX_SPRITES)
	{
		FieldEffectFreeGraphicsResources(&gSprites[sDexNavHudPtr->spriteIdItem]);
		DestroySprite(&gSprites[sDexNavHudPtr->spriteIdItem]);
	}

	if (sDexNavHudPtr->spriteIdPotential[0] < MAX_SPRITES)
	{
		FieldEffectFreeGraphicsResources(&gSprites[sDexNavHudPtr->spriteIdPotential[0]]);
		DestroySprite(&gSprites[sDexNavHudPtr->spriteIdPotential[0]]);
	}

	if (sDexNavHudPtr->spriteIdPotential[1] < MAX_SPRITES)
	{
		FieldEffectFreeGraphicsResources(&gSprites[sDexNavHudPtr->spriteIdPotential[1]]);
		DestroySprite(&gSprites[sDexNavHudPtr->spriteIdPotential[1]]);
	}

	if (sDexNavHudPtr->spriteIdPotential[2] < MAX_SPRITES)
	{
		FieldEffectFreeGraphicsResources(&gSprites[sDexNavHudPtr->spriteIdPotential[2]]);
		DestroySprite(&gSprites[sDexNavHudPtr->spriteIdPotential[2]]);
	}

	//FreeSpriteTilesByTag(0x4736);
	FreeSpriteTilesByTag(0x61);
	FreeSpriteTilesByTag(0x2613);
	FreeSpriteTilesByTag(0x5424);
	FreeSpriteTilesByTag(0x5425);
	FreeSpriteTilesByTag(0x8472);
	FreeSpriteTilesByTag(0x1EE7); //Font Sprite
	FreeSpriteTilesByTag(0xFDF1); //Black Bars
	FreeSpriteTilesByTag(0x3039);
	FreeSpritePaletteByTag(0x8472);
	FreeSpritePaletteByTag(0x3039);

	Free(sDexNavHudPtr);
	DisableInterrupts(2);
	SetHBlankCallback(NullSubHBlank);

/*
	// WRITE_REG_WININ(0x1F1F);
	REG_WININ = WININ_BUILD(WIN_BG0 | WIN_BG1 | WIN_BG2 | WIN_BG3 | WIN_OBJ, WIN_BG0 |
							WIN_BG1 | WIN_BG2 | WIN_BG3 | WIN_OBJ);
	//WRITE_REG_BLDCNT(0x401E);
	REG_BLDCNT = BLDALPHA_BLEND(BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_TGT1_BG3 | BLDCNT_TGT1_OBJ, 0);


	// WRITE_REG_WININ(0x1F1F);
	REG_WININ = WININ_BUILD(WIN_BG0 | WIN_BG1 | WIN_BG2 | WIN_BG3 | WIN_OBJ, WIN_BG0 |
		WIN_BG1 | WIN_BG2 | WIN_BG3 | WIN_OBJ);
	//WRITE_REG_BLDCNT(0x401E);
	REG_BLDCNT = BLDALPHA_BLEND(BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_TGT1_BG3 | BLDCNT_TGT1_OBJ, 0);*/
}

extern const u8 SystemScript_DisplayDexnavMsg[];
static void DexNavShowFieldMessage(u8 id)
{
	u16 species = sDexNavHudPtr->species;
	TryRandomizeSpecies(&species);

	ScriptContext2_Enable();
	DismissMapNamePopup();
	ScriptContext1_SetupScript(SystemScript_DisplayDexnavMsg);
	GetSpeciesName(gStringVar1, species);

	switch(id)
	{
		case FIELD_MSG_NOT_IN_AREA:
			gLoadPointer = gText_CannotBeFound;
			break;
		case FIELD_MSG_LOOK_IN_OTHER_SPOT:
			gLoadPointer = gText_NotFoundNearby;
			break;
		case FIELD_MSG_TOO_DARK:
			gLoadPointer = gText_TooDarkForDexNav;
			break;
		case FIELD_MSG_GOT_AWAY:
			gLoadPointer = gText_GotAway;
			break;
		case FIELD_MSG_LOST_SIGNAL:
			gLoadPointer = gText_LostSignal;
			break;
		case FIELD_MSG_SNEAK_NEXT_TIME:
			gLoadPointer = gText_GotAwayShouldSneak;
			break;
	}
}


static void OutlinedFontDraw(u8 spriteId, u8 tileNum, u16 size)
{
	if (spriteId >= MAX_SPRITES)
		return;

	u8* originalDst, *dst, *toWrite, *strPtr, tile;
	tile = gSprites[spriteId].oam.tileNum + tileNum;
	toWrite = (u8*)((tile * TILE_SIZE) + SPRITE_RAM);
	originalDst = dst = Calloc(size + TILE_SIZE);
	strPtr = gStringVar4;

	u8 counter = 1;
	u16 index = 320;
	u16 prevIndex;

	u8 element = *strPtr;

	while (element != 0xFF)
	{
		prevIndex = index;
		if ((element <= 0xEE) && (element >= 0xD5))
		{
			// lower case letters
			index = (((element - 0xD5) * TILE_SIZE) + 1600);
		}
		else if ((element <= 0xD4) && (element >= 0xBB))
		{
			// upper case letters
			index = (((element - 0xBB) * TILE_SIZE) + 768);
		}
		else if ((element <= 0xAA) && (element >= 0xA1))
		{
			// numbers
			index = (element - 0xA1) * TILE_SIZE;
		}
		else
		{
			// misc pchars
			u8 symbolId = 0;
			switch (element)
			{
				case 0xF0: // colon
				case 0x0: // space bar
					symbolId = 1;
					break;
				case 0x36: // semi colon used indication of str end
					symbolId = 2;
					break;
				case 0xAC: // question mark
					symbolId = 3;
					break;
				case 0xAE: // dash
					symbolId = 4;
					break;
				case 0xAD: // period
					symbolId = 5;
					break;
				case 0xBA: // slash
					symbolId = 6;
					break;
				case 0xB1: // open double quote
					symbolId = 7;
					break;
				case 0xB2: // close double quote
					symbolId = 8;
					break;
				case 0xB3: // open single quote
					symbolId = 9;
					break;
				case 0xB4: // close single quote
					symbolId = 10;
					break;
				case 0xB0: // elipsis ...
					symbolId = 11;
					break;
				case 0xB8: // comma
					symbolId = 12;
					break;
				case 0xB5: // male
					symbolId = 13;
					//dst =
					break;
				case 0xB6: // f
					symbolId = 14;
					break;
				case 0xFF: // empty
					symbolId = 1;
					break;
			}

			index = (symbolId + 9) * TILE_SIZE;
		}

		if ((counter == 0) || (*(strPtr + 1) == 0xFF))
		{
			// first or last pcharacters don't need pixel merging
			Memcpy(dst, &gInterfaceGfx_dexnavStarsTiles[index], TILE_SIZE);
		}
		else if ((element == 0x0))
		{
			Memcpy(dst, &gInterfaceGfx_dexnavStarsTiles[index], TILE_SIZE);
			u8 *prevLetter = (u8*)(&gInterfaceGfx_dexnavStarsTiles[prevIndex]);
			*(dst + 0) = *(prevLetter + 2);
			*(dst + 4) = *(prevLetter + 6);
			*(dst + 8) = *(prevLetter + 10);
			*(dst + 12) = *(prevLetter + 14);
			*(dst + 16) = *(prevLetter + 18);
			*(dst + 20) = *(prevLetter + 22);
			*(dst + 24) = *(prevLetter + 26);
			*(dst + 28) = *(prevLetter + 30);
		}
		else if ((*(strPtr + 1) != 0xFF))
		{
			// pcharacter in middle, if blank space fill blank with previous pcharacter's last pixel row IFF previous pchar's last pixel row non-empty
			Memcpy((void*)dst, &gInterfaceGfx_dexnavStarsTiles[index], TILE_SIZE);
			u8 *prevLetter = (u8*)(&gInterfaceGfx_dexnavStarsTiles[prevIndex]);
			*(dst) |= (((*(prevLetter + 0) & 0xF) == 0) ? (*(dst + 0) & 0xF) : (*(prevLetter + 0) & 0xF));
			*(dst + 4) |= (((*(prevLetter + 4) & 0xF) == 0) ? (*(dst + 4) & 0xF) : (*(prevLetter + 4) & 0xF));
			*(dst + 8) |= (((*(prevLetter + 8) & 0xF) == 0) ? (*(dst + 8) & 0xF) : (*(prevLetter + 8) & 0xF));
			*(dst + 12) |= (((*(prevLetter + 12) & 0xF) == 0) ? (*(dst + 12) & 0xF) : (*(prevLetter + 12) & 0xF));
			*(dst + 16) |= (((*(prevLetter + 16) & 0xF) == 0) ? (*(dst + 16) & 0xF) : (*(prevLetter + 16) & 0xF));
			*(dst + 20) |= (((*(prevLetter + 20) & 0xF) == 0) ? (*(dst + 20) & 0xF) : (*(prevLetter + 20) & 0xF));
			*(dst + 24) |= (((*(prevLetter + 24) & 0xF) == 0) ? (*(dst + 24) & 0xF) : (*(prevLetter + 24) & 0xF));
			*(dst + 28) |= (((*(prevLetter + 28) & 0xF) == 0) ? (*(dst + 28) & 0xF) : (*(prevLetter + 28) & 0xF));
		}

		if ((counter == 2) && (*(strPtr + 1) != 0xFF))
		{
			// every two pchars, we need to merge
			// 8x8px made of 4x8px from previous pchar and 4x8px of this pchar
			*(dst - 30) = (((*(dst - 30) & 0x0F) == 0) ? (*(dst) & 0xF) :(*(dst - 30) & 0x0F)) | (*(dst) & 0xF0);
			*(dst - 26) = (((*(dst - 26) & 0x0F) == 0) ? (*(dst + 4) & 0xF): (*(dst - 26) & 0x0F))  | (*(dst + 4) & 0xF0);
			*(dst - 22) = (((*(dst - 22) & 0x0F) == 0) ? (*(dst + 8) & 0xF): (*(dst - 22) & 0x0F)) | (*(dst + 8) & 0xF0);
			*(dst - 18) = (((*(dst - 18) & 0x0F) == 0) ? (*(dst + 12) & 0xF): (*(dst - 18) & 0x0F)) | (*(dst + 12) & 0xF0);
			*(dst - 14) = (((*(dst - 14) & 0x0F) == 0) ? (*(dst + 16) & 0xF): (*(dst - 14) & 0x0F)) | (*(dst + 16) & 0xF0);
			*(dst - 10) = (((*(dst - 10) & 0x0F) == 0) ? (*(dst + 20) & 0xF): (*(dst - 10) & 0x0F)) | (*(dst + 20) & 0xF0);
			*(dst - 6) = (((*(dst - 6) & 0x0F) == 0) ? (*(dst + 24) & 0xF): (*(dst - 6) & 0x0F)) | (*(dst + 24) & 0xF0);
			*(dst - 2) = (((*(dst - 2) & 0x0F) == 0) ? (*(dst + 28) & 0xF): (*(dst - 2) & 0x0F)) | (*(dst + 28) & 0xF0);

			// last two pixels unconditional
			*(dst - 29) |= *(dst + 1);
			*(dst - 25) |= *(dst + 5);
			*(dst - 21) |= *(dst + 9);
			*(dst - 17) |= *(dst + 13);
			*(dst - 13) |= *(dst + 17);
			*(dst - 9) |= *(dst + 21);
			*(dst - 5) |= *(dst + 25);
			*(dst - 1) |= *(dst + 29);

			dst -= TILE_SIZE;
			counter = 0;
		}
		counter++;
		dst += TILE_SIZE; // next tile
		strPtr++;
		element = *strPtr;
	}

	Memcpy((void*) toWrite, originalDst, size);
	Free(originalDst);
}


static void DexNavSightUpdate(u8 sight)
{
	u8 spriteId = sDexNavHudPtr->spriteIdSight;
	if (spriteId < MAX_SPRITES)
		StartSpriteAnim(&gSprites[spriteId], sight);
}

static void DexNavIconsVisionUpdate(u8 proximity, u8 searchLevel)
{
	// The sight eye and species icon are always drawn in the HUD
	// 	At search level 2, first move is drawn
	// 	At search level 3, Ability and Held item are drawn
	// 	At search level 5, Potential is drawn

	// species and sight/info are always shown, regardless of proximity
	if (sDexNavHudPtr->spriteIdSpecies < MAX_SPRITES)
		gSprites[sDexNavHudPtr->spriteIdSpecies].invisible = FALSE;

	if (sDexNavHudPtr->spriteIdSight < MAX_SPRITES)
		gSprites[sDexNavHudPtr->spriteIdSight].invisible = FALSE;

	// hide everything at the start, and recalc what needs to be shown
	if (sDexNavHudPtr->spriteIdMove < MAX_SPRITES)
		gSprites[sDexNavHudPtr->spriteIdMove].invisible = TRUE;

	if (sDexNavHudPtr->spriteIdAbility < MAX_SPRITES)
		gSprites[sDexNavHudPtr->spriteIdAbility].invisible = TRUE;

	if (sDexNavHudPtr->spriteIdItem < MAX_SPRITES)
		gSprites[sDexNavHudPtr->spriteIdItem].invisible = TRUE;

	if (sDexNavHudPtr->spriteIdPotential[0] < MAX_SPRITES)
		gSprites[sDexNavHudPtr->spriteIdPotential[0]].invisible = TRUE;

	if (sDexNavHudPtr->spriteIdPotential[1] < MAX_SPRITES)
		gSprites[sDexNavHudPtr->spriteIdPotential[1]].invisible = TRUE;

	if (sDexNavHudPtr->spriteIdPotential[2] < MAX_SPRITES)
		gSprites[sDexNavHudPtr->spriteIdPotential[2]].invisible = TRUE;

	if (proximity < 3)
	{
		// at proximity 5, the sight should start to show and indicate sneaking is required
		DexNavSightUpdate(0);
		if (searchLevel > 1)
		{
			// show move, hide others
			if (sDexNavHudPtr->spriteIdMove < MAX_SPRITES)
				gSprites[sDexNavHudPtr->spriteIdMove].invisible = FALSE;
		}
		if (searchLevel > 2)
		{
			// show ability, move, hide others
			if (sDexNavHudPtr->spriteIdAbility < MAX_SPRITES)
				gSprites[sDexNavHudPtr->spriteIdAbility].invisible = FALSE;
			if (sDexNavHudPtr->heldItem)
			{
				// toggle item view
				if (sDexNavHudPtr->spriteIdItem < MAX_SPRITES)
					gSprites[sDexNavHudPtr->spriteIdItem].invisible = FALSE;
			}
		}
		if (searchLevel > 4)
		{
			if (sDexNavHudPtr->spriteIdPotential[0] < MAX_SPRITES)
				gSprites[sDexNavHudPtr->spriteIdPotential[0]].invisible = FALSE;

			if (sDexNavHudPtr->spriteIdPotential[1] < MAX_SPRITES)
				gSprites[sDexNavHudPtr->spriteIdPotential[1]].invisible = FALSE;

			if (sDexNavHudPtr->spriteIdPotential[2] < MAX_SPRITES)
				gSprites[sDexNavHudPtr->spriteIdPotential[2]].invisible = FALSE;
		}
	}
	else if (proximity <= SNEAKING_PROXIMITY)
		DexNavSightUpdate(1); // Sneaking is required flag
	else
		DexNavSightUpdate(2); // Sneaking is not required
}

extern const u8 SystemScript_StartDexNavBattle[];
static void Task_ManageDexNavHUD(u8 taskId)
{
	//Check for out of range
	if (sDexNavHudPtr->proximity > 20)
	{
		DestroyTask(taskId);
		DexNavFreeHUD();
		DexNavShowFieldMessage(FIELD_MSG_LOST_SIGNAL);
		return;
	}

	//Check for timeout.
	gTasks[taskId].data[1]++;
	if (gTasks[taskId].data[1] > DEXNAV_TIMEOUT * 60)
	{
		gCurrentDexNavChain = 0; //A Pokemon running like this resets the chain
		DestroyTask(taskId);
		DexNavFreeHUD();
		DexNavShowFieldMessage(FIELD_MSG_GOT_AWAY);
		return;
	}

	if (sDexNavHudPtr->proximity <= SNEAKING_PROXIMITY && TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_DASH | PLAYER_AVATAR_FLAG_BIKE)) //If player is close and running then the Pokemon should flee
	{
		gCurrentDexNavChain = 0; //A Pokemon running like this resets the chain
		DestroyTask(taskId);
		DexNavFreeHUD();
		DexNavShowFieldMessage(FIELD_MSG_SNEAK_NEXT_TIME);
		return;
	}

	//Check if script just executed
	if (ScriptContext2_IsEnabled() == TRUE)
	{
		//gCurrentDexNavChain = 0; //Not fair because of the repel pop up
		DestroyTask(taskId);
		DexNavFreeHUD();
		return;
	}

	if (gMain.newKeys & (B_BUTTON | START_BUTTON))
	{
		gCurrentDexNavChain = 0; //A Pokemon running like this resets the chain
		DestroyTask(taskId);
		DexNavFreeHUD();
		PlaySE(SE_POKENAV_OFF);
		return;
	}

	//Caves and water the pokemon moves around
	if ((sDexNavHudPtr->environment == ENCOUNTER_TYPE_WATER || !IsMapTypeOutdoors(GetCurrentMapType()))
	#ifdef UNBOUND
	&& GetCurrentRegionMapSectionId() != MAPSEC_FLOWER_PARADISE
	#endif
	&& sDexNavHudPtr->proximity < 2
	&& sDexNavHudPtr->movementTimes < 2)
	{
		switch(sDexNavHudPtr->environment)
		{
			case ENCOUNTER_TYPE_LAND:
				FieldEffectFreeGraphicsResources(&gSprites[sDexNavHudPtr->spriteIdShakingGrass]);
				FieldEffectActiveListRemove(FLDEFF_CAVE_DUST);
				FieldEffectActiveListRemove(FLDEFF_REPEATING_SPARKLES);
				FieldEffectActiveListRemove(FLDEFF_SHAKING_GRASS);
				FieldEffectActiveListRemove(FLDEFF_SHAKING_LONG_GRASS);
				FieldEffectActiveListRemove(FLDEFF_SAND_HOLE);
				break;
			case ENCOUNTER_TYPE_WATER:
				FieldEffectFreeGraphicsResources(&gSprites[sDexNavHudPtr->spriteIdShakingGrass]);
				FieldEffectActiveListRemove(FLDEFF_SPLASHING_WATER);
				FieldEffectActiveListRemove(FLDEFF_LAVA_BUBBLES);
				break;
			default:
				break;
		};

		while(!ShakingGrass(sDexNavHudPtr->environment, 8, 8, 1));
		sDexNavHudPtr->movementTimes += 1;
	}

	// check for encounter start
	if (sDexNavHudPtr-> proximity < 1)
	{
		DexNavGetMon(sDexNavHudPtr->species, sDexNavHudPtr->potential, sDexNavHudPtr->pokemonLevel,
					sDexNavHudPtr->ability, sDexNavHudPtr->moveId, sDexNavHudPtr->searchLevel, gCurrentDexNavChain);
		DestroyTask(taskId);

		// increment the search level
		u16 dexNum = SpeciesToNationalPokedexNum(sDexNavHudPtr->species);
		if (sSearchLevels[dexNum] < 255)
			sSearchLevels[dexNum] += 1;

		// Freeing only the state, objects and hblank cleared on battle start.
		Free(sDexNavHudPtr);

		gDexNavStartedBattle = TRUE;
		DismissMapNamePopup();
		ScriptContext1_SetupScript(SystemScript_StartDexNavBattle);
/*
		// exclamation point animation over the player
		PlaySE(SE_EXCLAIM);
		MakeExclamationMark(gEventObjects, &gSprites[gPlayerAvatar->spriteId]);
		FieldEffectStart(0x0);

		// do battle
		DoStandardWildBattle();*/
	};

	// HUD needs updating iff player has moved
	DexNavProximityUpdate();
	if (!(gTasks[taskId].data[0] == sDexNavHudPtr->proximity))
	{
		DexNavIconsVisionUpdate(sDexNavHudPtr->proximity, sDexNavHudPtr->searchLevel);
		gTasks[taskId].data[0] = sDexNavHudPtr->proximity;
	}
}


// ===================================== //
// ================ HUD ================ //
// ===================================== //
static u8 GetEncounterLevel(u16 species, u8 environment)
{
	u32 i;
	const struct WildPokemonInfo* landMonsInfo = LoadProperMonsData(LAND_MONS_HEADER);
	const struct WildPokemonInfo* waterMonsInfo = LoadProperMonsData(WATER_MONS_HEADER);

	u8 min = 100;
	u8 max = 0;

	switch (environment)
	{
		case 0:	// grass
			if (landMonsInfo == NULL)
				return MAX_LEVEL + 1; //Hidden pokemon should only appear on walkable tiles or surf tiles

			for (i = 0; i < NUM_LAND_MONS; ++i)
			{
				struct WildPokemon monData = landMonsInfo->wildPokemon[i];
				if (monData.species == species)
				{
					min = (min < monData.minLevel) ? min : monData.minLevel;
					max = (max > monData.maxLevel) ? max : monData.maxLevel;
					break;
				}
			}

			if (i >= NUM_LAND_MONS) //Pokemon not found here
				return MAX_LEVEL + 1;
			break;

		case 1:	//water
			if (waterMonsInfo == NULL)
				return MAX_LEVEL + 1; //Hidden pokemon should only appear on walkable tiles or surf tiles

			for (i = 0; i < NUM_WATER_MONS; ++i)
			{
				struct WildPokemon monData = waterMonsInfo->wildPokemon[i];
				if (monData.species == species)
				{
					min = (min < monData.minLevel) ? min : monData.minLevel;
					max = (max > monData.maxLevel) ? max : monData.maxLevel;
					break;
				}
			}

			if (i >= NUM_WATER_MONS) //Pokemon not found here
				return MAX_LEVEL + 1;
			break;

		default:
			return MAX_LEVEL + 1;
	}

	if (max == 0)
		return 0xFF; //Free dexnav display message

	//Mod div by 0 edge case.
	if (min == max)
		return min;

	return (min + (Random() % (max - min)));
}


static u8 DexNavGenerateMonLevel(u16 species, u8 chainLevel, u8 environment)
{
	u8 levelBase, levelBonus;

	levelBase = GetEncounterLevel(species, environment);
	if (levelBase > MAX_LEVEL)
		return 0;

	levelBonus = chainLevel / 5;

	if (Random() % 100 < 4) //4% chance of having a +10 level
		levelBonus += 10;

	if (levelBase + levelBonus > MAX_LEVEL)
		return MAX_LEVEL;
	else
		return levelBase + levelBonus;
}


static u16 DexNavGenerateHeldItem(u16 species, u8 searchLevel)
{
	u16 randVal = Random() % 100;
	u8 searchLevelInfluence = searchLevel >> 1;
	u16 item1 = gBaseStats[species].item1;
	u16 item2 = gBaseStats[species].item2;

	// if both are the same, 100% to hold
	if (item1 == item2)
		return item1;

	// if no items can be held, then yeah...no items
	if (item2 == ITEM_NONE && item1 == ITEM_NONE)
		return ITEM_NONE;

	// if only one entry, 50% chance
	if (item2 == ITEM_NONE && item1 != ITEM_NONE)
		return (randVal < 50) ? item1 : ITEM_NONE;

	// if both are distinct item1 = 50% + srclvl/2; item2 = 5% + srchlvl/2
	if (randVal < (50 + searchLevelInfluence + 5 + searchLevel))
		return (randVal > 5 + searchLevelInfluence) ? item1 : item2;
	else
		return ITEM_NONE;

	return ITEM_NONE;
}


static u8 DexNavGenerateHiddenAbility(u16 species, u8 searchLevel)
{
	bool8 genAbility = FALSE;
	u16 randVal = Random() % 100;
	TryRandomizeSpecies(&species);

	if (searchLevel < 5)
	{
		#if (SEARCHLEVEL0_ABILITYCHANCE != 0)
		if (randVal < SEARCHLEVEL0_ABILITYCHANCE)
			genAbility = TRUE;
		#endif
	}
	else if (searchLevel < 10)
	{
		#if (SEARCHLEVEL5_ABILITYCHANCE != 0)
		if (randVal < SEARCHLEVEL5_ABILITYCHANCE)
			genAbility = TRUE;
		#endif
	}
	else if (searchLevel < 25)
	{
		#if (SEARCHLEVEL10_ABILITYCHANCE != 0)
		if (randVal < SEARCHLEVEL10_ABILITYCHANCE)
			genAbility = TRUE;
		#endif
	}
	else if (searchLevel < 50)
	{
		#if (SEARCHLEVEL25_ABILITYCHANCE != 0)
		if (randVal < SEARCHLEVEL25_ABILITYCHANCE)
			genAbility = TRUE;
		#endif
	}
	else if (searchLevel < 100)
	{
		#if (SEARCHLEVEL50_ABILITYCHANCE != 0)
		if (randVal < SEARCHLEVEL50_ABILITYCHANCE)
			genAbility = TRUE;
		#endif
	}
	else
	{
		#if (SEARCHLEVEL100_ABILITYCHANCE != 0)
		if (randVal < SEARCHLEVEL100_ABILITYCHANCE)
			genAbility = TRUE;
		#endif
	}

	if (genAbility && gBaseStats[species].hiddenAbility != ABILITY_NONE
	&& GetSetPokedexFlag(SpeciesToNationalPokedexNum(species), FLAG_GET_CAUGHT)) //Only give hidden ability if Pokemon has been caught before
		return TryRandomizeAbility(gBaseStats[species].hiddenAbility, species);
	else
	{
		//Pick a normal ability of that Pokemon
		u8 ability;
		if (gBaseStats[species].ability2 != ABILITY_NONE)
			ability = (randVal & 1) == 0 ? gBaseStats[species].ability1 : gBaseStats[species].ability2;
		else
			ability = gBaseStats[species].ability1;

		return TryRandomizeAbility(ability, species);
	}
}

#pragma GCC diagnostic ignored "-Wtype-limits"
static u8 DexNavGeneratePotential(u8 searchLevel)
{
	u8 genChance = 0;
	u16 randVal = Random() % 100;
	if (searchLevel < 5)
	{
		genChance = SEARCHLEVEL0_ONESTAR + SEARCHLEVEL0_TWOSTAR + SEARCHLEVEL0_THREESTAR;
		if (randVal < genChance)
		{
			// figure out which star it is
			if (randVal < SEARCHLEVEL0_ONESTAR)
				return 1;
			else if (randVal < (SEARCHLEVEL0_ONESTAR + SEARCHLEVEL0_TWOSTAR))
				return 2;
			else
				return 3;
		}
	}
	else if (searchLevel < 10)
	{
		genChance = SEARCHLEVEL5_ONESTAR + SEARCHLEVEL5_TWOSTAR + SEARCHLEVEL5_THREESTAR;
		if (randVal < genChance)
		{
			// figure out which star it is
			if (randVal < SEARCHLEVEL5_ONESTAR)
				return 1;
			else if (randVal < (SEARCHLEVEL5_ONESTAR + SEARCHLEVEL5_TWOSTAR))
				return 2;
			else
				return 3;
		}
	}
	else if (searchLevel < 25)
	{
		genChance = SEARCHLEVEL10_ONESTAR + SEARCHLEVEL10_TWOSTAR + SEARCHLEVEL10_THREESTAR;
		if (randVal < genChance)
		{
			// figure out which star it is
			if (randVal < SEARCHLEVEL10_ONESTAR)
				return 1;
			else if (randVal < (SEARCHLEVEL10_ONESTAR + SEARCHLEVEL10_TWOSTAR))
				return 2;
			else
				return 3;
		}
	}
	else if (searchLevel < 50)
	{
		genChance = SEARCHLEVEL25_ONESTAR + SEARCHLEVEL25_TWOSTAR + SEARCHLEVEL25_THREESTAR;
		if (randVal < genChance)
		{
			// figure out which star it is
			if (randVal < SEARCHLEVEL25_ONESTAR)
				return 1;
			else if (randVal < (SEARCHLEVEL25_ONESTAR + SEARCHLEVEL25_TWOSTAR))
				return 2;
			else
				return 3;
		}
	}
	else if (searchLevel < 100)
	{
		genChance = SEARCHLEVEL50_ONESTAR + SEARCHLEVEL50_TWOSTAR + SEARCHLEVEL50_THREESTAR;
		if (randVal < genChance)
		{
			// figure out which star it is
			if (randVal < SEARCHLEVEL50_ONESTAR)
				return 1;
			else if (randVal < (SEARCHLEVEL50_ONESTAR + SEARCHLEVEL50_TWOSTAR))
				return 2;
			else
				return 3;
		}
	}
	else
	{
		genChance = SEARCHLEVEL100_ONESTAR + SEARCHLEVEL100_TWOSTAR + SEARCHLEVEL100_THREESTAR;
		if (randVal < genChance)
		{
			// figure out which star it is
			if (randVal < SEARCHLEVEL100_ONESTAR)
				return 1;
			else if (randVal < (SEARCHLEVEL100_ONESTAR + SEARCHLEVEL100_TWOSTAR))
				return 2;
			else
				return 3;
		}
	}
	// No potential
	return 0;
}


static void DexNavGenerateMoveset(u16 species, u8 searchLevel, u8 encounterLevel, u16* moveLoc)
{
	bool8 genMove = FALSE;
	u16 randVal = Random() % 100;

	//Evaluate if Pokemon should get an egg move in first slot
	if (searchLevel < 5)
	{
		#if (SEARCHLEVEL0_MOVECHANCE != 0)
		if (randVal < SEARCHLEVEL0_MOVECHANCE)
			genMove = TRUE;
		#endif
	}
	else if (searchLevel < 10)
	{
		#if (SEARCHLEVEL5_MOVECHANCE != 0)
		if (randVal < SEARCHLEVEL5_MOVECHANCE)
			genMove = TRUE;
		#endif
	}
	else if (searchLevel < 25)
	{
		#if (SEARCHLEVEL10_MOVECHANCE != 0)
		if (randVal < SEARCHLEVEL10_MOVECHANCE)
			genMove = TRUE;
		#endif
	}
	else if (searchLevel < 50)
	{
		#if (SEARCHLEVEL25_MOVECHANCE != 0)
		if (randVal < SEARCHLEVEL25_MOVECHANCE)
			genMove = TRUE;
		#endif
	}
	else if (searchLevel < 100)
	{
		#if (SEARCHLEVEL50_MOVECHANCE != 0)
		if (randVal < SEARCHLEVEL50_MOVECHANCE)
			genMove = TRUE;
		#endif
	}
	else
	{
		#if (SEARCHLEVEL100_MOVECHANCE != 0)
		if (randVal < SEARCHLEVEL100_MOVECHANCE)
			genMove = TRUE;
		#endif
	}

	//Generate a wild mon and copy moveset
	CreateWildMon(species, encounterLevel, FindHeaderIndexWithLetter(species, sDexNavHudPtr->unownLetter - 1), TRUE);

	//Store generated mon moves into Dex Nav Struct
	for (int i = 0; i < MAX_MON_MOVES; ++i)
	{
		moveLoc[i] = gEnemyParty[0].moves[i];
	}

	// set first move slot to a random egg move if search level is good enough
	if (genMove == TRUE)
	{
		u16 eggMoveBuffer[EGG_MOVES_ARRAY_COUNT];
		u8 numEggMoves = GetEggMoves(&gEnemyParty[0], &eggMoveBuffer);
		if (numEggMoves != 0)
		{
			u8 index = RandRange(0, numEggMoves);
			moveLoc[0] = eggMoveBuffer[index];
		}
	}
}

static void DexNavDrawBlackBars(u8 spriteIdAddr[2])
{
	LoadCompressedSpriteSheetUsingHeap(&sBlackBarTiles);
	LoadSpritePalette(&sHeldItemSpritePalette);
	spriteIdAddr[0] = CreateSprite(&sBlackBarTemplate, ICONX + 16 + 16, ICONY - 2, 0x0);
	spriteIdAddr[1] = CreateSprite(&sBlackBarTemplate, ICONX + 16 + 144, ICONY - 2, 0x0);
}

static void DexNavDrawSight(u8 sightLevel, u8* spriteIdAddr)
{
	LoadCompressedSpriteSheetUsingHeap(&sSightSpriteSheet);
	LoadSpritePalette(&sHeldItemSpritePalette);
	u8 spriteId = CreateSprite(&sSightTemplate, 176 + (16 / 2), 148 + (8 / 2), 0);
	*spriteIdAddr = spriteId; //Must go before DexNavSightUpdate!
	if (spriteId < MAX_SPRITES)
		DexNavSightUpdate(sightLevel);
};

static void DexNavDrawBButton(u8* spriteIdAddr)
{
	LoadCompressedSpriteSheetUsingHeap(&sBButtonSpriteSheet);
	LoadSpritePalette(&sHeldItemSpritePalette);
	u8 spriteId = CreateSprite(&sBButtonTemplate, 208 + (32 / 2), 148 + (8 / 2), 0);
	*spriteIdAddr = spriteId;
};

static void DexNavDrawAbility(u8 ability, u8* spriteIdAddr)
{
	LoadCompressedSpriteSheetUsingHeap(&sAbilityCanvasSpriteSheet);
	LoadSpritePalette(&sHeldItemSpritePalette);
	u8 spriteId = CreateSprite(&sAbilityCanvasTemplate, ICONX + 80, ICONY + 0x12, 0x0);
	if (spriteId < MAX_SPRITES)
	{
		//Ability name beside move name
		u8 len = sDexNavHudPtr->moveNameLength;
		gSprites[spriteId].pos1.x += ((8 * (len/2)) + (4 * (len % 2)));

		//Copy ability string from table using state id
		CopyAbilityName(gStringVar4, ability);

		//Format string so it's even length or if it's odd ends in two spaces
		len = StringLength(gStringVar4);
		if (!(len % 2))
		{
			gStringVar4[len] = 0x0;
			gStringVar4[len + 1] = 0x0;
			gStringVar4[len + 2] = 0xFF;
		}

		//Draw the Ability name on a blank sprite
		OutlinedFontDraw(spriteId, 0, 32 * 8);
	}

	*spriteIdAddr = spriteId;
}

static void DexNavDrawMove(u16 move, u8 searchLevel, u8* spriteIdAddr)
{
	LoadSpritePalette(&sHeldItemSpritePalette);
	LoadCompressedSpriteSheetUsingHeap(&sMoveCanvasSpriteSheet);
	u8 spriteId = CreateSprite(&sMoveCanvasTemplate, ICONX + 80, ICONY + 0x12, 0x0);
	if (spriteId < MAX_SPRITES)
	{
		// Copy move string from table using state id, add '/' character to the end of it
		u8* ptr = StringCopy(gStringVar4, gMoveNames[move]);
		u8 len = StringLength(gStringVar4);

		if (searchLevel > 2)
		{
			*ptr = 0xBA;
			len += 1;
		}

		// record length of move with slash for ability name to be placed beside it
		sDexNavHudPtr->moveNameLength = len;

		// adjust string to be even chars, if odd end in two spaces
		if (!(len % 2))
		{
			gStringVar4[len] = 0x0;
			gStringVar4[len + 1] = 0x0;
			gStringVar4[len + 2] = 0xFF;
		}
		else
		{
			gStringVar4[len] = 0xFF;
		}

		//Draw the move name on a blank sprite
		OutlinedFontDraw(spriteId, 0, 32 * 8);
	}

	*spriteIdAddr = spriteId;
};

static void DexNavDrawPotential(u8 potential, u8* spriteIdAddr)
{
	// allocate both the lit and unlit star to VRAM
	LoadSpriteSheet(&sStarLitSpriteSheet);
	LoadSpriteSheet(&sStarDullSpriteSheet);
	LoadSpritePalette(&sHeldItemSpritePalette);

	// create star objects and space them according to potential 0 - 3
	u8 i, spriteId;
	for (i = 0; i < 3; ++i)
	{
		if (potential > i)
			spriteId = CreateSprite(&sStarLitTemplate, ICONX + 23 + (i * 8), ICONY + 0x5, 0x0);
		else
			spriteId = CreateSprite(&sStarDullTemplate, ICONX + 23 + (i * 8), ICONY + 0x5, 0x0);

		spriteIdAddr[i] = spriteId;
	}
}

void DexNavHudDrawSpeciesIcon(u16 species, u8* spriteIdAddr)
{
	u32 pid = 0xFFFFFFFF;
	if (species == SPECIES_UNOWN)
		pid = GenerateUnownPersonalityByLetter(sDexNavHudPtr->unownLetter - 1);

	//Load which palette the species icon uses
	TryRandomizeSpecies(&species);
	LoadMonIconPalette(species);

	//Create the icon
	u8 spriteId = CreateMonIcon(species, SpriteCB_PokeIcon, ICONX, ICONY, 0, pid, FALSE);
	*spriteIdAddr = spriteId;
}

void DexNavDrawHeldItem(u8* spriteIdAddr)
{
	//Create SPRITE for held item icon
	LoadSpriteSheet(&sHeldItemSpriteSheet);
	LoadSpritePalette(&sHeldItemSpritePalette);
	u8 spriteId = CreateSprite(&sHeldItemTemplate, ICONX + 0x8, ICONY + 0xC, 0x0);
	*spriteIdAddr = spriteId;
}

static void DexNavDrawIcons(void)
{
	u8 searchLevel = sDexNavHudPtr->searchLevel;
	DexNavDrawBlackBars(sDexNavHudPtr->spriteIdBlackBar);
	DexNavDrawSight(sDexNavHudPtr->proximity, &sDexNavHudPtr->spriteIdSight);
	DexNavDrawBButton(&sDexNavHudPtr->spriteIdBButton);
	DexNavDrawMove(sDexNavHudPtr->moveId[0], searchLevel, &sDexNavHudPtr->spriteIdMove);
	DexNavDrawHeldItem(&sDexNavHudPtr->spriteIdItem);
	DexNavDrawAbility(sDexNavHudPtr->ability, &sDexNavHudPtr->spriteIdAbility);
	DexNavDrawPotential(sDexNavHudPtr->potential, &sDexNavHudPtr->spriteIdPotential[0]);
	DexNavHudDrawSpeciesIcon(sDexNavHudPtr->species, &sDexNavHudPtr->spriteIdSpecies);
}


void InitDexNavHUD(u16 species, u8 environment)
{
	sDexNavHudPtr = Calloc(sizeof(struct DexnavHudData));
	// assign non-objects to struct
	sDexNavHudPtr->species = species;

	if (species != gLastDexNavSpecies)
	{
		gLastDexNavSpecies = species;
		gDexNavCooldown = FALSE; //Don't need to walk another step if searching for a different Pokemon
	}

	if (IS_NEWER_UNOWN_LETTER(species))
	{
		sDexNavHudPtr->unownLetter = species - SPECIES_UNOWN_B + 2; //Because B is 1
		sDexNavHudPtr->species = species = SPECIES_UNOWN;
	}
	else //UNOWN A
		sDexNavHudPtr->unownLetter = 1;

	sDexNavHudPtr->environment = environment;
	u8 searchLevel = sSearchLevels[SpeciesToNationalPokedexNum(species)];
	sDexNavHudPtr->searchLevel = searchLevel;
	sDexNavHudPtr->pokemonLevel = DexNavGenerateMonLevel(species, gCurrentDexNavChain, environment);

	if (Overworld_GetFlashLevel() > 0)
	{
		Free(sDexNavHudPtr);
		DexNavShowFieldMessage(FIELD_MSG_TOO_DARK);
		return;
	}

	if (sDexNavHudPtr->pokemonLevel < 1)
	{
		Free(sDexNavHudPtr);
		DexNavShowFieldMessage(FIELD_MSG_NOT_IN_AREA);
		return;
	}

	// draw shaking grass
	//extern u8 ShakingGrass(u8, u8, u8, bool8);
	if (gDexNavCooldown || !ShakingGrass(environment, 12, 12, 0))
	{
		Free(sDexNavHudPtr);
		gDexNavCooldown = TRUE; //A Pokemon can't be found until the player takes at least one step or searches for another Pokemon manually
		DexNavShowFieldMessage(FIELD_MSG_LOOK_IN_OTHER_SPOT);
		return;
	}

	//Populate sDexNavHudPtr objects
	DexNavGenerateMoveset(species, searchLevel, sDexNavHudPtr->pokemonLevel, &sDexNavHudPtr->moveId[0]);
	sDexNavHudPtr->heldItem = DexNavGenerateHeldItem(species, searchLevel);
	sDexNavHudPtr->ability = DexNavGenerateHiddenAbility(species, searchLevel);
	sDexNavHudPtr->potential = DexNavGeneratePotential(searchLevel);
	DexNavProximityUpdate();

	//Draw icons
	DexNavDrawIcons();

	//Hide icons based on proximity and search level
	DexNavIconsVisionUpdate(sDexNavHudPtr->proximity, searchLevel);

	//Enable Hblank interrupt
	/*EnableInterrupts(2);
	SetHBlankCallback(DexHUDHBlank);*/

	// task update HUD
	u8 taskId = CreateTask(Task_ManageDexNavHUD, 0x1);
	if (taskId != 0xFF)
		gTasks[taskId].data[0] = gSprites[gPlayerAvatar->spriteId].pos1.x;

	IncrementGameStat(GAME_STAT_DEXNAV_SCANNED);
}


// This is called via a c1 from the GUI, while waiting to return to the OW
static void ExecDexNavHUD(void)
{
	if (!gPaletteFade->active && !ScriptContext2_IsEnabled() && gMain.callback2 == CB2_Overworld)
	{
		SetMainCallback1(CB1_Overworld);
		InitDexNavHUD(Var8000, Var8001);
	}
}

bool8 IsDexNavHudActive(void)
{
	return FuncIsActiveTask(Task_ManageDexNavHUD);
}

// ========================================== //
// ================== GUI =================== //
// ========================================== //

//General Util

//Cleans the windows
static void CleanWindow(u8 windowId)
{
	FillWindowPixelBuffer(windowId, PIXEL_FILL(0));
}

//Display commited windows
static void CommitWindow(u8 windowId)
{
	CopyWindowToVram(windowId, 3);
	PutWindowTilemap(windowId);
}

static void PrintDexNavMessage(u8 messageId)
{
	const u8* text;

	switch (messageId)
	{
		case MESSAGE_INVALID:
			text = gText_DexNav_Invalid;
			break;
		case MESSAGE_CHOOSE_MON:
			text = gText_DexNav_ChooseMon;
			break;
		case MESSAGE_REGISTERED:
			text = gText_DexNav_Locked;
			break;
		case MESSAGE_NO_DATA:
			text = gText_DexNav_NoDataForSlot;
			break;
		case MESSAGE_TOO_DARK:
			text = gText_DexNav_TooDark;
			break;
		default:
			text = NULL;
			break;
	}

	CleanWindow(WIN_MESSAGE);
	WindowPrint(WIN_MESSAGE, 1, 2, 8, &sDexNav_WhiteText, 0, text);
	CommitWindow(WIN_MESSAGE);
}

static void PrintDexNavError(void)
{
	if (sDexNavGuiPtr->selectedArr == ROW_WATER
	&&  sDexNavGuiPtr->selectedIndex >> 1 >= sDexNavGuiPtr->numWaterMons + sDexNavGuiPtr->numHiddenWaterMons)
	{
		PrintDexNavMessage(MESSAGE_NO_DATA); //No data in slot
		PlaySE(SE_ERROR);
	}
	else if (sDexNavGuiPtr->selectedArr == ROW_LAND
	&&  sDexNavGuiPtr->selectedIndex >> 1 >= sDexNavGuiPtr->numGrassMons + sDexNavGuiPtr->numHiddenLandMons)
	{
		PrintDexNavMessage(MESSAGE_NO_DATA); //No data in slot
		PlaySE(SE_ERROR);
	}
	else //Selected unidentified Pokemon
	{
		if (Overworld_GetFlashLevel() > 0)
			PrintDexNavMessage(MESSAGE_TOO_DARK); //Prevent bugs by not letting the player search until room is lit up
		else
			PrintDexNavMessage(MESSAGE_INVALID); //Can't be searched yet
		PlaySE(SE_ERROR);
	}
}

//Checks if all Pokemon that can be encountered in the grass/ground have been capture
static bool8 CapturedAllLandBasedPokemon(void)
{
	u16 i, species;
	u8 num = 0;
	const struct WildPokemonInfo* landMonsInfo = LoadProperMonsData(LAND_MONS_HEADER);

	if (landMonsInfo != NULL)
	{
		for (i = 0; i < NUM_LAND_MONS; ++i)
		{
			species = landMonsInfo->wildPokemon[i].species;
			if (species != SPECIES_NONE)
			{
				++num;
				if (!GetSetPokedexFlag(SpeciesToNationalPokedexNum(species), FLAG_GET_CAUGHT))
					break;
			}
		}

		if (i >= NUM_LAND_MONS && num > 0) //All land mons caught and there were land mons to catch
			return TRUE;
	}

	return FALSE;
}

//Checks if all Pokemon that can be encountered while surfing have been capture
static bool8 CapturedAllWaterBasedPokemon(void)
{
	u16 i, species;
	u8 num = 0;
	const struct WildPokemonInfo* waterMonsInfo = LoadProperMonsData(WATER_MONS_HEADER);

	if (waterMonsInfo != NULL)
	{
		for (i = 0; i < NUM_WATER_MONS; ++i)
		{
			species = waterMonsInfo->wildPokemon[i].species;
			if (species != SPECIES_NONE)
			{
				++num;
				if (!GetSetPokedexFlag(SpeciesToNationalPokedexNum(species), FLAG_GET_CAUGHT))
					break;
			}
		}

		if (i >= NUM_WATER_MONS && num > 0) //All water mons caught and there were water mons to catch
			return TRUE;
	}

	return FALSE;
}

//GUI

static void CB1_OpenDexNavScan(void)
{
	ExecDexNavHUD();
}

static void Task_DexNavFadeOutToStartMenu(u8 taskId)
{
	if (!gPaletteFade->active)
	{
		SetMainCallback2(CB2_ReturnToFieldWithOpenMenu);
		Free(sDexNavGuiPtr->tilemapPtr);
		Free(sDexNavGuiPtr);
		FreeAllWindowBuffers();
		DestroyTask(taskId);
	}
}

static void Task_DexNavFadeOutToScan(u8 taskId)
{
	if (!gPaletteFade->active)
	{
		SetMainCallback1(CB1_OpenDexNavScan);
		SetMainCallback2(CB2_ReturnToFieldContinueScript); //Continue script is needed so followers don't get messed up
		Free(sDexNavGuiPtr->tilemapPtr);
		Free(sDexNavGuiPtr);
		FreeAllWindowBuffers();
		DestroyTask(taskId);
	}
}

static void SpriteCB_GUICursor(struct Sprite* sprite)
{
	if (sDexNavGuiPtr->selectedArr == ROW_WATER)
	{
		//Water
		sprite->pos1.x = sCursorPositionsWater[sDexNavGuiPtr->selectedIndex];
		sprite->pos1.y = sCursorPositionsWater[sDexNavGuiPtr->selectedIndex + 1];
	}
	else
	{
		//Grass
		sprite->pos1.x = sCursorPositionsLand[sDexNavGuiPtr->selectedIndex];
		sprite->pos1.y = sCursorPositionsLand[sDexNavGuiPtr->selectedIndex + 1];
	}
}

static void Task_DexNavWaitForKeyPress(u8 taskId)
{
	if (JOY_NEW(A_BUTTON))
	{
		//Check selection is valid. Play sound if invalid
		u16 species = sDexNavGuiPtr->selectedArr == ROW_WATER ? sDexNavGuiPtr->waterSpecies[sDexNavGuiPtr->selectedIndex / 2] : sDexNavGuiPtr->grassSpecies[sDexNavGuiPtr->selectedIndex / 2];

		if (species != SPECIES_NONE && Overworld_GetFlashLevel() == 0) //DexNav doesn't work in dark areas (sprites wouldn't show up)
		{
			if (species == SPECIES_UNOWN)
			{
				u8 letter = sDexNavGuiPtr->unownFormsByDNavIndices[sDexNavGuiPtr->selectedIndex / 2] - 1;
				if (letter > 0)
					species = SPECIES_UNOWN_B + letter - 1;
			}

			//Species was valid, save and enter OW HUD mode
			Var8000 = species;
			Var8001 = sDexNavGuiPtr->selectedArr;
			PlaySE(SE_POKENAV_SEARCHING);
			BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 16, RGB_BLACK);
			gTasks[taskId].func = Task_DexNavFadeOutToScan;
			return;
		}
		else //No species in this slot
		{
			PrintDexNavError();
		}
	}
	else if (JOY_NEW(B_BUTTON))
	{
		BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 16, RGB_BLACK);
		gTasks[taskId].func = Task_DexNavFadeOutToStartMenu;
	}
	else if (JOY_NEW(R_BUTTON))
	{
		//Check selection is valid. Play sound if invalid
		u16 species = sDexNavGuiPtr->selectedArr ? sDexNavGuiPtr->waterSpecies[sDexNavGuiPtr->selectedIndex / 2] : sDexNavGuiPtr->grassSpecies[sDexNavGuiPtr->selectedIndex / 2];

		if (species != SPECIES_NONE && Overworld_GetFlashLevel() == 0)
		{
			if (species == SPECIES_UNOWN)
			{
				u8 letter = sDexNavGuiPtr->unownFormsByDNavIndices[sDexNavGuiPtr->selectedIndex / 2] - 1;
				if (letter > 0)
					species = SPECIES_UNOWN_B + letter - 1;
			}

			//Species was valid
			DexNavDisplaySpeciesData();
			PrintDexNavMessage(MESSAGE_REGISTERED);
			PlaySE(SE_POKENAV_SEARCHING);

			//Create value to store in a var
			u16 varStore = (sDexNavGuiPtr->selectedArr << 15) | species;
			VarSet(VAR_DEXNAV, varStore);

			//Update R-Button mode if applicable
			#ifdef VAR_R_BUTTON_MODE
			VarSet(VAR_R_BUTTON_MODE, OPTIONS_R_BUTTON_MODE_DEXNAV);
			#endif
		}
		else //Not valid species
		{
			PrintDexNavError();
		}
	}
	else if (JOY_REPT(DPAD_DOWN))
	{
		if (sDexNavGuiPtr->selectedArr == ROW_LAND)
		{
			if (sDexNavGuiPtr->selectedIndex >= LAND_SECOND_ROW_FIRST_INDEX)
			{
				sDexNavGuiPtr->selectedIndex = MathMin(sDexNavGuiPtr->selectedIndex - LAND_ROW_LENGTH, WATER_ROW_LAST_INDEX);
				sDexNavGuiPtr->selectedArr = ROW_WATER;
			}
			else
			{
				sDexNavGuiPtr->selectedIndex += LAND_ROW_LENGTH; //Move to second row of land
			}
		}
		else //ROW_WATER
		{
			sDexNavGuiPtr->selectedArr = ROW_LAND; //Keep index the same
		}

		DexNavDisplaySpeciesData();
		PrintDexNavMessage(MESSAGE_CHOOSE_MON);
		PlaySE(SE_POKENAV_ONE_BEEP);
	}
	else if (JOY_REPT(DPAD_UP))
	{
		if (sDexNavGuiPtr->selectedArr == ROW_LAND)
		{
			if (sDexNavGuiPtr->selectedIndex >= LAND_SECOND_ROW_FIRST_INDEX)
			{
				sDexNavGuiPtr->selectedIndex -= LAND_ROW_LENGTH; //Move to first row of land
			}
			else
			{
				sDexNavGuiPtr->selectedIndex = MathMin(sDexNavGuiPtr->selectedIndex, WATER_ROW_LAST_INDEX);
				sDexNavGuiPtr->selectedArr = ROW_WATER;
			}
		}
		else //ROW_WATER
		{
			sDexNavGuiPtr->selectedArr = ROW_LAND;
			sDexNavGuiPtr->selectedIndex += LAND_ROW_LENGTH; //So be on the second row
		}

		DexNavDisplaySpeciesData();
		PrintDexNavMessage(MESSAGE_CHOOSE_MON);
		PlaySE(SE_POKENAV_ONE_BEEP);
	}
	else if (JOY_REPT(DPAD_LEFT))
	{
		if (sDexNavGuiPtr->selectedArr == ROW_WATER)
			sDexNavGuiPtr->selectedIndex = (sDexNavGuiPtr->selectedIndex == 0) ? WATER_ROW_LAST_INDEX : sDexNavGuiPtr->selectedIndex - ROW_MON_LENGTH;
		else //ROW_LAND
		{
			if (sDexNavGuiPtr->selectedIndex == 0 || sDexNavGuiPtr->selectedIndex == LAND_SECOND_ROW_FIRST_INDEX)
				sDexNavGuiPtr->selectedIndex += LAND_FIRST_ROW_LAST_INDEX; //Wrap around
			else
				sDexNavGuiPtr->selectedIndex -= ROW_MON_LENGTH;
		}

		DexNavDisplaySpeciesData();
		PrintDexNavMessage(MESSAGE_CHOOSE_MON);
		PlaySE(SE_POKENAV_ONE_BEEP);
	}
	else if (JOY_REPT(DPAD_RIGHT))
	{
		if (sDexNavGuiPtr->selectedArr == ROW_WATER)
			sDexNavGuiPtr->selectedIndex = (sDexNavGuiPtr->selectedIndex == WATER_ROW_LAST_INDEX) ? 0 : sDexNavGuiPtr->selectedIndex + ROW_MON_LENGTH;
		else //ROW_LAND
		{
			if (sDexNavGuiPtr->selectedIndex == LAND_FIRST_ROW_LAST_INDEX || sDexNavGuiPtr->selectedIndex == LAND_SECOND_ROW_LAST_INDEX)
				sDexNavGuiPtr->selectedIndex -= LAND_FIRST_ROW_LAST_INDEX; //Wrap around
			else
				sDexNavGuiPtr->selectedIndex += ROW_MON_LENGTH;
		}

		DexNavDisplaySpeciesData();
		PrintDexNavMessage(MESSAGE_CHOOSE_MON);
		PlaySE(SE_POKENAV_ONE_BEEP);
	}
}

static void Task_DexNavFadeIn(u8 taskId)
{
	if (!gPaletteFade->active)
		gTasks[taskId].func = Task_DexNavWaitForKeyPress;
}

static void MainCB2_DexNav(void)
{
	RunTasks();
	AnimateSprites();
	BuildOamBuffer();
	UpdatePaletteFade();
}

static void VBlankCB_DexNav(void)
{
	LoadOam();
	ProcessSpriteCopyRequests();
	TransferPlttBuffer();
}

static bool8 SpeciesInArray(u16 species, u8 indexCount, u8 unownLetter)
{
	TryRandomizeSpecies(&species);
	u16 dexNum = SpeciesToNationalPokedexNum(species);

	//Disallow species not seen
	if (!GetSetPokedexFlag(dexNum, FLAG_GET_SEEN))
	{
		for (int i = 0; i < NUM_LAND_MONS; ++i)
		{
			#ifdef SPECIES_UNOWN
			if (species == SPECIES_UNOWN && InTanobyRuins())
			{
				if (sDexNavGuiPtr->unownForms[i] == unownLetter) //Already in array
					return TRUE;
			}
			else
			#endif
			if (sDexNavGuiPtr->hiddenSpecies[i] == SPECIES_TABLES_TERMIN)
			{
				sDexNavGuiPtr->hiddenSpecies[i] = dexNum;
				sDexNavGuiPtr->hiddenSpecies[i + 1] = SPECIES_TABLES_TERMIN;
				break;
			}
			else if (sDexNavGuiPtr->hiddenSpecies[i] == dexNum) //Already in array
				return TRUE;
		}

		if (indexCount == NUM_LAND_MONS)
			sDexNavGuiPtr->numHiddenLandMons++; //Increase how many question marks to print
		else
			sDexNavGuiPtr->numHiddenWaterMons++;

		return TRUE;
	}

	for (u8 i = 0; i < indexCount; ++i)
	{
		if (indexCount == NUM_LAND_MONS)
		{
			#ifdef SPECIES_UNOWN
			if (species == SPECIES_UNOWN && InTanobyRuins()) //This Unown code is copied from above b/c either
			{												 //all Unown are seen, or none at all
				if (sDexNavGuiPtr->unownForms[i] == unownLetter) //Already in array
					return TRUE;
			}
			else
			#endif
			{
				u16 wildSpecies = sDexNavGuiPtr->grassSpecies[i];
				TryRandomizeSpecies(&wildSpecies);
				if (SpeciesToNationalPokedexNum(wildSpecies) == dexNum)
					return TRUE;
			}
		}
		else
		{
			u16 wildSpecies = sDexNavGuiPtr->waterSpecies[i];
			TryRandomizeSpecies(&wildSpecies);
			if (SpeciesToNationalPokedexNum(wildSpecies) == dexNum)
				return TRUE;
		}
	}

	return FALSE;
}

static void DexNavPopulateEncounterList(void)
{
	//Populate unique wild grass encounters
	u8 grassIndex = 0;
	u8 waterIndex = 0;
	u16 species;

	const struct WildPokemonInfo* landMonsInfo = LoadProperMonsData(LAND_MONS_HEADER);
	const struct WildPokemonInfo* waterMonsInfo = LoadProperMonsData(WATER_MONS_HEADER);

	sDexNavGuiPtr->hiddenSpecies[0] = SPECIES_TABLES_TERMIN;

	if (landMonsInfo != NULL)
	{
		for (int i = 0; i < NUM_LAND_MONS; ++i)
		{
			species = landMonsInfo->wildPokemon[i].species;
			if (species != SPECIES_NONE && !SpeciesInArray(species, NUM_LAND_MONS, PickUnownLetter(species, i)))
			{
				sDexNavGuiPtr->grassSpecies[grassIndex++] = landMonsInfo->wildPokemon[i].species;

				if (InTanobyRuins())
				{
					sDexNavGuiPtr->unownForms[i] = PickUnownLetter(species, i);
					sDexNavGuiPtr->unownFormsByDNavIndices[grassIndex - 1] = PickUnownLetter(species, i);
				}
			}
		}
	}

	sDexNavGuiPtr->hiddenSpecies[0] = SPECIES_TABLES_TERMIN;

	if (waterMonsInfo != NULL)
	{
		for (int i = 0; i < NUM_WATER_MONS; ++i)
		{
			species = waterMonsInfo->wildPokemon[i].species;
			if (species != SPECIES_NONE && !SpeciesInArray(species, NUM_WATER_MONS, PickUnownLetter(species, i)))
			{
				sDexNavGuiPtr->waterSpecies[waterIndex++] = waterMonsInfo->wildPokemon[i].species;
			}
		}
	}

	#ifdef NATIONAL_DEX_UNOWN
	if (InTanobyRuins() && !GetSetPokedexFlag(NATIONAL_DEX_UNOWN, FLAG_GET_SEEN))
	{ //This is so the right amount of ? appear for Unown in the different chambers
		u16 unowns[NUM_LAND_MONS + 1];
		unowns[0] = SPECIES_TABLES_TERMIN;

		sDexNavGuiPtr->numHiddenLandMons = 0;
		for (int i = 0; i < NUM_LAND_MONS; ++i)
		{
			u8 letter = PickUnownLetter(SPECIES_UNOWN, i);
			if (!CheckTableForSpecies(letter, unowns)) //Table with Unown letters treated like a species table
			{
				unowns[sDexNavGuiPtr->numHiddenLandMons++] = letter;
				unowns[sDexNavGuiPtr->numHiddenLandMons] = SPECIES_TABLES_TERMIN; //Shift end down 1
			}
		}
	}
	#endif

	sDexNavGuiPtr->numGrassMons = grassIndex;
	sDexNavGuiPtr->numWaterMons = waterIndex;
};

static void PrintGUISpeciesName(u16 species)
{
	const u8* text;
	CleanWindow(WIN_SPECIES);

	if (species != SPECIES_NONE)
		text = gSpeciesNames[species];
	else
		text = gText_DexNav_NoInfo;

	WindowPrint(WIN_SPECIES, 0, 0, 4, &sDexNav_BlackText, 0, text);
	CommitWindow(WIN_SPECIES);
}

static void PrintGUISearchLevel(u16 species)
{
	const u8* text;
	CleanWindow(WIN_SEARCH_LEVEL);

	if (species == SPECIES_NONE)
		text = gText_DexNav_NoInfo;
	else
	{
		u16 dexNum = SpeciesToNationalPokedexNum(species);
		ConvertIntToDecimalStringN(gStringVar4, sSearchLevels[dexNum], 0, 4);
		text = gStringVar4;
	}

	WindowPrint(WIN_SEARCH_LEVEL, 0, 0, 4, &sDexNav_BlackText, 0, text);
	CommitWindow(WIN_SEARCH_LEVEL);
}

static void PrintGUILevelBonus(u16 species)
{
	const u8* text;
	u16 dexNum = SpeciesToNationalPokedexNum(species);
	CleanWindow(WIN_LEVEL_BONUS);

	if (species == SPECIES_NONE)
		text = gText_DexNav_NoInfo;
	else
	{
		u8 searchLevelBonus = 0;
		if ((sSearchLevels[dexNum] >> 2) > 20)
			searchLevelBonus = 20;
		else
			searchLevelBonus = (sSearchLevels[dexNum] >> 2);

		ConvertIntToDecimalStringN(gStringVar4, searchLevelBonus, 0, 4);
		text = gStringVar4;
	}

	WindowPrint(WIN_LEVEL_BONUS, 0, 0, 4, &sDexNav_BlackText, 0, text);
	CommitWindow(WIN_LEVEL_BONUS);
}

static void PrintGUIHiddenAbility(u16 species)
{
	const u8* text;
	u16 dexNum = SpeciesToNationalPokedexNum(species);

	CleanWindow(WIN_HIDDEN_ABILITY);

	if (GetSetPokedexFlag(dexNum, FLAG_GET_CAUGHT) || species == SPECIES_NONE) //Only display hidden ability if Pokemon has been caught
	{
		if (species != SPECIES_NONE && gBaseStats[species].hiddenAbility != ABILITY_NONE)
			text = GetAbilityName(gBaseStats[species].hiddenAbility);
		else
			text = gText_DexNav_NoInfo;
	}
	else
		text = gText_DexNav_CaptureToSee;

	WindowPrint(WIN_HIDDEN_ABILITY, 0, 0, 4, &sDexNav_BlackText, 0, text);
	CommitWindow(WIN_HIDDEN_ABILITY);
}

static void DexNavDisplaySpeciesData(void)
{
	u16 species = sDexNavGuiPtr->selectedArr == ROW_WATER ? sDexNavGuiPtr->waterSpecies[sDexNavGuiPtr->selectedIndex >> 1] : sDexNavGuiPtr->grassSpecies[sDexNavGuiPtr->selectedIndex >> 1];
	TryRandomizeSpecies(&species);

	PrintGUISpeciesName(species);
	PrintGUISearchLevel(species);
	PrintGUILevelBonus(species);
	PrintGUIHiddenAbility(species);
}

static void DexNavLoadAreaNamesAndInstructions(void)
{
	const u8* landText;
	const u8* waterText;
	
	CleanWindow(WIN_LAND);
	CleanWindow(WIN_WATER);
	CleanWindow(WIN_MAP_NAME);
	CleanWindow(WIN_INSTRUCTIONS);

	//Print Wild Area Names
	#ifdef UNBOUND
	if (MAP_IS(FLOWER_PARADISE_A))
	{
		landText = gText_PinkFlowers;
		waterText = gText_YellowFlowers;
	}
	else if (MAP_IS(FLOWER_PARADISE_B))
	{
		landText = gText_RedFlowers;
		waterText = gText_BlueFlowers;
	}
	else if (MAP_IS(FLOWER_PARADISE_C))
	{
		landText = gText_PinkAndPurpleFlowers;
		waterText = gText_BlueAndYellowFlowers;
	}
	else if (IsCurrentAreaVolcano())
	{
		landText = gText_DexNavLand;
		waterText = gText_Magma;
	}
	else if (IsCurrentAreaDesert())
	{
		landText = gText_Sand;
		waterText = gText_DexNavWater;
	}
	else
	#endif
	{
		landText = gText_DexNavLand;
		waterText = gText_DexNavWater;
	}

	WindowPrint(WIN_LAND, 1, 5, 6, &sDexNav_WhiteText, 0, landText);
	WindowPrint(WIN_WATER, 1, 4, 6, &sDexNav_WhiteText, 0, waterText);

	//Print Location Name
	GetMapName(gStringVar4, GetCurrentRegionMapSectionId(), 0);
	WindowPrint(WIN_MAP_NAME, 1, 2, 0, &sDexNav_WhiteText, 0, gStringVar4);

	//Print Instructions
	WindowPrint(WIN_INSTRUCTIONS, 0, 5, 0, &sDexNav_WhiteText, 0, gText_DexNavInstructions);

	CommitWindow(WIN_WATER);
	CommitWindow(WIN_LAND);
	CommitWindow(WIN_MAP_NAME);
	CommitWindow(WIN_INSTRUCTIONS);
}

static void CreateNoDataIcon(s16 x, s16 y)
{
	CreateSprite(&sNoDataIconTemplate, x, y, 0); //No data in spot
}

static void DexNavLoadMonIcons(void)
{
	s16 x, y;
	u8 letter;
	u32 pid = 0xFFFFFFFF;
	u8 hiddenLandMons = sDexNavGuiPtr->numHiddenLandMons;
	u8 hiddenWaterMons = sDexNavGuiPtr->numHiddenWaterMons;

	LoadCompressedSpriteSheetUsingHeap(&sNoDataIconSpriteSheet);
	LoadMonIconPalettes();

	for (u8 i = 0; i < NUM_LAND_MONS; ++i)
	{
		u16 species = sDexNavGuiPtr->grassSpecies[i];
		x = 20 + (24 * (i % 6));
		y = 92 + (i > 5 ? 28 : 0);

		if (species == SPECIES_NONE)
		{
			if (hiddenLandMons == 0)
			{
				CreateNoDataIcon(x, y);
				continue;
			}
			else
				hiddenLandMons--;
		}

		letter = sDexNavGuiPtr->unownFormsByDNavIndices[i];
		if (letter > 0)
			pid = GenerateUnownPersonalityByLetter(letter - 1);

		TryRandomizeSpecies(&species);
		CreateMonIcon(species, SpriteCB_PokeIcon, x, y, 0, pid, 0);
	}

	for (u8 i = 0; i < NUM_WATER_MONS; ++i)
	{
		u16 species = sDexNavGuiPtr->waterSpecies[i];
		x = 30 + 24 * i;
		y = 48;

		if (species == SPECIES_NONE)
		{
			if (hiddenWaterMons == 0)
			{
				CreateNoDataIcon(x, y);
				continue;
			}
			else
				hiddenWaterMons--;
		}

		letter = PickUnownLetter(species, i);
		if (letter > 0)
			pid = GenerateUnownPersonalityByLetter(letter - 1);

		TryRandomizeSpecies(&species);
		CreateMonIcon(species, SpriteCB_PokeIcon, x, y, 0, pid, 0);
	}
}

static void CreateCursor(void)
{
	LoadCompressedSpriteSheetUsingHeap(&sCursorSpriteSheet);
	LoadCompressedSpritePaletteUsingHeap(&sCursorSpritePalette);
	sDexNavGuiPtr->cursorId = CreateSprite(&sGUICursorTemplate, 30, 48, 0);
}

static void DexNavLoadCapturedAllSymbol(void)
{
	LoadCompressedSpriteSheetUsingHeap(&sCapturedAllPokemonSpriteSheet);

	if (CapturedAllLandBasedPokemon())
		CreateSprite(&sCapturedAllPokemonSymbolTemplate,  154, 77, 0);

	if (CapturedAllWaterBasedPokemon())
		CreateSprite(&sCapturedAllPokemonSymbolTemplate,  140, 29, 0);
}

static void InitDexNavGui(void)
{
	DexNavPopulateEncounterList();
	DexNavDisplaySpeciesData();
	DexNavLoadAreaNamesAndInstructions();
	PrintDexNavMessage(MESSAGE_CHOOSE_MON);
	DexNavLoadMonIcons();
	CreateCursor();
	DexNavLoadCapturedAllSymbol();
}

static void LoadDexNavGfx(void)
{
	const u8* palette;

	decompress_and_copy_tile_data_to_vram(BG_BACKGROUND, gInterfaceGfx_dexnavGuiTiles, 0, 0, 0);
	LZDecompressWram(gInterfaceGfx_dexnavGuiMap, sDexNavGuiPtr->tilemapPtr);
	
	//Choose palette based on current location
	if (IsMapTypeIndoors(GetCurrentMapType()))
		palette = gInterfaceGfx_DexNavGuiIndoorPal;
	else if (IsCurrentAreaVolcano()) //Load special palette for volcanos
		palette = gInterfaceGfx_DexNavGuiVolcanoPal;
	#ifdef UNBOUND
	else if (MAP_IS(FLOWER_PARADISE_A))
		palette = gInterfaceGfx_DexNavGuiFlowerParadiseAPal;
	else if (MAP_IS(FLOWER_PARADISE_B))
		palette = gInterfaceGfx_DexNavGuiFlowerParadiseBPal;
	else if (MAP_IS(FLOWER_PARADISE_C))
		palette = gInterfaceGfx_DexNavGuiFlowerParadiseCPal;
	#endif
	else if (IsCurrentAreaDarkerCave())
		palette = gInterfaceGfx_DexNavGuiDarkerCavePal;
	else if (GetCurrentMapType() == MAP_TYPE_UNDERGROUND)
		palette = gInterfaceGfx_DexNavGuiCavePal;
	else if (IsCurrentAreaAutumn())
		palette = gInterfaceGfx_DexNavGuiAutumnPal;
	else if (IsCurrentAreaWinter())
		palette = gInterfaceGfx_DexNavGuiWinterPal;
	else if (IsCurrentAreaDesert())
		palette = gInterfaceGfx_DexNavGuiDesertPal;
	else if (IsCurrentAreaSwamp())
		palette = gInterfaceGfx_DexNavGuiSwampPal;
	else
		palette = gInterfaceGfx_dexnavGuiPal;

	LoadCompressedPalette(palette, 0, 0x20);
	LoadPalette(&sDexNavGuiTextPal, 0xF0, 0x20); //Pal 15
}

static void ClearTasksAndGraphicalStructs(void)
{
    ScanlineEffect_Stop();
    ResetTasks();
    ResetSpriteData();
    ResetTempTileDataBuffers();
    ResetPaletteFade();
    FreeAllSpritePalettes();
}

static void ClearVramOamPlttRegs(void)
{
	DmaFill16(3, 0, VRAM, VRAM_SIZE);
	DmaFill32(3, 0, OAM, OAM_SIZE);
	DmaFill16(3, 0, PLTT, PLTT_SIZE);
	SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_ON | DISPCNT_OBJ_1D_MAP);
	SetGpuReg(REG_OFFSET_BG3CNT, DISPCNT_MODE_0);
	SetGpuReg(REG_OFFSET_BG2CNT, DISPCNT_MODE_0);
	SetGpuReg(REG_OFFSET_BG1CNT, DISPCNT_MODE_0);
	SetGpuReg(REG_OFFSET_BG0CNT, DISPCNT_MODE_0);
	SetGpuReg(REG_OFFSET_BG3HOFS, DISPCNT_MODE_0);
	SetGpuReg(REG_OFFSET_BG3VOFS, DISPCNT_MODE_0);
	SetGpuReg(REG_OFFSET_BG2HOFS, DISPCNT_MODE_0);
	SetGpuReg(REG_OFFSET_BG2VOFS, DISPCNT_MODE_0);
	SetGpuReg(REG_OFFSET_BG1HOFS, DISPCNT_MODE_0);
	SetGpuReg(REG_OFFSET_BG1VOFS, DISPCNT_MODE_0);
	SetGpuReg(REG_OFFSET_BG0HOFS, DISPCNT_MODE_0);
	SetGpuReg(REG_OFFSET_BG0VOFS, DISPCNT_MODE_0);
}

static void CB2_DexNav(void)
{
	switch (gMain.state) {
		case 0:
		default:
			SetVBlankCallback(NULL);
			ClearVramOamPlttRegs();
			gMain.state++;
			break;
		case 1:
			ClearTasksAndGraphicalStructs();
			gMain.state++;
			break;
		case 2:
			sDexNavGuiPtr->tilemapPtr = Malloc(0x1000);
			ResetBgsAndClearDma3BusyFlags(0);
			InitBgsFromTemplates(0, sDexNavBgTemplates, NELEMS(sDexNavBgTemplates));
			SetBgTilemapBuffer(BG_BACKGROUND, sDexNavGuiPtr->tilemapPtr);
			gMain.state++;
			break;
		case 3:
			LoadDexNavGfx();
			gMain.state++;
			break;
		case 4:
			if (IsDma3ManagerBusyWithBgCopy() != TRUE)
			{
				ShowBg(BG_TEXT);
				ShowBg(BG_BACKGROUND);
				CopyBgTilemapBufferToVram(BG_BACKGROUND);
				gMain.state++;
			}
			break;
		case 5:
			InitWindows(sDexNavWinTemplates);
			DeactivateAllTextPrinters();
			gMain.state++;
			break;
		case 6:
			BeginNormalPaletteFade(0xFFFFFFFF, 0, 16, 0, RGB_BLACK);
			gMain.state++;
			break;
		case 7:
			SetVBlankCallback(VBlankCB_DexNav);
			InitDexNavGui();
			CreateTask(Task_DexNavFadeIn, 0);
			SetMainCallback2(MainCB2_DexNav);
			gMain.state = 0;
			break;
	}
}

bool8 StartMenuDexNavCallback(void)
{
    if (!gPaletteFade->active)
    {
        PlayRainStoppingSoundEffect();
        DestroySafariZoneStatsWindow();
        CleanupOverworldWindowsAndTilemaps();
		sDexNavGuiPtr = Calloc(sizeof(struct DexNavGuiData));
        SetMainCallback2(CB2_DexNav);
        return TRUE;
    }

    return FALSE;
}

// ========================================== //
// ============ Script Specials ============= //
// ========================================== //

void sp0E2_IsDexNavCompleteHere(void)
{
	const struct WildPokemonInfo* landMonsInfo = LoadProperMonsData(LAND_MONS_HEADER);
	const struct WildPokemonInfo* waterMonsInfo = LoadProperMonsData(WATER_MONS_HEADER);

	gSpecialVar_LastResult = TRUE;

	if (landMonsInfo != NULL && !CapturedAllLandBasedPokemon())
		gSpecialVar_LastResult = FALSE;
	else if (waterMonsInfo != NULL && !CapturedAllWaterBasedPokemon())
		gSpecialVar_LastResult = FALSE;
}
