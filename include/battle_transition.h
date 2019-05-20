#pragma once

#include "global.h"

//Exported Functions
void __attribute__((long_call)) sub_80D3120(s16 spriteId, s16 value);
void __attribute__((long_call)) IncreaseMugshotFuncState(s16 spriteId);
s16 __attribute__((long_call)) IsOpponentMugshotDoneSlidingRight(s16 spriteId);
void __attribute__((long_call)) HBlankCB_Phase2_Mugshots(void);
void __attribute__((long_call)) sub_80D3E28(u16** a0, u16** a1);
void __attribute__((long_call)) SpiteCB_Mugshot(struct Sprite* sprite);

//Exported Data Structures
struct TransitionData
{
    vu8 VBlank_DMA;
    u16 WININ;
    u16 WINOUT;
    u16 WIN0H;
    u16 WIN0V;
    u16 field_A; // unused
    u16 field_C; // unused
    u16 BLDCNT;
    u16 BLDALPHA;
    u16 BLDY;
    s16 field_14;
    s16 field_16;
    s16 BG0HOFS_1;
    s16 BG0HOFS_2;
    s16 BG0VOFS; // used but not set
    s16 field_1E; // unused
    s16 field_20;
    s16 field_22; // unused
    s16 data[11];
};

//Exported Constants
enum // TRANSITION_MUGSHOT
{
    MUGSHOT_LORELEI,
    MUGSHOT_BRUNO,
    MUGSHOT_AGATHA,
    MUGSHOT_LANCE,
    MUGSHOT_CHAMPION,
    MUGSHOTS_COUNT
};

#define B_TRANSITION_BLUR                   0 //Checkers
#define B_TRANSITION_SWIRL                  1 //Wavy
#define B_TRANSITION_SHUFFLE                2 //Ripple
#define B_TRANSITION_BIG_POKEBALL           3
#define B_TRANSITION_POKEBALLS_TRAIL        4
#define B_TRANSITION_CLOCKWISE_BLACKFADE    5
#define B_TRANSITION_RIPPLE                 6
#define B_TRANSITION_WAVE                   7 //Wave washes over screen
#define B_TRANSITION_SLICE                  8 //The screen is chopped into slices
#define B_TRANSITION_WHITEFADE              9
#define B_TRANSITION_GRID_SQUARES           10
#define B_TRANSITION_SHARDS                 11
#define B_TRANSITION_LORELEI                 12
#define B_TRANSITION_BRUNO                  13
#define B_TRANSITION_AGATHA                 14
#define B_TRANSITION_LANCE                  15
#define B_TRANSITION_CHAMPION               16
#define B_TRANSITION_SPIRAL_OUTWARRDS       17
#define B_TRANSITION_COUNT                  18

// added in Emerald
#define B_TRANSITION_AQUA                   17
#define B_TRANSITION_MAGMA                  18
#define B_TRANSITION_REGICE                 19
#define B_TRANSITION_REGISTEEL              20
#define B_TRANSITION_REGIROCK               21
#define B_TRANSITION_KYOGRE                 22
#define B_TRANSITION_GROUDON                23
#define B_TRANSITION_RAYQUAZA               24
#define B_TRANSITION_SHRED_SPLIT            25
#define B_TRANSITION_BLACKHOLE1             26
#define B_TRANSITION_BLACKHOLE2             27
#define B_TRANSITION_RECTANGULAR_SPIRAL     28
#define B_TRANSITION_29                     29
#define B_TRANSITION_30                     30
#define B_TRANSITION_31                     31
#define B_TRANSITION_32                     32
#define B_TRANSITION_33                     33
#define B_TRANSITION_34                     34
#define B_TRANSITION_35                     35
#define B_TRANSITION_36                     36
#define B_TRANSITION_37                     37
#define B_TRANSITION_38                     38
#define B_TRANSITION_39                     39
#define B_TRANSITION_40                     40
#define B_TRANSITION_41                     41
//#define B_TRANSITION_COUNT                  42
