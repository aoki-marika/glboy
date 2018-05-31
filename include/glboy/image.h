#pragma once

#include "gl.h"
#include "gfx_constants.h"

void gbParseTile(unsigned char bytes[TILE_BYTES], GLuint pixels[TILE_SIZE]);
void gbParseImage(int tileCount, unsigned char bytes[tileCount * TILE_BYTES], GLuint pixels[tileCount * TILE_SIZE]);