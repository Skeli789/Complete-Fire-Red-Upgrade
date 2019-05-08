#pragma once

u8* ItemId_GetName(u16 itemId);
u8 ItemId_GetHoldEffect(u16 itemId);
u8 ItemId_GetHoldEffectParam(u16 itemId);
u8 ItemId_GetSecondaryId(u16 itemId);
bool8 IsMegaStone(u16 item);
bool8 IsZCrystal(u16 item);

u32 CanMonLearnTMHM(struct Pokemon* mon, u8 tm);