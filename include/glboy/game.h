#pragma once

#include <stdbool.h>

bool gbInit();
bool gbRun();
bool gbQuit();

void gbSetUpdateCallback(void (*callback)());
void gbSetRenderCallback(void (*callback)());