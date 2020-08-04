#include "defines.h"
#include "defines_battle.h"
#include "../include/sprite.h"
#include "../include/string_util.h"
#include "../include/window.h"
#include "../include/constants/songs.h"

#include "../include/new/accuracy_calc.h"
#include "../include/new/ai_util.h"
#include "../include/new/battle_indicators.h"
#include "../include/new/battle_util.h"
#include "../include/new/damage_calc.h"
#include "../include/new/dynamax.h"
#include "../include/new/general_bs_commands.h"
#include "../include/new/item.h"
#include "../include/new/frontier.h"
#include "../include/new/mega.h"
#include "../include/new/move_menu.h"
#include "../include/new/move_menu_battle_scripts.h"
#include "../include/new/move_tables.h"
#include "../include/new/multi.h"
#include "../include/new/set_z_effect.h"
#include "../include/new/text.h"
#include "../include/new/util.h"
#include "../include/new/z_move_effects.h"

/*
move_menu.c
	functions for displaying move data and z moves in the battle menu

tables:
	sTypeIconPositions
	sTypeIconPicTable

*/

//TODO: Make The Z-Move Names change colour (look in SetPpNumbersPaletteInMoveSelection)
//Fix Heal Block + Pollen Puff on partner link battle

extern const u8 gMoveNames[][MOVE_NAME_LENGTH + 1];
extern const u8 gTypeNames[][TYPE_NAME_LENGTH + 1];
extern const u8 gDynamaxMovePowers[MOVES_COUNT];
extern const u8 sTargetIdentities[];
extern const u16 gUserInterfaceGfx_TypeHighlightingPal[];
extern const u8 gMoveEffectsThatIgnoreWeaknessResistance[];
extern const u8 CamomonsTypeIconsTiles[];
extern const u8 CamomonsTypeIcons2Tiles[];
extern const u16 CamomonsTypeIconsPal[];
extern const u16 CamomonsTypeIcons2Pal[];

//This file's functions:
static bool8 TriggerMegaEvolution(void);
static void MoveNameToDisplayedStringBattle(u8 moveIndex);
static void MoveSelectionDisplayMoveNames(void);
static void MoveSelectionDisplayMoveType(void);
static bool8 MoveSelectionDisplayZMove(void);
static bool8 MoveSelectionDisplayMaxMove(void);
static void ZMoveSelectionDisplayPpNumber(void);
static void ZMoveSelectionDisplayPower(void);
static void MaxMoveSelectionDisplayPower(void);
static void MoveSelectionDisplayDetails(void);
static void ReloadMoveNamesIfNecessary(void);
static void CloseZMoveDetails(void);
static void CloseMaxMoveDetails(void);
static void TryLoadTypeIcons(void);
static void SpriteCB_CamomonsTypeIcon(struct Sprite* sprite);

static const struct Coords16 sTypeIconPositions[][/*IS_SINGLE_BATTLE*/2] =
{
#ifndef UNBOUND //MODIFY THIS
	[B_POSITION_PLAYER_LEFT] =
	{
		[TRUE] = {221, 86}, 	//Single Battle
		[FALSE] = {144, 70},	//Double Battle
	},
	[B_POSITION_OPPONENT_LEFT] =
	{
		[TRUE] = {20, 26}, 		//Single Battle
		[FALSE] = {97, 14},		//Double Battle
	},
	[B_POSITION_PLAYER_RIGHT] =
	{
		[FALSE] = {156, 96},	//Double Battle
	},
	[B_POSITION_OPPONENT_RIGHT] =
	{
		[FALSE] = {85, 39},		//Double Battle
	},
#else //For Pokemon Unbound
	[B_POSITION_PLAYER_LEFT] =
	{
		[TRUE] = {225, 86}, 	//Single Battle
		[FALSE] = {142, 70},	//Double Battle
	},
	[B_POSITION_OPPONENT_LEFT] =
	{
		[TRUE] = {18, 26}, 		//Single Battle
		[FALSE] = {99, 15},		//Double Battle
	},
	[B_POSITION_PLAYER_RIGHT] =
	{
		[FALSE] = {154, 96},	//Double Battle
	},
	[B_POSITION_OPPONENT_RIGHT] =
	{
		[FALSE] = {87, 40},		//Double Battle
	},
#endif
};

static const struct OamData sTypeIconOAM =
{
	.affineMode = ST_OAM_AFFINE_OFF,
	.objMode = ST_OAM_OBJ_NORMAL,
	.shape = SPRITE_SHAPE(8x16),
	.size = SPRITE_SIZE(8x16),
	.priority = 1, //Same level as health bar
};

#define type_icon_frame(ptr, frame) {.data = (u8 *)ptr + (1 * 2 * frame * 32), .size = 1 * 2 * 32}
static const struct SpriteFrameImage sTypeIconPicTable[] =
{
	[TYPE_NORMAL] =		type_icon_frame(CamomonsTypeIconsTiles, TYPE_NORMAL),
	[TYPE_FIGHTING] =	type_icon_frame(CamomonsTypeIconsTiles, TYPE_FIGHTING),
	[TYPE_FLYING] =		type_icon_frame(CamomonsTypeIcons2Tiles, TYPE_FLYING),
	[TYPE_POISON] =		type_icon_frame(CamomonsTypeIcons2Tiles, TYPE_POISON),
	[TYPE_GROUND] =		type_icon_frame(CamomonsTypeIcons2Tiles, TYPE_GROUND),
	[TYPE_ROCK] =		type_icon_frame(CamomonsTypeIconsTiles, TYPE_ROCK),
	[TYPE_BUG] =		type_icon_frame(CamomonsTypeIconsTiles, TYPE_BUG),
	[TYPE_GHOST] =		type_icon_frame(CamomonsTypeIconsTiles, TYPE_GHOST),
	[TYPE_STEEL] =		type_icon_frame(CamomonsTypeIconsTiles, TYPE_STEEL),
	[TYPE_MYSTERY] =	type_icon_frame(CamomonsTypeIcons2Tiles, TYPE_MYSTERY),
	[TYPE_FIRE] =		type_icon_frame(CamomonsTypeIconsTiles, TYPE_FIRE),
	[TYPE_WATER] =		type_icon_frame(CamomonsTypeIconsTiles, TYPE_WATER),
	[TYPE_GRASS] =		type_icon_frame(CamomonsTypeIconsTiles, TYPE_GRASS),
	[TYPE_ELECTRIC] =	type_icon_frame(CamomonsTypeIconsTiles, TYPE_ELECTRIC),
	[TYPE_PSYCHIC] =	type_icon_frame(CamomonsTypeIconsTiles, TYPE_PSYCHIC),
	[TYPE_ICE] =		type_icon_frame(CamomonsTypeIconsTiles, TYPE_ICE),
	[TYPE_DRAGON] =		type_icon_frame(CamomonsTypeIcons2Tiles, TYPE_DRAGON),
	[TYPE_DARK] =		type_icon_frame(CamomonsTypeIconsTiles, TYPE_DARK),
	[0x12] =			type_icon_frame(CamomonsTypeIcons2Tiles, TYPE_MYSTERY),
	[TYPE_ROOSTLESS] = 	type_icon_frame(CamomonsTypeIcons2Tiles, TYPE_MYSTERY),
	[TYPE_BLANK] = 		type_icon_frame(CamomonsTypeIcons2Tiles, TYPE_MYSTERY),
	[0x15] = 			type_icon_frame(CamomonsTypeIcons2Tiles, TYPE_MYSTERY),
	[0x16] = 			type_icon_frame(CamomonsTypeIcons2Tiles, TYPE_MYSTERY),
	[TYPE_FAIRY] = 		type_icon_frame(CamomonsTypeIconsTiles, TYPE_FAIRY),
};

static struct SpriteTemplate sTypeIconSpriteTemplate =
{
	.tileTag = 0xFFFF,
	.paletteTag = TYPE_ICON_TAG,
	.oam = &sTypeIconOAM,
	.anims = gDummySpriteAnimTable,
	.images = sTypeIconPicTable,
	.affineAnims = gDummySpriteAffineAnimTable,
	.callback = SpriteCB_CamomonsTypeIcon,
};

static struct SpriteTemplate sTypeIconSpriteTemplate2 =
{
	.tileTag = 0xFFFF,
	.paletteTag = TYPE_ICON_TAG_2,
	.oam = &sTypeIconOAM,
	.anims = gDummySpriteAnimTable,
	.images = sTypeIconPicTable,
	.affineAnims = gDummySpriteAffineAnimTable,
	.callback = SpriteCB_CamomonsTypeIcon,
};

static const struct SpritePalette sTypeIconPalTemplate =
{
	.data = CamomonsTypeIconsPal,
	.tag = TYPE_ICON_TAG,
};

static const struct SpritePalette sTypeIconPalTemplate2 =
{
	.data = CamomonsTypeIcons2Pal,
	.tag = TYPE_ICON_TAG_2,
};

void InitMoveSelectionsVarsAndStrings(void)
{
	TryLoadMegaTriggers();
	TryLoadZTrigger();
	TryLoadDynamaxTrigger();
	TryLoadTypeIcons();
	MoveSelectionDisplayMoveNames();
	gMultiUsePlayerCursor = 0xFF;
	MoveSelectionCreateCursorAt(gMoveSelectionCursor[gActiveBattler], 0);
	MoveSelectionDisplayPpString();
	MoveSelectionDisplayPpNumber();
	MoveSelectionDisplayMoveType();
}

