#include "defines.h"
#include "../include/bg.h"
#include "../include/event_object_movement.h"
#include "../include/field_screen_effect.h"
#include "../include/field_weather.h"
#include "../include/gpu_regs.h"
#include "../include/item_menu.h"
#include "../include/menu.h"
#include "../include/overworld.h"
#include "../include/random.h"
#include "../include/palette.h"
#include "../include/pokemon_icon.h"
#include "../include/scanline_effect.h"
#include "../include/script.h"
#include "../include/string_util.h"
#include "../include/window.h"
#include "../include/constants/event_objects.h"
#include "../include/constants/songs.h"
#include "../include/gba/io_reg.h"

#include "../include/new/build_pokemon.h"
#include "../include/new/frontier.h"
#include "../include/new/mega.h"

extern const u8 RaidBattleIntroBGTiles[];
extern const u8 RaidBattleIntroBGPal[];
extern const u8 RaidBattleIntroBGMap[];
extern const u8 RaidBattleIntroBGUnboundPal[];
extern const u8 RaidBattleStarTiles[];
extern const u8 RaidBattleStarPal[];
extern const u8 RaidBattleCursorTiles[];
extern const u8 RaidBattleCursorPal[];

extern const u8 gText_RecommendedLevel[];
extern const u8 gText_RaidIntroSelection[];
extern const u8 gText_RaidBattleRules[];
extern const u8 gText_RaidBattleChoosePartner[];

#define MAX_NUM_PARTNERS 3
#define MAX_TEAM_SIZE 3

struct Partner
{
	u16 id;
	u16 graphicsId;
	u16 team[MAX_TEAM_SIZE];
};

struct RaidBattleIntro
{
	u8* tilemapPtr;
	struct Partner partners[MAX_NUM_PARTNERS];
	u32 personality;
	u16 species;
	u8 rank;
	u8 selectedTeam;
	u8 monSpriteId;
	bool8 outlinedSprite;
};

#define sRaidBattleIntroPtr (*((struct RaidBattleIntro**) 0x203E038))

enum Windows
{
	WIN_RECOMMENDED_LEVEL,
	WIN_CHOOSE_PARTNER,
	WIN_RULES,
	WIN_TYPE_1,
	WIN_TYPE_2,
	WIN_INSTRUCTIONS,
	WINDOW_COUNT,
};

static void SpriteCB_RedArrowTeamSelection(struct Sprite* sprite);

static const struct WindowTemplate sRaidBattleIntroWinTemplates[WINDOW_COUNT + 1] =
{
	[WIN_RECOMMENDED_LEVEL] =
	{
		.bg = 1,
		.tilemapLeft = 17,
		.tilemapTop = 0,
		.width = 14,
		.height = 3,
		.paletteNum = 15,
		.baseBlock = 1,
	},
	[WIN_CHOOSE_PARTNER] =
	{
		.bg = 1,
		.tilemapLeft = 16,
		.tilemapTop = 3,
		.width = 14,
		.height = 2,
		.paletteNum = 14,
		.baseBlock = 43,
	},
	[WIN_RULES] =
	{
		.bg = 1,
		.tilemapLeft = 1,
		.tilemapTop = 13,
		.width = 10,
		.height = 6,
		.paletteNum = 15,
		.baseBlock = 71,
	},
	[WIN_TYPE_1] =
	{
		.bg = 1,
		.tilemapLeft = 8,
		.tilemapTop = 0,
		.width = 4,
		.height = 2,
		.paletteNum = 12,
		.baseBlock = 131,
	},
	[WIN_TYPE_2] =
	{
		.bg = 1,
		.tilemapLeft = 12,
		.tilemapTop = 0,
		.width = 4,
		.height = 2,
		.paletteNum = 12,
		.baseBlock = 149,
	},
	[WIN_INSTRUCTIONS] =
	{
		.bg = 1,
		.tilemapLeft = 8,
		.tilemapTop = 18,
		.width = 22,
		.height = 2,
		.paletteNum = 15,
		.baseBlock = 167,
	},
	DUMMY_WIN_TEMPLATE
};

