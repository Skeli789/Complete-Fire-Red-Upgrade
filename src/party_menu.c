#include "defines.h"
#include "defines_battle.h"
#include "../include/evolution_scene.h"
#include "../include/field_control_avatar.h"
#include "../include/field_player_avatar.h"
#include "../include/field_effect.h"
#include "../include/field_screen_effect.h"
#include "../include/field_weather.h"
#include "../include/fieldmap.h"
#include "../include/item_use.h"
#include "../include/item_menu.h"
#include "../include/menu.h"
#include "../include/metatile_behavior.h"
#include "../include/overworld.h"
#include "../include/party_menu.h"
#include "../include/pokemon_icon.h"
#include "../include/pokemon_storage_system.h"
#include "../include/pokemon_summary_screen.h"
#include "../include/random.h"
#include "../include/script.h"
#include "../include/sound.h"
#include "../include/start_menu.h"
#include "../include/string_util.h"
#include "../include/text.h"
#include "../include/wild_encounter.h"
#include "../include/window.h"
#include "../include/constants/abilities.h"
#include "../include/constants/hold_effects.h"
#include "../include/constants/items.h"
#include "../include/constants/item_effects.h"
#include "../include/constants/moves.h"
#include "../include/constants/pokemon.h"
#include "../include/constants/pokedex.h"
#include "../include/constants/region_map_sections.h"
#include "../include/constants/songs.h"

#include "../include/new/battle_strings.h"
#include "../include/new/build_pokemon.h"
#include "../include/new/dns.h"
#include "../include/new/evolution.h"
#include "../include/new/follow_me.h"
#include "../include/new/form_change.h"
#include "../include/new/item.h"
#include "../include/new/learn_move.h"
#include "../include/new/multi.h"
#include "../include/new/overworld.h"
#include "../include/new/party_menu.h"
#include "../include/new/util.h"

/*
party_menu.c
	handles anything related to the party menu, such as field moves, new party menu GUIs, etc...
*/

#define MENU_UP -1
#define MENU_DOWN 1
#define MENU_LEFT -2
#define MENU_RIGHT 2

struct PartyMenuBoxInfoRects
{
	void (*blitFunc)(u8 windowId, u8 x, u8 y, u8 width, u8 height, bool8 isEgg);
	u8 dimensions[24];
	u8 descTextLeft;
	u8 descTextTop;
	u8 descTextWidth;
	u8 descTextHeight;
};

struct PartyMenuBox
{
	const struct PartyMenuBoxInfoRects *infoRects;
	const u8 *spriteCoords;
	u8 windowId;
	u8 monSpriteId;
	u8 itemSpriteId;
	u8 pokeballSpriteId;
	u8 statusSpriteId;
};

extern struct PartyMenuBox* sPartyMenuBoxes;

//Vanilla Static Functions
void __attribute__((long_call)) CreatePartyMonSprites(u8 slot);
void __attribute__((long_call)) RenderPartyMenuBox(u8 slot);
u8 __attribute__((long_call)) DisplayPartyMenuMessage(const u8* str, u8 b);
void __attribute__((long_call)) ReturnToUseOnWhichMon(u8 taskId);
void __attribute__((long_call)) DisplayPartyPokemonNickname(struct Pokemon* mon, struct PartyMenuBox* ptr, u8 c);
void __attribute__((long_call)) DisplayPartyPokemonLevelCheck(struct Pokemon*, struct PartyMenuBox*, u8);
void __attribute__((long_call)) DisplayPartyPokemonGenderNidoranCheck(struct Pokemon*, struct PartyMenuBox*, u8);
void __attribute__((long_call)) DisplayPartyPokemonOtherText(u8, struct PartyMenuBox*, u8);
void __attribute__((long_call)) UpdatePartyMonHeldItemSprite(struct Pokemon *mon, struct PartyMenuBox* ptr);
void __attribute__((long_call)) HandleChooseMonCancel(u8 taskId, s8 *ptr);
s8 __attribute__((long_call)) GetNewSlotDoubleLayout(s8 a, s8 b);
void __attribute__((long_call)) Task_ReturnToChooseMonAfterText(u8 taskId);
void __attribute__((long_call)) PartyMenuRemoveWindow(u8* ptr);
void __attribute__((long_call)) CB2_ShowPokemonSummaryScreen(void);
void __attribute__((long_call)) Task_UpdatePartyMenuHeldItemSprite(u8 taskId);
void __attribute__((long_call)) PartyMenu_MoveCursorToConfirm(void);
bool8 __attribute__((long_call)) NotUsingHPEVItemOnShedinja(struct Pokemon *mon, u16 item);
bool8 __attribute__((long_call)) IsItemFlute(u16 item);
bool8 __attribute__((long_call)) ExecuteTableBasedItemEffect_(u8 partyMonIndex, u16 item, u8 monMoveIndex);
bool8 __attribute__((long_call)) IsHPRecoveryItem(u16 item);
void __attribute__((long_call)) SetPartyMonAilmentGfx(struct Pokemon *mon, struct PartyMenuBox *menuBox);
void __attribute__((long_call)) Task_PartyMenuModifyHP(u8 taskId);
void __attribute__((long_call)) ResetHPTaskData(u8 taskId, u8 caseId, u32 hp);
void __attribute__((long_call)) GetMedicineItemEffectMessage(u16 item);
void __attribute__((long_call)) Task_DisplayHPRestoredMessage(u8 taskId);
void __attribute__((long_call)) UpdateMonDisplayInfoAfterRareCandy(u8 slot, struct Pokemon *mon);
void __attribute__((long_call)) PartyMenu_DisplayMonNeedsToReplaceMove(u8 taskId);
void __attribute__((long_call)) PartyMenu_DisplayMonLearnedMove(u8 taskId, u16 move);
void __attribute__((long_call)) ShiftMoveSlot(struct Pokemon *mon, u8 slotTo, u8 slotFrom);
void __attribute__((long_call)) PartyMenuTryEvolution(u8 taskId);
void __attribute__((long_call)) FreePartyPointers(void);
void __attribute__((long_call)) PartyMenuDisplayYesNoMenu(void);
void __attribute__((long_call)) ItemUseCB_RareCandyStep(u8 taskId, UNUSED TaskFunc func);
void __attribute__((long_call)) sub_8124DC0(u8 taskId);

//This file's functions:
static void OpenSummary(u8 taskId);
static void DisplayPartyPokemonSelectDataSpecial(u8 slot, u8 stringID);
static void DisplayPartyPokemonPriorityText(u8 stringID, struct PartyMenuBox* ptr, u8 c);
static bool8 SetUpFieldMove_Fly(void);
static bool8 SetUpFieldMove_Surf(void);
static bool8 SetUpFieldMove_Waterfall(void);
static bool8 SetUpFieldMove_Teleport(void);
static void FieldCallback_Dive(void);
static bool8 SetUpFieldMove_Dive(void);
static bool8 SetUpFieldMove_SweetScent(void);
static void FieldCallback_RockClimb(void);
static bool8 SetUpFieldMove_RockClimb(void);
static void FieldCallback_Defog(void);
static bool8 SetUpFieldMove_Defog(void);
static void CursorCb_MoveItemCallback(u8 taskId);
static void CursorCb_MoveItem(u8 taskId);

//*highlightedMon = 0 is Player's Pokemon out
//*highlightedMon = 1 is Link Partner's Pokemon out
/*FR LG Format is:		DPPT+ Format is:
	2						0	1
0	3						2	4
1	4						3	5
	5

	7							7
*/
//*highlightedMon = 7 is when the party menu cancel button is selected
//This is party menu selection used in Multi Battles
void SetNewPartySelectTarget2(s8* highlightedMon, s8 movementDir)
{

#ifdef GEN4_PLUS_SELECTION_SCREEN
	switch (movementDir)
	{
	case MENU_UP:
		if (*highlightedMon != 6 && *highlightedMon != 7)
			sPartyMenuInternal->lastSelectedSlot = *highlightedMon;

		switch (*highlightedMon) {
			case 0:
			case 1:
				*highlightedMon = 7; //Goto cancel button
				break;

			case 2:
				*highlightedMon -= 2;
				break;

			case 3:
			case 5:
				--(*highlightedMon);
				break;

			case 4:
				*highlightedMon -= 3;
				break;

			case 6:
				*highlightedMon = gPlayerPartyCount - 1;
				break;

			case 7:
				switch (sPartyMenuInternal->lastSelectedSlot) {
					case 0: //You just looped around from your side (arrived at cancel by pressing up)
						if (gPlayerParty[3].species != SPECIES_NONE)
							*highlightedMon = 3; //Go to your last mon from cancel
						else if (gPlayerParty[2].species != SPECIES_NONE)
							*highlightedMon = 2;
						else
							*highlightedMon = 0;
						break;

					case 1: //You just looped around from partner's side (arrived at cancel by pressing up)
						if (gPlayerParty[5].species != SPECIES_NONE)
							*highlightedMon = 5; //Go to partner's last mon from cancel
						else if (gPlayerParty[4].species != SPECIES_NONE)
							*highlightedMon = 4;
						else
							*highlightedMon = 1;
						break;

					default: //Go back to whichever mon you were on before (arrived at cancel by pressing down)
						*highlightedMon = sPartyMenuInternal->lastSelectedSlot;
				}
				break;
		}
		break;

	case MENU_DOWN:
		if (*highlightedMon != 6 && *highlightedMon != 7)
			sPartyMenuInternal->lastSelectedSlot = *highlightedMon;

		switch (*highlightedMon) {
			case 0:
				if (gPlayerParty[2].species != SPECIES_NONE)
					*highlightedMon = 2;
				else
					*highlightedMon = 7;
				break;

			case 1:
				if (gPlayerParty[4].species != SPECIES_NONE)
					*highlightedMon = 4;
				else
					*highlightedMon = 7;
				break;

			case 2:
			case 4:
				if (gPlayerParty[*highlightedMon + 1].species != SPECIES_NONE)
					*highlightedMon = *highlightedMon + 1;
				else
					*highlightedMon = 7;
				break;

			case 3:
			case 5:
			case 6:
				*highlightedMon = 7;
				break;

			case 7:
				switch (sPartyMenuInternal->lastSelectedSlot) {
					case 0:
					case 1:
						*highlightedMon = sPartyMenuInternal->lastSelectedSlot;
						break;

					case 4:
					case 5:
						*highlightedMon = 1;
						break;

					case 2:
					case 3:
						*highlightedMon = 0;
				}
				break;
		}
		break;

	case MENU_RIGHT:
		if (*highlightedMon != 6 && *highlightedMon != 7)
			sPartyMenuInternal->lastSelectedSlot = *highlightedMon;

	PICK_AGAIN_RIGHT:
		switch (*highlightedMon) {
			case 0:
			case 1:
				*highlightedMon += 1;
				break;

			case 2:
			case 3:
				*highlightedMon += 2;
				break;

			case 4:
				*highlightedMon -= 1;
				break;

			case 5:
				*highlightedMon = 7;
				break;

			case 7:
				*highlightedMon = 0;
				break;
		}
		if (*highlightedMon != 7 && gPlayerParty[*highlightedMon].species == 0)
			goto PICK_AGAIN_RIGHT;
		break;

	case MENU_LEFT:
		 if (*highlightedMon != 6 && *highlightedMon != 7)
			sPartyMenuInternal->lastSelectedSlot = *highlightedMon;

	PICK_AGAIN_LEFT:
		switch (*highlightedMon) {
			case 0:
				*highlightedMon = 7;
				break;

			case 1:
			case 2:
				*highlightedMon -= 1;
				break;

			case 4:
			case 5:
				*highlightedMon -= 2;
				break;

			case 3:
				*highlightedMon += 1;
				break;

			case 7:
				*highlightedMon = 5;
				break;
		}
		if (*highlightedMon != 7 && gPlayerParty[*highlightedMon].species == 0)
			goto PICK_AGAIN_LEFT;
		break;
	}

#else
	s8 unk2 = movementDir;

	switch (movementDir)
	{
	case MENU_UP:
		if (*highlightedMon == 0)
		{
			*highlightedMon = 7;
			break;
		}
		else if (*highlightedMon == 6) //No idea what this is for
		{
			*highlightedMon = gPlayerPartyCount - 1;
			break;
		}
		else if (*highlightedMon == 7)
		{
			if (sPartyMenuInternal->chooseHalf)
			{
				*highlightedMon = 6; //Set to 6 here because the minus will set it to 5
				break;
			}
			(*highlightedMon)--;
		}

		unk2 = GetNewSlotDoubleLayout(*highlightedMon, unk2);
		if (unk2 != -1)
			*highlightedMon = unk2;
		break;

	case MENU_DOWN:
		if (*highlightedMon == 6)
		{
			*highlightedMon = 7;
		}
		else if (*highlightedMon == 7)
		{
			*highlightedMon = 0;
		}
		else
		{
			unk2 = GetNewSlotDoubleLayout(*highlightedMon, 1);
			if (unk2 == -1)
			{
				if (sPartyMenuInternal->chooseHalf)
					*highlightedMon = 6;
				else
					*highlightedMon = 7;
			}
			else
			{
				*highlightedMon = unk2;
			}
		}
		break;
	case MENU_RIGHT:
		if (*highlightedMon == 0)
		{
			if (sPartyMenuInternal->lastSelectedSlot == 3)
			{
				if (gPlayerParty[3].species != SPECIES_NONE)
					*highlightedMon = 3; //Go to your last mon
			}
			else if (gPlayerParty[2].species != SPECIES_NONE)
			{
				*highlightedMon = 2; //Go to the pokemon at the top right (your second last mon)
			}
		}
		else if (*highlightedMon == 1)
		{
			if (sPartyMenuInternal->lastSelectedSlot == 5)
			{
				if (gPlayerParty[5].species != SPECIES_NONE)
					*highlightedMon = 5;
			}
			else if (gPlayerParty[4].species != SPECIES_NONE)
			{
				*highlightedMon = 4;
			}
		}
		break;
	case MENU_LEFT:
		if (*highlightedMon == 2 || *highlightedMon == 3) //Your two mons
		{
			sPartyMenuInternal->lastSelectedSlot = *highlightedMon;
			*highlightedMon = 0;
		}
		else if (*highlightedMon == 4 || *highlightedMon == 5)
		{
			sPartyMenuInternal->lastSelectedSlot = *highlightedMon;
			*highlightedMon = 1;
		}
		break;
	}
#endif
}

