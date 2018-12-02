#include <stdio.h>
#include <stdlib.h>
#include <lz77/lempel_ziv.h>
#include "lz77_test.h"

int main(int argc, char *argv[]) {
    if (codeer_daarna_decodeer("../tests/testfiles/test_one.txt")) {
        printf("CODEER EN DECODEER GESLAAGD");
    }
    printf("CODEER EN DECODEER GESLAAGD");
    return (0);
}