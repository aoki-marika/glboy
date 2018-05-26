#pragma once

#include <stdbool.h>

#include "gl.h"
#include "sdl.h"
#include "gfx.h"
#include "gfx_constants.h"

typedef struct
{
    bool up, down, left, right;
    bool a, b, start, select;
} GBInputState;

bool gbInit();

void gbSetUpdateCallback(void (*callback)());
void gbSetRenderCallback(void (*callback)());
GBInputState gbGetInputState();

void gbSetColours(SDL_Color colours[PAL_LENGTH]); //screen colours

// map palette index to colour index
void gbSetBackgroundPalette(int palette[PAL_LENGTH]); //background and window
bool gbSetSpritePalette(int index, int palette[PAL_LENGTH]);

bool gbSetTileData(int type, int index, GLuint data[]);

GBTileMap *gbGetBackground(int index);
bool gbSetActiveBackground(int index);

GBTileMap *gbGetWindow(int index);
bool gbSetActiveWindow(int index);

bool gbAddSprite(GBSprite *sprite);
bool gbRemoveSprite(GBSprite *sprite);

bool gbRun();
bool gbQuit();