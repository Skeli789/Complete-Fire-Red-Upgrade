#pragma once

#include "../global.h"
#include "../../src/config.h"

/**
 * \file item.h
 * \brief Contains functions relating to items and the bag.
 *		  Also helps with TM/HM expansion and the bag expansion.
 */

//Exported Functions
u16 SanitizeItemId(u16 itemId);
const u8* ItemId_GetName(u16 itemId);
u8 ItemId_GetHoldEffect(u16 itemId);
u8 ItemId_GetHoldEffectParam(u16 itemId);
u8 ItemId_GetMystery2Id(u16 itemId);
bool8 IsMegaStone(u16 item);
bool8 IsZCrystal(u16 item);
bool8 IsTypeZCrystal(u16 item, u8 moveType);
bool8 IsBerry(u16 item);
bool8 IsPinchBerryItemEffect(u8 itemEffect);
bool8 IsGem(u16 item);
bool8 IsTMHM(u16 item);
u8 TMIdFromItemId(u16 itemId);
u8 BerryIdFromItemId(u16 item);

u32 CanMonLearnTMHM(struct Pokemon* mon, u8 tm);
bool8 CanMonLearnTutorMove(struct Pokemon* mon, u8 tutorId);
u16 GetExpandedTutorMove(u8 tutorId);
void CopyTMName(u8* dst, u16 itemId);
u8 CanMonLearnTMTutor(struct Pokemon* mon, u16 item, u8 tutor);

u16 GetNumItemsInPocket(u8 pocket);
u16 GetCurrentPocketItemAmount(void);
bool8 DoesBagHaveBerry(void);
void SortBerriesOrTMHMs(struct BagPocket* bagPocket);
void SortItemsInBag(u8 pocket, u8 type);

//Hooked in Functions
u8 TryHandleExcuseForDracoMeteorTutor(struct Pokemon* mon);
u8 TryHandleExcuseForDracoMeteorTutorAlreadyKnow(void);
void CancelPartyMenuLearnTutor(u8 taskId);
u16 ItemIdToBattleMoveId(u16 item);
void LoadTMNameWithNo(u8* dst, u16 itemId);
void LoadTmHmNameInMart(u16 item);
void* LoadTmHmMartDescription(u16 item);
bool8 CheckIsHmMove(u16 move);
bool8 CheckTmHmInFront(u16 item);
u8 CheckDiscIsTmHm(struct Sprite* disc, u16 itemId);
u8 TmHMDiscPosition(unusedArg struct Sprite* disc, u8 tmId);
bool8 CheckReusableTMs(u16 item);
u8 CheckHmSymbol(u16 item);
bool8 CheckSellTmHm(u16 item);
void CheckTmPurchase(u16 item, u8 taskId);
bool8 CheckBuyableTm(u16 item, u8 taskId);
u16 CheckTmPrice(u16 item);
u8 CheckSingleBagTm(u16 item);
const void* FixTmHmDiscPalette(u8 type);

void CompactItemsInBagPocket(struct ItemSlot* itemSlots, u16 amount);
void Task_ReturnToItemListAfterItemPurchase(u8 taskId);
void SetMemoryForBagStorage(void);
void AllocateBagItemListBuffers(void);
bool8 AllocateBerryPouchListBuffers(void);
void PokeDudeBackupBag(void);
void PokeDudeRestoreBag(void);
void PokeDudeBackupKeyItemsTMs(void);
void PokeDudeRestoreKeyItemsTMs(void);
void StoreBagItemCount(void);
bool8 TrySetupSortBag(u8 taskId);
void LoadBagSorterMenuOptions(void);
void PrintBagSortItemQuestion(u8 windowId);

//Exported Constants
enum
{
    CAN_LEARN_MOVE,
    CANNOT_LEARN_MOVE,
    ALREADY_KNOWS_MOVE,
    CANNOT_LEARN_MOVE_IS_EGG,
};

#define NUM_TMSHMS NUM_TMS + NUM_HMS	// never change this

#define gTMHMMoves ((const u16*) *((u32*) 0x8125A8C))
#define gTutorMoves ((const u16*) *((u32*) 0x8120BE4))
