#include "defines.h"
#include "../include/battle.h"
#include "../include/bg.h"
#include "../include/event_data.h"
#include "../include/event_object_movement.h"
#include "../include/field_effect.h"
#include "../include/field_message_box.h"
#include "../include/field_player_avatar.h"
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
#include "../include/script.h"
#include "../include/sprite.h"
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
	functions for the simplified dexnav system
		credits to FBI: https://github.com/EternalCode/Dexnav

	Known BUGS:
		-GUI gets messed up in dark, flashable rooms
*/

#define sDNavState (*((struct DexnavHudData**) 0x203E038))

#define ROW_LAND 0
#define ROW_WATER 1

#define IS_NEWER_UNOWN_LETTER(species) (species >= SPECIES_UNOWN_B && species <= SPECIES_UNOWN_QUESTION)

//This file's functions:
static void DestroyTaskCompletedTextbox(u8 tId);
static void MsgNormal(const u8* str);
static void VblackCallbackSeq(void);
static void ClearHandlers(void);
static void DexNavGUICallback2(void);
static void ResetPalSettings(void);
static void ResetBgSettings(void);
static void Setup(void);
static bool8 SpeciesInArray(u16 species, u8 indexCount, u8 unownLetter);
static void DexNavGetMon(u16 species, u8 potential, u8 level, u8 ability, u16* moves, u8 searchLevel, u8 chain);
static u8 FindHeaderIndexWithLetter(u16 species, u8 letter);
static u8 PickTileScreen(u8 targetBehaviour, u8 areaX, u8 areaY, s16 *xBuff, s16 *yBuff, u8 smallScan);
static u8 DexNavPickTile(u8 environment, u8 xSize, u8 ySize, bool8 smallScan);
static u8 ShakingGrass(u8 environment, u8 xSize, u8 ySize, bool8 smallScan);
//static void DexHUDHBlank(void);
static void DexNavProximityUpdate(void);
static void NullSubHBlank(void);
static void DexNavFreeHUD(void);
static void DexNavShowMessage(u8 id);
static void OutlinedFontDraw(u8 objId, u8 tileNum, u16 size);
static void DexNavSightUpdate(u8 sight);
static void DexNavIconsVisionUpdate(u8 proximity, u8 searchLevel);
static void DexNavManageHUD(u8 taskId);
static u8 GetEncounterLevel(u16 species, u8 environment);
static u8 DexNavGenerateMonLevel(u16 species, u8 chainLevel, u8 environment);
static u16 DexNavGenerateHeldItem(u16 species, u8 searchLevel);
static u8 DexNavGenerateHiddenAbility(u16 species, u8 searchLevel);
static u8 DexNavGeneratePotential(u8 searchLevel);
static void DexNavGenerateMoveset(u16 species, u8 searchLevel, u8 encounterLevel, u16* moveLoc);
static void DexNavDrawBlackBars(u8* objidAddr);
static void DexNavDrawSight(u8 sight_lvl, u8* objidAddr);
static void DexNavDrawAbility(u8 ability, u8* objidAddr);
static void DexNavDrawMove(u16 move, u8 searchLevel, u8* objidAddr);
static void DexNavDrawPotential(u8 potential, u8* objidAddr);
static void DexNavHudDrawSpeciesIcon(u16 species, u8* objIdAddr);
static void DexNavDrawHeldItem(u8* objidAddr);
static void DexNavDrawIcons(void);
void InitDexNavHUD(u16 species, u8 environment);
static void ExecDexNavHUD(void);
//static u8 StartMenuDexNavCallback(void);
static void DexNavGuiSetup(void);
static void DexNavLoadPokeIcons(void);
static void CreateNoDataIcon(s16 x, s16 y);
static void UpdateCursorPosition(void);
static void SpawnPointerArrow(void);
static void DexNavLoadAreaNames(void);
static void DexNavLoadNames(u8 status);
static void DexNavLoadCapturedAllSymbol(void);
static void CallbackDexNavOW(void);
static void DexNavGuiExitSearch(void);
static void DexNavGuiExitNoSearch(void);
static void DexNavPopulateEncounterList(void);
static void DexNavGuiHandler(void);
static void PrintDexNavError(void);
static void FieldEff_CaveDust(void);
static void FieldEff_Sparkles(void);
void ToolSelection(u8 taskId);
//static void CloseStartMenu(void);

static void DestroyTaskCompletedTextbox(u8 tId)
{
	if (*gBoxStatusAndType != 1)
	{
		HideFieldMessageBox();
		DestroyTask(tId);
		ScriptContext2_Disable();
	}
}


static void MsgNormal(const u8* str)
{
   TextboxFdecodeAutoAndCreateTask(str);
   *gBoxStatusAndType = 1;
   SetTextboxSignpostDesign();
   CreateTask(DestroyTaskCompletedTextbox, 0x1);
   ScriptContext2_Enable();
   DismissMapNamePopup();
   return;
}

static void VblackCallbackSeq(void)
{
	LoadOam();
	ProcessSpriteCopyRequests();
	TransferPlttBuffer();
}

static void ResetGPU(void)
{
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

static void ClearHandlers(void)
{
	SetVBlankCallback(NULL);
	DmaFill16(3, 0, VRAM, VRAM_SIZE);
	DmaFill32(3, 0, OAM, OAM_SIZE);
	DmaFill16(3, 0, PLTT, PLTT_SIZE);
	ResetGPU();
}

static void DexNavGUICallback2(void)
{
	BuildOamBuffer();
	AnimateSprites();
	UpdatePaletteFade();
	RunTasks();
	DoScheduledBgTilemapCopiesToVram();
	// merge textbox and text tile maps
	RunTextPrinters();
}


static void ResetPalSettings(void)
{
	ResetPaletteFade();
	FadeBgPalAndFillBlack();
	FreeAllSpritePalettes();
	PalTagsStart = 0;
}


static void ResetBgSettings(void)
{
	CleanupOverworldWindowsAndTilemaps();
	ResetBgsAndClearDma3BusyFlags(0);
	ChangeBgX(0, 0, 0);
	ChangeBgY(0, 0, 0);
	ChangeBgX(1, 0, 0);
	ChangeBgY(1, 0, 0);
	ChangeBgX(2, 0, 0);
	ChangeBgY(2, 0, 0);
	ChangeBgX(3, 0, 0);
	ChangeBgY(3, 0, 0);
}


static void Setup(void)
{
	ClearHandlers();	// callbacks
	ResetBgSettings();	// BGs
	ResetPalSettings();	// pals
	ResetSpriteData();	// objs
	FreeSpriteTileRanges();
	u32 set = 0;
	CpuFastSet((void*)&set, (void*)VRAM, CPUModeFS(0x10000, CPUFSSET)); 	// VRAM clear
	// gTasks
	MallocInit((void*) 0x2000000, 0x1C000);
	ResetTasks();
}


static bool8 SpeciesInArray(u16 species, u8 indexCount, u8 unownLetter)
{
	u16 dexNum = SpeciesToNationalPokedexNum(species);

	//Disallow species not seen
	if (!GetSetPokedexFlag(dexNum, FLAG_GET_SEEN))
	{
		for (int i = 0; i < NUM_LAND_MONS; ++i)
		{
			#ifdef SPECIES_UNOWN
			if (species == SPECIES_UNOWN && InTanobyRuins())
			{
				if (sDNavState->unownForms[i] == unownLetter) //Already in array
					return TRUE;
			}
			else
			#endif
			if (sDNavState->hiddenSpecies[i] == SPECIES_TABLES_TERMIN)
			{
				sDNavState->hiddenSpecies[i] = dexNum;
				sDNavState->hiddenSpecies[i + 1] = SPECIES_TABLES_TERMIN;
				break;
			}
			else if (sDNavState->hiddenSpecies[i] == dexNum) //Already in array
				return TRUE;
		}

		if (indexCount == NUM_LAND_MONS)
			sDNavState->numHiddenLandMons++; //Increase how many question marks to print
		else
			sDNavState->numHiddenWaterMons++;

		return TRUE;
	}

	for (u8 i = 0; i < indexCount; ++i)
	{
		if (indexCount == NUM_LAND_MONS)
		{
			#ifdef SPECIES_UNOWN
			if (species == SPECIES_UNOWN && InTanobyRuins()) //This Unown code is copied from above b/c either
			{												 //all Unown are seen, or none at all
				if (sDNavState->unownForms[i] == unownLetter) //Already in array
					return TRUE;
			}
			else
			#endif
			if (SpeciesToNationalPokedexNum(sDNavState->grassSpecies[i]) == dexNum)
				return TRUE;
		}
		else
		{
			if (SpeciesToNationalPokedexNum(sDNavState->waterSpecies[i]) == dexNum)
				return TRUE;
		}
	}

	return FALSE;
}


// ===================================== //
// ===== Dex Nav Pokemon Generator ===== //
// ===================================== //


void DexNavGetMon(u16 species, u8 potential, u8 level, u8 ability, u16* moves, u8 searchLevel, u8 chain)
{
	struct Pokemon* pkmn = &gEnemyParty[0];

	//Try Force Shiny
	//https://bulbapedia.bulbagarden.net/wiki/DexNav#Shiny_probability
	u32 i, otherValue, numChecks, charmBonus, chainBonus, randBonus;
	otherValue = 0;
	charmBonus = (CheckBagHasItem(ITEM_SHINY_CHARM, 1) > 0) ? 2 : 0;
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
	CreateWildMon(species, level, FindHeaderIndexWithLetter(species, sDNavState->unownLetter - 1), TRUE);

	//Pick potential ivs to set to 31
	u8 iv[3];
	for (i = 0; i < NELEMS(iv); ++i)
		iv[i] = Random() % 6;

	if ((iv[0] != iv[1]) && (iv[0] != iv[2]) && (iv[1] != iv[2]))
	{
		u8 perfectIv = 0x1F;
		if (potential > 2)
			SetMonData(pkmn, MON_DATA_HP_IV + iv[2], &perfectIv);
		else if (potential > 1)
			SetMonData(pkmn, MON_DATA_HP_IV + iv[1], &perfectIv);
		else if (potential)
			SetMonData(pkmn, MON_DATA_HP_IV + iv[0], &perfectIv);
	}

	//Set ability
	if (gBaseStats[species].hiddenAbility == ability)
		pkmn->hiddenAbility = TRUE;
	else
		GiveMonNatureAndAbility(pkmn, GetNature(pkmn), (gBaseStats[species].ability2 == ability) ? 1 : 0, FALSE);

	//Set moves
	for (i = 0; i < MAX_MON_MOVES; ++i)
	{
		pkmn->moves[i] = sDNavState->moveId[i];
		pkmn->pp[i] = gBattleMoves[moves[i]].pp;
	}

	CalculateMonStats(pkmn);
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

	//u8 searchLevel = sDNavState->searchLevel / 5;
	//u8 chance = searchLevel + 1;
	return PickTileScreen(targetBehaviour, xSize, ySize, &(sDNavState->tileX), &(sDNavState->tileY), smallScan);
}


static bool8 ShakingGrass(u8 environment, u8 xSize, u8 ySize, bool8 smallScan)
{
	if (DexNavPickTile(environment, xSize, ySize, smallScan))
	{
		u8 metatileBehaviour = MapGridGetMetatileField(sDNavState->tileX, sDNavState->tileY, 0xFF);
		gFieldEffectArguments->effectPos.x = sDNavState->tileX;
		gFieldEffectArguments->effectPos.y = sDNavState->tileY;
		gFieldEffectArguments->priority = 0xFF; // height.
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
				if (IsCurrentAreaVolcano())
					FieldEffectStart(FLDEFF_LAVA_BUBBLES);
				else
					FieldEffectStart(FLDEFF_SPLASHING_WATER);
				break;
			default:
				FieldEffectStart(FLDEFF_REPEATING_SPARKLES); //So the game doesn't crash on something useless
				break;
		};

		//Get objid of shaking grass
		for (u32 i = 0; i < MAX_SPRITES; ++i)
		{
			if (gSprites[i].callback == (SpriteCallback) 0x080DCD1D)
			{
				sDNavState->objIdShakingGrass = i;
				return TRUE;
			}
		}

		return TRUE;
	}

	return FALSE;
};