static const struct BgTemplate sRaidBattleIntroBgTemplates[] =
{
	{ //Intro Background
		.bg = 2,
		.charBaseIndex = 0,
		.mapBaseIndex = 31,
		.screenSize = 0,
		.paletteMode = 0,
		.priority = 2,
		.baseTile = 0,
	},
	{ //Text
		.bg = 1,
		.charBaseIndex = 2,
		.mapBaseIndex = 6,
		.screenSize = 0,
		.paletteMode = 0,
		.priority = 1,
		.baseTile = 0,
	},
	{ //Instructions
		.bg = 0,
		.charBaseIndex = 1,
		.mapBaseIndex = 24,
		.screenSize = 0,
		.paletteMode = 0,
		.priority = 0,
		.baseTile = 0,
	}
};

enum
{
	RAID_BATTLE_STAR_TAG = 0x2710,
	RAID_BATTLE_CURSOR_TAG,
};

static const struct OamData sStarOAM =
{
	.affineMode = ST_OAM_AFFINE_OFF,
	.objMode = ST_OAM_OBJ_NORMAL,
	.shape = SPRITE_SHAPE(8x8),
	.size = SPRITE_SIZE(8x8),
	.priority = 2, //On BG 2
};

static const struct OamData sOamData_RedArrowCursor =
{
	.affineMode = ST_OAM_AFFINE_OFF,
	.objMode = ST_OAM_OBJ_NORMAL,
	.shape = SPRITE_SHAPE(32x32),
	.size = SPRITE_SIZE(32x32),
	.priority = 0, //Above everything
};

static const struct SpriteTemplate sRaidBattleStarSpriteTemplate =
{
	.tileTag = RAID_BATTLE_STAR_TAG,
	.paletteTag = RAID_BATTLE_STAR_TAG,
	.oam = &sStarOAM,
	.anims = gDummySpriteAnimTable,
	.images = NULL,
	.affineAnims = gDummySpriteAffineAnimTable,
	.callback = SpriteCallbackDummy,
};

static const struct SpriteTemplate sRaidBattleCursorSpriteTemplate =
{
	.tileTag = RAID_BATTLE_CURSOR_TAG,
	.paletteTag = RAID_BATTLE_CURSOR_TAG,
	.oam = &sOamData_RedArrowCursor,
	.anims = gDummySpriteAnimTable,
	.images = NULL,
	.affineAnims = gDummySpriteAffineAnimTable,
	.callback = SpriteCB_RedArrowTeamSelection,
};

static const struct CompressedSpriteSheet sRaidBattleStarSpriteSheet = {RaidBattleStarTiles, (8 * 8) / 2, RAID_BATTLE_STAR_TAG};
static const struct CompressedSpritePalette sRaidBattleStarSpritePalette = {RaidBattleStarPal, RAID_BATTLE_STAR_TAG};
static const struct CompressedSpriteSheet sRaidBattleCursorSpriteSheet = {RaidBattleCursorTiles, (32 * 32) / 2, RAID_BATTLE_CURSOR_TAG};
static const struct CompressedSpritePalette sRaidBattleCursorSpritePalette = {RaidBattleCursorPal, RAID_BATTLE_CURSOR_TAG};

