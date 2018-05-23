#pragma once

#include "gl.h"

void gbLoadImage(unsigned char bytes[], int pixels[]);
void gbConvertImage(int pixels[], GLuint convertedPixels[]); //temporary