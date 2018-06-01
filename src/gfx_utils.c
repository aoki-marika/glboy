#include <stdio.h>

#include "gfx_utils.h"
#include "sdl.h"
#include "gfx_constants.h"

bool gbGlError(const char *message)
{
    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        printf("Error %s! %u\n", message, error);
        return true;
    }

    return false;
}

bool gbSdlError(const char *message)
{
    const char *error = SDL_GetError();
    if (*error)
    {
        printf("Error %s! %s\n", message, error);
        SDL_ClearError();
        return true;
    }

    return false;
}

void gbSetTileMapTile(GBTileMap *map, int x, int y, int tile)
{
    map->tiles[x + (y * map->width)] = tile;
}

void gbClearTileMap(GBTileMap *map)
{
    memset(map->tiles, 0, sizeof(int) * (map->width * map->height));
}

bool gbCreateTexture(GLuint *texture, GLint internalFormat, GLsizei width, GLsizei height, GLuint pixels[])
{
    // generate the texture and set pixel data
    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    // set min/mag and wrap defaults
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // unbind the texture so geometry isnt autmatically drawn with this texture
    glBindTexture(GL_TEXTURE_2D, 0);

    // check for errors
    if (gbGlError("creating texture"))
        return false;

    return true;
}

bool gbCreateTileTexture(GLuint *texture, GLuint pixels[])
{
    return gbCreateTexture(texture, GL_LUMINANCE, TILE_WIDTH, TILE_HEIGHT, pixels);
}

bool gbCreateShader(GLuint *shader, GLenum type, const GLchar *source[])
{
    // create and compile the shader
    *shader = glCreateShader(type);
    glShaderSource(*shader, 1, source, NULL);
    glCompileShader(*shader);

    // check for compilation errors
    if (gbShaderError(*shader, GL_COMPILE_STATUS, "compiling shader"))
        return false;

    return true;
}

void gbPrintProgramLog(GLuint program)
{
    if (glIsProgram(program))
    {
        int infoLogLength = 0;
        int maxLength = infoLogLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        char infoLog[maxLength];
        glGetProgramInfoLog(program, maxLength, &infoLogLength, infoLog);

        if (infoLogLength > 0)
            printf("[Program] %s\n", infoLog);
    }
    else
        printf("Given name '%d' is not a program.\n", program);
}

void gbPrintShaderLog(GLuint shader)
{
    if (glIsShader(shader))
    {
        int infoLogLength = 0;
        int maxLength = infoLogLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

        char infoLog[maxLength];
        glGetShaderInfoLog(shader, maxLength, &infoLogLength, infoLog);

        if (infoLogLength > 0)
            printf("[Shader] %s\n", infoLog);
    }
    else
        printf("Given name '%d' is not a shader.\n", shader);
}

bool gbProgramError(GLuint program, GLenum pname, const char *message)
{
    GLint check = GL_FALSE;
    glGetProgramiv(program, pname, &check);

    if (check != GL_TRUE)
    {
        printf("Error %s '%d'!\n", message, program);
        gbPrintProgramLog(program);
        return true;
    }

    return false;
}

bool gbShaderError(GLuint shader, GLenum pname, const char *message)
{
    GLint check = GL_FALSE;
    glGetShaderiv(shader, pname, &check);

    if (check != GL_TRUE)
    {
        printf("Error %s '%d'!\n", message, shader);
        gbPrintShaderLog(shader);
        return true;
    }

    return false;
}