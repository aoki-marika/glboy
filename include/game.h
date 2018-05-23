#pragma once

#include <stdbool.h>

#include "gl.h"

bool gbInit();
void gbSetRenderCallback(void (*callback)());
bool gbRun();
bool gbQuit();
bool gbCreateTexture(GLuint *texture, GLuint pixels[]);