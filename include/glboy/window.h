#pragma once

#include "tile.h"

GBTileMap *gbGetWindow(int index);
GBTileMap *gbGetActiveWindow();
bool gbSetActiveWindow(int index);

bool gbRenderWindow();