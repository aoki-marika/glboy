#include "input.h"

GBInputState lastState;

GBInputState gbGetInputState()
{
    const Uint8 *s = SDL_GetKeyboardState(NULL);

    bool up = s[SCANCODE_UP];
    bool down = s[SCANCODE_DOWN];
    bool left = s[SCANCODE_LEFT];
    bool right = s[SCANCODE_RIGHT];

    bool a = s[SCANCODE_A];
    bool b = s[SCANCODE_B];
    bool start = s[SCANCODE_START];
    bool select = s[SCANCODE_SELECT];

    GBInputState state = {
        .up = {
            .pressed = up,
            .repeat = up && lastState.up.pressed,
        },
        .down = {
            .pressed = down,
            .repeat = down && lastState.down.pressed,
        },
        .left = {
            .pressed = left,
            .repeat = left && lastState.left.pressed,
        },
        .right = {
            .pressed = right,
            .repeat = right && lastState.right.pressed,
        },
        .a = {
            .pressed = a,
            .repeat = a && lastState.a.pressed,
        },
        .b = {
            .pressed = b,
            .repeat = b && lastState.b.pressed,
        },
        .start = {
            .pressed = start,
            .repeat = start && lastState.start.pressed,
        },
        .select = {
            .pressed = select,
            .repeat = select && lastState.select.pressed,
        },
    };

    lastState = state;
    return state;
}