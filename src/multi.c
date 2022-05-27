#include "defines.h"
#include "defines_battle.h"
#include "../include/battle_anim.h"
#include "../include/event_data.h"
#include "../include/m4a.h"
#include "../include/pokeball.h"
#include "../include/sound.h"
#include "../include/battle_string_ids.h"

#include "../include/new/ai_master.h"
#include "../include/new/ai_switching.h"
#include "../include/new/ai_util.h"
#include "../include/new/battle_anims.h"
#include "../include/new/battle_util.h"
#include "../include/new/build_pokemon.h"
#include "../include/new/dynamax.h"
#include "../include/new/frontier.h"
#include "../include/new/mega.h"
#include "../include/new/move_menu.h"
#include "../include/new/multi.h"
#include "../include/new/switching.h"
/*
multi.c
	handles partner battle logic
*/

#define BANK_PLAYER_ALLY 2
#define sBattler data[6]

#define Script_TrainerSpotted (u8*) 0x081A4EB4

//This file's functions:
static void BattleIntroOpponent2SendsOutMonAnimation(void);
static u32 CalcPrizeiMoneyForTrainer(u16 trainerId);
static void PlayerPartnerBufferExecComplete(void);
static void PlayerPartnerBufferRunCommand(void);
static void PlayerPartnerHandleChooseMove(void);
static void PlayerPartnerHandlePrintSelectionString(void);
static void PlayerPartnerHandleChooseAction(void);
static void PlayerPartnerHandleChoosePokemon(void);

extern void (*const sPlayerPartnerBufferCommands[COMMAND_MAX])(void);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void BattleIntroOpponent1SendsOutMonAnimation(void)
{
	if (gBattleExecBuffer)
		return;

	for (gActiveBattler = 0; gActiveBattler < gBattlersCount; ++gActiveBattler)
	{
		if (GetBattlerPosition(gActiveBattler) == B_POSITION_OPPONENT_LEFT)
		{
			EmitIntroTrainerBallThrow(0);
			MarkBufferBankForExecution(gActiveBattler);
			if (gBattleTypeFlags & (BATTLE_TYPE_MULTI | BATTLE_TYPE_TWO_OPPONENTS))
			{
				gBattleMainFunc = BattleIntroOpponent2SendsOutMonAnimation;
				return;
			}
		}
	}

	gBattleMainFunc = BattleIntroRecordMonsToDex;
}

static void BattleIntroOpponent2SendsOutMonAnimation(void)
{
	for (gActiveBattler = 0; gActiveBattler < gBattlersCount; ++gActiveBattler)
	{
		if (GetBattlerPosition(gActiveBattler) == B_POSITION_OPPONENT_RIGHT)
		{
			EmitIntroTrainerBallThrow(0);
			MarkBufferBankForExecution(gActiveBattler);
		}
	}

	gBattleMainFunc = BattleIntroRecordMonsToDex;
}

void MultiInitPokemonOrder(void)
{
	/*
	 * In a regular double battle, the first two pokemon are the first
	 * two be set out. However in a multi battle, both trainers need
	 * to send out their first Pokemon, which corresponds to slot 0
	 * and 3.
	 */
	if (gBattleTypeFlags & (BATTLE_TYPE_MULTI | BATTLE_TYPE_TWO_OPPONENTS))
	{
		gBattlerPartyIndexes[B_POSITION_OPPONENT_LEFT] = 0;
		gBattlerPartyIndexes[B_POSITION_OPPONENT_RIGHT] = 3;
	}

	if (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER)
		gBattlerPartyIndexes[BANK_PLAYER_ALLY] = 3;
}

void MultiBattleAddSecondOpponent(void)
{
	if (gBattleTypeFlags & (BATTLE_TYPE_MULTI | BATTLE_TYPE_TWO_OPPONENTS))
	{
		if (GetBattlerPosition(gActiveBattler) == 3)
		{
			EmitDrawTrainerPic(0);
			MarkBufferBankForExecution(gActiveBattler);
		}
	}

	/* Send command to partner in case of tag team battle */
	if (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER)
	{
		if (GetBattlerPosition(gActiveBattler) == 2)
		{
			EmitDrawTrainerPic(0);
			MarkBufferBankForExecution(gActiveBattler);
		}
	}
}

extern void SpriteCallbackDummy3(struct Sprite* sprite);
#define NULL_SPRITE_CALLBACK(spriteId) (gSprites[spriteId].callback == SpriteCallbackDummy || gSprites[spriteId].callback == SpriteCallbackDummy3)

static void ShowBattlerHealthbox(u8 bank)
{
	UpdateHealthboxAttribute(gHealthboxSpriteIds[bank], GetBankPartyData(bank), HEALTHBOX_ALL);
	StartHealthboxSlideIn(bank);
	SetHealthboxSpriteVisible(gHealthboxSpriteIds[bank]);
}

static bool8 IsBattlerAnimDone(u8 bank)
{
	return NULL_SPRITE_CALLBACK(gBattleControllerData[bank])
		&& NULL_SPRITE_CALLBACK(gBattlerSpriteIds[bank]);
}

