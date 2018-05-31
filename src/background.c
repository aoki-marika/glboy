#include <stdlib.h>
#include <stdio.h>

#include "background.h"
#include "gfx_constants.h"

GBTileMap gBackgrounds[BG_COUNT];
int gActiveBackground;

bool gbBackgroundInit()
{
    for (int i = 0; i < BG_COUNT; i++)
    {
        int *tiles = (int *)calloc(BG_SIZE, sizeof(int));

        gBackgrounds[i].width = BG_WIDTH;
        gBackgrounds[i].height = BG_HEIGHT;
        gBackgrounds[i].tiles = tiles;
    }

    return true;
}

bool gbBackgroundQuit()
{
    for (int i = 0; i < BG_COUNT; i++)
        free(gBackgrounds[i].tiles);

    return true;
}

bool verifyBgIndex(int index)
{
    if (index >= BG_COUNT)
    {
        printf("Background %i is out of range (%i).\n", index, BG_COUNT);
        return false;
    }

    return true;
}

GBTileMap *gbGetBackground(int index)
{
    if (!verifyBgIndex(index))
        return NULL;

    return &gBackgrounds[index];
}

GBTileMap *gbGetActiveBackground()
{
    return gbGetBackground(gActiveBackground);
}

bool gbSetActiveBackground(int index)
{
    if (!verifyBgIndex(index))
        return false;

    gActiveBackground = index;
    return true;
}