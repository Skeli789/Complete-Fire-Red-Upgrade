#include "global.h"
#include "constants/event_objects.h"
#include "constants/species.h"
#include "new/character_customization.h"

extern const struct SpritePalette gObjectEventSpritePalettes16[];
extern const struct SpritePalette gObjectEventSpritePalettes17[];
extern const struct SpritePalette gObjectEventSpritePalettes18[];
extern const struct SpritePalette gObjectEventSpritePalettes19[];
extern const struct SpritePalette gObjectEventSpritePalettes20[];

static const struct SpritePalette* const sObjectEventSpritePalettesTables[] = {
    gObjectEventSpritePalettes16,
    gObjectEventSpritePalettes17,
    gObjectEventSpritePalettes18,
    gObjectEventSpritePalettes19,
    gObjectEventSpritePalettes20,
};