#include "defines.h"
#include "../include/load_save.h"
#include "../include/menu_helpers.h"
#include "../include/rtc.h"
#include "../include/save.h"
#include "../include/string_util.h"
#include "../include/constants/vars.h"

#include "../include/new/dns.h"
#include "../include/new/save.h"
#include "../include/new/ram_locs_battle.h"
/*
save.c
	handles save block expansion functions/structures
*/

// Each 4 KiB flash sector contains 3968 bytes of actual data followed by a 128 byte footer
extern struct SaveSection gSaveDataBuffer;
#define SECTOR_DATA_SIZE 0xFF0
#define SECTOR_FOOTER_SIZE 128
#define NUM_SECTORS_PER_SAVE_SLOT 14
#define FILE_SIGNATURE 0x08012025
#define SAVE_BLOCK_PARASITE 0x0203B174
#define PARASITE_SIZE 0xEC4

// old 0x080DA23C table changes
const struct SaveSectionOffsets gSaveSectionOffsets[] =
{
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
static const u16 sSaveBlockParasiteSizes[3] =
{
	SECTOR_DATA_SIZE - 0xF24, // 0xCC
	SECTOR_DATA_SIZE - 0xD98, // 0x258
	SECTOR_DATA_SIZE - 0x450, // 0xBA0
};

//Old vanilla functions:
void __attribute__((long_call)) PrintSaveErrorStatus(u8 taskId, const u8 *str);

//This file's functions:
static bool8 IsValidFileSignature(u32 signature);
static void LoadSector30And31(void);
static u8 SaveSector30And31(void);
static void LoadParasite(void);
extern bool8 TryPreventIncompleteSaves(u8 taskId);

static bool8 IsValidFileSignature(u32 signature)
{
	return signature == FILE_SIGNATURE
		#ifdef CUSTOM_FILE_SIGNATURE_OLD
		|| signature == CUSTOM_FILE_SIGNATURE_OLD
		#endif
		#ifdef CUSTOM_FILE_SIGNATURE 
		|| signature == CUSTOM_FILE_SIGNATURE
		#endif
		;
}

//Saving and loading for sector 30 and 31.
static void LoadSector30And31(void)
{
	struct SaveSection* saveBuffer = &gSaveDataBuffer;

	//Load sector 30
	u32 startLoc = SAVE_BLOCK_PARASITE + PARASITE_SIZE;
	Memset(saveBuffer, 0, sizeof(struct SaveSection));
	DoReadFlashWholeSection(30, saveBuffer);
	Memcpy((void*)(startLoc), saveBuffer, SECTOR_DATA_SIZE);

	//Load sector 31
	startLoc += SECTOR_DATA_SIZE;
	Memset(saveBuffer, 0, sizeof(struct SaveSection));
	DoReadFlashWholeSection(31, saveBuffer);
	Memcpy((void*) startLoc, saveBuffer, SECTOR_DATA_SIZE);
}

static u8 SaveSector30And31(void)
{
	u8 retVal;
	struct SaveSection* saveBuffer = &gSaveDataBuffer;

	//Write sector 30
	Memset(saveBuffer, 0, sizeof(struct SaveSection));
	u32 startLoc = SAVE_BLOCK_PARASITE + PARASITE_SIZE;
	Memcpy(saveBuffer->data, (void*)(startLoc), SECTOR_DATA_SIZE);
	retVal = TryWriteSector(30, saveBuffer->data);
	if (retVal != SAVE_STATUS_OK)
		return retVal; //Error

	//Write sector 31
	Memset(saveBuffer, 0, sizeof(struct SaveSection));
	startLoc += SECTOR_DATA_SIZE;
	Memcpy(saveBuffer->data, (void*)(startLoc), SECTOR_DATA_SIZE);
	return TryWriteSector(31, saveBuffer->data);
}

//This parasitic saveblock idea originated from JPAN's work. Frees up 0xEC4 bytes - almost a sector
void SaveParasite(void)
{
	struct SaveSection* sector = gFastSaveSection;
	u32 size = 0;
	u32* data = NULL;
	u32* parasiteP1 = (u32*) SAVE_BLOCK_PARASITE;
	u32* parasiteP2 = (u32*) (SAVE_BLOCK_PARASITE + sSaveBlockParasiteSizes[0]);
	u32* parasiteP3 = (u32*) (SAVE_BLOCK_PARASITE + sSaveBlockParasiteSizes[0] + sSaveBlockParasiteSizes[1]);

	switch (sector->id) {
		case 0:
			data = parasiteP1;
			size = sSaveBlockParasiteSizes[0];
			break;
		case 4:
			data = parasiteP2;
			size = sSaveBlockParasiteSizes[1];
			break;
		case 13:
			data = parasiteP3;
			size = sSaveBlockParasiteSizes[2];
			break;
		default:
			return;
	}

	u16 index = SECTOR_DATA_SIZE - size;
	Memcpy(&sector->data[index], (u32*) data, size);
}

static void LoadParasite(void)
{
	struct SaveSection* sector = gFastSaveSection;
	u32 size = 0;
	u32* data = NULL;
	u32* parasiteP1 = (u32*) SAVE_BLOCK_PARASITE;
	u32* parasiteP2 = (u32*) (SAVE_BLOCK_PARASITE + sSaveBlockParasiteSizes[0]); //b240
	u32* parasiteP3 = (u32*) (SAVE_BLOCK_PARASITE + sSaveBlockParasiteSizes[0] + sSaveBlockParasiteSizes[1]); //b496

	switch (sector->id) {
		case 0:
			data = parasiteP1;
			size = sSaveBlockParasiteSizes[0];
			break;
		case 4:
			data = parasiteP2;
			size = sSaveBlockParasiteSizes[1];
			break;
		case 13:
			data = parasiteP3;
			size = sSaveBlockParasiteSizes[2];
			break;
		default:
			return;
	}

	u16 index = SECTOR_DATA_SIZE - size;
	Memcpy(data, &sector->data[index], size);
}

// 080D9E54
u8 HandleLoadSector(unusedArg u16 a1, const struct SaveBlockChunk* location)
{
	u16 id;
	u16 sector = NUM_SECTORS_PER_SAVE_SLOT * (gSaveCounter % 2);
	bool8 checksumStatus = FALSE;

	for (u8 i = 0; i < NUM_SECTORS_PER_SAVE_SLOT; ++i)
	{
		DoReadFlashWholeSection(i + sector, gFastSaveSection);

		id = gFastSaveSection->id;
		if (id == 0)
			gFirstSaveSector = i;

		u16 checksum = CalculateSaveChecksum(gFastSaveSection->data, location[id].size);
		if (IsValidFileSignature(gFastSaveSection->signature)
		&&  gFastSaveSection->checksum == checksum)
		{
			Memcpy(location[id].data, gFastSaveSection->data, location[id].size);
			LoadParasite();
			checksumStatus = TRUE;
		}
	}

	//Start of the game don't load jibberish into block
	if (checksumStatus)
		LoadSector30And31();

	return 1;
}

// 080DA120
u8 TryLoadSaveSector(u8 sector, u8* data, u16 size)
{
	u16 i;
	struct SaveSection* section = &gSaveDataBuffer;

	DoReadFlashWholeSection(sector, section);
	if (IsValidFileSignature(section->signature))
	{
		u16 checksum = CalculateSaveChecksum(section->data, size);
		if (section->id == checksum)
		{
			for (i = 0; i < size; i++)
				data[i] = section->data[i];
			return SAVE_STATUS_OK;
		}
		else
		{
			return SAVE_STATUS_INVALID;
		}
	}
	else
	{
		return SAVE_STATUS_EMPTY;
	}
}

u8 GetSaveValidStatus(const struct SaveBlockChunk *chunks)
{
	u16 sector;
	bool8 signatureValid;
	u16 checksum;
	s32 slot1saveCounter = 0;
	s32 slot2saveCounter = 0;
	u8 slot1Status;
	u8 slot2Status;
	u32 validSectors;
	const u32 ALL_SECTORS = (1 << NUM_SECTORS_PER_SAVE_SLOT) - 1;  // bitmask of all saveblock sectors

	//Check save slot 1.
	validSectors = 0;
	signatureValid = FALSE;
	for (sector = 0; sector < NUM_SECTORS_PER_SAVE_SLOT; sector++)
	{
		DoReadFlashWholeSection(sector, gFastSaveSection);
		if (IsValidFileSignature(gFastSaveSection->signature))
		{
			signatureValid = TRUE;
			checksum = CalculateSaveChecksum(gFastSaveSection->data, chunks[gFastSaveSection->id].size);
			if (gFastSaveSection->checksum == checksum)
			{
				slot1saveCounter = gFastSaveSection->counter;
				validSectors |= 1 << gFastSaveSection->id;
			}
		}
	}

	if (signatureValid)
	{
		if (validSectors == ALL_SECTORS)
			slot1Status = SAVE_STATUS_OK;
		else
			slot1Status = SAVE_STATUS_ERROR;
	}
	else
	{
		slot1Status = SAVE_STATUS_EMPTY;
	}

	//Check save slot 2.
	validSectors = 0;
	signatureValid = FALSE;
	for (sector = 0; sector < NUM_SECTORS_PER_SAVE_SLOT; sector++)
	{
		DoReadFlashWholeSection(NUM_SECTORS_PER_SAVE_SLOT + sector, gFastSaveSection);
		if (IsValidFileSignature(gFastSaveSection->signature))
		{
			signatureValid = TRUE;
			checksum = CalculateSaveChecksum(gFastSaveSection->data, chunks[gFastSaveSection->id].size);
			if (gFastSaveSection->checksum == checksum)
			{
				slot2saveCounter = gFastSaveSection->counter;
				validSectors |= 1 << gFastSaveSection->id;
			}
		}
	}

	if (signatureValid)
	{
		if (validSectors == ALL_SECTORS)
			slot2Status = SAVE_STATUS_OK;
		else
			slot2Status = SAVE_STATUS_ERROR;
	}
	else
	{
		slot2Status = SAVE_STATUS_EMPTY;
	}

	if (slot1Status == SAVE_STATUS_OK && slot2Status == SAVE_STATUS_OK)
	{
		// Choose counter of the most recent save file
		if ((slot1saveCounter == -1 && slot2saveCounter == 0) || (slot1saveCounter == 0 && slot2saveCounter == -1))
		{
			if ((unsigned)(slot1saveCounter + 1) < (unsigned)(slot2saveCounter + 1))
				gSaveCounter = slot2saveCounter;
			else
				gSaveCounter = slot1saveCounter;
		}
		else
		{
			if (slot1saveCounter < slot2saveCounter)
				gSaveCounter = slot2saveCounter;
			else
				gSaveCounter = slot1saveCounter;
		}
		return SAVE_STATUS_OK;
	}

	if (slot1Status == SAVE_STATUS_OK)
	{
		gSaveCounter = slot1saveCounter;
		if (slot2Status == SAVE_STATUS_ERROR)
			return SAVE_STATUS_ERROR;
		else
			return SAVE_STATUS_OK;
	}

	if (slot2Status == SAVE_STATUS_OK)
	{
		gSaveCounter = slot2saveCounter;
		if (slot1Status == SAVE_STATUS_ERROR)
			return SAVE_STATUS_ERROR;
		else
			return SAVE_STATUS_OK;
	}

	if (slot1Status == SAVE_STATUS_EMPTY && slot2Status == SAVE_STATUS_EMPTY)
	{
		gSaveCounter = 0;
		gFirstSaveSector = 0;
		return SAVE_STATUS_EMPTY;
	}

	gSaveCounter = 0;
	gFirstSaveSector = 0;
	return 2;
}

// 080D9870
u8 HandleWriteSector(u16 chunkId, const struct SaveBlockChunk* location)
{
	u16 sectorNum;
	u8* chunkData;
	u16 chunkSize;

	//Get nonbackup sector slot
	sectorNum = chunkId + gFirstSaveSector;
	sectorNum %= NUM_SECTORS_PER_SAVE_SLOT;
	sectorNum += NUM_SECTORS_PER_SAVE_SLOT * (gSaveCounter % 2);

	chunkData = location[chunkId].data;
	chunkSize = location[chunkId].size;

	//Clear save section
	Memset(gFastSaveSection, 0, sizeof(struct SaveSection));

	gFastSaveSection->id = chunkId;
	#ifdef CUSTOM_FILE_SIGNATURE 
	gFastSaveSection->signature = CUSTOM_FILE_SIGNATURE;
	#else
	gFastSaveSection->signature = FILE_SIGNATURE;
	#endif

	gFastSaveSection->counter = gSaveCounter;

	Memcpy(gFastSaveSection->data, chunkData, chunkSize);

	gFastSaveSection->checksum = CalculateSaveChecksum(chunkData, chunkSize);

	//Write data to leftover save section
	SaveParasite();
	u8 retVal = TryWriteSector(sectorNum, gFastSaveSection->data);
	return retVal;
}

// If chunkId is 0xFFFF, this function will write all of the chunks pointed to by 'chunks'.
// Otherwise, it will write a single chunk with the given 'chunkId'.
u8 SaveWriteToFlash(u16 chunkId, const struct SaveBlockChunk *chunks)
{
    u32 retVal;
    u16 i;

    gFastSaveSection = &gSaveDataBuffer;

    if (chunkId != 0xFFFF)  // write single chunk
    {
        retVal = HandleWriteSector(chunkId, chunks);
    }
    else //Write all chunks
    {
        gLastKnownGoodSector = gFirstSaveSector; // backup the current written sector before attempting to write.
        gPrevSaveCounter = gSaveCounter;
        gFirstSaveSector++;
        gFirstSaveSector %= NUM_SECTORS_PER_SAVE_SLOT; // array count save sector locations
        gSaveCounter++;
        retVal = SAVE_STATUS_OK;

        for (i = 0; i < NUM_SECTORS_PER_SAVE_SLOT; i++)
            HandleWriteSector(i, chunks);
	
		SaveSector30And31();

        //Check for any bad sectors
        if (gDamagedSaveSectors != 0) //Skip the damaged sector.
        {
            retVal = SAVE_STATUS_ERROR;
            gFirstSaveSector = gLastKnownGoodSector;
            gSaveCounter = gPrevSaveCounter;
        }
    }

    return retVal;
}

u8 HandleSavingData(u8 saveType)
{
	u32* backupPtr = gMain.vblankCounter1;
	gMain.vblankCounter1 = NULL;
	UpdateSaveAddresses();
	#ifdef VAR_LAST_SAVE
	UpdateTimeInVars(VAR_LAST_SAVE); //Update last saved
	#endif

	switch (saveType) {
		case SAVE_NORMAL: // normal save. also called by overwriting your own save.
		default:
		{
			SaveSerializedGame();
			SaveWriteToFlash(0xFFFF, gRamSaveSectionLocations);
			break;
		}
		case SAVE_LINK: // _081532C4
		{
			SaveSerializedGame();
			for(u8 i = 0; i < 5; i++)
				SaveWriteToFlash(i, gRamSaveSectionLocations);
			break;
		}
		//case SAVE_EREADER:
			//Removed for more save space
		case SAVE_HALL_OF_FAME: // hall of fame.
			if (GetGameStat(GAME_STAT_ENTERED_HOF) < 999)
				IncrementGameStat(GAME_STAT_ENTERED_HOF);
			SaveSerializedGame();
			SaveWriteToFlash(0xFFFF, gRamSaveSectionLocations);
			HandleWriteSectorNBytes(SECTOR_ID_HOF_1, gDecompressionBuffer, 0xF80);
			HandleWriteSectorNBytes(SECTOR_ID_HOF_2, gDecompressionBuffer + 0xF80, 0xF80);
			break;

		case SAVE_OVERWRITE_DIFFERENT_FILE:
		{
			//Clear old HOF data
			for (u8 i = SECTOR_ID_HOF_1; i < SECTORS_COUNT; i++)
			{
				u32 *t = (u32*)0x03007430;
				EraseFlash EraseFlashSector = (EraseFlash)(*t);
				EraseFlashSector(i);
			}
			SaveSerializedGame();
			SaveWriteToFlash(0xFFFF, gRamSaveSectionLocations);
			break;
		}
	};
	gMain.vblankCounter1 = backupPtr;
	return 0;
}

//Hard to save the parasite with the vanilla save splitting to prevent cheating
u8 SaveDataAfterLinkBattle(void)
{
	gTerrainType = 0; //Doesn't get cleared for the second player
	TrySavingData(SAVE_NORMAL);
	ClearContinueGameWarpStatus2();
	return 3; //New state in switch statement
}

u8 SaveDataAfterLinkTrade(void)
{
	TrySavingData(SAVE_NORMAL);
	ClearContinueGameWarpStatus2();
	return 4; //New state in switch statement
}

void NewGameWipeNewSaveData(void)
{
	#ifdef UNBOUND
	extern void WipeUnboundNewSaveRAM(void);
	WipeUnboundNewSaveRAM();
	#else
	Memset((void*) SAVE_BLOCK_PARASITE, 0, 0x2EA4);
	#endif
}

static void Task_SaveErrorStatus_RunPrinter(unusedArg u8 taskId)
{
	if (!gPaletteFade->active)
		RunTextPrinters();
}

void PrintChangeSaveTypeErrorStatus(u8 taskId, const u8* str)
{
	PrintSaveErrorStatus(taskId, str);
	gTasks[taskId].func = Task_SaveErrorStatus_RunPrinter; //Override normal
}

extern const u8 gText_MainMenuEnableRTC[];
extern const u8 gText_MainMenuEnableRTCNoSave[];
extern const u8 gText_MainMenuTimeSetInFuture[];
extern bool8 sPrintedRTCWarning;
bool8 TryDisplayMainMenuRTCWarning(unusedArg u8 taskId)
{
	#ifdef TIME_ENABLED
	if (!sPrintedRTCWarning)
	{
		#ifdef UNBOUND
		if (TryPreventIncompleteSaves(taskId))
		{
			sPrintedRTCWarning = TRUE;
			return TRUE;
		}
		else
		#endif
		if (RtcGetErrorStatus() & RTC_ERR_FLAG_MASK)
		{
			const u8* warning;
			sPrintedRTCWarning = TRUE;
			
			if (gSaveFileStatus == SAVE_STATUS_EMPTY)
				warning = gText_MainMenuEnableRTCNoSave;
			else
				warning = gText_MainMenuEnableRTC;
			
			PrintSaveErrorStatus(taskId, warning);
			gTasks[taskId].func = (void*) (0x0800C688 | 1); // Task_SaveErrorStatus_RunPrinterThenWaitButton
			return TRUE;
		}
		else if (IsDayInVarInFuture(VAR_SWARM_DAILY_EVENT)) //If the player tampered with their system time to access more daily events
		{
			sPrintedRTCWarning = TRUE;
			BufferYearMonthDayFromVar(VAR_SWARM_DAILY_EVENT);
			StringExpandPlaceholders(gStringVarC, gText_MainMenuTimeSetInFuture);
			PrintSaveErrorStatus(taskId, gStringVarC);
			gTasks[taskId].func = (void*) (0x0800C688 | 1); // Task_SaveErrorStatus_RunPrinterThenWaitButton
			return TRUE;
		}
	}
	#endif

	return FALSE;
}

u8* GetExpandedFlagPointer(u16 id)
{
	#ifdef SAVE_BLOCK_EXPANSION
		if (id >= 0x900 && id < 0x1900)
			return &gExpandedFlags[(id - 0x900) / 8];
		else if (id >= 0x4000)
			return NULL;
		else if (id >= 0x1900)
			return (u8*) &Var8000; //Returns Var8000 to prevent bad memory access
		else
			return NULL;
	#else
		if (id >= 0x4000)
			return NULL;
		else if (id >= 0x1900)
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
