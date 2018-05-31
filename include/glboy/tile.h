#pragma once

#include <stdbool.h>

#include "gl.h"
#include "gfx_constants.h"

bool gbTileInit();
bool gbTileQuit();

GLuint gbGetTileData(int type, int index);

bool gbSetTileData(int type, int index, GLuint data[TILE_SIZE]);
bool gbSetTileDataMultiple(int type, int index, int count, GLuint data[count * TILE_SIZE]);