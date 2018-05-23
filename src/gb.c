#include "gb.h"
#include "sdl.h"
#include "gl.h"
#include "gfx_constants.h"

SDL_Window *gWindow;
SDL_GLContext gContext;

bool glError(const char *message)
{
    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        printf("Error %s! %u\n", message, error);
        return true;
    }

    return false;
}

bool sdlError(const char *message)
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

bool gbInit()
{
    // setup SDL
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    // create the main window
    gWindow = SDL_CreateWindow("GlBoy",
                               SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                               SCREEN_WIDTH, SCREEN_HEIGHT,
                               SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    if (sdlError("initializing SDL"))
        return false;

    // get the OpenGL context
    gContext = SDL_GL_CreateContext(gWindow);
    if (sdlError("initializing OpenGL context"))
        return false;

    // setup the scene with defaults
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glOrtho(0.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0, 1.0, -1.0);
    glClearColor(1, 1, 1, 1);

    // initialize the matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // check for any OpenGL errors from intializing
    if (glError("initializing OpenGL"))
        return false;

    // say we succesfully initalized
    return true;
}

void gbQuit()
{
    // quit SDL and OpenGL
    SDL_DestroyWindow(gWindow);
    SDL_GL_DeleteContext(gContext);
    SDL_Quit();
}