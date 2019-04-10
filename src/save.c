#include "defines.h"
#include "../include/save.h"
#include "../include/constants/vars.h"

//extern void dprintf(const char * str, ...);

// yes that's var 0x8000. It will be used for save index tracking.
u8* ParasiteSizeIndex = (u8*)0x020370B8;

// Each 4 KiB flash sector contains 3968 bytes of actual data followed by a 128 byte footer
#define SECTOR_DATA_SIZE 0xFF0
#define SECTOR_FOOTER_SIZE 128
#define NUM_SECTORS_PER_SAVE_SLOT 14
#define PLAYER_NAME_LENGTH 8
#define FILE_SIGNATURE 0x08012025
#define gSaveBlockParasite 0x0203B174
#define parasiteSize 0xEC4

// old 0x080DA23C table changes
struct SaveSectionOffset SaveSectionOffsets[] = {
    {SECTOR_DATA_SIZE * 0, 0xF24}, // saveblock2
    // 0xCC byes saved

    {SECTOR_DATA_SIZE * 0, SECTOR_DATA_SIZE}, // saveblock 1
    {SECTOR_DATA_SIZE * 1, SECTOR_DATA_SIZE}, // saveblock 1
    {SECTOR_DATA_SIZE * 2, SECTOR_DATA_SIZE}, // saveblock 1
    {SECTOR_DATA_SIZE * 3, 0xD98}, // saveblock 1
    // 0x258 bytes saved

    {SECTOR_DATA_SIZE * 0, SECTOR_DATA_SIZE}, // saveblock Pokemonstorage
    {SECTOR_DATA_SIZE * 1, SECTOR_DATA_SIZE},
    {SECTOR_DATA_SIZE * 2, SECTOR_DATA_SIZE},
    {SECTOR_DATA_SIZE * 3, SECTOR_DATA_SIZE},
    {SECTOR_DATA_SIZE * 4, SECTOR_DATA_SIZE},
    {SECTOR_DATA_SIZE * 5, SECTOR_DATA_SIZE},
    {SECTOR_DATA_SIZE * 6, SECTOR_DATA_SIZE},
    {SECTOR_DATA_SIZE * 7, SECTOR_DATA_SIZE},
    {0x7F80, 0x450},
    // 0xBA0 saved
};

/* Any save sector that isn't full, we'll plop our data of these sizes in there */
u16 SaveBlockParasiteSizes[3] = {
        SECTOR_DATA_SIZE - 0xF24, // 0xCC
        SECTOR_DATA_SIZE - 0xD98, // 0x258
        SECTOR_DATA_SIZE - 0x450, // 0xBA0
};


/* Saving and loading for sector 30 and 31. Could potentially add the Hall of fame sectors too */
void loadSector30And31() {
    struct SaveSection* saveBuffer = (struct SaveSection*)0x02039A38;
    Memset(saveBuffer, 0, sizeof(struct SaveSection));
    DoReadFlashWholeSection(30, saveBuffer);
    u32 startLoc = gSaveBlockParasite + parasiteSize;
    Memcpy((void*)(startLoc), saveBuffer, SECTOR_DATA_SIZE);

    Memset(saveBuffer, 0, sizeof(struct SaveSection));
    DoReadFlashWholeSection(31, saveBuffer);
    startLoc += SECTOR_DATA_SIZE;
	#ifdef UNBOUND
    Memcpy((void*)(startLoc), saveBuffer, 0xBD8);
	#else
	Memcpy((void*)(startLoc), saveBuffer, SECTOR_DATA_SIZE);
	#endif
}

void saveSector30And31() {
    struct SaveSection* saveBuffer = (struct SaveSection*)0x02039A38;
    Memset(saveBuffer, 0, sizeof(struct SaveSection));
    u32 startLoc = gSaveBlockParasite + parasiteSize;
    Memcpy(saveBuffer->data, (void*)(startLoc), SECTOR_DATA_SIZE);
    TryWriteSector(30, saveBuffer->data);
    /* Write sector 31 */
    Memset(saveBuffer, 0, sizeof(struct SaveSection));
    startLoc += SECTOR_DATA_SIZE;
    Memcpy(saveBuffer->data, (void*)(startLoc), SECTOR_DATA_SIZE);
    TryWriteSector(31, saveBuffer->data);
}


/* This parasitic saveblock idea originated from JPAN's work. Frees up 0xEC4 bytes - almost a sector */
void saveParasite() {
    struct SaveSection* s = gFastSaveSection;
    u32 size = 0;
    u32* data = NULL;
    u32* parasiteP1 = (u32*)gSaveBlockParasite;
    u32* parasiteP2 = (u32*)(gSaveBlockParasite + SaveBlockParasiteSizes[0]);
    u32* parasiteP3 = (u32*)(gSaveBlockParasite + SaveBlockParasiteSizes[0] + SaveBlockParasiteSizes[1]);

    switch (s->id) {
        case 0:
            data = parasiteP1;
            size = SaveBlockParasiteSizes[0];
            break;
        case 4:
            data = parasiteP2;
            size = SaveBlockParasiteSizes[1];
            break;
        case 13:
            data = parasiteP3;
            size = SaveBlockParasiteSizes[2];
            break;
        default:
            return;
    };
    u16 index = SECTOR_DATA_SIZE - size;
    Memcpy(&(s->data[index]), (u32*)data, size);
}


