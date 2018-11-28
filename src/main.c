#include <stdio.h>
#include <stdlib.h>
#include "lempel_ziv.h"
//#include "../tests/ukkonen_test.h"

#define FILE_NAME "./test.txt"

int main(int argc, char *argv[]) {
//    freopen("../src/test.txt", "r", stdin);
//    freopen("../src/coded", "w", stdout);
//    codeer_lz();

    freopen("../src/coded", "r", stdin);
    decodeer_lz();
}