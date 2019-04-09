#include "defines.h"
#include "../include/bg.h"
#include "../include/new/dexnav.h"
#include "../include/constants/moves.h"
#include "../include/constants/species.h"
#include "../include/constants/items.h"
#include "../include/new/build_pokemon.h"
#include "../include/songs.h"
#include "../include/wild_encounter.h"
#include "../include/palette.h"
#include "../include/gba/io_reg.h"
/*
Simplified DexNav System
	credits to FBI: https://github.com/EternalCode/Dexnav
*/


extern void CreateWildMon(u16 species, u8 level, u8 monHeaderIndex, bool8 purgeParty);
extern void DoStandardWildBattle(void);
extern void DexNavGuiHandler(void);
extern u8 ExecDexNav(void);
extern void TransferPlttBuffer(void);

extern u8 GetPlayerDistance(s16 x, s16 y);

extern const struct WildPokemonHeader* GetCurrentMapWildMonHeaderId(void);

extern u8 gMoveNames[][MOVE_NAME_LENGTH + 1];

extern u8 gText_DexNavText[];
extern u8 gText_PokeTools[];
extern u8 gText_GotAway[];
extern u8 gText_LostSignal[];
extern u8 gText_CannotBeFound[];
extern u8 gText_NotFoundNearby[];
extern u8 gText_DexNavBack[];
extern u8 gText_DexNav_NoInfo[];
extern u8 gText_DexNav_ChooseMon[];
extern u8 gText_DexNav_Invalid[];
extern u8 gText_DexNav_Locked[];

static struct DexnavHudData** DNavState = (struct DexnavHudData**)(DEXNAV_SAVERAM + MAX_NUM_POKEMON);

extern void DexNavPopulateEncounterList();
extern void ExecDexNavHUD(void);


void DestroyTaskCompletedTextbox(u8 tId)
{
    if (!gBoxStatusAndType) {
        TextboxClose();
        DestroyTask(tId);
        ScriptEnvDisable();
    }
};

void MsgNormal(const u8* str)
{
   TextboxFdecodeAutoAndCreateTask(str);
   *gBoxStatusAndType = 1;
   CreateTask((TaskFunc)DestroyTaskCompletedTextbox, 0x1);
   ScriptContext2_Enable();
   return;
};


u16 RandRange(u16 min, u16 max)
{
    if (min == max)
		return min;
    return (Random() % (max - min)) + min;
};


void VblackCallbackSeq(void)
{
	LoadOAM();
	ProcessSpriteCopyRequests();
	TransferPlttBuffer();
};


void PokeNavCallback2(void)
{
	SyncTilemaps();
	ProcessSpriteCopyRequests();
	TransferPlttBuffer();
};


void ClearHandlers(void)
{
	SetVBlankCallback((void*) 0);
	SetHBlankCallback((void*) 0);
	SetCallback1((void*) 0);
	SetCallback2((void*) 0);
};

void DexNavGUICallback2(void)
{
    BuildOAMBuffer();
    AnimateSprites();
    UpdatePaletteFade();
    RunTasks();
    SyncTilemaps();
    // merge textbox and text tile maps
    RemoBoxesUploadTilesets();
};


void ResetPalSettings(void)
{
	ResetPaletteFade();
	FadeBgPalAndFillBlack();
	FreeAllSpritePalettes();
	PalTagsStart = 0;
}


void ResetBgSettings(void)
{
	CleanupOverworldWindowsAndTilemaps();
	ResetBgsAndClearDma3BusyFlags(0);
	BgIdModOffsetX(0, 0, 0);
    BgIdModOffsetY(0, 0, 0);
	BgIdModOffsetX(1, 0, 0);
    BgIdModOffsetY(1, 0, 0);
	BgIdModOffsetX(2, 0, 0);
    BgIdModOffsetY(2, 0, 0);
	BgIdModOffsetX(3, 0, 0);
    BgIdModOffsetY(3, 0, 0);
};


void ResetBoxes(void)
{
	DeactivateAllTextPrinters();
	FreeAllWindowBuffers();
};


void Setup(void)
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
};


bool8 SpeciesInArray(u16 species, u8 indexCount)
{
    // disallow species not seen
    if (!GetIndexFromDexFlag(species, FLAG_GET_SEEN))
        return TRUE;
    for (u8 i = 0; i < indexCount; ++i)
	{
        if (indexCount == 12)
		{
            if ((*DNavState)->grassSpecies[i] == species)
                return TRUE;
        } 
		else 
		{
            if ((*DNavState)->waterSpecies[i] == species)
                return TRUE;
        }
    }
    return FALSE;
};


// ===================================== //
// ===== Dex Nav Pokemon Generator ===== //
// ===================================== // 


void DexNavGetMon(u16 species, u8 potential, u8 level, u8 ability, u16* moves)
{
    //struct Pokemon* pkmn = party_opponent;
	pokemon_t* pkmn = &gEnemyParty[0];

    // clear canvas
    //Memset((void*)gEnemyParty, 0, 100);
	PokemonSlotPurge(pkmn);
	
    //CreateBoxMon(&gEnemyParty[0]->base, species, level, 0x0, 0, 0, 0, saveblock2->trainerid);
	CreateBoxMon(pkmn, species, level, 0, 0, 0, 0, gSaveBlock2->playerTrainerId);

    // set IVs randomly
    u8 i;
    u8 currIv = MON_DATA_HP_IV;
    for (i = 0; i < 6; ++i)
	{
        u8 valIv = (Random() & 0x1F);
        SetMonData2(pkmn, currIv, (void*) &valIv);
        currIv++;
    }

    // pick potential ivs to set to 31
	u8 iv[3];
	iv[0] = Random() % 6;
	iv[1] = Random() % 6;
	iv[2] = Random() % 6;
	if ((iv[0] != iv[1]) && (iv[0] != iv[2]) && (iv[1] != iv[2]))
	{
		u8 perfectIv = 0x1F;
		if (potential > 2)
			SetMonData2(pkmn, MON_DATA_HP_IV + iv[2], (void*) &perfectIv);
		else if (potential > 1)
			SetMonData2(pkmn, MON_DATA_HP_IV + iv[1], (void*) &perfectIv);
		else if (potential)
			SetMonData2(pkmn, MON_DATA_HP_IV + iv[0], (void*) &perfectIv);
	}

    // set ability
	u8 onBit;
	if (gBaseStats[species].hiddenAbility == ability)
		onBit = 1;
	else
		onBit = 0;
    //u8 onBit = gBaseStats[species].ability2 ? 1 : 0;
    SetMonData2(pkmn, MON_DATA_ALT_ABILITY, (void*) &onBit);

    // set moves
    SetMonData2(pkmn, MON_DATA_MOVE1, &moves);
    SetMonData2(pkmn, MON_DATA_MOVE2, &moves + 1);
    SetMonData2(pkmn, MON_DATA_MOVE3, &moves + 2);
    SetMonData2(pkmn, MON_DATA_MOVE4, &moves + 3);

    // set PP of moves
    SetMonData2(pkmn, MON_DATA_PP1, &gBattleMoves[moves[0]].pp);
    SetMonData2(pkmn, MON_DATA_PP2, &gBattleMoves[moves[1]].pp);
    SetMonData2(pkmn, MON_DATA_PP3, &gBattleMoves[moves[2]].pp);
    SetMonData2(pkmn, MON_DATA_PP4, &gBattleMoves[moves[3]].pp);

    // pokerus
    i = 0xFF;
    SetMonData2(pkmn, MON_DATA_POKERUS, &i);
    CalculateMonStats(pkmn);
};



// ===== Pokemon Field Tile ===== //
u8 GetPlayerDistance(s16 x, s16 y)
{
    u16 deltaX = ABS(x - (gSaveBlock1->pos.x + 7));
    u16 deltaY = ABS(y - (gSaveBlock1->pos.y + 7));
    return deltaX + deltaY;
}



u8 PickTileScreen(u8 targetBehaviour, u8 areaX, u8 areaY, s16 *xBuff, s16 *yBuff, u8 smallScan)
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
            u8 blockProperties = MetatileBehavior_GetLowerBytes(tileBehaviour, 4);

            // check NPCs on tile
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

            // tile must be target behaviour (wild tile) and must be passable
            if ((targetBehaviour == blockProperties))
			{// ) {
                 // caves and water need to have their encounter values scaled higher
                u8 weight = 0;
                if (targetBehaviour == 2)
				{
                    // water
                    u8 scale = 320 - (smallScan * 200) - (GetPlayerDistance(topX, topY) / 2);
                    weight = (((Random() % scale) <= 1) && (CurrMapHeightMismatch(gEventObjects[gPlayerAvatar->spriteId].elevation, topX, topY))) ;
                } 
				else if (!CheckOpenSky(gMapHeader.mapType))
				{
                    // cave basically needs another check to see if the tile is passable
                    u8 scale = 440 - (smallScan * 200) - (GetPlayerDistance(topX, topY) / 2)  - (2 * (topX + topY));
                    Var8002 = scale;
                    weight = ((Random() % scale) < 1) && (!MetatileBehavior_GetLowerBytes(tileBehaviour, 6));

                } 
				else 
				{
                    // grass
                    u8 scale = 100 - (GetPlayerDistance(topX, topY) * 2);
                    weight = ((Random() % scale) <= 5);
                }

                if (weight) 
				{
                    *xBuff = topX;
                    *yBuff = topY;
                    return 1;
                }
            }
            topX += 1;
        }
        topY += 1;
        topX = gSaveBlock1->pos.x - SCANSTART_X + (smallScan * 5);
    }
    return 0;
};


