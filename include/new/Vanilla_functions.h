#pragma once
#include "../../src/defines.h"
#include "../string_util.h"

#define BC_Menu 0x8014040 | 1

bool8 __attribute__((long_call)) ScriptEnv2IsEnabled(void);
void __attribute__((long_call)) ScriptEnvDisable(void);
void* __attribute__((long_call)) memcpy_(void *dst, const void* src, u8 size);
void* __attribute__((long_call)) memset_(void *dst, u8 pattern, u8 size);

void* __attribute__((long_call)) Memcpy(void *dst, const void *src, u32 size);
void* __attribute__((long_call)) Memset(void *dst, u8 pattern, u32 size);

void __attribute__((long_call)) PokemonSlotPurge(pokemon_t* mon);

//PokeFireRed Function Names

//Pokemon Functions
void __attribute__((long_call)) EmitSetMonData(u8 a, u8 request, u8 c, u8 bytes, void *data);
void __attribute__((long_call)) ZeroPlayerPartyMons(void);
void __attribute__((long_call)) ZeroEnemyPartyMons(void);
void __attribute__((long_call)) CreateMon(struct Pokemon *mon, u16 species, u8 level, u8 fixedIV, u8 hasFixedPersonality, u32 fixedPersonality, u8 otIdType, u32 fixedOtId);
//void __attribute__((long_call)) CreateBoxMon(struct Pokemon *mon, u16 species, u8 level, u8 fixedIV, u8 hasFixedPersonality, u32 fixedPersonality, u8 otIdType, u32 fixedOtId);
void __attribute__((long_call)) CreateMonWithNature(struct Pokemon *mon, u16 species, u8 level, u8 fixedIV, u8 nature);
void __attribute__((long_call)) CreateMonWithGenderNatureLetter(struct Pokemon *mon, u16 species, u8 level, u8 fixedIV, u8 gender, u8 nature, u8 unownLetter);
void __attribute__((long_call)) CalculateMonStats(pokemon_t *mon);
u8 __attribute__((long_call)) GetMonGender(pokemon_t* mon);
u8 __attribute__((long_call)) GetBoxMonGender(struct BoxPokemon* boxMon);
u8 __attribute__((long_call)) GetGenderFromSpeciesAndPersonality(u16 species, u32 personality);

//u8 __attribute__((long_call)) SendMonToPC(struct Pokemon* mon);
u8 __attribute__((long_call)) GetPartyAbility(const pokemon_t* const);
//u8 __attribute__((long_call)) CalculatePlayerPartyCount(void);
u8 __attribute__((long_call)) CalculatePPWithBonus(u16 move, u8 ppBonuses, u8 moveIndex);
u16 __attribute__((long_call)) SpeciesToNationalPokedexNum(u16 species);
//void __attribute__((long_call)) MonGainEVs(struct Pokemon *mon, u16 defeatedSpecies);
s8 __attribute__((long_call)) GetPokeFlavourRelation(u32 pid, u8 flavor);
bool8 __attribute__((long_call)) IsTradedMon(struct Pokemon *mon);
bool8 __attribute__((long_call)) IsOtherTrainer(u32 otId, u8* otName);
void __attribute__((long_call)) MonRestorePP(struct Pokemon *mon);
bool8 __attribute__((long_call)) GetSetPokedexFlag(u16 nationalNum, u8 caseID);
u16 __attribute__((long_call)) GetPokedexHeightWeight(u16 dexNum, u8 data);
void __attribute__((long_call)) ReducePartyToThree(void);
u8 __attribute__((long_call)) pokemon_order_func(u8 a);
bool8 __attribute__((long_call)) GetIndexFromDexFlag(u16 index, u8 dexFlag);
u8 __attribute__((long_call)) GetEggMoves(pokemon_t* poke, void* storageAddr);
void __attribute__((long_call)) Special_0DD_DeleteMove();
u8 __attribute__((long_call)) NicknameMalloc(u8 a1, void *name, u16 species, u8 gender, u16 PID, void (*callback));

//u8 __attribute__((long_call)) GetWildDataIndexForMap(void);

void __attribute__((long_call)) BufferPokeNameSize(u16 species, void* varAddress);
u8 __attribute__((long_call)) CalculateHeight(u16 species, void* varAddress);
void __attribute__((long_call)) CreateMaleMon(pokemon_t* poke_address, u16 species, u8 level);
//u32 __attribute__((long_call)) GetAttrFromAnyBox(u8 boxId, u8 boxSlot, u8 dataRequest);
//void __attribute__((long_call)) SetAttrFromAnyBox(u8 boxId, u8 boxSlot, u8 dataRequest, void* dataAddr);
u16 __attribute__((long_call)) GetBaseSpecies(u16 species);

/*Overworld Functions*/
u8 __attribute__((long_call)) GetTrainerEncounterMusicId(u16 trainerOpponentId);
u8 __attribute__((long_call)) GetCurrentMapBattleScene(void);

void __attribute__((long_call)) PlayerGetDestCoords(s16* x, s16* y);
u8 __attribute__((long_call)) TestPlayerAvatarFlags(u8 a);