void HandleInputChooseMove(void)
{
	bool32 canSelectTarget = FALSE;
	struct ChooseMoveStruct* moveInfo = (struct ChooseMoveStruct*)(&gBattleBufferA[gActiveBattler][4]);
	gNewBS->zMoveData.used[gActiveBattler] = moveInfo->zMoveUsed; //So the link opponent's previous Z-selection can't be used
	gNewBS->megaData.done[gActiveBattler] = moveInfo->megaDone; //So the link opponent's previous Z-selection can't be used
	gNewBS->ultraData.done[gActiveBattler] = moveInfo->ultraDone;
	gNewBS->dynamaxData.used[gActiveBattler] = moveInfo->dynamaxDone;

	if (moveInfo->zMoveUsed)
		gNewBS->zMoveData.toBeUsed[gActiveBattler] = FALSE;

	if (moveInfo->megaDone)
		gNewBS->megaData.chosen[gActiveBattler] = FALSE;

	if (moveInfo->ultraDone)
		gNewBS->ultraData.chosen[gActiveBattler] = FALSE;

	if (moveInfo->dynamaxDone)
		gNewBS->dynamaxData.toBeUsed[gActiveBattler] = FALSE;

	gNewBS->zMoveData.partyIndex[SIDE(gActiveBattler)] = moveInfo->zPartyIndex;
	gNewBS->dynamaxData.partyIndex[SIDE(gActiveBattler)] = moveInfo->dynamaxPartyIndex;

	sub_8033AC8();

	if (gMain.newKeys & A_BUTTON)
	{
		u8 moveTarget;
		u16 chosenMove = moveInfo->moves[gMoveSelectionCursor[gActiveBattler]];

		PlaySE(SE_SELECT);
		if (chosenMove == MOVE_CURSE)
		{
			if (moveInfo->monType1 != TYPE_GHOST && moveInfo->monType2 != TYPE_GHOST && moveInfo->monType3 != TYPE_GHOST)
				moveTarget = MOVE_TARGET_USER;
			else
				moveTarget = MOVE_TARGET_SELECTED;
		}
		else if (chosenMove == MOVE_ACUPRESSURE && !(IS_DOUBLE_BATTLE))
			moveTarget = MOVE_TARGET_USER; //Only can target yourself in singles
		else
			moveTarget = gBattleMoves[chosenMove].target;

		if (gNewBS->zMoveData.viewing && SPLIT(chosenMove) != SPLIT_STATUS) //Status moves keep original targets
			moveTarget = gBattleMoves[moveInfo->possibleZMoves[gMoveSelectionCursor[gActiveBattler]]].target;

		if (gNewBS->dynamaxData.viewing || moveInfo->dynamaxed)
			moveTarget = gBattleMoves[moveInfo->possibleMaxMoves[gMoveSelectionCursor[gActiveBattler]]].target;

		if (moveTarget & MOVE_TARGET_USER)
			gMultiUsePlayerCursor = gActiveBattler;
		else
			gMultiUsePlayerCursor = GetBattlerAtPosition((GetBattlerPosition(gActiveBattler) & BIT_SIDE) ^ BIT_SIDE);

		if (gBattleBufferA[gActiveBattler][1]) // double battle
		{
			if (!(moveTarget & (MOVE_TARGET_RANDOM | MOVE_TARGET_BOTH | MOVE_TARGET_DEPENDS | MOVE_TARGET_FOES_AND_ALLY | MOVE_TARGET_OPPONENTS_FIELD | MOVE_TARGET_USER)))
			{
				if (!(gNewBS->dynamaxData.viewing || moveInfo->dynamaxed) //Not a max move
				|| CountAliveMonsInBattle(BATTLE_ALIVE_DEF_SIDE, gActiveBattler, FOE(gActiveBattler)) > 1) //At least two targets
					canSelectTarget++; // either selected or user
			}

			if (moveInfo->currentPp[gMoveSelectionCursor[gActiveBattler]] == 0)
			{
				canSelectTarget = FALSE;
			}
			else if (!(moveTarget & (MOVE_TARGET_USER | MOVE_TARGET_USER_OR_PARTNER)) && CountAliveMons(BATTLE_ALIVE_EXCEPT_ACTIVE) <= 1)
			{
				gMultiUsePlayerCursor = GetDefaultMoveTarget(gActiveBattler); //0x803F6A0
				canSelectTarget = FALSE;
			}
		}

		ResetPaletteFadeControl();
		BeginNormalPaletteFade(0xF0000, 0, 0, 0, 0x7FFF);

		if (!canSelectTarget)
		{
			CloseZMoveDetails();
			CloseMaxMoveDetails();
			TryRemoveDoublesKillingScore(gActiveBattler, gMultiUsePlayerCursor, chosenMove);
			EmitMoveChosen(1, gMoveSelectionCursor[gActiveBattler], gMultiUsePlayerCursor, gNewBS->megaData.chosen[gActiveBattler], gNewBS->ultraData.chosen[gActiveBattler], gNewBS->zMoveData.toBeUsed[gActiveBattler], gNewBS->dynamaxData.toBeUsed[gActiveBattler]);
			PlayerBufferExecCompleted();
		}
		else
		{
			gBattlerControllerFuncs[gActiveBattler] = HandleInputChooseTarget;

			if (moveTarget & (MOVE_TARGET_USER | MOVE_TARGET_USER_OR_PARTNER))
				gMultiUsePlayerCursor = gActiveBattler;
			else if (gAbsentBattlerFlags & gBitTable[GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT)])
				gMultiUsePlayerCursor = GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT);
			else
				gMultiUsePlayerCursor = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);

			gSprites[gBattlerSpriteIds[gMultiUsePlayerCursor]].callback = sub_8012044; //sub_8039AD8 in Emerald
			MoveSelectionDisplayMoveType();
		}
	}
	else if (gMain.newKeys & B_BUTTON)
	{
		gNewBS->zMoveData.viewing = FALSE;
		gNewBS->zMoveData.viewingDetails = FALSE;
		gNewBS->dynamaxData.viewing = FALSE;
		gMoveSelectionCursor[gActiveBattler] = 0;
		PlaySE(SE_SELECT);
		EmitTwoReturnValues(1, 10, 0xFFFF);
		PlayerBufferExecCompleted();
		ResetPaletteFadeControl();
		BeginNormalPaletteFade(0xF0000, 0, 0, 0, 0x7FFF);
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
		if (gNumberOfMovesToChoose > 1 && !(gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_FRONTIER))
		&& !gNewBS->dynamaxData.viewing
		&& !gNewBS->zMoveData.viewing
		&& !gNewBS->zMoveData.viewingDetails)
		{
			MoveSelectionCreateCursorAt(gMoveSelectionCursor[gActiveBattler], 29);

			if (gMoveSelectionCursor[gActiveBattler] != 0)
				gMultiUsePlayerCursor = 0;
			else
				gMultiUsePlayerCursor = gMoveSelectionCursor[gActiveBattler] + 1;

			MoveSelectionCreateCursorAt(gMultiUsePlayerCursor, 27);
			BattlePutTextOnWindow(gText_BattleSwitchWhich, 0xB); //0x80D87BC
			gBattlerControllerFuncs[gActiveBattler] = HandleMoveSwitching; //0x802EF58
		}
	}
	else if (gMain.newKeys & (START_BUTTON | R_BUTTON))
	{
		if (!MoveSelectionDisplayZMove()) //Only one is allowed at a time
		{
			if (!TriggerMegaEvolution() && gBattleTypeFlags & BATTLE_TYPE_DYNAMAX)
				MoveSelectionDisplayMaxMove();
		}
	}
	else if (gMain.newKeys & L_BUTTON)
	{
		if (!gNewBS->zMoveData.viewing && !gNewBS->dynamaxData.viewing)
			MoveSelectionDisplayDetails();
	}
}

static bool8 TriggerMegaEvolution(void)
{
	#ifndef MEGA_EVOLUTION_FEATURE
		return FALSE;
	#else
	struct ChooseMoveStruct *moveInfo = (struct ChooseMoveStruct*)(&gBattleBufferA[gActiveBattler][4]);
	if (!moveInfo->canMegaEvolve)
		return FALSE;

	if (moveInfo->megaVariance == MEGA_VARIANT_ULTRA_BURST) //Ultra Burst
	{
		if (gNewBS->ultraData.chosen[gActiveBattler])
		{ // Turn Off
			PlaySE(3);
			gNewBS->ultraData.chosen[gActiveBattler] = FALSE;
			return TRUE;
		}
		else
		{ // Turn On
			PlaySE(2);
			gNewBS->ultraData.chosen[gActiveBattler] = TRUE;
			return TRUE;
		}
	}
	else //Regular Megas
	{
		if (gNewBS->megaData.chosen[gActiveBattler])
		{
			// Turn Off
			PlaySE(3);
			gNewBS->megaData.chosen[gActiveBattler] = 0;
			return TRUE;
		}
		else
		{
			// Turn On
			PlaySE(2);
			gNewBS->megaData.chosen[gActiveBattler] = 1;
			return TRUE;
		}
	}
	return FALSE;
	#endif
}

//This function sends useful data over Link Cable for the move menu to use
void EmitChooseMove(u8 bufferId, bool8 isDoubleBattle, bool8 NoPpNumber, struct ChooseMoveStruct *movePpData)
{
	u32 i, j;
	const struct Evolution* evolutions;

	struct ChooseMoveStruct* tempMoveStruct = Calloc(sizeof(struct ChooseMoveStruct)); //Make space for new expanded data
	Memcpy(tempMoveStruct, movePpData, sizeof(struct ChooseMoveStructOld)); //Copy the old data
	tempMoveStruct->monType1 = gBattleMons[gActiveBattler].type1;
	tempMoveStruct->monType2 = gBattleMons[gActiveBattler].type2;
	tempMoveStruct->monType3 = gBattleMons[gActiveBattler].type3;
	tempMoveStruct->ability = ABILITY(gActiveBattler);

	//Fix Transformed Move PP
	if (IS_TRANSFORMED(gActiveBattler))
	{
		for (i = 0; i < MAX_MON_MOVES; ++i)
		{
			if (tempMoveStruct->maxPp[i] > 5)
				tempMoveStruct->maxPp[i] = 5;
		}
	}

	#ifdef DYNAMAX_FEATURE
	tempMoveStruct->dynamaxed = IsDynamaxed(gActiveBattler);
	tempMoveStruct->dynamaxDone = gNewBS->dynamaxData.used[gActiveBattler];
	if ((!gNewBS->dynamaxData.used[gActiveBattler] || IsDynamaxed(gActiveBattler))
	&& DynamaxEnabled(gActiveBattler)
	&& MonCanDynamax(GetBankPartyData(gActiveBattler))
	&& !BATTLER_SEMI_INVULNERABLE(gActiveBattler))
	{
		for (i = 0; i < MAX_MON_MOVES; ++i)
			tempMoveStruct->possibleMaxMoves[i] = GetMaxMove(gActiveBattler, i);
	}
	#endif

	gBattleScripting.dmgMultiplier = 1;
	for (i = 0; i < MAX_MON_MOVES; ++i)
	{
		u8 foe = (IS_DOUBLE_BATTLE && !BATTLER_ALIVE(FOE(gActiveBattler))) ? PARTNER(FOE(gActiveBattler)) : FOE(gActiveBattler);
		u16 originalMove = gBattleMons[gActiveBattler].moves[i];
		u16 move = (tempMoveStruct->dynamaxed) ? tempMoveStruct->possibleMaxMoves[i] : originalMove;

		tempMoveStruct->moveTypes[i] = GetMoveTypeSpecial(gActiveBattler, move);

		if (IS_DOUBLE_BATTLE && CountAliveMonsInBattle(BATTLE_ALIVE_DEF_SIDE, gActiveBattler, foe) >= 2) //Because target can vary, display only attacker's modifiers
		{
			tempMoveStruct->movePowers[i] = CalcVisualBasePower(gActiveBattler, gActiveBattler, move, TRUE);
			tempMoveStruct->moveAcc[i] = VisualAccuracyCalc_NoTarget(move, gActiveBattler);

			if (tempMoveStruct->possibleMaxMoves[i] != MOVE_NONE)
			{
				gNewBS->ai.zMoveHelper = originalMove;
				tempMoveStruct->maxMovePowers[i] = CalcVisualBasePower(gActiveBattler, gActiveBattler, tempMoveStruct->possibleMaxMoves[i], TRUE);
				gNewBS->ai.zMoveHelper = MOVE_NONE;
			}

			for (j = 0; j < gBattlersCount; ++j)
			{
				if (SPLIT(move) != SPLIT_STATUS)
				{
					u8 moveResult;

					if (j == gActiveBattler || j == PARTNER(gActiveBattler))
					{
						tempMoveStruct->moveResults[GetBattlerPosition(j)][i] = 0;
						continue;
					}

					moveResult = VisualTypeCalc(move, gActiveBattler, j);

					if (!(moveResult & MOVE_RESULT_NO_EFFECT)
					&& (CheckTableForMoveEffect(move, gMoveEffectsThatIgnoreWeaknessResistance) || gBattleMoves[move].effect == EFFECT_0HKO))
						moveResult = 0; //These moves can have no effect, but are neither super nor not very effective
					tempMoveStruct->moveResults[GetBattlerPosition(j)][i] = moveResult;
				}
				else
					tempMoveStruct->moveResults[GetBattlerPosition(j)][i] = 0;
			}
		}
		else //Single Battle or single target
		{
			tempMoveStruct->movePowers[i] = CalcVisualBasePower(gActiveBattler, foe, move, FALSE);
			tempMoveStruct->moveAcc[i] = VisualAccuracyCalc(move, gActiveBattler, foe);

			if (tempMoveStruct->possibleMaxMoves[i] != MOVE_NONE)
			{
				gNewBS->ai.zMoveHelper = originalMove;
				tempMoveStruct->maxMovePowers[i] = CalcVisualBasePower(gActiveBattler, foe, tempMoveStruct->possibleMaxMoves[i], FALSE);
				gNewBS->ai.zMoveHelper = MOVE_NONE;
			}

			if (SPLIT(move) != SPLIT_STATUS)
			{
				u8 moveResult = VisualTypeCalc(move, gActiveBattler, foe);

				if (!(moveResult & MOVE_RESULT_NO_EFFECT)
				&& (CheckTableForMoveEffect(move, gMoveEffectsThatIgnoreWeaknessResistance) || gBattleMoves[move].effect == EFFECT_0HKO))
					moveResult = 0; //These moves can have no effect, but are neither super nor not very effective

				tempMoveStruct->moveResults[GetBattlerPosition(foe)][i] = moveResult;
			}
			else
				tempMoveStruct->moveResults[GetBattlerPosition(foe)][i] = 0;
		}
	}

	tempMoveStruct->megaDone = gNewBS->megaData.done[gActiveBattler];
	tempMoveStruct->ultraDone = gNewBS->ultraData.done[gActiveBattler];
	if (!IS_TRANSFORMED(gActiveBattler) && !tempMoveStruct->dynamaxed)
	{
		if (!gNewBS->megaData.done[gActiveBattler])
		{
			evolutions = CanMegaEvolve(gActiveBattler, FALSE);
			if (evolutions != NULL)
			{
				if (!BankMegaEvolved(gActiveBattler, FALSE)
				&& MegaEvolutionEnabled(gActiveBattler)
				&& !BATTLER_SEMI_INVULNERABLE(gActiveBattler) //No Mega Evolving while not on screen
				&& !DoesZMoveUsageStopMegaEvolution(gActiveBattler) //No Mega Evolving if you've used a Z-Move (*cough* *cough* Rayquaza)
				&& !DoesDynamaxUsageStopMegaEvolution(gActiveBattler))
				{
					tempMoveStruct->canMegaEvolve = TRUE;
					tempMoveStruct->megaVariance = evolutions->unknown;
				}
			}
		}

		if (!gNewBS->ultraData.done[gActiveBattler])
		{
			evolutions = CanMegaEvolve(gActiveBattler, TRUE); //Check Ultra Burst
			if (evolutions != NULL)
			{
				if (!BankMegaEvolved(gActiveBattler, TRUE) //Check Ultra Burst
				&& !BATTLER_SEMI_INVULNERABLE(gActiveBattler) //No Ultra Bursting while not on screen
				&& !DoesDynamaxUsageStopMegaEvolution(gActiveBattler))
				{
					tempMoveStruct->canMegaEvolve = TRUE;
					tempMoveStruct->megaVariance = evolutions->unknown;
				}
			}
		}
	}

	tempMoveStruct->zMoveUsed = gNewBS->zMoveData.used[gActiveBattler];
	tempMoveStruct->zPartyIndex = gNewBS->zMoveData.partyIndex[SIDE(gActiveBattler)];
	if (!gNewBS->zMoveData.used[gActiveBattler] && !IsMegaZMoveBannedBattle() && !IsMega(gActiveBattler) && !IsBluePrimal(gActiveBattler) && !IsRedPrimal(gActiveBattler))
	{
		u8 limitations = CheckMoveLimitations(gActiveBattler, 0, MOVE_LIMITATION_PP);

		for (i = 0; i < MAX_MON_MOVES; ++i)
		{
			if (!(limitations & gBitTable[i])) //Don't display a Z-Move if the base move has no PP
				tempMoveStruct->possibleZMoves[i] = CanUseZMove(gActiveBattler, i, 0);
		}
	}

	gBattleBuffersTransferData[0] = CONTROLLER_CHOOSEMOVE;
	gBattleBuffersTransferData[1] = isDoubleBattle;
	gBattleBuffersTransferData[2] = NoPpNumber;
	gBattleBuffersTransferData[3] = 0;
	for (i = 0; i < sizeof(*tempMoveStruct); ++i)
		gBattleBuffersTransferData[4 + i] = ((u8*)(tempMoveStruct))[i];
	PrepareBufferDataTransfer(bufferId, gBattleBuffersTransferData, sizeof(*tempMoveStruct) + 4);

	Free(tempMoveStruct);
}

