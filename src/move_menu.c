#include "defines.h"
#include "helper_functions.h"
#include "mega.h"
#include "AI_Helper_Functions.h"

//Include Z-Power Effect under Status-Z name
//Make The Z-Move Names change colour (look in SetPpNumbersPaletteInMoveSelection)
//Send possible Mega Data over link cable

#define SE_SELECT 0x5
#define gText_BattleSwitchWhich (u8*) 0x83FE7A0
#define gText_TypeWord (u8*) 0x83FE76A
#define gText_MoveInterfaceType (u8*) 0x83FE770
#define Pal_PPDisplay (u16*) 0x8D2FBB4

extern u8* ZMoveNames[];
extern u8 gMoveNames[][MOVE_NAME_LENGTH + 1];

extern u8 StringNull[];
extern u8 gText_Power[];

extern const struct Evolution* CanMegaEvolve(u8 bank, bool8 CheckUBInstead);
extern bool8 MegaEvolutionEnabled(u8 bank);
extern bool8 BankMegaEvolved(u8 bank, bool8 checkUB);
extern u8 GetMoveTypeSpecial(u8 bankAtk, move_t);

void HandleInputChooseMove(void);
bool8 TriggerMegaEvolution(void);
void EmitChooseMove(u8 bufferId, bool8 isDoubleBattle, bool8 NoPpNumber, struct ChooseMoveStruct *movePpData);
void EmitMoveChosen(u8 bufferId, u8 chosenMoveIndex, u8 target, u8 megaState, u8 ultraState, u8 zMoveState);
void MoveSelectionDisplayMoveType(void);
bool8 MoveSelectionDisplayZMove(void);
void ZMoveSelectionDisplayPpNumber(void);
void ZMoveSelectionDisplayPower(void);
void ReloadMoveNamesIfNecessary(void);

