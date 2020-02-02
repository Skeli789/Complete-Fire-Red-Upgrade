#pragma once

#include "global.h"

#include "gba/m4a_internal.h"

void __attribute__((long_call)) m4aSoundVSync(void);
void __attribute__((long_call)) m4aSoundVSyncOn(void);

void __attribute__((long_call)) m4aSoundInit(void);
void __attribute__((long_call)) m4aSoundMain(void);
void __attribute__((long_call)) m4aSongNumStart(u16 n);
void __attribute__((long_call)) m4aSongNumStartOrChange(u16 n);
void __attribute__((long_call)) m4aSongNumStartOrContinue(u16 n);
void __attribute__((long_call)) m4aSongNumStop(u16 n);
void __attribute__((long_call)) m4aMPlayStop(struct MusicPlayerInfo *mplayInfo);
void __attribute__((long_call)) m4aMPlayAllStop(void);
void __attribute__((long_call)) m4aMPlayContinue(struct MusicPlayerInfo *mplayInfo);
void __attribute__((long_call)) m4aMPlayFadeOut(struct MusicPlayerInfo *mplayInfo, u16 speed);
void __attribute__((long_call)) m4aMPlayFadeOutTemporarily(struct MusicPlayerInfo *mplayInfo, u16 speed);
void __attribute__((long_call)) m4aMPlayFadeIn(struct MusicPlayerInfo *mplayInfo, u16 speed);
void __attribute__((long_call)) m4aMPlayImmInit(struct MusicPlayerInfo *mplayInfo);
void __attribute__((long_call)) m4aMPlayVolumeControl(struct MusicPlayerInfo *mplayInfo, u16 trackBits, u16 volume);

void __attribute__((long_call)) MPlayStart(struct MusicPlayerInfo *mplayInfo, const struct SongHeader *songHeader);

extern struct MusicPlayerInfo gMPlayInfo_BGM;
extern struct MusicPlayerInfo gMPlayInfo_SE1;
extern struct MusicPlayerInfo gMPlayInfo_SE2;
extern struct MusicPlayerInfo gMPlayInfo_SE3;
extern struct SoundInfo gSoundInfo;

#define gMPlayTable ((const struct MusicPlayer*) 0x84A329C)
#define gSongTable ((const struct Song*) *((u32*) 0x81DD11C))
