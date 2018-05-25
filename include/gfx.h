#pragma once

#include <stdbool.h>

#include "gl.h"

typedef struct
{
    int width;
    int height;
    int x;
    int y;
    int *tiles;
} GBTileMap;

typedef enum
{
    GBSpritePriorityAbove = 0, //on top of the window and background
    GBSpritePriorityBelow = 1, //hidden behind colours 1-3 of the window and background (not 0)
} GBSpritePriority;

typedef struct
{
    int x;
    int y;
    int tile;
    int palette; //index of the sprite palette to use
    bool flipX;
    bool flipY;
    GBSpritePriority priority;
} GBSprite;

void gbSetTileMapTile(GBTileMap *map, int x, int y, int tile);

bool gbCreateTexture(GLuint *texture, GLint internalFormat, GLsizei width, GLsizei height, GLuint pixels[]);
bool gbCreateImageTexture(GLuint *texture, GLuint pixels[]);
bool gbCreateShader(GLuint *shader, GLenum type, const GLchar *source[]);

void gbPrintProgramLog(GLuint program);
void gbPrintShaderLog(GLuint shader);
bool gbProgramError(GLuint program, GLenum pname, const char *message);
bool gbShaderError(GLuint shader, GLenum pname, const char *message);