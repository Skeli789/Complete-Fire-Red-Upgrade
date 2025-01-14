#pragma once

#include "../global.h"

/**
 * \file learn_move.h
 * \brief Contains functions relating to Pokemon learning new moves. Also contains
 *		  functions for Evolution moves and the Move Relearner.
 */

//Exported Structs
struct MovePair
{
	u16 move;
	u16 num;
};

//Exported Functions
void GiveBoxMonInitialMoveset(struct BoxPokemon* boxMon);
u16 MonTryLearningNewMoveAfterEvolution(struct Pokemon* mon, bool8 firstMove);
u8 GetLevelUpMovesBySpecies(u16 species, u16* moves);
u8 GetLevelUpMovePairsBySpecies(u16 species, struct MovePair* moves);
u16 BuildLearnableMoveset(struct Pokemon* mon, u16* moves);
u16 BuildTMMoveset(struct Pokemon* mon, struct MovePair* moves);
u16 BuildTutorMoveset(struct Pokemon* mon, struct MovePair* moves);
u16 RandomizeMove(u16 move);

//Functions Hooked In
u16 MonTryLearningNewMove(struct Pokemon* mon, bool8 firstMove);
u8 GetMoveRelearnerMoves(struct Pokemon* mon, u16* moves);
u8 GetNumberOfRelearnableMoves(struct Pokemon* mon);
u16 GiveMoveToBoxMon(struct BoxPokemon *boxMon, u16 move);
void SetMonMoveSlot(struct Pokemon* mon, u16 move, u8 slot);
void InitLearnMoveFix(void);
bool8 CheckMoveRelearnerMoveLimit(u8 counter);
u16 GetMoveIdFromRelearnerStruct(u8 index);
void InitMoveRelearnerMoveIDs(void);
u8 GetRelearnableMoves(struct Pokemon* mon);
const u8* CopyMoveReminderMoveName(u8 cursor);
bool16 InitMoveRelearnerWindows(void);

//Exported Constants
#define MAX_LEARNABLE_MOVES 50
