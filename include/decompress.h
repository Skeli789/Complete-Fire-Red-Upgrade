#pragma once

#include "global.h"

#include "sprite.h"

#define gDecompressionBuffer 	 ((u8*) 0x201C000)

void __attribute__((long_call)) LZDecompressVram(const void *src, void *dest);
void __attribute__((long_call)) LZDecompressWram(const void *src, void *dest);
void __attribute__((long_call)) LZ77UnCompWram(const void *src, void *dest);
void __attribute__((long_call)) LZ77UnCompVram(const void *src, void *dest);
u16 __attribute__((long_call)) LoadCompressedSpriteSheet(const struct CompressedSpriteSheet *src);
void __attribute__((long_call)) LoadCompressedSpritePalette(const struct CompressedSpritePalette *src);
bool8 __attribute__((long_call)) LoadCompressedSpriteSheetUsingHeap(const struct CompressedSpriteSheet* src);
bool8 __attribute__((long_call)) LoadCompressedSpritePaletteUsingHeap(const struct CompressedSpritePalette *src);
void __attribute__((long_call)) LoadCompressedSpritePaletteOverrideBuffer(const struct CompressedSpritePalette *a, void *buffer);
void __attribute__((long_call)) LoadCompressedSpriteSheetOverrideBuffer(const struct CompressedSpriteSheet *src, void *buffer);
u16 __attribute__((long_call)) LoadCompressedSpriteSheet(const struct CompressedSpriteSheet *src);
void __attribute__((long_call)) HandleLoadSpecialPokePic_DontHandleDeoxys(const struct CompressedSpriteSheet *src, void *dest, s32 species, u32 personality);

/*
u16 LoadCompressedObjectPic(const struct CompressedSpriteSheet *src);
void LoadCompressedObjectPicOverrideBuffer(const struct CompressedSpriteSheet *src, void *buffer);
bool8 LoadCompressedObjectPicUsingHeap(const struct CompressedSpriteSheet* src);

void LoadCompressedObjectPalette(const struct CompressedSpritePalette *src);
void LoadCompressedObjectPaletteOverrideBuffer(const struct CompressedSpritePalette *a, void *buffer);
bool8 LoadCompressedObjectPaletteUsingHeap(const struct CompressedSpritePalette *src);

void DecompressPicFromTable(const struct CompressedSpriteSheet *src, void* buffer, s32 species);
void DecompressPicFromTable_2(const struct CompressedSpriteSheet *src, void* buffer, s32 species);
void DecompressPicFromTable_DontHandleDeoxys(const struct CompressedSpriteSheet *src, void* buffer, s32 species);

void HandleLoadSpecialPokePic(const struct CompressedSpriteSheet *src, void *dest, s32 species, u32 personality);
void HandleLoadSpecialPokePic_2(const struct CompressedSpriteSheet *src, void *dest, s32 species, u32 personality);

void LoadSpecialPokePic(const struct CompressedSpriteSheet *src, void *dest, s32 species, u32 personality, bool8 isFrontPic);
void LoadSpecialPokePic_2(const struct CompressedSpriteSheet *src, void *dest, s32 species, u32 personality, bool8 isFrontPic);
void LoadSpecialPokePic_DontHandleDeoxys(const struct CompressedSpriteSheet *src, void *dest, s32 species, u32 personality, bool8 isFrontPic);

u32 sub_8034974(const u8 *ptr);
*/