/* //Causes the game to lag due to interference from DNS :(
static void DexHUDHBlank(void)
{
	if (REG_VCOUNT > (160 - 19)) //Fill 19 pixels
	{
		REG_BLDY = 0xC;
		REG_WININ = WININ_BUILD(WIN_BG0 | WIN_BG1 | WIN_BG2 | WIN_BG3 | WIN_OBJ | WIN_BLD,  WIN_BG0 | WIN_BG1 | WIN_BG2 | WIN_BG3 | WIN_OBJ | WIN_BLD);
		REG_BLDCNT = (BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_TGT1_BG3 | BLDCNT_TGT1_OBJ | BLDCNT_TGT1_BD | BLDCNT_TGT2_BG0 | BLDCNT_TGT2_BG1 | BLDCNT_TGT2_BG2 | BLDCNT_TGT2_BG3 | BLDCNT_EFFECT_DARKEN);
	}
	else //Default values from overworld
	{
		REG_WININ = WININ_BUILD(WIN_BG0 | WIN_BG1 | WIN_BG2 | WIN_BG3 | WIN_OBJ, WIN_BG0 | WIN_BG1 | WIN_BG2 | WIN_BG3 | WIN_OBJ);
		REG_BLDCNT = BLDALPHA_BLEND(BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_TGT1_BG3 | BLDCNT_TGT1_OBJ, 0);
	}
};
*/

static void DexNavProximityUpdate(void)
{
	sDNavState->proximity = GetPlayerDistance(sDNavState->tileX, sDNavState->tileY);
};


static void NullSubHBlank(void)
{
};

static void DexNavFreeHUD(void)
{
	switch (sDNavState->environment)
	{
		case 0:
			if (!IsMapTypeOutdoors(GetCurrentMapType()))
				FieldEffectStop(&gSprites[sDNavState->objIdShakingGrass], 0x1A); //cave
			else
				FieldEffectStop(&gSprites[sDNavState->objIdShakingGrass], 0x13);
			break;
		case 1:
			FieldEffectStop(&gSprites[sDNavState->objIdShakingGrass], 0x13);
			break;
	}

	//Clear mon icon sprite
	SafeFreeMonIconPalette(sDNavState->species);
	DestroyMonIconSprite(&gSprites[sDNavState->objIdSpecies]);

	//Clear black bars
	FieldEffectFreeGraphicsResources(&gSprites[sDNavState->objIdBlackBar[0]]);
	ResetSprite(&gSprites[sDNavState->objIdBlackBar[0]]);
	ResetSprite(&gSprites[sDNavState->objIdBlackBar[1]]);
	ResetSprite(&gSprites[sDNavState->objIdBlackBar[2]]);
	ResetSprite(&gSprites[sDNavState->objIdBlackBar[3]]);

	if (sDNavState->objIdSight < MAX_SPRITES)
	{
		FieldEffectFreeGraphicsResources(&gSprites[sDNavState->objIdSight]);
		ResetSprite(&gSprites[sDNavState->objIdSight]);
	}

	if (sDNavState->objIdAbility < MAX_SPRITES)
	{
		FieldEffectFreeGraphicsResources(&gSprites[sDNavState->objIdAbility]);
		ResetSprite(&gSprites[sDNavState->objIdAbility]);
	}

	if (sDNavState->objIdMove < MAX_SPRITES)
	{
		FieldEffectFreeGraphicsResources(&gSprites[sDNavState->objIdMove]);
		ResetSprite(&gSprites[sDNavState->objIdMove]);
	}

	if (sDNavState->objIdItem < MAX_SPRITES)
	{
		FieldEffectFreeGraphicsResources(&gSprites[sDNavState->objIdItem]);
		ResetSprite(&gSprites[sDNavState->objIdItem]);
	}

	if (sDNavState->objIdPotential[0] < MAX_SPRITES)
	{
		FieldEffectFreeGraphicsResources(&gSprites[sDNavState->objIdPotential[0]]);
		ResetSprite(&gSprites[sDNavState->objIdPotential[0]]);
	}

	if (sDNavState->objIdPotential[1] < MAX_SPRITES)
	{
		FieldEffectFreeGraphicsResources(&gSprites[sDNavState->objIdPotential[1]]);
		ResetSprite(&gSprites[sDNavState->objIdPotential[1]]);
	}

	if (sDNavState->objIdPotential[2] < MAX_SPRITES)
	{
		FieldEffectFreeGraphicsResources(&gSprites[sDNavState->objIdPotential[2]]);
		ResetSprite(&gSprites[sDNavState->objIdPotential[2]]);
	}

	//FreeSpriteTilesByTag(0x4736);
	FreeSpriteTilesByTag(0x61);
	FreeSpriteTilesByTag(0x2613);
	FreeSpriteTilesByTag(0x5424);
	FreeSpriteTilesByTag(0x8472);
	FreeSpriteTilesByTag(0x1EE7); //Font Sprite
	FreeSpriteTilesByTag(0xFDF1); //Black Bars
	FreeSpriteTilesByTag(0x3039);
	FreeSpritePaletteByTag(0x8472);
	FreeSpritePaletteByTag(0x3039);

	Free(sDNavState);
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
static void DexNavShowMessage(u8 id)
{
	switch(id)
	{
		case 0:
			gLoadPointer = gText_GotAway;
			break;
		case 1:
			gLoadPointer = gText_LostSignal;
			break;
		case 2:
			gLoadPointer = gText_GotAwayShouldSneak;
			break;
	}

	ScriptContext2_Enable();
	DismissMapNamePopup();
	ScriptContext1_SetupScript(SystemScript_DisplayDexnavMsg);
}


static void OutlinedFontDraw(u8 objId, u8 tileNum, u16 size)
{
	if (objId >= MAX_SPRITES)
		return;

	u8 tile = gSprites[objId].oam.tileNum + tileNum;
	u8* toWrite = (u8*)((tile * TILE_SIZE) + SPRITE_RAM);
	//u8* dst = (u8*)Malloc(size + TILE_SIZE);
	//u8* dst = &gEnemyParty[0];

	u8* originalDst, *dst;
	originalDst = dst = Calloc(size + TILE_SIZE);
	u8* strPtr = gStringVar4;

	u8 counter = 1;
	u16 index = 320;
	u16 prevIndex;

	//u8 element = *gStringVar4;

	u8 element = *strPtr;
	//u8 i = 0;

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
			};
			index = (symbolId + 9) * TILE_SIZE;
		}

		// TODO: Use macros here //

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
		//gStringVar4++;
		//element = *gStringVar4;
		//i++;
		strPtr++;
		element = *strPtr;
	}

	Memcpy((void*) toWrite, originalDst, size);
	Free(originalDst);
}


