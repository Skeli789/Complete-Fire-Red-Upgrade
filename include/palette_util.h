#ifndef GUARD_PALETTE_UTIL_H
#define GUARD_PALETTE_UTIL_H

struct PulseBlendSettings
{
	u16 blendColor;
	u16 paletteOffset;
	u8 numColors;
	u8 delay;
	u8 numFadeCycles;
	s8 maxBlendCoeff:4;
	s8 fadeType:2;
	s8 restorePaletteOnUnload:1;
	s8 notDuringPalFade:1;
};

struct PulseBlendPalette
{
	u8 paletteSelector;
	u8 blendCoeff:4;
	u8 fadeDirection:1;
	s8 unk1_5:1;
	s8 available:1;
	u32 inUse:1;
	u8 delayCounter;
	u8 fadeCycleCounter;
	struct PulseBlendSettings pulseBlendSettings;
};

struct PulseBlend
{
	u16 usedPulseBlendPalettes;
	struct PulseBlendPalette pulseBlendPalettes[16];
};

int __attribute__((long_call)) InitPulseBlendPaletteSettings(struct PulseBlend *, const struct PulseBlendSettings *);
void __attribute__((long_call)) InitPulseBlend(struct PulseBlend *);
void __attribute__((long_call)) MarkUsedPulseBlendPalettes(struct PulseBlend *, u16, u8);
void __attribute__((long_call)) UnloadUsedPulseBlendPalettes(struct PulseBlend *, u16, u8);
void __attribute__((long_call)) UnmarkUsedPulseBlendPalettes(struct PulseBlend *, u16, u8);
void __attribute__((long_call)) UpdatePulseBlend(struct PulseBlend *);

#endif // GUARD_PALETTE_UTIL_H