void EmitMoveChosen(u8 bufferId, u8 chosenMoveIndex, u8 target, u8 megaState, u8 ultraState, u8 zMoveState, u8 dynamaxState)
{
	gBattleBuffersTransferData[0] = CONTROLLER_TWORETURNVALUES;
	gBattleBuffersTransferData[1] = ACTION_RUN_BATTLESCRIPT;
	gBattleBuffersTransferData[2] = chosenMoveIndex;
	gBattleBuffersTransferData[3] = target;
	gBattleBuffersTransferData[4] = megaState;
	gBattleBuffersTransferData[5] = ultraState;
	gBattleBuffersTransferData[6] = zMoveState;
	gBattleBuffersTransferData[7] = dynamaxState;
	PrepareBufferDataTransfer(bufferId, gBattleBuffersTransferData, 8);
}

const u8 sText_StabMoveInterfaceType[] = {0xFC, 0x05, 0x05, 0xFC, 0x04, 0x09, 0x0E, 0x08, 0xFF};

static void MoveNameToDisplayedStringBattle(u8 moveIndex)
{
	struct ChooseMoveStruct *moveInfo = (struct ChooseMoveStruct*)(&gBattleBufferA[gActiveBattler][4]);

	if (moveInfo->dynamaxed && moveInfo->moves[moveIndex] != MOVE_NONE)
	{
		if (IsGMaxMove(moveInfo->possibleMaxMoves[moveIndex]))
			gDisplayedStringBattle[0] = PC_G; //Short for G-Max
		else
			gDisplayedStringBattle[0] = PC_M; //Short for Max

		gDisplayedStringBattle[1] = TXT_MINUS;
		StringCopy(gDisplayedStringBattle + 2, gMoveNames[moveInfo->possibleMaxMoves[moveIndex]]);
	}
	else
		StringCopy(gDisplayedStringBattle, gMoveNames[moveInfo->moves[moveIndex]]);
}

static void MoveSelectionDisplayMoveNames(void)
{
    s32 i;
    struct ChooseMoveStruct *moveInfo = (struct ChooseMoveStruct*)(&gBattleBufferA[gActiveBattler][4]);
    gNumberOfMovesToChoose = 0;

    for (i = 0; i < MAX_MON_MOVES; i++)
    {
        MoveSelectionDestroyCursorAt(i);
		MoveNameToDisplayedStringBattle(i);
        BattlePutTextOnWindow(gDisplayedStringBattle, i + 3);
        if (moveInfo->moves[i] != MOVE_NONE)
            gNumberOfMovesToChoose++;
    }
}

#define SUPER_EFFECTIVE_COLOURS 0
#define NOT_VERY_EFFECTIVE_COLOURS 4
#define NO_EFFECT_COLOURS 8
#define REGULAR_COLOURS 12
static void MoveSelectionDisplayMoveType(void)
{
	u8 *txtPtr, moveType;
	const u8* formatting;
	struct ChooseMoveStruct *moveInfo = (struct ChooseMoveStruct*)(&gBattleBufferA[gActiveBattler][4]);

	#ifdef DISPLAY_REAL_MOVE_TYPE_ON_MENU
		moveType = moveInfo->moveTypes[gMoveSelectionCursor[gActiveBattler]];
	#else
		if (!moveInfo->dynamaxed)
			moveType = gBattleMoves[moveInfo->moves[gMoveSelectionCursor[gActiveBattler]]].type;
		else
			moveType = gBattleMoves[moveInfo->possibleMaxMoves[gMoveSelectionCursor[gActiveBattler]]].type;
	#endif

	//Update Palette Fading for Effectiveness
	#ifdef DISPLAY_EFFECTIVENESS_ON_MENU
		if (!(gBattleTypeFlags & BATTLE_TYPE_LINK) && !IS_GHOST_BATTLE) //Don't use this feature in link battles or battles against Ghosts
		{
			u8 stab = 0;
			const u16* palPtr = gUserInterfaceGfx_TypeHighlightingPal;
			if (SPLIT(moveInfo->moves[gMoveSelectionCursor[gActiveBattler]]) != SPLIT_STATUS
			&&	(moveType == moveInfo->monType1
			  || moveType == moveInfo->monType2
			  || moveType == moveInfo->monType3))
			{
				stab = 2;
			}

			u8 moveResult = 0;
			if (IS_SINGLE_BATTLE)
				moveResult = moveInfo->moveResults[GetBattlerPosition(FOE(gActiveBattler))][gMoveSelectionCursor[gActiveBattler]];
			else if (gBattlerControllerFuncs[gActiveBattler] == HandleInputChooseTarget)
				moveResult = moveInfo->moveResults[GetBattlerPosition(gMultiUsePlayerCursor)][gMoveSelectionCursor[gActiveBattler]];
			else if (CountAliveMonsInBattle(BATTLE_ALIVE_DEF_SIDE, gActiveBattler, FOE(gActiveBattler)) <= 1) //Only 1 enemy left
			{
				u8 bankDef = FOE(gActiveBattler);
				if (!BATTLER_ALIVE(bankDef))
					bankDef = PARTNER(bankDef);

				moveResult = moveInfo->moveResults[GetBattlerPosition(bankDef)][gMoveSelectionCursor[gActiveBattler]];
			}

			if (moveResult & MOVE_RESULT_SUPER_EFFECTIVE)
			{
				gPlttBufferUnfaded[88] = palPtr[SUPER_EFFECTIVE_COLOURS + stab + 0];
				gPlttBufferUnfaded[89] = palPtr[SUPER_EFFECTIVE_COLOURS + stab + 1];
				formatting = sText_StabMoveInterfaceType;
			}
			else if (moveResult & MOVE_RESULT_NOT_VERY_EFFECTIVE)
			{
				gPlttBufferUnfaded[88] = palPtr[NOT_VERY_EFFECTIVE_COLOURS + stab + 0];
				gPlttBufferUnfaded[89] = palPtr[NOT_VERY_EFFECTIVE_COLOURS + stab + 1];
				formatting = sText_StabMoveInterfaceType;
			}
			else if (moveResult & MOVE_RESULT_NO_EFFECT)
			{
				gPlttBufferUnfaded[88] = palPtr[NO_EFFECT_COLOURS + 0]; //No STAB for moves with no effect
				gPlttBufferUnfaded[89] = palPtr[NO_EFFECT_COLOURS + 1];
				formatting = sText_StabMoveInterfaceType;
			}
			else //Nothing special about move
			{
				gPlttBufferUnfaded[88] = palPtr[REGULAR_COLOURS + stab + 0];
				gPlttBufferUnfaded[89] = palPtr[REGULAR_COLOURS + stab + 1];
				formatting = sText_StabMoveInterfaceType;
			}
		}
		else
	#endif
			formatting = gText_MoveInterfaceType;

	CpuCopy16(&gPlttBufferUnfaded[88], &gPlttBufferFaded[88], sizeof(u16));
	CpuCopy16(&gPlttBufferUnfaded[89], &gPlttBufferFaded[89], sizeof(u16));

	txtPtr = StringCopy(gDisplayedStringBattle, gText_TypeWord);
	txtPtr[0] = EXT_CTRL_CODE_BEGIN;
	txtPtr++;
	txtPtr[0] = 6;
	txtPtr++;
	txtPtr[0] = 1;
	txtPtr++;
	txtPtr = StringCopy(txtPtr, formatting);

	StringCopy(txtPtr, gTypeNames[moveType]);
	BattlePutTextOnWindow(gDisplayedStringBattle, 8);
}

