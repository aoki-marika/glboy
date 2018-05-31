#include <stdio.h>

#include "window.h"
#include "palette.h"
#include "gfx_constants.h"

GBTileMap gWindows[WIN_COUNT];
int gActiveWindow;

bool verifyWinIndex(int index)
{
    if (index >= WIN_COUNT)
    {
        printf("Window %i is out of range (%i).\n", index, WIN_COUNT);
        return false;
    }

    return true;
}

GBTileMap *gbGetWindow(int index)
{
    if (!verifyWinIndex(index))
        return NULL;

    return &gWindows[index];
}

GBTileMap *gbGetActiveWindow()
{
    return gbGetWindow(gActiveWindow);
}

bool gbSetActiveWindow(int index)
{
    if (!verifyWinIndex(index))
        return false;

    gActiveWindow = index;
    return true;
}

bool gbRenderWindow()
{
    gbSetActivePalette(GBPaletteTypeBackground, 0);
    gbSetPaletteMode(GBPaletteModeWindow);

    return gbRenderTileMap(gbGetActiveWindow(), TILE_DATA_BG, false);
}