void CursorCb_Summary(u8 taskId)
{
	PlaySE(SE_SELECT);
	sPartyMenuInternal->exitCallback = CB2_ShowPokemonSummaryScreen; //sub_81B3828 in Emerald

	if (gMain.inBattle != 0)
		OpenSummary(taskId);
	else
		Task_ClosePartyMenu(taskId); //sub_81B12C0 in Emerald
}

static void OpenSummary(u8 taskId)
{
	Task_ClosePartyMenu(taskId); //Replace this
}

// Summary Screen wraps around
// Credit to Sagiri: https://github.com/Sagiri/sswa

#define PAGE_INFO	   0
#define PAGE_SKILLS	 1
#define PAGE_ATTACKS	2

u8 ChangeSummaryScreenMonSinglesDoubles(u8 delta)
{
	u8 numMons = sMonSummaryScreen->lastIndex + 1;
	delta += numMons;

	//Guarantees result will be in range [0, numMons)
	u8 result = (sLastViewedMonIndex + delta) % numMons;

	//Skip over eggs on other pages
	if (sMonSummaryScreen->curPageIndex != PAGE_INFO)
	{
		while (GetMonData(&sMonSummaryScreen->monList.mons[result], MON_DATA_IS_EGG, NULL))
			result = (result + delta) % numMons;
	}

	// necessary to gracefully handle parties of 1 Pokemon
	if (result == sLastViewedMonIndex)
		return -1;

	return result;
};

#define sMultiPokemonPartyMenuOrder ((u8*) 0x8463FB8)
s8 ChangeSummaryScreenMonMulti(s8 delta)
{
	u8 i;
	u8 listPos = 0;
	u8 numMons = PARTY_SIZE;
	delta += numMons;

	for (i = 0; i < PARTY_SIZE; ++i)
	{
		if (sMultiPokemonPartyMenuOrder[i] == sLastViewedMonIndex)
		{
			listPos = i;
			break;
		}
	}

	//Guarantees result will be in range [0, numMons)
	u8 result = (listPos + delta) % numMons;

	//Skip over eggs on other pages
	if (sMonSummaryScreen->curPageIndex != PAGE_INFO)
	{
		while (GetMonData(&gPlayerParty[sMultiPokemonPartyMenuOrder[result]], MON_DATA_IS_EGG, NULL))
			result = (result + delta) % numMons;
	}

	//Skip over empty slots
	while (GetMonData(&gPlayerParty[sMultiPokemonPartyMenuOrder[result]], MON_DATA_SPECIES, NULL) == SPECIES_NONE)
		result = (result + delta) % numMons;

	return sMultiPokemonPartyMenuOrder[result];
}

bool8 SummaryScreen_IsMultiBattlePartner(void)
{
	#ifdef FLAG_VIEWING_MULTI_TRAINER_TEAM
	if (FlagGet(FLAG_VIEWING_MULTI_TRAINER_TEAM))
		return TRUE;
	#endif

	#ifdef FLAG_USE_HALF_PARTNER_TEAM
	if (FlagGet(FLAG_USE_HALF_PARTNER_TEAM))
	{
		u8 multiTrainerId = gTrainers[VarGet(VAR_PARTNER)].encounterMusic;

		if (multiTrainerId > 0
		&& GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_OT_ID, NULL) == gFrontierMultiBattleTrainers[multiTrainerId - 1].otId)
			return TRUE;
	}
	#endif

    return IsMultiBattle()
		&& (sLastViewedMonIndex >= 4 || sLastViewedMonIndex == 1);
}

bool32 SummaryScreenMonIsFromGBA(void)
{
    u8 version = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_MET_GAME, 0);
	return version == VERSION_FIRE_RED; //Not compatible with any other game
}

bool8 TryReplaceSummaryScreenLocationWithFarAwayLocation(struct Pokemon* mon, u8 metLocation)
{
	if (IsTradedMon(mon) && !SummaryScreen_IsMultiBattlePartner())
		metLocation = 0; //Forces "met in a trade"

	return metLocation;
}


//Battle Tower Selection Updates//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern u8 gOtherText_NoMoreOnePoke[];
#define gOtherText_NoMoreTwoPoke (u8*) 0x8416B3E
#define gOtherText_NoMoreThreePoke (u8*) 0x8416B16
extern u8 gOtherText_NoMoreFourPoke[];
extern u8 gOtherText_NoMoreFivePoke[];

extern u8 gText_First[];
extern u8 gText_Second[];
extern u8 gText_Third[];
extern u8 gText_Fourth[];
extern u8 gText_Fifth[];
extern u8 gText_Sixth[];

static const u8* sChoosePokemonMaxStrings[PARTY_SIZE - 1] =
{
	gOtherText_NoMoreOnePoke,
	gOtherText_NoMoreTwoPoke,
	gOtherText_NoMoreThreePoke,
	gOtherText_NoMoreFourPoke,
	gOtherText_NoMoreFivePoke,
};

static const u8* const sOrderStrings[PARTY_SIZE] =
{
	gText_First,
	gText_Second,
	gText_Third,
	gText_Fourth,
	gText_Fifth,
	gText_Sixth,
};

u8 ChoosePokemon_LoadMaxPKMNStr(const u8** strPtr, bool8 loadString)
{
	u8 max = GetNumMonsOnTeamInFrontier();

	if (FlagGet(FLAG_BATTLE_FACILITY))
	{
		if (loadString)
		{
			if (max < 6)
				*strPtr = sChoosePokemonMaxStrings[max - 1];
			else
				*strPtr = sChoosePokemonMaxStrings[0]; //Can't select more than 6 anyways
		}
	}
	else
	{
		max = 3;

		if (loadString)
			*strPtr = gOtherText_NoMoreThreePoke;
	}

	return max;
}

void CursorCb_Enter(u8 taskId)
{
	u8 max;
	const u8* string = NULL;
	int i;

	if (gPartyMenu.unk_8_6 == 2)
	{
		max = 2;
		string = sChoosePokemonMaxStrings[max - 1];
	}
	else
		max = ChoosePokemon_LoadMaxPKMNStr(&string, TRUE);

	PartyMenuRemoveWindow(&sPartyMenuInternal->windowId[0]);
	PartyMenuRemoveWindow(&sPartyMenuInternal->windowId[1]);

	bool8 eligibleIndices[PARTY_SIZE] = {FALSE};
	for (i = 0; i < PARTY_SIZE; ++i)
		eligibleIndices[i] = IsMonAllowedInBattleTower(&gPlayerParty[i]); //Record who's currently eligible

	for (i = 0; i < max; ++i)
	{
		if (gSelectedOrderFromParty[i] == 0)
		{
			PlaySE(SE_SELECT);
			gSelectedOrderFromParty[i] = gPartyMenu.slotId + 1;
			DisplayPartyPokemonPriorityText(i, &sPartyMenuBoxes[gPartyMenu.slotId], 1);

			if (i == (max - 1))
				PartyMenu_MoveCursorToConfirm();

			DisplayPartyMenuStdMessage(0);
			gTasks[taskId].func = Task_HandleChooseMonInput;

			for (i = 0; i < PARTY_SIZE; ++i) //Reload everyone else's text if needed
			{
				if (eligibleIndices[i] //Was eligible before the selection
				&& !IsMonAllowedInBattleTower(&gPlayerParty[i]) //Mon is now ineligible
				&& GetMonData(&gPlayerParty[i], MON_DATA_SPECIES, NULL) != SPECIES_NONE)
					DisplayPartyPokemonSelectData(i, 6);  //Reload eligibility text
			}

			return;
		}
	}

	PlaySE(SE_ERROR);
	DisplayPartyMenuMessage(string, 1);
	gTasks[taskId].func = Task_ReturnToChooseMonAfterText;
}

void CursorCb_NoEntry(u8 taskId)
{
	int i, j;
	u8 max = ChoosePokemon_LoadMaxPKMNStr(NULL, FALSE);

	PlaySE(SE_SELECT);
	PartyMenuRemoveWindow(&sPartyMenuInternal->windowId[0]);
	PartyMenuRemoveWindow(&sPartyMenuInternal->windowId[1]);

	bool8 eligibleIndices[PARTY_SIZE] = {FALSE};
	for (i = 0; i < PARTY_SIZE; ++i)
		eligibleIndices[i] = IsMonAllowedInBattleTower(&gPlayerParty[i]); //Record who's currently eligible

	for (i = 0; i < max; ++i) //Remove chosen mon
	{
		if (gSelectedOrderFromParty[i] == gPartyMenu.slotId + 1)
		{
			for (j = i; j < (max - 1); ++j)
				gSelectedOrderFromParty[j] = gSelectedOrderFromParty[j + 1];

			gSelectedOrderFromParty[j] = 0;
			break;
		}
	}

	DisplayPartyPokemonOtherText(1, &sPartyMenuBoxes[gPartyMenu.slotId], 1);
	for (i = 0; i < (max - 1); ++i) //Reload the order text
	{
		if (gSelectedOrderFromParty[i] != 0)
			DisplayPartyPokemonPriorityText(i, &sPartyMenuBoxes[gSelectedOrderFromParty[i] - 1], 1);
	}

	for (i = 0; i < PARTY_SIZE; ++i) //Reload everyone else's text if needed
	{
		if (!eligibleIndices[i] //Wasn't eligible before the cancel
		&& IsMonAllowedInBattleTower(&gPlayerParty[i]) //Mon is now eligible
		&& GetMonData(&gPlayerParty[i], MON_DATA_SPECIES, NULL) != SPECIES_NONE) //Legit Pokemon
			DisplayPartyPokemonSelectData(i, 1);  //Reload eligibility text
	}

	DisplayPartyMenuStdMessage(0);
	gTasks[taskId].func = Task_HandleChooseMonInput;
}

void DisplayPartyPokemonSelectForBattle(u8 slot)
{
	u8 max;
	struct Pokemon *mon = &gPlayerParty[slot];
	u8* ptr = gSelectedOrderFromParty;

	if (gPartyMenu.unk_8_6 == 2)
		max = 2;
	else
		max = ChoosePokemon_LoadMaxPKMNStr(NULL, FALSE);

	if (!IsMonAllowedInBattleTower(mon))
	{
		DisplayPartyPokemonSelectData(slot, 6);
		return;
	}
	else
	{
		for (int i = 0; i < max; i++)
		{
			if (ptr[i] != 0 && (ptr[i] - 1) == slot)
			{
				DisplayPartyPokemonSelectDataSpecial(slot, i);
				return;
			}
		}
		DisplayPartyPokemonSelectData(slot, 1);
	}
}

static void DisplayPartyPokemonSelectDataSpecial(u8 slot, u8 stringID)
{
	struct Pokemon* mon = &gPlayerParty[slot];

	sPartyMenuBoxes[slot].infoRects->blitFunc(sPartyMenuBoxes[slot].windowId, 0, 0, 0, 0, 1);
	DisplayPartyPokemonNickname(mon, &sPartyMenuBoxes[slot], 0);
	if (!GetMonData(mon, MON_DATA_IS_EGG, NULL))
	{
		DisplayPartyPokemonLevelCheck(mon, &sPartyMenuBoxes[slot], 0);
		DisplayPartyPokemonGenderNidoranCheck(mon, &sPartyMenuBoxes[slot], 0);
	}

	DisplayPartyPokemonPriorityText(stringID, &sPartyMenuBoxes[slot], 0);
}

static void DisplayPartyPokemonPriorityText(u8 stringID, struct PartyMenuBox* ptr, u8 c)
{
	if (c != 0)
	{
		int unk = ((ptr->infoRects->descTextLeft % 8) + ptr->infoRects->descTextWidth + 7) / 8;
		int unk2 = ((ptr->infoRects->descTextTop % 8) + ptr->infoRects->descTextHeight + 7) / 8;
		ptr->infoRects->blitFunc(ptr->windowId, ptr->infoRects->descTextLeft >> 3, ptr->infoRects->descTextTop >> 3, unk, unk2, 1);
	}
	if (c != 2)
		WindowPrint(ptr->windowId, 1, ptr->infoRects->descTextLeft, ptr->infoRects->descTextTop, (void*) 0x8459FFC, 0, sOrderStrings[stringID]);
}

//Run when "Enter" is pressed
u8 CanPokemonSelectedBeEnteredInBattleTower(void)
{
	u8 i, j;
	u8 tier = VarGet(VAR_BATTLE_FACILITY_TIER);
	struct Pokemon* party = gPlayerParty;
	u8 maxLength = GetNumMonsOnTeamInFrontier();

	if (gSelectedOrderFromParty[maxLength - 1] == 0) //Not enough mon's entered
	{
		if (maxLength == 1)
			return 14;

		if (FlagGet(FLAG_BATTLE_FACILITY))
		{
			ConvertIntToDecimalStringN(gStringVar1, maxLength, 0, 1);
			return 17;
		}
	}

	if (!FlagGet(FLAG_BATTLE_FACILITY))
		return 0xFF;

	for (i = 0; i < maxLength - 1; ++i)
	{
		u16 species = GetMonData(&party[gSelectedOrderFromParty[i] - 1], MON_DATA_SPECIES, NULL);
		u16 item = GetMonData(&party[gSelectedOrderFromParty[i] - 1], MON_DATA_HELD_ITEM, NULL);
		for (j = i + 1; j < maxLength; j++)
		{
			if (tier != BATTLE_FACILITY_NO_RESTRICTIONS
			&& species == GetMonData(&party[gSelectedOrderFromParty[j] - 1], MON_DATA_SPECIES, NULL))
				return 18;
			if (item != 0 && DuplicateItemsAreBannedInTier(tier, VarGet(VAR_BATTLE_FACILITY_BATTLE_TYPE))
			&& item == GetMonData(&party[gSelectedOrderFromParty[j] - 1], MON_DATA_HELD_ITEM, NULL))
				return 19;
		}
	}

	return 0xFF;
}

