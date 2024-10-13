/********\

CUSTOM FILE!
THIS IS A CUSTOM FILE THAT ISN'T NORMALLY IN THE CFRU.

Give Credits to:
- Acimut
if used.

\********/

#include "../include/global.h"
#include "../include/sound.h"
#include "../include/new/Vanilla_functions.h"
#include "../include/main.h"
#include "../include/script.h"
#include "../include/sprite.h"
#include "../include/event_data.h"
#include "../include/constants/flags.h"

extern void CB2_ShowEvIv(void);

/**
 * Punto de entrada
 * Inicio de la rutina
 * Función inicial
 * main function
 * como quieran llamarlo.
*/
void Call_EvIv(void)
{
    PlaySE(244);
    QuestLog_CutRecording(); 
    SetVBlankCallback(NULL);
    SetMainCallback2(CB2_ShowEvIv);
    ScriptContext2_Enable();
}
