#include "defines.h"
#include "../include/link.h"
#include "../include/script.h"
#include "../include/field_player_avatar.h"
#include "../include/field_weather.h"
#include "../include/item_menu.h"
#include "../include/overworld.h"
#include "../include/map_name_popup.h"
#include "../include/rtc.h"
#include "../include/start_menu.h"
#include "../include/party_menu.h"
#include "../include/constants/region_map_sections.h"

#include "../include/new/dexnav.h"
#include "../include/new/overworld.h"
#include "../include/new/read_keys.h"
#include "../include/new/util.h"

/*
read_keys.c
	emulated JPANs keypad hacks, allowing the designer to force key presses, prevent them, or map functions onto them, among other uses.
*/

/*
// called from AgbMain
void InitKeys(void)
{
	gKeyRepeatContinueDelay = 5;
	gKeyRepeatStartDelay = 40;

	gMain.heldKeys = 0;
	gMain.newKeys = 0;
	gMain.newAndRepeatedKeys = 0;
	gMain.heldKeysRaw = 0;
	gMain.newKeysRaw = 0;
}
*/
// values from InitKeys

#define gKeyRepeatContinueDelay 5
#define gKeyRepeatStartDelay 40

extern const u8 SystemScript_EnableAutoRun[];
extern const u8 SystemScript_DisableAutoRun[];
extern const u8 SystemScript_EnableBikeTurboBoost[];
extern const u8 SystemScript_DisableBikeTurboBoost[];
extern const u8 SystemScript_PartyMenuFromField[];
extern const u8 SystemScript_ItemMenuFromField[];
extern const u8 SystemScript_MiningScan[];
extern const u8 SystemScript_DebugMenu[];

static void CB2_PartyMenuFromField(void);
static void CB2_ItemMenuFromField(void);
#ifdef SAVE_BLOCK_EXPANSION
static void TryForcedScript(u8 keyFlag, u16 currKeys);
static u16 TryForcedKey(u8 keyFlag, u16 currKeys);
static u16 TryIgnoringKeys(u8 keyFlag, u16 currKeys);

static void TryForcedScript(u8 keyFlag, u16 currKeys)
{
	if (!(keyFlag & 0x4))
		return;	// flag not set

	if (ScriptContext2_IsEnabled())
		return;	// a script is already enabled

	if (gTasks[0].func != Task_WeatherMain)
		return;

	if (gKeypadSetter->keyToRunScript & currKeys)
		return;

	ScriptContext2_Enable();
	DismissMapNamePopup();
	ScriptContext1_SetupScript(gKeypadSetter->scriptToRun);
}


static u16 TryForcedKey(u8 keyFlag, u16 currKeys)
{
	if (!(keyFlag & 0x1))
		return currKeys;

	if (gKeypadSetter->keyForcingCounter == 0)
		return currKeys;

	gKeypadSetter->keyForcingCounter -= 1;
	return (~(gKeypadSetter->keyMapToForce) & currKeys);
}


static u16 TryIgnoringKeys(u8 keyFlag, u16 currKeys)
{
	if (!(keyFlag & 0x2))
		return currKeys;

	return (currKeys | gKeypadSetter->keysToIgnore);
};
#endif


