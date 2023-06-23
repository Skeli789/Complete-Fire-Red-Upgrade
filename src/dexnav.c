#include "defines.h"
#include "../include/battle.h"
#include "../include/bg.h"
#include "../include/event_data.h"
#include "../include/event_object_movement.h"
#include "../include/field_effect.h"
#include "../include/field_effect_helpers.h"
#include "../include/field_message_box.h"
#include "../include/field_player_avatar.h"
#include "../include/field_weather.h"
#include "../include/fieldmap.h"
#include "../include/gpu_regs.h"
#include "../include/international_string_util.h"
#include "../include/item_menu.h"
#include "../include/map_name_popup.h"
#include "../include/menu.h"
#include "../include/m4a.h"
#include "../include/main.h"
#include "../include/metatile_behavior.h"
#include "../include/new_menu_helpers.h"
#include "../include/overworld.h"
#include "../include/palette.h"
#include "../include/pokemon.h"
#include "../include/pokemon_icon.h"
#include "../include/random.h"
#include "../include/region_map.h"
#include "../include/scanline_effect.h"
#include "../include/script.h"
#include "../include/sound.h"
#include "../include/sprite.h"
#include "../include/start_menu.h"
#include "../include/string_util.h"
#include "../include/text.h"
#include "../include/text_window.h"
#include "../include/wild_encounter.h"
#include "../include/window.h"
#include "../include/constants/abilities.h"
#include "../include/constants/field_effects.h"
#include "../include/constants/items.h"
#include "../include/constants/maps.h"
#include "../include/constants/metatile_behaviors.h"
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
#include "../include/new/dns.h"
#include "../include/new/item.h"
#include "../include/new/learn_move.h"
#include "../include/new/overworld.h"
#include "../include/new/wild_encounter.h"
#include "../include/new/util.h"

/*
dexnav.c
	Functions for the simplified DexNav system
		Credits to FBI for the original source: https://github.com/EternalCode/Dexnav
*/

#define sDexNavHudPtr (*((struct DexnavHudData**) 0x203E038))
#define sDexNavGUIPtr (*((struct DexNavGUIData**) 0x203E038))

#define AREA_LAND 0
#define AREA_WATER 1

#define IS_NEWER_UNOWN_LETTER(species) (species >= SPECIES_UNOWN_B && species <= SPECIES_UNOWN_QUESTION)

extern const struct SwarmData gSwarmTable[];

//External functions
extern void sp09A_StopSounds(void);
const u8* __attribute__((long_call)) GetCoordEventScriptAtPosition(struct MapHeader * mapHeader, u16 x, u16 y, u8 z);

//This file's functions
static void DexNavGetMon(u16 species, u8 potential, u8 level, u8 ability, u16* moves, u8 searchLevel, u8 chain);
static u16 TryRandomizePumpkabooForm(u16 species);
static bool8 SpeciesHasMultipleSearchableForms(u16 species);
static u8 FindHeaderIndexWithLetter(u16 species, u8 letter);
static void UpdatePlayerDistances(s16 x, s16 y);
static u8 PickTileScreen(u8 targetBehaviour, u8 widthX, u8 heightY, s16 *xBuff, s16 *yBuff, bool8 mustFindTile);
static u8 DexNavPickTile(u8 environment, u8 xSize, u8 ySize, bool8 mustFindTile);
static u8 ShakingGrass(u8 environment, u8 xSize, u8 ySize, bool8 mustFindTile);
//static void DexHUDHBlank(void);
static void DexNavProximityUpdate(void);
static void StopDexNavFieldEffect(void);
static void DexNavFreeHUD(void);
static void DexNavShowFieldMessage(u8 id);
static void OutlinedFontDraw(u8 spriteId, u16 tileNum, u16 size);
static void DexNavSightUpdate(u8 sight);
static void DexNavIconsVisionUpdate(u8 proximity, u8 searchLevel);
static void Task_ManageDexNavHUD(u8 taskId);
static u8 GetTotalEncounterChance(u16 species, u8 environment);
static u8 GetEncounterLevel(u16 species, u8 environment, bool8 detectorMode);
static u8 DexNavGenerateMonLevel(u16 species, u8 chainLevel, u8 environment, bool8 detectorMode);
static u16 DexNavGenerateHeldItem(u16 species, u8 searchLevel);
static u8 DexNavGenerateHiddenAbility(u16 species, u8 searchLevel);
static u8 DexNavGeneratePotential(u8 searchLevel);
static void DexNavGenerateMoveset(u16 species, u8 searchLevel, u8 encounterLevel, u16* moveLoc);
static void DexNavDrawBlackBar(u8* windowId);
static void DexNavDrawDirectionalArrow(u8* windowId);
static void DexNavDrawChainNumber(u8* spriteIdNumAddr, u8* spriteIdStarAddr);
static void DexNavDrawSight(u8 sight_lvl, u8* spriteIdAddr);
static void DexNavDrawAbility(u8 ability, u16 species, u8* spriteIdAddr);
static void DexNavDrawMove(u16 move, u8 searchLevel, u8* spriteIdAddr);
static void DexNavDrawPotential(u8 potential, u8* spriteIdAddr);
static void DexNavHudDrawSpeciesIcon(u16 species, u8* spriteIdAddr);
static void DexNavDrawHeldItem(u8* spriteIdAddr);
static void DexNavDrawIcons(bool8 detectorMode);
static void ExecDexNavHUD(void);

//GUI Functions
static bool8 AnyPokemonInCurrentArea(void);
static u8 GetTotalLandMonCount(void);
static u8 GetTotalWaterMonCount(void);
static u8 GetTotalWaterMonSlots(void);
static u8 GetWaterRowCount(void);
static bool8 IsFishingEncounterMethod(u8 method);
static bool8 CapturedAllLandBasedPokemon(void);
static bool8 CapturedAllWaterBasedPokemon(void);
static bool8 TryAddSpeciesToArray(u16 species, u8 encounterMethod, u8 indexCount, u8 unownLetter);
static void DexNavPopulateEncounterList(void);
static void RegisterSpecies(u16 species, u8 taskId);
static bool8 CanWaterMonBeSearched(void);
static void CopySpeciesNameToStringVar1_AdjustUnownName(u16 species);
static void CleanWindow(u8 windowId);
static void CleanWindows(void);
static void CommitWindow(u8 windowId);
static void CommitWindows(void);
static void PrintDexNavMessage(u8 messageId);
static void PrintDexNavError(u8 taskId, u8 specificMsgId3);
static void HideDexNavMessage(void);
static void ContextMenuMoveCursorFunc(unusedArg s32 listIndex, bool8 onInit, unusedArg struct ListMenu* list);
static void ContextMenuItemPrintFunc(unusedArg u8 windowId, unusedArg s32 listIndex, unusedArg u8 y);
static void ShowSelectedContextMenu(u8 taskId, u16 species);
static void RemoveContextMenu(u8 taskId);
static const struct TextColor* GetSpeciesNameTextColour(void);
static const struct TextColor* GetSpeciesDetailsTextColour(void);
static void PrintGUIAreaName(void);
static void PrintGUIChainLength(void);
static void PrintGUIAreaDescriptors(void);
static void PrintGUISpeciesName(u16 species);
static void PrintGUISpeciesTypes(u16 species);
static void PrintGUISearchLevel(u16 species);
static void PrintGUIEncounterMethod(u8 encounterMethod);
static void PrintGUIHiddenAbility(u16 species);
static void PrintGUIHeldItems(u16 species);
static void DexNavDisplaySpeciesData(void);
static u8 CreateNoDataIcon(s16 x, s16 y);
static void CreateGrayscaleMonIconPalettes(void);
static u16 GetSpeciesAtCursorPos(void);
static u16 TryAdjustUnownSpeciesAtCursorPos(u16 species);
static u8 GetLandSlotSelected(void);
static u8 GetLandRowSelected(void);
static u8 GetLandRowSlotSelected(void);
static u8 GetWaterSlotSelected(void);
static u8 GetWaterRowSelected(void);
static u8 GetWaterRowSlotSelected(void);
static bool8 IsCurrentRowEmpty(void);
static bool8 IsOnlyOneRow(void);
static void ChangeSelectedRow(s8 change);
static void ChangeSelectedCol(s8 change);
static void TrySetSelectedColumnToLastIndex(void);
static void UpdateSpritePosition(struct Sprite* sprite, u8 rowType, u8 selectedIndex);
static void SpriteCB_GUICursor(struct Sprite* sprite);
static void SpriteCB_GUIRegisteredIcon(struct Sprite* sprite);
static void SpriteCB_LandMonIcon(struct Sprite* sprite);
static void SpriteCB_WaterMonIcon(struct Sprite* sprite);
static void Task_DexNavFadeIn(u8 taskId);
static void Task_PrintMessageThenClose(u8 taskId);
static void Task_DexNavWaitForKeyPress(u8 taskId);
static void Task_WaitForContextMenuPreMessage(u8 taskId);
static void Task_WaitForErrorMessage(u8 taskId);
static void Task_HandleContextMenu(u8 taskId);
static void FreeAndCloseDexNav(u8 taskId);
static void Task_DexNavFadeOutToStartMenu(u8 taskId);
static void Task_DexNavFadeOutToScan(u8 taskId);
static void CB1_OpenDexNavScan(void);
static void MainCB2_DexNav(void);
static void VBlankCB_DexNav(void);
static void DexNavPrintStaticText(void);
static void DexNavLoadMonIcons(void);
static void CreateWaterScrollArrows(void);
static void CreateCursor(void);
static void CreateRegisteredIcon(void);
static void DexNavLoadCapturedAllSymbol(void);
static void InitDexNavGUI(void);
static void LoadDexNavBgGfx(void);
static void ClearTasksAndGraphicalStructs(void);
static void ClearVramOamPlttRegs(void);
static void CB2_DexNav(void);

#include "../include/new/dexnav_data.h"


// ===================================== //
// ===== Dex Nav Pokemon Generator ===== //
// ===================================== //

static void DexNavGetMon(u16 species, u8 potential, u8 level, u8 ability, u16* moves, u8 searchLevel, u8 chain)
{
	struct Pokemon* mon = &gEnemyParty[0];

	//Try Force Shiny
	//https://bulbapedia.bulbagarden.net/wiki/DexNav#Shiny_probability
	u32 i, otherValue, numChecks, chainBonus, randBonus;
	otherValue = 0;
	chainBonus = (chain == 50) ? 5 : (chain == 100) ? 10 : 0;
	randBonus = (Random() % 100 < 4 ? 4 : 0);
	numChecks = 1 + chainBonus + randBonus; //Shiny Charm is handled elsewhere

	if (searchLevel > 200)
	{
		if (searchLevel == 255) //Max Search Level
			otherValue += 999 - 200; //Reward the player for hitting this level by bumping the bonus to 999 (max ORAS num)
		else
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
		u32 check = Random32() % 10000;
		if (check < otherValue)
		{
			FlagSet(FLAG_SHINY_CREATION);
			break;
		}
	}

	//Create standard wild Pokemon
	species = TryRandomizePumpkabooForm(species);
	CreateWildMon(species, level, FindHeaderIndexWithLetter(species, sDexNavHudPtr->unownLetter - 1), TRUE);
	GiveMonXPerfectIVs(mon, potential);

	//Set Ability
	TryRandomizeSpecies(&species);
	if (GetHiddenAbility(species) == ability)
		mon->hiddenAbility = TRUE;
	else if (gBaseStats[species].ability2 != ABILITY_NONE) //Helps fix a bug where Unown would crash the game in the below function
		GiveMonNatureAndAbility(mon, GetNature(mon), (GetAbility2(species) == ability) ? 1 : 0, IsMonShiny(mon), TRUE, TRUE); //Make sure details match what was on the HUD

	//Set moves
	for (i = 0; i < MAX_MON_MOVES; ++i)
		SetMonMoveSlot(mon, moves[i], i);

	//Set item
	SetMonData(mon, MON_DATA_HELD_ITEM, &sDexNavHudPtr->heldItem);

	CalculateMonStats(mon);
	HealMon(mon); //Restore PP and fix HP if IV changed
}

static u16 TryRandomizePumpkabooForm(u16 species)
{
	#ifdef FLAG_POKEMON_RANDOMIZER
	if (!FlagGet(FLAG_POKEMON_RANDOMIZER))
	#endif
	{
		#if (defined NATIONAL_DEX_PUMPKABOO && defined NATIONAL_DEX_GOURGEIST)
		u16 dexNum = SpeciesToNationalPokedexNum(species);
		if (dexNum == NATIONAL_DEX_PUMPKABOO)
		{
			switch (Random() % 4)
			{
				//case 0:
				//	break; //Keep same species
				case 1:
					species = SPECIES_PUMPKABOO_M;
					break;
				case 2:
					species = SPECIES_PUMPKABOO_L;
					break;
				case 3:
					species = SPECIES_PUMPKABOO_XL;
					break;
			}
		}
		else if (dexNum == NATIONAL_DEX_GOURGEIST)
		{
			switch (Random() % 4)
			{
				//case 0:
				//	break; //Keep same species
				case 1:
					species = SPECIES_GOURGEIST_M;
					break;
				case 2:
					species = SPECIES_GOURGEIST_L;
					break;
				case 3:
					species = SPECIES_GOURGEIST_XL;
					break;
			}
		}
		#endif
	}

	return species;
}

