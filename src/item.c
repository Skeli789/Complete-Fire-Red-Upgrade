#include "defines.h"
#include "../include/item_menu.h"
#include "../include/list_menu.h"
#include "../include/menu.h"
#include "../include/menu_helpers.h"
#include "../include/money.h"
#include "../include/script.h"
#include "../include/shop.h"
#include "../include/string_util.h"
#include "../include/window.h"
#include "../include/constants/hold_effects.h"
#include "../include/constants/items.h"
#include "../include/constants/moves.h"
#include "../include/constants/pokedex.h"
#include "../include/constants/songs.h"
#include "../include/constants/tutors.h"

#include "../include/new/util.h"
#include "../include/new/item.h"
#include "../include/new/set_z_effect.h"
/*
item.c
	handles all item related functions, such as returning hold effects, tm/hm expansion, etc.
*/

#define EOS 0xFF

struct BagMenuAlloc
{
	MainCallback exitCB;
	u8 itemOriginalLocation;
	u8 pocketSwitchMode:4;
	u8 itemMenuIcon:2;
	u8 inhibitItemDescriptionPrint:2;
	u16 contextMenuSelectedItem;
	u8 pocketScrollArrowsTask;
	u8 pocketSwitchArrowsTask;
	u8 nItems[3];
	u8 maxShowed[3];
	u8 data[4];
};

extern const u8 gMoveNames[][MOVE_NAME_LENGTH + 1];
extern const u8 gText_ThrowInOnePremierBall[];
extern const u8 gText_ThrowInPremierBalls[];
extern const u16 gItemsByType[];
extern struct BagMenuAlloc* sBagMenuDisplay; //0x203AD10
extern const u8* sBagContextMenuItemsPtr; //0x203AD24
extern u8 sBagContextMenuNumItems; //0x203AD28
extern u8 sItemDescriptionPocket; //0x203E053

//This file's functions:
static void StringAppendFullMoveName(u8* dst, const u8* src);

static s8 CompareBerries(struct ItemSlot* itemSlot1, struct ItemSlot* itemSlot2);
static s8 CompareTMs(struct ItemSlot* itemSlot1, struct ItemSlot* itemSlot2);
static s8 CompareItemsAlphabetically(struct ItemSlot* itemSlot1, struct ItemSlot* itemSlot2);
static s8 CompareItemsByType(struct ItemSlot* itemSlot1, struct ItemSlot* itemSlot2);
static s8 CompareItemsByMost(struct ItemSlot* itemSlot1, struct ItemSlot* itemSlot2);
static s8 CompareItemsByLeast(struct ItemSlot* itemSlot1, struct ItemSlot* itemSlot2);
static s8 CompareItemsByHavingValue(struct ItemSlot* itemSlot1, struct ItemSlot* itemSlot2);
void MergeSort(struct ItemSlot* array, u32 low, u32 high, s8 (*comparator)(struct ItemSlot*, struct ItemSlot*));
void Merge(struct ItemSlot* arr, u32 l, u32 m, u32 r, s8 (*comparator)(struct ItemSlot*, struct ItemSlot*));
static void BagMenu_SortByName(u8 taskId);
static void BagMenu_SortByType(u8 taskId);
static void BagMenu_SortByMost(u8 taskId);
static void BagMenu_SortByLeast(u8 taskId);
static void BagMenu_SortByAmount(u8 taskId);
static void BagMenu_ConfirmSort(u8 taskId);
static void BagMenu_CancelSort(u8 taskId);
static void Task_SortFinish(u8 taskId);
static void FinishBagSortIntro(u8 taskId);

//General Utility Functions
u16 SanitizeItemId(u16 itemId)
{
	if (itemId >= ITEMS_COUNT)
		return ITEM_NONE;

	return itemId;
}

const u8* ItemId_GetName(u16 itemId)
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

bool8 IsMegaStone(u16 item)
{
	return ItemId_GetHoldEffect(item) == ITEM_EFFECT_MEGA_STONE;
}

bool8 IsPrimalOrb(u16 item)
{
	return ItemId_GetHoldEffect(item) == ITEM_EFFECT_PRIMAL_ORB;
}

bool8 IsZCrystal(u16 item)
{
	return ItemId_GetHoldEffect(item) == ITEM_EFFECT_Z_CRYSTAL;
}

bool8 IsTypeZCrystal(u16 item, u8 moveType)
{
	return IsZCrystal(item) && ItemId_GetHoldEffectParam(item) == moveType && !IsSpecialZCrystal(item);
}

bool8 IsBerry(u16 item)
{
	return GetPocketByItemId(item) == POCKET_BERRY_POUCH;
}

bool8 IsPinchBerryItemEffect(u8 itemEffect)
{
	switch (itemEffect) {
		case ITEM_EFFECT_ATTACK_UP:
		case ITEM_EFFECT_DEFENSE_UP:
		case ITEM_EFFECT_SPEED_UP:
		case ITEM_EFFECT_SP_ATTACK_UP:
		case ITEM_EFFECT_SP_DEFENSE_UP:
		case ITEM_EFFECT_CRITICAL_UP:
		case ITEM_EFFECT_RANDOM_STAT_UP:
		case ITEM_EFFECT_CUSTAP_BERRY:
		case ITEM_EFFECT_MICLE_BERRY:
			return TRUE;
	}

	return FALSE;
}

bool8 IsGem(u16 item)
{
	return ItemId_GetHoldEffect(item) == ITEM_EFFECT_GEM;
}

bool8 IsTMHM(u16 item)
{
	return GetPocketByItemId(item) == POCKET_TM_CASE;
}

u8 TMIdFromItemId(u16 itemId)
{
	#ifdef EXPANDED_TMSHMS
	u8 tmNum = ItemId_GetMystery2(itemId);

	if (itemId == ITEM_NONE)
		return 255; //So blank items get put at the end
	else if (tmNum == 0)
		return itemId - ITEM_TM01_FOCUS_PUNCH;
	else
		return tmNum-1;
	#else
		return itemId - ITEM_TM01_FOCUS_PUNCH;
	#endif
}

u8 BerryIdFromItemId(u16 item)
{
	u8 secondaryId = ItemId_GetMystery2(item);
	if (secondaryId != 0)
		return secondaryId;

	if (item < ITEM_CHERI_BERRY
	||  item >= ITEM_ENIGMA_BERRY_OLD)
		return 255;

	return item - ITEM_CHERI_BERRY;
}

bool8 GetSetItemObtained(u16 item, u8 caseId)
{
	u8 index;
	u8 bit;
	u8 mask;
	
	index = item / 8;
	bit = item % 8;
	mask = 1 << bit;
	switch (caseId)
	{
		case FLAG_GET_OBTAINED:
			return gSaveBlock1->itemObtainedFlags[index] & mask;
		case FLAG_SET_OBTAINED:
			gSaveBlock1->itemObtainedFlags[index] |= mask;
	}
 
	return FALSE;
}