static bool8 GetRaidBattleData(void)
{
	u32 i, j, k;
	bool8 checkedPartners[gNumRaidPartners];

	DetermineRaidStars();
	DetermineRaidSpecies();
	DetermineRaidLevel();
	sRaidBattleIntroPtr->rank = gRaidBattleStars;
	sRaidBattleIntroPtr->species = gRaidBattleSpecies;

	if (gRaidBattleSpecies == SPECIES_NONE)
		return FALSE;

	TryRandomizeSpecies(&sRaidBattleIntroPtr->species);

	for (i = 0; i < gNumRaidPartners; ++i)
		checkedPartners[i] = FALSE;

	DetermineRaidPartners(checkedPartners, MAX_NUM_PARTNERS);

	k = 0;
	for (i = 0; i < gNumRaidPartners; ++i)
	{
		if (checkedPartners[i] == TRUE) //0xFF means not viable
		{
			struct Partner* partner = &sRaidBattleIntroPtr->partners[k++];

			partner->id = i;
			partner->graphicsId = gRaidPartners[i].owNum;

			for (j = 0; j < MAX_TEAM_SIZE; ++j)
			{
				const struct BattleTowerSpread* spread = GetRaidMultiSpread(i, j, sRaidBattleIntroPtr->rank);
				if (spread != NULL)
					partner->team[j] = spread->species;
				else
					break;
			}
		}

		if (k >= MAX_NUM_PARTNERS)
			break;
	}

	if (k == 0) //No partners found
		return FALSE;

	return TRUE;

/*Test Data
	gRaidBattleStars = 6;
	sRaidBattleIntroPtr->species = SPECIES_SALAMENCE;
	sRaidBattleIntroPtr->rank = 6;

	sRaidBattleIntroPtr->partners[0].graphicsId = EVENT_OBJ_GFX_RIVAL;
	sRaidBattleIntroPtr->partners[0].team[0] = SPECIES_TYRANITAR;
	sRaidBattleIntroPtr->partners[0].team[1] = SPECIES_MAMOSWINE;
	sRaidBattleIntroPtr->partners[0].team[2] = SPECIES_GRANBULL;

	sRaidBattleIntroPtr->partners[1].graphicsId = EVENT_OBJ_GFX_JAX;
	sRaidBattleIntroPtr->partners[1].team[0] = SPECIES_GOLURK;
	sRaidBattleIntroPtr->partners[1].team[1] = SPECIES_MAGNEZONE;
	sRaidBattleIntroPtr->partners[1].team[2] = SPECIES_SALAMENCE;

	sRaidBattleIntroPtr->partners[2].graphicsId = EVENT_OBJ_GFX_RED;
	sRaidBattleIntroPtr->partners[2].team[0] = SPECIES_PIKACHU_CAP_ORIGINAL;
	sRaidBattleIntroPtr->partners[2].team[1] = SPECIES_SNORLAX;
	sRaidBattleIntroPtr->partners[2].team[2] = SPECIES_MEWTWO;

	return TRUE;
*/
}

static void OutlineMonSprite(u8 spriteId)
{
	u32 i = 0;
	u8 buffer[(64 * 64) / 2] = {0};
	u8* offset = (void*)(OBJ_VRAM0) + (gSprites[spriteId].oam.tileNum * 32);
	u8* originalOffset = offset;

	for (i = 0; i < (64 * 64) / 2; ++i)
	{
		/*Pixel Map - Bits
		6	7

		1	2	3

		4	5
		*/

		/*Pixel map - Bytes
		67
		12 3
		45
		*/

		u8 nextByteColumn = 1;
		if (i % 4 == 3)
			nextByteColumn = 0x1D;

		u8 nextByteRow = 4;
		if (i % 0x20 >= 0x1C)
			nextByteRow = 0xE4;

		u8 previousByteRow = 4;
		if (i % 0x20 < 4)
			previousByteRow = 0xE4;

		//Next column
		u8 pixel1 = offset[i] & 0xF;
		u8 pixel2 = (offset[i] >> 4) & 0xF;

		u8 pixel3 = offset[i + nextByteColumn] & 0xF;

		//Next row
		u8 pixel4 = offset[i + nextByteRow] & 0xF;
		u8 pixel5 = (offset[i + nextByteRow] >> 4) & 0xF;

		//Row Above
		u8 pixel6 = offset[i - previousByteRow] & 0xF;
		u8 pixel7 = (offset[i - previousByteRow] >> 4) & 0xF;

		bool8 isTopBorder = (i < 0xE4) && ((i % 0x20) < 4);
		bool8 isBottomBorder = (i >= 0x71C) && ((i % 0x20) >= 0x1C);
		bool8 isLeftBorder = ((i % 0x100) < 0x20) && ((i % 4) == 0);
		bool8 isRightBorder = ((i % 0x100) >= 0xE0) && ((i % 4) == 3);

		if (pixel1 != 0)
		{
			if (isTopBorder || isBottomBorder || isLeftBorder)
				buffer[i + 0] |= 0xF; //Set lower bit to white
			else
				buffer[i + 0] |= 0x1; //Set lower bit to black

			if (pixel2 == 0)
				buffer[i + 0] |= 0xF0; //Set upper bit

			if (pixel4 == 0)
				buffer[i + nextByteRow] |= 0xF; //Set lower bit

			if (i >= previousByteRow && pixel6 == 0)
				buffer[i - previousByteRow] |= 0xF; //Set lower bit*/
		}

		if (pixel2 != 0)
		{
			if (isTopBorder || isBottomBorder || isRightBorder) //Right border
				buffer[i + 0] |= 0xF0; //Set upper bit to white
			else
				buffer[i + 0] |= 0x10; //Set upper bit to black

			if (pixel1 == 0)
				buffer[i + 0] |= 0xF; //Set lower bit

			if (pixel3 == 0 && (i % 0x100 < 0xE0 || i % 4 != 3))
				buffer[i + nextByteColumn] |= 0xF; //Set lower bit

			if (pixel5 == 0)
				buffer[i + nextByteRow] |= 0xF0; //Set upper bit

			if (i >= previousByteRow && pixel7 == 0)
				buffer[i - previousByteRow] |= 0xF0; //Set upper bit
		}

		if (pixel3 != 0 && !isRightBorder)
		{
			if (pixel2 == 0)
				buffer[i + 0] |= 0xF0; //Set upper bit
		}
	}

	CpuCopy32(buffer, originalOffset, (64 * 64) / 2);
	++sRaidBattleIntroPtr->outlinedSprite;
}

