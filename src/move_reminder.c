#include "defines.h"

#include "../include/move_reminder.h"

#include "../include/constants/moves.h"

extern const u8 gMoveNames[][MOVE_NAME_LENGTH + 1];
extern u8 GetMoveRelearnerMoves(struct Pokemon* mon, u16* moves);

// original structure
#ifndef EXPAND_MOVE_REMINDER
	#define MAX_MOVE_REMINDER_MOVES 25	//24 moves + cancel
	
	struct MoveRelearner {
		u8 state;		//0x0
		u8 unk[0x19];
		u8 numMenuChoices;		//0x1A
		u8 numToShowAtOnce;
		u8 unk2[0x1F - 0x1B];
		struct ListMenuItem menuItems[MAX_MOVE_REMINDER_MOVES];	//0x20		24 moves + cancel
		u16 moves[MAX_MOVE_REMINDER_MOVES];	//0xe8
		const u8 listMenuNames[MAX_MOVE_REMINDER_MOVES][MOVE_NAME_LENGTH + 1];	// 0x11A, move names + cancel
		bool8 isSwitching;	// 0x25F, loads new gfx, data if set to 1 (upon scrolling)
		u8 partySlot;		// 0x260
		u8 unk4[0xA66 - 0x260];
		u8 cursorPos; //0xa67
		u8 unk5[0xA6C - 0xA67];
	};
#else
	#define MAX_MOVE_REMINDER_MOVES 51	//50 moves + cancel

	struct MoveRelearner {
		u8 state;	//0x0
		u8 unk[0x19];	//0x1 - 0x19
		u8 numMenuChoices;		//0x1A
		u8 numToShowAtOnce;
		u8 unk2[0x1F - 0x1B];
		struct ListMenuItem menuItems[MAX_MOVE_REMINDER_MOVES];	//0x20 - 0x1b8
		u16 moves[MAX_MOVE_REMINDER_MOVES];
		u8 unk3[0x25E - 0x21E];
		bool8 isSwitching;	// 0x25F, loads new gfx, data if set to 1 (upon scrolling)
		u8 partySlot;		// 0x260
		u8 moveSlotToOverwrite;	//0x261
		u8 unk4[0xA66 - 0x261];
		u8 cursorPos; //0xa67
		u8 unk5[0xA6F - 0xA67];
		const u8 listMenuNames[MAX_MOVE_REMINDER_MOVES][MOVE_NAME_LENGTH + 1];
	};
#endif


struct MoveRelearner MoveRelearnerStruct;

#define gMoveRelearnerStruct ((struct MoveRelearner*) (*(u32*) 0x203AAB4))


void InitLearnMoveFix(void)
{
	*sMoveRelearnerStructPtr = Calloc(sizeof(MoveRelearnerStruct));
}



bool8 CheckMoveRelearnerMoveLimit(u8 counter)
{
	if (counter >= MAX_MOVE_REMINDER_MOVES)
		return FALSE;
	return TRUE;
}



u16 GetMoveIdFromRelearnerStruct(u8 index)
{
	return gMoveRelearnerStruct->moves[index];
}



void InitMoveRelearnerMoveIDs(void)
{
	for (u8 i = 0; i < MAX_MOVE_REMINDER_MOVES; ++i)
	{
		gMoveRelearnerStruct->moves[i] = 0;
	}
}

/*
u8 LoadMoveRelearnerMovesList(const struct ListMenuItem *items, u16 numChoices)
{
    //gMultiuseListMenuTemplate = sMoveRelearnerMovesListTemplate;
	gMultiuseListMenuTemplate->moveCursorFunc = sMoveRelearnerMovesListTemplate;
	
	
	gMultiuseListMenuTemplate->totalItems = numChoices;
    gMultiuseListMenuTemplate->items = items;

    if (numChoices < 6)
    {
        gMultiuseListMenuTemplate->maxShowed = numChoices;
    }
    else
    {
        gMultiuseListMenuTemplate->maxShowed = 6;
    }
    return gMultiuseListMenuTemplate->maxShowed;
}
*/

u8 GetRelearnableMoves(pokemon_t* mon)
{
	u8 numMoves = GetMoveRelearnerMoves(mon, &gMoveRelearnerStruct->moves[0]);
	gMoveRelearnerStruct->numMenuChoices = numMoves;
	
	u8 i = 0;
	if (numMoves > 0)
	{
		for (i = 0; i < numMoves; ++i)
		{
			StringCopy((void*) &gMoveRelearnerStruct->listMenuNames[i], gMoveNames[gMoveRelearnerStruct->moves[i]]);
			gMoveRelearnerStruct->menuItems[i].name = (void*) &gMoveRelearnerStruct->listMenuNames[i];
			gMoveRelearnerStruct->menuItems[i].id = i;
		}
	}
	
	// set CANCEL as last option
	StringCopy((void*) &gMoveRelearnerStruct->listMenuNames[i], &gText_Cancel[0]);
	gMoveRelearnerStruct->menuItems[i].id = 0xFE;
	gMoveRelearnerStruct->menuItems[i].name = &gText_Cancel[0];
	gMoveRelearnerStruct->numMenuChoices++;
	
	// buffer nickname
	GetMonData(mon, MON_DATA_NICKNAME, &gStringVar1[0]);
	
	//gMoveRelearnerStruct->numToShowAtOnce = LoadMoveRelearnerMovesList(gMoveRelearnerStruct->menuItems, gMoveRelearnerStruct->numMenuChoices);
	
	//LoadMoveRelearnerMovesList(gMoveRelearnerStruct->menuItems, gMoveRelearnerStruct->numMenuChoices);
	
	//gMultiuseListMenuTemplate->totalItems = gMoveRelearnerStruct->numMenuChoices;
	
	return gMoveRelearnerStruct->numMenuChoices;	//total list count
}



/*
void SetMoveReminderMultiTemplate(void)
{
	gMultiuseListMenuTemplate->items = &gMoveRelearnerStruct->menuItems[0];
}
*/


extern const u8 gText_MoveRelearnerAskTeach[];

const u8* CopyMoveReminderMoveName(u8 cursor)
{
	StringCopy(&gStringVar2[0], (void*) gMoveRelearnerStruct->listMenuNames[cursor]);
	GetMonData(&gPlayerParty[gMoveRelearnerStruct->partySlot], MON_DATA_NICKNAME, &gStringVar3[0]);
	return &gText_MoveRelearnerAskTeach[0];
}


