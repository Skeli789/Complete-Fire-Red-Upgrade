#ifndef GUARD_TILESET_ANIMS_H
#define GUARD_TILESET_ANIMS_H

void __attribute__((long_call)) InitTilesetAnimations(void);
void __attribute__((long_call)) InitSecondaryTilesetAnimation(void);
void __attribute__((long_call)) UpdateTilesetAnimations(void);
void __attribute__((long_call)) TransferTilesetAnimsBuffer(void);

#endif // GUARD_TILESET_ANIMS_H
