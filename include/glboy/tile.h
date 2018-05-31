#pragma once

#include <stdbool.h>

#include "gl.h"
#include "gfx_constants.h"

typedef struct
{
    int width;
    int height;
    int x;
    int y;
    int *tiles;
} GBTileMap;

bool gbTileInit();
bool gbTileQuit();

GLuint gbGetTileData(int type, int index);

bool gbSetTileData(int type, int index, GLuint data[TILE_SIZE]);
bool gbSetTileDataMultiple(int type, int index, int count, GLuint data[count * TILE_SIZE]);

bool gbRenderTile(int type, int index, int x, int y, float z, bool flipX, bool flipY);
bool gbRenderTileMap(GBTileMap *map, int type, bool wrap);