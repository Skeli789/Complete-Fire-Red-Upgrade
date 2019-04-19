#ifndef GUARD_SOUND_H
#define GUARD_SOUND_H

#include "global.h"

void InitMapMusic(void);
void MapMusicMain(void);
void ResetMapMusic(void);
u16 GetCurrentMapMusic(void);
void PlayNewMapMusic(u16 songNum);
void StopMapMusic(void);
void FadeOutMapMusic(u8 speed);
void FadeOutAndPlayNewMapMusic(u16 songNum, u8 speed);
void FadeOutAndFadeInNewMapMusic(u16 songNum, u8 fadeOutSpeed, u8 fadeInSpeed);
void FadeInNewMapMusic(u16 songNum, u8 speed);
bool8 IsNotWaitingForBGMStop(void);
void PlayFanfareByFanfareNum(u8 fanfareNum);
bool8 WaitFanfare(bool8 stop);
void StopFanfareByFanfareNum(u8 fanfareNum);
void PlayFanfare(u16 songNum);
bool8 IsFanfareTaskInactive(void);
void FadeInNewBGM(u16 songNum, u8 speed);
void FadeOutBGMTemporarily(u8 speed);
bool8 IsBGMPausedOrStopped(void);
void FadeInBGM(u8 speed);
void FadeOutBGM(u8 speed);
bool8 IsBGMStopped(void);

void __attribute__((long_call)) PlayCry1(u16 species, s8 pan);
void __attribute__((long_call)) PlayCry2(u16 species, s8 pan, s8 volume, u8 priority);
void __attribute__((long_call)) PlayCry3(u16 species, s8 pan, u8 mode);
void __attribute__((long_call)) PlayCry4(u16 species, s8 pan, u8 mode);
void __attribute__((long_call)) PlayCry5(u16 species, u8 mode);
void __attribute__((long_call)) PlayCry6(u16 species, s8 pan, u8 mode);
void __attribute__((long_call)) PlayCryInternal(u16 species, s8 pan, s8 volume, u8 priority, u8 mode);
bool8 __attribute__((long_call)) IsCryFinished(void);
void __attribute__((long_call)) StopCryAndClearCrySongs(void);
void __attribute__((long_call)) StopCry(void);
bool8 __attribute__((long_call)) IsCryPlayingOrClearCrySongs(void);
bool8 __attribute__((long_call)) IsCryPlaying(void);
void __attribute__((long_call)) PlayBGM(u16 songNum);
void __attribute__((long_call)) PlaySE(u16 songNum);
void __attribute__((long_call)) PlaySE12WithPanning(u16 songNum, s8 pan);
void __attribute__((long_call)) PlaySE1WithPanning(u16 songNum, s8 pan);
void __attribute__((long_call)) PlaySE2WithPanning(u16 songNum, s8 pan);
void __attribute__((long_call)) SE12PanpotControl(s8 pan);
bool8 __attribute__((long_call)) IsSEPlaying(void);
bool8 __attribute__((long_call)) IsBGMPlaying(void);
bool8 __attribute__((long_call)) IsSpecialSEPlaying(void);

#endif // GUARD_SOUND_H
