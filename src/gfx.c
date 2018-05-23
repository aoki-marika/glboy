#include "gfx.h"
#include "gfx_constants.h"
#include "utils.h"

bool gbCreateTexture(GLuint *texture, GLuint pixels[])
{
    // generate the texture and set pixel data
    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, TILE_WIDTH, TILE_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

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