static void SetShinyAnimOver(u8 bank)
{
	gBattleSpritesDataPtr->healthBoxesData[bank].triedShinyMonAnim = FALSE;
	gBattleSpritesDataPtr->healthBoxesData[bank].finishedShinyMonAnim = FALSE;
}

static void FreeShinySpriteGfx(void)
{
	FreeSpriteTilesByTag(ANIM_TAG_GOLD_STARS);
	FreeSpritePaletteByTag(ANIM_TAG_GOLD_STARS);
}

static void MultiIntro_FoeWaitForShinyAnimAndHealthbox(void)
{
	bool8 twoMons;
	bool8 healthboxAnimDone = FALSE;
	u8 partner = PARTNER(gActiveBattler);

	if (IS_SINGLE_BATTLE || (IS_DOUBLE_BATTLE && gBattleTypeFlags & (BATTLE_TYPE_MULTI | BATTLE_TYPE_TWO_OPPONENTS)))
	{
		if (gSprites[gHealthboxSpriteIds[gActiveBattler]].callback == SpriteCallbackDummy)
			healthboxAnimDone = TRUE;

		twoMons = FALSE;
	}
	else
	{
		if (gSprites[gHealthboxSpriteIds[gActiveBattler]].callback == SpriteCallbackDummy
		 && gSprites[gHealthboxSpriteIds[partner]].callback == SpriteCallbackDummy)
			healthboxAnimDone = TRUE;

		twoMons = TRUE;
	}

	if (IsCryPlayingOrClearCrySongs())
		healthboxAnimDone = FALSE;

	if (healthboxAnimDone)
	{
		if (twoMons)
		{
			if (gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].finishedShinyMonAnim
			&& gBattleSpritesDataPtr->healthBoxesData[partner].finishedShinyMonAnim)
			{
				SetShinyAnimOver(gActiveBattler);
				SetShinyAnimOver(partner);
				FreeShinySpriteGfx();
			}
			else
				return;
		}
		else if (gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].finishedShinyMonAnim)
		{
			if (GetBattlerPosition(gActiveBattler) == B_POSITION_OPPONENT_RIGHT)
			{
				if (!gBattleSpritesDataPtr->healthBoxesData[partner].triedShinyMonAnim
				&& !gBattleSpritesDataPtr->healthBoxesData[partner].finishedShinyMonAnim)
					FreeShinySpriteGfx();
				else
					return;
			}

			SetShinyAnimOver(gActiveBattler);
		}
		else
			return;

		gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].introEndDelay = 3;
		gBattlerControllerFuncs[gActiveBattler] = OpponentIntro_DelayAndEnd;
	}
}

static void TryStartOpponentShinyAnimation(u8 bank)
{
	if (!gBattleSpritesDataPtr->healthBoxesData[bank].triedShinyMonAnim 
	&& !gBattleSpritesDataPtr->healthBoxesData[bank].ballAnimActive 
	&& !gBattleSpritesDataPtr->healthBoxesData[bank].finishedShinyMonAnim)
		TryStartShinyAnimation(bank, GetBankPartyData(bank));
}

static void MultiIntro_FoeTryShinyAnimShowHealthbox(void)
{
	bool32 bgmRestored = FALSE;
	bool32 battlerAnimsDone = FALSE;
	u8 partner = PARTNER(gActiveBattler);

	TryStartOpponentShinyAnimation(gActiveBattler);
	if (IS_DOUBLE_BATTLE && !(gBattleTypeFlags & (BATTLE_TYPE_MULTI | BATTLE_TYPE_TWO_OPPONENTS)))
		TryStartOpponentShinyAnimation(partner);

	if (!gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].ballAnimActive && !gBattleSpritesDataPtr->healthBoxesData[partner].ballAnimActive)
	{
		if (!gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].healthboxSlideInStarted)
		{
			ShowBattlerHealthbox(gActiveBattler);
			if (IS_DOUBLE_BATTLE && !(gBattleTypeFlags & BATTLE_TYPE_MULTI))
				ShowBattlerHealthbox(partner);
		}

		gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].healthboxSlideInStarted = TRUE;
	}

	if (!gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].waitForCry
	&& gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].healthboxSlideInStarted
	&& !gBattleSpritesDataPtr->healthBoxesData[partner].waitForCry
	&& !IsCryPlayingOrClearCrySongs())
	{
		if (!gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].bgmRestored)
		{
			if (gBattleTypeFlags & BATTLE_TYPE_MULTI && gBattleTypeFlags & BATTLE_TYPE_LINK)
			{
				if (GetBattlerPosition(gActiveBattler) == B_POSITION_OPPONENT_LEFT)
					m4aMPlayContinue(&gMPlayInfo_BGM);
			}
			else
				m4aMPlayVolumeControl(&gMPlayInfo_BGM, 0xFFFF, 0x100);
		}

		gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].bgmRestored = TRUE;
		bgmRestored = TRUE;
	}

	if (IS_SINGLE_BATTLE || (IS_DOUBLE_BATTLE && gBattleTypeFlags & BATTLE_TYPE_MULTI))
	{
		if (IsBattlerAnimDone(gActiveBattler))
			battlerAnimsDone = TRUE;
	}
	else
	{
		if (IsBattlerAnimDone(gActiveBattler) && IsBattlerAnimDone(partner))
			battlerAnimsDone = TRUE;
	}

	if (bgmRestored && battlerAnimsDone)
	{
		if (IS_DOUBLE_BATTLE && !(gBattleTypeFlags & BATTLE_TYPE_MULTI))
		{
			DestroySprite(&gSprites[gBattleControllerData[partner]]);
			SetBattlerShadowSpriteCallback(partner, GetMonData(GetBankPartyData(partner), MON_DATA_SPECIES, NULL));
		}

		m4aMPlayVolumeControl(&gMPlayInfo_BGM, 0xFFFF, 256);
		DestroySprite(&gSprites[gBattleControllerData[gActiveBattler]]);
		SetBattlerShadowSpriteCallback(gActiveBattler, GetMonData(GetBankPartyData(gActiveBattler), MON_DATA_SPECIES, NULL));
		gBattleSpritesDataPtr->animationData->introAnimActive = FALSE;
		gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].bgmRestored = FALSE;
		gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].healthboxSlideInStarted = FALSE;
		gBattlerControllerFuncs[gActiveBattler] = MultiIntro_FoeWaitForShinyAnimAndHealthbox;
	}
}

