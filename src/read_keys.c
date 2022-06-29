#include "defines.h"
#include "../include/bg.h"
#include "../include/event_object_movement.h"
#include "../include/link.h"
#include "../include/field_player_avatar.h"
#include "../include/field_weather.h"
#include "../include/item_menu.h"
#include "../include/map_name_popup.h"
#include "../include/overworld.h"
#include "../include/party_menu.h"
#include "../include/rtc.h"
#include "../include/script.h"
#include "../include/start_menu.h"
#include "../include/constants/maps.h"
#include "../include/constants/region_map_sections.h"
#include "../include/constants/songs.h"

#include "../include/new/dexnav.h"
#include "../include/new/item.h"
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

extern const u8 EventScript_SecondBagItemCanBeRegisteredToL[];
extern const u8 SystemScript_EnableAutoRun[];
extern const u8 SystemScript_DisableAutoRun[];
extern const u8 SystemScript_EnableBikeTurboBoost[];
extern const u8 SystemScript_DisableBikeTurboBoost[];
extern const u8 SystemScript_EnableSurfTurboBoost[];
extern const u8 SystemScript_DisableSurfTurboBoost[];
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
	u16 currKeys, keyInput, tryKey;
	RtcCalcLocalTime(); //Called here for convenience

	#ifdef SAVE_BLOCK_EXPANSION
	currKeys = gKeyReg;
	tryKey = gKeypadSetter->keyFlags;
	if (tryKey != 0)
	{
		TryForcedScript(tryKey, currKeys);
		currKeys = TryForcedKey(tryKey, currKeys);
		currKeys = TryIgnoringKeys(tryKey, currKeys);
	}
	keyInput = KEYS_MASK ^ currKeys;
	#else
	keyInput = REG_KEYINPUT ^ KEYS_MASK;
	#endif
	gMain.newKeysRaw = keyInput & ~gMain.heldKeysRaw;
	gMain.newKeys = gMain.newKeysRaw;
	gMain.newAndRepeatedKeys = gMain.newKeysRaw;

	if (keyInput != 0 && gMain.heldKeysRaw == keyInput)
	{
		gMain.keyRepeatCounter--;
		if (gMain.keyRepeatCounter == 0)
		{
			gMain.newAndRepeatedKeys = keyInput;
			gMain.keyRepeatCounter = gKeyRepeatContinueDelay;

			if (gSaveBlock2->optionsButtonMode == OPTIONS_BUTTON_MODE_L_EQUALS_A
			&& gMain.newKeys & L_BUTTON)
				gMain.newAndRepeatedKeys |= A_BUTTON;
		}
	}
	else
	{
		// If there is no input or the input has changed, reset the counter.
		gMain.keyRepeatCounter = gKeyRepeatStartDelay;
	}

	gMain.heldKeysRaw = keyInput;
	gMain.heldKeys = gMain.heldKeysRaw;

	// Remap L to A if the L=A option is enabled.
	if (gSaveBlock2->optionsButtonMode == OPTIONS_BUTTON_MODE_L_EQUALS_A)
	{
		if (gMain.newKeys & L_BUTTON)
			gMain.newKeys |= A_BUTTON;

		if (gMain.heldKeys & L_BUTTON)
			gMain.heldKeys |= A_BUTTON;
	}

	if (gMain.newKeys & gMain.watchedKeysMask)
		gMain.watchedKeysPressed = TRUE;
}

bool8 StartLButtonFunc(void)
{
	if (IsDexNavHudActive())
		return FALSE;

	if (gSaveBlock2->optionsButtonMode == OPTIONS_BUTTON_MODE_SECOND_REGISTERED_ITEM)
	{
		u16 secondRegisteredItem = gSaveBlock1->registeredItems[1];
		if (secondRegisteredItem != ITEM_NONE)
		{
			if (CheckBagHasItem(secondRegisteredItem, 1))
			{
				UseRegisteredItem(secondRegisteredItem);
				return TRUE;
			}
			else 
				RemoveRegisteredItem(secondRegisteredItem);
		}

		ScriptContext1_SetupScript(EventScript_SecondBagItemCanBeRegisteredToL);
		return TRUE;
	}
	else if (gSaveBlock2->optionsButtonMode != OPTIONS_BUTTON_MODE_L_EQUALS_A)
	{
		#ifdef FLAG_BIKE_TURBO_BOOST
		if (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_BIKE))
		{
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
			
			return TRUE;
		}
		else
		#endif
		#ifdef FLAG_SURF_TURBO_BOOST
		if (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_SURFING) || gMapHeader.mapType == MAP_TYPE_UNDERWATER)
		{
			DismissMapNamePopup();

			if (FlagGet(FLAG_SURF_TURBO_BOOST))
			{
				FlagClear(FLAG_SURF_TURBO_BOOST);
				ScriptContext1_SetupScript(SystemScript_DisableSurfTurboBoost);
			}
			else
			{
				FlagSet(FLAG_SURF_TURBO_BOOST);
				ScriptContext1_SetupScript(SystemScript_EnableSurfTurboBoost);
			}
			
			return TRUE;
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

			return TRUE;
			#endif
		}
	}
	
	return FALSE;
}

