#include "defines.h"
#include "helper_functions.h"

void atk53_trainerslidein(void) {
	gActiveBattler = GetBattlerAtPosition(gBattlescriptCurrInstr[1]);
    EmitTrainerSlide(0);
    MarkBufferBankForExecution(gActiveBattler);
    gBattlescriptCurrInstr += 2;
}