void __attribute__((long_call)) OpponentStartSendOutAnim(u8 battlerId, bool8 dontClearSubstituteBit);
void Task_OpponentStartSendOutAnim(u8 taskId)
{
    u8 savedActiveBank = gActiveBattler;

    gActiveBattler = gTasks[taskId].data[0];
    if (IS_SINGLE_BATTLE || gBattleTypeFlags & BATTLE_TYPE_MULTI)
    {
        gBattleBufferA[gActiveBattler][1] = gBattlerPartyIndexes[gActiveBattler];
        OpponentStartSendOutAnim(gActiveBattler, FALSE);
    }
    else if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS)
    {
        gBattleBufferA[gActiveBattler][1] = gBattlerPartyIndexes[gActiveBattler];
        OpponentStartSendOutAnim(gActiveBattler, FALSE);
    }
    else
    {
        gBattleBufferA[gActiveBattler][1] = gBattlerPartyIndexes[gActiveBattler];
        OpponentStartSendOutAnim(gActiveBattler, FALSE);
        gActiveBattler ^= BIT_FLANK;
        gBattleBufferA[gActiveBattler][1] = gBattlerPartyIndexes[gActiveBattler];
        OpponentStartSendOutAnim(gActiveBattler, FALSE);
        gActiveBattler ^= BIT_FLANK;
    }

    gBattlerControllerFuncs[gActiveBattler] = MultiIntro_FoeTryShinyAnimShowHealthbox;
    gActiveBattler = savedActiveBank;
    DestroyTask(taskId);
}

static void PlayerIntro_WaitForShinyAnimAndHealthbox(void)
{
	bool8 healthboxAnimDone = FALSE;
	u8 partner = PARTNER(gActiveBattler);

	//Check if healthbox has finished sliding in
	if (IS_SINGLE_BATTLE || (IS_DOUBLE_BATTLE && gBattleTypeFlags & BATTLE_TYPE_MULTI))
	{
		if (gSprites[gHealthboxSpriteIds[gActiveBattler]].callback == SpriteCallbackDummy)
			healthboxAnimDone = TRUE;
	}
	else
	{
		if (gSprites[gHealthboxSpriteIds[gActiveBattler]].callback == SpriteCallbackDummy
		 && gSprites[gHealthboxSpriteIds[partner]].callback == SpriteCallbackDummy)
			healthboxAnimDone = TRUE;
	}

	//If healthbox and shiny anim are done
	if (healthboxAnimDone && gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].finishedShinyMonAnim
	&& gBattleSpritesDataPtr->healthBoxesData[partner].finishedShinyMonAnim)
	{
		//Reset shiny anim (even if it didn't occur)
		SetShinyAnimOver(gActiveBattler);
		SetShinyAnimOver(partner);
		FreeShinySpriteGfx();

		HandleLowHpMusicChange(GetBankPartyData(gActiveBattler), gActiveBattler);
		if (IS_DOUBLE_BATTLE)
			HandleLowHpMusicChange(GetBankPartyData(partner), partner);

		gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].introEndDelay = 3;
		gBattlerControllerFuncs[gActiveBattler] = PlayerIntro_DelayAndEnd;
	}
}

static void TryStartPlayerMonShinyAnim(u8 bank)
{
	if (!gBattleSpritesDataPtr->healthBoxesData[bank].triedShinyMonAnim 
	&& !gBattleSpritesDataPtr->healthBoxesData[bank].ballAnimActive)
		TryStartShinyAnimation(bank, GetBankPartyData(bank));
}

