#pragma once

#include <stdbool.h>

#include "gl.h"
#include "sdl.h"
#include "gfx.h"
#include "gfx_constants.h"

bool gbInit();

void gbSetRenderCallback(void (*callback)());

void gbSetColours(SDL_Color colours[PAL_SIZE]); // screen colours
void gbSetPalette(int palette[PAL_SIZE]); // map palette index to colour index

GBTileMap *gbGetBackground(int i);
bool gbSetActiveBackground(int i);

bool gbRun();
bool gbQuit();