#pragma once

#include "../global.h"
#include "../../src/config.h"

extern const u16 Fairy_TM_DiskPal[];

u8* ItemId_GetName(u16 itemId);
u8 ItemId_GetHoldEffect(u16 itemId);
u8 ItemId_GetHoldEffectParam(u16 itemId);
u8 ItemId_GetSecondaryId(u16 itemId);
bool8 IsMegaStone(u16 item);
bool8 IsZCrystal(u16 item);
bool8 IsTypeZCrystal(u16 item, u8 moveType);
bool8 IsBerry(u16 item);
bool8 IsPinchBerryItemEffect(u8 itemEffect);
bool8 IsGem(u16 item);
bool8 IsTMHM(u16 item);
void CopyTMName(u8* dst, u16 itemId);

u32 CanMonLearnTMHM(struct Pokemon* mon, u8 tm);
bool8 CanMonLearnTutorMove(struct Pokemon* mon, u8 tutorId);
u16 GetExpandedTutorMove(u8 tutorId);
u8 CanMonLearnTMTutor(struct Pokemon* mon, u16 item, u8 tutor);

//Hooked in Functions:
u8 TryHandleExcuseForDracoMeteorTutor(struct Pokemon* mon);
u8 TryHandleExcuseForDracoMeteorTutorAlreadyKnow(void);

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