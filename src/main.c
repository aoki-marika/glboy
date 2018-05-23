#include <stdio.h>

#include "game.h"
#include "image.h"
#include "gfx.h"
#include "gfx_constants.h"

GLuint gWindowTexture, gATexture;

void renderMain()
{
    glBindTexture(GL_TEXTURE_2D, gWindowTexture);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f, 0.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(TILE_WIDTH, 0.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(TILE_WIDTH, TILE_HEIGHT);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(0.0f, TILE_HEIGHT);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, gATexture);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(TILE_WIDTH, 0.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(TILE_WIDTH * 2, 0.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(TILE_WIDTH * 2, TILE_HEIGHT);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(TILE_WIDTH, TILE_HEIGHT);
    glEnd();
}

int main(int argc, char *argv[])
{
    if (!gbInit())
        return 1;

    unsigned char window[] = {
        0xFF, 0x00, 0x7E, 0xFF, 0x85, 0x81, 0x89, 0x83, 0x93, 0x85, 0xA5, 0x8B, 0xC9, 0x97, 0x7E, 0xFF
    };

    unsigned char a[] = {
        0x7C, 0x7C, 0x00, 0xC6, 0xC6, 0x00, 0x00, 0xFE, 0xC6, 0xC6, 0x00, 0xC6, 0xC6, 0x00, 0x00, 0x00
    };

    GLuint windowPixels[TILE_SIZE];
    GLuint aPixels[TILE_SIZE];

    gbLoadImage(window, windowPixels);
    gbCreateImageTexture(&gWindowTexture, windowPixels);

    gbLoadImage(a, aPixels);
    gbCreateImageTexture(&gATexture, aPixels);

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

    for (int y = 0; y < BG_HEIGHT; y++)
    {
        for (int x = 0; x < BG_WIDTH; x++)
        {
            gbSetTileMapTile(gbGetBackground(0), x, y, y % 2 ? gWindowTexture : gATexture);
        }
    }

    if (!gbRun())
        return 1;

    if (!gbQuit())
        return 1;

    glDeleteTextures(1, &gWindowTexture);
    glDeleteTextures(1, &gATexture);

    return 0;
}