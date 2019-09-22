#pragma once

#include "global.h"

// Exported type declarations

// Exported RAM declarations

// Exported ROM declarations
void __attribute__((long_call)) SetGpuReg(u8 regOffset, u16 value);

/*
void InitGpuRegManager(void);
void CopyBufferedValuesToGpuRegs(void);
void SetGpuReg_ForcedBlank(u8 regOffset, u16 value);
u16 GetGpuReg(u8 regOffset);
void SetGpuRegBits(u8 regOffset, u16 mask);
void ClearGpuRegBits(u8 regOffset, u16 mask);
void EnableInterrupts(u16 mask);
void DisableInterrupts(u16 mask);
*/
