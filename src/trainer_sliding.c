#include "defines.h"
#include "defines_battle.h"

#include "../include/new/battle_util.h"
#include "../include/new/multi.h"
#include "../include/new/trainer_sliding.h"
#include "../include/new/trainer_sliding_data.h"
/*
trainer_sliding.c
	handles mid-battle trainer sliding and related message.
*/

struct TrainerSlide
{
	u16 trainerId;
	const u8* msgFirstDown;
	const u8* msgLastSwitchIn;
	const u8* msgLastLowHp;
};

struct DynamaxTrainerSlide
{
	u16 trainerId;
	const u8* dynamaxMsg;
};

static const struct TrainerSlide sTrainerSlides[] =
{
	{},

	#ifdef UNBOUND //For Pokemon Unbound - Feel free to remove
	{0x6, sText_MirskleFirstMonDown, NULL, NULL},
	{0x2CF, sText_MirskleFirstMonDown, NULL, NULL},
	{0x2D0, sText_MirskleFirstMonDown, NULL, NULL},
	{0x2D1, sText_MirskleFirstMonDown, NULL, NULL},
	{0x19F, NULL, sText_VegaLastSwitchIn, NULL},
	{0x2D2, NULL, sText_VegaLastSwitchIn, NULL},
	{0x2D3, NULL, sText_VegaLastSwitchIn, NULL},
	{0x2D4, NULL, sText_VegaLastSwitchIn, NULL},
	{0x14, NULL, sText_AliceLastSwitchIn, NULL},
	{0x2D5, NULL, sText_AliceLastSwitchIn, NULL},
	{0x2D6, NULL, sText_AliceLastSwitchIn, NULL},
	{0x2D7, NULL, sText_AliceLastSwitchIn, NULL},
	{0x18, NULL, sText_MelLastSwitchIn, NULL},
	{0x2D8, NULL, sText_MelLastSwitchIn, NULL},
	{0x2D9, NULL, sText_MelLastSwitchIn, NULL},
	{0x2DA, NULL, sText_MelLastSwitchIn, NULL},
	{0x1A2, NULL, sText_GalavanLastSwitchIn, NULL},
	{0x2DB, NULL, sText_GalavanLastSwitchIn, NULL},
	{0x2DC, NULL, sText_GalavanLastSwitchIn, NULL},
	{0x2DD, NULL, sText_GalavanLastSwitchIn, NULL},
	{0x1A1, NULL, sText_TessyLastSwitchIn, NULL},
	{0x2DE, NULL, sText_TessyLastSwitchIn, NULL},
	{0x2DF, NULL, sText_TessyLastSwitchIn, NULL},
	{0x2E0, NULL, sText_TessyLastSwitchIn, NULL},

	{0x15C, NULL, NULL, sText_Zeph1LowHP},
	{0x2AB, NULL, NULL, sText_Zeph1LowHP},
	{0x2AC, NULL, NULL, sText_Zeph1LowHP},
	{0x2AD, NULL, NULL, sText_Zeph1LowHP},
	{0x15E, NULL, NULL, sText_Zeph2LowHP},
	{0x2B7, NULL, NULL, sText_Zeph2LowHP},
	{0x2B8, NULL, NULL, sText_Zeph2LowHP},
	{0x2B9, NULL, NULL, sText_Zeph2LowHP},

	{0x19, NULL, sText_MaximaLastSwitchIn, NULL},
	{0x2BD, NULL, sText_MaximaLastSwitchIn, NULL},
	{0x2BE, NULL, sText_MaximaLastSwitchIn, NULL},
	{0x2BF, NULL, sText_MaximaLastSwitchIn, NULL},
	#endif
};

static const struct DynamaxTrainerSlide sDynamaxTrainerSlides[] =
{
	{0x17, gText_TestTrainerDynamaxMsg}, //Test data
};

//This file's functions:
static bool8 IsBankHpLow(u8 bank);
static u8 GetEnemyMonCount(bool8 onlyAlive);

void atk53_trainerslidein(void)
{
	gActiveBattler = GetBattlerAtPosition(gBattlescriptCurrInstr[1]);
	EmitTrainerSlide(0);
	MarkBufferBankForExecution(gActiveBattler);
	gBattlescriptCurrInstr += 2;
}

