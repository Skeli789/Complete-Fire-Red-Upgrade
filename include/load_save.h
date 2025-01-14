#ifndef GUARD_LOAD_SAVE_H
#define GUARD_LOAD_SAVE_H

#include "global.h"

extern bool32 gFlashMemoryPresent;

void __attribute__((long_call)) SetContinueGameWarpStatus(void);
void __attribute__((long_call)) ClearContinueGameWarpStatus2(void);
bool32 __attribute__((long_call)) UseContinueGameWarp(void);
void __attribute__((long_call)) SavePlayerParty(void);
void __attribute__((long_call)) LoadPlayerParty(void);

/*
void ClearSav2(void);
void ClearSav1(void);
void CheckForFlashMemory(void);
void MoveSaveBlocks_ResetHeap(void);
bool32 GetSecretBase2Field_9(void);
void ClearSecretBase2Field_9(void);
void SetSecretBase2Field_9(void);
void SetSecretBase2Field_9_AndHideBG(void);
void ClearSecretBase2Field_9_2(void);
void SaveSerializedGame(void);
void LoadSerializedGame(void);
void LoadPlayerBag(void);
void SavePlayerBag(void);
void SetSaveBlocksPointers(void);
void MoveSaveBlocks_ResetHeap(void);
void ApplyNewEncryptionKeyToAllEncryptedData(u32 encryptionKey);
void ApplyNewEncryptionKeyToBagItems(u32 encryptionKey);
void ApplyNewEncryptionKeyToWord(u32 * word, u32 encryptionKey);
void ApplyNewEncryptionKeyToHword(u16 * hword, u32 encryptionKey);
void SetContinueGameWarpStatusToDynamicWarp(void);
void ClearContinueGameWarpStatus();*/

#endif // GUARD_LOAD_SAVE_H