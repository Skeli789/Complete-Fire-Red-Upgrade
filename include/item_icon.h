#pragma once

extern void* gItemIconDecompressionBuffer;
extern void* gItemIcon4x4Buffer;

extern const struct SpriteTemplate gItemIconSpriteTemplate;

void __attribute__((long_call)) ResetItemMenuIconState(void);
u8 __attribute__((long_call)) AddItemIconSprite(u16 tilesTag, u16 paletteTag, u16 itemId);
u8 __attribute__((long_call)) AddCustomItemIconSprite(const struct SpriteTemplate *customSpriteTemplate, u16 tilesTag, u16 paletteTag, u16 itemId);
void __attribute__((long_call)) CreateItemMenuIcon(u16 itemId, u8 idx);
void __attribute__((long_call)) DestroyItemMenuIcon(u8 idx);

/*
bool8 AllocItemIconTemporaryBuffers(void);
void FreeItemIconTemporaryBuffers(void);
void CopyItemIconPicTo4x4Buffer(const void *src, void *dest);
const void *GetItemIconPicOrPalette(u16 itemId, u8 which);
*/