u8 DexNavPickTile(u8 environment, u8 xSize, u8 ySize, bool8 smallScan)
{
    // Pick a specific tile based on environment
    u8 targetBehaviour = 0;
    switch (environment)
	{
        case 0:
        {
            targetBehaviour = 1; // grass
            break;
        }
        default:
        {
            targetBehaviour = 2; // water
            break;
        }
    };
    // tile pick chance is 5% + 1/5 search level
    
	//u8 searchLevel = (*DNavState)->searchLevel / 5;
    //u8 chance = searchLevel + 1;
    return PickTileScreen(targetBehaviour, xSize, ySize, &((*DNavState)->tileX), &((*DNavState)->tileY), smallScan);
};



u8 ShakingGrass(u8 environment, u8 xSize, u8 ySize, bool8 smallScan)
{
    if (DexNavPickTile(environment, xSize, ySize, smallScan))
	{

        gFieldEffectArguments->effectPos.x = (*DNavState)->tileX;
        gFieldEffectArguments->effectPos.y = (*DNavState)->tileY;
        gFieldEffectArguments->priority = 0xFF; // height.
        switch (environment)
		{
            case 0:
                {
					if (!CheckOpenSky(gMapHeader.mapType))
						ExecOE(OEI_CAVE);
					else
						ExecOE(OEI_GRASS);
					break;
                }
            case 1:
                ExecOE(OEI_WATER);
                break;
            default:
                ExecOE(0x31);
                break;
        };//ExecOE(*d); //16 water, 13 grass

        // get objid of shaking grass
        u8 i;
        for (i = 0; i < 64; ++i)
		{
            if (gSprites[i].callback == (SpriteCallback) 0x080DCD1D)
			{
                (*DNavState)->objIdShakingGrass = i;
                return 1;
            }
        }
        return 1;
    }
	else
        return 0;	
    return 0;
};




void DexHUDHBlank(void)
{
    if (REG_VCOUNT > (160 - 19))
	{ // fill 19 pixels

        //WRITE_REG_WININ(0x3F3F);
        REG_WININ = WININ_BUILD(WIN_BG0 | WIN_BG1 | WIN_BG2 | WIN_BG3 | WIN_OBJ | WIN_BLD,  WIN_BG0 | WIN_BG1 | WIN_BG2 | WIN_BG3 | WIN_OBJ | WIN_BLD);

        //WRITE_REG_BLDCNT(0x0FFE);
        REG_BLDCNT = (BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_TGT1_BG3 | BLDCNT_TGT1_OBJ | BLDCNT_TGT1_BD | BLDCNT_TGT2_BG0 | BLDCNT_TGT2_BG1 | BLDCNT_TGT2_BG2 | BLDCNT_TGT2_BG3 | BLDCNT_EFFECT_DARKEN);
        REG_BLDY = 0xC;


    } 
	else
	{
        // default values from overworld
        // WRITE_REG_WININ(0x1F1F);
        REG_WININ = WININ_BUILD(WIN_BG0 | WIN_BG1 | WIN_BG2 | WIN_BG3 | WIN_OBJ, WIN_BG0 |
                        WIN_BG1 | WIN_BG2 | WIN_BG3 | WIN_OBJ);
        //WRITE_REG_BLDCNT(0x401E);
        REG_BLDCNT = BLDALPHA_BLEND(BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_TGT1_BG3 | BLDCNT_TGT1_OBJ, 0);
    }
};


void DexNavProximityUpdate(void)
{
    (*DNavState)->proximity =  GetPlayerDistance((*DNavState)->tileX, (*DNavState)->tileY);
};



void NullSubHBlank(void)
{
    return;
};


void DexNavFreeHUD(void)
{
    switch ((*DNavState)->environment)
	{
        case 0:
            if (!CheckOpenSky(gMapHeader.mapType))
                StopOE(&gSprites[(*DNavState)->objIdShakingGrass], 0x1A); //cave
			else 
                StopOE(&gSprites[(*DNavState)->objIdShakingGrass], 0x13);
            break;
        case 1:
            StopOE(&gSprites[(*DNavState)->objIdShakingGrass], 0x13);
            break;
    };
    FreeOBJ(&gSprites[(*DNavState)->objIdSpecies]);
    DeleteOBJ(&gSprites[(*DNavState)->objIdSpecies]);

    FreeOBJ(&gSprites[(*DNavState)->objIdSight]);
    DeleteOBJ(&gSprites[(*DNavState)->objIdSight]);

    FreeOBJ(&gSprites[(*DNavState)->objIdAbility]);
    DeleteOBJ(&gSprites[(*DNavState)->objIdAbility]);

    FreeOBJ(&gSprites[(*DNavState)->objIdMove]);
    DeleteOBJ(&gSprites[(*DNavState)->objIdMove]);

    FreeOBJ(&gSprites[(*DNavState)->objIdItem]);
    DeleteOBJ(&gSprites[(*DNavState)->objIdItem]);

    FreeOBJ(&gSprites[(*DNavState)->objIdPotential[0]]);
    DeleteOBJ(&gSprites[(*DNavState)->objIdPotential[0]]);

    FreeOBJ(&gSprites[(*DNavState)->objIdPotential[1]]);
    DeleteOBJ(&gSprites[(*DNavState)->objIdPotential[1]]);

    FreeOBJ(&gSprites[(*DNavState)->objIdPotential[2]]);
    DeleteOBJ(&gSprites[(*DNavState)->objIdPotential[2]]);

    FreeSpriteTilesByTag(0x4736);
    FreeSpriteTilesByTag(0x61);
    FreeSpriteTilesByTag(0x2613);
    FreeSpriteTilesByTag(0x5424);
    FreeSpriteTilesByTag(0x8472);
    FreeSpriteTilesByTag(0x3039);
    FreeSpriteTilesByTag(0x1EE7);
    FreeSpritePaletteByTag(0x8472);
    FreeSpritePaletteByTag(0x3039);

    Free((void *)*DNavState);
    DisableInterrupts(2);
    SetHBlankCallback(NullSubHBlank);

    // WRITE_REG_WININ(0x1F1F);
    REG_WININ = WININ_BUILD(WIN_BG0 | WIN_BG1 | WIN_BG2 | WIN_BG3 | WIN_OBJ, WIN_BG0 |
                            WIN_BG1 | WIN_BG2 | WIN_BG3 | WIN_OBJ);
    //WRITE_REG_BLDCNT(0x401E);
    REG_BLDCNT = BLDALPHA_BLEND(BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_TGT1_BG3 | BLDCNT_TGT1_OBJ, 0);
};



void DexNavShowMessage(u8 id)
{
    switch(id)
	{
        case 0:
            //extern void MsgNormal(pchar *);
            //pchar empty_string[] = _("The Pokémon got away!\p");
            MsgNormal(&gText_GotAway[0]);
            break;
        case 1:
            //extern void MsgNormal(pchar *);
            //pchar empty_string[] = _("There is no reaction.\nThe signal was lost!\p");
            MsgNormal(&gText_LostSignal[0]);
            break;
    };
    return;
};



