#include "../include/gba/types.h"
#include "../include/gba/io_reg.h"
#include "../include/global.h"
#include "../include/palette.h"
#include "../include/pokemon.h"
#include "../include/sprite.h"
#include "../include/pokemon_icon.h"
#include "../include/gpu_regs.h"
#include "../include/main_menu.h"
#define gMonIconPalettes ((u16 *)(0x083d3740))



u16 coords_menu[][2] = {
    {40,      16+58+32},
    {40+32,   16+58+32},
    {40+32*2, 16+58+32},
    {40+32*3, 16+58+32},
    {40+32*4, 16+58+32},
    {40+32*5, 16+58+32},
};

u8 sPokemonIconPalSlots[] = {10, 11, 12, 13, 14, 15};
/*
void PrintPokemonIconsOnCard(void)
{
    u8 i;
    u8 paletteSlots[6];

    memcpy(paletteSlots, sPokemonIconPalSlots, sizeof(sPokemonIconPalSlots));

    for (i = 0; i < 6; i++)
    {
        void *mon = &gPlayerParty[i];
        u16 species = GetMonData(mon, MON_DATA_SPECIES);
        u16 pid = GetMonData(mon, MON_DATA_PERSONALITY);

        if (species != 0)
        {
            u8 monSpecies = GetMonIconPaletteIndexFromSpecies(species);
            WriteSequenceToBgTilemapBuffer(1, 16 * i + 448, 2*i + 3, 15, 4, 4, paletteSlots[monSpecies], 1);
        }
    }
}

void LoadMonIconGfx(void)
{
    u8 i;

    u16 palettes[PARTY_SIZE*16];

    CpuCopy16(gMonIconPalettes, palettes, sizeof(palettes));

    LoadPalette(palettes, 80, 192);
    for (i = 0; i < PARTY_SIZE; i++)
    {
        void *mon = &gPlayerParty[i];
        u16 species = (mon, MON_DATA_SPECIES);
        u16 pid = GetMonData(mon, MON_DATA_PERSONALITY);
        if (species != 0)
        {
            LoadBgTiles(1, GetMonIconTiles(species, 0), 512, 16 * i + 32);
        }
    }
} 
*/

void PrintMonIcons(void)
{
    //LoadMonIconGfx();
    //PrintPokemonIconsOnCard();

    // object approach instead
    LoadMonIconPalettes();
    //ResetSpriteData();

    SetGpuRegBits(REG_OFFSET_DISPCNT, DISPCNT_OBJWIN_ON);
    SetGpuRegBits(REG_OFFSET_WINOUT, WINOUT_WINOBJ_OBJ);
    SetGpuRegBits(REG_OFFSET_WININ, WININ_WIN0_OBJ);
 
 
    // two loops is fine tbh
    for (int i = 0; i < 6; i++)
    {
        void *mon = &gPlayerParty[i];
        u16 species = GetMonData(mon, MON_DATA_SPECIES, NULL);
        u16 pid = GetMonData(mon, MON_DATA_PERSONALITY, NULL);

        if (species != 0) 
        {
            u8 spriteId;
            spriteId = CreateMonIcon(species, SpriteCB_PokeIcon, 96+16, 40+16, 0, pid, 0);
            gSprites[spriteId].oam.priority = 0;
            gSprites[spriteId].invisible = 0;
            gSprites[spriteId].pos1.x = 0;
            gSprites[spriteId].pos1.y = 0;
            gSprites[spriteId].pos2.x = coords_menu[i][0];
            gSprites[spriteId].pos2.y = coords_menu[i][1];
            //gSprites[spriteId].oam.objMode = ST_OAM_OBJ_WINDOW;
        }
    }

    for (int i = 0; i < 6; i++)
    {
        void *mon = &gPlayerParty[i];
        u16 species = GetMonData(mon, MON_DATA_SPECIES, NULL);
        u16 pid = GetMonData(mon, MON_DATA_PERSONALITY, NULL);

        if (species != 0)
        {
            u8 spriteId;
            spriteId = CreateMonIcon(species, SpriteCB_PokeIcon, 96+16, 40+16, 0, pid, 0);
            gSprites[spriteId].oam.priority = 0;
            gSprites[spriteId].invisible = 0;
            gSprites[spriteId].pos1.x = 0;
            gSprites[spriteId].pos1.y = 0;
            gSprites[spriteId].pos2.x = coords_menu[i][0];
            gSprites[spriteId].pos2.y = coords_menu[i][1];
            gSprites[spriteId].oam.objMode = ST_OAM_OBJ_WINDOW;
        }
    }
}


void Task_WaitFadeAndPrintMainMenuText(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        Task_PrintMainMenuText(taskId);
        PrintMonIcons();
    }
}
