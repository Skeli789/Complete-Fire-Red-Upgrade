#include "defines.h"
#include "../include/constants/hold_effects.h"
#include "../include/constants/items.h"
#include "../include/constants/moves.h"

#include "../include/new/helper_functions.h"
#include "../include/new/item.h"
#include "../include/shop.h"
#include "../include/constants/songs.h"

u8 ItemId_GetSecondaryId(u16 itemId);
u8* ItemId_GetName(u16 itemId);

extern u8 gMoveNames[][MOVE_NAME_LENGTH + 1];
extern u8 gText_ThrowInOnePremierBall[];
extern u8 gText_ThrowInPremierBalls[];



u8* ItemId_GetName(u16 itemId)
{
	u8* name = gItems[SanitizeItemId(itemId)].name;

	if (name[3] == 0x8) //Expanded Item Names
		name = T1_READ_PTR(name);
    return name;
}


u8 ItemId_GetHoldEffect(u16 itemId)
{
    return gItems[SanitizeItemId(itemId)].holdEffect;
}

u8 ItemId_GetHoldEffectParam(u16 itemId)
{
    return gItems[SanitizeItemId(itemId)].holdEffectParam;
}

u8 ItemId_GetSecondaryId(u16 itemId)
{
    return gItems[SanitizeItemId(itemId)].secondaryId;
}

bool8 IsMegaStone(u16 item)
{
	return ItemId_GetHoldEffect(item) == ITEM_EFFECT_MEGA_STONE;
}

bool8 IsZCrystal(u16 item)
{
	return ItemId_GetHoldEffect(item) == ITEM_EFFECT_Z_CRYSTAL;
}

//TM + HMs////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef EXPANDED_TMSHMS
	typedef u32 TM_HM_T[4]; //extern const u32 gTMHMLearnsets[NUM_SPECIES][4];
#else
	typedef u32 TM_HM_T[2]; //extern const u32 gTMHMLearnsets[NUM_SPECIES][2];
#endif

#define gTMHMLearnsets ((TM_HM_T*) *((u32*) 0x8043C68))
#define gTMHMMoves ((const u16*) *((u32*) 0x8125A8C))

u32 CanMonLearnTMHM(struct Pokemon* mon, u8 tm)
{
    u16 species = GetMonData(mon, MON_DATA_SPECIES2, 0);
    if (species == SPECIES_EGG)
	{
        return 0;
    }

	u32 mask;
	if (tm < 32)
	{
		mask = 1 << tm;
		return gTMHMLearnsets[species][0] & mask;
	}
	else if (tm >= 32 && tm < 64)
	{
		mask = 1 << (tm - 32);
		return gTMHMLearnsets[species][1] & mask;
	}
	#ifdef EXPANDED_TMSHMS
	else if (tm >= 64 && tm < 96)
	{
		mask = 1 << (tm - 64);
		return gTMHMLearnsets[species][2] & mask;
	}
	else if (tm >= 96 && tm < 128)
	{
		mask = 1 << (tm - 96);
		return gTMHMLearnsets[species][3] & mask;
	}
	#endif
	else
		return 0;
}


// item ID to Tm number to Move ID
//		use secondary ID for tm indices if expanded
u16 ItemIdToBattleMoveId(u16 item) {
	#ifdef EXPANDED_TMSHMS
		u16 tmNum = ItemId_GetSecondaryId(item);	// secondary id -> tm num
	#else
		u16 tmNum = item - ITEM_TM01;
	#endif
    return gTMHMMoves[tmNum];
}


u16 RefineTmOrdering(void) {
	#ifdef TMS_BEFORE_HMS
		return 0;
	#else
		return ITEM_TM50;
	#endif
}



// function to fix tm move names that are full length in the bag
void StringAppendFullMoveName(u8 *dest, u8 *src) {
	s8 i;
	if (NUM_HMS >= 10)
		i = -2;
	else
		i = -1;

    while (i < MOVE_NAME_LENGTH)
	{
		dest++;
		i++;
	}

    StringCopy(dest, src);
	dest++;
	u8 end = 0xFF;
	StringAppend(dest, &end);
}


