#pragma once

#include "global.h"

#define WEATHER_NONE           0
#define WEATHER_CLOUDS         1
#define WEATHER_SUNNY          2
#define WEATHER_RAIN_LIGHT     3
#define WEATHER_SNOW           4
#define WEATHER_RAIN_MED       5
#define WEATHER_FOG_1          6
#define WEATHER_STEADY_SNOW    7
#define WEATHER_SANDSTORM      8
#define WEATHER_FOG_2          9
#define WEATHER_FOG_3          10
#define WEATHER_SHADE          11
#define WEATHER_DROUGHT        12
#define WEATHER_RAIN_HEAVY     13
#define WEATHER_BUBBLES        14
#define WEATHER_15             15

void __attribute__((long_call)) Task_WeatherMain(u8 taskId);
u8 __attribute__((long_call)) GetCurrentWeather(void);

// Controls how the weather should be changing the screen palettes.
enum
{
    WEATHER_PAL_STATE_CHANGING_WEATHER,
    WEATHER_PAL_STATE_SCREEN_FADING_IN,
    WEATHER_PAL_STATE_SCREEN_FADING_OUT,
    WEATHER_PAL_STATE_IDLE,
};

// For the FadeScreen function.
enum
{
    FADE_FROM_BLACK,
    FADE_TO_BLACK,
    FADE_FROM_WHITE,
    FADE_TO_WHITE,
};

struct Weather
{
    union
    {
        struct
        {
            struct Sprite *rainSprites[24];
            struct Sprite *snowflakeSprites[101];
            struct Sprite *cloudSprites[3];
        } s1;
        struct
        {
            u8 filler0[0xA0];
            struct Sprite *fog1Sprites[20];
            struct Sprite *ashSprites[20];
            struct Sprite *fog2Sprites[20];
            struct Sprite *sandstormSprites1[20];
            struct Sprite *sandstormSprites2[5];
        } s2;
    } sprites;
    u8 gammaShifts[19][32];
    u8 altGammaShifts[19][32];
    /*0x6C0*/ s8 gammaIndex;
    /*0x6C1*/ s8 gammaTargetIndex;
    /*0x6C2*/ u8 gammaStepDelay;
    /*0x6C3*/ u8 gammaStepFrameCounter;
    /*0x6C4*/ u16 fadeDestColor;
    /*0x6C6*/ u8 palProcessingState;
    /*0x6C7*/ u8 fadeScreenCounter;
    /*0x6C8*/ bool8 readyForInit;
    /*0x6C9*/ u8 taskId;
    /*0x6CA*/ u8 unknown_6CA;
    /*0x6CB*/ u8 unknown_6CB;
    /*0x6CC*/ u16 initStep;
    /*0x6CE*/ u16 finishStep;
    /*0x6D0*/ u8 currWeather;
    u8 nextWeather;
    u8 weatherGfxLoaded;
    bool8 weatherChangeComplete;
    u8 weatherPicSpritePalIndex;
    u8 altGammaSpritePalIndex;
    u16 unknown_6D6;
    u8 unknown_6D8;
    u8 unknown_6D9;
    u8 rainSpriteCount;
    u8 unknown_6DB;
    u8 unknown_6DC;
    u8 rainStrength;
    /*0x6DE*/ u8 cloudSpritesCreated;
    u8 filler_6DF[1];
    u16 unknown_6E0;
    u16 unknown_6E2;
    u8 snowflakeSpriteCount;
    u8 unknown_6E5;
    u16 unknown_6E6;
    u16 thunderCounter;
    u8 unknown_6EA;
    u8 unknown_6EB;
    u8 unknown_6EC;
    u8 unknown_6ED;
    u16 fog1ScrollPosX;
    u16 unknown_6F0;
    u16 unknown_6F2;
    u8 lightenedFogSpritePals[6];
    u8 lightenedFogSpritePalsCount;
    u8 fog1SpritesCreated;
    u16 unknown_6FC;
    u16 unknown_6FE;
    u8 ashSpritesCreated;
    u8 filler_701[3];
    u32 unknown_704;
    u32 unknown_708;
    u8 filler_70C[2];
    u16 unknown_70E;
    u16 unknown_710;
    u16 unknown_712;
    u16 unknown_714;
    u8 sandstormSprites1Created;
    u8 sandstormSprites2Created;
    u16 unknown_718;
    u16 unknown_71A;
    u16 unknown_71C;
    u16 unknown_71E;
    u16 unknown_720;
    u16 unknown_722;
    u8 fog2SpritesCreated;
    u8 filler_725[1];
    u16 unknown_726;
    u16 unknown_728;
    u16 unknown_72A;
    u16 unknown_72C;
    u8 unknown_72E;
    u8 filler_72F;
    /*0x730*/ u16 currBlendEVA;
    /*0x732*/ u16 currBlendEVB;
    /*0x734*/ u16 targetBlendEVA;
    /*0x736*/ u16 targetBlendEVB;
    /*0x738*/ u8 blendUpdateCounter;
    /*0x739*/ u8 blendFrameCounter;
    /*0x73A*/ u8 blendDelay;
    u8 filler_73B[0x3C-0x3B];
    s16 unknown_73C;
    s16 unknown_73E;
    s16 unknown_740;
    s16 unknown_742;
    u8 filler_744[0xD-4];
    s8 loadDroughtPalsIndex;
    u8 loadDroughtPalsOffset;
};