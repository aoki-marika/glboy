#include "game.h"
#include "utils.h"
#include "input_constants.h"

typedef enum
{
    GBShaderTypeBg = 0,
    GBShaderTypeWin = 1,
    GBShaderTypeSprite = 2,
} GBShaderType;

bool gInitialized = false;
bool gRunning = false;

SDL_Window *gWindow;
SDL_GLContext gContext;

GLfloat gColours[PAL_LENGTH][3];
GLuint gPaletteProgram, gPaletteVertexShader, gPaletteFragmentShader;
GLint gPaletteProgramColours, gPaletteProgramPalette, gPaletteProgramType;
int gBackgroundPalette[PAL_LENGTH];
int gSpritePalettes[SPRITE_PAL_COUNT][PAL_LENGTH];

GLuint *gTileData[TILE_DATA_COUNT];

GBTileMap gBackgrounds[BG_COUNT];
int gActiveBackground;

GBTileMap gWindows[WIN_COUNT];
int gActiveWindow;

GBSprite *gActiveSprites[SPRITE_COUNT];
int gActiveSpriteCount;

void (*gRenderCallback)(), (*gUpdateCallback)();

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

    //todo: window colour 0 below sprites
    const GLchar *fragmentSource[] =
    {
        "uniform sampler2D texture; \
         uniform vec3 colours[4]; \
         uniform int palette[4]; \
         uniform int type; \
         uniform int typeBg, typeWin, typeSprite; \
         \
         void main() \
         { \
             vec4 t = texture2D(texture, gl_TexCoord[0].xy); \
             int i = int((t.r * 255.0) + 0.5); \
             vec3 c = colours[palette[i]]; \
             \
             float a = t.a; \
             if (type == typeSprite && i == 0) \
                a = 0.0; \
             \
             gl_FragColor = vec4(c.r, c.g, c.b, a); \
             \
             if ((type == typeBg || type == typeSprite) && i == 0) \
                gl_FragDepth = 1.0; \
             else \
                gl_FragDepth = gl_FragCoord.z; \
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
    gPaletteProgramType = glGetUniformLocation(gPaletteProgram, "type");

    // set the type values
    glUniform1i(glGetUniformLocation(gPaletteProgram, "typeBg"), GBShaderTypeBg);
    glUniform1i(glGetUniformLocation(gPaletteProgram, "typeWin"), GBShaderTypeWin);
    glUniform1i(glGetUniformLocation(gPaletteProgram, "typeSprite"), GBShaderTypeSprite);

    // say the setup was successful
    return true;
}

void setupTileData()
{
    for (int i = 0; i < TILE_DATA_COUNT; i++)
        gTileData[i] = (GLuint *)calloc(TILE_DATA_TILE_COUNT, sizeof(GLuint));
}

