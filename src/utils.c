#include "utils.h"
#include "gl.h"
#include "sdl.h"

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