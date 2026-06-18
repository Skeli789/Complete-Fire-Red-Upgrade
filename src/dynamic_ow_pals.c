#include "defines.h"
#include "../include/event_object_movement.h"
#include "../include/field_weather.h"
#include "../include/constants/field_effects.h"

#include "../include/new/character_customization.h"
#include "../include/new/dynamic_ow_pals.h"
#include "../include/new/util.h"

/*
dynamic_ow_pals.c
	handles the dynamic loading and tracking of overworld sprite palettes
Credit to Navenatox
*/
//#define RGB(R, G, B)	(R | G << 5 | B << 10)
#define Red(Color)		((Color) & 31)
#define Green(Color)	((Color >> 5) & 31)
#define Blue(Color)		((Color >> 10) & 31)

#ifdef PALETTE_SWAPPER
#define OBJ_EVENT_PAL_TAG_NONE 0x11FF
#else
#define LoadNPCPalette(PalTag, PalSlot) ((void(*)(u16, u8)) (0x805F538 | 1))(PalTag, PalSlot)
#endif
#define TintOBJPalette(PalSlot) ((void(*)(u8))0x8083598+1)(PalSlot)

#define OverworldIsActive FuncIsActiveTask(Task_WeatherMain)

#define SetUpDisguise(Arg1, Arg2, PalSlot) ((u8(*)(u8, u8, u8))0x80DCA10+1)(Arg1, Arg2, PalSlot)

#define CpuSetFill (1 << 24)

#define FOG_FADE_COLOUR TintColor(RGB(28, 31, 28))
#define FOG_BRIGHTEN_INTENSITY 12

struct PalRef
{
	u8 Type;
	u8 Count;
	u16 PalTag;
};

#define sPalRefs ((struct PalRef*) 0x203B7D4) //Make sure to change reference in BT scripts if modified

//This file's functions:
static u16 TintColor(u16 color);
static u8 GetPalTypeByPalTag(u16 palTag);
static u8 FindPalTag(u16 PalTag);
static u8 PalRefIncreaseCount(u8 palSlot);
static void BrightenReflection(u8 palSlot);
static u8 AddPalTag(u16 palTag);
static void MaskPaletteIfFadingIn(u8 palSlot);

u8 AddPalRef(u8 type, u16 palTag)
{
	for (int i = 0; i < 16; i++)
	{
		if (sPalRefs[i].Type == PalTypeUnused)
		{
			sPalRefs[i].Type = type;
			sPalRefs[i].PalTag = palTag;
			return i;
		}
	}

	return 0xFF; //No more space
}

u8 FindPalRef(u8 type, u16 palTag)
{
	for (int i = 0; i < 16; i++)
	{
		if (sPalRefs[i].Type == type && sPalRefs[i].PalTag == palTag)
			return i;
	}

	return 0xFF; // not found
}

void RemovePalRef(u16 palTag)
{
	for (int i = 0; i < 16; i++)
	{
		if (sPalRefs[i].PalTag == palTag)
		{
			sPalRefs[i].Type = 0;
			sPalRefs[i].PalTag = 0;
			sPalRefs[i].Count = 0;
		}
	}
}

u8 GetPalTypeByPaletteOffset(u16 offset)
{
	if (&gPlttBufferUnfaded[offset] < gPlttBufferUnfaded2)
		return 0;

	u8 palSlot = (u32) (&gPlttBufferUnfaded[offset] - gPlttBufferUnfaded2) / 16;
	return sPalRefs[palSlot].Type;
}

u8 GetFadeTypeByWeather(u8 weather)
{
	switch (weather)
	{
		case WEATHER_NONE:
		case WEATHER_CLOUDS:
		case WEATHER_SUNNY:
		case WEATHER_SNOW:
		case WEATHER_STEADY_SNOW:
		case WEATHER_SANDSTORM:
		default:
			return 0; //Normal fade

		case WEATHER_RAIN_LIGHT:
		case WEATHER_RAIN_MED:
		case WEATHER_SHADE:
		case 13:
			return 1; //Fade and darken

		case WEATHER_FOG_1:
		case WEATHER_FOG_2:
		case WEATHER_FOG_3:
		case WEATHER_BUBBLES:
			return 2; //Fade and brighten

		case WEATHER_DROUGHT:
			return 3; //Harsh sunlight
	}
}

u8 GetFadeTypeForCurrentWeather(void)
{
	return GetFadeTypeByWeather(gWeatherPtr->currWeather);
}

static u16 TintColor(u16 color)
{
	switch (ColorFilter)
	{
		case 1:
		case 3:
			TintPalette_GrayScale(&color, 1);
			break;
		case 2:
			TintPalette_SepiaTone(&color, 1);
			break;
	}

	return color;
}


