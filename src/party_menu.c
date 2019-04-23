#include "defines.h"
#include "../include/songs.h"
#include "../include/party_menu.h"
#include "../include/new/Vanilla_functions.h"

#include "../include/new/helper_functions.h"

#define MENU_UP -1
#define MENU_DOWN 1
#define MENU_LEFT -2
#define MENU_RIGHT 2

void SetNewPartySelectTarget2(s8* highlightedMon, s8 movementDir);
void CursorCb_Summary(u8 taskId);
void openSummary(u8 taskId);

/*
struct PartyMenuViewing
{
    TaskFunc unk0;
    MainCallback exitCallback;
    u32 unk8_0:1;
    u32 lastViewed:3; //The last mon you selected before going back to the left
    u32 unk8_2:7;
    u32 unk9_0:7;
    u32 unkA_0:14;
    u8 unkC[3]; //unkC[1] is highlighted mon
    u8 unkF[8];
    u8 unk17;
    u16 palBuffer[0xB0];
    u8 filler[0xA0];
    s16 data[16];
};
*/

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
    ((u32*) gPartyMenuView->summaryCallback)[1] = (u32) sub_8122D78; //sub_81B3828 in Emerald

	if (gMain.inBattle != 0)
		openSummary(taskId);
	else
		sub_811FA78(taskId); //sub_81B12C0 in Emerald
}

void openSummary(u8 taskId)
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






