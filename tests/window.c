#include <glboy/game.h>
#include <glboy/image.h>
#include <glboy/input.h>

int gActiveWin;

void updateMain()
{
    GBInputState state = gbGetInputState();

    if (state.a.pressed && !state.a.repeat)
    {
        gActiveWin = !gActiveWin;
        gbSetActiveWindow(gActiveWin);
    }

    int x = 0;
    int y = 0;

    if (state.up.pressed)
        y = 1;
    else if (state.down.pressed)
        y = -1;

    if (state.left.pressed)
        x = 1;
    else if (state.right.pressed)
        x = -1;

    gbGetWindow(gActiveWin)->x += x;
    gbGetWindow(gActiveWin)->y += y;
}

int main(int argc, char *argv[])
{
    if (!gbInit())
        return 1;

    unsigned char topLeft[] = {
        0x00, 0x00, 0x3F, 0x3F, 0x6F, 0x70, 0x5F, 0x60, 0x7F, 0x40, 0x7F, 0x40, 0x7F, 0x40, 0x7F, 0x40
    };

    unsigned char topRight[] = {
        0x00, 0x00, 0xFC, 0xFC, 0xF6, 0x0E, 0xFA, 0x06, 0xFE, 0x02, 0xFE, 0x02, 0xFE, 0x02, 0xFE, 0x02
    };

    unsigned char bottomLeft[] = {
        0x7F, 0x40, 0x7F, 0x40, 0x7F, 0x40, 0x7F, 0x40, 0x5F, 0x60, 0x6F, 0x70, 0x3F, 0x3F, 0x00, 0x00
    };

    unsigned char bottomRight[] = {
        0xFE, 0x02, 0xFE, 0x02, 0xFE, 0x02, 0xFE, 0x02, 0xFA, 0x06, 0xF6, 0x0E, 0xFC, 0xFC, 0x00, 0x00
    };

    unsigned char top[] = {
        0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00
    };

    unsigned char bottom[] = {
        0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00
    };

    unsigned char left[] = {
        0x7F, 0x40, 0x7F, 0x40, 0x7F, 0x40, 0x7F, 0x40, 0x7F, 0x40, 0x7F, 0x40, 0x7F, 0x40, 0x7F, 0x40
    };

    unsigned char right[] = {
        0xFE, 0x02, 0xFE, 0x02, 0xFE, 0x02, 0xFE, 0x02, 0xFE, 0x02, 0xFE, 0x02, 0xFE, 0x02, 0xFE, 0x02
    };

    unsigned char text[] = {
        0xFF, 0x00, 0x28, 0xD7, 0xFF, 0x00, 0x14, 0xEB, 0xFF, 0x00, 0x21, 0xDE, 0xFF, 0x00, 0x80, 0x7F
    };

    GLuint topLeftPixels[TILE_SIZE];
    GLuint topRightPixels[TILE_SIZE];
    GLuint bottomLeftPixels[TILE_SIZE];
    GLuint bottomRightPixels[TILE_SIZE];
    GLuint topPixels[TILE_SIZE];
    GLuint bottomPixels[TILE_SIZE];
    GLuint leftPixels[TILE_SIZE];
    GLuint rightPixels[TILE_SIZE];
    GLuint textPixels[TILE_SIZE];

    gbParseTile(topLeft, topLeftPixels);
    gbParseTile(topRight, topRightPixels);
    gbParseTile(bottomLeft, bottomLeftPixels);
    gbParseTile(bottomRight, bottomRightPixels);
    gbParseTile(top, topPixels);
    gbParseTile(bottom, bottomPixels);
    gbParseTile(left, leftPixels);
    gbParseTile(right, rightPixels);
    gbParseTile(text, textPixels);

    int tl = 1, tr = 2, bl = 3, br = 4, t = 5, b = 6, l = 7, r = 8, x = 9;

    if (!gbSetTileData(TILE_DATA_BG, tl, topLeftPixels) ||
        !gbSetTileData(TILE_DATA_BG, tr, topRightPixels) ||
        !gbSetTileData(TILE_DATA_BG, bl, bottomLeftPixels) ||
        !gbSetTileData(TILE_DATA_BG, br, bottomRightPixels) ||
        !gbSetTileData(TILE_DATA_BG, t, topPixels) ||
        !gbSetTileData(TILE_DATA_BG, b, bottomPixels) ||
        !gbSetTileData(TILE_DATA_BG, l, leftPixels) ||
        !gbSetTileData(TILE_DATA_BG, r, rightPixels) ||
        !gbSetTileData(TILE_DATA_BG, x, textPixels))
        return 1;

    SDL_Color colours[PAL_LENGTH] = {
        { 222, 248, 210 },
        { 130, 192, 117 },
        { 43, 104, 86 },
        { 5, 24, 32 },
    };

    int palette[PAL_LENGTH] = {
        PAL_WHITE,
        PAL_LGREY,
        PAL_DGREY,
        PAL_BLACK,
    };

    gbSetColours(colours);
    gbSetBackgroundPalette(palette);
    gbSetUpdateCallback(updateMain);

    // window one
    const int testWinOneWidth = TILES_X;
    const int testWinOneHeight = 5;

    gbGetWindow(0)->y = (TILES_Y - testWinOneHeight) * TILE_HEIGHT;
    gbGetWindow(0)->width = testWinOneWidth;
    gbGetWindow(0)->height = testWinOneHeight;

    int *testWinOneTiles = (int *)calloc(testWinOneWidth * testWinOneHeight, sizeof(int));
    gbGetWindow(0)->tiles = testWinOneTiles;

    int testWinOne[testWinOneHeight][testWinOneWidth] = {
        { tl, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, tr },
        { l, x, x, x, x, x, x, x, x, x, x, x, x, x, x, x, x, x, x, r },
        { l, x, x, x, x, x, x, x, x, x, x, x, x, x, x, x, x, x, x, r },
        { l, x, x, x, x, x, x, x, x, x, x, x, x, x, x, x, x, x, x, r },
        { bl, b, b, b, b, b, b, b, b, b, b, b, b, b, b, b, b, b, b, br },
    };

    for (int y = 0; y < testWinOneHeight; y++)
    {
        for (int x = 0; x < testWinOneWidth; x++)
        {
            gbSetTileMapTile(gbGetWindow(0), x, y, testWinOne[y][x]);
        }
    }

    // window two
    const int testWinTwoWidth = 5;
    const int testWinTwoHeight = 5;

    gbGetWindow(1)->width = testWinTwoWidth;
    gbGetWindow(1)->height = testWinTwoHeight;

    int *testWinTwoTiles = (int *)calloc(testWinTwoWidth * testWinTwoHeight, sizeof(int));
    gbGetWindow(1)->tiles = testWinTwoTiles;

    int testWinTwo[testWinTwoHeight][testWinTwoWidth] = {
        { tl, t, t, t, tr },
        { l, x, x, x, r },
        { l, x, x, x, r },
        { l, x, x, x, r },
        { bl, b, b, b, br },
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

    free(testWinOneTiles);
    free(testWinTwoTiles);

    if (!gbRun())
        return 1;

    if (!gbQuit())
        return 1;

    return 0;
}