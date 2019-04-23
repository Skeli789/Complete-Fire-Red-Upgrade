#ifndef GUARD_HALL_OF_FAME_H
#define GUARD_HALL_OF_FAME_H

#include "global.h"
#include "pokemon.h"

#define HALL_OF_FAME_MAX_TEAMS 40
#define GAME_STAT_ENTERED_HOF 10

/*
struct __attribute__((packed, aligned(4))) HallofFameMon {
    u32 tid;
    u32 personality;
    u16 species;
    u8 lvl;
    u8 nick[POKEMON_NAME_LENGTH];
};

struct __attribute__((packed)) HallofFameTeam {
    struct HallofFameMon mon[PARTY_SIZE];
};

//_Static_assert(sizeof(struct HallofFameTeam) * HALL_OF_FAME_MAX_TEAMS <= 0x1770, "");
*/



#define gText_PickCancel ((void*)  0x08415D78)
#define gText_PickNextCancel ((void*) 0x08415D60)
#define gText_HOFNumber ((void*) 0x084160B4)
#define gText_Number ((void*) 0x084160EC)
#define gText_Level ((void*) 0x084160F4)
#define gText_IDNumber ((void*) 0x08416104)
#define gText_SavingDontTurnOffPower ((void*) 0x08419F54)
#define sUnknown_0840C23C ((void*) 0x0840C23C)

extern const s16 sHallOfFame_MonFullTeamPositions[6][4];
extern const s16 sHallOfFame_MonHalfTeamPositions[3][4];

/*
typedef s16 FullTeamPositions[6][4];
typedef s16 HalfTeamPositions[3][4];
#define HallofFameMonFullTeamPositions (*FullTeamPositions 0x0840C268)
#define HallofFameMonHalfTeamPositions (*HalfTeamPositions 0x0840C298)
*/


struct __attribute__((packed, aligned(4))) HallofFameMon {
    u32 tid;
    u32 personality;
    u16 species;
    u8 lvl;
    u8 nick[POKEMON_NAME_LENGTH];
};

struct __attribute__((packed)) HallofFameTeam {
	struct HallofFameMon mon[PARTY_SIZE];
};

//extern struct HallofFameTeam* sHofMonPtr;


void CB2_DoHallOfFameScreen(void);
void CB2_DoHallOfFameScreenDontSaveData(void);
void CB2_DoHallOfFamePC(void);
void sub_8175280(void);

void __attribute__((long_call)) sub_8112450(void);
bool8 __attribute__((long_call)) InitHallOfFameScreen(void);
void __attribute__((long_call)) Task_Hof_SetMonDisplayTask(u8 taskId);
void __attribute__((long_call)) Task_Hof_TrySaveData(u8 taskId);
void __attribute__((long_call)) Task_Hof_PaletteFadeAndPrintWelcomeText(u8 taskId);
u8 __attribute__((long_call)) HofPC_CreateWindow(u8 bg, u8 xPos, u8 yPos, u8 palette, u16 baseTile);
void __attribute__((long_call)) Task_HofPC_PrintDataIsCorrupted(u8 taskId);
void __attribute__((long_call)) HofPC_PutText(const u8 *string, const u8 *string2, u8 a3, u8 a4, bool8 copyToVram);
void __attribute__((long_call)) Task_HofPC_HandleInput(u8 taskId);
void __attribute__((long_call)) SpriteCB_GetOnScreenAndAnimate(struct Sprite *sprite);
void __attribute__((long_call)) NewMenuHelpers_DrawDialogueFrame(u8, u8);
u8 __attribute__((long_call)) Save_LoadGameData(u8 a1);


#endif // GUARD_HALL_OF_FAME_H