void LoadTmHmName(u8 *dest, u16 itemId) {
	u16 tmNum = ItemId_GetSecondaryId(itemId);
	StringCopy(&gStringVar4[0], (void*) 0x84166FF);

	if (tmNum >= NUM_TMS)
	{
		// HM
		u16 hmNum = tmNum - NUM_TMS + 1;
		StringAppend(&gStringVar4[0], (void*) 0x8463178);
		StringAppend(&gStringVar4[0], (void*) 0x8416226);
		if (NUM_HMS < 10)
			ConvertIntToDecimalStringN(&gStringVar1[0], hmNum, 2, 1);
		else
			ConvertIntToDecimalStringN(&gStringVar1[0], hmNum, 2, 2);
	}
	else
	{
		// TM
		StringAppend(&gStringVar4[0], (void*) 0x8416226);
		if (NUM_TMS < 100)
			ConvertIntToDecimalStringN(&gStringVar1[0], tmNum+1, 2, 2);
		else
			ConvertIntToDecimalStringN(&gStringVar1[0], tmNum+1, 2, 3);
	}
	StringAppend(&gStringVar4[0], &gStringVar1[0]);
	StringAppend(&gStringVar4[0], (void*) 0x846317C);
	StringAppend(&gStringVar4[0], (void*) 0x8416703);

	if (StringLength(gMoveNames[ItemIdToBattleMoveId(itemId)]) == MOVE_NAME_LENGTH && tmNum >= NUM_TMS)
		StringAppendFullMoveName(&gStringVar4[0], gMoveNames[ItemIdToBattleMoveId(itemId)]);
	else
		StringAppend(&gStringVar4[0], gMoveNames[ItemIdToBattleMoveId(itemId)]);

	StringCopy(dest, &gStringVar4[0]);
}



enum {
    CAN_LEARN_MOVE,
    CANNOT_LEARN_MOVE,
    ALREADY_KNOWS_MOVE,
    CANNOT_LEARN_MOVE_IS_EGG
};

u8 CanMonLearnTMTutor(struct Pokemon *mon, u16 item, u8 tutor) {
    u16 move;

    if (GetMonData(mon, MON_DATA_IS_EGG, NULL))
        return CANNOT_LEARN_MOVE_IS_EGG;

    //if (item >= ITEM_TM01_FOCUS_PUNCH)
	if (GetPocketByItemId(item) == POCKET_TM_HM)
    {
        //if (CanMonLearnTMHM(mon, item - ITEM_TM01_FOCUS_PUNCH))
		if (CanMonLearnTMHM(mon, ItemId_GetSecondaryId(item)))
            move = ItemIdToBattleMoveId(item);
        else
            return CANNOT_LEARN_MOVE;
        //do {} while (0); // :morphon:
    }
    else if (CanLearnTutorMove(GetMonData(mon, MON_DATA_SPECIES, NULL), tutor) == FALSE)
    {
        return CANNOT_LEARN_MOVE;
    }
    else
    {
        move = GetTutorMove(tutor);
    }

    if (MonKnowsMove(mon, move) == TRUE)
        return ALREADY_KNOWS_MOVE;
    else
        return CAN_LEARN_MOVE;
}



//08125a90
//080441b8
bool8 CheckIsHmMove(u16 move) {
#ifdef DELETABLE_HMS
	return FALSE;
#else
	for (u16 i = NUM_TMS; i < NUM_TMSHMS; ++i)
	{
		if (move == gTMHMMoves[i])
			return TRUE;
	}

    return FALSE;
#endif
}




// Premier Ball Bonus
#define tItemCount data[1]
#define tItemId data[5]
void Task_ReturnToItemListAfterItemPurchase(u8 taskId)
{
    s16 *data = gTasks[taskId].data;

    if (gMain.newKeys & (A_BUTTON | B_BUTTON))
    {
        PlaySE(SE_SELECT);
		if (tItemId == ITEM_POKE_BALL)
		{
			u8 nPremier = tItemCount/10;
			if (nPremier > 0 && AddBagItem(ITEM_PREMIER_BALL, nPremier) == TRUE)
			{
				ConvertIntToDecimalStringN(&gStringVar1[0], nPremier, 2, 1);
				if (nPremier == 1)
					BuyMenuDisplayMessage(taskId, gText_ThrowInOnePremierBall, BuyMenuReturnToItemList);
				else
					BuyMenuDisplayMessage(taskId, gText_ThrowInPremierBalls, BuyMenuReturnToItemList);
			}
			else
			{
				BuyMenuReturnToItemList(taskId);
			}
		}
		else
		{
			BuyMenuReturnToItemList(taskId);
		}
    }
}



