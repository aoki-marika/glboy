#include <stdio.h>

#include "game.h"
#include "image.h"
#include "gfx.h"
#include "gfx_constants.h"

int gActive;

void renderMain()
{
    if (gActive == 0)
    {
        gbGetBackground(0)->x += 1;
        gbGetBackground(0)->y -= 1;

        gbGetWindow(0)->y -= 1;
    }
    if (gActive == 1)
    {
        gbGetBackground(1)->x -= 1;
        gbGetBackground(1)->y += 1;

        gbGetWindow(1)->x += 1;
        gbGetWindow(1)->y += 1;
    }

    gbSetActiveBackground(gActive);
    gbSetActiveWindow(gActive);
    gActive = !gActive;
}

int main(int argc, char *argv[])
{
    if (!gbInit())
        return 1;

    unsigned char empty[] = {
        0x00, 0x00, 0x7E, 0x7E, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x7E, 0x7E, 0x00, 0x00
    };

    unsigned char filled[] = {
        0x00, 0x00, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x00, 0x00
    };

    GLuint emptyPixels[TILE_SIZE];
    GLuint filledPixels[TILE_SIZE];

    GLuint e, f;

    gbLoadImage(empty, emptyPixels);
    gbCreateImageTexture(&e, emptyPixels);

    gbLoadImage(filled, filledPixels);
    gbCreateImageTexture(&f, filledPixels);

    SDL_Color colours[PAL_COUNT] = {
        // original
        // { 255, 255, 255 },
        // { 170, 170, 170 },
        // { 85, 85, 85 },
        // { 0, 0, 0 },

        // emulated
        { 222, 248, 210 },
        { 130, 192, 117 },
        { 43, 104, 86 },
        { 5, 24, 32 },
    };

    int palette[PAL_COUNT] = {
        PAL_WHITE,
        PAL_LGREY,
        PAL_DGREY,
        PAL_BLACK,
    };

    gbSetColours(colours);
    gbSetPalette(palette);
    gbSetRenderCallback(renderMain);

    // test backgrounds

    GLuint testBgOne[BG_HEIGHT][BG_WIDTH] = {
        { f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f },
        { f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, f },
        { f, 0, f, 0, 0, f, 0, f, f, f, f, 0, f, 0, 0, 0, 0, f, 0, 0, 0, 0, 0, f, f, 0, 0, 0, f, 0, 0, f },
        { f, 0, f, 0, 0, f, 0, f, 0, 0, 0, 0, f, 0, 0, 0, 0, f, 0, 0, 0, 0, f, 0, 0, f, 0, f, 0, f, 0, f },
        { f, 0, f, 0, 0, f, 0, f, 0, 0, 0, 0, f, 0, 0, 0, 0, f, 0, 0, 0, 0, f, 0, 0, f, 0, 0, 0, f, 0, f },
        { f, 0, f, f, f, f, 0, f, f, f, 0, 0, f, 0, 0, 0, 0, f, 0, 0, 0, 0, f, 0, 0, f, 0, 0, 0, f, 0, f },
        { f, 0, f, 0, 0, f, 0, f, 0, 0, 0, 0, f, 0, 0, 0, 0, f, 0, 0, 0, 0, f, 0, 0, f, 0, 0, f, 0, 0, f },
        { f, 0, f, 0, 0, f, 0, f, 0, 0, 0, 0, f, 0, 0, 0, 0, f, 0, 0, 0, 0, f, 0, 0, f, 0, 0, 0, 0, 0, f },
        { f, 0, f, 0, 0, f, 0, f, f, f, f, 0, f, f, f, f, 0, f, f, f, f, 0, 0, f, f, 0, 0, 0, e, 0, 0, f },
        { f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, f },
        { f, 0, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, 0, f },
        { f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, f },
        { f, 0, f, 0, 0, f, 0, 0, f, f, 0, 0, f, f, f, 0, 0, f, 0, 0, 0, 0, f, f, f, 0, 0, 0, f, 0, 0, f },
        { f, 0, f, 0, 0, f, 0, f, 0, 0, f, 0, f, 0, 0, f, 0, f, 0, 0, 0, 0, f, 0, 0, f, 0, 0, f, 0, 0, f },
        { f, 0, f, 0, 0, f, 0, f, 0, 0, f, 0, f, 0, 0, f, 0, f, 0, 0, 0, 0, f, 0, 0, f, 0, 0, f, 0, 0, f },
        { f, 0, f, 0, 0, f, 0, f, 0, 0, f, 0, f, f, f, 0, 0, f, 0, 0, 0, 0, f, 0, 0, f, 0, 0, f, 0, 0, f },
        { f, 0, f, 0, f, f, 0, f, 0, 0, f, 0, f, 0, 0, f, 0, f, 0, 0, 0, 0, f, 0, 0, f, 0, 0, f, 0, 0, f },
        { f, 0, f, f, f, f, 0, f, 0, 0, f, 0, f, 0, 0, f, 0, f, 0, 0, 0, 0, f, 0, 0, f, 0, 0, 0, 0, 0, f },
        { f, 0, f, 0, 0, f, 0, 0, f, f, 0, 0, f, 0, 0, f, 0, f, f, f, f, 0, f, f, f, 0, 0, 0, e, 0, 0, f },
        { f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, f },
        { f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, f },
        { f, 0, f, e, e, e, e, e, e, e, e, e, e, e, e, e, f, e, e, e, e, e, e, e, e, e, e, e, e, e, f, f },
        { f, 0, e, f, 0, 0, 0, 0, 0, e, 0, 0, 0, 0, 0, f, e, f, 0, 0, 0, 0, 0, e, 0, 0, 0, 0, 0, f, e, f },
        { f, 0, e, 0, f, 0, 0, 0, 0, e, 0, 0, 0, 0, f, 0, e, 0, f, 0, 0, 0, 0, e, 0, 0, 0, 0, f, 0, e, f },
        { f, 0, e, 0, 0, f, 0, 0, 0, e, 0, 0, 0, f, 0, 0, e, 0, 0, f, 0, 0, 0, e, 0, 0, 0, f, 0, 0, e, f },
        { f, 0, e, 0, 0, 0, f, 0, 0, e, 0, 0, f, 0, 0, 0, e, 0, 0, 0, f, 0, 0, e, 0, 0, f, 0, 0, 0, e, f },
        { f, 0, e, 0, 0, 0, 0, f, 0, e, 0, f, 0, 0, 0, 0, e, 0, 0, 0, 0, f, 0, e, 0, f, 0, 0, 0, 0, e, f },
        { f, 0, e, 0, 0, 0, 0, 0, f, e, f, 0, 0, 0, 0, 0, e, 0, 0, 0, 0, 0, f, e, f, 0, 0, 0, 0, 0, e, f },
        { f, 0, e, e, e, e, e, e, e, f, e, e, e, e, e, e, e, e, e, e, e, e, e, f, e, e, e, e, e, e, e, f },
        { f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, f },
        { f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, f },
        { f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f },
    };

    GLuint testBgTwo[BG_HEIGHT][BG_WIDTH] = {
        { f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f },
        { f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, f },
        { f, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f },
        { f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, f },
        { f, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f },
        { f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, f },
        { f, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f },
        { f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, f },
        { f, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f },
        { f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, f },
        { f, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f },
        { f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, f },
        { f, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f },
        { f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, f },
        { f, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f },
        { f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, f },
        { f, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f },
        { f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, f },
        { f, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f },
        { f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, f },
        { f, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f },
        { f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, f },
        { f, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f },
        { f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, f },
        { f, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f },
        { f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, f },
        { f, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f },
        { f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, f },
        { f, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f },
        { f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, f },
        { f, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f, 0, f },
        { f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f },
    };

    for (int y = 0; y < BG_HEIGHT; y++)
    {
        for (int x = 0; x < BG_WIDTH; x++)
        {
            gbSetTileMapTile(gbGetBackground(0), x, y, testBgOne[y][x]);
            gbSetTileMapTile(gbGetBackground(1), x, y, testBgTwo[y][x]);
        }
    }

    // test window one

    const int testWinOneWidth = TILES_X;
    const int testWinOneHeight = 6;

    gbGetWindow(0)->y = (TILES_Y - testWinOneHeight) * TILE_HEIGHT;
    gbGetWindow(0)->width = testWinOneWidth;
    gbGetWindow(0)->height = testWinOneHeight;

    GLuint *testWinOneTiles = (GLuint *)calloc(testWinOneWidth * testWinOneHeight, sizeof(GLuint));
    gbGetWindow(0)->tiles = testWinOneTiles;

    GLuint testWinOne[testWinOneHeight][testWinOneWidth] = {
        { f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f },
        { f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, f },
        { f, 0, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, 0, f },
        { f, 0, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, 0, f },
        { f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, f },
        { f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f },
    };

    for (int y = 0; y < testWinOneHeight; y++)
    {
        for (int x = 0; x < testWinOneWidth; x++)
        {
            gbSetTileMapTile(gbGetWindow(0), x, y, testWinOne[y][x]);
        }
    }

    // test window two

    const int testWinTwoWidth = 5;
    const int testWinTwoHeight = 5;

    gbGetWindow(1)->width = testWinTwoWidth;
    gbGetWindow(1)->height = testWinTwoHeight;

    GLuint *testWinTwoTiles = (GLuint *)calloc(testWinTwoWidth * testWinTwoHeight, sizeof(GLuint));
    gbGetWindow(1)->tiles = testWinTwoTiles;

    GLuint testWinTwo[testWinTwoHeight][testWinTwoWidth] = {
        { f, f, f, f, f },
        { f, 0, 0, 0, f },
        { f, 0, e, 0, f },
        { f, 0, 0, 0, f },
        { f, f, f, f, f },
    };

    for (int y = 0; y < testWinTwoHeight; y++)
    {
        for (int x = 0; x < testWinTwoWidth; x++)
        {
            gbSetTileMapTile(gbGetWindow(1), x, y, testWinTwo[y][x]);
        }
    }

    if (!gbRun())
        return 1;

    if (!gbQuit())
        return 1;

    glDeleteTextures(1, &e);
    glDeleteTextures(1, &f);

    free(testWinOneTiles);
    free(testWinTwoTiles);

    return 0;
}