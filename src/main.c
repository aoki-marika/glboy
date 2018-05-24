#include <stdio.h>

#include "game.h"
#include "image.h"
#include "gfx.h"
#include "gfx_constants.h"

GLuint gEmptyTexture, gFilledTexture;

void renderMain()
{
    gbGetBackground(0)->x += 1;
    gbGetBackground(0)->y -= 1;

    // glBindTexture(GL_TEXTURE_2D, gEmptyTexture);
    // glBegin(GL_QUADS);
    //     glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f, 0.0f);
    //     glTexCoord2f(1.0f, 0.0f); glVertex2f(TILE_WIDTH, 0.0f);
    //     glTexCoord2f(1.0f, 1.0f); glVertex2f(TILE_WIDTH, TILE_HEIGHT);
    //     glTexCoord2f(0.0f, 1.0f); glVertex2f(0.0f, TILE_HEIGHT);
    // glEnd();

    // glBindTexture(GL_TEXTURE_2D, gFilledTexture);
    // glBegin(GL_QUADS);
    //     glTexCoord2f(0.0f, 0.0f); glVertex2f(TILE_WIDTH, 0.0f);
    //     glTexCoord2f(1.0f, 0.0f); glVertex2f(TILE_WIDTH * 2, 0.0f);
    //     glTexCoord2f(1.0f, 1.0f); glVertex2f(TILE_WIDTH * 2, TILE_HEIGHT);
    //     glTexCoord2f(0.0f, 1.0f); glVertex2f(TILE_WIDTH, TILE_HEIGHT);
    // glEnd();
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

    GLuint test[BG_WIDTH][BG_HEIGHT] = {
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
        { f, 0, f, e, e, e, e, e, e, e, e, e, e, e, e, e, f, e, e, e, e, e, e, e, e, e, e, e, e, e, f, f },
        { f, 0, e, f, 0, 0, 0, 0, 0, e, 0, 0, 0, 0, 0, f, e, f, 0, 0, 0, 0, 0, e, 0, 0, 0, 0, 0, f, e, f },
        { f, 0, e, 0, f, 0, 0, 0, 0, e, 0, 0, 0, 0, f, 0, e, 0, f, 0, 0, 0, 0, e, 0, 0, 0, 0, f, 0, e, f },
        { f, 0, e, 0, 0, f, 0, 0, 0, e, 0, 0, 0, f, 0, 0, e, 0, 0, f, 0, 0, 0, e, 0, 0, 0, f, 0, 0, e, f },
        { f, 0, e, 0, 0, 0, f, 0, 0, e, 0, 0, f, 0, 0, 0, e, 0, 0, 0, f, 0, 0, e, 0, 0, f, 0, 0, 0, e, f },
        { f, 0, e, 0, 0, 0, 0, f, 0, e, 0, f, 0, 0, 0, 0, e, 0, 0, 0, 0, f, 0, e, 0, f, 0, 0, 0, 0, e, f },
        { f, 0, e, 0, 0, 0, 0, 0, f, e, f, 0, 0, 0, 0, 0, e, 0, 0, 0, 0, 0, f, e, f, 0, 0, 0, 0, 0, e, f },
        { f, 0, e, e, e, e, e, e, e, f, e, e, e, e, e, e, e, e, e, e, e, e, e, f, e, e, e, e, e, e, e, f },
        { f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, f },
        { f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f },
    };

    for (int y = 0; y < BG_HEIGHT; y++)
    {
        for (int x = 0; x < BG_WIDTH; x++)
        {
            gbSetTileMapTile(gbGetBackground(0), x, y, test[y][x]);
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