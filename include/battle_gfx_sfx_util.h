#pragma once

#include "sprite.h"

void __attribute__((long_call)) InitAndLaunchSpecialAnimation(u8 activeBank, u8 atkBank, u8 defBank, u8 tableId);
void __attribute__((long_call)) ClearBehindSubstituteBit(u8 bank);

/*
void __attribute__((long_call)) AllocateBattleSpritesData(void);
void __attribute__((long_call)) FreeBattleSpritesData(void);
u16 __attribute__((long_call)) ChooseMoveAndTargetInBattlePalace(void);
void __attribute__((long_call)) sub_805D714(struct Sprite *sprite);
void __attribute__((long_call)) sub_805D770(struct Sprite *sprite, bool8 arg1);
void __attribute__((long_call)) sub_805D7AC(struct Sprite *sprite);
void __attribute__((long_call)) InitAndLaunchChosenStatusAnimation(bool8 isStatus2, u32 status);
bool8 __attribute__((long_call)) TryHandleLaunchBattleTableAnimation(u8 activeBank, u8 atkBank, u8 defBank, u8 tableId, u16 argument);
bool8 __attribute__((long_call)) IsMoveWithoutAnimation(u16 moveId, u8 animationTurn);
bool8 __attribute__((long_call)) mplay_80342A4(u8 bank);
void __attribute__((long_call)) BattleLoadOpponentMonSpriteGfx(struct Pokemon *mon, u8 bank);
void __attribute__((long_call)) BattleLoadPlayerMonSpriteGfx(struct Pokemon *mon, u8 bank);
void __attribute__((long_call)) nullsub_23(void);
void __attribute__((long_call)) nullsub_24(u16 species);
void __attribute__((long_call)) DecompressTrainerFrontPic(u16 frontPicId, u8 bank);
void __attribute__((long_call)) DecompressTrainerBackPic(u16 backPicId, u8 bank);
void __attribute__((long_call)) nullsub_25(u8 arg0);
void __attribute__((long_call)) FreeTrainerFrontPicPalette(u16 frontPicId);
void __attribute__((long_call)) sub_805DFFC(void);
bool8 __attribute__((long_call)) BattleLoadAllHealthBoxesGfx(u8 state);
void __attribute__((long_call)) LoadBattleBarGfx(u8 arg0);
bool8 __attribute__((long_call)) BattleInitAllSprites(u8 *state1, u8 *bank);
void __attribute__((long_call)) ClearSpritesHealthboxAnimData(void);
void __attribute__((long_call)) CopyAllBattleSpritesInvisibilities(void);
void __attribute__((long_call)) CopyBattleSpriteInvisibility(u8 bank);
void __attribute__((long_call)) HandleSpeciesGfxDataChange(u8 bankAtk, u8 bankDef, bool8 notTransform);
void __attribute__((long_call)) BattleLoadSubstituteOrMonSpriteGfx(u8 bank, bool8 loadMonSprite);
void __attribute__((long_call)) LoadBattleMonGfxAndAnimate(u8 bank, bool8 loadMonSprite, u8 spriteId);
void __attribute__((long_call)) TrySetBehindSubstituteSpriteBit(u8 bank, u16 move);
void __attribute__((long_call)) HandleLowHpMusicChange(struct Pokemon *mon, u8 bank);
void __attribute__((long_call)) BattleStopLowHpSound(void);
u8 __attribute__((long_call)) GetMonHPBarLevel(struct Pokemon *mon);
void __attribute__((long_call)) sub_805EAE8(void);
void __attribute__((long_call)) sub_805EB9C(u8 affineMode);
void __attribute__((long_call)) LoadAndCreateEnemyShadowSprites(void);
void __attribute__((long_call)) SpriteCB_SetInvisible(struct Sprite *sprite);
void __attribute__((long_call)) SetBankEnemyShadowSpriteCallback(u8 bank, u16 species);
void __attribute__((long_call)) EnemyShadowCallbackToSetInvisible(u8 bank);
void __attribute__((long_call)) sub_805EF14(void);
void __attribute__((long_call)) ClearTemporarySpeciesSpriteData(u8 bank, bool8 dontClearSubstitute);
void __attribute__((long_call)) AllocateMonSpritesGfx(void);
void __attribute__((long_call)) FreeMonSpritesGfx(void);
bool32 __attribute__((long_call)) ShouldPlayNormalPokeCry(struct Pokemon *mon);
*/