//Field Moves///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern u8 gMoveNames[][MOVE_NAME_LENGTH + 1];

extern const u8 gMenuText_Move[];
extern const u8 gText_FieldMoveDesc_RockClimb[];
extern const u8 gText_FieldMoveDesc_Defog[];
extern const u8 gText_FieldMoveDesc_Dive[];

extern const u8 EventScript_RockClimb[];
extern const u8 EventScript_Defog[];

// Field Move IDs
enum FieldMovesIDs
{
	FIELD_MOVE_FLASH,
	FIELD_MOVE_CUT,
	FIELD_MOVE_FLY,
	FIELD_MOVE_STRENGTH,
	FIELD_MOVE_SURF,
	FIELD_MOVE_ROCK_SMASH,
	FIELD_MOVE_WATERFALL,
	FIELD_MOVE_TELEPORT,
	FIELD_MOVE_DIG,
	FIELD_MOVE_MILK_DRINK,
	FIELD_MOVE_SOFT_BOILED,
	FIELD_MOVE_SWEET_SCENT,
	FIELD_MOVE_ROCK_CLIMB,
	FIELD_MOVE_DEFOG,
	FIELD_MOVE_DIVE,
	FIELD_MOVE_COUNT
};

#define CursorCb_FieldMove (TaskFunc) 0x81245a5

struct
{
	const u8 *text;
	TaskFunc func;
} const gPartyMenuCursorOptions[] =
{
	[MENU_SUMMARY] =   {(void*) 0x8416994, (void*) 0x8122d4d},
	[MENU_SWITCH] =	{(void*) 0x841698d, (void*) 0x8122e01},
	[MENU_CANCEL1] =   {(void*) 0x84161c1, (void*) 0x8123641},
	[MENU_ITEM] =	  {(void*) 0x84161d4, (void*) 0x81236a1},
	[MENU_GIVE] =	  {(void*) 0x84161b2, (void*) 0x8123705},
	[MENU_TAKE_ITEM] = {(void*) 0x84161de, (void*) 0x8123c39},
	[MENU_MAIL] =	  {(void*) 0x84161d9, (void*) 0x8123cf5},
	[MENU_TAKE_MAIL] = {(void*) 0x84169b2, (void*) 0x8123e0d},
	[MENU_READ] =	  {(void*) 0x84169b7, (void*) 0x8123d59},
	[MENU_CANCEL2] =   {(void*) 0x84161c1, (void*) 0x8124065},
	[MENU_SHIFT] =	 {(void*) 0x841697e, (void*) 0x81240f5},
	[MENU_SEND_OUT] =  {(void*) 0x8416984, (void*) 0x81240f5},
	[MENU_ENTER] =	 {(void*) 0x84169a3, (void*) 0x8124155},
	[MENU_NO_ENTRY] =  {(void*) 0x84169a9, (void*) 0x8124279},
	[MENU_STORE] =	 {(void*) 0x84161e3, (void*) 0x8124355},
	[MENU_REGISTER] =  {(void*) 0x841b6f4, (void*) 0x8124385},
	[MENU_TRADE1] =	{(void*) 0x84169bc, (void*) 0x8124491},
	[MENU_TRADE2] =	{(void*) 0x84169bc, (void*) 0x81245a1},
	[MENU_MOVE_ITEM] = {gMenuText_Move, CursorCb_MoveItem},

	//Field Moves
	[MENU_FIELD_MOVES + FIELD_MOVE_FLASH] =	      {gMoveNames[MOVE_FLASH], CursorCb_FieldMove},
	[MENU_FIELD_MOVES + FIELD_MOVE_CUT] =		  {gMoveNames[MOVE_CUT], CursorCb_FieldMove},
	[MENU_FIELD_MOVES + FIELD_MOVE_FLY] =		  {gMoveNames[MOVE_FLY], CursorCb_FieldMove},
	[MENU_FIELD_MOVES + FIELD_MOVE_STRENGTH] =	  {gMoveNames[MOVE_STRENGTH], CursorCb_FieldMove},
	[MENU_FIELD_MOVES + FIELD_MOVE_SURF] =		  {gMoveNames[MOVE_SURF], CursorCb_FieldMove},
	[MENU_FIELD_MOVES + FIELD_MOVE_ROCK_SMASH] =  {gMoveNames[MOVE_ROCKSMASH], CursorCb_FieldMove},
	[MENU_FIELD_MOVES + FIELD_MOVE_WATERFALL] =   {gMoveNames[MOVE_WATERFALL], CursorCb_FieldMove},
	[MENU_FIELD_MOVES + FIELD_MOVE_TELEPORT] =	  {gMoveNames[MOVE_TELEPORT], CursorCb_FieldMove},
	[MENU_FIELD_MOVES + FIELD_MOVE_DIG] =		  {gMoveNames[MOVE_DIG], CursorCb_FieldMove},
	[MENU_FIELD_MOVES + FIELD_MOVE_MILK_DRINK] =  {gMoveNames[MOVE_MILKDRINK], CursorCb_FieldMove},
	[MENU_FIELD_MOVES + FIELD_MOVE_SOFT_BOILED] = {gMoveNames[MOVE_SOFTBOILED], CursorCb_FieldMove},
	[MENU_FIELD_MOVES + FIELD_MOVE_SWEET_SCENT] = {gMoveNames[MOVE_SWEETSCENT], CursorCb_FieldMove},
	[MENU_FIELD_MOVES + FIELD_MOVE_ROCK_CLIMB] =  {gMoveNames[MOVE_ROCKCLIMB], CursorCb_FieldMove},
	[MENU_FIELD_MOVES + FIELD_MOVE_DEFOG] =	      {gMoveNames[MOVE_DEFOG], CursorCb_FieldMove},
	[MENU_FIELD_MOVES + FIELD_MOVE_DIVE] =	 	  {gMoveNames[MOVE_DIVE], CursorCb_FieldMove},
};

struct
{
	bool8 (*fieldMoveFunc)(void);
	u8 msgId;
} const gFieldMoveCursorCallbacks[] =
{
	[FIELD_MOVE_FLASH] = {(void*) 0x80C9B2D, 0x0d},
	[FIELD_MOVE_CUT] = {(void*) 0x8097899, 0x07},
	[FIELD_MOVE_FLY] = {SetUpFieldMove_Fly, 0x0d},
	[FIELD_MOVE_STRENGTH] = {(void*) 0x80D07ED, 0x0d},
	[FIELD_MOVE_SURF] = {SetUpFieldMove_Surf, 0x08},
	[FIELD_MOVE_ROCK_SMASH] = {(void*) 0x80C99D9, 0x0d},
	[FIELD_MOVE_WATERFALL] = {SetUpFieldMove_Waterfall, 0x0d},
	[FIELD_MOVE_TELEPORT] = {SetUpFieldMove_Teleport, 0x0d},
	[FIELD_MOVE_DIG] = {(void*) 0x80C9A79, 0x0d},
	[FIELD_MOVE_MILK_DRINK] = {(void*) 0x80E5685, 0x10},
	[FIELD_MOVE_SOFT_BOILED] = {(void*) 0x80E5685, 0x10},
	[FIELD_MOVE_SWEET_SCENT] = {(void*) SetUpFieldMove_SweetScent, 0x0d},
	[FIELD_MOVE_ROCK_CLIMB] = {(void*) SetUpFieldMove_RockClimb, 0x0d},
	[FIELD_MOVE_DEFOG] = {(void*) SetUpFieldMove_Defog, 0x0d},
	[FIELD_MOVE_DIVE] = {SetUpFieldMove_Dive, 0x0d},
};

const u8* const gFieldMoveDescriptions[] =
{
	[FIELD_MOVE_FLASH] = (void*) 0x8417583, //Flash
	[FIELD_MOVE_CUT] = (void*) 0x8417533, //Cut
	[FIELD_MOVE_FLY] = (void*) 0x8417548, //Fly
	[FIELD_MOVE_STRENGTH] = (void*) 0x841756E, //Strength
	[FIELD_MOVE_SURF] = (void*) 0x841755D, //Surf
	[FIELD_MOVE_ROCK_SMASH] = (void*) 0x8417596, //Rock Smash
	[FIELD_MOVE_WATERFALL] = (void*) 0x84175AE, //Waterfall
	[FIELD_MOVE_TELEPORT] = (void*) 0x84175FB, //Teleport
	[FIELD_MOVE_DIG] = (void*) 0x84175CC, //Dig
	[FIELD_MOVE_MILK_DRINK] = (void*) 0x84175F1, //Milk Drink
	[FIELD_MOVE_SOFT_BOILED] = (void*) 0x84175F1, //Softboiled
	[FIELD_MOVE_SWEET_SCENT] = (void*) 0x84175DE, //Sweet Scent
	[FIELD_MOVE_ROCK_CLIMB] = gText_FieldMoveDesc_RockClimb,
	[FIELD_MOVE_DEFOG] = gText_FieldMoveDesc_Defog,
	[FIELD_MOVE_DIVE] = gText_FieldMoveDesc_Dive,
};

const u16 gFieldMoves[FIELD_MOVE_COUNT] =
{
	[FIELD_MOVE_FLASH] = MOVE_FLASH,
	[FIELD_MOVE_CUT] = MOVE_CUT,
	[FIELD_MOVE_FLY] = MOVE_FLY,
	[FIELD_MOVE_STRENGTH] = MOVE_STRENGTH,
	[FIELD_MOVE_SURF] = MOVE_SURF,
	[FIELD_MOVE_ROCK_SMASH] = MOVE_ROCKSMASH,
	[FIELD_MOVE_WATERFALL] = MOVE_WATERFALL,
	[FIELD_MOVE_TELEPORT] = MOVE_TELEPORT,
	[FIELD_MOVE_DIG] = MOVE_DIG,
	[FIELD_MOVE_MILK_DRINK] = MOVE_MILKDRINK,
	[FIELD_MOVE_SOFT_BOILED] = MOVE_SOFTBOILED,
	[FIELD_MOVE_SWEET_SCENT] = MOVE_SWEETSCENT,
	[FIELD_MOVE_ROCK_CLIMB] = MOVE_ROCKCLIMB,
	[FIELD_MOVE_DEFOG] = MOVE_DEFOG,
	[FIELD_MOVE_DIVE] = MOVE_DIVE,
};

#ifndef UNBOUND //MODIFY THIS

const u8 gFieldMoveBadgeRequirements[FIELD_MOVE_COUNT] =
{
	[FIELD_MOVE_FLASH] = 1,
	[FIELD_MOVE_CUT] = 2,
	[FIELD_MOVE_FLY] = 3,
	[FIELD_MOVE_STRENGTH] = 4,
	[FIELD_MOVE_SURF] = 5,
	[FIELD_MOVE_ROCK_SMASH] = 6,
	[FIELD_MOVE_WATERFALL] = 0,
	[FIELD_MOVE_ROCK_CLIMB] = 0,
	[FIELD_MOVE_DEFOG] = 0,
	[FIELD_MOVE_DIVE] = 0,
};

#else //For Pokemon Unbound

const u8 gFieldMoveBadgeRequirements[FIELD_MOVE_COUNT] =
{
	[FIELD_MOVE_FLY] = 1,
	[FIELD_MOVE_CUT] = 2,
	[FIELD_MOVE_ROCK_SMASH] = 3,
	[FIELD_MOVE_STRENGTH] = 4,
	[FIELD_MOVE_SURF] = 5,
	[FIELD_MOVE_ROCK_CLIMB] = 6,
	[FIELD_MOVE_WATERFALL] = 7,
	[FIELD_MOVE_DIVE] = 8,
	[FIELD_MOVE_DEFOG] = 0,
	[FIELD_MOVE_FLASH] = 0,
};

#endif