void __attribute__((long_call)) ScriptContext2_Enable(void);
void __attribute__((long_call)) ScriptContext1_Stop(void);
void __attribute__((long_call)) PlayNewMapMusic(u16 musicId);

//Quest Log
void __attribute__((long_call)) QuestLogRemtachBattleStore(void);
bool8 __attribute__((long_call)) IsQuestLogActive(void);

void __attribute__((long_call)) SyncTilemaps(void);
void __attribute__((long_call)) SetVBlankCallback(void (*callback));
void __attribute__((long_call)) SetHBlankCallback(void (*callback));

void __attribute__((long_call)) OverworldCallback2(void);

void __attribute__((long_call)) BuildOAMBuffer(void);
void __attribute__((long_call)) AnimateSprites(void);
void __attribute__((long_call)) BgIdSetTilemap(u8 layer, u8* space);
void __attribute__((long_call)) BgIdMarkForSync(u8 bgid);

//Other Functions
void* __attribute__((long_call)) Malloc(u32 size);
void* __attribute__((long_call)) Calloc(u32 size);
void __attribute__((long_call)) Free(void *pointer);

void __attribute__((long_call)) CleanupOverworldWindowsAndTilemaps(void);
void __attribute__((long_call)) ResetBgsAndClearDma3BusyFlags(u8 bg);	// in bg.h

int __attribute__((long_call)) CountTrailingZeroBits(u32 value);
void __attribute__((long_call)) IncrementGameStat(u8 gameStat);
void __attribute__((long_call)) nullsub_8(void);
s16 __attribute__((long_call)) Sine(s16 index, s16 amplitude);
//u16 __attribute__((long_call)) Random();

void __attribute__((long_call)) PlayCry3(u16 species, s8 pan, u8 mode);
void __attribute__((long_call)) PlayBGM(u16 songNum);
void __attribute__((long_call)) PlaySE(u16 songNum);

u8* __attribute__((long_call)) ReturnEmptyStringIfNull(const u8* string);
void __attribute__((long_call)) DisplayItemMessage(u8 taskId, u8 a, const u8* str, void(*callback)(u8 taskId));
void __attribute__((long_call)) bag_menu_inits_lists_menu(u8 taskId);
void __attribute__((long_call)) AddTextPrinterParameterized4(u8 windowId, u8 fontId, u8 left, u8 top, u8 letterSpacing, u8 lineSpacing, const u8 *color, s8 speed, const u8 *str);
void __attribute__((long_call)) CpuSet(const void* src, void* dest, u32 mode);
void __attribute__((long_call)) CpuFastSet(const void* src, void* dest, u32 mode);
u16 __attribute__((long_call)) Sqrt(u32);
u32 __attribute__((long_call)) udivsi(u32, u32);
u32 __attribute__((long_call)) umodsi(u32, u32);

void __attribute__((long_call)) ReturnToFieldContinueScriptPlayMapMusic(void);

void __attribute__((long_call)) PatchObjectPalette(u16 PalTag, u8 PalSlot);

void __attribute__((long_call)) LogCoordsCameraRelative(s32* x, s32* y, u8 size_x, u8 size_y);
void __attribute__((long_call)) LoadOAM(void);
void __attribute__((long_call)) ProcessSpriteCopyRequests(void);
// Textbox Stuff
void __attribute__((long_call)) TextboxClose(void);
void __attribute__((long_call)) TextboxFdecodeAutoAndCreateTask(const u8* text);
void __attribute__((long_call)) DeactivateAllTextPrinters(void);

// Start Menu
void __attribute__((long_call)) CloseSafariStepsBox(void);
void __attribute__((long_call)) CloseStartMenuDescriptionBox(void);
void __attribute__((long_call)) HideStartMenu(void);
void __attribute__((long_call)) StartMenuPokedexFunc(void);

void __attribute__((long_call)) MallocInit(void* memStart, u32 memSize);
void __attribute__((long_call)) EnableInterrupts(u8 disable);
void __attribute__((long_call)) DisableInterrupts(u8 disable);


void* __attribute__((long_call)) LoadPartyIconTiles(u16 species, u32 pid, bool8 isDeoxys);

u8 __attribute__((long_call)) CurrMapHeightMismatch(u8 height, s16 x, s16 y);

void __attribute__((long_call)) MakeExclamationMark(struct MapObject* npc, struct Sprite* s);

u16 __attribute__((long_call)) GetCombinedOTID(void);

void __attribute__((long_call)) PicBoxSetter(void);
bool8 __attribute__((long_call)) BoxCreateTask(void* func);
void __attribute__((long_call)) TaskDestroyPicBox(u8 taskId);
u8 __attribute__((long_call)) PicBoxWindow(u16 x, u16 y, u8, u8);

u32 __attribute__((long_call)) GetGameStat(u8 index);
u8* __attribute__((long_call)) StringExpandPlaceholders(u8 *dest, const u8 *src);
u16 __attribute__((long_call)) SpeciesToPokedexNum(u16 species);

//The Deal Breaker
void __attribute__((long_call)) break_func(u32);
//u32 break_helper(u32 a);
