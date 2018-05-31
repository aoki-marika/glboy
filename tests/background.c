#include <stdio.h>
#include <glboy/game.h>
#include <glboy/image.h>
#include <glboy/input.h>
#include <glboy/gfx_utils.h>

int gActiveBg;

void updateMain()
{
    GBInputState state = gbGetInputState();

    if (state.a.pressed && !state.a.repeat)
    {
        gActiveBg = !gActiveBg;
        gbSetActiveBackground(gActiveBg);
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

    gbGetBackground(gActiveBg)->x += x;
    gbGetBackground(gActiveBg)->y += y;
}

int main(int argc, char *argv[])
{
    if (!gbInit())
        return 1;

    const int tileCount = 2;
    unsigned char tiles[tileCount * TILE_BYTES] = {
        0x00, 0x00, 0x7E, 0x7E, 0x62, 0x46, 0x42, 0x42, 0x42, 0x42, 0x46, 0x62, 0x7E, 0x7E, 0x00, 0x00,
        0x00, 0x00, 0x7E, 0x7E, 0x42, 0x7E, 0x5A, 0x66, 0x5A, 0x66, 0x42, 0x7E, 0x7E, 0x7E, 0x00, 0x00,
    };

    GLuint pixels[tileCount * TILE_SIZE];
    gbParseImage(tileCount, tiles, pixels);

    if (!gbSetTileDataMultiple(TILE_DATA_BG, 1, tileCount, pixels))
        return 1;

    int e = 1, f = 2;

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

    int testBgOne[BG_HEIGHT][BG_WIDTH] = {
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

    int testBgTwo[BG_HEIGHT][BG_WIDTH] = {
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

    if (!gbRun())
        return 1;

    if (!gbQuit())
        return 1;

    return 0;
}