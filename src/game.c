#include "game.h"
#include "utils.h"

bool gInitialized = false;
bool gRunning = false;

SDL_Window *gWindow;
SDL_GLContext gContext;

GLuint gPaletteProgram, gPaletteVertexShader, gPaletteFragmentShader;
GLint gPaletteProgramColours, gPaletteProgramPalette;
GLfloat gColours[PAL_SIZE][3];
int gPalette[PAL_SIZE];

GBTileMap gBackgrounds[BG_COUNT];
int gActiveBackground;

void (*gRenderCallback)();

bool setupPaletteShader()
{
    // create the palette program
    gPaletteProgram = glCreateProgram();

    // create the vertex and fragment shaders
    const GLchar *vertexSource[] =
    {
        "void main() { \
             gl_Position = ftransform(); \
             gl_TexCoord[0] = gl_MultiTexCoord0; \
         }"
    };

    const GLchar *fragmentSource[] =
    {
        "uniform sampler2D texture; \
         uniform vec3 colours[4]; \
         uniform int palette[4]; \
         \
         void main() \
         { \
             vec4 texel = texture2D(texture, gl_TexCoord[0].xy); \
             int i = int((texel.r * 255.0) + 0.5); \
             vec3 p = colours[palette[i]]; \
             gl_FragColor = vec4(p.r, p.g, p.b, texel.a); \
         }"
    };

    gbCreateShader(&gPaletteVertexShader, GL_VERTEX_SHADER, vertexSource);
    gbCreateShader(&gPaletteFragmentShader, GL_FRAGMENT_SHADER, fragmentSource);

    // attach the shaders
    glAttachShader(gPaletteProgram, gPaletteVertexShader);
    glAttachShader(gPaletteProgram, gPaletteFragmentShader);

    // link the program
    glLinkProgram(gPaletteProgram);

    // check for program errors
    if (gbProgramError(gPaletteProgram, GL_LINK_STATUS, "linking program"))
        return false;

    // use the palette program
    glUseProgram(gPaletteProgram);

    // get all the uniforms
    gPaletteProgramColours = glGetUniformLocation(gPaletteProgram, "colours");
    gPaletteProgramPalette = glGetUniformLocation(gPaletteProgram, "palette");

    // say the setup was successful
    return true;
}

void setupBackground()
{
    for (int i = 0; i < BG_COUNT; i++)
    {
        GLuint *tiles = (GLuint *)calloc(BG_SIZE, sizeof(GLuint));

        gBackgrounds[i].width = BG_WIDTH;
        gBackgrounds[i].height = BG_HEIGHT;
        gBackgrounds[i].tiles = tiles;
    }
}

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

    // setup the palette shadewr
    if (!setupPaletteShader())
        return false;

    // setup the tile maps
    setupBackground();

    // check for any OpenGL errors from intializing
    if (gbGlError("initializing OpenGL"))
        return false;

    gInitialized = true;
    return true;
}

void gbSetRenderCallback(void (*callback)())
{
    gRenderCallback = callback;
}

void gbSetColours(SDL_Color colours[PAL_SIZE])
{
    // convert colours to float vec4s for GLSL
    GLfloat newColours[PAL_SIZE][3];

    for (int i = 0; i < PAL_SIZE; i++)
    {
        newColours[i][0] = colours[i].r / 255.0f;
        newColours[i][1] = colours[i].g / 255.0f;
        newColours[i][2] = colours[i].b / 255.0f;

        for (int c = 0; c < 3; c++)
            gColours[i][c] = newColours[i][c];
    }

    // set the shader colours
    glUniform3fv(gPaletteProgramColours, PAL_SIZE, (const GLfloat *)newColours);

    // update the clear colour
    gbSetPalette(gPalette);
}

void gbSetPalette(int palette[PAL_SIZE])
{
    // set the shader palette
    glUniform1iv(gPaletteProgramPalette, PAL_SIZE, palette);

    // update the clear colour
    int c = palette[PAL_WHITE];
    glClearColor(gColours[c][0], gColours[c][1], gColours[c][2], 1);

    // update gPalette
    for (int i = 0; i < PAL_SIZE; i++)
        gPalette[i] = palette[i];
}

bool verifyBgIndex(int i)
{
    if (i >= BG_COUNT)
    {
        printf("Background %i is out of range (max %i)", i, BG_COUNT - 1);
        return false;
    }

    return true;
}

GBTileMap *gbGetBackground(int i)
{
    if (!verifyBgIndex(i))
        return NULL;

    return &gBackgrounds[i];
}

bool gbSetActiveBackground(int i)
{
    if (!verifyBgIndex(i))
        return false;

    gActiveBackground = i;
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
}

// wrap an index to a count
int wrapi(int value, int max)
{
    int v = value;

    if (v < 0)
        v += max * (-v / max + 1);

    return v % max;
}

int calculateMapDrawPosition(int pos, int tileSize)
{
    int s = pos > 0 ? tileSize : -tileSize;
    int r = pos > 0 ? tileSize : 0;

    return wrapi(pos, s) - r;
}

int calculateMapStartTile(int pos, int tileSize, int mapSize)
{
    return wrapi(pos * -1 / tileSize, mapSize);
}

void renderTileMap(GBTileMap *map)
{
    // make sure theres a tile offscreen in every direction so scrolling doesnt have artifacts

    int drawX = calculateMapDrawPosition(map->x, TILE_WIDTH);
    int drawY = calculateMapDrawPosition(map->y, TILE_HEIGHT);

    int width = TILES_X + 1;
    int height = TILES_Y + 1;

    int startTileX = calculateMapStartTile(map->x, TILE_WIDTH, map->width);
    int startTileY = calculateMapStartTile(map->y, TILE_HEIGHT, map->height);

    // printf("| draw: % 2i,% 2i | start tile: %i,%i |\n", drawX, drawY, startTileX, startTileY);

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int tx = wrapi(startTileX + x, map->width);
            int ty = wrapi(startTileY + y, map->height);

            int dx = drawX + (x * TILE_WIDTH);
            int dy = drawY + (y * TILE_HEIGHT);

            glBindTexture(GL_TEXTURE_2D, map->tiles[tx + (ty * map->width)]);
            glBegin(GL_QUADS);
                glTexCoord2f(0.0f, 0.0f); glVertex2f(dx, dy);
                glTexCoord2f(1.0f, 0.0f); glVertex2f(dx + TILE_WIDTH, dy);
                glTexCoord2f(1.0f, 1.0f); glVertex2f(dx + TILE_WIDTH, dy + TILE_HEIGHT);
                glTexCoord2f(0.0f, 1.0f); glVertex2f(dx, dy + TILE_HEIGHT);
            glEnd();
        }
    }
}

void render()
{
    // clear the colour buffer
    glClear(GL_COLOR_BUFFER_BIT);

    // render the active background
    renderTileMap(gbGetBackground(gActiveBackground));

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

    gRunning = true;
    while (gRunning)
    {
        update();
        render();
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

    // delete the palette shaders and program
    glDeleteShader(gPaletteVertexShader);
    glDeleteShader(gPaletteFragmentShader);
    glDeleteProgram(gPaletteProgram);

    // quit SDL and OpenGL
    SDL_DestroyWindow(gWindow);
    SDL_GL_DeleteContext(gContext);
    SDL_Quit();

    // free all the tile maps
    for (int i = 0; i < BG_COUNT; i++)
        free(gBackgrounds[i].tiles);

    // reset the initialized state
    gInitialized = false;

    return true;
}