u8 ReformatItemDescription(u16 itemId, u8* dest, u8 maxWidth)
{
	//Requires dest be memset to 0xFF before calling!
	u8 count = 0;
	u8 k = 0;
	u8 numLines = 1;
	u8 buffer[150];
	u8* desc;
	u8* lineStart;
	
	desc = ItemId_GetDescription(itemId);
	StringExpandPlaceholders(buffer, desc);
	lineStart = dest;

	while (buffer[k] != EOS)
	{
		if (GetStringWidth(0, lineStart, 0) >= maxWidth)
		{
			do
			{
				//Go to end of previous word
				dest--;
				k--;
			} while (buffer[k] != CHAR_SPACE && buffer[k] != CHAR_NEWLINE);

			if (buffer[k + 1] != EOS) //String will continue on another line
			{
				*dest = CHAR_NEWLINE;
				numLines++;
			}

			count = 0;
			dest++;
			k++;
			lineStart = dest;
			continue;
		}

		*dest = buffer[k];
		if (buffer[k] == CHAR_NEWLINE)
		{
			if (buffer[k - 1] != CHAR_SPACE) //Don't double space
				*dest = CHAR_SPACE;
			else
				dest--; //Don't change curr position
		}

		dest++;
		k++;
		count++;
	}

	// finish string
	*dest = EOS;

	return numLines;
}

//////////////////////////TM + HMs////////////////////////////////////////////////
#ifdef EXPANDED_TMSHMS
	typedef u32 TM_HM_T[4]; //extern const u32 gTMHMLearnsets[NUM_SPECIES][4];
#else
	typedef u32 TM_HM_T[2]; //extern const u32 gTMHMLearnsets[NUM_SPECIES][2];
#endif

#if (NUM_MOVE_TUTORS > 64)
	typedef u32 ExpandedTutor_T[4]; //extern const u32 gTutorMoves[NUM_SPECIES][4];
#else
	typedef u32 ExpandedTutor_T[2]; //extern const u32 gTutorMoves[NUM_SPECIES][2];
#endif

#define gTMHMLearnsets ((TM_HM_T*) *((u32*) 0x8043C68))
#define gTutorLearnsets ((ExpandedTutor_T*) *((u32*) 0x8120C30))

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
		return (gTMHMLearnsets[species][0] & mask) != 0 ? TRUE : FALSE;
	}
	else if (tm >= 32 && tm < 64)
	{
		mask = 1 << (tm - 32);
		return (gTMHMLearnsets[species][1] & mask) != 0 ? TRUE : FALSE;
	}
	#ifdef EXPANDED_TMSHMS
	else if (tm >= 64 && tm < 96)
	{
		mask = 1 << (tm - 64);
		return (gTMHMLearnsets[species][2] & mask) != 0 ? TRUE : FALSE;
	}
	else if (tm >= 96 && tm < 128)
	{
		mask = 1 << (tm - 96);
		return (gTMHMLearnsets[species][3] & mask) != 0 ? TRUE : FALSE;
	}
	#endif
	else
		return 0;
}

#ifdef EXPANDED_MOVE_TUTORS

bool8 CanMonLearnTutorMove(struct Pokemon* mon, u8 tutorId)
{
	u32 mask;
	u16 species = GetMonData(mon, MON_DATA_SPECIES, NULL);

	if (tutorId < NUM_MOVE_TUTORS)
	{
		if (tutorId < 32)
		{
			mask = 1 << tutorId;
			return (gTutorLearnsets[species][0] & mask) != 0 ? TRUE : FALSE;
		}
		else if (tutorId >= 32 && tutorId < 64)
		{
			mask = 1 << (tutorId - 32);
			return (gTutorLearnsets[species][1] & mask) != 0 ? TRUE : FALSE;
		}
	#if (NUM_MOVE_TUTORS > 64)
		else if (tutorId >= 64 && tutorId < 96)
		{
			mask = 1 << (tutorId - 64);
			return (gTutorLearnsets[species][2] & mask) != 0 ? TRUE : FALSE;
		}
		else if (tutorId >= 96 && tutorId < 128)
		{
			mask = 1 << (tutorId - 96);
			return (gTutorLearnsets[species][3] & mask) != 0 ? TRUE : FALSE;
		}
	#endif
	}

	//Special move tutors not stored in a table
	u16 dexNum = SpeciesToNationalPokedexNum(species);
	switch (tutorId) {
		case TUTOR_SPECIAL_DRACO_METEOR:
			return GetMonData(mon, MON_DATA_FRIENDSHIP, NULL) >= MAX_FRIENDSHIP
				&& (gBaseStats[species].type1 == TYPE_DRAGON
				 || gBaseStats[species].type2 == TYPE_DRAGON);
		#ifdef NATIONAL_DEX_KELDEO
		case TUTOR_SPECIAL_SECRET_SWORD:
			return dexNum == NATIONAL_DEX_KELDEO;
		#endif
		#ifdef NATIONAL_DEX_MELOETTA
		case TUTOR_SPECIAL_RELIC_SONG:
			return dexNum == NATIONAL_DEX_MELOETTA;
		#endif
		#ifdef NATIONAL_DEX_PIKACHU
		case TUTOR_SPECIAL_VOLT_TACKLE:
			return dexNum == NATIONAL_DEX_PIKACHU;
		#endif
		#ifdef NATIONAL_DEX_RAYQUAZA
		case TUTOR_SPECIAL_DRAGON_ASCENT:
			return dexNum == NATIONAL_DEX_RAYQUAZA;
		#endif
		#ifdef NATIONAL_DEX_ZYGARDE
		case TUTOR_SPECIAL_THOUSAND_ARROWS:
		case TUTOR_SPECIAL_THOUSAND_WAVES:
		case TUTOR_SPECIAL_CORE_ENFORCER:
			return dexNum == NATIONAL_DEX_ZYGARDE;
		#endif
		case TUTOR_SPECIAL_STEEL_BEAM:
			return gBaseStats[species].type1 == TYPE_STEEL
				|| gBaseStats[species].type2 == TYPE_STEEL
			#ifdef NATIONAL_DEX_ZACIAN
				|| dexNum == NATIONAL_DEX_ZACIAN
			#endif
			#ifdef NATIONAL_DEX_ZAMAZENTA
				|| dexNum == NATIONAL_DEX_ZAMAZENTA
			#endif
				;
	}

	return FALSE;
}

u16 GetExpandedTutorMove(u8 tutorId)
{
	switch (tutorId) {
		case TUTOR_SPECIAL_DRACO_METEOR:
			return MOVE_DRACOMETEOR;
		case TUTOR_SPECIAL_SECRET_SWORD:
			return MOVE_SECRETSWORD;
		case TUTOR_SPECIAL_RELIC_SONG:
			return MOVE_RELICSONG;
		case TUTOR_SPECIAL_VOLT_TACKLE:
			return MOVE_VOLTTACKLE;
		case TUTOR_SPECIAL_DRAGON_ASCENT:
			return MOVE_DRAGONASCENT;
		case TUTOR_SPECIAL_THOUSAND_ARROWS:
			return MOVE_THOUSANDARROWS;
		case TUTOR_SPECIAL_THOUSAND_WAVES:
			return MOVE_THOUSANDWAVES;
		case TUTOR_SPECIAL_CORE_ENFORCER:
			return MOVE_COREENFORCER;
		case TUTOR_SPECIAL_STEEL_BEAM:
			return MOVE_STEELBEAM;
		default:
			if (tutorId >= NUM_MOVE_TUTORS)
				return MOVE_NONE;
			else
				return gTutorMoves[tutorId];
	}
}