static void PlayerIntro_TryShinyAnimShowHealthbox(void)
{
	bool32 bgmRestored = FALSE;
	bool32 battlerAnimsDone = FALSE;
	u8 partner = PARTNER(gActiveBattler);
	
	TryStartPlayerMonShinyAnim(gActiveBattler); //Start shiny animation if applicable for 1st pokemon
	TryStartPlayerMonShinyAnim(partner); //Start shiny animation if applicable for 2nd pokemon

	//Show healthbox after ball anim
	if (!gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].ballAnimActive 
	&& !gBattleSpritesDataPtr->healthBoxesData[partner].ballAnimActive)
	{
		if (!gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].healthboxSlideInStarted)
		{
		
			ShowBattlerHealthbox(gActiveBattler);
			if (IS_DOUBLE_BATTLE && !(gBattleTypeFlags & BATTLE_TYPE_MULTI))
				ShowBattlerHealthbox(partner);
		}

		gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].healthboxSlideInStarted = TRUE;
	}

	//Restore BGM after cry has played and healthbox anim is started
	if (!gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].waitForCry
	&& gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].healthboxSlideInStarted
	&& !gBattleSpritesDataPtr->healthBoxesData[partner].waitForCry
	&& !IsCryPlayingOrClearCrySongs())
	{
		if (!gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].bgmRestored)
		{
			if (gBattleTypeFlags & BATTLE_TYPE_MULTI && gBattleTypeFlags & BATTLE_TYPE_LINK)
				m4aMPlayContinue(&gMPlayInfo_BGM);
			else
				m4aMPlayVolumeControl(&gMPlayInfo_BGM, 0xFFFF, 0x100);
		}

		gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].bgmRestored = TRUE;
		bgmRestored = TRUE;
	}

	//Wait for battler anims
	if (IS_SINGLE_BATTLE || (IS_DOUBLE_BATTLE && gBattleTypeFlags & BATTLE_TYPE_MULTI))
	{
		if (IsBattlerAnimDone(gActiveBattler))
			battlerAnimsDone = TRUE;
	}
	else
	{
		if (IsBattlerAnimDone(gActiveBattler) && IsBattlerAnimDone(partner))
			battlerAnimsDone = TRUE;
	}

	//Clean up
	if (bgmRestored && battlerAnimsDone)
	{
		m4aMPlayVolumeControl(&gMPlayInfo_BGM, 0xFFFF, 0x100);
		DestroySprite(&gSprites[gBattleControllerData[gActiveBattler]]);
		if (IS_DOUBLE_BATTLE && !(gBattleTypeFlags & BATTLE_TYPE_MULTI))
			DestroySprite(&gSprites[gBattleControllerData[partner]]);

		gBattleSpritesDataPtr->animationData->introAnimActive = FALSE;
		gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].bgmRestored = FALSE;
		gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].healthboxSlideInStarted = FALSE;
		gBattlerControllerFuncs[gActiveBattler] = PlayerIntro_WaitForShinyAnimAndHealthbox;
	}
}

void __attribute__((long_call)) PlayerStartSendOutAnim(u8 battlerId, bool8 dontClearSubstituteBit);
void Task_PlayerStartSendOutAnim(u8 taskId)
{
	if (gTasks[taskId].data[1] < 31)
	{
		++gTasks[taskId].data[1];
	}
	else
	{
		u8 savedActiveBattler = gActiveBattler;
		gActiveBattler = gTasks[taskId].data[0];

		if (IsTagBattle() && GetBattlerPosition(gActiveBattler) == B_POSITION_PLAYER_RIGHT) //AI Partner
		{
			//Let the player's controller do the send in for both mons
			PlayerBufferExecCompleted();
		}
		else
		{
			if (IS_SINGLE_BATTLE || (gBattleTypeFlags & BATTLE_TYPE_MULTI))
			{
				gBattleBufferA[gActiveBattler][1] = gBattlerPartyIndexes[gActiveBattler];
				PlayerStartSendOutAnim(gActiveBattler, FALSE);
			}
			else
			{
				gBattleBufferA[gActiveBattler][1] = gBattlerPartyIndexes[gActiveBattler];
				PlayerStartSendOutAnim(gActiveBattler, FALSE);
				gActiveBattler ^= BIT_FLANK;
				gBattleBufferA[gActiveBattler][1] = gBattlerPartyIndexes[gActiveBattler];
				BattleLoadPlayerMonSpriteGfx(GetBankPartyData(gActiveBattler), gActiveBattler);
				PlayerStartSendOutAnim(gActiveBattler, FALSE);
				gActiveBattler ^= BIT_FLANK;
			}

			gBattlerControllerFuncs[gActiveBattler] = PlayerIntro_TryShinyAnimShowHealthbox;
		}

		gActiveBattler = savedActiveBattler;
		DestroyTask(taskId);
	}
}

u8* GetTrainerBLoseText(void)
{
	return sTrainerDefeatSpeech_B; //ExtensionState.trainerBDefeatSpeech
}

