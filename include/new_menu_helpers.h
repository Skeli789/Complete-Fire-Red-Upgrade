#ifndef GUARD_NEW_MENU_HELPERS_H
#define GUARD_NEW_MENU_HELPERS_H

#include "global.h"
#include "text.h"
#include "task.h"

struct TextPrinterTemplate;	// ??

u16 __attribute((long_call)) AddTextPrinterParametrized(u8 windowId, u8 fontId, const u8 *str, u8 speed, void ( *callback)(u16, struct TextPrinter *), u8 fgColor, u8 bgColor, u8 shadowColor);
void __attribute((long_call)) DisplayItemMessageOnField(u8 taskId, u8 bgId, const u8 *src, TaskFunc callback);
void __attribute((long_call)) SetStandardWindowBorderStyle(u8 a0, u8 a1);
u8 __attribute((long_call)) GetPlayerTextSpeed(void);
void __attribute__((long_call)) DrawDialogueFrameNewMenuHelpers(u8 windowId, bool8 transfer);
u16 __attribute__((long_call)) AddTextPrinterParameterized2(u8 windowId, u8 fontId, const u8 *str, u8 speed, void (*callback)(struct TextPrinterTemplate *, u16), u8 fgColor, u8 bgColor, u8 shadowColor);
void __attribute__((long_call)) DrawHelpMessageWindowWithText(const u8 * text);
u8 __attribute__((long_call)) GetTextSpeedSetting(void);
void __attribute__((long_call)) DrawDialogueFrame(u8 windowId, bool8 transfer);
void __attribute__((long_call)) LoadStdWindowFrameGfx(void);
u16 __attribute__((long_call)) GetStdWindowBaseTileNum(void);

#endif // GUARD_NEW_MENU_HELPERS_H