void SetPartyMonFieldSelectionActions(struct Pokemon *mons, u8 slotId)
{
	u8 i, j, k;
	#ifdef ONLY_CHECK_ITEM_FOR_HM_USAGE
	bool8 knowsFly = FALSE;
	bool8 knowsDig = FALSE;
	bool8 knowsCut = FALSE;
	#endif

	sPartyMenuInternal->numActions = 0;
	AppendToList(sPartyMenuInternal->actions, &sPartyMenuInternal->numActions, MENU_SUMMARY);

	#ifdef FLAG_SANDBOX_MODE
	if (FlagGet(FLAG_SANDBOX_MODE) && FlagGet(FLAG_SYS_GAME_CLEAR))
		goto SKIP_FIELD_MOVES;
	#endif

	#ifdef NO_FIELD_MOVES
	goto SKIP_FIELD_MOVES;
	#endif

	//Add field moves to action list
	for (i = 0, k = 0; i < MAX_MON_MOVES; ++i)
	{
		for (j = 0; j < NELEMS(gFieldMoves); ++j)
		{
			if (GetMonData(&mons[slotId], MON_DATA_MOVE1 + i, NULL) == gFieldMoves[j])
			{
				#ifdef ONLY_CHECK_ITEM_FOR_HM_USAGE
				if (gFieldMoves[j] == MOVE_ROCKCLIMB
				&& !CheckBagHasItem(ITEM_HM08_ROCK_CLIMB, 1))
					continue; //Don't allow Rock Climbing until the item is obtained
				#endif

				AppendToList(sPartyMenuInternal->actions, &sPartyMenuInternal->numActions, j + MENU_FIELD_MOVES);
				++k;

				#ifdef ONLY_CHECK_ITEM_FOR_HM_USAGE
				if (gFieldMoves[j] == MOVE_FLY)
					knowsFly = TRUE; //No point in appending Fly if it is already there
				else if (gFieldMoves[j] == MOVE_DIG)
					knowsDig = TRUE;
				else if (gFieldMoves[j] == MOVE_CUT)
					knowsCut = TRUE;
				#endif
			}
		}
	}

	//Try to give the mon fly
	#ifdef ONLY_CHECK_ITEM_FOR_HM_USAGE
	u16 species = GetMonData(&mons[slotId], MON_DATA_SPECIES2, NULL);
	if (species != SPECIES_NONE && species != SPECIES_EGG)
	{
		#ifdef UNBOUND
		if (k < MAX_MON_MOVES && !knowsCut) //Doesn't know 4 field moves
		{
			if (GetCurrentRegionMapSectionId() == MAPSEC_GRIM_WOODS
			&& VarGet(VAR_SQ_WEED_WHACKER) > 0 && VarGet(VAR_SQ_WEED_WHACKER) < 2 //Weed Whacker in progress
			#ifndef DEBUG_HMS
			&& HasBadgeToUseFieldMove(FIELD_MOVE_CUT)
			&& (FlagGet(FLAG_BOUGHT_ADM) || FlagGet(FLAG_SANDBOX_MODE) ||
			 (CheckBagHasItem(ITEM_HM01_CUT, 1) > 0 && CanMonLearnTMTutor(&mons[slotId], ITEM_HM01_CUT, 0) == CAN_LEARN_MOVE))
			#endif
			)
			{
				AppendToList(sPartyMenuInternal->actions, &sPartyMenuInternal->numActions, MENU_FIELD_MOVES + FIELD_MOVE_CUT);
				++k;
			}
		}
		#endif

		if (k < MAX_MON_MOVES && !knowsFly) //Doesn't know 4 field moves
		{
			if (Overworld_MapTypeAllowsTeleportAndFly(gMapHeader.mapType) //Only add if usable
			#ifndef DEBUG_HMS
			&& HasBadgeToUseFieldMove(FIELD_MOVE_FLY)
			&& (
			 #ifdef FLAG_BOUGHT_ADM
			 FlagGet(FLAG_BOUGHT_ADM) ||
			 #endif
			 #ifdef FLAG_SANDBOX_MODE
			 FlagGet(FLAG_SANDBOX_MODE) ||
			 #endif
			 (CheckBagHasItem(ITEM_HM02_FLY, 1) > 0 && CanMonLearnTMTutor(&mons[slotId], ITEM_HM02_FLY, 0) == CAN_LEARN_MOVE))
			#endif
			)
			{
				AppendToList(sPartyMenuInternal->actions, &sPartyMenuInternal->numActions, MENU_FIELD_MOVES + FIELD_MOVE_FLY);
				++k;
			}
		}

		if (k < MAX_MON_MOVES && !knowsDig) //Doesn't know 4 field moves
		{
			if (CanUseEscapeRopeOnCurrMap() //Only add if usable
			#ifndef DEBUG_HMS
			&& HasBadgeToUseFieldMove(FIELD_MOVE_DIG)
			&& (
			 #ifdef FLAG_BOUGHT_ADM
			 FlagGet(FLAG_BOUGHT_ADM) ||
			 #endif
			 #ifdef FLAG_SANDBOX_MODE
			 FlagGet(FLAG_SANDBOX_MODE) ||
			 #endif
			 (CheckBagHasItem(ITEM_TM28_DIG, 1) > 0 && CanMonLearnTMTutor(&mons[slotId], ITEM_TM28_DIG, 0) == CAN_LEARN_MOVE))
			#endif
			)
			{
				AppendToList(sPartyMenuInternal->actions, &sPartyMenuInternal->numActions, MENU_FIELD_MOVES + FIELD_MOVE_DIG);
				++k;
			}
		}
	}
	#endif

#ifdef FLAG_SANDBOX_MODE
SKIP_FIELD_MOVES:
#endif
	if (!ShouldDisablePartyMenuItemsBattleTower())
	{
		if (GetMonData(&mons[1], MON_DATA_SPECIES, NULL) != SPECIES_NONE)
			AppendToList(sPartyMenuInternal->actions, &sPartyMenuInternal->numActions, MENU_SWITCH);
		if (IsMail(GetMonData(&mons[slotId], MON_DATA_HELD_ITEM, NULL)))
			AppendToList(sPartyMenuInternal->actions, &sPartyMenuInternal->numActions, MENU_MAIL);
		else
			AppendToList(sPartyMenuInternal->actions, &sPartyMenuInternal->numActions, MENU_ITEM);
	}

	AppendToList(sPartyMenuInternal->actions, &sPartyMenuInternal->numActions, MENU_CANCEL1);
}

static bool8 SetUpFieldMove_Fly(void)
{
	if (gFollowerState.inProgress && !(gFollowerState.flags & FOLLOWER_FLAG_CAN_LEAVE_ROUTE))
		return FALSE;

	#ifdef UNBOUND
	if (GetCurrentRegionMapSectionId() == MAPSEC_NEWMOON_ISLAND
	&& VarGet(VAR_DARKRAI_EVENT) < 3)
		return FALSE; //Can't fly while dreaming
	#endif

	if (Overworld_MapTypeAllowsTeleportAndFly(gMapHeader.mapType) == TRUE)
	{
		gFieldCallback2 = NULL; //Undoes when its set when party menu is accessed via R
		return TRUE;
	}

	return FALSE;
}

#define FieldCallback_Surf (void*) (0x812497C | 1)
static bool8 SetUpFieldMove_Surf(void)
{
	if (gFollowerState.inProgress && !(gFollowerState.flags & FOLLOWER_FLAG_CAN_SURF))
		return FALSE;

	if (IsCurrentAreaVolcano())
		return FALSE;

	u16 item = ITEM_NONE;
	#ifdef ONLY_CHECK_ITEM_FOR_HM_USAGE
	item = ITEM_HM03_SURF;
	#endif

	if (PartyHasMonWithFieldMovePotential(MOVE_SURF, item, SHOULDNT_BE_SURFING) < PARTY_SIZE
	&& IsPlayerFacingSurfableFishableWater() == TRUE)
	{
		gFieldCallback2 = FieldCallback_PrepareFadeInFromMenu;
		gPostMenuFieldCallback = FieldCallback_Surf;
		return TRUE;
	}

	return FALSE;
}

static bool8 SetUpFieldMove_Waterfall(void)
{
	s16 x, y;

	if (gFollowerState.inProgress && !(gFollowerState.flags & FOLLOWER_FLAG_CAN_WATERFALL))
		return FALSE;

	GetXYCoordsOneStepInFrontOfPlayer(&x, &y);
	if (MetatileBehavior_IsWaterfall(MapGridGetMetatileBehaviorAt(x, y)) == TRUE && IsPlayerSurfingNorthOrSouth() == TRUE)
	{
		gFieldCallback2 = FieldCallback_PrepareFadeInFromMenu;
		gPostMenuFieldCallback = (void*) 0x8124ADD;
		return TRUE;
	}

	return FALSE;
}

#define FieldCallback_Teleport (void*) (0x80F6730 | 1)
static bool8 SetUpFieldMove_Teleport(void)
{
	if (gFollowerState.inProgress && !(gFollowerState.flags & FOLLOWER_FLAG_CAN_LEAVE_ROUTE))
		return FALSE;

	if (Overworld_MapTypeAllowsTeleportAndFly(gMapHeader.mapType) == TRUE)
	{
		gFieldCallback2 = FieldCallback_PrepareFadeInFromMenu;
		gPostMenuFieldCallback = FieldCallback_Teleport;
		return TRUE;
	}

	return FALSE;
}

static void FieldCallback_Dive(void)
{
	gFieldEffectArguments[0] = GetCursorSelectionMonId();
	FieldEffectStart(FLDEFF_USE_DIVE);
}

static bool8 SetUpFieldMove_Dive(void)
{
	if (gFollowerState.inProgress && !(gFollowerState.flags & FOLLOWER_FLAG_CAN_DIVE))
		return FALSE;

	if (!HasBadgeToUseFieldMove(FIELD_MOVE_DIVE))
		return FALSE;

	#if (defined FLAG_BOUGHT_ADM && !defined DEBUG_HMS)
	if (!FlagGet(FLAG_BOUGHT_ADM))
		return FALSE;
	#endif

	gFieldEffectArguments[1] = TrySetDiveWarp();
	if (gFieldEffectArguments[1] != 0)
	{
		gFieldCallback2 = FieldCallback_PrepareFadeInFromMenu;
		gPostMenuFieldCallback = FieldCallback_Dive;
		return TRUE;
	}

	return FALSE;
}

static bool8 SetUpFieldMove_SweetScent(void)
{
	if (gMapHeader.mapType == MAP_TYPE_UNDERWATER)
		return FALSE; //Sweet Scent glitches out sprites underwater

	gFieldCallback2 = FieldCallback_PrepareFadeInFromMenu;
	gPostMenuFieldCallback = FieldCallback_SweetScent;
	return TRUE;
}

static void FieldCallback_RockClimb(void)
{
	gFieldEffectArguments[0] = GetCursorSelectionMonId();
	ScriptContext1_SetupScript(EventScript_RockClimb);
}

static bool8 SetUpFieldMove_RockClimb(void)
{
	if (gFollowerState.inProgress && !(gFollowerState.flags & FOLLOWER_FLAG_CAN_ROCK_CLIMB))
		return FALSE;

	if (IsPlayerFacingRockClimbableWall())
	{
		gFieldCallback2 = FieldCallback_PrepareFadeInFromMenu;
		gPostMenuFieldCallback = FieldCallback_RockClimb;
		return TRUE;
	}

	return FALSE;
}

static void FieldCallback_Defog(void)
{
	u8 monId = GetCursorSelectionMonId();

	gFieldEffectArguments[0] = monId;
	GetMonNickname(&gPlayerParty[monId], gStringVar1);
	ScriptContext1_SetupScript(EventScript_Defog);
}

static bool8 SetUpFieldMove_Defog(void)
{
	u8 weather = GetCurrentWeather();

	if (weather == WEATHER_FOG_1
	||  weather == WEATHER_FOG_2
	||  weather == WEATHER_FOG_3)
	{
		gFieldCallback2 = FieldCallback_PrepareFadeInFromMenu;
		gPostMenuFieldCallback = FieldCallback_Defog;
		return TRUE;
	}

	return FALSE;
}

bool8 HasBadgeToUseFieldMove(unusedArg u8 id)
{
	#ifdef DEBUG_HMS
		return TRUE;
	#else
		return gFieldMoveBadgeRequirements[id] == 0
			|| FlagGet(FLAG_BADGE01_GET + (gFieldMoveBadgeRequirements[id] - 1));
	#endif
}

bool8 HasBadgeToUseSurf(void)
{
	return HasBadgeToUseFieldMove(FIELD_MOVE_SURF);
}

bool8 HasBadgeToUseFlash(void)
{
	return HasBadgeToUseFieldMove(FIELD_MOVE_FLASH);
}

bool8 HasBadgeToUseWaterfall(void)
{
	return HasBadgeToUseFieldMove(FIELD_MOVE_WATERFALL);
}

bool8 HasBadgeToUseRockClimb(void)
{
	return HasBadgeToUseFieldMove(FIELD_MOVE_ROCK_CLIMB);
}

bool8 HasBadgeToUseDive(void)
{
	return HasBadgeToUseFieldMove(FIELD_MOVE_DIVE);
}

//The following specials are meant to help implement "PokeRide" properly
void sp100_CanPlayerUseFlashInCurrentLocation(void)
{
	gSpecialVar_LastResult = gMapHeader.cave == TRUE && !FlagGet(FLAG_SYS_FLASH_ACTIVE);
}

void sp101_CanPlayerFlyInCurrentLocation(void)
{
	gSpecialVar_LastResult = FALSE;
	if (gFollowerState.inProgress && !(gFollowerState.flags & FOLLOWER_FLAG_CAN_LEAVE_ROUTE))
		return;

	gSpecialVar_LastResult = Overworld_MapTypeAllowsTeleportAndFly(gMapHeader.mapType);
}

void sp102_IsPlayerFacingSurfableWater(void)
{
	gSpecialVar_LastResult = FALSE;
	if (gFollowerState.inProgress && !(gFollowerState.flags & FOLLOWER_FLAG_CAN_SURF))
		return;

	gSpecialVar_LastResult = IsPlayerFacingSurfableFishableWater();
}

void sp103_IsPlayerFacingClimbableWaterfall(void)
{
	s16 x, y;
	GetXYCoordsOneStepInFrontOfPlayer(&x, &y);

	gSpecialVar_LastResult = FALSE;
	if (gFollowerState.inProgress && !(gFollowerState.flags & FOLLOWER_FLAG_CAN_WATERFALL))
		return;

	gSpecialVar_LastResult = MetatileBehavior_IsWaterfall(MapGridGetMetatileBehaviorAt(x, y)) && IsPlayerSurfingNorthOrSouth();
}

void sp104_IsPlayerOnDiveableWater(void)
{
	gSpecialVar_LastResult = FALSE;
	if (gFollowerState.inProgress && !(gFollowerState.flags & FOLLOWER_FLAG_CAN_DIVE))
		return;

	gSpecialVar_LastResult = TrySetDiveWarp();
}

void sp105_IsPlayerFacingClimbableWall(void)
{
	gSpecialVar_LastResult = FALSE;
	if (gFollowerState.inProgress && !(gFollowerState.flags & FOLLOWER_FLAG_CAN_ROCK_CLIMB))
		return;

	gSpecialVar_LastResult = IsPlayerFacingRockClimbableWall();
}

void sp109_IsPlayerFacingNPCWithOverworldPic(void)
{
	gSpecialVar_LastResult = CheckObjectGraphicsInFrontOfPlayer(Var8000);
}

void sp10A_CanUseCutOnTree(void)
{
	u16 item = ITEM_NONE;

	#ifdef ONLY_CHECK_ITEM_FOR_HM_USAGE
	item = ITEM_HM01_CUT;
	#endif

	Var8004 = PARTY_SIZE;
	if (HasBadgeToUseFieldMove(FIELD_MOVE_CUT))
	{
		#ifdef FLAG_BOUGHT_ADM
		if (FlagGet(FLAG_BOUGHT_ADM))
			Var8004 = 0; //Mon doesn't matter, just can't be over 6
		else
		#endif
		#ifdef FLAG_SANDBOX_MODE
		if (FlagGet(FLAG_SANDBOX_MODE))
			Var8004 = 0; //Mon doesn't matter, just can't be over 6
		else
		#endif
			Var8004 = PartyHasMonWithFieldMovePotential(MOVE_CUT, item, SHOULDNT_BE_SURFING);
	}
}

