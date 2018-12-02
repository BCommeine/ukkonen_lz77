//
// Created by bert on 30/11/18.
//

#include <stdio.h>
#include <stdlib.h>
#include "ukkonen.h"

int main(int argc, char *argv[]) {
    struct Tree* tree = build_tree();
    print_and_free(tree);
}