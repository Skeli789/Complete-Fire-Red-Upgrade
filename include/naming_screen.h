#ifndef GUARD_NAMING_SCREEN_H
#define GUARD_NAMING_SCREEN_H

#include "global.h"


#define KBEVENT_NONE 0
#define KBEVENT_PRESSED_A 5
#define KBEVENT_PRESSED_B 6
#define KBEVENT_PRESSED_SELECT 8
#define KBEVENT_PRESSED_START 9

enum
{
    NAMING_SCREEN_PLAYER,
    NAMING_SCREEN_BOX,
    NAMING_SCREEN_CAUGHT_MON,
    NAMING_SCREEN_NAME_RATER,
    NAMING_SCREEN_RIVAL,

	//New
    NAMING_SCREEN_CHOOSE_NUMBER,
	NAMING_SCREEN_ENTER_PHRASE,
	NAMING_SCREEN_ENTER_PASSWORD,
	NAMING_SCREEN_ENTER_5_CHAR_PASSWORD,
	NAMING_SCREEN_ENTER_SPECIES_NAME,
};

struct NamingScreenTemplate
{
    u8 copyExistingString;
    u8 maxChars;
    u8 iconFunction;
    u8 addGenderIcon;
    u8 initialPage;
    u8 unused;
    const u8 *title;
};

struct NamingScreenData {
 /*0x0*/    u8 tilemapBuffer1[0x800];
 /*0x800*/  u8 tilemapBuffer2[0x800];
 /*0x800*/  u8 tilemapBuffer3[0x800];
 /*0x1800*/ u8 textBuffer[0x10];
 /*0x1810*/ u8 tileBuffer[0x600];
 /*0x1E10*/ u8 state;
 /*0x1E11*/ u8 windows[5];
 /*0x1E16*/ u16 inputCharBaseXPos;
 /*0x1E18*/ u16 bg1vOffset;
 /*0x1E1A*/ u16 bg2vOffset;
 /*0x1E1C*/ u16 bg1Priority;
 /*0x1E1E*/ u16 bg2Priority;
 /*0x1E20*/ u8 bgToReveal;
 /*0x1E21*/ u8 bgToHide;
 /*0x1E22*/ u8 currentPage;
 /*0x1E23*/ u8 cursorSpriteId;
 /*0x1E24*/ u8 selectBtnFrameSpriteId;
 /*0x1E25*/ u8 keyRepeatStartDelayCopy;
 /*0x1E28*/ const struct NamingScreenTemplate *template;
 /*0x1E2C*/ u8 templateNum;
 /*0x1E30*/ u8 *destBuffer;
 /*0x1E34*/ u16 monSpecies;
 /*0x1E36*/ u16 monGender;
 /*0x1E38*/ u32 monPersonality;
 /*0x1E3C*/ MainCallback returnCallback;
};

enum
{
    PAGE_OTHERS,
    PAGE_UPPER,
    PAGE_LOWER,
};

enum
{
    MAIN_STATE_BEGIN_FADE_IN,
    MAIN_STATE_WAIT_FADE_IN,
    MAIN_STATE_HANDLE_INPUT,
    MAIN_STATE_MOVE_TO_OK_BUTTON,
    MAIN_STATE_START_PAGE_SWAP,
    MAIN_STATE_WAIT_PAGE_SWAP,
    MAIN_STATE_6,
    MAIN_STATE_UPDATE_SENT_TO_PC_MESSAGE,
    MAIN_STATE_BEGIN_FADE_OUT,
};

enum
{
    INPUT_STATE_DISABLED,
    INPUT_STATE_ENABLED,
    INPUT_STATE_2,
};

u8 __attribute__((long_call)) DoNamingScreen(u8 caseId, u8* dst, u16 monSpecies, u8 monGender, u32 monPersonality, void (*callback)(void));
void __attribute__((long_call)) NamingScreen_TryStartButtonFlash(u8, u8, u8);
bool8 __attribute__((long_call)) NameChooserSelectLetter(void);
void __attribute__((long_call)) NamingScreen_SquishCursor(void);
void __attribute__((long_call)) SetInputState(u8);
bool8 __attribute__((long_call)) MainState_StartPageSwap(void);
u8 __attribute__((long_call)) GetKeyRoleAtCursorPos(void);
bool8 __attribute__((long_call)) NamingScreen_TriggerKeyboardChange(void);
void __attribute__((long_call)) MoveCursorToOKButton(void);
void __attribute__((long_call)) DeleteTextCharacter(void);
bool8 __attribute__((long_call)) KeyboardKeyHandler_Page(u8);
bool8 __attribute__((long_call)) KeyboardKeyHandler_Backspace(u8);
bool8 __attribute__((long_call)) KeyboardKeyHandler_OK(u8);
u8 __attribute__((long_call)) GetInputEvent(void);
bool8 __attribute__((long_call)) MainState_WaitPageSwap(void);
void __attribute__((long_call)) NamingScreen_GetCursorPos(s16 *x, s16 *y);
u8 __attribute__((long_call)) NamingScreen_GetCharAtKeyboardPos(s16 x, s16 y);

#endif // GUARD_NAMING_SCREEN_H