static void Task_RaidBattleIntroFadeOut(u8 taskId)
{
	if (!gPaletteFade->active)
	{
		SetMainCallback2(CB2_ReturnToFieldContinueScript);
		Free(sRaidBattleIntroPtr->tilemapPtr);
		Free(sRaidBattleIntroPtr);
		FreeAllWindowBuffers();
		DestroyTask(taskId);
	}
}

static void Task_RaidBattleIntroWaitForKeyPress(u8 taskId)
{
	u8 i;
	u16 id;
	const u8* name;

	if (gMain.newKeys & A_BUTTON)
	{
		PRESSED_A:
		id = sRaidBattleIntroPtr->partners[sRaidBattleIntroPtr->selectedTeam].id;

		gSpecialVar_LastResult = 1;
		FlagSet(FLAG_TAG_BATTLE);
		VarSet(VAR_PARTNER, RAID_BATTLE_MULTI_TRAINER_TID);
		VarSet(VAR_FACILITY_TRAINER_ID_PARTNER, id);
		VarSet(VAR_PARTNER_BACKSPRITE, gRaidPartners[id].backSpriteId);

		name = TryGetRivalNameByTrainerClass(gRaidPartners[id].trainerClass);
		if (name == NULL)
			name = gRaidPartners[id].name;
		StringCopy(gStringVar1, name);
		StringCopy(gStringVar7, name);
		GetSpeciesName(gStringVar2, sRaidBattleIntroPtr->species);
		GetSpeciesName(gStringVar8, sRaidBattleIntroPtr->species);

		PlaySE(SE_CORRECT);
		BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 16, RGB_BLACK);
		gTasks[taskId].func = Task_RaidBattleIntroFadeOut;
	}
	else if (gMain.newKeys & B_BUTTON)
	{
		gSpecialVar_LastResult = 0;

		BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 16, RGB_BLACK);
		gTasks[taskId].func = Task_RaidBattleIntroFadeOut;
	}
	else if (gMain.newAndRepeatedKeys & START_BUTTON)
	{
		u8 numTeams = 0;
		PlaySE(SE_SELECT);
		for (i = 0; i < MAX_TEAM_SIZE; ++i)
		{
			if (sRaidBattleIntroPtr->partners[i].graphicsId != 0)
				++numTeams;
		}

		sRaidBattleIntroPtr->selectedTeam = Random() % numTeams;
		goto PRESSED_A;
	}
	else if (gMain.newAndRepeatedKeys & DPAD_UP)
	{
		PlaySE(SE_SELECT);
		if (sRaidBattleIntroPtr->selectedTeam == 0)
		{
			for (i = 0; i < MAX_TEAM_SIZE; ++i)
			{
				if (sRaidBattleIntroPtr->partners[i].graphicsId != 0)
					++sRaidBattleIntroPtr->selectedTeam;
				else
					break;
			}

			sRaidBattleIntroPtr->selectedTeam -= 1; //Prevent overflow
		}
		else
			--sRaidBattleIntroPtr->selectedTeam;
	}
	else if (gMain.newAndRepeatedKeys & DPAD_DOWN)
	{
		PlaySE(SE_SELECT);
		++sRaidBattleIntroPtr->selectedTeam;

		if (sRaidBattleIntroPtr->selectedTeam >= MAX_TEAM_SIZE
		||  sRaidBattleIntroPtr->partners[sRaidBattleIntroPtr->selectedTeam].graphicsId == 0)
			sRaidBattleIntroPtr->selectedTeam = 0;
	}
}

