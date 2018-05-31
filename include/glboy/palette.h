#pragma once

#include <stdbool.h>

#include "sdl.h"
#include "gfx_constants.h"

typedef enum
{
    GBPaletteTypeBackground = 0,
    GBPaletteTypeSprite = 1,
} GBPaletteType;

typedef enum
{
    GBPaletteModeBackground = 0,
    GBPaletteModeWindow = 1,
    GBPaletteModeSprite = 2,
} GBPaletteMode;

bool gbPaletteInit();
bool gbPaletteQuit();

void gbSetColours(SDL_Color colours[PAL_LENGTH]);

void gbSetBackgroundPalette(int palette[PAL_LENGTH]);
bool gbSetSpritePalette(int index, int palette[PAL_LENGTH]);

bool gbSetActivePalette(GBPaletteType type, int index);
void gbSetPaletteMode(GBPaletteMode mode);