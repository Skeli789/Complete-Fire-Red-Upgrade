#include "../include/window.h"
#include "../include/text.h"
#include "../include/new_menu_helpers.h"

extern struct TextPrinter sTextPrinters[];

bool32 RunTextPrintersForInstantText(void) {
    int i, j;
    u16 result;

    if (GetTextSpeedSetting() != 0x7f) {
        return FALSE;
    }

    for (i = 0; i < 0x20; ++i)
    {
        if (sTextPrinters[i].active != 0)
        {
            for (j = 0; j < 0x400; j++) {
                u32 oldState;
                u32 newState;

                oldState = sTextPrinters[i].state;
                result = RenderFont(&sTextPrinters[i]);
                newState = sTextPrinters[i].state;

                if (result == 0) {
                    if (sTextPrinters[i].callback != 0)
                        sTextPrinters[i].callback(&sTextPrinters[i].printerTemplate, result);
                } else if (result == 3) {
                    if (sTextPrinters[i].callback != 0)
                        sTextPrinters[i].callback(&sTextPrinters[i].printerTemplate, result);
                    if (oldState == 0 && newState != 0)
                        CopyWindowToVram(sTextPrinters[i].printerTemplate.windowId, 2);
                    break;
                } else if (result == 1) {
                    CopyWindowToVram(sTextPrinters[i].printerTemplate.windowId, 2);
                    sTextPrinters[i].active = 0;
                    break;
                }
            }
        }
    }
    
    return TRUE;
}