void TrainerSlideInScriptingBank(void)
{
	gActiveBattler = gBattleScripting.bank;
	EmitTrainerSlide(0);
	MarkBufferBankForExecution(gActiveBattler);
}

void TrainerSlideOutScriptingBank(void)
{
	gActiveBattler = gBattleScripting.bank;
	EmitTrainerSlideBack(0);
	MarkBufferBankForExecution(gActiveBattler);
}

//The modifications made to this function don't affect
//sliding in anyway. They allow expanded Battle Backgrounds.
void HandleIntroSlide(u8 terrain)
{
	u8 taskId;

	for (int bank = 0; bank < gBattlersCount; ++bank)
	{
		if (GetMonAbility(GetBankPartyData(bank)) == ABILITY_ILLUSION)
			gStatuses3[bank] |= STATUS3_ILLUSION;
	}

	if (gBattleTypeFlags & BATTLE_TYPE_LINK)
	{
		taskId = CreateTask(BattleIntroSlideLink, 0);
	}
	else if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER)
	{
		taskId = CreateTask(BattleIntroSlide3, 0);
	}
	else if ((gBattleTypeFlags & BATTLE_TYPE_KYOGRE_GROUDON) && gGameVersion != VERSION_RUBY) //Why?
	{
		terrain = BATTLE_TERRAIN_UNDERWATER;
		taskId = CreateTask(BattleIntroSlide2, 0);
	}
	else if (terrain > 0x13) //Terrain Champion
	{
		taskId = CreateTask(BattleIntroSlide3, 0);
	}
	else
	{
		taskId = CreateTask(sBattleIntroSlideFuncs[terrain], 0);
	}

	gTasks[taskId].data[0] = 0;
	gTasks[taskId].data[1] = terrain;
	gTasks[taskId].data[2] = 0;
	gTasks[taskId].data[3] = 0;
	gTasks[taskId].data[4] = 0;
	gTasks[taskId].data[5] = 0;
	gTasks[taskId].data[6] = 0;
}

static u8 GetEnemyMonCount(bool8 onlyAlive)
{
	u8 i, count = 0;

	for (i = 0; i < PARTY_SIZE; i++)
	{
		u32 species = GetMonData(&gEnemyParty[i], MON_DATA_SPECIES2, NULL);
		if (species != SPECIES_NONE
		&&  species != SPECIES_EGG
		&& (!onlyAlive || gEnemyParty[i].hp))
			++count;
	}

	return count;
}

static bool8 IsBankHpLow(u8 bank)
{
	return udivsi((gBattleMons[bank].hp * 100), gBattleMons[bank].maxHP) < 25;
}

bool8 ShouldDoTrainerSlide(u8 bank, u16 trainerId, u8 caseId)
{
	u32 i;

	if (!(gBattleTypeFlags & BATTLE_TYPE_TRAINER) || SIDE(bank) != B_SIDE_OPPONENT)
		return FALSE;

	for (i = 0; i < ARRAY_COUNT(sTrainerSlides); ++i)
	{
		if (trainerId == sTrainerSlides[i].trainerId)
		{
			gBattleScripting.bank = bank;
			switch (caseId) {
				case TRAINER_SLIDE_LAST_SWITCHIN:
					if (sTrainerSlides[i].msgLastSwitchIn != NULL && GetEnemyMonCount(TRUE) == 1)
					{
						gBattleStringLoader = sTrainerSlides[i].msgLastSwitchIn;
						return TRUE;
					}
					break;

				case TRAINER_SLIDE_LAST_LOW_HP:
					if (sTrainerSlides[i].msgLastLowHp != NULL
					&& GetEnemyMonCount(TRUE) == 1
					&& IsBankHpLow(bank)
					&& !gNewBS->trainerSlideLowHpMsgDone)
					{
						gNewBS->trainerSlideLowHpMsgDone = TRUE;
						gBattleStringLoader = sTrainerSlides[i].msgLastLowHp;
						return TRUE;
					}
					break;

				case TRAINER_SLIDE_FIRST_DOWN:
					if (sTrainerSlides[i].msgFirstDown != NULL && GetEnemyMonCount(TRUE) == GetEnemyMonCount(FALSE) - 1)
					{
						gBattleStringLoader = sTrainerSlides[i].msgFirstDown;
						return TRUE;
					}
					break;
			}
			break;
		}
	}

	return FALSE;
}