void OutlinedFontDraw(u8 objId, u8 tileNum, u16 size)
{	
	u8 tile = gSprites[objId].oam.tileNum + tileNum;
    u8* toWrite = (u8*)((tile * TILE_SIZE) + (SPRITE_RAM));
    //u8* dst = (u8*)Malloc(size + TILE_SIZE);
    //u8* dst = &gEnemyParty[0];
	
	u8* dst = (u8*) (0x0202402c);
	
    u8 counter = 1;
    u16 index = 320;
    u16 prevIndex;
	
	u8 element = *gStringVar4;
	u8 i = 0;
	
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

        if ((counter == 0) || (*(gStringVar4 + 1) == 0xFF))
		{
            // first or last pcharacters don't need pixel merging
            Memcpy((void*)dst, (void*)(&gInterfaceGfx_dexnavStarsTiles[index]), TILE_SIZE);
        }
		else if ((element == 0x0))
		{
            Memcpy((void*)dst, (void*)(&gInterfaceGfx_dexnavStarsTiles[index]), TILE_SIZE);
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
		else if ((*(gStringVar4 + 1) != 0xFF))
		{

            // pcharacter in middle, if blank space fill blank with previous pcharacter's last pixel row IFF previous pchar's last pixel row non-empty
            Memcpy((void*)dst, (void*)(&gInterfaceGfx_dexnavStarsTiles[index]), TILE_SIZE);
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

        if ((counter == 2) && (*(gStringVar4 + 1) != 0xFF))
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
		i++;
		element = *(gStringVar4 + i);
    }
    Memcpy((void*) toWrite, (void*) dst, size);
    Memset((void*) dst, 0x0, size + TILE_SIZE);
};




void DexNavSightUpdate(u8 sight)
{
    u8 objId = (*DNavState)->objIdSight;
    // draw sight eye on first tile, takes up two tiles
    u8 tileid = gSprites[objId].oam.tileNum;
    u8* toWrite = (u8*)((tileid * 32) + (SPRITE_RAM));
    Memcpy((void*)toWrite, (void*) &(gInterfaceGfx_dexnavStarsTiles[((19 * 4) + (7 - (2 * sight))) * 32]), 64);

    // draw the B button tile
    Memcpy((void*)(toWrite + 128), (void*) &(gInterfaceGfx_dexnavStarsTiles[((19 * 4) + 2) *32]), 64);

    // draw info text on the 5th tile
    //pchar back[] = _(" Back  ");
    StringCopy((u8*) gStringVar4, (u8*) &gText_DexNavBack[0]);
    OutlinedFontDraw(objId, 5, 32 * 8);
};



void DexNavIconsVisionUpdate(u8 proximity, u8 searchLevel)
{
     //   The sight eye and species icon are always drawn in the HUD
       // At search level 2, first move is drawn
       // At search level 3, Ability and Held item are drawn
       // At search level 5, Potential is drawn
    

    // species and sight/info are always shown, regardless of proximity
    gSprites[(*DNavState)->objIdSpecies].oam.affineMode = 0;
    gSprites[(*DNavState)->objIdSight].oam.affineMode = 0;

    // hide everything at the start, and recalc what needs to be shown
    gSprites[(*DNavState)->objIdMove].oam.affineMode = 2;
    gSprites[(*DNavState)->objIdAbility].oam.affineMode = 2;
    gSprites[(*DNavState)->objIdItem].oam.affineMode = 2;
    gSprites[(*DNavState)->objIdPotential[0]].oam.affineMode = 2;
    gSprites[(*DNavState)->objIdPotential[1]].oam.affineMode = 2;
    gSprites[(*DNavState)->objIdPotential[2]].oam.affineMode = 2;

    //extern void DexNavSightUpdate(u8);
    if (proximity < 3)
	{
        // at proximity 5, the sight should start to show and indicate sneaking is required
        DexNavSightUpdate(0); // alert level TODO: Update in a better way utiliting sight_level
        if (searchLevel > 1)
		{
            // show move, hide others
            gSprites[(*DNavState)->objIdMove].oam.affineMode = 0;
        }
        if (searchLevel > 2)
		{
            // show ability, move, hide others
            gSprites[(*DNavState)->objIdAbility].oam.affineMode = 0;
            if ((*DNavState)->heldItem)
			{
                // toggle item view
                gSprites[(*DNavState)->objIdItem].oam.affineMode = 0;
            }
        }
        if (searchLevel > 4)
		{
            gSprites[(*DNavState)->objIdPotential[0]].oam.affineMode = 0;
            gSprites[(*DNavState)->objIdPotential[1]].oam.affineMode = 0;
            gSprites[(*DNavState)->objIdPotential[2]].oam.affineMode = 0;
        }

    } 
	else if (proximity < 6)
        DexNavSightUpdate(1); // Sneaking is required flag
	else
        DexNavSightUpdate(2); // Sneaking is not required
};




void DexNavManageHUD(u8 taskId)
{
    // check for out of range
    if ((*DNavState)->proximity > 20)
	{
        DestroyTask(taskId);
        DexNavFreeHUD();
        DexNavShowMessage(1);
        return;
    }

    // check for timeout.
    gTasks[taskId].data[1]++;
    if (gTasks[taskId].data[1] > (0x384))
	{
        DestroyTask(taskId);
        DexNavFreeHUD();
        DexNavShowMessage(0);
        return;
    }

    // check if script just executed
    if (ScriptEnv2IsEnabled())
	{
        DestroyTask(taskId);
        DexNavFreeHUD();
        return;
    }

    // caves and water the pokemon moves around
    if ((((*DNavState)->environment == 1) || (!CheckOpenSky(gMapHeader.mapType)))
    && ((*DNavState)->proximity < 2) && ((*DNavState)->movementTimes < 2))
	{
        switch((*DNavState)->environment)
		{
            case 0:
                StopOE(&gSprites[(*DNavState)->objIdShakingGrass], 0x1A); // 1a
                break;
            case 1:
                StopOE(&gSprites[(*DNavState)->objIdShakingGrass], 0x16);
                break;
            default:
                break;
        };
        //extern u8 ShakingGrass(u8, u8, u8, bool8);
        while(!(ShakingGrass((*DNavState)->environment, 8, 8, 1)))
		{
            __asm__("mov r8, r8"); // nop...do I even need this?
        }
        (*DNavState)->movementTimes++;
    }

    // check for encounter start
    if ((*DNavState)-> proximity < 1)
	{
        //extern void DexNavGetMon(u16 species, u8 potential, u8 level, u8 ability, u16* moves);
        DexNavGetMon((*DNavState)->species, (*DNavState)->potential, (*DNavState)->pokemonLevel,
        (*DNavState)->ability, (*DNavState)->moveId);
        DestroyTask(taskId);

        // increment the search level
        if (SearchLevels[(*DNavState)->species] < 100)
            SearchLevels[(*DNavState)->species] += 1;

        // Freeing only the state, objects and hblank cleared on battle start.
        Free((void *)*DNavState);

        // exclamation point animation over the player
        MakeExclamationMark(gEventObjects, &gSprites[gPlayerAvatar->spriteId]);
        ExecOE(0x0);

        // do battle
        DoStandardWildBattle();
    };

    // TODO : CHECK SNEAKING
    // HUD needs updating iff player has moved
    DexNavProximityUpdate();
    if (!(gTasks[taskId].data[0] == (*DNavState)->proximity))
	{
        DexNavIconsVisionUpdate((*DNavState)->proximity, (*DNavState)->searchLevel);
        gTasks[taskId].data[0] = (*DNavState)->proximity;
    }

};



// ===================================== //
// ================ HUD ================ //
// ===================================== // 
u8 GetEncounterLevel(u16 species, u8 environment)
{
    //u8 index = GetWildDataIndexForMap();
	const struct WildPokemonHeader* header = GetCurrentMapWildMonHeaderId();
    if (header == NULL)
		return 22;
	
	u8 min = 100;
    u8 max = 0;
	u8 loopInd;
	//struct WildPokemon MonData;
    
	switch (environment)
	{
		case 0:
			if (header->landMonsInfo == NULL)
				return 22;			
			for (u8 i = 0; i < 12; ++i)
			{
				if (header->landMonsInfo->wildPokemon[i].species == species)
				{
					min = (min < header->landMonsInfo->wildPokemon[i].minLevel) ? min : header->landMonsInfo->wildPokemon[i].minLevel;
					max = (max < header->landMonsInfo->wildPokemon[i].maxLevel) ? max : header->landMonsInfo->wildPokemon[i].maxLevel;
				}
			}
			break;
			//struct WildPokemon MonData = header->landMonsInfo->wildPokemon;
			//if (MonData == 0)
			//	return 22;			
			//break;
			
		case 1:
			if (header->waterMonsInfo == NULL)
				return 22;

			for (u8 i = 0; i < loopInd; ++i)
			{
				if (header->waterMonsInfo->wildPokemon[i].species == species)
				{
					min = (min < header->waterMonsInfo->wildPokemon[i].minLevel) ? min : header->waterMonsInfo->wildPokemon[i].minLevel;
					max = (max < header->waterMonsInfo->wildPokemon[i].maxLevel) ? max : header->waterMonsInfo->wildPokemon[i].maxLevel;
				}
			}
			break;
		default:
			return 22;
	}
			
	/*
	switch (environment)
	{
        // grass
        case 0:
        {
			if (header->landMonsInfo == NULL)
				return 0;
            u8 i;
            for (i = 0; i < 12; ++i)
			{
                //struct WildPokemon WildMons = gWildMonHeaders[index].landMonsInfo->wildEncounters->wildGrass[i];
                //if (WildMons.species == species)
				if (header->landMonsInfo->wildPokemon[i].species == species)
				{
                    min = (min < header->landMonsInfo->wildPokemon[i].minLevel) ? min : header->landMonsInfo->wildPokemon[i].minLevel;
                    max = (max > header->landMonsInfo->wildPokemon[i].maxLevel) ? max : header->landMonsInfo->wildPokemon[i].maxLevel;
                }
            }
            break;
        }
        // surf
        case 1:
        {
            u8 i;
            for (i = 0; i < 5; ++i)
			{
                struct WildPokemon WildMons = gWildMonHeaders[index].waterMonsInfo->wildEncounters->wildWater[i];
                if (WildMons.species == species)
				{
                    min = (min < WildMons.minLevel) ? min : WildMons.minLevel;
                    max = (max > WildMons.maxLevel) ? max : WildMons.maxLevel;
                }
            }
            break;
        }
        default:
        {
            return 22; // Hidden pokemon should only appear on walkable tiles or surf tiles
            break;
        }
    };
	
	*/
	
    if (max == 0)
	{
        // Free dexnav display message
        return 0xFF;
    }

    // mod div by 0 edge case.
    if (min == max)
        return min;
    return (min + (Random() % (max - min)));
};



u8 DexNavGenerateMonLevel(u16 species, u8 searchLevel, u8 environment)
{
    u8 levelBase = GetEncounterLevel(species, environment);
    if (levelBase > 100)
        return (0);
	
    u8 searchLevelBonus = 0;
    if ((searchLevel >> 2) > 20)
        searchLevelBonus = 20; 
	else
        searchLevelBonus = (searchLevel >> 2);
    
    if ((searchLevelBonus + levelBase) > 100)
        return 100;
    else
        return ((searchLevel >> 2) + (levelBase));
};


u16 DexNavGenerateHeldItem(u16 species, u8 searchLevel)
{
    u16 randVal = Random() % 100;
    u8 searchLevelInfluence = searchLevel >> 1;
    u16 item1 = gBaseStats[species].item1;
    u16 item2 = gBaseStats[species].item2;

    // if both are the same, 100% to hold
    if (item1 == item2)
        return item1;

    // if no items can be held, then yeah..no items
    if ((item2 == ITEM_NONE) && (item1 == ITEM_NONE))
        return ITEM_NONE;

    // if only one entry, 50% chance
    if ((item2 == ITEM_NONE) && (item1 != ITEM_NONE))
        return (randVal <= 50) ? item1 : ITEM_NONE;

    // if both are distinct item1 = 50% + srclvl/2; item2 = 5% + srchlvl/2
    if (randVal < ((50 + searchLevelInfluence) + (5 + searchLevel)))
        return (randVal > (5 + searchLevelInfluence)) ? item1 : item2;
    else
        return ITEM_NONE;
    return ITEM_NONE;
};


u8 DexNavGenerateHiddenAbility(u16 species, u8 searchLevel)
{
    bool8 genAbility = FALSE;
    u16 randVal = Random() % 100;
    if (searchLevel < 5)
	{
        if (randVal <= SEARCHLEVEL0_ABILITYCHANCE)
			genAbility = TRUE;
    } 
	else if (searchLevel < 10)
	{
        if (randVal <= SEARCHLEVEL5_ABILITYCHANCE)
			genAbility = TRUE;
    } 
	else if (searchLevel < 25)
	{
        if (randVal <= SEARCHLEVEL10_ABILITYCHANCE)
			genAbility = TRUE;
    } 
	else if (searchLevel < 50)
	{
        if (randVal <= SEARCHLEVEL25_ABILITYCHANCE)
			genAbility = TRUE;
    } 
	else if (searchLevel < 100)
	{
        if (randVal <= SEARCHLEVEL50_ABILITYCHANCE)
			genAbility = TRUE;
    } 
	else
	{
        if (randVal <= SEARCHLEVEL100_ABILITYCHANCE)
			genAbility = TRUE;
    }

    if ((genAbility) && (gBaseStats[species].hiddenAbility != ABILITY_NONE))
        return gBaseStats[species].hiddenAbility; // should be ability when reworked
    else 
	{
        // pick a normal ability of that Pokemon
        if (gBaseStats[species].ability2)
            return randVal % 2 ? gBaseStats[species].ability1 : gBaseStats[species].ability2; 
		else
            return gBaseStats[species].ability1;
    }
};



u8 DexNavGeneratePotential(u8 searchLevel)
{
    u8 genChance = 0;
    u16 randVal = Random() % 100;
    if (searchLevel < 5) {
        genChance = SEARCHLEVEL0_ONESTAR + SEARCHLEVEL0_TWOSTAR + SEARCHLEVEL0_THREESTAR;
        if (randVal <= genChance)
		{
            // figure out which star it is
            if (randVal <= SEARCHLEVEL0_ONESTAR)
                return 1;
            else if (randVal <= (SEARCHLEVEL0_ONESTAR + SEARCHLEVEL0_TWOSTAR))
                return 2;
            else
                return 3;
        }
    }
	else if (searchLevel < 10)
	{
        genChance = SEARCHLEVEL5_ONESTAR + SEARCHLEVEL5_TWOSTAR + SEARCHLEVEL5_THREESTAR;
        if (randVal <= genChance)
		{
            // figure out which star it is
            if (randVal <= SEARCHLEVEL5_ONESTAR)
                return 1;
            else if (randVal <= (SEARCHLEVEL5_ONESTAR + SEARCHLEVEL5_TWOSTAR))
                return 2;
            else
                return 3;
        }
    }
	else if (searchLevel < 25)
	{
        genChance = SEARCHLEVEL10_ONESTAR + SEARCHLEVEL10_TWOSTAR + SEARCHLEVEL10_THREESTAR;
        if (randVal <= genChance)
		{
            // figure out which star it is
            if (randVal <= SEARCHLEVEL10_ONESTAR)
                return 1;
            else if (randVal <= (SEARCHLEVEL10_ONESTAR + SEARCHLEVEL10_TWOSTAR))
                return 2;
            else
                return 3;
        }
    }
	else if (searchLevel < 50)
	{
        genChance = SEARCHLEVEL25_ONESTAR + SEARCHLEVEL25_TWOSTAR + SEARCHLEVEL25_THREESTAR;
        if (randVal <= genChance)
		{
            // figure out which star it is
            if (randVal <= SEARCHLEVEL25_ONESTAR)
                return 1;
            else if (randVal <= (SEARCHLEVEL25_ONESTAR + SEARCHLEVEL25_TWOSTAR))
                return 2;
            else
                return 3;
        }
    }
	else if (searchLevel < 100)
	{
        genChance = SEARCHLEVEL50_ONESTAR + SEARCHLEVEL50_TWOSTAR + SEARCHLEVEL50_THREESTAR;
        if (randVal <= genChance)
		{
            // figure out which star it is
            if (randVal <= SEARCHLEVEL50_ONESTAR)
                return 1;
            else if (randVal <= (SEARCHLEVEL50_ONESTAR + SEARCHLEVEL50_TWOSTAR))
                return 2;
            else
                return 3;
        }
    }
	else
	{
        genChance = SEARCHLEVEL100_ONESTAR + SEARCHLEVEL100_TWOSTAR + SEARCHLEVEL100_THREESTAR;
        if (randVal <= genChance)
		{
            // figure out which star it is
            if (randVal <= SEARCHLEVEL100_ONESTAR)
                return 1;
            else if (randVal <= (SEARCHLEVEL100_ONESTAR + SEARCHLEVEL100_TWOSTAR))
                return 2;
            else
                return 3;
        }
    }
    // No potential
    return 0;
};


void DexNavGenerateMove(u16 species, u8 searchLevel, u8 encounterLevel, u16* moveLoc)
{
    bool8 genMove = FALSE;
    u16 randVal = Random() % 100;

    // evaluate if Pokemon should get an egg move in first slot
    if (searchLevel < 5)
	{
        if (randVal <= SEARCHLEVEL0_MOVECHANCE)
			genMove = TRUE;
    }
	else if (searchLevel < 10)
	{
        if (randVal <= SEARCHLEVEL5_MOVECHANCE)
			genMove = TRUE;
    }
	else if (searchLevel < 25)
	{
        if (randVal <= SEARCHLEVEL10_MOVECHANCE)
			genMove = TRUE;
    }
	else if (searchLevel < 50)
	{
        if (randVal <= SEARCHLEVEL25_MOVECHANCE)
			genMove = TRUE;
    }
	else if (searchLevel < 100)
	{
        if (randVal <= SEARCHLEVEL50_MOVECHANCE)
			genMove = TRUE;
    }
	else
	{
        if (randVal <= SEARCHLEVEL100_MOVECHANCE)
			genMove = TRUE;
    }
    // genMove = TRUE;
    // get the species's egg move, if one exists
    //u16 move[4] = {MOVE_NONE, MOVE_NONE, MOVE_NONE, MOVE_NONE};
    
	// generate a wild mon and copy moveset
	#ifdef TANOBY_RUINS_ENABLED
		const struct WildPokemonHeader* header = GetCurrentMapWildMonHeaderId();
		u8 wildMonIndex = 0;
		if (header->landMonsInfo != NULL)
			wildMonIndex = ChooseWildMonIndex_Land();
		else if (header->waterMonsInfo != NULL)
			wildMonIndex = ChooseWildMonIndex_WaterRock();
		CreateWildMon(species, encounterLevel, wildMonIndex, FALSE);
	#else
		CreateWildMon(species, encounterLevel, 0, FALSE);
	#endif

	// store generated mon moves into Dex Nav Struct
	for (u8 i = 0; i < 4; i++)
	{
		moveLoc[i] = gEnemyParty[0].moves[i];
	}

	// set first move slot to a random egg move if search level is good enough
	if (genMove)
	{
		u16 eggMoveBuffer[EGGMOVE_MAX];
		u8 numEggMoves = GetEggMoves(&gEnemyParty[0], &eggMoveBuffer);
		if (numEggMoves != 0)
		{
			u8 index = RandRange(0, EGGMOVE_MAX);
			moveLoc[0] = eggMoveBuffer[index];
		}
	}

	
/*		OLD, keep for now
	if (genMove)
	{
        u16 eggmoveBuff[EGGMOVE_MAX];
        u16 eggSpecies = GetBaseSpecies(species);
        PopulateEggMoveBuffer(eggSpecies, &eggmoveBuff[0]);
        move[0] =  eggmoveBuff[Random() % CountEggMoves(&eggmoveBuff[0])];
    }
	else
        genMove = FALSE;	// species has no egg moves



    // calculate the normal moves
    u16 thisMove = MOVE_NONE;

    u8 i = 0;
    u8 cMoveIndex = genMove;
    while (thisMove < MOVES_COUNT)
	{
       struct LearnsetEntry learnable_move = pokemon_learnset[species][i];
       thisMove = learnable_move.move;

       // learnset is level ordered, so exit early
       if (learnable_move.level > encounterLevel) {
            break;
       } else {
            // if an eggmove is in the first slot, skip it and move to the next slot
            if ((genMove) && (!(i % 4))) {
                move[1] = thisMove;
            } else {
                move[cMoveIndex] = thisMove;
            }
       }
       i += 1;
       cMoveIndex += 1;

    }

    // assign moves to array
    for (i = 0; i < 4; ++i)
	{
        moveLoc[i] = move[i];
    }
	
*/


    return;
};




void DexNavDrawSight(u8 sight_lvl, u8* objidAddr)
{
    // create empty object of size 64x32 to draw icons on
    struct SpriteSheet sightTiles = {(const u8*)(&gInterfaceGfx_emptyTiles), 0x800, 0x5424};
    struct SpriteTemplate fontTempSight = {
		.tileTag = 0x5424,
		.paletteTag = 0x8472,
		.oam = (struct OamData*) &FontOAM,
		.anims = (const union AnimCmd* const*)0x8231CF0, 
		.images = NULL,
		.affineAnims = (const union AffineAnimCmd* const*) 0x8231CFC,
		.callback = (SpriteCallback) 0x800760D,
	};
	
    GpuTileObjAllocTagAndUpload(&sightTiles);
    GpuPalObjAllocTagAndApply(&HeldPal);
    u8 objId = CreateSprite(&fontTempSight, ICONX + 192, ICONY + 0x12, 0x0);
    *objidAddr = objId;
    gSprites[objId].oam.affineMode = 2;
    extern void DexNavSightUpdate(u8);
    DexNavSightUpdate(sight_lvl);
    gSprites[objId].oam.objMode = 1;
};


void DexNavDrawAbility(u8 ability, u8* objidAddr)
{
    // create empty object of size 32x64 to draw font on
    struct SpriteSheet FontSpriteAbility = {(const u8*)(&gInterfaceGfx_emptyTiles), 0x800, 0x1EE7};
    struct SpriteTemplate FontTempAbility = {
		.tileTag = 0x1EE7, 
		.paletteTag = 0x8472, 
		.oam = (struct OamData*) &FontOAM, 
		.anims = (const union AnimCmd* const*)0x8231CF0, 
		.images = NULL,
		.affineAnims = (const union AffineAnimCmd* const*) 0x8231CFC,  
		.callback = (SpriteCallback) 0x800760D,
	};
    GpuTileObjAllocTagAndUpload(&FontSpriteAbility);
    GpuPalObjAllocTagAndApply(&HeldPal);
    u8 objId = CreateSprite(&FontTempAbility, ICONX + 80, ICONY + 0x12, 0x0);
    *objidAddr = objId;
    gSprites[objId].oam.affineMode = 2;

    // ability name beside move name
    u8 len = (*DNavState)->moveNameLength;
    gSprites[objId].pos1.x += ((8 * (len/2)) + (4 * (len % 2)));

    // Copy ability string from table using state id
    Memcpy((void *)(gStringVar4), (void *)&(gAbilityNames[ability]), ABILITY_NAME_LENGTH);

    // format string so it's even length or if it's odd ends in two spaces
    gStringVar4[ABILITY_NAME_LENGTH + 1] = 0xFF;
    len = StringLength(gStringVar4);
    if (!(len % 2)) 
	{
        gStringVar4[len] = 0x0;
        gStringVar4[len + 1] = 0x0;
        gStringVar4[len + 2] = 0xFF;
    }

    // write name to object
    OutlinedFontDraw(objId, 0, 32 * 8);
    gSprites[objId].oam.objMode = 1;
    return;
};



void DexNavDrawMove(u16 move, u8 searchLevel, u8* objidAddr)
{
    // create empty object of size 32x64 to draw font on
    struct SpriteSheet FontSpriteMove = {(const u8*)(&gInterfaceGfx_emptyTiles), 0x800, 0x4736};
    struct SpriteTemplate FontTempMove = {
		.tileTag = 0x4736,
		.paletteTag = 0x8472,
		.oam = (struct OamData*) &FontOAM,
		.anims = (const union AnimCmd* const*) 0x8231CF0,
		.images = NULL,
        .affineAnims = (const union AffineAnimCmd* const*) 0x8231CFC,
		.callback = (SpriteCallback) 0x800760D,
	};
    GpuTileObjAllocTagAndUpload(&FontSpriteMove);
    GpuPalObjAllocTagAndApply(&HeldPal);
    u8 objId = CreateSprite(&FontTempMove, ICONX + 80, ICONY + 0x12, 0x0);
    *objidAddr = objId;
    gSprites[objId].oam.affineMode = 2;

    // Copy move string from table using state id, add '/' character to the end of it
    Memcpy((void *)gStringVar4, (void *)gMoveNames[move], MOVE_NAME_LENGTH);
    gStringVar4[MOVE_NAME_LENGTH + 1] = 0xFF;
    u8 len = StringLength(gStringVar4);

    if (searchLevel > 2) 
	{
        gStringVar4[len] = 0xBA;
        len += 1;
    }

    // record length of move with slash for ability name to be placed beside it
    (*DNavState)->moveNameLength = len;

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
    gSprites[objId].oam.objMode = 1;
};



void DexNavDrawPotential(u8 potential, u8* objidAddr) 
{
    // 19 tiles per row, stars are on the 4th row. 1 tile is 32 bytes. Hence 19 * 4 *32
    struct SpriteSheet StarIconLit = {(const u8*)(&(gInterfaceGfx_dexnavStarsTiles[19 * 4 * 32])), 64, 0x61};
    struct SpriteSheet StarIconOff = {(const u8*)(&(gInterfaceGfx_dexnavStarsTiles[((19 * 4) + 1) *32])), 64, 0x2613};

    // TODO: put these function pointers and other data ptrs in PokeAGB.
    struct SpriteTemplate StarLitTemp = {
		.tileTag = 0x61,
		.paletteTag = 0x8472,
		.oam = (struct OamData*) &HeldOAM,
		.anims = (const union AnimCmd* const*) 0x8231CF0, 
		.images = NULL,
        .affineAnims = (const union AffineAnimCmd* const*) 0x8231CFC, 
		.callback = (SpriteCallback) 0x800760D,
	};
    struct SpriteTemplate StarOffTemp = {
		.tileTag = 0x2613,
		.paletteTag = 0x8472,
		.oam = (struct OamData*) &HeldOAM,
		.anims = (const union AnimCmd* const*)0x8231CF0, 
		.images = NULL,
        .affineAnims = (const union AffineAnimCmd* const*) 0x8231CFC, 
		.callback = (SpriteCallback) 0x800760D,
		};

    // allocate both the lit and unlit star to VRAM
    GpuTileObjAllocTagAndUpload(&StarIconLit);
    GpuTileObjAllocTagAndUpload(&StarIconOff);
    GpuPalObjAllocTagAndApply(&HeldPal);

    // create star objects and space them according to potential 0 - 3
    u8 objId;
    u8 i;
    for (i = 0; i < 3; ++i)
	{
        if (potential > i)
		{
            objId = CreateSprite(&StarLitTemp, ICONX + 23 + (i * 8), ICONY + 0x5, 0x0);
        } 
		else 
		{
            objId = CreateSprite(&StarOffTemp, ICONX + 23 + (i * 8), ICONY + 0x5, 0x0);
        }
        objidAddr[i] = objId;
        gSprites[objId].oam.affineMode = 2;
        gSprites[objId].oam.objMode = 1;
    }
};



//const (u8*) gPokeIconPals[] = {(u32*) 0x83d3740, 0x83D3760, 0x83D3780}; 



//void DexNavDrawSpeciesIcon(u16 species, u8* objIdAddr)
void DexNavDrawSpeciesIcon(u16 species)
{
	//doesnt make it here
	u32 pid = RandRange(0, 0xFFFF) | RandRange(0, 0xFFFF) << 16;
	CreateMonIcon(species, SpriteCB_PokeIcon, ICONX, ICONY, 0, pid, FALSE);



/* 

 // check which palette the species icon uses
    u8 iconPal = gMonIconPaletteIndices[species];
    //struct SpritePalette bulbPal = {&(gPokeIconPals[iconPal]), 0x3139};
    struct SpritePalette bulbPal = {(gPokeIconPals[iconPal]), 0x3139};
	GpuPalObjAllocTagAndApply(&bulbPal);

    u32 pid = RandRange(0, 0xFFFF) | RandRange(0, 0xFFFF) << 16;
    void *IconGFX = LoadPartyIconTiles(species, pid, FALSE);
    struct SpriteSheet BulbIcon = {IconGFX, 4 * 8 * 32, 0x3139};
    GpuTileObjAllocTagAndUpload(&BulbIcon);

    // create object
    u8 objId = CreateSprite(&BulbTemp, ICONX, ICONY, 0x0);
    *objIdAddr = objId;
    //dprintf("address is: %x\n", objects[objId].image);
    
	//dprintf("address is: %x\n", &picon_oam);
    gSprites[objId].oam.affineMode = 2;
    gSprites[objId].oam.objMode = 1;
	
*/


};


void DexNavDrawHeldItem(u8* objidAddr)
{
    // create object for held item icon
    struct SpriteSheet HeldIcon = {(const u8*)(0x0845A3AC), 64, 0x8472};
    struct SpriteTemplate HeldTemp = {
		.tileTag = 0x8472,
		.paletteTag = 0x8472,
		.oam = (struct OamData*) &HeldOAM,
		.anims = (const union AnimCmd* const*)0x8231CF0, 
		.images = NULL,
		.affineAnims = (const union AffineAnimCmd* const*) 0x8231CFC,
		.callback = (SpriteCallback) 0x800760D,
		};
    GpuTileObjAllocTagAndUpload(&HeldIcon);
    GpuPalObjAllocTagAndApply(&HeldPal);
    u8 objId = CreateSprite(&HeldTemp, ICONX + 0x8, ICONY + 0xC, 0x0);
    *objidAddr = objId;
    gSprites[objId].oam.affineMode = 2;
    gSprites[objId].oam.objMode = 1;
};



void DexNavDrawIcons(void)
{
    //DOESNT MAKE IT HERE
	u8 searchLevel = (*DNavState)->searchLevel;
    DexNavDrawSight((*DNavState)->proximity, &(*DNavState)->objIdSight);
    //DexNavDrawSpeciesIcon((*DNavState)->species, &(*DNavState)->objIdSpecies);
	DexNavDrawSpeciesIcon((*DNavState)->species);
    DexNavDrawMove((*DNavState)->moveId[0], searchLevel, &(*DNavState)->objIdMove);
    DexNavDrawAbility((*DNavState)->ability, &(*DNavState)->objIdAbility);
    DexNavDrawHeldItem(&(*DNavState)->objIdItem);
    DexNavDrawPotential((*DNavState)->potential, &(*DNavState)->objIdPotential[0]);
};



void InitDexNavHUD(u16 species, u8 environment)
{
	*DNavState = (struct DexnavHudData*)Calloc(sizeof(struct DexnavHudData));
    // assign non-objects to struct
    (*DNavState)->species = species;
    (*DNavState)->environment = environment;
    u8 searchLevel = SearchLevels[species];
    (*DNavState)->searchLevel = searchLevel;
    (*DNavState)->pokemonLevel = DexNavGenerateMonLevel(species, searchLevel, environment);


    if ((*DNavState)->pokemonLevel < 1)
	{
        Free((void*)*DNavState);
        // pchar empty_string[] = _("This Pokemon cannot be found here.\p");
        MsgNormal(&gText_CannotBeFound[0]);
        return;
    }

    // draw shaking grass
    //extern u8 ShakingGrass(u8, u8, u8, bool8);
    if (!ShakingGrass(environment, 12, 12, 0))
	{
        Free((void*)*DNavState);
        //pchar empty_string[] = _("It couldn’t be found nearby.\nTry looking in a different area!\p");
        MsgNormal(&gText_NotFoundNearby[0]);
        return;
    }

    // Populate DNavState objects
    DexNavGenerateMove(species, searchLevel, (*DNavState)->pokemonLevel, &(*DNavState)->moveId[0]);
    (*DNavState)->heldItem = DexNavGenerateHeldItem(species, searchLevel);
    (*DNavState)->ability = DexNavGenerateHiddenAbility(species, searchLevel);
    (*DNavState)->potential = DexNavGeneratePotential(searchLevel);
    DexNavProximityUpdate();

    // draw icons
    DexNavDrawIcons();

    // hide icons based on proximity and search level
    DexNavIconsVisionUpdate((*DNavState)->proximity, searchLevel);

    // enable Hblank interrupt
    EnableInterrupts(2);
    SetHBlankCallback((SuperCallback)DexHUDHBlank);

    // task update HUD
    u8 taskId = CreateTask((TaskFunc)DexNavManageHUD, 0x1);
    gTasks[taskId].data[0] = gSprites[gPlayerAvatar->spriteId].pos1.x;

};



// This is called via a c1 from the GUI, while waiting to return to the OW
void ExecDexNavHUD(void) 
{
    if (!gPaletteFade->active && !ScriptEnv2IsEnabled() && gMain.callback2 == OverworldCallbackSwitchStartMenu)
	{
        SetCallback1(OverworldCallback1);
        InitDexNavHUD(Var8000, Var8001);
    }
};

u8 ExecDexNav(void)
{
    BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 16, 0x0000);
	SetCallback1(DexNavGuiHandler);
    gMain.state = 0;
    return TRUE;
};


