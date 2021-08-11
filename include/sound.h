#pragma once

#include "global.h"

extern bool8 gDisableMusic;
extern bool8 gWarpMusicDisable;

struct Fanfare
{
	u16 songNum;
	u16 duration;
};

void __attribute__((long_call)) InitMapMusic(void);
void __attribute__((long_call)) MapMusicMain(void);
void __attribute__((long_call)) ResetMapMusic(void);
u16 __attribute__((long_call)) GetCurrentMapMusic(void);
void __attribute__((long_call)) PlayNewMapMusic(u16 songNum);
void __attribute__((long_call)) StopMapMusic(void);
void __attribute__((long_call)) FadeOutMapMusic(u8 speed);
void __attribute__((long_call)) FadeOutAndPlayNewMapMusic(u16 songNum, u8 speed);
void __attribute__((long_call)) FadeOutAndFadeInNewMapMusic(u16 songNum, u8 fadeOutSpeed, u8 fadeInSpeed);
void __attribute__((long_call)) FadeInNewMapMusic(u16 songNum, u8 speed);
bool8 __attribute__((long_call)) IsNotWaitingForBGMStop(void);
void __attribute__((long_call)) PlayFanfareByFanfareNum(u8 fanfareNum);
bool8 __attribute__((long_call)) WaitFanfare(bool8 stop);
void __attribute__((long_call)) StopFanfareByFanfareNum(u8 fanfareNum);
void __attribute__((long_call)) PlayFanfare(u16 songNum);
bool8 __attribute__((long_call)) IsFanfareTaskInactive(void);
void __attribute__((long_call)) FadeInNewBGM(u16 songNum, u8 speed);
void __attribute__((long_call)) FadeOutBGMTemporarily(u8 speed);
bool8 __attribute__((long_call)) IsBGMPausedOrStopped(void);
void __attribute__((long_call)) FadeInBGM(u8 speed);
void __attribute__((long_call)) FadeOutBGM(u8 speed);
bool8 __attribute__((long_call)) IsBGMStopped(void);

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
void __attribute__((long_call)) SetBGMVolume_SuppressHelpSystemReduction(u16 volume);
void __attribute__((long_call)) BGMVolumeMax_EnableHelpSystemReduction(void);