void sp10B_CanUseRockSmashOnRock(void)
{
	u16 item = ITEM_NONE;

	#ifdef ONLY_CHECK_ITEM_FOR_HM_USAGE
	item = ITEM_HM06_ROCK_SMASH;
	#endif

	Var8004 = PARTY_SIZE;
	if (HasBadgeToUseFieldMove(FIELD_MOVE_ROCK_SMASH))
	{
		#ifdef FLAG_BOUGHT_ADM
		if (FlagGet(FLAG_BOUGHT_ADM))
			Var8004 = 0; //Mon doesn't matter, just can't be over 6
		else
		#endif
		#ifdef FLAG_SANDBOX_MODE
		if (FlagGet(FLAG_SANDBOX_MODE))
			Var8004 = 0; //Mon doesn't matter, just can't be over 6
		else
		#endif
			Var8004 = PartyHasMonWithFieldMovePotential(MOVE_ROCKSMASH, item, SHOULDNT_BE_SURFING);
	}
}

void sp10C_CanUseStrengthOnBoulder(void)
{
	u16 item = ITEM_NONE;

	#ifdef ONLY_CHECK_ITEM_FOR_HM_USAGE
	item = ITEM_HM04_STRENGTH;
	#endif

	Var8004 = PARTY_SIZE;
	if (HasBadgeToUseFieldMove(FIELD_MOVE_STRENGTH))
	{
		#ifdef FLAG_BOUGHT_ADM
		if (FlagGet(FLAG_BOUGHT_ADM))
			Var8004 = 0; //Mon doesn't matter, just can't be over 6
		else
		#endif
		#ifdef FLAG_SANDBOX_MODE
		if (FlagGet(FLAG_SANDBOX_MODE))
			Var8004 = 0; //Mon doesn't matter, just can't be over 6
		else
		#endif
			Var8004 = PartyHasMonWithFieldMovePotential(MOVE_STRENGTH, item, SHOULDNT_BE_SURFING);
	}
}

//Move Item - Credits to Sagiri/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define gText_PkmnNotHolding (const u8*) 0x08416C8F
#define  gText_PkmnWasGivenItem (const u8*) 0x8416C2A
extern const u8 gText_SwapItemsPart1[];
extern const u8 gText_SwapItemsPart2[];

#define MSG_MOVE 37
#define MSG_FUSE 38

const u8 gPartyMenu_ItemMenuActions[] = {MENU_GIVE, MENU_TAKE_ITEM, MENU_MOVE_ITEM, MENU_CANCEL2};

static void CursorCb_MoveItemCallback(u8 taskId)
{
	u16 item1, item2;
	u8 buffer[100];

	if (gPaletteFade->active || some_other_kind_of_link_test())
		return;

	switch (PartyMenuButtonHandler(&gPartyMenu.slotId2)) {
		case 1: //User hit A on a Pokemon
			//Pokemon can't give away items to eggs or themselves
			if (GetMonData(&gPlayerParty[gPartyMenu.slotId2], MON_DATA_IS_EGG, NULL)
			|| gPartyMenu.slotId == gPartyMenu.slotId2)
			{
				PlaySE(SE_ERROR);
				return;
			}

			PlaySE(SE_SELECT);
			gPartyMenu.action = PARTY_ACTION_CHOOSE_MON;

			//Look up held items
			item1 = GetMonData(&gPlayerParty[gPartyMenu.slotId], MON_DATA_HELD_ITEM, NULL);
			item2 = GetMonData(&gPlayerParty[gPartyMenu.slotId2], MON_DATA_HELD_ITEM, NULL);

			//Swap the held items
			SetMonData(&gPlayerParty[gPartyMenu.slotId], MON_DATA_HELD_ITEM, &item2);
			SetMonData(&gPlayerParty[gPartyMenu.slotId2], MON_DATA_HELD_ITEM, &item1);

			//Update the held item icons
			UpdatePartyMonHeldItemSprite(&gPlayerParty[gPartyMenu.slotId],
						&sPartyMenuBoxes[gPartyMenu.slotId]);

			UpdatePartyMonHeldItemSprite(&gPlayerParty[gPartyMenu.slotId2],
						&sPartyMenuBoxes[gPartyMenu.slotId2]);

			//Create the string describing the move
			if (item2 == ITEM_NONE)
			{
				GetMonNickname(&gPlayerParty[gPartyMenu.slotId2], gStringVar1);
				CopyItemName(item1, gStringVar2);
				StringExpandPlaceholders(gStringVar4, gText_PkmnWasGivenItem);
			}
			else
			{
				GetMonNickname(&gPlayerParty[gPartyMenu.slotId], gStringVar1);
				CopyItemName(item1, gStringVar2);
				StringExpandPlaceholders(buffer, gText_SwapItemsPart1);

				StringAppend(buffer, gText_SwapItemsPart2);
				GetMonNickname(&gPlayerParty[gPartyMenu.slotId2], gStringVar1);
				CopyItemName(item2, gStringVar2);
				StringExpandPlaceholders(gStringVar4, buffer);
			}

			//Display the string
			DisplayPartyMenuMessage(gStringVar4, 1);

			//Update colors of selected boxes
			AnimatePartySlot(gPartyMenu.slotId2, 1);
			AnimatePartySlot(gPartyMenu.slotId, 0);

			//Move cursor to second select slot
			gPartyMenu.slotId = gPartyMenu.slotId2;

			//Return to the main party menu
			ScheduleBgCopyTilemapToVram(2);
			gTasks[taskId].func = Task_UpdatePartyMenuHeldItemSprite;
			break;
		case 2:	 // User hit B or A while on Cancel
			HandleChooseMonCancel(taskId, &gPartyMenu.slotId2);
			break;
	}
}

static void CursorCb_MoveItem(u8 taskId)
{
	struct Pokemon* mon = &gPlayerParty[gPartyMenu.slotId];

	PlaySE(SE_SELECT);

	//Delete old windows
	PartyMenuRemoveWindow(&sPartyMenuInternal->windowId[1]);
	PartyMenuRemoveWindow(&sPartyMenuInternal->windowId[0]);

	if (GetMonData(mon, MON_DATA_HELD_ITEM, NULL) != ITEM_NONE)
	{
		gPartyMenu.action = PARTY_ACTION_SWITCH;

		//Show "Move item to where" in bottom left
		DisplayPartyMenuStdMessage(MSG_MOVE);
		//Update color of first selected box
		AnimatePartySlot(gPartyMenu.slotId, 1);

		//Set up callback
		gPartyMenu.slotId2 = gPartyMenu.slotId;
		gTasks[taskId].func = CursorCb_MoveItemCallback;
	}
	else
	{
		//Create and display string about lack of hold item
		GetMonNickname(mon, gStringVar1);
		StringExpandPlaceholders(gStringVar4, gText_PkmnNotHolding);
		DisplayPartyMenuMessage(gStringVar4, 1);

		//Return to the main party menu
		ScheduleBgCopyTilemapToVram(2);
		gTasks[taskId].func = Task_UpdatePartyMenuHeldItemSprite;
	}
}

//Functions relating to staying on the party screen after an item is used
static bool8 IsUsePartyMenuItemHPEVModifier(struct Pokemon* mon, u16 oldHP, u16 item);
static void AdjustFriendshipForEVReducingBerry(struct Pokemon* mon);
static void ItemUseCB_EVReducingBerry(u8 taskId, TaskFunc func);
static void ItemUseCB_FormChangeItem(u8 taskId, TaskFunc func);
static void FormChangeItem_ShowPartyMenuFromField(u8 taskId);
static void ItemUseCB_DNASplicersStep(u8 taskId, TaskFunc func);
static void Task_TryLearnPostFormeChangeMove(u8 taskId);
static struct Pokemon* GetBaseMonForFusedSpecies(u16 species);
static void ItemUseCB_AbilityCapsule(u8 taskId, TaskFunc func);
static u8 GetAbilityCapsuleNewAbility(struct Pokemon* mon);
static void Task_OfferAbilityChange(u8 taskId);
static void Task_HandleAbilityChangeYesNoInput(u8 taskId);
static void Task_ChangeAbility(u8 taskId);
static void ItemUseCB_MaxPowder(u8 taskId, TaskFunc func);
static void Task_OfferGigantamaxChange(u8 taskId);
static void Task_HandleGigantamaxChangeYesNoInput(u8 taskId);
static void Task_ChangeGigantamax(u8 taskId);

void Task_ClosePartyMenuAfterText(u8 taskId)
{
	if (!IsPartyMenuTextPrinterActive())
	{
		if (gPartyMenu.menuType != PARTY_MENU_TYPE_IN_BATTLE
		&& gPartyMenu.action == PARTY_ACTION_USE_ITEM
		&& CheckBagHasItem(Var800E, 1)
		&& Var800E != ITEM_DNA_SPLICERS
		&& Var800E != ITEM_N_SOLARIZER
		&& Var800E != ITEM_N_LUNARIZER)
		{
			ClearStdWindowAndFrameToTransparent(6, 0);
			ScheduleBgCopyTilemapToVram(2);
			ReturnToUseOnWhichMon(taskId);
		}
		else //No More Item or In Battle
		{
			if (gPartyMenuUseExitCallback == FALSE)
				sPartyMenuInternal->exitCallback = NULL;

			Task_ClosePartyMenu(taskId);
		}	
	}
}

void InitPartyMenuAfterCancellingLearnMove(TaskFunc task)
{
	u8 action = (gPartyMenu.action == PARTY_ACTION_USE_ITEM) ? PARTY_ACTION_USE_ITEM : PARTY_ACTION_CHOOSE_MON; //Eg. after Rare Candy. Prevents returning to the Bag
	InitPartyMenu(PARTY_MENU_TYPE_FIELD, PARTY_LAYOUT_SINGLE, action, TRUE, PARTY_MSG_NONE, task, gPartyMenu.exitCallback);
}

static bool8 IsUsePartyMenuItemHPEVModifier(struct Pokemon* mon, u16 oldHP, u16 item)
{
	return GetMonData(mon, MON_DATA_HP, NULL) != oldHP
		&& GetItemEffectType(item) == ITEM_EFFECT_HP_EV;
}

#define gText_WontHaveEffect (const u8*) 0x84169DC
void ItemUseCB_MedicineStep(u8 taskId, TaskFunc func)
{
	u16 hp = 0;
	struct Pokemon *mon = &gPlayerParty[gPartyMenu.slotId];
	u16 item = Var800E;
	bool8 canHeal;

	if (NotUsingHPEVItemOnShedinja(mon, item))
	{
		canHeal = IsHPRecoveryItem(item);
		if (canHeal == TRUE || GetItemEffectType(item) == ITEM_EFFECT_HP_EV)
		{
			hp = GetMonData(mon, MON_DATA_HP, NULL);
			if (hp == GetMonData(mon, MON_DATA_MAX_HP, NULL))
				canHeal = FALSE;
		}

		if (ExecuteTableBasedItemEffect_(gPartyMenu.slotId, item, 0))
		{
			WONT_HAVE_EFFECT:
			PlaySE(SE_SELECT);
			gPartyMenuUseExitCallback = FALSE;
			DisplayPartyMenuMessage(gText_WontHaveEffect, TRUE);
			ScheduleBgCopyTilemapToVram(2);
			gTasks[taskId].func = func;
			return;
		}
	}
	else
	{
		goto WONT_HAVE_EFFECT;
	}

	gPartyMenuUseExitCallback = TRUE;

	if (!IsItemFlute(item))
	{
		PlaySE(SE_USE_ITEM);
		if (gPartyMenu.action != PARTY_ACTION_REUSABLE_ITEM)
			RemoveBagItem(item, 1);
	}
	else //Flute
	{
		PlaySE(SE_FLUTE);
	}

	SetPartyMonAilmentGfx(mon, &sPartyMenuBoxes[gPartyMenu.slotId]);
	if (gSprites[sPartyMenuBoxes[gPartyMenu.slotId].statusSpriteId].invisible)
		DisplayPartyPokemonLevelCheck(mon, &sPartyMenuBoxes[gPartyMenu.slotId], 1);

	if (canHeal == TRUE)
	{
		if (hp == 0)
			AnimatePartySlot(gPartyMenu.slotId, 1);
		PartyMenuModifyHP(taskId, gPartyMenu.slotId, 1, GetMonData(mon, MON_DATA_HP, NULL) - hp, Task_DisplayHPRestoredMessage);
		ResetHPTaskData(taskId, 0, hp);
	}
	else
	{
		if (IsUsePartyMenuItemHPEVModifier(mon, hp, item))
		{
			if (hp == 0) //Was dead
				AnimatePartySlot(gPartyMenu.slotId, 1);
			UpdateMonDisplayInfoAfterRareCandy(gPartyMenu.slotId, mon); //So Max HP Updates
		}

		GetMonNickname(mon, gStringVar1);
		GetMedicineItemEffectMessage(item);
		DisplayPartyMenuMessage(gStringVar4, TRUE);
		ScheduleBgCopyTilemapToVram(2);
		gTasks[taskId].func = func;
	}
}

void Task_DoLearnedMoveFanfareAfterText(u8 taskId)
{
	if (!IsPartyMenuTextPrinterActive())
	{
		if (gPartyMenu.action == PARTY_ACTION_CHOOSE_MON)
			gPartyMenu.action = PARTY_ACTION_USE_ITEM;

		PlayFanfare(MUS_FANFA1);
		gTasks[taskId].func = (void*) (0x8125D2C | 1); //Task_LearnNextMoveOrClosePartyMenu
	}
}

