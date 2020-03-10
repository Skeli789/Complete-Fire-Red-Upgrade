#include "defines.h"
#include "../include/field_control_avatar.h"
#include "../include/field_player_avatar.h"
#include "../include/field_effect.h"
#include "../include/field_weather.h"
#include "../include/fieldmap.h"
#include "../include/menu.h"
#include "../include/metatile_behavior.h"
#include "../include/overworld.h"
#include "../include/party_menu.h"
#include "../include/script.h"
#include "../include/string_util.h"
#include "../include/text.h"
#include "../include/window.h"
#include "../include/constants/items.h"
#include "../include/constants/moves.h"
#include "../include/constants/songs.h"

#include "../include/new/build_pokemon.h"
#include "../include/new/follow_me.h"
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

//This file's functions:
static void OpenSummary(u8 taskId);
static void DisplayPartyPokemonSelectDataSpecial(u8 slot, u8 stringID);
static void DisplayPartyPokemonPriorityText(u8 stringID, struct Struct203B0B4* ptr, u8 c);
static bool8 SetUpFieldMove_Fly(void);
static bool8 SetUpFieldMove_Surf(void);
static bool8 SetUpFieldMove_Waterfall(void);
static bool8 SetUpFieldMove_Teleport(void);
static void FieldCallback_Dive(void);
static bool8 SetUpFieldMove_Dive(void);
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
			gPartyMenuView->lastViewed = *highlightedMon;

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
				switch (gPartyMenuView->lastViewed) {
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
						*highlightedMon = gPartyMenuView->lastViewed;
				}
				break;
		}
		break;

	case MENU_DOWN:
		if (*highlightedMon != 6 && *highlightedMon != 7)
			gPartyMenuView->lastViewed = *highlightedMon;

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
				switch (gPartyMenuView->lastViewed) {
					case 0:
					case 1:
						*highlightedMon = gPartyMenuView->lastViewed;
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
			gPartyMenuView->lastViewed = *highlightedMon;

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
			gPartyMenuView->lastViewed = *highlightedMon;

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
			if (gPartyMenuView->unk8_0) //gUnknown_0203CEC4->unk8_0 in Emerald
			{
				*highlightedMon = 6; //Set to 6 here because the minus will set it to 5
				break;
			}
			(*highlightedMon)--;
		}
		//This probably relates to getting the next mon in the party
		unk2 = sub_812029C(*highlightedMon, unk2); //sub_81B1B00 in Emerald
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
			unk2 = sub_812029C(*highlightedMon, 1); //sub_81B1B00 in Emerald
			if (unk2 == -1)
			{
				if (gPartyMenuView->unk8_0) //gUnknown_0203CEC4->unk8_0 in Emerald
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
			if (gPartyMenuView->lastViewed == 3) //gUnknown_0203CEC4->unk8_1 in Emerald
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
			if (gPartyMenuView->lastViewed == 5) //gUnknown_0203CEC4->unk8_1 in Emerald
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
			gPartyMenuView->lastViewed = *highlightedMon;
			*highlightedMon = 0;
		}
		else if (*highlightedMon == 4 || *highlightedMon == 5)
		{
			gPartyMenuView->lastViewed = *highlightedMon;
			*highlightedMon = 1;
		}
		break;
	}
#endif
}

void CursorCb_Summary(u8 taskId)
{
	PlaySE(SE_SELECT);
	gPartyMenuView->exitCallback = sub_8122D78; //sub_81B3828 in Emerald

	if (gMain.inBattle != 0)
		OpenSummary(taskId);
	else
		sub_811FA78(taskId); //sub_81B12C0 in Emerald
}

static void OpenSummary(u8 taskId)
{
	sub_811FA78(taskId); //Replace this
}

// Summary Screen wraps around
// Credit to Sagiri: https://github.com/Sagiri/sswa

#define PAGE_INFO	   0
#define PAGE_SKILLS	 1
#define PAGE_ATTACKS	2

