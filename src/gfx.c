#include <stdio.h>

#include "gfx.h"
#include "gfx_constants.h"
#include "utils.h"

bool gbCreateTexture(GLuint *texture, GLint internalFormat, GLsizei width, GLsizei height, GLuint pixels[])
{
    // generate the texture and set pixel data
    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    // set the wrap/repeat modes
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // set the min/mag filters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // unbind the texture so geometry isnt autmatically drawn with this texture
    glBindTexture(GL_TEXTURE_2D, 0);

    // check for errors
    if (glError("creating texture"))
        return false;

    return true;
}

bool gbCreateImageTexture(GLuint *texture, GLuint pixels[])
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
    // make sure program is actually a program
    if (glIsProgram(program))
    {
        int infoLogLength = 0;
        int maxLength = infoLogLength;

        // get the length of the info log
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        // allocate the info log
        char infoLog[maxLength];

        // get and print the log
        glGetProgramInfoLog(program, maxLength, &infoLogLength, infoLog);

        if (infoLogLength > 0)
            printf("[Program] %s\n", infoLog);
    }
    else
        printf("Given name '%d' is not a program.\n", program);
}

void gbPrintShaderLog(GLuint shader)
{
    // make sure shader is actually a shader
    if (glIsShader(shader))
    {
        int infoLogLength = 0;
        int maxLength = infoLogLength;

        // get the length of the info log
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

        // allocate the info log
        char infoLog[maxLength];

        // get and print the log
        glGetShaderInfoLog(shader, maxLength, &infoLogLength, infoLog);

        if (infoLogLength > 0)
            printf("[Shader] %s\n", infoLog);
    }
    else
        printf("Given name '%d' is not a shader.\n", shader);
}

bool gbProgramError(GLuint program, GLenum pname, const char *message)
{
    // check for program errors
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
    // check for shader errors
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