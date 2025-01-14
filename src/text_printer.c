#include "defines.h"
#include "../include/text.h"
#include "../include/new_menu_helpers.h"
#include "../include/window.h"
#include "../include/constants/songs.h"

extern struct TextPrinter sTextPrinters[];

bool8 IsAutoScrollEnabled(void)
{
	#if (defined AUTOSCROLL_TEXT_BY_HOLDING_R || defined DEBUG_AUTO_SCROLL)
	return TRUE;
	#endif

	#ifdef FLAG_SANDBOX_MODE
	if (FlagGet(FLAG_SANDBOX_MODE))
		return TRUE;
	#endif

	#ifdef FLAG_NEW_GAME_PLUS
	if (FlagGet(FLAG_NEW_GAME_PLUS))
		return TRUE;
	#endif

	#ifdef UNBOUND
	if (FlagGet(FLAG_SYS_GAME_CLEAR))
		return TRUE;
	#endif

	return FALSE;
}

bool32 RunTextPrintersForInstantText(void)
{
	#ifdef INSTANT_TEXT
	int i, j;
	u16 result;

	if (GetTextSpeedSetting() != 0x7f)
		return FALSE;

	for (i = 0; i < 0x20; ++i)
	{
		if (sTextPrinters[i].active != 0)
		{
			for (j = 0; j < 0x400; j++)
			{
				u32 oldState;
				u32 newState;

				oldState = sTextPrinters[i].state;
				result = RenderFont(&sTextPrinters[i]);
				newState = sTextPrinters[i].state;

				if (result == 0)
				{
					if (sTextPrinters[i].callback != 0)
						sTextPrinters[i].callback(&sTextPrinters[i].printerTemplate, result);
				}
				else if (result == 3)
				{
					if (sTextPrinters[i].callback != 0)
						sTextPrinters[i].callback(&sTextPrinters[i].printerTemplate, result);
					if (oldState == 0 && newState != 0)
						CopyWindowToVram(sTextPrinters[i].printerTemplate.windowId, 2);
					break;
				}
				else if (result == 1)
				{
					CopyWindowToVram(sTextPrinters[i].printerTemplate.windowId, 2);
					sTextPrinters[i].active = 0;
					break;
				}
			}
		}
	}
	
	return TRUE;
	#else
	return FALSE;
	#endif
}

bool16 TextPrinterWaitWithDownArrow(struct TextPrinter *textPrinter)
{
	bool8 result = FALSE;
	if (gTextFlags.autoScroll != 0)
	{
		result = TextPrinterWaitAutoMode(textPrinter);
	}
	else
	{
		TextPrinterDrawDownArrow(textPrinter);
		if (JOY_NEW(A_BUTTON | B_BUTTON)
		|| (JOY_HELD(R_BUTTON) && IsAutoScrollEnabled()))
		{
			result = TRUE;
			PlaySE(SE_SELECT);
		}
	}

	return result;
}

bool16 TextPrinterWait(struct TextPrinter *textPrinter)
{
	bool16 result = FALSE;
	if (gTextFlags.autoScroll != 0)
	{
		result = TextPrinterWaitAutoMode(textPrinter);
	}
	else
	{
		if (JOY_NEW(A_BUTTON | B_BUTTON)
		|| (JOY_HELD(R_BUTTON) && IsAutoScrollEnabled()))
		{
			result = TRUE;
			PlaySE(SE_SELECT);
		}
	}

	return result;
}

bool8 __attribute__((long_call)) WaitForAorBPressOld(void);
bool8 WaitForAorBPress(void)
{
	if (JOY_HELD(R_BUTTON) && IsAutoScrollEnabled())
		return TRUE;

	return WaitForAorBPressOld();
}

bool8 ShouldCloseBattleLevelUpBox(void)
{
	return gMain.newKeys
		|| (JOY_HELD(R_BUTTON) && IsAutoScrollEnabled());
}
