#pragma once

#include "global.h"

extern const u8 *const gUnknown_0858BAF0[9];

bool8 __attribute__((long_call)) ScriptMenu_Multichoice(u8 left, u8 top, u8 var3, u8 var4);
bool8 __attribute__((long_call)) ScriptMenu_MultichoiceWithDefault(u8 left, u8 top, u8 var3, u8 var4, u8 var5);
bool8 __attribute__((long_call)) ScriptMenu_YesNo(u8 var1, u8 var2);
bool8 __attribute__((long_call)) ScriptMenu_MultichoiceGrid(u8 left, u8 top, u8 multichoiceId, u8 a4, u8 columnCount);
bool8 __attribute__((long_call)) ScriptMenu_ShowPokemonPic(u16 var1, u8 var2, u8 var3);
bool8 __attribute__((long_call)) (*ScriptMenu_GetPicboxWaitFunc(void))(void);
u8 __attribute__((long_call)) CreateWindowFromRect(u8, u8, u8, u8);