// gfx clean
void DexNavGuiSetup(void)
{
    Setup();	
    FreeAllWindowBuffers();
    BgIdModOffsetX(0, 0, 0);
    BgIdModOffsetY(0, 0, 0);
    BgIdModOffsetX(1, 0, 0);
    BgIdModOffsetY(1, 0, 0);
    ResetBgsAndClearDma3BusyFlags(0);
    InitBgsFromTemplates(0, (struct BgTemplate *)&BgConfigDexNavGUI, 4);
    u32 set = 0;	
    CpuFastSet((void*)&set, (void*)VRAM, CPUModeFS(0x10000, CPUFSSET));
    GpuSyncBGHide(1);
    GpuSyncBGHide(0);
    SetCallback2(DexNavGUICallback2);
    SetVBlankCallback(VblackCallbackSeq);
};


void DexNavLoadPokeIcons(void)
{
	/*
	s16 x;
	s16 y;
	u32 pid = 0xFFFFFFFF;
	LoadMonIconPalettes();
	u16 species;
	
	// load grass species icons
	for (int i = 0; i < 12; ++i)
	{
		species = (*DNavState)->grassSpecies[i];
		x = 20 + (24 * (i % 6));
		y = 92 + (i > 5 ? 28 : 0);
		CreateMonIcon(species, SpriteCB_PokeIcon, x, y, 0, pid, FALSE);
	} 
	
	// water species
	for (int i = 0; i < 5; ++i)
	{
		species = (*DNavState)->waterSpecies[i];
		x = 30 + 24*i;
		y = 48;
		CreateMonIcon(species, SpriteCB_PokeIcon, x, y, 0, pid, FALSE);
	}
	
 
	
	
	// allocate all pallettes
    for (u32 i = 0; i < 3; ++i)
	{
        struct SpritePalette pal = {&gPokeIconPals[i], ICON_PAL_TAG + i};
        GpuPalObjAllocTagAndApply(&pal);
    }
	*/
	
	LoadMonIconPalettes();

    for (u8 i = 0; i < 12; ++i)
	{
        u16 species = (*DNavState)->grassSpecies[i];
        u32 pid = 0xFFFFFFFF;
		
		void *IconGFX = LoadPartyIconTiles(species, pid, FALSE);
		u16 gfxTag = ICON_GFX_TAG + i;
		u16 palTag = ICON_PAL_TAG + gMonIconPaletteIndices[species];
		struct SpriteSheet IconTiles = {IconGFX, 4 * 8 * 32, gfxTag};
		GpuTileObjAllocTagAndUpload(&IconTiles);
		struct SpriteTemplate IconTemplate = {
										.tileTag = gfxTag,
										.paletteTag = palTag,
										.oam = (struct OamData*) &IconOAM,
										.anims = (const union AnimCmd* const*) 0x8231CF0,
										.images = (const struct SpriteFrameImage *) &IconTiles,
										.affineAnims = (const union AffineAnimCmd* const*) 0x8231CFC,
										.callback = (SpriteCallback) SpriteCallbackDummy,
										};
															// x and y pos of sprite
		CreateSprite(&IconTemplate, 20 + (24 * (i % 6)), 92 + (i > 5 ? 28 : 0), 0);
    }
	
	
	
    for (u8 i = 0; i < 5; ++i)
	{
        u16 species = (*DNavState)->waterSpecies[i];
        u32 pid = 0xFFFFFFFF;
        //if (species > 0) {
            void *IconGFX = LoadPartyIconTiles(species, pid, FALSE);
            u16 gfxTag = ICON_GFX_TAG + i + 12;
            u16 palTag = ICON_PAL_TAG + gMonIconPaletteIndices[species];
            struct SpriteSheet IconTiles = {IconGFX, 4 * 8 * 32, gfxTag};
            GpuTileObjAllocTagAndUpload(&IconTiles);
            struct SpriteTemplate IconTemplate = {
                                            .tileTag = gfxTag,
                                            .paletteTag = palTag,
                                            .oam = (struct OamData*) &IconOAM,
                                            .anims = (const union AnimCmd* const*) 0x8231CF0,
                                            .images = (const struct SpriteFrameImage *) &IconTiles,
                                            .affineAnims = (const union AffineAnimCmd* const*) 0x8231CFC,
                                            .callback = (SpriteCallback) SpriteCallbackDummy,
                                            };
                                                                // x and y pos of sprite
            CreateSprite(&IconTemplate, 30 + 24 * i, 48, 0);
        //}
    }
	
};


