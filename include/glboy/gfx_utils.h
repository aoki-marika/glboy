#pragma once

#include <stdbool.h>

#include "gl.h"
#include "tile.h"

bool gbGlError(const char *message);
bool gbSdlError(const char *message);

void gbSetTileMapTile(GBTileMap *map, int x, int y, int tile);

bool gbCreateTexture(GLuint *texture, GLint internalFormat, GLsizei width, GLsizei height, GLuint pixels[]);
bool gbCreateTileTexture(GLuint *texture, GLuint pixels[]);
bool gbCreateShader(GLuint *shader, GLenum type, const GLchar *source[]);

void gbPrintProgramLog(GLuint program);
void gbPrintShaderLog(GLuint shader);
bool gbProgramError(GLuint program, GLenum pname, const char *message);
bool gbShaderError(GLuint shader, GLenum pname, const char *message);