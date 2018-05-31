#pragma once

#include <stdbool.h>

#include "gl.h"
#include "sdl.h"
#include "gfx_utils.h"
#include "gfx_constants.h"

bool gbInit();

void gbSetUpdateCallback(void (*callback)());
void gbSetRenderCallback(void (*callback)());

bool gbRun();
bool gbQuit();