#define sLevelUpWindowStatNames ((const u8**) 0x8459B48)
//Fix vanilla function
void DrawLevelUpWindowPg2(u16 windowId, u16 *currStats, u8 bgColor, u8 fgColor, u8 shadowColor)
{
	s16 statsRearrange[NUM_STATS];
	u8 textbuf[12];
	struct TextColor textColor = {bgColor, fgColor, shadowColor};
	u16 i;
	u8 ndigits = 0;
	u16 x;

	FillWindowPixelBuffer(windowId, PIXEL_FILL(bgColor));

	statsRearrange[0] = currStats[0];
	statsRearrange[1] = currStats[1];
	statsRearrange[2] = currStats[2];
	statsRearrange[3] = currStats[4];
	statsRearrange[4] = currStats[5];
	statsRearrange[5] = currStats[3];

	//Align the numbers based on the largest numbers of digits
	for (i = 0; i < NUM_STATS; ++i)
	{
		if (statsRearrange[i] >= 1000)
		{
			ndigits = 4;
			break; //No bigger number
		}
		else if (statsRearrange[i] >= 100)
		{
			if (ndigits < 3)
				ndigits = 3;
		}
		else if (statsRearrange[i] >= 10)
		{
			if (ndigits < 2)
				ndigits = 2;
		}
		else
		{
			if (ndigits < 1)
				ndigits = 1;
		}
	}

	for (i = 0; i < NUM_STATS; ++i)
	{
		ConvertIntToDecimalStringN(textbuf, statsRearrange[i], STR_CONV_MODE_RIGHT_ALIGN, ndigits);
		x = 6 * (4 - ndigits);
		WindowPrint(windowId, 2, 0, i * 15, &textColor, TEXT_SPEED_FF, sLevelUpWindowStatNames[i]);
		WindowPrint(windowId, 2, 56 + x, i * 15, &textColor, TEXT_SPEED_FF, textbuf);
	}
}

void ItemUseCB_EvolutionStone(u8 taskId, TaskFunc func)
{
	PlaySE(SE_SELECT);
	u16 targetSpecies = GetEvolutionTargetSpecies(&gPlayerParty[gPartyMenu.slotId], EVO_MODE_ITEM_USE, gSpecialVar_ItemId);

	if (targetSpecies == SPECIES_NONE)
	{
		gPartyMenuUseExitCallback = FALSE;
		DisplayPartyMenuMessage(gText_WontHaveEffect, TRUE);
		ScheduleBgCopyTilemapToVram(2);
		gTasks[taskId].func = func;
	}
	else
	{
		FreePartyPointers();
		gCB2_AfterEvolution = gPartyMenu.exitCallback;
		BeginEvolutionScene(&gPlayerParty[gPartyMenu.slotId], targetSpecies, FALSE, gPartyMenu.slotId);
		DestroyTask(taskId);
		RemoveBagItem(gSpecialVar_ItemId, 1);
	}
}

void FieldUseFunc_EVReducingBerry(u8 taskId)
{
	gItemUseCB = ItemUseCB_EVReducingBerry;
	SetUpItemUseCallback(taskId);
}

static void AdjustFriendshipForEVReducingBerry(struct Pokemon* mon)
{
	u8 boostAmount;
	u8 holdEffect = ItemId_GetHoldEffect(GetMonData(mon, MON_DATA_HELD_ITEM, NULL));
	u16 friendship = GetMonData(mon, MON_DATA_FRIENDSHIP, NULL);

	if (friendship < 100)
	{
		boostAmount = 10;
	}
	else if (friendship >= 100 && friendship < 200)
	{
		boostAmount = 5;
	}
	else //friendship >= 200
	{
		boostAmount = 2;
	}

	if (holdEffect == ITEM_EFFECT_HAPPINESS_UP)
		friendship += (boostAmount * 15) / 10; //1.5x Boost
	else
		friendship += boostAmount;

	if (GetMonData(mon, MON_DATA_POKEBALL, NULL) == ITEM_LUXURY_BALL)
		friendship++;
	if (GetMonData(mon, MON_DATA_MET_LOCATION, NULL) == GetCurrentRegionMapSectionId()) //This is official...who knew?
		friendship++;

	if (friendship > MAX_FRIENDSHIP)
		friendship = MAX_FRIENDSHIP;

	SetMonData(mon, MON_DATA_FRIENDSHIP, &friendship);
}

#define gStatNamesTable ((const u8**) 0x83FD5D0)
extern const u8 gText_EVReducingBerryBothEffects[];
extern const u8 gText_EVReducingBerryLoweredStat[];
extern const u8 gText_EVReducingBerryIncreasedFriendship[];
static void ItemUseCB_EVReducingBerry(u8 taskId, TaskFunc func)
{
	struct Pokemon* mon = &gPlayerParty[gPartyMenu.slotId];
	u16 item = Var800E;
	u8 stat = ItemId_GetHoldEffectParam(item);
	u8 ev = GetMonData(mon, MON_DATA_HP_EV + stat, NULL);
	u8 friendship = GetMonData(mon, MON_DATA_FRIENDSHIP, NULL);

	PlaySE(SE_SELECT);
	if (friendship < MAX_FRIENDSHIP || ev > 0) //Stat can fall or friendship can increase
	{
		u16 oldHP = GetMonData(mon, MON_DATA_HP, NULL);
		u16 oldMaxHP = GetMonData(mon, MON_DATA_MAX_HP, NULL);
		GetMonNickname(mon, gStringVar1);
		StringCopy(gStringVar2, gStatNamesTable[stat]);

		if (friendship < MAX_FRIENDSHIP && ev > 0)
		{
			if (ev > 10)
				ev -= 10;
			else
				ev = 0;

			SetMonData(mon, MON_DATA_HP_EV + stat, &ev);
			AdjustFriendshipForEVReducingBerry(mon);
			StringExpandPlaceholders(gStringVar4, gText_EVReducingBerryBothEffects);
			CalculateMonStats(mon);
		}
		else if (friendship == MAX_FRIENDSHIP && ev > 0)
		{
			if (ev > 10)
				ev -= 10;
			else
				ev = 0;

			SetMonData(mon, MON_DATA_HP_EV + stat, &ev);
			StringExpandPlaceholders(gStringVar4, gText_EVReducingBerryLoweredStat);
			CalculateMonStats(mon);
		}
		else
		{
			AdjustFriendshipForEVReducingBerry(mon);
			StringExpandPlaceholders(gStringVar4, gText_EVReducingBerryIncreasedFriendship);
		}
		
		if (oldMaxHP != GetMonData(mon, MON_DATA_MAX_HP, NULL) //Pomeg Berry
		#ifdef SPECIES_SHEDINJA
		&& GetMonData(mon, MON_DATA_SPECIES, NULL) != SPECIES_SHEDINJA
		#endif
		)
		{
			u16 hpDiff = oldMaxHP - GetMonData(mon, MON_DATA_MAX_HP, NULL);
			if (oldHP == 0) //Mon was fainted before
				SetMonData(mon, MON_DATA_HP, &oldHP); //Keep it fainted
			else if (GetMonData(mon, MON_DATA_HP, NULL) == oldHP) //HP didn't change for some reason
			{
				if (hpDiff > oldHP)
					oldHP = 1; //Don't faint it
				else
					oldHP = MathMax(1, oldHP - hpDiff);

				SetMonData(mon, MON_DATA_HP, &oldHP);
			}

			UpdateMonDisplayInfoAfterRareCandy(gPartyMenu.slotId, mon); //So Max HP Updates
		}

		RemoveBagItem(item, 1);
		DisplayPartyMenuMessage(gStringVar4, TRUE);
		ScheduleBgCopyTilemapToVram(2);
		gTasks[taskId].func = func;
	}
	else //No Effect
	{
		gPartyMenuUseExitCallback = FALSE;
		DisplayPartyMenuMessage(gText_WontHaveEffect, TRUE);
		ScheduleBgCopyTilemapToVram(2);
		gTasks[taskId].func = func;
	}
}

void FieldUseFunc_FormChangeItem(u8 taskId)
{
	gItemUseCB = ItemUseCB_FormChangeItem;

	if (gTasks[taskId].data[3] == 0) //From Bag
	{
		SetUpItemUseCallback(taskId);
	}
	else //From Overworld
	{
		FadeScreen(FADE_TO_BLACK, 0);
		gTasks[taskId].func = FormChangeItem_ShowPartyMenuFromField;
	}
}

static void FormChangeItem_ShowPartyMenuFromField(u8 taskId)
{
	if (!gPaletteFade->active)
	{
		CleanupOverworldWindowsAndTilemaps();
		PrepareOverworldReturn();
		InitPartyMenu(PARTY_MENU_TYPE_FIELD, PARTY_LAYOUT_SINGLE, PARTY_ACTION_USE_ITEM, TRUE, PARTY_MSG_USE_ON_WHICH_MON, Task_HandleChooseMonInput, CB2_ReturnToFieldContinueScript);
		DestroyTask(taskId);
	}
}

void UpdateMonIconSpecies(u8 iconSpriteId, u16 species)
{
	gSprites[iconSpriteId].images = (void*) GetMonIconTiles(species, FALSE);
	gSprites[iconSpriteId].oam.paletteNum = IndexOfSpritePaletteTag(POKE_ICON_BASE_PAL_TAG + GetMonIconPaletteIndexFromSpecies(species));
}

extern const u8 gText_FormChangeItem[];
extern const u8 gText_AlreadyHaveFused[];
extern const u8 gText_NoSpaceForDefuse[];
extern const u8 gText_CantFuseWithPokemon[];
void DoItemFormChange(struct Pokemon* mon, u16 species)
{
	SetMonData(mon, MON_DATA_SPECIES, &species);
	CalculateMonStats(mon);
	UpdateMonIconSpecies(sPartyMenuBoxes[gPartyMenu.slotId].monSpriteId, species);
	PlayCry1(species, 0);
	GetMonNickname(mon, gStringVar1);
	StringExpandPlaceholders(gStringVar4, gText_FormChangeItem);
	DisplayPartyMenuMessage(gStringVar4, TRUE);
	ScheduleBgCopyTilemapToVram(2);	
}

static bool8 FormFuseItemMatchesSpecies(u16 item, u16 species)
{
	switch (species)
	{
		case SPECIES_RESHIRAM:
		case SPECIES_ZEKROM:
		case SPECIES_KYUREM:
		case SPECIES_KYUREM_BLACK:
		case SPECIES_KYUREM_WHITE:
			return item == ITEM_DNA_SPLICERS;
		case SPECIES_NECROZMA:
			return item == ITEM_N_SOLARIZER || item == ITEM_N_LUNARIZER;
		case SPECIES_SOLGALEO:
		case SPECIES_NECROZMA_DUSK_MANE:
			return item == ITEM_N_SOLARIZER;
		case SPECIES_LUNALA:
		case SPECIES_NECROZMA_DAWN_WINGS:
			return item == ITEM_N_LUNARIZER;
	}

	return FALSE;
}

