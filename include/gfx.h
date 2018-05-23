#pragma once

#include <stdbool.h>

#include "gl.h"

bool gbCreateTexture(GLuint *texture, GLint internalFormat, GLsizei width, GLsizei height, GLuint pixels[]);
bool gbCreateImageTexture(GLuint *texture, GLuint pixels[]);
bool gbCreateShader(GLuint *shader, GLenum type, const GLchar *source[]);

void gbPrintProgramLog(GLuint program);
void gbPrintShaderLog(GLuint shader);
bool gbProgramError(GLuint program, GLenum pname, const char *message);
bool gbShaderError(GLuint shader, GLenum pname, const char *message);