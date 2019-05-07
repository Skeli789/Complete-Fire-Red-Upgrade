#include "defines.h"
#include "../include/constants/hold_effects.h"
#include "../include/new/helper_functions.h"
#include "../include/new/item.h"

u8* ItemId_GetName(u16 itemId)
{
	u8* name = gItems[SanitizeItemId(itemId)].name;
	
	if (name[3] == 0x8) //Expanded Item Names
		name = T1_READ_PTR(name);
    return name;
};

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
};

bool8 IsMegaStone(u16 item)
{
	return ItemId_GetHoldEffect(item) == ITEM_EFFECT_MEGA_STONE;
}

bool8 IsZCrystal(u16 item)
{
	return ItemId_GetHoldEffect(item) == ITEM_EFFECT_Z_CRYSTAL;
}

/*
Expanded TMs/HMs
	credit to ghoulslash
	
These use the items secondary id rather than item id to allow you to insert the TMs anywhere in the item data
*/
#define TMHM_LEARNSET(moves) {(u32)(moves), ((u64)(moves) >> 32)}
#define TMHM(tmhm) ((u64)1 << (ITEM_##tmhm - ITEM_TM01_FOCUS_PUNCH))

#ifdef EXPANDED_TMSHMS
	const u32 gTMHMLearnsets[NUM_SPECIES][4];
	const u16 gTMHMMoves[NUM_TMSHMS];
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
};


// item ID to Tm number to Move ID
//		use secondary ID for tm indices if expanded
u16 ItemIdToBattleMoveId(u16 item) {
	#ifdef EXPANDED_TMSHMS
		u16 tmNum = ItemId_GetSecondaryId(item);	// secondary id -> tm num
	#else
		u16 tmNum = item - ITEM_TM01_FOCUS_PUNCH;
	#endif
    return gTMHMMoves[tmNum];	
};


/*
void LoadTmHmName(s8 *dest, u16 itemId) {
	
	StringCopy(&gStringVar4[0], (void*) 0x84166FF);
	
	if (ItemId_GetSecondaryId(itemId) >= NUM_TMS)
	{
		// HM
		
		
	}
	else
	{
		
	}
};	
*/


	
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
};	
*/
			



/* ORIGINAL
u32 CanMonLearnTMHM(struct Pokemon *mon, u8 tm) {
    u16 species = GetMonData(mon, MON_DATA_SPECIES2, 0);
    if (species == SPECIES_EGG)
    {
        return 0;
    }
    else if (tm < 32)
    {
        u32 mask = 1 << tm;
        return gTMHMLearnsets[species][0] & mask;
    }
    else
    {
        u32 mask = 1 << (tm - 32);
        return gTMHMLearnsets[species][1] & mask;
    }
};
*/
