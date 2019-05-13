#include "defines.h"
#include "../include/script.h"
#include "../include/shop.h"
#include "../include/constants/hold_effects.h"
#include "../include/constants/items.h"
#include "../include/constants/moves.h"
#include "../include/constants/songs.h"

#include "../include/new/helper_functions.h"
#include "../include/new/item.h"

#define EOS 0xFF

u8 ItemId_GetSecondaryId(u16 itemId);
u8* ItemId_GetName(u16 itemId);

extern u8 gMoveNames[][MOVE_NAME_LENGTH + 1];
extern u8 gText_ThrowInOnePremierBall[];
extern u8 gText_ThrowInPremierBalls[];

//This file's functions:
static s8 CompareItemsAlphabetically(u16 item1, u16 item2);
static s8 CompareItemsByType(u16 item1, u16 item2);
static void MergeSort(struct ItemSlot* array, u32 low, u32 high, s8 (*comparator)(u16, u16));
static void Merge(struct ItemSlot* arr, u32 l, u32 m, u32 r, s8 (*comparator)(u16, u16));

//General Utility Functions
u16 SanitizeItemId(u16 itemId)
{
    if (itemId >= ITEMS_COUNT)
        return ITEM_NONE;

    return itemId;
}

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
    return ItemId_GetMystery2(itemId);
}

bool8 IsMegaStone(u16 item)
{
	return ItemId_GetHoldEffect(item) == ITEM_EFFECT_MEGA_STONE;
}

bool8 IsZCrystal(u16 item)
{
	return ItemId_GetHoldEffect(item) == ITEM_EFFECT_Z_CRYSTAL;
}

bool8 IsBerry(u16 item)
{
	return GetPocketByItemId(item) == POCKET_BERRY_POUCH;
}

u8 TMIdFromItemId(u16 itemId)
{
	u8 tmNum = ItemId_GetSecondaryId(itemId);
	if (itemId == ITEM_NONE)
		tmNum = 255; //So blank items get put at the end
	else if (tmNum == 0)
		tmNum = itemId - ITEM_TM01;

	return tmNum;
}