#endif

u8 TryHandleExcuseForDracoMeteorTutor(unusedArg struct Pokemon* mon)
{
#ifdef EXPANDED_MOVE_TUTORS
	u8 tutorId = Var8005;

	if (tutorId == TUTOR_SPECIAL_DRACO_METEOR)
	{
		u16 species = GetMonData(mon, MON_DATA_SPECIES, NULL);
		Var8005 = 1;

		if (gBaseStats[species].type1 != TYPE_DRAGON
		&&  gBaseStats[species].type2 != TYPE_DRAGON)
			Var8005 = 3;
		else if (GetMonData(mon, MON_DATA_FRIENDSHIP, NULL) < MAX_FRIENDSHIP) //Dragon-type not at max friendship
			Var8005 = 5;

		return 1;
	}
#endif

	return 0;
}

u8 TryHandleExcuseForDracoMeteorTutorAlreadyKnow(void)
{
#ifdef EXPANDED_MOVE_TUTORS
	u8 tutorId = Var8005;

	if (tutorId == TUTOR_SPECIAL_DRACO_METEOR)
	{
		Var8005 = 2;
		return 1;
	}
#endif

	return 0;
}

void CancelPartyMenuLearnTutor(u8 taskId)
{
	gTasks[taskId].func = (void*) 0x81255BD;
}

// item ID to Tm number to Move ID
//		use secondary ID for tm indices if expanded
u16 ItemIdToBattleMoveId(u16 item)
{
	return gTMHMMoves[TMIdFromItemId(item)];
}

//Function to fix tm move names that are full length in the bag
static void StringAppendFullMoveName(u8* dst, const u8* src)
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

void CopyTMName(u8* dst, u16 itemId)
{
	StringCopy(dst, gMoveNames[ItemIdToBattleMoveId(itemId)]);
}

void LoadTMNameWithNo(u8* dst, u16 itemId)
{
	#ifdef EXPANDED_TMSHMS
	u8 tmNum = ItemId_GetMystery2(itemId);
	#else
	u8 tmNum = (itemId - ITEM_TM01_FOCUS_PUNCH) + 1;
	#endif

	StringCopy(gStringVar4, (void*) 0x84166FF);

	if (tmNum > NUM_TMS)
	{
		// HM
		u8 hmNum = tmNum - NUM_TMS;
		StringAppend(gStringVar4, (void*) 0x8463178);
		StringAppend(gStringVar4, (void*) 0x8416226);
		if (NUM_HMS < 10)
			ConvertIntToDecimalStringN(&gStringVar1[0], hmNum, 2, 1);
		else
			ConvertIntToDecimalStringN(&gStringVar1[0], hmNum, 2, 2);
	}
	else
	{
		// TM
		StringAppend(gStringVar4, (void*) 0x8416226);
		if (NUM_TMS < 100)
			ConvertIntToDecimalStringN(gStringVar1, tmNum, 2, 2);
		else
			ConvertIntToDecimalStringN(gStringVar1, tmNum, 2, 3);
	}

	StringAppend(gStringVar4, gStringVar1);
	StringAppend(gStringVar4, (void*) 0x846317C);
	StringAppend(gStringVar4, (void*) 0x8416703);

	if (StringLength(gMoveNames[ItemIdToBattleMoveId(itemId)]) == MOVE_NAME_LENGTH && tmNum > NUM_TMS)
		StringAppendFullMoveName(gStringVar4, gMoveNames[ItemIdToBattleMoveId(itemId)]);
	else
		StringAppend(gStringVar4, gMoveNames[ItemIdToBattleMoveId(itemId)]);

	StringCopy(dst, gStringVar4);
}


// Assumes no HMs will be in the mart...
void LoadTmHmNameInMart(u16 item)
{
	u8 tmNum;

	#ifdef EXPANDED_TMSHMS
		tmNum = ItemId_GetMystery2(item);
		if (NUM_TMS < 100)
			ConvertIntToDecimalStringN(&gStringVar1[0], tmNum, 2, 2);
		else
			ConvertIntToDecimalStringN(&gStringVar1[0], tmNum, 2, 3);
	#else
		tmNum = (item - ITEM_TM01_FOCUS_PUNCH) + 1;
		ConvertIntToDecimalStringN(&gStringVar1[0], tmNum, 2, 2);
	#endif
}

extern u8* gMoveDescriptions[];
void* LoadTmHmMartDescription(u16 item)
{
	if (GetPocketByItemId(item) == POCKET_TM_HM)
		return gMoveDescriptions[ItemIdToBattleMoveId(item)-1];
	else
		return ItemId_GetDescription(item);
}

