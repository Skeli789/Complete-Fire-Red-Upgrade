/********\
CUSTOM FILE!
THIS IS A CUSTOM FILE THAT ISN'T NORMALLY IN THE CFRU.
Give Credits to:
- Skeli
- Greenphx
if used.
\********/

#include "defines.h"
#include "../include/random.h"
#include "../include/constants/abilities.h"
#include "../include/global.h"

#include "../include/new/damage_calc.h"
#include "../include/new/evolution.h"
#include "../include/new/frontier.h"
#include "../include/new/mega.h"
#include "../include/new/dynamax.h"
#include "../include/constants/items.h"
#include "../include/constants/moves.h"
#include "../include/tm_case.h"
#include "../include/menu_helpers.h"
#include "../include/scanline_effect.h"
#include "../include/sprite.h"
#include "../include/palette.h"
#include "../include/task.h"
#include "../include/item.h"
#include "../include/list_menu.h"
#include "../include/new/item.h"
#include "../include/list_menu.h"
#include "../include/mgba.h"
#include "../include/malloc.h"
#include "../include/menu.h"
#include "../include/pokemon_icon.h"
#include "../include/window.h"
#include "../include/string_util.h"
#include "../include/new/Vanilla_functions.h"
#include "../include/text.h"
#include "../include/gba/io_reg.h"
#include "../include/pokemon.h"
#include "../include/gpu_regs.h"


/*
tm_case.c
	adds party icons to the tm case
*/

extern const u8 gMoveNames[][12 + 1];

struct UnkStruct_203B10C
{
    void (* savedCallback)(void);
    u8 tmCaseMenuType;
    u8 unk_05;
    u8 unk_06;
    u16 selectedRow;
    u16 scrollOffset;
};

struct UnkStruct_203B118
{
    void (* savedCallback)(void);
    u8 tmSpriteId;
    u8 maxTMsShown;
    u8 numTMs;
    u8 contextMenuWindowId;
    u8 scrollIndicatorArrowPairId;
    u16 currItem;
    const u8 * menuActionIndices;
    u8 numMenuActions;
    s16 seqId;
    u8 filler_14[8];
};

extern struct UnkStruct_203B118 * sTMCaseDynamicResources;
extern struct UnkStruct_203B10C * sTMCaseStaticResources;


extern const struct SpritePalette gMonIconPaletteTable[6];
extern const u16 sSpriteImageSizes[3][4];

extern const u8 gText_TMCase[];
static const u8 sTextColors[][3] = {
    {0, 1, 2},
    {0, 2, 3},
    {0, 3, 6},
    {0, 14, 10}
};

void NullFunc(void) //Hooked in
{
}

void PrintStringTMCaseOnWindow3(void)
{
    u32 distance = 80 - GetStringWidth(1, gText_TMCase, 0);
    AddTextPrinterParameterized3(3, 1, distance / 2, 1, sTextColors[0], 0, gText_TMCase);
}

extern const u8 gText_TMCaseWillBePutAway[];

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

void CreateTMCaseSprite(void)
{
	u32 i, counter;
	LoadMonIconPalettes();
	CreateGrayscaleMonIconPalettes(); //For mons that haven't been caught yet

	for (i = 0, counter = 0; i < PARTY_SIZE; ++i)
	{
		struct Pokemon* mon = &gPlayerParty[i];
		u16 species = GetMonData(&gPlayerParty[i], MON_DATA_SPECIES2, NULL);
		if (species != SPECIES_NONE && species != SPECIES_EGG)
		{
			s16 x = 1 + (32/2) + (counter % 3) * 32;
			s16 y = 29 + (32 / 2);
			if (counter >= 3)
				y = 58 + (32 / 2);

			u8 spriteId = CreateMonIcon(species, SpriteCallbackDummy, x, y, 0, GetMonData(mon, MON_DATA_PERSONALITY, NULL), FALSE);
			if (spriteId < MAX_SPRITES)
			{
				struct Sprite* sprite = &gSprites[spriteId];
				sprite->data[0] = i; //Mon Id
				sprite->data[1] = sprite->oam.paletteNum; //Save original palette number
			}

			++counter;
		}
	}
}

void ChangeMonIconPalsInTMCase(u16 itemId)
{
	u32 i;
	for (i = 0; i < MAX_SPRITES; ++i)
	{
		struct Sprite* sprite = &gSprites[i];
		if (sprite->oam.paletteNum < 6) //Is mon icon
		{
			bool8 canLearn = CanMonLearnTMTutor(&gPlayerParty[sprite->data[0]], itemId, 0);
		    SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT2_ALL);
            SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(7, 11));
			if (itemId != 0 && (canLearn == 0 || canLearn == 2)) //Can learn move or already knows
			{
                sprite->oam.objMode = ST_OAM_OBJ_NORMAL;
				sprite->callback = SpriteCB_PokeIcon;
			}
			else
			{
                sprite->oam.objMode = ST_OAM_OBJ_BLEND;
                SetPartyHPBarSprite(sprite, 0); //Stop moving and reload original frame
				sprite->callback(sprite); //Update frame
				sprite->callback = SpriteCallbackDummy; //Stop animating old mon icon
			}
		}
	}
} 

