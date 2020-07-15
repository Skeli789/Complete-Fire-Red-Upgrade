#pragma once

#include "global.h"

bool8 __attribute__((long_call)) ShowFieldMessage(const u8* message);
void __attribute__((long_call)) HideFieldMessageBox(void);
void __attribute__((long_call)) TextboxFdecodeAutoAndCreateTask(const u8* text);

/*
bool8 __attribute__((long_call)) sub_8098238(const u8 *message);
bool8 __attribute__((long_call)) ShowFieldAutoScrollMessage(const u8 *message);
bool8 __attribute__((long_call)) IsFieldMessageBoxHidden(void);
*/
