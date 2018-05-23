#include <stdio.h>

#include "gb.h"

int main(int argc, char *argv[])
{
    if (!gbInit())
        return 1;

    if (!gbRun())
        return 1;

    if (!gbQuit())
        return 1;

    return 0;
}