#pragma once

#define TILE_WIDTH 8
#define TILE_HEIGHT 8
#define TILE_SIZE TILE_WIDTH * TILE_HEIGHT

#define TILES_X 20
#define TILES_Y 18

#define SCREEN_WIDTH TILE_WIDTH * TILES_X
#define SCREEN_HEIGHT TILE_HEIGHT * TILES_Y

#define PAL_COUNT 4
#define PAL_WHITE 0
#define PAL_LGREY 1
#define PAL_DGREY 2
#define PAL_BLACK 3

#define BG_COUNT 2
#define WIN_COUNT 2

#define BG_WIDTH 32
#define BG_HEIGHT 32
#define BG_SIZE BG_WIDTH * BG_HEIGHT

// todo: bg/sprite data half shared (to match actual GB)?
#define TILE_DATA_COUNT 2
#define TILE_DATA_TILE_COUNT 256
#define TILE_DATA_BG 0
#define TILE_DATA_SPRITE 1

#define SPRITE_COUNT 40