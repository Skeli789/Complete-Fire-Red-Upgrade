#include "defines.h"
#include "helper_functions.h"
#include "catching.h"
#include "Vanilla_Functions.h"
/*
emulate JPANs keypad specials

hook at 080005e8 via r0
*/

void tryForcedScript(u8 key, u16 currKeys) {
	if (!(key & 0x4))
		return;
	else if (gScriptEnv2->enabled)
		return;
	else if (!(gKeyBluff->pressed & currKeys))
		return;
	else if (gTasks[0].func == (void*) TaskOverworld)
		return;
	script_env_2_enable();
	script_run((void*) gKeyBluff->scriptToRun);
	return;
};

u16 tryForcedKey(u8 key, u16 currKeys) {
	if (!(key & 0x1))
		return 0;
	u8 keyFlags = gKeyBluff->keyFlag;
	if (keyFlags == 0)
		return 0;
	keyFlags -= 1;
	gKeyBluff->keyFlag = keyFlags;
	u16 keyMap = ~gKeyBluff->keyMapToForce;
	return (keyMap & currKeys);
};

u16 tryIgnoringKeys(u8 key, u16 currKeys) {
	if (!(key & 0x2))
		return 0;
	return (currKeys | gKeyBluff->keysToIgnore);
};

void LoadKeys(void) {
	u16 currKeys = gKeyReg;
	u8 tryKey = gKeyBluff->checkKey;
	//u8 forcedKey = gKeyBluff;	//temp
	
	if (tryKey != 0)
	{
		tryForcedScript(tryKey, currKeys);
		currKeys = tryForcedKey(tryKey, currKeys);
		currKeys = tryIgnoringKeys(tryKey, currKeys);
	}
	
	// to do
	
};


