#include "defines.h"
#include "defines_battle.h"
#include "../include/daycare.h"
#include "../include/list_menu.h"
#include "../include/move_reminder.h"
#include "../include/string_util.h"
#include "../include/constants/moves.h"

#include "../include/new/daycare.h"
#include "../include/new/item.h"
#include "../include/new/learn_move.h"
#include "../include/new/move_reminder_data.h"
/*
learn_move.c
	handles functions for pokemon trying to learn moves
*/

extern const u8 gMoveNames[][MOVE_NAME_LENGTH + 1];

#ifdef EXPAND_MOVESETS
	extern const struct LevelUpMove* const gLevelUpLearnsets[];
#else
	#define gLevelUpLearnsets ((struct LevelUpMove**) *((u32*) 0x8043E20)) //extern const struct LevelUpMove* const gLevelUpLearnsets[];
#endif

//#define gMoveToLearn (*((u16*) 0x2024022))
#define sLearningMoveTableID (*((u8*) 0x2024028))
#define sMoveRelearnerStruct ((struct MoveRelearner*) 0x203AAB4)

//This file's functions
#ifdef FLAG_POKEMON_LEARNSET_RANDOMIZER
static move_t RandomizeMove(u16 move);
#endif

void GiveBoxMonInitialMoveset(struct BoxPokemon* boxMon)
{
	s32 i, k, index;
	u16 moveStack[MAX_LEARNABLE_MOVES];

	u16 species = GetBoxMonData(boxMon, MON_DATA_SPECIES, NULL);
	s32 level = GetLevelFromBoxMonExp(boxMon);

	for (i = 0, k = 0; !(gLevelUpLearnsets[species][i].level == 0
				  && gLevelUpLearnsets[species][i].level == 0xFF); ++i)
	{
		struct LevelUpMove lvlUpMove = gLevelUpLearnsets[species][i];
		u16 move = lvlUpMove.move;

		#ifdef FLAG_POKEMON_LEARNSET_RANDOMIZER
		if (FlagGet(FLAG_POKEMON_LEARNSET_RANDOMIZER) && !FlagGet(FLAG_BATTLE_FACILITY))
			move = RandomizeMove(move);
		#endif

		if (lvlUpMove.level > level)
			break;

		moveStack[k++] = move;
	}

	index = k - MAX_MON_MOVES;
	if (index < 0)
		index = 0;

	while (index < k && GiveMoveToBoxMon(boxMon, moveStack[index++]) != 0xFFFF);
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

		#ifdef FLAG_POKEMON_LEARNSET_RANDOMIZER
		if (FlagGet(FLAG_POKEMON_LEARNSET_RANDOMIZER) && !FlagGet(FLAG_BATTLE_FACILITY))
			gMoveToLearn = RandomizeMove(gMoveToLearn);
		#endif

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

		#ifdef FLAG_POKEMON_LEARNSET_RANDOMIZER
		if (FlagGet(FLAG_POKEMON_LEARNSET_RANDOMIZER) && !FlagGet(FLAG_BATTLE_FACILITY))
			gMoveToLearn = RandomizeMove(gMoveToLearn);
		#endif

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

#ifdef FLAG_MOVE_RELEARNER_IGNORE_LEVEL
	if (FlagGet(FLAG_MOVE_RELEARNER_IGNORE_LEVEL))
		level = MAX_LEVEL;
#endif

#ifdef FLAG_EGG_MOVE_RELEARNER
	if (FlagGet(FLAG_EGG_MOVE_RELEARNER))
	{
		numMoves = GetAllEggMoves(mon, moves, TRUE);
		return numMoves;
	}
#endif

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

	for (i = 0; i < MAX_LEARNABLE_MOVES && !(gLevelUpLearnsets[species][i].move == 0
						&& gLevelUpLearnsets[species][i].level == 0xFF); ++i)
	{
		u16 move = gLevelUpLearnsets[species][i].move;

		#ifdef FLAG_POKEMON_LEARNSET_RANDOMIZER
		if (FlagGet(FLAG_POKEMON_LEARNSET_RANDOMIZER) && !FlagGet(FLAG_BATTLE_FACILITY))
			move = RandomizeMove(move);
		#endif

		moves[numMoves++] = move;
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

#ifdef FLAG_POKEMON_LEARNSET_RANDOMIZER
static move_t RandomizeMove(u16 move)
{
	move = (move * T1_READ_32(gSaveBlock2->playerTrainerId));
	move %= NON_Z_MOVE_COUNT;

	if (move == MOVE_NONE || move == MOVE_STRUGGLE)
		return move + 1;

	return move;
}
#endif

u16 BuildLearnableMoveset(struct Pokemon* mon, u16* moves)
{
	u16 numLevelMoves = 0;
	u16 numEggMoves = 0;
	u16 numTotalMoves;
	u16 species = GetMonData(mon, MON_DATA_SPECIES, NULL);
	u16 dexNum = SpeciesToNationalPokedexNum(species);

	numLevelMoves += GetLevelUpMovesBySpecies(species, moves);
	moves += numLevelMoves; //Increase Ptr
	numEggMoves += GetAllEggMoves(mon, moves, FALSE);
	moves += numEggMoves; //Increase Ptr

	numTotalMoves = numLevelMoves + numEggMoves;

	for (int i = 0; i < NUM_TMSHMS; ++i)
	{
		if (CanMonLearnTMHM(mon, i))
		{
			*moves = gTMHMMoves[i];
			++numTotalMoves;
			++moves; //Increase Ptr
		}

		#ifdef EXPANDED_MOVE_TUTORS
		u8 tutRet = CanMonLearnTutorMove(mon, i);
		#else
		u8 tutRet = CanLearnTutorMove(mon->species, i);
		#endif
		if (tutRet == TRUE
		|| (tutRet > TRUE && tutRet == dexNum))
		{
			#ifdef EXPANDED_MOVE_TUTORS
			*moves = GetExpandedTutorMove(i);
			#else
			*moves = GetTutorMove(i);
			#endif

			if (*moves != MOVE_NONE)
			{
				++numTotalMoves;
				++moves; //Increase Ptr
			}
		}
	}

	return numTotalMoves;
}

u16 GiveMoveToBoxMon(struct BoxPokemon *boxMon, u16 move)
{
	for (int i = 0; i < MAX_MON_MOVES; i++)
	{
		u16 existingMove = GetBoxMonData(boxMon, MON_DATA_MOVE1 + i, NULL);
		if (!existingMove)
		{
			SetBoxMonData(boxMon, MON_DATA_MOVE1 + i, &move);
			SetBoxMonData(boxMon, MON_DATA_PP1 + i, &gBattleMoves[move].pp);

			if (move == MOVE_SECRETSWORD && GetBoxMonData(boxMon, MON_DATA_SPECIES, NULL) == SPECIES_KELDEO)
			{
				u16 resolute = SPECIES_KELDEO_RESOLUTE;
				SetBoxMonData(boxMon, MON_DATA_SPECIES, &resolute);
			}

			return move;
		}

		if (existingMove == move)
			return -2;
	}

	return 0xFFFF;
}

void SetMonMoveSlot(struct Pokemon* mon, u16 move, u8 slot)
{
	u16 form;

	if (GetMonData(mon, MON_DATA_MOVE1 + slot, NULL) == MOVE_SECRETSWORD
	&& move != MOVE_SECRETSWORD
	&& GetMonData(mon, MON_DATA_SPECIES, NULL) == SPECIES_KELDEO_RESOLUTE)
	{
		form = SPECIES_KELDEO; //Keldeo reverts if it forgets Secret Sword
		SetMonData(mon, MON_DATA_SPECIES, &form);
	}

	SetMonData(mon, MON_DATA_MOVE1 + slot, &move);
	SetMonData(mon, MON_DATA_PP1 + slot, &gBattleMoves[move].pp);

	if (move == MOVE_SECRETSWORD && GetMonData(mon, MON_DATA_SPECIES, NULL) == SPECIES_KELDEO)
	{
		form = SPECIES_KELDEO_RESOLUTE; //Keldeo transforms if it learns Secret Sword
		SetMonData(mon, MON_DATA_SPECIES, &form);
	}
}

//Move Reminder//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define MAX_MOVE_REMINDER_MOVES (MAX_LEARNABLE_MOVES + 1) //50 moves + cancel

struct MoveRelearner
{
    u8 state;
    u8 unk_01;
    u8 unk_02;
    u8 spriteIds[2];
    u8 filler_05[0x13];
    u8 unk_18;
    u8 scrollPositionMaybe;
    u8 numLearnableMoves;
    u8 numToShowAtOnce;
    u8 unk_1C;
    u8 unk_1D;
    u8 unk_1E;
	struct ListMenuItem menuItems[MAX_MOVE_REMINDER_MOVES];	//0x20 - 0x1b8
	u16 moves[MAX_MOVE_REMINDER_MOVES];
	u8 filler[0x41]; //Recycle old string buffer section
	bool8 scheduleMoveInfoUpdate;	// 0x25F, loads new gfx, data if set to 1 (upon scrolling)
	u8 selectedPartyMember;		// 0x260
	u8 selectedMoveSlot;	//0x261
	u8 unk_262;
    u8 listMenuTaskId;
    u8 bg1TilemapBuffer[BG_SCREEN_SIZE]; // 264
    u8 textColor[3]; // A64
	u8 cursorPos; //0xa67
    u8 selectedIndex;
    u16 listMenuScrollPos;
    u16 listMenuScrollRow;
	const u8 listMenuNames[MAX_MOVE_REMINDER_MOVES][MOVE_NAME_LENGTH + 1];
};

#define gMoveRelearnerStruct (*((struct MoveRelearner**) 0x203AAB4))
#define gText_MoveRelearnerAskTeach (u8*) 0x841E3FB


void InitLearnMoveFix(void)
{
	gMoveRelearnerStruct = Calloc(sizeof(struct MoveRelearner));
}


bool8 CheckMoveRelearnerMoveLimit(u8 counter)
{
	return counter < MAX_MOVE_REMINDER_MOVES;
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


u8 GetRelearnableMoves(struct Pokemon* mon)
{
	int i = 0;
	u8 numMoves = GetMoveRelearnerMoves(mon, &gMoveRelearnerStruct->moves[0]);
	gMoveRelearnerStruct->numLearnableMoves = numMoves;

	if (numMoves > 0)
	{
		for (i = 0; i < numMoves; ++i)
		{
			StringCopy((void*) &gMoveRelearnerStruct->listMenuNames[i], gMoveNames[gMoveRelearnerStruct->moves[i]]);
			gMoveRelearnerStruct->menuItems[i].name = (const u8*) &gMoveRelearnerStruct->listMenuNames[i];
			gMoveRelearnerStruct->menuItems[i].id = i;
		}
	}

	//Set CANCEL as last option
	StringCopy((void*) &gMoveRelearnerStruct->listMenuNames[i], &gText_Cancel[0]);
	gMoveRelearnerStruct->menuItems[i].id = 0xFE;
	gMoveRelearnerStruct->menuItems[i].name = gText_Cancel;
	gMoveRelearnerStruct->numLearnableMoves++;

	//Buffer nickname
	GetMonData(mon, MON_DATA_NICKNAME, gStringVar1);

	return gMoveRelearnerStruct->numLearnableMoves;	//total list count
}


const u8* CopyMoveReminderMoveName(u8 cursor)
{
	StringCopy(gStringVar2, gMoveRelearnerStruct->listMenuNames[cursor]);
	//GetMonData(&gPlayerParty[gMoveRelearnerStruct->selectedPartyMember], MON_DATA_NICKNAME, gStringVar3);
	return gText_MoveRelearnerAskTeach;
}


bool16 InitMoveRelearnerWindows(void)
{
	#ifdef EXPAND_MOVE_REMINDER_DESCRIPTION
		return InitWindows(sMoveRelearnerExpandedTemplates);
	#else
		return InitWindows(sMoveRelearnerWindowTemplates);
	#endif
}
