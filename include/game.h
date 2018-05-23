#pragma once

#include <stdbool.h>

#include "gl.h"
#include "gfx_constants.h"

bool gbInit();
void gbSetRenderCallback(void (*callback)());
void gbSetPalette(GLuint colours[PAL_SIZE]); //todo: multiple palette types
bool gbRun();
bool gbQuit();