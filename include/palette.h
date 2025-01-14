#pragma once

#include "global.h"

#define gPaletteFade_selectedPalettes (gPaletteFade->multipurpose1) // normal and fast fade
#define gPaletteFade_blendCnt         (gPaletteFade->multipurpose1) // hardware fade
#define gPaletteFade_delay            (gPaletteFade->multipurpose2) // normal and hardware fade
#define gPaletteFade_submode          (gPaletteFade->multipurpose2) // fast fade

#define PLTT_BUFFER_SIZE 0x200
#define PLTT_DECOMP_BUFFER_SIZE (PLTT_BUFFER_SIZE * 2)

enum
{
    NORMAL_FADE,
    FAST_FADE,
    HARDWARE_FADE,
};

enum
{
    FAST_FADE_IN_FROM_WHITE,
    FAST_FADE_OUT_TO_WHTIE,
    FAST_FADE_IN_FROM_BLACK,
    FAST_FADE_OUT_TO_BLACK,
};

struct PaletteFadeControl
{
    u32 multipurpose1;
    u8 delayCounter:6;
    u16 y:5; // blend coefficient
    u16 targetY:5; // target blend coefficient
    u16 blendColor:15;
    u16 active:1;
    u16 multipurpose2:6;
    u16 yDec:1; // whether blend coefficient is decreasing
    u16 bufferTransferDisabled:1;
    u16 mode:2;
    u16 shouldResetBlendRegisters:1;
    u16 hardwareFadeFinishing:1;
    u16 softwareFadeFinishingCounter:5;
    u16 softwareFadeFinishing:1;
    u16 objPaletteToggle:1;
    u8 deltaY:4; // rate of change of blend coefficient
};


extern struct PaletteFadeControl gPaletteFade;
extern u32 gPlttBufferTransferPending;
extern u8 gPaletteDecompressionBuffer[];
/*extern u16 gPlttBufferUnfaded[PLTT_BUFFER_SIZE];
extern u16 gPlttBufferFaded[PLTT_BUFFER_SIZE];
*/

void __attribute__((long_call)) BlendPalette(u16 palOffset, u16 numEntries, u8 coeff, u16 blendColor);
void __attribute__((long_call)) LoadCompressedPalette(const void* src, u16 offset, u16 size);
void __attribute__((long_call)) LoadPalette(const void *src, u16 offset, u16 size);
void __attribute__((long_call)) FillPalette(u16 value, u16 offset, u16 size);
//void __attribute__((long_call)) TransferPlttBuffer(void);
u8 __attribute__((long_call)) UpdatePaletteFade(void);
void __attribute__((long_call)) ResetPaletteFade(void);
void __attribute__((long_call)) ReadPlttIntoBuffers(void);
bool8 __attribute__((long_call)) BeginNormalPaletteFade(u32 selectedPalettes, s8 delay, u8 startY, u8 targetY, u16 blendColor);
void __attribute__((long_call)) FadeBgPalAndFillBlack(void);
void __attribute__((long_call)) ResetPaletteStructByUid(u16 a1);
void __attribute__((long_call)) ResetPaletteStruct(u8 paletteNum);
void __attribute__((long_call)) ResetPaletteFadeControl(void);
u8 __attribute__((long_call)) GetPaletteNumByUid(u16 uid);
u8 __attribute__((long_call)) UpdateNormalPaletteFade(void);
void __attribute__((long_call)) InvertPlttBuffer(u32 selectedPalettes);
void __attribute__((long_call)) TintPlttBuffer(u32 selectedPalettes, s8 r, s8 g, s8 b);
void __attribute__((long_call)) UnfadePlttBuffer(u32 selectedPalettes);
void __attribute__((long_call)) BeginFastPaletteFade(u8 submode);
void __attribute__((long_call)) BeginFastPaletteFadeInternal(u8 submode);
u8 __attribute__((long_call)) UpdateFastPaletteFade(void);
void __attribute__((long_call)) BeginHardwarePaletteFade(u8 blendCnt, u8 delay, u8 y, u8 targetY, u8 shouldResetBlendRegisters);
u8 __attribute__((long_call)) UpdateHardwarePaletteFade(void);
void __attribute__((long_call)) UpdateBlendRegisters(void);
bool8 __attribute__((long_call)) IsSoftwarePaletteFadeFinishing(void);
void __attribute__((long_call)) BlendPalettes(u32 selectedPalettes, u8 coeff, u16 color);
void __attribute__((long_call)) BlendPalettesUnfaded(u32 selectedPalettes, u8 coeff, u16 color);
void __attribute__((long_call)) TintPalette_GrayScale(u16 *palette, u16 count);
void __attribute__((long_call)) TintPalette_GrayScale2(u16 *palette, u16 count);
void __attribute__((long_call)) TintPalette_SepiaTone(u16 *palette, u16 count);
void __attribute__((long_call)) TintPalette_CustomTone(u16 *palette, u16 count, u16 rTone, u16 gTone, u16 bTone);
void __attribute__((long_call)) sub_80716F8(const u16 *src, u16 *dst, u16 count, u8 a4);
void __attribute__((long_call)) sub_80717A8(u32 a1, s8 a2, u8 a3, u8 a4, u16 a5, u8 a6, u8 a7);
bool32 __attribute__((long_call)) sub_807185C(u8 var);

// extern struct PaletteFadeControl gPaletteFade;