u8 PaletteNeedsFogBrightening(u8 palSlot) // hook at 0x7A748
{
	palSlot = palSlot - 16;

	return sPalRefs[palSlot].Type == PalTypeReflection
		#ifdef FADE_NPCS_IN_FOG
		|| sPalRefs[palSlot].Type == PalTypeNPC
		#endif
		|| sPalRefs[palSlot].Type == PalTypeAnimation;
}

static u8 GetPalTypeByPalTag(u16 palTag)
{
	if (palTag == FLDEFF_PAL_TAG_SMALL_SPARKLE)
		return PalTypeOther; //Small sparkle should never be faded by DNS

	if (palTag >= 0x1000 && palTag <= 0x1012)
		return PalTypeAnimation;

	#ifdef UNBOUND
	if (palTag == 0x1115) //For the boat
		return PalTypeNPC; 
	#endif
	
	if (palTag == 0x1200)
		return PalTypeWeather;

	return PalTypeOther;
}

static u8 FindPalTag(u16 palTag)
{
	if (PalTagsStart >= 16)
		return 0xFF;

	for (int i = PalTagsStart; i < 16; ++i)
	{
		if (PalTags[i] == palTag)
			return i;
	}

	return 0xFF; //Not found
}

u8 FindPalette(u16 palTag) //Hook at 0x80089E8 via r1
{
	if (OverworldIsActive || palTag == 0x1200)
		return FindPalRef(GetPalTypeByPalTag(palTag), palTag); // 0x1200 is for weather sprites

	return FindPalTag(palTag);
}

static u8 PalRefIncreaseCount(u8 palSlot)
{
	sPalRefs[palSlot].Count++;
	return palSlot;
}

void PalRefDecreaseCount(u8 palSlot)
{
	if (sPalRefs[palSlot].Count != 0)
		sPalRefs[palSlot].Count--;
	if (sPalRefs[palSlot].Count == 0)
	{
		sPalRefs[palSlot].Type = 0;
		sPalRefs[palSlot].PalTag = 0;
	}
}

void ClearAllPalRefs(void)
{
	int fill = 0;
	CpuSet(&fill, sPalRefs, 32 | CpuSetFill);
}

void ClearAllPalettes(void) //Hook at 0x5F574 via r0
{
	int fill = 0;
	CpuSet(&fill, &gPlttBufferUnfaded[16 * 16], 256 | CpuSetFill);
}

static void BrightenReflection(u8 palSlot)
{
	u8 R, G, B;
	u16 color;
	u16* pal = &gPlttBufferFaded[palSlot * 16 + 16 * 16];

	for (int i = 0; i < 16; ++i)
	{
		color = pal[i];
		R = Red(color) + 5;
		G = Green(color) + 5;
		B = Blue(color) + 10;
		if (R > 31) R = 31;
		if (G > 31) G = 31;
		if (B > 31) B = 31;
		pal[i] = RGB(R, G, B);
	}

	CpuSet(pal, &gPlttBufferUnfaded[palSlot * 16 + 16 * 16], 16);
}

static u8 AddPalTag(u16 palTag)
{
	if (PalTagsStart >= 16)
		return 0xFF;

	for (int i = PalTagsStart; i < 16; ++i)
	{
		if (PalTags[i] == 0xFFFF)
		{
			PalTags[i] = palTag;
			return i;
		}
	}

	return 0xFF; // no more space
}

u8 FindOrLoadPalette(struct SpritePalette* pal) //Hook at 0x8928 via r1
{
	u8 palSlot;
	u16 palTag = pal->tag;

	if (OverworldIsActive
	#ifdef UNBOUND
	|| palTag == 0x1115 //For the black boat
	#endif
	|| palTag == 0x1200) //0x1200 is for weather sprites
	{
		palSlot = FindPalRef(GetPalTypeByPalTag(palTag), palTag);
		if (palSlot != 0xFF)
			return palSlot;

		palSlot = AddPalRef(GetPalTypeByPalTag(palTag), palTag);
		if (palSlot == 0xFF)
			return 0xFF;
	}
	else
	{
		palSlot = FindPalTag(palTag);
		if (palSlot != 0xFF)
			return palSlot;

		palSlot = AddPalTag(palTag);
		if (palSlot == 0xFF)
			return 0xFF;
	}

	DoLoadSpritePalette(pal->data, palSlot * 16);
	return palSlot;
}

static void MaskPaletteIfFadingIn(u8 palSlot) //Prevent the palette from flashing briefly before fading starts
{
	u8 fadeState = gWeatherPtr->palProcessingState;
	u8 aboutToFadeIn = gWeatherPtr->fadeInFirstFrame;

	if (fadeState == 1 && aboutToFadeIn)
	{
		u16 fadeColor = gWeatherPtr->fadeDestColor;
		CpuSet(&fadeColor, &gPlttBufferFaded[palSlot * 16 + 16 * 16], 16 | CpuSetFill);
	}
}