void TryDoDynamaxTrainerSlide(void)
{
	u32 i;
	u16 trainerId;

	if (!(gBattleTypeFlags & BATTLE_TYPE_TRAINER) || SIDE(gBattleScripting.bank) != B_SIDE_OPPONENT)
		return;

	if (IsTwoOpponentBattle() && GetBattlerPosition(gBattleScripting.bank) == B_POSITION_OPPONENT_RIGHT)
		trainerId = gTrainerBattleOpponent_B;
	else
		trainerId = gTrainerBattleOpponent_A;

	gBattleStringLoader = gText_DefaultTrainerDynamaxMsg;
	for (i = 0; i < ARRAY_COUNT(sDynamaxTrainerSlides); ++i)
	{
		if (trainerId == sDynamaxTrainerSlides[i].trainerId)
			gBattleStringLoader = sDynamaxTrainerSlides[i].dynamaxMsg;
	}

	BattleScriptPush(gBattlescriptCurrInstr + 5); //After callasm
	gBattlescriptCurrInstr = BattleScript_TrainerSlideMsgRet - 5;
}

//Hook in Battle Main
void CheckLastMonLowHPSlide(void)
{
	if (ShouldDoTrainerSlide(GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT), gTrainerBattleOpponent_A, TRAINER_SLIDE_LAST_LOW_HP)
	|| (IsTwoOpponentBattle() && ShouldDoTrainerSlide(GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT), gTrainerBattleOpponent_B, TRAINER_SLIDE_LAST_LOW_HP))
	|| (IS_DOUBLE_BATTLE && ShouldDoTrainerSlide(GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT), gTrainerBattleOpponent_A, TRAINER_SLIDE_LAST_LOW_HP)))
		BattleScriptExecute(BattleScript_TrainerSlideMsgEnd2);
}

//handletrainerslidemsg BANK CASE
void atkFF1C_handletrainerslidemsg(void)
{
	gActiveBattler = GetBankForBattleScript(gBattlescriptCurrInstr[1]);

	if (IS_DOUBLE_BATTLE)
		gActiveBattler &= BIT_SIDE; //Always mon on enemy left in doubles

	u8 caseId = gBattlescriptCurrInstr[2];

	switch(caseId) {
		case 0:
			gNewBS->savedObjId = gBattlerSpriteIds[gActiveBattler];
			break;

		case 1:
			gBattlerSpriteIds[gActiveBattler] = gNewBS->savedObjId;
			if (BATTLER_ALIVE(gActiveBattler))
				BattleLoadOpponentMonSpriteGfx(GetBankPartyData(gActiveBattler), gActiveBattler);
	}

	gBattlescriptCurrInstr += 3;
}

//trytrainerslidefirstdownmsg BANK
void atkFF1D_trytrainerslidefirstdownmsg(void)
{
	u8 pos, shouldDo;

	gActiveBattler = GetBankForBattleScript(gBattlescriptCurrInstr[1]);
	pos = GetBattlerPosition(gActiveBattler);

	if (IsTwoOpponentBattle() && pos == B_POSITION_OPPONENT_RIGHT)
		shouldDo = ShouldDoTrainerSlide(gActiveBattler, gTrainerBattleOpponent_B, TRAINER_SLIDE_FIRST_DOWN);
	else
		shouldDo = ShouldDoTrainerSlide(gActiveBattler, gTrainerBattleOpponent_A, TRAINER_SLIDE_FIRST_DOWN);

	if (shouldDo)
	{
		BattleScriptPush(gBattlescriptCurrInstr + 2);
		gBattlescriptCurrInstr = BattleScript_TrainerSlideMsgRet;
		return;
	}

	gBattlescriptCurrInstr += 2;
}

void atkFF1E_trainerslideout(void)
{
	gActiveBattler = GetBattlerAtPosition(gBattlescriptCurrInstr[1]);
	EmitTrainerSlideBack(0);
	MarkBufferBankForExecution(gActiveBattler);
	gBattlescriptCurrInstr += 2;
}