void HandleInputChooseMove(void)
{
    bool32 canSelectTarget = FALSE;
    struct ChooseMoveStruct* moveInfo = (struct ChooseMoveStruct*)(&gBattleBufferA[gActiveBattler][4]);

	sub_8033AC8();

    if (gMain.newKeys & A_BUTTON) {
        u8 moveTarget;
		
		if (ZMoveData->viewing) {
			ZMoveData->toBeUsed[gActiveBattler] = TRUE;
			ZMoveData->viewing = FALSE;
			gWindows[3].window.width = 8; //Restore Window Size from Z-Move mod
			gWindows[3].window.height = 2;
			Free(gWindows[3].tileData);
			gWindows[3].tileData = ZMoveData->backupTilemap;
			ZMoveData->backupTilemap = NULL;
		}
		
        PlaySE(SE_SELECT);
        if (moveInfo->moves[gMoveSelectionCursor[gActiveBattler]] == MOVE_CURSE)
        {
            if (moveInfo->monType1 != TYPE_GHOST && moveInfo->monType2 != TYPE_GHOST && moveInfo->monType3 != TYPE_GHOST)
                moveTarget = MOVE_TARGET_USER;
            else
                moveTarget = MOVE_TARGET_SELECTED;
        }
		else if (moveInfo->moves[gMoveSelectionCursor[gActiveBattler]] == MOVE_ACUPRESSURE && !(gBattleTypeFlags & BATTLE_TYPE_DOUBLE))
			moveTarget = MOVE_TARGET_USER; //Only can target yourself in singles
        else
            moveTarget = gBattleMoves[moveInfo->moves[gMoveSelectionCursor[gActiveBattler]]].target;

        if (moveTarget & MOVE_TARGET_USER)
            gMultiUsePlayerCursor = gActiveBattler;
        else
            gMultiUsePlayerCursor = GetBattlerAtPosition((GetBattlerPosition(gActiveBattler) & BIT_SIDE) ^ BIT_SIDE);

        if (!gBattleBufferA[gActiveBattler][1]) // not a double battle
        {
            if (moveTarget & MOVE_TARGET_USER_OR_SELECTED && !gBattleBufferA[gActiveBattler][2])
                canSelectTarget++;
        }
        else // double battle
        {
            if (!(moveTarget & (MOVE_TARGET_RANDOM | MOVE_TARGET_BOTH | MOVE_TARGET_DEPENDS | MOVE_TARGET_FOES_AND_ALLY | MOVE_TARGET_OPPONENTS_FIELD | MOVE_TARGET_USER)))
                canSelectTarget++; // either selected or user

            if (moveInfo->currentPp[gMoveSelectionCursor[gActiveBattler]] == 0)
            {
                canSelectTarget = FALSE;
            }
            else if (!(moveTarget & (MOVE_TARGET_USER | MOVE_TARGET_USER_OR_SELECTED)) && CountAliveMons(BATTLE_ALIVE_EXCEPT_ACTIVE) <= 1)
            {
                gMultiUsePlayerCursor = GetDefaultMoveTarget(gActiveBattler); //0x803F6A0
                canSelectTarget = FALSE;
            }
        }

		ResetPaletteFadeControl();
		BeginNormalPaletteFade(0xF0000, 0, 0, 0, 0x7FFF);

        if (!canSelectTarget)
        {
			EmitMoveChosen(1, gMoveSelectionCursor[gActiveBattler], gMultiUsePlayerCursor, MegaData->chosen[gActiveBattler], UltraData->chosen[gActiveBattler], ZMoveData->toBeUsed[gActiveBattler]);
            PlayerBufferExecCompleted();
        }
        else
        {
            gBattleBankFunc[gActiveBattler] = (u32) HandleInputChooseTarget;

            if (moveTarget & (MOVE_TARGET_USER | MOVE_TARGET_USER_OR_SELECTED))
                gMultiUsePlayerCursor = gActiveBattler;
            else if (gAbsentBattlerFlags & gBitTable[GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT)])
                gMultiUsePlayerCursor = GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT);
            else
                gMultiUsePlayerCursor = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);

            gSprites[gBattlerSpriteIds[gMultiUsePlayerCursor]].callback = sub_8012044; //sub_8039AD8 in Emerald
        }
    }
    else if (gMain.newKeys & B_BUTTON)
    {
		if (ZMoveData->viewing) {
			ZMoveData->viewing = FALSE;
			gWindows[3].window.width = 8; //Restore Window Size from Z-Move mod
			gWindows[3].window.height = 2;
			Free(gWindows[3].tileData);
			gWindows[3].tileData = ZMoveData->backupTilemap;
			ZMoveData->backupTilemap = NULL;
		}
		gMoveSelectionCursor[gActiveBattler] = 0;
        PlaySE(SE_SELECT);
        EmitTwoReturnValues(1, 10, 0xFFFF);
        PlayerBufferExecCompleted();
		ResetPaletteFadeControl();
    }
    else if (gMain.newKeys & DPAD_LEFT)
    {
        if (gMoveSelectionCursor[gActiveBattler] & 1)
        {
			ReloadMoveNamesIfNecessary();
            MoveSelectionDestroyCursorAt(gMoveSelectionCursor[gActiveBattler]); //0x8030A14
            gMoveSelectionCursor[gActiveBattler] ^= 1;
            PlaySE(SE_SELECT);
            MoveSelectionCreateCursorAt(gMoveSelectionCursor[gActiveBattler], 0); //0x80309CC
            MoveSelectionDisplayPpNumber(); //0x80309CC
            MoveSelectionDisplayMoveType();
			BeginNormalPaletteFade(0xF0000, 0, 0, 0, 0x7FFF);
        }
    }
    else if (gMain.newKeys & DPAD_RIGHT)
    {
        if (!(gMoveSelectionCursor[gActiveBattler] & 1)
         && (gMoveSelectionCursor[gActiveBattler] ^ 1) < gNumberOfMovesToChoose)
        {
			ReloadMoveNamesIfNecessary();
            MoveSelectionDestroyCursorAt(gMoveSelectionCursor[gActiveBattler]);
            gMoveSelectionCursor[gActiveBattler] ^= 1;
            PlaySE(SE_SELECT);
            MoveSelectionCreateCursorAt(gMoveSelectionCursor[gActiveBattler], 0);
            MoveSelectionDisplayPpNumber();
            MoveSelectionDisplayMoveType();
			BeginNormalPaletteFade(0xF0000, 0, 0, 0, 0x7FFF);
        }
    }
    else if (gMain.newKeys & DPAD_UP)
    {
        if (gMoveSelectionCursor[gActiveBattler] & 2)
        {
			ReloadMoveNamesIfNecessary();
            MoveSelectionDestroyCursorAt(gMoveSelectionCursor[gActiveBattler]);
            gMoveSelectionCursor[gActiveBattler] ^= 2;
            PlaySE(SE_SELECT);
            MoveSelectionCreateCursorAt(gMoveSelectionCursor[gActiveBattler], 0);
            MoveSelectionDisplayPpNumber();
            MoveSelectionDisplayMoveType();
			BeginNormalPaletteFade(0xF0000, 0, 0, 0, 0x7FFF);
        }
    }
    else if (gMain.newKeys & DPAD_DOWN)
    {
        if (!(gMoveSelectionCursor[gActiveBattler] & 2)
         && (gMoveSelectionCursor[gActiveBattler] ^ 2) < gNumberOfMovesToChoose)
        {
			ReloadMoveNamesIfNecessary();
            MoveSelectionDestroyCursorAt(gMoveSelectionCursor[gActiveBattler]);
            gMoveSelectionCursor[gActiveBattler] ^= 2;
            PlaySE(SE_SELECT);
            MoveSelectionCreateCursorAt(gMoveSelectionCursor[gActiveBattler], 0);
            MoveSelectionDisplayPpNumber();
            MoveSelectionDisplayMoveType();
			BeginNormalPaletteFade(0xF0000, 0, 0, 0, 0x7FFF);
        }
    }
    else if (gMain.newKeys & SELECT_BUTTON)
    {
        if (gNumberOfMovesToChoose > 1 && !(gBattleTypeFlags & BATTLE_TYPE_LINK) && !ZMoveData->viewing)
        {
            MoveSelectionCreateCursorAt(gMoveSelectionCursor[gActiveBattler], 29);

            if (gMoveSelectionCursor[gActiveBattler] != 0)
                gMultiUsePlayerCursor = 0;
            else
                gMultiUsePlayerCursor = gMoveSelectionCursor[gActiveBattler] + 1;

            MoveSelectionCreateCursorAt(gMultiUsePlayerCursor, 27);
            BattlePutTextOnWindow(gText_BattleSwitchWhich, 0xB); //0x80D87BC
            gBattleBankFunc[gActiveBattler] = (u32) HandleMoveSwitching; //0x802EF58
        }
    }
	else if (gMain.newKeys & START_BUTTON)
	{
		if (!MoveSelectionDisplayZMove()) //Only one is allowed at a time
			TriggerMegaEvolution();
	}
	else if (gMain.newKeys & R_BUTTON)
	{
		if (!MoveSelectionDisplayZMove()) //Only one is allowed at a time
			TriggerMegaEvolution();
	}
}