static u8 FindHeaderIndexWithLetter(u16 species, u8 letter)
{
	if (!InTanobyRuins())
		return 0;

	for (u8 i = 0; i < MAX_TOTAL_LAND_MONS; ++i)
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

static void UpdatePlayerDistances(s16 x, s16 y)
{
	sDexNavHudPtr->xProximity = abs(x - (gSaveBlock1->pos.x + 7));
	sDexNavHudPtr->yProximity = abs(y - (gSaveBlock1->pos.y + 7));
}

static bool8 IsEncounterTile(s16 x, s16 y, u8 targetBehaviour)
{
	if (MetatileBehavior_IsStairs(MapGridGetMetatileBehaviorAt(x, y)))
		return FALSE; //Can't encounter on stairs because it's wonky on side stairs

	if (GetCoordEventScriptAtPosition(&gMapHeader, x - 7, y - 7, MapGridGetZCoordAt(x, y)))
		return FALSE; //Better not to start an enounter on spot that a script should be activated on

	u32 tileBehaviour = MapGridGetMetatileField(x, y, 0xFF);
	u8 blockProperties = GetMetatileAttributeFromRawMetatileBehavior(tileBehaviour, METATILE_ATTRIBUTE_ENCOUNTER_TYPE);
	return (blockProperties & targetBehaviour) != 0;
}

static bool8 CanTileBeReachedFromTile(s16 oldX, s16 oldY, s16 newX, s16 newY) //Eg. is there edge between two nodes
{
	s16 newXInGrid = newX - sDexNavHudPtr->scanLeftX;
	s16 newYInGrid = newY - sDexNavHudPtr->scanTopY;

	if (sDexNavHudPtr->reachableTiles[newYInGrid][newXInGrid] //Tile is already reachable so it doesn't need to be reached again
	|| sDexNavHudPtr->impassibleTiles[newYInGrid][newXInGrid]) //Tile can't be walked on anyway
		return FALSE;

	u8 behaviour = MapGridGetMetatileBehaviorAt(newX, newY);
	if (MapGridIsImpassableAt(newX, newY)
	|| behaviour == MB_MUDDY_SLOPE //Can't actually make it over this tile
	|| behaviour == MB_CRACKED_FLOOR //Can't actually make it over this tile
	|| (newX - 7) >= gMapHeader.mapLayout->width //DexNav scan stops when leaving the map
	|| (newX - 7) < 0
	|| (newY - 7) >= gMapHeader.mapLayout->height
	|| (newY - 7) < 0)
	{
		sDexNavHudPtr->impassibleTiles[newYInGrid][newXInGrid] = TRUE;
		return FALSE;
	}

	return !IsZCoordMismatchAt(MapGridGetZCoordAt(oldX, oldY), newX, newY); //Must be on same elevation
}

//Only called if a position is actually reachable
//Not a function to limit necessary function calls
//The x and y coords are spliced together to make the array writes faster than by writing manually to a Coords16 array
#define EnqueuePos(queue, queueCount, x, y)                                                          \
{                                                                                                    \
	((u32*) queue)[queueCount] = (x) | ((y) << 16);                                                  \
	sDexNavHudPtr->reachableTiles[y - sDexNavHudPtr->scanTopY][x - sDexNavHudPtr->scanLeftX] = TRUE; \
}

static void UpdateReachableTiles(s16 startX, s16 startY, u32 totalLengthX, u32 totalLengthY)
{
	u32 currQueueIndex = 0;
	u32 queueCount = 1;
	struct Coords16 queue[totalLengthX * totalLengthY];
	Memset(queue, 0x0, sizeof(queue));
	queue[0].x = startX; //Because they're set here and not in EnqueuePos, the player's tile isn't considered (intended)
	queue[0].y = startY;

	while (currQueueIndex < queueCount)
	{
		s16 x = queue[currQueueIndex].x;
		s16 y = queue[currQueueIndex].y;

		if (x - 1 >= sDexNavHudPtr->scanLeftX
		&& CanTileBeReachedFromTile(x, y, x - 1, y))
			EnqueuePos(queue, queueCount++, x - 1, y);

		if (x + 1 <= sDexNavHudPtr->scanRightX
		&& CanTileBeReachedFromTile(x, y, x + 1, y))
			EnqueuePos(queue, queueCount++, x + 1, y);

		if (y - 1 >= sDexNavHudPtr->scanTopY
		&& CanTileBeReachedFromTile(x, y, x, y - 1))
			EnqueuePos(queue, queueCount++, x, y - 1);

		if (y + 1 <= sDexNavHudPtr->scanBottomY
		&& CanTileBeReachedFromTile(x, y, x, y + 1))
			EnqueuePos(queue, queueCount++, x, y + 1);

		++currQueueIndex;
	}

	/* Recursive DFS Approach - may blow stack (like on water)
	//This function is only called if a tile is reachable
	sDexNavHudPtr->reachableTiles[y - sDexNavHudPtr->scanTopY][x - sDexNavHudPtr->scanLeftX] =  TRUE;

	if (x - 1 >= sDexNavHudPtr->scanLeftX
	&& CanTileBeReachedFromTile(x, y, x - 1, y))
		UpdateReachableTiles(x - 1, y);

	if (x + 1 <= sDexNavHudPtr->scanRightX
	&& CanTileBeReachedFromTile(x, y, x + 1, y))
		UpdateReachableTiles(x + 1, y);

	if (y - 1 >= sDexNavHudPtr->scanTopY
	&& CanTileBeReachedFromTile(x, y, x, y - 1))
		UpdateReachableTiles(x, y - 1);

	if (y + 1 <= sDexNavHudPtr->scanBottomY
	&& CanTileBeReachedFromTile(x, y, x, y + 1))
		UpdateReachableTiles(x, y + 1);
	*/
}

static bool8 PickTileScreen(u8 targetBehaviour, u8 widthX, u8 heightY, s16 *xBuff, s16 *yBuff, bool8 mustFindTile)
{
	u32 i, j, totalLengthX, totalLengthY, tileCount, playerX, playerY;
	playerX = gSaveBlock1->pos.x + 7;
	playerY = gSaveBlock1->pos.y + 7;

	sDexNavHudPtr->scanLeftX = playerX - widthX;
	sDexNavHudPtr->scanTopY = playerY - heightY;
	sDexNavHudPtr->scanRightX = playerX + widthX;
	sDexNavHudPtr->scanBottomY = playerY + heightY;
	totalLengthX = sDexNavHudPtr->scanRightX - sDexNavHudPtr->scanLeftX;
	totalLengthY = sDexNavHudPtr->scanBottomY - sDexNavHudPtr->scanTopY;

	struct Coords16 availableTiles[totalLengthX * totalLengthY];
	Memset(availableTiles, 0x0, sizeof(availableTiles));
	Memset(sDexNavHudPtr->reachableTiles, 0x0, sizeof(sDexNavHudPtr->reachableTiles)); //From previous scans if necessary (eg. in caves and on water)
	Memset(sDexNavHudPtr->impassibleTiles, 0x0, sizeof(sDexNavHudPtr->impassibleTiles));

	//First remove any tiles from consideration that have NPCs on them
	for (i = 0; i < EVENT_OBJECTS_COUNT; ++i)
	{
		if (gEventObjects[i].active
		&& gEventObjects[i].currentCoords.x >= sDexNavHudPtr->scanLeftX
		&& gEventObjects[i].currentCoords.x <= sDexNavHudPtr->scanRightX
		&& gEventObjects[i].currentCoords.y >= sDexNavHudPtr->scanTopY
		&& gEventObjects[i].currentCoords.y <= sDexNavHudPtr->scanBottomY)
		{
			s16 x = gEventObjects[i].currentCoords.x - sDexNavHudPtr->scanLeftX;
			s16 y = gEventObjects[i].currentCoords.y - sDexNavHudPtr->scanTopY;
			sDexNavHudPtr->impassibleTiles[y][x] = TRUE;
		}
	}

	//Then determine the tiles that can be reached from the player's current position
	UpdateReachableTiles(playerX, playerY, totalLengthX, totalLengthY);

	//Of the tiles that can be reached, add the encounter tiles to a list
	for (i = 0, tileCount = 0; i < totalLengthY; ++i)
	{
		for (j = 0; j < totalLengthX; ++j)
		{
			if (sDexNavHudPtr->reachableTiles[i][j])
			{
				s16 x = sDexNavHudPtr->scanLeftX + j;
				s16 y = sDexNavHudPtr->scanTopY + i;

				if (IsEncounterTile(x, y, targetBehaviour))
					((u32*) availableTiles)[tileCount++] = x | (y << 16); //Splice them together to make the array write faster
			}
		}
	}

	//Finally pick a random tile of the available tiles
	if (tileCount > 0)
	{
		if (!mustFindTile)
		{
			if (tileCount < 5)
				return FALSE; //Pokemon should never be found in such a small space

			u16 rate = 70 + sDexNavHudPtr->searchLevel; //Pokemon with Search Level 30+ are guaranteed to be found
			if (Random() % 100 > rate)
				return FALSE; //Modulate the encounter rate
		}

		struct Coords16* pos = &availableTiles[Random() % tileCount];
		*xBuff = pos->x;
		*yBuff = pos->y;
		return TRUE;
	}

	return FALSE;
}

static bool8 DexNavPickTile(u8 environment, u8 xSize, u8 ySize, bool8 mustFindTile)
{
	//Pick a specific tile based on environment
	u8 targetBehaviour;
	switch (environment)
	{
		case ENCOUNTER_TYPE_LAND:
			targetBehaviour = TILE_FLAG_ENCOUNTER_TILE; // grass
			break;
		default:
			targetBehaviour = TILE_FLAG_SURFABLE; // cave, water
			break;
	}

	return PickTileScreen(targetBehaviour, xSize, ySize, &sDexNavHudPtr->tileX, &sDexNavHudPtr->tileY, mustFindTile);
}

static bool8 ShakingGrass(u8 environment, u8 xSize, u8 ySize, bool8 mustFindTile)
{
	if (DexNavPickTile(environment, xSize, ySize, mustFindTile))
	{
		u8 fieldEffect;
		u8 metatileBehaviour = MapGridGetMetatileField(sDexNavHudPtr->tileX, sDexNavHudPtr->tileY, 0xFF);
		gFieldEffectArguments[0] = sDexNavHudPtr->tileX;
		gFieldEffectArguments[1] = sDexNavHudPtr->tileY;
		gFieldEffectArguments[2] = 0xFF; //Below everything
		gFieldEffectArguments[3] = ZCoordToPriority(MapGridGetZCoordAt(sDexNavHudPtr->tileX, sDexNavHudPtr->tileY));

		switch (environment)
		{
			case ENCOUNTER_TYPE_LAND:
				if (!IsMapTypeOutdoors(GetCurrentMapType()))
				{
					if (MetatileBehavior_IsTallGrass(metatileBehaviour)) //Grass in cave
						fieldEffect = FLDEFF_SHAKING_GRASS;
					else if (MetatileBehavior_IsLongGrass(metatileBehaviour)) //Really tall grass
						fieldEffect = FLDEFF_SHAKING_LONG_GRASS;
					else if (MetatileBehavior_IsSandOrShallowFlowingWater(metatileBehaviour))
						fieldEffect = FLDEFF_SAND_HOLE;
					else
						fieldEffect = FLDEFF_CAVE_DUST; //Default in caves is dust
				}
				else
				{
					if (MetatileBehavior_IsTallGrass(metatileBehaviour)) //Regular grass
						fieldEffect = FLDEFF_SHAKING_GRASS;
					else if (MetatileBehavior_IsLongGrass(metatileBehaviour)) //Really tall grass
						fieldEffect = FLDEFF_SHAKING_LONG_GRASS;
					else if (MetatileBehavior_IsSandOrShallowFlowingWater(metatileBehaviour)) //Desert Sand
						fieldEffect = FLDEFF_SAND_HOLE;
					else if (MetatileBehavior_IsMountain(metatileBehaviour)) //Rough Terrain
						fieldEffect = FLDEFF_CAVE_DUST;
					else //Flowers, etc.
						fieldEffect = FLDEFF_REPEATING_SPARKLES; //Default on land is sparkles
				}
				break;
			case ENCOUNTER_TYPE_WATER:
				#ifdef UNBOUND
				if (GetCurrentRegionMapSectionId() == MAPSEC_FLOWER_PARADISE)
					fieldEffect = FLDEFF_REPEATING_SPARKLES;
				else
				#endif
				if (IsCurrentAreaVolcano())
					fieldEffect = FLDEFF_LAVA_BUBBLES;
				else
					fieldEffect = FLDEFF_SPLASHING_WATER;
				break;
			default:
				fieldEffect = FLDEFF_REPEATING_SPARKLES; //So the game doesn't crash on something useless
				break;
		}

		FieldEffectStart(fieldEffect);

		//Get spriteId of field effect
		for (u32 i = 0; i < MAX_SPRITES; ++i)
		{
			if (gSprites[i].inUse
			&& gSprites[i].data[0] == fieldEffect
			&& gSprites[i].callback == WaitFieldEffectSpriteAnim)
			{
				sDexNavHudPtr->spriteIdShakingGrass = i;
				break;
			}
		}

		return TRUE;
	}
	else
		return FALSE;
};

static void DexNavProximityUpdate(void)
{
	sDexNavHudPtr->totalProximity = GetPlayerDistance(sDexNavHudPtr->tileX, sDexNavHudPtr->tileY);
	UpdatePlayerDistances(sDexNavHudPtr->tileX, sDexNavHudPtr->tileY); //Used by the directional arrow
}

static void StopDexNavFieldEffect(void)
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
	}
}

static void DexNavFreeHUD(void)
{
	StopDexNavFieldEffect();

	//Clear mon icon sprite
	SafeFreeMonIconPalette(sDexNavHudPtr->species);
	DestroyMonIcon(&gSprites[sDexNavHudPtr->spriteIdSpecies]);

	//Clean arrow
	CleanWindow(sDexNavHudPtr->arrowWindowId);
	CopyWindowToVram(sDexNavHudPtr->arrowWindowId, COPYWIN_BOTH);
	ClearWindowTilemap(sDexNavHudPtr->blackBarWindowId);
	RemoveWindow(sDexNavHudPtr->arrowWindowId);

	//Clean other sprites
	if (sDexNavHudPtr->spriteIdSight < MAX_SPRITES)
		FieldEffectFreeGraphicsResources(&gSprites[sDexNavHudPtr->spriteIdSight]);

	if (sDexNavHudPtr->spriteIdBButton < MAX_SPRITES)
		FieldEffectFreeGraphicsResources(&gSprites[sDexNavHudPtr->spriteIdBButton]);

	if (sDexNavHudPtr->spriteIdChainNumber < MAX_SPRITES)
		FieldEffectFreeGraphicsResources(&gSprites[sDexNavHudPtr->spriteIdChainNumber]);

	if (sDexNavHudPtr->spriteIdChainStar < MAX_SPRITES)
		FieldEffectFreeGraphicsResources(&gSprites[sDexNavHudPtr->spriteIdChainStar]);

	if (sDexNavHudPtr->spriteIdAbility < MAX_SPRITES)
		FieldEffectFreeGraphicsResources(&gSprites[sDexNavHudPtr->spriteIdAbility]);

	if (sDexNavHudPtr->spriteIdMove < MAX_SPRITES)
		FieldEffectFreeGraphicsResources(&gSprites[sDexNavHudPtr->spriteIdMove]);

	if (sDexNavHudPtr->spriteIdItem < MAX_SPRITES)
		FieldEffectFreeGraphicsResources(&gSprites[sDexNavHudPtr->spriteIdItem]);

	if (sDexNavHudPtr->spriteIdPotential[0] < MAX_SPRITES)
		FieldEffectFreeGraphicsResources(&gSprites[sDexNavHudPtr->spriteIdPotential[0]]);

	if (sDexNavHudPtr->spriteIdPotential[1] < MAX_SPRITES)
		FieldEffectFreeGraphicsResources(&gSprites[sDexNavHudPtr->spriteIdPotential[1]]);

	if (sDexNavHudPtr->spriteIdPotential[2] < MAX_SPRITES)
		FieldEffectFreeGraphicsResources(&gSprites[sDexNavHudPtr->spriteIdPotential[2]]);

	//Clear black bar - last so it doesn't look weird
	CleanWindow(sDexNavHudPtr->blackBarWindowId);
	CopyWindowToVram(sDexNavHudPtr->blackBarWindowId, COPYWIN_BOTH);
	ClearWindowTilemap(sDexNavHudPtr->blackBarWindowId);
	RemoveWindow(sDexNavHudPtr->blackBarWindowId);

	//Idk what FBI used these tags for
	//FreeSpriteTilesByTag(0x4736);
	FreeSpriteTilesByTag(0x61);
	FreeSpriteTilesByTag(0x2613);
	FreeSpriteTilesByTag(0x5424);
	FreeSpriteTilesByTag(0x5425);
	FreeSpriteTilesByTag(GFX_TAG_HELD_ITEM);
	FreeSpriteTilesByTag(0x1EE7); //Font Sprite
	FreeSpriteTilesByTag(0xFDF1); //Black Bars
	FreeSpriteTilesByTag(0x3039);
	FreeSpritePaletteByTag(GFX_TAG_HELD_ITEM);
	FreeSpritePaletteByTag(0x3039);

	Free(sDexNavHudPtr);
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
			if (SpeciesHasMultipleSearchableForms(species))
				gLoadPointer = gText_FormCannotBeFound;
			else
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
		case FIELD_MSG_GOT_AWAY_DETECTOR:
			gLoadPointer = gText_PokemonGotAway;
			break;
		case FIELD_MSG_LOST_SIGNAL:
			gLoadPointer = gText_LostSignal;
			break;
		case FIELD_MSG_SNEAK_NEXT_TIME:
			gLoadPointer = gText_GotAwayShouldSneak;
			break;
		case FIELD_MSG_SNEAK_NEXT_TIME_DETECTOR:
			gLoadPointer = gText_PokemonGotAwayShouldSneak;
			break;
	}
}