static void ItemUseCB_FormChangeItem(u8 taskId, TaskFunc func)
{
	struct Pokemon* mon = &gPlayerParty[gPartyMenu.slotId];
	u16 item = Var800E;
	u16 species = GetMonData(mon, MON_DATA_SPECIES2, NULL);

	PlaySE(SE_SELECT);
	switch (item) {
		case ITEM_GRACIDEA:
			#if (defined SPECIES_SHAYMIN && defined SPECIES_SHAYMIN_SKY)
			if (species == SPECIES_SHAYMIN && !IsNightTime()) //Shaymin can't change form at night
			{
				species = SPECIES_SHAYMIN_SKY;
				DoItemFormChange(mon, species);
				gTasks[taskId].func = func;
			}
			else
			#endif
				goto NO_EFFECT;
			break;
		case ITEM_REVEAL_GLASS:
			switch (species) {
				#if (defined SPECIES_TORNADUS && defined SPECIES_TORNADUS_THERIAN)
				case SPECIES_TORNADUS:
					species = SPECIES_TORNADUS_THERIAN;
					break;
				case SPECIES_TORNADUS_THERIAN:
					species = SPECIES_TORNADUS;
					break;
				#endif
				#if (defined SPECIES_THUNDURUS && defined SPECIES_THUNDURUS_THERIAN)
				case SPECIES_THUNDURUS:
					species = SPECIES_THUNDURUS_THERIAN;
					break;
				case SPECIES_THUNDURUS_THERIAN:
					species = SPECIES_THUNDURUS;
					break;
				#endif
				#if (defined SPECIES_LANDORUS && defined SPECIES_LANDORUS_THERIAN)
				case SPECIES_LANDORUS:
					species = SPECIES_LANDORUS_THERIAN;
					break;
				case SPECIES_LANDORUS_THERIAN:
					species = SPECIES_LANDORUS;
					break;	
				#endif
				#if (defined SPECIES_ENAMORUS && defined SPECIES_ENAMORUS_THERIAN)
				case SPECIES_ENAMORUS:
					species = SPECIES_ENAMORUS_THERIAN;
					break;
				case SPECIES_ENAMORUS_THERIAN:
					species = SPECIES_ENAMORUS;
					break;	
				#endif
				default:
					goto NO_EFFECT;
			}

			DoItemFormChange(mon, species);
			gTasks[taskId].func = func;
			break;
		case ITEM_PRISON_BOTTLE:
			#if (defined SPECIES_HOOPA && defined SPECIES_HOOPA_UNBOUND)
			if (species == SPECIES_HOOPA)
			{
				species = SPECIES_HOOPA_UNBOUND;
				DoItemFormChange(mon, species);
				gTasks[taskId].func = func;
			}
			else if (species == SPECIES_HOOPA_UNBOUND)
			{
				species = SPECIES_HOOPA;
				DoItemFormChange(mon, species);
				gTasks[taskId].func = func;
			}
			else
			#endif
				goto NO_EFFECT;
			break;
		case ITEM_RED_NECTAR:
			#if (defined SPECIES_ORICORIO && defined SPECIES_ORICORIO_Y && defined SPECIES_ORICORIO_P && defined SPECIES_ORICORIO_S)
			if (species == SPECIES_ORICORIO_Y || species == SPECIES_ORICORIO_P || species == SPECIES_ORICORIO_S)
			{
				species = SPECIES_ORICORIO;
				DoItemFormChange(mon, species);
				RemoveBagItem(Var800E, 1);
				gTasks[taskId].func = func;
			}
			else
			#endif
				goto NO_EFFECT;
			break;
		case ITEM_YELLOW_NECTAR:
			#if (defined SPECIES_ORICORIO && defined SPECIES_ORICORIO_Y && defined SPECIES_ORICORIO_P && defined SPECIES_ORICORIO_S)
			if (species == SPECIES_ORICORIO || species == SPECIES_ORICORIO_P || species == SPECIES_ORICORIO_S)
			{
				species = SPECIES_ORICORIO_Y;
				DoItemFormChange(mon, species);
				RemoveBagItem(Var800E, 1);
				gTasks[taskId].func = func;
			}
			else
			#endif
				goto NO_EFFECT;
			break;
		case ITEM_PINK_NECTAR:
			#if (defined SPECIES_ORICORIO && defined SPECIES_ORICORIO_Y && defined SPECIES_ORICORIO_P && defined SPECIES_ORICORIO_S)
			if (species == SPECIES_ORICORIO || species == SPECIES_ORICORIO_Y || species == SPECIES_ORICORIO_S)
			{
				species = SPECIES_ORICORIO_P;
				DoItemFormChange(mon, species);
				RemoveBagItem(Var800E, 1);
				gTasks[taskId].func = func;
			}
			else
			#endif
				goto NO_EFFECT;
			break;
		case ITEM_PURPLE_NECTAR:
			#if (defined SPECIES_ORICORIO && defined SPECIES_ORICORIO_Y && defined SPECIES_ORICORIO_P && defined SPECIES_ORICORIO_S)
			if (species == SPECIES_ORICORIO || species == SPECIES_ORICORIO_Y || species == SPECIES_ORICORIO_P)
			{
				species = SPECIES_ORICORIO_S;
				DoItemFormChange(mon, species);
				RemoveBagItem(Var800E, 1);
				gTasks[taskId].func = func;
			}
			else
			#endif
				goto NO_EFFECT;
			break;
		case ITEM_DNA_SPLICERS:
		case ITEM_N_SOLARIZER:
		case ITEM_N_LUNARIZER:
			if (!FormFuseItemMatchesSpecies(item, species))
				goto NO_EFFECT;

			#if (defined SPECIES_KYUREM && defined SPECIES_NECROZMA)
			if (species == SPECIES_KYUREM || species == SPECIES_NECROZMA)
			{
				DisplayPartyMenuStdMessage(MSG_FUSE); //Show "Fuse with which Pokemon?" in bottom left
				AnimatePartySlot(gPartyMenu.slotId, 1); //Update color of first selected box

				//Set up callback
				gPartyMenu.slotId2 = gPartyMenu.slotId;
				gItemUseCB = ItemUseCB_DNASplicersStep;
			}
			else
			#endif
			#if (defined SPECIES_KYUREM_BLACK && defined SPECIES_KYUREM_WHITE && defined SPECIES_NECROZMA_DUSK_MANE && defined SPECIES_NECROZMA_DAWN_WINGS)
				if (species == SPECIES_KYUREM_BLACK || species == SPECIES_KYUREM_WHITE || species == SPECIES_NECROZMA_DUSK_MANE || species == SPECIES_NECROZMA_DAWN_WINGS)
			{
				u8 slotId;
				for (slotId = 0; GetMonData(&gPlayerParty[slotId], MON_DATA_SPECIES, NULL) != SPECIES_NONE && slotId < PARTY_SIZE; ++slotId);

				if (GetMonData(GetBaseMonForFusedSpecies(species), MON_DATA_SPECIES, NULL) == SPECIES_NONE) //No fused mon
				{
					goto NO_EFFECT;
				}
				else if (slotId == PARTY_SIZE) //No space for fused mon
				{
					GetMonNickname(mon, gStringVar1);
					StringExpandPlaceholders(gStringVar4, gText_NoSpaceForDefuse);
					DisplayPartyMenuMessage(gStringVar4, TRUE);
					ScheduleBgCopyTilemapToVram(2);
					gTasks[taskId].func = func;
				}
				else
				{
					struct Pokemon* baseMon = GetBaseMonForFusedSpecies(species);
					gPlayerParty[slotId] = *baseMon;
					Memset(baseMon, 0, sizeof(*baseMon));
					CreatePartyMonSprites(slotId);
					RenderPartyMenuBox(slotId);

					if (species == SPECIES_KYUREM_BLACK || species == SPECIES_KYUREM_WHITE)
						species = SPECIES_KYUREM;
					else
						species = SPECIES_NECROZMA;
					DoItemFormChange(mon, species);
					gTasks[taskId].func = Task_TryLearnPostFormeChangeMove;
				}
			}
			else
			#endif
				goto NO_EFFECT;
			break;
		default:
		NO_EFFECT:
			gPartyMenuUseExitCallback = FALSE;
			DisplayPartyMenuMessage(gText_WontHaveEffect, TRUE);
			ScheduleBgCopyTilemapToVram(2);
			gTasks[taskId].func = func;
	}
}

static struct Pokemon* GetBaseMonForFusedSpecies(u16 species)
{
	switch (species) {
		case SPECIES_KYUREM_BLACK:
			return &gSaveBlock1->fusedZekrom;
		case SPECIES_KYUREM_WHITE:
			return &gSaveBlock1->fusedReshiram;
		case SPECIES_NECROZMA_DUSK_MANE:
			return &gSaveBlock1->fusedSolgaleo;
		case SPECIES_NECROZMA_DAWN_WINGS:
			return &gSaveBlock1->fusedLunala;
	}
	
	return NULL;
}

static bool8 AlreadyFused(u16 baseSpecies, u16 fuseSpecies)
{
	bool8 alreadyFused = FALSE;
	
	switch (baseSpecies) {
		case SPECIES_KYUREM:
			switch (fuseSpecies) {
				case SPECIES_RESHIRAM:
					if (GetMonData(&gSaveBlock1->fusedReshiram, MON_DATA_SPECIES, NULL) != SPECIES_NONE)
						alreadyFused = TRUE;
					break;
				case SPECIES_ZEKROM:
					if (GetMonData(&gSaveBlock1->fusedZekrom, MON_DATA_SPECIES, NULL) != SPECIES_NONE)
						alreadyFused = TRUE;
					break;
			}
			break;
		case SPECIES_NECROZMA:
			switch (fuseSpecies) {
				case SPECIES_SOLGALEO:
					if (GetMonData(&gSaveBlock1->fusedSolgaleo, MON_DATA_SPECIES, NULL) != SPECIES_NONE)
						alreadyFused = TRUE;
					break;
				case SPECIES_ZEKROM:
					if (GetMonData(&gSaveBlock1->fusedLunala, MON_DATA_SPECIES, NULL) != SPECIES_NONE)
						alreadyFused = TRUE;
					break;
			}
			break;
	}
	
	if (alreadyFused)	
	{
		gPartyMenuUseExitCallback = FALSE;
		GetSpeciesName(gStringVar1, baseSpecies);
		GetSpeciesName(gStringVar2, fuseSpecies);
		StringExpandPlaceholders(gStringVar4, gText_AlreadyHaveFused);
		DisplayPartyMenuMessage(gStringVar4, TRUE);
		ScheduleBgCopyTilemapToVram(2);
	}
	
	return alreadyFused;
}

static bool8 DoBaseAndFuseSpeciesMatch(u16 baseSpecies, u16 fuseSpecies)
{
	switch (baseSpecies) {
		case SPECIES_KYUREM:
			switch (fuseSpecies) {
				case SPECIES_RESHIRAM:
				case SPECIES_ZEKROM:
					return TRUE;
			}
			break;
		case SPECIES_NECROZMA:
			switch (fuseSpecies) {
				case SPECIES_SOLGALEO:
				case SPECIES_LUNALA:
					return TRUE;
			}
			break;
	}
	
	return FALSE;
}

static void ItemUseCB_DNASplicersStep(u8 taskId, TaskFunc func)
{
	u16 baseSpecies, fuseSpecies;

	if (gPaletteFade->active || some_other_kind_of_link_test())
		return;

	switch (PartyMenuButtonHandler(&gPartyMenu.slotId2)) {
		case 1: //User hit A on a Pokemon
			baseSpecies = GetMonData(&gPlayerParty[gPartyMenu.slotId2], MON_DATA_SPECIES2, NULL);
			fuseSpecies = GetMonData(&gPlayerParty[gPartyMenu.slotId], MON_DATA_SPECIES2, NULL);
			PlaySE(SE_SELECT);

			//Update colors of selected windows
			AnimatePartySlot(gPartyMenu.slotId2, 1); //Kyurem
			AnimatePartySlot(gPartyMenu.slotId, 0); //Reshiram or Zekrom

			if (AlreadyFused(baseSpecies, fuseSpecies))
			{
				gTasks[taskId].func = func;
				return;
			}

			if (DoBaseAndFuseSpeciesMatch(baseSpecies, fuseSpecies)
			&& FormFuseItemMatchesSpecies(Var800E, fuseSpecies))
			{
				//Swap Slot Ids
				u8 temp = gPartyMenu.slotId;
				gPartyMenu.slotId = gPartyMenu.slotId2;
				gPartyMenu.slotId2 = temp; //Icon update acts on slotId 1

				//Get Proper Form
				struct Pokemon* mon = &gPlayerParty[gPartyMenu.slotId];
				switch (fuseSpecies) {
					case SPECIES_RESHIRAM:
						baseSpecies = SPECIES_KYUREM_WHITE;
						break;
					case SPECIES_ZEKROM:
						baseSpecies = SPECIES_KYUREM_BLACK;
						break;
					case SPECIES_SOLGALEO:
						baseSpecies = SPECIES_NECROZMA_DUSK_MANE;
						break;
					case SPECIES_LUNALA:
						baseSpecies = SPECIES_NECROZMA_DAWN_WINGS;
						break;
				}
				DoItemFormChange(mon, baseSpecies);

				//Backup and Delete Fused Mon
				*GetBaseMonForFusedSpecies(baseSpecies) = gPlayerParty[gPartyMenu.slotId2]; //Save base mon
				ZeroMonData(&gPlayerParty[gPartyMenu.slotId2]);
				FillWindowPixelBuffer(sPartyMenuBoxes[gPartyMenu.slotId2].windowId, PIXEL_FILL(0));
				CopyWindowToVram(sPartyMenuBoxes[gPartyMenu.slotId2].windowId, 3);
				PutWindowTilemap(sPartyMenuBoxes[gPartyMenu.slotId2].windowId);
				RenderPartyMenuBox(gPartyMenu.slotId2);
				DestroySprite(&gSprites[sPartyMenuBoxes[gPartyMenu.slotId2].pokeballSpriteId]);
				DestroySprite(&gSprites[sPartyMenuBoxes[gPartyMenu.slotId2].itemSpriteId]);
				DestroySprite(&gSprites[sPartyMenuBoxes[gPartyMenu.slotId2].monSpriteId]);
				DestroySprite(&gSprites[sPartyMenuBoxes[gPartyMenu.slotId2].statusSpriteId]);
				CompactPartySlots(); //Must be down here so Pokemon don't shift before the empty space is loaded
				
				if (gPartyMenu.slotId2 < gPartyMenu.slotId)
					--gPartyMenu.slotId; //Adjust since second mon is gone
				
				gPartyMenuUseExitCallback = FALSE;
				gTasks[taskId].func = Task_TryLearnPostFormeChangeMove;
			}
			else
			{
				//Return to the bag menu
				gPartyMenuUseExitCallback = FALSE;
				DisplayPartyMenuMessage(gText_CantFuseWithPokemon, TRUE);
				ScheduleBgCopyTilemapToVram(2);
				gTasks[taskId].func = func;
			}
			break;
		case 2:	 // User hit B or A while on Cancel
			gTasks[taskId].func = func;
			break;
	}
}

extern u16 gMoveToLearn;
static void Task_TryLearnPostFormeChangeMove(u8 taskId)
{
	if (!IsPartyMenuTextPrinterActive())
	{
		struct Pokemon* mon = &gPlayerParty[gPartyMenu.slotId];
		u16 species = GetMonData(&gPlayerParty[gPartyMenu.slotId], MON_DATA_SPECIES, NULL);
		u16 replaceMove = MOVE_NONE;
		u16 replaceMove2 = MOVE_NONE;

		gMoveToLearn = MOVE_NONE;
		switch (species) {
			case SPECIES_KYUREM:
				gMoveToLearn = MOVE_GLACIATE;
				replaceMove = MOVE_FUSIONBOLT; //Either Fusion Bolt or Fusion Flare
				replaceMove2 = MOVE_FUSIONFLARE;
				break;
			case SPECIES_KYUREM_WHITE:
				gMoveToLearn = MOVE_FUSIONFLARE;
				replaceMove = MOVE_GLACIATE;
				break;
			case SPECIES_KYUREM_BLACK:
				gMoveToLearn = MOVE_FUSIONBOLT;
				replaceMove = MOVE_GLACIATE;
				break;
			case SPECIES_NECROZMA:
				replaceMove = MOVE_SUNSTEELSTRIKE;
				replaceMove2 = MOVE_MOONGEISTBEAM;
				break;
			case SPECIES_NECROZMA_DUSK_MANE:
				gMoveToLearn = MOVE_SUNSTEELSTRIKE;
				break;
			case SPECIES_NECROZMA_DAWN_WINGS:
				gMoveToLearn = MOVE_MOONGEISTBEAM;
				break;
		}

		if (gMoveToLearn != MOVE_NONE)
		{
			if (replaceMove != MOVE_NONE)
			{
				u8 movePos = FindMovePositionInMonMoveset(replaceMove, mon);
				if (movePos < MAX_MON_MOVES)
				{
					SetMonMoveSlot(mon, gMoveToLearn, movePos);
					PartyMenu_DisplayMonLearnedMove(taskId, gMoveToLearn);
					return;
				}
			}

			if (replaceMove2 != MOVE_NONE)
			{
				u8 movePos = FindMovePositionInMonMoveset(replaceMove2, mon);
				if (movePos < MAX_MON_MOVES)
				{
					SetMonMoveSlot(mon, gMoveToLearn, movePos);
					PartyMenu_DisplayMonLearnedMove(taskId, gMoveToLearn);
					return;
				}
			}

			u8 movePos = FindMovePositionInMonMoveset(MOVE_NONE, mon);
			if (movePos < MAX_MON_MOVES)
			{
				SetMonMoveSlot(mon, gMoveToLearn, movePos);
				PartyMenu_DisplayMonLearnedMove(taskId, gMoveToLearn);
				return;
			}

			PartyMenu_DisplayMonNeedsToReplaceMove(taskId);
		}
		else
		{
			if (replaceMove != MOVE_NONE)
			{
				u8 movePos = FindMovePositionInMonMoveset(replaceMove, mon);
				if (movePos < MAX_MON_MOVES)
				{
					SetMonMoveSlot(mon, MOVE_NONE, movePos);
					RemoveMonPPBonus(mon, movePos);
					for (u8 i = movePos; i < MAX_MON_MOVES - 1; ++i)
						ShiftMoveSlot(mon, i, i + 1);
				}
			}

			if (replaceMove2 != MOVE_NONE)
			{
				u8 movePos = FindMovePositionInMonMoveset(replaceMove2, mon);
				if (movePos < MAX_MON_MOVES)
				{
					SetMonMoveSlot(mon, MOVE_NONE, movePos);
					RemoveMonPPBonus(mon, movePos);
					for (u8 i = movePos; i < MAX_MON_MOVES - 1; ++i)
						ShiftMoveSlot(mon, i, i + 1);
				}
			}

			Task_ClosePartyMenuAfterText(taskId);
		}
	}
}