u8 CanMonLearnTMTutor(struct Pokemon* mon, u16 item, u8 tutor)
{
	u16 move;

	if (GetMonData(mon, MON_DATA_IS_EGG, NULL))
		return CANNOT_LEARN_MOVE_IS_EGG;

	//if (item >= ITEM_TM01_FOCUS_PUNCH)
	if (GetPocketByItemId(item) == POCKET_TM_HM)
	{
		//if (CanMonLearnTMHM(mon, item - ITEM_TM01_FOCUS_PUNCH))
		if (CanMonLearnTMHM(mon, TMIdFromItemId(item)))
			move = ItemIdToBattleMoveId(item);
		else
			return CANNOT_LEARN_MOVE;
		//do {} while (0); // :morphon:
	}
	#ifdef EXPANDED_MOVE_TUTORS
	else if (!CanMonLearnTutorMove(mon, tutor))
	{
		return CANNOT_LEARN_MOVE;
	}
	else
	{
		move = GetExpandedTutorMove(tutor);
	}
	#else
	else if (!CanLearnTutorMove(mon->species, tutor))
	{
		return CANNOT_LEARN_MOVE;
	}
	else
	{
		move = GetTutorMove(tutor);
	}
	#endif

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
		++move; //So compiler doesn't complain
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


bool8 CheckTmHmInFront(u16 item)
{
	#ifdef TMS_BEFORE_HMS
	if (TMIdFromItemId(item) > NUM_TMS)
		return TRUE;
	#else
	if (TMIdFromItemId(item) < NUM_TMS)
		return TRUE;
	#endif
	return FALSE;
}


u8 CheckDiscIsTmHm(struct Sprite* disc, u16 itemId)
{
	#ifdef EXPANDED_TMSHMS
	if (TMIdFromItemId(itemId) >= NUM_TMS)
		StartSpriteAnim(disc, 1);
	else
		StartSpriteAnim(disc, 0);
	#else
	if (itemId <= ITEM_TM50_OVERHEAT)
		StartSpriteAnim(disc, 0);
	else
		StartSpriteAnim(disc, 1);
	#endif

	return ItemId_GetMystery2(itemId);
}


u8 TmHMDiscPosition(unusedArg struct Sprite* disc, u8 tmId)
{
	u8 num;
	if (tmId <= NUM_TMS)
		num = tmId + 8;
	else
		num = tmId - NUM_TMS;

	#ifdef EXPANDED_TMSHMS
		num /= 2;
	#endif

	return num;
}


bool8 CheckReusableTMs(u16 item)
{
	#ifdef REUSABLE_TMS
		if (TMIdFromItemId(item) != 0)
			return TRUE;
		else
			return FALSE;
	#else
		if (item > ITEM_TM50_OVERHEAT)
			return TRUE;
		else
			return FALSE;
	#endif
}


u8 CheckHmSymbol(u16 item)
{
	#ifdef EXPANDED_TMSHMS
		#ifdef REUSABLE_TMS
			if (TMIdFromItemId(item) < NUM_TMS)
				return 2;	//no HM symbol or quantity
			else
				return 0;	//HM symbol, no quantity
		#else
			if (TMIdFromItemId(item) < NUM_TMS)
				return 1;	//no HM symbol, show quantity
			else
				return 0;	//HM symbol, no qty
		#endif
	#else
		#ifdef REUSABLE_TMS
			if (ItemIsUnique(item) > 0)
				return 0;	//HM symbol, no quantity
			else
				return 2;	//no HM symbol or quantity
		#else
			if (ItemIsUnique(item) > 0)
				return 0;	//HM symbol, no quantity
			else
				return 1;	//no HM symbol, show qty
		#endif
	#endif
}


bool8 CheckSellTmHm(u16 item)
{
	#ifdef REUSABLE_TMS
		if (GetPocketByItemId(item) == POCKET_TM_HM)
			return FALSE;
		else
		{
			if (ItemId_GetPrice(item) == 0)
				return FALSE;
			else
				return TRUE;
		}
	#else
		if (ItemId_GetPrice(item) == 0)
			return FALSE;
		else
			return TRUE;
	#endif
}



extern const u8 gText_SingleTmBuy[];
void CheckTmPurchase(u16 item, u8 taskId)
{
	CopyItemName(item, &gStringVar1[0]);
	#ifdef REUSABLE_TMS
		if (GetPocketByItemId(item) == POCKET_TM_HM)
		{
			ConvertIntToDecimalStringN(&gStringVar2[0], ItemId_GetPrice(item), 3, 8);
			StringCopy(&gStringVar3[0], gMoveNames[ItemIdToBattleMoveId(item)]);
			BuyMenuDisplayMessage(taskId, &gText_SingleTmBuy[0], BuyMenuConfirmPurchase);
		}
		else
			BuyMenuDisplayMessage(taskId, (void*) 0x8416766, Task_BuyHowManyDialogueInit);
	#else
		BuyMenuDisplayMessage(taskId, (void*) 0x8416766, Task_BuyHowManyDialogueInit);
	#endif
}


extern const u8 gText_AlreadyOwnTM[];
bool8 CheckBuyableTm(u16 item, u8 taskId)
{
	#ifdef REUSABLE_TMS
		if (GetPocketByItemId(item) == POCKET_TM_HM && CheckBagHasItem(item, 1))
		{
			BuyMenuDisplayMessage(taskId, &gText_AlreadyOwnTM[0], BuyMenuReturnToItemList);
			return TRUE;
		}
		else
		{
			u32 price = ItemId_GetPrice(item);
			gShopDataPtr->itemPrice = price;
			if (IsEnoughMoney(&gSaveBlock1->money, price))
				return FALSE;
			else
			{
				BuyMenuDisplayMessage(taskId, (void*) 0x8416842, BuyMenuReturnToItemList);
				return TRUE;
			}
		}
	#else
		u32 price = ItemId_GetPrice(item);
		gShopDataPtr->itemPrice = price;
		if (IsEnoughMoney(&gSaveBlock1->money, price))
			return FALSE;
		else
		{
			BuyMenuDisplayMessage(taskId, (void*) 0x8416842, BuyMenuReturnToItemList);
			return TRUE;
		}
	#endif
}

extern const u8 gText_Purchased[];
void PrintTmPriceOrPurchased(u8 windowId, u16 item, u8 y)
{
	s32 x;
	u8 *loc;
	
	#ifdef REUSABLE_TMS
		if (GetPocketByItemId(item) == POCKET_TM_CASE && CheckBagHasItem(item, 1))
		{
			BuyMenuPrint(windowId, 0, gText_Purchased, 0x58, y, 0, 0, 0xFF, 1);
			return;
		}
	#endif
	
	ConvertIntToDecimalStringN(gStringVar1, ItemId_GetPrice(item), 0, 5);
	x = 5 - StringLength(gStringVar1);
	loc = gStringVar4;
	while (x-- != 0)
		*loc++ = 0;
	
	StringExpandPlaceholders(loc, (void*) 0x841697A);
	BuyMenuPrint(windowId, 0, gStringVar4, 0x66, y, 0, 0, 0xFF, 1);
}

u8 CheckSingleBagTm(unusedArg u16 item)
{
	#ifdef REUSABLE_TMS
		if (GetPocketByItemId(item) == POCKET_TM_HM)
			return 1;
		else
			return 0xFF;
	#else
		return 0xFF;
	#endif
}

extern const u16 Fairy_TM_DiskPal[];
const void* FixTmHmDiscPalette(u8 type)
{
	if (type == TYPE_FAIRY)
		return &Fairy_TM_DiskPal[0];
	else
		return 0;
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
		if (GetPocketByItemId(tItemId) == POCKET_POKE_BALLS)
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

#define tItemCount data[1]
#define tListTaskId data[7]
void Task_ReturnToSellListAfterTmPurchase(u8 taskId)
{
	s16 *data = gTasks[taskId].data;

	if (gMain.newKeys & (A_BUTTON | B_BUTTON))
	{
		IncrementGameStat(GAME_STAT_SHOPPED);
		RemoveMoney(&gSaveBlock1->money, gShopDataPtr->itemPrice);
		PlaySE(SE_MONEY);
		PrintMoneyAmountInMoneyBox(0, GetMoney(&gSaveBlock1->money), 0);
		RedrawListMenu(tListTaskId);
		BuyMenuReturnToItemList(taskId);
	}
}

void ReloadMartListForTmPurchase(u8 taskId)
{	
	#ifdef REUSABLE_TMS
	s16 *data = gTasks[taskId].data;

	if (GetPocketByItemId(tItemId) == POCKET_TM_CASE)
		BuyMenuDisplayMessage(taskId, (void*) 0x84167E7, Task_ReturnToSellListAfterTmPurchase);
	else
	#endif
		BuyMenuDisplayMessage(taskId, (void*) 0x84167E7, (TaskFunc) (0x809BF0C | 1)); //BuyMenuSubtractMoney
}
#undef tItemId

//Multiple Registered Items////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool8 IsItemRegistered(u16 item)
{
	u32 i;

	for (i = 0; i < REGISTERED_ITEM_COUNT; ++i)
	{
		if (gSaveBlock1->registeredItems[i] == item)
			return TRUE;
	}

	return FALSE;
}

bool8 CanRegisterNewItem(void)
{
	u32 i;

	for (i = 0; i < REGISTERED_ITEM_COUNT; ++i)
	{
		if (gSaveBlock1->registeredItems[i] == ITEM_NONE)
			return TRUE;
	}

	return FALSE;
}

void RegisterItem(u16 item)
{
	u32 i;

	for (i = 0; i < REGISTERED_ITEM_COUNT; ++i)
	{
		if (gSaveBlock1->registeredItems[i] == ITEM_NONE)
		{
			gSaveBlock1->registeredItems[i] = item;
			break;
		}
	}
}

void RemoveRegisteredItem(u16 item)
{
	u32 i;

	for (i = 0; i < REGISTERED_ITEM_COUNT; ++i)
	{
		if (gSaveBlock1->registeredItems[i] == item)
		{
			gSaveBlock1->registeredItems[i] = ITEM_NONE;
			CompactRegisteredItems();
			break;
		}
	}
}

void CompactRegisteredItems(void)
{
	u32 i, j;
 
	//Bubble sort because it doesn't need to be quick
	for (i = 0; i < REGISTERED_ITEM_COUNT - 1; ++i)       
	{
	   // Last i elements are already in place    
		for (j = 0; j < REGISTERED_ITEM_COUNT - i - 1; ++j)
		{
			if (gSaveBlock1->registeredItems[j] == ITEM_NONE && gSaveBlock1->registeredItems[j + 1] != ITEM_NONE)
			{
			    u16 temp = gSaveBlock1->registeredItems[j + 1];
				gSaveBlock1->registeredItems[j + 1] = gSaveBlock1->registeredItems[j];
				gSaveBlock1->registeredItems[j] = temp;
			}
		}
	}
}

void HandleItemRegistration(u16 item)
{
	if (IsItemRegistered(item))
		RemoveRegisteredItem(item); //Toggle off
	else
	{
		if (!CanRegisterNewItem())
			RemoveRegisteredItem(gSaveBlock1->registeredItems[0]); //Remove the oldest item to make room for a new one

		RegisterItem(item);
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

struct BagSlots
{
    struct ItemSlot bagPocket_Items[NUM_REGULAR_ITEMS];
    struct ItemSlot bagPocket_KeyItems[NUM_KEY_ITEMS];
    struct ItemSlot bagPocket_PokeBalls[NUM_POKE_BALLS];
    u16 itemsAbove[3];
    u16 cursorPos[3];
    u16 registeredItems[NELEMS(gSaveBlock1->registeredItems)];
    u16 pocket;
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
	sItemDescriptionPocket = 0; //Reset item description printer
	sListBuffer1 = Malloc(sizeof(struct ListBuffer1));
	sListBuffer2 = Malloc(sizeof(struct ListBuffer2));
}

extern struct ListMenuItem* sBerryPouchListMenuItems;
bool8 AllocateBerryPouchListBuffers(void)
{
	sBerryPouchListMenuItems = Malloc(NUM_BERRIES * sizeof(struct ListMenuItem));
	if (sBerryPouchListMenuItems == NULL)
		return FALSE;

	return TRUE;
}

extern struct BagSlots* sBackupPlayerBag;
void BackupPlayerBag(void)
{
	u32 i;

	sBackupPlayerBag = Calloc(sizeof(struct BagSlots));
	Memcpy(sBackupPlayerBag->bagPocket_Items, sBagRegularItems, NUM_REGULAR_ITEMS * sizeof(struct ItemSlot));
	Memcpy(sBackupPlayerBag->bagPocket_KeyItems, sBagKeyItems, NUM_KEY_ITEMS * sizeof(struct ItemSlot));
	Memcpy(sBackupPlayerBag->bagPocket_PokeBalls, sBagPokeBalls, NUM_POKE_BALLS * sizeof(struct ItemSlot));
	Memcpy(sBackupPlayerBag->registeredItems, gSaveBlock1->registeredItems, sizeof(gSaveBlock1->registeredItems));
	sBackupPlayerBag->pocket = gBagMenuState.pocket;
	for (i = 0; i < 3; i++)
	{
		sBackupPlayerBag->itemsAbove[i] = gBagMenuState.scrollPosition[i];
		sBackupPlayerBag->cursorPos[i] = gBagMenuState.cursorPosition[i];
	}

	Memset(sBagRegularItems, 0, sizeof(struct ItemSlot) * NUM_REGULAR_ITEMS); //Too many to use ClearItemSlots
	ClearItemSlots(sBagKeyItems, NUM_KEY_ITEMS);
	ClearItemSlots(sBagPokeBalls, NUM_POKE_BALLS);
	Memset(gSaveBlock1->registeredItems, 0, sizeof(gSaveBlock1->registeredItems));
	ResetBagCursorPositions();
}

void RestorePlayerBag(void)
{
	u32 i;

	Memcpy(sBagRegularItems, sBackupPlayerBag->bagPocket_Items, NUM_REGULAR_ITEMS * sizeof(struct ItemSlot));
	Memcpy(sBagKeyItems, sBackupPlayerBag->bagPocket_KeyItems, NUM_KEY_ITEMS * sizeof(struct ItemSlot));
	Memcpy(sBagPokeBalls, sBackupPlayerBag->bagPocket_PokeBalls, NUM_POKE_BALLS * sizeof(struct ItemSlot));
	Memcpy(gSaveBlock1->registeredItems, sBackupPlayerBag->registeredItems, sizeof(gSaveBlock1->registeredItems));
	gBagMenuState.pocket = sBackupPlayerBag->pocket;
	for (i = 0; i < 3; i++)
	{
		gBagMenuState.scrollPosition[i] = sBackupPlayerBag->itemsAbove[i];
		gBagMenuState.cursorPosition[i] = sBackupPlayerBag->cursorPos[i];
	}

	Free(sBackupPlayerBag);
}

#define sPokeDudeItemBackupPtr2 (*((struct ItemSlot***) 0x203B11C))
void PokeDudeBackupKeyItemsTMs(void)
{
	struct ItemSlot* ptr = Calloc((NUM_KEY_ITEMS + sBagPocketArrangement.tmAmount + 1) * sizeof(struct ItemSlot));
	sPokeDudeItemBackupPtr2 = Calloc(0x164);
	*sPokeDudeItemBackupPtr2 = ptr;

	Memcpy(ptr, sBagPocketArrangement.keyItemRam, NUM_KEY_ITEMS * sizeof(struct ItemSlot));
	Memcpy(ptr + NUM_KEY_ITEMS, sBagPocketArrangement.tmRam, sBagPocketArrangement.tmAmount * sizeof(struct ItemSlot));

	Memset(sBagPocketArrangement.keyItemRam, 0, NUM_KEY_ITEMS * sizeof(struct ItemSlot));
	Memset(sBagPocketArrangement.tmRam, 0, sBagPocketArrangement.tmAmount * sizeof(struct ItemSlot));
}

void PokeDudeRestoreKeyItemsTMs(void)
{
	struct ItemSlot* ptr = *sPokeDudeItemBackupPtr2;
	Memcpy(sBagPocketArrangement.keyItemRam, ptr, NUM_KEY_ITEMS * sizeof(struct ItemSlot));
	Memcpy(sBagPocketArrangement.tmRam, ptr + NUM_KEY_ITEMS, sBagPocketArrangement.tmAmount * sizeof(struct ItemSlot));

	Free(ptr);
}

void StoreBagItemCount(void)
{
	struct ItemSlot* itemMem;
	u16 itemAmount;
	u16 checkLength;
	bool8 tossed;

	for (u8 pocket = 0; pocket < 3; ++pocket)
	{
		tossed = FALSE;
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

void CompactItemsInBagPocket(struct ItemSlot* itemSlots, u16 amount)
{
	MergeSort(itemSlots, 0, amount - 1, CompareItemsByHavingValue); //Sort all the null items to the back
}


//Functions For Sorting Bag/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define gMenuText_Use (const u8*) 0x84161a0
#define gMenuText_Toss (const u8*) 0x84161a4
#define gMenuText_Register (const u8*) 0x84161a9
#define gMenuText_Give (const u8*) 0x84161b2
#define gText_Cancel2 (const u8*) 0x84161c1
#define gMenuText_Check (const u8*) 0x84161e9
#define gMenuText_Open (const u8*) 0x84161f4
#define gMenuText_Walk (const u8*) 0x84161bc
#define gMenuText_Deselect (const u8*) 0x84161f9
#define gText_EmptyString2 (const u8*) 0x84161cd

#define ItemMenu_UseOutOfBattle (void*) 0x8109c51
#define ItemMenu_Toss (void*) 0x8109cc1
#define ItemMenu_Register (void*) 0x810a001
#define ItemMenu_Give (void*) 0x810a0a9
#define ItemMenu_Cancel (void*) 0x810a2dd
#define ItemMenu_UseInBattle (void*) 0x810a325
#define ItemMenu_Register (void*) 0x810a001

extern const u8 gText_WantToSortItems[];
extern const u8 gMenuText_ByType[];
extern const u8 gMenuText_ByName[];
extern const u8 gMenuText_ByMost[];
extern const u8 gMenuText_ByLeast[];
extern const u8 gMenuText_ByAmount[];
extern const u8 gText_type[];
extern const u8 gText_name[];
extern const u8 gText_most[];
extern const u8 gText_least[];
extern const u8 gText_amount[];
extern const u8 gText_SortItemsBy[];
extern const u8 gText_ItemsSortedBy[];

enum
{
	SORT_ALPHABETICALLY,
	SORT_BY_TYPE,
	SORT_BY_LEAST,
	SORT_BY_MOST,
	SORT_BY_AMOUNT,
};

enum
{
	BAG_MENU_OPTION_CANCEL = 4,

	BAG_MENU_OPTION_BY_NAME = 12,
	BAG_MENU_OPTION_BY_TYPE,
	BAG_MENU_OPTION_BY_MOST,
	BAG_MENU_OPTION_BY_LEAST,
	BAG_MENU_OPTION_BY_AMOUNT,
};

const struct YesNoFuncTable sYesNoSortFunctions = {BagMenu_ConfirmSort, BagMenu_CancelSort};

const u8* const sSortTypeStrings[] =
{
	[SORT_ALPHABETICALLY] = gText_name,
	[SORT_BY_TYPE] = gText_type,
	[SORT_BY_LEAST] = gText_least,
	[SORT_BY_MOST] = gText_most,
	[SORT_BY_AMOUNT] = gText_amount,
};

const struct MenuAction sItemMenuActions[] =
{
	{gMenuText_Use, 		{ItemMenu_UseOutOfBattle}},
	{gMenuText_Toss, 		{ItemMenu_Toss}},
	{gMenuText_Register, 	{ItemMenu_Register}},
	{gMenuText_Give, 		{ItemMenu_Give}},
	{gText_Cancel2, 		{ItemMenu_Cancel}},
	{gMenuText_Use, 		{ItemMenu_UseInBattle}},
	{gMenuText_Check, 		{ItemMenu_UseOutOfBattle}},
	{gMenuText_Open, 		{ItemMenu_UseOutOfBattle}},
	{gMenuText_Open, 		{ItemMenu_UseInBattle}},
	{gMenuText_Walk, 		{ItemMenu_UseOutOfBattle}},
	{gMenuText_Deselect, 	{ItemMenu_Register}},
	{gText_EmptyString2, 	{NULL}},
	[BAG_MENU_OPTION_BY_NAME] = 	{gMenuText_ByName, 		{BagMenu_SortByName}},
	[BAG_MENU_OPTION_BY_TYPE] = 	{gMenuText_ByType, 		{BagMenu_SortByType}},
	[BAG_MENU_OPTION_BY_MOST] = 	{gMenuText_ByMost, 		{BagMenu_SortByMost}},
	[BAG_MENU_OPTION_BY_LEAST] =	{gMenuText_ByLeast, 	{BagMenu_SortByLeast}},
	[BAG_MENU_OPTION_BY_AMOUNT] =	{gMenuText_ByAmount, 	{BagMenu_SortByAmount}},
};

const u8 gBagMenuSortItems[] =
{
	BAG_MENU_OPTION_BY_NAME,
	BAG_MENU_OPTION_BY_TYPE,
	//BAG_MENU_OPTION_BY_MOST,
	//BAG_MENU_OPTION_BY_LEAST,
	BAG_MENU_OPTION_BY_AMOUNT,
	BAG_MENU_OPTION_CANCEL,
};

const u8 gBagMenuSortKeyItems[] =
{
	BAG_MENU_OPTION_BY_NAME,
	BAG_MENU_OPTION_CANCEL,
};

const u8 gBagMenuSortPokeBalls[] =
{
	BAG_MENU_OPTION_BY_NAME,
	//BAG_MENU_OPTION_BY_MOST,
	//BAG_MENU_OPTION_BY_LEAST,
	BAG_MENU_OPTION_BY_AMOUNT,
	BAG_MENU_OPTION_CANCEL,
};

static s8 CompareBerries(struct ItemSlot* itemSlot1, struct ItemSlot* itemSlot2)
{
	u8 id1 = BerryIdFromItemId(itemSlot1->itemId);
	u8 id2 = BerryIdFromItemId(itemSlot2->itemId);

	if (id1 < id2)
		return -1;
	else if (id1 > id2)
		return 1;

	return 0;
}

static s8 CompareTMs(struct ItemSlot* itemSlot1, struct ItemSlot* itemSlot2)
{
	u8 id1 = TMIdFromItemId(itemSlot1->itemId);
	u8 id2 = TMIdFromItemId(itemSlot2->itemId);

	#ifndef TMS_BEFORE_HMS
	if (id1 <= NUM_TMS && id2 > NUM_TMS)
		return -1;
	else if (id2 <= NUM_TMS && id1 > NUM_TMS)
		return 1;
	#else
	if (id2 <= NUM_TMS && id1 > NUM_TMS)
		return -1;
	else if (id1 <= NUM_TMS && id2 > NUM_TMS)
		return 1;
	#endif
	else if (id1 < id2)
		return -1;
	else if (id2 > id1)
		return 1;
	return 0;
}

void SortBerriesOrTMHMs(struct BagPocket* bagPocket)
{
	s8 (*func)(struct ItemSlot*, struct ItemSlot*);
	u16 itemAmount = bagPocket->capacity;

	if (bagPocket->itemSlots == sBagPocketArrangement.tmRam)
		func = CompareTMs;
	else
		func = CompareBerries;

	MergeSort(bagPocket->itemSlots, 0, itemAmount - 1, func);
}

static s8 CompareItemsAlphabetically(struct ItemSlot* itemSlot1, struct ItemSlot* itemSlot2)
{
	u16 item1 = itemSlot1->itemId;
	u16 item2 = itemSlot2->itemId;

	if (item1 == ITEM_NONE)
		return 1;
	else if (item2 == ITEM_NONE)
		return -1;

	const u8* name1 = ItemId_GetName(item1);
	const u8* name2 = ItemId_GetName(item2);

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

static s8 CompareItemsByType(struct ItemSlot* itemSlot1, struct ItemSlot* itemSlot2)
{
	//Null items go last
	u8 type1 = (itemSlot1->itemId == ITEM_NONE) ? 0xFF : gItemsByType[itemSlot1->itemId];
	u8 type2 = (itemSlot2->itemId == ITEM_NONE) ? 0xFF : gItemsByType[itemSlot2->itemId];

	if (type1 < type2)
		return -1;
	else if (type1 > type2)
		return 1;

	return CompareItemsAlphabetically(itemSlot1, itemSlot2); //Items are of same type so sort alphabetically
}

static s8 CompareItemsByMost(struct ItemSlot* itemSlot1, struct ItemSlot* itemSlot2)
{
	u16 quantity1 = itemSlot1->quantity;
	u16 quantity2 = itemSlot2->quantity;

	if (itemSlot1->itemId == ITEM_NONE)
		return 1;
	else if (itemSlot2->itemId == ITEM_NONE)
		return -1;

	if (quantity1 < quantity2)
		return 1;
	else if (quantity1 > quantity2)
		return -1;

	return CompareItemsAlphabetically(itemSlot1, itemSlot2); //Items have same quantity so sort alphabetically
}

static s8 CompareItemsByLeast(struct ItemSlot* itemSlot1, struct ItemSlot* itemSlot2)
{
	u16 quantity1 = itemSlot1->quantity;
	u16 quantity2 = itemSlot2->quantity;

	if (itemSlot1->itemId == ITEM_NONE)
		return 1;
	else if (itemSlot2->itemId == ITEM_NONE)
		return -1;

	if (quantity1 < quantity2)
		return -1;
	else if (quantity1 > quantity2)
		return 1;

	return CompareItemsAlphabetically(itemSlot1, itemSlot2); //Items have same quantity so sort alphabetically
}

static s8 CompareItemsByHavingValue(struct ItemSlot* itemSlot1, struct ItemSlot* itemSlot2)
{
	if (itemSlot1->itemId == ITEM_NONE)
		return 1;
	else if (itemSlot2->itemId == ITEM_NONE)
		return -1;

	if (itemSlot1->quantity == 0)
	{
		itemSlot1->itemId = ITEM_NONE; //Remove bugged items
		return 1;
	}
	else if (itemSlot2->quantity == 0)
	{
		itemSlot2->itemId = ITEM_NONE; //Remove bugged items
		return -1;
	}

	return 0;
}

void SortItemsInBag(u8 pocket, u8 type)
{
	struct ItemSlot* itemMem;
	u16 itemAmount;
	s8 (*func)(struct ItemSlot*, struct ItemSlot*);

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
		case SORT_BY_LEAST:
			func = CompareItemsByLeast;
			break;
		case SORT_BY_MOST:
		case SORT_BY_AMOUNT:
			func = CompareItemsByMost;
			break;
		default:
			func = CompareItemsByType;
	}

	MergeSort(itemMem, 0, itemAmount - 1, func);
}

void MergeSort(struct ItemSlot* array, u32 low, u32 high, s8 (*comparator)(struct ItemSlot*, struct ItemSlot*))
{
	if (high <= low)
		return;

	u32 mid = low + (high - low) / 2;
	MergeSort(array, low, mid, comparator); //Sort left half.
	MergeSort(array, mid + 1, high, comparator); //Sort right half.
	Merge(array, low, mid, high, comparator); //Merge results.
}

//Can't be declared static or blows the stack
void Merge(struct ItemSlot* array, u32 low, u32 mid, u32 high, s8 (*comparator)(struct ItemSlot*, struct ItemSlot*))
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
		else if (comparator(&aux[j], &aux[i]) < 0)
			array[k] = aux[j++];
		else
			array[k] = aux[i++];
	}
}

static void FinishBagSortIntro(u8 taskId)
{
	ClearWindowTilemap(GetBagWindow(0xA));
	ClearWindowTilemap(GetBagWindow(6));
	HideBagWindow(0xA);
	HideBagWindow(6);
	PutWindowTilemap(0);

	StringExpandPlaceholders(gStringVar4, gText_SortItemsBy);
	BagPrintTextOnWindow(ShowBagWindow(6, 1), 2, gStringVar4, 0, 2, 1, 0, 0, 1);
	BagCreateYesNoMenuBottomRight(taskId, &sYesNoSortFunctions);
}

static void BagMenu_SortByName(u8 taskId)
{
	gTasks[taskId].data[2] = SORT_ALPHABETICALLY;
	StringCopy(gStringVar1, sSortTypeStrings[SORT_ALPHABETICALLY]);
	FinishBagSortIntro(taskId);
}

static void BagMenu_SortByType(u8 taskId)
{
	gTasks[taskId].data[2] = SORT_BY_TYPE;
	StringCopy(gStringVar1, sSortTypeStrings[SORT_BY_TYPE]);
	FinishBagSortIntro(taskId);
}

static void BagMenu_SortByMost(u8 taskId)
{
	gTasks[taskId].data[2] = SORT_BY_MOST;
	StringCopy(gStringVar1, sSortTypeStrings[SORT_BY_MOST]);
	FinishBagSortIntro(taskId);
}

static void BagMenu_SortByLeast(u8 taskId)
{
	gTasks[taskId].data[2] = SORT_BY_LEAST;
	StringCopy(gStringVar1, sSortTypeStrings[SORT_BY_LEAST]);
	FinishBagSortIntro(taskId);
}

static void BagMenu_SortByAmount(u8 taskId)
{
	gTasks[taskId].data[2] = SORT_BY_AMOUNT; //Just a copy of greatest->least
	StringCopy(gStringVar1, sSortTypeStrings[SORT_BY_AMOUNT]);
	FinishBagSortIntro(taskId);
}

static void BagMenu_CancelSort(u8 taskId)
{
	s16* data = gTasks[taskId].data;

	HideBagWindow(6);
	PutWindowTilemap(1);
	ScheduleBgCopyTilemapToVram(0);
	BagMenu_PrintCursor_(data[0], 1);
	Task_RedrawArrowsAndReturnToBagMenuSelect(taskId);
}

static void BagMenu_ConfirmSort(u8 taskId)
{
	s16* data = gTasks[taskId].data;
	u16* scrollPos = &gBagMenuState.scrollPosition[gBagMenuState.pocket];
	u16* cursorPos = &gBagMenuState.cursorPosition[gBagMenuState.pocket];

	HideBagWindow(6);
	sItemDescriptionPocket = 0x0; //Sorting Items
	StringCopy(gStringVar1, sSortTypeStrings[data[2]]);
	StringExpandPlaceholders(gStringVar4, gText_ItemsSortedBy);
	BagPrintTextOnWindow(ShowBagWindow(6, 3), 2, gStringVar4, 0, 2, 1, 0, 0, 1);
	SortItemsInBag(gBagMenuState.pocket, data[2]);
	DestroyListMenuTask(data[0], scrollPos, cursorPos);
	SetInitialScrollAndCursorPositions(gBagMenuState.pocket);
	Bag_BuildListMenuTemplate(gBagMenuState.pocket);
	data[0] = ListMenuInit(gMultiuseListMenuTemplate, *scrollPos, *cursorPos);
	PlaySE(SE_CORRECT);
	gTasks[taskId].func = Task_SortFinish;
}

static void Task_SortFinish(u8 taskId)
{
	s16* data = gTasks[taskId].data;

	if (gMain.newKeys & (A_BUTTON | B_BUTTON))
	{
		PlaySE(SE_SELECT);
		HideBagWindow(6);
		PutWindowTilemap(1);
		ScheduleBgCopyTilemapToVram(0);
		BagMenu_PrintCursor_(data[0], 1);
		Task_RedrawArrowsAndReturnToBagMenuSelect(taskId);
	}
}

bool8 TrySetupSortBag(u8 taskId)
{
	if (gMain.newKeys & START_BUTTON && gBagMenuState.location == BAG_OPEN_REGULAR)
	{
		BagDestroyPocketScrollArrowPair();
		Var800E = 0xF9F9;
		PlaySE(SE_WIN_OPEN);
		gTasks[taskId].func = Task_ItemContextMenuByLocation;
		return TRUE;
	}

	if (Var800E == 0xF9F9)
		Var800E = 0; //Don't activate sorter

	return FALSE;
}

void LoadBagSorterMenuOptions(void)
{
	switch (gBagMenuState.pocket + 1) {
		case POCKET_KEY_ITEMS:
			sBagContextMenuItemsPtr = gBagMenuSortKeyItems;
			sBagContextMenuNumItems = NELEMS(gBagMenuSortKeyItems);
			break;
		case POCKET_POKE_BALLS:
			sBagContextMenuItemsPtr = gBagMenuSortPokeBalls;
			sBagContextMenuNumItems = NELEMS(gBagMenuSortPokeBalls);
			break;
		default:
			sBagContextMenuItemsPtr = gBagMenuSortItems;
			sBagContextMenuNumItems = NELEMS(gBagMenuSortItems);
			break;
	}
}

void PrintBagSortItemQuestion(u8 windowId)
{
	BagPrintTextOnWindow(windowId, 2, gText_WantToSortItems, 0, 2, 1, 0, 0, 1);
}

//Bag Scroll Speed
#define tItemIndex data[0]
void Task_PrintItemDescriptionOnKeyRelease(u8 taskId)
{
	if (JOY_NEW_AND_REPEATED(B_BUTTON | SELECT_BUTTON))
	{
		//Closing the bag or moving items
		DestroyTask(taskId);
	}
	else if ((JOY_NEW_AND_REPEATED(DPAD_LEFT) && gBagMenuState.pocket > 0)
		  || (JOY_NEW_AND_REPEATED(DPAD_RIGHT) && gBagMenuState.pocket < MAIN_POCKETS_COUNT - 1))
	{
		//Changed pockets
		//Pressing Left & Right otherwise shouldn't destroy the task
		DestroyTask(taskId);
	}
	else if (JOY_NEW_AND_REPEATED(A_BUTTON | START_BUTTON) //Selecting item or sorting items
		|| (!JOY_HELD(DPAD_DOWN) && !JOY_HELD(DPAD_UP))) //Stopped scrolling
	{
		sItemDescriptionPocket = gBagMenuState.pocket + 1; //Update description's pocket number
		const u8 *description = ItemId_GetDescription(BagGetItemIdByPocketPosition(gBagMenuState.pocket + 1, gTasks[taskId].tItemIndex));
		BagPrintTextOnWindow(1, 2, description, 0, 3, 2, 0, 0, 0);
		DestroyTask(taskId);
	}
}

static void TryDestroyItemDescriptionTask(void)
{
	u8 taskId = FindTaskIdByFunc(Task_PrintItemDescriptionOnKeyRelease);
	if (taskId != 0xFF)
		DestroyTask(taskId);
}

void PrintItemDescriptionOnMessageWindow(u16 itemIndex)
{
	const u8 *description;
	FillWindowPixelBuffer(1, PIXEL_FILL(0));

	if (itemIndex < GetCurrentPocketItemAmount()) //Not end of bag
	{
		description = ItemId_GetDescription(BagGetItemIdByPocketPosition(gBagMenuState.pocket + 1, itemIndex));

		if (JOY_NEW(DPAD_DOWN | DPAD_UP) //Single item scroll
		|| itemIndex == 0 //Top of bag
		|| sItemDescriptionPocket != gBagMenuState.pocket + 1 //Changed pockets so print some description
		|| sItemDescriptionPocket == 0) //Open bag or post item sort
		{
			sItemDescriptionPocket = gBagMenuState.pocket + 1;
			BagPrintTextOnWindow(1, 2, description, 0, 3, 2, 0, 0, 0); //Print description right away
			TryDestroyItemDescriptionTask(); //Task no longer needed
		}
		else
		{
			u8 taskId = FindTaskIdByFunc(Task_PrintItemDescriptionOnKeyRelease);
			if (taskId == 0xFF)
				taskId = CreateTask(Task_PrintItemDescriptionOnKeyRelease, 0x0); //Print description when scroll is done

			if (taskId != 0xFF)
				gTasks[taskId].tItemIndex = itemIndex;
			CopyWindowToVram(1, 2); //Blank out description box
			PutWindowTilemap(1);
		}
	}
	else
	{
		sItemDescriptionPocket = gBagMenuState.pocket + 1; //Update description's pocket number
		description = (void*) 0x84162F5; //gText_CloseBag;
		BagPrintTextOnWindow(1, 2, description, 0, 3, 2, 0, 0, 0); //Print description right away
		TryDestroyItemDescriptionTask(); //Task no longer needed
	}
}
#undef tItemIndex

void ForceRedrawItemDescription(void)
{
	sItemDescriptionPocket = 0;
}