//This function sucks
//It was written by god knows who and is super hard to modify
static void OutlinedFontDraw(u8 spriteId, u16 tileNum, u16 size)
{
	if (spriteId >= MAX_SPRITES)
		return;

	u8* originalDst, *dst, *toWrite, *strPtr;
	u16 tile = gSprites[spriteId].oam.tileNum + tileNum;
	toWrite = (u8*)((tile * TILE_SIZE) + SPRITE_RAM);
	originalDst = dst = Calloc(size + TILE_SIZE);
	strPtr = gStringVar4;

	u8 counter = 1;
	u16 index = 320;
	u16 prevIndex;

	u8 element = *strPtr;

	while (element != EOS)
	{
		prevIndex = index;
		if (element >= CHAR_a && element <= CHAR_z)
		{
			//Lower case letters
			index = ((element - CHAR_a) * TILE_SIZE) + 1600;
		}
		else if (element >= CHAR_A && element <= CHAR_Z)
		{
			//Upper case letters
			index = ((element - CHAR_A) * TILE_SIZE) + 768;
		}
		else if (element >= CHAR_0 && element <= CHAR_9)
		{
			//Numbers
			index = (element - CHAR_0) * TILE_SIZE;
		}
		else
		{
			//Misc characters
			u8 symbolId = 0;
			switch (element)
			{
				case CHAR_COLON:
				case CHAR_SPACE:
					symbolId = 1;
					break;
				case 0x36: // semi colon used indication of str end
					symbolId = 2;
					break;
				case CHAR_QUESTION_MARK:
					symbolId = 3;
					break;
				case CHAR_HYPHEN:
					symbolId = 4;
					break;
				case CHAR_PERIOD:
					symbolId = 5;
					break;
				case CHAR_SLASH:
					symbolId = 6;
					break;
				case CHAR_DBL_QUOT_LEFT:
					symbolId = 7;
					break;
				case CHAR_DBL_QUOT_RIGHT:
					symbolId = 8;
					break;
				case CHAR_SGL_QUOT_LEFT:
					symbolId = 9;
					break;
				case CHAR_SGL_QUOT_RIGHT:
					symbolId = 10;
					break;
				case CHAR_ELLIPSIS:
					symbolId = 11;
					break;
				case CHAR_COMMA:
					symbolId = 12;
					break;
				case CHAR_MALE:
					symbolId = 13;
					//dst =
					break;
				case CHAR_FEMALE:
					symbolId = 14;
					break;
				case EOS:
					symbolId = 1;
					break;
			}

			index = (symbolId + 9) * TILE_SIZE;
		}

		if (counter == 0 || *(strPtr + 1) == EOS)
		{
			// first or last pcharacters don't need pixel merging
			Memcpy(dst, &gInterfaceGfx_dexnavStarsTiles[index], TILE_SIZE);
		}
		else if (element == CHAR_SPACE)
		{
			Memcpy(dst, &gInterfaceGfx_dexnavStarsTiles[index], TILE_SIZE);
			u8* prevLetter = (u8*)(&gInterfaceGfx_dexnavStarsTiles[prevIndex]);
			*(dst +  0) = *(prevLetter +  2);
			*(dst +  4) = *(prevLetter +  6);
			*(dst +  8) = *(prevLetter + 10);
			*(dst + 12) = *(prevLetter + 14);
			*(dst + 16) = *(prevLetter + 18);
			*(dst + 20) = *(prevLetter + 22);
			*(dst + 24) = *(prevLetter + 26);
			*(dst + 28) = *(prevLetter + 30);
		}
		else if (*(strPtr + 1) != EOS)
		{
			// pcharacter in middle, if blank space fill blank with previous pcharacter's last pixel row IFF previous pchar's last pixel row non-empty
			Memcpy(dst, &gInterfaceGfx_dexnavStarsTiles[index], TILE_SIZE);

			//I'm not sure what the below piece of code does
			//When uncommented, it makes letters that come after "i"'s look bad
			/*u8* prevLetter = (u8*)(&gInterfaceGfx_dexnavStarsTiles[prevIndex]);
			*(dst)      |= (((*(prevLetter +  0) & 0xF) == 0) ? (*(dst +  0) & 0xF) : (*(prevLetter +  0) & 0xF));
			*(dst +  4) |= (((*(prevLetter +  4) & 0xF) == 0) ? (*(dst +  4) & 0xF) : (*(prevLetter +  4) & 0xF));
			*(dst +  8) |= (((*(prevLetter +  8) & 0xF) == 0) ? (*(dst +  8) & 0xF) : (*(prevLetter +  8) & 0xF));
			*(dst + 12) |= (((*(prevLetter + 12) & 0xF) == 0) ? (*(dst + 12) & 0xF) : (*(prevLetter + 12) & 0xF));
			*(dst + 16) |= (((*(prevLetter + 16) & 0xF) == 0) ? (*(dst + 16) & 0xF) : (*(prevLetter + 16) & 0xF));
			*(dst + 20) |= (((*(prevLetter + 20) & 0xF) == 0) ? (*(dst + 20) & 0xF) : (*(prevLetter + 20) & 0xF));
			*(dst + 24) |= (((*(prevLetter + 24) & 0xF) == 0) ? (*(dst + 24) & 0xF) : (*(prevLetter + 24) & 0xF));
			*(dst + 28) |= (((*(prevLetter + 28) & 0xF) == 0) ? (*(dst + 28) & 0xF) : (*(prevLetter + 28) & 0xF));*/
		}

		if (counter == 2 && *(strPtr + 1) != EOS)
		{
			//Every second character, merge
			//8x8px tile made of 4x8px from previous char and 4x8px of this char
			*(dst - 30) = (((*(dst - 30) & 0x0F) == 0) ? (*(dst +  0) & 0xF) : (*(dst - 30) & 0x0F)) | (*(dst +  0) & 0xF0);
			*(dst - 26) = (((*(dst - 26) & 0x0F) == 0) ? (*(dst +  4) & 0xF) : (*(dst - 26) & 0x0F)) | (*(dst +  4) & 0xF0);
			*(dst - 22) = (((*(dst - 22) & 0x0F) == 0) ? (*(dst +  8) & 0xF) : (*(dst - 22) & 0x0F)) | (*(dst +  8) & 0xF0);
			*(dst - 18) = (((*(dst - 18) & 0x0F) == 0) ? (*(dst + 12) & 0xF) : (*(dst - 18) & 0x0F)) | (*(dst + 12) & 0xF0);
			*(dst - 14) = (((*(dst - 14) & 0x0F) == 0) ? (*(dst + 16) & 0xF) : (*(dst - 14) & 0x0F)) | (*(dst + 16) & 0xF0);
			*(dst - 10) = (((*(dst - 10) & 0x0F) == 0) ? (*(dst + 20) & 0xF) : (*(dst - 10) & 0x0F)) | (*(dst + 20) & 0xF0);
			*(dst -  6) = (((*(dst -  6) & 0x0F) == 0) ? (*(dst + 24) & 0xF) : (*(dst -  6) & 0x0F)) | (*(dst + 24) & 0xF0);
			*(dst -  2) = (((*(dst -  2) & 0x0F) == 0) ? (*(dst + 28) & 0xF) : (*(dst -  2) & 0x0F)) | (*(dst + 28) & 0xF0);

			//Last two pixels unconditional
			*(dst - 29) |= *(dst +  1);
			*(dst - 25) |= *(dst +  5);
			*(dst - 21) |= *(dst +  9);
			*(dst - 17) |= *(dst + 13);
			*(dst - 13) |= *(dst + 17);
			*(dst -  9) |= *(dst + 21);
			*(dst -  5) |= *(dst + 25);
			*(dst -  1) |= *(dst + 29);

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
				if (sDexNavHudPtr->heldItem != ITEM_NONE
				&& sDexNavHudPtr->spriteIdItem < MAX_SPRITES)
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

	//Draw directional arrow
	if (sDexNavHudPtr->xProximity == 0 && sDexNavHudPtr->yProximity == 0) //On spot
	{
		FillWindowPixelBuffer(sDexNavHudPtr->arrowWindowId, PIXEL_FILL(1)); //Clean tiles with black
	}
	else
	{
		const u8* arrowString;
		const struct TextColor arrowColour =
		{
			.bgColor = 1, //Black
			.fgColor = 2, //White
			.shadowColor = 3, //Gray
		};

		if (sDexNavHudPtr->xProximity > sDexNavHudPtr->yProximity) //Player is farther horizontally
		{
			if (gSaveBlock1->pos.x + 7 < sDexNavHudPtr->tileX) //Player is on the left of tile
				arrowString = gText_DexNav_RightArrow;
			else //Player is on the right of tile
				arrowString = gText_DexNav_LeftArrow;
		}
		else //Player is closer vertically
		{
			if (gSaveBlock1->pos.y + 7 < sDexNavHudPtr->tileY) //Player is above tile
				arrowString = gText_DexNav_DownArrow;
			else //Player is below tile
				arrowString = gText_DexNav_UpArrow;
		}

		WindowPrint(sDexNavHudPtr->arrowWindowId, 0, 4, 0, &arrowColour, 0, arrowString);
	}
}

extern const u8 SystemScript_StartDexNavBattle[];
static void Task_ManageDexNavHUD(u8 taskId)
{
	//Check for out of range
	if (sDexNavHudPtr->totalProximity > 20
	|| IsMapNamePopupTaskActive())
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
		if (gTasks[taskId].data[7]) //Detector Mode
			DexNavShowFieldMessage(FIELD_MSG_GOT_AWAY_DETECTOR);
		else
			DexNavShowFieldMessage(FIELD_MSG_GOT_AWAY);
		return;
	}

	if (sDexNavHudPtr->totalProximity <= SNEAKING_PROXIMITY && TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_DASH | PLAYER_AVATAR_FLAG_BIKE)) //If player is close and running then the Pokemon should flee
	{
		gCurrentDexNavChain = 0; //A Pokemon running like this resets the chain
		DexNavFreeHUD();
		if (gTasks[taskId].data[7]) //Detector Mode
			DexNavShowFieldMessage(FIELD_MSG_SNEAK_NEXT_TIME_DETECTOR);
		else
			DexNavShowFieldMessage(FIELD_MSG_SNEAK_NEXT_TIME);
		DestroyTask(taskId);
		return;
	}

	//Check if script just executed
	if (ScriptContext2_IsEnabled() == TRUE)
	{
		//gCurrentDexNavChain = 0; //Not fair because of the repel pop up
		DexNavFreeHUD();
		DestroyTask(taskId);
		return;
	}

	if (gMain.newKeys & B_BUTTON)
	{
		PlaySE(SE_POKENAV_OFF);
		gCurrentDexNavChain = 0; //A Pokemon running like this resets the chain
		DexNavFreeHUD();
		DestroyTask(taskId);
		return;
	}

	//Caves and water the pokemon moves around
	#ifdef DEXNAV_POKEMON_MOVE_IN_CAVES_WATER
	if ((sDexNavHudPtr->environment == ENCOUNTER_TYPE_WATER || !IsMapTypeOutdoors(GetCurrentMapType()))
	#ifdef UNBOUND
	&& GetCurrentRegionMapSectionId() != MAPSEC_FLOWER_PARADISE
	#endif
	&& sDexNavHudPtr->totalProximity < 2
	&& sDexNavHudPtr->movementTimes < 2)
	{
		StopDexNavFieldEffect();
		while(!ShakingGrass(sDexNavHudPtr->environment, SCAN_SIZE_X_REPEAT, SCAN_SIZE_Y_REPEAT, TRUE));
		sDexNavHudPtr->movementTimes += 1;
	}
	#endif

	// check for encounter start
	if (sDexNavHudPtr->totalProximity < 1)
	{
		u16 species = sDexNavHudPtr->species;

		DexNavGetMon(sDexNavHudPtr->species, sDexNavHudPtr->potential, sDexNavHudPtr->pokemonLevel,
					sDexNavHudPtr->ability, sDexNavHudPtr->moveId, sDexNavHudPtr->searchLevel, gCurrentDexNavChain);
		DestroyTask(taskId);

		TryRandomizeSpecies(&species);

		//Increment the search level
		u16 dexNum = SpeciesToNationalPokedexNum(species);
		if (gDexNavSearchLevels[dexNum] < 255)
			gDexNavSearchLevels[dexNum] += 1;

		//Freeing only the state, objects and hblank cleared on battle start.
		Free(sDexNavHudPtr);

		gDexNavStartedBattle = TRUE;
		DismissMapNamePopup();
		ScriptContext1_SetupScript(SystemScript_StartDexNavBattle);
	};

	//HUD needs updating iff player has moved
	DexNavProximityUpdate();
	if (gTasks[taskId].data[0] != sDexNavHudPtr->totalProximity)
	{
		DexNavIconsVisionUpdate(sDexNavHudPtr->totalProximity, sDexNavHudPtr->searchLevel);
		gTasks[taskId].data[0] = sDexNavHudPtr->totalProximity;
	}
}


// ===================================== //
// ================ HUD ================ //
// ===================================== //
static const u8 sLandEncounterRates[] = {20, 20, 10, 10, 10, 10, 5, 5, 4, 4, 1, 1};
static const u8 sWaterEncounterRates[] = {60, 30, 5, 4, 1};
static const u8 sFishingEncounterRates[] = {70, 30, 60, 20, 20, 40, 40, 15, 4, 1};
static u8 GetTotalEncounterChance(u16 species, u8 environment)
{
	u32 i;
	const struct WildPokemonInfo* landMonsInfo = LoadProperMonsData(LAND_MONS_HEADER);
	const struct WildPokemonInfo* waterMonsInfo = LoadProperMonsData(WATER_MONS_HEADER);
	const struct WildPokemonInfo* fishingMonsInfo = LoadProperMonsData(FISHING_MONS_HEADER);
	u8 chance = 0;

	switch (environment)
	{
		case ENCOUNTER_TYPE_LAND: ;
			if (landMonsInfo == NULL)
				break; //Hidden pokemon should only appear on walkable tiles or surf tiles

			//Check swarming mon first
			u8 swarmIndex = GetCurrentSwarmIndex();
			if (IsValidSwarmIndex(swarmIndex))
			{
				if (GetCurrentRegionMapSectionId() == gSwarmTable[swarmIndex].mapName
				&& species == gSwarmTable[swarmIndex].species)
				{
					chance += SWARM_CHANCE;
					break;
				}
			}

			for (i = 0; i < MAX_TOTAL_LAND_MONS; ++i)
			{
				const struct WildPokemon* monData = &landMonsInfo->wildPokemon[i];
				if (monData->species == species)
					chance += sLandEncounterRates[i];
			}
			break;

		case ENCOUNTER_TYPE_WATER:
			if (waterMonsInfo != NULL)
			{
				for (i = 0; i < NUM_WATER_MONS; ++i)
				{
					const struct WildPokemon* monData = &waterMonsInfo->wildPokemon[i];
					if (monData->species == species)
						chance += sWaterEncounterRates[i];
				}
			}

			if (fishingMonsInfo != NULL)
			{
				for (i = 0; i < NUM_FISHING_MONS; ++i)
				{
					const struct WildPokemon* monData = &fishingMonsInfo->wildPokemon[i];
					if (monData->species == species)
						chance += sFishingEncounterRates[i];
				}
			}
			break;
	}

	return chance;
}

