#include "image.h"
#include "gfx_constants.h"

void gbLoadImage(unsigned char bytes[], int pixels[])
{
    for (int y = 0; y < TILE_HEIGHT; y++)
    {
        for (int x = 0; x < TILE_WIDTH; x++)
        {
            char highBit = (bytes[y * 2 + 1] >> (TILE_WIDTH - 1 - x)) & 1;
            char lowBit = (bytes[y * 2] >> (TILE_WIDTH - 1 - x)) & 1;
            pixels[x + (y * TILE_WIDTH)] = (highBit << 1) | lowBit;
        }
    }
}

void gbConvertImage(int pixels[], GLuint convertedPixels[])
{
    for (int i = 0; i < TILE_SIZE; i++)
    {
        GLubyte *colours = (GLubyte *)&convertedPixels[i];
        colours[0] = pixels[i];
    }
}