static bool8 MoveSelectionDisplayZMove(void)
{
	struct ChooseMoveStruct *moveInfo = (struct ChooseMoveStruct*)(&gBattleBufferA[gActiveBattler][4]);
	u16 zmove = moveInfo->possibleZMoves[gMoveSelectionCursor[gActiveBattler]];
	u8 i;

	if (gNewBS->zMoveData.viewing)
	{
		ReloadMoveNamesIfNecessary();
		PlaySE(3); //Turn Off
		return TRUE;
	}

	if (moveInfo->dynamaxed)
		return FALSE;

	if (zmove != MOVE_NONE)
	{
		for (i = 0; i < MAX_MON_MOVES; ++i)
		{
			MoveSelectionDestroyCursorAt(i);
			StringCopy(gDisplayedStringBattle, StringNull);
			BattlePutTextOnWindow(gDisplayedStringBattle, i + 3);
		}

		if (zmove == 0xFFFF)
		{
			u8 zEffect = gBattleMoves[moveInfo->moves[gMoveSelectionCursor[gActiveBattler]]].z_move_effect;

			if (zEffect == Z_EFFECT_CURSE)
			{
				if (moveInfo->monType1 == TYPE_GHOST || moveInfo->monType2 == TYPE_GHOST || moveInfo->monType3 == TYPE_GHOST)
					zEffect = Z_EFFECT_RECOVER_HP;
				else
					zEffect = Z_EFFECT_ATK_UP_1;
			}

			gDisplayedStringBattle[0] = EOS;
			switch (zEffect) {
				case Z_EFFECT_RESET_STATS:
					StringCopy(gDisplayedStringBattle, gText_ResetStats);
					break;

				case Z_EFFECT_ALL_STATS_UP_1:
					StringCopy(gDisplayedStringBattle, gText_StatsPlus);
					break;

				case Z_EFFECT_BOOST_CRITS:
					StringCopy(gDisplayedStringBattle, gText_CritHitsPlus);
					break;

				case Z_EFFECT_FOLLOW_ME:
					StringCopy(gDisplayedStringBattle, gText_FollowMe);
					break;

				case Z_EFFECT_RECOVER_HP:
					StringCopy(gDisplayedStringBattle, gText_RecoverHP);
					break;

				case Z_EFFECT_RESTORE_REPLACEMENT_HP:
					StringCopy(gDisplayedStringBattle, gText_HealAllyHP);
					break;

				case Z_EFFECT_ATK_UP_1:
				case Z_EFFECT_DEF_UP_1:
				case Z_EFFECT_SPD_UP_1:
				case Z_EFFECT_SPATK_UP_1:
				case Z_EFFECT_SPDEF_UP_1:
				case Z_EFFECT_ACC_UP_1:
				case Z_EFFECT_EVSN_UP_1:
					gDisplayedStringBattle[0] = TXT_PLUS;
					gDisplayedStringBattle[1] = 0;
					gDisplayedStringBattle[2] = EOS;
					PREPARE_STAT_BUFFER(gBattleTextBuff1, zEffect - Z_EFFECT_ATK_UP_1 + 1);
					ExpandBattleTextBuffPlaceholders(gBattleTextBuff1, gDisplayedStringBattle + 2);
					break;

				case Z_EFFECT_ATK_UP_2:
				case Z_EFFECT_DEF_UP_2:
				case Z_EFFECT_SPD_UP_2:
				case Z_EFFECT_SPATK_UP_2:
				case Z_EFFECT_SPDEF_UP_2:
				case Z_EFFECT_ACC_UP_2:
				case Z_EFFECT_EVSN_UP_2:
					gDisplayedStringBattle[0] = TXT_PLUS;
					gDisplayedStringBattle[1] = TXT_PLUS;
					gDisplayedStringBattle[2] = 0;
					gDisplayedStringBattle[3] = EOS;
					PREPARE_STAT_BUFFER(gBattleTextBuff1, zEffect - Z_EFFECT_ATK_UP_2 + 1);
					ExpandBattleTextBuffPlaceholders(gBattleTextBuff1, gDisplayedStringBattle + 3);
					break;

				case Z_EFFECT_ATK_UP_3:
				case Z_EFFECT_DEF_UP_3:
				case Z_EFFECT_SPD_UP_3:
				case Z_EFFECT_SPATK_UP_3:
				case Z_EFFECT_SPDEF_UP_3:
				case Z_EFFECT_ACC_UP_3:
				case Z_EFFECT_EVSN_UP_3:
					gDisplayedStringBattle[0] = TXT_PLUS;
					gDisplayedStringBattle[1] = TXT_PLUS;
					gDisplayedStringBattle[2] = TXT_PLUS;
					gDisplayedStringBattle[3] = 0;
					gDisplayedStringBattle[4] = EOS;
					PREPARE_STAT_BUFFER(gBattleTextBuff1, zEffect - Z_EFFECT_ATK_UP_3 + 1);
					ExpandBattleTextBuffPlaceholders(gBattleTextBuff1, gDisplayedStringBattle + 4);
					break;
			}
			BattlePutTextOnWindow(gDisplayedStringBattle, 3 + 2); //Slot of Move 3

			gDisplayedStringBattle[0] = 0xD4;
			gDisplayedStringBattle[1] = 0xAE; //Z-
			StringCopy(gDisplayedStringBattle + 2, gMoveNames[moveInfo->moves[gMoveSelectionCursor[gActiveBattler]]]);
		}
		else
		{
			ZMoveSelectionDisplayPower();
			StringCopy(gDisplayedStringBattle, GetZMoveName(zmove));
		}
		BattlePutTextOnWindow(gDisplayedStringBattle, 3);

		ZMoveSelectionDisplayPpNumber();
		MoveSelectionCreateCursorAt(0, 0);
		gNewBS->zMoveData.viewing = TRUE;
		PlaySE(2); //Turn On
		return TRUE;
	}

	return FALSE;
}

static bool8 MoveSelectionDisplayMaxMove(void)
{
	struct ChooseMoveStruct *moveInfo = (struct ChooseMoveStruct*)(&gBattleBufferA[gActiveBattler][4]);
	u16 maxMove = moveInfo->possibleMaxMoves[gMoveSelectionCursor[gActiveBattler]];
	u8 i;

	if (gNewBS->dynamaxData.viewing)
	{
		ReloadMoveNamesIfNecessary();

		if (!moveInfo->dynamaxed) //Don't play sound if already Dynamaxed
			PlaySE(3); //Turn Off
		return TRUE;
	}

	if (maxMove != MOVE_NONE)
	{
		for (i = 0; i < MAX_MON_MOVES; ++i)
		{
			MoveSelectionDestroyCursorAt(i);
			StringCopy(gDisplayedStringBattle, StringNull);
			BattlePutTextOnWindow(gDisplayedStringBattle, i + 3);
		}

		u8 maxEffect = gBattleMoves[maxMove].z_move_effect;

		gDisplayedStringBattle[0] = EOS;
		switch (maxEffect) {
			case MAX_EFFECT_NONE:
				StringCopy(gDisplayedStringBattle, gText_MaxMoveProtect);
				break;

			case MAX_EFFECT_RAISE_TEAM_ATTACK:
			case MAX_EFFECT_RAISE_TEAM_DEFENSE:
			case MAX_EFFECT_RAISE_TEAM_SPEED:
			case MAX_EFFECT_RAISE_TEAM_SP_ATK:
			case MAX_EFFECT_RAISE_TEAM_SP_DEF:
				StringCopy(gDisplayedStringBattle, gText_MaxMoveRaiseTeamStat);
				PREPARE_STAT_BUFFER(gBattleTextBuff1, maxEffect - MAX_EFFECT_RAISE_TEAM_ATTACK + 1);
				ExpandBattleTextBuffPlaceholders(gBattleTextBuff1, gDisplayedStringBattle + 7);
				break;

			case MAX_EFFECT_LOWER_ATTACK:
			case MAX_EFFECT_LOWER_DEFENSE:
			case MAX_EFFECT_LOWER_SPEED:
			case MAX_EFFECT_LOWER_SP_ATK:
			case MAX_EFFECT_LOWER_SP_DEF:
				gDisplayedStringBattle[0] = TXT_MINUS;
				gDisplayedStringBattle[1] = 0;
				gDisplayedStringBattle[2] = EOS;
				PREPARE_STAT_BUFFER(gBattleTextBuff1, maxEffect - MAX_EFFECT_LOWER_ATTACK + 1);
				ExpandBattleTextBuffPlaceholders(gBattleTextBuff1, gDisplayedStringBattle + 2);
				StringAppend(gDisplayedStringBattle, gText_MaxMoveFoes);
				break;

			case MAX_EFFECT_SUN:
				StringCopy(gDisplayedStringBattle, gText_MaxMoveSet);
				StringAppend(gDisplayedStringBattle, gText_MaxMoveSunny);
				StringAppend(gDisplayedStringBattle, gText_MaxMoveWeather);
				break;
			case MAX_EFFECT_RAIN:
				StringCopy(gDisplayedStringBattle, gText_MaxMoveSet);
				StringAppend(gDisplayedStringBattle, gText_MaxMoveRainy);
				StringAppend(gDisplayedStringBattle, gText_MaxMoveWeather);
				break;
			case MAX_EFFECT_SANDSTORM:
				StringCopy(gDisplayedStringBattle, gText_MaxMoveSet);
				StringAppend(gDisplayedStringBattle, gText_MaxMoveSandstorm);
				StringAppend(gDisplayedStringBattle, gText_MaxMoveWeather);
				break;
			case MAX_EFFECT_HAIL:
				StringCopy(gDisplayedStringBattle, gText_MaxMoveSet);
				StringAppend(gDisplayedStringBattle, gText_MaxMoveHail);
				StringAppend(gDisplayedStringBattle, gText_MaxMoveWeather);
				break;

			case MAX_EFFECT_ELECTRIC_TERRAIN:
				StringCopy(gDisplayedStringBattle, gText_MaxMoveSet);
				StringAppend(gDisplayedStringBattle, gText_MaxMoveElectric);
				StringAppend(gDisplayedStringBattle, gText_MaxMoveTerrain);
				break;
			case MAX_EFFECT_GRASSY_TERRAIN:
				StringCopy(gDisplayedStringBattle, gText_MaxMoveSet);
				StringAppend(gDisplayedStringBattle, gText_MaxMoveGrassy);
				StringAppend(gDisplayedStringBattle, gText_MaxMoveTerrain);
				break;
			case MAX_EFFECT_MISTY_TERRAIN:
				StringCopy(gDisplayedStringBattle, gText_MaxMoveSet);
				StringAppend(gDisplayedStringBattle, gText_MaxMoveMisty);
				StringAppend(gDisplayedStringBattle, gText_MaxMoveTerrain);
				break;
			case MAX_EFFECT_PSYCHIC_TERRAIN:
				StringCopy(gDisplayedStringBattle, gText_MaxMoveSet);
				StringAppend(gDisplayedStringBattle, gText_MaxMovePsychic);
				StringAppend(gDisplayedStringBattle, gText_MaxMoveTerrain);
				break;
			case MAX_EFFECT_VINE_LASH:
			case MAX_EFFECT_WILDFIRE:
			case MAX_EFFECT_CANNONADE:
			case MAX_EFFECT_VOLCAITH_FOES:
				StringCopy(gDisplayedStringBattle, gText_MaxMoveLastingDamage);
				break;
			case MAX_EFFECT_EFFECT_SPORE_FOES:
				StringCopy(gDisplayedStringBattle, gText_MaxMoveEffectSpore);
				break;
			case MAX_EFFECT_PARALYZE_FOES:
				StringCopy(gDisplayedStringBattle, gText_MaxMoveParalyzeFoes);
				break;
			case MAX_EFFECT_CONFUSE_FOES_PAY_DAY:
				StringCopy(gDisplayedStringBattle, gText_MaxMoveConfusePayDay);
				break;
			case MAX_EFFECT_CRIT_PLUS:
				StringCopy(gDisplayedStringBattle, gText_MaxMoveCritPlus);
				break;
			case MAX_EFFECT_MEAN_LOOK:
				StringCopy(gDisplayedStringBattle, gText_MaxMoveMeanLook);
				break;
			case MAX_EFFECT_AURORA_VEIL:
				StringCopy(gDisplayedStringBattle, gText_MaxMoveAuroraVeil);
				break;
			case MAX_EFFECT_INFATUATE_FOES:
				StringCopy(gDisplayedStringBattle, gText_MaxMoveInfatuate);
				break;
			case MAX_EFFECT_RECYCLE_BERRIES:
				StringCopy(gDisplayedStringBattle, gText_MaxMoveRecycleBerries);
				break;
			case MAX_EFFECT_POISON_FOES:
				StringCopy(gDisplayedStringBattle, gText_MaxMovePoisonFoes);
				break;
			case MAX_EFFECT_STEALTH_ROCK:
				StringCopy(gDisplayedStringBattle, gText_MaxMoveStealthRock);
				break;
			case MAX_EFFECT_DEFOG:
				StringCopy(gDisplayedStringBattle, gText_MaxMoveRemoveScreens);
				break;
			case MAX_EFFECT_POISON_PARALYZE_FOES:
				StringCopy(gDisplayedStringBattle, gText_MaxMovePoisonParalyzeFoes);
				break;
			case MAX_EFFECT_HEAL_TEAM:
				StringCopy(gDisplayedStringBattle, gText_MaxMoveHealTeam);
				break;
			case MAX_EFFECT_SPITE:
				StringCopy(gDisplayedStringBattle, gText_MaxMoveSpite);
				break;
			case MAX_EFFECT_GRAVITY:
				StringCopy(gDisplayedStringBattle, gText_MaxMoveGravity);
				break;
			case MAX_EFFECT_FIRE_SPIN_FOES:
			case MAX_EFFECT_SANDBLAST_FOES:
				StringCopy(gDisplayedStringBattle, gText_MaxMoveTrapDamage);
				break;
			case MAX_EFFECT_YAWN_FOE:
				StringCopy(gDisplayedStringBattle, gText_MaxMoveYawn);
				break;
			case MAX_EFFECT_LOWER_EVASIVENESS_FOES:
				StringCopy(gDisplayedStringBattle, gText_MaxMoveLowerEvasiveness);
				break;
			case MAX_EFFECT_AROMATHERAPY:
				StringCopy(gDisplayedStringBattle, gText_MaxMoveAromatherapy);
				break;
			case MAX_EFFECT_CONFUSE_FOES:
				StringCopy(gDisplayedStringBattle, gText_MaxMoveConfuseFoes);
				break;
			case MAX_EFFECT_STEELSURGE:
				StringCopy(gDisplayedStringBattle, gText_MaxMoveSteelsurge);
				break;
			case MAX_EFFECT_TORMENT_FOES:
				StringCopy(gDisplayedStringBattle, gText_MaxMoveTormentFoes);
				break;
			case MAX_EFFECT_LOWER_SPEED_2_FOES:
				StringCopy(gDisplayedStringBattle, gText_MaxMoveLowerSpeed2);
				break;
		}

		BattlePutTextOnWindow(gDisplayedStringBattle, 3 + 2); //Slot of Move 3

		MaxMoveSelectionDisplayPower();

		if (IsGMaxMove(maxMove))
		{
			gDisplayedStringBattle[0] = PC_G;
			gDisplayedStringBattle[1] = TXT_MINUS;
			gDisplayedStringBattle[2] = PC_M;
			gDisplayedStringBattle[3] = PC_a;
			gDisplayedStringBattle[4] = PC_x;
			gDisplayedStringBattle[5] = PC_SPACE;
			StringCopy(gDisplayedStringBattle + 6, gMoveNames[maxMove]);
		}
		else
		{
			gDisplayedStringBattle[0] = PC_M;
			gDisplayedStringBattle[1] = PC_a;
			gDisplayedStringBattle[2] = PC_x;
			gDisplayedStringBattle[3] = PC_SPACE;
			StringCopy(gDisplayedStringBattle + 4, gMoveNames[maxMove]);
		}

		BattlePutTextOnWindow(gDisplayedStringBattle, 3);
		MoveSelectionCreateCursorAt(0, 0);
		gNewBS->dynamaxData.viewing = TRUE;

		if (!moveInfo->dynamaxed) //Don't play sound if already Dynamaxed
			PlaySE(2); //Turn On
		return TRUE;
	}

	return FALSE;
}

