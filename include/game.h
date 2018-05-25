#pragma once

#include <stdbool.h>

#include "gl.h"
#include "sdl.h"
#include "gfx.h"
#include "gfx_constants.h"

bool gbInit();

void gbSetRenderCallback(void (*callback)());

void gbSetColours(SDL_Color colours[PAL_COUNT]); //screen colours
void gbSetPalette(int palette[PAL_COUNT]); //map palette index to colour index

// todo: i -> index

bool gbSetTileData(int type, int i, GLuint data[]);

GBTileMap *gbGetBackground(int i);
bool gbSetActiveBackground(int i);

GBTileMap *gbGetWindow(int i);
bool gbSetActiveWindow(int i);

bool gbRun();
bool gbQuit();