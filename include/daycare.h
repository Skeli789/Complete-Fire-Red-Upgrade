#pragma once

#include "global.h"

//Parent compatibility scores
#define PARENTS_INCOMPATIBLE       0
#define PARENTS_LOW_COMPATIBILITY  20
#define PARENTS_MED_COMPATIBILITY  50
#define PARENTS_MAX_COMPATIBILITY  70

u16 __attribute__((long_call)) GetEggSpecies(u16 species);
//u16 __attribute__((long_call)) DetermineEggSpeciesAndParentSlots(struct DayCare *daycare, u8 *parentSlots);
//void __attribute__((long_call)) AlterEggSpeciesWithIncenseItem(u16 *species, struct DayCare *daycare);
//void __attribute__((long_call)) SetInitialEggData(struct Pokemon *mon, u16 species, struct DayCare *daycare);
void __attribute__((long_call)) RemoveEggFromDayCare(struct DayCare *daycare);
void __attribute__((long_call)) RemoveIVIndexFromList(u8 *ivs, u8 selectedIv);
bool8 __attribute__((long_call)) ShouldEggHatch(void);
//u8 __attribute__((long_call)) GetDaycareCompatibilityScore(struct DayCare *daycare);
u8 __attribute__((long_call)) GetNumLevelsGainedForDaycareMon(struct DaycareMon *daycareMon);
bool8 __attribute__((long_call)) EggGroupsOverlap(u16 *eggGroups1, u16 *eggGroups2);

/*
#define EGG_HATCH_LEVEL 5

u8 *GetMonNick(struct Pokemon *mon, u8 *dest);
u8 *GetBoxMonNick(struct BoxPokemon *mon, u8 *dest);
u8 CountPokemonInDaycare(struct DayCare *daycare);
void InitDaycareMailRecordMixing(struct DayCare *daycare, struct RecordMixingDayCareMail *daycareMail);
void StoreSelectedPokemonInDaycare(void);
u16 TakePokemonFromDaycare(void);
void GetDaycareCost(void);
u8 GetNumLevelsGainedFromDaycare(void);
void TriggerPendingDaycareEgg(void);
void RejectEggFromDayCare(void);
void CreateEgg(struct Pokemon *mon, u16 species, bool8 setHotSpringsLocation);
void GiveEggFromDaycare(void);
u16 GetSelectedMonNickAndSpecies(void);
void GetDaycareMonNicknames(void);
u8 GetDaycareState(void);
void SetDaycareCompatibilityString(void);
bool8 NameHasGenderSymbol(const u8 *name, u8 genderRatio);
void ShowDaycareLevelMenu(void);
void ChooseSendDaycareMon(void);
*/
