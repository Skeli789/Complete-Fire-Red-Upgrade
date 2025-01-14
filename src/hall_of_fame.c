#include "defines.h"
#include "../include/hall_of_fame.h"
#include "../include/field_weather.h"
#include "../include/menu.h"
#include "../include/overworld.h"
#include "../include/save.h"
#include "../include/script.h"
#include "../include/script_menu.h"
#include "../include/sound.h"
#include "../include/string_util.h"

#include "../include/new/mega.h"

//Hall of Fame Fix for Expanded Pokemon
//Based on https://github.com/Sagiri/fame-hall

#ifdef UNBOUND
#define HALL_OF_FAME_BG_PAL RGB(23, 16, 28) //0x7217
#else
#define HALL_OF_FAME_BG_PAL RGB(22, 24, 29)
#endif

void CB2_DoHallOfFameScreen(void);
static void Task_Hof_InitTeamSaveData(u8 taskId);
void Task_Hof_DisplayMon(u8 taskId);
static void Task_Hof_PlayMonCryAndPrintInfo(u8 taskId);
static void Task_Hof_TryDisplayAnotherMon(u8 taskId);
void Task_HofPC_CopySaveData(u8 taskId);
void Task_HofPC_DrawSpritesPrintText(u8 taskId);
void Task_HofPC_PrintMonInfo(u8 taskId);
void Task_Hof_InitMonData(u8 taskId);
void HallOfFame_PrintMonInfo(struct HallofFameMon *currMon, unusedArg u8 a1, unusedArg u8 a2);

void CB2_DoHallOfFameScreen(void)
{
	if (!InitHallOfFameScreen())
	{
		u8 taskId = CreateTask(Task_Hof_InitMonData, 0);
		#ifdef DEBUG_HOF
		gTasks[taskId].tDontSaveData = TRUE;
		#else
		gTasks[taskId].tDontSaveData = FALSE;
		#endif
		sHofMonPtr = Calloc(sizeof(*sHofMonPtr));
	}
}

void Task_Hof_InitMonData(u8 taskId)
{
	u16 i, j;
	gTasks[taskId].tMonNumber = 0;  // valid pokes

	for (i = 0; i < PARTY_SIZE; ++i)
	{
		u8 nick[POKEMON_NAME_LENGTH + 2];
		u16 species = GetMonData(&gPlayerParty[i], MON_DATA_SPECIES2, NULL);

		if (species != SPECIES_NONE)
		{
			#ifdef SHOW_MEGAS_IN_HOF
			u16 item = GetMonData(&gPlayerParty[i], MON_DATA_HELD_ITEM, NULL);
			u16 altSpecies = GetMegaSpecies(species, item, gPlayerParty[i].moves); //Try Mega Evolve the mon
			if (altSpecies != SPECIES_NONE)
				species = altSpecies; //Update with Mega Evolved form
			else
			{
				altSpecies = GetPrimalSpecies(species, item);
				if (altSpecies != SPECIES_NONE)
					species = altSpecies; //Update with Primal form
			}
			#endif

			sHofMonPtr->mon[i].species = species;
			sHofMonPtr->mon[i].tid = GetMonData(&gPlayerParty[i], MON_DATA_OT_ID, NULL);
			sHofMonPtr->mon[i].personality = GetMonData(&gPlayerParty[i], MON_DATA_PERSONALITY, NULL);
			sHofMonPtr->mon[i].lvl = GetMonData(&gPlayerParty[i], MON_DATA_LEVEL, NULL);
			GetMonData(&gPlayerParty[i], MON_DATA_NICKNAME, nick);
			for (j = 0; j < POKEMON_NAME_LENGTH; ++j)
				sHofMonPtr->mon[i].nick[j] = nick[j];
			gTasks[taskId].tMonNumber++;
		}
		else
		{
			sHofMonPtr->mon[i].species = 0;
			sHofMonPtr->mon[i].tid = 0;
			sHofMonPtr->mon[i].personality = 0;
			sHofMonPtr->mon[i].lvl = 0;
			sHofMonPtr->mon[i].nick[0] = EOS;
		}
	}

	sHofSelectedPaletteIndices = 0;
	gTasks[taskId].tDisplayedMonId = 0;
	gTasks[taskId].tPlayerSpriteID = 0xFF;

	for (i = 0; i < PARTY_SIZE; ++i)
		gTasks[taskId].tMonSpriteId(i) = 0xFF;

	if (gTasks[taskId].tDontSaveData != 0)
		gTasks[taskId].func = Task_Hof_SetMonDisplayTask;
	else
		gTasks[taskId].func = Task_Hof_InitTeamSaveData;
}