u8 ChangeSummaryScreenMon(u8 delta)
{
	u8 numMons = gSummaryScreenData->maxPartyIndex + 1;
	delta += numMons;

	// guarantees result will be in range [0, numMons)
	u8 result = umodsi(gCurrentPartyIndex + delta, numMons);

	// skip over eggs on other pages
	if (gSummaryScreenData->currentPage != PAGE_INFO)
	{
		while (GetMonData(gSummaryScreenData->partyData+result, MON_DATA_IS_EGG, NULL))
		{
			result = umodsi(result + delta, numMons);
		}
	}

	// necessary to gracefully handle parties of 1 Pokemon
	if (result == gCurrentPartyIndex)
		return -1;

	return result;
};

//Battle Tower Selection Updates//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct PartyMenuView2
{
	MainCallback exitCallback;
	TaskFunc unk4;
	u8 unk8;
	//u8 unk8_0:4;
	//u8 mode:2;
	//u8 unk8_2:2;
	s8 highlightedMon;
	s8 unkA;
	u8 unkB;
	u16 unkC;
	s16 unkE;
	s16 unk10;
};

#define gPartyMenuView2 ((struct PartyMenuView2*) 0x203B0A0) //gUnknown_0203CEC8 in Emerald
#define gUnknown_0203B0B4 ((struct Struct203B0B4*) *((u32*) 0x203B0B4)) //gUnknown_0203CEDC in Emerald

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