bool8 TriggerMegaEvolution(void) {
	
	const struct Evolution* evolutions = CanMegaEvolve(gActiveBattler, FALSE);
	
	if (evolutions == NULL)
		evolutions = CanMegaEvolve(gActiveBattler, TRUE); //Check Ultra Burst
	
	if (evolutions == NULL)
		return FALSE;

	if (evolutions->unknown == MEGA_VARIANT_ULTRA_BURST) { //Ultra Burst
		if (!BankMegaEvolved(gActiveBattler, TRUE)) //Checking Ultra Burst
		{
			if (UltraData->chosen[gActiveBattler]) {
				// Turn Off
				PlaySE(3);
				UltraData->chosen[gActiveBattler] = FALSE;
				return TRUE;
			} 
			else {
				// Turn On
				PlaySE(2);
				UltraData->chosen[gActiveBattler] = TRUE;
				return TRUE;
			}
		}
	}
	
	else if (!BankMegaEvolved(gActiveBattler, FALSE) 
		  && MegaEvolutionEnabled(gActiveBattler) 
		  && !(ZMoveData->partyIndex[SIDE(gActiveBattler)] & gBitTable[gBattlerPartyIndexes[gActiveBattler]])) //No Mega Evolving if you've used a Z-Move (*cough* *cough* Rayquaza)
	{ //Regular Megas
		if (MegaData->chosen[gActiveBattler]) {
			// Turn Off
			PlaySE(3);
			MegaData->chosen[gActiveBattler] = 0;
			return TRUE;
		} 
		else {
			// Turn On
			PlaySE(2);
			MegaData->chosen[gActiveBattler] = 1;
			return TRUE;
		}
	}
	return FALSE;
}

