#include <stdio.h>

#include "image.h"

void gbParseTile(unsigned char bytes[TILE_BYTES], GLuint pixels[TILE_SIZE])
{
    for (int y = 0; y < TILE_HEIGHT; y++)
    {
        for (int x = 0; x < TILE_WIDTH; x++)
        {
            char highBit = (bytes[y * 2 + 1] >> (TILE_WIDTH - 1 - x)) & 1;
            char lowBit = (bytes[y * 2] >> (TILE_WIDTH - 1 - x)) & 1;

            GLubyte *colours = (GLubyte *)&pixels[x + (y * TILE_WIDTH)];
            colours[0] = (highBit << 1) | lowBit;
        }
    }
}

void gbParseImage(int tileCount, unsigned char bytes[tileCount * TILE_BYTES], GLuint pixels[tileCount * TILE_SIZE])
{
    for (int i = 0; i < tileCount; i++)
    {
        unsigned char b[TILE_BYTES];

        for (int bi = 0; bi < TILE_BYTES; bi++)
            b[bi] = bytes[(i * TILE_BYTES) + bi];

        GLuint p[TILE_SIZE];
        gbParseTile(b, p);

        for (int pi = 0; pi < TILE_SIZE; pi++)
            pixels[(i * TILE_SIZE) + pi] = p[pi];
    }
}