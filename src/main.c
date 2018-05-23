#include <stdio.h>

#include "gb.h"

int main(int argc, char *argv[])
{
    if (!gbInit())
        return 1;

    gbQuit();
    return 0;
}