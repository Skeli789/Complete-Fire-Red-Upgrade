#ifndef GUARD_NEW_MENU_HELPERS_H
#define GUARD_NEW_MENU_HELPERS_H

#include "global.h"
#include "text.h"
#include "task.h"

struct TextPrinterTemplate;	// ??

void sub_81973A4(void);
void sub_81973C4(u8, u8);
void sub_819746C(u8 windowId, bool8 copyToVram);
void sub_81973FC(u8, u8);
u16 __attribute((long_call)) AddTextPrinterParametrized(u8 windowId, u8 fontId, const u8 *str, u8 speed, void ( *callback)(u16, struct TextPrinter *), u8 fgColor, u8 bgColor, u8 shadowColor);
void __attribute((long_call)) DisplayItemMessageOnField(u8 taskId, u8 bgId, const u8 *src, TaskFunc callback);
void sub_8197434(u8 a0, u8 a1);
void __attribute((long_call)) SetStandardWindowBorderStyle(u8 a0, u8 a1);
void sub_8197930(void);
u8 __attribute((long_call)) GetPlayerTextSpeed(void);
void sub_80F6F54(u8, u8);
u8 sub_80F78A8(void);
void sub_80F6E9C(void);

void __attribute__((long_call)) DrawDialogueFrameNewMenuHelpers(u8 windowId, bool8 transfer);
u16 __attribute__((long_call)) AddTextPrinterParameterized2(u8 windowId, u8 fontId, const u8 *str, u8 speed, void (*callback)(struct TextPrinterTemplate *, u16), u8 fgColor, u8 bgColor, u8 shadowColor);


#endif // GUARD_NEW_MENU_HELPERS_H
