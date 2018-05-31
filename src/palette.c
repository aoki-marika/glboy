#include "palette.h"
#include "gl.h"
#include "gfx_utils.h"

GLfloat gColours[PAL_LENGTH][3];

GLuint gPaletteProgram, gPaletteVertexShader, gPaletteFragmentShader;
GLint gPaletteProgramColours, gPaletteProgramPalette, gPaletteProgramMode;

int gBackgroundPalette[PAL_LENGTH];
int gSpritePalettes[SPRITE_PAL_COUNT][PAL_LENGTH];

bool gbPaletteInit()
{
    gPaletteProgram = glCreateProgram();

    const GLchar *vertexSource[] =
    {
        " \
        void main() { \
            gl_Position = ftransform(); \
            gl_TexCoord[0] = gl_MultiTexCoord0; \
        }"
    };

    // todo: window colour 0 below sprites
    const GLchar *fragmentSource[] =
    {
        " \
        uniform sampler2D texture; \
        uniform vec3 colours[4]; \
        uniform int palette[4]; \
        uniform int mode; \
        uniform int modeBg, modeWin, modeSprite; \
        \
        void main() \
        { \
            vec4 t = texture2D(texture, gl_TexCoord[0].xy); \
            int i = int((t.r * 255.0) + 0.5); \
            vec3 c = colours[palette[i]]; \
            \
            float a = t.a; \
            if (mode == modeSprite && i == 0) \
                a = 0.0; \
            \
            gl_FragColor = vec4(c.r, c.g, c.b, a); \
            \
            if ((mode == modeBg || mode == modeSprite) && i == 0) \
                gl_FragDepth = 1.0; \
            else \
                gl_FragDepth = gl_FragCoord.z; \
        }"
    };

    gbCreateShader(&gPaletteVertexShader, GL_VERTEX_SHADER, vertexSource);
    gbCreateShader(&gPaletteFragmentShader, GL_FRAGMENT_SHADER, fragmentSource);

    glAttachShader(gPaletteProgram, gPaletteVertexShader);
    glAttachShader(gPaletteProgram, gPaletteFragmentShader);

    glLinkProgram(gPaletteProgram);

    if (gbProgramError(gPaletteProgram, GL_LINK_STATUS, "linking program"))
        return false;

    glUseProgram(gPaletteProgram);

    // get all the uniforms
    gPaletteProgramColours = glGetUniformLocation(gPaletteProgram, "colours");
    gPaletteProgramPalette = glGetUniformLocation(gPaletteProgram, "palette");
    gPaletteProgramMode = glGetUniformLocation(gPaletteProgram, "mode");

    // set the mode values
    glUniform1i(glGetUniformLocation(gPaletteProgram, "modeBg"), GBPaletteModeBackground);
    glUniform1i(glGetUniformLocation(gPaletteProgram, "modeWin"), GBPaletteModeWindow);
    glUniform1i(glGetUniformLocation(gPaletteProgram, "modeSprite"), GBPaletteModeSprite);

    return true;
}

bool gbPaletteQuit()
{
    glDeleteShader(gPaletteVertexShader);
    glDeleteShader(gPaletteFragmentShader);
    glDeleteProgram(gPaletteProgram);

    return true;
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

void setShaderPalette(int palette[PAL_LENGTH])
{
    glUniform1iv(gPaletteProgramPalette, PAL_LENGTH, palette);
}

bool gbSetActivePalette(GBPaletteType type, int index)
{
    if (type == GBPaletteTypeBackground)
    {
        setShaderPalette(gBackgroundPalette);
    }
    else if (type == GBPaletteTypeSprite)
    {
        if (index >= SPRITE_PAL_COUNT)
        {
            printf("Sprite palette index %i is out of range (%i).", index, SPRITE_PAL_COUNT);
            return false;
        }

        setShaderPalette(gSpritePalettes[index]);
    }

    return true;
}

void gbSetPaletteMode(GBPaletteMode mode)
{
    glUniform1i(gPaletteProgramMode, mode);
}