void UpdateCursorPosition(void)
{
    // update cursor position
    if ((*DNavState)->selectedArr)
	{
        // water
        gSprites[(*DNavState)->cursorId].pos1.x = CursorPositions2[(*DNavState)->selectedIndex];
        gSprites[(*DNavState)->cursorId].pos1.y = CursorPositions2[(*DNavState)->selectedIndex + 1];
    }
	else
	{
        // grass
        gSprites[(*DNavState)->cursorId].pos1.x = CursorPositions1[(*DNavState)->selectedIndex];
        gSprites[(*DNavState)->cursorId].pos1.y = CursorPositions1[(*DNavState)->selectedIndex + 1];
    }
};


void SpawnPointerArrow(void)
{
    struct SpriteSheet CursorGFX = {
		(void*)gInterfaceGfx_selectionCursorTiles,
		32 * 32,
		SELECTION_CURSOR_TAG,
	};
    struct SpritePalette CursorPal = {
		(void*)gInterfaceGfx_selectionCursorPal,
		SELECTION_CURSOR_TAG,
	};
    struct SpriteTemplate CursorTemp = {
		.tileTag = SELECTION_CURSOR_TAG,
		.paletteTag = SELECTION_CURSOR_TAG,
		.oam = (struct OamData*) &CursorOAM,
        .anims = (const union AnimCmd* const*) 0x8231CF0,
		.images = (const struct SpriteFrameImage *) &CursorGFX,
		.affineAnims = (const union AffineAnimCmd* const*) 0x8231CFC,
		.callback = (SpriteCallback) SpriteCallbackDummy,
	};

    // uncompressed
    GpuTileObjAllocTagAndUpload(&CursorGFX);
    GpuPalObjAllocTagAndApply(&CursorPal);
    (*DNavState)->cursorId = CreateSprite(&CursorTemp, 30, 48, 0);
    UpdateCursorPosition();
};