//This function sends useful data over Link Cable for the move menu to use
void EmitChooseMove(u8 bufferId, bool8 isDoubleBattle, bool8 NoPpNumber, struct ChooseMoveStruct *movePpData)
{
    u32 i;
	
	u8* moveTypes = Malloc(sizeof(u8) * MAX_MON_MOVES);
	
	for (i = 0; i < MAX_MON_MOVES; ++i)
		moveTypes[i] = GetMoveTypeSpecial(gActiveBattler, gBattleMons[gActiveBattler].moves[i]);

    gBattleBuffersTransferData[0] = CONTROLLER_CHOOSEMOVE;
    gBattleBuffersTransferData[1] = isDoubleBattle;
    gBattleBuffersTransferData[2] = NoPpNumber;
    gBattleBuffersTransferData[3] = 0;
    for (i = 0; i < sizeof(*movePpData) - 6; i++)
        gBattleBuffersTransferData[4 + i] = *((u8*)(movePpData) + i);
	gBattleBuffersTransferData[4 + i] = moveTypes[0];
	gBattleBuffersTransferData[5 + i] = moveTypes[1];
	gBattleBuffersTransferData[6 + i] = moveTypes[2];
	gBattleBuffersTransferData[7 + i] = moveTypes[3];
	gBattleBuffersTransferData[8 + i] = gBattleMons[gActiveBattler].type3;
    PrepareBufferDataTransfer(bufferId, gBattleBuffersTransferData, (sizeof(*movePpData) - 1) + 9);
}

void EmitMoveChosen(u8 bufferId, u8 chosenMoveIndex, u8 target, u8 megaState, u8 ultraState, u8 zMoveState)
{
    gBattleBuffersTransferData[0] = CONTROLLER_TWORETURNVALUES;
    gBattleBuffersTransferData[1] = ACTION_RUN_BATTLESCRIPT;
    gBattleBuffersTransferData[2] = chosenMoveIndex;
    gBattleBuffersTransferData[3] = target;
	gBattleBuffersTransferData[4] = megaState;
	gBattleBuffersTransferData[5] = ultraState;
	gBattleBuffersTransferData[6] = zMoveState;
    PrepareBufferDataTransfer(bufferId, gBattleBuffersTransferData, 7);
}

void MoveSelectionDisplayMoveType(void)
{
    u8 *txtPtr;
    struct ChooseMoveStruct *moveInfo = (struct ChooseMoveStruct*)(&gBattleBufferA[gActiveBattler][4]);

    txtPtr = StringCopy(gDisplayedStringBattle, gText_TypeWord);
    txtPtr[0] = EXT_CTRL_CODE_BEGIN;
    txtPtr++;
    txtPtr[0] = 6;
    txtPtr++;
    txtPtr[0] = 1;
    txtPtr++;
	txtPtr = StringCopy(txtPtr, gText_MoveInterfaceType);
	
	#ifdef DISPLAY_REAL_MOVE_TYPE_ON_MENU
		StringCopy(txtPtr, gTypeNames[moveInfo->moveTypes[gMoveSelectionCursor[gActiveBattler]]]); //This isn't working
	#else
		StringCopy(txtPtr, gTypeNames[gBattleMoves[moveInfo->moves[gMoveSelectionCursor[gActiveBattler]]].type]);
	#endif
    BattlePutTextOnWindow(gDisplayedStringBattle, 8);
}

bool8 MoveSelectionDisplayZMove(void) {
	if (ZMoveData->used[gActiveBattler] || (MegaData->partyIndex[SIDE(gActiveBattler)] & gBitTable[gBattlerPartyIndexes[gActiveBattler]]))
		return FALSE;

	struct ChooseMoveStruct *moveInfo = (struct ChooseMoveStruct*)(&gBattleBufferA[gActiveBattler][4]);
	u16 zmove = ShouldAIUseZMove(gActiveBattler, 0, moveInfo->moves[gMoveSelectionCursor[gActiveBattler]]);
	u8 i;
	
	if (ZMoveData->viewing) {
		ReloadMoveNamesIfNecessary();
		PlaySE(3); //Turn Off
		return TRUE;
	}
	
	if (zmove) {
		for (i = 0; i < MAX_MON_MOVES; ++i) {
			MoveSelectionDestroyCursorAt(i);
			StringCopy(gDisplayedStringBattle, StringNull);
			BattlePutTextOnWindow(gDisplayedStringBattle, i + 3);
		}
		
		if (zmove == 0xFFFF) {
			gDisplayedStringBattle[0] = 0xD4;
			gDisplayedStringBattle[1] = 0xAE; //Z-
			StringCopy(gDisplayedStringBattle + 2, gMoveNames[moveInfo->moves[gMoveSelectionCursor[gActiveBattler]]]);
		}
		else
			StringCopy(gDisplayedStringBattle, ZMoveNames[zmove - MOVE_BREAKNECK_BLITZ_P]);
		
		gWindows[3].window.width = 17;
		gWindows[3].window.height = 4; //Double the size to help super long Z-move names
		ZMoveData->backupTilemap = gWindows[3].tileData;
		gWindows[3].tileData = Calloc(0x880); //Because of the size expansion of the tile map, new memory was necessary to display the Z-Move name
		BattlePutTextOnWindow(gDisplayedStringBattle, 3);
		ZMoveSelectionDisplayPpNumber();
		ZMoveSelectionDisplayPower();
		MoveSelectionCreateCursorAt(0, 0);
		ZMoveData->viewing = TRUE;
		PlaySE(2); //Turn On
		return TRUE;
	}
	
	return FALSE;
}