static void ZMoveSelectionDisplayPpNumber(void)
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

static void ZMoveSelectionDisplayPower(void)
{
	u8 *txtPtr;
	struct ChooseMoveStruct *moveInfo = (struct ChooseMoveStruct*)(&gBattleBufferA[gActiveBattler][4]);
	u16 move = moveInfo->moves[gMoveSelectionCursor[gActiveBattler]];
	u16 power = gBattleMoves[move].z_move_power;

	u16 zmove = moveInfo->possibleZMoves[gMoveSelectionCursor[gActiveBattler]];

	if (zmove >= MOVE_CATASTROPIKA)
		power = gBattleMoves[zmove].power;

	if (SPLIT(move) != SPLIT_STATUS)
	{
		txtPtr = StringCopy(gDisplayedStringBattle, gText_Power);
		ConvertIntToDecimalStringN(txtPtr, power, STR_CONV_MODE_LEFT_ALIGN, 3);
		BattlePutTextOnWindow(gDisplayedStringBattle, 3 + 2); //Slot of Move 2
	/*
		txtPtr = StringCopy(gDisplayedStringBattle, gText_Power);
		txtPtr[0] = EXT_CTRL_CODE_BEGIN;
		txtPtr++;
		txtPtr[0] = 6;
		txtPtr++;
		txtPtr[0] = 1;
		txtPtr++;
		ConvertIntToDecimalStringN(txtPtr, power, STR_CONV_MODE_LEFT_ALIGN, 3);
		BattlePutTextOnWindow(gDisplayedStringBattle, 8); //Where Move Type goes
	*/
	}
}

static void MaxMoveSelectionDisplayPower(void)
{
	#ifdef DYNAMAX_FEATURE
	u8 *txtPtr;
	struct ChooseMoveStruct *moveInfo = (struct ChooseMoveStruct*)(&gBattleBufferA[gActiveBattler][4]);
	u16 move = moveInfo->moves[gMoveSelectionCursor[gActiveBattler]];

	#ifdef DISPLAY_REAL_POWER_ON_MENU
		u16 power = moveInfo->maxMovePowers[gMoveSelectionCursor[gActiveBattler]];
	#else
		u16 power = gDynamaxMovePowers[move];
	#endif

	if (SPLIT(move) != SPLIT_STATUS)
	{
		txtPtr = StringCopy(gDisplayedStringBattle, gText_Power);
		txtPtr[0] = EXT_CTRL_CODE_BEGIN;
		txtPtr++;
		txtPtr[0] = 6;
		txtPtr++;
		txtPtr[0] = 1;
		txtPtr++;
		ConvertIntToDecimalStringN(txtPtr, power, STR_CONV_MODE_LEFT_ALIGN, 3);
		BattlePutTextOnWindow(gDisplayedStringBattle, 8); //Where Move Type goes
	}
	#endif
}

static void MoveSelectionDisplayDetails(void)
{
	u8 *txtPtr;
	struct ChooseMoveStruct *moveInfo = (struct ChooseMoveStruct*)(&gBattleBufferA[gActiveBattler][4]);

	if (gNewBS->zMoveData.viewingDetails)
	{
		ReloadMoveNamesIfNecessary();
		return;
	}

	for (int i = 0; i < MAX_MON_MOVES; ++i)
	{
		MoveSelectionDestroyCursorAt(i);
		StringCopy(gDisplayedStringBattle, StringNull);
		BattlePutTextOnWindow(gDisplayedStringBattle, i + 3);
	}

//Diaplay Move Name
	MoveNameToDisplayedStringBattle(gMoveSelectionCursor[gActiveBattler]);
	BattlePutTextOnWindow(gDisplayedStringBattle, 3);

//Display Move Accuracy
	txtPtr = StringCopy(gDisplayedStringBattle, gText_Acc);
	#ifdef DISPLAY_REAL_ACCURACY_ON_MENU
		if (moveInfo->dynamaxed || moveInfo->moveAcc[gMoveSelectionCursor[gActiveBattler]] == 0xFFFF)
			StringCopy(txtPtr, gText_NoMiss);
		else
			ConvertIntToDecimalStringN(txtPtr, moveInfo->moveAcc[gMoveSelectionCursor[gActiveBattler]], STR_CONV_MODE_LEFT_ALIGN, 3);
	#else
		if (moveInfo->dynamaxed || gBattleMoves[moveInfo->moves[gMoveSelectionCursor[gActiveBattler]]].accuracy == 0)
			StringCopy(txtPtr, gText_NoMiss);
		else
			ConvertIntToDecimalStringN(txtPtr, gBattleMoves[moveInfo->moves[gMoveSelectionCursor[gActiveBattler]]].accuracy, STR_CONV_MODE_LEFT_ALIGN, 3);
	#endif
	BattlePutTextOnWindow(gDisplayedStringBattle, 3 + 1); //Slot of Move 2

//Display Move Power
	if (!moveInfo->dynamaxed && gBattleMoves[moveInfo->moves[gMoveSelectionCursor[gActiveBattler]]].effect == EFFECT_0HKO)
	{
		StringCopy(gDisplayedStringBattle, gText_OHKO);
	}
	else
	{
		u16 power;
		txtPtr = StringCopy(gDisplayedStringBattle, gText_Power);
		#ifdef DISPLAY_REAL_POWER_ON_MENU
			if (moveInfo->dynamaxed)
				power = moveInfo->maxMovePowers[gMoveSelectionCursor[gActiveBattler]];
			else
				power = moveInfo->movePowers[gMoveSelectionCursor[gActiveBattler]];
		#else
			if (moveInfo->dynamaxed)
				power = gDynamaxMovePowers[moveInfo->moves[gMoveSelectionCursor[gActiveBattler]]];
			else
				power = gBattleMoves[moveInfo->moves[gMoveSelectionCursor[gActiveBattler]]].power;
		#endif
		ConvertIntToDecimalStringN(txtPtr, power, STR_CONV_MODE_LEFT_ALIGN, 3);
	}
	BattlePutTextOnWindow(gDisplayedStringBattle, 3 + 2); //Slot of Move 3

//Display Move Split
	switch(SPLIT(moveInfo->moves[gMoveSelectionCursor[gActiveBattler]])) {
		case SPLIT_SPECIAL:
			StringCopy(gDisplayedStringBattle, gText_Special);
			break;
		case SPLIT_STATUS:
			StringCopy(gDisplayedStringBattle, gText_Status);
			break;
		default:
			StringCopy(gDisplayedStringBattle, gText_Physical);
	}
	BattlePutTextOnWindow(gDisplayedStringBattle, 3 + 3); //Slot of Move 4

	MoveSelectionCreateCursorAt(0, 0);
	gNewBS->zMoveData.viewingDetails = TRUE;
}

static void ReloadMoveNamesIfNecessary(void)
{
	if (gNewBS->zMoveData.viewing || gNewBS->dynamaxData.viewing)
	{
		gNewBS->zMoveData.viewing = FALSE;
		gNewBS->dynamaxData.viewing = FALSE;
		MoveSelectionDestroyCursorAt(0);
		MoveSelectionDisplayMoveNames();
		MoveSelectionCreateCursorAt(gMoveSelectionCursor[gActiveBattler], 0);
		MoveSelectionDisplayPpNumber();
		MoveSelectionDisplayMoveType();
	}
	else if (gNewBS->zMoveData.viewingDetails)
	{
		gNewBS->zMoveData.viewingDetails = FALSE;
		MoveSelectionDestroyCursorAt(0);
		MoveSelectionDisplayMoveNames();
		MoveSelectionCreateCursorAt(gMoveSelectionCursor[gActiveBattler], 0);
		MoveSelectionDisplayPpNumber();
		MoveSelectionDisplayMoveType();
	}
}

static void CloseZMoveDetails(void)
{
	if (gNewBS->zMoveData.viewing)
	{
		gNewBS->zMoveData.toBeUsed[gActiveBattler] = TRUE;
		gNewBS->zMoveData.viewing = FALSE;
	}

	gNewBS->zMoveData.viewingDetails = FALSE;
}

static void CloseMaxMoveDetails(void)
{
	struct ChooseMoveStruct *moveInfo = (struct ChooseMoveStruct*)(&gBattleBufferA[gActiveBattler][4]);

	if (gNewBS->dynamaxData.viewing)
	{
		if (!moveInfo->dynamaxed)
			gNewBS->dynamaxData.toBeUsed[gActiveBattler] = TRUE; //Only set if not already dynamaxed

		gNewBS->dynamaxData.viewing = FALSE;
	}
}