u8 BerryIdFromItemId(u16 item)
{
	u8 secondaryId = ItemId_GetSecondaryId(item);
	if (secondaryId != 0)
		return secondaryId;

	if (item < ITEM_CHERI_BERRY
	||  item >= ITEM_ENIGMA_BERRY_OLD)
		return 255;
		
	return item - ITEM_CHERI_BERRY;
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
u16 ItemIdToBattleMoveId(u16 item)
{
    return gTMHMMoves[TMIdFromItemId(item)];
}


u16 RefineTmOrdering(void)
{
	#ifdef TMS_BEFORE_HMS
		return 0;
	#else
		return ITEM_TM50;
	#endif
}

//Function to fix tm move names that are full length in the bag
void StringAppendFullMoveName(u8* dst, u8* src)
{
	s8 i;
	if (NUM_HMS >= 10)
		i = -2;
	else
		i = -1;

    while (i < MOVE_NAME_LENGTH)
	{
		dst++;
		i++;
	}

    StringCopy(dst, src);
	dst++;
	u8 end = 0xFF;
	StringAppend(dst, &end);
}


void LoadTmHmName(u8 *dst, u16 itemId)
{
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

	StringCopy(dst, &gStringVar4[0]);
}

enum
{
    CAN_LEARN_MOVE,
    CANNOT_LEARN_MOVE,
    ALREADY_KNOWS_MOVE,
    CANNOT_LEARN_MOVE_IS_EGG,
};

u8 CanMonLearnTMTutor(struct Pokemon* mon, u16 item, u8 tutor)
{
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
    else if (!CanLearnTutorMove(mon->species, tutor))
    {
        return CANNOT_LEARN_MOVE;
    }
    else
    {
        move = GetTutorMove(tutor);
    }

    if (MonKnowsMove(mon, move))
        return ALREADY_KNOWS_MOVE;
    else
        return CAN_LEARN_MOVE;
}

//08125a90
//080441b8
bool8 CheckIsHmMove(u16 move)
{
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
    s16* data = gTasks[taskId].data;

    if (gMain.newKeys & (A_BUTTON | B_BUTTON))
    {
        PlaySE(SE_SELECT);
		if (tItemId == ITEM_POKE_BALL)
		{
			#ifdef MULTIPLE_PREMIER_BALLS_AT_ONCE
			u8 nPremier = tItemCount / 10;
			#else
			u8 nPremier = 1;
			#endif
			if (nPremier > 0 && AddBagItem(ITEM_PREMIER_BALL, nPremier))
			{
				ConvertIntToDecimalStringN(gStringVar1, nPremier, 2, 1);
				StringCopy(gStringVar2, ItemId_GetName(ITEM_PREMIER_BALL));
				
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

//Bag Expansion////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define gBagPockets ((struct BagPockets*) 0x203988C)

#define sBagRegularItems ((struct ItemSlot*) 0x203BB20)
#define sBagKeyItems (sBagRegularItems + NUM_REGULAR_ITEMS)
#define sBagPokeBalls (sBagKeyItems + NUM_KEY_ITEMS)
#define sBagTMHM (sBagPokeBalls + NUM_POKE_BALLS)
#define sBagBerries (sBagTMHM + NUM_TMSHMS)

#define sBagItemAmounts ((u16*) 0x203C74A)

#define NUM_REGULAR_ITEMS 450
#define NUM_KEY_ITEMS 75
#define NUM_POKE_BALLS 50
#define NUM_BERRIES 75

#define LARGEST_POCKET_NUM NUM_REGULAR_ITEMS
#define TOTAL_POSSIBLE_BAG_ITEMS (450 + 75 + 50 + 75 + 128) //Do not change this line!

//Throw an error if the player tried to add too many items.
#if (NUM_REGULAR_ITEMS + NUM_KEY_ITEMS + NUM_POKE_BALLS + NUM_BERRIES + NUM_TMSHMS) > TOTAL_POSSIBLE_BAG_ITEMS
#error "The total number of bag items has exceeded 650! Please reduce the possible number of items in the bag."
#endif

struct ListBuffer1
{
    struct ListMenuItem subBuffers[LARGEST_POCKET_NUM];
};

struct ListBuffer2
{
    s8 name[LARGEST_POCKET_NUM][24];
};

struct BerryListBuffer
{
	struct ListMenuItem subBuffers[NUM_BERRIES];
};

struct BagPockets
{
	struct ItemSlot* itemRam;
	u32 itemAmount;
	struct ItemSlot* keyItemRam;
	u32 keyItemAmount;
	struct ItemSlot* pokeBallRam;
	u32 pokeBallAmount;
	struct ItemSlot* tmRam;
	u32 tmAmount;
	struct ItemSlot* berryRam;
	u32 berryAmount;
};

static const struct BagPockets sBagPocketArrangement =
{
	.itemRam = sBagRegularItems,
	.itemAmount = NUM_REGULAR_ITEMS,
	.keyItemRam = sBagKeyItems,
	.keyItemAmount = NUM_KEY_ITEMS,
	.pokeBallRam = sBagPokeBalls,
	.pokeBallAmount = NUM_POKE_BALLS,
	.tmRam = sBagTMHM,
	.tmAmount = NUM_TMSHMS,
	.berryRam = sBagBerries,
	.berryAmount = NUM_BERRIES,
};

void SetMemoryForBagStorage(void)
{
	*gBagPockets = sBagPocketArrangement;
}

#define sListBuffer1 (*((struct ListBuffer1**) 0x203AD18))
#define sListBuffer2 (*((struct ListBuffer2**) 0x203AD1C))

void AllocateBagItemListBuffers(void)
{
    sListBuffer1 = Malloc(sizeof(struct ListBuffer1));
    sListBuffer2 = Malloc(sizeof(struct ListBuffer2));
}

#define sBerryListBuffer (*((struct BerryListBuffer**) 0x203F37C))
bool8 AllocateBerryPouchListBuffers(void)
{
	sBerryListBuffer = Malloc(sizeof(struct BerryListBuffer));
	
	if (sBerryListBuffer == NULL)
		return FALSE;
	
	return TRUE;
}

#define sPokeDudeItemBackupPtr (*((struct ItemSlot**) 0x203AD2C))
void PokeDudeBackupBag(void)
{
	sPokeDudeItemBackupPtr = Calloc((NUM_REGULAR_ITEMS + NUM_KEY_ITEMS + NUM_POKE_BALLS) * sizeof(struct ItemSlot));
	Memcpy(sPokeDudeItemBackupPtr, sBagPocketArrangement.itemRam, (NUM_REGULAR_ITEMS + NUM_KEY_ITEMS + NUM_POKE_BALLS) * sizeof(struct ItemSlot));
	Memset(sBagPocketArrangement.itemRam, 0, (NUM_REGULAR_ITEMS + NUM_KEY_ITEMS + NUM_POKE_BALLS) * sizeof(struct ItemSlot));
}

void PokeDudeRestoreBag(void)
{
	Memcpy(sBagPocketArrangement.itemRam, sPokeDudeItemBackupPtr, (NUM_REGULAR_ITEMS + NUM_KEY_ITEMS + NUM_POKE_BALLS) * sizeof(struct ItemSlot));
}

#define sPokeDudeItemBackupPtr2 (*((struct ItemSlot**) 0x203B11C))
void PokeDudeBackupKeyItemsTMs(void)
{
	sPokeDudeItemBackupPtr2 = Calloc((NUM_KEY_ITEMS + NUM_TMSHMS) * sizeof(struct ItemSlot));
	Memcpy(sPokeDudeItemBackupPtr2, sBagPocketArrangement.keyItemRam, NUM_KEY_ITEMS * sizeof(struct ItemSlot));
	Memcpy(sPokeDudeItemBackupPtr2 + NUM_KEY_ITEMS + NUM_POKE_BALLS, sBagPocketArrangement.tmRam, NUM_TMSHMS * sizeof(struct ItemSlot));
	
	Memset(sBagPocketArrangement.keyItemRam, 0, NUM_KEY_ITEMS * sizeof(struct ItemSlot));
	Memset(sBagPocketArrangement.tmRam, 0, NUM_TMSHMS * sizeof(struct ItemSlot));
}

void PokeDudeRestoreKeyItemsTMs(void)
{
	Memcpy(sBagPocketArrangement.keyItemRam, sPokeDudeItemBackupPtr2, NUM_KEY_ITEMS * sizeof(struct ItemSlot));
	Memcpy(sBagPocketArrangement.tmRam, sPokeDudeItemBackupPtr2 + NUM_KEY_ITEMS + NUM_POKE_BALLS, NUM_TMSHMS * sizeof(struct ItemSlot));
}

void StoreBagItemCount(void)
{
	struct ItemSlot* itemMem;
	u16 itemAmount;
	u16 checkLength;
	bool8 tossed = FALSE;
	
	for (u8 pocket = 0; pocket < 3; ++pocket)
	{
		switch (pocket) {
			case 0:
				itemMem = sBagPocketArrangement.itemRam;
				itemAmount = sBagPocketArrangement.itemAmount;
				break;
			case 1:
				itemMem = sBagPocketArrangement.keyItemRam;
				itemAmount = sBagPocketArrangement.keyItemAmount;
				break;		
			default:
				itemMem = sBagPocketArrangement.pokeBallRam;
				itemAmount = sBagPocketArrangement.pokeBallAmount;	
		}

		for (checkLength = 0; itemMem[checkLength].itemId != ITEM_NONE; ++checkLength);

		if (checkLength + 1 < itemAmount && itemMem[checkLength + 1].itemId != ITEM_NONE)
		{
			tossed = TRUE;
			++checkLength;
			
			for (; itemMem[checkLength].itemId != ITEM_NONE; ++checkLength);	
		}

		if (tossed)
			checkLength -= 1;

		sBagItemAmounts[pocket] = checkLength;
	}
}

u16 GetNumItemsInPocket(u8 pocket)
{
	if (pocket > 2)
		return 0;
	
	return sBagItemAmounts[pocket];
}

#define sCurrentBagPocket (*((u8*) 0x203AD02))
u16 GetCurrentPocketItemAmount(void)
{
	return sBagItemAmounts[sCurrentBagPocket];
}

bool8 DoesBagHaveBerry(void)
{
	if (CheckBagHasItem(ITEM_BERRY_POUCH, 1)
	&& sBagBerries[0].itemId != 0) //At least one berry in berry pouch
		return gSpecialVar_LastResult = 1;
	
	return gSpecialVar_LastResult = 0;
}


//Functions For Sorting Bag
static s8 CompareBerries(u16 item1, u16 item2)
{
	u8 id1 = BerryIdFromItemId(item1);
	u8 id2 = BerryIdFromItemId(item2);
	
	if (id1 < id2)
		return -1;
	else if (id1 > id2)
		return 1;

	return 0;
}

static s8 CompareTMs(u16 item1, u16 item2)
{
	u8 id1 = TMIdFromItemId(item1);
	u8 id2 = TMIdFromItemId(item2);
	
	#ifndef TMS_BEFORE_HMS
		if (id1 > NUM_TMS)
		{
			id1 -= NUM_TMS; //HM
			id2 += NUM_HMS; //So if TM, will always follow HM
		}
		
		if (id2 > NUM_TMS)
		{
			id2 -= NUM_TMS; //HM
			id1 += NUM_HMS; //So if TM, will always follow HM
		}
	#endif
	
	if (id1 < id2)
		return -1;
	else if (id1 > id2)
		return 1;

	return 0;
}

void SortBerriesOrTMHMs(struct BagPocket* bagPocket)
{
	s8 (*func)(u16, u16);
	u16 itemAmount = bagPocket->capacity;
	
	if (bagPocket->itemSlots == sBagPocketArrangement.tmRam)
		func = CompareTMs;
	else
		func = CompareBerries;
		
	MergeSort(bagPocket->itemSlots, 0, itemAmount - 1, func);
}

static s8 CompareItemsAlphabetically(u16 item1, u16 item2)
{
	if (item1 == ITEM_NONE)
		return 1;
	else if (item2 == ITEM_NONE)
		return -1;

	u8* name1 = ItemId_GetName(item1);
	u8* name2 = ItemId_GetName(item2);
	
	for (int i = 0; ; ++i)
	{
		if (name1[i] == EOS && name2[i] != EOS)
			return -1;
		else if (name1[i] != EOS && name2[i] == EOS)
			return 1;
		else if (name1[i] == EOS && name2[i] == EOS)
			return 0;
			
		if (name1[i] < name2[i])
			return -1;
		else if (name1[i] > name2[i])
			return 1;
	}
	
	return 0; //Will never be reached
}

u16 sItemsByType[ITEMS_COUNT] =
{
	[ITEM_REPEL] = 0,
	[ITEM_SUPER_REPEL] = 1,
	[ITEM_MAX_REPEL] = 2,

/*
	[ITEM_VENUSAURITE] = ,
	[ITEM_CHARIZARDITE_X] = ,
	[ITEM_CHARIZARDITE_Y] = ,
	[ITEM_BLASTOISINITE] = ,
	[ITEM_BEEDRILLITE] = ,
	[ITEM_PIDGEOTITE] = ,
	[ITEM_ALAKAZITE] = ,
	[ITEM_SLOWBRONITE] = ,
	[ITEM_GENGARITE] = ,
	[ITEM_KANGASKHANITE] = ,
	[ITEM_PINSIRITE] = ,
	[ITEM_GYARADOSITE] = ,
	[ITEM_AERODACTYLITE] = ,
	[ITEM_MEWTWONITE_X] = ,
	[ITEM_MEWTWONITE_Y] = ,
	[ITEM_AMPHAROSITE] = ,
	[ITEM_STEELIXITE] = ,
	[ITEM_SCIZORITE] = ,
	[ITEM_HERACRONITE] = ,
	[ITEM_HOUNDOOMINITE] = ,
	[ITEM_TYRANITARITE] = ,
	[ITEM_SCEPTILITE] = ,
	[ITEM_BLAZIKENITE] = ,
	[ITEM_SWAMPERTITE] = ,
	[ITEM_GARDEVOIRITE] = ,
	[ITEM_SABLENITE] = ,
	[ITEM_MAWILITE] = ,
	[ITEM_AGGRONITE] = ,
	[ITEM_MEDICHAMITE] = ,
	[ITEM_MANECTITE] = ,
	[ITEM_SHARPEDONITE] = ,
	[ITEM_CAMERUPTITE] = ,
	[ITEM_ALTARIANITE] = ,
	[ITEM_BANETTITE] = ,
	[ITEM_ABSOLITE] = ,
	[ITEM_GLALITITE] = ,
	[ITEM_SALAMENCITE] = ,
	[ITEM_METAGROSSITE] = ,
	[ITEM_LATIASITE] = ,
	[ITEM_LATIOSITE] = ,
	[ITEM_LOPUNNITE] = ,
	[ITEM_GARCHOMPITE] = ,
	[ITEM_LUCARIONITE] = ,
	[ITEM_ABOMASITE] = ,
	[ITEM_GALLADITE] = ,
	[ITEM_AUDINITE] = ,
	[ITEM_DIANCITE] = ,
*/
};

static s8 CompareItemsByType(u16 item1, u16 item2)
{
	if (sItemsByType[item1] < sItemsByType[item2])
		return -1;
	else if (sItemsByType[item1] > sItemsByType[item2])
		return 1;
	
	return 0;
}

enum
{
	SORT_ALPHABETICALLY,
	SORT_BY_TYPE,
};

void SortItemsInBag(u8 pocket, u8 type)
{
	struct ItemSlot* itemMem;
	u16 itemAmount;
	s8 (*func)(u16, u16);

	switch (pocket) {
		case 0:
			itemMem = sBagPocketArrangement.itemRam;
			itemAmount = sBagPocketArrangement.itemAmount;
			break;
		case 1:
			itemMem = sBagPocketArrangement.keyItemRam;
			itemAmount = sBagPocketArrangement.keyItemAmount;
			break;		
		default:
			itemMem = sBagPocketArrangement.pokeBallRam;
			itemAmount = sBagPocketArrangement.pokeBallAmount;	
	}
	
	switch (type) {
		case SORT_ALPHABETICALLY:
			func = CompareItemsAlphabetically;
			break;
		default:
			func = CompareItemsByType;
	}
	
	MergeSort(itemMem, 0, itemAmount - 1, func);
}

static void MergeSort(struct ItemSlot* array, u32 low, u32 high, s8 (*comparator)(u16, u16))
{
	if (high <= low)
		return;

	u32 mid = low + (high - low) / 2;
	MergeSort(array, low, mid, comparator); //Sort left half.
	MergeSort(array, mid + 1, high, comparator); //Sort right half.
	Merge(array, low, mid, high, comparator); //Merge results.
}

static void Merge(struct ItemSlot* array, u32 low, u32 mid, u32 high, s8 (*comparator)(u16, u16))
{
	u32 i = low;
	u32 j = mid + 1;
	struct ItemSlot aux[high + 1];
	
	for (u32 k = low; k <= high; ++k)
		aux[k] = array[k];
		
	for (u32 k = low; k <= high; ++k)
	{ //Merge back to a[low..high]
		if (i > mid)
			array[k] = aux[j++];
		else if (j > high)
			array[k] = aux[i++];
		else if (comparator(aux[j].itemId, aux[i].itemId) < 0)
			array[k] = aux[j++];
		else
			array[k] = aux[i++];
	}
}