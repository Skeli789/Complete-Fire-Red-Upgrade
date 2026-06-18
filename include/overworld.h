#pragma once

#include "global.h"

#define LINK_KEY_CODE_NULL 0x00
#define LINK_KEY_CODE_EMPTY 0x11
#define LINK_KEY_CODE_DPAD_DOWN 0x12
#define LINK_KEY_CODE_DPAD_UP 0x13
#define LINK_KEY_CODE_DPAD_LEFT 0x14
#define LINK_KEY_CODE_DPAD_RIGHT 0x15
#define LINK_KEY_CODE_UNK_2 0x16
#define LINK_KEY_CODE_EXIT_ROOM 0x17
#define LINK_KEY_CODE_START_BUTTON 0x18
#define LINK_KEY_CODE_A_BUTTON 0x19
#define LINK_KEY_CODE_UNK_4 0x1A // I'd guess this is the B button?

// These two are a hack to stop user input until link stuff can be
// resolved.
#define LINK_KEY_CODE_HANDLE_RECV_QUEUE 0x1B
#define LINK_KEY_CODE_HANDLE_SEND_QUEUE 0x1C
#define LINK_KEY_CODE_UNK_7 0x1D
#define LINK_KEY_CODE_UNK_8 0x1E

#define MOVEMENT_MODE_FREE 0
#define MOVEMENT_MODE_FROZEN 1
#define MOVEMENT_MODE_SCRIPTED 2

struct InitialPlayerAvatarState
{
    u8 transitionFlags;
    u8 direction;
};

struct LinkPlayerEventObject
{
    u8 active;
    u8 linkPlayerId;
    u8 eventObjId;
    u8 movementMode;
};

// Exported RAM declarations
extern struct WarpData gLastUsedWarp;
extern struct LinkPlayerEventObject gLinkPlayerEventObjects[4];

extern u16 *gBGTilemapBuffers1;
extern u16 *gBGTilemapBuffers2;
extern u16 *gBGTilemapBuffers3;
extern u16 gHeldKeyCodeToSend;
extern void (*gFieldCallback)(void);
extern bool8 (*gFieldCallback2)(void);
extern u8 gLocalLinkPlayerId;
extern u8 gFieldLinkPlayerCount;
extern u8 gGlobalFieldTintMode;

// Exported ROM declarations
extern const struct Coords32 gDirectionToVectors[];

