#include <stdio.h>
#include <stdlib.h>
#include "lempel_ziv.h"

int main(int argc, char *argv[]) {
    bool opt = false;
    while ((argc > 1) && (argv[1][0] == '-'))
    {
        switch (argv[1][1])
        {
            case 'c':
                codeer_lz(opt);
                break;

            case 'd':
                decodeer_lz(opt);
                break;

            case 'o':
                opt = true;
                break;

            default:
                printf("Wrong Argument: %s\n", argv[1]);
                break;
        }

        ++argv;
        --argc;
    }
    return (0);
}