void FieldUseFunc_AbilityCapsule(u8 taskId)
{
	gItemUseCB = ItemUseCB_AbilityCapsule;
	SetUpItemUseCallback(taskId);
}

extern const u8 gText_AbilityCapsuleOfferChange[];
extern const u8 gText_AbilityCapsuleChangedAbility[];
static void ItemUseCB_AbilityCapsule(u8 taskId, TaskFunc func)
{
	struct Pokemon* mon = &gPlayerParty[gPartyMenu.slotId];
	u8 changeTo = GetAbilityCapsuleNewAbility(mon); //Pick Ability to change to

	PlaySE(SE_SELECT);
	if (changeTo != ABILITY_NONE) //Ability can be changed
	{
		GetMonNickname(mon, gStringVar1);
		CopyAbilityName(gStringVar2, changeTo, mon->species);
		StringExpandPlaceholders(gStringVar4, gText_AbilityCapsuleOfferChange);
		DisplayPartyMenuMessage(gStringVar4, TRUE);
		ScheduleBgCopyTilemapToVram(2);
		gTasks[taskId].func = Task_OfferAbilityChange;
	}
	else //No Effect
	{
		gPartyMenuUseExitCallback = FALSE;
		DisplayPartyMenuMessage(gText_WontHaveEffect, TRUE);
		ScheduleBgCopyTilemapToVram(2);
		gTasks[taskId].func = func;
	}
}

static u8 GetAbilityCapsuleNewAbility(struct Pokemon* mon)
{
	u16 item = Var800E;
	u8 abilityType = ItemId_GetHoldEffectParam(item);
	u16 species = GetMonData(mon, MON_DATA_SPECIES, NULL);
	u8 ability = GetMonAbility(mon);
	u8 ability1 = GetAbility1(species);
	u8 ability2 = GetAbility2(species);
	u8 hiddenAbility = GetHiddenAbility(species);
	u8 changeTo = ABILITY_NONE;

	if (abilityType != 0) //Hidden Ability Capsule
	{
		if (ability != hiddenAbility
		&& hiddenAbility != ABILITY_NONE
		#ifdef UNBOUND
		&& (FlagGet(FLAG_ABILITY_RANDOMIZER)
		 || SpeciesToNationalPokedexNum(species) != NATIONAL_DEX_ZYGARDE) //Must be given with Power Construct
		&& (FlagGet(FLAG_SYS_GAME_CLEAR) //Can be given once the game is cleared
		 || VarGet(VAR_GAME_DIFFICULTY) < OPTIONS_EXPERT_DIFFICULTY //Or if the player is not on a crazy difficulty
		 || gBaseStats[species].hiddenAbility != ABILITY_IMPOSTER) //Don't allow Imposter until the post-game
		#endif
		)
			changeTo = hiddenAbility; //Set the Hidden Ability
	}
	else //Regular ability capsule
	{
		if (ability == ability1)
		{
			if (ability != ability2 && ability2 != ABILITY_NONE)
				changeTo = ability2;
		}
		else if (ability == ability2) //Explicit check just in case the Pokemon has its Hidden Ability
		{
			if (ability1 != ABILITY_NONE)
				changeTo = ability1;
		}
	}

	return changeTo;
}

static void Task_OfferAbilityChange(u8 taskId)
{
	if (IsPartyMenuTextPrinterActive() != TRUE)
	{
		PartyMenuDisplayYesNoMenu();
		gTasks[taskId].func = Task_HandleAbilityChangeYesNoInput;
	}
}

static void Task_HandleAbilityChangeYesNoInput(u8 taskId)
{
	switch (Menu_ProcessInputNoWrapClearOnChoose())
	{
		case 0:
			gTasks[taskId].func = Task_ChangeAbility;
			break;
		case MENU_B_PRESSED:
			PlaySE(SE_SELECT);
			// Fallthrough
		case 1:
			gTasks[taskId].func = Task_ClosePartyMenuAfterText;
			break;
	}
}

static void Task_ChangeAbility(u8 taskId)
{
	u16 item = Var800E;
	u8 abilityType = ItemId_GetHoldEffectParam(item);
	struct Pokemon* mon = &gPlayerParty[gPartyMenu.slotId];
	u16 species = GetMonData(mon, MON_DATA_SPECIES, NULL);
	PlaySE(SE_USE_ITEM);
	
	if (abilityType != 0) //Hidden Ability capsule
	{
		mon->hiddenAbility = TRUE;
	}
	else //Regular Ability capsule
	{
		u32 personality = GetMonData(mon, MON_DATA_PERSONALITY, NULL);
		u8 abilityNum = (personality & 1) ^ 1; //Flip ability bit

		u32 otId = GetMonData(mon, MON_DATA_OT_ID, NULL);
		u16 sid = HIHALF(otId);
		u16 tid = LOHALF(otId);

		u8 gender = GetGenderFromSpeciesAndPersonality(species, personality);
		bool8 isShiny = IsMonShiny(mon);
		u8 letter = GetUnownLetterFromPersonality(personality);
		u8 nature = GetNatureFromPersonality(personality);
		bool8 isMinior = IsMinior(species);
		u16 miniorCore = GetMiniorCoreFromPersonality(personality);

		//Change the ability while keeping other personality values the same
		do
		{
			personality = Random32();

			if (isShiny)
			{
				u8 shinyRange = 1;
				personality = (((shinyRange ^ (sid ^ tid)) ^ LOHALF(personality)) << 16) | LOHALF(personality);
			}

			personality &= ~(1);
			personality |= abilityNum; //Either 0 or 1

		} while (GetNatureFromPersonality(personality) != nature
		|| GetGenderFromSpeciesAndPersonality(species, personality) != gender
		|| (!isShiny && IsShinyOtIdPersonality(otId, personality)) //No free shinies
		|| (species == SPECIES_UNOWN && GetUnownLetterFromPersonality(personality) != letter)
		|| (isMinior && GetMiniorCoreFromPersonality(personality) != miniorCore));

		mon->hiddenAbility = FALSE;
		SetMonData(mon, MON_DATA_PERSONALITY, &personality);
		CalculateMonStats(mon);
	}

	GetMonNickname(mon, gStringVar1);
	CopyAbilityName(gStringVar2, GetMonAbility(mon), species);
	StringExpandPlaceholders(gStringVar4, gText_AbilityCapsuleChangedAbility);
	DisplayPartyMenuMessage(gStringVar4, TRUE);
	ScheduleBgCopyTilemapToVram(2);
	gTasks[taskId].func = Task_ClosePartyMenuAfterText;
	RemoveBagItem(item, 1);
}

void FieldUseFunc_MaxPowder(u8 taskId)
{
	gItemUseCB = ItemUseCB_MaxPowder;
	SetUpItemUseCallback(taskId);
}

extern const u8 gText_MaxPowderOfferGive[];
extern const u8 gText_MaxPowderOfferRevert[];
extern const u8 gText_MaxPowderAllowsGigantamax[];
extern const u8 gText_MaxPowderRemovesGigantamax[];
static void ItemUseCB_MaxPowder(u8 taskId, TaskFunc func)
{
	struct Pokemon* mon = &gPlayerParty[gPartyMenu.slotId];

	PlaySE(SE_SELECT);
	if (mon->gigantamax) //Can revert
	{
		GetMonNickname(mon, gStringVar1);
		StringExpandPlaceholders(gStringVar4, gText_MaxPowderOfferRevert);
		DisplayPartyMenuMessage(gStringVar4, TRUE);
		ScheduleBgCopyTilemapToVram(2);
		gTasks[taskId].func = Task_OfferGigantamaxChange;
	}
	else if (GetGigantamaxSpecies(GetMonData(mon, MON_DATA_SPECIES, NULL), TRUE) != SPECIES_NONE) //Has Gigantamax potential
	{
		GetMonNickname(mon, gStringVar1);
		StringExpandPlaceholders(gStringVar4, gText_MaxPowderOfferGive);
		DisplayPartyMenuMessage(gStringVar4, TRUE);
		ScheduleBgCopyTilemapToVram(2);
		gTasks[taskId].func = Task_OfferGigantamaxChange;
	}
	else //No Effect
	{
		gPartyMenuUseExitCallback = FALSE;
		DisplayPartyMenuMessage(gText_WontHaveEffect, TRUE);
		ScheduleBgCopyTilemapToVram(2);
		gTasks[taskId].func = func;
	}
}

static void Task_OfferGigantamaxChange(u8 taskId)
{
	if (IsPartyMenuTextPrinterActive() != TRUE)
	{
		PartyMenuDisplayYesNoMenu();
		gTasks[taskId].func = Task_HandleGigantamaxChangeYesNoInput;
	}
}

static void Task_HandleGigantamaxChangeYesNoInput(u8 taskId)
{
	switch (Menu_ProcessInputNoWrapClearOnChoose())
	{
		case 0:
			gTasks[taskId].func = Task_ChangeGigantamax;
			break;
		case MENU_B_PRESSED:
			PlaySE(SE_SELECT);
			// Fallthrough
		case 1:
			gTasks[taskId].func = Task_ClosePartyMenuAfterText;
			break;
	}
}

static void Task_ChangeGigantamax(u8 taskId)
{
	struct Pokemon* mon = &gPlayerParty[gPartyMenu.slotId];

	PlaySE(SE_USE_ITEM);	
	mon->gigantamax ^= TRUE; //Flip bit
	GetMonNickname(mon, gStringVar1);

	if (mon->gigantamax)
		StringExpandPlaceholders(gStringVar4, gText_MaxPowderAllowsGigantamax);
	else
		StringExpandPlaceholders(gStringVar4, gText_MaxPowderRemovesGigantamax);

	DisplayPartyMenuMessage(gStringVar4, TRUE);
	ScheduleBgCopyTilemapToVram(2);
	gTasks[taskId].func = Task_ClosePartyMenuAfterText;
	RemoveBagItem(Var800E, 1);
}

static void Task_HoneyField(u8 taskId)
{
	SetWeatherScreenFadeOut();
	StartSweetScentFieldEffect();
	DestroyTask(taskId);	
}

void FieldUseFunc_Honey(u8 taskId)
{
	RemoveBagItem(Var800E, 1);
	sItemUseOnFieldCB = Task_HoneyField;
	SetUpItemUseOnFieldCallback(taskId);
}

extern u8 GetCurrentLevelCap(void); //Must be implemented yourself
void ItemUseCB_RareCandy(u8 taskId, TaskFunc func)
{
	bool8 noEffect;
	struct Pokemon *mon = &gPlayerParty[gPartyMenu.slotId];
	u16 item = gSpecialVar_ItemId;
	u8 level = GetMonData(mon, MON_DATA_LEVEL, NULL);

	PlaySE(SE_SELECT);

	if (level >= MAX_LEVEL
	#ifdef FLAG_HARD_LEVEL_CAP
	|| (FlagGet(FLAG_HARD_LEVEL_CAP) && level >= GetCurrentLevelCap())
	#endif
	)
	{
		if (GetEvolutionTargetSpecies(mon, EVO_MODE_NORMAL, 0) == SPECIES_NONE) //Can't use Rare Candy to evolve mon
			noEffect = TRUE;
		else
		{
			RemoveBagItem(item, 1);
			PartyMenuTryEvolution(taskId);
			return;
		}
	}
	else
		noEffect = PokemonItemUseNoEffect(mon, item, gPartyMenu.slotId, 0);

	if (noEffect)
	{
		gPartyMenuUseExitCallback = FALSE;
		DisplayPartyMenuMessage(gText_WontHaveEffect, TRUE);
		ScheduleBgCopyTilemapToVram(2);
		gTasks[taskId].func = func;
	}
	else
	{
		ItemUseCB_RareCandyStep(taskId, func);
	}
}

#ifdef UNBOUND
void FieldUseFunc_VsSeeker(u8 taskId)
{
	u8 mapSec = GetCurrentRegionMapSectionId();

	if ((gMapHeader.mapType != MAP_TYPE_ROUTE
	  && gMapHeader.mapType != MAP_TYPE_TOWN
	  && gMapHeader.mapType != MAP_TYPE_CITY)
	|| mapSec == MAPSEC_GRIM_WOODS
	|| mapSec == MAPSEC_VIVILL_WOODS
	|| mapSec == MAPSEC_REDWOOD_FOREST)
	{
		PrintNotTheTimeToUseThat(taskId, gTasks[taskId].data[3]);
	}
	else
	{
		sItemUseOnFieldCB = (void*) (0x810C670 | 1); //Task_VsSeeker_0
		SetUpItemUseOnFieldCallback(taskId);
	}
}
#endif
