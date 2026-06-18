#pragma once

#include "../global.h"

/**
 * \file dynamic_ow_pals.h
 * \brief Contains the implementation of dynamic loading of OW palettes by
 *		  Navenatox: https://www.pokecommunity.com/showthread.php?p=9030971.
 */

//Exported Functions
u8 AddPalRef(u8 type, u16 palTag);
u8 FindPalRef(u8 type, u16 palTag);
void RemovePalRef(u16 palTag);
u8 GetPalTypeByPaletteOffset(u16 offset);
u8 GetFadeTypeByWeather(u8 weather);
void PalRefDecreaseCount(u8 palSlot);
void ClearAllPalRefs(void);
u8 GetPalSlotMisc(u32 OBJData);
u8 FindOrLoadNPCPalette(u16 palTag);
u8 FindOrCreateReflectionPalette(u8 palSlotNPC);
void FogBrightenPalettes(u16 brightenIntensity);
void FogBrightenAndFade(u8 palSlot, u8 fadeIntensity, u16 fadeColor);
u8 GetDarkeningTypeBySlot(u8 palSlot);

//Hooked In Functions
u8 PaletteNeedsFogBrightening(u8 palSlot);
u8 FindPalette(u16 palTag);
void ClearAllPalettes(void);
u8 FindOrLoadPalette(struct SpritePalette* pal);
u8 SetUpTreeDisguise(void);
u8 SetUpRockDisguise(void);
u8 SetUpWeirdDisguise(void);
void LoadCloudOrSandstormPalette(u16* pal);

//Exported Constants
#define PalTypeUnused 0
#define PalTypeNPC 1
#define PalTypeAnimation 2
#define PalTypeWeather 3
#define PalTypeReflection 4
#define PalTypeOther 5

#ifdef PALETTE_SWAPPER
void LoadObjectEventPalette(u16 paletteTag);
void PatchObjectPalette(u16 paletteTag, u8 paletteSlot);
u8 FindObjectEventPaletteIndexByTag(u16 tag);
u8 GetObjectEventPalettesTableByTag(u16 tag);
#endif