static u8 GetEncounterLevel(u16 species, u8 environment, bool8 detectorMode)
{
	u32 i;
	const struct WildPokemon* monData;
	const struct WildPokemonInfo* landMonsInfo = LoadProperMonsData(LAND_MONS_HEADER);
	const struct WildPokemonInfo* waterMonsInfo = LoadProperMonsData(WATER_MONS_HEADER);
	const struct WildPokemonInfo* fishingMonsInfo = LoadProperMonsData(FISHING_MONS_HEADER);

	u8 min = 100;
	u8 max = 0;

	switch (environment)
	{
		case ENCOUNTER_TYPE_LAND:
			if (landMonsInfo == NULL)
				return MAX_LEVEL + 1; //Hidden pokemon should only appear on walkable tiles or surf tiles

			for (i = 0; i < MAX_TOTAL_LAND_MONS; ++i)
			{
				monData = &landMonsInfo->wildPokemon[i];
				if (monData->species == species)
				{
					min = (min < monData->minLevel) ? min : monData->minLevel;
					max = (max > monData->maxLevel) ? max : monData->maxLevel;
					break;
				}
			}

			if (i >= MAX_TOTAL_LAND_MONS) //Pokemon not found here
			{
				//Check swarming mon
				u8 swarmIndex = GetCurrentSwarmIndex();
				if (detectorMode)
				{
					goto PICK_RANDOM_LAND_LEVEL;
				}
				else if (IsValidSwarmIndex(swarmIndex))
				{
					if (GetCurrentRegionMapSectionId() == gSwarmTable[swarmIndex].mapName
					&& species == gSwarmTable[swarmIndex].species)
					{
						//Pick index at random and choose min and max from there
						PICK_RANDOM_LAND_LEVEL:
						i = RandRange(0, NELEMS(landMonsInfo->wildPokemon));
						monData = &landMonsInfo->wildPokemon[Random() % MAX_TOTAL_LAND_MONS];
						min = monData->minLevel;
						max = monData->maxLevel;
						break;
					}
				}

				return MAX_LEVEL + 1;
			}
			break;

		case ENCOUNTER_TYPE_WATER:
			if (waterMonsInfo != NULL)
			{
				for (i = 0; i < NUM_WATER_MONS; ++i)
				{
					monData = &waterMonsInfo->wildPokemon[i];
					if (monData->species == species)
					{
						min = (min < monData->minLevel) ? min : monData->minLevel;
						max = (max > monData->maxLevel) ? max : monData->maxLevel;
						break;
					}
				}

				if (i < NUM_WATER_MONS) //Pokemon found here
					break;

				if (detectorMode)
				{
					i = RandRange(0, NELEMS(waterMonsInfo->wildPokemon));
					monData = &waterMonsInfo->wildPokemon[Random() % NUM_WATER_MONS];
					min = monData->minLevel;
					max = monData->maxLevel;
					break;
				}
			}

			if (fishingMonsInfo != NULL)
			{
				for (i = 0; i < NUM_FISHING_MONS; ++i)
				{
					monData = &fishingMonsInfo->wildPokemon[i];
					if (monData->species == species)
					{
						min = (min < monData->minLevel) ? min : monData->minLevel;
						max = (max > monData->maxLevel) ? max : monData->maxLevel;
						break;
					}
				}

				if (i < NUM_FISHING_MONS) //Pokemon found here
					break;

				if (detectorMode)
				{
					i = RandRange(0, NELEMS(fishingMonsInfo->wildPokemon));
					monData = &fishingMonsInfo->wildPokemon[Random() % NUM_FISHING_MONS];
					min = monData->minLevel;
					max = monData->maxLevel;
					break;
				}
			}

			return MAX_LEVEL + 1; //Hidden pokemon should only appear on walkable tiles or surf tiles

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


extern u8 GetCurrentLevelCap(void); //Must be implemented yourself
static u8 DexNavGenerateMonLevel(u16 species, u8 chainLevel, u8 environment, bool8 detectorMode)
{
	u8 levelBase, levelBonus;

	levelBase = GetEncounterLevel(species, environment, detectorMode);
	if (levelBase > MAX_LEVEL)
		return 0;

	levelBonus = chainLevel / 5;

	if (Random() % 100 < 4) //4% chance of having a +10 level
		levelBonus += 10;

	if (levelBase + levelBonus > MAX_LEVEL)
		return MAX_LEVEL;
	#ifdef FLAG_HARD_LEVEL_CAP
	else if (FlagGet(FLAG_HARD_LEVEL_CAP) && levelBase + levelBonus > GetCurrentLevelCap())
		return GetCurrentLevelCap();
	#endif
	else
		return levelBase + levelBonus;
}


static u16 DexNavGenerateHeldItem(u16 species, u8 searchLevel)
{
	return GenerateWildMonHeldItem(species, searchLevel >> 4);
}

static u8 DexNavGenerateHiddenAbility(u16 species, u8 searchLevel)
{
	bool8 genAbility = FALSE;
	u16 randVal = Random() % 100;

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

	#ifdef UNBOUND
	if (genAbility
	&& !FlagGet(FLAG_SYS_GAME_CLEAR)
	&& VarGet(VAR_GAME_DIFFICULTY) >= OPTIONS_EXPERT_DIFFICULTY
	&& gBaseStats[species].hiddenAbility == ABILITY_IMPOSTER)
		genAbility = FALSE; //Don't allow Imposter until the post-game
	#endif

	if (genAbility && gBaseStats[species].hiddenAbility != ABILITY_NONE
	&& GetSetPokedexFlag(SpeciesToNationalPokedexNum(species), FLAG_GET_CAUGHT)) //Only give hidden ability if Pokemon has been caught before
		return GetHiddenAbility(species);
	else
	{
		//Pick a normal ability of that Pokemon
		u8 ability;
		u8 ability1 = GetAbility1(species);
		u8 ability2 = GetAbility2(species);
		
		if (ability2 != ABILITY_NONE)
			ability = (randVal & 1) == 0 ? ability1 : ability2;
		else
			ability = ability1;

		return ability;
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
	u32 i;
	u16 randVal = Random() % 100;
	bool8 genMove = FALSE;

	//Evaluate if Pokemon should get an egg move in first slot
	if (searchLevel < 5)
	{
		#if (SEARCHLEVEL0_MOVECHANCE != 0)
		genMove = randVal < SEARCHLEVEL0_MOVECHANCE;
		#endif
	}
	else if (searchLevel < 10)
	{
		#if (SEARCHLEVEL5_MOVECHANCE != 0)
		genMove = randVal < SEARCHLEVEL5_MOVECHANCE;
		#endif
	}
	else if (searchLevel < 25)
	{
		#if (SEARCHLEVEL10_MOVECHANCE != 0)
		genMove = randVal < SEARCHLEVEL10_MOVECHANCE;
		#endif
	}
	else if (searchLevel < 50)
	{
		#if (SEARCHLEVEL25_MOVECHANCE != 0)
		genMove = randVal < SEARCHLEVEL25_MOVECHANCE;
		#endif
	}
	else if (searchLevel < 100)
	{
		#if (SEARCHLEVEL50_MOVECHANCE != 0)
		genMove = randVal < SEARCHLEVEL50_MOVECHANCE;
		#endif
	}
	else
	{
		#if (SEARCHLEVEL100_MOVECHANCE != 0)
		genMove =randVal < SEARCHLEVEL100_MOVECHANCE;
		#endif
	}

	//Generate a wild mon and copy moveset
	CreateWildMon(species, encounterLevel, FindHeaderIndexWithLetter(species, sDexNavHudPtr->unownLetter - 1), TRUE);

	//Set first move slot to a random Egg Move if search level is good enough
	if (genMove == TRUE)
	{
		u16 eggMoveBuffer[EGG_MOVES_ARRAY_COUNT];
		u8 numEggMoves = GetAllEggMoves(&gEnemyParty[0], eggMoveBuffer, TRUE);

		if (numEggMoves > 0)
		{
			u8 index = RandRange(0, numEggMoves);
			SetMonData(&gEnemyParty[0], MON_DATA_MOVE1, &eggMoveBuffer[index]);
		}
	}

	//Store generated mon moves into Dex Nav Struct
	for (i = 0; i < MAX_MON_MOVES; ++i)
		moveLoc[i] = GetMonData(&gEnemyParty[0], MON_DATA_MOVE1 + i, NULL);
}


static void DexNavDrawBlackBar(u8* windowId)
{
	struct WindowTemplate template;
	static const u16 blackBarPal[] = {RGB(7, 25, 13), RGB(4, 4, 4),
									  RGB(31, 31, 31), RGB(12, 12, 12)}; //For arrow

	LoadPalette(blackBarPal, 0xD0, 0x8);
	PreservePaletteInWeather(13);
	template = SetWindowTemplateFields(0, 0, 16, 30, 4, 13, GetStdWindowBaseTileNum());
	*windowId = AddWindow(&template);
	PutWindowTilemap(*windowId);
	FillWindowPixelBuffer(*windowId, PIXEL_FILL(0)); //Clean tiles
	FillWindowPixelRect(*windowId, PIXEL_FILL(1), 0, 4, 240, 28); //Black bar
	CopyWindowToVram(*windowId, COPYWIN_BOTH);
}

static void DexNavDrawDirectionalArrow(u8* windowId)
{
	struct WindowTemplate template;

	template = SetWindowTemplateFields(0, 24, 18, 2, 2, 13, GetStdWindowBaseTileNum() - 4);
	*windowId = AddWindow(&template);
	PutWindowTilemap(*windowId);
	FillWindowPixelBuffer(*windowId, PIXEL_FILL(1)); //Clean tiles with black
	CopyWindowToVram(*windowId, COPYWIN_BOTH);
}


static void DexNavDrawChainNumber(u8* spriteIdNumAddr, u8* spriteIdStarAddr)
{
	u8 spriteIdNum = MAX_SPRITES;
	u8 spriteIdStar = MAX_SPRITES;

	//Allocate both the lit and unlit star to VRAM
	LoadSpriteSheet(&sStarLitSpriteSheet);
	LoadSpriteSheet(&sStarDullSpriteSheet);
	LoadSpritePalette(&gHeldItemSpritePalette);

	if (gCurrentDexNavChain > 0) //Started a chain
	{
		LoadCompressedSpriteSheetUsingHeap(&sChainNumberCanvasSpriteSheet);
		LoadSpritePalette(&gHeldItemSpritePalette);
		spriteIdNum = CreateSprite(&sChainNumberCanvasTemplate, ICONX + 80, ICONY + 0x12, 0x0);
		if (spriteIdNum < MAX_SPRITES)
		{
			//Get Text
			StringCopy(gStringVar4, gText_DexNavHUDChainNumber);
			ConvertIntToDecimalStringN(gStringVar1, gCurrentDexNavChain, 0, 3);
			StringAppend(gStringVar4, gStringVar1);

			//Adjust Position
			gSprites[spriteIdNum].pos1.x = 205 + (32 / 2);
			gSprites[spriteIdNum].pos1.y = 137 + (8 / 2);

			if (gCurrentDexNavChain < 10)
				gSprites[spriteIdNum].pos1.x += 8;
			else if (gCurrentDexNavChain < 100)
				gSprites[spriteIdNum].pos1.x += 4;

			//Format string so it's even length or if it's odd ends in two spaces
			u8 len = StringLength(gStringVar4);
			if (!(len % 2))
			{
				gStringVar4[len] = 0x0;
				gStringVar4[len + 1] = 0x0;
				gStringVar4[len + 2] = 0xFF;
			}

			//Draw the chain number on a blank sprite
			OutlinedFontDraw(spriteIdNum, 0, 16 * 8);

			//Try create a star if there's a higher chance of a shiny being found
			if (gCurrentDexNavChain == 50 || gCurrentDexNavChain == 100)
			{
				s16 xPos = ICONX + 188;
				if (gCurrentDexNavChain == 100)
					xPos -= 4;
				spriteIdStar = CreateSprite(&sStarLitTemplate, xPos, ICONY - 6, 0x0);
			}
		}
	}

	*spriteIdNumAddr = spriteIdNum;
	*spriteIdStarAddr = spriteIdStar;
}

static void DexNavDrawSight(u8 sightLevel, u8* spriteIdAddr)
{
	LoadCompressedSpriteSheetUsingHeap(&sSightSpriteSheet);
	LoadSpritePalette(&gHeldItemSpritePalette);
	u8 spriteId = CreateSprite(&sSightTemplate, 175 + (16 / 2), 148 + (8 / 2), 0);
	*spriteIdAddr = spriteId; //Must go before DexNavSightUpdate!
	if (spriteId < MAX_SPRITES)
		DexNavSightUpdate(sightLevel);
};

static void DexNavDrawBButton(u8* spriteIdAddr)
{
	LoadCompressedSpriteSheetUsingHeap(&sBButtonSpriteSheet);
	LoadSpritePalette(&gHeldItemSpritePalette);
	u8 spriteId = CreateSprite(&sBButtonTemplate, 208 + (32 / 2), 148 + (8 / 2), 0);
	*spriteIdAddr = spriteId;
};

static void DexNavDrawAbility(u8 ability, u16 species, u8* spriteIdAddr)
{
	LoadCompressedSpriteSheetUsingHeap(&sAbilityCanvasSpriteSheet);
	LoadSpritePalette(&gHeldItemSpritePalette);
	u8 spriteId = CreateSprite(&sAbilityCanvasTemplate, ICONX + 80, ICONY + 0x12, 0x0);
	if (spriteId < MAX_SPRITES)
	{
		//Ability name beside move name
		u8 len = sDexNavHudPtr->moveNameLength;
		gSprites[spriteId].pos1.x += ((8 * (len/2)) + (4 * (len % 2)));

		//Copy ability string from table using state id
		TryRandomizeSpecies(&species);
		CopyAbilityName(gStringVar4, ability, species);

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
	LoadSpritePalette(&gHeldItemSpritePalette);
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
	//Create star objects and space them according to potential 0 - 3 (sprite sheets and palettes loaded in DexNavDrawChainNumber)
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
	if (spriteId < MAX_SPRITES)
		gSprites[spriteId].oam.priority = 0;
}

void DexNavDrawHeldItem(u8* spriteIdAddr)
{
	//Create SPRITE for held item icon
	LoadSpriteSheet(&gHeldItemSpriteSheet);
	LoadSpritePalette(&gHeldItemSpritePalette);
	u8 spriteId = CreateSprite(&gHeldItemTemplate, ICONX + 0x8, ICONY + 0xC, 0x0);
	*spriteIdAddr = spriteId;
}

static void DexNavDrawIcons(bool8 detectorMode)
{
	u8 searchLevel = sDexNavHudPtr->searchLevel;
	DexNavDrawBlackBar(&sDexNavHudPtr->blackBarWindowId);
	DexNavDrawDirectionalArrow(&sDexNavHudPtr->arrowWindowId);
	DexNavDrawChainNumber(&sDexNavHudPtr->spriteIdChainNumber, &sDexNavHudPtr->spriteIdChainStar);
	DexNavDrawSight(sDexNavHudPtr->totalProximity, &sDexNavHudPtr->spriteIdSight);
	DexNavDrawBButton(&sDexNavHudPtr->spriteIdBButton);
	
	if (!detectorMode)
	{
		DexNavDrawMove(sDexNavHudPtr->moveId[0], searchLevel, &sDexNavHudPtr->spriteIdMove);
		DexNavDrawHeldItem(&sDexNavHudPtr->spriteIdItem);
		DexNavDrawAbility(sDexNavHudPtr->ability, sDexNavHudPtr->species, &sDexNavHudPtr->spriteIdAbility);
		DexNavDrawPotential(sDexNavHudPtr->potential, &sDexNavHudPtr->spriteIdPotential[0]);
		DexNavHudDrawSpeciesIcon(sDexNavHudPtr->species, &sDexNavHudPtr->spriteIdSpecies);
	}
	else
	{
		DexNavHudDrawSpeciesIcon(SPECIES_NONE, &sDexNavHudPtr->spriteIdSpecies); //Show ? instead of species
		sDexNavHudPtr->spriteIdMove = MAX_SPRITES;
		sDexNavHudPtr->spriteIdItem = MAX_SPRITES;
		sDexNavHudPtr->spriteIdAbility = MAX_SPRITES;
		Memset(sDexNavHudPtr->spriteIdPotential, MAX_SPRITES, sizeof(sDexNavHudPtr->spriteIdPotential));
	}
}

extern void sp0AF_DismountBicyle(void);
bool8 InitDexNavHUD(u16 species, u8 environment, bool8 detectorMode)
{
	if (Overworld_GetFlashLevel() > 0)
	{
		DexNavShowFieldMessage(FIELD_MSG_TOO_DARK);
		return FALSE;
	}

	sDexNavHudPtr = Calloc(sizeof(struct DexnavHudData));
	// assign non-objects to struct
	sDexNavHudPtr->species = species;
	TryRandomizeSpecies(&species);
	//Species now refers to the potentially randomized species,
	//while sDexNavHudPtr->species refers to the original species

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
	u16 dexNum = SpeciesToNationalPokedexNum(species);
	u8 searchLevel = gDexNavSearchLevels[dexNum];
	sDexNavHudPtr->searchLevel = searchLevel;
	sDexNavHudPtr->pokemonLevel = DexNavGenerateMonLevel(sDexNavHudPtr->species, gCurrentDexNavChain, environment, detectorMode);

	if (sDexNavHudPtr->pokemonLevel < 1)
	{
		Free(sDexNavHudPtr);
		DexNavShowFieldMessage(FIELD_MSG_NOT_IN_AREA);
		return FALSE;
	}

	u8 totalEncounterChance = GetTotalEncounterChance(sDexNavHudPtr->species, environment);
	u8 randVal = (GetSetPokedexFlag(dexNum, FLAG_GET_CAUGHT)) ? 0 : Random() % 100; //Pokemon already caught are easy to catch again
	sDexNavHudPtr->elevation = gEventObjects[gPlayerAvatar->eventObjectId].currentElevation; //Constant elevation for all tiles (helps prevent crashes in caves)
	//*((u8*) 0x2023D70) = randVal; //For debugging
	if ((!detectorMode && randVal >= totalEncounterChance * 2) //Harder Pokemon to find in the area are half as hard to find with the DexNav
	|| (!detectorMode && gDexNavCooldown)
	|| (!detectorMode && VarGet(VAR_REPEL_STEP_COUNT) == 1) //1 step remaining on the repel - player takes a step, repel wears off and they can search again
	|| !ShakingGrass(environment, SCAN_SIZE_X_START, SCAN_SIZE_Y_START, detectorMode)) //Draw shaking tile
	{
		Free(sDexNavHudPtr);
		gDexNavCooldown = TRUE; //A Pokemon can't be found until the player takes at least one step or searches for another Pokemon manually
		DexNavShowFieldMessage(FIELD_MSG_LOOK_IN_OTHER_SPOT);
		return FALSE;
	}

	//Populate sDexNavHudPtr objects
	DexNavGenerateMoveset(sDexNavHudPtr->species, searchLevel, sDexNavHudPtr->pokemonLevel, &sDexNavHudPtr->moveId[0]);
	sDexNavHudPtr->heldItem = DexNavGenerateHeldItem(species, searchLevel);
	sDexNavHudPtr->ability = DexNavGenerateHiddenAbility(species, searchLevel);
	sDexNavHudPtr->potential = DexNavGeneratePotential(searchLevel);
	DexNavProximityUpdate();

	//Draw icons
	DexNavDrawIcons(detectorMode);

	//Hide icons based on proximity and search level
	DexNavIconsVisionUpdate(sDexNavHudPtr->totalProximity, searchLevel);

	//Get off bike so the Pokemon doesn't run away
	sp0AF_DismountBicyle();

	// task update HUD
	u8 taskId = CreateTask(Task_ManageDexNavHUD, 1);
	if (taskId != 0xFF)
	{
		gTasks[taskId].data[0] = gSprites[gPlayerAvatar->spriteId].pos1.x;
		gTasks[taskId].data[7] = detectorMode;
	}

	IncrementGameStat(GAME_STAT_DEXNAV_SCANNED);
	return TRUE; //HUD has been started
}


// This is called via a c1 from the GUI, while waiting to return to the OW
static void ExecDexNavHUD(void)
{
	if (!gPaletteFade->active && !ScriptContext2_IsEnabled() && gMain.callback2 == CB2_Overworld)
	{
		SetMainCallback1(CB1_Overworld);
		InitDexNavHUD(Var8008, Var8009, FALSE);
	}
}

bool8 IsDexNavHudActive(void)
{
	return FuncIsActiveTask(Task_ManageDexNavHUD);
}

// ========================================== //
// ================== GUI =================== //
// ========================================== //

//General Util//

static bool8 AnyPokemonInCurrentArea(void)
{
	return GetTotalLandMonCount() != 0 || GetTotalWaterMonCount() != 0;
}

static u8 GetTotalLandMonCount(void)
{
	return sDexNavGUIPtr->numLandMons + sDexNavGUIPtr->numHiddenLandMons;
}

static u8 GetTotalWaterMonCount(void)
{
	return sDexNavGUIPtr->numWaterMons + sDexNavGUIPtr->numHiddenWaterMons;
}

static u8 GetTotalWaterMonSlots(void)
{
	return GetWaterRowCount() * WATER_ROW_LENGTH;
}

static u8 GetWaterRowCount(void)
{
	u8 count = (GetTotalWaterMonCount() / WATER_ROW_LENGTH);

	if (GetTotalWaterMonCount() % WATER_ROW_LENGTH != 0) //Not even cut
		count += 1;

	return max(count, MIN_WATER_ROW_COUNT);
}

static bool8 IsFishingEncounterMethod(u8 method)
{
	switch (method)
	{
		case ENCOUNTER_METHOD_OLD_ROD:
		case ENCOUNTER_METHOD_GOOD_ROD:
		case ENCOUNTER_METHOD_SUPER_ROD:
		case ENCOUNTER_METHOD_SURF_OLD_ROD:
		case ENCOUNTER_METHOD_SURF_GOOD_ROD:
		case ENCOUNTER_METHOD_SURF_SUPER_ROD:
			return TRUE;
	}

	return FALSE;
}

//Checks if all Pokemon that can be encountered in the grass/ground have been captured
static bool8 CapturedAllLandBasedPokemon(void)
{
	u16 i, species;
	u8 num = 0;
	const struct WildPokemonInfo* landMonsInfo = LoadProperMonsData(LAND_MONS_HEADER);

	#ifdef UNBOUND
	if (GetCurrentRegionMapSectionId() == MAPSEC_DISTORTION_WORLD)
		landMonsInfo = NULL; //Wipe data so Giratina doesn't show up in DexNav
	#endif

	if (landMonsInfo != NULL)
	{
		for (i = 0; i < MAX_TOTAL_LAND_MONS; ++i)
		{
			species = landMonsInfo->wildPokemon[i].species;
			if (species != SPECIES_NONE)
			{
				++num;
				if (!GetSetPokedexFlag(SpeciesToNationalPokedexNum(species), FLAG_GET_CAUGHT))
					break;
			}
		}

		//Check swarming mon
		u8 swarmIndex = GetCurrentSwarmIndex();
		if (IsValidSwarmIndex(swarmIndex)
		&& GetCurrentRegionMapSectionId() == gSwarmTable[swarmIndex].mapName)
		{
			if (!GetSetPokedexFlag(SpeciesToNationalPokedexNum(gSwarmTable[swarmIndex].species), FLAG_GET_CAUGHT))
				return FALSE;
		}

		if (i >= MAX_TOTAL_LAND_MONS && num > 0) //All land mons caught and there were land mons to catch
			return TRUE;
	}

	return FALSE;
}

//Checks if all Pokemon that can be encountered while surfing/fishing have been captured
static bool8 CapturedAllWaterBasedPokemon(void)
{
	u16 i, species;
	const struct WildPokemonInfo* waterMonsInfo = LoadProperMonsData(WATER_MONS_HEADER);
	const struct WildPokemonInfo* fishingMonsInfo = LoadProperMonsData(FISHING_MONS_HEADER);

	#ifdef UNBOUND
	if (GetCurrentRegionMapSectionId() == MAPSEC_DISTORTION_WORLD)
		waterMonsInfo = NULL; //Wipe data so Giratina doesn't show up in DexNav
	#endif

	if (waterMonsInfo == NULL && fishingMonsInfo == NULL)
		return FALSE; //Can't catch all Pokemon when there are none

	if (waterMonsInfo != NULL)
	{
		for (i = 0; i < NUM_WATER_MONS; ++i)
		{
			species = waterMonsInfo->wildPokemon[i].species;
			if (species != SPECIES_NONE)
			{
				if (!GetSetPokedexFlag(SpeciesToNationalPokedexNum(species), FLAG_GET_CAUGHT))
					return FALSE; //Didn't catch at least one
			}
		}
	}

	if (fishingMonsInfo != NULL)
	{
		for (i = 0; i < NUM_FISHING_MONS; ++i)
		{
			species = fishingMonsInfo->wildPokemon[i].species;
			if (species != SPECIES_NONE)
			{
				if (!GetSetPokedexFlag(SpeciesToNationalPokedexNum(species), FLAG_GET_CAUGHT))
					return FALSE; //Didn't catch at least one
			}
		}
	}

	return TRUE;
}

static bool8 TryAddSpeciesToArray(u16 species, u8 encounterMethod, u8 indexCount, u8 unownLetter)
{
	u32 i;
	TryRandomizeSpecies(&species);
	u16 dexNum = SpeciesToNationalPokedexNum(species);

	//Disallow species not seen
	if (!GetSetPokedexFlag(dexNum, FLAG_GET_SEEN))
	{
		for (i = 0; i < max(MAX_TOTAL_LAND_MONS, MAX_TOTAL_WATER_MONS); ++i)
		{
			#ifdef SPECIES_UNOWN
			if (species == SPECIES_UNOWN && InTanobyRuins())
			{
				if (sDexNavGUIPtr->unownForms[i] == unownLetter) //Already in array
					return FALSE;
			}
			else
			#endif
			if (sDexNavGUIPtr->hiddenSpecies[i] == HIDDEN_SPEECIES_TERMIN)
			{
				sDexNavGUIPtr->hiddenSpecies[i] = dexNum;
				break;
			}
			else if (sDexNavGUIPtr->hiddenSpecies[i] == dexNum) //Already in array
				return FALSE;
		}

		if (indexCount == MAX_TOTAL_LAND_MONS)
		{
			sDexNavGUIPtr->numHiddenLandMons++; //Increase how many question marks to print

			if (i < NELEMS(sDexNavGUIPtr->hiddenLandEncounterMethod))
				sDexNavGUIPtr->hiddenLandEncounterMethod[i] = encounterMethod;
		}
		else
		{
			sDexNavGUIPtr->numHiddenWaterMons++;

			if (i < NELEMS(sDexNavGUIPtr->hiddenWaterEncounterMethod))
				sDexNavGUIPtr->hiddenWaterEncounterMethod[i] = encounterMethod;
		}

		return FALSE;
	}

	for (i = 0; i < indexCount; ++i)
	{
		if (indexCount == MAX_TOTAL_LAND_MONS)
		{
			#ifdef SPECIES_UNOWN
			if (species == SPECIES_UNOWN && InTanobyRuins()) //This Unown code is copied from above b/c either
			{												 //all Unown are seen, or none at all
				if (sDexNavGUIPtr->unownForms[i] == unownLetter) //Already in array
					return FALSE;
			}
			else
			#endif
			{
				u16 wildSpecies = sDexNavGUIPtr->grassSpecies[i];
				TryRandomizeSpecies(&wildSpecies);
				if (SpeciesToNationalPokedexNum(wildSpecies) == dexNum)
					return FALSE;
			}
		}
		else
		{
			u16 wildSpecies = sDexNavGUIPtr->waterSpecies[i];
			TryRandomizeSpecies(&wildSpecies);
			if (SpeciesToNationalPokedexNum(wildSpecies) == dexNum)
				return FALSE;
		}
	}

	return TRUE;
}

static void DexNavPopulateEncounterList(void)
{
	//Populate unique wild grass encounters
	u8 grassIndex = 0;
	u8 waterIndex = 0;
	u16 species, i, j;

	const struct WildPokemonInfo* landMonsInfo = LoadProperMonsData(LAND_MONS_HEADER);
	const struct WildPokemonInfo* waterMonsInfo = LoadProperMonsData(WATER_MONS_HEADER);
	const struct WildPokemonInfo* fishingMonsInfo = LoadProperMonsData(FISHING_MONS_HEADER);
	
	#ifdef UNBOUND
	if (GetCurrentRegionMapSectionId() == MAPSEC_DISTORTION_WORLD)
	{
		//Wipe data so Giratina doesn't show up in DexNav
		landMonsInfo = NULL;
		waterMonsInfo = NULL;
	}
	#endif

	Memset(sDexNavGUIPtr->hiddenSpecies, HIDDEN_SPEECIES_TERMIN & 0xFF, sizeof(sDexNavGUIPtr->hiddenSpecies)); //Used in TryAddSpeciesToArray

	if (landMonsInfo != NULL)
	{
		for (i = 0; i < MAX_TOTAL_LAND_MONS; ++i)
		{
			species = landMonsInfo->wildPokemon[i].species;
			if (species != SPECIES_NONE && TryAddSpeciesToArray(species, ENCOUNTER_METHOD_GRASS, MAX_TOTAL_LAND_MONS, PickUnownLetter(species, i)))
			{
				if (InTanobyRuins())
				{
					sDexNavGUIPtr->unownForms[i] = PickUnownLetter(species, i);
					sDexNavGUIPtr->unownFormsByDNavIndices[grassIndex] = PickUnownLetter(species, i);
				}

				sDexNavGUIPtr->grassSpecies[grassIndex++] = landMonsInfo->wildPokemon[i].species;
			}
		}

		//Add swarming mon
		u8 swarmIndex = GetCurrentSwarmIndex();
		if (IsValidSwarmIndex(swarmIndex))
		{
			u16 swarmSpecies = gSwarmTable[swarmIndex].species;
			if (GetCurrentRegionMapSectionId() == gSwarmTable[swarmIndex].mapName
			&& grassIndex < NELEMS(sDexNavGUIPtr->grassSpecies)
			&& TryAddSpeciesToArray(swarmSpecies, ENCOUNTER_METHOD_SWARM, MAX_TOTAL_LAND_MONS, PickUnownLetter(swarmSpecies, 0)))
			{
				sDexNavGUIPtr->landEncounterMethod[grassIndex] = ENCOUNTER_METHOD_SWARM;
				sDexNavGUIPtr->grassSpecies[grassIndex++] = swarmSpecies;
			}
		}
	}

	Memset(sDexNavGUIPtr->hiddenSpecies, HIDDEN_SPEECIES_TERMIN & 0xFF, sizeof(sDexNavGUIPtr->hiddenSpecies)); //Used in TryAddSpeciesToArray

	if (waterMonsInfo != NULL)
	{
		for (i = 0; i < NUM_WATER_MONS; ++i)
		{
			species = waterMonsInfo->wildPokemon[i].species;
			if (species != SPECIES_NONE && TryAddSpeciesToArray(species, ENCOUNTER_METHOD_WATER, MAX_TOTAL_WATER_MONS, PickUnownLetter(species, i)))
			{
				sDexNavGUIPtr->waterEncounterMethod[waterIndex] = ENCOUNTER_METHOD_WATER;
				sDexNavGUIPtr->waterSpecies[waterIndex++] = waterMonsInfo->wildPokemon[i].species;
			}
		}
	}

	if (fishingMonsInfo != NULL)
	{
		for (i = 0; i < NUM_OLD_ROD_MONS; ++i)
		{
			species = fishingMonsInfo->wildPokemon[i].species;
			if (species != SPECIES_NONE)
			{
				if (TryAddSpeciesToArray(species, ENCOUNTER_METHOD_OLD_ROD, MAX_TOTAL_WATER_MONS, PickUnownLetter(species, i)))
				{
					sDexNavGUIPtr->waterEncounterMethod[waterIndex] = ENCOUNTER_METHOD_OLD_ROD;
					sDexNavGUIPtr->waterItemRequired[waterIndex] = ITEM_OLD_ROD; //Can only be searched if player has an Old Rod
					sDexNavGUIPtr->waterSpecies[waterIndex++] = fishingMonsInfo->wildPokemon[i].species;
				}
				else
				{
					for (j = 0; j < waterIndex; ++j)
					{
						if (sDexNavGUIPtr->waterSpecies[j] == fishingMonsInfo->wildPokemon[i].species
						&& sDexNavGUIPtr->waterEncounterMethod[j] == ENCOUNTER_METHOD_WATER)
						{
							sDexNavGUIPtr->waterEncounterMethod[j] = ENCOUNTER_METHOD_SURF_OLD_ROD; //Can be found both ways
							break;
						}
					}
				}
			}
		}

		for (i = NUM_OLD_ROD_MONS; i < NUM_OLD_ROD_MONS + NUM_GOOD_ROD_MONS; ++i)
		{
			species = fishingMonsInfo->wildPokemon[i].species;
			if (species != SPECIES_NONE)
			{
				if (TryAddSpeciesToArray(species, ENCOUNTER_METHOD_GOOD_ROD, MAX_TOTAL_WATER_MONS, PickUnownLetter(species, i)))
				{
					sDexNavGUIPtr->waterEncounterMethod[waterIndex] = ENCOUNTER_METHOD_GOOD_ROD;
					sDexNavGUIPtr->waterItemRequired[waterIndex] = ITEM_GOOD_ROD; //Can only be searched if player has a Good Rod
					sDexNavGUIPtr->waterSpecies[waterIndex++] = fishingMonsInfo->wildPokemon[i].species;
				}
				else
				{
					for (j = 0; j < waterIndex; ++j)
					{
						if (sDexNavGUIPtr->waterSpecies[j] == fishingMonsInfo->wildPokemon[i].species
						&& sDexNavGUIPtr->waterEncounterMethod[j] == ENCOUNTER_METHOD_WATER)
						{
							sDexNavGUIPtr->waterEncounterMethod[j] = ENCOUNTER_METHOD_SURF_GOOD_ROD; //Can be found both ways
							break;
						}
					}
				}
			}
		}

		for (i = NUM_OLD_ROD_MONS + NUM_GOOD_ROD_MONS; i < NUM_FISHING_MONS; ++i)
		{
			species = fishingMonsInfo->wildPokemon[i].species;
			if (species != SPECIES_NONE)
			{
				if (TryAddSpeciesToArray(species, ENCOUNTER_METHOD_SUPER_ROD, MAX_TOTAL_WATER_MONS, PickUnownLetter(species, i)))
				{
					sDexNavGUIPtr->waterEncounterMethod[waterIndex] = ENCOUNTER_METHOD_SUPER_ROD;
					sDexNavGUIPtr->waterItemRequired[waterIndex] = ITEM_SUPER_ROD; //Can only be searched if player has an Super Rod
					sDexNavGUIPtr->waterSpecies[waterIndex++] = fishingMonsInfo->wildPokemon[i].species;
				}
				else
				{
					for (j = 0; j < waterIndex; ++j)
					{
						if (sDexNavGUIPtr->waterSpecies[j] == fishingMonsInfo->wildPokemon[i].species
						&& sDexNavGUIPtr->waterEncounterMethod[j] == ENCOUNTER_METHOD_WATER)
						{
							sDexNavGUIPtr->waterEncounterMethod[j] = ENCOUNTER_METHOD_SURF_SUPER_ROD; //Can be found both ways
							break;
						}
					}
				}
			}
		}
	}

	#ifdef NATIONAL_DEX_UNOWN
	if (InTanobyRuins() && !GetSetPokedexFlag(NATIONAL_DEX_UNOWN, FLAG_GET_SEEN))
	{ //This is so the right amount of ? appear for Unown in the different chambers
		u16 unowns[MAX_TOTAL_LAND_MONS + 1];
		unowns[0] = HIDDEN_SPEECIES_TERMIN;

		sDexNavGUIPtr->numHiddenLandMons = 0;
		for (int i = 0; i < MAX_TOTAL_LAND_MONS; ++i)
		{
			u8 letter = PickUnownLetter(SPECIES_UNOWN, i);
			if (!CheckTableForSpecies(letter, unowns)) //Table with Unown letters treated like a species table
			{
				unowns[sDexNavGUIPtr->numHiddenLandMons++] = letter;
				unowns[sDexNavGUIPtr->numHiddenLandMons] = HIDDEN_SPEECIES_TERMIN; //Shift end down 1
			}
		}
	}
	#endif

	sDexNavGUIPtr->numLandMons = grassIndex;
	sDexNavGUIPtr->numWaterMons = waterIndex;

	if (GetTotalLandMonCount() == 0 && GetTotalWaterMonCount() != 0)
		sDexNavGUIPtr->selectedArea = AREA_WATER; //Start with water selected because no searchable land mons anyway
}

static void RegisterSpecies(u16 species, u8 taskId)
{
	//Species was valid
	DexNavDisplaySpeciesData();

	//Create value to store in a var
	u16 varStore = (sDexNavGUIPtr->selectedArea << 15) | species;
	VarSet(VAR_DEXNAV, varStore);

	//Update R-Button mode if applicable
	#ifdef VAR_R_BUTTON_MODE
	VarSet(VAR_R_BUTTON_MODE, OPTIONS_R_BUTTON_MODE_DEXNAV);
	#endif
	
	//Update registered icon details
	sDexNavGUIPtr->registeredIndex = sDexNavGUIPtr->selectedIndex;
	sDexNavGUIPtr->registeredArea = sDexNavGUIPtr->selectedArea;
	sDexNavGUIPtr->registeredIconVisible = TRUE;

	TryRandomizeSpecies(&species);
	CopySpeciesNameToStringVar1_AdjustUnownName(species);
	PrintDexNavMessage(MESSAGE_REGISTERED);
	PlaySE(SE_POKENAV_SEARCHING);
	gTasks[taskId].func = Task_WaitForErrorMessage;
}

static bool8 CanWaterMonBeSearched(void)
{
	if (sDexNavGUIPtr->selectedArea == AREA_WATER
	&& sDexNavGUIPtr->waterItemRequired[GetWaterSlotSelected()] != ITEM_NONE
	&& !CheckBagHasItem(sDexNavGUIPtr->waterItemRequired[GetWaterSlotSelected()], 1)) //Don't have Rod required to find this mon normally
	{
		u16 species = GetSpeciesAtCursorPos();
		TryRandomizeSpecies(&species);
		CopySpeciesNameToStringVar1_AdjustUnownName(species);
		StringCopy(gStringVar2, ItemId_GetName(sDexNavGUIPtr->waterItemRequired[GetWaterSlotSelected()]));
		return FALSE;
	}

	return TRUE;
}

static void CopySpeciesNameToStringVar1_AdjustUnownName(u16 species)
{
	StringCopy(gStringVar1, gSpeciesNames[species]);

	#ifdef SPECIES_UNOWN
	if (InTanobyRuins() && species == SPECIES_UNOWN)
	{
		const u8 aSuffix[] = {CHAR_SPACE, CHAR_A, EOS};
		StringAppend(gStringVar1, aSuffix);
	}
	#endif
}

//GUI Util//

static void CleanWindow(u8 windowId)
{
	FillWindowPixelBuffer(windowId, PIXEL_FILL(0));
}

static void CleanWindows(void)
{
	for (u32 i = 0; i < WINDOW_COUNT; ++i)
		CleanWindow(i);
}

static void CommitWindow(u8 windowId)
{
	CopyWindowToVram(windowId, COPYWIN_BOTH);
	PutWindowTilemap(windowId);
}

static void CommitWindows(void)
{
	for (u32 i = 0; i < WINDOW_COUNT; ++i)
		CommitWindow(i);
}

static void PrintDexNavMessage(u8 messageId)
{
	const u8* text;
	u8 textBoxBgColour = 1;

	//Create textbox
	TextWindow_SetUserSelectedFrame(WIN_TEXTBOX, 500, 0xE0); //The StdWindowBaseTileNum is not in use
	DrawStdFrameWithCustomTileAndPalette(WIN_TEXTBOX, FALSE, 500, 14);
	FillWindowPixelBuffer(WIN_TEXTBOX, PIXEL_FILL(textBoxBgColour));
	CommitWindow(WIN_TEXTBOX);

	//Determine string to print
	switch (messageId)
	{
		case MESSAGE_POKEMON_SELECTED:
			text = gText_DexNav_PokemonSelected;
			break;
		case MESSAGE_REGISTERED:
			text = gText_DexNav_Locked;
			break;
		case MESSAGE_NEED_ROD:
			text = gText_DexNav_NeedRod;
			break;
		case MESSAGE_INVALID:
			text = gText_DexNav_Invalid;
			break;
		case MESSAGE_NO_DATA:
			text = gText_DexNav_NoDataForSlot;
			break;
		case MESSAGE_TOO_DARK:
			text = gText_DexNav_TooDark;
			break;
		case MESSAGE_NO_POKEMON_HERE:
			text = gText_DexNav_NoPokemonHere;
			break;
		default:
			text = NULL;
			break;
	}

	//Add text
	StringExpandPlaceholders(gStringVar4, text);
	WindowPrint(WIN_TEXTBOX, 1, 4, 0, &sBlackText, GetPlayerTextSpeedDelay(), gStringVar4);
	RunTextPrinters();

	if (sDexNavGUIPtr->cursorSpriteId < MAX_SPRITES)
		StartSpriteAnim(&gSprites[sDexNavGUIPtr->cursorSpriteId], 1); //Pointing and unmoving

	ShowBg(BG_TEXTBOX);
}

static void PrintDexNavError(u8 taskId, u8 specificMsgId)
{
	bool8 error;

	if (specificMsgId != 0xFF)
	{
		error = TRUE;
		PrintDexNavMessage(specificMsgId);
	}
	else
	{
		if (sDexNavGUIPtr->selectedArea == AREA_WATER
		&&  GetWaterSlotSelected() >= GetTotalWaterMonCount())
		{
			PrintDexNavMessage(MESSAGE_NO_DATA); //No data in slot
			error = TRUE;
		}
		else if (sDexNavGUIPtr->selectedArea == AREA_LAND
		&&  GetLandSlotSelected() >= GetTotalLandMonCount())
		{
			PrintDexNavMessage(MESSAGE_NO_DATA); //No data in slot
			error = TRUE;
		}
		else //Selected unidentified Pokemon
		{
			if (Overworld_GetFlashLevel() > 0)
				PrintDexNavMessage(MESSAGE_TOO_DARK); //Prevent bugs by not letting the player search until room is lit up
			else
				PrintDexNavMessage(MESSAGE_INVALID); //Can't be searched yet

			error = TRUE;	
		}
	}

	if (error)
	{
		sp09A_StopSounds();
		PlaySE(SE_ERROR);
		gTasks[taskId].func = Task_WaitForErrorMessage;
	}
}

static void HideDexNavMessage(void)
{
	if (sDexNavGUIPtr->cursorSpriteId < MAX_SPRITES)
		StartSpriteAnim(&gSprites[sDexNavGUIPtr->cursorSpriteId], 0); //Moving

	HideBg(BG_TEXTBOX);
}

static void ContextMenuMoveCursorFunc(unusedArg s32 listIndex, bool8 onInit, unusedArg struct ListMenu* list)
{
	if (!onInit) //Changing selection
		PlaySE(SE_SELECT);
}

static void ContextMenuItemPrintFunc(unusedArg u8 windowId, unusedArg s32 listIndex, unusedArg u8 y)
{
}

static void ShowSelectedContextMenu(u8 taskId, u16 species)
{
	CleanWindow(WIN_CONTEXT_MENU);

	//Set up the player's chosen border
	TextWindow_SetUserSelectedFrame(WIN_CONTEXT_MENU, 500, 0xE0); //500 is a base block not used
	DrawStdFrameWithCustomTileAndPalette(WIN_CONTEXT_MENU, FALSE, 500, 14);

	//Prepare the list menu
	gTasks[taskId].data[0] = ListMenuInit(&sContextMenuTemplate, 0, 0);
	gTasks[taskId].data[1] = species;

	//Move everything to VRAM
	CommitWindow(WIN_CONTEXT_MENU);
}

static void RemoveContextMenu(u8 taskId)
{
	DestroyTask(gTasks[taskId].data[0]);
	ClearDialogWindowAndFrame(WIN_CONTEXT_MENU, TRUE);
	CleanWindow(WIN_CONTEXT_MENU);
	CommitWindow(WIN_CONTEXT_MENU);
}

static const struct TextColor* GetSpeciesNameTextColour(void)
{
	return &sWhiteText;
}

static const struct TextColor* GetSpeciesDetailsTextColour(void)
{
	#ifdef UNBOUND
		return &sWhiteText;
	#else
		return &sBlackText;
	#endif
}

static void PrintGUIAreaName(void)
{
	CleanWindow(WIN_MAP_NAME);
	GetMapName(gStringVar4, GetCurrentRegionMapSectionId(), 0);
	WindowPrint(WIN_MAP_NAME, 1, 2, 0, &sWhiteText, 0, gStringVar4);
	CommitWindow(WIN_MAP_NAME);
}

static void PrintGUIChainLength(void)
{
	u8 chainLength = (gCurrentDexNavChain == 0) ? 0 : gCurrentDexNavChain - 1; //Always 1 less than what's stored internally
	const struct TextColor* colour = (chainLength == 49 || chainLength == 99) ? &sLightRedText : &sWhiteText;

	CleanWindow(WIN_CHAIN_LENGTH);
	StringCopy(gStringVar4, gText_DexNav_Chain);
	ConvertIntToDecimalStringN(gStringVar1, chainLength, 0, 3);
	StringAppend(gStringVar4, gStringVar1);
	s16 x = GetStringRightAlignXOffset(1, gStringVar4, sDexNavWinTemplates[WIN_CHAIN_LENGTH].width * 8) - 4;

	WindowPrint(WIN_CHAIN_LENGTH, 1, x, 0, colour, 0, gStringVar4);
	CommitWindow(WIN_CHAIN_LENGTH);
}

static void PrintGUIAreaDescriptors(void)
{
	const u8* landText;
	const u8* waterText;
	
	#ifdef UNBOUND
	s16 x = 5;
	s16 landY = 6;
	s16 waterY = 1;
	#else
	s16 x = 5;
	s16 landY = 0;
	s16 waterY = 0;
	#endif

	CleanWindow(WIN_LAND);
	CleanWindow(WIN_WATER);

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
	else if (gMapHeader.mapType == MAP_TYPE_UNDERWATER)
	{
		landText = gText_DexNavReeds;
		waterText = gText_DexNavDeepWater;
	}
	else
	#endif
	{
		landText = gText_DexNavLand;
		waterText = gText_DexNavWater;
	}

	WindowPrint(WIN_LAND, 1, x, landY, &sWhiteText, 0, landText);
	WindowPrint(WIN_WATER, 1, x, waterY, &sWhiteText, 0, waterText);
	CommitWindow(WIN_WATER);
	CommitWindow(WIN_LAND);
}

static void PrintGUISpeciesName(u16 species)
{
	const u8* text = gText_DexNav_NoInfo;
	u8 fontSize = 1;
	CleanWindow(WIN_SPECIES);

	if (species != SPECIES_NONE)
		text = gSpeciesNames[species];

	s16 x = GetStringCenterAlignXOffset(fontSize, text, sDexNavWinTemplates[WIN_SPECIES].width * 8 + 6);
	WindowPrint(WIN_SPECIES, 1, x, 3, GetSpeciesNameTextColour(), 0, text);
	CommitWindow(WIN_SPECIES);
}

static void PrintGUISpeciesTypes(u16 species)
{
	u8 type1 = gBaseStats[species].type1;
	u8 type2 = gBaseStats[species].type2;
	CleanWindow(WIN_MON_TYPE_1);
	CleanWindow(WIN_MON_TYPE_2);

	if (species != SPECIES_NONE)
	{
		blit_move_info_icon(WIN_MON_TYPE_1, type1 + 1, 5, 4);
		if (type1 != type2)
			blit_move_info_icon(WIN_MON_TYPE_2, type2 + 1, 1, 4);
	}

	CommitWindow(WIN_MON_TYPE_1);
	CommitWindow(WIN_MON_TYPE_2);
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
		ConvertIntToDecimalStringN(gStringVar4, gDexNavSearchLevels[dexNum], 0, 4);
		text = gStringVar4;
	}

	WindowPrint(WIN_SEARCH_LEVEL, 0, 4, 3, GetSpeciesDetailsTextColour(), 0, text);
	CommitWindow(WIN_SEARCH_LEVEL);
}

static void PrintGUIEncounterMethod(u8 encounterMethod)
{
	const u8* text = gText_DexNav_NoInfo;
	CleanWindow(WIN_METHOD);

	if (encounterMethod < ENCOUNTER_METHOD_COUNT)
	{
		switch (encounterMethod)
		{
			case ENCOUNTER_METHOD_GRASS:
				text = gText_DexNav_Walk;
				break;
			case ENCOUNTER_METHOD_WATER:
				if (IsCurrentAreaVolcano())
					text = gText_DexNav_LavaSurf;
				else
					text = gText_DexNav_Surf;
				break;
			case ENCOUNTER_METHOD_OLD_ROD:
				text = gText_DexNav_OldRod;
				break;
			case ENCOUNTER_METHOD_GOOD_ROD:
				text = gText_DexNav_GoodRod;
				break;
			case ENCOUNTER_METHOD_SUPER_ROD:
				text = gText_DexNav_SuperRod;
				break;
			case ENCOUNTER_METHOD_SURF_OLD_ROD:
				text = gText_DexNav_SurfOldRod;
				break;
			case ENCOUNTER_METHOD_SURF_GOOD_ROD:
				text = gText_DexNav_SurfGoodRod;
				break;
			case ENCOUNTER_METHOD_SURF_SUPER_ROD:
				text = gText_DexNav_SurfSuperRod;
				break;
			case ENCOUNTER_METHOD_SWARM:
				text = gText_DexNav_Swarm;
				break;
		}
	}

	WindowPrint(WIN_METHOD, 0, 4, 3, GetSpeciesDetailsTextColour(), 0, text);
	CommitWindow(WIN_METHOD);
}

static void PrintGUIHiddenAbility(u16 species)
{
	const u8* text = gText_DexNav_NoInfo;

	CleanWindow(WIN_HIDDEN_ABILITY);

	if (species != SPECIES_NONE)
	{
		u16 dexNum = SpeciesToNationalPokedexNum(species);

		if (GetSetPokedexFlag(dexNum, FLAG_GET_CAUGHT)) //Only display hidden ability if Pokemon has been caught
		{
			u8 hiddenAbility = GetHiddenAbility(species);

			if (species != SPECIES_NONE && hiddenAbility != ABILITY_NONE)
				text = GetAbilityName(hiddenAbility, species);
		}
		else
			text = gText_DexNav_CaptureToSee;
	}

	//Print Text
	u8 xPos = 4;
	u16 windowWidth = (sDexNavWinTemplates[WIN_HIDDEN_ABILITY].width - 1) * 8 - 3; //Leave at least 1 whitespace at the end
	u16 largeWidth = GetStringWidth(0, text, 0);

	if (largeWidth > windowWidth) //Too big to start at a pos of one tile over
		xPos = 2; //Move left to fit

	WindowPrint(WIN_HIDDEN_ABILITY, 0, xPos, 3, GetSpeciesDetailsTextColour(), 0, text);
	CommitWindow(WIN_HIDDEN_ABILITY);
}

static void PrintGUIHeldItems(u16 species)
{
	const u8* item1Text = gText_DexNav_NoInfo;
	const u8* item2Text = gText_EmptyString;
	CleanWindow(WIN_HELD_ITEMS);

	if (species != SPECIES_NONE)
	{
		#ifdef FLAG_UNLOCKED_DEXNAV_HELD_ITEMS
		if (!FlagGet(FLAG_UNLOCKED_DEXNAV_HELD_ITEMS))
		{
			item1Text = gText_DexNav_Unavailable;
		}
		else
		#endif
		{
			u16 dexNum = SpeciesToNationalPokedexNum(species);

			if (GetSetPokedexFlag(dexNum, FLAG_GET_CAUGHT)) //Only display items if Pokemon has been caught
			{
				if (species != SPECIES_NONE)
				{
					u16 item1 = gBaseStats[species].item1;
					u16 item2 = gBaseStats[species].item2;

					if (item1 != ITEM_NONE)
					{
						item1Text = ItemId_GetName(item1);

						if (item2 != ITEM_NONE && item1 != item2)
							item2Text = ItemId_GetName(item2);
					}
					else if (item2 != ITEM_NONE)
						item2Text = ItemId_GetName(item2);
				}
			}
			else
				item1Text = gText_DexNav_CaptureToSee;
		}
	}

	WindowPrint(WIN_HELD_ITEMS, 0, 4, 2, GetSpeciesDetailsTextColour(), 0, item1Text);
	WindowPrint(WIN_HELD_ITEMS, 0, 4, 13, GetSpeciesDetailsTextColour(), 0, item2Text);
	CommitWindow(WIN_HELD_ITEMS);
}

static void DexNavDisplaySpeciesData(void)
{
	u16 species = SPECIES_NONE;
	u8 method = ENCOUNTER_METHOD_COUNT;

	//First determine the correct encounter method and species to display
	if (sDexNavGUIPtr->selectedArea == AREA_WATER)
	{
		u8 waterSlot = GetWaterSlotSelected();
		species = sDexNavGUIPtr->waterSpecies[waterSlot];

		if (species != SPECIES_NONE || waterSlot < GetTotalWaterMonCount())
		{
			u8 newMethod;
			method = ENCOUNTER_METHOD_WATER;

			if (waterSlot >= sDexNavGUIPtr->numWaterMons)
				newMethod = sDexNavGUIPtr->hiddenWaterEncounterMethod[waterSlot - sDexNavGUIPtr->numWaterMons];
			else
				newMethod = sDexNavGUIPtr->waterEncounterMethod[waterSlot];

			if (IsFishingEncounterMethod(newMethod))
				method = newMethod;

			#ifdef UNBOUND
			if (GetCurrentRegionMapSectionId() == MAPSEC_FLOWER_PARADISE)
				method = ENCOUNTER_METHOD_GRASS;
			#endif
		}
	}
	else //Land
	{
		u8 landSlot = GetLandSlotSelected();
		species = sDexNavGUIPtr->grassSpecies[landSlot];

		if (species != SPECIES_NONE || landSlot < GetTotalLandMonCount())
		{
			u8 newMethod;
			method = ENCOUNTER_METHOD_GRASS;

			if (landSlot >= sDexNavGUIPtr->numLandMons)
				newMethod = sDexNavGUIPtr->hiddenLandEncounterMethod[landSlot - sDexNavGUIPtr->numLandMons];
			else
				newMethod = sDexNavGUIPtr->landEncounterMethod[landSlot];

			if (newMethod == ENCOUNTER_METHOD_SWARM)
				method = newMethod;
		}
	}

	TryRandomizeSpecies(&species);

	//Then print the data
	PrintGUISpeciesName(species);
	PrintGUISpeciesTypes(species);
	PrintGUISearchLevel(species);
	PrintGUIEncounterMethod(method);
	PrintGUIHiddenAbility(species);
	PrintGUIHeldItems(species);
}

static u8 CreateNoDataIcon(unusedArg s16 x, unusedArg s16 y)
{
	#ifdef UNBOUND
	return MAX_SPRITES;
	#else
	return CreateSprite(&sNoDataIconTemplate, x, y, 0); //No data in spot
	#endif
}

static void CreateGrayscaleMonIconPalettes(void)
{
	u8 index = IndexOfSpritePaletteTag(POKE_ICON_BASE_PAL_TAG);
	u8 finalIndex = index + 3;

	for (; index < finalIndex; ++index)
	{
		//Make a copy of each mon icon palette in the garbage slots usually loaded after the normal 3
		Memcpy(&gPlttBufferUnfaded2[(index + 3) * 16], &gPlttBufferUnfaded2[index * 16], 0x20);
		Memcpy(&gPlttBufferFaded2[(index + 3) * 16], &gPlttBufferFaded2[index * 16], 0x20);

		//Grayscale that copy
		TintPalette_GrayScale(&gPlttBufferUnfaded2[(index + 3) * 16], 16);
		TintPalette_GrayScale(&gPlttBufferFaded2[(index + 3) * 16], 16);
	}
}

static u16 GetSpeciesAtCursorPos(void)
{
	return sDexNavGUIPtr->selectedArea == AREA_WATER ? sDexNavGUIPtr->waterSpecies[GetWaterSlotSelected()] : sDexNavGUIPtr->grassSpecies[GetLandSlotSelected()];
}

static u16 TryAdjustUnownSpeciesAtCursorPos(u16 species)
{
	u16 randomizedSpecies = species;
	TryRandomizeSpecies(&randomizedSpecies);

	if (randomizedSpecies == SPECIES_UNOWN)
	{
		u8 letter = sDexNavGUIPtr->unownFormsByDNavIndices[GetLandSlotSelected()] - 1;
		if (letter > 0)
			species = SPECIES_UNOWN_B + letter - 1;
	}

	return species;
}

static u8 GetLandSlotSelected(void)
{
	return sDexNavGUIPtr->selectedIndex;
}

static u8 GetLandRowSelected(void)
{
	return sDexNavGUIPtr->selectedIndex / LAND_ROW_LENGTH;
}

static u8 GetLandRowSlotSelected(void)
{
	return sDexNavGUIPtr->selectedIndex % LAND_ROW_LENGTH;
}

static u8 GetWaterSlotSelected(void)
{
	return sDexNavGUIPtr->selectedIndex;
}

static u8 GetWaterRowSelected(void)
{
	return sDexNavGUIPtr->selectedIndex / WATER_ROW_LENGTH;
}

static u8 GetWaterRowSlotSelected(void)
{
	return sDexNavGUIPtr->selectedIndex % WATER_ROW_LENGTH;
}

static bool8 IsCurrentRowEmpty(void)
{
	#ifdef UNBOUND
	if (sDexNavGUIPtr->selectedArea == AREA_LAND)
	{
		if (GetLandRowSelected() * LAND_ROW_LENGTH >= GetTotalLandMonCount())
			return TRUE;
	}
	else //Water
	{
		if (GetWaterRowSelected() * WATER_ROW_LENGTH >= GetTotalWaterMonCount())
			return TRUE;
	}
	#endif

	return FALSE;
}

static bool8 IsOnlyOneRow(void)
{
	#ifdef UNBOUND
	return (GetTotalLandMonCount() == 0 && GetTotalWaterMonCount() <= WATER_ROW_LENGTH)
		|| (GetTotalWaterMonCount() == 0 && GetTotalLandMonCount() <= LAND_ROW_LENGTH);
	#else
	return FALSE; //The X's can be viewed
	#endif
}

static void ChangeSelectedRow(s8 change)
{
	bool8 canWaterScroll = FALSE;

	if (sDexNavGUIPtr->selectedArea == AREA_LAND)
	{
		if (change < 0) //Up
		{
			if (GetLandSlotSelected() < LAND_ROW_LENGTH) //On first row
			{
				if (GetTotalWaterMonCount() > 0)
				{
					sDexNavGUIPtr->selectedArea = AREA_WATER; //Move to water area
					sDexNavGUIPtr->selectedIndex = (GetTotalWaterMonSlots() - WATER_ROW_LENGTH) + GetLandRowSlotSelected(); //Last water row
					canWaterScroll = TRUE; //Flip back to bottom
					sDexNavGUIPtr->waterRowsAbove = GetWaterRowCount() - GetWaterRowSelected();
				}
				else
					sDexNavGUIPtr->selectedIndex = (MAX_TOTAL_LAND_MONS - LAND_ROW_LENGTH) + GetLandRowSlotSelected(); //Wrap to last land row
			}
			else
				sDexNavGUIPtr->selectedIndex -= LAND_ROW_LENGTH; //Move to previous row of land
		}
		else //Down
		{
			if (GetLandSlotSelected() >= MAX_TOTAL_LAND_MONS - LAND_ROW_LENGTH) //On last row
			{
				if (GetTotalWaterMonCount() > 0)
				{
					canWaterScroll = TRUE; //Flip back to top
					sDexNavGUIPtr->selectedArea = AREA_WATER; //Move to water area
				}

				sDexNavGUIPtr->selectedIndex = GetLandRowSlotSelected(); //First water/land row
			}
			else
				sDexNavGUIPtr->selectedIndex += LAND_ROW_LENGTH; //Move to next row of land
		}
	}
	else //AREA_WATER
	{
		if (change < 0) //Up
		{
			if (GetWaterSlotSelected() < WATER_ROW_LENGTH) //On first row
			{
				if (GetTotalLandMonCount() > 0)
				{
					sDexNavGUIPtr->selectedArea = AREA_LAND; //Move to land area
					sDexNavGUIPtr->selectedIndex = (MAX_TOTAL_LAND_MONS - LAND_ROW_LENGTH) + GetLandRowSlotSelected(); //Last land row
				}
				else
					sDexNavGUIPtr->selectedIndex = (GetTotalWaterMonSlots() - WATER_ROW_LENGTH) + GetLandRowSlotSelected(); //Wrap to last water row
			}
			else
			{
				if (sDexNavGUIPtr->waterRowsAbove == 0)
					canWaterScroll = TRUE;
				else
					sDexNavGUIPtr->waterRowsAbove -= 1;
		
				sDexNavGUIPtr->selectedIndex -= WATER_ROW_LENGTH; //Move to previous row of water
			}
		}
		else //Down
		{
			if (GetWaterSlotSelected() >= GetTotalWaterMonSlots() - WATER_ROW_LENGTH) //On last row
			{
				if (GetTotalLandMonCount() > 0)
					sDexNavGUIPtr->selectedArea = AREA_LAND; //Move to land area

				sDexNavGUIPtr->selectedIndex = GetWaterRowSlotSelected(); //First land/water row
			}
			else
			{
			
				if (sDexNavGUIPtr->waterRowsAbove > 0)
					canWaterScroll = TRUE;
				else
					sDexNavGUIPtr->waterRowsAbove += 1;

				sDexNavGUIPtr->selectedIndex += WATER_ROW_LENGTH; //Move to next row of water
			}
		}
	}

	if (IsCurrentRowEmpty())
	{
		ChangeSelectedRow(change); //Go again
		return;
	}

	TrySetSelectedColumnToLastIndex();

	if (canWaterScroll && sDexNavGUIPtr->selectedArea == AREA_WATER) //Still on or just moved to water section
		sDexNavGUIPtr->waterRowScroll = GetWaterRowSelected() >= VISIBLE_WATER_ROW_COUNT ? GetWaterRowSelected() : 0;

	//sDexNavGUIPtr->landRowScroll = GetLandRowSelected();
}

static void ChangeSelectedCol(s8 change)
{
	if (sDexNavGUIPtr->selectedArea == AREA_WATER)
	{
		if (change < 0) //Left
		{
			if (GetWaterRowSlotSelected() == 0)
				sDexNavGUIPtr->selectedIndex += WATER_ROW_LAST_INDEX; //Wrap around
			else
				sDexNavGUIPtr->selectedIndex -= ROW_MON_LENGTH;
		}
		else //Right
		{
			if (GetWaterRowSlotSelected() >= WATER_ROW_LAST_INDEX
			#ifdef UNBOUND
			|| sDexNavGUIPtr->selectedIndex >= GetTotalWaterMonCount() - 1
			#endif
			)
				sDexNavGUIPtr->selectedIndex = GetWaterRowSelected() * WATER_ROW_LENGTH; //Move to beginning of row
			else
				sDexNavGUIPtr->selectedIndex += ROW_MON_LENGTH;
		}
	}
	else //AREA_LAND
	{
		if (change < 0) //Left
		{
			if (GetLandRowSlotSelected() == 0)
				sDexNavGUIPtr->selectedIndex += LAND_ROW_LAST_INDEX; //Wrap around
			else
				sDexNavGUIPtr->selectedIndex -= ROW_MON_LENGTH;
		}
		else //Right
		{
			if (GetLandRowSlotSelected() >= LAND_ROW_LAST_INDEX
			#ifdef UNBOUND
			|| sDexNavGUIPtr->selectedIndex >= GetTotalLandMonCount() - 1
			#endif
			)
				sDexNavGUIPtr->selectedIndex = GetLandRowSelected() * LAND_ROW_LENGTH; //Move to beginning of row
			else
				sDexNavGUIPtr->selectedIndex += ROW_MON_LENGTH;
		}
	}

	TrySetSelectedColumnToLastIndex();
}

static void TrySetSelectedColumnToLastIndex(void)
{
	#ifdef UNBOUND
	if (sDexNavGUIPtr->selectedArea == AREA_LAND && sDexNavGUIPtr->selectedIndex >= GetTotalLandMonCount())
		sDexNavGUIPtr->selectedIndex = GetTotalLandMonCount() - 1; //Assumes it'll stay on the same row
	else if (sDexNavGUIPtr->selectedArea == AREA_WATER && sDexNavGUIPtr->selectedIndex >= GetTotalWaterMonCount())
		sDexNavGUIPtr->selectedIndex = GetTotalWaterMonCount() - 1; //Assumes it'll stay on the same row
	#endif
}

//Sprite Callbacks//

static void UpdateSpritePosition(struct Sprite* sprite, u8 areaType, u8 selectedIndex)
{
	s16 baseX, baseY, xSpace, ySpace, row, col;

	if (areaType == AREA_WATER)
	{
		baseX = 4 + (32 / 2);
		baseY = 22 + (32 / 2);
		xSpace = 24;
		ySpace = 25;
		row = min(selectedIndex / WATER_ROW_LENGTH, VISIBLE_WATER_ROW_COUNT - 1);
		col = selectedIndex % WATER_ROW_LENGTH;
	}
	else //Land
	{
		baseX = 4 + (32 / 2);
		baseY = 94 + (32 / 2);
		xSpace = 24;
		ySpace = 25;
		row = min(selectedIndex / LAND_ROW_LENGTH, VISIBLE_LAND_ROW_COUNT - 1);
		col = selectedIndex % LAND_ROW_LENGTH;
	}

	sprite->pos1.x = baseX + (xSpace * col);
	sprite->pos1.y = baseY + (ySpace * row);
}

static void SpriteCB_GUICursor(struct Sprite* sprite)
{
	u8 selectedIndex = sDexNavGUIPtr->selectedIndex;

	if (sDexNavGUIPtr->selectedArea == AREA_WATER && sDexNavGUIPtr->waterRowScroll != 0)
		selectedIndex = (selectedIndex % WATER_ROW_LENGTH) + (sDexNavGUIPtr->waterRowsAbove * WATER_ROW_LENGTH); //Adjust for offset
		
	UpdateSpritePosition(sprite, sDexNavGUIPtr->selectedArea, selectedIndex);
}

static void SpriteCB_GUIRegisteredIcon(struct Sprite* sprite)
{
	if (!sDexNavGUIPtr->registeredIconVisible) //No mon is registered
	{
		sprite->invisible = TRUE;
	}
	else
	{
		u8 index = sDexNavGUIPtr->registeredIndex;

		if (sDexNavGUIPtr->registeredArea == AREA_WATER && sDexNavGUIPtr->waterRowScroll != 0)
			index = (index % WATER_ROW_LENGTH) + (sDexNavGUIPtr->waterRowsAbove * WATER_ROW_LENGTH); //Adjust for offset

		sprite->invisible = FALSE;
		UpdateSpritePosition(sprite, sDexNavGUIPtr->registeredArea, index);
	}
}

static void SpriteCB_LandMonIcon(struct Sprite* sprite)
{
	if (!sprite->data[1]) //Caught mon
		SpriteCB_PokeIcon(sprite);
}

static void SpriteCB_WaterMonIcon(struct Sprite* sprite)
{
	u8 rowNumber = sprite->data[0];

	if (!sprite->data[1]) //Caught mon
		SpriteCB_PokeIcon(sprite);

	sprite->invisible = TRUE; //Hide by default

	if (sDexNavGUIPtr->waterRowScroll == 0) //No scroll yet
	{
		sprite->pos2.y = 0;
		if (rowNumber < VISIBLE_WATER_ROW_COUNT)
			sprite->invisible = FALSE;
	}
	else
	{
		if (rowNumber == sDexNavGUIPtr->waterRowScroll)
			sprite->invisible = FALSE;
		else if (rowNumber == sDexNavGUIPtr->waterRowScroll - 1) //Previous row
		{
			sprite->invisible = FALSE;
			sprite->pos2.y = -25; //ySpace above
		}
	}
}


//Input Handler//

static void Task_DexNavFadeIn(u8 taskId)
{
	if (!gPaletteFade->active)
	{
		if (!AnyPokemonInCurrentArea())
		{
			PrintDexNavMessage(MESSAGE_NO_POKEMON_HERE);
			gTasks[taskId].func = Task_PrintMessageThenClose;
		}
		else
			gTasks[taskId].func = Task_DexNavWaitForKeyPress;
	}
}

static void Task_PrintMessageThenClose(u8 taskId)
{
	if (!RunTextPrintersAndIsPrinter0Active())
	{
		BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 16, RGB_BLACK);
		gTasks[taskId].func = Task_DexNavFadeOutToStartMenu;
	}
}

static void Task_DexNavWaitForKeyPress(u8 taskId)
{
	if (JOY_NEW(A_BUTTON))
	{
		//Check selection is valid. Play sound if invalid
		u16 species = GetSpeciesAtCursorPos();

		if (!CanWaterMonBeSearched())
			PrintDexNavError(taskId, MESSAGE_NEED_ROD);
		else if (species != SPECIES_NONE && Overworld_GetFlashLevel() == 0) //DexNav doesn't work in dark areas (sprites wouldn't show up)
		{
			species = TryAdjustUnownSpeciesAtCursorPos(species);

			//Species was valid, open context menu to confirm user action
			PlaySE(SE_SELECT);
			gTasks[taskId].data[1] = species;
			TryRandomizeSpecies(&species);
			CopySpeciesNameToStringVar1_AdjustUnownName(species);
			PrintDexNavMessage(MESSAGE_POKEMON_SELECTED);
			gTasks[taskId].func = Task_WaitForContextMenuPreMessage;
		}
		else //No species in this slot
		{
			PrintDexNavError(taskId, 0xFF);
		}
	}
	else if (JOY_NEW(R_BUTTON))
	{
		//Check selection is valid. Play sound if invalid
		u16 species = GetSpeciesAtCursorPos();

		if (!CanWaterMonBeSearched())
			PrintDexNavError(taskId, MESSAGE_NEED_ROD);
		else if (species != SPECIES_NONE && Overworld_GetFlashLevel() == 0)
		{
			species = TryAdjustUnownSpeciesAtCursorPos(species);
			RegisterSpecies(species, taskId);
		}
		else //Not valid species
			PrintDexNavError(taskId, 0xFF);
	}
	else if (JOY_NEW(B_BUTTON))
	{
		BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 16, RGB_BLACK);
		gTasks[taskId].func = Task_DexNavFadeOutToStartMenu;
	}
	else if (JOY_REPT(DPAD_DOWN))
	{
		if (!IsOnlyOneRow())
		{
			ChangeSelectedRow(1);
			DexNavDisplaySpeciesData();
			PlaySE(SE_POKENAV_ONE_BEEP);
		}
		else
			PlaySE(SE_ERROR);
	}
	else if (JOY_REPT(DPAD_UP))
	{
		if (!IsOnlyOneRow())
		{
			ChangeSelectedRow(-1);
			DexNavDisplaySpeciesData();
			PlaySE(SE_POKENAV_ONE_BEEP);
		}
		else
			PlaySE(SE_ERROR);
	}
	else if (JOY_REPT(DPAD_LEFT))
	{
		ChangeSelectedCol(-1);
		DexNavDisplaySpeciesData();
		PlaySE(SE_POKENAV_ONE_BEEP);
	}
	else if (JOY_REPT(DPAD_RIGHT))
	{
		ChangeSelectedCol(1);
		DexNavDisplaySpeciesData();
		PlaySE(SE_POKENAV_ONE_BEEP);
	}
}

static void Task_WaitForContextMenuPreMessage(u8 taskId)
{
	if (!RunTextPrintersAndIsPrinter0Active())
	{
		u16 species = gTasks[taskId].data[1];
		ShowSelectedContextMenu(taskId, species);
		gTasks[taskId].func = Task_HandleContextMenu;
	}
}

static void Task_WaitForErrorMessage(u8 taskId)
{
	if (!RunTextPrintersAndIsPrinter0Active())
	{
		HideDexNavMessage();
		gTasks[taskId].func = Task_DexNavWaitForKeyPress;
	}
}

static void Task_HandleContextMenu(u8 taskId)
{
	s32 input = ListMenu_ProcessInput(gTasks[taskId].data[0]);
	switch (input)
	{
		case LIST_CANCEL:
			RETURN_TO_MAIN_INTERFACE:
			PlaySE(SE_SELECT);
			RemoveContextMenu(taskId);
			HideDexNavMessage();
			gTasks[taskId].func = Task_DexNavWaitForKeyPress;
			break;
		case LIST_NOTHING_CHOSEN:
			break;
		default:
			switch (input)
			{
				case 0: //Register
					RegisterSpecies(gTasks[taskId].data[1], taskId); //Species
					RemoveContextMenu(taskId);
					break;
				case 1: //Scan
					Var8008 = gTasks[taskId].data[1]; //Species
					Var8009 = sDexNavGUIPtr->selectedArea;
					PlaySE(SE_POKENAV_SEARCHING);
					RemoveContextMenu(taskId);
					HideDexNavMessage();
					BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 16, RGB_BLACK);
					gTasks[taskId].func = Task_DexNavFadeOutToScan;
					break;
				default: //Close
					goto RETURN_TO_MAIN_INTERFACE;
			}
	}
}

