#include "defines.h"
#include "../include/link.h"
#include "../include/menu.h"
#include "../include/safari_zone.h"
#include "../include/start_menu.h"
#include "../include/constants/flags.h"
#include "../include/constants/songs.h"

#include "../include/new/dexnav.h"

/*
start_menu.c
	Functions to redo how the start menu is generated, and
	associated functions such as safari steps/ball count.
*/

//This file's functions:
static void CloseStartMenu(void);
static void BuildNormalStartMenu();
static void BuildSafariZoneStartMenu();
#ifdef FLAG_POKETOOLS_MENU
static void BuildPokeToolsMenu(void);
#endif

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

const u8* const sStartMenuDescriptionItems[] =
{
	gText_PokedexDescription,
	gText_PokemonDescription,
	gText_BagDescription,
	gText_PlayerDescription,
	gText_SaveDescription,
	gText_OptionDescription,
	gText_ExitDescription,
	gText_RetireDescription,
	gText_PlayerDescription,
	gText_ToolsDescription,
	gText_DexNavDescription,
};

void __attribute__((long_call)) CloseStartMenuDescriptionBox(void);
void __attribute__((long_call)) HideStartMenu(void);

static void BuildNormalStartMenu()
{
	if (FlagGet(FLAG_SYS_POKEDEX_GET))
	{
		#ifdef FLAG_SYS_DEXNAV
			if (FlagGet(FLAG_SYS_DEXNAV))
				AddStartMenuAction(MENU_ACTION_POKETOOLS);
			else
		#endif
				AddStartMenuAction(MENU_ACTION_POKEDEX);
	}

	if (FlagGet(FLAG_SYS_POKEMON_GET))
		AddStartMenuAction(MENU_ACTION_POKEMON);

	#ifdef FLAG_SYS_BAG_HIDE
		if (!FlagGet(FLAG_SYS_BAG_HIDE))
	#endif
			AddStartMenuAction(MENU_ACTION_BAG);

	#ifdef FLAG_SYS_PLAYER_HIDE
		if (!FlagGet(FLAG_SYS_PLAYER_HIDE))
	#endif
			AddStartMenuAction(MENU_ACTION_PLAYER);

	#ifdef FLAG_SYS_SAVE_HIDE
		if (!FlagGet(FLAG_SYS_SAVE_HIDE))
	#endif
			AddStartMenuAction(MENU_ACTION_SAVE);

	//These two are always present
	AddStartMenuAction(MENU_ACTION_OPTION);
	AddStartMenuAction(MENU_ACTION_EXIT);
}


static void BuildSafariZoneStartMenu()
{
	AddStartMenuAction(MENU_ACTION_RETIRE_SAFARI);

	if (FlagGet(FLAG_SYS_POKEDEX_GET))
	{
		#ifdef FLAG_SYS_DEXNAV
			if (FlagGet(FLAG_SYS_DEXNAV))
				AddStartMenuAction(MENU_ACTION_POKETOOLS);
			else
		#endif
				AddStartMenuAction(MENU_ACTION_POKEDEX);
	}

	#ifdef FLAG_SYS_POKEMON_GET
	if (FlagGet(FLAG_SYS_POKEMON_GET))
	#endif
		AddStartMenuAction(MENU_ACTION_POKEMON);

	#ifdef FLAG_SYS_BAG_GET
		if (FlagGet(FLAG_SYS_BAG_GET))
	#endif
			AddStartMenuAction(MENU_ACTION_BAG);

	#ifdef FLAG_SYS_PLAYER_GET
		if (FlagGet(FLAG_SYS_PLAYER_GET))
	#endif
			AddStartMenuAction(MENU_ACTION_PLAYER);

	AddStartMenuAction(MENU_ACTION_OPTION);
	AddStartMenuAction(MENU_ACTION_EXIT);
}


#ifdef FLAG_POKETOOLS_MENU
static void BuildPokeToolsMenu(void)
{
	gStartMenu->numItems = 0;

	AddStartMenuAction(MENU_ACTION_POKEDEX);
	AddStartMenuAction(MENU_ACTION_DEXNAV);
	AddStartMenuAction(MENU_ACTION_EXIT);

	FlagClear(FLAG_POKETOOLS_MENU);
}
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
	DestroySafariZoneStatsWindow();	//0x806EF18
	CloseStartMenuDescriptionBox();	//0x80F7998
	HideStartMenu();		//0x806FEA0
}


u8 PokeToolsFunc(void)
{
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