// hook at 080005e8 via r0
void ReadKeys(void)
{
	RtcCalcLocalTime(); //Called here for convenience

	#ifdef SAVE_BLOCK_EXPANSION
		u16 currKeys = gKeyReg;
		u8 tryKey = gKeypadSetter->keyFlags;
		if (tryKey != 0)
		{
			TryForcedScript(tryKey, currKeys);
			currKeys = TryForcedKey(tryKey, currKeys);
			currKeys = TryIgnoringKeys(tryKey, currKeys);
		}
		u16 keyInput = KEYS_MASK ^ currKeys;
	#else
		u16 keyInput = REG_KEYINPUT ^ KEYS_MASK;
	#endif
	gMain.newKeysRaw = keyInput & ~gMain.heldKeysRaw;
	gMain.newKeys = gMain.newKeysRaw;
	gMain.newAndRepeatedKeys = gMain.newKeysRaw;

	// BUG: Key repeat won't work when pressing L using L=A button mode
	// because it compares the raw key input with the remapped held keys.
	// Note that newAndRepeatedKeys is never remapped either.

	if (keyInput != 0 && gMain.heldKeys == keyInput)
	{
		gMain.keyRepeatCounter--;
		if (gMain.keyRepeatCounter == 0)
		{
			gMain.newAndRepeatedKeys = keyInput;
			gMain.keyRepeatCounter = gKeyRepeatContinueDelay;
		}
	}
	else
	{
		// If there is no input or the input has changed, reset the counter.
		gMain.keyRepeatCounter = gKeyRepeatStartDelay;
	}

	gMain.heldKeysRaw = keyInput;
	gMain.heldKeys = gMain.heldKeysRaw;

	bool8 inOverworld = FuncIsActiveTask(Task_WeatherMain);

	// Remap L to A if the L=A option is enabled.
	if (gSaveBlock2->optionsButtonMode == 2)
	{
		if (gMain.newKeys & L_BUTTON)
			gMain.newKeys |= A_BUTTON;

		if (gMain.heldKeys & L_BUTTON)
			gMain.heldKeys |= A_BUTTON;
	}
	else if (gMain.newKeys & L_BUTTON //Can't be used if L=A
	&& !ScriptContext2_IsEnabled()
	&& !IsDexNavHudActive()
	&& inOverworld) //In the overworld
	{
		#ifdef FLAG_BIKE_TURBO_BOOST
		if (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_BIKE))
		{
			ScriptContext2_Enable();
			DismissMapNamePopup();

			if (FlagGet(FLAG_BIKE_TURBO_BOOST))
			{
				FlagClear(FLAG_BIKE_TURBO_BOOST);
				ScriptContext1_SetupScript(SystemScript_DisableBikeTurboBoost);
			}
			else
			{
				FlagSet(FLAG_BIKE_TURBO_BOOST);
				ScriptContext1_SetupScript(SystemScript_EnableBikeTurboBoost);
			}
		}
		else
		#endif
		#ifdef FLAG_RUNNING_ENABLED
		if (FlagGet(FLAG_RUNNING_ENABLED)) //Only toggle auto-run if can run in the first place
		#endif
		{
			#ifdef FLAG_AUTO_RUN
			ScriptContext2_Enable();
			DismissMapNamePopup();

			if (FlagGet(FLAG_AUTO_RUN))
			{
				FlagClear(FLAG_AUTO_RUN);
				ScriptContext1_SetupScript(SystemScript_DisableAutoRun);
			}
			else
			{
				FlagSet(FLAG_AUTO_RUN);
				ScriptContext1_SetupScript(SystemScript_EnableAutoRun);
			}
			#endif
		}
	}

	if (gMain.newKeys & R_BUTTON
	&& inOverworld
	&& !ScriptContext2_IsEnabled()
	&& !IsDexNavHudActive()
	&& !InUnionRoom())
	{
		u16 dexNavSpecies = VarGet(VAR_DEXNAV);

		#ifndef VAR_R_BUTTON_MODE
		if (dexNavSpecies != SPECIES_NONE)
			InitDexNavHUD(dexNavSpecies & 0x7FFF, dexNavSpecies >> 15);
		#else
		switch (VarGet(VAR_R_BUTTON_MODE)) {
			case OPTIONS_R_BUTTON_MODE_DEXNAV:
				if (dexNavSpecies != SPECIES_NONE)
					InitDexNavHUD(dexNavSpecies & 0x7FFF, dexNavSpecies >> 15);
				break;
			case OPTIONS_R_BUTTON_MODE_POKEMON_MENU:
				if (!gPaletteFade->active)
				{
					ScriptContext2_Enable();
					ScriptContext1_SetupScript(SystemScript_PartyMenuFromField);
				}
				break;
			case OPTIONS_R_BUTTON_MODE_BAG:
				if (!gPaletteFade->active)
				{
					ScriptContext2_Enable();
					ScriptContext1_SetupScript(SystemScript_ItemMenuFromField);
				}
				break;
			case OPTIONS_R_BUTTON_MODE_MINING:
				#ifdef MB_UNDERGROUND_MINING
				if (GetCurrentRegionMapSectionId() == MAPSEC_KBT_EXPRESSWAY)
				{
					TryLoadMiningSpots();
					ChooseMiningSpotToShow();
					ScriptContext2_Enable();
					ScriptContext1_SetupScript(SystemScript_MiningScan);
				}
				#endif
				break;
			case OPTIONS_R_BUTTON_MODE_DEBUG:
				ScriptContext2_Enable();
				ScriptContext1_SetupScript(SystemScript_DebugMenu);
				break;
		}
		#endif
	}

	if (gMain.newKeys & gMain.watchedKeysMask)
		gMain.watchedKeysPressed = TRUE;
}

void InitPartyMenuFromField(void)
{
	PlayRainStoppingSoundEffect();
	CleanupOverworldWindowsAndTilemaps();
	SetMainCallback2(CB2_PartyMenuFromField); //Display party menu
}

void InitBagMenuFromField(void)
{
	PlayRainStoppingSoundEffect();
	CleanupOverworldWindowsAndTilemaps();
	SetMainCallback2(CB2_ItemMenuFromField); //Display item menu
}

static void CB2_PartyMenuFromField(void)
{
    InitPartyMenu(PARTY_MENU_TYPE_FIELD, PARTY_LAYOUT_SINGLE, PARTY_ACTION_CHOOSE_MON, FALSE, PARTY_MSG_CHOOSE_MON, (void*) (0x811FB28 | 1), CB2_ReturnToField);
}

static void CB2_ItemMenuFromField(void)
{
	GoToBagMenu(BAG_OPEN_REGULAR, OPEN_BAG_LAST, CB2_ReturnToField);
}
