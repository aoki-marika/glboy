#pragma once

#include <stdbool.h>

#include "gl.h"

bool gbCreateTexture(GLuint *texture, GLint internalFormat, GLsizei width, GLsizei height, GLuint pixels[]);
bool gbCreateImageTexture(GLuint *texture, GLuint pixels[]);