bool8 StartRButtonFunc(void)
{
	if (IsDexNavHudActive() || InUnionRoom())
		return FALSE;

	u16 dexNavSpecies = VarGet(VAR_DEXNAV);

	#ifndef VAR_R_BUTTON_MODE
	if (dexNavSpecies != SPECIES_NONE)
	{
		DismissMapNamePopup();
		ChangeBgY(0, 0, 0);
		if (!InitDexNavHUD(dexNavSpecies & 0x7FFF, dexNavSpecies >> 15, FALSE))
			return TRUE; //HUD wasn't enabled and a script was started instead
		return FALSE; //Don't enable the script context
	}
	#else
	switch (VarGet(VAR_R_BUTTON_MODE)) {
		case OPTIONS_R_BUTTON_MODE_DEXNAV:
			if (dexNavSpecies != SPECIES_NONE && FlagGet(FLAG_SYS_DEXNAV))
			{
				DismissMapNamePopup();
				ChangeBgY(0, 0, 0);
				if (!InitDexNavHUD(dexNavSpecies & 0x7FFF, dexNavSpecies >> 15, FALSE))
					return TRUE; //HUD wasn't enabled and a script was started instead
				return FALSE; //Don't enable the script context
			}
			break;
		case OPTIONS_R_BUTTON_MODE_POKEMON_MENU:
			if (!gPaletteFade->active && FlagGet(FLAG_SYS_POKEMON_GET))
			{
				ScriptContext2_Enable();
				ScriptContext1_SetupScript(SystemScript_PartyMenuFromField);
				return TRUE;
			}
			break;
		case OPTIONS_R_BUTTON_MODE_BAG:
			if (!gPaletteFade->active && !FlagGet(FLAG_SYS_BAG_HIDE))
			{
				ScriptContext2_Enable();
				ScriptContext1_SetupScript(SystemScript_ItemMenuFromField);
				return TRUE;
			}
			break;
		case OPTIONS_R_BUTTON_MODE_MISSION_LOG:
			#ifdef FLAG_SYS_QUEST_LOG
			if (!gPaletteFade->active && FlagGet(FLAG_SYS_QUEST_LOG))
			{
				PlaySE(SE_SELECT);
				typedef void (*OpenQuestLogFromOverworld_T) (void);
				#define OpenQuestLogFromOverworld ((OpenQuestLogFromOverworld_T) (0x801D770 |1))
				OpenQuestLogFromOverworld();
				return TRUE;
			}
			#endif
			break;
		case OPTIONS_R_BUTTON_MODE_MINING:
			#ifdef MB_UNDERGROUND_MINING
			if (GetCurrentRegionMapSectionId() == MAPSEC_KBT_EXPRESSWAY
			|| MAP_IS(CRYSTAL_PEAK_1F_LEFT_ROOM)
			|| (gMapHeader.mapType == MAP_TYPE_UNDERWATER
			 && !MAP_IS(MIRSKLE_LAB_UNDERWATER_1)
			 && !MAP_IS(MIRSKLE_LAB_UNDERWATER_2)
			 && !MAP_IS(VIVILL_WAREHOUSE_UNDERWATER)))
			{
				TryLoadMiningSpots();
				ChooseMiningSpotToShow();
				ScriptContext2_Enable();
				ScriptContext1_SetupScript(SystemScript_MiningScan);
				return TRUE;
			}
			#endif
			break;
		case OPTIONS_R_BUTTON_MODE_DEBUG:
			ScriptContext2_Enable();
			ScriptContext1_SetupScript(SystemScript_DebugMenu);
			return TRUE;
	}
	#endif

	return FALSE;
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
	FreezeEventObjects();
	ScriptContext2_Enable(); //So HMs don't cause problems
	PrepareOverworldReturn();
	InitPartyMenu(PARTY_MENU_TYPE_FIELD, PARTY_LAYOUT_SINGLE, PARTY_ACTION_CHOOSE_MON, FALSE, PARTY_MSG_CHOOSE_MON, (void*) (0x811FB28 | 1), CB2_ReturnToFieldFromDiploma);
}

static void CB2_ItemMenuFromField(void)
{
	FreezeEventObjects();
	PrepareOverworldReturn();
	GoToBagMenu(BAG_OPEN_REGULAR, OPEN_BAG_LAST, CB2_ReturnToFieldFromDiploma); //Continue script is needed so followers don't get messed up
}

u8 GetLRKeysState(void)
{
	if (gSaveBlock2->optionsButtonMode != OPTIONS_BUTTON_MODE_L_EQUALS_A)
	{
		if (JOY_NEW(L_BUTTON))
			return 1;
		if (JOY_NEW(R_BUTTON))
			return 2;
	}

	return 0;
}

u8 GetLRKeysPressedAndHeld(void)
{
	if (gSaveBlock2->optionsButtonMode != OPTIONS_BUTTON_MODE_L_EQUALS_A)
	{
		if (JOY_REPT(L_BUTTON))
			return 1;
		if (JOY_REPT(R_BUTTON))
			return 2;
	}

	return 0;
}

s32 sub_8104284(void)
{
	if (gSaveBlock2->optionsButtonMode != OPTIONS_BUTTON_MODE_L_EQUALS_A)
	{
		if (JOY_HELD(R_BUTTON) && JOY_NEW(DPAD_LEFT))
			return 1;
		else if (JOY_HELD(R_BUTTON) && JOY_NEW(DPAD_RIGHT))
			return 2;
	}

	return 0;
}