void HandleMoveSwitchingUpdate(void)
{
	u8 perMovePPBonuses[4];
	struct ChooseMoveStruct moveStruct;
	u8 totalPPBonuses;
	struct ChooseMoveStruct *moveInfo = (struct ChooseMoveStruct*)(&gBattleBufferA[gActiveBattler][4]);
	int i, j;

	//Swap Moves
	i = moveInfo->moves[gMoveSelectionCursor[gActiveBattler]];
	moveInfo->moves[gMoveSelectionCursor[gActiveBattler]] = moveInfo->moves[gMultiUsePlayerCursor];
	moveInfo->moves[gMultiUsePlayerCursor] = i;

	//Swap PP
	i = moveInfo->currentPp[gMoveSelectionCursor[gActiveBattler]];
	moveInfo->currentPp[gMoveSelectionCursor[gActiveBattler]] = moveInfo->currentPp[gMultiUsePlayerCursor];
	moveInfo->currentPp[gMultiUsePlayerCursor] = i;

	i = moveInfo->maxPp[gMoveSelectionCursor[gActiveBattler]];
	moveInfo->maxPp[gMoveSelectionCursor[gActiveBattler]] = moveInfo->maxPp[gMultiUsePlayerCursor];
	moveInfo->maxPp[gMultiUsePlayerCursor] = i;

	//Swap Move Types Details
	i = moveInfo->moveTypes[gMoveSelectionCursor[gActiveBattler]];
	moveInfo->moveTypes[gMoveSelectionCursor[gActiveBattler]] = moveInfo->moveTypes[gMultiUsePlayerCursor];
	moveInfo->moveTypes[gMultiUsePlayerCursor] = i;

	//Swap Move Powers Details
	i = moveInfo->movePowers[gMoveSelectionCursor[gActiveBattler]];
	moveInfo->movePowers[gMoveSelectionCursor[gActiveBattler]] = moveInfo->movePowers[gMultiUsePlayerCursor];
	moveInfo->movePowers[gMultiUsePlayerCursor] = i;

	i = moveInfo->maxMovePowers[gMoveSelectionCursor[gActiveBattler]];
	moveInfo->maxMovePowers[gMoveSelectionCursor[gActiveBattler]] = moveInfo->maxMovePowers[gMultiUsePlayerCursor];
	moveInfo->maxMovePowers[gMultiUsePlayerCursor] = i;

	//Swap Move Accuracies Details
	i = moveInfo->moveAcc[gMoveSelectionCursor[gActiveBattler]];
	moveInfo->moveAcc[gMoveSelectionCursor[gActiveBattler]] = moveInfo->moveAcc[gMultiUsePlayerCursor];
	moveInfo->moveAcc[gMultiUsePlayerCursor] = i;

	//Swap Effectiveness Details
	for (j = 0; j < gBattlersCount; ++j)
	{
		i = moveInfo->moveResults[j][gMoveSelectionCursor[gActiveBattler]];
		moveInfo->moveResults[j][gMoveSelectionCursor[gActiveBattler]] = moveInfo->moveResults[j][gMultiUsePlayerCursor];
		moveInfo->moveResults[j][gMultiUsePlayerCursor] = i;
	}

	//Swap Possible Z-Moves
	i = moveInfo->possibleZMoves[gMoveSelectionCursor[gActiveBattler]];
	moveInfo->possibleZMoves[gMoveSelectionCursor[gActiveBattler]] = moveInfo->possibleZMoves[gMultiUsePlayerCursor];
	moveInfo->possibleZMoves[gMultiUsePlayerCursor] = i;

	//Swap Possible Max-Moves
	i = moveInfo->possibleMaxMoves[gMoveSelectionCursor[gActiveBattler]];
	moveInfo->possibleMaxMoves[gMoveSelectionCursor[gActiveBattler]] = moveInfo->possibleMaxMoves[gMultiUsePlayerCursor];
	moveInfo->possibleMaxMoves[gMultiUsePlayerCursor] = i;

	if (gDisableStructs[gActiveBattler].mimickedMoves & gBitTable[gMoveSelectionCursor[gActiveBattler]])
	{
		gDisableStructs[gActiveBattler].mimickedMoves &= (~gBitTable[gMoveSelectionCursor[gActiveBattler]]);
		gDisableStructs[gActiveBattler].mimickedMoves |= gBitTable[gMultiUsePlayerCursor];
	}

	MoveSelectionDisplayMoveNames();

	for (i = 0; i < MAX_MON_MOVES; i++)
		perMovePPBonuses[i] = (gBattleMons[gActiveBattler].ppBonuses & (3 << (i * 2))) >> (i * 2);

	totalPPBonuses = perMovePPBonuses[gMoveSelectionCursor[gActiveBattler]];
	perMovePPBonuses[gMoveSelectionCursor[gActiveBattler]] = perMovePPBonuses[gMultiUsePlayerCursor];
	perMovePPBonuses[gMultiUsePlayerCursor] = totalPPBonuses;

	totalPPBonuses = 0;
	for (i = 0; i < MAX_MON_MOVES; i++)
		totalPPBonuses |= perMovePPBonuses[i] << (i * 2);

	gBattleMons[gActiveBattler].ppBonuses = totalPPBonuses;

	for (i = 0; i < MAX_MON_MOVES; i++)
	{
		gBattleMons[gActiveBattler].moves[i] = moveInfo->moves[i];
		gBattleMons[gActiveBattler].pp[i] = moveInfo->currentPp[i];
	}

	if (!(gBattleMons[gActiveBattler].status2 & STATUS2_TRANSFORMED))
	{
		for (i = 0; i < MAX_MON_MOVES; i++)
		{
			moveStruct.moves[i] = GetMonData(&gPlayerParty[gBattlerPartyIndexes[gActiveBattler]], MON_DATA_MOVE1 + i, 0);
			moveStruct.currentPp[i] = GetMonData(&gPlayerParty[gBattlerPartyIndexes[gActiveBattler]], MON_DATA_PP1 + i, 0);
		}

		totalPPBonuses = GetMonData(&gPlayerParty[gBattlerPartyIndexes[gActiveBattler]], MON_DATA_PP_BONUSES, 0);
		for (i = 0; i < MAX_MON_MOVES; i++)
			perMovePPBonuses[i] = (totalPPBonuses & (3 << (i * 2))) >> (i * 2);

		i = moveStruct.moves[gMoveSelectionCursor[gActiveBattler]];
		moveStruct.moves[gMoveSelectionCursor[gActiveBattler]] = moveStruct.moves[gMultiUsePlayerCursor];
		moveStruct.moves[gMultiUsePlayerCursor] = i;

		i = moveStruct.currentPp[gMoveSelectionCursor[gActiveBattler]];
		moveStruct.currentPp[gMoveSelectionCursor[gActiveBattler]] = moveStruct.currentPp[gMultiUsePlayerCursor];
		moveStruct.currentPp[gMultiUsePlayerCursor] = i;

		totalPPBonuses = perMovePPBonuses[gMoveSelectionCursor[gActiveBattler]];
		perMovePPBonuses[gMoveSelectionCursor[gActiveBattler]] = perMovePPBonuses[gMultiUsePlayerCursor];
		perMovePPBonuses[gMultiUsePlayerCursor] = totalPPBonuses;

		totalPPBonuses = 0;
		for (i = 0; i < MAX_MON_MOVES; i++)
			totalPPBonuses |= perMovePPBonuses[i] << (i * 2);

		for (i = 0; i < MAX_MON_MOVES; i++)
		{
			SetMonData(&gPlayerParty[gBattlerPartyIndexes[gActiveBattler]], MON_DATA_MOVE1 + i, &moveStruct.moves[i]);
			SetMonData(&gPlayerParty[gBattlerPartyIndexes[gActiveBattler]], MON_DATA_PP1 + i, &moveStruct.currentPp[i]);
		}

		SetMonData(&gPlayerParty[gBattlerPartyIndexes[gActiveBattler]], MON_DATA_PP_BONUSES, &totalPPBonuses);
	}
}

void HandleInputChooseTarget(void)
{
	int i;
	u8 identities[4];
	memcpy(identities, sTargetIdentities, 4 /*NELEMS(sTargetIdentities)*/); //sTargetIdentities = 0x8250980
	struct ChooseMoveStruct *moveInfo = (struct ChooseMoveStruct*)(&gBattleBufferA[gActiveBattler][4]);

	DoBounceEffect(gMultiUsePlayerCursor, BOUNCE_HEALTHBOX, 15, 1); //0x8012138

	if (gBattlersCount != 0)
	{
		for (i = 0; i < gBattlersCount; ++i)
		{
			if (i != gMultiUsePlayerCursor)
				EndBounceEffect(i, BOUNCE_HEALTHBOX); //0x8012258
		}
	}

	if (JOY_NEW(A_BUTTON))
	{
		PlaySE(SE_SELECT);
		gSprites[gBattlerSpriteIds[gMultiUsePlayerCursor]].callback = sub_8012098; //sub_8039B2C in Emerald
		TryRemoveDoublesKillingScore(gActiveBattler, gMultiUsePlayerCursor, moveInfo->moves[gMoveSelectionCursor[gActiveBattler]]);
		EmitMoveChosen(1, gMoveSelectionCursor[gActiveBattler], gMultiUsePlayerCursor, gNewBS->megaData.chosen[gActiveBattler], gNewBS->ultraData.chosen[gActiveBattler], gNewBS->zMoveData.toBeUsed[gActiveBattler], gNewBS->dynamaxData.toBeUsed[gActiveBattler]);
		CloseZMoveDetails();
		CloseMaxMoveDetails();
		EndBounceEffect(gMultiUsePlayerCursor, BOUNCE_HEALTHBOX);
		PlayerBufferExecCompleted();
	}
	else if (JOY_NEW(B_BUTTON))
	{
		PlaySE(SE_SELECT);
		gSprites[gBattlerSpriteIds[gMultiUsePlayerCursor]].callback = sub_8012098; //sub_8039B2C in Emerald
		gBattlerControllerFuncs[gActiveBattler] = HandleInputChooseMove;
		DoBounceEffect(gActiveBattler, BOUNCE_HEALTHBOX, 7, 1);
		DoBounceEffect(gActiveBattler, BOUNCE_MON, 7, 1);
		EndBounceEffect(gMultiUsePlayerCursor, BOUNCE_HEALTHBOX);

		ReloadMoveNamesIfNecessary();
		TryLoadMegaTriggers();
		TryLoadZTrigger();
		TryLoadDynamaxTrigger();
		MoveSelectionDisplayMoveType();
	}
	else if (JOY_NEW(DPAD_LEFT | DPAD_UP))
	{
		PlaySE(SE_SELECT);
		gSprites[gBattlerSpriteIds[gMultiUsePlayerCursor]].callback = sub_8012098; //sub_8039B2C in Emerald

		do
		{
			u8 currSelIdentity = GetBattlerPosition(gMultiUsePlayerCursor);

			u16 move = moveInfo->moves[gMoveSelectionCursor[gActiveBattler]];
			switch (move) {
				case MOVE_ACUPRESSURE:
					if (!(gAbsentBattlerFlags & gBitTable[gMultiUsePlayerCursor ^ BIT_FLANK]))
					{
						gMultiUsePlayerCursor ^= BIT_FLANK;
					}
					gSprites[gBattlerSpriteIds[gMultiUsePlayerCursor]].callback = sub_8012044; //sub_8039AD8 in Emerald
					return;

				case MOVE_POLLENPUFF:
					if (IsHealBlocked(gActiveBattler) //Affected by Heal Block can't target partner
					&& !(gAbsentBattlerFlags & gBitTable[gMultiUsePlayerCursor ^ 2]))
					{
						gMultiUsePlayerCursor ^= BIT_FLANK;
						gSprites[gBattlerSpriteIds[gMultiUsePlayerCursor]].callback = sub_8012044; //sub_8039AD8 in Emerald
						return;
					}
			}

			for (i = 0; i < MAX_BATTLERS_COUNT; i++)
			{
				if (currSelIdentity == identities[i])
					break;
			}
			do
			{
				if (--i < 0)
					i = MAX_BATTLERS_COUNT - 1; // UB: array out of range
				gMultiUsePlayerCursor = GetBattlerAtPosition(identities[i]);
			} while (gMultiUsePlayerCursor == gBattlersCount);

			i = 0;
			move = moveInfo->moves[gMoveSelectionCursor[gActiveBattler]];
			switch (GetBattlerPosition(gMultiUsePlayerCursor)) {
				case B_POSITION_PLAYER_LEFT:
				case B_POSITION_PLAYER_RIGHT:
					if (gActiveBattler != gMultiUsePlayerCursor)
						i++;
					else if (gBattleMoves[move].target & MOVE_TARGET_USER_OR_PARTNER)
						i++;

					if ((moveInfo->dynamaxed || gNewBS->dynamaxData.viewing) && gMultiUsePlayerCursor == PARTNER(gActiveBattler))
						i = FALSE; //Can't use Max Move on partner

					break;
				case B_POSITION_OPPONENT_LEFT:
				case B_POSITION_OPPONENT_RIGHT:
					i++;
					break;
			}

			if (gAbsentBattlerFlags & gBitTable[gMultiUsePlayerCursor])
				i = 0;

		} while (i == 0);

		gSprites[gBattlerSpriteIds[gMultiUsePlayerCursor]].callback = sub_8012044; //sub_8039AD8 in Emerald
		MoveSelectionDisplayMoveType();
	}
	else if (JOY_NEW(DPAD_RIGHT | DPAD_DOWN))
	{
		PlaySE(SE_SELECT);
		gSprites[gBattlerSpriteIds[gMultiUsePlayerCursor]].callback = sub_8012098; //sub_8039B2C in Emerald

		do
		{
			u8 currSelIdentity = GetBattlerPosition(gMultiUsePlayerCursor);

			u16 move = moveInfo->moves[gMoveSelectionCursor[gActiveBattler]];
			switch (move) {
				case MOVE_ACUPRESSURE:
					if (!(gAbsentBattlerFlags & gBitTable[gMultiUsePlayerCursor ^ BIT_FLANK]))
					{
						gMultiUsePlayerCursor ^= BIT_FLANK;
					}
					gSprites[gBattlerSpriteIds[gMultiUsePlayerCursor]].callback = sub_8012044; //sub_8039AD8 in Emerald
					return;

				case MOVE_POLLENPUFF:
					if (IsHealBlocked(gActiveBattler) //Affected by Heal Block can't target partner
					&& !(gAbsentBattlerFlags & gBitTable[gMultiUsePlayerCursor ^ 2]))
					{
						gMultiUsePlayerCursor ^= BIT_FLANK;
						gSprites[gBattlerSpriteIds[gMultiUsePlayerCursor]].callback = sub_8012044; //sub_8039AD8 in Emerald
						return;
					}
			}

			for (i = 0; i < MAX_BATTLERS_COUNT; i++)
			{
				if (currSelIdentity == identities[i])
					break;
			}
			do
			{
				if (++i > 3)
					i = 0;
				gMultiUsePlayerCursor = GetBattlerAtPosition(identities[i]);
			} while (gMultiUsePlayerCursor == gBattlersCount);

			i = 0;
			move = moveInfo->moves[gMoveSelectionCursor[gActiveBattler]];
			switch (GetBattlerPosition(gMultiUsePlayerCursor)) {
				case B_POSITION_PLAYER_LEFT:
				case B_POSITION_PLAYER_RIGHT:
					if (gActiveBattler != gMultiUsePlayerCursor)
						i++;
					else if (gBattleMoves[move].target & MOVE_TARGET_USER_OR_PARTNER)
						i++;

					if ((moveInfo->dynamaxed || gNewBS->dynamaxData.viewing) && gMultiUsePlayerCursor == PARTNER(gActiveBattler))
						i = FALSE; //Can't use Max Move on partner
					break;
				case B_POSITION_OPPONENT_LEFT:
				case B_POSITION_OPPONENT_RIGHT:
					i++;
					break;
			}

			if (gAbsentBattlerFlags & gBitTable[gMultiUsePlayerCursor])
				i = 0;

		} while (i == 0);

		gSprites[gBattlerSpriteIds[gMultiUsePlayerCursor]].callback = sub_8012044; //sub_8039AD8 in Emerald
		MoveSelectionDisplayMoveType();
	}
}

