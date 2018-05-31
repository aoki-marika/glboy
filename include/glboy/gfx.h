#pragma once

#include <stdbool.h>

typedef struct
{
    int width;
    int height;
    int x;
    int y;
    int *tiles;
} GBTileMap;

typedef enum
{
    GBSpritePriorityAbove = 0, //on top of the window and background
    GBSpritePriorityBelow = 1, //behind the window and colours 1-3 of the background (not 0)
} GBSpritePriority;

typedef struct
{
    int x;
    int y;
    int tile;
    int palette; //index of the sprite palette to use
    bool flipX;
    bool flipY;
    GBSpritePriority priority;
} GBSprite;