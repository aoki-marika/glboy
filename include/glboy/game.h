#pragma once

#include <stdbool.h>

#include "gl.h"
#include "sdl.h"
#include "gfx_utils.h"
#include "gfx_constants.h"

bool gbInit();

void gbSetUpdateCallback(void (*callback)());
void gbSetRenderCallback(void (*callback)());

void gbSetColours(SDL_Color colours[PAL_LENGTH]); //screen colours

// map palette index to colour index
void gbSetBackgroundPalette(int palette[PAL_LENGTH]); //background and window
bool gbSetSpritePalette(int index, int palette[PAL_LENGTH]);

bool gbSetTileData(int type, int index, GLuint data[TILE_SIZE]);
bool gbSetTileDataMultiple(int type, int index, int count, GLuint data[count * TILE_SIZE]);

GBTileMap *gbGetBackground(int index);
bool gbSetActiveBackground(int index);

GBTileMap *gbGetWindow(int index);
bool gbSetActiveWindow(int index);

bool gbAddSprite(GBSprite *sprite);
bool gbRemoveSprite(GBSprite *sprite);

bool gbRun();
bool gbQuit();