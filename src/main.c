#include <stdio.h>

#include "game.h"
#include "image.h"
#include "gfx.h"
#include "gfx_constants.h"

GLuint gTex;

void renderMain()
{
    glBindTexture(GL_TEXTURE_2D, gTex);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f, 0.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(TILE_WIDTH, 0.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(TILE_WIDTH, TILE_HEIGHT);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(0.0f, TILE_HEIGHT);
    glEnd();
}

int main(int argc, char *argv[])
{
    if (!gbInit())
        return 1;

    unsigned char bytes[] = {
        0xFF, 0x00, 0x7E, 0xFF, 0x85, 0x81, 0x89, 0x83, 0x93, 0x85, 0xA5, 0x8B, 0xC9, 0x97, 0x7E, 0xFF
    };

    int pixels[TILE_SIZE];
    GLuint convertedPixels[TILE_SIZE];

    gbLoadImage(bytes, pixels);
    gbConvertImage(pixels, convertedPixels);
    gbCreateImageTexture(&gTex, convertedPixels);

    SDL_Color palette[PAL_SIZE] = {
        { 255, 255, 255 },
        { 170, 170, 170 },
        { 85, 85, 85 },
        { 0, 0, 0 },
    };

    gbSetPalette(palette);
    gbSetRenderCallback(renderMain);

    if (!gbRun())
        return 1;

    if (!gbQuit())
        return 1;

    glDeleteTextures(1, &gTex);

    return 0;
}