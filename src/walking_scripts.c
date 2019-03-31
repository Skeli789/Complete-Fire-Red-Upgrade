#include "defines.h"
#include "helper_functions.h"

u8* gDefaultWalkingScripts[] =
{
	(u32) 0,
	(u32) 0,
	(u32) 0,
	(u32) 0,
};

// hack safari step function to include custom walking scripts
// hook at 080A0F0C via r0
bool8 TakeStep(void)
{
	// increment new pedometer, always on
	if (gPedometers->alwaysActive != 0xFFFFFFFF)
		gPedometers->alwaysActive += 1;
	
	// check new pedometers
	if ((FlagGet(FLAG_LONG_PEDOMETER)) && gPedometers->large != 0xFFFFFFFF)
		gPedometers->large += 1;
	if ((FlagGet(FLAG_MED_PEDOMETER)) && gPedometers->medium != 0xFFFF)
		gPedometers->medium += 1;
	if ((FlagGet(FLAG_SMALL_PEDOMETER_1)) && gPedometers->smallOne != 0xFF)
		gPedometers->smallOne += 1;		
	if ((FlagGet(FLAG_SMALL_PEDOMETER_2)) && gPedometers->smallTwo != 0xFF)
		gPedometers->smallTwo += 1;	
	
	// check in safari zone
	if ((GetSafariZoneFlag() == TRUE) && (gSafariSteps != 0))
	{
		gSafariSteps -= 1;
		if (gSafariSteps == 0)	// safari steps went to zero
		{
			ScriptContext1_SetupScript((void*) SafariZoneEndScript);
			return TRUE;
		}
    }
	else
	{
		// check custom walking scripts
		if (gWalkingScript != 0)
		{
			ScriptContext1_SetupScript((void*) gWalkingScript);
			return TRUE;
		}
		else
		{
			u8 scriptInd = VarGet(DEFAULT_WALKING_SCRIPT);
			if (scriptInd != 0 || scriptInd > ARRAY_COUNT(gDefaultWalkingScripts))
			{
				if (gDefaultWalkingScripts[scriptInd-1] == 0)
					return FALSE;
				ScriptContext1_SetupScript(gDefaultWalkingScripts[scriptInd-1]);
				return TRUE;
			}
			else
				return FALSE;
		}
	}
	return FALSE;
};

