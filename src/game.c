#include "game.h"
#include "sdl.h"
#include "gl.h"
#include "palette.h"
#include "tile.h"
#include "background.h"
#include "window.h"
#include "sprite.h"
#include "gfx_utils.h"

bool gInitialized = false;
bool gRunning = false;

SDL_Window *gWindow;
SDL_GLContext gGlContext;

void (*gRenderCallback)(), (*gUpdateCallback)();

bool gbInit()
{
    if (gInitialized)
    {
        printf("Cannot initialize GlBoy when it was already initialized.\n");
        return false;
    }

    // init SDL and get a window
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    gWindow = SDL_CreateWindow("GlBoy",
                               SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                               SCREEN_WIDTH, SCREEN_HEIGHT,
                               SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    if (gbSdlError("initializing SDL"))
        return false;

    // set the preferred OpenGL version
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    // get the OpenGL context
    gGlContext = SDL_GL_CreateContext(gWindow);

    printf("Using OpenGL %s and GLSL %s.\n", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(0.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0, 1.0, -1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);

    glDepthFunc(GL_LEQUAL);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (!gbPaletteInit() ||
        !gbTileInit() ||
        !gbBackgroundInit())
        return false;

    // check for any OpenGL errors from intializing
    if (gbGlError("initializing OpenGL"))
        return false;

    gInitialized = true;
    return true;
}

void update()
{
    SDL_Event event;

    while (SDL_PollEvent(&event) != 0)
    {
        switch (event.type)
        {
            case SDL_QUIT:
                gRunning = false;
                return;
            default:
                break;
        }
    }

    if (gUpdateCallback)
        gUpdateCallback();
}

// todo: more errors/safety in render

void render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // render all the layers
    gbRenderBackground();
    gbRenderWindow();
    gbRenderSprites();

    // reset the palette mode
    gbSetPaletteMode(GBPaletteModeBackground);

    if (gRenderCallback)
        gRenderCallback();

    // display the new frame
    SDL_GL_SwapWindow(gWindow);
}

bool gbRun()
{
    if (!gInitialized)
    {
        printf("Cannot run GlBoy when it was not initialized.\n");
        return false;
    }

    if (gRunning)
    {
        printf("Cannot run GlBoy when it is already running.\n");
        return false;
    }

    Uint32 frameStart;
    gRunning = true;

    while (gRunning)
    {
        frameStart = SDL_GetTicks();

        update();
        render();

        Uint32 frameDuration = SDL_GetTicks() - frameStart;
        if (frameDuration < SCREEN_FRAME_MS)
            SDL_Delay(SCREEN_FRAME_MS - frameDuration);
    }

    return true;
}

bool gbQuit()
{
    if (!gInitialized)
    {
        printf("Cannot quit GlBoy when it was not initialized.\n");
        return false;
    }

    // stop the program if its running
    gRunning = false;

    // quit the palette system
    if (!gbPaletteQuit())
        return false;

    // quit SDL and OpenGL
    SDL_DestroyWindow(gWindow);
    SDL_GL_DeleteContext(gGlContext);
    SDL_Quit();

    // quit all the other systems
    if (!gbBackgroundQuit() ||
        !gbTileQuit() ||
        !gbSpriteQuit())
        return false;

    gInitialized = false;
    return true;
}

void gbSetUpdateCallback(void (*callback)())
{
    gUpdateCallback = callback;
}

void gbSetRenderCallback(void (*callback)())
{
    gRenderCallback = callback;
}