void DexNavLoadNames(u8 status)
{
    // clean boxes
    for (u32 i = 0; i < 5; ++i)
	{
        FillWindowPixelBuffer(i, 0);
    }
	
    // rbox commit species name
    u16 species = (*DNavState)->selectedArr ? (*DNavState)->waterSpecies[(*DNavState)->selectedIndex >> 1] : (*DNavState)->grassSpecies[(*DNavState)->selectedIndex>>1];
    if (species)
        WindowPrint(0, 0, 0, 4, &DexNav_BlackText, 0, gSpeciesNames[species]);
	else
        WindowPrint(0, 0, 0, 4, &DexNav_BlackText, 0, &gText_DexNav_NoInfo[0]);

    // rbox commit search level
    ConvertIntToDecimalStringN(gStringVar4, SearchLevels[species], 0, 4);
    WindowPrint(1, 0, 0, 4, &DexNav_BlackText, 0, &gStringVar4[0]);

    // rbox commit level bonus
    u8 searchLevelBonus = 0;
    if ((SearchLevels[species] >> 2) > 20)
        searchLevelBonus = 20;
    else
        searchLevelBonus = (SearchLevels[species] >> 2);
	
    ConvertIntToDecimalStringN(gStringVar4, searchLevelBonus, 0, 4);
    WindowPrint(2, 0, 0, 4, &DexNav_BlackText, 0, &gStringVar4[0]);

    // rbox commit hidden ability name
    if (gBaseStats[species].hiddenAbility)
        WindowPrint(3, 0, 0, 4, &DexNav_BlackText, 0, gAbilityNames[gBaseStats[species].hiddenAbility]);
    else
        WindowPrint(3, 0, 0, 4, &DexNav_BlackText, 0, &gText_DexNav_NoInfo[0]);

    // rbox commit status message bar
    switch(status)
	{
        case 0:
            WindowPrint(4, 1, 0, 8, &DexNav_WhiteText, 0, &gText_DexNav_Invalid[0]);
            break;
        case 1:
            WindowPrint(4, 1, 0, 8, &DexNav_WhiteText, 0, &gText_DexNav_ChooseMon[0]);
            break;
        case 2:
            WindowPrint(4, 1, 0, 8, &DexNav_WhiteText, 0, &gText_DexNav_Locked[0]);
			break;
    }
    // display committed gfx
    for (u8 i = 0; i < 5; ++i)
	{
        CopyWindowToVram(i, 3);
        PutWindowTilemap(i);
    }
};

