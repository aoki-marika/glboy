#include "game.h"
#include "utils.h"
#include "gfx.h"

bool gInitialized = false;
bool gRunning = false;

SDL_Window *gWindow;
SDL_GLContext gContext;
GLuint gPaletteProgram, gPaletteVertexShader, gPaletteFragmentShader;
GLfloat gColours[PAL_SIZE][3];

void (*gRenderCallback)();

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

    if (sdlError("initializing SDL"))
        return false;

    // set the OpenGL context version
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    // get the OpenGL context
    gContext = SDL_GL_CreateContext(gWindow);
    if (sdlError("initializing OpenGL context"))
        return false;

    // print out the OpenGL and GLSL versions
    printf("Running OpenGL %s and GLSL %s.\n", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));

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
    if (gbProgramError(gPaletteProgram, "linking program"))
        return false;

    // use the palette program
    glUseProgram(gPaletteProgram);

    // check for any OpenGL errors from intializing
    if (glError("initializing OpenGL"))
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

    glUniform3fv(glGetUniformLocation(gPaletteProgram, "colours"), PAL_SIZE, (const GLfloat *)newColours);
    glClearColor(newColours[PAL_WHITE][0],
                 newColours[PAL_WHITE][1],
                 newColours[PAL_WHITE][2],
                 1);
}

void gbSetPalette(int palette[PAL_SIZE])
{
    glUniform1iv(glGetUniformLocation(gPaletteProgram, "palette"), PAL_SIZE, palette);

    int i = palette[PAL_WHITE];
    glClearColor(gColours[i][0], gColours[i][1], gColours[i][2], 1);
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

void render()
{
    // clear the colour buffer
    glClear(GL_COLOR_BUFFER_BIT);

    // todo: not sure how this will work out in real usage, needs testing
    GLint tex = glGetUniformLocation(gPaletteProgram, "texture");
    glUniform1i(tex, 0);
    glActiveTexture(GL_TEXTURE0);

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

    // delete the palette texture and program
    glDeleteShader(gPaletteVertexShader);
    glDeleteShader(gPaletteFragmentShader);
    glDeleteProgram(gPaletteProgram);

    // quit SDL and OpenGL
    SDL_DestroyWindow(gWindow);
    SDL_GL_DeleteContext(gContext);
    SDL_Quit();

    // reset the initialized state
    gInitialized = false;

    return true;
}