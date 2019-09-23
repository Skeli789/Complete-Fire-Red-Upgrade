#pragma once

#include "../global.h"
#include "../window.h"

/**
 * \file move_reminder_data.h
 * \brief A file to be included on by "src/learn_move.c". Contains window
 *		  data relating the Move Relearner.
 */

enum
{
	WINDOW_TYPE_PP_SYMBOLS,
	WINDOW_POWER_ACC_SYMBOLS,
	WINDOW_MOVE_TYPE,
	WINDOW_MOVE_POWER,
	WINDOW_MOVE_PP,
	WINDOW_DESCRIPTION,
	WINDOW_MOVE_LIST,
	WINDOW_BOTTOM_MSG,
};

const struct WindowTemplate sMoveRelearnerExpandedTemplates[] =
{
	[WINDOW_TYPE_PP_SYMBOLS] =
    {
        .bg = 0,
        .tilemapLeft = 0,
        .tilemapTop = 0,
        .width = 6,
        .height = 4,
        .paletteNum = 13,
        .baseBlock = 20,
    },

	[WINDOW_POWER_ACC_SYMBOLS] =
    {
        .bg = 0,
        .tilemapLeft = 10,
        .tilemapTop = 0,
        .width = 5,
        .height = 5,
        .paletteNum = 13,
        .baseBlock = 44,
    },

	[WINDOW_MOVE_TYPE] =
    {
        .bg = 0,
        .tilemapLeft = 5,
        .tilemapTop = 0,
        .width = 5,
        .height = 2,
        .paletteNum = 13,
        .baseBlock = 69,
    },

	[WINDOW_MOVE_POWER] =
    {
        .bg = 0,
        .tilemapLeft = 15,
        .tilemapTop = 0,
        .width = 3,
        .height = 5,
        .paletteNum = 15,
        .baseBlock = 79,
    },

	[WINDOW_MOVE_PP] =
	{
		.bg = 0,
		.tilemapLeft = 5,
		.tilemapTop = 2,
		.width = 3,
		.height = 3,
		.paletteNum = 15,
		.baseBlock = 94,
	},

	[WINDOW_DESCRIPTION] =
	{
		.bg = 0,
		.tilemapLeft = 1,
		.tilemapTop = 4,
		.width = 15,
		.height = 9,
		.paletteNum = 15,
		.baseBlock = 103,
	},

	[WINDOW_MOVE_LIST]
	{
		.bg = 0,
		.tilemapLeft = 19,
		.tilemapTop = 1,
		.width = 10,
		.height = 12,
		.paletteNum = 15,
		.baseBlock = 238,
	},

	[WINDOW_BOTTOM_MSG] =
    {
        .bg = 0,
        .tilemapLeft = 2,
        .tilemapTop = 15,
        .width = 26,
        .height = 4,
        .paletteNum = 15,
        .baseBlock = 358,
    },
    DUMMY_WIN_TEMPLATE,
};
