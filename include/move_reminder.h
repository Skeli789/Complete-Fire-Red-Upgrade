#ifndef GUARD_MOVE_RELEARNER_H
#define GUARD_MOVE_RELEARNER_H

#include "global.h"
#include "window.h"

#define LIST_NOTHING_CHOSEN -1
#define LIST_CANCEL -2
#define LIST_HEADER -3

#define LIST_NO_MULTIPLE_SCROLL     0
#define LIST_MULTIPLE_SCROLL_DPAD   1
#define LIST_MULTIPLE_SCROLL_L_R    2

#define MENU_STATE_FADE_TO_BLACK 0
#define MENU_STATE_WAIT_FOR_FADE 1
#define MENU_STATE_UNREACHABLE 2
#define MENU_STATE_SETUP_BATTLE_MODE 3
#define MENU_STATE_IDLE_BATTLE_MODE 4
#define MENU_STATE_SETUP_CONTEST_MODE 5
#define MENU_STATE_IDLE_CONTEST_MODE 6
// State 7 is skipped.
#define MENU_STATE_PRINT_TEACH_MOVE_PROMPT 8
#define MENU_STATE_TEACH_MOVE_CONFIRM 9
// States 10 and 11 are skipped.
#define MENU_STATE_PRINT_GIVE_UP_PROMPT 12
#define MENU_STATE_GIVE_UP_CONFIRM 13
#define MENU_STATE_FADE_AND_RETURN 14
#define MENU_STATE_RETURN_TO_FIELD 15
#define MENU_STATE_PRINT_TRYING_TO_LEARN_PROMPT 16
#define MENU_STATE_WAIT_FOR_TRYING_TO_LEARN 17
#define MENU_STATE_CONFIRM_DELETE_OLD_MOVE 18
#define MENU_STATE_PRINT_WHICH_MOVE_PROMPT 19
#define MENU_STATE_SHOW_MOVE_SUMMARY_SCREEN 20
// States 21, 22, and 23 are skipped.
#define MENU_STATE_PRINT_STOP_TEACHING 24
#define MENU_STATE_WAIT_FOR_STOP_TEACHING 25
#define MENU_STATE_CONFIRM_STOP_TEACHING 26
#define MENU_STATE_CHOOSE_SETUP_STATE 27
#define MENU_STATE_FADE_FROM_SUMMARY_SCREEN 28
#define MENU_STATE_TRY_OVERWRITE_MOVE 29
#define MENU_STATE_DOUBLE_FANFARE_FORGOT_MOVE 30
#define MENU_STATE_PRINT_TEXT_THEN_FANFARE 31
#define MENU_STATE_WAIT_FOR_FANFARE 32
#define MENU_STATE_WAIT_FOR_A_BUTTON 33

#define sMoveRelearnerMovesListTemplate ((void*) 0x83FFA94)

/*
const struct ListMenuTemplate sMoveRelearnerMovesListTemplate =
{
    .items = NULL,
    .moveCursorFunc = MoveRelearnerCursorCallback,
    .itemPrintFunc = NULL,
    .totalItems = 0,
    .maxShowed = 0,
    .windowId = 2,
    .header_X = 0,
    .item_X = 8,
    .cursor_X = 0,
    .upText_Y = 1,
    .cursorPal = 2,
    .fillValue = 1,
    .cursorShadowPal = 3,
    .lettersSpacing = 0,
    .itemVerticalPadding = 0,
    .scrollMultiple = LIST_NO_MULTIPLE_SCROLL,
    .fontId = 1,
    .cursorKind = 0
};
*/

const struct WindowTemplate sMoveRelearnerWindowTemplates[] =
{
    {
        .bg = 0,
        .tilemapLeft = 0,
        .tilemapTop = 0,
        .width = 6,
        .height = 7,
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
		.tilemapTop = 6,
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


void __attribute__((long_call)) CB2_InitLearnMove(void);
u8 __attribute__((long_call)) GetNumMoveRelearnerMoves(pokemon_t* mon, void *moveLocs);


#endif
