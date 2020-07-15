#pragma once

#include "global.h"
#include "text.h"

#define PIXEL_FILL(num) ((num) | ((num) << 4))

enum
{
    WINDOW_BG,
    WINDOW_TILEMAP_LEFT,
    WINDOW_TILEMAP_TOP,
    WINDOW_WIDTH,
    WINDOW_HEIGHT,
    WINDOW_PALETTE_NUM,
    WINDOW_BASE_BLOCK,
    WINDOW_TILE_DATA
};

struct WindowTemplate
{
    u8 bg;
    u8 tilemapLeft;
    u8 tilemapTop;
    u8 width;
    u8 height;
    u8 paletteNum;
    u16 baseBlock;
};

#define WINDOWS_MAX 32

#define DUMMY_WIN_TEMPLATE          \
{                                   \
    0xFF,                           \
    0,                              \
    0,                              \
    0,                              \
    0,                              \
    0,                              \
    0,                              \
}

#define COPYWIN_MAP  1
#define COPYWIN_GFX  2
#define COPYWIN_BOTH 3

struct Window
{
    struct WindowTemplate window;
    u8 *tileData;
};

bool16 __attribute__((long_call)) InitWindows(const struct WindowTemplate* templates);
u16 __attribute__((long_call)) AddWindow(const struct WindowTemplate *template);
void __attribute__((long_call)) RemoveWindow(u8 windowId);
void __attribute__((long_call)) FreeAllWindowBuffers(void);
void __attribute__((long_call)) CopyWindowToVram(u8 windowId, u8 mode);
void __attribute__((long_call)) PutWindowTilemap(u8 windowId);
void __attribute__((long_call)) PutWindowRectTilemapOverridePalette(u8 windowId, u8 x, u8 y, u8 width, u8 height, u8 palette);
void __attribute__((long_call)) ClearWindowTilemap(u8 windowId);
void __attribute__((long_call)) PutWindowRectTilemap(u8 windowId, u8 x, u8 y, u8 width, u8 height);
void __attribute__((long_call)) BlitBitmapToWindow(u8 windowId, const u8 *pixels, u16 x, u16 y, u16 width, u16 height);
void __attribute__((long_call)) BlitBitmapRectToWindow(u8 windowId, const u8 *pixels, u16 srcX, u16 srcY, u16 srcWidth, int srcHeight, u16 destX, u16 destY, u16 rectWidth, u16 rectHeight);
void __attribute__((long_call)) BlitBitmapRectToWindowWithColorKey(u8 windowId, const u8 *pixels, u16 srcX, u16 srcY, u16 srcWidth, int srcHeight, u16 destX, u16 destY, u16 rectWidth, u16 rectHeight, u8 colorKey);
void __attribute__((long_call)) FillWindowPixelRect(u8 windowId, u8 fillValue, u16 x, u16 y, u16 width, u16 height);
void __attribute__((long_call)) CopyToWindowPixelBuffer(u8 windowId, const void *src, u16 size, u16 tileOffset);
void __attribute__((long_call)) FillWindowPixelBuffer(u8 windowId, u8 fillValue);
void __attribute__((long_call)) ScrollWindow(u8 windowId, u8 direction, u8 distance, u8 fillValue);
void __attribute__((long_call)) CallWindowFunction(u8 windowId, void ( *func)(u8, u8, u8, u8, u8, u8));
bool8 __attribute__((long_call)) SetWindowAttribute(u8 windowId, u8 attributeId, u32 value);
u32 __attribute__((long_call)) GetWindowAttribute(u8 windowId, u8 attributeId);
u8 __attribute__((long_call)) GetNumActiveWindowsOnBg(u8 bgId);
u8 __attribute__((long_call)) WindowPrint(u8 windowId, u8 fontId, u8 left, u8 top, const struct TextColor* color, s8 speed, const u8 *str);
void __attribute__((long_call)) ChoiceSetupSimple(u8 rid, u8 fboxid, u8 x, u8 y, u8 y_stride, u8 num_choices, u8 preselected_choice);
s8 __attribute__((long_call)) RboxChoiceUpdate(void);
u8 __attribute__((long_call)) ClearStdWindowAndFrameToTransparent(u8 id, bool8 update);
void __attribute__((long_call)) ClearDialogWindowAndFrame(u8 windowId, bool8 copyToVram);

/*
int AddWindowWithoutTileMap(const struct WindowTemplate *template);
void CopyWindowRectToVram(u32 windowId, u32 mode, u32 x, u32 y, u32 w, u32 h);
u16 AddWindow8Bit(struct WindowTemplate *template);
u16 AddWindow8Bit(struct WindowTemplate *template);
void FillWindowPixelRect8Bit(u8 windowId, u8 fillValue, u16 x, u16 y, u16 width, u16 height);
void BlitBitmapRectToWindow4BitTo8Bit(u8 windowId, u8 *pixels, u16 srcX, u16 srcY, u16 srcWidth, int srcHeight, u16 destX, u16 destY, u16 rectWidth, u16 rectHeight, u8 paletteNum);
void CopyWindowToVram8Bit(u8 windowId, u8 mode);
*/
//extern void* gWindowBgTilemapBuffers[];
//extern struct Window gWindows[];
