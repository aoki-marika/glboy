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

bool gbSetTileData(int type, int index, GLuint data[]);

GBTileMap *gbGetBackground(int index);
bool gbSetActiveBackground(int index);

GBTileMap *gbGetWindow(int index);
bool gbSetActiveWindow(int index);

bool gbAddSprite(GBSprite *sprite);
bool gbRemoveSprite(GBSprite *sprite);

bool gbRun();
bool gbQuit();