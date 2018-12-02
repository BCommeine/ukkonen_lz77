//
// Created by bert on 1/12/18.
//

#include <memory.h>
#include <stdio.h>
#include "lz77_test.h"


bool codeer_daarna_decodeer(char* filename) {
    freopen("../tests/testfiles/test_one.txt", "r", stdin);
    freopen("../tests//testfiles/output", "w", stdout);
    codeer_lz();
    freopen("../tests/testfiles/output", "r", stdin);
    freopen("../tests//testfiles/result", "w", stdout);
    decodeer_lz();
    freopen ("/dev/tty", "w", stdout);
    //    char* command = strcat(filename, "diff ../tests/testfiles/result ");
//    system(command);
}
