#pragma once

#include <stdbool.h>

#include "gl.h"
#include "sdl.h"
#include "gfx_utils.h"
#include "gfx_constants.h"

bool gbInit();

void gbSetUpdateCallback(void (*callback)());
void gbSetRenderCallback(void (*callback)());

bool gbSetTileData(int type, int index, GLuint data[TILE_SIZE]);
bool gbSetTileDataMultiple(int type, int index, int count, GLuint data[count * TILE_SIZE]);

GBTileMap *gbGetWindow(int index);
bool gbSetActiveWindow(int index);

bool gbAddSprite(GBSprite *sprite);
bool gbRemoveSprite(GBSprite *sprite);

bool gbRun();
bool gbQuit();