static void DexNavSightUpdate(u8 sight)
{
	u8 objId = sDNavState->objIdSight;
	if (objId < MAX_SPRITES)
	{
		// draw sight eye on first tile, takes up two tiles
		u8 tileid = gSprites[objId].oam.tileNum;
		u8* toWrite = (u8*)((tileid * 32) + (SPRITE_RAM));
		Memcpy(toWrite, &gInterfaceGfx_dexnavStarsTiles[((19 * 4) + (7 - (2 * sight))) * 32], 64);

		// draw the B button tile
		Memcpy(toWrite + 128, &gInterfaceGfx_dexnavStarsTiles[((19 * 4) + 2) *32], 64);

		// draw info text on the 5th tile
		//pchar back[] = _(" Back  ");
		StringCopy((u8*) gStringVar4, (u8*) &gText_DexNavBack[0]);
		OutlinedFontDraw(objId, 5, 32 * 8);
	}
}


static void DexNavIconsVisionUpdate(u8 proximity, u8 searchLevel)
{
	// The sight eye and species icon are always drawn in the HUD
	// 	At search level 2, first move is drawn
	// 	At search level 3, Ability and Held item are drawn
	// 	At search level 5, Potential is drawn

	// species and sight/info are always shown, regardless of proximity
	if (sDNavState->objIdSpecies < MAX_SPRITES)
		gSprites[sDNavState->objIdSpecies].invisible = FALSE;

	if (sDNavState->objIdSight < MAX_SPRITES)
		gSprites[sDNavState->objIdSight].invisible = FALSE;

	// hide everything at the start, and recalc what needs to be shown
	if (sDNavState->objIdMove < MAX_SPRITES)
		gSprites[sDNavState->objIdMove].invisible = TRUE;

	if (sDNavState->objIdAbility < MAX_SPRITES)
		gSprites[sDNavState->objIdAbility].invisible = TRUE;

	if (sDNavState->objIdItem < MAX_SPRITES)
		gSprites[sDNavState->objIdItem].invisible = TRUE;

	if (sDNavState->objIdPotential[0] < MAX_SPRITES)
		gSprites[sDNavState->objIdPotential[0]].invisible = TRUE;

	if (sDNavState->objIdPotential[1] < MAX_SPRITES)
		gSprites[sDNavState->objIdPotential[1]].invisible = TRUE;

	if (sDNavState->objIdPotential[2] < MAX_SPRITES)
		gSprites[sDNavState->objIdPotential[2]].invisible = TRUE;

	if (proximity < 3)
	{
		// at proximity 5, the sight should start to show and indicate sneaking is required
		DexNavSightUpdate(0);
		if (searchLevel > 1)
		{
			// show move, hide others
			if (sDNavState->objIdMove < MAX_SPRITES)
				gSprites[sDNavState->objIdMove].invisible = FALSE;
		}
		if (searchLevel > 2)
		{
			// show ability, move, hide others
			if (sDNavState->objIdAbility < MAX_SPRITES)
				gSprites[sDNavState->objIdAbility].invisible = FALSE;
			if (sDNavState->heldItem)
			{
				// toggle item view
				if (sDNavState->objIdItem < MAX_SPRITES)
					gSprites[sDNavState->objIdItem].invisible = FALSE;
			}
		}
		if (searchLevel > 4)
		{
			if (sDNavState->objIdPotential[0] < MAX_SPRITES)
				gSprites[sDNavState->objIdPotential[0]].invisible = FALSE;

			if (sDNavState->objIdPotential[1] < MAX_SPRITES)
				gSprites[sDNavState->objIdPotential[1]].invisible = FALSE;

			if (sDNavState->objIdPotential[2] < MAX_SPRITES)
				gSprites[sDNavState->objIdPotential[2]].invisible = FALSE;
		}
	}
	else if (proximity <= SNEAKING_PROXIMITY)
		DexNavSightUpdate(1); // Sneaking is required flag
	else
		DexNavSightUpdate(2); // Sneaking is not required
}