void setupBackgrounds()
{
    for (int i = 0; i < BG_COUNT; i++)
    {
        int *tiles = (int *)calloc(BG_SIZE, sizeof(int));

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
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);

    glDepthFunc(GL_LEQUAL);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // setup the palette shadewr
    if (!setupPaletteShader())
        return false;

    // setup the tile data and maps
    setupTileData();
    setupBackgrounds();

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

void gbSetColours(SDL_Color colours[PAL_LENGTH])
{
    // convert colours to float vec4s for GLSL
    GLfloat newColours[PAL_LENGTH][3];

    for (int i = 0; i < PAL_LENGTH; i++)
    {
        newColours[i][0] = colours[i].r / 255.0f;
        newColours[i][1] = colours[i].g / 255.0f;
        newColours[i][2] = colours[i].b / 255.0f;

        for (int c = 0; c < 3; c++)
            gColours[i][c] = newColours[i][c];
    }

    // set the shader colours
    glUniform3fv(gPaletteProgramColours, PAL_LENGTH, (const GLfloat *)newColours);

    // update the clear colour
    gbSetBackgroundPalette(gBackgroundPalette);
}

void gbSetBackgroundPalette(int palette[PAL_LENGTH])
{
    // update the palette
    for (int i = 0; i < PAL_LENGTH; i++)
        gBackgroundPalette[i] = palette[i];

    // update the clear colour
    int c = palette[PAL_WHITE];
    glClearColor(gColours[c][0], gColours[c][1], gColours[c][2], 1);
}

bool gbSetSpritePalette(int index, int palette[PAL_LENGTH])
{
    if (index >= SPRITE_PAL_COUNT)
    {
        printf("Sprite palette index %i is out of range (%i).\n", index, SPRITE_PAL_COUNT);
        return false;
    }

    // update the palette
    for (int i = 0; i < PAL_LENGTH; i++)
        gSpritePalettes[index][i] = palette[i];

    return true;
}

bool verifyTypeIndex(int type)
{
    if (type >= TILE_DATA_COUNT)
    {
        printf("Tile data type %i is out of range (%i).\n", type, TILE_DATA_COUNT);
        return false;
    }

    return true;
}

bool gbSetTileData(int type, int index, GLuint data[TILE_SIZE])
{
    if (!verifyTypeIndex(type))
        return false;

    if (index >= TILE_DATA_TILE_COUNT)
    {
        printf("Tile data index %i is out of range (%i).\n", index, TILE_DATA_TILE_COUNT);
        return false;
    }

    GLuint *tile = &gTileData[type][index];

    // delete the existing texture (if there is one) and create the new one
    glDeleteTextures(1, tile);
    gbCreateImageTexture(tile, data);

    return true;
}

bool gbSetTileDataMultiple(int type, int index, int count, GLuint data[count * TILE_SIZE])
{
    if (!verifyTypeIndex(type))
        return false;

    for (int i = 0; i < count; i++)
    {
        GLuint d[TILE_SIZE];

        for (int di = 0; di < TILE_SIZE; di++)
            d[di] = data[(i * TILE_SIZE) + di];

        if (!gbSetTileData(type, index + i, d))
            return false;
    }

    return true;
}

bool verifyBgIndex(int index)
{
    if (index >= BG_COUNT)
    {
        printf("Background %i is out of range (%i).\n", index, BG_COUNT);
        return false;
    }

    return true;
}

GBTileMap *gbGetBackground(int index)
{
    if (!verifyBgIndex(index))
        return NULL;

    return &gBackgrounds[index];
}

bool gbSetActiveBackground(int index)
{
    if (!verifyBgIndex(index))
        return false;

    gActiveBackground = index;
    return true;
}

bool verifyWinIndex(int index)
{
    if (index >= WIN_COUNT)
    {
        printf("Window %i is out of range (%i).\n", index, WIN_COUNT);
        return false;
    }

    return true;
}

GBTileMap *gbGetWindow(int index)
{
    if (!verifyWinIndex(index))
        return NULL;

    return &gWindows[index];
}

bool gbSetActiveWindow(int index)
{
    if (!verifyWinIndex(index))
        return false;

    gActiveWindow = index;
    return true;
}

bool gbAddSprite(GBSprite *sprite)
{
    if (gActiveSpriteCount >= SPRITE_COUNT)
    {
        printf("Cannot add sprite, already at %i sprites.\n", SPRITE_COUNT);
        return false;
    }

    gActiveSprites[gActiveSpriteCount] = sprite;
    gActiveSpriteCount++;

    return true;
}

bool removeInactiveSpriteError()
{
    printf("Cannot remove a sprite that is not active.\n");
    return false;
}

bool gbRemoveSprite(GBSprite *sprite)
{
    // dont bother with anything if there arent any active sprites
    if (gActiveSpriteCount >= SPRITE_COUNT)
        return removeInactiveSpriteError();

    // find the sprite
    int spriteIndex = -1;

    for (int i = 0; i < SPRITE_COUNT; i++)
    {
        if (gActiveSprites[i] == sprite)
        {
            spriteIndex = i;
            break;
        }
    }

    // if the sprite wasnt active, return an error
    if (spriteIndex == -1)
        return removeInactiveSpriteError();

    // shift back the elements to remove the sprite
    for (int i = spriteIndex; i < SPRITE_COUNT - 1; i++)
        gActiveSprites[i] = gActiveSprites[i + 1];

    gActiveSpriteCount--;
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

int wrapIndex(int index, int length)
{
    int i = index;
    if (i < 0)
        i += length * (-i / length + 1);

    return i % length;
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
    return wrapIndex(-pos / tileSize, mapSize);
}

float scaleForFlip(bool flip)
{
    return flip ? -1.0f : 1.0f;
}

float translateForFlip(bool flip, int pos, int size)
{
    return flip ? -pos * 2 - size : 0.0f;
}

void renderTile(int dataType, int dataIndex, int x, int y, float z, bool flipX, bool flipY)
{
    glBindTexture(GL_TEXTURE_2D, gTileData[dataType][dataIndex]);

    if (flipX || flipY)
    {
        glPushMatrix();
            glScalef(scaleForFlip(flipX),
                     scaleForFlip(flipY),
                     1.0f);
            glTranslatef(translateForFlip(flipX, x, TILE_WIDTH),
                         translateForFlip(flipY, y, TILE_HEIGHT),
                         0.0f);
    }

    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(x + TILE_WIDTH, y, z);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(x + TILE_WIDTH, y + TILE_HEIGHT, z);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + TILE_HEIGHT, z);
    glEnd();

    if (flipX || flipY)
        glPopMatrix();
}

// todo: stutter when moving around 0,0

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

                renderTile(dataType, map->tiles[tx + (ty * map->width)], dx, dy, Z_BG, false, false);
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

                renderTile(dataType, map->tiles[x + (y * map->width)], dx, dy, Z_BG, false, false);
            }
        }
    }
}