struct MapHeader* __attribute__((long_call)) Overworld_GetMapHeaderByGroupAndId(u16 mapGroup, u16 mapNum);
void __attribute__((long_call)) ResetInitialPlayerAvatarState(void);
void __attribute__((long_call)) StoreInitialPlayerAvatarState(void);
void __attribute__((long_call)) SetDefaultFlashLevel(void);
void __attribute__((long_call)) Overworld_SetFlashLevel(s32 flashLevel);
u8 __attribute__((long_call)) Overworld_GetFlashLevel(void);
void __attribute__((long_call)) SetCurrentMapLayout(u16 mapLayoutId);
u16 __attribute__((long_call)) GetLocationMusic(struct WarpData *warp);
u16 __attribute__((long_call)) GetCurrLocationDefaultMusic(void);
u16 __attribute__((long_call)) GetWarpDestinationMusic(void);
void __attribute__((long_call)) Overworld_ResetMapMusic(void);
void __attribute__((long_call)) Overworld_PlaySpecialMapMusic(void);
void __attribute__((long_call)) Overworld_SetSavedMusic(u16 songNum);
void __attribute__((long_call)) Overworld_ClearSavedMusic(void);
void __attribute__((long_call)) Overworld_ChangeMusicToDefault(void);
void __attribute__((long_call)) Overworld_ChangeMusicTo(u16 newMusic);
u8 __attribute__((long_call)) GetMapMusicFadeoutSpeed(void);
void __attribute__((long_call)) TryFadeOutOldMapMusic(void);
bool8 __attribute__((long_call)) BGMusicStopped(void);
void __attribute__((long_call)) Overworld_FadeOutMapMusic(void);
void __attribute__((long_call)) UpdateAmbientCry(s16 *state, u16 *delayCounter);
bool8 __attribute__((long_call)) CanMusicBeChangedOnMap(u16 songId);
u8 __attribute__((long_call)) GetMapTypeByGroupAndId(s8 mapGroup, s8 mapNum);
u8 __attribute__((long_call)) GetMapTypeByWarpData(struct WarpData *warp);
u8 __attribute__((long_call)) GetCurrentMapType(void);
u8 __attribute__((long_call)) GetLastUsedWarpMapType(void);
bool8 __attribute__((long_call)) IsMapTypeOutdoors(u8 mapType);
bool8 __attribute__((long_call)) Overworld_MapTypeAllowsTeleportAndFly(u8 mapType);
bool8 __attribute__((long_call)) IsMapTypeIndoors(u8 mapType);
u8 __attribute__((long_call)) GetSavedWarpRegionMapSectionId(void);
u8 __attribute__((long_call)) GetCurrentRegionMapSectionId(void);
u8 __attribute__((long_call)) GetCurrentMapBattleScene(void);
void __attribute__((long_call)) CleanupOverworldWindowsAndTilemaps(void);
bool8 __attribute__((long_call)) IsUpdateLinkStateCBActive(void);
void __attribute__((long_call)) CB1_Overworld(void);
void __attribute__((long_call)) CB2_OverworldBasic(void);
void __attribute__((long_call)) CB2_Overworld(void);
void __attribute__((long_call)) SetMainCallback1(void (*cb)(void));
void __attribute__((long_call)) CB2_NewGame(void);
void CB2_WhiteOut(void);
void __attribute__((long_call)) CB2_LoadMap(void);
void __attribute__((long_call)) CB2_ReturnToField(void);
void __attribute__((long_call)) CB2_ReturnToFieldLocal(void);
void __attribute__((long_call)) CB2_ReturnToFieldLink(void);
void __attribute__((long_call)) CB2_ReturnToFieldFromMultiplayer(void);
void __attribute__((long_call)) CB2_ReturnToFieldWithOpenMenu(void);
void __attribute__((long_call)) CB2_ReturnToFieldContinueScript(void);
void __attribute__((long_call)) CB2_ReturnToFieldContinueScriptPlayMapMusic(void);
void __attribute__((long_call)) CB2_ReturnToFieldFromDiploma(void);
void __attribute__((long_call)) CB2_Credits(void);
void __attribute__((long_call)) SetWarpDestination(s8 mapGroup, s8 mapNum, s8 warpId, s8 x, s8 y);
u8 __attribute__((long_call)) GetPlayerFacing(void);
void __attribute__((long_call)) Overworld_SetEventObjTemplateMovementType(u8 localId, u8 movementType);
void __attribute__((long_call)) SetContinueGameWarp(s8 mapGroup, s8 mapNum, s8 warpId, s8 x, s8 y);
void __attribute__((long_call)) SetWarpData(struct WarpData *warp, s8 mapGroup, s8 mapNum, s8 warpId, s8 x, s8 y);
void __attribute__((long_call)) SetEscapeWarp(s8 mapGroup, s8 mapNum, s8 warpId, s8 x, s8 y);
const struct MapHeader* __attribute__((long_call)) GetDestinationWarpMapHeader(void);
void __attribute__((long_call)) FieldClearVBlankHBlankCallbacks(void);
bool32 __attribute__((long_call)) IsPlayerDefeated(u32 battleOutcome);
void __attribute__((long_call)) SetBattledTrainerFlag(void);
void __attribute__((long_call)) QuestLogEvents_HandleEndTrainerBattle(void);
void __attribute__((long_call)) FieldCB_SafariZoneRanOutOfBalls(void);
void __attribute__((long_call)) ResetSafariZoneFlag_(void);
void __attribute__((long_call)) DoWhiteOut(void);
void __attribute__((long_call)) SetInitialPlayerAvatarStateWithDirection(u8 direction);
void __attribute__((long_call)) ScriptContext_Init(void);
void __attribute__((long_call)) UnlockPlayerFieldControls(void);
void __attribute__((long_call)) DoMapLoadLoop(u8 *state);
void __attribute__((long_call)) QuestLog_CutRecording(void);
void __attribute__((long_call)) CB2_NewGameOld(void);
void __attribute__((long_call)) SetFieldVBlankCallback(void);
void Nuzlock_PokemonEraser(void);