u8 GetPalSlotMisc(u32 OBJData)
{
	u8 palSlot;
	u16 palTag = *(u16*)(OBJData + 2);
	if (palTag == 0xFFFF)
		return 0xFF;

	palSlot = FindPalette(palTag);
	if (palSlot != 0xFF)
		return PalRefIncreaseCount(palSlot);

	if (palTag != 0x1200)
		return 0xFF;

	//Load the rain palette
	palSlot = AddPalRef(PalTypeWeather, palTag);
	if (palSlot == 0xFF)
		return 0xFF;

	DoLoadSpritePalette((u16*) 0x83C2CE0, palSlot * 16);
	TintOBJPalette(palSlot);
	MaskPaletteIfFadingIn(palSlot);
	return PalRefIncreaseCount(palSlot);
}

#ifdef PALETTE_SWAPPER
void LoadObjectEventPalette(u16 paletteTag) {
	u16 i = FindObjectEventPaletteIndexByTag(paletteTag);
	const struct SpritePalette* palettes = gObjectEventSpritePalettesSwitcher[GetObjectEventPalettesTableByTag(paletteTag)];
    if (palettes[i].tag != OBJ_EVENT_PAL_TAG_NONE)
    {
        TryLoadObjectPalette(&palettes[i]);
    }
}
void PatchObjectPalette(u16 paletteTag, u8 paletteSlot) {
	u8 paletteIndex = FindObjectEventPaletteIndexByTag(paletteTag);
	const struct SpritePalette* palettes = gObjectEventSpritePalettesSwitcher[GetObjectEventPalettesTableByTag(paletteTag)];
    LoadPalette(palettes[paletteIndex].data, 16 * paletteSlot + 0x100, 0x20);
    ApplyGlobalFieldPaletteTint(paletteSlot);
}

u8 FindObjectEventPaletteIndexByTag(u16 tag)
{
    u8 i;
	const struct SpritePalette* palettes = gObjectEventSpritePalettesSwitcher[GetObjectEventPalettesTableByTag(tag)];
    for (i = 0; palettes[i].tag != OBJ_EVENT_PAL_TAG_NONE; i++)
    {
        if (palettes[i].tag == tag)
        {
            return i;
        }
    }
    return 0xFF;
}

u8 GetObjectEventPalettesTableByTag(u16 tag)
{
	u8 paletteTable = (tag >> 8) & 0xFF;
	if (gObjectEventSpritePalettesSwitcher[paletteTable] != 0)
		return paletteTable;
	else
		return 11;
}
#endif

u8 FindOrLoadNPCPalette(u16 palTag)
{
	u8 palSlot = FindPalRef(PalTypeNPC, palTag);
	if (palSlot != 0xFF)
		return PalRefIncreaseCount(palSlot);

	palSlot = AddPalRef(PalTypeNPC, palTag);
	if (palSlot == 0xFF)
		return PalRefIncreaseCount(0);

	#ifdef PALETTE_SWAPPER
	PatchObjectPalette(palTag, palSlot);
	#else
	LoadNPCPalette(palTag, palSlot);
	#endif
	#ifdef UNBOUND
	if (IsPaletteTagAffectedByCharacterCustomization(palTag))
		ChangeEventObjPal(0x100 + palSlot * 16, palTag);
	#endif
	FogBrightenPalettes(FOG_BRIGHTEN_INTENSITY);
	MaskPaletteIfFadingIn(palSlot);
	return PalRefIncreaseCount(palSlot);
}

u8 FindOrCreateReflectionPalette(u8 palSlotNPC)
{
	u16 palTag = sPalRefs[palSlotNPC].PalTag;
	u8 palSlot = FindPalRef(PalTypeReflection, palTag);
	if (palSlot != 0xFF)
		return PalRefIncreaseCount(palSlot);

	palSlot = AddPalRef(PalTypeReflection, palTag);
	if (palSlot == 0xFF)
		return PalRefIncreaseCount(0);

	#ifdef PALETTE_SWAPPER
	PatchObjectPalette(palTag, palSlot);
	#else
	LoadNPCPalette(palTag, palSlot);
	#endif
	#ifdef UNBOUND
	if (IsPaletteTagAffectedByCharacterCustomization(palTag))
		ChangeEventObjPal(0x100 + palSlot * 16, palTag);
	#endif
	BlendPalettes(gBitTable[(palSlot + 16)], 6, RGB(12, 20, 27)); //Make it blueish
	BrightenReflection(palSlot); //And a little brighter
	TintOBJPalette(palSlot);
	MaskPaletteIfFadingIn(palSlot);
	return PalRefIncreaseCount(palSlot);
}


