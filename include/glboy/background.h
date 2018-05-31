#pragma once

#include "tile.h"

bool gbBackgroundInit();
bool gbBackgroundQuit();

GBTileMap *gbGetBackground(int index);
GBTileMap *gbGetActiveBackground();
bool gbSetActiveBackground(int index);