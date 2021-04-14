#ifndef GUARD_GBA_SYSCALL_H
#define GUARD_GBA_SYSCALL_H

#define RESET_EWRAM      0x01
#define RESET_IWRAM      0x02
#define RESET_PALETTE    0x04
#define RESET_VRAM       0x08
#define RESET_OAM        0x10
#define RESET_SIO_REGS   0x20
#define RESET_SOUND_REGS 0x40
#define RESET_REGS       0x80
#define RESET_ALL        0xFF

void __attribute__((long_call)) SoftReset(u32 resetFlags);

void __attribute__((long_call)) RegisterRamReset(u32 resetFlags);

void __attribute__((long_call)) VBlankIntrWait(void);

u16 __attribute__((long_call)) Sqrt(u32 num);

u16 __attribute__((long_call)) ArcTan2(s16 x, s16 y);

#define CPU_SET_SRC_FIXED 0x01000000
#define CPU_SET_16BIT     0x00000000
#define CPU_SET_32BIT     0x04000000

void __attribute__((long_call)) CpuSet(const void *src, void *dest, u32 control);

#define CPU_FAST_SET_SRC_FIXED 0x01000000

void __attribute__((long_call)) CpuFastSet(const void *src, void *dest, u32 control);

void __attribute__((long_call)) BgAffineSet(struct BgAffineSrcData *src, struct BgAffineDstData *dest, s32 count);

void __attribute__((long_call)) ObjAffineSet(struct ObjAffineSrcData *src, void *dest, s32 count, s32 offset);

//void LZ77UnCompWram(const void *src, void *dest);

//void LZ77UnCompVram(const void *src, void *dest);

void __attribute__((long_call))RLUnCompWram(const void *src, void *dest);

void __attribute__((long_call))RLUnCompVram(const void *src, void *dest);

int __attribute__((long_call))MultiBoot(struct MultiBootParam *mp);

#endif // GUARD_GBA_SYSCALL_H