extern const struct TextColor gUnknown_08459FFC[];

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

	if ((gPartyMenuView2->unk8 & 0xC0) == 0x80)
	{
		max = 2;
		string = sChoosePokemonMaxStrings[max - 1];
	}
	else
		max = ChoosePokemon_LoadMaxPKMNStr(&string, TRUE);

	sub_8121CE4(&gPartyMenuView->windowId[0]);
	sub_8121CE4(&gPartyMenuView->windowId[1]);

	bool8 eligibleIndices[PARTY_SIZE] = {FALSE};
	for (i = 0; i < PARTY_SIZE; ++i)
		eligibleIndices[i] = IsMonAllowedInBattleTower(&gPlayerParty[i]); //Record who's currently eligible

	for (i = 0; i < max; ++i)
	{
		if (gSelectedOrderFromParty[i] == 0)
		{
			PlaySE(SE_SELECT);
			gSelectedOrderFromParty[i] = gPartyMenuView2->highlightedMon + 1;
			DisplayPartyPokemonPriorityText(i, &gUnknown_0203B0B4[gPartyMenuView2->highlightedMon], 1);

			if (i == (max - 1))
				sub_8124258();

			PrintPartyMenuPromptText(0);
			gTasks[taskId].func = sub_811FB28;

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
	DisplayPartyMenuMsgBox(string, 1);
	gTasks[taskId].func = sub_81203B8;
}

void CursorCb_NoEntry(u8 taskId)
{
	int i, j;
	u8 max = ChoosePokemon_LoadMaxPKMNStr(NULL, FALSE);

	PlaySE(SE_SELECT);
	sub_8121CE4(&gPartyMenuView->windowId[0]);
	sub_8121CE4(&gPartyMenuView->windowId[1]);

	bool8 eligibleIndices[PARTY_SIZE] = {FALSE};
	for (i = 0; i < PARTY_SIZE; ++i)
		eligibleIndices[i] = IsMonAllowedInBattleTower(&gPlayerParty[i]); //Record who's currently eligible

	for (i = 0; i < max; ++i) //Remove chosen mon
	{
		if (gSelectedOrderFromParty[i] == gPartyMenuView2->highlightedMon + 1)
		{
			for (j = i; j < (max - 1); ++j)
				gSelectedOrderFromParty[j] = gSelectedOrderFromParty[j + 1];

			gSelectedOrderFromParty[j] = 0;
			break;
		}
	}

	DisplayPartyPokemonOtherText(1, &gUnknown_0203B0B4[gPartyMenuView2->highlightedMon], 1);
	for (i = 0; i < (max - 1); ++i) //Reload the order text
	{
		if (gSelectedOrderFromParty[i] != 0)
			DisplayPartyPokemonPriorityText(i, &gUnknown_0203B0B4[gSelectedOrderFromParty[i] - 1], 1);
	}

	for (i = 0; i < PARTY_SIZE; ++i) //Reload everyone else's text if needed
	{
		if (!eligibleIndices[i] //Wasn't eligible before the cancel
		&& IsMonAllowedInBattleTower(&gPlayerParty[i]) //Mon is now eligible
		&& GetMonData(&gPlayerParty[i], MON_DATA_SPECIES, NULL) != SPECIES_NONE) //Legit Pokemon
			DisplayPartyPokemonSelectData(i, 1);  //Reload eligibility text
	}

	PrintPartyMenuPromptText(0);
	gTasks[taskId].func = sub_811FB28;
}

void DisplayPartyPokemonSelectForBattle(u8 slot)
{
	u8 max;
	struct Pokemon *mon = &gPlayerParty[slot];
	u8* ptr = gSelectedOrderFromParty;

	if ((gPartyMenuView2->unk8 & 0xC0) == 0x80)
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

	gUnknown_0203B0B4[slot].unk0->unk0(gUnknown_0203B0B4[slot].windowId, 0, 0, 0, 0, 1);
	DisplayPartyPokemonNickname(mon, &gUnknown_0203B0B4[slot], 0);
	if (!GetMonData(mon, MON_DATA_IS_EGG, NULL))
	{
		DisplayPartyPokemonLevelCheck(mon, &gUnknown_0203B0B4[slot], 0);
		DisplayPartyPokemonGenderNidoranCheck(mon, &gUnknown_0203B0B4[slot], 0);
	}

	DisplayPartyPokemonPriorityText(stringID, &gUnknown_0203B0B4[slot], 0);
}

static void DisplayPartyPokemonPriorityText(u8 stringID, struct Struct203B0B4* ptr, u8 c)
{
	if (c != 0)
	{
		int unk = ((ptr->unk0->unk1C % 8) + ptr->unk0->unk1E + 7) / 8;
		int unk2 = ((ptr->unk0->unk1D % 8) + ptr->unk0->unk1F + 7) / 8;
		ptr->unk0->unk0(ptr->windowId, ptr->unk0->unk1C >> 3, ptr->unk0->unk1D >> 3, unk, unk2, 1);
	}
	if (c != 2)
		WindowPrint(ptr->windowId, 1, ptr->unk0->unk1C, ptr->unk0->unk1D, (void*) 0x8459FFC, 0, sOrderStrings[stringID]);
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
	[FIELD_MOVE_SWEET_SCENT] = {(void*) 0x80DE0C9, 0x0d},
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

#define FIELD_MOVE_TERMINATOR MOVE_GUILLOTINE
const u16 gFieldMoves[] =
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
	[FIELD_MOVE_COUNT] = FIELD_MOVE_TERMINATOR
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
	[FIELD_MOVE_DEFOG] = 1,
	[FIELD_MOVE_CUT] = 2,
	[FIELD_MOVE_ROCK_SMASH] = 3,
	[FIELD_MOVE_STRENGTH] = 4,
	[FIELD_MOVE_SURF] = 5,
	[FIELD_MOVE_ROCK_CLIMB] = 6,
	[FIELD_MOVE_WATERFALL] = 7,
	[FIELD_MOVE_DIVE] = 8,
	[FIELD_MOVE_FLY] = 0,
	[FIELD_MOVE_FLASH] = 0,
};

#endif

static bool8 SetUpFieldMove_Fly(void)
{
	if (gFollowerState.inProgress && !(gFollowerState.flags & FOLLOWER_FLAG_CAN_LEAVE_ROUTE))
		return FALSE;

	if (Overworld_MapTypeAllowsTeleportAndFly(gMapHeader.mapType) == TRUE)
		return TRUE;

	return FALSE;
}

#define FieldCallback_Surf (void*) (0x812497C | 1)
static bool8 SetUpFieldMove_Surf(void)
{
	if (gFollowerState.inProgress && !(gFollowerState.flags & FOLLOWER_FLAG_CAN_SURF))
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
    if (MetatileBehavior_IsWaterfall(MapGridGetMetatileBehaviorAt(x, y)) == TRUE && IsPlayerSurfingNorth() == TRUE)
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
	((u32*) gFieldEffectArguments)[0] = GetCursorSelectionMonId();
	FieldEffectStart(FLDEFF_USE_DIVE);
}

static bool8 SetUpFieldMove_Dive(void)
{
	if (gFollowerState.inProgress && !(gFollowerState.flags & FOLLOWER_FLAG_CAN_DIVE))
		return FALSE;

	((u32*) gFieldEffectArguments)[1] = TrySetDiveWarp();
	if (((u32*) gFieldEffectArguments)[1] != 0)
	{
		gFieldCallback2 = FieldCallback_PrepareFadeInFromMenu;
		gPostMenuFieldCallback = FieldCallback_Dive;
		return TRUE;
	}

	return FALSE;
}

static void FieldCallback_RockClimb(void)
{
	((u32*) gFieldEffectArguments)[0] = GetCursorSelectionMonId();
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

	((u32*) gFieldEffectArguments)[0] = monId;
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

	gSpecialVar_LastResult = MetatileBehavior_IsWaterfall(MapGridGetMetatileBehaviorAt(x, y)) && IsPlayerSurfingNorth();
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
		Var8004 = PartyHasMonWithFieldMovePotential(MOVE_CUT, item, SHOULDNT_BE_SURFING);
}

void sp10B_CanUseRockSmashOnRock(void)
{
	u16 item = ITEM_NONE;

	#ifdef ONLY_CHECK_ITEM_FOR_HM_USAGE
	item = ITEM_HM06_ROCK_SMASH;
	#endif

	Var8004 = PARTY_SIZE;
	if (HasBadgeToUseFieldMove(FIELD_MOVE_ROCK_SMASH))
		Var8004 = PartyHasMonWithFieldMovePotential(MOVE_ROCKSMASH, item, SHOULDNT_BE_SURFING);
}

void sp10C_CanUseStrengthOnBoulder(void)
{
	u16 item = ITEM_NONE;

	#ifdef ONLY_CHECK_ITEM_FOR_HM_USAGE
	item = ITEM_HM04_STRENGTH;
	#endif

	Var8004 = PARTY_SIZE;
	if (HasBadgeToUseFieldMove(FIELD_MOVE_STRENGTH))
		Var8004 = PartyHasMonWithFieldMovePotential(MOVE_STRENGTH, item, SHOULDNT_BE_SURFING);
}

//Move Item - Credits to Sagiri/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define gText_PkmnNotHolding (const u8*) 0x08416C8F
#define  gText_PkmnWasGivenItem (const u8*) 0x8416C2A
extern const u8 gText_SwapItemsPart1[];
extern const u8 gText_SwapItemsPart2[];

#define MSG_MOVE 37

const u8 gPartyMenu_ItemMenuActions[] = {MENU_GIVE, MENU_TAKE_ITEM, MENU_MOVE_ITEM, MENU_CANCEL2};

static void CursorCb_MoveItemCallback(u8 taskId)
{
	u16 item1, item2;
	u8 buffer[100];

	if (gPaletteFade->active || some_other_kind_of_link_test())
		return;

	switch (PartyMenuButtonHandler(&gPartyMenuView2->unkA)) {
		case 1: //User hit A on a Pokemon
			//Pokemon can't give away items to eggs or themselves
			if (GetMonData(&gPlayerParty[gPartyMenuView2->unkA], MON_DATA_IS_EGG, NULL)
			|| gPartyMenuView2->highlightedMon == gPartyMenuView2->unkA)
			{
				PlaySE(SE_ERROR);
				return;
			}

			PlaySE(SE_SELECT);
			gPartyMenuView2->unkB = 0;

			//Look up held items
			item1 = GetMonData(&gPlayerParty[gPartyMenuView2->highlightedMon], MON_DATA_HELD_ITEM, NULL);
			item2 = GetMonData(&gPlayerParty[gPartyMenuView2->unkA], MON_DATA_HELD_ITEM, NULL);

			//Swap the held items
			SetMonData(&gPlayerParty[gPartyMenuView2->highlightedMon], MON_DATA_HELD_ITEM, &item2);
			SetMonData(&gPlayerParty[gPartyMenuView2->unkA], MON_DATA_HELD_ITEM, &item1);

			//Update the held item icons
			sub_81224B4(&gPlayerParty[gPartyMenuView2->highlightedMon],
						&gUnknown_0203B0B4[gPartyMenuView2->highlightedMon]);

			sub_81224B4(&gPlayerParty[gPartyMenuView2->unkA],
						&gUnknown_0203B0B4[gPartyMenuView2->unkA]);

			//Create the string describing the move
			if (item2 == ITEM_NONE)
			{
				GetMonNickname(&gPlayerParty[gPartyMenuView2->unkA], gStringVar1);
				CopyItemName(item1, gStringVar2);
				StringExpandPlaceholders(gStringVar4, gText_PkmnWasGivenItem);
			}
			else
			{
				GetMonNickname(&gPlayerParty[gPartyMenuView2->highlightedMon], gStringVar1);
				CopyItemName(item1, gStringVar2);
				StringExpandPlaceholders(buffer, gText_SwapItemsPart1);

				StringAppend(buffer, gText_SwapItemsPart2);
				GetMonNickname(&gPlayerParty[gPartyMenuView2->unkA], gStringVar1);
				CopyItemName(item2, gStringVar2);
				StringExpandPlaceholders(gStringVar4, buffer);
			}

			//Display the string
			DisplayPartyMenuMsgBox(gStringVar4, 1);

			//Update colors of selected boxes
			sub_811F818(gPartyMenuView2->unkA, 0);
			sub_811F818(gPartyMenuView2->highlightedMon, 1);

			//Return to the main party menu
			BgIdMarkForSync(2);
			gTasks[taskId].func = sub_8123BF0;
			break;
		case 2:	 // User hit B or A while on Cancel
			sub_811FD88(taskId, &gPartyMenuView2->unkA);
			break;
	}
}

static void CursorCb_MoveItem(u8 taskId)
{
	struct Pokemon *mon = &gPlayerParty[gPartyMenuView2->highlightedMon];

	PlaySE(SE_SELECT);

	//Delete old windows
	sub_8121CE4(&gPartyMenuView->windowId[1]);
	sub_8121CE4(&gPartyMenuView->windowId[0]);

	if (GetMonData(mon, MON_DATA_HELD_ITEM, NULL) != ITEM_NONE)
	{
		gPartyMenuView2->unkB = 8;

		//Show "Move item to where" in bottom left
		PrintPartyMenuPromptText(MSG_MOVE);
		//Update color of first selected box
		sub_811F818(gPartyMenuView2->highlightedMon, 1);

		//Set up callback
		gPartyMenuView2->unkA = gPartyMenuView2->highlightedMon;
		gTasks[taskId].func = CursorCb_MoveItemCallback;
	}
	else
	{
		//Create and display string about lack of hold item
		GetMonNickname(mon, gStringVar1);
		StringExpandPlaceholders(gStringVar4, gText_PkmnNotHolding);
		DisplayPartyMenuMsgBox(gStringVar4, 1);

		//Return to the main party menu
		BgIdMarkForSync(2);
		gTasks[taskId].func = sub_8123BF0;
	}
}
