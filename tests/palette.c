#include <glboy/game.h>
#include <glboy/image.h>
#include <glboy/input.h>
#include <glboy/palette.h>
#include <glboy/background.h>

void defaultColours()
{
    SDL_Color colours[PAL_LENGTH] = {
        { 222, 248, 210 },
        { 130, 192, 117 },
        { 43, 104, 86 },
        { 5, 24, 32 },
    };

    gbSetColours(colours);
}

void defaultPalette()
{
    int palette[PAL_LENGTH] = {
        PAL_WHITE,
        PAL_LGREY,
        PAL_DGREY,
        PAL_BLACK,
    };

    gbSetBackgroundPalette(palette);
}

Uint8 randChannel()
{
    return rand() % 256;
}

SDL_Color randColour()
{
    SDL_Color c = { randChannel(), randChannel(), randChannel() };
    return c;
}

void updateMain()
{
    GBInputState state = gbGetInputState();

    if (state.start.pressed && !state.start.repeat)
        defaultColours();

    if (state.select.pressed && !state.select.repeat)
        defaultPalette();

    if (state.a.pressed && !state.a.repeat)
    {
        int palette[PAL_LENGTH] = {
            rand() % PAL_LENGTH,
            rand() % PAL_LENGTH,
            rand() % PAL_LENGTH,
            rand() % PAL_LENGTH,
        };

        gbSetBackgroundPalette(palette);
    }

    if (state.b.pressed && !state.b.repeat)
    {
        SDL_Color colours[PAL_LENGTH] = {
            randColour(),
            randColour(),
            randColour(),
            randColour(),
        };

        gbSetColours(colours);
    }
}

int main(int argc, char *argv[])
{
    if (!gbInit())
        return 1;

    unsigned char tile[TILE_BYTES] = {
        0x33, 0x0F, 0x33, 0x0F, 0x33, 0x0F, 0x33, 0x0F, 0x33, 0x0F, 0x33, 0x0F, 0x33, 0x0F, 0x33, 0x0F
    };

    GLuint pixels[TILE_SIZE];
    gbParseTile(tile, pixels);

    if (!gbSetTileData(TILE_DATA_BG, 0, pixels))
        return 1;

    defaultColours();
    defaultPalette();
    gbSetUpdateCallback(updateMain);

    if (!gbRun())
        return 1;

    if (!gbQuit())
        return 1;

    return 0;
}