void loadParasite() {
    struct SaveSection* s = gFastSaveSection;
    u32 size = 0;
    u32* data = NULL;
    u32* parasiteP1 = (u32*)gSaveBlockParasite;
    u32* parasiteP2 = (u32*)(gSaveBlockParasite + SaveBlockParasiteSizes[0]); //b240
    u32* parasiteP3 = (u32*)(gSaveBlockParasite + SaveBlockParasiteSizes[0] + SaveBlockParasiteSizes[1]); //b496
    switch (s->id) {
        case 0:
            data = parasiteP1;
            size = SaveBlockParasiteSizes[0];
            break;
        case 4:
            data = parasiteP2;
            size = SaveBlockParasiteSizes[1];
            break;
        case 13:
            data = parasiteP3;
            size = SaveBlockParasiteSizes[2];
            break;
        default:
            return;
    };
    u16 index = SECTOR_DATA_SIZE - size;
    Memcpy(data, &(s->data[index]), size);
}

// 080D9E54
u8 HandleLoadSector(u16 a1, const struct SaveBlockChunk *chunks) {
	++a1; //So the compiler doesn't complain
	
    struct SaveSection* saveSection = gFastSaveSection;
    u16 sector = NUM_SECTORS_PER_SAVE_SLOT * ((*gSaveCounter) % 2);
    bool8 checksum_status = FALSE;
    for (u8 i = 0; i < NUM_SECTORS_PER_SAVE_SLOT; i++) {
        DoReadFlashWholeSection(i + sector, saveSection);
        u16 id = saveSection->id;
        if (id == 0)
            *gFirstSaveSector = i;
        u16 checksum = CalculateChecksum(saveSection->data, chunks[id].size);
        checksum_status = checksum == saveSection->checksum;
        checksum_status = checksum && saveSection->signature == FILE_SIGNATURE;
        if (checksum_status) {
            Memcpy(chunks[id].data, saveSection->data, chunks[id].size);
            loadParasite();
        }
    }
    // start of the game don't load jibberish into block
    //dprintf("checksum status is %d\n", checksum_status);
    if (checksum_status)
        loadSector30And31();
    return 1;
}

// 080D9870
u8 HandleWriteSector(u16 chunkId, const struct SaveBlockChunk *chunks) {
    struct SaveSection* saveSection = gFastSaveSection;
    // get nonbackup sector slot
    u16 sectorNum = chunkId + *gFirstSaveSector;
    sectorNum %= NUM_SECTORS_PER_SAVE_SLOT;
    sectorNum += NUM_SECTORS_PER_SAVE_SLOT * ((*gSaveCounter) % 2);
    u8* chunkData = chunks[chunkId].data;
    u16 chunkSize = chunks[chunkId].size;
    /* write to save section */
    // clear
    Memset(saveSection, 0, sizeof(struct SaveSection));
    saveSection->id = chunkId;
    saveSection->signature = FILE_SIGNATURE;
    saveSection->counter = (*gSaveCounter);
    Memcpy(saveSection->data, chunkData, chunkSize);
    saveSection->checksum = CalculateChecksum(chunkData, chunkSize);
    // write data to leftover save section
    saveParasite();
    u8 retVal = TryWriteSector(sectorNum, saveSection->data);
    saveSector30And31();
    return retVal;
}

void call_something(u16 arg, EraseFlash func) {
    func(arg);
}


u8 HandleSavingData(u8 saveType) {
    u32 *backupPtr = gMain.vblankCounter1;
    //u8 *tempAddr;
    gMain.vblankCounter1 = NULL;
    UpdateSaveAddresses();
    switch (saveType) {
            // fallthrough
        case SAVE_NORMAL: // normal save. also called by overwriting your own save.
        default:
        {
            SaveSerializedGame();
            save_write_to_flash(0xFFFF, gRamSaveSectionLocations);
            break;
        }
        case SAVE_LINK: // _081532C4
        {
            SaveSerializedGame();
            for(u8 i = 0; i < 5; i++) {
                save_write_to_flash(i, gRamSaveSectionLocations);
            }
            break;
        }
        case SAVE_OVERWRITE_DIFFERENT_FILE:
        {
            for (u8 i = (14 * 2 + 0); i < 32; i++) {
                u32 *t = (u32*)0x03007430;
                EraseFlash EraseFlashSector = (EraseFlash)(*t);
                call_something(i, EraseFlashSector);
            }
            SaveSerializedGame();
            save_write_to_flash(0xFFFF, gRamSaveSectionLocations);
            break;
        }
    };
    gMain.vblankCounter1 = backupPtr;
    return 0;
}

u8* GetExpandedFlagPointer(u16 id)
{
	#ifdef SAVE_BLOCK_EXPANSION
		if (id >= 0x900 && id < 0x1900)
			return &gExpandedFlags[(id - 0x900) / 8];
		else if (id >= 0x1900)
			return (u8*) &Var8000; //Returns Var8000 to prevent bad memory access
		else
			return NULL;
	#else
		if (id >= 0x1900)
			return (u8*) &Var8000; //Returns Var8000 to prevent bad memory access
		else
			return NULL;
	#endif
}

u16* GetExpandedVarPointer(u16 id)
{
	#ifdef SAVE_BLOCK_EXPANSION
		if (id >= 0x5000 && id < 0x5200)
			return &gExpandedVars[id - 0x5000];
		else if (id >= VARS_START + 0x100 && id < SPECIAL_VARS_START)
			return (u16*) 1; //Indicates to return NULL
		else
			return NULL;
	#else
		if (id >= VARS_START + 0x100 && id < SPECIAL_VARS_START)
			return (u16*) 1; //Indicates to return NULL
		else
			return NULL;
	#endif
	
}