static void SpriteCB_RedArrowTeamSelection(struct Sprite* sprite)
{
	sprite->pos1.y = 59 + (sRaidBattleIntroPtr->selectedTeam * 33);

	if (sprite->data[1])
	{
		sprite->data[0] -= 1;
		if (sprite->data[0] == 0)
			sprite->data[1] = FALSE;
	}
	else
	{
		sprite->data[0] += 1;
		if (sprite->data[0] == 20)
			sprite->data[1] = TRUE;
	}

	sprite->pos2.x = 5 - sprite->data[0] / 4;
}

static void Task_RaidBattleIntroFadeIn(u8 taskId)
{
	if (sRaidBattleIntroPtr->outlinedSprite < 2)
		OutlineMonSprite(sRaidBattleIntroPtr->monSpriteId);

	if (!gPaletteFade->active)
	{
		PlaySE(SE_RG_CARD3);
		gTasks[taskId].func = Task_RaidBattleIntroWaitForKeyPress;
	}
}

static void PrintInstructions(void)
{
	struct TextColor partnerColour =
	{
		.bgColor = 0, //Transparent
		.fgColor = 2, //Dark Gray
		.shadowColor = 3, //Gray
	};

	struct TextColor colour =
	{
		.bgColor = 0, //Transparent
		.fgColor = 1, //White
		.shadowColor = 2,
	};

	WindowPrint(WIN_RULES, 0, 0, 2, &colour, 0, gText_RaidBattleRules);

	StringCopy(gStringVar1, gText_RecommendedLevel);
	ConvertIntToDecimalStringN(gStringVar2, GetRaidRecommendedLevel(), 0, 3);
	StringAppend(gStringVar1, gStringVar2);
	WindowPrint(WIN_RECOMMENDED_LEVEL, 0, 2, 0, &colour, 0, gStringVar1);

	WindowPrint(WIN_INSTRUCTIONS, 0, 2, 4, &colour, 0, gText_RaidIntroSelection);

	WindowPrint(WIN_CHOOSE_PARTNER, 3, 1, 4, &partnerColour, 0, gText_RaidBattleChoosePartner);
}

static void ShowStars(void)
{
	LoadCompressedSpriteSheetUsingHeap(&sRaidBattleStarSpriteSheet);
	LoadCompressedSpritePaletteUsingHeap(&sRaidBattleStarSpritePalette);

	for (int i = 0; i < sRaidBattleIntroPtr->rank; ++i)
		CreateSprite(&sRaidBattleStarSpriteTemplate, 9 + (9 * i), 8, 0);
}

static void ShowRaidPokemonSprite(void)
{
	u16 species = sRaidBattleIntroPtr->species;
	u32 personality = 0xFFFFFFFF; //For male sprite //sRaidBattleIntroPtr->personality;
	u32 otid = T1_READ_32(gSaveBlock2->playerTrainerId);

	const struct CompressedSpritePalette* pal = GetMonSpritePalStructFromOtIdPersonality(species, otid, personality);

	//Create black silhouette
	sRaidBattleIntroPtr->monSpriteId = CreateMonPicSprite_HandleDeoxys(species, otid, personality, 1, 45, 57, 0, pal->tag);

	u16* palette = &gPlttBufferUnfaded2[IndexOfSpritePaletteTag(pal->tag) * 16];
	for (int i = 0; i < 16; i++)
		*palette++ = RGB(4, 4, 4);

	//Create white outline
	*(palette - 1) = RGB(31, 31, 31);
}

