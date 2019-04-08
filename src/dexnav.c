#include "defines.h"
#include "../include/new/dexnav.h"

/*
Simplified DexNav System
	credits to FBI: https://github.com/EternalCode/Dexnav
*/


// FUNCTIONS
//void outlined_font_draw(u8 obj_id, u8 tile_num, u16 size);


/*

void task_delete_completed_textbox(u8 tId)
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
   gBoxStatusAndType = 1;
   CreateTask((TaskFunc)task_delete_completed_textbox, 0x1);
   ScriptContext2_Enable();
   return;
};


u16 rand_range(u16 min, u16 max)
{
    if (min == max)
		return min;
    return (rand() % (max - min)) + min;
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
	SetVBlankCallback(0);
	SetHBlankCallback(0);
	SetCallback1(0);
	SetCallback2(0);
};

void DexNavGUICallback2(void)
{
    BuildOamBuffer();
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
    // callbacks
    ClearHandlers();
    // BGs
    ResetBgSettings();
    // pals
    ResetPalSettings();
    // objs
    obj_and_aux_reset_all();
    gpu_tile_obj_tags_reset();
    // VRAM clear
    u32 set = 0;
    CpuFastSet((void*)&set, (void*)ADDR_VRAM, CPUModeFS(0x10000, CPUFSSET));
    // tasks
    Malloc_init((void*)0x2000000, 0x1C000);
    tasks_init();
};


// ===================================== //
// ================ HUD ================ //
// ===================================== // 
void InitDexNavHUD(u16 species, u8 environment)
{
    *DNavState = (struct DexnavHudData*)Calloc(sizeof(struct DexnavHudData));
    // assign non-objects to struct
    (*DNavState)->species = species;
    (*DNavState)->environment = environment;
    u8 search_level = SearchLevels[species];
    (*DNavState)->search_level = search_level;
    (*DNavState)->pokemon_level = dexnav_generate_pokemonlvl(species, search_level, environment);


    if ((*DNavState)->pokemon_level < 1)
	{
        free((void*)*DNavState);
        // pchar empty_string[] = _("This Pokemon cannot be found here.\p");
        MsgNormal(empty_string);
        return;
    }

    // draw shaking grass
    extern u8 shaking_grass(u8, u8, u8, bool8);
    if (!shaking_grass(environment, 12, 12, 0))
	{
        free((void*)*DNavState);
        //pchar empty_string[] = _("It couldn’t be found nearby.\nTry looking in a different area!\p");
        MsgNormal(empty_string);
        return;
    }

    // Populate DNavState objects
    dexnav_generate_move(species, search_level, (*DNavState)->pokemon_level, &(*DNavState)->move_id[0]);
    (*DNavState)->held_item = dexnav_generate_helditem(species, search_level);
    (*DNavState)->ability = dexnav_generate_hiddenability(species, search_level);
    (*DNavState)->potential = dexnav_generate_potential(search_level);
    dexnav_proximity_update();

    // draw icons
    dexnav_draw_icons();

    // hide icons based on proximity and search level
    dexnav_icons_vision_update((*DNavState)->proximity, search_level);

    // enable Hblank interrupt
    interrupts_enable(2);
    SetHBlankCallback((SuperCallback)dexhud_hblank);

    // task update HUD
    u8 t_id = task_add((TaskFunc)DexNavManageHUD, 0x1);
    tasks[t_id].priv[0] = gSprites[walkrun_state.oamid].pos1.x;

};



// This is called via a c1 from the GUI, while waiting to return to the OW
void ExecDexNav_hud() {
    if (!gPaletteFade->active && !script_env_2_is_enabled() && super.callback2 == c2_overworld)
	{
        SetCallback1(c1_overworld);
        InitDexNavHUD(Var8000, Var8001);
    }
};


// ============================= //
// ========= HUD ICONS ========= //
// ============================= //
void dexnav_draw_ability(enum PokemonAbility ability, u8* objid)
{
    // create empty object of size 32x64 to draw font on
    struct SpriteTiles fontsprite_ability = {(const u8*)(&emptyTiles), 0x800, 0x1EE7};
    struct SpriteTemplate font_temp_ability = {0x1EE7, 0x8472, &fontOAM, nullframe, 0x0,
                                nullrsf, (ObjectCallback)0x800760D};
    GpuTileObjAllocTagAndUpload(&fontsprite_ability);
    GpuPalObjAllocTagAndApply(&heldPal);
    u8 obj_id = CreateSprite(&font_temp_ability, ICONX + 80, ICONY + 0x12, 0x0);
    *objid = obj_id;
    gSprites[obj_id].final_oam.affine_mode = 2;

    // ability name beside move name
    u8 len = (*DNavState)->move_name_length;
    gSprites[obj_id].pos1.x += ((8 * (len/2)) + (4 * (len % 2)));

    // Copy ability string from table using state id
    memcpy((void *)(string_buffer), (void *)&(pokemon_ability_names[ability]), POKEAGB_ABILITY_NAME_LENGTH);

    // format string so it's even length or if it's odd ends in two spaces
    string_buffer[POKEAGB_ABILITY_NAME_LENGTH + 1] = 0xFF;
    len = pstrlen(string_buffer);
    if (!(len % 2)) 
	{
        string_buffer[len] = 0x0;
        string_buffer[len + 1] = 0x0;
        string_buffer[len + 2] = 0xFF;
    }

    // write name to object
    outlined_font_draw(obj_id, 0, 32 * 8);
    gSprites[obj_id].final_oam.obj_mode = 1;
    return;
};



void dexnav_draw_move(u16 move, u8 search_level, u8* objid)
{
    // create empty object of size 32x64 to draw font on
    struct SpriteTiles fontsprite_move = {(const u8*)(&emptyTiles), 0x800, 0x4736};
    struct SpriteTemplate font_temp_move = {0x4736, 0x8472, &fontOAM, nullframe, 0x0,
                            nullrsf, (ObjectCallback)0x800760D};
    GpuTileObjAllocTagAndUpload(&fontsprite_move);
    GpuPalObjAllocTagAndApply(&heldPal);
    u8 obj_id = CreateSprite(&font_temp_move, ICONX + 80, ICONY + 0x12, 0x0);
    *objid = obj_id;
    gSprites[obj_id].final_oam.affine_mode = 2;

    // Copy move string from table using state id, add '/' character to the end of it
    memcpy((void *)string_buffer, (void *)pokemon_move_names[move], POKEAGB_MOVE_NAME_LENGTH);
    string_buffer[POKEAGB_MOVE_NAME_LENGTH + 1] = 0xFF;
    u8 len = pstrlen(string_buffer);

    if (search_level > 2) 
	{
        string_buffer[len] = 0xBA;
        len += 1;
    }

    // record length of move with slash for ability name to be placed beside it
    (*DNavState)->move_name_length = len;

    // adjust string to be even chars, if odd end in two spaces
    if (!(len % 2)) 
	{
        string_buffer[len] = 0x0;
        string_buffer[len + 1] = 0x0;
        string_buffer[len + 2] = 0xFF;
    } 
	else 
	{
        string_buffer[len] = 0xFF;
    }

    // write name to object
    outlined_font_draw(obj_id, 0, 32 * 8);
    gSprites[obj_id].final_oam.obj_mode = 1;
};


void dexnav_draw_potential(u8 potential, u8* objid) 
{
    // 19 tiles per row, stars are on the 4th row. 1 tile is 32 bytes. Hence 19 * 4 *32
    struct SpriteTiles staricon_lit = {(const u8*)(&(dexnav_starsTiles[19 * 4 * 32])), 64, 0x61};
    struct SpriteTiles staricon_off = {(const u8*)(&(dexnav_starsTiles[((19 * 4) + 1) *32])), 64, 0x2613};

    // TODO: put these function pointers and other data ptrs in PokeAGB.
    struct SpriteTemplate star_lit_temp = {0x61, 0x8472, &held_oam, nullframe, 0x0,
                                        nullrsf, (ObjectCallback)0x800760D};
    struct SpriteTemplate star_off_temp = {0x2613, 0x8472, &held_oam, nullframe, 0x0,
                                        nullrsf, (ObjectCallback)0x800760D};

    // allocate both the lit and unlit star to VRAM
    GpuTileObjAllocTagAndUpload(&staricon_lit);
    GpuTileObjAllocTagAndUpload(&staricon_off);
    GpuPalObjAllocTagAndApply(&heldPal);

    // create star objects and space them according to potential 0 - 3
    u8 obj_id;
    u8 i;
    for (i = 0; i < 3; ++i)
	{
        if (potential > i)
		{
            obj_id = CreateSprite(&star_lit_temp, ICONX + 23 + (i * 8), ICONY + 0x5, 0x0);
        } 
		else 
		{
            obj_id = CreateSprite(&star_off_temp, ICONX + 23 + (i * 8), ICONY + 0x5, 0x0);
        }
        objid[i] = obj_id;
        gSprites[obj_id].final_oam.affine_mode = 2;
        gSprites[obj_id].final_oam.obj_mode = 1;
    }
};


void dexnav_draw_sight(u8 sight_lvl, u8* objid)
{
    // create empty object of size 64x32 to draw icons on
    struct SpriteTiles sight_tiles = {(const u8*)(&emptyTiles), 0x800, 0x5424};
    struct SpriteTemplate fontTempSight = {0x5424, 0x8472, &fontOAM, nullframe, 0x0,
                                            nullrsf, (ObjectCallback)0x800760D};
    GpuTileObjAllocTagAndUpload(&sight_tiles);
    GpuPalObjAllocTagAndApply(&heldPal);
    u8 obj_id = CreateSprite(&fontTempSight, ICONX + 192, ICONY + 0x12, 0x0);
    *objid = obj_id;
    gSprites[obj_id].final_oam.affine_mode = 2;
    extern void dexnav_sight_update(u8);
    dexnav_sight_update(sight_lvl);
    gSprites[obj_id].final_oam.obj_mode = 1;
};


void dexnav_sight_update(u8 sight)
{
    u8 obj_id = (*DNavState)->obj_id_sight;
    // draw sight eye on first tile, takes up two tiles
    u8 tileid = gSprites[obj_id].final_oam.tile_num;
    u8* towrite = (u8*)((tileid * 32) + (SPRITE_RAM));
    memcpy((void*)towrite, (void*) &(dexnav_starsTiles[((19 * 4) + (7 - (2 * sight))) * 32]), 64);

    // draw the B button tile
    memcpy((void*)(towrite + 128), (void*) &(dexnav_starsTiles[((19 * 4) + 2) *32]), 64);

    // draw info text on the 5th tile
    //pchar back[] = _(" Back  ");
    pstrcpy(string_buffer, back);
    outlined_font_draw(obj_id, 5, 32 * 8);
};


void dexnav_draw_helditem(u8* objid)
{
    // create object for held item icon
    struct SpriteTiles heldicon = {(const u8*)(&gfx_heldicons), 64, 0x8472};
    struct SpriteTemplate heldtemp = {0x8472, 0x8472, &held_oam, nullframe, 0x0,
                                    nullrsf, (ObjectCallback)0x800760D};
    GpuTileObjAllocTagAndUpload(&heldicon);
    GpuPalObjAllocTagAndApply(&heldPal);
    u8 obj_id = CreateSprite(&heldtemp, ICONX + 0x8, ICONY + 0xC, 0x0);
    *objid = obj_id;
    gSprites[obj_id].final_oam.affine_mode = 2;
    gSprites[obj_id].final_oam.obj_mode = 1;
};


void dexnav_draw_speciesicon(u16 species, u8* objid)
{

    // check which palette the species icon uses
    u8 icon_pal = pokeicon_pal_indices[species];
    struct SpritePalette bulbpal = {(u8*)&(pokeicon_pals[icon_pal]), 0x3139};
    GpuPalObjAllocTagAndApply(&bulbpal);

    u32 pid = rand_range(0, 0xFFFF) | rand_range(0, 0xFFFF) << 16;
    void *icon_gfx = load_party_icon_tiles_with_form(species, pid, false);
    struct SpriteTiles bulbicon = {icon_gfx, 4 * 8 * 32, 0x3139};
    GpuTileObjAllocTagAndUpload(&bulbicon);

    // create object
    u8 obj_id = CreateSprite(&bulbtemp, ICONX, ICONY, 0x0);
    *objid = obj_id;
    //dprintf("address is: %x\n", objects[obj_id].image);
    dprintf("address is: %x\n", &picon_oam);
    gSprites[obj_id].final_oam.affine_mode = 2;
    gSprites[obj_id].final_oam.obj_mode = 1;
};


void dexnav_draw_icons()
{
    u8 search_level = (*DNavState)->search_level;
    dexnav_draw_sight((*DNavState)->proximity, &(*DNavState)->obj_id_sight);
    dexnav_draw_speciesicon((*DNavState)->species, &(*DNavState)->obj_id_species);
    dexnav_draw_move((*DNavState)->move_id[0], search_level, &(*DNavState)->obj_id_move);
    dexnav_draw_ability((*DNavState)->ability, &(*DNavState)->obj_id_ability);
    dexnav_draw_helditem(&(*DNavState)->obj_id_item);
    dexnav_draw_potential((*DNavState)->potential, &(*DNavState)->obj_id_potential[0]);
};


void outlined_font_draw(u8 obj_id, u8 tile_num, u16 size)
{
    u8 tile = gSprites[obj_id].final_oam.tile_num + tile_num;
    u8* towrite = (u8*)((tile * TILE_SIZE) + (SPRITE_RAM));
    //u8* dst = (u8*)Malloc(size + TILE_SIZE);
    u8 *dst = (u8*)0x202402C;
    pchar *string_buff_ptr = string_buffer;
    pchar element = *string_buff_ptr;
    u8 counter = 1;
    u16 index = 320;
    u16 prev_index;
    while (element != 0xFF)
	{
        prev_index = index;
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
            u8 symbol_id = 0;
            switch (element) 
			{
                case 0xF0: // colon
                case 0x0: // space bar
                {
                    symbol_id = 1;
                    break;
                }
                case 0x36: // semi colon used indication of str end
                {
                    symbol_id = 2;
                    break;
                }
                case 0xAC: // question mark
                {
                    symbol_id = 3;
                    break;
                }
                case 0xAE: // dash
                {
                    symbol_id = 4;
                    break;
                }
                case 0xAD: // period
                {
                    symbol_id = 5;
                    break;
                }
                case 0xBA: // slash
                {
                    symbol_id = 6;
                    break;
                }
                case 0xB1: // open double quote
                {
                    symbol_id = 7;
                    break;
                }
                case 0xB2: // close double quote
                {
                    symbol_id = 8;
                    break;
                }
                case 0xB3: // open single quote
                {
                    symbol_id = 9;
                    break;
                }
                case 0xB4: // close single quote
                {
                    symbol_id = 10;
                    break;
                }
                case 0xB0: // elipsis ...
                {
                    symbol_id = 11;
                    break;
                }
                case 0xB8: // comma
                {
                    symbol_id = 12;
                    break;
                }
                case 0xB5: // male
                {
                    symbol_id = 13;
                    //dst =
                    break;
                }
                case 0xB6: // f
                {
                    symbol_id = 14;
                    break;
                }
                case 0xFF: // empty
                {
                    symbol_id = 1;
                    break;
                }
            };
            index = (symbol_id + 9) * TILE_SIZE;
        }

        // TODO: Use macros here //

        if ((counter == 0) || (*(string_buff_ptr + 1) == 0xFF))
		{
            // first or last pcharacters don't need pixel merging
            memcpy((void*)dst, (void*)(&dexnav_starsTiles[index]), TILE_SIZE);
        }
		else if ((element == 0x0))
		{
            memcpy((void*)dst, (void*)(&dexnav_starsTiles[index]), TILE_SIZE);
            u8 *prev_letter = (u8*)(&dexnav_starsTiles[prev_index]);
            *(dst + 0) = *(prev_letter + 2);
            *(dst + 4) = *(prev_letter + 6);
            *(dst + 8) = *(prev_letter + 10);
            *(dst + 12) = *(prev_letter + 14);
            *(dst + 16) = *(prev_letter + 18);
            *(dst + 20) = *(prev_letter + 22);
            *(dst + 24) = *(prev_letter + 26);
            *(dst + 28) = *(prev_letter + 30);
        }
		else if ((*(string_buff_ptr + 1) != 0xFF))
		{

            // pcharacter in middle, if blank space fill blank with previous pcharacter's last pixel row IFF previous pchar's last pixel row non-empty
            memcpy((void*)dst, (void*)(&dexnav_starsTiles[index]), TILE_SIZE);
            u8 *prev_letter = (u8*)(&dexnav_starsTiles[prev_index]);
            *(dst) |= (((*(prev_letter + 0) & 0xF) == 0) ? (*(dst + 0) & 0xF) : (*(prev_letter + 0) & 0xF));
            *(dst + 4) |= (((*(prev_letter + 4) & 0xF) == 0) ? (*(dst + 4) & 0xF) : (*(prev_letter + 4) & 0xF));
            *(dst + 8) |= (((*(prev_letter + 8) & 0xF) == 0) ? (*(dst + 8) & 0xF) : (*(prev_letter + 8) & 0xF));
            *(dst + 12) |= (((*(prev_letter + 12) & 0xF) == 0) ? (*(dst + 12) & 0xF) : (*(prev_letter + 12) & 0xF));
            *(dst + 16) |= (((*(prev_letter + 16) & 0xF) == 0) ? (*(dst + 16) & 0xF) : (*(prev_letter + 16) & 0xF));
            *(dst + 20) |= (((*(prev_letter + 20) & 0xF) == 0) ? (*(dst + 20) & 0xF) : (*(prev_letter + 20) & 0xF));
            *(dst + 24) |= (((*(prev_letter + 24) & 0xF) == 0) ? (*(dst + 24) & 0xF) : (*(prev_letter + 24) & 0xF));
            *(dst + 28) |= (((*(prev_letter + 28) & 0xF) == 0) ? (*(dst + 28) & 0xF) : (*(prev_letter + 28) & 0xF));
        }

        if ((counter == 2) && (*(string_buff_ptr + 1) != 0xFF))
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
        string_buff_ptr++;
        element = *string_buff_ptr;
    }
    memcpy((void*)towrite, (void*)(0x202402C), size);
    Memset((void*)(0x202402C), 0x0, size + TILE_SIZE);
};


// ============================= //
// ======== HUD UPDATER ======== //
// ============================= //
void dexnav_message_show(u8 id)
{
    switch(id) {
        case 0:
        {
            //extern void MsgNormal(pchar *);
            //pchar empty_string[] = _("The Pokémon got away!\p");
            MsgNormal(gText_GotAway);
            break;
        }
        case 1:
        {
            //extern void MsgNormal(pchar *);
            //pchar empty_string[] = _("There is no reaction.\nThe signal was lost!\p");
            MsgNormal(gText_LostSignal);
            break;
        }
    };
    return;
};


void dexhud_hblank(void)
{
    if (REG_VCOUNT > (160 - 19))
	{ // fill 19 pixels

        //WRITE_REG_WININ(0x3F3F);
        REG_WININ = WININ_BUILD(WIN_BG0 | WIN_BG1 | WIN_BG2 | WIN_BG3 | WIN_OBJ | WIN_BLD,  WIN_BG0 | WIN_BG1 | WIN_BG2 | WIN_BG3 | WIN_OBJ | WIN_BLD);

        //WRITE_REG_BLDCNT(0x0FFE);
        REG_BLDCNT = (BLDCNT_BG1_SRC | BLDCNT_BG2_SRC | BLDCNT_BG3_SRC | BLDCNT_SPRITES_SRC | BLDCNT_BACKDROP_SRC | BLDCNT_BG0_DST | BLDCNT_BG1_DST | BLDCNT_BG2_DST | BLDCNT_BG3_DST | BLDCNT_DARKEN);
        REG_BLDY = 0xC;


    } 
	else
	{
        // default values from overworld
        // WRITE_REG_WININ(0x1F1F);
        REG_WININ = WININ_BUILD(WIN_BG0 | WIN_BG1 | WIN_BG2 | WIN_BG3 | WIN_OBJ, WIN_BG0 |
                        WIN_BG1 | WIN_BG2 | WIN_BG3 | WIN_OBJ);
        //WRITE_REG_BLDCNT(0x401E);
        REG_BLDCNT = BLDALPHA_BUILD(BLDCNT_BG1_SRC | BLDCNT_BG2_SRC | BLDCNT_BG3_SRC | BLDCNT_SPRITES_SRC, 0);
    }
};


void dexnav_proximity_update(void)
{
    extern u8 GetPlayerDistance(s16 x, s16 y);
    (*DNavState)->proximity =  GetPlayerDistance((*DNavState)->tile_x, (*DNavState)->tile_y);
};


void dexnav_icons_vision_update(u8 proximity, u8 search_level)
{
     //   The sight eye and species icon are always drawn in the HUD
       // At search level 2, first move is drawn
       // At search level 3, Ability and Held item are drawn
       // At search level 5, Potential is drawn
    

    // species and sight/info are always shown, regardless of proximity
    gSprites[(*DNavState)->obj_id_species].final_oam.affine_mode = 0;
    gSprites[(*DNavState)->obj_id_sight].final_oam.affine_mode = 0;

    // hide everything at the start, and recalc what needs to be shown
    gSprites[(*DNavState)->obj_id_move].final_oam.affine_mode = 2;
    gSprites[(*DNavState)->obj_id_ability].final_oam.affine_mode = 2;
    gSprites[(*DNavState)->obj_id_item].final_oam.affine_mode = 2;
    gSprites[(*DNavState)->obj_id_potential[0]].final_oam.affine_mode = 2;
    gSprites[(*DNavState)->obj_id_potential[1]].final_oam.affine_mode = 2;
    gSprites[(*DNavState)->obj_id_potential[2]].final_oam.affine_mode = 2;

    extern void dexnav_sight_update(u8);
    if (proximity < 3)
	{
        // at proximity 5, the sight should start to show and indicate sneaking is required
        dexnav_sight_update(0); // alert level TODO: Update in a better way utiliting sight_level
        if (search_level > 1)
		{
            // show move, hide others
            gSprites[(*DNavState)->obj_id_move].final_oam.affine_mode = 0;
        }
        if (search_level > 2)
		{
            // show ability, move, hide others
            gSprites[(*DNavState)->obj_id_ability].final_oam.affine_mode = 0;
            if ((*DNavState)->held_item)
			{
                // toggle item view
                gSprites[(*DNavState)->obj_id_item].final_oam.affine_mode = 0;
            }
        }
        if (search_level > 4)
		{
            gSprites[(*DNavState)->obj_id_potential[0]].final_oam.affine_mode = 0;
            gSprites[(*DNavState)->obj_id_potential[1]].final_oam.affine_mode = 0;
            gSprites[(*DNavState)->obj_id_potential[2]].final_oam.affine_mode = 0;
        }

    } 
	else if (proximity < 6)
	{
        dexnav_sight_update(1); // Sneaking is required flag
    } 
	else
	{
        dexnav_sight_update(2); // Sneaking is not required
    }
};

void nullsubhblank(void)
{
    return;
};


void dexnav_hud_free(void)
{
    switch ((*DNavState)->environment)
	{
        case 0:
            {
            if (!is_light_level_1_2_3_or_6__opensky(currentmap_header.light))
			{
                // cave
                oe_stop(&gSprites[(*DNavState)->obj_id_shaking_grass], 0x1A); //1a
            } else {
                oe_stop(&gSprites[(*DNavState)->obj_id_shaking_grass], 0x13);
            }
            break;
            }
        case 1:
            oe_stop(&gSprites[(*DNavState)->obj_id_shaking_grass], 0x13);
            break;
    };
    obj_free(&gSprites[(*DNavState)->obj_id_species]);
    obj_delete(&gSprites[(*DNavState)->obj_id_species]);

    obj_free(&gSprites[(*DNavState)->obj_id_sight]);
    obj_delete(&gSprites[(*DNavState)->obj_id_sight]);

    obj_free(&gSprites[(*DNavState)->obj_id_ability]);
    obj_delete(&gSprites[(*DNavState)->obj_id_ability]);

    obj_free(&gSprites[(*DNavState)->obj_id_move]);
    obj_delete(&gSprites[(*DNavState)->obj_id_move]);

    obj_free(&gSprites[(*DNavState)->obj_id_item]);
    obj_delete(&gSprites[(*DNavState)->obj_id_item]);

    obj_free(&gSprites[(*DNavState)->obj_id_potential[0]]);
    obj_delete(&gSprites[(*DNavState)->obj_id_potential[0]]);

    obj_free(&gSprites[(*DNavState)->obj_id_potential[1]]);
    obj_delete(&gSprites[(*DNavState)->obj_id_potential[1]]);

    obj_free(&gSprites[(*DNavState)->obj_id_potential[2]]);
    obj_delete(&gSprites[(*DNavState)->obj_id_potential[2]]);

    gpu_tile_obj_free_by_tag(0x4736);
    gpu_tile_obj_free_by_tag(0x61);
    gpu_tile_obj_free_by_tag(0x2613);
    gpu_tile_obj_free_by_tag(0x5424);
    gpu_tile_obj_free_by_tag(0x8472);
    gpu_tile_obj_free_by_tag(0x3039);
    gpu_tile_obj_free_by_tag(0x1EE7);
    gpu_pal_free_by_tag(0x8472);
    gpu_pal_free_by_tag(0x3039);

    free((void *)*DNavState);
    interrupts_disable(2);
    SetHBlankCallback(nullsubhblank);

    // WRITE_REG_WININ(0x1F1F);
    REG_WININ = WININ_BUILD(WIN_BG0 | WIN_BG1 | WIN_BG2 | WIN_BG3 | WIN_OBJ, WIN_BG0 |
                            WIN_BG1 | WIN_BG2 | WIN_BG3 | WIN_OBJ);
    //WRITE_REG_BLDCNT(0x401E);
    REG_BLDCNT = BLDALPHA_BUILD(BLDCNT_BG1_SRC | BLDCNT_BG2_SRC | BLDCNT_BG3_SRC | BLDCNT_SPRITES_SRC, 0);
};


void DexNavManageHUD(u8 taskId)
{
    // check for out of range
    if ((*DNavState)->proximity > 20)
	{
        task_del(taskId);
        dexnav_hud_free();
        dexnav_message_show(1);
        return;
    }

    // check for timeout.
    tasks[taskId].priv[1]++;
    if (tasks[taskId].priv[1] > (0x384))
	{
        task_del(taskId);
        dexnav_hud_free();
        dexnav_message_show(0);
        return;
    }

    // check if script just executed
    if (script_env_2_is_enabled())
	{
        task_del(taskId);
        dexnav_hud_free();
        return;
    }

    // caves and water the pokemon moves around
    if ((((*DNavState)->environment == 1) || (!is_light_level_1_2_3_or_6__opensky(currentmap_header.light)))
    && ((*DNavState)->proximity < 2) && ((*DNavState)->movement_times < 2))
	{
        switch((*DNavState)->environment)
		{
            case 0:
                oe_stop(&gSprites[(*DNavState)->obj_id_shaking_grass], 0x1A); // 1a
                break;
            case 1:
                oe_stop(&gSprites[(*DNavState)->obj_id_shaking_grass], 0x16);
                break;
            default:
                break;
        };
        extern u8 shaking_grass(u8, u8, u8, bool8);
        while(!(shaking_grass((*DNavState)->environment, 8, 8, 1)))
		{
            __asm__("mov r8, r8"); // nop...do I even need this?
        }
        (*DNavState)->movement_times += 1;
    }

    // check for encounter start
    if ((*DNavState)-> proximity < 1)
	{
        extern void dexnav_gen_pkmnn(u16 species, u8 potential, u8 level, u8 ability, u16* moves);
        dexnav_gen_pkmnn((*DNavState)->species, (*DNavState)->potential, (*DNavState)->pokemon_level,
        (*DNavState)->ability, (*DNavState)->move_id);
        task_del(taskId);

        // increment the search level
        if (SearchLevels[(*DNavState)->species] < 100)
		{
            SearchLevels[(*DNavState)->species] += 1;
        }

        // freeing only the state, objects and hblank cleared on battle start.
        free((void *)*DNavState);

        // exclamation point animation over the player
        an_exclamation_mark(npc_states, &gSprites[walkrun_state.oamid]);
        oe_exec(0x0);

        // do battle
        exec_battle();


    };

    // TODO : CHECK SNEAKING
    // HUD needs updating iff player has moved
    dexnav_proximity_update();
    if (!(tasks[taskId].priv[0] == (*DNavState)->proximity))
	{
        dexnav_icons_vision_update((*DNavState)->proximity, (*DNavState)->search_level);
        tasks[taskId].priv[0] = (*DNavState)->proximity;
    }

};


// ============================== //
// ===== GUI
// ============================== //
// Entry point into dexnav + dexnav gui 
u8 ExecDexNav(void)
{
    BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 16, 0x0000);
    void DexNavGuiHandler(void);
    SetCallback1(DexNavGuiHandler);
    gMain.state = 0;
    return true;
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
    bg_vram_setup(0, (struct BgTemplate *)&BgConfigDexNavGUI, 4);
    u32 set = 0;
    CpuFastSet((void*)&set, (void*)ADDR_VRAM, CPUModeFS(0x10000, CPUFSSET));
    gpu_sync_bg_hide(1);
    gpu_sync_bg_hide(0);
    SetCallback2(DexNavGUICallback2);
    SetVBlankCallback(VblackCallbackSeq);
};

void DexNavLoadPokeIcons()
{
    // allocate all pallettes
    for (u32 i = 0; i < 3; ++i)
	{
        struct SpritePalette pal = {&pokeicon_pals[i], ICON_PAL_TAG + i};
        GpuPalObjAllocTagAndApply(&pal);
    }

    for (u8 i = 0; i < 12; ++i)
	{
        u16 species = (*DNavState)->grass_species[i];
        u32 pid = 0xFFFFFFFF;
        //if (species > 0) {
            void *icon_gfx = load_party_icon_tiles_with_form(species, pid, false);
            u16 gfx_tag = ICON_GFX_TAG + i;
            u16 pal_tag = ICON_PAL_TAG + pokeicon_pal_indices[species];
            struct SpriteTiles icon_tiles = {icon_gfx, 4 * 8 * 32, gfx_tag};
            GpuTileObjAllocTagAndUpload(&icon_tiles);
            struct SpriteTemplate icon_template = {
                                            .tiles_tag = gfx_tag,
                                            .pal_tag = pal_tag,
                                            .oam = &iconOAM,
                                            .animation = (const struct Frame (**)[])0x8231CF0,
                                            .graphics = &icon_tiles,
                                            .rotscale = (const struct RotscaleFrame (**)[])0x8231CFC,
                                            .callback = oac_nullsub,
                                            };
                                                                // x and y pos of sprite
            CreateSprite(&icon_template, 20 + (24 * (i % 6)), 92 + (i > 5 ? 28 : 0), 0);
        //}
    }
    for (u8 i = 0; i < 5; ++i)
	{
        u16 species = (*DNavState)->water_species[i];
        u32 pid = 0xFFFFFFFF;
        //if (species > 0) {
            void *icon_gfx = load_party_icon_tiles_with_form(species, pid, false);
            u16 gfx_tag = ICON_GFX_TAG + i + 12;
            u16 pal_tag = ICON_PAL_TAG + pokeicon_pal_indices[species];
            struct SpriteTiles icon_tiles = {icon_gfx, 4 * 8 * 32, gfx_tag};
            GpuTileObjAllocTagAndUpload(&icon_tiles);
            struct SpriteTemplate icon_template = {
                                            .tiles_tag = gfx_tag,
                                            .pal_tag = pal_tag,
                                            .oam = &iconOAM,
                                            .animation = (const struct Frame (**)[])0x8231CF0,
                                            .graphics = &icon_tiles,
                                            .rotscale = (const struct RotscaleFrame (**)[])0x8231CFC,
                                            .callback = oac_nullsub,
                                            };
                                                                // x and y pos of sprite
            CreateSprite(&icon_template, 30 + 24 * i, 48, 0);
        //}
    }
};


void update_cursor_position(void)
{
    // update cursor position
    if ((*DNavState)->selected_arr)
	{
        // water
        gSprites[(*DNavState)->cursor_id].pos1.x = cursor_positions2[(*DNavState)->selected_index];
        gSprites[(*DNavState)->cursor_id].pos1.y = cursor_positions2[(*DNavState)->selected_index + 1];
    } 
	else
	{
        // grass
        gSprites[(*DNavState)->cursor_id].pos1.x = cursor_positions1[(*DNavState)->selected_index];
        gSprites[(*DNavState)->cursor_id].pos1.y = cursor_positions1[(*DNavState)->selected_index + 1];
    }
};


void SpawnPointerArrow(void)
{
    struct SpriteTiles cursor_gfx = {(void*)selection_cursorTiles, 32 * 32, SELECTION_CURSOR_TAG};
    struct SpritePalette cursor_pal = {(void*)selection_cursorPal, SELECTION_CURSOR_TAG};
    struct SpriteTemplate cursor_temp = {SELECTION_CURSOR_TAG, SELECTION_CURSOR_TAG, &cursor_oam,
                                    (const struct Frame (**)[])0x8231CF0, &cursor_gfx,
                                    (const struct RotscaleFrame (**)[])0x8231CFC, (ObjectCallback)oac_nullsub};

    // uncompressed
    GpuTileObjAllocTagAndUpload(&cursor_gfx);
    GpuPalObjAllocTagAndApply(&cursor_pal);
    (*DNavState)->cursor_id = CreateSprite(&cursor_temp, 30, 48, 0);
    update_cursor_position();
};


//const pchar no_info[] = _("--------");
//const pchar resting_text[] = _("Choose a Pokémon");
//const pchar invalid_pick_text[] = _("That Pokémon can’t be searched!");
//const pchar select_locked_text[] = _("Pokémon searchable via Select!");

void DexNavLoadNames(u8 status)
{
    // clean boxes
    for (u32 i = 0; i < 5; ++i)
	{
        FillWindowPixelBuffer(i, 0);
    }
	
    // rbox commit species name
    u16 species = (*DNavState)->selected_arr ? (*DNavState)->water_species[(*DNavState)->selected_index >> 1] : (*DNavState)->grass_species[(*DNavState)->selected_index>>1];
    if (species)
        WindowPrint(0, 0, 0, 4, &DexNav_BlackText, 0, pokemon_names[species]);
	else
        WindowPrint(0, 0, 0, 4, &DexNav_BlackText, 0, &no_info[0]);

    // rbox commit search level
    fmt_int_10(string_buffer, SearchLevels[species], 0, 4);
    WindowPrint(1, 0, 0, 4, &DexNav_BlackText, 0, &string_buffer[0]);

    // rbox commit level bonus
    u8 search_level_bonus = 0;
    if ((SearchLevels[species] >> 2) > 20)
        search_level_bonus = 20;
    else
        search_level_bonus = (SearchLevels[species] >> 2);
	
    fmt_int_10(string_buffer, search_level_bonus, 0, 4);
    WindowPrint(2, 0, 0, 4, &DexNav_BlackText, 0, &string_buffer[0]);

    // rbox commit hidden ability name
    if (gBaseStats[species].hidden_ability)
        WindowPrint(3, 0, 0, 4, &DexNav_BlackText, 0, pokemon_ability_names[gBaseStats[species].hidden_ability]);
    else
        WindowPrint(3, 0, 0, 4, &DexNav_BlackText, 0, &no_info[0]);

    // rbox commit status message bar
    switch(status)
	{
        case 0:
            WindowPrint(4, 1, 0, 8, &DexNav_WhiteText, 0, &invalid_pick_text[0]);
            break;
        case 1:
            WindowPrint(4, 1, 0, 8, &DexNav_WhiteText, 0, &resting_text[0]);
            break;
        case 2:
            WindowPrint(4, 1, 0, 8, &DexNav_WhiteText, 0, &select_locked_text[0]);
    }
    // display committed gfx
    for (u8 i = 0; i < 5; ++i)
	{
        CopyWindowToVram(i, 3);
        PutWindowTilemap(i);
    }
};


void c1_dexnavOW(void)
{
    extern void ExecDexNav_hud(void);
    ExecDexNav_hud();
};


void dexnav_gui_exit_search(void)
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
                free((*DNavState)->backbuffer);
                gMain.state++;
            }
            break;
        case 2:
            m4aMPlayVolumeControl(&mplay_BGM, 0xFFFF, 256);
            SetCallback1(c1_dexnavOW);
            SetCallback2(c2_return_overworld_music_scripts_cont);
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
                free((*DNavState)->backbuffer);
                free((void*)*DNavState);
                gMain.state++;
            }
            break;
        case 2:
            m4aMPlayVolumeControl(&mplay_BGM, 0xFFFF, 256);
            SetCallback1(c1_overworld);
            SetCallback2(c2_overworld_switch_start_menu);
            break;
	}
};


//extern void DexNavPopulateEncounterList();

void DexNavGuiHandler(void)
{
    switch(gMain.state)
	{
        case 0:
            if (!gPaletteFade->active)
			{
                DexNavGuiSetup();
                SetCallback1(DexNavGuiHandler);
                // allocate dexnav struct
                *DNavState = (struct DexnavHudData*)Calloc(sizeof(struct DexnavHudData));
                gMain.state++;
            }
            break;
        case 1:
        {
                // load BG assets
                void *DexNav_gBackBuffer = Malloc(0x800);
                (*DNavState)->backbuffer = DexNav_gBackBuffer;
                LoadPalette((void *)(&dnav_text_pal), 15 * 16, 32);
                LoadCompressedPalette((void *)dexnav_guiPal, 0, 32);
                LZ77UnCompWram((void *)dexnav_guiMap, (void *)DexNav_gBackBuffer);
                LZ77UnCompVram((void *)dexnav_guiTiles, (void *)0x06000000);
                BgIdSetTilemap(1, DexNav_gBackBuffer);
                BgIdMarkForSync(1);
                BgIdMarkForSync(0);
                gMain.state++;
        }
            break;
        case 2:
            InitWindows(dexnav_boxes);
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
            (*DNavState)->selected_arr = 0;
            (*DNavState)->selected_index = 0;
            gMain.state++;
            break;
        case 4:
            if (!gPaletteFade->active)
			{
                switch (gMain.newAndRepeatedKeys & (KEY_A | KEY_B | KEY_DOWN | KEY_UP | KEY_LEFT | KEY_RIGHT | KEY_SELECT))
				{
                    case KEY_A:
                        {
                        // check selection is valid. Play sound if invalid
                        u16 species = (*DNavState)->selected_arr ? (*DNavState)->water_species[(*DNavState)->selected_index >> 1] : (*DNavState)->grass_species[(*DNavState)->selected_index>>1];
                        // if species is MISSINGNO then error
                        if (species)
						{
                            // species was valid, save and enter OW HUD mode
                            Var8000 = species;
                            Var8001 = (*DNavState)->selected_arr;
                            SetCallback1(dexnav_gui_exit_search);
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
                    case KEY_B:
                        // exit to start menu
                        SetCallback1(DexNavGuiExitNoSearch);
                        gMain.state = 0;
                        return;
                    case KEY_DOWN:
                    case KEY_UP:
                        (*DNavState)->selected_arr = !(*DNavState)->selected_arr;
                        (*DNavState)->selected_index = 0;
                        DexNavLoadNames(1);
                        break;
                    case KEY_LEFT:
                        // wrap cursor around
                        if ((*DNavState)->selected_arr)
						{
                            (*DNavState)->selected_index = (*DNavState)->selected_index == 0 ? 4 * 2 : (*DNavState)->selected_index - 2;
                        } 
						else
						{
                            (*DNavState)->selected_index = (*DNavState)->selected_index == 0 ? 11 * 2 : (*DNavState)->selected_index - 2;
                        }
                        DexNavLoadNames(1);
                        break;
                    case KEY_RIGHT:
                        if ((*DNavState)->selected_arr)
						{
                            // don't go past index 4 in water
                            (*DNavState)->selected_index = (*DNavState)->selected_index == 4 * 2 ? 0 : (*DNavState)->selected_index + 2;
                        }
						else
						{
                            // don't go past index 11 in grass
                            (*DNavState)->selected_index = (*DNavState)->selected_index == 11 * 2 ? 0 : (*DNavState)->selected_index + 2;
                        }
                        DexNavLoadNames(1);
                        break;
                    case KEY_SELECT:
                    {
                        // check selection is valid. Play sound if invalid
                        u16 species = (*DNavState)->selected_arr ? (*DNavState)->water_species[(*DNavState)->selected_index >> 1] : (*DNavState)->grass_species[(*DNavState)->selected_index>>1];
                        // if species is MISSINGNO then error
                        if (species)
						{
                            // species was valid
                            DexNavLoadNames(2);
                            PlaySE(SE_PC_ON);
                            // create value to store in a var
                            u16 varStore = ((*DNavState)->selected_arr << 15) | species;
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
                update_cursor_position();
                PlaySE(SE_SELECT);
            }
            break;
        default:
            break;
    };
};


// ================== //
// ===GUI SETUP ICONS
// ================== //
bool8 SpeciesInArray(u16 species, u8 indexCount)
{
    // disallow species not seen
    if (!GetIndexFromDexFlag(species, FLAG_GET_SEEN))
        return true;
    for (u8 i = 0; i < indexCount; ++i)
	{
        if (indexCount == 12)
		{
            if ((*DNavState)->grass_species[i] == species)
                return true;
        } 
		else 
		{
            if ((*DNavState)->water_species[i] == species)
                return true;
        }
    }
    return false;
};


void DexNavPopulateEncounterList(void)
{
    // nop struct data
    Memset(&((*DNavState)->grass_species[0]), 0, 34);
    // populate unique wild grass encounters
    u8 insertionIndex = 0;
    u8 index = GetWildDataIndexForMap();

    if (wild_pokemon_data[index].grass_encounter)
	{
        for (u8 i = 0; i < 12; ++i)
		{
            struct wild_pokemon w_pkmn = wild_pokemon_data[index].grass_encounter->wild_encounters->wild_grass[i];
            if (!SpeciesInArray(w_pkmn.species, 12))
			{
                (*DNavState)->grass_species[insertionIndex] = w_pkmn.species;
                insertionIndex++;
            }
        }
    };
    // populate unique wild water encounters
    insertionIndex = 0;
    // exit if no water encounters
    if (wild_pokemon_data[index].water_encounter == NULL)
        return;
    for (u8 i = 0; i < 5; ++i)
	{
        struct wild_pokemon w_pkmn = wild_pokemon_data[index].water_encounter->wild_encounters->wild_water[i];
        if (!SpeciesInArray(w_pkmn.species, 5)) {
            (*DNavState)->water_species[insertionIndex] = w_pkmn.species;
            insertionIndex++;
        }
    }
};




// ============================= //
// ===== Overworld Effects ===== //
// ============================= //
void OeiCaveEffect(void)
{
    struct SpritePalette caveSmoke = {(const u8*)&caveSmokePal, 0x1005};
    GpuPalObjAllocTagAndApply(&caveSmoke);
    GpuPalApply((u8 *)&caveSmokePal, 29 * 16, 32);
    LogCoordsCameraRelative(&gFieldEffectArguments.effect_pos.x, &gFieldEffectArguments.effect_pos.y, 8, 8);
    u8 objId = TemplateInstanciateReverseSearch(&objt_cave, gFieldEffectArguments.effect_pos.x, gFieldEffectArguments.effect_pos.y, 0xFF);
    if (objId != 64)
	{
        gSprites[objId].coordOffsetEnabled = TRUE;
        gSprites[objId].data[0] = 22;
    }
};


// ========================================== //
// ======== Check Select in Overworld ======= //
// ========================================== //
u8 SelectHookRestore(void)
{
    ScriptContext1_SetupScript((void*) 0x81A77A0); // script that lets you know you can register item via select
    return 0;
}


u8 CheckRegisteredSelect(void) {
    u16 varVal = var_load(DEXNAV_VAR);
    if (varVal) {
        if (FuncIsActiveTask(DexNavManageHUD))
		{
            // don't redo the search...
            return 0;
        }
        InitDexNavHUD(varVal & 0x7FFF, varVal >> 15);
        return 0;
    }
    return 1;
}


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
            u8 boxId = AddWindow(&tbox);
            FillWindowPixelBuffer(boxId, 0x11);
            WindowOutline(boxId, 1, 0x214, 0xE);
            WindowPrint(boxId, 1, 8, 2, &MenuTextBlack, 0, gText_DexNavText);
            ChoiceSetupSimple(boxId, 2, 0, 1, 16, TOOL_COUNT, 0);
            CopyWindowToVram(boxId, 3);
            PutWindowTilemap(boxId);
            tasks[taskId].data[1] = boxId;
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
                RboxIdClean(tasks[taskId].data[1], 1);
                RemoveWindow(tasks[taskId].data[1]);
                ScriptEnvDisable();
                DestroyTask(taskId);
            }
            return;
        }
    }
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





// ===== Pokemon Field Tile ===== //
u8 GetPlayerDistance(s16 x, s16 y)
{
    u16 deltaX = ABS(x - (gSaveBlock1->pos.x + 7));
    u16 deltaY = ABS(y - (gSaveBlock1->pos.y + 7));
    return deltaX + deltaY;
}



u8 PickTileScreen(u8 targetBehaviour, u8 areaX, u8 areaY, s16 *xBuff, s16 *yBuff, u8 findChance, u8 smallScan) {
    // area of map to cover starting from camera position {-7, -7}
    s16 topX = gSaveBlock1->pos.x - SCANSTART_X + (smallScan * 5);
    s16 topY = gSaveBlock1->pos.y - SCANSTART_Y + (smallScan * 5);
    s16 botX = topX + areaX;
    s16 botY = topY + areaY;

    // loop through every tile in area and evaluate
    while (topY < botY) {
        while (topX < botX) {
            u32 tileBehaviour = MapGridGetMetatileField(topX, topY, 0xFF);
            u8 blockProperties = MetatileBehavior_GetLowerBytes(tileBehaviour, 4);

            // check NPCs on tile
            bool8 goNext = false;
            for (u8 i = 0; i < NPC_STATE_ID_MAX; ++i)
			{
                if (npc_states[i].to.x == topX && npc_states[i].to.y == topY)
				{
                    goNext = true;
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
                    weight = (((rand() % scale) <= 1) && (cur_mapdata_height_mismatch(npc_states[walkrun_state.oamid].height, topX, topY))) ;
                } 
				else if (!is_light_level_1_2_3_or_6__opensky(currentmap_header.light))
				{
                    // cave basically needs another check to see if the tile is passable
                    u8 scale = 440 - (smallScan * 200) - (GetPlayerDistance(topX, topY) / 2)  - (2 * (topX + topY));
                    var_8002= scale;
                    weight = ((rand() % scale) < 1) && (!MetatileBehavior_GetLowerBytes(tileBehaviour, 6));

                } 
				else 
				{
                    // grass
                    u8 scale = 100 - (GetPlayerDistance(topX, topY) * 2);
                    weight = ((rand() % scale) <= 5);
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


u8 dexnav_picktile(u8 environment, u8 xSize, u8 ySize, bool8 smallScan) {
    // Pick a specific tile based on environment
    u8 targetBehaviour = 0;
    switch (environment) {
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
    u8 search_level = (*DNavState)->search_level / 5;
    u8 chance = search_level + 1;

    return PickTileScreen(targetBehaviour, xSize, ySize, &((*DNavState)->tile_x),
    &((*DNavState)->tile_y) , chance, smallScan);
}



u8 shaking_grass(u8 environment, u8 xSize, u8 ySize, bool8 smallScan) {
    if (dexnav_picktile(environment, xSize, ySize, smallScan))
	{

        gFieldEffectArguments.effect_pos.x = (*DNavState)->tile_x;
        gFieldEffectArguments.effect_pos.y = (*DNavState)->tile_y;
        gFieldEffectArguments.priority = 0xFF; // height.
        switch (environment)
		{
            case 0:
                {
					if (!is_light_level_1_2_3_or_6__opensky(currentmap_header.light))
						oe_exec(OEI_CAVE);
					else
						oe_exec(OEI_GRASS);
					break;
                }
            case 1:
                oe_exec(OEI_WATER);
                break;
            default:
                oe_exec(0x31);
                break;
        };//oe_exec(*d); //16 water, 13 grass

        // get objid of shaking grass
        u8 i;
        for (i = 0; i < 64; ++i)
		{
            if (gSprites[i].callback == (ObjectCallback)0x080DCD1D)
			{
                (*DNavState)->obj_id_shaking_grass = i;
                return 1;
            }
        }
        return 1;
    }
	else
        return 0;	
    return 0;

}


// ===== Dex Nav Pokemon Generator ===== //
u8 count_eggmoves(u16 *eggmove_buff) {
    u8 i;
    u8 count = 0;
    for (i = 0; i < EGGMOVE_MAX; ++i) {
        if (eggmove_buff[i] > MOVE_MAX) {
            return count;
        } else {
            count += 1;
        }
    }
    return count;
};



void populate_eggmovebuffer(u16 egg_species, u16 *eggmove_buff) {
    Memset((void *)eggmove_buff, 0x0, EGGMOVE_MAX);
    u16 i;
    for (i = 0; i < 0x471; ++i) {
        if (pokemon_eggmoves[i] == (egg_species + 20000)) { //  lol gamefreak
            memcpy((void *)eggmove_buff, (void*)&pokemon_eggmoves[i + 1], 2 * EGGMOVE_MAX);
            return;
        }

    }
};



u8 get_encounter_level(u16 species, u8 environment) {
    u8 index = GetWildDataIndexForMap();
    u8 min = 100;
    u8 max = 0;
    switch (environment) {
        // grass
        case 0:
        {
            u8 i;
            for (i = 0; i < 12; ++i) {
                struct wild_pokemon w_pkmn = wild_pokemon_data[index].grass_encounter->wild_encounters->wild_grass[i];
                if (w_pkmn.species == species) {
                    min = (min < w_pkmn.min_level) ? min : w_pkmn.min_level;
                    max = (max > w_pkmn.max_level) ? max : w_pkmn.max_level;

                }
            }
            break;
        }
        // surf
        case 1:
        {
            u8 i;
            for (i = 0; i < 5; ++i) {
                struct wild_pokemon w_pkmn = wild_pokemon_data[index].water_encounter->wild_encounters->wild_water[i];
                if (w_pkmn.species == species) {
                    min = (min < w_pkmn.min_level) ? min : w_pkmn.min_level;
                    max = (max > w_pkmn.max_level) ? max : w_pkmn.max_level;
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
    if (max == 0) {
        // free dexnav display message
        return 0xFF;
    }

    // mod div by 0 edge case.
    if (min == max) {
        return min;
    }
    return (min + (rand() % (max - min)));
};



void dexnav_generate_move(u16 species, u8 search_level, u8 encounter_level, u16* move_loc) {
    bool8 gen_move = false;
    u16 rand_val = rand() % 100;

    // evaluate if Pokemon should get a custom first move
    if (search_level < 5) {
        if (rand_val <= SEARCHLEVEL0_MOVECHANCE) {
                gen_move = true;
        }
    } else if (search_level < 10) {
        if (rand_val <= SEARCHLEVEL5_MOVECHANCE) {
                gen_move = true;
        }
    } else if (search_level < 25) {
        if (rand_val <= SEARCHLEVEL10_MOVECHANCE) {
                gen_move = true;
        }
    } else if (search_level < 50) {
        if (rand_val <= SEARCHLEVEL25_MOVECHANCE) {
                gen_move = true;
        }
    } else if (search_level < 100) {
        if (rand_val <= SEARCHLEVEL50_MOVECHANCE) {
                gen_move = true;
        }
    } else {
        if (rand_val <= SEARCHLEVEL100_MOVECHANCE) {
                gen_move = true;
        }
    }
    // gen_move = true;
    // get the species's egg move, if one exists
    enum Move move[4] = {MOVE_NONE, MOVE_NONE, MOVE_NONE, MOVE_NONE};
    if (gen_move) {
        enum Move eggmove_buff[EGGMOVE_MAX];
        enum Move egg_species = get_base_species(species);
        populate_eggmovebuffer(egg_species, &eggmove_buff[0]);
        move[0] =  eggmove_buff[rand() % count_eggmoves(&eggmove_buff[0])];
    } else {
        // case the species has no egg moves
        gen_move = false;
    }

    // calculate the normal moves
    enum Move this_move = MOVE_NONE;

    u8 i = 0;
    u8 c_move_index = gen_move;
    while (this_move < MOVE_MAX) {
       struct LearnsetEntry learnable_move = pokemon_learnset[species][i];
       this_move = learnable_move.move;

       // learnset is level ordered, so exit early
       if (learnable_move.level > encounter_level) {
            break;
       } else {
            // if an eggmove is in the first slot, skip it and move to the next slot
            if ((gen_move) && (!(i % 4))) {
                move[1] = this_move;
            } else {
                move[c_move_index] = this_move;
            }
       }
       i += 1;
       c_move_index += 1;

    }

    // assign moves to array
    for (i = 0; i < 4; ++i) {
        move_loc[i] = move[i];
    }
    return;
};



enum PokemonAbility dexnav_generate_hiddenability(u16 species, u8 search_level) {
    bool8 gen_ability = false;
    u16 rand_val = rand() % 100;
    if (search_level < 5) {
        if (rand_val <= SEARCHLEVEL0_ABILITYCHANCE) {
                gen_ability = true;
        }
    } else if (search_level < 10) {
        if (rand_val <= SEARCHLEVEL5_ABILITYCHANCE) {
                gen_ability = true;
        }
    } else if (search_level < 25) {
        if (rand_val <= SEARCHLEVEL10_ABILITYCHANCE) {
                gen_ability = true;
        }
    } else if (search_level < 50) {
        if (rand_val <= SEARCHLEVEL25_ABILITYCHANCE) {
                gen_ability = true;
        }
    } else if (search_level < 100) {
        if (rand_val <= SEARCHLEVEL50_ABILITYCHANCE) {
                gen_ability = true;
        }
    } else {
        if (rand_val <= SEARCHLEVEL100_ABILITYCHANCE) {
                gen_ability = true;
        }
    }

    if ((gen_ability) && (gBaseStats[species].hidden_ability != ABILITY_NONE)) {
        return gBaseStats[species].hidden_ability; // should be ability when reworked
    } else {
        // pick a normal ability of that Pokemon
        if (gBaseStats[species].ability2) {
            return rand_val % 2 ? gBaseStats[species].ability1 : gBaseStats[species].ability2;
        } else {
            return gBaseStats[species].ability1;
        }
    }
};



u8 dexnav_generate_potential(u8 search_level) {
    u8 gen_chance = 0;
    u16 rand_val = rand() % 100;
    if (search_level < 5) {
        gen_chance = SEARCHLEVEL0_ONESTAR + SEARCHLEVEL0_TWOSTAR + SEARCHLEVEL0_THREESTAR;
        if (rand_val <= gen_chance) {
            // figure out which star it is
            if (rand_val <= SEARCHLEVEL0_ONESTAR) {
                return 1;
            } else if (rand_val <= (SEARCHLEVEL0_ONESTAR + SEARCHLEVEL0_TWOSTAR)) {
                return 2;
            } else {
                return 3;
            }
        }
    } else if (search_level < 10) {
        gen_chance = SEARCHLEVEL5_ONESTAR + SEARCHLEVEL5_TWOSTAR + SEARCHLEVEL5_THREESTAR;
        if (rand_val <= gen_chance) {
            // figure out which star it is
            if (rand_val <= SEARCHLEVEL5_ONESTAR) {
                return 1;
            } else if (rand_val <= (SEARCHLEVEL5_ONESTAR + SEARCHLEVEL5_TWOSTAR)) {
                return 2;
            } else {
                return 3;
            }
        }
    } else if (search_level < 25) {
        gen_chance = SEARCHLEVEL10_ONESTAR + SEARCHLEVEL10_TWOSTAR + SEARCHLEVEL10_THREESTAR;
        if (rand_val <= gen_chance) {
            // figure out which star it is
            if (rand_val <= SEARCHLEVEL10_ONESTAR) {
                return 1;
            } else if (rand_val <= (SEARCHLEVEL10_ONESTAR + SEARCHLEVEL10_TWOSTAR)) {
                return 2;
            } else {
                return 3;
            }
        }
    } else if (search_level < 50) {
        gen_chance = SEARCHLEVEL25_ONESTAR + SEARCHLEVEL25_TWOSTAR + SEARCHLEVEL25_THREESTAR;
        if (rand_val <= gen_chance) {
            // figure out which star it is
            if (rand_val <= SEARCHLEVEL25_ONESTAR) {
                return 1;
            } else if (rand_val <= (SEARCHLEVEL25_ONESTAR + SEARCHLEVEL25_TWOSTAR)) {
                return 2;
            } else {
                return 3;
            }
        }
    } else if (search_level < 100) {
        gen_chance = SEARCHLEVEL50_ONESTAR + SEARCHLEVEL50_TWOSTAR + SEARCHLEVEL50_THREESTAR;
        if (rand_val <= gen_chance) {
            // figure out which star it is
            if (rand_val <= SEARCHLEVEL50_ONESTAR) {
                return 1;
            } else if (rand_val <= (SEARCHLEVEL50_ONESTAR + SEARCHLEVEL50_TWOSTAR)) {
                return 2;
            } else {
                return 3;
            }
        }
    } else {
        gen_chance = SEARCHLEVEL100_ONESTAR + SEARCHLEVEL100_TWOSTAR + SEARCHLEVEL100_THREESTAR;
        if (rand_val <= gen_chance) {
            // figure out which star it is
            if (rand_val <= SEARCHLEVEL100_ONESTAR) {
                return 1;
            } else if (rand_val <= (SEARCHLEVEL100_ONESTAR + SEARCHLEVEL100_TWOSTAR)) {
                return 2;
            } else {
                return 3;
            }
        }
    }
    // No potential
    return 0;
};



enum Item dexnav_generate_helditem(u16 species, u8 search_level) {
    u16 rand_val = rand() % 100;
    u8 search_level_influence = search_level >> 1;
    enum Item item1 = gBaseStats[species].item1;
    enum Item item2 = gBaseStats[species].item2;

    // if both are the same, 100% to hold
    if (item1 == item2) {
        return item1;
    }

    // if no items can be held, then yeah..no items
    if ((item2 == ITEM_NONE) && (item1 == ITEM_NONE)) {
        return ITEM_NONE;
    }

    // if only one entry, 50% chance
    if ((item2 == ITEM_NONE) && (item1 != ITEM_NONE)) {
        return (rand_val <= 50) ? item1 : ITEM_NONE;
    }

    // if both are distinct item1 = 50% + srclvl/2; item2 = 5% + srchlvl/2
    if (rand_val < ((50 + search_level_influence) + (5 + search_level))) {
        return (rand_val > (5 + search_level_influence)) ? item1 : item2;
    } else {
        return ITEM_NONE;
    }
    return ITEM_NONE;
};



u8 dexnav_generate_pokemonlvl(u16 species, u8 search_level, u8 environment) {
    u8 level_base = get_encounter_level(species, environment);
    if (level_base > 100) {
        return (0);
    }
    u8 search_level_bonus = 0;
    if ((search_level >> 2) > 20) {
        search_level_bonus = 20;
    } else {
        search_level_bonus = (search_level >> 2);
    }
    if ((search_level_bonus + level_base) > 100) {
        return 100;
    } else {
        return ((search_level >> 2) + (level_base));
    }
};


void dexnav_gen_pkmnn(u16 species, u8 potential, u8 level, u8 ability, u16* moves) {
    struct Pokemon* pkmn = party_opponent;

    // clear canvas
    Memset((void*)pkmn, 0, 100);
    pokemon_make(&pkmn->base, species, level, 0x0, 0, 0, 0, saveblock2->trainerid);


    // set IVs randomly
    u8 i;
    u8 current_iv = REQUEST_HP_IV;
    for (i = 0; i < 6; ++i) {
        u8 iv_value = (rand() & 0x1F);
        set_pokemon_data_2(pkmn, current_iv, (void*) &iv_value);
        current_iv++;
    }

    // pick potential ivs to set to 31
    while (1) {
        u8 iv[3];
        iv[0] = rand() % 6;
        iv[1] = rand() % 6;
        iv[2] = rand() % 6;
        if ((iv[0] != iv[1]) && (iv[0] != iv[2]) && (iv[1] != iv[2])) {
            u8 perfect_iv = 0x1F;
            if (potential > 2) {
                set_pokemon_data_2(pkmn, REQUEST_HP_IV + iv[2], (void*) &perfect_iv);
            }
            if (potential > 1) {
                set_pokemon_data_2(pkmn, REQUEST_HP_IV + iv[1], (void*) &perfect_iv);
            }
            if (potential) {
                set_pokemon_data_2(pkmn, REQUEST_HP_IV + iv[0], (void*) &perfect_iv);
            }
        }
        break;
    }

    // set ability
    // TODO MANAGE HIDDEN ABILITIES
    u8 on_bit = gBaseStats[species].ability2 ? 1 : 0;
    set_pokemon_data_2(pkmn, REQUEST_ABILITY_BIT, (void*) &on_bit);

    // set moves
    set_pokemon_data_2(pkmn, REQUEST_MOVE1, (void*) (moves));
    set_pokemon_data_2(pkmn, REQUEST_MOVE2, (void*) (moves + 1));
    set_pokemon_data_2(pkmn, REQUEST_MOVE3, (void*) (moves + 2));
    set_pokemon_data_2(pkmn, REQUEST_MOVE4, (void*) (moves + 3));

    // set PP of moves
    set_pokemon_data_2(pkmn, REQUEST_PP1, (void *) &pokemon_moves[moves[0]].pp);
    set_pokemon_data_2(pkmn, REQUEST_PP2, (void *) &pokemon_moves[moves[1]].pp);
    set_pokemon_data_2(pkmn, REQUEST_PP3, (void *) &pokemon_moves[moves[2]].pp);
    set_pokemon_data_2(pkmn, REQUEST_PP4, (void *) &pokemon_moves[moves[3]].pp);

    // pokerus
    i = 0xFF;
    set_pokemon_data_2(pkmn, REQUEST_POKERUS_REMAINING, &i);
    recalculate_stats(pkmn);


};





*/