u8 TrySetCantSelectMoveBattleScript(void)
{
	u8 limitations, ability, holdEffect, isAnyMaxMove;
	u16 move, *choicedMove;

	limitations = 0; isAnyMaxMove = FALSE;
	ability = ABILITY(gActiveBattler);
	holdEffect = ITEM_EFFECT(gActiveBattler);
	move = gBattleMons[gActiveBattler].moves[gBattleBufferB[gActiveBattler][2]];
	choicedMove = &gBattleStruct->choicedMove[gActiveBattler];

	gStringBank = gActiveBattler;
	gBattleScripting.bank = gActiveBattler;
	gCurrentMove = move;

	if (RAID_BATTLE_END) //Fix bug where partner would be forced to use a move with 0 PP and cause the game to crash
		return FALSE;

	if (IsDynamaxed(gActiveBattler) || gNewBS->dynamaxData.toBeUsed[gActiveBattler])
		move = gCurrentMove = GetMaxMove(gActiveBattler, gBattleBufferB[gActiveBattler][2]);

	if (IsAnyMaxMove(move))
		isAnyMaxMove = TRUE; //Save time later

	if (gDisableStructs[gActiveBattler].encoredMove
	&& gDisableStructs[gActiveBattler].encoredMove != move
	&& !gNewBS->zMoveData.toBeUsed[gActiveBattler] //Can still use other Z-Moves even if encored
	&& !isAnyMaxMove)
	{
		gCurrentMove = gDisableStructs[gActiveBattler].encoredMove;
		gSelectionBattleScripts[gActiveBattler] = BattleScript_MustSelectEncoredMove;
		++limitations;
	}
	else if (gDisableStructs[gActiveBattler].disabledMove == move && move != MOVE_NONE)
	{
		gSelectionBattleScripts[gActiveBattler] = BattleScript_SelectingDisabledMove;
		++limitations;
	}
	else if (IsTormented(gActiveBattler) && !isAnyMaxMove && move == gLastUsedMoves[gActiveBattler] && move != MOVE_STRUGGLE)
	{
		CancelMultiTurnMoves(gActiveBattler);
		gSelectionBattleScripts[gActiveBattler] = BattleScript_SelectingTormentedMove;
		++limitations;
	}
	else if (IsTaunted(gActiveBattler) != 0 && SPLIT(move) == SPLIT_STATUS)
	{
		gSelectionBattleScripts[gActiveBattler] = BattleScript_SelectingNotAllowedMoveTaunt;
		limitations++;
	}
	else if (IsImprisoned(gActiveBattler, move) && !isAnyMaxMove)
	{
		gSelectionBattleScripts[gActiveBattler] = BattleScript_SelectingImprisionedMove;
		++limitations;
	}
	else if (!isAnyMaxMove && ability == ABILITY_GORILLATACTICS && *choicedMove != 0 && *choicedMove != 0xFFFF && *choicedMove != move)
	{
		gCurrentMove = *choicedMove;
		gLastUsedAbility = ability;
		gSelectionBattleScripts[gActiveBattler] = BattleScript_SelectingNotAllowedMoveChoiceAbility;
		++limitations;
	}
	else if (!isAnyMaxMove && holdEffect == ITEM_EFFECT_CHOICE_BAND && *choicedMove != 0 && *choicedMove != 0xFFFF && *choicedMove != move)
	{
		gCurrentMove = *choicedMove;
		gLastUsedItem = ITEM(gActiveBattler);
		gSelectionBattleScripts[gActiveBattler] = BattleScript_SelectingNotAllowedMoveChoiceItem;
		++limitations;
	}
	else if (holdEffect == ITEM_EFFECT_ASSAULT_VEST && SPLIT(move) == SPLIT_STATUS)
	{
		gLastUsedItem = ITEM(gActiveBattler);
		gSelectionBattleScripts[gActiveBattler] = BattleScript_SelectingNotAllowedMoveAssaultVest;
		++limitations;
	}
	#ifdef FLAG_SKY_BATTLE
	else if (FlagGet(FLAG_SKY_BATTLE) && CheckTableForMove(move, gSkyBattleBannedMoves))
	{
		gSelectionBattleScripts[gActiveBattler] = BattleScript_SelectingNotAllowedSkyBattle;
		++limitations;
	}
	#endif
	else if (IsGravityActive() && CheckTableForMove(move, gGravityBannedMoves))
	{
		gSelectionBattleScripts[gActiveBattler] = BattleScript_SelectingNotAllowedGravity;
		++limitations;
	}
	else if (CantUseSoundMoves(gActiveBattler) && CheckSoundMove(move))
	{
		gSelectionBattleScripts[gActiveBattler] = BattleScript_SelectingNotAllowedThroatChop;
		++limitations;
	}
	else if (IsHealBlocked(gActiveBattler) && CheckHealingMove(move))
	{
		gSelectionBattleScripts[gActiveBattler] = BattleScript_SelectingNotAllowedHealBlock;
		++limitations;
	}
	else if (gBattleMons[gActiveBattler].pp[gBattleBufferB[gActiveBattler][2]] == 0)
	{
		gSelectionBattleScripts[gActiveBattler] = BattleScript_SelectingMoveWithNoPP;
		++limitations;
	}
	
	if (limitations != 0)
	{
		gNewBS->dynamaxData.toBeUsed[gActiveBattler] = FALSE;
		gNewBS->zMoveData.toBeUsed[gActiveBattler] = FALSE;
	}

	return limitations;
}

void PlayerHandleChooseAction(void)
{
	int i;
	u16 itemId = gBattleBufferA[gActiveBattler][2] | (gBattleBufferA[gActiveBattler][3] << 8);
	bool8 raidBattleEnd = RAID_BATTLE_END;

	gNewBS->zMoveData.toBeUsed[gActiveBattler] = FALSE;

	if (IS_DOUBLE_BATTLE)
	{
		if (raidBattleEnd && gActiveBattler == GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT))
		{
			if ((gBattleBufferA[gActiveBattler][1] == ACTION_USE_ITEM && GetPocketByItemId(itemId) == POCKET_POKEBALLS)
			|| gBattleBufferA[gActiveBattler][1] == ACTION_RUN)
			{
				gNewBS->NoMoreMovingThisTurn |= gBitTable[gActiveBattler];
				EmitTwoReturnValues(1, ACTION_USE_MOVE, 0);
				PlayerBufferExecCompleted();
				return;
			}
			else
			{
				EmitTwoReturnValues(1, ACTION_CANCEL_PARTNER, 0);
				PlayerBufferExecCompleted();
				return;
			}
		}
		else
		{
			//Running or using balls cancels the second mon's attack
			if (!IsBagDisabled()
			&&  gBattleBufferA[gActiveBattler][1] == ACTION_USE_ITEM //If mon 1 used a ball, then
			&&  GetPocketByItemId(itemId) == POCKET_POKEBALLS)		 //mon 2 doesn't get to do anything.
			{
				gNewBS->NoMoreMovingThisTurn |= gBitTable[gActiveBattler];

				if (raidBattleEnd) //Raid mon can't move if it's dead
					gNewBS->NoMoreMovingThisTurn |= gBitTable[BANK_RAID_BOSS];

				EmitTwoReturnValues(1, ACTION_USE_MOVE, 0);
				PlayerBufferExecCompleted();
				return;
			}
			else if (GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT) == gActiveBattler
			&& gBattleBufferA[gActiveBattler][1] == ACTION_RUN)
			{
				gNewBS->NoMoreMovingThisTurn |= gBitTable[gActiveBattler];
				EmitTwoReturnValues(1, ACTION_USE_MOVE, 0);
				PlayerBufferExecCompleted();
				return;
			}
		}
	}

	gBattlerControllerFuncs[gActiveBattler] = HandleChooseActionAfterDma3;

	if (raidBattleEnd) //Time to catch Raid opponent
		BattlePutTextOnWindow(gText_BattleMenuRaidEnd, 2);
	else if ((IS_DOUBLE_BATTLE)
	&& GetBattlerPosition(gActiveBattler) == B_POSITION_PLAYER_RIGHT
	&& !(gAbsentBattlerFlags & gBitTable[GetBattlerAtPosition(B_POSITION_PLAYER_LEFT)])
	&& !(gBattleTypeFlags & (BATTLE_TYPE_MULTI | BATTLE_TYPE_INGAME_PARTNER))
	&& gBattleBufferA[gActiveBattler][1] != ACTION_USE_ITEM) //Mon 1 didn't use item
	#ifndef UNBOUND
		BattlePutTextOnWindow(gText_BattleMenu2, 2);
	else
		BattlePutTextOnWindow(gText_BattleMenu, 2);
	#else
		BattlePutTextOnWindow(gText_UnboundBattleMenu2, 2);
	else
		BattlePutTextOnWindow(gText_UnboundBattleMenu, 2);
	#endif

	for (i = 0; i < 4; i++)
		ActionSelectionDestroyCursorAt(i);

	ActionSelectionCreateCursorAt(gActionSelectionCursor[gActiveBattler], 0);

	if (raidBattleEnd)
		BattleStringExpandPlaceholdersToDisplayedString(gText_CaptureRaidMon);
	else
		BattleStringExpandPlaceholdersToDisplayedString(gText_WhatWillPkmnDo);
	BattlePutTextOnWindow(gDisplayedStringBattle, 1);
}

