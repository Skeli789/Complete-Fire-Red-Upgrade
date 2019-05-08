#include "defines.h"
#include "../include/start_menu.h"
#include "../include/link.h"
#include "../include/menu.h"
#include "../include/constants/flags.h"
#include "../include/new/dexnav.h"
#include "../include/safari_zone.h"

#ifdef FLAG_SYS_DEXNAV

	// need:
		//0006EF9C
		//0006F374
		//0006F3F8

	const struct MenuAction sStartMenuItems[] =
	{
		{gText_MenuPokedex, {.u8_void = StartMenuPokedexCallback}},
		{gText_MenuPokemon, {.u8_void = StartMenuPokemonCallback}},
		{gText_MenuBag, {.u8_void = StartMenuBagCallback}},
		{gText_MenuPlayer, {.u8_void = StartMenuPlayerNameCallback}},
		{gText_MenuSave, {.u8_void = StartMenuSaveCallback}},
		{gText_MenuOption, {.u8_void = StartMenuOptionCallback}},
		{gText_MenuExit, {.u8_void = StartMenuExitCallback}},
		{gText_MenuRetire, {.u8_void = StartMenuSafariZoneRetireCallback}},
		{gText_MenuPlayer, {.u8_void = StartMenuLinkModePlayerNameCallback}},
		{gText_PokeTools, {.u8_void = PokeToolsFunc}},
	};
#else
	#define sStartMenuItems ((struct MenuAction*) 0x806EF9C)
#endif



void BuildNormalStartMenu() {
	
	#ifdef DYNAMIC_START_MENU		
		if (FlagGet(FLAG_SYS_POKEDEX_GET) == TRUE)
		{
			#ifdef FLAG_SYS_DEXNAV
				if (FlagGet(FLAG_SYS_DEXNAV))
					AddStartMenuAction(MENU_ACTION_POKETOOLS);
				else
					AddStartMenuAction(MENU_ACTION_POKEDEX);
			#else
				AddStartMenuAction(MENU_ACTION_POKEDEX);
			#endif
		}
		if (FlagGet(FLAG_SYS_POKEMON_GET) == TRUE)
			AddStartMenuAction(MENU_ACTION_POKEMON);
		
		if (FlagGet(FLAG_SYS_BAG_GET) == TRUE)
			AddStartMenuAction(MENU_ACTION_BAG);
		
		AddStartMenuAction(MENU_ACTION_PLAYER);
		
		if (FlagGet(FLAG_SYS_SAVE_GET) == TRUE)
			AddStartMenuAction(MENU_ACTION_SAVE);
		
		AddStartMenuAction(MENU_ACTION_OPTION);
		AddStartMenuAction(MENU_ACTION_EXIT);
		
	#else
		//normal start menu
		if (FlagGet(FLAG_SYS_POKEDEX_GET) == TRUE)
		{
			#ifdef FLAG_SYS_DEXNAV
				if (FlagGet(FLAG_SYS_DEXNAV))
					AddStartMenuAction(MENU_ACTION_POKETOOLS);
				else
					AddStartMenuAction(MENU_ACTION_POKEDEX);
			#else
				AddStartMenuAction(MENU_ACTION_POKEDEX);
			#endif
		}
		if (FlagGet(FLAG_SYS_POKEMON_GET) == TRUE)
			AddStartMenuAction(MENU_ACTION_POKEMON);
		
		AddStartMenuAction(MENU_ACTION_BAG);
		AddStartMenuAction(MENU_ACTION_PLAYER);
		AddStartMenuAction(MENU_ACTION_SAVE);
		AddStartMenuAction(MENU_ACTION_OPTION);
		AddStartMenuAction(MENU_ACTION_EXIT);
	
	#endif	
};


void BuildSafariZoneStartMenu() {
    AddStartMenuAction(MENU_ACTION_RETIRE_SAFARI);
    AddStartMenuAction(MENU_ACTION_POKEDEX);
    AddStartMenuAction(MENU_ACTION_POKEMON);
    AddStartMenuAction(MENU_ACTION_BAG);
    AddStartMenuAction(MENU_ACTION_PLAYER);
    AddStartMenuAction(MENU_ACTION_OPTION);
    AddStartMenuAction(MENU_ACTION_EXIT);
}



void BuildStartMenuActions(void)
{
    sNumStartMenuActions = 0;
	
    if (IsUpdateLinkStateCBActive() == TRUE)
        BuildLinkModeStartMenu();
    else if (InUnionRoom() == TRUE)
        BuildUnionRoomStartMenu();
    else if (GetSafariZoneFlag() == TRUE)
        BuildSafariZoneStartMenu();
    else
        BuildNormalStartMenu();
}

