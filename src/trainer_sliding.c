#include "defines.h"
#include "defines_battle.h"

#include "../include/new/battle_util.h"
#include "../include/new/trainer_sliding.h"
#include "../include/new/trainer_sliding_data.h"

struct TrainerSlide
{
	u16 trainerId;
	u8* msgFirstDown;
	u8* msgLastSwitchIn;
	u8* msgLastLowHp;
};

static const struct TrainerSlide sTrainerSlides[] =
{
	{0x17, sText_VegaFirstMonDown, sText_VegaLastSwitchIn, sText_VegaLastLowHP},
	{0x19F, sText_VegaFirstMonDown, sText_VegaLastSwitchIn, sText_VegaLastLowHP},
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

//The modifications made to this function don't affect
//sliding in anyway. They allow expanded Battle Backgrounds.
void HandleIntroSlide(u8 terrain)
{
	u8 taskId;

	for (int bank = 0; bank < gBattlersCount; ++bank)
	{
		if (GetPartyAbility(GetBankPartyData(bank)) == ABILITY_ILLUSION)
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
			gBattleScripting->bank = bank;
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

//Hook in Battle Miain
void CheckLastMonLowHPSlide(void)
{
	if (ShouldDoTrainerSlide(GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT), gTrainerBattleOpponent_A, TRAINER_SLIDE_LAST_LOW_HP))
		BattleScriptExecute(BattleScript_TrainerSlideMsgEnd2);
}

//handletrainerslidemsg BANK CASE
void atkFF1C_handletrainerslidemsg(void)
{
	gActiveBattler = GetBattleBank(gBattlescriptCurrInstr[1]);
	u8 caseId = gBattlescriptCurrInstr[2];

	switch(caseId) {
		case 0:
			gNewBS->savedObjId = gBattlerSpriteIds[gActiveBattler];
			break;

		case 1:
			gBattlerSpriteIds[gActiveBattler] = gNewBS->savedObjId;
			if (gBattleMons[gActiveBattler].hp)
				BattleLoadOpponentMonSpriteGfx(GetBankPartyData(gActiveBattler), gActiveBattler);
	}

	gBattlescriptCurrInstr += 3;
}

//trytrainerslidefirstdownmsg BANK
void atkFF1D_trytrainerslidefirstdownmsg(void)
{
	gActiveBattler = GetBattleBank(gBattlescriptCurrInstr[1]);

	if (ShouldDoTrainerSlide(gActiveBattler, gTrainerBattleOpponent_A, TRAINER_SLIDE_FIRST_DOWN))
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