extern const u8 SystemScript_StartDexNavBattle[];
static void DexNavManageHUD(u8 taskId)
{
	//Check for out of range
	if (sDNavState->proximity > 20)
	{
		DestroyTask(taskId);
		DexNavFreeHUD();
		DexNavShowMessage(1);	//there is no reaction!\nThe signal was lost!
		return;
	}

	//Check for timeout.
	gTasks[taskId].data[1]++;
	if (gTasks[taskId].data[1] > DEXNAV_TIMEOUT * 60)
	{
		gCurrentDexNavChain = 0; //A Pokemon running like this resets the chain
		DestroyTask(taskId);
		DexNavFreeHUD();
		DexNavShowMessage(0);	//The pokemon got away!
		return;
	}

	if (sDNavState->proximity <= SNEAKING_PROXIMITY && TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_DASH | PLAYER_AVATAR_FLAG_BIKE)) //If player is close and running then the Pokemon should flee
	{
		gCurrentDexNavChain = 0; //A Pokemon running like this resets the chain
		DestroyTask(taskId);
		DexNavFreeHUD();
		DexNavShowMessage(2);	//The pokemon got away!\nTry moving more slowly.
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
	if ((sDNavState->environment == ENCOUNTER_TYPE_WATER || !IsMapTypeOutdoors(GetCurrentMapType()))
	&& sDNavState->proximity < 2
	&& sDNavState->movementTimes < 2)
	{
		switch(sDNavState->environment)
		{
			case ENCOUNTER_TYPE_LAND:
				FieldEffectStop(&gSprites[sDNavState->objIdShakingGrass], FLDEFF_CAVE_DUST); // 1a
				break;
			case ENCOUNTER_TYPE_WATER:
				FieldEffectStop(&gSprites[sDNavState->objIdShakingGrass], FLDEFF_SPLASHING_WATER);
				break;
			default:
				break;
		};

		while(!ShakingGrass(sDNavState->environment, 8, 8, 1));
		sDNavState->movementTimes += 1;
	}

	// check for encounter start
	if (sDNavState-> proximity < 1)
	{
		DexNavGetMon(sDNavState->species, sDNavState->potential, sDNavState->pokemonLevel,
					sDNavState->ability, sDNavState->moveId, sDNavState->searchLevel, gCurrentDexNavChain);
		DestroyTask(taskId);

		// increment the search level
		u16 dexNum = SpeciesToNationalPokedexNum(sDNavState->species);
		if (sSearchLevels[dexNum] < 255)
			sSearchLevels[dexNum] += 1;

		// Freeing only the state, objects and hblank cleared on battle start.
		Free(sDNavState);

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
	if (!(gTasks[taskId].data[0] == sDNavState->proximity))
	{
		DexNavIconsVisionUpdate(sDNavState->proximity, sDNavState->searchLevel);
		gTasks[taskId].data[0] = sDNavState->proximity;
	}
}


// ===================================== //
// ================ HUD ================ //
// ===================================== //
static u8 GetEncounterLevel(u16 species, u8 environment)
{
	const struct WildPokemonInfo* landMonsInfo = LoadProperMonsData(LAND_MONS_HEADER);
	const struct WildPokemonInfo* waterMonsInfo = LoadProperMonsData(WATER_MONS_HEADER);

	u8 min = 100;
	u8 max = 0;

	switch (environment)
	{
		case 0:	// grass
			if (landMonsInfo == NULL)
				return 22; //Hidden pokemon should only appear on walkable tiles or surf tiles

			for (u8 i = 0; i < NUM_LAND_MONS; ++i)
			{
				struct WildPokemon monData = landMonsInfo->wildPokemon[i];
				if (monData.species == species)
				{
					min = (min < monData.minLevel) ? min : monData.minLevel;
					max = (max > monData.maxLevel) ? max : monData.maxLevel;
				}
			}
			break;

		case 1:	//water
			if (waterMonsInfo == NULL)
				return 22; //Hidden pokemon should only appear on walkable tiles or surf tiles

			for (u8 i = 0; i < NUM_WATER_MONS; ++i)
			{
				struct WildPokemon monData = waterMonsInfo->wildPokemon[i];
				if (monData.species == species)
				{
					min = (min < monData.minLevel) ? min : monData.minLevel;
					max = (max > monData.maxLevel) ? max : monData.maxLevel;
				}
			}
			break;

		default:
			return 22;
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
	CreateWildMon(species, encounterLevel, sDNavState->selectedIndex / 2, TRUE);


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

extern const u8 gInterfaceGfx_DexnavBarTiles[];
static const struct CompressedSpriteSheet sBlackBarTiles = {gInterfaceGfx_DexnavBarTiles, (64 * 32) / 2, 0xFDF1};
static const struct SpriteTemplate sBlackBarTemplate =
{
	.tileTag = 0xFDF1,
	.paletteTag = 0x8472,
	.oam = &sBlackBarOAM,
	.anims = gDummySpriteAnimTable,
	.images = NULL,
	.affineAnims = gDummySpriteAffineAnimTable,
	.callback = SpriteCallbackDummy,
};

static void DexNavDrawBlackBars(u8 objidAddr[4])
{
	LoadSpritePalette(&HeldPal);
	LoadCompressedSpriteSheetUsingHeap(&sBlackBarTiles);
	objidAddr[0] = CreateSprite(&sBlackBarTemplate, ICONX + 16, ICONY - 2, 0x0);
	objidAddr[1] = CreateSprite(&sBlackBarTemplate, ICONX + 16 + 64, ICONY - 2, 0x0);
	objidAddr[2] = CreateSprite(&sBlackBarTemplate, ICONX + 16 + 64 + 64, ICONY - 2, 0x0);
	objidAddr[3] = CreateSprite(&sBlackBarTemplate, ICONX + 16 + 64 + 64 + 64, ICONY - 2, 0x0);
}

// create empty object of size 64x32 to draw icons on
static const struct CompressedSpriteSheet sightTiles = {(const u8*)(&gInterfaceGfx_emptyTiles), (64 * 32) / 2, 0x5424};
static const struct SpriteTemplate fontTempSight =
{
	.tileTag = 0x5424,
	.paletteTag = 0x8472,
	.oam = (struct OamData*) &FontOAM,
	.anims = (const union AnimCmd* const*) gDummySpriteAnimTable,
	.images = NULL,
	.affineAnims = (const union AffineAnimCmd* const*) gDummySpriteAffineAnimTable,
	.callback = SpriteCallbackDummy,
};

static void DexNavDrawSight(u8 sight_lvl, u8* objidAddr)
{
	LoadSpritePalette(&HeldPal);
	LoadCompressedSpriteSheetUsingHeap(&sightTiles);
	u8 objId = CreateSprite(&fontTempSight, ICONX + 192, ICONY + 0x12, 0x0);
	if (objId < MAX_SPRITES)
	{
		*objidAddr = objId;
		DexNavSightUpdate(sight_lvl);
	}
};

// create empty object of size 64x32 to draw font on
static const struct CompressedSpriteSheet FontSpriteAbility = {(const u8*)(&gInterfaceGfx_emptyTiles), (64 * 32) / 2, 0x1EE7};
static const struct SpriteTemplate FontTempAbility =
{
	.tileTag = 0x1EE7,
	.paletteTag = 0x8472,
	.oam = (struct OamData*) &FontOAM,
	.anims = (const union AnimCmd* const*)gDummySpriteAnimTable,
	.images = 0,
	.affineAnims = (const union AffineAnimCmd* const*) gDummySpriteAffineAnimTable,
	.callback = SpriteCallbackDummy,
};

static void DexNavDrawAbility(u8 ability, u8* objidAddr)
{
	LoadSpritePalette(&HeldPal);
	LoadCompressedSpriteSheetUsingHeap(&FontSpriteAbility);
	u8 objId = CreateSprite(&FontTempAbility, ICONX + 80, ICONY + 0x12, 0x0);
	if (objId < MAX_SPRITES)
	{
		*objidAddr = objId;

		//Ability name beside move name
		u8 len = sDNavState->moveNameLength;
		gSprites[objId].pos1.x += ((8 * (len/2)) + (4 * (len % 2)));

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

		//Write ability name on sprite
		OutlinedFontDraw(objId, 0, 32 * 8);
	}
}

	// create empty object of size 64x32 to draw font on
static const struct CompressedSpriteSheet FontSpriteMove = {(u8*) gInterfaceGfx_emptyTiles, (64 * 32) / 2, 0x4736};
static const struct SpriteTemplate FontTempMove =
{
	.tileTag = 0x4736,
	.paletteTag = 0x8472,
	.oam = (struct OamData*) &FontOAM,
	.anims = (const union AnimCmd* const*) gDummySpriteAnimTable,
	.images = 0,
	.affineAnims = (const union AffineAnimCmd* const*) gDummySpriteAffineAnimTable,
	.callback = SpriteCallbackDummy,
};

static void DexNavDrawMove(u16 move, u8 searchLevel, u8* objidAddr)
{
	LoadSpritePalette(&HeldPal);
	LoadCompressedSpriteSheetUsingHeap(&FontSpriteMove);
	u8 objId = CreateSprite(&FontTempMove, ICONX + 80, ICONY + 0x12, 0x0);
	if (objId < MAX_SPRITES)
	{
		*objidAddr = objId;

		// Copy move string from table using state id, add '/' character to the end of it
		u8* ptr = StringCopy(gStringVar4, gMoveNames[move]);
		u8 len = StringLength(gStringVar4);

		if (searchLevel > 2)
		{
			*ptr = 0xBA;
			len += 1;
		}

		// record length of move with slash for ability name to be placed beside it
		sDNavState->moveNameLength = len;

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

		// write name to object
		OutlinedFontDraw(objId, 0, 32 * 8);
	}
};

//19 tiles per row, stars are on the 4th row. 1 tile is 32 bytes. Hence 19 * 4 *32
struct SpriteSheet StarIconLit = {(const u8*)(&(gInterfaceGfx_dexnavStarsTiles[19 * 4 * 32])), 64, 0x61};
struct SpriteSheet StarIconOff = {(const u8*)(&(gInterfaceGfx_dexnavStarsTiles[((19 * 4) + 1)*32])), 64, 0x2613};

static const struct SpriteTemplate StarLitTemp =
{
	.tileTag = 0x61,
	.paletteTag = 0x8472,
	.oam = (struct OamData*) &HeldOAM,
	.anims = (const union AnimCmd* const*) gDummySpriteAnimTable,
	.images = 0,
	.affineAnims = (const union AffineAnimCmd* const*) gDummySpriteAffineAnimTable,
	.callback = SpriteCallbackDummy,
};

static const struct SpriteTemplate StarOffTemp =
{
	.tileTag = 0x2613,
	.paletteTag = 0x8472,
	.oam = &HeldOAM,
	.anims = gDummySpriteAnimTable,
	.images = NULL,
	.affineAnims = gDummySpriteAffineAnimTable,
	.callback = SpriteCallbackDummy,
};

static void DexNavDrawPotential(u8 potential, u8* objidAddr)
{
	// allocate both the lit and unlit star to VRAM
	LoadSpriteSheet(&StarIconLit);
	LoadSpriteSheet(&StarIconOff);
	LoadSpritePalette(&HeldPal);

	// create star objects and space them according to potential 0 - 3
	u8 objId;
	u8 i;
	for (i = 0; i < 3; ++i)
	{
		if (potential > i)
			objId = CreateSprite(&StarLitTemp, ICONX + 23 + (i * 8), ICONY + 0x5, 0x0);
		else
			objId = CreateSprite(&StarOffTemp, ICONX + 23 + (i * 8), ICONY + 0x5, 0x0);

		objidAddr[i] = objId;
	}
}


void DexNavHudDrawSpeciesIcon(u16 species, u8* objIdAddr)
{
	u32 pid = 0xFFFFFFFF;
	if (species == SPECIES_UNOWN)
		pid = GenerateUnownPersonality(sDNavState->unownLetter - 1);

	//Load which palette the species icon uses
	LoadMonIconPalette(species);

	//Create the icon
	u8 objId = CreateMonIcon(species, SpriteCB_PokeIcon, ICONX, ICONY, 0, pid, FALSE);
	*objIdAddr = objId;
}

static const struct SpriteSheet HeldIcon = {(const u8*) (0x0845A3AC), 64, 0x8472};
static const struct SpriteTemplate HeldTemp =
{
	.tileTag = 0x8472,
	.paletteTag = 0x8472,
	.oam = &HeldOAM,
	.anims = gDummySpriteAnimTable,
	.images = 0,
	.affineAnims = gDummySpriteAffineAnimTable,
	.callback = SpriteCallbackDummy,
};

void DexNavDrawHeldItem(u8* objidAddr)
{
	//Create SPRITE for held item icon
	LoadSpriteSheet(&HeldIcon);
	LoadSpritePalette(&HeldPal);
	u8 objId = CreateSprite(&HeldTemp, ICONX + 0x8, ICONY + 0xC, 0x0);
	*objidAddr = objId;
};


static void DexNavDrawIcons(void)
{
	u8 searchLevel = sDNavState->searchLevel;
	DexNavDrawBlackBars(sDNavState->objIdBlackBar);
	DexNavDrawSight(sDNavState->proximity, &sDNavState->objIdSight);
	DexNavDrawMove(sDNavState->moveId[0], searchLevel, &sDNavState->objIdMove);
	DexNavDrawHeldItem(&sDNavState->objIdItem);
	DexNavDrawAbility(sDNavState->ability, &sDNavState->objIdAbility);
	DexNavDrawPotential(sDNavState->potential, &sDNavState->objIdPotential[0]);
	DexNavHudDrawSpeciesIcon(sDNavState->species, &sDNavState->objIdSpecies);
};


void InitDexNavHUD(u16 species, u8 environment)
{
	sDNavState = Calloc(sizeof(struct DexnavHudData));
	// assign non-objects to struct
	sDNavState->species = species;

	if (IS_NEWER_UNOWN_LETTER(species))
	{
		sDNavState->unownLetter = species - SPECIES_UNOWN_B + 2; //Because B is 1
		sDNavState->species = species = SPECIES_UNOWN;
	}
	else //UNOWN A
		sDNavState->unownLetter = 1;

	sDNavState->environment = environment;
	u8 searchLevel = sSearchLevels[SpeciesToNationalPokedexNum(species)];
	sDNavState->searchLevel = searchLevel;
	sDNavState->pokemonLevel = DexNavGenerateMonLevel(species, gCurrentDexNavChain, environment);

	if (sDNavState->pokemonLevel < 1)
	{
		Free(sDNavState);
		MsgNormal(gText_CannotBeFound);
		return;
	}

	// draw shaking grass
	//extern u8 ShakingGrass(u8, u8, u8, bool8);
	if (!ShakingGrass(environment, 12, 12, 0))
	{
		Free(sDNavState);
		MsgNormal(gText_NotFoundNearby);
		return;
	}

	//Populate sDNavState objects
	DexNavGenerateMoveset(species, searchLevel, sDNavState->pokemonLevel, &sDNavState->moveId[0]);
	sDNavState->heldItem = DexNavGenerateHeldItem(species, searchLevel);
	sDNavState->ability = DexNavGenerateHiddenAbility(species, searchLevel);
	sDNavState->potential = DexNavGeneratePotential(searchLevel);
	DexNavProximityUpdate();

	//Draw icons
	DexNavDrawIcons();

	//Hide icons based on proximity and search level
	DexNavIconsVisionUpdate(sDNavState->proximity, searchLevel);

	//Enable Hblank interrupt
	//EnableInterrupts(2);
	//SetHBlankCallback(DexHUDHBlank);

	// task update HUD
	u8 taskId = CreateTask((TaskFunc)DexNavManageHUD, 0x1);
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


u8 StartMenuDexNavCallback(void)
{
	BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 16, 0x0000);
	SetMainCallback1(DexNavGuiHandler);
	gMain.state = 0;
	return TRUE;
};


// gfx clean
static void DexNavGuiSetup(void)
{
	Setup();
	FreeAllWindowBuffers();
	ChangeBgX(0, 0, 0);
	ChangeBgY(0, 0, 0);
	ChangeBgX(1, 0, 0);
	ChangeBgY(1, 0, 0);
	ResetBgsAndClearDma3BusyFlags(0);
	InitBgsFromTemplates(0, (struct BgTemplate *)&BgConfigDexNavGUI, 4);
	u32 set = 0;
	CpuFastSet((void*)&set, (void*)VRAM, CPUModeFS(0x10000, CPUFSSET));
	HideBg(1);
	HideBg(0);
	SetMainCallback2(DexNavGUICallback2);
	SetVBlankCallback(VblackCallbackSeq);
};

static void DexNavLoadPokeIcons(void)
{
	s16 x, y;
	u8 letter;
	u32 pid = 0xFFFFFFFF;
	u8 hiddenLandMons = sDNavState->numHiddenLandMons;
	u8 hiddenWaterMons = sDNavState->numHiddenWaterMons;

	LoadMonIconPalettes();

	for (u8 i = 0; i < NUM_LAND_MONS; ++i)
	{
		u16 species = sDNavState->grassSpecies[i];
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

		letter = sDNavState->unownFormsByDNavIndices[i];
		if (letter > 0)
			pid = GenerateUnownPersonality(sDNavState->unownFormsByDNavIndices[i] - 1);

		CreateMonIcon(species, SpriteCB_PokeIcon, x, y, 0, pid, 0);
	}

	for (u8 i = 0; i < NUM_WATER_MONS; ++i)
	{
		u16 species = sDNavState->waterSpecies[i];
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
			pid = GenerateUnownPersonality(letter - 1);

		CreateMonIcon(species, SpriteCB_PokeIcon, x, y, 0, pid, 0);
	}
}

extern struct OamData* sMonIconOamData;
static struct CompressedSpriteSheet IconTiles = {gInterfaceGfx_DexNavNoDataSymbolTiles, 32 * 32, ICON_GFX_TAG};

static void CreateNoDataIcon(s16 x, s16 y)
{
	LoadCompressedSpriteSheetUsingHeap(&IconTiles);
	struct SpriteTemplate IconTemplate =
	{
		.tileTag = ICON_GFX_TAG,
		.paletteTag = ICON_PAL_TAG,
		.oam = (struct OamData*) &IconOAM,
		.anims = (const union AnimCmd* const*) gDummySpriteAnimTable,
		.images = NULL,
		.affineAnims = (const union AffineAnimCmd* const*) gDummySpriteAffineAnimTable,
		.callback = (SpriteCallback) SpriteCallbackDummy,
	};

	CreateSprite(&IconTemplate, x, y, 0);
}

static void UpdateCursorPosition(void)
{
	// update cursor position
	if (sDNavState->selectedArr)
	{
		// water
		gSprites[sDNavState->cursorId].pos1.x = CursorPositions2[sDNavState->selectedIndex];
		gSprites[sDNavState->cursorId].pos1.y = CursorPositions2[sDNavState->selectedIndex + 1];
	}
	else
	{
		// grass
		gSprites[sDNavState->cursorId].pos1.x = CursorPositions1[sDNavState->selectedIndex];
		gSprites[sDNavState->cursorId].pos1.y = CursorPositions1[sDNavState->selectedIndex + 1];
	}
}


static const struct SpriteSheet CursorGFX = {(void*) gInterfaceGfx_selectionCursorTiles, 32 * 32, SELECTION_CURSOR_TAG};
static const struct SpritePalette CursorPal = {(void*) gInterfaceGfx_selectionCursorPal, SELECTION_CURSOR_TAG};
static const struct SpriteTemplate CursorTemp =
{
	.tileTag = SELECTION_CURSOR_TAG,
	.paletteTag = SELECTION_CURSOR_TAG,
	.oam = (struct OamData*) &CursorOAM,
	.anims = (const union AnimCmd* const*) gDummySpriteAnimTable,
	.images = (const struct SpriteFrameImage *) &CursorGFX,
	.affineAnims = (const union AffineAnimCmd* const*) gDummySpriteAffineAnimTable,
	.callback = SpriteCallbackDummy,
};


static void SpawnPointerArrow(void)
{
	// uncompressed
	LoadSpriteSheet(&CursorGFX);
	LoadSpritePalette(&CursorPal);
	sDNavState->cursorId = CreateSprite(&CursorTemp, 30, 48, 0);
	UpdateCursorPosition();
}


static void DexNavLoadAreaNames(void)
{
	const u8* landText;
	const u8* waterText;

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
	else
	#endif
	{
		landText = gText_DexNavLand;
		waterText = gText_DexNavWater;
	}

	WindowPrint(WINDOW_WATER, 1, 4, 6, &DexNav_WhiteText, 0, waterText);
	WindowPrint(WINDOW_LAND, 1, 5, 6, &DexNav_WhiteText, 0, landText);
}


static void DexNavLoadNames(u8 status)
{
	// clean boxes
	for (int i = 0; i < WINDOW_COUNT; ++i)
	{
		FillWindowPixelBuffer(i, 0);
	}

	//Print species name
	u16 species = sDNavState->selectedArr == ROW_WATER ? sDNavState->waterSpecies[sDNavState->selectedIndex >> 1] : sDNavState->grassSpecies[sDNavState->selectedIndex>>1];
	u16 dexNum = SpeciesToNationalPokedexNum(species);

	if (species != SPECIES_NONE)
		WindowPrint(WINDOW_SPECIES, 0, 0, 4, &DexNav_BlackText, 0, gSpeciesNames[species]);
	else
		WindowPrint(WINDOW_SPECIES, 0, 0, 4, &DexNav_BlackText, 0, gText_DexNav_NoInfo);

	//Print search level
	ConvertIntToDecimalStringN(gStringVar4, sSearchLevels[dexNum], 0, 4);

	if (species == SPECIES_NONE)
		WindowPrint(WINDOW_SEARCH_LEVEL, 0, 0, 4, &DexNav_BlackText, 0, gText_DexNav_NoInfo);
	else
		WindowPrint(WINDOW_SEARCH_LEVEL, 0, 0, 4, &DexNav_BlackText, 0, gStringVar4);

	//Print level bonus
	u8 searchLevelBonus = 0;
	if ((sSearchLevels[dexNum] >> 2) > 20)
		searchLevelBonus = 20;
	else
		searchLevelBonus = (sSearchLevels[dexNum] >> 2);

	ConvertIntToDecimalStringN(gStringVar4, searchLevelBonus, 0, 4);

	if (species == SPECIES_NONE)
		WindowPrint(WINDOW_LEVEL_BONUS, 0, 0, 4, &DexNav_BlackText, 0, gText_DexNav_NoInfo);
	else
		WindowPrint(WINDOW_LEVEL_BONUS, 0, 0, 4, &DexNav_BlackText, 0, gStringVar4);

	//Print hidden ability name
	if (GetSetPokedexFlag(dexNum, FLAG_GET_CAUGHT) || species == SPECIES_NONE)
	{
		if (gBaseStats[species].hiddenAbility != ABILITY_NONE) //Only display hidden ability is Pokemon has been caught
			WindowPrint(WINDOW_HIDDEN_ABILITY, 0, 0, 4, &DexNav_BlackText, 0, GetAbilityName(gBaseStats[species].hiddenAbility));
		else
			WindowPrint(WINDOW_HIDDEN_ABILITY, 0, 0, 4, &DexNav_BlackText, 0, gText_DexNav_NoInfo);
	}
	else
		WindowPrint(WINDOW_HIDDEN_ABILITY, 0, 0, 4, &DexNav_BlackText, 0, gText_DexNav_CaptureToSee);

	//Print status message bar
	switch(status)
	{
		case 0:
			WindowPrint(WINDOW_REPLY_TEXT, 1, 2, 8, &DexNav_WhiteText, 0, gText_DexNav_Invalid);
			break;
		case 1:
			WindowPrint(WINDOW_REPLY_TEXT, 1, 2, 8, &DexNav_WhiteText, 0, gText_DexNav_ChooseMon);
			break;
		case 2:
			WindowPrint(WINDOW_REPLY_TEXT, 1, 2, 8, &DexNav_WhiteText, 0, gText_DexNav_Locked);
			break;
		case 3:
			WindowPrint(WINDOW_REPLY_TEXT, 1, 2, 8, &DexNav_WhiteText, 0, gText_DexNav_NoDataForSlot);
			break;
	}

	//Load area names
	DexNavLoadAreaNames();

	//Load map name
	GetMapName(gStringVar4, GetCurrentRegionMapSectionId(), 0);
	WindowPrint(WINDOW_MAP_NAME, 1, 2, 0, &DexNav_WhiteText, 0, gStringVar4);

	// display committed gfx
	for (u8 i = 0; i < WINDOW_COUNT; ++i)
	{
		CopyWindowToVram(i, 3);
		PutWindowTilemap(i);
	}
}


static const struct CompressedSpriteSheet sCapturedAllPokemonSpriteSheet = {gInterfaceGfx_CapturedAllPokemonTiles, (8 * 8) / 2, 0xFDF2}; //Tag is from Mega Evo and not in use
static const struct SpriteTemplate sCapturedAllPokemonSymbolTemplate =
{
	.tileTag = 0xFDF2,
	.paletteTag = SELECTION_CURSOR_TAG,
	.oam = (struct OamData*) &sCapturedAllPokemonSymbolOAM,
	.anims = (const union AnimCmd* const*) gDummySpriteAnimTable,
	.images = NULL,
	.affineAnims = (const union AffineAnimCmd* const*) gDummySpriteAffineAnimTable,
	.callback = SpriteCallbackDummy,
};

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

static void DexNavLoadCapturedAllSymbol(void)
{
	LoadCompressedSpriteSheetUsingHeap(&sCapturedAllPokemonSpriteSheet);

	if (CapturedAllLandBasedPokemon())
		CreateSprite(&sCapturedAllPokemonSymbolTemplate,  154, 77, 0);

	if (CapturedAllWaterBasedPokemon())
		CreateSprite(&sCapturedAllPokemonSymbolTemplate,  140, 29, 0);
}

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

static void CallbackDexNavOW(void)
{
	ExecDexNavHUD();
}


static void DexNavGuiExitSearch(void)
{
	switch (gMain.state)
	{
		case 0:
			BeginNormalPaletteFade(~0, 0, 0x0, 0x10, 0);
			PlaySE(SE_POKENAV_SEARCHING);
			gMain.state += 1;
			break;
		case 1:
			if (!gPaletteFade->active)
			{
				Free(sDNavState->backBuffer);
				gMain.state += 1;
			}
			break;
		case 2:
			m4aMPlayVolumeControl(&gMPlayInfo_BGM, 0xFFFF, 256);
			SetMainCallback1(CallbackDexNavOW);
			SetMainCallback2(CB2_ReturnToFieldContinueScript);
			break;
	}
}


static void DexNavGuiExitNoSearch(void)
{
	switch (gMain.state)
	{
		case 0:
			BeginNormalPaletteFade(~0, 0, 0x0, 0x10, 0);
			gMain.state += 1;
			break;
		case 1:
			if (!gPaletteFade->active)
			{
				Free(sDNavState->backBuffer);
				Free(sDNavState);
				gMain.state += 1;
			}
			break;
		case 2:
			m4aMPlayVolumeControl(&gMPlayInfo_BGM, 0xFFFF, 256);
			SetMainCallback1(CB1_Overworld);
			SetMainCallback2(CB2_ReturnToFieldWithOpenMenu);
			break;
	}
}


static void DexNavPopulateEncounterList(void)
{
	// nop struct data
	Memset(sDNavState->grassSpecies, 0, sizeof(sDNavState->grassSpecies)
									  + sizeof(sDNavState->waterSpecies)
									  + sizeof(sDNavState->hiddenSpecies)
									  + sizeof(sDNavState->unownForms)
									  + sizeof(sDNavState->unownFormsByDNavIndices));

	// populate unique wild grass encounters
	u8 grassIndex = 0;
	u8 waterIndex = 0;
	u16 species;

	const struct WildPokemonInfo* landMonsInfo = LoadProperMonsData(LAND_MONS_HEADER);
	const struct WildPokemonInfo* waterMonsInfo = LoadProperMonsData(WATER_MONS_HEADER);

	sDNavState->hiddenSpecies[0] = SPECIES_TABLES_TERMIN;

	if (landMonsInfo != NULL)
	{
		for (int i = 0; i < NUM_LAND_MONS; ++i)
		{
			species = landMonsInfo->wildPokemon[i].species;
			if (species != SPECIES_NONE && !SpeciesInArray(species, NUM_LAND_MONS, PickUnownLetter(species, i)))
			{
				sDNavState->grassSpecies[grassIndex++] = landMonsInfo->wildPokemon[i].species;

				if (InTanobyRuins())
				{
					sDNavState->unownForms[i] = PickUnownLetter(species, i);
					sDNavState->unownFormsByDNavIndices[grassIndex - 1] = PickUnownLetter(species, i);
				}
			}
		}
	}

	sDNavState->hiddenSpecies[0] = SPECIES_TABLES_TERMIN;

	if (waterMonsInfo != NULL)
	{
		for (int i = 0; i < NUM_WATER_MONS; ++i)
		{
			species = waterMonsInfo->wildPokemon[i].species;
			if (species != SPECIES_NONE && !SpeciesInArray(species, NUM_WATER_MONS, PickUnownLetter(species, i)))
			{
				sDNavState->waterSpecies[waterIndex++] = waterMonsInfo->wildPokemon[i].species;
			}
		}
	}

	#ifdef NATIONAL_DEX_UNOWN
	if (InTanobyRuins() && !GetSetPokedexFlag(NATIONAL_DEX_UNOWN, FLAG_GET_SEEN))
	{ //This is so the right amount of ? appear for Unown in the different chambers
		u16 unowns[NUM_LAND_MONS + 1];
		unowns[0] = SPECIES_TABLES_TERMIN;

		sDNavState->numHiddenLandMons = 0;
		for (int i = 0; i < NUM_LAND_MONS; ++i)
		{
			u8 letter = PickUnownLetter(SPECIES_UNOWN, i);
			if (!CheckTableForSpecies(letter, unowns)) //Table with Unown letters treated like a species table
			{
				unowns[sDNavState->numHiddenLandMons++] = letter;
				unowns[sDNavState->numHiddenLandMons] = SPECIES_TABLES_TERMIN; //Shift end down 1
			}
		}
	}
	#endif

	sDNavState->numGrassMons = grassIndex;
	sDNavState->numWaterMons = waterIndex;
};

#define LAND_ROW_LENGTH (6 * 2)
#define LAND_FIRST_ROW_LAST_INDEX (5 * 2)
#define LAND_SECOND_ROW_LAST_INDEX (LAND_FIRST_ROW_LAST_INDEX + LAND_ROW_LENGTH)
#define LAND_SECOND_ROW_FIRST_INDEX (6 * 2)
#define WATER_ROW_LAST_INDEX (4 * 2)
#define ROW_MON_LENGTH 2

static void DexNavGuiHandler(void)
{
	switch(gMain.state)
	{
		case 0:
			if (!gPaletteFade->active)
			{
				DexNavGuiSetup();
				SetMainCallback1(DexNavGuiHandler);
				// allocate dexnav struct
				sDNavState = Calloc(sizeof(struct DexnavHudData));
				gMain.state += 1;
			}
			break;
		case 1: ;
			//Load BG assets
			void* DexNav_gbackBuffer = Malloc(0x800);
			sDNavState->backBuffer = DexNav_gbackBuffer;
			LoadPalette(&DexNavTextPal, 15 * 16, 32);
			const u8* palette;

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
			else if (IsCurrentAreaSwamp())
				palette = gInterfaceGfx_DexNavGuiSwampPal;
			else
				palette = gInterfaceGfx_dexnavGuiPal;

			LoadCompressedPalette(palette, 0, 32);

			LZ77UnCompWram(gInterfaceGfx_dexnavGuiMap, DexNav_gbackBuffer);
			LZ77UnCompVram(gInterfaceGfx_dexnavGuiTiles, (void*) 0x06000000);
			SetBgTilemapBuffer(1, DexNav_gbackBuffer);
			ScheduleBgCopyTilemapToVram(1);
			ScheduleBgCopyTilemapToVram(0);
			gMain.state += 1;
			break;

		case 2:
			InitWindows(sDexNavWindows);
			DexNavPopulateEncounterList();
			DexNavLoadNames(1);
			gMain.state += 1;
			break;
		case 3:
		   // REG_DISPCNT = 0x7F60;
			//REG_WININ = WININ_BUILD(WIN_BG0 | WIN_BG1 | WIN_BG2 | WIN_BG3 | WIN_OBJ, WIN_BG0 |
			 //			   WIN_BG1 | WIN_BG2 | WIN_BG3 | WIN_OBJ);
			//WRITE_REG_BLDCNT(0x401E);
			//REG_BLDCNT = BLDALPHA_BUILD(BLDCNT_BG1_SRC | BLDCNT_BG2_SRC | BLDCNT_BG3_SRC | BLDCNT_SPRITES_SRC, 0);

			BeginNormalPaletteFade(0xFFFFFFFF, 0, 16, 0, 0x0000);
			ShowBg(0);
			ShowBg(1);
			DexNavLoadPokeIcons();
			SpawnPointerArrow();
			DexNavLoadCapturedAllSymbol();
			sDNavState->selectedArr = 0;
			sDNavState->selectedIndex = 0;
			gMain.state += 1;
			break;

		case 4:
			if (!gPaletteFade->active)
			{
				switch (gMain.newAndRepeatedKeys &
					(A_BUTTON | B_BUTTON | DPAD_DOWN | DPAD_UP | DPAD_LEFT | DPAD_RIGHT | R_BUTTON))
				{
					case A_BUTTON: ;
						// check selection is valid. Play sound if invalid
						u16 species = sDNavState->selectedArr == ROW_WATER ? sDNavState->waterSpecies[sDNavState->selectedIndex / 2] : sDNavState->grassSpecies[sDNavState->selectedIndex / 2];
						// if species is MISSINGNO then error
						if (species != SPECIES_NONE)
						{
							if (species == SPECIES_UNOWN)
							{
								u8 letter = sDNavState->unownFormsByDNavIndices[sDNavState->selectedIndex / 2] - 1;
								if (letter > 0)
									species = SPECIES_UNOWN_B + letter - 1;
							}

							//Species was valid, save and enter OW HUD mode
							Var8000 = species;
							Var8001 = sDNavState->selectedArr;
							SetMainCallback1(DexNavGuiExitSearch);
							gMain.state = 0;
							return;
						}
						else
						{
							PrintDexNavError();
						}
						break;

					case B_BUTTON:
						// exit to start menu
						SetMainCallback1(DexNavGuiExitNoSearch);
						gMain.state = 0;
						return;
					case DPAD_DOWN:
						if (sDNavState->selectedArr == ROW_LAND)
						{
							if (sDNavState->selectedIndex >= LAND_SECOND_ROW_FIRST_INDEX)
							{
								sDNavState->selectedIndex = MathMin(sDNavState->selectedIndex - LAND_ROW_LENGTH, WATER_ROW_LAST_INDEX);
								sDNavState->selectedArr = ROW_WATER;
							}
							else
							{
								sDNavState->selectedIndex += LAND_ROW_LENGTH; //Move to second row of land
							}
						}
						else //ROW_WATER
						{
							sDNavState->selectedArr = ROW_LAND; //Keep index the same
						}
						DexNavLoadNames(1);
						PlaySE(SE_POKENAV_ONE_BEEP);
						break;

					case DPAD_UP:
						if (sDNavState->selectedArr == ROW_LAND)
						{
							if (sDNavState->selectedIndex >= LAND_SECOND_ROW_FIRST_INDEX)
							{
								sDNavState->selectedIndex -= LAND_ROW_LENGTH; //Move to first row of land
							}
							else
							{
								sDNavState->selectedIndex = MathMin(sDNavState->selectedIndex, WATER_ROW_LAST_INDEX);
								sDNavState->selectedArr = ROW_WATER;
							}
						}
						else //ROW_WATER
						{
							sDNavState->selectedArr = ROW_LAND;
							sDNavState->selectedIndex += LAND_ROW_LENGTH; //So be on the second row
						}
						DexNavLoadNames(1);
						PlaySE(SE_POKENAV_ONE_BEEP);
						break;

					case DPAD_LEFT:
						// wrap cursor around
						if (sDNavState->selectedArr == ROW_WATER)
							sDNavState->selectedIndex = (sDNavState->selectedIndex == 0) ? WATER_ROW_LAST_INDEX : sDNavState->selectedIndex - ROW_MON_LENGTH;
						else //ROW_LAND
						{
							if (sDNavState->selectedIndex == 0 || sDNavState->selectedIndex == LAND_SECOND_ROW_FIRST_INDEX)
								sDNavState->selectedIndex += LAND_FIRST_ROW_LAST_INDEX; //Wrap around
							else
								sDNavState->selectedIndex -= ROW_MON_LENGTH;
						}
						DexNavLoadNames(1);
						PlaySE(SE_POKENAV_ONE_BEEP);
						break;
					case DPAD_RIGHT:
						if (sDNavState->selectedArr == ROW_WATER)
							sDNavState->selectedIndex = (sDNavState->selectedIndex == WATER_ROW_LAST_INDEX) ? 0 : sDNavState->selectedIndex + ROW_MON_LENGTH;
						else //ROW_LAND
						{
							if (sDNavState->selectedIndex == LAND_FIRST_ROW_LAST_INDEX || sDNavState->selectedIndex == LAND_SECOND_ROW_LAST_INDEX)
								sDNavState->selectedIndex -= LAND_FIRST_ROW_LAST_INDEX; //Wrap around
							else
								sDNavState->selectedIndex += ROW_MON_LENGTH;
						}
						DexNavLoadNames(1);
						PlaySE(SE_POKENAV_ONE_BEEP);
						break;
					case R_BUTTON:
					{
						// check selection is valid. Play sound if invalid
						u16 species = sDNavState->selectedArr ? sDNavState->waterSpecies[sDNavState->selectedIndex / 2] : sDNavState->grassSpecies[sDNavState->selectedIndex / 2];
						// if species is MISSINGNO then error
						if (species != SPECIES_NONE)
						{
							if (species == SPECIES_UNOWN)
							{
								u8 letter = sDNavState->unownFormsByDNavIndices[sDNavState->selectedIndex / 2] - 1;
								if (letter > 0)
									species = SPECIES_UNOWN_B + letter - 1;
							}

							// species was valid
							DexNavLoadNames(2);
							PlaySE(SE_POKENAV_SEARCHING);
							// create value to store in a var
							u16 varStore = (sDNavState->selectedArr << 15) | species;
							VarSet(VAR_DEXNAV, varStore);

							#ifdef VAR_R_BUTTON_MODE
							VarSet(VAR_R_BUTTON_MODE, OPTIONS_R_BUTTON_MODE_DEXNAV);
							#endif
						}
						else
						{
							// beep and update
							DexNavLoadNames(0);
							PlaySE(SE_ERROR);
						}
						break;
					}
					default:
						return;
				}
				UpdateCursorPosition();
			}
			break;
	}
}

static void PrintDexNavError(void)
{
	if (sDNavState->selectedArr == ROW_WATER
	&&  sDNavState->selectedIndex >> 1 >= sDNavState->numWaterMons + sDNavState->numHiddenWaterMons)
	{
		//Print Pokemon can't be searched yet
		DexNavLoadNames(3);
		PlaySE(SE_ERROR);
	}
	else if (sDNavState->selectedArr == ROW_LAND
	&&  sDNavState->selectedIndex >> 1 >= sDNavState->numGrassMons + sDNavState->numHiddenLandMons)
	{
		//Print Pokemon can't be searched yet
		DexNavLoadNames(3);
		PlaySE(SE_ERROR);
	}
	else //Selected unidentified Pokemon
	{
		//Print invalid Pokemon
		DexNavLoadNames(0);
		PlaySE(SE_ERROR);
	}
}

// =================================== //
// ===== Overworld Field Effects ===== //
// =================================== //
static const struct SpriteSheet sCaveGfx[4] =
{
	{
		.data = (const u8*)&gInterfaceGfx_caveSmokeTiles[128 * 0],
		.size = 0x80,
		.tag = 0xFFFF,
	},
	{
		.data = (const u8*)&gInterfaceGfx_caveSmokeTiles[128 * 1],
		.size = 0x80,
		.tag = 0xFFFF,
	},
	{
		.data = (const u8*)&gInterfaceGfx_caveSmokeTiles[128 * 2],
		.size = 0x80,
		.tag = 0xFFFF,
	},
	{
		.data = (const u8*)&gInterfaceGfx_caveSmokeTiles[128 * 3],
		.size = 0x80,
		.tag = 0xFFFF,
	},
};

static const struct SpriteTemplate ObjtCave =
{
	.tileTag = 0xFFFF,
	.paletteTag = SMOKE_TAG,
	.oam = (struct OamData*) 0x83A36F0,
	.anims = (const union AnimCmd* const*) 0x83A5B70,
	.images = (const struct SpriteFrameImage *) sCaveGfx,
	.affineAnims = gDummySpriteAffineAnimTable,
	.callback = (SpriteCallback) 0x80DCD1D,
};


static const struct SpritePalette sCaveSmokePalTemplate =
{
	.data = gInterfaceGfx_caveSmokePal,
	.tag = SMOKE_TAG,
};

static void FieldEff_CaveDust(void)
{
	LoadSpritePalette(&sCaveSmokePalTemplate);
	LoadPalette(gInterfaceGfx_caveSmokePal, 29 * 16, 32);
	LogCoordsCameraRelative(&gFieldEffectArguments->effectPos.x, &gFieldEffectArguments->effectPos.y, 8, 8);

	u8 objId = CreateSpriteAtEnd(&ObjtCave, gFieldEffectArguments->effectPos.x, gFieldEffectArguments->effectPos.y, 0xFF);
	if (objId != MAX_SPRITES)
	{
		gSprites[objId].coordOffsetEnabled = 1;
		gSprites[objId].data[0] = 22;
	}
}


const union AnimCmd sFieldEffectObjectImageAnim_Sparkles[] =
{
	ANIMCMD_FRAME(0, 8),
	ANIMCMD_FRAME(1, 8),
	ANIMCMD_FRAME(2, 8),
	ANIMCMD_FRAME(3, 8),
	ANIMCMD_FRAME(4, 8),
	ANIMCMD_FRAME(5, 8),
	ANIMCMD_JUMP(0),
};

const union AnimCmd* const sFieldEffectObjectImageAnimTable_Sparkles[] =
{
	sFieldEffectObjectImageAnim_Sparkles,
};

static const struct SpriteFrameImage sFieldEffectObjectPicTable_Sparkles[] =
{
	overworld_frame(gInterfaceGfx_SparklesTiles, 2, 2, 0),
	overworld_frame(gInterfaceGfx_SparklesTiles, 2, 2, 1),
	overworld_frame(gInterfaceGfx_SparklesTiles, 2, 2, 2),
	overworld_frame(gInterfaceGfx_SparklesTiles, 2, 2, 3),
	overworld_frame(gInterfaceGfx_SparklesTiles, 2, 2, 4),
	overworld_frame(gInterfaceGfx_SparklesTiles, 2, 2, 5),
};

static const struct SpriteTemplate sSpriteTemplateSparkles =
{
	.tileTag = 0xFFFF,
	.paletteTag = SMOKE_TAG,
	.oam = (struct OamData*) 0x83A36F0,
	.anims = sFieldEffectObjectImageAnimTable_Sparkles,
	.images = sFieldEffectObjectPicTable_Sparkles,
	.affineAnims = gDummySpriteAffineAnimTable,
	.callback = (SpriteCallback) 0x80DCD1D,
};

static const struct SpritePalette sSparklesPalTemplate =
{
	.data = gInterfaceGfx_SparklesPal,
	.tag = SMOKE_TAG,
};

static void FieldEff_Sparkles(void)
{
	LoadSpritePalette(&sSparklesPalTemplate);
	LoadPalette(gInterfaceGfx_SparklesPal, 29 * 16, 32);
	LogCoordsCameraRelative(&gFieldEffectArguments->effectPos.x, &gFieldEffectArguments->effectPos.y, 8, 8);

	u8 objId = CreateSpriteAtEnd(&sSpriteTemplateSparkles, gFieldEffectArguments->effectPos.x, gFieldEffectArguments->effectPos.y, 0xFF);
	if (objId != MAX_SPRITES)
	{
		gSprites[objId].coordOffsetEnabled = 1;
		gSprites[objId].data[0] = 22;
	}
}


static const struct SpriteFrameImage sFieldEffectObjectPicTable_LavaBubbles[] =
{
	overworld_frame(gInterfaceGfx_LavaBubblesTiles, 2, 2, 0),
	overworld_frame(gInterfaceGfx_LavaBubblesTiles, 2, 2, 1),
	overworld_frame(gInterfaceGfx_LavaBubblesTiles, 2, 2, 2),
	overworld_frame(gInterfaceGfx_LavaBubblesTiles, 2, 2, 3),
};

static const struct SpriteTemplate sSpriteTemplateLavaBubbles =
{
	.tileTag = 0xFFFF,
	.paletteTag = SMOKE_TAG,
	.oam = (struct OamData*) 0x83A36F0,
	.anims = (const union AnimCmd* const*) 0x83A5B70,
	.images = (const struct SpriteFrameImage *) sFieldEffectObjectPicTable_LavaBubbles,
	.affineAnims = gDummySpriteAffineAnimTable,
	.callback = (SpriteCallback) 0x80DCD1D,
};

static const struct SpritePalette sLavaBubblesPalTemplate =
{
	.data = gInterfaceGfx_LavaBubblesPal,
	.tag = SMOKE_TAG,
};

static void FieldEff_LavaBubbles(void)
{
	LoadSpritePalette(&sLavaBubblesPalTemplate);
	LoadPalette(gInterfaceGfx_LavaBubblesPal, 29 * 16, 32);
	LogCoordsCameraRelative(&gFieldEffectArguments->effectPos.x, &gFieldEffectArguments->effectPos.y, 8, 8);

	u8 objId = CreateSpriteAtEnd(&sSpriteTemplateLavaBubbles, gFieldEffectArguments->effectPos.x, gFieldEffectArguments->effectPos.y, 0xFF);
	if (objId != MAX_SPRITES)
	{
		gSprites[objId].coordOffsetEnabled = 1;
		gSprites[objId].data[0] = 22;
	}
}

const struct FieldEffectScript FieldEffectScript_CaveDust =
{
	FLDEFF_CALLASM, FieldEff_CaveDust,
	FLDEFF_END,
};

const struct FieldEffectScript FieldEffectScript_Sparkles =
{
	FLDEFF_CALLASM, FieldEff_Sparkles,
	FLDEFF_END,
};

const struct FieldEffectScript FieldEffectScript_LavaBubbles =
{
	FLDEFF_CALLASM, FieldEff_LavaBubbles,
	FLDEFF_END,
};

// ========================================== //
// ======== Check Select in Overworld ======= //
// ========================================== //
/*
u8 SelectHookRestore(void)
{
	ScriptContext1_SetupScript((void*) 0x81A77A0); // script that lets you know you can register item via select
	return 0;
};
*/

/*Replaced with R-Button
u8 CheckRegisteredSelect(void)
{
	u16 varVal = VarGet(VAR_DEXNAV);
	if (varVal)
	{
		if ()
			return 0;	// don't redo the search...
		InitDexNavHUD(varVal & 0x7FFF, varVal >> 15);
		return 0;
	}
	return 1;
};
*/

bool8 IsDexNavHudActive(void)
{
	return FuncIsActiveTask(DexNavManageHUD);
}
