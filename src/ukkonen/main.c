//
// Created by bert on 30/11/18.
//

#include <stdio.h>
#include <stdlib.h>
#include "ukkonen.h"

int main(int argc, char *argv[]) {
    freopen("./data", "r", stdin);
    struct Tree* tree = build_tree();
    printf("%s", tree->code);
    print_and_free(tree);
}