static void FreeAndCloseDexNav(u8 taskId)
{
	Free(sDexNavGUIPtr);
	FreeAllWindowBuffers();
	BGMVolumeMax_EnableHelpSystemReduction();
	DestroyTask(taskId);
}

static void Task_DexNavFadeOutToStartMenu(u8 taskId)
{
	if (!gPaletteFade->active)
	{
		SetMainCallback2(CB2_ReturnToFieldWithOpenMenu);
		FreeAndCloseDexNav(taskId);
	}
}

static void Task_DexNavFadeOutToScan(u8 taskId)
{
	if (!gPaletteFade->active)
	{
		PrepareOverworldReturn();
		SetMainCallback1(CB1_OpenDexNavScan);
		SetMainCallback2(CB2_ReturnToFieldFromDiploma); //Needed so followers don't get messed up
		FreeAndCloseDexNav(taskId);
	}
}

static void CB1_OpenDexNavScan(void)
{
	ExecDexNavHUD();
}


//Setup//

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

static void DexNavPrintStaticText(void)
{
	PrintGUIAreaName();
	PrintGUIChainLength();
	PrintGUIAreaDescriptors();
}

static void DexNavLoadMonIcons(void)
{
	u8 letter;
	u32 pid = 0xFFFFFFFF;
	u8 hiddenLandMons = sDexNavGUIPtr->numHiddenLandMons;
	u8 hiddenWaterMons = sDexNavGUIPtr->numHiddenWaterMons;
	u8 registeredArea = VarGet(VAR_DEXNAV) >> 15;
	u16 registeredSpecies = VarGet(VAR_DEXNAV) & 0x7FFF;

	#ifdef VAR_R_BUTTON_MODE
	if (VarGet(VAR_R_BUTTON_MODE) != OPTIONS_R_BUTTON_MODE_DEXNAV)
		registeredSpecies = SPECIES_NONE;
	#endif

	LoadCompressedSpriteSheetUsingHeap(&sNoDataIconSpriteSheet);
	LoadMonIconPalettes();
	CreateGrayscaleMonIconPalettes(); //For mons that haven't been caught yet

	for (u8 i = 0; i < MAX_TOTAL_LAND_MONS; ++i)
	{
		u16 species = sDexNavGUIPtr->grassSpecies[i];

		if (species == SPECIES_NONE)
		{
			if (hiddenLandMons == 0)
			{
				u8 spriteId = CreateNoDataIcon(0, 0);
				if (spriteId < MAX_SPRITES)
					UpdateSpritePosition(&gSprites[spriteId], AREA_LAND, i);
				continue;
			}
			else
				hiddenLandMons--;
		}

		letter = sDexNavGUIPtr->unownFormsByDNavIndices[i];
		if (letter > 0)
			pid = GenerateUnownPersonalityByLetter(letter - 1);

		TryRandomizeSpecies(&species);
		u8 spriteId = CreateMonIcon(species, SpriteCB_LandMonIcon, 0, 0, (MAX_TOTAL_LAND_MONS - i) + 1, pid, 0);
		if (spriteId < MAX_SPRITES)
		{
			UpdateSpritePosition(&gSprites[spriteId], AREA_LAND, i);

			if (letter > 1) //This slot is an Unown greater than A
				species = SPECIES_UNOWN_B + letter - 2;

			if (species != SPECIES_NONE)
			{
				if (registeredArea == AREA_LAND && registeredSpecies == species && registeredSpecies != SPECIES_NONE)
				{
					sDexNavGUIPtr->selectedArea = sDexNavGUIPtr->registeredArea = AREA_LAND;
					sDexNavGUIPtr->selectedIndex = sDexNavGUIPtr->registeredIndex = i;
					sDexNavGUIPtr->registeredIconVisible = TRUE;
				}

				if (!GetSetPokedexFlag(SpeciesToNationalPokedexNum(species), FLAG_GET_CAUGHT)) //Not caught
				{
					gSprites[spriteId].data[1] = TRUE; //Disallow movement
					gSprites[spriteId].oam.paletteNum += 3; //Switch to grayscale pal
				}
			}
		}
	}

	for (u8 i = 0; i < GetTotalWaterMonSlots(); ++i)
	{
		u16 species = sDexNavGUIPtr->waterSpecies[i];
		u8 rowNumber = i / WATER_ROW_LENGTH;

		if (species == SPECIES_NONE)
		{
			if (hiddenWaterMons == 0)
			{
				u8 spriteId = CreateNoDataIcon(0, 0);
				if (spriteId < MAX_SPRITES)
				{
					UpdateSpritePosition(&gSprites[spriteId], AREA_WATER, i);
					gSprites[spriteId].data[0] = rowNumber;
					gSprites[spriteId].callback = SpriteCB_WaterMonIcon;
				}

				continue;
			}
			else
				hiddenWaterMons--;
		}

		letter = PickUnownLetter(species, i);
		if (letter > 0)
			pid = GenerateUnownPersonalityByLetter(letter - 1);

		TryRandomizeSpecies(&species);
		u8 spriteId = CreateMonIcon(species, SpriteCB_WaterMonIcon, 0, 0, (GetTotalWaterMonSlots() - i) + 1, pid, 0);
		if (spriteId < MAX_SPRITES)
		{
			gSprites[spriteId].data[0] = rowNumber;
			UpdateSpritePosition(&gSprites[spriteId], AREA_WATER, i);

			if (species != SPECIES_NONE)
			{
				if (letter > 1) //This slot is an Unown greater than A
					species = SPECIES_UNOWN_B + letter - 2;

				if (registeredArea == AREA_WATER && registeredSpecies == species && registeredSpecies != SPECIES_NONE)
				{
					if (!sDexNavGUIPtr->registeredIconVisible
					|| TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_SURFING)) //Only override a land mon if actually on water
					{
						sDexNavGUIPtr->selectedArea = sDexNavGUIPtr->registeredArea = AREA_WATER;
						sDexNavGUIPtr->selectedIndex = sDexNavGUIPtr->registeredIndex = i;
						sDexNavGUIPtr->registeredIconVisible = TRUE;
					}
				}

				if (!GetSetPokedexFlag(SpeciesToNationalPokedexNum(species), FLAG_GET_CAUGHT)) //Not caught
				{
					gSprites[spriteId].data[1] = TRUE; //Disallow movement
					gSprites[spriteId].oam.paletteNum += 3; //Switch to grayscale pal
				}
			}
		}
	}
}