u8 SetUpTreeDisguise(void) //Hook at 0xDC9E0 via r3
{
	return SetUpDisguise(0x1C, 0x18, FindOrLoadNPCPalette(TREE_DISGUISE_PAL_ID));
}


u8 SetUpRockDisguise(void) //Hook at 0xDC9F0 via r3
{
	return SetUpDisguise(0x1D, 0x19, FindOrLoadNPCPalette(ROCK_DISGUISE_PAL_ID));
}


u8 SetUpWeirdDisguise(void) //Hook at 0xDCA00 via r3
{
	return SetUpDisguise(0x24, 0x1C, FindOrLoadNPCPalette(WEIRD_DISGUISE_PAL_ID));
}

void FogBrightenPalettes(u16 brightenIntensity)
{
	u8 currWeather = gWeatherPtr->currWeather;
	if (GetFadeTypeByWeather(currWeather) != 2)
		return; //Only brighten if there is fog weather and not underwater

	if (gWeatherPtr->palProcessingState != 3)
		return; // don't brighten while fading

	for (int i = 16; i < 32; i++)
	{
		if (PaletteNeedsFogBrightening(i))
		{
			if (currWeather == WEATHER_BUBBLES)
				FogBrightenAndFade(i, 0x1, RGB(15, 16, 14));
			else
				BlendPalette(i * 16, 16, brightenIntensity, FOG_FADE_COLOUR);
		}
	}
}

void FogBrightenAndFade(u8 palSlot, u8 fadeIntensity, u16 fadeColor)
{
	u8 R, G, B;
	u16 color;
	u8 brightenIntensity = AlphaBlendingCoeffA;
	u16 brightenColor = FOG_FADE_COLOUR;

	for (int i = 0; i < 16; i++)
	{
		color = gPlttBufferUnfaded[palSlot * 16 + i];
		R = Red(color);
		G = Green(color);
		B = Blue(color);

		R += (Red(brightenColor) - R) *  brightenIntensity / 16;
		G += (Green(brightenColor) - G) *  brightenIntensity / 16;
		B += (Blue(brightenColor) - B) *  brightenIntensity / 16;

		R += (Red(fadeColor) - R) *  fadeIntensity / 16;
		G += (Green(fadeColor) - G) *  fadeIntensity / 16;
		B += (Blue(fadeColor) - B) *  fadeIntensity / 16;

		gPlttBufferFaded[palSlot * 16 + i] = RGB(R, G, B);
	}
}

void LoadCloudOrSandstormPalette(u16* pal) //Hook at 0x7ABC0 via r1
{
	u8 palSlot;
	palSlot = AddPalRef(PalTypeWeather, 0x1200);
	if (palSlot == 0xFF)
		return;

	DoLoadSpritePalette(pal, palSlot * 16);
	TintOBJPalette(palSlot);
	MaskPaletteIfFadingIn(palSlot);
}

u8 GetDarkeningTypeBySlot(u8 palSlot) //Replaces table at 0x3C2CC0
{
	if (palSlot < 13)
		return 1;

	if (palSlot < 16)
		return 0;

	u8 type = sPalRefs[palSlot - 16].Type;
	switch (type) {
		case PalTypeNPC:
		case PalTypeWeather:
			return 2;
		case PalTypeAnimation:
		case PalTypeReflection:
			return 1;
		default:
			return 0;
	}
}

extern const u16 HailstormWeatherPal[];
#define gSandstormWeatherPalette (const u16*) 0x83C2D20
void LoadPaletteForOverworldSandstorm(void)
{
	if (gWeatherPtr->currWeather == WEATHER_STEADY_SNOW) //"Snow"
		LoadCustomWeatherSpritePalette(HailstormWeatherPal);
	else
        LoadCustomWeatherSpritePalette(gSandstormWeatherPalette);
}

u8 LoadPaletteForEmotionBubbles(void)
{
	#ifdef UNBOUND
	return FindOrLoadNPCPalette(0x11A2);
	#else
	return FindOrLoadNPCPalette(0x1100);
	#endif
}

u8 sub_805F510(const struct SpritePalette *spritePalette)
{
	if (IndexOfSpritePaletteTag(spritePalette->tag) != 0xFF)
		return 0xFF;

	u8 palSlot = LoadSpritePalette(spritePalette);
	#ifdef UNBOUND
	if (palSlot != 0xFF)
	{
		if (IsPaletteTagAffectedByCharacterCustomization(spritePalette->tag))
			ChangeEventObjPal(0x100 + palSlot * 16, spritePalette->tag);
	}
	#endif
	return palSlot;
}
