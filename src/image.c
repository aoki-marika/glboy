#include "image.h"
#include "gfx_constants.h"

void gbLoadImage(unsigned char bytes[], GLuint pixels[])
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