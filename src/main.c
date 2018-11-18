#include <stdio.h>
#include <stdlib.h>
#include "ukkonen.h"

#define FILE_NAME "./test.txt"

int main(int argc, char *argv[]) {
        struct Tree* tree = build_tree();
        print_and_free(tree);
}