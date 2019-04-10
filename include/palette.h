#ifndef GUARD_PALETTE_H
#define GUARD_PALETTE_H

#include "global.h"

#include "global.h"

#define gPaletteFade_selectedPalettes (gPaletteFade.multipurpose1) // normal and fast fade
#define gPaletteFade_blendCnt         (gPaletteFade.multipurpose1) // hardware fade
#define gPaletteFade_delay            (gPaletteFade.multipurpose2) // normal and hardware fade
#define gPaletteFade_submode          (gPaletteFade.multipurpose2) // fast fade

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

/*
extern struct PaletteFadeControl gPaletteFade;
extern u32 gPlttBufferTransferPending;
extern u8 gPaletteDecompressionBuffer[];
extern u16 gPlttBufferUnfaded[PLTT_BUFFER_SIZE];
extern u16 gPlttBufferFaded[PLTT_BUFFER_SIZE];
*/

void __attribute__((long_call)) LoadCompressedPalette(const void *, u16, u16);
void __attribute__((long_call)) LoadPalette(const void *, u16, u16);
void __attribute__((long_call)) FillPalette(u16, u16, u16);
//void __attribute__((long_call)) TransferPlttBuffer(void);	// redone in DNS.c
u8 __attribute__((long_call)) UpdatePaletteFade(void);
void __attribute__((long_call)) ResetPaletteFade(void);
void __attribute__((long_call)) ReadPlttIntoBuffers(void);
bool8 __attribute__((long_call)) BeginNormalPaletteFade(u32, s8, u8, u8, u16);
bool8 __attribute__((long_call)) unref_sub_8073D3C(u32, u8, u8, u8, u16);
void __attribute__((long_call)) unref_sub_8073D84(u8, u32 *);
void __attribute__((long_call)) ResetPaletteStructByUid(u16);
void __attribute__((long_call)) ResetPaletteStruct(u8);
void __attribute__((long_call)) ResetPaletteFadeControl();
void __attribute__((long_call)) unref_sub_8074168(u16);
void __attribute__((long_call)) unref_sub_8074194(u16);
void __attribute__((long_call)) InvertPlttBuffer(u32);
void __attribute__((long_call)) TintPlttBuffer(u32, s8, s8, s8);
void __attribute__((long_call)) UnfadePlttBuffer(u32);
void __attribute__((long_call)) BeginFastPaletteFade(u8);
void __attribute__((long_call)) BeginHardwarePaletteFade(u8, u8, u8, u8, u8);
void __attribute__((long_call)) BlendPalettes(u32, u8, u16);
void __attribute__((long_call)) BlendPalettesUnfaded(u32, u8, u16);
void __attribute__((long_call)) sub_80716F8(const u16 *, u16 *, u16, u8);

// extern struct PaletteFadeControl gPaletteFade;

#endif // GUARD_PALETTE_H
