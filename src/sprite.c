#include "sprite.h"
#include "palette.h"
#include "tile.h"
#include "gfx_constants.h"

GBSprite *gActiveSprites[SPRITE_COUNT];
int gActiveSpriteCount;

bool gbSpriteQuit()
{
    for (int i = 0; i < gActiveSpriteCount; i++)
        gbRemoveSprite(gActiveSprites[i]);

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

void gbRenderSprites()
{
    int currentPalete = -1;
    gbSetPaletteMode(GBPaletteModeSprite);

    for (int i = 0; i < gActiveSpriteCount; i++)
    {
        GBSprite *s = gActiveSprites[i];

        // apply the palette
        if (s->palette != currentPalete)
        {
            currentPalete = s->palette;
            gbSetActivePalette(GBPaletteTypeSprite, currentPalete);
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
        gbRenderTile(TILE_DATA_SPRITE, s->tile, s->x, s->y, z, s->flipX, s->flipY);
    }
}