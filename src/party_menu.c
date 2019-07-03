#include "defines.h"
#include "../include/party_menu.h"
#include "../include/text.h"
#include "../include/constants/songs.h"
#include "../include/window.h"

#include "../include/new/build_pokemon.h"
#include "../include/new/Helper_Functions.h"
#include "../include/new/party_menu.h"
#include "../include/new/Vanilla_functions.h"

#define MENU_UP -1
#define MENU_DOWN 1
#define MENU_LEFT -2
#define MENU_RIGHT 2

//This file's functions:
static void OpenSummary(u8 taskId);
static void DisplayPartyPokemonSelectDataSpecial(u8 slot, u8 stringID);
static void DisplayPartyPokemonPriorityText(u8 stringID, struct Struct203B0B4* ptr, u8 c);

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
    ((u32*) gPartyMenuView->unk0)[1] = (u32) sub_8122D78; //sub_81B3828 in Emerald

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

#define PAGE_INFO       0
#define PAGE_SKILLS     1
#define PAGE_ATTACKS    2

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

struct Struct203B0A0
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

#define gUnknown_0203B0A0 ((struct Struct203B0A0*) 0x203B0A0) //gUnknown_0203CEC8 in Emerald
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

	if (FlagGet(BATTLE_TOWER_FLAG))
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

	if ((gUnknown_0203B0A0->unk8 & 0xC0) == 0x80)
	{
		max = 2;
		string = sChoosePokemonMaxStrings[max - 1];
	}
	else
		max = ChoosePokemon_LoadMaxPKMNStr(&string, TRUE);
	
    sub_8121CE4(&gPartyMenuView->unk0[0xC]);
    sub_8121CE4(&gPartyMenuView->unk0[0xD]);

	bool8 eligibleIndices[PARTY_SIZE] = {FALSE};
	for (i = 0; i < PARTY_SIZE; ++i)
		eligibleIndices[i] = IsMonAllowedInBattleTower(&gPlayerParty[i]); //Record who's currently eligible

    for (i = 0; i < max; ++i)
    {
        if (gSelectedOrderFromParty[i] == 0)
        {
            PlaySE(SE_SELECT);
            gSelectedOrderFromParty[i] = gUnknown_0203B0A0->highlightedMon + 1;
            DisplayPartyPokemonPriorityText(i, &gUnknown_0203B0B4[gUnknown_0203B0A0->highlightedMon], 1);
            
			if (i == (max - 1))
                sub_8124258();
			
            display_pokemon_menu_message(0);
            gTasks[taskId].func = sub_811FB28;
			
			for (i = 0; i < PARTY_SIZE; ++i) //Reload everyone else's text if needed
			{	
				if (eligibleIndices[i] //Was eligible before the selection
				&& !IsMonAllowedInBattleTower(&gPlayerParty[i])) //Mon is now ineligible
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
    sub_8121CE4(&gPartyMenuView->unk0[0xC]);
    sub_8121CE4(&gPartyMenuView->unk0[0xD]);
    
	bool8 eligibleIndices[PARTY_SIZE] = {FALSE};
	for (i = 0; i < PARTY_SIZE; ++i)
		eligibleIndices[i] = IsMonAllowedInBattleTower(&gPlayerParty[i]); //Record who's currently eligible
	
	for (i = 0; i < max; ++i) //Remove chosen mon
    {
        if (gSelectedOrderFromParty[i] == gUnknown_0203B0A0->highlightedMon + 1)
        {
            for (j = i; j < (max - 1); ++j)
                gSelectedOrderFromParty[j] = gSelectedOrderFromParty[j + 1];

            gSelectedOrderFromParty[j] = 0;
            break;
        }
    }

    DisplayPartyPokemonOtherText(1, &gUnknown_0203B0B4[gUnknown_0203B0A0->highlightedMon], 1);
    for (i = 0; i < (max - 1); ++i) //Reload the order text
    {
        if (gSelectedOrderFromParty[i] != 0)
            DisplayPartyPokemonPriorityText(i, &gUnknown_0203B0B4[gSelectedOrderFromParty[i] - 1], 1);
    }
	
	for (i = 0; i < PARTY_SIZE; ++i) //Reload everyone else's text if needed
	{	
		if (!eligibleIndices[i] //Wasn't eligible before the cancel
		&& IsMonAllowedInBattleTower(&gPlayerParty[i])) //Mon is now eligible
			DisplayPartyPokemonSelectData(i, 1);  //Reload eligibility text
	}

    display_pokemon_menu_message(0);
    gTasks[taskId].func = sub_811FB28;
}

void DisplayPartyPokemonSelectForBattle(u8 slot)
{
	u8 max;
    struct Pokemon *mon = &gPlayerParty[slot];
    u8* ptr = gSelectedOrderFromParty;
	
	if ((gUnknown_0203B0A0->unk8 & 0xC0) == 0x80)
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
    u8 tier = VarGet(BATTLE_TOWER_TIER);
    struct Pokemon* party = gPlayerParty;
    u8 maxLength = GetNumMonsOnTeamInFrontier();

    if (gSelectedOrderFromParty[maxLength - 1] == 0) //No mon's entered
    {
        if (maxLength == 1)
            return 14;

        ConvertIntToDecimalStringN(gStringVar1, maxLength, 0, 1);
        return 17;
    }

    for (i = 0; i < maxLength - 1; ++i)
    {
        u16 species = GetMonData(&party[gSelectedOrderFromParty[i] - 1], MON_DATA_SPECIES, NULL);
        u16 item = GetMonData(&party[gSelectedOrderFromParty[i] - 1], MON_DATA_HELD_ITEM, NULL);
        for (j = i + 1; j < maxLength; j++)
        {
            if (species == GetMonData(&party[gSelectedOrderFromParty[j] - 1], MON_DATA_SPECIES, NULL))
                return 18;
            if (item != 0 && (tier == BATTLE_TOWER_STANDARD || tier == BATTLE_TOWER_MIDDLE_CUP)
			&& item == GetMonData(&party[gSelectedOrderFromParty[j] - 1], MON_DATA_HELD_ITEM, NULL))
                return 19;
        }
    }

    return 0xFF;
}