#pragma once

#include "../global.h"
#include "../window.h"

extern const struct WindowTemplate sMoveRelearnerExpandedTemplates[];
const struct WindowTemplate sMoveRelearnerExpandedTemplates[] =
{
    {	// TYPE, PP, symbols
        .bg = 0,
        .tilemapLeft = 0,
        .tilemapTop = 0,
        .width = 6,
        .height = 4,
        .paletteNum = 0xD,
        .baseBlock = 0x14,
    },
    {
        .bg = 0,
        .tilemapLeft = 0xA,
        .tilemapTop = 0,
        .width = 0x5,
        .height = 5,
        .paletteNum = 0xD,
        .baseBlock = 0x3E,
    },
    {
        .bg = 0,
        .tilemapLeft = 5,
        .tilemapTop =0,
        .width = 5,
        .height = 2,
        .paletteNum = 0xD,
        .baseBlock = 0x57,
    },
    {
        .bg = 0,
        .tilemapLeft = 0xF,
        .tilemapTop = 0,
        .width = 3,
        .height = 5,
        .paletteNum = 0xF,
        .baseBlock = 0x61,
    },
	{
		.bg = 0,
		.tilemapLeft = 5,
		.tilemapTop = 2,
		.width = 3,
		.height = 3,
		.paletteNum = 0xF, 
		.baseBlock = 0x70,
	},
	{	// description box
		.bg = 0,
		.tilemapLeft = 2,
		.tilemapTop = 4,
		.width = 0xF,
		.height = 8,
		.paletteNum = 0xF,
		.baseBlock = 0x79,
	},
	{
		.bg = 0,
		.tilemapLeft = 0x13,
		.tilemapTop = 1,
		.width = 0xA,
		.height = 0xC,
		.paletteNum = 0xF,
		.baseBlock = 0xF1,
	},
    {	// Bottom msgbox
        .bg = 0,
        .tilemapLeft = 2,
        .tilemapTop = 0xF,
        .width = 0x1A,
        .height = 4,
        .paletteNum = 0xF,
        .baseBlock = 0x169
    },
    DUMMY_WIN_TEMPLATE,
};