u32 MultiMoneyCalc(void)
{
	u32 money = CalcPrizeiMoneyForTrainer(gTrainerBattleOpponent_A);

	if (gBattleTypeFlags & (BATTLE_TYPE_MULTI | BATTLE_TYPE_TWO_OPPONENTS))
		money += CalcPrizeiMoneyForTrainer(VarGet(VAR_SECOND_OPPONENT));

	return money;
}

#define gTrainerMoneyTable ((struct TrainerMoney*) *((u32*) 0x80259CC))
static u32 CalcPrizeiMoneyForTrainer(u16 trainerId)
{
	u8 i, firstMonId, lastMonId, level, rate;
	struct Trainer* trainer = &gTrainers[trainerId];

	//Get the party range to search through for the given trainer
	if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS)
	{
		if (gTrainerBattleOpponent_A == trainerId)
		{
			firstMonId = 0;
			lastMonId = 3;
		}
		else
		{
			firstMonId = 3;
			lastMonId = 6;
		}
	}
	else
	{
		firstMonId = 0;
		lastMonId = 6;
	}

	//Scan through the party backwards until the last mon
	for (i = lastMonId - 1, level = 0; i >= firstMonId; --i)
	{
		u16 species = GetMonData(&gEnemyParty[i], MON_DATA_SPECIES2, NULL);
		if (species != SPECIES_NONE && species != SPECIES_EGG)
		{
			level = GetMonData(&gEnemyParty[i], MON_DATA_LEVEL, NULL); //The level of the last mon
			break;
		}
	}

	//Get the money rate
	for (i = 0, rate = 0; i < NUM_TRAINER_CLASSES; ++i)
	{
		if (gTrainerMoneyTable[i].trainerClass == trainer->trainerClass)
		{
			rate = gTrainerMoneyTable[i].money;
			break;
		}

		if (gTrainerMoneyTable[i].trainerClass == 0xFF)
		{
			rate = gTrainerMoneyTable[0].money;
			break;
		}
	}

	//Final calc
	u32 money = (rate * 4) * level * gBattleStruct->moneyMultiplier;

	if (IS_DOUBLE_BATTLE && !(gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS))
		money *= 2;

	return money;
}

bool8 IsLinkDoubleBattle(void)
{
	u32 flags = (BATTLE_TYPE_MULTI | BATTLE_TYPE_TRAINER | BATTLE_TYPE_LINK | BATTLE_TYPE_DOUBLE);

	if ((gBattleTypeFlags & flags) == flags)
		return TRUE;
	else
		return FALSE;
}

bool8 IsMultiBattle(void)
{
	u32 flags = (BATTLE_TYPE_MULTI | BATTLE_TYPE_INGAME_PARTNER | BATTLE_TYPE_TRAINER | BATTLE_TYPE_DOUBLE);
	u32 flags2 = (BATTLE_TYPE_MULTI | BATTLE_TYPE_TRAINER | BATTLE_TYPE_DOUBLE);
	u32 flags3 = (BATTLE_TYPE_INGAME_PARTNER | BATTLE_TYPE_TRAINER | BATTLE_TYPE_DOUBLE);
	u32 flags4 = (BATTLE_TYPE_INGAME_PARTNER | BATTLE_TYPE_DOUBLE);

	if (((gBattleTypeFlags & flags) == flags
	||   (gBattleTypeFlags & flags) == flags2
	||   (gBattleTypeFlags & flags) == flags3
	||   (gBattleTypeFlags & flags) == flags4) && gMain.inBattle)
		return TRUE;
	else
		return FALSE;
}

bool8 IsTwoOpponentBattle(void)
{
	if ((gBattleTypeFlags & (BATTLE_TYPE_DOUBLE | BATTLE_TYPE_TWO_OPPONENTS)) == (BATTLE_TYPE_DOUBLE | BATTLE_TYPE_TWO_OPPONENTS))
		return TRUE;

	return FALSE;
}

