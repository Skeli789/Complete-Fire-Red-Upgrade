#include "defines.h"
#include "../include/constants/hold_effects.h"
#include "../include/constants/items.h"
#include "../include/constants/moves.h"

#include "../include/new/helper_functions.h"
#include "../include/new/item.h"

u8 ItemId_GetSecondaryId(u16 itemId);

extern u8 gMoveNames[][MOVE_NAME_LENGTH + 1];

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

#ifdef EXPANDED_TMSHMS
	extern const u32 gTMHMLearnsets[NUM_SPECIES][4];
	extern const u16 gTMHMMoves[NUM_TMSHMS];
#else
	const gTMHMLearnsets[][2];
	const gTMHMMoves[];
#endif


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
		u16 tmNum = item - ITEM_TM01_FOCUS_PUNCH;
	#endif
    return gTMHMMoves[tmNum];
}



/*
void SortBerriesOrTMHMs(struct BagPocket *bagPocket) {
    u16 i, j;

    for (i = 0; i < bagPocket->capacity - 1; ++i)
    {
        for (j = i + 1; j < bagPocket->capacity; j++)
        {
            if (GetBagItemQuantity(&bagPocket->itemSlots[i].quantity) != 0)
            {
                if (GetBagItemQuantity(&bagPocket->itemSlots[j].quantity) == 0)
                    continue;
                if (bagPocket->itemSlots[i].itemId <= bagPocket->itemSlots[j].itemId)
                    continue;
            }
            SwapItemSlots(&bagPocket->itemSlots[i], &bagPocket->itemSlots[j]);
        }
    }
}
*/



u16 RefineTmOrdering(void) {
	#ifdef EXPANDED_TMSHMS
		#ifdef TMS_BEFORE_HMS
			return 0;
		#else
			return ITEM_TM50;
		#endif
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




/*
	StringCopy(gStringVar4, gMoveNames[ItemIdToBattleMoveId(itemId)]);
	if (itemId >= ITEM_HM01)
	{
		ConvertIntToDecimalStringN(gStringVar4, itemId-ITEM_HM01+1, 2, 1);
		StringExpandPlaceholders(dest, gText_ClearTo11Var1Clear5Var2);
	}
	else
	{
		ConvertIntToDecimalStringN(gStringVar4, itemId-ITEM_TM01+1, 2, 2);
		StringExpandPlaceholders(dest, gText_UnkF908Var1Clear7Var2);
	}
}
*/



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

/*
bool8 CheckIsHmMove(u16 move) {
    u16 i = NUM_TMS;
    while (gTMHMMoves[i] != 0xFFFF)
	{
        if (sHMMoves[++i] == move)
            return TRUE;
	}
	return FALSE;

*/