void setShaderPalette(int palette[PAL_LENGTH])
{
    glUniform1iv(gPaletteProgramPalette, PAL_LENGTH, palette);
}

void setShaderType(int type)
{
    glUniform1i(gPaletteProgramType, type);
}

//todo: verify values from structs

void render()
{
    // clear the colour and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set the palette for the BG and window
    setShaderPalette(gBackgroundPalette);

    // render the active background
    setShaderType(GBShaderTypeBg);
    renderTileMap(gbGetBackground(gActiveBackground), TILE_DATA_BG, true);

    // render the active window
    setShaderType(GBShaderTypeWin);
    renderTileMap(gbGetWindow(gActiveWindow), TILE_DATA_BG, false);

    // render the active sprites
    int currentSpritePalette = -1;
    setShaderType(GBShaderTypeSprite);

    for (int i = 0; i < gActiveSpriteCount; i++)
    {
        GBSprite *s = gActiveSprites[i];

        // apply the palette
        if (s->palette != currentSpritePalette)
        {
            currentSpritePalette = s->palette;
            setShaderPalette(gSpritePalettes[currentSpritePalette]);
        }

        float z;
        switch (s->priority)
        {
            case GBSpritePriorityAbove:
                z = Z_ABOVE;
                break;
            case GBSpritePriorityBelow:
                z = Z_BELOW;
                break;
            default:
                break;
        }

        // render the sprite
        renderTile(TILE_DATA_SPRITE, s->tile, s->x, s->y, z, s->flipX, s->flipY);
    }

    // reset the shader type
    setShaderType(GBShaderTypeBg);

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

    // free all the tile data
    for (int i = 0; i < TILE_DATA_COUNT; i++)
    {
        glDeleteTextures(TILE_DATA_TILE_COUNT, gTileData[i]);
        free(gTileData[i]);
    }

    //todo: free all active sprites?

    // reset the initialized state
    gInitialized = false;

    return true;
}