#ifndef GUARD_CONSTANTS_FIELD_WEATHER_H
#define GUARD_CONSTANTS_FIELD_WEATHER_H

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

#define MAX_RAIN_SPRITES             24
#define NUM_CLOUD_SPRITES            3
#define NUM_FOG_HORIZONTAL_SPRITES   20
#define NUM_ASH_SPRITES              20
#define NUM_FOG_DIAGONAL_SPRITES     20
#define NUM_SANDSTORM_SPRITES        20
#define NUM_SWIRL_SANDSTORM_SPRITES  5

// Controls how the weather should be changing the screen palettes.
#define WEATHER_PAL_STATE_CHANGING_WEATHER   0
#define WEATHER_PAL_STATE_SCREEN_FADING_IN   1
#define WEATHER_PAL_STATE_SCREEN_FADING_OUT  2
#define WEATHER_PAL_STATE_IDLE               3

// Modes for FadeScreen
#define FADE_FROM_BLACK  0
#define FADE_TO_BLACK    1
#define FADE_FROM_WHITE  2
#define FADE_TO_WHITE    3

#endif // GUARD_CONSTANTS_FIELD_WEATHER_H