void CallbackDexNavOW(void)
{
	ExecDexNavHUD();
};




void DexNavGuiExitSearch(void)
{
    switch (gMain.state)
	{
        case 0:
            BeginNormalPaletteFade(~0, 0, 0x0, 0x10, 0);
            PlaySE(SE_PC_ON);
            gMain.state++;
            break;
        case 1:
            if (!gPaletteFade->active)
			{
                Free((*DNavState)->backBuffer);
                gMain.state++;
            }
            break;
        case 2:
            m4aMPlayVolumeControl(&gMPlay_BGM, 0xFFFF, 256);
            SetCallback1(CallbackDexNavOW);
            SetCallback2(CallbackOWandContinueScriptsMusic);
            break;
	}
};


void DexNavGuiExitNoSearch(void)
{
    switch (gMain.state)
	{
        case 0:
            BeginNormalPaletteFade(~0, 0, 0x0, 0x10, 0);
            gMain.state++;
            break;
        case 1:
            if (!gPaletteFade->active)
			{
                Free((*DNavState)->backBuffer);
                Free((void*)*DNavState);
                gMain.state++;
            }
            break;
        case 2:
            m4aMPlayVolumeControl(&gMPlay_BGM, 0xFFFF, 256);
            SetCallback1(OverworldCallback1);
            SetCallback2(OverworldCallbackSwitchStartMenu);
            break;
	}
};

void DexNavPopulateEncounterList(void)
{
    // nop struct data
    //Memset(&((*DNavState)->grassSpecies[0]), 0, 34);
	Memset(&((*DNavState)->grassSpecies[0]), 0, 34);
   
    
	// populate unique wild grass encounters
    u8 insertionIndex = 0;
    u8 index = GetWildDataIndexForMap();
	
	//const struct WildPokemonHeader* header = GetCurrentMapWildMonHeaderId();

    if (gWildMonHeaders[index].landMonsInfo)
	{
        for (u8 i = 0; i < 12; ++i)
		{
            //struct WildPokemon WildMons = gWildMonHeaders[index].landMonsInfo->wildEncounters->wildGrass[i];
			struct WildPokemon WildMons = gWildMonHeaders[index].landMonsInfo->wildPokemon[i];
            if (!SpeciesInArray(WildMons.species, 12))
			{
                (*DNavState)->grassSpecies[insertionIndex] = WildMons.species;
                insertionIndex++;
            }
        }
    };
	
    // populate unique wild water encounters
    insertionIndex = 0;
    // exit if no water encounters
    if (gWildMonHeaders[index].waterMonsInfo == NULL)
        return;
    for (u8 i = 0; i < 5; ++i)
	{
        //struct WildPokemon WildMons = gWildMonHeaders[index].waterMonsInfo->wildEncounters->wildWater[i];
        struct WildPokemon WildMons = gWildMonHeaders[index].waterMonsInfo->wildPokemon[i];
		if (!SpeciesInArray(WildMons.species, 5)) {
            (*DNavState)->waterSpecies[insertionIndex] = WildMons.species;
            insertionIndex++;
        }
    }
};

