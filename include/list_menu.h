#pragma once
#include "global.h"
#include "window.h"

#define LIST_NOTHING_CHOSEN -1
#define LIST_CANCEL -2
#define LIST_HEADER -3

#define LIST_NO_MULTIPLE_SCROLL     0
#define LIST_MULTIPLE_SCROLL_DPAD   1
#define LIST_MULTIPLE_SCROLL_L_R    2

enum
{
    SCROLL_ARROW_LEFT,
    SCROLL_ARROW_RIGHT,
    SCROLL_ARROW_UP,
    SCROLL_ARROW_DOWN
};

struct ListMenu;

struct ListMenuItem
{
    const u8* name;
    s32 id;
};

extern struct ListMenuItem gMultiChoice[7];

struct ListMenuTemplate
{
    /*0x00*/ const struct ListMenuItem *items;
    /*0x04*/ void (* moveCursorFunc)(s32 itemIndex, bool8 onInit, struct ListMenu *list);
    /*0x08*/ void (* itemPrintFunc)(u8 windowId, s32 itemId, u8 y);
    /*0x0C*/ u16 totalItems;
    /*0x0E*/ u16 maxShowed;
    /*0x10*/ u8 windowId;
    /*0x11*/ u8 header_X;
    /*0x12*/ u8 item_X;
    /*0x13*/ u8 cursor_X;
    /*0x14*/ u8 upText_Y:4; // x1, x2, x4, x8 = xF
             u8 cursorPal:4; // x10, x20, x40, x80 = xF0
    /*0x15*/ u8 fillValue:4; // x1, x2, x4, x8 = xF
             u8 cursorShadowPal:4; // x10, x20, x40, x80 = xF0
    /*0x16*/ u8 lettersSpacing:3;
             u8 itemVerticalPadding:3;
             u8 scrollMultiple:2; // x40, x80 = xC0
    /*0x17*/ u8 fontId:6; // x1, x2, x4, x8, x10, x20 = x3F
             u8 cursorKind:2; // x40, x80
}; /* size = 0x18 */

struct ListMenu
{
    struct ListMenuTemplate template;
    u16 cursorPos;
    u16 itemsAbove;
    u8 unk_1C;
    u8 unk_1D;
    u8 taskId;
    u8 unk_1F;
};

struct ListMenuWindowRect
{
    u8 x;
    u8 y;
    u8 width;
    u8 height;
    u8 palNum;
};

struct ScrollArrowsTemplate
{
    u8 firstArrowType;
    u8 firstX;
    u8 firstY;
    u8 secondArrowType;
    u8 secondX;
    u8 secondY;
    u16 fullyUpThreshold;
    u16 fullyDownThreshold;
    u16 tileTag;
    u16 palTag;
    u8 palNum;
};

struct CursorStruct
{
    u8 left;
    u8 top;
    u16 rowWidth;
    u16 rowHeight;
    u16 tileTag;
    u16 palTag;
    u8 palNum;
};

extern struct ScrollArrowsTemplate gTempScrollArrowTemplate;
//extern struct ListMenuTemplate gMultiuseListMenuTemplate;

// Exported ROM declarations
s32 __attribute__((long_call)) DoMysteryGiftListMenu(const struct WindowTemplate *windowTemplate, const struct ListMenuTemplate *listMenuTemplate, u8 arg2, u16 tileNum, u16 palNum);
u8 __attribute__((long_call)) ListMenuInit(const struct ListMenuTemplate *listMenuTemplate, u16 cursorPos, u16 itemsAbove);
u8 __attribute__((long_call)) ListMenuInitInRect(struct ListMenuTemplate *listMenuTemplate, struct ListMenuWindowRect *arg1, u16 scrollOffset, u16 selectedRow);
s32 __attribute__((long_call)) ListMenu_ProcessInput(u8 listTaskId);
void __attribute__((long_call)) DestroyListMenuTask(u8 listTaskId, u16 *scrollOffset, u16 *selectedRow);
void __attribute__((long_call)) RedrawListMenu(u8 listTaskId);
void __attribute__((long_call)) ChangeListMenuPals(u8 listTaskId, u8 cursorPal, u8 fillValue, u8 cursorShadowPal);
void __attribute__((long_call)) ChangeListMenuCoords(u8 listTaskId, u8 x, u8 y);
s32 __attribute__((long_call)) ListMenuTestInput(struct ListMenuTemplate *template, u32 scrollOffset, u32 selectedRow, u16 keys, u16 *newScrollOffset, u16 *newSelectedRow);
void __attribute__((long_call)) ListMenuGetCurrentItemArrayId(u8 listTaskId, u16 *arrayId);
void __attribute__((long_call)) ListMenuGetScrollAndRow(u8 listTaskId, u16 *scrollOffset, u16 *selectedRow);
u16 __attribute__((long_call)) ListMenuGetYCoordForPrintingArrowCursor(u8 listTaskId);
void __attribute__((long_call)) ListMenuOverrideSetColors(u8 cursorPal, u8 fillValue, u8 cursorShadowPal);
void __attribute__((long_call)) ListMenuDefaultCursorMoveFunc(s32 arg0, u8 arg1, struct ListMenu *list);
s32 __attribute__((long_call)) ListMenuGetUnkIndicatorsStructFields(u8 taskId, u8 field);
void __attribute__((long_call)) ListMenuSetUnkIndicatorsStructField(u8 taskId, u8 field, s32 value);
u8 __attribute__((long_call)) AddScrollIndicatorArrowPair(const struct ScrollArrowsTemplate *arrowInfo, u16 *arg1);
u8 __attribute__((long_call)) AddScrollIndicatorArrowPairParameterized(u32 arrowType, s32 commonPos, s32 firstPos, s32 secondPos, s32 fullyDownThreshold, s32 tileTag, s32 palTag, u16 *currItemPtr);
void __attribute__((long_call)) RemoveScrollIndicatorArrowPair(u8 taskId);
void __attribute__((long_call)) Task_ScrollIndicatorArrowPairOnMainMenu(u8 taskId);
