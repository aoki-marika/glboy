#pragma once

#include <stdbool.h>

#include "gl.h"

typedef struct
{
    int width;
    int height;
    int x;
    int y;
    GLuint *tiles;
} GBTileMap;

void gbSetTileMapTile(GBTileMap *map, int x, int y, GLuint tile);

bool gbCreateTexture(GLuint *texture, GLint internalFormat, GLsizei width, GLsizei height, GLuint pixels[]);
bool gbCreateImageTexture(GLuint *texture, GLuint pixels[]);
bool gbCreateShader(GLuint *shader, GLenum type, const GLchar *source[]);

void gbPrintProgramLog(GLuint program);
void gbPrintShaderLog(GLuint shader);
bool gbProgramError(GLuint program, GLenum pname, const char *message);
bool gbShaderError(GLuint shader, GLenum pname, const char *message);