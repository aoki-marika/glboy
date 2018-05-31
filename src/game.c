#include "game.h"
#include "palette.h"
#include "background.h"
#include "window.h"
#include "tile.h"
#include "sprite.h"
#include "input_constants.h"

bool gInitialized = false;
bool gRunning = false;

SDL_Window *gWindow;
SDL_GLContext gContext;

void (*gRenderCallback)(), (*gUpdateCallback)();

bool gbInit()
{
    if (gInitialized)
    {
        printf("Cannot initialize GlGB when it was already initialized.\n");
        return false;
    }

    // setup SDL
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    // create the main window
    gWindow = SDL_CreateWindow("GlBoy",
                               SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                               SCREEN_WIDTH, SCREEN_HEIGHT,
                               SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    if (gbSdlError("initializing SDL"))
        return false;

    // set the OpenGL context version
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    // get the OpenGL context
    gContext = SDL_GL_CreateContext(gWindow);
    if (gbSdlError("initializing OpenGL context"))
        return false;

    // print out the OpenGL and GLSL versions
    printf("Using OpenGL %s and GLSL %s.\n", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));

    // setup the viewport
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // initialize the projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // setup the coordinates so 0,0 is the top-left corner
    glOrtho(0.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0, 1.0, -1.0);

    // initialize the model view matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // setup the scene
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

void gbSetUpdateCallback(void (*callback)())
{
    gUpdateCallback = callback;
}

void gbSetRenderCallback(void (*callback)())
{
    gRenderCallback = callback;
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

int wrapIndex(int index, int length)
{
    if (index > 0)
        return index % length;
    else
        return (index % length + length) % length;
}

int calculateMapDrawPosition(int pos, int tileSize)
{
    if (pos > 0)
        return wrapIndex(pos, tileSize) - tileSize;
    else
        return wrapIndex(pos, -tileSize);
}

int calculateMapStartTile(int pos, int tileSize, int mapSize)
{
    float p = -pos / tileSize;

    if (pos > 0)
        p -= 1;

    return wrapIndex(p, mapSize);
}

void renderTileMap(GBTileMap *map, int dataType, bool wrap)
{
    if (wrap)
    {
        int startDrawX = calculateMapDrawPosition(map->x, TILE_WIDTH);
        int startDrawY = calculateMapDrawPosition(map->y, TILE_HEIGHT);

        int startTileX = calculateMapStartTile(map->x, TILE_WIDTH, map->width);
        int startTileY = calculateMapStartTile(map->y, TILE_HEIGHT, map->height);

        // add 1 to TILES_X/Y so that there is always one tile offscreen for smooth scrolling

        for (int y = 0; y < TILES_Y + 1; y++)
        {
            for (int x = 0; x < TILES_X + 1; x++)
            {
                int tx = wrapIndex(startTileX + x, map->width);
                int ty = wrapIndex(startTileY + y, map->height);

                int dx = startDrawX + (x * TILE_WIDTH);
                int dy = startDrawY + (y * TILE_HEIGHT);

                gbRenderTile(dataType, map->tiles[tx + (ty * map->width)], dx, dy, Z_BG, false, false);
            }
        }
    }
    else
    {
        // non-wrapped maps are typically only onscreen, so dont bother clipping

        for (int y = 0; y < map->height; y++)
        {
            for (int x = 0; x < map->width; x++)
            {
                int dx = map->x + (x * TILE_WIDTH);
                int dy = map->y + (y * TILE_HEIGHT);

                gbRenderTile(dataType, map->tiles[x + (y * map->width)], dx, dy, Z_BG, false, false);
            }
        }
    }
}

// todo: more errors/safety in render

void render()
{
    // clear the colour and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set the palette for the BG and window
    gbSetActivePalette(GBPaletteTypeBackground, 0);

    // render the active background
    gbSetPaletteMode(GBPaletteModeBackground);
    renderTileMap(gbGetActiveBackground(), TILE_DATA_BG, true);

    // render the active window
    gbSetPaletteMode(GBPaletteModeWindow);
    renderTileMap(gbGetActiveWindow(), TILE_DATA_BG, false);

    // render the active sprites
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
        printf("Cannot run GlGB when it was not initialized.\n");
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
        printf("Cannot quit GlGB when it was not initialized.\n");
        return false;
    }

    // stop the program if its running
    gRunning = false;

    if (!gbPaletteQuit())
        return false;

    // quit SDL and OpenGL
    SDL_DestroyWindow(gWindow);
    SDL_GL_DeleteContext(gContext);
    SDL_Quit();

    if (!gbBackgroundQuit())
        return false;

    if (!gbTileQuit())
        return false;

    if (!gbSpriteQuit())
        return false;

    // reset the initialized state
    gInitialized = false;

    return true;
}