void DexNavGuiHandler(void)
{
	switch(gMain.state)
	{
        case 0:
            if (!gPaletteFade->active)
			{
                DexNavGuiSetup();
				// gets through here
				
                SetCallback1(DexNavGuiHandler);
                // allocate dexnav struct
                *DNavState = (struct DexnavHudData*)Calloc(sizeof(struct DexnavHudData));
				gMain.state++;
                //gMain.state = 1;
			}
            break;
        case 1:
		{
				// load BG assets
				void* DexNav_gbackBuffer = Malloc(0x800);
				(*DNavState)->backBuffer = DexNav_gbackBuffer;
				LoadPalette((void*) &DexNavTextPal, 15 * 16, 32);
				LoadCompressedPalette((void*) gInterfaceGfx_dexnavGuiPal, 0, 32);				
				LZ77UnCompWram((void*) gInterfaceGfx_dexnavGuiMap, (void*) DexNav_gbackBuffer);
				LZ77UnCompVram((void*) gInterfaceGfx_dexnavGuiTiles, (void*) 0x06000000);
				BgIdSetTilemap(1, DexNav_gbackBuffer);
				BgIdMarkForSync(1);
				BgIdMarkForSync(0);
				gMain.state++;
		}
			break;
			
        case 2:
			InitWindows(DexNavBoxes);
            DexNavPopulateEncounterList();
            DexNavLoadNames(1);
            gMain.state++;
            break;
        case 3:
            BeginNormalPaletteFade(0xFFFFFFFF, 0, 16, 0, 0x0000);
            GpuSyncBGShow(0);
            GpuSyncBGShow(1);
            DexNavLoadPokeIcons();
            SpawnPointerArrow();
            (*DNavState)->selectedArr = 0;
            (*DNavState)->selectedIndex = 0;
            gMain.state++;
            break;
        case 4:
            if (!gPaletteFade->active)
			{
                switch (gMain.newAndRepeatedKeys &
					(A_BUTTON | B_BUTTON | DPAD_DOWN | DPAD_UP | DPAD_LEFT | DPAD_RIGHT | SELECT_BUTTON))
				{
                    case A_BUTTON:
                        {
                        // check selection is valid. Play sound if invalid
                        u16 species = (*DNavState)->selectedArr ? (*DNavState)->waterSpecies[(*DNavState)->selectedIndex >> 1] : (*DNavState)->grassSpecies[(*DNavState)->selectedIndex>>1];
                        // if species is MISSINGNO then error
                        if (species)
						{
                            // species was valid, save and enter OW HUD mode
                            Var8000 = species;
                            Var8001 = (*DNavState)->selectedArr;
                            SetCallback1(DexNavGuiExitSearch);
                            gMain.state = 0;
                            return;
                        } 
						else
						{
                            // beep and update
                            DexNavLoadNames(0);
                            PlaySE(SE_JITE_PYOKO);
                        }
                        break;
                        }
                    case B_BUTTON:
                        // exit to start menu
                        SetCallback1(DexNavGuiExitNoSearch);
                        gMain.state = 0;
                        return;
                    case DPAD_DOWN:
                    case DPAD_UP:
                        (*DNavState)->selectedArr = !(*DNavState)->selectedArr;
                        (*DNavState)->selectedIndex = 0;
                        DexNavLoadNames(1);
                        break;
                    case DPAD_LEFT:
                        // wrap cursor around
                        if ((*DNavState)->selectedArr)
                            (*DNavState)->selectedIndex = (*DNavState)->selectedIndex == 0 ? 4 * 2 : (*DNavState)->selectedIndex - 2;
						else
                            (*DNavState)->selectedIndex = (*DNavState)->selectedIndex == 0 ? 11 * 2 : (*DNavState)->selectedIndex - 2;
                        DexNavLoadNames(1);
                        break;
                    case DPAD_RIGHT:
                        if ((*DNavState)->selectedArr)
                            // don't go past index 4 in water
                            (*DNavState)->selectedIndex = (*DNavState)->selectedIndex == 4 * 2 ? 0 : (*DNavState)->selectedIndex + 2;
						else
                            // don't go past index 11 in grass
                            (*DNavState)->selectedIndex = (*DNavState)->selectedIndex == 11 * 2 ? 0 : (*DNavState)->selectedIndex + 2;
                        DexNavLoadNames(1);
                        break;
                    case SELECT_BUTTON:
                    {
                        // check selection is valid. Play sound if invalid
                        u16 species = (*DNavState)->selectedArr ? (*DNavState)->waterSpecies[(*DNavState)->selectedIndex >> 1] : (*DNavState)->grassSpecies[(*DNavState)->selectedIndex>>1];
                        // if species is MISSINGNO then error
                        if (species)
						{
                            // species was valid
                            DexNavLoadNames(2);
                            PlaySE(SE_PC_ON);
                            // create value to store in a var
                            u16 varStore = ((*DNavState)->selectedArr << 15) | species;
                            VarSet(DEXNAV_VAR, varStore);
                        } 
						else
						{
                            // beep and update
                            DexNavLoadNames(0);
                            PlaySE(SE_JITE_PYOKO);
                        }
                        break;
                    }
                    default:
                        return;
                };
                UpdateCursorPosition();
                PlaySE(SE_SELECT);
            }
            break;
        default:
            break;
    };
};




// ============================= //
// ===== Overworld Effects ===== //
// ============================= //
void OeiCaveEffect(void)
{
    //struct SpritePalette caveSmoke = {(const u8*)&gInterfaceGfx_caveSmokePal, 0x1005};
	struct SpritePalette caveSmoke = {(void*) &gInterfaceGfx_caveSmokePal, 0x1005};
    GpuPalObjAllocTagAndApply(&caveSmoke);
    GpuPalApply((u8 *)&gInterfaceGfx_caveSmokePal, 29 * 16, 32);
    LogCoordsCameraRelative(&gFieldEffectArguments->effectPos.x, &gFieldEffectArguments->effectPos.y, 8, 8);
    u8 objId = TemplateInstanciateReverseSearch(&ObjtCave, gFieldEffectArguments->effectPos.x, gFieldEffectArguments->effectPos.y, 0xFF);
    if (objId != 64)
	{
        gSprites[objId].coordOffsetEnabled = TRUE;
        gSprites[objId].data[0] = 22;
    }
};

const struct OieState2 caveState = { .p = (const struct SpritePalette*) 0x083A5348,
                                        .s = (SuperCallback) OeiCaveEffect,
                                        };


// ========================================== //
// ======== Check Select in Overworld ======= //
// ========================================== //
u8 SelectHookRestore(void)
{
    ScriptContext1_SetupScript((void*) 0x81A77A0); // script that lets you know you can register item via select
    return 0;
};


u8 CheckRegisteredSelect(void)
{
    u16 varVal = VarGet(DEXNAV_VAR);
    if (varVal)
	{
        if (FuncIsActiveTask(DexNavManageHUD))
            return 0;	// don't redo the search...
        InitDexNavHUD(varVal & 0x7FFF, varVal >> 15);
        return 0;
    }
    return 1;
};


// ============================== //
// ========= POKETOOLS ========== //
// ============================== //
void ToolSelection(u8 taskId)
{    
    switch (priv0)
	{
        case 0:
            if (ScriptEnv2IsEnabled())
                return;
            priv0++;
			break;
        case 1:
            ScriptContext2_Enable();
            u8 boxId = AddWindow(&Tbox);
            FillWindowPixelBuffer(boxId, 0x11);
            WindowOutline(boxId, 1, 0x214, 0xE);
            WindowPrint(boxId, 1, 8, 2, &MenuTextBlack, 0, &gText_DexNavText[0]);
            ChoiceSetupSimple(boxId, 2, 0, 1, 16, TOOL_COUNT, 0);
            CopyWindowToVram(boxId, 3);
            PutWindowTilemap(boxId);
            gTasks[taskId].data[1] = boxId;
            priv0++;
            break;
        case 2:
		{
            s8 choice = RboxChoiceUpdate();
            if (choice == 0)
			{
                //pokedex
                StartMenuPokedexFunc();
                DestroyTask(taskId);
            } 
			else if (choice == 1)
			{
                // dexnav
                ExecDexNav();
                DestroyTask(taskId);
            } 
			else if (choice == -1)
			{
                // b pressed, exit
                RboxIdClean(gTasks[taskId].data[1], 1);
                RemoveWindow(gTasks[taskId].data[1]);
                ScriptEnvDisable();
                DestroyTask(taskId);
            }
            break;
		}
    }
	return;
};



void CloseStartMenu(void)
{
	CloseSafariStepsBox();	// void safari_stepscount_close(void) 0806EF18
	CloseStartMenuDescriptionBox();	// void sm_close_description(void) 080F7998
	HideStartMenu();		// void sm_close_menu(void) 0806FEA0
}



u8 PokeToolsFunc(void)
{
    CloseStartMenu();
    CreateTask(ToolSelection, 0);
    return 1;
}