static void CreateWaterScrollArrows(void)
{
	if (GetWaterRowCount() > 2)
	{
		s16 rowCount = (s32) GetWaterRowCount() - 2 + 1; //Max 2 showed at a time
		if (rowCount < 0)
			rowCount = 0;
			
		AddScrollIndicatorArrowPairParameterized(
			SCROLL_ARROW_DOWN,
			79, //X
			25, //Top Y
			25 + 62, //Bottom Y
			rowCount, //Total items
			110,
			110,
			&sDexNavGUIPtr->waterRowScroll
		);
	}
}

static void CreateCursor(void)
{
	if (AnyPokemonInCurrentArea()) //Don't bother if going to close right away
	{
		LoadSpriteSheet(&sCursorSpriteSheet);
		LoadSpritePalette(&sCursorSpritePalette);
		sDexNavGUIPtr->cursorSpriteId = CreateSprite(&sGUICursorTemplate, 30, 48, 0);
	}
	else
		sDexNavGUIPtr->cursorSpriteId = MAX_SPRITES;
}

static void CreateRegisteredIcon(void)
{
	if (AnyPokemonInCurrentArea()) //Don't bother if going to close right away
	{
		LoadCompressedSpriteSheet(&sRegisteredIconSpriteSheet); //Uses the cursor palette
		u8 spriteId = CreateSprite(&sRegisteredIconTemplate, 0, 0, 1);
		if (spriteId < MAX_SPRITES)
		{
			gSprites[spriteId].pos2.x = -8;
			gSprites[spriteId].pos2.y = -6;
		}
	}
}

