#include "defines.h"
#include "../include/link.h"
#include "../include/menu.h"
#include "../include/safari_zone.h"
#include "../include/start_menu.h"
#include "../include/constants/flags.h"
#include "../include/constants/songs.h"

#include "../include/new/dexnav.h"

u8 PokeToolsFunc(void);

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
	{gText_DexNav, {.u8_void = ExecDexNav}},
};

const u8* sStartMenuDescriptionItems[] = 
{
	&gText_PokedexDescription[0],
	&gText_PokemonDescription[0],
	&gText_BagDescription[0],
	&gText_PlayerDescription[0],
	&gText_SaveDescription[0],
	&gText_OptionDescription[0],
	&gText_ExitDescription[0],
	&gText_RetireDescription[0],
	&gText_PlayerDescription[0],
	&gText_ToolsDescription[0],
	&gText_DexNavDescription[0],
};


void BuildNormalStartMenu() {	
	if (FlagGet(FLAG_SYS_POKEDEX_GET))
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
	
	if (FlagGet(FLAG_SYS_POKEMON_GET))
		AddStartMenuAction(MENU_ACTION_POKEMON);
	
	#ifdef FLAG_SYS_BAG_GET
		if (FlagGet(FLAG_SYS_BAG_GET))
			AddStartMenuAction(MENU_ACTION_BAG);
	#else
		AddStartMenuAction(MENU_ACTION_BAG);
	#endif
	
	#ifdef FLAG_SYS_PLAYER_GET
		if (FlagGet(FLAG_SYS_PLAYER_GET))
			AddStartMenuAction(MENU_ACTION_PLAYER);
	#else
		AddStartMenuAction(MENU_ACTION_PLAYER);
	#endif
	
	#ifdef FLAG_SYS_SAVE_GET
		if (FlagGet(FLAG_SYS_SAVE_GET))
			AddStartMenuAction(MENU_ACTION_SAVE);
	#else
		AddStartMenuAction(MENU_ACTION_SAVE);
	#endif
	
	// these two always present
	AddStartMenuAction(MENU_ACTION_OPTION);
	AddStartMenuAction(MENU_ACTION_EXIT);
};


void BuildSafariZoneStartMenu() {
    AddStartMenuAction(MENU_ACTION_RETIRE_SAFARI);

	if (FlagGet(FLAG_SYS_POKEDEX_GET))
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

	if (FlagGet(FLAG_SYS_POKEMON_GET))
		AddStartMenuAction(MENU_ACTION_POKEMON);
	
	#ifdef FLAG_SYS_BAG_GET
		if (FlagGet(FLAG_SYS_BAG_GET))
			AddStartMenuAction(MENU_ACTION_BAG);
	#else
		AddStartMenuAction(MENU_ACTION_BAG);
	#endif
	
	#ifdef FLAG_SYS_PLAYER_GET
		if (FlagGet(FLAG_SYS_PLAYER_GET))
			AddStartMenuAction(MENU_ACTION_PLAYER);
	#else
		AddStartMenuAction(MENU_ACTION_PLAYER);
	#endif
	
    AddStartMenuAction(MENU_ACTION_OPTION);
    AddStartMenuAction(MENU_ACTION_EXIT);
}


#ifdef FLAG_POKETOOLS_MENU
void BuildPokeToolsMenu(void) {
	
	gStartMenu->numItems = 0;
	
	AddStartMenuAction(MENU_ACTION_POKEDEX);
	AddStartMenuAction(MENU_ACTION_DEXNAV);
	AddStartMenuAction(MENU_ACTION_EXIT);
	
	FlagClear(FLAG_POKETOOLS_MENU);
};
#endif


void BuildStartMenuActions(void)
{
    gStartMenu->numItems = 0;
	
    if (IsUpdateLinkStateCBActive())
        BuildLinkModeStartMenu();
    else if (InUnionRoom())
        BuildUnionRoomStartMenu();
    else if (GetSafariZoneFlag())
        BuildSafariZoneStartMenu();
	#ifdef FLAG_POKETOOLS_MENU
	else if (FlagGet(FLAG_POKETOOLS_MENU))
		BuildPokeToolsMenu();
	#endif
    else
        BuildNormalStartMenu();
}


static void CloseStartMenu(void)
{
	CloseSafariStepsBox();	// void safari_stepscount_close(void) 0806EF18
	CloseStartMenuDescriptionBox();	// void sm_close_description(void) 080F7998
	HideStartMenu();		// void sm_close_menu(void) 0806FEA0
}



u8 PokeToolsFunc(void) {
    CloseStartMenu();
	PlaySE(SE_WIN_OPEN);
	
	#ifdef FLAG_POKETOOLS_MENU
		FlagSet(FLAG_POKETOOLS_MENU);
		ShowStartMenu();
	#else
		CreateTask(ToolSelection, 0);
	#endif
	
    return 1;
}

