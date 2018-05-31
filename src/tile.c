#include <stdlib.h>
#include <stdio.h>

#include "tile.h"
#include "gfx_utils.h"

GLuint *gTileData[TILE_DATA_COUNT];

bool gbTileInit()
{
    for (int i = 0; i < TILE_DATA_COUNT; i++)
        gTileData[i] = (GLuint *)calloc(TILE_DATA_TILE_COUNT, sizeof(GLuint));

    return true;
}

bool gbTileQuit()
{
    for (int i = 0; i < TILE_DATA_COUNT; i++)
    {
        glDeleteTextures(TILE_DATA_TILE_COUNT, gTileData[i]);
        free(gTileData[i]);
    }

    return true;
}

bool verifyType(int type)
{
    if (type >= TILE_DATA_COUNT)
    {
        printf("Tile data type %i is out of range (%i).\n", type, TILE_DATA_COUNT);
        return false;
    }

    return true;
}

bool verifyDataIndex(int index)
{
    if (index >= TILE_DATA_TILE_COUNT)
    {
        printf("Tile data index %i is out of range (%i).\n", index, TILE_DATA_TILE_COUNT);
        return false;
    }

    return true;
}

GLuint gbGetTileData(int type, int index)
{
    if (!verifyType(type))
        return false;

    if (!verifyDataIndex(index))
        return false;

    return gTileData[type][index];
}

bool gbSetTileData(int type, int index, GLuint data[TILE_SIZE])
{
    if (!verifyType(type))
        return false;

    if (!verifyDataIndex(index))
        return false;

    GLuint *tile = &gTileData[type][index];

    // delete the existing texture (if there is one) and create the new one
    glDeleteTextures(1, tile);
    gbCreateTileTexture(tile, data);

    return true;
}

bool gbSetTileDataMultiple(int type, int index, int count, GLuint data[count * TILE_SIZE])
{
    if (!verifyType(type))
        return false;

    // iterate through every TILE_SIZE pixels in data and load the tile data within
    for (int i = 0; i < count; i++)
    {
        GLuint d[TILE_SIZE];

        for (int di = 0; di < TILE_SIZE; di++)
            d[di] = data[(i * TILE_SIZE) + di];

        if (!gbSetTileData(type, index + i, d))
            return false;
    }

    return true;
}

float scaleForFlip(bool flip)
{
    return flip ? -1.0f : 1.0f;
}

float translateForFlip(bool flip, int pos, int size)
{
    return flip ? -pos * 2 - size : 0.0f;
}

bool gbRenderTile(int type, int index, int x, int y, float z, bool flipX, bool flipY)
{
    if (!verifyType(type))
        return false;

    if (!verifyDataIndex(index))
        return false;

    glBindTexture(GL_TEXTURE_2D, gTileData[type][index]);

    if (flipX || flipY)
    {
        glPushMatrix();
            glScalef(scaleForFlip(flipX), scaleForFlip(flipY), 1.0f);
            glTranslatef(translateForFlip(flipX, x, TILE_WIDTH), translateForFlip(flipY, y, TILE_HEIGHT), 0.0f);
    }

    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(x + TILE_WIDTH, y, z);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(x + TILE_WIDTH, y + TILE_HEIGHT, z);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + TILE_HEIGHT, z);
    glEnd();

    if (flipX || flipY)
        glPopMatrix();

    return true;
}