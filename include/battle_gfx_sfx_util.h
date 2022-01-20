#pragma once

#include "sprite.h"

/*
void __attribute__((long_call)) AllocateBattleSpritesData(void);
void __attribute__((long_call)) FreeBattleSpritesData(void);
void __attribute__((long_call)) SpriteCB_WaitForBattlerBallReleaseAnim(struct Sprite *sprite);*/
void __attribute__((long_call)) SpriteCB_TrainerSlideIn(struct Sprite *sprite);
/*void __attribute__((long_call)) InitAndLaunchChosenStatusAnimation(bool8 isStatus2, u32 status);
bool8 __attribute__((long_call)) TryHandleLaunchBattleTableAnimation(u8 activeBattler, u8 atkBattler, u8 defBattler, u8 tableId, u16 argument);*/
void __attribute__((long_call)) InitAndLaunchSpecialAnimation(u8 activeBattler, u8 atkBattler, u8 defBattler, u8 tableId);
/*bool8 __attribute__((long_call)) IsMoveWithoutAnimation(u16 moveId, u8 animationTurn);
bool8 __attribute__((long_call)) IsBattleSEPlaying(u8 battlerId);
void __attribute__((long_call)) BattleLoadOpponentMonSpriteGfx(struct Pokemon *mon, u8 battlerId);
void __attribute__((long_call)) BattleLoadPlayerMonSpriteGfx(struct Pokemon *mon, u8 battlerId);
void __attribute__((long_call)) DecompressGhostFrontPic(struct Pokemon *unused, u8 battlerId);
void __attribute__((long_call)) DecompressTrainerFrontPic(u16 frontPicId, u8 battlerId);*/
void __attribute__((long_call)) DecompressTrainerBackPalette(u16 a1, u8 a2);
/*void __attribute__((long_call)) BattleGfxSfxDummy3(u8 a1);*/
void __attribute__((long_call)) FreeTrainerFrontPicPaletteAndTile(u16 frontPicId);
/*bool8 __attribute__((long_call)) BattleLoadAllHealthBoxesGfx(u8 state);
void __attribute__((long_call)) LoadBattleBarGfx(u8 arg0);
bool8 __attribute__((long_call)) BattleInitAllSprites(u8 *state, u8 *battlerId);
void __attribute__((long_call)) ClearSpritesHealthboxAnimData(void);
void __attribute__((long_call)) CopyAllBattleSpritesInvisibilities(void);
void __attribute__((long_call)) CopyBattleSpriteInvisibility(u8 battlerId);
void __attribute__((long_call)) HandleSpeciesGfxDataChange(u8 battlerAtk, u8 battlerDef, u8 notTransform);
void __attribute__((long_call)) BattleLoadSubstituteOrMonSpriteGfx(u8 battlerId, bool8 loadMonSprite);
void __attribute__((long_call)) LoadBattleMonGfxAndAnimate(u8 battlerId, bool8 loadMonSprite, u8 spriteId);
void __attribute__((long_call)) TrySetBehindSubstituteSpriteBit(u8 battlerId, u16 move);*/
void __attribute__((long_call)) ClearBehindSubstituteBit(u8 battlerId);
/*void __attribute__((long_call)) HandleLowHpMusicChange(struct Pokemon *mon, u8 battlerId);
void __attribute__((long_call)) BattleStopLowHpSound(void);
void __attribute__((long_call)) HandleBattleLowHpMusicChange(void);
void __attribute__((long_call)) SetBattlerSpriteAffineMode(u8 affineMode);
void __attribute__((long_call)) LoadAndCreateEnemyShadowSprites(void);
void __attribute__((long_call)) SpriteCB_SetInvisible(struct Sprite *sprite);*/
void __attribute__((long_call)) SetBattlerShadowSpriteCallback(u8 battlerId, u16 species);
/*void __attribute__((long_call)) HideBattlerShadowSprite(u8 battlerId);
void __attribute__((long_call)) BattleInterfaceSetWindowPals(void);
void __attribute__((long_call)) ClearTemporarySpeciesSpriteData(u8 battlerId, bool8 dontClearSubstitute);
void __attribute__((long_call)) AllocateMonSpritesGfx(void);
void __attribute__((long_call)) FreeMonSpritesGfx(void);
bool32 __attribute__((long_call)) ShouldPlayNormalPokeCry(struct Pokemon *mon);
*/