static void Task_Hof_InitTeamSaveData(u8 taskId)
{
	u16 i;
	struct HallofFameTeam* lastSavedTeam = (struct HallofFameTeam *)(gDecompressionBuffer);
	Var8005 = 50; //0x20370C2
	
	if (!gHasHallOfFameRecords)
		Memset(gDecompressionBuffer, 0, 0x2000);
	else
	{
		if (Save_LoadGameData(3) != TRUE)
			Memset(gDecompressionBuffer, 0, 0x2000);
	}

	for (i = 0; i < HALL_OF_FAME_MAX_TEAMS; ++i, ++lastSavedTeam)
	{
		if (lastSavedTeam->mon[0].species == SPECIES_NONE)
			break;
	}

	if (i >= HALL_OF_FAME_MAX_TEAMS)
	{
		struct HallofFameTeam* afterTeam = (struct HallofFameTeam *)(gDecompressionBuffer);
		struct HallofFameTeam* beforeTeam = (struct HallofFameTeam *)(gDecompressionBuffer);
		afterTeam++;

		for (i = 0; i < HALL_OF_FAME_MAX_TEAMS - 1; ++i, beforeTeam++, afterTeam++)
			*beforeTeam = *afterTeam;

		lastSavedTeam--;
	}
	*lastSavedTeam = *sHofMonPtr;

	NewMenuHelpers_DrawDialogueFrame(0, 0);
	AddTextPrinterParameterized2(0, 1, gText_SavingDontTurnOffPower, 0, NULL, 2, 1, 3);
	CopyWindowToVram(0, 3);
	gTasks[taskId].func = Task_Hof_TrySaveData;
}


void Task_Hof_DisplayMon(u8 taskId)
{
	u8 spriteId;
	s16 xPos, yPos, dtsX, dstY;

	u16 currMonId = gTasks[taskId].tDisplayedMonId;
	struct HallofFameMon *currMon = &sHofMonPtr->mon[currMonId];

	if (gTasks[taskId].tMonNumber > 3)
	{
		xPos = sHallOfFame_MonFullTeamPositions[currMonId][0];
		yPos = sHallOfFame_MonFullTeamPositions[currMonId][1];
		dtsX = sHallOfFame_MonFullTeamPositions[currMonId][2];
		dstY = sHallOfFame_MonFullTeamPositions[currMonId][3];
	}
	else
	{
		xPos = sHallOfFame_MonHalfTeamPositions[currMonId][0];
		yPos = sHallOfFame_MonHalfTeamPositions[currMonId][1];
		dtsX = sHallOfFame_MonHalfTeamPositions[currMonId][2];
		dstY = sHallOfFame_MonHalfTeamPositions[currMonId][3];
	}

	spriteId = CreateMonPicSprite_HandleDeoxys(currMon->species, currMon->tid, currMon->personality, 1, xPos, yPos, currMonId, 0xFFFF);
	gSprites[spriteId].tDestinationX = dtsX;
	gSprites[spriteId].tDestinationY = dstY;
	gSprites[spriteId].data[0] = 0;
	gSprites[spriteId].callback = SpriteCB_GetOnScreenAndAnimate;
	gTasks[taskId].tMonSpriteId(currMonId) = spriteId;
	ClearDialogWindowAndFrame(0, TRUE);
	gTasks[taskId].func = Task_Hof_PlayMonCryAndPrintInfo;
}

static void Task_Hof_PlayMonCryAndPrintInfo(u8 taskId)
{
	u16 currMonId = gTasks[taskId].tDisplayedMonId;
	struct HallofFameMon *currMon = &sHofMonPtr->mon[currMonId];
	struct Sprite *monSprite = &gSprites[gTasks[taskId].tMonSpriteId(currMonId)];

	if (monSprite->data[0])
	{
		if (currMon->species != SPECIES_EGG)
			PlayCry1(currMon->species, 0);

		HallOfFame_PrintMonInfo(currMon, 0, 14);
		gTasks[taskId].tFrameCount = 120;
		gTasks[taskId].func = Task_Hof_TryDisplayAnotherMon;
	}
}


static void Task_Hof_TryDisplayAnotherMon(u8 taskId)
{
	u16 currPokeID = gTasks[taskId].tDisplayedMonId;
	struct HallofFameMon *currMon = &sHofMonPtr->mon[currPokeID];

	if (gTasks[taskId].tFrameCount != 0)
	{
		gTasks[taskId].tFrameCount--;
	}
	else
	{
		sHofSelectedPaletteIndices |= (0x10000 << gSprites[gTasks[taskId].tMonSpriteId(currPokeID)].oam.paletteNum);
		if (gTasks[taskId].tDisplayedMonId <= 4 && currMon[1].species != SPECIES_NONE)  // there is another pokemon to display
		{
			gTasks[taskId].tDisplayedMonId++;
			BeginNormalPaletteFade(sHofSelectedPaletteIndices, 0, 12, 12, HALL_OF_FAME_BG_PAL);
			gSprites[gTasks[taskId].tMonSpriteId(currPokeID)].oam.priority = 1;
			gTasks[taskId].func = Task_Hof_DisplayMon;
		}
		else
		{
			gTasks[taskId].func = Task_Hof_PaletteFadeAndPrintWelcomeText;
		}
	}
}

