#include "defines.h"
#include "helper_functions.h"
/*
Credit to Navenatox
*/


//#define RGB(R, G, B)	(R | G << 5 | B << 10)
#define Red(Color)		((Color) & 31)
#define Green(Color)	((Color >> 5) & 31)
#define Blue(Color)		((Color >> 10) & 31)

#define LoadOBJPalette(PalAddress, StartColor) ((void(*)(Palette*, u16))0x80089A0+1)(PalAddress, StartColor)
#define LoadNPCPalette(PalTag, PalSlot) ((void(*)(u16, u8))0x805F538+1)(PalTag, PalSlot)
#define FadePalette(StartColor, ColorCount, FadeIntensity, FadeColor) ((void(*)(u16, u16, u8, u16))0x8045274+1)(StartColor, ColorCount, FadeIntensity, FadeColor)
#define TintOBJPalette(PalSlot) ((void(*)(u8))0x8083598+1)(PalSlot)
#define TintGrayscale(Colors, ColorCount) ((void(*)(u16*, u16))0x8071544+1)(Colors, ColorCount)
#define TintSepia(Colors, ColorCount) ((void(*)(u16*, u16))0x80715F4+1)(Colors, ColorCount)

#define TaskIsRunning(Func) ((u8(*)(TaskFunc))0x8077650+1)(Func)
#define TaskOverworld ((TaskFunc)0x8079E0C+1)
#define OverworldIsActive TaskIsRunning(TaskOverworld)

#define SetUpDisguise(Arg1, Arg2, PalSlot) ((u8(*)(u8, u8, u8))0x80DCA10+1)(Arg1, Arg2, PalSlot)

#define CpuSetFill (1 << 24)

#define PalTypeUnused 0
#define PalTypeNPC 1
#define PalTypeAnimation 2
#define PalTypeWeather 3
#define PalTypeReflection 4
#define PalTypeOther 5

// #define PalRefs ((PalRef*)0x203FF00)	// moved to ram_locs
#define PalTags ((u16*)0x3000DE8)

#define Palettes ((Palette*)0x20375F8)
#define UnfadedPalettes ((Palette*)0x20371F8)

#define PalTagsStart *((u8*)0x3003E58)
#define ColorFilter *((u8*)0x2036E28)
#define FadeInfo2 ((void*)0x2037F34+0x6C0)
#define AlphaBlendingCoeffA *((u8*)0x3000052)

typedef struct
{
	u8 Type;
	u8 Count;
	u16 PalTag;
} PalRef;

typedef struct
{
	u16 Colors[16];
} Palette;

typedef struct
{
	Palette *Address;
	u16 PalTag;
} PalInfo;

typedef void(*TaskFunc)(u8);


u8 AddPalRef(u8 Type, u16 PalTag)
{
	int i;
	for (i = 0; i < 16; i++)
	{
		if (PalRefs[i].Type == PalTypeUnused)
		{
			PalRefs[i].Type = Type;
			PalRefs[i].PalTag = PalTag;
			return i;
		}
	}
	return 0xFF; // no more space
};


u8 FindPalRef(u8 Type, u16 PalTag)
{
	int i;
	for (i = 0; i < 16; i++)
	{
		if (PalRefs[i].Type == Type && PalRefs[i].PalTag == PalTag) return i;
	}
	return 0xFF; // not found
};


u8 GetFadeTypeByWeather(u8 Weather)
{
	switch (Weather)
	{
		case 0:
		case 1:
		case 2:
		case 7:
		case 8:
		default:
			return 0; // normal fade
		case 3:
		case 4:
		case 5:
		case 11:
		case 13:
			return 1; // fade and darken
		case 6:
		case 9:
		case 10:
		case 14:
			return 2; // fade and brighten
		case 12:
			return 3; // harsh sunlight
	}
};


u16 TintColor(u16 Color)
{
	switch (ColorFilter)
	{
		case 1:
		case 3:
			TintGrayscale(&Color, 1);
			break;
		case 2:
			TintSepia(&Color, 1);
			break;
	}
	return Color;
};


u8 PaletteNeedsFogBrightening(u8 PalSlot) // hook at 0x7A748
{
	return PalRefs[PalSlot - 16].Type == PalTypeAnimation; // only brighten animations
};

u8 GetPalTypeByPalTag(u16 PalTag)
{
	if (PalTag >= 0x1000 && PalTag <= 0x1010)
		return PalTypeAnimation;
	if (PalTag == 0x1200)
		return PalTypeWeather;
	return PalTypeOther;
};