bool8 BankSideHasTwoTrainers(u8 bank)
{
	u8 side = SIDE(bank);

	return ((side == B_SIDE_OPPONENT && IsTwoOpponentBattle())
		 || (side == B_SIDE_PLAYER && IsTagBattle()));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool8 IsTagBattle(void)
{
	u32 flags = (BATTLE_TYPE_DOUBLE | BATTLE_TYPE_INGAME_PARTNER);

	if ((gBattleTypeFlags & flags) == flags)
		return TRUE;
	else
		return FALSE;
}

bool8 IsPartnerAttacker(void)
{
	return gBankAttacker == B_POSITION_PLAYER_RIGHT;
}

u8* PartnerLoadName(void)
{
	CopyFrontierTrainerName(gStringVar4, VarGet(VAR_PARTNER), 2);
	return gStringVar4;
}

u8 LoadPartnerBackspriteIndex(void)
{
	return VarGet(VAR_PARTNER_BACKSPRITE);
}

void ChooseProperPartnerController(void)
{
	gBattlerPositions[0] = B_POSITION_PLAYER_LEFT;
	gBattlerPositions[1] = B_POSITION_OPPONENT_LEFT;
	gBattlerPositions[3] = B_POSITION_OPPONENT_RIGHT;
	gBattlersCount = 4;

	if (IsRaidBattle())
	{
		gBattlersCount = 3;
		gBattlerPositions[3] = 0xFF;
	}

	gBattlerControllerFuncs[0] = SetControllerToPlayer;
	gBattlerControllerFuncs[1] = SetControllerToOpponent;
	gBattlerControllerFuncs[3] = SetControllerToOpponent;

	if (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER)
	{
		gBattlerControllerFuncs[2] = SetControllerToPlayerPartner;
		gBattlerPositions[2] = B_POSITION_PLAYER_RIGHT; //The game crashes when this line is not here for some reason
	}
	else
	{
		gBattlerControllerFuncs[2] = SetControllerToPlayer;
		gBattlerPositions[2] = B_POSITION_PLAYER_RIGHT; //The game crashes when this line is not here for some reason
	}
}

void SetControllerToPlayerPartner(void)
{
	gBattlerControllerFuncs[gActiveBattler] = PlayerPartnerBufferRunCommand;
}

static void PlayerPartnerBufferExecComplete(void)
{
	gBattlerControllerFuncs[gActiveBattler] = PlayerPartnerBufferRunCommand;
	if (gBattleTypeFlags & BATTLE_TYPE_LINK)
	{
		u8 playerId = GetMultiplayerId();

		PrepareBufferDataTransferLink(2, 4, &playerId);
		gBattleBufferA[gActiveBattler][0] = CONTROLLER_TERMINATOR_NOP;
	}
	else
	{
		gBattleExecBuffer &= ~gBitTable[gActiveBattler];
	}
}

static void PlayerPartnerBufferRunCommand(void)
{
	if (gBattleExecBuffer & gBitTable[gActiveBattler])
	{
		u8 buffer = gBattleBufferA[gActiveBattler][0];

		if (buffer > COMMAND_MAX)
			PlayerPartnerBufferExecComplete();
		else
			sPlayerPartnerBufferCommands[buffer]();
	}
}

static void PlayerPartnerHandleChooseMove(void)
{
	u8 chosenMovePos, moveTarget;
	u16 chosenMove;
	struct ChooseMoveStruct* moveInfo = (struct ChooseMoveStruct*)(&gBattleBufferA[gActiveBattler][4]);

	BattleAI_SetupAIData(0xF);
	chosenMovePos = BattleAI_ChooseMoveOrAction();
	chosenMove = moveInfo->moves[chosenMovePos];
	moveTarget = GetBaseMoveTarget(chosenMove, gActiveBattler);

	if (moveTarget & MOVE_TARGET_USER)
	{
		gBankTarget = gActiveBattler;
	}
	else if (moveTarget & MOVE_TARGET_USER_OR_PARTNER)
	{
		if (SIDE(gBankTarget) != SIDE(gActiveBattler))
			gBankTarget = gActiveBattler;
	}
	else if (moveTarget & MOVE_TARGET_BOTH)
	{
		gBankTarget = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
		if (gAbsentBattlerFlags & gBitTable[gBankTarget])
			gBankTarget = GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT);
	}

	//You get 1 of 3 of the following gimmicks per Pokemon
	if (moveInfo->possibleZMoves[chosenMovePos]) //Checked first b/c Rayquaza can do all 3
	{
		if (ShouldAIUseZMoveByMoveAndMovePos(gActiveBattler, gBankTarget, moveInfo->moves[chosenMovePos], chosenMovePos))
			gNewBS->zMoveData.toBeUsed[gActiveBattler] = TRUE;
	}
	else if (moveInfo->canMegaEvolve)
	{
		if (!ShouldAIDelayMegaEvolution(gActiveBattler, gBankTarget, chosenMove, FALSE, TRUE))
		{
			if (moveInfo->megaVariance != MEGA_VARIANT_ULTRA_BURST)
				gNewBS->megaData.chosen[gActiveBattler] = TRUE;
			else if (moveInfo->megaVariance == MEGA_VARIANT_ULTRA_BURST)
				gNewBS->ultraData.chosen[gActiveBattler] = TRUE;
		}
	}
	else if (moveInfo->possibleMaxMoves[chosenMovePos])
	{
		if (ShouldAIDynamax(gActiveBattler))
			gNewBS->dynamaxData.toBeUsed[gActiveBattler] = TRUE;
	}

	//This is handled again later, but it's only here to help with the case of choosing Helping Hand when the partner is switching out.
	gBattleStruct->chosenMovePositions[gActiveBattler] = chosenMovePos;
	gBattleStruct->moveTarget[gActiveBattler] = gBankTarget;
	gChosenMovesByBanks[gActiveBattler] = chosenMove;

	if (IsMockBattle())
		TryRemovePartnerDoublesKillingScoreComplete(gActiveBattler, gBankTarget, chosenMove, moveTarget, FALSE); //Moves are chosen in order of bank

	EmitMoveChosen(1, chosenMovePos, gBankTarget, gNewBS->megaData.chosen[gActiveBattler], gNewBS->ultraData.chosen[gActiveBattler], gNewBS->zMoveData.toBeUsed[gActiveBattler], FALSE);
	PlayerPartnerBufferExecComplete();
}