static void ShowRaidPokemonTypes(void)
{
	u16 species = sRaidBattleIntroPtr->species;
	u8 type1 = gBaseStats[species].type1;
	u8 type2 = gBaseStats[species].type2;

	blit_move_info_icon(WIN_TYPE_1, type1 + 1, 0, 2);
	if (type1 != type2)
		blit_move_info_icon(WIN_TYPE_2, type2 + 1, 0, 2);
}

static void ShowPartnerTeams(void)
{
	u8 i, j;

	for (i = 0; i < MAX_NUM_PARTNERS; ++i)
	{
		if (sRaidBattleIntroPtr->partners[i].graphicsId != 0)
		{
			AddPseudoEventObject(sRaidBattleIntroPtr->partners[i].graphicsId, SpriteCallbackDummy, 126, 59 + (i * 33), 0);

			for (j = 0; j < MAX_TEAM_SIZE; ++j)
			{
				u16 species = sRaidBattleIntroPtr->partners[i].team[j];
				TryRandomizeSpecies(&species);
				if (species != SPECIES_NONE)
				{
					LoadMonIconPalette(species);
					CreateMonIcon(species, SpriteCB_PokeIcon, 158 + (32 * j), 59 + (i * 33), 0, 0xFFFFFFFF, FALSE);
				}
			}
		}
	}
}

static void ShowSelectionArrow(void)
{
	LoadCompressedSpriteSheetUsingHeap(&sRaidBattleCursorSpriteSheet);
	LoadCompressedSpritePaletteUsingHeap(&sRaidBattleCursorSpritePalette);
	CreateSprite(&sRaidBattleCursorSpriteTemplate, 95, 59, 0);
}

static void MainCB2_RaidBattleIntro(void)
{
	RunTasks();
	AnimateSprites();
	BuildOamBuffer();
	UpdatePaletteFade();
}

typedef void (*TransferPlttBuffer_T) (void);
#define TransferPlttBuffer ((TransferPlttBuffer_T) (0x08070474 |1))
static void VBlankCB_RaidBattleIntro(void)
{
	LoadOam();
	ProcessSpriteCopyRequests();
	TransferPlttBuffer();
}

//Cleans the windows
static void CleanWindows(void)
{
	for (int i = 0; i < WINDOW_COUNT; ++i)
		FillWindowPixelBuffer(i, PIXEL_FILL(0));
}
//Display commited windows
static void CommitWindows(void)
{
	for (u8 i = 0; i < WINDOW_COUNT; ++i)
	{
		CopyWindowToVram(i, 3);
		PutWindowTilemap(i);
	}
}

static void InitRaidBattleIntro(void)
{
	CleanWindows();
	CommitWindows();

	ShowStars();
	PrintInstructions();
	ShowRaidPokemonSprite();
	ShowRaidPokemonTypes();
	ShowPartnerTeams();
	ShowSelectionArrow();

	//Display newly commited windows
	CommitWindows();
}

static void LoadRaidBattleIntroGfx(void)
{
	decompress_and_copy_tile_data_to_vram(2, &RaidBattleIntroBGTiles, 0, 0, 0);
	LZDecompressWram(RaidBattleIntroBGMap, sRaidBattleIntroPtr->tilemapPtr);
	#ifdef UNBOUND
	LoadPalette(RaidBattleIntroBGUnboundPal, 0, 0x20);
	#else
	LoadPalette(RaidBattleIntroBGPal, 0, 0x20);
	#endif
	LoadMenuElementsPalette(0xC0, 1);
	Menu_LoadStdPalAt(0xF0);
}