static void DexNavLoadCapturedAllSymbol(void)
{
	LoadCompressedSpriteSheetUsingHeap(&sCapturedAllPokemonSpriteSheet);
	LoadCompressedSpritePaletteUsingHeap(&sCapturedAllPokemonSpritePalette);

	if (CapturedAllLandBasedPokemon())
	{
		#ifdef UNBOUND
		s16 x = 153; s16 y = 93;
		#else
		s16 x = 154; s16 y = 95;
		#endif
		CreateSprite(&sCapturedAllPokemonSymbolTemplate,  x, y, 0);
	}

	if (CapturedAllWaterBasedPokemon())
	{
		#ifdef UNBOUND
		s16 x = 153; s16 y = 24;
		#else
		s16 x = 154; s16 y = 23;
		#endif
		CreateSprite(&sCapturedAllPokemonSymbolTemplate,  x, y, 0);
	}
}

static void InitDexNavGUI(void)
{
	CleanWindows(); //Prevents black bar bug
	CommitWindows();

	DexNavPopulateEncounterList();
	DexNavLoadMonIcons(); //Must be done before printing text because it can alter the default cursor
	DexNavDisplaySpeciesData();
	DexNavPrintStaticText();
	CreateWaterScrollArrows();
	CreateCursor();
	CreateRegisteredIcon();
	DexNavLoadCapturedAllSymbol();
}