void Task_HofPC_CopySaveData(u8 taskId)
{
	HofPC_CreateWindow(0, 0x1E, 0, 0xC, 0x226);
	if (Save_LoadGameData(SAVE_HALL_OF_FAME) != SAVE_STATUS_OK)
	{
		gTasks[taskId].func = Task_HofPC_PrintDataIsCorrupted;
	}
	else
	{
		u16 i;
		struct HallofFameTeam* savedTeams;

		CpuCopy16(gDecompressionBuffer, sHofMonPtr, 0x2000);
		savedTeams = sHofMonPtr;
		for (i = 0; i < HALL_OF_FAME_MAX_TEAMS; ++i, ++savedTeams)
		{
			if (savedTeams->mon[0].species == SPECIES_NONE)
				break;
		}

		if (i < HALL_OF_FAME_MAX_TEAMS)
			gTasks[taskId].tCurrTeamNo = i - 1;
		else
			gTasks[taskId].tCurrTeamNo = HALL_OF_FAME_MAX_TEAMS - 1;

		gTasks[taskId].tCurrPageNo = GetGameStat(GAME_STAT_ENTERED_HOF);
		gTasks[taskId].func = Task_HofPC_DrawSpritesPrintText;
	}
}

void Task_HofPC_DrawSpritesPrintText(u8 taskId)
{
	struct HallofFameTeam* savedTeams = sHofMonPtr;
	struct HallofFameMon* currMon;
	u16 i;

	for (i = 0; i < gTasks[taskId].tCurrTeamNo; ++i)
		savedTeams++;

	currMon = &savedTeams->mon[0];
	sHofSelectedPaletteIndices = 0;
	gTasks[taskId].tCurrMonId = 0;
	gTasks[taskId].tMonNo = 0;

	for (i = 0; i < PARTY_SIZE; ++i, currMon++)
	{
		if (currMon->species != 0)
			gTasks[taskId].tMonNo++;
	}

	currMon = &savedTeams->mon[0];

	for (i = 0; i < PARTY_SIZE; ++i, currMon++)
	{
		if (currMon->species != 0)
		{
			u16 spriteId;
			s16 posX, posY;

			if (gTasks[taskId].tMonNo > 3)
			{
				posX = sHallOfFame_MonFullTeamPositions[i][2];
				posY = sHallOfFame_MonFullTeamPositions[i][3];
			}
			else
			{
				posX = sHallOfFame_MonHalfTeamPositions[i][2];
				posY = sHallOfFame_MonHalfTeamPositions[i][3];
			}

			if (currMon->species == SPECIES_EGG)
				posY += 10;

			spriteId = CreateMonPicSprite_HandleDeoxys(currMon->species, currMon->tid, currMon->personality, 1, posX, posY, i, 0xFFFF);
			gSprites[spriteId].oam.priority = 1;
			gTasks[taskId].tMonSpriteId(i) = spriteId;
		}
		else
		{
			gTasks[taskId].tMonSpriteId(i) = 0xFF;
		}
	}

	BlendPalettes(0xFFFF0000, 0xC, HALL_OF_FAME_BG_PAL);

	ConvertIntToDecimalStringN(gStringVar1, gTasks[taskId].tCurrPageNo, STR_CONV_MODE_LEFT_ALIGN, 3);
	StringExpandPlaceholders(gStringVar4, gText_HOFNumber);

	if (gTasks[taskId].tCurrTeamNo <= 0)
		HofPC_PutText(gStringVar4, gText_PickCancel, 0, 0, TRUE);
	else
		HofPC_PutText(gStringVar4, gText_PickNextCancel, 0, 0, TRUE);

	gTasks[taskId].func = Task_HofPC_PrintMonInfo;
}