u8 FindPalTag(u16 PalTag)
{
	int i;
	if (PalTagsStart >= 16)
		return 0xFF;
	for (i = PalTagsStart; i < 16; i++)
	{
		if (PalTags[i] == PalTag)
			return i;
	}
	return 0xFF; // not found
};

u8 FindPalette(u16 PalTag) // hook at 0x89E8 via r1
{
	if (OverworldIsActive || PalTag == 0x1200)
		return FindPalRef(GetPalTypeByPalTag(PalTag), PalTag); // 0x1200 is for weather sprites
	else
		return FindPalTag(PalTag);
};

u8 PalRefIncreaseCount(u8 PalSlot)
{
	PalRefs[PalSlot].Count++;
	return PalSlot;
};


void PalRefDecreaseCount(u8 PalSlot)
{
	if (PalRefs[PalSlot].Count != 0)
		PalRefs[PalSlot].Count--;
	if (PalRefs[PalSlot].Count == 0)
	{
		PalRefs[PalSlot].Type = 0;
		PalRefs[PalSlot].PalTag = 0;
	}
};


void ClearAllPalRefs(void)
{
	int Fill = 0;
	CpuSet(&Fill, PalRefs, 32 | CpuSetFill);
};


void ClearAllPalettes(void) // hook at 0x5F574 via r0
{
	int Fill = 0;
	CpuSet(&Fill, &UnfadedPalettes[16], 256 | CpuSetFill);
};

void BrightenReflection(u8 PalSlot)
{
	Palette *Pal = &Palettes[PalSlot + 16];
	u16 Color;
	u8 R, G, B;
	int i;
	for (i = 0; i < 16; i++)
	{
		Color = Pal->Colors[i];
		R = Red(Color) + 5;
		G = Green(Color) + 5;
		B = Blue(Color) + 10;
		if (R > 31) R = 31;
		if (G > 31) G = 31;
		if (B > 31) B = 31;
		Pal->Colors[i] = RGB(R, G, B);
	}
	CpuSet(Pal, &UnfadedPalettes[PalSlot + 16], 16);
};



u8 AddPalTag(u16 PalTag)
{
	int i;
	if (PalTagsStart >= 16)
		return 0xFF;
	for (i = PalTagsStart; i < 16; i++)
	{
		if (PalTags[i] == 0xFFFF)
		{
			PalTags[i] = PalTag;
			return i;
		}
	}
	return 0xFF; // no more space
};



u8 FindOrLoadPalette(PalInfo *Pal) // hook at 0x8928 via r1
{
	u8 PalSlot;
	u16 PalTag = Pal->PalTag;
	if (OverworldIsActive || PalTag == 0x1200) // 0x1200 is for weather sprites
	{
		PalSlot = FindPalRef(GetPalTypeByPalTag(PalTag), PalTag);
		if (PalSlot != 0xFF)
			return PalSlot;
		PalSlot = AddPalRef(GetPalTypeByPalTag(PalTag), PalTag);
		if (PalSlot == 0xFF)
			return 0xFF;
	}
	else
	{
		PalSlot = FindPalTag(PalTag);
		if (PalSlot != 0xFF)
			return PalSlot;
		PalSlot = AddPalTag(PalTag);
		if (PalSlot == 0xFF)
			return 0xFF;
	}
	LoadOBJPalette(Pal->Address, PalSlot * 16);
	return PalSlot;
};


void MaskPaletteIfFadingIn(u8 PalSlot) // prevent the palette from flashing briefly before fading starts
{
	u8 FadeState = *(u8*)(FadeInfo2 + 6);
	u8 AboutToFadeIn = *(u8*)(FadeInfo2 + 10);
	if (FadeState == 1 && AboutToFadeIn)
	{
		u16 FadeColor = *(u16*)(FadeInfo2 + 4);
		CpuSet(&FadeColor, &Palettes[PalSlot + 16], 16 | CpuSetFill);
	}
};


u8 GetPalSlotMisc(u32 OBJData)
{
	u8 PalSlot;
	u16 PalTag = *(u16*)(OBJData + 2);
	if (PalTag == 0xFFFF)
		return 0xFF;
	PalSlot = FindPalette(PalTag);
	if (PalSlot != 0xFF)
		return PalRefIncreaseCount(PalSlot);
	if (PalTag != 0x1200)
		return 0xFF;

	// load the rain palette
	PalSlot = AddPalRef(PalTypeWeather, PalTag);
	if (PalSlot == 0xFF)
		return 0xFF;
	LoadOBJPalette((Palette*)0x83C2CE0, PalSlot * 16);
	TintOBJPalette(PalSlot);
	MaskPaletteIfFadingIn(PalSlot);
	return PalRefIncreaseCount(PalSlot);
};


