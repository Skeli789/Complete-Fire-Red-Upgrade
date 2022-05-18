#ifndef GUARD_PLAYER_TM_CASE_H
#define GUARD_PLAYER_TM_CASE_H

#include "global.h"

void __attribute__((long_call)) LoadBGTemplates(void);
void __attribute__((long_call)) InitWindowTemplatesAndPals(void);
bool8 __attribute__((long_call)) HandleLoadTMCaseGraphicsAndPalettes(void);
//void __attribute__((long_call)) SortPocketAndPlaceHMsFirst(struct BagPocket * pocket);
void __attribute__((long_call)) TMCaseSetup_GetTMCount(void);
//void __attribute__((long_call)) TMCaseSetup_InitListMenuPositions(void);
void __attribute__((long_call)) TMCaseSetup_UpdateVisualMenuOffset(void);
//void __attribute__((long_call)) DrawMoveInfoUIMarkers(void);
void __attribute__((long_call)) CreateTMCaseListMenuBuffers(void);
void __attribute__((long_call)) InitTMCaseListMenuItems(void);
void __attribute__((long_call)) PrintStringTMCaseOnWindow3(void);
void __attribute__((long_call)) CreateTMCaseScrollIndicatorArrowPair_Main(void);
//u8 __attribute__((long_call)) CreateTMSprite(u16);
void __attribute__((long_call)) VBlankCB_Idle(void);
void __attribute__((long_call)) CB2_Idle(void);
void __attribute__((long_call)) Task_TMCaseDude1(u8);
void __attribute__((long_call)) Task_TMCaseMain(u8);
void __attribute__((long_call)) SetBagItemQuantity(u16 *, u16);
void __attribute__((long_call)) *AllocZeroed(u32 size);
void __attribute__((long_call)) SpriteCB_MonIcon(struct Sprite *);
void __attribute__((long_call)) AddTextPrinterParameterized_ColorByIndex(u8 windowId, u8 fontId, const u8 * str, u8 x, u8 y, u8 letterSpacing, u8 lineSpacing, u8 speed, u8 colorIdx);
bool8 __attribute__((long_call)) itemid_is_unique(u16 itemId);
void __attribute__((long_call)) TMCase_SetWindowBorder2(u8 windowId);

#endif //GUARD_PLAYER_TM_CASE_H