void ZMoveSelectionDisplayPpNumber(void)
{
    u8 *txtPtr;
    const u16* palPtr = Pal_PPDisplay;

	//Remove Palette Fading On The PP
    gPlttBufferUnfaded[92] = palPtr[(3 * 2) + 0];
    gPlttBufferUnfaded[91] = palPtr[(3 * 2) + 1];

    CpuCopy16(&gPlttBufferUnfaded[92], &gPlttBufferFaded[92], sizeof(u16));
    CpuCopy16(&gPlttBufferUnfaded[91], &gPlttBufferFaded[91], sizeof(u16));

	//Load PP Text
    txtPtr = ConvertIntToDecimalStringN(gDisplayedStringBattle, 1, STR_CONV_MODE_RIGHT_ALIGN, 2);
    txtPtr[0] = CHAR_SLASH;
    txtPtr++;
    ConvertIntToDecimalStringN(txtPtr, 1, STR_CONV_MODE_RIGHT_ALIGN, 2);

    BattlePutTextOnWindow(gDisplayedStringBattle, 9);
}

void ZMoveSelectionDisplayPower(void) {
	u8 *txtPtr;
	struct ChooseMoveStruct *moveInfo = (struct ChooseMoveStruct*)(&gBattleBufferA[gActiveBattler][4]);
	u16 move = moveInfo->moves[gMoveSelectionCursor[gActiveBattler]];
	u8 power = gBattleMoves[move].z_move_power;
	
	u16 zmove = ShouldAIUseZMove(gActiveBattler, 0, moveInfo->moves[gMoveSelectionCursor[gActiveBattler]]);
	
	if (zmove >= MOVE_CATASTROPIKA)
		power = gBattleMoves[zmove].power;
	
	if (SPLIT(move) != SPLIT_STATUS) {
	/* 	The preffered way was to display the power string under the Z-Move name.
		However, it causes too many graphical issues that I don't know how to fix.

		txtPtr = StringCopy(gDisplayedStringBattle, gText_Power);
		ConvertIntToDecimalStringN(txtPtr, power, STR_CONV_MODE_LEFT_ALIGN, 3);
		BattlePutTextOnWindow(gDisplayedStringBattle, 3 + 3); //Slot of Move 3
	*/
		txtPtr = StringCopy(gDisplayedStringBattle, gText_Power);
		txtPtr[0] = EXT_CTRL_CODE_BEGIN;
		txtPtr++;
		txtPtr[0] = 6;
		txtPtr++;
		txtPtr[0] = 1;
		txtPtr++;
		ConvertIntToDecimalStringN(txtPtr, power, STR_CONV_MODE_LEFT_ALIGN, 3);
		BattlePutTextOnWindow(gDisplayedStringBattle, 8); //Slot of Move 3
	}
	else {
	}
}


void ReloadMoveNamesIfNecessary(void) {
	if (ZMoveData->viewing) {
		ZMoveData->viewing = FALSE;
		gWindows[3].window.width = 8; //Restore Window Size from Z-Move mod
		gWindows[3].window.height = 2;
		Free(gWindows[3].tileData);
		gWindows[3].tileData = ZMoveData->backupTilemap;
		ZMoveData->backupTilemap = NULL;
		MoveSelectionDestroyCursorAt(0);
		MoveSelectionDisplayMoveNames();
		MoveSelectionCreateCursorAt(gMoveSelectionCursor[gActiveBattler], 0);
		MoveSelectionDisplayPpNumber();
		MoveSelectionDisplayMoveType();
	}
}