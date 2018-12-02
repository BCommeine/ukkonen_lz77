#include <stdio.h>
#include <stdlib.h>
#include <lz77/lempel_ziv.h>
#include "ukkonen_test.h"

int main(int argc, char *argv[]) {
    if (test_number_of_leaves("../tests/testfiles/test_two.txt")) {
        printf("Number of leaves:\t\tCorrect\n");
    } else {
        printf("Number of leaves:\t\tWrong\n");
    }
    if (test_all_suffixes("../tests/testfiles/test_four.txt")) {
        printf("All suffixes present:\tCorrect\n");
    } else {
        printf("All suffixes present:\tWrong");
    }
    return (0);
}