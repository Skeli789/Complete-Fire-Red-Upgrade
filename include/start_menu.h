#ifndef GUARD_START_MENU_H
#define GUARD_START_MENU_H

#include "global.h"

extern const u8 gText_MenuPokedex[];
extern const u8 gText_MenuPokemon[];
extern const u8 gText_MenuPlayer[];
extern const u8 gText_MenuSave[];
extern const u8 gText_MenuOption[];
extern const u8 gText_MenuExit[];
extern const u8 gText_MenuRetire[];
extern const u8 gText_PokeTools[];
extern const u8 gText_DexNav[];

#ifdef UNBOUND
extern const u8 gText_MenuCube[];
#define gText_MenuBag gText_MenuCube
#else
extern const u8 gText_MenuBag[];
#endif

extern const u8 gText_PokedexDescription[];
extern const u8 gText_PokemonDescription[];
extern const u8 gText_BagDescription[];
extern const u8 gText_PlayerDescription[];
extern const u8 gText_SaveDescription[];
extern const u8 gText_OptionDescription[];
extern const u8 gText_ExitDescription[];
extern const u8 gText_RetireDescription[];
extern const u8 gText_PlayerDescription[];
extern const u8 gText_ToolsDescription[];
extern const u8 gText_DexNavDescription[];


typedef void (*MainCallback)(void);

struct StartMenu {
	MainCallback activeContext;
	u8 cursorPos;
	u8 numItems;
	u8 menuIndices[9];
	u8 stateTracker;
	u8 stateTracker2;
	u8 safariZone;
};


enum
{
    MENU_ACTION_POKEDEX,
    MENU_ACTION_POKEMON,
    MENU_ACTION_BAG,
    MENU_ACTION_PLAYER,
    MENU_ACTION_SAVE,
    MENU_ACTION_OPTION,
    MENU_ACTION_EXIT,
    MENU_ACTION_RETIRE_SAFARI,
    MENU_ACTION_PLAYER_LINK,
	MENU_ACTION_POKETOOLS,
	MENU_ACTION_DEXNAV,
	NUM_START_MENU_OPTIONS,	// always have at end
};

void AppendToList(u8* list, u8* pos, u8 newEntry);

bool8 __attribute__((long_call)) IsUpdateLinkStateCBActive(void);
//void __attribute__((long_call)) BuildSafariZoneStartMenu(void);
void __attribute__((long_call)) BuildLinkModeStartMenu(void);
void __attribute__((long_call)) BuildUnionRoomStartMenu(void);
void __attribute__((long_call)) AddStartMenuAction(u8 action);

bool8 __attribute__((long_call)) StartMenuPokedexCallback(void);
bool8 __attribute__((long_call)) StartMenuPokemonCallback(void);
bool8 __attribute__((long_call)) StartMenuBagCallback(void);
bool8 __attribute__((long_call)) StartMenuPlayerNameCallback(void);
bool8 __attribute__((long_call)) StartMenuSaveCallback(void);
bool8 __attribute__((long_call)) StartMenuOptionCallback(void);
bool8 __attribute__((long_call)) StartMenuExitCallback(void);
bool8 __attribute__((long_call)) StartMenuSafariZoneRetireCallback(void);
bool8 __attribute__((long_call)) StartMenuLinkModePlayerNameCallback(void);
void __attribute__((long_call)) TaskStartMenu(u8 taskId);
void __attribute__((long_call)) ShowStartMenu(void);


#endif // GUARD_START_MENU_H