static void PlayerPartnerHandlePrintSelectionString(void)
{
	u16* stringId = (u16 *)(&gBattleBufferA[gActiveBattler][2]);

	if (IsMockBattle() && *stringId == STRINGID_ITEMSCANTBEUSEDNOW) //Important to display this string to the player
		PlayerHandlePrintSelectionString();
	else
		PlayerPartnerBufferExecComplete();
}

void __attribute__((long_call)) PlayerHandleChoosePokemon(void);
static void PlayerPartnerHandleChooseAction(void)
{
	if (IsMockBattle())
	{
		PlayerHandleChooseAction(); //Allow the player to choose to Fight, Switch, etc., but the actual choice will be made by the AI
		return;
	}

	u8 partner = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
	u16 itemId = gBattleBufferA[gActiveBattler][2] | (gBattleBufferA[gActiveBattler][3] << 8);

	if (RAID_BATTLE_END) //mon 2 doesn't get to do anything.
	{
		if ((gChosenActionByBank[partner] == ACTION_USE_ITEM && GetPocketByItemId(itemId) == POCKET_POKEBALLS)
		|| gChosenActionByBank[partner] == ACTION_RUN)
		{
			gNewBS->NoMoreMovingThisTurn |= gBitTable[gActiveBattler];
			EmitTwoReturnValues(1, ACTION_USE_MOVE, 0);
			PlayerPartnerBufferExecComplete();
			return;
		}
		else
		{
			EmitTwoReturnValues(1, ACTION_CANCEL_PARTNER, 0);
			PlayerPartnerBufferExecComplete();
			return;
		}
	}
	else if (gChosenActionByBank[partner] == ACTION_RUN
	|| (!IsBagDisabled()
	 && gChosenActionByBank[partner] == ACTION_USE_ITEM
	 && GetPocketByItemId(itemId) == POCKET_POKEBALLS)) //mon 2 doesn't get to do anything.
	{
		gNewBS->NoMoreMovingThisTurn |= gBitTable[gActiveBattler];

		EmitTwoReturnValues(1, ACTION_USE_MOVE, 0);
		PlayerPartnerBufferExecComplete();
		return;
	}

	AI_TrySwitchOrUseItem();
	PlayerPartnerBufferExecComplete();
}

static void PlayerPartnerHandleChoosePokemon(void)
{
	u8 chosenMonId;

	if (IsMockBattle())
	{
		PlayerHandleChoosePokemon(); //Allow the player to choose the Pokemon to switch to
		return;
	}

	if (gBattleStruct->switchoutIndex[SIDE(gActiveBattler)] == PARTY_SIZE)
	{
		u8 battlerIn1, battlerIn2, firstId, lastId;
		struct Pokemon* party = LoadPartyRange(gActiveBattler, &firstId, &lastId);

		if (IS_DOUBLE_BATTLE)
		{
			battlerIn1 = gActiveBattler; //The dead mon
			if (gAbsentBattlerFlags & gBitTable[PARTNER(gActiveBattler)])
				battlerIn2 = gActiveBattler;
			else
				battlerIn2 = PARTNER(battlerIn1);
		}
		else
		{
			battlerIn1 = gActiveBattler;
			battlerIn2 = gActiveBattler;
		}
	
		if (gNewBS->ai.bestMonIdToSwitchInto[gActiveBattler][0] == PARTY_SIZE
		|| GetMonData(&party[gNewBS->ai.bestMonIdToSwitchInto[gActiveBattler][0]], MON_DATA_HP, NULL) == 0 //Best mon is dead
		|| gNewBS->ai.bestMonIdToSwitchInto[gActiveBattler][0] == gBattlerPartyIndexes[battlerIn1]
		|| gNewBS->ai.bestMonIdToSwitchInto[gActiveBattler][0] == gBattlerPartyIndexes[battlerIn2]) //The best mon is already in
			CalcMostSuitableMonToSwitchInto();

		chosenMonId = GetMostSuitableMonToSwitchInto();

		if (chosenMonId == PARTY_SIZE)
		{
			for (chosenMonId = firstId; chosenMonId < lastId; ++chosenMonId)
			{
				if (party[chosenMonId].species != SPECIES_NONE
				&& party[chosenMonId].hp != 0
				&& !GetMonData(&party[chosenMonId], MON_DATA_IS_EGG, 0)
				&& chosenMonId != gBattlerPartyIndexes[battlerIn1]
				&& chosenMonId != gBattlerPartyIndexes[battlerIn2])
					break;
			}
		}
	}
	else
	{
		chosenMonId = gBattleStruct->switchoutIndex[SIDE(gActiveBattler)];
		gBattleStruct->switchoutIndex[SIDE(gActiveBattler)] = PARTY_SIZE;
	}

	RemoveBestMonToSwitchInto(gActiveBattler);

	/* Perform switchout */
	gBattleStruct->monToSwitchIntoId[gActiveBattler] = chosenMonId;
	EmitChosenMonReturnValue(1, chosenMonId, 0);
	PlayerPartnerBufferExecComplete();
}

