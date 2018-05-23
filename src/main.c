#include <stdio.h>

#include "game.h"
#include "image.h"
#include "gfx_constants.h"

int main(int argc, char *argv[])
{
    if (!gbInit())
        return 1;

    unsigned char bytes[] = {
        0xFF, 0x00, 0x7E, 0xFF, 0x85, 0x81, 0x89, 0x83, 0x93, 0x85, 0xA5, 0x8B, 0xC9, 0x97, 0x7E, 0xFF
    };

    int pixels[TILE_SIZE];
    gbLoadImage(bytes, pixels);

    if (!gbRun())
        return 1;

    if (!gbQuit())
        return 1;

    return 0;
}