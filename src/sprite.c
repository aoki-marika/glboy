#include "sprite.h"
#include "palette.h"
#include "tile.h"
#include "gfx_constants.h"

GBSpriteMode gSpriteMode;

GBSprite *gActiveSprites[SPRITE_COUNT];
int gActiveSpriteCount;

bool gbSpriteQuit()
{
    for (int i = 0; i < gActiveSpriteCount; i++)
        gbRemoveSprite(gActiveSprites[i]);

    return true;
}

void gbSetSpriteMode(GBSpriteMode mode)
{
    gSpriteMode = mode;
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

bool verifyTileIndex(int index)
{
    int count = TILE_DATA_TILE_COUNT;

    if (gSpriteMode == GBSpriteMode8x16)
        count /= 2;

    return index < count;
}

bool gbRenderSprites()
{
    int currentPalete = -1;
    gbSetPaletteMode(GBPaletteModeSprite);

    for (int i = 0; i < gActiveSpriteCount; i++)
    {
        GBSprite *s = gActiveSprites[i];

        // set the active palette
        if (s->palette != currentPalete)
        {
            currentPalete = s->palette;

            if (!gbSetActivePalette(GBPaletteTypeSprite, currentPalete))
                return false;
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
        }

        if (!verifyTileIndex(s->tile))
            return false;

        // render the sprite
        if (!gbRenderTile(TILE_DATA_SPRITE, s->tile, s->x, s->y, z, s->flipX, s->flipY))
            return false;

        // render the second tile if we are in 8x16 sprite mode
        if (gSpriteMode == GBSpriteMode8x16)
            if (!gbRenderTile(TILE_DATA_SPRITE, s->tile + 1, s->x, s->y + TILE_HEIGHT, z, s->flipX, s->flipY))
                return false;
    }

    return true;
}