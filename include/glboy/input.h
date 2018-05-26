#include <stdbool.h>

#include "input_constants.h"

typedef struct
{
    bool pressed;
    bool repeat;
} GBButtonState;

typedef struct
{
    GBButtonState up, down, left, right;
    GBButtonState a, b, start, select;
} GBInputState;

GBInputState gbGetInputState();