static void ClearTasksAndGraphicalStructs(void)
{
	ScanlineEffect_Stop();
	ResetTasks();
	ResetSpriteData();
	ResetTempTileDataBuffers();
	ResetPaletteFade();
	FreeAllSpritePalettes();
}

static void ClearVramOamPlttRegs(void)
{
	DmaFill16(3, 0, VRAM, VRAM_SIZE);
	DmaFill32(3, 0, OAM, OAM_SIZE);
	DmaFill16(3, 0, PLTT, PLTT_SIZE);
	SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_ON | DISPCNT_OBJ_1D_MAP);
	SetGpuReg(REG_OFFSET_BG3CNT, DISPCNT_MODE_0);
	SetGpuReg(REG_OFFSET_BG2CNT, DISPCNT_MODE_0);
	SetGpuReg(REG_OFFSET_BG1CNT, DISPCNT_MODE_0);
	SetGpuReg(REG_OFFSET_BG0CNT, DISPCNT_MODE_0);
	SetGpuReg(REG_OFFSET_BG3HOFS, DISPCNT_MODE_0);
	SetGpuReg(REG_OFFSET_BG3VOFS, DISPCNT_MODE_0);
	SetGpuReg(REG_OFFSET_BG2HOFS, DISPCNT_MODE_0);
	SetGpuReg(REG_OFFSET_BG2VOFS, DISPCNT_MODE_0);
	SetGpuReg(REG_OFFSET_BG1HOFS, DISPCNT_MODE_0);
	SetGpuReg(REG_OFFSET_BG1VOFS, DISPCNT_MODE_0);
	SetGpuReg(REG_OFFSET_BG0HOFS, DISPCNT_MODE_0);
	SetGpuReg(REG_OFFSET_BG0VOFS, DISPCNT_MODE_0);
}

static void CB2_RaidBattleIntro(void)
{
	if (gMain.state <= 7)
	{
		switch (gMain.state) {
			case 0:
				SetVBlankCallback(NULL);
				ClearVramOamPlttRegs();
				gMain.state++;
				break;
			case 1:
				ClearTasksAndGraphicalStructs();
				gMain.state++;
				break;
			case 2:
				sRaidBattleIntroPtr->tilemapPtr = Malloc(0x1000);
				ResetBgsAndClearDma3BusyFlags(0);
				InitBgsFromTemplates(0, sRaidBattleIntroBgTemplates, 3);
				SetBgTilemapBuffer(2, sRaidBattleIntroPtr->tilemapPtr);
				gMain.state++;
				break;
			case 3:
				LoadRaidBattleIntroGfx();
				gMain.state++;
				break;
			case 4:
				if (!free_temp_tile_data_buffers_if_possible())
				{
					ShowBg(0);
					ShowBg(1);
					ShowBg(2);
					CopyBgTilemapBufferToVram(2);
					gMain.state++;
				}
				break;
			case 5:
				InitWindows(sRaidBattleIntroWinTemplates);
				DeactivateAllTextPrinters();
				gMain.state++;
				break;
			case 6:
				BeginNormalPaletteFade(0xFFFFFFFF, 0, 16, 0, RGB_BLACK);
				gMain.state++;
				break;
			case 7:
				SetVBlankCallback(VBlankCB_RaidBattleIntro);
				InitRaidBattleIntro();
				CreateTask(Task_RaidBattleIntroFadeIn, 0);
				SetMainCallback2(MainCB2_RaidBattleIntro);
				gMain.state = 0;
				break;
		}
	}
}

void sp115_RaidBattleAvailable(void)
{
	if (!HasRaidBattleAlreadyBeenDone()
	&& (GetRaidBattleData() || FlagGet(FLAG_BATTLE_FACILITY)))
		gSpecialVar_LastResult = TRUE;
	else
		gSpecialVar_LastResult = FALSE;
}

void sp116_StartRaidBattleIntro(void)
{
	sRaidBattleIntroPtr = Calloc(sizeof(struct RaidBattleIntro));
	gSpecialVar_LastResult = FALSE;

	if (GetRaidBattleData())
	{
		PlayRainStoppingSoundEffect();
		SetMainCallback2(CB2_RaidBattleIntro);
	}
}