#define WRAP_COMMAND_ADDRESS(x) ((void (*)(void)) (x))

/*
 * Most of these are just reused from the player's command table to
 * save rewriting them all.
 */
void (*const sPlayerPartnerBufferCommands[COMMAND_MAX])(void) =
{
	WRAP_COMMAND_ADDRESS(0x8030B91), /* 00 */
	WRAP_COMMAND_ADDRESS(0x80313B1), /* 01 */
	WRAP_COMMAND_ADDRESS(0x8031439), /* 02 */
	WRAP_COMMAND_ADDRESS(0x8031E8D), /* 03 */
	WRAP_COMMAND_ADDRESS(0x8031F01), /* 04 */
	WRAP_COMMAND_ADDRESS(0x8031F69), /* 05 */
	WRAP_COMMAND_ADDRESS(0x8032161), /* 06 */
	WRAP_COMMAND_ADDRESS(0x803227D), /* 07 */
	WRAP_COMMAND_ADDRESS(0x8032429), /* 08 */
	WRAP_COMMAND_ADDRESS(0x8032591), /* 09 */
	WRAP_COMMAND_ADDRESS(0x8032651), /* 0a */
	WRAP_COMMAND_ADDRESS(0x803273D), /* 0b */
	WRAP_COMMAND_ADDRESS(0x803275D), /* 0c */
	WRAP_COMMAND_ADDRESS(0x80327B1), /* 0d */
	WRAP_COMMAND_ADDRESS(0x8032811), /* 0e */
	WRAP_COMMAND_ADDRESS(0x8032841), /* 0f */
	WRAP_COMMAND_ADDRESS(0x8032AFD), /* 10 */
	PlayerPartnerHandlePrintSelectionString,	/* 11 */
	PlayerPartnerHandleChooseAction,	 		/* 12 */
	WRAP_COMMAND_ADDRESS(0x8032C49), 			/* 13 */
	PlayerPartnerHandleChooseMove,		 		/* 14 */
	WRAP_COMMAND_ADDRESS(0x8032CED), 			/* 15 */
	PlayerPartnerHandleChoosePokemon,		   /* 16 */
	WRAP_COMMAND_ADDRESS(0x8032E29), /* 17 */
	WRAP_COMMAND_ADDRESS(0x8032E4D), /* 18 */
	WRAP_COMMAND_ADDRESS(0x8032F4D), /* 19 */
	WRAP_COMMAND_ADDRESS(0x8032FE9), /* 1a */
	WRAP_COMMAND_ADDRESS(0x8033061), /* 1b */
	WRAP_COMMAND_ADDRESS(0x80330C9), /* 1c */
	WRAP_COMMAND_ADDRESS(0x8033135), /* 1d */
	WRAP_COMMAND_ADDRESS(0x8033141), /* 1e */
	WRAP_COMMAND_ADDRESS(0x80331F5), /* 1f */
	WRAP_COMMAND_ADDRESS(0x8033225), /* 20 */
	WRAP_COMMAND_ADDRESS(0x8033231), /* 21 */
	WRAP_COMMAND_ADDRESS(0x8033245), /* 22 */
	WRAP_COMMAND_ADDRESS(0x8033259), /* 23 */
	WRAP_COMMAND_ADDRESS(0x803326D), /* 24 */
	WRAP_COMMAND_ADDRESS(0x8033281), /* 25 */
	WRAP_COMMAND_ADDRESS(0x803329D), /* 26 */
	WRAP_COMMAND_ADDRESS(0x80332D5), /* 27 */
	WRAP_COMMAND_ADDRESS(0x80332ED), /* 28 */
	WRAP_COMMAND_ADDRESS(0x8033315), /* 29 */
	WRAP_COMMAND_ADDRESS(0x8033385), /* 2a */
	WRAP_COMMAND_ADDRESS(0x8033391), /* 2b */
	WRAP_COMMAND_ADDRESS(0x80333D5), /* 2c */
	WRAP_COMMAND_ADDRESS(0x8033405), /* 2d */
	WRAP_COMMAND_ADDRESS(0x8033445), /* 2e */
	WRAP_COMMAND_ADDRESS(0x8033479), /* 2f */
	WRAP_COMMAND_ADDRESS(0x803376D), /* 30 */
	WRAP_COMMAND_ADDRESS(0x8033879), /* 31 */
	WRAP_COMMAND_ADDRESS(0x80338C9), /* 32 */
	WRAP_COMMAND_ADDRESS(0x80338ED), /* 33 */
	WRAP_COMMAND_ADDRESS(0x803394D), /* 34 */
	WRAP_COMMAND_ADDRESS(0x80339B5), /* 35 */
	WRAP_COMMAND_ADDRESS(0x8033A11), /* 36 */
	WRAP_COMMAND_ADDRESS(0x8033A79), /* 37 */
	WRAP_COMMAND_ADDRESS(0x8033AC5), /* 38 */
};