static void LoadDexNavBgGfx(void)
{
	const u8 *tiles, *map;
	const u16 *palette, *altPalette;

	#ifdef UNBOUND
	tiles = DexNavBGUnboundTiles;
	map = DexNavBGUnboundMap;
	palette = DexNavBGUnboundPal;
	#else
	tiles = DexNavBGTiles;
	map = DexNavBGMap;
	palette = DexNavBGPal;
	#endif

	decompress_and_copy_tile_data_to_vram(BG_BACKGROUND, tiles, 0, 0, 0);
	LZDecompressWram(map, sDexNavGUIPtr->tilemapPtr);

	//Choose palette based on current location
	if (IsMapTypeIndoors(GetCurrentMapType()))
		altPalette = DexNavBG_IndoorPal;
	else if (IsCurrentAreaVolcano()) //Load special palette for volcanos
		altPalette = DexNavBG_VolcanoPal;
	#ifdef UNBOUND
	else if (MAP_IS(FLOWER_PARADISE_A))
		altPalette = DexNavBG_FlowerParadiseAPal;
	else if (MAP_IS(FLOWER_PARADISE_B))
		altPalette = DexNavBG_FlowerParadiseBPal;
	else if (MAP_IS(FLOWER_PARADISE_C))
		altPalette = DexNavBG_FlowerParadiseCPal;
	#endif
	else if (IsCurrentAreaDarkerCave())
		altPalette = DexNavBG_DarkerCavePal;
	else if (GetCurrentMapType() == MAP_TYPE_UNDERGROUND)
		altPalette = DexNavBG_CavePal;
	else if (IsCurrentAreaAutumn())
		altPalette = DexNavBG_AutumnPal;
	else if (IsCurrentAreaWinter())
		altPalette = DexNavBG_WinterPal;
	else if (IsCurrentAreaDesert())
		altPalette = DexNavBG_DesertPal;
	else if (IsCurrentAreaSwamp())
		altPalette = DexNavBG_SwampPal;
	else
		altPalette = NULL;

	LoadPalette(palette, 0, 0x20); //Pal 0
	if (altPalette != NULL)
		LoadPalette(altPalette + 1, 1, 0x2 * 6); //Pal 0 - copy first 6 real colours
	LoadMenuElementsPalette(12 * 0x10, 1); //Pal 12
	Menu_LoadStdPalAt(15 * 0x10); //Pal 15
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
			SetBGMVolume_SuppressHelpSystemReduction(160);
			SetVBlankCallback(NULL);
			ClearVramOamPlttRegs();
			gMain.state++;
			break;
		case 1:
			ClearTasksAndGraphicalStructs();
			gMain.state++;
			break;
		case 2:
			sDexNavGUIPtr->tilemapPtr = Malloc(0x1000);
			ResetBgsAndClearDma3BusyFlags(0);
			InitBgsFromTemplates(0, sDexNavBgTemplates, NELEMS(sDexNavBgTemplates));
			SetBgTilemapBuffer(BG_BACKGROUND, sDexNavGUIPtr->tilemapPtr);
			gMain.state++;
			break;
		case 3:
			LoadDexNavBgGfx();
			gMain.state++;
			break;
		case 4:
			if (!free_temp_tile_data_buffers_if_possible())
			{
				ShowBg(BG_TEXT);
				ShowBg(BG_TEXT_2);
				ShowBg(BG_BACKGROUND);
				CopyBgTilemapBufferToVram(BG_BACKGROUND);
				gMain.state++;
			}
			break;
		case 5:
			Free(sDexNavGUIPtr->tilemapPtr);
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
			InitDexNavGUI();
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
		sDexNavGUIPtr = Calloc(sizeof(struct DexNavGUIData));
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