void Task_HofPC_PrintMonInfo(u8 taskId)
{
	struct HallofFameTeam* savedTeams = sHofMonPtr;
	struct HallofFameMon* currMon;
	u16 i;
	u16 currMonID;

	for (i = 0; i < gTasks[taskId].tCurrTeamNo; ++i)
		savedTeams++;

	for (i = 0; i < PARTY_SIZE; ++i)
	{
		u16 spriteId = gTasks[taskId].tMonSpriteId(i);
		if (spriteId != 0xFF)
			gSprites[spriteId].oam.priority = 1;
	}

	currMonID = gTasks[taskId].tMonSpriteId(gTasks[taskId].tCurrMonId);
	gSprites[currMonID].oam.priority = 0;
	sHofSelectedPaletteIndices = (0x10000 << gSprites[currMonID].oam.paletteNum) ^ 0xFFFF0000;
	BlendPalettesUnfaded(sHofSelectedPaletteIndices, 0xC, HALL_OF_FAME_BG_PAL);

	currMon = &savedTeams->mon[gTasks[taskId].tCurrMonId];
	if (currMon->species != SPECIES_EGG)
	{
		StopCryAndClearCrySongs();
		PlayCry1(currMon->species, 0);
	}
	HallOfFame_PrintMonInfo(currMon, 0, 14);

	gTasks[taskId].func = Task_HofPC_HandleInput;
}

void HallOfFame_PrintMonInfo(struct HallofFameMon *currMon, unusedArg u8 a1, unusedArg u8 a2)
{
	u8 text[16];
	u8 text2[24];
	u16 i;
	u8 *stringPtr;
	u16 dexNumber;
	s32 width;
	s32 x;
	u16 species = currMon->species;

	FillWindowPixelBuffer(0, 0);
	PutWindowTilemap(0);

	// dex number
	if (species != SPECIES_EGG)
	{
		stringPtr = StringCopy(text, gText_Number);
		dexNumber = SpeciesToPokedexNum(species);
		if (dexNumber != 0xFFFF)
		{
			text[0] = (dexNumber / 100) + CHAR_0;
			text[1] = ((dexNumber %= 100) / 10) + CHAR_0;
			text[2] = (dexNumber % 10) + CHAR_0;
		}
		else
		{
			text[0] = text[1] = text[2] = CHAR_QUESTION_MARK;
		}
		text[3] = EOS;
		StringAppend(text2, text);
		WindowPrint(0, 2, 0x10, 1, sUnknown_0840C23C, 0, text);
	}

	// nick, species names, gender and level
	i = 0;
	if (currMon->nick[0] != EOS)
	{
		for (i = 0; i < 10 && currMon->nick[i] != EOS; i++)
			text[i] = currMon->nick[i];
	}
	text[i] = EOS;
	width = GetStringWidth(2, text, GetFontAttribute(2, FONTATTR_LETTER_SPACING));

	if (species == SPECIES_EGG)
	{
		x = 0x80 - width / 2;
		WindowPrint(0, 2, x, 1, sUnknown_0840C23C, 0, text);
	}
	else
	{
		x = 0x80 - width;
		WindowPrint(0, 2, x, 1, sUnknown_0840C23C, 0, text);

		text[0] = CHAR_SLASH;
		stringPtr = StringCopy(text + 1, gSpeciesNames[species]);

		#if (defined NATIONAL_DEX_NIDORAN_M && defined NATIONAL_DEX_NIDORAN_F)
		if (species != NATIONAL_DEX_NIDORAN_M && species != NATIONAL_DEX_NIDORAN_F)
		#endif
		{
			switch (GetGenderFromSpeciesAndPersonality(species, currMon->personality))
			{
				case MON_MALE:
					*stringPtr++ = CHAR_MALE;
					break;
				case MON_FEMALE:
					*stringPtr++ = CHAR_FEMALE;
					break;
				default:
					*stringPtr++ = CHAR_SPACE;
					break;
			}
		}

		*stringPtr = EOS;
		WindowPrint(0, 2, 0x80, 1, sUnknown_0840C23C, 0, text);

		stringPtr = StringCopy(text, gText_Level);
		ConvertIntToDecimalStringN(stringPtr, currMon->lvl, STR_CONV_MODE_LEFT_ALIGN, 3);
		WindowPrint(0, 2, 0x20, 0x11, sUnknown_0840C23C, 0, text);

		stringPtr = StringCopy(text, gText_IDNumber);
		ConvertIntToDecimalStringN(stringPtr, (u16)(currMon->tid), STR_CONV_MODE_LEADING_ZEROS, 5);
		WindowPrint(0, 2, 0x60, 0x11, sUnknown_0840C23C, 0, text);
	}

	CopyWindowToVram(0, COPYWIN_BOTH);
}

void __attribute__((long_call)) Task_HOFPCWaitForPaletteFade(u8 taskId);

void ReshowPCMenuAfterHallOfFamePC(void)
{
	ScriptContext2_Enable();
	Overworld_PlaySpecialMapMusic();
	CreatePCMenu();
	ScriptMenu_DisplayPCStartupPrompt();
	gWeatherPtr->readyForInit = TRUE; //Fixes start menu bug
	FadeScreen(FADE_FROM_BLACK, 0);
	CreateTask(Task_HOFPCWaitForPaletteFade, 10);
}