u8 FindOrLoadNPCPalette(u16 PalTag)
{
	u8 PalSlot;
	PalSlot = FindPalRef(PalTypeNPC, PalTag);
	if (PalSlot != 0xFF)
		return PalRefIncreaseCount(PalSlot);
	PalSlot = AddPalRef(PalTypeNPC, PalTag);
	if (PalSlot == 0xFF)
		return PalRefIncreaseCount(0);
	LoadNPCPalette(PalTag, PalSlot);
	MaskPaletteIfFadingIn(PalSlot);
	return PalRefIncreaseCount(PalSlot);
};


u8 FindOrCreateReflectionPalette(u8 PalSlotNPC)
{
	u8 PalSlot;
	u16 PalTag = PalRefs[PalSlotNPC].PalTag;
	PalSlot = FindPalRef(PalTypeReflection, PalTag);
	if (PalSlot != 0xFF)
		return PalRefIncreaseCount(PalSlot);
	PalSlot = AddPalRef(PalTypeReflection, PalTag);
	if (PalSlot == 0xFF)
		return PalRefIncreaseCount(0);
	LoadNPCPalette(PalTag, PalSlot);
	FadePalette((PalSlot + 16) * 16, 16, 6, RGB(12, 20, 27)); // make it blueish
	BrightenReflection(PalSlot); // and a little brighter
	TintOBJPalette(PalSlot);
	MaskPaletteIfFadingIn(PalSlot);
	return PalRefIncreaseCount(PalSlot);
};


u8 SetUpTreeDisguise(void) // hook at 0xDC9E0 via r3
{
	return SetUpDisguise(0x1C, 0x18, FindOrLoadNPCPalette(0x1105));
};


u8 SetUpRockDisguise(void) // hook at 0xDC9F0 via r3
{
	return SetUpDisguise(0x1D, 0x19, FindOrLoadNPCPalette(0x1104));
};


u8 SetUpWeirdDisguise(void) // hook at 0xDCA00 via r3
{
	return SetUpDisguise(0x24, 0x1C, FindOrLoadNPCPalette(0x1103));
};



void FogBrightenPalettes(u16 BrightenIntensity)
{
	u8 Weather = *(u8*)(FadeInfo2 + 16);
	if (GetFadeTypeByWeather(Weather) != 2)
		return; // only brighten if there is fog weather
	u8 FadeState = *(u8*)(FadeInfo2 + 6);
	if (FadeState != 3)
		return; // don't brighten while fading
	u16 BrightenColor = TintColor(RGB(28, 31, 28));
	int i;
	for (i = 16; i < 32; i++)
	{
		if (PaletteNeedsFogBrightening(i)) FadePalette(i * 16, 16, BrightenIntensity, BrightenColor);
	}
};


void FogBrightenAndFade(u8 PalSlot, u8 FadeIntensity, u16 FadeColor)
{
	u8 BrightenIntensity = AlphaBlendingCoeffA;
	u16 BrightenColor = TintColor(RGB(28, 31, 28));
	u16 Color;
	u8 R, G, B;
	int i;
	for (i = 0; i < 16; i++)
	{
		Color = UnfadedPalettes[PalSlot].Colors[i];
		R = Red(Color);
		G = Green(Color);
		B = Blue(Color);

		R += (Red(BrightenColor) - R) *  BrightenIntensity / 16;
		G += (Green(BrightenColor) - G) *  BrightenIntensity / 16;
		B += (Blue(BrightenColor) - B) *  BrightenIntensity / 16;

		R += (Red(FadeColor) - R) *  FadeIntensity / 16;
		G += (Green(FadeColor) - G) *  FadeIntensity / 16;
		B += (Blue(FadeColor) - B) *  FadeIntensity / 16;

		Palettes[PalSlot].Colors[i] = RGB(R, G, B);
	}
};


void LoadCloudOrSandstormPalette(Palette *Pal) // hook at 0x7ABC0 via r1
{
	u8 PalSlot;
	PalSlot = AddPalRef(PalTypeWeather, 0x1200);
	if (PalSlot == 0xFF)
		return;
	LoadOBJPalette(Pal, PalSlot * 16);
	TintOBJPalette(PalSlot);
	MaskPaletteIfFadingIn(PalSlot);
};



u8 GetDarkeningTypeBySlot(u8 PalSlot) // replaces table at 0x3C2CC0
{
	if (PalSlot < 13)
		return 1;
	if (PalSlot < 16)
		return 0;
	u8 Type = PalRefs[PalSlot - 16].Type;
	if (Type == PalTypeNPC || Type == PalTypeWeather)
		return 2;
	if (Type == PalTypeAnimation || Type == PalTypeReflection)
		return 1;
	return 0;
};



