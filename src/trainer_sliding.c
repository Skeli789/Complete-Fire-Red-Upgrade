#include "defines.h"
#include "helper_functions.h"

extern const TaskFunc sBattleIntroSlideFuncs[];

void atk53_trainerslidein(void) {
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