void HandleInputChooseAction(void)
{
	DoBounceEffect(gActiveBattler, BOUNCE_HEALTHBOX, 7, 1);
	DoBounceEffect(gActiveBattler, BOUNCE_MON, 7, 1);

	if (gMain.newKeys & A_BUTTON)
	{
		PlaySE(SE_SELECT);

		if (RAID_BATTLE_END)
		{
			switch (gActionSelectionCursor[gActiveBattler]) {
				case 0:
					EmitTwoReturnValues(1, ACTION_USE_ITEM, 0);
					break;
				case 1:
					EmitTwoReturnValues(1, ACTION_RUN, 0);
					break;
			}
		}
		else
		{
			switch (gActionSelectionCursor[gActiveBattler]) {
				case 0:
					EmitTwoReturnValues(1, ACTION_USE_MOVE, 0);
					break;
				case 1:
					gNewBS->megaData.chosen[gActiveBattler] = FALSE;
					gNewBS->ultraData.chosen[gActiveBattler] = FALSE;
					EmitTwoReturnValues(1, ACTION_USE_ITEM, 0);
					break;
				case 2:
					gNewBS->megaData.chosen[gActiveBattler] = FALSE;
					gNewBS->ultraData.chosen[gActiveBattler] = FALSE;
					EmitTwoReturnValues(1, ACTION_SWITCH, 0);
					break;
				case 3:
					gNewBS->megaData.chosen[gActiveBattler] = FALSE;
					gNewBS->ultraData.chosen[gActiveBattler] = FALSE;

					if ((IS_DOUBLE_BATTLE)
					&& GetBattlerPosition(gActiveBattler) == B_POSITION_PLAYER_RIGHT
					&& !(gAbsentBattlerFlags & gBitTable[GetBattlerAtPosition(B_POSITION_PLAYER_LEFT)])
					&& !(gBattleTypeFlags & (BATTLE_TYPE_MULTI | BATTLE_TYPE_INGAME_PARTNER))
					&& gBattleBufferA[gActiveBattler][1] != ACTION_USE_ITEM) //Mon 1 didn't use item
						goto CANCEL_PARTNER;

					EmitTwoReturnValues(1, ACTION_RUN, 0);
					break;
			}
		}

		PlayerBufferExecCompleted();
	}
	else if (gMain.newKeys & DPAD_LEFT)
	{
		if (gActionSelectionCursor[gActiveBattler] & 1) // if is B_ACTION_USE_ITEM or B_ACTION_RUN
		{
			PlaySE(SE_SELECT);
			ActionSelectionDestroyCursorAt(gActionSelectionCursor[gActiveBattler]);
			gActionSelectionCursor[gActiveBattler] ^= 1;
			ActionSelectionCreateCursorAt(gActionSelectionCursor[gActiveBattler], 0);
		}
	}
	else if (gMain.newKeys & DPAD_RIGHT)
	{
		if (!(gActionSelectionCursor[gActiveBattler] & 1)) // if is B_ACTION_USE_MOVE or B_ACTION_SWITCH
		{
			PlaySE(SE_SELECT);
			ActionSelectionDestroyCursorAt(gActionSelectionCursor[gActiveBattler]);
			gActionSelectionCursor[gActiveBattler] ^= 1;
			ActionSelectionCreateCursorAt(gActionSelectionCursor[gActiveBattler], 0);
		}
	}
	else if (gMain.newKeys & DPAD_UP)
	{
		if (!RAID_BATTLE_END && gActionSelectionCursor[gActiveBattler] & 2) // if is B_ACTION_SWITCH or B_ACTION_RUN
		{
			PlaySE(SE_SELECT);
			ActionSelectionDestroyCursorAt(gActionSelectionCursor[gActiveBattler]);
			gActionSelectionCursor[gActiveBattler] ^= 2;
			ActionSelectionCreateCursorAt(gActionSelectionCursor[gActiveBattler], 0);
		}
	}
	else if (gMain.newKeys & DPAD_DOWN)
	{
		if (!RAID_BATTLE_END && !(gActionSelectionCursor[gActiveBattler] & 2)) // if is B_ACTION_USE_MOVE or B_ACTION_USE_ITEM
		{
			PlaySE(SE_SELECT);
			ActionSelectionDestroyCursorAt(gActionSelectionCursor[gActiveBattler]);
			gActionSelectionCursor[gActiveBattler] ^= 2;
			ActionSelectionCreateCursorAt(gActionSelectionCursor[gActiveBattler], 0);
		}
	}
	else if (gMain.newKeys & B_BUTTON)
	{
		if ((IS_DOUBLE_BATTLE)
		 && GetBattlerPosition(gActiveBattler) == B_POSITION_PLAYER_RIGHT
		 && !(gAbsentBattlerFlags & gBitTable[GetBattlerAtPosition(B_POSITION_PLAYER_LEFT)])
		 && !(gBattleTypeFlags & (BATTLE_TYPE_MULTI | BATTLE_TYPE_INGAME_PARTNER)))
		{
		CANCEL_PARTNER:
			if (gBattleBufferA[gActiveBattler][1] == ACTION_USE_ITEM)
				return;

			PlaySE(SE_SELECT);
			EmitTwoReturnValues(1, ACTION_CANCEL_PARTNER, 0);
			PlayerBufferExecCompleted();
		}
	}
	else if (gMain.newKeys & START_BUTTON)
	{
		SwapHpBarsWithHpText();
	}
}

bool8 CheckCantMoveThisTurn(void)
{
	if (gNewBS->NoMoreMovingThisTurn & gBitTable[gActiveBattler])
	{
		EmitMoveChosen(1, 0, gMultiUsePlayerCursor, FALSE, FALSE, FALSE, FALSE);
		return TRUE;
	}
	return FALSE;
}

bool8 IsBagDisabled(void)
{
	#ifdef VAR_GAME_DIFFICULTY
	u8 difficulty = VarGet(VAR_GAME_DIFFICULTY);
	
	if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
	{
		if (difficulty == OPTIONS_HARD_DIFFICULTY)
		{
			if (gNewBS->playerItemUsedCount >= 4) //Max four items can be used
				return FALSE;
		}
		if (difficulty >= OPTIONS_EXPERT_DIFFICULTY) //No items in battles for Experts
			return FALSE;
	}
	#endif

	return FlagGet(FLAG_DISABLE_BAG) || (gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_EREADER_TRAINER | BATTLE_TYPE_FRONTIER));
}

static void TryLoadTypeIcons(void)
{
	#ifndef HEALTHBAR_TYPE_ICONS
	if (((gBattleTypeFlags & BATTLE_TYPE_CAMOMONS)
	#ifdef FLAG_HEALTHBAR_TYPE_ICONS
	|| FlagGet(FLAG_HEALTHBAR_TYPE_ICONS)
	#endif
	)
	&&
	#else
	if (
	#endif
	!(gBattleTypeFlags & BATTLE_TYPE_LINK)
	&& IndexOfSpritePaletteTag(TYPE_ICON_TAG) == 0xFF)
	{
		LoadSpritePalette(&sTypeIconPalTemplate);
		LoadSpritePalette(&sTypeIconPalTemplate2);
		for (u8 position = 0; position < gBattlersCount; ++position)
		{
			if (!BATTLER_ALIVE(GetBattlerAtPosition(position)))
				continue;

			for (u8 typeNum = 0; typeNum < 2; ++typeNum) //Load each type
			{
				u8 spriteId;
				s16 x = sTypeIconPositions[position][IS_SINGLE_BATTLE].x;
				s16 y = sTypeIconPositions[position][IS_SINGLE_BATTLE].y + (11 * typeNum); //2nd type is 13px below

				u8* type1Ptr;
				if (gStatuses3[GetBattlerAtPosition(position)] & STATUS3_ILLUSION && !(gBattleTypeFlags & BATTLE_TYPE_CAMOMONS))
					type1Ptr = &gBaseStats[GetIllusionPartyData(GetBattlerAtPosition(position))->species].type1;
				else
					type1Ptr = &gBattleMons[GetBattlerAtPosition(position)].type1;

				u8 type = *(type1Ptr + typeNum);

				switch (type) { //Certain types have a different palette
					case TYPE_FLYING:
					case TYPE_POISON:
					case TYPE_GROUND:
					case TYPE_DRAGON:
					case TYPE_MYSTERY:
					case TYPE_ROOSTLESS:
					case TYPE_BLANK:
						spriteId = CreateSpriteAtEnd(&sTypeIconSpriteTemplate2, x, y, 0xFF);
						break;
					default:
						spriteId = CreateSpriteAtEnd(&sTypeIconSpriteTemplate, x, y, 0xFF);
				}

				if (spriteId != MAX_SPRITES)
				{
					struct Sprite* sprite = &gSprites[spriteId];
					sprite->data[0] = position;
					sprite->data[1] = gActiveBattler;
					sprite->data[3] = y; //Save original y-value for bouncing

					if (IS_SINGLE_BATTLE)
					{
						if (SIDE(GetBattlerAtPosition(position)) == B_SIDE_PLAYER)
							SetSpriteOamFlipBits(sprite, TRUE, FALSE); //Flip horizontally
					}
					else //Double Battle
					{
						if (SIDE(GetBattlerAtPosition(position)) == B_SIDE_OPPONENT)
							SetSpriteOamFlipBits(sprite, TRUE, FALSE); //Flip horizontally
					}

					RequestSpriteFrameImageCopy(type, sprite->oam.tileNum, sprite->images);
				}
			}
		}
	}
}

static void SpriteCB_CamomonsTypeIcon(struct Sprite* sprite)
{
	u8 position = sprite->data[0];
	u8 bank = sprite->data[1];

	if (sprite->data[2] == 10)
	{
		FreeSpritePaletteByTag(TYPE_ICON_TAG);
		DestroySprite(sprite);
		return;
	}

	//Type icons should prepare to destroy themselves if the Player is not choosing an action
	if (gBattlerControllerFuncs[bank] != (void*) (0x08032C90 | 1) //PlayerHandleChooseMove
	&&  gBattlerControllerFuncs[bank] != (void*) (0x0802EA10 | 1) //HandleInputChooseMove
	&&  gBattlerControllerFuncs[bank] != (void*) (0x08032C4C | 1) //HandleChooseMoveAfterDma3
	&&  gBattlerControllerFuncs[bank] != HandleInputChooseMove
	&&  gBattlerControllerFuncs[bank] != HandleInputChooseTarget
	&&  gBattlerControllerFuncs[bank] != HandleMoveSwitching
	&&  gBattlerControllerFuncs[bank] != HandleInputChooseMove)
	{
		if (IS_SINGLE_BATTLE)
		{
			switch (position) {
				case B_POSITION_PLAYER_LEFT:
					sprite->pos1.x -= 1;
					break;
				case B_POSITION_OPPONENT_LEFT:
					sprite->pos1.x += 1;
					break;
			}
		}
		else //Double Battle
		{
			switch (position) {
				case B_POSITION_PLAYER_LEFT:
				case B_POSITION_PLAYER_RIGHT:
					sprite->pos1.x += 1;
					break;
				case B_POSITION_OPPONENT_LEFT:
				case B_POSITION_OPPONENT_RIGHT:
					sprite->pos1.x -= 1;
					break;
			}
		}

		++sprite->data[2];
		return;
	}

	if (IS_SINGLE_BATTLE)
	{
		switch (position) {
			case B_POSITION_PLAYER_LEFT:
				if (sprite->pos1.x < sTypeIconPositions[position][TRUE].x + 10)
					sprite->pos1.x += 1;
				break;
			case B_POSITION_OPPONENT_LEFT:
				if (sprite->pos1.x > sTypeIconPositions[position][TRUE].x - 10)
					sprite->pos1.x -= 1;
				break;
		}
	}
	else //Double Battle
	{
		switch (position) {
			case B_POSITION_PLAYER_LEFT:
			case B_POSITION_PLAYER_RIGHT:
				if (sprite->pos1.x > sTypeIconPositions[position][FALSE].x - 10)
					sprite->pos1.x -= 1;
				break;
			case B_POSITION_OPPONENT_LEFT:
			case B_POSITION_OPPONENT_RIGHT:
				if (sprite->pos1.x < sTypeIconPositions[position][FALSE].x + 10)
					sprite->pos1.x += 1;
				break;
		}
	}

	//Deal with bouncing player healthbox
	s16 originalY = sprite->data[3];
	struct Sprite* healthbox = &gSprites[gHealthboxSpriteIds[GetBattlerAtPosition(position)]];
	sprite->pos1.y = originalY + healthbox->pos2.y;
}
