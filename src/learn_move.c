#include "defines.h"
#include "../include/list_menu.h"
#include "../include/string_util.h"

//struct MoveRelearner
//{
//    u8 state;
//    u8 heartSpriteIds[16];              /*0x001*/
//    u16 movesToLearn[4];                /*0x012*/   //0xE8
//    u8 filler1A[0x44 - 0x1A];           /*0x01A*/
//    u8 partyMon;                        /*0x044*/	//0x260
//    u8 moveSlot;                        /*0x045*/
//    struct ListMenuItem menuItems[20];  /*0x048*/	//0x11A
//    u8 fillerE8[0x110 - 0xE8];          /*0x0E8*/
//    u8 numMenuChoices;                  /*0x110*/ 	//0x1A
//    u8 numToShowAtOnce;                 /*0x111*/
//    u8 moveListMenuTask;                /*0x112*/
//    u8 moveListScrollArrowTask;         /*0x113*/
//    u8 moveDisplayArrowTask;            /*0x114*/
//    u16 scrollOffset;                   /*0x116*/
//};

#define gMoveToLearn (*((u16*) 0x2024022))
#define sLearningMoveTableID (*((u8*) 0x2024028))
#define sMoveRelearnerStruct ((struct MoveRelearner*) 0x203AAB4)
#define gText_Cancel ((u8*) 0x84161C1)
#define MAX_LEARNABLE_MOVES 50

void GiveBoxMonInitialMoveset(struct BoxPokemon* boxMon)
{
    u16 species = GetBoxMonData(boxMon, MON_DATA_SPECIES, NULL);
    s32 level = GetLevelFromBoxMonExp(boxMon);
    s32 i;

    for (i = 0; !(gLevelUpLearnsets[species][i].level == 0 
			      && gLevelUpLearnsets[species][i].level == 0xFF); ++i)
    {
        struct LevelUpMove lvlUpMove = gLevelUpLearnsets[species][i];

        if (lvlUpMove.level > level)
            break;

        if (GiveMoveToBoxMon(boxMon, lvlUpMove.move) == 0xFFFF)
            DeleteFirstMoveAndGiveMoveToBoxMon(boxMon, lvlUpMove.move);
    }
}

u16 MonTryLearningNewMove(struct Pokemon* mon, bool8 firstMove)
{
    u32 retVal = 0;
    u16 species = mon->species;
    u8 level = mon->level;
	struct LevelUpMove lvlUpMove;
	
    // since you can learn more than one move per level
    // the game needs to know whether you decided to
    // learn it or keep the old set to avoid asking
    // you to learn the same move over and over again
    if (firstMove)
    {
        sLearningMoveTableID = 0;
		lvlUpMove = gLevelUpLearnsets[species][sLearningMoveTableID];

        while (lvlUpMove.level != level)
        {
            lvlUpMove = gLevelUpLearnsets[species][++sLearningMoveTableID];
            if (lvlUpMove.move == 0
			&&  lvlUpMove.level == 0xFF)
                return 0;
        }
    }

	lvlUpMove = gLevelUpLearnsets[species][sLearningMoveTableID];
    if (lvlUpMove.level == level)
    {
        gMoveToLearn = lvlUpMove.move;
        ++sLearningMoveTableID;
        retVal = GiveMoveToMon(mon, gMoveToLearn);
    }

    return retVal;
}

u16 MonTryLearningNewMoveAfterEvolution(struct Pokemon* mon, bool8 firstMove)
{
    u32 retVal = 0;
    u16 species = mon->species;
    u8 level = mon->level;
	struct LevelUpMove lvlUpMove;
	
    if (firstMove)
    {
        sLearningMoveTableID = 0;
		lvlUpMove = gLevelUpLearnsets[species][sLearningMoveTableID];

        while (lvlUpMove.level != 0 && lvlUpMove.level != level)
        {
            lvlUpMove = gLevelUpLearnsets[species][++sLearningMoveTableID];
            if (lvlUpMove.move == 0
			&&  lvlUpMove.level == 0xFF)
                return retVal; //0
        }
    }

	lvlUpMove = gLevelUpLearnsets[species][sLearningMoveTableID];
    if (lvlUpMove.level == level || lvlUpMove.level == 0)
    {
        gMoveToLearn = lvlUpMove.move;
        ++sLearningMoveTableID;
        retVal = GiveMoveToMon(mon, gMoveToLearn);
    }

    return retVal;
}

u8 GetMoveRelearnerMoves(struct Pokemon* mon, u16* moves)
{
    u16 learnedMoves[4];
    u8 numMoves = 0;
    u16 species = mon->species;
    u8 level = mon->level;
    int i, j, k;

    for (i = 0; i < MAX_MON_MOVES; ++i)
        learnedMoves[i] = mon->moves[i];

    for (i = 0; i < MAX_LEARNABLE_MOVES; ++i) //50 max moves can be relearned
    {
		struct LevelUpMove lvlUpMove = gLevelUpLearnsets[species][i];

        if (lvlUpMove.move == 0 && lvlUpMove.level == 0xFF)
			break;

        if (lvlUpMove.level <= level)
        {
            for (j = 0; j < MAX_MON_MOVES && learnedMoves[j] != lvlUpMove.move; ++j)
                ;

            if (j == MAX_MON_MOVES)
            {
                for (k = 0; k < numMoves && moves[k] != lvlUpMove.move; ++k)
                    ;

                if (k == numMoves)
                    moves[numMoves++] = lvlUpMove.move;
            }
        }
    }

    return numMoves;
}

u8 GetLevelUpMovesBySpecies(u16 species, u16* moves)
{
    u8 numMoves = 0;
    int i;

    for (i = 0; i < MAX_LEARNABLE_MOVES && !(gLevelUpLearnsets[species][i].level == 0 
						&& gLevelUpLearnsets[species][i].level == 0xFF); ++i)
	{
         moves[numMoves++] = gLevelUpLearnsets[species][i].move;
	}

    return numMoves;
}

u8 GetNumberOfRelearnableMoves(struct Pokemon* mon)
{
    u16 moves[MAX_LEARNABLE_MOVES];

	if (GetMonData(mon, MON_DATA_IS_EGG, 0))
		return 0;

	return GetMoveRelearnerMoves(mon, moves); //Returns the number of moves
}

/*
void CreateLearnableMovesList(void)
{
    s32 i;
    u8 nickname[POKEMON_NAME_LENGTH + 1];
	u16 moves[MAX_LEARNABLE_MOVES] = {0};

    sMoveRelearnerStruct->numMenuChoices = GetMoveRelearnerMoves(&gPlayerParty[sMoveRelearnerStruct->partyMon], moves);

    for (i = 0; i < sMoveRelearnerStruct->numMenuChoices; i++)
    {
        sMoveRelearnerStruct->menuItems[i].name = gMoveNames[moves[i]];
        sMoveRelearnerStruct->menuItems[i].id = moves[i];
    }

    GetMonData(&gPlayerParty[sMoveRelearnerStruct->partyMon], MON_DATA_NICKNAME, nickname);
    StringCopy10(gStringVar1, nickname);
    sMoveRelearnerStruct->menuItems[sMoveRelearnerStruct->numMenuChoices].name = gText_Cancel;
    sMoveRelearnerStruct->menuItems[sMoveRelearnerStruct->numMenuChoices].id = LIST_CANCEL;
    sMoveRelearnerStruct->numMenuChoices++;
    sMoveRelearnerStruct->numToShowAtOnce = LoadMoveRelearnerMovesList(sMoveRelearnerStruct->menuItems, sMoveRelearnerStruct->numMenuChoices);
}*/