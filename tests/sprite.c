#include <glboy/game.h>
#include <glboy/image.h>
#include <glboy/input.h>
#include <glboy/palette.h>
#include <glboy/background.h>
#include <glboy/window.h>
#include <glboy/tile.h>
#include <glboy/sprite.h>

const int test_sprite_count = 2;

GBSprite *gSprites;
int gActiveSprite;
GBSpriteMode gMode;

void updateMain()
{
    GBInputState state = gbGetInputState();

    if (state.a.pressed && !state.a.repeat)
        gActiveSprite = !gActiveSprite;

    if (state.b.pressed && !state.b.repeat)
    {
        gMode = !gMode;
        gbSetSpriteMode(gMode);
    }

    int x = 0;
    int y = 0;

    if (state.up.pressed)
        y = -1;
    else if (state.down.pressed)
        y = 1;

    if (state.left.pressed)
        x = -1;
    else if (state.right.pressed)
        x = 1;

    gSprites[gActiveSprite].x += x;
    gSprites[gActiveSprite].y += y;
}

int main(int argc, char *argv[])
{
    if (!gbInit())
        return 1;

    const int tileCount = 4;
    unsigned char tiles[tileCount * TILE_BYTES] = {
        0x20, 0x1C, 0x40, 0x3E, 0x80, 0x7F, 0x00, 0xFF, 0x00, 0xFF, 0x01, 0xFF, 0x02, 0x7E, 0x04, 0x3C,
        0x00, 0x00, 0x00, 0xC3, 0x00, 0x7E, 0x18, 0x24, 0x18, 0x24, 0x18, 0x24, 0x18, 0x66, 0x00, 0xFF,
        0x33, 0x0F, 0x33, 0x0F, 0x33, 0x0F, 0x33, 0x0F, 0x33, 0x0F, 0x33, 0x0F, 0x33, 0x0F, 0x33, 0x0F,
        0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    };

    GLuint pixels[tileCount * TILE_SIZE];
    gbParseImage(tileCount, tiles, pixels);

    if (!gbSetTileDataMultiple(TILE_DATA_SPRITE, 0, tileCount, pixels) ||
        !gbSetTileDataMultiple(TILE_DATA_BG, 1, tileCount, pixels))
        return 1;

    int ball = 0, gradient = 1;

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

    int spritePaletteOne[PAL_LENGTH] = {
        PAL_WHITE,
        PAL_BLACK,
        PAL_DGREY,
        PAL_LGREY,
    };

    int spritePaletteTwo[PAL_LENGTH] = {
        PAL_BLACK,
        PAL_WHITE,
        PAL_LGREY,
        PAL_DGREY,
    };

    gbSetColours(colours);
    gbSetBackgroundPalette(palette);
    gbSetSpritePalette(0, spritePaletteOne);
    gbSetSpritePalette(1, spritePaletteTwo);
    gbSetSpriteMode(GBSpriteMode8x8);
    gbSetUpdateCallback(updateMain);

    // setup background and window for priority testing
    for (int i = 0; i < BG_SIZE / 4; i++)
        gbGetBackground(0)->tiles[i] = 1 + (gradient * 2);

    const int winWidth = 5;
    const int winHeight = 5;

    gbGetWindow(0)->width = winWidth;
    gbGetWindow(0)->height = winHeight;

    int *winTiles = (int *)calloc(winWidth * winHeight, sizeof(int));
    gbGetWindow(0)->tiles = winTiles;

    for (int i = 0; i < winWidth * winHeight; i++)
        gbGetWindow(0)->tiles[i] = 1 + (ball * 2);

    // test sprites
    GBSprite sprites[] = {
        {
            .x = 10,
            .y = 10,
            .tile = ball,
            .palette = 0,
            .flipX = true,
        },
        {
            .x = 35,
            .y = 10,
            .tile = gradient,
            .palette = 1,
            .flipX = true,
            .flipY = true,
            .priority = GBSpritePriorityBelow,
        },
    };

    gSprites = sprites;

    for (int i = 0; i < test_sprite_count; i++)
        if (!gbAddSprite(&gSprites[i]))
            return 1;

    if (!gbRun())
        return 1;

    if (!gbQuit())
        return 1;

    free(winTiles);

    return 0;
}