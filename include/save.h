#pragma once

#include "global.h"

#define NUM_SECTORS 32 // defined in agb_flash but not in a header

#define SAVE_STATUS_EMPTY 0
#define SAVE_STATUS_OK 1
#define SAVE_STATUS_INVALID  2
#define SAVE_STATUS_NO_FLASH 4
#define SAVE_STATUS_ERROR 0xFF

enum
{
    SAVE_NORMAL,
    SAVE_LINK,
    SAVE_EREADER,
    SAVE_HALL_OF_FAME,
    SAVE_OVERWRITE_DIFFERENT_FILE,
    SAVE_HALL_OF_FAME_ERASE_BEFORE, //Unused
};

struct SaveBlockChunk
{
    void* data;
    u16 size;
};

struct SaveSection
{
    u8 data[0xFF4];
    u16 id;
    u16 checksum;
    u32 signature;
    u32 counter;
}; // size is 0x1000

// headless save section?
struct UnkSaveSection
{
    u8 data[0xFF4];
    u32 signature;
}; // size is 0xFF8

struct SaveSectionOffsets
{
    u16 toAdd;
    u16 size;
};

extern struct SaveBlockChunk gRamSaveSectionLocations[0xE];

#define UNKNOWN_CHECK_VALUE 0x8012025

#define NUM_SECTORS_PER_SAVE_SLOT 14  // Number of sectors occupied by a save slot
#define SECTOR_SAVE1(n)  (n)
#define SECTOR_SAVE2(n)  ((n) + NUM_SECTORS_PER_SAVE_SLOT)
#define SECTOR_ID_HOF_1 28
#define SECTOR_ID_HOF_2 29
#define SECTORS_COUNT 32

// SetSectorDamagedStatus states
enum
{
    ENABLE,
    DISABLE,
    CHECK // unused
};

// Do save types
enum
{
    NORMAL_SAVE,
    LINK_SAVE,
    EREADER_SAVE,
    HOF_SAVE,
    DIFFERENT_FILE_SAVE,
    HOF_DELETE_SAVE // unused
};

extern u16 gFirstSaveSector;
extern u32 gPrevSaveCounter;
extern u16 gLastKnownGoodSector;
extern u32 gDamagedSaveSectors;
extern u32 gSaveCounter;
extern struct SaveSection* gFastSaveSection; // the pointer is in fast IWRAM but may sometimes point to the slower EWRAM.
extern u16 gUnknown_3005398;
extern u16 gSaveUnusedVar;
extern u16 gSaveFileStatus;
extern void (*gGameContinueCallback)(void);
extern struct SaveBlockChunk gRamSaveSectionLocations[0xE];
extern u16 gSaveSucceeded;

extern struct SaveSection gSaveDataBuffer;

void __attribute__((long_call)) ClearSaveData(void);
void __attribute__((long_call)) SaveSerializedGame(void);
void __attribute__((long_call)) UpdateSaveAddresses(void);
u16 __attribute__((long_call)) CalculateSaveChecksum(void* data, u16 size);
u8 __attribute__((long_call)) TryWriteSector(u8 sector, u8 *data);
u8 SaveWriteToFlash(u16 chunkId, const struct SaveBlockChunk *chunks);
u8 __attribute__((long_call)) DoReadFlashWholeSection(u8 sector, struct SaveSection *section);
u8 __attribute__((long_call)) Save_LoadGameData(u8 a1);
u8 __attribute__((long_call)) TrySavingData(u8 saveType);
u8 __attribute__((long_call)) HandleWriteSectorNBytes(u8 sector, u8 *data, u16 size);

/*
void Save_ResetSaveCounters(void);
bool32 SetSectorDamagedStatus(u8 op, u8 bit);
u8 HandleWriteSector(u16 a1, const struct SaveBlockChunk *location);
u8 HandleWriteSectorNBytes(u8 sector, u8 *data, u16 size);
u32 RestoreSaveBackupVarsAndIncrement(const struct SaveBlockChunk *location);
u32 RestoreSaveBackupVars(const struct SaveBlockChunk *location);
u8 sub_80D9AA4(u16 a1, const struct SaveBlockChunk *location);
u8 sub_80D9B04(u16 a1, const struct SaveBlockChunk *location);
u8 ClearSaveData_2(u16 a1, const struct SaveBlockChunk *location);
u8 sav12_xor_get(u16 a1, const struct SaveBlockChunk *location);
u8 sub_80D9D88(u16 a1, const struct SaveBlockChunk *location);
u8 sub_80D9E14(u16 a1, const struct SaveBlockChunk *location);
u8 sub_80D9E54(u16 a1, const struct SaveBlockChunk *location);
u8 GetSaveValidStatus(const struct SaveBlockChunk *location);
u8 sub_80DA120(u8 a1, u8 *data, u16 size);
u8 HandleSavingData(u8 saveType);
u8 sub_80DA3AC(void);
bool8 sub_80DA3D8(void);
u8 sub_80DA40C(void);
u8 sub_80DA434(void);
u8 sub_80DA45C(void);
bool8 sub_80DA4A0(void);
u8 Save_LoadGameData(u8 a1);
u32 TryCopySpecialSaveSection(u8 sector, u8* dst);
u32 sub_8153634(u8 sector, u8* src);
void sub_8153688(u8 taskId);
u32 sub_80DA5E0(u8 sector, u8* src);
void sub_80DA634(u8 taskId);
*/
