#include <stdio.h>

#include "game.h"
#include "image.h"
#include "gfx.h"
#include "gfx_constants.h"

GLuint gEmptyTexture, gFilledTexture;

int gBg;

void renderMain()
{
    if (gBg == 0)
    {
        gbGetBackground(0)->x += 1;
        gbGetBackground(0)->y -= 1;
    }
    if (gBg == 1)
    {
        gbGetBackground(1)->x -= 1;
        gbGetBackground(1)->y += 1;
    }

    gbSetActiveBackground(gBg);
    gBg = !gBg;
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

    gbLoadImage(empty, emptyPixels);
    gbCreateImageTexture(&gEmptyTexture, emptyPixels);

    gbLoadImage(filled, filledPixels);
    gbCreateImageTexture(&gFilledTexture, filledPixels);

    SDL_Color colours[PAL_SIZE] = {
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

    int palette[PAL_SIZE] = {
        PAL_WHITE,
        PAL_LGREY,
        PAL_DGREY,
        PAL_BLACK,
    };

    gbSetColours(colours);
    gbSetPalette(palette);
    gbSetRenderCallback(renderMain);

    GLuint e = gEmptyTexture;
    GLuint f = gFilledTexture;

    GLuint testOne[BG_HEIGHT][BG_WIDTH] = {
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

    GLuint testTwo[BG_HEIGHT][BG_WIDTH] = {
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
            gbSetTileMapTile(gbGetBackground(0), x, y, testOne[y][x]);
            gbSetTileMapTile(gbGetBackground(1), x, y, testTwo[y][x]);
        }
    }

    if (!gbRun())
        return 1;

    if (!gbQuit())
        return 1;

    glDeleteTextures(1, &gEmptyTexture);
    glDeleteTextures(1, &gFilledTexture);

    return 0;
}